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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/lcdc.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _LCDC_H_
#define _LCDC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'lcdc'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// LCDC_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_LCDC_BASE               0x00B98000


typedef volatile struct
{
    REG32                          apbi_ctrl_lcd;                //0x00000000
    REG32 Reserved_00000004;                    //0x00000004
    REG32                          APBI_FIFO;                    //0x00000008
    REG32 Reserved_0000000C[253];               //0x0000000C
    REG32                          LCD_CTRL;                     //0x00000400
    REG32                          LCD_STATUS;                   //0x00000404
    REG32                          LCD_DATA;                     //0x00000408
    REG32                          LCD_CLEAR;                    //0x0000040C
    REG32                          LCD_CFG;                      //0x00000410
    REG32                          LCD_TIMING;                   //0x00000414
    REG32 Reserved_00000418[250];               //0x00000418
    REG32                          SLCD_CTRL;                    //0x00000800
    REG32                          SLCD_STATUS;                  //0x00000804
    REG32                          SLCD_CFG;                     //0x00000808
    REG32                          SLCD_INT;                     //0x0000080C
    REG32                          SLCD_RW;                      //0x00000810
    REG32                          SLCD_CDOUT;                   //0x00000814
    REG32                          SLCD_DIN;                     //0x00000818
    REG32                          SLCD_DIVCLK;                  //0x0000081C
} HWP_LCDC_T;

#define hwp_lcdc                    ((HWP_LCDC_T*) KSEG1(REG_LCDC_BASE))


//apbi_ctrl_lcd
#define LCDC_L_ENDIAN(n)            (((n)&3)<<0)
#define LCDC_SOFT_RST_L             (1<<2)

//APBI_FIFO
#define LCDC_DATA_IN(n)             (((n)&0xFFFFFFFF)<<0)

//LCD_CTRL
#define LCDC_LCD_ENABLE             (1<<0)
#define LCDC_LCD_CTRL_RW            (1<<4)
#define LCDC_LCD_CTRL_RS            (1<<5)
#define LCDC_LCD_CTRL_AF            (1<<8)
#define LCDC_LCD_CTRL_CNT(n)        (((n)&0xFFFFF)<<12)

//LCD_STATUS
#define LCDC_LCD_RDY                (1<<0)
#define LCDC_LCD_INT_STATUS         (1<<16)
#define LCDC_LCD_INT_CAUSE          (1<<20)

//LCD_DATA
#define LCDC_LCD_DATA(n)            (((n)&0xFFFFFFFF)<<0)

//LCD_CLEAR
#define LCDC_LCD_INT_CLR            (1<<16)

//LCD_CFG
#define LCDC_LCD_CFG_DBS(n)         (((n)&7)<<0)
#define LCDC_LCD_CFG_CS(n)          (((n)&7)<<4)
#define LCDC_LCD_CFG_PIS            (1<<8)
#define LCDC_LCD_CS_POL(n)          (((n)&7)<<12)
#define LCDC_LCD_INT_MASK           (1<<16)

//LCD_TIMING
#define LCDC_LCD_TIMING_TAS(n)      (((n)&3)<<0)
#define LCDC_LCD_TIMING_TCH(n)      (((n)&0x3F)<<8)
#define LCDC_LCD_TIMING_TCL(n)      (((n)&0x3F)<<16)

//SLCD_CTRL
#define LCDC_ENABLE                 (1<<0)
#define LCDC_SCL_ENABLE             (1<<4)
#define LCDC_APB_FF                 (1<<8)
#define LCDC_CD                     (1<<12)
#define LCDC_DMA_CNT(n)             (((n)&0xFFFF)<<16)

//SLCD_STATUS
#define LCDC_READY                  (1<<0)
#define LCDC_SLCD_INT_STATUS        (1<<8)
#define LCDC_SLCD_INT_CAUSE         (1<<12)

//SLCD_CFG
#define LCDC_LINE                   (1<<0)
#define LCDC_CE                     (1<<4)
#define LCDC_SLCD_INT_MASK          (1<<8)

//SLCD_INT
#define LCDC_INT_CLEAR              (1<<8)

//SLCD_RW
#define LCDC_RW                     (1<<0)
#define LCDC_READ_BYTE(n)           (((n)&7)<<4)

//SLCD_CDOUT
#define LCDC_CMD_DATA(n)            (((n)&0xFFFFFFFF)<<0)

//SLCD_DIN
#define LCDC_DIN(n)                 (((n)&0xFFFFFFFF)<<0)

//SLCD_DIVCLK
#define LCDC_DIV_CLK(n)             (((n)&0xFFFF)<<0)





#endif
