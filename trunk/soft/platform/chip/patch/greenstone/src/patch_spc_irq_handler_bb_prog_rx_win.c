/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2004, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      patch_spc_irq_handler_bb_prog_rx_win.c                           */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      replace the function bb_prog_rx_win form spc_irq_handler.c       */
/*      using the hardware patch mecanism                                */
/*                                                                       */
/*************************************************************************/
// TOP
#include "cs_types.h"
#include "gsm.h"
#include "baseband_defs.h"
#include "chip_id.h"
// SPC
#include "spc_ctx.h"
//#include "spc_private.h"
#include "spc_mailbox.h"
//#include "spcp_debug.h"
#include "spc_profile_codes.h"
// SPAL
#include "spal_comregs.h"
#include "spal_cfg.h"
#include "spal_irq.h"
#include "spal_ifc.h"
#include "spal_mem.h"
#include "spal_tcu.h"
#include "spal_rfif.h"
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

/********************************************************************************/
/*                          bb_prog_rx_win                                      */
/********************************************************************************/
VOID SPAL_FUNC_INTERNAL patch_bb_prog_rx_win(UINT8 win_type,BOOL swapIQ)
{
    // TODO: to save space we only have 3 MonIfcBuffer.
    //       this implies that INTRF process for slot 0 MUST
    //       be finished before slot 3 reception
    UINT32 mask=0;
    UINT8 MonIdx = g_spcCtx->slot_count % 3;

    if (g_spcCtx->rx_count)
        spal_IfcDisableRf();

    if (swapIQ)
        g_spcCtx->RfIfCtrl |= RF_IF_RX_SWAP_I_Q_YES;
    else
        g_spcCtx->RfIfCtrl &= ~RF_IF_RX_SWAP_I_Q_YES;

    // Apply change to RfIf
    spal_RfIfWriteCtrl(g_spcCtx->RfIfCtrl | RF_IF_RX_FIFO_RESET);

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
                            BB_IFC_ENABLE | BB_IFC_FIFO_MODE);
        break;
            
        case GSM_WIN_TYPE_MONIT:  
            // XXX: this statement uses the "case fallback" and must stay as is.
            // (i.e. don't move cases, don't add a break...)
            MonIdx = g_spcCtx->rx_count;
        case GSM_WIN_TYPE_INTRF:
            // Setup RFIF
            //FIXME AGATE g_spcCtx->RfIfCtrl &= ~RF_IF_RX_DE_ENABLE;
            // Setup IFC
            spal_IfcParamRf(BB_IFC_END_TC,
                            (UINT32) &(g_spcStaticBufTab.MonIfcBuffer[MonIdx]),
                            0,
                            MBOX_MON_WIN_SIZE,
                            BB_IFC_ENABLE);
        break;
        
        case GSM_WIN_TYPE_SCH:    
            // Setup IFC
            spal_IfcParamRf(BB_IFC_END_TC,
                            (UINT32) &(g_spcStaticBufTab.RxIfcBuffer[1]),
                            0,
                            (2*BB_RX_SCH_MARGIN+BB_BURST_ACTIVE_SIZE),
                            BB_IFC_ENABLE);
        break;
        
        case GSM_WIN_TYPE_FCCH:   
            // Setup IFC
            spal_IfcParamRf(BB_IFC_END_TC,
                            (UINT32) &(g_spcStaticBufTab.RxIfcBuffer[0]),
                            (UINT32) &(g_spcStaticBufTab.RxIfcBuffer[0])+
                                                              BB_FCCH_SIZE*3*4,
                            BB_FCCH_SIZE,
                            BB_IFC_ENABLE | BB_IFC_FIFO_MODE);

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
}


