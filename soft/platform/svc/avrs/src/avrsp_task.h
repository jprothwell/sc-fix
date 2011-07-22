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
//
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/avrs/src/avrsp_task.h $
//  $Author: romuald $
//  $Date
//  $Revision: 22154 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file avrsp_task.h
///
/// This file describes the AVRS task API.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _AVRSP_TASK_H_
#define _AVRSP_TASK_H_

#include "cs_types.h"
#include "sxr_ops.h"

#include "avrs_m.h"
#include "camd_m.h"

#define ZOOM_PRECISION 1024

// =============================================================================
// AVRS_INTERNAL_STATUS_T
// -----------------------------------------------------------------------------
/// The status of avrs service
// =============================================================================
typedef enum
{
    AVRS_STATUS_CLOSE = 0,
    AVRS_STATUS_OPEN,
    AVRS_STATUS_PICTURE_STREAMING
} AVRS_STATUS_T ;


// =============================================================================
// AVRS_OP_ID_T
// -----------------------------------------------------------------------------
/// Type used to describe the camera driver status in the message sent 
/// through the AVRS message mailbox.
// =============================================================================
typedef enum
{
    /// A picture has been captured by the camera sensor.
    /// (Sent to AVRS by CAMD). Once a picture is captured, AVRS task will use 
    /// AMJR to encode this picture and produce a preview frame, adapted to the 
    /// size of the screen
    AVRS_OP_ID_CAPTURE_DONE = CAMD_MSG_HEADER_ID,

    /// Stop the picture stream. (Sent by AVRS'user). AVRS will finish the 
    /// operation related to the current frame before stopping.
    AVRS_OP_ID_START_PICTURE_STREAM,

    /// Stop the picture stream. (Sent by AVRS'user). AVRS will finish the 
    /// operation related to the current frame before stopping.
    AVRS_OP_ID_STOP_PICTURE_STREAM,

    /// TODO Implement the use of that one. It could be used to replace CAMS 
    /// with AVRS.
    AVRS_OP_ID_START_CAPTURE,

    /// Display, with LCDD, the preview picture. (Sent by AVRS IRQ handler for
    /// interrupt from VPP). AVRS task will be in charge of calling the display
    /// handler  provided by the user in charge of handling the preview display.
    /// (This way, the handler is not called in an interrupt context).
    /// This message also correspond to the end of the 
    /// FIXME Rename ?
    AVRS_OP_ID_DISPLAY_PREVIEW,


    AVRS_OP_ID_CAPTURE_QTY
} AVRS_OP_ID_T;



// =============================================================================
// AVRS_MSG_T
// -----------------------------------------------------------------------------
/// Type used to encode the messages sent to the mailbox.
// (size must be at most four 32 bits words)
// =============================================================================
typedef struct
{
    AVRS_OP_ID_T    headerId;
    UINT32    status;
} AVRS_MSG_T;


// =============================================================================
// AVRS_MSG_CAPTURE_T
// -----------------------------------------------------------------------------
/// Type used to encode the start capture message. headerId must be set to 
/// #AVRS_OP_ID_START_CAPTURE;
// =============================================================================
typedef struct
{
    AVRS_OP_ID_T            headerId;
    LCDD_FBW_T*             dataFbw;
    LCDD_FBW_T*             prvwFbw;
    AVRS_CAPTURE_HANDLER_T  captureHandler;
} AVRS_MSG_CAPTURE_T;


// =============================================================================
// AVRS_MSG_PICTURE_STREAM_T
// -----------------------------------------------------------------------------
/// Type used to encode the start capture message. headerId must be set to 
/// #AVRS_OP_ID_START_PICTURE_STREAM;
// =============================================================================
typedef struct
{
    AVRS_OP_ID_T                headerId;
    AVRS_DECODED_PICTURE_T*     previewPicture;
    AVRS_ENCODED_PICTURE_T*     encodedFrame;
    AVRS_REC_VIDEO_RESOLUTION_T resolution;
    AVRS_FBW_STREAM_T*          fbwStream;
    AVRS_ENC_BUFFER_STREAM_T*   encBufStream;
    UINT32                      streamDepth;
} AVRS_MSG_PICTURE_STREAM_T;


// =============================================================================
// MACROS                                                                       
// =============================================================================
#define AVRS_TASK_STACK_SIZE    256
#define AVRS_TASK_PRIORITY      50

/// Id of messages coming from AVRS
#define AVRS_MSG_HEADER_ID 0xA0C01F70

// =============================================================================
// PROTECTED VARIABLES                                                            
// =============================================================================

EXPORT sxr_TaskDesc_t   g_avrsTask;
EXPORT UINT8            g_avrsIdTask;
EXPORT UINT8            g_avrsMbx;
EXPORT AVRS_STATUS_T    g_avrsStatus;
EXPORT UINT32           g_avrsAvailResolution;
EXPORT UINT32           g_avrsValidParam;
EXPORT LCDD_FBW_T       g_avrsCameraRawDataFbw;
EXPORT UINT32           g_avrsCameraRawDataSize;
EXPORT UINT32           g_avrsDigitalZoomFactor;


// ==============================================================================
// avrs_Task
// ------------------------------------------------------------------------------
/// AVRS task.
// ==============================================================================
PROTECTED VOID avrs_Task(VOID* param);

PROTECTED VOID avrs_SetCameraResolution(LCDD_FBW_T* Fbw);

#endif // _AVRSP_TASK_H_

