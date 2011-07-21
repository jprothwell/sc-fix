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
 *	 PhotoArt.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Photo Art Applcation Source of PhotoEditor
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/****************************************************************************
* Include Files                                                                
*****************************************************************************/
#include "mmi_features.h"
#ifdef __MMI_PHOTOEDITOR__

#define __NEWSIMULATOR 

#ifdef MMI_ON_HARDWARE_P
	#include "kal_release.h"
	#include "nvram_data_items.h" 	
#else
	#include "kal_non_specific_general_types.h"
#endif /* MMI_ON_HARDWARE_P */


#include "custmenures.h"
#include "globalconstants.h"				/* key id define */
#include "fat_fs.h"							/* must after globalconstants.h */
#include "globaldefs.h"						/* golbal image and string id. */
#include "gui.h"								/* gui functions */
#include "wgui_categories.h"
#include "historygprot.h"					/* screen history */
#include "unicodexdcl.h"					/* unicode transform functions */
#include "nvramtype.h"						/* nvram data type */
#include "nvramprot.h"						/* nvram access fucntions */	
#include "nvramenum.h"						/* nvram id define */
#include "eventsgprot.h"		
#include "settingdefs.h"					/* st_notifyduration define */
#include "settingprofile.h"				/* success_tone define */
#include "wgui.h"								/* for getdatetime() */
#include "commonscreens.h"					/* displaypopup() */
#include "debuginitdef.h"					/* mmi_assert */
#include "mainmenudef.h"					/* multimedia icon */
#include "nvram_data_items.h" 			
#include "wgui_categories_multimedia.h"
#include "wgui_categories_fmgr.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_inputs.h"
#include "frameworkstruct.h"				/* for psextfuncptr */
#include "conversions.h"					/* short file name conversion */
#include "filemanagerdef.h"
#include "filemanagergprot.h"				/* file path util function */
#include "filemgr.h"
#include "lcd_sw_rnd.h"
#include "gdi_include.h"					/* graphic library */
#include "gdi_image_jpeg.h"

#include "med_api.h"							/* media task camera module */
#include	"mdi_datatype.h"
#include "mdi_camera.h"

#include "photoeditor.h"
#include "photoeditorgprot.h"
#include "phoedtscrenum.h"
#include "phoedtstrenum.h"
#include "phoedtimgenum.h"
#include "photoart.h"

#include "mmi_features_camera.h"
#include "cameraresdefs.h"					/* for camrea error string */

#include "mmi_trace.h"

#ifdef __DRM_SUPPORT__
	#include "fs_errcode.h"
	#include "drm_gprot.h"					/* drm support */
#endif

#ifndef MMI_ON_HARDWARE_P
	int	*DRM_get_rights(FS_HANDLE input) { return 0; }
	void	kal_sleep_task(kal_uint32 time_in_ticks) {}
#endif


#ifdef __PHOEDT_FEATURE_PHOART__

/***************************************************************************** 
* Define
*****************************************************************************/
#define PHOART_MAIN_CANVAS_COLOR				(GDI_COLOR_BLACK)
#define PHOART_MAIN_FRAME_COLOR				(GDI_COLOR_WHITE)
#ifndef __MMI_MAINLCD_240X320__
	#define PHOART_MAIN_ARROW_FROM_CENTER		(50)
#else
	#define PHOART_MAIN_ARROW_FROM_CENTER		(70)
#endif 

#define PHOART_CLIP_CANVAS_COLOR				(GDI_COLOR_WHITE)
#define PHOART_CLIP_FRAME_INSIDE_COLOR		(GDI_COLOR_WHITE)
#define PHOART_CLIP_FRAME_OUTSIDE_COLOR	(GDI_COLOR_BLACK)

#define PHOART_CLIP_SCREEN_SPACING			(5)
#define PHOART_CLIP_RECT_START_RATIO		(80)
#define PHOART_CLIP_RECT_MIN_RATIO			(30)

#define PHOART_ICON_BBOX_SHIFT				(2)
#define PHOART_ICON_BBOX_LINE_WIDTH			(3)

#define PHOART_TEXT_BBOX_SHIFT				(4)
#define PHOART_TEXT_BBOX_LINE_WIDTH			(3)

#define PHOART_HINT_SPACING					(5)
#define PHOART_TEXT_COLOR_COUNT				(23)

#define DUMMY_COMMAND 							(0)


#define ICON_LAYER_X1 		(g_phoart_cntx.icon_offset_x-PHOART_ICON_BBOX_SHIFT)
#define ICON_LAYER_X2 		(g_phoart_cntx.icon_offset_x+g_phoart_cntx.icon_width+PHOART_ICON_BBOX_SHIFT-1)
#define ICON_LAYER_Y1 		(g_phoart_cntx.icon_offset_y-PHOART_ICON_BBOX_SHIFT)
#define ICON_LAYER_Y2		(g_phoart_cntx.icon_offset_y+g_phoart_cntx.icon_height+PHOART_ICON_BBOX_SHIFT-1)
#define ICON_LAYER_WIDTH	(g_phoart_cntx.icon_width+PHOART_ICON_BBOX_SHIFT*2)
#define ICON_LAYER_HEIGHT	(g_phoart_cntx.icon_height+PHOART_ICON_BBOX_SHIFT*2)

#define TEXT_LAYER_X1 		(g_phoart_cntx.text_offset_x-PHOART_TEXT_BBOX_SHIFT)
#define TEXT_LAYER_X2 		(g_phoart_cntx.text_offset_x+g_phoart_cntx.text_width+PHOART_TEXT_BBOX_SHIFT-1)
#define TEXT_LAYER_Y1 		(g_phoart_cntx.text_offset_y-PHOART_TEXT_BBOX_SHIFT)
#define TEXT_LAYER_Y2 		(g_phoart_cntx.text_offset_y+g_phoart_cntx.text_height+PHOART_TEXT_BBOX_SHIFT-1)
#define TEXT_LAYER_WIDTH	(g_phoart_cntx.text_width+PHOART_TEXT_BBOX_SHIFT*2)
#define TEXT_LAYER_HEIGHT	(g_phoart_cntx.text_height+PHOART_TEXT_BBOX_SHIFT*2)

#if ((LCD_WIDTH == 176) && (LCD_HEIGHT == 220))
	#define PHOART_SAVE_PREDICT_SIZE 	(30*1024)	/* 30k */
#elif ( (LCD_WIDTH == 220) && (LCD_HEIGHT == 176) )	
	#define PHOART_SAVE_PREDICT_SIZE 	(30*1024)	/* 30k */
#elif ((LCD_WIDTH == 240) && (LCD_HEIGHT == 320))
	#define PHOART_SAVE_PREDICT_SIZE 	(50*1024)	/* 50k */
#else
	#define PHOART_SAVE_PREDICT_SIZE 	(20*1024)	/* 20k */
#endif


typedef enum {
	PHOART_BORDER_NONE,
	PHOART_BORDER_CORNER,
	PHOART_BORDER_ALL
} phoart_draw_border_enum;



/***************************************************************************** 
* Global Variable
*****************************************************************************/
extern wgui_inline_item	 	wgui_inline_items[];
extern phoedt_cntx_struct	g_phoedt_cntx;
extern BOOL						r2lMMIFlag;

#ifndef APPMEM_SUPPORT
	extern U32 camera_osd_layer_buffer[];
#endif /* APPMEM_SUPPORT */

/***************************************************************************** 
* Global Function
*****************************************************************************/
extern void draw_resizeable_arrow(S32 x, S32 y, U8 arrow_type, color_t c, S32 width, S32 height);
extern void UI_draw_horizontal_line(S32 x1, S32 x2, S32 y, color_t c);
extern void UI_putpixel(S32 x, S32 y, color_t c);
extern void UI_draw_vertical_line(S32 y1, S32 y2, S32 x, color_t c);
extern void DisplayIdleScreen(void);
extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void) ;
extern unsigned long AllocMediaMemory(unsigned long clicks);
extern GDI_RESULT gdi_image_jpeg_draw_screen(S32 src_x,S32 src_y, S32 clip_x1,S32 clip_x2,S32 clip_y1,S32 clip_y2,S32 show_width,S32 show_height);
extern void FreeMediaMemory(unsigned long base);
extern U8 PhnsetGetDefEncodingType(void);
extern void GBSetInputboxLSKFunction(FuncPtr f);
/*****************************************************************************
* Local Vairable                                                           
*****************************************************************************/
static phoart_cntx_struct 			g_phoart_cntx;
static phoedt_nvram_data_struct	*g_phoart_setting = &g_phoedt_cntx.setting_data; 

#ifdef __MMI_TOUCH_SCREEN__
static phoart_touch_screen_context_struct		g_phoart_touch_screen_cntx;
#endif


const color_t phoart_text_color_list[PHOART_TEXT_COLOR_COUNT] = 
{
	{0,0,255},	/* pure blue will treat as none */
	{61,102,206},
	{115,241,252},
	{145,61,206},
	{203,161,205},	
	{255,53,240},
	{244,154,193},	
	{254,132,0},
	{255,240,0},
	{254,0,24},
	{249,173,129},
	{89,189,69},
	{201,231,197},
	{0,114,54},
	{196,223,155},
	{96,57,19},
	{200,159,113},
	{27,20,100},
	{255,255,255},
	{200,200,200},	
	{150,150,150},
	{100,100,100},
	{0,0,0}
};



const U8	phoart_effect_command_map[PHOART_EFFECT_COUNT+1] = 
{
#ifdef PHOART_FEATURE_EFFECT_GRAYSCALE
	MDI_CAMERA_POST_EFFECT_GRAYSCALE,
#endif

#ifdef PHOART_FEATURE_EFFECT_SEPIA
	MDI_CAMERA_POST_EFFECT_SEPIA,
#endif

#ifdef PHOART_FEATURE_EFFECT_SEPIA_GREEN
	MDI_CAMERA_POST_EFFECT_SEPIAGREEN,
#endif	

#ifdef PHOART_FEATURE_EFFECT_SEPIA_BLUE
	MDI_CAMERA_POST_EFFECT_SEPIABLUE,
#endif	

#ifdef PHOART_FEATURE_EFFECT_COLOR_INVERT
	MDI_CAMERA_POST_EFFECT_COLORINV,
#endif	

#ifdef PHOART_FEATURE_EFFECT_GRAY_INVERT
	MDI_CAMERA_POST_EFFECT_GRAYINV,
#endif	

#ifdef PHOART_FEATURE_EFFECT_WATER_COLOR
	MDI_CAMERA_POST_EFFECT_WATERCOLOR,
#endif	

#ifdef PHOART_FEATURE_EFFECT_LIGHT_BLUR
	MDI_CAMERA_POST_EFFECT_LIGHTBLUR,
#endif	

#ifdef PHOART_FEATURE_EFFECT_BLUR
	MDI_CAMERA_POST_EFFECT_BLUR,
#endif	
	
#ifdef PHOART_FEATURE_EFFECT_STRONG_BLUR
	MDI_CAMERA_POST_EFFECT_STRONGBLUR,
#endif	
	
#ifdef PHOART_FEATURE_EFFECT_UNSHARP
	MDI_CAMERA_POST_EFFECT_UNSHARP,
#endif	

#ifdef PHOART_FEATURE_EFFECT_SHARPEN	
	MDI_CAMERA_POST_EFFECT_SHARPEN,
#endif	
	
#ifdef PHOART_FEATURE_EFFECT_MORE_SHARPEN
	MDI_CAMERA_POST_EFFECT_MORESHARPEN,
#endif	

#ifdef PHOART_FEATURE_EFFECT_MEDIAN	
	MDI_CAMERA_POST_EFFECT_MEDIAN,
#endif	

#ifdef PHOART_FEATURE_EFFECT_DILATION	
	MDI_CAMERA_POST_EFFECT_DILATION,
#endif	

#ifdef PHOART_FEATURE_EFFECT_EROSION
	MDI_CAMERA_POST_EFFECT_EROSION,
#endif	

	DUMMY_COMMAND
};





/*****************************************************************************
* Local Variable                                                           
*****************************************************************************/
const U8	phoart_adjust_image_command_map[PHOART_ADJUST_IMAGE_TYPE_COUNT+1] = 
{
	MDI_CAMERA_ADJUSMENT_BRIGHTNESS,
	MDI_CAMERA_ADJUSMENT_CONTRAST,
	MDI_CAMERA_ADJUSMENT_SATURATION,
	MDI_CAMERA_ADJUSMENT_HUE,
	DUMMY_COMMAND
};

const U8	phoart_adjust_color_command_map[PHOART_ADJUST_COLOR_TYPE_COUNT+1] = 
{
	MDI_CAMERA_ADJUSMENT_COLOR_R,
	MDI_CAMERA_ADJUSMENT_COLOR_G,
	MDI_CAMERA_ADJUSMENT_COLOR_B,
	DUMMY_COMMAND
};



/*****************************************************************************
* Local Function                                                           
*****************************************************************************/
/* app */
static void mmi_phoart_hilight_app(void);
static void mmi_phoart_show_press_release_softkey(void);
static void mmi_phoart_lsk_press(void);
static void mmi_phoart_lsk_release(void);
static void mmi_phoart_rsk_press(void);
static void mmi_phoart_rsk_release(void);
static void mmi_phoart_draw_softkey(PS8 lsk_str, PS8 rsk_str, BOOL is_clear_bg);
static void mmi_phoart_draw_title(PS8 title_str, PS8 sel_str, BOOL is_clear_bg);
static BOOL mmi_phoart_write_cache_image(PU8 buf_ptr);
static BOOL mmi_phoart_load_cache_image(PU8 buf_ptr);
static void mmi_phoart_draw_image_border(void);

/* file selection screen */
static void mmi_phoart_entry_app_screen(void);
		 void mmi_phoart_fmgr_select_result_callback(void *in_ptr, int is_short);

/* clip option screen */
static void mmi_phoart_entry_clip_option_screen(void);
static void mmi_phoart_back_from_clip_option_screen(void);
static void mmi_phoart_hilight_auto_resize(void);
static void mmi_phoart_hilight_mainlcd_clip(void);
#ifdef __MMI_SUBLCD__
	static void mmi_phoart_hilight_sublcd_clip(void);
#endif /* __MMI_SUBLCD__ */

/* clip screen */
static void mmi_phoart_entry_clip_screen(void);
static void mmi_phoart_exit_clip_screen(void);
static void mmi_phoart_clip_draw_clip_rect(void);
static void mmi_phoart_clip_move_clip_rect_cyclic(void);
static void mmi_phoart_clip_up_arrow_press(void);
static void mmi_phoart_clip_down_arrow_press(void);
static void mmi_phoart_clip_left_arrow_press(void);
static void mmi_phoart_clip_right_arrow_press(void);
static void mmi_phoart_clip_up_arrow_release(void);
static void mmi_phoart_clip_down_arrow_release(void);
static void mmi_phoart_clip_left_arrow_release(void);
static void mmi_phoart_clip_right_arrow_release(void);
static void mmi_phoart_clip_scale_up(void);
static void mmi_phoart_clip_scale_down(void);
static void mmi_phoart_clip_clip_image(void);


/* main screen */
static void mmi_phoart_entry_main_screen(void);
static void mmi_phoart_exit_main_screen(void);
static U8 	mmi_phoart_del_screen_id_hdlr(void*);
static void mmi_phoart_exit_main_confirm(void);
static void mmi_phoart_exit_main_confirm_yes(void);
static void mmi_phoart_main_draw_osd(void);

/* option screen */
static void mmi_phoart_entry_option_screen(void);

/* save screen */
static void mmi_phoart_hilight_save_as(void);
static void mmi_phoart_entry_editor_filename_screen(void);
static void mmi_phoart_entry_editor_filename_internal(void);
static void mmi_phoart_return_to_editor_screen(void);
static void mmi_phoart_back_from_editor_screen(void);
static void mmi_phoart_entry_editor_option_screen(void);
static void mmi_phoart_hilight_editor_done(void);
static void mmi_phoart_hilight_editor_cancel(void);
static void mmi_phoart_hilight_editor_input(void);
static void mmi_phoart_cancel_save_file(void);
static void mmi_phoart_save_file(void);
	
/* add effect */
static void mmi_phoart_entry_effect_select_screen(void);
static void mmi_phoart_hilight_add_effect(void);
static void mmi_phoart_effect_select_highlight_hdlr(S32 idx);
static void mmi_phoart_entry_effect_edit_screen(void);
static void mmi_phoart_exit_effect_edit_screen(void);
static void mmi_phoart_effect_edit_add_effect(void);
static void mmi_phoart_effect_edit_change_forward(void);
static void mmi_phoart_effect_edit_change_backward(void);
static void mmi_phoart_effect_edit_draw_osd(void);

/* add frame */
static void mmi_phoart_entry_frame_select_screen(void);
static void mmi_phoart_hilight_add_frame(void);
static void mmi_phoart_frame_select_highlight_hdlr(S32 idx);
static void mmi_phoart_entry_frame_edit_screen(void);
static void mmi_phoart_exit_frame_edit_screen(void);
static void mmi_phoart_frame_edit_draw_frame(void);
static void mmi_phoart_frame_edit_change_backward(void);
static void mmi_phoart_frame_edit_change_forward(void);
static void mmi_phoart_frame_edit_draw_osd(BOOL is_draw_frame);

/* add icon */
static void mmi_phoart_entry_icon_select_screen(void);
static void mmi_phoart_hilight_add_icon(void);
static void mmi_phoart_icon_select_hide_image_callback(S32 idx);
static void mmi_phoart_icon_select_draw_image_callback(	S32 idx, S32 page_start_idx, S32 page_end_idx, 
																			BOOL is_hilighted, BOOL is_need_update, BOOL is_new_page, 
																			S32 x1, S32 y1, S32 x2, S32 y2);
static void mmi_phoart_icon_select_highlight_hdlr(S32 idx);
static void mmi_phoart_entry_icon_edit_screen(void);
static void mmi_phoart_exit_icon_edit_screen(void);
static U8 	mmi_phoart_icon_del_screen_id_hdlr(void *ptr);
static void mmi_phoart_icon_edit_draw_icon(U16 border_style);
static void mmi_phoart_icon_edit_move_down_press(void);
static void mmi_phoart_icon_edit_move_up_press(void);
static void mmi_phoart_icon_edit_move_left_press(void);
static void mmi_phoart_icon_edit_move_right_press(void);
static void mmi_phoart_icon_edit_move_down_release(void);
static void mmi_phoart_icon_edit_move_up_release(void);
static void mmi_phoart_icon_edit_move_left_release(void);
static void mmi_phoart_icon_edit_move_right_release(void);
static void mmi_phoart_icon_edit_move_icon_cyclic(void);
static void mmi_phoart_icon_edit_stamp(void);
static void mmi_phoart_icon_edit_stamp_when_done(void);
static void mmi_phoart_icon_edit_draw_osd(void);

/* add text */
static void mmi_phoart_entry_text_str_editor_screen(void);
static void mmi_phoart_hilight_add_text(void);
static void mmi_phoart_entry_text_str_editor_option_screen(void);
static void mmi_phoart_text_hilight_editor_done(void);
static void mmi_phoart_text_hilight_editor_cancel(void);
static void mmi_phoart_text_hilight_editor_input(void);
static void mmi_phoart_text_cancel_edit(void);
static void mmi_phoart_text_draw_style_text(S32 offset_x, S32 offset_y, S32 width, S32 height, PS8 str_ptr);
static void mmi_phoart_text_select_draw_text(void);
static void mmi_phoart_text_inline_select_highlight_hdlr(S32 index);
static void mmi_phoart_text_select_highlight_hdlr(S32 index);
static void mmi_phoart_text_select_text_color_hilight_callback(U16 index);
static void mmi_phoart_text_select_border_color_hilight_callback(U16 index);
static void mmi_phoart_text_select_shadow_color_hilight_callback(U16 index);
static void mmi_phoart_text_select_draw_info_callback(S32 index, S32 x1, S32 y1, S32 x2, S32 y2);
static void mmi_phoart_text_select_done_hdlr(void);
static void mmi_phoart_entry_text_select_screen(void);
static void mmi_phoart_exit_text_select_screen(void);

static void mmi_phoart_entry_text_edit_screen(void);
static void mmi_phoart_exit_text_edit_screen(void);
static U8	mmi_phoart_text_del_screen_id_hdlr(void *ptr);
static void mmi_phoart_text_edit_draw_text(U16 border_style);
static void mmi_phoart_text_edit_move_down_press(void);
static void mmi_phoart_text_edit_move_up_press(void);
static void mmi_phoart_text_edit_move_left_press(void);
static void mmi_phoart_text_edit_move_right_press(void);
static void mmi_phoart_text_edit_move_down_release(void);
static void mmi_phoart_text_edit_move_up_release(void);
static void mmi_phoart_text_edit_move_left_release(void);
static void mmi_phoart_text_edit_move_right_release(void);
static void mmi_phoart_text_edit_move_text_cyclic(void);
static void mmi_phoart_text_edit_stamp(void);
static void mmi_phoart_text_edit_stamp_when_done(void);
static void mmi_phoart_text_edit_draw_osd(void);

/* adjustment */
static void mmi_phoart_entry_adjust_option_screen(void);
static void mmi_phoart_adjust_hilight_option(void);
static void mmi_phoart_adjust_hilight_image_adjust(void);
static void mmi_phoart_adjust_hilight_color_adjust(void);
static void mmi_phoart_entry_adjust_edit_screen(void);
static void mmi_phoart_exit_adjust_edit_screen(void);
static void mmi_phoart_adjust_edit_add_adjustment(void);
static void mmi_phoart_adjust_edit_right_arrow_press(void);
static void mmi_phoart_adjust_edit_left_arrow_press(void);
static void mmi_phoart_adjust_edit_init_list(void);
static void mmi_phoart_adjust_edit_draw_image_list(void);
static void mmi_phoart_adjust_edit_image_list_up(void);
static void mmi_phoart_adjust_edit_image_list_down(void);
static void mmi_phoart_adjust_edit_image_draw_osd(void);
static void mmi_phoart_adjust_edit_draw_color_list(void);
static void mmi_phoart_adjust_edit_color_list_up(void);
static void mmi_phoart_adjust_edit_color_list_down(void);
static void mmi_phoart_adjust_edit_color_draw_osd(void);

/* flip */
static void mmi_phoart_hilight_flip(void);
static void mmi_phoart_entry_flip_edit_screen(void);
static U8	mmi_phoart_flip_del_screen_id_hdlr(void *ptr);
#ifdef __MMI_TOUCH_SCREEN__
	static void mmi_phoart_flip_edit_draw_arrow(void);
#endif /* __MMI_TOUCH_SCREEN__ */
static void mmi_phoart_exit_flip_edit_screen(void);
static void mmi_phoart_flip_edit_vertical_flip(void);
static void mmi_phoart_flip_edit_horizontal_flip(void);
static void mmi_phoart_flip_edit_draw_osd(void);

/* expand canvas */
static void mmi_phoart_expand_canvas_confirm(void);
static void mmi_phoart_hilight_expand_canvas(void);
static void mmi_phoart_expand_canvas_confirm_yes(void);

/* help */
static void mmi_phoart_entry_help_screen(void);
static void mmi_phoart_hilight_help(void);
void mmi_phoart_layer_flatten_to_base(gdi_handle handle0, gdi_handle handle1,gdi_handle handle2, gdi_handle handle3);

/* touch screen */
#ifdef __MMI_TOUCH_SCREEN__
	void mmi_phoart_pen_down_hdlr(mmi_pen_point_struct pos);
	void mmi_phoart_pen_up_hdlr(mmi_pen_point_struct pos);
	void mmi_phoart_pen_move_hdlr(mmi_pen_point_struct pos);
	void mmi_phoart_pen_repeat_hdlr(mmi_pen_point_struct pos);
#endif


 
/*****************************************************************************
* FUNCTION
*	 mmi_phoart_init_app()
* DESCRIPTION
*   init photo art applcation
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_phoart_init_app(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetHiliteHandler(MENU_ID_PHOART_APP, mmi_phoart_hilight_app);
	SetHiliteHandler(MENU_ID_PHOART_AUTO_RESIZE, mmi_phoart_hilight_auto_resize);
	SetHiliteHandler(MENU_ID_PHOART_MAINLCD_CLIP, mmi_phoart_hilight_mainlcd_clip);
#ifdef __MMI_SUBLCD__
	SetHiliteHandler(MENU_ID_PHOART_SUBLCD_CLIP, mmi_phoart_hilight_sublcd_clip);
#endif 

	/* save screen */
	SetHiliteHandler(MENU_ID_PHOART_SAVE_AS, mmi_phoart_hilight_save_as);
	SetHiliteHandler(MENU_ID_PHOART_EDITOR_DONE, mmi_phoart_hilight_editor_done);
	SetHiliteHandler(MENU_ID_PHOART_EDITOR_CANCEL, mmi_phoart_hilight_editor_cancel);	
	SetHiliteHandler(MENU_ID_PHOART_EDITOR_INPUT, mmi_phoart_hilight_editor_input);

	/* add effect */
	SetHiliteHandler(MENU_ID_PHOART_ADD_EFFECT, mmi_phoart_hilight_add_effect);
	
	/* add frame */
	SetHiliteHandler(MENU_ID_PHOART_ADD_FRAME, mmi_phoart_hilight_add_frame);
	
	/* add icon */
	SetHiliteHandler(MENU_ID_PHOART_ADD_ICON, mmi_phoart_hilight_add_icon);

	/* add text */
	SetHiliteHandler(MENU_ID_PHOART_ADD_TEXT, mmi_phoart_hilight_add_text);	
	SetHiliteHandler(MENU_ID_PHOART_TEXT_EDITOR_DONE, mmi_phoart_text_hilight_editor_done);
	SetHiliteHandler(MENU_ID_PHOART_TEXT_EDITOR_CANCEL, mmi_phoart_text_hilight_editor_cancel);	
	SetHiliteHandler(MENU_ID_PHOART_TEXT_EDITOR_INPUT, mmi_phoart_text_hilight_editor_input);

	/* adjustment */
	SetHiliteHandler(MENU_ID_PHOART_ADJUST_OPTION, mmi_phoart_adjust_hilight_option);
	SetHiliteHandler(MENU_ID_PHOART_ADJUST_IMAGE_ADJUST, mmi_phoart_adjust_hilight_image_adjust);	
	SetHiliteHandler(MENU_ID_PHOART_ADJUST_COLOR_ADJUST, mmi_phoart_adjust_hilight_color_adjust);		

	/* flip */
	SetHiliteHandler(MENU_ID_PHOART_FLIP, mmi_phoart_hilight_flip);
	
	/* expand canvas */
	SetHiliteHandler(MENU_ID_PHOART_EXPAND_CANVAS, mmi_phoart_hilight_expand_canvas);		
	
	/* help */
	SetHiliteHandler(MENU_ID_PHOART_HELP, mmi_phoart_hilight_help);	

	g_phoart_cntx.is_edited					= FALSE;
	g_phoart_cntx.is_lsk_pressed			= FALSE;
	g_phoart_cntx.is_rsk_pressed			= FALSE;	
	g_phoart_cntx.origin_image_buf		= NULL;
	g_phoart_cntx.interrupt_backup_buf	= NULL;
	
	g_phoart_cntx.text_color 			= *current_MMI_theme->title_text_color;
	g_phoart_cntx.text_border_color 	= *current_MMI_theme->title_text_border_color;

	g_phoart_cntx.adjust_image_index = 0;
	g_phoart_cntx.adjust_color_index = 0;	
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_app()
* DESCRIPTION
*   hilight photo art menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_app(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_phoart_entry_app_screen, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_entry_app_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_show_press_release_softkey()
* DESCRIPTION
*   draw and show softkey based on different screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_show_press_release_softkey(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   switch(GetExitScrnID())
	{
		case SCR_ID_PHOART_MAIN:
			{
				mmi_phoart_main_draw_osd();
			}
			break;

		case SCR_ID_PHOART_EFFECT_EDIT:
			{
				mmi_phoart_effect_edit_draw_osd();
			}
			break;
		case SCR_ID_PHOART_ADJUST_EDIT:			
			{
				if(g_phoart_cntx.adjust_type == PHOART_ADJUST_IMAGE_TYPE)
				{
					mmi_phoart_adjust_edit_image_draw_osd();
				}
				else if(g_phoart_cntx.adjust_type == PHOART_ADJUST_COLOR_TYPE)
				{
					mmi_phoart_adjust_edit_color_draw_osd();
				}
				else
				{
					MMI_ASSERT(0);
				}
			}
			break;

		case SCR_ID_PHOART_FLIP_EDIT:
			{
				mmi_phoart_flip_edit_draw_osd();
			}
			break;			

	
			
		case SCR_ID_PHOART_FRAME_EDIT:
			{
				mmi_phoart_frame_edit_draw_osd(TRUE);
			}
			break;

		case SCR_ID_PHOART_ICON_EDIT:
			{
				mmi_phoart_icon_edit_draw_osd();
			}
			break;

		case SCR_ID_PHOART_TEXT_EDIT:
			{
				mmi_phoart_text_edit_draw_osd();
			}
			break;

		default:
			MMI_ASSERT(0);
   	}

		gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);


}

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_lsk_press()
* DESCRIPTION
*   lsk press hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_lsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
  	g_phoart_cntx.is_lsk_pressed = TRUE;
	mmi_phoart_show_press_release_softkey();
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_lsk_release()
* DESCRIPTION
*   lsk press hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_lsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	PU8	buf_ptr;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_lsk_pressed = FALSE;
	mmi_phoart_show_press_release_softkey();

	/* sofkey release action */
   switch(GetExitScrnID())
	{
		case SCR_ID_PHOART_MAIN:
			{
				mmi_phoart_entry_option_screen();
			}
			break;

		case SCR_ID_PHOART_EFFECT_EDIT:	/* Done */
			{
				/* get base layer's buffer pointer */
				gdi_layer_get_buffer_ptr(&buf_ptr);

				if(mmi_phoart_write_cache_image(buf_ptr) == FALSE)
				{
					/* TODO: error handing */
				}

				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_INFO, 1, PHOEDT_NOTIFY_SHORT_DURATION, SUCCESS_TONE);
				DeleteUptoScrID(SCR_ID_PHOART_MAIN);	
			}
			break;		
			
		case SCR_ID_PHOART_ICON_EDIT:	
			{
				mmi_phoart_icon_edit_stamp_when_done();

				/* get base layer's buffer pointer */
				gdi_layer_get_buffer_ptr(&buf_ptr);

				if(mmi_phoart_write_cache_image(buf_ptr) == FALSE)
				{
					/* TODO: error handing */
				}

				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_INFO, 1, PHOEDT_NOTIFY_SHORT_DURATION, SUCCESS_TONE);
				DeleteUptoScrID(SCR_ID_PHOART_MAIN);					
			}
			break;
			
		case SCR_ID_PHOART_TEXT_EDIT:
			{
				mmi_phoart_text_edit_stamp_when_done();

				/* get base layer's buffer pointer */
				gdi_layer_get_buffer_ptr(&buf_ptr);

				if(mmi_phoart_write_cache_image(buf_ptr) == FALSE)
				{
					/* TODO: error handing */
				}

				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_INFO, 1, PHOEDT_NOTIFY_SHORT_DURATION, SUCCESS_TONE);
				DeleteUptoScrID(SCR_ID_PHOART_MAIN);					
			}
			break;

			

			
		case SCR_ID_PHOART_FLIP_EDIT:	
		case SCR_ID_PHOART_ADJUST_EDIT:			
			{
				/* get base layer's buffer pointer */
				gdi_layer_get_buffer_ptr(&buf_ptr);

				if(mmi_phoart_write_cache_image(buf_ptr) == FALSE)
				{
					/* TODO: error handing */
				}

				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_INFO, 1, PHOEDT_NOTIFY_SHORT_DURATION, SUCCESS_TONE);
				DeleteUptoScrID(SCR_ID_PHOART_MAIN);					
			}
			break;
			
		case SCR_ID_PHOART_FRAME_EDIT:	/* Done */
			{
				/* merge frame layer with base layer */
				
				/* draw frame alone on OSD layer*/	
				gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
				gdi_layer_clear(GDI_COLOR_TRANSPARENT);
				mmi_phoart_frame_edit_draw_frame();
				gdi_layer_pop_and_restore_active();

				g_phoart_cntx.resized_image_height 			= UI_device_height;
				g_phoart_cntx.resized_image_width 			= UI_device_width;
				g_phoart_cntx.resized_image_offset_x 		= 0;
				g_phoart_cntx.resized_image_offset_y 		= 0;	

				
				/* merge frame layer with base layer */
//				gdi_layer_flatten_to_base(g_phoart_cntx.base_layer_handle, g_phoart_cntx.osd_layer_handle, 0, 0);
				mmi_phoart_layer_flatten_to_base(g_phoart_cntx.base_layer_handle, g_phoart_cntx.osd_layer_handle, 0, 0);

				/* redraw other OSD components (title, softkey) except frame */
				mmi_phoart_frame_edit_draw_osd(FALSE);

				/* get base layer's buffer pointer */
				gdi_layer_get_buffer_ptr(&buf_ptr);

				/* dump to cache */
				if(mmi_phoart_write_cache_image(buf_ptr) == FALSE)
				{
					/* TODO: error handing */
				}

				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_INFO, 1, PHOEDT_NOTIFY_SHORT_DURATION, SUCCESS_TONE);
				DeleteUptoScrID(SCR_ID_PHOART_MAIN);
			}
			break;

		default:
			MMI_ASSERT(0);
	}
	
	
		
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_rsk_press()
* DESCRIPTION
*   lsk press hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_rsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	g_phoart_cntx.is_rsk_pressed = TRUE;
	mmi_phoart_show_press_release_softkey();

	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_rsk_release()
* DESCRIPTION
*   lsk press hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_rsk_release(void)
{	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_rsk_pressed = FALSE;
	g_phoart_cntx.is_user_goback = TRUE;

	mmi_phoart_show_press_release_softkey();

	/* sofkey release action */
	switch(GetExitScrnID())
	{
		case SCR_ID_PHOART_MAIN:
			{
				if(g_phoart_cntx.is_edited)
					mmi_phoart_exit_main_confirm();
				else
					GoBackHistory();
			}
			break;

		case SCR_ID_PHOART_EFFECT_EDIT:	/* cancel */
		case SCR_ID_PHOART_FRAME_EDIT:	
		case SCR_ID_PHOART_ICON_EDIT:
		case SCR_ID_PHOART_TEXT_EDIT:	
		case SCR_ID_PHOART_FLIP_EDIT:
		case SCR_ID_PHOART_ADJUST_EDIT:
			{
				DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_CANCELED), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_SHORT_DURATION, 0);
				DeleteNHistory(1);
			}
			break;

		default:
			MMI_ASSERT(0);	
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_end_key_press()
* DESCRIPTION
*   press end key hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_end_key_press(void)
{	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* 
		set this flag true, will free memory when exit photoart main screen
		(1) User directly press end key.
		(2) Clam close. Clam close will also call this function 
	*/
	g_phoart_cntx.is_free_memory = TRUE;

	/* call standand end-key handler */
	g_phoart_cntx.end_key_hdlr();
}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_draw_softkey()
* DESCRIPTION
*   lsk press hdlr
* PARAMETERS
*	 lsk_str				IN			lsk string
*	 rsk_str				IN			rsk string
*	 is_clear_bg 		IN			clear background flag
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_draw_softkey(PS8 lsk_str, PS8 rsk_str, BOOL is_clear_bg)
{	
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 str_width;
	S32 str_height;
	S32 offset_x;
	S32 offset_y;
	S32 x1;
	S32 x2;
	S32 y1;
	S32 y2;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	/* check if has softkey need update */
	if( (lsk_str==NULL) && (rsk_str==NULL) )
		return;
	gdi_layer_push_clip();
	gdi_layer_push_text_clip();
	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();	

	coolsand_UI_set_font(&MMI_medium_font);

	x1 = 0;
	y1 = UI_device_height-MMI_button_bar_height;
	x2 = UI_device_width-1;
	y2 = UI_device_height-1;

	/* clear softkey background */
	if(is_clear_bg)
		gdi_draw_solid_rect(x1, y1, x2, y2,	GDI_COLOR_TRANSPARENT);


	/* draw lsk */
	if(lsk_str!=NULL)
	{
		coolsand_UI_measure_string((UI_string_type)lsk_str, &str_width, &str_height);
		
		offset_x = 4;
		offset_y = (UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1);

		if(g_phoart_cntx.is_lsk_pressed)
		{
			offset_x++;
			offset_y++;	
		}

		if(r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(offset_x+str_width, offset_y);
		}
		else
		{
			coolsand_UI_move_text_cursor(offset_x, offset_y);
		}
		
		coolsand_UI_set_text_color(g_phoart_cntx.text_color);
		coolsand_UI_set_text_border_color(g_phoart_cntx.text_border_color);
	   coolsand_UI_print_bordered_text((UI_string_type)lsk_str);

	#ifdef __MMI_TOUCH_SCREEN__
		g_phoart_touch_screen_cntx.lsk.pos_x=offset_x;
		g_phoart_touch_screen_cntx.lsk.pos_y=offset_y;
		g_phoart_touch_screen_cntx.lsk.width=str_width;
		g_phoart_touch_screen_cntx.lsk.height=str_height;
	#endif
	}
#ifdef __MMI_TOUCH_SCREEN__
	else
	{
		g_phoart_touch_screen_cntx.lsk.pos_x=0;
		g_phoart_touch_screen_cntx.lsk.pos_y=0;
		g_phoart_touch_screen_cntx.lsk.width=0;
		g_phoart_touch_screen_cntx.lsk.height=0;

	}
#endif
	/* draw rsk */
	if(rsk_str!=NULL)
	{
		coolsand_UI_measure_string((UI_string_type)rsk_str, &str_width, &str_height);
		
		offset_x = UI_device_width-str_width-4;
		offset_y = (UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1);

		if(g_phoart_cntx.is_rsk_pressed)
		{
			offset_x++;
			offset_y++;	
		}

		if(r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(offset_x+str_width, offset_y);
		}
		else
		{
			coolsand_UI_move_text_cursor(offset_x, offset_y);
		}

		coolsand_UI_set_text_color(g_phoart_cntx.text_color);
		coolsand_UI_set_text_border_color(g_phoart_cntx.text_border_color);
	   coolsand_UI_print_bordered_text((UI_string_type)rsk_str);

	#ifdef __MMI_TOUCH_SCREEN__
		g_phoart_touch_screen_cntx.rsk.pos_x=offset_x;
		g_phoart_touch_screen_cntx.rsk.pos_y=offset_y;
		g_phoart_touch_screen_cntx.rsk.width=str_width;
		g_phoart_touch_screen_cntx.rsk.height=str_height;
	#endif
	}	
#ifdef __MMI_TOUCH_SCREEN__
	else
	{
		g_phoart_touch_screen_cntx.rsk.pos_x=0;
		g_phoart_touch_screen_cntx.rsk.pos_y=0;
		g_phoart_touch_screen_cntx.rsk.width=0;
		g_phoart_touch_screen_cntx.rsk.height=0;

	}
#endif
	
	gdi_layer_pop_clip();
	gdi_layer_pop_text_clip();
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_draw_title()
* DESCRIPTION
*   draw title
* PARAMETERS
*	 title_str		IN 		title string ptr
*	 sel_str			IN 		selection string ptr
*	 is_clear_bg	IN			clear background flag
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_draw_title(PS8 title_str, PS8 sel_str, BOOL is_clear_bg)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 	x1;
	S32 	x2;
	S32 	y1;
	S32 	y2;
	S32 	offset_x;
	S32 	offset_y;
	S32	str_width;
	S32	str_height;
	S32	arrow_size;
	U16   cur_screen;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	gdi_layer_push_clip();
	gdi_layer_push_text_clip();
	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();	

	
	x1 = 0;
	y1 = 0;
	x2 = UI_device_width-1;
	y2 = MMI_title_height*2;

	cur_screen = GetExitScrnID();

	coolsand_UI_set_font(&MMI_medium_font);
	coolsand_UI_set_text_color(g_phoart_cntx.text_color);
	coolsand_UI_set_text_border_color(g_phoart_cntx.text_border_color);
  
	/* clear background */
	if(is_clear_bg)
		gdi_draw_solid_rect(x1, y1, x2, y2,	GDI_COLOR_TRANSPARENT);

	coolsand_UI_measure_string((UI_string_type)title_str, &str_width, &str_height);
	
	offset_x = (UI_device_width-str_width)>>1;
	offset_y = (MMI_title_height-str_height)>>1;

	if(r2lMMIFlag)
	{
		coolsand_UI_move_text_cursor(offset_x+str_width, offset_y);
	}
	else
	{
		coolsand_UI_move_text_cursor(offset_x, offset_y);
	}

	coolsand_UI_print_bordered_text((UI_string_type)title_str);

	if(sel_str != NULL)
	{
		/* draw selection text */
		coolsand_UI_measure_string((UI_string_type)sel_str, &str_width, &str_height);
		
		offset_x = (UI_device_width-str_width)>>1;
		offset_y = MMI_title_height;

		if(r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(offset_x+str_width, offset_y);
		}
		else
		{
			coolsand_UI_move_text_cursor(offset_x, offset_y);
		}
		
		coolsand_UI_print_bordered_text((UI_string_type)sel_str);

		/* Draw Arrow */
		offset_y = MMI_title_height;
		offset_x = (UI_device_width>>1)-PHOART_MAIN_ARROW_FROM_CENTER-str_height;
		arrow_size = str_height-2;
		draw_resizeable_arrow(offset_x-1, offset_y, 0, g_phoart_cntx.text_border_color, arrow_size, arrow_size);		
		draw_resizeable_arrow(offset_x, offset_y, 0, g_phoart_cntx.text_border_color, arrow_size, arrow_size);	
		draw_resizeable_arrow(offset_x+1, offset_y+1, 0, g_phoart_cntx.text_color, arrow_size-2, arrow_size-2);
		
		#ifdef __MMI_TOUCH_SCREEN__
			g_phoart_touch_screen_cntx.prev_arrow.pos_x	= offset_x-1;
			g_phoart_touch_screen_cntx.prev_arrow.pos_y	= offset_y;
			g_phoart_touch_screen_cntx.prev_arrow.width	= arrow_size;
			g_phoart_touch_screen_cntx.prev_arrow.height	= arrow_size;
		#endif

		offset_x = (UI_device_width>>1)+PHOART_MAIN_ARROW_FROM_CENTER;
		draw_resizeable_arrow(offset_x+1, offset_y, 1, g_phoart_cntx.text_border_color, arrow_size, arrow_size);		
		draw_resizeable_arrow(offset_x, offset_y, 1, g_phoart_cntx.text_border_color, arrow_size, arrow_size);	
		draw_resizeable_arrow(offset_x+1, offset_y+1, 1, g_phoart_cntx.text_color, arrow_size-2, arrow_size-2);

		#ifdef __MMI_TOUCH_SCREEN__
			g_phoart_touch_screen_cntx.next_arrow.pos_x	= offset_x+1;
			g_phoart_touch_screen_cntx.next_arrow.pos_y	= offset_y;
			g_phoart_touch_screen_cntx.next_arrow.width	= arrow_size;
			g_phoart_touch_screen_cntx.next_arrow.height	= arrow_size;
		#endif

	}


	gdi_layer_pop_clip();
	gdi_layer_pop_text_clip();
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_write_cache_image()
* DESCRIPTION
*   write buffer to cache file
* PARAMETERS
*	 buf_ptr		IN		buffer pointer
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static BOOL mmi_phoart_write_cache_image(PU8 buf_ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	memcpy(g_phoart_cntx.origin_image_buf, buf_ptr, PHOART_FRAME_BUFFER_SIZE);
	return TRUE;

#if 0 
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_load_cache_image()
* DESCRIPTION
*   read buffer from cache file
* PARAMETERS
*	 buf_ptr		IN		buffer pointer
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static BOOL mmi_phoart_load_cache_image(PU8 buf_ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	memcpy(buf_ptr, g_phoart_cntx.origin_image_buf, PHOART_FRAME_BUFFER_SIZE);
	return TRUE;

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_draw_image_border()
* DESCRIPTION
*   draw image frame
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_draw_image_border(void)
{   
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32	frame_x1;
	S32	frame_y1;
	S32	frame_x2;
	S32	frame_y2;

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	gdi_layer_push_clip();
	gdi_layer_reset_clip();
	
	if((g_phoart_cntx.resized_image_width != UI_device_width) || 
		(g_phoart_cntx.resized_image_height != UI_device_height))
	{
		frame_x1 = g_phoart_cntx.resized_image_offset_x-1;
		frame_y1 = g_phoart_cntx.resized_image_offset_y-1;
		frame_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width;
		frame_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height;

		if(frame_x1 < 0)
			frame_x1 = 0;

		if(frame_y1 < 0)
			frame_y1 = 0;

		if(frame_x2 > UI_device_width-1)
		{
			frame_x2 = 	UI_device_width-1;
			g_phoart_cntx.resized_image_width-=1;			
		}

		if(frame_y2 > UI_device_height-1)
		{
			frame_y2 = 	UI_device_height-1;
			g_phoart_cntx.resized_image_height-=1;
		}

		if(g_phoart_cntx.resized_image_width != UI_device_height)
		{
			/* left V line */
			gdi_draw_line(	frame_x1, 
								frame_y1,
								frame_x1, 
								frame_y2,
								PHOART_MAIN_FRAME_COLOR);	

			/* right V line */
			gdi_draw_line(	frame_x2, 
								frame_y1,
								frame_x2, 
								frame_y2,
								PHOART_MAIN_FRAME_COLOR);		
		}

		if(g_phoart_cntx.resized_image_height != UI_device_width)
		{
			/* top H line */
			gdi_draw_line(	frame_x1, 
								frame_y1,
								frame_x2, 
								frame_y1,
								PHOART_MAIN_FRAME_COLOR);	

			/* bottom H line */
			gdi_draw_line(	frame_x1, 
								frame_y2,
								frame_x2, 
								frame_y2,
								PHOART_MAIN_FRAME_COLOR);		
		}

		

		gdi_draw_rect(	frame_x1, 
							frame_y1,
							frame_x2, 
							frame_y2,
							PHOART_MAIN_FRAME_COLOR);
	}

	gdi_layer_pop_clip();
	
}






/***************************************************************************** 
*
* File Selection Screen
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_app_screen()
* DESCRIPTION
*   entry photo art applcation
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_app_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	FMGR_FILTER filter;
	U8				*gui_buffer;
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}

	EntryNewScreen(SCR_ID_PHOART_SELECT_FILE, NULL, mmi_phoart_entry_app_screen, NULL);

	g_phoart_cntx.is_edit_from_fmgr = FALSE;

	gui_buffer 	= GetCurrGuiBuffer(SCR_ID_PHOART_CLIP_OPTION);
	
	FMGR_FILTER_INIT(&filter);

#ifdef WIN32 
	/* for MNT debug use */
	FMGR_FILTER_SET(&filter, FMGR_TYPE_GIF);
#endif 
	FMGR_FILTER_SET(&filter, FMGR_TYPE_JPG);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_JPEG);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);
	
	mmi_fmgr_select_path_and_enter(	APP_PHOEDT, 
												FMGR_SELECT_FILE|FMGR_SELECT_REPEAT, 
												filter, 
												(PS8)(S8*)L"root",
												(PsExtFuncPtr)mmi_phoart_fmgr_select_result_callback);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_fmgr_select_result_callback()
* DESCRIPTION
*   select file result callback function
* PARAMETERS
*	 filepath		IN			select file path
*	 is_shot			IN			is short naming
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_phoart_fmgr_select_result_callback(void *filepath, int is_short)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	FS_DiskInfo	disk_info;
   FS_HANDLE	fs_handle;
  	U64			free_space;
  	S8				drv_name[16];
  	S32			str_len;

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	str_len = UCS2Strlen((PS8)filepath);

	if(str_len + 3 > FMGR_PATH_CHAR_COUNT)
	{
		DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT),
						 IMG_GLOBAL_WARNING, 
						 1, 
						 PHOEDT_NOTIFY_DURATION, 
						 WARNING_TONE);
						 
		/* delete up to one level before file selection */
		DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
		DeleteNHistory(1);
		return;	
	}
   
	g_phoart_cntx.is_short_filename = is_short;
   
	if(filepath == NULL)
	{
		/* cencel file selection - delete up to one level before file selection */
		DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
		DeleteNHistory(1);
		GoBackHistory();
	}
	else
	{
		
	#ifdef __DRM_SUPPORT__
		/* check if it DRM file */
		fs_handle = DRM_open_file((PU16)filepath, FS_READ_ONLY, DRM_PERMISSION_NONE);

		if(fs_handle > FS_NO_ERROR)
		{
			if(DRM_get_rights(fs_handle) != NULL)
         {
				/* is DRM file, prohibit */
				/* no permission to display */
				DisplayPopup((PU8)GetString(STR_GLOBAL_DRM_PROHIBITED), IMG_GLOBAL_ERROR, 0, PHOEDT_NOTIFY_DURATION, ERROR_TONE);
				DeleteNHistory(1);		

				DRM_close_file(fs_handle);
				return;
			}

			/* not drm file */
			DRM_close_file(fs_handle);
		}


	#endif /* __DRM_SUPPORT__ */

		/* save file path to nvram */
		mmi_fmgr_util_store_filepath(filepath, FMGR_PATH_ID_PHOTO_EDITOR);

		memset(drv_name, 0, sizeof(drv_name));
		UCS2Strncpy((PS8)drv_name, (PS8)filepath, 3);
	
  		fs_handle = MMI_FS_GetDiskInfo((UINT8 *)drv_name, &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
		
		if(fs_handle >= 0)
		{
			/* in byte */
			free_space = disk_info.FreeClusters*disk_info.SectorsPerCluster*disk_info.BytesPerSector;

			/* free space not enoguth, display popup */
			if(free_space < PHOART_SAVE_PREDICT_SIZE)
			{
				DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_FREE_SPACE_NOT_ENOUGH), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);

				/* delete up to one level before file selection */
				DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
				DeleteNHistory(1);
				return;
			}

			/* check memory card lock */
			if(disk_info.WriteProtect)
			{
				DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_UNLOCK_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);

				/* delete up to one level before file selection */
				DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
				DeleteNHistory(1);
				return;
			}

			/* if no error - enter clip option screen */
			mmi_phoart_entry_clip_option_screen();
			
		}
		else
		{
			/* file system error */
			DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILE_SYSTEM_TEXT), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);

			/* delete up to one level before file selection */
			DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
			DeleteNHistory(1);
			return;
		}
	}
	
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_from_fmgr()
* DESCRIPTION
*   entry from file manager
* PARAMETERS
*	 filepath  	IN 		path to edit
*	 is_short	IN			is short file name
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_phoart_entry_from_fmgr(void *filepath, BOOL is_short)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	FS_DiskInfo	disk_info;
   FS_HANDLE	fs_handle;
  	U64			free_space;
  	S8				drv_name[16];
  	S32			str_len;

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	str_len = UCS2Strlen((PS8)filepath);

	if(str_len + 3 > FMGR_PATH_CHAR_COUNT)
	{
		DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT), 
						 IMG_GLOBAL_WARNING, 
						 1, 
						 PHOEDT_NOTIFY_DURATION, 
						 WARNING_TONE);
		return;	
	}
   
	g_phoart_cntx.is_edit_from_fmgr = TRUE;
	g_phoart_cntx.is_short_filename = is_short;	
 
	/* save file path to nvram */
	mmi_fmgr_util_store_filepath(filepath, FMGR_PATH_ID_PHOTO_EDITOR);

	memset(drv_name, 0, sizeof(drv_name));
	UCS2Strncpy((PS8)drv_name, (PS8)filepath, 3);

	fs_handle = MMI_FS_GetDiskInfo((UINT8 *)drv_name, &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
	
	if(fs_handle >= 0)
	{
		/* in byte */
		free_space = disk_info.FreeClusters*disk_info.SectorsPerCluster*disk_info.BytesPerSector;

		/* free space not enoguth, display popup */
		if(free_space < PHOART_SAVE_PREDICT_SIZE)
		{
			DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_FREE_SPACE_NOT_ENOUGH), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
			return;
		}


		if(disk_info.WriteProtect)
		{
			DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_UNLOCK_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
			return;
		}

		/* if no error - enter clip option screen */
		mmi_phoart_entry_clip_option_screen();
		
	}
	else
	{
		/* file system error */
		DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILE_SYSTEM_TEXT), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
		return;
	}
	
}



/***************************************************************************** 
*
* Clip Option Screen
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_clip_option_screen()
* DESCRIPTION
*   clip option screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_clip_option_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[16];
	U16	item_icons[16];
	U16	num_item;
	U8		*gui_buffer;
	U8		*popup_list[16];
	S32	i;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_CLIP_OPTION, NULL, mmi_phoart_entry_clip_option_screen, NULL);

	g_phoart_cntx.is_clip = FALSE;

	gui_buffer 	= GetCurrGuiBuffer(SCR_ID_PHOART_CLIP_OPTION);
	num_item		= GetNumOfChild(MENU_ID_PHOART_CLIP);
	
	GetSequenceStringIds(MENU_ID_PHOART_CLIP, item_list);
	GetSequenceImageIds(MENU_ID_PHOART_CLIP, item_icons);
	SetParentHandler(MENU_ID_PHOART_CLIP);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i = 0; i < num_item; i++)
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);

	for(i = 0 ; i < num_item; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_PHOART_CLIP, popup_list);

	ShowCategory52Screen(STR_ID_PHOART_CLIP,	
								MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_item, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								gui_buffer);


	SetRightSoftkeyFunction(mmi_phoart_back_from_clip_option_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_back_from_clip_option_screen, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_back_from_clip_option_screen()
* DESCRIPTION
*   back from clip option screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_back_from_clip_option_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	GoBackHistory();
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_auto_resize()
* DESCRIPTION
*   auto resize to fit mainlcd size
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_auto_resize(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.clip_type = PHOART_AUTO_RESIZE; 
   
	SetLeftSoftkeyFunction(mmi_phoart_entry_main_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_entry_main_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_mainlcd_clip()
* DESCRIPTION
*   clip image to fit mainlcd size
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_mainlcd_clip(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.clip_type = PHOART_MAINLCD_CLIP;

	SetLeftSoftkeyFunction(mmi_phoart_entry_clip_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_entry_clip_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);   
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_sublcd_clip()
* DESCRIPTION
*   clip image to fit sublcd size
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
static void mmi_phoart_hilight_sublcd_clip(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.clip_type = PHOART_SUBLCD_CLIP;  

  	SetLeftSoftkeyFunction(mmi_phoart_entry_clip_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_entry_clip_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);    
}
#endif /* __MMI_SUBLCD__ */

 



/***************************************************************************** 
*
* Clip Screen
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_clip_screen()
* DESCRIPTION
*   enter clip screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_clip_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8				buf_filename[FMGR_PATH_BUFFER_SIZE];
   S32			display_wnd_width;
   S32			display_wnd_height;
   U32			scale_ratio;
   GDI_RESULT 	result;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
   
	/* init variable */
	g_phoart_cntx.scale_ratio 					= PHOART_CLIP_RECT_START_RATIO;
	g_phoart_cntx.is_up_arrow_pressed 		= FALSE;
	g_phoart_cntx.is_down_arrow_pressed 	= FALSE;
	g_phoart_cntx.is_left_arrow_pressed 	= FALSE;
	g_phoart_cntx.is_right_arrow_pressed 	= FALSE;

   
	EntryNewScreen(SCR_ID_PHOART_CLIP, mmi_phoart_exit_clip_screen, mmi_phoart_entry_clip_screen, NULL);

	gdi_layer_lock_frame_buffer();
	
	if(g_phoart_cntx.clip_type == PHOART_MAINLCD_CLIP)
	{
		ShowCategory221Screen(	STR_ID_PHOART_MAINLCD_CLIP, 0,
										STR_GLOBAL_OK,IMG_GLOBAL_OK,
										STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
										PHOART_CLIP_CANVAS_COLOR,
										NULL);
		
		gdi_lcd_get_dimension(&g_phoart_cntx.clip_size_width, &g_phoart_cntx.clip_size_height);
	}
	else /* Sub */
	{
#ifdef __MMI_SUBLCD__
		ShowCategory221Screen(	STR_ID_PHOART_SUBLCD_CLIP, 0,
										STR_GLOBAL_OK,IMG_GLOBAL_OK,
										STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
										PHOART_CLIP_CANVAS_COLOR,
										NULL);

		gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
		gdi_lcd_get_dimension(&g_phoart_cntx.clip_size_width, &g_phoart_cntx.clip_size_height);
		gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE)	;
#else
		MMI_ASSERT(0);
#endif /* __MMI_SUBLCD__ */

	}
 
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_phoart_clip_clip_image,KEY_EVENT_UP);
	
	SetKeyHandler(mmi_phoart_clip_up_arrow_press,		KEY_UP_ARROW, 		KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_clip_down_arrow_press,		KEY_DOWN_ARROW, 	KEY_EVENT_DOWN);		
	SetKeyHandler(mmi_phoart_clip_left_arrow_press,		KEY_LEFT_ARROW, 	KEY_EVENT_DOWN);			
	SetKeyHandler(mmi_phoart_clip_right_arrow_press,	KEY_RIGHT_ARROW, 	KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_clip_up_arrow_press,		KEY_2, 				KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_clip_down_arrow_press,		KEY_8, 				KEY_EVENT_DOWN);		
	SetKeyHandler(mmi_phoart_clip_left_arrow_press,		KEY_4, 				KEY_EVENT_DOWN);			
	SetKeyHandler(mmi_phoart_clip_right_arrow_press,	KEY_6, 				KEY_EVENT_DOWN);	
	
	SetKeyHandler(mmi_phoart_clip_up_arrow_release,		KEY_UP_ARROW, 		KEY_EVENT_UP);	
	SetKeyHandler(mmi_phoart_clip_down_arrow_release,	KEY_DOWN_ARROW, 	KEY_EVENT_UP);		
	SetKeyHandler(mmi_phoart_clip_left_arrow_release,	KEY_LEFT_ARROW, 	KEY_EVENT_UP);			
	SetKeyHandler(mmi_phoart_clip_right_arrow_release,	KEY_RIGHT_ARROW, 	KEY_EVENT_UP);	
	SetKeyHandler(mmi_phoart_clip_up_arrow_release,		KEY_2, 				KEY_EVENT_UP);	
	SetKeyHandler(mmi_phoart_clip_down_arrow_release,	KEY_8, 				KEY_EVENT_UP);		
	SetKeyHandler(mmi_phoart_clip_left_arrow_release,	KEY_4, 				KEY_EVENT_UP);			
	SetKeyHandler(mmi_phoart_clip_right_arrow_release,	KEY_6, 				KEY_EVENT_UP);	

	SetKeyHandler(mmi_phoart_clip_scale_up,	KEY_POUND,	KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_clip_scale_down,	KEY_STAR, 	KEY_EVENT_DOWN);


	/***************************************************************************** 
	* Layers
	*****************************************************************************/
	gdi_layer_multi_layer_enable();
	
	/* base layer */
	gdi_layer_get_active(&g_phoart_cntx.base_layer_handle);

	/* clip rect layer */
	gdi_layer_create(0, 0, UI_device_width, UI_device_height, &g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();
	

	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, g_phoart_cntx.osd_layer_handle, 0, 0);


	/* get file name from NVRAM */
	mmi_fmgr_util_load_filepath(buf_filename, FMGR_PATH_ID_PHOTO_EDITOR);
	 
 
	/* draw image */
	result = gdi_image_get_dimension_file(	(PS8)buf_filename, 
														&g_phoart_cntx.image_width, 
														&g_phoart_cntx.image_height);
	
	if((result < 0) || (g_phoart_cntx.image_width <= 0) || (g_phoart_cntx.image_height <= 0))
	{
		gdi_layer_unlock_frame_buffer();
		gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	
		DisplayPopup((PU8)GetString(STR_FMGR_INVALID_FORMAT), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
		DeleteNHistory(1);
		return;
	}

	display_wnd_width 	= UI_device_width-PHOART_CLIP_SCREEN_SPACING*2;
	display_wnd_height 	= MMI_content_height-PHOART_CLIP_SCREEN_SPACING*2;
	mmi_trace(1," mmi_phoart_entry_clip_screen (%d,%d,%d,%d,%d,%d,%d,%d)",display_wnd_width, 
										display_wnd_height, 
										g_phoart_cntx.image_width, 
										g_phoart_cntx.image_height,
										g_phoart_cntx.resized_image_offset_x, 
										g_phoart_cntx.resized_image_offset_y,
										g_phoart_cntx.resized_image_width, 
										g_phoart_cntx.resized_image_height);
	gdi_layer_reset_clip();
	if( (g_phoart_cntx.image_width > display_wnd_width) ||  (g_phoart_cntx.image_height > display_wnd_height))
	{
		gdi_image_util_fit_bbox(display_wnd_width, 
										display_wnd_height, 
										g_phoart_cntx.image_width, 
										g_phoart_cntx.image_height,
										&g_phoart_cntx.resized_image_offset_x, 
										&g_phoart_cntx.resized_image_offset_y,
										&g_phoart_cntx.resized_image_width, 
										&g_phoart_cntx.resized_image_height);

		if(g_phoart_cntx.resized_image_width == 0)
		{
			g_phoart_cntx.resized_image_width = 1;
			g_phoart_cntx.resized_image_offset_x--;
		}

		if(g_phoart_cntx.resized_image_height == 0)
		{
			g_phoart_cntx.resized_image_height = 1;
			g_phoart_cntx.resized_image_offset_y--;
		}
	
		scale_ratio = ((g_phoart_cntx.resized_image_offset_x)<<10) / g_phoart_cntx.image_width;

		g_phoart_cntx.resized_image_offset_x += PHOART_CLIP_SCREEN_SPACING;
		g_phoart_cntx.resized_image_offset_y += MMI_title_height+MMI_title_y+PHOART_CLIP_SCREEN_SPACING;
	
		result = gdi_image_draw_resized_file(	g_phoart_cntx.resized_image_offset_x, 
															g_phoart_cntx.resized_image_offset_y, 
															g_phoart_cntx.resized_image_width, 
															g_phoart_cntx.resized_image_height, 
															buf_filename);

		if(result < 0)
		{
			gdi_layer_unlock_frame_buffer();
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

			DisplayPopup((PU8)GetString(STR_FMGR_INVALID_FORMAT), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
			DeleteNHistory(1);
			return;
		}
	}
	else
	{
		scale_ratio = 1<<10;

		g_phoart_cntx.resized_image_width 	= g_phoart_cntx.image_width;
		g_phoart_cntx.resized_image_height 	= g_phoart_cntx.image_height;
			
		g_phoart_cntx.resized_image_offset_x = ((display_wnd_width-g_phoart_cntx.image_width)>>1)+PHOART_CLIP_SCREEN_SPACING; 
		g_phoart_cntx.resized_image_offset_y = ((display_wnd_height-g_phoart_cntx.image_height)>>1)+MMI_title_height+MMI_title_y+PHOART_CLIP_SCREEN_SPACING;
		
		gdi_image_draw_file(g_phoart_cntx.resized_image_offset_x, g_phoart_cntx.resized_image_offset_y, buf_filename);
	}

	/* draw a border for image to make it looks better */
	gdi_draw_rect(	g_phoart_cntx.resized_image_offset_x-1, 
						g_phoart_cntx.resized_image_offset_y-1, 
						g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width,
						g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height,
						PHOART_CLIP_FRAME_INSIDE_COLOR);

	gdi_draw_rect(	g_phoart_cntx.resized_image_offset_x-2, 
						g_phoart_cntx.resized_image_offset_y-2, 
						g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width+1,
						g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height+1,
						PHOART_CLIP_FRAME_OUTSIDE_COLOR);


	/* fit clip rect into image */
	gdi_image_util_fit_bbox(g_phoart_cntx.resized_image_width, 
									g_phoart_cntx.resized_image_height, 
									g_phoart_cntx.clip_size_width, 
									g_phoart_cntx.clip_size_height,
									&g_phoart_cntx.clip_rect_center_x,
									&g_phoart_cntx.clip_rect_center_y,
									&g_phoart_cntx.clip_rect_width, 
									&g_phoart_cntx.clip_rect_height);

	
	g_phoart_cntx.clip_rect_center_x += (g_phoart_cntx.clip_rect_width>>1)+g_phoart_cntx.resized_image_offset_x;
	g_phoart_cntx.clip_rect_center_y += (g_phoart_cntx.clip_rect_height>>1)+g_phoart_cntx.resized_image_offset_y;
	mmi_trace(1," mmi_phoart_entry_clip_screen 111(%d,%d,%d,%d,%d,%d,%d,%d)",display_wnd_width, 
										display_wnd_height, 
										g_phoart_cntx.image_width, 
										g_phoart_cntx.image_height,
										g_phoart_cntx.resized_image_offset_x, 
										g_phoart_cntx.resized_image_offset_y,
										g_phoart_cntx.resized_image_width, 
										g_phoart_cntx.resized_image_height);

	mmi_phoart_clip_draw_clip_rect();

	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_clip_screen()
* DESCRIPTION
*   enter clip screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_clip_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   /* cancel timer */
   coolsand_UI_cancel_timer(mmi_phoart_clip_move_clip_rect_cyclic);   

   /* free layer */
	gdi_layer_flatten_previous_to_base();
   
	gdi_layer_free(g_phoart_cntx.osd_layer_handle);
  	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, 0, 0, 0);
  	gdi_layer_multi_layer_disable();	
}


 
/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_draw_clip_rect()
* DESCRIPTION
*   draw clip retangle
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_draw_clip_rect(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   S32	rect_x1;
   S32	rect_y1;
   S32	rect_x2;
   S32	rect_y2;
   S32	x_shift;
   S32 	y_shift;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);

	x_shift = (g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100;
	y_shift = (g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100;

	rect_x1 = g_phoart_cntx.clip_rect_center_x-x_shift;
	rect_y1 = g_phoart_cntx.clip_rect_center_y-y_shift;
	rect_x2 = g_phoart_cntx.clip_rect_center_x+x_shift;
	rect_y2 = g_phoart_cntx.clip_rect_center_y+y_shift;

	/* rectangle maybe have 1 pixel shifting, make it align to image */
	if(rect_x2 >= g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width)
		rect_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1;

	if(rect_y2 >= g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height)
		rect_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1;
	
	gdi_draw_rect(rect_x1, rect_y1, rect_x2, rect_y2, GDI_COLOR_RED);

	gdi_layer_pop_and_restore_active();	

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_move_clip_rect_cyclic()
* DESCRIPTION
*   move clip retangle
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_move_clip_rect_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   S32	move_step;
   S32  	diff;
   S32 	image_x1;
   S32 	image_x2;
   S32 	image_y1;
   S32 	image_y2; 
   S32 	rect_x1;
   S32 	rect_x2;
   S32 	rect_y1;
   S32 	rect_y2;

   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	g_phoart_cntx.move_count++;

   if(g_phoart_cntx.move_count < 5)
   	move_step = 1;
   else if(g_phoart_cntx.move_count < 7)
   	move_step = 3;
   else if(g_phoart_cntx.move_count < 10)
   	move_step = 5;
   else if(g_phoart_cntx.move_count < 15)
   	move_step = 7; 
   else
   	move_step = 10;

	image_x1 = g_phoart_cntx.resized_image_offset_x;
	image_y1 = g_phoart_cntx.resized_image_offset_y;	
	image_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1;
	image_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1;	

   rect_x1	= g_phoart_cntx.clip_rect_center_x-(g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100;
   rect_y1	= g_phoart_cntx.clip_rect_center_y-(g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100;
   rect_x2	= g_phoart_cntx.clip_rect_center_x+(g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100;
   rect_y2	= g_phoart_cntx.clip_rect_center_y+(g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100;

   
   if(g_phoart_cntx.is_up_arrow_pressed)
  	{
  		if(rect_y1 > image_y1)
  		{
			diff = (rect_y1-move_step)-image_y1;
				
			if(diff > 0)
				g_phoart_cntx.clip_rect_center_y -= move_step;
			else
				g_phoart_cntx.clip_rect_center_y -= move_step+diff;

			mmi_phoart_clip_draw_clip_rect(); 
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
  		}
   }
   else if(g_phoart_cntx.is_down_arrow_pressed)
   {
    	if(rect_y2 < image_y2)
  		{ 
			diff = image_y2-(rect_y2+move_step);
		   
			if(diff > 0)
				g_phoart_cntx.clip_rect_center_y += move_step;
			else
				g_phoart_cntx.clip_rect_center_y += move_step+diff;

			mmi_phoart_clip_draw_clip_rect(); 
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);	
    	}
   }
   else if(g_phoart_cntx.is_left_arrow_pressed)
  	{
   	if(rect_x1 > image_x1)
  		{	
			diff = (rect_x1-move_step)-image_x1;
		   
			if(diff > 0)
				g_phoart_cntx.clip_rect_center_x -= move_step;
			else
				g_phoart_cntx.clip_rect_center_x -= move_step+diff;

			mmi_phoart_clip_draw_clip_rect(); 
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);		
   	}
  	}
   else if(g_phoart_cntx.is_right_arrow_pressed)
   {
    	if(rect_x2 < image_x2)
  		{    
			diff = image_x2-(rect_x2+move_step);
		   
			if(diff > 0)
				g_phoart_cntx.clip_rect_center_x += move_step;
			else
				g_phoart_cntx.clip_rect_center_x += move_step+diff;

			mmi_phoart_clip_draw_clip_rect(); 
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);		
    	}

   }
   else
   {
		MMI_ASSERT(0);
   }

   coolsand_UI_start_timer(100, mmi_phoart_clip_move_clip_rect_cyclic);

}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_up_arrow_press()
* DESCRIPTION
*   move clip region up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_up_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	if(!g_phoart_cntx.is_up_arrow_pressed)
	{
		g_phoart_cntx.is_up_arrow_pressed 	= TRUE;
		g_phoart_cntx.move_count 				= 0;
   
 		mmi_phoart_clip_move_clip_rect_cyclic();
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_down_arrow_press()
* DESCRIPTION
*   move clip region down
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_down_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	if(!g_phoart_cntx.is_down_arrow_pressed)
	{
		g_phoart_cntx.is_down_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0;
		
  		mmi_phoart_clip_move_clip_rect_cyclic();
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_left_arrow_press()
* DESCRIPTION
*   move clip region left
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_left_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_left_arrow_pressed)
	{   
		g_phoart_cntx.is_left_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0; 
		
		mmi_phoart_clip_move_clip_rect_cyclic(); 
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_right_arrow_press()
* DESCRIPTION
*   move clip region right
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_right_arrow_press(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_right_arrow_pressed)
	{
		g_phoart_cntx.is_right_arrow_pressed 	= TRUE;
		g_phoart_cntx.move_count 					= 0;
	
	 	mmi_phoart_clip_move_clip_rect_cyclic();
	}
}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_up_arrow_release()
* DESCRIPTION
*   move clip region up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_up_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	g_phoart_cntx.is_up_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_clip_move_clip_rect_cyclic);      
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_down_arrow_release()
* DESCRIPTION
*   move clip region down
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_down_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	g_phoart_cntx.is_down_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_clip_move_clip_rect_cyclic);      
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_left_arrow_release()
* DESCRIPTION
*   move clip region left
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_left_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_left_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_clip_move_clip_rect_cyclic);   
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_right_arrow_release()
* DESCRIPTION
*   stop move clip region right
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_right_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	g_phoart_cntx.is_right_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_clip_move_clip_rect_cyclic);   
}

 
 


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_scale_up()
* DESCRIPTION
*   increase rect size
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_scale_up(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 x_min;
  	S32 x_max; 
  	S32 y_min; 
  	S32 y_max; 	

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	if(g_phoart_cntx.scale_ratio < 100)
	{
		g_phoart_cntx.scale_ratio += 10;

		y_min = g_phoart_cntx.clip_rect_center_y-(g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100;
		y_max = g_phoart_cntx.clip_rect_center_y+(g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100;
		x_min = g_phoart_cntx.clip_rect_center_x-(g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100;
		x_max = g_phoart_cntx.clip_rect_center_x+(g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100;

		if(y_min < g_phoart_cntx.resized_image_offset_y)
			g_phoart_cntx.clip_rect_center_y += g_phoart_cntx.resized_image_offset_y-y_min;	
	
		if(y_max > g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height)
			g_phoart_cntx.clip_rect_center_y -= y_max-(g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height);

		if(x_min < g_phoart_cntx.resized_image_offset_x)
			g_phoart_cntx.clip_rect_center_x += g_phoart_cntx.resized_image_offset_x-x_min;	
	
		if(x_max > g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width)
			g_phoart_cntx.clip_rect_center_x -= x_max-(g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width);
		
  		mmi_phoart_clip_draw_clip_rect(); 
		
		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_clip_scale_down()
* DESCRIPTION
*   decreasee rect size
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_clip_scale_down(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 x_min;
  	S32 x_max; 
  	S32 y_min; 
  	S32 y_max; 	

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/		
	if(g_phoart_cntx.scale_ratio > PHOART_CLIP_RECT_MIN_RATIO)
	{
		g_phoart_cntx.scale_ratio -= 10;
		
		y_min = g_phoart_cntx.clip_rect_center_y-(g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100;
		y_max = g_phoart_cntx.clip_rect_center_y+(g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100;
		x_min = g_phoart_cntx.clip_rect_center_x-(g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100;
		x_max = g_phoart_cntx.clip_rect_center_x+(g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100;

		if(y_min < g_phoart_cntx.resized_image_offset_y)
			g_phoart_cntx.clip_rect_center_y += g_phoart_cntx.resized_image_offset_y-y_min;	
	
		if(y_max > g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height)
			g_phoart_cntx.clip_rect_center_y -= y_max-(g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height);

		if(x_min < g_phoart_cntx.resized_image_offset_x)
			g_phoart_cntx.clip_rect_center_x += g_phoart_cntx.resized_image_offset_x-x_min;	
	
		if(x_max > g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width)
			g_phoart_cntx.clip_rect_center_x -= x_max-(g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width);
		
  		mmi_phoart_clip_draw_clip_rect(); 

		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);
	}
}


/*****************************************************************************
 * FUNCTION
 *  mmi_phoart_clip_clip_image
 * DESCRIPTION
 *  prepare clip para and enter main screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_phoart_clip_clip_image(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
//	S32 scale_ratio;
//	S32 resized_width;
//	S32 resized_height;
	S32 x1, y1, x2, y2;
	S32 clip_width;
	S32 clip_height;
//	S32 shift;

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	/* 
		resized_image_width	   = image width on clip screen
		resized_image_height		= image height on clip screen
		clip_rect_width		 	= image clip width on clip screen		
		clip_rect_height		 	= image clip height on clip screen	
		image_width					= original image width	 
		image_height				= original image height
	*/

   #if 1
	g_phoart_cntx.is_clip = TRUE;

	/* calculate how large the image will be after resize */
	clip_width 		= g_phoart_cntx.clip_rect_width*g_phoart_cntx.scale_ratio/100;
	clip_height	 	= g_phoart_cntx.clip_rect_height*g_phoart_cntx.scale_ratio/100;

	if(clip_width == 0)
		clip_width = 1;

	if(clip_height ==0)
		clip_height = 1;
	


	/* calc offset of clip region on resized image */
	x1 = g_phoart_cntx.clip_rect_center_x-(clip_width>>1)-g_phoart_cntx.resized_image_offset_x;
	x2 = g_phoart_cntx.clip_rect_center_x+(clip_width>>1)-g_phoart_cntx.resized_image_offset_x;

	y1 = g_phoart_cntx.clip_rect_center_y-(clip_height>>1)-g_phoart_cntx.resized_image_offset_y; 
	y2 = g_phoart_cntx.clip_rect_center_y+(clip_height>>1)-g_phoart_cntx.resized_image_offset_y; 
	
	/* scale up offset */
	g_phoart_cntx.clip_x1 = x1;
	g_phoart_cntx.clip_x2 = x2;
	g_phoart_cntx.clip_y1 = y1;
	g_phoart_cntx.clip_y2 = y2;
	g_phoart_cntx.resized_image_width 	= clip_width;
	g_phoart_cntx.resized_image_height 	= clip_height;	


	/* 
		there may have rounding problem, that the clip region maybe outside of image region,
		we re-align it back to image region 
	*/
	mmi_trace(1," g_phoart_cntx.clip_x(x1,y1,x2,y2,width,height)(%d %d %d %d,%d,%d)",x1,y1,x2,y2,g_phoart_cntx.resized_image_width,g_phoart_cntx.resized_image_height);

	#else
	
	g_phoart_cntx.is_clip = TRUE;

	/* calculate how large the image will be after resize */
	clip_width 		= g_phoart_cntx.clip_rect_width*g_phoart_cntx.scale_ratio/100;
	clip_height	 	= g_phoart_cntx.clip_rect_height*g_phoart_cntx.scale_ratio/100;

	if(clip_width == 0)
		clip_width = 1;

	if(clip_height ==0)
		clip_height = 1;
	
	resized_width 	= g_phoart_cntx.resized_image_width*g_phoart_cntx.clip_size_width/clip_width;

	/* calculate scale ratio and resize image height */
	scale_ratio		 = resized_width*100/g_phoart_cntx.image_width;
	resized_height	= g_phoart_cntx.image_height*scale_ratio/100;

	/* calc offset of clip region on resized image */
	x1 = g_phoart_cntx.clip_rect_center_x-(clip_width>>1)-g_phoart_cntx.resized_image_offset_x;
	x2 = g_phoart_cntx.clip_rect_center_x+(clip_width>>1)-g_phoart_cntx.resized_image_offset_x;

	y1 = g_phoart_cntx.clip_rect_center_y-(clip_height>>1)-g_phoart_cntx.resized_image_offset_y; 
	y2 = g_phoart_cntx.clip_rect_center_y+(clip_height>>1)-g_phoart_cntx.resized_image_offset_y; 
	
	/* scale up offset */
	scale_ratio = g_phoart_cntx.clip_size_width*100/clip_width;
	x1 *= scale_ratio;
	x1 /= 100;

	y1 *= scale_ratio;
	y1 /= 100;

	x2 = x1+g_phoart_cntx.clip_size_width-1;
	y2 = y1+g_phoart_cntx.clip_size_height-1;

	g_phoart_cntx.clip_x1 = x1;
	g_phoart_cntx.clip_x2 = x2;
	g_phoart_cntx.clip_y1 = y1;
	g_phoart_cntx.clip_y2 = y2;
	
	g_phoart_cntx.resized_image_width 	= resized_width;
	g_phoart_cntx.resized_image_height 	= resized_height;	

	/* 
		there may have rounding problem, that the clip region maybe outside of image region,
		we re-align it back to image region 
	*/
	mmi_trace(1,"guoj++ g_phoart_cntx.clip_x(x1,y1,x2,y2,width,height)(%d %d %d %d,%d,%d)",x1,y1,x2,y2,resized_width,resized_height);
	if(g_phoart_cntx.clip_x1 < 0)
	{
		shift = -g_phoart_cntx.clip_x1;
		g_phoart_cntx.clip_x1 = 0;
		g_phoart_cntx.clip_x2 += shift;

		if(g_phoart_cntx.clip_x2 > g_phoart_cntx.resized_image_width-1)
		{
			g_phoart_cntx.clip_x2 = g_phoart_cntx.resized_image_width-1;
		}
	}
	
	if(g_phoart_cntx.clip_x2 > g_phoart_cntx.resized_image_width-1)
	{
		shift = g_phoart_cntx.clip_x2 - (g_phoart_cntx.resized_image_width-1);
		g_phoart_cntx.clip_x2 = (g_phoart_cntx.resized_image_width-1);
		g_phoart_cntx.clip_x1 -= shift;
		
		if(g_phoart_cntx.clip_x1 < 0)
		{
			g_phoart_cntx.clip_x1 = 0;
		}
	}
	
	if(g_phoart_cntx.clip_y1 < 0)
	{
		shift = -g_phoart_cntx.clip_y1;
		g_phoart_cntx.clip_y1 = 0;
		g_phoart_cntx.clip_y2 += shift;

		if(g_phoart_cntx.clip_y2 > g_phoart_cntx.resized_image_height-1)
		{
			g_phoart_cntx.clip_y2 = g_phoart_cntx.resized_image_height-1;
		}
	}
	
	if(g_phoart_cntx.clip_y2 > g_phoart_cntx.resized_image_height-1)
	{
		shift = g_phoart_cntx.clip_y2 - (g_phoart_cntx.resized_image_height-1);
		g_phoart_cntx.clip_y2 = (g_phoart_cntx.resized_image_height-1);
		g_phoart_cntx.clip_y1 -= shift;
		
		if(g_phoart_cntx.clip_y1 < 0)
		{
			g_phoart_cntx.clip_y1 = 0;
		}
	}

	#endif
	/* enter main screen */
	mmi_phoart_entry_main_screen();
}


/***************************************************************************** 
*
* Main Screen
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_main_screen()
* DESCRIPTION
*   enter main screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_main_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8					buf_filename[FMGR_PATH_BUFFER_SIZE];
   S8 tmpfilename[FMGR_PATH_BUFFER_SIZE];
   S32				image_width;
   S32				image_height;
 	PU8				buf_ptr;
   U8					*gui_buffer;
   GDI_RESULT 		result = 0;
	S32 				offset_x = 0;
	S32				offset_y = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_MAIN, mmi_phoart_exit_main_screen, mmi_phoart_entry_main_screen, NULL);
	
	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	coolsand_UI_set_font(&MMI_medium_font);
	
	gui_buffer = GetCurrGuiBuffer(SCR_ID_PHOART_MAIN);

	/* load data from nvram */
	mmi_phoedt_load_data();

	/* set del screen handler */
	SetDelScrnIDCallbackHandler(SCR_ID_PHOART_MAIN, mmi_phoart_del_screen_id_hdlr);


	if(gui_buffer == NULL)
	{
		#ifdef APPMEM_SUPPORT
			/* allocate memory if ASM is used */
//			g_phoart_cntx.origin_image_buf = kal_app_mem_alloc(PHOART_FRAME_BUFFER_SIZE, KAL_APPMEM_APP, MOD_MMI); 
			g_phoart_cntx.origin_image_buf = (PU8)(AllocMediaMemory(PHOART_FRAME_BUFFER_SIZE)); 
		#else
			/* use camera's buffer */
			g_phoart_cntx.origin_image_buf = (PU8)camera_osd_layer_buffer;
		#endif
	}
	

	/* image is edited */
	g_phoart_cntx.is_edited		 	= TRUE;

	/* default set to do not free memory, only end key and clam-close or self go back will free */
	g_phoart_cntx.is_free_memory 	= FALSE;		
	
	/* entry full screen */
	entry_full_screen();

	/* register key hdlr */
	SetKeyHandler(mmi_phoart_lsk_press,	 		KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_lsk_release, 		KEY_LSK, KEY_EVENT_UP);	

	SetKeyHandler(mmi_phoart_rsk_press, 		KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_rsk_release, 		KEY_RSK, KEY_EVENT_UP);	

	/* get end key hdlr and hook ours */
	g_phoart_cntx.end_key_hdlr = GetKeyHandler (KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler(mmi_phoart_end_key_press, 	KEY_END, KEY_EVENT_DOWN);	

	/***************************************************************************** 
	* Layers
	*****************************************************************************/
	gdi_layer_multi_layer_enable();
	
	/* base layer */
	gdi_layer_get_active(&g_phoart_cntx.base_layer_handle);

	/* osd layer */
	gdi_layer_create(0, 0, UI_device_width, UI_device_height, &g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();
	
	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, g_phoart_cntx.osd_layer_handle, 0, 0);
	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	/* lock frame for drawing */
	gdi_layer_lock_frame_buffer();

	if(gui_buffer == NULL)
	{
		/* newly entry - start to edit a new image */
		
		/* get file name from NVRAM */
		mmi_fmgr_util_load_filepath(buf_filename, FMGR_PATH_ID_PHOTO_EDITOR);		
		UnicodeToAnsii(tmpfilename, buf_filename);
		gdi_layer_clear_background(PHOART_MAIN_CANVAS_COLOR);
		gdi_layer_reset_clip();
	mmi_trace(1," mmi_phoart_entry_main_screen tmpfilename=%s,g_phoart_cntx.is_clip =%d",tmpfilename,g_phoart_cntx.is_clip );
		if(g_phoart_cntx.is_clip == FALSE)
		{
			/* image is not clipped */
			/******* draw image ********/
			result = gdi_image_get_dimension_file(	(PS8)buf_filename, 
																&image_width, 
																&image_height);
			mmi_trace(1," mmi_phoart_entry_main_screen result=%d,image_width=%d,image_height=%d",result,image_width,image_height);
			/* error handling */
			if((result < 0) || (image_width <= 0) || (image_height <=0))
			{
				gdi_layer_unlock_frame_buffer();
				
				DisplayPopup((PU8)GetString(STR_FMGR_INVALID_FORMAT), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
				DeleteNHistory(1);
				return;
			}
			
			if( (image_width > UI_device_width) || (image_height > UI_device_height))
			{
				gdi_image_util_fit_bbox(UI_device_width, 
												UI_device_height, 
												image_width, 
												image_height,
												&g_phoart_cntx.resized_image_offset_x, 
												&g_phoart_cntx.resized_image_offset_y,
												&g_phoart_cntx.resized_image_width, 
												&g_phoart_cntx.resized_image_height);

				if(g_phoart_cntx.resized_image_width == 0)
				{
					g_phoart_cntx.resized_image_width = 1;
					g_phoart_cntx.resized_image_offset_x--;
				}

				if(g_phoart_cntx.resized_image_height == 0)
				{
					g_phoart_cntx.resized_image_height = 1;
					g_phoart_cntx.resized_image_offset_y--;
				}
			
				result = gdi_image_draw_resized_file(	g_phoart_cntx.resized_image_offset_x, 
																	g_phoart_cntx.resized_image_offset_y, 
																	g_phoart_cntx.resized_image_width, 
																	g_phoart_cntx.resized_image_height, 
																	buf_filename);
			}
			else
			{
				g_phoart_cntx.resized_image_offset_x 	= (UI_device_width-image_width)>>1;
				g_phoart_cntx.resized_image_offset_y 	= (UI_device_height-image_height)>>1;
				g_phoart_cntx.resized_image_width    	= image_width;
				g_phoart_cntx.resized_image_height		= image_height;
				
				result =gdi_image_draw_file(g_phoart_cntx.resized_image_offset_x, g_phoart_cntx.resized_image_offset_y, buf_filename);
			mmi_trace(1," 222mmi_phoart_entry_main_screen result=%d,image_width=%d,image_height=%d,g_phoart_cntx.resized_image_offset_x=%d,g_phoart_cntx.resized_image_offset_y=%d",result,image_width,image_height,g_phoart_cntx.resized_image_offset_x,g_phoart_cntx.resized_image_offset_y);
			}	
			/* error handling */
			if((result < 0) || (image_width <= 0) || (image_height <=0))
			{
				gdi_layer_unlock_frame_buffer();
				
				DisplayPopup((PU8)GetString(STR_FMGR_INVALID_FORMAT), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
				DeleteNHistory(1);
				return;
			}

		}
		else
		{
			/* clip image */
			/* use gdi's clip jpeg function */
//			offset_x = (UI_device_width-g_phoart_cntx.clip_size_width)>>1;
//			offset_y = (UI_device_height-g_phoart_cntx.clip_size_height)>>1;
				g_phoart_cntx.resized_image_offset_x 	= (UI_device_width-g_phoart_cntx.resized_image_width)>>1;
				g_phoart_cntx.resized_image_offset_y 	= (UI_device_height-g_phoart_cntx.resized_image_height)>>1;
			
			#ifndef WIN32

	//			result =gdi_image_draw_file(0, 0, buf_filename);
				gdi_image_jpeg_draw_screen(
													g_phoart_cntx.clip_x1,
													g_phoart_cntx.clip_y1,
													g_phoart_cntx.resized_image_offset_x,
													g_phoart_cntx.resized_image_offset_y,
													g_phoart_cntx.resized_image_offset_x + g_phoart_cntx.resized_image_width,
													g_phoart_cntx.resized_image_offset_y + g_phoart_cntx.resized_image_height,
													g_phoart_cntx.resized_image_width,
													g_phoart_cntx.resized_image_height
													);
#if 0
				result= gdi_image_jpeg_draw_resized_file_with_output_clipping(offset_x-g_phoart_cntx.clip_x1, 
																									offset_y-g_phoart_cntx.clip_y1, 
																									g_phoart_cntx.resized_image_width,
																									g_phoart_cntx.resized_image_height,
																									g_phoart_cntx.clip_x1,
																									g_phoart_cntx.clip_y1,
																									g_phoart_cntx.clip_x2,
																									g_phoart_cntx.clip_y2,
																									buf_filename);

#endif
				mmi_trace(1," gdi_image_jpeg_draw_resized_file_with_output_clipping 2222result=%d (%d %d %d %d %d %d %d %d)",result,offset_x-g_phoart_cntx.clip_x1,offset_y-g_phoart_cntx.clip_y1, 
																									g_phoart_cntx.resized_image_width,
																									g_phoart_cntx.resized_image_height,
																									g_phoart_cntx.clip_x1,
																									g_phoart_cntx.clip_y1,
																									g_phoart_cntx.clip_x2,
																									g_phoart_cntx.clip_y2);
				/* error handling */
				if(result == GDI_JPEG_ERR_IMAGE_TOO_LARGE)
				{
					{
						gdi_layer_unlock_frame_buffer();
						
						DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_IMAGE_TOO_WIDE), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
						DeleteNHistory(1);
						return;
					}
				}			

				/* error handling */
				if(result < 0)
				{
					gdi_layer_unlock_frame_buffer();
					
					DisplayPopup((PU8)GetString(STR_FMGR_INVALID_FORMAT), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
					DeleteNHistory(1);
					return;
				}
			
			#endif
	#if 0		
			g_phoart_cntx.resized_image_offset_x	= offset_x;
			g_phoart_cntx.resized_image_offset_y	= offset_y;
			g_phoart_cntx.resized_image_width		= g_phoart_cntx.clip_size_width;
			g_phoart_cntx.resized_image_height		= g_phoart_cntx.clip_size_height;
	#endif

		}
		
		/* dump original image to cache file */
		gdi_layer_get_buffer_ptr(&buf_ptr);

		if(mmi_phoart_write_cache_image(buf_ptr) == FALSE)
		{
			/* TODO: error handing */
		}	
	}
	else
	{
		/* back from edited screens or interrupt */
		gdi_layer_get_buffer_ptr(&buf_ptr);			
		mmi_phoart_load_cache_image(buf_ptr);
	}


	/* draw main's osd content */
	mmi_phoart_main_draw_osd();

	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

	#ifdef __MMI_TOUCH_SCREEN__
		mmi_wgui_register_pen_down_handler(mmi_phoart_pen_down_hdlr);
		mmi_wgui_register_pen_up_handler(mmi_phoart_pen_up_hdlr);
		mmi_wgui_register_pen_move_handler(mmi_phoart_pen_move_hdlr);
	#endif

}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_main_screen()
* DESCRIPTION
*   exit main screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_main_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		

	/* free memory */	
	if(g_phoart_cntx.is_free_memory)
	{
		#ifdef APPMEM_SUPPORT
			if(g_phoart_cntx.origin_image_buf != NULL)
			{
//				kal_app_mem_free((void*)g_phoart_cntx.origin_image_buf);
				FreeMediaMemory((unsigned long)g_phoart_cntx.origin_image_buf);
				g_phoart_cntx.origin_image_buf = NULL;
			}
		#endif
		
		g_phoart_cntx.origin_image_buf = NULL;
	}

	/* stop option arrow animation */
	gdi_image_stop_animation_all();

	/* merge layer to base */
	gdi_layer_flatten_previous_to_base();

   /* free layer */
	gdi_layer_free(g_phoart_cntx.osd_layer_handle);
  	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, 0, 0, 0);
  	gdi_layer_multi_layer_disable();	


}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_del_screen_id_hdlr()
* DESCRIPTION
*   callback function when someone delete this screen's id
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static U8 mmi_phoart_del_screen_id_hdlr(void *ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
#ifdef APPMEM_SUPPORT
	/* free memory */	
	if(g_phoart_cntx.origin_image_buf != NULL)
	{
//		kal_app_mem_free((void*)g_phoart_cntx.origin_image_buf);
		FreeMediaMemory((unsigned long)g_phoart_cntx.origin_image_buf);
		g_phoart_cntx.origin_image_buf = NULL;
	}
#else
	g_phoart_cntx.origin_image_buf = NULL;
#endif 

	return FALSE;
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_main_confirm()
* DESCRIPTION
*   exit confirm 
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_main_confirm(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						 STR_GLOBAL_NO, IMG_GLOBAL_NO,
						 get_string(STR_ID_PHOART_NOTIFY_EXIT_CONFIRM), 
						 IMG_GLOBAL_QUESTION, 
						 WARNING_TONE); 

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_phoart_exit_main_confirm_yes, KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_main_confirm_yes()
* DESCRIPTION
*   exit confirm yes
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_main_confirm_yes(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* cencel file selection - delete up to one level before file selection */
	DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
	DeleteNHistory(1);
	GoBackHistory();
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_main_draw_osd()
* DESCRIPTION
*   exit confirm yes
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_main_draw_osd(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   S32				image_width;
   S32				image_height;
   S32				str_width;
   S32				str_height;
   UI_string_type	title_string;
	S32 				offset_x;
	S32				offset_y;

	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);

	gdi_layer_clear(GDI_COLOR_TRANSPARENT);

	/* draw image frame */
	mmi_phoart_draw_image_border();	

	/* use same text color as softkey */
	coolsand_UI_set_font(&MMI_medium_font);
	coolsand_UI_set_text_color(g_phoart_cntx.text_color);
	coolsand_UI_set_text_border_color(g_phoart_cntx.text_border_color);

	/* draw title */
	title_string = (UI_string_type)get_string(STR_ID_PHOART_APP);
	coolsand_UI_measure_string(title_string, &str_width, &str_height);

	if(r2lMMIFlag)
	{
		coolsand_UI_move_text_cursor(((UI_device_width-str_width)>>1)+str_width, (MMI_title_height-str_height)>>1);
	}
	else
	{
		coolsand_UI_move_text_cursor((UI_device_width-str_width)>>1, (MMI_title_height-str_height)>>1);
	}
	
   coolsand_UI_print_bordered_text(title_string);
   
	/******* draw softkey ********/

	/* init softkey */
	if(g_phoart_cntx.is_edited)
	{
		mmi_phoart_draw_softkey((PS8)get_string(STR_GLOBAL_OPTIONS), 
										(PS8)get_string(STR_ID_PHOART_EXIT),
										FALSE);
	}
	else
	{
		mmi_phoart_draw_softkey((PS8)get_string(STR_GLOBAL_OPTIONS), 
										(PS8)get_string(STR_GLOBAL_BACK),
										FALSE);	
	}

	gdi_image_get_dimension_id(IMG_ID_PHOART_OPTION_ARROW, &image_width, &image_height);
		
	offset_x = (MMI_softkey_width-image_width)>>1;
	offset_y = UI_device_height-(MMI_button_bar_height)-image_height-2;	/* 2 pixel spacing between buttom bar */
	
	gdi_image_draw_animation_id(offset_x, offset_y, IMG_ID_PHOART_OPTION_ARROW, NULL);

	gdi_layer_pop_and_restore_active();

}





/***************************************************************************** 
*
* Option Screen
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_option_screen()
* DESCRIPTION
*   entry photo art option screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_option_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[16];
	U16	item_icons[16];
	U16	num_item;
	U8		*gui_buffer;
	U8		*popup_list[16];
	S32	i;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
   /* set this flag before enter screen */
	EntryNewScreen(SCR_ID_PHOART_OPTION, NULL, mmi_phoart_entry_option_screen, NULL);

	gui_buffer 	= GetCurrGuiBuffer(SCR_ID_PHOART_OPTION);
	num_item		= GetNumOfChild(MENU_ID_PHOART_OPTION);
	
	GetSequenceStringIds(MENU_ID_PHOART_OPTION, item_list);
	GetSequenceImageIds(MENU_ID_PHOART_OPTION, item_icons);
	SetParentHandler(MENU_ID_PHOART_OPTION);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i = 0; i < num_item; i++)
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);

	for(i = 0 ; i < num_item; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_PHOART_OPTION, popup_list);

	ShowCategory52Screen(STR_GLOBAL_OPTIONS,	
								MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_item, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								gui_buffer);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	
}





/***************************************************************************** 
*
* Save Screen
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_save_as()
* DESCRIPTION
*   hilight save as menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_save_as(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_phoart_entry_editor_filename_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_entry_editor_filename_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_editor_done()
* DESCRIPTION
*   hilight editor "done" menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_editor_done(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction(mmi_phoart_save_file, KEY_EVENT_UP );
}





/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_editor_cancel()
* DESCRIPTION
*   hilight editor "cancel" menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_editor_cancel(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction( mmi_phoart_cancel_save_file, KEY_EVENT_UP);
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_editor_input()
* DESCRIPTION
*   hilight editor "input" menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_editor_input(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction( EntryInputMethodScreen, KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_cancel_save_file()
* DESCRIPTION
*   cancel save file
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_cancel_save_file(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  	DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_FILE_NOT_SAVED), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
	DeleteUptoScrID(SCR_ID_PHOART_MAIN);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_editor_filename_screen()
* DESCRIPTION
*   entry edit filename screen, this function will prepare valid filename and 
*	 call an internal editor function for screen display
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_editor_filename_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
 	S8		buf_filepath[FMGR_PATH_BUFFER_SIZE];  
	S8		buf_filename[FMGR_PATH_BUFFER_SIZE];
	S8		seq_buffer[15];
	S8		seq_buffer_unicode[30];	
	S8		test_buffer[(PHOART_MAX_FILENAME_CHAR+FMGR_MAX_EXT_LEN+1)*ENCODING_LENGTH];	
	PS8	filename_ptr;
	PS8	ext_ptr;
	S32 	filename_len;
	S32 	ext_len;
	S32   seq_num;
	S8 ext_tmp[8];
	
	FS_HANDLE	file_handle;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	/* extract file name for full path */
	mmi_fmgr_util_load_filepath(buf_filepath, FMGR_PATH_ID_PHOTO_EDITOR);
	
	filename_ptr = mmi_fmgr_extract_file_name(buf_filepath);

	if(g_phoart_cntx.is_short_filename)
	{
		mmi_chset_mixed_text_to_ucs2_str((U8*)buf_filename, 
													FMGR_PATH_BUFFER_SIZE, 
													(U8*)filename_ptr, 
													PhnsetGetDefEncodingType());
	}
	else
	{
		pfnUnicodeStrcpy((PS8)buf_filename, (PS8)filename_ptr);
	}
	
	filename_len = pfnUnicodeStrlen((PS8)buf_filename);

	ext_ptr = mmi_fmgr_extract_ext_file_name(buf_filename);
	ext_len = pfnUnicodeStrlen((PS8)ext_ptr);

	/* remove ext len, +1 for dot */
	filename_len -= (ext_len+1);

	/* +3 for _XX */	
	if(filename_len+3 > PHOART_MAX_FILENAME_CHAR)
	{
		/* original filename too long, need truncate */
		buf_filename[((PHOART_MAX_FILENAME_CHAR-3-1)*ENCODING_LENGTH)] = 0;
		buf_filename[((PHOART_MAX_FILENAME_CHAR-3-1)*ENCODING_LENGTH)+1] = 0;
	}
	else
	{
		/* remove ext */
		buf_filename[((filename_len)*ENCODING_LENGTH)] = 0;
		buf_filename[((filename_len)*ENCODING_LENGTH)+1] = 0;	
	}

	pfnUnicodeStrcpy((PS8)g_phoart_cntx.filename_edit_buf, (PS8)buf_filename);

	/* add seq num */
	seq_num = 1;

	mmi_fmgr_util_load_filepath(buf_filename, FMGR_PATH_ID_PHOTO_EDITOR);
	filename_ptr = mmi_fmgr_extract_file_name(buf_filename);
	AnsiiToUnicodeString(ext_tmp,".jpg");
	while(TRUE)
	{
		sprintf((PS8)seq_buffer, "_%02d", (U16)seq_num);
		AnsiiToUnicodeString((PS8)seq_buffer_unicode, (PS8)seq_buffer);		
		pfnUnicodeStrcpy((PS8)test_buffer, (PS8)g_phoart_cntx.filename_edit_buf);
		pfnUnicodeStrcat((PS8)test_buffer, (PS8)seq_buffer_unicode);
		pfnUnicodeStrcat((PS8)test_buffer, (PS8)ext_tmp);	
		pfnUnicodeStrcpy((PS8)filename_ptr, (PS8)test_buffer);

		file_handle = MMI_FS_Open((UINT8*)buf_filename, FS_READ_ONLY);
		
		if(file_handle >= 0)
		{
			/* file exist */
			FS_Close(file_handle);
			seq_num++;	
		}
		else
		{
			/* file not exit */
			break;
		}

		/* will overwrite existing function if exceed 100 */
		if(seq_num == 100)
			break;
		
	}

	pfnUnicodeStrcpy((PS8)g_phoart_cntx.filename_edit_buf, (PS8)test_buffer);

	/* remove extension */
	ext_ptr = mmi_fmgr_extract_ext_file_name(g_phoart_cntx.filename_edit_buf);
	*(ext_ptr-2)	= 0;
	*(ext_ptr-1) 	= 0;
	
	mmi_phoart_entry_editor_filename_internal();
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_editor_filename_internal()
* DESCRIPTION
*   entry edit filename internal function
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_editor_filename_internal(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		*gui_buffer;
 	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
   EntryNewScreen(SCR_ID_PHOART_EDITOR_FILENAME, NULL, mmi_phoart_entry_editor_filename_internal, NULL );

   gui_buffer = GetCurrGuiBuffer(SCR_ID_PHOART_EDITOR_FILENAME); 
	
   ShowCategory5Screen(	STR_ID_PHOART_EDIT_FILENAME, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
			               STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
			               STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			               INPUT_TYPE_ALPHANUMERIC_UPPERCASE,
			               (U8*)g_phoart_cntx.filename_edit_buf,
			               PHOART_MAX_FILENAME_CHAR,
			               gui_buffer);
   	#if defined(__MMI_GB__)                                                              
	GBSetInputboxLSKFunction(mmi_phoart_entry_editor_option_screen);
       #endif
	SetLeftSoftkeyFunction(mmi_phoart_entry_editor_option_screen,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(mmi_phoart_back_from_editor_screen,KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction(mmi_phoart_return_to_editor_screen);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_return_to_editor_screen()
* DESCRIPTION
*   return to editor from input selection
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_return_to_editor_screen(void)
{
   GoBacknHistory(1);
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_back_from_editor_screen()
* DESCRIPTION
*   back from edit filename screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_back_from_editor_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_FILE_NOT_SAVED), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
   DeleteNHistory(1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_editor_option_screen()
* DESCRIPTION
*   entry edit option screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_editor_option_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[16];
	U16	item_icons[16];
	U16	num_item;
	U8		*gui_buffer;
	U8		*popup_list[16];
	S32	i;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		

	/* store edited text to NVRAM */
	mmi_phoedt_store_data();

	EntryNewScreen(SCR_ID_PHOART_EDITOR_OPTION, NULL, mmi_phoart_entry_editor_option_screen, NULL);

	gui_buffer 	= GetCurrGuiBuffer(SCR_ID_PHOART_EDITOR_OPTION);
	num_item		= GetNumOfChild(MENU_ID_PHOART_EDITOR_OPTION);
	
	GetSequenceStringIds(MENU_ID_PHOART_EDITOR_OPTION, item_list);
	GetSequenceImageIds(MENU_ID_PHOART_EDITOR_OPTION, item_icons);
	SetParentHandler(MENU_ID_PHOART_EDITOR_OPTION);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i = 0; i < num_item; i++)
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);

	for(i = 0 ; i < num_item; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_PHOART_EDITOR_OPTION, popup_list);

	ShowCategory52Screen(STR_GLOBAL_OPTIONS,	
								MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_item, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								gui_buffer);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);


}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_save_file()
* DESCRIPTION
*   entry saving screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_save_file(void)
{
  /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8						buf_filename[FMGR_PATH_BUFFER_SIZE+PHOART_MAX_FILENAME_CHAR*ENCODING_LENGTH];
	PS8					filename_ptr;
	MDI_RESULT 			ret;
	gdi_handle  		image_layer_handle;
	gdi_rect_struct	rect;
	PU8					result_buf;
	S8 tmp_ext[16];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	/* check if file is empty or not */
	if(pfnUnicodeStrlen((PS8)g_phoart_cntx.filename_edit_buf) == 0)
	{
		DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_EMPTY_STRING), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
		return;
	}

	AnsiiToUnicodeString(tmp_ext, ".jpg");
 	/* prepare filepath */
   mmi_fmgr_util_load_filepath(buf_filename, FMGR_PATH_ID_PHOTO_EDITOR);
	filename_ptr = mmi_fmgr_extract_file_name(buf_filename);
	
	/* add new filename to path */
	pfnUnicodeStrcpy((PS8)filename_ptr, (PS8)g_phoart_cntx.filename_edit_buf);
	pfnUnicodeStrcat((PS8)buf_filename, (PS8)tmp_ext);

	if(pfnUnicodeStrlen((PS8)buf_filename) > FMGR_PATH_CHAR_COUNT)
	{
		DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
		return;
	}

	/* image is in original image buffer g_phoart_cntx.origin_image_buf */	

	/* if image is not full screen, move it to proper place */
	
	if((g_phoart_cntx.resized_image_width != UI_device_width) || 
    	(g_phoart_cntx.resized_image_height != UI_device_height))
	{
		rect.x1 = 0;
		rect.y1 = 0;
		rect.x2 = g_phoart_cntx.resized_image_width-1;	
		rect.y2 = g_phoart_cntx.resized_image_height-1;


		/* allocate buffer for icon layer */
		#ifdef APPMEM_SUPPORT 			
//			result_buf = kal_app_mem_alloc(PHOART_FRAME_BUFFER_SIZE, KAL_APPMEM_APP, MOD_MMI); 
			result_buf = (PU8)(AllocMediaMemory(PHOART_FRAME_BUFFER_SIZE)); 
		#else
			result_buf = (PU8)camera_osd_layer_buffer;
			result_buf = &result_buf[PHOART_FRAME_BUFFER_SIZE];
		#endif /* APPMEM_SUPPORT */		

		gdi_2d_memory_blt(g_phoart_cntx.origin_image_buf, 
								UI_device_width, 
								g_phoart_cntx.resized_image_offset_x, 
								g_phoart_cntx.resized_image_offset_y, 
								g_phoart_cntx.resized_image_width, 
								g_phoart_cntx.resized_image_height,
								result_buf, 
								g_phoart_cntx.resized_image_width, 
								0, 
								0, 
								rect);

		/* create a layer and capture this layer as jepg file */
		gdi_layer_create_using_outside_memory( 0, 
															0, 
															g_phoart_cntx.resized_image_width, 
															g_phoart_cntx.resized_image_height, 
															&image_layer_handle, 
															(PU8)result_buf, 
															(S32)PHOART_FRAME_BUFFER_SIZE);

//		ret = mdi_camera_encode_layer_to_jpeg(image_layer_handle, (PS8)buf_filename);
		ret = gdi_layer_save_jpeg_file(image_layer_handle, (PS8)buf_filename);

		gdi_layer_free(image_layer_handle);

		/* allocate buffer for icon layer */
		#ifdef APPMEM_SUPPORT 			
//			kal_app_mem_free(result_buf); 
			FreeMediaMemory((unsigned long)result_buf); 
		#endif /* APPMEM_SUPPORT */		
		
	}
	else
	{
		/* create a layer and capture this layer as jepg file */
		gdi_layer_create_using_outside_memory( 0, 
															0, 
															g_phoart_cntx.resized_image_width, 
															g_phoart_cntx.resized_image_height, 
															&image_layer_handle, 
															(PU8)g_phoart_cntx.origin_image_buf, 
															(S32)PHOART_FRAME_BUFFER_SIZE);

//		ret = mdi_camera_encode_layer_to_jpeg(image_layer_handle, (PS8)buf_filename);
		ret = gdi_layer_save_jpeg_file(image_layer_handle, (PS8)buf_filename);
		gdi_layer_free(image_layer_handle);
	}


	
#ifdef WIN32
	/* simulaotr */
	{
		FS_HANDLE file_handle;
		file_handle = FS_Open((PU16)buf_filename, FS_CREATE|FS_READ_WRITE, 0);

		if(file_handle>=0)
			FS_Close(file_handle);

		ret = 0;
	}
#endif

	switch(ret)
	{
		case MDI_RES_CAMERA_SUCCEED:
			DisplayPopup((PU8)GetString(STR_GLOBAL_SAVED), IMG_GLOBAL_INFO, 1, PHOEDT_NOTIFY_DURATION, SUCCESS_TONE);
			
			if(g_phoart_cntx.is_edit_from_fmgr)
			{
				DeleteUptoScrID(SCR_ID_PHOART_CLIP_OPTION);
			}
			else
			{
				DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
			}
			
			DeleteNHistory(1);	
			break;

		case MDI_RES_CAMERA_ERR_DISK_FULL:
			DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_DISK_FULL), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, SUCCESS_TONE);

			if(g_phoart_cntx.is_edit_from_fmgr)
			{
				DeleteUptoScrID(SCR_ID_PHOART_CLIP_OPTION);
			}
			else
			{
				DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
			}
			
			DeleteNHistory(1);			
			break;

		case MDI_RES_CAMERA_ERR_WRITE_PROTECTION:
			DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_WRITE_PROTECTION), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, SUCCESS_TONE);

			if(g_phoart_cntx.is_edit_from_fmgr)
			{
				DeleteUptoScrID(SCR_ID_PHOART_CLIP_OPTION);
			}
			else
			{
				DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
			}
			
			DeleteNHistory(1);			
			break;
			
		case MDI_RES_CAMERA_ERR_NO_DISK:
			DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, SUCCESS_TONE);

			if(g_phoart_cntx.is_edit_from_fmgr)
			{
				DeleteUptoScrID(SCR_ID_PHOART_CLIP_OPTION);
			}
			else
			{
				DeleteUptoScrID(SCR_ID_PHOART_SELECT_FILE);
			}
			
			DeleteNHistory(1);			
			break;

		default:
			DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILENAME_TEXT), IMG_GLOBAL_ERROR, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
			break;
	}


}




/***************************************************************************** 
*
* Add Effect
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_effect_select_screen()
* DESCRIPTION
*   entry effect selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_effect_select_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[16];
	U16	item_icons[16];
	U16	num_item;
	U8		*gui_buffer;
	U8		*popup_list[16];
	S32	i;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_EFFECT_SELECT, NULL, mmi_phoart_entry_effect_select_screen, NULL);

	gui_buffer 	= GetCurrGuiBuffer(SCR_ID_PHOART_EFFECT_SELECT);
	num_item		= GetNumOfChild(MENU_ID_PHOART_ADD_EFFECT);
	
	GetSequenceStringIds(MENU_ID_PHOART_ADD_EFFECT, item_list);
	GetSequenceImageIds(MENU_ID_PHOART_ADD_EFFECT, item_icons);
	SetParentHandler(MENU_ID_PHOART_ADD_EFFECT);

	RegisterHighlightHandler(mmi_phoart_effect_select_highlight_hdlr);

 	for(i = 0; i < num_item; i++)
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);

	for(i = 0 ; i < num_item; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_PHOART_ADD_EFFECT, popup_list);

	ShowCategory52Screen(STR_ID_PHOART_ADD_EFFECT,	
								MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_item, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								gui_buffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_add_effect()
* DESCRIPTION
*   hilight save as menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_add_effect(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_phoart_entry_effect_select_screen,KEY_EVENT_UP);
} 


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_effect_select_highlight_hdlr()
* DESCRIPTION
*   callback function to get item
* PARAMETERS
*	 idx		IN		item index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_effect_select_highlight_hdlr(S32 idx)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	g_phoart_cntx.selected_effect = (U16)idx;
	
	SetLeftSoftkeyFunction(mmi_phoart_entry_effect_edit_screen, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_entry_effect_edit_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_effect_edit_screen()
* DESCRIPTION
*   entry effect selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_effect_edit_screen(void)
{
	/*----------------------------------------------------------------*/		
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_EFFECT_EDIT, mmi_phoart_exit_effect_edit_screen, mmi_phoart_entry_effect_edit_screen, NULL);
	
	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();


	g_phoart_cntx.is_user_goback = FALSE;

	/* entry full screen */
	entry_full_screen();

	SetKeyHandler(mmi_phoart_effect_edit_change_backward, KEY_VOL_DOWN, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_effect_edit_change_forward, KEY_VOL_UP, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_effect_edit_change_forward, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_effect_edit_change_backward, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	#ifdef WIN32
		SetKeyHandler(mmi_phoart_effect_edit_change_backward, KEY_7, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_phoart_effect_edit_change_forward, KEY_1, KEY_EVENT_DOWN);
	#endif

	/* register key hdlr */
	SetKeyHandler(mmi_phoart_lsk_press,	 	KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_lsk_release, 	KEY_LSK, KEY_EVENT_UP);	

	SetKeyHandler(mmi_phoart_rsk_press, 	KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_rsk_release, 	KEY_RSK, KEY_EVENT_UP);	

	
	/***************************************************************************** 
	* Layers
	*****************************************************************************/
	gdi_layer_multi_layer_enable();
	
	/* base layer */
	gdi_layer_get_active(&g_phoart_cntx.base_layer_handle);

	/* clip rect layer */
	gdi_layer_create(0, 0, UI_device_width, UI_device_height, &g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();
	
	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, g_phoart_cntx.osd_layer_handle, 0, 0);

	/* apply effect */
	mmi_phoart_effect_edit_add_effect();

	/* draw OSD */
	mmi_phoart_effect_edit_draw_osd();

	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

	#ifdef __MMI_TOUCH_SCREEN__
		mmi_wgui_register_pen_down_handler(mmi_phoart_pen_down_hdlr);
		mmi_wgui_register_pen_up_handler(mmi_phoart_pen_up_hdlr);
		mmi_wgui_register_pen_move_handler(mmi_phoart_pen_move_hdlr);
	#endif

}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_effect_edit_screen()
* DESCRIPTION
*   entry effect selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_effect_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		

   /* free layer */
	gdi_layer_flatten_previous_to_base();
   
	gdi_layer_free(g_phoart_cntx.osd_layer_handle);
  	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, 0, 0, 0);
  	gdi_layer_multi_layer_disable();	

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_effect_edit_add_effect()
* DESCRIPTION
*   change effect
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_effect_edit_add_effect(void)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	PU8	buf_ptr;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	/* load original image from cache to [Base Layer] */
	gdi_layer_get_buffer_ptr(&buf_ptr);
	mmi_phoart_load_cache_image(buf_ptr);

	/* add effect to layer */
	mdi_camera_add_post_effect(g_phoart_cntx.base_layer_handle, phoart_effect_command_map[g_phoart_cntx.selected_effect]);

	/* some effect will apply to full screen, we have to paint bg color back when image is smaller than fullscreen */

	/*
		*******************
		*		  (1)			*
		*						*	
		*-----*******-----*
		*		*******		*
		* (3)	******* (4)	*
		*		*******		*
		*-----*******-----*
		*						*
		*		  (2)			*
		*******************
	*/
	if((g_phoart_cntx.resized_image_width != UI_device_width) || 
		(g_phoart_cntx.resized_image_height != UI_device_height))
	{
		/* region (1) */
		gdi_draw_solid_rect(	0, 
									0, 
									UI_device_width-1, 
									g_phoart_cntx.resized_image_offset_y-1, 
									PHOART_MAIN_CANVAS_COLOR);

		/* region (2) */
		gdi_draw_solid_rect(	0, 
									g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height, 
									UI_device_width-1, 
									UI_device_height-1, 
									PHOART_MAIN_CANVAS_COLOR);

		/* region (3) */
		gdi_draw_solid_rect( 0, 
									g_phoart_cntx.resized_image_offset_y, 
									g_phoart_cntx.resized_image_offset_x-1, 
									g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1, 
									PHOART_MAIN_CANVAS_COLOR);

		/* region (4) */
		gdi_draw_solid_rect( g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width, 
									g_phoart_cntx.resized_image_offset_y, 
									UI_device_width-1, 
									g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1, 
									PHOART_MAIN_CANVAS_COLOR);


	}
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_effect_edit_change_forward()
* DESCRIPTION
*   callback function to get item
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_effect_edit_change_forward(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	if(g_phoart_cntx.selected_effect < PHOART_EFFECT_COUNT-1)
	{
		g_phoart_cntx.selected_effect++;
	}
	else
	{
		g_phoart_cntx.selected_effect = 0;
	}

	/* apply effect */
	gdi_layer_lock_frame_buffer();

	mmi_phoart_effect_edit_add_effect();
	mmi_phoart_effect_edit_draw_osd();	

	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_effect_edit_change_backward()
* DESCRIPTION
*   callback function to get item
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_effect_edit_change_backward(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	if(g_phoart_cntx.selected_effect > 0)
	{
		g_phoart_cntx.selected_effect--;
	}
	else
	{
		g_phoart_cntx.selected_effect = PHOART_EFFECT_COUNT-1;
	}
	
	/* apply effect */
	gdi_layer_lock_frame_buffer();
	
	mmi_phoart_effect_edit_add_effect();
	mmi_phoart_effect_edit_draw_osd();
	
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_effect_edit_draw_osd()
* DESCRIPTION
*   draw effect screen osd layer content
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_effect_edit_draw_osd(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* draw other UI compoment on OSD layer */
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);

	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	
	mmi_phoart_draw_image_border();
	
	mmi_phoart_draw_title(	(PS8)get_string(STR_ID_PHOART_ADD_EFFECT_TITLE),
									(PS8)get_string((U16)(STR_ID_PHOART_EFFECT_START+g_phoart_cntx.selected_effect+1)),
									FALSE);
	
	mmi_phoart_draw_softkey((PS8)get_string(STR_GLOBAL_DONE), 
									(PS8)get_string(STR_GLOBAL_CANCEL),
									FALSE);
	
	gdi_layer_pop_and_restore_active();
	
}



/***************************************************************************** 
*
* Add Frame
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_frame_select_screen()
* DESCRIPTION
*   entry frame selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_frame_select_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[16];
	U16	item_icons[16];
	U16	num_item;
	U8		*gui_buffer;
	U8		*popup_list[16];
	S32	i;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_FRAME_SELECT, NULL, mmi_phoart_entry_frame_select_screen, NULL);

	gui_buffer 	= GetCurrGuiBuffer(SCR_ID_PHOART_FRAME_SELECT);
	num_item		= GetNumOfChild(MENU_ID_PHOART_ADD_FRAME);
	
	GetSequenceStringIds(MENU_ID_PHOART_ADD_FRAME, item_list);
	GetSequenceImageIds(MENU_ID_PHOART_ADD_FRAME, item_icons);
	SetParentHandler(MENU_ID_PHOART_ADD_FRAME);

	RegisterHighlightHandler(mmi_phoart_frame_select_highlight_hdlr);

	for(i = 0; i < num_item; i++)
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);

	for(i = 0 ; i < num_item; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_PHOART_ADD_FRAME, popup_list);

	ShowCategory52Screen(STR_ID_PHOART_ADD_FRAME,	
								MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_item, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								gui_buffer);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_add_frame()
* DESCRIPTION
*   hilight save as menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_add_frame(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_phoart_entry_frame_select_screen,KEY_EVENT_UP);
} 




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_frame_select_highlight_hdlr()
* DESCRIPTION
*   callback function to get item
* PARAMETERS
*	 idx		IN		item index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_frame_select_highlight_hdlr(S32 idx)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	g_phoart_cntx.selected_frame = (U16)idx;
	
	SetLeftSoftkeyFunction(mmi_phoart_entry_frame_edit_screen, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_entry_frame_edit_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_frame_edit_screen()
* DESCRIPTION
*   entry frame selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_frame_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
   PU8			buf_ptr;
	
	/*----------------------------------------------------------------*/		
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_FRAME_EDIT, mmi_phoart_exit_frame_edit_screen, mmi_phoart_entry_frame_edit_screen, NULL);

	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	/* entry full screen */
	entry_full_screen();

	SetKeyHandler(mmi_phoart_frame_edit_change_forward, KEY_VOL_DOWN, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_frame_edit_change_backward, KEY_VOL_UP, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_frame_edit_change_forward, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_frame_edit_change_backward, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	#ifdef WIN32
		SetKeyHandler(mmi_phoart_frame_edit_change_forward, KEY_7, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_phoart_frame_edit_change_backward, KEY_1, KEY_EVENT_DOWN);
	#endif

	/* register key hdlr */
	SetKeyHandler(mmi_phoart_lsk_press,	 	KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_lsk_release, 	KEY_LSK, KEY_EVENT_UP);	

	SetKeyHandler(mmi_phoart_rsk_press, 	KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_rsk_release, 	KEY_RSK, KEY_EVENT_UP);	
	
	/***************************************************************************** 
	* Layers
	*****************************************************************************/
	gdi_layer_multi_layer_enable();
	
	/* base layer */
	gdi_layer_get_active(&g_phoart_cntx.base_layer_handle);

	/* osd layer */
	gdi_layer_create(0, 0, UI_device_width, UI_device_height, &g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();

	
	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle,
									g_phoart_cntx.osd_layer_handle,  
									0, 0);

	/* load original image from cache to [Base layer] */
	gdi_layer_get_buffer_ptr(&buf_ptr);
	mmi_phoart_load_cache_image(buf_ptr);

	/* draw OSD */
	mmi_phoart_frame_edit_draw_osd(TRUE);

	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

	#ifdef __MMI_TOUCH_SCREEN__
		mmi_wgui_register_pen_down_handler(mmi_phoart_pen_down_hdlr);
		mmi_wgui_register_pen_up_handler(mmi_phoart_pen_up_hdlr);
		mmi_wgui_register_pen_move_handler(mmi_phoart_pen_move_hdlr);
	#endif

}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_frame_edit_screen()
* DESCRIPTION
*   entry frame selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_frame_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
   /* free layer */
	gdi_layer_flatten_previous_to_base();

	gdi_layer_free(g_phoart_cntx.osd_layer_handle);

  	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, 0, 0, 0);
  	gdi_layer_multi_layer_disable();	

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_frame_edit_draw_frame()
* DESCRIPTION
*   change frame
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_frame_edit_draw_frame(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
   gdi_layer_push_clip();
   #if 0
	gdi_layer_set_clip(	g_phoart_cntx.resized_image_offset_x,
								g_phoart_cntx.resized_image_offset_y,
								g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1,
								g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1);
#endif
	gdi_image_draw_id(0, 0, (U16)(IMG_ID_PHOART_FRAME_START+g_phoart_cntx.selected_frame+1));	

	gdi_layer_pop_clip();

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_frame_edit_change_backward()
* DESCRIPTION
*   callback function to get item
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_frame_edit_change_backward(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	if(g_phoart_cntx.selected_frame >0 )
	{
		g_phoart_cntx.selected_frame--;
	}
	else
	{
		g_phoart_cntx.selected_frame = PHOART_FRAME_COUNT-1;
	}

	/* draw frame */	
	mmi_phoart_frame_edit_draw_osd(TRUE);

	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_frame_edit_change_forward()
* DESCRIPTION
*   callback function to get item
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_frame_edit_change_forward(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	if(g_phoart_cntx.selected_frame  < PHOART_FRAME_COUNT-1)
	{
		g_phoart_cntx.selected_frame++;
	}
	else
	{
		g_phoart_cntx.selected_frame = 0;
	}
	
	/* draw osd */	
	mmi_phoart_frame_edit_draw_osd(TRUE);

	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_frame_edit_draw_osd()
* DESCRIPTION
*   draw add frame screen osd layer content
* PARAMETERS
*	 is_draw_frame 	IN 	clear bg and redraw frame
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_frame_edit_draw_osd(BOOL is_draw_frame)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* draw other UI compoment on OSD layer */
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);

	if(is_draw_frame)
	{
		gdi_layer_clear(GDI_COLOR_TRANSPARENT);
		mmi_phoart_frame_edit_draw_frame();
	}
	
	mmi_phoart_draw_image_border();

	/* draw softkey & title */
	mmi_phoart_draw_softkey((PS8)get_string(STR_GLOBAL_DONE), 
									(PS8)get_string(STR_GLOBAL_CANCEL),
									FALSE);				

	mmi_phoart_draw_title(	(PS8)get_string(STR_ID_PHOART_ADD_FRAME_TITLE),
									(PS8)get_string((U16)(STR_ID_PHOART_FRAME_START+g_phoart_cntx.selected_frame+1)),
									FALSE);
	
	gdi_layer_pop_and_restore_active();
	
}




/***************************************************************************** 
*
* Add Icon
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_icon_select_screen()
* DESCRIPTION
*   entry icon selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_icon_select_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		*gui_buffer;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_ICON_SELECT, NULL, mmi_phoart_entry_icon_select_screen, NULL);

	gui_buffer = GetCurrGuiBuffer(SCR_ID_PHOART_ICON_SELECT);

	RegisterHighlightHandler(mmi_phoart_icon_select_highlight_hdlr);
	
	ShowCategory214Screen(	STR_ID_PHOART_ADD_ICON, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
		                   	STR_GLOBAL_OK, IMG_GLOBAL_OK,   	
                       	 	STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
									PHOART_ICON_COUNT,									/* num of item */
									mmi_phoart_icon_select_draw_image_callback, 	/* draw image callback */
									mmi_phoart_icon_select_hide_image_callback,	/* hide image callback */
									3,															/* number per row */	
									3,															/* number per col */
									0,															/* hilighted item */
									(U8*)gui_buffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_phoart_entry_icon_edit_screen, KEY_EVENT_UP);
}





/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_add_icon()
* DESCRIPTION
*   hilight save as menuitem hdlr
* PARAMETERS
*   idx					IN		selected index
*   page_start_idx	IN		page's start index
*   page_end_idx		IN		page's end index
*	 is_highlghted 	IN		is this selection highlighted
*	 is_need_update 	IN		is thie item need update
*	 is_new_page		IN		is new page
*	 x1					IN		draw region's x1
*	 y1					IN		draw region's y1
*	 x2					IN		draw region's x2
*	 y2					IN		draw region's y2
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_select_draw_image_callback(	S32 idx, S32 page_start_idx, S32 page_end_idx, 
																			BOOL is_hilighted, BOOL is_need_update, BOOL is_new_page, 
																			S32 x1, S32 y1, S32 x2, S32 y2)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S32 image_width;
	S32 image_height;
	U16 image_id;
	S32 box_width;
	S32 box_height;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	if(is_need_update)
	{

		gdi_layer_push_clip();
		gdi_layer_set_clip(x1, y1, x2, y2);

		box_width 	= x2-x1+1;
		box_height 	= y2-y1+1;
		
		image_id = (U16)(IMG_ID_PHOART_ICON_START+idx+1);
		
		gdi_image_get_dimension_id(image_id, &image_width, &image_height);
		gdi_image_draw_id(((box_width-image_width)>>1)+x1, ((box_height-image_height)>>1)+y1, image_id);

		gdi_layer_pop_clip();
	}
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_add_icon()
* DESCRIPTION
*   hilight save as menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_select_hide_image_callback(S32 idx)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_add_icon()
* DESCRIPTION
*   hilight save as menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_add_icon(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_phoart_entry_icon_select_screen, KEY_EVENT_UP);
} 



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_select_highlight_hdlr()
* DESCRIPTION
*   callback function to get item
* PARAMETERS
*	 idx		IN		item index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_select_highlight_hdlr(S32 idx)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	g_phoart_cntx.selected_icon = (U16)idx;
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_icon_edit_screen()
* DESCRIPTION
*   entry icon selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_icon_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	PU8			buf_ptr;
	U8				*gui_buffer;  

	#ifndef APPMEM_SUPPORT 	
		PU8		add_item_buf_ptr;
	#endif /* !APPMEM_SUPPORT */
	
	/*----------------------------------------------------------------*/		
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_ICON_EDIT, mmi_phoart_exit_icon_edit_screen, mmi_phoart_entry_icon_edit_screen, NULL);

	/* reset flag */
	g_phoart_cntx.is_user_goback = FALSE;

	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

   gui_buffer = GetCurrGuiBuffer(SCR_ID_PHOART_ICON_EDIT); 

	/* entry full screen */
	entry_full_screen();

	/* inti variable */
	g_phoart_cntx.is_up_arrow_pressed 		= FALSE;
	g_phoart_cntx.is_down_arrow_pressed 	= FALSE;
	g_phoart_cntx.is_left_arrow_pressed 	= FALSE;
	g_phoart_cntx.is_right_arrow_pressed 	= FALSE;	

	SetKeyHandler(mmi_phoart_icon_edit_move_up_press, KEY_UP_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_icon_edit_move_down_press, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_icon_edit_move_left_press, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_icon_edit_move_right_press, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_icon_edit_move_up_release, KEY_UP_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_icon_edit_move_down_release, KEY_DOWN_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_icon_edit_move_left_release, KEY_LEFT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_icon_edit_move_right_release, KEY_RIGHT_ARROW, KEY_EVENT_UP);

	SetKeyHandler(mmi_phoart_icon_edit_move_up_press, KEY_2, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_icon_edit_move_down_press, KEY_8, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_icon_edit_move_left_press, KEY_4, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_icon_edit_move_right_press, KEY_6, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_icon_edit_move_up_release, KEY_2, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_icon_edit_move_down_release, KEY_8, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_icon_edit_move_left_release, KEY_4, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_icon_edit_move_right_release, KEY_6, KEY_EVENT_UP);
	
	SetKeyHandler(mmi_phoart_icon_edit_stamp, KEY_ENTER, KEY_EVENT_DOWN);

	/* register key hdlr */
	SetKeyHandler(mmi_phoart_lsk_press,	 	KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_lsk_release, 	KEY_LSK, KEY_EVENT_UP);	

	SetKeyHandler(mmi_phoart_rsk_press, 	KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_rsk_release, 	KEY_RSK, KEY_EVENT_UP);	
	
	/***************************************************************************** 
	* Layers
	*****************************************************************************/
	gdi_layer_multi_layer_enable();
	
	/* base layer */
	gdi_layer_get_active(&g_phoart_cntx.base_layer_handle);
		
	/* osd layer */
	gdi_layer_create(0, 0, UI_device_width, UI_device_height, &g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();
	
	/* drawing */
	gdi_layer_lock_frame_buffer();

	/* load original image from cache */
	gdi_layer_get_buffer_ptr(&buf_ptr);

	if(gui_buffer != NULL)
	{
		/* restore from interrupt */
		memcpy(buf_ptr, g_phoart_cntx.interrupt_backup_buf, PHOART_FRAME_BUFFER_SIZE);

		#ifdef APPMEM_SUPPORT 	
			/* free backup memory */
//			kal_app_mem_free((void*)g_phoart_cntx.interrupt_backup_buf);
			FreeMediaMemory((unsigned long)g_phoart_cntx.interrupt_backup_buf);
		#endif
		
		g_phoart_cntx.interrupt_backup_buf = NULL;

	}		
	else
	{
		/* align icon at center */
		gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ICON_START+g_phoart_cntx.selected_icon+1), 
											&g_phoart_cntx.icon_width, 
											&g_phoart_cntx.icon_height);
	
		g_phoart_cntx.icon_offset_x = (UI_device_width-g_phoart_cntx.icon_width)>>1;
		g_phoart_cntx.icon_offset_y = (UI_device_height-g_phoart_cntx.icon_height)>>1;	
		
		mmi_phoart_load_cache_image(buf_ptr);
	}

	/* allocate buffer for icon layer */
	#ifdef APPMEM_SUPPORT 			
//		g_phoart_cntx.add_item_buf= kal_app_mem_alloc(PHOART_FRAME_BUFFER_SIZE, KAL_APPMEM_APP, MOD_MMI); 
		g_phoart_cntx.add_item_buf= (PU8)(AllocMediaMemory(PHOART_FRAME_BUFFER_SIZE)); 
	#else
		add_item_buf_ptr = (PU8)camera_osd_layer_buffer;
		g_phoart_cntx.add_item_buf = &add_item_buf_ptr[PHOART_FRAME_BUFFER_SIZE];
	#endif /* APPMEM_SUPPORT */		

	/* create layer */
	gdi_layer_create_using_outside_memory(	ICON_LAYER_X1, 
														ICON_LAYER_Y1, 
														ICON_LAYER_WIDTH, 
														ICON_LAYER_HEIGHT,
														&g_phoart_cntx.icon_layer_handle, 
														g_phoart_cntx.add_item_buf, 
														PHOART_FRAME_BUFFER_SIZE);
	
	gdi_layer_push_and_set_active(g_phoart_cntx.icon_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();

	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle,
									g_phoart_cntx.icon_layer_handle,
									g_phoart_cntx.osd_layer_handle,  
									0);
	
	mmi_phoart_icon_edit_draw_osd();
	mmi_phoart_icon_edit_draw_icon(PHOART_BORDER_CORNER);

	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

	#ifdef __MMI_TOUCH_SCREEN__
		mmi_wgui_register_pen_down_handler(mmi_phoart_pen_down_hdlr);
		mmi_wgui_register_pen_up_handler(mmi_phoart_pen_up_hdlr);
		mmi_wgui_register_pen_move_handler(mmi_phoart_pen_move_hdlr);
	#endif
	
}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_icon_edit_screen()
* DESCRIPTION
*   entry icon selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_icon_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	PU8	buf_ptr;

   #ifndef APPMEM_SUPPORT 	
		PU8	back_buf_ptr;
   #endif /* !APPMEM_SUPPORT */
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	gdi_layer_free(g_phoart_cntx.icon_layer_handle);


	/* free add item layer & buffer */
	#ifdef APPMEM_SUPPORT 	
		/* free backup memory */
//		kal_app_mem_free((void*)g_phoart_cntx.add_item_buf);
		FreeMediaMemory((unsigned long)g_phoart_cntx.add_item_buf);
	#endif
		
	g_phoart_cntx.add_item_buf = NULL;


	if(g_phoart_cntx.is_user_goback == FALSE)
	{
		/*
			if not user presse rsk. it might be 
			(1) interrupt.
			(2) user pressed idle key 
		*/
			
		if(IDLE_SCREEN_ID != GetActiveScreenId())
		{	
			/* set del screen handler */
			SetDelScrnIDCallbackHandler(SCR_ID_PHOART_ICON_EDIT, mmi_phoart_icon_del_screen_id_hdlr);
			
			/* if it is exit because of interrupt */
			gdi_layer_get_buffer_ptr(&buf_ptr);


			#ifdef APPMEM_SUPPORT 			
//				g_phoart_cntx.interrupt_backup_buf = kal_app_mem_alloc(PHOART_FRAME_BUFFER_SIZE, KAL_APPMEM_APP, MOD_MMI); 
				g_phoart_cntx.interrupt_backup_buf = (PU8)(AllocMediaMemory(PHOART_FRAME_BUFFER_SIZE)); 
			#else
				back_buf_ptr = (PU8)camera_osd_layer_buffer;
				g_phoart_cntx.interrupt_backup_buf = &back_buf_ptr[PHOART_FRAME_BUFFER_SIZE];
			#endif /* APPMEM_SUPPORT */
			
			memcpy(g_phoart_cntx.interrupt_backup_buf, buf_ptr, PHOART_FRAME_BUFFER_SIZE);
		}
	
	}

	/* flatten layer */
	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, g_phoart_cntx.osd_layer_handle, 0, 0);
	gdi_layer_flatten_previous_to_base();

	/* stop timer */
	coolsand_UI_cancel_timer(mmi_phoart_icon_edit_move_icon_cyclic);
   
   /* free layer */
	gdi_layer_free(g_phoart_cntx.osd_layer_handle);
	
  	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, 0, 0, 0);
  	gdi_layer_multi_layer_disable();	

}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_del_screen_id_hdlr()
* DESCRIPTION
*   callback function when someone delete this screen's id
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static U8 mmi_phoart_icon_del_screen_id_hdlr(void *ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		

	/* free memory */	
	if(g_phoart_cntx.interrupt_backup_buf != NULL)
	{
		#ifdef APPMEM_SUPPORT
//			kal_app_mem_free((void*)g_phoart_cntx.interrupt_backup_buf);
			FreeMediaMemory((unsigned long)g_phoart_cntx.interrupt_backup_buf);
		#endif 		
		
		g_phoart_cntx.interrupt_backup_buf = NULL;
	}

	return FALSE;
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_draw_icon()
* DESCRIPTION
*   draw icon
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_draw_icon(U16 border_style)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 bbox_x1;
	S32 bbox_y1;
	S32 bbox_x2;
	S32 bbox_y2;
	S32 line_width;
	S32 i;
	gdi_color line_color;
	S32 clip_x1;
	S32 clip_x2;
	S32 clip_y1;
	S32 clip_y2;	
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	gdi_layer_push_and_set_active(g_phoart_cntx.icon_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);

	clip_x1 = g_phoart_cntx.resized_image_offset_x;
	clip_y1 = g_phoart_cntx.resized_image_offset_y;	
	clip_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1;
	clip_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1;

	if(clip_x1 > ICON_LAYER_X1)
	{
		clip_x1 = clip_x1-ICON_LAYER_X1;
	}
	else
	{
		clip_x1 = 0;
	}

	if(clip_y1 > ICON_LAYER_Y1)
	{
		clip_y1 = clip_y1-ICON_LAYER_Y1;
	}
	else
	{
		clip_y1 = 0;
	}

	if(ICON_LAYER_X2 > clip_x2)
	{
		clip_x2 = ICON_LAYER_WIDTH-(ICON_LAYER_X2-clip_x2);
	}
	else
	{
		clip_x2 = ICON_LAYER_WIDTH-1;
	}

	if(ICON_LAYER_Y2 > clip_y2)
	{
		clip_y2 = ICON_LAYER_HEIGHT-(ICON_LAYER_Y2-clip_y2);
	}
	else
	{
		clip_y2 = ICON_LAYER_HEIGHT-1;
	}
		

	gdi_layer_push_clip();
	gdi_layer_set_clip(clip_x1, clip_y1, clip_x2, clip_y2);
	
	gdi_image_draw_id(PHOART_ICON_BBOX_SHIFT, PHOART_ICON_BBOX_SHIFT, (U16)(IMG_ID_PHOART_ICON_START+g_phoart_cntx.selected_icon+1));	

	/* draw line */

	if(border_style == PHOART_BORDER_CORNER)
	{

		for(i = 0; i < 2; i++)
		{
			line_width = PHOART_ICON_BBOX_LINE_WIDTH-i;
		
			line_color = GDI_COLOR_RED;

			bbox_x1 = i;
			bbox_y1 = i;
			bbox_x2 = g_phoart_cntx.icon_width+PHOART_ICON_BBOX_SHIFT*2-1-i;
			bbox_y2 = g_phoart_cntx.icon_height+PHOART_ICON_BBOX_SHIFT*2-1-i;

			gdi_draw_line(bbox_x1, bbox_y1, bbox_x1+line_width, bbox_y1, line_color);
			gdi_draw_line(bbox_x1, bbox_y1, bbox_x1, bbox_y1+line_width, line_color);

			gdi_draw_line(bbox_x2, bbox_y1, bbox_x2-line_width, bbox_y1, line_color);
			gdi_draw_line(bbox_x2, bbox_y1, bbox_x2, bbox_y1+line_width, line_color);

			gdi_draw_line(bbox_x1, bbox_y2, bbox_x1+line_width, bbox_y2, line_color);
			gdi_draw_line(bbox_x1, bbox_y2, bbox_x1, bbox_y2-line_width, line_color);

			gdi_draw_line(bbox_x2, bbox_y2, bbox_x2-line_width, bbox_y2, line_color);
			gdi_draw_line(bbox_x2, bbox_y2, bbox_x2, bbox_y2-line_width, line_color);	
		}

	}
	else if(border_style == PHOART_BORDER_ALL)
	{
		for(i = 0; i < 2; i++)
		{
			line_color = GDI_COLOR_RED;

			bbox_x1 = i;
			bbox_y1 = i;
			bbox_x2 = g_phoart_cntx.icon_width+PHOART_ICON_BBOX_SHIFT*2-1-i;
			bbox_y2 = g_phoart_cntx.icon_height+PHOART_ICON_BBOX_SHIFT*2-1-i;

			
			gdi_draw_line(bbox_x1, bbox_y1, bbox_x2, bbox_y1, line_color);
			gdi_draw_line(bbox_x2, bbox_y1, bbox_x2, bbox_y2, line_color);
			gdi_draw_line(bbox_x2, bbox_y2, bbox_x1, bbox_y2, line_color);
			gdi_draw_line(bbox_x1, bbox_y2, bbox_x1, bbox_y1, line_color);
		}
	}
	
	gdi_layer_pop_clip();
	gdi_layer_pop_and_restore_active();	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_move_down_press()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_move_down_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_up_arrow_pressed)
	{   
		g_phoart_cntx.is_up_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0; 
		
		mmi_phoart_icon_edit_move_icon_cyclic(); 
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_move_up_press()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_move_up_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_down_arrow_pressed)
	{   
		g_phoart_cntx.is_down_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0; 
		
		mmi_phoart_icon_edit_move_icon_cyclic(); 
	}	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_move_left_press()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_move_left_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_left_arrow_pressed)
	{   
		g_phoart_cntx.is_left_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0; 
		
		mmi_phoart_icon_edit_move_icon_cyclic(); 
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_move_right_press()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_move_right_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_right_arrow_pressed)
	{   
		g_phoart_cntx.is_right_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0; 
		
		mmi_phoart_icon_edit_move_icon_cyclic(); 
	}	
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_move_down_release()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_move_down_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_up_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_icon_edit_move_icon_cyclic);  
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_move_up_release()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_move_up_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_down_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_icon_edit_move_icon_cyclic);  
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_move_left_release()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_move_left_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_left_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_icon_edit_move_icon_cyclic);  

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_move_right_release()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_move_right_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_right_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_icon_edit_move_icon_cyclic);  

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_move_icon_cyclic()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_move_icon_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   S32	move_step;
   S32  	diff;
   S32 	image_x1;
   S32 	image_x2;
   S32 	image_y1;
   S32 	image_y2; 
	S32	icon_center_x;
	S32 	icon_center_y;
	S32	ori_icon_x1;
	S32	ori_icon_y1;			
	S32	ori_icon_x2;	
	S32	ori_icon_y2;		

   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	icon_center_x = g_phoart_cntx.icon_offset_x + (g_phoart_cntx.icon_width>>1);
	icon_center_y = g_phoart_cntx.icon_offset_y + (g_phoart_cntx.icon_height>>1);

	image_x1 = g_phoart_cntx.resized_image_offset_x;
	image_y1 = g_phoart_cntx.resized_image_offset_y;	
	image_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1;
	image_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1;	

	ori_icon_x1 = g_phoart_cntx.icon_offset_x;
	ori_icon_y1 = g_phoart_cntx.icon_offset_y;
	ori_icon_x2 = g_phoart_cntx.icon_offset_x+g_phoart_cntx.icon_width-1; 	
	ori_icon_y2 = g_phoart_cntx.icon_offset_y+g_phoart_cntx.icon_height-1; 

   
	g_phoart_cntx.move_count++;

   if(g_phoart_cntx.move_count < 3)
   	move_step = 1;
   else if(g_phoart_cntx.move_count < 5)
   	move_step = 3;
   else if(g_phoart_cntx.move_count < 8)
   	move_step = 6;
   else if(g_phoart_cntx.move_count < 12)
   	move_step = 8; 
   else
   	move_step = 12;


   if(g_phoart_cntx.is_down_arrow_pressed)
  	{
		diff = (icon_center_y-move_step)-image_y1;
		
		if(diff > 0)
			g_phoart_cntx.icon_offset_y -= move_step;
		else
			g_phoart_cntx.icon_offset_y -= move_step+diff;
   }
   else if(g_phoart_cntx.is_up_arrow_pressed)
   {
		diff = image_y2-(icon_center_y+move_step);
	   
		if(diff > 0)
			g_phoart_cntx.icon_offset_y += move_step;
		else
			g_phoart_cntx.icon_offset_y += move_step+diff;
   }
   else if(g_phoart_cntx.is_left_arrow_pressed)
  	{
		diff = (icon_center_x-move_step)-image_x1;
	   
		if(diff > 0)
			g_phoart_cntx.icon_offset_x -= move_step;
		else
			g_phoart_cntx.icon_offset_x -= move_step+diff;
  	}
   else if(g_phoart_cntx.is_right_arrow_pressed)
   {
		diff = image_x2-(icon_center_x+move_step);
	   
		if(diff > 0)
			g_phoart_cntx.icon_offset_x += move_step;
		else
			g_phoart_cntx.icon_offset_x += move_step+diff;
   }
   else
   {
		MMI_ASSERT(0);
   }

	if(ICON_LAYER_X2 >= UI_device_width)
	{
		g_phoart_cntx.icon_offset_x = UI_device_width-ICON_LAYER_WIDTH+PHOART_ICON_BBOX_SHIFT;
	}

	if(ICON_LAYER_Y2 >= UI_device_height)
	{
		g_phoart_cntx.icon_offset_y = UI_device_height-ICON_LAYER_HEIGHT+PHOART_ICON_BBOX_SHIFT;
	}

   /* GDI cant flatten layer with neg postion, so force position at positive coord */
	if((g_phoart_cntx.icon_offset_x-PHOART_ICON_BBOX_SHIFT) < 0)
	{
		g_phoart_cntx.icon_offset_x = PHOART_ICON_BBOX_SHIFT;
	}

	if((g_phoart_cntx.icon_offset_y-PHOART_ICON_BBOX_SHIFT) < 0)
	{
		g_phoart_cntx.icon_offset_y = PHOART_ICON_BBOX_SHIFT;
	}

	gdi_layer_push_and_set_active(g_phoart_cntx.icon_layer_handle);
	gdi_layer_set_position(	ICON_LAYER_X1, ICON_LAYER_Y1);
	gdi_layer_pop_and_restore_active();	

	/* draw icon */
	mmi_phoart_icon_edit_draw_icon(PHOART_BORDER_CORNER);

   coolsand_UI_start_timer(100, mmi_phoart_icon_edit_move_icon_cyclic);
	
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	
}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_stamp()
* DESCRIPTION
*   stamp on image
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_stamp(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	mmi_phoart_icon_edit_draw_icon(PHOART_BORDER_ALL);   
   gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
   
	kal_sleep_task(30); /* sleep 30 tick */
   
	/*********** flatten **********/
	/* call this function to merge icon on to image, active must be base */
  	gdi_layer_push_and_set_active(g_phoart_cntx.base_layer_handle);
   
	mmi_phoart_icon_edit_draw_icon(PHOART_BORDER_NONE);   
	gdi_layer_flatten_to_base(g_phoart_cntx.base_layer_handle, g_phoart_cntx.icon_layer_handle, 0, 0);

	mmi_phoart_icon_edit_draw_icon(PHOART_BORDER_CORNER);   
   gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	
	gdi_layer_pop_and_restore_active();	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_stamp_when_done()
* DESCRIPTION
*   stamp on image
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_stamp_when_done(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/*********** flatten **********/
	/* call this function to merge icon on to image, active must be base */
  	gdi_layer_push_and_set_active(g_phoart_cntx.base_layer_handle);
	mmi_phoart_icon_edit_draw_icon(PHOART_BORDER_NONE);   
//	gdi_layer_flatten_to_base(g_phoart_cntx.base_layer_handle, g_phoart_cntx.icon_layer_handle, 0, 0);
	mmi_phoart_layer_flatten_to_base(g_phoart_cntx.base_layer_handle, g_phoart_cntx.icon_layer_handle, 0, 0);
	gdi_layer_pop_and_restore_active();	
}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_icon_edit_draw_osd()
* DESCRIPTION
*   draw add icon screen osd layer content
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_icon_edit_draw_osd(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* draw other UI compoment on OSD layer */
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);

	gdi_layer_clear(GDI_COLOR_TRANSPARENT);

	mmi_phoart_draw_image_border();

	/* softkey, title */
	mmi_phoart_draw_softkey((PS8)get_string(STR_GLOBAL_DONE), 
									(PS8)get_string(STR_GLOBAL_CANCEL),
									FALSE);				

	mmi_phoart_draw_title((PS8)get_string(STR_ID_PHOART_ADD_ICON_TITLE), NULL, FALSE);

	gdi_layer_pop_and_restore_active();
	
}



/***************************************************************************** 
*
* Add Text
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_text_editor_string_screen()
* DESCRIPTION
*   entry text selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_text_str_editor_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		*gui_buffer;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
   EntryNewScreen(SCR_ID_PHOART_TEXT_STR_EDITOR, NULL, mmi_phoart_entry_text_str_editor_screen, NULL );

   gui_buffer = GetCurrGuiBuffer(SCR_ID_PHOART_TEXT_STR_EDITOR); 

   ShowCategory5Screen(	STR_ID_PHOART_TEXT_EDITOR, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
			               STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
			               STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			               INPUT_TYPE_ALPHANUMERIC_UPPERCASE,
			               (U8*)g_phoart_setting->pa_add_text_string,
			               PHOART_TEXT_MAX_CHAR+1, 
			               gui_buffer);
   
	SetLeftSoftkeyFunction(mmi_phoart_entry_text_str_editor_option_screen,KEY_EVENT_UP);
	#if defined(__MMI_GB__)                                                              
	GBSetInputboxLSKFunction(mmi_phoart_entry_text_str_editor_option_screen);
       #endif
	SetCategory5RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction(mmi_phoart_return_to_editor_screen);

}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_add_text()
* DESCRIPTION
*   hilight save as menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_add_text(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_phoart_entry_text_str_editor_screen,KEY_EVENT_UP);
} 



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_text_editor_option_screen()
* DESCRIPTION
*   entry edit option screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_text_str_editor_option_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[16];
	U16	item_icons[16];
	U16	num_item;
	U8		*gui_buffer;
	U8		*popup_list[16];
	S32	i;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_TEXT_STR_EDITOR_OPTION, NULL, mmi_phoart_entry_text_str_editor_option_screen, NULL);

	gui_buffer 	= GetCurrGuiBuffer(SCR_ID_PHOART_TEXT_STR_EDITOR_OPTION);
	num_item		= GetNumOfChild(MENU_ID_PHOART_TEXT_EDITOR_OPTION);
	
	GetSequenceStringIds(MENU_ID_PHOART_TEXT_EDITOR_OPTION, item_list);
	GetSequenceImageIds(MENU_ID_PHOART_TEXT_EDITOR_OPTION, item_icons);
	SetParentHandler(MENU_ID_PHOART_TEXT_EDITOR_OPTION);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
	for(i = 0; i < num_item; i++)
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);

	for(i = 0 ; i < num_item; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_PHOART_TEXT_EDITOR_OPTION, popup_list);

	ShowCategory52Screen(STR_GLOBAL_OPTIONS,	
								MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_item, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								gui_buffer);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_hilight_editor_done()
* DESCRIPTION
*   hilight editor "done" menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_hilight_editor_done(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction(mmi_phoart_entry_text_select_screen, KEY_EVENT_UP );
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_hilight_editor_cancel()
* DESCRIPTION
*   hilight editor "cancel" menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_hilight_editor_cancel(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction(mmi_phoart_text_cancel_edit, KEY_EVENT_UP);
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_hilight_editor_input()
* DESCRIPTION
*   hilight editor "input" menuitem hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_hilight_editor_input(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction( EntryInputMethodScreen, KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_cancel_edit()
* DESCRIPTION
*   cancel text string edit
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_cancel_edit(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	DeleteUptoScrID(SCR_ID_PHOART_MAIN);
   GoBackHistory();
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_draw_style_text()
* DESCRIPTION
*   draw style text
* PARAMETERS
*	 offset_x		IN			start of text x position 	
*	 offset_y		IN			start of text y position 
*	 width			IN			string width
*	 height			IN			string height	
*	 str_ptr 		IN 		string pointer
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_draw_style_text(S32 offset_x, S32 offset_y, S32 width, S32 height, PS8 str_ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	if(r2lMMIFlag)
	{
		offset_x += coolsand_UI_get_string_width((UI_string_type)str_ptr);
	}
	
	coolsand_UI_set_font(&g_phoart_cntx.text_font);

	if(g_phoart_setting->pa_shadow_color)
	{
		if(g_phoart_setting->pa_border_color)
		{
			coolsand_UI_set_text_color(phoart_text_color_list[g_phoart_setting->pa_shadow_color]);	
			coolsand_UI_set_text_border_color(phoart_text_color_list[g_phoart_setting->pa_shadow_color]);		
			coolsand_UI_move_text_cursor(offset_x+1, offset_y+1);
		   coolsand_UI_print_bordered_text((UI_string_type)str_ptr);
		}
		else
		{
			coolsand_UI_set_text_color(phoart_text_color_list[g_phoart_setting->pa_shadow_color]);	
			coolsand_UI_move_text_cursor(offset_x+1, offset_y+1);
		   coolsand_UI_print_text((UI_string_type)str_ptr);
		}
	} 

	if(g_phoart_setting->pa_border_color)
	{
		coolsand_UI_set_text_color(phoart_text_color_list[g_phoart_setting->pa_text_color+1]);	
		coolsand_UI_set_text_border_color(phoart_text_color_list[g_phoart_setting->pa_border_color]);
		coolsand_UI_move_text_cursor(offset_x, offset_y);
	   coolsand_UI_print_bordered_text((UI_string_type)str_ptr);
	}
	else
	{
		coolsand_UI_set_text_color(phoart_text_color_list[g_phoart_setting->pa_text_color+1]);
		coolsand_UI_move_text_cursor(offset_x, offset_y);
	   coolsand_UI_print_text((UI_string_type)str_ptr);
	}
	
	coolsand_UI_BLT_double_buffer(offset_x, offset_y, offset_x+width, offset_y+height);

	/* retore to medium font */
	coolsand_UI_set_font(&MMI_medium_font);	
	
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_select_draw_text()
* DESCRIPTION
*   draw text on info box 
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_select_draw_text(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 	box_offset_x;
	S32 	box_offset_y;
	S32 	box_width;
	S32 	box_height;
	S32 	str_offset_x;
	S32 	str_offset_y;
	S32 	str_width;
	S32 	str_height;

	S32 	grid_size;
	S32 	grid_height;
	S32 	grid_row;
	S32 	grid_col;
	S32 	i, j;
	BOOL 	toggle;
	S8  	str_buf[(PHOART_TEXT_MAX_CHAR+10)*ENCODING_LENGTH];
	S32	str_len;
	S32	dot_width;
	S32	dot_height;
	S32	str_total_width;

	S32 	index;
	S32	char_width;
	S32	char_height;
	
	UI_character_type	*char_ptr;		
	gdi_color light_gray;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		

	coolsand_UI_reset_clip();
    

   /* draw text background grid */
	light_gray = GDI_RGB_TO_BUFFER_FORMAT(180, 180, 180);
	
	grid_size	= 7;
	grid_row		= 4;
	grid_height = grid_size*grid_row;

	box_width 	= g_phoart_cntx.text_info_x2-g_phoart_cntx.text_info_x1;
	box_height 	= g_phoart_cntx.text_info_y2-g_phoart_cntx.text_info_y1;	

	box_offset_y 	= (box_height-grid_height)>>1;
	grid_col 		= (box_width-box_offset_y)/grid_size;
	box_offset_x	= (box_width-(grid_col*grid_size))>>1;

	box_offset_x += g_phoart_cntx.text_info_x1;
	box_offset_y += g_phoart_cntx.text_info_y1;


	for(i = 0 ; i < grid_row; i++)
	{
		toggle = (i%2)?TRUE:FALSE;
		
		for(j = 0; j < grid_col; j++)
		{
			if(toggle)
			{
				gdi_draw_solid_rect(	box_offset_x+j*grid_size, 
											box_offset_y+i*grid_size, 
											box_offset_x+(j+1)*grid_size, 
											box_offset_y+(i+1)*grid_size,
											light_gray);

			}
			else
			{
				gdi_draw_solid_rect(	box_offset_x+j*grid_size, 
											box_offset_y+i*grid_size, 
											box_offset_x+(j+1)*grid_size, 
											box_offset_y+(i+1)*grid_size,
											GDI_COLOR_WHITE);
			}			
			toggle = toggle?FALSE:TRUE;

		}
		
	}
	
	
	/* draw stylied text */
	
	coolsand_UI_reset_text_clip();

	/* set font style */
	memcpy(&g_phoart_cntx.text_font, &MMI_medium_font, sizeof(stFontAttribute));
	g_phoart_cntx.text_font.italic = (g_phoart_setting->pa_text_italic)?TRUE:FALSE;

	if(g_phoart_setting->pa_text_size == PHOART_TEXT_SIZE_SMALL)
	{
		g_phoart_cntx.text_font.size = SMALL_FONT;
	}
	else if(g_phoart_setting->pa_text_size == PHOART_TEXT_SIZE_MEDIUM)
	{
		g_phoart_cntx.text_font.size = MEDIUM_FONT;
	}
	else if(g_phoart_setting->pa_text_size == PHOART_TEXT_SIZE_LARGE)
	{
		g_phoart_cntx.text_font.size = LARGE_FONT;
	}	
	else
	{
		MMI_ASSERT(0);
	}

	coolsand_UI_set_font(&g_phoart_cntx.text_font);
	coolsand_UI_measure_string((UI_string_type)g_phoart_setting->pa_add_text_string, &str_width, &str_height);


	coolsand_UI_lock_double_buffer();

	pfnUnicodeStrcpy((PS8)str_buf, (PS8)g_phoart_setting->pa_add_text_string);

	/* if string too long, change to xxxxxx.. */
	if(str_width > box_width-(grid_size>>1))
	{
		str_len = pfnUnicodeStrlen(str_buf);
		coolsand_UI_measure_string((UI_string_type)L"..", &dot_width, &dot_height);
	
		str_total_width = dot_width;
		char_ptr = (UI_character_type*)str_buf;
		
		for( index = 0 ; index < str_len; index++ )
		{
			coolsand_UI_measure_character(char_ptr[index], &char_width, &char_height);
				
			if( (str_total_width+char_width) > box_width-(grid_size>>1))
			{	
				pfnUnicodeStrcpy(&str_buf[(index-1)*ENCODING_LENGTH], (PS8)L"..");
				break;
			}
			else
				str_total_width+=char_width;
		}

		str_width = str_total_width;	

		str_offset_x = ((box_width-str_width)>>1)+(grid_size>>1);
		str_offset_y = (box_height-str_height)>>1;
		
	}
	else
	{
		str_offset_x = (box_width-str_width)>>1;
		str_offset_y = (box_height-str_height)>>1;
	}

		
	mmi_phoart_text_draw_style_text(	g_phoart_cntx.text_info_x1+str_offset_x, 
												g_phoart_cntx.text_info_y1+str_offset_y, 
												str_width, 
												str_height,
												str_buf);
	coolsand_UI_reset_text_clip();

	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(	g_phoart_cntx.text_info_x1,
											g_phoart_cntx.text_info_y1,
											g_phoart_cntx.text_info_x2,
											g_phoart_cntx.text_info_y2);

	coolsand_UI_set_font(&MMI_medium_font);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_inline_select_highlight_hdlr()
* DESCRIPTION
*   draw text on info box 
* PARAMETERS
*	 index 		IN		font size inline selection hilight handler
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_inline_select_highlight_hdlr(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   if(g_phoart_cntx.is_text_select_ready)
   {
 		mmi_phoart_text_select_draw_text();
   }
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_inline_select_highlight_hdlr()
* DESCRIPTION
*   hight handler for move down or up to select on different inline item
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_select_highlight_hdlr(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
  	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(mmi_phoart_text_select_done_hdlr); 
	
	g_phoart_cntx.text_select_highlight_index = index;
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_select_text_color_hilight_callback()
* DESCRIPTION
*   text color selection highlight handler
* PARAMETERS
*	 index 		IN		selected color index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_select_text_color_hilight_callback(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_setting->pa_text_color = index;
   
   if(g_phoart_cntx.is_text_select_ready)
   {
 		mmi_phoart_text_select_draw_text();
   }
} 



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_select_border_color_hilight_callback()
* DESCRIPTION
*   border color selection highlight handler
* PARAMETERS
*	 index 		IN		selected color index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_select_border_color_hilight_callback(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_setting->pa_border_color = index;
   
   if(g_phoart_cntx.is_text_select_ready)
   {
 		mmi_phoart_text_select_draw_text();
   }
} 



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_select_shadow_color_hilight_callback()
* DESCRIPTION
*   border color selection highlight handler
* PARAMETERS
*	 index 		IN		selected shadow color index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_select_shadow_color_hilight_callback(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_setting->pa_shadow_color = index;
   
   if(g_phoart_cntx.is_text_select_ready)
   {
 		mmi_phoart_text_select_draw_text();
   }
} 



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_inline_select_highlight_hdlr()
* DESCRIPTION
*   draw text on info box 
* PARAMETERS
*	 index 		IN		font size inline selection hilight handler
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_select_draw_info_callback(S32 index, S32 x1, S32 y1, S32 x2, S32 y2)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.text_info_x1 = x1;
	g_phoart_cntx.text_info_y1 = y1;
	g_phoart_cntx.text_info_x2 = x2;
	g_phoart_cntx.text_info_y2 = y2;	

	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();
	mmi_phoart_text_select_draw_text();
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_select_done_hdlr()
* DESCRIPTION
*   Category 57 select done handler
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_select_done_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   mmi_phoart_entry_text_edit_screen();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_phoart_entry_text_select_screen
 * DESCRIPTION
 *  entry text selection screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_phoart_entry_text_select_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *gui_buffer;
    U8 *input_buffer;
    U16 item_offset;
    U16 input_buffer_size;

    UI_image_ID_type item_icons[] = 
    {
        IMG_ID_PHOART_TEXT_TEXT_COLOR,
        0,
        IMG_ID_PHOART_TEXT_BORDER_COLOR,
        0,
        IMG_ID_PHOART_TEXT_SHADOW_COLOR,
        0,
        IMG_ID_PHOART_TEXT_SIZE,
        0,
        IMG_ID_PHOART_TEXT_ITALIC,
        0
    };

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* check if it is empty string */
    if (pfnUnicodeStrlen(g_phoart_setting->pa_add_text_string) == 0)
    {

        DisplayPopup(
            (PU8) GetString(STR_ID_PHOART_NOTIFY_EMPTY_STRING),
            IMG_GLOBAL_WARNING,
            1,
            PHOEDT_NOTIFY_DURATION,
            WARNING_TONE);
        return;
    }

    EntryNewScreen(SCR_ID_PHOART_TEXT_SELECT, mmi_phoart_exit_text_select_screen, NULL, NULL);
    gui_buffer = GetCurrGuiBuffer(SCR_ID_PHOART_TEXT_SELECT);
    input_buffer = GetCurrNInputBuffer(SCR_ID_PHOART_TEXT_SELECT, &input_buffer_size);

    g_phoart_cntx.is_text_select_ready = FALSE; /* callback function of inline selection will not update UI */

    g_phoart_cntx.italic_str[0] = (UI_string_type) GetString((U16) (STR_GLOBAL_OFF));
    g_phoart_cntx.italic_str[1] = (UI_string_type) GetString((U16) (STR_GLOBAL_ON));
    g_phoart_cntx.size_str[0] = (UI_string_type) GetString((U16) (STR_ID_PHOART_TEXT_SIZE_SMALL));
    g_phoart_cntx.size_str[1] = (UI_string_type) GetString((U16) (STR_ID_PHOART_TEXT_SIZE_MEDIUM));
    g_phoart_cntx.size_str[2] = (UI_string_type) GetString((U16) (STR_ID_PHOART_TEXT_SIZE_LARGE));

    /* clear inline item */
    memset((void*)wgui_inline_items, 0, sizeof(InlineItem) * PHOART_TEXT_SELECT_COUNT * 2);
    item_offset = 0;

    /* start of Category 57 */
    InitializeCategory57Screen();

    /* text color */
    SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8) GetString((U16) (STR_ID_PHOART_TEXT_TEXT_COLOR)));
    SetInlineItemActivation(&wgui_inline_items[item_offset], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

    SetInlineItemColorSelect(
        &wgui_inline_items[item_offset++],
        PHOART_TEXT_COLOR_COUNT - 1,    /* skip first one */
        (U16) g_phoart_setting->pa_text_color,
        (color_t*) & phoart_text_color_list[1],
        mmi_phoart_text_select_text_color_hilight_callback);

    /* border color */
    SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8) GetString((U16) (STR_ID_PHOART_TEXT_BORDER_COLOR)));
    SetInlineItemActivation(&wgui_inline_items[item_offset], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

    SetInlineItemColorSelect(
        &wgui_inline_items[item_offset++],
        PHOART_TEXT_COLOR_COUNT,
        (U16) g_phoart_setting->pa_border_color,
        (color_t*) phoart_text_color_list,
        mmi_phoart_text_select_border_color_hilight_callback);

    /* shadow color */
    SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8) GetString((U16) (STR_ID_PHOART_TEXT_SHADOW_COLOR)));
    SetInlineItemActivation(&wgui_inline_items[item_offset], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

    SetInlineItemColorSelect(
        &wgui_inline_items[item_offset++],
        PHOART_TEXT_COLOR_COUNT,
        (U16) g_phoart_setting->pa_shadow_color,
        (color_t*) phoart_text_color_list,
        mmi_phoart_text_select_shadow_color_hilight_callback);

    /* font size */
    SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8) GetString((U16) (STR_ID_PHOART_TEXT_SIZE)));
    SetInlineItemActivation(&wgui_inline_items[item_offset], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

    SetInlineItemSelect(
        &wgui_inline_items[item_offset++],
        3,
        (U8 **) g_phoart_cntx.size_str,
        &g_phoart_setting->pa_text_size);

    RegisterInlineSelectHighlightHandler(
        &wgui_inline_items[item_offset - 1],
        mmi_phoart_text_inline_select_highlight_hdlr);

    /* italic */
    SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8) GetString((U16) (STR_ID_PHOART_TEXT_ITALIC)));
    SetInlineItemActivation(&wgui_inline_items[item_offset], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

    SetInlineItemSelect(
        &wgui_inline_items[item_offset++],
        2,
        (U8 **) g_phoart_cntx.italic_str,
        &g_phoart_setting->pa_text_italic);

    RegisterInlineSelectHighlightHandler(
        &wgui_inline_items[item_offset - 1],
        mmi_phoart_text_inline_select_highlight_hdlr);

    if (input_buffer != NULL)
    {
        SetCategory57Data(wgui_inline_items, PHOART_TEXT_SELECT_COUNT * 2, input_buffer);
    }

    RegisterHighlightHandler(mmi_phoart_text_select_highlight_hdlr);

    /* append a info box on top of cateogory 57 screen */
    Category57AppendTopInfoBox(2, mmi_phoart_text_select_draw_info_callback, NULL, TRUE);

    /* show category */
    ShowCategory57Screen(
        STR_ID_PHOART_TEXT_STYLE,
        MAIN_MENU_TITLE_MULTIMEDIA_ICON,
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        item_offset,
        item_icons,
        wgui_inline_items,
        0,
        gui_buffer);

    /* set key hdlr */
    SetCategory57LeftSoftkeyFunction(mmi_phoart_text_select_done_hdlr);
    SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
    DisableCategory57ScreenDone();

    g_phoart_cntx.is_text_select_ready = TRUE;

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_text_select_screen()
* DESCRIPTION
*   exit text selection screen 
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_text_select_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 		input_buffer_size;                
	history_t 	h;
	S16		n_history = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	h.scrnID = SCR_ID_PHOART_TEXT_SELECT;
	CloseCategory57Screen();
	h.entryFuncPtr = mmi_phoart_entry_text_select_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer,(S8*)&n_history);
	GetCategory57History (h.guiBuffer);
	input_buffer_size = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*)h.inputBuffer);
	AddNHistory(h, input_buffer_size);

	/* store edit text to NVRAM */
	mmi_phoedt_store_data();
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_del_screen_id_hdlr()
* DESCRIPTION
*   callback function when someone delete this screen's id
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static U8 mmi_phoart_text_del_screen_id_hdlr(void *ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		

	/* free memory */	
	if(g_phoart_cntx.interrupt_backup_buf != NULL)
	{
		#ifdef APPMEM_SUPPORT
//			kal_app_mem_free((void*)g_phoart_cntx.interrupt_backup_buf);
			FreeMediaMemory((unsigned long)g_phoart_cntx.interrupt_backup_buf);
		#endif 
		
		g_phoart_cntx.interrupt_backup_buf = NULL;
	}

	return FALSE;
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_text_edit_screen()
* DESCRIPTION
*   entry text selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_text_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
   PU8	buf_ptr;
	U8		*gui_buffer;

	#ifndef APPMEM_SUPPORT 	
		PU8	add_item_buf_ptr;
	#endif /* !APPMEM_SUPPORT */
	
	/*----------------------------------------------------------------*/		
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_TEXT_EDIT, mmi_phoart_exit_text_edit_screen, mmi_phoart_entry_text_edit_screen, NULL);

	/* reset flag */
	g_phoart_cntx.is_user_goback = FALSE;

	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	gui_buffer = GetCurrGuiBuffer(SCR_ID_PHOART_TEXT_EDIT);

	/* entry full screen */
	entry_full_screen();

	/* inti variable */
	g_phoart_cntx.is_up_arrow_pressed 		= FALSE;
	g_phoart_cntx.is_down_arrow_pressed 	= FALSE;
	g_phoart_cntx.is_left_arrow_pressed 	= FALSE;
	g_phoart_cntx.is_right_arrow_pressed 	= FALSE;	


	#ifdef WIN32
		SetKeyHandler(mmi_phoart_text_edit_stamp, KEY_5, KEY_EVENT_DOWN);		
	#endif 
	
	SetKeyHandler(mmi_phoart_text_edit_move_up_press, KEY_UP_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_text_edit_move_down_press, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_text_edit_move_left_press, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_text_edit_move_right_press, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_text_edit_move_up_release, KEY_UP_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_text_edit_move_down_release, KEY_DOWN_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_text_edit_move_left_release, KEY_LEFT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_text_edit_move_right_release, KEY_RIGHT_ARROW, KEY_EVENT_UP);

	SetKeyHandler(mmi_phoart_text_edit_move_up_press, KEY_2, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_text_edit_move_down_press, KEY_8, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_text_edit_move_left_press, KEY_4, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_text_edit_move_right_press, KEY_6, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_text_edit_move_up_release, KEY_2, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_text_edit_move_down_release, KEY_8, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_text_edit_move_left_release, KEY_4, KEY_EVENT_UP);
	SetKeyHandler(mmi_phoart_text_edit_move_right_release, KEY_6, KEY_EVENT_UP);
	
	SetKeyHandler(mmi_phoart_text_edit_stamp, KEY_ENTER, KEY_EVENT_DOWN);
	
	/* register key hdlr */
	SetKeyHandler(mmi_phoart_lsk_press,	 	KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_lsk_release, 	KEY_LSK, KEY_EVENT_UP);	

	SetKeyHandler(mmi_phoart_rsk_press, 	KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_rsk_release, 	KEY_RSK, KEY_EVENT_UP);		
	
	/***************************************************************************** 
	* Layers
	*****************************************************************************/
	gdi_layer_multi_layer_enable();
	
	/* base layer */
	gdi_layer_get_active(&g_phoart_cntx.base_layer_handle);

	/* osd layer */
	gdi_layer_create(0, 0, UI_device_width, UI_device_height, &g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();
	
	
	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle,
									g_phoart_cntx.osd_layer_handle, 
									0, 0);


	/* drawing */
	gdi_layer_lock_frame_buffer();

	/* load original image from cache */
	gdi_layer_get_buffer_ptr(&buf_ptr);

	
	if(gui_buffer != NULL)
	{
		/* restore from interrupt */
		memcpy(buf_ptr, g_phoart_cntx.interrupt_backup_buf, PHOART_FRAME_BUFFER_SIZE);

		/* free backup memory */
		#ifdef APPMEM_SUPPORT 
//			kal_app_mem_free((void*)g_phoart_cntx.interrupt_backup_buf);
			FreeMediaMemory((unsigned long)g_phoart_cntx.interrupt_backup_buf);
		#endif
		
		g_phoart_cntx.interrupt_backup_buf = NULL;
	}		
	else
	{
		/* align text at center */
		coolsand_UI_set_font(&g_phoart_cntx.text_font);
		coolsand_UI_measure_string((UI_string_type)g_phoart_setting->pa_add_text_string, &g_phoart_cntx.text_width, &g_phoart_cntx.text_height);

		/* +1 for border */
		g_phoart_cntx.text_width += 1;
			
		g_phoart_cntx.text_offset_x = (UI_device_width-g_phoart_cntx.text_width)>>1;
		g_phoart_cntx.text_offset_y = (UI_device_height-g_phoart_cntx.text_height)>>1;

		/* load from original image cache */
		mmi_phoart_load_cache_image(buf_ptr);
	}

	/* allocate buffer for icon layer */
	#ifdef APPMEM_SUPPORT 			
//		g_phoart_cntx.add_item_buf= kal_app_mem_alloc(PHOART_FRAME_BUFFER_SIZE, KAL_APPMEM_APP, MOD_MMI); 
		g_phoart_cntx.add_item_buf= (PU8)(AllocMediaMemory(PHOART_FRAME_BUFFER_SIZE)); 
	#else
		add_item_buf_ptr = (PU8)camera_osd_layer_buffer;
		g_phoart_cntx.add_item_buf = &add_item_buf_ptr[PHOART_FRAME_BUFFER_SIZE];
	#endif /* APPMEM_SUPPORT */		

	/* 
		string maybe too long. move it to align at left border, since GDI currently cant 
		flatten layer with negative offset 
	*/

	if((g_phoart_cntx.text_offset_x-PHOART_TEXT_BBOX_SHIFT) < 0)
	{
		g_phoart_cntx.text_offset_x = PHOART_TEXT_BBOX_SHIFT;
	}

	/* create layer */
	gdi_layer_create_using_outside_memory(	TEXT_LAYER_X1, 
														TEXT_LAYER_Y1, 
														TEXT_LAYER_WIDTH, 
														TEXT_LAYER_HEIGHT,
														&g_phoart_cntx.text_layer_handle, 
														g_phoart_cntx.add_item_buf, 
														PHOART_FRAME_BUFFER_SIZE);
	
	gdi_layer_push_and_set_active(g_phoart_cntx.text_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();

	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle,
									g_phoart_cntx.text_layer_handle,
									g_phoart_cntx.osd_layer_handle,  
									0);
	
	mmi_phoart_text_edit_draw_osd();
	mmi_phoart_text_edit_draw_text(PHOART_BORDER_CORNER);

	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	
	#ifdef __MMI_TOUCH_SCREEN__
		mmi_wgui_register_pen_down_handler(mmi_phoart_pen_down_hdlr);
		mmi_wgui_register_pen_up_handler(mmi_phoart_pen_up_hdlr);
		mmi_wgui_register_pen_move_handler(mmi_phoart_pen_move_hdlr);
	#endif
}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_text_edit_screen()
* DESCRIPTION
*   entry text selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_text_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	PU8	buf_ptr;
   
   #ifndef APPMEM_SUPPORT 	
		PU8	back_buf_ptr;
   #endif /* !APPMEM_SUPPORT */
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

	gdi_layer_free(g_phoart_cntx.text_layer_handle);

	/* free add item layer & buffer */
	#ifdef APPMEM_SUPPORT 	
		/* free backup memory */
//		kal_app_mem_free((void*)g_phoart_cntx.add_item_buf);
		FreeMediaMemory((unsigned long)g_phoart_cntx.add_item_buf);
	#endif
		
	g_phoart_cntx.add_item_buf = NULL;


	if(g_phoart_cntx.is_user_goback == FALSE)
	{
		/*
			if not user exit screen. it might be 
			(1) interrupt.
			(2) user pressed idle key 
		*/
		
		if(IDLE_SCREEN_ID != GetActiveScreenId())
		{	
			/* set del screen handler */
			SetDelScrnIDCallbackHandler(SCR_ID_PHOART_TEXT_EDIT, mmi_phoart_text_del_screen_id_hdlr);
		
			/* if it is exit because of interrupt */
			gdi_layer_get_buffer_ptr(&buf_ptr);

			#ifdef APPMEM_SUPPORT 			
//				g_phoart_cntx.interrupt_backup_buf = kal_app_mem_alloc(PHOART_FRAME_BUFFER_SIZE, KAL_APPMEM_APP, MOD_MMI); 
				g_phoart_cntx.interrupt_backup_buf = (PU8)(AllocMediaMemory(PHOART_FRAME_BUFFER_SIZE)); 
			#else
				back_buf_ptr = (PU8)camera_osd_layer_buffer;
				g_phoart_cntx.interrupt_backup_buf = &back_buf_ptr[PHOART_FRAME_BUFFER_SIZE];
			#endif /* APPMEM_SUPPORT */		

			memcpy(g_phoart_cntx.interrupt_backup_buf, buf_ptr, PHOART_FRAME_BUFFER_SIZE);
		}
	}

	/* flatten layer */
	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, g_phoart_cntx.osd_layer_handle, 0, 0);
	gdi_layer_flatten_previous_to_base();

	
	/* stop timer */
	coolsand_UI_cancel_timer(mmi_phoart_text_edit_move_text_cyclic);
	
   
   /* free layer */
	gdi_layer_free(g_phoart_cntx.osd_layer_handle);

  	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, 0, 0, 0);
  	gdi_layer_multi_layer_disable();	


}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_draw_text()
* DESCRIPTION
*   draw text
* PARAMETERS
*	 border_style		IN 	border style
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_draw_text(U16 border_style)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 bbox_x1;
	S32 bbox_y1;
	S32 bbox_x2;
	S32 bbox_y2;
	S32 line_width;
	S32 i;
	gdi_color line_color;
	S32 clip_x1;
	S32 clip_x2;
	S32 clip_y1;
	S32 clip_y2;	
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	gdi_layer_push_and_set_active(g_phoart_cntx.text_layer_handle);   	
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);

	clip_x1 = g_phoart_cntx.resized_image_offset_x;
	clip_y1 = g_phoart_cntx.resized_image_offset_y;	
	clip_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1;
	clip_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1;

	if(clip_x1 > TEXT_LAYER_X1)
	{
		clip_x1 = clip_x1-TEXT_LAYER_X1;
	}
	else
	{
		clip_x1 = 0;
	}

	if(clip_y1 > TEXT_LAYER_Y1)
	{
		clip_y1 = clip_y1-TEXT_LAYER_Y1;
	}
	else
	{
		clip_y1 = 0;
	}

	if(TEXT_LAYER_X2 > clip_x2)
	{
		clip_x2 = TEXT_LAYER_WIDTH-(TEXT_LAYER_X2-clip_x2);
	}
	else
	{
		clip_x2 = TEXT_LAYER_WIDTH-1;
	}

	if(TEXT_LAYER_Y2 > clip_y2)
	{
		clip_y2 = TEXT_LAYER_HEIGHT-(TEXT_LAYER_Y2-clip_y2);
	}
	else
	{
		clip_y2 = TEXT_LAYER_HEIGHT-1;
	}

	gdi_layer_push_clip();
	gdi_layer_set_clip(clip_x1, clip_y1, clip_x2, clip_y2);


	coolsand_UI_set_font(&g_phoart_cntx.text_font);
	
	mmi_phoart_text_draw_style_text(	PHOART_TEXT_BBOX_SHIFT, 
												PHOART_TEXT_BBOX_SHIFT,
												g_phoart_cntx.text_width,
												g_phoart_cntx.text_height,
												g_phoart_setting->pa_add_text_string);


	if(border_style == PHOART_BORDER_CORNER)
	{
		for(i = 0; i < 2; i++)
		{
			line_width = PHOART_TEXT_BBOX_LINE_WIDTH-i;
		
			line_color = GDI_COLOR_RED;

			bbox_x1 = i;
			bbox_y1 = i;
			bbox_x2 = g_phoart_cntx.text_width+PHOART_TEXT_BBOX_SHIFT*2-1-i;
			bbox_y2 = g_phoart_cntx.text_height+PHOART_TEXT_BBOX_SHIFT*2-1-i;

			
			gdi_draw_line(bbox_x1, bbox_y1, bbox_x1+line_width, bbox_y1, line_color);
			gdi_draw_line(bbox_x1, bbox_y1, bbox_x1, bbox_y1+line_width, line_color);

			gdi_draw_line(bbox_x2, bbox_y1, bbox_x2-line_width, bbox_y1, line_color);
			gdi_draw_line(bbox_x2, bbox_y1, bbox_x2, bbox_y1+line_width, line_color);

			gdi_draw_line(bbox_x1, bbox_y2, bbox_x1+line_width, bbox_y2, line_color);
			gdi_draw_line(bbox_x1, bbox_y2, bbox_x1, bbox_y2-line_width, line_color);

			gdi_draw_line(bbox_x2, bbox_y2, bbox_x2-line_width, bbox_y2, line_color);
			gdi_draw_line(bbox_x2, bbox_y2, bbox_x2, bbox_y2-line_width, line_color);
		}

	}
	else if(border_style == PHOART_BORDER_ALL)
	{
		for(i = 0; i < 2; i++)
		{
			line_color = GDI_COLOR_RED;

			bbox_x1 = i;
			bbox_y1 = i;
			bbox_x2 = g_phoart_cntx.text_width+PHOART_TEXT_BBOX_SHIFT*2-1-i;
			bbox_y2 = g_phoart_cntx.text_height+PHOART_TEXT_BBOX_SHIFT*2-1-i;

			gdi_draw_line(bbox_x1, bbox_y1, bbox_x2, bbox_y1, line_color);
			gdi_draw_line(bbox_x2, bbox_y1, bbox_x2, bbox_y2, line_color);
			gdi_draw_line(bbox_x2, bbox_y2, bbox_x1, bbox_y2, line_color);
			gdi_draw_line(bbox_x1, bbox_y2, bbox_x1, bbox_y1, line_color);
		}
	}

	/* reset font */
	coolsand_UI_set_font(&MMI_medium_font);

	gdi_layer_pop_clip();	
	gdi_layer_pop_and_restore_active();
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_move_down_press()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_move_down_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_up_arrow_pressed)
	{   
		g_phoart_cntx.is_up_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0; 
		
		mmi_phoart_text_edit_move_text_cyclic(); 
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_move_up_press()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_move_up_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_down_arrow_pressed)
	{   
		g_phoart_cntx.is_down_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0; 
		
		mmi_phoart_text_edit_move_text_cyclic(); 
	}	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_move_left_press()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_move_left_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_left_arrow_pressed)
	{   
		g_phoart_cntx.is_left_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0; 
		
		mmi_phoart_text_edit_move_text_cyclic(); 
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_move_right_press()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_move_right_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(!g_phoart_cntx.is_right_arrow_pressed)
	{   
		g_phoart_cntx.is_right_arrow_pressed = TRUE;
		g_phoart_cntx.move_count 				= 0; 
		
		mmi_phoart_text_edit_move_text_cyclic(); 
	}	
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_move_down_release()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_move_down_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_up_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_text_edit_move_text_cyclic);  
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_move_up_release()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_move_up_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_down_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_text_edit_move_text_cyclic);  
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_move_left_release()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_move_left_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_left_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_text_edit_move_text_cyclic);  

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_move_right_release()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_move_right_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.is_right_arrow_pressed = FALSE;
   coolsand_UI_cancel_timer(mmi_phoart_text_edit_move_text_cyclic);  

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_move_text_cyclic()
* DESCRIPTION
*   move up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_move_text_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   S32	move_step;
   S32  	diff;
   S32 	image_x1;
   S32 	image_x2;
   S32 	image_y1;
   S32 	image_y2; 
	S32	text_center_x;
	S32 	text_center_y;
	S32	ori_text_x1;
	S32	ori_text_y1;			
	S32	ori_text_x2;	
	S32	ori_text_y2;		

   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	text_center_x = g_phoart_cntx.text_offset_x + (g_phoart_cntx.text_width>>1);
	text_center_y = g_phoart_cntx.text_offset_y + (g_phoart_cntx.text_height>>1);

	image_x1 = g_phoart_cntx.resized_image_offset_x;
	image_y1 = g_phoart_cntx.resized_image_offset_y;	
	image_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1;
	image_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1;	

	ori_text_x1 = g_phoart_cntx.text_offset_x;
	ori_text_y1 = g_phoart_cntx.text_offset_y;
	ori_text_x2 = g_phoart_cntx.text_offset_x+g_phoart_cntx.text_width-1; 	
	ori_text_y2 = g_phoart_cntx.text_offset_y+g_phoart_cntx.text_height-1; 

   
	g_phoart_cntx.move_count++;

   if(g_phoart_cntx.move_count < 3)
   	move_step = 1;
   else if(g_phoart_cntx.move_count < 5)
   	move_step = 3;
   else if(g_phoart_cntx.move_count < 8)
   	move_step = 6;
   else if(g_phoart_cntx.move_count < 12)
   	move_step = 8; 
   else
   	move_step = 12;


   if(g_phoart_cntx.is_down_arrow_pressed)
  	{
		diff = (text_center_y-move_step)-image_y1;
		
		if(diff > 0)
			g_phoart_cntx.text_offset_y -= move_step;
		else
			g_phoart_cntx.text_offset_y -= move_step+diff;
   }
   else if(g_phoart_cntx.is_up_arrow_pressed)
   {
		diff = image_y2-(text_center_y+move_step);
	   
		if(diff > 0)
			g_phoart_cntx.text_offset_y += move_step;
		else
			g_phoart_cntx.text_offset_y += move_step+diff;
   }
   else if(g_phoart_cntx.is_left_arrow_pressed)
  	{
		diff = (text_center_x-move_step)-image_x1;
	   
		if(diff > 0)
			g_phoart_cntx.text_offset_x -= move_step;
		else
			g_phoart_cntx.text_offset_x -= move_step+diff;
  	}
   else if(g_phoart_cntx.is_right_arrow_pressed)
   {
		diff = image_x2-(text_center_x+move_step);
	   
		if(diff > 0)
			g_phoart_cntx.text_offset_x += move_step;
		else
			g_phoart_cntx.text_offset_x += move_step+diff;
   }
   else
   {
		MMI_ASSERT(0);
   }

	if(TEXT_LAYER_X2 >= UI_device_width)
	{
		g_phoart_cntx.text_offset_x = UI_device_width-TEXT_LAYER_WIDTH+PHOART_TEXT_BBOX_SHIFT;
	}

	if(TEXT_LAYER_Y2 >= UI_device_height)
	{
		g_phoart_cntx.text_offset_y = UI_device_height-TEXT_LAYER_HEIGHT+PHOART_TEXT_BBOX_SHIFT;
	}

   /* GDI cant flatten layer with neg postion, so force position at positive coord */
	if((g_phoart_cntx.text_offset_x-PHOART_TEXT_BBOX_SHIFT) < 0)
	{
		g_phoart_cntx.text_offset_x = PHOART_TEXT_BBOX_SHIFT;
	}

	if((g_phoart_cntx.text_offset_y-PHOART_TEXT_BBOX_SHIFT) < 0)
	{
		g_phoart_cntx.text_offset_y = PHOART_TEXT_BBOX_SHIFT;
	}

	gdi_layer_push_and_set_active(g_phoart_cntx.text_layer_handle);
	gdi_layer_set_position(	TEXT_LAYER_X1, TEXT_LAYER_Y1);
	gdi_layer_pop_and_restore_active();	

	/* draw text */
	mmi_phoart_text_edit_draw_text(PHOART_BORDER_CORNER);

   coolsand_UI_start_timer(100, mmi_phoart_text_edit_move_text_cyclic);
	
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	
}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_stamp()
* DESCRIPTION
*   stamp on image
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_stamp(void)
{
  /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	mmi_phoart_text_edit_draw_text(PHOART_BORDER_ALL);   
   gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
   
	kal_sleep_task(30); /* sleep 30 tick */
   
	/*********** flatten **********/
	/* call this function to merge icon on to image, active must be base */
  	gdi_layer_push_and_set_active(g_phoart_cntx.base_layer_handle);
   
	mmi_phoart_text_edit_draw_text(PHOART_BORDER_NONE);   
	gdi_layer_flatten_to_base(g_phoart_cntx.base_layer_handle, g_phoart_cntx.text_layer_handle, 0, 0);

	mmi_phoart_text_edit_draw_text(PHOART_BORDER_CORNER);   
   gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	
	gdi_layer_pop_and_restore_active();		
	
}


int mmi_blendPartPure(UINT16 *baseBuffer,UINT16 baseBufW,UINT16 baseBufH,UINT16 *relBuffer,UINT16 relBufW,UINT16 relBufH,UINT16 transfCol,UINT16 *outBuffer,UINT16 xOffsetOfBase,UINT16 yOffsetOfBase)
{
	
//	UINT16 *tmpBuffer = baseBuffer +  xOffsetOfBase*yOffsetOfBase;
	UINT32 baseMax=baseBufW*baseBufH,relMax = relBufW*relBufH;
	UINT32 baseVW=0,baseVH=0/*,baseVi=0*/;
	if(baseMax>=relMax)
	{

		for(baseVH=0;baseVH<baseBufH;baseVH++)
		{
			for(baseVW=0;baseVW<baseBufW;baseVW++)
			{
				if((baseVH>=yOffsetOfBase &&baseVH<(yOffsetOfBase+relBufH)) && (baseVW>= xOffsetOfBase&&baseVW<(xOffsetOfBase+relBufW)))
				{
					if(relBuffer[(baseVH-yOffsetOfBase)*relBufW+(baseVW-xOffsetOfBase)] != transfCol)
					{
						outBuffer[baseVH*baseBufW+baseVW]=(relBuffer[(baseVH-yOffsetOfBase)*relBufW+(baseVW-xOffsetOfBase)]);
					}
					else
					{
						outBuffer[baseVH*baseBufW+baseVW] = baseBuffer[baseVH*baseBufW+baseVW];
					}
				}
				else
				{
					outBuffer[baseVH*baseBufW+baseVW] = baseBuffer[baseVH*baseBufW+baseVW];
				}
			}
		}
		return 0;
	}
	return -1;
}

void mmi_phoart_layer_flatten_to_base(gdi_handle handle0, gdi_handle handle1,gdi_handle handle2, gdi_handle handle3)
{
	int ret;
	U8	 *base_buf_ptr;
	U8	 *ref_buf_ptr;
	S32 base_layer_width=0,ref_layer_width=0;
	S32 base_layer_height = 0,ref_layer_height = 0;
	S32 base_offset_x = 0,ref_offset_x = 0;
	S32 base_offset_y = 0,ref_offset_y = 0;
	gdi_layer_push_and_set_active(handle0);

	gdi_layer_get_buffer_ptr(&base_buf_ptr);
	gdi_layer_get_dimension(&base_layer_width, &base_layer_height);
	gdi_layer_get_position(&base_offset_x, &base_offset_y);

	gdi_layer_pop_and_restore_active();

	gdi_layer_push_and_set_active(handle1);

	gdi_layer_get_buffer_ptr(&ref_buf_ptr);
	gdi_layer_get_dimension(&ref_layer_width, &ref_layer_height);
	gdi_layer_get_position(&ref_offset_x, &ref_offset_y);

	gdi_layer_pop_and_restore_active();

	mmi_trace(1," phoart_layer_flatten_to_base %d %d %d %d",base_layer_width,base_layer_height,base_offset_x,base_offset_y);
	mmi_trace(1," phoart_layer_flatten_to_base %d %d %d %d",ref_layer_width,ref_layer_height,ref_offset_x,ref_offset_y);
	ret = mmi_blendPartPure((UINT16*)base_buf_ptr,base_layer_width,base_layer_height,(UINT16 *)ref_buf_ptr,ref_layer_width,ref_layer_height,GDI_COLOR_TRANSPARENT,(UINT16 *)base_buf_ptr,ref_offset_x,ref_offset_y);
	mmi_trace(1," phoart_layer_flatten_to_base ret=%d",ret);

}
/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_stamp_when_done()
* DESCRIPTION
*   stamp on image when press lsk "Done"
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_stamp_when_done(void)
{
  /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/*********** flatten **********/
	/* call this function to merge icon on to image, active must be base */
  	gdi_layer_push_and_set_active(g_phoart_cntx.base_layer_handle);
  	mmi_phoart_text_edit_draw_text(PHOART_BORDER_NONE);   
//	gdi_layer_flatten_to_base(g_phoart_cntx.base_layer_handle, g_phoart_cntx.text_layer_handle, 0, 0);
	mmi_phoart_layer_flatten_to_base(g_phoart_cntx.base_layer_handle, g_phoart_cntx.text_layer_handle, 0, 0);
	gdi_layer_pop_and_restore_active();		
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_text_edit_draw_osd()
* DESCRIPTION
*   draw add text screen osd layer content
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_text_edit_draw_osd(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* draw other UI compoment on OSD layer */
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);

	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	
	mmi_phoart_draw_image_border();

	/* restore softkey, title */
	mmi_phoart_draw_softkey((PS8)get_string(STR_GLOBAL_DONE), 
									(PS8)get_string(STR_GLOBAL_CANCEL),
									FALSE);				

	mmi_phoart_draw_title((PS8)get_string(STR_ID_PHOART_ADD_TEXT_TITLE), NULL, FALSE);

	gdi_layer_pop_and_restore_active();
	
}




/***************************************************************************** 
*
* Adjsutment
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_adjust_option_screen()
* DESCRIPTION
*   entry adjust option screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_adjust_option_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[16];
	U16	item_icons[16];
	U16	num_item;
	U8		*gui_buffer;
	U8		*popup_list[16];
	S32	i;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_ADJUST_OPTION, NULL, mmi_phoart_entry_adjust_option_screen, NULL);

	gui_buffer 	= GetCurrGuiBuffer(SCR_ID_PHOART_ADJUST_OPTION);
	num_item		= GetNumOfChild(MENU_ID_PHOART_ADJUST_OPTION);
	
	GetSequenceStringIds(MENU_ID_PHOART_ADJUST_OPTION, item_list);
	GetSequenceImageIds(MENU_ID_PHOART_ADJUST_OPTION, item_icons);
	SetParentHandler(MENU_ID_PHOART_ADJUST_OPTION);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i = 0; i < num_item; i++)
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);

	for(i = 0 ; i < num_item; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_PHOART_OPTION, popup_list);

	ShowCategory52Screen(STR_ID_PHOART_ADJUST,	
								MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_item, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								gui_buffer);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_hilight_option()
* DESCRIPTION
*   hilight "adjustment"
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_hilight_option(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction(mmi_phoart_entry_adjust_option_screen, KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_hilight_image_adjust()
* DESCRIPTION
*   hilight "image adjust"
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_hilight_image_adjust(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction(mmi_phoart_entry_adjust_edit_screen, KEY_EVENT_UP);

	g_phoart_cntx.adjust_type = PHOART_ADJUST_IMAGE_TYPE;	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_hilight_color_adjust()
* DESCRIPTION
*   hilight "color adjust"
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_hilight_color_adjust(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(mmi_phoart_entry_adjust_edit_screen, KEY_EVENT_UP);

	g_phoart_cntx.adjust_type = PHOART_ADJUST_COLOR_TYPE;	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_adjust_edit_screen()
* DESCRIPTION
*   entry adjust edit screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_adjust_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		*gui_buffer;

	/*----------------------------------------------------------------*/		
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOART_ADJUST_EDIT, mmi_phoart_exit_adjust_edit_screen, mmi_phoart_entry_adjust_edit_screen, NULL);

	gui_buffer = GetCurrGuiBuffer(SCR_ID_PHOART_ADJUST_EDIT);

	if(gui_buffer == NULL)
	{
		/* init adjust selection */
		g_phoart_cntx.adjust_image_index = 0;
		g_phoart_cntx.adjust_color_index = 0;		
		
		mmi_phoart_adjust_edit_init_list();
	}

	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	/* entry full screen */
	entry_full_screen();

	if(g_phoart_cntx.adjust_type == PHOART_ADJUST_IMAGE_TYPE)
	{
		SetKeyHandler(mmi_phoart_adjust_edit_image_list_up, KEY_UP_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_phoart_adjust_edit_image_list_down, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	}
	else if(g_phoart_cntx.adjust_type == PHOART_ADJUST_COLOR_TYPE)
	{
		SetKeyHandler(mmi_phoart_adjust_edit_color_list_up, KEY_UP_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_phoart_adjust_edit_color_list_down, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	}
	else
	{
		MMI_ASSERT(0);
	}
	
	SetKeyHandler(mmi_phoart_adjust_edit_right_arrow_press, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_adjust_edit_left_arrow_press, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	/* register key hdlr */
	SetKeyHandler(mmi_phoart_lsk_press,	 	KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_lsk_release, 	KEY_LSK, KEY_EVENT_UP);	

	SetKeyHandler(mmi_phoart_rsk_press, 	KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_rsk_release, 	KEY_RSK, KEY_EVENT_UP);	

	
	/***************************************************************************** 
	* Layers
	*****************************************************************************/
	gdi_layer_multi_layer_enable();
	
	/* base layer */
	gdi_layer_get_active(&g_phoart_cntx.base_layer_handle);

	/* osd layer */
	gdi_layer_create(0, 0, UI_device_width, UI_device_height, &g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();

	
	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle,
									g_phoart_cntx.osd_layer_handle,  
									0, 0);

	/* drawing */
	gdi_layer_lock_frame_buffer();

	

	if(g_phoart_cntx.adjust_type == PHOART_ADJUST_IMAGE_TYPE)
	{
		mmi_phoart_adjust_edit_image_draw_osd();
	}
	else if(g_phoart_cntx.adjust_type == PHOART_ADJUST_COLOR_TYPE)
	{
		mmi_phoart_adjust_edit_color_draw_osd();
	}
	else
	{
		MMI_ASSERT(0);
	}

	/* draw adjusted frame on base layer */
	mmi_phoart_adjust_edit_add_adjustment();

	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

	#ifdef __MMI_TOUCH_SCREEN__
		mmi_wgui_register_pen_down_handler(mmi_phoart_pen_down_hdlr);
		mmi_wgui_register_pen_up_handler(mmi_phoart_pen_up_hdlr);
		mmi_wgui_register_pen_move_handler(mmi_phoart_pen_move_hdlr);
		mmi_wgui_register_pen_repeat_handler(mmi_phoart_pen_repeat_hdlr);
	#endif

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_adjust_edit_screen()
* DESCRIPTION
*   exit adjust edit screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_adjust_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
   /* free layer */
	gdi_layer_flatten_previous_to_base();

	gdi_layer_free(g_phoart_cntx.osd_layer_handle);

  	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, 0, 0, 0);
  	gdi_layer_multi_layer_disable();	
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_add_adjustment()
* DESCRIPTION
*   press right arrow
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_add_adjustment(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
   PU8	buf_ptr;
	S32	i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	/* load original image from cache to [Base layer] */
	gdi_layer_get_buffer_ptr(&buf_ptr);
	mmi_phoart_load_cache_image(buf_ptr);

	if(g_phoart_cntx.adjust_type == PHOART_ADJUST_IMAGE_TYPE)
	{
		for(i = 0 ; i < PHOART_ADJUST_IMAGE_TYPE_COUNT; i++)
		{
			mdi_camera_add_post_adjustment(	g_phoart_cntx.base_layer_handle, 
														phoart_adjust_image_command_map[i],
														g_phoart_cntx.adjust_image_value[i]);
		}
	}
	else if(g_phoart_cntx.adjust_type == PHOART_ADJUST_COLOR_TYPE)
	{	
		for(i = 0 ; i < PHOART_ADJUST_COLOR_TYPE_COUNT; i++)
		{
			mdi_camera_add_post_adjustment(	g_phoart_cntx.base_layer_handle, 
														phoart_adjust_color_command_map[i],
														g_phoart_cntx.adjust_color_value[i]);
		}	
	}
	else
	{
		MMI_ASSERT(0);
	}	

	/* some adjustment will apply to full screen, we have to paint bg color back when image is smaller than fullscreen */

	/*
		*******************
		*		  (1)			*
		*						*	
		*-----*******-----*
		*		*******		*
		* (3)	******* (4)	*
		*		*******		*
		*-----*******-----*
		*						*
		*		  (2)			*
		*******************
	*/
	
	if((g_phoart_cntx.resized_image_width != UI_device_width) || 
		(g_phoart_cntx.resized_image_height != UI_device_height))
	{
		/* region (1) */
		gdi_draw_solid_rect(	0, 
									0, 
									UI_device_width-1, 
									g_phoart_cntx.resized_image_offset_y-1, 
									PHOART_MAIN_CANVAS_COLOR);

		/* region (2) */
		gdi_draw_solid_rect(	0, 
									g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height, 
									UI_device_width-1, 
									UI_device_height-1, 
									PHOART_MAIN_CANVAS_COLOR);

		/* region (3) */
		gdi_draw_solid_rect( 0, 
									g_phoart_cntx.resized_image_offset_y, 
									g_phoart_cntx.resized_image_offset_x-1, 
									g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1, 
									PHOART_MAIN_CANVAS_COLOR);

		/* region (4) */
		gdi_draw_solid_rect( g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width, 
									g_phoart_cntx.resized_image_offset_y, 
									UI_device_width-1, 
									g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1, 
									PHOART_MAIN_CANVAS_COLOR);


	}

}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_right_arrow_press()
* DESCRIPTION
*   press right arrow
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_right_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 index;
	S32 next_value;


	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_phoart_cntx.adjust_type == PHOART_ADJUST_IMAGE_TYPE)
	{
		index = g_phoart_cntx.adjust_image_index;
		next_value = g_phoart_cntx.adjust_image_value[index]+g_phoart_cntx.adjust_image_step_value[index];
		
		if(next_value >  g_phoart_cntx.adjust_image_max_value[index])
		{
			g_phoart_cntx.adjust_image_value[index] = g_phoart_cntx.adjust_image_max_value[index];
		}
		else
		{
			g_phoart_cntx.adjust_image_value[index] = next_value;
		}

		mmi_phoart_adjust_edit_image_draw_osd();

		/* add effect to image */
		mmi_phoart_adjust_edit_add_adjustment();
		
		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);
		
	}
	else if(g_phoart_cntx.adjust_type == PHOART_ADJUST_COLOR_TYPE)
	{
		index = g_phoart_cntx.adjust_color_index;
		next_value = g_phoart_cntx.adjust_color_value[index]+g_phoart_cntx.adjust_color_step_value[index];
		
		if(next_value >  g_phoart_cntx.adjust_color_max_value[index])
		{
			g_phoart_cntx.adjust_color_value[index] = g_phoart_cntx.adjust_color_max_value[index];
		}
		else
		{
			g_phoart_cntx.adjust_color_value[index] = next_value;
		}

		mmi_phoart_adjust_edit_color_draw_osd();

		/* add effect to image */
		mmi_phoart_adjust_edit_add_adjustment();

		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

		
	}
	else
	{
		MMI_ASSERT(0);
	}

	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_left_arrow_press()
* DESCRIPTION
*   press right arrow
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_left_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 index;
	S32 next_value;

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_phoart_cntx.adjust_type == PHOART_ADJUST_IMAGE_TYPE)
	{
		index = g_phoart_cntx.adjust_image_index;
		next_value = g_phoart_cntx.adjust_image_value[index]-g_phoart_cntx.adjust_image_step_value[index];
		
		if(next_value <  g_phoart_cntx.adjust_image_min_value[index])
		{
			g_phoart_cntx.adjust_image_value[index] = g_phoart_cntx.adjust_image_min_value[index];
		}
		else
		{
			g_phoart_cntx.adjust_image_value[index] = next_value;
		}

		mmi_phoart_adjust_edit_image_draw_osd();

		/* add effect to image */
		mmi_phoart_adjust_edit_add_adjustment();
		
		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

		
	}
	else if(g_phoart_cntx.adjust_type == PHOART_ADJUST_COLOR_TYPE)
	{
		index = g_phoart_cntx.adjust_color_index;
		next_value = g_phoart_cntx.adjust_color_value[index]-g_phoart_cntx.adjust_color_step_value[index];
		
		if(next_value <  g_phoart_cntx.adjust_color_min_value[index])
		{
			g_phoart_cntx.adjust_color_value[index] = g_phoart_cntx.adjust_color_min_value[index];
		}
		else
		{
			g_phoart_cntx.adjust_color_value[index] = next_value;
		}

		mmi_phoart_adjust_edit_color_draw_osd();

		/* add effect to image */
		mmi_phoart_adjust_edit_add_adjustment();

		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);
		
	}
	else
	{
		MMI_ASSERT(0);
	}


}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_init_list()
* DESCRIPTION
*   init slider value
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_init_list(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_phoart_cntx.adjust_image_value[PHOART_ADJUST_IMAGE_BRIGHTNESS]	= 128;
	g_phoart_cntx.adjust_image_value[PHOART_ADJUST_IMAGE_CONTRAST]		= 128;
	g_phoart_cntx.adjust_image_value[PHOART_ADJUST_IMAGE_SATURATION]	= 128;	
	g_phoart_cntx.adjust_image_value[PHOART_ADJUST_IMAGE_HUE]			= 0;

	g_phoart_cntx.adjust_image_min_value[PHOART_ADJUST_IMAGE_BRIGHTNESS]	= 8;
	g_phoart_cntx.adjust_image_min_value[PHOART_ADJUST_IMAGE_CONTRAST]	= 8;
	g_phoart_cntx.adjust_image_min_value[PHOART_ADJUST_IMAGE_SATURATION]	= 8;	
	g_phoart_cntx.adjust_image_min_value[PHOART_ADJUST_IMAGE_HUE]			= 0;
	
	g_phoart_cntx.adjust_image_max_value[PHOART_ADJUST_IMAGE_BRIGHTNESS]	= 248;
	g_phoart_cntx.adjust_image_max_value[PHOART_ADJUST_IMAGE_CONTRAST]	= 248;
	g_phoart_cntx.adjust_image_max_value[PHOART_ADJUST_IMAGE_SATURATION]	= 248;
	g_phoart_cntx.adjust_image_max_value[PHOART_ADJUST_IMAGE_HUE]			= 360;
	
	g_phoart_cntx.adjust_image_step_value[PHOART_ADJUST_IMAGE_BRIGHTNESS]	= 20;
	g_phoart_cntx.adjust_image_step_value[PHOART_ADJUST_IMAGE_CONTRAST]		= 20;
	g_phoart_cntx.adjust_image_step_value[PHOART_ADJUST_IMAGE_SATURATION]	= 20;
	g_phoart_cntx.adjust_image_step_value[PHOART_ADJUST_IMAGE_HUE]				= 30;

	g_phoart_cntx.adjust_color_max_value[PHOART_ADJUST_COLOR_R]	= 8;
	g_phoart_cntx.adjust_color_max_value[PHOART_ADJUST_COLOR_G]	= 8; 
	g_phoart_cntx.adjust_color_max_value[PHOART_ADJUST_COLOR_B]	= 8;

	g_phoart_cntx.adjust_color_min_value[PHOART_ADJUST_COLOR_R]	= 0;
	g_phoart_cntx.adjust_color_min_value[PHOART_ADJUST_COLOR_G]	= 0; 
	g_phoart_cntx.adjust_color_min_value[PHOART_ADJUST_COLOR_B]	= 0;	

	g_phoart_cntx.adjust_color_value[PHOART_ADJUST_COLOR_R]	= 4;
	g_phoart_cntx.adjust_color_value[PHOART_ADJUST_COLOR_G]	= 4;
	g_phoart_cntx.adjust_color_value[PHOART_ADJUST_COLOR_B]	= 4;
	
	g_phoart_cntx.adjust_color_step_value[PHOART_ADJUST_COLOR_R]	= 1;
	g_phoart_cntx.adjust_color_step_value[PHOART_ADJUST_COLOR_G]	= 1;
	g_phoart_cntx.adjust_color_step_value[PHOART_ADJUST_COLOR_B]	= 1;
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_draw_image_list()
* DESCRIPTION
*   draw slider
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_draw_image_list(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 i;
	S32 offset_x;
	S32 offset_y;
	S32 image_width;
	S32 image_height;
	S32 bar_width;
	S32 bar_height;
	S32 bar_shift;
	S32 slider_width;
	S32 slider_height;
	S32 slider_shift;
	S32 spacing;
	S32 step_count;
	S32 bar_step;


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	gdi_layer_push_clip();
	gdi_layer_reset_clip();

	spacing 	= 5;
	offset_y = MMI_title_height;

	gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_IMAGE_START+1), &image_width, &image_height);
	gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_BAR), &bar_width, &bar_height);
	gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_SLIDER), &slider_width, &slider_height);	

	for(i = 0 ; i < PHOART_ADJUST_IMAGE_TYPE_COUNT; i++)
	{
		offset_x = spacing; 
		
		if(i == g_phoart_cntx.adjust_image_index)
		{
			/* is highlighted */

			/* draw hiligh icon */
			gdi_image_draw_id(offset_x, offset_y, (U16)(IMG_ID_PHOART_ADJUST_IMAGE_HILIGHT_START+i+1));

			/* draw bar */
			offset_x 		+= image_width+spacing+(slider_width>>1);
			bar_shift 		= (image_height-bar_height)>>1;
			slider_shift 	= (image_height-slider_height)>>1;		

			gdi_image_draw_id(offset_x, offset_y+bar_shift, IMG_ID_PHOART_ADJUST_BAR);

			#ifdef __MMI_TOUCH_SCREEN__
				g_phoart_touch_screen_cntx.bar_x = offset_x;
				g_phoart_touch_screen_cntx.bar_y = offset_y+bar_shift;
				g_phoart_touch_screen_cntx.bar_height = bar_height;
				g_phoart_touch_screen_cntx.bar_width = bar_width;
			#endif

			/* draw slider */
			step_count = g_phoart_cntx.adjust_image_max_value[i]-g_phoart_cntx.adjust_image_min_value[i];
			step_count /= g_phoart_cntx.adjust_image_step_value[i];

			bar_step = bar_width*100/step_count;

			offset_x += bar_step*(g_phoart_cntx.adjust_image_value[i]-g_phoart_cntx.adjust_image_min_value[i])/g_phoart_cntx.adjust_image_step_value[i]/100;
			offset_x -= (slider_width>>1);

			gdi_image_draw_id(offset_x, offset_y+slider_shift, IMG_ID_PHOART_ADJUST_SLIDER);

			#ifdef __MMI_TOUCH_SCREEN__
				g_phoart_touch_screen_cntx.init_x = offset_x;
				g_phoart_touch_screen_cntx.init_y = offset_y+slider_shift;
				g_phoart_touch_screen_cntx.slider_height = slider_height;
				g_phoart_touch_screen_cntx.slider_width = slider_width;
			#endif /* __MMI_TOUCH_SCREEN__ */

		}
		else
		{
			/* not highlighted */
			gdi_image_draw_id(offset_x, offset_y, (U16)(IMG_ID_PHOART_ADJUST_IMAGE_START+i+1));
		}

		offset_y += spacing+image_height;
	}

	
	gdi_layer_pop_clip();

}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_image_list_up()
* DESCRIPTION
*   move selection up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_image_list_up(void)
{
	if(g_phoart_cntx.adjust_image_index == 0)
	{
		g_phoart_cntx.adjust_image_index = PHOART_ADJUST_IMAGE_TYPE_COUNT-1;
	}
	else
	{
		g_phoart_cntx.adjust_image_index--;
	}

	mmi_phoart_adjust_edit_image_draw_osd();

	gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_image_list_down()
* DESCRIPTION
*   move selection down
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_image_list_down(void)
{
	if(g_phoart_cntx.adjust_image_index == PHOART_ADJUST_IMAGE_TYPE_COUNT-1)
	{
		g_phoart_cntx.adjust_image_index = 0;
	}
	else
	{
		g_phoart_cntx.adjust_image_index++; 
	}

	mmi_phoart_adjust_edit_image_draw_osd();

	gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);


	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_image_draw_osd
* DESCRIPTION
*   draw image adjust osd 
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_image_draw_osd(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_clip();
	gdi_layer_reset_clip();

	/* clear background */
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);

	mmi_phoart_draw_image_border();	
	
	mmi_phoart_adjust_edit_draw_image_list();
	
	mmi_phoart_draw_softkey((PS8)get_string(STR_GLOBAL_DONE), 
									(PS8)get_string(STR_GLOBAL_CANCEL),
									FALSE);				
	
	mmi_phoart_draw_title(	(PS8)get_string((U16)(STR_ID_PHOART_ADJUST_IMAGE_TITLE_START+g_phoart_cntx.adjust_image_index+1)),
									NULL,
									FALSE);


	gdi_layer_pop_clip();
	gdi_layer_pop_and_restore_active();	
}





/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_draw_color_list()
* DESCRIPTION
*   draw slider
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_draw_color_list(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 i;
	S32 offset_x;
	S32 offset_y;
	S32 image_width;
	S32 image_height;
	S32 bar_width;
	S32 bar_height;
	S32 bar_shift;
	S32 slider_width;
	S32 slider_height;
	S32 slider_shift;
	S32 spacing;
	S32 step_count;
	S32 bar_step;


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	gdi_layer_push_clip();
	gdi_layer_reset_clip();

	spacing 	= 5;
	offset_y = MMI_title_height;

	gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_COLOR_START+1), &image_width, &image_height);
	gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_BAR), &bar_width, &bar_height);
	gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_SLIDER), &slider_width, &slider_height);	

	for(i = 0 ; i < PHOART_ADJUST_COLOR_TYPE_COUNT; i++)
	{
		offset_x = spacing; 
		
		if(i == g_phoart_cntx.adjust_color_index)
		{
			/* is highlighted */

			/* draw hiligh icon */
			gdi_image_draw_id(offset_x, offset_y, (U16)(IMG_ID_PHOART_ADJUST_COLOR_HILIGHT_START+i+1));

			/* draw bar */
			offset_x 		+= image_width+spacing+(slider_width>>1);
			bar_shift 		= (image_height-bar_height)>>1;
			slider_shift 	= (image_height-slider_height)>>1;		

			gdi_image_draw_id(offset_x, offset_y+bar_shift, IMG_ID_PHOART_ADJUST_BAR);

			#ifdef __MMI_TOUCH_SCREEN__
				g_phoart_touch_screen_cntx.bar_x = offset_x;
				g_phoart_touch_screen_cntx.bar_y = offset_y+bar_shift;
				g_phoart_touch_screen_cntx.bar_height = bar_height;
				g_phoart_touch_screen_cntx.bar_width = bar_width;
			#endif

			/* draw slider */
			step_count = g_phoart_cntx.adjust_color_max_value[i]-g_phoart_cntx.adjust_color_min_value[i];
			step_count /= g_phoart_cntx.adjust_color_step_value[i];

			bar_step = bar_width*100/step_count;

			offset_x += bar_step*(g_phoart_cntx.adjust_color_value[i]-g_phoart_cntx.adjust_color_min_value[i])/g_phoart_cntx.adjust_color_step_value[i]/100;
			offset_x -= (slider_width>>1);

			gdi_image_draw_id(offset_x, offset_y+slider_shift, IMG_ID_PHOART_ADJUST_SLIDER);

			#ifdef __MMI_TOUCH_SCREEN__
				g_phoart_touch_screen_cntx.init_x = offset_x;
				g_phoart_touch_screen_cntx.init_y = offset_y+slider_shift;
				g_phoart_touch_screen_cntx.slider_height = slider_height;
				g_phoart_touch_screen_cntx.slider_width = slider_width;
			#endif /* __MMI_TOUCH_SCREEN__ */
	
		}
		else
		{
			/* not highlighted */
			gdi_image_draw_id(offset_x, offset_y, (U16)(IMG_ID_PHOART_ADJUST_COLOR_START+i+1));
		}

		offset_y += spacing+image_height;
	}


	gdi_layer_pop_clip();

}




/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_color_list_up()
* DESCRIPTION
*   move selection up
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_color_list_up(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_phoart_cntx.adjust_color_index == 0)
	{
		g_phoart_cntx.adjust_color_index = PHOART_ADJUST_COLOR_TYPE_COUNT-1;
	}
	else
	{
		g_phoart_cntx.adjust_color_index--;
	}

	mmi_phoart_adjust_edit_color_draw_osd();

	gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_color_list_down()
* DESCRIPTION
*   move selection down
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_color_list_down(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	if(g_phoart_cntx.adjust_color_index == PHOART_ADJUST_COLOR_TYPE_COUNT-1)
	{
		g_phoart_cntx.adjust_color_index = 0;
	}
	else
	{
		g_phoart_cntx.adjust_color_index++; 
	}

	mmi_phoart_adjust_edit_color_draw_osd();

	gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);


	
}






/*****************************************************************************
* FUNCTION
*	 mmi_phoart_adjust_edit_color_draw_osd
* DESCRIPTION
*   draw color adjust osd 
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_adjust_edit_color_draw_osd(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_clip();
	gdi_layer_reset_clip();

	/* clear background */
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);

	mmi_phoart_draw_image_border();	
	
	mmi_phoart_adjust_edit_draw_color_list();

	mmi_phoart_draw_softkey((PS8)get_string(STR_GLOBAL_DONE), 
									(PS8)get_string(STR_GLOBAL_CANCEL),
									FALSE);				
	
	mmi_phoart_draw_title(	(PS8)get_string((U16)(STR_ID_PHOART_ADJUST_COLOR_TITLE_START+g_phoart_cntx.adjust_color_index+1)),
									NULL,
									FALSE);

	gdi_layer_pop_clip();
	gdi_layer_pop_and_restore_active();	
}




/***************************************************************************** 
*
* Flip
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_flip()
* DESCRIPTION
*	 hilight "FLIP"
* PARAMETERS
*	 void
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_phoart_hilight_flip(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/ 	
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(mmi_phoart_entry_flip_edit_screen, KEY_EVENT_UP);
}
	


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_flip_edit_screen()
* DESCRIPTION
*   entry flip screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_flip_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
   PU8	buf_ptr;
	U8		*gui_buffer;
	
	/*----------------------------------------------------------------*/		
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	/* reset flag */
	g_phoart_cntx.is_user_goback = FALSE;
   
	EntryNewScreen(SCR_ID_PHOART_FLIP_EDIT, mmi_phoart_exit_flip_edit_screen, mmi_phoart_entry_flip_edit_screen, NULL);
	gui_buffer = GetCurrGuiBuffer(SCR_ID_PHOART_FLIP_EDIT);

	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	/* entry full screen */
	entry_full_screen();

	SetKeyHandler(mmi_phoart_flip_edit_vertical_flip, KEY_UP_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_flip_edit_vertical_flip, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_flip_edit_horizontal_flip, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_flip_edit_horizontal_flip, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetKeyHandler(mmi_phoart_flip_edit_vertical_flip, KEY_2, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_flip_edit_vertical_flip, KEY_8, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_flip_edit_horizontal_flip, KEY_6, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phoart_flip_edit_horizontal_flip, KEY_4, KEY_EVENT_DOWN);

	/* register key hdlr */
	SetKeyHandler(mmi_phoart_lsk_press,	 	KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_lsk_release, 	KEY_LSK, KEY_EVENT_UP);	

	SetKeyHandler(mmi_phoart_rsk_press, 	KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_phoart_rsk_release, 	KEY_RSK, KEY_EVENT_UP);	

	
	/***************************************************************************** 
	* Layers
	*****************************************************************************/
	gdi_layer_multi_layer_enable();
	
	/* base layer */
	gdi_layer_get_active(&g_phoart_cntx.base_layer_handle);

	/* osd layer */
	gdi_layer_create(0, 0, UI_device_width, UI_device_height, &g_phoart_cntx.osd_layer_handle);
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();

	
	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle,
									g_phoart_cntx.osd_layer_handle,  
									0, 0);

	gdi_layer_get_buffer_ptr(&buf_ptr);
	
	if(gui_buffer != NULL)
	{
		/* restore from interrupt */
		memcpy(buf_ptr, g_phoart_cntx.interrupt_backup_buf, PHOART_FRAME_BUFFER_SIZE);

		#ifdef APPMEM_SUPPORT 	
			/* free backup memory */
//			kal_app_mem_free((void*)g_phoart_cntx.interrupt_backup_buf);
			FreeMediaMemory((unsigned long)g_phoart_cntx.interrupt_backup_buf);
		#endif
		
		g_phoart_cntx.interrupt_backup_buf = NULL;

	}
	else
	{
		mmi_phoart_load_cache_image(buf_ptr);
	}

	/* draw osd */
	mmi_phoart_flip_edit_draw_osd();

	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

	#ifdef __MMI_TOUCH_SCREEN__
		mmi_wgui_register_pen_down_handler(mmi_phoart_pen_down_hdlr);
		mmi_wgui_register_pen_up_handler(mmi_phoart_pen_up_hdlr);
		mmi_wgui_register_pen_move_handler(mmi_phoart_pen_move_hdlr);
	#endif

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_flip_edit_draw_arrow()
* DESCRIPTION
*   draw arrow
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
static void mmi_phoart_flip_edit_draw_arrow(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 	center_x;
	S32 	center_y;
	S32 	area_width;
	S32 	area_height;
	S32 	arrow_height;
	color_t content;
	color_t line;

	/*----------------------------------------------------------------*/		
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* this function is copied from wgui_category.c */
	gdi_layer_push_clip();
	gdi_layer_reset_clip();
	
	center_x			= 0;
	center_y			= 0;
	area_width		= 20;
	area_height 	= MMI_button_bar_height-2;
	arrow_height	= 5;
	content 			= g_phoart_cntx.text_color;
	line 				= g_phoart_cntx.text_border_color;
	center_x 		= UI_device_width>>1;
	center_y 		= UI_device_height-(MMI_button_bar_height>>1);
	
	/* up arrow */
	UI_draw_horizontal_line(center_x-3,center_x+3,center_y-((area_height>>1)-arrow_height),line);
	UI_putpixel(center_x-3,center_y-((area_height>>1)-arrow_height)-1,line);
	UI_putpixel(center_x+3,center_y-((area_height>>1)-arrow_height)-1,line);
	UI_putpixel(center_x-2,center_y-((area_height>>1)-arrow_height)-2,line);
	UI_putpixel(center_x+2,center_y-((area_height>>1)-arrow_height)-2,line);
	UI_putpixel(center_x-1,center_y-((area_height>>1)-arrow_height)-3,line);
	UI_putpixel(center_x+1,center_y-((area_height>>1)-arrow_height)-3,line);
	UI_putpixel(center_x,center_y-((area_height>>1)-arrow_height)-4,line);

	UI_putpixel(center_x-2,center_y-((area_height>>1)-arrow_height)-1,content);
	UI_putpixel(center_x-1,center_y-((area_height>>1)-arrow_height)-1,content);
	UI_putpixel(center_x,center_y-((area_height>>1)-arrow_height)-1,content);
	UI_putpixel(center_x+1,center_y-((area_height>>1)-arrow_height)-1,content);
	UI_putpixel(center_x+2,center_y-((area_height>>1)-arrow_height)-1,content);
	UI_putpixel(center_x-1,center_y-((area_height>>1)-arrow_height)-2,content);
	UI_putpixel(center_x,center_y-((area_height>>1)-arrow_height)-2,content);
	UI_putpixel(center_x+1,center_y-((area_height>>1)-arrow_height)-2,content);
	UI_putpixel(center_x,center_y-((area_height>>1)-arrow_height)-3,content);

	/* down arrow */
	UI_draw_horizontal_line(center_x-3,center_x+3,center_y+((area_height>>1)-arrow_height),line);
	UI_putpixel(center_x-3,center_y+((area_height>>1)-arrow_height)+1,line);
	UI_putpixel(center_x+3,center_y+((area_height>>1)-arrow_height)+1,line);
	UI_putpixel(center_x-2,center_y+((area_height>>1)-arrow_height)+2,line);
	UI_putpixel(center_x+2,center_y+((area_height>>1)-arrow_height)+2,line);
	UI_putpixel(center_x-1,center_y+((area_height>>1)-arrow_height)+3,line);
	UI_putpixel(center_x+1,center_y+((area_height>>1)-arrow_height)+3,line);
	UI_putpixel(center_x,center_y+((area_height>>1)-arrow_height)+4,line);

	UI_putpixel(center_x-2,center_y+((area_height>>1)-arrow_height)+1,content);
	UI_putpixel(center_x-1,center_y+((area_height>>1)-arrow_height)+1,content);
	UI_putpixel(center_x,center_y+((area_height>>1)-arrow_height)+1,content);
	UI_putpixel(center_x+1,center_y+((area_height>>1)-arrow_height)+1,content);
	UI_putpixel(center_x+2,center_y+((area_height>>1)-arrow_height)+1,content);
	UI_putpixel(center_x-1,center_y+((area_height>>1)-arrow_height)+2,content);
	UI_putpixel(center_x,center_y+((area_height>>1)-arrow_height)+2,content);
	UI_putpixel(center_x+1,center_y+((area_height>>1)-arrow_height)+2,content);
	UI_putpixel(center_x,center_y+((area_height>>1)-arrow_height)+3,content);

	/* left arrow */
	UI_draw_vertical_line(center_y-3,center_y+3,center_x-((area_width>>1)-arrow_height),line);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-1,center_y-3,line);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-1,center_y+3,line);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-2,center_y-2,line);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-2,center_y+2,line);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-3,center_y-1,line);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-3,center_y+1,line);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-4,center_y,line);

	UI_putpixel(center_x-((area_width>>1)-arrow_height)-1,center_y-2,content);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-1,center_y-1,content);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-1,center_y,content);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-1,center_y+1,content);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-1,center_y+2,content);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-2,center_y-1,content);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-2,center_y,content);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-2,center_y+1,content);
	UI_putpixel(center_x-((area_width>>1)-arrow_height)-3,center_y,content);


	/* right arrow */
	UI_draw_vertical_line(center_y-3,center_y+3,center_x+((area_width>>1)-arrow_height),line);

	UI_putpixel(center_x+((area_width>>1)-arrow_height)+1,center_y-3,line);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+1,center_y+3,line);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+2,center_y-2,line);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+2,center_y+2,line);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+3,center_y-1,line);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+3,center_y+1,line);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+4,center_y,line);

	UI_putpixel(center_x+((area_width>>1)-arrow_height)+1,center_y-2,content);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+1,center_y-1,content);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+1,center_y,content);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+1,center_y+1,content);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+1,center_y+2,content);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+2,center_y-1,content);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+2,center_y,content);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+2,center_y+1,content);
	UI_putpixel(center_x+((area_width>>1)-arrow_height)+3,center_y,content);

	gdi_layer_pop_clip();
}
#endif /* __MMI_TOUCH_SCREEN__ */


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_exit_flip_edit_screen()
* DESCRIPTION
*   entry frame selection screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_exit_flip_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	PU8	buf_ptr;
	
   #ifndef APPMEM_SUPPORT 	
		PU8	back_buf_ptr;
   #endif /* !APPMEM_SUPPORT */

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(g_phoart_cntx.is_user_goback == FALSE)
	{
		/*
			if not user exit screen. it might be 
			(1) interrupt.
			(2) user pressed idle key 
		*/
		
		if(IDLE_SCREEN_ID != GetActiveScreenId())
		{	
			/* set del screen handler */
			SetDelScrnIDCallbackHandler(SCR_ID_PHOART_TEXT_EDIT, mmi_phoart_flip_del_screen_id_hdlr);
		
			/* if it is exit because of interrupt */
			gdi_layer_get_buffer_ptr(&buf_ptr);

			#ifdef APPMEM_SUPPORT 			
//				g_phoart_cntx.interrupt_backup_buf = kal_app_mem_alloc(PHOART_FRAME_BUFFER_SIZE, KAL_APPMEM_APP, MOD_MMI); 
				g_phoart_cntx.interrupt_backup_buf = (PU8)(AllocMediaMemory(PHOART_FRAME_BUFFER_SIZE)); 
			#else
				back_buf_ptr = (PU8)camera_osd_layer_buffer;
				g_phoart_cntx.interrupt_backup_buf = &back_buf_ptr[PHOART_FRAME_BUFFER_SIZE];
			#endif /* APPMEM_SUPPORT */		

			memcpy(g_phoart_cntx.interrupt_backup_buf, buf_ptr, PHOART_FRAME_BUFFER_SIZE);
		}
	}
   
   /* free layer */
	gdi_layer_flatten_previous_to_base();

	gdi_layer_free(g_phoart_cntx.osd_layer_handle);

  	gdi_layer_set_blt_layer(g_phoart_cntx.base_layer_handle, 0, 0, 0);
  	gdi_layer_multi_layer_disable();	
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_flip_del_screen_id_hdlr()
* DESCRIPTION
*   callback function when someone delete this screen's id
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static U8 mmi_phoart_flip_del_screen_id_hdlr(void *ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		

	/* free memory */	
	if(g_phoart_cntx.interrupt_backup_buf != NULL)
	{
		#ifdef APPMEM_SUPPORT
//			kal_app_mem_free((void*)g_phoart_cntx.interrupt_backup_buf);
			FreeMediaMemory((unsigned long)g_phoart_cntx.interrupt_backup_buf);
		#endif 
		
		g_phoart_cntx.interrupt_backup_buf = NULL;
	}

	return FALSE;
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_flip_edit_vertical_flip()
* DESCRIPTION
*   flip image vertically
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_flip_edit_vertical_flip(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
   U16 	*src_coolsand_ptr;
   U16 	*dest_coolsand_ptr;
	U8	*buf_ptr;
	U16   temp_color;
	S32	swap_count;
	S32	image_width;
	S32	image_height;
	S32	image_offset_x;
	S32	image_offset_y;
	S32	count;
	
	/*----------------------------------------------------------------*/		
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	image_width 	= g_phoart_cntx.resized_image_width;
	image_height 	= g_phoart_cntx.resized_image_height;
	image_offset_x = g_phoart_cntx.resized_image_offset_x;
	image_offset_y = g_phoart_cntx.resized_image_offset_y;	

	/* get src buffer frame ptr */
	gdi_layer_get_buffer_ptr((PU8*)(&buf_ptr));

	src_coolsand_ptr 	= (U16*)buf_ptr+(image_offset_y*UI_device_width+image_offset_x);
	dest_coolsand_ptr 	= (U16*)buf_ptr+( ((image_offset_y+image_height-1)*UI_device_width)+(image_offset_x));

	swap_count = (image_width*image_height)>>1;

	count = 0;
	
	while(1)
	{
		temp_color		 	= *src_coolsand_ptr;
		*src_coolsand_ptr 	= *dest_coolsand_ptr;
		*dest_coolsand_ptr 	= temp_color;

		count++;
		src_coolsand_ptr++;
		dest_coolsand_ptr++;
		
		if(count%image_width == 0)
		{
			/* jump pitch */
			src_coolsand_ptr 	+= (UI_device_width-image_width);
			dest_coolsand_ptr 	-= (UI_device_width+image_width);
		}

		/* terminate condition */
		if(count >= swap_count)
			break;
		
	}	

	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_flip_edit_horizontal_flip()
* DESCRIPTION
*   flip image horizontal
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
GDI_RESULT gdi_layer_get_buffer_ptr(U8 **buf_ptr);
static void mmi_phoart_flip_edit_horizontal_flip(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	U16	*src_coolsand_ptr;
	U16	*dest_coolsand_ptr;
	U8	*buf_ptr = NULL;
	U16	temp_color;
	S32	swap_count;
	S32	image_width;
	S32	image_height;
	S32	image_offset_x;
	S32	image_offset_y;
	S32	count;
	S32	i;

	/*----------------------------------------------------------------*/	 
	/* Code Body																		*/
	/*----------------------------------------------------------------*/ 
	image_width	 	= g_phoart_cntx.resized_image_width;
	image_height	= g_phoart_cntx.resized_image_height;
	image_offset_x = g_phoart_cntx.resized_image_offset_x;
	image_offset_y = g_phoart_cntx.resized_image_offset_y;	 

	/* get src buffer frame ptr */
	gdi_layer_get_buffer_ptr((U8 **)&buf_ptr);

	src_coolsand_ptr	 = (U16 *)buf_ptr+(image_offset_y*UI_device_width+image_offset_x);
	dest_coolsand_ptr = src_coolsand_ptr+image_width-1;

	swap_count = image_width>>1;


	for(i = 0; i < image_height; i++)
	{
		count = 0;
		while(1)
		{
			temp_color 		 	= *src_coolsand_ptr;
			*src_coolsand_ptr	= *dest_coolsand_ptr;
			*dest_coolsand_ptr  = temp_color;		
			
			count++;
			src_coolsand_ptr++;
			dest_coolsand_ptr--;

			if(count >= swap_count)
				break;
		}
		
		src_coolsand_ptr 	+= UI_device_width-swap_count;
		dest_coolsand_ptr 	+= UI_device_width+swap_count;
		
	}

	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

}

	
/*****************************************************************************
* FUNCTION
*	 mmi_phoart_flip_edit_draw_osd()
* DESCRIPTION
*   draw flip screen osd layer
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_flip_edit_draw_osd(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	gdi_layer_push_and_set_active(g_phoart_cntx.osd_layer_handle);
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	
	/* draw image broder */
	mmi_phoart_draw_image_border();	

	/* draw softkey & title  */
	mmi_phoart_draw_softkey((PS8)get_string(STR_GLOBAL_DONE), 
									(PS8)get_string(STR_GLOBAL_CANCEL),
									FALSE);				

	mmi_phoart_draw_title(	(PS8)get_string(STR_ID_PHOART_FLIP_TITLE),
									NULL,
									FALSE);
									
	#ifdef __MMI_TOUCH_SCREEN__
		mmi_phoart_flip_edit_draw_arrow();
	#endif

 	gdi_layer_pop_and_restore_active();
}


/***************************************************************************** 
*
* Expand Canvas
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_phoart_expand_canvas_confirm()
* DESCRIPTION
*   entry expand canvas screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_expand_canvas_confirm(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if((g_phoart_cntx.resized_image_width == UI_device_width) && 
		(g_phoart_cntx.resized_image_height == UI_device_height))
	{
		DisplayPopup((PU8)GetString(STR_ID_PHOART_NOTIFY_ALREADY_FULL_SCREEN), IMG_GLOBAL_WARNING, 1, PHOEDT_NOTIFY_DURATION, WARNING_TONE);
		return;
	}
	   
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						 STR_GLOBAL_NO, IMG_GLOBAL_NO,
						 get_string(STR_ID_PHOART_NOTIFY_ASK_EXPAND_CANVAS), 
						 IMG_GLOBAL_QUESTION, 
						 WARNING_TONE); 

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_phoart_expand_canvas_confirm_yes, KEY_EVENT_UP);
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_expand_canvas_confirm_yes()
* DESCRIPTION
*   expand canvas confirm "yes" handler
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_expand_canvas_confirm_yes(void)
{
	g_phoart_cntx.resized_image_height 			= UI_device_height;
	g_phoart_cntx.resized_image_width 			= UI_device_width;
	g_phoart_cntx.resized_image_offset_x 		= 0;
	g_phoart_cntx.resized_image_offset_y 		= 0;	
	
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_INFO, 1, PHOEDT_NOTIFY_DURATION, SUCCESS_TONE);
	DeleteNHistory(1);
}


/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_expand_canvas()
* DESCRIPTION
*   hilight "expand canvas"
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_expand_canvas(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction(mmi_phoart_expand_canvas_confirm, KEY_EVENT_UP);
}





/***************************************************************************** 
*
* Help
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_help_screen()
* DESCRIPTION
*   entry help screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_entry_help_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/	
	PS8 buffer;
	S32 buffer_size;

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	EntryNewScreen(SCR_ID_PHOART_HELP, NULL, mmi_phoart_entry_help_screen, NULL);

	buffer		= GetString(STR_ID_PHOART_HELP_DESC);
	buffer_size	= pfnUnicodeStrlen(buffer);

	ShowCategory74Screen( STR_ID_PHOART_HELP_TITLE, 
								 MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								 0, 0,
								 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								 (PU8)buffer, buffer_size, NULL);

	/* go back to game menu */
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_hilight_help()
* DESCRIPTION
*   hilight "help"
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoart_hilight_help(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetLeftSoftkeyFunction(mmi_phoart_entry_help_screen, KEY_EVENT_UP);
}



/***************************************************************************** 
*
* Touch Screen
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_phoart_pen_down_hdlr()
* DESCRIPTION
*   pen down hdlr
* PARAMETERS
*	 pos		IN		pen down postion
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
void mmi_phoart_pen_down_hdlr(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/	
	mmi_wgui_component_info_struct obj;

	U8 index;
	S32 image_width, image_height;
	S32 bar_width;
	S32 bar_height;
	S32 bar_x;
	S32 slider_width;
	S32 slider_height;
	S32 slider_x;
	S32 slider_y;
	S32 offset_y;
	S32 offset_x;
	S32 spacing;
	S32 rect_x1, rect_y1, rect_width, rect_height;
	
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	/* LSK */
	if(mmi_wgui_test_object_position(pos,g_phoart_touch_screen_cntx.lsk) == MMI_TRUE )	/* within LSK */
	{
		/* draw clicked LSK */
		mmi_phoart_lsk_press();
		g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_LSK;
		return;
	}
	
	/* RSK */
	if(mmi_wgui_test_object_position(pos,g_phoart_touch_screen_cntx.rsk) == MMI_TRUE)	/* within RSK */
	{
		/* draw clicked RSK */
		mmi_phoart_rsk_press();
		g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_RSK;
		return;
	}

	if(GetActiveScreenId()==SCR_ID_PHOART_ICON_EDIT) /* icon edit screen */
	{
		obj.pos_x 	= ICON_LAYER_X1;
		obj.pos_y	= ICON_LAYER_Y1;
		obj.width 	= ICON_LAYER_WIDTH;
		obj.height 	= ICON_LAYER_HEIGHT;

		g_phoart_cntx.pen_offset_x = pos.x;
		g_phoart_cntx.pen_offset_y = pos.y;		
		
		if ( mmi_wgui_test_object_position(pos, obj) == MMI_TRUE)
		{
			g_phoart_touch_screen_cntx.icon_edit_update_icon = MMI_FALSE;
			g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_EDIT_ICON;
			mmi_phoart_icon_edit_draw_icon(PHOART_BORDER_ALL);
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);			
			return;
		}
	}

	if(GetActiveScreenId()==SCR_ID_PHOART_TEXT_EDIT) /* icon edit screen */
	{
		obj.pos_x	= TEXT_LAYER_X1;
		obj.pos_y	= TEXT_LAYER_Y1;
		obj.width	= TEXT_LAYER_WIDTH;
		obj.height	= TEXT_LAYER_HEIGHT;
		
		g_phoart_cntx.pen_offset_x = pos.x;
		g_phoart_cntx.pen_offset_y = pos.y;		
		
		if(mmi_wgui_test_object_position(pos, obj) == MMI_TRUE)
		{
			g_phoart_touch_screen_cntx.text_edit_update_text = MMI_FALSE;
			g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_EDIT_TEXT;
			mmi_phoart_text_edit_draw_text(PHOART_BORDER_ALL);
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);			
			return;
		}
	}

	if(GetActiveScreenId()==SCR_ID_PHOART_EFFECT_EDIT) /* effect edit screen */
	{
		if ( mmi_wgui_test_object_position(pos, g_phoart_touch_screen_cntx.prev_arrow) == MMI_TRUE )
		{
			mmi_phoart_effect_edit_change_backward();
			g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_EDIT_EFFECT_PREV;
			return;
		}
		if ( mmi_wgui_test_object_position(pos, g_phoart_touch_screen_cntx.next_arrow) == MMI_TRUE )
		{
			mmi_phoart_effect_edit_change_forward();
			g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_EDIT_EFFECT_NEXT;
			return;
		}
	}

	if(GetActiveScreenId()==SCR_ID_PHOART_FRAME_EDIT) /* frame edit screen */
	{
		if ( mmi_wgui_test_object_position(pos, g_phoart_touch_screen_cntx.prev_arrow) == MMI_TRUE )
		{
			mmi_phoart_frame_edit_change_backward();
			g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_EDIT_FRAME_PREV;
			return;
		}
		if ( mmi_wgui_test_object_position(pos, g_phoart_touch_screen_cntx.next_arrow) == MMI_TRUE )
		{
			mmi_phoart_frame_edit_change_forward();
			g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_EDIT_FRAME_NEXT;
			return;
		}
	}

	/* add pen handler for clip */
	if ( GetActiveScreenId()==SCR_ID_PHOART_CLIP) /* clip screen */
	{
		rect_x1	= g_phoart_cntx.clip_rect_center_x-(g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100;
		rect_y1	= g_phoart_cntx.clip_rect_center_y-(g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100;
		rect_width	= (g_phoart_cntx.clip_rect_width)*g_phoart_cntx.scale_ratio/100;
		rect_height	= (g_phoart_cntx.clip_rect_height)*g_phoart_cntx.scale_ratio/100;
		
		if ( PEN_CHECK_BOUND(pos.x, pos.y, rect_x1, rect_y1, rect_width, rect_height) )/* pen position inside rect */
		{
			g_phoart_touch_screen_cntx.init_x = pos.x;
			g_phoart_touch_screen_cntx.init_y = pos.y;
		}
		g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_CLIP_RECT;
		return;
	}

	if ( GetActiveScreenId()==SCR_ID_PHOART_ADJUST_EDIT) /* Adjust screen */
	{
		bar_width		= g_phoart_touch_screen_cntx.bar_width;
		bar_height		= g_phoart_touch_screen_cntx.bar_height;
		bar_x				= g_phoart_touch_screen_cntx.bar_x;
		slider_width	= g_phoart_touch_screen_cntx.slider_width;
		slider_height	= g_phoart_touch_screen_cntx.slider_height;
		
		slider_x	= g_phoart_touch_screen_cntx.init_x;
		slider_y	= g_phoart_touch_screen_cntx.init_y;
		offset_y = MMI_title_height;
		offset_x	= 5;
		spacing	= 5;

		gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_BAR), &bar_width, &bar_height);
		gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_SLIDER), &slider_width, &slider_height);	

		if(g_phoart_cntx.adjust_type == PHOART_ADJUST_IMAGE_TYPE)
		{
			U8 i=0;

			index = g_phoart_cntx.adjust_image_index;

			for (i=0; i<=PHOART_ADJUST_IMAGE_TYPE_COUNT; i++)
			{
				gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_IMAGE_START+1+i), &image_width, &image_height);

				/* pen down on icon */
				if ( PEN_CHECK_BOUND(pos.x, pos.y, offset_x, offset_y, image_width, image_height) )
				{
					g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_ADJUST_ICON;

					if ( i==index )
						return;

					g_phoart_cntx.adjust_image_index = i;

					mmi_phoart_adjust_edit_image_draw_osd();

					gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);


					return;
				}
				offset_y += image_height+spacing;
			}

				if ( PEN_CHECK_BOUND(pos.x, pos.y, bar_x-(slider_width>>1), slider_y, bar_width, slider_height) )
				{

					if ( PEN_CHECK_BOUND(pos.x, pos.y, slider_x, slider_y, slider_width, slider_height ) )
					{
						g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_ADJUST_SLIDER;
						return;
					}
					else
					{
						g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_ADJUST_BAR;

						if ( pos.x<slider_x )
							g_phoart_cntx.adjust_image_value[index]-=g_phoart_cntx.adjust_image_step_value[index];

						else if ( pos.x>slider_x+slider_width-1 )
							g_phoart_cntx.adjust_image_value[index]+=g_phoart_cntx.adjust_image_step_value[index];

						else
							return;

						mmi_phoart_adjust_edit_image_draw_osd();

						/* add effect to image */
						mmi_phoart_adjust_edit_add_adjustment();
						
						gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

						return;
					}
				}

//			}
		}

		else if(g_phoart_cntx.adjust_type == PHOART_ADJUST_COLOR_TYPE)
		{
			U8 i=0;

			index = g_phoart_cntx.adjust_color_index;

			for (i=0; i<=PHOART_ADJUST_COLOR_TYPE_COUNT; i++)
			{
				gdi_image_get_dimension_id((U16)(IMG_ID_PHOART_ADJUST_COLOR_START+1+i), &image_width, &image_height);

				/* pen down on icon */
				if ( PEN_CHECK_BOUND(pos.x, pos.y, offset_x, offset_y, image_width, image_height) )
				{
					g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_ADJUST_ICON;

					if ( i==index )
						return;

					g_phoart_cntx.adjust_color_index = i;

					mmi_phoart_adjust_edit_color_draw_osd();

					gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

					return;
				}
				offset_y += image_height+spacing;
			} 
				if ( PEN_CHECK_BOUND(pos.x, pos.y, bar_x-(slider_width>>1), slider_y, bar_width, slider_height) )
				{

					if ( PEN_CHECK_BOUND(pos.x, pos.y, slider_x, slider_y, slider_width, slider_height ) )
					{
						g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_ADJUST_SLIDER;
						return;
					}
					else
					{
						g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_ADJUST_BAR;

						if ( pos.x<slider_x )
							g_phoart_cntx.adjust_color_value[index]-=g_phoart_cntx.adjust_color_step_value[index];

						else if ( pos.x>slider_x+slider_width-1 )
							g_phoart_cntx.adjust_color_value[index]+=g_phoart_cntx.adjust_color_step_value[index];

						else
							return;

						mmi_phoart_adjust_edit_color_draw_osd();

						/* add effect to image */
						mmi_phoart_adjust_edit_add_adjustment();
						
						gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

						return;
					}
				}
			//}		
		}
	}

	/* no event */
	g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_NONE;
}
#endif /* __MMI_TOUCH_SCREEN__ */



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_pen_up_hdlr()
* DESCRIPTION
*   pen down hdlr
* PARAMETERS
*	 pos		IN		pen up postion
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
void mmi_phoart_pen_up_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	
	switch(g_phoart_touch_screen_cntx.pen_event_on_object)
	{
		case	PHOART_LSK:
			/* draw released LSK */
			if( mmi_wgui_test_object_position(pos,g_phoart_touch_screen_cntx.lsk) == MMI_TRUE)	/* outside LSK */
				mmi_phoart_lsk_release();
			break;

		case	PHOART_RSK:
			/* draw released RSK */
			if( mmi_wgui_test_object_position(pos,g_phoart_touch_screen_cntx.rsk) == MMI_TRUE) /* outside LSK */
				mmi_phoart_rsk_release();
			break;
			
		case PHOART_EDIT_ICON:
			mmi_phoart_icon_edit_draw_icon(PHOART_BORDER_CORNER);
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
			break;

		case PHOART_EDIT_TEXT:
			mmi_phoart_text_edit_draw_text(PHOART_BORDER_CORNER);
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
			break;

		default:
			break;
	}
	g_phoart_touch_screen_cntx.pen_event_on_object = PHOART_NONE;
}
#endif /* __MMI_TOUCH_SCREEN__ */



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_pen_move_hdlr()
* DESCRIPTION
*   pen down hdlr
* PARAMETERS
*	 pos		IN		pen move postion
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
void mmi_phoart_pen_move_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32 softkey_pressed = 0;
	S32 shift_x;
	S32 shift_y;	
	S32 image_x1;
	S32 image_x2;
	S32 image_y1;
	S32 image_y2;	
	S32 icon_x1;
	S32 icon_x2;	
	S32 icon_y1;
	S32 icon_y2;	
	S32 text_x1;
	S32 text_x2;	
	S32 text_y1;
	S32 text_y2;	
	S32 bar_width;
	S32 bar_x;
	S32 slider_width;
	S32 slider_x;
	S32 x1, x2, y1, y2;
	S32 display_x1;
	S32 display_x2;
	S32 display_y1;
	S32 display_y2;
	S32 curr_clip_rect_center_x;
	S32 curr_clip_rect_center_y;
	
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	switch(g_phoart_touch_screen_cntx.pen_event_on_object)
	{
		case PHOART_LSK:
			if(mmi_wgui_test_object_position(pos,g_phoart_touch_screen_cntx.lsk) == MMI_TRUE) /* within LSK */
				softkey_pressed=1;
			else
				softkey_pressed=0;
			if ( softkey_pressed != g_phoart_cntx.is_lsk_pressed )
			{
				g_phoart_cntx.is_lsk_pressed=softkey_pressed;
				mmi_phoart_show_press_release_softkey();
			}
			break;
			
		case PHOART_RSK:
			if(mmi_wgui_test_object_position(pos,g_phoart_touch_screen_cntx.rsk) == MMI_TRUE) /* within RSK */
				softkey_pressed=1;
			else
				softkey_pressed=0;
			if(softkey_pressed != g_phoart_cntx.is_rsk_pressed)
			{
				g_phoart_cntx.is_rsk_pressed=softkey_pressed;
				mmi_phoart_show_press_release_softkey();
			}
			break;
			
		case PHOART_EDIT_ICON:
			image_x1 = g_phoart_cntx.resized_image_offset_x;
			image_y1 = g_phoart_cntx.resized_image_offset_y;	
			image_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1;
			image_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1;	

			icon_x1 = ICON_LAYER_X1;
			icon_y1 = ICON_LAYER_Y1;
			icon_x2 = ICON_LAYER_X2;
			icon_y2 = ICON_LAYER_Y2;
			
			shift_x = pos.x-g_phoart_cntx.pen_offset_x;
			shift_y = pos.y-g_phoart_cntx.pen_offset_y;			

			if(shift_x > 0)
			{
				/* avoid out of image region */
				if((icon_x2+shift_x) > image_x2)
				{
					shift_x = image_x2-icon_x2;
				}
			}
			else if(shift_x < 0)
			{
				if((icon_x1+shift_x) < image_x1)
				{
					shift_x = image_x1-icon_x1;
				}	
			}

			if(shift_y > 0)
			{
				/* avoid out of image region */
				if((icon_y2+shift_y) > image_y2)
				{
					shift_y = image_y2-icon_y2;
				}
			}
			else if(shift_y < 0)
			{
				if((icon_y1+shift_y) < image_y1)
				{
					shift_y = image_y1-icon_y1;
				}	
			}
	 			
			g_phoart_cntx.icon_offset_x +=shift_x;
			g_phoart_cntx.icon_offset_y +=shift_y;

			g_phoart_cntx.pen_offset_x = pos.x;
			g_phoart_cntx.pen_offset_y = pos.y;	

		   /* GDI cant flatten layer with neg postion, so force position at positive coord */
			if((g_phoart_cntx.icon_offset_x-PHOART_ICON_BBOX_SHIFT) < 0)
			{
				g_phoart_cntx.icon_offset_x = PHOART_ICON_BBOX_SHIFT;
			}

			if((g_phoart_cntx.icon_offset_y-PHOART_ICON_BBOX_SHIFT) < 0)
			{
				g_phoart_cntx.icon_offset_y = PHOART_ICON_BBOX_SHIFT;
			}

			gdi_layer_push_and_set_active(g_phoart_cntx.icon_layer_handle);
			gdi_layer_set_position(ICON_LAYER_X1, ICON_LAYER_Y1);
			gdi_layer_pop_and_restore_active();	

			mmi_phoart_icon_edit_draw_icon(PHOART_BORDER_ALL);
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
			
			break;
			
		case PHOART_EDIT_TEXT:

			image_x1 = g_phoart_cntx.resized_image_offset_x;
			image_y1 = g_phoart_cntx.resized_image_offset_y;	
			image_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1;
			image_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1;	

			text_x1 = TEXT_LAYER_X1;
			text_y1 = TEXT_LAYER_Y1;
			text_x2 = TEXT_LAYER_X2;
			text_y2 = TEXT_LAYER_Y2;
			
			shift_x = pos.x-g_phoart_cntx.pen_offset_x;
			shift_y = pos.y-g_phoart_cntx.pen_offset_y;			

			if(shift_x > 0)
			{
				/* avoid out of image region */
				if((text_x2+shift_x) > image_x2)
				{
					shift_x = image_x2-text_x2;
				}
			}
			else if(shift_x < 0)
			{
				if((text_x1+shift_x) < image_x1)
				{
					shift_x = image_x1-text_x1;
				}	
			}

			if(shift_y > 0)
			{
				/* avoid out of image region */
				if((text_y2+shift_y) > image_y2)
				{
					shift_y = image_y2-text_y2;
				}
			}
			else if(shift_y < 0)
			{
				if((text_y1+shift_y) < image_y1)
				{
					shift_y = image_y1-text_y1;
				}	
			}
	 			
			g_phoart_cntx.text_offset_x +=shift_x;
			g_phoart_cntx.text_offset_y +=shift_y;

			g_phoart_cntx.pen_offset_x = pos.x;
			g_phoart_cntx.pen_offset_y = pos.y;	

		   /* GDI cant flatten layer with neg postion, so force position at positive coord */
			if((g_phoart_cntx.text_offset_x-PHOART_TEXT_BBOX_SHIFT) < 0)
			{
				g_phoart_cntx.text_offset_x = PHOART_TEXT_BBOX_SHIFT;
			}

			if((g_phoart_cntx.text_offset_y-PHOART_TEXT_BBOX_SHIFT) < 0)
			{
				g_phoart_cntx.text_offset_y = PHOART_TEXT_BBOX_SHIFT;
			}

			gdi_layer_push_and_set_active(g_phoart_cntx.text_layer_handle);
			gdi_layer_set_position(TEXT_LAYER_X1, TEXT_LAYER_Y1);
			gdi_layer_pop_and_restore_active();	

			mmi_phoart_text_edit_draw_text(PHOART_BORDER_ALL);
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

			break;
		case PHOART_CLIP_RECT:
			{
				/* check if the clip rect is out of the screen */
				display_x1 = g_phoart_cntx.resized_image_offset_x;
				display_x2 = g_phoart_cntx.resized_image_offset_x+g_phoart_cntx.resized_image_width-1;
				display_y1 = g_phoart_cntx.resized_image_offset_y;
				display_y2 = g_phoart_cntx.resized_image_offset_y+g_phoart_cntx.resized_image_height-1;
				curr_clip_rect_center_x = g_phoart_cntx.clip_rect_center_x;
				curr_clip_rect_center_y = g_phoart_cntx.clip_rect_center_y;

				if ( pos.x<=display_x1 || pos.x>=display_x2 || pos.y<=display_y1 || pos.y>=display_y2)
					return;

				g_phoart_cntx.clip_rect_center_x += (pos.x-g_phoart_touch_screen_cntx.init_x);
				g_phoart_cntx.clip_rect_center_y += (pos.y-g_phoart_touch_screen_cntx.init_y);
				g_phoart_touch_screen_cntx.init_x = pos.x;
				g_phoart_touch_screen_cntx.init_y = pos.y;
				x1 = g_phoart_cntx.clip_rect_center_x-(g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100 ;
				x2 = g_phoart_cntx.clip_rect_center_x+(g_phoart_cntx.clip_rect_width>>1)*g_phoart_cntx.scale_ratio/100 ;
				y1 = g_phoart_cntx.clip_rect_center_y-(g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100 ;
				y2 = g_phoart_cntx.clip_rect_center_y+(g_phoart_cntx.clip_rect_height>>1)*g_phoart_cntx.scale_ratio/100 ;

				if ( x1<display_x1 || x2>display_x2 || y1<display_y1 || y2>display_y2)
				{
					g_phoart_cntx.clip_rect_center_x = curr_clip_rect_center_x;
					g_phoart_cntx.clip_rect_center_y = curr_clip_rect_center_y;
				}
				else /* move the clip rect */
				{
					mmi_phoart_clip_draw_clip_rect();
					gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);	
				}
				break;
			}
		case PHOART_ADJUST_BAR:
			//break;

		case PHOART_ADJUST_SLIDER:
			{
				bar_width		= g_phoart_touch_screen_cntx.bar_width;
				bar_x		 		= g_phoart_touch_screen_cntx.bar_x;
				slider_width	= g_phoart_touch_screen_cntx.slider_width;
				slider_x			= g_phoart_touch_screen_cntx.init_x;

				if ( g_phoart_cntx.adjust_type == PHOART_ADJUST_IMAGE_TYPE )
				{
					U8 i=g_phoart_cntx.adjust_image_index;

					if ( pos.x<slider_x && pos.x >bar_x)
						g_phoart_cntx.adjust_image_value[i]-=g_phoart_cntx.adjust_image_step_value[i];

					else if ( pos.x>slider_x+slider_width-1 && pos.x < bar_x+bar_width-1)
						g_phoart_cntx.adjust_image_value[i]+=g_phoart_cntx.adjust_image_step_value[i];

					else 
						return;

					mmi_phoart_adjust_edit_image_draw_osd();

					/* add effect to image */
					mmi_phoart_adjust_edit_add_adjustment();
						
					gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

					return;

				}

				else if ( g_phoart_cntx.adjust_type == PHOART_ADJUST_COLOR_TYPE )
				{
					U8 i=g_phoart_cntx.adjust_color_index;

					if ( pos.x<slider_x && pos.x >bar_x)
						g_phoart_cntx.adjust_color_value[i]-=g_phoart_cntx.adjust_color_step_value[i];

					else if ( pos.x>slider_x+slider_width-1 && pos.x < bar_x+bar_width-1)
						g_phoart_cntx.adjust_color_value[i]+=g_phoart_cntx.adjust_color_step_value[i];

					else
						return;

					mmi_phoart_adjust_edit_color_draw_osd();

					/* add effect to image */
					mmi_phoart_adjust_edit_add_adjustment();
						
					gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

					return;
				}
			}
			break;
	}
}
#endif /* __MMI_TOUCH_SCREEN__ */

/*****************************************************************************
* FUNCTION
*	 mmi_phoart_pen_repeat_hdlr()
* DESCRIPTION
*   pen repeat hdlr
* PARAMETERS
*	 pos		IN		pen up postion
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
void mmi_phoart_pen_repeat_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32 bar_width;
	S32 bar_height;
	S32 bar_x;
	S32 bar_y;
	S32 slider_width;
	S32 slider_height;
	S32 slider_x;
	S32 slider_y;
	
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	
	switch (g_phoart_touch_screen_cntx.pen_event_on_object)
	{
		case	PHOART_ADJUST_BAR:
			{
				bar_width		= g_phoart_touch_screen_cntx.bar_width;
				bar_height		= g_phoart_touch_screen_cntx.bar_height;
				bar_x				= g_phoart_touch_screen_cntx.bar_x;
				bar_y				= g_phoart_touch_screen_cntx.bar_y;
				slider_width	= g_phoart_touch_screen_cntx.slider_width;
				slider_height	= g_phoart_touch_screen_cntx.slider_height;
				slider_x			= g_phoart_touch_screen_cntx.init_x;
				slider_y			= g_phoart_touch_screen_cntx.init_y;

				if ( g_phoart_cntx.adjust_type == PHOART_ADJUST_IMAGE_TYPE )
				{
					U8 i=g_phoart_cntx.adjust_image_index;

					if ( pos.x<slider_x && pos.x >bar_x)
						g_phoart_cntx.adjust_image_value[i]-=g_phoart_cntx.adjust_image_step_value[i];

					else if ( pos.x>slider_x+slider_width-1 && pos.x < bar_x+bar_width-1)
						g_phoart_cntx.adjust_image_value[i]+=g_phoart_cntx.adjust_image_step_value[i];

					else 
						return;

					mmi_phoart_adjust_edit_image_draw_osd();

					/* add effect to image */
					mmi_phoart_adjust_edit_add_adjustment();
						
					gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

					return;

				}

				else if ( g_phoart_cntx.adjust_type == PHOART_ADJUST_COLOR_TYPE )
				{
					U8 i=g_phoart_cntx.adjust_color_index;

					if ( pos.x<slider_x && pos.x >bar_x)
						g_phoart_cntx.adjust_color_value[i]-=g_phoart_cntx.adjust_color_step_value[i];

					else if ( pos.x>slider_x+slider_width-1 && pos.x < bar_x+bar_width-1)
						g_phoart_cntx.adjust_color_value[i]+=g_phoart_cntx.adjust_color_step_value[i];

					else
						return;

					mmi_phoart_adjust_edit_color_draw_osd();

					/* add effect to image */
					mmi_phoart_adjust_edit_add_adjustment();
						
					gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

					return;
				}
			}
			break;

		default:
			break;
	}
}

#endif /* __MMI_TOUCH_SCREEN__ */


#endif /* __PHOEDT_FEATURE_PHOART__ */
#endif /* __MMI_PHOTOEDITOR__ */


