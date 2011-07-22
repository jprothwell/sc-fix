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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/au/ebc/include/lcdd_config.h $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file lcdd_config.h                                                       //
/// That file describes the configuration of the structure used to set up     //
/// the AU driver stored in TGT.                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _LCDD_CONFIG_H_
#define _LCDD_CONFIG_H_


#include "cs_types.h"

#include "hal_ebc.h"


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
#define LCDD_CONFIG_STRUCT_T LCDD_CONFIG_AU_EBC_STRUCT_T


#include "tgt_lcdd_cfg.h"


// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// LCDD_SCREEN_CONFIG_T
// -----------------------------------------------------------------------------
/// Type of the configuration structure for a AU screen.
// =============================================================================
typedef struct
{
    /// CS on which the screen is plugged.
    HAL_EBC_CS_T     csNum;
    /// EBC CS configuration.
    HAL_EBC_CS_CFG_T csConfig;
    /// MA line number, on the external address bus, used to select between
    /// the command and data register. Depending one the chip select used,
    /// the addressable size implies that all the address lines cannot be used
    /// All value between 0 and 18 should be correct.
    UINT32  rsLine;
} LCDD_SCREEN_CONFIG_T;


// =============================================================================
// LCDD_CONFIG_STRUCT_T
// -----------------------------------------------------------------------------
/// Configuration structure for the AU LCDD driver, including both screens.
// =============================================================================
struct LCDD_CONFIG_AU_EBC_STRUCT_T
{
    LCDD_SCREEN_CONFIG_T main;
    LCDD_SCREEN_CONFIG_T sub;
};




#endif // _LCDD_CONFIG_H_


