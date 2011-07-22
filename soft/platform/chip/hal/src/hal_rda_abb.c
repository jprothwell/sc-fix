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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_rda_abb.c $ //
//    $Author: yanshengzhang $                                                        //
//    $Date: 2011-05-06 21:29:02 +0800 (Fri, 06 May 2011) $                     //
//    $Revision: 7456 $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_rda_abb.c                                                           //
///                                                                           //
/// This file decribes the RDA Analog Base Band driver API.         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "sxr_tls.h"

#include "halp_debug.h"
#include "hal_ispi.h"
#include "hal_rda_abb.h"

#define ABB_REG_READ_WAITING_TIME 5

PUBLIC VOID hal_AbbOpen(VOID)
{
    static BOOL abbInitDone = FALSE;

    if (!abbInitDone)
    {
        // reset software and register
        hal_AbbRegWrite(GPADC_SETTING, 0x0000);
        hal_TimDelay(1);
        hal_AbbRegWrite(GPADC_SETTING, 0x0003);

        hal_AbbRegWrite(CODEC_MISC_SETTING,
                        ABB_SYNC_CLK_FREQ(DCDC_FREQ_DIV_VALUE_12)|
                        ABB_MASH_EN_ADC|ABB_MICBIAS_SEL);

        hal_AbbRegWrite(CODEC_POWER_CTRL, ABB_RSTN_CODEC_DR|ABB_RSTN_CODEC|
                                          ABB_DAC_EN_CODEC_DR);

        hal_AbbRegWrite(0x20, 0x002e);
        hal_AbbRegWrite(0x25, 0xeffe);
        hal_AbbRegWrite(0x29, 0x0f0c);
        // MIC AdcGain[12:9] = 0(-10dB), 1(-8dB), 2(-12dB), 3(-4dB)
        // 4(-2dB), 5(-6dB)
#ifdef GALLITE_IS_CT1129
        hal_AbbRegWrite(0x2b, 0x0a03);
#else
        hal_AbbRegWrite(0x2b, 0x0003);
#endif
        hal_AbbRegWrite(CODEC_POWER_CTRL, ABB_RSTN_CODEC_DR|ABB_RSTN_CODEC|
                                          ABB_DAC_EN_CODEC_DR|ABB_DAC_EN_CODEC);

        abbInitDone = TRUE;
    }
}

PRIVATE BOOL hal_AbbIspiCsActivate(BOOL singleWrite)
{
    // No need to activate CS for ISPI single data frame write-only operation
    if(singleWrite)
    {
        return TRUE;
    }

    return hal_IspiCsActivate(HAL_ISPI_CS_ABB);
}

PRIVATE BOOL hal_AbbIspiCsDeactivate(BOOL singleWrite)
{
    // No need to activate CS for ISPI single data frame write-only operation
    if(singleWrite)
    {
        return TRUE;
    }

    return hal_IspiCsDeactivate(HAL_ISPI_CS_ABB);
}

//=============================================================================
// hal_AbbIspiRead
//-----------------------------------------------------------------------------
/// Read codec register on ISPI, the CS must already be activated
/// @param regIdx register to read
/// @return register value                 0 write 1 read
//=============================================================================
PROTECTED UINT32 hal_AbbIspiRead(UINT32 regIdx)
{
    UINT32 wrData, rdData;
    UINT32 count;

    wrData = (1<<25) | ((regIdx & 0x1ff) << 16) | 0;
    
    while(hal_IspiTxFifoAvail(HAL_ISPI_CS_ABB) < 1 ||
            hal_IspiSendData(HAL_ISPI_CS_ABB, wrData, TRUE) == 0);

    //wait until any previous transfers have ended
    while(!hal_IspiTxFinished(HAL_ISPI_CS_ABB));
    count = hal_IspiGetData(HAL_ISPI_CS_ABB, &rdData);
    HAL_ASSERT(1 == count, "ABB ISPI expected 1 data got %d !", count);

    rdData &= 0xffff;
    HAL_TRACE(HAL_SPI_TRC, 0, " ##### ABB reg =0x%x , value = 0x%x ",regIdx,rdData);

    return rdData;
}

//=============================================================================
// hal_AbbIspiWrite
//-----------------------------------------------------------------------------
/// Write codec register on ISPI, the CS must already be activated
/// @param regIdx register to read
/// @param value register value
//=============================================================================
PROTECTED VOID hal_AbbIspiWrite(UINT32 regIdx, UINT32 value)
{
    HAL_TRACE(HAL_SPI_TRC, 0, " ABB write addr = 0x%x data =0x%0x ",regIdx,value);

    UINT32 wrData;
    
    wrData = (0<<25) | ((regIdx & 0x1ff) << 16) | (value&0xffff);

    while(hal_IspiTxFifoAvail(HAL_ISPI_CS_ABB) < 1 ||
            hal_IspiSendData(HAL_ISPI_CS_ABB, wrData, FALSE) == 0);

    //wait until any previous transfers have ended
    while(!hal_IspiTxFinished(HAL_ISPI_CS_ABB));    
}

PUBLIC BOOL hal_AbbRegRead(UINT32 addr, UINT32* pData)
{
    if (!hal_AbbIspiCsActivate(FALSE))
    {
        return FALSE;
    }

    UINT16 spiOutSel = hal_AbbIspiRead(CODEC_RESET_CTRL);
    spiOutSel = (spiOutSel & ~ABB_SPI_OUT_MASK);
    if (addr < CODEC_FUNC_MIN_REG_INDEX)
    {
        spiOutSel |= ABB_SPI_OUT_ABB;
    }
    else
    {
        spiOutSel |= ABB_SPI_OUT_CODEC;
    }
    hal_AbbIspiWrite(CODEC_RESET_CTRL, spiOutSel);

    *pData = hal_AbbIspiRead(addr);
    
    hal_AbbIspiCsDeactivate(FALSE);
    return TRUE;
}

PUBLIC VOID hal_AbbRegBlockingRead(UINT32 addr, UINT32* pData)
{
    while(!hal_AbbRegRead(addr, pData))
    {
        sxr_Sleep(ABB_REG_READ_WAITING_TIME);
    }
}

PUBLIC BOOL hal_AbbRegWrite(UINT32 addr, UINT32 data)
{
#if 0
    if (!hal_AbbIspiCsActivate(TRUE))
    {
        return FALSE;
    }
#endif

    hal_AbbIspiWrite(addr, data);

#if 0
    hal_AbbIspiCsDeactivate(TRUE);
#endif

    return TRUE;
}



