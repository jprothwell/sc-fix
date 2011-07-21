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
 * Filename:
 * ---------
 *   wgui_categories_idlescreen.c
 * Project:
 * --------
 
 * Description:
 * ------------
 * Idle Screen related categories.
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globaldefs.h"
#include "gui_data_types.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "eventsdef.h"
#include "gui_themes.h"
#include "wgui.h"
#include "custthemesres.h"
#include "wgui_inputs.h"
#include "wgui_categories.h"
#include "wgui_softkeys.h"
#include "wgui_status_icons.h"
#include "wgui_categories_mm.h"
#include "wgui_categories_idlescreen.h"
#include "wgui_categories_multimedia.h"
#include "idleappprot.h"
#include "idleappdef.h"
#include "settingdefs.h"
#include "phonesetup.h"
#ifdef __MMI_AUDIO_PLAYER__
/* dependent headers for AudioPlayerXXX.h" */
#include "gui_data_types.h"
#include "frameworkstruct.h"
#include "globalconstants.h"
#include "mdi_datatype.h"
#include "kal_release.h"
#include "conversions.h"
#include "filemgr.h"

#include "audioplayerdef.h"
#include "audioplayertype.h"
#endif /* __MMI_AUDIO_PLAYER__ */ 

 
#include "debuginitdef.h"

#include "lcd_sw_rnd.h"         /* required for macro black_backlight, white_backlight, blue_backlight macro */

#include "gdi_include.h"        /* include for graphic lib */

#include "satgprots.h"
#include "wgui_draw_manager.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "quickaccessmenuitemprot.h"
#include "wgui_touch_screen.h"
#endif /* __MMI_TOUCH_SCREEN__ */ 

 
#include "simdetectiongprot.h"
 

#include "fs_type.h"
#include "fs_func.h"

#include "scrmemmgrgprot.h"

#ifdef __DRM_SUPPORT__
#include "drm_gprot.h"//102406 drm check
#endif 

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
#include "PdaStyle.h"
#endif
/*-zhouqin 2011-02-16 modify for q6 320X240 */

#include "screensaverdefs.h"
/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-11 17:21 */
#include "simdetectiongexdcl.h"
#include "simdetectiongprot.h"
#include "profilegprots.h"
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-11 17:21 */
#if defined(__MMI_MAINLCD_220X176__)  //&&defined(__MMI_TOUCH_SCREEN__)
#include "calendardef.h"
#define E71_SHORTCUT_HORIZON_LINE_Y		57
#define E71_SLAVE_NETWORK_NAME_Y	MMI_STATUS_BAR_HEIGHT+((E71_SHORTCUT_HORIZON_LINE_Y-MMI_STATUS_BAR_HEIGHT)>>1)
#define E71_MASTER_NETWORK_NAME_Y	MMI_STATUS_BAR_HEIGHT//(E71_SLAVE_NETWORK_NAME_Y-(E71_SHORTCUT_HORIZON_LINE_Y-E71_SLAVE_NETWORK_NAME_Y))
#define E71_NETWORK_NAME_X 			70
static UI_character_type shrtcut_date_string[64];
#define MMI_SHORTCUT_HORIZON_SWITCH_FEATURE

#endif

#ifdef __MMI_NOKIA_STYLE_N800__
#define MMI_SHORTCUT_HORIZON_SWITCH_FEATURE
#endif
#undef __NEWSIMULATOR

scrolling_text scroll_sat_idleText;

#define NORMAL_IDLE_SCREEN                   0
#define ANALOG_IDLE_SCREEN                   1
#define DIGITAL_IDLE_SCREEN                     2

#define NORMAL_SCREEN_SAVER                     0
#define ANALOG_SCREEN_SAVER                     1
#define DIGITAL_SCREEN_SAVER                 2

#define  VIEW_WALLPAPER_CLOCK                1
#define  VIEW_SCREENSAVER_CLOCK                 2
#define PREVIEW_CLOCK                        3

#ifdef __MMI_ANALOG_CLOCK__
void show_analog_clock(void);
void analog_clock_initialize(void);
#endif /* __MMI_ANALOG_CLOCK__ */ 
void idle_screen_show_network_name(void);
#if defined(__MMI_NOKIA_STYLE_N800__) || defined (__MMI_BLACKBERRY_IDLE_SHORTCUT__) 
#if defined (__MMI_BLACKBERRY_IDLE_SHORTCUT__) 
    #define MAX_IDLE_SCREEN_SHORTCUTS   3
	#define MMI_BLACKBERRY_IDLE_SHRTCUT_IMAGE_Y	(UI_device_height - MMI_button_bar_height - 40)
#else
    #define MAX_IDLE_SCREEN_SHORTCUTS   7
#endif
#define IDLE_TOUCH_SHORTCUT_UNPRESSED   0
#define IDLE_TOUCH_SHORTCUT_PRESSED     1
typedef struct
{
    S32 x;
    S32 y;
    S32 width;
    S32 height;
    S32 state;
    FuncPtr entryfn;
} idle_touch_screen_shortcut;
static idle_touch_screen_shortcut touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS];
void gui_idle_touch_screen_display_pressed_shorcut_item(S32 index);
void gui_idle_touch_screen_display_scroll_shorcut_item(S32 index);
void ExtendIdleKeyRightHandler(void);
void ExtendIdleKeyLeftHandler(void);
void ExtendIdleKey_Left_or_Right_UpHandler(void);
void mmi_nokia_shortcut_entryfn(void);
#define IDLE_SCREEN_SHORTCUT_LINE_COUNTS	3

#if defined (__MMI_BLACKBERRY_IDLE_SHORTCUT__) 
#define MAX_IDLE_SCREEN_HOR_SHORTCUTS	3
#define T800_SHORTCUT_ITEM_STRING_START_X	1
#define T800_SHORTCUT_ITEM_STRING_START_Y	18
#define T800_SHORTCUT_ITEM_STRING_AREA_END_X	70
#define T800_SHORTCUT_ITEM_STRING_AREA_H		14
#else
#define MAX_IDLE_SCREEN_HOR_SHORTCUTS	5
#define T800_SHORTCUT_ITEM_STRING_START_X	2
#define T800_SHORTCUT_ITEM_STRING_START_Y	24
#define T800_SHORTCUT_ITEM_STRING_AREA_END_X	106
#define T800_SHORTCUT_ITEM_STRING_AREA_H		20
#endif

#if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)	
S32 curr_horizon_hilite_index = 1;
S32 curr_vertical_hilite_index = -1;
#else
S32 curr_horizon_hilite_index = 2;
S32 curr_vertical_hilite_index = 0;
#endif
FuncPtr shortcut_entryfn_p = NULL;
#endif
/***************************************************************************/
#ifdef __MMI_FM_RADIO__
BOOL mmi_fmrdo_is_power_on(void);
UI_string_type mmi_fmrdo_get_channel_name_or_freq(void);
scrolling_text scroll_fmrdo_idleText;
#endif /* __MMI_FM_RADIO__ */ 

#ifdef __MMI_AUDIO_PLAYER__
extern audply_struct g_audply;
scrolling_text scroll_audply_idleText;
UI_character_type idle_title[FMGR_MAX_FILE_LEN + 1];
#endif /* __MMI_AUDIO_PLAYER__ */ 

#include "lcd_if.h"
gdi_handle wgui_layer_1 = GDI_LAYER_EMPTY_HANDLE;
gdi_handle wgui_base_layer = GDI_LAYER_EMPTY_HANDLE;

/* store Main lcd width */
extern S32 MAIN_LCD_device_width;

/* store Main LCD height */
extern S32 MAIN_LCD_device_height;

 
extern BOOL r2lMMIFlag;

/* CSD end */

U8 idle_screen_type = NORMAL_IDLE_SCREEN;

static U8 display_network_name = TRUE;

 
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
static U8 gIsWallpaperChange = 0;
#endif 

#ifdef __MMI_IDLE_FULL_SCREEN__
extern U16 on_idle_screen;
#endif 
static S32 gis_on_idlescreen = 0;

#if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
extern GDI_HANDLE animation_handle;
#endif 

#if defined(__MMI_MAINLCD_220X176__)
void display_touch_idle_screen_shortcuts(void);
void setup_touch_idle_screen_shortcuts(void);
#if defined(__MMI_TOUCH_SCREEN__)
void mmi_touch_shortcut_pen_down_hdlr(mmi_pen_point_struct point);
void mmi_touch_shortcut_pen_move_hdlr(mmi_pen_point_struct point);
#if 0
void mmi_touch_shortcut_pen_repeat_hdlr(mmi_pen_point_struct point);
void mmi_touch_shortcut_pen_long_tap_hdlr(mmi_pen_point_struct point);
#endif
void mmi_touch_shortcut_pen_up_hdlr(mmi_pen_point_struct point);
#endif
extern void wgui_pen_execute_CSK_handler(U16 key_type);
#endif
extern S32 UI_get_font_height(UI_font_type f);

/*****************************************************************************
 * FUNCTION
 *  is_on_idlescreen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 is_on_idlescreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gis_on_idlescreen;
}


/*****************************************************************************
 * FUNCTION
 *  set_on_idlescreen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  temp_on_idlescreen      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_on_idlescreen(S32 temp_on_idlescreen)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gis_on_idlescreen = temp_on_idlescreen;
}

 

extern S16 status_icon;

/* Idle screen related wrappers  */
/* store the adress of string which stores idle screen netwrok name */
UI_string_type idle_screen_network_name[MMI_SIM_NUMBER] = {NULL,};

/* store teh address of string which store the netwrok name */
UI_string_type idle_screen_network_status[MMI_SIM_NUMBER] = {NULL,};

/* store the image id of wallpaper */
extern UI_image_ID_type  idle_screen_wallpaper_ID;
/* store the image name of wallpaper */
extern S8 *idle_screen_wallpaper_name_original;
extern S8 *idle_screen_wallpaper_name;
S8 *idle_screen_cache_name;
UI_image_ID_type  idle_screen_cache_id;

#ifdef __MMI_SAVE_SS_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_SS_SPEEDUP__  20091125 */
UI_image_ID_type  idle_screen_screensaver_ID;
extern S8 *idle_screen_screensaver_name;
extern S8 *idle_screen_screensaver_name_original;
#endif
#if (defined(__MMI_SAVE_SS_SPEEDUP__) || defined(__MMI_SAVE_PON_SPEEDUP__)) /*gdm modify 20091125*/
S32 cat166_n_frames = 0;
S32 cat166_frame_number = 0;
extern S32 cat166_image_offset_y;
extern S32 cat166_image_offset_x;
extern S32 cat166_image_width;
extern S32 cat166_image_height;
S32 cat166_animation_delay;
#endif
#ifdef __MMI_SAVE_PON_SPEEDUP__ /*gdm modify to add  the switch of __MMI_SAVE_PON_SPEEDUP__  20091125 */
UI_image_ID_type  idle_screen_poweron_ID;
extern S8 *idle_screen_poweron_name;
extern S8 *idle_screen_poweron_name_original;
#endif

 
#if (defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) && defined(__MMI_MAINLCD_240X320__))
stFontAttribute idle_screen_network_name_font = {1, 0, 0, LARGE_FONT, 0, 0};
#else 
#if defined(__MMI_MAINLCD_220X176__)

stFontAttribute idle_screen_network_name_font = {0, 0, 0, SMALL_FONT, 0, 0};
#else 
stFontAttribute idle_screen_network_name_font = {0, 0, 0, MEDIUM_FONT, 0, 0};
#endif 

#endif 
 
stFontAttribute idle_screen_network_status_font = {0, 0, 0, MEDIUM_FONT, 0, 0};

 
/* store the x position of wallpaer of idle screen */
S32 cat33_image_x;

/* store the y position of wallpaer of idle screen */
S32 cat33_image_y;

/* store the image width  of wallpaer of idle screen */
S32 cat33_image_width;

/* store the image height  of wallpaer of idle screen */
S32 cat33_image_height;

/* store the total number frames  of animation wallpaper */
S32 cat33_n_frames = 0;

/* store the current frame number of animation wallppaer */
S32 cat33_frame_number = 0;

/* store the delay time to show next frame of animation wallpapaer */
S32 cat33_animation_delay = 1000;

/* store the image id ofcategory86 used foe analog clock */
PU8 cat86_image;

/* store x position of image,y position of image ,image widh ,image height */
S32 cat86_image_x,cat86_image_y;//,cat86_image_width,cat86_image_height;//062106 cat128

#ifdef __MMI_LCD_PARTIAL_ON__
static UI_time cat161_time;
static S32 cat161_last_y_offset;
#endif /* __MMI_LCD_PARTIAL_ON__ */ 

 
/* store the type of screen saver like normal or analog/digital screen saver */
U8 screen_saver_type = NORMAL_SCREEN_SAVER;
U8 *ScreenSaverName;
U8 disp_network_screeen_saver = 0;
extern S32 PhnsetScreenSaverBadFileCallBack(void);
extern void PhnsetWallpaperBadFileCallBack(U16 imgID);

extern void dm_set_scr_bg_image_no_draw(U16 image_id, S8 *file_name, S32 x, S32 y, U8 opacity);         

#ifdef __MMI_SCREEN_ROTATE__ // wangmg[3.1]
extern MMI_BOOL mmi_frm_is_screen_width_height_swapped(void);
#endif

S32 cat128_image_x, cat128_image_y;//, cat128_image_width, cat128_image_height;
PU8 cat128_image;

scrolling_text *extra_scrolling_text = NULL;
S16 extra_text_x1 = -1;
S16 extra_text_y1 = -1;
S16 extra_text_x2 = -1;
S16 extra_text_y2 = -1;

 
#if defined (__MMI_WALLPAPER_ON_BOTTOM__)
static UI_image_ID_type  idle_scr_bg_ID;
static S8 *idle_scr_bg_filename;
static S32 idle_scr_bg_x, idle_scr_bg_y;
static U8 idle_scr_bg_opacity;   
#endif /* defined (__MMI_WALLPAPER_ON_BOTTOM__) */ 
 

 
extern U16 title_bg_id;

 

 
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
extern MMI_BOOL PhnsetGetShowDateTimeStatus(void);
UI_image_ID_type  idlescreen_bar_id = IMG_TECHNO_ANALOG_CLOCK;//070306 Alpha layer
#endif 
 

 
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
 
 
#define X_INSERT 20     /* Gap from left and right end for audio. */
#endif /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */ 

#if defined (__MMI_MAINLCD_240X320__)&&(MMI_SIM_NUMBER < 4)
#define IDLESCREEN_TEXT_GAP 10
#else
#define IDLESCREEN_TEXT_GAP 5
#endif 

 
 
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
extern is_status_bar_shuffled;
#endif 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
extern is_button_bar_shuffled;
#endif 
#ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
#define X_INSERT 10     /* Gap from left and right end for audio. */
#endif 
 
extern icontext_button MMI_softkeys[];
extern void cat129_clear_icon_bg(void *);

pBOOL mmi_bootup_is_sim_valid()
{
	U16 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].IsSimValid)
			return MMI_TRUE;
	}
	return MMI_FALSE;
}
pBOOL mmi_bootup_is_in_security_check()
{
	return g_pwr_context[0].PowerOnTillIdleApp;
}
pBOOL mmi_idle_is_on_idle_screen()
{
	return g_idle_context.IsOnIdleScreen;
}
pBOOL mmi_bootup_is_idle_screen_reached()
{
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-3-11 18:17 */
    #if 0
    if (g_pwr_context.PowerOnMMIStatus >= MMI_POWER_ON_IDLE)
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
    #endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-3-11 18:17 */
     return MMI_FALSE;
}
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-11 17:38 */

/**************************************************************************/

/* micha1111, michaSATc3, #212 */


/*****************************************************************************
 * FUNCTION
 *  satIdleTextScrollTimer
 * DESCRIPTION
 *  handle scrolling text of SAT scrrem
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void satIdleTextScrollTimer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_handle_scrolling_text(&scroll_sat_idleText);
}


/*****************************************************************************
 * FUNCTION
 *  audply_idle_text_scroll_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void audply_idle_text_scroll_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if !defined(__MMI_MAINLCD_220X176__)	
#ifdef __MMI_AUDIO_PLAYER__
    gui_handle_scrolling_text(&scroll_audply_idleText);
#endif 
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  fmrdo_idle_text_scroll_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void fmrdo_idle_text_scroll_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_FM_RADIO__
    gui_handle_scrolling_text(&scroll_fmrdo_idleText);
#endif 
}

 


/*****************************************************************************
 * FUNCTION
 *  wgui_hide_background_with_wallpaper
 * DESCRIPTION
 *  redrwa the background by using the wallpaper
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y poistion
 *  x2      [IN]        End x position
 *  y2      [IN]        End y postion
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_hide_background_with_wallpaper(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifndef __GDI_MEMORY_PROFILE_2__
    PU8 idle_screen_wallpaper;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __GDI_MEMORY_PROFILE_2__
    /* gui_fill_rectangle(x1,y1,x2,y2,wgui_transparent_color); */
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, current_MMI_theme->general_background_filler);         
    if (idle_screen_wallpaper_ID == 0)
    {
        S32 disp_height = UI_device_height, disp_width = UI_device_width;

    #ifndef __MMI_IDLE_FULL_SCREEN__
        disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
        disp_width = UI_device_width - 1;
    #endif /* __MMI_IDLE_FULL_SCREEN__ */ 

        if ((disp_width >= cat33_image_width) && (disp_height >= cat33_image_height))   /* image can be displayed entirely */
        {
            S32 offset_x = 0;
            S32 offset_y = 0;

            /* align preview window at the center of screen */
            offset_x = (disp_width - cat33_image_width) >> 1;
            offset_y = (disp_height - cat33_image_height) >> 1;

            gdi_image_draw_file(offset_x, offset_y, (PS8) idle_screen_wallpaper_name);
        }
        else    /* image is larger than screen size */
        {
            S32 resized_offset_x;
            S32 resized_offset_y;
            S32 resized_width;
            S32 resized_height;

            gdi_image_util_fit_bbox(
                disp_width,
                disp_height,
                cat33_image_width,
                cat33_image_height,
                &resized_offset_x,
                &resized_offset_y,
                &resized_width,
                &resized_height);

            gdi_image_draw_resized_file(
                resized_offset_x,
                resized_offset_y,
                resized_width,
                resized_height,
                (PS8) idle_screen_wallpaper_name);
        }
    }
    else
    {
        idle_screen_wallpaper = get_image(idle_screen_wallpaper_ID);
        if (cat33_n_frames > 1)
        {
            gui_show_animated_image_frame(
                cat33_image_x,
                cat33_image_y,
                idle_screen_wallpaper,
                (S16) cat33_frame_number);
        }
        else
        {
            gui_show_image(cat33_image_x, cat33_image_y, idle_screen_wallpaper);
        }
    }
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
}

 


/*****************************************************************************
 * FUNCTION
 *  idle_screen_sat_display_text_background_redraw
 * DESCRIPTION
 *  redrwa the background ofSAT scrolling text area
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y poistion
 *  x2      [IN]        End x position
 *  y2      [IN]        End y postion
 * RETURNS
 *  void
 *****************************************************************************/
 
#if 0
#ifdef __GDI_MEMORY_PROFILE_2__
#else
#endif
#else /* 0 */ 
void idle_screen_sat_display_text_background_redraw(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
    wgui_hide_background_with_wallpaper(x1, y1, x2, y2);
    gui_pop_clip();
}
#endif /* 0 */ 
 

#ifdef __MMI_AUDIO_PLAYER__
extern BOOL mmi_audply_is_playing(void);
#endif 


/*****************************************************************************
 * FUNCTION
 *  idle_screen_show_network_details
 * DESCRIPTION
 *  show network name and network status on idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef __MMI_NOKIA_STYLE_N800__
void idle_screen_show_network_details(void)
{

	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	color_t c;
	S32 width, x, y = 0, h = 0, h2 = 0;
	S32 h_Total = 0, h2_Total = 0;
	BOOL show_extra = FALSE;
	UI_string_type extra_text = NULL;
	U8 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	void (*extra_scrolling_text_timer) (void) = NULL;

#if !defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) || !defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
	S32 w_inset = MMI_status_icon_bars[1].x2 - MMI_status_icon_bars[1].x1 + 1;
#else 
	S32 w_inset = X_INSERT;
#endif 

#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_push_clip();
    gdi_layer_set_clip(extra_text_x1, extra_text_y1, extra_text_x2, extra_text_y2);
    if (extra_text_x1 >= 0)
    {
        gdi_draw_solid_rect(extra_text_x1, extra_text_y1, extra_text_x2, extra_text_y2, GDI_COLOR_TRANSPARENT);
    }
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

#if defined(__MMI_NOKIA_STYLE_N800__)    
        /* show network name */
    idle_screen_show_network_name();
#endif

	mmi_trace(g_sw_NW, "MMI_NW: idle_screen_show_network_details");
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	for(i = 0; i < 1; i++)
#else
	for(i = 0; i < MMI_SIM_NUMBER; i++)
#endif
	{

		if (idle_screen_network_status[i] != NULL)
		{
			gui_set_font(&idle_screen_network_status_font);
			

			c = gui_color(255, 255, 255);

			gui_set_text_color(c);


			c = gui_color(0, 0, 0);

			gui_set_text_border_color(c);

			width = gui_get_string_width(idle_screen_network_status[i]);
			if (!r2lMMIFlag)
			{
				x = (UI_device_width >> 1) - (width >> 1);
			}
			else
			{
				x = (UI_device_width >> 1) - (width >> 1) + width;
			}


			y = MMI_status_bar_height + h + 7;


			y += (h+IDLESCREEN_TEXT_GAP)*i*2;// which sim network detail y postion
			
			gui_move_text_cursor(x, y + 20);

			gui_print_bordered_text(idle_screen_network_status[0]);

			gui_measure_string(idle_screen_network_status[0], &width, &h2);
			h2 += IDLESCREEN_TEXT_GAP*i;
		}   /* idle_screen_network_status!=NULL */
	}

	/* stop previous text scrolling */
	if (extra_scrolling_text)
	{
		gui_scrolling_text_stop(extra_scrolling_text);
		extra_scrolling_text = NULL;
	}

	if (GetSATIdleText() != NULL)
	{
		show_extra = TRUE;
		extra_text = (UI_string_type) GetSATIdleText();
		extra_scrolling_text = &scroll_sat_idleText;
		extra_scrolling_text_timer = satIdleTextScrollTimer;
	}
#if !defined(__MMI_MAINLCD_220X176__)
#ifdef __MMI_AUDIO_PLAYER__
	if (mmi_audply_is_playing())
	{
		show_extra = TRUE;
		memcpy(idle_title,g_audply.title,(FMGR_MAX_FILE_LEN + 1)*sizeof(UI_character_type));
		extra_text = (UI_string_type) idle_title;
		extra_scrolling_text = &scroll_audply_idleText;
		extra_scrolling_text_timer = audply_idle_text_scroll_timer;
	}
#endif /* __MMI_AUDIO_PLAYER__ */ 

#ifdef __MMI_FM_RADIO__
	if (mmi_fmrdo_is_power_on())
	{
		show_extra = TRUE;
		extra_text = (UI_string_type) mmi_fmrdo_get_channel_name_or_freq();
		extra_scrolling_text = &scroll_fmrdo_idleText;
		extra_scrolling_text_timer = fmrdo_idle_text_scroll_timer;
	}
#endif /* __MMI_FM_RADIO__ */ 
#endif

	if (idle_screen_type == ANALOG_IDLE_SCREEN || idle_screen_type == DIGITAL_IDLE_SCREEN)
	{
		show_extra = FALSE;
	}

	if (show_extra && extra_scrolling_text != NULL)
	{
		color_t text_color, border_color;

		y = MMI_status_bar_height + h + 7 + h2;
		gui_set_font(&idle_screen_network_name_font);

		y+=MMI_IDLE_EXTRA_TEXT_OFFSET;          //added by kecx for fixing bug 12401 on 20090623
		extra_text_x1 = (S16) w_inset;
		extra_text_y1 = (S16) y;
		extra_text_x2 = (S16) (UI_device_width - w_inset - 1);
		extra_text_y2 = (S16) (y + h);

		text_color = gui_color(255, 255, 255);

		gui_set_text_color(text_color);

		border_color = gui_color(0, 0, 0);

		gui_set_text_border_color(border_color);

		if (r2lMMIFlag)
		{
			gui_move_text_cursor(extra_text_x1 + gui_get_character_width((UI_character_type) '['), extra_text_y1);
		}
		else
		{
			gui_move_text_cursor(extra_text_x1, extra_text_y1);
		}

		gui_print_bordered_text((UI_string_type)L"[");
#if defined(__MMI_MAINLCD_220X176__)

        if (r2lMMIFlag)
        {
            gui_move_text_cursor(extra_text_x2 - 2 *gui_get_character_width((UI_character_type) ']'), extra_text_y1);
        }
        else
        {
            gui_move_text_cursor(extra_text_x2 - 7 -  gui_get_character_width((UI_character_type) ']'), extra_text_y1);
        }
#else
		if (r2lMMIFlag)
		{
			gui_move_text_cursor(extra_text_x2 - 7 + gui_get_character_width((UI_character_type) ']'), extra_text_y1);
		}
		else
		{
			gui_move_text_cursor(extra_text_x2 - 7, extra_text_y1);
		}
#endif

		gui_print_bordered_text((UI_string_type)L"]");

		if (width <= (UI_device_width - ((w_inset + 5) * 2)))
		{
			width = gui_get_string_width(extra_text);

			if (!r2lMMIFlag)
			{
				x = (UI_device_width >> 1) - (width >> 1);
			}
			else
			{
				x = (UI_device_width >> 1) + (width >> 1);
			}

			gui_move_text_cursor(x, y);
			gui_print_bordered_text(extra_text);
		}
		else
		{
			x = w_inset + 8;
			width = UI_device_width - (w_inset + 8) * 2;
			gui_create_scrolling_text(extra_scrolling_text,
									x,
									y,
									width,
									h + 2,
									extra_text,
									extra_scrolling_text_timer,
									idle_screen_sat_display_text_background_redraw,
									text_color,
									border_color);
			extra_scrolling_text->scroll_gap = 20;
			extra_scrolling_text->flags |= UI_SCROLLING_TEXT_BORDERED_TEXT;
			gui_show_scrolling_text(extra_scrolling_text);
		}
	}
	else    /* show_extra && extra_scrolling_text != NULL */
	{
		extra_text_x1 = -1; /* for indicate no draw extra_text this time, so no need to clean it next time */
	}

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_pop_clip();
	gdi_layer_blt_previous(extra_text_x1, extra_text_y1-1, extra_text_x2, extra_text_y2+2);
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

	if (GetSATIdleIcon())
	{
		y = MMI_status_bar_height + h + 7 + h2 + 20;
		gui_measure_image(GetSATIdleIcon(), &width, &h);
		x = (UI_device_width >> 1) - (width >> 1);
		gui_push_clip();
		gui_set_clip(x, y, x + width, y + h);
		gui_show_image(x, y, GetSATIdleIcon());
		gui_pop_clip();
	}
}
 #else
void idle_screen_show_network_details(void)
{

	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	color_t c;
	S32 width, x, y = 0, h = 0, h2 = 0;
	S32 h_Total = 0, h2_Total = 0;
	BOOL show_extra = FALSE;
	UI_string_type extra_text = NULL;
	U8 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	void (*extra_scrolling_text_timer) (void) = NULL;

#if !defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) || !defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
#if defined(__PROJECT_GALLITE_C01__)
	S32 w_inset = MMI_status_icon_bars[1].x2 - MMI_status_icon_bars[1].x1 + 1+6;
#else 
	S32 w_inset = MMI_status_icon_bars[1].x2 - MMI_status_icon_bars[1].x1 + 1;
#endif 
#else 
	S32 w_inset = X_INSERT;
#endif 

#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_push_clip();
#if defined(__PROJECT_GALLITE_C01__)
    gdi_layer_set_clip(0, 0, UI_device_width - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width) - 1, UI_device_height);
#else 
    gdi_layer_set_clip(0, 0, UI_device_width - (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width) - 1, UI_device_height);
#endif 
    if (extra_text_x1 >= 0)
    {
        gdi_draw_solid_rect(extra_text_x1, extra_text_y1-1, extra_text_x2, extra_text_y2+2, GDI_COLOR_TRANSPARENT);
    }
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

	mmi_trace(g_sw_NW, "MMI_NW: idle_screen_show_network_details");
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	for(i = 0; i < 1; i++)
#else
	for(i = 0; i < MMI_SIM_NUMBER; i++)
#endif
	{
		if (idle_screen_network_name[i] != NULL)
		{
			mmi_trace(g_sw_NW, "MMI_NW: show idle_screen_network_name[%d]",i);
			/* set font according to string width */
		#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
			gdi_layer_push_clip();
			gdi_layer_set_clip(MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width,
								0,
								UI_device_width - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width) - 1,
								MMI_title_height);
			if (gui_get_string_width(idle_screen_network_name[0]) > UI_device_width - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width - 1)
			{
				gui_set_font(&MMI_small_font);
			}
			else
			{
				gui_set_font(&MMI_medium_font);
			}
		#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
			gui_set_font(&idle_screen_network_name_font);
		#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 

			/*
			* #if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			* c = gui_color(255,255,255);
			* #elif defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
			* gui_set_font(&idle_screen_network_name_font);
			* c=gui_color(255,255,255);
			* #else
			* gui_set_font(&idle_screen_network_name_font);
			* c=gui_color(0,255,255);
			* #endif
			*/
			c = gui_color(255, 255, 255);
			gui_set_text_color(c);

		#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
			c = *current_MMI_theme->title_text_color;
		#elif defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) && defined(__MMI_MAINLCD_240X320__)
			c = gui_color(18, 114, 175);
		#else
			c = gui_color(0, 0, 0);
		#endif
			gui_set_text_border_color(c);

			gui_measure_string(idle_screen_network_name[i], &width, &h);

			h = UI_get_font_height(&MMI_large_font);

			if (!r2lMMIFlag)
			{
				x = (UI_device_width >> 1) - (width >> 1);
			}
			else
			{
				x = (UI_device_width >> 1) - (width >> 1) + width;
			}

		#if defined(__MMI_UI_DALMATIAN_STATUSBAR__)
			if (UI_font->type == MMI_small_font.type)
			{
				y = (MMI_title_height >> 1) - (h >> 1); /* for center in Title bar */
			}
			else
			{
				y = MMI_title_height - (h + 1); /* add 1 to create gap bottom of text */
			}
		#elif defined (__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			y = (UI_device_height >> 1) + (UI_device_height >> 2) - IDLESCREEN_TEXT_GAP - h;
		#else 
			#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
			y = MMI_status_bar_height + 4;
			/*+ zhouqin 2011-02-16 modify for q6 320X240 */
			#elif defined(__MMI_SYMBIAN_STYLE_IDLE__)
			y = IDLE_NETWORK_GAP + 2;
			/*- zhouqin 2011-02-16 modify for q6 320X240 */
			#else
			y = MMI_status_bar_height + 2;
			#endif 
		#endif
#if defined(__MMI_MAINLCD_128X128__)
			y += 0x10*i;//each sim network name y postion 
			h_Total += 0x10*i;
#else
#if defined(__PROJECT_GALLITE_C01__)
			y += (h+IDLESCREEN_TEXT_GAP)*i;//each sim network name y postion
#else 
			y += (h+IDLESCREEN_TEXT_GAP)*i + h2_Total;//each sim network name y postion
#endif 

			h_Total += (h+IDLESCREEN_TEXT_GAP);
#endif

			gui_move_text_cursor(x, y + IDLESCREEN_TEXT_GAP);

			gui_print_bordered_text(idle_screen_network_name[i]);

		#if defined(__MMI_UI_DALMATIAN_STATUSBAR__)
			gdi_layer_pop_clip();
			gdi_layer_blt_previous(MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width,
									0,
									UI_device_width - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width) - 1,
									MMI_title_height);
		#endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) */ 
		}   /* idle_screen_network_name!=NULL */
		else
		{
			mmi_trace(g_sw_NW, "MMI_NW: %s(%d) name is NULL", __FUNCTION__, i);
		}

		if (idle_screen_network_status[i] != NULL)
		{
			mmi_trace(g_sw_NW, "MMI_NW: show idle_screen_network_status[%d]",i);
			gui_set_font(&idle_screen_network_status_font);

		#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			#ifdef __MMI_MAINLCD_240X320__
			c = gui_color(18, 114, 175);
			#else 
			c = gui_color(255, 255, 255);
			#endif 
		#else /* defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) */ 
			#ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
			c = *current_MMI_theme->title_text_color;
			#else 
			c = gui_color(255, 255, 0);
			#endif 
		#endif /* defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) */ 
			gui_set_text_color(c);

		#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) && defined(__MMI_MAINLCD_240X320__)
			c = gui_color(255, 255, 255);
		#else 
			c = gui_color(0, 0, 0);
		#endif 
			gui_set_text_border_color(c);

			width = gui_get_string_width(idle_screen_network_status[i]);
			if (!r2lMMIFlag)
			{
				x = (UI_device_width >> 1) - (width >> 1);
			}
			else
			{
				x = (UI_device_width >> 1) - (width >> 1) + width;
			}

		#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
			gui_measure_string(idle_screen_network_status[i], &width, &h2);
			if (PhnsetGetShowDateTimeStatus() != MMI_FALSE)
			{
				y = UI_device_height - (MMI_button_bar_height + (main_LCD_dt_object.time.height) * 3 + (BAR_CONTENT_GAP));
			}
			else
			{
				y = UI_device_height - (MMI_button_bar_height + (h2 + BAR_CONTENT_GAP) + (BAR_CONTENT_GAP));
			}
		#elif defined (__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			y = y + h + IDLESCREEN_TEXT_GAP;
		/*+ zhouqin 2011-02-16 modify for q6 320X240 */
		#elif defined(__MMI_SYMBIAN_STYLE_IDLE__)
			y = IDLE_NETWORK_GAP +h+ 2;
		/*- zhouqin 2011-02-16 modify for q6 320X240 */
		#else 
			y = MMI_status_bar_height + h + 2;
		#endif 

#if defined(__PROJECT_GALLITE_C01__)
			y += (h+IDLESCREEN_TEXT_GAP)*i;// which sim network detail y postion
#else 
			y += (h+IDLESCREEN_TEXT_GAP)*i + h2_Total;// which sim network detail y postion
#endif 
			
			gui_move_text_cursor(x, y + IDLESCREEN_TEXT_GAP);

			gui_print_bordered_text(idle_screen_network_status[i]);

			gui_measure_string(idle_screen_network_status[i], &width, &h2);
			h2_Total += (h2+IDLESCREEN_TEXT_GAP);
		}   /* idle_screen_network_status!=NULL */
	}

	/* stop previous text scrolling */
	if (extra_scrolling_text)
	{
		gui_scrolling_text_stop(extra_scrolling_text);
		extra_scrolling_text = NULL;
	}

	if (GetSATIdleText() != NULL)
	{
		show_extra = TRUE;
		extra_text = (UI_string_type) GetSATIdleText();
		extra_scrolling_text = &scroll_sat_idleText;
		extra_scrolling_text_timer = satIdleTextScrollTimer;
	}
#if !defined(__MMI_MAINLCD_220X176__)
#ifdef __MMI_AUDIO_PLAYER__
	if (mmi_audply_is_playing())
	{
		show_extra = TRUE;
		memcpy(idle_title,g_audply.title,(FMGR_MAX_FILE_LEN + 1)*sizeof(UI_character_type));
		extra_text = (UI_string_type) idle_title;
		extra_scrolling_text = &scroll_audply_idleText;
		extra_scrolling_text_timer = audply_idle_text_scroll_timer;
	}
#endif /* __MMI_AUDIO_PLAYER__ */ 

#ifdef __MMI_FM_RADIO__
	if (mmi_fmrdo_is_power_on())
	{
		show_extra = TRUE;
		extra_text = (UI_string_type) mmi_fmrdo_get_channel_name_or_freq();
		extra_scrolling_text = &scroll_fmrdo_idleText;
		extra_scrolling_text_timer = fmrdo_idle_text_scroll_timer;
	}
#endif /* __MMI_FM_RADIO__ */ 
#endif

	if (idle_screen_type == ANALOG_IDLE_SCREEN || idle_screen_type == DIGITAL_IDLE_SCREEN)
	{
		show_extra = FALSE;
	}

	if (show_extra && extra_scrolling_text != NULL)
	{
		color_t text_color, border_color;

	#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
		y = (UI_device_height >> 1) - (h >> 1);
		gui_set_font(&MMI_medium_font);
	#elif defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
		gui_set_font(&idle_screen_network_name_font);
		gui_measure_string(extra_text, &width, &h);
		if (PhnsetGetShowDateTimeStatus() != MMI_FALSE)
		{
			if (idle_screen_network_status[0] != NULL)
			{
				y = UI_device_height - (MMI_button_bar_height + (main_LCD_dt_object.time.height) * 4 + (BAR_CONTENT_GAP));
			}
			else
			{
				y = UI_device_height - (MMI_button_bar_height + (main_LCD_dt_object.time.height) * 3 + (BAR_CONTENT_GAP));
			}
			else if ((PhnsetGetShowDateTimeStatus() == MMI_FALSE) || (idle_screen_network_status[0] != NULL))
			{
				y = UI_device_height - (MMI_button_bar_height + (h2 + BAR_CONTENT_GAP) * 2 + (BAR_CONTENT_GAP));
			}
			else
			{
				y = UI_device_height - (MMI_button_bar_height + (h + BAR_CONTENT_GAP) + (BAR_CONTENT_GAP));
			}
		}
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#elif defined(__MMI_SYMBIAN_STYLE_IDLE__)
	        y = IDLE_NETWORK_GAP +2  + h_Total + h2_Total + IDLESCREEN_TEXT_GAP;
	/*- zhouqin 2011-02-16 modify for q6 320X240 */	
	#else 	
		y = MMI_status_bar_height + h_Total + h2_Total + IDLESCREEN_TEXT_GAP;
		gui_set_font(&idle_screen_network_name_font);
	#endif 
	
	#ifndef __MMI_UI_DALMATIAN_IDLESCREEN__
		gui_set_font(&idle_screen_network_name_font);
		gui_measure_string(extra_text, &width, &h);
	#endif /* __MMI_UI_DALMATIAN_IDLESCREEN__ */ 
		extra_text_x1 = (S16) w_inset;
		extra_text_y1 = (S16) y;
		extra_text_x2 = (S16) (UI_device_width - w_inset - 1);
		extra_text_y2 = (S16) (y + h);

	#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
		#ifdef __MMI_MAINLCD_240X320__
		text_color = gui_color(18, 114, 175);
		#else 
		text_color = (current_MMI_theme->formatted_inputbox_normal_filler->shadow_color);
		#endif 
	#elif defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
		text_color = *current_MMI_theme->title_text_color;
	#else 
		text_color = gui_color(255, 255, 0);
	#endif 

		gui_set_text_color(text_color);

	#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
		border_color = gui_color(255, 255, 255);
	#elif defined( __MMI_UI_DALMATIAN_IDLESCREEN__)
		border_color = gui_color(255, 255, 255);
	#else 
		border_color = gui_color(0, 0, 0);
	#endif 

		gui_set_text_border_color(border_color);

		if (r2lMMIFlag)
		{
			gui_move_text_cursor(extra_text_x1 + gui_get_character_width((UI_character_type) '['), extra_text_y1);
		}
		else
		{
			gui_move_text_cursor(extra_text_x1, extra_text_y1);
		}

		gui_print_bordered_text((UI_string_type)L"[");
#if defined(__MMI_MAINLCD_220X176__)

        if (r2lMMIFlag)
        {
            gui_move_text_cursor(extra_text_x2 - 2 *gui_get_character_width((UI_character_type) ']'), extra_text_y1);
        }
        else
        {
            gui_move_text_cursor(extra_text_x2 - 7 -  gui_get_character_width((UI_character_type) ']'), extra_text_y1);
        }
#else
		if (r2lMMIFlag)
		{
			gui_move_text_cursor(extra_text_x2 - 7 + gui_get_character_width((UI_character_type) ']'), extra_text_y1);
		}
		else
		{
			gui_move_text_cursor(extra_text_x2 - 7, extra_text_y1);
		}
#endif

		gui_print_bordered_text((UI_string_type)L"]");

		if (width <= (UI_device_width - ((w_inset + 5) * 2)))
		{
			width = gui_get_string_width(extra_text);

			if (!r2lMMIFlag)
			{
				x = (UI_device_width >> 1) - (width >> 1);
			}
			else
			{
				x = (UI_device_width >> 1) + (width >> 1);
			}

			gui_move_text_cursor(x, y);
			gui_print_bordered_text(extra_text);
		}
		else
		{
#if defined(__PROJECT_GALLITE_C01__)
			x = w_inset + 12;
			width = UI_device_width - (w_inset + 12) * 2;
#else 
			x = w_inset + 8;
			width = UI_device_width - (w_inset + 8) * 2;
#endif 

		#ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
			gui_create_scrolling_text(extra_scrolling_text,
									x,
									y,
									width,
									h + 2,
									extra_text,
									extra_scrolling_text_timer,
									idle_screen_sat_display_text_background_redraw,
									text_color,
									border_color);

		#else /* __MMI_UI_DALMATIAN_IDLESCREEN__ */ 
			gui_create_scrolling_text(extra_scrolling_text,
									x,
									y,
									width,
									h + 2,
									extra_text,
									extra_scrolling_text_timer,
									idle_screen_sat_display_text_background_redraw,
									text_color,
									border_color);
		#endif /* __MMI_UI_DALMATIAN_IDLESCREEN__ */ 
			extra_scrolling_text->scroll_gap = 20;
			extra_scrolling_text->flags |= UI_SCROLLING_TEXT_BORDERED_TEXT;
			gui_show_scrolling_text(extra_scrolling_text);
		}
	}
	else    /* show_extra && extra_scrolling_text != NULL */
	{
		extra_text_x1 = -1; /* for indicate no draw extra_text this time, so no need to clean it next time */
	}

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_pop_clip();
	gdi_layer_blt_previous(extra_text_x1, extra_text_y1-1, extra_text_x2, extra_text_y2+2);
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

	if (GetSATIdleIcon())
	{
		y = MMI_status_bar_height + h + 7 + h2 + 20;
		gui_measure_image(GetSATIdleIcon(), &width, &h);
		x = (UI_device_width >> 1) - (width >> 1);
		gui_push_clip();
		gui_set_clip(x, y, x + width, y + h);
		gui_show_image(x, y, GetSATIdleIcon());
		gui_pop_clip();
	}

}
 #endif
#ifdef __MMI_MAINLCD_220X176__
extern U16   gKeyPadLockFlag;
#endif
void idle_screen_redraw(void)
{
	gdi_layer_push_clip();
	gdi_layer_set_clip(0, 0, UI_device_width, UI_device_height);
	gdi_draw_solid_rect(0, 0, UI_device_width, UI_device_height, GDI_COLOR_TRANSPARENT);
	show_status_icons();
	idle_screen_show_network_details();
	update_mainlcd_dt_display();
#ifdef __MMI_WGUI_CSK_ENABLE__
#ifdef __MMI_MAINLCD_220X176__
	if (gKeyPadLockFlag == 0 )
#endif		
	{
	    EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
	    redraw_softkey(MMI_CENTER_SOFTKEY);
	}
#endif
#if (defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__) || defined(__MMI_MAINLCD_220X176__))
        display_touch_idle_screen_shortcuts();
#endif /* defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__) */ 
	redraw_softkey(MMI_LEFT_SOFTKEY);
	redraw_softkey(MMI_RIGHT_SOFTKEY);
	
	
      gdi_layer_pop_clip();
}
/*****************************************************************************
 * FUNCTION
 *  idle_screen_show_network_name
 * DESCRIPTION
 *  show network name  on idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_NOKIA_STYLE_N800__
void idle_screen_show_network_name(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	color_t c;
	S32 width, x, y, h;
	U8 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	gui_reset_text_clip();
	mmi_trace(g_sw_NW, "MMI_NW: %s", __FUNCTION__);
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	for(i = 0; i < 1; i++)
#else
	for(i = 0; i < MMI_SIM_NUMBER; i++)
#endif
	{
		if (idle_screen_network_name[i] != NULL)
		{
			gui_set_font(&idle_screen_network_name_font);
			c = gui_color(255, 255, 255);
			gui_set_text_color(c);

			c = gui_color(0, 0, 0);

			gui_set_text_border_color(c);
			gui_measure_string(idle_screen_network_name[i], &width, &h);

			h = UI_get_font_height(&MMI_default_font);


			if (r2lMMIFlag)
			{
				x = (UI_device_width >> 1) + (width >> 1);
			}
			else
			{
				x = (UI_device_width >> 1) - (width >> 1);
			}

#if defined(__MMI_NOKIA_STYLE_N800__)    
                    if (idle_screen_type == ANALOG_IDLE_SCREEN || idle_screen_type == DIGITAL_IDLE_SCREEN)
                    {
                        y = 138;//170;//MMI_IDLE_NETWORK_NAME_Y_WITH_CLOCK_BG;//sa
                    }
                    else
                    {
                        y =138;//170;// MMI_IDLE_NETWORK_NAME_Y;
                    }

                	 if (r2lMMIFlag)
                	{
                		x= 166;
                	 }
                	 else
                	 {
                        	 x=1;
                	 }
#else
     			y = MMI_status_bar_height + 2;
#endif
			h =16;
			y += (h+2)*i;// which sim network name y postion
			
			
#if defined(__MMI_NOKIA_STYLE_N800__)    
	gdi_draw_solid_rect(0, y - 1, UI_device_width, y+h, GDI_COLOR_TRANSPARENT);  

#else
			gdi_draw_solid_rect(0, y+10, UI_device_width - (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width) - 1, y+h+20, GDI_COLOR_TRANSPARENT);  
#endif

			gui_move_text_cursor(x, y);
			gui_print_bordered_text(idle_screen_network_name[i]);
			
			#if defined(__MMI_NOKIA_STYLE_N800__)    
			
			gdi_layer_blt_previous(0,y, UI_device_width,y+h);

#else
		gdi_layer_blt_previous(0,MMI_status_bar_height, UI_device_width,y+h+20);
#endif

		}   /* idle_screen_network_name!=NULL */
		else
		{
			mmi_trace(g_sw_NW, "MMI_NW: %s(%d), name is NULL", __FUNCTION__, i);
		}
	}
}
#else
void idle_screen_show_network_name(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	color_t c;
	S32 width, x, y, h;
	U8 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	gui_reset_text_clip();
	mmi_trace(g_sw_NW, "MMI_NW: %s", __FUNCTION__);
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	for(i = 0; i < 1; i++)
#else
	for(i = 0; i < MMI_SIM_NUMBER; i++)
#endif
	{
		if (idle_screen_network_name[i] != NULL)
		{
			mmi_trace(g_sw_NW, "MMI_NW: show idle_screen_network_name[%d]", i);
		#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
			gui_set_font(&idle_screen_network_name_font);
			c = (current_MMI_theme->formatted_inputbox_normal_filler->shadow_color);
		#elif defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
			gui_push_clip();
			gui_set_text_clip(MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width,
							0,
							UI_device_width - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width) - 1,
							MMI_title_height);
			if (gui_get_string_width(idle_screen_network_name[i]) > UI_device_width - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width - 1)
			{
				gui_set_font(&MMI_small_font);
			}
			else
			{
				gui_set_font(&MMI_medium_font);
			}
			c = gui_color(255, 255, 255);   /* idle_scr_text_color; */
		#else 
			gui_set_font(&idle_screen_network_name_font);
			c = gui_color(255, 255, 255);
		#endif 
			gui_set_text_color(c);

		#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			c = (current_UI_theme->window_title_theme->active_filler->c);
		#elif defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
			c = *current_MMI_theme->title_text_color;
		#else 
			c = gui_color(0, 0, 0);
		#endif 
        #if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__) && defined(__MMI_MAINLCD_160X128__) 
            gui_set_font(&MMI_small_font);
        #endif

			gui_set_text_border_color(c);
			gui_measure_string(idle_screen_network_name[i], &width, &h);

        #if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__) && defined(__MMI_MAINLCD_160X128__) 
			if(IsChineseSet())
				h = 15;
			else
			h = UI_get_font_height(&MMI_medium_font)+1;
        #else
			h = UI_get_font_height(&MMI_large_font);
        #endif



			if (r2lMMIFlag)
			{
				x = (UI_device_width >> 1) + (width >> 1);
			}
			else
			{
				x = (UI_device_width >> 1) - (width >> 1);
			}

		#ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
			if (UI_font->type == MMI_small_font.type)
			{
				y = (MMI_title_height >> 1) - (h >> 1); /* for center in Title bar */
			}
			else
			{
				y = MMI_title_height - (h + 1);
			}
		#else /* __MMI_UI_DALMATIAN_IDLESCREEN__ */ 
			#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
			y = MMI_status_bar_height + 4;
			/*+ zhouqin 2011-02-16 modify for q6 320X240 */
			#elif defined(__MMI_SYMBIAN_STYLE_IDLE__)
			y = IDLE_NETWORK_GAP + 2;
			/*- zhouqin 2011-02-16 modify for q6 320X240 */
			#else 
			y = MMI_status_bar_height + 2;
			#endif 
		#endif /* __MMI_UI_DALMATIAN_IDLESCREEN__ */ 

        #if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__) && defined(__MMI_MAINLCD_160X128__) 
				x = 2;
        		if(0 == i)
        			y = main_LCD_dt_object.time.y + main_LCD_dt_object.time.height +3;
        		else if(1 == i)
        			y = main_LCD_dt_object.time.y + main_LCD_dt_object.time.height +3 + h;
        		else
        			y = main_LCD_dt_object.time.y + main_LCD_dt_object.time.height +3 + (2*h);
        		gdi_draw_solid_rect(0,y,UI_device_width, y+h,GDI_COLOR_TRANSPARENT);

			    gui_move_text_cursor(x, y);
        #else

			y += (h+IDLESCREEN_TEXT_GAP)*i;// which sim network name y postion

#if defined(__PROJECT_GALLITE_C01__) || defined(__MMI_USING_BLACK_THEME_BG__)
			gdi_draw_solid_rect(0, y+IDLESCREEN_TEXT_GAP - 1, UI_device_width - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width) - 1, y+h+20, GDI_COLOR_TRANSPARENT);  
#else 
			gdi_draw_solid_rect(0, y+IDLESCREEN_TEXT_GAP - 1, UI_device_width - (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width) - 1, y+h+20, GDI_COLOR_TRANSPARENT);  
#endif 
                   
			//gdi_draw_solid_rect(0, y+IDLESCREEN_TEXT_GAP, UI_device_width - (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width) - 1, y+h+20, GDI_COLOR_TRANSPARENT);  

			gui_move_text_cursor(x, y+IDLESCREEN_TEXT_GAP);
        #endif
        
			gui_print_bordered_text(idle_screen_network_name[i]);
		#ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
			gui_pop_clip();
			gui_BLT_double_buffer(MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width,
								0,
								UI_device_width - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width) - 1,
								MMI_title_height);
			gui_reset_text_clip();
			/*qiff add to blt when on GS PRJ.*/	
		#else
            #if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__) && defined(__MMI_MAINLCD_160X128__) 
            gdi_layer_blt_previous(x,y, UI_device_width,y+h);
            #else    
            gdi_layer_blt_previous(0,MMI_status_bar_height, UI_device_width,y+h+20);
            #endif
		#endif /* __MMI_UI_DALMATIAN_IDLESCREEN__ */ 
		}   /* idle_screen_network_name!=NULL */
		else
		{
			mmi_trace(g_sw_NW, "MMI_NW: %s(%d), name is NULL", __FUNCTION__, i);
		}
	}
}
#endif
/*****************************************************************************
 * FUNCTION
 *  dummy_dt_hide_function
 * DESCRIPTION
 *  dummy func of hide date
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void dummy_dt_hide_function(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x1);
    UI_UNUSED_PARAMETER(y1);
    UI_UNUSED_PARAMETER(x2);
    UI_UNUSED_PARAMETER(y2);
}

extern void redraw_status_icon_bar(U8 j);


/*****************************************************************************
 * FUNCTION
 *  cat33_analog_clock_hide_dt_display
 * DESCRIPTION
 *  hide function of date display of analog clock
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void cat33_analog_clock_hide_dt_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifndef __GDI_MEMORY_PROFILE_2__
    PU8 idle_screen_wallpaper = get_image(idle_screen_wallpaper_ID);
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = 0;
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    y1 = MMI_status_bar_height + MMI_title_height;
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    /* for proper display of the status bar at analog clock */
    if (is_on_idlescreen())
    {
        y1 = 0;
    }
    else
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
         
        y1 = MMI_status_bar_height;
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
    x2 = UI_device_width - 1;
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    /* for proper display of the status bar at analog clock */
    if (is_on_idlescreen())
    {
        y2 = (UI_device_height - 1 - MMI_button_bar_height - MMI_status_bar_height - 4);
    }
    else
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
         
        y2 = UI_device_height - MMI_button_bar_height;
    gui_set_clip(x1, y1, x2, y2);
#ifdef __GDI_MEMORY_PROFILE_2__
    /* gui_fill_rectangle(x1,y1,x2,y2,wgui_transparent_color); */
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    if (idle_screen_wallpaper_ID == 0)
    {
        ASSERT(idle_screen_wallpaper_ID == 0);   
        gdi_image_draw_file(cat33_image_x, cat33_image_y, idle_screen_wallpaper_name);
    }
    else
    {
        if (cat33_n_frames > 1)
        {
            gui_show_animated_image_frame(
                cat33_image_x,
                cat33_image_y,
                idle_screen_wallpaper,
                (S16) cat33_frame_number);
        }
        else
        {
            gui_show_image(cat33_image_x, cat33_image_y, idle_screen_wallpaper);
        }
    }
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    redraw_status_icon_bar(1);
    idle_screen_show_network_name();
}


/*****************************************************************************
 * FUNCTION
 *  cat33_digital_clock_hide_dt_display
 * DESCRIPTION
 *  hide function of date display of digital  clock
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
/* Assumes that the wallpaper displayed behind the digital clock is always full screen */
void draw_wallpaper(void);
void cat33_digital_clock_hide_dt_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
#ifndef __GDI_MEMORY_PROFILE_2__
    PU8 idle_screen_wallpaper = get_image(idle_screen_wallpaper_ID);
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     

     
#ifndef __MMI_UI_DALMATIAN_IDLESCREEN__
    /* To retain passing coordinates to the clip window */
    x1 = 0;
    x2 = UI_device_width - 1;
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    /* for proper display of the status bar at digital clock */
    if (is_on_idlescreen())
    {
        y1 = 0;
    }
    else
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
         
        y1 = MMI_status_bar_height;
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    /* for proper display of the status bar at digital clock */
    if (is_on_idlescreen())
    {
        y2 = (UI_device_height - 1 - MMI_button_bar_height - MMI_status_bar_height - 4);
    }
    else
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
         
         
        /* change for idlescreen shortcuts as their height is greater
           than MMI_button_bar_height */
#if defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__)
        y2 = UI_device_height - max_touch_idle_screen_shortcut_height() - 1;
#else 
        y2 = UI_device_height - MMI_button_bar_height;
#endif 
     
#endif /* __MMI_UI_DALMATIAN_IDLESCREEN__ */ 
     
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);

     
#ifdef __GDI_MEMORY_PROFILE_2__
    /* gui_fill_rectangle(x1,y1,x2,y2,wgui_transparent_color); */
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#else /* __GDI_MEMORY_PROFILE_2__ */ 
#if 0
#endif /* 0 */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    /* draw_wallpaper(); */
     

    redraw_status_icon_bar(1);
    gui_pop_clip();

     
    /* idle_screen_show_network_details(); */
    if (idle_screen_type == ANALOG_IDLE_SCREEN || idle_screen_type == DIGITAL_IDLE_SCREEN)
    {
        idle_screen_show_network_name();
    }
    else
    {
        idle_screen_show_network_details();
    }
     
}

 
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__


/*****************************************************************************
 * FUNCTION
 *  cat33_hide_date_display
 * DESCRIPTION
 *  hide function of date display on idlescreen
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void cat33_hide_date_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
     
#ifdef __GDI_MEMORY_PROFILE_2__
    /* gui_fill_rectangle(x1, y1, x2, y2, wgui_transparent_color); */
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    /* gui_fill_rectangle(x1, y1, x2, y2, current_MMI_theme->title_filler->c); */
    gui_show_image(0, MMI_status_bar_height, get_image(idlescreen_bar_id));//070306 Alpha layer
     
    gui_pop_clip();
}

 


/*****************************************************************************
 * FUNCTION
 *  cat33_hide_day_string_display
 * DESCRIPTION
 *  hide function of day display on idlescreen
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void cat33_hide_day_string_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
#ifdef __GDI_MEMORY_PROFILE_2__
    /* gui_fill_rectangle(x1,y1,x2,y2,wgui_transparent_color); */
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    gui_show_image(0, MMI_status_bar_height, get_image(idlescreen_bar_id));//070306 Alpha layer
    gui_pop_clip();
}

 
#endif /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */ 
 


/*****************************************************************************
 * FUNCTION
 *  cat33_hide_time_display
 * DESCRIPTION
 *  hide function of time display of analog/digital  clock
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
 
#if 0
#ifndef __GDI_MEMORY_PROFILE_2__
#endif
#ifdef __GDI_MEMORY_PROFILE_2__
#else
#endif
#else /* 0 */ 
void cat33_hide_time_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(0, y1, MAIN_LCD_device_width - 1, y2);
    wgui_hide_background_with_wallpaper(x1, y1, x2, y2);
    redraw_status_icon_bar(1);
    gui_pop_clip();
}
#endif /* 0 */ 
 


/*****************************************************************************
 * FUNCTION
 *  idle_screen_hide_status_icons_bar0
 * DESCRIPTION
 *  hide function of stats icon horizontal bar of idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void idle_screen_hide_status_icons_bar0(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;

#ifndef __MMI_IDLE_FULL_SCREEN__
    color_t c = *current_MMI_theme->statusbar0_background_color;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    get_status_icon_bar_clip(0, &x1, &y1, &x2, &y2);
    gui_set_clip(x1, y1, x2, y2);
#ifdef __MMI_IDLE_FULL_SCREEN__
     
    if (idle_screen_wallpaper_ID == 0)
#ifdef __GDI_MEMORY_PROFILE_2__
        /* gui_fill_rectangle(x1,y1,x2,y2,wgui_transparent_color); */
        gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);      
#else /* __GDI_MEMORY_PROFILE_2__ */ 
        draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    else
    {
    #ifdef __GDI_MEMORY_PROFILE_2__
        /* gui_fill_rectangle(x1,y1,x2,y2,wgui_transparent_color); */
#if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
	    if (GetActiveScreenId() != IDLE_SCREEN_ID)
		gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_BLACK);
	    else	
	#endif	

gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);      
    #else /* __GDI_MEMORY_PROFILE_2__ */ 
         
    #if !((defined __MMI_UI_DALMATIAN_STATUSBAR__) && (defined __MMI_MAINLCD_176X220__)&& (defined __MMI_MAINLCD_220X176__))
        draw_wallpaper();
    #endif 
    #endif /* __GDI_MEMORY_PROFILE_2__ */ 
         
    }
     
#else /* __MMI_IDLE_FULL_SCREEN__ */ 
    gui_fill_rectangle(x1, y1, x2, y2, c);
#endif /* __MMI_IDLE_FULL_SCREEN__ */ 
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  idle_screen_hide_status_icons_bar1
 * DESCRIPTION
 *  hide function of stats icon vertical  bar of idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void idle_screen_hide_status_icons_bar1(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    PU8 idle_screen_wallpaper;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    idle_screen_wallpaper = get_image(idle_screen_wallpaper_ID);
    gui_push_clip();
    get_status_icon_bar_clip(1, &x1, &y1, &x2, &y2);
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    y1 = y1 + 4;    /* To restrict the clip area below from the time date area lines. */
#endif 
     
    gui_set_clip(x1, y1, x2, y2);
     
    if (idle_screen_wallpaper_ID == 0)
#ifdef __GDI_MEMORY_PROFILE_2__
        /* gui_fill_rectangle(x1,y1,x2,y2,wgui_transparent_color); */
        gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);      
#else /* __GDI_MEMORY_PROFILE_2__ */ 
        draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    else
    {
    #ifdef __GDI_MEMORY_PROFILE_2__
        /* gui_fill_rectangle(x1,y1,x2,y2,wgui_transparent_color); */
        gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);      
    #else /* __GDI_MEMORY_PROFILE_2__ */ 
        draw_wallpaper();
    #endif /* __GDI_MEMORY_PROFILE_2__ */ 
    }
     
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  SetIdleScreenWallpaper
 * DESCRIPTION
 *  Sets the Image to be used by the idle screen
 * PARAMETERS
 *  image_ID        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetIdleScreenWallpaper(U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 idle_screen_wallpaper;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifndef __MMI_UI_TECHNO_IDLESCREEN_BAR__    
#ifdef __MMI_ANALOG_CLOCK__
    if ((image_ID == IMG_ID_DISPCHAR_ANALOG_CLK))
    {
        idle_screen_type = ANALOG_IDLE_SCREEN;
        idle_screen_wallpaper_ID = IMG_ID_DISPCHAR_ANALOG_CLK;
    }
    else
#endif /* __MMI_ANALOG_CLOCK__ */ 

#ifdef __MMI_DIGITAL_CLOCK__
    if ((image_ID == IMG_ID_DISPCHAR_DIGITAL_CLK))
    {
        idle_screen_wallpaper_ID = IMG_ID_DISPCHAR_DIGITAL_CLK;
        idle_screen_type = DIGITAL_IDLE_SCREEN;
    }
    else
#endif /* __MMI_DIGITAL_CLOCK__ */ 
#endif /* !__MMI_UI_TECHNO_IDLESCREEN_BAR__ */
    {
        idle_screen_type = NORMAL_IDLE_SCREEN;
        idle_screen_wallpaper_ID = image_ID;
    }
    idle_screen_wallpaper = get_image(idle_screen_wallpaper_ID);
    gui_measure_image(idle_screen_wallpaper, &cat33_image_width, &cat33_image_height);
    cat33_n_frames = gui_image_n_frames(idle_screen_wallpaper);
    //cat33_image_x=(UI_device_width>>1)-(cat33_image_width>>1);
    //cat33_image_y=(UI_device_height>>1)-(cat33_image_height>>1);
    cat33_image_x = (UI_device_width - cat33_image_width) >> 1;  
    cat33_image_y = (UI_device_height - cat33_image_height) >> 1;
}


/*****************************************************************************
 * FUNCTION
 *  SetIdleScreenNetworkName
 * DESCRIPTION
 *  Sets the network name to be displayed by the idle screen
 * PARAMETERS
 *  s       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void SetIdleScreenNetworkName(U8 *s, U8 nSimID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 str_len, i, real_pos;
    S8 start_flag = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Lisen 0921-1 */
    if (s == NULL)
    {
        mmi_trace(g_sw_NW,"MMI_NW: Func: %s NULL", __FUNCTION__);
        idle_screen_network_name[nSimID] = NULL;
        return;
    }
	
    real_pos = 0;
    str_len = (U16) gui_strlen((UI_string_type) s) << 1;
    for (i = 0; i < str_len; i += 2)
    {
        if (s[i] != 0x20 || s[i + 1] != 0x00)
        {
            start_flag = 1;
        }
        if (start_flag == 1)    /* real data starts */
        {
            s[real_pos] = s[i];
            s[real_pos + 1] = s[i + 1];
            real_pos += 2;
        }
    }
    if (real_pos < str_len)
    {
        for (i = real_pos; i < str_len; i++)
        {
            s[i] = '\0';
        }
    }
    str_len = (U16) gui_strlen((UI_string_type) s) << 1;
    for (i = str_len - 2; i > 0; i -= 2)
    {
        if (s[i] != 0x20 || s[i + 1] != 0x00)
        {
            break;
        }
        else
        {
            s[i] = '\0';
            s[i + 1] = '\0';
        }
    }
    idle_screen_network_name[nSimID] = (UI_string_type) s;
}

/*****************************************************************************
 * FUNCTION
 *  SetIdleScreenNetworkStatus
 * DESCRIPTION
 *  Sets the network status to be displayed by the idle screen
 * PARAMETERS
 *  s       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void SetIdleScreenNetworkStatus(U8 *s, U8 nSimID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 str_len, i, real_pos;    /* To avoid the strlen length 0 case */
    S8 start_flag = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    real_pos = 0;
    str_len = (S16) gui_strlen((UI_string_type) s) << 1;
    for (i = 0; i < str_len; i += 2)
    {
        if (s[i] != 0x20 || s[i + 1] != 0x00)
        {
            start_flag = 1;
        }
        if (start_flag == 1)    /* real data starts */
        {
            s[real_pos] = s[i];
            s[real_pos + 1] = s[i + 1];
            real_pos += 2;
        }
    }
    if (real_pos < str_len)
    {
        for (i = real_pos; i < str_len; i++)
        {
            s[i] = '\0';
        }
    }
    str_len = (S16) gui_strlen((UI_string_type) s) << 1;
    for (i = str_len - 2; i > 0; i -= 2)
    {
        if (s[i] != 0x20 || s[i + 1] != 0x00)
        {
            break;
        }
        else
        {
            s[i] = '\0';
            s[i + 1] = '\0';
        }
    }
    idle_screen_network_status[nSimID] = (UI_string_type) s;
}

/*****************************************************************************
 * FUNCTION
 *  cat33_animation_processor
 * DESCRIPTION
 *  a function which shows frames of animated wallpaper
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat33_animation_processor(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 idle_screen_wallpaper = get_image(idle_screen_wallpaper_ID);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cat33_frame_number++;
    if (cat33_frame_number >= cat33_n_frames)
    {
        cat33_frame_number = 0;
    }
    gui_start_timer(cat33_animation_delay, cat33_animation_processor);

    gui_lock_double_buffer();

#ifndef __MMI_IDLE_FULL_SCREEN__
    gui_set_clip(0, MMI_status_bar_height, UI_device_width - 1, UI_device_height - MMI_button_bar_height);
#else /* __MMI_IDLE_FULL_SCREEN__ */ 
     
#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__)
    gui_set_clip(
        0,
        MMI_status_bar_height + MMI_title_height,
        UI_device_width - 1,
        UI_device_height - MMI_button_bar_height);
#else /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) */ 
     
    gui_reset_clip();
#endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) */ 
#endif /* __MMI_IDLE_FULL_SCREEN__ */ 
    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, current_MMI_theme->general_background_filler);
    gui_show_animated_image_frame(cat33_image_x, cat33_image_y, idle_screen_wallpaper, (S16) cat33_frame_number);

     
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
     
#ifndef __MMI_UI_TECHNO_STATUS_ICON__
    if (PhnsetGetShowDateTimeStatus() == MMI_TRUE)
#endif 
         
        gui_show_image(0, MMI_status_bar_height, get_image(idlescreen_bar_id));//070306 Alpha layer
#endif /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */ 
     
    show_main_LCD_dt_display();
     
#ifndef __MMI_UI_DALMATIAN_STATUSBAR__
#ifdef __MMI_IDLE_FULL_SCREEN__
    redraw_status_icon_bar(0);
#endif 
    redraw_status_icon_bar(1);
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
    idle_screen_show_network_details();

     
#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__
#ifdef __MMI_IDLE_FULL_SCREEN__
    if (cat33_image_height < UI_device_height)
    {
        show_softkey_background();
    }
    show_left_softkey();
    show_right_softkey();
#endif /* __MMI_IDLE_FULL_SCREEN__ */ 
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
     

    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}

 


/*****************************************************************************
 * FUNCTION
 *  wgui_set_wallpaper_change
 * DESCRIPTION
 *  Sets the Image to be used by the idle screen using ID
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
void wgui_set_wallpaper_change(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gIsWallpaperChange = 1;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_is_wallpaper_change
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
U8 wgui_is_wallpaper_change(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gIsWallpaperChange;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_reset_wallpaper_change
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_reset_wallpaper_change(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gIsWallpaperChange = 0;
}
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 


/*****************************************************************************
 * FUNCTION
 *  SetIdleScreenWallpaperID
 * DESCRIPTION
 *  
 * PARAMETERS
 *  image_ID        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 14:28 */
void SetIdleScreenWallpaperID(U16 image_ID/*, MMI_BOOL is_default*/)
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 14:28 */
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 idle_screen_wallpaper;
    
     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    U16 bg_img_id;
    S8 *bg_img_name;
    S32 bg_img_x, bg_img_y;
    U8 bg_img_opacity;
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     

#ifndef __MMI_UI_TECHNO_IDLESCREEN_BAR__
#ifdef __MMI_ANALOG_CLOCK__
    if ((image_ID == IMG_ID_DISPCHAR_ANALOG_CLK))
    {
        idle_screen_type = ANALOG_IDLE_SCREEN;
         
        idle_screen_wallpaper_ID = IMG_ID_DISPCHAR_ANALOG_CLK;
         
    }
    else
#endif /* __MMI_ANALOG_CLOCK__ */ 
#ifdef __MMI_DIGITAL_CLOCK__
    if ((image_ID == IMG_ID_DISPCHAR_DIGITAL_CLK))
    {
        idle_screen_wallpaper_ID = IMG_ID_DISPCHAR_DIGITAL_CLK;
        idle_screen_type = DIGITAL_IDLE_SCREEN;
    }
    else
#endif /* __MMI_DIGITAL_CLOCK__ */ 
#endif /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */
    {
        idle_screen_type = NORMAL_IDLE_SCREEN;
        idle_screen_wallpaper_ID = image_ID;
    }

    idle_screen_wallpaper = get_image(idle_screen_wallpaper_ID);
    gdi_image_get_dimension(idle_screen_wallpaper, &cat33_image_width, &cat33_image_height);
    gdi_image_get_frame_count_id(idle_screen_wallpaper_ID, &cat33_n_frames);
    cat33_image_x = (UI_device_width - cat33_image_width) >> 1;
    cat33_image_y = (UI_device_height - cat33_image_height) >> 1;

#ifdef CSD_TARGET
    if(cat33_n_frames > 1)
    {
#ifdef __MMI_SAVE_WP_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_WP_SPEEDUP__*/
        /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-11 9:50 */
        wgui_save_wallpaper_to_speedup(/*is_default*/);
        /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-11 9:50 */
#endif
    }
    else
#endif /* CSD_TARGET */        
    {
    #ifdef __MMI_WALLPAPER_ON_BOTTOM__
        idle_screen_cache_id = idle_screen_wallpaper_ID;
        idle_screen_cache_name = NULL;
            
        dm_set_scr_bg_redraw(MMI_TRUE);
        dm_get_scr_bg_image(&bg_img_id, &bg_img_name, &bg_img_x, &bg_img_y, &bg_img_opacity);
        dm_set_scr_bg_image(idle_screen_cache_id, idle_screen_cache_name, bg_img_x, bg_img_y, bg_img_opacity);
    #endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
    }
}


#ifdef __MMI_WALLPAPER_ON_BOTTOM__


/*****************************************************************************
 * FUNCTION
 *  SetIdleScreenWallpaperID
 * DESCRIPTION
 *  
 * PARAMETERS
 *  image_ID        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
 void SetListWallpaperID(U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 bg_img_id;
    S8 *bg_img_name;
    S32 bg_img_x, bg_img_y;
    U8 bg_img_opacity;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    idle_screen_type = NORMAL_IDLE_SCREEN;
    idle_screen_wallpaper_ID = image_ID;
    dm_set_scr_bg_redraw(MMI_TRUE);
    dm_get_scr_bg_image(&bg_img_id, &bg_img_name, &bg_img_x, &bg_img_y, &bg_img_opacity);
    dm_set_scr_bg_image(idle_screen_wallpaper_ID, NULL, bg_img_x, bg_img_y, bg_img_opacity);
}
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 

#ifdef __MMI_SAVE_WP_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_WP_SPEEDUP__*/
/*****************************************************************************
 * FUNCTION
 *  SetIdleScreenWallpaperName
 * DESCRIPTION
 *  
 * PARAMETERS
 *  name        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetIdleScreenWallpaperName(UI_string_type name)
{
  /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 disp_height = UI_device_height, disp_width = UI_device_width;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    idle_screen_wallpaper_ID = 0;
    idle_screen_type = NORMAL_IDLE_SCREEN;

#ifndef __MMI_IDLE_FULL_SCREEN__
    disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
    disp_width = UI_device_width - 1;
#endif /* __MMI_IDLE_FULL_SCREEN__ */ 

mmi_trace(g_sw_WpSsPwon, "gdm: into SetIdleScreenWallpaperName(), disp_height=%d, disp_width=%d", disp_height, disp_width);
mmi_trace(g_sw_WpSsPwon, "gdm: cat33_image_width=%d, cat33_image_height=%d", cat33_image_width, cat33_image_height);
    gdi_image_get_dimension_file((S8*) name, &cat33_image_width, &cat33_image_height);
mmi_trace(g_sw_WpSsPwon, "gdm: cat33_image_width=%d, cat33_image_height=%d", cat33_image_width, cat33_image_height);
    cat33_image_x = (disp_width - cat33_image_width) >> 1;   
    cat33_image_y = (disp_height - cat33_image_height) >> 1;
mmi_trace(g_sw_WpSsPwon, "gdm: cat33_image_x=%d, cat33_image_y=%d,cat33_n_frames=%d", cat33_image_x, cat33_image_y,cat33_n_frames);

    gdi_image_get_frame_count_file((U8 *)name, &cat33_n_frames);
mmi_trace(g_sw_WpSsPwon, "gdm: cat33_n_frames=%d", cat33_n_frames);

    pfnUnicodeStrcpy(idle_screen_wallpaper_name_original, (S8*) name);
    idle_screen_wallpaper_name = idle_screen_wallpaper_name_original;

}

#endif
/*****************************************************************************
 * FUNCTION
 *  draw_wallpaper
 * DESCRIPTION
 *  Redraws the Idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_wallpaper(void)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 idle_screen_wallpaper = get_image(idle_screen_wallpaper_ID);

     
    S32 disp_height = UI_device_height, disp_width = UI_device_width;

    GDI_RESULT ret = GDI_SUCCEED;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
    if (animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(animation_handle);
        animation_handle = GDI_ERROR_HANDLE;
    }
#endif /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, current_MMI_theme->general_background_filler);
#ifndef __MMI_IDLE_FULL_SCREEN__
     
    disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
    disp_width = UI_device_width - 1;
    gui_set_clip(0, MMI_status_bar_height, UI_device_width - 1, UI_device_height - MMI_button_bar_height);
#endif /* __MMI_IDLE_FULL_SCREEN__ */ 
#ifdef __MMI_SCREEN_ROTATE__
	if(mmi_frm_is_screen_width_height_swapped()){
		idle_screen_wallpaper = get_image(IMG_SCREEN_ROTATE_BACKGROUND);
		gdi_image_draw(0, 0, idle_screen_wallpaper);
		return;
	}
#endif
    if (idle_screen_wallpaper_ID != 0)  /* image comes from resource */
    {
  		mmi_trace(g_sw_WpSsPwon, "gdm: in draw_wallpaper: default resource: cat33_n_frames=%d",cat33_n_frames);
      if (cat33_n_frames > 1)
    #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
            gdi_image_draw_animation_frames(
                cat33_image_x,
                cat33_image_y,
                idle_screen_wallpaper,
                &animation_handle,
                (S16) cat33_frame_number);
    #else /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
        {
            gdi_image_draw_animation_single_frame(
                cat33_image_x,
                cat33_image_y,
                idle_screen_wallpaper,
                (S16) cat33_frame_number);
            cat33_animation_delay = gdi_image_get_animation_delay();
        }
    #endif /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
        else
        {
            gdi_image_draw(cat33_image_x, cat33_image_y, idle_screen_wallpaper);
        }
    }
    else
    {
        /* 102406 DRM check Start */
        FS_HANDLE originalPath = -1;
    #ifdef __DRM_SUPPORT__
        kal_bool DRM_ret = KAL_FALSE;
    #endif

        if (cat33_n_frames <= 1)//check if the original file of the cached static image still exists and the DRM right as well
        {
            if (idle_screen_wallpaper_name_original == NULL)
            {
                ret = GDI_IMAGE_ERR_INVALID_FILE;
            }
            else
            {
            #ifdef __DRM_SUPPORT__
                originalPath = DRM_open_file((PU16) idle_screen_wallpaper_name_original, FS_READ_ONLY | FS_OPEN_NO_DIR, DRM_PERMISSION_DISPLAY);
                if (originalPath >= 0)
                {
                    DRM_ret = DRM_validate_permission(originalPath, DRM_PERMISSION_DISPLAY);
                }
                if (DRM_ret == KAL_FALSE)
            #else
                originalPath = MMI_FS_Open((PU8) idle_screen_wallpaper_name_original, FS_READ_ONLY | FS_OPEN_NO_DIR);
                if (originalPath < 0)
            #endif
                {
                    ret = GDI_IMAGE_ERR_INVALID_FILE;
                }
                if (originalPath >= 0)
                {
                #ifdef __DRM_SUPPORT__
                    DRM_close_file(originalPath);
                #else
                    MMI_FS_Close(originalPath);
                #endif
                    originalPath = -1;
                }
             }
        }
        
        if (ret != GDI_IMAGE_ERR_INVALID_FILE)
        {
            S32 offset_x = 0;
            S32 offset_y = 0;

            gdi_image_get_dimension_file(idle_screen_wallpaper_name, &cat33_image_width, &cat33_image_height);
            mmi_trace(g_sw_WpSsPwon, "gdm: in draw_wallpaper(): cat33_image_width=%d, cat33_image_height=%d ", cat33_image_width,cat33_image_height);
            cat33_image_x = (disp_width - cat33_image_width) >> 1;
            cat33_image_y = (disp_height - cat33_image_height) >> 1;

            if ((disp_width >= cat33_image_width) && (disp_height >= cat33_image_height))       /* image can be displayed entirely */
            {
                /* align preview window at the center of screen */
                offset_x = (disp_width - cat33_image_width) >> 1;
                offset_y = (disp_height - cat33_image_height) >> 1;

            #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
            	mmi_trace(g_sw_WpSsPwon, "gdm: in draw_wallpaper: user define ");
            mmi_trace(g_sw_WpSsPwon,"gdm:idle_screen_wallpaper_name");
		ret = gdi_image_draw_animation_file(
                        offset_x,
                        offset_y,
                        (PS8) idle_screen_wallpaper_name,
                        &animation_handle);
            #else /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
                ret = gdi_image_draw_file(offset_x, offset_y, (PS8) idle_screen_wallpaper_name);
            #endif /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
            }
            else    /* image is larger than screen size */
            {
                S32 resized_offset_x;
                S32 resized_offset_y;
                S32 resized_width;
                S32 resized_height;

                gdi_image_util_fit_bbox(
                    disp_width,
                    disp_height,
                    cat33_image_width,
                    cat33_image_height,
                    &resized_offset_x,
                    &resized_offset_y,
                    &resized_width,
                    &resized_height);

                /* for images of other formats, try to set center alignment */
            #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
                ret = gdi_image_draw_animation_resized_file(
                        resized_offset_x,
                        resized_offset_y,
                        resized_width,
                        resized_height,
                        (PS8) idle_screen_wallpaper_name,
                        &animation_handle);
                /* ret = gdi_image_draw_resized_file(resized_offset_x, resized_offset_y, resized_width, resized_height, (PS8)idle_screen_wallpaper_name); */
            #else /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
                ret = gdi_image_draw_resized_file(
                        resized_offset_x,
                        resized_offset_y,
                        resized_width,
                        resized_height,
                        (PS8) idle_screen_wallpaper_name);
            #endif /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
            }
        }
        /* 102406 DRM check End */

        mmi_trace(g_sw_WpSsPwon, "gdm: in draw_wallpaper(), ret=%d",ret);
        if (ret < 0 && ret != GDI_GIF_ERR_OUT_OF_CLIP_REGION && ret != GDI_BMP_ERR_OUT_OF_CLIP_REGION)
        {
             
            PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "draw_wallpaper():[$$$]GDI_RESULT:%d[$$$]\n", ret));
            mmi_trace(g_sw_WpSsPwon, "gdm: in draw_wallpaper(0, call PhnsetWallpaperBadFileCallBack()");
            PhnsetWallpaperBadFileCallBack(0);
            idle_screen_wallpaper = get_image(idle_screen_wallpaper_ID);
            cat33_n_frames = gui_image_n_frames(idle_screen_wallpaper);
            gdi_image_get_dimension(idle_screen_wallpaper, &cat33_image_width, &cat33_image_height);
            cat33_image_x = (disp_width - cat33_image_width) >> 1;
            cat33_image_y = (disp_height - cat33_image_height) >> 1;
            if (cat33_n_frames > 1)
        #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
                gdi_image_draw_animation_frames(
                    cat33_image_x,
                    cat33_image_y,
                    idle_screen_wallpaper,
                    &animation_handle,
                    (S16) cat33_frame_number);
        #else /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
                gdi_image_draw_animation_single_frame(
                    cat33_image_x,
                    cat33_image_y,
                    idle_screen_wallpaper,
                    (S16) cat33_frame_number);
        #endif /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
            else
            {
                gdi_image_draw(cat33_image_x, cat33_image_y, idle_screen_wallpaper);
            }
        }
    }
}


#ifdef __MMI_SAVE_SS_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_SS_SPEEDUP__  20091125*/
/*****************************************************************************
 * FUNCTION
 *  SetIdleScreenSaveName
 * DESCRIPTION
 *  
 * PARAMETERS
 *  name        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetIdleScreenSaveName(UI_string_type name)
{
  /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 disp_height = UI_device_height, disp_width = UI_device_width;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    idle_screen_screensaver_ID = 0;
    idle_screen_type = NORMAL_IDLE_SCREEN;

#ifndef __MMI_IDLE_FULL_SCREEN__
    disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
    disp_width = UI_device_width - 1;
#endif /* __MMI_IDLE_FULL_SCREEN__ */ 

mmi_trace(g_sw_WpSsPwon, "gdm: into SetIdleScreenSaveName(), disp_height=%d, disp_width=%d", disp_height, disp_width);
mmi_trace(g_sw_WpSsPwon, "gdm: cat166_image_width=%d, cat166_image_height=%d", cat166_image_width, cat166_image_height);
    gdi_image_get_dimension_file((S8*) name, &cat166_image_width, &cat166_image_height);
mmi_trace(g_sw_WpSsPwon, "gdm: cat166_image_width=%d, cat166_image_height=%d", cat166_image_width, cat33_image_height);
    cat166_image_offset_x= (disp_width - cat166_image_width) >> 1;   
    cat166_image_offset_y= (disp_height - cat166_image_width) >> 1;
mmi_trace(g_sw_WpSsPwon, "gdm: cat166_image_offset_x=%d, cat166_image_offset_y=%d,cat166_n_frames=%d", cat166_image_offset_x, cat166_image_offset_y,cat166_n_frames);

    gdi_image_get_frame_count_file((U8 *)name, &cat166_n_frames);
mmi_trace(g_sw_WpSsPwon, "gdm: cat166_n_frames=%d", cat166_n_frames);

    pfnUnicodeStrcpy(idle_screen_screensaver_name_original, (S8*) name);
    idle_screen_screensaver_name = idle_screen_screensaver_name_original;
    
}

/*****************************************************************************
 * FUNCTION
 *  draw_screensave
 * DESCRIPTION
 *  Redraws the Idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_screensaver(void)
{
    PU8 idle_screen_screensaver = get_image(idle_screen_screensaver_ID);
    S32 disp_height = UI_device_height, disp_width = UI_device_width;

    GDI_RESULT ret = GDI_SUCCEED;
    mmi_trace(g_sw_WpSsPwon, "gdm: into draw_screensave()");

#if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
    if (animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(animation_handle);
        animation_handle = GDI_ERROR_HANDLE;
    }
#endif

    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, current_MMI_theme->general_background_filler);

#ifndef __MMI_IDLE_FULL_SCREEN__
    disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
    disp_width = UI_device_width - 1;
    gui_set_clip(0, MMI_status_bar_height, UI_device_width - 1, UI_device_height - MMI_button_bar_height);
#endif 

    if (idle_screen_screensaver_ID != 0)  /* image comes from resource */
    {
  		mmi_trace(g_sw_WpSsPwon, "gdm: in draw_screensave(): default resource: cat166_n_frames=%d",cat166_n_frames);
      if (cat166_n_frames > 1)
    #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
            gdi_image_draw_animation_frames(
                cat166_image_offset_x,
                cat166_image_offset_y,
                idle_screen_screensaver,
                &animation_handle,
                (S16) cat166_frame_number);
    #else 
    
        {
            gdi_image_draw_animation_single_frame(
                cat166_image_offset_x,
                cat166_image_offset_y,
                idle_screen_screensaver,
                (S16) cat166_frame_number);
            cat166_animation_delay = gdi_image_get_animation_delay();
        }
    #endif 
    
      else
      {
          gdi_image_draw(cat166_image_offset_x, cat166_image_offset_y, idle_screen_screensaver);
      }
   }
   else
   {
        FS_HANDLE originalPath = 0;
    #ifdef __DRM_SUPPORT__
        kal_bool DRM_ret = KAL_FALSE;
    #endif

        if (cat166_n_frames <= 1)//check if the original file of the cached static image still exists and the DRM right as well
        {
            if (idle_screen_screensaver_name_original == NULL)
            {
                ret = GDI_IMAGE_ERR_INVALID_FILE;
            }
            else
            {
            #ifdef __DRM_SUPPORT__
                originalPath = DRM_open_file((PU16) idle_screen_screensaver_name_original, FS_READ_ONLY | FS_OPEN_NO_DIR, DRM_PERMISSION_DISPLAY);
                if (originalPath >= 0)
                {
                    DRM_ret = DRM_validate_permission(originalPath, DRM_PERMISSION_DISPLAY);
                }
                if (DRM_ret == KAL_FALSE)
            #else
                originalPath = MMI_FS_Open((U8 *) idle_screen_screensaver_name_original, FS_READ_ONLY | FS_OPEN_NO_DIR);
                if (originalPath < 0)
            #endif
                {
                    ret = GDI_IMAGE_ERR_INVALID_FILE;
                }
                if (originalPath >= 0)
                {
                #ifdef __DRM_SUPPORT__
                    DRM_close_file(originalPath);
                #else
                    MMI_FS_Close(originalPath);
                #endif
                    originalPath = 0;
                }
             }
        }
        
        if (ret != GDI_IMAGE_ERR_INVALID_FILE)
        {
            S32 offset_x = 0;
            S32 offset_y = 0;

            gdi_image_get_dimension_file(idle_screen_screensaver_name, &cat166_image_width, &cat166_image_height);
            	mmi_trace(g_sw_WpSsPwon, "gdm: in draw_screensave(): cat166_image_width=%d, cat166_image_height=%d ", cat166_image_width,cat166_image_height);
            cat166_image_offset_x= (disp_width - cat166_image_width) >> 1;
            cat166_image_offset_y = (disp_height - cat166_image_height) >> 1;

            if ((disp_width >= cat166_image_width) && (disp_height >= cat166_image_height))       /* image can be displayed entirely */
            {
                /* align preview window at the center of screen */
                offset_x = (disp_width - cat166_image_width) >> 1;
                offset_y = (disp_height - cat166_image_height) >> 1;

            #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
            	mmi_trace(g_sw_WpSsPwon, "gdm: in draw_screensave(): user define ");
         		ret = gdi_image_draw_animation_file(
                                 offset_x,
                                 offset_y,
                                 (PS8) idle_screen_screensaver_name,
                                 &animation_handle);
            #else
                ret = gdi_image_draw_file(offset_x, offset_y, (PS8) idle_screen_screensaver_name);
            #endif 
            }
            
            else    /* image is larger than screen size */
            {
                S32 resized_offset_x;
                S32 resized_offset_y;
                S32 resized_width;
                S32 resized_height;

                gdi_image_util_fit_bbox(
                    disp_width,
                    disp_height,
                    cat166_image_width,
                    cat166_image_height,
                    &resized_offset_x,
                    &resized_offset_y,
                    &resized_width,
                    &resized_height);

            	mmi_trace(g_sw_WpSsPwon, "gdm: offset_x=%d, offset_y=%d,width=%d,height=%d",resized_offset_x,resized_offset_y,resized_width,resized_height);
                /* for images of other formats, try to set center alignment */
            #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
                ret = gdi_image_draw_animation_resized_file(
                        resized_offset_x,
                        resized_offset_y,
                        resized_width,
                        resized_height,
                        (PS8) idle_screen_screensaver_name,
                        &animation_handle);
                /* ret = gdi_image_draw_resized_file(resized_offset_x, resized_offset_y, resized_width, resized_height, (PS8)idle_screen_wallpaper_name); */
            #else /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
                ret = gdi_image_draw_resized_file(
                        resized_offset_x,
                        resized_offset_y,
                        resized_width,
                        resized_height,
                        (PS8) idle_screen_screensaver_name);
            #endif /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
            }
        }
        /* 102406 DRM check End */

mmi_trace(g_sw_WpSsPwon, "gdm: in draw_screensave(), ret=%d",ret);
        if (ret < 0 && ret != GDI_GIF_ERR_OUT_OF_CLIP_REGION && ret != GDI_BMP_ERR_OUT_OF_CLIP_REGION)
        {
             
            PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "draw_screensaver():[$$$]GDI_RESULT:%d[$$$]\n", ret));
            mmi_trace(g_sw_WpSsPwon, "gdm: in draw_screensave(), call PhnsetScreenSaverBadFileCallBack()");
            PhnsetScreenSaverBadFileCallBack();
            idle_screen_screensaver = get_image(idle_screen_screensaver_ID);
            cat166_n_frames = gui_image_n_frames(idle_screen_screensaver);
            gdi_image_get_dimension(idle_screen_screensaver, &cat166_image_width, &cat166_image_height);
            cat166_image_offset_x = (disp_width - cat166_image_width) >> 1;
            cat166_image_offset_y = (disp_height - cat166_image_height) >> 1;
            if (cat166_n_frames > 1)
        #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
                gdi_image_draw_animation_frames(
                    cat166_image_offset_x,
                    cat166_image_offset_y,
                    idle_screen_screensaver,
                    &animation_handle,
                    (S16) cat166_frame_number);
        #else 
                gdi_image_draw_animation_single_frame(
                    cat166_image_offset_x,
                    cat166_image_offset_y,
                    idle_screen_screensaver,
                    (S16) cat166_frame_number);
        #endif
            else
            {
                gdi_image_draw(cat166_image_offset_x, cat166_image_offset_y, idle_screen_screensaver);
            }
        }
    }
}
#endif

#ifdef __MMI_SAVE_PON_SPEEDUP__ /*gdm modify to add  the switch of __MMI_SAVE_PON_SPEEDUP__  20091125*/
/*****************************************************************************
 * FUNCTION
 *  SetIdlePowerOnName
 * DESCRIPTION
 *  
 * PARAMETERS
 *  name        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetIdlePowerOnName(UI_string_type name)
{
  /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 disp_height = UI_device_height, disp_width = UI_device_width;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    idle_screen_poweron_ID = 0;
    idle_screen_type = NORMAL_IDLE_SCREEN;

#ifndef __MMI_IDLE_FULL_SCREEN__
    disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
    disp_width = UI_device_width - 1;
#endif /* __MMI_IDLE_FULL_SCREEN__ */ 

mmi_trace(g_sw_WpSsPwon, "gdm: into SetIdlePowerOnName(), disp_height=%d, disp_width=%d", disp_height, disp_width);
mmi_trace(g_sw_WpSsPwon, "gdm: cat166_image_width=%d, cat166_image_height=%d", cat166_image_width, cat166_image_height);
    gdi_image_get_dimension_file((S8*) name, &cat166_image_width, &cat166_image_height);
mmi_trace(g_sw_WpSsPwon, "gdm: cat166_image_width=%d, cat166_image_height=%d", cat166_image_width, cat166_image_height);
    cat166_image_offset_x= (disp_width - cat166_image_width) >> 1;   
    cat166_image_offset_y= (disp_height - cat166_image_width) >> 1;
mmi_trace(g_sw_WpSsPwon, "gdm: cat166_image_offset_x=%d, cat166_image_offset_y=%d,cat166_n_frames=%d", cat166_image_offset_x, cat166_image_offset_y,cat166_n_frames);

    gdi_image_get_frame_count_file((U8 *)name, &cat166_n_frames);
mmi_trace(g_sw_WpSsPwon, "gdm: cat166_n_frames=%d", cat166_n_frames);

    pfnUnicodeStrcpy(idle_screen_poweron_name_original, (S8*) name);
    idle_screen_poweron_name = idle_screen_poweron_name_original;
    
}

/*****************************************************************************
 * FUNCTION
 *  draw_poweron
 * DESCRIPTION
 *  Redraws the poweron animation
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 extern U8 mmi_fmgr_get_file_group (S8 *path);
extern pBOOL PhnsetRstPwrOnDisplayDefault(void);

void draw_poweron(void)
{
    PU8 idle_screen_power= get_image(idle_screen_poweron_ID);
    S32 disp_height = UI_device_height, disp_width = UI_device_width;

    GDI_RESULT ret = GDI_SUCCEED;
    mmi_trace(g_sw_WpSsPwon, "gdm: into draw_poweron()");

#if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
    if (animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(animation_handle);
        animation_handle = GDI_ERROR_HANDLE;
    }
#endif

    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, current_MMI_theme->general_background_filler);

#ifndef __MMI_IDLE_FULL_SCREEN__
    disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
    disp_width = UI_device_width - 1;
    gui_set_clip(0, MMI_status_bar_height, UI_device_width - 1, UI_device_height - MMI_button_bar_height);
#endif 

    if (idle_screen_poweron_ID!= 0)  /* image comes from resource */
    {
  		mmi_trace(g_sw_WpSsPwon, "gdm: in draw_poweron(): default resource: cat166_n_frames=%d",cat166_n_frames);
      if (cat166_n_frames > 1)
    #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
            gdi_image_draw_animation_frames(
                cat166_image_offset_x,
                cat166_image_offset_y,
                idle_screen_power,
                &animation_handle,
                (S16) cat166_frame_number);
    #else 
    
        {
            gdi_image_draw_animation_single_frame(
                cat166_image_offset_x,
                cat166_image_offset_y,
                idle_screen_power,
                (S16) cat166_frame_number);
            cat166_animation_delay = gdi_image_get_animation_delay();
        }
    #endif 
    
      else
      {
          gdi_image_draw(cat166_image_offset_x, cat166_image_offset_y, idle_screen_power);
      }
   }
   else
   {
        FS_HANDLE originalPath = 0;
    #ifdef __DRM_SUPPORT__
        kal_bool DRM_ret = KAL_FALSE;
    #endif

        if (cat166_n_frames <= 1)//check if the original file of the cached static image still exists and the DRM right as well
        {
            if (idle_screen_poweron_name_original == NULL)
            {
                ret = GDI_IMAGE_ERR_INVALID_FILE;
            }
            else
            {
            #ifdef __DRM_SUPPORT__
                originalPath = DRM_open_file((PU16) idle_screen_poweron_name_original, FS_READ_ONLY | FS_OPEN_NO_DIR, DRM_PERMISSION_DISPLAY);
                if (originalPath >= 0)
                {
                    DRM_ret = DRM_validate_permission(originalPath, DRM_PERMISSION_DISPLAY);
                }
                if (DRM_ret == KAL_FALSE)
            #else
                originalPath = MMI_FS_Open((U8 *) idle_screen_poweron_name_original, FS_READ_ONLY | FS_OPEN_NO_DIR);
                if (originalPath < 0)
            #endif
                {
                    ret = GDI_IMAGE_ERR_INVALID_FILE;
                }
                if (originalPath >= 0)//gaodm 081105
                {
                #ifdef __DRM_SUPPORT__
                    DRM_close_file(originalPath);
                #else
                    MMI_FS_Close(originalPath);
                #endif
                    originalPath = 0;
                }
             }
        }
        
        if (ret != GDI_IMAGE_ERR_INVALID_FILE&&mmi_fmgr_get_file_group(idle_screen_poweron_name)!=FMGR_GROUP_VIDEO)//chenhe add 081218 for bug 10662.we shouldn't get fiel info by gdiImage function,and the function has L problem to be resolved
        {
            S32 offset_x = 0;
            S32 offset_y = 0;

            gdi_image_get_dimension_file(idle_screen_poweron_name, &cat166_image_width, &cat166_image_height);
            	mmi_trace(g_sw_WpSsPwon, "gdm: in draw_poweron(): cat166_image_width=%d, cat166_image_height=%d ", cat166_image_width,cat166_image_height);
            cat166_image_offset_x= (disp_width - cat166_image_width) >> 1;
            cat166_image_offset_y = (disp_height - cat166_image_height) >> 1;

            if ((disp_width >= cat166_image_width) && (disp_height >= cat166_image_height))       /* image can be displayed entirely */
            {
                /* align preview window at the center of screen */
                offset_x = (disp_width - cat166_image_width) >> 1;
                offset_y = (disp_height - cat166_image_height) >> 1;

            #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
            	mmi_trace(g_sw_WpSsPwon, "gdm: in draw_poweron(): user define ");
         		ret = gdi_image_draw_animation_file(
                                 offset_x,
                                 offset_y,
                                 (PS8) idle_screen_poweron_name,
                                 &animation_handle);
            #else
                ret = gdi_image_draw_file(offset_x, offset_y, (PS8) idle_screen_poweron_name);
            #endif 
            }
            
            else    /* image is larger than screen size */
            {
                S32 resized_offset_x;
                S32 resized_offset_y;
                S32 resized_width;
                S32 resized_height;

                gdi_image_util_fit_bbox(
                    disp_width,
                    disp_height,
                    cat166_image_width,
                    cat166_image_height,
                    &resized_offset_x,
                    &resized_offset_y,
                    &resized_width,
                    &resized_height);

            	mmi_trace(g_sw_WpSsPwon, "gdm: offset_x=%d, offset_y=%d,width=%d,height=%d",resized_offset_x,resized_offset_y,resized_width,resized_height);
                /* for images of other formats, try to set center alignment */
            #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
                ret = gdi_image_draw_animation_resized_file(
                        resized_offset_x,
                        resized_offset_y,
                        resized_width,
                        resized_height,
                        (PS8) idle_screen_poweron_name,
                        &animation_handle);
            #else /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
                ret = gdi_image_draw_resized_file(
                        resized_offset_x,
                        resized_offset_y,
                        resized_width,
                        resized_height,
                        (PS8) idle_screen_poweron_name);
            #endif /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
            }
        }
        /* 102406 DRM check End */

      	mmi_trace(g_sw_WpSsPwon, "gdm: in draw_poweron(): ret=%d ", ret);
        if (ret < 0 && ret != GDI_GIF_ERR_OUT_OF_CLIP_REGION && ret != GDI_BMP_ERR_OUT_OF_CLIP_REGION)
        {
             
            PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "draw_poweron():[$$$]GDI_RESULT:%d[$$$]\n", ret));
            mmi_trace(g_sw_WpSsPwon, "gdm: in draw_poweron(), call PhnsetRstPwrOnDisplayDefault()");
            PhnsetRstPwrOnDisplayDefault();
            idle_screen_power = get_image(idle_screen_poweron_ID);
            cat166_n_frames = gui_image_n_frames(idle_screen_power);
            gdi_image_get_dimension(idle_screen_power, &cat166_image_width, &cat166_image_height);
            cat166_image_offset_x = (disp_width - cat166_image_width) >> 1;
            cat166_image_offset_y = (disp_height - cat166_image_height) >> 1;
            if (cat166_n_frames > 1)
        #if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
                gdi_image_draw_animation_frames(
                    cat166_image_offset_x,
                    cat166_image_offset_y,
                    idle_screen_power,
                    &animation_handle,
                    (S16) cat166_frame_number);
        #else 
                gdi_image_draw_animation_single_frame(
                    cat166_image_offset_x,
                    cat166_image_offset_y,
                    idle_screen_power,
                    (S16) cat166_frame_number);
        #endif
            else
            {
                gdi_image_draw(cat166_image_offset_x, cat166_image_offset_y, idle_screen_power);
            }
        }
    }
}
#endif

/*****************************************************************************
 * FUNCTION
 *  wgui_stop_wallpaper
 * DESCRIPTION
 *  Stop wallpaper animation
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_stop_wallpaper(void)
{
    animation_handle = cat129_get_image_handle();
    if (animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(animation_handle);
        animation_handle = GDI_ERROR_HANDLE;
    }

        animation_handle = cat222_get_image_handle();
    if (animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(animation_handle);
        animation_handle = GDI_ERROR_HANDLE;
    }
    
}

#ifdef __MMI_SAVE_WP_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_WP_SPEEDUP__*/
/*****************************************************************************
 * FUNCTION
 *  wgui_save_wallpaper_to_speedup
 * DESCRIPTION
 *  Save the layer of wallpaper as fast format
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 14:30 */
void wgui_save_wallpaper_to_speedup(void/*MMI_BOOL is_default*/)
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 14:30 */
{
//#ifdef GDI_USING_LAYER//{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    U16 bg_img_id;
    S8 *bg_img_name;
    S32 bg_img_x, bg_img_y;
    U8 bg_img_opacity;
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
    U8 *outside_mem = NULL;
    gdi_handle temp_wallpaper_handle = GDI_NULL_HANDLE;
    S32 disp_height = UI_device_height, disp_width = UI_device_width;
    U16 type;
    MMI_BOOL isFile = MMI_FALSE;
	FS_DiskInfo disk_info;
	S32	fs_ret;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "wgui_save_wallpaper_to_speedup"));
    
    mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_wallpaper_to_speedup(), idle_screen_wallpaper_ID=%d", idle_screen_wallpaper_ID);
    if (idle_screen_wallpaper_ID)
    {
        type = gdi_image_get_type_from_id(idle_screen_wallpaper_ID);
    }
    else
    {
        type = gdi_image_get_type_from_file(idle_screen_wallpaper_name);
        isFile = MMI_TRUE;
    }
    mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_wallpaper_to_speedup(), type=%d, isFile=%d,cat33_n_frames=%d", type,isFile,cat33_n_frames);
    wgui_stop_wallpaper();
    if ((isFile && cat33_n_frames <= 1) ||
        type == GDI_IMAGE_TYPE_M3D ||
        type == GDI_IMAGE_TYPE_M3D_FILE || 
        (type == GDI_IMAGE_TYPE_GIF && cat33_n_frames > 1) ||
        type == GDI_IMAGE_TYPE_GIF_FILE)
    {

        outside_mem = (U8 *)mmi_frm_scrmem_alloc(((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3));
        if (outside_mem)
            gdi_layer_create_using_outside_memory(0, 0, UI_device_width, UI_device_height, &temp_wallpaper_handle, outside_mem, ((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3));
        else
	     temp_wallpaper_handle = dm_get_layer_handle(0);

        /* redraw the animation to get the first frame */
        #ifdef __GDI_MEMORY_PROFILE_2__
            gdi_layer_push_and_set_active(temp_wallpaper_handle);
            
            gdi_layer_clear(GDI_COLOR_WHITE);
        #endif /* __GDI_MEMORY_PROFILE_2__ */ 
            
            draw_wallpaper();
        /* 
         * 1. idle_screen_wallpaper_ID == 0 means PhnsetWallpaperBadFileCallBack was not called 
         * 2. is file, just cache it no matter what type it is
         */
        #if 0 // gdm 080623 for gif file
        if( (idle_screen_wallpaper_ID <= 1 || !isFile) )
        #else
        if( (idle_screen_wallpaper_ID <= 1 || !isFile)  && (cat33_n_frames <= 1) && (type != GDI_IMAGE_TYPE_GIF_FILE))
        #endif
        {
		        fs_ret = MMI_FS_GetDiskInfo("E", &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);//add gdm 091123,if space enough to save pbm file 
			 if( (fs_ret >= 0) && (disk_info.FreeClusters >= UI_device_height*UI_device_width*2) )
			 {
		            if ((0 <= gdi_layer_save_pbm_file(temp_wallpaper_handle, (S8*) PHNSET_PREDEFINED_WP_PATH_PBM)))
		            {
		                mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_wallpaper_to_speedup(), come here pbm");
		                idle_screen_wallpaper_name = (S8*) PHNSET_PREDEFINED_WP_PATH_PBM;
		            }
		            else if (0 <= gdi_layer_save_jpeg_file(temp_wallpaper_handle, (S8*) PHNSET_PREDEFINED_WP_PATH_JPG))
		            {
		                mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_wallpaper_to_speedup(), come here jpg");
		                idle_screen_wallpaper_name = (S8*) PHNSET_PREDEFINED_WP_PATH_JPG;
		            }
		            else
		            {
		                ;   /* use original file. */
		            }
			}
				 
            /* To tell list menu the image size */
            if (idle_screen_wallpaper_name == (S8*) PHNSET_PREDEFINED_WP_PATH_PBM ||
                idle_screen_wallpaper_name == (S8*) PHNSET_PREDEFINED_WP_PATH_JPG)
            {
            #ifndef __MMI_IDLE_FULL_SCREEN__
                disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
                disp_width = UI_device_width - 1;
            #endif /* __MMI_IDLE_FULL_SCREEN__ */ 
                cat33_image_width = disp_width;
                cat33_image_height = disp_height;
                
            }
        }

            
        #ifdef __GDI_MEMORY_PROFILE_2__
            gdi_layer_pop_and_restore_active();
        #endif 

        if (dm_get_layer_handle(0) != temp_wallpaper_handle)
        {
            gdi_layer_free(temp_wallpaper_handle);
        }

        if (outside_mem)
            mmi_frm_scrmem_free(outside_mem);

    #ifdef __MMI_WALLPAPER_ON_BOTTOM__
        idle_screen_cache_name = idle_screen_wallpaper_name;
        idle_screen_cache_id = idle_screen_wallpaper_ID;
   
        /* make listmenu apply still image */
        dm_set_scr_bg_redraw(MMI_TRUE);
        dm_get_scr_bg_image(&bg_img_id, &bg_img_name, &bg_img_x, &bg_img_y, &bg_img_opacity);
        dm_set_scr_bg_image(idle_screen_cache_id, idle_screen_wallpaper_name, bg_img_x, bg_img_y, bg_img_opacity);
    #endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 

    
        #if 0//ggdm 080623 for gif file
        if (cat33_n_frames > 1 || type == GDI_IMAGE_TYPE_SVG_FILE)
        {
            /* make idlescreen apply animate image */
            idle_screen_wallpaper_name = idle_screen_wallpaper_name_original;
        }
        #endif
    }
//#endif//}
}
#endif

#ifdef __MMI_SAVE_SS_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_SS_SPEEDUP__  20091125*/
/*****************************************************************************
 * FUNCTION
 *  wgui_save_ScreenSave_to_speedup
 * DESCRIPTION
 *  Save the layer of wallpaper as fast format
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern const U16 PHNSET_PREDEFINED_SS_PATH_JPG[];
extern const U16 PHNSET_PREDEFINED_SS_PATH_PBM[];
void wgui_save_ScreenSave_to_speedup(void)
{
    U8 *outside_mem = NULL;
    gdi_handle temp_wallpaper_handle = GDI_NULL_HANDLE;
    S32 disp_height = UI_device_height, disp_width = UI_device_width;
    U16 type;
    MMI_BOOL isFile = MMI_FALSE;
	FS_DiskInfo disk_info;
	S32	fs_ret;

    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "wgui_save_ScreenSave_to_speedup"));
    
mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_ScreenSave_to_speedup(), idle_screen_screensaver_ID=%d", idle_screen_screensaver_ID);
    if (idle_screen_screensaver_ID)
    {
        type = gdi_image_get_type_from_id(idle_screen_screensaver_ID);
    }
    else
    {
        type = gdi_image_get_type_from_file(idle_screen_screensaver_name);
        isFile = MMI_TRUE;
    }
mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_ScreenSave_to_speedup(), type=%d, isFile=%d,cat166_n_frames=%d", type,isFile,cat166_n_frames);
    wgui_stop_wallpaper();
    if ((isFile && cat166_n_frames <= 1) ||
        type == GDI_IMAGE_TYPE_M3D ||
        type == GDI_IMAGE_TYPE_M3D_FILE || 
        (type == GDI_IMAGE_TYPE_GIF && cat166_n_frames > 1) ||
        type == GDI_IMAGE_TYPE_GIF_FILE)
    {

        outside_mem = (U8 *)mmi_frm_scrmem_alloc(((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3));
        if (outside_mem)
            gdi_layer_create_using_outside_memory(0, 0, UI_device_width, UI_device_height, &temp_wallpaper_handle, outside_mem, ((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3));
        else
            temp_wallpaper_handle = dm_get_layer_handle(0);

        /* redraw the animation to get the first frame */
        #ifdef __GDI_MEMORY_PROFILE_2__
            gdi_layer_push_and_set_active(temp_wallpaper_handle);
            
            gdi_layer_clear(GDI_COLOR_WHITE);
        #endif /* __GDI_MEMORY_PROFILE_2__ */ 
            
            draw_screensaver();
        /* 
         * 1. idle_screen_screensaver_ID == 0 means PhnsetScreenSaverBadFileCallBack was not called 
         * 2. is file, just cache it no matter what type it is
         */
        if (idle_screen_screensaver_ID <= 1 || !isFile)  
        {
				fs_ret = MMI_FS_GetDiskInfo("C", &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);//add gdm 091123,if space enough to save pbm file 
				if( (fs_ret >= 0) && (disk_info.FreeClusters >= UI_device_height*UI_device_width*2) )
				{
			            if (
                            #ifdef __MMI_FS_QM_SUPPORT__
								((gdi_layer_get_bit_per_pixel() * UI_device_width * UI_device_height + 13) <
								MMI_FS_QmGetFree((BYTE*) "Z:\\@USER\\")) &&
                            #endif /* __MMI_FS_QM_SUPPORT__ */ 
			                   (0 <= gdi_layer_save_pbm_file(temp_wallpaper_handle, (S8*) PHNSET_PREDEFINED_SS_PATH_PBM)))
			            {
								mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_ScreenSave_to_speedup(), come here pbm");
								idle_screen_screensaver_name = (S8*) PHNSET_PREDEFINED_SS_PATH_PBM;
			            }
			            else if (0 <= gdi_layer_save_jpeg_file(temp_wallpaper_handle, (S8*) PHNSET_PREDEFINED_SS_PATH_JPG))
			            {
								mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_ScreenSave_to_speedup(), come here jpg");
								idle_screen_screensaver_name = (S8*) PHNSET_PREDEFINED_SS_PATH_JPG;
			            }
			            else
			            {
								;   /* use original file. */
			            }
				}

            if (idle_screen_screensaver_name == (S8*) PHNSET_PREDEFINED_SS_PATH_JPG ||
                idle_screen_screensaver_name == (S8*) PHNSET_PREDEFINED_SS_PATH_PBM)
            {
            #ifndef __MMI_IDLE_FULL_SCREEN__
                disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
                disp_width = UI_device_width - 1;
            #endif /* __MMI_IDLE_FULL_SCREEN__ */ 
                cat166_image_width = disp_width;
                cat166_image_height = disp_height;
            }
            
        }

        #ifdef __GDI_MEMORY_PROFILE_2__
            gdi_layer_pop_and_restore_active();
        #endif 

        if (dm_get_layer_handle(0) != temp_wallpaper_handle)
        {
            gdi_layer_free(temp_wallpaper_handle);
        }

        if (outside_mem)
            mmi_frm_scrmem_free(outside_mem);
    
        if (cat166_n_frames > 1 || type != GDI_IMAGE_TYPE_BMP_FILE || type != GDI_IMAGE_TYPE_JPG_FILE )
        {
            idle_screen_screensaver_name = idle_screen_screensaver_name_original;
        }
        
    }
    
}

#endif

#ifdef __MMI_SAVE_PON_SPEEDUP__ /*gdm modify to add  the switch of __MMI_SAVE_PON_SPEEDUP__  20091125*/
/*****************************************************************************
 * FUNCTION
 *  wgui_save_PowerON_to_speedup
 * DESCRIPTION
 *  Save the layer of poweron as fast format
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern const U16 PHNSET_PREDEFINED_PWO_PATH_JPG[]; 
extern const U16 PHNSET_PREDEFINED_PWO_PATH_PBM[];
void wgui_save_PowerON_to_speedup(void)
{
    U8 *outside_mem = NULL;
    gdi_handle temp_wallpaper_handle = GDI_NULL_HANDLE;
    S32 disp_height = UI_device_height, disp_width = UI_device_width;
    U16 type;
    MMI_BOOL isFile = MMI_FALSE;
	FS_DiskInfo disk_info;
	S32	fs_ret;

    PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "wgui_save_PowerON_to_speedup"));
    
mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_PowerON_to_speedup(), idle_screen_poweron_ID=%d", idle_screen_poweron_ID);
    if (idle_screen_poweron_ID)
    {
        type = gdi_image_get_type_from_id(idle_screen_poweron_ID);
    }
    else
    {
        type = gdi_image_get_type_from_file(idle_screen_poweron_name);
        isFile = MMI_TRUE;
    }
mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_PowerON_to_speedup(), type=%d, isFile=%d,cat166_n_frames=%d", type,isFile,cat166_n_frames);
    wgui_stop_wallpaper();
    if ((isFile && cat166_n_frames <= 1) ||
        type == GDI_IMAGE_TYPE_M3D ||
        type == GDI_IMAGE_TYPE_M3D_FILE || 
        (type == GDI_IMAGE_TYPE_GIF && cat166_n_frames > 1) ||
        type == GDI_IMAGE_TYPE_GIF_FILE)
    {

        outside_mem = (U8 *)mmi_frm_scrmem_alloc(((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3));
        if (outside_mem)
            gdi_layer_create_using_outside_memory(0, 0, UI_device_width, UI_device_height, &temp_wallpaper_handle, outside_mem, ((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3));
        else
            temp_wallpaper_handle = dm_get_layer_handle(0);

        /* redraw the animation to get the first frame */
        #ifdef __GDI_MEMORY_PROFILE_2__
            gdi_layer_push_and_set_active(temp_wallpaper_handle);
            
            gdi_layer_clear(GDI_COLOR_WHITE);
        #endif /* __GDI_MEMORY_PROFILE_2__ */ 
            
            draw_poweron();
        /* 
         * 1. idle_screen_poweron == 0 means PhnsetRstPwrOnDisplayDefault was not called 
         * 2. is file, just cache it no matter what type it is
         */
        if (idle_screen_poweron_ID <= 1 || !isFile)  
        {
				fs_ret = MMI_FS_GetDiskInfo("C", &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);//add gdm 091123,if space enough to save pbm file 
				if( (fs_ret >= 0) && (disk_info.FreeClusters >= UI_device_height*UI_device_width*2) )
				{
		            if (
		                  #ifdef __MMI_FS_QM_SUPPORT__
		                   ((gdi_layer_get_bit_per_pixel() * UI_device_width * UI_device_height + 13) <
		                    MMI_FS_QmGetFree((BYTE*) "Z:\\@USER\\")) &&
		                   #endif /* __MMI_FS_QM_SUPPORT__ */ 
		                   (0 <= gdi_layer_save_pbm_file(temp_wallpaper_handle, (S8*) PHNSET_PREDEFINED_PWO_PATH_PBM)))
		            {
							mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_poweron_to_speedup(), come here pbm");
							idle_screen_poweron_name = (S8*) PHNSET_PREDEFINED_PWO_PATH_PBM;
		            }
		            else if (0 <= gdi_layer_save_jpeg_file(temp_wallpaper_handle, (S8*) PHNSET_PREDEFINED_PWO_PATH_JPG))
		            {
							mmi_trace(g_sw_WpSsPwon, "gdm: into wgui_save_poweron_to_speedup(), come here jpg");
							idle_screen_poweron_name = (S8*) PHNSET_PREDEFINED_PWO_PATH_JPG;
		            }
		            else
		            {
		                ;   /* use original file. */
		            }
            
			}
				
            if (idle_screen_poweron_name == (S8*) PHNSET_PREDEFINED_PWO_PATH_JPG ||
                idle_screen_poweron_name == (S8*) PHNSET_PREDEFINED_PWO_PATH_PBM)
            {
            #ifndef __MMI_IDLE_FULL_SCREEN__
                disp_height = UI_device_height - MMI_status_bar_height - MMI_button_bar_height;
                disp_width = UI_device_width - 1;
            #endif /* __MMI_IDLE_FULL_SCREEN__ */ 
                cat166_image_width = disp_width;
                cat166_image_height = disp_height;
            }
            
        }

        #ifdef __GDI_MEMORY_PROFILE_2__
            gdi_layer_pop_and_restore_active();
        #endif 

        if (dm_get_layer_handle(0) != temp_wallpaper_handle)
        {
            gdi_layer_free(temp_wallpaper_handle);
        }

        if (outside_mem)
            mmi_frm_scrmem_free(outside_mem);
    
        if (cat166_n_frames > 1 || type != GDI_IMAGE_TYPE_BMP_FILE || type != GDI_IMAGE_TYPE_JPG_FILE )
        {
            idle_screen_poweron_name = idle_screen_poweron_name_original;
        }
        
    }
    
}

#endif
/*****************************************************************************
* FUNCTION
*  wgui_get_wallpaper_frame_count
* DESCRIPTION
*  Provide wallpaper frame count. 
* PARAMETERS
*	 void	
* RETURNS
*   void
*****************************************************************************/
MMI_BOOL wgui_check_wallpaper_frame_count(S8 *name, S8 **error_str)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 wallpaper_frames = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_image_gif_get_frame_count_file(name, &wallpaper_frames);
    PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "Wallpaper frame count = %d",  wallpaper_frames));

    /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 10:47 */
    if(wallpaper_frames > 30/*PHNSET_MAX_WALLPAPER_FRAME*/)
    {
        *error_str = (PS8)GetString(0/*STR_ID_WP_FRAME_ERR*/);
        /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 10:47 */
        return MMI_FALSE;
    }
    else
    {
        *error_str = NULL;
        return MMI_TRUE;
    }

}


 
#if defined(__MMI_NOKIA_STYLE_N800__)|| defined (__MMI_BLACKBERRY_IDLE_SHORTCUT__) 
//static S32 touch_idle_screen_highlighted_shortcut;
//static S32 touch_idle_screen_highlight_changed;

extern void mmi_phb_entry_main_menu(void);
extern void mmi_msg_entry_divided_inbox_menu(void);
extern void ClndrPreEntryApp(void);
extern void mmi_msg_entry_inbox_menu(void);
extern void mmi_msg_entry_inbox_list(void);
extern void mmi_camera_entry_app_screen(void);
extern void mmi_imgview_entry_app(void);
extern void MMI_dummy_function(void);
extern void mmi_audply_entry_main(void);
extern void mmi_fmrdo_run_app(void);
extern void MTPNP_jWap_mainMenu_entry_handler (void);
extern void jWap_mainMenu_entry_handler(void);
extern void EntryScrProfiles(void);

#if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
extern void EntryDMCHISTMainMenu(void);
extern void EntryMaterial(void);
#endif
#if defined(MMI_ON_WIN32)
void jmetoc_qq_entry(void)
{}
#else
extern void jmetoc_qq_entry(void);
#endif
/*****************************************************************************
 * FUNCTION
 *  setup_touch_idle_screen_shortcuts
 * DESCRIPTION
 *  This function is used to set up touch screen shortcuts
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 
 *  This routine is dependent on Order of Image Id's of Shortcut Item Images.
 *  DON'T CHANGE THE ORDER OF IMAGE ID's
 *****************************************************************************/
void setup_touch_idle_screen_shortcuts(void)////sa
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 image_width[MAX_IDLE_SCREEN_SHORTCUTS], image_height[MAX_IDLE_SCREEN_SHORTCUTS], i, total_width =
    0, x,y, x_margin;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifndef __MMI_DISABLE_DUAL_SENSOR__
    extern void camera_SwitchSensorReset();
    camera_SwitchSensorReset();
#endif
    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        gui_measure_image((PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + i)), &image_width[i], &image_height[i]);
        total_width += image_width[i];
        touch_idle_shortcuts[i].height = image_height[i];
        touch_idle_shortcuts[i].width = image_width[i];
    }

#ifdef __MMI_BLACKBERRY_QWERTY__
	touch_idle_shortcuts[0].entryfn = EntryDMCHISTMainMenu;
	touch_idle_shortcuts[1].entryfn = EntryMaterial;
	touch_idle_shortcuts[2].entryfn = mmi_phb_entry_main_menu;
    x_margin = (UI_device_width - total_width) /(MAX_IDLE_SCREEN_SHORTCUTS + 1); 
#else

    touch_idle_shortcuts[0].entryfn = ClndrPreEntryApp;
#ifdef __MMI_MULTI_SIM__
    touch_idle_shortcuts[1].entryfn = mmi_msg_entry_inbox_menu;
#else
    touch_idle_shortcuts[1].entryfn = mmi_msg_entry_inbox_list;
#endif
    touch_idle_shortcuts[2].entryfn = mmi_camera_entry_app_screen;
   // touch_idle_shortcuts[3].entryfn = jmetoc_qq_entry; should entry MSN
   #if defined( JATAAYU_SUPPORT ) && defined (JATAAYU_SUPPORT_WAP)
   #ifdef __MMI_MULTI_SIM__
        touch_idle_shortcuts[3].entryfn = MTPNP_jWap_mainMenu_entry_handler;
   #else
   	touch_idle_shortcuts[3].entryfn = jWap_mainMenu_entry_handler;
   #endif
    #else
       touch_idle_shortcuts[3].entryfn = mmi_imgview_entry_app;
    #endif
    #if defined(__MMI_NOKIA_STYLE_N800__) 
        touch_idle_shortcuts[4].entryfn = mmi_imgview_entry_app;
        touch_idle_shortcuts[5].entryfn = mmi_audply_entry_main;
        touch_idle_shortcuts[6].entryfn = mmi_fmrdo_run_app;
    #else
        touch_idle_shortcuts[4].entryfn = EntryScrProfiles;
    #endif
    x_margin = (UI_device_width - total_width) /(MAX_IDLE_SCREEN_SHORTCUTS - 1); 
#endif
    
    
#if defined(__MMI_NOKIA_STYLE_N800__)
    y = 90;//85;sa
    x = x_margin;
    x = 7;//sa

    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        if(i>4)
        {
        //mym modify 20091015
            if (r2lMMIFlag)
            {
                touch_idle_shortcuts[i].x = UI_DEVICE_WIDTH - 30;
            }
            else
            {
                touch_idle_shortcuts[i].x = 1;
            }
        //end mym
        touch_idle_shortcuts[i].y = y; //xiaoyao add 20071128 //UI_device_height - touch_idle_shortcuts[i].height;//rework by jiaquan
        y += touch_idle_shortcuts[i].height+ 2;
        }
        else
        {
            touch_idle_shortcuts[i].x = x;//x;
            touch_idle_shortcuts[i].y =50;//55;// UI_device_height - touch_idle_shortcuts[i].height;
            x += touch_idle_shortcuts[i].width + 3;//x_margin;
        }
    }
#else
    x = x_margin;
    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
            touch_idle_shortcuts[i].x = x; 
            touch_idle_shortcuts[i].y =MMI_BLACKBERRY_IDLE_SHRTCUT_IMAGE_Y;
            x += touch_idle_shortcuts[i].width + x_margin; 
    }
#endif
}
void display_font_idle_screen(S32 jia,S32 x1,S32 x2,S32 y1,S32 y2)//xiaoyao
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 str_w, str_h,jiayuan;
    S32 xx1, yy1, xx2, yy2;
//    U8 data = 0;
  //  S16 error = 0;

    xx1=x1;
    xx2=x2;
    yy1=y1;
    yy2=y2;
    jiayuan=jia;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_lock_double_buffer();
#endif 
    gui_push_text_clip();
    //  gui_set_font(&MMI_small_font);
#if 0 //def  __MMI_LANG_VIETNAMESE__
    ReadValue(NVRAM_SETTING_LANG, &data, DS_BYTE, &error);
    if(data == 3)
        gui_set_font(&UI_DEFAULT_FONT);//MMI_small_font  ,UI_DEFAULT_FONT
    else
        gui_set_font(&MMI_small_font);//MMI_small_font  ,UI_DEFAULT_FONT
#else
    gui_set_font(&UI_DEFAULT_FONT);//MMI_small_font  ,UI_DEFAULT_FONT  //danny 0523
#endif //__MMI_LANG_VIETNAMESE__//sa
    gui_measure_string((UI_string_type)GetString(jia), &str_w, &str_h);
    gui_set_text_color(gui_color(255,255, 255));
    gdi_layer_set_clip(xx1 - 2, yy1 - 2, xx2, yy2);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(UI_device_width -35, yy1);//(xx1+160, yy1);

    }
    else
    {
        gui_move_text_cursor(xx1, yy1);
    }
    gui_print_text(get_string(jiayuan));	
    gui_pop_text_clip();
#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(xx1 - 2, yy1 - 2, xx2+100, yy2);
#endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
}
#include "statusiconres.h"
#include "../../mmi_csdapp/FMRadio/include/fmradiodef.h"
/*****************************************************************************
 * FUNCTION
 *  display_touch_idle_screen_shortcuts
 * DESCRIPTION
 *  This function is used to draw touch screen shortcuts
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 
 *  This routine is dependent on Order of Image Id's of Shortcut Item Images.
 *  DON'T CHANGE THE ORDER OF IMAGE ID's
 *****************************************************************************/
extern void UI_draw_horizontal_line_type(S32 y1, S32 y2, S32 x, color_t c, S32 type_line);
extern S32 max_touch_idle_screen_shortcut_height(void);

void show_item_title(S32 x,S32 y)
{
	S32 string_width,string_height;

	gui_measure_string(MMI_title_string,&string_width,&string_height);
	if((x+string_width) >= T800_SHORTCUT_ITEM_STRING_AREA_END_X)
		return;
	gui_push_text_clip();   
	gui_set_text_clip(x, y,T800_SHORTCUT_ITEM_STRING_AREA_END_X, y +T800_SHORTCUT_ITEM_STRING_AREA_H +1);
	gdi_draw_solid_rect(x, y,T800_SHORTCUT_ITEM_STRING_AREA_END_X, y + T800_SHORTCUT_ITEM_STRING_AREA_H +1,GDI_COLOR_TRANSPARENT);	
#if	defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
	gui_set_text_color(gui_color(255,255,255));
#endif
	if(r2lMMIFlag)
		gui_move_text_cursor(x + string_width,y);
	else
		gui_move_text_cursor(x,y);
	gui_print_text(MMI_title_string);
	gui_pop_text_clip();
	gui_BLT_double_buffer(x,y,T800_SHORTCUT_ITEM_STRING_AREA_END_X,y + T800_SHORTCUT_ITEM_STRING_AREA_H +1);
}

scrolling_text short_cut_audply_title;
BOOL short_cut_audply_title_created = FALSE;
void mmi_shortcut_scroll_title_handler(void)
{
    if (short_cut_audply_title_created)
    {
        gui_handle_scrolling_text(&short_cut_audply_title);
    }
}
static void mmi_shortcut_redraw_title_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
}

void display_touch_idle_screen_shortcuts(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
  
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	gui_lock_double_buffer();
#if	!defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
	gdi_draw_solid_rect(0,
						45,
						UI_device_width,
						touch_idle_shortcuts[5].y,
						GDI_COLOR_TRANSPARENT);
#endif
	gui_idle_touch_screen_display_scroll_shorcut_item(curr_horizon_hilite_index);
#if 0	
    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS - 2; i++)
    {
        gui_show_image(
						touch_idle_shortcuts[i].x,
        				touch_idle_shortcuts[i].y,
        				get_image( (IMG_IDLE_MAINMENU_SHORCUT + i))
        				);	
    }
#endif	
    
 #if !defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
 	UI_draw_horizontal_line_type(0,240,46,gui_color(255,255,255),1);
    UI_draw_horizontal_line_type(0,240,82,gui_color(255,255,255),1);
	if(1 == curr_vertical_hilite_index)
	{
		gdi_draw_solid_rect(0,
						touch_idle_shortcuts[5].y,
						UI_device_width,
						touch_idle_shortcuts[5].y + touch_idle_shortcuts[5].height,
						GDI_COLOR_TRANSPARENT);
		gui_show_image(
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].x,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y,
						get_image(IMG_IDLE_MAINMENU1_SHORCUT_PRESSED)
						);
		
		if(mmi_audply_is_playing())
		{
			S32 w,character_height;
			gui_set_font( &MMI_medium_font );
			gui_measure_string(g_audply.title, &w, &character_height );
			if( w < (UI_device_width - 30))
			{
				gui_move_text_cursor(30,93);
				gui_print_text(g_audply.title);
			}
			else			
				gui_print_truncated_text(30,93,UI_device_width - 30,g_audply.title);
		}
		else
			display_font_idle_screen(STR_ID_AUDPLY_TITLE,30,UI_device_width - 1,95,95 + touch_idle_shortcuts[6].height);	
		MMI_title_string = (UI_string_type)get_string(STR_ID_AUDPLY_TITLE);	
	}
	else
	{
		gdi_draw_solid_rect(0,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y,
						UI_device_width,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y + touch_idle_shortcuts[5].height,
						GDI_COLOR_TRANSPARENT);
		gui_show_image(
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].x,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y,
						get_image(IMG_IDLE_MAINMENU_SHORCUT + 5)
						);
		if(mmi_audply_is_playing())
		{
			S32 w,character_height;
			gui_set_font( &MMI_medium_font );
			gui_measure_string(g_audply.title, &w, &character_height );
			if( w < (UI_device_width - 30))
			{
				gui_move_text_cursor(30,93);
				gui_print_text(g_audply.title);
			}
			else		
                    {
                                color_t f = {0,0,0,100} ;
                                color_t b = {255,255,255,100} ;
                                if(!short_cut_audply_title_created)
                                {
                                        gui_create_scrolling_text(
                                        &short_cut_audply_title,
                                        30,
                                        93,
                                        UI_device_width - 30,   /* -4 to prevent bordered wrods from exceeding the edge*/
                                        20,
                                        (UI_string_type) g_audply.title,
                                        mmi_shortcut_scroll_title_handler,
                                        mmi_shortcut_redraw_title_background,
                                        f,
                                        b);
                                        short_cut_audply_title_created = TRUE;
                                }
                                short_cut_audply_title.text_font = (UI_font_type)&MMI_medium_font;
                                short_cut_audply_title.scroll_size = 3; 
                                short_cut_audply_title.scroll_time = 200;  
                                short_cut_audply_title.scroll_pause = 5;    
                                short_cut_audply_title.flags |= UI_SCROLLING_TEXT_BORDERED_TEXT;
                                 gui_scrolling_text_stop(&short_cut_audply_title);
                                gui_show_scrolling_text(&short_cut_audply_title);

                            }         
				//gui_print_truncated_text(30,93,UI_device_width - 30,g_audply.title);
		}	
		else
			display_font_idle_screen(STR_ID_AUDPLY_TITLE,30,UI_device_width - 1,95,95 + touch_idle_shortcuts[6].height);
	}
	if(2 == curr_vertical_hilite_index)
	{
		gdi_draw_solid_rect(0,
						touch_idle_shortcuts[6].y,
						UI_device_width,
						touch_idle_shortcuts[6].y + touch_idle_shortcuts[6].height,
						GDI_COLOR_TRANSPARENT);
		gui_show_image(
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].x,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y,
						get_image(IMG_IDLE_MESSAGE1_SHORTCUT_PRESSED)
						);
		MMI_title_string = (UI_string_type)get_string(STR_ID_FMRDO_TITLE);	
	}
	else
	{
		gdi_draw_solid_rect(0,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y,
						UI_device_width,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].height,
						GDI_COLOR_TRANSPARENT);
		gui_show_image(
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].x,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y,
						get_image(IMG_IDLE_MAINMENU_SHORCUT + 6)
						);

	}
#else   // defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
/*
    if(curr_vertical_hilite_index)
		gdi_draw_solid_rect(T800_SHORTCUT_ITEM_STRING_START_X,
							  T800_SHORTCUT_ITEM_STRING_START_Y,
							  T800_SHORTCUT_ITEM_STRING_START_X+55,
							  T800_SHORTCUT_ITEM_STRING_START_Y+10,
							  GDI_COLOR_TRANSPARENT);//clear shortcut item string area when shortcut is not selected
	*/						  
    #endif

#if !defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
	display_font_idle_screen(STR_ID_FMRDO_TITLE,30,UI_device_width - 1,117,117 + touch_idle_shortcuts[6].height);
	//show_item_title(T800_SHORTCUT_ITEM_STRING_START_X,T800_SHORTCUT_ITEM_STRING_START_Y);
#endif
    gui_unlock_double_buffer();
#if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
	gui_BLT_double_buffer(0,
						T800_SHORTCUT_ITEM_STRING_START_Y,
						UI_device_width,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS-1].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS-1].height
						);	
#else

    gui_BLT_double_buffer(0, 45,UI_device_width,touch_idle_shortcuts[6].y + touch_idle_shortcuts[6].height);
#endif
}
#if !defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
void display_touch_idle_screen_shortcuts1(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, max_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        gui_show_image(
        touch_idle_shortcuts[i].x,
        touch_idle_shortcuts[i].y,
        get_image( (IMG_IDLE_MAINMENU_SHORCUT + i)));
        display_font_idle_screen(STR_ID_AUDPLY_TITLE,30,240,95,320);//
        display_font_idle_screen(STR_ID_FMRDO_TITLE,30,240,117,320);//
        UI_draw_horizontal_line_type(0,320,45,gui_color(255,255,255),1);
        UI_draw_horizontal_line_type(0,320,80,gui_color(255,255,255),1);
    }
    max_height = max_touch_idle_screen_shortcut_height();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, UI_device_height - max_height, UI_device_width, UI_device_height);
}
#endif

/*****************************************************************************
 * FUNCTION
 *  max_touch_idle_screen_shortcut_height
 * DESCRIPTION
 *  This function is used to find the maximum height of idlescreen shortcut
 *  among them.
 * PARAMETERS
 *  void
 * RETURNS
 *  S32 max_height
 
 *  
 *****************************************************************************/
S32 max_touch_idle_screen_shortcut_height(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, max_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* In case Image's are of different height, Find the maximum height image */
    max_height = touch_idle_shortcuts[0].height;

    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        if (max_height < touch_idle_shortcuts[i].height)
        {
            max_height = touch_idle_shortcuts[i].height;
        }
    }
    return (max_height);
}

#elif defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__)
#define MAX_IDLE_SCREEN_SHORTCUTS   4

typedef struct
{
    S32 x;
    S32 y;
    S32 width;
    S32 height;
    FuncPtr entryfn;
} idle_touch_screen_shortcut;

static idle_touch_screen_shortcut touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS];
static S32 touch_idle_screen_highlighted_shortcut;
static S32 touch_idle_screen_highlight_changed;

extern void mmi_phb_entry_main_menu(void);
extern void EntryScrMessagesMenuList(void);

 
extern void EntryMainMenuFromIdleScreen(void);

 


/*****************************************************************************
 * FUNCTION
 *  setup_touch_idle_screen_shortcuts
 * DESCRIPTION
 *  This function is used to set up touch screen shortcuts
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 
 *  This routine is dependent on Order of Image Id's of Shortcut Item Images.
 *  DON'T CHANGE THE ORDER OF IMAGE ID's
 *****************************************************************************/
void setup_touch_idle_screen_shortcuts(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 image_width[MAX_IDLE_SCREEN_SHORTCUTS], image_height[MAX_IDLE_SCREEN_SHORTCUTS], i, total_width =
        0, x, x_margin;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        gui_measure_image((PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + i)), &image_width[i], &image_height[i]);
        total_width += image_width[i];
        touch_idle_shortcuts[i].height = image_height[i];
        touch_idle_shortcuts[i].width = image_width[i];
    }

    MMI_DBG_ASSERT(total_width >= 0 && total_width <= UI_device_width);

     
    //      touch_idle_shortcuts[0].entryfn = NULL; /* Needs to be decided */
    touch_idle_shortcuts[0].entryfn = EntryMainMenuFromIdleScreen;
     
    touch_idle_shortcuts[1].entryfn = EntryScrMessagesMenuList;
    touch_idle_shortcuts[2].entryfn = mmi_phb_entry_main_menu;
    touch_idle_shortcuts[3].entryfn = IdleScreenDigitHandler;

    x_margin = (UI_device_width - total_width) / ((MAX_IDLE_SCREEN_SHORTCUTS - 1) + 2); /* Margin between shortcut Images,Leave margin on front and back also */

    x = x_margin;

    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        touch_idle_shortcuts[i].x = x;
        touch_idle_shortcuts[i].y = UI_device_height - touch_idle_shortcuts[i].height;
        x += touch_idle_shortcuts[i].width + x_margin;
    }

}


/*****************************************************************************
 * FUNCTION
 *  display_touch_idle_screen_shortcuts
 * DESCRIPTION
 *  This function is used to draw touch screen shortcuts
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 
 *  This routine is dependent on Order of Image Id's of Shortcut Item Images.
 *  DON'T CHANGE THE ORDER OF IMAGE ID's
 *****************************************************************************/
void display_touch_idle_screen_shortcuts(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, max_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    //      function is added for calculating the maximum height of idlescreen shortcut
    //      max_height = touch_idle_shortcuts[0].height;    /*In case Image's are of different height, Find the maximum height image*/

    gui_lock_double_buffer();
    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        gui_show_image(
            touch_idle_shortcuts[i].x,
            touch_idle_shortcuts[i].y,
            get_image((UI_image_ID_type ) (IMG_IDLE_MAINMENU_SHORCUT + i)));
        //              if(max_height < touch_idle_shortcuts[i].height)
        //                      max_height = touch_idle_shortcuts[i].height;
    }
    max_height = max_touch_idle_screen_shortcut_height();
     
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, UI_device_height - max_height, UI_device_width, UI_device_height);
}

 


/*****************************************************************************
 * FUNCTION
 *  max_touch_idle_screen_shortcut_height
 * DESCRIPTION
 *  This function is used to find the maximum height of idlescreen shortcut
 *  among them.
 * PARAMETERS
 *  void
 * RETURNS
 *  S32 max_height
 
 *  
 *****************************************************************************/
S32 max_touch_idle_screen_shortcut_height(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, max_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* In case Image's are of different height, Find the maximum height image */
    max_height = touch_idle_shortcuts[0].height;

    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        if (max_height < touch_idle_shortcuts[i].height)
        {
            max_height = touch_idle_shortcuts[i].height;
        }
    }
    return (max_height);
}

 
#endif /* defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__) */ 
/* ~] */

 
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__


/*****************************************************************************
 * FUNCTION
 *  show_boundries_to_status_bar_at_bottom_idlescreen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_boundries_to_status_bar_at_bottom_idlescreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    c = gui_color(255, 255, 255);
    gui_draw_horizontal_line(0, UI_device_width - 1, UI_device_height - 1 - MMI_button_bar_height, c);
    gui_draw_horizontal_line(
        0,
        UI_device_width - 1,
        UI_device_height - 1 - MMI_button_bar_height - MMI_status_bar_height - 2,
        c);
    gui_draw_horizontal_line(0, UI_device_width - 1, 0, c);

#ifdef __MMI_MAINLCD_128X128__
    gui_draw_horizontal_line(0, UI_device_width - 1, MMI_status_bar_height + 2, c);
#else 
    gui_draw_horizontal_line(0, UI_device_width - 1, MMI_status_bar_height, c);
#endif 
}
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory33Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory33Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
  mmi_trace(g_sw_WpSsPwon, "gdm: RedrawCategory33Screen() ");
#ifndef __GDI_MEMORY_PROFILE_2__
    gui_cancel_timer(cat33_animation_processor);
#endif 

#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
    gui_push_clip();
    gui_set_clip(0, MMI_status_bar_height, UI_device_width - 1, MMI_status_bar_height + MMI_IDLESCREEN_BAR_HEIGHT - 1);
#ifndef __MMI_UI_TECHNO_STATUS_ICON__
    if (PhnsetGetShowDateTimeStatus() == MMI_TRUE)
#endif 
        gui_show_image(0, MMI_status_bar_height, get_image(idlescreen_bar_id));//070306 Alpha layer
    gui_pop_clip();
#endif /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */ 

#ifdef __GDI_MEMORY_PROFILE_2__
    update_mainlcd_dt_display();
#else 
    show_main_LCD_dt_display();
#endif 

#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    MMI_title_string = NULL;
    MMI_title_icon = NULL;
    draw_title();
    arrange_status_icons();
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 

#ifndef __MMI_IDLE_FULL_SCREEN__
    idle_screen_hide_status_icons_bar0();
#endif 
    in_idle_screen();

#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    disable_statusbar_fill_background();
#endif 

    show_status_icons();

#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    enable_statusbar_fill_background();
    refresh_fixed_status_icon(STATUS_ICON_SIGNAL_STRENGTH, 0);
    refresh_fixed_status_icon(STATUS_ICON_BATTERY_STRENGTH, 1);
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    is_button_bar_shuffled = 0;
#endif 

#ifdef __MMI_UI_TECHNO_STATUS_ICON__
    refresh_fixed_status_icon(STATUS_ICON_SIGNAL_STRENGTH, 0);
    refresh_fixed_status_icon(STATUS_ICON_BATTERY_STRENGTH, 0);
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
    if (idle_screen_type == ANALOG_IDLE_SCREEN || idle_screen_type == DIGITAL_IDLE_SCREEN)
    {
        idle_screen_show_network_name();
    }
    else
    {
        #ifdef __MMI_BLACKBERRY_IDLE_SHORTCUT__
            idle_screen_show_network_name();
        #else

            idle_screen_show_network_details();
        #endif
    }

#ifdef __MMI_IDLE_FULL_SCREEN__
    on_idle_screen = 1;
#endif 
    set_on_idlescreen(1);

#if (defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__) || defined(__MMI_MAINLCD_220X176__))




    {
#if defined(__MMI_TOUCH_SCREEN__)
		mmi_pen_register_down_handler(mmi_touch_shortcut_pen_down_hdlr);
		mmi_pen_register_up_handler(mmi_touch_shortcut_pen_up_hdlr);
		mmi_pen_register_move_handler(mmi_touch_shortcut_pen_move_hdlr);
#endif
        display_touch_idle_screen_shortcuts();
    }
#elif defined(__MMI_NOKIA_STYLE_N800__) || defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
    if (mmi_bootup_get_active_flight_mode() == 0)
    {
        display_touch_idle_screen_shortcuts();
    }
#elif defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__)
    if (mmi_bootup_get_active_flight_mode() == 0)
    {
        display_touch_idle_screen_shortcuts();
    }
#endif /* defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__) */ 

    register_hide_status_icon_bar(0, idle_screen_hide_status_icons_bar0);
#if !defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)&&!defined(__PROJECT_GALLITE_C01__)
    register_hide_status_icon_bar(1, idle_screen_hide_status_icons_bar1);
#endif
#if(ENABLE_ANIMATED_WALLPAPERS)
#ifndef __GDI_MEMORY_PROFILE_2__
    if (cat33_n_frames > 1 && idle_screen_wallpaper_ID != 0)
    {
        mmi_trace(g_sw_WpSsPwon, "gdm: RedrawCategory33Screen() cat33_animation_delay=%d", cat33_animation_delay);
        gui_start_timer(cat33_animation_delay, cat33_animation_processor);
    }
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
#endif /* (ENABLE_ANIMATED_WALLPAPERS) */ 
}
	
/*****************************************************************************
 * FUNCTION
 *  dm_category_33_controlled_area
 * DESCRIPTION
 *  This function is used to draw the category controlled area of idle screen
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void dm_category_33_controlled_area(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory33Screen();
}

#ifdef __MMI_TOUCH_SCREEN__

 

#ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__


/*****************************************************************************
 * FUNCTION
 *  gui_idle_touch_screen_display_shorcut_item
 * DESCRIPTION
 *  This function is Display individual shorcut item on Touch idle screen
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 
 *  
 *****************************************************************************/
void gui_idle_touch_screen_display_shorcut_item(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
    S32 x1, y1, x2, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    MMI_DBG_ASSERT(index >= 0 && index < MAX_IDLE_SCREEN_SHORTCUTS);
     
    x1 = touch_idle_shortcuts[index].x;
    y1 = touch_idle_shortcuts[index].y;
    x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;
    y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
     
    gui_lock_double_buffer();
     
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
#ifdef __GDI_MEMORY_PROFILE_2__
    /* gui_fill_rectangle(x1, y1, x2, y2, wgui_transparent_color); */
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    /* gui_show_image(touch_idle_shortcuts[index].x,touch_idle_shortcuts[index].y,GetImage((U16)(IMG_IDLE_MAINMENU_SHORCUT+index))); */
    gui_show_image(x1, y1, (PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + index)));
    gui_pop_clip();
    gui_unlock_double_buffer();
    /* gui_BLT_double_buffer(touch_idle_shortcuts[index].x,touch_idle_shortcuts[index].y,touch_idle_shortcuts[index].x+touch_idle_shortcuts[index].width,touch_idle_shortcuts[index].y+touch_idle_shortcuts[index].height); */
    gui_BLT_double_buffer(x1, y1, x2, y2);
     
}


/*****************************************************************************
 * FUNCTION
 *  gui_idle_touch_screen_display_pressed_shorcut_item
 * DESCRIPTION
 *  This function is Display individual pressed shorcut item on Touch idle screen
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 
 *  
 *****************************************************************************/
void gui_idle_touch_screen_display_pressed_shorcut_item(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
    S32 x1, y1, x2, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    MMI_DBG_ASSERT(index >= 0 && index < MAX_IDLE_SCREEN_SHORTCUTS);
     
    x1 = touch_idle_shortcuts[index].x;
    y1 = touch_idle_shortcuts[index].y;
    x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;
    y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
     
    gui_lock_double_buffer();
     
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
#ifdef __GDI_MEMORY_PROFILE_2__
    /* gui_fill_rectangle(x1, y1, x2, y2, wgui_transparent_color); */
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    /* gui_show_image(touch_idle_shortcuts[index].x,touch_idle_shortcuts[index].y,GetImage((U16)(IMG_IDLE_MAINMENU_SHORCUT_PRESSED+index))); */
    gui_show_image(x1, y1, (PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT_PRESSED + index)));
    gui_pop_clip();
    gui_unlock_double_buffer();
    /* gui_BLT_double_buffer(touch_idle_shortcuts[index].x,touch_idle_shortcuts[index].y,touch_idle_shortcuts[index].x+touch_idle_shortcuts[index].width,touch_idle_shortcuts[index].y+touch_idle_shortcuts[index].height); */
    gui_BLT_double_buffer(x1, y1, x2, y2);
     
}


/*****************************************************************************
 * FUNCTION
 *  gui_idle_touch_screen_shortcut_translate_pen_position
 * DESCRIPTION
 *  This function finds index of shorcut item on which pen event came
 * PARAMETERS
 *  x           [IN]        Pen's x position
 *  y           [IN]        Pen's y position
 *  index       [OUT]       Index of selected item
 * RETURNS
 *  void
 
 *  
 *****************************************************************************/
void gui_idle_touch_screen_shortcut_translate_pen_position(S32 x, S32 y, S32 *index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *index = -1;

    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        if (x >= touch_idle_shortcuts[i].x && x <= touch_idle_shortcuts[i].x + touch_idle_shortcuts[i].width &&
            y >= touch_idle_shortcuts[i].y && y <= touch_idle_shortcuts[i].y + touch_idle_shortcuts[i].height)
        {
            *index = i;
            break;
        }
    }
}
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
/* ~] */


/*****************************************************************************
 * FUNCTION
 *  category33_pen_down_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category33_pen_down_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 pos_x = point.x, pos_y = point.y, y2;
    MMI_BOOL ret = MMI_FALSE;;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_idle_restart_screensaver_timer();
    mmi_idle_restart_keypad_lock_timer();

     
#ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__
     
    if (mmi_bootup_get_active_flight_mode() == 1)
    {
        y2 = UI_device_height - 1 - MMI_button_bar_height;
    }
    else
         
    {
        y2 = UI_device_height;  /* N CASE IDLE SCREEN SHORTCUTS ARE DISPLAYED, BUTTON BAR IS NOT DISPLAYED */
    }
#else /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    y2 = UI_device_height - 1 - MMI_button_bar_height;
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    /* ~] */
    if (pos_x > 0 && pos_x < UI_device_width - 1 && pos_y > MMI_content_y && pos_y < y2)
    {
        ret = MMI_TRUE;
         
    #ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__
        {
            S32 index;

            gui_idle_touch_screen_shortcut_translate_pen_position(pos_x, pos_y, &index);
            if (index >= 0 && index < MAX_IDLE_SCREEN_SHORTCUTS)
            {
                touch_idle_screen_highlighted_shortcut = index;
                touch_idle_screen_highlight_changed = 0;
                gui_idle_touch_screen_display_pressed_shorcut_item(index);
            }
        }
    #endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
        /* ~] */
    }
    return ret;
}

S32 cat33_scr_lock_repeat = -1;


/*****************************************************************************
 * FUNCTION
 *  category33_pen_long_tap_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category33_pen_long_tap_handler(mmi_pen_point_struct point)
{
#if 0
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 pos_x = point.x, pos_y = point.y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_keylock_context.gKeyPadLockFlag == 0 && pos_x >= UI_device_width >> 2 &&
        pos_x <= UI_device_width - (UI_device_width < 2) && pos_y >= UI_device_height >> 2 &&
        pos_y <= UI_device_height - (MMI_button_bar_height >> 2))
    {
        cat33_scr_lock_repeat = 0;
        return MMI_TRUE;
    }
    return MMI_FALSE;
	#else
	S32 pos_x=point.x,pos_y=point.y;
	extern U16 gKeyPadLockFlag ;
//	if ( gKeyPadLockFlag==0 && pos_x >=UI_device_width>>2 && pos_x <=UI_device_width-(UI_device_width<2) && pos_y >= UI_device_height>>2 &&pos_y <= UI_device_height-(MMI_button_bar_height>>2) )
//   if have problem ?????????????????????

	if ( gKeyPadLockFlag==0 && pos_x >=(UI_device_width/4) && (pos_x <=UI_device_width-1) && pos_y >= (UI_device_height/4) &&pos_y <= UI_device_height-(MMI_button_bar_height/4) )
	{
		cat33_scr_lock_repeat=0;
		return MMI_TRUE;
	}
	return MMI_FALSE;
	#endif
}


/*****************************************************************************
 * FUNCTION
 *  category33_pen_repeat_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category33_pen_repeat_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_idle_restart_screensaver_timer();
    mmi_idle_restart_keypad_lock_timer();
    return MMI_TRUE;
}

 
#ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__


/*****************************************************************************
 * FUNCTION
 *  category33_pen_up_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category33_pen_up_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 pos_x = point.x, pos_y = point.y, index;
    MMI_BOOL ret = MMI_TRUE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_idle_touch_screen_shortcut_translate_pen_position(pos_x, pos_y, &index);
    if (index >= 0 && index < MAX_IDLE_SCREEN_SHORTCUTS)
    {
        gui_idle_touch_screen_display_shorcut_item(index);
        if (!touch_idle_screen_highlight_changed)
        {
            if (touch_idle_shortcuts[index].entryfn)
            {
                touch_idle_shortcuts[index].entryfn();
            }
        }
    }
    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  category33_pen_move_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category33_pen_move_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 pos_x = point.x, pos_y = point.y, index;
    MMI_BOOL ret = MMI_TRUE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_idle_touch_screen_shortcut_translate_pen_position(pos_x, pos_y, &index);
    if (index != touch_idle_screen_highlighted_shortcut)
    {
        touch_idle_screen_highlight_changed = 1;
        /* Unpressed Image */
        gui_idle_touch_screen_display_shorcut_item(touch_idle_screen_highlighted_shortcut);
    }
    if (index >= 0 && index < MAX_IDLE_SCREEN_SHORTCUTS)
    {
        touch_idle_screen_highlighted_shortcut = index;
        /* Pressed Image */
        gui_idle_touch_screen_display_pressed_shorcut_item(index);
    }
    return ret;
}
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  cat33_hide_lsk
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat33_hide_lsk(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(
        MMI_softkeys[MMI_LEFT_SOFTKEY].x,
        MMI_softkeys[MMI_LEFT_SOFTKEY].y,
        MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width - 1,
        MMI_softkeys[MMI_LEFT_SOFTKEY].y + MMI_softkeys[MMI_LEFT_SOFTKEY].height - 1);
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_draw_solid_rect(
        MMI_softkeys[MMI_LEFT_SOFTKEY].x,
        MMI_softkeys[MMI_LEFT_SOFTKEY].y,
        MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width - 1,
        MMI_softkeys[MMI_LEFT_SOFTKEY].y + MMI_softkeys[MMI_LEFT_SOFTKEY].height - 1,
        GDI_COLOR_TRANSPARENT);
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  cat33_hide_rsk
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat33_hide_rsk(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(
        MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].y,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width - 1,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].y + MMI_softkeys[MMI_RIGHT_SOFTKEY].height - 1);
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_draw_solid_rect(
        MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].y,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width - 1,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].y + MMI_softkeys[MMI_RIGHT_SOFTKEY].height - 1,
        GDI_COLOR_TRANSPARENT);
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    gdi_layer_pop_clip();
}

#ifdef __MMI_WGUI_CSK_ENABLE__


/*****************************************************************************
 * FUNCTION
 *  cat33_hide_csk
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat33_hide_csk(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(
        MMI_softkeys[MMI_CENTER_SOFTKEY].x,
        MMI_softkeys[MMI_CENTER_SOFTKEY].y,
        MMI_softkeys[MMI_CENTER_SOFTKEY].x + MMI_softkeys[MMI_CENTER_SOFTKEY].width - 1,
        MMI_softkeys[MMI_CENTER_SOFTKEY].y + MMI_softkeys[MMI_CENTER_SOFTKEY].height - 1);
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_draw_solid_rect(
        MMI_softkeys[MMI_CENTER_SOFTKEY].x,
        MMI_softkeys[MMI_CENTER_SOFTKEY].y,
        MMI_softkeys[MMI_CENTER_SOFTKEY].x + MMI_softkeys[MMI_CENTER_SOFTKEY].width - 1,
        MMI_softkeys[MMI_CENTER_SOFTKEY].y + MMI_softkeys[MMI_CENTER_SOFTKEY].height - 1,
        GDI_COLOR_TRANSPARENT);
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    gdi_layer_pop_clip();
}
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory33Screen
 * DESCRIPTION
 *  Displays the Idle screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
/*
 * #ifdef MMI_ON_HARDWARE_P
 * extern kal_int32 L1T_GetFN( void );
 * #endif
 */
void ShowCategory33Screen_ext(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

     
#ifndef __MMI_UI_DALMATIAN_STATUSBAR__
    S32 status_bar1_y1;
    S32 status_bar1_y2;
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     

    UI_UNUSED_PARAMETER(history_buffer);

    GDI_LOCK;
    gui_lock_double_buffer();

     
    set_on_idlescreen(1);
     
 
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    set_status_bar_at_bottom();
#endif 
  
#ifdef __MMI_IDLE_FULL_SCREEN__
    on_idle_screen = 1;
     
    //For implement dalmatian on style 1, 2 and 3.
#if !(defined(__MMI_UI_DALMATIAN_SOFTKEYS__) && (defined(__MMI_MAINLCD_128X128__) || defined(__MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_160X128__)))
    disable_softkey_background();
#endif 
     
#endif /* __MMI_IDLE_FULL_SCREEN__ */ 

    cat33_frame_number = 0;
    MMI_menu_shortcut_number = -1;
    /* BUG SOFTKEYS START 20050902 */
    /* No need to set the default handlers for Left and Right Softkeys in case of
       Idlescreen Shortcuts. So passing the value 0 for string ID and icon ID. */
#ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__
     
    if (mmi_bootup_get_active_flight_mode() == 1)
    {
        change_left_softkey(left_softkey, left_softkey_icon);
        change_right_softkey(right_softkey, right_softkey_icon);
    }
    else
    {
         
        change_left_softkey(0, 0);
        change_right_softkey(0, 0);
         
    }
     
#else /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    /* BUG SOFKEYS END 20050902 */
    SetupCategoryKeyHandlers();
#ifdef __MMI_WGUI_CSK_ENABLE__
    //      set_softkey_label(L"Dial",MMI_CENTER_SOFTKEY);
    //      set_softkey_icon(NULL, MMI_CENTER_SOFTKEY);
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 

    /* No need to register the hide functions for Left and Right Sofkeys
       when Idle Screen Shortcuts are present.   */
#ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__
	if(mmi_bootup_get_active_flight_mode() == 1)
#endif
    {
    #if  1  
        {
            register_hide_left_softkey(cat33_hide_lsk);
            register_hide_right_softkey(cat33_hide_rsk);
        #ifdef __MMI_WGUI_CSK_ENABLE__
            register_hide_softkey(cat33_hide_csk, MMI_CENTER_SOFTKEY);
        #endif 
        }
    #else  
        {
 
               softkey background requires redrawing wallpaper, which is 
               very slow */
            set_softkey_flags_on(UI_BUTTON_NO_SHIFT_BUTTON, MMI_LEFT_SOFTKEY);
            set_softkey_flags_on(UI_BUTTON_NO_SHIFT_BUTTON, MMI_RIGHT_SOFTKEY);
        #ifdef __MMI_WGUI_CSK_ENABLE__
            set_softkey_flags_on(UI_BUTTON_NO_SHIFT_BUTTON, MMI_CENTER_SOFTKEY);        
        #endif
        }
    #endif  
    }

    register_hide_status_icon_bar(0, UI_dummy_function);
    register_hide_status_icon_bar(1, UI_dummy_function);
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__

    gui_register_button_shuffle(shuffle_draw_wallpaper);    /* register the background drower */
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 

#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    set_status_icon_bar_clip(
        0,
        0,
        MMI_title_height + STATUS_TITLE_GAP,
        MMI_STATUS_BAR_WIDTH - 1,
        MMI_title_height + STATUS_TITLE_GAP + MMI_SINGLE_STATUS_BAR_HEIGHT - 1);
    set_status_icon_bar_clip(
        1,
        UI_device_width - MMI_STATUS_BAR_WIDTH,
        MMI_title_height + STATUS_TITLE_GAP + MMI_status_bar_height - MMI_SINGLE_STATUS_BAR_HEIGHT,
        UI_device_width - 1,
        MMI_title_height + STATUS_TITLE_GAP + MMI_status_bar_height - 1);
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    set_status_icon_bar_clip(
        0,
        0,
        UI_device_height - 1 - MMI_button_bar_height - MMI_status_bar_height,
        UI_device_width - 1,
        UI_device_height - 1 - MMI_button_bar_height - 1);
#else /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
#ifdef __MMI_SCREEN_ROTATE__
            if(mmi_frm_is_screen_width_height_swapped())
            {
                set_status_icon_bar_clip(0, 0, 0, UI_device_width - 1 - MMI_ROTATED_BUTTON_BAR_HEIGHT, MMI_status_bar_height - 1);
            }
            else
            {
                set_status_icon_bar_clip(0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1);
            }
#else
    set_status_icon_bar_clip(0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1);
#endif
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
     

    /* 110105 status bar1 Start */
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
    if (PhnsetGetShowDateTimeStatus() == MMI_TRUE)
    {
        status_bar1_y1 = MMI_status_bar_height + MMI_IDLESCREEN_BAR_HEIGHT;
    }
    else
    {
        status_bar1_y1 = MMI_status_bar_height;
    }
#else /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */ 
    status_bar1_y1 = MMI_status_bar_height;
#endif /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */ 

#ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__
    if (mmi_bootup_get_active_flight_mode() == 1)
    {
        status_bar1_y2 = UI_device_height - MMI_softkey_height - 2;
    }
    else
    {
        status_bar1_y2 = UI_device_height - WGUI_TOUCH_IDLESCREEN_SHORTCUTS_HEIGHT - 2;
    }
#else /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    status_bar1_y2 = UI_device_height - MMI_softkey_height - 2;
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    /* 110105 status bar1 End */

#ifdef __MMI_UI_TECHNO_STATUS_ICON__
    set_status_icon_bar_clip(1, 0, 0, 0, 0);
    arrange_status_icons();
#else /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    set_status_icon_bar_clip(
        1,
        UI_device_width - 16,
        MMI_status_bar_height,
        UI_device_width - 1,
        UI_device_height - 1 - MMI_button_bar_height - MMI_status_bar_height - (STATUS_ICON_Y_GAP << 1) - 1);
    arrange_status_icons();
#else /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
#ifdef __MMI_KLG__
    set_status_icon_bar_clip(1, UI_device_width - 16, status_bar1_y1, UI_device_width - 1, UI_device_height >> 1);       
#else 
#if !defined(__MMI_NOKIA_STYLE_N800__)
#if defined(__MMI_MAINLCD_220X176__)
	set_status_icon_bar_clip(1, UI_device_width - 16, status_bar1_y1, UI_device_width - 1, status_bar1_y1);//__LINE__ + 2 line,causes invalid display in idle.
#else
	set_status_icon_bar_clip(1, UI_device_width - 16, status_bar1_y1, UI_device_width - 1, status_bar1_y2);      
#endif

#endif
#endif 
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
     
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
     
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 

#ifdef __MMI_SCREEN_ROTATE__
    show_status_icon_bar(0);
    hide_status_icon_bar(1);
#else
    show_status_icon_bar(0);
    show_status_icon_bar(1);
#endif

    switch (idle_screen_type)
    {
        case NORMAL_IDLE_SCREEN:
             
    #ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
	     TRACE_GDI_FUNCTION();
            set_main_LCD_dt_date_hide_function(cat33_hide_date_display);
            analog_clock_initialize();
            set_main_LCD_dt_time_hide_function(dummy_dt_hide_function);
             
            set_main_LCD_day_hide_function(cat33_hide_day_string_display);
             
    #else /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */ 
            set_main_LCD_dt_date_hide_function(dummy_dt_hide_function);
            set_main_LCD_dt_time_hide_function(cat33_hide_time_display);
    #endif /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */ 
             
             
    #if defined(__MMI_KLG__) || defined(__MMI_UI_STATUS_BAR_AT_BOTTOM__)
             
            set_dt_display(DT_IDLE_SCREEN_2);
    #else /* defined(__MMI_KLG__) || defined(__MMI_UI_STATUS_BAR_AT_BOTTOM__) */ 
            set_dt_display(DT_IDLE_SCREEN);
    #endif /* defined(__MMI_KLG__) || defined(__MMI_UI_STATUS_BAR_AT_BOTTOM__) */ 
            break;

    #ifdef __MMI_ANALOG_CLOCK__
        case ANALOG_IDLE_SCREEN:
            analog_clock_initialize();
            set_main_LCD_dt_date_hide_function(dummy_dt_hide_function);
            set_main_LCD_dt_time_hide_function(cat33_analog_clock_hide_dt_display);
            set_dt_display(DT_ANALOG_CLOCK_SC);
            break;
    #endif /* __MMI_ANALOG_CLOCK__ */ 

    #ifdef __MMI_DIGITAL_CLOCK__
        case DIGITAL_IDLE_SCREEN:
            set_main_LCD_dt_date_hide_function(dummy_dt_hide_function);
            set_main_LCD_dt_time_hide_function(cat33_digital_clock_hide_dt_display);
            set_dt_display(DT_DIGITAL_CLOCK_SC1);
            break;
    #endif /* __MMI_DIGITAL_CLOCK__ */ 
    }

    gui_unlock_double_buffer();

     
#if defined(__MMI_NOKIA_STYLE_N800__) || defined(__MMI_MAINLCD_220X176__) || defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
	    {
	        setup_touch_idle_screen_shortcuts();
	    }
#elif defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__)
     
    /* clear the memory of touch_idle_shortcuts when the Flight Mode is ON. */
	{
#ifdef __FLIGHT_MODE_SUPPORT__
#ifdef __MMI_MULTI_SIM__
		if(MTPNP_AD_Get_UsableSide_Number() == 0)
#else
		if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
#endif
#endif
	  //  if (mmi_bootup_get_active_flight_mode() == 0)
	    {
	        setup_touch_idle_screen_shortcuts();
	    }
	    else
	         
	    {
	        memset(touch_idle_shortcuts, 0, sizeof(idle_touch_screen_shortcut) * MAX_IDLE_SCREEN_SHORTCUTS);
	    }
	}
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    /* ~] */

#if (defined __MMI_TOUCH_SCREEN__)
    wgui_register_category_screen_control_area_pen_handlers(category33_pen_down_handler, MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(category33_pen_repeat_handler, MMI_PEN_EVENT_REPEAT);
     
#ifndef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__
    wgui_register_category_screen_control_area_pen_handlers(category33_pen_long_tap_handler, MMI_PEN_EVENT_LONG_TAP);
#else /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
     
    /* remove the pen handler registration for UP and MOVE event, when the Flight
       Mode is ON. */
    if (mmi_bootup_get_active_flight_mode() == 0)
    {
        wgui_register_category_screen_control_area_pen_handlers(category33_pen_up_handler, MMI_PEN_EVENT_UP);
        wgui_register_category_screen_control_area_pen_handlers(category33_pen_move_handler, MMI_PEN_EVENT_MOVE);
    }
     
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    /* ~] */
#endif /* (defined __MMI_TOUCH_SCREEN__) */ 
    gdi_layer_push_and_set_active(GDI_LAYER_MAIN_BASE_LAYER_HANDLE);
    gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
    gdi_layer_clear(GDI_COLOR_TRANSPARENT);
    gdi_layer_pop_and_restore_active();
    GDI_UNLOCK;
}

void ShowCategory33Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    ShowCategory33Screen_ext(
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        history_buffer);
    GDI_LOCK;
    ExitCategoryFunction = ExitCategory33Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCategoryFunction = dm_redraw_category_screen;

    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY33_ID;
    dm_data.S32flags = 0;
#ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__
    if (mmi_bootup_get_active_flight_mode() == 0)
    {
        dm_data.S32flags |= DM_NO_SOFTKEY;
    }
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */ 
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(dm_category_33_controlled_area);
    dm_redraw_category_screen();
    GDI_UNLOCK;
}

extern void leave_idle_screen(void);


/*****************************************************************************
 * FUNCTION
 *  ExitCategory33Screen
 * DESCRIPTION
 *  Exits the Idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory33Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    leave_idle_screen();
 
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    reset_status_bar_at_bottom();
#endif 
  
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
     
    MMI_title_y = MMI_TITLE_Y;
     
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    gui_register_button_shuffle(shuffle_draw_white);    /* register the background drower */
#endif 
     

#ifdef __MMI_IDLE_FULL_SCREEN__
    on_idle_screen = 0;
#endif 
     
     
    set_on_idlescreen(0);
     
#if defined(__MMI_NOKIA_STYLE_N800__)
    
    if(short_cut_audply_title_created)
    {
         gui_scrolling_text_stop(&short_cut_audply_title);
         short_cut_audply_title_created = FALSE;
    }
#endif
    if (extra_scrolling_text)
    {
        gui_scrolling_text_stop(extra_scrolling_text);
        extra_scrolling_text = NULL;
    }
    /* 110206 digital clock Start */
    extra_text_x1 = -1;
    extra_text_y1 = -1;
    extra_text_x2 = -1;
    extra_text_y2 = -1;
    /* 110206 digital clock End */

    gui_cancel_timer(cat33_animation_processor);
#if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
    if (animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(animation_handle);
        animation_handle = GDI_ERROR_HANDLE;
    }
#endif /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 
    close_status_icons();
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    close_shuffle();    /* to close the continous shuffling */
#endif 
     

     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    register_hide_status_icon_bar(0, MMI_dummy_function);
#endif 
     
    register_hide_status_icon_bar(1, MMI_dummy_function);
    close_main_LCD_dt_display();
    reset_softkeys();
    enable_softkey_background();
     
#ifdef __MMI_UI_TECHNO_STATUS_ICON__
    arrange_status_icons();
#endif 
     
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    set_status_icon_bar_clip(0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1);
#if defined(__MMI_KLG__)
    set_status_icon_bar_clip(
        1,
        UI_device_width - 16,
        MMI_status_bar_height,
        UI_device_width - 1,
        UI_device_height >> 1);
#else /* defined(__MMI_KLG__) */ 
    set_status_icon_bar_clip(
        1,
        UI_device_width - 16,
        MMI_status_bar_height,
        UI_device_width - 1,
        UI_device_height - MMI_softkey_height - 2);
#endif /* defined(__MMI_KLG__) */ 
    arrange_status_icons();
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
     
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}

 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__


/*****************************************************************************
 * FUNCTION
 *  shuffle_draw_wallpaper
 * DESCRIPTION
 *  Function for draw wallpaper background in shuffle at idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void shuffle_draw_wallpaper(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    draw_wallpaper();
#if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
    {
        U16 keyCode, keyType;

        GetkeyInfo(&keyCode, &keyType);
        if (keyType == KEY_EVENT_DOWN)
        {
            if (animation_handle != GDI_ERROR_HANDLE)
            {
                gdi_image_stop_animation(animation_handle);
                animation_handle = GDI_ERROR_HANDLE;
            }
        }
    }
#endif /* defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__) */ 

}
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__)


/*****************************************************************************
 * FUNCTION
 *  start_shuffle
 * DESCRIPTION
 *  Function for start the shuffle of status & button bar at idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void start_shuffle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    shuffle_status_bars();
#endif 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    gui_register_button_shuffle(shuffle_draw_wallpaper);
    gui_call_shuffle();
    if (is_button_bar_shuffled == 1)
    {
        is_button_bar_shuffled = 0;
    }
    else
    {
        is_button_bar_shuffled = 1;
    }
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
    gui_start_timer(BAR_SHUFFLE_TIMER, start_shuffle);

}


/*****************************************************************************
 * FUNCTION
 *  close_shuffle
 * DESCRIPTION
 *  Function for close down the shuffle of status & button bar at idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void close_shuffle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(start_shuffle);
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    is_status_bar_shuffled = 0; /* To reset the shuffle indicator for status bar */
#endif 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    is_button_bar_shuffled = 0; /* for button bar */
#endif 
}

#endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) */ 
 
#ifdef __MMI_ANALOG_CLOCK__
gdi_handle cat86_anim_handle;    


/*****************************************************************************
 * FUNCTION
 *  cat86_hide_analog_clock_display
 * DESCRIPTION
 *  hide function of analog clock of categor 86
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void cat86_hide_analog_clock_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = 0;
    y1 = (MMI_title_y + MMI_title_height);
    x2 = UI_device_width - 1;
    y2 = UI_device_height - MMI_button_bar_height;
    gui_set_clip(x1, y1, x2, y2);

     
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
#else 
    gui_show_image(cat86_image_x, cat86_image_y, cat86_image);
#endif 
     

    redraw_status_icon_bar(1);
    if (display_network_name)
    {
        idle_screen_show_network_name();
    }
}


/*****************************************************************************
 * FUNCTION
 *  DrawCate86CategoryControlArea
 * DESCRIPTION
 *  Draws the category86 category control area
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate86CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory86Screen();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory86Screen
 * DESCRIPTION
 *  Redraws the category86 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory86Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    //cat86_image=(PU8)GetImage(ANALOG_CLOCK);  
     

     
     

    /*
     * if(status_icon == 1) show_status_icons();
     * else if(status_icon==2)
     * {
     * draw_title();
     * show_title_status_icon();
     * }
     */
    show_main_LCD_dt_display();
    if (display_network_name)
    {
        idle_screen_show_network_name();
    }

}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory86Screen
 * DESCRIPTION
 *  Displays the Idle screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  img_id                  [IN]        
 *  network_string          [IN]        Service provider name
 *  history_buffer          [IN]        History buffer
 *  show_network_name       [IN]        If network name display on the screen
 * RETURNS
 *  void
 *****************************************************************************/
 
void ShowCategory86Screen(U16 title, U16 title_icon, U16 left_softkey, U16 left_softkey_icon, U16 right_softkey, U16 right_softkey_icon, U16 img_id, U8 *network_string, U8 *history_buffer, U8 show_network_name)     
 
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    U16 btn_prev_item_id = 0, btn_next_item_id = 0;  

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    UI_UNUSED_PARAMETER(network_string);
    gui_lock_double_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    ShowStatusIconsTitle();
#endif 
     

    /*
     * if((title==0) && (title_icon==0))
     * {  status_icon=1;
     * register_hide_status_icon_bar(0,idle_screen_hide_status_icons_bar0);
     * register_hide_status_icon_bar(1,idle_screen_hide_status_icons_bar1);
     * set_status_icon_bar_clip(0,0,0,UI_device_width-1,MMI_status_bar_height-1);
     *  
     * #ifdef __MMI_KLG__
     * set_status_icon_bar_clip(1,UI_device_width-16,MMI_status_bar_height,UI_device_width-1,UI_device_height>>1);
     * #else
     * set_status_icon_bar_clip(1,UI_device_width-16,MMI_status_bar_height,UI_device_width-1,UI_device_height-MMI_softkey_height-2);
     * #endif
     *  
     * show_status_icon_bar(0);
     * show_status_icon_bar(1);
     * }
     * else if(!((title==0xffff) && (title_icon==0xffff)))
     * {
     * status_icon=2;
     * MMI_title_string=(UI_string_type)get_string(title);
     * MMI_title_icon=(PU8)get_image(title_icon);
     * }
     */
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    display_network_name = show_network_name;

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    dm_get_scr_bg_image(&idle_scr_bg_ID, &idle_scr_bg_filename, &idle_scr_bg_x, &idle_scr_bg_y, &idle_scr_bg_opacity);
    dm_set_scr_bg_image(img_id, NULL, -1, -1, 255);
#else /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
    dm_add_image(get_image(img_id), NULL, NULL);
    wgui_set_wallpaper_on_bottom(MMI_TRUE);
    #ifndef __GDI_MEMORY_PROFILE_2__//062106 cat128
    cat86_image_x = cat86_image_y = 0;
    cat86_image = get_image(img_id);
    #endif 
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

     
    //check if the title bar buttons are enabled for the particular screen.
    if (wgui_is_touch_title_bar_buttons())
    {
        btn_prev_item_id = dm_add_button(
                            NULL,
                            get_image(LEFT_RED_ARROW),
                            get_image(LEFT_RED_ARROW),
                            cat129_clear_icon_bg);
        btn_next_item_id = dm_add_button(
                            NULL,
                            get_image(RIGHT_RED_ARROW),
                            get_image(RIGHT_RED_ARROW),
                            cat129_clear_icon_bg);
        title_bg_id = IMAGE_VIEW_TITLEBAR_BG;
    }
     

    analog_clock_initialize();
    set_main_LCD_dt_date_hide_function(dummy_dt_hide_function);
    set_main_LCD_dt_time_hide_function(cat86_hide_analog_clock_display);
    set_dt_display(DT_ANALOG_CLOCK_SC);

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory86Screen;
     
#if defined(__MMI_TOUCH_SCREEN__)
    if (wgui_is_touch_title_bar_buttons())
    {
        //BUG IMAGE VIEW START 20050917
        // register the handler for pen event down istead of pen event up.
        dm_register_button_functions(btn_prev_item_id, KEY_EVENT_UP, gMMI_touch_title_button1_down_handler);
        dm_register_button_functions(btn_next_item_id, KEY_EVENT_UP, gMMI_touch_title_button2_down_handler);
        /* BUG IMAGE VIEW END 20050917 */
    }
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
     
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_register_category_controlled_callback(DrawCate86CategoryControlArea);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY86_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory86Screen
 * DESCRIPTION
 *  Exits the Idle screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory86Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
    register_hide_status_icon_bar(1, MMI_dummy_function);
    close_status_icons();
    close_main_LCD_dt_display();
    reset_softkeys();

     
    title_bg_id = 0;

    wgui_reset_touch_title_bar_buttons();
     

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
     
    if (wgui_is_wallpaper_change())
    {
        U8 dummy_opacity;

        dm_get_scr_bg_image(&idle_scr_bg_ID, &idle_scr_bg_filename, &idle_scr_bg_x, &idle_scr_bg_y, &dummy_opacity);
        dm_set_scr_bg_image(idle_scr_bg_ID, idle_scr_bg_filename, idle_scr_bg_x, idle_scr_bg_y, idle_scr_bg_opacity);
        wgui_reset_wallpaper_change();
    }
    else
    {
        dm_set_scr_bg_image_no_draw(
            idle_scr_bg_ID,
            idle_scr_bg_filename,
            idle_scr_bg_x,
            idle_scr_bg_y,
            idle_scr_bg_opacity);
    }
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}

#endif /* __MMI_ANALOG_CLOCK__ */ 
extern gdi_handle animation_handle;

/*----------------------------------------------------------------------------/
Function:         RedrawCategory106Screen
Description:      redarw category 106
Input Parameters: noen
Output Parameters:   none
Returns:       none
-----------------------------------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory106Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory106Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 SScallbackID;
    PU8 image;
    color_t c = gui_color(0, 0, 0);
    gdi_handle h;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    c.alpha = 100;

    gui_lock_double_buffer();
    gui_reset_clip();
    clear_screen();
    gui_fill_rectangle(0, 0, UI_device_width - 1, UI_device_height - 1, c);
    if (ScreenSaverName == NULL)
    {
        wgui_show_animation();
    }
    else
    {
        S32 n_frames = 0;
        S32 width, height;
        GDI_RESULT ret;

        gdi_image_gif_get_frame_count_file((S8*) ScreenSaverName, &n_frames);
        gdi_image_get_dimension_file((S8*) ScreenSaverName, &width, &height);

        if (n_frames > 1)
        {
            ret = gdi_image_draw_animation_file(
                    (UI_device_width - width) >> 1,
                    (UI_device_height - height) >> 1,
                    (S8*) ScreenSaverName,
                    &h);
        }
        else
        {
            /* The orignal implementation seems just to fill whole screen */
            S32 disp_width = UI_device_width /* x2-x1 */ ;
            S32 disp_height = UI_device_height /* y2-y1 */ ;

            S32 offset_x = 0, offset_y = 0;
            S32 img_width = 0, img_heigth = 0;

            gdi_image_get_dimension_file((S8*) ScreenSaverName, &img_width, &img_heigth);
            if ((disp_width >= img_width) && (disp_height >= img_heigth))
            {

                /* align preview window at the center of screen */
                offset_x = (disp_width - img_width) >> 1;
                offset_y = (disp_height - img_heigth) >> 1;

                /* set clip region as content region */
                /* gdi_layer_set_clip(0, (MMI_title_y+MMI_title_height), UI_device_width-1, (MMI_title_y+MMI_title_height)+MMI_content_height-1); */

                ret = gdi_image_draw_file(offset_x, offset_y, (PS8) ScreenSaverName);
            }
            else
            {
                S32 resized_offset_x;
                S32 resized_offset_y;
                S32 resized_width;
                S32 resized_height;
                U16 img_type;

                gdi_image_util_fit_bbox(
                    disp_width,
                    disp_height,
                    img_width,
                    img_heigth,
                    &resized_offset_x,
                    &resized_offset_y,
                    &resized_width,
                    &resized_height);

                img_type = gdi_image_get_type_from_file((PS8) ScreenSaverName);
                if (img_type == GDI_IMAGE_TYPE_JPG_FILE)
                {
                    ret = gdi_image_draw_resized_file(
                            resized_offset_x,
                            resized_offset_y,
                            resized_width,
                            resized_height,
                            (PS8) ScreenSaverName);
                }
                else
                {
                    ret = gdi_image_draw_file(offset_x, offset_y, (PS8) ScreenSaverName);
                }
            }
        }

        if (ret < 0 && ret != GDI_GIF_ERR_OUT_OF_CLIP_REGION && ret != GDI_BMP_ERR_OUT_OF_CLIP_REGION)
        {
            PRINT_INFORMATION_2((MMI_TRACE_G7_MISC, "RedrawCategory106Screen():[$$$]ret:%d [$$$]\n", ret));
            SScallbackID = PhnsetScreenSaverBadFileCallBack();
            image = get_image((UI_image_ID_type ) SScallbackID);
            gui_measure_image(image, &width, &height);
            gdi_image_draw_animation(
                (UI_device_width - width) >> 1,
                (UI_device_height - height) >> 1,
                image,
                &animation_handle);
            /* gui_show_animated_image((UI_device_width-width)>>1,(UI_device_height-height)>>1,image); */
        }

        /*
         * gdi_image_get_dimension_file((S8*)ScreenSaverName,&width,&height);
         * if ( gdi_image_draw_animation_file  ((UI_device_width-width)>>1,(UI_device_height-height)>>1,(S8*)ScreenSaverName,&h) <0 )
         * {
         * SScallbackID = PhnsetScreenSaverBadFileCallBack();
         * image = get_image((UI_image_ID_type )SScallbackID);
         * gui_measure_image(image,&width,&height);
         * gui_show_animated_image((UI_device_width-width)>>1,(UI_device_height-height)>>1,image);
         * }
         */

    }
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory106Screen
 * DESCRIPTION
 *  Shows an animated icon
 * PARAMETERS
 *  icon_screensaver        [IN]        
 *  filename                [?]         
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Animated/Non Animated Icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory106Screen(U16 icon_screensaver, U8 *filename, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 screensaver_icon;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);

    entry_full_screen();
    gui_lock_double_buffer();
    clear_category_screen_key_handlers();
    screen_saver_type = NORMAL_SCREEN_SAVER;
    if (icon_screensaver != 0)
    {
        screensaver_icon = (PU8) get_image(icon_screensaver);
        wgui_set_animation((UI_device_width >> 1), (UI_device_height >> 1), icon_screensaver);
        ScreenSaverName = NULL;
    }
    else
    {
        ScreenSaverName = filename;
    }

    gui_unlock_double_buffer();
    ExitCategoryFunction = ExitCategory106Screen;
    RedrawCategoryFunction = RedrawCategory106Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCategory106Screen();
}

 


/*****************************************************************************
 * FUNCTION
 *  ExitCategory106Screen
 * DESCRIPTION
 *  Exits the category106 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory106Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(animation_handle);
    }
    /* gui_hide_animations(); */
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}

gdi_handle cat128_anim_handle;   


/*****************************************************************************
 * FUNCTION
 *  category128_hide_dt_display
 * DESCRIPTION
 *  hide date function of category128
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void category128_hide_dt_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = 0;
    x2 = UI_device_width - 1;
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)        
    y1 = MMI_title_y + MMI_title_height;
#else 
    y1 = MMI_status_bar_height;
#endif 
    y2 = UI_device_height - MMI_button_bar_height;
    gui_set_clip(x1, y1, x2, y2);

     
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
#else 
    gui_show_image(cat128_image_x, cat128_image_y, cat128_image);
#endif 
     

    if (status_icon)
    {
        redraw_status_icon_bar(1);
    }
    gui_reset_clip();
    if (display_network_name && (disp_network_screeen_saver == 1))
    {
        idle_screen_show_network_name();
    }
}


/*****************************************************************************
 * FUNCTION
 *  DrawCate128CategoryControlArea
 * DESCRIPTION
 *  Draws the category128 category control area
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate128CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory128Screen();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory128Screen
 * DESCRIPTION
 *  Redraws the category128 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory128Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    gui_set_clip(0, (MMI_title_y + MMI_title_height), UI_device_width - 1, UI_device_height - MMI_button_bar_height);
    if (display_network_name && (disp_network_screeen_saver == 1))
    {
        idle_screen_show_network_name();
    }
    show_main_LCD_dt_display();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory128Screen
 * DESCRIPTION
 *  Exits the category128 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory128Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
    reset_softkeys();
    register_hide_status_icon_bar(1, MMI_dummy_function);
    close_status_icons();
    close_main_LCD_dt_display();
    reset_multiline_inputbox();

     
    title_bg_id = 0;
    wgui_reset_touch_title_bar_buttons();
     

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
     
    if (wgui_is_wallpaper_change())
    {
        U8 dummy_opacity;

        dm_get_scr_bg_image(&idle_scr_bg_ID, &idle_scr_bg_filename, &idle_scr_bg_x, &idle_scr_bg_y, &dummy_opacity);
        dm_set_scr_bg_image(idle_scr_bg_ID, idle_scr_bg_filename, idle_scr_bg_x, idle_scr_bg_y, idle_scr_bg_opacity);
        wgui_reset_wallpaper_change();
    }
    else
    {
        dm_set_scr_bg_image_no_draw(
            idle_scr_bg_ID,
            idle_scr_bg_filename,
            idle_scr_bg_x,
            idle_scr_bg_y,
            idle_scr_bg_opacity);
    }
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory128Screen
 * DESCRIPTION
 *  Displays the Category128 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  screen_img              [IN]        Image id
 *  date_flag               [IN]        
 *  time_flag               [IN]        
 *  service_provider        [IN]        Service provider name id
 *  history_buffer          [IN]        History buffer
 *  show_network_name       [IN]        If network name display on the screen
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory128Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 screen_img,
        U8 date_flag,
        U8 time_flag,
        U8 service_provider,
        U8 *history_buffer,
        U8 show_network_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

     
    U16 btn_prev_item_id = 0, btn_next_item_id = 0;  

     

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);

    gui_lock_double_buffer();
    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    display_network_name = show_network_name;
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    ShowStatusIconsTitle();
#endif 
     

    /*
     * if((title==0) && (title_icon==0))
     * {  status_icon=1;
     * register_hide_status_icon_bar(0,idle_screen_hide_status_icons_bar0);
     * register_hide_status_icon_bar(1,idle_screen_hide_status_icons_bar1);
     * set_status_icon_bar_clip(0,0,0,UI_device_width-1,MMI_status_bar_height-1);
     *  
     * #ifdef __MMI_KLG__
     * set_status_icon_bar_clip(1,UI_device_width-16,MMI_status_bar_height,UI_device_width-1,UI_device_height>>1);
     * #else
     * set_status_icon_bar_clip(1,UI_device_width-16,MMI_status_bar_height,UI_device_width-1,UI_device_height-MMI_softkey_height-2);
     * #endif
     *  
     * show_status_icon_bar(0);
     * show_status_icon_bar(1);
     * }
     * else if(!((title==0xffff) && (title_icon==0xffff)))
     * {
     * status_icon=2;
     * MMI_title_string=(UI_string_type)get_string(title);
     * MMI_title_icon=(PU8)get_image(title_icon);
     * }
     */
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    dm_get_scr_bg_image(&idle_scr_bg_ID, &idle_scr_bg_filename, &idle_scr_bg_x, &idle_scr_bg_y, &idle_scr_bg_opacity);
    dm_set_scr_bg_image(screen_img, NULL, -1, -1, 255);
#else /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
    dm_add_image(get_image(screen_img), NULL, NULL);
    wgui_set_wallpaper_on_bottom(MMI_TRUE);
    #ifndef __GDI_MEMORY_PROFILE_2__//062106 cat128
    cat128_image_x = cat128_image_y = 0;
    cat128_image = get_image(screen_img);
    #endif
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

     
    //check if the title bar buttons are enabled for the particular screen.
    if (wgui_is_touch_title_bar_buttons())
    {
        btn_prev_item_id = dm_add_button(
                            NULL,
                            get_image(LEFT_RED_ARROW),
                            get_image(LEFT_RED_ARROW),
                            cat129_clear_icon_bg);
        btn_next_item_id = dm_add_button(
                            NULL,
                            get_image(RIGHT_RED_ARROW),
                            get_image(RIGHT_RED_ARROW),
                            cat129_clear_icon_bg);
        title_bg_id = IMAGE_VIEW_TITLEBAR_BG;
    }
     

    set_main_LCD_dt_date_hide_function(dummy_dt_hide_function);
    set_main_LCD_dt_time_hide_function(category128_hide_dt_display);
    if (date_flag == 1 && time_flag == 1)
    {
        set_dt_display(DT_DIGITAL_CLOCK_SC1);
    }
    else if (date_flag == 1)
    {
        set_dt_display(DT_DIGITAL_CLOCK_SC2);
    }
    else if (time_flag == 1)
    {
        set_dt_display(DT_DIGITAL_CLOCK_SC3);
    }

    if (service_provider == 1)
    {
        disp_network_screeen_saver = 1;
    }
    else
    {
        disp_network_screeen_saver = 0;
    }

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory128Screen;
     
#if defined(__MMI_TOUCH_SCREEN__)
    if (wgui_is_touch_title_bar_buttons())
    {
        //BUG IMAGE VIEW START 20050917
        // register the handler for pen event down istead of pen event up.
        dm_register_button_functions(btn_prev_item_id, KEY_EVENT_UP, gMMI_touch_title_button1_down_handler);
        dm_register_button_functions(btn_next_item_id, KEY_EVENT_UP, gMMI_touch_title_button2_down_handler);
        /* BUG IMAGE VIEW END 20050917 */
    }
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
     
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_register_category_controlled_callback(DrawCate128CategoryControlArea);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY128_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory131Screen
 * DESCRIPTION
 *  Exits the category131 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory131Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
    close_main_LCD_dt_display();
}

/* store the strat x postition of category131 image,strat y postition of category131 image,image width,image height */
S32 cat131_image_x, cat131_image_y, cat131_image_width, cat131_image_height;

/* store image id of category131 image  */
PU8 cat131_image;


/*****************************************************************************
 * FUNCTION
 *  cat131_hide_analog_clock_display
 * DESCRIPTION
 *  hide function of category131 for analog clock
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void cat131_hide_analog_clock_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = 16;
    y1 = (MMI_title_y + MMI_title_height);
    x2 = UI_device_width - 16;
    y2 = UI_device_height - MMI_button_bar_height;
    gui_set_clip(x1, y1, x2, y2);
    gdi_image_draw(cat131_image_x, cat131_image_y, (U8*) cat131_image);
}


/*****************************************************************************
 * FUNCTION
 *  cat131_hide_digital_clock_display
 * DESCRIPTION
 *  hide function of category131 for digital  clock
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void cat131_hide_digital_clock_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = 0;
    y1 = 0;
    x2 = UI_device_width - 1;
    y2 = UI_device_height - 1;
    gui_set_clip(x1, y1, x2, y2);
    gdi_image_draw(cat131_image_x, cat131_image_y, (U8*) cat131_image);
}


/*****************************************************************************
 * FUNCTION
 *  DrawCate131CategoryControlArea
 * DESCRIPTION
 *  Draws the category131 category control area
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate131CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory131Screen();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory131Screen
 * DESCRIPTION
 *  Redraws the category131 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory131Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    clear_screen_with_color(UI_COLOR_BLACK);
#ifndef __MMI_UI_TECHNO_IDLESCREEN_BAR__
#ifdef __MMI_ANALOG_CLOCK__
    if (screen_saver_type == ANALOG_SCREEN_SAVER)
    {
        cat131_image = get_image(IMG_ID_DISPCHAR_ANALOG_CLK);
    }
#endif /* __MMI_ANALOG_CLOCK__ */ 

#ifdef __MMI_DIGITAL_CLOCK__
    if (screen_saver_type == DIGITAL_SCREEN_SAVER)
    {
        cat131_image = get_image(IMG_ID_DISPCHAR_DIGITAL_CLK);
    }
#endif /* __MMI_DIGITAL_CLOCK__ */ 
#endif /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */
    gui_measure_image(cat131_image, &cat131_image_width, &cat131_image_height);
    cat131_image_x = (UI_device_width >> 1) - (cat131_image_width >> 1);
    cat131_image_y = (UI_device_height >> 1) - (cat131_image_height >> 1);
    gdi_image_draw(cat131_image_x, cat131_image_y, (U8*) cat131_image);
    show_main_LCD_dt_display();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory131Screen
 * DESCRIPTION
 *  show analog or digital clock screen saver depends icon_screensaver value
 * PARAMETERS
 *  icon_screensaver        [IN]        Id of the image
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory131Screen(U16 icon_screensaver, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);

    entry_full_screen();
    gui_lock_double_buffer();
    clear_category_screen_key_handlers();
    if (screen_saver_type == ANALOG_SCREEN_SAVER)
    {
    #ifdef __MMI_ANALOG_CLOCK__
        analog_clock_initialize();
        set_main_LCD_dt_date_hide_function(dummy_dt_hide_function);
        set_main_LCD_dt_time_hide_function(cat131_hide_analog_clock_display);
        set_dt_display(DT_ANALOG_CLOCK_SC);
    #endif /* __MMI_ANALOG_CLOCK__ */ 
    }
    else if (screen_saver_type == DIGITAL_SCREEN_SAVER)
    {
    #ifdef __MMI_DIGITAL_CLOCK__
        set_main_LCD_dt_date_hide_function(dummy_dt_hide_function);
        set_main_LCD_dt_time_hide_function(cat131_hide_digital_clock_display);
        set_dt_display(DT_DIGITAL_CLOCK_SC1);
    #endif /* __MMI_DIGITAL_CLOCK__ */ 
    }
    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory131Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_register_category_controlled_callback(DrawCate131CategoryControlArea);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY131_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}

#ifdef __MMI_LCD_PARTIAL_ON__


/*****************************************************************************
 * FUNCTION
 *  Category161ChangeTime
 * DESCRIPTION
 *  change the time of category161
 * PARAMETERS
 *  t       [IN]        Time
 * RETURNS
 *  void
 *****************************************************************************/
void Category161ChangeTime(UI_time *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cat161_time = *t;
}

#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)        
#define CAT161_ICON_GAP    (3)
#else 
#define CAT161_ICON_GAP    (2)
#endif 

extern stFontAttribute wgui_dialer_box_f1;


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory161Screen
 * DESCRIPTION
 *  Redraws the category161 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory161Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c;
    S32 twidth, theight;
    S32 y_round, partial_on_height = UI_LCD_PARTIAL_ON_HEIGHT;
    S32 x1, x2, y1, y2, y2_last = 0;     
    S32 text_x;
    S32 icon_width, icon_height;
    int icon_index, num_icon_displayed;
    U8 timestr[64];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();

#if defined(BLACK_BACKLIGHT)
    c.r = c.g = c.b = 0;
#elif defined(BLUE_BACKLIGHT)
    c.r = c.g = 0;
    c.b = 255;
#elif defined(WHITE_BACKLIGHT)
    c.r = c.g = c.b = 255;
#else 
    c.r = c.g = c.b = 255;
#endif 
    c.alpha = 100;

#ifndef WIN32
    y_round = lcd_partial_display_align_line(MAIN_LCD);
    if (y_round > 1 && (partial_on_height % y_round) != 0)
    {
        partial_on_height += (y_round - (partial_on_height % y_round));
    }
#endif /* WIN32 */ 

    y_round = UI_device_height /partial_on_height;

    x1 = 0;
    y1 = (cat161_time.nMin % y_round) * partial_on_height;
    x2 = UI_device_width - 1;
    y2 = y1 + partial_on_height - 1;

    /* Dialing font has only english characters */
    time_string(&cat161_time, (UI_string_type) timestr, DT_IDLE_SCREEN_ENGLISH_ONLY);

    gui_push_clip();
    gui_push_text_clip();

    gui_set_clip(0, 0, UI_device_width - 1, UI_device_height - 1);
    gui_set_text_clip(0, 0, UI_device_width - 1, UI_device_height - 1);
#if	defined (__MMI_MAINLCD_128X160__) || defined (__MMI_MAINLCD_128X128__)
{
	S32 txt_y,icon_x;
	S32 icon_y;
	txt_y = y1 + ((partial_on_height - theight) >> 1);

	if(txt_y > (UI_device_height>>1))
	{
		icon_y = y1  - partial_on_height +((partial_on_height - icon_height) >> 1) ;
		y1  = y1  - partial_on_height ;
	}
	else
	{
		icon_y = y2 + ((partial_on_height - icon_height) >> 1) ;
		y2 = y2 + partial_on_height  ;
	}

//clear last display area
    if (cat161_last_y_offset >= 0 && cat161_last_y_offset != y1)
    {
        y2_last = cat161_last_y_offset + partial_on_height*2 + 1;
    }
    if (cat161_last_y_offset >= 0 && cat161_last_y_offset != y1)
    {
        gui_fill_rectangle(x1, cat161_last_y_offset, x2, y2_last, c);
    }
    gui_fill_rectangle(x1, y1, x2, y2, c);

#ifdef __MMI_MAINLCD_128X160__
    gui_set_font(&MMI_medium_bold_font);
#else
    gui_set_font(&wgui_dialer_box_f1);
#endif
	
    gui_measure_string((UI_string_type) timestr, &twidth, &theight);
    text_x = (UI_device_width - twidth) >> 1;
//clear last display area end
	
//show status icon in blank space start
    if (MMI_status_icons_partial_display_n_icons > 0)
    {
        gui_measure_image(MMI_status_icons[0].icon_data, &icon_width, &icon_height);
        if ((CAT161_ICON_GAP + (icon_width + CAT161_ICON_GAP) * MMI_status_icons_partial_display_n_icons)
            * 2 > UI_device_width)
        {
            /* Do not center the text */
            num_icon_displayed = (UI_device_width  - (CAT161_ICON_GAP * 2)) / (icon_width + CAT161_ICON_GAP);
            if (num_icon_displayed > MMI_status_icons_partial_display_n_icons)
            {
                num_icon_displayed = MMI_status_icons_partial_display_n_icons;
            }
	     
        }
        else
        {
            num_icon_displayed = MMI_status_icons_partial_display_n_icons;
        }
		
	 icon_x =  (UI_device_width -  (icon_width + CAT161_ICON_GAP) * num_icon_displayed) >> 1;

        for (icon_index = 0; icon_index < num_icon_displayed; icon_index++)
        {
            if (icon_height > partial_on_height)
            {
                break;  /* Abnormal case */
            }

            gui_show_transparent_image(
                icon_x + (CAT161_ICON_GAP + icon_width) * icon_index,
                icon_y,
                MMI_status_icons[MMI_status_icons_list_partial_display[icon_index]].icon_data,
                0);
        }
    }
//show status icon in blank space end


#if defined(BLACK_BACKLIGHT)
    gui_set_text_color(gui_color(255, 255, 255));
#else
    gui_set_text_color(gui_color(0, 0, 0));
#endif

//show time string here
    if (r2lMMIFlag)
    {
        gui_move_text_cursor((text_x + twidth - 1), txt_y);
    }
    else
    {
        gui_move_text_cursor(text_x, txt_y);
    }
    gui_print_text((UI_string_type) timestr);
    gui_set_font(&MMI_medium_font);
//show time string end

}
#else
//clear last display area
    if (cat161_last_y_offset >= 0 && cat161_last_y_offset != y1)
    {
        y2_last = cat161_last_y_offset + partial_on_height - 1;
    }
    if (cat161_last_y_offset >= 0 && cat161_last_y_offset != y1)
    {
        gui_fill_rectangle(x1, cat161_last_y_offset, x2, y2_last, c);
    }
    gui_fill_rectangle(x1, y1, x2, y2, c);

    #if defined(__PROJECT_GALLITE_C01__)
    gui_set_font( &MMI_large_font);
    #else
    gui_set_font(&wgui_dialer_box_f1);
    #endif

    gui_measure_string((UI_string_type) timestr, &twidth, &theight);
    text_x = (UI_device_width - twidth) >> 1;
//clear last display area end
//show status icon in blank space start
    if (MMI_status_icons_partial_display_n_icons > 0)
    {
        gui_measure_image(MMI_status_icons[0].icon_data, &icon_width, &icon_height);
        if ((CAT161_ICON_GAP + (icon_width + CAT161_ICON_GAP) * MMI_status_icons_partial_display_n_icons)
            * 2 + twidth > UI_device_width)
        {
            /* Do not center the text */
            num_icon_displayed = (UI_device_width - twidth - (CAT161_ICON_GAP * 2)) / (icon_width + CAT161_ICON_GAP);
            if (num_icon_displayed > MMI_status_icons_partial_display_n_icons)
            {
                num_icon_displayed = MMI_status_icons_partial_display_n_icons;
            }
            text_x = CAT161_ICON_GAP + (icon_width + CAT161_ICON_GAP) * num_icon_displayed;
        }
        else
        {
            text_x = (UI_device_width - twidth) >> 1;
            num_icon_displayed = MMI_status_icons_partial_display_n_icons;
        }

        for (icon_index = 0; icon_index < num_icon_displayed; icon_index++)
        {
            if (icon_height > partial_on_height)
            {
                break;  /* Abnormal case */
            }

            gui_show_transparent_image(
                CAT161_ICON_GAP + (CAT161_ICON_GAP + icon_width) * icon_index,
                y1 + ((partial_on_height - icon_height) >> 1),
                MMI_status_icons[MMI_status_icons_list_partial_display[icon_index]].icon_data,
                0);
        }
    }
//show status icon in blank space end
#if defined(BLACK_BACKLIGHT)
    gui_set_text_color(gui_color(255, 255, 255));
#else
    gui_set_text_color(gui_color(0, 0, 0));
#endif

//show time string here
    if (r2lMMIFlag)
    {
        gui_move_text_cursor((text_x + twidth - 1), y1 + ((partial_on_height - theight) >> 1));
    }
    else
    {
        gui_move_text_cursor(text_x, y1 + ((partial_on_height - theight) >> 1));
    }
    gui_print_text((UI_string_type) timestr);
    gui_set_font(&MMI_medium_font);
//show time string end

#endif
    gui_pop_text_clip();
    gui_pop_clip();

    gui_unlock_double_buffer();

    /* Try to reduce the BLT region in order to save computation power */
    if (cat161_last_y_offset == -1) /* First time */
    {
        gdi_layer_blt_base_layer(0, 0, UI_device_width - 1, UI_device_height - 1);
    }
    else
    {
        if (cat161_last_y_offset != y1)
        {
           gdi_layer_blt_base_layer(x1, cat161_last_y_offset, x2, y2_last);
        }
       gdi_layer_blt_base_layer(x1, y1, x2, y2);
    }
    cat161_last_y_offset = y1;

#ifndef WIN32
    lcd_partial_on(MAIN_LCD, y1, y2);
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory161Screen
 * DESCRIPTION
 *  Displays the category161 screen
 * PARAMETERS
 *  t       [IN]        Time
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory161Screen(UI_time *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c;

    cat161_time = *t;
    cat161_last_y_offset = -1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(BLACK_BACKLIGHT)
    c.r = c.g = c.b = 0;
#elif defined(BLUE_BACKLIGHT)
    c.r = c.g = 0;
    c.b = 255;
#elif defined(WHITE_BACKLIGHT)
    c.r = c.g = c.b = 255;
#else 
    c.r = c.g = c.b = 255;
#endif 
    c.alpha = 100;

    entry_full_screen();

    gui_lock_double_buffer();
    gui_push_clip();
    gui_set_clip(0, 0, UI_device_width - 1, UI_device_height - 1);
    /* Delay filling white until RedrawCategory161Screen. We do not redraw the whole screen for each update. */
    gui_fill_rectangle(0, 0, UI_device_width - 1, UI_device_height - 1, c);

    gui_pop_clip();
    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory161Screen;
    RedrawCategoryFunction = RedrawCategory161Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;

    RedrawCategory161Screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory161Screen
 * DESCRIPTION
 *  Exits the category161 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory161Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifndef WIN32
    lcd_partial_off(MAIN_LCD);
#endif 
    /* Because partial display is text on white background,
       it looks weird if new popup comes and we don't clear background here. */
#if defined(BLACK_BACKLIGHT)
    gdi_layer_clear(GDI_COLOR_BLACK);
#elif defined(BLUE_BACKLIGHT)
    gdi_layer_clear(GDI_COLOR_BLUE);
#elif defined(WHITE_BACKLIGHT)
    gdi_layer_clear(GDI_COLOR_WHITE);
#else
    gdi_layer_clear(GDI_COLOR_WHITE);
#endif

    leave_full_screen();

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}

#endif /* __MMI_LCD_PARTIAL_ON__ */ 

 
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__


/*****************************************************************************
 * FUNCTION
 *  show_network_detail_on_animated_highlighter_on_idlescreen
 * DESCRIPTION
 *  Handle title text on idlescreen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_network_detail_on_animated_highlighter_on_idlescreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (idle_screen_type == ANALOG_IDLE_SCREEN || idle_screen_type == DIGITAL_IDLE_SCREEN)
    {
        idle_screen_show_network_name();
    }
    else
    {
        idle_screen_show_network_details();
    }
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    enable_statusbar_fill_background();
    refresh_fixed_status_icon(STATUS_ICON_SIGNAL_STRENGTH, 0);
    refresh_fixed_status_icon(STATUS_ICON_BATTERY_STRENGTH, 1);
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
}

#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
 
#if defined(__MMI_NOKIA_STYLE_N800__)|| defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)

#include"globalmenuitems.h"
#include "shortcutsdefs.h"
extern void HighlightClndrMenu(void);
extern void HighlightInboxHandler(void);
extern void highlight_mainmenu_multimedia(void);
extern void mmi_camera_hilight_app(void);
extern void mmi_profiles_entry_main_screen(void);
extern void HighlightAudioPlayer(void);
extern void mmi_fmrdo_highlight_menu(void);
extern void HighlightTDLMenu(void);
extern void mmi_imgview_highlight_app(void);
extern void HighlightAlmMenu(void);
extern void DsmMsn(void);

#if 0
const mmi_frm_hilite_hdlr_struct gudingitemid[7] = 
{
    {ORGANIZER_CALENDER_MENU, HighlightClndrMenu},
    {ORGANIZER_TODOLIST_MENU, HighlightTDLMenu},
    {MENU_ID_CAMERA_APP, mmi_camera_hilight_app},
    {MENU_ID_IMGVIEW_APP,mmi_imgview_highlight_app},//mym 20091015
    {AUDIO_PLAYER_MAIN_MENUID, HighlightAudioPlayer},
    {MENU_ID_FMRDO_MAIN, mmi_fmrdo_highlight_menu},//sa 0624
};
#endif
const U16 gudingitemid[7] = 
{
    ORGANIZER_CALENDER_MENU, 
    ORGANIZER_TODOLIST_MENU, 
    MENU_ID_CAMERA_APP, 
    MENU_ID_IMGVIEW_APP,
    MENU_ID_IMGVIEW_APP,
    AUDIO_PLAYER_MAIN_MENUID, 
    MENU_ID_FMRDO_MAIN
};

U8 N800_col = 0;
U8 N800_row = 0;
U8 N800_idx = 0;
U8 N800_last_highlight = 0;

extern shct_context_struct g_shct_cntx;
extern hiliteInfo maxHiliteInfo[MAX_HILITE_HANDLER];			/* available hilite func ptrs  */
#if 0 //!defined(__MMI_NOKIA_STYLE_N800__)
/*****************************************************************************
 * FUNCTION
 *  gui_idle_touch_screen_display_shorcut_item
 * DESCRIPTION
 *  This function is Display individual shorcut item on Touch idle screen
 * PARAMETERS
 *  index       [IN]        idle screen shortcut
 * RETURNS
 *  void
 
 *  void
 *****************************************************************************/
void gui_idle_touch_screen_display_shorcut_item(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    touch_idle_shortcuts[index].state = IDLE_TOUCH_SHORTCUT_UNPRESSED;
    
    x1 = touch_idle_shortcuts[index].x;
    y1 = touch_idle_shortcuts[index].y;
    x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;
    y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
    gui_lock_double_buffer();
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    gui_show_image(x1, y1, (PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + index)));
    gui_pop_clip();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(x1, y1, x2, y2);
}
#endif

#if defined(__MMI_NOKIA_STYLE_N800__)
void gui_idle_touch_screen_display_shorcut_item1(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
    S32 x1, y1, x2, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = touch_idle_shortcuts[index].x;
    y1 = touch_idle_shortcuts[index].y;
    x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;
    y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
     
    gui_lock_double_buffer();
     
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
#ifdef __GDI_MEMORY_PROFILE_2__
    /* gui_fill_rectangle(x1, y1, x2, y2, wgui_transparent_color); */
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    /* gui_show_image(touch_idle_shortcuts[index].x,touch_idle_shortcuts[index].y,GetImage((U16)(IMG_IDLE_MAINMENU_SHORCUT+index))); */
    gui_show_image(x1, y1, (PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + index)));
    display_touch_idle_screen_shortcuts1();
    gui_pop_clip();
    gui_unlock_double_buffer();
    /* gui_BLT_double_buffer(touch_idle_shortcuts[index].x,touch_idle_shortcuts[index].y,touch_idle_shortcuts[index].x+touch_idle_shortcuts[index].width,touch_idle_shortcuts[index].y+touch_idle_shortcuts[index].height); */
    gui_BLT_double_buffer(x1, y1, x2, y2);
     
}



/*****************************************************************************
 * FUNCTION
 *  gui_idle_touch_screen_display_pressed_shorcut_item
 * DESCRIPTION
 *  This function is Display individual pressed shorcut item on Touch idle screen
 * PARAMETERS
 *  index       [IN]        pressed shortcut index
 * RETURNS
 *  void
 
 *  void
 *****************************************************************************/
void gui_idle_title(S32 index)//yan
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
   // U8 data;
   // S16 error;
  //  S32 width, x, y;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_draw_solid_rect(1, 20, 100, 40, GDI_COLOR_TRANSPARENT);
    gui_lock_double_buffer();
    gui_push_clip();
    gui_set_clip(1, 20, 100, 40);//yan
    gui_set_font(&MMI_medium_font);
    gui_set_text_color(gui_color(255,255, 255));
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(36, 23);
        gui_print_text(get_string( STR_IDLE_MAINMENU_SHORCUT+index));
    }
    else
    {
        gui_move_text_cursor(1, 23);
        gui_print_text(get_string( STR_IDLE_MAINMENU_SHORCUT+index));//yan 
    }

    gui_pop_clip();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(1, 20, 100, 40);
}
#endif
#if 0 //!defined(__MMI_NOKIA_STYLE_N800__)
/*****************************************************************************
 * FUNCTION
 *  gui_idle_touch_screen_display_pressed_shorcut_item
 * DESCRIPTION
 *  This function is Display individual pressed shorcut item on Touch idle screen
 * PARAMETERS
 *  index       [IN]        pressed shortcut index
 * RETURNS
 *  void
 
 *  void
 *****************************************************************************/
void gui_idle_touch_screen_display_pressed_shorcut_item(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
  //  U8 data;
  //  S16 error;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    touch_idle_shortcuts[index].state = IDLE_TOUCH_SHORTCUT_PRESSED;

    x1 = touch_idle_shortcuts[index].x;
    y1 = touch_idle_shortcuts[index].y;
    x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;
    y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
    gui_lock_double_buffer();
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    gui_show_image(x1, y1, (PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT_PRESSED + index)));   

    gui_pop_clip();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(x1, y1, x2, y2);
}
#endif
#if defined(__MMI_NOKIA_STYLE_N800__)
void gui_idle_touch_screen_display_pressed_shorcut_item1(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
    S32 x1, y1, x2, y2;
  //  U8 data;
  //  S16 error;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    //  MMI_DBG_ASSERT(index >= 0 && index < MAX_IDLE_SCREEN_SHORTCUTS);
     
    x1 = touch_idle_shortcuts[index].x;
    y1 = touch_idle_shortcuts[index].y;
    x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;
    y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
     
    gui_lock_double_buffer();
     
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
#ifdef __GDI_MEMORY_PROFILE_2__
    /* gui_fill_rectangle(x1, y1, x2, y2, wgui_transparent_color); */
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);  
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    draw_wallpaper();
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    gui_show_image(x1, y1, (PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT_PRESSED + index)));
    //#endif
    display_touch_idle_screen_shortcuts1();//jiaquan
    gui_pop_clip();
    gui_unlock_double_buffer();
    /* gui_BLT_double_buffer(touch_idle_shortcuts[index].x,touch_idle_shortcuts[index].y,touch_idle_shortcuts[index].x+touch_idle_shortcuts[index].width,touch_idle_shortcuts[index].y+touch_idle_shortcuts[index].height); */
    gui_BLT_double_buffer(x1, y1, x2, y2);
     
}


/*****************************************************************************
 * FUNCTION
 *  gui_idle_touch_screen_shortcut_translate_pen_position
 * DESCRIPTION
 *  This function finds index of shorcut item on which pen event came
 * PARAMETERS
 *  x           [IN]        Pen's x position
 *  y           [IN]        Pen's y position
 *  index       [OUT]       Index of selected item
 * RETURNS
 *  void
 
 *  void
 *****************************************************************************/
void gui_idle_touch_screen_shortcut_translate_pen_position(S32 x, S32 y, S32 *index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *index = -1;

    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        if (x >= touch_idle_shortcuts[i].x && x <= touch_idle_shortcuts[i].x + touch_idle_shortcuts[i].width &&
        y >= touch_idle_shortcuts[i].y && y <= touch_idle_shortcuts[i].y + touch_idle_shortcuts[i].height)
        {
            *index = i;
            break;
        }
    }
}


#if 0
void RIGHTMOVE()
{
    s32 index;
    iindex1++;
    up1=0;
    index=iindex1;//iindex1=1
    a=iindex1;//a=1
    if(iindex2==6)
    {
        gui_idle_touch_screen_display_shorcut_item(6);
    }
    else
    {
        gui_idle_touch_screen_display_shorcut_item(b);
    }
    if(index!=0)
    {
        gui_idle_touch_screen_display_shorcut_item1(0);
    }
    if((index<5)&&(index!=0)) 
    { 
        gui_idle_touch_screen_display_pressed_shorcut_item(index);   
        gui_idle_title(index);
        gui_idle_touch_screen_display_shorcut_item(index-1);
    }
    else
    {
        index=0;
        iindex1=0;
        gui_idle_touch_screen_display_pressed_shorcut_item(index);   
        gui_idle_title(index);
        gui_idle_touch_screen_display_shorcut_item(4);
    }
}
void LEFTMOVE()
{  
	s32 index;
	up1=0;
	iindex1--;
	index=iindex1;
	a=iindex1;
	if(iindex2==6)
	{
		gui_idle_touch_screen_display_shorcut_item1(6);
	}
	else
	{
		gui_idle_touch_screen_display_shorcut_item1(b);
	}
	if(index!=0)
	{
		gui_idle_touch_screen_display_shorcut_item(0);
	}
	if((index>=0)) 
	{  
		gui_idle_touch_screen_display_pressed_shorcut_item(index);   
		gui_idle_title(index);
		gui_idle_touch_screen_display_shorcut_item(index+1);
	}
	else
	{
		index=4;
		iindex1=4;
		gui_idle_touch_screen_display_pressed_shorcut_item(index);   
		gui_idle_title(index);
		gui_idle_touch_screen_display_shorcut_item(0);
	}
}
void DOWNMOVE()
{
    s32 index;
    iindex1=0;
    iindex2++;
    index=iindex2;
    b=iindex2;

    if((iindex1<7/*8*/)&&(iindex1!=0))
    {
        gui_idle_touch_screen_display_shorcut_item(iindex1);
    }
    else if(iindex1==6)
    {
        gui_idle_touch_screen_display_shorcut_item(6);//(7);
    }	
    else
    {
        gui_idle_touch_screen_display_shorcut_item(a);
    }

    if((index<7/*8*/)&&(index!=4)) 
    { 
        gui_idle_touch_screen_display_shorcut_item(0);
        gui_idle_touch_screen_display_shorcut_item1(index-1);

        gui_idle_touch_screen_display_pressed_shorcut_item1(index);   
        gui_idle_title(index);
    }
    else
    {
        index=0;
        iindex2=4;
        gui_idle_touch_screen_display_shorcut_item1(6/*7*/);

        gui_idle_touch_screen_display_pressed_shorcut_item(0);   
        gui_idle_title(0);
    }
    if(index==5)
    {
        up1=5;
    }
    else if(index==6)
    {
        up1=6;
    }
    else
    {
        up1=0;
    }
}
void UPMOVE()
{   
    s32 index;
    iindex2--;
    index=iindex2;
    b=iindex2;
    iindex1=0;
    if((iindex1<6/*7*/)&&(iindex1!=0))
    {
        gui_idle_touch_screen_display_shorcut_item(iindex1);
    }
    else if(iindex1==6)
    {
        gui_idle_touch_screen_display_shorcut_item(6);
    }	
    else
    {
        gui_idle_touch_screen_display_shorcut_item(a);
    }
    if((index>=5)) 
    {  
        gui_idle_touch_screen_display_shorcut_item(0);
        gui_idle_touch_screen_display_shorcut_item1(index+1);

        gui_idle_touch_screen_display_pressed_shorcut_item1(index);   
        gui_idle_title(index);
    }
    else
    {
        index=0;
        iindex2=7/*8*/;
        gui_idle_touch_screen_display_shorcut_item1(5);

        gui_idle_touch_screen_display_pressed_shorcut_item(0);   
        gui_idle_title(0);
    }
    if(index==5)
    {
        up1=5;
    }
    else if(index==6)
    {
        up1=6;
    }
    else
    {
        up1=0;
    }
}
void N800_gui_idle_redraw_shortcut_icon()
{   
    s32 index;
    index=iindex2;

    switch(up1)
    {
        case 5:
            index = 5;
            break;
        case 6:
            index  = 6;
            break;
        default:
            index = iindex1;
            break;
    }

    gui_idle_touch_screen_display_pressed_shorcut_item1(index);   
    gui_idle_title(index);

}
void EntryMainMove001(void)
{
    if(up1==5)
    {
        g_shct_cntx.DediShctList[2]=(U16)gudingitemid[5];
    }
    else if(up1==6)
    {
        g_shct_cntx.DediShctList[2]=(U16)gudingitemid[6];
    }
    else
    {
        g_shct_cntx.DediShctList[2]=(U16)gudingitemid[iindex1];
    }

}
#else
void RIGHTMOVE()
{

    N800_row = 0;
    
    if(N800_col == 4 )
        N800_col = 0;
    else
        N800_col++;

     N800_last_highlight = N800_idx;
    N800_idx = N800_col;

    
    gui_idle_touch_screen_display_shorcut_item1(N800_last_highlight);
    gui_idle_touch_screen_display_pressed_shorcut_item1(N800_idx);   
    gui_idle_title(N800_idx);
        
}
void LEFTMOVE()
{  
    N800_row = 0;
    if(N800_col ==0 )
        N800_col = 4;
    else
        N800_col--;
    
    N800_last_highlight = N800_idx;
    N800_idx = N800_col;
    
    gui_idle_touch_screen_display_shorcut_item1(N800_last_highlight);
    gui_idle_touch_screen_display_pressed_shorcut_item1(N800_idx);   
    gui_idle_title(N800_idx);

}
void DOWNMOVE()
{
    N800_col = 0;
    if(N800_row ==2 )
        N800_row = 0;
    else
        N800_row++;
    

     N800_last_highlight = N800_idx;
    if(N800_row == 0 )
    {
        N800_idx = 0;
    }
    else
    {
        N800_idx = 4+ N800_row;

    }
    
    gui_idle_touch_screen_display_shorcut_item1(N800_last_highlight);
    gui_idle_touch_screen_display_pressed_shorcut_item1(N800_idx);   
    gui_idle_title(N800_idx);

}
void UPMOVE()
{   

    N800_col = 0;
    if(N800_row ==0 )
        N800_row = 2;
    else
        N800_row--;
    

     N800_last_highlight = N800_idx;

    if(N800_row == 0 )
    {
        N800_idx = 0;
    }
    else
    {
        N800_idx = 4+ N800_row;
    }
    
    gui_idle_touch_screen_display_shorcut_item1(N800_last_highlight);
    gui_idle_touch_screen_display_pressed_shorcut_item1(N800_idx);   
    gui_idle_title(N800_idx);

}

void EntryMainMove001(void)
{
     g_shct_cntx.DediShctList[2]=(U16)gudingitemid[N800_idx];
}
#endif

void EntryMainMove()
{
            g_shct_cntx.IsKeyRetrieving = TRUE;
           (*maxHiliteInfo[g_shct_cntx.DediShctList[2]].entryFuncPtr) ();

            g_shct_cntx.IsKeyRetrieving = FALSE;
            execute_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY);
}
void YITHEMEZUFU3(void)
{
    EntryMainMove001();
    EntryMainMove();
}
extern void SetCurrentProfileAsActivatedProfile(void);
void YITHEMEZUFU2(void)
{
    SetCurrentProfileAsActivatedProfile();
	SetKeyHandler(mmi_nokia_shortcut_entryfn,KEY_ENTER,KEY_EVENT_UP);
    //SetKeyHandler(YITHEMEZUFU3, KEY_ENTER, KEY_EVENT_UP);
}
#endif

void mmi_shortcut_left_move_event_down()
{
	ExtendIdleKeyLeftHandler();
}
void mmi_shortcut_right_move_event_down()
{
	ExtendIdleKeyRightHandler();
}
void mmi_shortcut_left_move_event_up()
{
	ExtendIdleKey_Left_or_Right_UpHandler();
}
void mmi_shortcut_right_move_event_up()
{
	ExtendIdleKey_Left_or_Right_UpHandler();
}

void mmi_shortcut_enter_event_up()
{
        mmi_nokia_shortcut_entryfn();
}

void gui_idle_touch_screen_display_shorcut_item(S32 index)
{
	S32 x1, y1, x2, y2;

	if(curr_vertical_hilite_index)
		return;
	MMI_DBG_ASSERT(index >= 0 && index < MAX_IDLE_SCREEN_HOR_SHORTCUTS);
	
	touch_idle_shortcuts[index].state = IDLE_TOUCH_SHORTCUT_UNPRESSED;
#if defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	x1 = touch_idle_shortcuts[index].x;	
	y1 = touch_idle_shortcuts[index].y;
	x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;	
	y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
#else
	x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x;	
	y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y;
	x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x + touch_idle_shortcuts[index].width - 1;	
	y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y + touch_idle_shortcuts[index].height - 1;
#endif
	gui_lock_double_buffer();
	gui_push_clip();
	gui_set_clip(x1,y1,x2,y2);
#if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
	gdi_draw_solid_rect(x1,y1,x2,y2,GDI_COLOR_TRANSPARENT);
	gui_show_image(x1, y1,(PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + index)));
#else
	gui_draw_gradient_rounded_rectangle(x1, y1, x2, y2,
			gui_color(192,192,192),
			gui_color(255,255,255),
			0);
	//gui_fill_rectangle(x1, y1, x2, y2, gui_color(255,0,0)); 
	gui_show_image(x1, y1,(PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + index)));
#endif
    gui_pop_clip();	
	gui_unlock_double_buffer();
	gui_BLT_double_buffer(0, 18, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height-1);
}

void gui_idle_touch_screen_display_pressed_shorcut_item(S32 index)
{
	S32 x1, y1, x2, y2;

	MMI_DBG_ASSERT((index >= 0) && (index < MAX_IDLE_SCREEN_HOR_SHORTCUTS));
#if defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	x1 = touch_idle_shortcuts[index].x;
	y1 = touch_idle_shortcuts[index].y;
	x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;
	y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
#else
	if (index & (1 <<30))
	{
		x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x;
		y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y;
		x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x + touch_idle_shortcuts[index].width - 1;
		y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y + touch_idle_shortcuts[index].height - 1;	
	}
	else if (index & (1 << 31))
	{
		x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2-1].x;
		y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2-1].y;
		x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2-1].x + touch_idle_shortcuts[index].width - 1;
		y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2-1].y + touch_idle_shortcuts[index].height - 1;	
	}
	else
	{
		x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2+1].x;
		y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2+1].y;
		x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2+1].x + touch_idle_shortcuts[index].width - 1;
		y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2+1].y + touch_idle_shortcuts[index].height - 1;		
	}
#endif

	gui_lock_double_buffer();
	gui_push_clip();
	gui_set_clip(x1, y1, x2, y2);
	//gui_fill_rectangle(x1, y1, x2, y2,gui_color(255,0,0));
#ifdef __MMI_BLACKBERRY_IDLE_SHORTCUT__
	gui_show_image(x1,y1,(PU8) GetImage((U16) (IMG_IDLE_QQ_SHORTCUT + index)));
#else
	gui_draw_gradient_rounded_rectangle(x1, y1, x2, y2,
		gui_color(192,192,192),gui_color(192,192,192),0);
	gui_show_image(x1, y1, 
		(PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + index)));
#endif
#if 0
	gui_show_image(touch_idle_shortcuts[0].x-9,touch_idle_shortcuts[0].y+8,
		(PU8)GetImage(IMG_LEFT_ARROW));
	gui_show_image(166,touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS-1].y+8,
		(PU8)GetImage(IMG_RIGHT_ARROW));	
#endif	
	gui_pop_clip();	
	gui_unlock_double_buffer();
	gui_BLT_double_buffer(0, 18, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height-1);
	
}

#if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
void mmi_shortcut_up_move(void)
{
	if(0 <= curr_vertical_hilite_index)
        curr_vertical_hilite_index--;
     
	display_touch_idle_screen_shortcuts();
}

void mmi_shortcut_down_move(void)
{
	if(0>= curr_vertical_hilite_index)
      curr_vertical_hilite_index++;
	display_touch_idle_screen_shortcuts();
}
#else
void mmi_shortcut_up_move(void)
{
	curr_vertical_hilite_index--;
	if(-1 == curr_vertical_hilite_index)
		curr_vertical_hilite_index = IDLE_SCREEN_SHORTCUT_LINE_COUNTS - 1;
	display_touch_idle_screen_shortcuts();
}

void mmi_shortcut_down_move(void)
{
	curr_vertical_hilite_index++;
	if(IDLE_SCREEN_SHORTCUT_LINE_COUNTS == curr_vertical_hilite_index)
		curr_vertical_hilite_index = 0;
	display_touch_idle_screen_shortcuts();
}
#endif
void gui_idle_touch_screen_display_scroll_shorcut_item(S32 index)
{
	S32 i; 
#if !defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)	
	S32 x1, y1, x2, y2;
#endif
	gui_lock_double_buffer();
	gui_push_clip();
	gui_set_clip(0, touch_idle_shortcuts[0].y, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height+1);	
	gdi_draw_solid_rect(0, touch_idle_shortcuts[0].y, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height+1,GDI_COLOR_TRANSPARENT);
#if defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	for (i = 0; i < MAX_IDLE_SCREEN_HOR_SHORTCUTS; i++)
	{
		if((index == i) && (0 == curr_vertical_hilite_index))
			gui_show_image(touch_idle_shortcuts[i].x,
							touch_idle_shortcuts[i].y,
							get_image((UI_image_ID_type)(IMG_IDLE_MAINMENU_SHORCUT_PRESSED + i)));

			/*gui_draw_gradient_rounded_rectangle(
												touch_idle_shortcuts[index].x,
												touch_idle_shortcuts[index].y,
												touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width,
												touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height,
												gui_color(255,255,255),
												gui_color(192,192,192),
												0);*/
		else
			gui_show_image(touch_idle_shortcuts[i].x,
							touch_idle_shortcuts[i].y,
							get_image((UI_image_ID_type) (IMG_IDLE_MAINMENU_SHORCUT + i)));
			
	}
#else
	for (i = 0; i < MAX_IDLE_SCREEN_HOR_SHORTCUTS; i++)
	{
		S32 j = i + index - (MAX_IDLE_SCREEN_HOR_SHORTCUTS/2);
		
		if(j < 0) j += MAX_IDLE_SCREEN_HOR_SHORTCUTS;
		if(j >= MAX_IDLE_SCREEN_HOR_SHORTCUTS) j -= MAX_IDLE_SCREEN_HOR_SHORTCUTS;
		
		gui_show_image(touch_idle_shortcuts[i].x,
			touch_idle_shortcuts[i].y,
			get_image((UI_image_ID_type) (IMG_IDLE_MAINMENU_SHORCUT + j)));		
	}
	if(!curr_vertical_hilite_index)
	{
          #if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
          #if 0
			gui_draw_gradient_rounded_rectangle(62,
											MMI_BLACKBERRY_IDLE_SHRTCUT_IMAGE_Y,
											92,
											MMI_BLACKBERRY_IDLE_SHRTCUT_IMAGE_Y+26,
											gui_color(255,255,255),
											gui_color(255,255,255),
											0);
			gui_show_image(62, MMI_BLACKBERRY_IDLE_SHRTCUT_IMAGE_Y,(PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + index)));
        #endif
            gui_show_image(touch_idle_shortcuts[1].x, touch_idle_shortcuts[1].y,(PU8) GetImage((U16) (IMG_IDLE_QQ_SHORTCUT + index)));

        #else
		x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x;
		y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y;
		x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x + touch_idle_shortcuts[index].width - 1;
		y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y + touch_idle_shortcuts[index].height - 1;
		gui_draw_gradient_rounded_rectangle(x1, y1, x2, y2,
			gui_color(255,255,255),
			gui_color(192,192,192),
			0);
		gui_show_image(x1, y1, 
			(PU8) GetImage((U16) (IMG_IDLE_MAINMENU_SHORCUT + index)));
          #endif
	}
#endif	
	//MMI_title_string = (UI_string_type)get_string(STR_IDLE_MAINMENU_SHORCUT + index);
	//show_item_title(T800_SHORTCUT_ITEM_STRING_START_X,T800_SHORTCUT_ITEM_STRING_START_Y);
#if 0
	gui_show_image(touch_idle_shortcuts[0].x-9,touch_idle_shortcuts[0].y+8,
		(PU8)GetImage(IMG_LEFT_ARROW));
	gui_show_image(166,touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS-1].y+8,
		(PU8)GetImage(IMG_RIGHT_ARROW));
#endif
	gui_pop_clip();
	gui_unlock_double_buffer();
	//gdi_draw_solid_rect(0, touch_idle_shortcuts[0].y, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height-1,GDI_COLOR_TRANSPARENT);
	gui_BLT_double_buffer(0, touch_idle_shortcuts[0].y, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height+1);
}

#if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
extern void goto_main_menu(void);
#endif
void mmi_nokia_shortcut_entryfn(void)
{
	if(!curr_vertical_hilite_index)
		shortcut_entryfn_p = touch_idle_shortcuts[curr_horizon_hilite_index].entryfn;
#if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
    else
        shortcut_entryfn_p = goto_main_menu;
#else
    else if(1 == curr_vertical_hilite_index)
		shortcut_entryfn_p = touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].entryfn;
	else if(2 == curr_vertical_hilite_index)
		shortcut_entryfn_p = touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].entryfn;
	else
		MMI_DBG_ASSERT(0);
#endif		
	shortcut_entryfn_p();
}
void ExtendIdleKey_Left_or_Right_UpHandler(void)
{
	if(curr_vertical_hilite_index)
		return;
	gui_idle_touch_screen_display_scroll_shorcut_item(curr_horizon_hilite_index);
}

void ExtendIdleKeyRightHandler(void)
{
	if(curr_vertical_hilite_index)
		return;
#if !defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
		gui_idle_touch_screen_display_shorcut_item(curr_horizon_hilite_index);
#endif
	curr_horizon_hilite_index++;
	if(MAX_IDLE_SCREEN_HOR_SHORTCUTS == curr_horizon_hilite_index)
		curr_horizon_hilite_index = 0;
//	gui_idle_touch_screen_display_pressed_shorcut_item(curr_horizon_hilite_index);	
}

void ExtendIdleKeyLeftHandler(void)
{
	if(curr_vertical_hilite_index)
		return;
#if !defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	gui_idle_touch_screen_display_shorcut_item(curr_horizon_hilite_index);
#endif
	if(0 == curr_horizon_hilite_index)
		curr_horizon_hilite_index = MAX_IDLE_SCREEN_HOR_SHORTCUTS - 1;
	else
		curr_horizon_hilite_index--;
#if defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	//gui_idle_touch_screen_display_pressed_shorcut_item(curr_horizon_hilite_index);
#else
	gui_idle_touch_screen_display_pressed_shorcut_item(curr_horizon_hilite_index | (1 << 31));
#endif
}
#endif
#if defined(__MMI_MAINLCD_220X176__)
#include"globalmenuitems.h"
#include "shortcutsdefs.h"
#define MAX_IDLE_SCREEN_SHORTCUTS   	9
#define IDLE_TOUCH_SHORTCUT_UNPRESSED   0
#define IDLE_TOUCH_SHORTCUT_PRESSED     1
#define E71_SHORTCUT_HEIGHT				27

#define E71_SHORTCUT_START_X			14
#define IDLE_SCREEN_SHORTCUT_LINE_COUNTS	3
#define MAX_IDLE_SCREEN_HOR_SHORTCUTS	6
#define E71_SHORTCUT_ITEM_STRING_START_X	2
#define E71_SHORTCUT_ITEM_STRING_START_Y	24
#define E71_SHORTCUT_ITEM_STRING_AREA_END_X	106
#define E71_SHORTCUT_ITEM_STRING_AREA_H		20	

#define E71_SHORTCUT_AUD_FILE_NAME_Y		132
typedef struct
{
    S32 x;
    S32 y;
    S32 width;
    S32 height;
    S32 state;
    FuncPtr entryfn;
} idle_touch_screen_shortcut;
static idle_touch_screen_shortcut touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS];
void gui_idle_touch_screen_display_pressed_shorcut_item(S32 index);
void gui_idle_touch_screen_display_scroll_shorcut_item(S32 index);
void ExtendIdleKeyRightHandler(void);
void ExtendIdleKeyLeftHandler(void);
void ExtendIdleKey_Left_or_Right_UpHandler(void);
void mmi_nokia_shortcut_entryfn(void);

S32 curr_horizon_hilite_index = 0;
S32 curr_vertical_hilite_index = 0;
FuncPtr shortcut_entryfn_p = NULL;

extern void HighlightClndrMenu(void);
extern void HighlightInboxHandler(void);
extern void highlight_mainmenu_multimedia(void);
extern void mmi_camera_hilight_app(void);
extern void mmi_profiles_entry_main_screen(void);
extern void HighlightAudioPlayer(void);
extern void mmi_fmrdo_highlight_menu(void);
extern void HighlightTDLMenu(void);
extern void mmi_imgview_highlight_app(void);
extern void HighlightAlmMenu(void);
extern void EntrySMSMainMenuList(void);
extern void GoToServicesMenu(void);
extern void mmi_phb_entry_main_menu(void);
extern void mmi_audply_entry_main(void);
extern void EntryMainExtra(void);
extern void EntryScrProfiles(void);
extern void ClndrPreEntryApp(void);
extern void EntryPhnsetSetDT(void);

extern void mmi_msg_entry_inbox_menu(void);
extern void mmi_camera_entry_app_screen(void);
extern void mmi_imgview_entry_app(void);
extern void MTPNP_jWap_mainMenu_entry_handler (void);

scrolling_text short_cut_audply_title;
BOOL short_cut_audply_title_created = FALSE;
void mmi_shortcut_scroll_title_handler(void)
{
    if (short_cut_audply_title_created)
    {
        gui_handle_scrolling_text(&short_cut_audply_title);
    }
}

void display_font_idle_screen(S32 jia,S32 x1,S32 x2,S32 y1,S32 y2)//xiaoyao
{
    S32 str_w, str_h,jiayuan;
    S32 xx1, yy1, xx2, yy2;

    xx1=x1;
    xx2=x2;
    yy1=y1;
    yy2=y2;
    jiayuan=jia;

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_lock_double_buffer();
#endif 
    gui_push_text_clip();
    gui_set_font(&UI_DEFAULT_FONT);//MMI_small_font  ,UI_DEFAULT_FONT  //danny 0523
    gui_measure_string((UI_string_type)GetString(jia), &str_w, &str_h);
    gui_set_text_color(gui_color(255,255, 255));
    gdi_layer_set_clip(xx1 - 2, yy1 - 2, xx2, yy2);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(UI_device_width -35, yy1);//(xx1+160, yy1);

    }
    else
    {
        gui_move_text_cursor(xx1, yy1);
    }
    gui_print_text(get_string(jiayuan));	
    gui_pop_text_clip();
#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(xx1 - 2, yy1 - 2, xx2+100, yy2);
#endif
}

void setup_touch_idle_screen_shortcuts(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 image_width[MAX_IDLE_SCREEN_SHORTCUTS], image_height[MAX_IDLE_SCREEN_SHORTCUTS], i, total_width =
    0, x,y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        gui_measure_image((PU8) GetImage((U16) (IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_MESSAGE + i)), &image_width[i], &image_height[i]);
        total_width += image_width[i];
        touch_idle_shortcuts[i].height = image_height[i];
        touch_idle_shortcuts[i].width = image_width[i];
    }

    touch_idle_shortcuts[0].entryfn = mmi_msg_entry_inbox_menu;
#ifdef JATAAYU_SUPPORT_WAP
    touch_idle_shortcuts[1].entryfn = MTPNP_jWap_mainMenu_entry_handler;
#else
    touch_idle_shortcuts[1].entryfn = mmi_imgview_entry_app;
#endif
    touch_idle_shortcuts[2].entryfn = mmi_phb_entry_main_menu;
    #if defined(TGT_GALLITE_G800)
    touch_idle_shortcuts[3].entryfn = mmi_analogtv_entry_app;
    #else
    touch_idle_shortcuts[3].entryfn = mmi_imgview_entry_app;
    #endif
    touch_idle_shortcuts[4].entryfn = mmi_camera_entry_app_screen;
    touch_idle_shortcuts[5].entryfn = EntryScrProfiles;
    touch_idle_shortcuts[6].entryfn = EntryPhnsetSetDT;
	touch_idle_shortcuts[7].entryfn = ClndrPreEntryApp;
	touch_idle_shortcuts[8].entryfn = mmi_audply_entry_main;
    //x_margin = (UI_device_width - total_width) /6;//((MAX_IDLE_SCREEN_SHORTCUTS - 1) + 2); /* Margin between shortcut Images,Leave margin on front and back also */
    y = E71_SHORTCUT_HORIZON_LINE_Y + E71_SHORTCUT_HEIGHT+2;//avoid conflict with dotted horizon line
    //x = x_margin;
    x = E71_SHORTCUT_START_X;
    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS; i++)
    {
        if(i>5)
        {
            if (r2lMMIFlag)
            {
                touch_idle_shortcuts[i].x = UI_DEVICE_WIDTH - x;
            }
            else
            {
				touch_idle_shortcuts[i].x = 3;
            }
			touch_idle_shortcuts[i].y = y;
			y += touch_idle_shortcuts[i].height+ 2;
        }
        else
        {
            touch_idle_shortcuts[i].x = x;
            touch_idle_shortcuts[i].y =E71_SHORTCUT_HORIZON_LINE_Y+1;
            x += touch_idle_shortcuts[i].width+5;
        }
    }
	date_string(&main_LCD_dt_object.date.t, (UI_string_type)shrtcut_date_string, main_LCD_dt_object.flags);
}

void display_touch_idle_screen_shortcuts(void)
{
	gui_lock_double_buffer();
	gui_idle_touch_screen_display_scroll_shorcut_item(curr_horizon_hilite_index);
#if 0	
    for (i = 0; i < MAX_IDLE_SCREEN_SHORTCUTS - 2; i++)
    {
        gui_show_image(
						touch_idle_shortcuts[i].x,
        				touch_idle_shortcuts[i].y,
        				get_image( (IMG_IDLE_MAINMENU_SHORCUT + i))
        				);	
    }
#endif	

	gdi_image_draw_id(0,E71_SHORTCUT_HORIZON_LINE_Y,IMG_IDLE_LINE_H);
	gdi_image_draw_id(0,E71_SHORTCUT_HORIZON_LINE_Y+E71_SHORTCUT_HEIGHT+1,IMG_IDLE_LINE_H);
	gdi_image_draw_id(0,touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].height+1,IMG_IDLE_LINE_H);
	gdi_image_draw_id(0,touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].height+1,IMG_IDLE_LINE_H);
	if(mmi_audply_is_playing())
		gdi_image_draw_id(0,touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].height+1,IMG_IDLE_LINE_H);
		
	if(1 == curr_vertical_hilite_index)
	{
		gdi_draw_solid_rect(1,
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].y+1,
							UI_device_width-3,
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].height-1,
							GDI_COLOR_BLACK);
		gdi_draw_rect(0,touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].y,
			UI_device_width-2,
			touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].height,
			GDI_COLOR_RED);
		gui_show_image(
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].x,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].y,
						get_image(IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_ALARM)
						);
		//gui_set_text_color(gui_color(255,255,255));		
	}
	else
	{
		gdi_draw_solid_rect(0,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].y,
						UI_device_width,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].height,
						GDI_COLOR_TRANSPARENT);
		gui_show_image(
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].x,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].y,
						get_image(IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_ALARM)
						);
		//gui_set_text_color(gui_color(0,0,0));
	}
	gui_set_text_color(gui_color(255,255,255));
	gui_move_text_cursor(23,90);//E71_DATE_DISPLAY_X,E71_DATE_DISPLAY_Y+1 
	gui_set_font(&MMI_medium_font);
	//gui_print_text(shrtcut_date_string);
	gui_print_bordered_text(shrtcut_date_string);
	if(2 == curr_vertical_hilite_index)
	{
		gdi_draw_solid_rect(
							1,
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y+1,
							UI_device_width-3,
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].height-1,
							GDI_COLOR_BLACK);
		gdi_draw_rect(0,touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y,
			UI_device_width-2,
			touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].height,
			GDI_COLOR_RED);
		gui_show_image(
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].x,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y,
						get_image(IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_CALENDAR)
						);
		//gui_set_text_color(gui_color(255,255,255));
		//MMI_title_string = (UI_string_type)get_string(STR_ID_FMRDO_TITLE);	
	}	
	else
	{
		gdi_draw_solid_rect(0,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y,
						UI_device_width,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].height,
						GDI_COLOR_TRANSPARENT);
		gui_show_image(
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].x,
						touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].y,
						get_image(IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_CALENDAR)
						);
		//gui_set_text_color(gui_color(0,0,0));
	}
	
	gui_set_text_color(gui_color(255,255,255));
	gui_set_font(&MMI_medium_font);
	gui_move_text_cursor(23,110);//E71_DATE_DISPLAY_X,E71_DATE_DISPLAY_Y+1 
	gui_print_bordered_text((UI_string_type)get_string(ORGANIZER_MENU_CALENDER_STRINGID));
	if(mmi_audply_is_playing() ||  mmi_fmrdo_is_power_on())
	{
		if(3 == curr_vertical_hilite_index)
		{
			gdi_draw_solid_rect(
								1,
								touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y+1,
								UI_device_width-3,
								touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].height-1,
								GDI_COLOR_BLACK);
			gdi_draw_rect(0,touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y,
				UI_device_width-2,
				touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].height,
				GDI_COLOR_RED);
			gui_show_image(
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].x,
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y,
							get_image(IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_MP3_PLAYING)
							);
		}
		else
		{
			gdi_draw_solid_rect(0,
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y,
							UI_device_width,
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].height,
							GDI_COLOR_TRANSPARENT);
			gui_show_image(
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].x,
							touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y,
							get_image(IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_MP3_PLAYING)
							);
		}
		
		gui_set_text_color(gui_color(255,255,255));
		gui_set_font(&MMI_medium_font);
		
		if(mmi_audply_is_playing() && g_audply.title)
		{
			S32 w = gui_get_string_width((UI_string_type)g_audply.title);
			if(w>(UI_device_width - 3 - 23))
				gui_print_truncated_borderd_text(23,E71_SHORTCUT_AUD_FILE_NAME_Y,(UI_device_width - 3 - 23),g_audply.title);
			else
			{
				gui_move_text_cursor(23,E71_SHORTCUT_AUD_FILE_NAME_Y);
				gui_print_bordered_text(g_audply.title);
			}
		}
		else if (mmi_fmrdo_is_power_on())
		{
		  UI_string_type extra_text=NULL; 
	        extra_text = (UI_string_type) mmi_fmrdo_get_channel_name_or_freq();
		 if (extra_text)
		 {
	  		 S32 w = gui_get_string_width(extra_text);
	  		 if(w>(UI_device_width - 3 - 23))
	  			gui_print_truncated_borderd_text(23,E71_SHORTCUT_AUD_FILE_NAME_Y,(UI_device_width - 3 - 23),extra_text);
	  		 else
	  		 {
	  			gui_move_text_cursor(23,E71_SHORTCUT_AUD_FILE_NAME_Y);
	  			gui_print_bordered_text(extra_text);
	  		 }
		}
	     }
	}	
	gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, E71_SHORTCUT_HORIZON_LINE_Y,UI_device_width,touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].y + touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].height+1);
	
}

void YITHEMEZUFU2(void)
{
    SetCurrentProfileAsActivatedProfile();
#ifndef __MMI_USE_LSK_AS_SEND2__
	SetKeyHandler(mmi_nokia_shortcut_entryfn,KEY_ENTER,KEY_EVENT_UP);
#else
	SetKeyHandler(EntryMainMenuFromIdleScreen,KEY_ENTER,KEY_EVENT_UP);
#endif
    //SetKeyHandler(YITHEMEZUFU3, KEY_ENTER, KEY_EVENT_UP);
}

void mmi_shortcut_left_move_event_down()
{
	ExtendIdleKeyLeftHandler();
}
void mmi_shortcut_right_move_event_down()
{
	ExtendIdleKeyRightHandler();
}
void mmi_shortcut_left_move_event_up()
{
	ExtendIdleKey_Left_or_Right_UpHandler();
}
void mmi_shortcut_right_move_event_up()
{
	ExtendIdleKey_Left_or_Right_UpHandler();
}
void gui_idle_touch_screen_display_shorcut_item(S32 index)
{
	S32 x1, y1, x2, y2;

	if(curr_vertical_hilite_index)
		return;
	MMI_DBG_ASSERT(index >= 0 && index < MAX_IDLE_SCREEN_HOR_SHORTCUTS);
	
	touch_idle_shortcuts[index].state = IDLE_TOUCH_SHORTCUT_UNPRESSED;
#if defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	x1 = touch_idle_shortcuts[index].x;	
	y1 = touch_idle_shortcuts[index].y;
	x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;	
	y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
#else
	x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x;	
	y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y;
	x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x + touch_idle_shortcuts[index].width - 1;	
	y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y + touch_idle_shortcuts[index].height - 1;
#endif
	gui_lock_double_buffer();
	gui_push_clip();
	gui_set_clip(x1,y1,x2,y2);
	gui_draw_gradient_rounded_rectangle(x1, y1, x2, y2,
			gui_color(192,192,192),
			gui_color(255,255,255),
			0);
	//gui_fill_rectangle(x1, y1, x2, y2, gui_color(255,0,0)); 
	gui_show_image(x1, y1,(PU8) GetImage((U16) (IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_MESSAGE + index)));
	gui_pop_clip();	
	gui_unlock_double_buffer();
	gui_BLT_double_buffer(0, 18, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height-1);
}

void gui_idle_touch_screen_display_pressed_shorcut_item(S32 index)
{
	S32 x1, y1, x2, y2;

	MMI_DBG_ASSERT((index >= 0) && (index < MAX_IDLE_SCREEN_HOR_SHORTCUTS));
#if defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	x1 = touch_idle_shortcuts[index].x;
	y1 = touch_idle_shortcuts[index].y;
	x2 = touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width - 1;
	y2 = touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height - 1;
#else
	if (index & (1 <<30))
	{
		x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x;
		y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y;
		x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x + touch_idle_shortcuts[index].width - 1;
		y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y + touch_idle_shortcuts[index].height - 1;	
	}
	else if (index & (1 << 31))
	{
		x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2-1].x;
		y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2-1].y;
		x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2-1].x + touch_idle_shortcuts[index].width - 1;
		y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2-1].y + touch_idle_shortcuts[index].height - 1;	
	}
	else
	{
		x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2+1].x;
		y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2+1].y;
		x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2+1].x + touch_idle_shortcuts[index].width - 1;
		y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2+1].y + touch_idle_shortcuts[index].height - 1;		
	}
#endif

	gui_lock_double_buffer();
	gui_push_clip();
	gui_set_clip(x1, y1, x2, y2);
	//gui_fill_rectangle(x1, y1, x2, y2,gui_color(255,0,0));

#if defined(__MMI_MAINLCD_220X176__)
#else

	gui_draw_gradient_rounded_rectangle(x1, y1, x2, y2,
		gui_color(192,192,192),gui_color(192,192,192),0);
#endif
	gui_show_image(x1, y1, 
		(PU8) GetImage((U16) (IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_MESSAGE + index)));
	gui_pop_clip();	
	gui_unlock_double_buffer();
	gui_BLT_double_buffer(x1, y1,x2, y2);
	
}

void mmi_shortcut_up_move(void)
{
	curr_vertical_hilite_index--;
	if(mmi_audply_is_playing())
	{
		if(-1 == curr_vertical_hilite_index)
			curr_vertical_hilite_index = IDLE_SCREEN_SHORTCUT_LINE_COUNTS;
	}
	else
	{
		if(-1 == curr_vertical_hilite_index)
			curr_vertical_hilite_index = IDLE_SCREEN_SHORTCUT_LINE_COUNTS - 1;
	}
	display_touch_idle_screen_shortcuts();
}

void mmi_shortcut_down_move(void)
{
	curr_vertical_hilite_index++;
	if(mmi_audply_is_playing())
	{
		if((IDLE_SCREEN_SHORTCUT_LINE_COUNTS+1) == curr_vertical_hilite_index)
			curr_vertical_hilite_index = 0;
	}
	else
	{
		if(IDLE_SCREEN_SHORTCUT_LINE_COUNTS == curr_vertical_hilite_index)
			curr_vertical_hilite_index = 0;
	}
	display_touch_idle_screen_shortcuts();
}

void gui_idle_touch_screen_display_scroll_shorcut_item(S32 index)
{
	S32 i; 
#if !defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)	
	S32 x1, y1, x2, y2;
#endif
	gui_lock_double_buffer();
	gui_push_clip();
	gui_set_clip(0, touch_idle_shortcuts[0].y-1, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height);	
	gdi_draw_solid_rect(0, touch_idle_shortcuts[0].y, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height,GDI_COLOR_TRANSPARENT);
#if defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	for (i = 0; i < MAX_IDLE_SCREEN_HOR_SHORTCUTS; i++)
	{
		if((index == i) && (0 == curr_vertical_hilite_index))
			gui_show_image(touch_idle_shortcuts[i].x,
							touch_idle_shortcuts[i].y,
							get_image((UI_image_ID_type)(IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_MESSAGE_SELECTED + i)));
			/*gui_draw_gradient_rounded_rectangle(
												touch_idle_shortcuts[index].x,
												touch_idle_shortcuts[index].y,
												touch_idle_shortcuts[index].x + touch_idle_shortcuts[index].width,
												touch_idle_shortcuts[index].y + touch_idle_shortcuts[index].height,
												gui_color(255,0,0),
												gui_color(0,0,0),
												1);*/
		else
			gui_show_image(touch_idle_shortcuts[i].x,
							touch_idle_shortcuts[i].y,
							get_image((UI_image_ID_type) (IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_MESSAGE + i)));
			
	}
#else
	for (i = 0; i < MAX_IDLE_SCREEN_HOR_SHORTCUTS; i++)
	{
		S32 j = i + index - (MAX_IDLE_SCREEN_HOR_SHORTCUTS/2);
		
		if(j < 0) j += MAX_IDLE_SCREEN_HOR_SHORTCUTS;
		if(j >= MAX_IDLE_SCREEN_HOR_SHORTCUTS) j -= MAX_IDLE_SCREEN_HOR_SHORTCUTS;
		
		gui_show_image(touch_idle_shortcuts[i].x,
			touch_idle_shortcuts[i].y,
			get_image((UI_image_ID_type) (IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_MESSAGE + j)));		
	}
	if(!curr_vertical_hilite_index)
	{
		x1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x;
		y1 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y;
		x2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].x + touch_idle_shortcuts[index].width - 1;
		y2 = touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS/2].y + touch_idle_shortcuts[index].height - 1;
		gui_draw_gradient_rounded_rectangle(x1, y1, x2, y2,
			gui_color(255,255,255),
			gui_color(192,192,192),
			0);
		gui_show_image(x1, y1, 
			(PU8) GetImage((U16) (IMG_IDLE_SHORTCUT_NOKIA_E_SERIES_MESSAGE + index)));
	}
#endif	
#if 0//for nokia e71 style,without title string in idle screen.
	MMI_title_string = (UI_string_type)get_string(STR_GLOBAL_OK + index);
	show_item_title(T800_SHORTCUT_ITEM_STRING_START_X,T800_SHORTCUT_ITEM_STRING_START_Y);
#endif
#if 0
	gui_show_image(touch_idle_shortcuts[0].x-9,touch_idle_shortcuts[0].y+8,
		(PU8)GetImage(IMG_LEFT_ARROW));
	gui_show_image(166,touch_idle_shortcuts[MAX_IDLE_SCREEN_HOR_SHORTCUTS-1].y+8,
		(PU8)GetImage(IMG_RIGHT_ARROW));
#endif
	gui_pop_clip();
	gui_unlock_double_buffer();
	//gdi_draw_solid_rect(0, touch_idle_shortcuts[0].y, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height-1,GDI_COLOR_TRANSPARENT);
	gui_BLT_double_buffer(0, touch_idle_shortcuts[0].y, UI_device_width-1, touch_idle_shortcuts[0].y+touch_idle_shortcuts[0].height);
}


void mmi_nokia_shortcut_entryfn(void)
{
	if(!curr_vertical_hilite_index)
		shortcut_entryfn_p = touch_idle_shortcuts[curr_horizon_hilite_index].entryfn;
	else if(1 == curr_vertical_hilite_index)
		shortcut_entryfn_p = touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 3].entryfn;
	else if(2 == curr_vertical_hilite_index)
		shortcut_entryfn_p = touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 2].entryfn;
	else if(3 == curr_vertical_hilite_index)
		shortcut_entryfn_p = touch_idle_shortcuts[MAX_IDLE_SCREEN_SHORTCUTS - 1].entryfn;
	else
		MMI_DBG_ASSERT(0);
		
	shortcut_entryfn_p();
}
void ExtendIdleKey_Left_or_Right_UpHandler(void)
{
	if(curr_vertical_hilite_index)
		return;
	gui_idle_touch_screen_display_scroll_shorcut_item(curr_horizon_hilite_index);
}

void ExtendIdleKeyRightHandler(void)
{
	if(curr_vertical_hilite_index)
		return;
#if !defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
		gui_idle_touch_screen_display_shorcut_item(curr_horizon_hilite_index);
#endif
	curr_horizon_hilite_index++;
	if(MAX_IDLE_SCREEN_HOR_SHORTCUTS == curr_horizon_hilite_index)
		curr_horizon_hilite_index = 0;
	gui_idle_touch_screen_display_pressed_shorcut_item(curr_horizon_hilite_index);	
}

void ExtendIdleKeyLeftHandler(void)
{
	if(curr_vertical_hilite_index)
		return;
#if !defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	gui_idle_touch_screen_display_shorcut_item(curr_horizon_hilite_index);
#endif
	if(0 == curr_horizon_hilite_index)
		curr_horizon_hilite_index = MAX_IDLE_SCREEN_HOR_SHORTCUTS - 1;
	else
		curr_horizon_hilite_index--;
#if defined(MMI_SHORTCUT_HORIZON_SWITCH_FEATURE)
	gui_idle_touch_screen_display_pressed_shorcut_item(curr_horizon_hilite_index);
#else
	gui_idle_touch_screen_display_pressed_shorcut_item(curr_horizon_hilite_index | (1 << 31));
#endif
}
#if defined(__MMI_TOUCH_SCREEN__)
#if 0
//this function is used for checking parameter value in break point.
U8 mmi_pen_check_bound(S32 x, S32 y, S32 x1,S32 y1,S32 width,S32 height)
{
	return ((x) >= (x1) && (y) >= (y1) && (x) < ((x1) + (width)) && (y) < ((y1) + (height))); 
}
#endif
void coolsand_UI_shortcut_translate_pen_event(S16 x,S16 y,mmi_pen_event_type_enum pen_event)
{
	U8 selected_shortcut_index;
	S16 x1 = touch_idle_shortcuts[0].x;
	S16 x2 = touch_idle_shortcuts[5].x;
	S16 y1 = touch_idle_shortcuts[0].y;
	
	if(PEN_CHECK_BOUND(x,y,x1,y1,x2-x1+touch_idle_shortcuts[5].width,touch_idle_shortcuts[5].height))
	{
		curr_vertical_hilite_index = 0;//shortcut selected is [0,5]
		curr_horizon_hilite_index = ((x-x1)/touch_idle_shortcuts[0].width);		
	}
	else if(PEN_CHECK_BOUND(x,y,x1,touch_idle_shortcuts[6].y,UI_device_width-(2*3),2*touch_idle_shortcuts[6].height))
		curr_vertical_hilite_index = 1+((y - touch_idle_shortcuts[6].y)/touch_idle_shortcuts[6].height);
	else if(PEN_CHECK_BOUND(x,y,x1,touch_idle_shortcuts[8].y,UI_device_width-(2*3),touch_idle_shortcuts[8].height))
	{
		if((mmi_audply_is_playing()))
			curr_vertical_hilite_index = 1+((y - touch_idle_shortcuts[6].y)/touch_idle_shortcuts[6].height);
		else
			return;
	}
	else
		return;
	
	switch(pen_event)
	{
		case MMI_PEN_EVENT_DOWN:
			display_touch_idle_screen_shortcuts();	
			break;
		case MMI_PEN_EVENT_UP:
		{
			if(0 == curr_vertical_hilite_index)
				selected_shortcut_index = curr_horizon_hilite_index;
			else
				selected_shortcut_index = 5+curr_vertical_hilite_index;

			if(touch_idle_shortcuts[selected_shortcut_index].entryfn)
				touch_idle_shortcuts[selected_shortcut_index].entryfn();
		}
			break;

	case MMI_PEN_EVENT_LONG_TAP:
	case MMI_PEN_EVENT_REPEAT:
	case MMI_PEN_EVENT_MOVE:
	case MMI_PEN_EVENT_ABORT:
	case MMI_PEN_STROKE_DOWN:
	case MMI_PEN_STROKE_UP:
	case MMI_PEN_STROKE_MOVE:
	case MMI_PEN_STROKE_LONGTAP:
	case MMI_PEN_EVENT_TYPE_MAX:
		break;
			
	}
	
}

void mmi_touch_shortcut_pen_down_hdlr(mmi_pen_point_struct point)
{
	MMI_BOOL ret = MMI_FALSE;
	mmi_gui_button_pen_enum button_event;

	ret = translate_softkey_pen_event(MMI_LEFT_SOFTKEY,MMI_PEN_EVENT_DOWN,point.x,point.y,&button_event);
	if (ret)
	{
		if (button_event == GUI_BUTTON_PEN_DOWN)
		{
			execute_softkey_function(KEY_EVENT_DOWN, MMI_LEFT_SOFTKEY);
		}
		return;
	}

	ret = translate_softkey_pen_event(MMI_RIGHT_SOFTKEY, MMI_PEN_EVENT_DOWN,point.x, point.y, &button_event);
	if(ret)
	{
		if (button_event == GUI_BUTTON_PEN_DOWN)
		{
			execute_softkey_function(KEY_EVENT_DOWN,MMI_RIGHT_SOFTKEY);
		}
		return;
	}
	
	ret = gui_icontext_button_translate_pen_event(&MMI_softkeys[MMI_CENTER_SOFTKEY],MMI_PEN_EVENT_DOWN,
													point.x,
													point.y,
													&button_event);
	if (ret)
	{
		if (button_event == GUI_BUTTON_PEN_DOWN)
		{
			wgui_pen_execute_CSK_handler(KEY_EVENT_DOWN);
		}
		return;
	}
	
	coolsand_UI_shortcut_translate_pen_event(point.x,point.y,MMI_PEN_EVENT_DOWN);
	


}
void mmi_touch_shortcut_pen_move_hdlr(mmi_pen_point_struct point)
{	
	coolsand_UI_shortcut_translate_pen_event(point.x,point.y,MMI_PEN_EVENT_DOWN);
}
void mmi_touch_shortcut_pen_up_hdlr(mmi_pen_point_struct point)
{
	MMI_BOOL ret = MMI_FALSE;
	mmi_gui_button_pen_enum button_event;

	ret = translate_softkey_pen_event(MMI_LEFT_SOFTKEY,MMI_PEN_EVENT_UP,point.x,point.y,&button_event);
	if (ret)
	{
		if (button_event == GUI_BUTTON_PEN_UP_INSIDE)
		{
			execute_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY);
		}
	}
	ret = translate_softkey_pen_event(MMI_RIGHT_SOFTKEY, MMI_PEN_EVENT_UP,point.x, point.y, &button_event);
	if(ret)
	{
		if (button_event == GUI_BUTTON_PEN_UP_INSIDE)
		{
			execute_softkey_function(KEY_EVENT_UP,MMI_RIGHT_SOFTKEY);
		}
	}
	coolsand_UI_shortcut_translate_pen_event(point.x,point.y,MMI_PEN_EVENT_UP);
}

#endif
#endif




#if defined(__COOLSAND_SUBMENU_LEVEL2_POPMENU__)
#define CS_POPMENU_START_X		0
#define CS_POPMENU_WIDTH		UI_device_width

s16 coordinate_set_popmenu[8];
void catPopMenu_hignlight_handler(S32 index)
{
	gdi_layer_lock_frame_buffer();
	MMI_list_highlight_handler(index);
	gdi_layer_unlock_frame_buffer();
}
void RedrawCategoryPopMenuScreen(void)
{
	gdi_layer_lock_frame_buffer();
	show_fixed_list();
	show_softkey(MMI_LEFT_SOFTKEY);
	show_softkey(MMI_RIGHT_SOFTKEY);
	gdi_layer_unlock_frame_buffer();
	gui_BLT_double_buffer(0,0,UI_device_width - 1,UI_device_height - 1);
}

S32 catPopMenu_get_category_history_size(void)
{
	return (sizeof(list_menu_category_history));
}
void ExitCategoryPopMenuScreen(void)
{
	ClearHighlightHandler();
	reset_softkeys();
	reset_menu_shortcut_handler();
	reset_fixed_list();
}

void ShowCategoryPopMenuScreen(
								U16 left_softkey,
								U16 left_softkey_icon,
								U16 right_softkey,
								U16 right_softkey_icon,
								S32 number_of_items,	
								U16 *list_of_items,
								U16 *list_of_icons,
								S32 highlighted_item,
								U8 *history_buffer)		
{
	S32 i,popmenu_start_y= 66;
	dm_data_struct dm_data;
	S32 popmenu_h;

	
	popmenu_start_y -= (get_menu_item_height() - (popmenu_start_y % (get_menu_item_height())));//modify popmenu_start_y value to make sure (phomenu_h - n*menu_height == 0)
	coordinate_set_popmenu[0] = DM_FULL_SCREEN_COORDINATE_FLAG;
	coordinate_set_popmenu[1] = CS_POPMENU_START_X;
	coordinate_set_popmenu[3] = CS_POPMENU_WIDTH;
	coordinate_set_popmenu[5] = DM_NO_FLAGS;
	coordinate_set_popmenu[6] = DM_DEFAULT_BUTTON_BAR_FLAG;
	coordinate_set_popmenu[7] = MMI_SOFTKEY_WIDTH;
	if((number_of_items * get_menu_item_height()) < (UI_device_height - MMI_button_bar_height - popmenu_start_y))
		popmenu_h = number_of_items * get_menu_item_height();
	else
		popmenu_h = (UI_device_height - MMI_button_bar_height - popmenu_start_y);
	coordinate_set_popmenu[4] = popmenu_h;
	coordinate_set_popmenu[2] = UI_device_height - MMI_BUTTON_BAR_HEIGHT - popmenu_h;
	
	gui_lock_double_buffer();
	gui_greyscale_rectangle(0, 0, UI_device_width - 1, UI_device_height - 2, MMI_BG_GREYSCALE_VALUE, MMI_BG_GREYSCALE_BLACK_VALUE);
	
	change_left_softkey(left_softkey,left_softkey_icon);
	change_right_softkey(right_softkey,right_softkey_icon);
	move_fixed_list(CS_POPMENU_START_X,UI_device_height - MMI_BUTTON_BAR_HEIGHT - popmenu_h);
	resize_fixed_list(CS_POPMENU_WIDTH,popmenu_h);
	MMI_current_menu_type = LIST_MENU;
	disable_menu_shortcut_box_display = 1;
	create_fixed_icontext_menuitems();
	associate_fixed_icontext_list();
	for(i = 0;i<number_of_items;i++)
		add_fixed_icontext_item(get_string(list_of_items[i]),(PU8)GetImage(list_of_icons[i]));

	register_fixed_list_shortcut_handler();
	register_fixed_list_keys();
	register_fixed_list_highlight_handler(catPopMenu_hignlight_handler);
		
	if(set_list_menu_category_history(MMI_CATEGORY_POPUPMENU_ID,history_buffer))
		fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
	else
		fixed_list_goto_item_no_redraw(highlighted_item);
	
	//show_fixed_list();
	
	gui_unlock_double_buffer(); 
	gui_BLT_double_buffer(0,0,UI_device_width - 1,UI_device_height - 1);
	ExitCategoryFunction = ExitCategoryPopMenuScreen;
	RedrawCategoryFunction = RedrawCategoryPopMenuScreen;
	
	GetCategoryHistorySize = catPopMenu_get_category_history_size;
	dm_data.S32ScrId = (S32)GetActiveScreenId();
	dm_data.S32CatId = MMI_CATEGORY_POPUPMENU_ID;
	dm_data.S32flags = DM_NO_FLAGS;
	dm_setup_data(&dm_data);
	dm_redraw_category_screen();

}
#endif
