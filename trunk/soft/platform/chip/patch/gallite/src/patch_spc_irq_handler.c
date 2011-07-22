/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2004, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      patch_spc_irq_handler.c                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      bcpu irq handler.                                                */
/*                                                                       */
/*************************************************************************/
// TOP
#include "cs_types.h"
#include "gsm.h"
#include "baseband_defs.h"
#include "chip_id.h"
// SPC
#include "spc_ctx.h"
#include "spc_private.h"
#include "spc_mailbox.h"
//#include "spcp_debug.h"
#include "spc_profile_codes.h"
// SPAL
#include "spal_comregs.h"
#include "spal_cfg.h"
#include "spal_irq.h"
#include "spal_ifc.h"
#include "spal_tcu.h"
#include "spal_rfif.h"
// CHIP
#include "global_macros.h"
#include "sys_irq.h"
#include "bb_irq.h"
#include "tcu.h"
#include "rf_if.h"
#include "bb_ifc.h"


#include "spal_mem.h"
#include "patch_spal_debug.h"
#define BB_PROG_RX_WIN_EVENT_BASE 0x5000


//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------

// sp context structure
extern SPC_CONTEXT_T g_spcCtxStruct;

#ifdef CHIP_HAS_EXTRA_WINDOWS
extern SPC_CONTEXT_EXT_T g_spcCtxStructExt;
#endif

#ifdef ENABLE_PATCH_SPC_IRQHANDLER

// Rx slot number to identify whether multislot window programming is finished.
extern UINT32 g_spcRxSlotNb;

BOOL SPAL_DATA_INTERNAL g_spcIfc2Done=FALSE;
// Forward declarations
VOID bb_prog_rx_win(UINT8 win_type,BOOL swapIQ);

//******************************************//
//*********                     ************//
//*********      IRQ Handler    ************//
//*********                     ************//
//******************************************//
PROTECTED VOID SPAL_FUNC_INTERNAL patch_spc_IrqHandler(SPAL_IRQ_STATUS_T status)
{
    // Always read the snap number from h/w, for g_spcCtx->currentSnap
    // might NOT be updated when the 2nd win is being programmed.
    UINT8 snap_nbr = spal_ComregsGetSnap();
    // reset s0 to the g_spcCtxStruct address value
    SPC_CONTEXT_T* temp = g_spcCtx;
    g_spcCtx = &g_spcCtxStruct;

    switch (status)
    {
        case SPAL_IRQ_RX_HALF:
            // Flag for servicing
            g_spcCtx->ifc2_pending++;
            g_spcCtx->bcpuIrqCause |= BB_IRQ_BB_IRQ_IFC2;
        break;
        
        case SPAL_IRQ_RX_FULL:
            if (!g_spcCtx->FcchActive)
            {
             #ifdef CHIP_HAS_EXTRA_WINDOWS  
                UINT8 curWinType = g_spcCtx->rx_count < MBOX_WINDOWS_QTY ?  
                    g_mailbox.pal2spc.win[snap_nbr].rx.rxTypeWin[g_spcCtx->rx_count]:
                    g_mailbox.extRef->pal2spc.rxwin[snap_nbr].rxTypeWin[g_spcCtxStructExt.rx_count];
              #else
                UINT8 curWinType = g_mailbox.pal2spc.win[snap_nbr].rx.rxTypeWin[g_spcCtx->rx_count];
              #endif

                // Multislot Window
                if ((g_spcCtx->slot_count + 1) < g_spcRxSlotNb)
                {
                    // Normal Burst are handled with IFC Fifo mode -> no IFC
                    // programmation needed, but Interference Win must be reprogrammed
                    if (curWinType == GSM_WIN_TYPE_INTRF)
                    {
                        bb_prog_rx_win( GSM_WIN_TYPE_INTRF,
                                        g_mailbox.pal2spc.win[snap_nbr].
                                                    rx.swapIQ[g_spcCtx->rx_count]);
                    }
                    g_spcCtx->slot_count++;
                }
                else // Next Rx Window
                {
                    if (g_mailbox.pal2spc.win[snap_nbr].rx.qty > (g_spcCtx->rx_count +1))
                    {
                        g_spcCtx->rx_count++; 
                        bb_prog_rx_win( g_mailbox.pal2spc.win[snap_nbr].
                                                 rx.rxTypeWin[g_spcCtx->rx_count],
                                        g_mailbox.pal2spc.win[snap_nbr].
                                                 rx.swapIQ[g_spcCtx->rx_count]);
                    }
#ifdef CHIP_HAS_EXTRA_WINDOWS
                    else if(g_mailbox.extRef->pal2spc.rxwin[snap_nbr].qty > g_spcCtxStructExt.rx_count)
                    {
                        g_spcCtx->rx_count++; 

                         bb_prog_rx_win( g_mailbox.extRef->pal2spc.rxwin[snap_nbr].
                                                 rxTypeWin[g_spcCtxStructExt.rx_count],
                                        g_mailbox.extRef->pal2spc.rxwin[snap_nbr].
                                                 swapIQ[g_spcCtxStructExt.rx_count]);                       
                    
                        g_spcCtxStructExt.rx_count++;
                        
                    }
#endif
                    if((curWinType == GSM_WIN_TYPE_NBURST)||(curWinType == GSM_WIN_TYPE_SCH))
                    {
                         g_spcIfc2Done=TRUE;
                    }			
                }

                // Flag the end of Nburst for SPP in case of EQU_HBURST_MODE
                // but don't flag for servicing as it's already done
                if ((curWinType == GSM_WIN_TYPE_NBURST) && g_spcCtx->equ_hburst_mode)
                {
                    // DTX is done here for HBURST equalization
                    if (g_spcCtx->Tx_off==TRUE)
                    {
                        spal_TcuDisableEvent(g_mailbox.pal2spc.rf.dtxTcoSettings);
                    }
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
            // Check for Rx Window
            if(g_mailbox.pal2spc.win[snap_nbr].rx.qty)
            {
                // Init
                g_spcCtx->FcchActive = 0;
                g_spcCtx->rx_count = 0;
                g_spcCtx->slot_count = 0;
#ifdef CHIP_HAS_EXTRA_WINDOWS
                g_spcCtxStructExt.rx_count = 0;
#endif
                g_spcRxSlotNb = 0;
                UINT8 RxBitMap = g_mailbox.pal2spc.win[snap_nbr].rx.nBurst.bitMap;
                while(RxBitMap)
                {
                    if ((RxBitMap&0x1)==1) g_spcRxSlotNb++;
                    RxBitMap>>=1;
                }

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
            UINT8 curWinType = g_mailbox.pal2spc.win[snap_nbr].
                                        rx.rxTypeWin[g_spcCtx->rx_count];

            // Multislot Window
            if ((g_spcCtx->slot_count + 1) < g_spcCtx->rxslotNb)
            {
                // Normal Burst are handled with IFC Fifo mode -> no IFC
                // programmation needed, but Interference Win must be reprogrammed
                if (curWinType == GSM_WIN_TYPE_INTRF)
                {
                    bb_prog_rx_win( GSM_WIN_TYPE_INTRF,
                                    g_mailbox.pal2spc.win[snap_nbr].
                                                rx.swapIQ[g_spcCtx->rx_count]);
                }
                g_spcCtx->slot_count++;
            }
            else // Next Rx Window
            {
                if (g_mailbox.pal2spc.win[snap_nbr].rx.qty > (g_spcCtx->rx_count +1))
                {
                    g_spcCtx->rx_count++; 
                    bb_prog_rx_win( g_mailbox.pal2spc.win[snap_nbr].
                                             rx.rxTypeWin[g_spcCtx->rx_count],
                                    g_mailbox.pal2spc.win[snap_nbr].
                                             rx.swapIQ[g_spcCtx->rx_count]);
                }
#ifdef CHIP_HAS_EXTRA_WINDOWS
                else if(g_mailbox.extRef->pal2spc.rxwin[snap_nbr].qty > g_spcCtxStructExt.rx_count)
                {
                    g_spcCtx->rx_count++; 

                     bb_prog_rx_win( g_mailbox.extRef->pal2spc.rxwin[snap_nbr].
                                             rxTypeWin[g_spcCtxStructExt.rx_count],
                                    g_mailbox.extRef->pal2spc.rxwin[snap_nbr].
                                             swapIQ[g_spcCtxStructExt.rx_count]);                       
                
                    g_spcCtxStructExt.rx_count++;
                    
                }
#endif                
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

#endif // ENABLE_PATCH_SPC_IRQHANDLER



//******************************************//
//*********                     ************//
//*********     Subroutines     ************//
//*********                     ************//
//******************************************//

#ifdef ENABLE_PATCH_BB_PROG_RX_WIN

/********************************************************************************/
/*                          bb_prog_rx_win                                      */
/********************************************************************************/
VOID SPAL_FUNC_INTERNAL patch_bb_prog_rx_win(UINT8 win_type,BOOL swapIQ)
{
    // TODO: to save space we only have 3 MonIfcBuffer.
    //       this implies that INTRF process for slot 0 MUST
    //       be finished before slot 3 reception
    UINT32 mask=0;
    UINT32 fcch_ifc_mode=0;
    UINT32 sbnb_ifc_mode=0;
    UINT8 MonIdx = g_spcCtx->slot_count % 3;

    if (g_spcCtx->rx_count)
        spal_IfcDisableRf();

    if (swapIQ)
        g_spcCtx->RfIfCtrl |= RF_IF_RX_SWAP_I_Q_YES;
    else
        g_spcCtx->RfIfCtrl &= ~RF_IF_RX_SWAP_I_Q_YES;
    
    // Apply change to RfIf
    // Reset the Rx Fifo BEFORE enabling the IFC...
    spal_RfIfWriteCtrl(g_spcCtx->RfIfCtrl | RF_IF_RX_FIFO_RESET);

    if (g_spcCtx->ifc2_burst_mode_sbnb)
    {
        sbnb_ifc_mode = BB_IFC_BURST_SIZE;
    }

    SPAL_BUFFER_PROFILE_WINDOW_ENTER_RAW(BB_PROG_RX_WIN_EVENT_BASE + win_type);

    switch (win_type)
    {
        case GSM_WIN_TYPE_NBURST:
            // Setup IFC
            if (g_spcCtx->equ_hburst_mode)
            {
                mask = BB_IFC_END_TC | BB_IFC_HALF_TC;
            }
            else
            {
                mask = BB_IFC_END_TC;
            }
            spal_IfcParamRf(mask,
                            (UINT32) &(g_spcStaticBufTab.RxIfcBuffer[0]),
                            (UINT32) &(g_spcStaticBufTab.RxIfcBuffer[0]) +
                                                       BB_BURST_TOTAL_SIZE*4*4,
                            BB_BURST_TOTAL_SIZE,
                            BB_IFC_ENABLE | BB_IFC_FIFO_MODE | sbnb_ifc_mode);
        break;
            
        case GSM_WIN_TYPE_MONIT:  
#ifdef CHIP_HAS_EXTRA_WINDOWS            
        case GSM_WIN_TYPE_MONIT_EXT:
#endif
            // XXX: this statement uses the "case fallback" and must stay as is.
            // (i.e. don't move cases, don't add a break...)
            MonIdx = g_spcCtx->rx_count;
        case GSM_WIN_TYPE_INTRF:
            // Setup IFC
            spal_IfcParamRf(BB_IFC_END_TC,
                            (UINT32) &(g_spcStaticBufTab.MonIfcBuffer[MonIdx*(g_spcCtx->MonWinSize)]),
                            0,
                            (UINT32) (g_spcCtx->MonWinSize),
                            BB_IFC_ENABLE | sbnb_ifc_mode);
        break;
        
        case GSM_WIN_TYPE_SCH:    
            // Setup IFC
            spal_IfcParamRf(BB_IFC_END_TC,
                            (UINT32) &(g_spcStaticBufTab.RxIfcBuffer[(BB_BURST_ACTIVE_SIZE+2*BB_RX_NBURST_MARGIN)]),
                            0,
                            (2*BB_RX_SCH_MARGIN+BB_BURST_ACTIVE_SIZE),
                            BB_IFC_ENABLE | sbnb_ifc_mode);
        break;
        
        case GSM_WIN_TYPE_FCCH:   
            // Setup IFC
            if (g_spcCtx->ifc2_burst_mode_fcch)
            {
                fcch_ifc_mode = BB_IFC_BURST_SIZE;
            }
            spal_IfcParamRf(BB_IFC_END_TC,
                            (UINT32) &(g_spcStaticBufTab.RxIfcBuffer[0]),
                            (UINT32) &(g_spcStaticBufTab.RxIfcBuffer[0])+
                            (UINT32) (g_spcCtx->FcchWinSize)*3*4,
                            (UINT32) (g_spcCtx->FcchWinSize),
                            BB_IFC_ENABLE | BB_IFC_FIFO_MODE | fcch_ifc_mode);

            g_spcCtx->FcchInit = 1;
            if (g_spcCtx->rx_count == 0) // First win
            {
                g_spcCtx->FcchActive = 1;
            }
            else
            {
                g_spcCtx->FcchSet = 1;
            }
        break;
    }
    
    SPAL_BUFFER_PROFILE_WINDOW_EXIT_RAW(BB_PROG_RX_WIN_EVENT_BASE + win_type);
}

#endif // ENABLE_PATCH_BB_PROG_RX_WIN


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

