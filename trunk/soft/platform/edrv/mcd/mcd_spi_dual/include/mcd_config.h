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
/// @file lcdd_config.h                                                       //
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
// 
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================

#define MCD_CONFIG_STRUCT_T MCD_CONFIG_DUAL_SDMMC_STRUCT_T

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// MCD_CONFIG_T
// -----------------------------------------------------------------------------
/// Configuration structure for the MCD_SPI driver CS
// =============================================================================
struct MCD_CONFIG_DUAL_SDMMC_STRUCT_T
{
    HAL_SPI_ID_T            usedSpiId;
    HAL_SPI_CS_T            usedSpiCs0;
    HAL_SPI_CS_T            usedSpiCs1;
    HAL_GPIO_GPIO_ID_T      tmpGpioOverCs0;
    HAL_GPIO_GPIO_ID_T      tmpGpioOverCs1;
    HAL_GPIO_GPIO_ID_T      cardDetectGpio;
} ;




// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================

#endif // _MCD_CONFIG_H_


