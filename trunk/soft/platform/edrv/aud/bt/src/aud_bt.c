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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/aud/bt/src/aud_bt.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-09-01 19:25:00 +0800 (Wed, 01 Sep 2010) $                     //   
//	$Revision: 1857 $                                                          //   
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

#include "aud_m.h"
#include "audp_debug.h"
#include "hal_error.h"
#include "aud_bt.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================
// =============================================================================
// AUD_BT_STATE_T
// -----------------------------------------------------------------------------
/// This type describes the actions currently being operated on the AUD driver.
// =============================================================================
typedef struct
{
    /// The driver is playing a stream.
    BOOL playing;

    /// The driver is recording a stream.
    BOOL recording;

    /// The driver is emitting a tone.
    BOOL toning;
} AUD_BT_STATE_T;


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
/// Opened status - All operation are described here. 
PRIVATE AUD_BT_STATE_T  g_audBtState
    
    = {FALSE, FALSE, FALSE};


// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// aud_BtSetup
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
PUBLIC AUD_ERR_T aud_BtSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg)
{
    // Nothing to be done for the BT chip ...
    return AUD_ERR_NO;
}
 

PRIVATE AUD_ERR_T aud_BtOpen(HAL_AIF_FREQ_T sampleRate, BOOL voice,
                             BOOL mono)
{
	HAL_AIF_CONFIG_T     aifCfg;
	HAL_AIF_SERIAL_CFG_T serialCfg;
	AUD_ERR_T errStatus = AUD_ERR_NO;
	
	if (voice)
	{
        AUD_ASSERT(sampleRate == HAL_AIF_FREQ_8000HZ,
                "AUD: BT: Voice mode only supports 8kHz frequency.");
        AUD_ASSERT(mono == TRUE,
                "AUD: BT: Voice mode is mono only.");
        // TODO add a bunch of assert/check
        
    	serialCfg.mode = HAL_SERIAL_MODE_VOICE;//HAL_SERIAL_MODE_I2S;	
    	serialCfg.aifIsMaster = TRUE; 	
    	serialCfg.lsbFirst  = FALSE;	
    	serialCfg.polarity  = FALSE;	
    	serialCfg.rxDelay   = HAL_AIF_RX_DELAY_2; // <-- 2 !!!	
    	serialCfg.txDelay   = HAL_AIF_TX_DELAY_1;//HAL_AIF_TX_DELAY_ALIGN;	
    	serialCfg.rxMode    = HAL_AIF_RX_MODE_STEREO_MONO_FROM_L; 	
    	serialCfg.txMode    = HAL_AIF_TX_MODE_MONO_STEREO_CHAN_L;	
    	
    	serialCfg.fs = 8000;	
    	serialCfg.bckLrckRatio = 31;	
    	serialCfg.invertBck = FALSE;	
    	serialCfg.outputHalfCycleDelay = FALSE; 	
    	serialCfg.inputHalfCycleDelay = FALSE; 	
    	serialCfg.enableBckOutGating = FALSE;
    	
    	aifCfg.interface = HAL_AIF_IF_SERIAL; // FIXME ?
    	aifCfg.serialCfg = &serialCfg;
    	aifCfg.channelNb = HAL_AIF_MONO;
    	aifCfg.sampleRate = HAL_AIF_FREQ_8000HZ;
    }
    else
    {
        aifCfg.interface = HAL_AIF_IF_PARALLEL_STEREO;
        aifCfg.serialCfg    = NULL;
        aifCfg.sampleRate   = sampleRate;
        aifCfg.channelNb    = mono ? HAL_AIF_MONO : HAL_AIF_STEREO;
    }
	
	errStatus = hal_AifOpen(&aifCfg);
	g_audBtState.playing = TRUE;
	SXS_TRACE(TSTDOUT,"AUD: BT: hal_AifOpen %d ",errStatus);
	return errStatus;
}


// =============================================================================
// aud_BtStreamStart
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
PUBLIC AUD_ERR_T aud_BtStreamStart(
                        SND_ITF_T itf,
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
	AUD_ERR_T errStatus = AUD_ERR_NO;
	
	if (g_audBtState.playing)
	{
	    AUD_TRACE(TSTDOUT, 0,
	        "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
	    return AUD_ERR_RESOURCE_BUSY;
	}
	else
	{
		errStatus = aud_BtOpen(stream->sampleRate, stream->voiceQuality,
                               (stream->channelNb == HAL_AIF_MONO));
        if (AUD_ERR_NO == errStatus)
        {
            errStatus = aud_BtSetup(itf, cfg);
            if (AUD_ERR_NO == errStatus)
            {
    			if (hal_AifPlayStream(stream) != HAL_ERR_NO)
    			{
    				AUD_TRACE(TSTDOUT, 0,
    				"AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
    				errStatus = AUD_ERR_RESOURCE_BUSY;
    				aud_BtStreamStop(SND_ITF_BLUETOOTH);
    			}
    		}
		}
		SXS_TRACE(TSTDOUT,"AUD: BT: hal_AifPlayStream %d",errStatus);
		return errStatus;
	}
}



// =============================================================================
// aud_BtStreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_BtStreamStop(SND_ITF_T itf)
{
	if (TRUE == g_audBtState.recording)
	{
		hal_AifStopRecord();
		g_audBtState.recording = FALSE;
	}
	if (TRUE == g_audBtState.playing)
	{
		hal_AifStopPlay();
		g_audBtState.playing = FALSE;
	}
	if (!g_audBtState.recording && !g_audBtState.playing)
	{
		hal_AifClose();
	}
    
    return AUD_ERR_NO;
}

// =============================================================================
// aud_BtStreamPause
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
PUBLIC AUD_ERR_T aud_BtStreamPause(SND_ITF_T itf, BOOL pause)
{
	switch (hal_AifPause(pause))
	{
	    case AUD_ERR_NO:
	    default:
	        return AUD_ERR_NO;
	        break;
	}
}


// =============================================================================
// aud_BtStreamRecord
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
PUBLIC AUD_ERR_T aud_BtStreamRecord(
                        SND_ITF_T itf,
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
	AUD_ERR_T errStatus = AUD_ERR_NO;
	
	if (g_audBtState.recording)
	{
	    AUD_TRACE(TSTDOUT, 0,
	        "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
	    return AUD_ERR_RESOURCE_BUSY;
	}
	else
	{
		 g_audBtState.recording = TRUE;
		 if (hal_AifRecordStream(stream) != HAL_ERR_NO)
	        {
	            AUD_TRACE(TSTDOUT, 0,
	                "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
	            errStatus = AUD_ERR_RESOURCE_BUSY;
	            aud_BtStreamStop(SND_ITF_BLUETOOTH);
	        }
	}
	SXS_TRACE(TSTDOUT,"AUD: BT: hal_AifRecordStream %d ",errStatus);
	return errStatus;
}



// =============================================================================
// aud_BtTone
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
PUBLIC AUD_ERR_T aud_BtTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T        tone,
                CONST AUD_DEVICE_CFG_T*             cfg,
                CONST BOOL                   start)
{
	//AUD_ASSERT(FALSE, "Attempt to use an inexisting audio device, %s, %d", __FILE__, __LINE__);    return AUD_ERR_NO;
    return AUD_ERR_NO;
}



// =============================================================================
// aud_BtTonePause
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
PUBLIC AUD_ERR_T aud_BtTonePause(SND_ITF_T itf, BOOL pause)
{
	//AUD_ASSERT(FALSE, "Attempt to use an inexisting audio device, %s, %d", __FILE__, __LINE__);
    return AUD_ERR_NO;
}


// =============================================================================
// aud_BtCalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_BtCalibUpdateValues(SND_ITF_T itf)
{
    return AUD_ERR_NO;
}




