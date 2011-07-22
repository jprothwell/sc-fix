//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_misc.c
///
/// This file contains miscellanous functions for SPP
///
/// @date 16/05/07
/// @author Laurent Bastide
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// include
#include "cs_types.h"
#include "global_macros.h"
#include "spal_irq.h"
#include "spal_mem.h"


//------------------------------------------------------------------------------
// globals (must be at a fixed place in SRAM)
//------------------------------------------------------------------------------
volatile UINT8 g_sppFullBurstReady SPAL_BBSRAM_GLOBAL = 0;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

// Functions used for EQU_HBURST_MODE mechanism
PUBLIC UINT8 spp_FullBurstReady(VOID)
{
    return g_sppFullBurstReady;
}

PUBLIC VOID spp_ResetFullBurstReady(VOID)
{
    g_sppFullBurstReady = 0;
}

PUBLIC VOID spp_OneFullBurstReady(VOID)
{
    volatile UINT32 critical_sec = spal_IrqEnterCriticalSection();     
    g_sppFullBurstReady++;
    spal_IrqExitCriticalSection(critical_sec);
}

PUBLIC VOID spp_OneFullBurstDone(VOID)
{
    volatile UINT32 critical_sec = spal_IrqEnterCriticalSection();     
    g_sppFullBurstReady--;
    spal_IrqExitCriticalSection(critical_sec);
}



