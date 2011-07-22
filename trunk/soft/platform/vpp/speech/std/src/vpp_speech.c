/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2003-2006, Coolsand Technologies, Inc.     */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vpp_speech.c                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This file contains the VPP API  functions.                        */
/*                                                                       */
/*************************************************************************/
#define VPP_PRINTF

#include "cs_types.h"

#include "hal_error.h"
#include "hal_voc.h"
#include "hal_speech.h"
#include "hal_dma.h"

#include "vppp_speech.h"
#include "vppp_speech_asm.h"
#include "vppp_speech_map.h"

#include "vpp_speech.h"
#include "vppp_debug.h"

#define ECHO_COHF                               			(0) //alias
#define ECHO_TF                                 			(1) //alias

#define PassThrough                             			(0) 
#define AdaptiveFilter                          			(1) 
#define AdaptiveFilterPostFilter                			(2) 
#define NoiseSuppression                        			(3) 
#define LowDelayOpenLoopAF                      			(4) 
#define TwoStepNoiseSuppression                 			(5) 
#define AdaptiveFilterPostFilterEnhanced        			(6) 

// Global variable storing the voc irq handler
PRIVATE VPP_SPEECH_WAKEUP_MODE_T g_vppSpeechWakeupMode   = VPP_SPEECH_WAKEUP_NO; 

INT32 g_RxBuf[160];
INT32 g_TxBuf[160];

//=============================================================================
// vpp_SpeechOpen function
//-----------------------------------------------------------------------------
PUBLIC HAL_ERR_T vpp_SpeechOpen(    HAL_VOC_IRQ_HANDLER_T       vocIrqHandler, 
                                    VPP_SPEECH_WAKEUP_MODE_T    wakeupMode)
{
    
    HAL_ERR_T status;
    HAL_VOC_CFG_T cfg;
    VPP_SPEECH_AUDIO_CFG_T audioCfg;
    VPP_SPEECH_INIT_T *pInit = hal_VocGetPointer(VPP_SPEECH_INIT_STRUCT); 
	

    
    cfg.vocCode              = g_vppSpeechMainCode;
    cfg.vocCodeSize          = g_vppSpeechMainCodeSize;
    cfg.pcVal                = VPP_SPEECH_MAIN_ENTRY;
    cfg.pcValCriticalSecMin  = VPP_SPEECH_CRITICAL_SECTION_MIN;
    cfg.pcValCriticalSecMax  = VPP_SPEECH_CRITICAL_SECTION_MAX;
    cfg.needOpenDoneIrq      = FALSE;
    cfg.irqMask.voc         = (vocIrqHandler) ? 1 : 0;
    cfg.irqMask.dmaVoc      = 0;
    cfg.vocIrqHandler       = vocIrqHandler;
    
    cfg.eventMask.wakeupIfc0 = 0;
    cfg.eventMask.wakeupIfc1 = 0;
    cfg.eventMask.wakeupDmae = 0;
    cfg.eventMask.wakeupDmai = 1;
    cfg.eventMask.wakeupSof0 = 0;
    cfg.eventMask.wakeupSof1 = 0;

    switch (wakeupMode)
    {
            case VPP_SPEECH_WAKEUP_SW_DECENC : 
            case VPP_SPEECH_WAKEUP_SW_ENC    : cfg.eventMask.wakeupSof0 = 1; break;
            case VPP_SPEECH_WAKEUP_SW_DEC    : cfg.eventMask.wakeupSof1 = 1; break;
            case VPP_SPEECH_WAKEUP_HW_DECENC :  
            case VPP_SPEECH_WAKEUP_HW_ENC    : cfg.eventMask.wakeupIfc0 = 1; break;
            case VPP_SPEECH_WAKEUP_HW_DEC    : cfg.eventMask.wakeupIfc1 = 1; break;
            default                          : break;
    }
              
    g_vppSpeechWakeupMode = wakeupMode;
    
    status = hal_VocOpen(&cfg);

    if (HAL_ERR_RESOURCE_BUSY == status) 
    {
        return status;
    }
    else if (HAL_ERR_RESOURCE_RESET == status) 
    {
        UINT16 i;

        // Init code state structure
        for (i = 0; i < 4; i++)
        {
            pInit->codeStateTab[i] = -1;
        }

        pInit->codePtrTab[VPP_SPEECH_EFR_AMR_IDX]          = hal_VocGetDmaiPointer(g_vppEfrAmrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_FR_IDX]               = hal_VocGetDmaiPointer(g_vppFrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_HR_IDX]               = hal_VocGetDmaiPointer(g_vppHrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_IDX]              = hal_VocGetDmaiPointer(g_vppAmrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_EFR_IDX]              = hal_VocGetDmaiPointer(g_vppEfrCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_ENC_475_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode475,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_ENC_515_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode515,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_ENC_59_IDX]       = hal_VocGetDmaiPointer(g_vppAmrEncCode59,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_ENC_67_IDX]       = hal_VocGetDmaiPointer(g_vppAmrEncCode67,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_ENC_74_IDX]       = hal_VocGetDmaiPointer(g_vppAmrEncCode74,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_ENC_795_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode795,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_ENC_102_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode102,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_ENC_122_IDX]      = hal_VocGetDmaiPointer(g_vppAmrEncCode122,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_DEC_475_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode475,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_DEC_515_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode515,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_DEC_59_IDX]       = hal_VocGetDmaiPointer(g_vppAmrDecCode59,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_DEC_67_IDX]       = hal_VocGetDmaiPointer(g_vppAmrDecCode67,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_DEC_74_IDX]       = hal_VocGetDmaiPointer(g_vppAmrDecCode74,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_DEC_795_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode795,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_DEC_102_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode102,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->codePtrTab[VPP_SPEECH_AMR_DEC_122_IDX]      = hal_VocGetDmaiPointer(g_vppAmrDecCode122,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

        pInit->constPtrTab[VPP_SPEECH_EFR_AMR_CONST_X_IDX] = hal_VocGetDmaiPointer(g_vppEfrAmrConstX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->constPtrTab[VPP_SPEECH_EFR_AMR_CONST_Y_IDX] = hal_VocGetDmaiPointer(g_vppEfrAmrConstY,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->constPtrTab[VPP_SPEECH_HR_CONST_X_IDX]      = hal_VocGetDmaiPointer(g_vppHrConstX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
        pInit->constPtrTab[VPP_SPEECH_HR_CONST_Y_IDX]      = hal_VocGetDmaiPointer(g_vppHrConstY,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
		
	

    }


     *((INT32 *)hal_VocGetPointer(GLOBAL_SPEECH_RAMX_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppSpeechRAMX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_SPEECH_CODE_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppSpeechCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_AEC_RAMX_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppAecRAMX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_AEC_CODE_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppAecCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_AEC_CONSTX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppAecConstantX,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);


     *((INT32 *)hal_VocGetPointer(GLOBAL_SPEECH_RAMY_BACKUP_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppSpeechRAMY,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_MORPH_CODE_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppMorphCode,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_MORPH_CONSTY_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_vppMorphConstantY,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);



     *((INT32 *)hal_VocGetPointer(GLOBAL_RX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_RxBuf,HAL_VOC_DMA_WRITE,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
     *((INT32 *)hal_VocGetPointer(GLOBAL_TX_ADDR)) = (INT32 )hal_VocGetDmaiPointer(g_TxBuf,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

	 
/**********************************************AEC******************************************************/
{
	
	VPP_SPEECH_AEC_INIT_T *Aec_Init =hal_VocGetPointer(m_AECProcPara_ADDR);

	Aec_Init->AecEnbleFlag=TRUE;
	Aec_Init->AgcEnbleFlag=FALSE;

	Aec_Init->ProcessMode=AdaptiveFilterPostFilterEnhanced;
	Aec_Init->InitRegularFactor=8;
	Aec_Init->AFUpdateFactor=8;
	Aec_Init->EchoEstMethod=ECHO_COHF;

	Aec_Init->NoiseUpdateFactor=64;
	Aec_Init->SNRUpdateFactor=4;
	Aec_Init->SNRLimit=13;
	Aec_Init->NoiseOverEstFactor=1;

	Aec_Init->NoiseGainLimit=40;
	Aec_Init->EchoUpdateFactor=32;
	Aec_Init->SERUpdateFactor=16;
	Aec_Init->SERLimit=3;

	Aec_Init->EchoOverEstFactor=2;
	Aec_Init->EchoGainLimit=5;
	Aec_Init->CNGFactor=2;
	Aec_Init->NoiseMin=0;

	Aec_Init->AmpGain=16;


	Aec_Init->NoiseGainLimitStep=0;
	Aec_Init->AmpThr=0x7f;

	Aec_Init->PFCLDataSmoothFactor=64;
	Aec_Init->PFCLNoiseUpdateFactor=64;
	Aec_Init->PFCLThrSigDet=160;
	Aec_Init->PFCLThrDT=64;

	Aec_Init->DataSmoothFactor=256;

	Aec_Init->PFCLThrNT=64;
	Aec_Init->PFCLThrDTDiff=6;
	Aec_Init->PFCLDTHoldTime=240;
	Aec_Init->PFCLDTStartTime=80;



	INT16 *NotchFIlterCoef =hal_VocGetPointer(GLOBAL_NOTCH_FILTER_COEF_ADDR);

	NotchFIlterCoef[0]=31919;
	NotchFIlterCoef[1]=30529;
	NotchFIlterCoef[2]=28258;
	NotchFIlterCoef[3]=25169;
	
	NotchFIlterCoef[4]=21354;
	NotchFIlterCoef[5]=16922;
	NotchFIlterCoef[6]=12002;
	NotchFIlterCoef[7]=6734;

	NotchFIlterCoef[8]=1272;
	NotchFIlterCoef[9]=-4227;
	NotchFIlterCoef[10]=-9603;
	NotchFIlterCoef[11]=-14703;

	NotchFIlterCoef[12]=-19377;
	NotchFIlterCoef[13]=-23492;
	NotchFIlterCoef[14]=-26928;
	NotchFIlterCoef[15]=0;


}
/**********************************************AEC******************************************************/

/**********************************************Morph Voice******************************************************/


     *((INT16 *)hal_VocGetPointer(MORPHVOICE_ENABLE_FLAG_ADDR)) =0;

     *((INT16 *)hal_VocGetPointer(PITCH_SHIFT_ADDR)) =0x4000;


	*((INT16 *)hal_VocGetPointer(HIGH_PASS_FILTER_ENABLE_FLAG_ADDR)) =TRUE;
	*((INT16 *)hal_VocGetPointer(NOTCH_FILTER_ENABLE_FLAG_ADDR)) =TRUE;
	*((INT16 *)hal_VocGetPointer(NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =TRUE;
	*((INT16 *)hal_VocGetPointer(NOISE_SUPPRESSER_WITHOUT_SPEECH_ENABLE_FLAG_ADDR)) =TRUE;

/**********************************************Morph Voice******************************************************/
    // irq generation
    switch (wakeupMode)
    {
        case VPP_SPEECH_WAKEUP_SW_ENC: 
        case VPP_SPEECH_WAKEUP_HW_ENC    : pInit->irqgen = VPP_SPEECH_IRQGEN_ENC; break;
        case VPP_SPEECH_WAKEUP_SW_DECENC :
        case VPP_SPEECH_WAKEUP_HW_DECENC :  
        case VPP_SPEECH_WAKEUP_SW_DEC    : 
        case VPP_SPEECH_WAKEUP_HW_DEC    : pInit->irqgen = VPP_SPEECH_IRQGEN_DEC; break;
        default                          : pInit->irqgen = VPP_SPEECH_IRQGEN_NONE; break;
    }
    
    // reset VPP speech
    vpp_SpeechReset();

    // default audio configuration
    audioCfg.echoEsOn   = 0;
    audioCfg.echoEsVad  = 0;
    audioCfg.echoEsDtd  = 0;
    audioCfg.echoExpRel = 0;
    audioCfg.echoExpMu  = 0;
    audioCfg.echoExpMin = 0;
    audioCfg.encMute    = VPP_SPEECH_UNMUTE;
    audioCfg.decMute    = VPP_SPEECH_UNMUTE;
    audioCfg.sdf        = NULL;
    audioCfg.mdf        = NULL;
    
    vpp_SpeechAudioCfg(&audioCfg);

    // invalidate the encoder out buffer
    ((HAL_SPEECH_ENC_OUT_T *)hal_VocGetPointer(VPP_SPEECH_ENC_OUT_STRUCT))->encMode = HAL_INVALID;
    
    // move to STALL location (VoC irq generated)
    if (HAL_ERR_RESOURCE_BUSY == hal_VocWakeup(HAL_VOC_START))
    {
            return HAL_ERR_RESOURCE_BUSY;
    }
 
    return status;
            
}

//=============================================================================
// vpp_SpeechClose function
//-----------------------------------------------------------------------------
/// Close VPP Speech, clear VoC wakeup masks. This function is 
/// called each time a voice call is stopped.
//=============================================================================
PUBLIC VOID vpp_SpeechClose(VOID)
{

    hal_VocClose();
    g_vppSpeechWakeupMode = VPP_SPEECH_WAKEUP_NO;
    
}

//=============================================================================
// vpp_SpeechReset function
//-----------------------------------------------------------------------------
/// Reset VPP (including swap buffer initialization).
//=============================================================================
PUBLIC VOID vpp_SpeechReset(VOID)
{
    ((VPP_SPEECH_INIT_T *)hal_VocGetPointer(VPP_SPEECH_INIT_STRUCT))->reset = 1; 
}

//=============================================================================
// vpp_SpeechAudioCfg function
//-----------------------------------------------------------------------------
/// Configure the audio parameters of VPP Speech.
/// @param pAudioCfg : pointer to the structure containing the audio configuration.
//=============================================================================
PUBLIC VOID vpp_SpeechAudioCfg(VPP_SPEECH_AUDIO_CFG_T *pAudioCfg)
{
        
    VPP_SPEECH_AUDIO_CFG_T *pDest = hal_VocGetPointer(VPP_SPEECH_AUDIO_CFG_STRUCT);
    
    // copy structure
    *pDest = *pAudioCfg;

    // convert to DMAI pointers
    pDest->sdf = hal_VocGetDmaiPointer(pDest->sdf,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    pDest->mdf = hal_VocGetDmaiPointer(pDest->mdf,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    
}

//============================================================================
// vpp_SpeechCodecCfg function
//-----------------------------------------------------------------------------
/// Configure the codec parameters in the decoder structure of VPP Speech.
/// @param pCodecCfg : pointer to the structure containing the decoder configuration.
//=============================================================================
PUBLIC VOID vpp_SpeechCodecCfg(HAL_SPEECH_DEC_IN_T *pCodecCfg)
{
    *(HAL_SPEECH_DEC_IN_T*) vpp_SpeechGetRxCodBuffer() = *pCodecCfg;
}


//=============================================================================
// vpp_SpeechScheduleOneFrame function
//-----------------------------------------------------------------------------
/// Schedule one decoding and/or one encoding frame. Convert to Vpp codec mode. 
/// @return error of type HAL_ERR_T
//=============================================================================
PUBLIC HAL_ERR_T vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_MODE_T    wakeupMode)
{

    HAL_ERR_T status = HAL_ERR_NO;
	
    if (wakeupMode == VPP_SPEECH_WAKEUP_SW_DEC)
    {
        status = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_1);
        
        if (HAL_ERR_NO != status) 
        {
                return status;
        }
    }

    if (wakeupMode == VPP_SPEECH_WAKEUP_SW_ENC)
    {
        status = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);
        
        if (HAL_ERR_NO != status) 
        {
                return status;
        }
    }

	
/*
    if ((g_vppSpeechWakeupMode == VPP_SPEECH_WAKEUP_SW_DECENC) || (g_vppSpeechWakeupMode == VPP_SPEECH_WAKEUP_SW_DEC))
    {
        status = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_1);
        
        if (HAL_ERR_NO != status) 
        {
                return status;
        }
    }

    if ((g_vppSpeechWakeupMode == VPP_SPEECH_WAKEUP_SW_DECENC) || (g_vppSpeechWakeupMode == VPP_SPEECH_WAKEUP_SW_ENC))
    {
        status = hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);        
    }
*/
    return status;
 
}




//=============================================================================
// vpp_SpeechGetRxPcmBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the output (Rx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechGetRxPcmBuffer(VOID)
{
	return (HAL_SPEECH_PCM_BUF_T *)g_RxBuf;
        
    //return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_DEC_OUT_STRUCT);
    
}

//=============================================================================
// vpp_SpeechGetTxPcmBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Tx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechGetTxPcmBuffer(VOID)
{
    	return (HAL_SPEECH_PCM_BUF_T *)g_TxBuf;
        
//    return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_ENC_IN_STRUCT);
    
}

//=============================================================================
// vpp_SpeechGetRxPcmBuffer_test function
//-----------------------------------------------------------------------------
/// @return pointer to the output (Rx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechGetRxPcmBuffer_test(VOID)
{
//	return (HAL_SPEECH_PCM_BUF_T *)g_RxBuf;
        
    return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_DEC_OUT_STRUCT);
    
}

//=============================================================================
// vpp_SpeechGetTxPcmBuffer_test function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Tx) PCM buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_PCM_BUF_T * vpp_SpeechGetTxPcmBuffer_test(VOID)
{
//    	return (HAL_SPEECH_PCM_BUF_T *)g_TxBuf;
        
    return (CONST HAL_SPEECH_PCM_BUF_T *) hal_VocGetPointer(VPP_SPEECH_ENC_IN_STRUCT);
    
}

//=============================================================================
// vpp_SpeechGetRxCodBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the input (Rx) coded buffer
//=============================================================================
PUBLIC HAL_SPEECH_DEC_IN_T * vpp_SpeechGetRxCodBuffer(VOID)
{
        
    return (HAL_SPEECH_DEC_IN_T *) hal_VocGetPointer(VPP_SPEECH_DEC_IN_STRUCT);
    
}

//=============================================================================
// vpp_SpeechGetTxCodBuffer function
//-----------------------------------------------------------------------------
/// @return pointer to the output (Tx) coded buffer
//=============================================================================
PUBLIC CONST HAL_SPEECH_ENC_OUT_T * vpp_SpeechGetTxCodBuffer(VOID)
{
        
    return (CONST HAL_SPEECH_ENC_OUT_T *) hal_VocGetPointer(VPP_SPEECH_ENC_OUT_STRUCT);
    
}



//=============================================================================
// vpp_SpeechSetReceiverDigitalGain function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC   VOID vpp_SpeechSetReceiverDigitalGain(UINT16 DigitalGain)
{

	switch (DigitalGain)
	{
		case 0:			
			DigitalGain=0x10;
			break;
		case 1:			
			DigitalGain=0x16;           ///1.3
			break;
		case 2:			
			DigitalGain=0x20;       ///1.3^2
			break;
		case 3:			
			DigitalGain=0x2d;     ///1.3^3
			break;
		default:
                    DigitalGain=0x10;
			break;	
	}
        
    * ( UINT16 *) hal_VocGetPointer(GLOBAL_RECEIVER_DIGITAL_GAIN_ADDR)=DigitalGain;
    * ( UINT16 *) hal_VocGetPointer(GLOBAL_RECEIVER_DIGITAL_MAXVALUE_ADDR)=(INT16)(((INT32)(32767*16))/DigitalGain);

	 return;
    
}

//=============================================================================
// vpp_SpeechSetAecEnableFlag function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC   VOID vpp_SpeechSetAecEnableFlag(INT8 AecEnbleFlag,INT8 AgcEnbleFlag)
{
        
	VPP_SPEECH_AEC_INIT_T *Aec_Init =hal_VocGetPointer(m_AECProcPara_ADDR);

	Aec_Init->AecEnbleFlag=AecEnbleFlag;
	Aec_Init->AgcEnbleFlag=AgcEnbleFlag;

	 return;
    
}


//=============================================================================
// vpp_SpeechSetMicDigitalGain function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC   VOID vpp_SpeechSetMicDigitalGain(UINT16 DigitalGain)
{
        DigitalGain=DigitalGain>>2;
    * ( UINT16 *) hal_VocGetPointer(GLOBAL_MIC_DIGITAL_GAIN_ADDR)=DigitalGain;
    * ( UINT16 *) hal_VocGetPointer(GLOBAL_MIC_DIGITAL_MAXVALUE_ADDR)=(INT16)(((INT32)(32767*16))/DigitalGain);


	 return;
    
}



//=============================================================================
// vpp_SpeechSetMorphVoice function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC   VOID vpp_SpeechSetMorphVoice(INT16 EnableFlag, INT16 MorphPara)
{
        
     *((INT16 *)hal_VocGetPointer(MORPHVOICE_ENABLE_FLAG_ADDR)) =EnableFlag;

     *((INT16 *)hal_VocGetPointer(PITCH_SHIFT_ADDR)) =MorphPara;


}



//=============================================================================
// vpp_SpeechSetAecPara function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC   VOID vpp_SpeechSetAecPara(INT8 NoiseGainLimit,INT8 NoiseMin,INT8 NoiseGainLimitStep,INT8 AmpThr)
{
        
	VPP_SPEECH_AEC_INIT_T *Aec_Init =hal_VocGetPointer(m_AECProcPara_ADDR);

	Aec_Init->NoiseGainLimit=NoiseGainLimit;
	Aec_Init->NoiseMin=NoiseMin;
	Aec_Init->NoiseGainLimitStep=NoiseGainLimitStep;
	Aec_Init->AmpThr=AmpThr;

	 return;
    
}


//=============================================================================
// vpp_SpeechSetFilterPara function
//-----------------------------------------------------------------------------
/// 
//=============================================================================
PUBLIC   VOID vpp_SpeechSetFilterPara(INT8 HighPassFilterFlag,INT8 NotchFilterFlag,INT8 NoiseSuppresserFlag,INT8 NoiseSuppresserWithoutSpeechFlag)
{
        
	*((INT16 *)hal_VocGetPointer(HIGH_PASS_FILTER_ENABLE_FLAG_ADDR)) =HighPassFilterFlag;
	*((INT16 *)hal_VocGetPointer(NOTCH_FILTER_ENABLE_FLAG_ADDR)) =NotchFilterFlag;
	*((INT16 *)hal_VocGetPointer(NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)) =NoiseSuppresserFlag;
	*((INT16 *)hal_VocGetPointer(NOISE_SUPPRESSER_WITHOUT_SPEECH_ENABLE_FLAG_ADDR)) =NoiseSuppresserWithoutSpeechFlag;
	
	 return;
    
}








