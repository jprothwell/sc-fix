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
//  $HeadURL$
//  $Author$
//  $Date$
//  $Revision$
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file mrsp.h
///
/// Description of handle
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MRSP_H_
#define _MRSP_H_

#include "cs_types.h"

#include "mrs_m.h"

#include "aud_m.h"
#include "avrs_m.h"

#include "mrsp_medium.h"
#include "mrsp_codec.h"


// =============================================================================
// MACROS
// =============================================================================


// =============================================================================
// TYPES
// =============================================================================

typedef struct MRS_CODEC_STRUCT_T MRS_CODEC_T;

// =============================================================================
// MRS_HANDLE_STRUCT_T
// -----------------------------------------------------------------------------
/// MRS handle description
// =============================================================================
struct MRS_HANDLE_STRUCT_T
{
    MRS_MEDIUM_T                mediumAudio;
    MRS_MEDIUM_T                mediumVideo;
    UINT32                      nbPlayed;

    // Audio buffer
    UINT8*                      audioBuffer;
    UINT32                      audioLength;
    // Video buffer
    UINT8*                      videoBuffer;
    UINT32                      videoLength;

    AVRS_ENC_AUDIO_STREAM_T     audioStream;
    AVRS_ENC_VIDEO_STREAM_T     videoStream;
    AVRS_BUFFER_CONTEXT_T       bufferContext;

    MRS_FILE_INFO_T             info;

    MRS_CFG_T                   config;
    MRS_CALLBACK_T              callback;

    MRS_CODEC_T*                codec;
    VOID*                       codecData;
};

// =============================================================================
// GLOBAL VARIABLES
// =============================================================================


// =============================================================================
// FUNCTIONS
// =============================================================================


#endif // _MRSP_H_
