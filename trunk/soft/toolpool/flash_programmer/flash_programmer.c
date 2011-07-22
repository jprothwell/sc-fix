
#include "cs_types.h"

#include "flash_prog_map.h"

#include "tgt_memd_cfg.h"
#include "memd_m.h"
#include "pmd_m.h"
#include "boot_usb_monitor.h"

#include "hal_sys.h"
#include "hal_debug.h"
#include "hal_dma.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "hal_map_engine.h"

// define the folowing to see the error code returned by the flash driver as host events
#define DEBUG_EVENTS
#ifdef DEBUG_EVENTS
#include "hal_host.h"
#endif

extern VOID mon_Event(UINT32 ch);
extern VOID boot_HostUsbEvent(UINT32 ch);


#if (CHIP_HAS_USB == 1) && (FASTPF_USE_USB == 1)
  #define SEND_EVENT boot_HostUsbEvent
#else
  #define SEND_EVENT mon_Event
#endif

#ifndef FLASH_USB_SEND_EVENT_DELAY
#define FLASH_USB_SEND_EVENT_DELAY    8192 // 0.5s
#endif /* FLASH_USB_SEND_EVENT_DELAY */

#define FP_DATA_BUFFER_LOCATION __attribute__((section(".ramucbss")))

BOOT_HOST_USB_MONITOR_CTX_T g_flashProgUsbContext;

//-----------------------------
// Three Data Buffers are available for CoolWatcher
//-----------------------------
#if (FPC_BUFFER_SIZE/4*4 != FPC_BUFFER_SIZE)
#error "FPC_BUFFER_SIZE should be aligned with word boundary"
#endif

PROTECTED UINT32 FP_DATA_BUFFER_LOCATION g_dataBufferA[FPC_BUFFER_SIZE/4];
PROTECTED UINT32 FP_DATA_BUFFER_LOCATION g_dataBufferB[FPC_BUFFER_SIZE/4];
PROTECTED UINT32 FP_DATA_BUFFER_LOCATION g_dataBufferC[FPC_BUFFER_SIZE/4];

EXPORT PUBLIC UINT32 _boot_loader_magic_number_in_flash;
 
PROTECTED HAL_MAP_VERSION_T g_flashProgVersion = {0, 0x20080121, 0, "Flash Programmer Version 1"};
PUBLIC FLASH_PROG_MAP_ACCESS_T g_flashProgDescr =
    { {FPC_PROTOCOL_MAJOR,FPC_PROTOCOL_MINOR},{{FPC_NONE, (UINT32)0, 0, 0} ,{FPC_NONE, (UINT32)0, 0, 0}},
        (UINT8*)g_dataBufferA, (UINT8*)g_dataBufferB, (UINT8*)g_dataBufferC, FPC_BUFFER_SIZE};

#if !(CHIP_HAS_USB == 1) && (FASTPF_USE_USB == 1)
    #error error: You can t use FASTPF_USE_USB without CHIP_HAS_USB
#endif

// =============================================================================
// FLASH_PROGRAMMER_SPECIAL_SECTOR_T
// -----------------------------------------------------------------------------
/// Describe the special sector (use a magic number the first word)
// =============================================================================
typedef struct
{
    UINT8* address;
    UINT32 magicNumber;
} FLASH_PROGRAMMER_SPECIAL_SECTOR_T;

// Check whether flash programmer data buffer is accessible
PROTECTED BOOL fp_CheckDataBuffer(VOID)
{
    static CONST UINT32 pattern[4] = { 0xa5a5a5a5, 0x5a5a5a5a, 0x4f35b7da, 0x8e354c91, };
    static VOLATILE UINT32 * CONST dataAry[6] = {
        (VOLATILE UINT32 *)&g_dataBufferA[0],
        (VOLATILE UINT32 *)&g_dataBufferA[FPC_BUFFER_SIZE/4 - 1],
        (VOLATILE UINT32 *)&g_dataBufferB[0],
        (VOLATILE UINT32 *)&g_dataBufferB[FPC_BUFFER_SIZE/4 - 1],
        (VOLATILE UINT32 *)&g_dataBufferC[0],
        (VOLATILE UINT32 *)&g_dataBufferC[FPC_BUFFER_SIZE/4 - 1],
    };

    for (int i=0; i<6; i++)
    {
        for (int j=0; j<4; j++)
        {
            *(dataAry[i]) = pattern[j];
            if (*(dataAry[i]) != pattern[j])
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

// This xcpu_main replaces the regular xcpu_main present in the platform library
PROTECTED int main(VOID)
{
    VOLATILE FPC_COMMAND_DATA_T*         cmds =
        (FPC_COMMAND_DATA_T*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)g_flashProgDescr.commandDescr);
    INT32                                i = 0;
    UINT32                               cpt;
    UINT32                               cpt2;
    INT32                                errorDuringCommand;
    INT32                                status = MEMD_ERR_NO;
    FPC_COMMAND_TYPE_T                   c;
    UINT32                               s;
    UINT32                               sectorWrite = 0;
    HAL_DMA_FCS_CFG_T                    fcsCtx;
    UINT8                                fcs[3];
    UINT32*                              verifySectorSize;
    UINT32*                              verifySectorAddress;
    UINT32*                              verifyAwaitedFcs;  
    UINT32                               verifyFirstBadSector;
    
    FLASH_PROGRAMMER_SPECIAL_SECTOR_T    flashSpecialSector[] = {
        { .address = 0x00000000,
            .magicNumber = 0xD9EF0045 },
        { .address = 0x00000000,
            .magicNumber = 0x00000000 },
    };

    hal_SysRequestFreq(HAL_SYS_FREQ_APP_USER_0, HAL_SYS_FREQ_104M, NULL);

    cmds[0].cmd = FPC_NONE;
    cmds[1].cmd = FPC_NONE;

//  enable exception (gdb breaks) but not external it, keep handler in rom
    asm volatile("li    $27,    (0x00400000 | 0x0001 | 0xc000)\n\t"
                 "mtc0  $0, $13\n\t"
                 "nop\n\t"
                 "mtc0  $27, $12\n\t"
//  k1 is status for os irq scheme
                 "li    $27, 1");

    memd_FlashOpen(tgt_GetMemdFlashConfig());
    memd_RamOpen(tgt_GetMemdRamConfig());

    if (!fp_CheckDataBuffer())
    {
        SEND_EVENT(EVENT_FLASH_PROG_MEM_RESET);
        hal_SysResetOut(TRUE);
        pmd_EnableMemoryPower(FALSE);
        hal_TimDelay(2 * HAL_TICK1S);
        pmd_EnableMemoryPower(TRUE);
        hal_TimDelay(10);
        hal_SysResetOut(FALSE);
        hal_TimDelay(10);

        memd_FlashOpen(tgt_GetMemdFlashConfig());
        memd_RamOpen(tgt_GetMemdRamConfig());

        if (!fp_CheckDataBuffer())
        {
            SEND_EVENT(EVENT_FLASH_PROG_MEM_ERROR);
            while(1);
        }
    }

    // Record the communication structure into HAL Mapping Engine
    hal_MapEngineRegisterModule(HAL_MAP_ID_FLASH_PROG, &g_flashProgVersion, &g_flashProgDescr);

#if (CHIP_HAS_USB == 1) && (FASTPF_USE_USB == 1)
    g_flashProgUsbContext.MonitorMode = BOOT_HOST_USB_MODE_BOOT;
    #if ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
    boot_HostUsbOpen(&g_flashProgUsbContext, 0,0);
    #else
    boot_HostUsbOpen(&g_flashProgUsbContext, 0);
    #endif
#endif


    SEND_EVENT(EVENT_FLASH_PROG_READY);
    while (1)
    {
#if (CHIP_HAS_USB == 1) && (FASTPF_USE_USB == 1)
        boot_HostUsbRecv();
        boot_HostUsbSend();
#endif
        
        c = cmds[i].cmd;
        
        switch (c)
        {
        case FPC_NONE:
        case FPC_DONE:
        case FPC_ERROR:
        case FPC_FCS_ERROR:
        case FPC_FLASH_NOT_AT_FF:
            break;

        case FPC_PROGRAM:
            errorDuringCommand = 0;
            
            // Do the RAM check if we are asked too. 
            // Do also a check of the FLASH (verify if everything is at FF).
            // These operations are port of the intensive verify.
            if(cmds[i].fcs & 0x80000000)
            { 
                fcsCtx.srcAddr      = cmds[i].ramAddr;
                fcsCtx.transferSize = cmds[i].size;
                fcsCtx.userHandler  = NULL;
                
                hal_DmaFcsStart(&fcsCtx);
                hal_DmaReadFcs(fcs);    
                
                if( (cmds[i].fcs & 0x00FFFFFF) != (fcs[0]<<16 | (fcs[1] << 8) | fcs[2]) )
                {
                    errorDuringCommand = 1;
                    cmds[i].cmd = FPC_FCS_ERROR; // FCS error
                }
                else
                {
                    // Verify the content of the flash, if every byte is at FF
                    verifySectorAddress = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(memd_FlashGetGlobalAddress((UINT8*)cmds[i].flashAddr));
                    for(cpt=0;cpt<cmds[i].size;cpt+=4,verifySectorAddress++)
                    {
                        if(*verifySectorAddress!=0xFFFFFFFF)
                        {
                            errorDuringCommand = 1;
                            cmds[i].cmd = FPC_FLASH_NOT_AT_FF; // FCS error
                        }
                    } 
                }
            }
            
            //Everything's ok til now
            if(errorDuringCommand==0)
            {
                status       = memd_FlashWrite(cmds[i].flashAddr,
                                               cmds[i].size, &s,
                                               (CHAR*) (cmds[i].ramAddr));
        
                cmds[i].size = s;
                if (status != MEMD_ERR_NO)
                {
                    errorDuringCommand = 1;
                    cmds[i].cmd = FPC_ERROR; //Standard error
                }
                else
                { 
                    // Flash write was ok, write magic number.
                    for(cpt = 0; flashSpecialSector[cpt].magicNumber != 0x00000000; ++cpt)
                    {
                        if(cmds[i].flashAddr == flashSpecialSector[cpt].address)
                        {
                            sectorWrite |= 1<<cpt;
                        }
                    }
                }
            }

            // ERROR MANAGEMENT FOR FPC_PROGRAM
            if(errorDuringCommand)
            { // programm Error
                
                SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
#ifdef DEBUG_EVENTS
                hal_HstSendEvent(0xef00000-status);
#endif
            }
            else
            {
                // Set command is done, switch to next command
                cmds[i].cmd  = FPC_DONE;
                SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                i           ^= 1;              
            }
            break;
            
        case FPC_ERASE_SECTOR:
            status = memd_FlashErase(cmds[i].flashAddr,NULL);
            if (status == MEMD_ERR_NO)
            { // OK
                cmds[i].cmd  = FPC_DONE;
                SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                i           ^= 1;
            }
            else
            { // Error
                cmds[i].cmd = FPC_ERROR;
                SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
#ifdef DEBUG_EVENTS
                hal_HstSendEvent(0xee00000-status);
#endif
            }
            break;
            
        case FPC_ERASE_CHIP:
            // Not implemented.
            cmds[i].cmd = FPC_ERROR;
            SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
            break;
            
        case FPC_GET_FINALIZE_INFO:
            // Count the number of sectors we have in the flashSpecialSector.
            cpt2 = 0;
            while(flashSpecialSector[cpt2].magicNumber != 0)
                cpt2++;
            
            // Write into ram all the magic numbers and their addresses
            verifySectorAddress     = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(cmds[i].ramAddr);
            for(cpt = 0; cpt < cpt2 ; cpt++)
            {
                *verifySectorAddress = (UINT32)flashSpecialSector[cpt].address;
                verifySectorAddress++;
                *verifySectorAddress = flashSpecialSector[cpt].magicNumber;
                verifySectorAddress++;
            }
            
            // Inform PC that the command has been treated
            cmds[i].size = cpt2;
            cmds[i].cmd  = FPC_DONE;
            SEND_EVENT(EVENT_FLASH_PROG_READY + i);
            i           ^= 1;            
            
            break;
        case FPC_CHECK_FCS:
            
            s = 0;
            
            verifySectorAddress     = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(cmds[i].ramAddr);
            verifySectorSize        = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(cmds[i].ramAddr+4);
            verifyAwaitedFcs        = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(cmds[i].ramAddr+8);
            verifyFirstBadSector    = 0xFFFFFFFF;
            
            for(cpt=0; cpt<cmds[i].size; cpt++)
            {
                fcsCtx.srcAddr      = (UINT8*)HAL_SYS_GET_UNCACHED_ADDR(memd_FlashGetGlobalAddress((UINT8*)*verifySectorAddress));
                fcsCtx.transferSize = *verifySectorSize;
                fcsCtx.userHandler  = NULL;
                
                hal_DmaFcsStart(&fcsCtx);
                hal_DmaReadFcs(fcs);    
               
                if( *verifyAwaitedFcs != (fcs[0]<<16 | (fcs[1] << 8) | fcs[2]) )
                {
                    // Uh, oh. Memcmp failed, there's an error.
                    s++;
                    // Replace FCS in ram for this block by the bad fcs
                    *verifyAwaitedFcs = (fcs[0]<<16 | (fcs[1] << 8) | fcs[2]);
                    // Memorize first bad sector
                    if(verifyFirstBadSector == 0xFFFFFFFF)
                    {
                        verifyFirstBadSector = cpt;
                    }
                }
                verifySectorAddress += 3;
                verifySectorSize    += 3;
                verifyAwaitedFcs    += 3;
            }
            
            if(s!=0)
            {
                //Send error code.
                cmds[i].ramAddr     = (UINT8*)verifyFirstBadSector;   
                cmds[i].cmd         = FPC_ERROR;
                SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
            }
            else
            {
                // No error, write the done flag and pass to the other command
                cmds[i].cmd  = FPC_DONE;
                SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                i           ^= 1;
            }
            break;
            
        case FPC_END:
            status = MEMD_ERR_NO;
            for (cpt = 0; sectorWrite >> cpt; ++cpt)
            {
                if (sectorWrite & (1<<cpt))
                {
                    status = memd_FlashWrite(flashSpecialSector[cpt].address,
                                             sizeof(UINT32), &s,
                                             (CHAR*)(&flashSpecialSector[cpt].magicNumber));
                    
                    if (status != MEMD_ERR_NO)
                    {
                        break;
                    }
  
                    // Check if the write of the magic number succeeded
                    if(* ((UINT32*)(memd_FlashGetGlobalAddress(flashSpecialSector[cpt].address)))
                         != flashSpecialSector[cpt].magicNumber)
                    {
                        status = -127; // FIXME: A bit permissive.
                        break;   
                    }
                }
            }

            if (status == MEMD_ERR_NO)
            { // OK
                cmds[i].cmd = FPC_DONE;
                SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                i           ^= 1;
            }
            else
            { // programm Error
                cmds[i].cmd = FPC_ERROR;
                SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
#ifdef DEBUG_EVENTS
                hal_HstSendEvent(0xef00000-status);
#endif
            }
            sectorWrite = 0;
            break;

        case FPC_RESTART:
            cmds[i].cmd = FPC_DONE;
            SEND_EVENT(EVENT_FLASH_PROG_READY + i);
            // Sleep a bit to wait for a few ms to be sure the event has been sent.
            hal_HstWaitLastEventSent();

            // Wait 3 32kHz ticks to let the event some distance ahead
            // before unleashing the dogs.
            hal_TimDelay(3);
            
            // Restart the power cycle the USB Phy and restart the system.
            hal_SysRestart();
            break;
        default:
            cmds[i].cmd = FPC_ERROR;
            SEND_EVENT(EVENT_FLASH_PROG_UNKNOWN + i);
#ifdef DEBUG_EVENTS
            hal_HstSendEvent(0xeee0000);
#endif
            break;
        }
    }
}

