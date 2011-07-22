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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/avrs/src/avrsp_video_handler.h $
//	$Author: romuald $                                                        // 
//	$Date: 2009-02-03 01:52:43 +0800 (Tue, 03 Feb 2009) $                     //   
//	$Revision: 21880 $                                                        //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file                                                                     //
/// That file describes the interface of AVRS internal video handler. That is 
/// the function called when the encoding of a video frame has been finished,
/// that do the data movement needed, the global variable updates and the 
/// message sending to the interested parties.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _AVRSP_VIDEO_HANDLER_H_
#define _AVRSP_VIDEO_HANDLER_H_


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
PROTECTED VOID avrs_VideoHandler(UINT8* encodedBuffer, UINT32 length);














#endif // _AVRSP_VIDEO_HANDLER_H_





