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

#ifndef _APPLICATION_VERSION_H_
#define _APPLICATION_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define APPLICATION_VERSION_REVISION                     (9008)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef APPLICATION_VERSION_NUMBER
#define APPLICATION_VERSION_NUMBER                       (0)
#endif

#ifndef APPLICATION_VERSION_DATE
#define APPLICATION_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef APPLICATION_VERSION_STRING
#define APPLICATION_VERSION_STRING                       "APPLICATION version string not defined"
#endif

#ifndef APPLICATION_VERSION_STRING_WITH_BRANCH
#define APPLICATION_VERSION_STRING_WITH_BRANCH           APPLICATION_VERSION_STRING " Branch: " "Sources/application/branches/4C4D"
#endif

#define APPLICATION_VERSION_STRUCT                       {APPLICATION_VERSION_REVISION, \
                                                  APPLICATION_VERSION_NUMBER, \
                                                  APPLICATION_VERSION_DATE, \
                                                  APPLICATION_VERSION_STRING_WITH_BRANCH}

#endif // _APPLICATION_VERSION_H_
