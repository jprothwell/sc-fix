//==============================================================================
//                                                                              
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.              
//                            All Rights Reserved                               
//                                                                              
//      This source code is the property of Coolsand Technologies and is        
//      confidential.  Any  modification, distribution,  reproduction or        
//      exploitation  of  any content of this file is totally forbidden,        
//      except  with the  written permission  of  Coolsand Technologies.        
//                                                                              
//==============================================================================
//                                                                              
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spal/src/spal_version.c $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file spal_version.c


#include "cs_types.h"
#include "hal_map_engine.h"

/// @todo: for next chips place the version information in rom

#define SPAL_VERSION_NUMBER SPC_IF_VER

#if ((CHIP_BYPASS_ROM == 1) || defined (REWRITE_ROM_AT_BOOT))
// special build use generated headers
#include "spalp_version.h"
#else
// use something to identify the rom code (here chip ID)
#include "chip_id.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE) // Greenstone chip
#define SPAL_VERSION_STRUCT { \
    .revision = 12063, \
    .number = 1, \
    .date = 20080407,\
    .string = "SPAL in Greenstone chip ROM" }
#else
// might want to change SPAL_VERSION_STRING depending on chips (?)
#define SPAL_VERSION_STRING "SPAL"
// use generated headers here
#include "spalp_version.h"

#endif
#endif



// =============================================================================
// g_spalMapVersion
// -----------------------------------------------------------------------------
/// Contains the version of SPAL.
// =============================================================================
PROTECTED CONST HAL_MAP_VERSION_T g_spalMapVersion
    = SPAL_VERSION_STRUCT;

