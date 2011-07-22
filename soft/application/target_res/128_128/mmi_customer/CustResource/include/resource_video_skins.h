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
 *	 resource_video_skins.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Video App customization file.
 *
 * Author:
 * -------
 *	 Bear (CSD00612)
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log$
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _VIDEO_SKIN_H_
#define _VIDEO_SKIN_H_

#include "mmi_features.h"

#include "cswtype.h" /* wufasong added 2007.06.16 */
#ifdef __MMI_VIDEO__


/***************************************************************************** 
* Define
*****************************************************************************/
typedef enum {
	VIDEO_TEXT_STYLE_NORMAL,
	VIDEO_TEXT_STYLE_BORDER,
	VIDEO_TEXT_STYLE_SHADOW	
} mmi_video_text_style;

typedef enum {
	VIDEO_ALIGN_LEFT,
	VIDEO_ALIGN_RIGHT,
	VIDEO_ALIGN_TOP,
	VIDEO_ALIGN_BOTTOM,	
	VIDEO_ALIGN_CENTER
} mmi_video_align_style;
	

/***************************************************************************** 
* Struct
*****************************************************************************/
typedef struct
{
	BOOL 	is_show;
	S32	offset_x;
	S32	offset_y;
} video_icon_struct;

typedef struct
{
	S32 	offset_x;
	S32	offset_y;
	S32	width;
	S32 	height;
} video_rect_struct;

typedef struct
{
	S32	offset_x;
	S32 	offset_y;
} video_pos_struct;


typedef struct {
	U8		r;
	U8		g;	
	U8		b;		
	U8		style_r;
	U8		style_g;	
	U8		style_b;
	U16	style;	
} video_style_text_struct;


typedef struct {	
	BOOL 	is_show;
	S32 	offset_x;
	S32	offset_y;
	video_style_text_struct style_text;
} video_osd_storage_struct;


typedef struct {

	U16	h_align;
	U16	v_align;
	BOOL	is_draw_app_name;
	BOOL	is_draw_filename;
	video_rect_struct 		rect;	
	video_style_text_struct	style_text;
} video_osd_title_struct;


typedef struct {
	video_rect_struct rect;
} video_osd_play_wnd_struct;


typedef struct {
	video_pos_struct 	bg;
	video_pos_struct	fill;
	video_pos_struct	empty;
} video_osd_progress_struct;

typedef struct {
	video_pos_struct				speed;
	video_pos_struct				volume;
	video_pos_struct				timer;
	video_pos_struct				state;	
	video_osd_progress_struct	progress;
} video_osd_vodply_panel_struct;

typedef struct {
	video_rect_struct				rect;	
	video_pos_struct				speed;
	video_pos_struct				volume;
	video_pos_struct				timer;
	video_pos_struct				state;	
	video_osd_progress_struct	progress;
} video_osd_vodply_full_panel_struct;


typedef struct {
	video_pos_struct				ev;
	video_pos_struct				zoom;
	video_pos_struct				timer;
	video_pos_struct				state;
	video_pos_struct				status;
} video_osd_vodrec_panel_struct;


typedef struct {
	BOOL							is_icon_softkey;
	video_pos_struct			icon_lsk_pos;
	video_pos_struct			icon_rsk_pos;		
	video_rect_struct			rect;
 	video_style_text_struct	lsk_text;
	video_style_text_struct	rsk_text;
} video_osd_softkey_struct;

typedef struct {
	BOOL							is_icon_softkey;
	video_pos_struct			icon_lsk_pos;
	video_pos_struct			icon_rsk_pos;		
 	video_style_text_struct	lsk_text;
	video_style_text_struct	rsk_text;
} video_osd_full_softkey_struct;


typedef struct {
	BOOL							is_show;
	video_rect_struct			rect;
 	video_style_text_struct	style_text;
} video_osd_hint_struct;

typedef struct {
	BOOL						is_show;
	video_rect_struct 	rect;
	video_icon_struct		night;
	video_icon_struct		led_highlight;
	video_icon_struct		record_aud;	
	video_icon_struct		size_limit;
	video_icon_struct 	time_limit;
} video_osd_vdorec_status_struct;


	

typedef struct {
	/************************************/
	/* 			  BASIC SKIN 				*/
	/************************************/
	video_osd_title_struct				title;
	video_osd_play_wnd_struct			preview_wnd;
	video_osd_vodrec_panel_struct		panel;
	video_osd_vdorec_status_struct	status;	
	video_osd_softkey_struct 			softkey;
	video_osd_hint_struct				hint;

 	#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
		/************************************/
		/* 		  TOUCHSCREEN SKIN			*/
		/************************************/	 	
		video_pos_struct		touch_ev_inc;
		video_pos_struct		touch_ev_dec;
		video_pos_struct		touch_zoom_inc;
		video_pos_struct		touch_zoom_dec;		
		video_pos_struct		touch_rec;
		video_pos_struct		touch_stop;		
	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */

} vdorec_osd_vdorec_layout_struct;


typedef struct {
	/************************************/
	/* 			  BASIC SKIN 				*/
	/************************************/		
	video_osd_title_struct				title;
	video_osd_play_wnd_struct			play_wnd;
	video_osd_vodply_panel_struct		panel;
	video_osd_softkey_struct 			softkey;

	/************************************/
	/* 		  TOUCHSCREEN SKIN			*/
	/************************************/	
	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		video_pos_struct		touch_speed_inc;
		video_pos_struct		touch_speed_dec;	
		video_pos_struct		touch_volume_inc;
		video_pos_struct		touch_volume_dec;
		video_pos_struct		touch_progress_inc;
		video_pos_struct		touch_progress_dec;
		video_pos_struct		touch_snapshot;
		video_pos_struct		touch_fullscreen;
		video_pos_struct		touch_play;	
		video_pos_struct		touch_stop;		
	#endif /* 	__VDOPLY_FEATURE_TOUCH_SCREEN__ */


	#ifdef __VDOPLY_FEATURE_FULLSCREEN__
		/************************************/
		/* 			FULLSCREEN SKIN			*/
		/************************************/	
		video_osd_full_softkey_struct			full_softkey;
		video_osd_vodply_full_panel_struct	full_panel;	
				
		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
			/************************************/
			/*   TOUCHSCREEN FULLSCREEN SKIN		*/
			/************************************/	
			video_pos_struct		full_touch_speed_inc;
			video_pos_struct		full_touch_speed_dec;	
			video_pos_struct		full_touch_volume_inc;
			video_pos_struct		full_touch_volume_dec;
			video_pos_struct		full_touch_progress_inc;
			video_pos_struct		full_touch_progress_dec;
			video_pos_struct		full_touch_play;	
			video_pos_struct		full_touch_stop;		
		#endif /* 	__VDOPLY_FEATURE_TOUCH_SCREEN__ */

	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

	U16 dummy_end;
} vdoply_osd_vdoply_layout_struct;



/***************************************************************************** 
* Local Variable
*****************************************************************************/
/* video osd display cntx */
extern const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx;
extern const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx;


#endif /* __MMI_VIDEO__ */
#endif /* _VIDEO_SKIN_H_ */


