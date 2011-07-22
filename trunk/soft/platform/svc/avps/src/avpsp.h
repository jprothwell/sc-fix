////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/avps/src/avpsp.h $
//  $Author: lilian $
//  $Date: 2009-03-19 21:28:53 +0800 (Thu, 19 Mar 2009) $
//  $Revision: 22988 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file avpsp.h
///
/// AVPS service internal variables
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _AVPSP_H_
#define _AVPSP_H_

#include "cs_types.h"

#include "avps_m.h"
#include "avpsp_cfg.h"

#include "aud_m.h"
#include "lcdd_m.h"

#include "vpp_amjp.h"
#include "vpp_speech.h"

typedef AVPS_BODY_MSG_T MsgBody_t;
#include "itf_msg.h"

// =============================================================================
// MACROS
// =============================================================================

#define AVPS_ERR_NB_MAX 20

/// Number of trames before unmuting AUD
#define AVPS_AUD_UNMUTE_TRAME_NB 4

/// Number of trames before unmuting VPP
#define AVPS_VPP_UNMUTE_TRAME_NB 3

// =============================================================================
// TYPES
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES
// =============================================================================

EXPORT PROTECTED UINT32* g_avpsPcmBuf0;
EXPORT PROTECTED UINT32* g_avpsPcmBuf1;

/// Number of played trames
EXPORT PROTECTED UINT32 g_avpsFramesPlayedNb;

/// Interface used to play the current stream.
EXPORT PROTECTED AUD_ITF_T g_avpsItf;

/// Audio configuration used to play the stream in normal state.
/// (ie not when started or stopped.
EXPORT PROTECTED AUD_LEVEL_T g_avpsAudioCfg;

/// User mbx to call with play status is stored here.
EXPORT PROTECTED UINT8 g_avpsUserMbx;

/// End address of the stream buffer
EXPORT PROTECTED UINT32 g_avpsAudioBufferLength;

/// Start address of the stream buffer
EXPORT PROTECTED UINT32* g_avpsAudioBufferStart;

/// End address of the stream buffer
EXPORT PROTECTED UINT32* g_avpsAudioBufferTop;

/// Number of bytes of data to read in the buffer
EXPORT PROTECTED INT32 g_avpsAudioBufferRemainingBytes;

/// Current position in the buffer. Next read data will be read from here.
EXPORT PROTECTED UINT32* g_avpsAudioBufferCurPos;

/// counter for the errors
EXPORT PROTECTED UINT32 g_avpsErrNb;

/// Number of bytes composing one trame for the codec currently used
EXPORT PROTECTED UINT32 g_avpsFrameSize;

/// Handshake to prevent the same message to be sent several times
/// When a message is sent, this variable is set to FALSE.
/// When data are added, it is set to TRUE.
EXPORT PROTECTED BOOL g_avpsAudioBufferingMsgReceived;

EXPORT PROTECTED BOOL g_avpsVideoBufferingMsgReceived;

// -----------------------------------------------------------------------------
// VIDEO RELATED GLOBAL VARIABLES
// -----------------------------------------------------------------------------
/// Ping pong encoded video buffer, halfes are arithmetically accessed thanks
/// to the buffer length.
EXPORT PROTECTED UINT8* g_avpsVideoEncodedBuffer;

/// Length of the encoded video buffer.
EXPORT PROTECTED UINT32 g_avpsVideoEncodedBufferLength;

/// Encoded video buffer ping-pong index (0 or 1).
EXPORT PROTECTED UINT32 g_avpsVideoEncodedIndex;

/// Ping-pong Frame Buffer windows to display the decoded video.
/// (Technically, only the buffer of the frame buffer window
/// does need to be ping-ponged, but, hey, that is more fun this
/// way).
EXPORT PROTECTED LCDD_FBW_T g_avpsDecodedVideoFbw[2];

/// Decoded video buffer ping-pong index (0 or 1).
EXPORT PROTECTED UINT32 g_avpsVideoDecodedIndex;

EXPORT PROTECTED UINT32 g_avpsNextVideoFramePlayed;

EXPORT PROTECTED UINT32 g_avpsAudioVideoRatio;

/// Frequency audio bars
EXPORT PROTECTED UINT8 g_avpsAudioBars[16];

EXPORT PROTECTED UINT32 g_avpsVideoDecodedDisplayIndex;

EXPORT PROTECTED INT32 g_avpsVideoBufferRemainingBytes;

/// Audio PCM buffer
/// swap buffer of 576 samples of 16 bits (*2 for highest rates) per channel,
/// (*2 for the stereo) that is to say to the most 2*(576*2*2*2) bytes. Thus in UINT32 :
EXPORT PROTECTED UINT32 g_avpsPcmBuf[2*576*2];

// =============================================================================
// FUNCTIONS
// =============================================================================


#endif // _AVPSP_H_


