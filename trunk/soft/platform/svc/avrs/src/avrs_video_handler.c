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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/avrs/src/avrs_video_handler.c $
//	$Author: romuald $                                                        // 
//	$Date: 2009-02-09 23:29:39 +0800 (Mon, 09 Feb 2009) $                     //   
//	$Revision: 22032 $                                                        //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file                                                                     //
/// That file implements AVRS internal video handler. That is 
/// the function called when the encoding of a video frame has been finished,
/// that do the data movement needed, the global variable updates and the 
/// message sending to the interested parties.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "sxr_ops.h"
#include "sxs_io.h"

#include "avrs_m.h"
#include "avrsp.h"
#include "avrsp_video_handler.h"


#include "hal_sys.h"

// =============================================================================
// avrs_VideoHandler
// -----------------------------------------------------------------------------
/// This is the video handler called when the encoding of a frame is finished.
/// (This function being the callback of a #IMGS_ENCODED_PICTURE_T structure, it
/// must comply to the #IMGS_ENCODED_PICTURE_T type prototype.)
///
/// @param encodedBuffer Pointer to the beginning of the encoded picture.
/// @param length Length of the picture in the buffer.
// =============================================================================
PROTECTED VOID avrs_VideoHandler(UINT8* encodedBuffer, UINT32 length)
{
    // Change the definition of the latest encoded 
    // video frame asynchronously.
    UINT32 csStatus = hal_SysEnterCriticalSection();
    g_avrsLatestEncodedFrame        = encodedBuffer;
    g_avrsLatestEncodedFrameLength  = length;
    hal_SysExitCriticalSection(csStatus);

    // Special case when there is no audio ...
    // Since normaly audio is used as the scheduler to 
    // send at the proper time the message with the pointer
    // to the encoded Jpeg image, when there is no audio,
    // we ('re in trouble) need to send the event manually
    // here.
    // FIXME: This won't enforce the frame rate if the 
    // source (camera) lags behind.
    if (!g_avrsAudioEnabled && g_avrsVideoStream.mbx != 0xFF)
    {
        AVRS_EVENT_T ev;
        ev.id           = AVRS_EVENT_ID_VIDEO;
        ev.bufferPos    = (UINT32*)(encodedBuffer);
        ev.frameSize    = length;
        ev.reserved[0]  = 0;
        sxr_Send(&ev, g_avrsVideoStream.mbx, SXR_SEND_EVT);
    }
}




