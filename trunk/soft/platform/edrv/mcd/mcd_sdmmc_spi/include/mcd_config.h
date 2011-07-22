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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/edrv/branches/Total_BJ/mcd/mcd_spi/include/mcd_config.h $ //
//  $Author:  $
//  $Date:  $
//  $Revision:  $
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file mcd_config.h                                                       //
/// That file describes the configuration of the structure used to set up     //
/// the AU driver stored in TGT.                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "hal_spi.h"
#include "hal_gpio.h"


#ifndef _MCD_CONFIG_H_
#define _MCD_CONFIG_H_




// =============================================================================
// MCD_CONFIG_SDMMC_STRUCT_T
// -----------------------------------------------------------------------------
/// Configuration structure for the MCD driver on SDMMC Interface
// =============================================================================

// =============================================================================
//  MACROS
// =============================================================================




// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// MCD_CONFIG_CARD_TYPE_T
// -----------------------------------------------------------------------------
/// Mcd device type.
// =============================================================================
typedef enum  _mcd_config_card_type_t
{
    /// SDMMC.
    MCD_CONFIG_CARD_TYPE_SDMMC,
    /// SPI.
    MCD_CONFIG_CARD_TYPE_SPI
}MCD_CONFIG_CARD_TYPE_T;

// =============================================================================
// MCD_CONFIG_SDMMC_SPI_STRUCT_T
// -----------------------------------------------------------------------------
/// Configuration structure for the MCD_SDMMC SPI driver CS
// =============================================================================

typedef struct _mcd_sdmmc_spi_if
{
    /// Device type,include SDMMC and SPI.
    /// If type set to MCD_CONFIG_CARD_TYPE_SPI,these fields are valid:usedSpiId,usedSpiCs0,usedSpiCs1 and cardDetectGpio.
    /// If type set to MCD_CONFIG_CARD_TYPE_SDMMC,these fields are valid:cardDetectGpio,gpioCardDetectHigh and dat3HasPullDown.
    MCD_CONFIG_CARD_TYPE_T type;
    /// Define SPI identity.
    HAL_SPI_ID_T             usedSpiId;
    /// define chip select for CS0.
    HAL_SPI_CS_T            usedSpiCs0;
    /// define chip select for CS1.
    HAL_SPI_CS_T            usedSpiCs1;
    /// Gpio Connected to socket to detect card insertion/removal .
    /// (set to #HAL_GPIO_NONE) if not available.
    HAL_GPIO_GPIO_ID_T  cardDetectGpio;
    /// Define the polarity of the above GPIO: 
    /// TRUE GPIO is high when card is present,
    ///FALSE GPIO is low when card is present.
    BOOL gpioCardDetectHigh;
    /// Wether or not the DAT3/CD line has a pull-down and can be used for 
    /// minimalist new card detection (no insertion/removal interrupt.)
    BOOL dat3HasPullDown;
}MCD_SDMMC_SPI_IF;

struct MCD_CONFIG_SDMMC_SPI_STRUCT_T
{
    MCD_SDMMC_SPI_IF mcd_if[2];
};

typedef struct MCD_CONFIG_SDMMC_SPI_STRUCT_T MCD_CONFIG_STRUCT_T;



// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================

#endif // _MCD_CONFIG_H_


