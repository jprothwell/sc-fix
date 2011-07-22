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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/esperite/include/bb_ctrl.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _BB_CTRL_H_
#define _BB_CTRL_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'bb_ctrl'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================

// ============================================================================
// BB_CLKS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
/// Baseband side System clocks
    BB_BCPU                                     = 0x00000000,
    BB_BCPU_INT                                 = 0x00000001,
    BB_AMBA                                     = 0x00000002,
    BB_PCLK_CONF                                = 0x00000003,
    BB_PCLK_DATA                                = 0x00000004,
    BB_VOC_AHB                                  = 0x00000005,
    BB_IFC_CH0                                  = 0x00000006,
    BB_IFC_CH1                                  = 0x00000007,
    BB_IFC_CH2                                  = 0x00000008,
    BB_IFC_CH3                                  = 0x00000009,
    BB_SRAM                                     = 0x0000000A,
    BB_A2A                                      = 0x0000000B,
    BB_ITLV                                     = 0x0000000C,
    BB_CAP                                      = 0x0000000D,
    BB_VITERBI                                  = 0x0000000E,
    BB_CIPHER                                   = 0x0000000F,
    BB_RF_IF                                    = 0x00000010,
    BB_XCOR                                     = 0x00000011,
    BB_AIF                                      = 0x00000012,
    BB_DMASC                                    = 0x00000013,
    BB_MEM                                      = 0x00000014,
/// Baseband side divided clock
/// divided BCK clock for DAI
    BBD_DAIS_208K                               = 0x00000015,
/// the following don't have an auto enable
    BB_IRQ                                      = 0x00000016,
    BB_SPY                                      = 0x00000017,
    BB_TEST                                     = 0x00000018,
/// keep last
    BB_NOGATE                                   = 0x00000019
} BB_CLKS_T;

#define NB_BB_CLK_AEN                            (22)
#define NB_BB_CLK_EN                             (25)
#define NB_BB_CLK                                (26)

// ============================================================================
// RESETS_BB_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    RST_BCPU                                    = 0x00000000,
    RST_BB_IRQ                                  = 0x00000001,
    RST_BB_IFC                                  = 0x00000002,
    RST_BB_SRAM                                 = 0x00000003,
    RST_ITLV                                    = 0x00000004,
    RST_CAP                                     = 0x00000005,
    RST_VITERBI                                 = 0x00000006,
    RST_CIPHER                                  = 0x00000007,
    RST_XCOR                                    = 0x00000008,
    RST_RF_IF                                   = 0x00000009,
    RST_VOC                                     = 0x0000000A,
    RST_AIF                                     = 0x0000000B,
    RST_DMASC                                   = 0x0000000C,
    RST_COM_REGS                                = 0x0000000D,
    RST_DAI                                     = 0x0000000E,
    RST_BB_SPY                                  = 0x0000000F,
    RST_BB_TEST                                 = 0x00000010,
    RST_BB_FULL                                 = 0x00000011,
    RST_BB_A2A                                  = 0x00000012,
    RST_BB_MEM_BRIDGE                           = 0x00000013
} RESETS_BB_T;

#define NB_RST_BB_REG                            (17)
#define NB_RST_BB                                (20)
#define BB_CTRL_PROTECT_LOCK                     (0XA50000)
#define BB_CTRL_PROTECT_UNLOCK                   (0XA50001)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// BB_CTRL_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_BB_CTRL_BASE            0x01900000

typedef volatile struct
{
    /// <strong>This register is used to Lock and Unlock the protected registers.</strong>
    REG32                          REG_DBG;                      //0x00000000
    /// This register is protected.
    REG32                          BB_Rst_Set;                   //0x00000004
    REG32                          BB_Rst_Clr;                   //0x00000008
    REG32                          Clk_BB_Mode;                  //0x0000000C
    REG32                          Clk_BB_Enable;                //0x00000010
    /// This register is protected.
    REG32                          Clk_BB_Disable;               //0x00000014
    REG32                          BCpu_Dbg_BKP;                 //0x00000018
    REG32                          BCpu_Dbg_Addr;                //0x0000001C
    REG32                          Cfg_Bcpu_Cache_Ram_Disable;   //0x00000020
} HWP_BB_CTRL_T;

#define hwp_bbCtrl                  ((HWP_BB_CTRL_T*) KSEG1(REG_BB_CTRL_BASE))


//REG_DBG
#define BB_CTRL_WRITE_UNLOCK_STATUS (1<<30)
#define BB_CTRL_WRITE_UNLOCK        (1<<31)

//BB_Rst_Set
#define BB_CTRL_SET_RST_BCPU        (1<<0)
#define BB_CTRL_SET_RST_BB_IRQ      (1<<1)
#define BB_CTRL_SET_RST_BB_IFC      (1<<2)
#define BB_CTRL_SET_RST_BB_SRAM     (1<<3)
#define BB_CTRL_SET_RST_ITLV        (1<<4)
#define BB_CTRL_SET_RST_CAP         (1<<5)
#define BB_CTRL_SET_RST_VITERBI     (1<<6)
#define BB_CTRL_SET_RST_CIPHER      (1<<7)
#define BB_CTRL_SET_RST_XCOR        (1<<8)
#define BB_CTRL_SET_RST_RF_IF       (1<<9)
#define BB_CTRL_SET_RST_VOC         (1<<10)
#define BB_CTRL_SET_RST_AIF         (1<<11)
#define BB_CTRL_SET_RST_DMASC       (1<<12)
#define BB_CTRL_SET_RST_COM_REGS    (1<<13)
#define BB_CTRL_SET_RST_DAI         (1<<14)
#define BB_CTRL_SET_RST_BB_SPY      (1<<15)
#define BB_CTRL_SET_RST_BB_TEST     (1<<16)
#define BB_CTRL_SET_RST_BB_FULL     (1<<31)
#define BB_CTRL_SET_BB_RST(n)       (((n)&0x1FFFF)<<0)
#define BB_CTRL_SET_BB_RST_MASK     (0x1FFFF<<0)
#define BB_CTRL_SET_BB_RST_SHIFT    (0)

//BB_Rst_Clr
#define BB_CTRL_CLR_RST_BCPU        (1<<0)
#define BB_CTRL_CLR_RST_BB_IRQ      (1<<1)
#define BB_CTRL_CLR_RST_BB_IFC      (1<<2)
#define BB_CTRL_CLR_RST_BB_SRAM     (1<<3)
#define BB_CTRL_CLR_RST_ITLV        (1<<4)
#define BB_CTRL_CLR_RST_CAP         (1<<5)
#define BB_CTRL_CLR_RST_VITERBI     (1<<6)
#define BB_CTRL_CLR_RST_CIPHER      (1<<7)
#define BB_CTRL_CLR_RST_XCOR        (1<<8)
#define BB_CTRL_CLR_RST_RF_IF       (1<<9)
#define BB_CTRL_CLR_RST_VOC         (1<<10)
#define BB_CTRL_CLR_RST_AIF         (1<<11)
#define BB_CTRL_CLR_RST_DMASC       (1<<12)
#define BB_CTRL_CLR_RST_COM_REGS    (1<<13)
#define BB_CTRL_CLR_RST_DAI         (1<<14)
#define BB_CTRL_CLR_RST_BB_SPY      (1<<15)
#define BB_CTRL_CLR_RST_BB_TEST     (1<<16)
#define BB_CTRL_CLR_RST_BB_FULL     (1<<31)
#define BB_CTRL_CLR_BB_RST(n)       (((n)&0x1FFFF)<<0)
#define BB_CTRL_CLR_BB_RST_MASK     (0x1FFFF<<0)
#define BB_CTRL_CLR_BB_RST_SHIFT    (0)

//Clk_BB_Mode
#define BB_CTRL_MODE_BB_BCPU        (1<<0)
#define BB_CTRL_MODE_BB_BCPU_INT_AUTOMATIC (0<<1)
#define BB_CTRL_MODE_BB_BCPU_INT_MANUAL (1<<1)
#define BB_CTRL_MODE_BB_AMBA_AUTOMATIC (0<<2)
#define BB_CTRL_MODE_BB_AMBA_MANUAL (1<<2)
#define BB_CTRL_MODE_BB_PCLK_CONF_AUTOMATIC (0<<3)
#define BB_CTRL_MODE_BB_PCLK_CONF_MANUAL (1<<3)
#define BB_CTRL_MODE_BB_PCLK_DATA_AUTOMATIC (0<<4)
#define BB_CTRL_MODE_BB_PCLK_DATA_MANUAL (1<<4)
#define BB_CTRL_MODE_BB_VOC_AHB_AUTOMATIC (0<<5)
#define BB_CTRL_MODE_BB_VOC_AHB_MANUAL (1<<5)
#define BB_CTRL_MODE_BB_IFC_CH0_AUTOMATIC (0<<6)
#define BB_CTRL_MODE_BB_IFC_CH0_MANUAL (1<<6)
#define BB_CTRL_MODE_BB_IFC_CH1_AUTOMATIC (0<<7)
#define BB_CTRL_MODE_BB_IFC_CH1_MANUAL (1<<7)
#define BB_CTRL_MODE_BB_IFC_CH2_AUTOMATIC (0<<8)
#define BB_CTRL_MODE_BB_IFC_CH2_MANUAL (1<<8)
#define BB_CTRL_MODE_BB_IFC_CH3_AUTOMATIC (0<<9)
#define BB_CTRL_MODE_BB_IFC_CH3_MANUAL (1<<9)
#define BB_CTRL_MODE_BB_SRAM_AUTOMATIC (0<<10)
#define BB_CTRL_MODE_BB_SRAM_MANUAL (1<<10)
#define BB_CTRL_MODE_BB_A2A_AUTOMATIC (0<<11)
#define BB_CTRL_MODE_BB_A2A_MANUAL  (1<<11)
#define BB_CTRL_MODE_BB_ITLV_AUTOMATIC (0<<12)
#define BB_CTRL_MODE_BB_ITLV_MANUAL (1<<12)
#define BB_CTRL_MODE_BB_CAP_AUTOMATIC (0<<13)
#define BB_CTRL_MODE_BB_CAP_MANUAL  (1<<13)
#define BB_CTRL_MODE_BB_VITERBI_AUTOMATIC (0<<14)
#define BB_CTRL_MODE_BB_VITERBI_MANUAL (1<<14)
#define BB_CTRL_MODE_BB_CIPHER_AUTOMATIC (0<<15)
#define BB_CTRL_MODE_BB_CIPHER_MANUAL (1<<15)
#define BB_CTRL_MODE_BB_RF_IF_AUTOMATIC (0<<16)
#define BB_CTRL_MODE_BB_RF_IF_MANUAL (1<<16)
#define BB_CTRL_MODE_BB_XCOR_AUTOMATIC (0<<17)
#define BB_CTRL_MODE_BB_XCOR_MANUAL (1<<17)
#define BB_CTRL_MODE_BB_AIF_AUTOMATIC (0<<18)
#define BB_CTRL_MODE_BB_AIF_MANUAL  (1<<18)
#define BB_CTRL_MODE_BB_DMASC_AUTOMATIC (0<<19)
#define BB_CTRL_MODE_BB_DMASC_MANUAL (1<<19)
#define BB_CTRL_MODE_BB_MEM_AUTOMATIC (0<<20)
#define BB_CTRL_MODE_BB_MEM_MANUAL  (1<<20)
#define BB_CTRL_MODE_BBD_DAIS_208K_AUTOMATIC (0<<21)
#define BB_CTRL_MODE_BBD_DAIS_208K_MANUAL (1<<21)
#define BB_CTRL_MODE_CLK_BB(n)      (((n)&0x1FFFFF)<<1)
#define BB_CTRL_MODE_CLK_BB_MASK    (0x1FFFFF<<1)
#define BB_CTRL_MODE_CLK_BB_SHIFT   (1)

//Clk_BB_Enable
#define BB_CTRL_ENABLE_BB_BCPU      (1<<0)
#define BB_CTRL_ENABLE_BB_BCPU_INT  (1<<1)
#define BB_CTRL_ENABLE_BB_AMBA      (1<<2)
#define BB_CTRL_ENABLE_BB_PCLK_CONF (1<<3)
#define BB_CTRL_ENABLE_BB_PCLK_DATA (1<<4)
#define BB_CTRL_ENABLE_BB_VOC_AHB   (1<<5)
#define BB_CTRL_ENABLE_BB_IFC_CH0   (1<<6)
#define BB_CTRL_ENABLE_BB_IFC_CH1   (1<<7)
#define BB_CTRL_ENABLE_BB_IFC_CH2   (1<<8)
#define BB_CTRL_ENABLE_BB_IFC_CH3   (1<<9)
#define BB_CTRL_ENABLE_BB_SRAM      (1<<10)
#define BB_CTRL_ENABLE_BB_A2A       (1<<11)
#define BB_CTRL_ENABLE_BB_ITLV      (1<<12)
#define BB_CTRL_ENABLE_BB_CAP       (1<<13)
#define BB_CTRL_ENABLE_BB_VITERBI   (1<<14)
#define BB_CTRL_ENABLE_BB_CIPHER    (1<<15)
#define BB_CTRL_ENABLE_BB_RF_IF     (1<<16)
#define BB_CTRL_ENABLE_BB_XCOR      (1<<17)
#define BB_CTRL_ENABLE_BB_AIF       (1<<18)
#define BB_CTRL_ENABLE_BB_DMASC     (1<<19)
#define BB_CTRL_ENABLE_BB_MEM       (1<<20)
#define BB_CTRL_ENABLE_BBD_DAIS_208K (1<<21)
#define BB_CTRL_ENABLE_BB_IRQ       (1<<22)
#define BB_CTRL_ENABLE_BB_SPY       (1<<23)
#define BB_CTRL_ENABLE_BB_TEST      (1<<24)
#define BB_CTRL_ENABLE_CLK_BB(n)    (((n)&0x1FFFFFF)<<0)
#define BB_CTRL_ENABLE_CLK_BB_MASK  (0x1FFFFFF<<0)
#define BB_CTRL_ENABLE_CLK_BB_SHIFT (0)

//Clk_BB_Disable
#define BB_CTRL_DISABLE_BB_BCPU     (1<<0)
#define BB_CTRL_DISABLE_BB_BCPU_INT (1<<1)
#define BB_CTRL_DISABLE_BB_AMBA     (1<<2)
#define BB_CTRL_DISABLE_BB_PCLK_CONF (1<<3)
#define BB_CTRL_DISABLE_BB_PCLK_DATA (1<<4)
#define BB_CTRL_DISABLE_BB_VOC_AHB  (1<<5)
#define BB_CTRL_DISABLE_BB_IFC_CH0  (1<<6)
#define BB_CTRL_DISABLE_BB_IFC_CH1  (1<<7)
#define BB_CTRL_DISABLE_BB_IFC_CH2  (1<<8)
#define BB_CTRL_DISABLE_BB_IFC_CH3  (1<<9)
#define BB_CTRL_DISABLE_BB_SRAM     (1<<10)
#define BB_CTRL_DISABLE_BB_A2A      (1<<11)
#define BB_CTRL_DISABLE_BB_ITLV     (1<<12)
#define BB_CTRL_DISABLE_BB_CAP      (1<<13)
#define BB_CTRL_DISABLE_BB_VITERBI  (1<<14)
#define BB_CTRL_DISABLE_BB_CIPHER   (1<<15)
#define BB_CTRL_DISABLE_BB_RF_IF    (1<<16)
#define BB_CTRL_DISABLE_BB_XCOR     (1<<17)
#define BB_CTRL_DISABLE_BB_AIF      (1<<18)
#define BB_CTRL_DISABLE_BB_DMASC    (1<<19)
#define BB_CTRL_DISABLE_BB_MEM      (1<<20)
#define BB_CTRL_DISABLE_BBD_DAIS_208K (1<<21)
#define BB_CTRL_DISABLE_BB_IRQ      (1<<22)
#define BB_CTRL_DISABLE_BB_SPY      (1<<23)
#define BB_CTRL_DISABLE_BB_TEST     (1<<24)
#define BB_CTRL_DISABLE_CLK_BB(n)   (((n)&0x1FFFFFF)<<0)
#define BB_CTRL_DISABLE_CLK_BB_MASK (0x1FFFFFF<<0)
#define BB_CTRL_DISABLE_CLK_BB_SHIFT (0)

//BCpu_Dbg_BKP
#define BB_CTRL_BKPT_EN             (1<<0)
#define BB_CTRL_BKPT_MODE(n)        (((n)&3)<<4)
#define BB_CTRL_BKPT_MODE_I         (0<<4)
#define BB_CTRL_BKPT_MODE_R         (1<<4)
#define BB_CTRL_BKPT_MODE_W         (2<<4)
#define BB_CTRL_BKPT_MODE_RW        (3<<4)
#define BB_CTRL_STALLED             (1<<8)

//BCpu_Dbg_Addr
#define BB_CTRL_BREAKPOINT_ADDRESS(n) (((n)&0x3FFFFFF)<<0)

//Cfg_Bcpu_Cache_Ram_Disable
#define BB_CTRL_BCPU_USE_MODE       (1<<0)
#define BB_CTRL_BCPU_CLK_OFF_MODE   (1<<1)
#define BB_CTRL_BCPU_CACHE_RAM_DISABLE(n) (((n)&3)<<0)
#define BB_CTRL_BCPU_CACHE_RAM_DISABLE_MASK (3<<0)
#define BB_CTRL_BCPU_CACHE_RAM_DISABLE_SHIFT (0)





#endif

