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

#ifndef _SPC_VERSION_H_
#define _SPC_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define SPC_VERSION_REVISION                     (8657)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef SPC_VERSION_NUMBER
#define SPC_VERSION_NUMBER                       (0)
#endif

#ifndef SPC_VERSION_DATE
#define SPC_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef SPC_VERSION_STRING
#define SPC_VERSION_STRING                       "SPC version string not defined"
#endif

#ifndef SPC_VERSION_STRING_WITH_BRANCH
#define SPC_VERSION_STRING_WITH_BRANCH           SPC_VERSION_STRING " Branch: " "Sources/chip/branches/gallite441/bcpu/spc"
#endif

#define SPC_VERSION_STRUCT                       {SPC_VERSION_REVISION, \
                                                  SPC_VERSION_NUMBER, \
                                                  SPC_VERSION_DATE, \
                                                  SPC_VERSION_STRING_WITH_BRANCH}

#endif // _SPC_VERSION_H_
