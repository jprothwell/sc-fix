/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2004, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      patch_spc_amr_rx.c                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      replace the function spc_AFSdecoding & spc_AHSdecoding           */
/*      from spc_amr_rx.c using the hardware patch mecanism              */
/*                                                                       */
/*************************************************************************/

#include "spc_ctx.h"
#include "spc_private.h"
#include "spcp_debug.h"
#include "spcp_amr.h"
#include "spc_profile_codes.h"
#include "spal_mem.h"

#ifdef ENABLE_PATCH_SPC_AFSDECODING

// ****************************************************************************
//  spc_AFSdecoding:
//
//      this function decodes speech or DTX control AFS frames and inband data
//      TxCodecMode or/and RxCodecMode and RxFrameType are updated
//                          
//
// ****************************************************************************
VOID SPAL_FUNC_INTERNAL patch_spc_AFSdecoding(SPP_AMR_CFG_T AMR_Cfg,
                     UINT32 *ItlvBuffInput,
                     UINT32 *RxItlvBufferOut,
                     UINT8 RxBuffOffset, 
                     UINT32 *BlockDecodedAdd,
                     UINT8 T2,
                     SPP_N_BLOCK_RESULT_T *NBlock_result,
                     UINT8* TxCodecMode,
                     UINT8* RxCodecMode,
                     UINT8 *RxFrameType,
                     UINT8 *ratscch_detected)
{

    UINT8 FrameType=0;
    UINT8 mode=0;
    UINT8 acs=0;
    UINT8 id1=0;
    UINT8 id2=0;
    UINT8 cm1=*RxCodecMode;
    UINT8 cm2=*TxCodecMode;
    

    spp_DeinterleaveBlock(ItlvBuffInput, ITLV_TYPE_1A, RxBuffOffset, RxItlvBufferOut);

    FrameType=spp_SearchAmrMarker(RxItlvBufferOut, &id1, &id2, 0);
    // if id1 is not enough reliable we take the previous value     
    if (id1==0xFF){
        if ((T2==3)||(T2==11)||(T2==20)){
            if (AMR_Cfg.phase==ODD_PHASE) cm1=*RxCodecMode; // odd phase CMI received
            else                          cm1=*TxCodecMode; //even phase CMC received   
        }
        else{
            if (AMR_Cfg.phase==ODD_PHASE) cm1=*TxCodecMode; // odd phase CMC received
            else                          cm1=*RxCodecMode; //even phase CMI received               
        }
    }
    else{   
        if (id1>=AMR_Cfg.nbActiveCodecSet){
            id1=AMR_Cfg.nbActiveCodecSet-1;
        }
        cm1=AMR_Cfg.activeCodecSet[id1];
    }

    switch (FrameType){
            
        case SPP_AFS_SPEECH:
            if ((T2==3)||(T2==11)||(T2==20)){
                mode=(AMR_Cfg.phase==ODD_PHASE)?cm1:*RxCodecMode; //odd phase CMI received even phase CMC received
            }
            else{
                mode=(AMR_Cfg.phase==ODD_PHASE)?*RxCodecMode:cm1; //odd phase CMC received even phase CMI received
            }
            if (g_spcAmrState.dtxUsed) mode=*RxCodecMode; //to avoid decoding with a wrong mode during dtx downlink
            spp_DecodeBlock(RxItlvBufferOut,
                            MBOX_TCH_AFS,
                            FrameType,
                            mode,
                            BlockDecodedAdd,
                            0,
                            (SPP_UNCACHED_BUFFERS_T*) g_spcCtx->Malloc,
                            NBlock_result);

            if(NBlock_result->Bfi==0){
                *RxFrameType=SPP_RX_SPEECH_GOOD;
                if ((T2==3)||(T2==11)||(T2==20)){
                    if (AMR_Cfg.phase==ODD_PHASE) *RxCodecMode=cm1; // odd phase CMI received
                    else                          *TxCodecMode=cm1; //even phase CMC received   
                }
                else{
                    if (AMR_Cfg.phase==ODD_PHASE) *TxCodecMode=cm1; // odd phase CMC received
                    else                          *RxCodecMode=cm1; //even phase CMI received               
                }
                if (NBlock_result->BitError > g_spcCtx->AFS_ber1_threshold[*RxCodecMode])
                {
                    *RxFrameType=SPP_RX_SPEECH_DEGRADED;
                } 
                g_spcAmrState.dtxUsed=0;
            }
            else{
                if (NBlock_result->BitError > g_spcCtx->AFS_ber2_threshold[*RxCodecMode])
                {
                    *RxFrameType=SPP_RX_NO_DATA;
                }
                else {*RxFrameType=SPP_RX_SPEECH_BAD;g_spcAmrState.dtxUsed=0;}
            }
            break;

        case SPP_AFS_SID_UPDATE:
            {
            UINT32 tmp_word[4];
            UINT8 i;   
            g_spcAmrState.dtxUsed=1;
            spp_DeinterleaveBlock(ItlvBuffInput+(RxBuffOffset^4)*32, ITLV_TYPE_1B, 0, RxItlvBufferOut);
            for (i=0;i<4;i++) tmp_word[i]=RxItlvBufferOut[2*i];
            id2=spp_GetAmrIc((UINT8*)tmp_word, 2);   
            if (id2<AMR_Cfg.nbActiveCodecSet) cm2=AMR_Cfg.activeCodecSet[id2];
            spp_DecodeBlock(RxItlvBufferOut,
                            MBOX_TCH_AFS,
                            FrameType,
                            cm1,
                            BlockDecodedAdd,
                            0,
                            (SPP_UNCACHED_BUFFERS_T*) g_spcCtx->Malloc,
                            NBlock_result);
            if(NBlock_result->Bfi==0){
                *RxFrameType=SPP_RX_SID_UPDATE;
                *TxCodecMode=cm2;
                *RxCodecMode=cm1;
            }
            else {
                if((BlockDecodedAdd[0]==0xFFFFFFFF)&&((BlockDecodedAdd[1]&0x7)==0x7))
                {
                    // SID FILLER in TFO
                    *RxFrameType=SPP_RX_NO_DATA;
                }
                else
                {
                    *RxFrameType=SPP_RX_SID_BAD;
                }
            }
            }
            break;

        case SPP_AFS_SID_FIRST:
            g_spcAmrState.dtxUsed=1;
            *RxFrameType=SPP_RX_SID_FIRST;
             if ((T2==3)||(T2==11)||(T2==20)){
                if (AMR_Cfg.phase==ODD_PHASE) *RxCodecMode=cm1; // odd phase CMI received
                else                          *TxCodecMode=cm1; //even phase CMC received   
            }
            else{
                if (AMR_Cfg.phase==ODD_PHASE) *TxCodecMode=cm1; // odd phase CMC received
                else                          *RxCodecMode=cm1; //even phase CMI received               
            }
            break;

        case SPP_AFS_ONSET:
            g_spcAmrState.dtxUsed=0;
            *RxFrameType=SPP_RX_ONSET;
            *RxCodecMode=cm1; // CMI received
            break;
        
        case SPP_AFS_RATSCCH:   
            g_spcAmrState.dtxUsed=0;
            *RxFrameType=SPP_RX_NO_DATA;
            if (id2<AMR_Cfg.nbActiveCodecSet) cm2=AMR_Cfg.activeCodecSet[id2];
            spp_DecodeBlock(RxItlvBufferOut,
                            MBOX_TCH_AFS,
                            FrameType,
                            cm1,
                            BlockDecodedAdd,
                            0,
                            (SPP_UNCACHED_BUFFERS_T*) g_spcCtx->Malloc,
                            NBlock_result);
            if(NBlock_result->Bfi==0){
                *TxCodecMode=cm2;
                *RxCodecMode=cm1;
            }
            *ratscch_detected=1;
            break;
    }

    id1=0;
    id2=0;
    for(id1=0;id1<8;id1++){
        if (id1==AMR_Cfg.activeCodecSet[id2]) {id2++;acs|=1<<id1;} 
    }

}

#endif // ENABLE_PATCH_SPC_AFSDECODING


#ifdef ENABLE_PATCH_SPC_AHSDECODING

// ****************************************************************************
//  spc_AHSdecoding:
//
//      this function decodes speech or DTX control AHS frames and inband data
//      TxCodecMode or/and RxCodecMode and RxFrameType are updated
//                          
//
// ****************************************************************************
VOID SPAL_FUNC_INTERNAL patch_spc_AHSdecoding(SPP_AMR_CFG_T AMR_Cfg,
                     UINT32 *ItlvBuffInput,
                     UINT32 *RxItlvBufferOut,
                     UINT8 RxBuffOffset,                     
                     UINT32 *BlockDecodedAdd,
                     UINT8 T2,SPP_N_BLOCK_RESULT_T *NBlock_result,
                     UINT8* TxCodecMode,
                     UINT8* RxCodecMode,
                     UINT8 *RxFrameType,
                     UINT8 *ratscch_detected){

    /*volatile struct mBoxBase *mailbox = (struct mBoxBase *) CT_mailbox_base_ptr;*/

    UINT8 FrameType=0;
    UINT8 mode=0;
    UINT8 acs=0;
    UINT8 i=0;
    UINT8 id1=0;
    UINT8 id2=0;
    UINT8 cm1=*RxCodecMode;
    UINT8 cm2=*TxCodecMode;
    UINT8 *ptr_byte;
    

    spp_DeinterleaveBlock(ItlvBuffInput, ITLV_TYPE_2A, RxBuffOffset, RxItlvBufferOut);

    
    FrameType=spp_SearchAmrMarker(RxItlvBufferOut, &id1, &id2, 1); 
    // if id1 is not enough reliable we take the previous value     
    if (id1==0xFF){
        if ((T2==2)||(T2==10)||(T2==19)||(T2==3)||(T2==11)||(T2==20)){
            if (AMR_Cfg.phase==ODD_PHASE) cm1=*RxCodecMode; // odd phase CMI received
            else                          cm1=*TxCodecMode; //even phase CMC received   
        }
        else{
            if (AMR_Cfg.phase==ODD_PHASE) cm1=*TxCodecMode; // odd phase CMC received
            else                          cm1=*RxCodecMode; //even phase CMI received               
        }
    }
    else{   
        if (id1>=AMR_Cfg.nbActiveCodecSet){
            id1=AMR_Cfg.nbActiveCodecSet-1;
        }
        cm1=AMR_Cfg.activeCodecSet[id1];
    }

    
    switch (FrameType){
            
        case SPP_AHS_SPEECH:
            if (*ratscch_detected==2){
                FrameType=SPP_AHS_RATSCCH_DATA;
                id1=spp_GetAmrIc((UINT8*)RxItlvBufferOut, 2);
                if (id1<AMR_Cfg.nbActiveCodecSet) cm1=AMR_Cfg.activeCodecSet[id1];  
            }
            if ((T2==2)||(T2==10)||(T2==19)||(T2==3)||(T2==11)||(T2==20)){
                mode=(AMR_Cfg.phase==ODD_PHASE)?cm1:*RxCodecMode; //odd phase CMI received even phase CMC received  
            }
            else{
                mode=(AMR_Cfg.phase==ODD_PHASE)?*RxCodecMode:cm1; //odd phase CMC received even phase CMI received              
            }
            if (g_spcAmrState.dtxUsed) mode=*RxCodecMode; //to aVOID decoding with a wrong mode during dtx downlink
            spp_DecodeBlock(RxItlvBufferOut,
                            MBOX_TCH_AHS,
                            FrameType,
                            mode,
                            BlockDecodedAdd,
                            0,
                            (SPP_UNCACHED_BUFFERS_T*) g_spcCtx->Malloc,
                            NBlock_result);
            if((NBlock_result->Bfi==0)&&(id1<AMR_Cfg.nbActiveCodecSet)){
                *RxFrameType=SPP_RX_SPEECH_GOOD;
                if ((T2==2)||(T2==10)||(T2==19)||(T2==3)||(T2==11)||(T2==20)){
                    if (AMR_Cfg.phase==ODD_PHASE) *RxCodecMode=cm1; // odd phase CMI received
                    else                          *TxCodecMode=cm1; //even phase CMC received   
                }
                else{
                    if (AMR_Cfg.phase==ODD_PHASE) *TxCodecMode=cm1; // odd phase CMC received
                    else                          *RxCodecMode=cm1; //even phase CMI received               
                }
                if (NBlock_result->BitError > g_spcCtx->AHS_ber1_threshold[*RxCodecMode])
                {
                    *RxFrameType=SPP_RX_SPEECH_DEGRADED;
                } 
                g_spcAmrState.dtxUsed=0;
            }
            else{
                if (NBlock_result->BitError > g_spcCtx->AHS_ber2_threshold[*RxCodecMode])
                {
                    *RxFrameType=SPP_RX_NO_DATA;
                }
                else {*RxFrameType=SPP_RX_SPEECH_BAD;g_spcAmrState.dtxUsed=0;}
            }

            if (*ratscch_detected==2){ *RxFrameType=SPP_RX_NO_DATA;*ratscch_detected=1;}
            break;

        case SPP_AHS_SID_UPDATE:
            g_spcAmrState.dtxUsed=1;
            spp_DeinterleaveBlock(ItlvBuffInput, ITLV_TYPE_2B, RxBuffOffset, RxItlvBufferOut);
            // decode the in band data id0
            id2=spp_GetAmrIc((UINT8*)RxItlvBufferOut, 2);     
            if (id2<AMR_Cfg.nbActiveCodecSet) cm2=AMR_Cfg.activeCodecSet[id2];
            spp_DecodeBlock(RxItlvBufferOut,
                            MBOX_TCH_AHS,
                            FrameType,
                            cm1,
                            BlockDecodedAdd,
                            0,
                            (SPP_UNCACHED_BUFFERS_T*) g_spcCtx->Malloc,
                            NBlock_result);
            if(NBlock_result->Bfi==0){
                *RxFrameType=SPP_RX_SID_UPDATE;
                *TxCodecMode=cm2;
                *RxCodecMode=cm1;
            }
            else {
                if((BlockDecodedAdd[0]==0xFFFFFFFF)&&((BlockDecodedAdd[1]&0x7)==0x7))
                {
                    *RxFrameType=SPP_RX_NO_DATA; // SID FILLER in TFO
                }
                else
                {
                    *RxFrameType=SPP_RX_SID_BAD;
                }
            }
            break;

        case SPP_AHS_SID_UPDATE_INH:
            g_spcAmrState.dtxUsed=0;
            *RxCodecMode=cm1;
            *RxFrameType=SPP_RX_NO_DATA;
            break;

        case SPP_AHS_SID_FIRST_P1:
            g_spcAmrState.dtxUsed=1;
            RxBuffOffset+=2;
            if (RxBuffOffset>4) RxBuffOffset=0;
            spp_DeinterleaveBlock(ItlvBuffInput, ITLV_TYPE_2A, RxBuffOffset, RxItlvBufferOut);
            // decode the in band data in SID_FIRST_P2 (even bits only)
            ptr_byte=(UINT8*)RxItlvBufferOut; 
            for (i=0;i<16;i++){
                ptr_byte[32+i]=ptr_byte[2*i];
            }
            id2=spp_GetAmrIc(ptr_byte+32, 2);  
            if (id2<AMR_Cfg.nbActiveCodecSet) cm2=AMR_Cfg.activeCodecSet[id2];
            *RxFrameType=SPP_RX_SID_FIRST;
            if ((T2==2)||(T2==10)||(T2==19)||(T2==3)||(T2==11)||(T2==20)){
                if (AMR_Cfg.phase==ODD_PHASE) {*RxCodecMode=cm1;*TxCodecMode=cm2;} // odd phase CMI received
                else                          {*TxCodecMode=cm1;*RxCodecMode=cm2;} //even phase CMC received    
            }
            else{
                if (AMR_Cfg.phase==ODD_PHASE) {*TxCodecMode=cm1;*RxCodecMode=cm2;} // odd phase CMC received
                else                          {*RxCodecMode=cm1;*TxCodecMode=cm2;} //even phase CMI received                
            }
            break;

        case SPP_AHS_SID_FIRST_INH:
            g_spcAmrState.dtxUsed=0;

            if ((T2==2)||(T2==10)||(T2==19)||(T2==3)||
                (T2==11)||(T2==20)){
                if (AMR_Cfg.phase==ODD_PHASE) *RxCodecMode=cm1; // odd phase CMI received
                else                          *TxCodecMode=cm1; //even phase CMC received   
            }
            else{
                if (AMR_Cfg.phase==ODD_PHASE) *TxCodecMode=cm1; // odd phase CMC received
                else                          *RxCodecMode=cm1; //even phase CMI received               
            }
            *RxFrameType=SPP_RX_NO_DATA;
            break;

        case SPP_AHS_ONSET:
            g_spcAmrState.dtxUsed=0;
            *RxFrameType=SPP_RX_ONSET;
            *RxCodecMode=cm1; // CMI received
            break;
        
        case SPP_AHS_RATSCCH_MARKER:    
            g_spcAmrState.dtxUsed=0;
            *RxFrameType=SPP_RX_NO_DATA;

            if ((T2==2)||(T2==10)||(T2==19)||(T2==3)||
                (T2==11)||(T2==20)){
                if (AMR_Cfg.phase==ODD_PHASE) *RxCodecMode=cm1; // odd phase CMI received
                else                          *TxCodecMode=cm1; //even phase CMC received   
            }
            else{
                if (AMR_Cfg.phase==ODD_PHASE) *TxCodecMode=cm1; // odd phase CMC received
                else                          *RxCodecMode=cm1; //even phase CMI received               
            }
            *ratscch_detected=2;
            break;
    }

        
    id1=0;
    id2=0;
    for(id1=0;id1<8;id1++){
        if (id1==AMR_Cfg.activeCodecSet[id2]) {id2++;acs|=1<<id1;} 
    }
}

#endif // ENABLE_PATCH_SPC_AHSDECODING
