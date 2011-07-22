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
//    THIS FILE WAS GENERATED AUTOMATICALLY BY THE MAKE PROCESS.
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                             
//                                                                              
//==============================================================================

#ifndef _@{MOD}_VERSION_H_
#define _@{MOD}_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define @{MOD}_VERSION_REVISION                     (@{LOCAL_REVISION})

// =============================================================================
//  TYPES
// =============================================================================

#ifndef @{MOD}_VERSION_NUMBER
#define @{MOD}_VERSION_NUMBER                       (0)
#endif

#ifndef @{MOD}_VERSION_DATE
#define @{MOD}_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef @{MOD}_VERSION_STRING
#define @{MOD}_VERSION_STRING                       "@{MOD} version string not defined"
#endif

#ifndef @{MOD}_VERSION_STRING_WITH_BRANCH
#define @{MOD}_VERSION_STRING_WITH_BRANCH           @{MOD}_VERSION_STRING " Branch: " @{LOCAL_BRANCH}
#endif

#define @{MOD}_VERSION_STRUCT                       {@{MOD}_VERSION_REVISION, \
                                                  @{MOD}_VERSION_NUMBER, \
                                                  @{MOD}_VERSION_DATE, \
                                                  @{MOD}_VERSION_STRING_WITH_BRANCH}

#endif // _@{MOD}_VERSION_H_
