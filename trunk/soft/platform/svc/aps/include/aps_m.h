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
//  $HeadURL$ //
//	$Author$                                                        // 
//	$Date$                     //   
//	$Revision$                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file                                                                     //
/// That file describes the audio Play Service (aps) interface.             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _APS_M_H_
#define _APS_M_H_

#include "cs_types.h"

#include "hal_aif.h"
#include "aud_m.h"

/// @file aps_m.h
/// @mainpage Audio Plays Service API
/// @page aps_mainpage  Audio Plays Service API
/// 
/// This service provides playing capabilities for various encoded streams.
/// They can be played in loop or not and interrupts are generated when the middle
/// and the end of the buffer are reached. \n
/// If a handler for those interrupt is passed to APS, it is expected that they
/// are used to refill the buffer with data, and APS will make a count of available
/// data and suspend playing in case of underflow. #aps_AddedData needs to be called
/// to tell APS of the added data. \n
/// On the other hand, if no handler is defined, APS will loop on the buffer 
/// whatever happens. It doesn't take #aps_AddedData into account, and no 
/// call to it is needed.
/// 
/// 
/// 
/// @par Configuration requirements
/// HAL and AUD must be poperly configured in order to use the aps service.
/// 
/// 
/// The API is detailed in the following group: @ref aps
///
///
/// @defgroup aps Play Service (APS)
/// @{
///  

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// APS_PLAY_MODE_T
// -----------------------------------------------------------------------------
/// This type describes the encoding mode used in a stream to play.
// =============================================================================
typedef enum
{
    APS_PLAY_MODE_AMR475,
    APS_PLAY_MODE_AMR515,
    APS_PLAY_MODE_AMR59,
    APS_PLAY_MODE_AMR67,
    APS_PLAY_MODE_AMR74,
    APS_PLAY_MODE_AMR795,
    APS_PLAY_MODE_AMR102,
    APS_PLAY_MODE_AMR122,
    APS_PLAY_MODE_FR,
    APS_PLAY_MODE_HR,
    APS_PLAY_MODE_EFR,
    APS_PLAY_MODE_PCM,
    // TODO Implement that mode
    APS_PLAY_MODE_AMR_RING,
    APS_PLAY_MODE_MP3,
    APS_PLAY_MODE_INVALID,

    APS_PLAY_MODE_QTY
} APS_PLAY_MODE_T;



// =============================================================================
// APS_ERR_T
// -----------------------------------------------------------------------------
/// Error types for the AUD module.
// =============================================================================
typedef enum
{
    /// No error occured
    APS_ERR_NO,

    /// An attempt to access a busy resource failed
    /// (Resource use not possible)
    APS_ERR_RESOURCE_BUSY,

    /// Attempt to open a resource already opened,
    /// (no need to open again to use).
    APS_ERR_ALREADY_OPENED,
    
    /// Timeout while trying to access the resource
    APS_ERR_RESOURCE_TIMEOUT,
    
    /// Invalid parameter
    APS_ERR_BAD_PARAMETER,

    /// The specified interface does not exist
    APS_ERR_NO_ITF,

    /// What ?
    APS_ERR_UNKNOWN,


    APS_ERR_QTY
} APS_ERR_T;


// =============================================================================
// APS_STATUS_T
// -----------------------------------------------------------------------------
/// Status returned by the APS to the calling entity. The possible value
/// describes various information about the status of the play.
// =============================================================================
typedef enum
{
    APS_STATUS_MID_BUFFER_REACHED,
    APS_STATUS_END_BUFFER_REACHED,
    APS_STATUS_NO_MORE_DATA,
    APS_STATUS_ERR,

    APS_STATUS_QTY
} APS_STATUS_T;



// =============================================================================
// APS_USER_HANDLER_T
// -----------------------------------------------------------------------------
/// To give back status to the calling task, the APS needs to be given a 
/// callback function. This function is called whenever an event occurs
/// which needs to be reported back to the APS used. This status is reported
/// as the parameter of the function. The function is passed as a parameter to 
/// the function #aps_Play. This type is the type describing such a function.
///
/// Note: This function must be as minimalist as possible, as it will be 
/// executed during an interrupt. It should for example only send one event
/// to a task, asking for data refilling of the buffer. The data copy is
/// then done by the task, when it is rescheduled, but not directly by the
/// user handler.
// =============================================================================
typedef VOID (*APS_USER_HANDLER_T)(APS_STATUS_T);


// =============================================================================
// APS_AUDIO_CFG_T
// -----------------------------------------------------------------------------
/// Configuration structure.
/// 
/// A configuration structure allows to play a stream with the proper configuration
/// set as far as the audio interface and the decoding are concerned.
/// @todo Expand this structure whether needed.
// =============================================================================
typedef struct
{
    /// Speaker level, 0=MUTE, MAX=AUD_LEVEL_MAX
    AUD_SPK_LEVEL_T spkLevel;
    
    /// Pointer to a filter to apply to the stream
    UINT32*     filter;
    
} APS_AUDIO_CFG_T;





// =============================================================================
// APS_ENC_STREAM_T
// -----------------------------------------------------------------------------
/// This type defines an encoded stream. All the fields might not be needed 
/// depending on the encoding scheme used.
// =============================================================================
typedef struct
{
    /// 
    UINT32 startAddress;

    /// Length of the buffer. That is not the total length of the song
    /// to play, as data can be added to the buffer while playing.
    /// cf. API functions for details.
    UINT32 length;

    /// Mode used to play that stream
    APS_PLAY_MODE_T mode;

    /// Sample Rate of the stream. Optional, this value is ignored
    /// by mode not needing it.
    HAL_AIF_FREQ_T sampleRate;

    /// Number of channel of the stream. Optional,this value is ignored
    /// by mode not needing it.
    HAL_AIF_CH_NB_T channelNb;

    /// Is this encoded in voice mode (voice quality, mono, 8kHz)
    BOOL    voiceQuality;

    /// Function called to transmit play status to the caller
    /// If null, no handler is called.
    APS_USER_HANDLER_T handler;

} APS_ENC_STREAM_T;





// =============================================================================
//  FUNCTIONS
// =============================================================================




// =============================================================================
// aps_Setup
// -----------------------------------------------------------------------------
/// Configure the aps service.. 
/// 
/// This functions configures the playing of a stream on a given audio interface:
/// gain for the side tone and  
/// the speaker, input selection for the microphone and output selection for 
/// the speaker... (TODO complete)
/// 
/// @param itf Interface number of the interface to setup.
/// @param cfg The configuration set applied to the audio interface. See 
/// #AUD_CFG_T for more details.
/// @return #APS_ERR_NO it can execute the configuration.
// =============================================================================
DEPRECATED PUBLIC APS_ERR_T aps_Setup(AUD_ITF_T itf, CONST APS_AUDIO_CFG_T* cfg);



// =============================================================================
// aps_Play
// -----------------------------------------------------------------------------
/// Start the playing of a stream. 
/// 
/// This function  outputs the 
/// audio on the audio interface specified as a parameter, on the output selected in 
/// the #aps_Setup function. \n
/// In normal operation, when the buffer runs out, the playing will stop. It can
/// loop according to the caller choice. If a handler is defined, it is expected 
/// that the handler will refeed data and call #aps_AddedData to tell APS about 
/// that. If there is no handler, in loop mode, the buffer will be played in loop
/// without expecting new data.
/// APS can a user function
/// at the middle or at the end of the playback.
/// 
/// @param itf Interface on which play that stream
/// @param stream Stream to play. Handler called at the middle and end of the buffer
/// are defined there.
/// @param cfg The configuration set applied to the audio interface
/// @param loop \c TRUE if the buffer is played in loop mode.
/// @return #APS_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command, or if VPP is unavailable.
///         #APS_ERR_NO it can execute the command.
// =============================================================================
DEPRECATED PUBLIC APS_ERR_T aps_Play(
                        CONST AUD_ITF_T      itf,
                        CONST APS_ENC_STREAM_T* stream,
                        CONST APS_AUDIO_CFG_T* cfg,
                        BOOL loop);



// =============================================================================
// aps_Pause
// -----------------------------------------------------------------------------
/// This function pauses the audio stream playback.
/// If the function returns 
/// #APS_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param pause If \c TRUE, pauses a playing stream. If \c FALSE (and a
/// stream is paused), resumes a paused stream.
/// @return #APS_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #APS_ERR_NO if it can execute the command.
// =============================================================================
DEPRECATED PUBLIC APS_ERR_T aps_Pause(BOOL pause);



// =============================================================================
// aps_Stop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback.
/// If the function returns 
/// #APS_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #APS_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #APS_ERR_NO if it can execute the command.
// =============================================================================
DEPRECATED PUBLIC APS_ERR_T aps_Stop(VOID);


// =============================================================================
// aps_GetBufPosition
// -----------------------------------------------------------------------------
/// This function returns the current position in the stream buffer.
/// This position points the next bytes to be played.
/// The bytes before this position might not yet have been played at the 
/// time this position is read, but are in the process pipe to be played.
///
/// @return The pointer to the next sample to be fetched from the stream buffer.
// =============================================================================
DEPRECATED PUBLIC UINT32* aps_GetBufPosition(VOID);


// =============================================================================
// aps_AddedData
// -----------------------------------------------------------------------------
/// When a stream buffer is played in loop mode, already played data can be 
/// replaced by new ones to play a long song seamlessly. The APS service needs 
/// to be informed about the number of new bytes to play in the buffer. This 
/// is the role of the #aps_AddedData function.
///
/// It is mandatory to call this function when a stream is played in loop mode
/// with handler defined.
///
/// If the function returns 
/// #APS_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param addedDataBytes Number of bytes added to the buffer.
/// @return #APS_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #APS_ERR_NO if it can execute the command.
// =============================================================================
DEPRECATED PUBLIC APS_ERR_T aps_AddedData(UINT32 addedDataBytes);


///  @} <- End of the aps group 



#endif // _APS_M_H_

