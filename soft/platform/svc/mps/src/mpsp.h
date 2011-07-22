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
/// @file mpsp.h
///
/// Description of handle
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MPSP_H_
#define _MPSP_H_

#include "cs_types.h"

#include "mps_m.h"

#include "aud_m.h"
#include "avps_m.h"

#include "mpsp_medium.h"


// =============================================================================
// MACROS
// =============================================================================


// =============================================================================
// TYPES
// =============================================================================

typedef struct MPS_DEMUX_STRUCT_T MPS_DEMUX_T;

// =============================================================================
// MPS_HANDLE_STRUCT_T
// -----------------------------------------------------------------------------
/// MPS handle description
// =============================================================================
struct MPS_HANDLE_STRUCT_T
{
    MPS_MEDIUM_T                 mediumAudio;
    MPS_MEDIUM_T                 mediumVideo;
    UINT32                       nbPlayed;

    // Audio buffer
    UINT8*                       audioBuffer;
    UINT32                       audioLength;
    // Video buffer
    UINT8*                       videoBuffer;
    UINT32                       videoLength;

    AVPS_ENC_STREAM_T            stream;
    AVPS_BUFFER_CONTEXT_T        bufferContext;

    MPS_FILE_INFO_T              info;

    MPS_CONFIG_T                 config;
    MPS_CALLBACK_T               callback;

    MPS_DEMUX_T*                 demux;
    VOID*                        demuxData;
};

// =============================================================================
// GLOBAL VARIABLES
// =============================================================================


// =============================================================================
// FUNCTIONS
// =============================================================================


#endif // _MPSP_H_
