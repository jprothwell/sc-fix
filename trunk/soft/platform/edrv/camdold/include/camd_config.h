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
//
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/camdold/include/camd_config.h $
//  $Author: yanshengzhang $
//  $Date: 2011-05-06 21:29:02 +0800 (Fri, 06 May 2011) $
//  $Revision: 7456 $
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _CAMD_CONFIG_H_
#define _CAMD_CONFIG_H_

#include "hal_i2c.h"
#ifdef __PRJ_WITH_SPICAM__
#include "hal_gpio.h"
#endif
// =============================================================================
// CAMD_CONFIG_T
// -----------------------------------------------------------------------------
/// Configuration structure for camera driver.
/// Defines parameters such as the I2C bus ID, etc.
// =============================================================================
typedef struct
{
    /// I2C bus ID
    HAL_I2C_BUS_ID_T i2cBusId;
#ifdef __PRJ_WITH_SPICAM__
    /// SPI camera pin :CSB
    HAL_APO_ID_T  spiCamPinCSB;
    /// SPI camera pin :overflow_m
    HAL_APO_ID_T  spiCamPinOverflowM;
    /// SPI camera pin :overflow_s
    HAL_APO_ID_T  spiCamPinOverflowS;
#endif
} CAMD_CONFIG_T;


#endif // _CAMD_CONFIG_H_











