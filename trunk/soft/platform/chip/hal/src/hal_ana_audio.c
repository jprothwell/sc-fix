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
//
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_ana_audio.c $
//  $Author: admin $
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $
//  $Revision: 269 $
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_ana_audio.c                                                     //
/// That file implements the driver offering access to the analog audio on 
/// on the chip.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#if (CHIP_HAS_ACCO_AUDIO)

#include "cs_types.h"
#include "chip_id.h"

#include "global_macros.h"
#include "ana_acco.h"
#include "tcu.h"
#include "cfg_regs.h"

#include "pmd_m.h"

#include "hal_sys.h"
#include "hal_ana_audio.h"
#include "hal_debug.h"

#include "halp_sys.h"
#include "halp_debug.h"
#include "halp_ana_audio.h"

// For timers
#include "cmn_defs.h"
#include "sxr_tim.h"


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================
// Use specific macros to access the registers that are renamed from one
// chip to the other
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
#define HAL_ANA_AUDIO_STEREO_DAC_REG                    hwp_extApb
#define HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(name)         REGS_ ## name
#else
#error "Unsupported CHIP_ASIC_ID for HAL Audio"
#endif

// =============================================================================
//  TYPES
// =============================================================================
typedef enum
{
    HAL_ANA_AUDIO_SPK,
    HAL_ANA_AUDIO_MIC,

    HAL_ANA_AUDIO_IO_QTY
} HAL_ANA_AUDIO_IO_T;



// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

/// Store the opened status for the speaker and mic
PRIVATE BOOL g_halAnaAudioIoOpened[HAL_ANA_AUDIO_IO_QTY]
      = {FALSE, FALSE};

/// Store the value of the selected mic
PRIVATE HAL_ANA_AUDIO_LOUD_SPEAKER_TYPE_T g_halAnaAudioLoudSpeakerType
      = HAL_ANA_AUDIO_LOUD_SPEAKER_STEREO;

/// Voice codec opened ?
PRIVATE BOOL g_halAnaAudioVoiceCodecOpened 
      = FALSE;

#if (CHIP_HAS_STEREO_DAC == 1)
/// Stereo DAC opened ?
PRIVATE BOOL g_halAnaAudioStereoDacOpened 
      = FALSE;

PRIVATE BOOL g_halAnaAudioVolumeLocked 
      = TRUE;

/// Target values for the analog and digital gains of the 
/// stereo macro, respectively.
PRIVATE UINT16 g_halAnaAudioVolumeTarget[3] 
      = {0, 0, 0};

/// Current values for the analog right and left, and digital gains of the 
/// stereo macro, respectively.
PRIVATE UINT16 g_halAnaAudioVolumeCurrent[3] 
      = {0, 0, 0};

/// User handler called when the configured volume target has been reached.
PRIVATE HAL_ANA_AUDIO_HANDLER_T g_halAnaAudioTargetVolumeHandler = NULL;
#endif


// =============================================================================
//  FUNCTIONS
// =============================================================================

#if (CHIP_HAS_STEREO_DAC == 1)
// =============================================================================
// hal_AnaAudioStereoTrackVolume
// -----------------------------------------------------------------------------
/// (This function is called in as a function timer.)
/// That function aims at setting the values set in the stereo DAC registers
/// to the target volume as silky smooth as possible, to avoid pop noises.
///
/// Current values of the gains are in g_halAnaAudioVolumeCurrent.
/// Target values are in g_halAnaAudioVolumeTarget.
// =============================================================================
PRIVATE VOID hal_AnaAudioStereoTrackVolume(VOID);
#endif



// =============================================================================
// hal_AnaAudioOpen
// -----------------------------------------------------------------------------
/// Opens the audio codec of the analog macro. This function must be called 
/// any other function of the driver can be used. Takes a resource.
/// This function handles the spk/mic management: the codec is only opened
/// once.
/// @param io Speaker or Mic, for which we want to open the codec.
// =============================================================================
PRIVATE VOID hal_AnaAudioOpen(HAL_ANA_AUDIO_IO_T io)
{
    UINT32 status;
    // Check we're no doing anything silly
    HAL_ASSERT(!g_halAnaAudioIoOpened[io], "Opening an already"
            "opened %d ...", io);

    // Warning: That only works with 2 ios ...:1-0 =1 et 1-1=0
    // Is the other one opened, in that case the codec has 
    // already been kicked off
    status = hal_SysEnterCriticalSection();
    // Set the io as opened.
    g_halAnaAudioIoOpened[io] = TRUE;

    if (g_halAnaAudioIoOpened[1-io])
    {
        // Codec already opened, nothing to see here for ya
    }
    else
    {
        // Open the codec

        // Configure the audio codec
        hwp_anaAccoFe->audio_ctrl_set = ANA_ACCO_AU_RESET_RELEASE
                                      | ANA_ACCO_AU_DEEP_PD_RELEASE
                                      | ANA_ACCO_AU_SD_ENABLE
                                      | ANA_ACCO_AU_DACA_ENABLE;

        // Take a resource
        hal_SysRequestFreq(HAL_SYS_FREQ_AUDIO, HAL_SYS_FREQ_26M, NULL);
       
        // Clocks are set by hal_SysSetupSystemClock
    }
    hal_SysExitCriticalSection(status);
}



// =============================================================================
// hal_AnaAudioClose
// -----------------------------------------------------------------------------
/// Closes the audio codec of the analog macro. Free a resource.
/// This function handles the spk/mic management: the codec is only closed
/// when nobody needs it.
/// @param io Speaker or Mic, for which we want to close the codec.
// =============================================================================
PRIVATE VOID hal_AnaAudioClose(HAL_ANA_AUDIO_IO_T io)
{
    UINT32 status;
    // Check we're no doing anything silly
    HAL_ASSERT(g_halAnaAudioIoOpened[io], "Closing a not"
            "opened %d ...", io);

    // Warning: That only works with 2 ios ...:1-0 =1 et 1-1=0
    // Is the other one opened, in that case the codec has 
    // to be close.
    status = hal_SysEnterCriticalSection();
    
    // Switch of the io
    if (io == HAL_ANA_AUDIO_SPK)
    {
        hwp_anaAccoFe->audio_spk_gain = 0;
    }
    else
    {
        // io == HAL_ANA_AUDIO_MIC
        hwp_anaAccoFe->audio_mic_gain = 0;
    }


    // Set the io as closed.
    g_halAnaAudioIoOpened[io] = FALSE;

    if (g_halAnaAudioIoOpened[1-io])
    {
        // The other io (mic or spk) is yet opened,
        // don't close the codec.
    }
    else
    {
        // Close the codec
        hwp_anaAccoFe->audio_ctrl_clr = ANA_ACCO_AU_RESET_N
                                  | ANA_ACCO_AU_DEEP_PD_N
                                  | ANA_ACCO_AU_SD_DISABLE
                                  | ANA_ACCO_AU_DACA_DISABLE;

        // free a resource
        hal_SysRequestFreq(HAL_SYS_FREQ_AUDIO, HAL_SYS_FREQ_32K, NULL);
    }
    hal_SysExitCriticalSection(status);    
}



// =============================================================================
// hal_AnaAudioVoiceSpkOpen
// -----------------------------------------------------------------------------
/// Opens and enables the speaker of the analog macro. This function shares a
/// resource with the #hal_AnaAudioMicOpen. This common resource is taken as
/// long as either one of them is opened.
// =============================================================================
PRIVATE VOID hal_AnaAudioVoiceSpkOpen(VOID)
{
    UINT32 status = hal_SysEnterCriticalSection();
    if (!g_halAnaAudioVoiceCodecOpened)
    {
        g_halAnaAudioVoiceCodecOpened = TRUE;
        hal_SysExitCriticalSection(status);
        HAL_TRACE(HAL_AUDIO_TRC, 0,"hal_AnaAudioVoiceSpkOpen");
        hal_AnaAudioOpen(HAL_ANA_AUDIO_SPK);
        hwp_anaAccoFe->audio_ctrl_set = ANA_ACCO_AU_SPK_ENABLE;
    }
    hal_SysExitCriticalSection(status);
}



// =============================================================================
// hal_AnaAudioVoiceSpkClose
// -----------------------------------------------------------------------------
/// Closes and disables the speaker of the analog macro. This function shares a
/// resource with the #hal_AnaAudioMicClose. This common resource is freed as
/// soon as both are closed.
// =============================================================================
PRIVATE VOID hal_AnaAudioVoiceSpkClose(VOID)
{
    UINT32 status = hal_SysEnterCriticalSection();
    if (g_halAnaAudioVoiceCodecOpened)
    {
        g_halAnaAudioVoiceCodecOpened = FALSE;
        HAL_TRACE(HAL_AUDIO_TRC, 0,"hal_AnaAudioVoiceSpkClose");
        hal_AnaAudioClose(HAL_ANA_AUDIO_SPK);
        hwp_anaAccoFe->audio_ctrl_clr = ANA_ACCO_AU_SPK_DISABLE;
    }
    hal_SysExitCriticalSection(status);
}



// =============================================================================
// hal_AnaAudioMicOpen
// -----------------------------------------------------------------------------
/// Opens and enables the speaker of the analog macro. This function shares a
/// resource with the #hal_AnaAudioVoiceSpkOpen. This common resource is taken as
/// long as either one of them is opened.
// =============================================================================
PUBLIC VOID hal_AnaAudioMicOpen(VOID)
{
    HAL_TRACE(HAL_AUDIO_TRC, 0,"hal_AnaAudioMicOpen");

    pmd_EnablePower(PMD_POWER_MIC, TRUE);
    hal_AnaAudioOpen(HAL_ANA_AUDIO_MIC);
    hwp_anaAccoFe->audio_ctrl_set = ANA_ACCO_AU_MIC_ENABLE;
}



// =============================================================================
// hal_AnaAudioMicClose
// -----------------------------------------------------------------------------
/// Closes and disables the speaker of the analog macro. This function shares a
/// resource with the #hal_AnaAudioVoiceSpkClose. This common resource is freed as
/// soon as both are closed.
// =============================================================================
PUBLIC VOID hal_AnaAudioMicClose(VOID)
{
    HAL_TRACE(HAL_AUDIO_TRC, 0,"hal_AnaAudioMicClose");
    hal_AnaAudioClose(HAL_ANA_AUDIO_MIC);
    hwp_anaAccoFe->audio_ctrl_clr = ANA_ACCO_AU_MIC_DISABLE;
    pmd_EnablePower(PMD_POWER_MIC, FALSE);
}


// =============================================================================
// hal_AnaAudioMicSel
// -----------------------------------------------------------------------------
/// Choose the microphone to use.
/// @param mic microphone to use.
// =============================================================================
PUBLIC VOID hal_AnaAudioMicSel(HAL_ANA_AUDIO_MIC_T mic)
{
    // TODO Implement the deactivation of the previous interface
    // before setting a new one
    UINT32 status = hal_SysEnterCriticalSection();
    switch (mic)
    {
        case HAL_ANA_AUDIO_MIC_RECEIVER:
            // TODO Create that value in the XML describing this register
            hwp_anaAccoFe->audio_sel = (hwp_anaAccoFe->audio_sel & ~ANA_ACCO_AU_MIC_SEL)
                            | (0<<ANA_ACCO_AU_MIC_SEL_SHIFT);
            break;
        
        case HAL_ANA_AUDIO_MIC_EAR_PIECE:
            // TODO Create that value in the XML describing this register
            hwp_anaAccoFe->audio_sel = (hwp_anaAccoFe->audio_sel & ~ANA_ACCO_AU_MIC_SEL)
                            | (1<<ANA_ACCO_AU_MIC_SEL_SHIFT);
            break;

        case HAL_ANA_AUDIO_MIC_LOUD_SPEAKER:
            // TODO Create that value in the XML describing this register
            hwp_anaAccoFe->audio_sel = (hwp_anaAccoFe->audio_sel & ~ANA_ACCO_AU_MIC_SEL)
                            | (0<<ANA_ACCO_AU_MIC_SEL_SHIFT);
            break;

        default:
            HAL_ASSERT(FALSE, "Invalid mic set %d for the Analog Audio", mic);
    }
    
    // Store the set value
    hal_SysExitCriticalSection(status);
}




// =============================================================================
// hal_AnaAudioMicSetGain
// -----------------------------------------------------------------------------
/// This function is used to set the gain of the microphone.
///
/// @param mute If \c TRUE, the microphone is muted, whatever the other parameters
/// are.
/// @param analogGain Analog gain for the microphone.
/// @param digitalGain Digital gain for the microphone, limited to the range
/// 0 (+0dB) to 3 (+18dB).
// =============================================================================
PUBLIC VOID hal_AnaAudioMicSetGain(BOOL mute, 
        HAL_ANA_AUDIO_MIC_ANALOG_GAIN_T analogGain,
        HAL_ANA_AUDIO_DIGITAL_GAIN_T digitalGain)
{
    hwp_anaAccoFe->audio_mic_gain = (mute?ANA_ACCO_AU_MIC_MUTE_MUTE:ANA_ACCO_AU_MIC_MUTE_UNMUTE)
                | ANA_ACCO_AU_MIC_ANA_GAIN(analogGain)
                | ANA_ACCO_AU_MIC_DIG_GAIN(digitalGain);
}


// =============================================================================
// hal_AnaAudioSpkSetGain
// -----------------------------------------------------------------------------
/// This function is used to set the gain of the speaker.
///
/// @param spkConfig Configuration of the speaker. The different gains to set
/// are got from the fields of that structure.
/// @param handler If not \c NULL, user handler called when the volume has
/// been reached.
// =============================================================================
PRIVATE VOID hal_AnaAudioSpkSetGain(HAL_ANA_AUDIO_SPK_CONFIG_T* spkConfig,
                                    HAL_ANA_AUDIO_HANDLER_T     handler)
{
    if (spkConfig->mute)
    {
        hwp_anaAccoFe->audio_spk_gain = ANA_ACCO_AU_SPK_MUTE_MUTE;
#if CHIP_HAS_STEREO_DAC == 1
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_HP_GAIN_L = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLVOL_MUTE) | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLVU);
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_HP_GAIN_R = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRVOL_MUTE) | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRVU);
#endif
        // Shutdown speakers to avoid blowing wind noises
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_CTRL_SET = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLPD_POWER_DOWN) 
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRPD_POWER_DOWN);
#endif
    }
    else
    {
#if (HAL_SOUND_LIMITER == 1)
        hwp_anaAccoFe->audio_spk_gain = ANA_ACCO_AU_SPK_MUTE_UNMUTE
            | ANA_ACCO_AU_SPK_ANA_GAIN(
                spkConfig->voiceAnalogGain > HAL_ANA_AUDIO_SPK_ANALOG_GAIN_0dB?
                HAL_ANA_AUDIO_SPK_ANALOG_GAIN_0dB:spkConfig->voiceAnalogGain)
            | ANA_ACCO_AU_SPK_DIG_GAIN(
                spkConfig->digitalGain > HAL_ANA_AUDIO_DIGITAL_GAIN_0dB?
                HAL_ANA_AUDIO_DIGITAL_GAIN_0dB:spkConfig->digitalGain);
#else
        hwp_anaAccoFe->audio_spk_gain = ANA_ACCO_AU_SPK_MUTE_UNMUTE
                            | ANA_ACCO_AU_SPK_ANA_GAIN(spkConfig->voiceAnalogGain)
                            | ANA_ACCO_AU_SPK_DIG_GAIN(spkConfig->digitalGain);
#endif


#if CHIP_HAS_STEREO_DAC == 1
        UINT32 status;
        // Whenever there is a stereo DAC, we can apply the pream value for the 
        // audio coming from the `voice` ol' acco interface.
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_MIXER_HP_GAIN = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VOI2HPVOL)(spkConfig->preampGain);

        // Set the volume target, and unlock the tracker if needed
        status = hal_SysEnterCriticalSection();
#if (HAL_SOUND_LIMITER == 1)
        g_halAnaAudioVolumeTarget[0] =
            (spkConfig->stereoAnalogRightGain > HAL_ANA_AUDIO_SPK_STEREO_GAIN_m1_5dB)?
            HAL_ANA_AUDIO_SPK_STEREO_GAIN_m1_5dB:spkConfig->stereoAnalogRightGain;
        g_halAnaAudioVolumeTarget[1] =
            (spkConfig->stereoAnalogLeftGain > HAL_ANA_AUDIO_SPK_STEREO_GAIN_m1_5dB)?
            HAL_ANA_AUDIO_SPK_STEREO_GAIN_m1_5dB:spkConfig->stereoAnalogLeftGain;
        g_halAnaAudioVolumeTarget[2] =
            (spkConfig->digitalGain > HAL_ANA_AUDIO_DIGITAL_GAIN_0dB)?
            HAL_ANA_AUDIO_DIGITAL_GAIN_0dB:spkConfig->digitalGain;
#else
        g_halAnaAudioVolumeTarget[0] = spkConfig->stereoAnalogRightGain;
        g_halAnaAudioVolumeTarget[1] = spkConfig->stereoAnalogLeftGain;
        g_halAnaAudioVolumeTarget[2] = spkConfig->digitalGain;
#endif
        // Set volume handler.
        g_halAnaAudioTargetVolumeHandler = handler;
        HAL_TRACE(HAL_AUDIO_TRC, 0, "Set Target %d,  %d @%#x", g_halAnaAudioVolumeTarget[0],
                            g_halAnaAudioVolumeTarget[1], hal_TimGetUpTime());
        HAL_TRACE(HAL_AUDIO_TRC, 0, "Current val %d,  %d @%#x", g_halAnaAudioVolumeCurrent[0],
                            g_halAnaAudioVolumeCurrent[1], hal_TimGetUpTime());
        if (g_halAnaAudioVolumeLocked)
        {
            // Unlock the tracker
            g_halAnaAudioVolumeLocked = FALSE;
            hal_AnaAudioStereoTrackVolume();
        }
        // Else the tracker will take the new target into account.
        hal_SysExitCriticalSection(status);

        

        // Enable speaker
        if (spkConfig->spkSel != HAL_ANA_AUDIO_SPK_RECEIVER)
        {
            HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_CTRL_CLR = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLPD_POWER_DOWN) 
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRPD_POWER_DOWN);   
 
     
            HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_ANA_CTRL = HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_ANA_CTRL
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLZC)
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRZC);
        }

#else
    // There is no volume tracker, so call the handler directly here
    if (handler != NULL)
    {
        handler();
    }
#endif
    }
}

#if CHIP_HAS_STEREO_DAC == 1


// =============================================================================
// hal_AnaAudioStereoSetGainRegisters
// -----------------------------------------------------------------------------
/// Configure the gain register of the stereo macro.
/// @param analogRightGain  Analog Right gain to set.
/// @param analogLeftGain   Analog Left gain to set.
/// @param digitalGain      Digital gain to set.
// =============================================================================
PRIVATE VOID hal_AnaAudioStereoSetGainRegisters(UINT32 analogRightGain, UINT32 analogLeftGain, UINT32 digitalGain)
{
    UINT32 temp;
    // Analog gains
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_HP_GAIN_L = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLVOL)(analogLeftGain) | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLVU);
    HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_HP_GAIN_R = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRVOL)(analogRightGain) | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRVU);
#endif
    
    // Digital gains
    // Use the intermediate value to avoid the writing 
    // of 0 as a digital gain value.
    temp = HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL &
        (~HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACIL_SHIFT_MASK) & ~HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACIR_SHIFT_MASK));
    HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL = temp | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACIL_SHIFT)(digitalGain)
                                           | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACIR_SHIFT)(digitalGain);
}

// =============================================================================
// hal_AnaAudioStereoTrackVolume
// -----------------------------------------------------------------------------
/// That function aims at setting the values set in the stereo DAC registers
/// to the target volume as silky smooth as possible, to avoid pop noises.
///
/// Current values of the gains are in g_halAnaAudioVolumeCurrent.
/// Target values are in g_halAnaAudioVolumeTarget.
// =============================================================================
PRIVATE VOID hal_AnaAudioStereoTrackVolume(VOID)
{
    HAL_PROFILE_FUNCTION_ENTER(hal_AnaAudioStereoTrackVolume);
    UINT32 status;
    HAL_ANA_AUDIO_HANDLER_T handlerToCall = NULL;

    // Update digital gain before the  analog gain
    if (g_halAnaAudioVolumeCurrent[2] != g_halAnaAudioVolumeTarget[2])
    {
       // Update analog gain of one step.
       if (g_halAnaAudioVolumeCurrent[2] > g_halAnaAudioVolumeTarget[2])
       {
           g_halAnaAudioVolumeCurrent[2] -= 1;
       }
       else
       {
           g_halAnaAudioVolumeCurrent[2] += 1;
       }
    }
    else
    {
        // Update analog right gain only if digital gain is on target
        if (g_halAnaAudioVolumeCurrent[0] != g_halAnaAudioVolumeTarget[0])
        {
            // Update analog gain of one step.
            if (g_halAnaAudioVolumeCurrent[0] > g_halAnaAudioVolumeTarget[0])
            {
                g_halAnaAudioVolumeCurrent[0] -= 1;
            }
            else
            {
                g_halAnaAudioVolumeCurrent[0] += 1;
            }
        }
        
        // Update analog left gain only if digital gain is on target
        if (g_halAnaAudioVolumeCurrent[1] != g_halAnaAudioVolumeTarget[1])
        {
            // Update analog gain of one step.
            if (g_halAnaAudioVolumeCurrent[1] > g_halAnaAudioVolumeTarget[1])
            {
                g_halAnaAudioVolumeCurrent[1] -= 1;
            }
            else
            {
                g_halAnaAudioVolumeCurrent[1] += 1;
            }
        }
    }

    // Update registers
    HAL_TRACE(HAL_AUDIO_TRC, 0, "Set Current val to %d,  %d, %d@%#x", g_halAnaAudioVolumeCurrent[0],
                            g_halAnaAudioVolumeCurrent[1], g_halAnaAudioVolumeCurrent[2], hal_TimGetUpTime());
    hal_AnaAudioStereoSetGainRegisters(g_halAnaAudioVolumeCurrent[0],
                                       g_halAnaAudioVolumeCurrent[1],
                                       g_halAnaAudioVolumeCurrent[2]);
    
    // Check if target is reached.
    status = hal_SysEnterCriticalSection();
    if ((g_halAnaAudioVolumeCurrent[0] == g_halAnaAudioVolumeTarget[0]) 
      && (g_halAnaAudioVolumeCurrent[1] == g_halAnaAudioVolumeTarget[1])
      && (g_halAnaAudioVolumeCurrent[2] == g_halAnaAudioVolumeTarget[2]))
    {
        // Volume reached. Save user handler to call it outside the critical
        // section.
        g_halAnaAudioVolumeLocked           = TRUE;
        handlerToCall                       = g_halAnaAudioTargetVolumeHandler;
        g_halAnaAudioTargetVolumeHandler    = NULL;
    }
    else
    {
        // Start the respawn for some ms later to adjust the remaining
        // differences between current and targeted volume.
        sxr_StartFunctionTimer(20 MILLI_SECONDS, hal_AnaAudioStereoTrackVolume, 0, 0);
    }
    hal_SysExitCriticalSection(status);

    // Call the user volume handler, if any
    if (handlerToCall)
    {
        handlerToCall();
    }
    HAL_PROFILE_FUNCTION_EXIT(hal_AnaAudioStereoTrackVolume);

}





// =============================================================================
// hal_AnaAudioStereoDacOpen
// -----------------------------------------------------------------------------
/// Open the stereo DAC.
/// @param freq Sample rate of the stream to play through the DAC.
/// @param voiceOnly TRUE if, and only if, the stereo DAC is only used
/// as a preamplifier of the analog voice codec, when it is output on the
/// ear-piece or the loudspeaker interface.
/// @param lsType Type of loudpspeaker plugged on the stereo interface. This 
/// parameter is ignored in voice mode, as the voice stream is send to both
/// headphone outputs.
// =============================================================================
PRIVATE VOID hal_AnaAudioStereoDacOpen(HAL_AIF_FREQ_T freq, BOOL voiceOnly,
                                       HAL_ANA_AUDIO_LOUD_SPEAKER_TYPE_T lsType)
{
    UINT32 mixing;
    UINT32 status = hal_SysEnterCriticalSection();
   
 
 
    if (!g_halAnaAudioStereoDacOpened)
    {
        g_halAnaAudioStereoDacOpened = TRUE;

        // Set volume to 0
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_ANA_CTRL = 0;
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_MIXER_HP_GAIN = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VOI2HPVOL)(7);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_HP_GAIN_L = 0 | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLVU);
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_HP_GAIN_R = 0 | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRVU);
#endif

        // power up the stereo dac LDO
        pmd_EnablePower(PMD_POWER_STEREO_DAC, TRUE);

        // Take resource
        hal_SysRequestFreq(HAL_SYS_FREQ_STEREO, HAL_SYS_FREQ_26M, NULL);
    }
    hal_SysExitCriticalSection(status);
        
    // TODO Add mono/stereo handling here? Handled by the hal_AifOpen 
    // currently.
    HAL_TRACE(HAL_AUDIO_TRC, 0,"Stereo DAC open: freq:%d, voiceOnly:%d, loudspeaker:%d", freq, voiceOnly, lsType);
    switch (lsType)
    {
        case HAL_ANA_AUDIO_LOUD_SPEAKER_STEREO:
            mixing = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_LDA2HPL_ENABLE) | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_RDA2HPR_ENABLE);
            break;

        case HAL_ANA_AUDIO_LOUD_SPEAKER_MONO_LEFT:
            mixing = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_LDA2HPL_ENABLE) | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_RDA2HPL_ENABLE);
            break;

        default:
            // Mono right
            mixing = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_LDA2HPR_ENABLE) | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_RDA2HPR_ENABLE);
            break;
    }


    if (voiceOnly == FALSE)
    {    
        HAL_TRACE(HAL_AUDIO_TRC, 0,"AUDIO Stereo DAC open (not voice): freq:%d, voiceOnly:%d, loudspeaker:%d", freq, voiceOnly, lsType);
        
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_CTRL_CLR = 0
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_RST_H_RESET)
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VREFPD_POWER_DOWN) 
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_IBPD_POWER_DOWN) 
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACPDL_POWER_DOWN) 
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACPDR_POWER_DOWN) 
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLKENB_L_DISABLE);

        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_ANA_CTRL = mixing
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CAPLESS_ENABLE)
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VOI2HP_ENABLE)
                // There is sometimes a noise when the zero crossing 
                // is enable at the opening before the opening of the 
                // DAC. When it as already been opened, the set gain 
                // function have set the zero-crossing in this register,
                // so we keep them. The mixing bits are the only one
                // that could change, so we clear them before.
                                        | (HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_ANA_CTRL &
                                                ~(HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_LDA2HPL_ENABLE)|HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_RDA2HPL_ENABLE)|HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_LDA2HPR_ENABLE)|HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_RDA2HPR_ENABLE)));
                   
        // Clear the bitfields that are frequency dependent before
        // setting them.
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL &=
            ~(HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_MASK)
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_MASK)
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_MASK)
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_MASK)
#endif
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV_MASK)
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACINV_L));

        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACINV_R); 

        switch (freq)
        {
            case HAL_AIF_FREQ_48000HZ:
                HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_OSR_125) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_NO_MUTE) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_NO_MUTE) 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_12_MHZ)
#endif 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV)(0);        
                break;

            case  HAL_AIF_FREQ_44100HZ:
                HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_OSR_135) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_NO_MUTE) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_NO_MUTE) 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_12_MHZ)
#endif 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV)(0);        
                break;
                
            case  HAL_AIF_FREQ_32000HZ:
                HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_OSR_125) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_NO_MUTE) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_NO_MUTE) 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_12_MHZ)
#endif 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV)(1);        
                break;

            case  HAL_AIF_FREQ_24000HZ:
                HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_OSR_125) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_NO_MUTE) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_NO_MUTE) 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_12_MHZ)
#endif 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV)(2);        
                break;

            case  HAL_AIF_FREQ_22050HZ:
                HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_OSR_135) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_NO_MUTE) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_NO_MUTE) 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_12_MHZ)
#endif 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV)(2);        
                break;

            case  HAL_AIF_FREQ_16000HZ:
                HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_OSR_125) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_NO_MUTE) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_NO_MUTE) 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_12_MHZ)
#endif 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV)(4);
                break;

            case  HAL_AIF_FREQ_12000HZ:
                HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_OSR_125) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_NO_MUTE) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_NO_MUTE) 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_12_MHZ)
#endif 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV)(6); 
                break;

            case  HAL_AIF_FREQ_11025HZ:
                HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_OSR_135) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_NO_MUTE) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_NO_MUTE) 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_12_MHZ)
#endif 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV)(6);        
                break;

            case  HAL_AIF_FREQ_8000HZ: 
                HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_DIG_CTRL |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACFLT_OSR_125) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_L_NO_MUTE) 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACMU_R_NO_MUTE) 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_SRC_SEL_12_MHZ)
#endif 
                                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLK_DIV)(10);        
                break;
        }
    }
    else
    {
        HAL_TRACE(HAL_AUDIO_TRC, 0,"AUDIO Stereo DAC open (voice !!!): freq:%d, voiceOnly:%d, loudspeaker:%d", freq, voiceOnly, lsType);
        // Voice  only == TRUE
        // Only support stream @8Khz
        HAL_ASSERT(freq == HAL_AIF_FREQ_8000HZ, "Unsupported frequency to go from the analog voice code"
                "through the Stereo DAC");
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_ANA_CTRL = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VOI2HP_ENABLE)
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLZC)
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRZC);

        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_CTRL_CLR = 0
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VREFPD_POWER_DOWN) 
                                        | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_IBPD_POWER_DOWN);
        //                                | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_RST_H_RESET);
    }    
        
}


// =============================================================================
// hal_AnaAudioStereoDacPreOpen
// -----------------------------------------------------------------------------
/// Preopen the Stereo DAC. This is a dirty thing needed by some shortcoming 
/// of the stereo DAC. It is needed to preopen the stereo DAC several seconds
/// before being able to use, otherwise, a loud pop noise can be heard.
/// Without the preopening, the output signal sent to the speaker may experience
/// a step down of one Volt, resulting in a unpleasant noise.
/// The bits set in #hal_AnaAudioStereoDacPreOpen must not be cleared, otherwise
/// the output signal will slowly decrease, and lead to a pop noise at the next
/// opening.
///
// =============================================================================
PROTECTED VOID hal_AnaAudioStereoDacPreOpen(VOID)
{
    HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_CTRL_CLR = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VMIDPD_POWER_DOWN);
    HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_REF_GEN |= HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VMIDSEL)(1);
}
#endif


// =============================================================================
// hal_AnaAudioStereoDacClose
// -----------------------------------------------------------------------------
/// Closes the Stereo DAC.
// =============================================================================
PRIVATE VOID hal_AnaAudioStereoDacClose(VOID)
{
#if (CHIP_HAS_STEREO_DAC == 1)
    HAL_TRACE(HAL_AUDIO_TRC, 0,"Stereo DAC Close");
    UINT32 status = hal_SysEnterCriticalSection();

   
    if (g_halAnaAudioStereoDacOpened)
    {
        g_halAnaAudioStereoDacOpened = FALSE;
        HAL_TRACE(HAL_AUDIO_TRC, 0,"Stereo DAC Close, and it was opened !");

        // Cut volume down
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_ANA_CTRL = 0;
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_MIXER_HP_GAIN = HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VOI2HPVOL)(7);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_HP_GAIN_L = 0 | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLVU);
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_HP_GAIN_R = 0 | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRVU);
#endif

        // Power down the stereo dac LDO.
        // This is not done anymore as cutting the power of the Stero DAC will
        // generate a pop noise. So this LDO is on all the time, except when the
        // phone goes to low power: when the PMIC goes to low power, the power
        // of the stereo DAC must be turned off.
        //pmd_EnablePower(PMD_POWER_STEREO_DAC, FALSE);

        // Disable VMID for off mode.
        // Removed to fix the start pop noise
        //HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_REF_GEN &= ~HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VMIDSEL_MASK);
            
        // Disable the analog part.
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_ANA_CTRL = 0;
        
        // Put everything in powerdown and reset.
        HAL_ANA_AUDIO_STEREO_DAC_REG->STEREO_DAC_CTRL_SET =
            HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPLPD_POWER_DOWN) 
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_HPRPD_POWER_DOWN) 
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_VREFPD_POWER_DOWN) 
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_IBPD_POWER_DOWN) 
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACPDL_POWER_DOWN) 
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_DACPDR_POWER_DOWN) 
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_RST_H_RESET) 
            | HAL_ANA_AUDIO_STEREO_DAC_BITFIELD(AU_CLKENB_L_DISABLE);

        // Free resource
        hal_SysRequestFreq(HAL_SYS_FREQ_STEREO, HAL_SYS_FREQ_32K, NULL);
    }
    
    hal_SysExitCriticalSection(status);

#endif
}



// =============================================================================
// hal_AnaAudioSpkConfig
// -----------------------------------------------------------------------------
/// Choose the speaker to use and configure the gain. The gain will be smoothly 
/// adjusted following an internal ramp, and if specified, \c handler will be
/// called once the desired level has been reached.
///
/// If this function is called several times before the volume gains have reached
/// the desired value, the targeted volume will be updated to the latest
/// requested and only the latest registered, if any, handler will be called.
/// 
/// @param spkCfg Speaker configuration to use.
/// @param handler Handler to call  when volume has been reached, if not
/// \c NULL.
// =============================================================================
PUBLIC VOID hal_AnaAudioSpkConfig(HAL_ANA_AUDIO_SPK_CONFIG_T*   spkCfg,
                                  HAL_ANA_AUDIO_HANDLER_T       handler)
{

    HAL_TRACE(HAL_AUDIO_TRC, 0,"AUDIO open: freq:%d, voiceOnly:%d", spkCfg->freq, spkCfg->voiceQuality);

    // The handler is going to be passed to the volume tracker called in a function
    // timer. As we clearly intend to change the volume configuration (We're in 
    // the process of calling #hal_AnaAudioSpkConfig ...), we don't want to be 
    // kept in a bastard state where the old handler is called instead of the new
    // one.
    sxr_StopFunctionTimer(hal_AnaAudioStereoTrackVolume);
    // Consider gains are locked and need to be tracked again.
    g_halAnaAudioVolumeLocked = TRUE;

    switch (spkCfg->spkSel)
    {
        case HAL_ANA_AUDIO_SPK_RECEIVER:
            // Only the 8k voiceQuality (mono ?) stream can be output
            // there
            HAL_ASSERT((spkCfg->voiceQuality == TRUE)
                    && (spkCfg->freq == HAL_AIF_FREQ_8000HZ),
                    "Analog receiver can only output 8kHz voice quality"
                    "streams");
            
            // Open voice codec and chose output.
            hal_AnaAudioVoiceSpkOpen();
            hwp_anaAccoFe->audio_sel = (hwp_anaAccoFe->audio_sel & ~ANA_ACCO_AU_SPK_SEL)
                            | (0<<ANA_ACCO_AU_SPK_SEL_SHIFT);
#if (CHIP_HAS_STEREO_DAC == 1)
            // Close stereo codec
            hal_AnaAudioStereoDacClose();
#endif
            //pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, FALSE);
            break;

        case HAL_ANA_AUDIO_SPK_EAR_PIECE:
            if (spkCfg->voiceQuality == TRUE)
            {
                HAL_ASSERT(spkCfg->freq == HAL_AIF_FREQ_8000HZ,
                    "Analog voice codec (@voice quality) only supports 8kHz streams");
                // Open voice codec
                hal_AnaAudioVoiceSpkOpen();
                hwp_anaAccoFe->audio_sel = (hwp_anaAccoFe->audio_sel & ~ANA_ACCO_AU_SPK_SEL)
                                | (1<<ANA_ACCO_AU_SPK_SEL_SHIFT);
            
#if (CHIP_HAS_STEREO_DAC == 1)
                // Open stereo codec to go through
                hal_AnaAudioStereoDacOpen(HAL_AIF_FREQ_8000HZ, TRUE, HAL_ANA_AUDIO_LOUD_SPEAKER_STEREO);
#endif
            }
            else
            {
#if (CHIP_HAS_STEREO_DAC == 1)
                // Open stereo codec
                hal_AnaAudioStereoDacOpen(spkCfg->freq, FALSE, HAL_ANA_AUDIO_LOUD_SPEAKER_STEREO);
#endif
                // Close the voice codec
                hal_AnaAudioVoiceSpkClose();
            }
            //pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, FALSE);
            break;
        
        case HAL_ANA_AUDIO_SPK_LOUD_SPEAKER:
            if (spkCfg->voiceQuality == TRUE)
            {
                HAL_ASSERT(spkCfg->freq == HAL_AIF_FREQ_8000HZ,
                    "Analog voice codec (@voice quality) only supports 8kHz streams");
                // Open voice codec
                hal_AnaAudioVoiceSpkOpen();
                hwp_anaAccoFe->audio_sel = (hwp_anaAccoFe->audio_sel & ~ANA_ACCO_AU_SPK_SEL)
                                | (1<<ANA_ACCO_AU_SPK_SEL_SHIFT);
#if (CHIP_HAS_STEREO_DAC == 1)
                // Open stereo codec to go through
                hal_AnaAudioStereoDacOpen(HAL_AIF_FREQ_8000HZ, TRUE, g_halAnaAudioLoudSpeakerType);
#endif
            }
            else
            {
#if (CHIP_HAS_STEREO_DAC == 1)
                // Open stereo codec
                hal_AnaAudioStereoDacOpen(spkCfg->freq, FALSE, g_halAnaAudioLoudSpeakerType);
#endif
                // Close the voice codec
                hal_AnaAudioVoiceSpkClose();
            }
            //pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, TRUE);
            break;

        default:
            HAL_ASSERT(FALSE, "Invalid speaker selection for "
                              "hal_AnaAudioSpkConfig: %d",
                               spkCfg->spkSel);
    }
    // Set gains
    hal_AnaAudioSpkSetGain(spkCfg, handler);
}



// =============================================================================
// hal_AnaAudioSpkOpen
// -----------------------------------------------------------------------------
/// Opens and enables the speaker of the analog macro. This function shares a
/// resource with the #hal_AnaAudioMicOpen. This common resource is taken to
/// #HAL_SYS_FREQ_26M as long as either one of them is opened.
/// @param spkConfig Configuration to apply to the opened speaker (selection,
/// gain, etc ...)
/// @param lsType Loudspeaker plugging scheme (stereo, mono on left channel, 
/// etc ...)
// =============================================================================
PUBLIC VOID hal_AnaAudioSpkOpen(HAL_ANA_AUDIO_SPK_CONFIG_T* spkConfig,
                                HAL_ANA_AUDIO_LOUD_SPEAKER_TYPE_T lsType)
{
    // Reinitialize audio tracking scheme
#if (CHIP_HAS_STEREO_DAC == 1)
    g_halAnaAudioVolumeLocked = TRUE;
    g_halAnaAudioVolumeTarget[0] = 0; 
    g_halAnaAudioVolumeTarget[1] = 0; 
    g_halAnaAudioVolumeCurrent[0] = 0;
    g_halAnaAudioVolumeCurrent[1] = 0;
#endif

    HAL_TRACE(HAL_AUDIO_TRC, 0, "AUDIO: <<< hal_AnaAudioSpkOpen");
    g_halAnaAudioLoudSpeakerType = lsType;
    hal_AnaAudioSpkConfig(spkConfig, NULL);
}




// =============================================================================
// hal_AnaAudioSpkClose
// -----------------------------------------------------------------------------
/// Closes and disables the speaker of the analog macro. This function shares a
/// resource with the #hal_AnaAudioMicClose. This common resource is freed to
/// #HAL_SYS_FREQ_32K as soon as both are closed.
// =============================================================================
PUBLIC VOID hal_AnaAudioSpkClose(VOID)
{
    HAL_TRACE(HAL_AUDIO_TRC, 0, "AUDIO: hal_AnaAudioSpkClose >>>");
    hal_AnaAudioStereoDacClose();
    hal_AnaAudioVoiceSpkClose(); 



}










#endif // CHIP_HAS_ACCO_AUDIO


