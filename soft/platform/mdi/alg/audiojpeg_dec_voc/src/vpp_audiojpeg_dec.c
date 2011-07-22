/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2003-2006, Coolsand Technologies, Inc.     */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vpp.c                                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This file contains the VPP API  functions.                        */
/*                                                                       */
/*************************************************************************/
//#include "stdio.h"

#include "cs_types.h"

#include "hal_error.h"
#include "hal_voc.h"

#include "vpp_audiojpeg_dec_asm.h"
#include "mcip_debug.h"
#include "vpp_audiojpeg_dec.h"

#include "calib_m.h"

CONST INT16 f_sbc_persition[30]= 
{
	2, 2, 2, 4, 4, 
	4, 4, 4, 4, 8, 8, 8, 8, 
	8, 8, 16, 16, 16, 16, 16, 32, 
	32, 32, 32, 32, 64, 64, 64, 64,
	0
};

BOOL vpp_AudioJpegDecVocOpen_status=FALSE;

//=============================================================================
// vpp_AudioJpegDecOpen function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_AudioJpegDecOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler) 
{
    HAL_VOC_CFG_T cfg;
    vpp_AudioJpeg_DEC_IN_T * pDecIn;
    vpp_AudioJpeg_DEC_OUT_T *pDecStatus;
    INT32 * *pDMA;

    CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
	
   diag_printf("[vpp_AudioJpeg_DEC]Opening VPP AUDIO_DEC\n"); 
    
      
    cfg.vocCode              = G_VppCommonDecCode;
    cfg.vocCodeSize        = vpp_AudioJpeg_DEC_CODE_SIZE;
    cfg.pcVal                   = vpp_AudioJpeg_DEC_CODE_ENTRY;
    cfg.pcValCriticalSecMin  = vpp_AudioJpeg_DEC_CRITICAL_SECTION_MIN;
    cfg.pcValCriticalSecMax  = vpp_AudioJpeg_DEC_CRITICAL_SECTION_MAX;
    cfg.needOpenDoneIrq      = FALSE;
    cfg.irqMask.voc          = (vocIrqHandler) ? 1 : 0;
    cfg.irqMask.dmaVoc       = 0;
    cfg.vocIrqHandler        = vocIrqHandler;
    
    cfg.eventMask.wakeupIfc0 = 0;
    cfg.eventMask.wakeupIfc1 = 0;
    cfg.eventMask.wakeupDmae = 0;
    cfg.eventMask.wakeupDmai = 0;
    cfg.eventMask.wakeupSof0 = 0;
    cfg.eventMask.wakeupSof1 = 0;
    
    // load the VPP AUDIO_DEC  code and configure the VoC resource        
    switch (hal_VocOpen(&cfg))
    {
        // error opening the resource
        case HAL_ERR_RESOURCE_BUSY:                 
            diag_printf("[vpp_AudioJpeg_DEC]##WARNING##Error opening VoC resource\n"); 
            return HAL_ERR_RESOURCE_BUSY;
                                        
        // first open, load the constant tables                            
        case HAL_ERR_RESOURCE_RESET:              
            diag_printf("[vpp_AudioJpeg_DEC]First open.\n");			
            break;

        default:           
            diag_printf("[vpp_AudioJpeg_DEC]No first open.\n"); 
            break;
    }                          

   diag_printf("[vpp_AudioJpeg_DEC]Initializing the DMA addr.\n");
      //mp3
   	pDMA = hal_VocGetPointer(VPP_MP3_Code_ExternalAddr_addr);
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3DecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3DecConstX,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3DecConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    
	//aac
	pDMA = hal_VocGetPointer(VPP_AAC_Code_ExternalAddr_addr);

	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3Layer12Dec_DMA_ConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);	
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

	//amr
	pDMA = hal_VocGetPointer(VPP_AMR_Code_ExternalAddr_addr);

	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAmrDecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppMp3Dec_EQ_DMA_ConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAmrDecConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

#ifndef TARGET_VIDEO_PLAYER_SUPPORT_OFF
	//jpeg
	pDMA = hal_VocGetPointer(VPP_JPEG_Code_ExternalAddr_addr);

	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppJpegDecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppJpegDecConstY,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);


	
	pDMA = hal_VocGetPointer(VPP_AAC_Tab_huffTabSpec_START_addr);
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecHuffTabSpec,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecCos4sin4tab,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecTwidTabOdd,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	pDMA=pDMA+1;
	*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppAacDecSinWindow,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

      //h263 zoom
	pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
     *pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263ZoomCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	 pDMA=pDMA+1;
     *pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263ZoomConstX,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
#endif

       //sbc encode
	pDMA = hal_VocGetPointer(VPP_SBC_ENC_Code_ExternalAddr_addr);
     *pDMA=hal_VocGetDmaiPointer((INT32*)G_VppSBCCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);

	pDMA = hal_VocGetPointer(VPP_Speak_AntiDistortion_Filter_Coef_addr);
     *pDMA=hal_VocGetDmaiPointer((INT32*)(&(calibPtr->bb->audio[4].vocFilters.sdfFilter[0])),  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);


     for(INT32 i=0;i<30;i++)
     {
        calibPtr->bb->audio[4].vocFilters.sdfFilter[32+i]=f_sbc_persition[i];
     }

	pDecIn = hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
	pDecIn->reset=1;
	pDecIn->EQ_Type = -1;  	


    
	 pDecStatus = (vpp_AudioJpeg_DEC_OUT_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_OUT_STRUCT);
	 pDecStatus->ErrorStatus=0;
	 pDecStatus->mode=-1;//not 0~9;
 

    // move to STALL location (VoC irq generated)

      vpp_AudioJpegDecVocOpen_status=TRUE;

    return  hal_VocWakeup(HAL_VOC_START);
            
}


//=============================================================================
// vpp_AudioJpegDecClose function
//-----------------------------------------------------------------------------
void vpp_AudioJpegDecClose(void)
{
    if(vpp_AudioJpegDecVocOpen_status==TRUE)
	    hal_VocClose();   
    vpp_AudioJpegDecVocOpen_status=FALSE;    
    diag_printf("[vpp_AudioJpeg_DEC]Closing VPP\n");
    
}
//=============================================================================
// vpp_AudioJpegDecStatus function
//-----------------------------------------------------------------------------
void vpp_AudioJpegDecStatus(vpp_AudioJpeg_DEC_OUT_T * pDecStatus)
{  
    // copy status structure from VoC RAM to the destination.
    *pDecStatus = *((vpp_AudioJpeg_DEC_OUT_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_OUT_STRUCT));
}

//=============================================================================
// vpp_AudioJpegDecScheduleOneFrame function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_AudioJpegDecScheduleOneFrame(vpp_AudioJpeg_DEC_IN_T *pDecIn)
{
    vpp_AudioJpeg_DEC_IN_T * pDecInVoC = (vpp_AudioJpeg_DEC_IN_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
	
 	*pDecInVoC=*pDecIn;
  	
    pDecInVoC->inStreamBufAddr   = hal_VocGetDmaiPointer(pDecIn->inStreamBufAddr,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    pDecInVoC->inStreamUBufAddr   = hal_VocGetDmaiPointer(pDecIn->inStreamUBufAddr,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    pDecInVoC->inStreamVBufAddr   = hal_VocGetDmaiPointer(pDecIn->inStreamVBufAddr,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    pDecInVoC->outStreamBufAddr = hal_VocGetDmaiPointer(pDecIn->outStreamBufAddr,HAL_VOC_DMA_WRITE,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
    pDecIn->reset=0;
    // diag_printf("[vpp_AudioJpeg_DEC]Codecmode:%d\n",pDecInVoC->mode); 	
     
    // schedule next frame on sof1 event
    return hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);
 
}


HAL_ERR_T  vpp_AudioJpegDecSetCurMode(INT16 pVoc_Video_Mode, INT16 pVoc_Audio_Mode)
{
	INT16 * pointVoC = hal_VocGetPointer(VPP_Current_Audio_Mode_addr);

	*pointVoC=pVoc_Audio_Mode;
	pointVoC=pointVoC+1;
	*pointVoC=pVoc_Video_Mode;
	
	return HAL_ERR_NO;
}


HAL_ERR_T  vpp_AudioJpegDecSetInputMode(INT16 pVoc_Input_Mode)
{
	vpp_AudioJpeg_DEC_IN_T * pDecInVoC = (vpp_AudioJpeg_DEC_IN_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
	
   	pDecInVoC->mode=pVoc_Input_Mode;
	pDecInVoC->BsbcEnable=0;
    
    	return HAL_ERR_NO;
}

INT16  vpp_AudioJpegDecGetMode(void)
{
	vpp_AudioJpeg_DEC_IN_T * pDecInVoC = (vpp_AudioJpeg_DEC_IN_T *)hal_VocGetPointer(vpp_AudioJpeg_DEC_IN_STRUCT);
	return pDecInVoC->mode;
}

//=============================================================================
// vpp_AudioJpegSetReuseCode function
//reuse with VPP_H263_Zoom_Code_ExternalAddr_addr
//code_id: 0=zoom, 1=rmvb, 2=264 ...
//set after vpp_AudioJpegDecOpen			
//-----------------------------------------------------------------------------
void vpp_AudioJpegSetReuseCode(INT16 code_id)
{
	INT32 * *pDMA;

	switch(code_id)
	{
		case 0:
			//h263 zoom
			pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263ZoomCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			pDMA=pDMA+1;
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppH263ZoomConstX,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			break;
		case 1:
			//rmvb
			pDMA = hal_VocGetPointer(VPP_H263_Zoom_Code_ExternalAddr_addr);
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppRMDecCode,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			pDMA=pDMA+1;
			*pDMA=hal_VocGetDmaiPointer((INT32*)G_VppRMDecConsty,  HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
			break;
		default:
			break;
	}
}

