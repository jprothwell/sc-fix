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
//  $HeadURL: svn://atlas/SVN_SOFT/Granite/trunk/platform/calib/src/calibp_debug.h $
//	$Author: romuald $                                                        
//	$Date: 2007-10-04 12:43:01 +0200 (Thu, 04 Oct 2007) $                      
//	$Revision: 6630 $                                                           
//                                                                            
////////////////////////////////////////////////////////////////////////////////
//                                                                            
/// @file calibp_debug.h                                                      
/// Debug features to be used only inside the module (protected).             
/// If this file is used for all the debug needs of the module, it will be    
/// easier to disable those debug features for the whole module (by changing  
/// the value of a define while compiling).                                   
//                                                                            
////////////////////////////////////////////////////////////////////////////////

#ifndef  _CALIBP_DEBUG_H_
#define  _CALIBP_DEBUG_H_

#include "hal_debug.h"
#include "calib_profile_codes.h"
#include "sxs_io.h"



// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
/// @todo implement, replacing level by _CALIB or something
// =============================================================================
#ifndef CALIB_NO_PRINTF
#define CALIB_TRACE(level, tstmap, format, ...)  hal_DbgTrace(level,tstmap,format, ##__VA_ARGS__)
#else
#define CALIB_TRACE(level, tsmap, fmt, ...)
#endif



#ifdef CALIB_NO_ASSERT
#define CALIB_ASSERT(BOOL, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#define CALIB_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#endif



// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
// =============================================================================
#ifdef CALIB_PROFILING


// =============================================================================
//  CALIB_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define CALIB_PROFILE_PULSE(pulseName)  \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_HAL, (CP_ ## pulseName))


// =============================================================================
//  CALIB_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define CALIB_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, (CP_ ## eventName))


// =============================================================================
//  CALIB_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define CALIB_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, (CP_ ## eventName))


// =============================================================================
//  CALIB_PROFILE_WINDOW_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define CALIB_PROFILE_WINDOW_ENTER(eventName) \
        hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_HAL, (CP_ ## eventName))


// =============================================================================
//  CALIB_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define CALIB_PROFILE_WINDOW_EXIT(eventName) \
        hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_HAL, (CP_ ## eventName))


#else // CALIB_PROFILING

#define CALIB_PROFILE_FUNCTION_ENTER(eventName)
#define CALIB_PROFILE_FUNCTION_EXIT(eventName)
#define CALIB_PROFILE_WINDOW_ENTER(eventName)
#define CALIB_PROFILE_WINDOW_EXIT(eventName)
#define CALIB_PROFILE_PULSE(pulseName)

#endif // CALIB_PROFILING



#endif //_CALIBP_DEBUG_H_


