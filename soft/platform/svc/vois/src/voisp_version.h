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

#ifndef _VOIS_VERSION_H_
#define _VOIS_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define VOIS_VERSION_REVISION                     (8872)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef VOIS_VERSION_NUMBER
#define VOIS_VERSION_NUMBER                       (0)
#endif

#ifndef VOIS_VERSION_DATE
#define VOIS_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef VOIS_VERSION_STRING
#define VOIS_VERSION_STRING                       "VOIS version string not defined"
#endif

#ifndef VOIS_VERSION_STRING_WITH_BRANCH
#define VOIS_VERSION_STRING_WITH_BRANCH           VOIS_VERSION_STRING " Branch: " "Sources/svc/branches/Total_BJ/vois"
#endif

#define VOIS_VERSION_STRUCT                       {VOIS_VERSION_REVISION, \
                                                  VOIS_VERSION_NUMBER, \
                                                  VOIS_VERSION_DATE, \
                                                  VOIS_VERSION_STRING_WITH_BRANCH}

#endif // _VOIS_VERSION_H_
