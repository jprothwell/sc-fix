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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/src/bcpu_registermap.c $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file bcpu_registermap.c

#include "bcpu_registermap.h"


// =============================================================================
// g_spalMapVersion
// -----------------------------------------------------------------------------
/// Contains the version of SPAL.
// =============================================================================
EXPORT PROTECTED HAL_MAP_VERSION_T g_spalMapVersion;


// =============================================================================
// g_sppMapVersion
// -----------------------------------------------------------------------------
/// Contains the version of SPP.
// =============================================================================
EXPORT PROTECTED HAL_MAP_VERSION_T g_sppMapVersion;


// =============================================================================
// g_spcMapVersion
// -----------------------------------------------------------------------------
/// Contains the version of SPC.
// =============================================================================
EXPORT PROTECTED HAL_MAP_VERSION_T g_spcMapVersion;


// =============================================================================
// g_spcMapAccess
// -----------------------------------------------------------------------------
/// Contains the addresses of the structures of SPC that will be accessible from
/// a remote PC (e.g. through CoolWatcher).
/// XCPU does not need to know the type of this, so just get the symbol by it's
/// name.
// =============================================================================
EXPORT PROTECTED VOID* g_spcMapAccess;



// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// bcpu_RegisterYourself
// -----------------------------------------------------------------------------
/// This function registers the module itself to HAL so that the version and
/// the map accessor are filled. Then, the CSW get version function and the
/// CoolWatcher get version command will work.
/// THIS FUNCTION IS IN BCPU MODULE BUT WILL BE EXECUTED BY XCPU !!
// =============================================================================
PUBLIC VOID bcpu_RegisterYourself(VOID)
{
    // Fill the map structure with HAL's version and accessible structure map
    hal_MapEngineRegisterModule(HAL_MAP_ID_SPAL,
                                &g_spalMapVersion,
                                NULL);    
    hal_MapEngineRegisterModule(HAL_MAP_ID_SPP,
                                &g_sppMapVersion,
                                NULL);    
    hal_MapEngineRegisterModule(HAL_MAP_ID_SPC,
                                &g_spcMapVersion,
                                &g_spcMapAccess);    
}
