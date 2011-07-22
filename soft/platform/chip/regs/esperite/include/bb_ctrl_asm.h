#ifndef _BB_CTRL_ASM_H_
#define _BB_CTRL_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'bb_ctrl'."
#endif

#include "globals_asm.h"

/// Baseband side System clocks
#define BB_BCPU                                  (0)
#define BB_BCPU_INT                              (1)
#define BB_AMBA                                  (2)
#define BB_PCLK_CONF                             (3)
#define BB_PCLK_DATA                             (4)
#define BB_VOC_AHB                               (5)
#define BB_IFC_CH0                               (6)
#define BB_IFC_CH1                               (7)
#define BB_IFC_CH2                               (8)
#define BB_IFC_CH3                               (9)
#define BB_SRAM                                  (10)
#define BB_A2A                                   (11)
#define BB_ITLV                                  (12)
#define BB_CAP                                   (13)
#define BB_VITERBI                               (14)
#define BB_CIPHER                                (15)
#define BB_RF_IF                                 (16)
#define BB_XCOR                                  (17)
#define BB_AIF                                   (18)
#define BB_DMASC                                 (19)
#define BB_MEM                                   (20)
/// Baseband side divided clock
/// divided BCK clock for DAI
#define BBD_DAIS_208K                            (21)
#define NB_BB_CLK_AEN                            (22)
/// the following don't have an auto enable
#define BB_IRQ                                   (22)
#define BB_SPY                                   (23)
#define BB_TEST                                  (24)
#define NB_BB_CLK_EN                             (25)
/// keep last
#define BB_NOGATE                                (25)
#define NB_BB_CLK                                (26)
#define RST_BCPU                                 (0)
#define RST_BB_IRQ                               (1)
#define RST_BB_IFC                               (2)
#define RST_BB_SRAM                              (3)
#define RST_ITLV                                 (4)
#define RST_CAP                                  (5)
#define RST_VITERBI                              (6)
#define RST_CIPHER                               (7)
#define RST_XCOR                                 (8)
#define RST_RF_IF                                (9)
#define RST_VOC                                  (10)
#define RST_AIF                                  (11)
#define RST_DMASC                                (12)
#define RST_COM_REGS                             (13)
#define RST_DAI                                  (14)
#define RST_BB_SPY                               (15)
#define RST_BB_TEST                              (16)
#define NB_RST_BB_REG                            (17)
#define RST_BB_FULL                              (17)
#define RST_BB_A2A                               (18)
#define RST_BB_MEM_BRIDGE                        (19)
#define NB_RST_BB                                (20)
#define BB_CTRL_PROTECT_LOCK                     (0XA50000)
#define BB_CTRL_PROTECT_UNLOCK                   (0XA50001)

//==============================================================================
// BB_Clks
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// Resets_BB
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// bb_ctrl
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_BB_CTRL_BASE            0x01900000

#define REG_BB_CTRL_BASE_HI         BASE_HI(REG_BB_CTRL_BASE)
#define REG_BB_CTRL_BASE_LO         BASE_LO(REG_BB_CTRL_BASE)

#define REG_BB_CTRL_REG_DBG         REG_BB_CTRL_BASE_LO + 0x00000000
#define REG_BB_CTRL_BB_RST_SET      REG_BB_CTRL_BASE_LO + 0x00000004
#define REG_BB_CTRL_BB_RST_CLR      REG_BB_CTRL_BASE_LO + 0x00000008
#define REG_BB_CTRL_CLK_BB_MODE     REG_BB_CTRL_BASE_LO + 0x0000000C
#define REG_BB_CTRL_CLK_BB_ENABLE   REG_BB_CTRL_BASE_LO + 0x00000010
#define REG_BB_CTRL_CLK_BB_DISABLE  REG_BB_CTRL_BASE_LO + 0x00000014
#define REG_BB_CTRL_BCPU_DBG_BKP    REG_BB_CTRL_BASE_LO + 0x00000018
#define REG_BB_CTRL_BCPU_DBG_ADDR   REG_BB_CTRL_BASE_LO + 0x0000001C
#define REG_BB_CTRL_CFG_BCPU_CACHE_RAM_DISABLE REG_BB_CTRL_BASE_LO + 0x00000020

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
