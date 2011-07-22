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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/esperite/include/itlv.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _ITLV_H_
#define _ITLV_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'itlv'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// ITLV_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_ITLV_BASE               0x01904000

typedef volatile struct
{
    REG32                          command;                      //0x00000000
    REG32                          status;                       //0x00000004
    REG32                          burst_base;                   //0x00000008
    REG32                          frame_base;                   //0x0000000C
    REG32                          int_status;                   //0x00000010
} HWP_ITLV_T;

#define hwp_itlv                    ((HWP_ITLV_T*) KSEG1(REG_ITLV_BASE))


//command
#define ITLV_NB_BITS(n)             (((n)&0x7FF)<<20)
#define ITLV_BURST_OFFSET(n)        (((n)&31)<<8)
#define ITLV_ITLV_TYPE(n)           (((n)&15)<<4)
#define ITLV_ITLV_TYPE_MASK         (15<<4)
#define ITLV_ITLV_TYPE_SHIFT        (4)
#define ITLV_ITLV_TYPE_TYPE_1A      (0<<4)
#define ITLV_ITLV_TYPE_TYPE_1B_HD   (1<<4)
#define ITLV_ITLV_TYPE_TYPE_1C      (2<<4)
#define ITLV_ITLV_TYPE_TYPE_2A      (3<<4)
#define ITLV_ITLV_TYPE_TYPE_2B      (4<<4)
#define ITLV_ITLV_TYPE_TYPE_3       (5<<4)
#define ITLV_ITLV_TYPE_TYPE_H1      (6<<4)
#define ITLV_ITLV_TYPE_TYPE_H2      (7<<4)
#define ITLV_ITLV_TYPE_TYPE_H3      (8<<4)
#define ITLV_ITLV_TYPE_TYPE_H4      (8<<4)
#define ITLV_ITLV_TYPE_TYPE_D1      (10<<4)
#define ITLV_ITLV_TYPE_TYPE_D2      (11<<4)
#define ITLV_ITLV_TYPE_TYPE_D3      (12<<4)
#define ITLV_INT_MASK               (1<<3)
#define ITLV_SV_SIZE_SV_4BIT        (0<<2)
#define ITLV_SV_SIZE_SV_8BIT        (1<<2)
#define ITLV_DITLV_START            (1<<1)
#define ITLV_ITLV_START             (1<<0)

//status
#define ITLV_BUSY                   (1<<0)

//burst_base
#define ITLV_BURST_BASE(n)          (((n)&0x7FF)<<4)

//frame_base
#define ITLV_FRAME_BASE(n)          (((n)&0x1FFF)<<2)

//int_status
#define ITLV_IT_STATUS              (1<<16)
#define ITLV_IT_CAUSE               (1<<0)





#endif

