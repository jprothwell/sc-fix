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
//  $HeadURL: http://10.10.100.14/svn/developing1/Sources/edrv/trunk/lcdd/ili9225br/gouda/include/lcdd_config.h $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-07-28 20:17:59 +0800 (星期�? 28 七月 2010) $                     //   
//	$Revision: 835 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file lcdd_config.h                                                       //
/// That file describes the configuration of the structure used to set up     //
/// the AU driver stored in TGT.                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "hal_gouda.h"


#ifndef _LCDD_CONFIG_H_
#define _LCDD_CONFIG_H_


// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
// LCDD_CONFIG_T
// -----------------------------------------------------------------------------
/// The type TGT_LCDD_CONFIG_T is defined in tgt_lcdd_cfg.h using the structure
/// #LCDD_CONFIG_STRUCT_T, the following macro allow to map the correct structure
/// and keep the actual structures different between each models for the
/// documentation.
///
/// Make sure to include lcdd_config.h before lcdd_m.h or tgt_lcdd_cfg.h !
// =============================================================================
#define LCDD_CONFIG_STRUCT_T LCDD_CONFIG_MS1321_GOUDA_STRUCT_T

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// LCDD_CONFIG_STRUCT_T
// -----------------------------------------------------------------------------
/// Configuration structure for the Ili9325 LCD driver, using the Gouda module of the chip.
// =============================================================================
struct LCDD_CONFIG_MS1321_GOUDA_STRUCT_T
{
    HAL_GOUDA_LCD_CONFIG_T   config;
    HAL_GOUDA_LCD_TIMINGS_T  timings[HAL_GOUDA_LCD_TIMINGS_QTY];
};


#endif // _LCDD_CONFIG_H_


