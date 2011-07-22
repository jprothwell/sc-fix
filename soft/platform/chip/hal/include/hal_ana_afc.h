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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_ana_afc.h $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_ana_afc.h                                                           //
/// That file describes the AFC driver API                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef  _HAL_AFC_H_
#define  _HAL_AFC_H_

#include "cs_types.h"

/// @defgroup ana_afc HAL ANA AFC Driver
///
/// @{

// =============================================================================
//  FUNCTIONS 
// =============================================================================

// =============================================================================
// hal_AnaAfcOpen
// -----------------------------------------------------------------------------
/// Power Up and Initialize the AFC.
/// Doesn't take a resource.
// =============================================================================
PUBLIC VOID hal_AnaAfcOpen(VOID);

// =============================================================================
// hal_AnaAfcFlushFifo
// -----------------------------------------------------------------------------
/// Flushes the AFC Fifo
// =============================================================================
PUBLIC VOID hal_AnaAfcFlushFifo(VOID);

// =============================================================================
// hal_AnaAfcClose
// -----------------------------------------------------------------------------
/// Close the AFC.
// =============================================================================
PUBLIC VOID hal_AnaAfcClose(VOID);

// =============================================================================
// hal_AnaAfcNxtFrameValue
// -----------------------------------------------------------------------------
/// Program AFC value that will be set at next frame
/// @param data AFC value set at the next frame.
// =============================================================================
PUBLIC VOID hal_AnaAfcNxtFrameValue(UINT32 data);

// =============================================================================
// hal_AnaAfcForceValue
// -----------------------------------------------------------------------------
/// Force an AFC value
/// @param data AFC value immediately forced.
// =============================================================================
PUBLIC VOID hal_AnaAfcForceValue(UINT32 data);

/// @} // <--end of the afc group

#endif // _HAL_AFC_H_



