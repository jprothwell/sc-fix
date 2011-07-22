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
//  $HeadURL: svn://atlas/SVN_SOFT/Granite/trunk/platform/calib/src/calibp_stub.h $
//	$Author: romuald $
//	$Date: 2007-10-05 16:37:32 +0200 (Fri, 05 Oct 2007) $
//	$Revision: 6638 $
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file calibp_stub.h                                                       //
/// That file provides the private API for the calibration stub               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _CALIBP_STUB_H_
#define _CALIBP_STUB_H_

#include "cs_types.h"
#include "calib_m.h"



// =============================================================================
// g_calibStubGlobalCtxPtr
// -----------------------------------------------------------------------------
/// Pointert to the calibration context written by the Host to send command
/// to the CES.
// =============================================================================
EXPORT PROTECTED VOLATILE CALIB_STUB_CTX_T*	g_calibStubGlobalCtxPtr;



// =============================================================================
// g_calibStubLocalCtx
// -----------------------------------------------------------------------------
/// Context used by the CES, copied from the #g_calibStubGlobalCtx.
// =============================================================================
EXPORT PROTECTED CALIB_STUB_CTX_T			g_calibStubLocalCtx;



// =============================================================================
// calib_DispState
// -----------------------------------------------------------------------------
/// Display depending on the states.
/// @param full If \c TRUE,  clear the screen and rewrite everything.
// =============================================================================
PROTECTED VOID calib_DispState(BOOL full);



// =============================================================================
// calib_PulseLight
// -----------------------------------------------------------------------------
/// I am alive and I claim it ! function, through a PWL (Keyboard backlighting
/// presumably. The pulsing speed is used to display the calibration state.
///
/// @param speed Speed of the glow.
// =============================================================================
PROTECTED VOID calib_PulseLight(INT32 speed);




// =============================================================================
// calib_StubOpen
// -----------------------------------------------------------------------------
/// Initializes the calibration stub. Among operation is the registering of the 
/// calib stub state machine as the FINT Irq handler.
// =============================================================================
PROTECTED VOID calib_StubOpen(VOID);



// =============================================================================
// calib_StubTaskInit
// -----------------------------------------------------------------------------
/// Calib Stub OS task.
// =============================================================================
PROTECTED VOID calib_StubTaskInit(VOID);


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================





#endif // _CALIBP_STUB_H_

