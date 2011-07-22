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

#ifndef _PAL_VERSION_H_
#define _PAL_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define PAL_VERSION_REVISION                     (7545)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef PAL_VERSION_NUMBER
#define PAL_VERSION_NUMBER                       (0)
#endif

#ifndef PAL_VERSION_DATE
#define PAL_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef PAL_VERSION_STRING
#define PAL_VERSION_STRING                       "PAL version string not defined"
#endif

#ifndef PAL_VERSION_STRING_WITH_BRANCH
#define PAL_VERSION_STRING_WITH_BRANCH           PAL_VERSION_STRING " Branch: " "Sources/chip/branches/gallite441/pal"
#endif

#define PAL_VERSION_STRUCT                       {PAL_VERSION_REVISION, \
                                                  PAL_VERSION_NUMBER, \
                                                  PAL_VERSION_DATE, \
                                                  PAL_VERSION_STRING_WITH_BRANCH}

#endif // _PAL_VERSION_H_
