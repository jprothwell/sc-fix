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

#ifndef _VPP_VERSION_H_
#define _VPP_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define VPP_VERSION_REVISION                     (8959)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef VPP_VERSION_NUMBER
#define VPP_VERSION_NUMBER                       (0)
#endif

#ifndef VPP_VERSION_DATE
#define VPP_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef VPP_VERSION_STRING
#define VPP_VERSION_STRING                       "VPP version string not defined"
#endif

#ifndef VPP_VERSION_STRING_WITH_BRANCH
#define VPP_VERSION_STRING_WITH_BRANCH           VPP_VERSION_STRING " Branch: " "Sources/vpp/branches/Total_BJ"
#endif

#define VPP_VERSION_STRUCT                       {VPP_VERSION_REVISION, \
                                                  VPP_VERSION_NUMBER, \
                                                  VPP_VERSION_DATE, \
                                                  VPP_VERSION_STRING_WITH_BRANCH}

#endif // _VPP_VERSION_H_
