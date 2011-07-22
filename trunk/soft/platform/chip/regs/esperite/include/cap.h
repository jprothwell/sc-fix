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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/esperite/include/cap.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _CAP_H_
#define _CAP_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'cap'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CAP_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_CAP_BASE                0x01905000

typedef volatile struct
{
    REG32                          Ctrl;                         //0x00000000
    REG32                          Operation;                    //0x00000004
    REG32                          Status;                       //0x00000008
    REG32                          Src_Data_addr;                //0x0000000C
    REG32                          Dst_Data_addr;                //0x00000010
    REG32                          Punct_addr;                   //0x00000014
    REG32                          CRC_code_LSB;                 //0x00000018
    REG32                          CRC_code_MSB;                 //0x0000001C
} HWP_CAP_T;

#define hwp_cap                     ((HWP_CAP_T*) KSEG1(REG_CAP_BASE))


//Ctrl
#define CAP_FIRST_POLY(n)           (((n)&15)<<0)
#define CAP_SECOND_POLY(n)          (((n)&15)<<4)
#define CAP_THIRD_POLY(n)           (((n)&7)<<8)
#define CAP_FOURTH_POLY(n)          (((n)&7)<<11)
#define CAP_FITH_POLY(n)            (((n)&7)<<14)
#define CAP_SIXTH_POLY(n)           (((n)&7)<<17)
#define CAP_RSC_POLY(n)             (((n)&7)<<20)
#define CAP_NB_POLY(n)              (((n)&7)<<23)
#define CAP_ENABLE_PUNCTURING       (1<<26)
#define CAP_ENABLE_CRC_PRELOADING   (1<<27)

//Operation
#define CAP_START_CRC_ENCODING      (1<<0)
#define CAP_START_CRC_DECODING      (1<<1)
#define CAP_START_CONV              (1<<2)
#define CAP_START_TRAPPING          (1<<3)
#define CAP_BIT_NUMBER(n)           (((n)&0x3FF)<<16)

//Status
#define CAP_ENABLE                  (1<<0)

//Src_Data_addr
#define CAP_SRC_DATA_ADDR(n)        (((n)&0x1FFF)<<2)

//Dst_Data_addr
#define CAP_DST_DATA_ADDR(n)        (((n)&0x1FFF)<<2)

//Punct_addr
#define CAP_PUNCT_ADDR(n)           (((n)&0x1FFF)<<2)

//CRC_code_LSB
#define CAP_CRC_CODE_LSB(n)         (((n)&0xFFFFFFFF)<<0)

//CRC_code_MSB
#define CAP_CRC_CODE_MSB(n)         (((n)&0xFF)<<0)





#endif

