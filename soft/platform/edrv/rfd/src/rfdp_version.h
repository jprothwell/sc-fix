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

#ifndef _RFD_VERSION_H_
#define _RFD_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define RFD_VERSION_REVISION                     (8963)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef RFD_VERSION_NUMBER
#define RFD_VERSION_NUMBER                       (0)
#endif

#ifndef RFD_VERSION_DATE
#define RFD_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef RFD_VERSION_STRING
#define RFD_VERSION_STRING                       "RFD version string not defined"
#endif

#ifndef RFD_VERSION_STRING_WITH_BRANCH
#define RFD_VERSION_STRING_WITH_BRANCH           RFD_VERSION_STRING " Branch: " "Sources/edrv/trunk/rfd"
#endif

#define RFD_VERSION_STRUCT                       {RFD_VERSION_REVISION, \
                                                  RFD_VERSION_NUMBER, \
                                                  RFD_VERSION_DATE, \
                                                  RFD_VERSION_STRING_WITH_BRANCH}

#endif // _RFD_VERSION_H_
