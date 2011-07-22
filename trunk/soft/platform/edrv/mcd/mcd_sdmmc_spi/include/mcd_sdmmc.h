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
//  $HeadURL: svn://atlas/SVN_MIRROR/dev_mirror/modem2G/trunk/platform/edrv/mcd/include/mcd_spi.h$ //
//  $Author: caoxh $                                                          //
//  $Date: 2011-05-16 17:04:27 +0800 (星期五, 29 四月 2011) $                                                                    //
//  $Revision: 7330 $                                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
/// @file mcd_spi.h                                                             //
////////////////////////////////////////////////////////////////////////////////


#ifndef _MCD_SDMMC_H_
#define _MCD_SDMMC_H_

#include "cs_types.h"


// =============================================================================
//  Functions
// =============================================================================

// =============================================================================
// mcd_sdmmc_SetCardDetectHandler
// -----------------------------------------------------------------------------
/// Register a handler for card detection
///
/// @param handler function called when insertion/removal is detected.
// =============================================================================
PUBLIC MCD_ERR_T mcd_sdmmc_SetCardDetectHandler(MCD_CARD_DETECT_HANDLER_T handler);


// =============================================================================
// mcd_sdmmc_Open
// -----------------------------------------------------------------------------
/// Open the MMC/SD chip
/// This function does the init process of the MMC chip, including reseting
/// the chip, sending a command of init to MMC, and reading the CSD
/// configurations.
///
/// @param mcdCsd Pointer to the structure where the MMC CSD register info
/// are going to be written.
///
/// @param mcdVer is t card version.
// =============================================================================
PUBLIC MCD_ERR_T mcd_sdmmc_Open(MCD_CSD_T* mcdCsd, MCD_CARD_VER mcdVer,MCD_SDMMC_SPI_IF* mcdConfig);


// =============================================================================
// mcd_sdmmc_CardDetectUpdateStatus
// -----------------------------------------------------------------------------
/// Return the card status
///
/// @return Card status see #MCD_STATUS_T
// =============================================================================
PUBLIC VOID mcd_sdmmc_CardDetectHandler(VOID);

// =============================================================================
// mcd_sdmmc_GetCardSize
// -----------------------------------------------------------------------------
/// Get card size
///
/// @param size Structure use to store size of memory card
// =============================================================================
PUBLIC MCD_ERR_T mcd_sdmmc_GetCardSize(MCD_CARD_SIZE_T* size);

// =============================================================================
// mcd_sdmmc_Write
// -----------------------------------------------------------------------------
/// Write a block of data to MMC.
///
/// This function is used to write blocks of data on the MMC.
/// @param startAddr Start Adress  of the MMC memory block where the
/// data will be written
/// @param blockWr Pointer to the block of data to write. Must be aligned 
/// on a 32 bits boundary.
/// @param size Number of bytes to write. Must be an interger multiple of the 
/// sector size of the card.
// =============================================================================
PUBLIC MCD_ERR_T mcd_sdmmc_Write(UINT32 startAddr, CONST UINT8* blockWr, UINT32 size);


// =============================================================================
// mcd_sdmmc_Read
// -----------------------------------------------------------------------------
/// Read using pattern mode.
/// @param startAddr: of the MMC memory block where the data
/// will be read
/// @param blockRd Pointer to the buffer where the data will be stored. Must be aligned 
/// on a 32 bits boundary.
/// @param size Number of bytes to read. Must be an interger multiple of the 
/// sector size of the card.
// =============================================================================
PUBLIC MCD_ERR_T mcd_sdmmc_Read(UINT32 startAddr, UINT8* blockRd, UINT32 size);


// =============================================================================
// mcd_sdmmc_Close
// -----------------------------------------------------------------------------
/// Close MCD.
///
/// To be called at the end of the operations
/// @return MCD_ERR_NO if a response with a good crc was received,
///         MCD_ERR_CARD_NO_RESPONSE if no reponse was received within the 
/// driver configured timeout.
///          MCD_ERR_CARD_RESPONSE_BAD_CRC if the received response presented
///  a bad CRC.
///         MCD_ERR_CARD_TIMEOUT if the card timedout during procedure.
// =============================================================================
PUBLIC MCD_ERR_T mcd_sdmmc_Close(VOID);

// =============================================================================
// mcd_sdmmc_CardStatus
// -----------------------------------------------------------------------------
/// Return the card status
///
/// @return Card status see #MCD_STATUS_T
// =============================================================================
PUBLIC MCD_STATUS_T mcd_sdmmc_CardStatus(VOID);


#ifdef DUAL_TFLASH_SUPPORT
PUBLIC MCD_ERR_T mcd_sdmmc_SwitchOperationTo(UINT8 card_number);        
#endif



PUBLIC MCD_ERR_T mcd_sdmmc_GetCardSize(MCD_CARD_SIZE_T* size);
PUBLIC MCD_ERR_T mcd_sdmmc_GetWRBlockSize(UINT32* size);

/// @} <- End of the sdmmc group

#endif // _MCD_M_H_







