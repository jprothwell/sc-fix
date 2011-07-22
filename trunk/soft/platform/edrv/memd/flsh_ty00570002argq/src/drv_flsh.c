////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/memd/flsh_ty00570002argq/src/drv_flsh.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-09-01 19:25:00 +0800 (Wed, 01 Sep 2010) $                     //   
//	$Revision: 1857 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_flsh.c                                                          //
/// That file provides the driver implementation for the flsh_s71ws256nc0     //
/// flash.                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////




#include "cs_types.h"

#include "hal_ebc.h"
#include "hal_sys.h"
//#include "sxs_io.h"

#include "string.h"

#include "memd_m.h"
#include "memdp_debug.h"
#include "memdp.h"
#include "hal_host.h"


/* banks at flash bit 21 */
/* for Toshiba 128M */
#define FLASHBANK_MASK      (0x1F<<(21))     // Actually only need 4 bits, but have to use 5 due to shift
#define FLASHBLOCK_MASK     (~0x7fff)
#define FLASH_ADDR_MASK     0x03FFFFFF      // 0x0100_0000 to 0x0300_0000, ignore the 0x8 or 0xA
#define SEG_MASK            0x0FFFFFFF
#define MY_MASK 0x00FFFFFF

#define KBYTES              1024
#define MBYTES              (1024*KBYTES)


// -------------------------------
// FLASH/RAM configuration
// -------------------------------
#define FLASH_SIZE  16*MBYTES // 16MB

    



// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
/// Base address
UINT32 g_memdFlashBaseAddress __attribute__((section (".memd.globalvars.c"))) = 0;

BOOL g_memdDybOverride = TRUE;

PRIVATE BOOL memd_DYBCheck(VOLATILE UINT16 *Addr, VOLATILE UINT16 *Ptr);
PRIVATE MEMD_ERR_T memd_DYB(UINT8 *startFlashAddress, UINT8 *endFlashAddress, BOOL Enable);


#ifdef MEMD_FLASH_IS_BURST
PRIVATE VOID __attribute__((section (".sramtext"))) 
memd_FlashConfigureBurstMode(REG16* csBase, REG32* pCRE)
{
    // Entry
    *(csBase+0x555) = 0xaa;
    *(csBase+0x2aa) = 0x55;
    *(csBase+0x43555 ) = 0xc0;
    
    
    {
        UINT32 flush __attribute__((unused)) = *pCRE;
    }
    
}
#endif //!MEMD_FLASH_IS_BURST


// =============================================================================
// memd_FlashOpen
// -----------------------------------------------------------------------------
/// That function configures the flash.
/// It must be called before using any other function of the MEMD API.
///
/// @param cfg Pointer to the configuration to use to open the flash.
/// This parameter is ignored by the romulator driver.
/// @return Pointer towards the structure describing the flash.
// =============================================================================
PUBLIC MEMD_FLASH_LAYOUT_T* memd_FlashOpen(CONST MEMD_FLASH_CONFIG_T* cfg)
{
    MEMD_ASSERT(cfg != NULL, "Opening flash with a NULL configuration");
    // Open EBC ? (not needed with new hardware, as the only option
    // is enable ...

    // Open Flash, and store base address.
#ifdef MEMD_FLASH_IS_BURST
    g_memdFlashBaseAddress = (UINT32) hal_EbcFlashOpen(HAL_SYS_FREQ_13M,
                            &cfg->csConfig, memd_FlashConfigureBurstMode);
#else //!MEMD_FLASH_IS_BURST
#ifdef MEMD_RAM_IS_BURST
    g_memdFlashBaseAddress = (UINT32) hal_EbcFlashOpen(HAL_SYS_FREQ_13M,
                            &cfg->csConfig, NULL);
#else //!MEMD_RAM_IS_BURST
    g_memdFlashBaseAddress = (UINT32) hal_EbcFlashOpen(HAL_SYS_FREQ_39M,
                            &cfg->csConfig, NULL);
#endif //!MEMD_RAM_IS_BURST
#endif //!MEMD_FLASH_IS_BURST

    //SXS_TRACE(TSTDOUT,"hal_flash: g_memdFlashBaseAddress 0x%08x \n", g_memdFlashBaseAddress);
    return ((MEMD_FLASH_LAYOUT_T*) &g_memdFlashLayout);
}

#ifdef MEMD_RAM_IS_BURST
PRIVATE VOID __attribute__((section (".sramtext"))) 
memd_RamConfigureBurstMode(REG16* csBase, REG32* pCRE)
{
    UINT16 read;
UINT32 status;
    //while((*(volatile UINT32*)0xA0BBF008) == 0 );
    //*(volatile UINT32*)0xA0BBF008 = (0x8800);
    status = hal_SysEnterCriticalSection();
    
   
   
    

    read=*(REG16*)(csBase + 0x1fffff);
     
  
    *(REG16*)(csBase + 0x1fffff)= 0;
    *(REG16*)(csBase + 0x1fffff)= 0;
    *(REG16*)(csBase + 0x1fffff)= read;
    *(REG16*)(csBase + 0x1fffff)= read;
    
   // read = *(REG16*)(csBase + 0x072f7f);
   read = *(REG16*)(csBase + 0x072b7f);
    
   
    
    
    hal_SysExitCriticalSection(status);

    //while((*(volatile UINT32*)0xA0BBF008) == 0 );
    //*(volatile UINT32*)0xA0BBF008 = (0x8801);
}
#endif //!MEMD_RAM_IS_BURST


// =============================================================================
// memd_RamOpen
// -----------------------------------------------------------------------------
/// That function configures the RAM.
/// It must be called to configure the best settings for the RAM, thus enabling
/// the fatest access.
/// @param cfg Ram configuration used to open the RAM.
///
// =============================================================================
PUBLIC VOID memd_RamOpen(CONST MEMD_RAM_CONFIG_T* cfg)
{
    //hal_HstSendEvent(0x7700);
#ifdef MEMD_RAM_IS_BURST
    hal_EbcSramOpen(&cfg->csConfig, memd_RamConfigureBurstMode);
#else //!MEMD_RAM_IS_BURST

    // Open External SRAM
    hal_EbcCsOpen(HAL_EBC_SRAM, &cfg->csConfig);
#endif //!MEMD_RAM_IS_BURST
    //hal_HstSendEvent(0x7701);
}





PRIVATE UINT32 memd_FlashGetSectorLimits(UINT32 sectorId, UINT32 *start, UINT32 *end)
{
    UINT32 aStart, aEnd;
    int bankNum = 0;
    int majSecIdx, minSecIdx;
    UINT32 sectSize;

    if (sectorId == g_memdFlashLayout.fullSize)
    {
        *start = g_memdFlashLayout.fullSize;
        *end = g_memdFlashLayout.fullSize;
        return 0;
    }
    else if (sectorId > g_memdFlashLayout.fullSize)
    {
        *start = 0;
        *end = 0;
        MEMD_ASSERT(FALSE, "Out of accessible flash space !!!!");
        return 0;
    }
    else
    {
        while ( (sectorId >= g_memdFlashLayout.bankLayout[bankNum+1].bankStartAddr) &&
                (bankNum < (g_memdFlashLayout.numberOfBank-1)) )
        {
            ++bankNum;
        }
        aStart = g_memdFlashLayout.bankLayout[bankNum].bankStartAddr;

        majSecIdx = 0;
        aEnd = aStart +
            (g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][0] *
            g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1]);
        while (sectorId >= aEnd)
        {
            if (majSecIdx == 2)
            {
                *start = 0;
                *end = 0;
                MEMD_ASSERT(FALSE, "Out of accessible flash space !!!!");
                return 0;
            } else
            {
                ++majSecIdx;
                aStart = aEnd;
                aEnd = aStart +
                    (g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][0] *
                    g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1]);
            }
        }

        minSecIdx = 0;
        sectSize = g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1];
        aEnd = aStart + sectSize;
        while (sectorId >= aEnd)
        {
            aStart = aEnd;
            aEnd = aStart + sectSize;
            ++minSecIdx;
        }

        *start = aStart;
        *end = aEnd;
        return sectSize;
    }
}

PRIVATE BOOL memd_DYBCheck(VOLATILE UINT16 *Addr, VOLATILE UINT16 *Ptr)
{
    UINT16 DYB;

    // Entry
    *(Addr+0x555) = 0xaa;
    *(Addr+0x2aa) = 0x55;
    *(Addr+0x555) = 0x58;
    // DYB Status Read
    DYB = *(Ptr);
    // Need to exit DYB mode
    *(Addr+0x555) = 0xaa;
    *(Addr+0x2aa) = 0x55;
    *(Addr+0x555) = 0x90;
    *(Addr) = 0;
    if ((DYB & 0x1) == 0)
    {
        return FALSE;
    } else
    {
        return TRUE;
    }
}


// =============================================================================
// memd_FlashErase
// -----------------------------------------------------------------------------
/// This function erases contiguous flash sectors. 
/// Addresses <B> must be aligned on sectors</B>:
/// - The \c startFlashAddress is the address of the first sector to erase.
/// - The \c endFlashAddress is the address of the first sector NOT to erase.
///   If \c endFlashAddress is \c NULL, only one sector will be erased.
/// .
/// Care must be taken not to erase the code present at the beginning of the flash.
///
/// @param start_flashAddress The address of the first sector to erase
/// @param end_flashAddress The address of the first sector NOT to erase.
///   If \c NULL, only one sector will be erased 
/// @return #MEMD_ERR_NO, #MEMD_ERR_ERASE, #MEMD_ERR_ALIGN or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed
///
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashErase(UINT8 *startFlashAddress, UINT8 *endFlashAddress)
{
    UINT8 rdstatus, rdstatus_old;
    UINT32 status;
    VOLATILE UINT8 * ptr;
    VOLATILE UINT16 *BankAddr;
    UINT32 Start, End;
    UINT32 phys_end_add, phys_start_add;

    phys_start_add = (UINT32)startFlashAddress;
    MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    // Check that start & end addresses are aligned
    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        phys_end_add = End;
    } else
    {
        phys_end_add = (UINT32)endFlashAddress;
        MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")
        if (phys_end_add != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
            if (phys_end_add != Start)
            {
                return MEMD_ERR_ALIGN;
            }
        }
    }

    memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
    if (phys_start_add != Start)
    {
        return MEMD_ERR_ALIGN;
    }

    hal_EbcFlashWriteEnable(TRUE);

    while (Start != phys_end_add)
    {
        
        BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBANK_MASK));
        // Start should already be aligned to sector boundary, so shouldn't need any more masking
        ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + Start);
       

        //SXS_TRACE(TSTDOUT,"hal_flash: erase BankAddr 0x%08x, Page %d \n", BankAddr, GET_PAGE_PHYS(Start));
        //SXS_TRACE(TSTDOUT,"hal_flash: erase ptr 0x%08x \n", ptr);
        if (!g_memdDybOverride)
        {
            if (memd_DYBCheck(BankAddr, (UINT16*)ptr))
            {
                hal_EbcFlashWriteEnable(FALSE);
                return MEMD_ERR_PROTECT;
            }
        }

        status = hal_SysEnterCriticalSection();
        // Sector Erase command
        *(BankAddr+0x555) = 0xaa;
        *(BankAddr+0x2aa) = 0x55;
        *(BankAddr+0x555) = 0x80;
        *(BankAddr+0x555) = 0xaa;
        *(BankAddr+0x2aa) = 0x55;
        *(ptr) = 0x30;
        hal_SysExitCriticalSection(status);

        // note the pooling could be done on data == 0xff also
    
        // pooling
        rdstatus = (*ptr);
        while(1){
            rdstatus_old = rdstatus;
            rdstatus = (*ptr);
            // DQ6 & DQ2 not toggling? => done
            if (((rdstatus ^ rdstatus_old) & 0x44) == 0)
            break;
            // DQ5 = 1
            if ((rdstatus & 0x20) == 0x20) {
                rdstatus_old = (*ptr);
                rdstatus = (*ptr);
                // DQ6 & DQ2 not toggling? => done
                if (((rdstatus ^ rdstatus_old) & 0x44) == 0)
                    break;
                // reset
                (*BankAddr) = 0xf0;
                hal_EbcFlashWriteEnable(FALSE);
                return MEMD_ERR_ERASE;
            }
        }
        if (End != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(End, &Start, &End);
        } else
        {
            Start = End;
        }
    }
    // erase done
    hal_EbcFlashWriteEnable(FALSE);
    return MEMD_ERR_NO;
}



// =============================================================================
// memd_FlashWrite
// -----------------------------------------------------------------------------
/// This function writes data in the flash. It gets its data from \c buffer, copies 
/// \c byteSize bytes to the flash location designed by \c flashAddress. \c 
/// pWrittenbyteSize is filled with the actual number of written bytes (Equal 
/// to size, or less in case of error).
/// 
/// @param  flashAddress The byte offset within the flash chip. (Take care not 
/// to overwrite the code present at the beginning of the flash)
/// @param  byteSize Number of bytes to write in flash
/// @param  pWrittenbyteSize Number of bytes actually written in flash
/// @param  buffer Buffer where to get the data to write in the flash
/// @return #MEMD_ERR_NO, #MEMD_ERR_WRITE or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed.
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashWrite(UINT8 *flashAddress,
        UINT32 byteSize, 
        UINT32 * pWrittenbyteSize, 
        CONST UINT8* buffer)
{
    VOLATILE UINT16 * BankAddr;
    VOLATILE UINT16 * ptr;
    UINT16 rdstatus, data;
    UINT32 bsize=0, wsize=0, owsize=0;
    MEMD_ERR_T errorcode=MEMD_ERR_NO;
    UINT32 status;
    UINT32 phys_add;
    UINT32 SectAddr;
    UINT32 End;

    //SXS_TRACE(TSTDOUT,"hal_flash: Flash Write 0x%08x, %d\n", flash_address, bytesize);

    *pWrittenbyteSize=0;
    if (byteSize==0)
    {
        return MEMD_ERR_NO;
    }

    hal_EbcFlashWriteEnable(TRUE);

    phys_add = (UINT32)flashAddress;
    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")

    memd_FlashGetSectorLimits((UINT32)flashAddress, &SectAddr, &End);
   
    BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&FLASHBANK_MASK));
    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));
    //SectAddr += g_memdFlashBaseAddress;
   
   
    // Unlock Bypass Entry
    status = hal_SysEnterCriticalSection();
    //memd_DYB((UINT8*)(SectAddr ),NULL,FALSE);

    //hal_EbcFlashWriteEnable(TRUE);
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x20;
    hal_SysExitCriticalSection(status);
    //SXS_TRACE(TSTDOUT,"hal_flash: unlock\n");

    if ((phys_add&1) == 1)
    {
        data = *(ptr);
    } else {
        data = (*buffer) | ~0x00ff;
        buffer ++;
        bsize ++;
    }
    if (bsize<byteSize)
    {
        data = data & (((*buffer) << 8) | 0x00ff);
        buffer ++;
        bsize ++;
    }

    if (bsize==byteSize)
    {
        // last byte
        data = data & *(ptr);
    }
    // first byte is prepared
    /* 16b data ready write it to flash*/
    status = hal_SysEnterCriticalSection();
    *(BankAddr) = 0xa0;
    *(ptr)=data;
    hal_SysExitCriticalSection(status);

    while(bsize<byteSize) {
        UINT16 tdata;
        VOLATILE UINT16 * tptr;

        // do the next data preparation before the pooling so we are ready to do a new programming just after pooling is OK.
        tdata = data;
        tptr = ptr;

        owsize = wsize;

        wsize = bsize;
        ptr+=1;


        if (bsize<byteSize) {
            data = (*buffer) | ~0x00ff;
            buffer ++;
            bsize ++;
        }
        if(bsize<byteSize) {
            data = data & (((*buffer) << 8) | 0x00ff);
            buffer ++;
            bsize ++;
        }

        // pooling
        while(1)
        {
            rdstatus = (*tptr);
            // DQ7 = prog value ? ok done
            if (((rdstatus ^ tdata) & 0x80) == 0)
                break;
            // DQ5 = 1
            if ((rdstatus & 0x20) == 0x20)
            {
                rdstatus = (*tptr);
                // DQ7 = prog value ? ok done
                if (((rdstatus ^ tdata) & 0x80) == 0)
                    break;
                errorcode=MEMD_ERR_WRITE;
                break;
            }
        }
        if (errorcode!=MEMD_ERR_NO) break;
        if(bsize==byteSize) {
            /* last byte */
            data = data & *(ptr);
        }
        /* 16b data ready write it to flash*/
        status = hal_SysEnterCriticalSection();
        *(BankAddr) = 0xa0;
        *(ptr)=data;
        hal_SysExitCriticalSection(status);
    }
    if (errorcode!=MEMD_ERR_NO)
    {
        wsize = owsize;
    } else
    {
        // last data pooling
        while(1)
        {
            rdstatus = (*ptr);
            // DQ7 = prog value ? ok done
            if (((rdstatus ^ data) & 0x80) == 0)
                break;
            // DQ5 = 1
            if ((rdstatus & 0x20) == 0x20)
            {
                rdstatus = (*ptr);
                // DQ7 = prog value ? ok done
                if (((rdstatus ^ data) & 0x80) == 0)
                    break;
                errorcode=MEMD_ERR_WRITE;
                break;
            }
        }
        wsize = bsize;
    }
    
    *pWrittenbyteSize = wsize;
    // Unlock Bypass Exit
    status = hal_SysEnterCriticalSection();
    *(BankAddr) = 0x90;
    *(BankAddr) = 0x00;
   // memd_DYB((UINT8*)(SectAddr ),NULL,TRUE);
    hal_SysExitCriticalSection(status);

    hal_EbcFlashWriteEnable(FALSE);
    return errorcode;
}



// =============================================================================
// memd_FlashRead
// -----------------------------------------------------------------------------
/// This function reads data from the flash. It gets its data from \c flashAddress, 
/// copies \c byteSize bytes to \c buffer. \c pReadbyteSize is filled with the 
/// actual number of read bytes (Equal to \c byteSize, or less in case of error)
///
/// @param flashAddress The byte offset within the flash chip (Take care of 
/// the  positions of the code in the flash)
/// @param byteSize Number of bytes to read from the flash
/// @param pReadbyteSize Filled with the number of bytes actually read
/// @param buffer Buffer to fill with data from flash
/// @return #MEMD_ERR_NO
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashRead(UINT8 *flashAddress,
        UINT32  byteSize, 
        UINT32* pReadbyteSize,
        UINT8*  buffer)
{
    VOLATILE UINT8 * ptr;
    UINT32 phys_add;


    phys_add = (UINT32)flashAddress;
    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")

   
    ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + phys_add);
    
    /* could do aligned read from flash to improve bus accesses as it is uncached */
    memcpy(buffer, (UINT8 *)ptr, byteSize);
    *pReadbyteSize=byteSize;
    return MEMD_ERR_NO;
}



// =============================================================================
// memd_FlashGetGlobalAddress
// -----------------------------------------------------------------------------
/// This function returns the global address in the system memory map 
/// from the flash relative address. 
/// Using the returned address, the user will be able to read directly 
/// from the flash, without using #memd_FlashRead
///
/// Note that the flash can be read directly, but it cannot be written without 
/// using the memd driver functions. That's why the returned address is declared 
/// as \c CONST. The content of the flash can be erased without the help of the 
/// CPU (by sending a command to the flash chip), that's why the returned 
/// address is declared as \c VOLATILE.
///
/// @param flashAddress Byte address within the flash chip to be converted 
/// into a global byte address (take care to the positions of the code in flash)
/// @return The byte global address
// =============================================================================
PUBLIC VOLATILE CONST UINT8* memd_FlashGetGlobalAddress(UINT8 *flashAddress)
{
    VOLATILE CONST UINT8* ptr;
    UINT32 phys_add;

    phys_add = (UINT32)flashAddress;
    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    
    ptr = (VOLATILE CONST UINT8 *)(g_memdFlashBaseAddress + phys_add);
   

    return ptr;
}



PRIVATE MEMD_ERR_T memd_DYB(UINT8 *startFlashAddress, UINT8 *endFlashAddress, BOOL Enable)
{
    VOLATILE UINT16 * flashaddr;
    VOLATILE UINT16 * ptr;
    UINT32 Start, End;

    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits((UINT32)startFlashAddress , &Start, &End);
        endFlashAddress = (UINT8*)End;
    } else

    // Check that start & end addresses are aligned
    if ((UINT32)endFlashAddress != FLASH_SIZE) {
        memd_FlashGetSectorLimits((UINT32)endFlashAddress , &Start, &End);
        if ((UINT32)endFlashAddress != Start) {
            return MEMD_ERR_ALIGN;
        }
    }

    memd_FlashGetSectorLimits((UINT32)startFlashAddress , &Start, &End);
    if ((UINT32)startFlashAddress != Start)
        { return MEMD_ERR_ALIGN; }

    hal_EbcFlashWriteEnable(TRUE);

    flashaddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBANK_MASK));
    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBLOCK_MASK));
    while (Start != (UINT32)endFlashAddress)
    {
        *(flashaddr+0x555) = 0xaa;
        *(flashaddr+0x2aa) = 0x55;
        *(flashaddr+0x555) = 0x48;
        // DYB Set/Clr
        if (Enable)
            *(ptr) = 0x01;
        else
            *(ptr) = 0x00;

        if (End != FLASH_SIZE) {
            memd_FlashGetSectorLimits(End, &Start, &End);
        } else
        {
            Start = End;
        }
        flashaddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBANK_MASK));
        ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBLOCK_MASK));
    }


    if(startFlashAddress != endFlashAddress)
    {
        // Need to exit DYB mode
        *(flashaddr+0x555) = 0xaa;
        *(flashaddr+0x2aa) = 0x55;
        *(flashaddr+0x555) = 0x90;
        *(flashaddr) = 0;
    }

    hal_EbcFlashWriteEnable(FALSE);

    return MEMD_ERR_NO;
}


PRIVATE UINT16 memd_DYBStatus(UINT8 * SectorAddr, UINT16 *DYB)
{
    VOLATILE UINT16 * flashaddr;
    VOLATILE UINT16 * ptr;
    UINT32 Start, End;

    memd_FlashGetSectorLimits((UINT32)SectorAddr, &Start, &End);
    if ((UINT32)SectorAddr != Start)
    {
        *DYB = 0xffff;
        return MEMD_ERR_ALIGN;
    }


    flashaddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBANK_MASK));
    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBLOCK_MASK));

    // DYB Read
    hal_EbcFlashWriteEnable(TRUE);
    *(flashaddr+0x555) = 0xaa;
    *(flashaddr+0x2aa) = 0x55;
    *(flashaddr+0x555) = 0x58;
    *DYB = *(ptr);
    // Need to exit DYB mode
    *(flashaddr+0x555) = 0xaa;
    *(flashaddr+0x2aa) = 0x55;
    *(flashaddr+0x555) = 0x90;
    *(flashaddr) = 0;

    hal_EbcFlashWriteEnable(FALSE);

    return MEMD_ERR_NO;
}


// =============================================================================
// memd_FlashLock
// -----------------------------------------------------------------------------
/// This function protects the sectors between \c start_flashAddress and 
/// \c end_flashAddress.  Addresses must be sector aligned.  The sector 
/// specified by \c end_flashAddress is @b _NOT_ included in the 
/// protect procedure 
///
/// @param startFlashAddress Address of the first sector to protect
/// @param end_LashAddress Address of the first sector not to protect
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashLock(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    return memd_DYB(startFlashAddress, endFlashAddress, TRUE);
}


// =============================================================================
// memd_FlashUnlock
// -----------------------------------------------------------------------------
/// This function unlocks the protection of the flash sectors between 
/// \c start_flashAddress and \c end_flashAddress.  Addresses must be 
/// sector aligned.  The sector specified by \c end_flashAddress is 
/// _NOT_ included in the unprotect procedure 
///
/// @param start_flashAddress Address of the first sector to unprotect
/// @param end_flashAddress Address of the first sector not to unprotect
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// ============================================================================
PUBLIC MEMD_ERR_T memd_FlashUnlock(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    return memd_DYB(startFlashAddress, endFlashAddress, FALSE);
}

// =============================================================================
// memd_FlashGetLockStatus
// -----------------------------------------------------------------------------
/// This function gets the protections status of the sector starting at 
/// \c start_flashAddress.  This addresses must be sector 
/// aligned.
///
/// @param startFlashAddress Address of the beginning of the sector to check.
/// @param lockStatus Pointer to the boolean value retaining the result of the
/// get: \c TRUE if the sector is protected, \c FALSE otherwise. The value is 
/// only valid if the return value is MEMD_ERR_NO;
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashGetLockStatus(UINT8* startFlashAddress, BOOL* lockStatus)
{
    UINT16 dyb;
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ERR_T status = memd_DYBStatus(startFlashAddress, &dyb);
    if (dyb == 1)
    {
        *lockStatus = TRUE;
    }
    else
    {
        *lockStatus = FALSE;
    }
    return status;
}


// =============================================================================
// memd_FlashGetUserFieldInfo
// -----------------------------------------------------------------------------
/// That function get the user data info from memd.def.
///
/// @param userFieldBase The USER_DATA_BASE defined in memd.def.
/// @param userFieldSize The USER_DATA_SIZE defined in memd.def.
/// @param sectorSize The sector size of user data field, it is the large sector size.
///        Some small boot sectors can be regarded as one large sector.
/// @return VOID.
// =============================================================================
PUBLIC VOID memd_FlashGetUserFieldInfo(UINT32 *userFieldBase, UINT32 *userFieldSize, UINT32 *sectorSize, UINT32 *blockSize)
{
    UINT32 Start, End;
    
    *userFieldBase = g_memdFlashLayout.userDataBase;
    *userFieldSize = g_memdFlashLayout.userDataSize;
    *sectorSize = memd_FlashGetSectorLimits(*userFieldBase, &Start, &End);
    *blockSize = g_memdFlashLayout.userDataSectorLogicalBlockSize;
}

