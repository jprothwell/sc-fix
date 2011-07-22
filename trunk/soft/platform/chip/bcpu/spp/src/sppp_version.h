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

#ifndef _SPP_VERSION_H_
#define _SPP_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define SPP_VERSION_REVISION                     (7030)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef SPP_VERSION_NUMBER
#define SPP_VERSION_NUMBER                       (0)
#endif

#ifndef SPP_VERSION_DATE
#define SPP_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef SPP_VERSION_STRING
#define SPP_VERSION_STRING                       "SPP version string not defined"
#endif

#ifndef SPP_VERSION_STRING_WITH_BRANCH
#define SPP_VERSION_STRING_WITH_BRANCH           SPP_VERSION_STRING " Branch: " "Sources/chip/branches/gallite441/bcpu/spp"
#endif

#define SPP_VERSION_STRUCT                       {SPP_VERSION_REVISION, \
                                                  SPP_VERSION_NUMBER, \
                                                  SPP_VERSION_DATE, \
                                                  SPP_VERSION_STRING_WITH_BRANCH}

#endif // _SPP_VERSION_H_
