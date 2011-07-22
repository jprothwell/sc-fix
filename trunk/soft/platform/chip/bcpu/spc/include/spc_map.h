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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spc/include/spc_map.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
/// This file contains the portion of SPC's memory map that will be accessible through
/// CoolWatcher (or any CoolXml tool).
//
//==============================================================================

#ifndef _SPC_MAP_H_
#define _SPC_MAP_H_


#include "spc_mailbox.h"
#include "spc_ctx.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SPC_MAP_ACCESS_T
// -----------------------------------------------------------------------------
/// Type used to define the accessible structures of SPC.
// =============================================================================
typedef struct
{
    SPC_MAILBOX_T*                 mailbox;                      //0x00000000
    SPC_CONTEXT_T*                 context;                      //0x00000004
} SPC_MAP_ACCESS_T; //Size : 0x8





#endif

