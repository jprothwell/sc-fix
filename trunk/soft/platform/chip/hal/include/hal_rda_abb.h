////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2010-2010, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_rda_abb.h $ //
//    $Author: huazeng $                                                        //
//    $Date: 2011-06-03 20:22:01 +0800 (Fri, 03 Jun 2011) $                     //
//    $Revision: 8054 $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_rda_abb.h                                                           //
///                                                                           //
/// This file decribes the RDA Analog Base Band driver API.         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_RDA_ABB_H_
#define _HAL_RDA_ABB_H_

#include "cs_types.h"

typedef enum
{
    GPADC_SETTING = 0x01,
    USB_CONTROL = 0x04,
    CODEC_LDO_SETTING2 = 0x0a,
    CODEC_MISC_SETTING = 0x0b,
    CODEC_MODE_SEL = 0x0c,
    CODEC_CALIB_SETTING = 0x0d,
    CODEC_POWER_CTRL = 0x0e,
    CODEC_RESET_CTRL = 0x0f,
    CODEC_FUNC_FREQ_SAMPLE_SEL = 0x2c,
    CODEC_FUNC_FREQ_SAMPLE_DIV = 0x2d,
} ABB_REG_INDEX_T;

#define CODEC_FUNC_MIN_REG_INDEX (0x20)

// CODEC_MISC_SETTING
typedef enum
{
    DCDC_FREQ_DIV_VALUE_32 = 0,
    DCDC_FREQ_DIV_VALUE_24 = 1,
    DCDC_FREQ_DIV_VALUE_16 = 2,
    DCDC_FREQ_DIV_VALUE_12 = 3,
} DCDC_FREQ_DIV_VALUE;

#define ABB_SYNC_CLK_FREQ(n) (((n)&3)<<11)
#define ABB_SYNC_CLK_FREQ_MASK (3<<11)
#define ABB_SYNC_CLK_FREQ_SHIFT (11)
#define ABB_BOOST_BP_MIC (1<<4)
#define ABB_TEST_EN_ADC (1<<3)
#define ABB_MASH_EN_ADC (1<<2)
#define ABB_MICBIAS_SEL (1<<1)
#define SYN_EN_DAC (1<<0)

// CODEC_MODE_SEL
#define ABB_MODE_SEL_MASK (0x7ff<<4)
#define ABB_SPK_MODE1_SEL_MASK (3<<13)
#define ABB_SPK_MODE1_L_SEL (1<<14)
#define ABB_SPK_MODE1_R_SEL (1<<13)
#define ABB_SPK_MODE2_SEL_MASK (3<<11)
#define ABB_SPK_MODE2_L_SEL (1<<12)
#define ABB_SPK_MODE2_R_SEL (1<<11)
#define ABB_SPK_FM_SEL_MASK (1<<10)
#define ABB_SPK_FM_SEL (1<<10)
#define ABB_HP_MODE1_SEL_MASK (3<<8)
#define ABB_HP_MODE1_L_SEL (1<<9)
#define ABB_HP_MODE1_R_SEL (1<<8)
#define ABB_HP_MODE2_SEL_MASK (3<<6)
#define ABB_HP_MODE2_L_SEL (1<<7)
#define ABB_HP_MODE2_R_SEL (1<<4)
#define ABB_HP_DAC_SEL_MASK (1<<5)
#define ABB_HP_DAC_SEL (1<<5)
#define ABB_HP_FM_SEL_MASK (1<<4)
#define ABB_HP_FM_SEL (1<<4)
#define ABB_PA_EN2_MASK (0xf<<0)
#define ABB_PA_EN_RECI (1<<3)
#define ABB_PA_EN_HP (1<<2)
#define ABB_PA_EN_L_SPK (1<<1)
#define ABB_PA_EN_R_SPK (1<<0)

// CODEC_CALIB_SETTING
#define ABB_CALIB_LINEIN_HP (1<<11)
#define ABB_CALIB_LINEIN_SPK (1<<10)

// CODEC_POWER_CTRL
#define ABB_PD_CODEC_DR (1<<15)
#define ABB_PD_ALL_BIAS (1<<14)
#define ABB_PD_ADC (1<<13)
#define ABB_PD_MICBIAS (1<<12)
#define ABB_PD_DAC_L (1<<11)
#define ABB_PD_DAC_R (1<<10)
#define ABB_PD_DAC_FM_DR (1<<9)
#define ABB_PD_DAC_FM (1<<8)
#define ABB_RSTN_CODEC_DR (1<<5)
#define ABB_RSTN_CODEC (1<<4)
#define ABB_ADC_EN_CODEC_DR (1<<3)
#define ABB_ADC_EN_CODEC (1<<2)
#define ABB_DAC_EN_CODEC_DR (1<<1)
#define ABB_DAC_EN_CODEC (1<<0)

// CODEC_RESET_CTRL
#define ABB_SPI_OUT_ABB (0<<0)
#define ABB_SPI_OUT_CODEC (1<<0)
#define ABB_SPI_OUT_MASK (1<<0)

PUBLIC VOID hal_AbbOpen(VOID);
PUBLIC BOOL hal_AbbRegRead(UINT32 addr, UINT32* pData);
PUBLIC VOID hal_AbbRegBlockingRead(UINT32 addr, UINT32* pData);
PUBLIC BOOL hal_AbbRegWrite(UINT32 addr, UINT32 data);

#endif // _HAL_RDA_ABB_H_

