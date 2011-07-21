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
 *	 CameraMenu.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Header of Camera Menus
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"
#ifdef __MMI_CAMERA__

#define __NEWSIMULATOR 

#include "stdc.h"
#include "l4dr1.h"
#include "globaldefs.h"

#ifdef MMI_ON_HARDWARE_P
	#include "nvram_data_items.h" 		/* id to store file path */
#endif


#include "lcd_if.h"							/* lcd layer enable flag */
#include "lcd_sw_rnd.h"						/* for lcd size */
#include "mmi_features_camera.h"			/* camera features */

#include "custmenures.h"
#include "globalconstants.h"				/* key id define */
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
#include "gui_scrollbars.h"
#include "wgui.h"								/* for getdatetime() */
#include "gpioinc.h"							/* led */
#include "debuginitdef.h"					/* mmi_assert */
#include "settingprofile.h"
#include "commonscreens.h"					/* displaypopup() */
#include "mainmenudef.h"					/* multi-media icon */

#include "custmenures.h"
#include "globalconstants.h"				/* key id define */
#include "globaldefs.h"						/* golbal image and string id. */
#include "gui.h"								/* gui functions */
#include "fat_fs.h"
#include "historygprot.h"					/* screen history */
#include "unicodexdcl.h"					/* unicode transform functions */
#include "nvramtype.h"						/* nvram data type */
#include "nvramprot.h"						/* nvram access fucntions */	
#include "nvramenum.h"						/* nvram id define */
#include "eventsgprot.h"		
#include "settingdefs.h"					/* st_notifyduration define */
#include "settingprofile.h"				/* success_tone define */
#include "wgui.h"								/* for getdatetime() */
#include "gpioinc.h"							/* led */
#include "debuginitdef.h"					/* mmi_assert */
#include "settingprofile.h"
//#include "sublcdhistorygprot.h"
//#include "keybrd.h"							/* keypad tone related interface */

#include "wgui_categories.h"
#include "wgui_categories_multimedia.h"
#include "wgui_categories_popup.h"

#include "gdi_include.h"					/* graphic library */
#include "mdi_datatype.h"
#include "mdi_camera.h"						/* camera module */


#include "filemanagergprot.h"				/* file path / file error */
#include "filemanagerdef.h"				/* for imageviewer's screen id - scr_fmgr_explorer*/
#include "filemgr.h"							/* get user define file path */
#include "imageviewer.h"					/* imageviewer */
#include "nvram_data_items.h" 			/* id to store file path */
#include "imageviewerresdefs.h"

#include "resource_camera_skins.h"		/* ui custom config */
#include "cameraapp.h"						/* camera app data */
#include "cameramenu.h"						/* function declare */
#include "cameraresdefs.h"					/* resource id def */
#include "vdoplystrenum.h"

#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	#include "screenrotationgprot.h"
#endif

//#include "adp_filesystem.h" //wufasong added 2007.01.08

/* wufasong added 2007.03.05 */
#ifdef _MMI_OPERATOR_
#include "operator.h"
#endif

#include "mmi_trace.h"

/****************************************************************************
* Enum                                                                 
*****************************************************************************/
typedef enum {	
	CUSTOM_FX_BRIGHTNESS,
	CUSTOM_FX_SATURATION,
	CUSTOM_FX_CONTRAST,
	CUSTOM_FX_HUE
} custom_fx_edit_enum;

/****************************************************************************
* Struct                                                                 
*****************************************************************************/
typedef struct{

	/* strings for inline selection display */
	/* +1 avoid this feature's item is zero */
	UI_string_type wb_str[CAMERA_WB_ITEM_COUNT+1];  
	UI_string_type flash_str[CAMERA_FLASH_ITEM_COUNT+1];
//	UI_string_type led_highlight_str[CAMERA_LCD_HIGHLIGHT_ITEM_COUNT+1];	
	UI_string_type shutter_sound_str[CAMERA_SHUTTER_SOUND_ITEM_COUNT+1];
	UI_string_type ev_str[CAMERA_EV_ITEM_COUNT+1];
	UI_string_type bright_str[6];  //add by WeiD
	UI_string_type contrast_str[6];   //add by WeiD
	UI_string_type night_str[CAMERA_NIGHT_ITEM_COUNT+1];
	UI_string_type ae_meter_str[CAMERA_AE_METER_ITEM_COUNT+1];	
	UI_string_type closeup_str[CAMERA_CLOSEUP_ITEM_COUNT+1];
	UI_string_type banding_str[CAMERA_BANDING_ITEM_COUNT+1];
	UI_string_type iso_str[CAMERA_ISO_ITEM_COUNT+1];
	UI_string_type shot_mode_str[CAMERA_SHOT_MODE_ITEM_COUNT+1];

	UI_string_type shot_mode_shutter_pri_str[CAMERA_SHOT_MODE_SHUTTER_PRI_ITEM_COUNT+1];
	UI_string_type shot_mode_aperture_pri_str[CAMERA_SHOT_MODE_APERTURE_PRI_ITEM_COUNT+1];
	S32 shot_mode_shutter_pri_selected;
	S32 shot_mode_aperture_pri_selected;	
	
	/* selected item */
	S32 bright_selected; //add by WeiD
	S32 contrast_selected; //add by WeiD
	S32 wb_selected;
	S32 flash_selected;
	S32 led_highlight_selected;
	S32 shutter_sound_selected;
	S32 ev_selected;
	S32 night_selected;
	S32 ae_meter_selected;
	S32 closeup_selected;
	S32 banding_selected;
	S32 iso_selected;
	S32 shot_mode_selected;
} camera_camera_setting_inline_data_struct;

typedef struct{
	/* strings for inline selection display */
	UI_string_type image_size_str[CAMERA_IMAGE_SIZE_ITEM_COUNT];
	UI_string_type image_qty_str[CAMERA_IMAGE_QTY_ITEM_COUNT];

	/* selected item */
	S32 image_size_selected;
	S32 image_qty_selected;
} camera_image_setting_inline_data_struct;


typedef struct {
	/* inline selection */
	camera_camera_setting_inline_data_struct	camera_setting_inline;
	camera_image_setting_inline_data_struct	image_setting_inline;	

	S32	cur_select_add_frame_idx;
} camera_menu_contex_struct;


typedef struct {
	horizontal_slide 	slide_bar;
	S32					offset_x;
	S32 					offset_y;
	S32 					width;
	S32					height;
	U16					value;
} custom_fx_slider_struct;


typedef struct {
	custom_fx_slider_struct	brightness_slider;
	custom_fx_slider_struct	saturation_slider;
	custom_fx_slider_struct	contrast_slider;
	custom_fx_slider_struct	hue_slider;

	S32					text_box_offset_x;
	S32					text_box_offset_y;	
	S32					text_box_width;
	S32					text_box_height;	
	S32					spacing;
	S32					num_width;
	S32					num_height;
	gdi_handle			preview_wnd_layer;
	gdi_handle			base_layer;
	BOOL					is_left_arrow_press;
	BOOL					is_right_arrow_press;
	U16					select_idx;	
	U16					fx_idx;
} custom_fx_edit_struct;



/****************************************************************************
* Local Variable                                                                 
*****************************************************************************/
camera_menu_contex_struct 	g_camera_menu_cntx;
custom_fx_edit_struct 		g_camera_custom_fx_cntx;

/****************************************************************************
* Global Variable                                                                 
*****************************************************************************/
extern wgui_inline_item	 wgui_inline_items[];

extern const U8 camera_ev_command_map[];
extern const U8 camera_zoom_command_map[];
extern const U8 camera_effect_command_map[];
extern const U8 camera_wb_command_map[];
extern const U8 camera_preview_size_command_map[];
extern const U8 camera_jpg_size_command_map[];
extern const U8 camera_image_qty_command_map[];
extern const U8 camera_banding_command_map[];
extern const U8 camera_flash_command_map[];
extern const U8 camera_ae_meter_command_map[];
extern const U8 camera_iso_command_map[];
extern const U8 camera_shot_mode_command_map[];

/****************************************************************************
* Golbal Function
*****************************************************************************/
extern void AudioStopReq(U16 soundId);
extern U16 	GetCurKeypadTone(void);


/****************************************************************************
* Local function			
*****************************************************************************/

/* Image Viewer */
void mmi_camera_highlight_to_imgview(void);

/* Camera Setting */
void mmi_camera_highlight_camera_setting(void);
void mmi_camera_entry_camera_setting_screen(void);

/* Image Setting */
void mmi_camera_highlight_image_setting(void);
void mmi_camera_entry_image_setting_screen(void);

/* Effect Setting */
void mmi_camera_hint_effect_setting(U16 index);
void mmi_camera_highlight_effect(void);
void mmi_camera_set_effect_setting_value(void);
void mmi_camera_set_effect_setting_hdlr(S32 index);
void mmi_camera_effect_setting_highlight_hdlr(S32 idx);
void mmi_camera_entry_effect_setting_screen(void);

/* Shot Mode */
void mmi_camera_hint_shot_mode(U16 index);
void mmi_camera_highlight_effect(void);
void mmi_camera_set_shot_mode_value(void);
void mmi_camera_set_shot_mode_hdlr(S32 index);
void mmi_camera_shot_mode_highlight_hdlr(S32 idx);
void mmi_camera_entry_shot_mode_screen(void);

	/* Shot Mode Setting */
	void mmi_camera_exit_shot_mode_setting_screen(void);
	void mmi_camera_shot_mode_setting_set_value(void);
	void mmi_camera_entry_shot_mode_setting_screen(void);
	void mmi_camera_shot_mode_setting_highlight_hdlr(S32 index);

/* Add Frame */
void mmi_camera_hint_add_frame(U16 index);
void mmi_camera_highlight_add_frame(void);
void mmi_camera_set_add_frame_value(void);
void mmi_camera_set_add_frame_hdlr(S32 index);
void mmi_camera_entry_add_frame_screen(void);

/* Restore default */
void mmi_camera_highlight_restore_default(void);
void mmi_camera_entry_restore_default_confirm_screen(void);
void mmi_camera_entry_restore_default_confirm_yes(void);


void mmi_camera_highlight_to_imgview(void);
void mmi_camera_highlight_camera_setting(void);
void mmi_camera_highlight_image_setting(void);
void mmi_camera_highlight_effect_setting(void);

#ifdef __CAMERA_FEATURE_SHOT_MODE__
	void mmi_camera_highlight_shot_mode(void);
#endif /*  __CAMERA_FEATURE_SHOT_MODE__ */

void mmi_camera_highlight_cont_shot(void);
void mmi_camera_highlight_delay_timer(void);
void mmi_camera_highlight_self_capture(void);	
void mmi_camera_highlight_storage(void);
void mmi_camera_highlight_restore_default(void);
	
void mmi_camera_highlight_custom_fx_set(void);
void mmi_camera_highlight_custom_fx_edit(void);

#ifdef __CAMERA_FEATURE_SHOT_MODE__
	void mmi_camera_hint_shot_mode(U16 index);
#endif /*  __CAMERA_FEATURE_SHOT_MODE__ */

void mmi_camera_hint_cont_shot(U16 index);
void mmi_camera_hint_delay_timer(U16 index);
void mmi_camera_hint_add_frame(U16 index);
void mmi_camera_hint_storage(U16 index);

void mmi_camera_entry_delay_timer_screen(void);
void mmi_camera_entry_cont_shot_screen(void);

/* storage */
void mmi_camera_hint_storage(U16 index);
void mmi_camera_highlight_storage(void);
void mmi_camera_set_storage_value(void);
void mmi_camera_set_storage_path_check(U16 storage_id, BOOL is_popup_done);
void mmi_camera_set_storage_hdlr(S32 index);
void mmi_camera_entry_storage_screen(void);
void mmi_camera_entry_add_frame_view_screen(void);

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
	void mmi_camera_custom_fx_edit_right_arrow_press(void);
	void mmi_camera_custom_fx_edit_left_arrow_press(void);

	void mmi_camera_custom_fx_edit_draw_value(U16 idx);

	void mmi_camera_entry_custom_fx_edit_sublcd_screen(void);
	void mmi_camera_custom_fx_edit_draw_title(void);


	#ifdef __MMI_SUBLCD__
	void mmi_camera_entry_custom_fx_edit_sublcd_screen(void);
	void mmi_camera_exit_custom_fx_edit_sublcd_screen(void);
	#endif 
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */

#ifdef __MMI_CLAMSHELL__
U8 mmi_camera_custom_fx_clam_close_hdlr(void* ptr);
#endif

#ifndef __MMI_DISABLE_DUAL_SENSOR__
void mmi_camera_entry_switch_sensor(void);

void mmi_camera_highlight_switch_sensor(void);

#endif


extern BOOL checkTCardExist(void);
extern S32 gdi_image_read_length(U8 *image);
extern  void StrToRate( CALOPERATOR_STRUCT *Rate, char * str);
extern void RateToStr(char *str, CALOPERATOR_STRUCT *Rate);
extern pBOOL IsSilentModeActivated(void);

/*****************************************************************************
* FUNCTION
*  mmi_camera_init_option_menu
* DESCRIPTION
*  Init menus hdlr
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
#ifdef __PRJ_WITH_SPICAM__
extern void mmi_camera_capture_hilight_app(void);
#endif
void mmi_camera_init_option_menu(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	SetHiliteHandler(MENU_ID_CAMERA_TO_IMGVIEW,					mmi_camera_highlight_to_imgview);
#ifdef __PRJ_WITH_SPICAM__
    SetHiliteHandler(MENU_ID_CAMERA_TO_CAPTURE,					mmi_camera_capture_hilight_app);
#endif   
	SetHiliteHandler(MENU_ID_CAMERA_CAMERA_SETTING,				mmi_camera_highlight_camera_setting);
	SetHiliteHandler(MENU_ID_CAMERA_IMAGE_SETTING,				mmi_camera_highlight_image_setting);
	
#ifndef __MMI_DISABLE_DUAL_SENSOR__
    SetHiliteHandler(MENU_ID_CAMERA_SWITCH_SENSOR, mmi_camera_highlight_switch_sensor);
#endif

#if defined(__CAMERA_FEATURE_CONT_SHOT__)
SetHiliteHandler(MENU_ID_CAMERA_CONT_SHOT,					mmi_camera_highlight_cont_shot);
#endif
#if defined(__CAMERA_FEATURE_DELAY_TIMER__)
	SetHiliteHandler(MENU_ID_CAMERA_DELAY_TIMER,					mmi_camera_highlight_delay_timer);
#endif
	SetHiliteHandler(MENU_ID_CAMERA_SELF_CAPTURE,				mmi_camera_highlight_self_capture);	
#if defined(COOLSAND_MULTIMEDIA_USE_FLASHFS)
	SetHiliteHandler(MENU_ID_CAMERA_STORAGE,						mmi_camera_highlight_storage);
#endif
	SetHiliteHandler(MENU_ID_CAMERA_RESTORE_DEFAULT,			mmi_camera_highlight_restore_default);

#if defined(__CAMERA_FEATURE_EFFECT__)
	SetHiliteHandler(MENU_ID_CAMERA_EFFECT_SETTING,				mmi_camera_highlight_effect_setting);
	SetHintHandler(MENU_ID_CAMERA_EFFECT_SETTING,				mmi_camera_hint_effect_setting);
#endif

	#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
		SetHiliteHandler(MENU_ID_CAMERA_EFFECT_CUSTOM_FX_SET,				mmi_camera_highlight_custom_fx_set);
		SetHiliteHandler(MENU_ID_CAMERA_EFFECT_CUSTOM_FX_EDIT_EFFECT,	mmi_camera_highlight_custom_fx_edit);
	#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */

	#ifdef __CAMERA_FEATURE_SHOT_MODE__
		SetHiliteHandler(MENU_ID_CAMERA_SHOT_MODE,				mmi_camera_highlight_shot_mode);
		SetHintHandler(MENU_ID_CAMERA_SHOT_MODE,					mmi_camera_hint_shot_mode);
	#endif /*__CAMERA_FEATURE_SHOT_MODE__ */

	#ifdef 	__CAMERA_FEATURE_ADD_FRAME__
		SetHiliteHandler(MENU_ID_CAMERA_ADD_FRAME,				mmi_camera_highlight_add_frame);
		SetHintHandler(MENU_ID_CAMERA_ADD_FRAME,					mmi_camera_hint_add_frame);	
	#endif
#if defined(COOLSAND_MULTIMEDIA_USE_FLASHFS)
	mmi_trace(g_sw_CAMERA, "MENU_ID_CAMERA_STORAGE = %ld",MENU_ID_CAMERA_STORAGE);
#endif

#if defined(__CAMERA_FEATURE_CONT_SHOT__)
	SetHintHandler(MENU_ID_CAMERA_CONT_SHOT,						mmi_camera_hint_cont_shot);
#endif
#if defined(__CAMERA_FEATURE_DELAY_TIMER__)
	SetHintHandler(MENU_ID_CAMERA_DELAY_TIMER,					mmi_camera_hint_delay_timer);
#endif
#if defined(COOLSAND_MULTIMEDIA_USE_FLASHFS)
	SetHintHandler(MENU_ID_CAMERA_STORAGE,							mmi_camera_hint_storage);
#endif
}


 


/*****************************************************************************
*
* Camera Option Menu     
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_to_imgview()
* DESCRIPTION
*   link to image viewer 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_to_imgview(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_highlight_to_imgview()");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	SetLeftSoftkeyFunction(mmi_imageviewer_view_the_latest,KEY_EVENT_UP);
	SetKeyHandler(mmi_imageviewer_view_the_latest, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(mmi_imgview_entry_app,KEY_EVENT_UP);
	SetKeyHandler(mmi_imgview_entry_app, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#endif
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_option_menu_screen()
* DESCRIPTION
*   option menu
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_option_menu_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8			*guiBuffer;
	U8			*PopUpList[32];
	S32		i;

	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_entry_option_menu_screen()");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	EntryNewScreen(SCR_ID_CAMERA_OPTION, NULL, mmi_camera_entry_option_menu_screen, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

		if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
			mmi_frm_hide_menu_item(MENU_ID_CAMERA_ADD_FRAME);
		else
			mmi_frm_unhide_menu_item(MENU_ID_CAMERA_ADD_FRAME);
	#endif

	guiBuffer	= GetCurrGuiBuffer(SCR_ID_CAMERA_OPTION);
	nItems		= GetNumOfChild_Ext(MENU_ID_CAMERA_OPTION);
	GetSequenceStringIds_Ext(MENU_ID_CAMERA_OPTION,ItemList);
	GetSequenceImageIds_Ext(MENU_ID_CAMERA_OPTION,ItemIcons);
	SetParentHandler(MENU_ID_CAMERA_OPTION);


	for(i=0;i<nItems;i++)
		PopUpList[i]=NULL;

	ConstructHintsList(MENU_ID_CAMERA_OPTION, PopUpList);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);


	ShowCategory52Screen(	STR_GLOBAL_OPTIONS,	MAIN_MENU_TITLE_MULTIMEDIA_ICON,
									STR_GLOBAL_OK,IMG_GLOBAL_OK,
									STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
									nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

#ifdef __PRJ_WITH_SPICAM__
    SetLeftSoftkeyFunction(mmi_camera_entry_app_screen,KEY_EVENT_UP);
#endif
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}



#if defined(__CAMERA_FEATURE_DELAY_TIMER__)

/*****************************************************************************
*
* Delay Timer Menu     
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_hint_delay_timer()
* DESCRIPTION
*   delay timer's hint
* PARAMETERS
*	 index 		IN 		menuitem index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_hint_delay_timer(U16 index)
{
	memset((PS8)hintData[(index)], 0, sizeof(hintData[(index)]));
	pfnUnicodeStrcpy((S8*)hintData[index], 
					     (S8*)GetString((U16)(STR_ID_CAMERA_DELAY_TIMER_START + g_camera_cntx.setting.delay_timer+1)));
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_delay_timer()
* DESCRIPTION
*   delay timer highlight function
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_delay_timer(void)
{
	SetLeftSoftkeyFunction(mmi_camera_entry_delay_timer_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_entry_delay_timer_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_set_delay_timer_set_value()
* DESCRIPTION
*   set delay timer value
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_set_delay_timer_set_value(void)
{
	#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	#else
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteNHistory(1);
	g_camera_cntx.setting.delay_timer = (U16)GetCurrHiliteID();
	mmi_camera_store_setting();
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_delay_timer_screen()
* DESCRIPTION
*   entry delay timer screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_delay_timer_screen(void)
{
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8*		guiBuffer;
	EntryNewScreen(SCR_ID_CAMERA_DELAY_TIMER, NULL, mmi_camera_entry_delay_timer_screen, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
	#endif

	guiBuffer	= GetCurrGuiBuffer(SCR_ID_CAMERA_DELAY_TIMER);
	nItems		= GetNumOfChild(MENU_ID_CAMERA_DELAY_TIMER);
	GetSequenceStringIds(MENU_ID_CAMERA_DELAY_TIMER,ItemList);
	GetSequenceImageIds(MENU_ID_CAMERA_DELAY_TIMER,ItemIcons);
	SetParentHandler(MENU_ID_CAMERA_DELAY_TIMER);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	ShowCategory11Screen(	STR_ID_CAMERA_DELAY_TIMER,
									MAIN_MENU_TITLE_MULTIMEDIA_ICON,
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
									nItems,
									ItemList,
									g_camera_cntx.setting.delay_timer,
									guiBuffer);
	
	SetLeftSoftkeyFunction(mmi_camera_set_delay_timer_set_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
#endif


#if defined(__CAMERA_FEATURE_CONT_SHOT__)

/*****************************************************************************
*
* Cont Shot Menu     
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	mmi_camera_hint_cont_shot()
* DESCRIPTION
*  cont shot's hint
* PARAMETERS
*	 index 		IN 		menuitem index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_hint_cont_shot(U16 index)
{
	memset((PS8)hintData[(index)], 0, sizeof(hintData[(index)]));
	pfnUnicodeStrcpy((S8*)hintData[index], 
					     (S8*)GetString((U16)(STR_ID_CAMERA_CONT_SHOT_START + g_camera_cntx.setting.cont_shot+1)));
}


/*****************************************************************************
* FUNCTION
*	mmi_camera_highlight_cont_shot()
* DESCRIPTION
*  cont shot highlight function
* PARAMETERS
*	void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_cont_shot(void)
{
	SetLeftSoftkeyFunction(mmi_camera_entry_cont_shot_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_entry_cont_shot_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	mmi_camera_set_cont_shot_set_value()
* DESCRIPTION
*  set cont shot value 
* PARAMETERS
*	void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_set_cont_shot_set_value(void)
{
	#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	#else
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteNHistory(1);
	g_camera_cntx.setting.cont_shot = (U16)GetCurrHiliteID();

	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_BURST)
		{
			g_camera_cntx.setting.image_size 		= CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX;
			g_camera_cntx.image_size_before_burst 	= CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX;
			mmi_camera_preview_update_capture_size();
		}
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_TILE)
		{
			g_camera_cntx.is_tileshot 			= TRUE;
			g_camera_cntx.tile_cur_num 			= 0;
			g_camera_cntx.previous_image_size 	= g_camera_cntx.setting.image_size;
			g_camera_cntx.setting.image_size	= CAMERA_FEATURE_CONT_SHOT_TILE_IMAGE_IDX;
			g_camera_cntx.setting.zoom_idx = CAMERA_DEFAULT_SETTING_ZOOM_IDX;
			mmi_camera_preview_update_capture_size();
		}
		else
		{
			if(g_camera_cntx.is_tileshot == TRUE)
				g_camera_cntx.setting.image_size = g_camera_cntx.previous_image_size;
			g_camera_cntx.is_tileshot = FALSE;
		}
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

	mmi_camera_store_setting();
}



/*****************************************************************************
* FUNCTION
*	mmi_camera_entry_cont_shot_screen()
* DESCRIPTION
*  entry cont shot screen
* PARAMETERS
*	void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_cont_shot_screen(void)
{
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8*		guiBuffer;
	
	EntryNewScreen(SCR_ID_CAMERA_CONT_SHOT, NULL, mmi_camera_entry_cont_shot_screen, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif
	
	guiBuffer	= GetCurrGuiBuffer(SCR_ID_CAMERA_CONT_SHOT);
	nItems		= GetNumOfChild(MENU_ID_CAMERA_CONT_SHOT);
	GetSequenceStringIds(MENU_ID_CAMERA_CONT_SHOT,ItemList);
	GetSequenceImageIds(MENU_ID_CAMERA_CONT_SHOT,ItemIcons);
	SetParentHandler(MENU_ID_CAMERA_CONT_SHOT);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	ShowCategory11Screen(	STR_ID_CAMERA_CONT_SHOT,
									MAIN_MENU_TITLE_MULTIMEDIA_ICON,
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
									nItems,
									ItemList,
									g_camera_cntx.setting.cont_shot,
									guiBuffer);
	
	SetLeftSoftkeyFunction(mmi_camera_set_cont_shot_set_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	
}

#endif







#if defined(COOLSAND_MULTIMEDIA_USE_FLASHFS)


/*****************************************************************************
*
* Camera Storage Menu     
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_hint_storage()
* DESCRIPTION
*   storage hint
* PARAMETERS
*	 index 	IN 	storage id
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_hint_storage(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	memset((PS8)hintData[(index)], 0, sizeof(hintData[(index)]));
#ifdef COOLSAND_MULTIMEDIA_USE_FLASHFS
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_CAMERA_STORAGE_PHONE+g_camera_cntx.setting.storage) ));
#else
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_CAMERA_STORAGE_PHONE+1) )); //modified by WeiD
#endif
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_storage()
* DESCRIPTION
*   storage highlight
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_storage(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_entry_storage_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_entry_storage_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_set_storage_value()
* DESCRIPTION
*   set storage value
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_set_storage_value(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	mmi_camera_set_storage_path_check((U16)GetCurrHiliteID(), TRUE);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_set_storage_path_check()
* DESCRIPTION
*   set storage path check
* PARAMETERS
*	 storage_id					IN			storage id
*	 is_popup_done				IN			if success, show popup
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_set_storage_path_check(U16 storage_id, BOOL is_popup_done)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8				buf_filepath[FMGR_PATH_BUFFER_SIZE];
	S32			ret_drv=0;
	S8				buf[64];
	FS_HANDLE	file_handle;
	S32			create_result;
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
        U16                 highlighted;
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
#ifndef COOLSAND_MULTIMEDIA_USE_FLASHFS
	storage_id = 1; //for we can't save in phone.only in memory card  //add by WeiD
#endif
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
	        highlighted = storage_id;
        #else
        	g_camera_cntx.setting.storage = storage_id;
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/
        
 /*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
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
		AnsiiToUnicodeString(buf_filepath, buf);

		file_handle = MMI_FS_Open((const UINT8 *)buf_filepath, FS_OPEN_DIR | FS_READ_ONLY);
		
		#if 0 //gdm 090611 fix bug 12340
               file_handle= checkTCardExist();
		#endif
		
		//if(file_handle >= 0 || file_handle == FS_FOLDER_HANDLE) //chenhe comment
		if(file_handle)
		{
			/* drive is avaiable */
			FS_Close(file_handle);

			/* check if photo path exist or not */
			mmi_camera_get_storage_file_path(buf_filepath);
			create_result = mmi_camera_create_file_dir(buf_filepath);

			if(create_result == FS_WRITE_PROTECTION)
			{
				#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
				DisplayPopupRotated((PU8)GetString(FMGR_FS_WRITE_PROTECTION_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE,g_camera_cntx.osd_rotate);
				#else
				DisplayPopup((PU8)GetString(FMGR_FS_WRITE_PROTECTION_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				#endif
				/*+ zhouqin modify for dual t-flash card 20110503*/
				#ifndef DUAL_TFLASH_SUPPORT 
				/* set phone internal space as default */				
				g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
				#endif
				/*- zhouqin modify for dual t-flash card 20110503*/
			}
			else if(create_result == FS_DISK_FULL)
			{
				#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
				DisplayPopupRotated((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE,g_camera_cntx.osd_rotate);
				#else
				DisplayPopup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				#endif
				/*+ zhouqin modify for dual t-flash card 20110503*/
				#ifndef DUAL_TFLASH_SUPPORT 
				/* set phone internal space as default */				
				g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
				#endif
				/*- zhouqin modify for dual t-flash card 20110503*/
			}
			else if(create_result == FS_ROOT_DIR_FULL)
			{
				#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
				DisplayPopupRotated((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE,g_camera_cntx.osd_rotate);
				#else
				DisplayPopup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				#endif
				/*+ zhouqin modify for dual t-flash card 20110503*/
				#ifndef DUAL_TFLASH_SUPPORT 
				/* set phone internal space as default */				
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
				if(is_popup_done)
				{
					#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
					DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
					#else
					DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
					#endif
					DeleteNHistory(1);
				}
			}

		}
		else
		{
			#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
			DisplayPopupRotated((PU8)GetString(STR_ID_CAMERA_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE,g_camera_cntx.osd_rotate);
			#else
			DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			#endif
			/*+ zhouqin modify for dual t-flash card 20110503*/
			#ifndef DUAL_TFLASH_SUPPORT 
			/* set phone internal space as default */				
			g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
			#endif
			/*- zhouqin modify for dual t-flash card 20110503*/
		}
	}
	else
	{
		/* get drive error  */
		#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
		DisplayPopupRotated((PU8)GetString(STR_ID_CAMERA_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE,g_camera_cntx.osd_rotate);
		#else
		DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		#endif
		/*+ zhouqin modify for dual t-flash card 20110503*/
		#ifndef DUAL_TFLASH_SUPPORT 
		/* set phone internal space as default */				
		g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_PHONE;
		#endif
		/*- zhouqin modify for dual t-flash card 20110503*/
	}
	
	mmi_camera_store_setting();
}




/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_storage_screen()
* DESCRIPTION
*   entry storage screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_storage_screen(void)
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
	EntryNewScreen(SCR_ID_CAMERA_STORAGE, NULL, mmi_camera_entry_storage_screen, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif
	
	nItems		= GetNumOfChild(MENU_ID_CAMERA_STORAGE);
	GetSequenceStringIds(MENU_ID_CAMERA_STORAGE,ItemList);
	GetSequenceImageIds(MENU_ID_CAMERA_STORAGE,ItemIcons);
	SetParentHandler(MENU_ID_CAMERA_STORAGE);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory11Screen(	STR_ID_CAMERA_STORAGE, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									nItems, 
									ItemList,
									g_camera_cntx.setting.storage,				/* current index */
									NULL);

	SetLeftSoftkeyFunction(mmi_camera_set_storage_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

#endif



/***************************************************************************** 
*
* Custom FX
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_draw_hilight_item()
* DESCRIPTION
*   redraw hilight item
* PARAMETERS
*   cur_idx 	IN 	current selected index
*   previ_idx 	IN 	previous selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_draw_hilight_item(U16 cur_idx, U16 prev_idx)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	custom_fx_slider_struct *slider_p;
	S32 x1, y1, x2, y2;
	S32 spacing;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* de hilight previous */
	switch(prev_idx)
	{
		case CUSTOM_FX_BRIGHTNESS:
			slider_p = &g_camera_custom_fx_cntx.brightness_slider;	
			break;

		case CUSTOM_FX_SATURATION:
			slider_p = &g_camera_custom_fx_cntx.saturation_slider;	
			break;

		case CUSTOM_FX_CONTRAST:
			slider_p = &g_camera_custom_fx_cntx.contrast_slider;	
			break;

		case CUSTOM_FX_HUE:
			slider_p = &g_camera_custom_fx_cntx.hue_slider;	
			break;
	}

	spacing = (g_camera_custom_fx_cntx.spacing >> 1);

	x1 = spacing;
	x2 = UI_device_width-spacing-1;
	y1 = slider_p->offset_y-spacing;
	y2 = slider_p->offset_y+spacing+slider_p->height-1;
	
	gdi_draw_rect(x1, y1, x2, y2, GDI_COLOR_WHITE);


	/* highlight */
	switch(cur_idx)
	{
		case CUSTOM_FX_BRIGHTNESS:
			slider_p = &g_camera_custom_fx_cntx.brightness_slider;	
			break;

		case CUSTOM_FX_SATURATION:
			slider_p = &g_camera_custom_fx_cntx.saturation_slider;	
			break;

		case CUSTOM_FX_CONTRAST:
			slider_p = &g_camera_custom_fx_cntx.contrast_slider;	
			break;

		case CUSTOM_FX_HUE:
			slider_p = &g_camera_custom_fx_cntx.hue_slider;	
			break;
	}

	spacing = (g_camera_custom_fx_cntx.spacing >> 1);

	x1 = spacing;
	x2 = UI_device_width-spacing-1;
	y1 = slider_p->offset_y-spacing;
	y2 = slider_p->offset_y+spacing+slider_p->height-1;
	
	gdi_draw_rect(x1, y1, x2, y2, GDI_COLOR_BLACK);

}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_key_press_cyclic()
* DESCRIPTION
*   custom fx left or right arrow key long press cyclic handler
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_key_press_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_camera_custom_fx_cntx.is_right_arrow_press)
	{
		mmi_camera_custom_fx_edit_right_arrow_press();
		coolsand_UI_start_timer(100, mmi_camera_custom_fx_edit_key_press_cyclic);	
	}
	
	if(g_camera_custom_fx_cntx.is_left_arrow_press)
	{
		mmi_camera_custom_fx_edit_left_arrow_press();
		coolsand_UI_start_timer(100, mmi_camera_custom_fx_edit_key_press_cyclic);	
	}
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_right_arrow_release()
* DESCRIPTION
*   custom fx left arrow key release
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_right_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camera_custom_fx_cntx.is_right_arrow_press = FALSE;
	coolsand_UI_cancel_timer(mmi_camera_custom_fx_edit_key_press_cyclic);
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_right_arrow_press()
* DESCRIPTION
*   custom fx right arrow key release
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_right_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camera_custom_fx_cntx.is_right_arrow_press = TRUE;

	coolsand_UI_lock_double_buffer();
	
	switch(g_camera_custom_fx_cntx.select_idx)
	{
		case CUSTOM_FX_BRIGHTNESS:
			
			if(g_camera_custom_fx_cntx.brightness_slider.value+5 <= 100)
				g_camera_custom_fx_cntx.brightness_slider.value+=5;
			else
			{
				coolsand_UI_unlock_double_buffer();		
				return;
			}

			coolsand_UI_set_horizontal_slide_value(&g_camera_custom_fx_cntx.brightness_slider.slide_bar, g_camera_custom_fx_cntx.brightness_slider.value);
			coolsand_UI_show_horizontal_slide(&g_camera_custom_fx_cntx.brightness_slider.slide_bar);
			mdi_camera_update_para_brightness((U16)(g_camera_custom_fx_cntx.brightness_slider.value*255/100));
						
			break;

		case CUSTOM_FX_SATURATION:
			
			if(g_camera_custom_fx_cntx.saturation_slider.value+5 <= 100)
				g_camera_custom_fx_cntx.saturation_slider.value+=5;
			else
			{
				coolsand_UI_unlock_double_buffer();		
				return;
			}

			coolsand_UI_set_horizontal_slide_value(&g_camera_custom_fx_cntx.saturation_slider.slide_bar, g_camera_custom_fx_cntx.saturation_slider.value);
			coolsand_UI_show_horizontal_slide(&g_camera_custom_fx_cntx.saturation_slider.slide_bar);	
			mdi_camera_update_para_saturation((U16)(g_camera_custom_fx_cntx.saturation_slider.value*255/100));
			
			break;

		case CUSTOM_FX_CONTRAST:
			
			if(g_camera_custom_fx_cntx.contrast_slider.value+5 <= 100)
				g_camera_custom_fx_cntx.contrast_slider.value+=5;
			else
			{
				coolsand_UI_unlock_double_buffer();		
				return;
			}

			coolsand_UI_set_horizontal_slide_value(&g_camera_custom_fx_cntx.contrast_slider.slide_bar, g_camera_custom_fx_cntx.contrast_slider.value);
			coolsand_UI_show_horizontal_slide(&g_camera_custom_fx_cntx.contrast_slider.slide_bar);		
			mdi_camera_update_para_contrast((U16)(g_camera_custom_fx_cntx.contrast_slider.value*255/100));
			
			break;

		case CUSTOM_FX_HUE:
			
			if(g_camera_custom_fx_cntx.hue_slider.value+10 <= 360)
				g_camera_custom_fx_cntx.hue_slider.value+=10;
			else
				g_camera_custom_fx_cntx.hue_slider.value = 0;

			coolsand_UI_set_horizontal_slide_value(&g_camera_custom_fx_cntx.hue_slider.slide_bar, g_camera_custom_fx_cntx.hue_slider.value);
			coolsand_UI_show_horizontal_slide(&g_camera_custom_fx_cntx.hue_slider.slide_bar);			
			mdi_camera_update_para_hue(g_camera_custom_fx_cntx.hue_slider.value);
			
			break;
	}

	mmi_camera_custom_fx_edit_draw_value(g_camera_custom_fx_cntx.select_idx);

	coolsand_UI_unlock_double_buffer();	
	gdi_layer_blt_previous(0,0,UI_device_width-1, UI_device_height-1);

	coolsand_UI_start_timer(100, mmi_camera_custom_fx_edit_key_press_cyclic);

}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_left_arrow_release()
* DESCRIPTION
*   custom fx left arrow key release
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_left_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camera_custom_fx_cntx.is_left_arrow_press = FALSE;
	coolsand_UI_cancel_timer(mmi_camera_custom_fx_edit_key_press_cyclic);
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_left_arrow_press()
* DESCRIPTION
*   custom fx left arrow key press
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_left_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camera_custom_fx_cntx.is_left_arrow_press = TRUE;

	coolsand_UI_lock_double_buffer();

	switch(g_camera_custom_fx_cntx.select_idx)
	{
		case CUSTOM_FX_BRIGHTNESS:
			
			if(g_camera_custom_fx_cntx.brightness_slider.value-5 >= 0)
				g_camera_custom_fx_cntx.brightness_slider.value-=5;
			else
			{
				coolsand_UI_unlock_double_buffer();		
				return;
			}

			coolsand_UI_set_horizontal_slide_value(&g_camera_custom_fx_cntx.brightness_slider.slide_bar, g_camera_custom_fx_cntx.brightness_slider.value);
			coolsand_UI_show_horizontal_slide(&g_camera_custom_fx_cntx.brightness_slider.slide_bar);
			mdi_camera_update_para_brightness((U16)(g_camera_custom_fx_cntx.brightness_slider.value*255/100));
						
			break;

		case CUSTOM_FX_SATURATION:
			
			if(g_camera_custom_fx_cntx.saturation_slider.value-5 >= 0)
				g_camera_custom_fx_cntx.saturation_slider.value-=5;
			else
			{
				coolsand_UI_unlock_double_buffer();		
				return;
			}

			coolsand_UI_set_horizontal_slide_value(&g_camera_custom_fx_cntx.saturation_slider.slide_bar, g_camera_custom_fx_cntx.saturation_slider.value);
			coolsand_UI_show_horizontal_slide(&g_camera_custom_fx_cntx.saturation_slider.slide_bar);	
			mdi_camera_update_para_saturation((U16)(g_camera_custom_fx_cntx.saturation_slider.value*255/100));
			
			break;

		case CUSTOM_FX_CONTRAST:
			
			if(g_camera_custom_fx_cntx.contrast_slider.value-5 >= 0)
				g_camera_custom_fx_cntx.contrast_slider.value-=5;
			else
			{
				coolsand_UI_unlock_double_buffer();		
				return;
			}

			coolsand_UI_set_horizontal_slide_value(&g_camera_custom_fx_cntx.contrast_slider.slide_bar, g_camera_custom_fx_cntx.contrast_slider.value);
			coolsand_UI_show_horizontal_slide(&g_camera_custom_fx_cntx.contrast_slider.slide_bar);		
			mdi_camera_update_para_contrast((U16)(g_camera_custom_fx_cntx.contrast_slider.value*255/100));
			
			break;

		case CUSTOM_FX_HUE:
			
			if(g_camera_custom_fx_cntx.hue_slider.value-10 >= 0)
				g_camera_custom_fx_cntx.hue_slider.value-=10;
			else
				g_camera_custom_fx_cntx.hue_slider.value = 360;

			coolsand_UI_set_horizontal_slide_value(&g_camera_custom_fx_cntx.hue_slider.slide_bar, g_camera_custom_fx_cntx.hue_slider.value);
			coolsand_UI_show_horizontal_slide(&g_camera_custom_fx_cntx.hue_slider.slide_bar);			
			mdi_camera_update_para_hue(g_camera_custom_fx_cntx.hue_slider.value);
			
			break;
	}

	mmi_camera_custom_fx_edit_draw_value(g_camera_custom_fx_cntx.select_idx);
	coolsand_UI_unlock_double_buffer();		
	
	gdi_layer_blt_previous(0,0,UI_device_width-1, UI_device_height-1);

	coolsand_UI_start_timer(100, mmi_camera_custom_fx_edit_key_press_cyclic);	
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_up_arrow_press()
* DESCRIPTION
*   custom fx up arrow key press
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_up_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 prev_idx;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	prev_idx = g_camera_custom_fx_cntx.select_idx;

	if(g_camera_custom_fx_cntx.select_idx > CUSTOM_FX_BRIGHTNESS)
		g_camera_custom_fx_cntx.select_idx--;
	else
		g_camera_custom_fx_cntx.select_idx = CUSTOM_FX_HUE;

	gdi_layer_lock_frame_buffer();
	
	mmi_camera_custom_fx_edit_draw_title();
	mmi_camera_custom_fx_edit_draw_hilight_item(g_camera_custom_fx_cntx.select_idx, prev_idx);
	mmi_camera_custom_fx_edit_draw_value(g_camera_custom_fx_cntx.select_idx);

	gdi_layer_unlock_frame_buffer();
	
	gdi_layer_blt_previous(0,0,UI_device_width-1, UI_device_height-1);	
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_down_arrow_press()
* DESCRIPTION
*   custom fx down arrow key press
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_down_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 prev_idx;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	prev_idx = g_camera_custom_fx_cntx.select_idx;

	if(g_camera_custom_fx_cntx.select_idx < CUSTOM_FX_HUE)
		g_camera_custom_fx_cntx.select_idx++;
	else
		g_camera_custom_fx_cntx.select_idx = CUSTOM_FX_BRIGHTNESS;

	gdi_layer_lock_frame_buffer();

	mmi_camera_custom_fx_edit_draw_title();	
	mmi_camera_custom_fx_edit_draw_hilight_item(g_camera_custom_fx_cntx.select_idx, prev_idx);
	mmi_camera_custom_fx_edit_draw_value(g_camera_custom_fx_cntx.select_idx);	
	
	gdi_layer_unlock_frame_buffer();
	
	gdi_layer_blt_previous(0,0,UI_device_width-1, UI_device_height-1);	
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_draw_value()
* DESCRIPTION
*   draw item's current value
* PARAMETERS
*	 idx 	IN		index to be draw
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_draw_value(U16 idx)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	custom_fx_slider_struct *slider_p;
	S32 offset_x;
	S32 offset_y;
	S32 width;
	S32 height;
	U16 value;
	S8  buf[10];
	S8  buf_UCS2[20];	
	S32 str_width;
	S32 str_height;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	switch(idx)
	{
		case CUSTOM_FX_BRIGHTNESS:
			slider_p = &g_camera_custom_fx_cntx.brightness_slider;	
			value		= g_camera_custom_fx_cntx.brightness_slider.value;
			break;

		case CUSTOM_FX_SATURATION:
			slider_p = &g_camera_custom_fx_cntx.saturation_slider;
			value		= g_camera_custom_fx_cntx.saturation_slider.value;
			break;

		case CUSTOM_FX_CONTRAST:
			slider_p = &g_camera_custom_fx_cntx.contrast_slider;	
			value		= g_camera_custom_fx_cntx.contrast_slider.value;
			break;

		case CUSTOM_FX_HUE:
			slider_p = &g_camera_custom_fx_cntx.hue_slider;
			value		= g_camera_custom_fx_cntx.hue_slider.value;
			break;
	}

	coolsand_UI_reset_text_clip();
	gdi_layer_reset_clip();

	sprintf(buf, "%d", value);
	AnsiiToUnicodeString(buf_UCS2, buf);
	coolsand_UI_measure_string((UI_string_type)buf_UCS2, &str_width, &str_height);

	offset_x = slider_p->offset_x + slider_p->width + (g_camera_custom_fx_cntx.spacing >> 1);
	offset_y = slider_p->offset_y;
	width		= g_camera_custom_fx_cntx.num_width;
	height	= g_camera_custom_fx_cntx.num_height;	

	gdi_draw_solid_rect(offset_x, offset_y, offset_x+width-1, offset_y+height-1, GDI_COLOR_WHITE);

	offset_x += width-str_width;
	offset_y += ((height-str_height)>>1);
	
	coolsand_UI_move_text_cursor(offset_x, offset_y);
	coolsand_UI_set_font(&MMI_medium_font);
	coolsand_UI_set_text_color(coolsand_UI_color(0, 0 ,0));
   coolsand_UI_print_text((UI_string_type)buf_UCS2);
	
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_custom_fx_edit_save()
* DESCRIPTION
*   save edited value
* PARAMETERS
*	 void 
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_entry_custom_fx_edit_save(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* rescale brightness contrast hue from 0-100 back to 0-255 */
	g_camera_cntx.setting.fx_brightness[g_camera_custom_fx_cntx.fx_idx] = 
			g_camera_custom_fx_cntx.brightness_slider.value*255/100;
	
	g_camera_cntx.setting.fx_saturation[g_camera_custom_fx_cntx.fx_idx] = 
		g_camera_custom_fx_cntx.saturation_slider.value*255/100;
	
	g_camera_cntx.setting.fx_contrast[g_camera_custom_fx_cntx.fx_idx] = 
		g_camera_custom_fx_cntx.contrast_slider.value*255/100;
	
	g_camera_cntx.setting.fx_hue[g_camera_custom_fx_cntx.fx_idx] = 
		g_camera_custom_fx_cntx.hue_slider.value;

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	#else
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteNHistory(1);
	
	mmi_camera_store_setting();
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_clam_close_hdlr()
* DESCRIPTION
*   clam close handler
* PARAMETERS
*	 ptr 	IN 	pointer to message 
* RETURNS
*   U8
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
U8 mmi_camera_custom_fx_clam_close_hdlr(void* ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
#if defined(__MMI_CLAMSHELL__) && defined(MMI_ON_HARDWARE_P)
	mmi_eq_gpio_detect_ind_struct *gpioDetectInd;
	
	gpioDetectInd = (mmi_eq_gpio_detect_ind_struct *)ptr;
	
	if(gpioDetectInd->gpio_device == EXT_DEV_CLAM_CLOSE)
	{
		GoBackHistory();
		return FALSE;	
	}
#endif /* __MMI_CLAMSHELL__ && MMI_ON_HARDWARE_P */

	/* return false means will pass this interrut to default handler */
	return FALSE;	
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_custom_fx_edit_sublcd_screen()
* DESCRIPTION
*   exit sublcd screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
#ifdef __MMI_SUBLCD__
void mmi_camera_exit_custom_fx_edit_sublcd_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SubLCDHistoryNode SubLCDHistory;

	/* someone may exit/draw sublcd first then exit mainlcd, this may cause driver assert */
	mdi_camera_preview_stop();
	
	SubLCDHistory.entryFuncPtr = mmi_camera_entry_custom_fx_edit_sublcd_screen;
	AddSubLCDHistory(&SubLCDHistory);
}
#endif /* __MMI_SUBLCD__ */
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_custom_fx_edit_sublcd_screen()
* DESCRIPTION
*   entry sublcd screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
#ifdef __MMI_SUBLCD__
void mmi_camera_entry_custom_fx_edit_sublcd_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	ExecSubLCDCurrExitHandler();
	ShowCategory331Screen();
	mmi_camera_sublcd_draw_icon();
	SetSubLCDExitHandler(mmi_camera_exit_custom_fx_edit_sublcd_screen);
}
#endif /* __MMI_SUBLCD__ */
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_custom_fx_edit_draw_title()
* DESCRIPTION
*   draw custom fx's title, will draw current selected item's name
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_custom_fx_edit_draw_title(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	switch(g_camera_custom_fx_cntx.select_idx)
	{
		case CUSTOM_FX_BRIGHTNESS:
			ChangeTitle(0, (PU8)GetString(STR_ID_CAMERA_EFFECT_CUSTOM_FX_BRIGHTNESS));
			break;

		case CUSTOM_FX_SATURATION:
			ChangeTitle(0, (PU8)GetString(STR_ID_CAMERA_EFFECT_CUSTOM_FX_SATURATION));
			break;

		case CUSTOM_FX_CONTRAST:
			ChangeTitle(0, (PU8)GetString(STR_ID_CAMERA_EFFECT_CUSTOM_FX_CONTRAST));
			break;
			
		case CUSTOM_FX_HUE:
			ChangeTitle(0, (PU8)GetString(STR_ID_CAMERA_EFFECT_CUSTOM_FX_HUE));
			break;
	}

	draw_title();

}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_custom_fx_edit_screen()
* DESCRIPTION
*   exit edit screen
* PARAMETERS
*	 void 
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_exit_custom_fx_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* stop preview and power off */
	mdi_camera_preview_stop();
	mdi_camera_power_off();
	
	/* merge layers */
	gdi_layer_flatten_previous_to_base();

	/* free layers */
	gdi_layer_free(g_camera_custom_fx_cntx.preview_wnd_layer);
	gdi_layer_multi_layer_disable();
	
	coolsand_UI_cancel_timer(mmi_camera_custom_fx_edit_key_press_cyclic);

	/* re-enable keypad tone */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);

	#ifdef MMI_ON_HARDWARE_P
		/* un-hook clam close msg */
		SetInterruptEventHandler(NULL, NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	#endif

#ifdef __MMI_SUBLCD__	
	/* leave sublcd screen */
	GoBackSubLCDHistory();
#endif
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_custom_fx_edit_screen()
* DESCRIPTION
*   entry custom fx screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_entry_custom_fx_edit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		*guiBuffer;	
	S32	str_width;
	S32 	str_height;
	S32 	preview_wnd_height;
	S32 	preview_wnd_width;
	S32 	preview_wnd_offset_x;
	S32 	preview_wnd_offset_y;
	S32 	num_width;
	S32 	num_height;	
	S32 	spacing;
	S32	image_width;
	S32 	image_height;
	S32 	image_spacing;
	S32 	image_offset_x;
	S32	image_offset_y;
	S32	remain;

	mdi_camera_setting_struct	camera_setting_data;	
	custom_fx_slider_struct 	*slider_p;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	EntryNewScreen(STR_ID_CAMERA_EFFECT_CUSTOM_FX_EDIT_EFFECT, 
						mmi_camera_exit_custom_fx_edit_screen, 
						mmi_camera_entry_custom_fx_edit_screen, 
						NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif
	
	/* entry sublcd - avoid sulcd update */
	#ifdef __MMI_SUBLCD__
		ForceSubLCDScreen(mmi_camera_entry_custom_fx_edit_sublcd_screen);
	#endif 

	guiBuffer = GetCurrGuiBuffer(STR_ID_CAMERA_EFFECT_CUSTOM_FX_EDIT_EFFECT);

	/* force all playing keypad tone off */
	AudioStopReq(GetCurKeypadTone());

	/* disable key pad tone */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);

	g_camera_custom_fx_cntx.is_left_arrow_press = FALSE;
	g_camera_custom_fx_cntx.is_right_arrow_press = FALSE;	

	/* if first time enter */
	if(guiBuffer == NULL)
	{
		g_camera_custom_fx_cntx.select_idx = CUSTOM_FX_BRIGHTNESS;
		
		/* rescale brightness contrast stauration from 0-255 to 0-100 */		
		g_camera_custom_fx_cntx.brightness_slider.value	= g_camera_cntx.setting.fx_brightness[g_camera_custom_fx_cntx.fx_idx]*100/255;
		g_camera_custom_fx_cntx.saturation_slider.value = g_camera_cntx.setting.fx_saturation[g_camera_custom_fx_cntx.fx_idx]*100/255;
		g_camera_custom_fx_cntx.contrast_slider.value	= g_camera_cntx.setting.fx_contrast[g_camera_custom_fx_cntx.fx_idx]*100/255;
		g_camera_custom_fx_cntx.hue_slider.value			= g_camera_cntx.setting.fx_hue[g_camera_custom_fx_cntx.fx_idx];

		/* sometime rounding will get a value like 4, or 1, and we use 5 as out minium step */
		/* rounding them */

		/* brightness rounding */
		remain = g_camera_custom_fx_cntx.brightness_slider.value%5;
		
		if(remain != 0)
		{
			if(remain < 3)
			{
				g_camera_custom_fx_cntx.brightness_slider.value -= remain ;
			}
			else
			{
				g_camera_custom_fx_cntx.brightness_slider.value += (5-remain);
			}
		}

		/* saturation rounding */
		remain = g_camera_custom_fx_cntx.saturation_slider.value%5;
		
		if(remain != 0)
		{
			if(remain < 3)
			{
				g_camera_custom_fx_cntx.saturation_slider.value -= remain ;
			}
			else
			{
				g_camera_custom_fx_cntx.saturation_slider.value += (5-remain);
			}
		}

		/* contrast rounding */
		remain = g_camera_custom_fx_cntx.contrast_slider.value%5;
		
		if(remain != 0)
		{
			if(remain < 3)
			{
				g_camera_custom_fx_cntx.contrast_slider.value -= remain ;
			}
			else
			{
				g_camera_custom_fx_cntx.contrast_slider.value += (5-remain);
			}
		}

		/* hue rounding */
		remain = g_camera_custom_fx_cntx.hue_slider.value%10;
		
		if(remain != 0)
		{
			if(remain < 5)
			{
				g_camera_custom_fx_cntx.hue_slider.value -= remain ;
			}
			else
			{
				g_camera_custom_fx_cntx.hue_slider.value += (10-remain);
			}
		}
		
	}

	/* avoid draw empty category first */
	gdi_layer_lock_frame_buffer();
		
	/* draw a emptry category */
	ShowCategory221Screen(	STR_GLOBAL_OK,	/* just give it a string, for category221 to draw status and title bar */
									0,
									STR_GLOBAL_SAVE, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									GDI_COLOR_WHITE,
									NULL);
	
	gdi_layer_unlock_frame_buffer();
		

	mmi_camera_custom_fx_edit_draw_title();
	
	SetKeyHandler(mmi_camera_custom_fx_edit_down_arrow_press, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_camera_custom_fx_edit_up_arrow_press, KEY_UP_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_camera_custom_fx_edit_left_arrow_press, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_camera_custom_fx_edit_right_arrow_press, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_custom_fx_edit_left_arrow_release, KEY_LEFT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_custom_fx_edit_right_arrow_release, KEY_RIGHT_ARROW, KEY_EVENT_UP);	
	
	SetLeftSoftkeyFunction(mmi_camera_entry_custom_fx_edit_save, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	gdi_layer_lock_frame_buffer();

	coolsand_UI_set_font(&MMI_medium_font);
	coolsand_UI_measure_string((UI_string_type)GetString(STR_ID_CAMERA_EFFECT_CUSTOM_FX_BRIGHTNESS), &str_width, &str_height);
	coolsand_UI_measure_string((UI_string_type)L"255",&num_width, &num_height);

 	spacing = str_height >> 1;
	g_camera_custom_fx_cntx.spacing 		= spacing;
	g_camera_custom_fx_cntx.num_width 	= num_width;
	g_camera_custom_fx_cntx.num_height 	= num_height;
	
	preview_wnd_height		= MMI_content_height - (str_height*4 + spacing*6);
	preview_wnd_width 		= UI_device_width - spacing*2;
	preview_wnd_offset_x 	= spacing;
	preview_wnd_offset_y 	= MMI_title_height+MMI_title_y+spacing;

	gdi_layer_create(	preview_wnd_offset_x, 
							preview_wnd_offset_y, 
							preview_wnd_width, 
							preview_wnd_height, 
							&g_camera_custom_fx_cntx.preview_wnd_layer);

	gdi_layer_push_and_set_active(g_camera_custom_fx_cntx.preview_wnd_layer);
	gdi_layer_clear_background(GDI_COLOR_BLACK);
	gdi_layer_pop_and_restore_active();

	gdi_layer_get_base_handle(&g_camera_custom_fx_cntx.base_layer);
		
	gdi_layer_set_blt_layer(g_camera_custom_fx_cntx.base_layer, g_camera_custom_fx_cntx.preview_wnd_layer, 0, 0);
	



	/* brightness */
	slider_p = &g_camera_custom_fx_cntx.brightness_slider;
	
	slider_p->width 		= UI_device_width-spacing*4-num_width*2;
	slider_p->height 		= str_height;

	slider_p->offset_x 	= spacing*2+num_width;
	slider_p->offset_y 	= MMI_title_height+MMI_title_y+preview_wnd_height+spacing*2;

	coolsand_UI_create_horizontal_slide(&slider_p->slide_bar, slider_p->offset_x, slider_p->offset_y, slider_p->width, slider_p->height);
	coolsand_UI_set_horizontal_slide_range(&slider_p->slide_bar, 100);
	coolsand_UI_set_horizontal_slide_scale(&slider_p->slide_bar, 1);		
	coolsand_UI_set_horizontal_slide_value(&slider_p->slide_bar, slider_p->value);	
	coolsand_UI_show_horizontal_slide(&slider_p->slide_bar);


	gdi_image_get_dimension_id(IMG_ID_CAMERA_CUSTOM_FX_BIRIGHTNESS, &image_width, &image_height);

	image_spacing 	= (slider_p->height - image_height)>>1;
	image_offset_x = spacing+image_spacing+((num_width-image_width)>>1);
	image_offset_y = slider_p->offset_y+image_spacing;
	
	gdi_image_draw_id(image_offset_x, image_offset_y, IMG_ID_CAMERA_CUSTOM_FX_BIRIGHTNESS);

	mmi_camera_custom_fx_edit_draw_value(CUSTOM_FX_BRIGHTNESS);

	/* saturation */
	slider_p = &g_camera_custom_fx_cntx.saturation_slider;
	
	slider_p->width 		= UI_device_width-spacing*4-num_width*2;
	slider_p->height 		= str_height;

	slider_p->offset_x 	= spacing*2+num_width;
	slider_p->offset_y 	= MMI_title_height+MMI_title_y+preview_wnd_height+str_height+spacing*3;

	coolsand_UI_create_horizontal_slide(&slider_p->slide_bar, slider_p->offset_x, slider_p->offset_y, slider_p->width, slider_p->height);
	coolsand_UI_set_horizontal_slide_range(&slider_p->slide_bar, 100);
	coolsand_UI_set_horizontal_slide_scale(&slider_p->slide_bar, 1);		
	coolsand_UI_set_horizontal_slide_value(&slider_p->slide_bar, slider_p->value);	
	coolsand_UI_show_horizontal_slide(&slider_p->slide_bar);


	gdi_image_get_dimension_id(IMG_ID_CAMERA_CUSTOM_FX_SATURATION, &image_width, &image_height);

	image_spacing 	= (slider_p->height - image_height)>>1;
	image_offset_x = spacing+image_spacing+((num_width-image_width)>>1);
	image_offset_y = slider_p->offset_y+image_spacing;
	
	gdi_image_draw_id(image_offset_x, image_offset_y, IMG_ID_CAMERA_CUSTOM_FX_SATURATION);

	mmi_camera_custom_fx_edit_draw_value(CUSTOM_FX_SATURATION);


	/* contrast */
	slider_p = &g_camera_custom_fx_cntx.contrast_slider;
	
	slider_p->width 		= UI_device_width-spacing*4-num_width*2;
	slider_p->height 		= str_height;

	slider_p->offset_x 	= spacing*2+num_width;
	slider_p->offset_y 	= MMI_title_height+MMI_title_y+preview_wnd_height+str_height*2+spacing*4;

	coolsand_UI_create_horizontal_slide(&slider_p->slide_bar, slider_p->offset_x, slider_p->offset_y, slider_p->width, slider_p->height);
	coolsand_UI_set_horizontal_slide_range(&slider_p->slide_bar, 100);
	coolsand_UI_set_horizontal_slide_scale(&slider_p->slide_bar, 1);		
	coolsand_UI_set_horizontal_slide_value(&slider_p->slide_bar, slider_p->value);	
	coolsand_UI_show_horizontal_slide(&slider_p->slide_bar);


	gdi_image_get_dimension_id(IMG_ID_CAMERA_CUSTOM_FX_CONTRAST, &image_width, &image_height);

	image_spacing 	= (slider_p->height - image_height)>>1;
	image_offset_x = spacing+image_spacing+((num_width-image_width)>>1);
	image_offset_y = slider_p->offset_y+image_spacing;
	
	gdi_image_draw_id(image_offset_x, image_offset_y, IMG_ID_CAMERA_CUSTOM_FX_CONTRAST);

	mmi_camera_custom_fx_edit_draw_value(CUSTOM_FX_CONTRAST);

	/* Hue */
	slider_p = &g_camera_custom_fx_cntx.hue_slider;
	
	slider_p->width 		= UI_device_width-spacing*4-num_width*2;
	slider_p->height 		= str_height;

	slider_p->offset_x 	= spacing*2+num_width;
	slider_p->offset_y 	= MMI_title_height+MMI_title_y+preview_wnd_height+str_height*3+spacing*5;

	coolsand_UI_create_horizontal_slide(&slider_p->slide_bar, slider_p->offset_x, slider_p->offset_y, slider_p->width, slider_p->height);
	coolsand_UI_set_horizontal_slide_range(&slider_p->slide_bar, 360);
	coolsand_UI_set_horizontal_slide_scale(&slider_p->slide_bar, 1);		
	coolsand_UI_set_horizontal_slide_value(&slider_p->slide_bar, slider_p->value);	
	coolsand_UI_show_horizontal_slide(&slider_p->slide_bar);

	gdi_image_get_dimension_id(IMG_ID_CAMERA_CUSTOM_FX_HUE, &image_width, &image_height);

	image_spacing 	= (slider_p->height - image_height)>>1;
	image_offset_x = spacing+image_spacing+((num_width-image_width)>>1);
	image_offset_y = slider_p->offset_y+image_spacing;
	
	gdi_image_draw_id(image_offset_x, image_offset_y, IMG_ID_CAMERA_CUSTOM_FX_HUE);
	mmi_camera_custom_fx_edit_draw_value(CUSTOM_FX_HUE);


	mmi_camera_custom_fx_edit_draw_hilight_item(g_camera_custom_fx_cntx.select_idx, g_camera_custom_fx_cntx.select_idx);

	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0,0,UI_device_width-1, UI_device_height-1);


	/*********** start preview ************/

	/* stop MMI sleep */
	TurnOnBacklight(0);

#if defined(__MMI_CLAMSHELL__) && defined(MMI_ON_HARDWARE_P)
	/* register event hdlr */
	SetInterruptEventHandler(mmi_camera_custom_fx_clam_close_hdlr, NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
#endif /* __MMI_CLAMSHELL__ && MMI_ON_HARDWARE_P */

	/* stop LCD patten */
	StopLEDPatternBackGround();

	/* power on and init hardware module */
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
        CSD_UI_Free_bmpbuf();
#endif  
	mdi_camera_power_on();

	 
	camera_setting_data.zoom				= 10;
	camera_setting_data.wb					= camera_wb_command_map[g_camera_cntx.setting.wb];	
	camera_setting_data.ev					= camera_ev_command_map[g_camera_cntx.setting.ev];
	camera_setting_data.banding 			= camera_banding_command_map[g_camera_cntx.setting.banding];
	
	camera_setting_data.night				= g_camera_cntx.setting.night;
	camera_setting_data.flash				= camera_flash_command_map[g_camera_cntx.setting.flash];
	camera_setting_data.ae_meter			= camera_ae_meter_command_map[g_camera_cntx.setting.ae_meter];
	camera_setting_data.iso					= camera_iso_command_map[g_camera_cntx.setting.iso];
	
	camera_setting_data.shot_mode			= g_camera_cntx.setting.shot_mode;
	
//	camera_setting_data.effect				= camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL];
	camera_setting_data.effect				= camera_effect_command_map[0];  //modified by WeiD
	camera_setting_data.brightness 		= g_camera_custom_fx_cntx.brightness_slider.value*255/100;
	camera_setting_data.saturation 		= g_camera_custom_fx_cntx.saturation_slider.value*255/100;	
	camera_setting_data.contrast 			= g_camera_custom_fx_cntx.contrast_slider.value*255/100;	
	camera_setting_data.hue		 			= g_camera_custom_fx_cntx.hue_slider.value;		
	
	camera_setting_data.preview_width	= preview_wnd_width;
	camera_setting_data.preview_height	= preview_wnd_height;	
	
	/* capture parameters */
	camera_setting_data.image_qty			= camera_image_qty_command_map[g_camera_cntx.setting.image_qty];
	camera_setting_data.image_width		= g_camera_cntx.image_width;
	camera_setting_data.image_height		= g_camera_cntx.image_height;	

	camera_setting_data.lcm					= MDI_CAMERA_PREVIEW_LCM_MAINLCD;
	camera_setting_data.preview_rotate	= CAMERA_PREVIEW_MAINLCD_ROTATE;
	camera_setting_data.lcm_rotate		= CAMERA_LCM_MAINLCD_ROTATE;
#if defined(CAMERA_MODULE)
	/* start preview */
	mdi_camera_preview_start(	g_camera_custom_fx_cntx.preview_wnd_layer,
										(S32)0,
										(S32)0,
										(U16)preview_wnd_width,
										(U16)preview_wnd_height,
										GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1,
										GDI_LAYER_ENABLE_LAYER_1,
//										GDI_COLOR_TRANSPARENT,
										GDI_COLOR_BLACK,//GDI_COLOR_TRANSPARENT,//fengzb
										TRUE,
										&camera_setting_data); 

#elif defined(ISP_SUPPORT)
	/* start preview */
	mdi_camera_preview_start(	g_camera_custom_fx_cntx.preview_wnd_layer,
										(S32)0,
										(S32)0,
										(U16)preview_wnd_width,
										(U16)preview_wnd_height,
										GDI_LAYER_ENABLE_LAYER_0,
										GDI_LAYER_ENABLE_LAYER_1,
										0,	 
										TRUE,
										&camera_setting_data);
#endif
	
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_custom_fx_set()
* DESCRIPTION
*   entry custom fx screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_entry_custom_fx_set(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	#else
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteUptoScrID(SCR_ID_CAMERA_OPTION);

	g_camera_cntx.setting.effect = g_camera_cntx.selected_effect;
	
	mmi_camera_store_setting();
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_custom_fx_set()
* DESCRIPTION
*   entry custom fx screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_highlight_custom_fx_set(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_entry_custom_fx_set,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);	
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_custom_fx_edit()
* DESCRIPTION
*   highlight hdlr of edit menu item
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_highlight_custom_fx_edit(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_entry_custom_fx_edit_screen,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);	
}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_custom_fx_option_screen()
* DESCRIPTION
*   custom fx option screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
void mmi_camera_entry_custom_fx_option_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8			*guiBuffer;	

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_CAMERA_EFFECT_SET_OPTION, NULL, mmi_camera_entry_custom_fx_option_screen, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif
	
	guiBuffer	= GetCurrGuiBuffer( SCR_ID_CAMERA_EFFECT_SET_OPTION );
	nItems		= GetNumOfChild(MENU_ID_CAMERA_EFFECT_CUSTOM_FX_SET_OPTION);
	GetSequenceStringIds(MENU_ID_CAMERA_EFFECT_CUSTOM_FX_SET_OPTION,ItemList);
	GetSequenceImageIds(MENU_ID_CAMERA_EFFECT_CUSTOM_FX_SET_OPTION,ItemIcons);
	SetParentHandler(MENU_ID_CAMERA_EFFECT_CUSTOM_FX_SET_OPTION);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory11Screen(	STR_ID_CAMERA_EFFECT_SETTING, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									nItems, 
									ItemList,
									0,				
									guiBuffer);



}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */


#if defined(__CAMERA_FEATURE_EFFECT__)


/*****************************************************************************
*
* Camera Effect Setting Menu     
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_hint_effect_setting()
* DESCRIPTION
*   effect setting hint handler
* PARAMETERS
*	 index 	IN		index of selected item
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_hint_effect_setting(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	memset( (PS8)hintData[(index)], 0, sizeof(hintData[(index)]));
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_CAMERA_EFFECT_START+g_camera_cntx.setting.effect+1) ));
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_effect_setting()
* DESCRIPTION
*   effect setting highlight handler
* PARAMETERS
*	 index 	IN		index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_effect_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_entry_effect_setting_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_entry_effect_setting_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_set_effect_setting_value()
* DESCRIPTION
*   set effect
* PARAMETERS
*	void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_set_effect_setting_value(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
	if(g_camera_cntx.selected_effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_1)
	{
		g_camera_custom_fx_cntx.fx_idx = 0;
		mmi_camera_entry_custom_fx_option_screen();
		return;
	}
#endif

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_2__
	if(g_camera_cntx.selected_effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_2)
	{
		g_camera_custom_fx_cntx.fx_idx = 1;	
		mmi_camera_entry_custom_fx_option_screen();
		return;
	}
#endif

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_3__
	if(g_camera_cntx.selected_effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_3)
	{
		g_camera_custom_fx_cntx.fx_idx = 2;		
		mmi_camera_entry_custom_fx_option_screen();
		return;
	}
#endif

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	#else
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteNHistory(1);
	
	g_camera_cntx.setting.effect = g_camera_cntx.selected_effect;

	mmi_camera_store_setting();
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_effect_setting_highlight_hdlr()
* DESCRIPTION
*   effect setting highlight handler 
* PARAMETERS
*	 idx		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_effect_setting_highlight_hdlr(S32 idx)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* set selected effect */
	g_camera_cntx.selected_effect = idx;
	
#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
	if(idx == CAMERA_SETTING_EFFECT_CUSTOM_FX_1)
	{
		g_camera_custom_fx_cntx.fx_idx = 0;		
		SetKeyHandler(mmi_camera_entry_custom_fx_option_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		return;
	}
#endif

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_2__
	if(idx == CAMERA_SETTING_EFFECT_CUSTOM_FX_2)
	{
		g_camera_custom_fx_cntx.fx_idx = 1;		
		SetKeyHandler(mmi_camera_entry_custom_fx_option_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		return;
	}
#endif

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_3__
	if(idx == CAMERA_SETTING_EFFECT_CUSTOM_FX_3)
	{
		g_camera_custom_fx_cntx.fx_idx = 2;		
		SetKeyHandler(mmi_camera_entry_custom_fx_option_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		return;
	}
#endif

	/* not custom fx */
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);



}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_effect_setting_screen()
* DESCRIPTION
*   entry effect setting screen
* PARAMETERS
*	void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_effect_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8			*guiBuffer;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_CAMERA_EFFECT_SETTING, NULL, mmi_camera_entry_effect_setting_screen, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CAMERA_EFFECT_SETTING);

	nItems		= GetNumOfChild(MENU_ID_CAMERA_EFFECT_SETTING);
	GetSequenceStringIds(MENU_ID_CAMERA_EFFECT_SETTING,ItemList);
	mmi_trace(1,"mmi_camera_entry_effect_setting_screen,MENU_ID_CAMERA_EFFECT_SETTING is %d,MENU_ID_CAMERA_EFFECT_NORMAL is %d",MENU_ID_CAMERA_EFFECT_SETTING,MENU_ID_CAMERA_EFFECT_NORMAL);
	GetSequenceImageIds(MENU_ID_CAMERA_EFFECT_SETTING,ItemIcons);
	SetParentHandler(MENU_ID_CAMERA_EFFECT_SETTING);

	RegisterHighlightHandler(mmi_camera_effect_setting_highlight_hdlr);

	ShowCategory11Screen(	STR_ID_CAMERA_EFFECT_SETTING, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									nItems, 
									ItemList,
									g_camera_cntx.setting.effect,				/* current index */
									guiBuffer);

	SetLeftSoftkeyFunction(mmi_camera_set_effect_setting_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


#endif



/*****************************************************************************
*
* Camera Shot Mode Menu     
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_hint_shot_mode()
* DESCRIPTION
*   effect setting hint handler
* PARAMETERS
*	 index 	IN		index of selected item
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_hint_shot_mode(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	memset( (PS8)hintData[(index)], 0, sizeof(hintData[(index)]));
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_CAMERA_SHOT_MODE_START+g_camera_cntx.setting.shot_mode+1) ));
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_shot_mode()
* DESCRIPTION
*   effect setting highlight handler
* PARAMETERS
*	 index 	IN		index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_shot_mode(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_entry_shot_mode_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_entry_shot_mode_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_set_shot_mode_value()
* DESCRIPTION
*   set effect
* PARAMETERS
*	void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_set_shot_mode_value(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI__
		if(g_camera_cntx.selected_shot_mode== CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI)
		{
			mmi_camera_entry_shot_mode_setting_screen();
			return;
		}
	#endif
	
	#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI__

		if(g_camera_cntx.selected_shot_mode== CAMERA_SETTING_SHOT_MODE_APERTURE_PRI)
		{
			mmi_camera_entry_shot_mode_setting_screen();
			return;
		}
	#endif



	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	#else
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteNHistory(1);
	
	g_camera_cntx.setting.shot_mode = g_camera_cntx.selected_shot_mode;

	mmi_camera_store_setting();
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_shot_mode_highlight_hdlr()
* DESCRIPTION
*   effect setting highlight handler 
* PARAMETERS
*	 idx		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_shot_mode_highlight_hdlr(S32 idx)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* set selected effect */
	g_camera_cntx.selected_shot_mode = idx;

	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI__
		if(idx == CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI)
		{
			SetKeyHandler(mmi_camera_entry_shot_mode_setting_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
			return;
		}
	#endif
	
	#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI__

		if(idx == CAMERA_SETTING_SHOT_MODE_APERTURE_PRI)
		{
			SetKeyHandler(mmi_camera_entry_shot_mode_setting_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
			return;
		}
	#endif
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
}

/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_shot_mode_setting_screen()
* DESCRIPTION
*   exit shot mode setting screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_exit_shot_mode_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 		inputBufferSize;                
	S16		nHistory = 0;
	history_t 	h;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	CloseCategory57Screen();       
	h.scrnID			= SCR_ID_CAMERA_SHOT_MODE_SETTING;
	h.entryFuncPtr = mmi_camera_entry_image_setting_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	inputBufferSize = (U16)GetCategory57DataSize(); 
	GetCategory57History (h.guiBuffer);
	GetCategory57Data ((U8*)h.inputBuffer);       
	AddNHistory(h, inputBufferSize);          
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_shot_mode_setting_set_value()
* DESCRIPTION
*   set shot mode setting value
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_shot_mode_setting_set_value(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI__
		if(g_camera_cntx.selected_shot_mode == CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI)
		{
			g_camera_cntx.setting.shutter_pri = g_camera_menu_cntx.camera_setting_inline.shot_mode_shutter_pri_selected;
		}
	#endif
	
	#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI__

		if(	g_camera_cntx.selected_shot_mode == CAMERA_SETTING_SHOT_MODE_APERTURE_PRI)
		{
			g_camera_cntx.setting.aperture_pri = g_camera_menu_cntx.camera_setting_inline.shot_mode_aperture_pri_selected;
		}
	#endif
	g_camera_cntx.setting.shot_mode = g_camera_cntx.selected_shot_mode;
	mmi_camera_store_setting();

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	#else
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteNHistory(1);	
}

/*****************************************************************************
* FUNCTION
*	 mmi_camera_shot_mode_setting_highlight_hdlr()
* DESCRIPTION
*   shot mode setting screen's highlight handler
* PARAMETERS
*	 index 	IN		selected item's index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_shot_mode_setting_highlight_hdlr(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(mmi_camera_shot_mode_setting_set_value);
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_shot_mode_setting_screen()
* DESCRIPTION
*   entry image setting screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

void mmi_camera_entry_shot_mode_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		*guiBuffer;
	U8		*inputBuffer;
	U16	inputBufferSize;
	S32	item_offset = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	UI_image_ID_type item_icons[CAMERA_SHOT_MODE_SETTING_ITEM_COUNT*2] =
	{
			0,
			0,
	};

	EntryNewScreen(SCR_ID_CAMERA_SHOT_MODE_SETTING, mmi_camera_exit_shot_mode_setting_screen, NULL, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif

	InitializeCategory57Screen();
	
	g_camera_menu_cntx.camera_setting_inline.shot_mode_selected= g_camera_cntx.setting.shot_mode;

	/****************************************************************************
	*  Create inline selection                                                               
	*****************************************************************************/

	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI__
		if(g_camera_cntx.selected_shot_mode == CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI)
		{
			S32	i;

			item_icons[0] = IMG_ID_CAMERA_SHOT_MODE_SHUTTER_PRI;
			/* image size inline selection */
			for(i = 0 ; i < CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI_COUNT; i++)
			{
				g_camera_menu_cntx.camera_setting_inline.shot_mode_shutter_pri_str[i] 
					= (UI_string_type)GetString(g_camera_cntx.shot_mode_shutter_pri_str_id[i]);
			}
	
			SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_CAMERA_SHOT_MODE_SHUTTER_PRI)));
		
			SetInlineItemActivation(&wgui_inline_items[item_offset], 
											INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
											0 );
	
			SetInlineItemSelect(	&wgui_inline_items[item_offset++],
										CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI_COUNT,
										(U8**)g_camera_menu_cntx.camera_setting_inline.shot_mode_shutter_pri_str,
										&g_camera_menu_cntx.camera_setting_inline.shot_mode_shutter_pri_selected);

		}
	#endif
	
	#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI__

		if(g_camera_cntx.selected_shot_mode  == CAMERA_SETTING_SHOT_MODE_APERTURE_PRI)
		{
			S32 i;
			item_icons[0] = IMG_ID_CAMERA_SHOT_MODE_APERTURE_PRI;
			/* image size inline selection */
			for(i = 0 ; i < CAMERA_SETTING_SHOT_MODE_APERTURE_PRI_COUNT; i++)
			{
				g_camera_menu_cntx.camera_setting_inline.shot_mode_aperture_pri_str[i] 
					= (UI_string_type)GetString(g_camera_cntx.shot_mode_aperture_pri_str_id[i]);
			}
	
			SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_CAMERA_SHOT_MODE_APERTURE_PRI)));
		
			SetInlineItemActivation(&wgui_inline_items[item_offset], 
											INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
											0 );
	
			SetInlineItemSelect(	&wgui_inline_items[item_offset++],
										CAMERA_SETTING_SHOT_MODE_APERTURE_PRI_COUNT,
										(U8**)g_camera_menu_cntx.camera_setting_inline.shot_mode_aperture_pri_str,
										&g_camera_menu_cntx.camera_setting_inline.shot_mode_aperture_pri_selected);

		}
	#endif
	
	/* enter category */
	guiBuffer	= GetCurrGuiBuffer( SCR_ID_CAMERA_SHOT_MODE_SETTING );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_CAMERA_SHOT_MODE_SETTING, &inputBufferSize );
	
	if(inputBuffer != NULL)		
		SetCategory57Data(wgui_inline_items, CAMERA_SHOT_MODE_SETTING_ITEM_COUNT*2, inputBuffer);	


	RegisterHighlightHandler( mmi_camera_shot_mode_setting_highlight_hdlr );

	/* show category */
	ShowCategory57Screen(STR_ID_CAMERA_SHOT_MODE, MAIN_MENU_TITLE_MULTIMEDIA_ICON,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								wgui_inline_items,
								0, guiBuffer);

	/* set key hdlr */
	SetCategory57LeftSoftkeyFunction( mmi_camera_shot_mode_setting_set_value );
	DisableCategory57ScreenDone();
	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );

}

/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_shot_mode_screen()
* DESCRIPTION
*   entry effect setting screen
* PARAMETERS
*	void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_shot_mode_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8			*guiBuffer;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_CAMERA_SHOT_MODE, NULL, mmi_camera_entry_shot_mode_screen, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CAMERA_SHOT_MODE);

	nItems		= GetNumOfChild(MENU_ID_CAMERA_SHOT_MODE);
	GetSequenceStringIds(MENU_ID_CAMERA_SHOT_MODE,ItemList);
	GetSequenceImageIds(MENU_ID_CAMERA_SHOT_MODE,ItemIcons);
	SetParentHandler(MENU_ID_CAMERA_SHOT_MODE);

	RegisterHighlightHandler(mmi_camera_shot_mode_highlight_hdlr);

	ShowCategory11Screen(	STR_ID_CAMERA_SHOT_MODE, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									nItems, 
									ItemList,
									g_camera_cntx.setting.shot_mode,				/* current index */
									guiBuffer);

	SetLeftSoftkeyFunction(mmi_camera_set_shot_mode_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}





/***************************************************************************** 
*
* Add Frame
*
*****************************************************************************/

#ifdef __CAMERA_FEATURE_ADD_FRAME__
/*****************************************************************************
* FUNCTION
*	 mmi_camera_hint_add_frame()
* DESCRIPTION
*   add frame hint handler
* PARAMETERS
*	 index 	IN 	selected item's index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_hint_add_frame(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	memset( (PS8)hintData[(index)], 0, sizeof(hintData[(index)]));
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_CAMERA_ADD_FRAME_START+g_camera_cntx.setting.add_frame+1) ));
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_add_frame()
* DESCRIPTION
*   add frame highlight handler
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_add_frame(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_entry_add_frame_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_entry_add_frame_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_set_add_frame_value()
* DESCRIPTION
*   set add frame setting
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_set_add_frame_value(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* capture size not full screen */
	if(!g_camera_cntx.is_fullscreen)
	{
		if(g_camera_menu_cntx.cur_select_add_frame_idx != 0) 
		{
			#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
			DisplayPopupRotated((PU8)GetString(STR_ID_CAMERA_NOTIFY_CHANGE_SIZE_FOR_ADD_FRAME), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE,g_camera_cntx.osd_rotate);
			#else
			DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_CHANGE_SIZE_FOR_ADD_FRAME), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			#endif
			DeleteUptoScrID(SCR_ID_CAMERA_OPTION);
		}
		else
		{
			/* when selected item is OFF */
			#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
			DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
			#else
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
			#endif
			DeleteUptoScrID(SCR_ID_CAMERA_OPTION);
		}
	}
	else
	{
		#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
		DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
		#else
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
		#endif
		DeleteUptoScrID(SCR_ID_CAMERA_OPTION);
	}

	g_camera_cntx.setting.add_frame 	= g_camera_menu_cntx.cur_select_add_frame_idx;
	
if(g_camera_cntx.setting.add_frame == 0 && g_camera_cntx.is_tileshot==FALSE && g_camera_cntx.is_fullscreen == FALSE)
		g_camera_cntx.capture_path = CAPTURE_FROM_MEMORY;
	else 
		g_camera_cntx.capture_path = CAPTURE_FROM_LAYER;

	mmi_camera_store_setting();	
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_add_frame_goto_previous_frame()
* DESCRIPTION
*   go to previous frame
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_add_frame_goto_previous_frame(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_camera_menu_cntx.cur_select_add_frame_idx == 1)
		g_camera_menu_cntx.cur_select_add_frame_idx = CAMERA_SETTING_ADD_FRAME_COUNT-1;
	else 
		g_camera_menu_cntx.cur_select_add_frame_idx--;

	mmi_camera_entry_add_frame_view_screen();
	DeleteNHistory(1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_add_frame_goto_next_frame()
* DESCRIPTION
*   go to next frame
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_add_frame_goto_next_frame(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_camera_menu_cntx.cur_select_add_frame_idx >= CAMERA_SETTING_ADD_FRAME_COUNT-1)
		g_camera_menu_cntx.cur_select_add_frame_idx = 1;
	else
		g_camera_menu_cntx.cur_select_add_frame_idx++;

	mmi_camera_entry_add_frame_view_screen();
	DeleteNHistory(1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_add_frame_view_screen()
* DESCRIPTION
*   entry add frame viewer
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifndef CHIP_HAS_GOUDA
extern U16			simulator_Merge_buffer[GDI_LCD_WIDTH * GDI_LCD_HEIGHT];
#endif
void mmi_camera_entry_add_frame_view_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 img_id = 0 ;
   U16 str_id = 0;
   S32 offset_x,offset_y;
   U32  width,height,img_width,img_height;
#if (CHIP_HAS_GOUDA != 1)
   U16 i,j;
#endif
   U8 *pbuffptr = NULL;
   float datex, datey;
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_entry_add_frame_view_screen()");
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	EntryNewScreen(SCR_ID_CAMERA_ADD_FRAME_VIEW, NULL, mmi_camera_entry_add_frame_view_screen, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif
	
	gdi_layer_lock_frame_buffer();

	if(g_camera_cntx.is_tileshot)
	{
		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
			/* curretly use same id, can split in the future */
			str_id = (U16)(STR_ID_CAMERA_ADD_FRAME_OFF+g_camera_menu_cntx.cur_select_add_frame_idx);
			img_id = (U16)(IMG_ID_CAMERA_TILE_ADD_FRAME_START+g_camera_menu_cntx.cur_select_add_frame_idx);	
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */
	}
	else
	{
		str_id = (U16)(STR_ID_CAMERA_ADD_FRAME_OFF+g_camera_menu_cntx.cur_select_add_frame_idx);
		img_id = (U16)(IMG_ID_CAMERA_ADD_FRAME_START+g_camera_menu_cntx.cur_select_add_frame_idx);
	}
	
	/*
	ShowCategory221Screen(	str_id, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_ID_CAMERA_ADD_FRAME_SET, IMG_GLOBAL_OK, 
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									GDI_COLOR_WHITE, 
									NULL);
*/
	#ifndef __MMI_TOUCH_SCREEN__
		draw_arrow_indication(0,0,1,1);
	#endif /* __MMI_TOUCH_SCREEN__ */

//	gdi_layer_set_clip(0, MMI_title_y+MMI_title_height, UI_device_width-1, MMI_title_y+MMI_title_height+MMI_content_height-1);
	{
		//S32 offset_x,offset_y,width,height,img_width,img_height;
		#ifdef __MMI_MAINLCD_220X176__
		offset_x = 0;
		offset_y = 0;
		#else
		offset_x = MMI_title_x;
		offset_y = MMI_title_y+MMI_title_height;
		#endif
		
		gdi_image_get_dimension_id(img_id, &img_width, &img_height);
		mmi_trace(g_sw_CAMERA, "img_id = %d, img_width = %d, img_height = %d", img_id, img_width, img_height);
		mmi_trace(g_sw_CAMERA, "MMI_content_width = %d, MMI_content_height = %d", MMI_content_width, MMI_content_height);

		width = img_width; height = img_height;
		if(img_width > img_height)
		{
			if(img_width > MMI_content_width)
			{
#ifdef _MMI_OPERATOR_
				CALOPERATOR_STRUCT oper1, oper2, tmp;
				U8 s1[32], s2[32], res[32];

				memset(s1, 0x00, sizeof(s1));
				memset(s2, 0x00, sizeof(s2));
				sprintf(s1, "%d", img_width);
				sprintf(s2, "%d", MMI_content_width);

				StrToRate(&oper1, s1);
				StrToRate(&oper2, s2);
				CalculatorDivi(&oper1, &oper2); /* img_width/MMI_content_width */

				tmp = oper1;

				sprintf(s1, "%d", img_height);
				StrToRate(&oper1, s1);
				CalculatorDivi(&oper1, &tmp);   /* img_height /	tmp */

				memset(res, 0x00, sizeof(res));
				RateToStr(res, &oper1);
				img_height = atoi(res);
#else
				float tmp = (float)img_width/MMI_content_width;
				img_height /= 	tmp;
#endif
				img_width = MMI_content_width;
			}
			else
				offset_x +=  ((MMI_content_width-img_width)>>1);

        		#ifndef __MMI_MAINLCD_220X176__
			offset_y += ((MMI_content_height-img_height)>>1);
			#endif
		}
		else
		{
			if(img_height > MMI_content_height)
			{
#ifdef _MMI_OPERATOR_
				CALOPERATOR_STRUCT oper1, oper2, tmp;
				U8 s1[32], s2[32], res[32];

				memset(s1, 0x00, sizeof(s1));
				memset(s2, 0x00, sizeof(s2));
				sprintf(s1, "%d", img_height);
				sprintf(s2, "%d", MMI_content_height);

				StrToRate(&oper1, s1);
				StrToRate(&oper2, s2);
				CalculatorDivi(&oper1, &oper2); /* img_height/MMI_content_height */

				{
					U8 s[32];
					memset(s, 0x00, sizeof(s));
				    RateToStr(s, &oper1);
					mmi_trace(g_sw_CAMERA, "img_height = %d, MMI_content_height = %d", img_height, MMI_content_height);
					mmi_trace(g_sw_CAMERA, "result = %s", s);
				}

				tmp = oper1;

				sprintf(s1, "%d", img_width);
				StrToRate(&oper1, s1);
				CalculatorDivi(&oper1, &tmp);   /* img_width /	tmp */

				memset(res, 0x00, sizeof(res));
				RateToStr(res, &oper1);
				img_width = atoi(res);
				mmi_trace(g_sw_CAMERA, "res = %s", res);
				mmi_trace(g_sw_CAMERA, "img_width = %d", img_width);
#else
				float tmp = (float)img_height/MMI_content_height;
				img_width /= 	tmp;
#endif
				img_height = MMI_content_height;
			}
			else
				offset_y += ((MMI_content_height-img_height)>>1);

			offset_x += ((MMI_content_width-img_width)>>1);
		}
		mmi_trace(g_sw_CAMERA, "offset_x = %d", offset_x);
		mmi_trace(g_sw_CAMERA, "offset_y = %d", offset_y);
		mmi_trace(g_sw_CAMERA, "img_width = %d", img_width);
		mmi_trace(g_sw_CAMERA, "img_height = %d", img_height);
		mmi_trace(g_sw_CAMERA, "img_id = %d", img_id);
//			gdi_image_draw_resized_id(offset_x,offset_y,img_width,img_height,img_id);
		gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_WHITE);
		gdi_image_draw_id(0, 0, (U16)(img_id));


		gdi_layer_get_buffer_ptr((U8 **)&pbuffptr);
		datex = (float)width/img_width;
		datey = (float)height/img_height;
#ifndef CHIP_HAS_GOUDA

		memset(simulator_Merge_buffer, 0xff, LCD_WIDTH*LCD_HEIGHT*2);

		for(i=0; i<img_height; i++)
		{
			for(j=0; j<img_width; j++)
			{
				 simulator_Merge_buffer[(i+offset_y)*LCD_WIDTH+ j+offset_x]= ((UINT16*)pbuffptr)[(int)(i*datey)*LCD_WIDTH+ (int)(j*datex)];//[(i+offset_y)*128+ j+offset_x];
			}
			
		}	
	#ifdef __MMI_MAINLCD_220X176__
	ShowCategory221Screen(	0, 0, 
								STR_ID_CAMERA_ADD_FRAME_SET, IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
								GDI_COLOR_WHITE, 
								NULL);
	#else
		ShowCategory221Screen(	str_id, 
								MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
								STR_ID_CAMERA_ADD_FRAME_SET, IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
								GDI_COLOR_WHITE, 
								NULL);
	#endif
#ifdef __MMI_MAINLCD_220X176__
		for(i=0; i<img_height-MMI_button_bar_height; i++)
		{
			for(j=0; j<img_width; j++)
			{
				 ((UINT16*)pbuffptr)[(i+offset_y)*LCD_WIDTH+ j+offset_x]= simulator_Merge_buffer[(i+offset_y)*LCD_WIDTH+ j+offset_x];
			}
			
		}	
#else
		for(i=0; i<img_height; i++)
		{
			for(j=0; j<img_width; j++)
			{
				 ((UINT16*)pbuffptr)[(i+offset_y)*LCD_WIDTH+ j+offset_x]= simulator_Merge_buffer[(i+offset_y)*LCD_WIDTH+ j+offset_x];
			}
			
		}	
#endif
#else
			ShowCategory221Screen(	str_id, 
								MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
								STR_ID_CAMERA_ADD_FRAME_SET, IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
								GDI_COLOR_WHITE, 
								NULL);
#endif
	}

	gdi_layer_unlock_frame_buffer();
#if 1
	gdi_layer_blt_base_layer(0,0, UI_device_width-1, UI_device_height-1);
#else
    gdi_layer_blt_base_layer(0,0,128,20);
    gdi_layer_blt_base_layer(0,140,128,160);
#endif

	//gdi_image_draw_resized_id(offset_x,offset_y,img_width,img_height,img_id);

#if 0
    gdi_layer_blt_base_layer(0,0,128,20);
    gdi_layer_blt_base_layer(0,140,128,160);
#endif
	SetLeftSoftkeyFunction(mmi_camera_set_add_frame_value,KEY_EVENT_UP);									
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_add_frame_goto_previous_frame, KEY_UP_ARROW, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_add_frame_goto_next_frame, KEY_DOWN_ARROW, KEY_EVENT_DOWN);		
	/*gaosh Add Start For 6127 Ver: TBM780_T_20070807_2018  on 2007-8-8 10:36 */
	SetKeyHandler(mmi_camera_add_frame_goto_previous_frame, KEY_VOL_UP, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_add_frame_goto_next_frame, KEY_VOL_DOWN, KEY_EVENT_DOWN);		
	/*gaosh Add End  For 6127 Ver: TBM780_T_20070807_2018  on 2007-8-8 10:36 */
	
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_entry_add_frame_view_screen() OVER");
}

/*****************************************************************************
* FUNCTION
*	 mmi_camera_add_frame_highlight_hdlr()
* DESCRIPTION
*   add frame highlight handler
* PARAMETERS
*	 idx 	IN		highlight index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_add_frame_highlight_hdlr(S32 idx)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camera_menu_cntx.cur_select_add_frame_idx = idx;
	
	if(idx == 0) /* off */
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_camera_set_add_frame_value, KEY_EVENT_UP);
	}
	else /* frame */
	{
		ChangeLeftSoftkey(STR_GLOBAL_VIEW, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_camera_entry_add_frame_view_screen, KEY_EVENT_UP);
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_add_frame_screen()
* DESCRIPTION
*   entry add frame screen
* PARAMETERS
*	 void 
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_add_frame_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8			*guiBuffer;
	U16		selected_item;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_CAMERA_ADD_FRAME, NULL, mmi_camera_entry_add_frame_screen, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif
	
	guiBuffer 	= GetCurrGuiBuffer(SCR_ID_CAMERA_ADD_FRAME);
	nItems		= GetNumOfChild(MENU_ID_CAMERA_ADD_FRAME);
	mmi_trace(1,"MENU_ID_CAMERA_ADD_FRAME_OFF is %d,SCR_ID_CAMERA_ADD_FRAME is %d",MENU_ID_CAMERA_ADD_FRAME_OFF,SCR_ID_CAMERA_ADD_FRAME);
	GetSequenceStringIds(MENU_ID_CAMERA_ADD_FRAME,ItemList);
	GetSequenceImageIds(MENU_ID_CAMERA_ADD_FRAME,ItemIcons);
	SetParentHandler(MENU_ID_CAMERA_ADD_FRAME);

	RegisterHighlightHandler(mmi_camera_add_frame_highlight_hdlr);

	/* !NULL measn back from view screen or interrupt */
	if(guiBuffer != NULL)
	{
		selected_item = g_camera_menu_cntx.cur_select_add_frame_idx;
	}
	else
	{
		selected_item = g_camera_cntx.setting.add_frame;
	}

	ShowCategory11Screen(	STR_ID_CAMERA_ADD_FRAME, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									nItems, 
									ItemList,
									selected_item,						/* current index */
									NULL);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

#endif /* __CAMERA_FEATURE_ADD_FRAME__ */

/* wufasong added 2007.04.24 for MCL Layer getting frame image data from MMI. */
#ifdef __CAMERA_FEATURE_ADD_FRAME__
BOOL mmi_camera_get_frame_setting_flag(void)
{
	if(g_camera_cntx.setting.add_frame == 0) /* means no capture frame */
	{
		return FALSE;
	}

	return TRUE;
}

U8* mmi_camera_get_frame_image_data(void)
{
	U16 img_id = 0;
	//U16 str_id;
	//U8 image_identifier;
	U8 *image_ptr = NULL;
	
	if(g_camera_cntx.setting.add_frame == 0) /* means no capture frame */
	{
		return NULL;
	}
	
	if(g_camera_cntx.is_tileshot)
	{
		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
			/* curretly use same id, can split in the future */
			//str_id = (U16)(STR_ID_CAMERA_ADD_FRAME_OFF+g_camera_menu_cntx.cur_select_add_frame_idx);
			img_id = (U16)(IMG_ID_CAMERA_TILE_ADD_FRAME_START+g_camera_menu_cntx.cur_select_add_frame_idx);	
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */
	}
	else
	{
		//str_id = (U16)(STR_ID_CAMERA_ADD_FRAME_OFF+g_camera_menu_cntx.cur_select_add_frame_idx);
		img_id = (U16)(IMG_ID_CAMERA_ADD_FRAME_START+g_camera_menu_cntx.cur_select_add_frame_idx);
	}

	image_ptr = (U8*)GetImage(img_id);
	if (image_ptr != NULL)
	{
		U8 image_identifier;
		image_identifier	= image_ptr[0];

		ASSERT(image_identifier == GDI_IMAGE_TYPE_JPG); /* 目前只支持JPG格式的相框数据 */
		
		if (image_identifier == GDI_IMAGE_TYPE_GIF || image_identifier == GDI_IMAGE_TYPE_PNG)
		{
			image_ptr += 8;
		}
		else
		{
			image_ptr += 6;
		}
	}

	return image_ptr;
}

S32 mmi_camera_get_frame_image_data_size(void)
{
	U16 img_id = 0;
	U8 *image_ptr = NULL;
	s32 image_length = -1;
	
	if(g_camera_cntx.setting.add_frame == 0) /* means no capture frame */
	{
		return -1;
	}
	
	if(g_camera_cntx.is_tileshot)
	{
		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
			/* curretly use same id, can split in the future */
			//str_id = (U16)(STR_ID_CAMERA_ADD_FRAME_OFF+g_camera_menu_cntx.cur_select_add_frame_idx);
			img_id = (U16)(IMG_ID_CAMERA_TILE_ADD_FRAME_START+g_camera_menu_cntx.cur_select_add_frame_idx);	
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */
	}
	else
	{
		//str_id = (U16)(STR_ID_CAMERA_ADD_FRAME_OFF+g_camera_menu_cntx.cur_select_add_frame_idx);
		img_id = (U16)(IMG_ID_CAMERA_ADD_FRAME_START+g_camera_menu_cntx.cur_select_add_frame_idx);
	}

	image_ptr = (U8*)GetImage(img_id);
	if (image_ptr != NULL)
	{
		image_length = gdi_image_read_length(image_ptr);
	}

	return image_length;
}
#else
BOOL mmi_camera_get_frame_setting_flag(void)
{
	return FALSE;
}

U8* mmi_camera_get_frame_image_data(void)
{
	return NULL;
}

S32 mmi_camera_get_frame_image_data_size(void)
{
	return -1;
}
#endif





/***************************************************************************** 
*
* Camera Setting
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_camera_setting()
* DESCRIPTION
*   highligh camera setting on option menu
* PARAMETERS
*	 idx 
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_camera_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_entry_camera_setting_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_entry_camera_setting_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}

#define TRACE_VALUE(x) mmi_trace(g_sw_CAMERA, #x " = %d", x)
#define TRACE_VALUE_HEX(x) mmi_trace(g_sw_CAMERA, #x " = 0x%08x", x)


/*****************************************************************************
* FUNCTION
*	 mmi_camera_camera_setting_set_value()
* DESCRIPTION
*   set camera setting values
* PARAMETERS
*	 idx 
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_camera_setting_set_value(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camera_cntx.setting.wb				= g_camera_menu_cntx.camera_setting_inline.wb_selected;
	g_camera_cntx.setting.flash			= g_camera_menu_cntx.camera_setting_inline.flash_selected;
	g_camera_cntx.setting.led_highlight	= g_camera_menu_cntx.camera_setting_inline.led_highlight_selected;	
	g_camera_cntx.setting.shutter_sound = g_camera_menu_cntx.camera_setting_inline.shutter_sound_selected;	
	g_camera_cntx.setting.ev				= g_camera_menu_cntx.camera_setting_inline.ev_selected;
	g_camera_cntx.setting.reserve5 = g_camera_menu_cntx.camera_setting_inline.bright_selected;
#if defined(__MMI_MAINLCD_220X176__)
	g_camera_cntx.setting.contrast = g_camera_menu_cntx.camera_setting_inline.contrast_selected;
#else
	g_camera_cntx.setting.reserve6 = g_camera_menu_cntx.camera_setting_inline.contrast_selected;
#endif
       mmi_trace(1,"mmi_camera_camera_setting_set_value,g_camera_cntx.setting.ev is %d",g_camera_cntx.setting.ev	);
/*
	if (g_camera_cntx.setting.ev > MDI_CAMERA_EV_P2)
	{
		g_camera_cntx.setting.ev = MDI_CAMERA_EV_P2;
	}
	else if (g_camera_cntx.setting.ev < MDI_CAMERA_EV_N2)
	{
		g_camera_cntx.setting.ev = MDI_CAMERA_EV_N2;
	}
	*/
	g_camera_cntx.setting.night			= g_camera_menu_cntx.camera_setting_inline.night_selected;
	g_camera_cntx.setting.ae_meter		= g_camera_menu_cntx.camera_setting_inline.ae_meter_selected;
	g_camera_cntx.setting.closeup			= g_camera_menu_cntx.camera_setting_inline.closeup_selected;
	g_camera_cntx.setting.banding			= g_camera_menu_cntx.camera_setting_inline.banding_selected;
	g_camera_cntx.setting.iso				= g_camera_menu_cntx.camera_setting_inline.iso_selected;
	mmi_trace(1,"mmi_camera_camera_setting_set_value,g_camera_cntx.setting.night is %d",g_camera_cntx.setting.night	);

	mmi_camera_store_setting();

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	#else
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteNHistory(1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_camera_setting_highlight_hdlr()
* DESCRIPTION
*   highlight handler for camera setting menu
* PARAMETERS
*	 index 	IN		highlighted item index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_camera_setting_highlight_hdlr(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(mmi_camera_camera_setting_set_value);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_camera_setting_screen()
* DESCRIPTION
*   exit camera setting screen
* PARAMETERS
*	 void 
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_exit_camera_setting_screen(void)
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
	h.scrnID			= SCR_ID_CAMERA_CAMERA_SETTING;
	h.entryFuncPtr = mmi_camera_entry_camera_setting_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	inputBufferSize = (U16)GetCategory57DataSize();     
	GetCategory57History (h.guiBuffer);	
	GetCategory57Data ((U8*)h.inputBuffer);       
	AddNHistory(h, inputBufferSize);          
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_camera_setting_screen()
* DESCRIPTION
*   entry camera setting screen
* PARAMETERS
*	 void 
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
/*Jinzh Add Start For 插着耳机快门音从扬声器听到 Ver: TBM780  on 2007-8-27 15:48 */
#ifdef __FF_AUDIO_SET__
extern BOOL	cameraAppOnOff;
#endif
/*Jinzh Add End  For 插着耳机快门音从扬声器听到 Ver: TBM780  on 2007-8-27 15:48 */
/*weid add for presound*/
void mmi_camera_setting_preplay_sound(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_SILENT_MEETING_PROFILE__
	if( IsSilentModeActivated() || IsMeetingModeActivated() )
		return;
#else
	if( IsSilentModeActivated() )
		return;
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
   
   /* force to close multi channel sound */
	#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)
		mmi_camera_burstshot_close_multi_sound();
		coolsand_UI_cancel_timer(mmi_camera_burstshot_close_multi_sound);
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ && __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */

	mmi_trace(g_sw_CAMERA, "presound_sound = %d",g_camera_menu_cntx.camera_setting_inline.shutter_sound_selected);
	/* play capture sound */
	switch(g_camera_menu_cntx.camera_setting_inline.shutter_sound_selected)
	{
#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_1__	
		case CAMERA_SETTING_SHUTTER_SOUND_1:
		{
			/*Jinzh Modify Start For Camera shutter review Ver: TBM780  on 2007-8-13 9:22 */
#ifdef __FF_AUDIO_SET__
			cameraAppOnOff = TRUE;
#endif
			AudioPlayReq(CAMERA_CNT_SOUND1,DEVICE_AUDIO_PLAY_ONCE);
			/*Jinzh Modify End  For Camera shutter review Ver: TBM780  on 2007-8-13 9:22 */
#if 0			
			mdi_audio_play_string_with_vol_path((void*)camera_sound_shutter_1,
													(U32)CAMERA_SOUND_SHUTTER_1_LEN,
												#if defined(MT6228) || defined(MT6229)
													MDI_FORMAT_WAV,
												#else
													MDI_FORMAT_DVI_ADPCM,
												#endif
													DEVICE_AUDIO_PLAY_ONCE,
													NULL,
													NULL,
													CAMERA_SHUTTER_SOUND_VOLUMN,
													MDI_DEVICE_SPEAKER_BOTH);
#endif												
		}
		break;
#endif
#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_2__	

		case CAMERA_SETTING_SHUTTER_SOUND_2:
		{

			/*Jinzh Modify Start For Camera shutter review Ver: TBM780  on 2007-8-13 9:22 */
			//AudioPlayReq(SOUND_2,DEVICE_AUDIO_PLAY_ONCE);
#ifdef __FF_AUDIO_SET__
			cameraAppOnOff = TRUE;
#endif
			AudioPlayReq(CAMERA_CNT_SOUND2,DEVICE_AUDIO_PLAY_ONCE);
			/*Jinzh Modify End  For Camera shutter review Ver: TBM780  on 2007-8-13 9:22 */
#if 0			
			mdi_audio_play_string_with_vol_path((void*)camera_sound_shutter_2,
													(U32)CAMERA_SOUND_SHUTTER_2_LEN,
												#if defined(MT6228) || defined(MT6229)
													MDI_FORMAT_WAV,
												#else
													MDI_FORMAT_DVI_ADPCM,
												#endif
													DEVICE_AUDIO_PLAY_ONCE,
													NULL,
													NULL,
													CAMERA_SHUTTER_SOUND_VOLUMN,
													MDI_DEVICE_SPEAKER_BOTH);
#endif												
		}
		break;
#endif
#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_3__	

		case CAMERA_SETTING_SHUTTER_SOUND_3:
		{

			/*Jinzh Modify Start For Camera shutter review Ver: TBM780  on 2007-8-13 9:24 */
			//AudioPlayReq(SOUND_3,DEVICE_AUDIO_PLAY_ONCE);
#ifdef __FF_AUDIO_SET__
			cameraAppOnOff = TRUE;
#endif
			AudioPlayReq(CAMERA_CNT_SOUND3,DEVICE_AUDIO_PLAY_ONCE);
			/*Jinzh Modify End  For Camera shutter review Ver: TBM780  on 2007-8-13 9:24 */
#if 0			
			mdi_audio_play_string_with_vol_path((void*)camera_sound_shutter_3,
													(U32)CAMERA_SOUND_SHUTTER_3_LEN,
												#if defined(MT6228) || defined(MT6229)
													MDI_FORMAT_WAV,
												#else
													MDI_FORMAT_DVI_ADPCM,
												#endif
													DEVICE_AUDIO_PLAY_ONCE,
													NULL,
													NULL,
													CAMERA_SHUTTER_SOUND_VOLUMN,
													MDI_DEVICE_SPEAKER_BOTH);	
#endif												
		}
		break;
#endif		
		default:
			/* do not play sound */
			break;
	}
}

static U8 g_shutter_sound_selected = 0;
void mmi_camera_setting_shutter_sound_hdlr(S32 index)
{
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_setting_shutter_sound_hdlr() selected = %d,index = %d",g_shutter_sound_selected,index);
//	if(g_shutter_sound_selected != (U8)index)
	{
		g_shutter_sound_selected = (U8)index;
		g_camera_menu_cntx.camera_setting_inline.shutter_sound_selected = index;
		mmi_camera_setting_preplay_sound();
	}
}
/*weid add for presound*/

void mmi_camera_entry_camera_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8	 	*guiBuffer;
	U8		*inputBuffer;
	U16 	inputBufferSize;
	S32	item_offset = 0;
	S32	i;


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	UI_image_ID_type item_icons[CAMERA_CAMERA_SETTING_ITEM_COUNT*2] =
	{
		#ifdef __CAMERA_FEATURE_CONTRAST__
			//IMG_ID_CAMERA_WB,   //add by WeiD use WB icon for contrast
			IMG_ID_CAMERA_SETTING_CONTRAST,		//for contrast
			0,
		#endif

		#ifdef __CAMERA_FEATURE_BRIGHTNESS__
			//IMG_ID_CAMERA_LED_HIGHLIGHT,    //add by WeiD  use LED icon for brightness
			IMG_ID_CAMERA_SETTING_BRIGHTNESS,	//for brightness
			0,
		#endif

		#ifdef __CAMERA_FEATURE_WB__
			IMG_ID_CAMERA_WB,
			0,
		#endif 

		#ifdef __CAMERA_FEATURE_FLASH__
			IMG_ID_CAMERA_FLASH,
#if defined(__PROJECT_GALLITE_C01__)
                        0xFFFF,
#else
			0,
#endif
		#endif 

		#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
			IMG_ID_CAMERA_LED_HIGHLIGHT,
			0,
		#endif 
		
		#ifdef __CAMERA_FEATURE_SHUTTER_SOUND__
			//IMG_ID_CAMERA_SHUTTER_SOUND,
			IMG_ID_CAMERA_SETTING_SHUTTER_SOUND,		//for shutter_sound
#if defined(__PROJECT_GALLITE_C01__)
                        0xFFFF,
#else
			0,
#endif
		#endif 

		#ifdef __CAMERA_FEATURE_EV__
			IMG_ID_CAMERA_EV,
			0,
		#endif 

		#ifdef __CAMERA_FEATURE_NIGHT__
			IMG_ID_CAMERA_NIGHT,
			0,
		#endif 
		
		#ifdef __CAMERA_FEATURE_AE_METER__
			IMG_ID_CAMERA_AE_METER,
			0,
		#endif
		

		#ifdef __CAMERA_FEATURE_CLOSEUP__
			IMG_ID_CAMERA_CLOSEUP,
			0,
		#endif 

		#ifdef __CAMERA_FEATURE_BANDING__
			//IMG_ID_CAMERA_BANDING,
			IMG_ID_CAMERA_SETTING_BANDING,	//for banding
			0,
		#endif 

		#ifdef __CAMERA_FEATURE_ISO__
			IMG_ID_CAMERA_ISO,
			0,
		#endif 	
	};

	mmi_trace(g_sw_CAMERA, "CAMERA_CAMERA_SETTING_ITEM_COUNT = %d", CAMERA_CAMERA_SETTING_ITEM_COUNT);
	for (i = 0; i < CAMERA_CAMERA_SETTING_ITEM_COUNT; i++)
	{
		mmi_trace(g_sw_CAMERA, "i = %d, icon_items = %d", i, item_icons[i * 2]);
	}
	EntryNewScreen(SCR_ID_CAMERA_CAMERA_SETTING, mmi_camera_exit_camera_setting_screen, NULL, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif

	InitializeCategory57Screen();

	g_camera_menu_cntx.camera_setting_inline.wb_selected					= g_camera_cntx.setting.wb;
	g_camera_menu_cntx.camera_setting_inline.flash_selected				= g_camera_cntx.setting.flash;
	g_camera_menu_cntx.camera_setting_inline.led_highlight_selected	= g_camera_cntx.setting.led_highlight;	
	g_camera_menu_cntx.camera_setting_inline.shutter_sound_selected	= g_camera_cntx.setting.shutter_sound;
	g_camera_menu_cntx.camera_setting_inline.ev_selected					= g_camera_cntx.setting.ev;
	g_camera_menu_cntx.camera_setting_inline.night_selected				= g_camera_cntx.setting.night;
	g_camera_menu_cntx.camera_setting_inline.ae_meter_selected			= g_camera_cntx.setting.ae_meter;	
	g_camera_menu_cntx.camera_setting_inline.closeup_selected			= g_camera_cntx.setting.closeup;
	g_camera_menu_cntx.camera_setting_inline.banding_selected			= g_camera_cntx.setting.banding;
	g_camera_menu_cntx.camera_setting_inline.iso_selected					= g_camera_cntx.setting.iso;

//add by WeiD
#if defined(__MMI_MAINLCD_220X176__)
	g_camera_menu_cntx.camera_setting_inline.contrast_selected = g_camera_cntx.setting.contrast; 
#else
	g_camera_menu_cntx.camera_setting_inline.contrast_selected = g_camera_cntx.setting.reserve6;  
#endif
	g_camera_menu_cntx.camera_setting_inline.bright_selected = g_camera_cntx.setting.reserve5;


	/******************************************************************
	*	MACRO to create inline selection. 
	******************************************************************/
/*
//	#define CREATE_CAMERA_SETTING_INLINE_SELECT(a, A) \
//	{\
//		for(i = 0 ; i < CAMERA_SETTING_##A##_COUNT; i++)\
//		{\
//			g_camera_menu_cntx.camera_setting_inline.##a##_str[i] = (UI_string_type)GetString((U16)(STR_ID_CAMERA_##A##_START+i+1));\
//		}\
//		SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_CAMERA_##A)));\
//		SetInlineItemActivation(&wgui_inline_items[item_offset], \
//										INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, \
//										0 );\
//		SetInlineItemSelect(	&wgui_inline_items[item_offset++], \
//									CAMERA_SETTING_##A##_COUNT, \
//									(U8**)g_camera_menu_cntx.camera_setting_inline.##a##_str, \
//									&g_camera_menu_cntx.camera_setting_inline.##a##_selected);\
//	}
*/
	#define CREATE_CAMERA_SETTING_INLINE_SELECT(a, A) \
	{\
		for(i = 0 ; i < CAMERA_SETTING_##A##_COUNT; i++)\
		{\
			g_camera_menu_cntx.camera_setting_inline.a##_str[i] = (UI_string_type)GetString((U16)(STR_ID_CAMERA_##A##_START+i+1));\
		}\
		SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_CAMERA_##A)));\
		SetInlineItemActivation(&wgui_inline_items[item_offset], \
										INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, \
										0 );\
		SetInlineItemSelect(	&wgui_inline_items[item_offset++], \
									CAMERA_SETTING_##A##_COUNT, \
									(U8**)g_camera_menu_cntx.camera_setting_inline.a##_str, \
									&g_camera_menu_cntx.camera_setting_inline.a##_selected);\
	}



	/****************************************************************************
	*  Create inline selection                                                               
	*****************************************************************************/
#if defined(__CAMERA_FEATURE_CONTRAST__)
		for(i = 0 ; i < CAMERA_SETTING_CT_COUNT; i++)
		{
			g_camera_menu_cntx.camera_setting_inline.contrast_str[i] = (UI_string_type)GetString((U16)(STR_ID_CAMERA_CT_START+i)); //need to change str resource brightness
		}
		SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_CAMERA_CONTRAST)));   //
		SetInlineItemActivation(&wgui_inline_items[item_offset], 
										INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 
										0 );
		SetInlineItemSelect(	&wgui_inline_items[item_offset++], 
									CAMERA_SETTING_CT_COUNT, 
									(U8**)g_camera_menu_cntx.camera_setting_inline.contrast_str, 
									&g_camera_menu_cntx.camera_setting_inline.contrast_selected);
#endif
#if defined(__CAMERA_FEATURE_BRIGHTNESS__)
	   for(i = 0 ; i < CAMERA_SETTING_BT_COUNT; i++)
		{
			g_camera_menu_cntx.camera_setting_inline.bright_str[i] = (UI_string_type)GetString((U16)(STR_ID_CAMERA_BT_START+i)); //need to change str resource contrast
		}
		SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_CAMERA_BRIGHTNESS)));
		SetInlineItemActivation(&wgui_inline_items[item_offset], 
										INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 
										0 );
		SetInlineItemSelect(	&wgui_inline_items[item_offset++], 
									CAMERA_SETTING_BT_COUNT, 
									(U8**)g_camera_menu_cntx.camera_setting_inline.bright_str, 
									&g_camera_menu_cntx.camera_setting_inline.bright_selected);
#endif

//	CREATE_CAMERA_SETTING_INLINE_SELECT(bn,BT);	 //add by WeiD
//	CREATE_CAMERA_SETTING_INLINE_SELECT(ct,CT);
#ifdef __CAMERA_FEATURE_WB__
	//CREATE_CAMERA_SETTING_INLINE_SELECT(wb, WB);
	//chenhe
		for(i = 0 ; i < CAMERA_SETTING_WB_COUNT; i++)
		{
			g_camera_menu_cntx.camera_setting_inline.wb_str[i] 
				= (UI_string_type)GetString((U16)(STR_ID_CAMERA_WB_AUTO+i)); 
		}
		SetInlineItemCaption(	&wgui_inline_items[item_offset++],
							(PU8)GetString((U16)(STR_ID_CAMERA_WB)));   
		SetInlineItemActivation(	&wgui_inline_items[item_offset], 
							INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 
							0 );
		SetInlineItemSelect(		&wgui_inline_items[item_offset++], 
							CAMERA_SETTING_WB_COUNT, 
							(U8**)g_camera_menu_cntx.camera_setting_inline.wb_str, 
							&g_camera_menu_cntx.camera_setting_inline.wb_selected);	
	
		
#endif 

#ifdef __CAMERA_FEATURE_FLASH__
	CREATE_CAMERA_SETTING_INLINE_SELECT(flash, FLASH);
#endif 

#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
	CREATE_CAMERA_SETTING_INLINE_SELECT(led_highlight, LED_HIGHLIGHT);
#endif 

//	CREATE_CAMERA_SETTING_INLINE_SELECT(brightness,BT);
//    CREATE_CAMERA_SETTING_INLINE_SELECT(contrast,CT);

#ifdef __CAMERA_FEATURE_SHUTTER_SOUND__
//	CREATE_CAMERA_SETTING_INLINE_SELECT(shutter_sound, SHUTTER_SOUND);
	for(i = 0 ; i < CAMERA_SETTING_SHUTTER_SOUND_COUNT; i++)
		{
			g_camera_menu_cntx.camera_setting_inline.shutter_sound_str[i] 
				= (UI_string_type)GetString((U16)(STR_ID_CAMERA_SHUTTER_SOUND_START+i+1)); 
		}
		SetInlineItemCaption(	&wgui_inline_items[item_offset++],
							(PU8)GetString((U16)(STR_ID_CAMERA_SHUTTER_SOUND)));   
		SetInlineItemActivation(	&wgui_inline_items[item_offset], 
							INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 
							0 );
		SetInlineItemSelect(		&wgui_inline_items[item_offset++], 
							CAMERA_SETTING_SHUTTER_SOUND_COUNT, 
							(U8**)g_camera_menu_cntx.camera_setting_inline.shutter_sound_str, 
							&g_camera_menu_cntx.camera_setting_inline.shutter_sound_selected);
//add by WeiD
		RegisterInlineSelectHighlightHandler(&wgui_inline_items[item_offset - 1], 
			mmi_camera_setting_shutter_sound_hdlr);
		
#endif 

//#ifdef __CAMERA_FEATURE_EV__
#if 0
	//CREATE_CAMERA_SETTING_INLINE_SELECT(ev, EV);
	//chenhe
	for(i = 0 ; i < CAMERA_SETTING_EV_COUNT; i++)
	{
		g_camera_menu_cntx.camera_setting_inline.ev_str[i] 
			= (UI_string_type)GetString((U16)(STR_ID_CAMERA_EV_N2+i)); 
	}
	SetInlineItemCaption(	&wgui_inline_items[item_offset++],
						(PU8)GetString((U16)(STR_ID_CAMERA_EV)));   
	SetInlineItemActivation(	&wgui_inline_items[item_offset], 
						INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 
						0 );
	SetInlineItemSelect(		&wgui_inline_items[item_offset++], 
						CAMERA_SETTING_EV_COUNT, 
						(U8**)g_camera_menu_cntx.camera_setting_inline.ev_str, 
						&g_camera_menu_cntx.camera_setting_inline.ev_selected);
	mmi_trace(1,"mmi_camera_entry_camera_setting_screen,STR_ID_CAMERA_EV_N2 is %d",STR_ID_CAMERA_EV_N2);

#endif	
	
//#endif 

#ifdef __CAMERA_FEATURE_NIGHT__
	//CREATE_CAMERA_SETTING_INLINE_SELECT(night, NIGHT);
	//chenhe
		for(i = 0 ; i < CAMERA_SETTING_NIGHT_COUNT; i++)
		{
			g_camera_menu_cntx.camera_setting_inline.night_str[i] 
				= (UI_string_type)GetString((U16)(STR_ID_CAMERA_NIGHT_OFF+i)); 
		}
		SetInlineItemCaption(	&wgui_inline_items[item_offset++],
							(PU8)GetString((U16)(STR_ID_CAMERA_NIGHT)));   
		SetInlineItemActivation(	&wgui_inline_items[item_offset], 
							INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 
							0 );
		SetInlineItemSelect(		&wgui_inline_items[item_offset++], 
							CAMERA_SETTING_NIGHT_COUNT, 
							(U8**)g_camera_menu_cntx.camera_setting_inline.night_str, 
							&g_camera_menu_cntx.camera_setting_inline.night_selected);
	mmi_trace(1,"mmi_camera_entry_camera_setting_screen,STR_ID_CAMERA_NIGHT_OFF is %d",STR_ID_CAMERA_NIGHT_OFF);
#endif 

#ifdef __CAMERA_FEATURE_AE_METER__
	CREATE_CAMERA_SETTING_INLINE_SELECT(ae_meter, AE_METER);
#endif 

#ifdef __CAMERA_FEATURE_CLOSEUP__
	CREATE_CAMERA_SETTING_INLINE_SELECT(closeup, CLOSEUP);
#endif 
#if 0//chenhe for jasper
#ifdef __CAMERA_FEATURE_BANDING__
	CREATE_CAMERA_SETTING_INLINE_SELECT(banding, BANDING);
#endif 
#endif
#ifdef __CAMERA_FEATURE_ISO__
	CREATE_CAMERA_SETTING_INLINE_SELECT(iso, ISO);
#endif 

	guiBuffer	= GetCurrGuiBuffer( SCR_ID_CAMERA_CAMERA_SETTING );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_CAMERA_CAMERA_SETTING, &inputBufferSize );
	
	if(inputBuffer != NULL)		
		SetCategory57Data(wgui_inline_items, CAMERA_CAMERA_SETTING_ITEM_COUNT*2, inputBuffer);	
	
	RegisterHighlightHandler(mmi_camera_camera_setting_highlight_hdlr);
	
	/* show category */
	ShowCategory57Screen(STR_ID_CAMERA_CAMERA_SETTING, MAIN_MENU_TITLE_MULTIMEDIA_ICON,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								wgui_inline_items,
								0, guiBuffer);

	/* set key hdlr */
	SetCategory57LeftSoftkeyFunction(mmi_camera_camera_setting_set_value);
	DisableCategory57ScreenDone();
	SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
}



/***************************************************************************** 
*
* Image Setting
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_image_setting()
* DESCRIPTION
*   highlight handler on camera's option menu
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_image_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_entry_image_setting_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_entry_image_setting_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}


#ifndef __MMI_DISABLE_DUAL_SENSOR__
void mmi_camera_entry_switch_sensor(void)
{
#ifdef MMI_ON_HARDWARE_P
    extern void camera_SwitchSensor();
    camera_SwitchSensor();
#if defined(__CAMERA_OSD_HORIZONTAL__)
    DisplayPopupRotated(
        (PU8) GetString(STR_GLOBAL_DONE),
        IMG_GLOBAL_ACTIVATED,
        1,
        ST_NOTIFYDURATION,
        SUCCESS_TONE,
        g_camera_cntx.osd_rotate);
#else /* defined(__CAMERA_OSD_HORIZONTAL__) */ 
    DisplayPopup((PU8) GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
#endif /* defined(__CAMERA_OSD_HORIZONTAL__) */ 

  DeleteNHistory(1);
  GoBackHistory();
#endif 
}
void mmi_camera_highlight_switch_sensor(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_camera_entry_switch_sensor, KEY_EVENT_UP);
    SetKeyHandler(mmi_camera_entry_switch_sensor, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}
#endif

/*****************************************************************************
* FUNCTION
*	 mmi_camera_image_setting_set_value()
* DESCRIPTION
*   set image setting value
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_image_setting_set_value(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camera_cntx.setting.image_size		= g_camera_menu_cntx.image_setting_inline.image_size_selected;
	g_camera_cntx.setting.image_qty		= g_camera_menu_cntx.image_setting_inline.image_qty_selected;

	mmi_trace(g_sw_CAMERA, "Excute image_set_value image_size = %ld, image_qty = %ld",g_camera_cntx.setting.image_size,g_camera_cntx.setting.image_qty);
	/* reset zoom to default since image size might changed */
	g_camera_cntx.setting.zoom_idx = CAMERA_DEFAULT_SETTING_ZOOM_IDX;

	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		if(g_camera_cntx.is_tileshot)	
		{
			if(g_camera_cntx.setting.image_size != CAMERA_FEATURE_CONT_SHOT_TILE_IMAGE_IDX)
			{
				g_camera_cntx.is_tileshot 			= FALSE;
				g_camera_cntx.setting.cont_shot 	= CAMERA_SETTING_CONT_SHOT_OFF;
			}
		}
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_BURST)
		{
			if(g_camera_cntx.setting.image_size != CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX)
			{
				g_camera_cntx.setting.cont_shot = CAMERA_SETTING_CONT_SHOT_OFF;
			}
		}
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */
	
	mmi_camera_preview_update_capture_size();

	mmi_camera_store_setting();

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	#else
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteNHistory(1);	
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_image_setting_highlight_hdlr()
* DESCRIPTION
*   image setting screen's highlight handler
* PARAMETERS
*	 index 	IN		selected item's index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_image_setting_highlight_hdlr(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(mmi_camera_image_setting_set_value);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_image_setting_screen()
* DESCRIPTION
*   exit image setting screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_exit_image_setting_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 		inputBufferSize;                
	S16		nHistory = 0;
	history_t 	h;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	CloseCategory57Screen();       
	h.scrnID			= SCR_ID_CAMERA_IMAGE_SETTING;
	h.entryFuncPtr = mmi_camera_entry_image_setting_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	inputBufferSize = (U16)GetCategory57DataSize(); 
	GetCategory57History (h.guiBuffer);
	GetCategory57Data ((U8*)h.inputBuffer);       
	AddNHistory(h, inputBufferSize);          
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_image_setting_screen()
* DESCRIPTION
*   entry image setting screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_image_setting_screen(void)
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
	UI_image_ID_type item_icons[CAMERA_IMAGE_SETTING_ITEM_COUNT*2] =
	{
			IMG_ID_CAMERA_IMAGE_SIZE,
#if defined(__PROJECT_GALLITE_C01__)
			0xFFFF,
#else
			0,
#endif
			IMG_ID_CAMERA_IMAGE_QTY,
#if defined(__PROJECT_GALLITE_C01__)
			0xFFFF,
#else
			0,
#endif
	};

	EntryNewScreen(SCR_ID_CAMERA_IMAGE_SETTING, mmi_camera_exit_image_setting_screen, NULL, NULL);

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);

	#endif

	InitializeCategory57Screen();
	
	g_camera_menu_cntx.image_setting_inline.image_size_selected	= g_camera_cntx.setting.image_size;
	g_camera_menu_cntx.image_setting_inline.image_qty_selected	= g_camera_cntx.setting.image_qty;


	/****************************************************************************
	*  Create inline selection                                                               
	*****************************************************************************/
	/* image size inline selection */
	for(i = 0 ; i < CAMERA_SETTING_IMAGE_SIZE_COUNT; i++)
	{
		g_camera_menu_cntx.image_setting_inline.image_size_str[i] 
			= (UI_string_type)GetString(g_camera_cntx.image_size_str_id[i]);
	}
	
	SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_CAMERA_IMAGE_SIZE)));
		
	SetInlineItemActivation(&wgui_inline_items[item_offset], 
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );
	
	SetInlineItemSelect(	&wgui_inline_items[item_offset++],
								CAMERA_SETTING_IMAGE_SIZE_COUNT,
								(U8**)g_camera_menu_cntx.image_setting_inline.image_size_str,
								&g_camera_menu_cntx.image_setting_inline.image_size_selected);

#if 1//cancle it according to sensor driver by chenhe	
	/* image quality inline selection */
	for(i = 0 ; i < CAMERA_SETTING_IMAGE_QTY_COUNT; i++)
	{
		g_camera_menu_cntx.image_setting_inline.image_qty_str[i] 
			= (UI_string_type)GetString((U16)(STR_ID_CAMERA_IMAGE_QTY_START+i+1 /*chenhe,081021,fix bug 10115,for our only support normal or high*/));
	}
	
	SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString((U16)(STR_ID_CAMERA_IMAGE_QTY)));
		
	SetInlineItemActivation(&wgui_inline_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );
	
	SetInlineItemSelect(	&wgui_inline_items[item_offset++],
								CAMERA_SETTING_IMAGE_QTY_COUNT,
								(U8**)g_camera_menu_cntx.image_setting_inline.image_qty_str,
								&g_camera_menu_cntx.image_setting_inline.image_qty_selected);
#endif
	/* enter category */  
	guiBuffer	= GetCurrGuiBuffer( SCR_ID_CAMERA_IMAGE_SETTING );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_CAMERA_IMAGE_SETTING, &inputBufferSize );
	
	if(inputBuffer != NULL)		
		SetCategory57Data(wgui_inline_items, CAMERA_IMAGE_SETTING_ITEM_COUNT*2, inputBuffer);	


	RegisterHighlightHandler( mmi_camera_image_setting_highlight_hdlr );

	/* show category */
	ShowCategory57Screen(STR_ID_CAMERA_IMAGE_SETTING, MAIN_MENU_TITLE_MULTIMEDIA_ICON,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								wgui_inline_items,
								0, guiBuffer);

	/* set key hdlr */
	SetCategory57LeftSoftkeyFunction( mmi_camera_image_setting_set_value );
	DisableCategory57ScreenDone();
	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );

}


/*****************************************************************************
*
* Restore Default
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_restore_default_confirm_yes()
* DESCRIPTION
*   restore defualt confirm yes
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_restore_default_confirm_yes(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* restore default values */

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
	{
		mmi_camera_restore_setting();
		DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,CAMERA_DISPLAY_ROTATE_270);		
	}
	else
	{
		mmi_camera_restore_setting();
		DisplayPopupRotated((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE,g_camera_cntx.osd_rotate);
	}
	
	#else
	mmi_camera_restore_setting();
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	#endif
	DeleteNHistory(1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_restore_default_confirm_screen()
* DESCRIPTION
*   entry restore default confirm screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_entry_restore_default_confirm_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	DisplayConfirmRotated( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						 STR_GLOBAL_NO, IMG_GLOBAL_NO,
						 get_string(STR_ID_CAMERA_NOTIFY_RESTORE_DEFAULT_CONFIRM), IMG_GLOBAL_QUESTION, WARNING_TONE,g_camera_cntx.osd_rotate); 
	#else
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						 STR_GLOBAL_NO, IMG_GLOBAL_NO,
						 get_string(STR_ID_CAMERA_NOTIFY_RESTORE_DEFAULT_CONFIRM), IMG_GLOBAL_QUESTION, WARNING_TONE); 
	#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_camera_entry_restore_default_confirm_yes,KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_restore_default()
* DESCRIPTION
*   highlight handler for restore default menuitem
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_restore_default(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_entry_restore_default_confirm_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_entry_restore_default_confirm_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}





/*****************************************************************************
*
* Self Capture
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_set_self_capture()
* DESCRIPTION
*   Set self capture
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_set_self_capture(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camera_cntx.is_force_self_capture = TRUE;
	GoBackHistory();
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_highlight_self_capture()
* DESCRIPTION
*   highlight handler of self capture on option menu
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_highlight_self_capture(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	SetLeftSoftkeyFunction(mmi_camera_set_self_capture,KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_set_self_capture, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


#endif /* __MMI_CAMERA__ */


