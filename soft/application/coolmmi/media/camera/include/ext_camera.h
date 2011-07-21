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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    camera.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file contains the data structure for camera task
 *
 *
 * Author:
 * -------
 * -------
 *
  *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _EXT_CAMERA_H_
#define _EXT_CAMERA_H_

#if (defined(CAMERA_MODULE))
#include "kal_release.h"
#include "stack_timer.h"
#include "stack_config.h"
#include "lcd_if.h"
//#include "jpeg.h"

#include "di.h"

#define NULL_JPEG_FILE			0xFFFFFFFF
#define EXT_CAMERA_DIGITAL_ZOOM_INTERVAL	10

#define EXT_CAMERA_POWER_OFF_STATE		0
#define EXT_CAMERA_INITIAL_STATE			EXT_CAMERA_POWER_OFF_STATE+1
#define EXT_CAMERA_STANDBY_STATE			EXT_CAMERA_INITIAL_STATE+1
#define EXT_CAMERA_PREVIEW_STATE			EXT_CAMERA_STANDBY_STATE+1
#define EXT_CAMERA_CAPTURE_STATE			EXT_CAMERA_PREVIEW_STATE+1
#define EXT_CAMERA_VIDEO_RECORD_STATE  			EXT_CAMERA_CAPTURE_STATE+1


enum
{
	CAMERA_IMAGE_NORMAL=0,
	CAMERA_IMAGE_ROTATE_90,
	CAMERA_IMAGE_ROTATE_180,
	CAMERA_IMAGE_ROTATE_270,
	CAMERA_IMAGE_MIRROR,
	CAMERA_IMAGE_MIRROR_ROTATE_90,
	CAMERA_IMAGE_MIRROR_ROTATE_180,
	CAMERA_IMAGE_MIRROR_ROTATE_270
};

enum
{
	CLAM_SHELL_OPEN=0,
	CLAM_SHELL_CLOSE
};

typedef struct
{
	kal_uint32	burst_capture_file_size[20];
	kal_uint32	burst_capture_file_address[20];
}	ext_camera_burst_struct;

typedef struct
{
	kal_uint8	lcd_id;					/* which lcd will be updated (MAIN_LCD or SUB_LCD) */
	kal_uint16 	lcm_start_x;			/* the start x axis of LCM to be updated*/
	kal_uint16	lcm_start_y;			/* the start y axis of LCM to be updated*/
	kal_uint16 	lcm_end_x;				/* the end x axis of LCM to be updated*/
	kal_uint16 	lcm_end_y;				/* the end y axis of LCM to be updated*/
	kal_uint16 	dest_block_width;		/* x pitch of block write operation */
	kal_uint32  src_key_color;			/* transparent color */
	kal_uint32 	dest_buffer_address;	/* the start address of desination buffer for LCD memory write out */
	kal_uint32 	dest_buffer_size;		/* desination buffer size */
	kal_uint8	force_update;			/* force backend update LCD */
}	ext_camera_osd_para_struct;

typedef struct
{
	ext_camera_burst_struct	*burst_capture_ptr;
	kal_uint16	image_width;
	kal_uint16	image_height;
	kal_uint16  buffer_width;
	kal_uint16  buffer_height;
	kal_uint16  preview_offset_x;
	kal_uint16  preview_offset_y;
	kal_uint16	preview_width;
	kal_uint16	preview_height;
	kal_uint32	total_file_size;
	kal_uint32  src_key_color;	
	kal_uint8	captured_picture;
	kal_uint8   snapshot_number;
	kal_uint8	white_balance_mode1;
	kal_uint8	ev_value;
	kal_uint8	gamma_table;
	kal_uint8	zoom_factor;
	kal_uint8 	image_effect1;
	kal_uint8 	post_effect;			
	kal_uint8 	sticker_capture;		
	kal_uint8	lcd_id;					
	kal_uint8	frame_rate;
	kal_uint8	jpeg_compression_ratio;
	kal_uint8 	preview_rotate;
	kal_uint8 	banding_freq;
	kal_uint8	*jpeg_buffer_ptr;
	kal_uint16	*image_buffer_ptr;
	kal_uint32  image_buffer_size;
	kal_uint8	continue_capture;	
	kal_bool    video_mode;

	
} ext_camera_para_struct;


//chenhe add for jasperII +
typedef union
{
	struct _Camera_struct_
  {
  	kal_uint16 preview_width;	
  	kal_uint16 preview_height;
  	kal_uint8   frame_rate ; //use camera frame setting
  	kal_uint8  only1frameflag;
	}Camera_struct;
	
}CAM_MSG_STRUC;
//camera frame setting
#define  CAMERA_NO_BLEND_PREVIEW_FRAME_DEFAULT		1
#define  CAMERA_BLEND_PREVIEW_FRAME_DEFAULT		2	
#define  CAMERA_NO_BLEND_RECORD_FRAME_DEFAULT		3
#define  CAMERA_TSET_SLOW								4
//chenhe add for jasperII -

typedef struct
{
   kal_uint16 *frame_buf_addr;
   kal_uint32 length;
   kal_uint32 read_length;
   kal_uint16 *table_addr;
   kal_uint32 read_count;
   kal_uint8  video_encode_quality;
   kal_uint8  frame_rate;
} ext_camera_retrieve_video_struct;

typedef struct
{
#ifdef CAMERA_MODULE_WITH_LCD
   void (* cam_module_bypass)(void);	
#endif   
   void (* cam_module_power_on)(void);
	void (* cam_module_cmd_mapping)(void);
   kal_uint8 (* cam_module_reset_status)(void);
   kal_uint8 (* cam_module_preview_ready_check)(void);
   kal_uint32 (* cam_module_capture_ready_check)(void);
   void (* cam_module_power_off)(void);
   void (* cam_module_preview)(ext_camera_para_struct *ext_cam_para);
   void (* exit_cam_module_preview)(void);
   void (* cam_module_capture)(ext_camera_para_struct *ext_cam_para);
   kal_uint32 (* cam_module_jpeg_encode) (ext_camera_para_struct *ext_cam_para, kal_bool back_to_preview);
   void (* cam_module_set_para)(kal_uint8 cmd, kal_uint8 para);
   void (* cam_module_write_para)(kal_uint16 cmd, kal_uint16 para);
   kal_uint16 (* cam_module_read_para)(kal_uint16 cmd);
   kal_uint8 (* cam_module_frame_rate)(void);
#ifdef CAMERA_MODULE_WITH_LCD
   void (* cam_module_write_lcdfb)(ext_camera_osd_para_struct *ext_cam_lcd_para);
   void (* cam_module_start_video_encode)(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para);
   void (* cam_module_stop_video_encode)(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para);
   void (* cam_module_pause_video_encode)(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para);
   void (* cam_module_resume_video_encode)(void);
   void (* cam_module_retrieve_video_data)(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para);   
#endif
} ext_cam_module_func;

extern ext_cam_module_func *ext_cam_func;
extern ext_camera_para_struct ext_camera_config_data;
extern kal_bool video_clip_mode;

extern kal_uint8 cam_command_map[];
extern kal_uint8 cam_ev_param_map[];
extern kal_uint8 cam_zoom_param_map[];
extern kal_uint8 cam_effect_param_map[];
extern kal_uint8 cam_jpg_qty_param_map[];
extern kal_uint8 cam_wb_param_map[];
extern kal_uint8 cam_banding_param_map[];

void ext_camera_init(void);
void ext_camera_power_on(void);
kal_uint8 ext_camera_reset_status(void);
kal_uint32 ext_camera_ready_check(void);
void ext_camera_power_off(void);
void ext_camera_preview(ext_camera_para_struct *ext_cam_para, lcd_frame_update_struct *lcd_para);
kal_uint32 ext_camera_sticker_capture(ext_camera_para_struct *ext_cam_para, lcd_frame_update_to_mem_struct *lcd_para);
void exit_ext_camera_preview(void);
void ext_camera_capture(ext_camera_para_struct *ext_cam_para);
kal_uint32 ext_camera_video_clip(ext_camera_para_struct *ext_cam_para);
void ext_camera_set_parameter(kal_uint8 cmd, kal_uint8 para);
kal_bool write_ext_camera(kal_uint16 cmd, kal_uint16 para);
kal_bool read_ext_camera(kal_uint16 cmd, kal_uint16 *para);
kal_uint8 ext_camera_video_frame_rate(void);
kal_uint32 ext_camera_video_clip(ext_camera_para_struct *ext_cam_para);//add by WeiD

void ext_camera_set_osd_buffer(kal_uint32 osd_addr, kal_uint32 osd_size);
void ext_camera_update_backend_lcd_buffer(lcd_frame_update_struct *ext_camera_osd_data);
void ext_camera_start_video_encode(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para);
void ext_camera_retrieve_video_data(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para);
void ext_camera_pause_video_encode(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para);
void ext_camera_resume_video_encode(void);
void ext_camera_stop_video_encode(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_paras);

#endif  
#endif /* _EXT_CAMERA_H_ */
