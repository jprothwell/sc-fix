/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vpp_speech.h                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This file defines VPP Speech API structures and functions.        */
/*                                                                       */
/*************************************************************************/
/// 
///   @file vpp_speech.h
///   This file defines VPP Speech API structures and functions.
/// 

#ifndef VPP_SPEECH_H
#define VPP_SPEECH_H

#include "cs_types.h"
#include "hal_voc.h"
#include "hal_speech.h"

/// 
///  @defgroup vpp_speech VPP Speech API
///  @par
///  This document describes the characteristics of the VPP Speech module. 
///     
///  @par Processing configuration :
///  @par
///         The VPP Speech module can be used in different services, such as the GSM Speech during
///         a call (conversion from and to coded frames), the audio play and the audio record.
///         The VPP Speech module can execute two distinct GSM Speech coded frame operations:
///         1) <c> encode to </c> 2) <c> decode from </c>. Coded stands for one of the vocoder
///         modes specified in the GSM Specification (FR, HR, EFR or AMR). VPP Speech can be
///         configured to execute one of them (encode only or decode only) or both (encode one
///         frame then decode one frame). It can also be selected whether the frame operation
///         is triggered (started) on a software-generated event (by XCPU) or on a hardware-
///         generated event (by IFC).
///
///  @par Resource management :
///  @par
///         The VPP Speech module needs to be first opened and configured using function 
///         <b> \c vpp_SpeechOpen </b>. This function will also set the audio configuration
///         to a default value. When the module is no longer needed it should be closed using 
///         <b> \c vpp_SpeechClose </b>. Not closing the module would not free the VoC resource 
///         for other applications.
///
///  @par Audio configuration :
///  @par
///         The audio configuration function <b> \c vpp_SpeechAudioCfg </b> sets the echo 
///         cancellation processing, the anti-distorsion filtering and the digital gain.
///
///  @par Buffer accessing :
///  @par
///         The addresses of the buffers are returned by <b> \c vpp_SpeechGetXxxBuffer </b>. 
///
///@{

// ============================================================================
// Types
// ============================================================================

// ============================================================================
// VPP_SPEECH_WAKEUP_MODE_T
// ----------------------------------------------------------------------------
/// Defines the order in which the Speech task are executed an the way these 
/// tasks are started
// ============================================================================
typedef enum
{
    /// No wakeup enabled
    VPP_SPEECH_WAKEUP_NO = 0,
    /// Record on software event (XCPU)
    VPP_SPEECH_WAKEUP_SW_ENC,
    /// Play on software event (XCPU)
    VPP_SPEECH_WAKEUP_SW_DEC,
    /// Speech on software event (XCPU)
    VPP_SPEECH_WAKEUP_SW_DECENC,
    /// Record on hardware event (IFC)
    VPP_SPEECH_WAKEUP_HW_ENC,
    /// Play on hardware event (IFC)
    VPP_SPEECH_WAKEUP_HW_DEC,
    /// Record on hardware event (IFC)
    VPP_SPEECH_WAKEUP_HW_DECENC
            
} VPP_SPEECH_WAKEUP_MODE_T;

// ============================================================================
// VPP_SPEECH_MUTE_T
// ----------------------------------------------------------------------------
/// Mute or unmute the pcm buffer
// ============================================================================
typedef enum
{
    VPP_SPEECH_MUTE = 0x0,
    VPP_SPEECH_UNMUTE = 0x400

} VPP_SPEECH_MUTE_T;

// ============================================================================
// VPP_SPEECH_AUDIO_CFG_T
// ----------------------------------------------------------------------------
/// VPP Speech audio configuration for echo cancellation, encoder gain and anti-
/// distorsion filter, decoder gain and anti-distorsion filter
// ============================================================================
typedef struct
{
    /// enable the echo suppresor
    INT16 echoEsOn;
    /// echo suppresor voice activity detection threshold
    INT16 echoEsVad;
    /// echo suppresor double-talk detection threshold
    INT16 echoEsDtd;   
    /// echo canceller relative threshold
    INT16 echoExpRel;
    /// echo canceller mu paramater (NULL to disable the echo canceller)
    INT16 echoExpMu;
    /// echo canceller minimum threshold
    INT16 echoExpMin;
    /// encoder gain
    INT16 encMute;
    /// decoder gain
    INT16 decMute;
    /// pointer to the speaker anti-distorsion filter (SDF) coefficients
    INT32 *sdf;
    /// pointer to the mic anti-distorsion filter (MDF) coefficients
    INT32 *mdf;
    /// push-to-talk enable flag
    INT16 if1;
    
} VPP_SPEECH_AUDIO_CFG_T;

//=============================================================================
// vpp_SpeechOpen function
//-----------------------------------------------------------------------------
/// Initialize all VPP Speech code and environment variables. It also set the 
/// VoC wakeup masks necessary for the synchronization. This function is 
/// called each time a Speech call is started.
/// @param vocIrqHandler : user defined handler function for the VoC interrupt.
/// If the function is set to NULL, no interrupts are used (the interrupt mask 
/// is not set).
/// @param wakeupMode : choose to encode, decode or both. Select whether the 
/// VoC module wakeup is controlled by XCPU or by IFC interrupts.
/// @return error of type HAL_ERR_T
//=============================================================================
PUBLIC HAL_ERR_T vpp_SpeechOpen(    HAL_VOC_IRQ_HANDLER_T       vocIrqHandler, 
                                    VPP_SPEECH_WAKEUP_MODE_T    wakeupMode);

//=============================================================================
// vpp_SpeechClose function
//-----------------------------------------------------------------------------
/// Close VPP Speech, clear VoC wakeup masks. This function is 
/// called each time a voice call is stopped.
//=============================================================================
PUBLIC VOID vpp_SpeechClose(VOID);

//=============================================================================
// vpp_SpeechReset function
//-----------------------------------------------------------------------------
/// Reset VPP Speech.
//=============================================================================
PUBLIC VOID vpp_SpeechReset(VOID);

//=============================================================================
// vpp_SpeechAudioCfg function
//-----------------------------------------------------------------------------
/// Configure the audio parameters of VPP Speech.
/// @param pAudioCfg : pointer to the structure containing the audio configuration.
//=============================================================================
PUBLIC VOID vpp_SpeechAudioCfg(VPP_SPEECH_AUDIO_CFG_T *pAudioCfg);

//============================================================================
// vpp_SpeechCodecCfg function
//-----------------------------------------------------------------------------
/// Configure the codec parameters in the decoder structure of VPP Speech.
/// @param pCodecCfg : pointer to the structure containing the decoder configuration.
//=============================================================================
PUBLIC VOID vpp_SpeechCodecCfg(HAL_SPEECH_DEC_IN_T *pCodecCfg);

//=============================================================================
// vpp_SpeechScheduleOneFrame function
//-----------------------------------------------------------------------------
/// Schedule one decoding and/or one encoding frame. Convert to Vpp codec mode. 
/// @return error of type HAL_ERR_T
//=============================================================================
PUBLIC HAL_ERR_T vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_MODE_T    wakeupMode);

//=============================================================================
// vpp_SpeechGetRxPcmBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the output (Rx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechGetRxPcmBuffer(VOID);

//=============================================================================
// vpp_SpeechGetTxPcmBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Tx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechGetTxPcmBuffer(VOID);

//=============================================================================
// vpp_SpeechGetRxCodBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Rx) coded buffer
//=============================================================================
PUBLIC HAL_SPEECH_DEC_IN_T * vpp_SpeechGetRxCodBuffer(VOID);

//=============================================================================
// vpp_SpeechGetTxCodBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the output (Tx) coded buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_ENC_OUT_T * vpp_SpeechGetTxCodBuffer(VOID);

///  @} <- End of the vpp_speech group

//=============================================================================
// vpp_SpeechSetMicDigitalGain function
//-----------------------------------------------------------------------------
/// @
//=============================================================================
PUBLIC   VOID vpp_SpeechSetMicDigitalGain(UINT16 DigitalGain);

//=============================================================================
// vpp_SpeechSetMicDigitalGain function
//-----------------------------------------------------------------------------
/// @
//=============================================================================
PUBLIC   VOID vpp_SpeechSetReceiverDigitalGain(UINT16 DigitalGain);


PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechGetRxPcmBuffer_test(VOID);
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechGetTxPcmBuffer_test(VOID);

PUBLIC   VOID vpp_SpeechSetAecEnableFlag(INT8 AecEnbleFlag,INT8 AgcEnbleFlag);
PUBLIC   VOID vpp_SpeechSetAecPara(INT8 NoiseGainLimit,INT8 NoiseMin,INT8 NoiseGainLimitStep,INT8 AmpThr);

PUBLIC   VOID vpp_SpeechSetMorphVoice(INT16 EnableFlag, INT16 MorphPara);

PUBLIC   VOID vpp_SpeechSetFilterPara(INT8 HighPassFilterFlag,INT8 NotchFilterFlag,INT8 NoiseSuppresserFlag,INT8 NoiseSuppresserWithoutSpeechFlag);



#endif  // VPP_SPEECH_H
