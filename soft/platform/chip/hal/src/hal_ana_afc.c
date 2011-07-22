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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_ana_afc.c $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///     @file hal_ana_afc.c
///     Implementation of Granite's AFC Driver
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "global_macros.h"

#include "hal_tcu.h"
#include "hal_ana_afc.h"
#include "hal_mem_map.h"
#include "afc.h"
#include "ana_acco.h"

#define AFC_NEXT_VAL_DATE 0

// =============================================================================
// hal_AnaAfcOpen
// -----------------------------------------------------------------------------
/// Power Up and Initialize the AFC.
/// Takes a resource.
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_AnaAfcOpen(VOID)
{
    hwp_anaAccoFe->pa_afc_gp_ctrl_set = ANA_ACCO_AFC_RESET_RELEASE|ANA_ACCO_AFC_ENABLE;
    hwp_afc->AFC_Ctrl = AFC_AFC_FLUSH_FIFO;
}

// =============================================================================
// hal_AnaAfcFlushFifo
// -----------------------------------------------------------------------------
/// Flushes the AFC Fifo
// =============================================================================
PUBLIC VOID hal_AnaAfcFlushFifo(VOID)
{
    hwp_afc->AFC_Ctrl = AFC_AFC_FLUSH_FIFO;
}

// =============================================================================
// hal_AnaAfcClose
// -----------------------------------------------------------------------------
/// Close the AFC. Free a resource.
// =============================================================================
PUBLIC VOID hal_AnaAfcClose(VOID)
{
    hwp_anaAccoFe->pa_afc_gp_ctrl_clr = ANA_ACCO_AFC_RESET_N |ANA_ACCO_AFC_DISABLE;
}

// =============================================================================
// hal_AnaAfcSleep
// -----------------------------------------------------------------------------
/// Puts the AFC to sleep.
// =============================================================================
PUBLIC VOID hal_AnaAfcSleep(VOID)
{
    //#warning "This function (and file) has to be implemented."
    ///@todo : Do we need to implement a sleep function ?
}


// =============================================================================
// hal_AnaAfcWakeup
// -----------------------------------------------------------------------------
/// Wakes the AFC.
// =============================================================================
PUBLIC VOID hal_AnaAfcWakeup(VOID)
{
    //#warning "This function (and file) has to be implemented."
    ///@todo : Do we need to implement a wake function ?
}

// =============================================================================
// hal_AnaAfcNxtFrameValue
// -----------------------------------------------------------------------------
/// Program AFC value that will be set at next frame
/// @param data AFC value set at the next frame.
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_AnaAfcNxtFrameValue(UINT32 data)
{
    hwp_afc->AFC_Data = AFC_AFC_DATA(data);
    hal_TcuSetEvent(HAL_TCU_SET_NEXT_FC, AFC_NEXT_VAL_DATE);
}

// =============================================================================
// hal_AnaAfcForceValue
// -----------------------------------------------------------------------------
/// Force an AFC value
/// @param data AFC value immediately forced.
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_AnaAfcForceValue(UINT32 data)
{
    hwp_afc->AFC_Ctrl = AFC_AFC_FORCE_DATA(data) | AFC_AFC_FORCE_UPDATE;
}
