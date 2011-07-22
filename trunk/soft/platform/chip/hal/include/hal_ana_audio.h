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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_ana_audio.h $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                        //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_ana_audio.h                                                     //
/// That file provides the interface to access, configure and use the audio   //
/// functionalities of the analog macro.                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_ANA_AUDIO_H_
#define _HAL_ANA_AUDIO_H_

#include "cs_types.h"
#include "hal_aif.h"


/// @defgroup ana_audio HAL Analog Audio Driver
/// This Driver provides the API to configure and the use the audio 
/// functionalities of the analog macro. Its goal is not to provide an audio
/// driver API, but is to be used by the AUD driver for the analog macro audio.
///
/// @par Operation
/// Its core functionalities are thus limited to Opening and Closing
/// the microphone and speaker, choosing among the two available
/// of each sort, and setting the gains. The open functions must obviously
/// be called before using the microphone or speaker, respectively.
///
/// @par For chips with a stereo DAC, it can be used to play stream with a 
/// sample rate strictly above 8Khz, mono or stereo.
/// @{

// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  TYPES
// =============================================================================
// =============================================================================
// HAL_ANA_AUDIO_SPK_T
// -----------------------------------------------------------------------------
/// The analog macro offers two set of interfaces. This 
/// type is used to describes the speaker used.
// =============================================================================
typedef enum
{
    /// That output can only handle Mono 8Khz voice quality
    /// streams. (Streams with the voiceQuality field set to TRUE).
    HAL_ANA_AUDIO_SPK_RECEIVER,
    HAL_ANA_AUDIO_SPK_EAR_PIECE,
    HAL_ANA_AUDIO_SPK_LOUD_SPEAKER,

    HAL_ANA_AUDIO_SPK_QTY
} HAL_ANA_AUDIO_SPK_T;



// =============================================================================
// HAL_ANA_AUDIO_MIC_T
// -----------------------------------------------------------------------------
/// The analog macro offers two set of interfaces. This 
/// type is used to describes the microphone used.
// =============================================================================
typedef enum
{
    /// That input can only handle Mono 8Khz voice quality
    /// streams. (Streams with the voiceQuality field set to TRUE).
    HAL_ANA_AUDIO_MIC_RECEIVER,
    HAL_ANA_AUDIO_MIC_EAR_PIECE,
    HAL_ANA_AUDIO_MIC_LOUD_SPEAKER,

    HAL_ANA_AUDIO_MIC_QTY
} HAL_ANA_AUDIO_MIC_T;



// =============================================================================
// HAL_ANA_AUDIO_DIGITAL_GAIN_T
// -----------------------------------------------------------------------------
/// This type is used to describe the digital gain. This type is common for both
/// microphone, speaker and stereo speaker gain adjustment. All values are not 
/// necessarily available.
// =============================================================================
typedef enum
{
    HAL_ANA_AUDIO_DIGITAL_GAIN_0dB  = 0,
    HAL_ANA_AUDIO_DIGITAL_GAIN_6dB  = 1,
    HAL_ANA_AUDIO_DIGITAL_GAIN_12dB = 2,
    HAL_ANA_AUDIO_DIGITAL_GAIN_18dB = 3,
    HAL_ANA_AUDIO_DIGITAL_GAIN_24dB = 4,
    HAL_ANA_AUDIO_DIGITAL_GAIN_30dB = 5,
    HAL_ANA_AUDIO_DIGITAL_GAIN_36dB = 6,
    HAL_ANA_AUDIO_DIGITAL_GAIN_42dB = 7,

    HAL_ANA_AUDIO_DIGITAL_GAIN_QTY
} HAL_ANA_AUDIO_DIGITAL_GAIN_T;



// =============================================================================
// HAL_ANA_AUDIO_SPK_ANALOG_GAIN_T
// -----------------------------------------------------------------------------
/// This type is used to ajust the analog gain of the speaker.
// =============================================================================
typedef enum
{
    /// gain of - 15 dB
    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_m15dB = 0,
    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_m12dB = 1,
    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_m9dB = 2,
    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_m6dB = 3,
    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_m3dB = 4,
    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_0dB  = 5,
    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_3dB  = 6,
    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_6dB  = 7,

    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_QTY
} HAL_ANA_AUDIO_SPK_ANALOG_GAIN_T;



// =============================================================================
// HAL_ANA_AUDIO_MIC_ANALOG_GAIN_T
// -----------------------------------------------------------------------------
/// This type is used to ajust the analog gain of the microphone.
// =============================================================================
typedef enum
{
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_0dB   = 0,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_3dB   = 1,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_6dB   = 2,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_9dB   = 3,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_12dB  = 4,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_15dB  = 5,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_18dB  = 6,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_21dB  = 7,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_24dB  = 8,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_27dB  = 9,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_30dB  = 10,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_33dB  = 11,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_36dB  = 12,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_39dB  = 13,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_42dB  = 14,
    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_45dB  = 15,

    HAL_ANA_AUDIO_MIC_ANALOG_GAIN_QTY
} HAL_ANA_AUDIO_MIC_ANALOG_GAIN_T;



// =============================================================================
// HAL_ANA_AUDIO_SPK_PREAMP_GAIN_T
// -----------------------------------------------------------------------------
/// This gain adjusts the volume coming from the analog voice codec through 
/// the stereo DAC amplifier, when the #HAL_ANA_AUDIO_MIC_EAR_PIECE or
/// #HAL_ANA_AUDIO_MIC_LOUD_SPEAKER outputs are used.
// =============================================================================
typedef enum
{
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_m15dB = 7,
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_m12dB = 6,
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_m9dB  = 5,
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_m6dB  = 4,
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_m3dB  = 3,
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_0dB   = 2,
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_3dB   = 1,
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_6dB   = 0,
    
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_QTY   = 8,
} HAL_ANA_AUDIO_SPK_PREAMP_GAIN_T;


// =============================================================================
// HAL_ANA_AUDIO_SPK_STEREO_ANALOG_GAIN_T
// -----------------------------------------------------------------------------
/// Gain for the stereo output of the audio analog. It is only relevant when 
/// outputting a stream with a sample rate strictly above 8Khz.
// =============================================================================
typedef enum
{
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_MUTE      = 0,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m30dB     = 3,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m28_5dB   = 4,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m27dB     = 5,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m25_5dB   = 6,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m24dB     = 7,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m22_5dB   = 8,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m21dB     = 9,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m19_5dB   = 10,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m18dB     = 11,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m16_5dB   = 12,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m15dB     = 13,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m13_5dB   = 14,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m12dB     = 15,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m10_5dB   = 16,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m9dB      = 17,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m7_5dB    = 18,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m6dB      = 19,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m4_5dB    = 20,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m3dB      = 21,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_m1_5dB    = 22,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_0dB       = 23,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_1_5dB     = 24,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_3dB       = 25,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_4_5dB     = 26,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_6dB       = 27,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_7_5dB     = 28,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_9dB       = 29,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_10_5dB    = 30,
    HAL_ANA_AUDIO_SPK_STEREO_GAIN_12dB      = 31
} HAL_ANA_AUDIO_SPK_STEREO_ANALOG_GAIN_T;


// =============================================================================
// HAL_ANA_AUDIO_SPK_CONFIG_T
// -----------------------------------------------------------------------------
/// Describes the configuration of the analog audio.
// =============================================================================
typedef struct
{
    /// Speaker selection.
    HAL_ANA_AUDIO_SPK_T spkSel;

    /// Voice quality
    BOOL voiceQuality;

    /// Frequency of the stream to play
    HAL_AIF_FREQ_T freq;

    /// Shall the output be muted ?
    BOOL mute;

    /// Digital Gain
    HAL_ANA_AUDIO_DIGITAL_GAIN_T digitalGain;

    /// Voice analog gain
    HAL_ANA_AUDIO_SPK_ANALOG_GAIN_T voiceAnalogGain;

    /// Stereo DAC preamplifier for the voice output
    HAL_ANA_AUDIO_SPK_PREAMP_GAIN_T preampGain;

    /// Stereo DAC analog right gain
    HAL_ANA_AUDIO_SPK_STEREO_ANALOG_GAIN_T stereoAnalogRightGain;

    /// Stereo DAC analog left gain
    HAL_ANA_AUDIO_SPK_STEREO_ANALOG_GAIN_T stereoAnalogLeftGain;

} HAL_ANA_AUDIO_SPK_CONFIG_T;


// =============================================================================
// HAL_ANA_AUDIO_LOUD_SPEAKER_TYPE_T
// -----------------------------------------------------------------------------
/// Describes how the loudspeaker is plugged on the stereo output:
///   - is it a stereo loudspeaker ? (loudspeakers ?)
///   - is it a mono loudspeaker on the left channel ?
///   - is it a mono loudspeaker on the right channel ?
// =============================================================================
typedef enum
{
    HAL_ANA_AUDIO_LOUD_SPEAKER_STEREO,
    HAL_ANA_AUDIO_LOUD_SPEAKER_MONO_RIGHT,
    HAL_ANA_AUDIO_LOUD_SPEAKER_MONO_LEFT,

    HAL_ANA_AUDIO_LOUD_SPEAKER_QTY
} HAL_ANA_AUDIO_LOUD_SPEAKER_TYPE_T;


// =============================================================================
// HAL_ANA_AUDIO_HANDLER_T
// -----------------------------------------------------------------------------
/// This type describes the function called by the HAL Analog audio driver when
/// the volume has been set. As a matter of fact, the volume changes are
/// following a ramp. When the desired value has been reached, a user function
/// described by this type can be called.
// =============================================================================
typedef VOID (*HAL_ANA_AUDIO_HANDLER_T)(VOID);



// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// =============================================================================
//  FUNCTIONS
// =============================================================================

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
                                HAL_ANA_AUDIO_LOUD_SPEAKER_TYPE_T lsType);



// =============================================================================
// hal_AnaAudioSpkClose
// -----------------------------------------------------------------------------
/// Closes and disables the speaker of the analog macro. This function shares a
/// resource with the #hal_AnaAudioMicClose. This common resource is freed to
/// #HAL_SYS_FREQ_32K as soon as both are closed.
// =============================================================================
PUBLIC VOID hal_AnaAudioSpkClose(VOID);


// =============================================================================
// hal_AnaAudioMicOpen
// -----------------------------------------------------------------------------
/// Opens and enables the micro of the analog macro. This function shares a
/// resource with the #hal_AnaAudioSpkOpen. This common resource is taken to
/// #HAL_SYS_FREQ_26M as long as either one of them is opened.
// =============================================================================
PUBLIC VOID hal_AnaAudioMicOpen(VOID);



// =============================================================================
// hal_AnaAudioMicClose
// -----------------------------------------------------------------------------
/// Closes and disables the speaker of the analog macro. This function shares a
/// resource with the #hal_AnaAudioSpkClose. This common resource is freed to
/// #HAL_SYS_FREQ_32K as soon as both are closed.
// =============================================================================
PUBLIC VOID hal_AnaAudioMicClose(VOID);



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
                                  HAL_ANA_AUDIO_HANDLER_T       handler);



// =============================================================================
// hal_AnaAudioMicSel
// -----------------------------------------------------------------------------
/// Choose the microphone to use.
/// @param mic microphone to use.
// =============================================================================
PUBLIC VOID hal_AnaAudioMicSel(HAL_ANA_AUDIO_MIC_T mic);


// =============================================================================
// hal_AnaAudioMicSetGain
// -----------------------------------------------------------------------------
/// This function is used to set the gain of the microphone.
///
/// @param mute If \c TRUE, the microphone is muted, whatever the other parameters
/// are.
/// @param analogGain Analog gain for the microphone.
/// @param digitalGain Digital gain for the microphone.
// =============================================================================
PUBLIC VOID hal_AnaAudioMicSetGain(BOOL mute, 
        HAL_ANA_AUDIO_MIC_ANALOG_GAIN_T analogGain,
        HAL_ANA_AUDIO_DIGITAL_GAIN_T digitalGain);


/// @} // <-- End of the ana_audio group.

#endif // _HAL_ANA_AUDIO_H_


