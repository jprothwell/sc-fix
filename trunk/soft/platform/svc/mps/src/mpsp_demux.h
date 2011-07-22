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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/mps/src/mpsp_demux.h $
//  $Author: lilian $
//  $Date$
//  $Revision: 22947 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file mpsp_demux.h
///
/// MPS demux structure
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _DEMUXP_H_
#define _DEMUXP_H_

#include "cs_types.h"
#include "mpsp.h"

// =============================================================================
// MACROS
// =============================================================================


// =============================================================================
// TYPES
// =============================================================================

// =============================================================================
// MPS_DEMUX_OPEN_T
// -----------------------------------------------------------------------------
/// Callback to detect if the file use this demux
/// @param handle Current handle use
/// @return Private demux structure
// =============================================================================
typedef VOID*(*MPS_DEMUX_OPEN_T)(MPS_HANDLE_T handle);

// =============================================================================
// MPS_DEMUX_CLOSE_T
// -----------------------------------------------------------------------------
/// Callback use to close demux and free demux structure
/// @param handle Current handle use
/// @return Private demux structure
// =============================================================================
typedef VOID (*MPS_DEMUX_CLOSE_T)(MPS_HANDLE_T handle);

// =============================================================================
// MPS_DEMUX_PLAY_T
// -----------------------------------------------------------------------------
/// Callback use to start file playing
/// @param handle Current handle use
/// @return TRUE if the successful else FALSE
// =============================================================================
typedef BOOL (*MPS_DEMUX_PLAY_T)(MPS_HANDLE_T handle,
                                 AVPS_CFG_T* avpsCfg);

// =============================================================================
// MPS_DEMUX_PAUSE_T
// -----------------------------------------------------------------------------
/// Callback use to pause stream
/// @param handle Current handle use
// =============================================================================
typedef VOID (*MPS_DEMUX_PAUSE_T)(MPS_HANDLE_T handle);

// =============================================================================
// MPS_DEMUX_GET_AV_DATA_T
// -----------------------------------------------------------------------------
/// Callback use to add data in audio or video buffer
/// @param handle Current handle use
/// @param data Memory chunk when you can add data
/// @param size Size of memory chunk
/// @return TRUE if successful else FALSE
// =============================================================================
typedef UINT32 (*MPS_DEMUX_GET_AV_DATA_T)(MPS_HANDLE_T handle, UINT8* data,
                                          UINT32 size);

// =============================================================================
// MPS_DEMUX_SKIP_VIDEO_T
// -----------------------------------------------------------------------------
/// Callback use to skip one video frame
/// @param handle Current handle use
// =============================================================================
typedef VOID (*MPS_DEMUX_SKIP_VIDEO_T)(MPS_HANDLE_T handle);

// =============================================================================
// MPS_DEMUX_SEEK_T
// -----------------------------------------------------------------------------
/// Callback use to jump under the a specific position of stream
/// @param handle Current handle use
/// @param position The position when you want jump
/// @return TRUE if successful else FALSE
// =============================================================================
typedef BOOL (*MPS_DEMUX_SEEK_T)(MPS_HANDLE_T handle, UINT32 position);

// =============================================================================
// MPS_DEMUX_TELL_T
// -----------------------------------------------------------------------------
/// Callback use to keep the current position in file (unite second)
/// @param handle Current handle use
/// @return The current position
// =============================================================================
typedef UINT32 (*MPS_DEMUX_TELL_T)(MPS_HANDLE_T handle);

// =============================================================================
// MPS_DEMUX_T
// -----------------------------------------------------------------------------
/// Demux structure
// =============================================================================
struct MPS_DEMUX_STRUCT_T
{
    MPS_DEMUX_OPEN_T                       open;
    MPS_DEMUX_CLOSE_T                      close;
    MPS_DEMUX_PLAY_T                       play;
    MPS_DEMUX_PAUSE_T                      pause;
    MPS_DEMUX_GET_AV_DATA_T                getAudioData;
    MPS_DEMUX_GET_AV_DATA_T                getVideoData;
    MPS_DEMUX_SKIP_VIDEO_T                 skipVideo;
    MPS_DEMUX_SEEK_T                       seek;
    MPS_DEMUX_TELL_T                       tell;
};

// =============================================================================
// GLOBAL VARIABLES
// =============================================================================

EXPORT PROTECTED UINT8*           g_mpsBufferReadPos;

EXPORT PROTECTED UINT8*           g_mpsBufferWritePos;

// =============================================================================
// FUNCTIONS
// =============================================================================


#endif // _DEMUXP_H_
