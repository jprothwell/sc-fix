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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spc/src/spc_version.c $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file spp_version.c


#include "cs_types.h"
#include "hal_map_engine.h"
#include "chip_id.h"

#define SPC_VERSION_NUMBER SPC_IF_VER

// Tweak the version string
#if (CHIP_BYPASS_ROM == 1)
#define SPC_VERSION_STRING "SPC recompiled (not using rom)"
#elif (defined (REWRITE_ROM_AT_BOOT))
#define SPC_VERSION_STRING "SPC recompiled (FPGA soft rom)"
#else
// Romed version.

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE) // Greenstone chip
#define SPC_VERSION_STRING "SPC in Greenstone chip rom"
#else
#define SPC_VERSION_STRING "SPC in rom"
#endif // CHIP_ASIC_ID

#endif // Recompiled, soft rom or chip ?

// include generated headers (to get svn versions)
#include "spcp_version.h"


// =============================================================================
// g_spcMapVersion
// -----------------------------------------------------------------------------
/// Contains the version of SPC.
// =============================================================================
PROTECTED CONST HAL_MAP_VERSION_T g_spcMapVersion
    = SPC_VERSION_STRUCT;


