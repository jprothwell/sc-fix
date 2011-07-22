////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/boot/src/boot_sector.c $
//  $Author: huazeng $
//  $Date: 2011-01-05 18:10:02 +0800 (Wed, 05 Jan 2011) $
//  $Revision: 5085 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file boot_sector.c
///
/// Boot sector.
/// 
/// The code of the Boot Sector must be mapped in the first sector of the
/// Flash and should only use internal RAM (as the external SRAM might not
/// be usable in case of burst mode, after a reset).
//
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "string.h"

#include "chip_id.h"
#include "global_macros.h"
#include "cfg_regs.h"
#include "sys_ctrl.h"
#include "mem_bridge.h"
#include "debug_uart.h"
#include "debug_host.h"

#include "bootp_host.h"
#include "bootp_loader.h"
#include "bootp_sector.h"
#include "boot_host.h"
#include "boot_sector.h"
#include "boot.h"


// =============================================================================
// MACROS                                                                       
// =============================================================================


/// Linker section where the Boot Sector main function must be mapped:
/// Beginning of the first sector of the flash.
#define BOOT_SECTOR_SECTION_START  __attribute__((section (".boot_sector_start")))


/// Linker sections where the Boot Sector structure pointer must be mapped.
/// At a fixed address in the Internal SRAM, right before the fixed pointer.
#define BOOT_SECTOR_SECTION_STRUCT_PTR __attribute__((section (".boot_sector_struct_ptr")))


/// Linker sections where the Boot Sector reload structure pointer must be mapped.
/// At a fixed address in the Internal SRAM, right before the boot sector struct pointer.
#define BOOT_SECTOR_SECTION_RELOAD_STRUCT __attribute__((section (".boot_sector_reload_struct")))


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


/// Boot Sector structure pointer.
/// 
/// This pointer is filled by HAL during the normal execution of the code.
/// It is used by the Boot Sector to find the Boot Sector structure in
/// Internal RAM.
/// 
/// This pointer will be also used by the host tool (E.g., by CoolWatcher to configure
/// EBC during a restart). Its location is specified in the ld file for the software
/// on the target, and in the corresponding ???_soft_root.xmd for the host tool.
/// These location definitions should be always kept consistent.
PUBLIC BOOT_SECTOR_STRUCT_T* g_bootSectorStructPtr BOOT_SECTOR_SECTION_STRUCT_PTR;


// Copied by the lod command of CoolWatcher. Useful for Ramruns.
PROTECTED BOOT_HST_EXECUTOR_T g_bootHstExecutorCtx
    __attribute__((section (".executor_ctx"))) =
    {{ 0, (UINT32)boot_Sector, 0, 0, 0}, {0}};


/// Boot Sector reload structure.

/// This structure contains information written by HAL during the normal
/// execution of the code and used by the Boot Sector to reload the ram image
/// when the phone boots. 
PUBLIC BOOT_SECTOR_RELOAD_STRUCT_T g_bootSectorReloadStruct BOOT_SECTOR_SECTION_RELOAD_STRUCT;


// =============================================================================
// EXPORTS                                                            
// =============================================================================


// =============================================================================
// FUNCTIONS                                                            
// =============================================================================


// =============================================================================
// boot_BootSectorGetStructValidity
// -----------------------------------------------------------------------------
/// Check the validity of the Boot Sector structure by reading a bit in
/// the Boot Mode register and then by checking that the address contained in
/// the Boot Sector structure pointer is a valid address (in KSEG0 or in
/// KSEG(1) and not impair).
///
/// @return TRUE if valid, FALSE else.
// =============================================================================
INLINE BOOL boot_BootSectorGetStructValidity(VOID)
{
    if (FALSE == BOOT_SECTOR_GET_STRUCT_VALIDITY())
    {
        return FALSE;
    }

    // Is the address contained in g_bootSectorStructPtr contained in a
    // valid KSEG and is it pair?
    if (  (((UINT32)g_bootSectorStructPtr & 0x3) != 0) ||
          ( (((UINT32)g_bootSectorStructPtr & KSEG0(0)) != KSEG0(0)) &&
            (((UINT32)g_bootSectorStructPtr & KSEG1(0)) != KSEG1(0)) )  )
    {
        return FALSE;
    }

    // Is the address in RAM/SRAM space?
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    if ( ! (
             ( (UINT32)g_bootSectorStructPtr >= 0x80000000 &&
                  (UINT32)g_bootSectorStructPtr < 0x81000000 ) ||
             ( (UINT32)g_bootSectorStructPtr >= 0xa0000000 &&
                  (UINT32)g_bootSectorStructPtr < 0xa1000000 )
           ) )
    {
        return FALSE;
    }
#else // !CHIP_ASIC_ID_GREENSTONE
    if ( ! (
             ( (UINT32)g_bootSectorStructPtr >= 0x80000000 &&
                  (UINT32)g_bootSectorStructPtr < 0x82000000 ) ||
             ( (UINT32)g_bootSectorStructPtr >= 0xa0000000 &&
                  (UINT32)g_bootSectorStructPtr < 0xa2000000 )
           ) )
    {
        return FALSE;
    }
#endif // !CHIP_ASIC_ID_GREENSTONE

    return TRUE;
}


// =============================================================================
// boot_BootSectorGetReloadValid
// -----------------------------------------------------------------------------
/// Check the validity of the Boot Sector reload structure by reading the flag and checksum in
/// the Boot reload structure.
///
/// @return TRUE if valid, FALSE else.
// =============================================================================
INLINE BOOL boot_BootSectorGetReloadValid(VOID)
{
    if( (g_bootSectorReloadStruct.validTag != BOOT_SECTOR_RELOAD_VALID_TAG) ||
        (g_bootSectorReloadStruct.checkSum != ~BOOT_SECTOR_RELOAD_VALID_TAG) )
    {
        return FALSE;
    }

    g_bootSectorReloadStruct.validTag = 0;

    return TRUE;
}


// =============================================================================
// boot_SectorReload
// -----------------------------------------------------------------------------
/// Relocate the ram image to the execution section and jump to the entry point.
///
/// @return TRUE if succeeded, FALSE else.
// =============================================================================
INLINE BOOL boot_SectorReload(VOID)
{
    UINT32 i, j;
    for(j=0; j<BOOT_SECTOR_RELOAD_MAX_NUM; j++)
    {
        UINT32 *mapAddr = (UINT32 *)KSEG0(g_bootSectorReloadStruct.ctx[j].mapAddress);
        UINT32 *stroreAddr = (UINT32 *)KSEG0(g_bootSectorReloadStruct.ctx[j].storeAddress);
        UINT32 size = g_bootSectorReloadStruct.ctx[j].size;

        for(i=0; i<size; i++)
        {
            *mapAddr++ = *stroreAddr++;
        }
    }
    // Execute at the PC in the execution context with the provided SP
    boot_HstMonitorX();    
    return TRUE;
}


// =============================================================================
// boot_Sector
// -----------------------------------------------------------------------------
/// Entry of the Boot Sector in the first flash sector.
/// 
/// This code is executed right after the Boot ROM and right before the
/// Boot Loader, in normal mode execution. It does the following:
/// 
/// * Writes the EBC RAM chip select configuration if a valid configuration
/// has been found in the Boot Sector structure.
/// 
/// * If the command in the Boot Sector structure tells us to enter
/// boot monitor, we do so after sending an event.
/// 
/// * Else, we enter the Boot Loader, providing it with the #param.
/// 
/// @param param Generic parameter only used to invoke boot_LoaderEnter(param).
// =============================================================================
PROTECTED VOID BOOT_SECTOR_SECTION_START boot_Sector(UINT32 param)
{

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    // Enable the host from within. (Forced)
    // Set clock
    // Formula from the uart driver
    // Divider mode is 4
    // There is no PLL during monitoring
    // Rq: the plus 4/2 * rate is a subtle way to get the upper integer
    // There is no need to worry about updating the dividers, are we are using
    // the slow clock that is 'always' 26Mhz.
    UINT32 divider = ((BOOT_HST_SYSTEM_CLOCK + 2 * BOOT_HST_HOST_RATE) / ( 4 * BOOT_HST_HOST_RATE)) - 2;
    hwp_sysCtrl->Cfg_Clk_Uart[0] = SYS_CTRL_UART_DIVIDER(divider) | SYS_CTRL_UART_SEL_PLL_SLOW;
    
    // Force UART Host monitor on.
    hwp_debugHost->mode = DEBUG_HOST_FORCE_ON;
#endif

    BOOL bootSectorStructIsValid = boot_BootSectorGetStructValidity();

    // Writes the EBC RAM chip select configuration if a valid configuration
    // has been found in the Boot Sector structure.
    if (bootSectorStructIsValid)
    {
        if (g_bootSectorStructPtr->ebcConfigValidTag == BOOT_SECTOR_EBC_VALID_TAG)
        {
            mon_Event(BOOT_SECTOR_EVENT_CONFIG_RAM);
            hwp_memBridge->CS_Time_Write = g_bootSectorStructPtr->ebcConfigRamTimings;
            hwp_memBridge->CS_Config[1].CS_Mode = g_bootSectorStructPtr->ebcConfigRamMode;
        }
    }

    BOOL bootSectorGetReloadValid = boot_BootSectorGetReloadValid();

    if(bootSectorGetReloadValid)
    {
       boot_SectorReload();
    }

    // If a valid configuration has been found, those applied timings were
    // determined for a configured IO drive. As it is also something reasonable
    // to do with the slowest romed timing, we configure here the IO drives
    // to a 'reasonable' value that would allow both optimized and slow
    // timings to work.
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    hwp_extApb->IO_Drive_Select1 = 0x77777777;
    hwp_extApb->IO_Drive_Select2 = 0x77777777;
#endif


    // Use to fix some startup missing feature (like usb clock on greenstone)
    boot_SectorFixRom();

    if (bootSectorStructIsValid)
    {
        BOOT_SECTOR_CMD_T cmd = g_bootSectorStructPtr->command;
        // Execute the command of the Boot Sector structure only once.
        g_bootSectorStructPtr->command = 0;
        // If the command in the Boot Sector structure tells us to enter
        // boot monitor, we do so after sending an event.
        if (cmd == BOOT_SECTOR_CMD_ENTER_BOOT_MONITOR)
        {
            // Write in the exchange status we are in the 
            // monitor, ready for operation.
            hwp_debugHost->p2h_status = DEBUG_HOST_P2H_STATUS(BOOT_HST_STATUS_WAITING_FOR_COMMAND);
            mon_Event(BOOT_SECTOR_EVENT_ENTER_MONITOR);
            boot_SectorEnterBootMonitor();
        }
    }
    
    // Clear any remaining exchange status.
    hwp_debugHost->p2h_status = DEBUG_HOST_P2H_STATUS(BOOT_HST_STATUS_NONE);

    // Boot loader is now always linked with boot sector.
    boot_LoaderEnter(param);
}


