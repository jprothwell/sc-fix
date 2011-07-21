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
 *	vid_main.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary global functions/variables of video module.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT


#ifdef __MED_MJPG_MOD__

#ifndef _VID_MAIN_H
#define _VID_MAIN_H

#define VID_CLIP_LENGTH   15   /* second */

#define VID_RES_VDOREC_SUCCEED								0
#define VID_RES_VDOREC_DISK_FULL								2001
#define VID_RES_VDOREC_REACH_SIZE_LIMIT					2002
#define VID_RES_VDOREC_RECORD_ALREADY_STOP				2003


typedef enum
{
   VID_EVT_NOWAIT    = 0x0000,
   VID_EVT_RECORD	     = 0x0001,
   VID_EVT_STOP         = 0x0002,
   VID_EVT_OPEN_FILE      = 0x0004,
   VID_EVT_CLOSE_FILE      = 0x0008,
   VID_EVT_PLAY         = 0x0010,
   VID_EVT_SEEK         = 0x0020,
   VID_EVT_PAUSE        = 0x0040,
   VID_EVT_RESUME        = 0x0080,
   VID_EVT_SNAPSHOT      = 0x0100,
   VID_EVT_GET_IFRAME      = 0x0200,
   VID_EVT_GET_INFO      = 0x0400, 
   VID_EVT_FILE_MERGE      = 0x0800,
   VID_EVT_OPEN      = 0x1000,
   VID_EVT_CLOSE      = 0x2000,
   VID_EVT_SET_DISPLAY   = 0x4000,
   VID_EVT_PREVIEW           = 0x8000
} 
vid_wait_event_enum;



#if 0
#define VID_WAIT_EVENT(evt_) do{ \
         kal_uint32 retrieved_events;\
         VID_WAIT_EVT(evt_, __LINE__);\
         kal_retrieve_eg_events(vid_context_p->vid_event, (evt_), \
         KAL_OR_CONSUME, &retrieved_events, KAL_SUSPEND); }while(0) 


#define VID_SET_EVENT(evt_) do{ \
                              kal_set_eg_events(vid_context_p->vid_event, (evt_),KAL_OR);\
                              VID_SET_EVT(evt_,__LINE__); \
                              } while(0)
#elif 1
#define MAX_VID_SEM 2
extern HANDLE g_vid_sem;
#define VID_WAIT_EVENT(evt_) COS_WaitForSemaphore(g_vid_sem,COS_WAIT_FOREVER)
#if 0
do{ \
	TRACE_EF(g_sw_MP4," VID_WAIT_EVENT = 0x%x", evt_); \
	while(1) \
	{ \
		if(vid_sem & evt_) \
		{ \
		  	vid_sem = (vid_sem) & (!((U32)(evt_))); \
			break; \
		} \
		else \
		{ \
		} \
	} \
 }while(0) 

//			 COS_Sleep(2); 
#endif

#define VID_SET_EVENT(evt_) COS_ReleaseSemaphore(g_vid_sem)
#if 0 
do{ \
		TRACE_EF(g_sw_MP4," VID_SET_EVENT = 0x%x", evt_); \
		vid_sem |= evt_; \
} while(0)
#endif
#else
#define MAX_VID_SEM 2
extern HANDLE Vid_Semaphore[];

#define  VID_WAIT_EVENT(evt_)do{ \
	 switch( evt_) \
	 { \
 	 	case VID_EVT_SEEK: \
		{ \
			COS_WaitForSemaphore(Vid_Semaphore[0],COS_WAIT_FOREVER); \
			break; \
		} \
 	 	case VID_EVT_STOP: \
		{ \
			COS_WaitForSemaphore(Vid_Semaphore[1],COS_WAIT_FOREVER); \
			break; \
		} \
		default: \
			break; \
	 } \
    }while(0) 

#define VID_SET_EVENT(evt_) do{ \
	 switch( evt_) \
	 { \
 	 	case VID_EVT_SEEK: \
		{ \
			COS_ReleaseSemaphore(Vid_Semaphore[0]); \
			break; \
		} \
 	 	case VID_EVT_STOP: \
		{ \
			COS_ReleaseSemaphore(Vid_Semaphore[1]); \
			break; \
		} \
		default: \
			break; \
	 } \
	}while(0)
#endif


/*==== PROTOTYPE ===========*/
/* main */
void vid_startup_hdlr( ilm_struct *ilm_ptr);
void vid_main( ilm_struct *ilm_ptr);
kal_bool vid_init(void);

/* msg handler */
kal_uint32 vid_get_current_file_size(void);
kal_uint16 vid_get_disc_free_space(kal_wchar* dir_name, kal_uint32 *free);
kal_int16 vid_start_video_record(void);
kal_int16 vid_stop_video_record(void);
kal_int16 vid_pause_video_record(void);
kal_int16 vid_resume_video_record(void);

void vid_preview_check(void* arg);
void vid_preview_req_hdlr(ilm_struct *ilm_ptr);
void vid_record_req_hdlr(ilm_struct *ilm_ptr);
void vid_stop_req_hdlr(ilm_struct *ilm_ptr);
void vid_file_merge_req_hdlr(ilm_struct *ilm_ptr);
void vid_open_file_req_hdlr(ilm_struct *ilm_ptr);
void vid_close_file_req_hdlr(ilm_struct *ilm_ptr);
void vid_open_req_hdlr(ilm_struct *ilm_ptr);
void vid_close_req_hdlr(ilm_struct *ilm_ptr);
void vid_play_finish_hdlr(void);
void vid_play_req_hdlr(ilm_struct *ilm_ptr);
void vid_seek_req_hdlr(ilm_struct *ilm_ptr);
void vid_pause_req_hdlr(ilm_struct *ilm_ptr);
void vid_resume_req_hdlr(ilm_struct *ilm_ptr);
void vid_snapshot_req_hdlr(ilm_struct *ilm_ptr);
void vid_get_iframe_req_hdlr(ilm_struct *ilm_ptr);
void vid_set_display_req_hdlr(ilm_struct *ilm_ptr);

void vid_decode_event_ind_hdlr(ilm_struct *ilm_ptr);
#ifdef __MED_MJPG_AUDIO__            
void vid_audio_play_finish_ind_hdlr(ilm_struct *ilm_ptr);
#endif
void vid_stop_unfinished_task(void);
kal_bool vid_is_audio_channel_available(void);


/* ilm */
void vid_send_record_req(module_type src_mod_id, void* data);
void vid_send_stop_req(module_type src_mod_id);
void vid_send_file_merge_req(module_type src_mod_id, void* data);
void vid_send_open_file_req(module_type src_mod_id, void* data, kal_uint16 seq_num, kal_uint16 cache_in_mem,void (*videoplayer_drawtime_hdlr)(kal_int32));
void vid_send_close_file_req(module_type src_mod_id);
void vid_send_open_req(module_type src_mod_id, void* data);
void vid_send_close_req(module_type src_mod_id);
void vid_send_play_req(module_type src_mod_id, void* data);
void vid_send_seek_req(module_type src_mod_id, void* data);
void vid_send_pause_req(module_type src_mod_id);
void vid_send_resume_req(module_type src_mod_id);
void vid_send_snapshot_req(module_type src_mod_id, void* data);
void vid_send_get_iframe_req(module_type src_mod_id, void* data);
void vid_send_record_finish_ind(kal_int16 result);
void vid_send_file_merge_done_ind(kal_int16 result);
void vid_send_file_ready_ind(kal_int16 result);
void vid_send_play_finish_ind(kal_int16 result);
void vid_send_decode_event_ind(kal_uint8 event);
void vid_send_seek_done_ind(kal_int16 result);
void vid_send_iframe_ready_ind(kal_int16 result);
void vid_send_set_display_req(module_type src_mod_id, kal_uint8 device);
void vid_send_audio_play_finish_ind(kal_int16 result);
void vid_send_display_interface_req(module_type src_mod_id);
void vid_display_interface_req_hdlr(ilm_struct *ilm_ptr);



/* api */
void vid_set_result(kal_int32 result);
void vid_set_image_size(kal_uint16 x, kal_uint16 y);
void vid_set_total_time(kal_uint64 total_time);
void vid_set_current_time(kal_uint64 current_time);

#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
void vid_file_info(kal_uint16 width, kal_uint16 height,
                   kal_uint32 nbFrames, kal_uint32 totalTime);
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */

#endif /* _VID_MAIN_H */

#endif /* __MED_MJPG_MOD__ */

#endif /* MED_NOT_PRESENT */


