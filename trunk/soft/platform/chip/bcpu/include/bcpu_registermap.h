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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/include/bcpu_registermap.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file bcpu_registermap.h
///  This File contains SPC API
///  
///  @mainpage Modem2G BCPU registering to HAL (for version and map accessor)
///
///  This document describe the interface used by XCPU to get the BCPU modules
///  in the registery.
///
///  @author Coolsand Technologies, Inc.
///  @version 1.0
///  @date december 2008
///  
///  @defgroup bcpu_registermap 
///  @{
//
//==============================================================================

#ifndef _BCPU_REGISTERMAP_H_
#define _BCPU_REGISTERMAP_H_

#include "cs_types.h"
#include "hal_map_engine.h"

// =============================================================================
// bcpu_RegisterYourself
// -----------------------------------------------------------------------------
/// This function registers the module itself to HAL so that the version and
/// the map accessor are filled. Then, the CSW get version function and the
/// CoolWatcher get version command will work.
/// THIS FUNCTION IS IN BCPU MODULE BUT WILL BE EXECUTED BY XCPU !!
// =============================================================================
PUBLIC VOID bcpu_RegisterYourself(VOID);

#endif // _BCPU_REGISTERMAP_H_
