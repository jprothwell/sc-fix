/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *   vid_api.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary exported functions of image module.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES =========*/

/* system includes */
#include "kal_release.h" 
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"          /* task message communiction */
#include "stacklib.h"           /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"          /* event scheduler */
#include "stack_timer.h"         /* stack timer */

/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"

#ifdef MP4_CODEC
#include    "drv_comm.h"
//#include    "visual_comm.h"
#include    "fsal.h"
#include    "mp4_parser.h"
#include    "lcd_if.h"
#include    "rtc_sw.h"
#include    "video_file_creator.h"
#include    "bmd.h"
#include    "video_enc_glb.h"
#include    "video_dec_glb.h"
#include    "video_glb.h"
#endif

/* local includes */
#include "med_global.h"
#include "med_status.h"
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "vid_main.h"


#include "media_others.h"
#include "mmi_trace.h"

#if defined (__MED_VID_MOD__) || defined(__MED_MJPG_MOD__)

static kal_int32 vid_result;
static kal_uint16 image_x;
static kal_uint16 image_y;
static kal_uint64 vid_total_time;
static kal_uint64 vid_current_time;


void vid_send_preview_req(module_type src_mod_id, void* data);
void vid_display_set_req(module_type src_mod_id,kal_uint16 ZoomWidth, kal_uint16 ZoomHeight, kal_int16 startX, kal_int16 startY, kal_int16 cutX, kal_int16 cutY, kal_int16 cutW, kal_int16 cutH, kal_uint16 rotate);
void vid_rec_send_previewStop_req(module_type src_mod_id);
void vid_rec_send_pause_req(module_type src_mod_id);
void vid_rec_set_param_req(module_type src_mod_id, kal_uint16 param_id, kal_int16 value);
void vid_rec_send_resume_req1(module_type src_mod_id);
void vid_vdo_get_play_time_req(module_type src_mod_id);
void vid_vdo_get_play_time_block_req(module_type src_mod_id);
void vid_rec_send_stop_req(module_type src_mod_id);


/*==== FUNCTIONS ===========*/


/*************************************************************************
* FUNCTION
*	vid_set_result
*
* DESCRIPTION
*	This function is to set the result. 
*
* PARAMETERS
*	result
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_set_result(kal_int32 result)
{
   vid_result=result;
}

/*************************************************************************
* FUNCTION
*	vid_set_image_size
*
* DESCRIPTION
*	This function is to set the decoded image size. 
*
* PARAMETERS
*	x
*  y
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_set_image_size(kal_uint16 x, kal_uint16 y)
{
   image_x=x;
   image_y=y;
}

/*************************************************************************
* FUNCTION
*	vid_set_total_time
*
* DESCRIPTION
*	This function is to set the total time. 
*
* PARAMETERS
*	x
*  y
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_set_total_time(kal_uint64 total_time)
{
   vid_total_time=total_time;
}

/*************************************************************************
* FUNCTION
*	vid_set_current_time
*
* DESCRIPTION
*	This function is to set the current time. 
*
* PARAMETERS
*	x
*  y
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_set_current_time(kal_uint64 current_time)
{
   vid_current_time=current_time;
}

/*************************************************************************
* FUNCTION
*	media_vid_preview
*
* DESCRIPTION
*	This function is to start video preview. 
*
* PARAMETERS
*	module_type src_mod_id
*  vid_record_struct* record
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_preview(module_type src_mod_id, void* preview)
{
   vid_result=MED_RES_OK;
   vid_send_preview_req(src_mod_id, preview);
   VID_WAIT_EVENT(VID_EVT_PREVIEW);
   return vid_result;
}



/*************************************************************************
* FUNCTION
*	media_vid_record
*
* DESCRIPTION
*	This function is to start video recording. 
*
* PARAMETERS
*	module_type src_mod_id
*  void* record
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_record(module_type src_mod_id, void* record)
{
   vid_result=MED_RES_OK;
   vid_send_record_req(src_mod_id, record);
   VID_WAIT_EVENT(VID_EVT_RECORD);
   return vid_result;
}


/*************************************************************************
* FUNCTION
*	media_vid_stop
*
* DESCRIPTION
*	This function is to stop video recording/playing. 
*
* PARAMETERS
*	module_type src_mod_id
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_stop(module_type src_mod_id)
{
   vid_send_stop_req(src_mod_id);
   VID_WAIT_EVENT(VID_EVT_STOP);
}
void media_vid_display_set(module_type src_mod_id,kal_uint16 ZoomWidth, kal_uint16 ZoomHeight, kal_int16 startX, kal_int16 startY, kal_int16 cutX, kal_int16 cutY, kal_int16 cutW, kal_int16 cutH, kal_uint16 rotate)
{
   vid_display_set_req(src_mod_id,ZoomWidth,ZoomHeight,startX,startY,cutX,cutY,cutW,cutH,rotate);
}

//chenhe for jasperII
void media_vidrec_stop(module_type src_mod_id)
{
   vid_rec_send_stop_req(src_mod_id);
   VID_WAIT_EVENT(VID_EVT_STOP);
}

//chenhe for jasperII
void media_vidrec_previewStop(module_type src_mod_id)
{
   vid_rec_send_previewStop_req(src_mod_id);
   VID_WAIT_EVENT(VID_EVT_STOP);
}
/*************************************************************************
* FUNCTION
*	media_vid_file_merge
*
* DESCRIPTION
*	This function is to merge temp files to a video file. 
*
* PARAMETERS
*	module_type src_mod_id
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_file_merge(module_type src_mod_id, kal_wchar* path)
{
   vid_send_file_merge_req(src_mod_id, path);
   VID_WAIT_EVENT(VID_EVT_FILE_MERGE);
}

/*************************************************************************
* FUNCTION
*	media_vid_open_file
*
* DESCRIPTION
*	This function is to open a video file. 
*
* PARAMETERS
*	module_type src_mod_id
*  vid_open_file_struct* open_file
*  kal_uint16 seq_num
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_open_file(module_type src_mod_id, void* file_name, kal_uint16 seq_num,void (*videoplayer_drawtime_hdlr)(kal_int32))
{
   vid_result=MED_RES_OK;
   vid_send_open_file_req(src_mod_id, file_name, seq_num, 0,videoplayer_drawtime_hdlr);
   VID_WAIT_EVENT(VID_EVT_OPEN_FILE);  
   return vid_result;
}

/*************************************************************************
* FUNCTION
*	media_vid_open_file_to_mem
*
* DESCRIPTION
*	This function is to open a video file. 
*
* PARAMETERS
*	module_type src_mod_id
*  vid_open_file_struct* open_file
*  kal_uint16 seq_num
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_open_file_to_mem(module_type src_mod_id, void* file_name, kal_uint16 seq_num)
{
   vid_result=MED_RES_OK;
   vid_send_open_file_req(src_mod_id, file_name, seq_num, 1,NULL);
   VID_WAIT_EVENT(VID_EVT_OPEN_FILE);  
   return vid_result;
}


/*************************************************************************
* FUNCTION
*	media_vid_close_file
*
* DESCRIPTION
*	This function is to open a video file. 
*
* PARAMETERS
*	module_type src_mod_id
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_close_file(module_type src_mod_id)
{
   vid_send_close_file_req(src_mod_id);
   VID_WAIT_EVENT(VID_EVT_CLOSE_FILE);
}

/*************************************************************************
* FUNCTION
*	media_vid_open
*
* DESCRIPTION
*	This function is to open a video file/array/stream. 
*
* PARAMETERS
*	module_type src_mod_id
*  vid_open_file_struct* open_file
*  kal_uint16 seq_num
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_open(module_type src_mod_id, vid_open_struct* open)
{
   vid_result=MED_RES_OK;
   vid_send_open_req(src_mod_id, (void*)open);
   VID_WAIT_EVENT(VID_EVT_OPEN);
	mmi_trace(TRUE," Func: %s image_width:%d, image_height:%d", __FUNCTION__, image_x, image_x);

   if(open->image_width)   
      *open->image_width = image_x;
   if(open->image_height)
      *open->image_height = image_y;

   if(open->total_time)
      *open->total_time=vid_total_time;
   
   return vid_result;
}


/*************************************************************************
* FUNCTION
*	media_vid_close
*
* DESCRIPTION
*	This function is to close a video file/array/stream. 
*
* PARAMETERS
*	module_type src_mod_id
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_close(module_type src_mod_id)
{
   vid_send_close_req(src_mod_id);
   VID_WAIT_EVENT(VID_EVT_CLOSE);
}



/*************************************************************************
* FUNCTION
*	media_vid_play
*
* DESCRIPTION
*	This function is to start video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*  vid_play_struct* play
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_play(module_type src_mod_id, vid_play_struct* play)
{
	TRACE_EF(1," media_vid_play");
   vid_result=MED_RES_OK;
   vid_send_play_req(src_mod_id, play);
   VID_WAIT_EVENT(VID_EVT_PLAY);
   return vid_result;
}

/*************************************************************************
* FUNCTION
*	media_vid_seek
*
* DESCRIPTION
*	This function is to seek to the given position. 
*
* PARAMETERS
*	module_type src_mod_id
*  vid_seek_struct* seek
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_seek(module_type src_mod_id, vid_seek_struct* seek)
{
   vid_result=MED_RES_OK;
   vid_send_seek_req(src_mod_id, seek);
   VID_WAIT_EVENT(VID_EVT_SEEK);
   return vid_result;
}

/*************************************************************************
* FUNCTION
*	media_vid_pause
*
* DESCRIPTION
*	This function is to pause the video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_display_interface(module_type src_mod_id)
{
   vid_result=MED_RES_OK;
   //mmi_trace(1,"elisa media_vid_display_interface");
   vid_send_display_interface_req(src_mod_id);
   return vid_result;
}

/*************************************************************************
* FUNCTION
*	media_vid_pause
*
* DESCRIPTION
*	This function is to pause the video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_pause(module_type src_mod_id)
{
   vid_result=MED_RES_OK;
   mmi_trace(1,"elisa media_vid_pause");
   vid_send_pause_req(src_mod_id);
   VID_WAIT_EVENT(VID_EVT_PAUSE);
   return vid_result;
}

kal_int32 media_vidRec_pause(module_type src_mod_id)
{
   vid_result=MED_RES_OK;
   mmi_trace(1," media_vid_pause");
   vid_rec_send_pause_req(src_mod_id);
   VID_WAIT_EVENT(VID_EVT_PAUSE);
   return vid_result;
}



/*************************************************************************
* FUNCTION
*	media_vidRec_set_param
*
* DESCRIPTION
*	This function is to set the parameter of camera. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint16 param_id
*  kal_int16 value
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vidRec_set_param(module_type src_mod_id, kal_uint16 param_id, kal_int16 value)
{
   mmi_trace(1, "Excute funs: media_vidRec_set_param () is TRUE param_id = %ld ,value = %ld",param_id, value);
   vid_rec_set_param_req(src_mod_id, param_id, value);
}
/*************************************************************************
* FUNCTION
*	media_vid_resume
*
* DESCRIPTION
*	This function is to resume the video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_resume(module_type src_mod_id)
{
   vid_result=MED_RES_OK;
   vid_send_resume_req(src_mod_id);
   VID_WAIT_EVENT(VID_EVT_RESUME);
   return vid_result;
}

kal_int32 media_vidRec_resume(module_type src_mod_id)
{
   vid_result=MED_RES_OK;
      mmi_trace(1," media_vidRec_resume");

   vid_rec_send_resume_req1(src_mod_id);
   VID_WAIT_EVENT(VID_EVT_RESUME);
   return vid_result;
}

/*************************************************************************
* FUNCTION
*	media_vid_snapshot
*
* DESCRIPTION
*	This function is to get snapshot when the video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*  vid_snapshot_struct* snapshot
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_snapshot(module_type src_mod_id, vid_snapshot_struct* snapshot)
{
   vid_result=MED_RES_OK;
   vid_send_snapshot_req(src_mod_id, snapshot);
   VID_WAIT_EVENT(VID_EVT_SNAPSHOT);
   return vid_result;
}

/*************************************************************************
* FUNCTION
*	media_vid_get_iframe
*
* DESCRIPTION
*	This function is to get iframe when the video file. 
*
* PARAMETERS
*	module_type src_mod_id
*  vid_get_iframe_struct* iframe
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_get_iframe(module_type src_mod_id, vid_get_iframe_struct* iframe)
{
   vid_result=MED_RES_OK;
   vid_send_get_iframe_req(src_mod_id, iframe);
   VID_WAIT_EVENT(VID_EVT_GET_IFRAME);
   return vid_result;
}


/*************************************************************************
* FUNCTION
*	media_vid_set_display
*
* DESCRIPTION
*	This function is to set display device for video play. 
*
* PARAMETERS
*	module_type src_mod_id
*  vid_get_iframe_struct* iframe
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_set_display(module_type src_mod_id, kal_uint8 device)
{
   vid_result=MED_RES_OK;
   vid_send_set_display_req(src_mod_id, device);
   VID_WAIT_EVENT(VID_EVT_SET_DISPLAY);
   return vid_result;
}

/*************************************************************************
* FUNCTION
*	media_vid_set_em_mode
*
* DESCRIPTION
*	This function is to set engineer mode of video function. 
*
* PARAMETERS
*	kal_uint8 mode
* 
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_set_em_mode(kal_uint8 mode)
{
   vid_context_p->em_mode=mode;
}

/*************************************************************************
* FUNCTION
*	media_vid_get_em_mode
*
* DESCRIPTION
*	This function is to get engineer mode of video function. 
*
* PARAMETERS
*	None
* 
*
* RETURNS
*	kal_uint8 mode
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_uint8 media_vid_get_em_mode(void)
{
   return vid_context_p->em_mode;
}

/*************************************************************************
* FUNCTION
*	media_vid_get_current_play_time
*
* DESCRIPTION
*	This function is to get current time when video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint64* time
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_get_current_play_time_req(module_type src_mod_id)
{
	vid_vdo_get_play_time_req(src_mod_id);
}

/*************************************************************************
* FUNCTION
*	media_vid_get_current_play_time_block_req
*
* DESCRIPTION
*	This function is block to get current time when video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint64* time
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_get_current_play_time_block_req(module_type src_mod_id, kal_uint64 *time)
{
	vid_vdo_get_play_time_block_req(src_mod_id);
	VID_WAIT_EVENT(VID_EVT_GET_INFO);
	if(time)
		*time=vid_current_time;
}

#if defined (__MED_VID_MOD__) 
/*************************************************************************
* FUNCTION
*	media_vid_get_current_play_time
*
* DESCRIPTION
*	This function is to get current time when video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint64* time
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_get_current_play_time(module_type src_mod_id, kal_uint64* time)
{
   video_dec_get_play_time(time);
   vid_context_p->current_time=*time;
   if((*time)>=vid_context_p->total_time && 
      VID_IN_STATE(VID_VISUAL_PLAY_FINISH) &&
      VID_AUD_IN_STATE(VID_AUDIO_PLAY_HIGH_SPEED))
   {
      vid_send_audio_play_finish_ind(MED_RES_END_OF_FILE);
   }
      
}

/*************************************************************************
* FUNCTION
*	media_vid_get_decode_time
*
* DESCRIPTION
*	This function is to get current time by the given frame index. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint32 frame_no
*  kal_uint64* time
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_get_decode_time(module_type src_mod_id, kal_uint32 frame_no, kal_uint64* time)
{
   if(Video_GetDecodeTime(frame_no, time)==MEDIA_STATUS_OK)
      return MED_RES_OK;
   else
      return MED_RES_ERROR;
}

/*************************************************************************
* FUNCTION
*	media_vid_get_current_record_time
*
* DESCRIPTION
*	This function is to get current time when video recording. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint64* time
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_get_current_record_time(module_type src_mod_id, kal_uint64* time)
{
   *time=video_enc_get_current_time();
}

/*************************************************************************
* FUNCTION
*	media_vid_get_current_frame_num
*
* DESCRIPTION
*	This function is to get current frame number when video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint32* frame_num
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_get_current_frame_num(module_type src_mod_id, kal_uint32* frame_num)
{
   *frame_num=video_dec_get_current_frame_no();
}

/*************************************************************************
* FUNCTION
*	media_vid_frame_to_time
*
* DESCRIPTION
*	This function is to map given frame number to the time. 
*
* PARAMETERS
*	kal_uint32 frame
*  kal_uint32 *time
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_frame_to_time(kal_uint32 frame, kal_uint32 *time)
{
   if((video_dec_frame2time(frame, time))==MEDIA_STATUS_OK)
      return MED_RES_OK;
   else
      return MED_RES_ERROR;
}

/*************************************************************************
* FUNCTION
*	media_vid_time_to_frame
*
* DESCRIPTION
*	This function is to map given time to the frame number. 
*
* PARAMETERS
*	kal_uint32 time
*  kal_uint32 *frame
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_time_to_frame(kal_uint32 time, kal_uint32 *frame)
{
   if((video_dec_time2frame(time, frame))==MEDIA_STATUS_OK)
      return MED_RES_OK;
   else
      return MED_RES_ERROR;
}


/*************************************************************************
* FUNCTION
*	media_vid_check_unfinished_file
*
* DESCRIPTION
*	This function is to check unfinished video recording file. 
*
* PARAMETERS
*	module_type src_mod_id
* 
*
* RETURNS
*	True/False
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool media_vid_check_unfinished_file(module_type src_mod_id)
{
   return vid_check_unfinished_file();
}

/*************************************************************************
* FUNCTION
*	media_vid_clean_temp_files
*
* DESCRIPTION
*	This function is to clean temp files. 
*
* PARAMETERS
*	module_type src_mod_id
* 
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_clean_temp_files(module_type src_mod_id)
{
   vid_clean_temp_files();
}

#endif /* __MED_VID_MOD__ */

#if defined (__MED_MJPG_MOD__) 
/*************************************************************************
* FUNCTION
*	media_vid_get_current_play_time
*
* DESCRIPTION
*	This function is to get current time when video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint64* time
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_get_current_play_time(module_type src_mod_id, kal_uint64* time)
{
   *time=((MAX(vid_context_p->current_frame_num,0))*vid_context_p->period+
         (vid_context_p->play_speed>0?1:-1)*vid_context_p->time_elapsed);
}

/*************************************************************************
* FUNCTION
*	media_vid_get_decode_time
*
* DESCRIPTION
*	This function is to get current time by the given frame index. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint32 frame_no
*  kal_uint64* time
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_vid_get_decode_time(module_type src_mod_id, kal_uint32 frame_no, kal_uint64* time)
{
   *time=0;
   return MED_RES_OK;
}

/*************************************************************************
* FUNCTION
*	media_vid_get_current_record_time
*
* DESCRIPTION
*	This function is to get current time when video recording. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint64* time
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_get_current_record_time(module_type src_mod_id, kal_uint64* time)
{
   *time=MIN(((MAX(vid_context_p->current_frame_num,0))*vid_context_p->period), VID_CLIP_LENGTH*1000);
}

/*************************************************************************
* FUNCTION
*	media_vid_get_current_frame_num
*
* DESCRIPTION
*	This function is to get current frame number when video playing. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint32* frame_num
*
* RETURNS
*	The result of this action.
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_vid_get_current_frame_num(module_type src_mod_id, kal_uint32* frame_num)
{
   *frame_num=(kal_uint32)MAX(vid_context_p->current_frame_num,0);
}

/* dummy code */
kal_int32 media_vid_frame_to_time(kal_uint32 frame, kal_uint32 *time)
{
   return MED_RES_ERROR;
}

kal_int32 media_vid_time_to_frame(kal_uint32 time, kal_uint32 *frame)
{
   return MED_RES_ERROR;
}


kal_bool media_vid_check_unfinished_file(module_type src_mod_id)
{
   return KAL_FALSE;
}

void media_vid_clean_temp_files(module_type src_mod_id)
{
}

#endif /* __MED_MJPG_MOD__ */

#endif /* __MED_VID_MOD__  || __MED_MJPG_MOD__ */

#endif /* MED_NOT_PRESENT */



