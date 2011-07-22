//------------------------------------------------------------------------------
//              Copyright (C) 2004, Coolsand Technologies, Inc.               
//                            All Rights Reserved                             
//------------------------------------------------------------------------------
/// @file patch_spc_scheduler.c
///
/// This file contains Signal Processing Control scheduler functions
///
/// $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/patch/gallite/src/patch_spc_scheduler.c $ //
/// $Author: kongdq $                                                        // 
/// $Date: 2011-04-12 11:08:18 +0800 (Tue, 12 Apr 2011) $                     //   
/// $Revision: 7033 $                                                        //   
//------------------------------------------------------------------------------

#ifdef ENABLE_PATCH_SPC_SCHEDULERMAIN

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
//#include "spcp_debug.h"
#include "spcp_spp_cfg.h"
#include "spc_profile_codes.h"
// SPAL
#include "spal_comregs.h"
#include "spal_irq.h"
#include "spal_rfif.h"
// CHIP
#include "global_macros.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "bb_irq.h"
#include "bb_ifc.h"
#include "tcu.h"
#include "rf_if.h"

#include "spal_mem.h"
#include "patch_spal_debug.h"
#ifdef CHIP_HAS_EXTRA_WINDOWS
#include "patch_spc_private_ext.h"
#endif
//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------
// interleaver buffer 
extern PROTECTED SPC_ITLV_BUFFERS_T g_spcItlvBufTab;
// Global Context Structure statically placed in BB_SRAM
// Should be referenced by a pointer placed in a reserved register
// to ease address calculation and reduce code size.
extern PUBLIC SPC_CONTEXT_T g_spcCtxStruct;
// static RF samples reception buffers (uncached data section)  
extern PROTECTED SPC_STATIC_BUFFERS_T g_spcStaticBufTab;

extern BOOL SPAL_DATA_INTERNAL g_spcIfc2Done;
// Rx slot number to identify whether multislot window programming is finished.
// TODO:
// Move the variable to SPC context structure and named it as rx_slot_nb,
// and rename the original rxSlotNb as sched_rx_slot_nb.
UINT32 __attribute__((section(".srambss"))) g_spcRxSlotNb;

#ifdef CHIP_HAS_EXTRA_WINDOWS
extern SPC_CONTEXT_EXT_T g_spcCtxStructExt;
#endif

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
PROTECTED VOID my_spc_GnrlReset(VOID);

//=============================================================================
// spc_SchedulerMain()
//-----------------------------------------------------------------------------
/// This function performs the scheduling for Signal Processing Control.
//=============================================================================

PROTECTED VOID SPAL_FUNC_INTERNAL patch_spc_SchedulerMain(VOID)
{
    // Place Global Context address in a reserved register.
    //----------------------------------------------------------
    // CAUTION
    //----------------------------------------------------------
    // This must be done at the entrance (before g_spcCtx is referenced) of
    // each task and each IRQ handler that are using g_spcCtx.
    //----------------------------------------------------------
    g_spcCtx = &g_spcCtxStruct;

    // general reset for context variables & buffers   
    my_spc_GnrlReset();

    // main loop  
    while (1)
    {
#if (SPC_IF_VER != 0)
        //--------------------------
        //    Call back hook
        //--------------------------
        // Call a function defined by the XCPU, when the BCPU
        // doesn't have anything to do. This can be useful to add
        // things to do to the BCPU.
        // We do that only when there is no pending IRQ.
        if (g_spcCtx->schedulerHook != NULL &&
            g_spcCtx->bcpuIrqCause == 0)
        {
            ((VOID(*)(VOID))g_spcCtx->schedulerHook)();
        }
#endif // (SPC_IF_VER != 0)
        
        //--------------------------
        //    check sleep mode   
        //--------------------------
        volatile UINT32 critical_sec;
        critical_sec = spal_IrqEnterCriticalSection();
        if (g_spcCtx->bcpuIrqCause == 0)
        {
            // shutdown if nothing to do  
            SPC_BUFFER_PROFILE_WINDOW_EXIT(BCPU_RUNNING);
            UINT32 clk_bb_disable;
            spal_IrqWriteCpuSleep((UINT32) BB_IRQ_SLEEP);
            // Flush the write buffer
            clk_bb_disable = spal_IrqReadCpuSleep();
            SPC_BUFFER_PROFILE_WINDOW_ENTER(BCPU_RUNNING);
        }
        spal_IrqExitCriticalSection(critical_sec);
        //flush write buffer
        critical_sec=spal_ComregsGetSnap();

        //----------------------------------------------------------------   
        //                          FINT                                                     
        //----------------------------------------------------------------   
        // check each irq cause in g_spcCtx->bcpuIrqCause  
        // IFC2 service must be prioritary on Fint service for late Rx Window treatment  
        if (  (g_spcCtx->bcpuIrqCause & BB_IRQ_BB_IRQ_FRAME) && 
            (!(g_spcCtx->bcpuIrqCause & BB_IRQ_BB_IRQ_IFC2)||((g_spcCtx->frame_over==1)
    #ifdef CHIP_HAS_EXTRA_WINDOWS
            &&((g_spcCtxStructExt.rxQty == 0) || (g_spcCtxStructExt.frame_over==1))
    #endif
            )))
        {

            //HAL_W2SET(W2_FINT_SERV);
            SPC_BUFFER_PROFILE_WINDOW_ENTER(FRAME);

            critical_sec = spal_IrqEnterCriticalSection();
            g_spcCtx->bcpuIrqCause &= ~BB_IRQ_BB_IRQ_FRAME;
            spal_IrqExitCriticalSection(critical_sec);

            if (g_spcCtx->sendEqItr == 1)
            {
                g_spcCtx->sendEqItr = 0;
                spal_ComregsSetIrq(MBOX_DSP_EQUALIZATION_COMPLETE);
            }

            spc_UpdateOnFint();
#ifdef CHIP_HAS_EXTRA_WINDOWS
            spc_UpdateOnFintExt();
#endif
            if (g_spcCtx->txQty > 0)
            {
                spc_TxProcess();
            }

            if (g_spcCtx->rxQty > 0)
            {
                g_spcCtx->rxslotNb = (UINT8)g_spcRxSlotNb;
            }
#if (CHIP_EDGE_SUPPORTED == 1)
            else
            {
                if(g_spcCtx->egprs_rx_buffer_bmp)
                {
                    UINT8 i;
                    UINT8 j;
                    
                    // if we decode the third or the fourth block we need to recopy
                    // the fourth burst softbits previously saved at N+1
                    // if we execute those lines we are at N+2
                    for(i=0;i<2;i++)
                    {
                        UINT32* tmp1=&(g_Egprs_Softbit_Buff[87*i]);
                        UINT32* tmp2=g_spcCtx->ItlvBufRxCtrl+(i+2)*SPC_ITLV_RX_BUFF_OFFSET;
                        for(j=0;j<87;j++)
                        {
                            tmp2[j]=tmp1[j];
                        }    
                    }
                    for(i=0;i<2;i++)
                    {
                        for (j=0;j<g_spcCtx->rxslotNb;j++) 
                        {
                            if ((g_spcCtx->egprs_rx_buffer_bmp)&(0x1<<j))
                            {
                               break;
                            }    
                        }
                        spc_EgprsBlockDecode(j,i);
                        // reset the decoded buffer in the bitmap 
                        g_spcCtx->egprs_rx_buffer_bmp &= ~(0x1<<j);
                        // send an it to the XCPU for the end of Block Decoding if the last
                        // buffer has been decoded
                        if (g_spcCtx->egprs_rx_buffer_bmp==0)
                        {
#ifdef __USE_NB_DECODE_IRQ__
                            if(g_spcCtx->statWinRxCfg[j].itr &
                               MBOX_DSP_NB_DECODING_COMPLETE)
                            {
                                spal_ComregsSetIrq(MBOX_DSP_NB_DECODING_COMPLETE);
                            }
#endif
                            break;
                        }
                    }
                 } 
            }
#endif
            //HAL_W2CLR(W2_FINT_SERV);
            SPC_BUFFER_PROFILE_WINDOW_EXIT(FRAME);

        }//endif FINT 


        //----------------------------------------------------------------   
        //        IFC2 RF RX                                               
        //----------------------------------------------------------------   
        // check each irq cause in g_spcCtx->bcpuIrqCause  
        if (g_spcCtx->bcpuIrqCause & BB_IRQ_BB_IRQ_IFC2)
        {

            //HAL_W2SET(W2_IFC2_SERV);
            SPC_BUFFER_PROFILE_WINDOW_ENTER(RF_IFC);

            critical_sec = spal_IrqEnterCriticalSection();
            // AVOID missing an IFC2 IT if 2 (or more) of them happened
            // during last IFC2 servicing
            if(!(-- g_spcCtx->ifc2_pending))
                g_spcCtx->bcpuIrqCause &= ~BB_IRQ_BB_IRQ_IFC2;
            spal_IrqExitCriticalSection(critical_sec);
            
#ifdef CHIP_HAS_EXTRA_WINDOWS
            if(g_spcCtxStructExt.sched_rx_count < g_spcCtxStructExt.rxQty){
                spc_RxProcessExt();
            }

            if ((g_spcCtx->sched_rx_count < g_spcCtx->rxQty)  || ((g_spcCtx->rxQty==0) && (g_spcCtx->FcchActive)) )
             {
                spc_RxProcess();  
                              
                if(g_spcCtxStructExt.rx_count_flag == 1){
                    g_spcCtx->sched_rx_count++;
                    g_spcCtxStructExt.rx_count_flag = 0;
                }

            }
#else            
           spc_RxProcess();  
#endif 

            SPC_BUFFER_PROFILE_WINDOW_EXIT(RF_IFC);
            //HAL_W2CLR(W2_IFC2_SERV);
        } //end if IFC 
    } //end while(1)  

} //end main  



//=============================================================================
// spc_GnrlReset()
//-----------------------------------------------------------------------------
/// This function executes a general reset of the context
//=============================================================================
// As spc_GnrlReset() is run only once at BCPU startup time,
// we don't need to put it in internal SRAM
PROTECTED VOID my_spc_GnrlReset(VOID)
{
    UINT8 i,j;

    g_spcCtx->bcpuIrqCause=0;

    g_spcCtx->BlockSnR = 0;
    g_spcCtx->LastBlockSnR = 0;
    g_spcCtx->Rx_Hu_TCH_N_1 = 0;
    g_spcCtx->Rx_Hu_TCH_N = 0;
    g_spcCtx->Rx_Hl_TCH_N = 0;
    g_spcCtx->ChMode = MBOX_NO_TCH;
    g_spcCtx->PrevChMode = MBOX_NO_TCH;
    g_spcCtx->DedicatedActive = FALSE;
    g_spcCtx->Tx_off = FALSE;
    g_spcCtx->Init_FCCH = FALSE;
    for (i=0;i<4;i++) g_spcCtx->Rx_CS[i]=0;
    for (i=0;i<4;i++){
        for (j=0;j<8;j++) g_spcCtx->Rx_q_CCH[i][j] = 0;
    }
    g_spcCtx->currentSnap = 0;
    g_spcCtx->ifc2_pending = 0;
    g_spcCtx->sched_slot_count = 0;
    g_spcCtx->TxBlockOffset = 0;
    g_spcCtx->RxBlockOffset = 0;
    g_spcCtx->Tx_Hu = 0;
    g_spcCtx->Tx_Hl = 0;
    g_spcCtx->FacchEncoded = 0;
    g_spcCtx->DTX_on = 0;

    // RF-related setup
    if (g_mailbox.pal2spc.rf.dcoCalEnable)
        g_spcCtx->RfIfCtrl =    RF_IF_ENABLE_ENABLE | RF_IF_RX_CAL_BYPASS_ENABLED;
    else
        g_spcCtx->RfIfCtrl =    RF_IF_ENABLE_ENABLE | RF_IF_RX_CAL_BYPASS_BYPASSED;

#if (CHIP_HAS_ANA_RF)
    // Add ramping event to DTX TCO settings
    g_mailbox.pal2spc.rf.dtxTcoSettings |= TCU_DISABLE_RAMP_MASK;

    // Setup Rf_if in case of DigRf
    if (g_mailbox.pal2spc.rf.digrfEnable)
    {
        g_spcCtx->RfIfCtrl |=  RF_IF_DIGRF_ENABLE_ENABLE |
            (g_mailbox.pal2spc.rf.digrfRxRate?RF_IF_DIGRF_RX_RATE_TWO:RF_IF_DIGRF_RX_RATE_ONE) |
            (g_mailbox.pal2spc.rf.digrfRxClkPol?RF_IF_DIGRF_RX_CLK_POL:0) |
            (g_mailbox.pal2spc.rf.digrfTxMode?RF_IF_DIGRF_TX_MODE_BLOCK:RF_IF_DIGRF_TX_MODE_STREAM) |
            (g_mailbox.pal2spc.rf.digrfTxClkPol?RF_IF_DIGRF_TX_CLK_POL:0) |
#if (SPC_IF_VER == 1)
            RF_IF_DIGRF_SHIFT_SAMPLE(g_mailbox.pal2spc.rf.digrfRxSampleShift);
#else
            RF_IF_DIGRF_SHIFT_SAMPLE(g_mailbox.pal2spc.rf.digrfRxSampleWidth);
#endif

    }
#else // (!CHIP_HAS_ANA_RF) : DigRf only and modified RF_IF
    UINT8 sample_size = g_mailbox.pal2spc.rf.digrfRxSampleWidth;
    sample_size = (sample_size>16)?0:
                  (sample_size<12)?4:
                  (16-sample_size);
    g_spcCtx->RfIfCtrl |=  
            (g_mailbox.pal2spc.rf.digrfRxRate?RF_IF_DIGRF_RX_RATE_TWO:RF_IF_DIGRF_RX_RATE_ONE) |
            (g_mailbox.pal2spc.rf.digrfRxClkPol?RF_IF_DIGRF_RX_CLK_POL:0) |
            (g_mailbox.pal2spc.rf.digrfTxMode?RF_IF_DIGRF_TX_MODE_BLOCK:RF_IF_DIGRF_TX_MODE_STREAM) |
            (g_mailbox.pal2spc.rf.digrfTxClkPol?RF_IF_DIGRF_TX_CLK_POL:0) |
            RF_IF_DIGRF_SAMPLE_SIZE(sample_size) |
            ((g_mailbox.pal2spc.rf.digrfRxSampleAlign==SPC_DIGRF_SAMPLE_ALIGN_MSB)?
              0:RF_IF_DIGRF_ALIGNEMENT_SELECT);

#endif
    
    spal_RfIfWriteCtrl(g_spcCtx->RfIfCtrl);

    g_spcCtx->FcchFound = 0;
    g_spcCtx->FcchSet = 0;
    g_spcCtx->txQty = 0;
    g_spcCtx->rxQty = 0;
    g_spcCtx->Taf_Flag = 0;
    g_spcCtx->DTX_dwnlk_flag=0;
    g_spcCtx->DTX_dwnlk_count=0;
    g_spcCtx->sendEqItr = 0;
    g_spcCtx->resIdx = 0;
    for (i=0;i<4;i++) g_spcCtx->cs_identifier[i]=CS1_ID;
    g_spcCtx->frame_over=0;
    g_spcCtx->loop_bufIdx=0;
    g_spcCtx->CodecModeRequest=0;
    g_spcCtx->ratscch_to_encode=0;
    g_spcCtx->ACK_Activation=0;
    g_spcCtx->REQ_Activation=0;
    g_spcCtx->RATSCCH_REQ=NO_REQ;
    g_spcCtx->ratscch_detected=0;
    g_spcCtx->sidupdate_ctr=0;
    g_spcCtx->AFNmod104=0;
#if (CHIP_EDGE_SUPPORTED == 1)
    g_spcCtx->egprs_rx_buffer_bmp=0;
    for (i=0;i<4;i++) g_spcCtx->prev_rx_headertype[i]=0;
    for (i=0;i<4;i++) g_spcCtx->curr_rx_headertype[i]=0;
    g_spcCtx->Header_result.mcs=0;
    g_spcCtx->Header_result.cps[0]=0;
    g_spcCtx->Header_result.cps[1]=0;
#endif
    g_spcCtx->FCCH_Result.FOf = 0;
    g_spcCtx->FCCH_Result.TOf = 0;
    g_spcCtx->SCH_Result.FOf = 0;
    g_spcCtx->SCH_Result.TOf = 0;
    g_spcCtx->SCH_Result.Snr = 0;
    g_spcCtx->SCH_Result.Data[0] = 0;
    g_spcCtx->SCH_Result.Data[1] = 0;
    g_spcCtx->SCH_Result.Data[2] = 0;
    g_spcCtx->SCH_Result.Data[3] = 0;

    // interleaver input buffer
    g_spcCtx->ItlvBufRxCtrl = ((UINT32 *) KSEG1((UINT32)&(g_spcItlvBufTab.RxCtrl)));
    g_spcCtx->ItlvBufRxDedicated = ((UINT32 *) KSEG1((UINT32)&(g_spcItlvBufTab.RxDedicated)));
    g_spcCtx->ItlvBufTxCtrl = ((UINT32 *) KSEG1((UINT32)&(g_spcItlvBufTab.TxCtrl)));
    g_spcCtx->ItlvBufTxDedicated = ((UINT32 *) KSEG1((UINT32)&(g_spcItlvBufTab.TxDedicated)));
    g_spcCtx->ItlvBufTxFacch = ((UINT32 *) KSEG1((UINT32)&(g_spcItlvBufTab.TxFacch)));

    // interleaver output buffer
    g_spcCtx->Malloc = (UINT32*) &(g_spcStaticBufTab) + SPC_STATIC_BUF_SIZE;
    // next address must be aligned on 128 bytes
    g_spcCtx->ItlvBufRxFacch = (UINT32*) KSEG1((UINT32)(&(g_spcStaticBufTab.RxIfcBuffer[2])+0x40)&0xFFFFFFC0);
    g_spcCtx->loopC_buffer = ((UINT8 *) g_spcCtx->ItlvBufRxCtrl);
    // the last 100 estimated C/I in AMR are stored here
    g_spcCtx->CIest=(INT8 *) (g_spcCtx->ItlvBufTxFacch);

    // Tx buffer addresses initialisation in g_mailbox
    for (i=0; i < MBOX_MAX_BUF_IDX_QTY-1; i++) { 
        g_spcCtx->NBlockDataIn[i]=(UINT32*) KSEG1((UINT32) g_spcCtx->DCH_NBlockDataIn[i]);
    }
    g_spcCtx->NBlockDataIn[MBOX_MAX_BUF_IDX_QTY-1] = (UINT32*) KSEG1((UINT32) g_spcCtx->CCH_NBlockDataIn);
    g_spcCtx->NBlockDataIn[MBOX_MAX_BUF_IDX_QTY] = (UINT32*) KSEG1((UINT32) g_spcCtx->CSD_NBlockDataIn);

    for (i=0; i < MBOX_MAX_BUF_IDX_QTY+1; i++) {
        g_mailbox.spc2pal.encBuff[i] = (UINT8 *)g_spcCtx->NBlockDataIn[i];
    }
    
    // Rx buffer addresses initialisation in g_mailbox
    for (i=0; i < MBOX_MAX_BUF_IDX_QTY-1; i++) { // for PDCH block to encode
        g_spcCtx->NBlockDataOut[i] = (UINT32*) KSEG1((UINT32) g_spcCtx->DCH_NBlockDataOut[i]);
    }
    g_spcCtx->NBlockDataOut[MBOX_MAX_BUF_IDX_QTY-1] = (UINT32*) KSEG1((UINT32) g_spcCtx->CCH_NBlockDataOut);
    g_spcCtx->NBlockDataOut[MBOX_MAX_BUF_IDX_QTY] = (UINT32*) KSEG1((UINT32) g_spcCtx->CSD_NBlockDataOut);

    g_spcRxSlotNb = 0;
    
    spp_ResetFullBurstReady();
}

#endif // ENABLE_PATCH_SPC_SCHEDULERMAIN
