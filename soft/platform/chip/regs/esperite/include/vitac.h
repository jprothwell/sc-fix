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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/esperite/include/vitac.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _VITAC_H_
#define _VITAC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'vitac'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// VITAC_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_VITAC_BASE              0x01906000

typedef volatile struct
{
    /// Any write to this register clears VITAC's interrupt line.
    REG32                          command;                      //0x00000000
    REG32                          status;                       //0x00000004
    REG32                          ch_symb_addr;                 //0x00000008
    REG32                          exp_symb_addr;                //0x0000000C
    REG32                          pm_base_addr;                 //0x00000010
    REG32                          out_base_addr;                //0x00000014
    REG32                          H0_param;                     //0x00000018
    REG32                          HL_param;                     //0x0000001C
    /// This register bank stores the less significant bit of the output from the
    /// coder for a particular code. The kth butterfly uses the bit k of this register.
    /// Also used to store/init PM[0;5] in 8PSK equalization mode: RES_POLY0=PM[0;1],
    /// RES_POLY3=PM[2;3]and RES_POLY2=PM[4;5]
    REG32                          res_poly[3];                  //0x00000020
    /// This register bank stores the past symbol history of each node in 8PSK equalization
    /// mode. Used for init only. Each register stores a 4-states history for 2 nodes
    /// each byte corresponding to a state.
    REG32                          sHist[4];                     //0x0000002C
} HWP_VITAC_T;

#define hwp_vitac                   ((HWP_VITAC_T*) KSEG1(REG_VITAC_BASE))


//command
#define VITAC_START_GMSK_EQU        (1<<0)
#define VITAC_START_8PSK_EQU        (1<<1)
#define VITAC_START_DEC             (1<<2)
#define VITAC_START_TB              (1<<3)
#define VITAC_INT_MASK              (1<<4)
#define VITAC_PUNCTURED             (1<<5)
#define VITAC_NB_STATES(n)          (((n)&3)<<6)
#define VITAC_NB_STATES_MASK        (3<<6)
#define VITAC_NB_STATES_SHIFT       (6)
#define VITAC_NB_STATES_VITAC_16_STATES (0<<6)
#define VITAC_NB_STATES_VITAC_32_STATES (1<<6)
#define VITAC_NB_STATES_VITAC_64_STATES (2<<6)
#define VITAC_NB_STATES_VITAC_8PSK_STATES (3<<6)
#define VITAC_BKWD_TRELLIS          (1<<8)
#define VITAC_CODE_RATE(n)          (((n)&7)<<9)
#define VITAC_BM_SHIFT_LEV(n)       (((n)&15)<<12)
#define VITAC_SV_SHIFT_LEV(n)       (((n)&15)<<16)
#define VITAC_NB_SYMBOLS(n)         (((n)&0x3FF)<<20)

//status
#define VITAC_EQU_GMSK_PENDING      (1<<0)
#define VITAC_EQU_8PSK_PENDING      (1<<1)
#define VITAC_DEC_PENDING           (1<<2)
#define VITAC_TB_PENDING            (1<<3)
#define VITAC_IT_CAUSE              (1<<16)
#define VITAC_IT_STATUS             (1<<20)

//ch_symb_addr
#define VITAC_ADDR(n)               (((n)&0x1FFF)<<2)

//exp_symb_addr
//#define VITAC_ADDR(n)             (((n)&0x1FFF)<<2)

//pm_base_addr
//#define VITAC_ADDR(n)             (((n)&0x1FFF)<<2)

//out_base_addr
//#define VITAC_ADDR(n)             (((n)&0x1FFF)<<2)

//H0_param
#define VITAC_H0(n)                 (((n)&0xFFFFFFFF)<<0)

//HL_param
#define VITAC_HL_I_PART(n)          (((n)&0x1FFF)<<0)
#define VITAC_HL_Q_PART(n)          (((n)&0x1FFF)<<16)

//res_poly
#define VITAC_RES_POLY(n)           (((n)&0xFFFFFFFF)<<0)

//sHist
#define VITAC_STATE0_0(n)           (((n)&7)<<0)
#define VITAC_STATE0_1(n)           (((n)&7)<<4)
#define VITAC_STATE0_2(n)           (((n)&7)<<8)
#define VITAC_STATE0_3(n)           (((n)&7)<<12)
#define VITAC_STATE1_0(n)           (((n)&7)<<16)
#define VITAC_STATE1_1(n)           (((n)&7)<<20)
#define VITAC_STATE1_2(n)           (((n)&7)<<24)
#define VITAC_STATE1_3(n)           (((n)&7)<<28)





#endif

