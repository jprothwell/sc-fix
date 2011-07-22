////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2009, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/memd/flsh_romulator/src/drv_flsh.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-09-01 19:25:00 +0800 (Wed, 01 Sep 2010) $                     //   
//	$Revision: 1857 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_flsh.c                                                          //
/// That file provides the driver implementation for the romulator, reflecting//
/// any flash combo.                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "hal_ebc.h"
#include "hal_sys.h"

#include "string.h"

#include "memd_m.h"
#include "memdp_debug.h"
#include "memdp.h"


// If SAFE_MODE is 1, it will only erase the sector if the address is on the sector boundary
// otherwise, the driver will erase the sector that contains the address.
#define SAFE_MODE       1
#define KBYTES  1024
#define MBYTES (1024*KBYTES)

// -------------------------------
// FLASH/RAM configuration
// -------------------------------
#define FLASH_SIZE  16*MBYTES // 16MB

#define ROMU_1V1    0x0101
#define ROMU_1V2    0x0102
#define ROMU_1V3    0x0103
#define ROMU_1V5    0x0105
#define ROMU_1V6    0x0106
#define ROMU_1V7    0x0107
#define ROMU_1V8    0x0108

#if !defined(ROMU_VER)
#define ROMU_VER ROMU_1V1
#endif

#if (ROMU_VER == 0)
#error "invalid ROMU_VER version, check  source code for supported versions."
#endif
	
// -------------------------------
// FLASH SETTINGS
// -------------------------------
// The Romulator FLASH settings are defined by the Romulator
// Driver, as those settings are not board dependent.
#if !defined(MEMD_FLASH_IS_ADMUX)
#define FLASH_TIME_SETTING { \
    .rdLatHalfMode= 0, \
    .rdLat=         5, \
    .oedlyHalfMode= 0, \
    .oedly=         1, \
    .pageLat=       0, \
    .wrLatHalfMode= 0, \
    .wrLat=         4, \
    .wedlyHalfMode= 0, \
    .wedly=         1, \
    .relax=         2, \
    .forceRelax=    1}
#define FLASH_MODE_SETTING { \
    .csEn=          1, \
    .polarity=      0, \
    .admuxMode=     0, \
    .writeAllow=    0, \
    .wbeMode=       0, \
    .bedlyMode=     0, \
    .waitMode=      0, \
    .pageSize=      0, \
    .pageMode=      0, \
    .writePageMode= 0, \
    .burstMode=     0, \
    .writeBurstMode=0, \
    .advAsync=      0, \
    .advwePulse=    0, \
    .admuxDly=      0, \
    .writeSingle=   0, \
    .writeHold=     0}
#else //MEMD_FLASH_IS_ADMUX
#define FLASH_TIME_SETTING {                                            \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 6,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 1,                                                \
    .pageLat        = 2,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 5,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 1,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define FLASH_MODE_SETTING {                                            \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 1,                                                \
    .writeAllow     = 0,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#endif



    

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
/// Base address
UINT32 g_memdFlashBaseAddress  = 0;



/**
    Configuration structure used by the EBC to configure the flash
    Chip Select
*/
PRIVATE CONST HAL_EBC_CS_CFG_T memd_FlashSettings = 
    {FLASH_TIME_SETTING, FLASH_MODE_SETTING } ;


#define FLASH_ADDR_MASK 0x00ffffff

BOOL g_memdFlashLockOverride = FALSE;


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
    g_memdFlashBaseAddress = (UINT32) hal_EbcFlashOpen(HAL_SYS_FREQ_39M,
                            &memd_FlashSettings, NULL);

    return ((MEMD_FLASH_LAYOUT_T*) &g_memdFlashLayout);
}



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
    MEMD_ASSERT(cfg != NULL, "Opening RAM with a NULL configuration");

    // Open External SRAM
    hal_EbcCsOpen(HAL_EBC_SRAM, &cfg->csConfig);
}



//PRIVATE VOID memd_LockOverride(BOOL Enable)
//{
//    g_memdFlashLockOverride = Enable;
//}



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




//PRIVATE BOOL memd_LockCheck(VOLATILE UINT16 *Addr, volatile UINT16 *Ptr)
//{
//    return FALSE;
//}




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
    UINT32 start, end;
    UINT32 pErase;
    UINT32 phys_end_add, phys_start_add;
#if (ROMU_VER > ROMU_1V1)
    UINT32 status;
#endif
    phys_start_add = (UINT32)startFlashAddress;
    MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    // Check that start & end addresses are aligned
    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits(phys_start_add, &start, &end);
        phys_end_add = end;
    } else
    {
        phys_end_add = (UINT32)endFlashAddress;
        MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")
        memd_FlashGetSectorLimits(phys_end_add, &start, &end);
        if (phys_end_add != start)
        {
            #if SAFE_MODE
            return MEMD_ERR_ALIGN;
            #endif
        }
    }

    memd_FlashGetSectorLimits(phys_start_add, &start, &end);
    if (phys_start_add != start)
    {
        #if SAFE_MODE
        return MEMD_ERR_ALIGN;
        #endif
    }

    hal_EbcFlashWriteEnable(TRUE);

#if (ROMU_VER > ROMU_1V1)
    // Unlock Bypass Entry
    status = hal_SysEnterCriticalSection();
    *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0xAAA) = 0xaa;
    *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0x554) = 0x55;
    *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0xAAA) = 0x20;
    hal_SysExitCriticalSection(status);
    //SXS_TRACE(TSTDOUT,"hal_flash: unlock\n");
#endif

    while (start != phys_end_add)
    {
        if (!g_memdFlashLockOverride)
        {
            // Check Lock
        }

        // Fill the sector with FFFFs
        for (pErase = start; pErase < end; pErase+=2)
        {
#if (ROMU_VER > ROMU_1V1)
            status = hal_SysEnterCriticalSection();
            *(VOLATILE UINT16 *)(g_memdFlashBaseAddress) = 0xa0;
            *(UINT16*)(pErase+g_memdFlashBaseAddress) = 0xffff;
            hal_SysExitCriticalSection(status);
#else
            *(UINT16*)(pErase+g_memdFlashBaseAddress) = 0xffff;
#endif
        }
        if (end != g_memdFlashLayout.fullSize)
        {
            memd_FlashGetSectorLimits(end, &start, &end);
        } else
        {
            start = end;
        }
    }

#if (ROMU_VER > ROMU_1V1)
    // Unlock Bypass Exit
    status = hal_SysEnterCriticalSection();
    *(VOLATILE UINT16 *)(g_memdFlashBaseAddress) = 0x90;
    hal_SysExitCriticalSection(status);
#endif

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
    VOLATILE UINT16 * ptr;
    UINT16 data;
    UINT32 bsize=0, wsize=0;
    MEMD_ERR_T errorcode=MEMD_ERR_NO;
    UINT32 phys_add;
#if (ROMU_VER > ROMU_1V1)
    UINT32 status;
#endif

    *pWrittenbyteSize=0;
    if (byteSize==0)
    {
        return MEMD_ERR_NO;
    }

    hal_EbcFlashWriteEnable(TRUE);

    phys_add = (UINT32)flashAddress;
    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")

    if (!g_memdFlashLockOverride)
    {
    }

    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));


#if (ROMU_VER > ROMU_1V1)
    // Unlock Bypass Entry
    status = hal_SysEnterCriticalSection();
    *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0xAAA) = 0xaa;
    *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0x554) = 0x55;
    *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0xAAA) = 0x20;
    hal_SysExitCriticalSection(status);
    //SXS_TRACE(TSTDOUT,"hal_flash: unlock\n");
#endif

    // If not aligned on 16 bits, need to read low byte of existing word and modify upper byte
    if((phys_add&1) == 1) {
        data = *(ptr);
    } else {
        data = (*buffer) | ~0x00ff;
        buffer ++;
        bsize ++;
    }

    // Now write the rest
    while(wsize<byteSize) {
        if(bsize<byteSize) {
            data = data & (((*buffer) << 8) | 0x00ff);
            buffer ++;
            bsize ++;
        } else {
            /* last byte */
            data = data & *(ptr);
        }
        if (((*(ptr)) & data) == data)
        {
            // Simulate real flash, can only switch 1 -> 0
#if (ROMU_VER > ROMU_1V1)
            status = hal_SysEnterCriticalSection();
            *(VOLATILE UINT16 *)(g_memdFlashBaseAddress) = 0xa0;
            *(ptr)=data;        
            hal_SysExitCriticalSection(status);
#else
            *(ptr)=data;        
#endif
        } else
        {
            // you are trying to turn a 0 into a 1 - spansion will return an error
            // so is this emulator
            errorcode = MEMD_ERR_WRITE;
            break;
        }
        
        wsize = bsize;
        ptr+=1;
        /* write done, next data ? */
        if (bsize<byteSize) {
            data = (*buffer) | ~0x00ff;
            buffer ++;
            bsize ++;
        }
    } 
    *pWrittenbyteSize = wsize;

#if (ROMU_VER > ROMU_1V1)
    // Unlock Bypass Exit
    status = hal_SysEnterCriticalSection();
    *(VOLATILE UINT16 *)(g_memdFlashBaseAddress) = 0x90;
    hal_SysExitCriticalSection(status);
#endif

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

    ptr = (VOLATILE CONST UINT8*)(g_memdFlashBaseAddress + phys_add);
    return ptr;
}



PRIVATE MEMD_ERR_T memd_SetLock(UINT8 *startFlashAddress, UINT8 *endFlashAddress, BOOL Enable)
{
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
PUBLIC MEMD_ERR_T memd_FlashLock(UINT8* startFlashAddress, UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    return memd_SetLock(startFlashAddress, endFlashAddress, TRUE);
}



// =============================================================================
// memd_FlashUnLock
// -----------------------------------------------------------------------------
/// This function unlocks the protection of the flash sectors between 
/// \c start_flashAddress and \c end_flashAddress.  Addresses must be 
/// sector aligned.  The sector specified by \c end_flashAddress is 
/// _NOT_ included in the unprotect procedure 
///
/// @param start_flashAddress Address of the first sector to unprotect
/// @param end_flashAddress Address of the first sector not to unprotect
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashUnlock(UINT8* startFlashAddress, UINT8* endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    return memd_SetLock(startFlashAddress, endFlashAddress, FALSE);
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
/// only valid if the return value is MEMD_ERR_NO
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashGetLockStatus(UINT8* startFlashAddress, BOOL* lockStatus)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    return 0;
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
PUBLIC VOID memd_FlashGetUserFieldInfo(UINT32 *userFieldBase, UINT32 *userFieldSize, UINT32 *sectorSize)
{
    UINT32 Start, End;
    
    *userFieldBase = g_memdFlashLayout.userDataBase;
    *userFieldSize = g_memdFlashLayout.userDataSize;
    *sectorSize = memd_FlashGetSectorLimits(*userFieldBase, &Start, &End);
}



// =============================================================================
// memd_FlashRomulatorSetRamMode
// -----------------------------------------------------------------------------
/// Set the Romulator in RAM mode. This function is only effective when using
/// a romulator device. When using a real Flash Combo, the function returns 
/// \c FALSE and does nothing.
///
/// When the romualtor is in RAM mode, it can be accessed as a RAM, that is to 
/// say read and written without sending any command, but through direct 
/// addressing.
///
/// @param  enable Enable or disable the RAM mode.
/// @return \c TRUE if the operation could be done and the switch done,
///         \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL memd_FlashRomulatorSetRamMode(BOOL enable)
{
#if (ROMU_VER >= ROMU_1V6)
    UINT32 status;

    if (enable)
    {
        // Set romulator in RAM mode.
        status = hal_SysEnterCriticalSection();
        *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0xAAA) = 0xaa;
        *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0x554) = 0x55;
        *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0xAAA) = 0x30;
        hal_SysExitCriticalSection(status);
    }
    else
    {
        // Set romulator in Flash mode.
        status = hal_SysEnterCriticalSection();
        *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0xAAA) = 0xaa;
        *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0x554) = 0x55;
        *(VOLATILE UINT16 *)(g_memdFlashBaseAddress+0xAAA) = 0x40;
        hal_SysExitCriticalSection(status);
    }
    return TRUE;
#else
    MEMD_TRACE(MEMD_WARN_TRC, 0, "MEMD: Romulator Version Incompatible With Ram Mode Feature.");
    return FALSE;
#endif
 
}


