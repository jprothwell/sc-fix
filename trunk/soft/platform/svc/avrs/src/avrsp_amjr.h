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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/avrs/src/avrsp_amjr.h $
//  $Author: romuald $
//  $Date: 2009-02-04 20:10:45 +0800 (Wed, 04 Feb 2009) $
//  $Revision: 21930 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file XXXmodXXX_init.h
///
/// This document describes the HAL Init functions. Bla bla bla...
/// 
/// @par My very interesting paragraph
/// 
/// Bla bla bla...
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _AVRSP_AMJPG_H_
#define _AVRSP_AMJPG_H_

#include "cs_types.h"
#include "vpp_amjr.h"

/// Audio configuration for the AMJR decoder.
/// Initial setup is done by #avrs_Record.
EXPORT PROTECTED VPP_AMJR_AUDIO_CFG_T g_avrsVppAmjrAudioCfg ;

// =============================================================================
// avrs_VppAmjrHandler
// -----------------------------------------------------------------------------
/// Handler of the 'real' interruption from the VPP AMJR module. It dispatches
/// the causes to call accordingly one or both the handler for the audio or the
/// video handler.
/// It also handle the initialization interrupt cause.
///
/// @param status VPP AMJR interrupt status.
// =============================================================================
PROTECTED VOID avrs_VppAmjrHandler(HAL_VOC_IRQ_STATUS_T* status);


// =============================================================================
// avrs_VppAmjrAudioHandler
// -----------------------------------------------------------------------------
/// Handler for the audio cause of the VPP AMJR interrupt.
/// @param status VPP AMJR interrupt status.
// =============================================================================
PROTECTED VOID avrs_VppAmjrAudioHandler(HAL_VOC_IRQ_STATUS_T* status);


// =============================================================================
// avrs_VppAmjrVideoHandler
// -----------------------------------------------------------------------------
/// Handler for the video cause of the VPP AMJR interrupt.
/// @param status VPP AMJR interrupt status.
// =============================================================================
PROTECTED VOID avrs_VppAmjrVideoHandler(HAL_VOC_IRQ_STATUS_T* status);


#endif // _AVRSP_AMJPG_H_










