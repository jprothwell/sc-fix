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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/base/branches/gallite441/dbg/src/dbg_profile.c $
//  $Author: admin $
//  $Date: 2010-07-07 20:26:37 +0800 (Wed, 07 Jul 2010) $
//  $Revision: 268 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file dbg_profile.c
/// Implement the profiling mechanisms. Please refer to the CoolProfile tool 
/// documentation for details.
//
////////////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "chip_id.h"

#include "hal_debug.h"



// =============================================================================
// DBG_PXTS_LEVEL
// -----------------------------------------------------------------------------
/// When several levels are available for PXTS code, that one is used by 
/// DBG.
// =============================================================================
// FIXME Choose a value
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#define DBG_PXTS_LEVEL  0
#else
#define DBG_PXTS_LEVEL  HAL_DBG_PXTS_MMI
#endif


// =============================================================================
// 
// -----------------------------------------------------------------------------
/// 
// =============================================================================

// =============================================================================
// dbg_PxtsSendTrigger
// -----------------------------------------------------------------------------
/// Send to the profiling tool the code correponding to a pre-defined "trigger
/// tag". If the "PXTS Trigger" mode is enabled in the profiling tool, this
/// "trigger tag" will stop the PXTS recording. This can be used to stop the
/// PXTS recording from the embedded code.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsSendTrigger(VOID)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#else
    hal_DbgPxtsSendTrigger(DBG_PXTS_LEVEL);
#endif
}



// =============================================================================
// dbg_PxtsProfileFunctionEnter
// -----------------------------------------------------------------------------
/// This function has to be called when entering the function you want to profile.
/// 
/// @param functionId Code representing the function.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfileFunctionEnter(UINT16 functionId)
{
    hal_DbgPxtsProfileFunctionEnter(DBG_PXTS_LEVEL, functionId);
}



// =============================================================================
// dbg_PxtsProfileFunctionExit
// -----------------------------------------------------------------------------
/// This function has to be called when exiting the function you want to profile.
///
/// @param functionId Code representing the function.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfileFunctionExit(UINT16 functionId)
{
    hal_DbgPxtsProfileFunctionExit(DBG_PXTS_LEVEL, functionId);
}



// =============================================================================
// dbg_PxtsProfileWindowEnter
// -----------------------------------------------------------------------------
/// This function has to be called to mark the entrance in a window.
/// 
/// @param windowId Code representing the window which has been entered in.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfileWindowEnter(UINT16 windowId)
{
    hal_DbgPxtsProfileWindowEnter(DBG_PXTS_LEVEL, windowId);
}



// =============================================================================
// dbg_PxtsProfileWindowExit
// -----------------------------------------------------------------------------
/// This function has to be called to mark the exit of a window.
/// 
/// @param windowId Code representing the window which has been exited.
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfileWindowExit(UINT16 windowId)
{
    hal_DbgPxtsProfileWindowExit(DBG_PXTS_LEVEL, windowId);
}





// =============================================================================
// dbg_PxtsProfilePulse
// -----------------------------------------------------------------------------
/// Send to the profiling tool the code correponding to a pulse.
/// 
/// @param code Code representing the pulse
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID dbg_PxtsProfilePulse(UINT16 code)
{
    hal_DbgPxtsProfilePulse(DBG_PXTS_LEVEL, code);
}

