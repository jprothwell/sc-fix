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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/halp_ana_gpadc.h $ //
//    $Author: huazeng $                                                       //
//    $Date: 2010-08-03 17:00:19 +0800 (Tue, 03 Aug 2010) $                     //
//    $Revision: 959 $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_ana_gpadc.h                                                     //
///                                                                           //
/// Contains the GPADC driver private API                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef  _HALP_ANA_GPADC_H_
#define  _HALP_ANA_GPADC_H_

#include "cs_types.h"
#include "hal_ana_gpadc.h"




// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_AnaGpadcGpadc2Volt
// -----------------------------------------------------------------------------
/// Convert the value measured by the GPADC into mV, using the calibration
/// parameters.
///
/// @param gpadcVal Value read from the GPADC hardware module.
/// @return The corresponding measured tension, in mV.
// =============================================================================
PROTECTED HAL_ANA_GPADC_MV_T hal_AnaGpadcGpadc2Volt(UINT16 gpadcVal);


// =============================================================================
// hal_AnaGpadcCalibBandgap
// -----------------------------------------------------------------------------
// Calibration of the analog macros' bandgap.
// =============================================================================
PROTECTED VOID hal_AnaGpadcCalibBandgap(VOID);


// =======================================================
// hal_AnaGpadcSleep
// -------------------------------------------------------
/// called in #hal_LpsSleep to try to shut down the GPADC
/// and release the resource. (only if atp >= 250MS)
// =======================================================
PROTECTED VOID hal_AnaGpadcSleep(VOID);

// =======================================================
// hal_AnaGpadcWakeUp
// -------------------------------------------------------
/// called in #hal_LpsSleep to resume the GPADC operation
/// can take back the resource (if GPADC is open)
// =======================================================
PROTECTED VOID hal_AnaGpadcWakeUp(VOID);



#endif // _HALP_ANA_GPADC_H_



