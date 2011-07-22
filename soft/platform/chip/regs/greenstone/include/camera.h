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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/camera.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'camera'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// CAMERA_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_CAMERA_BASE             0x00B99000


typedef volatile struct
{
    REG32                          apbi_ctrl_camera;             //0x00000000
    REG32 Reserved_00000004;                    //0x00000004
    REG32                          APBI_FIFO_Rx;                 //0x00000008
    REG32 Reserved_0000000C[509];               //0x0000000C
    REG32                          Interrupt_CLEAR_register;     //0x00000800
    REG32                          Interrupt_STATUS_register;    //0x00000804
    REG32                          Interrupt_CAUSE_register;     //0x00000808
    REG32                          Interrupt_MASK_register;      //0x0000080C
    REG32                          Camera_CONTROL_register;      //0x00000810
} HWP_CAMERA_T;

#define hwp_camera                  ((HWP_CAMERA_T*) KSEG1(REG_CAMERA_BASE))


//apbi_ctrl_camera
#define CAMERA_L_ENDIAN(n)          (((n)&3)<<0)
#define CAMERA_SOFT_RST_L           (1<<2)

//APBI_FIFO_Rx
#define CAMERA_DATA_OUT(n)          (((n)&0xFFFFFFFF)<<0)

//Interrupt_CLEAR_register
#define CAMERA_OVFL                 (1<<0)
#define CAMERA_FSTART               (1<<1)
#define CAMERA_FEND                 (1<<2)
#define CAMERA_VSYNC                (1<<3)

//Interrupt_STATUS_register
//#define CAMERA_OVFL               (1<<0)
//#define CAMERA_FSTART             (1<<1)
//#define CAMERA_FEND               (1<<2)
//#define CAMERA_VSYNC              (1<<3)

//Interrupt_CAUSE_register
//#define CAMERA_OVFL               (1<<0)
//#define CAMERA_FSTART             (1<<1)
//#define CAMERA_FEND               (1<<2)
//#define CAMERA_VSYNC              (1<<3)

//Interrupt_MASK_register
//#define CAMERA_OVFL               (1<<0)
//#define CAMERA_FSTART             (1<<1)
//#define CAMERA_FEND               (1<<2)
//#define CAMERA_VSYNC              (1<<3)

//Camera_CONTROL_register
#define CAMERA_MODE                 (1<<0)
#define CAMERA_PWDON                (1<<1)
#define CAMERA_RESET                (1<<2)
#define CAMERA_ENCAMERA             (1<<3)
#define CAMERA_TEST                 (1<<4)





#endif
