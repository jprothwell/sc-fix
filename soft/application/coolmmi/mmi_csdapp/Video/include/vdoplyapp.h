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
 *	  VdoPlyApp.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Expose function and variable.
 *
 * Author:
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------

 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _VDOPLYAPP_H
#define _VDOPLYAPP_H

#include "mmi_features.h"
#ifdef __MMI_VIDEO__

/***************************************************************************** 
* Define
*****************************************************************************/
#define VDOPLY_PLAYBACK_ROTATE_0 				MDI_VIDEO_LCD_ROTATE_0
#define VDOPLY_PLAYBACK_ROTATE_90				MDI_VIDEO_LCD_ROTATE_90
#define VDOPLY_PLAYBACK_ROTATE_180 				MDI_VIDEO_LCD_ROTATE_180
#define VDOPLY_PLAYBACK_ROTATE_270				MDI_VIDEO_LCD_ROTATE_270
#define VDOPLY_PLAYBACK_ROTATE_0_MIRROR 		MDI_VIDEO_LCD_ROTATE_0_MIRROR
#define VDOPLY_PLAYBACK_ROTATE_90_MIRROR 		MDI_VIDEO_LCD_ROTATE_90_MIRROR
#define VDOPLY_PLAYBACK_ROTATE_180_MIRROR		MDI_VIDEO_LCD_ROTATE_180_MIRROR
#define VDOPLY_PLAYBACK_ROTATE_270_MIRROR		MDI_VIDEO_LCD_ROTATE_270_MIRROR


/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum
{
    VDOPLY_STATE_EXIT,
    VDOPLY_STATE_OPENING,
    VDOPLY_STATE_IDLE,
    VDOPLY_STATE_PAUSE,
    VDOPLY_STATE_PLAY,
    VDOPLY_STATE_BT_CONNECTING,    
    VDOPLY_STATE_SEEKING,
    VDOPLY_STATE_PLAY_SEEKING,
    VDOPLY_STATE_INTERRUPTED_OPENING,
    VDOPLY_STATE_INTERRUPTED_PAUSE,
    VDOPLY_STATE_INTERRUPTED_SEEKING, /* 10 */
    VDOPLY_STATE_BACKGROUND_CALL,
    
    VDOPLY_STATE_FULLSCR_IDLE,  
    VDOPLY_STATE_FULLSCR_PAUSE,
    VDOPLY_STATE_FULLSCR_PLAY,
    VDOPLY_STATE_FULLSCR_BT_CONNECTING,        
    VDOPLY_STATE_FULLSCR_SEEKING,
    VDOPLY_STATE_FULLSCR_PLAY_SEEKING,
        
    VDOPLY_STATE_STREAM_DISCONNECT, /* 18 */
    VDOPLY_STATE_STREAM_CONNECTING,
    VDOPLY_STATE_STREAM_INTERRUPTED_CONNECTING,
    VDOPLY_STATE_STREAM_CONNECTED,  
    VDOPLY_STATE_STREAM_BUFFERING,    
    VDOPLY_STATE_STREAM_PLAY,    
    VDOPLY_STATE_STREAM_BT_CONNECTING,
    VDOPLY_STATE_STREAM_BG_CALL_CONNECTED,
    VDOPLY_STATE_STREAM_BG_CALL_DISCONNECT,

    VDOPLY_STATE_STREAM_FULLSCR_DISCONNECT,  /* 27 */
    VDOPLY_STATE_STREAM_FULLSCR_CONNECTING,  
    VDOPLY_STATE_STREAM_FULLSCR_INTERRUPTED_CONNECTING,      
    VDOPLY_STATE_STREAM_FULLSCR_CONNECTED,    
    VDOPLY_STATE_STREAM_FULLSCR_BUFFERING,    
    VDOPLY_STATE_STREAM_FULLSCR_PLAY,   
    VDOPLY_STATE_STREAM_FULLSCR_BT_CONNECTING,

    VDOPLY_STATE_PROGRESSIVE_OPENING,   /* 34 */
    VDOPLY_STATE_PROGRESSIVE_INTERRUPTED_OPENING, 
    VDOPLY_STATE_PROGRESSIVE_BUFFERING,    
    VDOPLY_STATE_PROGRESSIVE_PLAY_BUFFERING,
    VDOPLY_STATE_PROGRESSIVE_FULLSCR_BUFFERING,        
    VDOPLY_STATE_PROGRESSIVE_FULLSCR_PLAY_BUFFERING,


    /* state only for drawing */
    VDOPLY_STATE_SNAPSHOT,
    VDOPLY_STATE_FULLSCR_SNAPSHOT

    
} vdoply_state_neum;




typedef struct {
	S32 	offset_y;
	S32 	hr_0_offset_x;
	S32 	hr_1_offset_x;
	S32 	col_0_offset_x;
	S32 	min_0_offset_x;
	S32 	min_1_offset_x;
	S32 	col_1_offset_x;
	S32 	sec_0_offset_x;
	S32 	sec_1_offset_x;
	U16	prev_hr;
	U16	prev_min;	
	U16	prev_sec;		
	
} vdoply_timer_struct;


typedef struct {

	S32	fill_width;
	S32	fill_height;	

} vdoply_progress_struct;


typedef struct {
	U16	width;
	U16	height;
	U32   total_time_duration;
	U16 startx;
	U16 starty;
	U16 rotate;
	U16 media_type;
	U8 aud_channel_no;
       U16 aud_sample_rate;
	mdi_video_track_enum track;
} vdoply_video_info_struct;


typedef struct {
	U32 	play_time;
	U32	start_play_time;		/* start play time - this is used to fix driver seek not accurate problem*/
	U32 	play_ratio;	
	U8		speed;
	U8		volume;
} vdoply_status_struct;


typedef struct {
	S32	offset_x;
	S32	offset_y;
	S32	width;
	S32	height;
	BOOL	is_press;
} vdoply_ui_touch_struct;


typedef enum{
	VDOPLY_TOUCH_SPEED_INC,
	VDOPLY_TOUCH_SPEED_DEC,
	VDOPLY_TOUCH_VOLUME_INC,
	VDOPLY_TOUCH_VOLUME_DEC,
	VDOPLY_TOUCH_PROGRESS_INC,
	VDOPLY_TOUCH_PROGRESS_DEC,
	VDOPLY_TOUCH_PROGRESS_BAR,
	VDOPLY_TOUCH_PLAY,
	VDOPLY_TOUCH_STOP,
	VDOPLY_TOUCH_SNAPSHOT,
	VDOPLY_TOUCH_FULLSCREEN,
	VDOPLY_TOUCH_RSK,
	VDOPLY_TOUCH_LSK,	
	VDOPLY_TOUCH_NONE
} vdoply_touch_object_enum;

/* context */
typedef struct {	
	
	gdi_handle						base_layer_handle;
	gdi_handle						play_wnd_layer_handle;
	gdi_handle						osd_layer_handle;

	PU8								osd_layer_buf_ptr;

	/* misc */
	U16								state;
	U16								play_source;
	U16								video_id;
	U16								video_name_id;
	PS8								filename;	
	PS8								lsk_str_ptr;
	PS8								rsk_str_ptr;
	U16								lsk_img_id;
	U16								rsk_img_id;	
	
	BOOL								is_lsk_pressed;
	BOOL								is_rsk_pressed;	
	BOOL								is_seek_fwd_key_pressed;
	BOOL								is_seek_bwd_key_pressed;
	BOOL								is_speed_inc_key_pressed;
	BOOL								is_speed_dec_key_pressed;	
	BOOL								is_vol_inc_key_pressed;
	BOOL								is_vol_dec_key_pressed;		
	BOOL								is_seeking;
	BOOL								is_sub_display;
	BOOL								is_short_filename;			

	U16								fullscr_osd_opacity;
	U16								normal_style_rotate;
	S32								normal_style_lcd_width;
	S32								normal_style_lcd_height;
	
	/* display */
	vdoply_timer_struct			timer;
	vdoply_progress_struct		progress;

	vdoply_timer_struct			fullscr_timer;
	vdoply_progress_struct		fullscr_progress;

	/* video clip */
	vdoply_video_info_struct	video_info;
	vdoply_status_struct			cur_status;

	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		U16								touch_object;
		
		vdoply_ui_touch_struct		touch_speed_inc;
		vdoply_ui_touch_struct		touch_speed_dec;
		vdoply_ui_touch_struct		touch_volume_inc;
		vdoply_ui_touch_struct		touch_volume_dec;	
		vdoply_ui_touch_struct		touch_progress_inc;
		vdoply_ui_touch_struct		touch_progress_dec;
		vdoply_ui_touch_struct		touch_progress_bar;
		vdoply_ui_touch_struct		touch_snapshot;
		vdoply_ui_touch_struct		touch_fullscreen;
		vdoply_ui_touch_struct		touch_play;
		vdoply_ui_touch_struct		touch_stop;
		vdoply_ui_touch_struct		touch_rsk;		
		vdoply_ui_touch_struct		touch_lsk;				

		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			vdoply_ui_touch_struct		full_touch_speed_inc;
			vdoply_ui_touch_struct		full_touch_speed_dec;
			vdoply_ui_touch_struct		full_touch_volume_inc;
			vdoply_ui_touch_struct		full_touch_volume_dec;	
			vdoply_ui_touch_struct		full_touch_progress_inc;
			vdoply_ui_touch_struct		full_touch_progress_dec;
			vdoply_ui_touch_struct		full_touch_progress_bar;
			vdoply_ui_touch_struct		full_touch_play;
			vdoply_ui_touch_struct		full_touch_stop;
			vdoply_ui_touch_struct		full_touch_rsk;		
			vdoply_ui_touch_struct		full_touch_lsk;
	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
	
	#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

	
} vdoply_context_struct;




/****************************************************************************
* Skin                                                            
*****************************************************************************/
typedef struct {
	/* image size */
	U8		digit_image_width;
	U8		digit_image_height;
	U8		colon_image_width;
	U8		colon_image_height;
	U8		spacing;

	/* offset */
	S16	digit_start_offset_x;
	S16	digit_start_offset_y;

	/* back ground */
	U8		bg_r;
	U8		bg_g;
	U8		bg_b;

	/* timer */
	S16	offset_x;
	S16	offset_y;
	U16	width;
	U16	height;

	/* flag */
	BOOL	draw_bg_image;
	BOOL	draw_bg_color;
	BOOL	draw_hour;
	BOOL	draw_min;
} multimedia_timer_struct;


typedef struct {
	S16	offset_x;
	S16	offset_y;
	U16	width;
	U16	height;
} multimedia_display_wnd_struct;

typedef struct {
	S16	offset_x;
	S16	offset_y;
	U16	width;
	U16	height;
} multimedia_slidebar_struct;



#endif /* __MMI_VIDEO__ */
#endif /* _VDOPLYAPP_H */ 


