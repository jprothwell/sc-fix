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
//    THIS FILE WAS GENERATED FROM ITS CORRESPONDING XML VERSION WITH COOLXML.  
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                             
//                                                                              
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spc/include/spc_ctx.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
/// @file spc.h
///  This File contains SPC context structures and enums.
///  
///  @author Coolsand Technologies, Inc.
///  @version 1.0
///  @date january 2008
///  
/// 
//
//==============================================================================

#ifndef _SPC_CTX_H_
#define _SPC_CTX_H_


#if (SPC_IF_VER == 0)
    #include "spc_ctx_v0.h"
#elif (SPC_IF_VER == 1)
    #include "spc_ctx_v1.h"
#elif (SPC_IF_VER == 2)
    #include "spc_ctx_v2.h"
#else
    #include "spc_ctx_std.h"
#endif

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================



#endif

