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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spp/src/spp_version.c $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file spp_version.c


#include "cs_types.h"
#include "hal_map_engine.h"

#define SPP_VERSION_NUMBER SPC_IF_VER

#if ((CHIP_BYPASS_ROM == 1) || defined (REWRITE_ROM_AT_BOOT))
// special build use generated headers
#include "sppp_version.h"
#else
// use something to identify the rom code (here chip ID)
#include "chip_id.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE) // Greenstone chip
#define SPP_VERSION_STRUCT { \
    .revision = 12063, \
    .number = 1, \
    .date = 20080407,\
    .string = "SPP in Greenstone chip ROM" }
#else
// might want to change SPP_VERSION_STRING depending on chips (?)
#define SPP_VERSION_STRING "SPP"
// use generated headers here
#include "sppp_version.h"

#endif
#endif


// =============================================================================
// g_sppMapVersion
// -----------------------------------------------------------------------------
/// Contains the version of SPP.
// =============================================================================
PROTECTED CONST HAL_MAP_VERSION_T g_sppMapVersion
    = SPP_VERSION_STRUCT;


