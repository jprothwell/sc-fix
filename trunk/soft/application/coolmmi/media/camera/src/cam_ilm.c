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
 *   cam_ilm.c
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
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
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
#include "app_ltlcom.h"          /* task message communiction */
#include "stacklib.h"           /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"          /* event scheduler */
#include "stack_timer.h"         /* stack timer */

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
#include "med_api.h"

#include "mmi_trace.h"

HANDLE GetMmiTaskHandle(module_type mod);

#ifdef __MED_CAM_MOD__

/*************************************************************************
* FUNCTION
*   cam_send_power_up_req
*
* DESCRIPTION
*   This function is to send camera power up request message. 
*
* PARAMETERS
*   module_type src_mod_id
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_send_power_up_req(module_type src_mod_id, kal_uint16  seq_num)
{
   mmi_trace(g_sw_CAMERA,"Excute funs: cam_send_power_up_req() is TRUE");
   media_cam_power_up_req_struct* msg_p = NULL;
   ilm_struct *ilm_ptr = NULL;

   msg_p = (media_cam_power_up_req_struct*)
            construct_local_para(sizeof(media_cam_power_up_req_struct), TD_CTRL);  
   msg_p->seq_num=seq_num;
                             
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_POWER_UP_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*   cam_send_power_down_req
*
* DESCRIPTION
*   This function is to send camera power down request message. 
*
* PARAMETERS
*   module_type src_mod_id
*  kal_int16 delay_time
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_send_power_down_req(module_type src_mod_id, kal_int16 delay_time)
{
   media_cam_power_down_req_struct* msg_p = NULL;
   ilm_struct *ilm_ptr = NULL;

   msg_p = (media_cam_power_down_req_struct*)
            construct_local_para(sizeof(media_cam_power_down_req_struct), TD_CTRL);  
                            
   msg_p->delay_time = delay_time;
   
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_POWER_DOWN_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*   cam_send_preview_req
*
* DESCRIPTION
*   This function is to send camera preview request message. 
*
* PARAMETERS
*   module_type src_mod_id
*  void* data
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_send_preview_req(module_type src_mod_id, void* data)
{
   media_cam_preview_req_struct* msg_p = NULL;
   ilm_struct *ilm_ptr = NULL;
   cam_preview_struct *preview=(cam_preview_struct*)data;

   mmi_trace(g_sw_CAMERA, "Excute func : cam_send_preview_req()");

   msg_p = (media_cam_preview_req_struct*)
            construct_local_para(sizeof(media_cam_preview_req_struct), TD_CTRL);  
                            
   msg_p->image_width = preview->image_width;
   msg_p->image_height = preview->image_height ;
                     
   msg_p->buffer_width = preview->buffer_width;
   msg_p->buffer_height = preview->buffer_height;
   msg_p->preview_width = preview->preview_width;
   msg_p->preview_height = preview->preview_height;
   msg_p->preview_offset_x = preview->preview_offset_x;
   msg_p->preview_offset_y = preview->preview_offset_y;
   msg_p->image_buffer_p = preview->image_buffer_p;
   msg_p->image_buffer_size = preview->image_buffer_size;
   msg_p->frame_rate = preview->frame_rate;
   msg_p->contrast = preview->contrast;
   msg_p->brightness = preview->brightness;
   msg_p->saturation = preview->saturation;
   msg_p->hue = preview->hue;
   msg_p->gamma = preview->gamma;
   msg_p->WB = preview->WB;
   msg_p->exposure = preview->exposure;
   msg_p->effect = preview->effect;
   msg_p->zoom_factor = preview->zoom_factor;
   msg_p->rotate = preview->rotate;
   msg_p->lcd_rotate = preview->lcd_rotate;
   msg_p->flash_mode = preview->flash_mode;
   msg_p->night_mode = preview->night_mode;
   msg_p->banding_freq= preview->banding_freq;
   msg_p->snapshot_number= preview->snapshot_number;
   msg_p->src_key_color= preview->src_key_color;

   msg_p->lcd_id = preview->lcd_id;
   msg_p->lcd_start_x = preview->lcd_start_x;
   msg_p->lcd_start_y = preview->lcd_start_y;
   msg_p->lcd_end_x = preview->lcd_end_x;
   msg_p->lcd_end_y = preview->lcd_end_y;
   msg_p->roi_offset_x = preview->roi_offset_x;
   msg_p->roi_offset_y = preview->roi_offset_y;
   msg_p->update_layer = preview->update_layer;
   msg_p->hw_update_layer = preview->hw_update_layer;
   /* LCD Setting */
   msg_p->roi_background_color = preview->roi_background_color;    

#if (defined(MT6228)||defined(MT6229))
   /* ISP Setting */
   msg_p->manual_wb = preview->manual_wb;    
   msg_p->ae_metering_mode = preview->ae_metering_mode;    
   msg_p->iso_setting = preview->iso_setting;    
   msg_p->scene_mode = preview->scene_mode;    
   /* IPP Setting */
   msg_p->image_pitch_mode = preview->image_pitch_mode;    
   msg_p->image_data_format = preview->image_data_format;    
   msg_p->image_pitch_bytes = preview->image_pitch_bytes;    

   msg_p->overlay_frame_mode = preview->overlay_frame_mode;    
   msg_p->overlay_color_depth = preview->overlay_color_depth;    
   msg_p->overlay_frame_source_key = preview->overlay_frame_source_key;    
   msg_p->overlay_frame_width = preview->overlay_frame_width;    
   msg_p->overlay_frame_height = preview->overlay_frame_height;    
   msg_p->overlay_frame_buffer_address = preview->overlay_frame_buffer_address;    
   /* Display Setting */
   msg_p->tv_output_mode = preview->tv_output_mode;    
   msg_p->tv_output = preview->tv_output;    
   msg_p->tv_output_width = preview->tv_output_width;    
   msg_p->tv_output_height = preview->tv_output_height;    
   msg_p->tv_output_buffer1_address = preview->tv_output_buffer1_address;    
   msg_p->tv_output_buffer2_address = preview->tv_output_buffer2_address;    
   msg_p->tv_output_buffer_size = preview->tv_output_buffer_size;    
#endif

mmi_trace(g_sw_CAMERA, "cam_send_preview_req width = %d, height = %d, x = %d, y = %d",preview->preview_width,preview->preview_height,preview->preview_offset_x,preview->preview_offset_y);
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_PREVIEW_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

	mmi_trace(g_sw_CAMERA, "MMI >> MED MSG_ID_MEDIA_CAM_PREVIEW_REQ");
   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*   cam_send_stop_req
*
* DESCRIPTION
*   This function is to send camera stop request message. 
*
* PARAMETERS
*   module_type src_mod_id
*  
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_send_stop_req(module_type src_mod_id)
{
   media_cam_stop_req_struct* msg_p = NULL;
   ilm_struct *ilm_ptr = NULL;
   msg_p = (media_cam_stop_req_struct*)
            construct_local_para(sizeof(media_cam_stop_req_struct), TD_CTRL);  

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_STOP_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*   cam_send_capture_req
*
* DESCRIPTION
*   This function is to send camera capture request message. 
*
* PARAMETERS
*   module_type src_mod_id
*  void* data
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_send_capture_req(module_type src_mod_id, void* data)
{
   media_cam_capture_req_struct* msg_p = NULL;
   ilm_struct *ilm_ptr = NULL;
   cam_capture_struct *capture=(cam_capture_struct*)data;

   msg_p = (media_cam_capture_req_struct*)
            construct_local_para(sizeof(media_cam_capture_req_struct), TD_CTRL);  
   mmi_trace(g_sw_CAMERA, "Excute funs: cam_send_capture_req media_mode = %d",capture->media_mode);
   msg_p->media_mode = capture->media_mode;
   msg_p->source_device = capture->source_device;
   msg_p->image_width = capture->image_width;
   msg_p->image_height = capture->image_height;
   msg_p->data = capture->data;
   msg_p->capture_buffer_p = capture->capture_buffer_p;
   msg_p->file_buffer_len = capture->file_buffer_len;
   msg_p->file_size_p = capture->file_size_p;
   msg_p->image_quality = capture->image_quality;
   msg_p->gray_mode = capture->gray_mode;
   msg_p->flash_mode = capture->flash_mode;
   msg_p->snapshot_number= capture->snapshot_number;
   msg_p->capture_layer = capture->capture_layer;
   msg_p->roi_offset_x = capture->roi_offset_x;
   msg_p->roi_offset_y = capture->roi_offset_y;
   msg_p->jpegs_p= (media_jpegs_struct*)capture->jpegs_p;
   msg_p->continue_capture = capture->continue_capture;
#if MT6228_SERIES
   /* ISP Setting */
   msg_p->flash_mode = capture->flash_mode;    
   
   /* JPEG Setting */
   msg_p->jpeg_yuv_mode = capture->jpeg_yuv_mode;    
   msg_p->thumbnail_mode = capture->thumbnail_mode;    
   msg_p->thumbnail_width = capture->thumbnail_width;    
   msg_p->thumbnail_height = capture->thumbnail_height;    
   /* IPP Setting */
   msg_p->overlay_frame_mode = capture->overlay_frame_mode;    
   msg_p->overlay_color_depth = capture->overlay_color_depth;    
   msg_p->overlay_frame_source_key = capture->overlay_frame_source_key;    
   msg_p->overlay_frame_width = capture->overlay_frame_width;    
   msg_p->overlay_frame_height = capture->overlay_frame_height;    
   msg_p->overlay_frame_buffer_address = capture->overlay_frame_buffer_address;    
   msg_p->image_pitch_mode = capture->image_pitch_mode;    
   msg_p->image_data_format = capture->image_data_format;    
   msg_p->image_pitch_bytes = capture->image_pitch_bytes;    
   /* Display Setting */
   msg_p->lcd_id = capture->lcd_id;    
   msg_p->lcm_start_x = capture->lcm_start_x;    
   msg_p->lcm_start_y = capture->lcm_start_y;    
   msg_p->lcm_end_x = capture->lcm_end_x;    
   msg_p->lcm_end_y = capture->lcm_end_y;    
//   msg_p->roi_offset_x = capture->roi_offset_x;    
//   msg_p->roi_offset_y = capture->roi_offset_y;    
   msg_p->update_layer = capture->update_layer;    
   msg_p->hw_update_layer = capture->hw_update_layer;    
   msg_p->display = capture->display;    
   msg_p->frame_buffer_address = capture->frame_buffer_address;    
   msg_p->frame_buffer_size = capture->frame_buffer_size;    
   
   msg_p->tv_output = capture->tv_output;    
   msg_p->tv_output_buffer1_address = capture->tv_output_buffer1_address;    
   msg_p->tv_output_buffer2_address = capture->tv_output_buffer2_address;    
   msg_p->tv_output_buffer_size = capture->tv_output_buffer_size;    
#endif
                              
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_CAPTURE_REQ;
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
void cam_send_set_param_req(module_type src_mod_id, kal_uint16 param_id, kal_int16 value)
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
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_SET_PARAM_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*   cam_send_set_flash_req
*
* DESCRIPTION
*   This function is to send set flash request message. 
*
* PARAMETERS
*   module_type src_mod_id
*  kal_int16 flash_mode
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_send_set_flash_req(module_type src_mod_id, kal_int16 flash_mode)
{
   media_cam_set_flash_req_struct* msg_p = NULL;
   ilm_struct *ilm_ptr = NULL;
   msg_p = (media_cam_set_flash_req_struct*)
            construct_local_para(sizeof(media_cam_set_flash_req_struct), TD_CTRL);  

   msg_p->flash_mode = flash_mode;

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_SET_FLASH_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

void cam_send_cam_save_photo_req(module_type src_mod_id,UINT32 filehandle)
{
   mmi_trace(g_sw_CAMERA,"Excute funs: cam_send_cam_save_photo_req() is TRUE");
   ilm_struct *ilm_ptr = NULL;

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_SAVE_PHOTO;
   ilm_ptr->local_para_ptr = (local_para_struct*)filehandle;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}
/*************************************************************************
* FUNCTION
*   cam_send_ready_ind
*
* DESCRIPTION
*   This function is to send camera ready indication message. 
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
void cam_send_ready_ind(kal_int16 result)
{
   media_cam_ready_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   

    
   ind_p = (media_cam_ready_ind_struct*)
            construct_local_para(sizeof(media_cam_ready_ind_struct), TD_CTRL);  

   ind_p->result = result;
   ind_p->seq_num = cam_context_p->seq_num;
   
   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = cam_context_p->src_mod;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_READY_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*   cam_send_preview_fail_ind
*
* DESCRIPTION
*   This function is to send camera preview fail indication message. 
*
* PARAMETERS
*   kal_int16 result
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_send_preview_fail_ind(kal_int16 result)
{
   media_cam_preview_fail_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   

    
   ind_p = (media_cam_preview_fail_ind_struct*)
            construct_local_para(sizeof(media_cam_preview_fail_ind_struct), TD_CTRL);  

   ind_p->result = result;
   
   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = cam_context_p->src_mod;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_PREVIEW_FAIL_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*   cam_send_event_ind
*
* DESCRIPTION
*   This function is to send camera event indication message. 
*
* PARAMETERS
*   kal_uint8 event
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_send_event_ind(kal_uint8 event, kal_uint8 cause)
{
   media_cam_event_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;

    
   ind_p = (media_cam_event_ind_struct*)
            construct_local_para(sizeof(media_cam_event_ind_struct), TD_CTRL);  

   ind_p->event = event;
   ind_p->cause = cause;
   
   ilm_ptr = allocate_ilm(MOD_VISUAL_HISR);
   ilm_ptr->src_mod_id = MOD_VISUAL_HISR;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_EVENT_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

/*************************************************************************
* FUNCTION
*   cam_send_capture_event_ind
*
* DESCRIPTION
*   This function is to send camera capture event indication message. 
*
* PARAMETERS
*   kal_uint8 event
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_send_capture_event_ind(kal_int16 result, kal_uint16 count)
{
   media_cam_capture_event_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;

    
   ind_p = (media_cam_capture_event_ind_struct*)
            construct_local_para(sizeof(media_cam_capture_event_ind_struct), TD_CTRL);  

   ind_p->result = result;
   ind_p->count = count;
   
   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = cam_context_p->src_mod;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_CAPTURE_EVENT_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

//send camera motioen detect open  indication message
void cam_send_md_open_req(module_type src_mod_id, kal_uint32 sensLevel, void(*md_callback)(int32))
{
   mmi_trace(g_sw_CAMERA,"Excute funs: cam_send_md_open_req() is TRUE");
   media_cam_md_open_req_struct* msg_p = NULL;
   ilm_struct *ilm_ptr = NULL;

   msg_p = (media_cam_md_open_req_struct*)
            construct_local_para(sizeof(media_cam_md_open_req_struct), TD_CTRL);  
   msg_p->sensLevel=sensLevel;
   msg_p->md_callback=md_callback;
                             
   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_MD_OPEN_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

//send camera motioen detect close  indication message
void cam_send_md_close_req(module_type src_mod_id)
{
   mmi_trace(g_sw_CAMERA,"Excute funs: cam_send_md_close_req() is TRUE");
   ilm_struct *ilm_ptr = NULL;

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_MD_CLOSE_REQ;
   ilm_ptr->local_para_ptr = NULL;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

void cam_send_md_work_ind(int32 result)
{
   media_cam_md_work_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   
   mmi_trace(g_sw_CAMERA,"Excute funs: cam_send_md_work_ind");
    
   ind_p = (media_cam_md_work_ind_struct*)
            construct_local_para(sizeof(media_cam_md_work_ind_struct), TD_CTRL);  

   ind_p->result = result;
   
   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = MOD_MMI;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_CAM_MD_WORK_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}

#endif /* __MED_CAM_MOD__ */

#endif /* MED_NOT_PRESENT */

