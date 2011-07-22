////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2010, Coolsand Technologies, Inc.            //
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
/// @file atvd_config.h
/// That file describes the configuration of the structure used to set up
/// the ATVD structure stored in TGT for analogTV driver.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "hal_gpio.h"
#include "hal_i2c.h"


#ifndef _ATVD_CONFIG_H_
#define _ATVD_CONFIG_H_

// =============================================================================
// ATVD_CONFIG_STRUCT_T
// -----------------------------------------------------------------------------
/// The type ATVD_CONFIG_STRUCT_T is defined in tgt_atvd_cfg.h using the structure
/// #ATVD_CONFIG_STRUCT_T, the following macro allow to map the correct structure
/// and keep the actual structures different between each models for the
/// documentation.
/// Describes the board specific configuration of the RDA5888S BT chip
// ============================================================================
struct ATVD_CONFIG_STRUCT_T
{
    HAL_I2C_BUS_ID_T    i2cBusId;
    HAL_APO_ID_T        pinPdn;
};



// =============================================================================
//  FUNCTIONS
// =============================================================================

///  @} <- End of the atvd group 

#endif // _ATVD_CONFIG_H_


