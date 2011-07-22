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

#ifndef _APS_VERSION_H_
#define _APS_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define APS_VERSION_REVISION                     (1758)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef APS_VERSION_NUMBER
#define APS_VERSION_NUMBER                       (0)
#endif

#ifndef APS_VERSION_DATE
#define APS_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef APS_VERSION_STRING
#define APS_VERSION_STRING                       "APS version string not defined"
#endif

#ifndef APS_VERSION_STRING_WITH_BRANCH
#define APS_VERSION_STRING_WITH_BRANCH           APS_VERSION_STRING " Branch: " "Sources/svc/branches/Total_BJ/aps"
#endif

#define APS_VERSION_STRUCT                       {APS_VERSION_REVISION, \
                                                  APS_VERSION_NUMBER, \
                                                  APS_VERSION_DATE, \
                                                  APS_VERSION_STRING_WITH_BRANCH}

#endif // _APS_VERSION_H_
