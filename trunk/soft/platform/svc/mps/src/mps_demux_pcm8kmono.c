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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/mps/src/mps_demux_pcm8kmono.c $
//  $Author: lilian $
//  $Date$
//  $Revision: 22987 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file demux_pcm8kmono.c
///
/// PCM8KMONO demux functions
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS
// =============================================================================

#include "cs_types.h"
#include "fs.h"

#include "mpsp_demux.h"
#include "mpsp.h"
#include "mpsp_debug.h"

#include "sxr_ops.h"
#include <string.h>

// This file must be removed
// This file must be removed
// This file must be removed
// This file must be removed
// This file must be removed
// This file must be removed
// This file must be removed
// This file must be removed
// This file must be removed
// This file must be removed
// This file must be removed
// This file must be removed

// =============================================================================
// PRIVATE PROTOTYPE FUNCTIONS
// =============================================================================

PRIVATE UINT32 mps_getAudioDataPcm8kmono(MPS_HANDLE_T handle, UINT8* data,
                                      UINT32 size);

PRIVATE VOID* mps_openPcm8kmono(MPS_HANDLE_T handle);

PRIVATE VOID mps_closePcm8kmono(MPS_HANDLE_T handle);

PRIVATE VOID mps_pausePcm8kmono(MPS_HANDLE_T handle);

PRIVATE BOOL mps_playPcm8kmono(MPS_HANDLE_T handle, AVPS_CFG_T* avpsCfg);

// =============================================================================
// MACROS
// =============================================================================


// =============================================================================
// TYPES
// =============================================================================


// =============================================================================
// PRIVATE VARIABLES
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES
// =============================================================================

DEPRECATED PROTECTED MPS_DEMUX_T g_pcm8kmonoDemux =
{
    .open                = mps_openPcm8kmono,
    .close               = mps_closePcm8kmono,
    .play                = mps_playPcm8kmono,
    .pause               = mps_pausePcm8kmono,
    .getAudioData        = mps_getAudioDataPcm8kmono,
    .getVideoData        = 0
};

// =============================================================================
// PRIVATE FUNCTIONS
// =============================================================================

PRIVATE VOID* mps_openPcm8kmono(MPS_HANDLE_T handle)
{
    // PCM8KMONO
    MPS_TRACE("mps_openPcm8kmono");

    handle->videoLength              = 0;
    handle->videoBuffer              = 0;
    handle->audioLength              = 1024*8;
    handle->audioBuffer              = (UINT8*) sxr_Malloc(handle->audioLength);
    handle->stream.audioMode         = AVPS_PLAY_AUDIO_MODE_PCM;
    handle->stream.audioSampleRate   = 8000;
    handle->stream.channelNb         = 1;
    handle->stream.voiceQuality      = TRUE;

    return (VOID*)1;
}

PRIVATE VOID mps_closePcm8kmono(MPS_HANDLE_T handle)
{
    avps_Stop(&handle->bufferContext);
    if (handle->audioBuffer != NULL)
    {
        sxr_Free(handle->audioBuffer);
        handle->audioBuffer = NULL;
    }
}

PRIVATE UINT32 mps_getAudioDataPcm8kmono(MPS_HANDLE_T handle, UINT8* data, UINT32 size)
{
    INT32 i;

    i = mps_MediumRead(&handle->mediumAudio, data, size);

    if(i < 0)
    {
        i = 0;
    }

    return i;
}

PRIVATE BOOL mps_playPcm8kmono(MPS_HANDLE_T handle, AVPS_CFG_T* avpsCfg)
{
    MPS_TRACE("mps_playPcm8kmono");

    if(mps_MediumIsEof(&handle->mediumAudio))
    {
        mps_MediumSeek(&handle->mediumAudio, 0, FS_SEEK_SET);
    }
    if(avps_Play(handle->config.audioPath, &handle->stream, avpsCfg, 0, &handle->bufferContext) == AVPS_ERR_NO)
    {
        return TRUE;
    }
    return FALSE;
}

PRIVATE VOID mps_pausePcm8kmono(MPS_HANDLE_T handle)
{

}

// =============================================================================
// FUNCTIONS
// =============================================================================

