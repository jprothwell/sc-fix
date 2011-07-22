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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/memd/include/memd_config.h $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                         //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file memd_config.h                                                       //
/// That file defines the configuration structures used by MEMD.              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _MEMD_CONFIG_H
#define _MEMD_CONFIG_H

#include "cs_types.h"
#include "hal_ebc.h"

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
// MEMD_FLASH_CONFIG_T
// -----------------------------------------------------------------------------
/// That type describes the settings to apply to configure the flash driver
/// to get the optimal performances.
// =============================================================================
typedef struct
{
    CONST HAL_EBC_CS_CFG_T csConfig;
} MEMD_FLASH_CONFIG_T;

// =============================================================================
// MEMD_RAM_CONFIG_T
// -----------------------------------------------------------------------------
/// That type describes the settings to apply to configure the ram driver
/// to get the optimal performances.
// =============================================================================
typedef struct
{
    CONST HAL_EBC_CS_CFG_T csConfig;
} MEMD_RAM_CONFIG_T;

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================


#endif // _MEMD_CONFIG_H

