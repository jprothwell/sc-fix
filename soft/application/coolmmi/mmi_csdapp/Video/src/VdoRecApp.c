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
 *   VdoRecApp.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Video Recorder Applcation.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MMI_VDORECAPP_C
#define _MMI_VDORECAPP_C

#include "mmi_features.h" 

#if defined(__MMI_VIDEO__)

#define __NEWSIMULATOR 

#include "stdc.h"
#include "l4dr1.h"
#include "globaldefs.h"

#include "mmi_features_video.h"	/* features */
  
/* framework related headers */
#include "custmenures.h"
#include "globalconstants.h"			/* key id define */
#include "globaldefs.h"					/* golbal image and string id. */
#include "gui.h"							/* gui functions */

#include "historygprot.h"				/* screen history */
#include "unicodexdcl.h"				/* unicode transform functions */
#include "nvramtype.h"					/* nvram data type */
#include "nvramprot.h"					/* nvram access fucntions */	
#include "nvramenum.h"					/* nvram id define */
#include "eventsgprot.h"				/* entery\exit functions */
#include "settingdefs.h"				/* st_notifyduration define */
#include "wgui.h"							/* for getdatetime() */
#include "gpioinc.h"						/* led */
#include "idleappprot.h"				/* for isidlearrived() */
#include "debuginitdef.h"				/* mmi_assert */	
#include "commonscreens.h"				/* popup */
#include "keybrd.h"						/* clear key function */
#include "protocolevents.h"			/* interrupt message id */
#include "sublcdhistorygprot.h"		/* sublcd */
#include "mainmenudef.h"				/* multi-media icon */
#include "callmanagementgprot.h"		/* getwapcallpresent */

#include "wgui_categories.h"			/* categorires */
#include "wgui_categories_popup.h"
#include "wgui_categories_multimedia.h"
#include "wgui_status_icons.h"		/* status ui */

#include "filemanagergprot.h"			/* file path / file error  */
#include "filemanagerdef.h"	  		/* file error text */
#include "filemgr.h"
#include "mmiapi_enum.h"				/* stop led */

/* gdi, mid, video rec headers */
#include "lcd_if.h"
#include "lcd_sw_rnd.h"
#include "gdi_include.h"				/* graphic library */
#include "mdi_datatype.h"
#include "mdi_video.h"					/* mdi video library */
#include "multimemmng.h"

#ifdef __MMI_PROJECT_T33__
#ifndef __VDOREC_FEATURE_TOUCH_SCREEN__
#define __VDOREC_FEATURE_TOUCH_SCREEN__
#endif
#endif


//for t800 only
#ifdef  __MMI_NOKIA_STYLE_N800__
#ifndef __VDOREC_FEATURE_TOUCH_SCREEN__
#define __VDOREC_FEATURE_TOUCH_SCREEN__
#endif
#endif
#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
	#include "wgui_touch_screen.h"
#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */

/* video recoder related */
#include "resource_video_skins.h"	/* ui custom config */

#include "vdorecscrenum.h"				/* screen id */
#include "vdorecstrenum.h"				/* string id */
#include "vdorecimgenum.h"				/* image id */
#include "vdorecapp.h"
#include "vdorecgprot.h"
#include "vdoplygprot.h"				/* entry function */
#include "vdoplyscrenum.h"

#ifdef __MMI_TVOUT__
	#include "mdi_tv.h"					/* mdi tvout */
	#include "wallpaperdefs.h"			/* some header for phonsetup.h */
	#include "downloaddefs.h"			/* some header for phonsetup.h */
	#include "phonesetup.h"				/* tvout display style */
#endif /* __MMI_TVOUT__ */

#include "mmi_trace.h"
#include "soundrecorderdef.h"
#include "vdoplyimgenum.h"
#include "vdoplystrenum.h"
#include "mmi_features_camera.h"
#include "cameraresdefs.h"
#include "cameraapp.h"
/***************************************************************************** 
* Define
*****************************************************************************/
#define DUMMY_COMMAND 					(0)
#define HINT_POPUP_FADE_TIME			(2000)

#define VDOREC_ERR_CORRUPT_FAT		(-1)
#define VDOREC_ERR_PHONE_DISK_FULL	(-2)
#define VDOREC_ERR_CARD_DISK_FULL	(-3)
#define VDOREC_ERR_WRITE_PROTECT		(-4)
#define VDOREC_ERR_ROOT_DIR_FULL		(-5)

#define VDOREC_OSD_BUFFER_SIZE 		(((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)*2) /* same as CAMERA_OSD_BUFFER_SIZE */


#define SHOW_EV_ZOOM
#define OPTION_ENABLED //defined by zhangl for option key 
/***************************************************************************** 
* Typedef 
*****************************************************************************/


/***************************************************************************** 
* Global Variable
*****************************************************************************/
extern wgui_inline_item		wgui_inline_items[];
extern BOOL						r2lMMIFlag;

#ifdef JATAAYU_SUPPORT
#ifdef JATAAYU_SUPPORT_MMS 
	extern INT8 jMMS_IsMMSAppOrDownloadActive(void);
#endif
#ifdef JATAAYU_SUPPORT_WAP
	extern  INT8 jMMS_IsWapConnected (void);
#endif
#endif
 
#ifndef __MMI_DISABLE_DUAL_SENSOR__
extern void  mmi_camera_preview_switch_sensor_hotkey_press(void);//, KEY_STAR , KEY_LONG_PRESS);
#endif

 
#ifndef APPMEM_SUPPORT
	extern U32 					camera_osd_layer_buffer[];
#endif /* APPMEM_SUPPORT */

static S8	buf_filename[FMGR_PATH_BUFFER_SIZE];
/****************************************************************************
* Global Function                                                           
*****************************************************************************/
extern void 		custom_start_flashlight(kal_uint8 red_level, kal_uint8 green_level, kal_uint8 blue_level, kal_uint8 duty);
extern void 		custom_stop_flashlight(void);
extern kal_uint8	GpioCtrlReq(kal_uint8 action);
extern BOOL 		IsClamClose(void);
extern void 		AudioStopReq(U16 soundId);
extern U16 			GetCurKeypadTone(void);
extern void 		refresh_status_icon_display(void); /* in Wgui_status_icons.c */

/***************************************************************************** 
* Local Variable
*****************************************************************************/
vdorec_context_struct		g_vdorec_cntx;
vdorec_menu_context_struct	g_vdorec_menu_cntx;

/***************************************************************************** 
* Local Function
*****************************************************************************/

#ifdef __MMI_UI_STYLE_4__
	static void mmi_vdorec_redraw_status_bg(void);
#endif /* __MMI_UI_STYLE_4__ */

/* app */
static void mmi_vdorec_blt_screen(void);
static void mmi_vdorec_hilight_app(void);
static void mmi_vdorec_restore_setting(void);
static BOOL mmi_vdorec_get_storage_disk_path(PS8 drv_buf);
static void vdorec_delete_RecFile(void);
static void mmi_vdorec_disply_popup(PU8 str_ptr,
												U16 img_id, 
												U16 image_on_bottom, 
												U16 tone_duration, 
												U16 tone_id);


/* preview */
static BOOL mmi_vdorec_check_and_display_error_popup(void);

#ifdef __VDOREC_FEATURE_VIDEO_SIZE__
	static U16  mmi_vdorec_resize_preview_layer(void);
#endif /* __VDOREC_FEATURE_VIDEO_SIZE__ */

static U16  mmi_vdorec_get_zoom_factor(void);
static void mmi_vdorec_lsk_press_hdlr(void);
static void mmi_vdorec_lsk_release_hdlr(void);
static void mmi_vdorec_rsk_press_hdlr(void);
static void mmi_vdorec_rsk_release_hdlr(void);
#ifdef SHORTCUT_DEFINED
static void mmi_vdorec_inc_ev_key_press_hdlr(void);
static void mmi_vdorec_inc_ev_key_release_hdlr(void);
static void mmi_vdorec_dec_ev_key_press_hdlr(void);
static void mmi_vdorec_dec_ev_key_release_hdlr(void);
#endif
#ifdef __VDOREC_FEATURE_ZOOM__
	static void mmi_vdorec_zoom_in(void);
#ifdef SHORTCUT_DEFINED
	static void mmi_vdorec_zoom_in_key_press_hdlr(void);
	static void mmi_vdorec_zoom_in_key_release_hdlr(void);
#endif	
	static void mmi_vdorec_zoom_out(void);
#ifdef SHORTCUT_DEFINED
	static void mmi_vdorec_zoom_out_key_press_hdlr(void);
	static void mmi_vdorec_zoom_out_key_release_hdlr(void);
#endif
#endif /* __VDOREC_FEATURE_ZOOM__ */

#ifdef SHORTCUT_DEFINED
static void mmi_vdorec_record_button_press_hdlr(void);
#endif
static void mmi_vdorec_turn_on_led_highlight(void);
static void mmi_vdorec_turn_off_led_highlight(void);
static void mmi_vdorec_record_result_hdlr_callback(MDI_RESULT result);

#ifdef SHORTCUT_DEFINED
#ifdef __VDOREC_FEATURE_EFFECT__
	static void mmi_vdorec_effect_setting_hotkey_press(void);
#endif /* __VDOREC_FEATURE_EFFECT__ */

#ifdef __VDOREC_FEATURE_WB__
	static void mmi_vdorec_wb_hotkey_press(void);
#endif /* __VDOREC_FEATURE_WB__ */

#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT__
	static void mmi_vdorec_led_highlight_hotkey_press(void);
#endif /* __VDOREC_FEATURE_LED_HIGHLIGHT__ */

#ifdef __VDOREC_FEATURE_NIGHT__
	static void mmi_vdorec_night_hotkey_press(void);
#endif /* __VDOREC_FEATURE_NIGHT__ */
#endif

static void mmi_vdorec_start_preview(void);
static void mmi_vdorec_enter_state(U16 state);

static U8 mmi_vdorec_post_interrupt_hdlr(void* inMsg);
static U8 mmi_vdorec_sms_interrupt_hdlr(void* inMsg);
static U8 mmi_vdorec_battery_status_interrupt_hdlr(void* inMsg);
static U8 mmi_vdorec_msdc_interrupt_hdlr(void* inMsg);

#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__)
	static U8 mmi_vdorec_clam_close_event_hdlr(void* ptr);
#endif /* __MMI_CLAMSHELL__ && __MMI_SLIDE__ */

static void mmi_vdorec_entry_app(void);
static void mmi_vdorec_exit_app(void);

#ifdef __MMI_SUBLCD__
	static void mmi_vdorec_draw_sublcd_icon(void);
	static void mmi_vdorec_entry_sublcd_screen(void);
	static void mmi_vdorec_exit_sublcd_screen(void);
#endif /* __MMI_SUBLCD__ */

static void mmi_vdorec_record_timer_cyclic(void);
static void mmi_vdorec_draw_title(void);
static void mmi_vdorec_draw_status(void);
static void mmi_vdorec_draw_softkey(void);
static void mmi_vdorec_set_softkey(PS8 lsk_str_ptr, PS8 rsk_str_ptr, U16 lsk_img_id, U16 rsk_img_id);
static void mmi_vdorec_draw_panel_state(void);

#ifdef __VDOREC_FEATURE_EV__
	static void mmi_vdorec_draw_panel_ev(void);
#endif /* __VDOREC_FEATURE_EV__ */

#ifdef __VDOREC_FEATURE_ZOOM__
	static void mmi_vdorec_draw_panel_zoom(void);
#endif /* __VDOREC_FEATURE_ZOOM__ */
static void mmi_vdoRec_draw_Recording_timer(U32 Pre_Recording_time,U32 Recording_time,U32 *is_ui_update);
static void mmi_vdorec_draw_panel_timer(void);
static void mmi_vdorec_draw_panel(void);
static void mmi_vdorec_init_panel(void);
static void mmi_vdorec_draw_style_text(PS8 str, S32 offset_x, S32 offset_y, video_style_text_struct *style_txt);
static void mmi_vdorec_hide_hint(void);
static void mmi_vdorec_draw_hint(void);
static void mmi_vdorec_set_hint(PS8 hint_str1, PS8 hint_str2, U16 fade_time);
static void mmi_vdorec_draw_osd(void);

/* save */
static void mmi_vdorec_save_confirm_no(void);
static void mmi_vdorec_entry_save_confirm_screen(void);
static void mmi_vdorec_save_result_hdlr_callback(MDI_RESULT result);
static void mmi_vdorec_exit_saving_screen(void);
static U8 	mmi_vdorec_saving_screen_del_hdlr(void* dummy_ptr);
static void mmi_vdorec_entry_saving_screen(void);

/* menu */
static void mmi_vdorec_init_option_menu(void);
static void mmi_vdorec_highlight_to_vdoply(void);
static void mmi_vdorec_entry_option_screen(void);
static void mmi_vdorec_highlight_camcoder_setting(void);
static void mmi_vdorec_camcoder_setting_lsk_press(void);
static void mmi_vdorec_camcoder_setting_highlight_hdlr(S32 index);
static void mmi_vdorec_exit_camcoder_setting_screen(void);
static void mmi_vdorec_entry_camcoder_setting_screen(void);

#ifndef __VDOREC_FEATURE_MERGE_MENU__
	#ifdef __VDOREC_FEATURE_MERGE_MENU__
	static void mmi_vdorec_highlight_video_setting(void);
	#endif
	static void mmi_vdorec_video_setting_lsk_press(void);
	static void mmi_vdorec_video_setting_highlight_hdlr(S32 index);
	static void mmi_vdorec_exit_video_setting_screen(void);
	static void mmi_vdorec_entry_video_setting_screen(void);
#endif /* !__VDOREC_FEATURE_MERGE_MENU__ */


static void mmi_vdorec_highlight_effect_setting(void);
static void mmi_vdorec_set_effect_setting_value(void);
static void mmi_vdorec_entry_effect_setting_screen(void);


#ifdef __VDOREC_FEATURE_STORAGE__
	static void mmi_vdorec_hint_storage(U16 index);
	static void mmi_vdorec_entry_storage_screen(void);
	static void mmi_vdorec_highlight_storage(void);
	static void mmi_vdorec_set_storage(void);
#endif /* __VDOREC_FEATURE_STORAGE__ */

static void mmi_vdorec_highlight_restore_default(void);
static void mmi_vdorec_entry_restore_default_confirm_yes(void);
static void mmi_vdorec_entry_restore_default_confirm_screen(void);

#if defined(__VDOREC_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
	static void mmi_vdorec_touch_scr_pen_down_hdlr(mmi_pen_point_struct pos);
	static void mmi_vdorec_touch_scr_pen_up_hdlr(mmi_pen_point_struct pos);
	static void mmi_vdorec_touch_scr_pen_move_hdlr(mmi_pen_point_struct pos);
	static BOOL mmi_vdorec_touch_scr_hit_test(S32 pos_x, S32 pos_y, vdorec_ui_touch_struct *touch_obj);
#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */

extern BOOL HandleLowBatteryProcess(void);
extern void mmi_show_qq_select_confirm(void (*default_entry) (void));

/****************************************************************************
* Setting command map                                                                
*****************************************************************************/

/*********** WB ***********/
const U8 vdorec_wb_command_map[VDOREC_SETTING_WB_COUNT+1] = 
{
#if 1 //chenhe for camera
	0,1,2,3,4,
#else
#ifdef __VDOREC_FEATURE_WB_AUTO__	
	MDI_VIDEO_WB_AUTO,								/*	CAMREA_SETTING_WB_AUTO */
#endif

#ifdef __VDOREC_FEATURE_WB_DAYLIGHT__
	MDI_VIDEO_WB_DAYLIGHT,							/* VDOREC_SETTING_WB_DAYLIGHT */
#endif

#ifdef __VDOREC_FEATURE_WB_TUNGSTEN__
	MDI_VIDEO_WB_TUNGSTEN,							/* VDOREC_SETTING_WB_TUNGSTEN */ 
#endif

#ifdef __VDOREC_FEATURE_WB_FLUORESCENT__
	MDI_VIDEO_WB_FLUORESCENT,						/* VDOREC_SETTING_WB_FLUORESCENT */
#endif

#ifdef __VDOREC_FEATURE_WB_CLOUD__
	MDI_VIDEO_WB_CLOUD,								/* VDOREC_SETTING_WB_CLOUD */
#endif

#ifdef __VDOREC_FEATURE_WB_INCANDESCENCE__
	MDI_VIDEO_WB_INCANDESCENCE,					/* VDOREC_SETTING_WB_INCANDESCENCE */
#endif	
#endif	
	DUMMY_COMMAND
};

/*********** EV for greenstone  ***********/
#define GS_VIDREC_EV_0 				(0)
#define GS_VIDREC_EV_P1 			(1)
#define GS_VIDREC_EV_P2 			(2)
#define GS_VIDREC_EV_P3 			(3)
#define GS_VIDREC_EV_P4 			(4)

const U8	vdorec_ev_command_map[VDOREC_SETTING_EV_COUNT+1] = 
{
	GS_VIDREC_EV_0,			/* VDOREC_SETTING_EV_0 */
	GS_VIDREC_EV_P1,			/* VDOREC_SETTING_EV_P1 */
	GS_VIDREC_EV_P2,			/* VDOREC_SETTING_EV_P2 */
	GS_VIDREC_EV_P3,			/* VDOREC_SETTING_EV_P3	*/
	GS_VIDREC_EV_P4,			/* VDOREC_SETTING_EV_P4 */

	DUMMY_COMMAND
};

/*********** Brightness for greenstone  ***********/
#define GS_VIDREC_BT_L1 			(0)
#define GS_VIDREC_BT_L2 			(1)
#define GS_VIDREC_BT_L3 			(2)
#define GS_VIDREC_BT_L4 			(3)
#define GS_VIDREC_BT_L5 			(4)

const U8	vdorec_bt_command_map[VDOREC_SETTING_BT_COUNT+1] = 
{
	GS_VIDREC_BT_L1,			/* VDOREC_SETTING_BRIGHT_1 */
	GS_VIDREC_BT_L2,			/* VDOREC_SETTING_BRIGHT_2 */
	GS_VIDREC_BT_L3,			/* VDOREC_SETTING_BRIGHT_3 */
	GS_VIDREC_BT_L4,			/* VDOREC_SETTING_BRIGHT_4	*/
	GS_VIDREC_BT_L5,			/* VDOREC_SETTING_BRIGHT_5 */

	DUMMY_COMMAND
};

/*********** Contract for greenstone  ***********/
#define GS_VIDREC_CT_L1 			(0)
#define GS_VIDREC_CT_L2 			(1)
#define GS_VIDREC_CT_L3 			(2)
#define GS_VIDREC_CT_L4 			(3)
#define GS_VIDREC_CT_L5 			(4)

const U8	vdorec_ct_command_map[VDOREC_SETTING_CONTRAST_COUNT+1] = 
{
	GS_VIDREC_CT_L1,			/* VDOREC_SETTING_CONTRAST_1 */
	GS_VIDREC_CT_L2,			/* VDOREC_SETTING_CONTRAST_2 */
	GS_VIDREC_CT_L3,			/* VDOREC_SETTING_CONTRAST_3 */
	GS_VIDREC_CT_L4,			/* VDOREC_SETTING_CONTRAST_4	*/
	GS_VIDREC_CT_L5,			/* VDOREC_SETTING_CONTRAST_5 */

	DUMMY_COMMAND
};

#if 0
/*********** EV ***********/
const U8	vdorec_ev_command_map[VDOREC_SETTING_EV_COUNT+1] = 
{
#ifdef __VDOREC_FEATURE_EV_4__	
	MDI_VIDEO_EV_N4,			/* VDOREC_SETTING_EV_N4 */
#endif

#ifdef __VDOREC_FEATURE_EV_3__		
	MDI_VIDEO_EV_N3,			/* VDOREC_SETTING_EV_N3 */
#endif

#ifdef __VDOREC_FEATURE_EV_2__	
	MDI_VIDEO_EV_N2,			/* VDOREC_SETTING_EV_N2 */
#endif

	MDI_VIDEO_EV_N1,			/* VDOREC_SETTING_EV_N1 */
	MDI_VIDEO_EV_0,			/* VDOREC_SETTING_EV_0 */
	MDI_VIDEO_EV_P1,			/* VDOREC_SETTING_EV_P1 */

#ifdef __VDOREC_FEATURE_EV_2__
	MDI_VIDEO_EV_P2,			/* VDOREC_SETTING_EV_P2 */
#endif

#ifdef __VDOREC_FEATURE_EV_3__
	MDI_VIDEO_EV_P3,			/* VDOREC_SETTING_EV_P3	*/
#endif

#ifdef __VDOREC_FEATURE_EV_4__
	MDI_VIDEO_EV_P4,			/* VDOREC_SETTING_EV_P4 */
#endif

	DUMMY_COMMAND
};
#endif
/*********** Banding ***********/
const U8 vdorec_banding_command_map[VDOREC_SETTING_BANDING_COUNT+1] =
{
	MDI_VIDEO_BANDING_50HZ,
	MDI_VIDEO_BANDING_60HZ,
	
	DUMMY_COMMAND
};

/*********** EFFECT ***********/
const U8 vdorec_effect_command_map[VDOREC_SETTING_EFFECT_COUNT+1] = 
{
#ifdef __VDOREC_FEATURE_EFFECT_NORMAL__	
	MDI_VIDEO_EFFECT_NOMRAL,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_GRAYSCALE__
	MDI_VIDEO_EFFECT_GRAYSCALE,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_SEPIA__
	MDI_VIDEO_EFFECT_SEPIA,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_SEPIA_GREEN__
	MDI_VIDEO_EFFECT_SEPIA_GREEN,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_SEPIA_BLUE__
	MDI_VIDEO_EFFECT_SEPIA_BLUE,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_COLOR_INVERT__
	MDI_VIDEO_EFFECT_COLOR_INVERT,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_GRAY_INVERT__
	MDI_VIDEO_EFFECT_GRAY_INVERT,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_BLACKBOARD__
	MDI_VIDEO_EFFECT_BLACKBOARD,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_WHITEBOARD__
	MDI_VIDEO_EFFECT_WHITEBOARD,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_COPPER_CARVING__
	MDI_VIDEO_EFFECT_COPPER_CARVING,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_BLUE_CARVING__
	MDI_VIDEO_EFFECT_BLUE_CARVING,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_EMBOSSMENT__
	MDI_VIDEO_EFFECT_EMBOSSMENT,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_CONTRAST__
	MDI_VIDEO_EFFECT_CONTRAST,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_JEAN__
	MDI_VIDEO_EFFECT_JEAN,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_SKETCH__
	MDI_VIDEO_EFFECT_SKETCH,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_OIL__
	MDI_VIDEO_EFFECT_OIL,
#endif

/////added  by zhangl
#ifdef  __VDOREC_FEATURE_EFFECT_ANTIQUE__
     MDI_VIDEO_EFFECT_ANTIQUE,
#endif

#ifdef  __VDOREC_FEATURE_EFFECT_REDRISH__
     MDI_VIDEO_EFFECT_REDRISH,
#endif

#ifdef  __VDOREC_FEATURE_EFFECT_GREENRISH__
    MDI_VIDEO_EFFECT_GREENRISH,
#endif

#ifdef  __VDOREC_FEATURE_EFFECT_BLUERISH__
    MDI_VIDEO_EFFECT_BLUERISH,
#endif


#ifdef __VDOREC_FEATURE_EFFECT_BLACKWHITE__
     MDI_VIDEO_EFFECT_BLACKWHITE,
#endif


#ifdef  __VDOREC_FEATURE_EFFECT_NEGATIVE__
    MDI_VIDEO_EFFECT_NEGATIVENESS,
#endif

	DUMMY_COMMAND
};


#if defined(MT6228) || defined(MT6229)
	 

	/*********** Video Size ***********/
	const U8 vdorec_video_size_command_map[VDOREC_SETTING_VIDEO_SIZE_COUNT+1] =
	{
		#ifdef __VDOREC_FEATURE_VIDEO_SIZE_SQCIF__
			MDI_VIDEO_VIDEO_SIZE_SQCIF,
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE_SQCIF__ */

		#ifdef __VDOREC_FEATURE_VIDEO_SIZE_QQVGA__
			MDI_VIDEO_VIDEO_SIZE_QQVGA,
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE_QQVGA__ */

		#ifdef __VDOREC_FEATURE_VIDEO_SIZE_QCIF__
			MDI_VIDEO_VIDEO_SIZE_QCIF,
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE_QCIF__ */

		#ifdef __VDOREC_FEATURE_VIDEO_SIZE_CIF__
			MDI_VIDEO_VIDEO_SIZE_CIF,
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE_CIF__ */
			
		DUMMY_COMMAND
	};

#else
	 

	/*********** Video Size ***********/
	const U8 vdorec_video_size_command_map[VDOREC_SETTING_VIDEO_SIZE_COUNT+1] =
	{
		MDI_VIDEO_VIDEO_SIZE_SQCIF,//added by zhangl for vidrec 128*96
		MDI_VIDEO_VIDEO_SIZE_QCIF,
		DUMMY_COMMAND
	};
#endif



/*********** Video Qty ***********/
const U8 vdorec_video_qty_command_map[VDOREC_SETTING_VIDEO_QTY_COUNT+1] =
{
	#ifdef __VDOREC_FEATURE_VIDEO_QTY_FINE__
		MDI_VIDEO_VIDEO_QTY_FINE,
	#endif /* __VDOREC_FEATURE_VIDEO_QTY_FINE__ */

	#ifdef __VDOREC_FEATURE_VIDEO_QTY_HIGH__
		MDI_VIDEO_VIDEO_QTY_HIGH,
	#endif /* __VDOREC_FEATURE_VIDEO_QTY_HIGH__ */

	#ifdef __VDOREC_FEATURE_VIDEO_QTY_NORMAL__
		MDI_VIDEO_VIDEO_QTY_NORMAL,
	#endif /* __VDOREC_FEATURE_VIDEO_QTY_NORMAL__ */

	#ifdef __VDOREC_FEATURE_VIDEO_QTY_LOW__
		MDI_VIDEO_VIDEO_QTY_LOW,
	#endif /* __VDOREC_FEATURE_VIDEO_QTY_LOW__ */
		
	DUMMY_COMMAND
};


/*********** Video Format ***********/
const U8 vdorec_video_format_command_map[VDOREC_SETTING_VIDEO_FORMAT_COUNT+1] =
{
	MDI_VIDEO_VIDEO_FORMAT_3GP,
	MDI_VIDEO_VIDEO_FORMAT_MP4,
		
	DUMMY_COMMAND
};







/*****************************************************************************
*
* Applcation Related
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	mmi_vdorec_is_in_app
* DESCRIPTION
*  is in video recorder app or not
* PARAMETERS
*  void
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
BOOL mmi_vdorec_is_in_app(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdorec_cntx.state != VDOREC_STATE_EXIT)
		return TRUE;
	else
		return FALSE;
}


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_blt_screen
* DESCRIPTION
*  blt full screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_blt_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		gdi_layer_blt_previous(0, 0, UI_device_height-1, UI_device_width-1);
	#else
		#if defined(__MMI_MAINLCD_220X176__)
			gdi_layer_blt_previous(182, 0, UI_device_width-1, UI_device_height-1);
			gdi_layer_blt_previous(0, g_vdorec_osd_cntx.preview_wnd.rect.offset_y +g_vdorec_osd_cntx.preview_wnd.rect.height, UI_device_width-1, UI_device_height-1);
		#else
		#if defined(__VDOREC_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
			gdi_layer_blt_previous(0, g_vdorec_osd_cntx.preview_wnd.rect.offset_y +g_vdorec_osd_cntx.preview_wnd.rect.height, UI_device_width-1, UI_device_height-1);
		#else 
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);	
		#endif
		#endif
//		gdi_layer_blt_previous(0,0, g_vdorec_osd_cntx.preview_wnd.rect.offset_y +g_vdorec_osd_cntx.preview_wnd.rect.height,  g_vdorec_osd_cntx.preview_wnd.rect.offset_x +g_vdorec_osd_cntx.preview_wnd.rect.width);	

	#endif 
}
/*****************************************************************************
* FUNCTION
*	mmi_vdorec_blt_hint_screen
* DESCRIPTION
*  just blt screen for hint
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_blt_hint_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	gdi_layer_blt_previous(0,g_vdorec_osd_cntx.preview_wnd.rect.offset_y, UI_device_width-1,  g_vdorec_osd_cntx.preview_wnd.rect.offset_y +g_vdorec_osd_cntx.preview_wnd.rect.height);	

}

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_blt_screen
* DESCRIPTION
*  blt full screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_blt_full_screen(void)
{

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		gdi_layer_blt_previous(0, 0, UI_device_height-1, UI_device_width-1);
	#else
		gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);	
	#endif 
	
}

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_hilight_app
* DESCRIPTION
*  highlight vodrec menuitem
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_hilight_app(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	mmi_trace(1," STR_ID_VDOREC_PAUSE= %d,STR_ID_VDOREC_WB=%d,STR_ID_VDOREC_NOTIFY_DISK_FULL=%d,STR_ID_VDOREC_WB_AUTO=%d",STR_ID_VDOREC_PAUSE,STR_ID_VDOREC_WB,STR_ID_VDOREC_NOTIFY_DISK_FULL,STR_ID_VDOREC_WB_AUTO);
//	return ; //delete by zhangl 11-17
	SetLeftSoftkeyFunction(mmi_vdorec_entry_app,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(mmi_vdorec_entry_app, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */	
}


 
/*****************************************************************************
* FUNCTION
*	mmi_vdorec_init_app
* DESCRIPTION
*  init vdorec applcation
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdorec_init_app(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetHiliteHandler(MENU_ID_VDOREC_APP, mmi_vdorec_hilight_app);
	
	#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
		SetHiliteHandler(MAIN_MENU_OPTION_VDOREC_APP, mmi_vdorec_hilight_app);
	#endif
	g_vdorec_cntx.state							= VDOREC_STATE_EXIT;
	g_vdorec_cntx.last_error					= MDI_RES_VDOREC_SUCCEED;
	g_vdorec_cntx.is_storage_path_changed	= FALSE;
	g_vdorec_cntx.is_saving						= FALSE;
	g_vdorec_cntx.is_sub_display				= FALSE;
	g_vdorec_cntx.is_setting_loaded			= FALSE;
	g_vdorec_cntx.is_force_saving				= FALSE;

	/* Init Option Menu */
	mmi_vdorec_init_option_menu();
}


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_redraw_status_bg
* DESCRIPTION
*  redraw status backgroud
* PARAMETERS
*  void
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __MMI_UI_STYLE_4__
static void mmi_vdorec_redraw_status_bg(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   S32 x1, y1, x2, y2;
   	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  	get_status_icon_bar_clip(0, &x1, &y1, &x2, &y2);
   
	gdi_layer_push_clip();
   gdi_layer_set_clip(x1, y1, x2, y2);
	gdi_image_draw_id(0, 0, IMG_ID_VDOREC_OSD_BG);

   gdi_layer_pop_clip();
}
#endif /* __MMI_UI_STYLE_4__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_load_setting
* DESCRIPTION
*  load setting
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdorec_load_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16	error;
   /*+ zhouqin modify for dual t-flash card 20110503*/
   #ifdef DUAL_TFLASH_SUPPORT
    S16 cardDrvLetter;
    S16 secardDrvLetter;
   #endif
   /*- zhouqin modify for dual t-flash card 20110503*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* if already loaded into memory, do not need load again */
   if(!g_vdorec_cntx.is_setting_loaded)
   {
		ReadRecord(NVRAM_EF_VIDEO_SETTING_LID, 1, (void*)&g_vdorec_cntx.setting , NVRAM_EF_VIDEO_SETTING_SIZE, &error);
		mmi_trace(g_sw_Vidrec,"---readrecord(): wb is %d-----",g_vdorec_cntx.setting.wb);
		/* first time init */
		if(g_vdorec_cntx.setting.wb == 0xff)
		{
			mmi_vdorec_restore_setting();
			mmi_trace(g_sw_Vidrec,"---init wb is %d-----",g_vdorec_cntx.setting.wb);
		}

		g_vdorec_cntx.setting.led_value = VDOREC_DEFAULT_PREVIEW_LED_VALUE;

		g_vdorec_cntx.is_setting_loaded = TRUE;
   }
   
/*+ zhouqin modify for dual t-flash card 20110503*/
#ifndef DUAL_TFLASH_SUPPORT
        g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_MEMORY_CARD;  //added by guojian ,only card is available 
#else		
	//if storage is t-flash but it does not exist,
	//reset storage before enter setting screen.
	if(g_vdorec_cntx.setting.storage== VDOREC_SETTING_STORAGE_MEMORY_CARD && 
		((cardDrvLetter = MMI_CARD_DRV) < 0))
	{	
            if((secardDrvLetter = MMI_SEC_CARD_DRV) < 0)
            {
            }
            else
            {
                g_vdorec_cntx.setting.storage= VDOREC_SETTING_STORAGE_MEMORY_SEC_CARD;
                mmi_vdorec_store_setting();
            }
		
	}
	else if(g_vdorec_cntx.setting.storage== VDOREC_SETTING_STORAGE_MEMORY_SEC_CARD && 
		((secardDrvLetter = MMI_SEC_CARD_DRV) < 0))
	{		
		g_vdorec_cntx.setting.storage= VDOREC_SETTING_STORAGE_MEMORY_CARD;
		mmi_vdorec_store_setting();
	}		         
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_store_setting
* DESCRIPTION
*  store setting
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdorec_store_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16	error;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   WriteRecord(NVRAM_EF_VIDEO_SETTING_LID, 1, (void*)&g_vdorec_cntx.setting, NVRAM_EF_VIDEO_SETTING_SIZE, &error);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_restore_setting
* DESCRIPTION
*  restore setting
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_restore_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdorec_cntx.setting.wb				= VDOREC_DEFAULT_SETTING_WB;
	g_vdorec_cntx.setting.ev				= VDOREC_DEFAULT_SETTING_EV;
	g_vdorec_cntx.setting.led_highlight	= VDOREC_DEFAULT_SETTING_LED_HIGHLIGHT;
	g_vdorec_cntx.setting.night			= VDOREC_DEFAULT_SETTING_NIGHT;
	g_vdorec_cntx.setting.banding			= VDOREC_DEFAULT_SETTING_BANDING;
	
	g_vdorec_cntx.setting.effect			= VDOREC_DEFAULT_SETTING_EFFECT;

	g_vdorec_cntx.setting.video_size		= VDOREC_DEFAULT_SETTING_VIDEO_SIZE;	
	g_vdorec_cntx.setting.video_qty		= VDOREC_DEFAULT_SETTING_VIDEO_QTY;
	g_vdorec_cntx.setting.size_limit		= VDOREC_DEFAULT_SETTING_SIZE_LIMIT;
	g_vdorec_cntx.setting.time_limit		= VDOREC_DEFAULT_SETTING_TIME_LIMIT;
	g_vdorec_cntx.setting.record_aud		= VDOREC_DEFAULT_SETTING_RECORD_AUD;
	g_vdorec_cntx.setting.video_format	= VDOREC_DEFAULT_SETTING_VIDEO_FORMAT;
	g_vdorec_cntx.setting.storage			= VDOREC_SETTING_STORAGE_MEMORY_CARD;
	
	g_vdorec_cntx.setting.brightness		=VDOREC_DEFAULT_SETTING_BT;//VDOREC_DEFAULT_SETTING_EV ;	
	g_vdorec_cntx.setting.contrast		=VDOREC_DEFAULT_SETTING_CT; //2;	
	g_vdorec_cntx.setting.zoom				= VDOREC_DEFAULT_SETTING_ZOOM;

	mmi_vdorec_store_setting();
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_check_and_restore_valid_drv
* DESCRIPTION
*  check if current storage path is valid or not.
* PARAMETERS
*  void
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
extern BOOL g_hasMountedTFlashFS;
extern BOOL g_hasMountedNandFlashFS;
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
extern BOOL g_hasMountedTFlash2FS;
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
BOOL mmi_vdorec_check_and_restore_valid_drv(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32			ret_drv;
  
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* Get storage location */
	if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_PHONE)
	{
		/* path is phone */
		ret_drv = MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
	}
	else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_CARD)
	{
		/* path is memory card */
		ret_drv 	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
	}
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
        else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		/* path is memory card2*/
		ret_drv 	= MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND, 1, FS_NO_ALT_DRIVE);
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	else
	{
		ASSERT(0);
		return FALSE;
	}

	
	if(ret_drv >= 0)
	{
#if defined(MMI_ON_WIN32)
		return TRUE;
#endif
#if 1 //chenhe
		if('d' == ret_drv || 'D' == ret_drv)
		{
			return g_hasMountedTFlashFS;
		}
		if('e' == ret_drv || 'E' == ret_drv)
		{
			return g_hasMountedNandFlashFS;
		}
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
		if('f' == ret_drv || 'F' == ret_drv)
		{
			return g_hasMountedTFlash2FS;
		}
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/
		else
		{
			return TRUE;
		}
#else
		/* drive available */
		sprintf(buf, "%c:\\", (U8)ret_drv);
		AnsiiToUnicodeString(UCS2_buf, buf);

		file_handle = MMI_FS_Open((kal_uint16*)UCS2_buf, FS_OPEN_DIR | FS_READ_ONLY);
		
		/* Memory card not available */
		if(file_handle >= 0)
			MMI_FS_Close(file_handle);
		else
		{
		#if 0
			/* card not available error */
			g_vdorec_cntx.setting.storage				= VDOREC_SETTING_STORAGE_PHONE;
			g_vdorec_cntx.is_storage_path_changed	= TRUE;
			mmi_vdorec_store_setting();
		#endif
			return FALSE;
		}
#endif		
	}
	else
	{
	#if 0
		/* drive not available error */
		g_vdorec_cntx.setting.storage				= VDOREC_SETTING_STORAGE_PHONE;
		g_vdorec_cntx.is_storage_path_changed	= TRUE;
		mmi_vdorec_store_setting();
	#endif
		return FALSE;
	}
	

	return TRUE;
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_get_storage_disk_path
* DESCRIPTION
*  get current active storage disk path
* PARAMETERS
*  void
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static BOOL mmi_vdorec_get_storage_disk_path(PS8 drv_buf)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32	ret_drv;
	S8		buf[64];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_PHONE)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);

		if(ret_drv >=0)
		{
			sprintf(buf, "%c:/", (U8)ret_drv);
			AnsiiToUnicodeString(drv_buf, buf);
			return TRUE;
		}
		else
		{
			/* phone drive should not fail */
			ASSERT(0);
			return FALSE;
		}
	}
	else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE , 1, FS_NO_ALT_DRIVE);

		if(ret_drv >=0)
		{
			sprintf(buf, "%c:/", (U8)ret_drv);
			AnsiiToUnicodeString(drv_buf, buf);
			return TRUE;
		}
		else
		{
			return FALSE;
		}

	}
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND , 1, FS_NO_ALT_DRIVE);

		if(ret_drv >=0)
		{
			sprintf(buf, "%c:/", (U8)ret_drv);
			AnsiiToUnicodeString(drv_buf, buf);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	return FALSE;
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_get_storage_file_path
* DESCRIPTION
*  get current active storage disk path
* PARAMETERS
*  a  OUT	storage file path
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
BOOL mmi_vdorec_get_storage_file_path(PS8 filepath)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8		drv_buf[8];
	S8   file_temp[100];
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   /* load setting, this get file path function may be called by other applcations */
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

	mmi_vdorec_load_setting();
	mmi_trace(g_sw_VdoRec,"mmi_vdorec_get_storage_file_path,g_vdorec_cntx.setting.storage is %d",g_vdorec_cntx.setting.storage);
	if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_PHONE)
	{
		if(mmi_vdorec_get_storage_disk_path(drv_buf))
		{
			pfnUnicodeStrcpy((PS8)filepath, (PS8)drv_buf);
			AnsiiToUnicodeString(file_temp,(PS8)VDOREC_STORAGE_FILEPATH_PHONE);
			pfnUnicodeStrcat((PS8)filepath, file_temp);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_CARD)
	{
		if(mmi_vdorec_get_storage_disk_path(drv_buf))
		{
			pfnUnicodeStrcpy((PS8)filepath, (PS8)drv_buf);
			AnsiiToUnicodeString(file_temp,(PS8)VDOREC_STORAGE_FILEPATH_MEMORY_CARD);
			pfnUnicodeStrcat((PS8)filepath, file_temp);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		if(mmi_vdorec_get_storage_disk_path(drv_buf))
		{
			pfnUnicodeStrcpy((PS8)filepath, (PS8)drv_buf);
			AnsiiToUnicodeString(file_temp,(PS8)VDOREC_STORAGE_FILEPATH_MEMORY_SEC_CARD);
			pfnUnicodeStrcat((PS8)filepath, file_temp);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	else
	{
		ASSERT(0);

	}
	return FALSE;
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_create_file_dir
* DESCRIPTION
*  create file directory-resursivly
* PARAMETERS
*  a  IN		filepath to be created
* RETURNS
*  S32
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
S32 mmi_vdorec_create_file_dir(S8* filepath)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8				buf_createpath[128];
	S8				buf_filepath[FMGR_PATH_BUFFER_SIZE],tmp[256];
	S32			fs_ret = -1;
	S32			str_len;
	S32			cmp_ret;
	S32			i;
	FS_HANDLE	file_handle= FS_ERROR_RESERVED;
	S8 slash_tmp[10];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	file_handle = MMI_FS_Open((const UINT8 *)filepath, FS_OPEN_DIR | FS_READ_ONLY);

	/* path already exist */
	//if(file_handle== FS_FOLDER_HANDLE)
	if(file_handle>=0)
	{
		MMI_FS_Close(file_handle);
		return 0; /* already exist, 0 means successful in file system */
	}
	
	/* create directory if it is not there */
	pfnUnicodeStrcpy((PS8)buf_filepath, (PS8)filepath);


	str_len = pfnUnicodeStrlen((PS8)buf_filepath);
	str_len *= ENCODING_LENGTH;
	AnsiiToUnicodeString(slash_tmp,(PS8)"/");

	/* skip drive. "C:\" */
	for(i = 3*ENCODING_LENGTH ; i < str_len; i+=2)
	{
		cmp_ret = pfnUnicodeStrncmp((PS8)&buf_filepath[i], slash_tmp, 1);
		if(cmp_ret == 0)
		{
			/* create dir */

			pfnUnicodeStrncpy((PS8)buf_createpath, (PS8)buf_filepath, i/ENCODING_LENGTH);
			buf_createpath[i]		= '\0';
			buf_createpath[i+1]	= '\0';
			fs_ret = MMI_FS_CreateDir((const UINT8 *)buf_createpath);	
			UnicodeToAnsii(tmp,buf_createpath);
			mmi_trace(TRUE,"mmi_vdorec_create_file_dir,fs_ret is %d,buf_createpath is %s",fs_ret,tmp);
			if(fs_ret < 0)
				return fs_ret;
		}
	}

	return fs_ret;
}

/*****************************************************************************
* FUNCTION
*	 mmi_vidrec_get_next_filename()
* DESCRIPTION
*   get next file name for m-jpeg
* PARAMETERS
*	 a  OUT    		next filename
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
 U16	vrfilename_seq_no;
//extern vdorec_context_struct g_vdorec_cntx;
void mmi_vidrec_get_next_filename(PS8 file_buf_p)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8					buffer[25];
	S8					UCS2_buffer[50];
	S8					UCS2_testname[50];
	S8					UCS2_alpha[10];
	S16				error = 0;

	U16				alpha_count;
	U16				alpha_index;
	FS_HANDLE		file_handle;

	//wufasong added 2007.01.19
//	S8 filename_ext_jpg[] = {0x2E,0x00,0x6A,0x00,0x70,0x00,0x67,0x00,0x00,0x00};
	S8 filename_ext_avi[] = {0x2E,0x00,0x61,0x00,0x76,0x00,0x69,0x00,0x00,0x00};
    mmi_trace(g_sw_Vidrec, "Excute func : mmi_vidrec_get_next_filename()");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

	/* get/store sequentail number */
	ReadValue(NVRAM_VDOREC_FILENAME_SEQ_NO, &vrfilename_seq_no, DS_SHORT , &error);
	if(vrfilename_seq_no == 0xffff)
	{
		vrfilename_seq_no = 1;	/* init value */
	}
	else 
	{
		vrfilename_seq_no++;
		if(vrfilename_seq_no > 9999)	/* more than 4 digits */
			vrfilename_seq_no = 1;
	}
//	WriteValue(NVRAM_VDOREC_FILENAME_SEQ_NO, &filename_seq_no, DS_SHORT , &error);

	alpha_count		= 'Z'-'A'+1 ;
	alpha_index		= 0;

	sprintf((PS8)buffer, "VIDREC%04d", vrfilename_seq_no);
	mmi_trace(g_sw_Vidrec, "filename_seq_no = %d", vrfilename_seq_no);
	mmi_trace(g_sw_Vidrec, "buffer = %s", buffer);
	AnsiiToUnicodeString((PS8)UCS2_buffer, (PS8)buffer);	

	while(1)
	{
		sprintf((PS8)buffer, "%c", 'A'+alpha_index);
		mmi_trace(g_sw_Vidrec, "-- %s --", buffer);
		AnsiiToUnicodeString((PS8)UCS2_alpha, (PS8)buffer);

		pfnUnicodeStrcpy(UCS2_testname, UCS2_buffer);
		pfnUnicodeStrcat(UCS2_testname, UCS2_alpha);
		//pfnUnicodeStrcat(UCS2_testname, (PS8)L".jpg"); /* wufasong updated 2007.01.19 */
		pfnUnicodeStrcat(UCS2_testname, filename_ext_avi);

              mmi_trace(g_sw_Vidrec, "g_camera_cntx.storage_filepath = .....");
//		mmi_trace_hex(g_sw_CAMERA, UCS2Strlen(g_camera_cntx.storage_filepath) * 2, g_camera_cntx.storage_filepath);
//		mmi_trace_hex(g_sw_CAMERA, UCS2Strlen(UCS2_testname) * 2, UCS2_testname);
		//wufasong updated since it is only PHONE storage 2007.01.19
		pfnUnicodeStrcpy((PS8)file_buf_p, (PS8)g_vdorec_cntx.storage_filepath);
		pfnUnicodeStrcat((PS8)file_buf_p, (PS8)UCS2_testname);
		//pfnUnicodeStrcpy((PS8)file_buf_p, (PS8)UCS2_testname);

		file_handle = MMI_FS_Open((const UINT8 *)file_buf_p, FS_READ_ONLY);
	
		if(file_handle >= 0)
		{
			mmi_trace(g_sw_Vidrec, "file exist");
			//mmi_trace_hex(g_sw_CAMERA, UCS2Strlen(UCS2_testname) * 2, UCS2_testname);
			/* file exist */
			MMI_FS_Close(file_handle);
			alpha_index++;	
		}
		else
		{
			mmi_trace(g_sw_Vidrec, "file not exist");
			/* file not exit. return */
			return;
		}

		/* exit condition - will get a IMGXXXXZ.jpg as file name */
		if(alpha_index >= alpha_count-1)
			return;
	}
}

/*****************************************************************************
* FUNCTION
*	 mmi_vidrec_store_filename()
* DESCRIPTION
*   store file name for m-jpeg
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_vidrec_store_filename(void){

		S16		error = 0;

		WriteValue(NVRAM_VDOREC_FILENAME_SEQ_NO, &vrfilename_seq_no, DS_SHORT , &error);

}




/*****************************************************************************
* FUNCTION
*	mmi_vdorec_get_save_filename
* DESCRIPTION
*  get next file name to be recorded
* PARAMETERS
*  a  UUT		filename buffer ptr
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
//to open when need
#if 0 //chenhe to delete warning
static void mmi_vdorec_get_save_filename(S8 *file_buf_p)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8					buffer[25];
	S8					UCS2_buffer[50];
	S8					UCS2_testname[50];
	S8					UCS2_alpha[10];
	S16				error = 0;
	U16				filename_seq_no;
	U16				alpha_count;
	U16				alpha_index;
	FS_HANDLE		file_handle= FS_ERROR_RESERVED;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* get/store sequentail number */
	ReadValue(NVRAM_VDOREC_FILENAME_SEQ_NO, &filename_seq_no, DS_SHORT , &error);
   
	if(filename_seq_no == 0xffff)
	{
		filename_seq_no = 1;	/* init value */
	}

	g_vdorec_cntx.filename_seq_no = filename_seq_no;

	alpha_count		= 'Z'-'A'+1 ;
	alpha_index		= 0;

	sprintf((PS8)buffer, "MOV%04d", filename_seq_no);
	AnsiiToUnicodeString((PS8)UCS2_buffer, (PS8)buffer);	

	while(1)
	{
		sprintf((PS8)buffer, "%c", 'A'+alpha_index);
		AnsiiToUnicodeString((PS8)UCS2_alpha, (PS8)buffer);

		pfnUnicodeStrcpy(UCS2_testname, UCS2_buffer);
		pfnUnicodeStrcat(UCS2_testname, UCS2_alpha);
		
		#if defined(ISP_SUPPORT)
			pfnUnicodeStrcat(UCS2_testname, (PS8)L".3gp");
		#elif defined(CAMERA_MODULE)
			pfnUnicodeStrcat(UCS2_testname, (PS8)L".avi");
		#endif /* chip version */

		pfnUnicodeStrncpy((PS8)file_buf_p, (PS8)g_vdorec_cntx.storage_filepath, FMGR_MAX_PATH_LEN);
		pfnUnicodeStrcat((PS8)file_buf_p, (PS8)UCS2_testname);

		file_handle = MMI_FS_Open((const UINT8 *)file_buf_p, FS_READ_ONLY);
	
		if(file_handle >= 0)
		{
			/* file exist */
			MMI_FS_Close(file_handle);
			alpha_index++;	
		}
		else
		{
			/* file not exit. return */
			g_vdorec_cntx.filename_seq_no++;
			WriteValue(NVRAM_VDOREC_FILENAME_SEQ_NO, &g_vdorec_cntx.filename_seq_no, DS_SHORT , &error);			
			return;
		}

		/* exit condition - will get a IMGXXXXZ.jpg as file name */
		if(alpha_index >= alpha_count-1)
		{
			g_vdorec_cntx.filename_seq_no++;
			WriteValue(NVRAM_VDOREC_FILENAME_SEQ_NO, &g_vdorec_cntx.filename_seq_no, DS_SHORT , &error);
			return;
		}
	}

}
#endif
/*****************************************************************************
* FUNCTION
*	mmi_vdorec_creat_file
* DESCRIPTION
*  to creat a new file for storing avi
* PARAMETERS
*  void
* RETURNS
*  error_type
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#if 0
#include "mmc_videorecord.h"
extern VIDREC_CONTEXT g_VidRecCntx;
extern FS_INFO g_FsVidRecInfo;
error_type mmi_vdorec_creat_file(void){
	
  char filename_xuml[520];
  uint8 tmp[128]="/t";
  char buf_filename[ 520 ];
/////////////////////////////////////////////////
/*******initialization of  the file process ********/
/////////////////////////////////////////////////
    FS_GetFSInfo("mmc0", &g_FsVidRecInfo);
   mmi_vidrec_get_next_filename(buf_filename);
   UnicodeToAnsii(filename_xuml, buf_filename);
   strcat(tmp,filename_xuml+2);
   AnsiiToUnicodeString(buf_filename,tmp);
    g_VidRecCntx.fileHandle = FS_Create(buf_filename, 0); // , FS_O_CREAT);
    if (g_VidRecCntx.fileHandle < 0)
    {
      diag_printf("fail to creat file: %s \n", buf_filename);
      return ERROR_OPENFAIL;
    }
    else
      diag_printf("success to creat  file, name: %s\n", buf_filename);
	return NO_ERRO;
}
#endif

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_disply_popup
* DESCRIPTION
*  display popup. will roate if is horizontal screen 
* PARAMETERS
*  str_ptr				IN		string pointer
*	img_id				IN		image id
*	image_on_bottom	IN		is image on bottom
*	tone_duration		IN		tone duration
*	tone_id				IN 	tine id
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_disply_popup(PU8 str_ptr,
												U16 img_id, 
												U16 image_on_bottom, 
												U16 tone_duration, 
												U16 tone_id)
{
	#if defined(__VDOPLY_FEATURE_HORIZONTAL_VIEW__)
		DisplayPopupRotated((PU8)str_ptr, (U16)img_id, (U8)image_on_bottom, (U32)tone_duration, (U8)tone_id, MMI_FRM_SCREEN_ROTATE_270);
	#else
		DisplayPopup((PU8)str_ptr, (U16)img_id, (U8)image_on_bottom, (U32)tone_duration, (U8)tone_id);
	#endif /* __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */
}



/*****************************************************************************
*
* Preview Screen
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_check_and_display_error_popup
* DESCRIPTION
*  display error popup.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static BOOL mmi_vdorec_check_and_display_error_popup(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if( g_vdorec_cntx.last_error != MDI_RES_VDOREC_SUCCEED)
	{
		ClearAllKeyHandler();
		switch(g_vdorec_cntx.last_error)
		{
		case MDI_RES_VDOREC_ERR_DISK_FULL:
			mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			break;			
		case MDI_RES_VDOREC_REACH_DISK_LIMIT:
			mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			mmi_trace(1,"file too small and delete it--");
   			vdorec_delete_RecFile();
			break;

		case MDI_RES_VDOREC_ERR_NO_DISK:
			mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD/*STR_ID_VDOREC_NOTIFY_NO_MEMORY_CARD*/), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			DeleteNHistory(1);
			break;

		case MDI_RES_VDOREC_ERR_WRITE_PROTECTION:
			mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_WRITE_PROTECTION/*STR_ID_VDOREC_NOTIFY_WRITE_PROTECTION*/), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			break;


		/* followings are serious error, has to exit app */
		case MDI_RES_VDOREC_ERR_POWER_ON_FAILED:
			mmi_vdorec_disply_popup((PU8)GetString(STR_ID_CAMERA_NOTIFY_HW_NOT_READY/*STR_ID_VDOREC_NOTIFY_HW_NOT_READY*/), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			DeleteNHistory(1);
			break;		
			
		case MDI_RES_VDOREC_ERR_PREVIEW_FAILED:
		case MDI_RES_VDOREC_ERR_RESUME_FAILED:
		case MDI_RES_VDOREC_ERR_PAUSE_FAILED:
		case MDI_RES_VDOREC_ERR_SAVE_FILE_FAILED:
		case MDI_RES_VDOREC_ERR_RECORD_FAILED:
			mmi_vdorec_disply_popup((PU8)GetString(STR_ID_CAMERA_NOTIFY_ERROR/*STR_ID_VDOREC_NOTIFY_ERROR*/), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			DeleteNHistory(1);
			break;

		case VDOREC_ERR_CORRUPT_FAT:
			mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_FAT_ALLOC_ERROR_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			DeleteNHistory(1);
			break;

		case VDOREC_ERR_PHONE_DISK_FULL:
			mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			DeleteNHistory(1);
			break;	

		case VDOREC_ERR_WRITE_PROTECT:
			mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_WRITE_PROTECTION_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			break;			

		case VDOREC_ERR_CARD_DISK_FULL:
			mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			break;
			
		case VDOREC_ERR_ROOT_DIR_FULL:
			mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			DeleteNHistory(1);			
			break;

		default:	
			MMI_ASSERT(0); /* use assert here, make sure all error case is handled properly */
		}

		/* clear record keys */
		ClearKeyHandler(VDOREC_RECORD_KEY, KEY_EVENT_UP);	
		
		if(VDOREC_RECORD_EXTRA_KEY!=-1)
			ClearKeyHandler(VDOREC_RECORD_EXTRA_KEY, KEY_EVENT_UP);

		g_vdorec_cntx.last_error = MDI_RES_VDOREC_SUCCEED;
		return TRUE;
	}
	else 
		return FALSE;
}




/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_resize_preview_layer()
* DESCRIPTION
*   resize preview layer 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_VIDEO_SIZE__
static U16 mmi_vdorec_resize_preview_layer(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32	video_width;
   S32 	video_height;
   S32	resized_offset_x;
   S32	resized_offset_y;   
   S32	resized_width;
   S32	resized_height;   
      
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	video_width 	= 0;
	video_height 	= 0;
	
	#ifdef __VDOREC_FEATURE_VIDEO_SIZE_SQCIF__
		if(g_vdorec_cntx.setting.video_size == VDOREC_SETTING_VIDEO_SIZE_SQCIF)
		{
			video_width 	= 129;
			video_height 	= 96;
		}
	#endif /* __VDOREC_FEATURE_VIDEO_SIZE_SQCIF__ */

	#ifdef __VDOREC_FEATURE_VIDEO_SIZE_QQVGA__
		if(g_vdorec_cntx.setting.video_size == VDOREC_SETTING_VIDEO_SIZE_QQVGA)
		{
			video_width 	= 160;
			video_height 	= 120;
		}
	#endif /* __VDOREC_FEATURE_VIDEO_SIZE_QQVGA__ */

	#ifdef __VDOREC_FEATURE_VIDEO_SIZE_QCIF__
		if(g_vdorec_cntx.setting.video_size == VDOREC_SETTING_VIDEO_SIZE_QCIF)
		{
			video_width 	= 176;
			video_height 	= 144;
		}
	#endif /* __VDOREC_FEATURE_VIDEO_SIZE_QCIF__ */

	#ifdef __VDOREC_FEATURE_VIDEO_SIZE_CIF__
		if(g_vdorec_cntx.setting.video_size == VDOREC_SETTING_VIDEO_SIZE_CIF)
		{
			video_width 	= 352;
			video_height 	= 288;
		}
	#endif /* __VDOREC_FEATURE_VIDEO_SIZE_CIF__ */

	/* make sure the size is valid */
	MMI_ASSERT(video_width != 0);

//	if((video_width > g_vdorec_osd_cntx.preview_wnd.rect.width) || 
//		(video_width > g_vdorec_osd_cntx.preview_wnd.rect.height))
//	{
		gdi_util_fit_box(	GDI_UTIL_MODE_LONG_SIDE_FIT, 
								g_vdorec_osd_cntx.preview_wnd.rect.width,
								g_vdorec_osd_cntx.preview_wnd.rect.height,
								video_width, 
								video_height, 
								&resized_offset_x, 
								&resized_offset_y,
								&resized_width, 
								&resized_height);
//	}
//	else
//	{
//		resized_offset_x 	= 0;
//		resized_offset_y 	= 0;
//		resized_width 	 	= video_width;
//		resized_height 	= video_height;		
//	}


	gdi_layer_push_and_set_active(g_vdorec_cntx.preview_layer_handle);

	gdi_layer_resize(resized_width, resized_height);
	gdi_layer_set_position(	g_vdorec_osd_cntx.preview_wnd.rect.offset_x+resized_offset_x, 
									g_vdorec_osd_cntx.preview_wnd.rect.offset_y+resized_offset_y);

	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{			
				mdi_tvout_resize_buffer(resized_width, resized_height);
			}
		}
	#endif /* __MMI_TVOUT__ */

	gdi_layer_pop_and_restore_active();
	
}
#endif /* __VDOREC_FEATURE_VIDEO_SIZE__ */


/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_get_zoom_factor()
* DESCRIPTION
*   get zoom factor based on feature define
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static U16 mmi_vdorec_get_zoom_factor(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOREC_FEATURE_ZOOM__
		switch(g_vdorec_cntx.setting.zoom)
		{
			case 0: 
				return VDOREC_FEATURE_ZOOM_0;
			case 1: 
				return VDOREC_FEATURE_ZOOM_1;
			case 2: 
				return VDOREC_FEATURE_ZOOM_2;
			case 3: 
				return VDOREC_FEATURE_ZOOM_3;
			case 4: 
				return VDOREC_FEATURE_ZOOM_4;
			case 5: 
				return VDOREC_FEATURE_ZOOM_5;		
			case 6: 
				return VDOREC_FEATURE_ZOOM_6;	
			case 7: 
				return VDOREC_FEATURE_ZOOM_7;	
			case 8: 
				return VDOREC_FEATURE_ZOOM_8;	
			case 9: 
				return VDOREC_FEATURE_ZOOM_9;	
			case 10: 
				return VDOREC_FEATURE_ZOOM_10;	
			case 11: 
				return VDOREC_FEATURE_ZOOM_11;	
			case 12: 
				return VDOREC_FEATURE_ZOOM_12;	
			case 13: 
				return VDOREC_FEATURE_ZOOM_13;	
			case 14: 
				return VDOREC_FEATURE_ZOOM_14;	
			case 15: 
				return VDOREC_FEATURE_ZOOM_15;	
			default:
				ASSERT(0);
				return 0;
		}
	#else
		return 0;
	#endif /* __VDOREC_FEATURE_ZOOM__ */
	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_lsk_press_hdlr
* DESCRIPTION
*  lsk press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/

static void mmi_vdorec_lsk_press_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdorec_cntx.is_lsk_pressed = TRUE;
    lcd_lock_all_blt_operation();
	mmi_vdorec_draw_softkey();
	mmi_vdorec_draw_panel();
	mmi_vdorec_draw_panel_state();
    lcd_unlock_all_blt_operation();
	mmi_vdorec_blt_screen();
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_lsk_release_hdlr
* DESCRIPTION
*  lsk release hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_lsk_release_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdorec_cntx.is_lsk_pressed = FALSE;
//	mmi_vdorec_draw_softkey();
//	mmi_vdorec_blt_screen();	

	mmi_trace(g_sw_VdoRec," mmi_vdorec_lsk_release_hdlr g_vdorec_cntx.state = %d",g_vdorec_cntx.state);
	#if 0//def __VDOREC_FEATURE_TOUCH_SCREEN__
		mmi_vdorec_entry_option_screen();
	#else
		switch(g_vdorec_cntx.state)
		{
			case VDOREC_STATE_PREVIEW:
				#ifdef OPTION_ENABLED
				mmi_vdorec_entry_option_screen(); // delete option function 11-13 by zhangl
				#endif
				break;

			case VDOREC_STATE_RECORD:
				mmi_vdorec_enter_state(VDOREC_STATE_PAUSE);
				break;

			case VDOREC_STATE_PAUSE:
				mmi_vdorec_enter_state(VDOREC_STATE_RECORD);
				break;
		}	
	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */
		
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_rsk_press_hdlr
* DESCRIPTION
*  rsk press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_rsk_press_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdorec_cntx.is_rsk_pressed = TRUE;
    lcd_lock_all_blt_operation();
	mmi_vdorec_draw_softkey();
	mmi_vdorec_draw_panel();
	mmi_vdorec_draw_panel_state();
    lcd_unlock_all_blt_operation();
	mmi_vdorec_blt_screen();		
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_rsk_release_hdlr
* DESCRIPTION
*  rsk release hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_rsk_release_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	mmi_trace(g_sw_VdoRec," mmi_vdorec_rsk_release_hdlr g_vdorec_cntx.state = %d",g_vdorec_cntx.state);
	g_vdorec_cntx.is_rsk_pressed = FALSE;
#if defined(__PROJECT_GALLITE_C01__)
	mmi_vdorec_draw_softkey();
	mmi_vdorec_blt_screen();	
#endif
	#if 0//def __VDOREC_FEATURE_TOUCH_SCREEN__
		GoBackHistory();
	#else
		switch(g_vdorec_cntx.state)
		{
			case VDOREC_STATE_PREVIEW:
          //mdi_video_rec_record_previewStop();//chenhe add
				#ifdef __MMI_VIDEO_23KEY__
					mmi_vdorec_record_button_press_hdlr();
				#else			
					GoBackHistory();
				#endif /* __MMI_VIDEO_23KEY__ */

				break;

			case VDOREC_STATE_RECORD:
			case VDOREC_STATE_PAUSE:				
				mdi_video_rec_record_stop();
				coolsand_UI_cancel_timer(mmi_vdorec_record_timer_cyclic);
  //      g_vdorec_cntx.state = VDOREC_STATE_PREVIEW;//chenhe for jasperII
				mmi_vdorec_entry_saving_screen();//chenhe for jasperII
				break;

			default:
					GoBackHistory();
					break;				
		}
	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */
	
}


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_inc_ev_key_press_hdlr
* DESCRIPTION
*  right arrow press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_EV__
static void mmi_vdorec_inc_ev_key_press_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#ifdef __VDOREC_FEATURE_RECORDING_EV__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_EV__ */


	g_vdorec_cntx.is_ev_inc_key_pressed = TRUE;
	
	if(g_vdorec_cntx.setting.ev < VDOREC_SETTING_EV_COUNT-1)
	{
		g_vdorec_cntx.setting.ev++;

		mdi_video_rec_update_para_ev(vdorec_ev_command_map[g_vdorec_cntx.setting.ev]);
        
        lcd_lock_all_blt_operation();
		mmi_vdorec_draw_panel_ev();
        lcd_unlock_all_blt_operation();

		mmi_vdorec_blt_screen();	

	}
}
#endif /* __VDOREC_FEATURE_EV__ */




/*****************************************************************************
* FUNCTION
*	mmi_vdorec_inc_ev_key_release_hdlr
* DESCRIPTION
*  right arrow release hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_EV__
static void mmi_vdorec_inc_ev_key_release_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#ifdef __VDOREC_FEATURE_RECORDING_EV__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_EV__ */

	g_vdorec_cntx.is_ev_inc_key_pressed = FALSE;
    lcd_lock_all_blt_operation();
	mmi_vdorec_draw_panel_ev();
    lcd_unlock_all_blt_operation();
	mmi_vdorec_blt_screen();	
}
#endif /* __VDOREC_FEATURE_EV__ */




/*****************************************************************************
* FUNCTION
*	mmi_vdorec_dec_ev_key_press_hdlr
* DESCRIPTION
*  left arrow press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_EV__
static void mmi_vdorec_dec_ev_key_press_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#ifdef __VDOREC_FEATURE_RECORDING_EV__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_EV__ */

	g_vdorec_cntx.is_ev_dec_key_pressed = TRUE;

	if(g_vdorec_cntx.setting.ev > 0)
	{
		g_vdorec_cntx.setting.ev--;

		mdi_video_rec_update_para_ev(vdorec_ev_command_map[g_vdorec_cntx.setting.ev]);
        lcd_lock_all_blt_operation();
		mmi_vdorec_draw_panel_ev();
        lcd_unlock_all_blt_operation();
		mmi_vdorec_blt_screen();	
	}
}
#endif /* __VDOREC_FEATURE_EV__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_dec_ev_key_release_hdlr
* DESCRIPTION
*  left arrow release hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_EV__
static void mmi_vdorec_dec_ev_key_release_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#ifdef __VDOREC_FEATURE_RECORDING_EV__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_EV__ */

	g_vdorec_cntx.is_ev_dec_key_pressed = FALSE;
    lcd_lock_all_blt_operation();
	mmi_vdorec_draw_panel_ev();
    lcd_unlock_all_blt_operation();
	mmi_vdorec_blt_screen();	
}
#endif /* __VDOREC_FEATURE_EV__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_inc_bt_key_press_hdlr
* DESCRIPTION
*  right arrow press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_BT__
static void mmi_vdorec_inc_bt_key_press_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOREC_FEATURE_RECORDING_EV__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#else
		/* not in preview state */
		if(g_vdorec_cntx.state != VDOREC_STATE_PREVIEW)
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_EV__ */

	g_vdorec_cntx.is_bt_inc_key_pressed = TRUE;
	
	if(g_vdorec_cntx.setting.brightness< VDOREC_SETTING_BT_COUNT-1)
	{
		g_vdorec_cntx.setting.brightness++;

		mdi_video_rec_update_para_ev(vdorec_bt_command_map[g_vdorec_cntx.setting.brightness]);
		
		mmi_vdorec_draw_panel_ev();
	
		mmi_vdorec_blt_screen();	

	}
}
#endif /* __VDOREC_FEATURE_BT__ */




/*****************************************************************************
* FUNCTION
*	mmi_vdorec_inc_ev_key_release_hdlr
* DESCRIPTION
*  right arrow release hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_BT__
static void mmi_vdorec_inc_bt_key_release_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOREC_FEATURE_RECORDING_EV__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#else
		/* not in preview state */
		if(g_vdorec_cntx.state != VDOREC_STATE_PREVIEW)
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_EV__ */

	g_vdorec_cntx.is_ev_inc_key_pressed = FALSE;
	mmi_vdorec_draw_panel_ev();
	mmi_vdorec_blt_screen();	
}
#endif /* __VDOREC_FEATURE_BT__ */




/*****************************************************************************
* FUNCTION
*	mmi_vdorec_dec_ev_key_press_hdlr
* DESCRIPTION
*  left arrow press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_BT__
static void mmi_vdorec_dec_bt_key_press_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOREC_FEATURE_RECORDING_EV__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#else
		/* not in preview state */
		if(g_vdorec_cntx.state != VDOREC_STATE_PREVIEW)
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_EV__ */

	g_vdorec_cntx.is_ev_dec_key_pressed = TRUE;

	if(g_vdorec_cntx.setting.ev > 0)
	{
		g_vdorec_cntx.setting.ev--;

		mdi_video_rec_update_para_ev(vdorec_ev_command_map[g_vdorec_cntx.setting.ev]);
		
		mmi_vdorec_draw_panel_ev();
	
		mmi_vdorec_blt_screen();	
	}
}
#endif /* __VDOREC_FEATURE_BT__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_dec_ev_key_release_hdlr
* DESCRIPTION
*  left arrow release hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_BT__
static void mmi_vdorec_dec_bt_key_release_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOREC_FEATURE_RECORDING_EV__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#else
		/* not in preview state */
		if(g_vdorec_cntx.state != VDOREC_STATE_PREVIEW)
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_EV__ */

	g_vdorec_cntx.is_ev_dec_key_pressed = FALSE;

	mmi_vdorec_draw_panel_ev();
	mmi_vdorec_blt_screen();	
}
#endif /* __VDOREC_FEATURE_BT__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_zoom_in
* DESCRIPTION
*  zoom in
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_ZOOM__
static void mmi_vdorec_zoom_in(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __VDOREC_FEATURE_RECORDING_ZOOM__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
#else
		/* not in preview state */
		if(g_vdorec_cntx.state != VDOREC_STATE_PREVIEW)
		{
			return;
		}
#endif /* __VDOREC_FEATURE_RECORDING_ZOOM__ */

	if(g_vdorec_cntx.setting.zoom < VDOREC_FEATURE_ZOOM_COUNT-1)
	{
		g_vdorec_cntx.setting.zoom += 1;
		
		mdi_video_rec_update_para_zoom(mmi_vdorec_get_zoom_factor());
        lcd_lock_all_blt_operation();
		mmi_vdorec_draw_panel_zoom();
        lcd_unlock_all_blt_operation();

		mmi_vdorec_blt_screen();	


		#if defined(ISP_SUPPORT)
			/* only build-in camera use long pressed zooming feature */
			/* since backend module's zooming time is not stable, will cause zooming jump two level */
			if(g_vdorec_cntx.is_first_time_zoom)
			{
				/* first time zoom, will wait longer to zoom again */
				coolsand_UI_start_timer(300, mmi_vdorec_zoom_in);
				g_vdorec_cntx.is_first_time_zoom = FALSE;
			}
			else
			{
				coolsand_UI_start_timer(100, mmi_vdorec_zoom_in);
			}
		#endif /* ISP_SUPPORT */
			
	}

	
}
#endif /* __VDOREC_FEATURE_ZOOM__ */ 


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_zoom_in_key_press_hdlr
* DESCRIPTION
*  up arrow press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_ZOOM__
static void mmi_vdorec_zoom_in_key_press_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   
	if(!g_vdorec_cntx.is_zoom_in_key_pressed)
	{
	   g_vdorec_cntx.is_zoom_in_key_pressed 	= TRUE;
		g_vdorec_cntx.is_first_time_zoom 		= TRUE;
		mmi_vdorec_zoom_in();
	}


}
#endif /* __VDOREC_FEATURE_ZOOM__ */ 


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_zoom_in_key_release_hdlr
* DESCRIPTION
*  up arrow release hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_ZOOM__
static void mmi_vdorec_zoom_in_key_release_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdorec_cntx.is_zoom_in_key_pressed = FALSE;
	coolsand_UI_cancel_timer(mmi_vdorec_zoom_in);   
    lcd_lock_all_blt_operation();
	mmi_vdorec_draw_panel_zoom();
    lcd_unlock_all_blt_operation();
	mmi_vdorec_blt_screen();	
}
#endif /* __VDOREC_FEATURE_ZOOM__ */ 


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_zoom_out
* DESCRIPTION
*  zoom in
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_ZOOM__
static void mmi_vdorec_zoom_out(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __VDOREC_FEATURE_RECORDING_ZOOM__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
#else
		/* not in preview state */
		if(g_vdorec_cntx.state != VDOREC_STATE_PREVIEW)
		{
			return;
		}
#endif /* __VDOREC_FEATURE_RECORDING_ZOOM__ */

	if(g_vdorec_cntx.setting.zoom > 0)
	{
		g_vdorec_cntx.setting.zoom -= 1;
		
		mdi_video_rec_update_para_zoom(mmi_vdorec_get_zoom_factor());
        lcd_lock_all_blt_operation();
		mmi_vdorec_draw_panel_zoom();
        lcd_unlock_all_blt_operation();

		mmi_vdorec_blt_screen();	

		#if defined(ISP_SUPPORT)
			/* only build-in camera use long pressed zooming feature */
			/* since backend module's zooming time is not stable, will cause zooming jump two level */
			if(g_vdorec_cntx.is_first_time_zoom)
			{
				/* first time zoom, will wait longer to zoom again */
				coolsand_UI_start_timer(300, mmi_vdorec_zoom_out);
			}
			else
			{
				coolsand_UI_start_timer(100, mmi_vdorec_zoom_out);
			}
		#endif /* ISP_SUPPORT */
	}

}
#endif /* __VDOREC_FEATURE_ZOOM__ */ 



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_zoom_out_key_press_hdlr
* DESCRIPTION
*  down arrow press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_ZOOM__
static void mmi_vdorec_zoom_out_key_press_hdlr(void)
{	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   
   if(!g_vdorec_cntx.is_zoom_out_key_pressed)
   {
  	   g_vdorec_cntx.is_zoom_out_key_pressed 	= TRUE;
		g_vdorec_cntx.is_first_time_zoom 		= TRUE;
		
		mmi_vdorec_zoom_out();
   }
}
#endif /* __VDOREC_FEATURE_ZOOM__ */ 


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_zoom_out_key_release_hdlr
* DESCRIPTION
*  up arrow release hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_ZOOM__
static void mmi_vdorec_zoom_out_key_release_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdorec_cntx.is_zoom_out_key_pressed = FALSE;
	coolsand_UI_cancel_timer(mmi_vdorec_zoom_out);
    lcd_lock_all_blt_operation();
	mmi_vdorec_draw_panel_zoom();
    lcd_unlock_all_blt_operation();
	mmi_vdorec_blt_screen();	
}
#endif /* __VDOREC_FEATURE_ZOOM__ */ 


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_record_button_press_hdlr
* DESCRIPTION
*  record button press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_record_button_keyup_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdorec_cntx.state == VDOREC_STATE_RECORD)
	{
		COS_Sleep(200);
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_record_button_press_hdlr
* DESCRIPTION
*  record button press hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_record_button_press_hdlr(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if( (g_vdorec_cntx.state == VDOREC_STATE_PREVIEW) || 
		 (g_vdorec_cntx.state == VDOREC_STATE_PAUSE) )
	{
		mmi_vdorec_enter_state(VDOREC_STATE_RECORD);
	}
	else if(g_vdorec_cntx.state == VDOREC_STATE_RECORD)
	{
	#if 0	/* stop record */
		mdi_video_rec_record_stop();
		coolsand_UI_cancel_timer(mmi_vdorec_record_timer_cyclic);
		
		mmi_vdorec_entry_saving_screen();
		return;
	 #endif	
	 	mmi_vdorec_enter_state(VDOREC_STATE_PAUSE);
	}
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_turn_on_led_highlight
* DESCRIPTION
*  turn on led highlight
* PARAMETERS
*  nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_turn_on_led_highlight(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT__

		duty = g_vdorec_cntx.setting.led_value;

	#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT_ON_OFF__
		if(g_vdorec_cntx.setting.led_highlight)
		{
			custom_start_flashlight(1,1,1, duty);
		}
	#else
		/* __VDOREC_FEATURE_LED_HIGHLIGHT_7_COLOR__ */
		switch(g_vdorec_cntx.setting.led_highlight)
		{
			case VDOREC_SETTING_LED_HIGHLIGHT_WHITE:
				custom_start_flashlight(1,1,1, duty);
				break;
				
			case 	VDOREC_SETTING_LED_HIGHLIGHT_RED:
				custom_start_flashlight(1,0,0, duty);	
				break;

			case 	VDOREC_SETTING_LED_HIGHLIGHT_GREEN:
				custom_start_flashlight(0,1,0, duty);	
				break;		


			case 	VDOREC_SETTING_LED_HIGHLIGHT_BLUE:
				custom_start_flashlight(0,0,1, duty);	
				break;			

			case 	VDOREC_SETTING_LED_HIGHLIGHT_YELLOW:
				custom_start_flashlight(1,1,0, duty);	
				break;	

			case 	VDOREC_SETTING_LED_HIGHLIGHT_PURPLE:
				custom_start_flashlight(1,0,1, duty);	
				break;	

			case 	VDOREC_SETTING_LED_HIGHLIGHT_CYAN:
				custom_start_flashlight(0,1,1, duty);	
				break;	
		}
	#endif /* __VDOREC_FEATURE_LED_HIGHLIGHT_ON_OFF__ */

	
	#endif /* __VDOREC_FEATURE_LED_HIGHLIGHT__ */

}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_turn_off_led_highlight
* DESCRIPTION
*  turn off led highlight
* PARAMETERS
*  nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_turn_off_led_highlight(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT__
		custom_stop_flashlight();
	#endif /* __VDOREC_FEATURE_LED_HIGHLIGHT__ */
}


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_record_result_hdlr_callback
* DESCRIPTION
*  record result callback hdlr
* PARAMETERS
*  a  IN		current state
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_record_result_hdlr_callback(MDI_RESULT result)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ASSERT((g_vdorec_cntx.state == VDOREC_STATE_RECORD) || (g_vdorec_cntx.state == VDOREC_STATE_PAUSE)); 

	coolsand_UI_cancel_timer(mmi_vdorec_record_timer_cyclic);

	if(result == MDI_RES_VDOREC_DISK_FULL)
	{
		g_vdorec_cntx.is_force_saving = TRUE;
		mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_INFO, 1, ST_NOTIFYDURATION, WARNING_TONE);
		return;
	}
	else if(result == MDI_RES_VDOREC_REACH_SIZE_LIMIT)
	{
		// bao debug: g_vdorec_cntx.is_force_saving = TRUE;
		mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOREC_NOTIFY_REACH_SIZE_LIMIT), IMG_GLOBAL_INFO, 1, ST_NOTIFYDURATION, WARNING_TONE);		
		return;
	}
	else if(result == MDI_RES_VDOREC_ERR_FILE_SIZE)
		{
		    mmi_trace(1,"file too small and delete it---zhangl");
		    vdorec_delete_RecFile();
		    return;
		}
	else if(result == MDI_RES_VDOREC_ERR_FAILED)
	{
		mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOREC_NOTIFY_ERROR), IMG_GLOBAL_INFO, 1, ST_NOTIFYDURATION, WARNING_TONE);		
		return;
	}
	
	mmi_vdorec_entry_saving_screen();
}



/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_preview_effect_setting_hotkey_press()
* DESCRIPTION
*   effect setting hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

#ifdef __VDOREC_FEATURE_EFFECT__
static void mmi_vdorec_effect_setting_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 str_buf[256];
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#ifdef __VDOREC_FEATURE_RECORDING_EFFECT__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_EFFECT__ */	

	if(g_vdorec_cntx.setting.effect < VDOREC_SETTING_EFFECT_COUNT-1)
		g_vdorec_cntx.setting.effect++;
	else
		g_vdorec_cntx.setting.effect = 0;

	pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
	pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_VDOREC_EFFECT_SETTING));
	pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");

	/* update osd */
	mmi_vdorec_set_hint(	(PS8)str_buf,
								(PS8)GetString((U16)(g_vdorec_cntx.setting.effect + STR_ID_VDOREC_EFFECT_START+1)),
								HINT_POPUP_FADE_TIME);

	mmi_vdorec_draw_osd();

	mdi_video_rec_update_para_effect(vdorec_effect_command_map[g_vdorec_cntx.setting.effect]);
		
//	mmi_vdorec_blt_screen();	
	mmi_vdorec_blt_hint_screen();
}
#endif /* __VDOREC_FEATURE_EFFECT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_wb_hotkey_press()
* DESCRIPTION
*   wb hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

static void mmi_vdorec_wb_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 str_buf[256];


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	#ifdef __VDOREC_FEATURE_RECORDING_WB__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_WB__ */
	
	if(g_vdorec_cntx.setting.wb < VDOREC_SETTING_WB_COUNT-1)
		g_vdorec_cntx.setting.wb++;
	else
		g_vdorec_cntx.setting.wb = 0;

	pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
	pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_VDOREC_WB));
	pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");

	/* update osd */
	mmi_vdorec_set_hint(	(PS8)str_buf, 
								(PS8)GetString((U16)(g_vdorec_cntx.setting.wb + STR_ID_VDOREC_WB_START+1)),
								HINT_POPUP_FADE_TIME);

	mmi_vdorec_draw_osd();
	
	mdi_video_rec_update_para_wb(vdorec_wb_command_map[g_vdorec_cntx.setting.wb]);

//	mmi_vdorec_blt_screen();
	mmi_vdorec_blt_hint_screen();
}

#ifdef __VDOREC_FEATURE_CT__
static void mmi_vdorec_ct_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	#ifdef __VDOREC_FEATURE_RECORDING_CT__
		/* not in preview & record state */
		if((g_vdorec_cntx.state != VDOREC_STATE_PREVIEW) && 
			(g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			(g_vdorec_cntx.state != VDOREC_STATE_PAUSE))
		{
			return;
		}
	#endif /* __VDOREC_FEATURE_RECORDING_CT__ */
	
   
	if(g_vdorec_cntx.setting.contrast< VDOREC_SETTING_CONTRAST_COUNT-1)
		g_vdorec_cntx.setting.contrast++;
	else
		g_vdorec_cntx.setting.contrast = 0;


	pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
	pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_VDOREC_CONTRAST));
	pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");

	/* draw osd */
	mmi_vdorec_set_hint(	(PS8)str_buf, 
								(PS8)GetString((U16)(g_vdorec_cntx.setting.contrast+ STR_ID_VDOREC_CONTRAST_START+1)),
								HINT_POPUP_FADE_TIME);

	mmi_vdorec_draw_osd();
	
	mdi_video_rec_update_para_ct(vdorec_ct_command_map[g_vdorec_cntx.setting.contrast]);
	
//	mmi_vdorec_blt_screen();	
	mmi_vdorec_blt_hint_screen();
}
#endif /* __VDOREC_FEATURE_CT__ */

/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_led_highlight_hotkey_press()
* DESCRIPTION
*   led highlight hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT__
static void mmi_vdorec_led_highlight_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 str_buf[256];


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_vdorec_cntx.setting.led_highlight < VDOREC_SETTING_LED_HIGHLIGHT_COUNT-1)
		g_vdorec_cntx.setting.led_highlight++;
	else
		g_vdorec_cntx.setting.led_highlight = 0;
	
	pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
	pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_VDOREC_LED_HIGHLIGHT));
	pfnUnicodeStrcat((PS8)str_buf, (PS8)L" ]");
	
	/* update osd */	
	mmi_vdorec_set_hint(	(PS8)str_buf, 
								(PS8)GetString((U16)(g_vdorec_cntx.setting.led_highlight + STR_ID_VDOREC_LED_HIGHLIGHT_START+1)),
								HINT_POPUP_FADE_TIME);

	mmi_vdorec_draw_osd();

	/* turn on/of led */
	if(g_vdorec_cntx.setting.led_highlight == VDOREC_SETTING_LED_HIGHLIGHT_OFF)
		mmi_vdorec_turn_off_led_highlight();
	else
		mmi_vdorec_turn_on_led_highlight();
	
	mmi_vdorec_turn_on_led_highlight();
	
	mmi_vdorec_blt_screen();	
}
#endif


/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_night_hotkey_press()
* DESCRIPTION
*   night hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef SHORTCUT_DEFINED
#ifdef __VDOREC_FEATURE_NIGHT__
static void mmi_vdorec_night_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdorec_cntx.setting.night < VDOREC_SETTING_NIGHT_COUNT-1)
		g_vdorec_cntx.setting.night++;
	else
		g_vdorec_cntx.setting.night = 0;

	pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
	pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_VDOREC_NIGHT));
	pfnUnicodeStrcat((PS8)str_buf, (PS8)L" ]");

	/* draw osd */
	mmi_vdorec_set_hint(	(PS8)str_buf, 
								(PS8)GetString((U16)(g_vdorec_cntx.setting.night + STR_ID_VDOREC_NIGHT_START+1)),
								HINT_POPUP_FADE_TIME);

	mmi_vdorec_draw_osd();
	
	mdi_video_rec_update_para_night(g_vdorec_cntx.setting.night);
	
	mmi_vdorec_blt_screen();	
}
#endif /* __VDOREC_FEATURE_NIGHT__ */

#endif
/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_start_preview()
* DESCRIPTION
*   Start preview
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_vdorec_start_preview(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	mdi_video_setting_struct	video_preview_data;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
       mmi_trace(1,"setting mmi_vdorec_start_preview wb=%d,ev=%d,",g_vdorec_cntx.setting.wb,g_vdorec_cntx.setting.ev);
       mmi_trace(1,"setting mmi_vdorec_start_preview effect=%d,zoom=%d,",g_vdorec_cntx.setting.effect,mmi_vdorec_get_zoom_factor());
       mmi_trace(1,"setting mmi_vdorec_start_preview night=%d,",g_vdorec_cntx.setting.night);
	mmi_trace(1,"setting mmi_vdorec_start_preview video_qty=%d,",g_vdorec_cntx.setting.video_qty);
	
	video_preview_data.wb 				= vdorec_wb_command_map[g_vdorec_cntx.setting.wb];
	video_preview_data.ev 				= vdorec_ev_command_map[g_vdorec_cntx.setting.ev];
	video_preview_data.banding 		= vdorec_banding_command_map[g_vdorec_cntx.setting.banding];			
	video_preview_data.video_size		= vdorec_video_size_command_map[g_vdorec_cntx.setting.video_size];	
	video_preview_data.video_qty 		= vdorec_video_qty_command_map[g_vdorec_cntx.setting.video_qty];
	video_preview_data.video_format 	=  vdorec_video_format_command_map[g_vdorec_cntx.setting.video_format];
	video_preview_data.effect 			= vdorec_effect_command_map[g_vdorec_cntx.setting.effect];

	video_preview_data.zoom 	 		= mmi_vdorec_get_zoom_factor();
	video_preview_data.night			= g_vdorec_cntx.setting.night;	
//	video_preview_data.zoom 	 		= 0;// need to modify and this is just for testing
	video_preview_data.brightness  	= 2;//128;
	video_preview_data.contrast 	 	= 2;//128;
	video_preview_data.saturation	 	= 128;			
	video_preview_data.hue 	 			= 0;
	video_preview_data.rotate 	 		= VDOREC_MAINLCD_ROTATE;

       mmi_trace(1,"setting 11mmi_vdorec_start_preview wb=%d,ev=%d,",video_preview_data.wb ,video_preview_data.ev );
       mmi_trace(1,"setting 11mmi_vdorec_start_preview effect=%d,zoom=%d,",video_preview_data.effect ,mmi_vdorec_get_zoom_factor());
       mmi_trace(1,"setting 11mmi_vdorec_start_preview night=%d,",g_vdorec_cntx.setting.night);

	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		video_preview_data.lcd_rotate = MDI_VIDEO_LCD_ROTATE_270;	
	#else
		video_preview_data.lcd_rotate = MDI_VIDEO_LCD_ROTATE_0;	
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
	
	#ifdef __VDOREC_FEATURE_SIZE_LIMIT__
		switch(g_vdorec_cntx.setting.size_limit)
		{
		
			case VDOREC_SETTING_SIZE_LIMIT_1:
				video_preview_data.size_limit	= VDOREC_FEATURE_SIZE_LIMIT_1;
				break;

			case VDOREC_SETTING_SIZE_LIMIT_2:
				video_preview_data.size_limit	= VDOREC_FEATURE_SIZE_LIMIT_2;
				break;

			case VDOREC_SETTING_SIZE_LIMIT_3:
				video_preview_data.size_limit	= VDOREC_FEATURE_SIZE_LIMIT_3;
				break;

			default:
				video_preview_data.size_limit = 0;
				break;
		}
	#else
		/* no size limit */
		video_preview_data.size_limit = 0;
	#endif /* __VDOREC_FEATURE_SIZE_LIMIT__ */
		
	#ifdef __VDOREC_FEATURE_TIME_LIMIT__
		switch(g_vdorec_cntx.setting.time_limit)
		{
		
			case VDOREC_SETTING_TIME_LIMIT_1:
				video_preview_data.time_limit	= VDOREC_FEATURE_TIME_LIMIT_1;
				break;

			case VDOREC_SETTING_TIME_LIMIT_2:
				video_preview_data.time_limit	= VDOREC_FEATURE_TIME_LIMIT_2;
				break;

			case VDOREC_SETTING_TIME_LIMIT_3:
				video_preview_data.time_limit	= VDOREC_FEATURE_TIME_LIMIT_3;
				break;

			default:
				video_preview_data.time_limit = 0;
				break;
		}
	#else
		/* no time limit */
		video_preview_data.time_limit = 0;
	#endif /* __VDOREC_FEATURE_TIME_LIMIT__ */


	#ifdef __VDOREC_FEATURE_RECORD_AUD__
		switch(g_vdorec_cntx.setting.record_aud)
		{
			case VDOREC_SETTING_RECORD_AUD_OFF:
				video_preview_data.record_aud = FALSE;
				break;

			case VDOREC_SETTING_RECORD_AUD_ON:
				video_preview_data.record_aud = TRUE;		
				break;
		}
	#else
		video_preview_data.record_aud = TRUE;
	#endif /* __VDOREC_FEATURE_RECORD_AUD__ */

	mmi_trace(g_sw_VdoRec," mmi_vdorec_start_preview");
	g_vdorec_cntx.last_error = 
			mdi_video_rec_preview_start(	g_vdorec_cntx.preview_layer_handle,
													GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2,
													GDI_LAYER_ENABLE_LAYER_1,
 													&video_preview_data);

}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_enter_state
* DESCRIPTION
*  enter state 
* PARAMETERS
*	a 	IN  	next state to be entered
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
//extern UINT32 MCI_CamPowerUp (INT32  vediomode);
//extern void set_mmc_camera_preview();

static void mmi_vdorec_enter_state(U16 state)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	prev_state;
	FS_HANDLE vdorec_filehandle;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_trace(1," mmi_vdorec_enter_state,state is %d",state);
   
	/* flush key event */
	ClearKeyEvents();

	SetKeyHandler(mmi_vdorec_lsk_press_hdlr, 				KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_vdorec_lsk_release_hdlr,			KEY_LSK, KEY_EVENT_UP);		

	SetKeyHandler(mmi_vdorec_rsk_press_hdlr, 				KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_vdorec_rsk_release_hdlr,			KEY_RSK, KEY_EVENT_UP);	
	
//#ifdef SHORTCUT_DEFINED/*delete shortcut by zhangl 11-17*/

	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		SetKeyHandler(mmi_vdorec_inc_ev_key_press_hdlr,		KEY_DOWN_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_inc_ev_key_release_hdlr,	KEY_DOWN_ARROW,	KEY_EVENT_UP);
		SetKeyHandler(mmi_vdorec_dec_ev_key_press_hdlr,		KEY_UP_ARROW,		KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_dec_ev_key_release_hdlr,	KEY_UP_ARROW,		KEY_EVENT_UP);

		SetKeyHandler(mmi_vdorec_zoom_out_key_press_hdlr,		KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_zoom_out_key_release_hdlr,	KEY_RIGHT_ARROW,	KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdorec_zoom_in_key_press_hdlr,		KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_zoom_in_key_release_hdlr,		KEY_LEFT_ARROW,	KEY_EVENT_UP);	
	#else	/* normal view */
/*	
		SetKeyHandler(mmi_vdorec_inc_ev_key_press_hdlr,		KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_inc_ev_key_release_hdlr,	KEY_RIGHT_ARROW,	KEY_EVENT_UP);
		SetKeyHandler(mmi_vdorec_dec_ev_key_press_hdlr,		KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_dec_ev_key_release_hdlr,	KEY_LEFT_ARROW,	KEY_EVENT_UP);

		SetKeyHandler(mmi_vdorec_zoom_out_key_press_hdlr,		KEY_DOWN_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_zoom_out_key_release_hdlr,	KEY_DOWN_ARROW,	KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdorec_zoom_in_key_press_hdlr,		KEY_UP_ARROW,		KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_zoom_in_key_release_hdlr,		KEY_UP_ARROW,		KEY_EVENT_UP);	
*/
		/****new key definition for greenstone, which will be according with definition of camera *******/
		SetKeyHandler(mmi_vdorec_inc_ev_key_press_hdlr,		KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_inc_ev_key_release_hdlr,	KEY_RIGHT_ARROW,	KEY_EVENT_UP);
		SetKeyHandler(mmi_vdorec_dec_ev_key_press_hdlr,		KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_dec_ev_key_release_hdlr,	KEY_LEFT_ARROW,	KEY_EVENT_UP);
		
		SetKeyHandler(mmi_vdorec_zoom_out_key_press_hdlr,		KEY_DOWN_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_zoom_out_key_release_hdlr,	KEY_DOWN_ARROW,	KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdorec_zoom_in_key_press_hdlr,		KEY_UP_ARROW,		KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_zoom_in_key_release_hdlr,		KEY_UP_ARROW,		KEY_EVENT_UP);	
	
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

//#endif

	#ifndef __MMI_VIDEO_23KEY__
		/* have dedicate record key */
		SetKeyHandler(mmi_vdorec_record_button_keyup_hdlr,	VDOREC_RECORD_KEY, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_vdorec_record_button_press_hdlr,	VDOREC_RECORD_KEY, KEY_EVENT_UP);

	
		SetKeyHandler(mmi_vdorec_record_button_keyup_hdlr,	KEY_5, KEY_EVENT_DOWN);//chenhe,test for jasperII
		SetKeyHandler(mmi_vdorec_record_button_press_hdlr,	KEY_5, KEY_EVENT_UP);//chenhe,test for jasperII
		if(VDOREC_RECORD_EXTRA_KEY != -1)
		{
			SetKeyHandler(mmi_vdorec_record_button_press_hdlr,	VDOREC_RECORD_EXTRA_KEY, KEY_EVENT_DOWN);	
		}
	#endif /* __MMI_VDOREC_23KEY__ */



	/* hot keys */
	#ifdef __VDOREC_FEATURE_EFFECT__
		if(VDOREC_EFFECT_KEY != -1)
			SetKeyHandler(mmi_vdorec_effect_setting_hotkey_press, VDOREC_EFFECT_KEY, KEY_EVENT_DOWN);
	#endif /* __VDOREC_FEATURE_EFFECT__ */
	


	#ifdef __VDOREC_FEATURE_WB__
		if(VDOREC_WB_KEY != -1)
			SetKeyHandler(mmi_vdorec_wb_hotkey_press, VDOREC_WB_KEY, KEY_EVENT_DOWN);
	#endif /* __VDOREC_FEATURE_WB__ */


	#ifdef __VDOREC_FEATURE_CT__ 
		if(VDOREC_CT_KEY != -1)
			SetKeyHandler(mmi_vdorec_ct_hotkey_press, VDOREC_CT_KEY, KEY_EVENT_DOWN);	
	#endif /* __VDOREC_FEATURE_NIGHT__ */


#ifdef SHORTCUT_DEFINED/*delete shortcut by zhangl 11-17*/
	#ifdef __VDOREC_FEATURE_NIGHT__ 
		if(VDOREC_NIGHT_KEY != -1)
			SetKeyHandler(mmi_vdorec_night_hotkey_press, VDOREC_NIGHT_KEY, KEY_EVENT_DOWN);	
	#endif /* __VDOREC_FEATURE_NIGHT__ */

	#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT__ 
		if(VDOREC_LED_HIGHLIGHT_KEY != -1)
			SetKeyHandler(mmi_vdorec_led_highlight_hotkey_press, VDOREC_LED_HIGHLIGHT_KEY, KEY_EVENT_DOWN);		
	#endif
	
#endif
	
	switch(state)
	{
		case VDOREC_STATE_PREVIEW:
		{
			prev_state 				= g_vdorec_cntx.state;
			g_vdorec_cntx.state 	= state;

			#ifdef __MMI_VIDEO_23KEY__
				mmi_vdorec_set_softkey(	(PS8)GetString(STR_GLOBAL_OPTIONS), 
												(PS8)GetString(STR_ID_VDOREC_RSK_RECORD),
												IMG_ID_VDOREC_LSK_OPTION,
												IMG_ID_VDOREC_RSK_RECORD);

			#else
				#ifdef OPTION_ENABLED
				/*	delete lsk menu*/
				mmi_vdorec_set_softkey(	(PS8)GetString(STR_GLOBAL_OPTIONS), 
												(PS8)GetString(STR_GLOBAL_BACK),
												IMG_ID_VDOREC_LSK_OPTION,
												IMG_ID_VDOREC_RSK_BACK);
				#else
			      mmi_vdorec_set_softkey(	NULL, 
												(PS8)GetString(STR_GLOBAL_BACK),
												0,
												IMG_ID_VDOREC_RSK_BACK);
				#endif
			#endif /* __MMI_VIDEO_23KEY__ */

            lcd_lock_all_blt_operation();
			mmi_vdorec_draw_softkey();
//			mmi_vdorec_draw_panel_state();
					
			mmi_vdorec_draw_panel();
            lcd_unlock_all_blt_operation();
             mmi_vdorec_blt_full_screen();

			mmi_trace(g_sw_VdoRec," VDOREC_STATE_PREVIEW mmi_vdorec_start_preview()");

			mmi_vdorec_start_preview();
							
#ifndef __MMI_DISABLE_DUAL_SENSOR__
                SetKeyHandler( mmi_camera_preview_switch_sensor_hotkey_press, KEY_0 , KEY_EVENT_DOWN);
#endif
			mmi_trace(g_sw_VdoRec,"chenhe,enter preview state,g_vdorec_cntx.last_error is %d",g_vdorec_cntx.last_error);
			mmi_vdorec_check_and_display_error_popup();
		}
		break;

		case VDOREC_STATE_RECORD:
		{
						
			prev_state = g_vdorec_cntx.state;
			g_vdorec_cntx.state = state;	

			#if defined(__VDOREC_FEATURE_TOUCH_SCREEN__)
				mmi_vdorec_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_PAUSE),
												(PS8)GetString(STR_ID_VDOPLY_STOP),
												IMG_ID_VDOREC_LSK_PAUSE,
												IMG_ID_VDOREC_RSK_STOP);
			#else
				mmi_vdorec_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_PAUSE/*STR_ID_VDOREC_PAUSE*/),
												(PS8)GetString(STR_ID_VDOPLY_STOP/*STR_ID_VDOREC_STOP*/),
												IMG_ID_VDOREC_LSK_PAUSE,
												IMG_ID_VDOREC_RSK_STOP);
			#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */
            lcd_lock_all_blt_operation();
			mmi_vdorec_draw_softkey();
//			mmi_vdorec_draw_panel_state();

			mmi_vdorec_draw_panel();
            lcd_unlock_all_blt_operation();
			mmi_vdorec_blt_screen();	
				
			if(prev_state == VDOREC_STATE_PAUSE)
			{
				mmi_trace(g_sw_VdoRec," mmi_vdorec_enter_state VDOREC_STATE_RECORD prev_state == VDOREC_STATE_PAUSE");
				g_vdorec_cntx.last_error = mdi_video_rec_record_resume();

				/* 
					this may happend Meida send out record finsih message but MMI not receive it,
					meanwhile MMI try to pasue.
					MDI will ignore this meassge when some one call pause API at this situation, so
					MMI has to enter save screen
				*/
				if(g_vdorec_cntx.last_error == MDI_RES_VDOREC_RECORD_ALREADY_STOP)
				{
					mmi_vdorec_entry_saving_screen();
					return;
				}
			}
			else
			{
//				mmi_vdorec_get_save_filename(buf_filename);
//				if(NO_ERRO!= mmi_vdorec_creat_file()){
//					g_vdorec_cntx.last_error=MDI_RES_VDOREC_ERR_DISK_FULL;
//					return;
//					}

			        mmi_vidrec_get_next_filename(buf_filename);
			        vdorec_filehandle = MMI_FS_Open(buf_filename, FS_CREATE_ALWAYS);
					
			        if (vdorec_filehandle < 0)
			        {
			            mmi_trace(1,"fail to creat file--- \n");
				     g_vdorec_cntx.last_error= MDI_RES_VDOREC_ERR_DISK_FULL;	
			            return;
			        }

			        mmi_vidrec_store_filename();  
					
				g_vdorec_cntx.last_error = mdi_video_rec_record_start(vdorec_filehandle, mmi_vdorec_record_result_hdlr_callback);
			}

			/* error check */
			if(mmi_vdorec_check_and_display_error_popup() == TRUE) /* TRUE means has error */
				return;

			/* start draw timer */
			mmi_vdorec_record_timer_cyclic();
		}
		break;

		case VDOREC_STATE_PAUSE:
		{		
			g_vdorec_cntx.state = state;	
			

			#if defined(__VDOREC_FEATURE_TOUCH_SCREEN__)
				mmi_vdorec_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_RESUME),
												(PS8)GetString(STR_ID_VDOPLY_STOP),
												IMG_ID_VDOREC_LSK_RESUME,
												IMG_ID_VDOREC_RSK_STOP);
			#else
				mmi_vdorec_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_RESUME/*STR_ID_VDOREC_RESUME*/), 
												(PS8)GetString(STR_ID_VDOPLY_STOP/*STR_ID_VDOREC_STOP*/),
												IMG_ID_VDOREC_LSK_RESUME,
												IMG_ID_VDOREC_RSK_STOP);	
			#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */
			

            lcd_lock_all_blt_operation();
			mmi_vdorec_draw_softkey();
//			mmi_vdorec_draw_panel_state();

			mmi_vdorec_draw_panel();
            lcd_unlock_all_blt_operation();
			mmi_vdorec_blt_screen();	
			
			g_vdorec_cntx.last_error = mdi_video_rec_record_pause();
			
			coolsand_UI_cancel_timer(mmi_vdorec_record_timer_cyclic);

			/* 
				this may happend Meida send out record finsih message but MMI not receive it,
				meanwhile MMI try to pasue.
				MDI will ignore this meassge when some one call pause API at this situation, so
				MMI has to enter save screen
			*/
			if(g_vdorec_cntx.last_error == MDI_RES_VDOREC_RECORD_ALREADY_STOP)
			{
				mmi_vdorec_entry_saving_screen();
				return;
			}
			else if(g_vdorec_cntx.last_error == MDI_RES_VDOREC_DISK_FULL)
			{
				g_vdorec_cntx.is_force_saving = TRUE;
				mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_INFO, 1, ST_NOTIFYDURATION, WARNING_TONE);
				return;
			}
			else if(g_vdorec_cntx.last_error == MDI_RES_VDOREC_REACH_SIZE_LIMIT)
			{
				g_vdorec_cntx.is_force_saving = TRUE;
				mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOREC_NOTIFY_REACH_SIZE_LIMIT), IMG_GLOBAL_INFO, 1, ST_NOTIFYDURATION, WARNING_TONE);		
				return;
			}

			/* pasue error check */
			if(mmi_vdorec_check_and_display_error_popup() == TRUE) /* TRUE means has error */
				return;

			mmi_vdorec_check_and_display_error_popup();
		}
		break;
			
	}
	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_post_interrupt_hdlr
* DESCRIPTION
*  Resume popup, will be called after all interrupt
* PARAMETERS
*	a IN  message pointer (not used)
* RETURNS
*  U8
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static U8 mmi_vdorec_post_interrupt_hdlr(void* inMsg)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetInterruptPopupDisplay(POPUP_ALL_ON);
	return FALSE;
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_post_interrupt_hdlr
* DESCRIPTION
*  Message interrupt hdlr
* PARAMETERS
*	a IN  message pointer (not used)
* RETURNS
*  U8
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static U8 mmi_vdorec_sms_interrupt_hdlr(void* inMsg)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetInterruptPopupDisplay(POPUP_ALL_OFF);
	return FALSE;
}
#if 0
static U8 mmi_vdorec_earphone_interrupt_hdlr(void* inMsg)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetInterruptPopupDisplay(POPUP_ALL_OFF);
	return FALSE;
}
#endif

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_battery_status_interrupt_hdlr
* DESCRIPTION
*  Battery status interrput handler
* PARAMETERS
*	a IN  message pointer
* RETURNS
*  U8
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static U8 mmi_vdorec_battery_status_interrupt_hdlr(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	mmi_eq_battery_status_ind_struct *p=(mmi_eq_battery_status_ind_struct*) inMsg;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	switch(p->battery_status)
	{
		case PMIC_CHARGER_IN:
		case PMIC_CHARGER_OUT:
		case PMIC_CHARGE_COMPLETE:
			SetInterruptPopupDisplay(POPUP_ALL_OFF);
			break;
		default:
		break;
	}	
#endif /* MMI_ON_HARDWARE_P */
	return FALSE; /* return false will process defualt hdlr */
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_msdc_interrupt_hdlr
* DESCRIPTION
*  Memory card interrupt handler.
* PARAMETERS
*	a IN  message pointer (not used)
* RETURNS
*  U8
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static U8 mmi_vdorec_msdc_interrupt_hdlr(void* inMsg)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* if storage is phone, do not need popup */
	if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_PHONE)
		SetInterruptPopupDisplay(POPUP_ALL_OFF);
	
	return FALSE;
}


/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_clam_close_event_hdlr()
* DESCRIPTION
*   handle clam close event 
* PARAMETERS
*	 ptr   IN	    message data ptr
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) && defined(MMI_ON_HARDWARE_P)
static U8 mmi_vdorec_clam_close_event_hdlr(void* ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_eq_gpio_detect_ind_struct *gpioDetectInd;
	
	gpioDetectInd = (mmi_eq_gpio_detect_ind_struct *)ptr;
	
	if((gpioDetectInd->gpio_device == EXT_DEV_CLAM_CLOSE) || 
		(gpioDetectInd->gpio_device == EXT_DEV_CLAM_OPEN) )
	{
		
		if(gpioDetectInd->gpio_device == EXT_DEV_CLAM_CLOSE)
		{
			GoBackHistory();
			return FALSE;	
		}
	}

	/* return false means will pass this interrut to default handler */
	return FALSE;	
}
#endif /* __MMI_CLAMSHELL__ && __MMI_SLIDE__ && MMI_ON_HARDWARE_P */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_app
* DESCRIPTION
*  video record app entry point
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void);
extern BOOL is_phb_init_done(void);
extern BOOL checkVMOn(void);
extern VOID mmi_display_gprs_active(void) ;
static void mmi_vdorec_entry_app(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8		buf_filepath[FMGR_PATH_BUFFER_SIZE];
	U8		*guiBuffer;
	U16	cur_storage;
	S32	create_result;

	#ifdef __MMI_TVOUT__
		U16	owner;
	#endif

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
    if(HandleLowBatteryProcess())
    {
        mmi_trace(1,"low battery in vdo rec");
        return ;
    }

	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(mmi_vdorec_entry_app);
		return;
	}
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}


#ifdef JATAAYU_SUPPORT
	if(
	#ifdef JATAAYU_SUPPORT_MMS
		jMMS_IsMMSAppOrDownloadActive()
	#endif	
	#ifdef JATAAYU_SUPPORT_WAP
	#ifdef JATAAYU_SUPPORT_MMS
		||
	#endif
		jMMS_IsWapConnected()
	#endif
		)
	{
		DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_GPRS_ACTIVE) ,
			IMG_GLOBAL_PROGRESS, CAMERA_WARNING_POPUPDURATION,
			UI_POPUP_NOTIFYDURATION_TIME,
			WARNING_TONE);
				return;
	}

#endif

#if 0
	if(!is_phb_init_done())
	{
	    //ʼ stringid =25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS/*IMG_GLOBAL_ACTIVATED*/, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);//modified by lin for bug 6761@20070929
		return;
	}
#endif	
	EntryNewScreen(SCR_ID_VDOREC_APP, mmi_vdorec_exit_app, mmi_vdorec_entry_app, NULL);

	mmi_trace(1,"chenhe,mmi_vdorec_entry_app");
	/* set TV out mode */
	/* if is full screen multimedia mode, change to tvout mode2 */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{
				owner = mdi_tvout_get_owner();

				/* tvout is enable */
				if(owner == MDI_TV_OWNER_GDI)
				{
					/* if is fullscreen, we shall set to owner to video here, to avoid GDI's update */
					mdi_tvout_set_owner(MDI_TV_OWNER_VIDEO);	
					
					/* start mode 2 - full screen preview update */
					mdi_tvout_set_mode(	MDI_TV_MODE_VIDEO_PREVIEW,
												0,						/* layer_width */
												0);					/* layer_height */
					
				}
			}
		}
	#endif /* __MMI_TVOUT__ */



	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	/* entry full screen app */
//	#ifndef __MMI_UI_STYLE_4__
		/* other style no status */
		entry_full_screen();
//	#endif /* __MMI_UI_STYLE_4__ */	

	guiBuffer = GetCurrGuiBuffer(SCR_ID_VDOREC_APP);


		/* entry sublcd */
	#ifdef __MMI_SUBLCD__
		if(!g_vdorec_cntx.is_sub_display)
		{
			ForceSubLCDScreen(mmi_vdorec_entry_sublcd_screen);
		}
	#endif 


	#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) && defined(MMI_ON_HARDWARE_P)
		/* register event hdlr */
		SetInterruptEventHandler(mmi_vdorec_clam_close_event_hdlr, NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	#endif /* __MMI_CLAMSHELL__ && __MMI_SLIDE__ && MMI_ON_HARDWARE_P */


	/* hook to touch screen callback */
	#if defined(__VDOREC_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
		mmi_wgui_register_pen_down_handler(mmi_vdorec_touch_scr_pen_down_hdlr);
		mmi_wgui_register_pen_up_handler(mmi_vdorec_touch_scr_pen_up_hdlr);
		mmi_wgui_register_pen_move_handler(mmi_vdorec_touch_scr_pen_move_hdlr);
	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ && __MMI_TOUCH_SCREEN__ */

	/* load setting */
	mmi_vdorec_load_setting();
	
//	mmi_trace(1,"---init effect is %d-----",g_vdorec_cntx.setting.effect);

	if(guiBuffer == NULL)
	{
		/* each time newly enter vdorec, zoom will be reset to default */
		g_vdorec_cntx.setting.zoom = VDOREC_DEFAULT_SETTING_ZOOM;
	}

	/* init some golbal value */
	g_vdorec_cntx.last_error					= MDI_RES_VDOREC_SUCCEED;
	g_vdorec_cntx.record_time					= 0;
	g_vdorec_cntx.is_lsk_pressed 				= FALSE;
	g_vdorec_cntx.is_rsk_pressed 				= FALSE;	
	g_vdorec_cntx.is_zoom_in_key_pressed 	= FALSE;
	g_vdorec_cntx.is_zoom_out_key_pressed 	= FALSE;
	g_vdorec_cntx.is_ev_inc_key_pressed 	= FALSE;
	g_vdorec_cntx.is_ev_dec_key_pressed 	= FALSE;

	/* clear hint buffer */
	memset(g_vdorec_cntx.hint1_buf, 0, VDOREC_HINT_BUF_SIZE);
	memset(g_vdorec_cntx.hint2_buf, 0, VDOREC_HINT_BUF_SIZE);	
	
	/* allocate storage path by using submenu's buffer */
	g_vdorec_cntx.storage_filepath = coolsand_UI_malloc(FMGR_PATH_BUFFER_SIZE);

	/* check if path is valid or not. if not will pop up a screen after the preview screen is shown */
	cur_storage = g_vdorec_cntx.setting.storage;


	
	if(mmi_vdorec_check_and_restore_valid_drv())
	{
		/* if no error. load path into buffer */
		mmi_vdorec_get_storage_file_path((PS8)g_vdorec_cntx.storage_filepath);
		create_result = mmi_vdorec_create_file_dir((PS8)g_vdorec_cntx.storage_filepath);

		/* create directory failed */
		if(create_result == FS_WRITE_PROTECTION)
		{
			g_vdorec_cntx.last_error = VDOREC_ERR_WRITE_PROTECT;

			/* has error - restore to phone */
			g_vdorec_cntx.setting.storage				= VDOREC_SETTING_STORAGE_PHONE;	
			g_vdorec_cntx.is_storage_path_changed 	= TRUE;
		}
		else if(create_result == FS_DISK_FULL)
		{
			if(cur_storage == VDOREC_SETTING_STORAGE_PHONE)
			{
				g_vdorec_cntx.last_error = VDOREC_ERR_PHONE_DISK_FULL;
			}
			else
			{
				/* storage is card */
				g_vdorec_cntx.last_error = VDOREC_ERR_CARD_DISK_FULL;

				/* has error - restore to phone */
				g_vdorec_cntx.setting.storage				= VDOREC_SETTING_STORAGE_PHONE;	
				g_vdorec_cntx.is_storage_path_changed 	= TRUE;
			}
		}
		else if(create_result == FS_ROOT_DIR_FULL)
		{
			if(cur_storage == VDOREC_SETTING_STORAGE_PHONE)
			{
				g_vdorec_cntx.last_error = VDOREC_ERR_ROOT_DIR_FULL;
			}
			else
			{
				/* storage is card */
				g_vdorec_cntx.last_error = VDOREC_ERR_ROOT_DIR_FULL;

				/* has error - restore to phone */
				g_vdorec_cntx.setting.storage				= VDOREC_SETTING_STORAGE_PHONE;	
				g_vdorec_cntx.is_storage_path_changed 	= TRUE;
			}
		}		
		
	}
	else
	{
		/* drv no valid, means card is plugged out (or phone FAT corrupted) */
		
		/* if storage before restoration is already in phone, means FAT is corrupted */
		if(cur_storage == VDOREC_SETTING_STORAGE_PHONE) 
		{
			g_vdorec_cntx.last_error = VDOREC_ERR_CORRUPT_FAT;
		}
		else
		{
			g_vdorec_cntx.last_error = MDI_RES_VDOREC_ERR_NO_DISK;

		}
	}


	/*******************************************************************
	* Init proper global state for video applcation  
	********************************************************************/ 
	/* force all playing keypad tone off */
	AudioStopReq(GetCurKeypadTone());
	
  	/* disable key pad tone */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);

	/* enable multi_layer */
	gdi_layer_multi_layer_enable();

	/* suspend background play */
	mdi_audio_suspend_background_play();

	/* disalbe align timer and enter game */
	UI_disable_alignment_timers();

	/* stop LED patten */
	GpioCtrlReq(MMIAPI_DEV_CTRL_GET);

	/* stop MMI sleep */
	TurnOnBacklight(0);

	/* use medium font */
	coolsand_UI_set_font(&MMI_medium_font);
	
	/***************************************************************************** 
	* Create layers 
	*****************************************************************************/
	/************ base layer layer ************/
	gdi_layer_get_base_handle(&g_vdorec_cntx.base_layer_handle);

	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		g_vdorec_cntx.lcd_width 	= UI_device_height;
		g_vdorec_cntx.lcd_height 	= UI_device_width;

		/* rotate base layer */
		gdi_layer_set_rotate(GDI_LAYER_ROTATE_270);
		gdi_layer_resize(g_vdorec_cntx.lcd_width, g_vdorec_cntx.lcd_height);
	#else
		g_vdorec_cntx.lcd_width 	= UI_device_width;
		g_vdorec_cntx.lcd_height 	= UI_device_height;

	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		mmi_frm_set_self_rotation_flag(MMI_FRM_SCREEN_ROTATE_270);
	
		/************ preview window layer ************/

		/* create at 0,0, then move to correct position after rotate */
		gdi_layer_create(	UI_device_width-g_vdorec_osd_cntx.preview_wnd.rect.offset_y-g_vdorec_osd_cntx.preview_wnd.rect.height,
								g_vdorec_osd_cntx.preview_wnd.rect.offset_x, 
								g_vdorec_osd_cntx.preview_wnd.rect.width,
								g_vdorec_osd_cntx.preview_wnd.rect.height,
								&g_vdorec_cntx.preview_layer_handle);

		gdi_layer_push_and_set_active(g_vdorec_cntx.preview_layer_handle);
		gdi_layer_clear_background(GDI_COLOR_BLACK);
	

		/* rotate preview layer */
		gdi_layer_set_rotate(GDI_LAYER_ROTATE_270);
		gdi_layer_set_position(	g_vdorec_osd_cntx.preview_wnd.rect.offset_x, 
										g_vdorec_osd_cntx.preview_wnd.rect.offset_y);
		
	#else
		/************ preview window layer ************/
		gdi_layer_create(	g_vdorec_osd_cntx.preview_wnd.rect.offset_x,
								g_vdorec_osd_cntx.preview_wnd.rect.offset_y, 
								g_vdorec_osd_cntx.preview_wnd.rect.width,
								g_vdorec_osd_cntx.preview_wnd.rect.height,
								&g_vdorec_cntx.preview_layer_handle);

		gdi_layer_push_and_set_active(g_vdorec_cntx.preview_layer_handle);
		gdi_layer_clear_background(GDI_COLOR_BLACK);
		
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
	
	gdi_layer_pop_and_restore_active();

	/* resized preview layer */
	#ifdef __VDOREC_FEATURE_VIDEO_SIZE__
		mmi_vdorec_resize_preview_layer();
	#endif /* __VDOREC_FEATURE_VIDEO_SIZE__ */

	/************ osd layer ************/
	/* create a double layer frame from outside memory */

	#ifdef APPMEM_SUPPORT
		/* with ASM */
		g_vdorec_cntx.osd_layer_buf_ptr = (PU8)AllocMediaMemory(VDOREC_OSD_BUFFER_SIZE);
	#else
		/* no ASM */
		g_vdorec_cntx.osd_layer_buf_ptr = (PU8)camera_osd_layer_buffer;
	#endif /* APPMEM_SUPPORT */

	gdi_layer_create_double_using_outside_memory(0,
																0,
																g_vdorec_cntx.lcd_width,
																g_vdorec_cntx.lcd_height,
																&g_vdorec_cntx.osd_layer_handle,
																(PU8)g_vdorec_cntx.osd_layer_buf_ptr, 
																(S32)VDOREC_OSD_BUFFER_SIZE); /* this buffer is double buffer size */
	
	gdi_layer_push_and_set_active(g_vdorec_cntx.osd_layer_handle);

	/* clear both layer */
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	
	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		/* rotate preview layer */
		gdi_layer_set_rotate(GDI_LAYER_ROTATE_270);
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
		
	gdi_layer_toggle_double();

	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		/* rotate preview layer */
		gdi_layer_set_rotate(GDI_LAYER_ROTATE_270);
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
	
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);	
	gdi_layer_pop_and_restore_active();		

	

	/***************************************************************************** 
	* register interrupt event hdlr
	*****************************************************************************/
	#ifdef MMI_ON_HARDWARE_P
		/* bettery */
		SetInterruptEventHandler(mmi_vdorec_battery_status_interrupt_hdlr, mmi_vdorec_post_interrupt_hdlr, MSG_ID_MMI_EQ_BATTERY_STATUS_IND);
		/*ear phone,chenhe add*/
	//	SetInterruptEventHandler(mmi_vdorec_earphone_interrupt_hdlr, mmi_vdorec_post_interrupt_hdlr, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
		
	#endif /* MMI_ON_HARDWARE_P */

	/* msg */
	SetInterruptEventHandler(mmi_vdorec_sms_interrupt_hdlr, mmi_vdorec_post_interrupt_hdlr, PRT_MSG_ID_MMI_SMS_DELIVER_MSG_IND);
	SetInterruptEventHandler(mmi_vdorec_sms_interrupt_hdlr, mmi_vdorec_post_interrupt_hdlr, PRT_MSG_ID_MMI_SMS_STATUS_REPORT_IND);
	SetInterruptEventHandler(mmi_vdorec_sms_interrupt_hdlr, mmi_vdorec_post_interrupt_hdlr, PRT_MSG_ID_MMI_SMS_MSG_WAITING_IND);	

	#ifdef MMI_ON_HARDWARE_P
		/* memory card */
		SetInterruptEventHandler(mmi_vdorec_msdc_interrupt_hdlr, mmi_vdorec_post_interrupt_hdlr, MSG_ID_FMT_MMI_CARD_PLUG_IN_IND);
		SetInterruptEventHandler(mmi_vdorec_msdc_interrupt_hdlr, mmi_vdorec_post_interrupt_hdlr, MSG_ID_FMT_MMI_CARD_PLUG_OUT_IND);
	#endif /* MMI_ON_HARDWARE_P */

	/****************************************************************************
	* draw skin                                                                 
	*****************************************************************************/

	mmi_vdorec_init_panel();

	gdi_layer_reset_clip();				/* use full screen clip */
	coolsand_UI_reset_text_clip();		/* use full screen text clip */

	gdi_layer_lock_frame_buffer();

	/* bg image */
	gdi_image_draw_id(0, 0, IMG_ID_VDOREC_OSD_BG);

	#ifdef __MMI_UI_STYLE_4__
		/* restore hide status function */
		register_hide_status_icon_bar(0, mmi_vdorec_redraw_status_bg);	
		refresh_status_icon_display();	
	#endif /* __MMI_UI_STYLE_4__ */
	mmi_vdorec_draw_panel_state();
	mmi_vdorec_draw_title();
	mmi_vdorec_draw_panel();
	mmi_vdorec_draw_osd();

	#ifdef __MMI_VIDEO_23KEY__
		mmi_vdorec_set_softkey(	(PS8)GetString(STR_GLOBAL_OPTIONS), 
										(PS8)GetString(STR_ID_VDOREC_RSK_RECORD),
										IMG_ID_VDOREC_LSK_OPTION,
										IMG_ID_VDOREC_RSK_RECORD);
	
	#else
	#ifdef OPTION_ENABLED
		mmi_vdorec_set_softkey(	(PS8)GetString(STR_GLOBAL_OPTIONS), 
										(PS8)GetString(STR_GLOBAL_BACK),
										IMG_ID_VDOREC_LSK_OPTION,
										IMG_ID_VDOREC_RSK_BACK);
	#else
		mmi_vdorec_set_softkey(	NULL, 
										(PS8)GetString(STR_GLOBAL_BACK),
										0,
										IMG_ID_VDOREC_RSK_BACK);	
	#endif
	
	#endif /* __MMI_VIDEO_23KEY__ */

	mmi_vdorec_draw_softkey();
	gdi_layer_unlock_frame_buffer();

#if 1
	/* blt inital skin to LCM */
	gdi_layer_set_blt_layer(g_vdorec_cntx.base_layer_handle,
									g_vdorec_cntx.preview_layer_handle,
									g_vdorec_cntx.osd_layer_handle,
									0);
#endif
	mmi_vdorec_blt_full_screen();
#if 0
	/* set blt layer */
#ifdef WIN32

	/* for PC environment */
	gdi_layer_set_blt_layer(g_vdorec_cntx.base_layer_handle,
									g_vdorec_cntx.preview_layer_handle,
									g_vdorec_cntx.osd_layer_handle,
									0);

	gdi_layer_push_and_set_active(g_vdorec_cntx.preview_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_GRAY); /* gray color for easy debug */
	gdi_layer_pop_and_restore_active();

#else
	#if defined(ISP_SUPPORT)
		/* for Target */
		gdi_layer_set_blt_layer(g_vdorec_cntx.base_layer_handle,
										GDI_LAYER_MEDIA_HW_TRIGGER_LAYER,
										g_vdorec_cntx.osd_layer_handle,
										0);
	
	#elif defined(CAMERA_MODULE)
		gdi_layer_set_blt_layer(g_vdorec_cntx.base_layer_handle,
										g_vdorec_cntx.preview_layer_handle,
										g_vdorec_cntx.osd_layer_handle,
										0);
	#endif 
	
#endif /* WIN32 */
#endif
	/* power on HW */
	if(g_vdorec_cntx.last_error == MDI_RES_VDOREC_SUCCEED)
		g_vdorec_cntx.last_error = mdi_video_rec_power_on();
	mmi_trace(g_sw_VdoRec,"chenhe,mmi_vdorec_entry_app,g_vdorec_cntx.last_error is %d",g_vdorec_cntx.last_error);
	if(mmi_vdorec_check_and_display_error_popup() != TRUE) /* !TRUE means error popup displayed */
	{
			mmi_trace(g_sw_VdoRec,"chenhe,mmi_vdorec_entry_app,");
		/* 
			if record finish has popup, it will not entry saving directly, 
			after popup gone, we have to enter saving procedure 
		*/
		if(g_vdorec_cntx.is_force_saving == TRUE)
		{
			mmi_trace(1," mmi_vdorec_enter_state(VDOREC_STATE_PREVIEW) 222222222");

			mmi_vdorec_entry_saving_screen();
			g_vdorec_cntx.is_force_saving = FALSE;
			return;
		}
			mmi_trace(1," mmi_vdorec_enter_state(VDOREC_STATE_PREVIEW) 1111111");
		/* ask media if there is a unsaved file */
		mmi_vdorec_get_storage_file_path(buf_filepath);
					mmi_trace(1," mmi_vdorec_enter_state(VDOREC_STATE_PREVIEW) 3333333333");

		if(mdi_video_rec_has_unsaved_file(buf_filepath) == TRUE)
		{
					mmi_trace(1," mmi_vdorec_enter_state(VDOREC_STATE_PREVIEW)4444444444444");

			mmi_vdorec_entry_save_confirm_screen();
			return;
		}
		else
		{
			mmi_trace(1," mmi_vdorec_enter_state(VDOREC_STATE_PREVIEW)");
//			mmi_trace(1,"---before effect is %d-----",g_vdorec_cntx.setting.effect);
			mmi_vdorec_enter_state(VDOREC_STATE_PREVIEW);
			mmi_vdorec_turn_on_led_highlight();
		}
		
	}
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_exit_app
* DESCRIPTION
*  video recorder ext function
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_exit_app(void)
{
#if 1
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
mmi_trace(1,"mmi_vdorec_exit_app,g_vdorec_cntx.state is %d",g_vdorec_cntx.state);
	if(g_vdorec_cntx.state == VDOREC_STATE_RECORD)
	{
		mdi_video_rec_record_stop();
	}
	else if(g_vdorec_cntx.state == VDOREC_STATE_PREVIEW)
	{
		//mdi_video_rec_preview_stop();			
		mdi_video_rec_record_previewStop();//chenhe add
		mmi_trace(1,"mmi_vdorec_exit_app,VDOREC_STATE_PREVIEW");
	}
	else if( g_vdorec_cntx.state == VDOREC_STATE_PAUSE)
	{
		mdi_video_rec_record_stop();
//		mdi_video_rec_preview_stop();
//    mdi_video_rec_record_previewStop();//chenhe add
	}
	g_vdorec_cntx.state = VDOREC_STATE_EXIT;

	mmi_vdorec_store_setting();
	
	mdi_video_rec_power_off();

	mmi_vdorec_turn_off_led_highlight();

	/* cancel active timer */
	coolsand_UI_cancel_timer(mmi_vdorec_record_timer_cyclic);
	coolsand_UI_cancel_timer(mmi_vdorec_hide_hint);
	
	#ifdef __VDOREC_FEATURE_ZOOM__
		coolsand_UI_cancel_timer(mmi_vdorec_zoom_in);
		coolsand_UI_cancel_timer(mmi_vdorec_zoom_out);
	#endif /* __VDOREC_FEATURE_ZOOM__ */

	gdi_layer_flatten_to_base(	g_vdorec_cntx.base_layer_handle,
										g_vdorec_cntx.preview_layer_handle, 
										g_vdorec_cntx.osd_layer_handle, 
										0);
	
	/* free layers */
	gdi_layer_free(g_vdorec_cntx.preview_layer_handle);
	g_vdorec_cntx.preview_layer_handle = 0;

	/* free osd layer */
	gdi_layer_free(g_vdorec_cntx.osd_layer_handle);
	g_vdorec_cntx.osd_layer_handle = GDI_LAYER_EMPTY_HANDLE;

	#ifdef APPMEM_SUPPORT
		/* with ASM */
		FreeMediaMemory((unsigned long)g_vdorec_cntx.osd_layer_buf_ptr); 
		g_vdorec_cntx.osd_layer_buf_ptr = NULL;
	#endif /* APPMEM_SUPPORT */

	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		/* restore base layer rotation */
		gdi_layer_set_rotate(GDI_LAYER_ROTATE_0);
		gdi_layer_resize(UI_device_width, UI_device_height);
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

	/* disable multi layer */
	gdi_layer_multi_layer_disable();
	
	/* resume alignment timer */
	UI_disable_alignment_timers();

	/* resume LED patten */
	GpioCtrlReq(MMIAPI_DEV_CTRL_RELEASE);


	/* let MMI can sleep */
	TurnOffBacklight();

	/* resume background audio */
	mdi_audio_resume_background_play();

	/* re-enable keypad tone */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);

	/* free buffer */
	coolsand_UI_free(g_vdorec_cntx.storage_filepath);
	g_vdorec_cntx.storage_filepath = NULL;
	
	clear_vdoRec_result_cb();
	
	/* exit sublcd */
#ifdef __MMI_SUBLCD__
	/* exit app screen if exist , and not exit because toggle self capture to sublcd */
	if(g_vdorec_cntx.is_sub_display)
	{
		GoBackSubLCDHistory();
		g_vdorec_cntx.is_sub_display = FALSE;
	}
#endif

	#ifdef MMI_ON_HARDWARE_P
		/* un-hook clam close msg */
		SetInterruptEventHandler(NULL, NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	#endif /* MMI_ON_HARDWARE_P */


	/* restore TV out mode */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{
				mdi_tvout_set_owner(MDI_TV_OWNER_GDI);			
				
				/* restore back to start mode1 */
				mdi_tvout_set_mode(	MDI_TV_MODE_LCD_SCREEN,
											0,						/* layer_width */
											0);					/* layer_height */
			}
		}
	#endif /* __MMI_TVOUT__ */

#endif
}



/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_draw_sublcd_icon()
* DESCRIPTION
*   draw a sublcd icon.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
static void mmi_vdorec_draw_sublcd_icon(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 image_width;
	S32 image_height;
	S32 lcd_width;
	S32 lcd_height;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
	gdi_lcd_get_dimension(&lcd_width, &lcd_height);

	gdi_layer_reset_clip();

	gdi_layer_clear_background(GDI_COLOR_WHITE);
	gdi_image_get_dimension_id(IMG_ID_VDOREC_SUBLCD_ICON, &image_width, &image_height);
	gdi_image_draw_id((lcd_width-image_width)>>1,(lcd_height-image_height)>>1,IMG_ID_VDOREC_SUBLCD_ICON);
	gdi_layer_blt_base_layer(0,0, lcd_width-1, lcd_height-1);

	gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
}
#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_entry_sublcd_screen()
* DESCRIPTION
*   entry sublcd screen, if still remain on standard MMI sublcd screen, time update will
*   redraw sublcd while mainlcd is previewing, will cause driver assert.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
static void mmi_vdorec_entry_sublcd_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ExecSubLCDCurrExitHandler();
	ShowCategory331Screen();
	SetSubLCDExitHandler(mmi_vdorec_exit_sublcd_screen);

	/* draw a icon on sub */
	mmi_vdorec_draw_sublcd_icon();

	g_vdorec_cntx.is_sub_display = TRUE;
}
#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_exit_sublcd_screen()
* DESCRIPTION
*   exit sublcd screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
static void mmi_vdorec_exit_sublcd_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   SubLCDHistoryNode SubLCDHistory;
	
	/* someone may exit/draw sublcd first then exit mainlcd, this may cause driver assert */
   /* also stop preview here */
	
	if(g_vdorec_cntx.state == VDOREC_STATE_RECORD)
	{
		mdi_video_rec_record_stop();
	}
	else if(g_vdorec_cntx.state == VDOREC_STATE_PREVIEW)
	{
		mdi_video_rec_preview_stop();
	}
	else if( g_vdorec_cntx.state == VDOREC_STATE_PAUSE)
	{
		mdi_video_rec_record_stop();
		mdi_video_rec_preview_stop();
	}

	g_vdorec_cntx.state = VDOREC_STATE_EXIT;

	/* exit sub */
	g_vdorec_cntx.is_sub_display = FALSE;
	
	SubLCDHistory.entryFuncPtr = mmi_vdorec_entry_sublcd_screen;
	AddSubLCDHistory(&SubLCDHistory);
}
#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_record_timer_cyclic
* DESCRIPTION
*  record timer cyclic to get and draw timer
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_record_timer_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 prev_time_in_sec;
	U32 time_in_sec;
	U32	is_ui_update = 0; 

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   	coolsand_UI_start_timer( 400, mmi_vdorec_record_timer_cyclic );

	prev_time_in_sec = (U32)(g_vdorec_cntx.record_time);
		
	mdi_video_rec_get_cur_record_time((U32 *)&g_vdorec_cntx.record_time);	
	time_in_sec = (U32)(g_vdorec_cntx.record_time/1000);

	/* time limit check */
	#ifdef __VDOREC_FEATURE_TIME_LIMIT__
		switch(g_vdorec_cntx.setting.time_limit)
		{
			case VDOREC_SETTING_TIME_LIMIT_1:
				if(time_in_sec > VDOREC_FEATURE_TIME_LIMIT_1)
				{
					mdi_video_rec_record_stop();
					
					g_vdorec_cntx.is_force_saving = TRUE;
					mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOREC_NOTIFY_REACH_TIME_LIMIT), IMG_GLOBAL_INFO, 0, ST_NOTIFYDURATION, WARNING_TONE);
					return;	
				}
				break;

			case VDOREC_SETTING_TIME_LIMIT_2:
				if(time_in_sec > VDOREC_FEATURE_TIME_LIMIT_2)
				{
					mdi_video_rec_record_stop();

					g_vdorec_cntx.is_force_saving = TRUE;				
					mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOREC_NOTIFY_REACH_TIME_LIMIT), IMG_GLOBAL_INFO, 0, ST_NOTIFYDURATION, WARNING_TONE);
					return;	
				}
				break;	
				
			case VDOREC_SETTING_TIME_LIMIT_3:
				if(time_in_sec > VDOREC_FEATURE_TIME_LIMIT_3)
				{
					mdi_video_rec_record_stop();

					g_vdorec_cntx.is_force_saving = TRUE;
					mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOREC_NOTIFY_REACH_TIME_LIMIT), IMG_GLOBAL_INFO, 0, ST_NOTIFYDURATION, WARNING_TONE);
					return;
				}
				break;		
		}
	#endif /* __VDOREC_FEATURE_TIME_LIMIT__*/
        lcd_lock_all_blt_operation();
		mmi_vdoRec_draw_Recording_timer(prev_time_in_sec,(U32)g_vdorec_cntx.record_time,&is_ui_update);
        lcd_unlock_all_blt_operation();

	if(1 == is_ui_update)
		{
		mmi_vdorec_blt_screen();		
		}
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_draw_title
* DESCRIPTION
*  draw title 
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_draw_title(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 str_width;
	S32 str_height;
	S32 offset_x;
	S32 offset_y;
	PS8 str_ptr;
	
	video_rect_struct *bbox_rect_p;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* display filename */
	if(g_vdorec_osd_cntx.title.is_draw_app_name)
	{
		bbox_rect_p =(video_rect_struct *) &g_vdorec_osd_cntx.title.rect;
		
		coolsand_UI_set_text_clip(bbox_rect_p->offset_x,
										bbox_rect_p->offset_y,
										bbox_rect_p->offset_x+bbox_rect_p->width-1,
										bbox_rect_p->offset_y+bbox_rect_p->height-1);

		str_ptr = (PS8)GetString(STR_ID_VDOREC_APP_NAME);

		coolsand_UI_measure_string((UI_string_type)str_ptr, &str_width, &str_height);


		if(g_vdorec_osd_cntx.title.v_align == VIDEO_ALIGN_TOP)
			offset_y = 0;
		else if (g_vdorec_osd_cntx.title.v_align == VIDEO_ALIGN_BOTTOM)
			offset_y = bbox_rect_p->height-str_height;
		else /* center */
			offset_y = (bbox_rect_p->height-str_height) >> 1;

		offset_y += bbox_rect_p->offset_y;


		if(g_vdorec_osd_cntx.title.h_align == VIDEO_ALIGN_LEFT)
			offset_x = 0;
		else if(g_vdorec_osd_cntx.title.h_align == VIDEO_ALIGN_RIGHT)
			offset_x = bbox_rect_p->width-str_width;
		else
			offset_x = (bbox_rect_p->width-str_width) >> 1;

		offset_x += bbox_rect_p->offset_x;
		mmi_trace(1,"mmi_vdorec_draw_title offset_x=%d,offset_y=%d,str_width=%d,str_height=%d,str_ptr=0x%x\n",offset_x,offset_y,str_width,str_height,str_ptr);
		mmi_vdorec_draw_style_text(str_ptr, offset_x, offset_y, (video_style_text_struct *)&g_vdorec_osd_cntx.title.style_text);


		coolsand_UI_reset_text_clip(); /* restore clip to full screen */
	
	}
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_draw_status
* DESCRIPTION
*  draw video recorder status icons.
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_draw_status(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	video_osd_vdorec_status_struct *status_p;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(!g_vdorec_osd_cntx.status.is_show)
		return;
   
	status_p =(video_osd_vdorec_status_struct *) &g_vdorec_osd_cntx.status;
	

	/****** draw icon MACRO ******/
	#define DRAW_OSD_ICON(a,A)\
	do {\
		if(status_p->a.is_show)\
		{\
			gdi_image_draw_id(status_p->a.offset_x+status_p->rect.offset_x, \
									status_p->a.offset_y+status_p->rect.offset_y, \
									(U16)(IMG_ID_VDOREC_OSD_##A##_START+g_vdorec_cntx.setting.a +1));\
		}\
	} while(0);

	#ifdef __VDOREC_FEATURE_NIGHT__
		DRAW_OSD_ICON(night,NIGHT);	
	#endif 

	#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT__
		DRAW_OSD_ICON(led_highlight,LED_HIGHLIGHT);	
	#endif 

	#ifdef __VDOREC_FEATURE_RECORD_AUD__
		DRAW_OSD_ICON(record_aud,RECORD_AUD);		
	#endif 

	#ifdef __VDOREC_FEATURE_SIZE_LIMIT__
		DRAW_OSD_ICON(size_limit,SIZE_LIMIT);	
	#endif

	#ifdef __VDOREC_FEATURE_TIME_LIMIT__
		DRAW_OSD_ICON(time_limit,TIME_LIMIT);	
	#endif
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_draw_softkey
* DESCRIPTION
*  draw softkey bar (softkey)
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_draw_softkey(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 lsk_offset_x;
	S32 lsk_offset_y;	
	S32 rsk_offset_x;
	S32 rsk_offset_y;	
	S32 str_width;
	S32 str_height;
	S32 img_width;
	S32 img_height;
	U16 image_id;

	#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
		vdorec_ui_touch_struct	*touch_p;
	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(g_vdorec_osd_cntx.softkey.is_icon_softkey == FALSE)
	{
	#if 1
	//	gdi_layer_reset_clip();
		#if 1 //delete error bar of bg zhangl//chenhe enable 090306
		gdi_image_draw_id(g_vdorec_osd_cntx.softkey.rect.offset_x,
								g_vdorec_osd_cntx.softkey.rect.offset_y,
								IMG_ID_VDOREC_OSD_BUTTOM_BAR_BG);	
		#endif
	       // beginning--modified by kecx on 20090219
	     gdi_layer_set_clip(g_vdorec_osd_cntx.softkey.rect.offset_x, 
							g_vdorec_osd_cntx.panel.state.offset_y,
							g_vdorec_osd_cntx.softkey.rect.offset_x + g_vdorec_osd_cntx.softkey.rect.width,
							g_vdorec_osd_cntx.softkey.rect.offset_y+g_vdorec_osd_cntx.softkey.rect.height);
		gdi_image_draw_id(0, 0, IMG_ID_VDOREC_OSD_BG);
		gdi_layer_reset_clip();
		// end--modified by kecx on 20090219
	#else
		gdi_layer_set_clip(	g_vdoply_osd_cntx.softkey.rect.offset_x, 
							g_vdoply_osd_cntx.softkey.rect.offset_y,
							g_vdoply_osd_cntx.softkey.rect.offset_x + 50,
							g_vdoply_osd_cntx.softkey.rect.offset_y+20);
		gdi_image_draw_id(0, 0, IMG_ID_VDOREC_OSD_BG);
		gdi_layer_reset_clip();
	#endif		
	#if 0
		gdi_image_draw_id(g_vdoply_osd_cntx.softkey.rect.offset_x,
								g_vdoply_osd_cntx.softkey.rect.offset_y,
								IMG_ID_VDOPLY_LSK_BG);	
		gdi_image_draw_id(LCD_WIDTH -44,
								LCD_HEIGHT - 20,
								IMG_ID_VDOPLY_RSK_BG);	

#endif
		/* lsk */
		if(g_vdorec_cntx.lsk_str_ptr != NULL)
		{
			coolsand_UI_measure_string((UI_string_type)g_vdorec_cntx.lsk_str_ptr, &str_width, &str_height);
			lsk_offset_x = 2;
			lsk_offset_y = (UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1)+1;

			if(g_vdorec_cntx.is_lsk_pressed)
			{
				lsk_offset_x++;
				lsk_offset_y++;	
			}

			mmi_vdorec_draw_style_text(g_vdorec_cntx.lsk_str_ptr, 
												lsk_offset_x,
												lsk_offset_y,
												(video_style_text_struct *)&g_vdorec_osd_cntx.softkey.lsk_text);

			#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
			
				touch_p = &g_vdorec_cntx.touch_lsk;
				touch_p->offset_x = lsk_offset_x;
				touch_p->offset_y = lsk_offset_y;
				touch_p->width		= str_width;
				touch_p->height	= str_height;			
				
			#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */				
		}


		/* rsk */
		if(g_vdorec_cntx.rsk_str_ptr != NULL)
		{
			
			coolsand_UI_measure_string((UI_string_type)g_vdorec_cntx.rsk_str_ptr, &str_width, &str_height);
			rsk_offset_x = UI_device_width-str_width-2;
			rsk_offset_y = (UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1)+1;

			if(g_vdorec_cntx.is_rsk_pressed)
			{
				rsk_offset_x++;
				rsk_offset_y++;	
			}
			
			mmi_vdorec_draw_style_text(g_vdorec_cntx.rsk_str_ptr, 
												rsk_offset_x,
												rsk_offset_y,
												(video_style_text_struct *)&g_vdorec_osd_cntx.softkey.lsk_text);
			
			#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
			
				touch_p = &g_vdorec_cntx.touch_rsk;
				touch_p->offset_x = rsk_offset_x;
				touch_p->offset_y = rsk_offset_y;
				touch_p->width		= str_width;
				touch_p->height	= str_height;			
				
			#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */		
		}

		#ifdef __MMI_MAINLCD_220X176__
		mmi_vdorec_draw_panel_timer();
		#endif
	}
	else
	{
		gdi_layer_reset_clip();

		/* lsk */
		if(g_vdorec_cntx.lsk_img_id != 0)
		{
			if(g_vdorec_cntx.is_lsk_pressed)
				image_id = g_vdorec_cntx.lsk_img_id+1;
			else
				image_id = g_vdorec_cntx.lsk_img_id;
	
			gdi_image_get_dimension_id(image_id, &img_width, &img_height);

			lsk_offset_x = g_vdorec_osd_cntx.softkey.icon_lsk_pos.offset_x;
			lsk_offset_y = g_vdorec_osd_cntx.softkey.icon_lsk_pos.offset_y;

			gdi_image_draw_id(lsk_offset_x, lsk_offset_y, image_id);

			#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
			
				touch_p = &g_vdorec_cntx.touch_lsk;
				touch_p->offset_x = lsk_offset_x;
				touch_p->offset_y = lsk_offset_y;
				touch_p->width		= img_width;
				touch_p->height	= img_height;			
				
			#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */				
		}

		/* rsk */
		if(g_vdorec_cntx.rsk_img_id != 0)
		{
			if(g_vdorec_cntx.is_rsk_pressed)
			{
				image_id = g_vdorec_cntx.rsk_img_id+1;
			}
			else
			{
				image_id = g_vdorec_cntx.rsk_img_id;
			}

			gdi_image_get_dimension_id(image_id, &img_width, &img_height);

			rsk_offset_x = g_vdorec_osd_cntx.softkey.icon_rsk_pos.offset_x;
			rsk_offset_y = g_vdorec_osd_cntx.softkey.icon_rsk_pos.offset_y;

			gdi_image_draw_id(rsk_offset_x, rsk_offset_y, image_id);

			#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
			
				touch_p = &g_vdorec_cntx.touch_rsk;
				touch_p->offset_x = rsk_offset_x;
				touch_p->offset_y = rsk_offset_y;
				touch_p->width		= img_width;
				touch_p->height	= img_height;			
				
			#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */				
		}

	}
	

}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_set_softkey
* DESCRIPTION
*  Set softkey string ptr that will be draw 
* PARAMETERS
*	lsk_str_ptr		IN  		lsk string pointer
*	rsk_str_ptr		IN			rsk string pointer
*	lsk_img_id		IN  		lsk image id
*	rsk_img_id		IN			rsk image id
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_set_softkey(PS8 lsk_str_ptr, PS8 rsk_str_ptr, U16 lsk_img_id, U16 rsk_img_id)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdorec_cntx.lsk_str_ptr 	= lsk_str_ptr;
	g_vdorec_cntx.rsk_str_ptr 	= rsk_str_ptr;
	
	g_vdorec_cntx.lsk_img_id 	= lsk_img_id;
	g_vdorec_cntx.rsk_img_id 	= rsk_img_id;
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_draw_panel_state
* DESCRIPTION
*  draw panel state (stop, pause, record)
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_draw_panel_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	mmi_trace(1," mmi_vdorec_draw_panel_state  state = %d",g_vdorec_cntx.state);
   
	#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__

		switch(g_vdorec_cntx.state)
		{
			case VDOREC_STATE_EXIT:			
			case VDOREC_STATE_PREVIEW:
			case VDOREC_STATE_PAUSE:				
			{
      				#if 0
				gdi_layer_set_clip(	g_vdorec_osd_cntx.panel.state.offset_x, 
									g_vdorec_osd_cntx.panel.state.offset_y,
									g_vdorec_osd_cntx.panel.state.offset_x + 40,
									g_vdorec_osd_cntx.panel.state.offset_y+40);
				gdi_image_draw_id(0, 0, IMG_ID_VDOREC_OSD_BG);
				gdi_layer_reset_clip();
				#endif
				//draw status
				gdi_image_draw_id(g_vdorec_osd_cntx.touch_rec.offset_x, 
						g_vdorec_osd_cntx.touch_rec.offset_y,
					(U16)(IMG_ID_VDOREC_RSK_RECORD));
				

				/* record */
				if(g_vdorec_cntx.touch_rec.is_press)
				{
					gdi_image_draw_id(g_vdorec_osd_cntx.panel.state.offset_x,
											g_vdorec_osd_cntx.panel.state.offset_y,
											(U16)(IMG_ID_VDOREC_TOUCH_OSD_REC_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdorec_osd_cntx.panel.state.offset_x,
											g_vdorec_osd_cntx.panel.state.offset_y,
											(U16)(IMG_ID_VDOREC_OSD_STATE_PAUSE));
//											(U16)(IMG_ID_VDOREC_TOUCH_OSD_REC));
				}
				#if 0//deleted by zhangl for tp_2324
//        mmi_trace(1,"mmi_vdorec_draw_panel_state,g_vdorec_cntx.touch_stop.is_press is %d,g_vdorec_osd_cntx.touch_stop.offset_x is %d,g_vdorec_osd_cntx.touch_stop.offset_y is %d",g_vdorec_cntx.touch_stop.is_press,g_vdorec_osd_cntx.touch_stop.offset_x,g_vdorec_osd_cntx.touch_stop.offset_y);
				/* stop icon */
				if(g_vdorec_cntx.touch_stop.is_press)
				{
					gdi_image_draw_id(g_vdorec_osd_cntx.touch_stop.offset_x,
											g_vdorec_osd_cntx.touch_stop.offset_y,
											(U16)(IMG_ID_VDOREC_TOUCH_OSD_STOP_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdorec_osd_cntx.touch_stop.offset_x,
											g_vdorec_osd_cntx.touch_stop.offset_y,
											(U16)(IMG_ID_VDOREC_TOUCH_OSD_STOP));

				}
				#endif
				
			}
			break;
		

			case VDOREC_STATE_RECORD:
			{
				#if 0
				gdi_layer_set_clip(	g_vdorec_osd_cntx.panel.state.offset_x, 
									g_vdorec_osd_cntx.panel.state.offset_y,
									g_vdorec_osd_cntx.panel.state.offset_x + 40,
									g_vdorec_osd_cntx.panel.state.offset_y+40);
				gdi_image_draw_id(0, 0, IMG_ID_VDOREC_OSD_BG);
				gdi_layer_reset_clip();
				#endif
				//draw status
				gdi_image_draw_id(g_vdorec_osd_cntx.touch_rec.offset_x, 
						g_vdorec_osd_cntx.touch_rec.offset_y,
						(U16)(IMG_ID_VDOREC_LSK_PAUSE));
				

				
				/* pause */				
				if(g_vdorec_cntx.touch_rec.is_press)
				{
					gdi_image_draw_id(g_vdorec_osd_cntx.panel.state.offset_x,
											g_vdorec_osd_cntx.panel.state.offset_y,
											(U16)(IMG_ID_VDOREC_TOUCH_OSD_PAUSE_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdorec_osd_cntx.panel.state.offset_x,
											g_vdorec_osd_cntx.panel.state.offset_y,
											(U16)(IMG_ID_VDOREC_OSD_STATE_REC));
//											(U16)(IMG_ID_VDOREC_TOUCH_OSD_PAUSE));
				}
				#if 0//deleted by zhangl for tp_2324
				/* stop icon */
				if(g_vdorec_cntx.touch_stop.is_press)
				{
					gdi_image_draw_id(g_vdorec_osd_cntx.touch_stop.offset_x,
											g_vdorec_osd_cntx.touch_stop.offset_y,
											(U16)(IMG_ID_VDOREC_TOUCH_OSD_STOP_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdorec_osd_cntx.touch_stop.offset_x,
											g_vdorec_osd_cntx.touch_stop.offset_y,
											(U16)(IMG_ID_VDOREC_TOUCH_OSD_STOP));
				}
				#endif
			}
			break;
		}

	#else /* is not touch screen */
		switch(g_vdorec_cntx.state)
		{
			case VDOREC_STATE_EXIT:
			case VDOREC_STATE_PREVIEW:
			{
      				#if 0
				gdi_layer_set_clip(	g_vdorec_osd_cntx.panel.state.offset_x, 
									g_vdorec_osd_cntx.panel.state.offset_y,
									g_vdorec_osd_cntx.panel.state.offset_x + 40,
									g_vdorec_osd_cntx.panel.state.offset_y+40);
				gdi_image_draw_id(0, 0, IMG_ID_VDOREC_OSD_BG);
				gdi_layer_reset_clip();
				
		#endif			
				gdi_image_draw_id(g_vdorec_osd_cntx.panel.state.offset_x, 
										g_vdorec_osd_cntx.panel.state.offset_y,
										(U16)(/*IMG_ID_VDOREC_OSD_STATE_REC*/IMG_ID_VDOREC_OSD_STATE_PREVIEW));
			
#if 0
				//draw status
				gdi_image_draw_id(g_vdorec_osd_cntx.panel.status.offset_x, 
						g_vdorec_osd_cntx.panel.status.offset_y,
					(U16)(IMG_ID_VDOREC_OSD_STATUS_PREVIEW));
#endif

			}
			break;

			case VDOREC_STATE_RECORD:
			{
				#if 0
				gdi_layer_set_clip(	g_vdorec_osd_cntx.panel.state.offset_x, 
									g_vdorec_osd_cntx.panel.state.offset_y,
									g_vdorec_osd_cntx.panel.state.offset_x + 40,
									g_vdorec_osd_cntx.panel.state.offset_y+40);
				gdi_image_draw_id(0, 0, IMG_ID_VDOREC_OSD_BG);
				gdi_layer_reset_clip();
				
				#endif	
				gdi_image_draw_id(g_vdorec_osd_cntx.panel.state.offset_x, 
										g_vdorec_osd_cntx.panel.state.offset_y,
										(U16)(IMG_ID_VDOREC_OSD_STATE_REC));
			
#if 0
				//draw status
				gdi_image_draw_id(g_vdorec_osd_cntx.panel.status.offset_x, 
						g_vdorec_osd_cntx.panel.status.offset_y,
						(U16)(IMG_ID_VDOREC_OSD_STATUS_RECORD));
#endif
			}
			break;	

			case VDOREC_STATE_PAUSE:
			{
				#if 0
				gdi_layer_set_clip(	g_vdorec_osd_cntx.panel.state.offset_x, 
									g_vdorec_osd_cntx.panel.state.offset_y,
									g_vdorec_osd_cntx.panel.state.offset_x + 40,
									g_vdorec_osd_cntx.panel.state.offset_y+40);
				gdi_image_draw_id(0, 0, IMG_ID_VDOREC_OSD_BG);
				gdi_layer_reset_clip();
				#endif

				gdi_image_draw_id(g_vdorec_osd_cntx.panel.state.offset_x, 
										g_vdorec_osd_cntx.panel.state.offset_y,
										(U16)(IMG_ID_VDOREC_OSD_STATE_PREVIEW));
#if 0	
				//draw status
				gdi_image_draw_id(g_vdorec_osd_cntx.panel.status.offset_x, 
						g_vdorec_osd_cntx.panel.status.offset_y,
						(U16)(IMG_ID_VDOREC_OSD_STATUS_PREVIEW));
#endif
			}
			break;	
		}


	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */

}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_draw_panel_ev
* DESCRIPTION
*  draw panel ev
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_EV__
static void mmi_vdorec_draw_panel_ev(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gdi_image_draw_id(g_vdorec_osd_cntx.panel.ev.offset_x,
							g_vdorec_osd_cntx.panel.ev.offset_y,
							(U16)(IMG_ID_VDOREC_OSD_EV_START+g_vdorec_cntx.setting.ev+1));

	mmi_trace(g_sw_VdoRec," mmi_vdorec_draw_panel_ev IMG_ID_VDOREC_OSD_EV_START = %d , ev = %d , g_vdorec_cntx.setting.ev = %d",IMG_ID_VDOREC_OSD_EV_START,IMG_ID_VDOREC_OSD_EV_START+g_vdorec_cntx.setting.ev+1,g_vdorec_cntx.setting.ev);
	#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
		{
		/* ev inc */
		if( (g_vdorec_cntx.touch_ev_inc.is_press) ||
			 (g_vdorec_cntx.is_ev_inc_key_pressed) )
		{
			gdi_image_draw_id(g_vdorec_osd_cntx.touch_ev_inc.offset_x,
									g_vdorec_osd_cntx.touch_ev_inc.offset_y,
									IMG_ID_VDOREC_TOUCH_OSD_EV_INC_SEL);			
		}
		else
		{

			if(g_vdorec_cntx.setting.ev < VDOREC_SETTING_EV_COUNT-1)
			{
				gdi_image_draw_id(g_vdorec_osd_cntx.touch_ev_inc.offset_x,
										g_vdorec_osd_cntx.touch_ev_inc.offset_y,
										IMG_ID_VDOREC_TOUCH_OSD_EV_INC);		

			}
			else
			{
				gdi_image_draw_id(g_vdorec_osd_cntx.touch_ev_inc.offset_x,
										g_vdorec_osd_cntx.touch_ev_inc.offset_y,
										IMG_ID_VDOREC_TOUCH_OSD_EV_INC_DIS);	
					
			}
		}

		/* ev dec */
		if( (g_vdorec_cntx.touch_ev_dec.is_press)||
			 (g_vdorec_cntx.is_ev_dec_key_pressed) )
			
		{
			gdi_image_draw_id(g_vdorec_osd_cntx.touch_ev_dec.offset_x,
									g_vdorec_osd_cntx.touch_ev_dec.offset_y,
									IMG_ID_VDOREC_TOUCH_OSD_EV_DEC_SEL);			
		}
		else
		{
			if(g_vdorec_cntx.setting.ev > 0)
			{
				gdi_image_draw_id(g_vdorec_osd_cntx.touch_ev_dec.offset_x,
										g_vdorec_osd_cntx.touch_ev_dec.offset_y,
										IMG_ID_VDOREC_TOUCH_OSD_EV_DEC);		
			}
			else
			{
					gdi_image_draw_id(g_vdorec_osd_cntx.touch_ev_dec.offset_x,
											g_vdorec_osd_cntx.touch_ev_dec.offset_y,
											IMG_ID_VDOREC_TOUCH_OSD_EV_DEC_DIS);	
			}	
		}
		}
	
	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */
	
}
#endif /* __VDOREC_FEATURE_EV__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_draw_panel_zoom
* DESCRIPTION
*  draw panel zoom
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_ZOOM__
static void mmi_vdorec_draw_panel_zoom(void)
{

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* zooming icon */
	gdi_image_draw_id(g_vdorec_osd_cntx.panel.zoom.offset_x,
							g_vdorec_osd_cntx.panel.zoom.offset_y,
							(U16)(IMG_ID_VDOREC_OSD_ZOOM_START+g_vdorec_cntx.setting.zoom+1));
	
	mmi_trace(g_sw_VdoRec," mmi_vdorec_draw_panel_zoom IMG_ID_VDOREC_OSD_ZOOM_START = %d ,zoom = %d, g_vdorec_cntx.setting.ev",IMG_ID_VDOREC_OSD_ZOOM_START,IMG_ID_VDOREC_OSD_ZOOM_START+g_vdorec_cntx.setting.zoom+1,g_vdorec_cntx.setting.ev);

	#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__

		/* zoom inc */
		if( (g_vdorec_cntx.touch_zoom_inc.is_press) || 
			 (g_vdorec_cntx.is_zoom_in_key_pressed) )
		{
			gdi_image_draw_id(g_vdorec_osd_cntx.touch_zoom_inc.offset_x,
									g_vdorec_osd_cntx.touch_zoom_inc.offset_y,
									IMG_ID_VDOREC_TOUCH_OSD_ZOOM_INC_SEL);			
		}
		else
		{

			if(g_vdorec_cntx.setting.zoom < VDOREC_FEATURE_ZOOM_COUNT-1)
			{
				gdi_image_draw_id(g_vdorec_osd_cntx.touch_zoom_inc.offset_x,
										g_vdorec_osd_cntx.touch_zoom_inc.offset_y,
										IMG_ID_VDOREC_TOUCH_OSD_ZOOM_INC);		

			}
			else
			{
				gdi_image_draw_id(g_vdorec_osd_cntx.touch_zoom_inc.offset_x,
										g_vdorec_osd_cntx.touch_zoom_inc.offset_y,
										IMG_ID_VDOREC_TOUCH_OSD_ZOOM_INC_DIS);	
					
			}
		}

		/* zoom dec */
		if( (g_vdorec_cntx.touch_zoom_dec.is_press)|| 
			 (g_vdorec_cntx.is_zoom_out_key_pressed) )
			
		{
			gdi_image_draw_id(g_vdorec_osd_cntx.touch_zoom_dec.offset_x,
									g_vdorec_osd_cntx.touch_zoom_dec.offset_y,
									IMG_ID_VDOREC_TOUCH_OSD_ZOOM_DEC_SEL);			
		}
		else
		{
			if(g_vdorec_cntx.setting.zoom > 0)
			{
				gdi_image_draw_id(g_vdorec_osd_cntx.touch_zoom_dec.offset_x,
										g_vdorec_osd_cntx.touch_zoom_dec.offset_y,
										IMG_ID_VDOREC_TOUCH_OSD_ZOOM_DEC);		
			}
			else
			{
					gdi_image_draw_id(g_vdorec_osd_cntx.touch_zoom_dec.offset_x,
											g_vdorec_osd_cntx.touch_zoom_dec.offset_y,
											IMG_ID_VDOREC_TOUCH_OSD_ZOOM_DEC_DIS);	
			}	
		}
	
	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */

}
#endif /* __VDOREC_FEATURE_ZOOM__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_Playing_timer
* DESCRIPTION
*  draw panel's timer
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoRec_draw_Recording_timer(U32 Pre_Recording_time,U32 Recording_time,U32 *is_ui_update)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 time;
	S32 hour;
	S32 hour_remain;
	S32 min;
	S32 sec;
       S32 old_sec = 0;
	S32 old_min = 0;
	S32 old_hour = 0;


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	time = Pre_Recording_time/1000;
	old_hour				= time/3600;
	hour_remain		= time%3600;
	old_min				= hour_remain/60;
	old_sec				= hour_remain%60;


	time = Recording_time/1000;
	hour				= time/3600;
	hour_remain		= time%3600;
	min				= hour_remain/60;
	sec				= hour_remain%60;

	if(old_sec != sec)
		{
		if(old_hour != hour)
			{

			/* hr */
			gdi_image_draw_id(g_vdorec_cntx.timer.hr_0_offset_x,
									g_vdorec_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)hour/10));

			gdi_image_draw_id(g_vdorec_cntx.timer.hr_1_offset_x,
									g_vdorec_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)hour%10));
			
			gdi_image_draw_id(g_vdorec_cntx.timer.col_0_offset_x,
									g_vdorec_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOREC_OSD_TIMER_COL));
			
			}
			if(old_min != min)
				{
				/* min */
				gdi_image_draw_id(g_vdorec_cntx.timer.min_0_offset_x,
										g_vdorec_cntx.timer.offset_y, 
										(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)min/10));

				gdi_image_draw_id(g_vdorec_cntx.timer.min_1_offset_x,
										g_vdorec_cntx.timer.offset_y, 
										(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)min%10));
				
				gdi_image_draw_id(g_vdorec_cntx.timer.col_1_offset_x,
										g_vdorec_cntx.timer.offset_y, 
										(U16)(IMG_ID_VDOREC_OSD_TIMER_COL));
				}
			/* sec */
			if(sec/10 != old_sec/10)
				{
				gdi_image_draw_id(g_vdorec_cntx.timer.sec_0_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)sec/10));
				}

			gdi_image_draw_id(g_vdorec_cntx.timer.sec_1_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)sec%10));

			*is_ui_update = 1;
		}
		

}

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_draw_panel_timer
* DESCRIPTION
*  draw panel timer
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_draw_panel_timer(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 hour;
	S32 hour_remain;
	S32 min;
	S32 sec;
	U32 rec_time;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	rec_time = (U32)(g_vdorec_cntx.record_time/1000);

	/******** draw timer *********/	
	hour				= rec_time/3600;
	hour_remain		= rec_time%3600;
	min				= hour_remain/60;
	sec				= hour_remain%60;

#if 1 //qiff modify for VDORECAPP 20090305
	/* BG */
	gdi_layer_push_clip();
	gdi_layer_set_clip(g_vdorec_osd_cntx.panel.timer.offset_x-1,
					g_vdorec_osd_cntx.panel.timer.offset_y -2, 
					32, 16);
	gdi_image_draw_id(g_vdorec_osd_cntx.panel.timer.offset_x,
							g_vdorec_osd_cntx.panel.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_BG));
	gdi_layer_pop_clip();
	//mmi_trace(1,"guoj++ mmi_vdorec_draw_panel_timer g_vdorec_cntx.timer.offset_y =%d,g_vdorec_cntx.timer.hr_0_offset_x=%d",g_vdorec_cntx.timer.offset_y,g_vdorec_cntx.timer.hr_0_offset_x );
#endif //qiff modify end
	/* hr */
	gdi_image_draw_id(g_vdorec_cntx.timer.hr_0_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)hour/10));

	gdi_image_draw_id(g_vdorec_cntx.timer.hr_1_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)hour%10));
	
	gdi_image_draw_id(g_vdorec_cntx.timer.col_0_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_COL));
	
	/* min */
	gdi_image_draw_id(g_vdorec_cntx.timer.min_0_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)min/10));

	gdi_image_draw_id(g_vdorec_cntx.timer.min_1_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)min%10));
	
	gdi_image_draw_id(g_vdorec_cntx.timer.col_1_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_COL));

	/* sec */
	gdi_image_draw_id(g_vdorec_cntx.timer.sec_0_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)sec/10));

	gdi_image_draw_id(g_vdorec_cntx.timer.sec_1_offset_x,
							g_vdorec_cntx.timer.offset_y, 
							(U16)(IMG_ID_VDOREC_OSD_TIMER_0+(U16)sec%10));

}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_draw_panel
* DESCRIPTION
*  draw panel
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_draw_panel(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_vdorec_draw_panel_state();
	mmi_vdorec_draw_panel_ev();
	
	#ifdef __VDOREC_FEATURE_ZOOM__
		mmi_vdorec_draw_panel_zoom();
	#endif /* __VDOREC_FEATURE_ZOOM__ */
	
	mmi_vdorec_draw_panel_timer();	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_init_panel
* DESCRIPTION
*  init panel (timer)
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_init_panel(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 digit_width;
	S32 col_width;	
	S32 height;

	#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
		vdorec_ui_touch_struct	*touch_p;
	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* timer */
	gdi_image_get_dimension_id(IMG_ID_VDOREC_OSD_TIMER_COL, 	&col_width, &height);	
	gdi_image_get_dimension_id(IMG_ID_VDOREC_OSD_TIMER_0, 	&digit_width, &height);
#if 0	
	g_vdorec_cntx.timer.offset_y 			= g_vdorec_osd_cntx.panel.timer.offset_y + 6;
	
	g_vdorec_cntx.timer.hr_0_offset_x 	= g_vdorec_osd_cntx.panel.timer.offset_x + 5;	

#else
	g_vdorec_cntx.timer.offset_y 			= g_vdorec_osd_cntx.panel.timer.offset_y;
	
	g_vdorec_cntx.timer.hr_0_offset_x 	= g_vdorec_osd_cntx.panel.timer.offset_x ;	

#endif
	g_vdorec_cntx.timer.hr_1_offset_x	= g_vdorec_cntx.timer.hr_0_offset_x+digit_width;
	g_vdorec_cntx.timer.col_0_offset_x	= g_vdorec_cntx.timer.hr_1_offset_x+digit_width;
	g_vdorec_cntx.timer.min_0_offset_x	= g_vdorec_cntx.timer.col_0_offset_x+col_width;
	g_vdorec_cntx.timer.min_1_offset_x	= g_vdorec_cntx.timer.min_0_offset_x+digit_width;
	g_vdorec_cntx.timer.col_1_offset_x	= g_vdorec_cntx.timer.min_1_offset_x+digit_width;
	g_vdorec_cntx.timer.sec_0_offset_x	= g_vdorec_cntx.timer.col_1_offset_x+col_width;	
	g_vdorec_cntx.timer.sec_1_offset_x	= g_vdorec_cntx.timer.sec_0_offset_x+digit_width;	

	mmi_trace(1," mmi_vdorec_init_panel g_vdorec_cntx.timer.offset_y =%d,g_vdorec_cntx.timer.hr_0_offset_x=%d",g_vdorec_cntx.timer.offset_y,g_vdorec_cntx.timer.hr_0_offset_x );
	#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
		g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;
	
		/* init touch region */
		
		/* ev inc */
		touch_p = &g_vdorec_cntx.touch_ev_inc;
		touch_p->offset_x = g_vdorec_osd_cntx.touch_ev_inc.offset_x;
		touch_p->offset_y = g_vdorec_osd_cntx.touch_ev_inc.offset_y;		
		touch_p->is_press = FALSE;
		gdi_image_get_dimension_id(IMG_ID_VDOREC_TOUCH_OSD_EV_INC, &touch_p->width, &touch_p->height);
	
		/* ev dec */
		touch_p = &g_vdorec_cntx.touch_ev_dec;
		touch_p->offset_x = g_vdorec_osd_cntx.touch_ev_dec.offset_x;
		touch_p->offset_y = g_vdorec_osd_cntx.touch_ev_dec.offset_y;		
		touch_p->is_press = FALSE;		
		gdi_image_get_dimension_id(IMG_ID_VDOREC_TOUCH_OSD_EV_DEC, &touch_p->width, &touch_p->height);

		/* zoom inc */
		touch_p = &g_vdorec_cntx.touch_zoom_inc;
		touch_p->offset_x = g_vdorec_osd_cntx.touch_zoom_inc.offset_x;
		touch_p->offset_y = g_vdorec_osd_cntx.touch_zoom_inc.offset_y;		
		touch_p->is_press = FALSE;		
		gdi_image_get_dimension_id(IMG_ID_VDOREC_TOUCH_OSD_ZOOM_INC, &touch_p->width, &touch_p->height);

		/* zoom dec */
		touch_p = &g_vdorec_cntx.touch_zoom_dec;
		touch_p->offset_x = g_vdorec_osd_cntx.touch_zoom_dec.offset_x;
		touch_p->offset_y = g_vdorec_osd_cntx.touch_zoom_dec.offset_y;
		touch_p->is_press = FALSE;		
		gdi_image_get_dimension_id(IMG_ID_VDOREC_TOUCH_OSD_ZOOM_DEC, &touch_p->width, &touch_p->height);

		/* play */
		touch_p = &g_vdorec_cntx.touch_rec;
		touch_p->offset_x = g_vdorec_osd_cntx.touch_rec.offset_x;
		touch_p->offset_y = g_vdorec_osd_cntx.touch_rec.offset_y;
		touch_p->is_press = FALSE;
//		gdi_image_get_dimension_id(IMG_ID_VDOREC_TOUCH_OSD_REC, &touch_p->width, &touch_p->height);
		gdi_image_get_dimension_id(IMG_ID_VDOREC_OSD_STATE_REC, &touch_p->width, &touch_p->height);

		/* stop */
		touch_p = &g_vdorec_cntx.touch_stop;
		touch_p->offset_x = g_vdorec_osd_cntx.touch_stop.offset_x;
		touch_p->offset_y = g_vdorec_osd_cntx.touch_stop.offset_y;
		touch_p->is_press = FALSE;
		gdi_image_get_dimension_id(IMG_ID_VDOREC_TOUCH_OSD_STOP, &touch_p->width, &touch_p->height);

	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */
	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_draw_style_text
* DESCRIPTION
*  draw styled text
* PARAMETERS
*	 a	IN		string
*	 b IN	 	offset x
*	 c IN		offset y
*	 d IN    text style
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_draw_style_text(PS8 str, S32 offset_x, S32 offset_y, video_style_text_struct *style_txt)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(r2lMMIFlag)
	{
		offset_x += coolsand_UI_get_string_width((UI_string_type)str);
	}
	
	if(style_txt->style == VIDEO_TEXT_STYLE_NORMAL)
	{
		coolsand_UI_move_text_cursor(offset_x, offset_y);
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->r, style_txt->g ,style_txt->b));
	  	coolsand_UI_print_text((UI_string_type)str);	
	}
	else if(style_txt->style == VIDEO_TEXT_STYLE_BORDER)
	{
		coolsand_UI_move_text_cursor(offset_x, offset_y);
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->r, style_txt->g, style_txt->b));
		coolsand_UI_set_text_border_color(coolsand_UI_color(style_txt->style_r, style_txt->style_g, style_txt->style_b));
	   coolsand_UI_print_bordered_text((UI_string_type)str);
	}
	else if(style_txt->style == VIDEO_TEXT_STYLE_SHADOW)
	{
		coolsand_UI_move_text_cursor(offset_x+1, offset_y+1);
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->style_r, style_txt->style_g ,style_txt->style_b));
	   coolsand_UI_print_text((UI_string_type)str);	

		coolsand_UI_move_text_cursor(offset_x, offset_y);	
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->r, style_txt->g ,style_txt->b));
	   coolsand_UI_print_text((UI_string_type)str);	
	}
	else
	{
		MMI_ASSERT(0);
	} 
	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_hide_hint
* DESCRIPTION
*  Hide hint 
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_hide_hint(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_vdorec_set_hint(NULL, NULL, 0);
	mmi_vdorec_draw_osd();
//	mmi_vdorec_blt_screen();	
	mmi_vdorec_blt_hint_screen();
}
	


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_save_confirm_no
* DESCRIPTION
*  enter save screen
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_draw_hint(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 	str1_width; 
	S32 	str1_height;
	S32 	str2_width; 
	S32 	str2_height;
	S32 	str1_offset_x = 0;
	S32 	str1_offset_y= 0;
	S32 	str2_offset_x= 0;
	S32 	str2_offset_y= 0;
	S32	spacing;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(!g_vdorec_osd_cntx.hint.is_show)
		return;
   
	if(g_vdorec_cntx.hint1_buf != NULL)
	{

		coolsand_UI_measure_string((UI_string_type)g_vdorec_cntx.hint1_buf, &str1_width, &str1_height); 

		if(g_vdorec_cntx.hint2_buf == NULL)
		{
			str1_offset_x = (g_vdorec_osd_cntx.hint.rect.width - str1_width)>>1;
			str1_offset_y = (g_vdorec_osd_cntx.hint.rect.height - str1_height)>>1;	
		}
		else
		{
			coolsand_UI_measure_string((UI_string_type)g_vdorec_cntx.hint2_buf, &str2_width, &str2_height); 

			str1_offset_x = (g_vdorec_osd_cntx.hint.rect.width - str1_width)>>1;

			spacing = g_vdorec_osd_cntx.hint.rect.height-str1_height-str2_height;
			spacing = (spacing > 0) ? (spacing/3+1) : 0;
			
			str1_offset_y = spacing;	

			str2_offset_x = (g_vdorec_osd_cntx.hint.rect.width - str2_width)>>1;
			str2_offset_y = spacing*2 + 1 + str1_height;
		}

		str1_offset_x += g_vdorec_osd_cntx.hint.rect.offset_x;
		str1_offset_y += g_vdorec_osd_cntx.hint.rect.offset_y;
		str2_offset_x += g_vdorec_osd_cntx.hint.rect.offset_x;
		str2_offset_y += g_vdorec_osd_cntx.hint.rect.offset_y;	

		gdi_layer_reset_clip();
		coolsand_UI_reset_text_clip();

		mmi_vdorec_draw_style_text(g_vdorec_cntx.hint1_buf, 
											str1_offset_x,
											str1_offset_y,
											(video_style_text_struct *)&g_vdorec_osd_cntx.hint.style_text);		
											
			
		if(g_vdorec_cntx.hint2_buf != NULL)
		{
			mmi_vdorec_draw_style_text(g_vdorec_cntx.hint2_buf, 
												str2_offset_x,
												str2_offset_y,
												(video_style_text_struct *)&g_vdorec_osd_cntx.hint.style_text);	
		}

		if(g_vdorec_cntx.fade_time != 0)
			coolsand_UI_start_timer(HINT_POPUP_FADE_TIME, mmi_vdorec_hide_hint);
		
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_set_hint()
* DESCRIPTION
*   set hint string
* PARAMETERS
*	 hint_str1		IN		string 1
*	 hint_str2		IN 	string 2
*	 fade_time 		IN 	hint fade out time
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_vdorec_set_hint(PS8 hint_str1, PS8 hint_str2, U16 fade_time)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* clear buffer */
	memset(g_vdorec_cntx.hint1_buf, 0, VDOREC_HINT_BUF_SIZE);
	memset(g_vdorec_cntx.hint2_buf, 0, VDOREC_HINT_BUF_SIZE);
	
	if(hint_str1 != NULL)
	{
		pfnUnicodeStrncpy((PS8)g_vdorec_cntx.hint1_buf, (PS8)hint_str1, VDOREC_HINT_CHAR_COUNT);
	}

	if(hint_str2 != NULL)
	{
		pfnUnicodeStrncpy((PS8)g_vdorec_cntx.hint2_buf, (PS8)hint_str2, VDOREC_HINT_CHAR_COUNT);
	}

	g_vdorec_cntx.fade_time = fade_time;

}


/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_draw_osd()
* DESCRIPTION
*   draw osd 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_vdorec_draw_osd(void)
{
  	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	gdi_layer_push_and_set_active(g_vdorec_cntx.osd_layer_handle);

	/* toggle double buffer */
	gdi_layer_toggle_double();

	/* clear bg */
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);

	/* drawing */
	mmi_vdorec_draw_status();
	mmi_vdorec_draw_hint();

	gdi_layer_pop_and_restore_active();
}



/*****************************************************************************
*
*	Save Screen
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_save_confirm_no
* DESCRIPTION
*  enter save screen
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_save_confirm_no(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8		buf_filepath[FMGR_PATH_BUFFER_SIZE];	
   S16 	error;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* delete unsaved file */
	mmi_vdorec_get_storage_file_path(buf_filepath);
	mdi_video_rec_delete_unsaved_file(buf_filepath);

	/* the seq num is already increase, so if we dont want to save this file, decrease it */
	ReadValue(NVRAM_VDOREC_FILENAME_SEQ_NO, &g_vdorec_cntx.filename_seq_no, DS_SHORT , &error);
   
	if(g_vdorec_cntx.filename_seq_no > 1)
	{
		g_vdorec_cntx.filename_seq_no--;
	}

	WriteValue(NVRAM_VDOREC_FILENAME_SEQ_NO, &g_vdorec_cntx.filename_seq_no, DS_SHORT , &error);

	GoBackHistory();
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_save_confirm_screen
* DESCRIPTION
*  enter save confirm screen
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_entry_save_confirm_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
	
		DisplayConfirmRotated(STR_GLOBAL_YES, IMG_GLOBAL_YES,
									STR_GLOBAL_NO, IMG_GLOBAL_NO,
									get_string(STR_ID_VDOREC_NOTIFY_SAVE_CONFIRM), 
									IMG_GLOBAL_QUESTION, WARNING_TONE,
									MMI_FRM_SCREEN_ROTATE_270); 	
	
	#else
	
		DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
							STR_GLOBAL_NO, IMG_GLOBAL_NO,
							get_string(STR_ID_VDOREC_NOTIFY_SAVE_CONFIRM), 
							IMG_GLOBAL_QUESTION, WARNING_TONE); 
	
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
   


	SetRightSoftkeyFunction(mmi_vdorec_save_confirm_no,KEY_EVENT_UP);	
	SetLeftSoftkeyFunction(mmi_vdorec_entry_saving_screen,KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_save_result_hdlr_callback
* DESCRIPTION
*  callback function when save is done 
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_save_result_hdlr_callback(MDI_RESULT result)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	DeleteScreenIfPresent(SCR_ID_VDOREC_SAVING);

	g_vdorec_cntx.is_saving	= FALSE;
		mmi_trace(1,"mmi_vdorec_save_result_hdlr_callback");

	if(GetTotalCallCount() == 0 || GetWapCallPresent())
	{
		if(result >= 0)
		{
			mmi_vdorec_disply_popup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
		}	
		else if(result == MDI_RES_VDOREC_ERR_DISK_FULL)
		{
			mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
		}
		else
		{
			mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOREC_NOTIFY_SAVE_FILE_ERROR), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		}
		mmi_trace(1,"mmi_vdorec_save_result_hdlr_callback,display,(IsClamClose() is %d",IsClamClose());
		DeleteUptoScrID(SCR_ID_VDOREC_APP);
#if 1
		/* if clam is close, delete on more screen, avoid enter video recorder */
		if(IsClamClose()) //chenhe delete for jasperII test
			DeleteNHistory(1);
#endif		
//		mmi_vdorec_enter_state(VDOREC_STATE_PREVIEW);

	}
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_exit_saving_screen
* DESCRIPTION
*  enter save screen
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_exit_saving_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* resume background play */
	mdi_audio_resume_background_play();
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_saving_screen_del_hdlr
* DESCRIPTION
*  Del screen callback function
* PARAMETERS
*	nil
* RETURNS
*  U8
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static U8 mmi_vdorec_saving_screen_del_hdlr(void* dummy_ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdorec_cntx.is_saving)
		return TRUE; /* if is saving, will not delete this screen from history */
	else
		return FALSE;
	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_saving_screen
* DESCRIPTION
*  enter save screen
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/


static void mmi_vdorec_entry_saving_screen_copy(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 *guiBuffer;
	S8	buf_filepath[FMGR_PATH_BUFFER_SIZE];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   mmi_trace(1,"mmi_vdorec_entry_saving_screen");
	g_vdorec_cntx.is_saving	= TRUE;
	SetDelScrnIDCallbackHandler(SCR_ID_VDOREC_SAVING, mmi_vdorec_saving_screen_del_hdlr);
	
	/* suspend background play */
	mdi_audio_suspend_background_play();

	EntryNewScreen(SCR_ID_VDOREC_SAVING, mmi_vdorec_exit_saving_screen, mmi_vdorec_entry_saving_screen, NULL);

	/* rotate UI */
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
/*
   ShowCategory66Screen(	STR_ID_VDOREC_APP_NAME, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
	                        0, 0, 0, 0,
	                        (PU8)GetString(STR_ID_VDOREC_NOTIFY_SAVING),
	                        IMG_GLOBAL_PROGRESS, NULL);
*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_VDOREC_SAVING);	

	if(guiBuffer == NULL)
	{
		/* check is entry from vdorec app */
		mmi_vdorec_get_storage_file_path(buf_filepath);

		g_vdorec_cntx.last_error = mdi_video_rec_save_file(buf_filepath, mmi_vdorec_save_result_hdlr_callback);//for jasperII

		if(mmi_vdorec_check_and_display_error_popup() == TRUE) /* TRUE means has error */
		{
			/* if has error, cant save, delete temp file */	
			mdi_video_rec_delete_unsaved_file(buf_filepath); 
			return;
		}
//		mmi_vdorec_save_result_hdlr_callback(1);//chenhe for jasperII
	}
	
	/* clear end key, this action cant be abort */
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
}

//chenhe add for new requrement,081210
void vdorec_delete_RecFile(VOID)
{
    mmi_trace(1,"chenhe,delete the recorded File");
    MMI_FS_Delete(buf_filename);
}
static void mmi_vdorec_cancel_save(void)
{
	vdorec_delete_RecFile();
	GoBackHistory();
	
}
//#include "factorymodedef.h" //just to use strid str_id_fm_camera_save_confrim
static void mmi_vdo_save_confirm(void)
{
	DisplayConfirm((U16)STR_GLOBAL_SAVE,0,(U16)STR_GLOBAL_CLEAR,0,(UI_string_type)GetString(STR_ID_VDOREC_NOTIFY_SAVE_CONFIRM),IMG_GLOBAL_QUESTION,0);
	SetLeftSoftkeyFunction(mmi_vdorec_entry_saving_screen_copy, KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_vdorec_cancel_save, KEY_EVENT_UP);
}

static void mmi_vdorec_entry_saving_screen(void)
{
	mmi_vdo_save_confirm();
}



/*****************************************************************************
*
* Menu Screens
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_init_option_menu
* DESCRIPTION
*  int option menu hdlr
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_init_option_menu(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetHiliteHandler(MENU_ID_VDOREC_TO_VDOPLY,			mmi_vdorec_highlight_to_vdoply);
	SetHiliteHandler(MENU_ID_VDOREC_CAMCODER_SETTING,	mmi_vdorec_highlight_camcoder_setting);

	#ifdef __VDOREC_FEATURE_MERGE_MENU__//changde by zhangl for #ifndef --> #ifdef
		SetHiliteHandler(MENU_ID_VDOREC_VIDEO_SETTING, mmi_vdorec_highlight_video_setting);
	#endif /* !__VDOREC_FEATURE_MERGE_MENU__ */

	#ifdef __VDOREC_FEATURE_EFFECT__
		SetHiliteHandler(MENU_ID_VDOREC_EFFECT_SETTING,	mmi_vdorec_highlight_effect_setting);
//		SetHintHandler(MENU_ID_VDOREC_EFFECT_SETTING,		mmi_vdorec_hint_effect_setting);
	#endif /* __VDOREC_FEATURE_EFFECT__ */

	#ifdef __VDOREC_FEATURE_STORAGE__
		SetHiliteHandler(MENU_ID_VDOREC_STORAGE, mmi_vdorec_highlight_storage);
		SetHintHandler(MENU_ID_VDOREC_STORAGE,	mmi_vdorec_hint_storage);	
	#endif /* __VDOREC_FEATURE_STORAGE__ */
	
	SetHiliteHandler(MENU_ID_VDOREC_RESTORE_DEFAULT,	mmi_vdorec_highlight_restore_default);
 
}


	
/*****************************************************************************
* FUNCTION
*	mmi_vdorec_highlight_to_vdoply
* DESCRIPTION
*  hiligith to vodply menuitem
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_highlight_to_vdoply(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_vdoply_entry_app,KEY_EVENT_UP);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_vdoply_entry_app, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_option_screen
* DESCRIPTION
*  entry option menu
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_entry_option_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	ItemList[16];
	U16	ItemIcons[16];
	U16	nItems;
	U8		*guiBuffer;
	U8		*PopUpList[32];
	S32	i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   	mmi_trace(g_sw_VdoRec," mmi_vdorec_entry_option_screen");
	EntryNewScreen(SCR_ID_VDOREC_OPTION, NULL, mmi_vdorec_entry_option_screen, NULL);

	guiBuffer	= GetCurrGuiBuffer(SCR_ID_VDOREC_OPTION);
	nItems		= GetNumOfChild(MENU_ID_VDOREC_OPTION);
	GetSequenceStringIds(MENU_ID_VDOREC_OPTION,ItemList);
	GetSequenceImageIds(MENU_ID_VDOREC_OPTION,ItemIcons);
	SetParentHandler(MENU_ID_VDOREC_OPTION);
	mmi_trace(1,"mmi_vdorec_entry_option_screen,nItems is %d,MENU_ID_VDOREC_OPTION is %d",nItems,MENU_ID_VDOREC_OPTION);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
		PopUpList[i]=NULL;

	ConstructHintsList(MENU_ID_VDOREC_OPTION, PopUpList);

	/* rotate UI */
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

	ShowCategory52Screen(	STR_GLOBAL_OPTIONS,		MAIN_MENU_TITLE_MULTIMEDIA_ICON,
									STR_GLOBAL_OK,				IMG_GLOBAL_OK,
									STR_GLOBAL_BACK,			IMG_GLOBAL_BACK,
									nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,	KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_highlight_camcoder_setting
* DESCRIPTION
*  highlight camecoder setting menuitem
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_highlight_camcoder_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_vdorec_entry_camcoder_setting_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_vdorec_entry_camcoder_setting_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_camcoder_setting_lsk_press
* DESCRIPTION
*  camcoder setting lsk press
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_camcoder_setting_lsk_press(void)
{

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#define CAMCODER_SETTING_SELECTED_VALUE(a, A) \
	{\
		g_vdorec_cntx.setting.a = g_vdorec_menu_cntx.camcoder_setting_inline.a##_selected;\
	}

	#ifdef __VDOREC_FEATURE_WB__
		CAMCODER_SETTING_SELECTED_VALUE(wb, WB);
	#endif

	#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT__
		CAMCODER_SETTING_SELECTED_VALUE(led_highlight, LED_HIGHLIGHT);
	#endif 

	#ifdef __VDOREC_FEATURE_EV__
		CAMCODER_SETTING_SELECTED_VALUE(ev, EV);
	#endif
	
	#ifdef __VDOREC_FEATURE_CT__ //add by zhangl for greenstone
		CAMCODER_SETTING_SELECTED_VALUE(contrast, CT);
	#endif
	
	#ifdef __VDOREC_FEATURE_NIGHT__
		CAMCODER_SETTING_SELECTED_VALUE(night, NIGHT);
	#endif

	#ifdef __VDOREC_FEATURE_BANDING__
		CAMCODER_SETTING_SELECTED_VALUE(banding, BANDING);

	#endif

	#ifdef __VDOREC_FEATURE_MERGE_MENU__
	
		/* merge "Video Setting" items into "CamCoder Setting" */
		#ifdef __VDOREC_FEATURE_VIDEO_SIZE__
			CAMCODER_SETTING_SELECTED_VALUE(video_size, VIDEO_SIZE);

		#endif /* __VDOREC_FEATURE_VIDEO_SIZE__ */
	
		#ifdef __VDOREC_FEATURE_VIDEO_QTY__
			CAMCODER_SETTING_SELECTED_VALUE(video_qty, VIDEO_QTY);
		#endif /* __VDOREC_FEATURE_VIDEO_QTY__ */

		#ifdef __VDOREC_FEATURE_SIZE_LIMIT__
			CAMCODER_SETTING_SELECTED_VALUE(size_limit, SIZE_LIMIT);

		#endif /* __VDOREC_FEATURE_SIZE_LIMIT__ */
		
		#ifdef __VDOREC_FEATURE_TIME_LIMIT__
			CAMCODER_SETTING_SELECTED_VALUE(time_limit, TIME_LIMIT);
		#endif /* __VDOREC_FEATURE_TIME_LIMIT__ */

		#ifdef __VDOREC_FEATURE_RECORD_AUD__
			CAMCODER_SETTING_SELECTED_VALUE(record_aud, RECORD_AUD);
		#endif /* __VDOREC_FEATURE_RECORD_AUD__ */

		#ifdef __VDOREC_FEATURE_VIDEO_FORMAT__
			CAMCODER_SETTING_SELECTED_VALUE(video_format, VIDEO_FORMAT);
		#endif /* __VDOREC_FEATURE_VIDEO_FORMAT__ */
		
	#endif /* __VDOREC_FEATURE_MERGE_MENU__ */
	


	
	mmi_vdorec_store_setting();

	mmi_vdorec_disply_popup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	DeleteNHistory(1);	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_camcoder_setting_highlight_hdlr
* DESCRIPTION
*  camcoder setting highlight handler
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_camcoder_setting_highlight_hdlr(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(mmi_vdorec_camcoder_setting_lsk_press);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_exit_camcoder_setting_screen
* DESCRIPTION
*  exit camcoder setting screen (inline selection screen)
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_exit_camcoder_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 		inputBufferSize;                
	history_t 	h;
	S16		nHistory = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	CloseCategory57Screen();       
	h.scrnID			= SCR_ID_VDOREC_CAMCODER_SETTING;
	h.entryFuncPtr = mmi_vdorec_entry_camcoder_setting_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	inputBufferSize = (U16)GetCategory57DataSize(); 
	GetCategory57History (h.guiBuffer);
	GetCategory57Data ((U8*)h.inputBuffer);       
	AddNHistory(h, inputBufferSize);          

}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_camcoder_setting_screen
* DESCRIPTION
*  entry camcoder setting screen (inline selection screen)
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_entry_camcoder_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		*guiBuffer;
	U8		*inputBuffer;
	U16	inputBufferSize;
	S32	item_offset = 0;
	S32	i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	UI_image_ID_type item_icons[VDOREC_CAMCODER_SETTING_ITEM_COUNT*2] =
	{
		#ifdef __VDOREC_FEATURE_WB__
			IMG_ID_VDOREC_WB,
			0,
		#endif

		#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT__
			IMG_ID_VDOREC_LED_HIGHLIGHT,
			0,
		#endif

		#ifdef __VDOREC_FEATURE_EV__
			IMG_ID_VDOREC_BT,
			0,
		#endif

		#ifdef __VDOREC_FEATURE_NIGHT__
			IMG_ID_VDOREC_NIGHT,
			0,		
		#endif
	// add contrast by zhangl for greestone
		#ifdef __VDOREC_FEATURE_CT__
			IMG_ID_VDOREC_CT,
			0,		
		#endif

		#ifdef __VDOREC_FEATURE_BANDING__
			IMG_ID_VDOREC_BANDING,
			0,		
		#endif

		#ifdef __VDOREC_FEATURE_MERGE_MENU__
		
		/* merge "Video Setting" items into "CamCoder Setting" */
			#ifdef __VDOREC_FEATURE_VIDEO_SIZE__
				IMG_ID_VDOREC_VIDEO_SIZE,
				0,
			#endif /* __VDOREC_FEATURE_VIDEO_SIZE__ */
		
			#ifdef __VDOREC_FEATURE_VIDEO_QTY__
				IMG_ID_VDOREC_VIDEO_QTY,
				0,
			#endif /* __VDOREC_FEATURE_VIDEO_QTY__ */
			
			#ifdef __VDOREC_FEATURE_SIZE_LIMIT__	
				IMG_ID_VDOREC_SIZE_LIMIT,
				0,
			#endif /* __VDOREC_FEATURE_SIZE_LIMIT__ */
			
			#ifdef __VDOREC_FEATURE_TIME_LIMIT__
				IMG_ID_VDOREC_TIME_LIMIT,
				0,
			#endif /* __VDOREC_FEATURE_TIME_LIMIT__ */

			#ifdef __VDOREC_FEATURE_RECORD_AUD__
				IMG_ID_VDOREC_RECORD_AUD,
				0,		
			#endif /* __VDOREC_FEATURE_RECORD_AUD__ */

			#ifdef __VDOREC_FEATURE_VIDEO_FORMAT__
				IMG_ID_VDOREC_VIDEO_FORMAT,
				0,	
			#endif /* __VDOREC_FEATURE_VIDEO_FORMAT__ */
			
		#endif /* __VDOREC_FEATURE_MERGE_MENU__ */
	};

	EntryNewScreen(SCR_ID_VDOREC_CAMCODER_SETTING, mmi_vdorec_exit_camcoder_setting_screen, NULL, NULL);

	InitializeCategory57Screen();	

	/******************************************************************
	*	MACRO to create inline selection. 
	******************************************************************/
	#define CREATE_CAMCODER_SETTING_INLINE_SELECT(a, A) \
	{\
		g_vdorec_menu_cntx.camcoder_setting_inline.a##_selected = g_vdorec_cntx.setting.a;\
		for(i = 0 ; i < VDOREC_SETTING_##A##_COUNT; i++)\
		{\
			g_vdorec_menu_cntx.camcoder_setting_inline.a##_str[i] = (UI_string_type)GetString((U16)(STR_ID_VDOREC_##A##_START+i+1));\
		}\
		SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_VDOREC_##A)));\
		SetInlineItemActivation(&wgui_inline_items[item_offset], \
										INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, \
										0 );\
		SetInlineItemSelect(	&wgui_inline_items[item_offset++], \
									VDOREC_SETTING_##A##_COUNT, \
									(U8**)g_vdorec_menu_cntx.camcoder_setting_inline.a##_str, \
									&g_vdorec_menu_cntx.camcoder_setting_inline.a##_selected);\
	}

	/****************************************************************************
	*  Create inline selection                                                               
	*****************************************************************************/

	#ifdef __VDOREC_FEATURE_WB__
		CREATE_CAMCODER_SETTING_INLINE_SELECT(wb, WB);
	#endif 
	#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT__
		CREATE_CAMCODER_SETTING_INLINE_SELECT(led_highlight, LED_HIGHLIGHT);
	#endif

	#ifdef __VDOREC_FEATURE_EV__
		CREATE_CAMCODER_SETTING_INLINE_SELECT(ev, EV);
	#endif
	
	#ifdef __VDOREC_FEATURE_CT__//added by zhangl for greenstone
		CREATE_CAMCODER_SETTING_INLINE_SELECT(contrast, CONTRAST);
	#endif
	
	#ifdef __VDOREC_FEATURE_NIGHT__
		CREATE_CAMCODER_SETTING_INLINE_SELECT(night, NIGHT);
	#endif

	#ifdef __VDOREC_FEATURE_BANDING__
		CREATE_CAMCODER_SETTING_INLINE_SELECT(banding, BANDING);	
	#endif

	#ifdef __VDOREC_FEATURE_MERGE_MENU__
	
		/* merge "Video Setting" items into "CamCoder Setting" */
		#ifdef __VDOREC_FEATURE_VIDEO_SIZE__
			CREATE_CAMCODER_SETTING_INLINE_SELECT(video_size, VIDEO_SIZE);
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE__ */
	
		#ifdef __VDOREC_FEATURE_VIDEO_QTY__
			CREATE_CAMCODER_SETTING_INLINE_SELECT(video_qty, VIDEO_QTY);
		#endif /* __VDOREC_FEATURE_VIDEO_QTY__ */

		#ifdef __VDOREC_FEATURE_SIZE_LIMIT__
			CREATE_CAMCODER_SETTING_INLINE_SELECT(size_limit, SIZE_LIMIT);
		#endif /* __VDOREC_FEATURE_SIZE_LIMIT__ */
		
		#ifdef __VDOREC_FEATURE_TIME_LIMIT__
			CREATE_CAMCODER_SETTING_INLINE_SELECT(time_limit, TIME_LIMIT);
		#endif /* __VDOREC_FEATURE_TIME_LIMIT__ */

		#ifdef __VDOREC_FEATURE_RECORD_AUD__
			CREATE_CAMCODER_SETTING_INLINE_SELECT(record_aud, RECORD_AUD);
		#endif /* __VDOREC_FEATURE_RECORD_AUD__ */

		#ifdef __VDOREC_FEATURE_VIDEO_FORMAT__
			CREATE_CAMCODER_SETTING_INLINE_SELECT(video_format, VIDEO_FORMAT);
		#endif /* __VDOREC_FEATURE_VIDEO_FORMAT__ */
		
	#endif /* __VDOREC_FEATURE_MERGE_MENU__ */

	guiBuffer	= GetCurrGuiBuffer( SCR_ID_VDOREC_CAMCODER_SETTING );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_VDOREC_CAMCODER_SETTING, &inputBufferSize );
	
	if(inputBuffer != NULL)		
		SetCategory57Data(wgui_inline_items, VDOREC_CAMCODER_SETTING_ITEM_COUNT*2, inputBuffer);	

	RegisterHighlightHandler( mmi_vdorec_camcoder_setting_highlight_hdlr );

	/* rotate UI */
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

	/* show category */
	ShowCategory57Screen(STR_ID_VDOREC_CAMCODER_SETTING, MAIN_MENU_TITLE_MULTIMEDIA_ICON,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								wgui_inline_items,
								0, guiBuffer);

	/* set key hdlr */
	SetCategory57LeftSoftkeyFunction( mmi_vdorec_camcoder_setting_lsk_press );
	DisableCategory57ScreenDone();
	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );

}




/*****************************************************************************
* FUNCTION
*	mmi_vdorec_highlight_video_setting
* DESCRIPTION
*  highlight video setting 
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_MERGE_MENU__//changde by zhangl for #ifndef --> #ifdef
static void mmi_vdorec_highlight_video_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_vdorec_entry_video_setting_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_vdorec_entry_video_setting_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}
#endif /* !__VDOREC_FEATURE_MERGE_MENU__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_video_setting_lsk_press
* DESCRIPTION
*  video setting lsk pressed
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifndef __VDOREC_FEATURE_MERGE_MENU__
static void mmi_vdorec_video_setting_lsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#define VIDEO_SETTING_SET_SELECTED_VALUE(a, A) \
	{\
		g_vdorec_cntx.setting.a = g_vdorec_menu_cntx.video_setting_inline.a##_selected;\
	}
#if 0
		g_vdorec_cntx.setting.video_size = g_vdorec_menu_cntx.video_setting_inline.video_size_selected;
		g_vdorec_cntx.setting.video_qty = g_vdorec_menu_cntx.video_setting_inline.video_qty_selected;
		g_vdorec_cntx.setting.size_limit = g_vdorec_menu_cntx.video_setting_inline.size_limit_selected;
		g_vdorec_cntx.setting.time_limit = g_vdorec_menu_cntx.video_setting_inline.time_limit_selected;
		g_vdorec_cntx.setting.record_aud = g_vdorec_menu_cntx.video_setting_inline.record_aud_selected;
		g_vdorec_cntx.setting.video_format= g_vdorec_menu_cntx.video_setting_inline.video_format_selected;
#else
	VIDEO_SETTING_SET_SELECTED_VALUE(video_size, VIDEO_SIZE);
	VIDEO_SETTING_SET_SELECTED_VALUE(video_qty, VIDEO_QTY);
	VIDEO_SETTING_SET_SELECTED_VALUE(size_limit, SIZE_LIMIT);
	VIDEO_SETTING_SET_SELECTED_VALUE(time_limit, TIME_LIMIT);
	VIDEO_SETTING_SET_SELECTED_VALUE(record_aud, RECORD_AUD);
	VIDEO_SETTING_SET_SELECTED_VALUE(video_format, VIDEO_FORMAT);
#endif	
	mmi_vdorec_store_setting();

	mmi_vdorec_disply_popup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	DeleteNHistory(1);	
}
#endif /* !__VDOREC_FEATURE_MERGE_MENU__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_video_setting_highlight_hdlr
* DESCRIPTION
*  video setting highlight hdlr
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifndef __VDOREC_FEATURE_MERGE_MENU__
static void mmi_vdorec_video_setting_highlight_hdlr(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(mmi_vdorec_video_setting_lsk_press);
}
#endif /* !__VDOREC_FEATURE_MERGE_MENU__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_exit_video_setting_screen
* DESCRIPTION
*  exit video setting screen (inline selection)
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifndef __VDOREC_FEATURE_MERGE_MENU__
static void mmi_vdorec_exit_video_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 		inputBufferSize;                
	history_t 	h;
	S16		nHistory = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	CloseCategory57Screen();       
	h.scrnID			= SCR_ID_VDOREC_VIDEO_SETTING;
	h.entryFuncPtr = mmi_vdorec_entry_video_setting_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	inputBufferSize = (U16)GetCategory57DataSize(); 
	GetCategory57History (h.guiBuffer);
	GetCategory57Data ((U8*)h.inputBuffer);       
	AddNHistory(h, inputBufferSize);          
}
#endif /* !__VDOREC_FEATURE_MERGE_MENU__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_video_setting_screen
* DESCRIPTION
*  entry video setting screen (inline selection)
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifndef __VDOREC_FEATURE_MERGE_MENU__
static void mmi_vdorec_entry_video_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		*guiBuffer;
	U8		*inputBuffer;
	U16	inputBufferSize;
	S32	item_offset = 0;
	S32	i;

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	UI_image_ID_type item_icons[VDOREC_VIDEO_SETTING_ITEM_COUNT*2] =
	{
		#ifdef __VDOREC_FEATURE_VIDEO_SIZE__
			IMG_ID_VDOREC_VIDEO_SIZE,
			0,
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE__ */
		
		#ifdef __VDOREC_FEATURE_VIDEO_QTY__
			IMG_ID_VDOREC_VIDEO_QTY,
			0,
		#endif /* __VDOREC_FEATURE_VIDEO_QTY__ */
		
		#ifdef __VDOREC_FEATURE_SIZE_LIMIT__	
			IMG_ID_VDOREC_SIZE_LIMIT,
			0,
		#endif /* __VDOREC_FEATURE_SIZE_LIMIT__ */
		
		#ifdef __VDOREC_FEATURE_TIME_LIMIT__
			IMG_ID_VDOREC_TIME_LIMIT,
			0,
		#endif /* __VDOREC_FEATURE_TIME_LIMIT__ */

		#ifdef __VDOREC_FEATURE_RECORD_AUD__
			IMG_ID_VDOREC_RECORD_AUD,
			0,		
		#endif /* __VDOREC_FEATURE_RECORD_AUD__ */

		#ifdef __VDOREC_FEATURE_VIDEO_FORMAT__
			IMG_ID_VDOREC_VIDEO_FORMAT,
			0,	
		#endif /* __VDOREC_FEATURE_VIDEO_FORMAT__ */
	};

	EntryNewScreen(SCR_ID_VDOREC_VIDEO_SETTING, mmi_vdorec_exit_video_setting_screen, NULL, NULL);

	InitializeCategory57Screen();	
	
	/******************************************************************
	*	MACRO to create inline selection. 
	******************************************************************/
	#define CREATE_VIDEO_SETTING_INLINE_SELECT(a, A) \
	{\
		g_vdorec_menu_cntx.video_setting_inline.a##_selected = g_vdorec_cntx.setting.a;\
		for(i = 0 ; i < VDOREC_SETTING_##A##_COUNT; i++)\
		{\
			g_vdorec_menu_cntx.video_setting_inline.a##_str[i] = (UI_string_type)GetString((U16)(STR_ID_VDOREC_##A##_START+1+i));\
		}\
		SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_VDOREC_##A)));\
		SetInlineItemActivation(&wgui_inline_items[item_offset], \
										INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, \
										0 );\
		SetInlineItemSelect(	&wgui_inline_items[item_offset++], \
									VDOREC_SETTING_##A##_COUNT, \
									(U8**)g_vdorec_menu_cntx.video_setting_inline.a##_str, \
									&g_vdorec_menu_cntx.video_setting_inline.a##_selected);\
	}

 
	/****************************************************************************
	*  Create inline selection                                                               
	*****************************************************************************/
	#ifdef __VDOREC_FEATURE_VIDEO_SIZE__
		CREATE_VIDEO_SETTING_INLINE_SELECT(video_size, VIDEO_SIZE);
	#endif /* __VDOREC_FEATURE_VIDEO_SIZE__ */

	#ifdef __VDOREC_FEATURE_VIDEO_QTY__
		CREATE_VIDEO_SETTING_INLINE_SELECT(video_qty, VIDEO_QTY);
	#endif /* __VDOREC_FEATURE_VIDEO_QTY__ */	


	#ifdef __VDOREC_FEATURE_SIZE_LIMIT__
		CREATE_VIDEO_SETTING_INLINE_SELECT(size_limit, SIZE_LIMIT);
	#endif /* __VDOREC_FEATURE_SIZE_LIMIT__ */
	
	#ifdef __VDOREC_FEATURE_TIME_LIMIT__
		CREATE_VIDEO_SETTING_INLINE_SELECT(time_limit, TIME_LIMIT);
	#endif /* __VDOREC_FEATURE_TIME_LIMIT__ */

	#ifdef __VDOREC_FEATURE_RECORD_AUD__
		CREATE_VIDEO_SETTING_INLINE_SELECT(record_aud, RECORD_AUD);
	#endif /* __VDOREC_FEATURE_RECORD_AUD__ */

	#ifdef __VDOREC_FEATURE_VIDEO_FORMAT__
		CREATE_VIDEO_SETTING_INLINE_SELECT(video_format, VIDEO_FORMAT);
	#endif /* __VDOREC_FEATURE_VIDEO_FORMAT__ */
		   
	guiBuffer	= GetCurrGuiBuffer( SCR_ID_VDOREC_VIDEO_SETTING );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_VDOREC_VIDEO_SETTING, &inputBufferSize );
	
	if(inputBuffer != NULL)		
		SetCategory57Data(wgui_inline_items, VDOREC_VIDEO_SETTING_ITEM_COUNT*2, inputBuffer);	

	RegisterHighlightHandler( mmi_vdorec_video_setting_highlight_hdlr );

	/* rotate UI */
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
	/* show category */
	ShowCategory57Screen(STR_ID_VDOREC_VIDEO_SETTING, MAIN_MENU_TITLE_MULTIMEDIA_ICON,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								wgui_inline_items,
								0, guiBuffer);

	/* set key hdlr */
	SetCategory57LeftSoftkeyFunction( mmi_vdorec_video_setting_lsk_press );
	DisableCategory57ScreenDone();
	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );

}
#endif /* !__VDOREC_FEATURE_MERGE_MENU__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_hint_effect_setting
* DESCRIPTION
*  hint effect setting
* PARAMETERS
*	a	IN 	hilight index
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#if 0 // to delete warning
static void mmi_vdorec_hint_effect_setting(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	memset( (PS8)hintData[(index)], 0, sizeof(hintData[(index)]));
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_VDOREC_EFFECT_START+g_vdorec_cntx.setting.effect+1) ));
}
#endif


/*****************************************************************************
* FUNCTION
*	mmi_vdorec_highlight_effect_setting
* DESCRIPTION
*  highlight effect setting
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_highlight_effect_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_vdorec_entry_effect_setting_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_vdorec_entry_effect_setting_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_set_effect_setting_value
* DESCRIPTION
*  effect setting set value
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_set_effect_setting_value(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_vdorec_disply_popup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	DeleteNHistory(1);
	g_vdorec_cntx.setting.effect = (U16)GetCurrHiliteID();
	mmi_vdorec_store_setting();
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_effect_setting_screen
* DESCRIPTION
*  entry effect setting screen
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_entry_effect_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	ItemList[16];
	U16	ItemIcons[16];
	U16	nItems;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	EntryNewScreen(SCR_ID_VDOREC_EFFECT_SETTING, NULL, mmi_vdorec_entry_effect_setting_screen, NULL);

	nItems = GetNumOfChild(MENU_ID_VDOREC_EFFECT_SETTING);
	GetSequenceStringIds(MENU_ID_VDOREC_EFFECT_SETTING,ItemList);
	GetSequenceImageIds(MENU_ID_VDOREC_EFFECT_SETTING,ItemIcons);
	SetParentHandler(MENU_ID_VDOREC_EFFECT_SETTING);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* rotate UI */
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

	ShowCategory11Screen(	STR_ID_VDOREC_EFFECT_SETTING, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									nItems, 
									ItemList,
									g_vdorec_cntx.setting.effect,				/* current index */
									NULL);

	SetLeftSoftkeyFunction(mmi_vdorec_set_effect_setting_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_hint_storage
* DESCRIPTION
*  hint storage 
* PARAMETERS
*	a	IN 	hilight index
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_STORAGE__
static void mmi_vdorec_hint_storage(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	memset((PS8)hintData[(index)], 0, sizeof(hintData[(index)]));
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_VDOREC_STORAGE_PHONE+g_vdorec_cntx.setting.storage) ));
}
#endif /* __VDOREC_FEATURE_STORAGE__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_storage_screen
* DESCRIPTION
*  entry storage setting screen 
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_STORAGE__
static void mmi_vdorec_entry_storage_screen(void)
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
	EntryNewScreen(SCR_ID_VDOREC_STORAGE, NULL, mmi_vdorec_entry_storage_screen, NULL);

	/*+ zhouqin modify for dual t-flash card 20110503*/
	#if defined(DUAL_TFLASH_SUPPORT)
	mmi_frm_hide_menu_item(MENU_ID_VDOREC_STORAGE_PHONE);

	nItems		= GetNumOfChild_Ext(MENU_ID_VDOREC_STORAGE);
	GetSequenceStringIds_Ext(MENU_ID_VDOREC_STORAGE,ItemList);
	GetSequenceImageIds_Ext(MENU_ID_VDOREC_STORAGE,ItemIcons);
	#else
	nItems		= GetNumOfChild(MENU_ID_VDOREC_STORAGE);
	GetSequenceStringIds(MENU_ID_VDOREC_STORAGE,ItemList);
	GetSequenceImageIds(MENU_ID_VDOREC_STORAGE,ItemIcons);
	#endif
	/*- zhouqin modify for dual t-flash card 20110503*/
	SetParentHandler(MENU_ID_VDOREC_STORAGE);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* rotate UI */
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

	ShowCategory11Screen(	STR_ID_VDOREC_STORAGE, 	MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK,				IMG_GLOBAL_OK,
									STR_GLOBAL_BACK,			IMG_GLOBAL_BACK, 
									nItems, ItemList,
									/*+ zhouqin modify for dual t-flash card 20110503*/
									#if defined(DUAL_TFLASH_SUPPORT)
									g_vdorec_cntx.setting.storage-1,								/* current index */
									#else
									g_vdorec_cntx.setting.storage,	
									#endif
									/*- zhouqin modify for dual t-flash card 20110503*/
                                                                        NULL);

	SetLeftSoftkeyFunction(mmi_vdorec_set_storage, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,	KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
#endif /* __VDOREC_FEATURE_STORAGE__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_highlight_storage
* DESCRIPTION
*  highlight storage
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_STORAGE__
static void mmi_vdorec_highlight_storage(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_vdorec_entry_storage_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_vdorec_entry_storage_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}
#endif /* __VDOREC_FEATURE_STORAGE__ */

/*+ zhouqin modify for video record in secord card20110530*/
#if defined(DUAL_TFLASH_SUPPORT)
/*****************************************************************************
* FUNCTION
*	mmi_vdorec_set_storage
* DESCRIPTION
*  set storage
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
BOOL mmi_vdorec_check_storage(void)
{
	if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_SEC_CARD)
           return TRUE;
	else
           return FALSE;
}
#endif
/*- zhouqin modify for video record in secord card20110530*/

/*****************************************************************************
* FUNCTION
*	mmi_vdorec_set_storage
* DESCRIPTION
*  set storage
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOREC_FEATURE_STORAGE__
static void mmi_vdorec_set_storage(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32			ret_drv= 0;
	S8				buf[64];
	S8				buf_filepath[FMGR_PATH_BUFFER_SIZE];
	FS_HANDLE	file_handle= FS_ERROR_RESERVED;
	S32			create_result;
	 
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/*+ zhouqin modify for dual t-flash card 20110503*/
	#if defined(DUAL_TFLASH_SUPPORT)
	g_vdorec_cntx.setting.storage = (U16)GetCurrHiliteID()+1;
	#else
	g_vdorec_cntx.setting.storage = (U16)GetCurrHiliteID();
	#endif
	/*- zhouqin modify for dual t-flash card 20110503*/

	if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_PHONE)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
	}
	else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE , 1, FS_NO_ALT_DRIVE);
	}
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND , 1, FS_NO_ALT_DRIVE);
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	else
	{
		ASSERT(0);
	}

	if(ret_drv >= 0)
	{
		sprintf(buf, "%c:\\", (U8)ret_drv);
		AnsiiToUnicodeString(buf_filepath, buf);

		file_handle = MMI_FS_Open((kal_uint16*)buf_filepath, FS_OPEN_DIR | FS_READ_ONLY);

		if(file_handle >= 0)
		{
			/* drive avalialbe */
			MMI_FS_Close(file_handle);

			/* check if photo path exist or not */
			mmi_vdorec_get_storage_file_path(buf_filepath);
			create_result = mmi_vdorec_create_file_dir(buf_filepath);

			if(create_result == FS_WRITE_PROTECTION)
			{
				mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_WRITE_PROTECTION_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
			}
			else if(create_result == FS_DISK_FULL)
			{
				mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
			}
			else if(create_result == FS_ROOT_DIR_FULL)
			{
				mmi_vdorec_disply_popup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
			}			
			else
			{
				g_vdorec_cntx.is_storage_path_changed = TRUE;
				mmi_vdorec_disply_popup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
				DeleteNHistory(1);
			}

		}
		else
		{
			mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOREC_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			/* set phone internal space as default */
			g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
		}
	}
	else
	{
		/* get drive error  */
		mmi_vdorec_disply_popup((PU8)GetString(STR_ID_VDOREC_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		
		/*+ zhouqin modify for dual t-flash card 20110503*/
		#ifndef  DUAL_TFLASH_SUPPORT
		/* set phone internal space as default */
		g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
		#else
		g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_MEMORY_CARD;
		#endif
		/*- zhouqin modify for dual t-flash card 20110503*/
	}

	mmi_vdorec_store_setting();
}
#endif /* __VDOREC_FEATURE_STORAGE__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_highlight_restore_default
* DESCRIPTION
*  highlight restore default
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_highlight_restore_default(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_vdorec_entry_restore_default_confirm_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_vdorec_entry_restore_default_confirm_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_restore_default_confirm_yes
* DESCRIPTION
*  restore default confirm yes.
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_entry_restore_default_confirm_yes(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* restore default values */
	mmi_vdorec_restore_setting();

	mmi_vdorec_disply_popup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	DeleteNHistory(1);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdorec_entry_restore_default_confirm_screen
* DESCRIPTION
*  entry restore default confirm
* PARAMETERS
*	nil
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdorec_entry_restore_default_confirm_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
	
		DisplayConfirmRotated(	STR_GLOBAL_YES, IMG_GLOBAL_YES,
										STR_GLOBAL_NO, IMG_GLOBAL_NO,
										get_string(STR_ID_VDOREC_NOTIFY_RESTORE_DEFAULT_CONFIRM), 
										IMG_GLOBAL_QUESTION, WARNING_TONE,
										MMI_FRM_SCREEN_ROTATE_270); 	
	#else
	
		DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
							STR_GLOBAL_NO, IMG_GLOBAL_NO,
							get_string(STR_ID_VDOREC_NOTIFY_RESTORE_DEFAULT_CONFIRM), 
							IMG_GLOBAL_QUESTION, WARNING_TONE); 

	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
   


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_vdorec_entry_restore_default_confirm_yes,KEY_EVENT_UP);
}




/*****************************************************************************
*
*	Touch Screen Support
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_touch_scr_pen_down_hdlr()
* DESCRIPTION
*   touch screen hdlr when pen down
* PARAMETERS
*	 pos		IN			pen down position
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__VDOREC_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static void mmi_vdorec_touch_scr_pen_down_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
		S32	tmp_x;
		S32	tmp_y;
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
	mmi_trace(g_sw_VdoRec," mmi_vdorec_touch_scr_pen_down_hdlr x,y(%d,%d)",pos.x,pos.y);
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
	
		/* cord tranfrom, since already rotate */		
		tmp_x = pos.x;
		tmp_y = pos.y;			
		pos.x = tmp_y;
		pos.y = (UI_device_width-1)-tmp_x;
			
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
	
	/* ev inc */
	if(mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_ev_inc))
	{
		if(g_vdorec_cntx.setting.ev >= VDOREC_SETTING_EV_COUNT-1)
			return;
		
		g_vdorec_cntx.touch_ev_inc.is_press = TRUE;
		mmi_vdorec_inc_ev_key_press_hdlr();

		g_vdorec_cntx.touch_object = VDOREC_TOUCH_EV_INC;			
		return;				
	}	

	/* ev dec */
	if(mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_ev_dec))
	{
		if(g_vdorec_cntx.setting.ev == 0)
			return;
		
		g_vdorec_cntx.touch_ev_dec.is_press = TRUE;
		mmi_vdorec_dec_ev_key_press_hdlr();

		g_vdorec_cntx.touch_object = VDOREC_TOUCH_EV_DEC;					
		return;		
	}	

	/* zoom inc */
	if(mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_zoom_inc))
	{
		g_vdorec_cntx.touch_zoom_inc.is_press = TRUE;
		
		mmi_vdorec_zoom_in();
		g_vdorec_cntx.touch_object = VDOREC_TOUCH_ZOOM_INC;	
		return;
	}	

	/* zoom dec */
	if(mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_zoom_dec))
	{
		g_vdorec_cntx.touch_zoom_dec.is_press = TRUE;
		
		mmi_vdorec_zoom_out();
		g_vdorec_cntx.touch_object = VDOREC_TOUCH_ZOOM_DEC;	
		return;		
	}	

	/* rec/pause */
	if(mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_rec))
	{
		g_vdorec_cntx.touch_rec.is_press = TRUE;
		mmi_vdorec_draw_panel_state();
		mmi_vdorec_blt_screen();	

		g_vdorec_cntx.touch_object = VDOREC_TOUCH_REC;	
		return;		
	}	

#if 0 // this function is cancelled
	/* stop */
	if(mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_stop))
	{
		/* if not recording or pause, cant stop */
		if( (g_vdorec_cntx.state != VDOREC_STATE_RECORD) &&
			 (g_vdorec_cntx.state != VDOREC_STATE_PAUSE) )
		{
			return;
		}
		
		g_vdorec_cntx.touch_stop.is_press = TRUE;
		mmi_vdorec_draw_panel_state();
		mmi_vdorec_blt_screen();	

		g_vdorec_cntx.touch_object = VDOREC_TOUCH_STOP;	
		return;		
	}	
#endif
	/* rsk */
	if(mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_rsk))
	{
		mmi_vdorec_rsk_press_hdlr();
		g_vdorec_cntx.touch_object = VDOREC_TOUCH_RSK;	
		return;		
	}	

	/* lsk */
	if(mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_lsk))
	{
		mmi_vdorec_lsk_press_hdlr();
		g_vdorec_cntx.touch_object = VDOREC_TOUCH_LSK;	
		return;		
	}		
	
}
#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */



/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_touch_scr_pen_up_hdlr()
* DESCRIPTION
*   touch screen hdlr when pen up
* PARAMETERS
*	 pos		IN			pen up position
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__VDOREC_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static void mmi_vdorec_touch_scr_pen_up_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_vdorec_cntx.touch_object == VDOREC_TOUCH_NONE)
		return;

	switch(g_vdorec_cntx.touch_object)
	{
		case VDOREC_TOUCH_EV_INC:
			g_vdorec_cntx.touch_ev_inc.is_press = FALSE;
			g_vdorec_cntx.is_ev_inc_key_pressed = FALSE;			
				
			mmi_vdorec_draw_panel_ev();
			mmi_vdorec_blt_screen();	
			g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			break;
	
		case VDOREC_TOUCH_EV_DEC:
			g_vdorec_cntx.touch_ev_dec.is_press = FALSE;
			g_vdorec_cntx.is_ev_dec_key_pressed = FALSE;


			mmi_vdorec_draw_panel_ev();
			mmi_vdorec_blt_screen();	
			g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			break;
	
		case VDOREC_TOUCH_ZOOM_INC:
			g_vdorec_cntx.touch_zoom_inc.is_press = FALSE;
			g_vdorec_cntx.is_zoom_in_key_pressed = FALSE;

//			coolsand_UI_cancel_timer(mmi_vdorec_zoom_in);
			
			mmi_vdorec_draw_panel_zoom();
			mmi_vdorec_blt_screen();	
			g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			break;

		case VDOREC_TOUCH_ZOOM_DEC:
			g_vdorec_cntx.touch_zoom_dec.is_press = FALSE;
			g_vdorec_cntx.is_zoom_out_key_pressed = FALSE;

//			coolsand_UI_cancel_timer(mmi_vdorec_zoom_out);

			mmi_vdorec_draw_panel_zoom();
			mmi_vdorec_blt_screen();	
			g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			break;

		case VDOREC_TOUCH_REC:
			g_vdorec_cntx.touch_rec.is_press = FALSE;

			#if 0//delete by zhangl for tp
			mmi_vdorec_draw_panel_state();
			mmi_vdorec_blt_screen();	
			#endif
			if(g_vdorec_cntx.state == VDOREC_STATE_RECORD)
			{
				mmi_vdorec_enter_state(VDOREC_STATE_PAUSE);	
			}
			else if( (g_vdorec_cntx.state == VDOREC_STATE_PAUSE) || 
						(g_vdorec_cntx.state == VDOREC_STATE_PREVIEW) )
			{
				mmi_vdorec_enter_state(VDOREC_STATE_RECORD);
			}
			else
			{
				MMI_ASSERT(0);
			}

			g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;
			break;
	
		case VDOREC_TOUCH_STOP:
			g_vdorec_cntx.touch_stop.is_press = FALSE;
			#if 0//delete by zhangl for tp
			mmi_vdorec_draw_panel_state();
			mmi_vdorec_blt_screen();	
			#endif
			#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
				if( (g_vdorec_cntx.state == VDOREC_STATE_RECORD) ||
					 (g_vdorec_cntx.state == VDOREC_STATE_PAUSE))
				{
						mdi_video_rec_record_stop();
						coolsand_UI_cancel_timer(mmi_vdorec_record_timer_cyclic);
						mmi_vdorec_entry_saving_screen();
				}
			#else
				mmi_vdorec_rsk_release_hdlr();
			#endif
			
			g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;
			break;

		case VDOREC_TOUCH_RSK:
			mmi_vdorec_rsk_release_hdlr();
			g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			break;

		case VDOREC_TOUCH_LSK:
			mmi_vdorec_lsk_release_hdlr();
			g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			break;			
	}
	
}
#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */



/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_touch_scr_pen_move_hdlr()
* DESCRIPTION
*   touch screen hdlr when pen move
* PARAMETERS
*	 pos		IN			pen move position
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__VDOREC_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static void mmi_vdorec_touch_scr_pen_move_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
		S32	tmp_x;
		S32	tmp_y;
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_vdorec_cntx.touch_object == VDOREC_TOUCH_NONE)
		return;

	#if defined(__VDOREC_FEATURE_HORIZONTAL_VIEW__)
	
		/* cord tranfrom, since already rotate */		
		tmp_x = pos.x;
		tmp_y = pos.y;			
		pos.x = tmp_y;
		pos.y = (UI_device_width-1)-tmp_x;
			
	#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
	

	switch(g_vdorec_cntx.touch_object)
	{
		case VDOREC_TOUCH_EV_INC:
			if(!mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_ev_inc))
			{		
				/* if move out */
				g_vdorec_cntx.touch_ev_inc.is_press = FALSE;
				g_vdorec_cntx.is_ev_inc_key_pressed = FALSE;
				
				mmi_vdorec_draw_panel_ev();
				mmi_vdorec_blt_screen();	
				g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			}
			break;
	
		case VDOREC_TOUCH_EV_DEC:
			if(!mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_ev_dec))
			{		
				/* if move out */
				g_vdorec_cntx.touch_ev_dec.is_press = FALSE;
				g_vdorec_cntx.is_ev_dec_key_pressed = FALSE;
				
				mmi_vdorec_draw_panel_ev();
				mmi_vdorec_blt_screen();	
				g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			}
			break;
	
		case VDOREC_TOUCH_ZOOM_INC:
			if(!mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_zoom_inc))
			{		
				/* if move out */
				g_vdorec_cntx.touch_zoom_inc.is_press = FALSE;
				g_vdorec_cntx.is_zoom_in_key_pressed = FALSE;

				coolsand_UI_cancel_timer(mmi_vdorec_zoom_in);
				
				mmi_vdorec_draw_panel_zoom();
				mmi_vdorec_blt_screen();	
				
				g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			}
			break;

		case VDOREC_TOUCH_ZOOM_DEC:
			if(!mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_zoom_dec))
			{		
				/* if move out */
				g_vdorec_cntx.touch_zoom_dec.is_press = FALSE;
				g_vdorec_cntx.is_zoom_out_key_pressed = FALSE;
				
				coolsand_UI_cancel_timer(mmi_vdorec_zoom_out);
				
				mmi_vdorec_draw_panel_zoom();
				mmi_vdorec_blt_screen();	
				
				g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			}
			break;

		case VDOREC_TOUCH_REC:
			if(!mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_rec))
			{		
				/* if move out */
				g_vdorec_cntx.touch_rec.is_press = FALSE;
				
				mmi_vdorec_draw_panel_state();
				mmi_vdorec_blt_screen();	
				g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			}
			break;

		case VDOREC_TOUCH_STOP:
			if(!mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_stop))
			{		
				/* if move out */
				g_vdorec_cntx.touch_stop.is_press = FALSE;
				
				mmi_vdorec_draw_panel_state();
				mmi_vdorec_blt_screen();	
				g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			}
			break;

		case VDOREC_TOUCH_RSK:
			if(!mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_rsk))
			{		
				/* if move out */
				g_vdorec_cntx.is_rsk_pressed = FALSE;	

				mmi_vdorec_draw_softkey();
				mmi_vdorec_blt_screen();	
				
				g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			}
			break;

		case VDOREC_TOUCH_LSK:
			if(!mmi_vdorec_touch_scr_hit_test(pos.x, pos.y, &g_vdorec_cntx.touch_lsk))
			{		
				/* if move out */
				g_vdorec_cntx.is_lsk_pressed = FALSE;	

				mmi_vdorec_draw_softkey();
				mmi_vdorec_blt_screen();	
				
				g_vdorec_cntx.touch_object = VDOREC_TOUCH_NONE;	
			}
			break;
			
	}
	
}
#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */


/*****************************************************************************
* FUNCTION
*	 mmi_vdorec_touch_scr_hit_test()
* DESCRIPTION
*   test if is within touch region
* PARAMETERS
*	 pos_x		IN			position x
*	 pos_y		IN			position y
*	 touch		IN			touch object structure
* RETURNS
*   BOOL	
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__VDOREC_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static BOOL mmi_vdorec_touch_scr_hit_test(S32 pos_x, S32 pos_y, vdorec_ui_touch_struct *touch_obj)
{
 	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if((pos_x >= touch_obj->offset_x) &&
		(pos_x <  touch_obj->offset_x + touch_obj->width) &&
		(pos_y >= touch_obj->offset_y) &&
		(pos_y <  touch_obj->offset_y + touch_obj->height))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */




#ifdef __MMI_NOKIA_STYLE_N800__
#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
#undef __VDOREC_FEATURE_TOUCH_SCREEN__
#endif
#endif

void mmi_vdorec_clean_app(void)
{
    if(IsScreenPresent( SCR_ID_VDOREC_APP))
       DeleteUptoScrID(IDLE_SCREEN_ID);
}

#ifdef __MMI_PROJECT_T33__
#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
#undef __VDOREC_FEATURE_TOUCH_SCREEN__
#endif
#endif

#endif /* __MMI_VIDEO_RECORDER__ */

#endif /* _MMI_VDORECAPP_C */


