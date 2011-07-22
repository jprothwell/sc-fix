//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spalp_private.h
///
/// Header for private SPAL functions and definitions. 
///
/// @date 26/06/07
/// @author Guillaume LEBRUN
//-----------------------------------------------------------------------------


#ifndef SPALP_PRIVATE_H
#define SPALP_PRIVATE_H


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
// spal public headers
#include "spal_vitac.h"
#include "spal_cap.h"
#include "spal_irq.h"


//------------------------------------------------------------------------------
// Defines 
//------------------------------------------------------------------------------


// Number/size of Tsc Cross-correlation for time syncronization
#define NB_TSC_XCOR_SIZE 15
#define SCH_TSC_XCOR_SIZE 63


//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------

EXPORT PROTECTED SPAL_IRQ_HANDLER_T g_spalIrqRegistry;


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//======================================================================
// spal_InvalidateDcache 
//----------------------------------------------------------------------
/// Data cache invalidation
/// !!! THIS FUNCTION SHOULD BE COMPILED IN MIPS32 !!!
///
/// @addtogroup misc_func
//======================================================================
PROTECTED VOID spal_InvalidateDcache(VOID);


//======================================================================
// spal_InvalidateCache 
//----------------------------------------------------------------------
// Both caches invalidation
// !!! THIS FUNCTION SHOULD BE COMPILED IN MIPS32 !!!
//======================================================================
PROTECTED VOID spal_InvalidateCache(VOID);


//======================================================================
// spal_IrqInit
//======================================================================
/// Select the handler to use for the bcpu
/// @param irqInRam If \c TRUE, the IRQ handler in internal ROM is used.
/// If \c FALSE, the one in internal SRAM is used.
//======================================================================
PROTECTED VOID spal_IrqInit(BOOL irqInRom);


//======================================================================
// spal_SetIrqMask
//----------------------------------------------------------------------
// Setup the IRQ mask settings for BaseBand CPU
//
// @todo see if we want to give more control to SPC
//======================================================================
PROTECTED VOID spal_IrqSetMask(VOID);


// ============================================================================
// spal_IrqHandler
// ----------------------------------------------------------------------------
/// spal IRQ handler, clearing the IRQ cause regiter and calling the user handler
/// defined by g_spalIrqRegistry. 
// ============================================================================
PROTECTED VOID spal_IrqHandler(VOID);


#endif // SPALP_PRIVATE_H
