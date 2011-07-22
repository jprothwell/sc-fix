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
/// @file gsensor_config.h
/// That file describes the configuration of the structure used to set up
///  
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _GSENSOR_CONFIG_H_
#define _GSENSOR_CONFIG_H_

#include "cs_types.h"

#include "hal_i2c.h"
#include "hal_gpio.h"



// =============================================================================
// GSENSOR_CONFIG_STRUCT_T
// -----------------------------------------------------------------------------
/// The type GSENSOR_CONFIG_STRUCT_T is defined in tgt_fmd_cfg.h using the structure
/// #GSENSOR_CONFIG_STRUCT_T, the following macro allow to map the correct structure
/// and keep the actual structures different between each models for the
/// documentation.
///
/// Make sure to include gsensor_config.h before gsensor_m.h or tgt_gsensor_cfg.h !
// =============================================================================

#define GSENSOR_CONFIG_STRUCT_T GSENSOR_CONFIG_MMA7660FC_STRUCT_T

#include "gsensor_m.h"


// =============================================================================
// GSENSOR_CONFIG_MMA7660FC_STRUCT_T
// -----------------------------------------------------------------------------
/// Describes the board specific configuration of the gsensor chip
// ============================================================================

#if 0
struct GSENSOR_CONFIG_MMA7660FC_STRUCT_T
{
    /// I2C bus used to configure the chip
    HAL_I2C_BUS_ID_T        i2cBusId;
    HAL_GPIO_GPIO_ID_T      gsensorGpio;
    HAL_APO_ID_T            scl_i2c;
    HAL_GPIO_GPIO_ID_T      sda_i2c;    
};
#endif

struct GSENSOR_CONFIG_MMA7660FC_STRUCT_T
{
    /// I2C bus used to configure the chip
    
    HAL_I2C_BUS_ID_T        i2cBusId;
    
    HAL_GPIO_GPIO_ID_T      gsensorGpio;
    
    HAL_GPIO_GPIO_ID_T      scl_i2c_gpio;  // if do not used the gpio SCL ,set it HAL_GPIO_NONE

    HAL_GPIO_GPO_ID_T       scl_i2c_gpo;   // if do not used gpo for SCL ,set it HAL_GPO_NONE
    
    HAL_GPIO_GPIO_ID_T      sda_i2c;    
};

#endif // 


