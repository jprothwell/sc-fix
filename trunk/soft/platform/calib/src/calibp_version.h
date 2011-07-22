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

#ifndef _CALIB_VERSION_H_
#define _CALIB_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define CALIB_VERSION_REVISION                     (8782)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef CALIB_VERSION_NUMBER
#define CALIB_VERSION_NUMBER                       (0)
#endif

#ifndef CALIB_VERSION_DATE
#define CALIB_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef CALIB_VERSION_STRING
#define CALIB_VERSION_STRING                       "CALIB version string not defined"
#endif

#ifndef CALIB_VERSION_STRING_WITH_BRANCH
#define CALIB_VERSION_STRING_WITH_BRANCH           CALIB_VERSION_STRING " Branch: " "Sources/platform/trunk/calib"
#endif

#define CALIB_VERSION_STRUCT                       {CALIB_VERSION_REVISION, \
                                                  CALIB_VERSION_NUMBER, \
                                                  CALIB_VERSION_DATE, \
                                                  CALIB_VERSION_STRING_WITH_BRANCH}

#endif // _CALIB_VERSION_H_
