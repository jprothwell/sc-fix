//------------------------------------------------------------------------------
//              Copyright (C) 2004, Coolsand Technologies, Inc.               
//                            All Rights Reserved                             
//------------------------------------------------------------------------------
/// @file spc_rx_ext.c
///
/// This file contains extended windows process
///

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
// TOP
#include "cs_types.h"
#include "gsm.h"
#include "baseband_defs.h"
#include "chip_id.h"
// SPC
#include "spc_ctx.h"
#include "spc_private.h"
#include "spc_mailbox.h"
#include "spcp_spp_cfg.h"
#include "spc_profile_codes.h"
// SPAL
#include "spal_comregs.h"
#include "spal_irq.h"
#include "spal_rfif.h"
#include "spal_mem.h"
// CHIP
#include "global_macros.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "bb_irq.h"
#include "bb_ifc.h"
#include "tcu.h"
#include "rf_if.h"

#include "patch_spal_debug.h"


#ifdef CHIP_HAS_EXTRA_WINDOWS
//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------

PUBLIC  SPC_CONTEXT_EXT_T  g_spcCtxStructExt SPAL_DATA_INTERNAL;


//=============================================================================
// spc_UpdateOnFintExt()
//-----------------------------------------------------------------------------
/// This function updates all the external context variables that change on FINT
//=============================================================================

PROTECTED VOID SPAL_FUNC_INTERNAL spc_UpdateOnFintExt(VOID)
{
    g_spcCtxStructExt.rxQty = g_mailbox.extRef->pal2spc.rxwin[g_spcCtx->currentSnap].qty; 
   
    if ((g_spcCtxStructExt.rxQty > 0) )
    {
        g_spcCtxStructExt.sched_rx_count=0;    
        g_spcCtxStructExt.sched_slot_count = 0;
        g_spcCtxStructExt.rx_count_flag = 0;
    }
    g_spcCtxStructExt.frame_over=0;

}

//=============================================================================
// spc_MonitoringProcessExt()
//-----------------------------------------------------------------------------
/// This function processes the monitoring results
//=============================================================================
PRIVATE VOID SPAL_FUNC_INTERNAL spc_MonitoringProcessExt(UINT8 rxCount, UINT8 winIdx)
{
    UINT32 Power;
        
    spp_GetMonPower(rxCount,(UINT32 *) &(g_spcStaticBufTab.MonIfcBuffer[rxCount*(g_spcCtx->MonWinSize)]),&Power);
    
    if(winIdx < MBOX_WINDOWS_QTY){
        g_mailbox.spc2pal.win[g_spcCtx->currentSnap].rx.monRes[winIdx].power = Power;
    }
    else{
        g_mailbox.extRef->spc2pal.rxwin[g_spcCtx->currentSnap].monRes[winIdx-MBOX_WINDOWS_QTY].power = Power;
    }
    
}

//=============================================================================
// spc_NBurstProcessExt(SPC_RX_LOCALS_T *spRx)
//-----------------------------------------------------------------------------
/// This function processes the Normal Burst reception for extra windows
//=============================================================================
 PUBLIC VOID  SPAL_FUNC_INTERNAL spc_NBurstProcessExt(SPC_RX_LOCALS_T *spRx)
{
 u32 *reg_debug_port_base = (u32*)hwp_debugPort;
 void (*spc_NBurstProcess)(VOID)  = (VOID (*)(VOID) )0x81e10841;
__asm__ (
            "addiu $29,-232     \n"
            "sw $17, 224($29)  \n"
            "sw $31, 228($29)    \n"
            "move $5, $4    \n"
            "addiu $4, $29, 32  \n"
            "li $6, 24  \n"
            "jal memcpy \n"
            "lw $7, %0  \n"
            "lw $2, %1  \n"
            "jr $2   \n"
            :
            :"g"(reg_debug_port_base), "g"(spc_NBurstProcess)
    );

}

//=============================================================================
// spc_FcchProcess()
//-----------------------------------------------------------------------------
/// This function processes the FCCH samples for each interrupt for extra windows
//=============================================================================
 PUBLIC VOID  SPAL_FUNC_INTERNAL spc_FcchProcessExt()
{
 u32 *reg_debug_port_base = (u32*)hwp_debugPort;
 void (*spc_FcchProcess)(VOID)  = (VOID (*)(VOID) )0x81e10731;
__asm__ (
            "addiu $29,-232     \n"
            "sw $17, 224($29)  \n"
            "sw $31, 228($29)    \n"
            "lw $7, %0  \n"
            "lw $2, %1  \n"
            "jr $2   \n"
            :
            :"g"(reg_debug_port_base), "g"(spc_FcchProcess)
    );
}

//=============================================================================
// spc_SchProcess()
//-----------------------------------------------------------------------------
/// This function processes the Synchro Burst reception for extra windows
//=============================================================================
 PUBLIC VOID  SPAL_FUNC_INTERNAL spc_SchProcessExt()
{
 u32 *reg_debug_port_base = (u32*)hwp_debugPort;
 void (*spc_SchProcess)(VOID)  = (VOID (*)(VOID) )0x81e107b1;
__asm__ (
            "addiu $29,-232     \n"
            "sw $17, 224($29)  \n"
            "sw $31, 228($29)    \n"
            "lw $7, %0  \n"
            "lw $2, %1  \n"
            "jr $2   \n"
            :
            :"g"(reg_debug_port_base), "g"(spc_SchProcess)
    );
}

//=============================================================================
// spc_RxProcessExt()
//-----------------------------------------------------------------------------
/// This function executes the extended Rx for the current frame 
//=============================================================================
PROTECTED VOID SPAL_FUNC_INTERNAL spc_RxProcessExt(VOID) 
{
    UINT8 current_type=0;
    UINT8 winIdx;

    SPC_RX_LOCALS_T spRx =
    {
        .ItlvBuffInput=NULL,
        .BlockDecodedAdd=g_spcCtx->NBlockDataOut[0],
        .burstIdx=g_mailbox.pal2spc.win[g_spcCtx->currentSnap].rx.nBurst.bstIdx,
        .buffIdx=g_mailbox.pal2spc.win[g_spcCtx->currentSnap].rx.nBurst.bufIdx,
        // duplicate the sched_slot_count for timing reason
        .slotIdx=g_spcCtx->sched_slot_count,
        .RxBuffOffset=0,
        .itlv_mode=ITLV_TYPE_1B,
        .StealingResult=0,
        .Copy_ChMode=g_spcCtx->ChMode,
        .voc_bfi=0,
        .voc_sid=0,
        .voc_taf=0,
        .voc_ufi=0,
        .RxFrameType=SPP_RX_NO_DATA,
        .RX_decode=0    
    };

    SPAL_BUFFER_PROFILE_FUNCTION_ENTER(spc_RxProcessExt);

    //read the rxtype  
    if (g_spcCtx->FcchActive == 0)
    {
        current_type = g_spcCtx->sched_rx_count < g_spcCtx->rxQty? 
                                g_mailbox.pal2spc.win[g_spcCtx->currentSnap].rx.rxTypeWin[g_spcCtx->sched_rx_count]:
                                g_mailbox.extRef->pal2spc.rxwin[g_spcCtx->currentSnap].rxTypeWin[g_spcCtxStructExt.sched_rx_count]; //come from external windows index(beyond 5)

        if (g_spcCtx->FcchSet==1)
        {
            g_spcCtx->FcchActive=1;
        }
        g_spcCtx->FcchSet=0;
        
    }
    
    switch (current_type)
    {
        //--------------------------------------------------------------------            
        //           FREQUENCY BURST                                      
        //--------------------------------------------------------------------            
        case GSM_WIN_TYPE_FCCH:
            if(g_spcCtx->sched_rx_count < g_spcCtx->rxQty){ //except the windows processed in spc_RxProcess
                break;
            }            
            //HAL_W2SET(W2_FCCH);
            SPAL_BUFFER_PROFILE_FUNCTION_ENTER(FCCH);
            spc_FcchProcessExt();
            //HAL_W2CLR(W2_FCCH);
            SPAL_BUFFER_PROFILE_FUNCTION_EXIT(FCCH);
        break;
        
        //--------------------------------------------------------------------            
        //           MONITORING                                          
        //--------------------------------------------------------------------            
        case GSM_WIN_TYPE_MONIT:
            if(g_spcCtx->sched_rx_count < g_spcCtx->rxQty){ //except the windows processed in spc_RxProcess
                break;
            }
        case GSM_WIN_TYPE_MONIT_EXT: //windows type processed in spc_RxProcessExt
            //HAL_W2SET(W2_MONITORING);
            SPAL_BUFFER_PROFILE_FUNCTION_ENTER(MONITORING);

            if(g_spcCtx->sched_rx_count < g_spcCtx->rxQty){
                   winIdx = g_mailbox.pal2spc.win[g_spcCtx->currentSnap].rx.monWinIdx[g_spcCtx->sched_rx_count];
                   spc_MonitoringProcessExt(g_spcCtx->sched_rx_count, winIdx);
                   g_spcCtxStructExt.rx_count_flag = 1;
            }else{
                   winIdx = g_mailbox.extRef->pal2spc.rxwin[g_spcCtx->currentSnap].monWinIdx[g_spcCtxStructExt.sched_rx_count];
                   spc_MonitoringProcessExt(g_spcCtxStructExt.sched_rx_count+MBOX_WINDOWS_QTY,winIdx);
                   g_spcCtxStructExt.sched_rx_count++;
            }
            
           //HAL_W2CLR(W2_MONITORING);
            SPAL_BUFFER_PROFILE_FUNCTION_EXIT(MONITORING);
        break;
  
        //--------------------------------------------------------------------            
        //           SYNCHRO BURST                                      
        //--------------------------------------------------------------------            
        case GSM_WIN_TYPE_SCH:
             if(g_spcCtx->sched_rx_count < g_spcCtx->rxQty){ //except the windows processed in spc_RxProcess
                break;
            }               
            //HAL_W2SET(W2_SCH);
            SPAL_BUFFER_PROFILE_FUNCTION_ENTER(SCH);
            spc_SchProcessExt();
            g_spcCtxStructExt.sched_rx_count++;
            //HAL_W2CLR(W2_SCH);
            SPAL_BUFFER_PROFILE_FUNCTION_EXIT(SCH);
        break;
        
        //--------------------------------------------------------------------            
        //             NORMAL BURST                                         
        //--------------------------------------------------------------------            
        case GSM_WIN_TYPE_NBURST://windows type processed in spc_RxProcessExt
            if(g_spcCtx->sched_rx_count < g_spcCtx->rxQty){ //except the windows processed in spc_RxProcess
                break;
            }
            SPAL_BUFFER_PROFILE_FUNCTION_ENTER(NBURST);
                   
            g_spcCtxStructExt.sched_slot_count++; //increase global context slot counter, which increaseed in RxProcess or in RxProcessExt

           if(g_spcCtxStructExt.sched_slot_count==g_spcCtx->rxslotNb)
           {
                  g_spcCtxStructExt.sched_rx_count++; //increase external rx counter
           }
     
            spc_NBurstProcessExt(&spRx);
            SPAL_BUFFER_PROFILE_FUNCTION_EXIT(NBURST);
        break;

        default:
        break;
    }                         //end swtich type

    
    if (g_spcCtxStructExt.sched_rx_count == g_spcCtxStructExt.rxQty)
    {
        g_spcCtxStructExt.frame_over=1; // end of the current frame process
    } 
    
    SPAL_BUFFER_PROFILE_FUNCTION_EXIT(spc_RxProcessExt);
}

#endif // CHIP_HAS_EXTRA_WINDOWS

