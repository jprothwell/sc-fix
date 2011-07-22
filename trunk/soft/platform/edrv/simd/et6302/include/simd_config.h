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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file simd_config.h                                                       //
/// That file describes the configuration of the structure used to set up     //
/// the sim in TGT.                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _SIMD_CONFIG_H_
#define _SIMD_CONFIG_H_

#include "cs_types.h"
#include "hal_spi.h"
#include "hal_gpio.h"

#define SIMD_CONFIG_STRUCT_T SIMD_CONFIG_ET6302_STRUCT_T



// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// SIMD_CONFIG_ET6302_STRUCT_T
// -----------------------------------------------------------------------------
/// Configuration structure for the SIM driver CS
// =============================================================================
struct SIMD_CONFIG_ET6302_STRUCT_T
{    
    HAL_SPI_ID_T            spiBus;
    HAL_SPI_CS_T            spiCs;
};



#endif // _SIMD_CONFIG_H_




