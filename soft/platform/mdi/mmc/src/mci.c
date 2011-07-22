////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2007, Coolsand Technologies, Inc.
//                       All Rights Reserved
//
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: mci.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//  
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "mci.h"

#include "mcip_debug.h"

#include "video_api.h"
//#include "med_main.h"
#include "fs_asyn.h"

#include "audio_api.h"
#include "mmc_camera.h"

#include "mmc_audiomp3stream.h"
#include "vidrec_api.h"

#include "cos.h"
#include "dm.h"
#include "mmc_sndrec.h"
#include "apfs.h"
#include "mmc_dafrtpss.h"

#include "apbs_m.h"
#include "mmc_midi.h"
#include "aud_fm.h"
#include "fmd_m.h"
#include "mcip_debug.h"

#include "hal_overlay.h"

#include "mmc_analogtv.h"
#include "cam_motion_detect.h"


static HANDLE g_ServerTaskHandle = HNULL;

static HANDLE g_AppTaskHandle = HNULL;

static HANDLE g_CodecTaskHandle = HNULL;

static HANDLE g_AgentTaskHandle = HNULL;

static HANDLE g_FsappTaskHandle = HNULL;


static HANDLE g_MMCTaskHandle = HNULL;

static HANDLE g_DITaskHandle = HNULL;

static HANDLE g_ImageTaskHandle = HNULL;

static MCI_PLAY_MODE_T  g_audio_format;

#define COS_MMC_TASKS_PRIORITY_BASE 180 

#define MMC_TASK_PRIORITY       (COS_MMC_TASKS_PRIORITY_BASE+0)
#define IMAGE_TASK_PRIORITY (COS_MMC_TASKS_PRIORITY_BASE+1)
#define USB_TASK_PRIORITY       (COS_MMC_TASKS_PRIORITY_BASE+2)

// for asyn fs task.
#define ASYNFS_TASK_PRIORITY (COS_MMC_TASKS_PRIORITY_BASE+3) 

#define MMC_TASK_STACK_SIZE     2048      // 2k
#define IMAGE_TASK_STACK_SIZE       2048    // 2k
#define USB_TASK_STACK_SIZE     2048      // 2k

// for asyn fs task.
#define ASYNFS_TASK_STACK_SIZE 2048   // 2k





/**************************************************************

    FUNCTION NAME       : GetMCITaskHandle

    PURPOSE             : Get    Task   Handle

    INPUT PARAMETERS    : module_type mod

    OUTPUT PARAMETERS   : VOID

    RETURNS             : HANDLE 
    
    REMARKS             : 

**************************************************************/
HANDLE GetMCITaskHandle(COSMBOXID mod)
{
    switch (mod)
    {
        case MBOX_ID_SERVER:
            return g_ServerTaskHandle;
        case MBOX_ID_APP: 
            return g_AppTaskHandle;
        case MBOX_ID_COCEC: 
            return g_CodecTaskHandle;
        case MBOX_ID_AGENT: 
            return g_AgentTaskHandle;
        case MBOX_ID_FSAPP:
            return g_FsappTaskHandle;           
                case MBOX_ID_MMC:
                        return g_MMCTaskHandle;
        case MBOX_ID_DI:
            return g_DITaskHandle;
          case MBOX_ID_IMAGE:
                        return g_ImageTaskHandle;
        default:
                //ASSERT(0);
            return g_ServerTaskHandle;
    }
}
/**************************************************************

    FUNCTION NAME       : SetMCITaskHandle

    PURPOSE             : Set  MmiTask  Handle

    INPUT PARAMETERS    : module_type mod, HANDLE hTask

    OUTPUT PARAMETERS   : VOID

    RETURNS             : BOOL 
    
    REMARKS             : 

**************************************************************/
BOOL SetMCITaskHandle(COSMBOXID mod, HANDLE hTask)
{
    switch (mod)
    {
        case MBOX_ID_SERVER:
            g_ServerTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_APP: 
            g_AppTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_COCEC: 
            g_CodecTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_AGENT: 
            g_AgentTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_FSAPP:
            g_FsappTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_MMC:
            g_MMCTaskHandle = hTask;
            return TRUE;
        case MBOX_ID_DI:
            g_DITaskHandle = hTask;
            return TRUE;
        case MBOX_ID_IMAGE:
            g_ImageTaskHandle = hTask;
            return TRUE;
        default:
            //ASSERT(0);
            return FALSE;
    }
}


extern VOID MMCTask(UINT32 data);
extern VOID ImageTask(UINT32 data);




void mmi_ebook_usb_mode_off(void);
void mmi_ebook_usb_mode_on(void);

/*
static INT32 MMC_GetLcdWidth()
{
     return LCD_WIDTH;
}

static INT32 MMC_GetLcdHeight()
{
    return LCD_HEIGHT;
}
*/

extern HANDLE g_hAsynFsTask;

INT32 MMC_LcdWidth=0;
INT32 MMC_LcdHeight=0;

/*
init lcd size for mci.
*/
BOOL MCI_LcdSizeSet(INT32 LcdWidth, INT32 LcdHeight)        
{
	if(LcdWidth<=0 || LcdHeight<=0)
		return FALSE;
     	MMC_LcdWidth=LcdWidth;
	MMC_LcdHeight=LcdHeight;
	return TRUE;
}


INT32  MMC_GetBVGA(INT32 bvga)
{
          return  bvga;
}

BOOL MCI_TaskInit(VOID)  //  MCI_MEDIA_PLAY_REQ,         
{   

diag_printf( "*******************MCI_TaskInit**********************");
 mmc_MemInit();
     	//MMC_LcdWidth=MMC_GetLcdWidth();
	//MMC_LcdHeight=MMC_GetLcdHeight();
 HANDLE hTask;
hTask = COS_CreateTask((PTASK_ENTRY)MMCTask, 
                NULL, NULL,
                MMC_TASK_STACK_SIZE, 
                MMC_TASK_PRIORITY, 
                COS_CREATE_DEFAULT, 0, "MMC Task"); 
    //ASSERT(hTask != HNULL);
    SetMCITaskHandle(MBOX_ID_MMC, hTask);

 // creat asyn fs task.
 g_hAsynFsTask = COS_CreateTask(BAL_AsynFsTask, 
                NULL, NULL,
                ASYNFS_TASK_STACK_SIZE, 
                ASYNFS_TASK_PRIORITY, 
                COS_CREATE_DEFAULT, 0, "ASYNFS Task");  
    


/*
hTask = COS_CreateTask(MMCTask, 
                NULL, NULL,
                USB_TASK_STACK_SIZE, 
                USB_TASK_PRIORITY, 
                COS_CREATE_DEFAULT, 0, "USB Task"); 
    ASSERT(hTask != HNULL);
    SetMCITaskHandle(MBOX_ID_MMC, hTask);*/

    return TRUE;

}






UINT32 MCI_Speach(boolean on) //chenhe for jasperII,for  can't speach on when calling
{
    return 0;
}

PRIVATE MCI_AUDIO_PLAY_CALLBACK_T g_mciAudioFinishedCallback = NULL;
PRIVATE MCI_AUDIO_BUFFER_PLAY_CALLBACK_T g_mciRingFinishedCallback = NULL;
PRIVATE MCI_AUDIO_FILE_RECORD_CALLBACK_T g_mciAudioRecordFinishedCallback = NULL;

VOID MCI_AudioFinished(MCI_ERR_T result)
{
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    if (g_mciAudioFinishedCallback)
    {
        (*g_mciAudioFinishedCallback)(result);
    }
}

VOID MCI_RingFinished(MCI_ERR_T result)
{
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    if (g_mciRingFinishedCallback)
    {
        (*g_mciRingFinishedCallback)(result);
    }
}

VOID MCI_AudioRecordFinished(MCI_ERR_T result)
{
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    if (g_mciAudioRecordFinishedCallback)
    {
        (*g_mciAudioRecordFinishedCallback)(result);
    }
}

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)

UINT32 MCI_AudioPlay (INT32 OutputPath,HANDLE fileHandle, mci_type_enum fielType,MCI_AUDIO_PLAY_CALLBACK_T callback,INT32 PlayProgress)   //  MCI_MEDIA_PLAY_REQ,         
{
	INT32 result;
	diag_printf( "***********OutputPath:%d*****file_name_p:%d, PlayProgress :%d  ",OutputPath,fileHandle,PlayProgress);
	g_mciAudioFinishedCallback=callback;

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_104M);

	result = LILY_AudioPlay(OutputPath, fileHandle,fielType, PlayProgress);
    if (result != MCI_ERR_NO)
    {
        csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    }

    return result;
}


UINT32 MCI_AudioPause(VOID)
{ 
    INT32 result;
    result = LILY_AudioPause();
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    return result;
}


UINT32 MCI_AudioGetDurationTime(HANDLE fileHandle, mci_type_enum fielType,INT32 BeginPlayProgress,INT32 OffsetPlayProgress,MCI_ProgressInf* PlayInformation) 
{ 
     return apfs_GetDurationTime( fileHandle,  fielType, BeginPlayProgress, OffsetPlayProgress, PlayInformation);
}


UINT32 MCI_AudioResume(HANDLE fileHandle)
{
    INT32 result;
    diag_printf( "##################MCI_AudioResume############################");
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_104M);
    result = LILY_AudioResume( fileHandle );
    return result;
}


UINT32 MCI_AudioStop(VOID)
{
    INT32 result;
    result = LILY_AudioStop();
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    return result;
}


UINT32 MCI_AudioGetID3(char *pfilename)     //  MCI_MEDIA_PLAY_REQ,         
{
    return 0;
}

UINT32 MCI_AudioGetPlayInformation(MCI_PlayInf* PlayInformation)     //  MCI_MEDIA_PLAY_REQ,         
{
	    
    	return LILY_AudioGetPlayInformation(PlayInformation);
}

UINT32 MCI_AudioSetVolume(UINT16 volume)
{
    return 0;
}


UINT32 MCI_AudioSetOutputPath(UINT16 OutputPath,UINT16 Onoff)
{
    return 0;
}


UINT32 MCI_AudioSetEQ(AUDIO_EQ EQMode)
{   
    INT32 result=0; 

    result = LILY_AudioSetEQ(EQMode);
	
    return result;
}


 MCI_ERR_T MCI_AudioGetFileInformation (CONST HANDLE FileHander,
											AudDesInfoStruct  * CONST DecInfo,
											CONST mci_type_enum FileType   )
 {
	return apfs_GetFileInformation(FileHander, DecInfo, FileType);
 }


#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */

extern MPEG_PLAY MpegPlayer;

MPEG_INPUT *mpeg_input_p = &(MpegPlayer.MpegInput);



void mmc_audio_bt_sco(bool flag)
{

MpegPlayer.MpegInput.bt_sco = flag;
MpegPlayer.MpegInput.pcmflag = TRUE;
}


#ifdef __BT_A2DP_PROFILE__ 
A2DP_codec_struct *MCI_A2dpDafOpen( A2DP_Callback pHandler, 
									    bt_a2dp_audio_cap_struct *daf_config_data, 
									    UINT8 *buf, UINT32 buf_len)
{      
 

    mpeg_input_p->audio_config_handle = daf_config_data;
 
    return A2DP_DAF_Open( &(mpeg_input_p->ss_handle),  pHandler, &(mpeg_input_p->audio_config_handle)->codec_cap.mp3, buf,  buf_len);
}



void MCI_A2dpDafClose(void)
{   
     A2DP_DAF_Close(&(mpeg_input_p->ss_handle));
     return;
}

extern A2DP_codec_struct *SBC_Open( SSHdl **ss_handle,  A2DP_Callback pHandler, 
							bt_a2dp_sbc_codec_cap_struct *sbc_config_data, 
   							UINT8 *buf,   UINT32 buf_len );

extern VOID SBC_Close( SSHdl **ss_handle );

A2DP_codec_struct *MCI_SBCOpen(    A2DP_Callback pHandler, 
									    bt_a2dp_audio_cap_struct *daf_config_data, 
									    UINT8 *buf, UINT32 buf_len)
{      
 
    mpeg_input_p->audio_config_handle = daf_config_data;
 return SBC_Open( &(mpeg_input_p->ss_handle),  pHandler, &(mpeg_input_p->audio_config_handle)->codec_cap.sbc, buf,  buf_len);

 return 0;
}


void MCI_SBCClose(void )
{   
     SBC_Close(&(mpeg_input_p->ss_handle));
     return;
}




#endif
extern AUD_LEVEL_T audio_cfg;
extern AUD_ITF_T audioItf;

//播放
//UINT32 MCI_AudioPlayBuffer(UINT32 *pBuffer, UINT32 len, UINT8 loop,MCI_AUDIO_BUFFER_PLAY_CALLBACK_T callback，enum format，INT32 startPosition)，
//取得播放位置及相关信息
//UINT32  MCI_AudioBufferplay_getinfo(struct* info),
//暂停
//UINT32 MCI_AudioPauseBufferplay(),
//恢复
//UINT32 MCI_AudioResumeBufferplay(),
//停止
//UINT32MCI_AudioStopBuffer（）

INT32  MCI_AudioPlayBuffer(INT32 *pBuffer, UINT32 len, UINT8 loop,MCI_AUDIO_BUFFER_PLAY_CALLBACK_T callback,MCI_PLAY_MODE_T  format,INT32 startPosition)
{
    APBS_ENC_STREAM_T stream;
    APBS_AUDIO_CFG_T apbsAudioCfg = 
        {
            .spkLevel = audio_cfg.spkLevel,
            .filter = NULL
        };

    if(format == MCI_PLAY_MODE_AMR_RING)
    {
        stream.startAddress = (UINT32)pBuffer+6;
        stream.length = len*4-6; 
    }
    else 
    {
        stream.startAddress = (UINT32)pBuffer;
        stream.length = len*4; 
    }
    stream.mode = format;
    stream.handler = NULL;

	g_mciRingFinishedCallback=callback;
    g_audio_format = format;

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_104M);

    INT32 result;
    if(format==MCI_PLAY_MODE_MID)
    {
#ifndef TARGET_MIDI_SUPPORT_OFF	  	
        result = MMC_Midi2RawBuffer(pBuffer,  len*4, loop);
#else
        result = MCI_ERR_UNKNOWN_FORMAT;
#endif
    }
    else
    {
        result = apbs_Play(audioItf, &stream, &apbsAudioCfg, loop);
    }
	//result = AudioMp3StartStream((uint32 *)pBuffer,len,loop);

    if (result != MCI_ERR_NO)
    {
        csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    }

    return result;
}

INT32  MCI_AUD_StreamPlayPCM( INT32 *pBuffer, UINT32 len, MCI_PLAY_MODE_T mciFormat, APBS_STREAM_USER_HANDLER_T callback,UINT32 sampleRate, UINT32	bitPerSample)

{

	  APBS_ENC_STREAM_T stream;
  	  APBS_AUDIO_CFG_T apbsAudioCfg = 
           {
                    .spkLevel = audio_cfg.spkLevel,
                    .filter = NULL
           };


   	 stream.startAddress = (UINT32)pBuffer;
    	 stream.length = len; 
      	 stream.mode = mciFormat;
	stream.channelNb = 1;
	stream.sampleRate = sampleRate;//HAL_AIF_FREQ_16000HZ
	stream.bitPerSample = bitPerSample;

	stream.handler =(APBS_USER_HANDLER_T) callback;

	g_mciRingFinishedCallback=NULL;
	g_audio_format = mciFormat;
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_104M);
    INT32 result;

      if(mciFormat == MCI_PLAY_MODE_MID)
      	{
        result = APBS_ERR_BAD_PARAMETER;
      	}
      else
      	{
	 result =  apbs_Play(audioItf, &stream, &apbsAudioCfg, TRUE);
      	}

	if (result != MCI_ERR_NO)
    {
        csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    }

    return result;
}


INT32  MCI_AUD_StreamPlay( INT32 *pBuffer, UINT32 len, MCI_PLAY_MODE_T mciFormat, APBS_STREAM_USER_HANDLER_T callback)
{
    APBS_ENC_STREAM_T stream;
    APBS_AUDIO_CFG_T apbsAudioCfg = 
        {
            .spkLevel = audio_cfg.spkLevel,
            .filter = NULL
        };


    stream.startAddress = (UINT32)pBuffer;
    stream.length = len; 
    stream.mode = mciFormat;
    stream.handler =(APBS_USER_HANDLER_T) callback;

    g_mciRingFinishedCallback=NULL;
    g_audio_format = mciFormat;

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_104M);

    INT32 result;
    if(mciFormat == MCI_PLAY_MODE_MID)
    {
        result = APBS_ERR_BAD_PARAMETER;
    }
    else
    {
        result = apbs_Play(audioItf, &stream, &apbsAudioCfg, TRUE);
    }

    if (result != MCI_ERR_NO)
    {
        csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    }

    return result;
}


INT32  MCI_AudioStopBuffer(void)
{
    INT32 result;
    if(g_audio_format==MCI_PLAY_MODE_MID)
    {
#ifndef TARGET_MIDI_SUPPORT_OFF	  	
        result = Audio_MidiStop();
#else
        result = APBS_ERR_NO;
#endif
    }
    else
    {
        result = apbs_Stop();
    }
    //result = AudioMp3StopStream();

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);

    return result;
}

UINT32 MCI_AudioPlayPause(VOID)
{
    INT32 result;
    if(g_audio_format==MCI_PLAY_MODE_MID)
    {
        result = APBS_ERR_BAD_PARAMETER;
    }
    else
    {
        result = apbs_Pause(TRUE);
    }

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);

    return result;
}

UINT32 MCI_AudioPlayResume(VOID)
{
    INT32 result;
    
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_104M);

    if(g_audio_format==MCI_PLAY_MODE_MID)
    {
        result = APBS_ERR_BAD_PARAMETER;
    }
    else
    {
        result = apbs_Pause(FALSE);
    }

    return result;
}

UINT32* MCI_GetBufPosition(VOID)
{
	return apbs_GetBufPosition();
}

void MCI_GetWriteBuffer( UINT32 **buffer, UINT32 *buf_len )
{
      apbs_GetWriteBuffer(buffer, buf_len);
}

	
UINT32 MCI_GetRemain(VOID)
{
       return apbs_GetRemain();
}
void MCI_DataFinished( void )
{
	apbs_DataFinished();
}
 INT32 MCI_AddedData(UINT32 addedDataBytes)
{
	return apbs_AddedData(addedDataBytes);
}

void  MCI_SetBuffer(UINT32 *buffer, uint32 buf_len)
{
	 apbs_SetBuffer(buffer, buf_len);
}

#ifndef TARGET_SOUND_RECORDER_SUPPORT_OFF
UINT32 MCI_AudioRecordStart (HANDLE fhd,mci_type_enum format,U8 quality, MCI_AUDIO_FILE_RECORD_CALLBACK_T callback)
{
    g_mciAudioRecordFinishedCallback=callback;

    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_104M);

    INT32 result = Mmc_sndRecStart(fhd,quality, format);
    if (result != MCI_ERR_NO)
    {
        csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    }

    return result;
}

UINT32 MCI_AudioRecordPause(VOID)
{
    INT32 result = Mmc_sndRecPause();
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    return result;
}
UINT32 MCI_AudioRecordResume(VOID)
{
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_104M);
    INT32 result = Mmc_sndRecResume();
    return result;
}
UINT32 MCI_AudioRecordStop(VOID)
{
    INT32 result = Mmc_sndRecStop();
    csw_SetResourceActivity(CSW_LP_RESOURCE_AUDIO_PLAYER, CSW_SYS_FREQ_32K);
    return result;
}
#endif

//FMRADIO START
UINT32 MCI_FM_RDA5800_init(uint8 scl,uint8 sda)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_close(VOID)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_open(UINT32 bUseLastFreq)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_tune(UINT32 iFrequency,UINT32 isStereo)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_mute(UINT32 isMute)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_setVolume(UINT32 iVol)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_seek(UINT32 isSeekUp)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_I2S(VOID)
{
    return 0;
}

UINT32 MCI_FM_RDA5800_STOPI2S(VOID)
{
    return 0;
}


VOID MCI_FmSetOutputDevice(MCI_AUDIO_PATH_T device)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
    AUD_SPK_T out = AUD_SPK_EAR_PIECE;
    if (device == MCI_PATH_HP || device == MCI_PATH_FM_HP)
    {
        out = AUD_SPK_EAR_PIECE;
    }
    else if (device == MCI_PATH_LSP || device == MCI_PATH_FM_LSP_HP)
    {
        out = AUD_SPK_LOUD_SPEAKER;
    }

    aud_SetCurOutputDevice(out);
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
}


extern uint32 pcmbuf_overlay[4608];

static BOOL g_FmBT8KEnbleFlag=0;

UINT32 MCI_FmPlay(void)
{
	AUD_ERR_T audError;
	HAL_AIF_STREAM_T stream;
	AUD_LEVEL_T level;

	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_FM]MCI_FmPlay!");
		
    level.spkLevel=AUD_SPK_MUTE; // MMI will set the volumn later
    level.sideLevel=0;
    level.micLevel=0;
    level.toneLevel=0;
    // Set audio interface 
    stream.startAddress = (UINT32*)pcmbuf_overlay;
    stream.length = 6400;

    // initialize the interrupt function.
    stream.halfHandler = NULL; 
    stream.endHandler = NULL;   

#if  (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
  
    stream.sampleRate = HAL_AIF_FREQ_48000HZ;
    stream.channelNb = HAL_AIF_STEREO;
    stream.voiceQuality = FALSE;
		
	hal_OverlayLoad(HAL_OVERLAY_INT_SRAM_ID_2);

	FMD_ERR_T fmdError = FMD_ERR_NO;
	FMD_I2S_CFG_T fmdI2sCfg={0};

	switch (stream.sampleRate)
	{
	case HAL_AIF_FREQ_8000HZ:		
  		fmdI2sCfg.freq=FMD_FREQ_8000HZ;		
		break;
			
	case HAL_AIF_FREQ_11025HZ:	
		fmdI2sCfg.freq=FMD_FREQ_11025HZ;		
		break;
		
	case HAL_AIF_FREQ_12000HZ:		
		fmdI2sCfg.freq=FMD_FREQ_12000HZ;		
		break;
		
	case HAL_AIF_FREQ_16000HZ:		
		fmdI2sCfg.freq=FMD_FREQ_16000HZ;		
		break;
		
	case HAL_AIF_FREQ_22050HZ:		
		fmdI2sCfg.freq=FMD_FREQ_22050HZ;		
		break;
		
	case HAL_AIF_FREQ_24000HZ:		
		fmdI2sCfg.freq=FMD_FREQ_24000HZ;		
		break;
		
	case HAL_AIF_FREQ_32000HZ:		
		fmdI2sCfg.freq=FMD_FREQ_32000HZ;		
		break;
		
	case HAL_AIF_FREQ_44100HZ:		
		fmdI2sCfg.freq=FMD_FREQ_44100HZ;		
		break;
		
	case HAL_AIF_FREQ_48000HZ:
		fmdI2sCfg.freq=FMD_FREQ_48000HZ;		
		break;			
	default:
		MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]Sample rate erro:%d",stream.sampleRate);
		break;
	}
       
	fmdI2sCfg.slave=TRUE;

    fmdError= fmd_I2sOpen( fmdI2sCfg);

	if (fmdError != FMD_ERR_NO)
	{		
		MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen FAILED");
		return AUD_ERR_RESOURCE_BUSY;
	}
	else
	{
		MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen SUCCESS");
	}
#endif // (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)

#if  (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

     if(g_FmBT8KEnbleFlag==1)
     	{
	        stream.sampleRate = HAL_AIF_FREQ_8000HZ;
	        stream.channelNb = HAL_AIF_MONO;
	        stream.voiceQuality = TRUE;
			
		hal_OverlayLoad(HAL_OVERLAY_INT_SRAM_ID_2);

		FMD_ERR_T fmdError = FMD_ERR_NO;
		FMD_I2S_CFG_T fmdI2sCfg={0};

		fmdI2sCfg.freq=FMD_FREQ_8000HZ;	

	       fmdError= fmd_I2sOpen( fmdI2sCfg);

		if (fmdError != FMD_ERR_NO)
		{		
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen FAILED");
			return AUD_ERR_RESOURCE_BUSY;
		}
		else
		{
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen SUCCESS");
		}
     	}
	 else
	 {
	        stream.sampleRate = HAL_AIF_FREQ_32000HZ;
	        stream.channelNb = HAL_AIF_STEREO;
	        stream.voiceQuality = FALSE;
			
		hal_OverlayLoad(HAL_OVERLAY_INT_SRAM_ID_2);

		FMD_ERR_T fmdError = FMD_ERR_NO;
		FMD_I2S_CFG_T fmdI2sCfg={0};

		switch (stream.sampleRate)
		{
		case HAL_AIF_FREQ_8000HZ:		
	  		fmdI2sCfg.freq=FMD_FREQ_8000HZ;		
			break;
				
		case HAL_AIF_FREQ_11025HZ:	
			fmdI2sCfg.freq=FMD_FREQ_11025HZ;		
			break;
			
		case HAL_AIF_FREQ_12000HZ:		
			fmdI2sCfg.freq=FMD_FREQ_12000HZ;		
			break;
			
		case HAL_AIF_FREQ_16000HZ:		
			fmdI2sCfg.freq=FMD_FREQ_16000HZ;		
			break;
			
		case HAL_AIF_FREQ_22050HZ:		
			fmdI2sCfg.freq=FMD_FREQ_22050HZ;		
			break;
			
		case HAL_AIF_FREQ_24000HZ:		
			fmdI2sCfg.freq=FMD_FREQ_24000HZ;		
			break;
			
		case HAL_AIF_FREQ_32000HZ:		
			fmdI2sCfg.freq=FMD_FREQ_32000HZ;		
			break;
			
		case HAL_AIF_FREQ_44100HZ:		 
			fmdI2sCfg.freq=FMD_FREQ_44100HZ;		
			break;
			
		case HAL_AIF_FREQ_48000HZ:
			fmdI2sCfg.freq=FMD_FREQ_48000HZ;		
			break;			
		default:
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]Sample rate erro:%d",stream.sampleRate);
			break;
		}
	       
		fmdI2sCfg.slave=TRUE;

	       fmdError= fmd_I2sOpen( fmdI2sCfg);

		if (fmdError != FMD_ERR_NO)
		{		
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen FAILED");
			return AUD_ERR_RESOURCE_BUSY;
		}
		else
		{
			MCI_TRACE(MCI_AUDIO_TRC, 0,     "[MCI]fmd_I2sOpen SUCCESS");
		}


	 }
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

	audioItf = AUD_ITF_FM;

	audError= aud_StreamStart(audioItf, &stream, &level);

	if (audError != AUD_ERR_NO)
	{
		MCI_TRACE(MCI_AUDIO_TRC, 0,   "[MCI]aud_FmStreamStart FAILED");
	}
	else
	{
		MCI_TRACE(MCI_AUDIO_TRC, 0,   "[MCI]aud_FmStreamStart SUCCESS");
	}

	return audError;
}

extern U8 GetHandsetInPhone(VOID);

UINT32 MCI_FmStop(VOID)
{
    AUD_ERR_T audError = AUD_ERR_NO;

    MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_FM]MCI_FmStop!audioItf = %d",audioItf);
 

    while((audError=aud_StreamStop(audioItf)) != AUD_ERR_NO)
    {
        COS_Sleep(100);
    }

    // Directly call MMI API GetHandsetInPhone() here.
    // In the future audioItf will be managed only in CSW.
    if (1 == GetHandsetInPhone())
    {
	    audioItf = AUD_ITF_EAR_PIECE;
    }
    else
    {
        audioItf = AUD_ITF_RECEIVER;
    }

//#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
    fmd_I2sClose();
    hal_OverlayUnload(HAL_OVERLAY_INT_SRAM_ID_2);
//#endif // (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)

    return audError;		
}


UINT32 MCI_FmSetup(UINT8 volume )
{
	AUD_ERR_T audError = AUD_ERR_NO;
	AUD_LEVEL_T cfg;

	if(volume>7)volume=7;

	

	cfg.spkLevel=volume;
	cfg.micLevel=0;
	cfg.sideLevel=0;
	cfg.toneLevel=0;

	

	audError=aud_Setup(audioItf,  & cfg);

	return audError;
}


//fmradio record
UINT32 MCI_FmrRecordStart (char *file_name_p)   //  MCI_MEDIA_PLAY_REQ,         
{
    return 0;
}
UINT32 MCI_FmrRecordPause(VOID)
{
    return 0;
}
UINT32 MCI_FmrRecordResume(VOID)
{
    return 0;
}
UINT32 MCI_FmrRecordStop(VOID)
{
    return 0;
}

/**************************************************************************/
/**************************************************************************/
/******************Video function API   ***************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*
    MCI_VID_CLOSE_REQ,          // 7
        MCI_VID_OPEN_FILE_REQ,      // 8
        MCI_VID_FILE_READY_IND,     // 9 
        MCI_VID_PLAY_REQ,               // 10
        MCI_VID_PLAY_FINISH_IND,        // 11 
        MCI_VID_STOP_REQ,               // 12
        MCI_VID_PAUSE_REQ,          // 13
        MCI_VID_RESUME_REQ,         // 14
        MCI_VID_SEEK_REQ,               // 15
        MCI_VID_SEEK_DONE_IND,
*/

#ifndef TARGET_VIDEO_PLAYER_SUPPORT_OFF
UINT32  MCI_DisplayVideoInterface (VOID)
{
    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_DISPLAY_INTERFACE_REQ);
    return 0;
}

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)

UINT32  MCI_VideoClose (VOID)   //MCI_VID_CLOSE_REQ
{   
    INT32 result;
    result = LILY_VideoClose();
    return result;

}

 UINT32 MCI_VideoOpenFile (INT32 OutputPath, uint8 open_audio, HANDLE fhd, UINT8 file_mode, UINT32 data_Len, mci_type_enum type, void(*vid_play_finish_callback)(int32), void (*vid_draw_panel_callback)(uint32))   //MCI_VID_OPEN_FILE_REQ
 {  

    INT32 result=0;
    //INT32 i;
    
    
    result     = LILY_VideoOpenFile( fhd, file_mode, data_Len, OutputPath,open_audio, type, vid_play_finish_callback, vid_draw_panel_callback);
#if 0
    if (result == 0)
    {
        mci_vid_file_ready_ind_struct msg;
        media_vid_file_ready_ind_struct *ind_p=NULL ;
        ilm_struct       *local_ilm_ptr = NULL;
        LILY_VideoGetInfo(&msg);
           ind_p = (media_vid_file_ready_ind_struct*)
                    construct_local_para(sizeof(media_vid_file_ready_ind_struct), TD_CTRL);  
           if(!ind_p)return 1;
           ind_p->result = (int16)result;
           ind_p->image_width=msg.image_width;
           ind_p->image_height=msg.image_height;
           ind_p->total_frame_num=msg.total_frame_num;
           ind_p->total_time=msg.total_time;
           ind_p->seq_num=msg.seq_num;
           ind_p->media_type =msg.media_type;

           local_ilm_ptr = allocate_ilm(MOD_MED);
           if(!local_ilm_ptr)return 1;
           local_ilm_ptr->src_mod_id = MOD_MED;
           local_ilm_ptr->dest_mod_id = MOD_MMI;
           local_ilm_ptr->sap_id = MED_SAP;
           
           local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_FILE_READY_IND;
           local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
           local_ilm_ptr->peer_buff_ptr = NULL;

           i=0;
           while(!mmc_SendMsg(GetMmiTaskHandle(local_ilm_ptr->dest_mod_id),local_ilm_ptr->msg_id, (uint32)local_ilm_ptr, 0, 0))
           {
            i++;
            diag_printf( "COS_SendEvent 2 MMI fail i=%d",i);
            COS_Sleep(20);//ms
            if(i>=3)
            {
                result=1;
                break;
            }
           }

    }
#endif

    return result;
}

UINT32 MCI_VideoPlayFinishInd (mci_vid_play_finish_ind_struct *plyFinish)   
{
    return 0;
}


UINT32  MCI_VideoGetPlayTime (VOID)  
{   
    mci_vid_file_ready_ind_struct fileInfo;
    LILY_VideoGetInfo(&fileInfo);
    return fileInfo.current_time;
}

VOID  MCI_VideoGetInfo ( UINT16  *image_width, UINT16  *image_height, UINT32  *total_time, UINT16  *aud_channel, UINT16  *aud_sample_rate,UINT16  *track)  
{   
    mci_vid_file_ready_ind_struct fileInfo;
    LILY_VideoGetInfo(&fileInfo);
    *image_width=fileInfo.image_width;
    *image_height=fileInfo.image_height;
    *total_time=fileInfo.total_time;
   *aud_channel = fileInfo.aud_channel;
   *aud_sample_rate = fileInfo.aud_sample_rate;
   *track = fileInfo.track;
    return ;
}


UINT32 MCI_VideoRecordFinishInd (INT32 finishCause)   
{
    return 0;
}

UINT32 MCI_VideoSeekDoneInd (mci_vid_seek_done_ind_struct *pSeekDone)   
{
    return 0;
}  

 VOID MCI_VideoOpenFileInd (mci_vid_file_ready_ind_struct *vdoOpen)    //MCI_VID_FILE_READY_IND
 {
    return;
}
 
 UINT32 MCI_VideoPlay (UINT16 startX, UINT16 startY)   //MCI_VID_PLAY_REQ
{       
    INT32 result;
    result = LILY_VideoPlay(startX,startY);
    return result;
}
 
VOID  MCI_VideoPlayInd (VOID)   // MCI_VID_PLAY_FINISH_IND
{
    return;
}

 
 UINT32 MCI_VideoStop (VOID)    // MCI_VID_STOP_REQ
{
    INT32 result;
    result = LILY_VideoStop();
    return result;
}
 
UINT32  MCI_VideoPause (VOID)  //  MCI_VID_PAUSE_REQ
{
    INT32 result;
    result = LILY_VideoPause();
    return result;
}
 
UINT32  MCI_VideoResume (VOID)  // MCI_VID_RESUME_REQ
{       
    INT32 result;
    result = LILY_VideoResume();
    return result;

}
 
 UINT32 MCI_VideoSeek (long long playtime, INT32 time_mode, INT16 startX, INT16 startY)   // MCI_VID_SEEK_REQ
  { 
    INT32 result;
    //media_vid_seek_done_ind_struct *ind_p=NULL;
    //ilm_struct       *local_ilm_ptr = NULL;
    //INT32 i;
    result    = LILY_VideoSeek(playtime, time_mode, startX, startY);
#if 0
    ind_p = (media_vid_seek_done_ind_struct*)
        construct_local_para(sizeof(media_vid_seek_done_ind_struct), TD_CTRL);  
    if(!ind_p)return 1;
    ind_p->result = (int16)result;
    local_ilm_ptr = allocate_ilm(MOD_MED);
    if(!local_ilm_ptr)return 1;
    local_ilm_ptr->src_mod_id = MOD_MED;
    local_ilm_ptr->dest_mod_id = MOD_MMI;
    ind_p->seq_num = 0;
    local_ilm_ptr->sap_id = MED_SAP;

    local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_SEEK_DONE_IND;
    local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
    local_ilm_ptr->peer_buff_ptr = NULL;

       i=0;
   while(!mmc_SendMsg(GetMmiTaskHandle(local_ilm_ptr->dest_mod_id),local_ilm_ptr->msg_id, (uint32)local_ilm_ptr, 0, 0))
     {
        i++;
        diag_printf( "COS_SendEvent 2 MMI fail i=%d",i);
        COS_Sleep(20);//ms
        if(i>=3)
        {
            result=1;
            break;
        }
       }
#endif       
    return result;
}
 
VOID  MCI_VideoSeekInd (VOID)  // MCI_VID_SEEK_DONE_IND
{
    return;
}

UINT32 MCI_VideoSet (UINT16 ZoomWidth, UINT16 ZoomHeight, INT16 startX, INT16 startY, INT16 cutX, INT16 cutY, INT16 cutW, INT16 cutH, UINT16 Rotate)  // MCI_VID_SET_MODE_REQ
{
	mci_vid_set_mode_ind_struct SetMode;
	UINT32 ret;
	SetMode.lcd_start_x = startX;
	SetMode.lcd_start_y = startY;
	SetMode.zoom_height = ZoomHeight;
	SetMode.zoom_width = ZoomWidth;
	SetMode.cutX= cutX;
	SetMode.cutY= cutY;
	SetMode.cutW= cutW;
	SetMode.cutH= cutH;
	SetMode.rotate=Rotate;
    ret = LILY_VideoSetInfo(&SetMode);
	return ret;
}

#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */
#endif


/**************************************************************************/
/**************************************************************************/
/******************Camera function API***************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*
    MCI_CAM_POWER_UP_REQ,       // 17
        MCI_CAM_POWER_DOWN_REQ, // 18
        MCI_CAM_PREVIEW_REQ,            // 19
        MCI_CAM_STOP_REQ,               // 20
        MCI_CAM_CAPTURE_REQ,            // 21
        MCI_CAM_SET_PARAM_REQ,      // 22
*/

#if (CSW_EXTENDED_API_CAMERA == 0)

INT32 cam_md_state=0;//0=close,1=open,2=need reopen

UINT32 MCI_CamPowerUp (INT32  vediomode,void (*cb)(int32))   // MCI_CAM_POWER_UP_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPowerUp.vediomode%d",vediomode);

	if(cam_md_state)
	{
		MCI_CamMdClose();
		//only close and not reopen again.
		cam_md_state=0;//reopen need keep 2 parameters.
	}
		
	return Lily_Camera_PowerOn(vediomode,cb);
}

UINT32 MCI_CamPowerDown (VOID)  //MCI_CAM_POWER_DOWN_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPowerDown");
	return Lily_Camera_PowerOff();
}

UINT32 MCI_CamPlayBack ()  //MCI_CAM_PLAY_BACK
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPlayBack");
	
	return Lily_Capture_Preview();
}

INT32  MCI_CamSavePhoto (INT32  filehandle)  //MCI_CAM_SAVE_PHOTO
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamSavePhoto.filehandle:%d",filehandle);
	return Lily_camera_savePhoto(filehandle);
}

UINT32 MCI_CamPreviewOpen(CAM_PREVIEW_STRUCT* data)  // MCI_CAM_PREVIEW_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPreviewOpen");
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]start_x:%d",data->start_x);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]start_y:%d",data->start_y);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]end_x:%d",data->end_x);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]end_y:%d",data->end_y);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]image_width:%d",data->image_width);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]image_height:%d",data->image_height);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]nightmode:%d",data->nightmode);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]imageQuality:%d",data->imageQuality);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]factor:%d",data->factor);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]contrast:%d",data->contrast);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]specialEffect:%d",data->specialEffect);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]brightNess:%d",data->brightNess);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]whiteBlance:%d",data->whiteBlance);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]exposure:%d",data->exposure);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]addFrame:%d",data->addFrame);
	
	return Lily_Camera_Preview(data);
}

UINT32 MCI_CamPreviewClose(VOID)  // MCI_CAM_STOP_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamPreviewClose");
	return Lily_Camera_PreviewOff();
}

 UINT32 MCI_CamCapture (CAM_CAPTURE_STRUCT *data)    //MCI_CAM_CAPTURE_REQ
 {
 	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamCapture");
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]image_width:%d",data->image_width);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]image_height:%d",data->image_height);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]media_mode:%d",data->media_mode);

    	return Lily_Camera_Capture(data);
 }

 UINT32 MCI_CamSetPara(INT32 effectCode,INT32 value)  // MCI_CAM_SET_PARAM_REQ
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]MCI_CamSetPara ");
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]effectCode:%d",effectCode);
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,"[MCI_CAMERA]value:%d",value);
	
	return  Lily_Camera_SetEffect(effectCode,value);
}

UINT32 MCI_CamCancelLastSave(VOID)  // MCI_CAM_PREVIEW_REQ
{
    return 0;
}

//open camera motion detect
INT32 MCI_CamMdOpen(INT32 sensLevel, VOID(*md_callback)(int32))
{
	if( motion_detect_open( sensLevel, md_callback)==0)
	{
		cam_md_state=1;
		return 0;
	}
	else
	{
		cam_md_state=0;
		return -1;
	}
}
//close camera motion detect
INT32 MCI_CamMdClose( VOID)
{
	cam_md_state=0;
	return motion_detect_close();
}

VOID MCI_CamDoMd( VOID)
{
	do_motion_detect();
}



#endif /* CSW_EXTENDED_API_CAMERA */

/**************************************************************************/
/**************************************************************************/
/******************Image  function API***************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*
    MCI_IMG_DECODE_REQ,         // 23 
    MCI_IMG_DECODE_EVENT_IND,   // 24
    MCI_IMG_DECODE_FINISH_IND,  // 25
    MCI_IMG_RESIZE_REQ,         // 26
    MCI_IMG_STOP_REQ,               // 27
*/


UINT32 MCI_ImgCheckDecodeState(VOID)
{
    return 0;
}

UINT32 MCI_ImgResize (img_decode_req_struct *imgPara)  // MCI_IMG_RESIZE_REQ
{
    return 0;
}
UINT32 MCI_ImgStop (VOID)  //MCI_IMG_STOP_REQ
{
    return 0;
}


INT32 MCI_ImgDisplay(uint8 startx,uint8 starty, uint8 width,uint8 height,char * filename)  // MCI_DI_IMAGE_DISPLAYSTILL_IND
{
    return 0;
}


UINT32 MCI_getLilyStatus(UINT32 cmd_arg)
{
    return 0;
}

#ifndef TARGET_VIDEO_RECORDER_SUPPORT_OFF
UINT32 MCI_VideoRecordPreviewStart (MMC_VDOREC_SETTING_STRUCT *previewPara, VOID(*vid_rec_finish_ind)(uint16 msg_result))
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordPreviewStart!");
	return LILY_VidrecPreviewStartReq(previewPara,vid_rec_finish_ind);   
}

UINT32 MCI_VideoRecordAdjustSetting (INT32 adjustItem, INT32 value)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordAdjustSetting!");
	return LILY_VidrecAdjustSettingReq(adjustItem, value);
}

UINT32 MCI_VideoRecordPreviewStop(VOID)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordPreviewStop!");
	return LILY_VidrecPreviewStopReq();
}
    
UINT32 MCI_VideoRecordStart(HANDLE filehandle)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordStart!");
	return LILY_VidrecRecordStartReq(filehandle);
}

UINT32 MCI_VideoRecordStop( VOID)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordStop!");
	return LILY_VidrecRecordStopReq();
}

UINT32 MCI_VideoRecordPause(VOID)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordPause!");
	return LILY_VidrecRecordPauseReq();
}

UINT32 MCI_VideoRecordResume( VOID)
{
	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MCI_VIDREC]MCI_VideoRecordResume!");
	return LILY_VidrecRecordResumeReq();
}

UINT32 MCI_VideoRecordSound(INT32 *PCMdata, UINT32 length)
{
    return 0;
}
#endif



