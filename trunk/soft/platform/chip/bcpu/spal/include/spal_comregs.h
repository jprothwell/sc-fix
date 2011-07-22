//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spal_comregs.h
///
/// Comregs functions for SPC. 
///
/// @date 16/05/07
/// @author Laurent Bastide
//-----------------------------------------------------------------------------

#ifndef __SPAL_COMREGS_H__
#define __SPAL_COMREGS_H__

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "comregs.h"
#include "global_macros.h"


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//======================================================================
// spal_ComregsSetIrq
//----------------------------------------------------------------------
//  Set Inter-Cpu Interrupt Cause
// 
// @param Irq UINT32. Bitmap of cause to be set. Depending on the current
//              Comregs Irq Mask setup this will trigger an interrupt
//              to the Xcpu.
//======================================================================
INLINE VOID spal_ComregsSetIrq(UINT32 Irq)
{
    hwp_bbComregs->ItReg_Set = Irq;
}

//======================================================================
// spal_ComregsClrIrq
//----------------------------------------------------------------------
// Clear Inter-Cpu Interrupt Cause
//
// @param Irq UINT32. Bitmap of cause to be cleared.
//
//======================================================================
INLINE VOID spal_ComregsClrIrq(UINT32 Irq)
{
    hwp_bbComregs->ItReg_Clr = Irq;
}

//======================================================================
// spal_ComregsGetSnap
//----------------------------------------------------------------------
// Get current Frame snapshot index
//
// @return Current Frame snapshot index
//======================================================================
INLINE UINT8 spal_ComregsGetSnap(VOID)
{
    return ((UINT8)hwp_bbComregs->Snapshot);
}

#endif // __SPAL_COMREGS_H__

