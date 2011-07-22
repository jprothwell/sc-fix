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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/mcd/mcd_sdmmc/include/mcd_config.h $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file mcd_config.h
/// That file describes the configuration of the structure used to set up
/// the SDMMC version of the MCD driver.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
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

#define MCD_CONFIG_STRUCT_T MCD_CONFIG_SDMMC_STRUCT_T

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// MCD_CONFIG_T
// -----------------------------------------------------------------------------
/// Configuration structure for the MCD driver on SDMMC Interface
// =============================================================================
struct MCD_CONFIG_SDMMC_STRUCT_T
{
    /// Gpio Connected to socket to detect card insertion/removal .
    /// (set to #HAL_GPIO_NONE) if not available.
    HAL_GPIO_GPIO_ID_T  cardDetectGpio;
    /// Define the polarity of the above GPIO: 
    /// \c TRUE GPIO is high when card is present,
    /// \c FALSE GPIO is low when card is present.
    BOOL                gpioCardDetectHigh;
    /// Wether or not the DAT3/CD line has a pull-down and can be used for 
    /// minimalist new card detection (no insertion/removal interrupt.)
    BOOL                dat3HasPullDown;
};




// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================

#endif // _MCD_CONFIG_H_


