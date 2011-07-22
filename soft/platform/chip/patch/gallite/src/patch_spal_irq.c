//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file patch_spal_irq.c
///
/// BB Irq driver functions. 
///
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/patch/gallite/src/patch_spal_irq.c $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2010-09-15 11:35:22 +0800 (Wed, 15 Sep 2010) $                     //   
//    $Revision: 2234 $                                                        //   
//-----------------------------------------------------------------------------

#ifdef ENABLE_PATCH_SPAL_IRQHANDLER

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
//#include "spalp_debug.h"
#include "spal_mem.h"

#include "patch_spal_debug.h"
#include "hal_debug.h"
#include "spal_ifc.h"


// Global variable storing the irq handler function
extern PROTECTED SPAL_IRQ_HANDLER_T g_spalIrqRegistry; 

extern UINT32 xcpu_error_code;
extern PROTECTED VOID spal_InvalidateDcache(VOID);

#define BB_IFC2_RX_NUM_EVENT_BASE 0x6000

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

// NOTE:
// It is better to call spal_GdbBreakRaise() instead of spal_GdbBreakSoftBreakPoint().
// However, spal_GdbBreakRaise() is not put in the spal_GdbGBreak table.
#define spal_SoftBreak()             asm volatile("break 1\n"            \
"nop\n"                                                                  \
"nop\n"                                                                  \
"nop\n")

PROTECTED VOID patch_spal_DbgAssert(VOID)
{
    volatile UINT32 critical_sec;
    critical_sec = spal_IrqEnterCriticalSection();
    
    spal_IfcDisableRf();
    spal_InvalidateDcache();
    while(1)
    {
        spal_SoftBreak();
    }
    
    spal_IrqExitCriticalSection(critical_sec);
}

//======================================================================
// spal_IrqHandler
//----------------------------------------------------------------------
/// SPAL IRQ Dispatcher
//======================================================================

PROTECTED VOID SPAL_FUNC_INTERNAL patch_spal_IrqHandler(VOID)
{
    // Read the cause
    UINT32 irq_cause = hwp_bbIrq->Cause;

    UINT32 ifc2_rx_num = hwp_bbIfc->ch2_cur_ahb_addr - hwp_bbIfc->ch2_start_addr;
    if (ifc2_rx_num > 0x7ff) ifc2_rx_num = 0x7ff;
    ifc2_rx_num |= BB_IFC2_RX_NUM_EVENT_BASE;
    SPAL_BUFFER_PROFILE_WINDOW_ENTER_RAW(ifc2_rx_num);
    SPAL_BUFFER_PROFILE_WINDOW_EXIT_RAW(ifc2_rx_num);

    UINT32 uncached_xcpu_error_code = *(VOLATILE UINT32 *)MEM_ACCESS_UNCACHED(&xcpu_error_code);
    if ( 0 != uncached_xcpu_error_code &&
        HAL_DBG_ERROR_NO != uncached_xcpu_error_code &&
        HAL_DBG_ERROR_GDB != uncached_xcpu_error_code )
    {
        patch_spal_DbgAssert();
    }
    
    //  Give priority to IFC2 IRQ: 
    //  in case of late window the SPC Ctx must not be updated 

    // ************************** //
    // ****  IFC2 RX Window  **** //
    // ************************** //
    
    if ( irq_cause & BB_IRQ_BB_IRQ_IFC2 )
    {
        SPAL_BUFFER_PROFILE_IRQ_ENTER(BB_IRQ_IFC2);

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

        SPAL_BUFFER_PROFILE_IRQ_EXIT(BB_IRQ_IFC2);
    }

    // **************** //
    // ****  FINT  **** //
    // **************** //
    
    if ( irq_cause & BB_IRQ_BB_IRQ_FRAME )
    {
        SPAL_BUFFER_PROFILE_FINT;   
       
        SPAL_BUFFER_PROFILE_IRQ_ENTER(BB_IRQ_FRAME);
        hwp_bbIrq->Pulse_Clear = BB_IRQ_BB_IRQ_FRAME;
        g_spalIrqRegistry(SPAL_IRQ_FINT);
        SPAL_BUFFER_PROFILE_IRQ_EXIT(BB_IRQ_FRAME);
    }
    
    // ******************* //
    // ****  TCU Irq  **** //
    // ******************* //
    
    if ( irq_cause & BB_IRQ_BB_IRQ_TCU0 )
    {
        SPAL_BUFFER_PROFILE_IRQ_ENTER(BB_IRQ_TCU0);
        hwp_bbIrq->Pulse_Clear = BB_IRQ_BB_IRQ_TCU0;
        g_spalIrqRegistry(SPAL_IRQ_TCU);
        SPAL_BUFFER_PROFILE_IRQ_EXIT(BB_IRQ_TCU0);
    }
    
#if (SPC_IF_VER != 0)
    // *********************** //
    // ****  Unknown IRQ  **** //
    // *********************** //
    
    if (!( (irq_cause & BB_IRQ_BB_IRQ_TCU0)  ||
           (irq_cause & BB_IRQ_BB_IRQ_FRAME) ||
           (irq_cause & BB_IRQ_BB_IRQ_IFC2) ))
    {
        SPAL_BUFFER_PROFILE_IRQ_ENTER(BB_IRQ_OTHER);
        g_spalIrqRegistry(SPAL_IRQ_OTHER);
        SPAL_BUFFER_PROFILE_IRQ_EXIT(BB_IRQ_OTHER);
    }
#endif // (SPC_IF_VER != 0)

}

#endif // ENABLE_PATCH_SPAL_IRQHANDLER

