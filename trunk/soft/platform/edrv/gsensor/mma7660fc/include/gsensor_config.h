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
/// @file fmd_config.h
/// That file describes the configuration of the structure used to set up
/// the FMD structure stored in TGT for RDA5802 driver.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _GSENSOR_CONFIG_H_
#define _GSENSOR_CONFIG_H_

#include "cs_types.h"

#include "hal_i2c.h"
#include "hal_gpio.h"



// =============================================================================
// FMD_CONFIG_T
// -----------------------------------------------------------------------------
/// The type GSENSOR_CONFIG_STRUCT_T is defined in tgt_fmd_cfg.h using the structure
/// #GSENSOR_CONFIG_STRUCT_T, the following macro allow to map the correct structure
/// and keep the actual structures different between each models for the
/// documentation.
///
///  
// =============================================================================
#define GSENSOR_CONFIG_STRUCT_T GSENSOR_CONFIG_MMA7660FC_STRUCT_T

#include "gsensor_m.h"


/// @defgroup  
/// @{


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// 
// -----------------------------------------------------------------------------
///  
// =============================================================================


// =============================================================================
// 
// -----------------------------------------------------------------------------
///  
// ============================================================================
struct GSENSOR_CONFIG_MMA7660FC_STRUCT_T
{
    /// I2C bus used to configure the chip
    HAL_I2C_BUS_ID_T        i2cBusId;
    HAL_GPIO_GPIO_ID_T      gsensorGpio;
};


// =============================================================================
//  FUNCTIONS
// =============================================================================

///  

#endif // _FMD_CONFIG_H_


