/******************************************************************************/
/*              Copyright (C) 2003, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:  Vpp_wma_dec.c														  */
/* Description:                                                               */
/*   Main test function														  */
/******************************************************************************/


//#include "lily.h"
//#include "ctype.h"
//#include "voc2.h"
#include "cs_types.h"
//#include "lp_ct810reg.h"
//#include "global_macros.h"
#include "hal_voc.h"
//#include "voc_cfg.h"
//#include "hal_private.h"
#include "hal_error.h"

#include "vpp_wma_dec.h"
#include "wmaudio.h"
#include "wmaudio_type.h"
#include "map_addr.h"

#include "mcip_debug.h"

const UINT32 TabinX[]={
#include "tabinx.tab"
};	
const UINT32 TabinY[]={
#include "tabiny.tab"
};
#include "huffmantbl.tab"
#include "g_sintableforreconstruction.tab"
#include "costab.tab"


const UINT32 wma_codeM[] = {
	#include "wma_common.tab"
	#include "wma_highmid.tab"
	#include "wma_midlow.tab"
	#include "wma_mid.tab"	
};
const UINT32 wma_codeH[] = {
	#include "wma_common.tab"
	#include "wma_highmid.tab"
	#include "wma_high.tab"
};

const UINT32 wma_codeL[] = {
	#include "wma_commonforl.tab"
};
const UINT32 wma_codeL_LPC[] = {
	#include "rlcodelpc.tab"
};
const UINT32 wma_codeL_IDCT[] = {
	#include "rlcodeinvedct4.tab"
};


//unsigned short WMA_Decoder[684];
HAL_ERR_T vpp_WmaGetInfo( int32 * pStreamIn ,VPP_WMA_DEC_CONTENT_T * pWmaProperty )
{
	int i;
 	tWMAFileStatus			iResult;				// api result       
	tWMAFileStateInternal WMAFileState;
    	uint8 *pchar;
	
    	diag_printf("VPP get wma file info\n"); 

	pchar = (int8 *)(&WMAFileState);
	for(i=0;i<sizeof(tWMAFileStateInternal);i++)
		*pchar++=0;

	WMAFileState.hdr_parse.pInput = pStreamIn;
	// initialize decoder
	iResult = WMAFileGetInfo((tHWMAFileState)(&WMAFileState));
	if(cWMA_NoErr != iResult)	return HAL_ERR_RESOURCE_NOT_ENABLED;

	pWmaProperty->nVersion = WMAFileState.hdr_parse.nVersion;
	pWmaProperty->nChannels = WMAFileState.hdr_parse.nChannels;
	pWmaProperty->nSamplesPerSec= WMAFileState.hdr_parse.nSamplesPerSec;
	pWmaProperty->nAvgBitsPerSec= WMAFileState.hdr_parse.nAvgBytesPerSec*8;
	pWmaProperty->msDuration= WMAFileState.hdr_parse.msDuration;

    	diag_printf("msDuration = %d\n",pWmaProperty->msDuration); 
	
	
	return HAL_ERR_NO;
}


//=============================================================================
// vpp_WmaDecOpen function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_WmaDecOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler, int32 * pStreamIn, 
			int16 * PrevOutput, VPP_WMA_DEC_CONTENT_T * pWmaProperty, int8 ifOpenVoc) 
{
    HAL_VOC_CFG_T cfg;
    HAL_VOC_DMA_CFG_T dmaCfg;
	//int * pIntA,*pIntB;
	int *pIntB;
	int i;
 	tWMAFileStatus			iResult;				// api result       
	int WeightingMode,fNoiseSub;
	tWMAFileStateInternal WMAFileState;
    	uint8 *pchar;

	diag_printf("[VPP_WMA]##WARNING##Opening VPP WMA Decoder\n"); 
	

// pls try to remove following  code! i havnt found the why error occurs when switch to 16K wma songs without the ram clr.
/////////////////////////////
		pIntB = (int*)hal_VocGetPointer(GLOBAL_tWMAFileStateInternal_ADDR);   
		
		for(i=0;i<(684)/2;i++)

		{
			*(pIntB++) = 0;
		} 	
////////////////////////////////////
	
	pchar = (int8 *)(&WMAFileState);
	for(i=0;i<sizeof(tWMAFileStateInternal);i++)
		*pchar++=0;

	WMAFileState.hdr_parse.pInput = pStreamIn;
	// initialize decoder
	iResult = WMAFileDecodeInit((tHWMAFileState)(&WMAFileState));
//   	diag_printf("init iResult =%d  \n",iResult); // added by liyongjian for debug
	if(cWMA_NoErr != iResult)	return HAL_ERR_RESOURCE_NOT_ENABLED;

	pWmaProperty->nVersion = WMAFileState.hdr_parse.nVersion;
	pWmaProperty->nChannels = WMAFileState.hdr_parse.nChannels;
	pWmaProperty->nSamplesPerSec= WMAFileState.hdr_parse.nSamplesPerSec;
	pWmaProperty->nAvgBitsPerSec= WMAFileState.hdr_parse.nAvgBytesPerSec*8;
	pWmaProperty->msDuration= WMAFileState.hdr_parse.msDuration;
	
 	if (WMA_OPEN_VOC_NO == ifOpenVoc)		return HAL_ERR_NO;
	
	WeightingMode = *(short *)hal_VocGetPointer(GLOBAL_m_iWeightingMode_ADDR);// LPC =0 BARD =1
	fNoiseSub = *(short *)hal_VocGetPointer(GLOBAL_m_fNoiseSub_ADDR);// LPC =0 BARD =1
   	diag_printf("[VPP_WMA]##WARNING##WeightingMode =%d fNoiseSub =%d \n",WeightingMode,fNoiseSub); // added by liyongjian for debug

	if(!WeightingMode||fNoiseSub)
		{
		if(WeightingMode)
			{
			if(fNoiseSub)
				{
				    cfg.vocCode              = wma_codeM;
				    cfg.vocCodeSize          = VPP_WMA_DEC_CODE_M_SIZE;			
				}
			else
				return HAL_ERR_RESOURCE_NOT_ENABLED;			
			}
		else
			{
				    cfg.vocCode              = wma_codeL;
				    cfg.vocCodeSize          = VPP_WMA_DEC_CODE_L_SIZE;						
			}
		}
	else
		{
		    cfg.vocCode              = wma_codeH;
		    cfg.vocCodeSize          = VPP_WMA_DEC_CODE_H_SIZE;		
		}
	
    cfg.pcVal                = VPP_WMA_DEC_CODE_ENTRY;
    cfg.pcValCriticalSecMin  = VPP_WMA_DEC_CRITICAL_SECTION_MIN;
    cfg.pcValCriticalSecMax  = VPP_WMA_DEC_CRITICAL_SECTION_MAX;
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
    
    // load the VPP Wma Decoder code and configure the VoC resource        
    switch (hal_VocOpen(&cfg))
    {
        // error opening the resource
        case HAL_ERR_RESOURCE_BUSY:     
            
			diag_printf("[VPP_WMA]##ERROR##Error opening VoC resource\n"); 
            return HAL_ERR_RESOURCE_BUSY;
                                        
        // first open, load the constant tables                            
        case HAL_ERR_RESOURCE_RESET:    
                                     
            // load X constant tables
            dmaCfg.extAddr           = (UINT32*)TabinX;
            dmaCfg.vocLocalAddr      = (UINT32*)hal_VocGetPointer(GLOBAL_ROM_BUFFER_X_ADDR);
            dmaCfg.size              = VPP_WMA_DEC_ROM_BUFFER_X_SIZE;
            dmaCfg.wr1Rd0            = HAL_VOC_DMA_READ;
            dmaCfg.needIrq           = FALSE;
    
            if (hal_VocDmaStart(&dmaCfg) == HAL_ERR_RESOURCE_BUSY)
            {
				diag_printf("[VPP_WMA]##WARNING##Error opening VoC DMA resource\n");
                return HAL_ERR_RESOURCE_BUSY;
            }

            while (hal_VocDmaDone() != TRUE);
    
            // load Y constant tables
            dmaCfg.extAddr           = (UINT32*)TabinY;
            dmaCfg.vocLocalAddr      = (UINT32*)hal_VocGetPointer(GLOBAL_ROM_BUFFER_Y_ADDR);
            dmaCfg.size              = VPP_WMA_DEC_ROM_BUFFER_Y_SIZE;
            dmaCfg.wr1Rd0            = HAL_VOC_DMA_READ;
            dmaCfg.needIrq           = FALSE;
    
            if (hal_VocDmaStart(&dmaCfg) == HAL_ERR_RESOURCE_BUSY)
            {
				diag_printf("[VPP_WMA]##WARNING##Error opening VoC DMA resource");
                return HAL_ERR_RESOURCE_BUSY;
            }

            while (hal_VocDmaDone() != TRUE);
            break;

        default:
           
			diag_printf("[VPP_WMA]##WARNING##No first open, no code and constants reload\n"); 
            break;
    }                          


     
	*((short*)hal_VocGetPointer(GLOBAL_wmaResult_ADDR))= cWMA_NoErr;
	*((short*)hal_VocGetPointer(LOCAL_pcSampleGet_ADDR))= 0x0000;	
	//fft table
	*(int *)hal_VocGetPointer(GLOBAL_FFTcostabExAddr_ADDR)=  (INT32)hal_VocGetDmaiPointer((INT32 *)costab,0,0,HAL_VOC_DMA_B2S_NO);
	//save history and reconstruction table
	*(int *)hal_VocGetPointer(GLOBAL_m_piSinForRecon_EXT_ADDR)=  (INT32)hal_VocGetDmaiPointer((INT32 *)g_SinTableForReconstruction,0,0,HAL_VOC_DMA_B2S_NO);
	//save history table
	*(int *)hal_VocGetPointer(GLOBAL_m_piSinForSaveHistory_ADDR)=(INT32) hal_VocGetDmaiPointer((INT32 *)(g_SinTableForReconstruction+2048),0,0,HAL_VOC_DMA_B2S_NO);	
	//history data buffer: short [2048*2]
	*(int *)hal_VocGetPointer(GLOBAL_PrevOutputDMAExAddr_ADDR)=(INT32) hal_VocGetDmaiPointer((INT32 *)PrevOutput,0,0,HAL_VOC_DMA_B2S_NO);
	//output pcm data buffer: short [2048*2] & VOC_CFG_DMA_EADDR_MASK;
//	*(int *)hal_VocGetPointer(GLOBAL_OutPcmDMAExAddr_ADDR)= VOC_DMA_EXTERN_ADDR((int)iPCMBuffer);
	 // huffman tables
	*(int *)hal_VocGetPointer(GLOBAL_HuffDecTblDMAExAddr_ADDR)= (INT32)hal_VocGetDmaiPointer((INT32 *)HuffmanTbl,0,0,HAL_VOC_DMA_B2S_NO);
	*(int32*)hal_VocGetPointer(GLOBAL_ReLoadLPC_EADDR)=(INT32) hal_VocGetDmaiPointer((INT32 *)wma_codeL_LPC,0,0,HAL_VOC_DMA_B2S_NO);  
	*(int32*)hal_VocGetPointer(GLOBAL_ReLoadIDCT_EADDR)=(INT32) hal_VocGetDmaiPointer((INT32 *)wma_codeL_IDCT,0,0,HAL_VOC_DMA_B2S_NO);

    *(int32*)hal_VocGetPointer(GLOBAL_INPUT_OFFSET_ADDR)= (*(INT32*)hal_VocGetPointer(GLOBAL_currPacketOffset_ADDR))&0xfffffffc;
   *(int32*)hal_VocGetPointer(GLOBAL_INPUT_DATALEN_ADDR)= 4096;    
  
// init stream buffer
    dmaCfg.extAddr           = (INT32*)pStreamIn +((*(int32*)hal_VocGetPointer(GLOBAL_currPacketOffset_ADDR))>>2);
    dmaCfg.vocLocalAddr      = (INT32*)hal_VocGetPointer(GLOBAL_INPUT_DATA_ADDR);
    dmaCfg.size              = 4096;
    dmaCfg.wr1Rd0            = HAL_VOC_DMA_READ;
    dmaCfg.needIrq           = FALSE;
    
    if (hal_VocDmaStart(&dmaCfg) == HAL_ERR_RESOURCE_BUSY)
    {
		diag_printf("[VPP_WMA]##ERROR##Error opening VoC DMA resource");
        return HAL_ERR_RESOURCE_BUSY;
    }

//	diag_printf("[VPP_WMA]start wait voc dam done\n");    
    while (hal_VocDmaDone() != TRUE);
	diag_printf("[VPP_WMA]voc wakeup!\n");    
    
    // move to STALL location (VoC irq generated)
    hal_VocWakeup(HAL_VOC_START);
 
    return HAL_ERR_NO;
            
}


//=============================================================================
// vpp_WmaDecClose function
//-----------------------------------------------------------------------------
void vpp_WmaDecClose(void)
{
    hal_VocClose();   
    diag_printf("[VPP_WMA]##WARNING##Closing VPP");
}

//=============================================================================
// vpp_WmaDecStatus function
//-----------------------------------------------------------------------------
void vpp_WmaInitStatus(VPP_WMA_DEC_OUT_T * pDecStatus)
{  
    // copy status structure from VoC RAM to the destination.
	pDecStatus->consumedLen = ((*(int*)hal_VocGetPointer(GLOBAL_INPUT_OFFSET_ADDR))+4096)>>2;   	
	pDecStatus->inputOffset = *(int*)hal_VocGetPointer(GLOBAL_INPUT_OFFSET_ADDR);
	pDecStatus->iSamplingRate = *(int*)hal_VocGetPointer(GLOBAL_m_iSamplingRate_ADDR);
 	 
	pDecStatus->WmaFileStatus = (VPP_WMA_DEC_STREAM_STATUS)*((short*)hal_VocGetPointer(GLOBAL_wmaResult_ADDR));
	pDecStatus->pcmFrameLen =(*(short*)hal_VocGetPointer(LOCAL_pcSampleGet_ADDR))>>2;
	pDecStatus->nbChannel = *((short*)hal_VocGetPointer(GLOBAL_m_cChannel_ADDR));
	
 	diag_printf("[VPP_WMA]##WARNING##consumedLen : %x  iSamplingRate :%d \n",pDecStatus->consumedLen,pDecStatus->iSamplingRate );
}

//=============================================================================
// vpp_WmaDecStatus function
//-----------------------------------------------------------------------------
void vpp_WmaDecStatus(VPP_WMA_DEC_OUT_T * pDecStatus)
{  
    // copy status structure from VoC RAM to the destination.
    	int  tempInputOffset;
	tempInputOffset = *(int32*)hal_VocGetPointer(GLOBAL_INPUT_OFFSET_ADDR);

	pDecStatus->consumedLen = (tempInputOffset - pDecStatus->inputOffset)>>2;
	pDecStatus->inputOffset = tempInputOffset;
	pDecStatus->WmaFileStatus = (VPP_WMA_DEC_STREAM_STATUS)*((short*)hal_VocGetPointer(GLOBAL_wmaResult_ADDR));
	pDecStatus->pcmFrameLen =*((short*)hal_VocGetPointer(LOCAL_pcSampleGet_ADDR));
	pDecStatus->nbChannel = *((short*)hal_VocGetPointer(GLOBAL_m_cChannel_ADDR));
	pDecStatus->EQtype = *((short*)hal_VocGetPointer(GLOBAL_EQtype_ADDR));

}


//=============================================================================
// vpp_WmaDecScheduleOneFrame function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_WmaDecScheduleOneFrame(INT32 * pStreamIn, INT32 * pStreamOut, UINT8 EQMode)
{
    // fill in the input structure
	*(INT32*)hal_VocGetPointer(GLOBAL_FILE_POINTER_ADDR) = (INT32)hal_VocGetDmaiPointer(pStreamIn,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	*(INT32*)hal_VocGetPointer(GLOBAL_OutPcmDMAExAddr_ADDR) = (INT32)hal_VocGetDmaiPointer(pStreamOut,HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
   	*((INT16*)hal_VocGetPointer(GLOBAL_EQtype_ADDR)) = (INT16)EQMode;
    // schedule next frame on sof1 event
    return hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_1);
 
}

