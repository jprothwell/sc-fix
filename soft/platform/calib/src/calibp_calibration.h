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
//  $HeadURL: svn://atlas/SVN_SOFT/Granite/trunk/platform/calib/src/calibp_calibration.h $
//	$Author: romuald $
//	$Date: 2007-10-05 16:37:32 +0200 (Fri, 05 Oct 2007) $
//	$Revision: 6638 $
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file calibp_calibration.h                                                //
/// That file provides the private interface for the calibration module.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _CALIBP_CALIBRATION_H_
#define _CALIBP_CALIBRATION_H_

#include "cs_types.h"
#include "calib_m.h"

// =============================================================================
// g_calibCalibration
// -----------------------------------------------------------------------------
/// Calibration Structure.
/// As we are in the calibration module, the structure can be directly accessed,
/// without using a pointer, as it is declared in this module.
// =============================================================================
EXPORT PROTECTED CALIB_CALIBRATION_T g_calibCalibration;



#endif // _CALIBP_CALIBRATION_H_



