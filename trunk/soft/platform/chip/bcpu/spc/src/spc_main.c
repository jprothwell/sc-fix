/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2003, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      spc_main.c                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      This file is the entry point for BCPU.                           */
/*                                                                       */
/*************************************************************************/

#include "cs_types.h"

#include "chip_id.h"

#include "spc_mailbox.h"
#include "spc_ctx.h"
#include "spc_map.h"
#include "spcp_spp_cfg.h"
#include "spcp_amr.h"
#include "spcp_debug.h"
#include "spc_profile_codes.h"
#include "spc_private.h"
#include "spp_gsm.h"
#include "spal_irq.h"
#include "spal_cfg.h"
#include "spal_comregs.h"
#include "spal_debug.h"
#include "spal_mem.h"
#include "spal_rfif.h" // FCCH RF-IF FIFO FIX

// sp context structure
EXPORT SPC_CONTEXT_T g_spcCtxStruct;
#if (!CHIP_BYPASS_ROM)
EXPORT volatile UINT32 g_bcpuIrqCause;
#endif

#if (CHIP_EDGE_SUPPORTED==1)
// Incremental redundancy buffer for EGPRS
PROTECTED SPP_EGPRS_IR_BUF_T g_Egprs_Ir_Buffer __attribute__((section("BB_SPC_IR_SECTION")));
#endif

#ifdef CHIP_HAS_EXTRA_WINDOWS
SPC_MBX_EXT_T g_ext_ref SPAL_UNCACHED_DATA_INTERNAL;
#endif

extern VOID spc_IrqSetMaskTcu1(VOID);
// =============================================================================
// g_spcMapAccess
// -----------------------------------------------------------------------------
/// Contains the addresses of the structures of SPC that will be accessible from
/// a remote PC (e.g. through CoolWatcher).
// =============================================================================
PROTECTED CONST SPC_MAP_ACCESS_T g_spcMapAccess
    = {(SPC_MAILBOX_T*)&g_mailbox, &g_spcCtxStruct};

#if 0
//  HOOK Testing functions
//
#include "global_macros.h"
#include "bb_irq.h"
#include "comregs.h"
#include "spal_debug.h"

PRIVATE UINT32 g_spcHookCtr = 0;

PROTECTED VOID spc_SchedHook(VOID)
{
    hwp_bbIrq->Mask_Set = BB_IRQ_BB_IRQ_COM0;
    hwp_bbComregs->Mask_Set = 0xff00;
    if(g_spcHookCtr++%100 == 3)
    {
        spal_DbgHstSendEvent(g_spcHookCtr);
    }
}

PROTECTED VOID spc_IrqHook(VOID)
{
    hwp_bbComregs->ItReg_Clr = 0xff00;

    spal_DbgHstSendEvent(0x66600666);
}

#endif


/////////////////////////////////////////////////////////////////
// Add SPC Scheduler hook to set one BfiThresold for each vocoder
//  for Greenstone only
/////////////////////////////////////////////////////////////////
#if (SPC_IF_VER == 1)
PROTECTED VOID SPAL_FUNC_INTERNAL spc_SchedHook(VOID)
{
    SPC_PROFILE_WINDOW_ENTER(SPC_SCHED_HOOK);
    
    if ((SPEECH(g_spcCtxStruct.ChMode)==EFR_CODEC)){
        g_spcCtxStruct.BfiThreshold = 70;//EFR with a strong crc check 
    }
    else if (RATE(g_spcCtxStruct.ChMode)==HR_RATE){  
        g_spcCtxStruct.BfiThreshold = 40;//HR keep the same threshold as before
    }
    ////////////////////////////////////////////////////////
    // check the dummy PCH burst detection is enabled
    if(g_spcCtxStruct.sched_rx_count==0)
    {
        g_mailbox.dummyPchResult=SPP_DUMMY_PCH_NO_RESULT;
    }
    else if((g_spcCtx->sendEqItr==1)&&(g_mailbox.dummyPchResult==SPP_DUMMY_PCH_NO_RESULT))
    {
        UINT8 buffIdx=g_mailbox.pal2spc.win[g_spcCtxStruct.currentSnap].rx.nBurst.bufIdx;
        UINT8 burstIdx=g_mailbox.pal2spc.win[g_spcCtxStruct.currentSnap].rx.nBurst.bstIdx;
        UINT32 *EqualizerOutput=(UINT32*)(g_spcCtxStruct.ItlvBufRxCtrl+
                                          buffIdx*SPC_ITLV_RX_BUFF_OFFSET+
                                          burstIdx * 32);

        if((g_spcCtxStruct.statWinRxCfg[buffIdx].itr&MBOX_DSP_PCH_READY))
        {
            UINT8 result;
            // check if it is the last burst
            if (burstIdx==3)
            {
                result = SPP_PCH_DECODING_COMPLETE;
            }
            else
            {
                result = spp_DetectDummyPCH(EqualizerOutput,
                                            burstIdx,
                                            ((g_mailbox.dummyPchThresh)&0xFF00)>>8,
                                            (g_mailbox.dummyPchThresh)&0xFF);
            } 
            g_mailbox.dummyPchResult = (UINT32) result;
            spal_ComregsSetIrq(MBOX_DSP_PCH_READY);
        }
    }
    ////////////////////////////////////////////////////////
    // send IRQ COM0 to XCPU at the end of the BCPU processing
    if ((g_spcCtxStruct.frame_over==1)&&(g_spcCtxStruct.FcchActive == 0))
    { // end of the current frame process
        spal_ComregsSetIrq(MBOX_DSP_TASK_COMPLETE);
    }

    SPC_PROFILE_WINDOW_EXIT(SPC_SCHED_HOOK);
}
#endif
/// **********************************************
/// BCPU entry point
/// This function should NOT be ROM'ed
/// **********************************************/
VOID bcpu_main(VOID)
{ 
    SPP_GLOBAL_PARAMS_T spcSppCfg;
    SPAL_GLOBAL_PARAMS_T spcSpalCfg;

    // Check that Xcpu did initiate the handshake
    if (g_mailbox.spc2pal.debug[0] == MBOX_XCPU_HELLO_WORD)
    {

        // Init AMR state
        g_spcAmrState.dtxUsed = 0;
        g_spcAmrState.prevId = 0;
        g_spcAmrState.rescheduledSidFirst = 0;
        g_spcAmrState.rescheduledSidUpdate = 0;
        g_spcAmrState.previousFrameType = SPP_TX_SPEECH;

        // Acknowledge the handshake
        g_mailbox.spc2pal.debug[0] = MBOX_BCPU_ACKNOWLEDGE_WORD;

        ///////////////////////////////////////////////////////////////////////
        //                  Setup SPC switches and parameters                //
        ///////////////////////////////////////////////////////////////////////
#if (SPC_IF_VER != 0)
#if (SPC_IF_VER == 1) // keep scheduler hook for greenstone
        g_spcCtxStruct.schedulerHook = (VOID*)spc_SchedHook;// NULL;
#else
        g_spcCtxStruct.schedulerHook = NULL;
#endif // (SPC_IF_VER == 1)
        g_spcCtxStruct.irqHook = /*(VOID*)spc_IrqHook;*/ NULL;
#endif // (SPC_IF_VER != 0)
        g_spcCtxStruct.FacchThreshold = 0xFFF8;// -8;
#if (SPC_IF_VER < 2)        
        g_spcCtxStruct.loop_bufIdx = 0;
        g_spcCtxStruct.BfiThreshold = 80;//50;
#else
        g_spcCtxStruct.dumpPtr = NULL;//(VOID*)spal_DbgPxtsDump;
        g_spcCtxStruct.dumpMask = 0;
        g_spcCtxStruct.ifc2_burst_mode_fcch = FALSE;
        g_spcCtxStruct.ifc2_burst_mode_sbnb = TRUE;
        g_spcCtxStruct.FR_BfiThreshold = 120;//50;
        g_spcCtxStruct.EFR_BfiThreshold = 120;//70;
        g_spcCtxStruct.HR_BfiThreshold = 60;//40;
        g_spcCtxStruct.FcchSet =  0; 
        g_spcCtxStruct.FcchWinSize =  BB_FCCH_SIZE; 
        g_spcCtxStruct.FcchPreOpening = BB_FCCH_PRE_OPENING;
        g_spcCtxStruct.MonWinSize =  MBOX_MON_WIN_SIZE; 
#endif // (SPC_IF_VER < 2)
        g_spcCtxStruct.UfiThreshold = 30;
        g_spcCtxStruct.FsBfiBerThresh = 120;//60;
        g_spcCtxStruct.FsBfiBer2Thresh = 120;//50;
        g_spcCtxStruct.FsBfiSnrThresh = -12;
        g_spcCtxStruct.HsBfiBerThresh = 60;//45;
        g_spcCtxStruct.HsBfiBer2Thresh = 50;//20;
        g_spcCtxStruct.HsBfiSnrThresh = 0;
        
        g_spcCtxStruct.AFS_ber1_threshold[0]=35;
        g_spcCtxStruct.AFS_ber1_threshold[1]=35;
        g_spcCtxStruct.AFS_ber1_threshold[2]=35;
        g_spcCtxStruct.AFS_ber1_threshold[3]=35;
        g_spcCtxStruct.AFS_ber1_threshold[4]=35;
        g_spcCtxStruct.AFS_ber1_threshold[5]=35;
        g_spcCtxStruct.AFS_ber1_threshold[6]=35;
        g_spcCtxStruct.AFS_ber1_threshold[7]=15;
        g_spcCtxStruct.AHS_ber1_threshold[0]=5;
        g_spcCtxStruct.AHS_ber1_threshold[1]=5;
        g_spcCtxStruct.AHS_ber1_threshold[2]=5;
        g_spcCtxStruct.AHS_ber1_threshold[3]=4;
        g_spcCtxStruct.AHS_ber1_threshold[4]=3;
        g_spcCtxStruct.AHS_ber1_threshold[5]=2;
        g_spcCtxStruct.AFS_ber2_threshold[0]=100;
        g_spcCtxStruct.AFS_ber2_threshold[1]=95;
        g_spcCtxStruct.AFS_ber2_threshold[2]=95;
        g_spcCtxStruct.AFS_ber2_threshold[3]=95;
        g_spcCtxStruct.AFS_ber2_threshold[4]=95;
        g_spcCtxStruct.AFS_ber2_threshold[5]=85;
        g_spcCtxStruct.AFS_ber2_threshold[6]=70;
        g_spcCtxStruct.AFS_ber2_threshold[7]=50;
        g_spcCtxStruct.AHS_ber2_threshold[0]=30;
        g_spcCtxStruct.AHS_ber2_threshold[1]=30;
        g_spcCtxStruct.AHS_ber2_threshold[2]=25;
        g_spcCtxStruct.AHS_ber2_threshold[3]=20;
        g_spcCtxStruct.AHS_ber2_threshold[4]=15;
        g_spcCtxStruct.AHS_ber2_threshold[5]=15;

#if (CHIP_EDGE_SUPPORTED==1)
        g_spcCtxStruct.egprsIrBuffer = &g_Egprs_Ir_Buffer;
#endif

        ///////////////////////////////////////////////////////////////////////
        //                  Setup SPP switches and parameters                //
        ///////////////////////////////////////////////////////////////////////
#if ((SPC_IF_VER == 0)||(SPC_IF_VER == 1)||(SPC_IF_VER == 2)||(SPC_IF_VER == 0xFF))
        spcSppCfg.bypassDcoc = (g_mailbox.pal2spc.rf.dcoCalEnable & SPC_SW_DCOC_ENABLED)?FALSE:
                               TRUE;
        // Cleanup the field to match rom'ed implementation
        g_mailbox.pal2spc.rf.dcoCalEnable &= SPC_HW_DCOC_ENABLED;
#else
        // TODO update interface
        spcSppCfg.bypassDcoc = TRUE;
#endif
        spcSppCfg.correctCS1 = FALSE;
        spcSppCfg.correctCS4 = FALSE;
        spcSppCfg.equ_hburst_mode = 
            g_spcCtxStruct.equ_hburst_mode = FALSE;
        spcSppCfg.symb_max = _SYMB_MAX;
        spcSppCfg.symb_min = _SYMB_MIN;
        spcSppCfg.fof_nb_symb = _FOF_NB_SYMB;
        spcSppCfg.fof_start_1 = _FOF_START_1;
        spcSppCfg.fof_start_2 = _FOF_START_2;
        spcSppCfg.fof_corr_gap = _FOF_CORR_GAP;
        spcSppCfg.agc_threshold = _AGC_THRESHOLD;
        spcSppCfg.mon_win_size = MBOX_MON_WIN_SIZE;

        // TODO: The shifts might be different depending on the xcv and the interface RF.
        // We might need in the future to pass these parameters from the xcv driver.
        spcSppCfg.shift_bm_offset= 0;
        spcSppCfg.shift_sb_offset= 1;
        
#if (SPC_IF_VER >= 2)
        spcSppCfg.fcch_size =  g_spcCtxStruct.FcchWinSize; 
        spcSppCfg.fcch_detect_func =  spp_FcchDetectAsm_gallite; 
        spcSppCfg.fcch_sliding_energy = TRUE; 
        spcSppCfg.fcch_hw_sliding_window = TRUE;
#endif

        // Open SPP (configure SPP)
        spp_Open(&spcSppCfg);
        
        ///////////////////////////////////////////////////////////////////////
        //                  Setup SPAL switches and parameters               //
        ///////////////////////////////////////////////////////////////////////
#if (CHIP_BYPASS_ROM == 1)

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
        spcSpalCfg.irqHandler = spc_IrqHandlerTcu1;
        spcSpalCfg.irqInRom = FALSE;
        spal_Open(&spcSpalCfg);
        spc_IrqSetMaskTcu1();
#else // CHIP_ASIC_ID
#if(CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
        spcSpalCfg.irqHandler = spc_IrqHandler;
        spcSpalCfg.irqInRom = FALSE;
        spal_Open(&spcSpalCfg);
        spc_IrqSetMaskTcu1();
#else // CHIP_ASIC_ID
        spcSpalCfg.irqHandler = spc_IrqHandler;
        spcSpalCfg.irqInRom = FALSE;
        spal_Open(&spcSpalCfg);
#endif
#endif // CHIP_ASIC_ID

#else // CHIP_BYPASS_ROM

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
        spcSpalCfg.irqHandler = spc_IrqHandlerTcu1;
        spcSpalCfg.irqInRom = TRUE;
        spal_Open(&spcSpalCfg);
        spc_IrqSetMaskTcu1();
#else // CHIP_ASIC_ID
#if(CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
        spcSpalCfg.irqHandler = spc_IrqHandler;
        spcSpalCfg.irqInRom = TRUE;
        spal_Open(&spcSpalCfg);
        spc_IrqSetMaskTcu1();		
#else // CHIP_ASIC_ID
        spcSpalCfg.irqHandler = spc_IrqHandler;
        spcSpalCfg.irqInRom = TRUE;
        spal_Open(&spcSpalCfg);
#endif
#endif // CHIP_ASIC_ID

#endif //CHIP_BYPASS_ROM

#ifdef CHIP_HAS_EXTRA_WINDOWS
        g_mailbox.extRef = &g_ext_ref;
#endif
        ///////////////////////////////////////////////////////////////////////
        //                 Start the main while loop                         //
        ///////////////////////////////////////////////////////////////////////
        spc_SchedulerMain();

    }
    else
    {
        g_mailbox.spc2pal.debug[0] = MBOX_BCPU_ERROR_WORD;
    }

    while (1);
}

