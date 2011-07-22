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
//  $HeadURL$ //
//	$Author$                                                        // 
//	$Date$                     //   
//	$Revision$                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file dll_cfg.h                                                          //
/// That file provides the trace, assert and profiling macros for the         //
//  PLAYS module.                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "hal_debug.h"
/// @todo include the profile codes here
#include "sxs_io.h"


#ifndef _DLL_CFG_H_
#define _DLL_CFG_H_

#define DLL_TRACE_MAX_LEVEL 16
// =============================================================================
// DLL_ERR_T
// -----------------------------------------------------------------------------
/// This types is used to describes all possible error returned by DLL's
/// API functions.
// =============================================================================
typedef enum
{
    /// No error occured
    DLL_ERR_NO,

    /// Unsupported operation or parameter
    DLL_ERR_UNSUPPORTED,

    DLL_ERR_QTY
} DLL_ERR_T;


// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
/// @todo implement, replacing level by _HAL or something
// =============================================================================
#ifndef DLL_NO_PRINTF
#define _DL_TRACEOUT(nModuleIdx, pFormat, ...)  TS_OutputText(nModuleIdx, pFormat, ##__VA_ARGS__)
#define DLL_TRACE(level, tsmap, format, ...)  hal_DbgTrace(level,tsmap,format, ##__VA_ARGS__)
#else
#define _DL_TRACEOUT(nModuleIdx, pFormat, ...) 
#define DLL_TRACE(level, tsmap, fmt, ...)
#endif



#ifdef DLL_NO_ASSERT
#define DLL_ASSERT(BOOL, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#define DLL_ASSERT(BOOL, format, ...)                   \
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
#ifdef DLL_PROFILING


// =============================================================================
//  DLL_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define DLL_PROFILE_PULSE(pulseName)  \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_SVC, (CP_ ## pulseName))


// =============================================================================
//  DLL_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define DLL_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  DLL_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define DLL_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  DLL_PROFILE_WINDOW_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define DLL_PROFILE_WINDOW_ENTER(eventName) \
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


// =============================================================================
//  DLL_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define DLL_PROFILE_WINDOW_EXIT(eventName) \
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))


#else // DLL_PROFILING

#define DLL_PROFILE_FUNCTION_ENTER(eventName)
#define DLL_PROFILE_FUNCTION_EXIT(eventName)
#define DLL_PROFILE_WINDOW_ENTER(eventName)
#define DLL_PROFILE_WINDOW_EXIT(eventName)
#define DLL_PROFILE_PULSE(pulseName)

#endif // DLL_PROFILING



#endif // _DLL_CFG_H_

