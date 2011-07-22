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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/pa_ctrl.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _PA_CTRL_H_
#define _PA_CTRL_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'pa_ctrl'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// PA_CTRL_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_PA_CTRL_BASE            0x00BA2000


typedef volatile struct
{
    REG32                          ctrl;                         //0x00000000
    /// The 5 available ramps are programmed thanks to 30 registers (6 registers
    /// per ramp). 
    /// For ramp N 
    /// Ramp[6*N+0] : Value index 0, 1, 2 
    /// Ramp[6*N+1] : Value index 3, 4, 5 
    /// Ramp[6*N+2] : Value index 6, 7, 8 
    /// Ramp[6*N+3] : Value index 9, 10, 11 
    /// Ramp[6*N+4] : Value index 12, 13, 14 
    /// Ramp[6*N+5] : Value index 15 only
    REG32                          Ramp[30];                     //0x00000004
} HWP_PA_CTRL_T;

#define hwp_paCtrl                  ((HWP_PA_CTRL_T*) KSEG1(REG_PA_CTRL_BASE))


//ctrl
#define PA_CTRL_ENABLE_ENABLE       (1<<0)
#define PA_CTRL_ENABLE_DISABLE      (0<<0)
#define PA_CTRL_RAMP_DURATION_16QBITS (1<<4)
#define PA_CTRL_RAMP_DURATION_32QBITS (0<<4)
#define PA_CTRL_FORCED_VAL(n)       (((n)&0x3FF)<<16)
#define PA_CTRL_FORCE_H             (1<<31)

//Ramp[0]
#define PA_CTRL_VALUE_0(n)          (((n)&0x3FF)<<0)
#define PA_CTRL_VALUE_1(n)          (((n)&0x3FF)<<10)
#define PA_CTRL_VALUE_2(n)          (((n)&0x3FF)<<20)





#endif
