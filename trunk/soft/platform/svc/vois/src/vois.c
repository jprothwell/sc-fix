////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/vois/src/vois.c $ //
//	$Author: xuml $                                                        // 
//	$Date: 2010-06-17 14:53:21 +0800 (Thu, 17 Jun 2010) $                     //   
//	$Revision: 36750 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file vois.c                                                               //
/// That file implementes the VOIS service.                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "string.h"

#include "hal_timers.h"
#include "hal_aif.h"
#include "hal_sys.h"
#include "hal_speech.h"

#include "vois_m.h"
#include "vois_map.h"
#include "voisp_map.h"
#include "voisp_cfg.h"

#include "aud_m.h"
#include "vpp_speech.h"
#include "calib_m.h"

#include "hal_map_engine.h"

#include "sxr_sbx.h"

#ifdef MAGIC_SOUND_SUPPORT
#include "jtae.h"
//#include "mdi_audio.h"
#endif


/// TODO Does this part of the doc belong to the API ?
/// Workflow description
///   Init:
///     Init VPP, and load the first encoded sample (Rx) in its decoding buffer,
///     and get the first (invalid) encoded sample (Tx) from the encoding buffer
///     thanks to the first interrupt that is generated after the opening of VPP
///     is complete and before any data is processed.
///     Mute mode is set.
///     Lauch play on the output decoded buffer of VPP.
///
///   Data flow:
///     Data are copied on VPP interrupts, meaning the previous set has been
///     decoded/encoded. Decoding and encoding are triggered by the IFC events
///     directly telling VPP what to do.
///     On IRQ from VPP, we copy the next buffer to be decoded and the freshly
///     encoded one to put in the Speech Fifo.



//#define SPEEH_RECORD_PCM_ENABLE


#ifdef SPEEH_RECORD_PCM_ENABLE
#include "cos.h"
#include "fs.h"

#endif


// =============================================================================
//  MACROS
// =============================================================================

/// FIXME USE the definitive notation from 
#define VPP_RX_BUFFER_SIZE  (sizeof(HAL_SPEECH_DEC_IN_T))
#define VPP_TX_BUFFER_SIZE  (sizeof(HAL_SPEECH_ENC_OUT_T))

        
/// Number of trames before unmuting AUD
#define VOIS_AUD_UNMUTE_TRAME_NB 2

/// Number of trames before unmuting VPP
#define VOIS_VPP_UNMUTE_TRAME_NB 2

#define VOC_INTR_STEP


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================



// =============================================================================
// g_voisCtx
// -----------------------------------------------------------------------------
/// VoiS context, holding global variables, to be watched through CoolWatcher.
// =============================================================================
PROTECTED VOIS_CONTEXT_T g_voisCtx =
{
            .voisStarted = FALSE,
            .voisEncOutput = NULL,
            .voisDecInput = NULL, 
            .voisTramePlayedNb = 0,
            .voisAudioCfg = {0,0,0,0},
            .voisVppCfg = {0,0,0,0,0,0,0,0,NULL, NULL,0},
    .voisItf = AUD_ITF_NONE,
    .voisLatestCfg =
    {
                                .spkSel     = AUD_SPK_RECEIVER,
                                .micSel     = AUD_MIC_RECEIVER,
                                .spkLevel   = AUD_SPK_MUTE,
                                .micLevel   = AUD_MIC_MUTE,
                                .sideLevel  = AUD_SIDE_MUTE,
                                .toneLevel  = AUD_TONE_0dB,
                                .encMute    = VPP_SPEECH_MUTE, 
        .decMute    = VPP_SPEECH_MUTE
    },
            .mode = HAL_AMR475_ENC & HAL_AMR475_DEC,
            .bufferSize = HAL_SPEECH_FRAME_SIZE_AMR475
};

/// Stream to encode.
PRIVATE HAL_AIF_STREAM_T g_voisPcmTxStream;

/// Decoded stream.
PRIVATE HAL_AIF_STREAM_T g_voisPcmRxStream;
       


// =============================================================================
// sample
// -----------------------------------------------------------------------------
/// A dirty test variable!
// =============================================================================
#ifdef VOIS_LOOP

UINT32 sample []= {
    #include "amr_ring_122_10sec.dat"
};

volatile BOOL g_voisUseSample = FALSE;
volatile UINT32 g_voisIndex = 0;
#endif // VOIS_LOOP

// =============================================================================
// sample
// -----------------------------------------------------------------------------
/// A dirty PCM dump variable!
// =============================================================================
#ifdef VOIS_DUMP_PCM

#define NB_DUMP_PCM_FRAMES 1000   

// to avoid the buffer corruption during the encode the dump for Tx is done in a external buffer
UINT32 g_voisPcmTxBuffer[HAL_SPEECH_FRAME_SIZE_PCM_BUF];
UINT32 g_voisPcmOut[NB_DUMP_PCM_FRAMES*HAL_SPEECH_FRAME_SIZE_PCM_BUF/4]; // NbFrames * 320 bytes
UINT32 g_voisPcmIn[NB_DUMP_PCM_FRAMES*HAL_SPEECH_FRAME_SIZE_PCM_BUF/4]; // NbFrames * 320 bytes
UINT32 g_voisPcmCount = 0;
UINT32 g_voisPcmRxSwap = 1; // out PCM 
UINT32 g_voisPcmTxSwap = 0; // in PCM

#endif //VOIS_DUMP_PCM


// =============================================================================
//  FUNCTIONS
// =============================================================================

#ifdef SPEEH_RECORD_PCM_ENABLE

  UINT8 * g_pmallocHeadReceiver=NULL;
  UINT8 * g_pmallocPReceiver=NULL;
 
  UINT8 * g_pmallocHeadMic=NULL;
  UINT8 * g_pmallocPMic=NULL;

  UINT8 *PcmBufferReceiver=NULL;
  UINT8 *PcmBufferMic=NULL;

UINT32 sample_test [2]= 
{
    0//#include "speech02.tab"
};

UINT32 *g_testbutterP=NULL;

#endif


#define MULTS( x, y)  (INT64)(((INT64)x *(INT64)y)>>16)



#define ABS_AGC(X)  ((X<0)?-X:X)


static int AGC_Coeff_l = 0x7fff;
static int AGC_serror_l = 0;
static int	AGC_acc_error_l = 0;

VOLATILE BOOL g_AGCEnbleFlag = FALSE;
VOLATILE AUD_SPK_LEVEL_T g_SpkLevel = 0;
VOLATILE AUD_ITF_T g_AudInterface = 0;

VOLATILE INT16 g_MorphVoiceEnableFlag = 0;
VOLATILE INT16 g_MorphVoicePara = 0;

VOLATILE INT16 g_VoCRunFlag = 0;



//this value is what u want to control audio gain level;
//it should be set by user;
static int  AGC_MAXVAL = 30000; // 16384; // 32767;

static void AGainCtr_S(short *in,short *out,int nLen)
{

	int temp;
	int index;

	INT64 temp_xuml;

	//VOIS_TRACE(TSTDOUT, 0, "AGC");

	for(index=0;index<nLen;index++)
		{
			temp = ABS_AGC(in[index]);

			temp_xuml=MULTS(temp,AGC_Coeff_l);


			if (temp_xuml>32767)
			{
				temp_xuml=32767;
			}

			AGC_serror_l = temp_xuml - AGC_MAXVAL;
			if(AGC_serror_l <0)  
			{
				
				if (AGC_serror_l<-32767/2)
				{
					AGC_serror_l=-1;
				}
				else
				{
					AGC_serror_l =0;
				}
			}
			
			AGC_acc_error_l = (AGC_acc_error_l >>17) + (AGC_serror_l <<1); 



			AGC_Coeff_l = AGC_Coeff_l - AGC_acc_error_l ;

			if (AGC_Coeff_l> 0x7fff*32)
			{
				AGC_Coeff_l=0x7fff*32;
			}
			
			temp_xuml=MULTS(in[index] ,AGC_Coeff_l);

			if (temp_xuml<-32768)
			{
				temp_xuml=-32768;
			}

			if (temp_xuml>32767)
			{
				temp_xuml=32767;
			}


			out[index] =temp_xuml; //MULTS(in[index] ,AGC_Coeff_l);

		}

}

VOID vois_SetAGC( BOOL EnbleFlag)
{
	VOIS_TRACE(TSTDOUT, 0, "AGC EnbleFlag:%d",EnbleFlag);
	if(EnbleFlag==TRUE)
	{
		g_AGCEnbleFlag = TRUE;
	}
	else
	{
		g_AGCEnbleFlag = FALSE;
	}

	return;
	
}
	

// =============================================================================
// vois_Open
// -----------------------------------------------------------------------------
/// Open the VOIS service. Must be called before any other VOIS function.
/// This function registers the VoiS context in HAL map engine.
// =============================================================================
PUBLIC VOID vois_Open(VOID)
{
    //  Init the pointer used by HST to say talk with the Platform's
    //  calibration code.
    //  This will allow HST to read the calib buffer. 
    hal_MapEngineRegisterModule(HAL_MAP_ID_VOIS, &g_voisMapVersion, &g_voisCtx);
}



#ifdef VOIS_LOOP    
// =============================================================================
// hal_VoisVppLoop
// -----------------------------------------------------------------------------
/// Loop between Tx and Rx shortcutting the Speech Fifo.
// =============================================================================
PRIVATE VOID hal_VoisVppLoop(HAL_SPEECH_DEC_IN_T* popedRx,
                                  CONST HAL_SPEECH_ENC_OUT_T* pushedTx)
{
    UINT32 i;

    popedRx->dtxOn = 0;
    popedRx->codecMode = g_voisCtx.mode;
    popedRx->decFrameType = 0;
    popedRx->bfi = 0;
    popedRx->sid = 0;
    popedRx->taf = 0;
    popedRx->ufi = 0;

    for (i=0; i< g_voisCtx.bufferSize/4 ; i++)
    {
        if (g_voisUseSample)
        {
             *((UINT32*)popedRx->decInBuf + i) = sample[g_voisIndex++];
             if (g_voisIndex > 3999) // 4000 values
             {
                 g_voisIndex = 0;
             }
        }
        else
        {
            *((UINT32*)popedRx->decInBuf + i) = *((UINT32*)pushedTx->encOutBuf + i);
        }
    }
}
#endif // VOIS_LOOP



// =============================================================================
// vois_SetCalibratedVppCfg
// -----------------------------------------------------------------------------
/// Using a VOIS configuration structure, this function gets the calibrated data
/// from the calibration structure to configure properly a VPP config
/// structure.
/// @param itf Audio Interface used.
/// @param vppCfg VPP configuration structure to set with calibrated data.
// =============================================================================
PRIVATE VOID vois_SetCalibratedVppCfg(AUD_ITF_T itf,
                                      VPP_SPEECH_AUDIO_CFG_T* vppCfg)
{
    // FIXME : All calibration to be checked
    CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();

    // Set the configuration (in the global variables)
    // Get VPP config From Calib
    vppCfg->echoEsOn     = calibPtr->bb->audio[itf].vocEc.esOn;
    vppCfg->echoEsVad    = calibPtr->bb->audio[itf].vocEc.esVad;
    vppCfg->echoEsDtd    = calibPtr->bb->audio[itf].vocEc.esDtd;
    vppCfg->echoExpRel   = calibPtr->bb->audio[itf].vocEc.ecRel;
    vppCfg->echoExpMu    = ((calibPtr->bb->audio[itf].vocEc.ecMu == 0) || (g_voisCtx.voisAudioCfg.spkLevel < calibPtr->bb->audio[itf].vocEc.ecMu)) ? 0 : 1;
    vppCfg->echoExpMin   = calibPtr->bb->audio[itf].vocEc.ecMin;
    vppCfg->sdf          = (INT32*)calibPtr->bb->audio[itf].vocFilters.sdfFilter;
    vppCfg->mdf          = (INT32*)calibPtr->bb->audio[itf].vocFilters.mdfFilter;    
}

  UINT8 *g_AgcPcmBufferReceiver=NULL;

	

// =============================================================================
// vois_VppSpeechHandler
// -----------------------------------------------------------------------------
/// Handler for the vpp module, for speech-kind encoded streams. 
/// In charge of copying new datas, dealing with
/// underflow (no more samples), pause and resume etc ... It is called 
/// every time a trame has been decoded by VPP.
// =============================================================================
PRIVATE VOID vois_VppSpeechHandler(HAL_VOC_IRQ_STATUS_T* status)
{
	      // FIXME : All calibration to be checked
    CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
	
//   VOIS_TRACE(TSTDOUT, 0, "VoC");

   g_VoCRunFlag=0;
	
//    if (g_voisCtx.voisTramePlayedNb == VOIS_AUD_UNMUTE_TRAME_NB)
//    {
        // Unmute AUD
        //aud_Setup(g_voisCtx.voisItf, (AUD_LEVEL_T*)&g_voisCtx.voisAudioCfg);
//    }

    if (g_voisCtx.voisTramePlayedNb == VOIS_VPP_UNMUTE_TRAME_NB)
    {
        // Unmute VPP
        vpp_SpeechAudioCfg((VPP_SPEECH_AUDIO_CFG_T*)&g_voisCtx.voisVppCfg);
    }

    // FIXME: trick to only copy data after a encode finished interrupt
    {    
        // Copy the buffer to decode (Rx) and fetch from VPP's memory
        // the freshly encoded buffer (Tx).
#ifdef VOIS_LOOP
        hal_VoisVppLoop(g_voisCtx.voisDecInput, g_voisCtx.voisEncOutput);
#else
	

        hal_SpeechPopRxPushTx(g_voisCtx.voisDecInput, g_voisCtx.voisEncOutput);

	{
		HAL_SPEECH_DEC_IN_T* popedRx=(HAL_SPEECH_DEC_IN_T*)g_voisCtx.voisDecInput;
		//if(popedRx->decFrameType==1)
		//{
		//	popedRx->decFrameType=3;
		//}		

	}

	//int32 timebegin=hal_TimGetUpTime();

	//VOIS_TRACE(TSTDOUT, 0, " g_AGCEnbleFlag:%d,g_SpkLevel:%d,g_AudInterface:%d",g_AGCEnbleFlag,g_SpkLevel,g_AudInterface);

	if(g_AGCEnbleFlag==TRUE&&g_SpkLevel==AUD_SPK_VOL_7&&g_AudInterface==AUD_ITF_RECEIVER)
	{
		if(g_voisCtx.voisTramePlayedNb%2==0)
		{
			AGainCtr_S((short *)(g_AgcPcmBufferReceiver+320),(short *)(g_AgcPcmBufferReceiver+320),160);
		}
		else
		{
			AGainCtr_S((short *)(g_AgcPcmBufferReceiver),(short *)(g_AgcPcmBufferReceiver),160);
		}
	}

	//int32 timeend=hal_TimGetUpTime();
//	 VOIS_TRACE (TSTDOUT,0,"AGC time = %dus",((timeend-timebegin)*1000)/16);

#ifdef SPEEH_RECORD_PCM_ENABLE

	if(g_voisCtx.voisTramePlayedNb%2==0)
	{
		/*memcpy(PcmBufferReceiver+320,g_testbutterP,320);

		g_testbutterP=g_testbutterP+80;

		if(g_testbutterP>sample_test+(sizeof(sample_test)/4)-80)
		{
			g_testbutterP=sample_test;
		}*/

		if(g_pmallocPReceiver<g_pmallocHeadReceiver+500*1024-320)
		{

			/*
			{
				INT16 *SampleValue=(INT16* )(PcmBufferReceiver+320);
				INT32 i;
				INT16 g_arsMaxValue=16384;
		
			        for (i = 0 ; i<160; i++)
			        {
		        		if(SampleValue[i]>g_arsMaxValue)
		        		{
		        			SampleValue[i]=g_arsMaxValue;
		        		}
					else if(SampleValue[i]<-g_arsMaxValue)
					{
						SampleValue[i]=-g_arsMaxValue;
					}

			        }
			}*/
			
			memcpy(g_pmallocPReceiver,PcmBufferReceiver+320, 320);
			g_pmallocPReceiver=g_pmallocPReceiver+320;

			
			//memcpy(g_pmallocPReceiver,PcmBufferReceiver, 640); //for gallite
			//g_pmallocPReceiver=g_pmallocPReceiver+640;
		}
		else
		{
			VOIS_TRACE(TSTDOUT| TLEVEL (1), 0, "RE OVER!");
		}

		
		if(g_pmallocPMic<g_pmallocHeadMic+500*1024-320)
		{
			memcpy(g_pmallocPMic,PcmBufferMic+320, 320);
			g_pmallocPMic=g_pmallocPMic+320;
		}
		else
		{
			VOIS_TRACE(TSTDOUT| TLEVEL (1), 0, "ME OVER!");
		}


	}
	else
	{
	/*
		memcpy(PcmBufferReceiver,g_testbutterP,320);

		g_testbutterP=g_testbutterP+80;

		if(g_testbutterP>sample_test+(sizeof(sample_test)/4)-80)
		{
			g_testbutterP=sample_test;
		} */

		
		if(g_pmallocPReceiver<g_pmallocHeadReceiver+500*1024-320)
		{
/*
			{
				INT16 *SampleValue=(INT16* )PcmBufferReceiver;
				INT32 i;
				INT16 g_arsMaxValue=16384;
		
			        for (i = 0 ; i<160; i++)
			        {
		        		if(SampleValue[i]>g_arsMaxValue)
		        		{
		        			SampleValue[i]=g_arsMaxValue;
		        		}
					else if(SampleValue[i]<-g_arsMaxValue)
					{
						SampleValue[i]=-g_arsMaxValue;				
					}

			        }
			}*/
			
			memcpy(g_pmallocPReceiver,PcmBufferReceiver, 320);
			g_pmallocPReceiver=g_pmallocPReceiver+320;
						
			//memcpy(g_pmallocPReceiver,PcmBufferReceiver, 640); //for gallite
			//g_pmallocPReceiver=g_pmallocPReceiver+640;
		}
		else
		{
			VOIS_TRACE(TSTDOUT| TLEVEL (1), 0, "RH OVER!");
		}

		
		if(g_pmallocPMic<g_pmallocHeadMic+500*1024-320)
		{
			memcpy(g_pmallocPMic,PcmBufferMic, 320);
			g_pmallocPMic=g_pmallocPMic+320;
		}
		else
		{
			VOIS_TRACE(TSTDOUT| TLEVEL (1), 0, "MH OVER!");
		}


		
	}


#endif


		vpp_SpeechSetAecEnableFlag(calibPtr->bb->audio[g_AudInterface].audioParams.AecEnbleFlag,
   						calibPtr->bb->audio[g_AudInterface].audioParams.AgcEnbleFlag);		


		vpp_SpeechSetAecPara(calibPtr->bb->audio[g_AudInterface].audioParams.NoiseGainLimit,
   						calibPtr->bb->audio[g_AudInterface].audioParams.NoiseMin,
   						calibPtr->bb->audio[g_AudInterface].audioParams.NoiseGainLimitStep,
   						calibPtr->bb->audio[g_AudInterface].audioParams.AmpThr);	


		vpp_SpeechSetFilterPara(calibPtr->bb->audio[g_AudInterface].audioParams.HighPassFilterFlag,
   						calibPtr->bb->audio[g_AudInterface].audioParams.NotchFilterFlag,
   						calibPtr->bb->audio[g_AudInterface].audioParams.NoiseSuppresserFlag,
   						calibPtr->bb->audio[g_AudInterface].audioParams.NoiseSuppresserWithoutSpeechFlag);	


/*

	if(g_SpkLevel%2!=0)
	{
		vpp_SpeechSetAecEnableFlag(TRUE,FALSE,FALSE,FALSE);
	}
	else
	{
		vpp_SpeechSetAecEnableFlag(FALSE,FALSE,FALSE,FALSE);
	}
  
	
*/

#ifdef VOIS_DUMP_PCM
        // swap for the next turn
        g_voisPcmRxSwap = 1 - g_voisPcmRxSwap;
        g_voisPcmTxSwap = 1 - g_voisPcmTxSwap;

        if ((g_voisPcmCount < NB_DUMP_PCM_FRAMES)&&
            (g_voisCtx.voisTramePlayedNb > VOIS_VPP_UNMUTE_TRAME_NB)&&
            (g_voisCtx.voisItf==AUD_ITF_LOUD_SPEAKER))
        {
                UINT32 i;

                // get the current pcm pointers
                UINT32 *pRx = ((UINT32*)vpp_SpeechGetRxPcmBuffer()) + g_voisPcmRxSwap*HAL_SPEECH_FRAME_SIZE_PCM_BUF/4;
                UINT32 *pTx = (UINT32*) HAL_SYS_GET_UNCACHED_ADDR(((UINT32*)g_voisPcmTxBuffer) + g_voisPcmTxSwap*HAL_SPEECH_FRAME_SIZE_PCM_BUF/4);

                
                // copy data
                for (i = g_voisPcmCount*HAL_SPEECH_FRAME_SIZE_PCM_BUF/4; i < (g_voisPcmCount+1)*HAL_SPEECH_FRAME_SIZE_PCM_BUF/4; i++)
                {
                        g_voisPcmOut[i] = *pRx++;
                        g_voisPcmIn[i] = *pTx++;
                }

                // uncrement counter
                g_voisPcmCount++;
        }
                
#endif

        
#endif

        g_voisCtx.voisTramePlayedNb++;
    }
}






// =============================================================================
// vois_Setup
// -----------------------------------------------------------------------------
/// Configure the vois service.. 
/// 
/// This functions configures the audio interface for the speech stream:
/// gain for the side tone and the speaker, input selection for the microphone
/// and output selection for the speaker... It also configures the decoding
/// parameters like audio cancellation. (cf #VOIS_AUDIO_CFG_T for exhaustive
/// details).
/// 
/// DO NOT CALL THAT FUNCTION BEFORE #vois_start.
/// 
/// @param itf Interface number of the interface to setup.
/// @param cfg The configuration set applied to the audio interface. See 
/// #VOIS_AUDIO_CFG_T for more details.
/// @return #VOIS_ERR_NO it can execute the configuration.
// =============================================================================

PUBLIC VOIS_ERR_T vois_Setup(AUD_ITF_T itf, CONST VOIS_AUDIO_CFG_T* cfg)
{
    AUD_ERR_T audErr = AUD_ERR_NO;
    AUD_LEVEL_T audioCfg;
    VPP_SPEECH_AUDIO_CFG_T vppCfg;

   	

      // FIXME : All calibration to be checked
    CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();

    VOIS_TRACE(TSTDOUT, 0, "vois_Setup");

    // Check that VOIS is running
    if (g_voisCtx.voisItf == AUD_ITF_NONE || !g_voisCtx.voisStarted)
    {
        // Ignore this call
        return VOIS_ERR_NO;
    }

#ifndef VOC_INTR_STEP
    UINT32 Wakeup_Mask_Status =  hal_VocIntrClose();
    if(Wakeup_Mask_Status==0xffffffff)
        return VOIS_ERR_NO;
#else
#ifdef GALLITE_IS_CT1129
    UINT32 now,voc_time;
    now = hal_TimGetUpTime();

   while(! g_VoCRunFlag)
   {
        voc_time = hal_TimGetUpTime();
         if (voc_time - now >(16384>>2)) 
         {
            g_VoCRunFlag=1;
         }
   }
   now = hal_TimGetUpTime();

   while( g_VoCRunFlag==1&&g_voisCtx.voisStarted==TRUE)
   {
   	// VOIS_TRACE(TSTDOUT, 0, "vois_Setup waiting");   
        voc_time = hal_TimGetUpTime();
         if (voc_time - now > 16384) 
        {
            g_VoCRunFlag=0;
            hal_HstSendEvent(0xaaaa9999);
         }
   }
#else
    while (g_VoCRunFlag==0);
    while (g_VoCRunFlag==1 && g_voisCtx.voisStarted==TRUE)
    {
        // VOIS_TRACE(TSTDOUT, 0, "vois_Setup waiting");   
    }
#endif
#endif
    g_SpkLevel=cfg->spkLevel;
	
    g_AudInterface=itf;

    // Record the last applied configuration
    *(VOIS_MAP_AUDIO_CFG_T*)&g_voisCtx.voisLatestCfg =*(VOIS_MAP_AUDIO_CFG_T*)cfg;
    
    // Those parameters are not calibration dependent.
    g_voisCtx.voisVppCfg.encMute = cfg->encMute;
    g_voisCtx.voisVppCfg.decMute = cfg->decMute;

    // disable push-to-talk
    g_voisCtx.voisVppCfg.if1 = 0; 

    // Set audio interface 
    g_voisCtx.voisAudioCfg.spkLevel = cfg->spkLevel;
    g_voisCtx.voisAudioCfg.micLevel = cfg->micLevel;
    g_voisCtx.voisAudioCfg.sideLevel = cfg->sideLevel;
    g_voisCtx.voisAudioCfg.toneLevel = cfg->toneLevel;
   
    // get calibration params for Vpp Speech. Need to be called after setting g_voisCtx 
    vois_SetCalibratedVppCfg(itf, (VPP_SPEECH_AUDIO_CFG_T*)&g_voisCtx.voisVppCfg);

    // If the interface is different, we stop and resume the streams
    // to apply the interface change.
    if (itf != g_voisCtx.voisItf)
    {
        // Stop the stream.
        audErr = aud_StreamStop(g_voisCtx.voisItf);
        g_voisCtx.voisItf = itf;
		

        // Set the DAC to use 
        g_voisPcmRxStream.voiceQuality = 
#ifdef VOIS_USE_STEREO_DAC_FOR_LOUDSPEAKER
                        // use stereo DAC if the mode is loudspeaker 
                        (itf == AUD_ITF_LOUD_SPEAKER) ? FALSE :
#endif
#ifdef VOIS_USE_STEREO_DAC_FOR_EARPIECE
                        // use stereo DAC if the mode is earpiece 
                        (itf == AUD_ITF_EAR_PIECE) ?   FALSE :
#endif
                        // default 
                        TRUE;

        // Enforce temporary muting and progressive level resume by the 
        // speech handler.
        g_voisCtx.voisTramePlayedNb = 0;
        
#ifdef VOIS_DUMP_PCM
        g_voisPcmCount = 0;
#endif    
        // Set audio interface, initially muted.
        *(AUD_LEVEL_T*)&audioCfg = *(AUD_LEVEL_T*)&g_voisCtx.voisAudioCfg;
        audioCfg.spkLevel   = cfg->spkLevel;
        audioCfg.micLevel   = cfg->micLevel;
        audioCfg.toneLevel  = cfg->sideLevel;
        audioCfg.sideLevel  = cfg->toneLevel;


        // Set the VPP config, initially muted.
        *(VPP_SPEECH_AUDIO_CFG_T*)&vppCfg = *(VPP_SPEECH_AUDIO_CFG_T*)&g_voisCtx.voisVppCfg;
        vppCfg.encMute = VPP_SPEECH_MUTE;
        vppCfg.decMute = VPP_SPEECH_MUTE;
	
        vpp_SpeechAudioCfg(&vppCfg);

	vpp_SpeechReset();


        if (audErr == AUD_ERR_NO)
        {
            // configure AUD and VPP buffers
            audErr = aud_StreamStart(itf, &g_voisPcmRxStream, &audioCfg);
        }
        
        if (audErr == AUD_ERR_NO)
        {
            // Start the other flux only if the Rx one is started
            audErr = aud_StreamRecord(itf, &g_voisPcmTxStream, &audioCfg);
        }
    }
    else
    {
        // Just apply the configuration change on the same
        // interface.
        vpp_SpeechAudioCfg((VPP_SPEECH_AUDIO_CFG_T*)&g_voisCtx.voisVppCfg);
        audErr = aud_Setup(itf, (AUD_LEVEL_T*)&g_voisCtx.voisAudioCfg);
    }
	//Set Mic Gain in VoC

    vpp_SpeechSetMicDigitalGain(calibPtr->bb->audio[itf].audioParams.MicDigitalGainInVoC);


   vpp_SpeechSetAecEnableFlag(calibPtr->bb->audio[itf].audioParams.AecEnbleFlag,
   						calibPtr->bb->audio[itf].audioParams.AgcEnbleFlag);	
   
   vpp_SpeechSetAecPara(calibPtr->bb->audio[itf].audioParams.NoiseGainLimit,
   						calibPtr->bb->audio[itf].audioParams.NoiseMin,
   						calibPtr->bb->audio[itf].audioParams.NoiseGainLimitStep,
   						calibPtr->bb->audio[itf].audioParams.AmpThr);	

    vpp_SpeechSetFilterPara(calibPtr->bb->audio[itf].audioParams.HighPassFilterFlag,
   						calibPtr->bb->audio[itf].audioParams.NotchFilterFlag,
   						calibPtr->bb->audio[itf].audioParams.NoiseSuppresserFlag,
   						calibPtr->bb->audio[itf].audioParams.NoiseSuppresserWithoutSpeechFlag);	



    vpp_SpeechSetReceiverDigitalGain((calibPtr->bb->audio[itf].audioGains.outGain[cfg->spkLevel]>>16)&0x3);

    VOIS_TRACE(TSTDOUT, 0, "SETUP Gain=0x%x", calibPtr->bb->audio[itf].audioGains.outGain[cfg->spkLevel]);

//    VOIS_TRACE(TSTDOUT, 0, "g_VoCRunFlag=%d", g_VoCRunFlag);

#ifndef VOC_INTR_STEP    
    hal_VocIntrOpen(Wakeup_Mask_Status);
#endif

    switch (audErr)
    {
        // TODO Add error as they come

        case AUD_ERR_NO:

            return VOIS_ERR_NO;
            break;

        default:

            return audErr; // ...unknown error ...
            break;
    }
}





VOID txhandle_half(VOID)
{
	/*	memcpy(PcmBufferMic,g_testbutterP,320);

		g_testbutterP=g_testbutterP+80;

		if(g_testbutterP>sample_test+(sizeof(sample_test)/4)-80)
		{
			g_testbutterP=sample_test;
		} */

//	vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC);
//	vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_ENC);
	g_VoCRunFlag=1;
	return;
}

VOID txhandle_end(VOID)
{
/*		memcpy(PcmBufferMic+320,g_testbutterP,320);

		g_testbutterP=g_testbutterP+80;

		if(g_testbutterP>sample_test+(sizeof(sample_test)/4)-80)
		{
			g_testbutterP=sample_test;
		}*/

//	vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC);
//	vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_ENC);
	g_VoCRunFlag=1;
	return;
}



// =============================================================================
// vois_Start
// -----------------------------------------------------------------------------
/// Start the VOIS service. 
/// 
/// This function records from the mic and outputs sound on the speaker
/// on the audio interface specified as a parameter, using the input and output
/// set by the \c parameter. \n
///
/// @param itf Interface providing the audion input and output.
/// @param cfg The configuration set applied to the audio interface
/// @return #VOIS_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command, or if VPP is unavailable.
///         #VOIS_ERR_NO it can execute the command.
// =============================================================================
PUBLIC VOIS_ERR_T vois_Start(
                        CONST AUD_ITF_T      itf,
                        CONST VOIS_AUDIO_CFG_T* cfg)
{
    VOIS_PROFILE_FUNCTION_ENTRY(vois_Start);
    VOIS_TRACE(_PAL| TLEVEL (1), 0, "VOIS Start");

    // Initial audio confguration
    AUD_LEVEL_T audioCfg;
    VPP_SPEECH_AUDIO_CFG_T vppCfg;
    AUD_ERR_T audErr;

    CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();

  //  g_AGCEnbleFlag = FALSE;
   g_SpkLevel=cfg->spkLevel;
       g_AudInterface=itf;


    // Record the last applied configuration
    *(VOIS_MAP_AUDIO_CFG_T*)&g_voisCtx.voisLatestCfg =*(VOIS_MAP_AUDIO_CFG_T*)cfg;
    
    // Configure the Speech FIFO
    hal_SpeechFifoReset();
    
    g_voisCtx.voisVppCfg.encMute = cfg->encMute;
    g_voisCtx.voisVppCfg.decMute = cfg->decMute;
    
    // disable push-to-talk
    g_voisCtx.voisVppCfg.if1 = 0; 

    // Set audio interface 
    g_voisCtx.voisAudioCfg.spkLevel = cfg->spkLevel;
    g_voisCtx.voisAudioCfg.micLevel = cfg->micLevel;
    g_voisCtx.voisAudioCfg.sideLevel = cfg->sideLevel;
    g_voisCtx.voisAudioCfg.toneLevel = cfg->toneLevel;
    
    // get calibration params for Vpp Speech. Need to be called after setting g_voisCtx 
    vois_SetCalibratedVppCfg(itf, (VPP_SPEECH_AUDIO_CFG_T*)&g_voisCtx.voisVppCfg);
    // Register global var
    g_voisCtx.voisItf = itf;

    g_voisCtx.voisTramePlayedNb = 0;
    
    // Set audio interface, initially muted
    *(AUD_LEVEL_T*)&audioCfg = *(AUD_LEVEL_T*)&g_voisCtx.voisAudioCfg;
    audioCfg.spkLevel   =  cfg->spkLevel;
    audioCfg.micLevel   =  cfg->micLevel;
    audioCfg.toneLevel  =  cfg->sideLevel;
    audioCfg.sideLevel  =  cfg->toneLevel;

    // Set the VPP config
    *(VPP_SPEECH_AUDIO_CFG_T*)&vppCfg = *(VPP_SPEECH_AUDIO_CFG_T*)&g_voisCtx.voisVppCfg;
    vppCfg.encMute = VPP_SPEECH_MUTE;
    vppCfg.decMute = VPP_SPEECH_MUTE;

	 g_AgcPcmBufferReceiver=(UINT8 *)((UINT32)( vpp_SpeechGetRxPcmBuffer())|0x20000000);//(UINT8 *)vpp_SpeechGetRxPcmBuffer();

	 

#ifdef SPEEH_RECORD_PCM_ENABLE

	  g_testbutterP=sample_test;


	g_pmallocPReceiver=g_pmallocHeadReceiver=(UINT8 *)COS_SHMEMMALLOC(500*1024);

	 if(g_pmallocHeadReceiver == NULL)
	{
		VOIS_TRACE (TSTDOUT,0,"[vois_Start]*****g_pmallocHeadReceiver*******no memory to malloc!!!  ************\n");
		return VOIS_ERR_RESOURCE_BUSY;
	}
	 else
	 {
	 	VOIS_TRACE(TSTDOUT, 0, "g_pmallocHeadReceiver  = %X", g_pmallocHeadReceiver);
        memset(g_pmallocHeadReceiver,1,500*1024);
		
	 }


	g_pmallocPMic=g_pmallocHeadMic=(UINT8 *)COS_SHMEMMALLOC(500*1024);

	 if(g_pmallocHeadMic == NULL)
	{
        COS_SHMEMFREE(g_pmallocHeadReceiver);
		VOIS_TRACE (TSTDOUT,0,"[vois_Start]******g_pmallocHeadMic******no memory to malloc!!!  ************\n");
		return VOIS_ERR_RESOURCE_BUSY;
	}
	 else
	 {
	 	VOIS_TRACE(TSTDOUT, 0, "g_pmallocHeadMic  = %X", g_pmallocHeadMic);
	    memset(g_pmallocHeadMic,1,500*1024);
	 }

	// PcmBufferReceiver=(UINT8 *)((UINT32)(vpp_SpeechGetRxPcmBuffer())|0x20000000);//(UINT8 *)vpp_SpeechGetRxPcmBuffer();


	 PcmBufferReceiver=(UINT8 *)((UINT32)( vpp_SpeechGetRxPcmBuffer())|0x20000000);
	 PcmBufferMic=(UINT8 *)((UINT32)(vpp_SpeechGetTxPcmBuffer())|0x20000000);//(UINT8 *)vpp_SpeechGetTxPcmBuffer();


#endif


    
    // Typical global vars
    // Those buffer are written by VoC, thus the need to access
    // them the uncached way.
    g_voisCtx.voisEncOutput = (HAL_SPEECH_ENC_OUT_T*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)vpp_SpeechGetTxCodBuffer());
    g_voisCtx.voisDecInput  = (HAL_SPEECH_DEC_IN_T*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)vpp_SpeechGetRxCodBuffer());

    // PCM Audio Tx stream, output of VPP
    g_voisPcmRxStream.startAddress = (UINT32*)vpp_SpeechGetRxPcmBuffer();
    g_voisPcmRxStream.length = sizeof(HAL_SPEECH_PCM_BUF_T);
    g_voisPcmRxStream.sampleRate = HAL_AIF_FREQ_8000HZ;
    g_voisPcmRxStream.channelNb = HAL_AIF_MONO;
    g_voisPcmRxStream.voiceQuality = TRUE;
    g_voisPcmRxStream.halfHandler =  NULL ; // Mechanical interaction with VPP's VOC
    g_voisPcmRxStream.endHandler =  NULL ; // Mechanical interaction with VPP's VOC

    // PCM Audio Rx stream, input of VPP
#ifdef VOIS_DUMP_PCM
    g_voisPcmTxStream.startAddress = (UINT32*)(g_voisPcmTxBuffer);
#else
    g_voisPcmTxStream.startAddress = (UINT32*)vpp_SpeechGetTxPcmBuffer();
#endif
    g_voisPcmTxStream.length = sizeof(HAL_SPEECH_PCM_BUF_T);
    g_voisPcmTxStream.sampleRate = HAL_AIF_FREQ_8000HZ;
    g_voisPcmTxStream.channelNb = HAL_AIF_MONO;
    g_voisPcmTxStream.voiceQuality = TRUE;
    g_voisPcmTxStream.halfHandler =  txhandle_half ; // Mechanical interaction with VPP's VOC
    g_voisPcmTxStream.endHandler =  txhandle_end ; // Mechanical interaction with VPP's VOC_AHB_BCPU_DEBUG_IRQ_CAUSE


    // set vpp
    UINT32 spStatus =    vpp_SpeechOpen(vois_VppSpeechHandler, ( VPP_SPEECH_WAKEUP_HW_DECENC ));

    VOIS_TRACE(TSTDOUT, 0, "VPP_SPEECH_OPEN STATUS = %08X", spStatus);
    if (spStatus == HAL_ERR_RESOURCE_BUSY)
    {
#ifdef SPEEH_RECORD_PCM_ENABLE
        COS_SHMEMFREE(g_pmallocHeadReceiver);
        COS_SHMEMFREE(g_pmallocHeadMic);
#endif
        return VOIS_ERR_RESOURCE_BUSY;
    }
    vpp_SpeechAudioCfg(&vppCfg);

    memset(g_voisPcmRxStream.startAddress,0,g_voisPcmRxStream.length);


    vpp_SpeechSetMicDigitalGain(calibPtr->bb->audio[itf].audioParams.MicDigitalGainInVoC);

   vpp_SpeechSetAecEnableFlag(calibPtr->bb->audio[itf].audioParams.AecEnbleFlag,
   						calibPtr->bb->audio[itf].audioParams.AgcEnbleFlag);	

   vpp_SpeechSetAecPara(calibPtr->bb->audio[itf].audioParams.NoiseGainLimit,
   						calibPtr->bb->audio[itf].audioParams.NoiseMin,
   						calibPtr->bb->audio[itf].audioParams.NoiseGainLimitStep,
   						calibPtr->bb->audio[itf].audioParams.AmpThr);	

    vpp_SpeechSetFilterPara(calibPtr->bb->audio[itf].audioParams.HighPassFilterFlag,
   						calibPtr->bb->audio[itf].audioParams.NotchFilterFlag,
   						calibPtr->bb->audio[itf].audioParams.NoiseSuppresserFlag,
   						calibPtr->bb->audio[itf].audioParams.NoiseSuppresserWithoutSpeechFlag);	
   

   vpp_SpeechSetReceiverDigitalGain((calibPtr->bb->audio[itf].audioGains.outGain[cfg->spkLevel]>>16)&0x3);

   vpp_SpeechSetMorphVoice(g_MorphVoiceEnableFlag,g_MorphVoicePara);


   VOIS_TRACE(TSTDOUT, 0, "Gain=0x%x", calibPtr->bb->audio[itf].audioGains.outGain[cfg->spkLevel]);

	   
    // configure AUD and VPP buffers
    audErr = aud_StreamStart(itf, &g_voisPcmRxStream, &audioCfg);
    
    if (audErr == AUD_ERR_NO)
    {
        // Start the other flux only if the Rx one is started
        audErr = aud_StreamRecord(itf, &g_voisPcmTxStream, &audioCfg);
    }

    switch (audErr)
    {
        case AUD_ERR_NO:
            break; 

        default:
            vpp_SpeechClose();
#ifdef SPEEH_RECORD_PCM_ENABLE
            COS_SHMEMFREE(g_pmallocHeadReceiver);
            COS_SHMEMFREE(g_pmallocHeadMic);
#endif
            VOIS_PROFILE_FUNCTION_EXIT(vois_Start);
            VOIS_TRACE(TSTDOUT, 0, "VOIS Start Failed !!!");
            return VOIS_ERR_UNKNOWN;
            break; // :)
    }

    VOIS_PROFILE_FUNCTION_EXIT(vois_Start);
    g_voisCtx.voisStarted = TRUE;
    return VOIS_ERR_NO;
}




#ifdef SPEEH_RECORD_PCM_ENABLE
extern UINT32 ML_LocalLanguage2UnicodeBigEnding(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset[12]);
BOOL CSW_Free (  PVOID pMemBlock);
#endif

// =============================================================================
// vois_Stop
// -----------------------------------------------------------------------------
/// This function stops the VOIS service.
/// If the function returns 
/// #VOIS_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #VOIS_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #VOIS_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC VOIS_ERR_T vois_Stop(VOID)
{
    VOIS_PROFILE_FUNCTION_ENTRY(vois_Stop);
    VOIS_TRACE(_PAL| TLEVEL (1), 0, "VOIS Stop");
	
	
    if (g_voisCtx.voisStarted)
    {
        // stop stream ...
        aud_StreamStop(g_voisCtx.voisItf);
        vpp_SpeechClose();
	g_VoCRunFlag=0;



#ifdef SPEEH_RECORD_PCM_ENABLE

	{
		 UINT8 SpkFileName[50] = "/t/speaker.pcm";		
		 UINT8 MicFileName[50] = "/t/mic.pcm";

		 UINT8 *Spkuniname=NULL;
		 UINT8 *Micuniname=NULL;
		 UINT32 Spknamelen=0;
 		 UINT32 Micnamelen=0;
		 
		 ML_LocalLanguage2UnicodeBigEnding(SpkFileName, strlen(SpkFileName), &Spkuniname, &Spknamelen, NULL);
 		 ML_LocalLanguage2UnicodeBigEnding(MicFileName, strlen(MicFileName), &Micuniname, &Micnamelen, NULL);
		 
		 INT32 SpkFileHandle=FS_Open(Spkuniname, FS_O_RDWR | FS_O_CREAT | FS_O_TRUNC, 0);
		 INT32 MicFileHandle=FS_Open(Micuniname, FS_O_RDWR | FS_O_CREAT | FS_O_TRUNC, 0);

		 CSW_Free(Spkuniname);
		 CSW_Free(Micuniname);


		 VOIS_TRACE (TSTDOUT,0,"[vois_Stop]Spk File Handle:0x%x",SpkFileHandle);
		 VOIS_TRACE (TSTDOUT,0,"[vois_Stop]Mic File Handle:0x%x",MicFileHandle);
		 
		INT32 Spkresult=FS_Write(SpkFileHandle,g_pmallocHeadReceiver,450*1024);
		INT32 Micresult=FS_Write(MicFileHandle,g_pmallocHeadMic+(320)*2,450*1024);

		 if(Spkresult != 450*1024)
		{
			VOIS_TRACE (TSTDOUT,0,"[vois_Stop_ERROR]spk write file error!\n");
		}
		 if(Micresult != 450*1024)
		{
			VOIS_TRACE (TSTDOUT,0,"[vois_Stop_ERROR]mic write file error!\n");
		}
		 FS_Close(SpkFileHandle);
 		 FS_Close(MicFileHandle);
		 COS_SHMEMFREE(g_pmallocHeadReceiver);
 		 COS_SHMEMFREE(g_pmallocHeadMic);
		
	}



#endif

        // and reset global state and cl
        g_voisCtx.voisItf = AUD_ITF_NONE;

        g_voisCtx.voisTramePlayedNb = 0;

        g_voisCtx.voisAudioCfg.spkLevel = AUD_SPK_MUTE;
        g_voisCtx.voisAudioCfg.micLevel = AUD_MIC_MUTE;
        g_voisCtx.voisAudioCfg.sideLevel = 0;
        g_voisCtx.voisAudioCfg.toneLevel = 0;

        g_voisCtx.voisVppCfg.echoEsOn = 0;
        g_voisCtx.voisVppCfg.echoEsVad = 0;
        g_voisCtx.voisVppCfg.echoEsDtd = 0;
        g_voisCtx.voisVppCfg.echoExpRel = 0;
        g_voisCtx.voisVppCfg.echoExpMu = 0;
        g_voisCtx.voisVppCfg.echoExpMin = 0;
        g_voisCtx.voisVppCfg.encMute = VPP_SPEECH_MUTE;
        g_voisCtx.voisVppCfg.decMute = VPP_SPEECH_MUTE;
        g_voisCtx.voisVppCfg.sdf = NULL;
        g_voisCtx.voisVppCfg.mdf = NULL;
        g_voisCtx.voisVppCfg.if1 = 0;

        hal_SpeechFifoReset();
        g_voisCtx.voisStarted = FALSE;

        // PCM Audio Tx stream, output of VPP
        g_voisPcmRxStream.startAddress = NULL;
        g_voisPcmRxStream.length = 0;
        g_voisPcmRxStream.sampleRate = HAL_AIF_FREQ_8000HZ;
        g_voisPcmRxStream.channelNb = HAL_AIF_MONO;
        g_voisPcmRxStream.voiceQuality = TRUE;
        g_voisPcmRxStream.halfHandler = NULL; // Mechanical interaction with VPP's VOC
        g_voisPcmRxStream.endHandler = NULL; // Mechanical interaction with VPP's VOC


        // PCM Audio Rx stream, input of VPP
        g_voisPcmTxStream.startAddress = NULL;
        g_voisPcmTxStream.length = 0;
        g_voisPcmTxStream.sampleRate = HAL_AIF_FREQ_8000HZ;
        g_voisPcmTxStream.channelNb = HAL_AIF_MONO;
        g_voisPcmTxStream.voiceQuality = TRUE;
        g_voisPcmTxStream.halfHandler = NULL; // Mechanical interaction with VPP's VOC
        g_voisPcmTxStream.endHandler = NULL; // Mechanical interaction with VPP's VOC
    }


     VOIS_PROFILE_FUNCTION_EXIT(vois_Stop);
    return VOIS_ERR_NO;
}



// =============================================================================
// vois_CalibUpdateValues
// -----------------------------------------------------------------------------
/// Update Vois related values depending on calibration parameters.
/// 
/// @return #VOIS_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #VOIS_ERR_NO otherwise
// =============================================================================
PUBLIC VOIS_ERR_T vois_CalibUpdateValues(VOID)
{
    // Only relevant would VoiS be started
    if (g_voisCtx.voisStarted)
    {
        return vois_Setup(g_voisCtx.voisItf, (VOIS_AUDIO_CFG_T*)&g_voisCtx.voisLatestCfg);
        // As vois_Setup does a aud_Setup, all settings 
        // will be updated from the possibly new calibration
        // values.
    }
    else
    {
        return VOIS_ERR_NO;
    }
}

typedef enum
{
	MAGIC_SOUND_OFF=0,
	MAGIC_SOUND_MAN_TO_WOMAN,
	MAGIC_SOUND_WOMAN_TO_MAN,
}MAGIC_SOUND_STATE;



INT32 vois_Set_MorphVoice( MAGIC_SOUND_STATE Value)
{
	VOIS_TRACE(TSTDOUT, 0, "MAGIC_SOUND_STATE=%d", Value);

	switch(Value)
	{
		case MAGIC_SOUND_OFF:
			g_MorphVoiceEnableFlag=FALSE;
			g_MorphVoicePara=0x4000;			

			break;
			
		case MAGIC_SOUND_MAN_TO_WOMAN:
			g_MorphVoiceEnableFlag=TRUE;
			g_MorphVoicePara=0x4800;

			break;
			
		case MAGIC_SOUND_WOMAN_TO_MAN:
			g_MorphVoiceEnableFlag=TRUE;
			g_MorphVoicePara=0x3200;
			
			break;
		default:
			g_MorphVoiceEnableFlag=FALSE;
			g_MorphVoicePara=0x4000;		
			VOIS_TRACE(TSTDOUT, 0, "MAGIC SOUND ERROR STATE");
			
			break;		
	}

	return 0;
	
}


