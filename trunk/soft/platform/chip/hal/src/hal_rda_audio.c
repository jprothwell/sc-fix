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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_rda_audio.c $ //
//    $Author: huazeng $                                                        //
//    $Date: 2011-07-12 15:44:27 +0800 (Tue, 12 Jul 2011) $                     //
//    $Revision: 8985 $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_rda_audio.c                                                           //
///                                                                           //
/// This file decribes the RDA Audio driver API.         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "cfg_regs.h"

#include "halp_debug.h"
#include "halp_sys.h"
#include "hal_sys.h"
#include "hal_rda_abb.h"
#include "hal_rda_audio.h"

#include "sxr_tls.h"
#ifdef AUD_EXT_PA_ON_WITH_LCD
#include "pmd_m.h"
#endif // AUD_EXT_PA_ON_WITH_LCD

PRIVATE HAL_AUD_USER_T g_halAudUser = HAL_AUD_USER_NULL;

#ifdef VMIC_POWER_ON_WORKAROUND
typedef enum
{
    HAL_AUD_EP_DETECT_OFF,
    HAL_AUD_EP_DETECT_CLOSING,
    HAL_AUD_EP_DETECT_OPENING,
    HAL_AUD_EP_DETECT_ON,
} HAL_AUD_EP_DETECT_T;
PRIVATE HAL_AUD_EP_DETECT_T g_halAudEarpieceDetectionOn = HAL_AUD_EP_DETECT_OFF;
#endif // VMIC_POWER_ON_WORKAROUND


#ifdef AUD_MUTE_FOR_BT_WORKAROUND
PRIVATE BOOL g_halAudMuteOutput = FALSE;
#endif // AUD_MUTE_FOR_BT_WORKAROUND


#ifdef RECV_USE_SPK_LINE
PUBLIC BOOL g_halAudRecvUseSpkLineRight = FALSE;
typedef enum
{
    HAL_AUD_DAC_PREV_PWR_OFF,
    HAL_AUD_DAC_PREV_PWR_ON,
    HAL_AUD_DAC_PREV_PWR_RESTORED,
} HAL_AUD_DAC_PREV_PWR_T;
PRIVATE HAL_AUD_DAC_PREV_PWR_T g_halAudDacPrevPwr = HAL_AUD_DAC_PREV_PWR_OFF;
#endif // RECV_USE_SPK_LINE

#ifdef AUD_EXT_PA_ON_WITH_LCD
typedef enum
{
    HAL_AUD_EXT_PA_PREV_PWR_OFF,
    HAL_AUD_EXT_PA_PREV_PWR_ON,
    HAL_AUD_EXT_PA_PREV_PWR_RESTORED,
} HAL_AUD_EXT_PA_PREV_PWR_T;
PRIVATE HAL_AUD_EXT_PA_PREV_PWR_T g_halAudExtPaPrevPwr = HAL_AUD_EXT_PA_PREV_PWR_OFF;
UINT32 g_halAudIntSpkPaStableTime = (20 MS_WAITING);
UINT32 g_halAudExtPaStableTime = (350 MS_WAITING);
#endif // AUD_EXT_PA_ON_WITH_LCD

#ifdef REDUCE_RECV_MIN_GAIN
PUBLIC UINT32 g_halRecvReducedLevel = 0;
#endif // REDUCE_RECV_MIN_GAIN


// ===========================================================

PRIVATE VOID hal_AudMuteAllChannels(VOID)
{
    // Mute all channels
    hal_AudSetRcvGain(0);
    hal_AudSetHeadGain(0);
    HAL_AUD_SPK_CFG_T spkCfg;
    spkCfg.reg = 0;
    hal_AudSetSpkCfg(spkCfg);
    HAL_AUD_MIC_CFG_T micCfg;
    micCfg.reg = 0;
    hal_AudSetMicCfg(micCfg);
}

#ifdef VMIC_POWER_ON_WORKAROUND
PUBLIC BOOL hal_AudEarpieceDetectInit(VOID)
{
    //hal_AbbOpen();
    
    // Power on audio codec components
    HAL_AUD_PD_T pdStatus, orgPdStatus;
    orgPdStatus = hal_AudGetPowerStatus();
    if (!orgPdStatus.noDeepPd)
    {
        pdStatus.reg = 0;
        pdStatus.noDeepPd = TRUE;
        hal_AudPowerOn(pdStatus);
        hal_TimDelay(AUD_TIME_POWER_ON_DEEP);
    }
    if (!orgPdStatus.noRefPd)
    {
        pdStatus.reg = 0;
        pdStatus.noRefPd = TRUE;
        hal_AudPowerOn(pdStatus);
        hal_TimDelay(AUD_TIME_POWER_ON_REF);
    }

    // Power on both Mic and AuxMic, then no matter which one
    // is selected in audio_sel_cfg, v_mic will be turned on.
    if (!orgPdStatus.noMicPd)
    {
        pdStatus.reg = 0;
        pdStatus.noMicPd = TRUE;
        hal_AudPowerOn(pdStatus);
        hal_TimDelay(AUD_TIME_POWER_ON_OTHER);
    }
    if (!orgPdStatus.noAuxMicPd)
    {
        pdStatus.reg = 0;
        pdStatus.noAuxMicPd = TRUE;
        hal_AudPowerOn(pdStatus);
        hal_TimDelay(AUD_TIME_POWER_ON_OTHER);
    }
    if (!orgPdStatus.noAdcPd)
    {
        pdStatus.reg = 0;
        pdStatus.noAdcPd = TRUE;
        hal_AudPowerOn(pdStatus);
    }
    
    // No need to request freq for earpiece
    g_halAudEarpieceDetectionOn = HAL_AUD_EP_DETECT_ON;

    return TRUE;
}
#endif // VMIC_POWER_ON_WORKAROUND


#ifdef AUD_MUTE_FOR_BT_WORKAROUND
// =============================================================================
// hal_AudMuteOutput
// -----------------------------------------------------------------------------
/// Power on/off the audio when it is playing.
/// The function is called by MCI only to redirect the audio output to the bluetooth device.
/// The function might be removed if MCI changes its way to switch between bluetooth and normal codec.
/// 
/// @param on TRUE if power on, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID hal_AudMuteOutput(BOOL mute)
{
    g_halAudMuteOutput = mute;

    if (mute)
    {
        // MCI will mute audio after stopping current stream but before restarting it.
        // So the mute operation is acutally done in hal_AudOpen()
        // -- within mutex control.
    }
    else
    {
        // MCI might unmute audio while playing current stream.

        if (g_halAudUser != HAL_AUD_USER_CODEC &&
            g_halAudUser != HAL_AUD_USER_FM &&
            g_halAudUser != HAL_AUD_USER_LINEIN)
        {
            // No stream is being played now
            return;
        }

        // Mutex control is done in aud_core.c

        HAL_AUD_PD_T pdStatus, orgPdStatus;
        orgPdStatus = hal_AudGetPowerStatus();
#if 0
        if (!orgPdStatus.noDeepPd)
        {
            pdStatus.reg = 0;
            pdStatus.noDeepPd = TRUE;
            hal_AudPowerOn(pdStatus);
            sxr_Sleep(AUD_TIME_POWER_ON_DEEP);
        }
        if (!orgPdStatus.noRefPd)
        {
            pdStatus.reg = 0;
            pdStatus.noRefPd = TRUE;
            hal_AudPowerOn(pdStatus);
            sxr_Sleep(AUD_TIME_POWER_ON_REF);
        }
#endif
        if (!orgPdStatus.noDacPd)
        {
            pdStatus.reg = 0;
            pdStatus.noDacPd = TRUE;
            hal_AudPowerOn(pdStatus);
            sxr_Sleep(AUD_TIME_POWER_ON_OTHER);
        }
        if (!orgPdStatus.noDacReset)
        {
            pdStatus.reg = 0;
            pdStatus.noDacReset = TRUE;
            hal_AudPowerOn(pdStatus);
        }
    }
}
#endif // AUD_MUTE_FOR_BT_WORKAROUND


PUBLIC BOOL hal_AudOpen(HAL_AUD_USER_T user, HAL_AUD_CALLBACK_T cbFunc)
{
    BOOL openAud = FALSE;
    UINT32 status;

    status = hal_SysEnterCriticalSection();
#ifdef VMIC_POWER_ON_WORKAROUND
    if (user == HAL_AUD_USER_EARPIECE)
    {
        if (g_halAudEarpieceDetectionOn == HAL_AUD_EP_DETECT_ON)
        {
            // Earpiece already opened
            hal_SysExitCriticalSection(status);
            return TRUE;
        }
        else if (g_halAudEarpieceDetectionOn == HAL_AUD_EP_DETECT_OFF &&
                    g_halAudUser != HAL_AUD_USER_OPENING &&
                    g_halAudUser != HAL_AUD_USER_CLOSING)
        {
            g_halAudEarpieceDetectionOn = HAL_AUD_EP_DETECT_OPENING;
            openAud = TRUE;
        }
    }
    else if (g_halAudUser == HAL_AUD_USER_NULL &&
                g_halAudEarpieceDetectionOn != HAL_AUD_EP_DETECT_OPENING &&
                g_halAudEarpieceDetectionOn != HAL_AUD_EP_DETECT_CLOSING)
    {
        g_halAudUser = HAL_AUD_USER_OPENING;
        openAud = TRUE;
    }
#else // ! VMIC_POWER_ON_WORKAROUND
    if (g_halAudUser == HAL_AUD_USER_NULL)
    {
        g_halAudUser = HAL_AUD_USER_OPENING;
        openAud = TRUE;
    }
#endif // ! VMIC_POWER_ON_WORKAROUND
    hal_SysExitCriticalSection(status);

    if (!openAud)
    {
        return FALSE;
    }

    //hal_AbbOpen();

    // Request freq resource (to avoid entering LP mode from now on,
    // including the following sleeps as well)
    hal_SysRequestFreq(HAL_SYS_FREQ_AUDIO, HAL_SYS_FREQ_26M, NULL);

#ifdef VMIC_POWER_ON_WORKAROUND
    if (user != HAL_AUD_USER_EARPIECE)
    {
#endif // VMIC_POWER_ON_WORKAROUND

    // Mute all channels
    hal_AudMuteAllChannels();

#ifdef VMIC_POWER_ON_WORKAROUND
    }
#endif // VMIC_POWER_ON_WORKAROUND

    // Set VCOM mode
    if (user == HAL_AUD_USER_FM)
    {
        hal_AudSetVcomMode(TRUE);
    }
    else
    {
        hal_AudSetVcomMode(FALSE);
        sxr_Sleep(AUD_TIME_SET_VCOM_MODE);
    }

    // Set default voice mode
    hal_AudSetVoiceMode(FALSE);

    // Invoke callback function before power on codec
    if (cbFunc != NULL)
    {
        (*cbFunc)();
    }

    // Power on audio codec components
    HAL_AUD_PD_T pdStatus, orgPdStatus;
    orgPdStatus = hal_AudGetPowerStatus();
    if (!orgPdStatus.noDeepPd)
    {
        pdStatus.reg = 0;
        pdStatus.noDeepPd = TRUE;
        hal_AudPowerOn(pdStatus);
        sxr_Sleep(AUD_TIME_POWER_ON_DEEP);
    }
    if (!orgPdStatus.noRefPd)
    {
        pdStatus.reg = 0;
        pdStatus.noRefPd = TRUE;
        hal_AudPowerOn(pdStatus);
        sxr_Sleep(AUD_TIME_POWER_ON_REF);
    }

#ifdef VMIC_POWER_ON_WORKAROUND
    if (user == HAL_AUD_USER_EARPIECE)
    {
        // Power on both Mic and AuxMic, then no matter which one
        // is selected in audio_sel_cfg, v_mic will be turned on.
        if (!orgPdStatus.noMicPd)
        {
            pdStatus.reg = 0;
            pdStatus.noMicPd = TRUE;
            hal_AudPowerOn(pdStatus);
            sxr_Sleep(AUD_TIME_POWER_ON_OTHER);
        }
        if (!orgPdStatus.noAuxMicPd)
        {
            pdStatus.reg = 0;
            pdStatus.noAuxMicPd = TRUE;
            hal_AudPowerOn(pdStatus);
            sxr_Sleep(AUD_TIME_POWER_ON_OTHER);
        }
        if (!orgPdStatus.noAdcPd)
        {
            pdStatus.reg = 0;
            pdStatus.noAdcPd = TRUE;
            hal_AudPowerOn(pdStatus);
        }        

        // No need to request freq for earpiece
        hal_SysRequestFreq(HAL_SYS_FREQ_AUDIO, HAL_SYS_FREQ_32K, NULL);
        g_halAudEarpieceDetectionOn = HAL_AUD_EP_DETECT_ON;

        return TRUE;
    }
#endif // VMIC_POWER_ON_WORKAROUND

    if (user == HAL_AUD_USER_CODEC)
    {
        if (!orgPdStatus.noMicPd)
        {
            pdStatus.reg = 0;
            pdStatus.noMicPd = TRUE;
            hal_AudPowerOn(pdStatus);
            sxr_Sleep(AUD_TIME_POWER_ON_OTHER);
        }
        if (!orgPdStatus.noAuxMicPd)
        {
            pdStatus.reg = 0;
            pdStatus.noAuxMicPd = TRUE;
            hal_AudPowerOn(pdStatus);
            sxr_Sleep(AUD_TIME_POWER_ON_OTHER);
        }
        if (!orgPdStatus.noAdcPd)
        {
            pdStatus.reg = 0;
            pdStatus.noAdcPd = TRUE;
            hal_AudPowerOn(pdStatus);
            sxr_Sleep(AUD_TIME_POWER_ON_OTHER);
        }
    }

#ifdef AUD_MUTE_FOR_BT_WORKAROUND
    if (!g_halAudMuteOutput)
#endif // AUD_MUTE_FOR_BT_WORKAROUND
    {
        if (!orgPdStatus.noDacPd)
        {
            pdStatus.reg = 0;
            pdStatus.noDacPd = TRUE;
            hal_AudPowerOn(pdStatus);
            sxr_Sleep(AUD_TIME_POWER_ON_OTHER);
        }
        if (!orgPdStatus.noDacReset)
        {
            pdStatus.reg = 0;
            pdStatus.noDacReset = TRUE;
            hal_AudPowerOn(pdStatus);
        }
    }

    // Request audio module (no need to enter critical section)
    g_halAudUser = user;

    return TRUE;
}

PUBLIC BOOL hal_AudClose(HAL_AUD_USER_T user)
{
    BOOL closeAud = FALSE;
    UINT32 status;

    status = hal_SysEnterCriticalSection();
#ifdef VMIC_POWER_ON_WORKAROUND
   if (user == HAL_AUD_USER_EARPIECE)
    {
        if (g_halAudEarpieceDetectionOn == HAL_AUD_EP_DETECT_OFF)
        {
            // Earpiece already closed
            hal_SysExitCriticalSection(status);
            return TRUE;
        }
        else if (g_halAudEarpieceDetectionOn == HAL_AUD_EP_DETECT_ON)
        {
            if (g_halAudUser == HAL_AUD_USER_NULL)
            {
                g_halAudEarpieceDetectionOn = HAL_AUD_EP_DETECT_CLOSING;
                closeAud = TRUE;
            }
            else if (g_halAudUser != HAL_AUD_USER_OPENING &&
                        g_halAudUser != HAL_AUD_USER_CLOSING)
            {
                // There is another audio user, so just set the flag and do not close the module
                g_halAudEarpieceDetectionOn = HAL_AUD_EP_DETECT_OFF;
                hal_SysExitCriticalSection(status);
                return TRUE;
            }
            // Else audio driver is being configured by a user other than earpiece, and we need to wait
        }
        // Else audio driver is being configured by earpiece, and we need to wait
    }
    else if (g_halAudUser == user)
    {
        if (g_halAudEarpieceDetectionOn != HAL_AUD_EP_DETECT_OPENING &&
             g_halAudEarpieceDetectionOn != HAL_AUD_EP_DETECT_CLOSING)
        {
            g_halAudUser = HAL_AUD_USER_CLOSING;
            closeAud = TRUE;
        }
        // Else audio driver is being configured by earpiece, and we need to wait
    }
    else
    {
            // 1. Audio driver is already closed
            // 2. Audio driver is opened by another user (not earpiece either)
            // 3. Audio driver is being configured by a user other than earpiece
            hal_SysExitCriticalSection(status);
            return TRUE;
    }
#else // ! VMIC_POWER_ON_WORKAROUND
    if (g_halAudUser == user)
    {
        g_halAudUser = HAL_AUD_USER_CLOSING;
        closeAud = TRUE;
    }
    else
    {
            // 1. Audio driver is already closed
            // 2. Audio driver is opened by another user
            // 3. Audio driver is being configured
            hal_SysExitCriticalSection(status);
            return TRUE;
    }
#endif // ! VMIC_POWER_ON_WORKAROUND
    hal_SysExitCriticalSection(status);

    if (!closeAud)
    {
        return FALSE;
    }

#ifdef VMIC_POWER_ON_WORKAROUND
    if (user != HAL_AUD_USER_EARPIECE)
    {
        // Let the sound fade out naturally
        sxr_Sleep(6 MS_WAITING);
    }
#endif // VMIC_POWER_ON_WORKAROUND

    // Mute all channels
    hal_AudMuteAllChannels();

    // Deselect all modules
    HAL_AUD_SEL_T selCfg;
    selCfg.reg = 0;
    hal_AudSetSelCfg(selCfg);

    // Power down finally
    HAL_AUD_PD_T pdStatus;
    pdStatus.reg = HAL_AUD_PD_MASK;
    hal_AudPowerDown(pdStatus);

#ifdef VMIC_POWER_ON_WORKAROUND
    if (user == HAL_AUD_USER_EARPIECE)
    {
        g_halAudEarpieceDetectionOn = HAL_AUD_EP_DETECT_OFF;
        return TRUE;
    }
#endif // VMIC_POWER_ON_WORKAROUND

    // Release freq resource
    hal_SysRequestFreq(HAL_SYS_FREQ_AUDIO, HAL_SYS_FREQ_32K, NULL);
    // Release audio module (no need to enter critical section)
    g_halAudUser = HAL_AUD_USER_NULL;

    return TRUE;
}

PUBLIC VOID hal_AudPowerOn(HAL_AUD_PD_T pdStatus)
{
#ifdef RECV_USE_SPK_LINE
    if (pdStatus.noDacPd)
    {
        if (g_halAudDacPrevPwr != HAL_AUD_DAC_PREV_PWR_RESTORED)
        {
            g_halAudDacPrevPwr = HAL_AUD_DAC_PREV_PWR_ON;
        }
    }
#endif // RECV_USE_SPK_LINE

    hwp_configRegs->audio_pd_set = pdStatus.reg;

#ifdef AUD_EXT_PA_ON_WITH_LCD
    if (pdStatus.noDacPd)
    {
        if (pmd_AudExtPaExists())
        {
            if (g_halAudExtPaPrevPwr != HAL_AUD_EXT_PA_PREV_PWR_RESTORED)
            {
                g_halAudExtPaPrevPwr = HAL_AUD_EXT_PA_PREV_PWR_ON;
            }
            // Do we need any stable time here?
        	pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, TRUE);
        }
    }
#endif // AUD_EXT_PA_ON_WITH_LCD
}

PUBLIC VOID hal_AudPowerDown(HAL_AUD_PD_T pdStatus)
{
#ifdef RECV_USE_SPK_LINE
    if (pdStatus.noDacPd)
    {
        // Audio might be stopped after LCD is turned off. In that case,
        // audio codec DAC can be powered off if requested.
        if (g_halAudDacPrevPwr == HAL_AUD_DAC_PREV_PWR_RESTORED)
        {
            // Not to direct-reg codec pd control
            hal_AbbRegWrite(CODEC_POWER_CTRL, ABB_RSTN_CODEC_DR|ABB_RSTN_CODEC|
                                              ABB_DAC_EN_CODEC_DR|ABB_DAC_EN_CODEC);
        }
        else
        {
            g_halAudDacPrevPwr = HAL_AUD_DAC_PREV_PWR_OFF;
            pdStatus.noDacPd = FALSE;
        }
    }
#endif // RECV_USE_SPK_LINE

    // Cannot go into deep sleep here
    pdStatus.noDeepPd = FALSE;
    pdStatus.noRefPd = FALSE;
#ifdef VMIC_POWER_ON_WORKAROUND
    if (g_halAudEarpieceDetectionOn == HAL_AUD_EP_DETECT_ON)
    {
        // Power on both Mic and AuxMic, then no matter which one
        // is selected in audio_sel_cfg, v_mic will be turned on.
        pdStatus.noMicPd = FALSE;
        pdStatus.noAuxMicPd = FALSE;
        pdStatus.noAdcPd = FALSE;
    }
#endif // VMIC_POWER_ON_WORKAROUND

    hwp_configRegs->audio_pd_clr = pdStatus.reg;

#ifdef AUD_EXT_PA_ON_WITH_LCD
    if (pdStatus.noDacPd)
    {
        if (pmd_AudExtPaExists())
        {
            // Audio might be stopped after LCD is turned off. In that case,
            // the external PA can be powered off if requested.
            if (g_halAudExtPaPrevPwr == HAL_AUD_EXT_PA_PREV_PWR_RESTORED)
            {
                // We will not go to deep sleep here
                sxr_Sleep(g_halAudIntSpkPaStableTime);
                pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, FALSE);
                sxr_Sleep(g_halAudExtPaStableTime);
            }
            else
            {
                g_halAudExtPaPrevPwr = HAL_AUD_EXT_PA_PREV_PWR_OFF;
            }
        }
    }
#endif // AUD_EXT_PA_ON_WITH_LCD
}

#if defined(RECV_USE_SPK_LINE) || defined(AUD_EXT_PA_ON_WITH_LCD)
PUBLIC VOID hal_AudLcdPowerOnCallback(VOID)
{
#ifdef RECV_USE_SPK_LINE
    // Power on audio codec DAC once LCD is turned on
    if (g_halAudDacPrevPwr == HAL_AUD_DAC_PREV_PWR_RESTORED)
    {
        HAL_AUD_PD_T pdStatus;
        pdStatus = hal_AudGetPowerStatus();
        if (pdStatus.noDacPd)
        {
            g_halAudDacPrevPwr = HAL_AUD_DAC_PREV_PWR_ON;
        }
        else
        {
            // Set g_halAudDacPrevPwr before calling hal_AudSetSelCfg(),
            // as hal_AudSetSelCfg() needs to check g_halAudDacPrevPwr
            g_halAudDacPrevPwr = HAL_AUD_DAC_PREV_PWR_OFF;

            HAL_AUD_SEL_T selCfg = hal_AudGetSelCfg();
            // Useless? selCfg.spkSel should have been set to zero
            selCfg.spkSel = FALSE;
            hal_AudSetSelCfg(selCfg);

            // Power on DAC (and its dependent power)
            pdStatus.reg = 0;
            pdStatus.noDeepPd = TRUE;
            pdStatus.noRefPd = TRUE;
            pdStatus.noDacPd = TRUE;
            hwp_configRegs->audio_pd_set = pdStatus.reg;
            // Direct-reg codec pd control
            hal_AbbRegWrite(CODEC_POWER_CTRL, ABB_PD_CODEC_DR|
                                              ABB_RSTN_CODEC_DR|ABB_RSTN_CODEC|
                                              ABB_DAC_EN_CODEC_DR|ABB_DAC_EN_CODEC);
        }
    }
#endif // RECV_USE_SPK_LINE

#ifdef AUD_EXT_PA_ON_WITH_LCD
    if (pmd_AudExtPaExists())
    {
        if (g_halAudExtPaPrevPwr == HAL_AUD_EXT_PA_PREV_PWR_RESTORED)
        {
            if (g_halAudUser != HAL_AUD_USER_CODEC &&
                g_halAudUser != HAL_AUD_USER_FM &&
                g_halAudUser != HAL_AUD_USER_LINEIN)
            {
                g_halAudExtPaPrevPwr = HAL_AUD_EXT_PA_PREV_PWR_OFF;
            }
            else
            {
                g_halAudExtPaPrevPwr = HAL_AUD_EXT_PA_PREV_PWR_ON;
            }
        }
        // Do we need any stable time here?
    	pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, TRUE);
    }
#endif // AUD_EXT_PA_ON_WITH_LCD
}

PUBLIC VOID hal_AudLcdPowerOffCallback(VOID)
{
#ifdef RECV_USE_SPK_LINE
    HAL_AUD_DAC_PREV_PWR_T orgDacPrevPwr = g_halAudDacPrevPwr;
    // Set g_halAudDacPrevPwr before calling hal_AudSetSelCfg(),
    // as hal_AudSetSelCfg() needs to check g_halAudDacPrevPwr
    g_halAudDacPrevPwr = HAL_AUD_DAC_PREV_PWR_RESTORED;

    // Restore the power state of audio codec DAC, or saying,
    // power off audio codec DAC if possible
    if (orgDacPrevPwr == HAL_AUD_DAC_PREV_PWR_OFF)
    {
        HAL_AUD_SEL_T selCfg = hal_AudGetSelCfg();
        selCfg.spkSel = FALSE;
        hal_AudSetSelCfg(selCfg);

        // Not to direct-reg codec pd control
        hal_AbbRegWrite(CODEC_POWER_CTRL, ABB_RSTN_CODEC_DR|ABB_RSTN_CODEC|
                                          ABB_DAC_EN_CODEC_DR|ABB_DAC_EN_CODEC);
        // Power off DAC
        HAL_AUD_PD_T pdStatus;
        pdStatus.reg = 0;
        pdStatus.noDacPd = TRUE;
        hwp_configRegs->audio_pd_clr = pdStatus.reg;
    }
#endif // RECV_USE_SPK_LINE

#ifdef AUD_EXT_PA_ON_WITH_LCD
    if (pmd_AudExtPaExists())
    {
        if (g_halAudExtPaPrevPwr == HAL_AUD_EXT_PA_PREV_PWR_OFF)
        {
    	    // Forbid deep sleep
    	    hal_SysRequestFreq(HAL_SYS_FREQ_AUDIO, HAL_SYS_FREQ_26M, NULL);
    		sxr_Sleep(g_halAudIntSpkPaStableTime);
            pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, FALSE);
    		sxr_Sleep(g_halAudExtPaStableTime);
    	    // Allow deep sleep from now on
    	    hal_SysRequestFreq(HAL_SYS_FREQ_AUDIO, HAL_SYS_FREQ_32K, NULL);
        }
        g_halAudExtPaPrevPwr = HAL_AUD_EXT_PA_PREV_PWR_RESTORED;
    }
#endif // AUD_EXT_PA_ON_WITH_LCD
}
#endif // RECV_USE_SPK_LINE || AUD_EXT_PA_ON_WITH_LCD

PUBLIC VOID hal_AudDeepPowerDown(VOID)
{
#ifdef RECV_USE_SPK_LINE
    // Not to direct-reg codec pd control
    hal_AbbRegWrite(CODEC_POWER_CTRL, ABB_RSTN_CODEC_DR|ABB_RSTN_CODEC|
                                      ABB_DAC_EN_CODEC_DR|ABB_DAC_EN_CODEC);
#endif // RECV_USE_SPK_LINE

#ifdef VMIC_POWER_ON_WORKAROUND
    if (g_halAudEarpieceDetectionOn == HAL_AUD_EP_DETECT_ON)
    {
        // Earpiece is on and do not power down audio
        return;
    }
#endif // VMIC_POWER_ON_WORKAROUND

    hwp_configRegs->audio_pd_clr = HAL_AUD_PD_MASK;
}

/// Must wait for AUD_TIME_SET_VCOM_MODE if setting to non-FM mode
PUBLIC VOID hal_AudSetVcomMode(BOOL isFmMode)
{
    if (isFmMode)
    {
        // FM mode (default mode)
        hal_AbbRegWrite(CODEC_LDO_SETTING2, 0x8800);
    }
    else
    {
        // Normal mode
        hal_AbbRegWrite(CODEC_LDO_SETTING2, 0x6800);
    }
}

PUBLIC VOID hal_AudSetVoiceMode(BOOL isVoiceMode)
{
    if (isVoiceMode)
    {
        hal_AbbRegWrite(0x29, 0x0f0c);
    }
    else
    {
        hal_AbbRegWrite(0x29, 0x0f04);
    }
}

PUBLIC VOID hal_AudSetSelCfg(HAL_AUD_SEL_T selCfg)
{
    // Set CODEC mode
    UINT32 codecSel = 0;
    // Save the orginal selection
    HAL_AUD_SEL_T orgSelCfg;
    orgSelCfg.reg = selCfg.reg;

    // LineIn config
    if (g_halAudUser == HAL_AUD_USER_LINEIN || g_halAudUser == HAL_AUD_USER_CLOSING)
    {
        UINT32 lineInSetting = 0x43;
        if (selCfg.headSel)
        {
            lineInSetting |= ABB_CALIB_LINEIN_HP;
        }
        if (selCfg.spkMonoSel || selCfg.spkSel)
        {
            lineInSetting |= ABB_CALIB_LINEIN_SPK;
        }

        hal_AbbRegWrite(CODEC_CALIB_SETTING, lineInSetting);
    }

#ifdef RECV_USE_SPK_LINE
    BOOL recvPaForceOn = FALSE;
    if (selCfg.headSel)
    {
        selCfg.spkSel = FALSE;
        selCfg.spkMonoSel = FALSE;
        selCfg.rcvSel = FALSE;
        codecSel |= ABB_PA_EN_HP;
    }
    else if (selCfg.spkSel || selCfg.spkMonoSel)
    {
        selCfg.spkSel = TRUE;
        selCfg.spkMonoSel = FALSE;
        selCfg.rcvSel = FALSE;
        if (g_halAudRecvUseSpkLineRight)
        {
            codecSel |= ABB_PA_EN_L_SPK;
        }
        else
        {
            codecSel |= ABB_PA_EN_R_SPK;
        }
    }
    else if (selCfg.rcvSel)
    {
        selCfg.spkSel = TRUE;
        selCfg.spkMonoSel = FALSE;
        selCfg.rcvSel = FALSE;
        if (g_halAudRecvUseSpkLineRight)
        {
            codecSel |= ABB_PA_EN_R_SPK;
        }
        else
        {
            codecSel |= ABB_PA_EN_L_SPK;
        }
    }
    else
    {
        // Select speaker output even in idle time, so that the capacitor conntected
        // to the receiver can be charged once LCD is turned on
        recvPaForceOn = TRUE;
        // If the LCD is off, there is no need to select speaker, for it
        // will be selected once LCD is turned on
        if (g_halAudDacPrevPwr != HAL_AUD_DAC_PREV_PWR_RESTORED)
        {
            selCfg.spkSel = TRUE;
            selCfg.spkMonoSel = FALSE;
            selCfg.rcvSel = FALSE;
        }
        // The CODEC_MODE_SEL still needs configuring, as this register
        // will not be changed when LCD is turned on
        if (g_halAudRecvUseSpkLineRight)
        {
            codecSel |= ABB_PA_EN_R_SPK;
        }
        else
        {
            codecSel |= ABB_PA_EN_L_SPK;
        }
    }
#endif // RECV_USE_SPK_LINE

    hwp_configRegs->audio_sel_cfg = selCfg.reg;

    if (selCfg.rcvSel || selCfg.spkSel || selCfg.spkMonoSel)
    {
#ifdef RECV_USE_SPK_LINE
        // If receiver PA is in forced-on status, there is no need to
        // switch on the audio source (speaker mode)
        if (!recvPaForceOn)
        {
            if (codecSel & ABB_PA_EN_R_SPK)
            {
                codecSel |= ABB_SPK_MODE1_R_SEL;
            }
            else
            {
                codecSel |= ABB_SPK_MODE1_L_SEL;
            }
        }
#else // !RECV_USE_SPK_LINE
        codecSel |= ABB_SPK_MODE1_L_SEL|ABB_SPK_MODE1_R_SEL;
#endif // !RECV_USE_SPK_LINE
        if (selCfg.fmSel)
        {
            codecSel |= ABB_SPK_FM_SEL;
        }
    }
    if (selCfg.headSel)
    {
        // Earpiece output energy is too large
        // Reduce ABB DAC digital gain to -15dB
        // DacGain[6:3] = 0 ... 15 = -15dB ... 0dB
        hal_AbbRegWrite(0x2a, 0xf000);

        codecSel |= ABB_HP_MODE1_L_SEL|ABB_HP_MODE1_R_SEL;
        if (selCfg.fmSel)
        {
            codecSel |= ABB_HP_FM_SEL;
        }
        else
        {
            codecSel |= ABB_HP_DAC_SEL;
        }
    }
    else
    {
#ifdef REDUCE_RECV_MIN_GAIN
        // Set ABB DAC digital gain
        if (g_halRecvReducedLevel >= 2)
        {
            // -9dB
            hal_AbbRegWrite(0x2a, 0xf030);
        }
        if (g_halRecvReducedLevel == 1)
        {
            // -6dB
            hal_AbbRegWrite(0x2a, 0xf041);
        }
        else // g_halRecvReducedLevel == 0
#endif // REDUCE_RECV_MIN_GAIN
        {
            // Restore ABB DAC digital gain
            // DacGain[6:3] = 0 ... 15 = -15dB ... 0dB
            if ((orgSelCfg.spkSel || orgSelCfg.spkMonoSel) && pmd_AudExtPaExists())
            {
                // -9dB for external audio PA to avoid distortion
                hal_AbbRegWrite(0x2a, 0xf030);
            }
            else
            {
                // -3dB
                hal_AbbRegWrite(0x2a, 0xf060);
            }
        }
    }

    hal_AbbRegWrite(CODEC_MODE_SEL, codecSel);

#ifdef RECV_USE_SPK_LINE
    // Direct-reg codec pd control
    hal_AbbRegWrite(CODEC_POWER_CTRL, ABB_PD_CODEC_DR|
                                      ABB_RSTN_CODEC_DR|ABB_RSTN_CODEC|
                                      ABB_DAC_EN_CODEC_DR|ABB_DAC_EN_CODEC);
#endif // RECV_USE_SPK_LINE
}

PUBLIC VOID hal_AudSetMicCfg(HAL_AUD_MIC_CFG_T micCfg)
{
    hwp_configRegs->audio_mic_cfg = micCfg.reg;
}

PUBLIC VOID hal_AudSetSpkCfg(HAL_AUD_SPK_CFG_T spkCfg)
{
    hwp_configRegs->audio_spk_cfg = spkCfg.reg;

    if (g_halAudUser == HAL_AUD_USER_LINEIN)
    {
        // TODO: Adjust LineIn gain on 8808
    }
}

PUBLIC VOID hal_AudSetRcvGain(UINT32 gain)
{
#ifdef RECV_USE_SPK_LINE
    HAL_AUD_SPK_CFG_T spkCfg;
    spkCfg.noSpkMute = TRUE;
    spkCfg.spkGain = gain;
    hwp_configRegs->audio_spk_cfg = spkCfg.reg;
#else // !RECV_USE_SPK_LINE
    hwp_configRegs->audio_rcv_gain = CFG_REGS_AU_RCV_GAIN(gain);
#endif // !RECV_USE_SPK_LINE
}

PUBLIC VOID hal_AudSetHeadGain(UINT32 gain)
{
    hwp_configRegs->audio_head_gain = CFG_REGS_AU_HEAD_GAIN(gain);

    if (g_halAudUser == HAL_AUD_USER_LINEIN)
    {
        // TODO: Adjust LineIn gain on 8808
    }
}

PUBLIC HAL_AUD_PD_T hal_AudGetPowerStatus(VOID)
{
    HAL_AUD_PD_T pdStatus;
    pdStatus.reg = hwp_configRegs->audio_pd_set;

#ifdef RECV_USE_SPK_LINE
    if (g_halAudDacPrevPwr == HAL_AUD_DAC_PREV_PWR_OFF)
    {
        pdStatus.noDacPd = FALSE;
    }
    else if (g_halAudDacPrevPwr == HAL_AUD_DAC_PREV_PWR_ON)
    {
        pdStatus.noDacPd = TRUE;
    }
#endif // RECV_USE_SPK_LINE

    return pdStatus;
}

PUBLIC HAL_AUD_SEL_T hal_AudGetSelCfg(VOID)
{
    HAL_AUD_SEL_T selCfg;
    selCfg.reg = hwp_configRegs->audio_sel_cfg;
    return selCfg;
}

PUBLIC HAL_AUD_MIC_CFG_T hal_AudGetMicCfg(VOID)
{
    HAL_AUD_MIC_CFG_T micCfg;
    micCfg.reg = hwp_configRegs->audio_mic_cfg;
    return micCfg;
}

PUBLIC HAL_AUD_SPK_CFG_T hal_AudGetSpkCfg(VOID)
{
    HAL_AUD_SPK_CFG_T spkCfg;
    spkCfg.reg = hwp_configRegs->audio_spk_cfg;
    return spkCfg;
}

PUBLIC UINT32 hal_AudGetRcvGain(VOID)
{
    return hwp_configRegs->audio_rcv_gain & CFG_REGS_AU_RCV_GAIN_MASK;
}

PUBLIC UINT32 hal_AudGetHeadGain(VOID)
{
    return hwp_configRegs->audio_head_gain & CFG_REGS_AU_HEAD_GAIN_MASK;
}


