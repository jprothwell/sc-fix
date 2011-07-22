/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2004, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      spc_irq_handler.c                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      bcpu irq handler.                                                */
/*                                                                       */
/*************************************************************************/
// TOP
#include "chip_id.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)

#include "cs_types.h"
#include "gsm.h"
#include "baseband_defs.h"
// SPC
#include "spc_ctx.h"
#include "spc_private.h"
#include "spc_mailbox.h"
#include "spcp_debug.h"
#include "spc_profile_codes.h"
// SPAL
#include "spal_comregs.h"
#include "spal_cfg.h"
#include "spal_irq.h"
#include "spal_ifc.h"
#include "spal_tcu.h"
#include "spal_rfif.h"
#include "spal_mem.h"
// CHIP
#include "global_macros.h"
#include "sys_irq.h"
#include "bb_irq.h"
#include "tcu.h"
#include "rf_if.h"
#include "bb_ifc.h"

//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------

// sp context structure
extern SPC_CONTEXT_T g_spcCtxStruct;

// Forward declarations
extern VOID bb_prog_rx_win(UINT8 win_type,BOOL swapIQ);
BOOL g_spcIfc2Done=FALSE;
BOOL g_spcTxOff=FALSE;
//******************************************//
//*********                     ************//
//*********      IRQ Handler    ************//
//*********                     ************//
//******************************************//
PROTECTED VOID SPAL_FUNC_INTERNAL spc_IrqHandlerTcu1(SPAL_IRQ_STATUS_T status)
{
    UINT8 snap_nbr=0;
    // reset s0 to the g_spcCtxStruct address value
    SPC_CONTEXT_T* temp = g_spcCtx;
    g_spcCtx = &g_spcCtxStruct;

    switch (status)
    {
        case SPAL_IRQ_RX_HALF:
            // postpone the disable TCU Event for DTX on the RX FULL IRQ
            if (g_spcCtx->Tx_off==TRUE)
            {
                g_spcTxOff = TRUE;
                g_spcCtx->Tx_off = FALSE;
            }
            // Flag for servicing
            g_spcCtx->ifc2_pending++;
            g_spcCtx->bcpuIrqCause |= BB_IRQ_BB_IRQ_IFC2;
        break;
        
        case SPAL_IRQ_RX_FULL:
            if (!g_spcCtx->FcchActive)
            {
                UINT8 curWinType = g_mailbox.pal2spc.win[g_spcCtx->currentSnap].
                                            rx.rxTypeWin[g_spcCtx->rx_count];
                
                if((curWinType == GSM_WIN_TYPE_NBURST)||(curWinType == GSM_WIN_TYPE_SCH))
                {
                    g_spcIfc2Done=TRUE;
                }

                // Multislot Window
                if ((g_spcCtx->slot_count + 1) < g_spcCtx->rxslotNb)
                {
                    // Normal Burst are handled with IFC Fifo mode -> no IFC
                    // programmation needed, but Interference Win must be reprogrammed
                    if (curWinType == GSM_WIN_TYPE_INTRF)
                    {
                        bb_prog_rx_win( GSM_WIN_TYPE_INTRF,
                                        g_mailbox.pal2spc.win[g_spcCtx->currentSnap].
                                                    rx.swapIQ[g_spcCtx->rx_count]);
                    }
                    g_spcCtx->slot_count++;
                }
                else // Next Rx Window
                {
                    if (g_mailbox.pal2spc.win[g_spcCtx->currentSnap].rx.qty > (g_spcCtx->rx_count +1))
                    {
                        g_spcCtx->rx_count++; 
                        bb_prog_rx_win( g_mailbox.pal2spc.win[g_spcCtx->currentSnap].
                                                 rx.rxTypeWin[g_spcCtx->rx_count],
                                        g_mailbox.pal2spc.win[g_spcCtx->currentSnap].
                                                 rx.swapIQ[g_spcCtx->rx_count]);
                    }
                }

                // Flag the end of Nburst for SPP in case of EQU_HBURST_MODE
                // but don't flag for servicing as it's already done
                if ((curWinType == GSM_WIN_TYPE_NBURST) && g_spcCtx->equ_hburst_mode)
                {
                    // DTX is done here for HBURST equalization
                    if (g_spcTxOff==TRUE)
                    {
                        spal_TcuDisableEvent(g_mailbox.pal2spc.rf.dtxTcoSettings);
                    }
                    g_spcTxOff = FALSE;
                    spp_OneFullBurstReady();
                }
                else
                {
                    // Flag for servicing
                    g_spcCtx->ifc2_pending++;
                    g_spcCtx->bcpuIrqCause |= BB_IRQ_BB_IRQ_IFC2;
                }
            }
            else // FcchActive
            {
                // Flag for servicing
                g_spcCtx->ifc2_pending++;
                g_spcCtx->bcpuIrqCause |= BB_IRQ_BB_IRQ_IFC2;
            }        
        break;
        
        case SPAL_IRQ_FINT:
            snap_nbr = spal_ComregsGetSnap();

            // Check for Rx Window
            if(g_mailbox.pal2spc.win[snap_nbr].rx.qty)
            {
                // Init
                g_spcCtx->FcchActive = 0;
                g_spcCtx->rx_count = 0;
                g_spcCtx->slot_count = 0;
                spal_IfcDisableRf();

                // Handle HW according to win type
                bb_prog_rx_win( g_mailbox.pal2spc.win[snap_nbr].rx.rxTypeWin[0],
                                g_mailbox.pal2spc.win[snap_nbr].rx.swapIQ[0]);

            } // End Prog Rx on FINT
            
            // Flag for servicing
            g_spcCtx->bcpuIrqCause |= BB_IRQ_BB_IRQ_FRAME;
        
        break;
        
        case SPAL_IRQ_TCU:
            // Re-enable PA & XCVER TCO events
            spal_TcuEnableEvent(g_mailbox.pal2spc.rf.dtxTcoSettings);
        break;

        case SPAL_IRQ_OTHER:
        hwp_bbIrq->Pulse_Clear = BB_IRQ_BB_IRQ_TCU1;
        //check if the IFC2 irq has already been received
        if (!g_spcIfc2Done)
        {
            UINT8 curWinType = g_mailbox.pal2spc.win[g_spcCtx->currentSnap].
                                        rx.rxTypeWin[g_spcCtx->rx_count];

            // Multislot Window
            if ((g_spcCtx->slot_count + 1) < g_spcCtx->rxslotNb)
            {
                // Normal Burst are handled with IFC Fifo mode -> no IFC
                // programmation needed, but Interference Win must be reprogrammed
                if (curWinType == GSM_WIN_TYPE_INTRF)
                {
                    bb_prog_rx_win( GSM_WIN_TYPE_INTRF,
                                    g_mailbox.pal2spc.win[g_spcCtx->currentSnap].
                                                rx.swapIQ[g_spcCtx->rx_count]);
                }
                g_spcCtx->slot_count++;
            }
            else // Next Rx Window
            {
                if (g_mailbox.pal2spc.win[g_spcCtx->currentSnap].rx.qty > (g_spcCtx->rx_count +1))
                {
                    g_spcCtx->rx_count++; 
                    bb_prog_rx_win( g_mailbox.pal2spc.win[g_spcCtx->currentSnap].
                                             rx.rxTypeWin[g_spcCtx->rx_count],
                                    g_mailbox.pal2spc.win[g_spcCtx->currentSnap].
                                             rx.swapIQ[g_spcCtx->rx_count]);
                }
            }

            // Flag for servicing
            g_spcCtx->ifc2_pending++;
            g_spcCtx->bcpuIrqCause |= BB_IRQ_BB_IRQ_IFC2;
            
        }
        g_spcIfc2Done=FALSE;
        break;

        default:
#if (SPC_IF_VER != 0)
            // Call back hook.
            // Call a function defined by the XCPU, when the BCPU
            // receives an unknown IRQ.
            if (g_spcCtx->irqHook != NULL)
            {
                ((VOID(*)(VOID))g_spcCtx->irqHook)();
            }
#endif // (SPC_IF_VER != 0)
        break;
    }
    g_spcCtx = temp;

} // spc_IrqHandler

//======================================================================
// spc_SetIrqMaskTcu1
//----------------------------------------------------------------------
/// Setup the IRQ mask settings for BaseBand CPU
//======================================================================
VOID spc_IrqSetMaskTcu1(VOID)
{
    hwp_bbIrq->Mask_Set = BB_IRQ_BB_IRQ_IFC2 | BB_IRQ_BB_IRQ_FRAME | BB_IRQ_BB_IRQ_TCU0 | BB_IRQ_BB_IRQ_TCU1;
    hwp_bbIrq->WakeUp_Mask = BB_IRQ_BB_IRQ_IFC2 | BB_IRQ_BB_IRQ_FRAME | BB_IRQ_BB_IRQ_TCU0 | BB_IRQ_BB_IRQ_TCU1;
    hwp_bbIrq->Pulse_Mask_Set = BB_IRQ_BB_IRQ_FRAME | BB_IRQ_BB_IRQ_TCU0 | BB_IRQ_BB_IRQ_TCU1;
}

#endif // CHIP_ASIC_ID_GREENSTONE
