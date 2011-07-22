//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spal_irq.c
///
/// BB Irq driver functions. 
///
/// @date 05/02/08
/// @author Yordan Tabakov
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "spalp_private.h"
#include "spal_cfg.h"
#include "global_macros.h"


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

// =============================================================================
// spal_Open
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function initializes the global parameter structure used by SPAL.
/// @param pCfg SPAL_GLOBAL_PARAMS_T. Pointer to the configuration structure.
// =============================================================================
PUBLIC VOID spal_Open (SPAL_GLOBAL_PARAMS_T* pCfg)
{
    spal_IrqInit(pCfg->irqInRom);
    g_spalIrqRegistry = pCfg->irqHandler;
    spal_IrqSetMask();
    

}


