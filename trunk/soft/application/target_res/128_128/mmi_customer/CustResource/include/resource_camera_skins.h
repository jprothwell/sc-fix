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
 *	 resource_camera_skins.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Camera customization file.
 *
 * Author:
 * -------
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/include/resource_camera_skins.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:01:06   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:41:34   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:26:56   zhangxb
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _CAMERA_SKIN_H_
#define _CAMERA_SKIN_H_

#include "mmi_features.h"
#ifdef __MMI_CAMERA__
#include "resource_header.h" /* wufasong added 2007.03.22 */
#include "cswtype.h"         /* wufasong added 2007.03.22 */


/***************************************************************************** 
* Define
*****************************************************************************/
#if defined(MT6228) || defined(MT6229)
	 
	#define CAMREA_SOUND_COUNTDOWN_LEN	(164)
	#define CAMERA_SOUND_SHUTTER_1_LEN	(4798)
	#define CAMERA_SOUND_SHUTTER_2_LEN	(3532)
	#define CAMERA_SOUND_SHUTTER_3_LEN	(6960)
	#define CAMERA_SOUND_DUMMY_BG_LEN	(94)
#else
	#define CAMREA_SOUND_COUNTDOWN_LEN	(164)
	#define CAMERA_SOUND_SHUTTER_1_LEN	(1199)
	#define CAMERA_SOUND_SHUTTER_2_LEN	(883)
	#define CAMERA_SOUND_SHUTTER_3_LEN	(1740)
	#define CAMERA_SOUND_DUMMY_BG_LEN	(94)	
#endif 


typedef enum {
	CAMERA_TEXT_STYLE_NORMAL,
	CAMERA_TEXT_STYLE_BORDER,
	CAMERA_TEXT_STYLE_SHADOW	
} mmi_camera_text_style;


/***************************************************************************** 
* Struct
*****************************************************************************/
typedef struct
{
	BOOL 	is_show;
	S32	offset_x;
	S32	offset_y;
} camera_osd_icon_struct;

typedef struct
{
	U8 r;
	U8 g;
	U8 b;
} camera_color_struct;

typedef struct {
	U8		r;
	U8		g;	
	U8		b;		
	U8		style_r;
	U8		style_g;	
	U8		style_b;
	U16	style;	
} camera_style_text_struct;

typedef struct
{
	S32 	offset_x;
	S32	offset_y;
	S32	width;
	S32 	height;
} camera_osd_panel_struct;

typedef struct
{
	BOOL	is_draw_mmi_softkey;
	camera_style_text_struct	lsk;
	camera_style_text_struct	rsk;
} camera_osd_softkey_struct;

typedef struct
{
	S32 	offset_x;
	S32	offset_y;
	S32	width;
	S32 	height;
} camera_preview_wnd_struct;


typedef struct
{
	BOOL	is_show;
	S32 	offset_x;
	S32	offset_y;
	S32	width;
	S32 	height;
	camera_style_text_struct style_text;
} camera_osd_hint_box_struct;


typedef struct 
{	
	BOOL 	is_show;
	BOOL	is_right_align;	
	S32 	offset_x;
	S32	offset_y;
	camera_style_text_struct style_text;
} camera_osd_storage_struct;

typedef struct 
{	
	BOOL 	is_draw_bg_image;
	BOOL	is_draw_bg_color;
	U8		r;
	U8		g;	
	U8		b;	
} camera_osd_bg_struct;

typedef struct 
{	
	BOOL 	is_draw_mmi_title_bar;
	BOOL	is_draw_mmi_caption;
} camera_osd_title_struct;

typedef struct
{
	S32	offset_x;
	S32	offset_y;
	S32	icon_per_page;
} camera_osd_iconbar_struct;
	

typedef struct {

	/* background */
	camera_osd_bg_struct				bg;

	/* title */
	camera_osd_title_struct			title;

	/* softkey (buttom bar) */
	camera_osd_softkey_struct		softkey;
	
	/* preview wnd */
	camera_preview_wnd_struct		preview_wnd;
	
	/* camera setting */
	camera_osd_icon_struct 			wb;
	camera_osd_icon_struct 			flash;
	camera_osd_icon_struct 			led_highlight;	
	camera_osd_icon_struct 			ev;		
	camera_osd_icon_struct 			ev_inc;		/* for touch screen */
	camera_osd_icon_struct 			ev_dec;		/* for touch screen */
	camera_osd_icon_struct			night;
	camera_osd_icon_struct			closeup;
	
	/* image setting  */ 
	camera_osd_icon_struct			image_size;
	camera_osd_icon_struct			image_qty;
	
	/* others */
	camera_osd_icon_struct			cont_shot;
	camera_osd_icon_struct			delay_timer;
	camera_osd_icon_struct			add_frame;	
	camera_osd_icon_struct			storage;
	camera_osd_icon_struct			effect;
	camera_osd_icon_struct			ae_meter;	
	camera_osd_icon_struct			iso;	
	camera_osd_icon_struct			shot_mode;	
	camera_osd_icon_struct			capture;	
	camera_osd_icon_struct			zoom;	
	camera_osd_icon_struct 			zoom_inc;		/* for touch screen */
	camera_osd_icon_struct 			zoom_dec;		/* for touch screen */

	/* icon list */
	camera_osd_iconbar_struct		iconbar;
	
	camera_osd_hint_box_struct		hint_box;
	camera_osd_storage_struct		remain_storage;

} camera_osd_layeout_struct;


typedef struct {
	camera_style_text_struct	poplist_title_text;
	camera_style_text_struct	poplist_list_text;
	camera_style_text_struct	poplist_list_highlight_text;

	camera_color_struct			poplist_bg_color;
	camera_color_struct			poplist_highlight_color;
	
	U16								poplist_bg_trans;
	
	S32								poplist_top_margin;
	S32								poplist_bottom_margin;	
	S32								poplist_left_margin;
	S32								poplist_right_margin;
} camera_osd_menu_struct;


/***************************************************************************** 
* Local Variable
*****************************************************************************/
/* camera osd display cntx */
const extern camera_osd_layeout_struct g_camera_osd_cntx;
/* +ven */

#ifdef __CAMERA_OSD_HORIZONTAL__	
extern camera_osd_layeout_struct g_camera_osd_cntx_270;
#endif

/* end +ven */

const extern camera_osd_menu_struct		g_camera_osd_menu_cntx;

/* camera sound */
#ifndef WIN32
	extern __align(2) const U8 camera_sound_countdown[CAMREA_SOUND_COUNTDOWN_LEN];
	extern __align(2) const U8 camera_sound_shutter_1[CAMERA_SOUND_SHUTTER_1_LEN];
	extern __align(2) const U8 camera_sound_shutter_2[CAMERA_SOUND_SHUTTER_2_LEN];
	extern __align(2) const U8 camera_sound_shutter_3[CAMERA_SOUND_SHUTTER_3_LEN];
	extern __align(2) const U8 camera_sound_dummy_bg[CAMERA_SOUND_DUMMY_BG_LEN];
#else
	extern const U8 camera_sound_countdown[CAMREA_SOUND_COUNTDOWN_LEN];
	extern const U8 camera_sound_shutter_1[CAMERA_SOUND_SHUTTER_1_LEN];
	extern const U8 camera_sound_shutter_2[CAMERA_SOUND_SHUTTER_2_LEN];
	extern const U8 camera_sound_shutter_3[CAMERA_SOUND_SHUTTER_3_LEN];
	extern const U8 camera_sound_dummy_bg[CAMERA_SOUND_DUMMY_BG_LEN];	
#endif 







#endif /* __MMI_CAMERA__ */
#endif /* _CAMERA_SKIN_H_ */


