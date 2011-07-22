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
//  $HeadURL: http://subversion-server/svn/developing/modem2G/trunk/platform/edrv/lcdd/au/include/lcdd_config.h $ //
//	$Author: xiaoke $                                                        // 
//	$Date: 2009-03-23 10:23:43 +0200 (Mon, 23 Mar 2009) $                     //   
//	$Revision: 7800 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file lcdd_config.h                                                       //
/// That file describes the configuration of the structure used to set up     //
/// the AU driver stored in TGT.                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "hal_lcdc.h"


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
/// Make sure to include fmd_config.h before fmd_m.h or tgt_fmd_cfg.h !
// =============================================================================
#define LCDD_CONFIG_STRUCT_T LCDD_CONFIG_ILI9325_LCDC_STRUCT_T

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// LCDD_CONFIG_STRUCT_T
// -----------------------------------------------------------------------------
/// Configuration structure for the Ili9325 LCD driver, using the LCDC controller module of the chip.
// =============================================================================
struct LCDD_CONFIG_ILI9325_LCDC_STRUCT_T
{
    HAL_LCDC_CONFIG_T   config;
    HAL_LCDC_TIMINGS_T  timings[HAL_LCDC_TIMINGS_QTY];
};


#endif // _LCDD_CONFIG_H_


