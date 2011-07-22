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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/aud/codec_gallite/src/aud_codec_common.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2011-07-12 11:53:33 +0800 (Tue, 12 Jul 2011) $                     //   
//	$Revision: 8975 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file aud_codec_common.c                                                        //
/// That file provides the implementation of the AUD common interface for the Codec //
//  chip.                                                                     //
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================

#include "cs_types.h"

#include "aud_codec_common.h"
#include "audp_debug.h"

#include "hal_aif.h"
#include "hal_error.h"
#include "hal_sys.h"

#include "hal_rda_abb.h"
#include "hal_rda_audio.h"
#include "hal_ispi.h"
#include "pmd_m.h"
#include "sxr_tls.h"
#include "calib_m.h"

#include "tgt_aud_cfg.h"


// aud_StreamStart() and aud_StreamStop(), aud_Setup() are of mutually exclusive access,
// with the help of semaphore. Then here we do not need mutex any more.


// =============================================================================
//  MACROS
// =============================================================================

#if defined(_FLASH_PROGRAMMER) || defined(_T_UPGRADE_PROGRAMMER)
#undef SPI_REG_DEBUG
#endif

// Define VCOM_BAT_LARGE_CAP to 1 if there is no external loud speaker and
// the capacitor connected to VCOM_BAT is larger than 1uF
#define VCOM_BAT_LARGE_CAP 0

#define FREQ_SAMPLE_SEL_8K (0x0080)
#define FREQ_SAMPLE_DIV_8K (0x0659)

#define FREQ_SAMPLE_SEL_11_025K (0x5087)
#define FREQ_SAMPLE_DIV_11_025K (0x049a)

#define FREQ_SAMPLE_SEL_12K (0x5087)
#define FREQ_SAMPLE_DIV_12K (0x043b)

#define FREQ_SAMPLE_SEL_16K (0x8084)
#define FREQ_SAMPLE_DIV_16K (0x032c)

#define FREQ_SAMPLE_SEL_22_05K (0x5085)
#define FREQ_SAMPLE_DIV_22_05K (0x024d)

#define FREQ_SAMPLE_SEL_24K (0x6086)
#define FREQ_SAMPLE_DIV_24K (0x021d)

#define FREQ_SAMPLE_SEL_32K (0x4081)
#define FREQ_SAMPLE_DIV_32K (0x0196)

#define FREQ_SAMPLE_SEL_44_1K (0x6082)
#define FREQ_SAMPLE_DIV_44_1K (0x0126)

#define FREQ_SAMPLE_SEL_48K (0xd083)
#define FREQ_SAMPLE_DIV_48K (0x010e)


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// AUD_CODEC_CALIB_OUT_GAINS_T
// -----------------------------------------------------------------------------
/// This type describes how the gains are recorded in the audio calibration
/// structure for the Analog AUD driver.
// =============================================================================
typedef union
{
    /// The value read from the calibration structure.
    UINT32  calibValue;
        
    /// Calibrated gains.
    struct 
    {
        /// Calibrated gains for a voice flow.
        /// Right gain on the stereo macro for the voice flow. (Only for LS or HS).
        UINT32  voiceRight:5;
        /// Left gain on the stereo macro for the voice flow (Only for LS or HS)..
        UINT32  voiceLeft:5;
        /// Preamplifier of the voice flow before the stereo macro (Only for LS or HS)..
        UINT32  voicePreamp:3;
        /// Analog gain of the voice flow.
        UINT32  voiceAnalog:3;
        /// Digital gain of the voice flow.
        UINT32  voiceDigital:2;

        /// Calibrated gains for a music flow.
        /// Right gain on the stereo macro for the music flow
        UINT32  musicRight:5;
        /// Left gain on the stereo macro for the music flow
        UINT32  musicLeft:5;
        /// Digital gain on the stereo macro for the music flow
        UINT32  musicDigital:3;
        /// Stuff bit.
        UINT32  :1;
    } gains;
} AUD_CODEC_CALIB_OUT_GAINS_T;


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

#ifdef REDUCE_RECV_MIN_GAIN
extern UINT32 g_halRecvReducedLevel;
#endif // REDUCE_RECV_MIN_GAIN

/// Playing status
PRIVATE BOOL  g_audCodecPlaying  = FALSE;

/// Recording status 
PRIVATE BOOL  g_audCodecRecording  = FALSE;

/// Openness status 
PRIVATE BOOL  g_audCodecOpened  = FALSE;

/// Remember the last settings applied
/// They will be set again to take into account any potential
/// calibration change.
PRIVATE AUD_LEVEL_T g_audCodecLatestLevel  = 
{       
        .spkLevel   = SND_SPK_MUTE,
        .micLevel   = SND_MIC_MUTE,
        .sideLevel  = SND_SIDE_MUTE,
        .toneLevel  = SND_TONE_0DB,
};

PRIVATE AUD_DEVICE_CFG_T g_audCodecLatestCfg   = 
{
        .spkSel = AUD_SPK_DISABLE,
        .spkType = AUD_SPEAKER_STEREO_NA,
        .micSel = AUD_MIC_DISABLE,
        .level  = &g_audCodecLatestLevel
};

PRIVATE HAL_AIF_FREQ_T g_audCodecSampleRate;

PRIVATE BOOL g_audCodecVoiceQuality;

PRIVATE HAL_AUD_USER_T g_audCodecCurrentUser = HAL_AUD_USER_NULL;

PRIVATE CONST HAL_AIF_CONFIG_T *g_audCodecCurrentAifCfgPtr = NULL;

PRIVATE CONST HAL_AIF_CONFIG_T g_audCodecToneAifCfg =
{
    .interface = HAL_AIF_IF_PARALLEL_STEREO,
    .sampleRate = HAL_AIF_FREQ_8000HZ,
    .channelNb = HAL_AIF_MONO,
    .serialCfg = NULL,
};

PRIVATE BOOL g_audCodecBufferLoopBack = FALSE;

PRIVATE BOOL g_audCodecMicInput = FALSE;

// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// aud_CodecCommonSetConfig
// -----------------------------------------------------------------------------
/// Set the audio configuration.
/// @param isRecording Whether it is recording stream or not
/// @param user The audio user
/// @param pAifCfg Pointing to the AIF configuration to be set
/// @return TRUE if success, FALSE otherwise
// =============================================================================
PUBLIC BOOL aud_CodecCommonSetConfig(BOOL isRecording, HAL_AUD_USER_T user, CONST HAL_AIF_CONFIG_T *pAifCfg)
{
    // It is required to call StreamStart before StreamRecord
    if (isRecording && g_audCodecCurrentUser == user)
    {
        // The audio mode was set when starting stream
        return TRUE;
    }

    if (g_audCodecCurrentUser != HAL_AUD_USER_NULL)
    {
        return FALSE;
    }

    g_audCodecCurrentAifCfgPtr = NULL;
    g_audCodecBufferLoopBack = FALSE;
    g_audCodecMicInput = FALSE;

    if (user == HAL_AUD_USER_CODEC)
    {
        AUD_ASSERT(pAifCfg != NULL, "Missing AIF cfg");
        g_audCodecCurrentAifCfgPtr = pAifCfg;

        HAL_AIF_IF_T aifIf = g_audCodecCurrentAifCfgPtr->interface;
        // Check if buffer loopback is needed
        if (aifIf == HAL_AIF_IF_SERIAL_IN_PARALLEL_OUT ||
            aifIf == HAL_AIF_IF_SERIAL_1_IN_PARALLEL_OUT ||
            aifIf == HAL_AIF_IF_SERIAL_2_IN_PARALLEL_OUT)
        {
            g_audCodecBufferLoopBack = TRUE;
        }
        // Check if there is MIC input
        if (aifIf == HAL_AIF_IF_PARALLEL ||
            aifIf == HAL_AIF_IF_PARALLEL_STEREO)
        {
            g_audCodecMicInput = TRUE;
        }
    }

    // g_audCodecCurrentUser will be cleared in aud_CodecCommonClose()
    g_audCodecCurrentUser = user;

    return TRUE;
}


// =============================================================================
// aud_CodecCommonSetRateStg
// -----------------------------------------------------------------------------
/// write the correct CodecGallite register to set the sample rate using HAL types.
/// @param  sampleRate The AIF sample rate
/// @return
// =============================================================================
PRIVATE VOID aud_CodecCommonSetRateStg(HAL_AIF_FREQ_T sampleRate)
{
    UINT32 sel = 0;
    UINT32 div = 0;
    
    switch (sampleRate)
    {
        case HAL_AIF_FREQ_8000HZ :
        {
		sel = FREQ_SAMPLE_SEL_8K;
              div = FREQ_SAMPLE_DIV_8K;
		break;
        }
        case HAL_AIF_FREQ_11025HZ:
        {
		sel = FREQ_SAMPLE_SEL_11_025K;
              div = FREQ_SAMPLE_DIV_11_025K;
		break;
        }
        case HAL_AIF_FREQ_12000HZ:
        {
		sel = FREQ_SAMPLE_SEL_12K;
              div = FREQ_SAMPLE_DIV_12K;
		break;
        }
        case HAL_AIF_FREQ_16000HZ:
        {
		sel = FREQ_SAMPLE_SEL_16K;
              div = FREQ_SAMPLE_DIV_16K;
		break;
        }
        case HAL_AIF_FREQ_22050HZ:
        {
		sel = FREQ_SAMPLE_SEL_22_05K;
              div = FREQ_SAMPLE_DIV_22_05K;
		break;
        }
        case HAL_AIF_FREQ_24000HZ:
        {
		sel = FREQ_SAMPLE_SEL_24K;
              div = FREQ_SAMPLE_DIV_24K;
		break;
        }
        case HAL_AIF_FREQ_32000HZ:
        {
		sel = FREQ_SAMPLE_SEL_32K;
              div = FREQ_SAMPLE_DIV_32K;
		break;
        }
        case HAL_AIF_FREQ_44100HZ:
        {
		sel = FREQ_SAMPLE_SEL_44_1K;
              div = FREQ_SAMPLE_DIV_44_1K;
		break;
        }
        case HAL_AIF_FREQ_48000HZ:
        {
		sel = FREQ_SAMPLE_SEL_48K;
              div = FREQ_SAMPLE_DIV_48K;
		break;
        }
        default:
        {
            AUD_ASSERT(FALSE, "Unsupported sample rate in AUD: %s, %d", __FILE__, __LINE__);
            break;
        }
    }

    hal_AbbRegWrite(CODEC_FUNC_FREQ_SAMPLE_SEL, sel);
    hal_AbbRegWrite(CODEC_FUNC_FREQ_SAMPLE_DIV, div);
    
    return;
}


// =============================================================================
// aud_CodeGallitePrePowerOnConfig
// -----------------------------------------------------------------------------
/// Set registers before powering on codec.
/// @return
// =============================================================================
PRIVATE VOID aud_CodeGallitePrePowerOnConfig(VOID)
{
    // Set sample rate
    aud_CodecCommonSetRateStg(g_audCodecSampleRate);
    // Set voice mode
    hal_AudSetVoiceMode(g_audCodecVoiceQuality);

    // Reset the digital part of audio codec
    hal_AbbRegWrite(CODEC_POWER_CTRL, ABB_RSTN_CODEC_DR|ABB_RSTN_CODEC|
                                      ABB_DAC_EN_CODEC_DR);
    hal_AbbRegWrite(CODEC_POWER_CTRL, ABB_RSTN_CODEC_DR|ABB_RSTN_CODEC|
                                      ABB_DAC_EN_CODEC_DR|ABB_DAC_EN_CODEC);
}


// =============================================================================
// aud_CodecCommonChipConfig
// -----------------------------------------------------------------------------
/// Configure the CodecGallite chip, through I2C.
/// The stream parameter is needed to know the format of the PCM 
/// thing to play. The cfg one is to know which mic, spk etc use
/// Interfaces matching table:
/// -
/// -
/// .
/// 
/// 
/// @param sampleRate Sample Rate of the audio to play
/// @param voice \c TRUE if in voice mode.
/// @param mono  \c TRUE if the stream played/recorded is in Mono
/// @return 
// =============================================================================
PRIVATE AUD_ERR_T aud_CodecCommonChipConfig(
                                        HAL_AIF_FREQ_T sampleRate,
                                        BOOL voice,
                                        BOOL mono)
{
    AUD_PROFILE_FUNCTION_ENTRY(aud_ChipConfig);

    if (voice)
    {
        AUD_ASSERT(sampleRate == HAL_AIF_FREQ_8000HZ,
                "AUD: CodecGallite: Voice mode only supports 8kHz frequency.");
        AUD_ASSERT(mono == TRUE,
                "AUD: CodecGallite: Voice mode is mono only.");
        // TODO add a bunch of assert/check
    }

    g_audCodecSampleRate = sampleRate;
    g_audCodecVoiceQuality = voice;

    BOOL ret = FALSE;
    if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
    {
        ret = hal_AudOpen(g_audCodecCurrentUser, aud_CodeGallitePrePowerOnConfig);
    }
    else if (g_audCodecCurrentUser == HAL_AUD_USER_FM ||
             g_audCodecCurrentUser == HAL_AUD_USER_LINEIN)
    {
        ret = hal_AudOpen(g_audCodecCurrentUser, NULL);
    }
    else
    {
        AUD_ASSERT(FALSE, "Invalid audio mode: %d", g_audCodecCurrentUser);
    }

    // Configure audio codec
    if (!ret)
    {
        return AUD_ERR_RESOURCE_BUSY;
    }

    AUD_PROFILE_FUNCTION_EXIT(aud_ChipConfig);
    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecCommonAifConfig
// -----------------------------------------------------------------------------
/// Configure the AIF interface for use with the CodecGallite chip.
/// @return AUD_ERR_NO
// =============================================================================
INLINE AUD_ERR_T aud_CodecCommonAifConfig(VOID)
{
    if (g_audCodecCurrentUser != HAL_AUD_USER_CODEC)
    {
        // No need to open AIF
        return AUD_ERR_NO;
    }

    hal_AifOpen(g_audCodecCurrentAifCfgPtr);

    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecCommonClose
// -----------------------------------------------------------------------------
/// Close the driver and configure the CodecGallite and AIF.
/// @return AUD_ERR_NO or ...
// =============================================================================
PRIVATE AUD_ERR_T aud_CodecCommonClose(VOID)
{
    if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
    {
        hal_AifClose();
    }

#ifdef REDUCE_RECV_MIN_GAIN
    g_halRecvReducedLevel = 0;
#endif // REDUCE_RECV_MIN_GAIN

    if (g_audCodecCurrentUser != HAL_AUD_USER_NULL)
    {
        // Except for earpiece detection, HAL AUD is always
        // used in EDRV AUD, and is protected by EDRV AUD mutex.
        while(!hal_AudClose(g_audCodecCurrentUser))
        {
            sxr_Sleep(AUD_TIME_RETRY);
        }

        g_audCodecCurrentUser = HAL_AUD_USER_NULL;
    }

    g_audCodecLatestCfg.spkSel = AUD_SPK_DISABLE;
    g_audCodecLatestCfg.spkType = AUD_SPEAKER_STEREO_NA;
    g_audCodecLatestCfg.micSel = AUD_MIC_DISABLE;

    g_audCodecOpened = FALSE;

    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecCommonOpen
// -----------------------------------------------------------------------------
/// Open the driver and configure the CodecGallite and AIF.
/// @param sampleRate Sample Rate of the audio to play
/// @param voice \c TRUE if in voice mode.
/// @param mono  \c TRUE if the stream played/recorded is in Mono
/// @return AUD_ERR_NO or ...
// =============================================================================
PRIVATE AUD_ERR_T aud_CodecCommonOpen(HAL_AIF_FREQ_T sampleRate, BOOL voice, BOOL mono)
{
#ifdef SPI_REG_DEBUG
    extern VOID abbd_RegisterSpiRegCheckFunc(VOID);
    abbd_RegisterSpiRegCheckFunc();
#endif // SPI_REG_DEBUG

    AUD_ERR_T errStatus;
    errStatus = aud_CodecCommonChipConfig(sampleRate, voice, mono);
    if (errStatus == AUD_ERR_NO)
    {
        errStatus = aud_CodecCommonAifConfig();
        if (errStatus == AUD_ERR_NO)
        {
            g_audCodecOpened = TRUE;
        }
    }

    if (!g_audCodecOpened)
    {
        aud_CodecCommonClose();
    }

    return errStatus;
}


// =============================================================================
// aud_CodecCommonSetup
// -----------------------------------------------------------------------------
/// Configure the audio. 
/// 
/// This functions configures an audio interface: gain for the side tone and  
/// the speaker, input selection for the microphone and output selection for 
/// the speaker.
/// 
/// @param cfg The configuration set applied to the audio interface. See 
/// #AUD_DEVICE_CFG_T for more details.
/// @return #AUD_ERR_NO if it can execute the configuration.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg)
{
    // Check parameters
    AUD_ASSERT(cfg->spkSel<AUD_SPK_QTY || cfg->spkSel == AUD_SPK_DISABLE,
            "AUD: Improper Spk Id:%d", cfg->spkSel);
    AUD_ASSERT(cfg->micSel<AUD_MIC_QTY || cfg->micSel == AUD_MIC_DISABLE,
            "AUD: Improper Mic Id:%d", cfg->micSel);
    AUD_ASSERT(cfg->level->spkLevel<AUD_SPK_VOL_QTY, "AUD: Improper Spk Level");
    AUD_ASSERT(cfg->level->micLevel<AUD_MIC_VOL_QTY, "AUD: Improper Mic Level");
    AUD_ASSERT(cfg->level->sideLevel<AUD_SIDE_VOL_QTY, "AUD: Improper Side Level");
    AUD_ASSERT(cfg->level->toneLevel<AUD_TONE_VOL_QTY, "AUD: Improper Tone Level: %d",cfg->level->toneLevel );

    UINT8 regSpkLvl;
    CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
    AUD_CODEC_CALIB_OUT_GAINS_T calibOutGains;

    calibOutGains.calibValue = calibPtr->bb->audio[itf].audioGains.outGain[cfg->level->spkLevel];

    if (g_audCodecVoiceQuality)
    {
        regSpkLvl = calibOutGains.gains.voiceLeft;

#ifdef REDUCE_RECV_MIN_GAIN
        // Hardcode all the audio gains for receiver voice
        if (itf == SND_ITF_RECEIVER &&
            cfg->level->spkLevel != SND_SPK_MUTE)
        {
#if 0
            AUD_CODEC_CALIB_OUT_GAINS_T calibOutGains1, calibOutGains2, calibOutGains3;
            calibOutGains1.calibValue = calibPtr->bb->audio[itf].audioGains.outGain[SND_SPK_VOL_1];
            calibOutGains2.calibValue = calibPtr->bb->audio[itf].audioGains.outGain[SND_SPK_VOL_2];
            calibOutGains3.calibValue = calibPtr->bb->audio[itf].audioGains.outGain[SND_SPK_VOL_3];
            if ( calibOutGains1.gains.voiceLeft == 0 &&
                 calibOutGains2.gains.voiceLeft == 0 &&
                 calibOutGains3.gains.voiceLeft == 0 )
#endif
            if (cfg->level->spkLevel == SND_SPK_VOL_1)
            {
                g_halRecvReducedLevel = 2;
                regSpkLvl = 0;
            }
            else if (cfg->level->spkLevel == SND_SPK_VOL_2)
            {
                g_halRecvReducedLevel = 1;
                regSpkLvl = 0;
            }
            else
            {
                g_halRecvReducedLevel = 0;
                switch(cfg->level->spkLevel)
                {
                    case SND_SPK_VOL_3:
                        regSpkLvl = 0;
                        break;
                    case SND_SPK_VOL_4:
                        regSpkLvl = 2;
                        break;
                    case SND_SPK_VOL_5:
                        regSpkLvl = 4;
                        break;
                    case SND_SPK_VOL_6:
                        regSpkLvl = 6;
                        break;
                    default: // SND_SPK_VOL_7
                        regSpkLvl = 8;
                        break;
                }
            }
        }
        else
        {
            g_halRecvReducedLevel = 0;
        }
#endif // REDUCE_RECV_MIN_GAIN
    }
    else
    {
        regSpkLvl = calibOutGains.gains.musicLeft;
    }

    // TODO Implement all the missing features

    // TODO Use calibration stuff to know the actual value to write in
    // the registers.

    // TODO Add if statement to discrimate about the Speaker, the recevier or other spkSel

    // TODO Mic interface to add in mic mode writing for volume

    HAL_AUD_SEL_T selCfg;
    selCfg.reg = 0;

    if (g_audCodecCurrentUser == HAL_AUD_USER_FM)
    {
        selCfg.fmSel = TRUE;
    }

    switch (cfg->spkSel)
    {
        case AUD_SPK_RECEIVER:
        {
            if(cfg->level->spkLevel == SND_SPK_MUTE)
            {
                selCfg.rcvSel = FALSE;
            }
            else
            {
                selCfg.rcvSel = TRUE;
                hal_AudSetRcvGain(regSpkLvl);
            }
            break;
        }
        case AUD_SPK_EAR_PIECE:
        {
            if(cfg->level->spkLevel == SND_SPK_MUTE)
            {
                selCfg.headSel = FALSE;
            }
            else
            {
                selCfg.headSel = TRUE;
                hal_AudSetHeadGain(regSpkLvl);
            }
            break;
        }
        case AUD_SPK_LOUD_SPEAKER:
        {
            HAL_AUD_SPK_CFG_T spkCfg;
            spkCfg.reg = 0;
            if (cfg->level->spkLevel == SND_SPK_MUTE)
            {
                selCfg.spkSel = FALSE;
                spkCfg.noSpkMute = FALSE;
            }
            else
            {
                if (cfg->spkType == AUD_SPEAKER_STEREO)
                {
                    selCfg.spkSel = TRUE;
                }
                else
                {
                    selCfg.spkMonoSel = TRUE;
                }

                spkCfg.noSpkMute = TRUE;
                spkCfg.spkGain = regSpkLvl;
            }
            hal_AudSetSpkCfg(spkCfg);
            break;
        }
        default:
        {
            break;
        }
    }

    if ( cfg->micSel == AUD_MIC_RECEIVER ||
          cfg->micSel == AUD_MIC_EAR_PIECE ||
          cfg->micSel == AUD_MIC_LOUD_SPEAKER )
    {
        switch (cfg->micSel)
        {
            case AUD_MIC_EAR_PIECE:
            {
                selCfg.auxMicSel = TRUE;
                break;
            }
            case AUD_MIC_RECEIVER:
            case AUD_MIC_LOUD_SPEAKER:
            {
                break;
            }
            default:
            {
                break;
            }
        }

        HAL_AUD_MIC_CFG_T micCfg;
        micCfg.reg = 0;
        if (cfg->level->micLevel != SND_MIC_MUTE)
        {
            micCfg.noMicMute = TRUE;
            micCfg.micGain = calibPtr->bb->audio[itf].audioGains.inGain;
        }
        hal_AudSetMicCfg(micCfg);
    }

    if ( cfg->spkSel != g_audCodecLatestCfg.spkSel ||
         cfg->micSel != g_audCodecLatestCfg.micSel ||
         ( cfg->spkSel == AUD_SPK_LOUD_SPEAKER &&
           cfg->spkType != g_audCodecLatestCfg.spkType ) ||
         ( (cfg->level->spkLevel == SND_SPK_MUTE || 
             g_audCodecLatestLevel.spkLevel == SND_SPK_MUTE) &&
           cfg->level->spkLevel != g_audCodecLatestLevel.spkLevel ) )
    {
        hal_AudSetSelCfg(selCfg);
    }

    if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
    {
        // Side tone volume controlled in the AIF. Normal use or loop test use.
        if (AUD_SIDE_VOL_TEST == cfg->level->sideLevel)
        {
            // If in loop test mode, the side tone and mic are set to their max values.
            HAL_AUD_MIC_CFG_T micCfg;
            micCfg.noMicMute = TRUE;
            micCfg.micGain = 15;
            hal_AudSetMicCfg(micCfg);
            hal_AifSetSideTone(10);
        }
        else
        {
            hal_AifSetSideTone(calibPtr->bb->audio[itf].audioGains.sideTone[cfg->level->spkLevel]);
        }
    }

    // Record last config
    g_audCodecLatestCfg.spkSel = cfg->spkSel;
    g_audCodecLatestCfg.spkType = cfg->spkType;
    g_audCodecLatestCfg.micSel = cfg->micSel;
    g_audCodecLatestLevel = *(cfg->level);

    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecCommonStreamStart
// -----------------------------------------------------------------------------
/// The common codes to start the playing of a stream. 
/// 
/// This function  outputs the 
/// audio on the audio interface specified as a parameter, on the output selected in 
/// the #aud_Setup function. \n
/// In normal operation, when 
/// the buffer runs out, the playing will wrap at the beginning. Here, there are two ways 
/// you can handle your buffer: HAL can call a user function at the middle or 
/// at the end of the playback or, depending 
/// on your application, you can simply poll the playing state using the "reached half"
/// and "reached end functions" TODO Ask if needed, and then implement !
/// 
/// @param stream Stream to play. Handler called at the middle and end of the buffer
/// are defined there.
/// @param cfg The configuration set applied to the audio interface
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command,
///         #AUD_ERR_NO it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonStreamStart(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;

    if (g_audCodecPlaying)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (!g_audCodecOpened)
    {
        errStatus = aud_CodecCommonOpen(stream->sampleRate, stream->voiceQuality,
            (stream->channelNb == HAL_AIF_MONO));

        if (errStatus != AUD_ERR_NO)
        {
            return errStatus;
        }
        else
        {
            needCloseAud = TRUE;
        }
    }

    errStatus = aud_CodecCommonSetup(itf, cfg);

    if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
    {
        if (errStatus == AUD_ERR_NO)
        {
            if (g_audCodecBufferLoopBack)
            {
				// Send the stream through the IFC
				if (hal_AifRecordStream(stream) != HAL_ERR_NO)
				{
				    AUD_TRACE(AUD_INFO_TRC, 0,
				        "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
				    errStatus = AUD_ERR_RESOURCE_BUSY;
				}
                else
                {
                    // Recording is in progress by now
                    g_audCodecRecording = TRUE;
                }
            }
        }

        if (errStatus == AUD_ERR_NO)
        {
#if (VCOM_BAT_LARGE_CAP)
            // If the capacitor connected to the loud speaker is large (larger
            // than 1uF), it might take an extra long time to be charged,
            // and then the audio PA can work. If the sound being played is
            // very short (e.g., camera shutter sound), it might NOT be
            // output via the loud speaker.
            // Workaround: Wait for a while before playing the sound.
            if (itf == SND_ITF_LOUD_SPEAKER)
            {
                sxr_Sleep(300 MS_WAITING);
            }
#endif // VCOM_BAT_LARGE_CAP
            // Send the stream through the IFC
            if (hal_AifPlayStream(stream) != HAL_ERR_NO)
            {
                AUD_TRACE(AUD_INFO_TRC, 0,
                    "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
                errStatus = AUD_ERR_RESOURCE_BUSY;
            }
        }
    } // HAL_AUD_USER_CODEC

    if (errStatus == AUD_ERR_NO)
    {
        needCloseAud = FALSE;
        g_audCodecPlaying = TRUE;
    }

    if (needCloseAud)
    {
        aud_CodecCommonClose();
    }

    return errStatus;
}


// =============================================================================
// aud_CodecCommonStreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonStreamStop(SND_ITF_T itf)
{
    if (g_audCodecPlaying)
    {
        if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
        {
            hal_AifStopPlay();
        }
        g_audCodecPlaying = FALSE;
    }

    if (g_audCodecRecording)
    {
        if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
        {
            hal_AifStopRecord();
        }
        if (g_audCodecMicInput)
        {
            pmd_EnablePower(PMD_POWER_MIC, FALSE);
        }
        g_audCodecRecording = FALSE;
    }

    if (g_audCodecOpened)
    {
        return aud_CodecCommonClose();
    }
    else
    {
        return AUD_ERR_NO;
    }
}


// =============================================================================
// aud_CodecCommonStreamPause
// -----------------------------------------------------------------------------
/// This function pauses the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param pause If \c TRUE, pauses the stream. If \c FALSE, resumes a paused
/// stream.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonStreamPause(SND_ITF_T itf, BOOL pause)
{
    if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
    {
        if (hal_AifPause(pause) == HAL_ERR_NO)
        {
            return AUD_ERR_NO;
        }
    }

    return AUD_ERR_NO;
}


// =============================================================================
// aud_CodecCommonStreamRecord
// -----------------------------------------------------------------------------
/// The common codes to manage the recording of a stream. 
/// 
///
/// The #aud_StreamRecord send a message to the driver which begins the 
/// dataflow from the audio and the compression. If the function returns \c 
/// FALSE, it means that the driver cannot handle the record. The value \c len 
/// should be the maximum size of the recording buffer and when the buffer is 
/// full, the recording will automatically wrap. A 
/// user function can be called, either when the middle or the end of the 
/// buffer is reached. (TODO add iatus about polling if available)
/// 
/// @param stream Stream recorded (describe the buffer and the possible interrupt
/// of middle-end buffer handlers.
/// @param cfg The configuration set applied to the audio interface.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver cannot handle the stream,
///         #AUD_ERR_NO if the stream ca be recorded.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;

    if (g_audCodecRecording)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (!g_audCodecOpened)
    {
        errStatus = aud_CodecCommonOpen(stream->sampleRate, stream->voiceQuality,
            (stream->channelNb == HAL_AIF_MONO));

        if (errStatus != AUD_ERR_NO)
        {
            return errStatus;
        }
        else
        {
            needCloseAud = TRUE;
        }
    }

    errStatus = aud_CodecCommonSetup(itf, cfg);

    if (g_audCodecCurrentUser == HAL_AUD_USER_CODEC)
    {
        if (errStatus == AUD_ERR_NO)
        {
            // Send the stream through the IFC
            if (hal_AifRecordStream(stream) != HAL_ERR_NO)
            {
                AUD_TRACE(AUD_INFO_TRC, 0,
                    "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
                errStatus = AUD_ERR_RESOURCE_BUSY;
            }
        }
    }

    if (errStatus == AUD_ERR_NO)
    {
        if (g_audCodecMicInput)
        {
            pmd_EnablePower(PMD_POWER_MIC, TRUE);
        }
        needCloseAud = FALSE;
        g_audCodecRecording = TRUE;
    }

    if (needCloseAud)
    {
        aud_CodecCommonClose();
    }

    return errStatus;
}


// =============================================================================
// aud_CodecCommonTone
// -----------------------------------------------------------------------------
//  Manage the playing of a tone: DTMF or Comfort Tone. 
/// 
/// Outputs a DTMF or comfort tone
///
/// When the audio output is enabled, a DTMF or a comfort tones can be output 
/// as well. This function starts the output of a tone generated in the audio 
/// module. \n
/// You can call this function several times without calling the 
/// #aud_ToneStop function or the #aud_TonePause function in 
/// between, if you just need to change the attenuation or the tone type. \n
/// If the function returns #AUD_ERR_RESOURCE_BUSY, it means that the driver is 
/// busy with an other audio command.
///
/// @param tone The tone to generate
/// @param cfg The configuration set applied to the audio interface
/// @param start If \c TRUE, starts the playing of the tone.
///              If \c FALSE, stops the tone. 
///
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T        tone,
                CONST AUD_DEVICE_CFG_T*             cfg,
                CONST BOOL                   start)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;

    if (!g_audCodecOpened && start)
    {
        // Open the codec
        // FIXME Use the stream phantom !
        if (!aud_CodecCommonSetConfig(FALSE, HAL_AUD_USER_CODEC, &g_audCodecToneAifCfg))
        {
            return AUD_ERR_RESOURCE_BUSY;
        }
        errStatus = aud_CodecCommonOpen(HAL_AIF_FREQ_8000HZ, FALSE, TRUE);
        needCloseAud = TRUE;
    }

    if (g_audCodecCurrentUser != HAL_AUD_USER_CODEC)
    {
        // AIF is not opened unless the user is codec.
        // The tone cannot be played here.
        return AUD_ERR_NO;
    }

    if (start)
    {
        if (errStatus == AUD_ERR_NO)
        {
            errStatus = aud_CodecCommonSetup(itf, cfg);
            if (errStatus == AUD_ERR_NO)
            {
                hal_AifTone(tone, cfg->level->toneLevel, TRUE);
                needCloseAud = FALSE;
            }
        }

        if (needCloseAud)
        {
            aud_CodecCommonClose();
        }
    }
    else
    {
        // Stop tone anyway
        hal_AifTone(0, 0, FALSE);
        if(!g_audCodecPlaying && !g_audCodecRecording)
        {
            aud_CodecCommonClose();
        }
    }

    return errStatus;
}


// =============================================================================
// aud_CodecCommonTonePause
// -----------------------------------------------------------------------------
/// This function pauses the audio tone.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param pause If \c TRUE, pauses the tone. If \c FALSE, resumes a paused
/// tone.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonTonePause(SND_ITF_T itf, BOOL pause)
{
    switch (hal_AifTonePause(pause))
    {
        case HAL_ERR_NO:
        default:
            return AUD_ERR_NO;
            break;
    }
}


// =============================================================================
// aud_CodecCommonCalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecCommonCalibUpdateValues(SND_ITF_T itf)
{
    // FIXME --> Open the i2c before ...
//    return aud_CodecCommonSetup(itf, &g_audCodecLatestCfg);
    return HAL_ERR_NO;
}


#ifdef SPI_REG_DEBUG
// ======================================================
// Debug: Read and Write Registers via SPI

#define ABBD_SPI_REG_VALUE_LOCATION __attribute__((section(".abb_reg_value")));

#define SPI_REG_WRITE_FLAG (1<<31)
#define SPI_REG_READ_FLAG (1<<30)
#define SPI_REG_FORCE_WRITE_FLAG (1<<29)
#define SPI_REG_FORCE_READ_FLAG (1<<28)
#define SPI_REG_DATA_MASK (0xffff)

#define ABB_REG_NUM (0x36)
UINT32 g_abbdSpiRegValue[ABB_REG_NUM] ABBD_SPI_REG_VALUE_LOCATION;

PROTECTED VOID abbd_SpiRegValueInit(VOID)
{
    for (int i=0; i<ABB_REG_NUM; i++)
    {
        g_abbdSpiRegValue[i] = 0;
    }
}

PUBLIC VOID abbd_SpiRegCheck(VOID)
{
    static BOOL initDone = FALSE;
    if (!initDone)
    {
        abbd_SpiRegValueInit();
        initDone = TRUE;
    }
    
    for (int i=0; i<ABB_REG_NUM; i++)
    {
        if (g_abbdSpiRegValue[i] & SPI_REG_WRITE_FLAG)
        {
            if (!hal_AbbRegWrite(i, g_abbdSpiRegValue[i]&SPI_REG_DATA_MASK))
            {
                continue;
            }
            g_abbdSpiRegValue[i] &= ~SPI_REG_WRITE_FLAG;
        }
        if (g_abbdSpiRegValue[i] & SPI_REG_READ_FLAG)
        {
            UINT32 data;
            if(!hal_AbbRegRead(i, &data))
            {
                continue;
            }
            g_abbdSpiRegValue[i] &= ~(SPI_REG_READ_FLAG|SPI_REG_DATA_MASK);
            g_abbdSpiRegValue[i] |= data&SPI_REG_DATA_MASK;
        }
    }
}

PUBLIC VOID abbd_RegisterSpiRegCheckFunc(VOID)
{
    static BOOL registerIdleHook = FALSE;
    if (!registerIdleHook)
    {
        registerIdleHook = TRUE;
        extern bool sxs_RegisterDebugIdleHookFunc(void (*funcPtr)(void));
        sxs_RegisterDebugIdleHookFunc(&abbd_SpiRegCheck);
    }
}
#endif // SPI_REG_DEBUG



