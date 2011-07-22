////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2007, Coolsand Technologies, Inc.
//                       All Rights Reserved
//
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: video_api.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   shenh
//
////////////////////////////////////////////////////////////////////////////////
#include "cswtype.h"
#include "cs_types.h"
#include "cpu_share.h"
#include "mcip_debug.h"

#include "mmc.h"
#include "video_api.h"

//#include "cpu_sleep.h"
#include "lily_statemachine.h"
#include "global.h"
#include "mmc_isom_video.h"

static uint8 stopID=0;
static uint8 resumeID=0;
static int16 offset_x=0;
static int16 offset_y=0;
static int32 videoType=0;
//static uint8  aviSetTag=0;//test .no avi zoom in

int32  LILY_VideoOpenFile (HANDLE fhd, uint8 file_mode, uint32 data_Len, int OutputPath,uint8 open_audio, mci_type_enum type, void(*vid_play_finish_callback)(int32), void (*vid_draw_panel_callback)(uint32))
{
	int32 result;
	diag_printf( "LILY_VideoOpenFile... \n");
	videoType = type;
	stopID=0;
	resumeID=0;
	
	if (STA_RES_FAIL == lily_changeToState(STM_MOD_VID))
	{
		diag_printf( "Error state!!!!!!!!!!!in bmpshow\n");
		return -1;
	}

	switch(videoType)
	{
		case MCI_TYPE_MJPG:
		/*
			{
				mp4_open_struct Mp4Open;
				aviSetTag=0;
				Mp4Open.file_handle=fhd;
				Mp4Open.OutputPath=OutputPath;
				Mp4Open.lcd_start_x=0;
				Mp4Open.lcd_start_y=0;
				Mp4Open.open_audio=open_audio;
				if((result=mmc_vid_open_file_req_hdlr(&Mp4Open))==MED_RES_OK)
				{
					mmc_EntryMode(MMC_MODE_VIDEO, videoType);
					mmc_SetModeStatus(NoneStatus);
				}
				else
					mmc_ExitMode();
			}
			break;
		*/
		case MCI_TYPE_RM:
#ifdef MP4_3GP_SUPPORT
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			if(videoType==MCI_TYPE_MJPG)
				diag_printf("Media Type: MCI_TYPE_MJPG");
			else if(videoType==MCI_TYPE_MP4)
				diag_printf("Media Type: MCI_TYPE_MP4");
			else if(videoType==MCI_TYPE_3GP)
				diag_printf("Media Type: MCI_TYPE_3GP");
			else if(videoType==MCI_TYPE_RM)
				diag_printf("Media Type: MCI_TYPE_RM");
				
			if((result=mmc_vid_isom_open_file(fhd, file_mode, data_Len, OutputPath, open_audio, type, vid_play_finish_callback, vid_draw_panel_callback))==ISOM_ERR_OK)
			{
				mmc_EntryMode(MMC_MODE_VIDEO, videoType);
				mmc_SetModeStatus(NoneStatus);
			}
			else
				mmc_ExitMode();
			
			break;
#endif
		default:
			result=-1;
			break;
	}
	return result;
}

int32  LILY_VideoSeek (uint32 seektime, int32 time_mode, int16 startX, int16 startY)
{
	int32 result;
	diag_printf( "LILY_VideoSeek... \n");
	stopID=0;
	switch(videoType)
	{
		case MCI_TYPE_MJPG:
		/*
			{
				mp4_seek_struct SeekInfo;
				SeekInfo.get_frame=1;
				SeekInfo.time=seektime;
				SeekInfo.time_mode=time_mode;
				if(aviSetTag==0)
				{
					offset_x=startX;
					offset_y=startY;
				}
				SeekInfo.lcd_start_x=offset_x;
				SeekInfo.lcd_start_y=offset_y;
				if(resumeID==0)
				{
					if((result=mmc_vid_seek_req_hdlr(&SeekInfo))==MED_RES_OK)
					{
						mmc_SetModeStatus(VideoPauseStatus);
					}
				}
				else
				{
					result=mmc_vid_stop_req_hdlr();
					if((result=mmc_vid_seek_req_hdlr(&SeekInfo))==MED_RES_OK)
					{
						mmc_SetModeStatus(VideoPauseStatus);
					}
					resumeID=0;
				}
			}
			break;
			*/
		case MCI_TYPE_RM:
#ifdef MP4_3GP_SUPPORT
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			offset_x=startX;
			offset_y=startY;
			
			if(resumeID==0)
			{
				if((result=mmc_vid_isom_seek(seektime,time_mode,startX,startY))==ISOM_ERR_OK)
				{
					mmc_SetModeStatus(VideoPauseStatus);
				}
			}
			else
			{
				result=mmc_vid_isom_stop();
				if((result=mmc_vid_isom_seek(seektime,time_mode,startX,startY))==ISOM_ERR_OK)
				{
					mmc_SetModeStatus(VideoPauseStatus);
				}
				resumeID=0;
			}
			break;
#endif
		default:
			result=-1;
			break;
	}
	return result;
}

int32  LILY_VideoPlay (uint16 lcd_start_x, uint16 lcd_start_y)
{
	int32 result;
	diag_printf( "LILY_VideoPlay... \n");
	stopID=0;
	switch(videoType)
	{
		case MCI_TYPE_MJPG:
		/*
			 if(lcd_start_x != 0 && lcd_start_y != 0)
		 	{
		 		if(aviSetTag==0)
		 		{
					 offset_x=lcd_start_x;
					 offset_y=lcd_start_y;
		 		}
		 	}
			 
			 if(resumeID==0)
		 	{
				if((result=mmc_vid_play_req_hdlr(offset_x, offset_y))==MED_RES_OK)
				{
					mmc_SetModeStatus(VideoPlayStatus);
				}
				else
				{
					result=0;
				}
		 	}
			 else
			 {
				if((result=mmc_vid_resume_req_hdlr())==MED_RES_OK)
				{
					mmc_SetModeStatus(VideoPlayStatus);
				}
				else
				{
					result=0;
				}
			 }
			break;
			*/
		case MCI_TYPE_RM:
#ifdef MP4_3GP_SUPPORT
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			if(resumeID==0)
			{
				 if(lcd_start_x != 0 && lcd_start_y != 0)
			 	{
					 offset_x=lcd_start_x;
					 offset_y=lcd_start_y;
			 	}

				if((result=mmc_vid_isom_play(offset_x, offset_y))==ISOM_ERR_OK)
				{
					mmc_SetModeStatus(VideoPlayStatus);
				}
				else
				{
					result=0;
				}
			}
			else
			{
				if((result=mmc_vid_isom_resume())==ISOM_ERR_OK)
				{
					mmc_SetModeStatus(VideoPlayStatus);
				}
				else
				{
					result=0;
				}
			}
			break;
#endif
		default:
			result=-1;
			break;
	}
	return result;
}

int32  LILY_VideoStop (void)
{
	int32 result;
	 diag_printf( "LILY_VideoStop... \n");
	 stopID=1;
	 resumeID=0;
	switch(videoType)
	{
		case MCI_TYPE_MJPG:
		/*
			result=mmc_vid_stop_req_hdlr();
			mmc_SetModeStatus(VideoStopStatus);
			break;
		*/
		case MCI_TYPE_RM:
#ifdef MP4_3GP_SUPPORT
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			result=mmc_vid_isom_stop();
			mmc_SetModeStatus(VideoStopStatus);
			break;
#endif
		default:
			result=-1;
			break;
	}
	return result;
}

int32  LILY_VideoPause (void) 
{
	int32 result;
	diag_printf( "LILY_VideoPause... \n");
	stopID=0;
	resumeID=1;
	switch(videoType)
	{
		case MCI_TYPE_MJPG:
		/*
			result=mmc_vid_pause_req_hdlr();
			mmc_SetModeStatus(VideoPauseStatus);
			break;
			*/
		case MCI_TYPE_RM:
#ifdef MP4_3GP_SUPPORT
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			if((result=mmc_vid_isom_pause())==ISOM_ERR_OK)
				mmc_SetModeStatus(VideoPauseStatus);
			else
			{
				result=0;
			}
			break;
#endif
		default:
			result=-1;
			break;
	}
	return result;
}

int32  LILY_VideoResume (void)
{
	int32 result;
	diag_printf( "LILY_VideoReusme... \n");
	stopID=0;
	resumeID=0;
	switch(videoType)
	{
		case MCI_TYPE_MJPG:
		/*
			if((result=mmc_vid_resume_req_hdlr())==MED_RES_OK)
			{
				mmc_SetModeStatus(VideoPlayStatus);
			}
			else
			{
				result=0;
			}
			break;
		*/
		case MCI_TYPE_RM:
#ifdef MP4_3GP_SUPPORT
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			if((result=mmc_vid_isom_resume())==ISOM_ERR_OK)
			{
				mmc_SetModeStatus(VideoPlayStatus);
			}
			else
			{
				result=0;
			}
			break;
#endif
		default:
			result=-1;
			break;
	}
	return result;
}

int32  LILY_VideoClose (void)
{
	int32 result;
	diag_printf( "LILY_VideoClose... \n");
	stopID=0;
	resumeID=0;
	switch(videoType)
	{
		case MCI_TYPE_MJPG:
		/*
			aviSetTag=0;
			result=mmc_vid_close_file_req_hdlr();
			mmc_SetModeStatus(NoneStatus);
			mmc_ExitMode();
			break;
		*/
		case MCI_TYPE_RM:
#ifdef MP4_3GP_SUPPORT
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			result=mmc_vid_isom_close_file();
			mmc_SetModeStatus(NoneStatus);
			mmc_ExitMode();
			break;
#endif
		default:
			result=-1;
			break;
	}
	
	lily_exitModule(STM_MOD_VID);
	return result;
}

int32  LILY_VideoGetInfo (mci_vid_file_ready_ind_struct *pFileInfo)
{
	int32 result;
#ifdef MP4_3GP_SUPPORT
	IsomPlayInfo Info;
#endif
	//diag_printf( "LILY_VideoGetInfo... \n");
	switch(videoType)
	{
		case MCI_TYPE_MJPG:
		/*
			{
				mp4_play_info  StateInfo;
				result=mmc_vid_get_info( &StateInfo);
				pFileInfo->total_time=StateInfo.total_time;
				if(stopID==0)
					pFileInfo->current_time=StateInfo.current_time;
				else
					pFileInfo->current_time=0;
				pFileInfo->total_frame_num=StateInfo.total_frame_num;
				pFileInfo->image_width=StateInfo.image_width;
				pFileInfo->image_height=StateInfo.image_height;
				//no using
				pFileInfo->padA=StateInfo.pada;
				pFileInfo->padB=StateInfo.padb;
				pFileInfo->result=StateInfo.result;
				pFileInfo->seq_num=StateInfo.seq_num;
				pFileInfo->media_type=videoType;
			}
			break;
			*/
		case MCI_TYPE_RM:
#ifdef MP4_3GP_SUPPORT
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			result= mmc_vid_isom_get_info( &Info);
			pFileInfo->total_time=Info.total_time;
			if(stopID==0)
				pFileInfo->current_time=Info.current_time;
			else
				pFileInfo->current_time=0;
			pFileInfo->total_frame_num=Info.total_frame_num;
			pFileInfo->image_width=Info.image_width;
			pFileInfo->image_height=Info.image_height;
			pFileInfo->media_type=videoType;
			//no using
			pFileInfo->padA=0;
			pFileInfo->padB=0;
			pFileInfo->result=0;
			pFileInfo->seq_num=0;
                     pFileInfo->aud_channel = Info.aud_channel_no;
                     pFileInfo->aud_sample_rate =Info.aud_sample_rate;
			pFileInfo->track    =  Info.track;
			break;
#endif
		default:
			result=-1;
			break;
	}
	return result;
}

int32  LILY_VideoSetInfo(mci_vid_set_mode_ind_struct *pSetMode)
{
	int32 result;
	diag_printf( "LILY_VideoSetInfo... \n");
	switch(videoType)
	{
		case MCI_TYPE_MJPG:
		#if 0
			{
				mp4_set_info SetInfo;
				SetInfo.zoom_mode=0;
				SetInfo.lcd_start_x=0;
				SetInfo.lcd_start_y=0;
				result=mmc_vid_set_info(&SetInfo);//no use
				if(aviSetTag==0)
					aviSetTag=1;
				else
					aviSetTag=0;
			}
			break;
		#endif	
		case MCI_TYPE_RM:
#ifdef MP4_3GP_SUPPORT
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			{
				IsomPlayMode  Mode;
				Mode.zoom_width=pSetMode->zoom_width;
				Mode.zoom_height=pSetMode->zoom_height;
				Mode.lcd_start_x=pSetMode->lcd_start_x;
				Mode.lcd_start_y=pSetMode->lcd_start_y;
				Mode.cutX=pSetMode->cutX;
				Mode.cutY=pSetMode->cutY;
				Mode.cutW=pSetMode->cutW;
				Mode.cutH=pSetMode->cutH;
				Mode.rotate=pSetMode->rotate;
				result=mmc_vid_isom_set_mode(&Mode);
			}
			break;
#endif
		default:
			result=-1;
			break;
	}
	return result;
}

