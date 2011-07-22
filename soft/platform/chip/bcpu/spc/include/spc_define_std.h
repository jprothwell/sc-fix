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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spc/include/spc_define_std.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
/// Contains defines for spc.xmd
//
//==============================================================================

#ifndef _SPC_DEFINE_STD_H_
#define _SPC_DEFINE_STD_H_



// =============================================================================
//  MACROS
// =============================================================================
#define SPC_ITLV_BUFF_TX_DEDICATED               (22*4)
#define SPC_ITLV_BUFF_RX_DEDICATED               (22*32)
#define SPC_ITLV_RX_BUFF_OFFSET                  (128)
#define SPC_ITLV_TX_BUFF_OFFSET                  (4*4)
#define SPC_TMP_ITLV_RX_BUFF_OFFSET              (5*128)
#define SPC_PDCH_DATA_BLOCK_SIZE                 (14)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SPC_PDCH_DATA_BLOCK_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT32 SPC_PDCH_DATA_BLOCK_T[14];




#endif

