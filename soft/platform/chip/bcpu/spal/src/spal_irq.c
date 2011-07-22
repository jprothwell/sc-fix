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
#include "bb_irq.h"
#include "bb_ifc.h"

#include "global_macros.h"
// To get BB-IRQ numbers
#include "globals.h"
#include "spal_irq.h"
#include "spalp_debug.h"
#include "spal_mem.h"

// Global variable storing the irq handler function
PROTECTED SPAL_IRQ_HANDLER_T g_spalIrqRegistry  SPAL_BBSRAM_GLOBAL = NULL; 


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//======================================================================
// spal_SetIrqMask
//----------------------------------------------------------------------
/// Setup the IRQ mask settings for BaseBand CPU
//======================================================================
PROTECTED VOID spal_IrqSetMask(VOID)
{
    hwp_bbIrq->Mask_Set = BB_IRQ_BB_IRQ_IFC2 | BB_IRQ_BB_IRQ_FRAME | BB_IRQ_BB_IRQ_TCU0;
    hwp_bbIrq->WakeUp_Mask = BB_IRQ_BB_IRQ_IFC2 | BB_IRQ_BB_IRQ_FRAME | BB_IRQ_BB_IRQ_TCU0;
    hwp_bbIrq->Pulse_Mask_Set = BB_IRQ_BB_IRQ_FRAME | BB_IRQ_BB_IRQ_TCU0;
}


//======================================================================
// spal_IrqHandler
//----------------------------------------------------------------------
/// SPAL IRQ Dispatcher
//======================================================================

PROTECTED VOID spal_IrqHandler(VOID)
{
    // Read the cause
    UINT32 irq_cause = hwp_bbIrq->Cause;

    //  Give priority to IFC2 IRQ: 
    //  in case of late window the SPC Ctx must not be updated 

    // ************************** //
    // ****  IFC2 RX Window  **** //
    // ************************** //
    
    if ( irq_cause & BB_IRQ_BB_IRQ_IFC2 )
    {
        SPAL_PROFILE_IRQ_ENTER(BB_IRQ_IFC2);

        if (hwp_bbIfc->ch2_status & BB_IFC_CAUSE_IHTC)
        {
            hwp_bbIfc->ch2_int_clear = BB_IFC_HALF_TC;
            g_spalIrqRegistry(SPAL_IRQ_RX_HALF);
        }
        else 
        {
            hwp_bbIfc->ch2_int_clear = BB_IFC_END_TC; 
            g_spalIrqRegistry(SPAL_IRQ_RX_FULL);
        }

        SPAL_PROFILE_IRQ_EXIT(BB_IRQ_IFC2);
    }

    // **************** //
    // ****  FINT  **** //
    // **************** //
    
    if ( irq_cause & BB_IRQ_BB_IRQ_FRAME )
    {
        SPAL_PROFILE_FINT;   
       
        SPAL_PROFILE_IRQ_ENTER(BB_IRQ_FRAME);
        hwp_bbIrq->Pulse_Clear = BB_IRQ_BB_IRQ_FRAME;
        g_spalIrqRegistry(SPAL_IRQ_FINT);
        SPAL_PROFILE_IRQ_EXIT(BB_IRQ_FRAME);
    }
    
    // ******************* //
    // ****  TCU Irq  **** //
    // ******************* //
    
    if ( irq_cause & BB_IRQ_BB_IRQ_TCU0 )
    {
        SPAL_PROFILE_IRQ_ENTER(BB_IRQ_TCU0);
        hwp_bbIrq->Pulse_Clear = BB_IRQ_BB_IRQ_TCU0;
        g_spalIrqRegistry(SPAL_IRQ_TCU);
        SPAL_PROFILE_IRQ_EXIT(BB_IRQ_TCU0);
    }
    
#if (SPC_IF_VER != 0)
    // *********************** //
    // ****  Unknown IRQ  **** //
    // *********************** //
    
    if (!( (irq_cause & BB_IRQ_BB_IRQ_TCU0)  ||
           (irq_cause & BB_IRQ_BB_IRQ_FRAME) ||
           (irq_cause & BB_IRQ_BB_IRQ_IFC2) ))
    {
        SPAL_PROFILE_IRQ_ENTER(BB_IRQ_OTHER);
        g_spalIrqRegistry(SPAL_IRQ_OTHER);
        SPAL_PROFILE_IRQ_EXIT(BB_IRQ_OTHER);
    }
#endif // (SPC_IF_VER != 0)

}


