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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_ana_rfif.h $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_ana_rfif.h                                                      //
/// That file provides the function related to the analog macro Driver.       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_ANA_RF_IF_
#define _HAL_ANA_RF_IF_


/// @defgroup ana_rf_if HAL ANA RF-IF
/// This is the driver of the RF Interface.
/// @{


// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_AnaRfIfTurnOn
// -----------------------------------------------------------------------------
/// Turn on the RF interface.
///
/// This function requests a resource of #HAL_SYS_FREQ_26M only for a pll to be on.
// =============================================================================
PUBLIC VOID hal_AnaRfIfTurnOn();



// =============================================================================
// hal_AnaRfIfTurnOff
// -----------------------------------------------------------------------------
/// Turn off the RF interface.
///
/// This function release the resource to #HAL_SYS_FREQ_32K.
// =============================================================================
PUBLIC VOID hal_AnaRfIfTurnOff(VOID);



/// @} // <-- End of the ana_rf_if group

#endif // _HAL_ANA_RF_IF_

