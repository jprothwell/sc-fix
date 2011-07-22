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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spp/src/sppp_debug.h $ //
//    $Author: admin $                                                        //
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //
//    $Revision: 269 $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file
/// Debug features to be used only inside the module (protected).
/// Is this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "spal_debug.h"
/// @todo include the profile codes here
#include "spp_profile_codes.h"

#ifndef _SPPP_DEBUG_H_
#define _SPPP_DEBUG_H_



// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
/// @todo implement
// =============================================================================
// Conditional Printf Usage
#ifndef CS_PLATFORM_SIMU
#ifndef SPP_NO_TRACE
#define SPP_TRACE(level, tstmap, format, ...)  spp_DbgTrace(level,tstmap,format, ##__VA_ARGS__)
#else
#define SPP_TRACE(level, tstmap, format, ...)
#endif //SPP_NO_PRINTF
#endif                                                  // #ifndef CS_PLATFORM_SIMU



#ifndef CS_PLATFORM_SIMU
#ifdef SPP_NO_ASSERT
#define SPP_ASSERT(boolCondition, format, ...)
#else
// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false,
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#define SPP_ASSERT(boolCondition, format, ...)                   \
    if (!(boolCondition)) {            \
        spp_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#endif
#endif                                                  // #ifndef CS_PLATFORM_SIMU



// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
/// @todo Implement them with spp_DbgProfilingEnter etc instead of this :
// =============================================================================
#ifdef SPP_PROFILING


// =============================================================================
//  SPP_PROFILE_FUNCTION_ENTRY
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define SPP_PROFILE_FUNCTION_ENTRY(eventName) \
        spal_DbgPxtsProfileFunctionEnter(SPAL_DBG_PXTS_SPP, (CP_ ## eventName))


// =============================================================================
//  SPP_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define SPP_PROFILE_FUNCTION_EXIT(eventName) \
        spal_DbgPxtsProfileFunctionExit(SPAL_DBG_PXTS_SPP, (CP_ ## eventName))


// =============================================================================
//  SPP_PROFILE_WINDOW_ENTRY
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define SPP_PROFILE_WINDOW_ENTRY(eventName) \
        spal_DbgPxtsProfileWindowEnter(SPAL_DBG_PXTS_SPP, (CP_ ## eventName))


// =============================================================================
//  SPP_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define SPP_PROFILE_WINDOW_EXIT(eventName) \
        spal_DbgPxtsProfileWindowExit(SPAL_DBG_PXTS_SPP, (CP_ ## eventName))


// =============================================================================
//  SPP_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define SPP_PROFILE_PULSE(pulseName)  \
                spal_DbgPxtsProfilePulse(SPAL_DBG_PXTS_SPP, (CP_ ## pulseName))


#else // SPP_PROFILING

#define SPP_PROFILE_FUNCTION_ENTRY(eventName)
#define SPP_PROFILE_FUNCTION_EXIT(eventName)
#define SPP_PROFILE_WINDOW_ENTRY(eventName)
#define SPP_PROFILE_WINDOW_EXIT(eventName)
#define SPP_PROFILE_PULSE(pulseName)

#endif // SPP_PROFILING



#endif // _SPPP_DEBUG_H_







