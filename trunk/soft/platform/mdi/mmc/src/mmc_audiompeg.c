
////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2007, Coolsand Technologies, Inc.
//                       All Rights Reserved
//
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: mmc_audiompeg.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   xuml
//
////////////////////////////////////////////////////////////////////////////////

#include "cswtype.h"
#include "mcip_debug.h"
#include "fs.h"
#include "mmc_audiompeg.h"
#include "hal_error.h"



#include "cpu_share.h"
#include "assert.h"
#include "hal_voc.h"

#include "global.h"
#include "audio_api.h"

#include "aud_m.h"

#include "fs_asyn.h"

#include "mci.h"
#include "cos.h"

#include "sxs_type.h"
#include "sxr_sbx.h"
#include "sxr_mem.h"

#include "string.h"
#include "hal_overlay.h"

#include "sxr_tls.h"

#include "resample.h"


#define AudioJpeg_INPUT_BUFSIZE		(80*1024)//20k byte
//#define MinFileLen                  (8*1024)
#define  MIN_INPUT_REMAIN_SIZE   (2*1024)
#ifdef PCM_OUT_24BIT_PER_SAMPLE
#define AUDIO_MAX_OUTPUT_BUFSIZE (1152*4)
#define AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE (1152*2*4*23) 


#define AACDEC_OUTPUT_BUFSIZE (1024*4)
#define AACDEC_PCM_OUTPUT_BUFFER_SIZE (1024*2*4*22) 

#define MP3DEC_OUTPUT_BUFSIZE (1152*4)
#define MP3DEC_PCM_OUTPUT_BUFFER_SIZE (1152*2*4*22) 
#else//16bit depth
#define AUDIO_MAX_OUTPUT_BUFSIZE (1152*4)
//#define AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE (1152*4*23) 
#define AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE (1152*4*4) 


#define AACDEC_OUTPUT_BUFSIZE (1024*4)
#define AACDEC_PCM_OUTPUT_BUFFER_SIZE (1024*4*4) 

#define MP3DEC_OUTPUT_BUFSIZE (1152*4)
#define MP3DEC_PCM_OUTPUT_BUFFER_SIZE (1152*4*4) 
#endif


   extern AUD_LEVEL_T audio_cfg;
   extern HAL_AIF_STREAM_T audioStream;
   extern AUD_ITF_T audioItf;
   
MPEG_PLAY MpegPlayer;

volatile INT32 Mmc_Audio_Pcm_Half;
volatile INT32 Mmc_Audio_FrameCount;
 volatile UINT32 Mmc_Audio_PcmDataCopyCount;
volatile UINT32 ErrorFrameCount=0;
volatile UINT32 ContinuePlayFlag=0;//this flag controls to continue play the left data in the pcm buffer
volatile UINT32 EndErrorFlag=0;


volatile UINT32 buffer_flag=0;
volatile UINT32 g_EndofFileFlag=0;
volatile UINT32 g_ReadFileFlag=0;
volatile UINT32 g_ReadFileSize=0;
volatile UINT32 g_MemMoveFlag=0;
volatile UINT32 g_ReadFileError=0;


volatile INT32 g_ReadFileCounter=0;

extern UINT32 pcmbuf_overlay[4608];

volatile INT32 g_ConsumedLength=0;
volatile UINT32 g_AudioMode=0;
volatile UINT32 g_AudioSeekFlag=0;

volatile INT32 g_MPEGFILESIZE=0;



static UINT16 g_Nout;
static UINT32 g_counterframe;

static INT16 g_NumOfOutSample=0;

static INT16 g_TotalFrame=0;


static UINT8 g_ResampleInitFlag=0;



static INT16 *g_BT8kPcmBuffer=NULL;


static  INT16  *g_BTDataAddress=0;




//#define MP3_PLAYER_AGC

#ifdef MP3_PLAYER_AGC

#define MULTS( x, y)  (INT64)(((INT64)x *(INT64)y)>>16)



#define ABS_AGC(X)  ((X<0)?-X:X)


static int AGC_Coeff_l = 0x7fff*2*8;
static int AGC_serror_l = 0;
static int	AGC_acc_error_l = 0;


static  short  *g_AgcDataAddress=0;
static  int g_AgcDataLength=0;


//this value is what u want to control audio gain level;
//it should be set by user;
static int  AGC_MAXVAL = 30000; // 16384; // 32767;


static void AGainCtr_S(short *in,short *out,int nLen)
{

	int temp;
	int index;

	INT64 temp_xuml;

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


#endif




//HANDLE g_FileHandle;
/*
* NAME:	MMC_AUDIODecClose()
* DESCRIPTION: Close aac decoder.
*/
//extern uint32 *DRV_PCMOutStop(void);
MCI_ERR_T MMC_AUDIODecClose (MPEG_PLAY *pAudioPlayer)
{

	MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MCI_MP3AAC]MMC_AUDIODecClose!");
	aud_StreamStop(audioItf);
	vpp_AudioJpegDecClose();
	while(g_ReadFileFlag==1)COS_Sleep(5); //wait for finishi reading file.
	mmc_MemFreeAll();


	hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_2);
	
	return  MCI_ERR_NO;
	
}

/*
* NAME:	MMC_AudioVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_FileReadFinish(void *pParam)
{
	FS_ASYN_READ_RESULT *pRsp = ( FS_ASYN_READ_RESULT*)pParam;
	g_ReadFileCounter--;
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]Read File Finish!"); 	
       if(pRsp->iResult!=0)
       {
       	MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MCI_MP3AAC_ERROR] Reading File Error!");  
		g_EndofFileFlag=1;
		g_ReadFileSize=0;
		g_ReadFileError=1;
		g_ReadFileFlag=0;
		return;
       }

	if(pRsp->uSize<((AudioJpeg_INPUT_BUFSIZE-MIN_INPUT_REMAIN_SIZE)>>1))
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MCI_MP3AAC]End of File!"); 
		g_EndofFileFlag=1;
		g_ReadFileSize=pRsp->uSize;
	}
	g_ReadFileFlag=0;


   return;
} 


	
/*
* NAME:	MMC_AudioVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioVocISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]VoC INT!");
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AUDIODEC_VOC);
} 



/*
* NAME:	MMC_AudioPcmISR()
* DESCRIPTION: ISR for pcm half buffer exhausted.
*/


void AudioHalfPcmISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]PCM Half!");
	Mmc_Audio_Pcm_Half = 0;

  mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AUDIODEC_PCM);
}

/*
* NAME:	MMC_AudioPcmISR()
* DESCRIPTION: ISR for pcm half buffer exhausted.
*/
void AudioEndPcmISR(void)
{
	//MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]PCM End!");
	Mmc_Audio_Pcm_Half = 1;
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_AUDIODEC_PCM);

}
/// Audio interface used in the test

void MMC_MpegPcmOutStart(MPEG_PLAY *pAudioPlayer)
{



MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]MMC_MpegPcmOutStart begin!");
    // Stream out


	if( pAudioPlayer->MpegInput.bt_sco)  //voice over bt sco
	{
	audioStream.startAddress  = (UINT32*) g_BT8kPcmBuffer;//pAudioPlayer->MpegOutput.pcm.Buffer;
	audioStream.length        = g_TotalFrame*g_NumOfOutSample*2;

	//   streamOut.sampleRate    = HAL_AIF_FREQ_8000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
	audioStream.channelNb     =HAL_AIF_MONO; //pAudioPlayer->Voc_AudioDecStatus.nbChannel;//HAL_AIF_STEREO;//HAL_AIF_MONO;

	pAudioPlayer->Voc_AudioDecStatus.SampleRate=HAL_AIF_FREQ_8000HZ;

      aud_StreamStop(audioItf);

	audioItf = AUD_ITF_BLUETOOTH;
      audioStream.voiceQuality         = TRUE; 

	}
	else
	{
	audioStream.startAddress  = (UINT32*) pAudioPlayer->MpegOutput.pcm.Buffer;
	audioStream.length        = pAudioPlayer->MpegOutput.pcm.Size*4;//MP3DEC_PCM_OUTPUT_BUFFER_SIZE;//AIF_FIFO_LEN * 4;
	//   streamOut.sampleRate    = HAL_AIF_FREQ_8000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
	audioStream.channelNb     = pAudioPlayer->Voc_AudioDecStatus.nbChannel;//HAL_AIF_STEREO;//HAL_AIF_MONO;
	
        audioStream.voiceQuality         = !TRUE;    // To adjust with a key
    
         aud_StreamStop(audioItf);

         if(pAudioPlayer->MpegInput.pcmflag)
        	audioItf = AUD_ITF_LOUD_SPEAKER;
   
	}

   
  //  audioStream.voiceQuality         = !TRUE;    // To adjust with a key
    audioStream.halfHandler   = AudioHalfPcmISR;
    audioStream.endHandler    = AudioEndPcmISR;



   	if(audioItf!=AUD_ITF_EAR_PIECE&&audioItf!=AUD_ITF_LOUD_SPEAKER&&audioItf!=AUD_ITF_BLUETOOTH)
   	{
   	  	  // Initial cfg
    		audioItf = AUD_ITF_EAR_PIECE;
   	}
   	

    MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]pAudioPlayer->Voc_AudioDecStatus.SampleRate:%d",pAudioPlayer->Voc_AudioDecStatus.SampleRate);

	switch (pAudioPlayer->Voc_AudioDecStatus.SampleRate)
	{
	case 48000:
		diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_48K\n");
  		audioStream.sampleRate    = HAL_AIF_FREQ_48000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
		break;
	case 44100:
		diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_44_1K\n");
		audioStream.sampleRate    = HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
		break;
	case 32000:
		diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_32K\n");
		audioStream.sampleRate    = HAL_AIF_FREQ_32000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
		break;
	case 24000:
		diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_24K\n");
		audioStream.sampleRate    = HAL_AIF_FREQ_24000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
		break;
	case 22050:
		diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_22_05K\n");
		audioStream.sampleRate    = HAL_AIF_FREQ_22050HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
		break;
		
	case 16000:
		diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_16K\n");
		audioStream.sampleRate    = HAL_AIF_FREQ_16000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
		break;
	case 12000:
		diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_12K\n");
		audioStream.sampleRate    = HAL_AIF_FREQ_12000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
		break;
	case 11025:
		diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_11_025K\n");
		audioStream.sampleRate    = HAL_AIF_FREQ_11025HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
		break;
	case 8000:
		diag_printf("[MMC_AUDIO]PCM_SAMPLE_RATE_8K\n");
		audioStream.sampleRate    = HAL_AIF_FREQ_8000HZ;//HAL_AIF_FREQ_44100HZ;//HAL_AIF_FREQ_8000HZ;
		break;			
	default:
		diag_printf("[MMC_AUDIO]##WARNING## Sample rate error:%d\n",pAudioPlayer->Voc_AudioDecStatus.SampleRate);
		break;
	}
	UINT32 errStatus = 	aud_StreamStart(audioItf, &audioStream, &audio_cfg);
	
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]MMC_MpegPcmOutStart end; errStatus = %d", errStatus);
}


# define N_(text)	text
# define gettext(text)	text
#define _(text) gettext(text)

//id3 xuml add
//#include "id3tag.h"
//#include <fcntl.h>
//#include <stdarg.h>
//#include <stdio.h>



/*
* NAME:	MMC_AudioDecOpen()
* DESCRIPTION: Initialize AUDIO decoder.
*/
INT32 *G_VppSBCConstY=NULL;

MCI_ERR_T MMC_AudioDecOpen (HANDLE fhd, MPEG_PLAY *pAudioPlayer)
{
	MPEG_INPUT *input = &pAudioPlayer->MpegInput;
	MPEG_OUTPUT *output = &pAudioPlayer->MpegOutput;
	HAL_ERR_T voc_ret = 0;
	int32 readlen = 0;
	uint32 tagsize=0;

	g_counterframe=0;
	g_ResampleInitFlag=1;

//	g_BT8KEnbleFlag=TRUE;



  MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]MMC_AudioDecOpen begin!");    
 
  
	input->fileHandle=-1;
	/*open AAC file*/
	input->fileHandle =fhd;

	if (input->fileHandle < 0)
	{
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MCI_MP3AAC]MMC_AudioDecOpen file open error!");
		
		return MCI_ERR_CANNOT_OPEN_FILE;
	}
	else
	{
		//MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]MMC_AudioDecOpen file open sucess!");
	}
  
	/*initialize I/O data struct*/
	G_VppSBCConstY = (INT32*)mmc_MemMalloc(468*4);
	input->bof = (uint8 *)mmc_MemMalloc(AudioJpeg_INPUT_BUFSIZE);
	
	if(input->bof==NULL || G_VppSBCConstY==NULL)
	{
		mmc_MemFreeAll();
		return MCI_ERR_OUT_OF_MEMORY;		
	}

	
	memset(input->bof, 0, AudioJpeg_INPUT_BUFSIZE);
	input->length = AudioJpeg_INPUT_BUFSIZE;
	input->data = input->bof;
	input->eof = input->bof + input->length;
	input->fileOffset = 0;
	input->readOffset = 0;
	input->inFlag = 0;
	input->is_first_frame=1;
	input->pcmflag = FALSE;
    //  	input->bt_sco= FALSE;

    
	output->length = (AUDIO_MAX_OUTPUT_BUFSIZE) >> 2;//80 int
	output->rPingpFlag = 0;
	output->wPingpFlag = 0;
	output->outFlag = 0;
	

	hal_OverlayLoad(HAL_OVERLAY_INT_SRAM_ID_2);
	output->pcm.Buffer = (uint32 *)pcmbuf_overlay;
	output->data =  (int16 *)pcmbuf_overlay;
	memset(output->pcm.Buffer,0,AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
	output->pcm.Buffer_head=output->pcm.Buffer;


   	g_BT8kPcmBuffer=(INT16 *)mmc_MemMalloc(AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
	memset(g_BT8kPcmBuffer, 0, AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);

      
	
	switch(pAudioPlayer->mode)
	{
		case MMC_AAC_DECODE:
			output->pcm.Size = AACDEC_PCM_OUTPUT_BUFFER_SIZE>>2;
			pAudioPlayer->Voc_AudioDecStatus.consumedLen = 0;
			pAudioPlayer->Voc_AudioDecStatus.nbChannel = 0;
			pAudioPlayer->Voc_AudioDecStatus.output_len = 0;
			pAudioPlayer->Voc_AudioDecStatus.streamStatus= 0;
			
			pAudioPlayer->Voc_AudioDecIN.EQ_Type=-1;
			pAudioPlayer->Voc_AudioDecIN.reset=1;
			break;
			
		case MMC_MP3_DECODE:
			output->pcm.Size = MP3DEC_PCM_OUTPUT_BUFFER_SIZE>>2;
			pAudioPlayer->Voc_AudioDecStatus.consumedLen = 0;
			pAudioPlayer->Voc_AudioDecStatus.nbChannel = 0;
			pAudioPlayer->Voc_AudioDecStatus.output_len = 0;
			pAudioPlayer->Voc_AudioDecStatus.streamStatus= 0;
			
			pAudioPlayer->Voc_AudioDecIN.EQ_Type=-1;
			pAudioPlayer->Voc_AudioDecIN.reset=1;			
			break;	
			
		default:
			MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MCI_MP3AAC_ERROR]Error Mode!");
			break;
	}
	  buffer_flag=0;
  	g_EndofFileFlag=0;
  	g_ReadFileFlag=0;
	Mmc_Audio_PcmDataCopyCount=0;
	Mmc_Audio_Pcm_Half=0;
	Mmc_Audio_FrameCount = 0;
	ErrorFrameCount=0;
	g_ReadFileSize=0;
	g_MemMoveFlag=0;
	g_ReadFileError=0;
	ContinuePlayFlag = 0;
	g_ReadFileFlag=0;
	/*prepare input stream*/
	readlen = FS_Read(input->fileHandle, input->bof, input->length);
	
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_MP3AAC] First Read Len=%d ;Need Length=%d\n", readlen,input->length);
	
	if(readlen<0)
	{
		return MCI_ERR_CANNOT_OPEN_FILE;
	}
	
	if(g_ConsumedLength>0)
	{
		g_AudioSeekFlag=1;
	}
	else
	{
		g_AudioSeekFlag=0;
	}

	
	if(readlen<input->length)
	{
		input->eof = input->bof + readlen;
		g_MemMoveFlag=1;
		g_EndofFileFlag=1;


		
		if(readlen<4096)
		{
				memset(input->bof +  readlen,0,4096-readlen);
				input->eof = input->bof + 4096; //4096 is two times than Voc inbuffer.				
		}
	}
	
	if ((input->data[0]=='I')&&(input->data[1]=='D')&&(input->data[2]=='3'))
	{
        /* high bit is not used */
		tagsize = (input->data[6] << 21) | (input->data[7] << 14) |
			(input->data[8] <<  7) | (input->data[9] <<  0);
		
		tagsize += 10;
		
		diag_printf("[MMC_AUDIO] tagsize: %d\n", tagsize);
		if(FS_Seek(input->fileHandle, tagsize, FS_SEEK_SET)<0)return MCI_ERR_CANNOT_OPEN_FILE;;
		
		readlen = FS_Read(input->fileHandle, input->bof, input->length);
		diag_printf("[MMC_AUDIO] after ID3first read len=%d ;need length=%d\n", readlen,input->length);
		if(readlen<0)
		{
			return MCI_ERR_CANNOT_OPEN_FILE;
		}
		if(readlen<input->length)
		{
			input->eof = input->bof + readlen;
			g_MemMoveFlag=1;
			g_EndofFileFlag=1;
			if(readlen<4096)
			{
				memset(input->bof +  readlen,0,4096-readlen);
				input->eof = input->bof + 4096; //4096 is two times than Voc inbuffer.
				
			}
		}
	}
	
	input->fileOffset += readlen;
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]MMC_AudioDecOpen vpp_AudioJpegDecOpen!");
	/*open voc decoder*/
	voc_ret = vpp_AudioJpegDecOpen((HAL_VOC_IRQ_HANDLER_T)MMC_AudioVocISR);
	 
	switch (voc_ret)
	{
	case HAL_ERR_NO:
		//diag_printf("[MMC_AUDIO] success to call vpp_AudioJpegDecOpen(), result: %d \n", voc_ret);
		break;
		
	case HAL_ERR_RESOURCE_RESET:
	case HAL_ERR_RESOURCE_BUSY:  
	case HAL_ERR_RESOURCE_TIMEOUT:
	case HAL_ERR_RESOURCE_NOT_ENABLED:	  
	case HAL_ERR_BAD_PARAMETER:
	case HAL_ERR_UART_RX_OVERFLOW:
	case HAL_ERR_UART_TX_OVERFLOW:
	case HAL_ERR_UART_PARITY:
	case HAL_ERR_UART_FRAMING:
	case HAL_ERR_UART_BREAK_INT:
	case HAL_ERR_TIM_RTC_NOT_VALID:
	case HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED:
	case HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED:
	case HAL_ERR_COMMUNICATION_FAILED:
	case HAL_ERR_QTY:
		MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MCI_MP3AAC_ERROR]Fail to Call vpp_AudioJpegDecOpen()t: %d \n", voc_ret);
		return MCI_ERR_ERROR;
	default:
		break;
	}
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]MMC_AudioDecOpen end!");
	return MCI_ERR_NO;
	
}



/*
* NAME:	MMC_AUDIODecVoc()
* DESCRIPTION: Decode AAC stream ......
*/
//static	HANDLE testfileHandle;
extern const INT32 G_VppSBCConstX[];
//extern  INT32 G_VppSBCConstY[411];
//#define SBC_Out_put_addr                            			 (6212 + 0x0000) //256 short  sbc_output_addr
//#define SBC_Out_put_addr                            			 (7916 + 0x0000) //256 short  sbc_output_addr
MCI_ERR_T MMC_AUDIODecVoc (MPEG_PLAY *pAudioPlayer)
{
	MPEG_INPUT *input = &pAudioPlayer->MpegInput;
	MPEG_OUTPUT *output = &pAudioPlayer->MpegOutput;
	vpp_AudioJpeg_DEC_OUT_T *pVoc_AudioDecStatus = &pAudioPlayer->Voc_AudioDecStatus;
	vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pAudioPlayer->Voc_AudioDecIN;

       SSHdl *my_ss_handle = pAudioPlayer->MpegInput.ss_handle;
      bt_a2dp_audio_cap_struct * my_audio_config_handle=   pAudioPlayer->MpegInput.audio_config_handle ;
       bt_a2dp_sbc_codec_cap_struct *sbc_config_handle =  &my_audio_config_handle->codec_cap.sbc;
	  
	
//	MCI_TRACE (TSTDOUT | MCI_AUDIO_TRC,0, "*******************MMC_AUDIODecVoc begin**********************");
	//MCI_TRACE (MCI_AUDIO_TRC,0, "*******************Mmc_Audio_FrameCount:%d",Mmc_Audio_FrameCount);
	//diag_printf("[MMC_AUDIO]frame: %d,input->bof:%x,input->data:%x,input->eof:%x\n", Mmc_Audio_FrameCount,input->bof,input->data,input->eof);	
	Mmc_Audio_FrameCount++;

	MCI_TRACE (TSTDOUT | MCI_AUDIO_TRC,0, "pAudioPlayer->MpegInput.bt_sco =%d",pAudioPlayer->MpegInput.bt_sco);


	if(input->is_first_frame==0)
	{	
		output->data=(int16*)output->pcm.Buffer;
		//vpp_AudioJpegDecStatus(pVoc_AudioDecStatus);
		//diag_printf("voc status, ErrorStatus : %d, nbChannel: %d,  consumedLen: %d\n", pVoc_AudioDecStatus->ErrorStatus, pVoc_AudioDecStatus->nbChannel, pVoc_AudioDecStatus->consumedLen );
		//diag_printf("[MMC_AUDIO]nbChannel : %d, output_len: %d\n", pVoc_AudioDecStatus->nbChannel, pVoc_AudioDecStatus->output_len);
		
		//input->data += (pVoc_AudioDecStatus->consumedLen);
		vpp_AudioJpegDecStatus(pVoc_AudioDecStatus);

		#ifdef MP3_PLAYER_AGC


//		diag_printf("voc status, ErrorStatus : %d, audioItf: %d,  audio_cfg.spkLevel: %d\n", pVoc_AudioDecStatus->ErrorStatus, audioItf, audio_cfg.spkLevel );

		if(pVoc_AudioDecStatus->ErrorStatus==0&&audioItf==AUD_ITF_LOUD_SPEAKER&&audio_cfg.spkLevel== AUD_SPK_VOL_7)
		{
			g_AgcDataLength=pVoc_AudioDecStatus->output_len/2;

		       int32 timebegin=hal_TimGetUpTime();

			AGainCtr_S(g_AgcDataAddress,g_AgcDataAddress,g_AgcDataLength);				

			int32 timeend=hal_TimGetUpTime();
          		 MCI_TRACE (TSTDOUT,0,"AGC time = %dms",((timeend-timebegin)*1000)/16384);

		}

		#endif
/*

             for(int i=0;i<pVoc_AudioDecStatus->output_len/2;i++)
             {
                    g_BTDataAddress[i]=(g_BTDataAddress[i]>>2);
             }
        
*/
//		MCI_TRACE (TSTDOUT | MCI_AUDIO_TRC,0, "*******************ErrorStatus : %d, nbChannel: %d,  consumedLen: %d,my_audio_config_handle->codec_type :%d**********************",pVoc_AudioDecStatus->ErrorStatus, pVoc_AudioDecStatus->nbChannel, pVoc_AudioDecStatus->consumedLen,my_audio_config_handle->codec_type );
	//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]pVoc_AudioDecStatus->consumedLen: %d",pVoc_AudioDecStatus->consumedLen);
     
          if( is_SSHdl_valid(my_ss_handle)&& (my_audio_config_handle->codec_type == 2))      //BT_A2DP_MP3
 		    my_ss_handle->PutData(my_ss_handle, input->data, pVoc_AudioDecStatus->consumedLen);	 

	    if( is_SSHdl_valid(my_ss_handle)&& (my_audio_config_handle->codec_type ==1))      //BT_A2DP_sbc
 		    my_ss_handle->PutData(my_ss_handle, (UINT8*) hal_VocGetPointer(SBC_Out_put_addr),pVoc_AudioDecStatus->ImgWidth);	 



		input->data += (pVoc_AudioDecStatus->consumedLen);
		g_ConsumedLength+=pVoc_AudioDecStatus->consumedLen;

		
		//if((input->eof - input->data)<MIN_INPUT_REMAIN_SIZE)
		if(g_ReadFileError==1)
		{
		     return MCI_ERR_ERROR;
		}
		if(g_MemMoveFlag==0)
		{
			if((input->data-input->bof)>(((AudioJpeg_INPUT_BUFSIZE-MIN_INPUT_REMAIN_SIZE)>>1)+MIN_INPUT_REMAIN_SIZE)&&buffer_flag==0&&g_EndofFileFlag==0)
			{
			//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]ErrorStatus : %d, nbChannel: %d,  consumedLen: %d",pVoc_AudioDecStatus->ErrorStatus, pVoc_AudioDecStatus->nbChannel, pVoc_AudioDecStatus->consumedLen );
				MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]input->eof - input->data: %d",(input->data-input->bof));
				FS_AsynReadReq (input->fileHandle,input->bof+MIN_INPUT_REMAIN_SIZE,((AudioJpeg_INPUT_BUFSIZE-MIN_INPUT_REMAIN_SIZE)>>1),MMC_FileReadFinish);
				g_ReadFileFlag=1;
				buffer_flag=1;
				g_ReadFileCounter++;
			}
			else if(g_EndofFileFlag!=0&&buffer_flag!=1)
			{
				input->eof =input->eof-((AudioJpeg_INPUT_BUFSIZE-MIN_INPUT_REMAIN_SIZE)>>1)+g_ReadFileSize;
				g_MemMoveFlag=1;								
			}

			if((input->eof - input->data)<MIN_INPUT_REMAIN_SIZE)
			{
				MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]input->eof - input->data: %d",(input->eof - input->data));
	
				memcpy(input->bof+MIN_INPUT_REMAIN_SIZE-(input->eof - input->data), input->data, (input->eof - input->data));
				input->data=input->bof+MIN_INPUT_REMAIN_SIZE-(input->eof - input->data);
				
				if(g_EndofFileFlag==0)
				{
					MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]Read File!");
					FS_AsynReadReq (input->fileHandle,input->bof+MIN_INPUT_REMAIN_SIZE+((AudioJpeg_INPUT_BUFSIZE-MIN_INPUT_REMAIN_SIZE)>>1),((AudioJpeg_INPUT_BUFSIZE-MIN_INPUT_REMAIN_SIZE)>>1),MMC_FileReadFinish);
					g_ReadFileFlag=1;
					buffer_flag=0;
					g_ReadFileCounter++;
				}
				else
				{
					input->eof =input->bof+MIN_INPUT_REMAIN_SIZE+g_ReadFileSize;
					g_MemMoveFlag=1;
					
				}				
				
			}

		}
		else
		{
			if((input->eof - input->data)<MIN_INPUT_REMAIN_SIZE&&EndErrorFlag==0)
			{
				MCI_TRACE (MCI_AUDIO_TRC,0, "[MCI_MP3AAC]input->eof - input->data: %d",(input->eof - input->data));
				memcpy(input->bof, input->data, (input->eof - input->data));
				memset(input->bof + (input->eof - input->data),0,4096);
				input->data=input->bof;
				input->eof=input->bof+(input->eof - input->data)+4096;	
				EndErrorFlag = 1;
			}
		}
		
	
		if(pVoc_AudioDecStatus->ErrorStatus==0)
		{

			 if( pAudioPlayer->MpegInput.bt_sco == TRUE)
			{
				INT32 i;

				int32 timebegin=hal_TimGetUpTime();		

				if(pVoc_AudioDecStatus->nbChannel==2)
				{
					INT16 *PCM_p=(INT16 *)g_BTDataAddress;
					
					for(i=0;i<pVoc_AudioDecStatus->output_len/2;i=i+2)
					{
						PCM_p[i/2]=(PCM_p[i]>>1)+(PCM_p[i+1]>>1);					
					}
				}


				if(g_ResampleInitFlag)
				{
					g_TotalFrame=output->pcm.Size/(pVoc_AudioDecStatus->output_len>>2);

					g_NumOfOutSample=ResampleInit(pVoc_AudioDecStatus->SampleRate, 8000, pVoc_AudioDecStatus->output_len, pVoc_AudioDecStatus->nbChannel);
					
					g_ResampleInitFlag=0;
				}	

				/* Resample stuff in input buffer */        

				g_Nout=ResampleOneFrame(g_BTDataAddress,g_BT8kPcmBuffer+g_counterframe*g_NumOfOutSample);

				g_counterframe++;

				if(g_counterframe>=g_TotalFrame)
				{
					g_counterframe=0;
				}

	          		MCI_TRACE (TSTDOUT,0,"g_Nout = %d",g_Nout);  


				int32 timeend=hal_TimGetUpTime();
				
	          		MCI_TRACE (TSTDOUT,0,"resample time = %dus",((timeend-timebegin)*1000000)/16384);

			}




		
                   ErrorFrameCount=0;
		     

			if(Mmc_Audio_Pcm_Half==0)
			{
				output->pcm.Buffer_head=output->pcm.Buffer; 		
			}
			else if(Mmc_Audio_Pcm_Half==1)
			{
				output->pcm.Buffer_head=output->pcm.Buffer+(output->pcm.Size>>1);			
			}
			
                 /* if(! is_SSHdl_valid(my_ss_handle))  
                  	{*/
	                output->data=(int16 *)&output->pcm.Buffer_head[Mmc_Audio_PcmDataCopyCount];
			Mmc_Audio_PcmDataCopyCount+=(pVoc_AudioDecStatus->output_len>>2);
		/*	
                  	}
		   else 	g_AudioSeekFlag=1;	  */

		}
		else
		{
			if(EndErrorFlag)
			{
				MCI_TRACE (TSTDOUT | MCI_AUDIO_TRC,0, "[MMC_MP3AAC_ERROR]Error Frame!EndErrorFlag=1");
				EndErrorFlag = 0;

				if(input->inFlag == 1)
				{
					return MCI_ERR_END_OF_FILE;
				}
				else
				{
						return MCI_ERR_ERROR;	
				}
			}
			else
			{
				ErrorFrameCount++;
				sxr_Sleep(5);
				if(g_ReadFileCounter>1)sxr_Sleep(80);
				//MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_MP3AAC_ERROR]Error Frame!:%d\n",ErrorFrameCount);
				if(ErrorFrameCount>100||g_ReadFileCounter>2)
				{
					MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_MP3AAC_ERROR]ErrorFrameCount:%d g_ReadFileCounter:%d",ErrorFrameCount,g_ReadFileCounter);
					return MCI_ERR_ERROR;				
				}
			}
		}
	}
	input->is_first_frame=0;
	//diag_printf("[MMC_AUDIO]Mmc_Audio_PcmDataCopyCount: %d\n", Mmc_Audio_PcmDataCopyCount);	
	pVoc_Voc_AudioDecIN->inStreamBufAddr=(INT32 *)input->data;
	pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)output->data;


#ifdef MP3_PLAYER_AGC
	g_AgcDataAddress=(INT16 *)((UINT32)( output->data)|0x20000000);

#endif

	g_BTDataAddress=(INT16 *)((UINT32)( output->data)|0x20000000);


	pVoc_Voc_AudioDecIN->mode=pAudioPlayer->mode;
	pVoc_Voc_AudioDecIN->BsbcEnable = 0;
	pVoc_Voc_AudioDecIN->audioItf=audioItf;

	 if( is_SSHdl_valid(my_ss_handle)&& (my_audio_config_handle->codec_type == 1))      //BT_A2DP_sbc
	 	{
	 	
	 	int mode =SBC_MODE_JOINT_STEREO;
                 pVoc_Voc_AudioDecIN->BsbcEnable = 1;
        
			
	      if( sbc_config_handle->channel_mode == 0x01 )
	         mode = SBC_MODE_JOINT_STEREO; // joint stereo
	      else if( sbc_config_handle->channel_mode == 0x02 )
	        mode = SBC_MODE_STEREO; // stereo
	      else if( sbc_config_handle->channel_mode == 0x04 )
	        mode = SBC_MODE_DUAL_CHANNEL; // dual
	      else if( sbc_config_handle->channel_mode == 0x08 )
	        mode= SBC_MODE_MONO; // mono
	      else
	        MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIO]##WARNING##ErrorSBC channel mode!\n"); 

		   pVoc_Voc_AudioDecIN->Source_width = mode;

	      if( sbc_config_handle->alloc_method  == 0x01 )
	           pVoc_Voc_AudioDecIN->Source_height= SBC_AM_LOUDNESS;// 
	      else if( sbc_config_handle->alloc_method  == 0x02 )
	           pVoc_Voc_AudioDecIN->Source_height= SBC_AM_SNR;//SBC_AM_LOUDNESS;//
	      else
	         MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIO]##WARNING##ErrorSBC alloc_method!\n"); 
		  
		 
		   pVoc_Voc_AudioDecIN->Cut_width =    SBC_BIT_POOL;
		   pVoc_Voc_AudioDecIN->inStreamUBufAddr = (INT32*)G_VppSBCConstX;
		   pVoc_Voc_AudioDecIN->inStreamVBufAddr = G_VppSBCConstY;
	 	}
	//MCI_TRACE (MCI_AUDIO_TRC,0, "*******************Mmc_Audio_PcmDataCopyCount:%d",Mmc_Audio_PcmDataCopyCount);
	if((input->inFlag==0)|| input->pcmflag )
	{

		if(Mmc_Audio_PcmDataCopyCount>=(output->pcm.Size))
		{
			Mmc_Audio_PcmDataCopyCount=0;
			input->inFlag = 1;
            
			MMC_MpegPcmOutStart(pAudioPlayer);		
            
                     input->pcmflag = FALSE;
		}
		else
		{

			vpp_AudioJpegDecScheduleOneFrame(pVoc_Voc_AudioDecIN);
		  
		}
	 
	}
	else
	{

		if(Mmc_Audio_PcmDataCopyCount>=(output->pcm.Size>>1))
		{
		
			Mmc_Audio_PcmDataCopyCount=0;
		}
		else
		{
		
			//MCI_TRACE (MCI_AUDIO_TRC,0, "*******************vpp_AudioJpegDecScheduleOneFrame************");

			vpp_AudioJpegDecScheduleOneFrame(pVoc_Voc_AudioDecIN);
		}

	} 

	 
	//diag_printf("voc status, ErrorStatus : %d, streamStatus: %d,  consumedLen: %d\n", pVoc_AudioDecStatus->ErrorStatus, pVoc_AudioDecStatus->streamStatus, pVoc_AudioDecStatus->consumedLen );
	//diag_printf("[MMC_AUDIO]nbChannel : %d, consumedLen: %d\n", pVoc_AudioDecStatus->nbChannel, pVoc_AudioDecStatus->consumedLen);
	return MCI_ERR_NO;
}

MCI_ERR_T MMC_AUDIODecPcm(MPEG_PLAY *pAudioPlayer)
{
	MPEG_INPUT *input = &pAudioPlayer->MpegInput;
	MPEG_OUTPUT *output = &pAudioPlayer->MpegOutput;	
	vpp_AudioJpeg_DEC_IN_T *pVoc_Voc_AudioDecIN = &pAudioPlayer->Voc_AudioDecIN;

       SSHdl *my_ss_handle = pAudioPlayer->MpegInput.ss_handle;
      bt_a2dp_audio_cap_struct * my_audio_config_handle=   pAudioPlayer->MpegInput.audio_config_handle ;
       bt_a2dp_sbc_codec_cap_struct *sbc_config_handle =  &my_audio_config_handle->codec_cap.sbc;


	
	pVoc_Voc_AudioDecIN->inStreamBufAddr=(INT32 *)input->data;
	pVoc_Voc_AudioDecIN->outStreamBufAddr=(INT32 *)output->data;
	pVoc_Voc_AudioDecIN->mode=pAudioPlayer->mode;
	pVoc_Voc_AudioDecIN->BsbcEnable = 0;
	//diag_printf("#########MMC_AUDIODecPcm:%d\n",Mmc_Audio_Pcm_Half);
  //    MCI_TRACE (MCI_AUDIO_TRC,0, "*******************PCM");
	//MCI_TRACE (MCI_AUDIO_TRC,0, "*******************ContinuePlayFlag:%d",ContinuePlayFlag);
	//MCI_TRACE (MCI_AUDIO_TRC,0, "*******************ContinuePlayFlag:%d",Mmc_Audio_PcmDataCopyCount);

	 if( is_SSHdl_valid(my_ss_handle)&& (my_audio_config_handle->codec_type == 1))      //BT_A2DP_sbc
	 	{
	 	
	 	int mode =SBC_MODE_JOINT_STEREO;
                 pVoc_Voc_AudioDecIN->BsbcEnable = 1;
 	
		  
	      if( sbc_config_handle->channel_mode == 0x01 )
	         mode = SBC_MODE_JOINT_STEREO; // joint stereo
	      else if( sbc_config_handle->channel_mode == 0x02 )
	        mode = SBC_MODE_STEREO; // stereo
	      else if( sbc_config_handle->channel_mode == 0x04 )
	        mode = SBC_MODE_DUAL_CHANNEL; // dual
	      else if( sbc_config_handle->channel_mode == 0x08 )
	        mode= SBC_MODE_MONO; // mono
	      else
	        MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIO]##WARNING##ErrorSBC channel mode!\n"); 

		   pVoc_Voc_AudioDecIN->Source_width = mode;

	      if( sbc_config_handle->alloc_method  == 0x01 )
	           pVoc_Voc_AudioDecIN->Source_height= SBC_AM_LOUDNESS;// 
	      else if( sbc_config_handle->alloc_method  == 0x02 )
	           pVoc_Voc_AudioDecIN->Source_height= SBC_AM_SNR;//SBC_AM_LOUDNESS;//
	      else
	         MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_AUDIO]##WARNING##ErrorSBC alloc_method!\n"); 

	   
		   pVoc_Voc_AudioDecIN->Cut_width =    SBC_BIT_POOL;
		   pVoc_Voc_AudioDecIN->inStreamUBufAddr = (INT32*)G_VppSBCConstX;
		   pVoc_Voc_AudioDecIN->inStreamVBufAddr = G_VppSBCConstY;
	 	}



	
	if(ContinuePlayFlag)
	{
		//Drv_PcmOutJudgeWhetherPlayEnd(&(output->pcm.Buffer_head[Mmc_Audio_PcmDataCopyCount>>1])-576);
		diag_printf("!!!!!play all left data in the pcm buffer\n");
		MMC_AUDIODecClose(&MpegPlayer);
		MCI_AudioFinished(MCI_ERR_END_OF_FILE);
	
		mmc_SetCurrMode(MMC_MODE_IDLE);//jiashuo add
		ContinuePlayFlag = 0;
	}
	else
	{
		if(Mmc_Audio_PcmDataCopyCount==0)
		{
		//MCI_TRACE (MCI_AUDIO_TRC,0, "#################PCM");
			vpp_AudioJpegDecScheduleOneFrame(pVoc_Voc_AudioDecIN);
		}
		else
		{
			MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0, "[MMC_AUD_WARN]DISCARD A PCM INT!");
			diag_printf("MMC_AUDIO XXX\n");			
			//EVENT_Send2Task(MBOX_ID_MMC,MSG_MMC_AUDIODEC_PCM);
			
		}
	}
	return MCI_ERR_NO;
}

#ifdef ID3_ENABLED
static
int message(char const *format, ...)
{
	int len, newline, result;
	va_list args;
	
	len = strlen(format);
	newline = (len > 0 && format[len - 1] == '\n');
	
	if (on_same_line && newline && len > 1)
		fputc('\n', stderr);
	
	va_start(args, format);
	result = vfprintf(stderr, format, args);
	va_end(args);
	
	if (on_same_line && !newline && result < on_same_line) 
	{
		unsigned int i;
		
		i = on_same_line - result;
		while (i--)
			putc(' ', stderr);
	}
	
	on_same_line = newline ? 0 : result;
	
	if (!newline) 
	{
		fputc('\r', stderr);
		fflush(stderr);
	}
	
	return result;
}


/*
* NAME:	show_id3()
* DESCRIPTION:	display an ID3 tag
*/
static
void show_id3(struct id3_tag const *tag)
{
	unsigned int i;
	struct id3_frame const *frame;
	id3_ucs4_t const *ucs4;
	id3_latin1_t *latin1;
	char const spaces[] = "          ";
	
	struct {
		char const *id;
		char const *name;
	} const info[] = {
		{ ID3_FRAME_TITLE,  N_("Title")     },
		{ "TIT3",           0               },  /* Subtitle */
		{ "TCOP",           0,              },  /* Copyright */
		{ "TPRO",           0,              },  /* Produced */
		{ "TCOM",           N_("Composer")  },
		{ ID3_FRAME_ARTIST, N_("Artist")    },
		{ "TPE2",           N_("Orchestra") },
		{ "TPE3",           N_("Conductor") },
		{ "TEXT",           N_("Lyricist")  },
		{ ID3_FRAME_ALBUM,  N_("Album")     },
		{ ID3_FRAME_YEAR,   N_("Year")      },
		{ ID3_FRAME_TRACK,  N_("Track")     },
		{ "TPUB",           N_("Publisher") },
		{ ID3_FRAME_GENRE,  N_("Genre")     },
		{ "TRSN",           N_("Station")   },
		{ "TENC",           N_("Encoder")   }
	};
	
	/* text information */
	
	for (i = 0; i < sizeof(info) / sizeof(info[0]); ++i)
	{
		union id3_field const *field;
		unsigned int nstrings, namelen, j;
		char const *name;
		
		frame = id3_tag_findframe(tag, info[i].id, 0);
		if (frame == 0)
			continue;
		
		field    = &frame->fields[1];
		nstrings = id3_field_getnstrings(field);
		
		name = info[i].name;
		if (name)
			name = gettext(name);
		
		namelen = name ? strlen(name) : 0;
		assert(namelen < sizeof(spaces));
		
		for (j = 0; j < nstrings; ++j) 
		{
			ucs4 = id3_field_getstrings(field, j);
			assert(ucs4);
			
			if (strcmp(info[i].id, ID3_FRAME_GENRE) == 0)
				ucs4 = id3_genre_name(ucs4);
			
			latin1 = id3_ucs4_latin1duplicate(ucs4);
			if (latin1 == 0)
				goto fail;
			
			if (j == 0 && name)
			{
				message("%s%s: %s\n", &spaces[namelen], name, latin1);
			}
			else 
			{
				if (strcmp(info[i].id, "TCOP") == 0 ||
					strcmp(info[i].id, "TPRO") == 0) 
				{
					message("%s  %s %s\n", spaces, (info[i].id[1] == 'C') ?
						_("Copyright (C)") : _("Produced (P)"), latin1);
				}
				else
					message("%s  %s\n", spaces, latin1);
			}
			
			//free(latin1);
		}
	}
	
	/* comments */
	
	i = 0;
	while ((frame = id3_tag_findframe(tag, ID3_FRAME_COMMENT, i++))) 
	{
		id3_latin1_t *ptr, *newline;
		int first = 1;
		
		ucs4 = id3_field_getstring(&frame->fields[2]);
		assert(ucs4);
		
		if (*ucs4)
			continue;
		
		ucs4 = id3_field_getfullstring(&frame->fields[3]);
		assert(ucs4);
		
		latin1 = id3_ucs4_latin1duplicate(ucs4);
		if (latin1 == 0)
			goto fail;
		
		ptr = latin1;
		while (*ptr) 
		{
			newline = strchr(ptr, '\n');
			if (newline)
				*newline = 0;
			
			if (strlen(ptr) > 66)
			{
				id3_latin1_t *linebreak;
				
				linebreak = ptr + 66;
				
				while (linebreak > ptr && *linebreak != ' ')
					--linebreak;
				
				if (*linebreak == ' ') 
				{
					if (newline)
						*newline = '\n';
					
					newline = linebreak;
					*newline = 0;
				}
			}
			
			if (first) 
			{
				char const *name;
				unsigned int namelen;
				
				name    = _("Comment");
				namelen = strlen(name);
				assert(namelen < sizeof(spaces));
				
				message("%s%s: %s\n", &spaces[namelen], name, ptr);
				first = 0;
			}
			else 
				message("%s  %s\n", spaces, ptr);
			
			ptr += strlen(ptr) + (newline ? 1 : 0);
		}
		
		//free(latin1);
		break;
	}
	
	if (0)
	{
fail:
    printf("error!\n");//error("id3", _("not enough memory to display tag"));
	}
}

#endif

int32 Audio_MpegPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress)
{
	int32 result;
	MCI_TRACE (MCI_AUDIO_TRC,0,"Audio_MpegPlay,fhd is %d",fhd);
	
	//g_FileHandle=fhd;
		if (fhd< 0)
	{
		diag_printf("[MMC_AUDIO_ERROR]ERROR file hander: %d \n", fhd);
		return MCI_ERR_CANNOT_OPEN_FILE;
	}

	if ((g_MPEGFILESIZE=FS_GetFileSize(fhd))<= 0)
	{
		diag_printf("[MMC_AUDIO_ERROR]can not get file size!:%d",g_MPEGFILESIZE);
		return MCI_ERR_CANNOT_OPEN_FILE;
	}

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]File Size: %d",g_MPEGFILESIZE);

	g_ConsumedLength=(INT32)(((INT64)PlayProgress*(INT64)g_MPEGFILESIZE)/10000);


	if ((result=FS_Seek(fhd,g_ConsumedLength,FS_SEEK_SET))< 0)
	{
		diag_printf("[MMC_AUDIO_ERROR]can not seek file!:%d",result);
		return MCI_ERR_CANNOT_OPEN_FILE;
	}

	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_AMR]g_AMRConsumedLength: %d",g_ConsumedLength);
	
	switch(filetype)
	{
		case MCI_TYPE_DAF:
			MpegPlayer.mode = MMC_MP3_DECODE;
			break;
			
		case MCI_TYPE_AAC:
			MpegPlayer.mode = MMC_AAC_DECODE;
			break;
			
		default:
			diag_printf("Audio_MpegPlay()->unsupport record media type");
			break;
	}
	
	MpegPlayer.MpegOutput.OutputPath = OutputPath;
	
	if((result= MMC_AudioDecOpen(fhd, &MpegPlayer))!=MCI_ERR_NO)
	{
		diag_printf("[MMC_AUDIO]##WARNING##MMC_AudioDecOpen() return false!\n");
		MMC_AUDIODecClose(&MpegPlayer);
		return result;
	}
	
	return  MCI_ERR_NO;
}

int32 Audio_MpegStop (void) 	
{
	//clear the flag used to play left data in the input buffer and pcm buffer 
	ErrorFrameCount=0;
	ContinuePlayFlag=0;


	MMC_AUDIODecClose(&MpegPlayer);	
	return  MCI_ERR_NO;
}


int32 Audio_MpegPause (void)	
{
	//MPEG_INPUT *input = &MpegPlayer.MpegInput;
	//Drv_PcmOutStop();
	g_AudioMode=MpegPlayer.mode;
	MCI_TRACE (MCI_AUDIO_TRC,0, "******************* Audio_MpegPause**********************");
	//aud_StreamPause(audioItf,TRUE);
	//clear the flag used to play left data in the input buffer and pcm buffer 
	ErrorFrameCount=0;
	ContinuePlayFlag=0;


	MMC_AUDIODecClose(&MpegPlayer);	
	
	//memset(MpegPlayer.MpegOutput.pcm.Buffer,0,AUDIO_MAX_PCM_OUTPUT_BUFFER_SIZE);
	return  MCI_ERR_NO;
}


int32 Audio_MpegResume ( HANDLE fhd) 
{
	int32 result;
//	Drv_PcmOutStart();
       MpegPlayer.mode=g_AudioMode;
//	aud_StreamPause(audioItf,FALSE);

	g_ConsumedLength-=2048;//VoC buffer length

	if(g_ConsumedLength<0)g_ConsumedLength=0;
	
	result=FS_Seek(fhd,g_ConsumedLength,FS_SEEK_SET);

	MCI_TRACE (MCI_AUDIO_TRC,0,"FS_seek pos:%d",result);
	
	if(result<0 )
	{
		MCI_TRACE (MCI_AUDIO_TRC,0,"FS_seek error!");
		return -1;
	}
	
	if((result= MMC_AudioDecOpen(fhd, &MpegPlayer))!=MCI_ERR_NO)
	{
		diag_printf("[MMC_AUDIO]##WARNING##MMC_AudioDecOpen() return false!\n");
		MMC_AUDIODecClose(&MpegPlayer);
		return result;
	}


	return  MCI_ERR_NO;
}


int32 Audio_MpegGetID3 (char * pFileName)  
{
#ifdef ID3_ENABLED

	file = id3_file_open(pFileName, ID3_FILE_MODE_READONLY);
	diag_printf("[MMC_AUDIO]************id3************: %x \n", file);
	
	if (file == 0) 
	{
		diag_printf("[MMC_AUDIO]##WARNING##************no id3!***********: %x \n", file);
		
	}
	else 
	{
		show_id3(id3_file_tag(file));
		id3_file_close(file);
	}
	
	
	diag_printf("[MMC_AUDIO]************id3  end************\n");
#endif 
	return MCI_ERR_NO;
}

int32 Audio_MpegGetPlayInformation (MCI_PlayInf * MCI_PlayInfMPEG)  
{

       if(g_MPEGFILESIZE>0)
       {
        	MCI_PlayInfMPEG->PlayProgress=(INT32)((((INT64)g_ConsumedLength)*10000)/g_MPEGFILESIZE);
       }
       else
       {
            MCI_PlayInfMPEG->PlayProgress=0;
       }
    
    
	MCI_TRACE (MCI_AUDIO_TRC,0,"[MCI_AUDIO_MPEG]PlayProgress:%d",MCI_PlayInfMPEG->PlayProgress);
	

	return MCI_ERR_NO;
}

int32 Audio_MpegUserMsg(int32 nMsg)
{
    //   SSHdl *my_ss_handle = MpegPlayer.MpegInput.ss_handle;
     

    switch (nMsg)
    {
    /*
      case MSG_MMC_AUDIODEC_A2DP:
// 	MCI_TRACE (TSTDOUT | MCI_AUDIO_TRC,0, "*******************MMCTask MSG_MMC_AUDIODEC_A2DP**********************");
	  	
	  	   if(( is_SSHdl_valid(my_ss_handle))&&(my_ss_handle->overflow))
	  	   	{
                   
                 my_ss_handle->overflow = FALSE;
                 my_ss_handle->PutData(my_ss_handle, (UINT8*) my_ss_handle->overflow_buf,my_ss_handle->overflow_size  );	 
			 	
		      }
	    //no break
*/
		
	case MSG_MMC_AUDIODEC_VOC:

// 	MCI_TRACE (TSTDOUT | MCI_AUDIO_TRC,0, "*******************MMCTask MSG_MMC_AUDIODEC_VOC**********************");
    /*                if(( is_SSHdl_valid(my_ss_handle))&&(my_ss_handle->overflow) )
        		  break;
*/
			switch (MMC_AUDIODecVoc(&MpegPlayer))
			{
			case MCI_ERR_END_OF_FILE:
				diag_printf("[MMC_AUDIO]MMC_AACDecRun() return FINISH!\n");

				ContinuePlayFlag = 1;
				break;
			case MCI_ERR_ERROR:
				diag_printf("[MMC_AUDIO]##WARNING##MMC_AACDecRun() return FALSE!\n");
				MMC_AUDIODecClose(&MpegPlayer);
				if(g_AudioSeekFlag==1)
				{
					MCI_AudioFinished(MCI_ERR_END_OF_FILE);
				}
				else
				{
					MCI_AudioFinished(MCI_ERR_INVALID_FORMAT);
				}
				mmc_SetCurrMode(MMC_MODE_IDLE);//jiashuo add
				ContinuePlayFlag = 0;
				break;
			default:
				break;
			}

    	   
		break;
	  
	case MSG_MMC_AUDIODEC_PCM:
//    MCI_TRACE (TSTDOUT | MCI_AUDIO_TRC,0, "*******************MMCTask MSG_MMC_AUDIODEC_PCM**********************");

			MMC_AUDIODecPcm(&MpegPlayer);
    
			break;
	default:
	diag_printf("[MMC_AUDIO]##WARNING##**************Error message******************!\n");
	break;
    }
	
    return 0;
}


