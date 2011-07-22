//------------------------------------------------------------------------------
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
//------------------------------------------------------------------------------
//                                                                            //
//  $Name: spal_irq.h $                                                       //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                   //   
//    $Revision: 269 $                                                         //   
//                                                                            //
//------------------------------------------------------------------------------
//                                                                            //
/// @file spal_irq_driver.S                                                   //
/// Not romed IRQ driver to choose to use the handler in INT SRAM instead of  //
/// the one in ROM.                                                           //
//                                                                            //
//------------------------------------------------------------------------------

#ifndef __SPAL_IRQ_H__
#define __SPAL_IRQ_H__


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "global_macros.h"
#include "bb_irq.h"


//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

// ============================================================================
// SPAL_IRQ_STATUS_T
// ----------------------------------------------------------------------------
/// This structure is used to represent the irq status
// ============================================================================
typedef enum 
{
    /// Frame interrupt
    SPAL_IRQ_FINT,
    /// Rx channel half buffer interrupt
    SPAL_IRQ_RX_HALF,
    /// Rx channel full buffer interrupt
    SPAL_IRQ_RX_FULL,
    /// Tcu interrupt
    SPAL_IRQ_TCU,
    /// Unknown IRQ, used for IRQ hook
    SPAL_IRQ_OTHER
            
} SPAL_IRQ_STATUS_T;

// ============================================================================
// SPAL_IRQ_HANDLER_T
// ----------------------------------------------------------------------------
/// User handler function type
// ============================================================================
typedef VOID (*SPAL_IRQ_HANDLER_T)(SPAL_IRQ_STATUS_T);


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------


// =============================================================================
// spal_IrqEnterCriticalSection
// -----------------------------------------------------------------------------
/// This function enters in a critical section (relative to the hardware 
/// interrupts) and return a status that should be given to the 
/// #spal_IrqExitCriticalSection function.
/// 
/// @return  The \c status that should be given to the #spal_IrqExitCriticalSection 
/// function.
// =============================================================================
INLINE UINT32 spal_IrqEnterCriticalSection(VOID)
{
    UINT32 status = hwp_bbIrq->SC;
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
    asm volatile ("");
    return status;
}


// =============================================================================
// spal_IrqExitCriticalSection
// -----------------------------------------------------------------------------
/// This function leaves the critical section. The <CODE>status</CODE> 
/// parameter is the return value of the #spal_IrqEnterCriticalSection call
/// that opened the critical section.
/// 
/// @param status The value returned by #spal_IrqEnterCriticalSection.
// =============================================================================
INLINE VOID spal_IrqExitCriticalSection(UINT32 status)
{
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
    asm volatile ("");
    hwp_bbIrq->SC = status;
}

// =============================================================================
// spal_IrqWriteCpuSleep
// -----------------------------------------------------------------------------
/// This function writes <CODE>mode</CODE> in the CPU Sleep register. 
/// 
/// @param mode 
// =============================================================================
INLINE VOID spal_IrqWriteCpuSleep(UINT32 mode)
{
    hwp_bbIrq->Cpu_Sleep = mode;
}

// =============================================================================
// spal_IrqReadCpuSleep
// -----------------------------------------------------------------------------
/// This function reads and returns the value of CPU Sleep register. 
/// 
/// @return  The \c mode 
// =============================================================================
INLINE UINT32 spal_IrqReadCpuSleep(VOID)
{
    UINT32 mode = hwp_bbIrq->Cpu_Sleep;
    return mode;
}

#endif // __SPAL_IRQ_H__


