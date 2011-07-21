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
 *	vid_ilm.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes send-ilm related funcions of image module.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES ===========*/

/* system includes */
#include "kal_release.h" 
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"       	/* task message communiction */
#include "stacklib.h"        	/* basic type for dll, evshed, stacktimer */
#include "event_shed.h"       	/* event scheduler */
#include "stack_timer.h"      	/* stack timer */

/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "fat_fs.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"

/* local includes */
#include "med_global.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "mmi_trace.h"
#include "vid_main.h"
#include "med_main.h"
#if defined( __MED_VID_MOD__)
//#include "med_v_struct.h" /* wufasong delete 2007.06.16 since no this file. */
#endif

#if defined( __MED_VID_MOD__) || defined (__MED_MJPG_MOD__)

/*************************************************************************
* FUNCTION
*   vid_send_preview_req
*
* DESCRIPTION
*   This function is to send video preview request to media task
*
* PARAMETERS
*   module_type src_mod_id
*   void* data
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void * kal_mem_cpy(void* dest, const void* src, kal_uint32 size);
HANDLE GetMmiTaskHandle(module_type mod);

void vid_send_preview_req(module_type src_mod_id, void* data)
{
   media_vid_preview_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_preview_req_struct*)
            construct_local_para(sizeof(media_vid_preview_req_struct), TD_CTRL);  
                            
   kal_mem_cpy(((kal_uint8*)msg_p)+sizeof(local_para_struct),
   		    ((kal_uint8*)data)   +sizeof(local_para_struct),
   		    sizeof(media_vid_preview_req_struct)-sizeof(local_para_struct));

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_PREVIEW_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   mmi_trace(g_sw_VdoRec," vid_send_preview_req  MSG_ID_MEDIA_VID_PREVIEW_REQ");
   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_record_req
*
* DESCRIPTION
*	This function is to send video record request to media task
*
* PARAMETERS
*	module_type src_mod_id
*  media_vid_record_req_struct* data
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_record_req(module_type src_mod_id, void* data)
{
   media_vid_record_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_record_req_struct*)
            construct_local_para(sizeof(media_vid_record_req_struct), TD_CTRL);  
                            
   kal_mem_cpy(((kal_uint8*)msg_p)+sizeof(local_para_struct),
   		    ((kal_uint8*)data)   +sizeof(local_para_struct),
   		    sizeof(media_vid_record_req_struct)-sizeof(local_para_struct));

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_RECORD_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;
   mmi_trace(g_sw_VdoRec," vid_send_record_req MSG_ID_MEDIA_VID_RECORD_REQ");
   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_stop_req
*
* DESCRIPTION
*	This function is to send video stop request to media task
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
void vid_send_stop_req(module_type src_mod_id)
{
   media_vid_stop_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_stop_req_struct*)
            construct_local_para(sizeof(media_vid_stop_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_STOP_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}
void vid_display_set_req(module_type src_mod_id,kal_uint16 ZoomWidth, kal_uint16 ZoomHeight, kal_int16 startX, kal_int16 startY, kal_int16 cutX, kal_int16 cutY, kal_int16 cutW, kal_int16 cutH, kal_uint16 rotate)
{
   media_vid_display_set_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_display_set_req_struct*)
            construct_local_para(sizeof(media_vid_display_set_req_struct), TD_CTRL);  

   msg_p->ZoomWidth = ZoomWidth;
   msg_p->ZoomHeight = ZoomHeight;
   msg_p->startX = startX;
   msg_p->startY = startY;
   msg_p->cutX= cutX;
   msg_p->cutY= cutY;
   msg_p->cutW= cutW;
   msg_p->cutH= cutH;
   msg_p->rotate=rotate;
   
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   	TRACE_EF(g_sw_MP4," vid_display_set_req MSG_ID_MEDIA_VID_DISPLAY_SET_REQ");

   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_DISPLAY_SET_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}
void vid_rec_send_stop_req(module_type src_mod_id)
{
   media_vid_stop_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
	mmi_trace(1,"vid_rec_send_stop_req");
   msg_p = (media_vid_stop_req_struct*)
            construct_local_para(sizeof(media_vid_stop_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VIDREC_STOP_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

void vid_vdo_get_play_time_req(module_type src_mod_id)
{
   media_vid_stop_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_stop_req_struct*)
            construct_local_para(sizeof(media_vid_stop_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_PLAY_GET_PLAY_TIME_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

void vid_vdo_get_play_time_block_req(module_type src_mod_id)
{
   media_vid_stop_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_stop_req_struct*)
            construct_local_para(sizeof(media_vid_stop_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_PLAY_GET_PLAY_TIME_BLOCK_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}
void vid_rec_send_previewStop_req(module_type src_mod_id)
{
   media_vid_stop_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
	mmi_trace(1,"vid_rec_send_stop_req");
   msg_p = (media_vid_stop_req_struct*)
            construct_local_para(sizeof(media_vid_stop_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VIDREC_PREVIEW_STOP_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

//chenhe for jasperII-

/*************************************************************************
* FUNCTION
*	vid_send_file_merge_req
*
* DESCRIPTION
*	This function is to send video file merge request to media task
*
* PARAMETERS
*	module_type src_mod_id
*  void* data
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_file_merge_req(module_type src_mod_id, void* data)
{
   media_vid_file_merge_req_struct* msg_p = NULL;
   ilm_struct  *ilm_ptr = NULL;

   msg_p = (media_vid_file_merge_req_struct*)
            construct_local_para(sizeof(media_vid_file_merge_req_struct), TD_CTRL);  
                            
   msg_p->file_path = (kal_wchar*)data;
   
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_FILE_MERGE_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_open_file_req
*
* DESCRIPTION
*	This function is to send video open file request to media task
*
* PARAMETERS
*	module_type src_mod_id
*  media_vid_open_file_req_struct* data
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_open_file_req(module_type src_mod_id, void* data, kal_uint16 seq_num, kal_uint16 cache_in_mem,void (*videoplayer_drawtime_hdlr)(kal_int32))
{
   media_vid_open_file_req_struct* msg_p = NULL;
   ilm_struct  *ilm_ptr = NULL;

   msg_p = (media_vid_open_file_req_struct*)
            construct_local_para(sizeof(media_vid_open_file_req_struct), TD_CTRL);  
                            
   msg_p->file_name_p = (kal_wchar*)data;
   msg_p->seq_num=seq_num;
   msg_p->cache_in_mem=cache_in_mem;
   msg_p->videoplayer_drawtime_hdlr=videoplayer_drawtime_hdlr;
   
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_OPEN_FILE_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;
	mmi_trace(1,"vid_send_open_file_req,success MSG_ID_MEDIA_VID_OPEN_FILE_REQ = %d", MSG_ID_MEDIA_VID_OPEN_FILE_REQ);
   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_open_file_req
*
* DESCRIPTION
*	This function is to send video close file request to media task
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
void vid_send_close_file_req(module_type src_mod_id)
{
   media_vid_close_file_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_close_file_req_struct*)
            construct_local_para(sizeof(media_vid_close_file_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_CLOSE_FILE_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_open_req
*
* DESCRIPTION
*	This function is to send video open request to media task
*
* PARAMETERS
*	module_type src_mod_id
*  void* data
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_open_req(module_type src_mod_id, void* data)
{
   media_vid_open_req_struct* msg_p = NULL;
   ilm_struct  *ilm_ptr = NULL;
   vid_open_struct* open=(vid_open_struct*)data;

   msg_p = (media_vid_open_req_struct*)
            construct_local_para(sizeof(media_vid_open_req_struct), TD_CTRL);  
                            
   msg_p->media_mode = open->media_mode;
   msg_p->media_type=open->media_type;
   msg_p->data=open->data;
   msg_p->data_size=open->data_size;
   msg_p->seq_num=open->seq_num;
   msg_p->max_width=open->max_width;
   msg_p->max_height=open->max_height;
   msg_p->blocking=open->blocking;
   msg_p->play_audio=open->play_audio;
   
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_OPEN_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_open_file_req
*
* DESCRIPTION
*	This function is to send video close request to media task
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
void vid_send_close_req(module_type src_mod_id)
{
   media_vid_close_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_close_req_struct*)
            construct_local_para(sizeof(media_vid_close_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_CLOSE_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_play_req
*
* DESCRIPTION
*	This function is to send video play request to media task
*
* PARAMETERS
*	module_type src_mod_id
*  media_vid_play_req_struct* data
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_play_req(module_type src_mod_id, void* data)
{
   media_vid_play_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
   vid_play_struct* play=(vid_play_struct*)data;

   msg_p = (media_vid_play_req_struct*)
            construct_local_para(sizeof(media_vid_play_req_struct), TD_CTRL);  
                            

   msg_p->display_width = play->display_width;
   msg_p->display_height = play->display_height;
   msg_p->display_offset_x = play->display_offset_x;
   msg_p->display_offset_y = play->display_offset_y;
   msg_p->image_buffer_p = play->image_buffer_p;
   msg_p->image_buffer_size = play->image_buffer_size;

   msg_p->play_audio = play->play_audio;
   msg_p->display_device = play->display_device;
   msg_p->audio_path = play->audio_path;
   msg_p->play_speed = play->play_speed;
   msg_p->seq_num = play->seq_num;
   msg_p->lcd_rotate = play->lcd_rotate;
   msg_p->repeats = play->repeats;
   
   msg_p->lcd_id = play->lcd_id;
   msg_p->lcd_start_x = play->lcd_start_x;
   msg_p->lcd_start_y = play->lcd_start_y;
   msg_p->lcd_end_x = play->lcd_end_x;
   msg_p->lcd_end_y = play->lcd_end_y;
   msg_p->roi_offset_x = play->roi_offset_x;
   msg_p->roi_offset_y = play->roi_offset_y;
   msg_p->update_layer = play->update_layer;
   msg_p->hw_update_layer = play->hw_update_layer;
   msg_p->video_display_offset_x = play->video_display_offset_x;
   msg_p->video_display_offset_y = play->video_display_offset_y;
#if (defined(MT6228)||defined(MT6229))
   msg_p->image_pitch_mode = play->image_pitch_mode;
   msg_p->image_data_format = play->image_data_format;
   msg_p->image_pitch_bytes = play->image_pitch_bytes;
	/* TV Setting */
   msg_p->tv_output = play->tv_output;
   msg_p->tv_output_buffer1_address = play->tv_output_buffer1_address;
   msg_p->tv_output_buffer2_address = play->tv_output_buffer2_address;
   msg_p->tv_output_buffer_size = play->tv_output_buffer_size;
#endif

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_PLAY_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_seek_req
*
* DESCRIPTION
*	This function is to send video seek request to media task
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint32 position
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_seek_req(module_type src_mod_id, void* data)
{  
   media_vid_seek_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
   vid_seek_struct* seek=(vid_seek_struct*)data;

   msg_p = (media_vid_seek_req_struct*)
            construct_local_para(sizeof(media_vid_seek_req_struct), TD_CTRL);  
                            
   msg_p->get_frame = seek->get_frame;
   msg_p->frame_num = seek->frame_num;
   msg_p->time = seek->time;
   msg_p->display_width = seek->display_width;
   msg_p->display_height = seek->display_height;
   msg_p->display_offset_x = seek->display_offset_x;
   msg_p->display_offset_y = seek->display_offset_y;
   msg_p->image_buffer_p = seek->image_buffer_p;
   msg_p->image_buffer_size = seek->image_buffer_size;
   msg_p->blocking = seek->blocking;
   msg_p->seq_num = seek->seq_num;
#if MT6228_SERIES
	msg_p->image_pitch_mode = seek->image_pitch_mode;							
	msg_p->image_data_format = seek->image_data_format;							
	msg_p->image_pitch_bytes = seek->image_pitch_bytes;							
	/* TV Setting */
	msg_p->tv_output = seek->tv_output;							
	msg_p->tv_output_buffer1_address = seek->tv_output_buffer1_address;							
	msg_p->tv_output_buffer2_address = seek->tv_output_buffer2_address;							
	msg_p->tv_output_buffer_size = seek->tv_output_buffer_size;							
#endif

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_SEEK_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;
	TRACE_EF(1," vid_send_seek_req MSG_ID_MEDIA_VID_SEEK_REQ = %d",MSG_ID_MEDIA_VID_SEEK_REQ);
	TRACE_EF(1," vid_send_seek_req msg_p->display_offset_x  = %d,msg_p->display_offset_y=%d",msg_p->display_offset_x ,msg_p->display_offset_y);

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_pause_req
*
* DESCRIPTION
*	This function is to send video pause request to media task
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
void vid_send_display_interface_req(module_type src_mod_id)
{
   media_vid_display_interface_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_display_interface_req_struct*)
            construct_local_para(sizeof(media_vid_display_interface_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_DISPLAY_INTERFACE_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_pause_req
*
* DESCRIPTION
*	This function is to send video pause request to media task
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
void vid_send_pause_req(module_type src_mod_id)
{
   media_vid_pause_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_pause_req_struct*)
            construct_local_para(sizeof(media_vid_pause_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_PAUSE_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}



void vid_rec_send_pause_req(module_type src_mod_id)
{
   media_vid_pause_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
mmi_trace(1,"vid_rec_send_pause_req");
   msg_p = (media_vid_pause_req_struct*)
            construct_local_para(sizeof(media_vid_pause_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VIDREC_PAUSE_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*   cam_send_set_param_req
*
* DESCRIPTION
*   This function is to send set parameter request message. 
*
* PARAMETERS
*   module_type src_mod_id
*  kal_uint16 param_id
*  kal_int16 value
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_rec_set_param_req(module_type src_mod_id, kal_uint16 param_id, kal_int16 value)
{
   media_cam_set_param_req_struct* msg_p = NULL;
   ilm_struct *ilm_ptr = NULL;
   msg_p = (media_cam_set_param_req_struct*)
            construct_local_para(sizeof(media_cam_set_param_req_struct), TD_CTRL);  

   msg_p->param_id = param_id;
   msg_p->value = value;

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VIDREC_SET_PARAM_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_resume_req
*
* DESCRIPTION
*	This function is to send video resume request to media task
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
void vid_send_resume_req(module_type src_mod_id)
{
   media_vid_resume_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
mmi_trace(1,"vid_send_resume_req");

   msg_p = (media_vid_resume_req_struct*)
            construct_local_para(sizeof(media_vid_resume_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_RESUME_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

void vid_Recsend_resume_req(module_type src_mod_id)
{
   media_vid_resume_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
mmi_trace(1,"vid_Recsend_resume_req");
   msg_p = (media_vid_resume_req_struct*)
            construct_local_para(sizeof(media_vid_resume_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = MOD_MMI;
   ilm_ptr->dest_mod_id = MOD_MED; 
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VIDREC_RESUME_REQ;//;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

void vid_rec_send_resume_req1(module_type src_mod_id)
{
   media_vid_pause_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
mmi_trace(1,"vid_rec_send_resume_req1");
   msg_p = (media_vid_pause_req_struct*)
            construct_local_para(sizeof(media_vid_pause_req_struct), TD_CTRL);  
            
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = MOD_MMI;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VIDREC_RESUME_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_snapshot_req
*
* DESCRIPTION
*	This function is to send video snapshot request to media task
*
* PARAMETERS
*	module_type src_mod_id
*  media_vid_snapshot_req_struct* data
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_snapshot_req(module_type src_mod_id, void* data)
{
   media_vid_snapshot_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
   vid_snapshot_struct* snapshot=(vid_snapshot_struct*)data;

   msg_p = (media_vid_snapshot_req_struct*)
            construct_local_para(sizeof(media_vid_snapshot_req_struct), TD_CTRL);  
                            
   msg_p->image_buffer_p = snapshot->image_buffer_p;
   msg_p->image_buffer_size = snapshot->image_buffer_size;

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_SNAPSHOT_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_get_iframe_req
*
* DESCRIPTION
*	This function is to send video get i-frame request to media task
*
* PARAMETERS
*	module_type src_mod_id
*  media_vid_get_iframe_req_struct* data
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_get_iframe_req(module_type src_mod_id, void* data)
{
   media_vid_get_iframe_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;
   vid_get_iframe_struct* iframe=(vid_get_iframe_struct*)data;

   msg_p = (media_vid_get_iframe_req_struct*)
            construct_local_para(sizeof(media_vid_get_iframe_req_struct), TD_CTRL);  
                            
   msg_p->media_mode = iframe->media_mode;
   msg_p->media_type = iframe->media_type;
   msg_p->data = iframe->data;

   msg_p->display_width = iframe->display_width;
   msg_p->display_height = iframe->display_height;
   msg_p->image_buffer_p = iframe->image_buffer_p;
   msg_p->image_buffer_size = iframe->image_buffer_size;

   msg_p->frame_num = iframe->frame_num;
   msg_p->seq_num = iframe->seq_num;
   msg_p->blocking = iframe->blocking;

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_GET_IFRAME_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_set_display_req
*
* DESCRIPTION
*	This function is to send video set display request to media task
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 device
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_set_display_req(module_type src_mod_id, kal_uint8 device)
{
   media_vid_set_display_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_vid_set_display_req_struct*)
            construct_local_para(sizeof(media_vid_set_display_req_struct), TD_CTRL);  
                            
   msg_p->device = device;

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_SET_DISPLAY_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_record_finish_ind
*
* DESCRIPTION
*	This function is to send video record finished indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_record_finish_ind2(ilm_struct *ilm_ptr)
{
   media_vid_record_finish_ind_struct *ind_p,*source_p;
   ilm_struct       *local_ilm_ptr = (ilm_struct *)NULL;
   
   ind_p = (media_vid_record_finish_ind_struct*)
            construct_local_para(sizeof(media_vid_record_finish_ind_struct), TD_CTRL);  
   source_p=(media_vid_record_finish_ind_struct*)ilm_ptr->local_para_ptr;

mmi_trace(1," vid_send_record_finish_ind2 source_p->result = %d",source_p->result);
	switch(source_p->result)
	{
		case VID_RES_VDOREC_REACH_SIZE_LIMIT:
			ind_p->result = MED_RES_END_OF_FILE;
			break;
		case VID_RES_VDOREC_RECORD_ALREADY_STOP:
			ind_p->result = MED_RES_END_OF_FILE;	
			break;

		case VID_RES_VDOREC_DISK_FULL:
			ind_p->result = 	MED_RES_DISC_FULL;
			break;
		default:
			
			break;
	}

   ind_p->seq_num = source_p->seq_num;

   local_ilm_ptr = allocate_ilm(MOD_MED);
   local_ilm_ptr->src_mod_id = MOD_MED;
   local_ilm_ptr->dest_mod_id = MOD_MMI;
   local_ilm_ptr->sap_id = MED_SAP;
   
   local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_RECORD_FINISH_IND;
   local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   local_ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(local_ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_record_finish_ind
*
* DESCRIPTION
*	This function is to send video record finished indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_record_finish_ind(kal_int16 result)
{
   media_vid_record_finish_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   
   ind_p = (media_vid_record_finish_ind_struct*)
            construct_local_para(sizeof(media_vid_record_finish_ind_struct), TD_CTRL);  

   ind_p->result = result;
   ind_p->seq_num = vid_context_p->seq_num;

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = vid_context_p->src_mod;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_RECORD_FINISH_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_record_finish_ind
*
* DESCRIPTION
*	This function is to send video record finished indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_file_merge_done_ind(kal_int16 result)
{
   media_vid_file_merge_done_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   
   ind_p = (media_vid_file_merge_done_ind_struct*)
            construct_local_para(sizeof(media_vid_file_merge_done_ind_struct), TD_CTRL);  

   ind_p->result = result;

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = vid_context_p->src_mod;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_FILE_MERGE_DONE_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_file_ready_ind
*
* DESCRIPTION
*	This function is to send video file ready indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
/*
void vid_send_file_ready_ind2(ilm_struct *ilm_ptr)
{
   media_vid_file_ready_ind_struct *ind_p , *source_p;
   ilm_struct       *local_ilm_ptr = NULL;
   
   ind_p = (media_vid_file_ready_ind_struct*)
            construct_local_para(sizeof(media_vid_file_ready_ind_struct), TD_CTRL);  
   
   source_p=(media_vid_file_ready_ind_struct*)ilm_ptr->local_para_ptr;

   ind_p->result = source_p->result;
   ind_p->image_width=source_p->image_width;
   ind_p->image_height=source_p->image_height;
   ind_p->total_frame_num=source_p->total_frame_num;
   ind_p->total_time=source_p->total_time;
   ind_p->seq_num=source_p->seq_num;
   ind_p->media_type =source_p->media_type;

   local_ilm_ptr = allocate_ilm(MOD_MED);
   local_ilm_ptr->src_mod_id = MOD_MED;
   local_ilm_ptr->dest_mod_id = MOD_MMI;
   local_ilm_ptr->sap_id = MED_SAP;
   
   local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_FILE_READY_IND;
   local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   local_ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(local_ilm_ptr);

}

*/
/*************************************************************************
* FUNCTION
*	vid_send_file_ready_ind
*
* DESCRIPTION
*	This function is to send video file ready indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_file_ready_ind(kal_int16 result)
{
   media_vid_file_ready_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;

   mmi_trace(TRUE, "Excute func : vid_send_file_ready_ind(result = %d)", result);
   
   ind_p = (media_vid_file_ready_ind_struct*)
            construct_local_para(sizeof(media_vid_file_ready_ind_struct), TD_CTRL);  

   ind_p->result = result;
   ind_p->image_width=vid_context_p->image_width;
   ind_p->image_height=vid_context_p->image_height;
   ind_p->total_frame_num=vid_context_p->total_frame_num;
   ind_p->total_time=vid_context_p->total_time;
   ind_p->seq_num=vid_context_p->seq_num;

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = vid_context_p->src_mod;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_FILE_READY_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
void vid_file_info(kal_uint16 width, kal_uint16 height,
                   kal_uint32 nbFrames, kal_uint32 totalTime)
{
   vid_context_p->image_width     = width;
   vid_context_p->image_height    = height;
   vid_context_p->total_frame_num = nbFrames;
   vid_context_p->total_time      = totalTime;
}
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */

/*************************************************************************
* FUNCTION
*	vid_send_play_finish_ind
*
* DESCRIPTION
*	This function is to send video play finish indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_play_finish_ind2(ilm_struct *ilm_ptr)
{
   media_vid_play_finish_ind_struct *ind_p, *source_p;
   ilm_struct       *local_ilm_ptr = (ilm_struct *)NULL;
   
   ind_p = (media_vid_play_finish_ind_struct*)
            construct_local_para(sizeof(media_vid_play_finish_ind_struct), TD_CTRL);  

   source_p=(media_vid_play_finish_ind_struct*)ilm_ptr->local_para_ptr;

   ind_p->result = source_p->result;
   ind_p->seq_num = source_p->seq_num;

   local_ilm_ptr = allocate_ilm(MOD_MED);
   local_ilm_ptr->src_mod_id = MOD_MED;
   local_ilm_ptr->dest_mod_id = MOD_MMI;
   local_ilm_ptr->sap_id = MED_SAP;
   
   local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_PLAY_FINISH_IND;
   local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   local_ilm_ptr->peer_buff_ptr = NULL;

  mmi_trace(1,"elisa _ vid_send_play_finish_ind   MSG_ID_MEDIA_VID_PLAY_FINISH_IND ");
   msg_send_ext_queue(local_ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_play_finish_ind
*
* DESCRIPTION
*	This function is to send video play finish indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_play_finish_ind(kal_int16 result)
{
   media_vid_play_finish_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   
   ind_p = (media_vid_play_finish_ind_struct*)
            construct_local_para(sizeof(media_vid_play_finish_ind_struct), TD_CTRL);  

   ind_p->result = result;
   ind_p->seq_num = vid_context_p->seq_num;

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = vid_context_p->src_mod;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_PLAY_FINISH_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

  mmi_trace(1,"elisa _ vid_send_play_finish_ind   MSG_ID_MEDIA_VID_PLAY_FINISH_IND ");
   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_decode_event_ind
*
* DESCRIPTION
*	This function is to send video decode event indication to video module
*
* PARAMETERS
*	kal_uint8 event
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_decode_event_ind(kal_uint8 event)
{
   media_vid_decode_event_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   
   ind_p = (media_vid_decode_event_ind_struct*)
            construct_local_para(sizeof(media_vid_decode_event_ind_struct), TD_CTRL);  

   ind_p->event = event;

   ilm_ptr = allocate_ilm(MOD_VISUAL_HISR);
   ilm_ptr->src_mod_id = MOD_VISUAL_HISR;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_DECODE_EVENT_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_seek_done_ind
*
* DESCRIPTION
*	This function is to send seek done indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_seek_done_ind2(ilm_struct *ilm_ptr)
{
   media_vid_seek_done_ind_struct *ind_p,*source_p;
   ilm_struct       *local_ilm_ptr = NULL;
   
   ind_p = (media_vid_seek_done_ind_struct*)
            construct_local_para(sizeof(media_vid_seek_done_ind_struct), TD_CTRL);  

   source_p=(media_vid_seek_done_ind_struct*)ilm_ptr->local_para_ptr;

   ind_p->result = source_p->result;

   local_ilm_ptr = allocate_ilm(MOD_MED);
   local_ilm_ptr->src_mod_id = MOD_MED;
   local_ilm_ptr->dest_mod_id = MOD_MMI;
   ind_p->seq_num = source_p->seq_num;
   local_ilm_ptr->sap_id = MED_SAP;
   
   local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_SEEK_DONE_IND;
   local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   local_ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(local_ilm_ptr);

}


/*************************************************************************
* FUNCTION
*	vid_send_seek_done_ind
*
* DESCRIPTION
*	This function is to send seek done indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_seek_done_ind(kal_int16 result)
{
   media_vid_seek_done_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   
   ind_p = (media_vid_seek_done_ind_struct*)
            construct_local_para(sizeof(media_vid_seek_done_ind_struct), TD_CTRL);  

   ind_p->result = result;

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = vid_context_p->src_mod;
   ind_p->seq_num = vid_context_p->seq_num;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_SEEK_DONE_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_iframe_ready_ind
*
* DESCRIPTION
*	This function is to send i-frame ready indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_iframe_ready_ind(kal_int16 result)
{
   media_vid_iframe_ready_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   
   ind_p = (media_vid_iframe_ready_ind_struct*)
            construct_local_para(sizeof(media_vid_iframe_ready_ind_struct), TD_CTRL);  

   ind_p->result = result;

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = vid_context_p->src_mod;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_IFRAME_READY_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

#endif

#if defined( __MED_VID_MOD__) 


/*************************************************************************
* FUNCTION
*	vid_send_audio_play_finish_ind
*
* DESCRIPTION
*	This function is to send audio play finish indication to the request module
*
* PARAMETERS
*	kal_int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_audio_play_finish_ind(kal_int16 result)
{
   media_vid_audio_play_finish_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   
   ind_p = (media_vid_audio_play_finish_ind_struct*)
            construct_local_para(sizeof(media_vid_audio_play_finish_ind_struct), TD_CTRL);  

   ind_p->result = result;

   ilm_ptr = allocate_ilm(MOD_MMI);
   ilm_ptr->src_mod_id = MOD_MMI;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_AUDIO_PLAY_FINISH_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}




/* ============= Send message to Media-v ==============*/

/*************************************************************************
* FUNCTION
*	vid_send_visual_record_req
*
* DESCRIPTION
*	This function is to send visual record request to media-v task
*
* PARAMETERS
*	media_visual_record_req_struct* msg_p
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_visual_record_req(void* data)
{
   media_visual_record_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_visual_record_req_struct*)
            construct_local_para(sizeof(media_visual_record_req_struct), TD_CTRL);  
                            

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = MOD_MED_V;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VISUAL_RECORD_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_visual_stop_req
*
* DESCRIPTION
*	This function is to send visual stop request to media-v task
*
* PARAMETERS
*	media_visual_stop_req_struct* msg_p
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_visual_stop_req(void* data)
{
   media_visual_stop_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_visual_stop_req_struct*)
            construct_local_para(sizeof(media_visual_stop_req_struct), TD_CTRL);  
                            

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = MOD_MED_V;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VISUAL_STOP_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_visual_play_req
*
* DESCRIPTION
*	This function is to send visual play request to media-v task
*
* PARAMETERS
*	media_visual_play_req_struct* msg_p
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_visual_play_req(void* data)
{
   media_visual_play_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_visual_play_req_struct*)
            construct_local_para(sizeof(media_visual_play_req_struct), TD_CTRL);  
                            
   

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = MOD_MED_V;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VISUAL_PLAY_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_visual_seek_req
*
* DESCRIPTION
*	This function is to send visual seek request to media-v task
*
* PARAMETERS
*	kal_uint32 frame_num
*  kal_uint8 display
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_visual_seek_req(kal_uint32 frame_num, kal_uint8 display)
{
   media_visual_seek_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_visual_seek_req_struct*)
            construct_local_para(sizeof(media_visual_seek_req_struct), TD_CTRL);  
                            
   msg_p->frame_num = frame_num;
   msg_p->display = display;

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = MOD_MED_V;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VISUAL_SEEK_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*	vid_send_visual_file_merge_req
*
* DESCRIPTION
*	This function is to send visual file merge request to media-v task
*
* PARAMETERS
*	None
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_send_visual_file_merge_req(void)
{
   media_visual_file_merge_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_visual_file_merge_req_struct*)
            construct_local_para(sizeof(media_visual_file_merge_req_struct), TD_CTRL);  
                            
   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = MOD_MED_V;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VISUAL_FILE_MERGE_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


#endif /* __MED_VID_MOD__ */

#endif /* MED_NOT_PRESENT */

