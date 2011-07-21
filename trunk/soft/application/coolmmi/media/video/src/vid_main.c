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
 *   vid_main.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary functions of video module.
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
#include "fat_fs.h"

/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"


/* local includes */
#include "med_global.h"
#include "med_status.h"
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "vid_main.h"


#include "mmi_trace.h"

#if defined (__MED_VID_MOD__) || defined (__MED_MJPG_MOD__)

vid_context_struct*   vid_context_p = &(med_context.vid_context);

#define DUMMY_VIDEO_FILE_CONTENT L"d:\\video\\dummy.3gp\0\0\0\0\0\0\0\0\0\0\0\0"

HANDLE g_vid_sem = 0;
HANDLE  Vid_Semaphore[MAX_VID_SEM] = {0}; //new add 

/*==== FUNCTIONS ===========*/

#ifdef __MED_VID_MOD__

void vid_create_dummy_video_file(void)
{
   FS_HANDLE fd=-1;
   kal_uint32 len;
   kal_int32 result;

   if((fd = MMI_FS_Open((kal_wchar*)VID_DUMMY_INIT_FILE, FS_READ_ONLY))>=0)
   {
      MMI_FS_Close(fd);
      return;
   }
   else if((fd = MMI_FS_Open((kal_wchar*)VID_INIT_FILE, FS_READ_ONLY))>=0)
   {
      MMI_FS_Close(fd);
      //FS_Rename((kal_wchar*)VID_INIT_FILE, (kal_wchar*)VID_DUMMY_INIT_FILE);
      return;
   }
   fd = MMI_FS_Open((kal_wchar*)VID_DUMMY_INIT_FILE, FS_CREATE|FS_READ_WRITE);
   if (fd>=0)
   {
      result = MMI_FS_Write(fd, 
                       DUMMY_VIDEO_FILE_CONTENT,
                       kal_wstrlen(DUMMY_VIDEO_FILE_CONTENT)*ENCODE_BYTE,
                       &len);
      MMI_FS_Close(fd);
   }
   
}

#endif

/******************************************************************
 * FUNCTION
 *    vid_startup_hdlr
 * DESCRIPTION
 *    This function is handle startup procedure of video module.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/
void vid_startup_hdlr( ilm_struct *ilm_ptr)
{
   vid_context_p->src_mod = ilm_ptr->src_mod_id;
#ifdef __MED_VID_MOD__
   vid_create_dummy_video_file();
#endif
}

/******************************************************************
 * FUNCTION
 *    vid_main
 * DESCRIPTION
 *    This function is main message dispatching function of video module.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/
 void vid_Rec_stop_req_hdlr(ilm_struct *ilm_ptr);
void vid_stop_req_hdlr(ilm_struct *ilm_ptr);
void vid_record_req_hdlr(ilm_struct *ilm_ptr);
void vid_get_play_time_req_hdlr(ilm_struct *ilm_ptr);
void vid_get_play_time_block_req_hdlr(ilm_struct *ilm_ptr);
void vid_seek_req_hdlr(ilm_struct *ilm_ptr);
void vid_Rec_previewStop_req_hdlr(ilm_struct *ilm_ptr);
void vid_Rec_pause_req_hdlr(ilm_struct *ilm_ptr); //chenhe for jasperII
void vid_Rec_resume_req_hdlr(ilm_struct *ilm_ptr); //chenhe for jasperII
void vid_Rec_set_param_req_hdlr(ilm_struct *ilm_ptr);
void vid_send_record_finish_ind2(ilm_struct *ilm_ptr);
void vid_display_set_req_hdlr(ilm_struct *ilm_ptr);
void vid_open_file_req_hdlr(ilm_struct *ilm_ptr);
void vid_play_req_hdlr(ilm_struct *ilm_ptr);
void vid_send_seek_done_ind2(ilm_struct *ilm_ptr);
void vid_send_file_ready_ind2(ilm_struct *ilm_ptr);

void vid_send_play_finish_ind2(ilm_struct *ilm_ptr);

void vid_main( ilm_struct *ilm_ptr)
{
    mmi_trace(g_sw_MED_Vid, "Excute func : vid_main(%d)", ilm_ptr->msg_id);
    mmi_trace(g_sw_MED_Vid, "MSG_ID_MED_VID_CODE_BEGIN = %d", MSG_ID_MED_VID_CODE_BEGIN);
    mmi_trace(g_sw_MED_Vid, "MSG_ID_MED_VID_CODE_END = %d", MSG_ID_MED_VID_CODE_END);

   switch( ilm_ptr->msg_id )
   {
#ifdef __MED_MJPG_MOD__
#ifdef VIDEO_RECORDER_SUPPORT
      case MSG_ID_MEDIA_VID_PREVIEW_REQ:
	  	mmi_trace(TRUE, "Excute func : elisa _vid_main  MSG_ID_MEDIA_VID_PREVIEW_REQ (%d)", ilm_ptr->msg_id);
         vid_preview_req_hdlr(ilm_ptr);
         break;
      /* message from MMIAPP */
      case MSG_ID_MEDIA_VID_RECORD_REQ:
	  	mmi_trace(TRUE, "Excute func : elisa _vid_main  MSG_ID_MEDIA_VID_RECORD_REQ (%d)", ilm_ptr->msg_id);
         vid_record_req_hdlr(ilm_ptr);
         break;
#endif         
      case MSG_ID_MEDIA_VID_STOP_REQ:
	  	   mmi_trace(TRUE, "Excute func : elisa _vid_main  MSG_ID_MEDIA_VID_STOP_REQ (%d)", ilm_ptr->msg_id);
         vid_stop_req_hdlr(ilm_ptr);
         break;
//chenhe for jasperII ++
#ifdef VIDEO_RECORDER_SUPPORT	
	case MSG_ID_MEDIA_VIDREC_STOP_REQ:
		mmi_trace(1,"MSG_ID_MEDIA_VIDREC_STOP_REQ");
		vid_Rec_stop_req_hdlr(ilm_ptr);//chenhe for jasperII
		break;
	case MSG_ID_MEDIA_VIDREC_PREVIEW_STOP_REQ:
		mmi_trace(1,"MSG_ID_MEDIA_VIDREC_PREVIEW_STOP_REQ");
		vid_Rec_previewStop_req_hdlr(ilm_ptr);
		break;
	case MSG_ID_MEDIA_VIDREC_PAUSE_REQ:
		mmi_trace(1,"MSG_ID_MEDIA_VIDREC_PAUSE_REQ");
		
		vid_Rec_pause_req_hdlr(ilm_ptr);
		break;

	case MSG_ID_MEDIA_VIDREC_RESUME_REQ:
		mmi_trace(1,"MSG_ID_MEDIA_VIDREC_RESUME_REQ"); 
		
		vid_Rec_resume_req_hdlr(ilm_ptr);
		break;	
	case MSG_ID_MEDIA_VIDREC_SET_PARAM_REQ:
		mmi_trace(1,"MSG_ID_MEDIA_VIDREC_SET_PARAM_REQ"); 
		
		vid_Rec_set_param_req_hdlr(ilm_ptr);    
		break;
	case MSG_ID_MEDIA_VID_RECORD_FINISH_IND:
		vid_send_record_finish_ind2(ilm_ptr);
		break;
#endif		        
	case MSG_ID_MEDIA_VID_DISPLAY_SET_REQ:
		mmi_trace(1," MSG_ID_MEDIA_VID_DISPLAY_SET_REQ"); 
		vid_display_set_req_hdlr(ilm_ptr);
		break;
//chenhe for jasperII -- 			
      case MSG_ID_MEDIA_VID_FILE_MERGE_REQ:
         vid_file_merge_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_VID_OPEN_FILE_REQ:
	  	    mmi_trace(TRUE, "Excute func : elisa _vid_main  MSG_ID_MEDIA_VID_OPEN_FILE_REQ (%d)", ilm_ptr->msg_id);
         vid_open_file_req_hdlr(ilm_ptr);
         break;
//	case MSG_ID_MEDIA_VID_FILE_READY_IND:
	//	vid_send_file_ready_ind2(ilm_ptr);
	//	break;
		 	
      case MSG_ID_MEDIA_VID_CLOSE_FILE_REQ:
	  	   mmi_trace(TRUE, "Excute func : elisa _vid_main  MSG_ID_MEDIA_VID_CLOSE_FILE_REQ (%d)", ilm_ptr->msg_id);
         vid_close_file_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_VID_OPEN_REQ:
	  	 mmi_trace(TRUE, "Excute func : elisa _vid_main  MSG_ID_MEDIA_VID_OPEN_REQ (%d)", ilm_ptr->msg_id);
         vid_open_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_VID_CLOSE_REQ:
	  	mmi_trace(TRUE, "Excute func : elisa _vid_main  MSG_ID_MEDIA_VID_CLOSE_REQ (%d)", ilm_ptr->msg_id);
         vid_close_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_VID_PLAY_REQ:

 		mmi_trace(TRUE, "Excute func : elisa_ _vid_main( MSG_ID_MEDIA_VID_PLAY_REQ%d)", ilm_ptr->msg_id);
         vid_play_req_hdlr(ilm_ptr);
         break;
	case MSG_ID_MEDIA_VID_PLAY_FINISH_IND:
		vid_send_play_finish_ind2(ilm_ptr);
		break;
      case MSG_ID_MEDIA_VID_SEEK_REQ:
	  	
 		mmi_trace(TRUE, "Excute func : elisa_ _vid_main( MSG_ID_MEDIA_VID_SEEK_REQ%d)", ilm_ptr->msg_id);
         vid_seek_req_hdlr(ilm_ptr);
         break;
	case MSG_ID_MEDIA_VID_SEEK_DONE_IND:
		vid_send_seek_done_ind2(ilm_ptr);
		break;
      case MSG_ID_MEDIA_VID_PAUSE_REQ:
	  	mmi_trace(TRUE, "Excute func : elisa_ _vid_main( MSG_ID_MEDIA_VID_PAUSE_REQ%d)", ilm_ptr->msg_id);
         vid_pause_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_VID_SET_DISPLAY_REQ:
	  	mmi_trace(TRUE, "Excute func : elisa_ _vid_main( MSG_ID_MEDIA_VID_SET_DISPLAY_REQ%d)", ilm_ptr->msg_id);
         vid_set_display_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_VID_PLAY_GET_PLAY_TIME_REQ:
         vid_get_play_time_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_VID_PLAY_GET_PLAY_TIME_BLOCK_REQ:
		 vid_get_play_time_block_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_VID_DISPLAY_INTERFACE_REQ:
         vid_display_interface_req_hdlr(ilm_ptr);
         break;

#ifdef __MED_MJPG_AUDIO__            
      case MSG_ID_MEDIA_VID_AUDIO_PLAY_FINISH_IND: 
	  	mmi_trace(TRUE, "Excute func : elisa_ _vid_main( MSG_ID_MEDIA_VID_AUDIO_PLAY_FINISH_IND%d)", ilm_ptr->msg_id);
         vid_audio_play_finish_ind_hdlr(ilm_ptr);
         break;
#endif
#endif
      default:
         break;
   }
}



/******************************************************************
 * FUNCTION
 *    vid_init
 * DESCRIPTION
 *    This function is used to init video module of media task.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/
kal_bool vid_init(void)
{
	mmi_trace(TRUE, "Excute func : vid_init()");
   vid_context_p->state = VID_IDLE;
   vid_context_p->aud_state = VID_AUDIO_IDLE;   
   vid_context_p->vid_event = kal_create_event_group("vid_events");
   vid_context_p->image_path_open=KAL_FALSE;
   vid_context_p->em_mode = VID_EM_MODE_DISABLED;
#ifdef __MED_VID_MOD__
   vid_context_p->audio_frame_num=0;
   vid_context_p->total_frame_num=0;
   vid_context_p->current_time=0;
   vid_context_p->total_time=0;
   vid_context_p->display_width=0;
   vid_context_p->display_height=0;
   vid_context_p->image_width=0;
   vid_context_p->image_height=0;
   vid_context_p->file_size=0;
   vid_context_p->storing_path[0]=0;
#endif
#ifdef __MED_MJPG_MOD__
   vid_context_p->total_frame_num=0;
   vid_context_p->current_time=0;
   vid_context_p->total_time=0;
   vid_context_p->display_width=0;
   vid_context_p->display_height=0;
   vid_context_p->image_width=0;
   vid_context_p->image_height=0;
   vid_context_p->file_size=0;
   vid_context_p->storing_path[0]=0;
#endif
   return KAL_TRUE;
}

#endif /* __MED_VID_MOD__ */
#endif /* MED_NOT_PRESENT */


