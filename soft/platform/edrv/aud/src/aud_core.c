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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/aud/src/aud_core.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2011-06-30 11:50:06 +0800 (Thu, 30 Jun 2011) $                     //   
//	$Revision: 8640 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file                                                                     //
/// That file describes the audio driver interface.                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "tgt_aud_cfg.h"

#include "aud_m.h"

#include "hal_aif.h"
#include "hal_sys.h"
#include "hal_rda_audio.h"

#include "audp_debug.h"

#define AUD_MUTEX_FOR_IRQ 1

#if (AUD_MUTEX_FOR_IRQ == 1)
#include "hal_timers.h"
#include "sxr_tls.h"

extern VOID hal_GetK1(UINT32* k1);
#else // (AUD_MUTEX_FOR_IRQ != 1)
#include "sxr_sbx.h"
#endif // (AUD_MUTEX_FOR_IRQ != 1)

#include "sxr_mem.h"


// =============================================================================
//  MACROS
// =============================================================================

#if  (AUD_MUTEX_FOR_IRQ == 1)
#define AUD_TIME_SEM_WAIT_INTERVAL (5 MS_WAITING)
#endif // (AUD_MUTEX_FOR_IRQ == 1)

#define SND_ITF_HAS_PHY_SPK(itf) \
    (itf == SND_ITF_RECEIVER || itf == SND_ITF_EAR_PIECE || itf == SND_ITF_LOUD_SPEAKER)


// =============================================================================
//  GLOBALS
// =============================================================================

PRIVATE AUD_TEST_T  g_audRunningTest        = AUD_TEST_NO;
PRIVATE SND_ITF_T   g_audEnabledRecordItf   = SND_ITF_NONE;
PRIVATE SND_ITF_T   g_audEnabledPlayItf     = SND_ITF_NONE;
PRIVATE SND_ITF_T   g_audEnabledToneItf     = SND_ITF_NONE;

#if (AUD_MUTEX_FOR_IRQ == 1)
/// status being configured
PRIVATE VOLATILE BOOL g_audBeingConfigured  = FALSE;
#else // (AUD_MUTEX_FOR_IRQ != 1)
PRIVATE BYTE        g_audSemaphore          = 0xFF;
#endif // (AUD_MUTEX_FOR_IRQ != 1)

/// Current output device for FM/TV
PRIVATE SND_ITF_T   g_audCurOutputDevice    = SND_ITF_NONE;


// =============================================================================
//  FUNCTIONS
// =============================================================================

// FIXME Remove this deprecated function
PROTECTED VOID aud_Init()
{
}


// =============================================================================
// aud_SemTake
// -----------------------------------------------------------------------------
/// Take the semaphore protecting AUD shared variables.
///
/// @return TRUE if the semaphore was free, and thus taken by that call, or
/// FALSE if the resource is not available.
// =============================================================================
PRIVATE BOOL aud_SemTake(VOID)
{
#if (AUD_MUTEX_FOR_IRQ == 1)
    UINT32 k1 = 0;
    hal_GetK1(&k1);
    
    UINT32 status = hal_SysEnterCriticalSection();
    while (g_audBeingConfigured)
    {
        if (k1 > 0)
        {
            // Interrupt can NOT sleep or wait
            hal_SysExitCriticalSection(status);
            return FALSE;
        }
        else
        {
            // Task can sleep
            sxr_Sleep(AUD_TIME_SEM_WAIT_INTERVAL);
        }
    }
    g_audBeingConfigured = TRUE;
    hal_SysExitCriticalSection(status);
#else // (AUD_MUTEX_FOR_IRQ != 1)
    // Create the semaphore if it doesn't exist.
    UINT32 status = hal_SysEnterCriticalSection();
    if (g_audSemaphore == 0xFF)
    {
        g_audSemaphore = sxr_NewSemaphore(1); 
    }
    hal_SysExitCriticalSection(status);

    sxr_TakeSemaphore(g_audSemaphore);
#endif // (AUD_MUTEX_FOR_IRQ != 1)

    return TRUE;
}


// =============================================================================
// aud_SemFree
// -----------------------------------------------------------------------------
/// Free the semaphore protecting AUD shared variables.
// =============================================================================
PRIVATE VOID aud_SemFree(VOID)
{
#if (AUD_MUTEX_FOR_IRQ == 1)
    g_audBeingConfigured = FALSE;
#else // (AUD_MUTEX_FOR_IRQ != 1)
    sxr_ReleaseSemaphore(g_audSemaphore);
#endif // (AUD_MUTEX_FOR_IRQ != 1)
}


// =============================================================================
// aud_Setup
// -----------------------------------------------------------------------------
/// Configure the audio. 
/// 
/// This functions configures an audio interface: gain for the side tone and  
/// the speaker, input selection for the microphone and output selection for 
/// the speaker, enabling of the microphone ... It can be used to change
/// 'on the fly' the input or output used to play/record a stream on the 
/// same interface, but cannot be used to change the interface during a
/// playback or a record.
/// 
/// @param itf Interface number of the interface to setup.
/// @param cfg The configuration set applied to the audio interface. See 
/// #AUD_LEVEL_T for more details.
/// @return #AUD_ERR_NO if it can execute the configuration.
// =============================================================================
PUBLIC AUD_ERR_T aud_Setup(SND_ITF_T itf, CONST AUD_LEVEL_T* cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
                "Exist on this target");

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_Setup begin itf: %d", itf);

    if (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
        return AUD_ERR_RESOURCE_BUSY;
    }

    // Only update a configuration if AUD is being used
    // Otherwise, do nothing.
    if (g_audEnabledRecordItf != SND_ITF_NONE || g_audEnabledPlayItf != SND_ITF_NONE || g_audEnabledToneItf != SND_ITF_NONE)
    {
        // Only change the AUD properties of the interface if, and only if
        // this interface is the interface being used.
        // FIXME The MMI is bogous and do not respect this constraint:
        // Fix the MMI and reenable this test. (We use a flag to avoid
        // to comment or #if 0 those line)
#ifdef THE_MMI_COMPLY_TO_AUD_SPEC
        if (((g_audEnabledRecordItf == itf) || (g_audEnabledRecordItf   == SND_ITF_NONE))
         && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE))
         && ((g_audEnabledToneItf   == itf) || (g_audEnabledToneItf     == SND_ITF_NONE)))
        {
            CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
            AUD_DEVICE_CFG_T deviceCfg;
            deviceCfg.spkSel    = audioCfg[itf].spkSel;
            deviceCfg.spkType   = audioCfg[itf].spkType;
            deviceCfg.micSel    = audioCfg[itf].micSel;
            deviceCfg.level     = cfg;

            if (!SND_ITF_HAS_PHY_SPK(itf))
            {
                if (SND_ITF_HAS_PHY_SPK(g_audCurOutputDevice))
                {
                    deviceCfg.spkSel = audioCfg[g_audCurOutputDevice].spkSel;
                }
            }

            if (audioCfg[itf].setup)
            {
                // There is a setup function to call
                // for that interface
                errStatus = (audioCfg[itf].setup(itf, &deviceCfg));
            }
            else
            {
                // There is no driver for that interface,
                // ie. the interface does not exist.
                errStatus =  AUD_ERR_NO_ITF;
            }
        }
        else
        {
            // Another interface is using the audio
            // ignore
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }

#else
        {
            CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
            AUD_DEVICE_CFG_T deviceCfg;

            if(g_audEnabledRecordItf!=SND_ITF_NONE)
            {
                g_audEnabledRecordItf=itf;
            }
            
            if(g_audEnabledPlayItf!=SND_ITF_NONE)
            {
                g_audEnabledPlayItf=itf;
            }
            
            if(g_audEnabledToneItf!=SND_ITF_NONE)
            {
                g_audEnabledToneItf=itf;
            }

            deviceCfg.spkSel    = audioCfg[itf].spkSel;
            deviceCfg.spkType   = audioCfg[itf].spkType;
            deviceCfg.micSel    = audioCfg[itf].micSel;
            deviceCfg.level     = cfg;

            if (!SND_ITF_HAS_PHY_SPK(itf))
            {
                if (SND_ITF_HAS_PHY_SPK(g_audCurOutputDevice))
                {
                    deviceCfg.spkSel = audioCfg[g_audCurOutputDevice].spkSel;
                }
            }

            if (audioCfg[itf].setup)
            {
                // There is a setup function to call
                // for that interface
                errStatus = (audioCfg[itf].setup(itf, &deviceCfg));
            }
            else
            {
                // There is no driver for that interface,
                // ie. the interface does not exist.
                errStatus =  AUD_ERR_NO_ITF;
            }
        }

#endif

    }
    else
    {
        // Nothing to do in that case, this can be 
        // interpreted as a no error call to the function.
        errStatus = AUD_ERR_NO;
    }

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_Setup end itf: %d", itf);
    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_StreamStart
// -----------------------------------------------------------------------------
/// Start the playing of a stream. 
/// 
/// This function  outputs the audio stream on the audio interface specified as 
/// a parameter, on the output selected in the AUD_LEVEL_T cfg parameter. \n
/// When the buffer runs out, the playing will wrap at the 
/// beginning of the buffer. If defined in the HAL_AIF_STREAM_T structure,
/// a user function can be called at the middle or at the end of the playback.
/// 
/// @param itf Interface on which play that stream
/// @param stream Stream to play. Handler called at the middle and end of the 
/// buffer are defined there. The length of a stream must be a multiple 
/// of 16 bytes.
/// @param cfg The configuration set applied to the audio interface
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command,
///         #AUD_ERR_NO it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_StreamStart(
                        CONST SND_ITF_T      itf,
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_LEVEL_T *       cfg)
{
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    AUD_DEVICE_CFG_T deviceCfg;
    AUD_ERR_T errStatus = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
               "Exist on this target");
    AUD_ASSERT(stream, "Stream == 0");
    AUD_ASSERT(stream->startAddress, "Stream address == NULL");
    AUD_ASSERT(stream->length, "Stream length == 0");

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_StreamStart itf: %d", itf);

    if (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (((g_audEnabledRecordItf == itf)             || (g_audEnabledRecordItf   == SND_ITF_NONE))
     &&  (g_audEnabledPlayItf   == SND_ITF_NONE)
     && ((g_audEnabledToneItf   == itf)             || (g_audEnabledToneItf     == SND_ITF_NONE)))
    {
        // Stop tone first
        if (g_audEnabledToneItf != SND_ITF_NONE)
        {
            if (audioCfg[itf].tone)
            {
                // Only the start fields needs to be specified, 
                // set to FALSE to stop the tone generation.
                errStatus = (audioCfg[itf].tone(itf, 0, NULL, FALSE));
                if (errStatus == AUD_ERR_NO)
                {
                    // Stop the tone
                    // Set the new used interface
                    g_audEnabledToneItf = SND_ITF_NONE;
                }
            }
            else
            {
                errStatus = AUD_ERR_NO_ITF;
            }
        }

        if (errStatus == AUD_ERR_NO)
        {
            deviceCfg.spkSel  = audioCfg[itf].spkSel;
            deviceCfg.spkType = audioCfg[itf].spkType;
            deviceCfg.micSel  = audioCfg[itf].micSel;
            deviceCfg.level   = cfg;

            if (!SND_ITF_HAS_PHY_SPK(itf))
            {
                if (SND_ITF_HAS_PHY_SPK(g_audCurOutputDevice))
                {
                    deviceCfg.spkSel = audioCfg[g_audCurOutputDevice].spkSel;
                }
            }

            if (audioCfg[itf].play)
            {
                // Interface driver exist: play
                errStatus = (audioCfg[itf].play(itf, stream, &deviceCfg));
                if (errStatus == AUD_ERR_NO)
                {
                    // The play actually started, set the (new) used interface
                    g_audEnabledPlayItf = itf;
                }
            }
            else
            {
                // No interface
                errStatus = AUD_ERR_NO_ITF;
            }
        }
    }
    else
    {
        // Interface is busy
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledPlayItf);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }

    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_StreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param itf Interface on which to stop the playing.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_StreamStop(CONST SND_ITF_T itf)
{
    // Some applications will try to stop an audio stream in mutiple paths,
    // then it is possible for them to stop a SND_ITF_NONE interface.
    if (itf == SND_ITF_NONE)
    {
        return AUD_ERR_NO;
    }

    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    AUD_ERR_T errStatus = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");
    AUD_TRACE(AUD_INFO_TRC, 0, "aud_StreamStop itf: %d", itf);

    if (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if ((g_audEnabledRecordItf != SND_ITF_NONE) || (g_audEnabledPlayItf != SND_ITF_NONE) || (g_audEnabledToneItf != SND_ITF_NONE))
    {
        if (((g_audEnabledRecordItf == itf) || (g_audEnabledRecordItf   == SND_ITF_NONE))
         && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE)) 
         && ((g_audEnabledToneItf   == itf) || (g_audEnabledToneItf     == SND_ITF_NONE)))
        {
            // Stop tone first
            if (g_audEnabledToneItf != SND_ITF_NONE)
            {
                if (audioCfg[itf].tone)
                {
                    // Only the start fields needs to be specified, 
                    // set to FALSE to stop the tone generation.
                    errStatus = (audioCfg[itf].tone(itf, 0, NULL, FALSE));
                    if (errStatus == AUD_ERR_NO)
                    {
                        // Stop the tone
                        // Set the new used interface
                        g_audEnabledToneItf = SND_ITF_NONE;
                    }
                }
                else
                {
                    errStatus = AUD_ERR_NO_ITF;
                }
            }

            if (errStatus == AUD_ERR_NO)
            {
                // Stop stream then
                if (audioCfg[itf].stop)
                {
                    errStatus = audioCfg[itf].stop(itf);
                    if (errStatus == AUD_ERR_NO)
                    {
                        // Freeing currently used interface
                        // Set the new freed interface
                        g_audEnabledRecordItf   = SND_ITF_NONE;
                        g_audEnabledPlayItf     = SND_ITF_NONE;
                        // Set current output device to default
                        g_audCurOutputDevice = SND_ITF_NONE;
                    }
                }
                else
                {
                    errStatus = AUD_ERR_NO_ITF;
                }
            }
        }
        else
        {
            // Another interface is in use. Ignore
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledPlayItf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }
    }
    else
    {
        // Nothing to stop, not illegal (though useless)
        errStatus = AUD_ERR_NO;
    }

    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_StreamPause
// -----------------------------------------------------------------------------
/// This function pauses or resume the audio stream playback or/and record.
/// If the function returns #AUD_ERR_RESOURCE_BUSY, it means that the driver 
/// is busy with another audio command.
///
/// @param itf Interface on which to stop the playing.
/// @param pause If \c TRUE, pauses the stream. If \c FALSE, resumes a paused
/// stream.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_StreamPause(CONST SND_ITF_T itf, CONST BOOL pause)
{
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    AUD_ERR_T errStatus = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
                "Exist on this target");   
    AUD_TRACE(AUD_INFO_TRC, 0, "aud_StreamPause itf: %d ", itf);
    if (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if ((g_audEnabledRecordItf != SND_ITF_NONE) || (g_audEnabledPlayItf != SND_ITF_NONE) || (g_audEnabledToneItf != SND_ITF_NONE))
    {
        if (((g_audEnabledRecordItf == itf) || (g_audEnabledRecordItf   == SND_ITF_NONE))
         && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE)) 
         && ((g_audEnabledToneItf   == itf) || (g_audEnabledToneItf     == SND_ITF_NONE)))
        {
            // Pause current interface
            if (audioCfg[itf].pause)
            {
                errStatus = (audioCfg[itf].pause(itf, pause));
            }
            else
            {
                errStatus = AUD_ERR_NO_ITF;
            }

        }
        else
        {
            // Another interface is in use. Ignore
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledPlayItf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }
    }
    else
    {
        // Nothing to pause, not illegal (though useless)
        errStatus = AUD_ERR_NO;
    }

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_StreamPause itf: %d, errStatus: %d", itf,errStatus);

    aud_SemFree();
    return errStatus;
}



// =============================================================================
// aud_StreamRecord
// -----------------------------------------------------------------------------
/// Manage the recording of a stream. 
/// This function records the audio stream from the audio interface specified as 
/// a parameter, from the input selected in the AUD_LEVEL_T cfg parameter.\n
/// In normal operation, when the buffer runs out, the recording will wrap at the 
/// beginning of the buffer. If defined in the HAL_AIF_STREAM_T structure,
/// a user function can be called at the middle or at the end of the record.
///
/// @param itf Interface whose PCM flow is recorded.
/// @param stream Stream recorded (describe the buffer and the possible interrupt
/// of middle-end buffer handlers. The length of a stream must be a multiple 
/// of 16 bytes.
/// @param cfg The configuration set applied to the audio interface.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver cannot handle the stream,
///         #AUD_ERR_NO if the stream ca be recorded.
// =============================================================================
PUBLIC AUD_ERR_T aud_StreamRecord(
                        CONST SND_ITF_T      itf,
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_LEVEL_T *       cfg)
{
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    AUD_DEVICE_CFG_T deviceCfg;
    AUD_ERR_T errStatus = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
                "Exist on this target");

    if (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if ((g_audEnabledRecordItf  == SND_ITF_NONE)
     && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE)) 
     && ((g_audEnabledToneItf   == itf) || (g_audEnabledToneItf     == SND_ITF_NONE)))
    {
        // Record
        deviceCfg.spkSel    = audioCfg[itf].spkSel;
        deviceCfg.spkType   = audioCfg[itf].spkType;
        deviceCfg.micSel    = audioCfg[itf].micSel;
        deviceCfg.level     = cfg;

        if (!SND_ITF_HAS_PHY_SPK(itf))
        {
            if (SND_ITF_HAS_PHY_SPK(g_audCurOutputDevice))
            {
                deviceCfg.spkSel = audioCfg[g_audCurOutputDevice].spkSel;
            }
        }

        if (audioCfg[itf].record)
        {
            errStatus = (audioCfg[itf].record(itf, stream, &deviceCfg));
            if (errStatus == AUD_ERR_NO)
            {
                // Start recording on the (new) enabled interface
                // Set the (new) used interface
                g_audEnabledRecordItf = itf;
            }
        }
        else
        {
            errStatus = AUD_ERR_NO_ITF;
        }
    }
    else
    {
        // AUD is activated on another interface
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledRecordItf);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }

    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_ToneStart
// -----------------------------------------------------------------------------
/// Manage the playing of a tone: DTMF or Comfort Tone. 
///
/// This function starts the output of a tone generated in the audio 
/// module on the selected interface. It a stream is being played on the same 
/// interface, the stream will continue to be played the  its sound won't be
/// output. It will be replaced instead by the generated tone.
///
/// You can call this function several times without calling the 
/// #aud_ToneStop function or the #aud_TonePause function in 
/// between, if you just need to change the attenuation or the tone type. \n
/// If the function returns #AUD_ERR_RESOURCE_BUSY, it means that the driver is 
/// busy with an other audio command.
///
/// @param itf AUD interface on which to play a tone.
/// @param tone The tone to generate.
/// @param cfg The configuration set applied to the audio interface.
///
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_ToneStart(
                CONST SND_ITF_T         itf,
                CONST SND_TONE_TYPE_T   tone,
                CONST AUD_LEVEL_T*      cfg)
{
    CONST AUD_ITF_CFG_T*    audioCfg = tgt_GetAudConfig();
    AUD_DEVICE_CFG_T        deviceCfg;
    AUD_ERR_T               errStatus;
    
    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");    
    AUD_TRACE(AUD_INFO_TRC, 0, "aud_ToneStart itf: %d", itf);
    if (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (((g_audEnabledRecordItf == itf) || (g_audEnabledRecordItf   == SND_ITF_NONE))
     && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE)) 
     && ((g_audEnabledToneItf   == SND_ITF_NONE)))
    {
        deviceCfg.spkSel    = audioCfg[itf].spkSel;
        deviceCfg.spkType   = audioCfg[itf].spkType;
        deviceCfg.micSel    = audioCfg[itf].micSel;
        deviceCfg.level     = cfg;

        if (audioCfg[itf].tone)
        {
            errStatus = (audioCfg[itf].tone(itf, tone, &deviceCfg, TRUE));
            if (errStatus == AUD_ERR_NO)
            {
                // Start toning on the (new) interface
                // Set the new used interface
                g_audEnabledToneItf = itf;
            }
        }
        else
        {
            errStatus = AUD_ERR_NO_ITF;
        }
    }
    else
    {
        // AUD is activated on another interface
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledToneItf);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }

    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_ToneStop
// -----------------------------------------------------------------------------
/// Stop the tone generation.
/// If the function returns  #AUD_ERR_RESOURCE_BUSY, it means that the driver 
/// is busy with an other audio command.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_ToneStop(CONST SND_ITF_T itf)
{
    CONST AUD_ITF_CFG_T*    audioCfg    = tgt_GetAudConfig();
    AUD_ERR_T               errStatus   = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");
    
    AUD_TRACE(AUD_INFO_TRC, 0, "aud_ToneStop itf: %d", itf);
    
    if (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (g_audEnabledToneItf != SND_ITF_NONE)
    {
        if (g_audEnabledToneItf    == itf)
        {
            if (audioCfg[itf].tone)
            {
                // Only the start fields needs to be specified, 
                // set to FALSE to stop the tone generation.
                errStatus = (audioCfg[itf].tone(itf, 0, NULL, FALSE));
                if (errStatus == AUD_ERR_NO)
                {
                    // Stop the tone
                    // Set the new used interface
                    g_audEnabledToneItf = SND_ITF_NONE;
                }
            }
            else
            {
                errStatus = AUD_ERR_NO_ITF;
            }
        }
        else
        {
            // Another AUD interface is activated
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledToneItf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }
    }
    else
    {
        // Nothing to stop, not illegal (useless, though)
        errStatus = AUD_ERR_NO;
    }

    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_TonePause
// -----------------------------------------------------------------------------
/// This function pauses or resume a tone.
/// If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param itf Interface on which to stop the playing.
/// @param pause If \c TRUE, pauses the tone. If \c FALSE, resumes a paused
/// tone.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_TonePause(CONST SND_ITF_T itf, CONST BOOL pause)
{
    CONST AUD_ITF_CFG_T*    audioCfg    = tgt_GetAudConfig();
    AUD_ERR_T               errStatus   = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");

    if (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (g_audEnabledToneItf != SND_ITF_NONE)
    {
        if (g_audEnabledToneItf    == itf)
        {
            // Pause the tone
            if (audioCfg[itf].tonePause)
            {
                errStatus = (audioCfg[itf].tonePause(itf, pause));
            }
            else
            {
                errStatus = AUD_ERR_NO_ITF;
            }
        }
        else
        {
            // Another AUD interface is activated
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledToneItf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }
    }
    else
    {
        // Nothing to pause, but not illegal (useless though)
        errStatus = AUD_ERR_NO;
    }

    aud_SemFree();
    return errStatus;
}

// 1 second for 8K Hz sample rate (16 bits per sample)
#define AUD_TEST_DEPTH  16000
/// TODO: Use a malloc and the test 'state machine' to 
/// handle the free.
/// Local buffer aimed at test modes.
/// They will be filled with 0, so that nothing is
/// played and we can only hear the side test.
UINT8 *g_audRecordNull = NULL;


// =============================================================================
// aud_TestModeSetup
// -----------------------------------------------------------------------------
/// Enable a test mode.
/// This function enables a test mode, enabling for example the side test
/// or the DAIs.
/// The audio interface must be enabled when this function is called.
/// This function disables the AIF (audio interface), sets the audio 
/// interface in the mode specified, and re-enables the AIF.
/// See the <B> Application Note 0018 </B> for more details on the DAI 
/// functionality and see 3GPP TS 44014-500 page 40 for the description of the 
/// DAI test interface.
/// 
/// @param itf AUD interface to test.
/// @param stream Depending on the test, this parameter can be ignored, or
/// used to configure it. (This is the case for the side test). This 
/// sets things relative to the streaming (sampling rate, etc)
/// @param cfg Depending on the test, this parameter can be ignored, or
/// used to configure it. (This is the case for the side test). Configure
/// the audio interface.
/// @param mode Test mode to set.
// =============================================================================
PUBLIC VOID aud_TestModeSetup(  CONST SND_ITF_T         itf,
                                CONST HAL_AIF_STREAM_T* stream,
                                CONST AUD_LEVEL_T *     cfg,
                                AUD_TEST_T              mode)
{
    // FIXME How to protect that state machine (It is one, nope ?)
    switch (mode)
    {
        case AUD_TEST_SIDE_TEST:
        {
            // Play 0 to add side test on it.
            UINT32 i;

            g_audRecordNull = sxr_HMalloc(AUD_TEST_DEPTH);
            if (g_audRecordNull == NULL)
            {
                return;
            }

            for (i=0 ; i<AUD_TEST_DEPTH; i++)
            {
                // Clear test buffer.
                g_audRecordNull[i] = 0;
            }

            // Copy configuration from parameter, 
            // but play null buffer.
            HAL_AIF_STREAM_T sideTestStream = *stream;
            sideTestStream.startAddress = (UINT32*)g_audRecordNull;
            sideTestStream.length       = AUD_TEST_DEPTH;

            aud_StreamStart(itf, &sideTestStream, cfg);
            sxr_Sleep(200 MS_WAITING);
            // The sound is replayed after 800ms (1s - 200ms)
            aud_StreamRecord(itf, &sideTestStream, cfg);
            g_audRunningTest = AUD_TEST_SIDE_TEST;
            break;
        }

        case AUD_TEST_NO:
            switch (g_audRunningTest)
            {
                case AUD_TEST_SIDE_TEST:
                    if (g_audRecordNull != NULL)
                    {
                        sxr_HFree(g_audRecordNull);
                        g_audRecordNull = NULL;
                    }
                    aud_StreamStop(itf);
                    g_audRunningTest = AUD_TEST_NO;
                    break;

                default:
                    break;
            }

        default:
            break;
    }
}


// =============================================================================
// aud_CalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_CalibUpdateValues(CONST SND_ITF_T itf)
{
    CONST AUD_ITF_CFG_T*    audioCfg    = tgt_GetAudConfig();
    AUD_ERR_T               errStatus   = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");

    if (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (((g_audEnabledRecordItf == SND_ITF_NONE)    && (g_audEnabledPlayItf == SND_ITF_NONE))
     || ((g_audEnabledRecordItf == SND_ITF_NONE)    && (g_audEnabledPlayItf == itf ))
     || ((g_audEnabledRecordItf == itf)             && (g_audEnabledPlayItf == SND_ITF_NONE ))
     || ((g_audEnabledRecordItf == itf)             && (g_audEnabledPlayItf == itf )))
    {
        if (audioCfg[itf].calibUpdate)
        {
            errStatus = (audioCfg[itf].calibUpdate(itf));
        }
        else
        {
            errStatus = AUD_ERR_NO_ITF;
        }
    }
    else
    {
        // Another interface is activated, don't mess with it
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledPlayItf);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }

    aud_SemFree();
    return errStatus;
}


PUBLIC VOID aud_SetCurOutputDevice(SND_ITF_T itf)
{
    if (itf == SND_ITF_NONE || SND_ITF_HAS_PHY_SPK(itf))
    {
        g_audCurOutputDevice = itf;
    }
}


PUBLIC SND_ITF_T aud_GetCurOutputDevice(VOID)
{
    return g_audCurOutputDevice;
}


#ifdef AUD_MUTE_FOR_BT_WORKAROUND
// =============================================================================
// aud_MuteOutputDevice
// -----------------------------------------------------------------------------
/// Power on/off the audio when it is playing.
/// The function is called by MCI only to redirect the audio output to the bluetooth device.
/// The function might be removed if MCI changes its way to switch between bluetooth and normal codec.
/// 
/// @param on TRUE if power on, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_MuteOutputDevice(BOOL mute)
{
    while (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", aud_MuteOutputDevice: Busy");
        sxr_Sleep(AUD_TIME_RETRY);
    }

    extern VOID hal_AudMuteOutput(BOOL);
    hal_AudMuteOutput(mute);

    aud_SemFree();
}
#endif // AUD_MUTE_FOR_BT_WORKAROUND


#if defined(RECV_USE_SPK_LINE) || defined(AUD_EXT_PA_ON_WITH_LCD)
// =============================================================================
// aud_LcdPowerOnCallback
// -----------------------------------------------------------------------------
/// Power on some audio components along with LCD.
/// The function is called by LCDD only.
/// 
/// @param on TRUE if power on, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_LcdPowerOnCallback(VOID)
{
    while (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", aud_LcdPowerOnCallback: Busy");
        sxr_Sleep(AUD_TIME_RETRY);
    }

    extern VOID hal_AudLcdPowerOnCallback(VOID);
    hal_AudLcdPowerOnCallback();

    aud_SemFree();
}

// =============================================================================
// aud_LcdPowerOffCallback
// -----------------------------------------------------------------------------
/// Restore the power down state for some audio components.
/// The function is called by LCDD only.
/// 
/// @param on TRUE if power on, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_LcdPowerOffCallback(VOID)
{
    while (!aud_SemTake())
    {
        // AUD busy with something else
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", aud_LcdPowerOffCallback: Busy");
        sxr_Sleep(AUD_TIME_RETRY);
    }

    extern VOID hal_AudLcdPowerOffCallback(VOID);
    hal_AudLcdPowerOffCallback();

    aud_SemFree();
}
#endif // RECV_USE_SPK_LINE || AUD_EXT_PA_ON_WITH_LCD


///  @} <- End of the audio group 




