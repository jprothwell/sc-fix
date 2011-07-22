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

#ifndef _DLL_VERSION_H_
#define _DLL_VERSION_H_

// =============================================================================
//  MACROS
// =============================================================================

#define DLL_VERSION_REVISION                     (8251)

// =============================================================================
//  TYPES
// =============================================================================

#ifndef DLL_VERSION_NUMBER
#define DLL_VERSION_NUMBER                       (0)
#endif

#ifndef DLL_VERSION_DATE
#define DLL_VERSION_DATE                         (BUILD_DATE)
#endif

#ifndef DLL_VERSION_STRING
#define DLL_VERSION_STRING                       "DLL version string not defined"
#endif

#ifndef DLL_VERSION_STRING_WITH_BRANCH
#define DLL_VERSION_STRING_WITH_BRANCH           DLL_VERSION_STRING " Branch: " "Sources/csw/branches/4C4D/base/dll"
#endif

#define DLL_VERSION_STRUCT                       {DLL_VERSION_REVISION, \
                                                  DLL_VERSION_NUMBER, \
                                                  DLL_VERSION_DATE, \
                                                  DLL_VERSION_STRING_WITH_BRANCH}

#endif // _DLL_VERSION_H_
