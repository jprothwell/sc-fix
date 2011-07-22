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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/aud/maxim/src/aud_maxim.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file aud_maxime.c                                                        //
/// That file provides the implementation of the AUD interface for the maxime //
//  chip.                                                                     //
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================

#include "cs_types.h"

#include "aud_maxim.h"
#include "audp_maxim.h"
#include "audp_debug.h"

#include "hal_i2c.h"
#include "hal_aif.h"
#include "hal_timers.h"
#include "hal_error.h"
#include "hal_sys.h"

#include "tgt_aud_cfg.h"

// =============================================================================
//  MACROS
// =============================================================================
/// Write Time out in 16384Hz ticks.
#define MAXIM_WR_TIMEOUT    0x200

/// Helper to write a register
#define MAXIM_WR(reg,val)   \
    { \
        halErr = HAL_ERR_RESOURCE_BUSY; \
        UINT32 startTime = hal_TimGetUpTime(); \
        while (halErr == HAL_ERR_RESOURCE_BUSY)\
        {   \
            halErr = hal_I2cSendByte(MAXIM_ADDR, (reg), (val)); \
            if (hal_TimGetUpTime() - startTime > MAXIM_WR_TIMEOUT) \
            { \
                break; \
            } \
        } \
        \
        if (halErr != HAL_ERR_NO) \
        { \
            AUD_ASSERT(FALSE, "%s line %d", __FILE__, __LINE__); \
                return AUD_ERR_RESOURCE_BUSY;   \
        } \
    }
    


// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

/// Playing status
PRIVATE BOOL  g_audMaximPlaying  = FALSE;

/// Recording status 
PRIVATE BOOL  g_audMaximRecording  = FALSE;

/// Openness status 
PRIVATE BOOL  g_audMaximOpened  = FALSE;

/// Being open status
PRIVATE BOOL g_audMaximOpening  = FALSE;

/// Remember the last settings applied
/// They will be set again to take into account any potential
/// calibration change.
PROTECTED AUD_LEVEL_T g_audMaximLatestLevel  = 
{       
        .spkLevel   = SND_SPK_MUTE,
        .micLevel   = SND_MIC_MUTE,
        .sideLevel  = SND_SIDE_MUTE,
        .toneLevel  = SND_TONE_0DB,
};

PROTECTED AUD_DEVICE_CFG_T g_audMaximLatestCfg   = 
    {
        .spkSel = AUD_SPK_RECEIVER,
        .micSel = AUD_MIC_RECEIVER,
        .level  = &g_audMaximLatestLevel
    };

// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// aud_MaximeGetRateStg
// -----------------------------------------------------------------------------
/// @return value to write in the correct maxim register to set the sample rate
/// using HAL types.
// =============================================================================
INLINE UINT32 aud_MaximeGetRateStg(HAL_AIF_FREQ_T sampleRate)
{
    UINT32 setting;
    switch (sampleRate)
    {
        case HAL_AIF_FREQ_8000HZ :
			setting =  STEREO_8K;
			break;

	    case HAL_AIF_FREQ_11025HZ:
			setting =  STEREO_11K;
			break;

	    case HAL_AIF_FREQ_12000HZ:
			setting =  STEREO_12K;
			break;

	    case HAL_AIF_FREQ_16000HZ:
			setting =  STEREO_16K;
			break;

	    case HAL_AIF_FREQ_22050HZ:
			setting =  STEREO_22K;
			break;

	    case HAL_AIF_FREQ_24000HZ:
			setting =  STEREO_24K;
			break;

	    case HAL_AIF_FREQ_32000HZ:
			setting =  STEREO_32K;
			break;

	    case HAL_AIF_FREQ_44100HZ:
			setting =  STEREO_44K;
			break;

        case HAL_AIF_FREQ_48000HZ:
            setting =  STEREO_48K;
			break;
        
        default:
            setting = 0;
            AUD_ASSERT(FALSE, "Unsupported sample rate in AUD: %s, %d", __FILE__, __LINE__);
    }
    
    return setting;
}

// =============================================================================
// aud_ChipConfig
// -----------------------------------------------------------------------------
/// Configure the maxim chip, through I2C.
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
/// @param master \c TRUE if the maxim is the I2S master.
/// @return 
// =============================================================================
PRIVATE AUD_ERR_T aud_ChipConfig(HAL_AIF_FREQ_T sampleRate, BOOL voice, BOOL mono, BOOL master)
{
    AUD_PROFILE_FUNCTION_ENTRY(aud_ChipConfig);
    HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;

    if (voice)
    {
        AUD_ASSERT(sampleRate == HAL_AIF_FREQ_8000HZ,
                "AUD: maxim: Voice mode only supports 8kHz frequency.");
        // TODO add a bunch of assert/check
    }

    ;

    // enable all IRQs
    MAXIM_WR(REG_MAXIM_INT_ENABLE,
            (INT_ICLD |INT_ISLD |INT_IULK |INT_IHSD |INT_IFAULT ) );

    // Disable primary digital audio interface for proper configure
    MAXIM_WR(REG_MAXIM_S1_IF_MODE_0, 0x0);

    // Not necessary configured
    if (master)
    {    
        MAXIM_WR(REG_MAXIM_S1_IF_MODE_1, S1MAS|S1DLY)
    }
    else
    {
        MAXIM_WR(REG_MAXIM_S1_IF_MODE_1, S1DLY);
    }    

    // 13MHz Clock and voice band filters for both DAC and ADC
    MAXIM_WR(REG_MAXIM_DIGITAL_FILTERS, ADC_BAND_PASS_FILTER_ENABLE | DAC_BAND_PASS_FILTER_ENABLE);
    
    // route digital audio from primary interface to both DACs
    if (mono)
    {
        MAXIM_WR(REG_MAXIM_DIGITAL_MIXERS, S1_LEFT_LEFT_DAC | S1_LEFT_RIGHT_DAC);
    }
    else
    {
        MAXIM_WR(REG_MAXIM_DIGITAL_MIXERS, S1_LEFT_LEFT_DAC | S1_RIGHT_RIGHT_DAC);
    }

    if (mono)
    {
        // Route right microphone to left ADCs
        MAXIM_WR(REG_MAXIM_ADC_IN_MIX, RIGHT_MIC_LEFT_ADC);
    }
    else
    {
        // Left on left, right on right
        MAXIM_WR(REG_MAXIM_ADC_IN_MIX, RIGHT_MIC_RIGHT_ADC | LEFT_MIC_LEFT_ADC);
    }
   
    // Route DACs to the output
    MAXIM_WR(REG_MAXIM_AUDIO_OUTPUT_MIX, LEFT_DAC_O_LEFT_AU_O | RIGHT_DAC_O_RIGHT_AU_O);

    // Not necessary configured
    MAXIM_WR(REG_MAXIM_MIC_L_IN_GAIN, 0x0);
   
    // Not necessary configured
    MAXIM_WR(REG_MAXIM_MIC_R_IN_GAIN, 0x0);
    
    // External microphone selected
    MAXIM_WR(REG_MAXIM_MIC_MODE, EXT_MIC_INPUT | RESISTOR_BLAS_ENABLE);
       
    // Left headphone amplifier -20dB gain
    MAXIM_WR(REG_MAXIM_H_R_LEFT_VOL, 8);
    
    // right headphone amplifier -20dB gain
    MAXIM_WR(REG_MAXIM_H_R_RIGHT_VOL, 8); // H_R_VOL_m20DB);
	    
    // Enable volume smoothing and zero-crossing detection 
    // FIXME Features not necessarily needed ?
    // and select the speaker amplifier for output
    MAXIM_WR(REG_MAXIM_AUDIO_OUT_MODE, VOLUME_SMOOTHING | ZERO_CROSS_DETECT | STEREO_HEADPHONE_MODE);

    // Enable the MAXIME and configure the charge_pump
    // circuitry to run from MCLK
    MAXIM_WR(REG_MAXIM_SYSTEM, CHARGE_PUMP_SELECT | CHARGE_PUMP_EN
                            | LOW_FREQ_OSC_EN | CLOCK_IN_EN | START_H_STOP_L) ;

    // Enable the left and right DAC
    MAXIM_WR(REG_MAXIM_SHUTDOWN,
                  DAC_LEFT_EN 
                | DAC_RIGHT_EN 
                | ADC_LEFT_EN 
                | ADC_RIGHT_EN 
                | MIC_LEFT_EN 
                | MIC_RIGHT_EN);
	
    // Disable primary digital audio interface for proper configure
    if (voice)
    {    
        MAXIM_WR(REG_MAXIM_S1_IF_MODE_0, SERIAL_DATA_OUTPUT_ENABLE 
                    | SERIAL_DATA_INPUT_ENABLE 
                    | MODE_VOICE);
    }
    else
    {
        MAXIM_WR(REG_MAXIM_S1_IF_MODE_0, (SERIAL_DATA_OUTPUT_ENABLE
                 | SERIAL_DATA_INPUT_ENABLE
                 |(mono?SERIAL_INPUT_MONO_MIX_ENABLE:0)
                 |(aud_MaximeGetRateStg(sampleRate))
                 ));
    }
    // Wait a bit ... TODO Remove that useless wait !
    hal_TimDelay(AUD_MAXIM_CFG_DELAY);
    
    AUD_PROFILE_FUNCTION_EXIT(aud_ChipConfig);
    return AUD_ERR_NO;
}


// =============================================================================
// aud_MaximAifConfig
// -----------------------------------------------------------------------------
/// Configure the AIF interface for use with the maxim chip.
/// @param sampleRate Sample Rate of the audio to play
/// @param voice \c TRUE if in voice mode.
/// @param mono  \c TRUE if the stream played/recorded is in Mono
/// @param master Is the maxim master ?
/// @return AUD_ERR_NO
// =============================================================================
INLINE AUD_ERR_T aud_MaximAifConfig(HAL_AIF_FREQ_T sampleRate, BOOL voice,
                                            BOOL mono, BOOL master,
                                            HAL_AIF_IF_T aifItf)
{
    HAL_AIF_SERIAL_CFG_T serialCfg;
    HAL_AIF_CONFIG_T     aifCfg;

    if (voice)
    {
        serialCfg.mode = HAL_SERIAL_MODE_VOICE;
    }
    else
    {
        serialCfg.mode = HAL_SERIAL_MODE_I2S;
    }
    // TODO add the DAI case !

    serialCfg.aifIsMaster = !master; // If the maxim is master, the AIF ain't.
    serialCfg.lsbFirst  = FALSE;
    serialCfg.polarity  = !voice; //TRUE;
    serialCfg.rxDelay   = HAL_AIF_RX_DELAY_2;
    serialCfg.txDelay   = HAL_AIF_TX_DELAY_1;
    if (mono)
    {
        serialCfg.rxMode    = HAL_AIF_RX_MODE_STEREO_MONO_FROM_L; 
        serialCfg.txMode    = HAL_AIF_TX_MODE_MONO_STEREO_DUPLI;
    }
    else
    {
        serialCfg.rxMode    = HAL_AIF_RX_MODE_STEREO_STEREO; 
        serialCfg.txMode    = HAL_AIF_TX_MODE_STEREO_STEREO;
    }

// LCK/BCK
    switch (sampleRate)
    {
        case HAL_AIF_FREQ_8000HZ:
            serialCfg.fs = 8000;
            if (voice)
            {
                serialCfg.bckLrckRatio = 25;
            }
            else
            {
                serialCfg.bckLrckRatio = 50;
            }
            break;

        case HAL_AIF_FREQ_11025HZ:
            serialCfg.fs = 11025;
            serialCfg.bckLrckRatio = 36;
            break;

        case HAL_AIF_FREQ_12000HZ:
            serialCfg.fs = 12000;
            serialCfg.bckLrckRatio = 38;
            break;

        case HAL_AIF_FREQ_16000HZ:
            serialCfg.fs = 16000;
            serialCfg.bckLrckRatio = 50;
            break;

        case HAL_AIF_FREQ_22050HZ:
            serialCfg.fs = 22050;
            serialCfg.bckLrckRatio = 40;
            break;

        case HAL_AIF_FREQ_24000HZ:
            serialCfg.fs = 24000;
            serialCfg.bckLrckRatio = 38;
            break;

        case HAL_AIF_FREQ_32000HZ:
            serialCfg.fs = 32000;
            serialCfg.bckLrckRatio = 56;
            break;

        case HAL_AIF_FREQ_44100HZ:
            serialCfg.fs = 44100;
            serialCfg.bckLrckRatio = 62;
            break;

        case HAL_AIF_FREQ_48000HZ:
            serialCfg.fs = 48000;
            serialCfg.bckLrckRatio = 36;
            break;

        default:
            AUD_ASSERT(FALSE, "Improper stream frequency.");
            // Die
            break;
    }


// -----------------------    
    serialCfg.invertBck = master?TRUE:FALSE;
    serialCfg.outputHalfCycleDelay = master?FALSE:TRUE; 
    serialCfg.inputHalfCycleDelay = FALSE; 
    serialCfg.enableBckOutGating = FALSE;

#if (CHIP_HAS_I2S_DI_1 == 1)
    // We need to choose the I2S 'interface' to use
    aifCfg.interface = aifItf;
#else
    aifCfg.interface = HAL_AIF_IF_SERIAL;
#endif
    aifCfg.serialCfg = &serialCfg;

    hal_AifOpen(&aifCfg);

    return AUD_ERR_NO;
}


// =============================================================================
// aud_MaximOpen
// -----------------------------------------------------------------------------
/// Open the driver and configure the maxim and AIF for use with the maxim chip.
/// @param sampleRate Sample Rate of the audio to play
/// @param voice \c TRUE if in voice mode.
/// @param master Is the maxim master ?
/// @return AUD_ERR_NO or ...
// =============================================================================
PRIVATE AUD_ERR_T aud_MaximOpen(HAL_AIF_FREQ_T sampleRate, BOOL voice, BOOL mono, BOOL master,
                                HAL_AIF_IF_T aifItf)
{
    // TODO Use a global var not to open it again
    UINT32 status = hal_SysEnterCriticalSection();
    if (g_audMaximOpening)
    {
        hal_SysExitCriticalSection(status);
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }
    else
    {
        g_audMaximOpening = TRUE;
        hal_SysExitCriticalSection(status);

        AUD_ERR_T errStatus;
        errStatus = aud_ChipConfig(sampleRate, voice, mono, master);
        if (errStatus == AUD_ERR_NO)
        {
            errStatus = aud_MaximAifConfig(sampleRate, voice, mono, master, aifItf);
            if (errStatus == AUD_ERR_NO)
            {
                g_audMaximOpened = TRUE;
            }
        }
        g_audMaximOpening = FALSE;
        return errStatus;
    }
}


// =============================================================================
// aud_MaximClose
// -----------------------------------------------------------------------------
/// Close the driver and configure the maxim and AIF for use with the maxim chip.
/// @return AUD_ERR_NO or ...
// =============================================================================
PRIVATE AUD_ERR_T aud_MaximClose(VOID)
{
    //
    hal_AifClose();
    HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;


    // Disable the MAXIM chip
    // b2 = 1, other bits null
    while (halErr == HAL_ERR_RESOURCE_BUSY)
    {
        halErr = hal_I2cSendByte(MAXIM_ADDR, REG_MAXIM_SYSTEM, 0x4);
    }

    if (halErr != HAL_ERR_NO)
    {
        AUD_ASSERT(FALSE, "%s line %d", __FILE__, __LINE__);
			return AUD_ERR_RESOURCE_BUSY;     
    }

    // Enable the left and right DAC
    halErr = HAL_ERR_RESOURCE_BUSY;
	while (halErr == HAL_ERR_RESOURCE_BUSY)
	{
		halErr = hal_I2cSendByte(MAXIM_ADDR, REG_MAXIM_SHUTDOWN, 0);
	}

	if (halErr != HAL_ERR_NO)
    {
        AUD_ASSERT(FALSE, "%s line %d", __FILE__, __LINE__);
			return AUD_ERR_RESOURCE_BUSY;       
    }

    g_audMaximOpened = FALSE;
    
    return AUD_ERR_NO;
}



// =============================================================================
// aud_MaximSetup
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
PUBLIC AUD_ERR_T aud_MaximSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg)
{
    
    HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;
    // TODO hack ?
    UINT8 regSpkLvl = 0x15 - cfg->level->spkLevel;

    // TODO Implement all the missing features
    
    // TODO Use calibration stuff to know the actual value to write in
    // the registers.

    // TODO Add if statement to discrimate about the Speaker, the recevier or other spkSel

    // TODO Mic interface to add in mic mode writing for volume

    // Record last config
    g_audMaximLatestCfg.spkSel = cfg->spkSel;
    g_audMaximLatestCfg.spkType = cfg->spkType;
    g_audMaximLatestCfg.micSel = cfg->micSel;
    g_audMaximLatestLevel = *(cfg->level);

    if (cfg->level->spkLevel == SND_SPK_MUTE)
    {
        halErr = HAL_ERR_RESOURCE_BUSY;
    	while (halErr == HAL_ERR_RESOURCE_BUSY)
    	{
    		halErr = hal_I2cSendByte(MAXIM_ADDR, REG_MAXIM_H_R_LEFT_VOL, H_R_MUTE);
    	}

    	if (halErr != HAL_ERR_NO)
        {
            AUD_ASSERT(FALSE, "%s line %d", __FILE__, __LINE__);
			return AUD_ERR_RESOURCE_BUSY;    
        }
    }
    else
    {
        // Main and side-tone volume
        halErr = HAL_ERR_RESOURCE_BUSY;
    	while (halErr == HAL_ERR_RESOURCE_BUSY)
    	{
    		halErr = hal_I2cSendByte(MAXIM_ADDR, REG_MAXIM_H_R_LEFT_VOL, regSpkLvl);
    	}

    	if (halErr != HAL_ERR_NO)
        {
            AUD_ASSERT(FALSE, "%s line %d", __FILE__, __LINE__);
			return AUD_ERR_RESOURCE_BUSY;    
        }

        halErr = HAL_ERR_RESOURCE_BUSY;
    	while (halErr == HAL_ERR_RESOURCE_BUSY)
    	{
    		halErr = hal_I2cSendByte(MAXIM_ADDR, REG_MAXIM_H_R_RIGHT_VOL, regSpkLvl);
    	}

    	if (halErr != HAL_ERR_NO)
        {
            AUD_ASSERT(FALSE, "%s line %d", __FILE__, __LINE__);
			return AUD_ERR_RESOURCE_BUSY;    
        }
    }

    if (cfg->level->micLevel == SND_MIC_MUTE)
    {
        halErr = HAL_ERR_RESOURCE_BUSY;
    	while (halErr == HAL_ERR_RESOURCE_BUSY)
    	{
    		halErr = hal_I2cSendByte(MAXIM_ADDR, REG_MAXIM_MIC_MODE, MIC_MUTE |EXT_MIC_INPUT | RESISTOR_BLAS_ENABLE);
    	}
        
    	if (halErr != HAL_ERR_NO)
        {
            AUD_ASSERT(FALSE, "%s line %d", __FILE__, __LINE__);
			return AUD_ERR_RESOURCE_BUSY;    
        }
    }
    else
    {
        halErr = HAL_ERR_RESOURCE_BUSY;
    	while (halErr == HAL_ERR_RESOURCE_BUSY)
    	{
    		halErr = hal_I2cSendByte(MAXIM_ADDR, REG_MAXIM_MIC_MODE, EXT_MIC_INPUT | RESISTOR_BLAS_ENABLE);
    	}

    	if (halErr != HAL_ERR_NO)
        {
            AUD_ASSERT(FALSE, "%s line %d", __FILE__, __LINE__);
			return AUD_ERR_RESOURCE_BUSY;    
        }
    }
    // Side volume controlled in the AIF
    hal_AifSetSideTone(cfg->level->sideLevel);

    // Tone generation volume
    // TODO implement if needed (instead of stoptone set newlevel in cfg
    // start tone
    //hal_AifSetToneLevel(cfg->toneLevel);

    return AUD_ERR_NO;
}



// =============================================================================
// aud_MaximStreamStart
// -----------------------------------------------------------------------------
/// Start the playing of a stream. 
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
PUBLIC AUD_ERR_T aud_MaximStreamStart(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    HAL_AIF_IF_T aifItf;

    // Get the I2S interface this driver instance is plugged onto:
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    aifItf = audioCfg[itf].parameter;


    if (g_audMaximPlaying)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }
    else
    {
        if (! g_audMaximOpened)
        {
            errStatus = aud_MaximOpen(stream->sampleRate, stream->voiceQuality,
                (stream->channelNb == HAL_AIF_MONO), FALSE, aifItf); // The AIF module is the master.
        }

        if (errStatus == AUD_ERR_NO)
        {
            errStatus = aud_MaximSetup(itf, cfg);
            if (errStatus == AUD_ERR_NO)
            {
                // Send the stream through the IFC
                if (hal_AifPlayStream(stream) != HAL_ERR_NO)
                {
                AUD_TRACE(AUD_INFO_TRC, 0,
                    "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
                    errStatus = AUD_ERR_RESOURCE_BUSY;
                }
                else
                {
                    g_audMaximPlaying = TRUE;
                }
            }
        }
        return errStatus;
    }
}



// =============================================================================
// aud_StreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_MaximStreamStop(SND_ITF_T itf)
{
    if (g_audMaximPlaying)
    {
        hal_AifStopPlay();
        g_audMaximPlaying = FALSE;
    }

    if (g_audMaximRecording)
    {
        hal_AifStopRecord();
        g_audMaximRecording = FALSE;
    }

    if (g_audMaximOpened)
    {
        return aud_MaximClose();
    }
    else
    {
        return AUD_ERR_NO;
    }
}

// =============================================================================
// aud_StreamPause
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
PUBLIC AUD_ERR_T aud_MaximStreamPause(SND_ITF_T itf, BOOL pause)
{
    switch (hal_AifPause(pause))
    {
        case HAL_ERR_NO:
        default:
            return AUD_ERR_NO;
            break;
    }
}


// =============================================================================
// aud_StreamRecord
// -----------------------------------------------------------------------------
/// Manage the recording of a stream. 
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
PUBLIC AUD_ERR_T aud_MaximStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    HAL_AIF_IF_T aifItf;
    // Get the I2S interface this driver instance is plugged onto:
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    aifItf = audioCfg[itf].parameter;


    if (g_audMaximRecording)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }
    else
    {
        if (!g_audMaximOpened)
        {
            errStatus = aud_MaximOpen(stream->sampleRate, stream->voiceQuality,
                (stream->channelNb == HAL_AIF_MONO),FALSE, aifItf); // The AIF module is the master.
        }

        if (errStatus == AUD_ERR_NO)
        {
            errStatus = aud_MaximSetup(itf, cfg);
            if (errStatus == AUD_ERR_NO)
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
                    g_audMaximRecording = TRUE;
                }
            }
        }
        return errStatus;
    }
    return AUD_ERR_NO;
}



// =============================================================================
// aud_MaximTone
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
/// @param attenuation The attenuation level of the tone generator
/// @param cfg The configuration set applied to the audio interface
/// @param start If \c TRUE, starts the playing of the tone.
///              If \c FALSE, stops the tone. 
///
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_MaximTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T        tone,
//                CONST SND_TONE_ATTENUATION_T attenuation,
                CONST AUD_DEVICE_CFG_T*             cfg,
                CONST BOOL                   start)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    HAL_AIF_IF_T aifItf;
    // Get the I2S interface this driver instance is plugged onto:
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    aifItf = audioCfg[itf].parameter;


    if (!g_audMaximOpened && start == TRUE)
    {
        // Open the maxim
        // FIXME Use the stream phantom !
        errStatus = aud_MaximOpen(HAL_AIF_FREQ_8000HZ, FALSE, TRUE, FALSE, aifItf); // The AIF module is the master.
    }
    
    if (start == FALSE)
    {
        hal_AifTone(0, 0, FALSE);
        if( !g_audMaximPlaying && !g_audMaximRecording)
        {
            aud_MaximClose();
        }
    }
    else
    {
        if (errStatus == AUD_ERR_NO)
        {
            errStatus = aud_MaximSetup(itf, cfg);
            if (errStatus == AUD_ERR_NO)
            {
                hal_AifTone(tone, cfg->level->toneLevel, start);
            }
        }
    }

    return AUD_ERR_NO;

}



// =============================================================================
// aud_MaximTonePause
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
PUBLIC AUD_ERR_T aud_MaximTonePause(SND_ITF_T itf, BOOL pause)
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
// aud_MaximCalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_MaximCalibUpdateValues(SND_ITF_T itf)
{
    // FIXME --> Open the i2c before ...
//    return aud_MaximSetup(itf, &g_audMaximLatestCfg);
    return HAL_ERR_NO;
}





