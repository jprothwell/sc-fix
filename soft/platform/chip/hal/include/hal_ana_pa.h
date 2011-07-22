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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_ana_pa.h $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///     @file hal_ana_pa.h                                                  
///     This file contains the power amplifier driver API.                    
//////////////////////////////////////////////////////////////////////////////// 




#ifndef  _HAL_ANA_PA_H_
#define  _HAL_ANA_PA_H_

#include "cs_types.h"

/// @defgroup ana_pa HAL ANA PA amplifier driver
///
/// @{

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
// HAL_ANA_PA_MAX_RAMP_QTY
// -----------------------------------------------------------------------------
/// Maximum number of programmable ramps  in the PA.
// =============================================================================
#define HAL_ANA_PA_MAX_RAMP_QTY 5




// =============================================================================
//  TYPES
// =============================================================================
// =============================================================================
// HAL_ANA_PA_RAMP_T
// -----------------------------------------------------------------------------
/// Description of the characteristics of a PA ramp.
/// Each value corresponds to one 10 bits DAC value of the ramp
// =============================================================================
typedef struct
{
    UINT32 id;
    UINT16 rampDacVal[16];
} HAL_ANA_PA_RAMP_T;



// =============================================================================
//  FUNCTIONS 
// =============================================================================

// =============================================================================
// hal_AnaPaOpen
// -----------------------------------------------------------------------------
/// Configure and enable the PA controller
///
/// This function requests a resource of #HAL_SYS_FREQ_26M only for a pll to be on.
///
/// @param rampOn16Qbits If \c TRUE,  the ramp duration
/// is set to 16 quarter bits, with a new value every quarter bit \n
///                        If \c FALSE, the ramp duration
/// is set to 32 quarter bits, with a new value every other quarter bit.
// =============================================================================
PUBLIC VOID hal_AnaPaOpen(BOOL rampOn16Qbits);



// =============================================================================
// hal_AnaPaClose
// -----------------------------------------------------------------------------
/// Disable the PA controller
///
/// This function release the resource to #HAL_SYS_FREQ_32K.
// =============================================================================
PUBLIC VOID hal_AnaPaClose(VOID);



// =============================================================================
// hal_AnaPaSetRamp
// -----------------------------------------------------------------------------
/// Program a ramp in the PA controller
/// The controller can store up to 5 ramps, which 
/// are indexed by the \c .id field of the #HAL_ANA_PA_RAMP_T
/// structure
///
/// @param ramp The ramp to program. 
// =============================================================================
PUBLIC VOID hal_AnaPaSetRamp(HAL_ANA_PA_RAMP_T ramp);



// =============================================================================
// hal_AnaPaSetRampCalib
// -----------------------------------------------------------------------------
/// This is a "calibration only" version of the ramp
/// driver. It generates 2 ramps of linear DAC values
/// between minDac and dacValue.
///
/// @param minDac Minimum value, start of the first ramp, end of the second.
/// @param dacValue Summit Dac Value, end of the first ramp and start of the
/// second.
// =============================================================================
PUBLIC VOID hal_AnaPaSetRampCalib (UINT16 minDac,UINT16 dacValue);



/// @} //<-- end of the padac group

#endif // _HAL_ANA_PA_H_


