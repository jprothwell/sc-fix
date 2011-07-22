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
//  $HeadURL$
//  $Author$
//  $Date$
//  $Revision$
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file mpsp_debug.h
///
/// Multimedia player service debug function
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MPSP_DEBUG_H_
#define _MPSP_DEBUG_H_

#include "cs_types.h"

#include "hal_debug.h"

#include "sxs_io.h"

#include "mps_profile_codes.h"


// =============================================================================
// MACROS
// =============================================================================

#ifdef MPS_PRINTF
#define MPS_TRACE(format, ...)                                    \
    hal_DbgTrace(TSTDOUT, 0, format, ##__VA_ARGS__)
#else
#define MPS_TRACE(fmt, ...)
#endif


#ifdef MPS_NO_ASSERT
#define MPS_ASSERT(BOOL, format, ...)
#else
#define MPS_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#endif

#ifdef MPS_PROFILING

// =============================================================================
//  MPS_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define MPS_PROFILE_FUNCTION_ENTER(eventName) \
        hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))

// =============================================================================
//  MPS_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define MPS_PROFILE_FUNCTION_EXIT(eventName) \
        hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))

// =============================================================================
//  MPS_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define MPS_PROFILE_PULSE(pulseName)  \
        hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_SVC, (CP_ ## pulseName))

#else

#define MPS_PROFILE_FUNCTION_ENTER(eventName)
#define MPS_PROFILE_FUNCTION_EXIT(eventName)
#define MPS_PROFILE_PULSE(pulseName)

#endif /* MPS_PROFILING */

// =============================================================================
// TYPES
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES
// =============================================================================


// =============================================================================
// FUNCTIONS
// =============================================================================

#endif // _MPSP_DEBUG_H_
