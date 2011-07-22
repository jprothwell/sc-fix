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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/esperite/include/comregs.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _COMREGS_H_
#define _COMREGS_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'comregs'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// COMREGS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_BB_COMREGS_BASE         0x0190F000

typedef volatile struct
{
    REG32                          Cause;                        //0x00000000
    REG32                          Mask_Set;                     //0x00000004
    REG32                          Mask_Clr;                     //0x00000008
    /// If accesses to ItReg_Set and ItReg_Clr registers are done simultaneously
    /// from both CPUs and affecting the same bits, the priority is given to set
    /// a bit.
    REG32                          ItReg_Set;                    //0x0000000C
    /// If accesses to ItReg_Set and ItReg_Clr registers are done simultaneously
    /// from both CPUs and affecting the same bits, the priority is given to set
    /// a bit.
    REG32                          ItReg_Clr;                    //0x00000010
} HWP_COMREGS_T;

#define hwp_bbComregs               ((HWP_COMREGS_T*) KSEG1(REG_BB_COMREGS_BASE))


//Cause
#define COMREGS_X_IRQ0_CAUSE(n)     (((n)&15)<<0)
#define COMREGS_X_IRQ0_CAUSE_MASK   (15<<0)
#define COMREGS_X_IRQ0_CAUSE_SHIFT  (0)
#define COMREGS_X_IRQ1_CAUSE(n)     (((n)&15)<<4)
#define COMREGS_X_IRQ1_CAUSE_MASK   (15<<4)
#define COMREGS_X_IRQ1_CAUSE_SHIFT  (4)
#define COMREGS_B_IRQ0_CAUSE(n)     (((n)&15)<<8)
#define COMREGS_B_IRQ0_CAUSE_MASK   (15<<8)
#define COMREGS_B_IRQ0_CAUSE_SHIFT  (8)
#define COMREGS_B_IRQ1_CAUSE(n)     (((n)&15)<<12)
#define COMREGS_B_IRQ1_CAUSE_MASK   (15<<12)
#define COMREGS_B_IRQ1_CAUSE_SHIFT  (12)

//Mask_Set
#define COMREGS_X_IRQ0_MASK_SET(n)  (((n)&15)<<0)
#define COMREGS_X_IRQ0_MASK_SET_MASK (15<<0)
#define COMREGS_X_IRQ0_MASK_SET_SHIFT (0)
#define COMREGS_X_IRQ1_MASK_SET(n)  (((n)&15)<<4)
#define COMREGS_X_IRQ1_MASK_SET_MASK (15<<4)
#define COMREGS_X_IRQ1_MASK_SET_SHIFT (4)
#define COMREGS_B_IRQ0_MASK_SET(n)  (((n)&15)<<8)
#define COMREGS_B_IRQ1_MASK_SET(n)  (((n)&15)<<12)

//Mask_Clr
#define COMREGS_X_IRQ0_MASK_CLR(n)  (((n)&15)<<0)
#define COMREGS_X_IRQ0_MASK_CLR_MASK (15<<0)
#define COMREGS_X_IRQ0_MASK_CLR_SHIFT (0)
#define COMREGS_X_IRQ1_MASK_CLR(n)  (((n)&15)<<4)
#define COMREGS_X_IRQ1_MASK_CLR_MASK (15<<4)
#define COMREGS_X_IRQ1_MASK_CLR_SHIFT (4)
#define COMREGS_B_IRQ0_MASK_CLR(n)  (((n)&15)<<8)
#define COMREGS_B_IRQ1_MASK_CLR(n)  (((n)&15)<<12)

//ItReg_Set
#define COMREGS_X_IRQ0_SET(n)       (((n)&15)<<0)
#define COMREGS_X_IRQ0_SET_MASK     (15<<0)
#define COMREGS_X_IRQ0_SET_SHIFT    (0)
#define COMREGS_X_IRQ1_SET(n)       (((n)&15)<<4)
#define COMREGS_X_IRQ1_SET_MASK     (15<<4)
#define COMREGS_X_IRQ1_SET_SHIFT    (4)
#define COMREGS_B_IRQ0_SET(n)       (((n)&15)<<8)
#define COMREGS_B_IRQ0_SET_MASK     (15<<8)
#define COMREGS_B_IRQ0_SET_SHIFT    (8)
#define COMREGS_B_IRQ1_SET(n)       (((n)&15)<<12)
#define COMREGS_B_IRQ1_SET_MASK     (15<<12)
#define COMREGS_B_IRQ1_SET_SHIFT    (12)
#define COMREGS_X_IRQ(n)            (((n)&0xFF)<<0)
#define COMREGS_X_IRQ_MASK          (0xFF<<0)
#define COMREGS_X_IRQ_SHIFT         (0)
#define COMREGS_B_IRQ(n)            (((n)&0xFF)<<8)
#define COMREGS_B_IRQ_MASK          (0xFF<<8)
#define COMREGS_B_IRQ_SHIFT         (8)

//ItReg_Clr
#define COMREGS_X_IRQ0_CLR(n)       (((n)&15)<<0)
#define COMREGS_X_IRQ1_CLR(n)       (((n)&15)<<4)
#define COMREGS_B_IRQ0_CLR(n)       (((n)&15)<<8)
#define COMREGS_B_IRQ1_CLR(n)       (((n)&15)<<12)





#endif

