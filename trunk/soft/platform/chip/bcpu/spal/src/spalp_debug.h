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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spal/src/spalp_debug.h $ //
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
#include "spal_profile_codes.h"

#ifndef _SPALP_DEBUG_H_
#define _SPALP_DEBUG_H_

// =============================================================================
//  BB IRQ defines for profiling
// =============================================================================
#define BB_IRQ_OTHER    0xF

// =============================================================================
//  PROFILING
// -----------------------------------------------------------------------------
/// Those are the macros to use for profiling
/// @todo Implement them with spal_DbgProfilingEnter etc instead of this :
// =============================================================================
#ifdef SPAL_PROFILING


// =============================================================================
//  SPAL_PROFILE_FUNCTION_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define SPAL_PROFILE_FUNCTION_ENTER(eventName) \
        spal_DbgPxtsProfileFunctionEnter(SPAL_DBG_PXTS_SPAL, (CP_ ## eventName))


// =============================================================================
//  SPAL_PROFILE_FUNCTION_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define SPAL_PROFILE_FUNCTION_EXIT(eventName) \
        spal_DbgPxtsProfileFunctionExit(SPAL_DBG_PXTS_SPAL, (CP_ ## eventName))


// =============================================================================
//  SPAL_PROFILE_WINDOW_ENTER
// -----------------------------------------------------------------------------
/// Use this macro at the begining of a profiled function or window.
// =============================================================================
#define SPAL_PROFILE_WINDOW_ENTER(eventName) \
        spal_DbgPxtsProfileWindowEnter(SPAL_DBG_PXTS_SPAL, (CP_ ## eventName))


// =============================================================================
//  SPAL_PROFILE_WINDOW_EXIT
// -----------------------------------------------------------------------------
/// Use this macro at the end of a profiled function or window.
// =============================================================================
#define SPAL_PROFILE_WINDOW_EXIT(eventName) \
        spal_DbgPxtsProfileWindowExit(SPAL_DBG_PXTS_SPAL, (CP_ ## eventName))


// =============================================================================
//  SPAL_PROFILE_PULSE
// -----------------------------------------------------------------------------
/// Use this macro to generate a profiling pulse.
// =============================================================================
#define SPAL_PROFILE_PULSE(pulseName)  \
                spal_DbgPxtsProfilePulse(SPAL_DBG_PXTS_SPAL, (CP_ ## pulseName))


#else // SPAL_PROFILING

#define SPAL_PROFILE_FUNCTION_ENTER(eventName)
#define SPAL_PROFILE_FUNCTION_EXIT(eventName)
#define SPAL_PROFILE_WINDOW_ENTER(eventName)
#define SPAL_PROFILE_WINDOW_EXIT(eventName)
#define SPAL_PROFILE_PULSE(pulseName)

#endif // SPAL_PROFILING


#ifdef SPAL_IRQ_PROFILING
#define SPAL_PROFILE_IRQ_ENTER(eventName) \
        spal_DbgPxtsSendCode(SPAL_DBG_PXTS_BB_IRQ, \
        (CPIRQSPACESTART |((eventName) & CPIRQSPACEMASK)) | CPBCPU)
#define SPAL_PROFILE_IRQ_EXIT(eventName) \
        spal_DbgPxtsSendCode(SPAL_DBG_PXTS_BB_IRQ, \
        (CPIRQSPACESTART | ((eventName) & CPIRQSPACEMASK) | CPEXITFLAG)| CPBCPU)
#else // SPAL_IRQ_PROFILING
#define SPAL_PROFILE_IRQ_ENTER(eventName)
#define SPAL_PROFILE_IRQ_EXIT(eventName)
#endif

// TODO: Always active?
#define SPAL_PROFILE_FINT spal_DbgPxtsSendCode(SPAL_DBG_PXTS_BB_IRQ, CPFINT)



#endif // _SPALP_DEBUG_H_







