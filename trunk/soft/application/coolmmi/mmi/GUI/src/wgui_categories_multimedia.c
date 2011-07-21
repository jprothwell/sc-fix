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
 *  wgui_categories_multimedia.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Multimedia related categories.
 *  [Category221]       Empty Screen
 *  [Category222]       View Image Screen
 *  [Category223]       Play Audio Screen
 *  [Category224]       Play Video Full Screen
 *  [Category225]       Play Video Standard Screen (With title & softkey)
 *  [Category331][SUB]  Emptry Sublcd Screen
 *  [Category332][SUB]  Image Icon Sublcd Screen 
 *  [Category333][SUB]  Play Audio Sublcd Screen
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef MMI_ON_HARDWARE_P
#undef __DRM_SUPPORT__
#undef __DRM_V01__
#endif /* MMI_ON_HARDWARE_P */ 

/* cat 222 profiling */
// #define CAT222_DECODE_TIME_TEST

#define __NEWSIMULATOR

#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

/* framework related include */
#include "eventsgprot.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "globaldefs.h"
#include "frameworkstruct.h"
#include "eventsdef.h"
#include "unicodexdcl.h"
#include "debuginitdef.h"
#include "fat_fs.h"
#include "settingprofile.h"     /* for error tone */

#ifdef __DRM_SUPPORT__
#include "fs_errcode.h"
#include "drm_gprot.h"
#endif /* __DRM_SUPPORT__ */ 

/* gui related include */
#include "gdi_include.h"        /* include for graphic lib */
#include "lcd_if.h"
#include "gui_themes.h"
#include "wgui.h"
#include "wgui_softkeys.h"
#include "wgui_fixed_menus.h"
#include "wgui_fixed_menuitems.h"
#include "wgui_dynamic_menus.h"
#include "wgui_dynamic_menuitems.h"
#include "wgui_categories.h"
#include "wgui_categories_sublcd.h"
#include "wgui_categories_multimedia.h"
#include "wgui_categories_inputs.h"     /* multiline_inputbox_height_pad */
#include "wgui_status_icons.h"
#include "commonscreens.h"      /* displaypopup() */
#include "phonesetupgprots.h"   /* phnsetgetdefencodingtype() */
#include "conversions.h"        /* char set conversion */
#include "scrmemmgrgprot.h"     /* screen memeory manager */

#include "wgui_draw_manager.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif 

#include "wallpaperdefs.h"      /* image size error string */
#include "gpioinc.h"    /* backlight on/off */

#include "filemgr.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"

#include "sublcdhistorygprot.h" /* sublcd function */

#ifdef CAT222_DECODE_TIME_TEST
#include "kal_release.h"        /* for kal time api */
#include "kal_nucleus.h"
#endif /* CAT222_DECODE_TIME_TEST */ 

#ifdef __MMI_VIDEO_PLAYER__
#include "med_api.h"    /* media task */
#include "med_main.h"   /* media task */
#include "mdi_datatype.h"
#include "mdi_video.h"  /* video lib */
#include "mdi_audio.h"

#include "lcd_sw_rnd.h"
#include "gdi_include.h"
#include "vdoplyimgenum.h"      /* sublcd video icon */
#endif /* __MMI_VIDEO_PLAYER__ */ 

#ifdef __MMI_TVOUT__
#include "mdi_tv.h"     /* mdi tvout */
#include "wallpaperdefs.h"      /* some header for phonsetup.h */
#include "downloaddefs.h"       /* some header for phonsetup.h */
#include "phonesetup.h" /* tvout display style */
#endif /* __MMI_TVOUT__ */ 

#ifdef __MMI_SWFLASH__
#include "mdi_swflash.h"
#include "mmi_phnset_dispchar.h"
#endif 
#undef __NEWSIMULATOR
#include "keybrd.h"   

// dingjian 20071101 add for JasperII
//#include "mci.h"
#include "ucs2prot.h"
/****************************************************************************
* Define
*****************************************************************************/

/****************************************************************************
* Global Variable
*****************************************************************************/
extern S16 status_icon;
extern S32 wgui_image_clip_x1;
extern S32 wgui_image_clip_x2;
extern S32 wgui_image_clip_y1;
extern S32 wgui_image_clip_y2;
extern U8 wgui_category_screen_no_buttons;
extern BOOL r2lMMIFlag;
extern U16 title_bg_id;
extern gdi_handle wgui_layer_1;
extern gdi_handle wgui_base_layer;
extern S8 *idle_screen_wallpaper_name;

/****************************************************************************
* Global Function
*****************************************************************************/
extern void wgui_set_animation_image_y(S32 x, S32 y, PU8 img);
extern void wgui_show_transparent_animation(void);
extern void set_main_LCD_time_duration_hide_function(void (*f) (S32 x1, S32 y1, S32 x2, S32 y2));       /* 101705 */
#ifdef __FF_AUDIO_SET__
extern void MusicEndAudioMode();
#endif
#ifdef __MMI_AUDIO_PLAYER__
extern int mmi_audply_exit_subLCD(void);
extern int mmi_audply_redraw_subLCD(BOOL);
extern BOOL mmi_audply_is_playing(void);
#endif /* __MMI_AUDIO_PLAYER__ */ 

#ifdef __MMI_FM_RADIO__
extern int mmi_fmrdo_exit_subLCD(void);
extern int mmi_fmrdo_redraw_subLCD(BOOL);
extern BOOL mmi_fmrdo_is_power_on(void);
#endif /* __MMI_FM_RADIO__ */ 

#ifdef __MMI_VIDEO_PLAYER__
mdi_video_info_struct wgui_video_info;
#endif 
#ifdef __MMI_SWFLASH__
extern void PhnsetWallpaperBadFileCallBack(U16 imgID);
extern void dm_category_33_controlled_area(dm_coordinates *coordinate);
extern pBOOL mmi_bootup_get_active_flight_mode(void);
extern void ShowCategory33Screen_ext(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *history_buffer);
extern void ExitCategory33Screen(void);
#endif 

/****************************************************************************
* Local Variable
*****************************************************************************/
/* [Category 221] */
static FuncPtr cat221_redraw_content_callback = NULL;
static gdi_color cat221_background_color;

/* [Category 222] */
static PS8 cat222_file_name_ptr;
static U16 cat222_title_icon;
static U16 cat222_title;
static PS8 cat222_button_str_ptr;   /* will use subMenu's data buffer to store */
static void (*cat222_decode_result_callback) (GDI_RESULT);
static BOOL cat222_is_short_filename;
static GDI_HANDLE cat222_anim_handle;
#ifdef __MMI_TVOUT__
static S32 cat222_img_width;
static S32 cat222_img_height;
static BOOL cat222_is_fullscr_tvout = FALSE;
#endif 

/* [Category 223] */
static BOOL cat223_display_duration = FALSE;

/* [Category Vdoply] */
#ifdef __MMI_VIDEO_PLAYER__
static U16 cat_vdoply_video_repeat_count;
static U8 cat_vdoply_audio_volumn;
static BOOL cat_vdoply_video_is_visual_update;
static BOOL cat_vdoply_video_is_play_audio;
static BOOL cat_vdoply_video_is_lcd_no_sleep;
static BOOL cat_vdoply_is_video_open;
static BOOL cat_vdoply_is_video_play;
static BOOL cat_vdoply_is_video_force_stop; /* force stop flag */
static BOOL cat_vdoply_is_video_from_id;
static GDI_HANDLE cat_vdoply_video_layer = GDI_ERROR_HANDLE;

#ifdef __MMI_SUBLCD__
static BOOL cat_vdoply_is_sublcd_display = FALSE;   /* must init */
#endif 
static void (*cat_vdoply_play_finish_callback) (MDI_RESULT result);
static U8 cat_vdoply_is_full_screen;
#endif /* __MMI_VIDEO_PLAYER__ */ 

/* [Category 229 ] */
#ifdef __MMI_SWFLASH__
extern mdi_swflash_info_struct wgui_swflash_info;

static U16 cat_swflash_repeat_count;
static U8 cat_swflash_audio_volumn;
static MMI_BOOL cat_swflash_is_audio_on;
static MMI_BOOL cat_swflash_is_vibrate_on;
static MMI_BOOL cat_swflash_is_interaction_on;
static MMI_BOOL cat_swflash_is_lcd_no_sleep;
static MMI_BOOL cat_swflash_is_open;
static MMI_BOOL cat_swflash_is_play;
static MMI_BOOL cat_swflash_is_from_id;
static GDI_HANDLE cat_swflash_layer = GDI_ERROR_HANDLE;
static void (*cat_swflash_play_finish_callback) (MDI_RESULT);
static U8 cat_swflash_is_full_screen;
static MDI_RESULT cat_swflash_play_ret;
static S32 g_mmi_idle_bg_media_type = 0;
#endif /* __MMI_SWFLASH__ */ 

/****************************************************************************
* Local Function
*****************************************************************************/
/* [Category 221] */
static void ExitCategory221Screen(void);

/* [Category 222] */
static void ExitCategory222Screen(void);
static void Cat222DecodeImage(void);
static void Cat222DrawTitle(void);

#ifdef __MMI_TVOUT__
static void Cat222ToggleTVDisplay(void);
#endif 

/* [Category 223] */
static void ExitCategory223Screen(void);

static void DrawCate223CategoryControlArea(dm_coordinates *coordinate);

static void Cat223DrawDurationBar(void);
static void Cat223InitDruration(UI_time *t, U32 duration);
static void Cat223HideDurationDisplay(S32 x1, S32 y1, S32 x2, S32 y2);

#ifdef __MMI_VIDEO_PLAYER__
   /* [Category Video Play Screen] */
static void ShowCategoryVdoplyScreen(BOOL is_fullscreen,
                                     U16 title_str,
                                     U16 title_icon,
                                     U16 lsk_str,
                                     U16 lsk_icon,
                                     U16 rsk_str,
                                     U16 rsk_icon,
                                     U16 video_id,
                                     PS8 video_filename,
                                     U16 repeat_count,
                                     BOOL is_visual_update,
                                     BOOL is_play_audio,
                                     BOOL is_lcd_no_sleep,
                                     GDI_COLOR bg_color, void (*play_finish_callback) (MDI_RESULT), void *gui_buffer);
static void ExitCategoryVdoplyScreen(void);
static void StopCategoryVdoplyVideo(void);
static void PauseCategoryVdoplyVideo(void);
static void ResumeCategoryVdoplyVideo(void);
static void CatVdoplyPlayFinishCallback(MDI_RESULT result);

#ifdef __MMI_SWFLASH__
extern void DrawCateSWFlashCategoryControlArea(dm_coordinates *coordinate);
extern void ExitCategory34Screen(void);
#endif

#ifdef __MMI_SUBLCD__
static void EnterCategoryVdoplySubScreen(void);
static void ExitCategoryVdoplySubScreen(void);
#endif /* __MMI_SUBLCD__ */ 
static void CatVdoplyVolUp(void);
static void CatVdoplyVolDown(void);
#endif /* __MMI_VIDEO_PLAYER__ */ 

#ifdef __MMI_SUBLCD__
/* [Category 331] [SUB] */
static void ExitCategory331Screen(void);

/* [Category 331] [SUB] */
static void ExitCategory332Screen(void);

/* [Category 333] [SUB] */
static U8 *GetCategory333History(U8 *history_buffer);
static S32 GetCategory333HistorySize(void);
static void ExitCategory333Screen(void);
static void RedrawCategory333Screen(void);
#endif /* __MMI_SUBLCD__ */ 

void cat226_hide_button_full_screen(void *button_object);
void ExitCategorySWFlashScreen(void);

/****************************************************************************
*
* MultiMedia Related [MainLCD] Categories
*
*****************************************************************************/

/*****************************************************************************
* [Category221]
*
* This category is basic frame with caption and softkeys.
*
*  **********************
*  *       Caption      *
*  **********************
*  *                    *
*  *                    *
*  *                    *
*  *                 <----------- Empty. Will hook call back function to redraw.
*  *                    *
*  *                    *
*  *                    *
*  *********    *********
*  *  LSK  *    *  RSK  *
*  **********************
*
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  DrawCate221CategoryControlArea
 * DESCRIPTION
 *  This function is used to draw the category controlled area of category221 screen
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate221CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_title_string != 0 || MMI_title_icon != 0)
    {
    #if defined(__MMI_SCREEN_ROTATE__)
        if (!mmi_frm_is_screen_width_height_swapped())
        {
            show_title_status_icon();
        }
    #else /* defined(__MMI_SCREEN_ROTATE__) */ 
        show_title_status_icon();
    #endif /* defined(__MMI_SCREEN_ROTATE__) */ 
        draw_title();
    }

    /* fill background */
    gdi_layer_reset_clip();

    /* if has title - draw content region only */
    if (MMI_title_string != 0 || MMI_title_icon != 0)
    {
        gdi_draw_solid_rect(
            0,
            (MMI_title_y + MMI_title_height),
            UI_device_width - 1,
            (MMI_title_y + MMI_title_height) + MMI_content_height - 1,
            cat221_background_color);
    }
    else
    {
        gdi_draw_solid_rect(
            0,
            0,
            UI_device_width - 1,
            (MMI_title_y + MMI_title_height) + MMI_content_height - 1,
            cat221_background_color);
    }

    /* redraw callback function */
    if (cat221_redraw_content_callback != NULL)
    {
        cat221_redraw_content_callback();
    }

}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory221Screen
 * DESCRIPTION
 *  Show category 221 screen.
 * PARAMETERS
 *  title                       [IN]        Title string id
 *  title_icon                  [IN]        Title image id
 *  left_softkey                [IN]        Lsk string id
 *  left_softkey_icon           [IN]        Lsk image id
 *  right_softkey               [IN]        Rsk string id
 *  right_softkey_icon          [IN]        Rsk image id
 *  background_color            [IN]        Background color for content region
 *  redraw_content_callback     [IN]        Callback function to redraw context (PS.This redraw callback pointer is used for framework to update content region when theme update.PS. MMI Framework will not call redraw function anymore. The pointer is useless)
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory221Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        gdi_color background_color,
        FuncPtr redraw_content_callback)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    cat221_background_color = background_color;
    cat221_redraw_content_callback = redraw_content_callback;

    /* hide status if will not draw title bar */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)       
    if (title == 0 && title_icon == 0)
    {
        hide_status_icon_bar(0);
    }
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) */ 

    /* lock */
    gui_lock_double_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    /* init title */
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory221Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_register_category_controlled_callback(DrawCate221CategoryControlArea);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY221_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory221Screen
 * DESCRIPTION
 *  Exit category 221
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void ExitCategory221Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();

#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)       
    show_status_icon_bar(0);
#endif 

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}

/*****************************************************************************
* [Category222]
*
* This category is for view image from file.
*
*  **********************
*  *       Caption      *
*  **********************
*  *                    *
*  *                    *
*  *                    *
*  *       (Image)      *
*  *                    *
*  *                    *
*  *                    *
*  *********    *********
*  *  LSK  *    *  RSK  *
*  **********************
*
*****************************************************************************/

/****************************************************************************
* Two View Mode. Turn on ONLY one style.
* (1) CAT222_IMAGE_FIT_FULLSCREEN_SIZE.
*     Image will fit full screen size. This is useful when view wallpapre that it will not shink.
*
* (2) CAT222_IMAGE_FIT_CONTENT_SIZE
*     Image will fit content region, and will have a photo like display.
*****************************************************************************/
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)        
// #define      CAT222_IMAGE_FIT_FULLSCREEN_SIZE
#define     CAT222_IMAGE_FIT_CONTENT_SIZE
#define     CAT222_IMAGE_SPACING    (5) /* spacing between image and LCM boundry */
#else /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
// #define      CAT222_IMAGE_FIT_FULLSCREEN_SIZE
#define     CAT222_IMAGE_FIT_CONTENT_SIZE
#define     CAT222_IMAGE_SPACING    (4) /* spacing between image and LCM boundry */
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 

void DrawCate222CategoryControlArea(dm_coordinates *coordinate);


/*****************************************************************************
 * FUNCTION
 *  cat222_get_image_handle
 * DESCRIPTION
 *  get the animation handle for cat 222
 * PARAMETERS
 *  void
 * RETURNS
 *  gdi_handle
 *****************************************************************************/
gdi_handle cat222_get_image_handle(void)//070706
{
    return (gdi_handle)cat222_anim_handle;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory222Screen
 * DESCRIPTION
 *  Show Category 222 Screen.
 * PARAMETERS
 *  title                       [IN]        )
 *  title_icon                  [IN]        Title image id
 *  left_softkey                [IN]        Lsk string id
 *  left_softkey_icon           [IN]        Lsk image id
 *  right_softkey               [IN]        Rsk string id
 *  right_softkey_icon          [IN]        Rsk image id
 *  background_color            [IN]        Background color for content region
 *  button_string               [IN]        String on LSK region. (If this pointer is not NULL, the string will draw on lsk region.)
 *  file_name_ptr               [IN]        Filename to be decode.
 *  is_short_filename           [IN]        File name is short or not.
 *  decode_result_callback      [IN]        Decode result callback function
 *  EX: It may be used as show index/total number.(?)
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory222Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        gdi_color background_color,
        S8 *button_string,
        S8 *file_name_ptr,
        BOOL is_short_filename,
        void (*decode_result_callback) (GDI_RESULT))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    CHAR tmp[4];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cat222_file_name_ptr = file_name_ptr;
    cat222_title_icon = title_icon;
    cat222_title = title;
    cat222_is_short_filename = is_short_filename;
    cat222_decode_result_callback = decode_result_callback;
    cat222_anim_handle = GDI_NULL_HANDLE;

    if (button_string != NULL)
    {
        cat222_button_str_ptr = (S8*) (subMenuDataPtrs[0]); /* use submenu's data buffer, not a good solution */

        memset(cat222_button_str_ptr, 0, MAX_SUB_MENU_SIZE);
        pfnUnicodeStrncpy(cat222_button_str_ptr, button_string, (MAX_SUB_MENU_SIZE / ENCODING_LENGTH) - 1);
    }
    else
    {
        cat222_button_str_ptr = NULL;
    }

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

#ifdef __MMI_TVOUT__
    SetKeyHandler(Cat222ToggleTVDisplay, KEY_STAR, KEY_EVENT_UP);
#endif 

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    ExitCategoryFunction = ExitCategory222Screen;

    TRACE_GDI_FUNCTION();

    dm_setup_category_functions(UI_dummy_function, dm_get_category_history, dm_get_category_history_size);
    dm_register_category_controlled_callback(DrawCate222CategoryControlArea);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY222_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND | DM_NO_BLT;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

    /* Draw Image */
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    Cat222DecodeImage();
#else
    UnicodeNToAnsii( tmp,(PS8)cat222_file_name_ptr,2);
    if (( (tmp[0] == 'C')||(tmp[0] == 'c')) ||( (tmp[0] == 'E')||(tmp[0] == 'e')))
      Cat222DecodeImage();
    else if (tmp[0]=='D'||(tmp[0] == 'd'))
    {
      Cat222DecodeImage();
    }
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory222Screen
 * DESCRIPTION
 *  Exit Category 222 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void ExitCategory222Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* restore TV out mode */
#ifdef __MMI_TVOUT__
    if (cat222_is_fullscr_tvout)
    {
        if (mdi_tvout_is_enable())
        {
            /* restore back to GDI */
            mdi_tvout_set_owner(MDI_TV_OWNER_GDI);

            /* restore back to start mode1 */
            mdi_tvout_set_mode(
                MDI_TV_MODE_LCD_SCR,
                0,  /* layer_width */
                0); /* layer_height */
        }
    }
#endif /* __MMI_TVOUT__ */ 

    /* stop all animation */
   // dingjian add for JasperII

    if (cat222_anim_handle != GDI_NULL_HANDLE)
    {
        gdi_image_stop_animation(cat222_anim_handle);
    }

    ClearHighlightHandler();
    reset_softkeys();

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  DrawCate222CategoryControlArea
 * DESCRIPTION
 *  This function is used to draw the category controlled area of category222 screen
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate222CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 str_height;
    S32 str_width;
    S32 spacing;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (cat222_button_str_ptr != NULL)
    {
        gdi_layer_push_text_clip();
        gdi_layer_set_text_clip(0, UI_device_width - MMI_button_bar_height, UI_device_width - 1, UI_device_height - 1);

        /* draw button string */
        gui_set_font(&MMI_medium_font);

        gui_measure_string((UI_string_type) cat222_button_str_ptr, &str_width, &str_height);

        spacing = (MMI_button_bar_height - str_height) >> 1;
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(spacing + str_width, UI_device_height - MMI_button_bar_height + spacing);
        }
        else
        {
            gui_move_text_cursor(spacing, UI_device_height - MMI_button_bar_height + spacing);
        }

        gui_set_text_color(gui_color(0, 0, 0));//chenhe to fix bug 10618. for the text colour is ought to be diffrent for backgrond color!
        coolsand_UI_print_text((UI_string_type) cat222_button_str_ptr);

        gdi_layer_pop_text_clip();
    }
    Cat222DrawTitle();
}


/*****************************************************************************
 * FUNCTION
 *  Cat222DrawTitle
 * DESCRIPTION
 *  draw cat 222 title
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void Cat222DrawTitle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 str_len;
    S32 index;
    S8 buf_filename_no_ext[(FMGR_MAX_FILE_LEN + 1) * ENCODING_LENGTH];
    S8 buf_filename_dest[(FMGR_MAX_FILE_LEN + 1) * ENCODING_LENGTH];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* clear background */
    gdi_draw_solid_rect(
        0,
        (MMI_title_y + MMI_title_height),
        UI_device_width - 1,
        UI_device_height - MMI_button_bar_height - 1,
        GDI_COLOR_WHITE);

    if (cat222_title == 0)
    {
        /* display filename */
        str_len = pfnUnicodeStrlen(cat222_file_name_ptr);

        /* find file name - by finding "\\" */
        for (index = str_len * 2 - 2; index >= 2; index -= 2)
        {
            if (pfnUnicodeStrncmp((PS8) & cat222_file_name_ptr[index], (PS8) L"\\", 1) == 0)
            {
                index += 2;
                break;
            }
        }

        pfnUnicodeStrncpy((PS8) buf_filename_no_ext, (PS8) (&(cat222_file_name_ptr[index])),(FMGR_MAX_FILE_LEN + 1) * ENCODING_LENGTH);//chenhe modify

        mmi_fmgr_hide_ext_name(buf_filename_no_ext);

        /* short name encoding */
        if (cat222_is_short_filename)
        {
            mmi_chset_mixed_text_to_ucs2_str(
                (U8*) buf_filename_dest,
                sizeof(buf_filename_dest),
                (U8*) buf_filename_no_ext,
                PhnsetGetDefEncodingType());

            ChangeTitle(cat222_title_icon, (PU8) (buf_filename_dest));
        }
        else
        {
            ChangeTitle(cat222_title_icon, (PU8) (buf_filename_no_ext));
        }
    }
    draw_title();

}
extern CHAR gLockImageBuff;


/*****************************************************************************
 * FUNCTION
 *  Cat222DecodeImage
 * DESCRIPTION
 *  Decode Image
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void Cat222DecodeImage(void)
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-4-22 11:42 */
#if 1
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/   
	S32				image_width;
	S32				image_height;
	S32				display_wnd_width;
	S32				display_wnd_height;

#if defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
	S32				rect_height;
	S32				rect_width;
	S32				rect_offset_x;
	S32				rect_offset_y;
#endif

	S32				offset_x;
	S32				offset_y;
	GDI_RESULT		result;
	S32				str_len;
	U16				image_type;
	UI_string_type	err_str_ptr;
	FS_HANDLE		fs_handle;
	INT32			file_size;
	S32				line_height;
		
#ifdef CAT222_DECODE_TIME_TEST
	kal_uint32 	start_tick;
	kal_uint32 	end_tick;
	kal_uint32	decode_ms;
	S8				decode_timer_buf[30];
	S8					buf_filename_no_ext[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];	
	S32			str_width;
	S32			str_height;	
#endif /* CAT222_DECODE_TIME_TEST */
	
//		mmi_trace(1,"Cat222DecodeImage ");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* init */
	err_str_ptr = NULL;

	/* lock */
	gdi_layer_lock_frame_buffer();


	/* push clip */
	gdi_layer_push_clip();
	gdi_layer_push_text_clip();

	gdi_layer_set_clip(	CAT222_IMAGE_SPACING,
								(MMI_title_y+MMI_title_height)+CAT222_IMAGE_SPACING,
								UI_device_width-CAT222_IMAGE_SPACING-1,
								(MMI_title_y+MMI_title_height)+MMI_content_height-CAT222_IMAGE_SPACING-1);


	/* get time tick before decoding */
	#ifdef CAT222_DECODE_TIME_TEST
		kal_get_time(&start_tick);
	#endif /* CAT222_DECODE_TIME_TEST */
	/* Decode file */
	/* get file dimension */
	result = gdi_image_get_dimension_file(	(PS8)cat222_file_name_ptr,
														&image_width,
														&image_height);

	/* temp solution, waiting for decoder has resizer for gif and bmp */
	image_type = gdi_image_get_type_from_file(cat222_file_name_ptr);

#if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE)
	display_wnd_width		= UI_device_width;
	display_wnd_height	= UI_device_height;

#elif defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
	display_wnd_width		= UI_device_width - CAT222_IMAGE_SPACING*2;
	display_wnd_height	= MMI_content_height - CAT222_IMAGE_SPACING*2;
#endif /* CAT222_TIME_FIT_XXX_SIZE */

	/* get file size */
	fs_handle = MMI_FS_Open(cat222_file_name_ptr, FS_READ_ONLY);

	if(fs_handle < 0)
	{
		if(FS_FILENAME_TOO_LONG == fs_handle)
			result = GDI_IMAGE_ERR_FILENAME_TOOLARGE;
		else 
		/* open file failed */
		result = -1;
	}
	else
	{
		MMI_FS_GetFileSize(fs_handle, &file_size);
		MMI_FS_Close(fs_handle);
	}

	#ifndef CAT222_DECODE_TIME_TEST
		/* check file size limit and image siz limit */
		if(result >= 0)
		{
			switch(image_type)
			{
				case GDI_IMAGE_TYPE_JPG_FILE:
					/* decoder will timeout if file too large, we dont need to protect here */
					break;
				
				case GDI_IMAGE_TYPE_BMP_FILE:
				case GDI_IMAGE_TYPE_WBMP_FILE:
					/* file size check - 512k */
					if(file_size > 512*1024)  
					{
						err_str_ptr = (UI_string_type)GetString(STR_ID_FMGR_FILE_TOO_LARGE_NO_DISPLAY);
						result = -1;					
					}
					break;			

				case GDI_IMAGE_TYPE_GIF_FILE:
					/* image size check */
					if((image_width > 640) || (image_height > 480))
					{
						err_str_ptr = (UI_string_type)GetString(STR_ID_FMGR_IMAGE_TOO_LARGE_NO_DISPLAY);
						result = -1;					
					}
					break;	
				case GDI_IMAGE_TYPE_PNG_FILE:
					/*here to limit the size of png*/
					/* if this happens, the decoder will resize the png image just as jpg */					

					break;

			}
		}
	#endif /* CAT222_DECODE_TIME_TEST */


	if(result >= 0)
	{
		if(1)//image_type == GDI_IMAGE_TYPE_JPG_FILE)//wangri and fengzb change 2007-7-18
		{
			#if 1 //#ifdef __GDI_MEMORY_PROFILE_2__ /* wufasong updated 2007.01.28 */
				if( (display_wnd_width >= image_width) && (display_wnd_height >= image_height))
				{

					#if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE)
						/* align preview window at the center of screen */
						offset_x = (display_wnd_width-image_width)>>1;
						offset_y = (display_wnd_height-image_height)>>1;

						/* set clip region as content region */
						gdi_layer_set_clip(0, (MMI_title_y+MMI_title_height), UI_device_width-1, (MMI_title_y+MMI_title_height)+MMI_content_height-1);

					if(image_type == GDI_IMAGE_TYPE_GIF_FILE)
					{
						result = gdi_image_draw_animation_file(offset_x, offset_y, (PS8)cat222_file_name_ptr, &cat222_anim_handle);
					}
					else
					{
						result = gdi_image_draw_file(offset_x, offset_y, (PS8)cat222_file_name_ptr);
					}

					#elif defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
						/* align preview window at the center of screen */
						offset_x = ((display_wnd_width-image_width+1)>>1)+CAT222_IMAGE_SPACING;
						offset_y = ((display_wnd_height-image_height+1)>>1)+(MMI_title_y+MMI_title_height)+CAT222_IMAGE_SPACING;

						/* set clip region as content region */
						gdi_layer_set_clip(0, (MMI_title_y+MMI_title_height), UI_device_width-1, (MMI_title_y+MMI_title_height)+MMI_content_height-1);

						if(image_type == GDI_IMAGE_TYPE_GIF_FILE)
						{
							result = gdi_image_draw_animation_file(offset_x, offset_y, (PS8)cat222_file_name_ptr, &cat222_anim_handle);
						}
						else
						{

							result = gdi_image_draw_file(offset_x, offset_y, (PS8)cat222_file_name_ptr);
						}
						rect_offset_x	= offset_x;
						rect_offset_y	= offset_y;
						rect_width		= image_width;
						rect_height		= image_height;
					#endif /* CAT222_TIME_FIT_XXX_SIZE */

				}
				else
				{
					#if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE) || defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
						S32				resized_offset_x;
						S32				resized_offset_y;
						S32				resized_wd;
						S32				resized_ht;
					#endif

					#if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE)

						gdi_image_util_fit_bbox(display_wnd_width,
														display_wnd_height,
														image_width, image_height,
														&resized_offset_x, &resized_offset_y,
														&resized_wd, &resized_ht);

						gdi_layer_set_clip(0, (MMI_title_y+MMI_title_height), UI_device_width-1, (MMI_title_y+MMI_title_height)+MMI_content_height-1);
						result = gdi_image_draw_resized_file(resized_offset_x, resized_offset_y, resized_wd, resized_ht, (PS8)cat222_file_name_ptr);

					#elif defined(CAT222_IMAGE_FIT_CONTENT_SIZE)

						gdi_image_util_fit_bbox(display_wnd_width,
														display_wnd_height,
														image_width, image_height,
														&resized_offset_x, &resized_offset_y,
														&resized_wd, &resized_ht);

						offset_x = resized_offset_x+CAT222_IMAGE_SPACING;
						offset_y = resized_offset_y+(MMI_title_y+MMI_title_height)+CAT222_IMAGE_SPACING;

						gdi_layer_set_clip(0, (MMI_title_y+MMI_title_height), UI_device_width-1, (MMI_title_y+MMI_title_height)+MMI_content_height-1);
					if(image_type == GDI_IMAGE_TYPE_GIF_FILE)
					{
						result = gdi_image_draw_animation_resized_file(	offset_x, 
																						offset_y, 
																						resized_wd, 
																						resized_ht, 
																						(PS8)cat222_file_name_ptr,
																						&cat222_anim_handle);
					}
					else
					{

						result = gdi_image_draw_resized_file(offset_x, offset_y, resized_wd, resized_ht, (PS8)cat222_file_name_ptr);
					}

						rect_offset_x	= offset_x;
						rect_offset_y	= offset_y;
						rect_width		= resized_wd;
						rect_height		= resized_ht;

					#endif /* CAT222_TIME_FIT_XXX_SIZE */
				}
			#else
				result = -1; /* no jpeg decoder support */
			#endif /* __GDI_MEMORY_PROFILE_2__ */
		}
		else /* not jpeg file */
		{

			#if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE)
				offset_x = (display_wnd_width-image_width)>>1;
				offset_y = (display_wnd_height-image_height)>>1;

				/* set clip region as content region */
				gdi_layer_set_clip(0, (MMI_title_y+MMI_title_height), UI_device_width-1, (MMI_title_y+MMI_title_height)+MMI_content_height-1);

				/* stop all animation and draw file */
				gdi_image_stop_animation_all();
				result = gdi_image_draw_animation_file(offset_x, offset_y, (PS8)cat222_file_name_ptr, &cat222_anim_handle);

			#elif defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
				/* align image at the center of screen */
				offset_x = ((display_wnd_width-image_width+1)>>1)+CAT222_IMAGE_SPACING;
				offset_y = ((display_wnd_height-image_height+1)>>1)+(MMI_title_y+MMI_title_height)+CAT222_IMAGE_SPACING;

				if(offset_x < CAT222_IMAGE_SPACING)
				{
					rect_offset_x	= CAT222_IMAGE_SPACING;
					rect_width		= display_wnd_width;
				}
				else
				{
					rect_offset_x	= offset_x;
					rect_width		= image_width;
				}

				if(offset_y < ((MMI_title_y+MMI_title_height)+CAT222_IMAGE_SPACING))
				{
					rect_offset_y	= (MMI_title_y+MMI_title_height)+CAT222_IMAGE_SPACING;
					rect_height		= display_wnd_height;
				}
				else
				{
					rect_offset_y	= offset_y;
					rect_height		= image_height;
				}

				gdi_layer_set_clip(	CAT222_IMAGE_SPACING,
											(MMI_title_y+MMI_title_height)+CAT222_IMAGE_SPACING,
											UI_device_width-CAT222_IMAGE_SPACING-1,
											(MMI_title_y+MMI_title_height)+MMI_content_height-CAT222_IMAGE_SPACING-1);

				/* stop all animation and draw file */
				gdi_image_stop_animation_all();
				result = gdi_image_draw_animation_file(offset_x, offset_y, (PS8)cat222_file_name_ptr, NULL);

      #endif /* CAT222_TIME_FIT_XXX_SIZE */

			} /* image type */
	}


	/* calc decode time and print on screen */
	#ifdef CAT222_DECODE_TIME_TEST
		if(result >= 0)
		{
			kal_get_time(&end_tick);
			decode_ms = kal_ticks_to_milli_secs(end_tick - start_tick);	

			/* print to a tmp buffer */
			sprintf(buf_filename_no_ext, "%d ms", decode_ms);
			AnsiiToUnicodeString((kal_char*)(decode_timer_buf), (kal_char*)buf_filename_no_ext);
			
			coolsand_UI_measure_string((UI_string_type)decode_timer_buf, &str_width, &str_height);

			offset_x = (UI_device_width-str_width)>>1;
			offset_y = ((MMI_content_height-str_height)>>1)+(MMI_title_y+MMI_title_height);
			coolsand_UI_reset_text_clip();

			coolsand_UI_set_text_color(coolsand_UI_color(0,0,0));
			coolsand_UI_set_text_border_color(coolsand_UI_color(255,255,255));

			coolsand_UI_move_text_cursor(offset_x, offset_y);
					
			coolsand_UI_print_bordered_text((UI_string_type)decode_timer_buf);
		}	
	#endif /* CAT222_DECODE_TIME_TEST */

	if(result >= 0)
	{
	#if defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
		/* if is fit content size mode, draw a rect to make the image like a photo */
		gdi_layer_set_clip(	0,
									(MMI_title_y+MMI_title_height),
									UI_device_width-1,
									(MMI_title_y+MMI_title_height)+MMI_content_height-1);

		gdi_draw_rect(	rect_offset_x-3,
							rect_offset_y-3,
							rect_offset_x+rect_width+2,
							rect_offset_y+rect_height+2,
							GDI_COLOR_BLACK);
	#endif /* CAT222_IMAGE_FIT_CONTENT_SIZE */
	}
	else
	{
		/* print error string */

		if(err_str_ptr == NULL)
		{
			if(result == GDI_JPEG_ERR_IMAGE_TOO_LARGE)
				err_str_ptr = (UI_string_type)GetString(STR_ID_FMGR_IMAGE_TOO_LARGE_NO_DISPLAY);
			else if(result == GDI_JPEG_ERR_FRAME_BUFFER_NOT_ENOUGH)
				err_str_ptr = (UI_string_type)GetString(FMGR_FS_OUT_OF_BUFFERS_TEXT);
			else if(result == GDI_IMAGE_ERR_FILENAME_TOOLARGE)
				err_str_ptr = (UI_string_type)GetString(STR_ID_FMGR_FILENAME_TOO_LONG);		
			else err_str_ptr = (UI_string_type)GetString(STR_ID_FMGR_INVALID_IMAGE_FORMAT);
		}

		MMI_message_string = err_str_ptr;

		/* show error string in multi-line input box */
		str_len = pfnUnicodeStrlen((PS8)MMI_message_string);

		create_multiline_inputbox_set_buffer(MMI_message_string, str_len, str_len,0);
		MMI_multiline_inputbox.flags	|=	UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
											 		|UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
													|UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
													|UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR
													|UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;
	
		MMI_multiline_inputbox.text_font=&MMI_medium_font;

		/* resize to content size and call show_xx_no_draw function to calc its line count */
		resize_multiline_inputbox(UI_device_width-2, MMI_content_height);		
		show_multiline_inputbox_no_draw();
		line_height = get_multiline_inputbox_line_height();

		/* resize and move to both vertical and horizontal center align */
		resize_multiline_inputbox(UI_device_width-2, (MMI_multiline_inputbox.n_lines*line_height)+MULTILINE_INPUTBOX_HEIGHT_PAD+MMI_multiline_inputbox.text_y);
		move_multiline_inputbox(1, MMI_title_height+MMI_title_y+((MMI_content_height-MMI_multiline_inputbox.height)>>1));
		show_multiline_inputbox();

	}
// dingjian 20071101 add for JasperII

	/* pop and blt */
	gdi_layer_pop_text_clip();
	gdi_layer_pop_clip();
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_base_layer(0,0, UI_device_width-1, UI_device_height-1);
	/* call decode result callback */
	if(cat222_decode_result_callback != NULL)
		cat222_decode_result_callback(result);
}

#else
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
   // dingjian 20071101 add for JasperII

    S32 image_width;
    S32 image_height;
    S32 display_wnd_width;
    S32 display_wnd_height;

#if defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
    S32 rect_height = 0;
    S32 rect_width = 0;
    S32 rect_offset_x = 0;
    S32 rect_offset_y = 0;
#endif /* defined(CAT222_IMAGE_FIT_CONTENT_SIZE) */ 

    S32 offset_x = 0;
    S32 offset_y = 0;
    GDI_RESULT result = 0;
    S32 str_len;
    U16 image_type;
    UI_string_type err_str_ptr;
    FS_HANDLE fs_handle;
    U32 file_size;
    S32 line_height;

#ifdef CAT222_DECODE_TIME_TEST
    kal_uint32 start_tick;
    kal_uint32 end_tick;
    kal_uint32 decode_ms;
    S8 decode_timer_buf[30];
    S8 buf_filename_no_ext[(FMGR_MAX_FILE_LEN + 1) * ENCODING_LENGTH];  /* 101705 */
    S32 str_width;
    S32 str_height;
#endif /* CAT222_DECODE_TIME_TEST */ 

#if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE) || defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
    S32 resized_offset_x;
    S32 resized_offset_y;
    S32 resized_width;
    S32 resized_height;
#endif /* defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE) || defined(CAT222_IMAGE_FIT_CONTENT_SIZE) */ 

#ifdef __MMI_TVOUT__
    S32 tvout_resized_offset_x;
    S32 tvout_resized_offset_y;
    S32 tvout_resized_width;
    S32 tvout_resized_height;
    S32 tvout_max_width;
    S32 tvout_max_height;
    GDI_HANDLE tvout_layer;
#endif /* __MMI_TVOUT__ */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* init */
    err_str_ptr = NULL;

    /* lock */
    gdi_layer_lock_frame_buffer();

    /* push clip */
    gdi_layer_push_clip();
    gdi_layer_push_text_clip();

    gdi_layer_set_clip(
        CAT222_IMAGE_SPACING,
        (MMI_title_y + MMI_title_height) + CAT222_IMAGE_SPACING,
        UI_device_width - CAT222_IMAGE_SPACING - 1,
        (MMI_title_y + MMI_title_height) + MMI_content_height - CAT222_IMAGE_SPACING - 1);

    /* get time tick before decoding */
#ifdef CAT222_DECODE_TIME_TEST
    kal_get_time(&start_tick);
#endif 

    /* Decode file */
    /* temp solution, waiting for decoder has resizer for gif and bmp */
    image_type = gdi_image_get_type_from_file(cat222_file_name_ptr);

    /* get file size */
    fs_handle = MMI_FS_Open((U16*) cat222_file_name_ptr, FS_READ_ONLY);

    if (fs_handle < 0)
    {
        /* open file failed */
        result = -1;
    }
    else
    {
        MMI_FS_GetFileSize(fs_handle, (kal_uint32*) & file_size);
        MMI_FS_Close(fs_handle);
    }
    TRACE_GDI_FUNCTION();
    mmi_trace(g_sw_GDI, "cat222_file_name_ptr = %s; image_type = %d", cat222_file_name_ptr,image_type); 
#ifndef CAT222_DECODE_TIME_TEST
    /* check file size limit and image siz limit */
    if (result >= 0)
    {
        switch (image_type)
        {
            case GDI_IMAGE_TYPE_SVG_FILE:
                if (file_size > 200 * 1024)     /* file size check - 200k */
                {
                    err_str_ptr = (UI_string_type) GetString(STR_ID_FMGR_FILE_TOO_LARGE_NO_DISPLAY);
                    result = -1;
                }
                break;
            case GDI_IMAGE_TYPE_JPG_FILE:
                /* decoder will timeout if file too large, we dont need to protect here */
                break;

            case GDI_IMAGE_TYPE_BMP_FILE:
            case GDI_IMAGE_TYPE_WBMP_FILE:

        #if defined(MT6228) || defined(MT6229)
                if (file_size > 1536 * 1024)    /* 1.5M */
        #else 
                if (file_size > 512 * 1024)     /* file size check - 512k */
        #endif 
                {
                    err_str_ptr = (UI_string_type) GetString(STR_ID_FMGR_FILE_TOO_LARGE_NO_DISPLAY);
                    result = -1;
                }
                break;

            case GDI_IMAGE_TYPE_GIF_FILE:
                break;

        }
    }
#endif /* CAT222_DECODE_TIME_TEST */ 

    /* get file dimension */

    if (result >= 0)
    {
        result = gdi_image_get_dimension_file((PS8) cat222_file_name_ptr, &image_width, &image_height);
        if (((image_width <= 0) || (image_height <= 0)) || result < 0)
        {
            result = -1;
            err_str_ptr = (UI_string_type) GetString(STR_ID_FMGR_INVALID_IMAGE_FORMAT);
        }
        else
        {
            cat222_img_width = image_width;
            cat222_img_height = image_height;
        }
    }
    
#if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE)
    display_wnd_width = UI_device_width;
    display_wnd_height = UI_device_height;

#elif defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
    display_wnd_width = UI_device_width - CAT222_IMAGE_SPACING * 2;
    display_wnd_height = MMI_content_height - CAT222_IMAGE_SPACING * 2;
#endif /* CAT222_TIME_FIT_XXX_SIZE */

#ifndef CAT222_DECODE_TIME_TEST
    /* check file size limit and image siz limit */
    if (result >= 0)
    {
        switch (image_type)
        {
            case GDI_IMAGE_TYPE_SVG_FILE:
            case GDI_IMAGE_TYPE_JPG_FILE:
            case GDI_IMAGE_TYPE_BMP_FILE:
            case GDI_IMAGE_TYPE_WBMP_FILE:
                break;
            case GDI_IMAGE_TYPE_GIF_FILE:
                /* image size check */
        #if defined(MT6228) || defined(MT6229)
                if (image_width * image_height > 640 * 480 * 4)
        #else 
                if (image_width * image_height > 640 * 480)
        #endif
                {
                    err_str_ptr = (UI_string_type) GetString(STR_ID_FMGR_IMAGE_TOO_LARGE_NO_DISPLAY);
                    result = -1;
                }
                break;

        }
    }
#endif /* CAT222_DECODE_TIME_TEST */ 

    /* clear gdi working buffer */
    gdi_image_clear_work_buffer();

    if (result >= 0)
    {
        if (TRUE)
        {
            if ((display_wnd_width >= image_width) && (display_wnd_height >= image_height))
            {
            #if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE)
                /* align preview window at the center of screen */
                offset_x = (display_wnd_width - image_width) >> 1;
                offset_y = (display_wnd_height - image_height) >> 1;

                /* set clip region as content region */
                gdi_layer_set_clip(
                    0,
                    (MMI_title_y + MMI_title_height),
                    UI_device_width - 1,
                    (MMI_title_y + MMI_title_height) + MMI_content_height - 1);

                if (image_type == GDI_IMAGE_TYPE_GIF_FILE || image_type == GDI_IMAGE_TYPE_M3D_FILE || image_type == GDI_IMAGE_TYPE_SVG_FILE)
                {
                    result = gdi_image_draw_animation_file(
                                offset_x,
                                offset_y,
                                (PS8) cat222_file_name_ptr,
                                &cat222_anim_handle);
                }
                else
                {
                    result = gdi_image_draw_file(offset_x, offset_y, (PS8) cat222_file_name_ptr);
                }

            #elif defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
                /* align preview window at the center of screen */
                offset_x = ((display_wnd_width - image_width + 1) >> 1) + CAT222_IMAGE_SPACING;
                offset_y =
                    ((display_wnd_height - image_height + 1) >> 1) + (MMI_title_y + MMI_title_height) +
                    CAT222_IMAGE_SPACING;

                /* set clip region as content region */
                gdi_layer_set_clip(
                    0,
                    (MMI_title_y + MMI_title_height),
                    UI_device_width - 1,
                    (MMI_title_y + MMI_title_height) + MMI_content_height - 1);

                if (image_type == GDI_IMAGE_TYPE_GIF_FILE || image_type == GDI_IMAGE_TYPE_M3D_FILE || image_type == GDI_IMAGE_TYPE_SVG_FILE)
                {
                    result = gdi_image_draw_animation_file(
                                offset_x,
                                offset_y,
                                (PS8) cat222_file_name_ptr,
                                &cat222_anim_handle);
                }
                else
                {
                    result = gdi_image_draw_file(offset_x, offset_y, (PS8) cat222_file_name_ptr);
                }

                rect_offset_x = offset_x;
                rect_offset_y = offset_y;
                resized_width = rect_width = image_width;
                resized_height = rect_height = image_height;
            #endif /* CAT222_TIME_FIT_XXX_SIZE */

            }
            else
            {
            #if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE)

                gdi_image_util_fit_bbox(
                    display_wnd_width,
                    display_wnd_height,
                    image_width,
                    image_height,
                    &resized_offset_x,
                    &resized_offset_y,
                    &resized_width,
                    &resized_height);

                gdi_layer_set_clip(
                    0,
                    (MMI_title_y + MMI_title_height),
                    UI_device_width - 1,
                    (MMI_title_y + MMI_title_height) + MMI_content_height - 1);

                if (image_type == GDI_IMAGE_TYPE_GIF_FILE || image_type == GDI_IMAGE_TYPE_M3D_FILE || image_type == GDI_IMAGE_TYPE_SVG_FILE)
                {
                    result = gdi_image_draw_animation_resized_file(
                                resized_offset_x,
                                resized_offset_y,
                                resized_width,
                                resized_height,
                                (PS8) cat222_file_name_ptr);
                }
                else
                {
                    result = gdi_image_draw_resized_file(
                                resized_offset_x,
                                resized_offset_y,
                                resized_width,
                                resized_height,
                                (PS8) cat222_file_name_ptr);
                }
            #elif defined(CAT222_IMAGE_FIT_CONTENT_SIZE)

                gdi_image_util_fit_bbox(
                    display_wnd_width,
                    display_wnd_height,
                    image_width,
                    image_height,
                    &resized_offset_x,
                    &resized_offset_y,
                    &resized_width,
                    &resized_height);

                offset_x = resized_offset_x + CAT222_IMAGE_SPACING;
                offset_y = resized_offset_y + (MMI_title_y + MMI_title_height) + CAT222_IMAGE_SPACING;

                gdi_layer_set_clip(
                    0,
                    (MMI_title_y + MMI_title_height),
                    UI_device_width - 1,
                    (MMI_title_y + MMI_title_height) + MMI_content_height - 1);
                rect_offset_x = offset_x;
                rect_offset_y = offset_y;
                rect_width = resized_width;
                rect_height = resized_height;

                gdi_layer_set_clip(
                            rect_offset_x - 3,
                            rect_offset_y - 3,
                            rect_offset_x + rect_width + 2,
                            rect_offset_y + rect_height + 2);



                if (image_type == GDI_IMAGE_TYPE_GIF_FILE || image_type == GDI_IMAGE_TYPE_M3D_FILE || image_type == GDI_IMAGE_TYPE_SVG_FILE)
                {
                
                    result = gdi_image_draw_animation_resized_file(
                                offset_x,
                                offset_y,
                                resized_width,
                                resized_height,
                                (PS8) cat222_file_name_ptr,
                                &cat222_anim_handle);
                }
                else
                {
                    result = gdi_image_draw_resized_file(
                                offset_x,
                                offset_y,
                                resized_width,
                                resized_height,
                                (PS8) cat222_file_name_ptr);
                }
            #endif /* CAT222_TIME_FIT_XXX_SIZE */
            }
        }
        else    /* not jpeg file */
        {

        #if defined(CAT222_IMAGE_FIT_FULLSCREEN_SIZE)
            offset_x = (display_wnd_width - image_width) >> 1;
            offset_y = (display_wnd_height - image_height) >> 1;

            /* set clip region as content region */
            gdi_layer_set_clip(
                0,
                (MMI_title_y + MMI_title_height),
                UI_device_width - 1,
                (MMI_title_y + MMI_title_height) + MMI_content_height - 1);

            /* stop all animation and draw file */
            gdi_image_stop_animation_all();
            result = gdi_image_draw_animation_file(offset_x, offset_y, (PS8) cat222_file_name_ptr, &cat222_anim_handle);

        #elif defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
            /* align image at the center of screen */
            offset_x = ((display_wnd_width - image_width + 1) >> 1) + CAT222_IMAGE_SPACING;
            offset_y =
                ((display_wnd_height - image_height + 1) >> 1) + (MMI_title_y + MMI_title_height) +
                CAT222_IMAGE_SPACING;

            if (offset_x < CAT222_IMAGE_SPACING)
            {
                rect_offset_x = CAT222_IMAGE_SPACING;
                rect_width = display_wnd_width;
            }
            else
            {
                rect_offset_x = offset_x;
                rect_width = image_width;
            }

            if (offset_y < ((MMI_title_y + MMI_title_height) + CAT222_IMAGE_SPACING))
            {
                rect_offset_y = (MMI_title_y + MMI_title_height) + CAT222_IMAGE_SPACING;
                rect_height = display_wnd_height;
            }
            else
            {
                rect_offset_y = offset_y;
                rect_height = image_height;
            }

            gdi_layer_set_clip(
                CAT222_IMAGE_SPACING,
                (MMI_title_y + MMI_title_height) + CAT222_IMAGE_SPACING,
                UI_device_width - CAT222_IMAGE_SPACING - 1,
                (MMI_title_y + MMI_title_height) + MMI_content_height - CAT222_IMAGE_SPACING - 1);

            /* stop all animation and draw file */
            gdi_image_stop_animation_all();
            result = gdi_image_draw_animation_file(offset_x, offset_y, (PS8) cat222_file_name_ptr, &cat222_anim_handle);
        #endif /* CAT222_TIME_FIT_XXX_SIZE */

        }   /* image type */
    }

#ifdef __MMI_TVOUT__
    /* prepare decode to TV-Out */
    if (cat222_is_fullscr_tvout == TRUE)
    {
        /* set TV out mode */
        if (mdi_tvout_is_enable())
        {
            /* calculate size */
             
            tvout_max_width = 320;
            tvout_max_height = 240;

            if ((tvout_max_width >= image_width) && (tvout_max_height >= image_height))
            {
                tvout_resized_width = image_width;
                tvout_resized_height = image_height;
            }
            else
            {
                gdi_image_util_fit_bbox(
                    tvout_max_width,
                    tvout_max_height,
                    image_width,
                    image_height,
                    &tvout_resized_offset_x,
                    &tvout_resized_offset_y,
                    &tvout_resized_width,
                    &tvout_resized_height);
            }

            /* if is fullscreen, we shall set to owner MMI Owner draw, to avoid GDI's update */
            mdi_tvout_set_owner(MDI_TV_OWNER_APP);

            /* set mode to owner draw */
            mdi_tvout_set_mode(
                MDI_TV_MODE_OWNER_DRAW,
                tvout_resized_width,    /* layer_width */
                tvout_resized_height);  /* layer_height */

            tvout_layer = mdi_tvout_get_active_layer();

            gdi_layer_push_and_set_active(tvout_layer);
            gdi_layer_reset_clip();

            result = gdi_image_draw_resized_file(
                        0,
                        0,
                        tvout_resized_width,
                        tvout_resized_height,
                        (PS8) cat222_file_name_ptr);

            gdi_layer_pop_and_restore_active();

            /* blt to TV */
            mdi_tvout_blt();

        }
    }
#endif /* __MMI_TVOUT__ */ 

    /* calc decode time and print on screen */
#ifdef CAT222_DECODE_TIME_TEST
    if (result >= 0)
    {
        kal_get_time(&end_tick);
        decode_ms = kal_ticks_to_milli_secs(end_tick - start_tick);

        /* print to a tmp buffer */
        sprintf(buf_filename_no_ext, "%d ms", decode_ms);
        AnsiiToUnicodeString((kal_char*) (decode_timer_buf), (kal_char*) buf_filename_no_ext);

        gui_measure_string((UI_string_type) decode_timer_buf, &str_width, &str_height);

        offset_x = (UI_device_width - str_width) >> 1;
        offset_y = ((MMI_content_height - str_height) >> 1) + (MMI_title_y + MMI_title_height);
        gui_reset_text_clip();

        gui_set_text_color(gui_color(0, 0, 0));
        gui_set_text_border_color(gui_color(255, 255, 255));

        gui_move_text_cursor(offset_x, offset_y);

        gui_print_bordered_text((UI_string_type) decode_timer_buf);
    }
#endif /* CAT222_DECODE_TIME_TEST */ 

    if (result >= 0)
    {
    #if defined(CAT222_IMAGE_FIT_CONTENT_SIZE)
        /* if is fit content size mode, draw a rect to make the image like a photo */
        gdi_layer_set_clip(
            0,
            (MMI_title_y + MMI_title_height),
            UI_device_width - 1,
            (MMI_title_y + MMI_title_height) + MMI_content_height - 1);

        gdi_draw_rect(
            rect_offset_x - 3,
            rect_offset_y - 3,
            rect_offset_x + rect_width + 2,
            rect_offset_y + rect_height + 2,
            GDI_COLOR_BLACK);
    #endif /* defined(CAT222_IMAGE_FIT_CONTENT_SIZE) */ 
    }
    else
    {
        /* print error string */

        if (err_str_ptr == NULL)
        {
            if (result == GDI_JPEG_ERR_IMAGE_TOO_LARGE)
            {
                err_str_ptr = (UI_string_type) GetString(FMGR_FS_OUT_OF_BUFFERS_TEXT);
            }
            else if (result == GDI_JPEG_ERR_DECODE_TIME_OUT)
            {
                /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 11:5 */
                err_str_ptr = (UI_string_type) GetString(STR_ID_FMGR_INVALID_IMAGE_FORMAT/*STR_ID_FMGR_SYSTEM_BUSY_TO_DECODE*/);
                /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 11:5 */
            }
            else
            {
                err_str_ptr = (UI_string_type) GetString(STR_ID_FMGR_INVALID_IMAGE_FORMAT);
            }
        }
        gdi_draw_solid_rect(
            offset_x,
            offset_y,
            offset_x + resized_width - 1,
            offset_y + resized_height - 1,
            GDI_COLOR_WHITE);

        MMI_message_string = err_str_ptr;

        /* show error string in multi-line input box */
        str_len = pfnUnicodeStrlen((PS8) MMI_message_string);

        create_multiline_inputbox_set_buffer(MMI_message_string, str_len, str_len, 0);
        MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
            | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
            | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
            | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;

        MMI_multiline_inputbox.text_font = &MMI_medium_font;

        /* resize to content size and call show_xx_no_draw function to calc its line count */
        resize_multiline_inputbox(UI_device_width - 2, MMI_content_height);
        show_multiline_inputbox_no_draw();
        line_height = get_multiline_inputbox_line_height();

        /* resize and move to both vertical and horizontal center align */
        resize_multiline_inputbox(
            UI_device_width - 2,
            (MMI_multiline_inputbox.n_lines * line_height) + MULTILINE_INPUTBOX_HEIGHT_PAD + MMI_multiline_inputbox.text_y);
        move_multiline_inputbox(
            1,
            MMI_title_height + MMI_title_y + ((MMI_content_height - MMI_multiline_inputbox.height) >> 1));

        show_multiline_inputbox();

    }

    /* pop and blt */
    gdi_layer_pop_text_clip();
    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_base_layer(0, 0, UI_device_width - 1, UI_device_height - 1);

    /* call decode result callback */
    if (cat222_decode_result_callback != NULL)
    {
        cat222_decode_result_callback(result);
    }
}
#endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-4-22 11:42 */


/*****************************************************************************
 * FUNCTION
 *  Cat222ToggleTVDisplay
 * DESCRIPTION
 *  Toggle TVout display
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_TVOUT__
static void Cat222ToggleTVDisplay(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 tvout_max_width;
    S32 tvout_max_height;
    S32 tvout_resized_width;
    S32 tvout_resized_height;
    S32 tvout_resized_offset_x;
    S32 tvout_resized_offset_y;
    S32 result;

    GDI_HANDLE tvout_layer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mdi_tvout_is_enable())
    {
        if (cat222_is_fullscr_tvout)
        {
            /* restore back to GDI */
            mdi_tvout_set_owner(MDI_TV_OWNER_GDI);

            /* restore back to start mode1 */
            mdi_tvout_set_mode(
                MDI_TV_MODE_LCD_SCR,
                0,  /* layer_width */
                0); /* layer_height */
            gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

            cat222_is_fullscr_tvout = FALSE;
        }
        else
        {
            /* calculate size */
             
            tvout_max_width = 320;
            tvout_max_height = 240;

            if ((tvout_max_width >= cat222_img_width) && (tvout_max_height >= cat222_img_height))
            {
                tvout_resized_width = cat222_img_width;
                tvout_resized_height = cat222_img_height;
            }
            else
            {
                gdi_image_util_fit_bbox(
                    tvout_max_width,
                    tvout_max_height,
                    cat222_img_width,
                    cat222_img_height,
                    &tvout_resized_offset_x,
                    &tvout_resized_offset_y,
                    &tvout_resized_width,
                    &tvout_resized_height);
            }

            /* if is fullscreen, we shall set to owner MMI Owner draw, to avoid GDI's update */
            mdi_tvout_set_owner(MDI_TV_OWNER_APP);

            /* set mode to owner draw */
            mdi_tvout_set_mode(
                MDI_TV_MODE_OWNER_DRAW,
                tvout_resized_width,    /* layer_width */
                tvout_resized_height);  /* layer_height */

            tvout_layer = mdi_tvout_get_active_layer();

            gdi_layer_push_and_set_active(tvout_layer);
            gdi_layer_reset_clip();

            result = gdi_image_draw_resized_file(
                        0,
                        0,
                        tvout_resized_width,
                        tvout_resized_height,
                        (PS8) cat222_file_name_ptr);

            gdi_layer_pop_and_restore_active();

            /* blt to TV */
            mdi_tvout_blt();

            cat222_is_fullscr_tvout = TRUE;
        }
    }
}
#endif /* __MMI_TVOUT__ */ 

/*****************************************************************************
* [Category223]
*
* This category is for play audio.
*
*  **********************
*  *       Caption      *
*  **********************
*  *                    *
*  *                    *
*  *       (Icon)       *
*  *                    *
*  *                    *
*  *      00:00:00      *
*  *                    *
*  *********    *********
*  *  LSK  *    *  RSK  *
*  **********************
*
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowCategory223Screen
 * DESCRIPTION
 *  Show sategory 223 screen.
 * PARAMETERS
 *  title                   [IN]        Title string id
 *  title_icon              [IN]        Title image id
 *  left_softkey            [IN]        Lsk string id
 *  left_softkey_icon       [IN]        Lsk image id
 *  right_softkey           [IN]        Rsk string id
 *  right_softkey_icon      [IN]        Rsk image id
 *  message_icon            [IN]        Notification message icon
 *  flag                    [IN]        Time display type
 *  duration                [IN]        Audio duration
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory223Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message_icon,
        U16 flag,
        U32 duration,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l, y, h, available_height, title_height = 0;
    S32 iwidth, iheight;
    PU8 image;
    UI_time t;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_stop_list_highlight_effect();
#endif 

    ADD_SCREEN_STRINGS((4, title, left_softkey, right_softkey, message));
    ADD_SCREEN_IMAGES((4, title_icon, left_softkey_icon, right_softkey_icon, message_icon));
    UI_UNUSED_PARAMETER(history_buffer);

    gui_lock_double_buffer();

#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    entry_full_screen();
#endif 

    MMI_menu_shortcut_number = -1;
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();

    dm_data.S32flags = 0;
    if ((left_softkey == 0) && (left_softkey_icon == 0) && (right_softkey == 0) && (right_softkey_icon == 0))
    {
        dm_data.S32flags = DM_NO_SOFTKEY;
    }
    else
    {
        dm_data.S32flags = 0;
        wgui_category_screen_no_buttons = 0;
        change_left_softkey(left_softkey, left_softkey_icon);
        change_right_softkey(right_softkey, right_softkey_icon);
        register_left_softkey_handler();
        register_right_softkey_handler();
        register_default_hide_softkeys();
    }
    if ((title == 0) && (title_icon == 0))
    {
        status_icon = 1;
        ShowStatusIconsTitle();
    }
    else if (!((title == 0xffff) && (title_icon == 0xffff)))
    {
        status_icon = 2;
        MMI_title_string = (UI_string_type) get_string(title);
        MMI_title_icon = (PU8) get_image(title_icon);
    }
    else
    {
        status_icon = 0;
    }
    image = (PU8) get_image(message_icon);
    gui_measure_image(image, &iwidth, &iheight);
    available_height = MMI_content_height;

    if (dm_data.S32flags & DM_NO_SOFTKEY)
    {
        available_height += MMI_button_bar_height;
    }

    switch (status_icon)
    {
        case 0:
            title_height = 0;
            available_height += MMI_status_bar_height;
            break;
        case 1:
            title_height = MMI_title_height + MMI_status_bar_height;
            break;
        case 2:
            title_height = MMI_title_y + MMI_title_height;
            break;
    }
    l = 70; /* width of time display */
    h = 20; /* height of time display */

    wgui_image_clip_x1 = 0;
    wgui_image_clip_x2 = UI_device_width - 1;
    wgui_image_clip_y1 = title_height;
    if (dm_data.S32flags & DM_NO_SOFTKEY)
    {
        wgui_image_clip_y2 = UI_device_height - 1;
    }
    else
    {
        wgui_image_clip_y2 = UI_device_height - MMI_button_bar_height - 1;
    }

	//chenhe,090714,duration is use our tick, 1/16384 second
     Cat223InitDruration(&t, (U32)(duration)>>14);

    set_main_LCD_time_duration_hide_function(Cat223HideDurationDisplay);

    set_time_duration(&t, duration);
	
    cat223_display_duration = MMI_TRUE;

    if (image == NULL)
    {
        y = title_height + (available_height >> 1) - (h >> 1);
        set_time_display(flag, (UI_device_width - l) / 2, y, l, h);
    }
    else
    {
        y = title_height + (available_height >> 1) - ((h + iheight + 4) >> 1);
        if (category_screen_layout_flags & WGUI_ORDER_IMAGE_TEXT)
        {
            wgui_set_animation_image_y((UI_device_width >> 1), y + 2, image);
            y = y + iheight + 4;
            set_time_display(flag, (UI_device_width - l) / 2, y, l, h);
        }
        else
        {
            y = title_height + ((available_height - iheight - h) >> 1);

            set_time_display(flag, (UI_device_width - l) / 2, y, l, h);

            y = title_height + available_height - iheight;

            wgui_set_animation_image_y((UI_device_width >> 1), y, image);
        }
    }

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory223Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY223_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawCate223CategoryControlArea);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory223Screen
 * DESCRIPTION
 *  Exit category 223
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void ExitCategory223Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();

    wgui_category_screen_no_buttons = 0;
    category_screen_layout_flags = 0;

    close_main_LCD_time_display();
}


/*****************************************************************************
 * FUNCTION
 *  DrawCate223CategoryControlArea
 * DESCRIPTION
 *  This function is used to draw the category controlled area of category223 screen
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
static void DrawCate223CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();

    if (status_icon == 1)
    {
        show_status_icons();
    }
    else if (status_icon == 2)
    {
        show_title_status_icon();
        draw_title();
    }
    gui_set_clip(wgui_image_clip_x1, wgui_image_clip_y1, wgui_image_clip_x2, wgui_image_clip_y2);
    wgui_show_transparent_animation();

    if (cat223_display_duration == MMI_TRUE)
    {
        Cat223DrawDurationBar();
    }
}


/*****************************************************************************
 * FUNCTION
 *  Cat223DrawDurationBar
 * DESCRIPTION
 *  Draw duration bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void Cat223DrawDurationBar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    time_display_duration();
}


/*****************************************************************************
 * FUNCTION
 *  Cat223InitDruration
 * DESCRIPTION
 *  Init duration parameters
 * PARAMETERS
 *  t               [?]         
 *  duration        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void Cat223InitDruration(UI_time *t, U32 duration)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 buf1, buf2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    t->DayIndex = 0;
    buf1 = duration / 60;
    t->nSec = duration - buf1 * 60;
    buf2 = buf1 / 60;
    t->nMin = buf1 - buf2 * 60;
    t->nHour = (U8) buf2;
}


/*****************************************************************************
 * FUNCTION
 *  Cat223HideDurationDisplay
 * DESCRIPTION
 *  Hide duration display
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void Cat223HideDurationDisplay(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = *current_MMI_theme->datetime_bar_duration_background_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
#ifdef TGT_GALLITE_G800    
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
#else
    gui_fill_rectangle(x1, y1, x2, y2, c);

#endif
    
    gui_pop_clip();
}

/*****************************************************************************
* [Category224]
*
* This category is for play a video clip. Can be full screen or standard MMI style.
*
*  **********************
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  * (Fullscreen Video) *
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  **********************
*
*****************************************************************************/

#ifdef __MMI_VIDEO__
//#define  BLACK_BUFFER_SIZE ((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)//061506 double buffer

#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)
static U8 *status_icon_buf = NULL;
static GDI_HANDLE status_icon_layer = GDI_ERROR_HANDLE;
#endif

U8 *gBlack_buf = NULL;
static gdi_handle *gBlack_layer = NULL;


/*****************************************************************************
 * FUNCTION
 *  ShowCategory224Screen
 * DESCRIPTION
 *  Show Category 224 Screen. Play video in full screen
 * PARAMETERS
 *  video_id                    [IN]        Video ID
 *  video_filename              [IN]        Video filename
 *  repeat_count                [IN]        Repeat count, 0 means infinite
 *  is_visual_update            [IN]        Update visual to LCM or not
 *  is_play_audio               [IN]        Play audio or not
 *  is_lcd_no_sleep             [IN]        Will let lcd no sleep. This shall always TRUE unless u will handle lcd sleeping event
 *  bg_color                    [IN]        Backgroud color
 *  play_finish_callback        [IN]        Play finish call back hdlr
 *  gui_buffer                  [IN]        Gui_buffer, use to detemin if it is newly enter
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory224Screen(
        U16 video_id,
        PS8 video_filename,
        U16 repeat_count,
        BOOL is_visual_update,
        BOOL is_play_audio,
        BOOL is_lcd_no_sleep,
        GDI_COLOR bg_color,
        void (*play_finish_callback) (MDI_RESULT),
        void *gui_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_VIDEO_PLAYER__
    ShowCategoryVdoplyScreen(
        TRUE,                   /* is_fullscreen *//* cat224 is fullscreen display */
        0,                      /* title_str */
        0,                      /* title_icon */
        0,                      /* lsk_str */
        0,                      /* lsk_icon */
        0,                      /* rsk_str */
        0,                      /* rsk_icon */
        video_id,               /* video_id */
        video_filename,         /* video_filename */
        repeat_count,           /* repeat_count */
        is_visual_update,       /* is_visual_update */
        is_play_audio,          /* is_play_audio */
        is_lcd_no_sleep,        /* is_lcd_no_sleep */
        bg_color,               /* bg_color */
        play_finish_callback,   /* play_finish_callback */
        gui_buffer);            /* gui_buffer */
#endif /* __MMI_VIDEO_PLAYER__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  StopCategory224Video
 * DESCRIPTION
 *  Stop Category224 screen's video
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void StopCategory224Video(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    StopCategoryVdoplyVideo();
}


/*****************************************************************************
 * FUNCTION
 *  PauseCategory224Video
 * DESCRIPTION
 *  Pause Category224 screen's video
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PauseCategory224Video(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PauseCategoryVdoplyVideo();
}


/*****************************************************************************
 * FUNCTION
 *  ResumeCategory224Video
 * DESCRIPTION
 *  Resume Category224 screen's video
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ResumeCategory224Video(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ResumeCategoryVdoplyVideo();
}

/*****************************************************************************
* [ShowCategory225Screen]
*
* Play video screen with title and softkey
*
*  **********************
*  *        Title       *
*  **********************
*  *                    *
*  *                    *
*  *                    *
*  *       (Video)      *
*  *                    *
*  *                    *
*  *                    *
*  ********      ********
*  * LSK  *      *  RSK *
*  **********************
*
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowCategory225Screen
 * DESCRIPTION
 *  Show Generic Play Video Category.
 * PARAMETERS
 *  title_str                   [IN]        Title string id
 *  title_icon                  [IN]        Title icon id
 *  lsk_str                     [IN]        Lsk str id
 *  lsk_icon                    [IN]        Lsk icon id
 *  rsk_str                     [IN]        Lsk str id
 *  rsk_icon                    [IN]        Lsk icon id
 *  video_id                    [IN]        Video ID
 *  video_filename              [IN]        Video filename
 *  repeat_count                [IN]        Repeat count, 0 means infinite
 *  is_visual_update            [IN]        Update visual to LCM or not
 *  is_play_audio               [IN]        Play audio or not
 *  is_lcd_no_sleep             [IN]        Will let lcd no sleep. This shall always TRUE unless u will handle lcd sleeping event
 *  bg_color                    [IN]        Backgroud color
 *  play_finish_callback        [IN]        Play finish call back hdlr
 *  gui_buffer                  [IN]        Gui_buffer, use to detemin if it is newly enter
 * RETURNS
 *  void
 *****************************************************************************/
extern void ShowCategory225Screen(
                U16 title_str,
                U16 title_icon,
                U16 lsk_str,
                U16 lsk_icon,
                U16 rsk_str,
                U16 rsk_icon,
                U16 video_id,
                PS8 video_filename,
                U16 repeat_count,
                BOOL is_visual_update,
                BOOL is_play_audio,
                BOOL is_lcd_no_sleep,
                GDI_COLOR bg_color,
                void (*play_finish_callback) (MDI_RESULT),
                void *gui_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_VIDEO_PLAYER__
    ShowCategoryVdoplyScreen(
        FALSE,                  /* is_fullscreen */
        title_str,              /* title_str */
        title_icon,             /* title_icon */
        lsk_str,                /* lsk_str */
        lsk_icon,               /* lsk_icon */
        rsk_str,                /* rsk_str */
        rsk_icon,               /* rsk_icon */
        video_id,               /* video_id */
        video_filename,         /* video_filename */
        repeat_count,           /* repeat_count */
        is_visual_update,       /* is_visual_update */
        is_play_audio,          /* is_play_audio */
        is_lcd_no_sleep,        /* is_lcd_no_sleep */
        bg_color,               /* bg_color */
        play_finish_callback,   /* play_finish_callback */
        gui_buffer);            /* gui_buffer */
#endif /* __MMI_VIDEO_PLAYER__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  DrawCateVdoCategoryControlArea
 * DESCRIPTION
 *  draw vdo contro area
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coord
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCateVdoCategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 resized_width, resized_height;
    S32 resized_offset_x, resized_offset_y;
    S32 content_width, content_height;
    S32 content_offset_x, content_offset_y;
    MDI_RESULT video_ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!cat_vdoply_is_full_screen)
    {
        content_width = MMI_content_width;
        content_height = MMI_content_height;
        content_offset_x = MMI_content_x;
        content_offset_y = MMI_content_y;
    }
    else
    {
        content_width = UI_device_width;
        content_height = UI_device_height;
        content_offset_x = 0;
        content_offset_y = 0;
    }


    /* 061506 double buffer Start */
    /*******************************************************************************
    *  Special status icon handle with double buffer
    *******************************************************************************/
    if (wgui_is_touch_title_bar_buttons())
    {
    #if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)
        U32 icon_flag;
    
        if (get_status_icon_bar_layer(0)==GDI_ERROR_HANDLE)
        {
            status_icon_buf = mmi_frm_scrmem_alloc(STATUS_ICON_BAR_DOUBLE_BUFFER_SIZE);
            MMI_ASSERT(status_icon_buf != NULL);
            gdi_layer_create_double_using_outside_memory(0, 0, UI_device_width, MMI_status_bar_height, &status_icon_layer, status_icon_buf, STATUS_ICON_BAR_DOUBLE_BUFFER_SIZE);
            MMI_ASSERT(status_icon_layer != GDI_ERROR_HANDLE);
            dm_add_blt_layer(status_icon_layer, DM_LAYER_TOP);
            gdi_layer_push_and_set_active(status_icon_layer);
            gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
            gdi_layer_clear(GDI_COLOR_TRANSPARENT);
            gdi_layer_pop_and_restore_active();
        }
        set_status_icon_bar_layer(0, status_icon_layer);
        icon_flag = get_status_icon_bar_type(0);
        icon_flag |= STATUS_ICON_BAR_ENABLE_DOUBLE_BUFFER;
        set_status_icon_bar_type(0, icon_flag);
        show_status_icons();
    #endif
    }
    /* 061506 double buffer End */

   /********************************************************************************
   *  Create Video Layer 
   ********************************************************************************/
    /* video is successfully opened, create video playback layer */
    if (cat_vdoply_is_video_open)
    {
        /* we need play video on an extra layer, so will use multi-layer */
        gdi_layer_multi_layer_enable();
        gdi_layer_get_base_handle(&wgui_base_layer);

        if (wgui_is_touch_title_bar_buttons())
        {
            gBlack_buf = mmi_frm_scrmem_alloc(BLACK_BUFFER_SIZE);

            if (GDI_LAYER_SUCCEED != dm_create_layer_using_outside_memory(
                                        0,
                                        0,
                                        UI_device_width,
                                        UI_device_height,
                                        &gBlack_layer,
                                        gBlack_buf,
                                        BLACK_BUFFER_SIZE,
                                        DM_LAYER_BOTTOM))
            {
                MMI_ASSERT(0);
            }
            gdi_layer_push_and_set_active(*gBlack_layer);
            gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
            gdi_layer_clear(GDI_COLOR_BLACK);
            gdi_layer_pop_and_restore_active();
        }

        if ((wgui_video_info.width > content_width) || (wgui_video_info.height > content_height))
        {
            /* video larger than drawable region, need resize */

            /* calc draw size */
            gdi_util_fit_box(
                GDI_UTIL_MODE_LONG_SIDE_FIT,
                content_width,
                content_height,
                wgui_video_info.width,
                wgui_video_info.height,
                &resized_offset_x,
                &resized_offset_y,
                &resized_width,
                &resized_height);
        }
        else
        {
            /* video smaller than content region */
            if (!cat_vdoply_is_full_screen)
            {
                /* not full screen mode, play default size */
                resized_offset_x = (content_width - wgui_video_info.width) >> 1;
                resized_offset_y = (content_height - wgui_video_info.height) >> 1;

                resized_width = wgui_video_info.width;
                resized_height = wgui_video_info.height;
            }
            else
            {
                /* full screen mode, stretch (enlarge) it to fit content region */
                gdi_util_fit_box(
                    GDI_UTIL_MODE_LONG_SIDE_FIT,
                    content_width,
                    content_height,
                    wgui_video_info.width,
                    wgui_video_info.height,
                    &resized_offset_x,
                    &resized_offset_y,
                    &resized_width,
                    &resized_height);
            }

        }

        /* shift to content position */
        resized_offset_x += content_offset_x;
        resized_offset_y += content_offset_y;

        if (wgui_is_touch_title_bar_buttons())
        {
            dm_create_layer(
                resized_offset_x,
                resized_offset_y,
                resized_width,
                resized_height,
                &cat_vdoply_video_layer,
                DM_LAYER_ACTIVE_TOP);
        }
        else
        {
            dm_create_layer(
                resized_offset_x,
                resized_offset_y,
                resized_width,
                resized_height,
                &cat_vdoply_video_layer,
                DM_LAYER_ACTIVE_BOTTOM);
        }

        /* clear this layer with black */
        gdi_layer_push_and_set_active(cat_vdoply_video_layer);
        gdi_layer_clear(GDI_COLOR_BLACK);
        gdi_layer_pop_and_restore_active();

        /* Get first frame and start to play */
        video_ret = mdi_video_ply_seek_and_get_frame(0, cat_vdoply_video_layer);
        if (video_ret < 0)
        {
            cat_vdoply_is_video_play = FALSE;
        }
        else
        {
            cat_vdoply_is_video_play = TRUE;
        }
    }

}
extern void mmi_vdo_get_display_rect(U16 *width, U16 *height);

/*****************************************************************************
* [ShowCategoryVdoplyScreen]
*
* Generic video play screen.
*
*  **********************
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  * (Fullscreen Video) *
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  **********************
*
*  **********************
*  *        Title       *
*  **********************
*  *                    *
*  *                    *
*  *                    *
*  *       (Video)      *
*  *                    *
*  *                    *
*  *                    *
*  ********      ********
*  * LSK  *      *  RSK *
*  **********************
*
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowCategoryVdoplyScreen
 * DESCRIPTION
 *  Show Generic Play Video Category.
 * PARAMETERS
 *  is_fullscreen               [IN]        Is full screen display, if false, will draw title and softkeys
 *  title_str                   [IN]        Title string id
 *  title_icon                  [IN]        Title icon id
 *  lsk_str                     [IN]        Lsk str id
 *  lsk_icon                    [IN]        Lsk icon id
 *  rsk_str                     [IN]        Lsk str id
 *  rsk_icon                    [IN]        Lsk icon id
 *  video_id                    [IN]        Video ID
 *  video_filename              [IN]        Video filename
 *  repeat_count                [IN]        Repeat count, 0 means infinite
 *  is_visual_update            [IN]        Update visual to LCM or not
 *  is_play_audio               [IN]        Play audio or not
 *  is_lcd_no_sleep             [IN]        Will let lcd no sleep. This shall always TRUE unless u will handle lcd sleeping event
 *  bg_color                    [IN]        Backgroud color
 *  play_finish_callback        [IN]        Play finish call back hdlr
 *  gui_buffer                  [IN]        Gui_buffer, use to detemin if it is newly enter
 * RETURNS
 *  void
 *****************************************************************************/
static void ShowCategoryVdoplyScreen(
                BOOL is_fullscreen,
                U16 title_str,
                U16 title_icon,
                U16 lsk_str,
                U16 lsk_icon,
                U16 rsk_str,
                U16 rsk_icon,
                U16 video_id,
                PS8 video_filename,
                U16 repeat_count,
                BOOL is_visual_update,
                BOOL is_play_audio,
                BOOL is_lcd_no_sleep,
                GDI_COLOR bg_color,
                void (*play_finish_callback) (MDI_RESULT),
                void *gui_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    U16 btn_prev_item_id, btn_next_item_id;
    S32 content_width, content_height;
    S32 content_offset_x, content_offset_y;
    MDI_RESULT video_ret = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* enter a sublcd screen with static image */
    /* 
     * this is very important since we cant draw anything on sublcd while video is playing,
     * which will casue driver assert.
     * 
     * caller of this category may ignore sublcd handling, it will be handled in this category 
     */
#ifdef __MMI_SUBLCD__
    if (!cat_vdoply_is_sublcd_display)
    {
        ForceSubLCDScreen(EnterCategoryVdoplySubScreen);
    }
#endif /* __MMI_SUBLCD__ */ 

    /* arg check */
    MMI_ASSERT((video_id == 0 && video_filename != NULL) || (video_id != 0 && video_filename == NULL));

    /* clear keys */
    clear_category_screen_key_handlers();
    clear_key_handlers();
    clear_left_softkey();
    clear_right_softkey();

    /* init var */
    cat_vdoply_is_video_from_id = (video_id != 0) ? TRUE : FALSE;
    cat_vdoply_video_is_visual_update = is_visual_update;
    cat_vdoply_video_is_play_audio = is_play_audio;
    cat_vdoply_video_is_lcd_no_sleep = is_lcd_no_sleep;
    cat_vdoply_play_finish_callback = play_finish_callback;

    /* stop MMI sleep */
    if (cat_vdoply_video_is_lcd_no_sleep)
    {
        TurnOnBacklight(0);
    }

    /* set audio volumn */
    if (gui_buffer == NULL)
    {
        /* first time enter */
        cat_vdoply_audio_volumn = LEVEL5;
    }

    if (cat_vdoply_video_is_play_audio)
    {
        /* full screen play do not allows vol adjustment */
        if (!is_fullscreen)
        {
            SetKeyHandler(CatVdoplyVolUp, KEY_VOL_UP, KEY_EVENT_DOWN);
            SetKeyHandler(CatVdoplyVolDown, KEY_VOL_DOWN, KEY_EVENT_DOWN);
        }

        /* suspend background audio */
        mdi_audio_suspend_background_play();
        
        /* init audio volumn */
        mdi_audio_set_volume(MDI_VOLUME_MEDIA, cat_vdoply_audio_volumn);
    }

    /*
     * user may request to repeat play many times, and wants a callback bewteen each play,
     * so we have to handle repeat play within this category by seeking to first frame 
     * after play finished each time, instead of using driver's repeat play.
     */

    cat_vdoply_video_repeat_count = (repeat_count == 0) ? 0xffff : repeat_count;        /* 0xff means infinte play */

    if (repeat_count != 0)
    {
        cat_vdoply_video_repeat_count--;
    }

   /********************************************************************************
   *  Draw non-full screen UI
   ********************************************************************************/
    if (!is_fullscreen)
    {
        /* not fullscreen, draw title and softkey */

        /* init softkey */
        change_left_softkey(lsk_str, lsk_icon);
        change_right_softkey(rsk_str, rsk_icon);
        register_left_softkey_handler();
        register_right_softkey_handler();
        register_default_hide_softkeys();

        /* init title */
        MMI_title_string = (UI_string_type) get_string(title_str);
        MMI_title_icon = (PU8) get_image(title_icon);

        /* set content size */
        if (wgui_is_touch_title_bar_buttons())
        {
            content_width = UI_device_width;
            content_height = UI_device_height;
            content_offset_x = 0;
            content_offset_y = 0;
            cat_vdoply_is_full_screen = TRUE;
            title_bg_id = IMAGE_VIEW_TITLEBAR_BG;
            wgui_set_wallpaper_on_bottom(MMI_TRUE);
        }
        else
        {
            content_width = MMI_content_width;
            content_height = MMI_content_height;
            content_offset_x = MMI_content_x;
            content_offset_y = MMI_content_y;
            cat_vdoply_is_full_screen = FALSE;
        }

        /* clear shortcut num */
        MMI_menu_shortcut_number = -1;

        /* draw to buffer, but not blt to LCM by using lock */
        gdi_layer_lock_frame_buffer();

        gdi_layer_reset_clip();
        gdi_layer_reset_text_clip();

        /* clear content color */
        if (wgui_is_touch_title_bar_buttons())
        {
            gdi_draw_solid_rect(0, 0, UI_device_width - 1, UI_device_height - 1, GDI_COLOR_TRANSPARENT);
        }
        else
        {
            gdi_draw_solid_rect(
                MMI_content_x,
                MMI_content_y,
                MMI_content_x + MMI_content_width - 1,
                MMI_content_y + MMI_content_height - 1,
                GDI_COLOR_WHITE);
            clear_buttonbar();
        }

        /* 061506 double buffer Start */
        if (wgui_is_touch_title_bar_buttons())
        {
        #if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)
            gOnFullScreen = MMI_IN_NORMAL_SCREEN;//063006 title shift
            show_status_icon_bar(0);
            register_hide_status_icon_bar(0, hide_status_icons_bar0);
        #endif
        }
        //show_title_status_icon();
        /* 061506 double buffer End */
        draw_title();
        show_softkey_background();
        show_left_softkey();
        show_right_softkey();

        gdi_layer_unlock_frame_buffer();

    }
    else
    {
        /* entry full screen */
        entry_full_screen();

        /* clear full screen */
        gdi_layer_clear(bg_color);

        /* full screen */
        content_width = UI_device_width;
        content_height = UI_device_height;
        content_offset_x = 0;
        content_offset_y = 0;
        cat_vdoply_is_full_screen = TRUE;
    }

   /********************************************************************************
   *  Open Video
   ********************************************************************************/
    if (video_id != 0)
    {
        /* source from id */
        /* always open with audio, if do not want audio, mute it by calling mdi function */
        video_ret = mdi_video_ply_open_clip_id(video_id, &wgui_video_info, is_play_audio);

        cat_vdoply_is_video_open = (video_ret >= 0) ? TRUE : FALSE;
    }
    else if (video_filename != NULL)
    {
        /* source from file */
        /* always open with audio, if do not want audio, dsiable it in play function */
        video_ret = mdi_video_ply_open_clip_file(video_filename, &wgui_video_info, is_play_audio);

        cat_vdoply_is_video_open = (video_ret >= 0) ? TRUE : FALSE;

		mmi_vdo_get_display_rect(&wgui_video_info.width, &wgui_video_info.height);

        mmi_trace(g_sw_CC,"Func: %s image_width:%d image_height:%d", __FUNCTION__, wgui_video_info.width, wgui_video_info.height);

    }
    else
    {
        /* no proper source */
        MMI_ASSERT(0);
    }

    /* check if the title bar buttons are enabled for the particular screen. */
    if (wgui_is_touch_title_bar_buttons())
    {
        btn_prev_item_id = dm_add_button(
                            NULL,
                            get_image(LEFT_RED_ARROW),
                            get_image(LEFT_RED_ARROW),
                            cat226_hide_button_full_screen);
        btn_next_item_id = dm_add_button(
                            NULL,
                            get_image(RIGHT_RED_ARROW),
                            get_image(RIGHT_RED_ARROW),
                            cat226_hide_button_full_screen);

    #if defined(__MMI_TOUCH_SCREEN__)

        /* register the handler for pen event down istead of pen event up. */
        dm_register_button_functions(btn_prev_item_id, KEY_EVENT_UP, gMMI_touch_title_button1_down_handler);
        dm_register_button_functions(btn_next_item_id, KEY_EVENT_UP, gMMI_touch_title_button2_down_handler);

    #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    }

    RedrawCategoryFunction = dm_redraw_category_screen;
    ExitCategoryFunction = ExitCategoryVdoplyScreen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;

    if (wgui_is_touch_title_bar_buttons())
    {
        dm_data.S32CatId = MMI_CATEGORY_VDOPLY_BUTTON;
    }
    else
    {
        dm_data.S32CatId = MMI_CATEGORY_VDOPLY;
    }

    dm_data.S32ScrId = (S32) GetActiveScreenId();
    if (is_fullscreen)
    	{
        dm_data.S32flags = DM_NO_TITLE | DM_NO_STATUS_BAR | DM_NO_SOFTKEY | DM_BUTTON_DISABLE_BACKGROUND;
	 mdi_video_ply_fullscreen_display_set(UI_device_width,UI_device_height,0,0,0);
    }
    else
    {
        dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
	 mdi_video_ply_fullscreen_display_set(MMI_content_width,MMI_content_height,MMI_content_x,MMI_content_y,0);
    }

    dm_setup_data(&dm_data);

    dm_register_category_controlled_callback(DrawCateVdoCategoryControlArea);
    dm_redraw_category_screen();
    /* 
     * register exit category function befor calling the callback function, 
     * since this call back may exit this category 
     */

    if (cat_vdoply_is_video_play == TRUE)
    {
        //gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);//061506 double buffer
        if (wgui_is_touch_title_bar_buttons())
        {
        /* 061506 double buffer Start */
        #if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)
            //black-video-base-status
            video_ret = mdi_video_ply_play(
                            cat_vdoply_video_layer,
                            GDI_LAYER_ENABLE_LAYER_0 | GDI_LAYER_ENABLE_LAYER_1 | GDI_LAYER_ENABLE_LAYER_2 | GDI_LAYER_ENABLE_LAYER_3,
                            GDI_LAYER_ENABLE_LAYER_1,
                            1,                              /* repeat count */
                            is_visual_update,               /* update to LCM or not */
                            MDI_DEVICE_SPEAKER2,            /* speaker & earphone */
                            MDI_VIDEO_LCD_ROTATE_0,         /* rotate */
                            100,                            /* 1x play speed */
                            CatVdoplyPlayFinishCallback);   /* hook callback */
        #else
            //black-video-base
            video_ret = mdi_video_ply_play(
                            cat_vdoply_video_layer,
                            GDI_LAYER_ENABLE_LAYER_0 | GDI_LAYER_ENABLE_LAYER_1 | GDI_LAYER_ENABLE_LAYER_2,
                            GDI_LAYER_ENABLE_LAYER_1,
                            1,                              /* repeat count */
                            is_visual_update,               /* update to LCM or not */
                            MDI_DEVICE_SPEAKER2,            /* speaker & earphone */
                            MDI_VIDEO_LCD_ROTATE_0,         /* rotate */
                            100,                            /* 1x play speed */
                            CatVdoplyPlayFinishCallback);   /* hook callback */
        #endif
        }
        else
        {
            //base-video
            video_ret = mdi_video_ply_play(
                            cat_vdoply_video_layer,
                            GDI_LAYER_ENABLE_LAYER_0 | GDI_LAYER_ENABLE_LAYER_1,
                            GDI_LAYER_ENABLE_LAYER_1,
                            1,                              /* repeat count */
                            is_visual_update,               /* update to LCM or not */
                            MDI_DEVICE_SPEAKER2,            /* speaker & earphone */
                            MDI_VIDEO_LCD_ROTATE_0,         /* rotate */
                            100,                            /* 1x play speed */
                            CatVdoplyPlayFinishCallback);   /* hook callback */
        }
        /* 061506 double buffer Start */

        cat_vdoply_is_video_play = (video_ret >= 0) ? TRUE : FALSE;
        cat_vdoply_is_video_force_stop = FALSE;
    }
	else
	{
	#ifdef __FF_AUDIO_SET__
		MusicEndAudioMode();//chenhe add for bug 10343 
	#endif	
	}

    if (cat_vdoply_is_video_play == FALSE)
    {
        /* open file failed or play failed - reutrn error cause */
        if (cat_vdoply_play_finish_callback != NULL)
        {
            cat_vdoply_play_finish_callback(video_ret);
        }
    }

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategoryVdoplyScreen
 * DESCRIPTION
 *  Exit Category 224 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void ExitCategoryVdoplyScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (cat_vdoply_is_video_open)
    {
        /* stop video playing */
        if (cat_vdoply_is_video_play)
        {
            mdi_video_ply_stop();
            cat_vdoply_is_video_play = FALSE;
        }

        /* close video source */
        if (cat_vdoply_is_video_from_id)
        {
            /* source from id */
            mdi_video_ply_close_clip_id();
            cat_vdoply_is_video_open = FALSE;
        }
        else
        {
            /* source from file */
            mdi_video_ply_close_clip_file();
            cat_vdoply_is_video_open = FALSE;
        }
	  	/* enable key pad tone */
	  	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
    }

    /* let MMI can sleep */
    if (cat_vdoply_video_is_lcd_no_sleep)
    {
        TurnOffBacklight();
    }

    if (cat_vdoply_video_is_play_audio)
    {
        /* resume background audio */
        mdi_audio_resume_background_play();
    }

    if (wgui_is_touch_title_bar_buttons())
    {
    /* 061506 double buffer Start */
    /*******************************************************************************
    *  Special status icon handle with double buffer
    *******************************************************************************/
    #if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)
        U32 icon_flag;
    
        if (get_status_icon_bar_layer(0)!=GDI_ERROR_HANDLE && status_icon_buf!=NULL)
        {
            mmi_frm_scrmem_free(status_icon_buf);
            status_icon_buf = NULL;
            status_icon_layer = GDI_ERROR_HANDLE;
            set_status_icon_bar_layer(0, GDI_ERROR_HANDLE);
        }
        icon_flag = get_status_icon_bar_type(0);
        icon_flag &= ~ STATUS_ICON_BAR_ENABLE_DOUBLE_BUFFER;
        set_status_icon_bar_type(0, icon_flag);
    #endif
    /* 061506 double buffer End */
    
        title_bg_id = 0;

        mmi_frm_scrmem_free(gBlack_buf);

        gBlack_buf = NULL;
        gBlack_layer = NULL;
        entry_full_screen();
    }
    wgui_reset_touch_title_bar_buttons();

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;

#ifdef __MMI_SUBLCD__
    /* exit sublcd */
    if (cat_vdoply_is_sublcd_display)
    {
        GoBackSubLCDHistory();
        cat_vdoply_is_sublcd_display = FALSE;
    }
#endif /* __MMI_SUBLCD__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  EnterCategoryVdoplySubScreen
 * DESCRIPTION
 *  Entery Category vdoply's sublcd screen, will draw an icon on sublcd
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_SUBLCD__
static void EnterCategoryVdoplySubScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ExecSubLCDCurrExitHandler();

    cat_vdoply_is_sublcd_display = TRUE;

    ShowCategory332Screen(IMG_ID_VDOPLY_SUBLCD_ICON);

    /* force all animation on sulcd stop, this is prevent IMG_ID_VDOPLY_SUBLCD_ICON is an animation gif */
    gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
    gdi_image_stop_animation_all();
    gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);

    SetSubLCDExitHandler(ExitCategoryVdoplySubScreen);

}
#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  ExitCategoryVdoplySubScreen
 * DESCRIPTION
 *  Exit Category Vdoply's sublcd screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_SUBLCD__
static void ExitCategoryVdoplySubScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    SubLCDHistoryNode sub_history;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* someone may exit/draw sublcd first then exit mainlcd, this may cause driver assert */
    if (cat_vdoply_is_video_play)
    {
        mdi_video_ply_stop();
        cat_vdoply_is_video_play = FALSE;
    }

    cat_vdoply_is_sublcd_display = FALSE;

    sub_history.entryFuncPtr = EnterCategoryVdoplySubScreen;
    AddSubLCDHistory(&sub_history);

}
#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  StopCategoryVdoplyVideo
 * DESCRIPTION
 *  Stop Category Vdoply screen's video
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void StopCategoryVdoplyVideo(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cat_vdoply_is_video_force_stop = TRUE;

    if (cat_vdoply_is_video_open)
    {
        /* stop video playing */
        if (cat_vdoply_is_video_play)
        {
            mdi_video_ply_stop();
            cat_vdoply_is_video_play = FALSE;
        }
    }
  	/* enable key pad tone */
  	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
}


/*****************************************************************************
 * FUNCTION
 *  PauseCategoryVdoplyVideo
 * DESCRIPTION
 *  Pause video play
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void PauseCategoryVdoplyVideo(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* stop video playing */
    if (cat_vdoply_is_video_open)
    {
        if (cat_vdoply_is_video_play)
        {
            mdi_video_ply_stop();
            cat_vdoply_is_video_play = FALSE;
        }
    }
    else
    {
        /* shall not call thie when video is not opened */
         
        // MMI_ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  ResumeCategoryVdoplyVideo
 * DESCRIPTION
 *  Resume video play
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void ResumeCategoryVdoplyVideo(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MDI_RESULT video_ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* the callback function maybe already exit this category and close video file */
    /* check before play this video */
    if (cat_vdoply_is_video_open)
    {
        /* start play again */
        if (wgui_is_touch_title_bar_buttons())
        {
            video_ret = mdi_video_ply_play(
                            cat_vdoply_video_layer,
                            GDI_LAYER_ENABLE_LAYER_0 | GDI_LAYER_ENABLE_LAYER_1 | GDI_LAYER_ENABLE_LAYER_2,
                            GDI_LAYER_ENABLE_LAYER_1,
                            1,                              /* repeat count */
                            cat_vdoply_video_is_visual_update,   /* update to LCM or not */
                            MDI_DEVICE_SPEAKER2,            /* speaker & earphone */
                            MDI_VIDEO_LCD_ROTATE_0,         /* rotate */
                            100,                            /* 1x play speed */
                            CatVdoplyPlayFinishCallback);   /* hook callback */
        }
        else
        {
            video_ret = mdi_video_ply_play(
                            cat_vdoply_video_layer,
                            GDI_LAYER_ENABLE_LAYER_0 | GDI_LAYER_ENABLE_LAYER_1,
                            GDI_LAYER_ENABLE_LAYER_1,
                            1,                              /* repeat count */
                            cat_vdoply_video_is_visual_update,   /* update to LCM or not */
                            MDI_DEVICE_SPEAKER2,            /* speaker & earphone */
                            MDI_VIDEO_LCD_ROTATE_0,         /* rotate */
                            100,                            /* 1x play speed */
                            CatVdoplyPlayFinishCallback);   /* hook callback */
        }

        if (video_ret < 0)
        {
            cat_vdoply_play_finish_callback(video_ret);

            if (cat_vdoply_play_finish_callback != NULL)
            {
                cat_vdoply_play_finish_callback(video_ret);
            }
        }
        else
        {
            cat_vdoply_is_video_play = TRUE;
        }
    }
    else
    {
        /* shall not call thie when video is not opened */
         
        // MMI_ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  CatVdoplyPlayFinishCallback
 * DESCRIPTION
 *  Play finish callback function, will handle looping here
 * PARAMETERS
 *  result      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void CatVdoplyPlayFinishCallback(MDI_RESULT result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MDI_RESULT video_ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (result >= 0)
    {
        /* success play finished */

        /* call callback function */
        if (cat_vdoply_play_finish_callback != NULL);
        {
            cat_vdoply_play_finish_callback(result);
        }

        /* teminate condition */
        if (cat_vdoply_video_repeat_count == 0)
        {
            return;
        }

        /* force stopped, will not loop again */
        if (cat_vdoply_is_video_force_stop == TRUE)
        {
            return;
        }

        /* calc remain count */
        if (cat_vdoply_video_repeat_count > 0)  /* play again */
        {
            if (cat_vdoply_video_repeat_count != 0xffff)
            {
                /* not infite play */
                cat_vdoply_video_repeat_count--;
            }
        }

        /* the callback function maybe already exit this category and close video file */
        /* check before play this video */
        if (cat_vdoply_is_video_open)
        {
            /* seek to first frame */
//			video_ret = mdi_video_ply_seek(0);
			video_ret = mdi_video_ply_seek_and_get_frame(0,cat_vdoply_video_layer);
//			COS_Sleep(300);

            if (video_ret < 0)
            {
                if (cat_vdoply_play_finish_callback != NULL)
                {
                    cat_vdoply_play_finish_callback(video_ret);
                }
                return;
            }

            /* start play again */
            if (wgui_is_touch_title_bar_buttons())
            {
                video_ret = mdi_video_ply_play(
                                cat_vdoply_video_layer,
                                GDI_LAYER_ENABLE_LAYER_0 | GDI_LAYER_ENABLE_LAYER_1 | GDI_LAYER_ENABLE_LAYER_2,
                                GDI_LAYER_ENABLE_LAYER_1,
                                1,                      /* repeat count */
                                cat_vdoply_video_is_visual_update,       /* update to LCM or not */
                                MDI_DEVICE_SPEAKER2,    /* speaker & earphone */
                                MDI_VIDEO_LCD_ROTATE_0, /* rotate */
                                100,                    /* 1x play speed */
                                CatVdoplyPlayFinishCallback);    /* hook callback */
            }
            else
            {
                video_ret = mdi_video_ply_play(
                                cat_vdoply_video_layer,
                                GDI_LAYER_ENABLE_LAYER_0 | GDI_LAYER_ENABLE_LAYER_1,
                                GDI_LAYER_ENABLE_LAYER_1,
                                1,                      /* repeat count */
                                cat_vdoply_video_is_visual_update,       /* update to LCM or not */
                                MDI_DEVICE_SPEAKER2,    /* speaker & earphone */
                                MDI_VIDEO_LCD_ROTATE_0, /* rotate */
                                100,                    /* 1x play speed */
                                CatVdoplyPlayFinishCallback);    /* hook callback */
            }

            if (video_ret < 0)
            {
                cat_vdoply_play_finish_callback(video_ret);

                if (cat_vdoply_play_finish_callback != NULL)
                {
                    cat_vdoply_play_finish_callback(video_ret);
                }
            }
        }
    }
    else
    {
        /* something wrong */

        /* call callback function */
        if (cat_vdoply_play_finish_callback != NULL);
        {
            cat_vdoply_play_finish_callback(result);
        }
    }

}


/*****************************************************************************
 * FUNCTION
 *  CatVdoplyVolUp
 * DESCRIPTION
 *  volume down key press hdlr
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void CatVdoplyVolUp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (cat_vdoply_audio_volumn < LEVEL7)
    {
        cat_vdoply_audio_volumn++;
        mdi_audio_set_volume(MDI_VOLUME_MEDIA, cat_vdoply_audio_volumn);
    }
}


/*****************************************************************************
 * FUNCTION
 *  CatVdoplyVolDown
 * DESCRIPTION
 *  volume down key press hdlr
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void CatVdoplyVolDown(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (cat_vdoply_audio_volumn > LEVEL1)
    {
        cat_vdoply_audio_volumn--;
        mdi_audio_set_volume(MDI_VOLUME_MEDIA, cat_vdoply_audio_volumn);
    }
}
#endif /* __MMI_VIDEO_PLAYER__ */ 

extern void cat33_hide_lsk(void);
extern void cat33_hide_rsk(void);
PU8 cat226_bg_img = NULL;


/*****************************************************************************
 * FUNCTION
 *  cat226_hide_lsk
 * DESCRIPTION
 *  hide category226 left softkey in normal mode
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat226_hide_lsk(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x, y, width, height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    wgui_softkey_get_dimension(MMI_LEFT_SOFTKEY, &width, &height);
    wgui_softkey_get_position(MMI_LEFT_SOFTKEY, &x, &y);
    gdi_layer_set_clip(x, y, x + width - 1, y + height - 1);
    gdi_image_draw(0, 0, (U8*) cat226_bg_img);
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  cat226_hide_rsk
 * DESCRIPTION
 *  hide category226 right softkey in normal mode
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat226_hide_rsk(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x, y, width, height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    wgui_softkey_get_dimension(MMI_RIGHT_SOFTKEY, &width, &height);
    wgui_softkey_get_position(MMI_RIGHT_SOFTKEY, &x, &y);
    gdi_layer_set_clip(x, y, x + width - 1, y + height - 1);
    gdi_image_draw(0, 0, (U8*) cat226_bg_img);
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  cat226_hide_button
 * DESCRIPTION
 *  
 * PARAMETERS
 *  button_object       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void cat226_hide_button(void *button_object)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    button *button_obj = (button*) button_object;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(
        button_obj->x,
        button_obj->y,
        button_obj->x + button_obj->width + 1,
        button_obj->y + button_obj->height + 1);
    gdi_image_draw(0, 0, (U8*) cat226_bg_img);
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  cat226_hide_button_full_screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  button_object       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void cat226_hide_button_full_screen(void *button_object)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    button *button_obj = (button*) button_object;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(
        button_obj->x,
        button_obj->y,
        button_obj->x + button_obj->width + 1,
        button_obj->y + button_obj->height + 1);
    gdi_draw_solid_rect(
        button_obj->x,
        button_obj->y,
        button_obj->x + button_obj->width + 1,
        button_obj->y + button_obj->height + 1,
        GDI_COLOR_TRANSPARENT);
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  cat226_draw_scroll_text_cb
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void cat226_draw_scroll_text_cb(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(x1, y1, x2, y2);
    gdi_image_draw(0, 0, (U8*) cat226_bg_img);
    gdi_layer_pop_clip();

}


/*****************************************************************************
 * FUNCTION
 *  cat226_draw_scroll_text_cb_full_screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void cat226_draw_scroll_text_cb_full_screen(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(x1, y1, x2, y2);
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
    gdi_layer_pop_clip();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory226Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory226Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    enable_softkey_background();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory226Screen
 * DESCRIPTION
 *  Category screen for image viewer
 *  one preview area, one image view area.
 * PARAMETERS
 *  left_softkey            [IN]            Left softkey string id
 *  left_softkey_icon       [IN]            Left softkey icon id
 *  right_softkey           [IN]            Right softkey string id
 *  right_softkey_icon      [IN]            Right softkey icon id
 *  bg_image_id             [IN]            Background image id
 *  str_1                   [IN]            String 1 for display ( filename )
 *  str_2                   [IN]            String 2 for display ( file count )
 *  left_arrow_up           [IN]            Up image id for left arrow
 *  left_arrow_down         [IN]            Down image id for left arrow
 *  event_callback1         [IN]            Event handler for left arrow
 *  right_arrow_up          [IN]            Up image id for right arrow
 *  right_arrow_down        [IN]            Down image id for right arrow
 *  event_callback2         [IN]            Event handler for right arrow
 *  icon1_up                [IN]            Up image id for icon1
 *  icon1_down              [IN]            
 *  event_callback3         [IN]            Event handler for icon1
 *  icon2_up                [IN]            Up image id for icon2
 *  icon2_down              [IN]            Down image id for icon2
 *  event_callback4         [IN]            Event handler for icon2
 *  icon3_up                [IN]            Up image id for icon3
 *  icon3_down              [IN]            Down image id for icon3
 *  event_callback5         [IN]            Event handler for icon3
 *  icon4_up                [IN]            Up image id for icon4
 *  icon4_down              [IN]            Down image id for icon4
 *  event_callback6         [IN]            Event handler for icon4
 *  area1_layer             [IN/OUT]        Layer handle for preview area
 *  area1_call_back         [IN]            Draw function of preview area
 *  area2_layer             [IN/OUT]        Layer handle for image content area
 *  area2_call_back         [IN]            Draw function of image content area
 *  layout                  [IN]            Normal mode / full screen mode
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory226Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 bg_image_id,
        UI_string_type str_1,
        UI_string_type str_2,
        U16 left_arrow_up,
        U16 left_arrow_down,
        FuncPtr event_callback1,
        U16 right_arrow_up,
        U16 right_arrow_down,
        FuncPtr event_callback2,
        U16 icon1_up,
        U16 icon1_down,
        FuncPtr event_callback3,
        U16 icon2_up,
        U16 icon2_down,
        FuncPtr event_callback4,
        U16 icon3_up,
        U16 icon3_down,
        FuncPtr event_callback5,
        U16 icon4_up,
        U16 icon4_down,
        FuncPtr event_callback6,
        GDI_HANDLE *area1_layer,
        void (*area1_call_back) (dm_coordinates *coordinate),
        GDI_HANDLE *area2_layer,
        void (*area2_call_back) (dm_coordinates *coordinate),
        cat226_screen_layout_enum layout)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    U8 index = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    entry_full_screen();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    disable_softkey_background();
    if (layout == VERTICAL_FULLSCREEN || layout == HORIZONTAL_FULLSCREEN)
    {
        register_hide_left_softkey(cat33_hide_lsk);
        register_hide_right_softkey(cat33_hide_rsk);
    }
    else
    {
        register_hide_left_softkey(cat226_hide_lsk);
        register_hide_right_softkey(cat226_hide_rsk);
    }

    cat226_bg_img = get_image(bg_image_id);
    dm_add_image(cat226_bg_img, NULL, NULL);

    /* dm_add_string(str_1, &MMI_default_font, UI_COLOR_WHITE, UI_COLOR_BLACK, NULL); */
    if (layout == VERTICAL_NORMAL || layout == HORIZONTAL_NORMAL)
    {
        dm_add_scroll_text(str_1, NULL, cat226_draw_scroll_text_cb, UI_COLOR_BLACK, UI_COLOR_WHITE);
        dm_add_button(NULL, get_image(left_arrow_up), get_image(left_arrow_down), cat226_hide_button);
        dm_add_button(NULL, get_image(right_arrow_up), get_image(right_arrow_down), cat226_hide_button);
        dm_add_button(NULL, get_image(icon1_up), get_image(icon1_down), cat226_hide_button);
        dm_add_button(NULL, get_image(icon2_up), get_image(icon2_down), cat226_hide_button);
        dm_add_button(NULL, get_image(icon3_up), get_image(icon3_down), cat226_hide_button);
        dm_add_button(NULL, get_image(icon4_up), get_image(icon4_down), cat226_hide_button);
    }
    else
    {
        dm_add_scroll_text(str_1, NULL, cat226_draw_scroll_text_cb_full_screen, UI_COLOR_BLACK, UI_COLOR_WHITE);
        dm_add_button(NULL, get_image(left_arrow_up), get_image(left_arrow_down), cat226_hide_button_full_screen);
        dm_add_button(NULL, get_image(right_arrow_up), get_image(right_arrow_down), cat226_hide_button_full_screen);
        dm_add_button(NULL, get_image(icon1_up), get_image(icon1_down), cat226_hide_button_full_screen);
        dm_add_button(NULL, get_image(icon2_up), get_image(icon2_down), cat226_hide_button_full_screen);
        dm_add_button(NULL, get_image(icon3_up), get_image(icon3_down), cat226_hide_button_full_screen);
        dm_add_button(NULL, get_image(icon4_up), get_image(icon4_down), cat226_hide_button_full_screen);
    }
    dm_add_string(str_2, &MMI_default_font, UI_COLOR_BLACK, UI_COLOR_WHITE, NULL);

    dm_register_button_functions(index, KEY_EVENT_UP, event_callback1);
    dm_register_button_functions(index++, KEY_EVENT_REPEAT, event_callback1);
    dm_register_button_functions(index, KEY_EVENT_UP, event_callback2);
    dm_register_button_functions(index++, KEY_EVENT_REPEAT, event_callback2);
    dm_register_button_functions(index++, KEY_EVENT_UP, event_callback3);
    dm_register_button_functions(index++, KEY_EVENT_UP, event_callback4);
    dm_register_button_functions(index, KEY_EVENT_UP, event_callback5);
    dm_register_button_functions(index++, KEY_EVENT_REPEAT, event_callback5);
    dm_register_button_functions(index, KEY_EVENT_UP, event_callback6);
    dm_register_button_functions(index++, KEY_EVENT_REPEAT, event_callback6);

    ExitCategoryFunction = ExitCategory226Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32CatId = MMI_CATEGORY226_ID1 + layout;
    dm_data.S32ScrId = GetActiveScreenId();
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND | DM_NO_BLT;
    dm_setup_data(&dm_data);

    dm_register_category_controlled_callback(area1_call_back);
    dm_register_category_controlled2_callback(area2_call_back);

    dm_redraw_category_screen();

    if (layout == VERTICAL_NORMAL || layout == HORIZONTAL_NORMAL)
    {
        *area1_layer = dm_get_layer_handle(0);
        *area2_layer = dm_get_layer_handle(1);
    }
    else
    {
        *area1_layer = GDI_ERROR_HANDLE;
        *area2_layer = dm_get_layer_handle(0);
    }
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  Cat226RegisterPenHandlers
 * DESCRIPTION
 *  Register pen event handlers for category control areas in category226
 * PARAMETERS
 *  ctrl_area_idx       [IN]        
 *  pen_hdlr            [IN]        
 *  event_type          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Cat226RegisterPenHandlers(U16 ctrl_area_idx, mmi_pen_handler pen_hdlr, mmi_pen_event_type_enum event_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ctrl_area_idx == 1)
    {
        wgui_register_category_screen_control_area_pen_handlers(pen_hdlr, event_type);
    }
    else if (ctrl_area_idx == 0)
    {
        wgui_register_category_screen_control_area2_pen_handlers(pen_hdlr, event_type);
    }
    else
    {
        MMI_ASSERT(ctrl_area_idx);
    }
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory227Screen
 * DESCRIPTION
 *  Category screen for image viewer
 *  one preview area, one image view area.
 * PARAMETERS
 *  title_string            [IN]        Title string
 *  title_icon              [IN]        Title icon
 *  left_softkey            [IN]        Left softkey string id
 *  left_softkey_icon       [IN]        Left softkey icon id
 *  right_softkey           [IN]        Right softkey string id
 *  right_softkey_icon      [IN]        Right softkey icon id
 *  display_call_back       [IN]        Event handler application customized area
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory227Screen(
        U8 *title_string,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        void (*display_call_back) (dm_coordinates *coordinate))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_string = (UI_string_type) title_string;
    MMI_title_icon = get_image(title_icon);
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    MMI_menu_shortcut_number = -1;

    ExitCategoryFunction = UI_dummy_function;
    GetCategoryHistorySize = dummy_get_history_size;
    GetCategoryHistory = dummy_get_history;
    RedrawCategoryFunction = dm_redraw_category_screen;

    dm_data.S32CatId = MMI_CATEGORY227_ID;
    dm_data.S32ScrId = GetActiveScreenId();
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(display_call_back);

    dm_redraw_category_screen();

}

#ifdef __MMI_SWFLASH__

/*****************************************************************************
 * FUNCTION
 *  SetSWFlashPlayerLayer
 * DESCRIPTION
 *  set swflash player layer
 * PARAMETERS
 *  layer_handle      [IN]        layer handle
 * RETURNS
 *  void
 *****************************************************************************/
void SetSWFlashPlayerLayer(GDI_HANDLE layer_handle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
 
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cat_swflash_layer = layer_handle;
}


/*****************************************************************************
 * FUNCTION
 *  CatSWFlashPlayEnd
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void CatSWFlashPlayEnd(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (cat_swflash_is_open == MMI_FALSE || cat_swflash_is_play == MMI_FALSE)
    {
        /* open file failed or play failed - reutrn error cause */
        if (cat_swflash_play_finish_callback != NULL)
        {
            cat_swflash_play_finish_callback(-1);
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  DrawCateSWFlashCategoryControlArea
 * DESCRIPTION
 *  draw vdo contro area
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coord
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCateSWFlashCategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 content_width, content_height;
    S32 content_offset_x, content_offset_y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!cat_swflash_is_full_screen)
    {
        content_width = MMI_content_width;
        content_height = MMI_content_height;
        content_offset_x = MMI_content_x;
        content_offset_y = MMI_content_y;
    }
    else
    {
        content_width = UI_device_width;
        content_height = UI_device_height;
        content_offset_x = 0;
        content_offset_y = 0;
    }

   /********************************************************************************
   *  Create Video Layer 
   ********************************************************************************/
    /* video is successfully opened, create video playback layer */
    if (cat_swflash_is_open)
    {
        if (cat_swflash_layer == GDI_ERROR_HANDLE || cat_swflash_layer == GDI_LAYER_EMPTY_HANDLE)
        {/* the swflash layer maybe already set */
            if (wgui_is_touch_title_bar_buttons())
            {
                dm_create_layer(
                    content_offset_x,
                    content_offset_y,
                    content_width,
                    content_height,
                    &cat_swflash_layer,
                    DM_LAYER_ACTIVE_TOP);
            }
            else
            {
                dm_create_layer(
                    content_offset_x,
                    content_offset_y,
                    content_width,
                    content_height,
                    &cat_swflash_layer,
                    DM_LAYER_ACTIVE_BOTTOM);
            }
        }

        /* clear this layer with black */
        gdi_layer_push_and_set_active(cat_swflash_layer);
        gdi_layer_clear(GDI_COLOR_BLACK);
        gdi_layer_pop_and_restore_active();

        /* Get first frame and start to play */
        cat_swflash_play_ret = mdi_swflash_play(
                                cat_swflash_layer,
                                1,                      /* repeat count */
                                MDI_DEVICE_SPEAKER2,    /* audio_path */
                                cat_swflash_play_finish_callback);     /* play_result_callback */

        cat_swflash_is_play = (cat_swflash_play_ret >= 0) ? MMI_TRUE : MMI_FALSE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory229Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title_str                   [?]         
 *  title_icon                  [IN]        
 *  lsk_str                     [IN]        
 *  lsk_icon                    [IN]        
 *  rsk_str                     [IN]        
 *  rsk_icon                    [IN]        
 *  resource_id                 [IN]        
 *  resource_filename           [IN]        
 *  repeat_count                [IN]        
 *  is_play_audio               [IN]        
 *  is_vibrate_on               [IN]        
 *  is_interaction_on           [IN]        
 *  is_lcd_no_sleep             [IN]        
 *  is_full_screen              [IN]        
 *  bg_color                    [IN]        
 *  play_finish_callback        [IN]        
 *  gui_buffer                  [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory229Screen(
        U8 *title_str,
        U16 title_icon,
        U16 lsk_str,
        U16 lsk_icon,
        U16 rsk_str,
        U16 rsk_icon,
        U16 resource_id,
        PS8 resource_filename,
        U16 repeat_count,
        MMI_BOOL is_play_audio,
        MMI_BOOL is_vibrate_on,
        MMI_BOOL is_interaction_on,
        MMI_BOOL is_lcd_no_sleep,
        MMI_BOOL is_full_screen,
        GDI_COLOR bg_color,
        void (*play_finish_callback) (MDI_RESULT),
        void *gui_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    U16 btn_prev_item_id, btn_next_item_id;
    U32 playmode;
    MDI_RESULT mfh_ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* init var */
    cat_swflash_is_from_id = (resource_id != 0) ? MMI_TRUE : MMI_FALSE;
    cat_swflash_is_audio_on = is_play_audio;
    cat_swflash_is_vibrate_on = is_vibrate_on;
    cat_swflash_is_interaction_on = is_interaction_on;
    cat_swflash_is_lcd_no_sleep = is_lcd_no_sleep;
    cat_swflash_play_finish_callback = play_finish_callback;
    cat_swflash_is_full_screen = is_full_screen;

    /* setup keys */
    set_left_softkey_label(get_string(lsk_str));
    set_left_softkey_icon(get_image(lsk_icon));
    set_right_softkey_label(get_string(rsk_str));
    set_right_softkey_icon(get_image(rsk_icon));
    SetupCategoryKeyHandlers();

    /* setup title */
    MMI_title_string = (UI_string_type) title_str;
    MMI_title_icon = NULL;
    MMI_menu_shortcut_number = -1;

    /* setup top selection bar */
    if (wgui_is_touch_title_bar_buttons())
    {
        wgui_set_wallpaper_on_bottom(MMI_TRUE);
        
        btn_prev_item_id = dm_add_button(
                            NULL,
                            get_image(LEFT_RED_ARROW),
                            get_image(LEFT_RED_ARROW),
                            cat226_hide_button_full_screen);
        btn_next_item_id = dm_add_button(
                            NULL,
                            get_image(RIGHT_RED_ARROW),
                            get_image(RIGHT_RED_ARROW),
                            cat226_hide_button_full_screen);
        title_bg_id = IMAGE_VIEW_TITLEBAR_BG;   /* for showing different title bar. */

    #if defined(__MMI_TOUCH_SCREEN__)
        /* register the handler for pen event down istead of pen event up. */
        dm_register_button_functions(btn_prev_item_id, KEY_EVENT_UP, gMMI_touch_title_button1_down_handler);
        dm_register_button_functions(btn_next_item_id, KEY_EVENT_UP, gMMI_touch_title_button2_down_handler);
    #endif /* defined(__MMI_TOUCH_SCREEN__) */ 

    }

    /* Start SWFlash handling */

    /* stop MMI sleep */
    if (cat_swflash_is_lcd_no_sleep)
    {
        TurnOnBacklight(0);
    }

    /* set audio volumn */
    if (gui_buffer == NULL)
    {
        /* first time enter */
        cat_swflash_audio_volumn = LEVEL5;
    }

    if (cat_swflash_is_audio_on)
    {
        /* init audio volumn */
        mdi_audio_set_volume(MDI_VOLUME_MEDIA, cat_swflash_audio_volumn);

        /* full screen play do not allows vol adjustment */
        if (!is_full_screen)
        {
            SetKeyHandler(CatVdoplyVolUp, KEY_VOL_UP, KEY_EVENT_DOWN);
            SetKeyHandler(CatVdoplyVolDown, KEY_VOL_DOWN, KEY_EVENT_DOWN);
        }

        /* suspend background audio */
        mdi_audio_suspend_background_play();
    }

    /*
     * user may request to repeat play many times, and wants a callback bewteen each play,
     * so we have to handle repeat play within this category by seeking to first frame 
     * after play finished each time, instead of using driver's repeat play.
     */

    cat_swflash_repeat_count = (repeat_count == 0) ? 0xffff : repeat_count;      /* 0xff means infinte play */

    if (repeat_count != 0)
    {
        cat_swflash_repeat_count--;
    }


   /********************************************************************************
   *  Open SWFlash
   ********************************************************************************/
#if 0
#endif 
    if (is_play_audio)
    {
        playmode = CIM_PLAYERMODE;
    }
    else
    {
        playmode = CIM_DESKTOPMODE;
    }


    if (resource_id != 0)
    {
        /* source from id */
        /* always open with audio, if do not want audio, mute it by calling mdi function */
        mfh_ret = mdi_swflash_open_id(resource_id, playmode, &wgui_swflash_info);

        cat_swflash_is_open = (mfh_ret >= 0) ? MMI_TRUE : MMI_FALSE;
    }
    else if (resource_filename != NULL)
    {
        /* source from file */
        /* always open with audio, if do not want audio, dsiable it in play function */
        mfh_ret = mdi_swflash_open_file(resource_filename, playmode, &wgui_swflash_info);

        cat_swflash_is_open = (mfh_ret >= 0) ? MMI_TRUE : MMI_FALSE;
    }
    else
    {
        /* no proper source */
        MMI_ASSERT(0);
    }

    /* setup draw manager functions */
    ExitCategoryFunction = ExitCategorySWFlashScreen;
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    if (wgui_is_touch_title_bar_buttons())
    {
        dm_data.S32CatId = MMI_CATEGORY229_ID;
    }
    else
    {
        dm_data.S32CatId = MMI_CATEGORY_VDOPLY;
    }

    dm_data.S32ScrId = (S32) GetActiveScreenId();
    if (is_full_screen)
    {
        if (wgui_is_touch_title_bar_buttons())
        {
            dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
        }
        else
        {
            dm_data.S32flags = DM_NO_TITLE | DM_NO_STATUS_BAR | DM_NO_SOFTKEY;
        }
    }
    else
    {
        dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    }

    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawCateSWFlashCategoryControlArea);
    dm_redraw_category_screen();

    CatSWFlashPlayEnd();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategorySWFlashScreen
 * DESCRIPTION
 *  Exit Category 224 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategorySWFlashScreen(void)
    {
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (cat_swflash_is_open)
    {
        /* stop video playing */
        if (cat_swflash_is_play)
        {
            mdi_swflash_stop();
            cat_swflash_is_play = MMI_FALSE;
        }

        /* close video source */
        if (cat_swflash_is_from_id)
        {
            /* source from id */
            mdi_swflash_close_id();
            cat_swflash_is_open = MMI_FALSE;
        }
        else
        {
            /* source from file */
            mdi_swflash_close_file();
            cat_swflash_is_open = MMI_FALSE;
        }
    }

    cat_swflash_is_open = MMI_FALSE;
    cat_swflash_is_play = MMI_FALSE;
    cat_swflash_is_from_id = MMI_FALSE;
    cat_swflash_layer = GDI_ERROR_HANDLE;
    cat_swflash_play_finish_callback = NULL;
    cat_swflash_play_ret = 0;

    /* let MMI can sleep */
    if (cat_swflash_is_lcd_no_sleep)
    {
        TurnOffBacklight();
    }

    if (cat_swflash_is_audio_on)
    {
        /* resume background audio */
        mdi_audio_resume_background_play();
    }

    if (wgui_is_touch_title_bar_buttons())
    {
        title_bg_id = 0;

        if (gBlack_buf != NULL)
        {
            mmi_frm_scrmem_free(gBlack_buf);
        }

        gBlack_buf = NULL;
        gBlack_layer = NULL;
        entry_full_screen();
    }

    wgui_reset_touch_title_bar_buttons();

}

/*****************************************************************************
 * FUNCTION
 *  GetCat34MediaType
 * DESCRIPTION
 *  Get category34 background media type
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
S32 GetCat34MediaType(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
 
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_mmi_idle_bg_media_type;
}

/*****************************************************************************
 * FUNCTION
 *  SetCat34MediaType
 * DESCRIPTION
 *  Get category34 background media type
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
void SetCat34MediaType(S32 media_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
 
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_idle_bg_media_type = media_type;
}


/*****************************************************************************
 * FUNCTION
 *  SetupCat34Media
 * DESCRIPTION
 *  Setup category34 background media type
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
mdi_result SetupCat34Media(U16 bg_media_id, S8 *bg_media_filename)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mdi_result mfh_ret = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if (dispchar_iswp_swflash() == MMI_TRUE)
    {
        g_mmi_idle_bg_media_type = MMI_IDLE_BG_MEDIA_SWFLASH;
    }
    else
    {
        g_mmi_idle_bg_media_type = MMI_IDLE_BG_MEDIA_IMG;
    }

    if (g_mmi_idle_bg_media_type == MMI_IDLE_BG_MEDIA_SWFLASH)
    {

        if(bg_media_id == 0xFFFF && bg_media_filename == NULL)
        {
            bg_media_id = PhnsetGetCurWallPaperIdx();
            bg_media_filename = idle_screen_wallpaper_name;
        }
    
        /********************************************************************************
        *  SWFlash Setting
        ********************************************************************************/
        cat_swflash_is_full_screen = MMI_TRUE;
        cat_swflash_repeat_count = 1;
        cat_swflash_audio_volumn = LEVEL5;
        cat_swflash_is_audio_on = MMI_FALSE;
        cat_swflash_is_vibrate_on = MMI_FALSE;
        cat_swflash_is_interaction_on = MMI_FALSE;
        cat_swflash_is_lcd_no_sleep = MMI_FALSE;

        /********************************************************************************
        *  Open SWFlash
        ********************************************************************************/
        
        if (bg_media_id != 0)
        {
            cat_swflash_is_from_id = MMI_TRUE;
            /* source from id */
            /* always open with audio, if do not want audio, mute it by calling mdi function */
            mfh_ret = mdi_swflash_open_id(bg_media_id, CIM_DESKTOPMODE, &wgui_swflash_info);

            cat_swflash_is_open = (mfh_ret >= 0) ? MMI_TRUE : MMI_FALSE;
        }
        else if (bg_media_filename!= NULL)
        {
            cat_swflash_is_from_id = MMI_FALSE;
            /* source from file */
            /* always open with audio, if do not want audio, dsiable it in play function */
            mfh_ret = mdi_swflash_open_file(bg_media_filename, CIM_DESKTOPMODE, &wgui_swflash_info);

            cat_swflash_is_open = (mfh_ret >= 0) ? MMI_TRUE : MMI_FALSE;
        }
        else
        {
         /* no proper source */
         MMI_ASSERT(0);
        }
    }
    else /* g_mmi_idle_bg_media_type == MMI_IDLE_BG_MEDIA_SWFLASH */
    {
        return 0;
    }
    return mfh_ret;
}

/*****************************************************************************
 * FUNCTION
 *  HandleCat34MediaPlay
 * DESCRIPTION
 *  Handle media play error
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HandleCat34MediaPlayFailed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if ( g_mmi_idle_bg_media_type == MMI_IDLE_BG_MEDIA_SWFLASH)
    {
        if (cat_swflash_is_open == MMI_FALSE || cat_swflash_play_ret < 0)
        {
                PhnsetWallpaperBadFileCallBack(0);
                g_mmi_idle_bg_media_type = MMI_IDLE_BG_MEDIA_IMG;
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  ShowCategory34Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  left_softkey            [IN]
 *  left_softkey_icon       [IN]
 *  right_softkey           [IN]
 *  right_softkey_icon      [IN]
 *  bg_media_id             [IN]
 *  bg_media_filename       [IN]
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory34Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 bg_media_id,
        S8 *bg_media_filename,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (dispchar_iswp_swflash() == MMI_TRUE)
    {
        g_mmi_idle_bg_media_type = MMI_IDLE_BG_MEDIA_SWFLASH;
    }
    else
    {
        g_mmi_idle_bg_media_type = MMI_IDLE_BG_MEDIA_IMG;
    }

    /********************************************************************************
    *  IdleScreen
    ********************************************************************************/
    ShowCategory33Screen_ext(
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        history_buffer);

    /********************************************************************************
    *  SWFlash Setting
    ********************************************************************************/
    SetupCat34Media(bg_media_id, bg_media_filename);

    ExitCategoryFunction = ExitCategory34Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCategoryFunction = dm_redraw_category_screen;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    if ( g_mmi_idle_bg_media_type == MMI_IDLE_BG_MEDIA_IMG )
    {
        dm_data.S32CatId = MMI_CATEGORY33_ID;
        dm_register_category_controlled_callback(NULL);
    }
    else if ( g_mmi_idle_bg_media_type == MMI_IDLE_BG_MEDIA_SWFLASH )
    {
        dm_data.S32CatId = MMI_CATEGORY34_ID;
        dm_register_category_controlled_callback(DrawCateSWFlashCategoryControlArea);
    }
    dm_data.S32flags = 0;
#ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__
    if (mmi_bootup_get_active_flight_mode() == 0)
    {
        dm_data.S32flags |= DM_NO_SOFTKEY;
    }
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    dm_data.S32flags |= DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_register_category_controlled2_callback(dm_category_33_controlled_area);
    dm_redraw_category_screen();

    HandleCat34MediaPlayFailed();
}

void ExitCategory34Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ( g_mmi_idle_bg_media_type == MMI_IDLE_BG_MEDIA_SWFLASH )
    {
        ExitCategorySWFlashScreen();
    }
    ExitCategory33Screen();
}

#endif /* __MMI_SWFLASH__ */ 
/****************************************************************************
*
* MultiMedia Related [SubLCD] Categories
*
*****************************************************************************/

/*****************************************************************************
* [Category331] [SUB]
*
* This category is a empty SubLCD category.
*
*  ****************
*  *              *
*  *              *
*  *              *
*  *              *
*  *              *
*  ****************
*
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowCategory331Screen
 * DESCRIPTION
 *  Enter Category 331 Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory331Screen(void)
{
#ifdef __MMI_SUBLCD__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;
    SUBLCD_ExitCategoryFunction();

    hide_status_icon_bar(2);

    SUBLCD_ExitCategoryFunction = ExitCategory331Screen;
    SUBLCD_RedrawCategoryFunction = MMI_dummy_function;
    SUBLCD_GetCategoryHistory = dummy_get_history;
    SUBLCD_GetCategoryHistorySize = dummy_get_history_size;
    GDI_UNLOCK;
#endif /* __MMI_SUBLCD__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory331Screen
 * DESCRIPTION
 *  Exit Category 331 Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_SUBLCD__
static void ExitCategory331Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SUBLCD_ExitCategoryFunction = MMI_dummy_function;
    SUBLCD_RedrawCategoryFunction = MMI_dummy_function;
    SUBLCD_GetCategoryHistory = dummy_get_history;
    SUBLCD_GetCategoryHistorySize = dummy_get_history_size;
}
#endif /* __MMI_SUBLCD__ */ 

/*****************************************************************************
* [Category332] [SUB]
*
* This category is to draw a image on sublcd
*
*  ****************
*  *              *
*  *              *
*  *  [ Image ]   *
*  *              *
*  *              *
*  ****************
*
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowCategory332Screen
 * DESCRIPTION
 *  Enter Category 332 Screen
 * PARAMETERS
 *  image_id        [IN]        Image id
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory332Screen(UI_image_ID_type  image_id)
{
#ifdef __MMI_SUBLCD__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 image_width;
    S32 image_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;
    SUBLCD_ExitCategoryFunction();

    UI_set_sub_LCD_graphics_context();

    hide_status_icon_bar(2);

    gdi_layer_reset_clip();
    gdi_layer_clear(GDI_COLOR_WHITE);

    if (image_id != 0)
    {
        gdi_image_get_dimension_id(image_id, &image_width, &image_height);
        gdi_image_draw_id((UI_device_width - image_width) >> 1, (UI_device_height - image_height) >> 1, image_id);
    }

    gdi_layer_blt_base_layer(0, 0, UI_device_width - 1, UI_device_height - 1);

    SUBLCD_ExitCategoryFunction = ExitCategory332Screen;
    SUBLCD_RedrawCategoryFunction = MMI_dummy_function;
    SUBLCD_GetCategoryHistory = dummy_get_history;
    SUBLCD_GetCategoryHistorySize = dummy_get_history_size;

    UI_set_main_LCD_graphics_context();
    GDI_UNLOCK;
#endif /* __MMI_SUBLCD__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory332Screen
 * DESCRIPTION
 *  Exit Category 332 Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_SUBLCD__
static void ExitCategory332Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SUBLCD_ExitCategoryFunction = MMI_dummy_function;
    SUBLCD_RedrawCategoryFunction = MMI_dummy_function;
    SUBLCD_GetCategoryHistory = dummy_get_history;
    SUBLCD_GetCategoryHistorySize = dummy_get_history_size;
}
#endif /* __MMI_SUBLCD__ */ 

/*****************************************************************************
* [Category333] [SUB]
*
* This category is for audio player sub display.
*
*  ****************
*  *              *
*  *              *
*  *  XXXXXXXXX   *
*  *              *
*  *              *
*  ****************
*
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowCategory333Screen
 * DESCRIPTION
 *  Enter Category 333 Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory333Screen(void)
{
#ifdef __MMI_SUBLCD__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;
    SUBLCD_ExitCategoryFunction();
    UI_set_sub_LCD_graphics_context();

    gui_lock_double_buffer();

    register_hide_status_icon_bar(2, SUBLCD_screens_hide_status_icons);
    show_status_icon_bar(2);

    gui_unlock_double_buffer();

    SUBLCD_ExitCategoryFunction = ExitCategory333Screen;
    SUBLCD_RedrawCategoryFunction = RedrawCategory333Screen;
    SUBLCD_GetCategoryHistory = GetCategory333History;
    SUBLCD_GetCategoryHistorySize = GetCategory333HistorySize;
    RedrawCategory333Screen();
    UI_set_main_LCD_graphics_context();
    GDI_UNLOCK;
#endif /* __MMI_SUBLCD__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory333Screen
 * DESCRIPTION
 *  Exit Category 333 Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_SUBLCD__
static void ExitCategory333Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_set_sub_LCD_graphics_context();
    close_status_icons();
#ifdef __MMI_AUDIO_PLAYER__
    mmi_audply_exit_subLCD();
#endif 
#ifdef __MMI_FM_RADIO__
    mmi_fmrdo_exit_subLCD();
#endif 
    gui_hide_animations();

    SUBLCD_ExitCategoryFunction = MMI_dummy_function;
    SUBLCD_RedrawCategoryFunction = MMI_dummy_function;
    SUBLCD_GetCategoryHistory = dummy_get_history;
    SUBLCD_GetCategoryHistorySize = dummy_get_history_size;
    UI_set_main_LCD_graphics_context();
}
#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory333Screen
 * DESCRIPTION
 *  Redraw Category 333 Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_SUBLCD__
static void RedrawCategory333Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = gui_color(0, 0, 0);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_set_sub_LCD_graphics_context();

    gui_lock_double_buffer();

#ifndef __MMI_SUB_WALLPAPER__
    show_sublcd_status_background(0, 0, (UI_device_width - 1), (UI_device_height - 1), MMI_FALSE);
#endif 
    SUBLCD_screens_show_status_icons();
    gui_draw_horizontal_line(0, UI_device_width - 1, 54, c);
#ifdef __MMI_AUDIO_PLAYER__
    if (mmi_audply_is_playing())
    {
        mmi_audply_redraw_subLCD(TRUE);
    }
#endif /* __MMI_AUDIO_PLAYER__ */ 
#ifdef __MMI_FM_RADIO__
    if (mmi_fmrdo_is_power_on())
    {
        mmi_fmrdo_redraw_subLCD(TRUE);
    }
#endif /* __MMI_FM_RADIO__ */ 

    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);

    UI_set_main_LCD_graphics_context();
}
#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  GetCategory333HistorySize
 * DESCRIPTION
 *  Get Category 333 History Size
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_SUBLCD__
static S32 GetCategory333HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (0);
}
#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  GetCategory333History
 * DESCRIPTION
 *  *   Get Category 333 History
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_SUBLCD__
static U8 *GetCategory333History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (history_buffer);
}
#endif /* __MMI_SUBLCD__ */ 

