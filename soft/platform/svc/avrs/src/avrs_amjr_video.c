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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/avrs/src/avrs_amjr_video.c $
//  $Author: romuald $
//  $Date: 2009-02-20 02:12:26 +0800 (Fri, 20 Feb 2009) $
//  $Revision: 22265 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file avrs_amjpg.c
///
/// MJPEG handler
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS
// =============================================================================

#include "cs_types.h"

#include "avrs_m.h"
#include "avrsp_amjr.h"
#include "avrsp.h"
#include "avrsp_debug.h"
#include "avrsp_task.h"

#include "aud_m.h"

#include "sxr_ops.h"
#include "sxs_io.h"

#include "hal_host.h"
#include "hal_debug.h"

#include "vpp_amjr.h"

#include <string.h>



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


// =============================================================================
// PRIVATE FUNCTIONS
// =============================================================================


// =============================================================================
// FUNCTIONS
// =============================================================================

// =============================================================================
// avrs_VppAmjrVideoHandler
// -----------------------------------------------------------------------------
/// Handler for the video cause of the VPP AMJR interrupt.
/// @param status VPP AMJR interrupt status.
// =============================================================================
PROTECTED VOID avrs_VppAmjrVideoHandler(HAL_VOC_IRQ_STATUS_T* status)
{
    VPP_AMJR_STATUS_T   amjrStatus;
    AVRS_MSG_T*          msg;

    AVRS_PROFILE_FUNCTION_ENTER(avrs_VppAmjrVideoHandler);

    // get status
    vpp_AmjrStatus(&amjrStatus);

    AVRS_TRACE(AVRS_INFO_TRC, 0, "VPP ENCODED LENGTH %d", amjrStatus.imagOutputLen);

    // Send the message to the task that the encoding of the picture frame 
    // and the calculation of the preview are done.
    msg = sxr_Malloc(sizeof(AVRS_MSG_T));
    msg->headerId = AVRS_OP_ID_DISPLAY_PREVIEW;
    msg->status = amjrStatus.imagOutputLen;
    sxr_Send(msg, g_avrsMbx, SXR_SEND_MSG);

    // profile
    AVRS_PROFILE_FUNCTION_EXIT(avrs_VppAmjrVideoHandler);
}


