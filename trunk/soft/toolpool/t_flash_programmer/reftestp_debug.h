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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/toolpool/trunk/t_flash_programmer/reftestp_debug.h $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-09-28 17:31:33 +0800 (Tue, 28 Sep 2010) $                     //   
//	$Revision: 2609 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file playsp_cfg.h                                                          //
/// That file provides the trace, assert and profiling macros for the         //
//  PLAYS module.                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "hal_debug.h"
/// @todo include the profile codes here
//#include "reftest_profile_codes.h"
#include "sxs_io.h"

#ifndef _REFTESTP_CFG_H_
#define _REFTESTP_CFG_H_



// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
/// @todo implement, replacing level by _HAL or something
// =============================================================================
#if (REFTEST_NO_PRINTF == 1)
#define REFTEST_TRACE(level, tsmap, fmt, ...)
#else
#define REFTEST_TRACE(level, tsmap, format, ...) \
            hal_DbgTrace(_MMI | level, tsmap, format, ##__VA_ARGS__)
#endif



#ifdef REFTEST_NO_ASSERT
#define REFTEST_ASSERT(BOOL, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#define REFTEST_ASSERT(BOOL, format, ...) \
    if (!(BOOL)) { \
        hal_DbgAssert(format, ##__VA_ARGS__); \
    }
#endif



// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
/// @todo Implement them with hal_DbgProfilingEnter etc instead of this :
// =============================================================================
#ifdef REFTEST_PROFILING


// =============================================================================
//  REFTEST_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define REFTEST_PROFILE_PULSE(pulseName) \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_SVC, (CP_ ## pulseName))


// =============================================================================
//  REFTEST_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the beginning of a profiled function or window.
// =============================================================================
#define REFTEST_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  REFTEST_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define REFTEST_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  REFTEST_PROFILE_WINDOW_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the beginning of a profiled function or window.
// =============================================================================
#define REFTEST_PROFILE_WINDOW_ENTER(eventName) \
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  REFTEST_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define REFTEST_PROFILE_WINDOW_EXIT(eventName) \
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


#else // REFTEST_PROFILING

#define REFTEST_PROFILE_FUNCTION_ENTER(eventName)
#define REFTEST_PROFILE_FUNCTION_EXIT(eventName)
#define REFTEST_PROFILE_WINDOW_ENTER(eventName)
#define REFTEST_PROFILE_WINDOW_EXIT(eventName)
#define REFTEST_PROFILE_PULSE(pulseName)

#endif // REFTEST_PROFILING



#endif // _REFTESTP_CFG_H_

