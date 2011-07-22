//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spal_cfg.h
///
/// This file provides the header for the configuration of spal.   
///
/// @date 29/01/08
/// @author Nadia Touliou
//-----------------------------------------------------------------------------

#ifndef __SPAL_CFG_H__
#define __SPAL_CFG_H__

#include "cs_types.h"
#include "spal_irq.h"

typedef struct
{
    BOOL irqInRom;                 /// Use irq contained on the ROM vs. RAM
    SPAL_IRQ_HANDLER_T irqHandler; /// Set VoC user defined interrupt handler function
    BOOL simuUL;                   /// test an uplink conding scheme, for model simulation only

} SPAL_GLOBAL_PARAMS_T;


// =============================================================================
// spal_Open
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function initializes the global parameter structure used by SPAL.
/// @param pCfg SPAL_GLOBAL_PARAMS_T. Pointer to the configuration structure.
// =============================================================================
PUBLIC VOID spal_Open (SPAL_GLOBAL_PARAMS_T* pCfg);




#endif

