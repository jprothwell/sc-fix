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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/ana_acco.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _ANA_ACCO_H_
#define _ANA_ACCO_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'ana_acco'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

typedef enum {
    ANA_TEST_NONE                               = 0x00000000,
    ANA_TEST_TX_DAC                             = 0x00000001,
    ANA_TEST_RX_ADC                             = 0x00000002,
    ANA_TEST_AFCDAC                             = 0x00000003,
    ANA_TEST_PADAC                              = 0x00000004,
    ANA_TEST_AU_CODEC                           = 0x00000005,
    ANA_TEST_GPADC                              = 0x00000006
} ANA_TEST_MODES_T;


// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// ANA_ACCO_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_ANA_ACCO_FE_BASE        0x00BA3000


typedef volatile struct
{
    REG32                          audio_ctrl_set;               //0x00000000
    REG32                          audio_ctrl_clr;               //0x00000004
    REG32                          audio_sel;                    //0x00000008
    REG32                          audio_mic_gain;               //0x0000000C
    REG32                          audio_spk_gain;               //0x00000010
    REG32                          audio_calib;                  //0x00000014
    REG32                          rf_ctrl_set;                  //0x00000018
    REG32                          rf_ctrl_clr;                  //0x0000001C
    REG32                          rf_tx_calib;                  //0x00000020
    REG32                          rf_tx_gmsk_calib;             //0x00000024
    REG32                          rf_tx_data_dac;               //0x00000028
    REG32                          rf_rx_calib;                  //0x0000002C
    /// This register is protected. (see sys_ctrl)
    REG32                          pll_ctrl;                     //0x00000030
    REG32                          pa_afc_gp_ctrl_set;           //0x00000034
    REG32                          pa_afc_gp_ctrl_clr;           //0x00000038
    REG32                          pa_afc_gp_calib;              //0x0000003C
    REG32                          ana_clk_ctrl;                 //0x00000040
    REG32                          ana_clk_man_en_set;           //0x00000044
    REG32                          ana_clk_man_en_clr;           //0x00000048
    REG32                          ana_prod_test_mode;           //0x0000004C
    /// This register is used only for debug purpose or production, should not be
    /// used in functional mode.
    REG32                          ana_test_ctrl;                //0x00000050
} HWP_ANA_ACCO_T;

#define hwp_anaAccoFe               ((HWP_ANA_ACCO_T*) KSEG1(REG_ANA_ACCO_FE_BASE))


//audio_ctrl_set
#define ANA_ACCO_AU_RESET_RELEASE   (1<<0)
#define ANA_ACCO_AU_DEEP_PD_RELEASE (1<<1)
#define ANA_ACCO_AU_SD_ENABLE       (1<<2)
#define ANA_ACCO_AU_DACA_ENABLE     (1<<3)
#define ANA_ACCO_AU_MIC_ENABLE      (1<<4)
#define ANA_ACCO_AU_SPK_ENABLE      (1<<5)
#define ANA_ACCO_AU_MIC_FILT_BP_ENABLE (1<<6)
#define ANA_ACCO_AU_SPK_FILT_BP_ENABLE (1<<7)

//audio_ctrl_clr
#define ANA_ACCO_AU_RESET_N         (1<<0)
#define ANA_ACCO_AU_DEEP_PD_N       (1<<1)
#define ANA_ACCO_AU_SD_DISABLE      (1<<2)
#define ANA_ACCO_AU_DACA_DISABLE    (1<<3)
#define ANA_ACCO_AU_MIC_DISABLE     (1<<4)
#define ANA_ACCO_AU_SPK_DISABLE     (1<<5)
#define ANA_ACCO_AU_MIC_FILT_BP_DISABLE (1<<6)
#define ANA_ACCO_AU_SPK_FILT_BP_DISABLE (1<<7)

//audio_sel
#define ANA_ACCO_AU_MIC_SEL         (1<<0)
#define ANA_ACCO_AU_MIC_SEL_MASK    (1<<0)
#define ANA_ACCO_AU_MIC_SEL_SHIFT   (0)
#define ANA_ACCO_AU_SPK_SEL         (1<<4)
#define ANA_ACCO_AU_SPK_SEL_MASK    (1<<4)
#define ANA_ACCO_AU_SPK_SEL_SHIFT   (4)

//audio_mic_gain
#define ANA_ACCO_AU_MIC_ANA_GAIN(n) (((n)&15)<<0)
#define ANA_ACCO_AU_MIC_DIG_GAIN(n) (((n)&3)<<4)
#define ANA_ACCO_AU_MIC_MUTE        (1<<16)
#define ANA_ACCO_AU_MIC_MUTE_MUTE   (0<<16)
#define ANA_ACCO_AU_MIC_MUTE_UNMUTE (1<<16)

//audio_spk_gain
#define ANA_ACCO_AU_SPK_ANA_GAIN(n) (((n)&7)<<0)
#define ANA_ACCO_AU_SPK_DIG_GAIN(n) (((n)&3)<<4)
#define ANA_ACCO_AU_SPK_MUTE        (1<<16)
#define ANA_ACCO_AU_SPK_MUTE_MUTE   (0<<16)
#define ANA_ACCO_AU_SPK_MUTE_UNMUTE (1<<16)

//audio_calib
#define ANA_ACCO_AU_BG_ENABLE       (1<<0)
#define ANA_ACCO_AU_BG_STARTUP      (1<<1)
#define ANA_ACCO_AU_BG_OK           (1<<2)
#define ANA_ACCO_AU_BG_CTRL(n)      (((n)&0x3F)<<4)
#define ANA_ACCO_AU_REF_ENABLE      (1<<12)
#define ANA_ACCO_AU_REF_CAL(n)      (((n)&7)<<16)

//rf_ctrl_set
#define ANA_ACCO_RF_RESET_RELEASE   (1<<0)
#define ANA_ACCO_RF_DEEP_PD_RELEASE (1<<1)
#define ANA_ACCO_RX_AAF_BYPASS      (1<<4)
#define ANA_ACCO_TX_DAC_ENABLE      (1<<16)

//rf_ctrl_clr
#define ANA_ACCO_RF_RESET_N         (1<<0)
#define ANA_ACCO_RF_DEEP_PD_N       (1<<1)
#define ANA_ACCO_RX_AAF_USE         (1<<4)
#define ANA_ACCO_TX_DAC_DISABLE     (1<<16)

//rf_tx_calib
#define ANA_ACCO_TX_BG_ENABLE       (1<<0)
#define ANA_ACCO_TX_BG_STARTUP      (1<<1)
#define ANA_ACCO_TX_BG_OK           (1<<2)
#define ANA_ACCO_TX_BG_CTRL(n)      (((n)&0x3F)<<4)
#define ANA_ACCO_TX_DAC_CURRENT_CALI(n) (((n)&7)<<12)
#define ANA_ACCO_TX_DAC_CURRENT_CALQ(n) (((n)&7)<<16)
#define ANA_ACCO_TX_REF_VOLTAGE_CAL(n) (((n)&15)<<20)
#define ANA_ACCO_TX_FILTER_TUNING(n) (((n)&3)<<24)

//rf_tx_gmsk_calib
#define ANA_ACCO_TX_GMSK_REDUCE_I(n) (((n)&15)<<0)
#define ANA_ACCO_TX_GMSK_OFFSET_I(n) (((n)&15)<<4)
#define ANA_ACCO_TX_GMSK_REDUCE_Q(n) (((n)&15)<<16)
#define ANA_ACCO_TX_GMSK_OFFSET_Q(n) (((n)&15)<<20)

//rf_tx_data_dac
#define ANA_ACCO_RF_TX_DATA_DAC_I(n) (((n)&0x3FF)<<0)
#define ANA_ACCO_RF_TX_DATA_DAC_Q(n) (((n)&0x3FF)<<16)

//rf_rx_calib
#define ANA_ACCO_RX_BG_ENABLE       (1<<0)
#define ANA_ACCO_RX_BG_STARTUP      (1<<1)
#define ANA_ACCO_RX_BG_OK           (1<<2)
#define ANA_ACCO_RX_BG_CTRL(n)      (((n)&0x3F)<<4)
#define ANA_ACCO_RX_REF_CAL(n)      (((n)&7)<<12)
#define ANA_ACCO_RX_DIG_GAIN        (1<<16)

//pll_ctrl
#define ANA_ACCO_PLL_PD_N           (1<<0)
#define ANA_ACCO_PLL_PD_N_ENABLE    (1<<0)
#define ANA_ACCO_PLL_PD_N_DISABLE   (0<<0)
#define ANA_ACCO_PLL_LOCKED         (1<<1)
#define ANA_ACCO_PLL_BYPASS         (1<<2)
#define ANA_ACCO_PLL_DIV2_DISABLE   (1<<3)
#define ANA_ACCO_PLL_CP_TUNE(n)     (((n)&3)<<4)
#define ANA_ACCO_PLL_CP_TUNE_0_PERCENT (0<<4)
#define ANA_ACCO_PLL_CP_TUNE_75_PERCENT (1<<4)
#define ANA_ACCO_PLL_CP_TUNE_100_PERCENT (2<<4)
#define ANA_ACCO_PLL_CP_TUNE_125_PERCENT (3<<4)
#define ANA_ACCO_PLL_RST_LOCK       (1<<6)
#define ANA_ACCO_PLL_IN_SOURCE      (1<<8)
#define ANA_ACCO_PLL_IN_SOURCE_CLOCKSQUARER (0<<8)
#define ANA_ACCO_PLL_IN_SOURCE_EXT  (1<<8)
#define ANA_ACCO_PLL_SYS_EON        (1<<9)
#define ANA_ACCO_PLL_SYS_EON_ENABLE (0<<9)
#define ANA_ACCO_PLL_SYS_EON_NO_CLOCK (1<<9)
#define ANA_ACCO_PLL_EXT_EON        (1<<10)
#define ANA_ACCO_PLL_EXT_EON_ENABLE (0<<10)
#define ANA_ACCO_PLL_EXT_EON_NO_CLOCK (1<<10)

//pa_afc_gp_ctrl_set
#define ANA_ACCO_PADAC_RESET_RELEASE (1<<0)
#define ANA_ACCO_AFC_RESET_RELEASE  (1<<1)
#define ANA_ACCO_GPADC_RESET_RELEASE (1<<2)
#define ANA_ACCO_PADAC_ENABLE       (1<<16)
#define ANA_ACCO_AFC_ENABLE         (1<<17)
#define ANA_ACCO_GPADC_ENABLE       (1<<18)

//pa_afc_gp_ctrl_clr
#define ANA_ACCO_PADAC_RESET_N      (1<<0)
#define ANA_ACCO_AFC_RESET_N        (1<<1)
#define ANA_ACCO_GPADC_RESET_N      (1<<2)
#define ANA_ACCO_PADAC_DISABLE      (1<<16)
#define ANA_ACCO_AFC_DISABLE        (1<<17)
#define ANA_ACCO_GPADC_DISABLE      (1<<18)

//pa_afc_gp_calib
#define ANA_ACCO_COMM_BG_ENABLE     (1<<0)
#define ANA_ACCO_COMM_BG_STARTUP    (1<<1)
#define ANA_ACCO_COMM_BG_OK         (1<<2)
#define ANA_ACCO_COMM_BG_CTRL(n)    (((n)&0x3F)<<4)

//ana_clk_ctrl
#define ANA_ACCO_RF_RX_CLK_6M5_POL  (1<<0)
#define ANA_ACCO_RF_RX_CLK_6M5_EN_MODE (1<<1)
#define ANA_ACCO_RF_TX_CLK_4M33_POL (1<<2)
#define ANA_ACCO_RF_TX_CLK_4M33_EN_MODE (1<<3)
#define ANA_ACCO_AU_CLK_520K_POL    (1<<4)
#define ANA_ACCO_AU_CLK_40K_POL     (1<<6)
#define ANA_ACCO_AU_CLK_520K_40K_EN_MODE (1<<7)
#define ANA_ACCO_GPADC_CLK_DIV(n)   (((n)&31)<<8)
#define ANA_ACCO_GPADC_CLK_DIV_MASK (31<<8)
#define ANA_ACCO_GPADC_CLK_DIV_SHIFT (8)
#define ANA_ACCO_GPADC_CLK_POL      (1<<14)
#define ANA_ACCO_GPADC_CLK_EN_MODE  (1<<15)
#define ANA_ACCO_AFC_CLK_DIV(n)     (((n)&0xFF)<<16)
#define ANA_ACCO_AFC_CLK_DIV_MASK   (0xFF<<16)
#define ANA_ACCO_AFC_CLK_DIV_SHIFT  (16)
#define ANA_ACCO_AFC_CLK_POL        (1<<24)
#define ANA_ACCO_AFC_CLK_EN_MODE    (1<<25)
#define ANA_ACCO_PA_CLK_1M08_POL    (1<<28)
#define ANA_ACCO_PA_CLK_1M08_EN_MODE (1<<29)

//ana_clk_man_en_set
#define ANA_ACCO_RF_RX_CLK_6M5_ENABLE (1<<1)
#define ANA_ACCO_RF_TX_CLK_4M33_ENABLE (1<<3)
#define ANA_ACCO_AU_CLK_520K_40K_ENABLE (1<<7)
#define ANA_ACCO_GPADC_CLK_ENABLE   (1<<15)
#define ANA_ACCO_AFC_CLK_ENABLE     (1<<25)
#define ANA_ACCO_PA_CLK_1M08_ENABLE (1<<29)
#define ANA_ACCO_SOFT_RESET_RELEASE (1<<31)

//ana_clk_man_en_clr
#define ANA_ACCO_RF_RX_CLK_6M5_DISABLE (1<<1)
#define ANA_ACCO_RF_TX_CLK_4M33_DISABLE (1<<3)
#define ANA_ACCO_AU_CLK_520K_40K_DISABLE (1<<7)
#define ANA_ACCO_GPADC_CLK_DISABLE  (1<<15)
#define ANA_ACCO_AFC_CLK_DISABLE    (1<<25)
#define ANA_ACCO_PA_CLK_1M08_DISABLE (1<<29)
#define ANA_ACCO_SOFT_RESET_N       (1<<31)

//ana_prod_test_mode
#define ANA_ACCO_TEST_MODE_SEL(n)   (((n)&7)<<0)
#define ANA_ACCO_TEST_MODE_SEL_MASK (7<<0)
#define ANA_ACCO_TEST_MODE_SEL_SHIFT (0)
#define ANA_ACCO_TEST_MODE_SEL_ANA_TEST_NONE (0<<0)
#define ANA_ACCO_TEST_MODE_SEL_ANA_TEST_TX_DAC (1<<0)
#define ANA_ACCO_TEST_MODE_SEL_ANA_TEST_RX_ADC (2<<0)
#define ANA_ACCO_TEST_MODE_SEL_ANA_TEST_AFCDAC (3<<0)
#define ANA_ACCO_TEST_MODE_SEL_ANA_TEST_PADAC (4<<0)
#define ANA_ACCO_TEST_MODE_SEL_ANA_TEST_AU_CODEC (5<<0)
#define ANA_ACCO_TEST_MODE_SEL_ANA_TEST_GPADC (6<<0)
#define ANA_ACCO_GPADC_SEL_TEST(n)  (((n)&0x3F)<<8)
#define ANA_ACCO_GPADC_EOC          (1<<15)
#define ANA_ACCO_TX_ON_TEST         (1<<16)
#define ANA_ACCO_TX_OEN_TEST        (1<<17)
#define ANA_ACCO_TX_LP_PD_TEST      (1<<18)
#define ANA_ACCO_RX_ON_TEST         (1<<24)

//ana_test_ctrl
#define ANA_ACCO_PA_TEST1           (1<<0)
#define ANA_ACCO_PA_TEST2           (1<<1)
#define ANA_ACCO_AFC_TEST1          (1<<4)
#define ANA_ACCO_AFC_TEST2          (1<<5)
#define ANA_ACCO_GP_TEST1           (1<<8)
#define ANA_ACCO_GP_TEST2           (1<<9)
#define ANA_ACCO_AUDIO_TEST1        (1<<12)
#define ANA_ACCO_AUDIO_TEST2        (1<<13)
#define ANA_ACCO_AUDIO_TEST3        (1<<14)
#define ANA_ACCO_TX_TEST1           (1<<16)
#define ANA_ACCO_TX_TEST2           (1<<17)
#define ANA_ACCO_TX_TEST3           (1<<18)
#define ANA_ACCO_TX_TEST4           (1<<19)
#define ANA_ACCO_RX_TEST1           (1<<20)
#define ANA_ACCO_RX_TEST2           (1<<21)
#define ANA_ACCO_RX_TEST3           (1<<22)
#define ANA_ACCO_RX_TEST4           (1<<23)





#endif
