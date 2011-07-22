#include "cswtype.h"
#include "cs_types.h"
#include "cpu_share.h"
#include "string.h"

#include "mmc.h"
#include "mmc_audiompeg.h"
#include "mmc_audioamr.h"
//#include "os_api.h"
//#include "mmc_audiowma.h"
#include "mmc_audiowav.h"
//#include "cpu_comm.h"
#include "mcip_debug.h"
//#include "mmc_video.h"
#include "mmc_isom_video.h"
#include "mmc_camera.h"
//#include "mmc_audiowma.h"
//#include "mmc_image.h"
//#include <cyg/kernel/kapi.h> //chenhe for semphore struct
#include "video_api.h"
#include "mmc_midi.h"
//#include"stack_config.h"
//#include"cos.h"
#include "sxs_type.h"
#include "sxr_sbx.h"
#include "mmc_videorecord.h"
#include "mmc_sndrec.h"
#include "hal_timers.h"

#include "mmc_analogtv.h"
#include "apbs_m.h"

#include "mmc_vdochat.h"
/* mmc task init */
extern VOID MMCTask(uint32 data);
extern VOID ImageTask(uint32 data);


extern BOOL SUL_ZeroMemory32(
                      VOID*  pBuf,
                      UINT32 count
                      );

#define MMC_TASK_STACK_SIZE 8 * 1024
#define IMAGE_TASK_STACK_SIZE 4*1024


#if 0
const char gMmcTaskStack[MMC_TASK_STACK_SIZE];
//char gImageTaskStack[IMAGE_TASK_STACK_SIZE];


const TASKINFO gMmcTaskInfo = {MMCTask, gMmcTaskStack, MMC_TASK_STACK_SIZE, 7, "MMC"};
//const TASKINFO gImageTaskInfo = {ImageTask,gImageTaskStack,IMAGE_TASK_STACK_SIZE,9,"IMAGE"};

void Mmc_ServerStart()
{
HANDLE hTask;
// hTask=COS_TaskCreate(&gMmcTaskInfo);//zhangl
hTask=COS_CreateTask((PTASK_ENTRY)gMmcTaskInfo.Entry, NULL, NULL, gMmcTaskInfo.StackSize, (UINT8)gMmcTaskInfo.Priority, COS_CREATE_DEFAULT, 0, (PCSTR)gMmcTaskInfo.Name);
 diag_printf( "*******************Mmc_ServerStart:%d**********************",hTask);
	SetMmiTaskHandle(MBOX_ID_MMC, hTask);
  //COS_TaskCreate(&gImageTaskInfo);  
  return ;
}
#endif
/*#######mmc data tran module#######*/



extern volatile BOOL interrupt_gif_play;
extern volatile BOOL gif_paly_in;
extern volatile BOOL gif_first_entry;
HANDLE camCapSem;//chenhe for capture



uint32 gnMmcMode=MMC_MODE_IDLE;
static mmcModeStatus gMmcModeStatus=NoneStatus;//JIASHUO
Audio_Selector gAudio;
#ifndef TARGET_VIDEO_PLAYER_SUPPORT_OFF
Video_Selector gVideo;
#endif

#ifndef TARGET_SOUND_RECORDER_SUPPORT_OFF
Record_Selector gRecord;
#endif
extern char g_FileName[];

#ifndef TARGET_SOUND_RECORDER_SUPPORT_OFF
void mmc_RecordInstall(int32 RecordType)
{
#if 0
  switch (RecordType)
  {
  case MCI_TYPE_DAF:
    diag_printf("install record mp3 message process function\n");
    break;
  case MMC_MEDIA_TYPE_AMR:
    diag_printf("install record amr message process function\n");
    break;
  default:
    ass(1);
    break;
  }
  #endif
}

void mmc_RecordUninstall()
{
  gRecord.Start   = 0;
  gRecord.Stop    = 0;
  gRecord.Pause   = 0;
  gRecord.Resume  = 0;
  gRecord.UserMsg = 0;
  return;
}
#endif

#ifndef TARGET_VIDEO_PLAYER_SUPPORT_OFF
void mmc_VideoInstall(int32 VideoType)
{
  switch (VideoType)
  {
  case MCI_TYPE_MJPG:
  /*
  gVideo.Open   = Video_MjpegOpen;    
    gVideo.Play   = Video_MjpegPlay;
    gVideo.Stop   = Video_MjpegStop;
    gVideo.Close= Video_MjpegClose;
    gVideo.Pause  = Video_MjpegPause;
    gVideo.Resume = Video_MjpegResume;
    gVideo.Seek   = Video_MjpegSeek;*/
   // gVideo.UserMsg = Video_MjpegUserMsg; 
   // break;
#ifdef MP4_3GP_SUPPORT
  case MCI_TYPE_MP4:
  case MCI_TYPE_3GP:
  case MCI_TYPE_RM:
  	 gVideo.UserMsg = Video_IsomUserMsg;
  	break;
#endif
  default:
    ass(1);
    break;
  }
}
 void mmc_VideoUninstall()
{
    gVideo.Open  = 0;
    gVideo.Play   = 0;
    gVideo.Stop   = 0;
    gVideo.Close= 0;
    gVideo.Pause  = 0;
    gVideo.Resume = 0;
    gVideo.Seek = 0;
    gVideo.UserMsg = 0;
    return ;
}
 #endif
 
void mmc_AudioInstall(int32 AudioType)
{
  switch (AudioType)
  {
/*  case MCI_TYPE_WMA:
    gAudio.Play   = Audio_WmaPlay;
    gAudio.Stop   = Audio_WmaStop;
    gAudio.Pause  = Audio_WmaPause;
    gAudio.Resume = Audio_WmaResume;
    gAudio.GetID3 = Audio_WmaGetID3;
    gAudio.UserMsg = Audio_WmaUserMsg;
    break; */
	
  case MCI_TYPE_AMR:
	gAudio.Play	= Audio_AmrPlay;
	gAudio.Stop	= Audio_AmrStop;
	gAudio.Pause = Audio_AmrPause;
	gAudio.Resume = Audio_AmrResume;
	gAudio.GetID3 = Audio_AmrGetID3;
	gAudio.UserMsg = Audio_AmrUserMsg;
	gAudio.GetPlayInformation  = Audio_AmrGetPlayInformation;
	break;      
  case MCI_TYPE_DAF:
  case MCI_TYPE_AAC:
    gAudio.Play   = Audio_MpegPlay;
    gAudio.Stop   = Audio_MpegStop;
    gAudio.Pause  = Audio_MpegPause;
    gAudio.Resume = Audio_MpegResume;
    gAudio.GetID3 = Audio_MpegGetID3;
    gAudio.UserMsg = Audio_MpegUserMsg;
    gAudio.GetPlayInformation  = Audio_MpegGetPlayInformation;
    break;
	
  case MCI_TYPE_WAV:
    gAudio.Play   = Audio_WavPlay;
    gAudio.Stop   = Audio_WavStop;
    gAudio.Pause  = Audio_WavPause;
    gAudio.Resume = Audio_WavResume;
    gAudio.GetID3 = Audio_WavGetID3;
    gAudio.UserMsg = Audio_WavUserMsg;
    gAudio.GetPlayInformation  = Audio_WavGetPlayInformation;
	break; 
	
#ifndef TARGET_MIDI_SUPPORT_OFF
  case MCI_TYPE_MIDI:
    gAudio.Play   = Audio_MidiPlay;
    gAudio.Stop   = Audio_MidiStop;
    gAudio.Pause  = Audio_MidiPause;
    gAudio.Resume = Audio_MidiResume;
    gAudio.GetID3 =  Audio_MidiGetID3;
    gAudio.UserMsg = Audio_MidiUserMsg;
    gAudio.GetPlayInformation  = Audio_MidiGetPlayInformation;
   break;
#endif
  default :
    ass(1);
    break;
  }
}
void mmc_AduioUninstall()
{
    gAudio.Play   = 0;
    gAudio.Stop   = 0;
    gAudio.Pause  = 0;
    gAudio.Resume = 0;
    gAudio.GetID3 = 0;
    gAudio.UserMsg = 0;
    return ;
}


void mmc_EntryMode(int32 mode, int32 arg)
{
  if (gnMmcMode != 0)
  {
      diag_printf("##### WARNING now stat is't idle \n");      
  }
  switch (mode)
  {
    case MMC_MODE_AUDIO:
      mmc_AudioInstall(arg);
      diag_printf("now entry <AUDIO> stat \n");
      break;
#ifndef TARGET_VIDEO_PLAYER_SUPPORT_OFF
    case MMC_MODE_VIDEO:
      mmc_VideoInstall(arg);      
      diag_printf("now entry <VIDIO> stat \n");
      break;
#endif	  
#ifndef TARGET_SOUND_RECORDER_SUPPORT_OFF
    case MMC_MODE_RECORD:
      mmc_RecordInstall(arg);      
      diag_printf("now entry <RECORD> stat \n");
      break;
#endif	  
    case MMC_MODE_VIDREC:
      diag_printf("now entry <VIDREC> stat \n");
      break;
    case MMC_MODE_CAMERA:
      diag_printf("now entry <CAMERA> stat \n");
      break;
    case MMC_MODE_IMAGE:
      diag_printf("now entry <IMAGE> stat \n");
      break;
    case MMC_MODE_INVALID:
      ass(1);
      break;
  }
  gnMmcMode = mode;
}
void mmc_ExitMode()
{
  switch (gnMmcMode)
  {
    case MMC_MODE_AUDIO:
      mmc_AduioUninstall();
      diag_printf(" now from <AUDIO> stat exit\n");
      break;
#ifndef TARGET_VIDEO_PLAYER_SUPPORT_OFF
    case MMC_MODE_VIDEO:
      mmc_VideoUninstall();      
      diag_printf("now from <VIDIO> stat exit\n");
      break;
#endif	  
#ifndef TARGET_SOUND_RECORDER_SUPPORT_OFF
    case MMC_MODE_RECORD:
      mmc_RecordUninstall();      
      diag_printf("now from <RECORD> stat exit\n");
      break;
#endif	  
    case MMC_MODE_VIDREC:
      diag_printf("now from <VIDREC> stat exit\n");
      break;
    case MMC_MODE_CAMERA:
	diag_printf("now from <CAMERA> stat exit\n");
      break;
    case MMC_MODE_IMAGE:
      diag_printf("now from <IMAGE> stat exit\n");
      break;
    case MMC_MODE_INVALID:
      diag_printf("##### WARNING now from <INVALID> stat exit\n");      
      break;
      
  }
  gnMmcMode = MMC_MODE_IDLE;
  mmc_MemFreeAll();
  return;
}

int32 mmc_SetCurrMode(int32 mode)
{
	gnMmcMode = mode;
	return 0;
}

int32 mmc_GetCurrMode()
{
  return gnMmcMode;
}

/* audio module */

/* end audio module */

void mmc_SendResult(uint32 nEvent, int32 result)
{
 // EVENT_Send2Jade(CHAN_MMC_ID, X_RESULT_MSG, EVENT_GetCommand(nEvent), result, 0);
  return ;
}
//extern vid_context_struct    *vid_context_p;
void mmc_vid_send_play_finish_ind(int32 result)
{
	mci_vid_play_finish_ind_struct plyFinish;
	//vid_context_p->current_time=0;
	plyFinish.result = result;
	diag_printf("lily send MCI_VID_PLAY_FINISH_IND result = %d \n",plyFinish.result);
	//xmsg_printf(CHAN_MMC_ID, MCI_VID_PLAY_FINISH_IND, "%t[%d]", &plyFinish,sizeof(mci_vid_play_finish_ind_struct));      
}

void mmc_vid_send_seek_done_ind(int32 result)
{
	mci_vid_seek_done_ind_struct seekDone;

	seekDone.result = result;
	diag_printf("lily send MCI_VID_SEEK_DONE_IND result = %d \n",seekDone.result);
	//xmsg_printf(CHAN_MMC_ID, MCI_VID_SEEK_DONE_IND, "%t[%d]", &seekDone,sizeof(mci_vid_seek_done_ind_struct));      
}
void mmc_SendFinishedInd()
{
//  EVENT_Send2Jade(CHAN_MMC_ID, MCI_MEDIA_PLAY_FINISH_IND, 0, 0, 0);
}

VOID mmc_SetModeStatus(mmcModeStatus status)
{
	gMmcModeStatus = status;
}

uint32 mmc_CheckModeStatus(int32 Msg)
{
	diag_printf("[MMC]######gnMmcMode:%d,gMmcModeStatus:%d,Msg:%d\n",gnMmcMode,gMmcModeStatus,Msg);
	switch(gnMmcMode)
	{
	    case MMC_MODE_IDLE:
			switch(Msg)
			{
			   case MCI_MEDIA_PLAY_REQ:
			   case MCI_MEDIA_GETID3_REQ:
			   case MCI_VID_OPEN_FILE_REQ:
			   	diag_printf("guoj++ lily/mmc_CheckModeStatus \n");
			   case MCI_RECORD_START_REQ:
			   case MCI_CAM_POWER_UP_REQ:
			   case MCI_REC_VIDEO_PREVIEW_START_REQ:
			   case MCI_IMG_DECODE_REQ:
			   case MCI_IMG_RESIZE_REQ:
			   case MCI_IMG_DISPLAY_IND:
			   case MCI_IMG_STOP_REQ:
				 break;
			   default:
			   	 return MMC_RECEIVE_WRONG_COMMAND;
			}
			break;
			
		case MMC_MODE_AUDIO:
			if(gMmcModeStatus==AudioPlayStatus)
			{
				switch(Msg)
				{
				  	case MCI_MEDIA_STOP_REQ:			
					case MCI_MEDIA_PAUSE_REQ:
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}
			else if(gMmcModeStatus==AudioPauseStatus)		
			{
				switch(Msg)
				{
				  	case MCI_MEDIA_STOP_REQ:			
					case MCI_MEDIA_RESUME_REQ:
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}
			break;
			
		case MMC_MODE_VIDEO:
			if(gMmcModeStatus==VideoPlayStatus)
			{
				switch(Msg)
				{
				  	case MCI_VID_STOP_REQ:			
					case MCI_VID_PAUSE_REQ:
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}
			else if(gMmcModeStatus==VideoPauseStatus)		
			{
				switch(Msg)
				{
				  	case MCI_VID_STOP_REQ:			
					case MCI_VID_RESUME_REQ:
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}			
			break;
			
		case MMC_MODE_RECORD:
			if(gMmcModeStatus==AudioRecordStatus)
			{
				switch(Msg)
				{
				  	case MCI_RECORD_STOP_REQ:			
					case MCI_RECORD_PAUSE_REQ:
					case REC_PCMDATA_IND:
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}
			else if(gMmcModeStatus==AudioRecordPauseStatus)		
			{
				switch(Msg)
				{
				  	case MCI_RECORD_STOP_REQ:			
					case MCI_RECORD_RESUME_REQ:
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}			
			break;
			
		case MMC_MODE_CAMERA:
			if(gMmcModeStatus==CameraPreviewStatus)
			{
				switch(Msg)
				{
				  	case MCI_CAM_CAPTURE_REQ:			
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}

			break;
			
		case MMC_MODE_VIDREC:
			if(gMmcModeStatus==VidrecPreviewStatus)
			{
				switch(Msg)
				{
				  	case MCI_REC_VIDEO_START_REQ:			
					case MCI_REC_VIDEO_STOP_REQ:
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}
			else if(gMmcModeStatus==VidrecRecordStatus)		
			{
				switch(Msg)
				{
				  	case MCI_REC_VIDEO_STOP_REQ:			
					case MCI_REC_VIDEO_PAUSE_REQ:
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}
			else if(gMmcModeStatus==VidrecPauseStatus)		
			{
				switch(Msg)
				{
				  	case MCI_REC_VIDEO_STOP_REQ:			
					case MCI_REC_VIDEO_RESUME_REQ:
					  break;
					default:
					  return MMC_RECEIVE_WRONG_COMMAND;
				}
			}
			break;
			
		case MMC_MODE_IMAGE:
			break;
			
		default:
			break;
	}	
	return 1;
}

#if 0

VOID 	mmc_VideoStateProcess(uint32 nEvent)
{
  int32 result;
  uint32 videotype;
  if (EVENT_GetType(nEvent) == EVENT_TYPE_JADE) /* process X msg */
  {
    switch (EVENT_GetCommand(nEvent))
    {
      case MCI_VID_OPEN_FILE_REQ:
	  	diag_printf("guoj++ lily/mmc_VideoStateProcess \n");
	  videotype = mmc_GetMediaTypeByFilename(EVENT_GetDataAddr(nEvent));
        mmc_EntryMode(MMC_MODE_VIDEO, videotype);
	gVideo.Open(EVENT_GetCmdArg(nEvent,0),EVENT_GetCmdArg(nEvent,1),
		EVENT_GetDataAddr(nEvent));
        break;
      case MCI_VID_SEEK_REQ:
	  gVideo.Seek(EVENT_GetCmdArg(nEvent,0), EVENT_GetCmdArg(nEvent,1));
	  break;
      case MCI_VID_PLAY_REQ:
	  gVideo.Play();
        break;
      case MCI_VID_STOP_REQ:
      	gVideo.Stop();
        break;
      case MCI_VID_CLOSE_REQ:
	  gVideo.Close();
	  mmc_ExitMode();
        break;
      case MCI_VID_PAUSE_REQ:
	  gVideo.Pause();
        break;
      case MCI_VID_RESUME_REQ:
	  gVideo.Resume();
        break;
	default:
        break;
    }
    mmc_SendResult(nEvent,result);
    EVENT_Free(nEvent);
  }
  else                   /* process custom msg */
  {
    gVideo.UserMsg(EVENT_GetCommand(nEvent));       
  }
}
VOID 	mmc_CameraStateProcess(int32 nEvent)
{
	  int32 result;

	if (EVENT_GetType(nEvent) == EVENT_TYPE_JADE) /* process X msg */
	{
		switch(EVENT_GetCommand(nEvent)){
		case MCI_CAM_POWER_UP_REQ:
			//we have to init voc at first;then cam power on
			mmc_EntryMode(MMC_MODE_CAMERA, 0);
			if ((result = Camera_PowerOn()) == 0)
			mmc_ExitMode();
			break;
		case MCI_CAM_POWER_DOWN_REQ:
			//		clearLilyTaskRunFlag();
			
			if ((result = Camera_PowerOff()) == 1)
			{
				mmc_ExitMode();
			}
			break;
		case MCI_CAM_PREVIEW_REQ:
			Camera_Preview(EVENT_GetDataAddr(nEvent));
			break;
		case MCI_CAM_CAPTURE_REQ:
			if(0)//mulitishot is TRUE
			{
			Camera_MSset(10,800);
		}
		else
		{
		}
		Camera_Capture();
		clearLilyTaskRunFlag();		
		break;	
	case MCI_CAM_CAPTURE_MS_IND:
		Camera_Capture();
		break;
	case MCI_CAM_SET_PARAM_REQ:
		Camera_SetEffect(EVENT_GetCmdArg(nEvent,0),EVENT_GetCmdArg(nEvent,1) );
		break;			
	case MCI_CAM_STOP_REQ:
		diag_printf(1,"MCI_CAM_STOP_REQ\n");
		Camera_PreviewOff();
//		clearLilyTaskRunFlag();
		break;	
	default:
		/* process custom msg */
		diag_printf("get a record msg from task\n");
		break;
	}
    mmc_SendResult(nEvent, result);
    EVENT_Free(nEvent);

	}
	  else
	  {
	    diag_printf("get a record msg from task,.nEvent is %ld\n", EVENT_GetCommand(nEvent));

	    Camera_UserMsg(EVENT_GetCommand(nEvent));
	  }

}
#endif
#if 0
VOID 	mmc_VidrecStateProcess(int32 nEvent)
{
  int32 result;
  if (EVENT_GetType(nEvent) == EVENT_TYPE_JADE) /* process X msg */
  {
//    diag_printf("mmc_VidrecStateProcess EVENT_GetCommand(nEvent) = %d\n", EVENT_GetCommand(nEvent));
    switch (EVENT_GetCommand(nEvent))
    {
      case MCI_REC_VIDEO_PREVIEW_START_REQ:
        mmc_EntryMode(MMC_MODE_VIDREC, 0);
        Vidrec_PreviewStart();
        mmc_SendResult(nEvent,1);
        break;
      case MCI_REC_VIDEO_PREVIEW_STOP_REQ:
        Vidrec_PreviewStop();
        mmc_SendResult(nEvent,1);
        mmc_ExitMode();
        break;
      case MCI_REC_VIDEO_START_REQ:
        result = Vidrec_RecordStart(EVENT_GetDataAddr(nEvent));
        mmc_SendResult(nEvent,result);
        break;
      case MCI_REC_VIDEO_STOP_REQ:
        Vidrec_RecordStop();
        mmc_SendResult(nEvent,1);
        break;
      case MCI_REC_VIDEO_PAUSE_REQ:
        Vidrec_RecordPause();
        mmc_SendResult(nEvent,1);
        break;
      case MCI_REC_VIDEO_RESUME_REQ:
        Vidrec_RecordResume();
        mmc_SendResult(nEvent,1);
        break;
      case REC_PCMDATA_IND:
        Vidrec_Sample(EVENT_GetDataAddr(nEvent), EVENT_GetDataSize(nEvent) >> 1);
        break;
      default :
        break;
    }
    //mmc_SendResult(nEvent,result);
    EVENT_Free(nEvent);
  }
  else                   /* process custom msg */
  {
    Vidrec_UserMsg(EVENT_GetCommand(nEvent));       
  }
}
#endif 
#if 0
int32 mmc_GetImageType(char * file_name)
{
	diag_printf("file_name = %s\n", file_name);
	if(strstr(file_name,".jpg") ||strstr(file_name,".JPG") || strstr(file_name, ".jpeg") ||strstr(file_name, ".JPEG"))
	{
		return MMC_IMAGE_TYPE_JPEG;
	}
	else if(strstr(file_name,".gif") ||strstr(file_name,".GIF") )
		return MMC_IMAGE_TYPE_GIF;
	
	else if(strstr(file_name,".bmp") ||strstr(file_name,".BMP") )
		return MMC_IMAGE_TYPE_BMP;
	else 
		return MMC_IMAGE_TYPE_INVALID;
}

VOID mmc_ImageStateProcess(int32 nEvent)
{
  int32 result;
  int32 imageType;
  mmc_EntryMode(MMC_MODE_IMAGE, 0);

  if (EVENT_GetType(nEvent) == EVENT_TYPE_JADE) /* process X msg */
  {
  	switch (EVENT_GetCommand(nEvent))
  	{
	  	case MCI_IMG_DECODE_REQ:
	       case MCI_IMG_RESIZE_REQ:
		case MCI_IMG_DISPLAY_IND:
		    	g_imgReq.image_clip_x1 = EVENT_GetCmdArg(nEvent, 0);
			g_imgReq.image_clip_x2 = EVENT_GetCmdArg(nEvent, 1);
			g_imgReq.image_clip_y1 = EVENT_GetCmdArg(nEvent, 2);
			g_imgReq.image_clip_y2 = EVENT_GetCmdArg(nEvent, 3);
			g_imgReq.display_width= EVENT_GetCmdArg(nEvent, 4);
			g_imgReq.display_height= EVENT_GetCmdArg(nEvent, 5);
			strcpy(g_imgReq.file_name,EVENT_GetDataAddr(nEvent));
		  	imageType = mmc_GetImageType(g_imgReq.file_name);
			switch (imageType)
			{
				case MMC_IMAGE_TYPE_JPEG:
					result = Msg_ProcessJpeg(EVENT_GetCommand(nEvent));
					break;
				case MMC_IMAGE_TYPE_BMP:
					result = Msg_ProcessBmp();
					break;
				case MMC_IMAGE_TYPE_GIF:
					gif_first_entry =1;
					result = Msg_ProcessGif();
					if (!result)
					EVENT_Send2Task(MBOX_ID_MMC,MSG_MMC_GIF_PLAY_CONTINUE); 
					break;
			}
			break;
  	}
	mmc_SendResult(nEvent,result);
	EVENT_Free(nEvent);
  }
  else                   /* process custom msg */
  {
    switch (EVENT_GetCommand(nEvent))
    {
    		case MSG_MMC_GIF_PLAY_CONTINUE:
			if (gif_paly_in && !interrupt_gif_play)
			{
				Msg_ProcessGif();
				//mmc_gif_draw(mmc_gif_disp_region_x1,mmc_gif_disp_region_y1,mmc_gif_disp_region_x2,mmc_gif_disp_region_y2,mmc_gif_disp_file_p);
				//EVENT_Send2Task(MBOX_ID_MMC,MSG_MMC_GIF_PLAY_CONTINUE);
			}
			break;
    }
  }
  if (!gif_paly_in)
  	mmc_ExitMode();
}
 #endif


 COS_EVENT ev ;
 COS_EVENT ev_play ;
 /*b_vga  is used to control the sensor's output size*/
  /*b_vga == 0 means sensor uses QVGA size output */
  /*b_vga != 0  means sensor uses  VGA  size output */
 INT32 b_vga = 0;
//extern int32 vid_sound_Rec;
//int32 jpeg_num=0; 
extern INT32 MMC_GetLcdWidth();
extern INT32 MMC_GetLcdHeight();
extern INT32 MMC_GetSensorSize();
VOID MMCTask(uint32 data)
{
	BOOL bRet;
	bRet = TRUE;
	//int result = 0;
	//INT32 in_file = NULL;
	//UINT8 playMode = 0;
	//UINT8 times = 0;
       diag_printf( "*******************MMCTask begin**********************");
	SUL_ZeroMemory32( &ev, SIZEOF(COS_EVENT) );
	SUL_ZeroMemory32( &ev_play, SIZEOF(COS_EVENT) );
		/*Initialize the size of the LCD in MCI layer.*/
	MCI_LcdSizeSet(MMC_GetLcdWidth(),MMC_GetLcdHeight());
	b_vga = MMC_GetBVGA(MMC_GetSensorSize());
//	Mp3Info.state = MP3DEC_STOP;
	for(;;)
	{		
		ev_play.nEventId = 0;
		COS_WaitEvent(GetMCITaskHandle(MBOX_ID_MMC), &ev_play, COS_WAIT_FOREVER);
 		switch(mmc_GetCurrMode())
    		{
		case MMC_MODE_AUDIO:
			//diag_printf( "*******************MMCTask MMC_MODE_AUDIO**********************");
			gAudio.UserMsg(ev_play.nEventId);		
			break;
#ifndef TARGET_VIDEO_PLAYER_SUPPORT_OFF
		case MMC_MODE_VIDEO:
			//diag_printf( "MMCTask i=%d\n", i);
			gVideo.UserMsg(&ev_play);
			break;
#endif			
		case MMC_MODE_CAMERA:
		case MMC_MODE_APBS:
			apbs_UserMsgHandle(&ev_play);
			//TRACE_EF(1, "MMC_MODE_CAMERA");
			//diag_printf("mmc receive cap_ind\n");
			Camera_UserMsg(&ev_play);
			break;
#ifndef TARGET_SOUND_RECORDER_SUPPORT_OFF
		case MMC_MODE_RECORD:
#endif			
		case MMC_MODE_VDOCHAT:
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)		
			vdochat_usrmsgHandle(&ev_play);
			//TRACE_EF(1, "MMC_MODE_CAMERA");
			//diag_printf("mmc receive cap_ind\n");
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)

#ifndef TARGET_SOUND_RECORDER_SUPPORT_OFF
			sndRec_msgHandle(ev_play.nEventId);
#endif			

			break;

#ifndef TARGET_VIDEO_RECORDER_SUPPORT_OFF			
		case MMC_MODE_VIDREC:
			VidRec_UserMsg(&ev_play);
			break;
#endif

		default:
//			diag_printf( "*******************MMCTask no message process**********************");
			//diag_printf("no message process\n");
			break;

	}
 }
}
VOID MMCTask_bak(uint32 data)
{
  uint32 nEvent;
  diag_printf("MMCTASK entry!!!!!!!!!!!!!!!!!!!!!!!\n");
  diag_printf( "*******************MMCTask begin**********************");
  //cyg_semaphore_init(&camCapSem,0);//chenhe for cap
//  COS_SemaphoreInit(&camCapSem,0);
 // lily_stateMachineInit();//chenhe for state machine management,071226
   COS_EVENT event;
  
  while(1)
  {

//  uint32 ret;
//  diag_printf( "*******************COS_MBoxGet:begin**********************");
  COS_WaitEvent(GetMCITaskHandle(MBOX_ID_MMC), &event, COS_WAIT_FOREVER);
//  diag_printf( "*******************COS_MBoxGet:end**********************");
  nEvent = event.nEventId; 
//    nEvent = COS_MBoxGet(MBOX_ID_MMC);
	
diag_printf( "*******************MMCTask:%d**********************");

    //diag_printf("get a message type : %d, xmsgid = %d, stat = %d\n",EVENT_GetType(nEvent), EVENT_GetCommand(nEvent), mmc_GetCurrMode());
    switch(mmc_GetCurrMode())
    {
		case MMC_MODE_IDLE:
      if (nEvent == REC_PCMDATA_IND)
      {
			  diag_printf("############## [vidrec warning]: MMC task receive a spare frame of PCM data! \n");
   //     EVENT_Free(nEvent);
      }
			break;
			
		case MMC_MODE_AUDIO:
			gAudio.UserMsg(nEvent);		
			break;
/*			
		case MMC_MODE_VIDEO:
			gVideo.UserMsg(EVENT_GetCommand(nEvent));
			break;
			
		case MMC_MODE_RECORD:
			if (EVENT_GetCommand(nEvent) == REC_PCMDATA_IND) 
			{
				//gRecord.Sample(EVENT_GetDataAddr(nEvent), EVENT_GetDataSize(nEvent) >> 1);
				RecordSample(EVENT_GetDataAddr(nEvent), EVENT_GetDataSize(nEvent) >> 1);
				EVENT_Free(nEvent);
			}
			else
			{
				//gRecord.UserMsg(EVENT_GetCommand(nEvent));	
				RecordUserMsg(EVENT_GetCommand(nEvent));
			}
			break;
			
		case MMC_MODE_FM_RECORD:
			FmrRecordUserMsg(EVENT_GetCommand(nEvent));
			break;		
			
		case MMC_MODE_CAMERA:
			//diag_printf("mmc receive cap_ind\n");
			Camera_UserMsg(EVENT_GetCommand(nEvent));
			break;
			
		case MMC_MODE_VIDREC:
      if (EVENT_GetCommand(nEvent) == REC_PCMDATA_IND)
      {
        LILY_VidrecAudioSampleReadyReq(nEvent);
        EVENT_Free(nEvent);
      }
      else
        Vidrec_UserMsg();
			break;
			
		case MMC_MODE_IMAGE:
			break;	
			
		case MMC_MODE_INVALID:
			break;
			*/
		default:
			ass(1);
			diag_printf("no message process\n");
			break;
    }
//    Lily_TaskCheckStack();
  }
}

int32 mmc_SendEvent(HANDLE hTask, uint32 nEventId)
{
 	COS_EVENT event={0,0,0,0};
 	event.nEventId = nEventId;		
  sxr_Send(&event, ((TASK_HANDLE* )hTask)->nMailBoxId, SXR_SEND_EVT);
  return TRUE;
}

int32 mmc_SendMsg(HANDLE hTask, uint32 nEventId, uint32 param1, uint32 param2, uint32 param3)
{
 	COS_EVENT event={0,0,0,0};
 	event.nEventId = nEventId;	
  event.nParam1 = param1;
  event.nParam2 = param2;
  event.nParam3 = param3;  
  sxr_Send(&event, ((TASK_HANDLE* )hTask)->nMailBoxId, SXR_SEND_EVT);
  return TRUE;
}

