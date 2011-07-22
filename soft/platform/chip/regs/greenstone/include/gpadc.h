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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/gpadc.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _GPADC_H_
#define _GPADC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'gpadc'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// GPADC_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_GPADC_BASE              0x00BA1000


typedef volatile struct
{
    REG32                          gpadc_ch_ctrl;                //0x00000000
    REG32                          gpadc_ch0;                    //0x00000004
    REG32                          gpadc_ch1;                    //0x00000008
    REG32                          gpadc_ch2;                    //0x0000000C
    REG32                          gpadc_ch3;                    //0x00000010
    REG32                          gpadc_data;                   //0x00000014
} HWP_GPADC_T;

#define hwp_gpadc                   ((HWP_GPADC_T*) KSEG1(REG_GPADC_BASE))


//gpadc_ch_ctrl
#define GPADC_CH0_EN                (1<<0)
#define GPADC_CH1_EN                (1<<1)
#define GPADC_CH2_EN                (1<<2)
#define GPADC_CH3_EN                (1<<3)
#define GPADC_ATP(n)                (((n)&7)<<8)
#define GPADC_ATP_122US             (0<<8)
#define GPADC_ATP_100MS             (1<<8)
#define GPADC_ATP_10MS              (2<<8)
#define GPADC_ATP_1MS               (3<<8)
#define GPADC_ATP_250MS             (4<<8)
#define GPADC_ATP_500MS             (5<<8)
#define GPADC_ATP_1S                (6<<8)
#define GPADC_ATP_2S                (7<<8)
#define GPADC_EOC                   (1<<16)
#define GPADC_BUSY                  (1<<20)
#define GPADC_MANUAL_CH(n)          (((n)&0x3F)<<24)
#define GPADC_MANUAL_START          (1<<31)
#define GPADC_CH_EN(n)              (((n)&15)<<0)
#define GPADC_CH_EN_MASK            (15<<0)
#define GPADC_CH_EN_SHIFT           (0)

//gpadc_ch0
#define GPADC_DATA(n)               (((n)&0x3FF)<<0)

//gpadc_ch1
//#define GPADC_DATA(n)             (((n)&0x3FF)<<0)

//gpadc_ch2
//#define GPADC_DATA(n)             (((n)&0x3FF)<<0)

//gpadc_ch3
//#define GPADC_DATA(n)             (((n)&0x3FF)<<0)

//gpadc_data
//#define GPADC_DATA(n)             (((n)&0x3FF)<<0)





#endif
