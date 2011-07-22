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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/avrs/src/avrs_amjr_handler.c $
//  $Author: romuald $
//  $Date: 2009-03-05 02:08:49 +0800 (Thu, 05 Mar 2009) $
//  $Revision: 22603 $
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
// PRIVATE FUNCTIONS
// =============================================================================


// =============================================================================
// FUNCTIONS
// =============================================================================

// =============================================================================
// avrs_VppAmjrHandler
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID avrs_VppAmjrHandler(HAL_VOC_IRQ_STATUS_T* status)
{
    VPP_AMJR_STATUS_T   amjrStatus;
    AVRS_PROFILE_FUNCTION_ENTER(avrs_VppAmjrHandler);

    // get status
    vpp_AmjrStatus(&amjrStatus);

    AVRS_TRACE(AVRS_INFO_TRC, 0, "VPP STREAM STATUS %#x %#x", amjrStatus.strmStatus, amjrStatus.inStreamBufIfcStatus);

    switch (amjrStatus.strmStatus)
    {
        // VPP Open init interrupt.
        case VPP_AMJR_STRM_ID_INIT:
            AVRS_TRACE(AVRS_INFO_TRC, 0, "AVRS: AMJR Handler: VPP_AMJR_STRM_ID_INIT IRQ");
            
            g_avrsTramePlayedNb = 0;
            // Init interrupt: the first buffer to encode is the first half 
            // of the PCM swap buffer.
            g_avrsVppAmjrAudioCfg.inBufAddr  = g_avrsPcmBuf_0;
            g_avrsVppAmjrAudioCfg.outBufAddr = g_avrsEncBuf_0;
            
            // Configure AMJR.
            vpp_AmjrAudioCfg(&g_avrsVppAmjrAudioCfg);
            
            // Schedule the encode of the first frame.
            vpp_AmjrScheduleOneAudioFrame(g_avrsVppAmjrAudioCfg.inBufAddr, g_avrsVppAmjrAudioCfg.outBufAddr);
            break;

        case VPP_AMJR_STRM_ID_AUDIO:
            avrs_VppAmjrAudioHandler(status);
            break;

        case VPP_AMJR_STRM_ID_VIDEO:
            avrs_VppAmjrVideoHandler(status);
            break;
    
        default:
            break;
            // We are never in this case
    }

    // profile
    AVRS_PROFILE_FUNCTION_EXIT(avrs_VppAmjrHandler);
}


