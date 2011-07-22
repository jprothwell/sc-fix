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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/avrs/src/avrsp_debug.h $ //
//	$Author: romuald $                                                        // 
//	$Date: 2009-02-20 02:12:26 +0800 (Fri, 20 Feb 2009) $                     //   
//	$Revision: 22265 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file playsp_cfg.h                                                          //
/// That file provides the trace, assert and profiling macros for the         //
//  PLAYS module.                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "hal_debug.h"
#include "sxs_io.h"
#include "avrs_profile_codes.h"
#include "svcp_debug.h"

#ifndef _AVRSP_CFG_H_
#define _AVRSP_CFG_H_


// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
#define AVRS_WARN_TRC   SVC_WARN_TRC

/// Informational trace interesting for AVRS only
#define AVRS_INFO_TRC   SVC_AVRS_TRC

/// reserved for debug (can be very verbose, but should probably not stay in code)
#define AVRS_DBG_TRC    SVC_DBG_TRC


// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
// =============================================================================
#ifndef AVRS_NO_PRINTF
#define AVRS_TRACE(level, tsmap, format, ...)  SVC_TRACE(level,tsmap,format, ##__VA_ARGS__)
#else
#define AVRS_TRACE(level, tsmap, fmt, ...)
#endif



#ifdef AVRS_NO_ASSERT
#define AVRS_ASSERT(BOOL, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#define AVRS_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#endif



// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
/// @todo Implement them with hal_DbgProfilingEnter etc instead of this :
// =============================================================================
#ifdef AVRS_PROFILING


// =============================================================================
//  AVRS_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define AVRS_PROFILE_PULSE(pulseName)  \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_SVC, (CP_ ## pulseName))


// =============================================================================
//  AVRS_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define AVRS_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  AVRS_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define AVRS_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  AVRS_PROFILE_WINDOW_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define AVRS_PROFILE_WINDOW_ENTER(eventName) \
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  AVRS_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define AVRS_PROFILE_WINDOW_EXIT(eventName) \
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


#else // AVRS_PROFILING

#define AVRS_PROFILE_FUNCTION_ENTER(eventName)
#define AVRS_PROFILE_FUNCTION_EXIT(eventName)
#define AVRS_PROFILE_WINDOW_ENTER(eventName)
#define AVRS_PROFILE_WINDOW_EXIT(eventName)
#define AVRS_PROFILE_PULSE(pulseName)

#endif // AVRS_PROFILING



#endif // _AVRSP_CFG_H_

