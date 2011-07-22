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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/memd/flsh_m36l0r7050t4/src/drv_flsh.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                          //   
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
#include "hal_mem_map.h"
#include "string.h"

#include "memd_m.h"
#include "memdp_debug.h"
#include "memdp.h"
#include "hal_host.h"


// -------------------------------
// Definition
// -------------------------------
/* banks at flash bit 19 */
#define FLASHBANK_MASK      (0x1F<<(20))     // Actually only need 4 bits, but have to use 5 due to shift
#define FLASH_ADDR_MASK     0x03FFFFFF      // 0x0100_0000 to 0x0300_0000, ignore the 0x8 or 0xA
#define MY_MASK 0x00FFFFFF      // FIX MY_MASK is only used for masking address to Block Lock function.  Do it properly?

#define KBYTES              1024
#define MBYTES              (1024*KBYTES)

#define CMD_READ_STATUS     0x70
#define CMD_READ_ESD        0x90
#define CMD_BLOCK_PROT      0x60
#define CMD_BLOCK_LOCK      0x01
#define CMD_BLOCK_UNLOCK    0xD0
#define CMD_READ_ARRAY      0xFF
#define CMD_BLOCK_ERASE_1   0x20
#define CMD_BLOCK_ERASE_2   0xD0
#define CMD_PROG            0x40
#define CMD_SET_CFG_REG_1   0x60
#define CMD_SET_CFG_REG_2   0x03
#define CMD_CLR_STATUS_REG  0x50

#define SR0                 (1<<0)
#define SR1                 (1<<1)
#define SR2                 (1<<2)
#define SR3                 (1<<3)
#define SR4                 (1<<4)
#define SR5                 (1<<5)
#define SR6                 (1<<6)
#define SR7                 (1<<7)



// -------------------------------
// FLASH/RAM configuration
// -------------------------------
#define FLASH_SIZE  16*MBYTES // 16MB


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
/// Base address
UINT32 g_memdFlashBaseAddress  = 0;

// -------------------------------
// Local Declaration
// -------------------------------
PRIVATE MEMD_ERR_T memd_BlockLock(UINT8 *phy_startFlshAddr, UINT8 *phy_endFlshAddr, BOOL Lock);


#ifdef MEMD_FLASH_IS_BURST
#define FLASH_CRD 0x19Cf

PRIVATE VOID HAL_FUNC_INTERNAL 
memd_FlashConfigureBurstMode(REG16* csBase, REG32* pCRE)
{
    // FLASH_CRD[15] : 0 = synch / 1 = asynch
    // FLASH_CRD[13:11] : X Latency 3 = 30MHz / 4 = 40MHz / 5 = 54MHz / 6 = 66MHz
    // FLASH_CRD[10] : Wait Polarity 0 = Active Low / 1 = Active High
    // FLASH_CRD[9] : Data Output Valid Config 0 = 1 Clock cycle / 1 = 2 Clock cycles
    // FLASH_CRD[8] : Wait Config 0 = Asserted during wait / 1 = Asserted 1 cycle before wait
    // FLASH_CRD[7] : 0 = RESERVED / 1 = Sequential
    // FLASH_CRD[6] : Valid Clock Edge 0 = Falling edge / 1 = Rising edge
    // FLASH_CRD[3] : 0 = Wrap / 1 = No wrap
    // FLASH_CRD[2:0] : Burst len 001 = 4 words / 010 = 8 words / 011 = 16 words / 111 Continuous
    //
    //
    // Set Configuration Register
    *(csBase+FLASH_CRD) = CMD_SET_CFG_REG_1;
    *(csBase+FLASH_CRD) = CMD_SET_CFG_REG_2;
    
    {
        UINT32 flush __attribute__((unused)) = *(csBase+FLASH_CRD);
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

    return ((MEMD_FLASH_LAYOUT_T*) &g_memdFlashLayout);
}

#ifdef MEMD_RAM_IS_BURST
#define PSRAM_BCR 0x08590f

PRIVATE VOID HAL_FUNC_INTERNAL 
memd_RamConfigureBurstMode(REG16* csBase, REG32* pCRE)
{
    UINT16 read;

    // Using the CR controlled method 
    // Configure BCR
    // PSRAM_BCR[15] : 0 = synch / 1 = asynch
    // PSRAM_BCR[14] : 0 = variable Latency / 1 = Fixed latency
    // PSRAM_BCR[13:11] : Latency count 011(3)-111(7) clock cycle could be used 
    // PSRAM_BCR[10] : Wait Polarity 0 = Active Low / 1 = Active High
    // PSRAM_BCR[9] : MUST be set to 0
    // PSRAM_BCR[8] : Wait Config 0 = Asserted during wait / 1 = Asserted 1 cycle before wait
    // PSRAM_BCR[7:6] : MUST be set to 0   
    // PSRAM_BCR[5:4] : driver strength bits 0 = Full Drive / 1 = 1/2 Drive / 2 = 1/4 Drive
    // PSRAM_BCR[3] : 0 = Wrap / 1 = No wrap
    // PSRAM_BCR[2:0] : Burst len 001 = 4 words / 010 = 8 words / 011 = 16 words / 100 = 32 words / 111 Continuous
    read=*(csBase+PSRAM_BCR); 
    *pCRE=1; // MEM_FIFO_CRE
    *(csBase+PSRAM_BCR)=read; // read burst with latency = 3 fixed (4 clocks)
    
    {
        UINT32 flush __attribute__((unused)) = *pCRE;
    }
    
    *pCRE=0;

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
	
#ifdef MEMD_RAM_IS_BURST
    hal_EbcSramOpen(&cfg->csConfig, memd_RamConfigureBurstMode);
#else //!MEMD_RAM_IS_BURST
    // Open External SRAM
    hal_EbcCsOpen(HAL_EBC_SRAM, &cfg->csConfig);
#endif //!MEMD_RAM_IS_BURST
   
}


// All parameters are _physical_ addresses
PRIVATE UINT32 memd_FlashGetSectorLimits(UINT32 phy_sectorId, UINT32 *phy_start, UINT32 *phy_end)
{
    UINT32 aStart, aEnd;
    int bankNum = 0;
    int majSecIdx, minSecIdx;
    UINT32 sectSize;

    if (phy_sectorId == g_memdFlashLayout.fullSize)
    {
        *phy_start = g_memdFlashLayout.fullSize;
        *phy_end = g_memdFlashLayout.fullSize;
        return 0;
    }
    else if (phy_sectorId > g_memdFlashLayout.fullSize)
    {
        *phy_start = 0;
        *phy_end = 0;
        MEMD_ASSERT(FALSE, "Out of accessible flash space !!!!");
        return 0;
    }
    else
    {
        while ( (phy_sectorId >= g_memdFlashLayout.bankLayout[bankNum+1].bankStartAddr) &&
                (bankNum < (g_memdFlashLayout.numberOfBank-1)) )
        {
            ++bankNum;
        }
        aStart = g_memdFlashLayout.bankLayout[bankNum].bankStartAddr;

        majSecIdx = 0;
        aEnd = aStart +
            (g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][0] *
            g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1]);
        while (phy_sectorId >= aEnd)
        {
            if (majSecIdx == 2)
            {
                *phy_start = 0;
                *phy_end = 0;
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
        while (phy_sectorId >= aEnd)
        {
            aStart = aEnd;
            aEnd = aStart + sectSize;
            ++minSecIdx;
        }

        *phy_start = aStart;
        *phy_end = aEnd;
        return sectSize;
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
/// @param start_flashAddress The address of the first sector to erase.  Must
/// be a _physical_ address (byte offset from start of Flash)
/// @param end_flashAddress The address of the first sector NOT to erase.
///   If \c NULL, only one sector will be erased 
/// @return #MEMD_ERR_NO, #MEMD_ERR_ERASE, #MEMD_ERR_ALIGN or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed
///
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashErase(UINT8 *startFlashAddress, UINT8 *endFlashAddress)
{
    UINT32 status;
    VOLATILE UINT16 * ptr;
    VOLATILE UINT16 *BankAddr;
    UINT32 phy_Start;
    UINT32 phy_End;
    UINT32 phys_end_add;
    UINT32 phys_start_add;
    MEMD_ERR_T errorcode=MEMD_ERR_NO;
    BOOL isLocked;
    UINT16 rdstatus;

    phys_start_add = (UINT32)startFlashAddress;
    MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
        "flash_address is expected to be a byte offset within the flash chip, not an absolute address");

    // Check that start & end addresses are aligned
    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits(phys_start_add, &phy_Start, &phy_End);
        phys_end_add = phy_End;
    } else
    {
        phys_end_add = (UINT32)endFlashAddress;
        MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute address")
        if (phys_end_add != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(phys_end_add , &phy_Start, &phy_End);
            if (phys_end_add != phy_Start)
            {
                return MEMD_ERR_ALIGN;
            }
        }
    }

    memd_FlashGetSectorLimits(phys_start_add, &phy_Start, &phy_End);
    if (phys_start_add != phy_Start)
    {
        return MEMD_ERR_ALIGN;
    }

    hal_EbcFlashWriteEnable(TRUE);

    BankAddr = NULL;
    while (phy_Start != phys_end_add)
    {
        BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
        // phy_Start should already be aligned to sector boundary, so shouldn't need any more masking
        ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + phy_Start);

       
        memd_FlashGetLockStatus((UINT8*)((UINT32)ptr & MY_MASK),&isLocked);
	 // Re-enable EBC write
        hal_EbcFlashWriteEnable(TRUE);
	
        if(isLocked == TRUE)
        {
           memd_BlockLock((UINT8*)((UINT32)ptr & MY_MASK),NULL,FALSE);
        }
  
        status = hal_SysEnterCriticalSection();		
        // Sector Erase command
        *(BankAddr) = CMD_BLOCK_ERASE_1;
        *(ptr) = CMD_BLOCK_ERASE_2;
        hal_SysExitCriticalSection(status);


        // pooling
        // Wait for Ready, then check status
        do{
             rdstatus = *BankAddr;
        }while((rdstatus & SR7) != SR7);   // Any address in the bank may be used

        // Should probably fix this scheme for reporting errors more cleanly.
        // For now, just prioritize the errors with the most significant error returned
        // in errorcode (order this list from least to most significant)
        if ((rdstatus & SR3) != 0)
        {
            // Vpp Invalid Error
            errorcode=MEMD_ERR_ERASE;
        }
        else if (((rdstatus & SR5) != 0) && ((rdstatus & SR4) != 0))
        {
            // Command Sequence Error
            errorcode=MEMD_ERR_ERASE;
        }
        else if ((rdstatus & SR5) != 0)
        {
            // Erase Error
            errorcode=MEMD_ERR_ERASE;
        }
        else if ((rdstatus & SR1) != 0)
        {
            errorcode=MEMD_ERR_PROTECT;
        }
        
        // Reset to read array mode when every block erase operation is finished.
	status = hal_SysEnterCriticalSection();
        *BankAddr = CMD_READ_ARRAY;
        hal_SysExitCriticalSection(status);

	// Clear status register if any error 
	if(errorcode != MEMD_ERR_NO)
	{
	    status = hal_SysEnterCriticalSection();
            *BankAddr = CMD_CLR_STATUS_REG;
            hal_SysExitCriticalSection(status);
	    hal_EbcFlashWriteEnable(FALSE);
            return errorcode;
	}

        if (phy_End != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(phy_End, &phy_Start, &phy_End);
        } else
        {
            phy_Start = phy_End;
        }
    }
    if (BankAddr != NULL)
    {
        // Return to Read Array mode
        status = hal_SysEnterCriticalSection();
        *BankAddr = CMD_READ_ARRAY;
        hal_SysExitCriticalSection(status);
    }

    // erase done
    hal_EbcFlashWriteEnable(FALSE);
    return errorcode;
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
    UINT16 data;
    UINT32 bsize=0;
    UINT32 wsize=0;
    UINT32 owsize=0;
    MEMD_ERR_T errorcode=MEMD_ERR_NO;
    UINT32 status;
    UINT32 phys_add;
    UINT32 phy_SectAddr;
    UINT32 phy_End;
    BOOL isLocked;
    UINT16 rdstatus;


    *pWrittenbyteSize=0;
    if (byteSize==0)
    {
        return MEMD_ERR_NO;
    }

    hal_EbcFlashWriteEnable(TRUE);

    phys_add = (UINT32)flashAddress;
    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
        "flash_address is expected to be a byte offset within the flash chip, not an absolute address")

    memd_FlashGetSectorLimits((UINT32)flashAddress, &phy_SectAddr, &phy_End);
    BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&FLASHBANK_MASK));
    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));

    // Unlock block
    memd_FlashGetLockStatus((UINT8*)((UINT32)phy_SectAddr & MY_MASK),&isLocked);
	hal_EbcFlashWriteEnable(TRUE);
    if(isLocked == TRUE)
        memd_BlockLock((UINT8*)((UINT32)phy_SectAddr & MY_MASK),NULL,FALSE);

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
    *(BankAddr) = CMD_PROG;
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
        // 

	do{
           rdstatus = *BankAddr;
	}while((rdstatus & SR7) != SR7);
        
        // Should probably fix this scheme for reporting errors more cleanly.
        // For now, just prioritize the errors with the most significant error returned
        // in errorcode (order this list from least to most significant)
        if ((rdstatus & SR4) != 0)
        {
            // Some other programming error, should be decoded, but maybe do it later
            errorcode=MEMD_ERR_WRITE;
        }
        else if ((rdstatus & SR1) != 0)
        {
            errorcode=MEMD_ERR_PROTECT;
        }

        if (errorcode!=MEMD_ERR_NO) break;
        if(bsize==byteSize) {
            /* last byte */
            *BankAddr = CMD_READ_ARRAY;
            data = data & *(ptr);
        }
        /* 16b data ready write it to flash*/
        status = hal_SysEnterCriticalSection();
        *(BankAddr) = CMD_PROG;
        *(ptr)=data;
        hal_SysExitCriticalSection(status);
    }
    if (errorcode!=MEMD_ERR_NO)
    {
        wsize = owsize;
    } else
    {
        // last data pooling
        do{
           rdstatus = *BankAddr;
	}while((rdstatus & SR7) != SR7);
     
        if ((rdstatus & SR4) != 0)
        {
            // Some other programming error, should be decoded, but maybe do it later
            errorcode=MEMD_ERR_WRITE;
        }
        else if ((rdstatus & SR1) != 0)
        {
            errorcode=MEMD_ERR_PROTECT;
        }

        wsize = bsize;
    }
		
    *pWrittenbyteSize = wsize;
  
    // return to Read Array mode
    status = hal_SysEnterCriticalSection();
    *BankAddr = CMD_READ_ARRAY;
    hal_SysExitCriticalSection(status);
       
    if(errorcode != MEMD_ERR_NO)
    {
    	status = hal_SysEnterCriticalSection();
        *BankAddr = CMD_CLR_STATUS_REG;          
        hal_SysExitCriticalSection(status);
    }

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
        "flash_address is expected to be a byte offset within the flash chip, not an absolute address")

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
        "flash_address is expected to be a byte offset within the flash chip, not an absolute address");

    ptr = (VOLATILE CONST UINT8 *)(g_memdFlashBaseAddress + phys_add);

    return ptr;
}


// =============================================================================
// memd_BlockLock
// -----------------------------------------------------------------------------
/// This Locks or Unlocks a block (sector) or range of blocks.
/// Flash Write Enable must already be set.
///
/// @param phy_startFlshAddr Start address of the block in flash to start
/// locking.  This must be aligned on a block.
/// @param phy_endFlshAddr End address of the block in flash to stop locking
/// (inclusive).  This must also be aligned on a block.  If this parameter is
/// NULL, then only one block (starting at phy_startFlshAddr) will be locked.
/// @return MEMD_ERR_T error type (alignment errors only)
// =============================================================================
PRIVATE MEMD_ERR_T memd_BlockLock(UINT8 *phy_startFlshAddr, UINT8 *phy_endFlshAddr, BOOL Lock)
{
    UINT32 phy_Start;
    UINT32 phy_End;
    UINT32 status;
    VOLATILE UINT16 * CurBankAddr;
    VOLATILE UINT16 * PrevBankAddr;

    // Check that start & end addresses are aligned
    if (phy_endFlshAddr == NULL)
    {
        memd_FlashGetSectorLimits((UINT32)phy_startFlshAddr, &phy_Start, &phy_End);
        phy_endFlshAddr = (UINT8*)phy_End;
    } else
    {
        if ((UINT32)phy_endFlshAddr != FLASH_SIZE) {
            memd_FlashGetSectorLimits((UINT32)phy_endFlshAddr , &phy_Start, &phy_End);
            if ((UINT32)phy_endFlshAddr != phy_Start) {
                return MEMD_ERR_ALIGN;
            }
        }
    }

    memd_FlashGetSectorLimits((UINT32)phy_startFlshAddr, &phy_Start, &phy_End);
    if ((UINT32)phy_startFlshAddr != phy_Start)
    {
        return MEMD_ERR_ALIGN;
    }

    PrevBankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));

    while (phy_Start != (UINT32)phy_endFlshAddr)
    {
        VOLATILE UINT16 * SectAddr;
        SectAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + phy_Start);
        CurBankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
        status = hal_SysEnterCriticalSection();
        // If Bank Address is the same as previous Bank Address, continue
        // otherwise, need to put old Bank into Read Array mode and
        // open new Bank.
        if (CurBankAddr != PrevBankAddr)
        {
            // Close Prev Bank
            *PrevBankAddr = CMD_READ_ARRAY;
            PrevBankAddr = CurBankAddr;
        }

        // Lock Set/Clr
        if (Lock)
        {
            *SectAddr = CMD_BLOCK_PROT;
            *SectAddr = CMD_BLOCK_LOCK;
        }
        else
        {
            *SectAddr = CMD_BLOCK_PROT;
            *SectAddr = CMD_BLOCK_UNLOCK;
        }
        hal_SysExitCriticalSection(status);

        if (phy_End != FLASH_SIZE) {
            memd_FlashGetSectorLimits(phy_End, &phy_Start, &phy_End);
        } else
        {
            phy_Start = phy_End;
        }
    }

    // Return to Read Array mode
    status = hal_SysEnterCriticalSection();
    *PrevBankAddr = CMD_READ_ARRAY;
    hal_SysExitCriticalSection(status);

    return MEMD_ERR_NO;
}

PRIVATE MEMD_ERR_T memd_BlockLockStatus(UINT8 * phy_SectorAddr, UINT16 *Locked)
{
    VOLATILE UINT16 * BankAddr;
    VOLATILE UINT16 * BlockAddr;
    UINT16 DeviceCode;
    UINT32 phy_Start;
    UINT32 phy_End;
    UINT32 status;

    memd_FlashGetSectorLimits((UINT32)phy_SectorAddr, &phy_Start, &phy_End);
    if ((UINT32)phy_SectorAddr != phy_Start)
    {
        *Locked = 0xffff;
        return MEMD_ERR_ALIGN;
    }

    BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
    BlockAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + phy_Start);

    // Block Protection Read
    hal_EbcFlashWriteEnable(TRUE);
    status = hal_SysEnterCriticalSection();
    *BankAddr = CMD_READ_ESD;
    DeviceCode = *(UINT16*)(BankAddr + 0x1);
    *Locked = *(UINT16*)(BlockAddr + 0x2);
    *BankAddr = CMD_READ_ARRAY;
    hal_SysExitCriticalSection(status);
    hal_EbcFlashWriteEnable(FALSE);

    // Write out Device Code for debug check
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
/// @param startFlashAddress Address of the first sector to protect.  Must be
/// a physical byte offset within the flash chip, not logical address.
/// @param end_LashAddress Address of the first sector not to protect.  Must
/// be a physical byte offset within the flash chip, not logical address.
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashLock(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute address");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute address");
    /* FIX Must enable Flash for writing in EBC */
    return memd_BlockLock(startFlashAddress, endFlashAddress, TRUE);
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
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute address");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute address");
    /* FIX Must enable Flash for writing in EBC */
    return memd_BlockLock(startFlashAddress, endFlashAddress, FALSE);
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
    UINT16 Locked;
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute address");
    MEMD_ERR_T status = memd_BlockLockStatus(startFlashAddress, &Locked);
    if (Locked == 0)
    {
        *lockStatus = FALSE;
    }
    else
    {
        /* FIX 'extra' Locked states */
        // Actually, there are 3 other states:
        // 0001 = Locked
        // 0002 = Unlocked and locked-down
        // 0003 = Locked and locked-down
        *lockStatus = TRUE;
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

