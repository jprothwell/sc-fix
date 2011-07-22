////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_ana_gpadc.c $ //
//    $Author: jiancui $                                                       //
//    $Date: 2011-07-07 20:48:54 +0800 (Thu, 07 Jul 2011) $                     //
//    $Revision: 8886 $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_ana_gpadc.h
///                                                                           //
/// Contains the GPADC driver Implementation.                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// FIXME Implement for Gallite !!

#include "chip_id.h"
#include "cs_types.h"

#include "global_macros.h"
#include "gpadc.h"
#if (CHIP_HAS_ACCO_PLL) // TODO : need a general flag for all ACOO macros?
#include "ana_acco.h"
#endif

#include "hal_sys.h"
#include "hal_ana_gpadc.h"

#include "halp_ana_gpadc.h"
#include "halp_sys.h"
#include "halp_debug.h"
#include "halp_calib.h"

#include "calib_m.h"


#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

#define GPADC_FAST_CLOSE 1

// Battery voltage = gpadc voltage * 3
#define HAL_ANA_GPADC_MVOLT_A   1133
#define HAL_ANA_GPADC_MVOLT_B   1400

#else // !Gallite
// =============================================================================
// HAL_ANA_GPADC_MVOLT_A
// -----------------------------------------------------------------------------
/// GPADC value to mV conversion constant A. (1700 mV measured)
// =============================================================================
#define HAL_ANA_GPADC_MVOLT_A   1700

// =============================================================================
// HAL_ANA_GPADC_MVOLT_B
// -----------------------------------------------------------------------------
/// GPADC value to mV conversion constant B. (2100 mV measured)
// =============================================================================
#define HAL_ANA_GPADC_MVOLT_B   2100

#endif // !Gallite


PRIVATE INT32 g_halGpadcSlope  = 0;
PRIVATE INT32 g_halGpadcIntcpt  = 0;
PRIVATE BOOL  g_halGpadcCalibrated = FALSE;

PRIVATE UINT8 g_halCalibPreviousBandgap = 0;
PUBLIC UINT32 g_halgpadcCalibHigh=0;
PUBLIC UINT32 g_halgpadcCalibLow=0;
PRIVATE UINT16 g_halGpadcLastVal[HAL_ANA_GPADC_CHAN_QTY] = 
    {
        HAL_ANA_GPADC_BAD_VALUE,
        HAL_ANA_GPADC_BAD_VALUE,
        HAL_ANA_GPADC_BAD_VALUE,
        HAL_ANA_GPADC_BAD_VALUE
    };

// using HAL_ANA_GPADC_ATP_QTY value for disabled channels 
// (so min atp is HAL_ANA_GPADC_ATP_QTY when all disabled!)
PRIVATE UINT8 g_halGpadcAtpConfig[HAL_ANA_GPADC_CHAN_QTY] = 
    {
        HAL_ANA_GPADC_ATP_QTY,
        HAL_ANA_GPADC_ATP_QTY,
        HAL_ANA_GPADC_ATP_QTY,
        HAL_ANA_GPADC_ATP_QTY
    };


#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
#ifndef FPGA
// =============================================================================
// hal_AnaGpadcPowerOn
// -----------------------------------------------------------------------------
/// Power on the GPADC hardware.
// =============================================================================
PRIVATE BOOL hal_AnaGpadcPowerOn(VOID)
{
    // Request freq resource
    hal_SysRequestFreq(HAL_SYS_FREQ_GPADC, HAL_SYS_FREQ_26M, NULL);

    UINT32 status = hal_SysEnterCriticalSection();
    // For async reset: first clear reset, second enable clock
    // Clear GPADC reset
    hwp_sysCtrl->Sys_Rst_Clr = SYS_CTRL_CLR_RST_GPADC;
    // Enable GPADC clock
    hwp_sysCtrl->Clk_Sys_Enable = SYS_CTRL_ENABLE_SYS_GPADC;
    hal_SysExitCriticalSection(status);

    // Setup s/w power control mode
    hwp_gpadc->ctrl = GPADC_PD_MODE_SW;
    // Power on ABB GPADC
    hwp_gpadc->cmd_clr = GPADC_GPADC_PD;
    // Clear ABB GPADC reset
    hwp_gpadc->cmd_clr = GPADC_GPADC_RESET;
    // Clear previous IRQs
    hwp_gpadc->irq_clr = GPADC_THRSHD0_IRQ|GPADC_THRSHD1_IRQ|GPADC_EOC;

    return TRUE;
}

// =============================================================================
// hal_AnaGpadcPowerOff
// -----------------------------------------------------------------------------
/// Power off the GPADC hardware.
// =============================================================================
PRIVATE VOID hal_AnaGpadcPowerOff(VOID)
{
#if (GPADC_FAST_CLOSE == 0)
    // Clear previous IRQs
    hwp_gpadc->irq_clr = GPADC_THRSHD0_IRQ|GPADC_THRSHD1_IRQ|GPADC_EOC;
    // Reset ABB GPADC
    hwp_gpadc->cmd_set = GPADC_GPADC_RESET;
    // Setup s/w power control mode
    hwp_gpadc->ctrl = GPADC_PD_MODE_SW;
    // Power off ABB GPADC
    hwp_gpadc->cmd_set = GPADC_GPADC_PD;
#endif // (GPADC_FAST_CLOSE == 0)

    UINT32 status = hal_SysEnterCriticalSection();
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    // Disable GPADC clock
    hwp_sysCtrl->Clk_Sys_Disable = SYS_CTRL_DISABLE_SYS_GPADC;
    // Reset GPADC
    hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SET_RST_GPADC;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
    hal_SysExitCriticalSection(status);

    // Release freq resource.
    hal_SysRequestFreq(HAL_SYS_FREQ_GPADC, HAL_SYS_FREQ_32K, NULL);
}
#endif // !FPGA
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

// =============================================================================
// hal_AnaGpadcMinAtp
// -----------------------------------------------------------------------------
/// @return minimum Atp value for configured channels, 
/// #HAL_ANA_GPADC_ATP_QTY when all chnnels are closed.
///
/// Use the global configuration variable \c g_halGpadcAtpConfig.
// =============================================================================
PRIVATE UINT8 hal_AnaGpadcMinAtp(VOID)
{
    HAL_ANA_GPADC_CHAN_T ch;
    UINT8 minAtp = HAL_ANA_GPADC_ATP_QTY;

    for (ch=HAL_ANA_GPADC_CHAN_0; ch<HAL_ANA_GPADC_CHAN_QTY; ch++)
    {
        if (g_halGpadcAtpConfig[ch] < minAtp) minAtp = g_halGpadcAtpConfig[ch];
    }

    return minAtp;
}

// =============================================================================
// hal_AnaGpadcGpadc2Volt
// -----------------------------------------------------------------------------
/// Convert the value measured by the GPADC into mV, using the calibration
/// parameters.
///
/// @param gpadcVal Value read from the GPADC hardware module.
/// @return The corresponding measured tension, in mV.
// =============================================================================
PROTECTED HAL_ANA_GPADC_MV_T hal_AnaGpadcGpadc2Volt(UINT16 gpadcVal)
{
    INT32 y1, y2, x1, x2;
    INT32 voltage;
    HAL_ANA_GPADC_MV_T rVoltage;

    if (gpadcVal == HAL_ANA_GPADC_BAD_VALUE)
    {
        // Bad values from the GPADC are still Bad Values
        // for the voltage-speaking user.
        return HAL_ANA_GPADC_BAD_VALUE;
    }
    else
    {
        y1 = HAL_ANA_GPADC_MVOLT_A*1000;
        y2 = HAL_ANA_GPADC_MVOLT_B*1000;

#ifdef GALLITE_IS_CT1129
        if(g_halgpadcCalibHigh)
        {
            x1 = g_halgpadcCalibLow;
            x2 = g_halgpadcCalibHigh;
        }
        else
        {
            x1 = g_halCalibration->bb->gpadc.sensorGainA;
            x2 = g_halCalibration->bb->gpadc.sensorGainB;
        }
#else
        x1 = g_halCalibration->bb->gpadc.sensorGainA;
        x2 = g_halCalibration->bb->gpadc.sensorGainB;
#endif
        if (!g_halGpadcCalibrated)
        {
            g_halGpadcSlope = (y2-y1)/(x2-x1);
            g_halGpadcIntcpt = ((y1*x2)-(x1*y2))/((x2-x1)*1000);
            g_halGpadcCalibrated = TRUE;
        }

        voltage = (((g_halGpadcSlope*gpadcVal)/1000)+(g_halGpadcIntcpt));
        if (voltage < 0)
        {
            rVoltage = 0;   // Should never be negative
        }
        else
        {
            rVoltage = (HAL_ANA_GPADC_MV_T)voltage;
        }
        return rVoltage;
    }
}

// =============================================================================
// hal_AnaGpadcDisable
// -----------------------------------------------------------------------------
/// Really close the GPADC hardware and stop periodic measurements.
/// parameters.
// =============================================================================
PRIVATE VOID hal_AnaGpadcDisable(VOID)
{
#if (defined(FPGA) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))

    // GPADC not implemented on FPGA
    return ;

#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

#if (GPADC_FAST_CLOSE == 0)
    // Close the GPADC module ()
    // Check if already enabled 
    if (hwp_gpadc->ctrl & (GPADC_CH0_EN|GPADC_CH1_EN|GPADC_CH2_EN|GPADC_CH3_EN))
    {
        while(hwp_gpadc->status & GPADC_BUSY);
    }

    // Disable all measurement
    hwp_gpadc->ctrl = 0;
    // Wait for the GPADC to be ready
    while(hwp_gpadc->status & GPADC_BUSY);
    // Clear EOC status bit
    hwp_gpadc->irq_clr = GPADC_EOC;
#endif // (GPADC_FAST_CLOSE == 0)

    // Power off ABB GPADC
    hal_AnaGpadcPowerOff();
    return;

#else // !FPGA && !Esperite && !Gallite

    // Close the GPADC module ()
    // Check if already enabled 
    if (hwp_gpadc->gpadc_ch_ctrl & (GPADC_CH0_EN|GPADC_CH1_EN|GPADC_CH2_EN|GPADC_CH3_EN))
    {
        while(hwp_gpadc->gpadc_ch_ctrl & GPADC_BUSY);

        // Disable all measurement
        hwp_gpadc->gpadc_ch_ctrl = 0;
        // Wait for the GPADC to be ready
        while(hwp_gpadc->gpadc_ch_ctrl & GPADC_BUSY);

        // Release the resource.
        hal_SysRequestFreq(HAL_SYS_FREQ_GPADC, HAL_SYS_FREQ_32K, NULL);

        // Configure the analog module.
        hwp_anaAccoFe->pa_afc_gp_ctrl_clr = ANA_ACCO_GPADC_RESET_N | ANA_ACCO_GPADC_DISABLE;
        // hwp_anaAccoFe->ana_clk_ctrl  = kept at the reset state.
    }

#endif // !FPGA && !Esperite && !Gallite
}

// =============================================================================
// hal_AnaGpadcEnable
// -----------------------------------------------------------------------------
/// Really open the GPADC hardware and start periodic measurements.
/// If no channels are enabled, it will call #hal_AnaGpadcDisable to ensure
/// the GPADC hardware is disabled.
///
/// Use the global configuration variable \c g_halGpadcConfig.
// =============================================================================
PRIVATE VOID hal_AnaGpadcEnable(VOID)
{
#if (defined(FPGA) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))

    // GPADC not implemented on FPGA
    return ;

#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

    // If already enabled, the driver handle that situtation by
    // becoming blocking as long as the GPADC is not ready for the new
    // setting. (few 26Mhz clock cycles)
    UINT32 gpadcControl = 0;
    UINT8 minAtp = hal_AnaGpadcMinAtp();

    // Only open the GPADC if at least one channel is required.
    if (minAtp != HAL_ANA_GPADC_ATP_QTY)
    {
        // If a channel is already enabled, wait for the fitted time
        // to write the register.
        if (hwp_gpadc->ctrl & (GPADC_CH0_EN|GPADC_CH1_EN|GPADC_CH2_EN|GPADC_CH3_EN))
        {
            while(hwp_gpadc->status & GPADC_BUSY);
        }
        else
        {
            // Only needed for the first time
            // Power on ABB GPADC.
            hal_AnaGpadcPowerOn();
        }

        // Because hardware will actually change it's ATP after the current perriod
        // is elapsed, switching from slow to fast will actually be long:
        // for example switching from GPADC_ATP_2S to GPADC_ATP_1MS will take 2s
        // Limit the atp to 10ms to hide some of this effect
        /// @todo: put this limit in HAL TGT config
        if (minAtp > HAL_ANA_GPADC_ATP_10MS)
        {
            minAtp = HAL_ANA_GPADC_ATP_122US; //HAL_ANA_GPADC_ATP_10MS;
        }

        // Configure the GPADC module.
        // Hardware ATP has a strange order, reorder it there
        switch (minAtp)
        {
            case HAL_ANA_GPADC_ATP_122US :
                gpadcControl |= GPADC_ATP_122US;
                break;
            case HAL_ANA_GPADC_ATP_1MS :
                gpadcControl |= GPADC_ATP_1MS;
                break;
            case HAL_ANA_GPADC_ATP_10MS :
                gpadcControl |= GPADC_ATP_10MS;
                break;
            case HAL_ANA_GPADC_ATP_100MS :
                gpadcControl |= GPADC_ATP_100MS;
                break;
            case HAL_ANA_GPADC_ATP_250MS :
                gpadcControl |= GPADC_ATP_250MS;
                break;
            case HAL_ANA_GPADC_ATP_500MS :
                gpadcControl |= GPADC_ATP_500MS;
                break;
            case HAL_ANA_GPADC_ATP_1S :
                gpadcControl |= GPADC_ATP_1S;
                break;
            case HAL_ANA_GPADC_ATP_2S :
                gpadcControl |= GPADC_ATP_2S;
                break;
            default :
                HAL_ASSERT(FALSE, "Invalid atp value", minAtp);
        }
        
        if (g_halGpadcAtpConfig[0] != HAL_ANA_GPADC_ATP_QTY)
        {
            gpadcControl |= GPADC_CH0_EN;
        }
        if (g_halGpadcAtpConfig[1] != HAL_ANA_GPADC_ATP_QTY)
        {
            gpadcControl |= GPADC_CH1_EN;
        }
        if (g_halGpadcAtpConfig[2] != HAL_ANA_GPADC_ATP_QTY)
        {
            gpadcControl |= GPADC_CH2_EN;
        }
        if (g_halGpadcAtpConfig[3] != HAL_ANA_GPADC_ATP_QTY)
        {
            gpadcControl |= GPADC_CH3_EN;
        }

        gpadcControl |= GPADC_PD_MODE_HW;
        hwp_gpadc->ctrl = gpadcControl;
        // Clear EOC status bit
        hwp_gpadc->irq_clr = GPADC_EOC;
        hwp_gpadc->cmd_set = GPADC_START_MEASURE;
    }
    else
    {
        // No channels are enabled, so ensure the GPADC module is disabled.
        hal_AnaGpadcDisable();
    }
    return;

#else // !FPGA && !Esperite && !Gallite

    // If already enabled, the driver handle that situtation by
    // becoming blocking as long as the GPADC is not ready for the new
    // setting. (few 26Mhz clock cycles)
    UINT32 gpadcControl = 0;
    UINT8 minAtp = hal_AnaGpadcMinAtp();

    // Only open the GPADC if at least one channel is required.
    if (minAtp != HAL_ANA_GPADC_ATP_QTY)
    {
        // If a channel is already enabled, wait for the fitted time
        // to write the register.
        if (hwp_gpadc->gpadc_ch_ctrl & (GPADC_CH0_EN|GPADC_CH1_EN|GPADC_CH2_EN|GPADC_CH3_EN))
        {
            while(hwp_gpadc->gpadc_ch_ctrl & GPADC_BUSY);
        }
        else
        {
            // Only needed for the first time
            // Configure the analog module.
            hal_SysRequestFreq(HAL_SYS_FREQ_GPADC, HAL_SYS_FREQ_26M, NULL);

            // Automatic enable is good for us, so we use it
            // And is set by hal_Open
            hwp_anaAccoFe->pa_afc_gp_ctrl_set = ANA_ACCO_GPADC_RESET_RELEASE | ANA_ACCO_GPADC_ENABLE;
            // hwp_anaAccoFe->ana_clk_ctrl  => set by hal_SysSetupSystemClock
        }
        
        // Because hardware will actually change it's ATP after the current perriod
        // is elapsed, switching from slow to fast will actually be long:
        // for example switching from GPADC_ATP_2S to GPADC_ATP_1MS will take 2s
        // Limit the atp to 10ms to hide some of this effect
        /// @todo: put this limit in HAL TGT config
        if (minAtp > HAL_ANA_GPADC_ATP_10MS)
        {
            minAtp = HAL_ANA_GPADC_ATP_10MS;
        }

        // Configure the GPADC module.
        // Hardware ATP has a strange order, reorder it there
        switch (minAtp)
        {
            case HAL_ANA_GPADC_ATP_122US :
                gpadcControl |= GPADC_ATP_122US;
                break;
            case HAL_ANA_GPADC_ATP_1MS :
                gpadcControl |= GPADC_ATP_1MS;
                break;
            case HAL_ANA_GPADC_ATP_10MS :
                gpadcControl |= GPADC_ATP_10MS;
                break;
            case HAL_ANA_GPADC_ATP_100MS :
                gpadcControl |= GPADC_ATP_100MS;
                break;
            case HAL_ANA_GPADC_ATP_250MS :
                gpadcControl |= GPADC_ATP_250MS;
                break;
            case HAL_ANA_GPADC_ATP_500MS :
                gpadcControl |= GPADC_ATP_500MS;
                break;
            case HAL_ANA_GPADC_ATP_1S :
                gpadcControl |= GPADC_ATP_1S;
                break;
            case HAL_ANA_GPADC_ATP_2S :
                gpadcControl |= GPADC_ATP_2S;
                break;
            default :
                HAL_ASSERT(FALSE, "Invalid atp value", minAtp);
        }
        
        if (g_halGpadcAtpConfig[0] != HAL_ANA_GPADC_ATP_QTY)
        {
            gpadcControl |= GPADC_CH0_EN;
        }
        if (g_halGpadcAtpConfig[1] != HAL_ANA_GPADC_ATP_QTY)
        {
            gpadcControl |= GPADC_CH1_EN;
        }
        if (g_halGpadcAtpConfig[2] != HAL_ANA_GPADC_ATP_QTY)
        {
            gpadcControl |= GPADC_CH2_EN;
        }
        if (g_halGpadcAtpConfig[3] != HAL_ANA_GPADC_ATP_QTY)
        {
            gpadcControl |= GPADC_CH3_EN;
        }
        
        hwp_gpadc->gpadc_ch_ctrl = gpadcControl;
    }
    else
    {
        // No channels are enabled, so ensure the GPADC module is disabled.
        hal_AnaGpadcDisable();
    }

#endif // !FPGA && !Esperite && !Gallite
}

// =============================================================================
// hal_AnaGpadcOpen
// -----------------------------------------------------------------------------
/// Open a GPADC channel and start periodic measurements.
///
/// This function requests a resource of #HAL_SYS_FREQ_26M.
///
/// That function starts measurements on one of the 4 periodic GPADC channels.
/// The ATP (Acquisition Time Period)is common, so the minimal value of atp
/// is used. Using atp value above HAL_ANA_GPADC_ATP_100MS allows to enter 
/// low power between the measure.
/// The measured channels are selected through
/// #HAL_ANA_GPADC_CFG_T parameter.
///
/// @param channel Channel to enable
/// @param atp Atp value required for that channel
// =============================================================================
PUBLIC VOID hal_AnaGpadcOpen(HAL_ANA_GPADC_CHAN_T channel, HAL_ANA_GPADC_ATP_T atp)
{
    HAL_ASSERT(g_halCalibration == calib_GetPointers(), 
            "GPADC Opening with HAL calib pointer inconsistency");

    // store the atp to the configuration array:
    g_halGpadcAtpConfig[channel] = atp;

    // enable the GPADC hardware
    hal_AnaGpadcEnable();
}


// =============================================================================
// hal_AnaGpadcClose
// -----------------------------------------------------------------------------
/// Close a GPADC channel. 
///
/// This function release the resource to #HAL_SYS_FREQ_32K when all channels
/// are closed.
///
/// This function stops the periodic measurements on a particular channel.
/// It close the GPADC hardware when all channels are disabled.
// =============================================================================
PUBLIC VOID hal_AnaGpadcClose(HAL_ANA_GPADC_CHAN_T channel)
{
    // store HAL_ANA_GPADC_ATP_QTY to the configuration array to disable the channel:
    g_halGpadcAtpConfig[channel] = HAL_ANA_GPADC_ATP_QTY;

    // open remaining enabled channels or close the GPADC hardware
    hal_AnaGpadcEnable();
}



// =============================================================================
// hal_AnaGpadcGet
// -----------------------------------------------------------------------------
/// Return the more up-to-date value measure for the given channel in mV. 
/// If the conversion is not finished yet, #HAL_ANA_GPADC_BAD_VALUE is returned.
///
/// Once the first measured has been correctly done, there will always
/// be a value ready to read for all the enabled channel.
///
/// @param channel Channel whose more up to date measured value
/// will be returned.
/// @return Measured value, in mV, or #HAL_ANA_GPADC_BAD_VALUE if no result is 
/// available yet.
// =============================================================================
PUBLIC HAL_ANA_GPADC_MV_T hal_AnaGpadcGet(HAL_ANA_GPADC_CHAN_T channel)
{
    HAL_ANA_GPADC_MV_T mv = hal_AnaGpadcGpadc2Volt(hal_AnaGpadcGetRaw(channel));

    //HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGet ch=%d val=%dmV",channel, mv);
    return (mv);
}




// =============================================================================
// hal_AnaGpadcGetRaw
// -----------------------------------------------------------------------------
/// Return the more up do date value measured for the specified GPADC channel,
/// if available, as the raw value read in the GPADC register. (no conversion
/// towards voltage is done.)
/// If no result is available because the conversion is not ready
/// yet, #HAL_ANA_GPADC_BAD_VALUE is returned.
/// 
/// @param channel Channel whose more up to date measured value will be returned.
/// @return Raw measured value for this channel by the GPADC,  or 
/// #HAL_ANA_GPADC_BAD_VALUE if no result is available yet.
// =============================================================================
UINT32 g_debugDumpGpadcRawValue = 0;
PUBLIC UINT16 hal_AnaGpadcGetRaw(HAL_ANA_GPADC_CHAN_T channel)
{
#if (defined(FPGA) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))

    // GPADC not implemented on FPGA
    return HAL_ANA_GPADC_BAD_VALUE;

#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

    UINT32 enabledMask = 0;
    REG32*  dataAddress = NULL;
    if (hwp_gpadc->status & GPADC_EOC)
    {   // Conversion is done, read the GPADC
        switch (channel)
        {
            case HAL_ANA_GPADC_CHAN_0:
                enabledMask = GPADC_CH0_EN;
                dataAddress = &hwp_gpadc->data_ch0;
                break;

            case HAL_ANA_GPADC_CHAN_1:
                enabledMask = GPADC_CH1_EN;
                dataAddress = &hwp_gpadc->data_ch1;
                break;

            case HAL_ANA_GPADC_CHAN_2:
                enabledMask = GPADC_CH2_EN;
                dataAddress = &hwp_gpadc->data_ch2;
                break;

            case HAL_ANA_GPADC_CHAN_3:
                enabledMask = GPADC_CH3_EN;
                dataAddress = &hwp_gpadc->data_ch3;
                break;

            default:
                HAL_ASSERT(FALSE, "hal_AnaGpadcGet called for an inexisting"\
                        "channel number");
        }

        if (hwp_gpadc->ctrl & enabledMask)
        {
            UINT16 rdata= (UINT16)*dataAddress;
            g_halGpadcLastVal[channel] = rdata;
            if (g_debugDumpGpadcRawValue)
            {
                HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGetRaw ch=%d val=0x%x",channel, rdata);
            }
            return rdata;
        }
        else
        {   // channel not enabled
            if (g_debugDumpGpadcRawValue)
            {
                HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGetRaw ch=%d val=0x%x DISABLED",channel, HAL_ANA_GPADC_BAD_VALUE);
            }
            return HAL_ANA_GPADC_BAD_VALUE;
        }
    }
    else
    {   // Conversion is not done, read the saved data at last sleep 
        // (will return HAL_ANA_GPADC_BAD_VALUE for disabled channels
        if (g_debugDumpGpadcRawValue)
        {
            HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGetRaw ch=%d val=0x%x SAVED",channel, g_halGpadcLastVal[channel]);
        }
        return g_halGpadcLastVal[channel];
    }

#else // !FPGA && !Esperite && !Gallite

    UINT32 enabledMask = 0;
    REG32*  dataAddress = NULL;
    if (hwp_gpadc->gpadc_ch_ctrl & GPADC_EOC)
    {   // Conversion is done, read the GPADC
        switch (channel)
        {
            case HAL_ANA_GPADC_CHAN_0:
                enabledMask = GPADC_CH0_EN;
                dataAddress = &hwp_gpadc->gpadc_ch0;
                break;

            case HAL_ANA_GPADC_CHAN_1:
                enabledMask = GPADC_CH1_EN;
                dataAddress = &hwp_gpadc->gpadc_ch1;
                break;

            case HAL_ANA_GPADC_CHAN_2:
                enabledMask = GPADC_CH2_EN;
                dataAddress = &hwp_gpadc->gpadc_ch2;
                break;

            case HAL_ANA_GPADC_CHAN_3:
                enabledMask = GPADC_CH3_EN;
                dataAddress = &hwp_gpadc->gpadc_ch3;
                break;

            default:
                HAL_ASSERT(FALSE, "hal_AnaGpadcGet called for an inexisting"\
                        "channel number");
        }

        if (hwp_gpadc->gpadc_ch_ctrl & enabledMask)
        {
            UINT16 rdata= (UINT16)*dataAddress;
            //HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGetRaw ch=%d val=0x%x",channel, rdata);
            return rdata;
        }
        else
        {   // channel not enabled
            //HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGetRaw ch=%d val=0x%x DISABLED",channel, HAL_ANA_GPADC_BAD_VALUE);
            return HAL_ANA_GPADC_BAD_VALUE;
        }
    }
    else
    {   // Conversion is not done, read the saved data at last sleep 
        // (will return HAL_ANA_GPADC_BAD_VALUE for disabled channels
        //HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGetRaw ch=%d val=0x%x SAVED",channel, g_halGpadcLastVal[channel]);
        return g_halGpadcLastVal[channel];
    }

#endif // !FPGA && !Esperite && !Gallite
}



// =============================================================================
// hal_AnaGpadcCalibBandgap
// -----------------------------------------------------------------------------
// Calibration of the analog macros' bandgap.
// =============================================================================
PROTECTED VOID hal_AnaGpadcCalibBandgap(VOID)
{
#if (defined(FPGA) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE) || \
     (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE))
        return;
#else // !FPGA && !Esperite && !Gallite
    if (g_halCalibration == NULL)
    {
        // In the flashprogrammer, we don't set
        // g_halCalibration

        // Enable Rx bandgap as ACCO PLL needs it !!!
        hwp_anaAccoFe->rf_rx_calib = 0x0;
        hwp_anaAccoFe->rf_rx_calib = ANA_ACCO_RX_BG_ENABLE 
                | ANA_ACCO_RX_BG_CTRL(0x25)
                | ANA_ACCO_RX_REF_CAL(3);
        return;
    }

    // Configure the bandgap. 
    // You may notice some odities, as some registers
    // cleared. It is to reproduce the exact Jade behaviour.
    // If it's useless, it will be gotten rid of.

    // pll control 
    hwp_anaAccoFe->pll_ctrl = ANA_ACCO_PLL_PD_N_DISABLE;
    hwp_anaAccoFe->pll_ctrl = ANA_ACCO_PLL_CP_TUNE_100_PERCENT
                            | ANA_ACCO_PLL_PD_N_ENABLE;
        
    // Ana audio control.
    hwp_anaAccoFe->ana_test_ctrl = 0;
    hwp_anaAccoFe->audio_calib = ANA_ACCO_AU_BG_ENABLE
                               | ANA_ACCO_AU_BG_CTRL(0x25)
                               | ANA_ACCO_AU_REF_ENABLE
                               | ANA_ACCO_AU_REF_CAL(0x3);

    // Comm control. 
    hal_AnaGpadcCalibUpdateBandgap();

    // Tx control. 
    hwp_anaAccoFe->rf_tx_calib = 0x0;
    hwp_anaAccoFe->rf_tx_calib = ANA_ACCO_TX_BG_ENABLE
            | ANA_ACCO_TX_BG_CTRL(0x25)
            | ANA_ACCO_TX_DAC_CURRENT_CALI(g_halCalibration->bb->rfAnalog.txDacCurI)
            | ANA_ACCO_TX_DAC_CURRENT_CALQ(g_halCalibration->bb->rfAnalog.txDacCurQ)
            | ANA_ACCO_TX_REF_VOLTAGE_CAL(g_halCalibration->bb->rfAnalog.txRefVolt);

    // Rx control. 
    hwp_anaAccoFe->rf_rx_calib = 0x0;
    hwp_anaAccoFe->rf_rx_calib = ANA_ACCO_RX_BG_ENABLE | ANA_ACCO_RX_BG_CTRL(0x25)
            | ANA_ACCO_RX_REF_CAL(g_halCalibration->bb->rfAnalog.rxRefCal);
            // | RF_DEEP_PD; is there for Jade ...
#endif // !FPGA && !Esperite && !Gallite
}




// =============================================================================
// hal_AnaGpadcCalibUpdateBandgap
// -----------------------------------------------------------------------------
/// Update the bandgap of the GPADC macro.
// =============================================================================
PUBLIC VOID hal_AnaGpadcCalibUpdateBandgap(VOID)
{
#if (defined(FPGA) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE) || \
     (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE))
        g_halCalibPreviousBandgap = 0; // avoid warning
        return;
#else // !FPGA && !Esperite && !Gallite
    if (g_halCalibration == NULL)
    {
        // In the flashprogrammer, we don't set
        // g_halCalibration
        return;
    }

    UINT8 newBandgap = g_halCalibration->bb->gpadc.bandgap;

//  Limit for the effectif bandgap. 
    if (newBandgap > 0x2f)
    {
        newBandgap = 0x2f;
    }
    else if (newBandgap < 0x20)
    {
        newBandgap = 0x20;
    }

//  Update the register only if the bandgap value changed. 
    if (g_halCalibPreviousBandgap == newBandgap)
    {
        return;
    }
    
    
//  Update only the bandgap in the register. 
    hwp_anaAccoFe->pa_afc_gp_calib = ANA_ACCO_COMM_BG_CTRL(newBandgap)
                                |  ANA_ACCO_COMM_BG_ENABLE;

//  Keep the previous value of the bandgap. 
    g_halCalibPreviousBandgap = newBandgap;
#endif // !FPGA && !Esperite && !Gallite
}


// =======================================================
// hal_AnaGpadcSleep
// -------------------------------------------------------
/// called in #hal_LpsSleep to try to shut down the GPADC
/// and release the resource. (only if atp >= 100MS)
// =======================================================
PROTECTED VOID hal_AnaGpadcSleep(VOID)
{
#if (defined(FPGA) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
    // GPADC not implemented in FPGA
    return;
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
    if (hwp_gpadc->ctrl & (GPADC_CH0_EN|GPADC_CH1_EN|GPADC_CH2_EN|GPADC_CH3_EN))
#else // !FPGA && !Esperite && !Gallite
    if (hwp_gpadc->gpadc_ch_ctrl & (GPADC_CH0_EN|GPADC_CH1_EN|GPADC_CH2_EN|GPADC_CH3_EN))
#endif // !FPGA && !Esperite && !Gallite
    {
        UINT8 minAtp = hal_AnaGpadcMinAtp();
        if (minAtp >= HAL_ANA_GPADC_ATP_100MS)
        {
            HAL_ANA_GPADC_CHAN_T channel;
            // initialyse the last values
            for (channel=HAL_ANA_GPADC_CHAN_0; channel<HAL_ANA_GPADC_CHAN_QTY; channel++)
            {
                g_halGpadcLastVal[channel] = hal_AnaGpadcGetRaw(channel);
            }

            hal_AnaGpadcDisable();
        }
        // otherwise GPADC will always be working (and LP mode is not allowed), until all
        // the channels with ATP<100ms are closed explicitly.
    }
}

// =======================================================
// hal_AnaGpadcWakeUp
// -------------------------------------------------------
/// called in #hal_LpsSleep to resume the GPADC operation
/// can take back the resource (if GPADC is open)
// =======================================================
PROTECTED VOID hal_AnaGpadcWakeUp(VOID)
{
#if (defined(FPGA) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
    // GPADC not implemented in FPGA
    return;
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
    if ((hwp_gpadc->ctrl & (GPADC_CH0_EN|GPADC_CH1_EN|GPADC_CH2_EN|GPADC_CH3_EN)) == 0)
#else // !FPGA && !Esperite && !Gallite
    if ((hwp_gpadc->gpadc_ch_ctrl & (GPADC_CH0_EN|GPADC_CH1_EN|GPADC_CH2_EN|GPADC_CH3_EN)) == 0)
#endif // !FPGA && !Esperite && !Gallite
    {
        hal_AnaGpadcEnable();
    }
}

// =============================================================================
// hal_AnaGpadcGetCalibHigh
// -----------------------------------------------------------------------------
/// Get GPADC Calib High sample
// =============================================================================
PUBLIC UINT32 * hal_AnaGpadcGetCalibHigh(VOID)
{
    return &g_halgpadcCalibHigh;
}

// =============================================================================
// hal_AnaGpadcGetCalibLow
// -----------------------------------------------------------------------------
/// Get GPADC Calib low sample
// =============================================================================
PUBLIC UINT32 * hal_AnaGpadcGetCalibLow(VOID)
{
    return &g_halgpadcCalibLow;
}

