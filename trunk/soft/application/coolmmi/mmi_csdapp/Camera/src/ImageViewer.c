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
 *	 ImageViewer.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Image Viewer.
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
#include "mmi_features.h"
#ifdef __MMI_CAMERA__

#define __NEWSIMULATOR 

#include "stdc.h"
#include "l4dr1.h"
#include "globaldefs.h"

#include "custmenures.h"
#include "custdatares.h"						/* res handle functions */
#include "globalconstants.h"					/* key id define */
#include "fat_fs.h"								/* must after globalconstants.h */
#include "globaldefs.h"							/* golbal image and string id. */
#include "gui.h"									/* gui functions */
#include "wgui_categories.h"
#include "historygprot.h"						/* screen history */
#include "unicodexdcl.h"						/* unicode transform functions */
#include "eventsgprot.h"		
#include "filesystemdef.h"						/* file function */
#include "nvramtype.h"							/* nvram data type */
#include "nvramprot.h"							/* nvram access fucntions */	
#include "nvramenum.h"							/* nvram id define */
#include "settingdefs.h"						/* st_notifyduration define */
#include "wgui_categories_multimedia.h"
#include "commonscreens.h"						/* popup */
#include "mainmenudef.h"						/* multi-media icon */
#include "frameworkstruct.h"					/* for psextfuncptr */


#include "lcd_sw_rnd.h"						/* for lcd size */
#ifndef __MMI_DRAW_MANAGER__
#define __MMI_DRAW_MANAGER__
#endif
#include "wgui_draw_manager.h"			/* for new imageviewer [dm_coordinate] */

#ifdef __MMI_DRAW_MANAGER__
#undef __MMI_DRAW_MANAGER__
#endif

#include "gdi_include.h"						/* graphic library */
#include "mdi_datatype.h"
#include "mdi_camera.h"							/* camera lib */

#include "settingprofile.h"					/* warning tone */
#include "filemanagerdef.h"					/* error string id */
#include "filemgr.h"

#include "mmi_features_camera.h"				/* features - in custom folder */
#include "cameraresdefs.h"
#include "cameraapp.h"							/* for storage file path */
#include "imageviewer.h"						/* iamge viewer header */
#include "imageviewerresdefs.h"				/* image viewer res ids */

#ifdef __DRM_SUPPORT__
	#include "fs_errcode.h"
	#include "drm_gprot.h"						/* drm support */
#endif

#if defined(__CAMERA_OSD_HORIZONTAL__) 
	#include "screenrotationgprot.h"
#endif
#include "vdoplystrenum.h"
#include "mmi_trace.h"

//#include "adp_filesystem.h" //wufasong added 2007.01.08

/***************************************************************************** 
* Define
*****************************************************************************/

#define IMAGEVIEWER_DISPLAY_LAYER_WIDTH   (LCD_WIDTH)
#define IMAGEVIEWER_DISPLAY_LAYER_HEIGHT	(LCD_HEIGHT)
#define IMAGEVIEWER_CACHE_LAYER_WIDTH	(LCD_WIDTH)
#define IMAGEVIEWER_CACHE_LAYER_HEIGHT	(LCD_HEIGHT)

#define IMAGEVIEWER_DISPLAY_BUFFER_SIZE	(((IMAGEVIEWER_DISPLAY_LAYER_WIDTH*IMAGEVIEWER_DISPLAY_LAYER_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3))
#define IMAGEVIEWER_CACHE_BUFFER_SIZE	(((IMAGEVIEWER_CACHE_LAYER_WIDTH*IMAGEVIEWER_CACHE_LAYER_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3))
#define IMAGEVIEWER_NUM_ZOOM_FACTOR (5) /* 1,2,4,8,16,32 */

/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef struct{
	S8 *filepath;
	S8	*count_str;
} imgview_context_struct;

#if defined(__CAMERA_OSD_HORIZONTAL__) 
typedef struct{
	S8 *filepath;
	S8	*count_str;

	pBOOL is_resume;
	pBOOL is_gif_file;
	pBOOL is_permitted;

	gdi_handle	display_layer_handle;
	gdi_handle 	cache_layer_handle;
	gdi_handle	viewport_layer_handle;
	gdi_handle  thumbnail_layer_handle;

	PU8 display_buffer_ptr;
	PU8 cache_buffer_ptr;

	/* app information */
	U16 zoom_factor_cnt;
	float zoom_factor;
	pBOOL image_movable;
	pBOOL is_zoomin_able;
	pBOOL is_zoomout_able;
	pBOOL is_reextract;
	pBOOL is_zoomin_pressed;
	pBOOL is_zoomout_pressed;
	pBOOL is_move_left_pressed;
	pBOOL is_move_right_pressed;
	pBOOL is_move_up_pressed;
	pBOOL is_move_down_pressed;
	pBOOL is_trigger_by_keypad;
	U8		imgview_rotate;

	pBOOL					is_err;
	
	cat226_screen_layout_enum	screen_mode;
	GDI_RESULT					result;
	/* img information */
	S32 image_width;
	S32 image_height;
	U16 image_type	;
	
	/* x, y, width, height in display layer's coordinate */
	U16 display_wnd_width;
	U16 display_wnd_height;
	S32 display_wnd_x;
	S32 display_wnd_y;
	S32 display_roi_x;
	S32 display_roi_y;
	U16 display_layer_width;
	U16 display_layer_height;

	U16 cache_layer_width;
	U16 cache_layer_height;

	/*  x, y, width, height in the image's coordinate */
	U16 viewport_wnd_width;
	U16 viewport_wnd_height;
	S32 viewport_wnd_x;
	S32 viewport_wnd_y;
	S32 viewport_roi_x;
	S32 viewport_roi_y;

	/* the new x, y of the display_wnd */
	S32 display_current_x;
	S32 display_current_y;

	/* the size of the  resized image */
	S32 resized_img_width;
	S32 resized_img_height;

	/* x, y, width, height in the thumbnail layer's coordinate */
	S32 thumbnail_wnd_width; 
	S32 thumbnail_wnd_height;
	S32 thumbnail_wnd_x;
	S32 thumbnail_wnd_y;
	S32 thumbnail_layer_width;
	S32 thumbnail_layer_height;

	/* x, y, width, height in the redbox's coordinate */	
	S32 redbox_wnd_width;
	S32 redbox_wnd_height;
	S32 redbox_wnd_x;
	S32 redbox_wnd_y;

	/* bounding box (x1,y1) (x2,y2) */
	S32 bounding_box_x1;
	S32 bounding_box_y1;
	S32 bounding_box_x2;	
	S32 bounding_box_y2;

	/* viewbox position*/
	S32 display_x;
	S32 display_y;

	/* debug info */
	pBOOL is_debug;
	
	U16 sort_option;
	S32 move_offset_x;		
	S32 move_offset_y;
#ifdef __MMI_TOUCH_SCREEN__
	/* pen event */
	S32 pen_down_x;
	S32 pen_down_y;
	pBOOL is_pen_down;	
#endif

} imageviewer_context_struct;
#endif
/***************************************************************************** 
* Global Variable
*****************************************************************************/
extern BOOL g_hasMountedTFlashFS;

/***************************************************************************** 
* Global Function
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/
imgview_context_struct g_imgview_context;

#if defined(__CAMERA_OSD_HORIZONTAL__) 
imageviewer_context_struct g_imageviewer_cntx;
float imageviewer_zoom_factor[IMAGEVIEWER_NUM_ZOOM_FACTOR]={1,1.414,2,2.83,4};


#ifndef APPMEM_SUPPORT
	U32 imageviewer_display_layer_buffer[(IMAGEVIEWER_DISPLAY_BUFFER_SIZE)/4];
	U32 imageviewer_cache_layer_buffer[IMAGEVIEWER_CACHE_BUFFER_SIZE/4];
#endif /* APPMEM_SUPPORT */
#endif

/***************************************************************************** 
* Local Function
*****************************************************************************/
/* view detail image screen */
void mmi_imgview_entry_view_detail_screen(void);
void mmi_imgview_exit_view_detail_screen(void);
void mmi_imgview_view_detail_press_up(void);
void mmi_imgview_view_detail_press_down(void);


/* storage setting */
void mmi_imgview_entry_set_storage_screen(void);
void mmi_imgview_hint_set_storage(U16 index);
void mmi_imgview_highlight_set_storage(void);
void mmi_imgview_set_storage_value(void);

/* view style */
void mmi_imgview_entry_set_view_style_screen(void);
void mmi_imgview_set_view_style_value(void);

static void mmi_imageviewer_display_popup(PU8 str_ptr,
														U16 img_id, 
														U16 image_on_bottom, 
														U16 tone_duration, 
														U16 tone_id);

/* image util funtion */
#if defined(__CAMERA_OSD_HORIZONTAL__) 

/* imageviewer entry/exit function */
void mmi_imageviewer_entry_app_screen(void);
void mmi_imageviewer_exit_app_screen(void);
void mmi_imageviewer_exit_app(void);
void mmi_imageviewer_view_the_latest(void);
cat226_screen_layout_enum mmi_imageviewer_get_screen_mode(void);

static void mmi_imageviewer_zoomin(void);
static void mmi_imageviewer_zoomout(void);
static void mmi_imageviewer_move_up(void);
static void mmi_imageviewer_move_down(void);
static void mmi_imageviewer_move_left(void);
static void mmi_imageviewer_move_right(void);

static void mmi_imageviewer_exit_button_pressed(void);

static void mmi_imageviewer_zoomin_pressed(void);
static void mmi_imageviewer_zoomout_pressed(void);
static void mmi_imageviewer_move_up_pressed(void);
static void mmi_imageviewer_move_down_pressed(void);
static void mmi_imageviewer_move_left_pressed(void);
static void mmi_imageviewer_move_right_pressed(void);

static void mmi_imageviewer_zoomin_released(void);
static void mmi_imageviewer_zoomout_released(void);
static void mmi_imageviewer_move_up_released(void);
static void mmi_imageviewer_move_down_released(void);
static void mmi_imageviewer_move_left_released(void);
static void mmi_imageviewer_move_right_released(void);

static void mmi_imageviewer_translate_to_xy(S32 offset_x, S32 offset_y);
#ifdef __MMI_TOUCH_SCREEN__
	static MMI_BOOL mmi_imageviewer_pen_down_hdlr(mmi_pen_point_struct pos);
	static MMI_BOOL mmi_imageviewer_pen_up_hdlr(mmi_pen_point_struct pos);
	static MMI_BOOL mmi_imageviewer_pen_move_hdlr(mmi_pen_point_struct pos);
#endif /* __MMI_TOUCH_SCREEN__ */

static void mmi_imageviewer_switch_debug(void);

static void mmi_imageviewer_next_image(void);
static void mmi_imageviewer_pre_image(void);
static void mmi_imageviewer_rotate(void);
static void mmi_imageviewer_switch_screen_mode(void);
static void mmi_imageviewer_horizontal_fullscreen_mode(void);
static void mmi_imageviewer_horzontal_mode(void);
static void mmi_imageviewer_vertical_fullscreen_mode(void);
static void mmi_imageviewer_vertical_mode(void);
static void mmi_imageviewer_set_screen_mode(cat226_screen_layout_enum	screen_mode);
static void mmi_imageviewer_init_key_handler(void);

/* image update related function */
static void mmi_imageviewer_init_image(dm_coordinates *coordinate);
static void mmi_imageviewer_init_thumbnail(dm_coordinates *coordinate);
static void mmi_imageviewer_refresh_image(void);
static void mmi_imageviewer_reextract_image(void);
static void mmi_imageviewer_extract_image_to_display_layer(void);
static void mmi_imageviewer_extract_image_to_cache_layer(void);
static void mmi_imageviewer_update_roi(void);
static void mmi_imageviewer_init_image_size(void);
static void mmi_imageviewer_fix_wnd(void);

/* resource related function */
static void mmi_imageviewer_create_resource(void);
static void mmi_imageviewer_release_resource(void);
#endif



extern U16 mmi_fmgr_kernel_get_file_filter_count();
extern BOOL is_phb_init_done(void);

static void mmi_imageviewer_display_popup(PU8 str_ptr,
														U16 img_id, 
														U16 image_on_bottom, 
														U16 tone_duration, 
														U16 tone_id)
{
	#if defined(__CAMERA_OSD_HORIZONTAL__) 
		DisplayPopupRotated(	(PU8)str_ptr, 
									(U16)img_id, 
									(U8)image_on_bottom, 
									(U32)tone_duration, 
									(U8)tone_id, 
									g_imageviewer_cntx.imgview_rotate);
	#else
		DisplayPopup(	(PU8)str_ptr, 
							(U16)img_id, 
							(U8)image_on_bottom, 
							(U32)tone_duration, 
							(U8)tone_id);
	#endif
}


#if defined(__CAMERA_OSD_HORIZONTAL__)
/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_reset_view_mode
* DESCRIPTION
*  reset view mode
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif 
/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_switch_screen_mode
* DESCRIPTION
*  switch view mode
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_switch_screen_mode(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.screen_mode%2 == 0)
		g_imageviewer_cntx.screen_mode++;
	else
		g_imageviewer_cntx.screen_mode--;

	EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
	DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
	mmi_imageviewer_entry_app_screen();
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_set_screen_mode
* DESCRIPTION
*  set view mode
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_set_screen_mode(cat226_screen_layout_enum screen_mode)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_imageviewer_cntx.screen_mode = screen_mode;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_get_view_mode
* DESCRIPTION
*  get view mode
* PARAMETERS
*  void
* RETURNS
*  cat226_screen_layout_enum
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
cat226_screen_layout_enum mmi_imageviewer_get_screen_mode(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	return g_imageviewer_cntx.screen_mode;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_switch_debug
* DESCRIPTION
*  switch debug flag (on/off)
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_switch_debug(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_imageviewer_cntx.is_debug = !g_imageviewer_cntx.is_debug;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_fix_wnd
* DESCRIPTION
*  fix height of view wnd and redbox 
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_fix_wnd()
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 tmp;
	float tmp_ratio;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	if(g_imageviewer_cntx.resized_img_height >= g_imageviewer_cntx.display_wnd_height)
	{
		/* cal the real viewport_wnd_height -> tmp */
		tmp_ratio = (float)g_imageviewer_cntx.image_height/g_imageviewer_cntx.resized_img_height ;
		tmp = g_imageviewer_cntx.display_wnd_height * tmp_ratio;

		/* cal the ratio of (new viewport_wnd_height: old viewport_wnd_height) */
		tmp_ratio = (float) tmp/g_imageviewer_cntx.viewport_wnd_height ;

		/* update new viewport_wnd_height */
		g_imageviewer_cntx.viewport_wnd_height  = tmp;

		/* update new redbox_wnd_height */
		g_imageviewer_cntx.redbox_wnd_height *= tmp_ratio;
	}

	if(g_imageviewer_cntx.resized_img_width >= g_imageviewer_cntx.display_wnd_width)
	{
		/* cal the real viewport_wnd_height -> tmp */
		tmp_ratio = (float)g_imageviewer_cntx.image_width/g_imageviewer_cntx.resized_img_width ;
		tmp = g_imageviewer_cntx.display_wnd_width* tmp_ratio;

		/* cal the ratio of (new viewport_wnd_width: old viewport_wnd_width) */
		tmp_ratio = (float) tmp/g_imageviewer_cntx.viewport_wnd_width ;

		/* update new viewport_wnd_width */
		g_imageviewer_cntx.viewport_wnd_width  = tmp;

		/* update new redbox_wnd_width */
		g_imageviewer_cntx.redbox_wnd_width *= tmp_ratio;
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_zoomin_pressed
* DESCRIPTION
*  press zoomin
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_zoomin_pressed(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_imageviewer_cntx.is_trigger_by_keypad = TRUE;
	if(!g_imageviewer_cntx.is_zoomin_pressed)
		mmi_imageviewer_zoomin();

	g_imageviewer_cntx.is_zoomin_pressed = TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_zoomin_released
* DESCRIPTION
*  release zoomin
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_zoomin_released(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_imageviewer_cntx.is_zoomin_pressed = FALSE;
	g_imageviewer_cntx.is_trigger_by_keypad = FALSE;
	
	coolsand_UI_cancel_timer(mmi_imageviewer_zoomin);
	if(g_imageviewer_cntx.is_reextract )
	{
		mmi_imageviewer_reextract_image();
		g_imageviewer_cntx.is_reextract = FALSE;
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_zoomout_pressed
* DESCRIPTION
*  press zoomout
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_zoomout_pressed(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_imageviewer_cntx.is_trigger_by_keypad = TRUE;
	if(!g_imageviewer_cntx.is_zoomout_pressed)
		mmi_imageviewer_zoomout();

	g_imageviewer_cntx.is_zoomout_pressed = TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_zoomout_released
* DESCRIPTION
*  release zoomout
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_zoomout_released(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_imageviewer_cntx.is_trigger_by_keypad = FALSE;
	g_imageviewer_cntx.is_zoomout_pressed = FALSE;
	coolsand_UI_cancel_timer(mmi_imageviewer_zoomout);
	if(g_imageviewer_cntx.is_reextract )
	{
		mmi_imageviewer_reextract_image();
		g_imageviewer_cntx.is_reextract = FALSE;
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_up_pressed
* DESCRIPTION
*  press move up image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_up_pressed(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return;
	
	if(!g_imageviewer_cntx.is_move_up_pressed)
		mmi_imageviewer_move_up();

	g_imageviewer_cntx.is_move_up_pressed = TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_up_released
* DESCRIPTION
*  release move up image 
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_up_released(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return;
	
	g_imageviewer_cntx.is_move_up_pressed = FALSE;
	coolsand_UI_cancel_timer(mmi_imageviewer_move_up);
	if(g_imageviewer_cntx.is_reextract )
	{
		mmi_imageviewer_reextract_image();
		g_imageviewer_cntx.is_reextract = FALSE;
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_down_pressed
* DESCRIPTION
*  press move down image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_down_pressed(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return;
	
	if(!g_imageviewer_cntx.is_move_down_pressed)
		mmi_imageviewer_move_down();

	g_imageviewer_cntx.is_move_down_pressed = TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_down_released
* DESCRIPTION
*  release move down image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_down_released(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return;
	
	g_imageviewer_cntx.is_move_down_pressed = FALSE;
	coolsand_UI_cancel_timer(mmi_imageviewer_move_down);
	if(g_imageviewer_cntx.is_reextract )
	{
		mmi_imageviewer_reextract_image();
		g_imageviewer_cntx.is_reextract = FALSE;
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_left_pressed
* DESCRIPTION
*  press move left image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_left_pressed(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return;
	
	if(!g_imageviewer_cntx.is_move_left_pressed)
		mmi_imageviewer_move_left();

	g_imageviewer_cntx.is_move_left_pressed = TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_left_released
* DESCRIPTION
*  release move left image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_left_released(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return;
	
	g_imageviewer_cntx.is_move_left_pressed = FALSE;
	coolsand_UI_cancel_timer(mmi_imageviewer_move_left);
	if(g_imageviewer_cntx.is_reextract )
	{
		mmi_imageviewer_reextract_image();
		g_imageviewer_cntx.is_reextract = FALSE;
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_right_pressed
* DESCRIPTION
*  press move right image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_right_pressed(void)
{

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return;
	
	if(!g_imageviewer_cntx.is_move_right_pressed)
		mmi_imageviewer_move_right();

	g_imageviewer_cntx.is_move_right_pressed = TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_right_released
* DESCRIPTION
*  release move right image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_right_released(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return;
	
	g_imageviewer_cntx.is_move_right_pressed = FALSE;
	coolsand_UI_cancel_timer(mmi_imageviewer_move_right);
	if(g_imageviewer_cntx.is_reextract )
	{
		mmi_imageviewer_reextract_image();
		g_imageviewer_cntx.is_reextract = FALSE;
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_reextract_image
* DESCRIPTION
*  reextract image to display layer and draw it on lcd
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_reextract_image(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.is_reextract)
	{
		g_imageviewer_cntx.display_roi_x -= g_imageviewer_cntx.display_current_x-g_imageviewer_cntx.display_wnd_x; 
		g_imageviewer_cntx.display_roi_y -= g_imageviewer_cntx.display_current_y-g_imageviewer_cntx.display_wnd_y;
		mmi_imageviewer_extract_image_to_display_layer();
		mmi_imageviewer_refresh_image();
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_up
* DESCRIPTION
*  move image up
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_up(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 offset = -g_imageviewer_cntx.move_offset_y;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(g_imageviewer_cntx.display_current_y == g_imageviewer_cntx.display_roi_y)
		return;

	if(g_imageviewer_cntx.is_move_up_pressed)
		coolsand_UI_start_timer(100, mmi_imageviewer_move_up);
	else	/* first time press up arrow, use longer time */
		coolsand_UI_start_timer(300, mmi_imageviewer_move_up);

	g_imageviewer_cntx.display_current_y += offset;

	if(g_imageviewer_cntx.display_current_y < g_imageviewer_cntx.display_roi_y)
	{
		offset -= (g_imageviewer_cntx.display_current_y-g_imageviewer_cntx.display_roi_y);
		g_imageviewer_cntx.display_current_y = g_imageviewer_cntx.display_roi_y;		
	}
	mmi_imageviewer_translate_to_xy(0,offset);
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_down
* DESCRIPTION
*  move image down
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_down(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 offset = g_imageviewer_cntx.move_offset_y;
	S32 offset_height = (g_imageviewer_cntx.resized_img_height >= g_imageviewer_cntx.display_wnd_height)?
							g_imageviewer_cntx.resized_img_height-g_imageviewer_cntx.display_wnd_height:0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	if(g_imageviewer_cntx.display_current_y == g_imageviewer_cntx.display_roi_y+offset_height)
		return;

	if(g_imageviewer_cntx.is_move_down_pressed)
		coolsand_UI_start_timer(100, mmi_imageviewer_move_down);
	else	/* first time press up arrow, use longer time */
		coolsand_UI_start_timer(300, mmi_imageviewer_move_down);

	g_imageviewer_cntx.display_current_y += offset;

	if(g_imageviewer_cntx.display_current_y > g_imageviewer_cntx.display_roi_y+g_imageviewer_cntx.resized_img_height-g_imageviewer_cntx.display_wnd_height +1)
	{
		offset -=  (g_imageviewer_cntx.display_current_y -(g_imageviewer_cntx.display_roi_y+g_imageviewer_cntx.resized_img_height-g_imageviewer_cntx.display_wnd_height +1));
		g_imageviewer_cntx.display_current_y = g_imageviewer_cntx.display_roi_y
															+g_imageviewer_cntx.resized_img_height
															-g_imageviewer_cntx.display_wnd_height +1;
	}

	mmi_imageviewer_translate_to_xy(0,offset);

}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_left
* DESCRIPTION
*  move image left
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_left(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 offset = -g_imageviewer_cntx.move_offset_x;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(g_imageviewer_cntx.display_current_x == g_imageviewer_cntx.display_roi_x)
		return;

	if(g_imageviewer_cntx.is_move_left_pressed)
		coolsand_UI_start_timer(100, mmi_imageviewer_move_left);
	else	/* first time press up arrow, use longer time */
		coolsand_UI_start_timer(300, mmi_imageviewer_move_left);

	g_imageviewer_cntx.display_current_x += offset;

	if(g_imageviewer_cntx.display_current_x < g_imageviewer_cntx.display_roi_x)
	{
		offset -= (g_imageviewer_cntx.display_current_x - g_imageviewer_cntx.display_roi_x);
		g_imageviewer_cntx.display_current_x = g_imageviewer_cntx.display_roi_x;
	}
	mmi_imageviewer_translate_to_xy(offset,0);
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_move_right
* DESCRIPTION
*  move image left
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_move_right(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 offset = g_imageviewer_cntx.move_offset_x;
	S32 offset_width = (g_imageviewer_cntx.resized_img_width >= g_imageviewer_cntx.display_wnd_width)?
							g_imageviewer_cntx.resized_img_width-g_imageviewer_cntx.display_wnd_width:0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(g_imageviewer_cntx.display_current_x == g_imageviewer_cntx.display_roi_x+offset_width)
		return;

	if(g_imageviewer_cntx.is_move_right_pressed)
		coolsand_UI_start_timer(100, mmi_imageviewer_move_right);
	else	/* first time press up arrow, use longer time */
		coolsand_UI_start_timer(300, mmi_imageviewer_move_right);

	g_imageviewer_cntx.display_current_x += offset;

	if(g_imageviewer_cntx.display_current_x >= g_imageviewer_cntx.display_roi_x+offset_width)
	{
		offset -= (g_imageviewer_cntx.display_current_x - (g_imageviewer_cntx.display_roi_x+offset_width));
		g_imageviewer_cntx.display_current_x = g_imageviewer_cntx.display_roi_x+offset_width;
	}
	mmi_imageviewer_translate_to_xy(offset,0);
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_rotate
* DESCRIPTION
*  rotate application
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_rotate(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if (g_imageviewer_cntx.screen_mode <2)
	{
		g_imageviewer_cntx.screen_mode += 2;
		g_imageviewer_cntx.imgview_rotate = 3 ;
	}
	else 
	{
		g_imageviewer_cntx.screen_mode -= 2;
		g_imageviewer_cntx.imgview_rotate = 0;
	}
	EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
	DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
	mmi_imageviewer_entry_app_screen();
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_horizontal_fullscreen_mode
* DESCRIPTION
*  switch to horizontal full screen mode
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_horizontal_fullscreen_mode(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_imageviewer_cntx.screen_mode = HORIZONTAL_FULLSCREEN;
	g_imageviewer_cntx.imgview_rotate = 3 ;
	EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
	DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
	mmi_imageviewer_entry_app_screen();
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_horzontal_mode
* DESCRIPTION
*  switch to horizontal normal mode
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_horzontal_mode(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_imageviewer_cntx.screen_mode = HORIZONTAL_NORMAL;
	g_imageviewer_cntx.imgview_rotate = 3 ;
	EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
	DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
	mmi_imageviewer_entry_app_screen();
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_vertical_fullscreen_mode
* DESCRIPTION
*  switch to vertical full screen mode
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_vertical_fullscreen_mode(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#if defined(__CAMERA_OSD_HORIZONTAL__)
	if(g_imageviewer_cntx.screen_mode == HORIZONTAL_FULLSCREEN)
	#else
	if(g_imageviewer_cntx.screen_mode == VERTICAL_FULLSCREEN)
	#endif
		g_imageviewer_cntx.screen_mode = VERTICAL_NORMAL;
	else 
		g_imageviewer_cntx.screen_mode++;


	if(g_imageviewer_cntx.screen_mode<2)
		g_imageviewer_cntx.imgview_rotate = 0 ;
	else
		g_imageviewer_cntx.imgview_rotate = 3 ;

	EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
	DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
	mmi_imageviewer_entry_app_screen();	
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_vertical_mode
* DESCRIPTION
*  switch to vertical normal mode
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_vertical_mode(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_imageviewer_cntx.screen_mode = VERTICAL_NORMAL;
	g_imageviewer_cntx.imgview_rotate = 0 ;
	EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
	DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
	mmi_imageviewer_entry_app_screen();
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_next_image
* DESCRIPTION
*  next image if exist
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_next_image()
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32							index;
	S32							cur_index;
	FMGR_FILE_INFO_STRUCT	file_info;
	S32							counter;
	S32							file_count;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* exit and clear previous screen for history - then enter again */
	cur_index	= mmi_fmgr_kernel_get_cur_idx();
	file_count	= mmi_fmgr_kernel_get_file_count();
	counter		= 0;

	if(file_count > 1)
	{
		do{
		
			mmi_fmgr_kernel_move_selection_down();
			index = mmi_fmgr_kernel_get_cur_idx();
			mmi_fmgr_kernel_get_file_info(index, &file_info);

			counter++;

			if(!(file_info.attribute & FS_ATTR_DIR))
			{
				EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
				DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
				mmi_imageviewer_entry_app_screen();
				return;
			}

			if(counter >= (file_count-1))
				return;
		} while(1);
	}

}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_pre_image
* DESCRIPTION
*  previous image if exist
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_pre_image()
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32							index;
	S32							cur_index;
	FMGR_FILE_INFO_STRUCT	file_info;
	S32							counter;
	S32							file_count;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* exit and clear previous screen for history - then enter again */
	cur_index	= mmi_fmgr_kernel_get_cur_idx();
	file_count	= mmi_fmgr_kernel_get_file_count();
	counter		= 0;

	if(file_count > 1)
	{
		do{
			mmi_fmgr_kernel_move_selection_up();
			index = mmi_fmgr_kernel_get_cur_idx();
			mmi_fmgr_kernel_get_file_info(index, &file_info);

			counter++;

			if(!(file_info.attribute & FS_ATTR_DIR))
			{
				EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
				DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
				mmi_imageviewer_entry_app_screen();
				return;
			}

			if(counter >= (file_count-1))
				return;
		} while(1);
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_exit_app_screen
* DESCRIPTION
*  exit app screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_exit_app_screen()
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* free buffer */
	coolsand_UI_free(g_imageviewer_cntx.filepath);
	coolsand_UI_free(g_imageviewer_cntx.count_str);

	mmi_imageviewer_release_resource();
//	if(g_imageviewer_cntx.sort_option!=0)
//	fmgr_set_sort_option(g_imageviewer_cntx.sort_option);

	g_imgview_context.filepath		= NULL;
	g_imgview_context.count_str	= NULL;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_exit_app
* DESCRIPTION
*  exit application
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_exit_app()
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	mmi_imageviewer_release_resource();
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_release_resource
* DESCRIPTION
*  release layer related resource
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void	mmi_imageviewer_release_resource()
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/****** free display layer ******/
	gdi_layer_free(g_imageviewer_cntx.display_layer_handle);
	g_imageviewer_cntx.display_layer_handle= GDI_LAYER_EMPTY_HANDLE;


	/****** free cache layer ******/
	gdi_layer_free(g_imageviewer_cntx.cache_layer_handle);
	g_imageviewer_cntx.cache_layer_handle = GDI_LAYER_EMPTY_HANDLE;

	#ifdef APPMEM_SUPPORT
		/* with ASM */
		FreeMediaMemory(g_imageviewer_cntx.display_buffer_ptr); 
		g_imageviewer_cntx.display_buffer_ptr = NULL;
		FreeMediaMemory(g_imageviewer_cntx.cache_buffer_ptr); 
		g_imageviewer_cntx.cache_buffer_ptr = NULL;
	#endif /* APPMEM_SUPPORT */

	coolsand_UI_free(g_imgview_context.filepath);
	coolsand_UI_free(g_imgview_context.count_str);

	g_imgview_context.filepath		= NULL;
	g_imgview_context.count_str	= NULL;

}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_init_key_handler
* DESCRIPTION
*  init key handler
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_init_key_handler()
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_imageviewer_cntx.screen_mode==HORIZONTAL_FULLSCREEN || g_imageviewer_cntx.screen_mode==HORIZONTAL_NORMAL)
	{
		SetKeyHandler(mmi_imageviewer_move_down_pressed,KEY_4,KEY_EVENT_DOWN);	
		SetKeyHandler(mmi_imageviewer_move_left_pressed,KEY_2,KEY_EVENT_DOWN);	
		SetKeyHandler(mmi_imageviewer_move_up_pressed,KEY_6,KEY_EVENT_DOWN);	
		SetKeyHandler(mmi_imageviewer_move_right_pressed,KEY_8,KEY_EVENT_DOWN);	

		SetKeyHandler(mmi_imageviewer_move_down_released,KEY_4,KEY_EVENT_UP);	
		SetKeyHandler(mmi_imageviewer_move_left_released,KEY_2,KEY_EVENT_UP);	
		SetKeyHandler(mmi_imageviewer_move_up_released,KEY_6,KEY_EVENT_UP);	
		SetKeyHandler(mmi_imageviewer_move_right_released,KEY_8,KEY_EVENT_UP);	

		if(g_imageviewer_cntx.result >= 0)
		{
			SetKeyHandler(mmi_imageviewer_zoomin_pressed,KEY_3,KEY_EVENT_DOWN);	
			SetKeyHandler(mmi_imageviewer_zoomout_pressed,KEY_9,KEY_EVENT_DOWN);	
			SetKeyHandler(mmi_imageviewer_zoomin_released,KEY_3,KEY_EVENT_UP);	
			SetKeyHandler(mmi_imageviewer_zoomout_released,KEY_9,KEY_EVENT_UP);	
		}


	}
	else
	{
		SetKeyHandler(mmi_imageviewer_move_down_pressed,KEY_8,KEY_EVENT_DOWN);	
		SetKeyHandler(mmi_imageviewer_move_left_pressed,KEY_4,KEY_EVENT_DOWN);	
		SetKeyHandler(mmi_imageviewer_move_up_pressed,KEY_2,KEY_EVENT_DOWN);	
		SetKeyHandler(mmi_imageviewer_move_right_pressed,KEY_6,KEY_EVENT_DOWN);	

		SetKeyHandler(mmi_imageviewer_move_down_released,KEY_8,KEY_EVENT_UP);	
		SetKeyHandler(mmi_imageviewer_move_left_released,KEY_4,KEY_EVENT_UP);	
		SetKeyHandler(mmi_imageviewer_move_up_released,KEY_2,KEY_EVENT_UP);	
		SetKeyHandler(mmi_imageviewer_move_right_released,KEY_6,KEY_EVENT_UP);	

		if(g_imageviewer_cntx.result >= 0)
		{
			SetKeyHandler(mmi_imageviewer_zoomin_pressed,KEY_1,KEY_EVENT_DOWN);	
			SetKeyHandler(mmi_imageviewer_zoomout_pressed,KEY_3,KEY_EVENT_DOWN);	
			SetKeyHandler(mmi_imageviewer_zoomin_released,KEY_1,KEY_EVENT_UP);	
			SetKeyHandler(mmi_imageviewer_zoomout_released,KEY_3,KEY_EVENT_UP);	
		}
	}


	SetKeyHandler(mmi_imageviewer_rotate,KEY_5,KEY_EVENT_UP);	


}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_entry_app_screen
* DESCRIPTION
*  entry function
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_entry_app_screen()
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	FMGR_FILE_INFO_STRUCT	file_info;
	S32							index;
	S32							total_count;
	S8								buffer[30];				/* for xxxx/xxxx */
	FS_HANDLE					fs_handle;
	U16							image_id;
	cat226_screen_layout_enum screen_mode;
	U16				err_str_ptr;
	U32							file_size;
#ifdef __DRM_SUPPORT__
	FS_HANDLE					file_handle;
#endif

	S8					buf_filename_no_ext[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
	S32				str_len;
	U8								*guiBuffer;
       S8                               *ptr_filename = NULL;
    mmi_trace(g_sw_CAMERA, "Excute func : mmi_imageviewer_entry_app_screen()");
//#if 0
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* exit previous screen */
	EntryNewScreen(SCR_ID_IMGVIEW_VIEW, mmi_imageviewer_exit_app_screen, mmi_imageviewer_entry_app_screen, NULL);
 
	/* allocate string buffer */
	g_imageviewer_cntx.filepath	= coolsand_UI_malloc(FMGR_PATH_BUFFER_SIZE);
	g_imageviewer_cntx.count_str	= coolsand_UI_malloc(60);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_IMGVIEW_VIEW);
	if(guiBuffer != NULL)
		g_imageviewer_cntx.is_resume = TRUE;
	else
		g_imageviewer_cntx.is_resume = FALSE;

	/* create resource for imageviewer */
	mmi_imageviewer_create_resource();
	
	/* prepare xxxx/xxxx string */
	index			= mmi_fmgr_kernel_get_cur_idx();
	total_count = mmi_fmgr_kernel_get_file_count();

	if(index >= total_count)
	{
	   index = 0;
		mmi_fmgr_kernel_move_selection_down();
   }

	mmi_fmgr_kernel_get_file_info(index, &file_info);
	sprintf(buffer, "%d/%d", (U16)index+1, (U16)total_count);
	AnsiiToUnicodeString((PS8)g_imageviewer_cntx.count_str, (PS8)buffer);

	/* append the name to path */
	mmi_fmgr_kernel_get_path_name(g_imageviewer_cntx.filepath);

	pfnUnicodeStrcat(g_imageviewer_cntx.filepath, file_info.file_name);

	g_imageviewer_cntx.is_permitted = FALSE;
	/* DRM permission check */
#ifdef __DRM_SUPPORT__

	if((file_handle = DRM_open_file((PU16)g_imageviewer_cntx.filepath, FS_READ_ONLY, DRM_PERMISSION_NONE )) >= FS_NO_ERROR)
	{
		if(DRM_validate_permission(file_handle, DRM_PERMISSION_DISPLAY))
		{
			DRM_consume_rights(file_handle, DRM_PERMISSION_DISPLAY);
			g_imageviewer_cntx.is_permitted = TRUE;
		}
		DRM_close_file(file_handle);
	}
	else if(file_handle < -1) /* file open error but not DRM related problem */
	{
		g_imageviewer_cntx.is_permitted = TRUE;
	}
#endif /* __DRM_SUPPORT__ */

//#if 0
	if(g_imageviewer_cntx.is_permitted==FALSE)
	{
		g_imageviewer_cntx.result = -1;
		err_str_ptr =STR_GLOBAL_DRM_PROHIBITED;
	}
	else
	{
		g_imageviewer_cntx.result = gdi_image_get_dimension_file(	(PS8)g_imageviewer_cntx.filepath,
														&(g_imageviewer_cntx.image_width),
														&(g_imageviewer_cntx.image_height));
		if((g_imageviewer_cntx.image_width==0) || (g_imageviewer_cntx.image_height==0))
		{
			g_imageviewer_cntx.result = -1;
			err_str_ptr = STR_ID_FMGR_INVALID_IMAGE_FORMAT;
		}
		
		/* temp solution, waiting for decoder has resizer for gif and bmp */
		g_imageviewer_cntx.image_type = gdi_image_get_type_from_file(g_imageviewer_cntx.filepath);
	}


	str_len = pfnUnicodeStrlen(g_imageviewer_cntx.filepath);

	/* find file name - by finding "\\"*/
	for(index = str_len*2-2; index >= 2; index -= 2)
	{
		if( pfnUnicodeStrncmp((PS8)&g_imageviewer_cntx.filepath[index], (PS8)L"\\", 1) == 0)
		{
			index+=2;
			break;
		}
	}

	pfnUnicodeStrcpy((PS8)buf_filename_no_ext, (PS8)(&(g_imageviewer_cntx.filepath[index])));

	mmi_fmgr_hide_ext_name(buf_filename_no_ext);
       ptr_filename= mmi_fmgr_extract_file_name(buf_filename_no_ext);
//#if 0
	#if defined(__CAMERA_OSD_HORIZONTAL__) 
	if(g_imageviewer_cntx.screen_mode==HORIZONTAL_FULLSCREEN || g_imageviewer_cntx.screen_mode==HORIZONTAL_NORMAL)
		{
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
		}
	else
		{
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_0);
		}
	#endif

	switch(g_imageviewer_cntx.screen_mode)
	{
		case VERTICAL_FULLSCREEN:
		case HORIZONTAL_FULLSCREEN:
			image_id = 0;
			screen_mode = VERTICAL_FULLSCREEN;
			break;
		case VERTICAL_NORMAL:
			image_id = IMG_ID_IMAGEVIEWER_BACKGROUND;
			screen_mode = VERTICAL_NORMAL;
			break;
		case HORIZONTAL_NORMAL:
			image_id = IMG_ID_IMAGEVIEWER_BACKGROUND_HORIZONTAL;
			screen_mode = VERTICAL_NORMAL;
		break;
	}


		
	fs_handle = MMI_FS_Open((U16*)g_imageviewer_cntx.filepath, FS_READ_ONLY);

	if(fs_handle < 0)
	{
		/* open file failed */
		g_imageviewer_cntx.result = -1;
		err_str_ptr = STR_ID_FMGR_INVALID_IMAGE_FORMAT;
	}
	else
	{
		MMI_FS_GetFileSize(fs_handle, (kal_uint32*)&file_size);
		MMI_FS_Close(fs_handle);
	}


	/* check file size limit and image siz limit */
	if(g_imageviewer_cntx.result >= 0)
	{
		switch(g_imageviewer_cntx.image_type)
		{
			case GDI_IMAGE_TYPE_JPG_FILE:
				/* decoder will timeout if file too large, we dont need to protect here */
				break;
			
			case GDI_IMAGE_TYPE_BMP_FILE:
			case GDI_IMAGE_TYPE_WBMP_FILE:
				/* file size check - 512k */
				if(file_size > 512*1024)  
				{
					err_str_ptr = STR_ID_FMGR_FILE_TOO_LARGE_NO_DISPLAY;
					g_imageviewer_cntx.result = -1;					
				}
				break;			

			case GDI_IMAGE_TYPE_GIF_FILE:
				/* image size check */
				if((g_imageviewer_cntx.image_width > 640) || (g_imageviewer_cntx.image_height > 480))
				{
					err_str_ptr = STR_ID_FMGR_IMAGE_TOO_LARGE_NO_DISPLAY;
					g_imageviewer_cntx.result = -1;
				}
				break;							

		}
	}



//#if 0
	ShowCategory226Screen(0,0,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,image_id,
	(UI_string_type)ptr_filename,(UI_string_type)g_imageviewer_cntx.count_str,
		IMG_ID_IMAGEVIEWER_LEFT_ARROW_UP,IMG_ID_IMAGEVIEWER_LEFT_ARROW_DOWN, mmi_imgview_view_detail_press_up,
		IMG_ID_IMAGEVIEWER_RIGHT_ARROW_UP,IMG_ID_IMAGEVIEWER_RIGHT_ARROW_DOWN,mmi_imgview_view_detail_press_down,
		IMG_ID_IMAGEVIEWER_ZOOMIN_UP,IMG_ID_IMAGEVIEWER_ZOOMIN_DOWN,mmi_imageviewer_zoomin,
		IMG_ID_IMAGEVIEWER_ZOOMOUT_UP,IMG_ID_IMAGEVIEWER_ZOOMOUT_DOWN,mmi_imageviewer_zoomout,
		IMG_ID_IMAGEVIEWER_ROTATE_UP,IMG_ID_IMAGEVIEWER_ROTATE_DOWN,mmi_imageviewer_rotate,
		IMG_ID_IMAGEVIEWER_SCREEN_MODE_UP,IMG_ID_IMAGEVIEWER_SCREEN_MODE_DOWN,mmi_imageviewer_switch_screen_mode,
		&g_imageviewer_cntx.thumbnail_layer_handle, mmi_imageviewer_init_thumbnail,
		&g_imageviewer_cntx.viewport_layer_handle, mmi_imageviewer_init_image,
	screen_mode);

//#if 0
#if defined(__CAMERA_OSD_HORIZONTAL__) 
	if(g_imageviewer_cntx.screen_mode==HORIZONTAL_FULLSCREEN || g_imageviewer_cntx.screen_mode==HORIZONTAL_NORMAL)
	{
		if(g_imageviewer_cntx.thumbnail_layer_handle != -1)
		{
		gdi_layer_push_and_set_active(g_imageviewer_cntx.thumbnail_layer_handle);
		gdi_layer_set_rotate(MMI_FRM_SCREEN_ROTATE_270);
		gdi_layer_pop_and_restore_active();
		}
		gdi_layer_push_and_set_active(g_imageviewer_cntx.viewport_layer_handle);
		gdi_layer_set_rotate(MMI_FRM_SCREEN_ROTATE_270);
		gdi_layer_pop_and_restore_active();
	}
	else
	{
		if(g_imageviewer_cntx.thumbnail_layer_handle != -1)
		{
		gdi_layer_push_and_set_active(g_imageviewer_cntx.thumbnail_layer_handle);
		gdi_layer_set_rotate(MMI_FRM_SCREEN_ROTATE_0);
		gdi_layer_pop_and_restore_active();
		}
		gdi_layer_push_and_set_active(g_imageviewer_cntx.viewport_layer_handle);
		gdi_layer_set_rotate(MMI_FRM_SCREEN_ROTATE_0);
		gdi_layer_pop_and_restore_active();

	}
#endif


	if(g_imageviewer_cntx.is_err==FALSE||g_imageviewer_cntx.result == -1)
	{
		gdi_layer_push_and_set_active(g_imageviewer_cntx.viewport_layer_handle);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_layer_pop_and_restore_active();
		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

		if(g_imageviewer_cntx.is_err == TRUE)
			mmi_imageviewer_display_popup((PU8)GetString(err_str_ptr), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);

		g_imageviewer_cntx.is_err = FALSE;
		g_imageviewer_cntx.is_zoomin_able = FALSE;
	}
	else
	{
		if(g_imageviewer_cntx.is_resume == FALSE)
		{
		/* it will calculate roi */
		mmi_imageviewer_init_image_size();
		mmi_imageviewer_update_roi();
		}

		/* Will be changed data*/
		mmi_imageviewer_extract_image_to_display_layer();
		
		if(g_imageviewer_cntx.result >=0)		
		{
			mmi_imageviewer_extract_image_to_cache_layer();
			/* init move offset */
			g_imageviewer_cntx.move_offset_x = g_imageviewer_cntx.image_width/g_imageviewer_cntx.thumbnail_wnd_width;
			g_imageviewer_cntx.move_offset_y = g_imageviewer_cntx.image_height/g_imageviewer_cntx.thumbnail_wnd_height;
			if(g_imageviewer_cntx.is_resume == FALSE)
			{
				// int redbox information 
				g_imageviewer_cntx.redbox_wnd_x = 0;
				g_imageviewer_cntx.redbox_wnd_y = 0;
				g_imageviewer_cntx.redbox_wnd_width = g_imageviewer_cntx.thumbnail_wnd_width;
				g_imageviewer_cntx.redbox_wnd_height = g_imageviewer_cntx.thumbnail_wnd_height;
			}
		
			mmi_imageviewer_refresh_image();
		}
	}
	mmi_imageviewer_init_key_handler();
	
#if 1
	
	#ifdef __MMI_TOUCH_SCREEN__
			Cat226RegisterPenHandlers(1,mmi_imageviewer_pen_down_hdlr,MMI_PEN_EVENT_DOWN);
			Cat226RegisterPenHandlers(1,mmi_imageviewer_pen_up_hdlr,MMI_PEN_EVENT_UP);
			Cat226RegisterPenHandlers(1,mmi_imageviewer_pen_move_hdlr,MMI_PEN_EVENT_MOVE);
	#endif /* __MMI_TOUCH_SCREEN__ */


	/* if less or equal to 1 image, no need to switch to next image */
	if(total_count > 1)
	{
		SetKeyHandler(mmi_imgview_view_detail_press_down, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_imgview_view_detail_press_up,	KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	}

	SetRightSoftkeyFunction(mmi_imageviewer_exit_button_pressed,	KEY_EVENT_UP);
#endif
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_exit_button_pressed
* DESCRIPTION
*  pen up hdlr
* PARAMETERS
*  pos IN
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_exit_button_pressed(void)
{
	g_imageviewer_cntx.is_err = TRUE;	
	GoBackHistory();
}


#ifdef __MMI_TOUCH_SCREEN__

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_pen_up_hdlr
* DESCRIPTION
*  pen up hdlr
* PARAMETERS
*  pos IN
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static MMI_BOOL mmi_imageviewer_pen_up_hdlr(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* calculate the diff of pos with pen_down_pos */ 
	/* call the translate_to_xy */
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return FALSE;
	
	g_imageviewer_cntx.is_pen_down= FALSE;

	if(g_imageviewer_cntx.is_reextract )
	{
		mmi_imageviewer_reextract_image();
		g_imageviewer_cntx.is_reextract = FALSE;
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_pen_down_hdlr
* DESCRIPTION
*  pen down hdlr
* PARAMETERS
*  pos IN
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static MMI_BOOL mmi_imageviewer_pen_down_hdlr(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 pos_x,pos_y;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* check if it is in the display layer */
	/* if yes, remember the position and set pen_down flag */
	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return FALSE;
	
	pos_x = pos.x;
	pos_y = pos.y;

	if( pos_x > g_imageviewer_cntx.display_x && pos_x < g_imageviewer_cntx.display_x + g_imageviewer_cntx.display_wnd_width &&
		pos_y > g_imageviewer_cntx.display_y && pos_y < g_imageviewer_cntx.display_y + g_imageviewer_cntx.display_wnd_height)
	{
		g_imageviewer_cntx.is_pen_down = TRUE;
		g_imageviewer_cntx.pen_down_x = pos_x;
		g_imageviewer_cntx.pen_down_y = pos_y;
		return TRUE;
	}
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_pen_move_hdlr
* DESCRIPTION
*  pen move hdlr
* PARAMETERS
*  pos IN
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static MMI_BOOL mmi_imageviewer_pen_move_hdlr(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 offset_x,offset_y;
	S32 pos_x,pos_y;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return FALSE;

	if( pos.x < g_imageviewer_cntx.display_x )
		pos_x = g_imageviewer_cntx.display_x;
	else if( pos.x > g_imageviewer_cntx.display_x + g_imageviewer_cntx.display_wnd_width )
		pos_x = g_imageviewer_cntx.display_x + g_imageviewer_cntx.display_wnd_width ;
	else
		pos_x = pos.x ;

	if( pos.y <g_imageviewer_cntx.display_y )
		pos_y = g_imageviewer_cntx.display_y ;
	else if ( pos.y >g_imageviewer_cntx.display_y + g_imageviewer_cntx.display_wnd_height)
		pos_y = g_imageviewer_cntx.display_y + g_imageviewer_cntx.display_wnd_height;
	else 
		pos_y = pos.y;

	/* calculate the diff of pos with pen_down_pos */ 
	/* call the translate_to_xy */

	if(g_imageviewer_cntx.is_pen_down)
	{

		S32 offset_width = (g_imageviewer_cntx.resized_img_width >= g_imageviewer_cntx.display_wnd_width)?
							g_imageviewer_cntx.resized_img_width-g_imageviewer_cntx.display_wnd_width:0;
	
		offset_x = g_imageviewer_cntx.pen_down_x-pos.x  ;
		offset_y = g_imageviewer_cntx.pen_down_y-pos.y  ;
		offset_x -= (offset_x%g_imageviewer_cntx.move_offset_x);
		offset_y -= (offset_y%g_imageviewer_cntx.move_offset_y);

		
		if(((g_imageviewer_cntx.display_current_x == g_imageviewer_cntx.display_roi_x)&&(offset_x<0))||((g_imageviewer_cntx.display_current_x == g_imageviewer_cntx.display_roi_x+offset_width)&&(offset_x>0)))
			offset_x = 0;

		if(((g_imageviewer_cntx.display_current_y == g_imageviewer_cntx.display_roi_y)&&(offset_y<0)) ||
			((g_imageviewer_cntx.display_current_y == g_imageviewer_cntx.display_roi_y+g_imageviewer_cntx.resized_img_height-g_imageviewer_cntx.display_wnd_height)&&(offset_y>0)))
			offset_y = 0;
	
		if(offset_x == 0 && offset_y == 0)
			return TRUE;

		g_imageviewer_cntx.display_current_x += offset_x;
		g_imageviewer_cntx.display_current_y += offset_y;

		if(g_imageviewer_cntx.display_current_x < g_imageviewer_cntx.display_roi_x)
		{
			offset_x -= (g_imageviewer_cntx.display_current_x - g_imageviewer_cntx.display_roi_x);
			g_imageviewer_cntx.display_current_x = g_imageviewer_cntx.display_roi_x;
		}

		if(g_imageviewer_cntx.display_current_y < g_imageviewer_cntx.display_roi_y)
		{
			offset_y -= (g_imageviewer_cntx.display_current_y-g_imageviewer_cntx.display_roi_y);
			g_imageviewer_cntx.display_current_y = g_imageviewer_cntx.display_roi_y;		
		}

		if(g_imageviewer_cntx.display_current_y > g_imageviewer_cntx.display_roi_y+g_imageviewer_cntx.resized_img_height-g_imageviewer_cntx.display_wnd_height +1)
		{
			offset_y -=  (g_imageviewer_cntx.display_current_y -(g_imageviewer_cntx.display_roi_y+g_imageviewer_cntx.resized_img_height-g_imageviewer_cntx.display_wnd_height +1));
			g_imageviewer_cntx.display_current_y = g_imageviewer_cntx.display_roi_y
															+g_imageviewer_cntx.resized_img_height
															-g_imageviewer_cntx.display_wnd_height +1;
		}

		if(g_imageviewer_cntx.display_current_x >= g_imageviewer_cntx.display_roi_x+offset_width)
		{
			offset_x -= (g_imageviewer_cntx.display_current_x - (g_imageviewer_cntx.display_roi_x+offset_width));
			g_imageviewer_cntx.display_current_x = g_imageviewer_cntx.display_roi_x+offset_width;
		}


		
		mmi_imageviewer_translate_to_xy(offset_x,offset_y);

		return TRUE;
	}
	else
		return FALSE;
}
#endif

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_refresh_image
* DESCRIPTION
*  refresh viewing image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_refresh_image()
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	
	PU8 src;
	PU8 dest;
	U16 src_width,src_height,src_pitch;
	S32 src_offset_x,src_offset_y;
	S32 dest_pitch, dest_offset_x, dest_offset_y	;
	gdi_rect_struct dest_clip;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   /* draw to viewport */

   if(g_imageviewer_cntx.is_gif_file != TRUE)
   {
		src_pitch = g_imageviewer_cntx.display_layer_width;
		dest_pitch = g_imageviewer_cntx.display_wnd_width;
		dest_clip.x1 = 0;
		dest_clip.y1 = 0;
		dest_clip.x2 = g_imageviewer_cntx.display_wnd_width-1;
		dest_clip.y2 = g_imageviewer_cntx.display_wnd_height-1;
		src_offset_x = ((S32)(g_imageviewer_cntx.display_layer_width - g_imageviewer_cntx.display_wnd_width)>>1);
		src_offset_y = ((S32)(g_imageviewer_cntx.display_layer_height - g_imageviewer_cntx.display_wnd_height)>>1);
		src_width =  g_imageviewer_cntx.display_wnd_width ;
		src_height = g_imageviewer_cntx.display_wnd_height;
		dest_offset_y = 0;
		dest_offset_x = 0;

		src =  g_imageviewer_cntx.display_buffer_ptr;
		gdi_layer_push_and_set_active(g_imageviewer_cntx.viewport_layer_handle);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_layer_get_buffer_ptr(&dest);

		gdi_2d_memory_blt(src, src_pitch, src_offset_x, src_offset_y, src_width, src_height,
							   dest, dest_pitch, dest_offset_x, dest_offset_y,dest_clip);

		gdi_layer_pop_and_restore_active();
   }	
		if(g_imageviewer_cntx.screen_mode == VERTICAL_NORMAL || g_imageviewer_cntx.screen_mode == HORIZONTAL_NORMAL)
		{
			/* draw to thumbnail */
			gdi_layer_push_and_set_active(g_imageviewer_cntx.thumbnail_layer_handle);
			gdi_layer_clear(GDI_COLOR_BLACK);
			gdi_layer_get_buffer_ptr(&dest);
			src = g_imageviewer_cntx.cache_buffer_ptr;
			dest_clip.x1 = 0;
			dest_clip.y1 = 0;
			dest_clip.x2 = g_imageviewer_cntx.thumbnail_layer_width-1;
			dest_clip.y2 = g_imageviewer_cntx.thumbnail_layer_height-1;
			gdi_2d_memory_blt(src, g_imageviewer_cntx.cache_layer_width,0,0, g_imageviewer_cntx.thumbnail_layer_width,g_imageviewer_cntx.thumbnail_layer_height, dest,g_imageviewer_cntx.thumbnail_layer_width,0,0,dest_clip );

			/* draw the redbox */
			gdi_draw_rect(g_imageviewer_cntx.redbox_wnd_x+g_imageviewer_cntx.thumbnail_wnd_x,g_imageviewer_cntx.redbox_wnd_y+g_imageviewer_cntx.thumbnail_wnd_y,g_imageviewer_cntx.redbox_wnd_x+g_imageviewer_cntx.thumbnail_wnd_x+g_imageviewer_cntx.redbox_wnd_width-1,g_imageviewer_cntx.redbox_wnd_y+g_imageviewer_cntx.thumbnail_wnd_y+g_imageviewer_cntx.redbox_wnd_height-1,GDI_COLOR_RED);
			gdi_layer_pop_and_restore_active();
		}

		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);		

}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_translate_to_xy
* DESCRIPTION
*  translate image for offset (x,y)
* PARAMETERS
*  x IN
*  y IN
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_translate_to_xy(S32 offset_x, S32 offset_y)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	
	S16 index = 1;
	S16 scase = 0;
	PU8 src,src2;
	PU8 dest;
	U16 src_width,src_height,output_clip_x1,output_clip_x2,output_clip_y1,output_clip_y2;
	U16 src_width2,src_height2;
	S32 src_offset_x,src_offset_y,src_pitch,src_pitch2;
	S32 dest_pitch, dest_offset_x, dest_offset_y	;
	gdi_rect_struct dest_clip;
	S32 dest_width,dest_height,wnd_width,wnd_height;
	float tmp_ratio ;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   g_imageviewer_cntx.is_reextract = FALSE;

	/* calculate the real wnd_width and wnd_height */
	if(g_imageviewer_cntx.resized_img_height < g_imageviewer_cntx.display_wnd_height)
		wnd_height = g_imageviewer_cntx.resized_img_height ;
	else 
		wnd_height = g_imageviewer_cntx.display_wnd_height;
   
	if(g_imageviewer_cntx.resized_img_width < g_imageviewer_cntx.display_wnd_width)
		wnd_width = g_imageviewer_cntx.resized_img_width ;
	else 
		wnd_width = g_imageviewer_cntx.display_wnd_width;

	/* check if need use the cache layer */
	if(g_imageviewer_cntx.display_current_x < g_imageviewer_cntx.bounding_box_x1)
		scase += index;
	index = index<<1;
	
	if(g_imageviewer_cntx.display_current_y < g_imageviewer_cntx.bounding_box_y1)
		scase += index;
	index = index<<1;
	
	if(g_imageviewer_cntx.display_current_x  + wnd_width -1>  g_imageviewer_cntx.bounding_box_x2)
		scase += index;
	index = index<<1;
		
	if(g_imageviewer_cntx.display_current_y  + wnd_height -1> g_imageviewer_cntx.bounding_box_y2)
		scase += index;

	// calculate viewport_wnd_x and viewport_wnd_y
	tmp_ratio = (float)g_imageviewer_cntx.viewport_wnd_width/wnd_width;
	g_imageviewer_cntx.viewport_wnd_x = g_imageviewer_cntx.viewport_wnd_x + (offset_x) * tmp_ratio + 0.5;
	tmp_ratio = (float)g_imageviewer_cntx.viewport_wnd_height/wnd_height;
	g_imageviewer_cntx.viewport_wnd_y = g_imageviewer_cntx.viewport_wnd_y + (offset_y) * tmp_ratio + 0.5;

	if (g_imageviewer_cntx.viewport_wnd_x+g_imageviewer_cntx.viewport_wnd_width > g_imageviewer_cntx.image_width)
		g_imageviewer_cntx.viewport_wnd_x = g_imageviewer_cntx.image_width - g_imageviewer_cntx.viewport_wnd_width;

	if (g_imageviewer_cntx.viewport_wnd_y+g_imageviewer_cntx.viewport_wnd_height > g_imageviewer_cntx.image_height)
		g_imageviewer_cntx.viewport_wnd_y = g_imageviewer_cntx.image_height - g_imageviewer_cntx.viewport_wnd_height;
		
	// calculate redbox_wnd_x and redbox_wnd_y
	tmp_ratio = (float)g_imageviewer_cntx.redbox_wnd_width/g_imageviewer_cntx.viewport_wnd_width;
	g_imageviewer_cntx.redbox_wnd_x = g_imageviewer_cntx.viewport_wnd_x * tmp_ratio+0.5;
	tmp_ratio = (float)g_imageviewer_cntx.redbox_wnd_height/g_imageviewer_cntx.viewport_wnd_height;
	g_imageviewer_cntx.redbox_wnd_y = g_imageviewer_cntx.viewport_wnd_y * tmp_ratio+0.5;

	if (g_imageviewer_cntx.resized_img_width <= g_imageviewer_cntx.display_wnd_width)
	{
		g_imageviewer_cntx.redbox_wnd_x = 0;
	}
	else 
	{
		if ((g_imageviewer_cntx.viewport_wnd_x+g_imageviewer_cntx.viewport_wnd_width >= g_imageviewer_cntx.image_width)
			||(g_imageviewer_cntx.redbox_wnd_x+g_imageviewer_cntx.redbox_wnd_width >= g_imageviewer_cntx.thumbnail_wnd_width))
			g_imageviewer_cntx.redbox_wnd_x = g_imageviewer_cntx.thumbnail_wnd_width - g_imageviewer_cntx.redbox_wnd_width ;
		
		if(g_imageviewer_cntx.redbox_wnd_x < 0)
			g_imageviewer_cntx.redbox_wnd_x =0;
	}

	if (g_imageviewer_cntx.resized_img_height <= g_imageviewer_cntx.display_wnd_height)
	{
		g_imageviewer_cntx.redbox_wnd_y = 0;
	}
	else
	{
		if ((g_imageviewer_cntx.viewport_wnd_y+g_imageviewer_cntx.viewport_wnd_height >= g_imageviewer_cntx.image_height)
			||(g_imageviewer_cntx.redbox_wnd_y+g_imageviewer_cntx.redbox_wnd_height >= g_imageviewer_cntx.thumbnail_wnd_height))
			g_imageviewer_cntx.redbox_wnd_y = g_imageviewer_cntx.thumbnail_wnd_height - g_imageviewer_cntx.redbox_wnd_height;

		if(g_imageviewer_cntx.redbox_wnd_y < 0)
			g_imageviewer_cntx.redbox_wnd_y =0;
		
	}


	if(g_imageviewer_cntx.screen_mode == VERTICAL_NORMAL || g_imageviewer_cntx.screen_mode == HORIZONTAL_NORMAL)
	{			
		/* draw to thumbnail */
		gdi_layer_push_and_set_active(g_imageviewer_cntx.thumbnail_layer_handle);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_layer_get_buffer_ptr(&dest);
		src = g_imageviewer_cntx.cache_buffer_ptr;
		dest_clip.x1 = 0;
		dest_clip.y1 = 0;
		dest_clip.x2 = g_imageviewer_cntx.thumbnail_layer_width-1;
		dest_clip.y2 = g_imageviewer_cntx.thumbnail_layer_height-1;
		gdi_2d_memory_blt(src, g_imageviewer_cntx.cache_layer_width,0,0, g_imageviewer_cntx.thumbnail_layer_width,g_imageviewer_cntx.thumbnail_layer_height, dest,g_imageviewer_cntx.thumbnail_layer_width,0,0,dest_clip );
		gdi_layer_pop_and_restore_active();
	}	
	
	if(scase!=0)
	{
		/* copy the area outsides the display layer from cache layer */
		g_imageviewer_cntx.is_reextract = TRUE;
		gdi_layer_push_and_set_active(g_imageviewer_cntx.cache_layer_handle);
		gdi_layer_get_buffer_ptr(&src2);
		gdi_layer_get_dimension(&src_pitch2,&dest_height);  // dest_height is useless.
		gdi_layer_pop_and_restore_active();
	
		src_offset_x = g_imageviewer_cntx.redbox_wnd_x+g_imageviewer_cntx.thumbnail_wnd_x;
		src_offset_y = g_imageviewer_cntx.redbox_wnd_y+g_imageviewer_cntx.thumbnail_wnd_y;
		src_width2 = g_imageviewer_cntx.redbox_wnd_width;
		src_height2 =g_imageviewer_cntx.redbox_wnd_height; 

		src2 += ((src_offset_y*src_pitch2*GDI_LAYER.bits_per_pixel)>>3);
		src2 += ((src_offset_x*GDI_LAYER.bits_per_pixel)>>3);

		gdi_layer_push_and_set_active(g_imageviewer_cntx.viewport_layer_handle);
		gdi_layer_get_buffer_ptr(&dest);

		gdi_layer_get_dimension(&dest_width,&dest_height);

		gdi_layer_pop_and_restore_active();
		output_clip_x1=0;
		output_clip_y1=0;
		output_clip_x2=dest_width-1;
		output_clip_y2=dest_height-1;
		
		/* 2d memory copy the correspondent area to viewport layer*/
		src_pitch = g_imageviewer_cntx.display_layer_width;
		dest_pitch = g_imageviewer_cntx.display_wnd_width;
		dest_clip.x1 = 0;
		dest_clip.y1 = 0;
		dest_clip.x2 = g_imageviewer_cntx.display_wnd_width-1;
		dest_clip.y2 = g_imageviewer_cntx.display_wnd_height-1;
		switch(scase)
		{
			case 1:
				src_offset_x = 0;
				src_offset_y = g_imageviewer_cntx.display_current_y;
				src_width = g_imageviewer_cntx.display_wnd_width + g_imageviewer_cntx.display_current_x;
				src_height = g_imageviewer_cntx.display_wnd_height;

				dest_offset_x = -g_imageviewer_cntx.display_current_x;
				dest_offset_y = 0;
				break;
			case 2:
				src_offset_x = g_imageviewer_cntx.display_current_x;
				src_offset_y = 0;
				src_width = g_imageviewer_cntx.display_wnd_width;
				src_height = g_imageviewer_cntx.display_wnd_height + g_imageviewer_cntx.display_current_y;

				dest_offset_x = 0; 
				dest_offset_y = -g_imageviewer_cntx.display_current_y;
				break;
			case 3:
				src_offset_x = 0;
				src_offset_y = 0;
				src_width = g_imageviewer_cntx.display_wnd_width + g_imageviewer_cntx.display_current_x;
				src_height = g_imageviewer_cntx.display_wnd_height + g_imageviewer_cntx.display_current_y;
				
				dest_offset_x = -g_imageviewer_cntx.display_current_x;
				dest_offset_y = -g_imageviewer_cntx.display_current_y;
				break;
			case 4:
				src_offset_x = g_imageviewer_cntx.display_current_x;
				src_offset_y = g_imageviewer_cntx.display_current_y;
				src_width =  g_imageviewer_cntx.display_layer_width - g_imageviewer_cntx.display_current_x;
				src_height = g_imageviewer_cntx.display_wnd_height;

				dest_offset_x = 0;
				dest_offset_y = 0;
				break;
			case 6:
				src_offset_x = g_imageviewer_cntx.display_current_x;
				src_offset_y = 0;
				src_width =  g_imageviewer_cntx.display_layer_width - g_imageviewer_cntx.display_current_x;
				src_height = g_imageviewer_cntx.display_wnd_height + g_imageviewer_cntx.display_current_y;
	
				dest_offset_x = 0; 
				dest_offset_y = -g_imageviewer_cntx.display_current_y;
				break;
			case 8:
				src_offset_x = g_imageviewer_cntx.display_current_x;
				src_offset_y = g_imageviewer_cntx.display_current_y;
				src_width =  g_imageviewer_cntx.display_wnd_width;
				src_height = g_imageviewer_cntx.display_layer_height - g_imageviewer_cntx.display_current_y;
		
				dest_offset_x = 0;
				dest_offset_y = 0;
				break;
			case 9:
				src_offset_x = 0;
				src_offset_y = g_imageviewer_cntx.display_current_y;
				src_width = g_imageviewer_cntx.display_wnd_width + g_imageviewer_cntx.display_current_x;
				src_height = g_imageviewer_cntx.display_layer_height - g_imageviewer_cntx.display_current_y;
				
				dest_offset_x = -g_imageviewer_cntx.display_current_x;
				dest_offset_y = 0;	
				break;
			case 12:
				src_offset_x = g_imageviewer_cntx.display_current_x;
				src_offset_y = g_imageviewer_cntx.display_current_y;
				src_width =  g_imageviewer_cntx.display_layer_width - g_imageviewer_cntx.display_current_x;
				src_height = g_imageviewer_cntx.display_layer_height - g_imageviewer_cntx.display_current_y;

				dest_offset_x = 0;
				dest_offset_y = 0;
				break;
			default:
	
				break;
		}
#ifdef MMI_ON_HARDWARE_P
		gdi_resizer(src2, src_width2, src_height2, src_pitch2, 
						dest, dest_width, dest_height, 
						output_clip_x1, output_clip_y1, output_clip_x2, output_clip_y2);
#endif
	}
	else
	{
		/* red box is still inside the display buffer */
		/* use 2d memory refresh the viewport layer */
		src_pitch = g_imageviewer_cntx.display_layer_width;
		dest_pitch = g_imageviewer_cntx.display_wnd_width;
		dest_clip.x1 = 0;
		dest_clip.y1 = 0;
		dest_clip.x2 = g_imageviewer_cntx.display_wnd_width-1;
		dest_clip.y2 = g_imageviewer_cntx.display_wnd_height-1;
		src_offset_x = g_imageviewer_cntx.display_current_x;
		src_offset_y = g_imageviewer_cntx.display_current_y;
		src_width =  g_imageviewer_cntx.display_wnd_width ;
		src_height = g_imageviewer_cntx.display_wnd_height;
		dest_offset_x = 0;
		dest_offset_y = 0;
	}

	if(g_imageviewer_cntx.resized_img_width < g_imageviewer_cntx.display_wnd_width)
	{
		dest_offset_x += ((g_imageviewer_cntx.display_wnd_width-g_imageviewer_cntx.resized_img_width)>>1);
	}

	if(g_imageviewer_cntx.resized_img_height < g_imageviewer_cntx.display_wnd_height)
	{
		dest_offset_y += ((g_imageviewer_cntx.display_wnd_height-g_imageviewer_cntx.resized_img_height)>>1);
	}
	
	/* copy from display_buffer to viewport layer */
//	if(abs(src_offset_x) < g_imageviewer_cntx.display_wnd_width &&  abs(src_offset_y) < g_imageviewer_cntx.display_wnd_height)
//	{
//		src = g_imageviewer_cntx.display_buffer_ptr;
//		gdi_layer_push_and_set_active(g_imageviewer_cntx.viewport_layer_handle);
//		gdi_layer_get_buffer_ptr(&dest);
//		gdi_layer_pop_and_restore_active();
//		gdi_2d_memory_blt(src, src_pitch, src_offset_x, src_offset_y, src_width, src_height,
//						   dest, dest_pitch, dest_offset_x, dest_offset_y,dest_clip);
//	}
	
	if(g_imageviewer_cntx.screen_mode == VERTICAL_NORMAL || g_imageviewer_cntx.screen_mode == HORIZONTAL_NORMAL)
	{
		/* draw the redbox */		
		gdi_layer_push_and_set_active(g_imageviewer_cntx.thumbnail_layer_handle);
		gdi_draw_rect(g_imageviewer_cntx.redbox_wnd_x+g_imageviewer_cntx.thumbnail_wnd_x,g_imageviewer_cntx.redbox_wnd_y+g_imageviewer_cntx.thumbnail_wnd_y,g_imageviewer_cntx.redbox_wnd_x+g_imageviewer_cntx.redbox_wnd_width+g_imageviewer_cntx.thumbnail_wnd_x-1,g_imageviewer_cntx.redbox_wnd_y+g_imageviewer_cntx.thumbnail_wnd_y+g_imageviewer_cntx.redbox_wnd_height-1,GDI_COLOR_RED);
		gdi_layer_pop_and_restore_active();
	}
	gdi_layer_blt_previous(0,0,UI_device_width-1,UI_device_height-1);
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_zoomout
* DESCRIPTION
*  zoomout image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_zoomout()
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
//	float tmp_ratio;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(g_imageviewer_cntx.zoom_factor_cnt==0)
		return;
	
	g_imageviewer_cntx.is_zoomin_able = TRUE;

	g_imageviewer_cntx.zoom_factor_cnt--;	

	if(g_imageviewer_cntx.is_trigger_by_keypad)
	{
	if(g_imageviewer_cntx.is_zoomout_pressed)
		coolsand_UI_start_timer(100, mmi_imageviewer_zoomout);
	else	/* first time press up arrow, use longer time */
		coolsand_UI_start_timer(300, mmi_imageviewer_zoomout);
	}

	if(g_imageviewer_cntx.zoom_factor_cnt==0)
	{
		mmi_imageviewer_init_image_size();

		/* init thumbnail image information */
		g_imageviewer_cntx.redbox_wnd_x = 0;
		g_imageviewer_cntx.redbox_wnd_y = 0;
		g_imageviewer_cntx.redbox_wnd_width = g_imageviewer_cntx.thumbnail_wnd_width;
		g_imageviewer_cntx.redbox_wnd_height = g_imageviewer_cntx.thumbnail_wnd_height;
	}
	else 
	{
		g_imageviewer_cntx.viewport_wnd_x += (g_imageviewer_cntx.viewport_wnd_width/2 + 0.5);
		g_imageviewer_cntx.viewport_wnd_y += (g_imageviewer_cntx.viewport_wnd_height/2 + 0.5);

		g_imageviewer_cntx.viewport_wnd_width = g_imageviewer_cntx.viewport_wnd_width * g_imageviewer_cntx.zoom_factor+0.5;
		g_imageviewer_cntx.viewport_wnd_height = g_imageviewer_cntx.viewport_wnd_height * g_imageviewer_cntx.zoom_factor+0.5;

		g_imageviewer_cntx.viewport_wnd_x -= (g_imageviewer_cntx.viewport_wnd_width/2 + 0.5);
		g_imageviewer_cntx.viewport_wnd_y -= (g_imageviewer_cntx.viewport_wnd_height/2 + 0.5);



		/* resize images */	
		g_imageviewer_cntx.resized_img_width = g_imageviewer_cntx.resized_img_width/ g_imageviewer_cntx.zoom_factor + 0.5;
		g_imageviewer_cntx.resized_img_height = g_imageviewer_cntx.resized_img_height/ g_imageviewer_cntx.zoom_factor + 0.5;


		g_imageviewer_cntx.redbox_wnd_x = g_imageviewer_cntx.redbox_wnd_x + (g_imageviewer_cntx.redbox_wnd_width/2) + 0.5;
		g_imageviewer_cntx.redbox_wnd_y = g_imageviewer_cntx.redbox_wnd_y + (g_imageviewer_cntx.redbox_wnd_height/2) + 0.5;
	
		g_imageviewer_cntx.redbox_wnd_width = g_imageviewer_cntx.redbox_wnd_width * g_imageviewer_cntx.zoom_factor +0.5;
		g_imageviewer_cntx.redbox_wnd_height =		g_imageviewer_cntx.redbox_wnd_height * g_imageviewer_cntx.zoom_factor+0.5;
	
		g_imageviewer_cntx.redbox_wnd_x = g_imageviewer_cntx.redbox_wnd_x - (g_imageviewer_cntx.redbox_wnd_width/2) + 0.5;
		g_imageviewer_cntx.redbox_wnd_y = g_imageviewer_cntx.redbox_wnd_y - (g_imageviewer_cntx.redbox_wnd_height/2) + 0.5;

		if(g_imageviewer_cntx.redbox_wnd_x < 0)
			g_imageviewer_cntx.redbox_wnd_x = 0;

		if(g_imageviewer_cntx.redbox_wnd_y < 0)
			g_imageviewer_cntx.redbox_wnd_y = 0;

		if(g_imageviewer_cntx.redbox_wnd_x + g_imageviewer_cntx.redbox_wnd_width > g_imageviewer_cntx.thumbnail_wnd_width)
			g_imageviewer_cntx.redbox_wnd_x = g_imageviewer_cntx.thumbnail_wnd_width - g_imageviewer_cntx.redbox_wnd_width;

		if(g_imageviewer_cntx.redbox_wnd_y + g_imageviewer_cntx.redbox_wnd_height > g_imageviewer_cntx.thumbnail_wnd_height)
			g_imageviewer_cntx.redbox_wnd_y = g_imageviewer_cntx.thumbnail_wnd_height - g_imageviewer_cntx.redbox_wnd_height;


		if(g_imageviewer_cntx.viewport_wnd_x < 0 )
			g_imageviewer_cntx.viewport_wnd_x  = 0;

		if(g_imageviewer_cntx.viewport_wnd_y < 0)
			g_imageviewer_cntx.viewport_wnd_y = 0;

		if(g_imageviewer_cntx.viewport_wnd_x + g_imageviewer_cntx.viewport_wnd_width > g_imageviewer_cntx.image_width)
			g_imageviewer_cntx.viewport_wnd_x  = g_imageviewer_cntx.image_width - g_imageviewer_cntx.viewport_wnd_width;

		if(g_imageviewer_cntx.viewport_wnd_y + g_imageviewer_cntx.viewport_wnd_height > g_imageviewer_cntx.image_height)
			g_imageviewer_cntx.viewport_wnd_y  = g_imageviewer_cntx.image_height - g_imageviewer_cntx.viewport_wnd_height;

	}	
	/* cal the roi */
	mmi_imageviewer_update_roi();

	/* extract image to display layer */	
	mmi_imageviewer_extract_image_to_display_layer();

	/* refresh image */
	mmi_imageviewer_refresh_image();

	g_imageviewer_cntx.zoom_factor  = 1.414;	
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_zoomin
* DESCRIPTION
*  zoomin image
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_zoomin()
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	float tmp_ratio;
	S32 prev_resized_img_width;
	S32 prev_resized_img_height;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if((g_imageviewer_cntx.is_gif_file == TRUE)||(g_imageviewer_cntx.is_zoomin_able == FALSE)
		||((g_imageviewer_cntx.zoom_factor_cnt+1)==IMAGEVIEWER_NUM_ZOOM_FACTOR)||(g_imageviewer_cntx.image_width < g_imageviewer_cntx.display_wnd_width))
		return;

	g_imageviewer_cntx.zoom_factor_cnt++;	


	/* while it is first-time zoomin, if the image display on screen currently is small than lcd size,
	   zoom to fit screen size, else zoom factor = 1.414 */
	if(g_imageviewer_cntx.zoom_factor_cnt==1)
	{
		tmp_ratio = (float)g_imageviewer_cntx.display_wnd_height/g_imageviewer_cntx.resized_img_height;
		if(g_imageviewer_cntx.zoom_factor <  tmp_ratio)
			g_imageviewer_cntx.zoom_factor =  tmp_ratio;

		tmp_ratio = (float)g_imageviewer_cntx.display_wnd_width/g_imageviewer_cntx.resized_img_width;
		if(g_imageviewer_cntx.zoom_factor <  tmp_ratio)
			g_imageviewer_cntx.zoom_factor =  tmp_ratio;
	}
	else
		g_imageviewer_cntx.zoom_factor  = 1.414;	

	/* if zoom in is trigger by keypad, start timer to zoom repeatly */
	if(g_imageviewer_cntx.is_trigger_by_keypad )
	{
		if(g_imageviewer_cntx.is_zoomin_pressed)
			coolsand_UI_start_timer(100, mmi_imageviewer_zoomin);
		else	/* first time press up arrow, use longer time */
			coolsand_UI_start_timer(300, mmi_imageviewer_zoomin);
	}

	if(g_imageviewer_cntx.screen_mode == VERTICAL_FULLSCREEN)
	{
		mmi_imageviewer_fix_wnd();
	}

	g_imageviewer_cntx.redbox_wnd_x = g_imageviewer_cntx.redbox_wnd_x + (g_imageviewer_cntx.redbox_wnd_width/2)+ 0.5;
	g_imageviewer_cntx.redbox_wnd_y = g_imageviewer_cntx.redbox_wnd_y + (g_imageviewer_cntx.redbox_wnd_height/2)+ 0.5;

	g_imageviewer_cntx.redbox_wnd_width = g_imageviewer_cntx.redbox_wnd_width / g_imageviewer_cntx.zoom_factor+0.5;
	g_imageviewer_cntx.redbox_wnd_height = g_imageviewer_cntx.redbox_wnd_height / g_imageviewer_cntx.zoom_factor+0.5;

	g_imageviewer_cntx.redbox_wnd_x = g_imageviewer_cntx.redbox_wnd_x - (g_imageviewer_cntx.redbox_wnd_width/2)+ 0.5;
	g_imageviewer_cntx.redbox_wnd_y = g_imageviewer_cntx.redbox_wnd_y - (g_imageviewer_cntx.redbox_wnd_height/2)+ 0.5;


	g_imageviewer_cntx.viewport_wnd_x += (g_imageviewer_cntx.viewport_wnd_width/2 + 0.5);
	g_imageviewer_cntx.viewport_wnd_y += (g_imageviewer_cntx.viewport_wnd_height/2 + 0.5);

	g_imageviewer_cntx.viewport_wnd_width = g_imageviewer_cntx.viewport_wnd_width / g_imageviewer_cntx.zoom_factor+0.5;
	g_imageviewer_cntx.viewport_wnd_height = g_imageviewer_cntx.viewport_wnd_height / g_imageviewer_cntx.zoom_factor+0.5;

	g_imageviewer_cntx.viewport_wnd_x -= (g_imageviewer_cntx.viewport_wnd_width/2 + 0.5);
	g_imageviewer_cntx.viewport_wnd_y -= (g_imageviewer_cntx.viewport_wnd_height/2 + 0.5);



	/* resize images */	
	prev_resized_img_width = g_imageviewer_cntx.resized_img_width;
	prev_resized_img_height = g_imageviewer_cntx.resized_img_height;
	
	g_imageviewer_cntx.resized_img_width = 	g_imageviewer_cntx.resized_img_width * g_imageviewer_cntx.zoom_factor + 0.5;
	g_imageviewer_cntx.resized_img_height = 	g_imageviewer_cntx.resized_img_height * g_imageviewer_cntx.zoom_factor + 0.5;


	if(g_imageviewer_cntx.resized_img_width > g_imageviewer_cntx.image_width)
	{
		g_imageviewer_cntx.zoom_factor  = (g_imageviewer_cntx.zoom_factor*g_imageviewer_cntx.image_width/g_imageviewer_cntx.resized_img_width);
		g_imageviewer_cntx.resized_img_width = g_imageviewer_cntx.image_width;
		g_imageviewer_cntx.resized_img_height = g_imageviewer_cntx.image_height;
		g_imageviewer_cntx.viewport_wnd_width  = 	g_imageviewer_cntx.display_wnd_width; 			
		g_imageviewer_cntx.viewport_wnd_height =	g_imageviewer_cntx.display_wnd_height;	
		g_imageviewer_cntx.is_zoomin_able = FALSE;
	}


	if(g_imageviewer_cntx.zoom_factor_cnt==1)
	{
		/* adjust the viewport width/height ratio to fit the display_layer's */
		if((prev_resized_img_width < g_imageviewer_cntx.display_wnd_width) && (g_imageviewer_cntx.resized_img_width <= g_imageviewer_cntx.display_wnd_width))
		{
			g_imageviewer_cntx.viewport_wnd_width  = g_imageviewer_cntx.image_width;
			g_imageviewer_cntx.redbox_wnd_width = g_imageviewer_cntx.thumbnail_wnd_width;
			g_imageviewer_cntx.viewport_wnd_x = 0;
		}
	
		if((prev_resized_img_height < g_imageviewer_cntx.display_wnd_height) && (g_imageviewer_cntx.resized_img_height <= g_imageviewer_cntx.display_wnd_height))
		{
			g_imageviewer_cntx.viewport_wnd_height  = g_imageviewer_cntx.image_height;
			g_imageviewer_cntx.redbox_wnd_height = g_imageviewer_cntx.thumbnail_wnd_height;
			g_imageviewer_cntx.viewport_wnd_y = 0;
		}
	}

	if (g_imageviewer_cntx.resized_img_width <= g_imageviewer_cntx.display_wnd_width)
	{
		g_imageviewer_cntx.redbox_wnd_x = 0;
		g_imageviewer_cntx.redbox_wnd_width = g_imageviewer_cntx.thumbnail_wnd_width;
	}
	if (g_imageviewer_cntx.resized_img_height <= g_imageviewer_cntx.display_wnd_height)
	{
		g_imageviewer_cntx.redbox_wnd_y = 0;
		g_imageviewer_cntx.redbox_wnd_height = g_imageviewer_cntx.thumbnail_wnd_height;
	}

	
	if(g_imageviewer_cntx.resized_img_width*1.414<g_imageviewer_cntx.image_width)
		g_imageviewer_cntx.is_zoomin_able = TRUE;
	else
		g_imageviewer_cntx.is_zoomin_able = FALSE;
	/* cal the roi */
	mmi_imageviewer_update_roi();


	/* extract image to display layer */	
	mmi_imageviewer_extract_image_to_display_layer();

	/* refresh image : 2d copy from display layer to ooxx layer */
	mmi_imageviewer_refresh_image();
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_view_the_latest
* DESCRIPTION
*  view the latest image directly
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_view_the_latest(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8				buf_filepath[FMGR_PATH_BUFFER_SIZE];
	FS_HANDLE	file_handle;
	FMGR_FILTER	filter;
	S32			create_result;
	U16 			file_count;

	mmi_trace(g_sw_CAMERA, "Excute func : mmi_imageviewer_view_the_latest()");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* load camera's storage setting */
	mmi_camera_load_setting();
	mmi_camera_get_storage_file_path(buf_filepath);
	create_result = mmi_camera_create_file_dir(buf_filepath);
    
	/* check if it is valid path */
	file_handle	= MMI_FS_Open((PU16)buf_filepath, FS_OPEN_DIR | FS_READ_ONLY);

	if(file_handle >= 0)
	{
		FS_Close(file_handle);

		/* check if device is busy or not */
		if (MMI_FS_GetDevStatus(buf_filepath[0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY)
		{
			/*  mmi_fmgr_select_path_and_enter will also add screen to histry */
			DeleteUptoScrID(SCR_ID_IMGVIEW_APP);
		
			/* Delete up to previous screen */
			DeleteNHistory(1);

			return;
		}
		
	}	
	else
	{
		if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_PHONE)
		{
			/* storage is phone */
			
			/* if storage is already on phone, but it is still failed, means phone's FAT is corrupted */
			if(create_result == FS_DISK_FULL)
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else if(create_result == FS_ROOT_DIR_FULL)
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_FAT_ALLOC_ERROR_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
	
			DeleteNHistory(1);
			return;
		}
		else
		{
			/* storage is card */
			
			/* photo's path is not exist, and card is write protected, cant create photo folder */
			if(create_result == FS_WRITE_PROTECTION)
				mmi_imageviewer_display_popup((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_WRITE_PROTECTION), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else if(create_result == FS_DISK_FULL)
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else if(create_result == FS_ROOT_DIR_FULL)
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else
				mmi_imageviewer_display_popup((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		
			g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
			mmi_camera_store_setting();
	
			DeleteUptoScrID(SCR_ID_IMGVIEW_APP);
			return;

		}

	}

	FMGR_FILTER_INIT(&filter);
	FMGR_FILTER_SET_IMAGE(&filter);

	/* init and sort */
//	fmgr_get_sort_option(&g_imageviewer_cntx.sort_option);
//	fmgr_set_sort_option (FS_SORT_TIME);

	mmi_fmgr_kernel_init(buf_filepath,filter,&file_count);

	g_imageviewer_cntx.imgview_rotate = g_camera_cntx.osd_rotate;

	if(file_count <= 0)
	{
		mmi_imageviewer_display_popup((PU8)GetString(STR_ID_FMGR_EMPTY_FOLDER), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		return;
	}	

	switch (g_imageviewer_cntx.imgview_rotate)
	{
		case 0:
			g_imageviewer_cntx.screen_mode = VERTICAL_FULLSCREEN;
		break;
		case 3:
			g_imageviewer_cntx.screen_mode = HORIZONTAL_FULLSCREEN;
		break;					
	}

	mmi_imageviewer_entry_app_screen();
}


/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_create_resource
* DESCRIPTION
*  create layer related resource
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_create_resource()
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* create tile result buffer */
	#ifdef APPMEM_SUPPORT
		/* with ASM */
		g_imageviewer_cntx.display_buffer_ptr = AllocMediaMemory(IMAGEVIEWER_DISPLAY_BUFFER_SIZE); 
		g_imageviewer_cntx.cache_buffer_ptr = AllocMediaMemory(IMAGEVIEWER_CACHE_BUFFER_SIZE);
	#else
		/* no ASM */
		g_imageviewer_cntx.display_buffer_ptr = (PU8)imageviewer_display_layer_buffer;
		g_imageviewer_cntx.cache_buffer_ptr = (PU8)imageviewer_cache_layer_buffer;
	#endif /* APPMEM_SUPPORT */

		if(g_imageviewer_cntx.screen_mode==HORIZONTAL_FULLSCREEN || g_imageviewer_cntx.screen_mode==HORIZONTAL_NORMAL)
		{
			g_imageviewer_cntx.display_layer_width=IMAGEVIEWER_DISPLAY_LAYER_HEIGHT;
			g_imageviewer_cntx.display_layer_height=IMAGEVIEWER_DISPLAY_LAYER_WIDTH;
			g_imageviewer_cntx.cache_layer_width=IMAGEVIEWER_CACHE_LAYER_HEIGHT;
			g_imageviewer_cntx.cache_layer_height=IMAGEVIEWER_CACHE_LAYER_WIDTH;
		}
		else
		{
			g_imageviewer_cntx.display_layer_width=IMAGEVIEWER_DISPLAY_LAYER_WIDTH;
			g_imageviewer_cntx.display_layer_height=IMAGEVIEWER_DISPLAY_LAYER_HEIGHT;
			g_imageviewer_cntx.cache_layer_width=IMAGEVIEWER_CACHE_LAYER_WIDTH;
			g_imageviewer_cntx.cache_layer_height=IMAGEVIEWER_CACHE_LAYER_HEIGHT;

		}


	/* create display layer resource */
	gdi_layer_create_using_outside_memory(	0,
														0,
														g_imageviewer_cntx.display_layer_width,
														g_imageviewer_cntx.display_layer_height,
														&g_imageviewer_cntx.display_layer_handle,
														(PU8)g_imageviewer_cntx.display_buffer_ptr, 
														(S32)IMAGEVIEWER_DISPLAY_BUFFER_SIZE);

	gdi_layer_push_and_set_active(g_imageviewer_cntx.display_layer_handle);
	gdi_layer_clear(GDI_COLOR_BLACK);
	gdi_layer_pop_and_restore_active();
	
	/* create cache layer resource */
	gdi_layer_create_using_outside_memory(	0,
														0,
														g_imageviewer_cntx.cache_layer_width,
														g_imageviewer_cntx.cache_layer_height,
														&g_imageviewer_cntx.cache_layer_handle,
														(PU8)g_imageviewer_cntx.cache_buffer_ptr, 
														(S32)IMAGEVIEWER_CACHE_BUFFER_SIZE);

	gdi_layer_push_and_set_active(g_imageviewer_cntx.cache_layer_handle);
	gdi_layer_clear(GDI_COLOR_BLACK);
	gdi_layer_pop_and_restore_active();		

}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_init_thumbnail
* DESCRIPTION
*  init thumbnail information
* PARAMETERS
*  coordinate IN the thumbnail layer info
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_init_thumbnail(dm_coordinates *coordinate)
{
	float tmp_height;

	/*if the image is vertical, recalculate the width, height of the thumbnail image */
	if(g_imageviewer_cntx.image_width <= g_imageviewer_cntx.image_height)
	{
		g_imageviewer_cntx.thumbnail_wnd_height =coordinate->s16Height;
		tmp_height = (float)g_imageviewer_cntx.image_height/g_imageviewer_cntx.thumbnail_wnd_height; 
		g_imageviewer_cntx.thumbnail_wnd_width = g_imageviewer_cntx.image_width/tmp_height+0.5;
		g_imageviewer_cntx.thumbnail_layer_width = coordinate->s16Width;
		g_imageviewer_cntx.thumbnail_layer_height =coordinate->s16Height;
		g_imageviewer_cntx.thumbnail_wnd_x = (coordinate->s16Width- g_imageviewer_cntx.thumbnail_wnd_width)>>1;
		g_imageviewer_cntx.thumbnail_wnd_y = 0;
	}
	else 
	{
		g_imageviewer_cntx.thumbnail_wnd_width =coordinate->s16Width;
		tmp_height = (float)g_imageviewer_cntx.image_width/g_imageviewer_cntx.thumbnail_wnd_width; 
		g_imageviewer_cntx.thumbnail_wnd_height = g_imageviewer_cntx.image_height/tmp_height+0.5;
		g_imageviewer_cntx.thumbnail_layer_width = coordinate->s16Width;
		g_imageviewer_cntx.thumbnail_layer_height =coordinate->s16Height;
		g_imageviewer_cntx.thumbnail_wnd_x = 0;
		g_imageviewer_cntx.thumbnail_wnd_y = (coordinate->s16Height- g_imageviewer_cntx.thumbnail_wnd_height)>>1;
	}	

	
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_init_image
* DESCRIPTION
*  init image information
* PARAMETERS
*  coordinate IN the image viewing layer info
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_init_image(dm_coordinates *coordinate)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   /* init image informatino */
	if(g_imageviewer_cntx.is_resume == FALSE)
	{
	g_imageviewer_cntx.zoom_factor_cnt = 0;									/* Do Not enlarge */
	g_imageviewer_cntx.image_movable = 0;
	}

	/* for viewing window */
		g_imageviewer_cntx.display_wnd_width = coordinate->s16Width;		/* layer_width from categoryscreen */
		g_imageviewer_cntx.display_wnd_height = coordinate->s16Height;		/* layer_height from categoryscreen */

	g_imageviewer_cntx.display_x = coordinate->s16X;
	g_imageviewer_cntx.display_y = coordinate->s16Y;
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_init_image_size
* DESCRIPTION
*  init the image size
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imageviewer_init_image_size()
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

		g_imageviewer_cntx.zoom_factor  = 1.414;

	if((g_imageviewer_cntx.image_width<g_imageviewer_cntx.display_wnd_width)
			&&(g_imageviewer_cntx.image_height<g_imageviewer_cntx.display_wnd_height)) 
	{
		g_imageviewer_cntx.resized_img_width =  g_imageviewer_cntx.image_width;
		g_imageviewer_cntx.resized_img_height = g_imageviewer_cntx.image_height;
	}
	else
	{
		float tmp_width = (float)g_imageviewer_cntx.image_width/g_imageviewer_cntx.display_wnd_width;
		float tmp_height = (float)g_imageviewer_cntx.image_height/g_imageviewer_cntx.display_wnd_height; 

		if(tmp_width == tmp_height)
		{
			g_imageviewer_cntx.resized_img_width =  g_imageviewer_cntx.display_wnd_width;
			g_imageviewer_cntx.resized_img_height = g_imageviewer_cntx.display_wnd_height;
		}
		else if (tmp_width > tmp_height)
		{
			g_imageviewer_cntx.resized_img_height = g_imageviewer_cntx.image_height/tmp_width+0.5;
			g_imageviewer_cntx.resized_img_width = g_imageviewer_cntx.display_wnd_width;
		}
		else
		{
			g_imageviewer_cntx.resized_img_width = g_imageviewer_cntx.image_width/tmp_height+0.5;
			g_imageviewer_cntx.resized_img_height = g_imageviewer_cntx.display_wnd_height;
		}
	}

		g_imageviewer_cntx.viewport_wnd_width = g_imageviewer_cntx.image_width;
		g_imageviewer_cntx.viewport_wnd_height =g_imageviewer_cntx.image_height; 
	
		if(g_imageviewer_cntx.resized_img_width<g_imageviewer_cntx.image_width && g_imageviewer_cntx.resized_img_height<g_imageviewer_cntx.image_height)
			g_imageviewer_cntx.is_zoomin_able = TRUE;	

}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_update_roi
* DESCRIPTION
*  update the roi information
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_update_roi()
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 offset_x,offset_y;
	float tmp_ratio ;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if (g_imageviewer_cntx.resized_img_width < g_imageviewer_cntx.display_wnd_width)
		offset_x = ((S32)(g_imageviewer_cntx.display_layer_width - g_imageviewer_cntx.resized_img_width)>>1);
	else
		offset_x = ((S32)(g_imageviewer_cntx.display_layer_width - g_imageviewer_cntx.display_wnd_width)>>1);


	if(g_imageviewer_cntx.resized_img_height < g_imageviewer_cntx.display_wnd_height)
		offset_y = ((S32)(g_imageviewer_cntx.display_layer_height - g_imageviewer_cntx.resized_img_height)>>1);
	else 
		offset_y = ((S32)(g_imageviewer_cntx.display_layer_height - g_imageviewer_cntx.display_wnd_height)>>1);

	if (g_imageviewer_cntx.zoom_factor_cnt==0)
	{
		g_imageviewer_cntx.display_roi_x = offset_x;
		g_imageviewer_cntx.display_roi_y = offset_y;	
		g_imageviewer_cntx.display_wnd_x = offset_x;
		g_imageviewer_cntx.display_wnd_y = offset_y;

		g_imageviewer_cntx.viewport_roi_x = 0;
		g_imageviewer_cntx.viewport_roi_y = 0;	
		g_imageviewer_cntx.viewport_wnd_x = 0;
		g_imageviewer_cntx.viewport_wnd_y = 0;	
	}
	else 
	{
		// calculate the position of the display_wnd
		tmp_ratio = (float)g_imageviewer_cntx.resized_img_width/g_imageviewer_cntx.image_width;
		
		g_imageviewer_cntx.display_wnd_x = offset_x;
		g_imageviewer_cntx.display_wnd_y = offset_y;

		g_imageviewer_cntx.viewport_roi_x = g_imageviewer_cntx.viewport_wnd_x 
															-offset_x/tmp_ratio +0.5;
		g_imageviewer_cntx.viewport_roi_y = g_imageviewer_cntx.viewport_wnd_y 
															-offset_y/tmp_ratio +0.5;
                                                      

		if(g_imageviewer_cntx.resized_img_width <= g_imageviewer_cntx.display_wnd_width)
			g_imageviewer_cntx.display_roi_x = offset_x;			
		else
			g_imageviewer_cntx.display_roi_x =	(-g_imageviewer_cntx.viewport_roi_x)*tmp_ratio+0.5;

		if(g_imageviewer_cntx.resized_img_height <= g_imageviewer_cntx.display_wnd_height)
			g_imageviewer_cntx.display_roi_y = offset_y;
		else
			g_imageviewer_cntx.display_roi_y = (-g_imageviewer_cntx.viewport_roi_y)*tmp_ratio+0.5;


		g_imageviewer_cntx.bounding_box_x1 = (g_imageviewer_cntx.display_roi_x>0)?g_imageviewer_cntx.display_roi_x:0;
		g_imageviewer_cntx.bounding_box_y1 = (g_imageviewer_cntx.display_roi_y>0)?g_imageviewer_cntx.display_roi_y:0;

		g_imageviewer_cntx.bounding_box_x2 =  g_imageviewer_cntx.display_roi_x + g_imageviewer_cntx.resized_img_width-1;
		g_imageviewer_cntx.bounding_box_y2 =  g_imageviewer_cntx.display_roi_y + g_imageviewer_cntx.resized_img_height-1;
		
		if(g_imageviewer_cntx.bounding_box_x2 >= g_imageviewer_cntx.display_layer_width)
			g_imageviewer_cntx.bounding_box_x2 = g_imageviewer_cntx.display_layer_width-1; 
                                                      
		if(g_imageviewer_cntx.bounding_box_y2 >= g_imageviewer_cntx.display_layer_height)
			g_imageviewer_cntx.bounding_box_y2 = g_imageviewer_cntx.display_layer_height-1; 		
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_extract_image_to_display_layer
* DESCRIPTION
*  extract image to display layer
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_extract_image_to_display_layer(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 tmp_case,idx ;
   S32 clip_x1,clip_y1,clip_x2,clip_y2;
	GDI_RESULT gdi_handle;
	U16 err_str_ptr;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	gdi_layer_push_and_set_active(g_imageviewer_cntx.display_layer_handle);
	gdi_layer_clear(GDI_COLOR_BLACK);

	g_imageviewer_cntx.display_current_x = g_imageviewer_cntx.display_wnd_x;
	g_imageviewer_cntx.display_current_y = g_imageviewer_cntx.display_wnd_y;
	
	g_imageviewer_cntx.is_gif_file = FALSE;

	if(g_imageviewer_cntx.is_permitted==FALSE)
		g_imageviewer_cntx.image_type =	GDI_IMAGE_TYPE_INVALID;
	switch(g_imageviewer_cntx.image_type)
	{
	case GDI_IMAGE_TYPE_INVALID:
	   gdi_image_draw_resized_id(g_imageviewer_cntx.display_roi_x,
		  								g_imageviewer_cntx.display_roi_y,
  								 		 g_imageviewer_cntx.resized_img_width,
										 g_imageviewer_cntx.resized_img_height,
	   								IMG_ID_IMAGEVIEWER_DRM);

		break;
	case GDI_IMAGE_TYPE_GIF_FILE:
	{
		S32 offset_x = 0;
		S32 offset_y = 0;

		g_imageviewer_cntx.is_gif_file = TRUE;
		
		if(g_imageviewer_cntx.display_wnd_width > g_imageviewer_cntx.resized_img_width)
			offset_x = (g_imageviewer_cntx.display_wnd_width - g_imageviewer_cntx.resized_img_width)>>1;	

		if(g_imageviewer_cntx.display_wnd_height > g_imageviewer_cntx.resized_img_height)
			offset_y = (g_imageviewer_cntx.display_wnd_height - g_imageviewer_cntx.resized_img_height)>>1;
		gdi_layer_set_active(g_imageviewer_cntx.viewport_layer_handle);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_image_draw_animation_file(offset_x,offset_y,g_imageviewer_cntx.filepath,NULL);

		break;
	}
	case GDI_IMAGE_TYPE_JPG:
	case GDI_IMAGE_TYPE_JPG_FILE:

	 
   idx =1;
   tmp_case = 0;
	/* if the image width > layer width */ 
	   if(g_imageviewer_cntx.resized_img_width > g_imageviewer_cntx.display_layer_width)
	tmp_case += idx;

	idx *=2;
	/* if the image height > layer height */ 
	   if(g_imageviewer_cntx.resized_img_height > g_imageviewer_cntx.display_layer_height)
	tmp_case += idx;

	switch(tmp_case)
	{
		case 0:
			clip_x1 = 0;
			clip_y1 = 0;
			clip_x2 = g_imageviewer_cntx.resized_img_width-1;
			clip_y2 = g_imageviewer_cntx.resized_img_height-1;
		   break;
		case 1:
			clip_x1 = -g_imageviewer_cntx.display_roi_x;
			clip_y1 = 0;
				clip_x2 = clip_x1+g_imageviewer_cntx.display_layer_width-1;
			clip_y2 = g_imageviewer_cntx.resized_img_height-1;

		   break;
		case 2:
			clip_x1 = 0;
			clip_y1 = -g_imageviewer_cntx.display_roi_y;
			clip_x2 = g_imageviewer_cntx.resized_img_width-1;
				clip_y2 = clip_y1+g_imageviewer_cntx.display_layer_height-1;

		   break;
		case 3:
			clip_x1 = -g_imageviewer_cntx.display_roi_x;
			clip_y1 = -g_imageviewer_cntx.display_roi_y;
				clip_x2 = clip_x1+g_imageviewer_cntx.display_layer_width-1;
				clip_y2 = clip_y1+g_imageviewer_cntx.display_layer_height-1;
			
			break;
	}

	if(clip_x1 < 0)		
		clip_x1 = 0;

	if(clip_y1 < 0)
		clip_y1 = 0;		

	if(clip_x2 >= g_imageviewer_cntx.resized_img_width)
		clip_x2 = g_imageviewer_cntx.resized_img_width-1;
	
	if(clip_y2 >= g_imageviewer_cntx.resized_img_height)
		clip_y2 = g_imageviewer_cntx.resized_img_height-1;

	gdi_handle = gdi_image_jpeg_draw_resized_file_with_output_clipping(g_imageviewer_cntx.display_roi_x,
   								  g_imageviewer_cntx.display_roi_y,
   								  g_imageviewer_cntx.resized_img_width,
   								  g_imageviewer_cntx.resized_img_height,
									  clip_x1,
									  clip_y1,
									  clip_x2,
									  clip_y2,
   								  g_imageviewer_cntx.filepath);

	if(gdi_handle == GDI_FAILED)
	{
		gdi_layer_push_and_set_active(g_imageviewer_cntx.viewport_layer_handle);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_layer_pop_and_restore_active();
		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);
		err_str_ptr = STR_ID_FMGR_INVALID_IMAGE_FORMAT;
			g_imageviewer_cntx.result = -1;

		
		if(g_imageviewer_cntx.is_err == TRUE)
			mmi_imageviewer_display_popup((PU8)GetString(err_str_ptr), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);

		g_imageviewer_cntx.is_err = FALSE;
		g_imageviewer_cntx.is_zoomin_able = FALSE;
	}
	break;
	default:
	   gdi_image_draw_resized_file(g_imageviewer_cntx.display_roi_x,
  								 		 g_imageviewer_cntx.display_roi_y,
  								 		 g_imageviewer_cntx.resized_img_width,
										 g_imageviewer_cntx.resized_img_height,
										 g_imageviewer_cntx.filepath);

	break;
	}
   gdi_layer_pop_and_restore_active();
   
}

/*****************************************************************************
* FUNCTION
*	mmi_imageviewer_extract_image_to_cache_layer
* DESCRIPTION
*  extract image to cache layer
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_imageviewer_extract_image_to_cache_layer(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if((g_imageviewer_cntx.screen_mode == VERTICAL_FULLSCREEN)||(g_imageviewer_cntx.screen_mode == HORIZONTAL_FULLSCREEN))
	{
		g_imageviewer_cntx.thumbnail_wnd_x = 0; 
		g_imageviewer_cntx.thumbnail_wnd_y = 0;
		g_imageviewer_cntx.thumbnail_wnd_width = g_imageviewer_cntx.resized_img_width;
		g_imageviewer_cntx.thumbnail_wnd_height = g_imageviewer_cntx.resized_img_height;
	}
   
   gdi_layer_push_and_set_active(g_imageviewer_cntx.cache_layer_handle);
	gdi_layer_clear(GDI_COLOR_BLACK);
   gdi_image_draw_resized_file(g_imageviewer_cntx.thumbnail_wnd_x,g_imageviewer_cntx.thumbnail_wnd_y,g_imageviewer_cntx.thumbnail_wnd_width,g_imageviewer_cntx.thumbnail_wnd_height,g_imageviewer_cntx.filepath);
	gdi_layer_pop_and_restore_active();
}
#endif


/*****************************************************************************
*
* Image Viewer View Large Image                                                                
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	mmi_imgview_highlight_view_detail
* DESCRIPTION
*  highlight handler
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_highlight_view_detail(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_imgview_highlight_view_detail()");
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
		SetLeftSoftkeyFunction(mmi_imageviewer_entry_app_screen, KEY_EVENT_UP);
		SetKeyHandler(mmi_imageviewer_entry_app_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#else
	   SetLeftSoftkeyFunction(mmi_imgview_entry_view_detail_screen, KEY_EVENT_UP);
		SetKeyHandler(mmi_imgview_entry_view_detail_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif

}



/*****************************************************************************
* FUNCTION
*	mmi_imgview_view_detail_press_up
* DESCRIPTION
*  press up in view detail screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_view_detail_press_up(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32							index;
	S32							cur_index;
	/*WeiD Add Start For 6176 Ver: TBM780  on 2007-8-20 14:33 */
	S32                                            filter_count;
	/*WeiD Add End  For 6176 Ver: TBM780  on 2007-8-20 14:33 */
	FMGR_FILE_INFO_STRUCT	file_info;
	S32							counter;
	S32							file_count;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* exit and clear previous screen for history - then enter again */
	cur_index	= mmi_fmgr_kernel_get_cur_idx();
	file_count	= mmi_fmgr_kernel_get_file_count();
	/*WeiD Add Start For 6176 Ver: TBM780  on 2007-8-20 14:33 */
	filter_count =	mmi_fmgr_kernel_get_file_filter_count();
	/*WeiD Add End  For 6176 Ver: TBM780  on 2007-8-20 14:33 */
	counter		= 0;


	/*WeiD Modify Start For 6176 Ver: TBM780  on 2007-8-20 14:33 */
	if(filter_count > 1)
	/*WeiD Modify End  For 6176 Ver: TBM780  on 2007-8-20 14:33 */
	{
		do{
			
			mmi_fmgr_kernel_move_selection_up();
			index = mmi_fmgr_kernel_get_cur_idx();
			mmi_fmgr_kernel_get_file_info(index, &file_info);

			counter++;


			if(!(file_info.attribute & FS_ATTR_DIR))
			{
				EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
				DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
				#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
					g_imageviewer_cntx.is_err=TRUE;
					mmi_imageviewer_entry_app_screen();
				#else
					mmi_imgview_entry_view_detail_screen();
				#endif
				return;
			}


			/*WeiD Modify Start For 6176 Ver: TBM780  on 2007-8-20 14:34 */
			if(counter >= (filter_count-1))
			/*WeiD Modify End  For 6176 Ver: TBM780  on 2007-8-20 14:34 */
				return;
	

		} while(1);
	}

}



/*****************************************************************************
* FUNCTION
*	mmi_imgview_view_detail_press_down
* DESCRIPTION
*  press down in view detail screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_view_detail_press_down(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32							index;
	S32							cur_index;
	FMGR_FILE_INFO_STRUCT	file_info;
	S32							counter;
	S32							file_count;
	/*WeiD Add End  For 6176 Ver: TBM780  on 2007-8-20 14:34 */
	S32                                            filter_count;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* exit and clear previous screen for history - then enter again */
	cur_index	= mmi_fmgr_kernel_get_cur_idx();
	file_count	= mmi_fmgr_kernel_get_file_count();
	/*WeiD Add Start For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	filter_count =	mmi_fmgr_kernel_get_file_filter_count();
	/*WeiD Add End  For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	counter		= 0;


	/*WeiD Modify Start For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	if(filter_count > 1)
	/*WeiD Modify End  For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	{
		do{
		
			mmi_fmgr_kernel_move_selection_down();
			index = mmi_fmgr_kernel_get_cur_idx();
			mmi_fmgr_kernel_get_file_info(index, &file_info);

			counter++;

			if(!(file_info.attribute & FS_ATTR_DIR))
			{
				EntryNewScreen(SCR_ID_IMGVIEW_VIEW, NULL, NULL, NULL);
				DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
				#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
				g_imageviewer_cntx.is_err = TRUE;
				mmi_imageviewer_entry_app_screen();
				#else
					mmi_imgview_entry_view_detail_screen();
				#endif

				return;
			}


			/*WeiD Modify Start For 6176 Ver: TBM780  on 2007-8-20 14:35 */
			if(counter >= (filter_count-1))
			/*WeiD Modify End  For 6176 Ver: TBM780  on 2007-8-20 14:35 */
				return;
	

		} while(1);
	}
}



/*****************************************************************************
* FUNCTION
*	mmi_imgview_exit_view_detail_screen
* DESCRIPTION
*  exit view detail screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_exit_view_detail_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* free buffer */
	if(g_imgview_context.filepath!=NULL)
	coolsand_UI_free(g_imgview_context.filepath);

	if(g_imgview_context.count_str!=NULL)
	coolsand_UI_free(g_imgview_context.count_str);

	g_imgview_context.filepath		= NULL;
	g_imgview_context.count_str	= NULL;
}

 

/*****************************************************************************
* FUNCTION
*	mmi_imgview_entry_view_detail_screen
* DESCRIPTION
*  entry view detail screen 
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_entry_view_detail_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	FMGR_FILE_INFO_STRUCT	file_info;
	S32							index;
	S32							total_count;
	/*WeiD Add Start For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	S32                                            filter_count;
	/*WeiD Add End  For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	S8								buffer[30];				/* for xxxx/xxxx */
	U8								*guiBuffer;

#ifdef __DRM_SUPPORT__
	BOOL 							is_permitted;
	FS_HANDLE					file_handle;
#endif

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* exit previous screen */
	EntryNewScreen(SCR_ID_IMGVIEW_VIEW, mmi_imgview_exit_view_detail_screen, mmi_imgview_entry_view_detail_screen, NULL);
 
	guiBuffer = GetCurrGuiBuffer(SCR_ID_IMGVIEW_VIEW);
	if(guiBuffer != NULL)
	{
		S8		buf_filepath[FMGR_PATH_BUFFER_SIZE];
		S8 	fmgr_filepath[FMGR_PATH_BUFFER_SIZE];
		mmi_fmgr_kernel_get_path_name	(fmgr_filepath);
		mmi_camera_get_storage_file_path(buf_filepath);

		if (pfnUnicodeStrcmp(buf_filepath,fmgr_filepath) != 0)
		{
			DeleteScreenIfPresent(SCR_ID_IMGVIEW_VIEW);
			GoBackHistory();
			return;
		}
	}


 
	/* allocate string buffer */
	g_imgview_context.filepath		= coolsand_UI_malloc(FMGR_PATH_BUFFER_SIZE);
	g_imgview_context.count_str	= coolsand_UI_malloc(60);


	/* prepare xxxx/xxxx string */
	index			= mmi_fmgr_kernel_get_cur_idx();
	total_count = mmi_fmgr_kernel_get_file_count();
	/*WeiD Add Start For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	filter_count =	mmi_fmgr_kernel_get_file_filter_count();
	/*WeiD Add End  For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	mmi_fmgr_kernel_get_file_info(index, &file_info);
	/*WeiD Modify Start For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	sprintf(buffer, "%d/%d", (U16)index+1, (U16)filter_count);
	/*WeiD Modify End  For 6176 Ver: TBM780  on 2007-8-20 14:35 */
	AnsiiToUnicodeString((PS8)g_imgview_context.count_str, (PS8)buffer);

	/* append the name to path */
	mmi_fmgr_kernel_get_path_name(g_imgview_context.filepath);

	
   if( (pfnUnicodeStrlen(g_imgview_context.filepath)+pfnUnicodeStrlen(file_info.file_name))*2 
   	   >= FMGR_PATH_BUFFER_SIZE )
   	{
   	    DisplayPopup((PU8)GetString(STR_ID_FMGR_FILENAME_TOO_LONG), IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);//STR_ID_FMGR_FILENAME_TOO_LONG
		DeleteNHistory(1);
		return;
     }
   else
	pfnUnicodeStrcat(g_imgview_context.filepath, file_info.file_name);


	/* DRM permission check */
#ifdef __DRM_SUPPORT__
	is_permitted = FALSE;

	if((file_handle = DRM_open_file((PU16)g_imgview_context.filepath, FS_READ_ONLY, DRM_PERMISSION_NONE )) >= FS_NO_ERROR)
	{
		if(DRM_validate_permission(file_handle, DRM_PERMISSION_DISPLAY))
		{
			DRM_consume_rights(file_handle, DRM_PERMISSION_DISPLAY);
			is_permitted = TRUE;
		}
		DRM_close_file(file_handle);
	}
	else if(file_handle < -1) /* file open error but not DRM related problem */
	{
		is_permitted = TRUE;
	}
#endif /* __DRM_SUPPORT__ */


#ifdef __DRM_SUPPORT__
	/* if not permitted */
	if(!is_permitted)
	{
		/* no permission to display */
      DisplayPopup((PU8)GetString(STR_GLOBAL_DRM_PROHIBITED), IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
		DeleteNHistory(1);
		return;
	}
#endif /* __DRM_SUPPORT__ */

	/* entry cat222 to decode and display a image from file */
	ShowCategory222Screen(	0, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	/* title */
									0,	0,											/* left soft key */
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK,	/* right soft key */
									GDI_COLOR_WHITE,							/* content BG color */
									(PS8)g_imgview_context.count_str,	/* button bar string */
									(PS8)g_imgview_context.filepath,		/* image file name */
									file_info.is_short,						/* is short filename */
									NULL);										/* result callback */
	
	/* if less or equal to 1 image, no need to switch to next image */
	/*WeiD Modify Start For 6176 Ver: TBM780  on 2007-8-20 14:36 */
	if(filter_count > 1)
	/*WeiD Modify End  For 6176 Ver: TBM780  on 2007-8-20 14:36 */
	{
		SetKeyHandler(mmi_imgview_view_detail_press_up,		KEY_UP_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_imgview_view_detail_press_down,	KEY_DOWN_ARROW, KEY_EVENT_DOWN);
		/*gaosh Add Start For 6127 Ver: TBM780_T_20070807_2018  on 2007-8-8 10:35 */
		SetKeyHandler(mmi_imgview_view_detail_press_up,		KEY_VOL_UP, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_imgview_view_detail_press_down,	KEY_VOL_DOWN, KEY_EVENT_DOWN);
		/*gaosh Add End  For 6127 Ver: TBM780_T_20070807_2018  on 2007-8-8 10:35 */
	}

	SetRightSoftkeyFunction(GoBackHistory,		KEY_EVENT_UP);
}





/*****************************************************************************
*
* Select Prefered Storage                                                                
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	mmi_imgview_hint_set_storage
* DESCRIPTION
*  set storage's hint
* PARAMETERS
*  a IN		selection index
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_hint_set_storage(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	memset( ( PS8 ) hintData[(index)], 0, sizeof(hintData[(index)]));
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_IMGVIEW_STORAGE_PHONE+g_camera_cntx.setting.storage) ));
}



/*****************************************************************************
* FUNCTION
*	mmi_imgview_highlight_set_storage
* DESCRIPTION
*  set storage's highlight
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_highlight_set_storage(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_imgview_entry_set_storage_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_imgview_entry_set_storage_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	mmi_imgview_highlight_set_view_style
* DESCRIPTION
*  set view style
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_highlight_set_view_style(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_imgview_entry_set_view_style_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_imgview_entry_set_view_style_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	mmi_imgview_set_storage_value
* DESCRIPTION
*  set storage's value
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_set_storage_value(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8				buf_UCS2[FMGR_PATH_BUFFER_SIZE];
  	S8				buf_filepath[FMGR_PATH_BUFFER_SIZE];
	S32			ret_drv = -1;
	S8				buf[64];
	FS_HANDLE	file_handle;
	S32			create_result;
	/*+ zhouqin modify for dual t-flash card 20110503*/
	#if defined(DUAL_TFLASH_SUPPORT)
	U16                 highlighted=0;
	#endif
	/*- zhouqin modify for dual t-flash card 20110503*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
	highlighted = GetCurrHiliteID();

	if(highlighted == CAMERA_SETTING_STORAGE_PHONE)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
	}
	else if(highlighted == CAMERA_SETTING_STORAGE_MEMORY_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE , 1, FS_NO_ALT_DRIVE);
	}
	else if(highlighted == CAMERA_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND , 1, FS_NO_ALT_DRIVE);
	}
	else
	{
		ASSERT(0);
	}
#else
	g_camera_cntx.setting.storage = GetCurrHiliteID();

	if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_PHONE)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
	}
	else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE , 1, FS_NO_ALT_DRIVE);
	}
	/*+ zhouqin modify for dual t-flash card 20110503*/
	#if defined(DUAL_TFLASH_SUPPORT)
	else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND , 1, FS_NO_ALT_DRIVE);
	}
	#endif
	/*- zhouqin modify for dual t-flash card 20110503*/
	else
	{
		ASSERT(0);
	}

#endif
/*- zhouqin modify for dual t-flash card 20110503*/


	if(ret_drv >= 0)
	{
		sprintf(buf, "%c:\\", (U8)ret_drv);
		AnsiiToUnicodeString(buf_UCS2, buf);

		file_handle = MMI_FS_Open((const UINT8*)buf_UCS2, FS_OPEN_DIR | FS_READ_ONLY);

		if(file_handle >= 0)
		{
			MMI_FS_Close(file_handle);
			/* drive is avaiable */

			/* check if photo path exist or not */
			mmi_camera_get_storage_file_path(buf_filepath);
			create_result = mmi_camera_create_file_dir(buf_filepath);

			if(create_result == FS_WRITE_PROTECTION)
			{
				mmi_imageviewer_display_popup((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_WRITE_PROTECTION), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				/*+ zhouqin modify for dual t-flash card 20110503*/
				#ifndef DUAL_TFLASH_SUPPORT 
				g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
				#endif
				/*- zhouqin modify for dual t-flash card 20110503*/
			}
			else if(create_result == FS_DISK_FULL)
			{
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				/*+ zhouqin modify for dual t-flash card 20110503*/
				#ifndef DUAL_TFLASH_SUPPORT 
				g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
				#endif
				/*- zhouqin modify for dual t-flash card 20110503*/
			}
			else if( create_result == FS_ROOT_DIR_FULL)
			{
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				/*+ zhouqin modify for dual t-flash card 20110503*/
				#ifndef DUAL_TFLASH_SUPPORT 
				g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
				#endif
				/*- zhouqin modify for dual t-flash card 20110503*/
			}
			else
			{
				g_camera_cntx.is_storage_path_changed = TRUE;
				/*+ zhouqin modify for dual t-flash card 20110503*/
				#if defined(DUAL_TFLASH_SUPPORT)
				g_camera_cntx.setting.storage = highlighted;
				#endif
				/*- zhouqin modify for dual t-flash card 20110503*/
				mmi_imageviewer_display_popup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
				DeleteUptoScrID(SCR_ID_IMGVIEW_APP);
			}

		}
		else
		{
			mmi_imageviewer_display_popup((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			/* set phone internal space as default */
			/*+ zhouqin modify for dual t-flash card 20110503*/
			#ifndef DUAL_TFLASH_SUPPORT 
			g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
			#endif
			/*- zhouqin modify for dual t-flash card 20110503*/
		}
	}
	else	
	{
		/* get drive error  */
			mmi_imageviewer_display_popup((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		/* set phone internal space as default */
		/*+ zhouqin modify for dual t-flash card 20110503*/
		#ifndef DUAL_TFLASH_SUPPORT 
		g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
		#endif
		/*- zhouqin modify for dual t-flash card 20110503*/
	}

	mmi_camera_store_setting();
}

/*****************************************************************************
* FUNCTION
*	mmi_imgview_entry_set_storage_screen
* DESCRIPTION
*  enter set storage screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_entry_set_storage_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	EntryNewScreen(SCR_ID_IMGVIEW_STORAGE, NULL, mmi_imgview_entry_set_storage_screen, NULL);


	/* load all settings */
	mmi_camera_load_setting();

	nItems		= GetNumOfChild(MENU_ID_IMGVIEW_STORAGE);
	GetSequenceStringIds(MENU_ID_IMGVIEW_STORAGE,ItemList);
	GetSequenceImageIds(MENU_ID_IMGVIEW_STORAGE,ItemIcons);
	SetParentHandler(MENU_ID_IMGVIEW_STORAGE);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
/*+ zhouqin modify for dual t-flash card 20110503*/
#ifndef DUAL_TFLASH_SUPPORT
// beginning--added by kecx for  fixing bug 13132 on 20090714
    if(!g_hasMountedTFlashFS)
        {
            g_camera_cntx.setting.storage=CAMERA_SETTING_STORAGE_PHONE;
        }
// end--added by kecx for fixing bug 13132 on 20090714 
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	ShowCategory11Screen(	STR_ID_IMGVIEW_STORAGE, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									nItems, 
									ItemList,
									g_camera_cntx.setting.storage,				/* current index */
									NULL);

	SetLeftSoftkeyFunction(mmi_imgview_set_storage_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}




/*****************************************************************************
*
* View Style                                                             
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	mmi_imgview_entry_set_view_style_screen
* DESCRIPTION
*  enter set view style screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_entry_set_view_style_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16		item_list[16];
	U16		item_icon[16];
	U16		num_item;
	S16		error;
	U8			imgview_style;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	EntryNewScreen(SCR_ID_IMGVIEW_VIEW_STYLE, NULL, mmi_imgview_entry_set_view_style_screen, NULL);

	num_item = GetNumOfChild(MENU_ID_IMGVIEW_VIEW_STYLE);
	GetSequenceStringIds(MENU_ID_IMGVIEW_VIEW_STYLE, item_list);
	GetSequenceImageIds(MENU_ID_IMGVIEW_VIEW_STYLE, item_icon);
	SetParentHandler(MENU_ID_IMGVIEW_VIEW_STYLE);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ReadValue(NVRAM_IMGVIEW_VIEW_SYTLE, &imgview_style, DS_BYTE , &error);

	ShowCategory11Screen(	STR_ID_IMGVIEW_VIEW_STYLE, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									num_item, 
									item_list,
									imgview_style,						/* current index */
									NULL);

	SetLeftSoftkeyFunction(mmi_imgview_set_view_style_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	mmi_imgview_set_view_style_value
* DESCRIPTION
*  set view style value
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_set_view_style_value(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8			imgview_style;
	S16		error;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	imgview_style = (U8)GetCurrHiliteID();

	WriteValue(NVRAM_IMGVIEW_VIEW_SYTLE, &imgview_style, DS_BYTE , &error);
	mmi_imageviewer_display_popup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	
	DeleteNHistory(2);
}

/*****************************************************************************
* FUNCTION
*	mmi_imgview_get_view_style
* DESCRIPTION
*  set view style value
* PARAMETERS
*  void
* RETURNS
*  U16 	style (0 = IMGVIEW_VIEW_STYLE_LIST, 1 = IMGVIEW_VIEW_STYLE_MATRIX)
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
U16 mmi_imgview_get_view_style(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8			imgview_style;
	S16		error;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	imgview_style = (U8)GetCurrHiliteID();

	ReadValue(NVRAM_IMGVIEW_VIEW_SYTLE, &imgview_style, DS_BYTE , &error);

	if(imgview_style == 0xff)
	{
		imgview_style = IMGVIEW_VIEW_STYLE_LIST;
		WriteValue(NVRAM_IMGVIEW_VIEW_SYTLE, &imgview_style, DS_BYTE , &error);
	}

	return (U16)imgview_style;
}

/*****************************************************************************
*
* Initialization and Entry of ImageViewer Application                                                                  
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	mmi_imgview_highlight_app
* DESCRIPTION
*  app hilight handler
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_highlight_app(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_imgview_entry_app,KEY_EVENT_UP);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#if !defined __MMI_MAINLCD_240X320__ && !defined (__MMI_BLACK_BERRY_MAIN_MENU__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */	
	SetKeyHandler(mmi_imgview_entry_app, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_imgview_init_app
* DESCRIPTION
*  init imageviwer applcation, register highlight handler 
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_init_app(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   mmi_trace(g_sw_CAMERA, "Excute func : mmi_imgview_init_app()");
   

   mmi_trace(TRUE, "^^^^^^^^^  MAIN_MENU_MULTIMEDIA_MENUID = %d", MAIN_MENU_MULTIMEDIA_MENUID);
	mmi_trace(TRUE, "^^^^^^^^^ MENU_ID_IMGVIEW_APP = %d", MENU_ID_IMGVIEW_APP);
	
#if 0
	mmi_trace(g_sw_CAMERA, "^^^^^^^^^ MENU_ID_IMGVIEW_EMPTY_FOLDER_OPTION = %d", MENU_ID_IMGVIEW_EMPTY_FOLDER_OPTION);
	mmi_trace(g_sw_CAMERA, "^^^^^^^^^ MENU_ID_IMGVIEW_STORAGE = %d", MENU_ID_IMGVIEW_STORAGE);
	mmi_trace(g_sw_CAMERA, "^^^^^^^^^ MENU_ID_IMGVIEW_VIEW_STYLE = %d", MENU_ID_IMGVIEW_VIEW_STYLE);
	mmi_trace(g_sw_CAMERA, "^^^^^^^^^ MENU_ID_IMGVIEW_STORAGE_PHONE = %d", MENU_ID_IMGVIEW_STORAGE_PHONE);
	mmi_trace(g_sw_CAMERA, "^^^^^^^^^ MENU_ID_IMGVIEW_STORAGE_MEMORY_CARD = %d", MENU_ID_IMGVIEW_STORAGE_MEMORY_CARD);
#endif
	/* init imageviewer application entry handler */
#if defined(__CAMERA_OSD_HORIZONTAL__)
	g_imageviewer_cntx.screen_mode = VERTICAL_FULLSCREEN;
	g_imageviewer_cntx.imgview_rotate = 0 ;
	g_imageviewer_cntx.is_resume = FALSE;
	g_imageviewer_cntx.is_debug = FALSE;
	g_imageviewer_cntx.is_trigger_by_keypad = FALSE;
	g_imageviewer_cntx.is_gif_file = FALSE;
	g_imageviewer_cntx.move_offset_x = 0;
	g_imageviewer_cntx.move_offset_y = 0;
	g_imageviewer_cntx.is_err=TRUE; 
#endif
	SetHiliteHandler(MENU_ID_IMGVIEW_APP,				mmi_imgview_highlight_app);
	
	SetHiliteHandler(MENU_ID_IMGVIEW_VIEW_DETAIL,	mmi_imgview_highlight_view_detail);
	SetHiliteHandler(MENU_ID_IMGVIEW_STORAGE,			mmi_imgview_highlight_set_storage);
	SetHiliteHandler(MENU_ID_IMGVIEW_VIEW_STYLE,		mmi_imgview_highlight_set_view_style);

	SetHintHandler(MENU_ID_IMGVIEW_STORAGE,			mmi_imgview_hint_set_storage);
}



/*****************************************************************************
* FUNCTION
*	mmi_imgview_exit_from_fmgr
* DESCRIPTION
*  exit function called by fmgr
* PARAMETERS
*  filename 	IN		filename(not used here)
*  is_shot		IN		is it short naming
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_exit_from_fmgr(void* filename, int is_short)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	DeleteUptoScrID(SCR_ID_IMGVIEW_APP);
	DeleteNHistory(1);
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*	mmi_imgview_exit_from_fmgr_emptry_folder
* DESCRIPTION
*  exit function called by fmgr in empty folder
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_imgview_exit_from_fmgr_emptry_folder(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	DeleteUptoScrID(SCR_ID_IMGVIEW_APP);
	DeleteNHistory(1);
	GoBackHistory();
}


/*****************************************************************************
* FUNCTION
*	mmi_imgview_entry_app
* DESCRIPTION
*  image viewer entry point 
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void) ;
void mmi_imgview_entry_app(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8				buf_filepath[FMGR_PATH_BUFFER_SIZE];
	FS_HANDLE	file_handle;
	FMGR_FILTER	filter;
	S32			create_result;
	U16   PreScrnID;

    mmi_trace(g_sw_CAMERA, "Excute func : mmi_imgview_entry_app()");
	PreScrnID  = GetCurrScrnId();
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
//#if 0

#if 0
	if(!is_phb_init_done())
	{
	    //开机初始化 stringid =25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS/*IMG_GLOBAL_ACTIVATED*/, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);//modified by lin for bug 6761@20070929
		return;
	}
#endif
		if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
	
	EntryNewScreen(SCR_ID_IMGVIEW_APP, NULL, mmi_imgview_entry_app, NULL);

	/* load camera's storage setting */
	mmi_camera_load_setting();
#ifndef COOLSAND_MULTIMEDIA_USE_FLASHFS	
	g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_MEMORY_CARD;
#endif

	mmi_camera_get_storage_file_path(buf_filepath);
	create_result = mmi_camera_create_file_dir(buf_filepath);
    if(create_result )    
    {        
        mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), 
        IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
        
        DeleteNHistory(1);
        return;    
    }          

    mmi_trace(g_sw_CAMERA,"mmi_imgview_entry_app,create_result is %d",create_result);
//#if 0
	/* reset imageviewr */
	#ifdef HORIZONTAL_CAMERA
		g_imageviewer_cntx.is_err = TRUE;
	#endif
	
	
	/* check if it is valid path */
	file_handle	= MMI_FS_Open((const UINT8*)buf_filepath, FS_OPEN_DIR | FS_READ_ONLY);
	//file_handle=0;
#if 1 
	if(file_handle >= 0)
	{
		MMI_FS_Close(file_handle);

		FMGR_FILTER_INIT(&filter);
		FMGR_FILTER_SET_IMAGE(&filter);
		mmi_fmgr_select_path_and_enter(	APP_IMAGEVIEWER, 
													FMGR_SELECT_BROWSE, 
													filter, 
													buf_filepath, 
													(PsExtFuncPtr)mmi_imgview_exit_from_fmgr);
		
#if 0
		/* check if device is busy or not */
		if (MMI_FS_GetDevStatus(buf_filepath[0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY)
		{
			/*  mmi_fmgr_select_path_and_enter will also add screen to histry */
			DeleteUptoScrID(SCR_ID_IMGVIEW_APP);
		
			/* Delete up to previous screen */
			DeleteNHistory(1);
		}
#endif
		
	}	
	else
	{
		if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_PHONE)
		{
			/* storage is phone */
			
			/* if storage is already on phone, but it is still failed, means phone's FAT is corrupted */
			if(create_result == FS_DISK_FULL)
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else if(create_result == FS_ROOT_DIR_FULL)
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_FAT_ALLOC_ERROR_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
	
			DeleteNHistory(1);
			return;
		}
		else
		{
			/* storage is card */
			
			/* photo's path is not exist, and card is write protected, cant create photo folder */
			if(create_result == FS_WRITE_PROTECTION)
				mmi_imageviewer_display_popup((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_WRITE_PROTECTION), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else if(create_result == FS_DISK_FULL)
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else if(create_result == FS_ROOT_DIR_FULL)
				mmi_imageviewer_display_popup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			else
				mmi_imageviewer_display_popup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD/*STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD*/), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		
//			g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
//			mmi_camera_store_setting();

			DeleteNHistory(1);
	
//			DeleteUptoScrID(PreScrnID);
//			DeleteUptoScrID(SCR_ID_IMGVIEW_APP);
			return;

		}

	}
#endif
}

#endif /* __MMI_CAMERA__ */



