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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/afc.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _AFC_H_
#define _AFC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'afc'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
#define NB_AFC_DATA                              (12)

// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// AFC_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_AFC_BASE                0x00BA2800


typedef volatile struct
{
    REG32                          AFC_Ctrl;                     //0x00000000
    REG32                          AFC_Data;                     //0x00000004
} HWP_AFC_T;

#define hwp_afc                     ((HWP_AFC_T*) KSEG1(REG_AFC_BASE))


//AFC_Ctrl
#define AFC_AFC_REMOVE              (1<<0)
#define AFC_AFC_FLUSH_FIFO          (1<<4)
#define AFC_AFC_FIFO_SPACE(n)       (((n)&15)<<8)
#define AFC_AFC_FORCE_DATA(n)       (((n)&0xFFF)<<16)
#define AFC_AFC_FORCE_UPDATE        (1<<31)

//AFC_Data
#define AFC_AFC_DATA(n)             (((n)&0xFFF)<<0)





#endif
