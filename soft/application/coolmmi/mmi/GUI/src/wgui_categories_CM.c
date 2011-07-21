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
 *  wgui_categories_CM.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Call Management related categories.
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
#undef __NEWSIMULATOR
#include "globaldefs.h"
#include "gui_data_types.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "eventsdef.h"
#include "gui_themes.h"
#include "wgui.h"
#include "custthemesres.h"
#include "debuginitdef.h"
#include "wgui_tab_bars.h"
#include "wgui_inputs.h"
#include "wgui_categories.h"
#include "wgui_softkeys.h"
#include "wgui_status_icons.h"
#include "wgui_categories_defs.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_idlescreen.h"
#include "wgui_categories_multimedia.h"
#include "wgui_categories_cm.h"
#include "callmanagementstruct.h"
#include "callsetup.h"
#include "phonebookdef.h"
#include "gdi_include.h"
#include "lcd_if.h"
#include "wgui_draw_manager.h"

#ifdef __MMI_TOUCH_DIAL_SCREEN__
#include "wgui_touch_screen.h"
#include "gui_setting.h"
#include "idleappprot.h"
#if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
#include "phonebookgprot.h"
#include "idleappdef.h"
#include "simdetectiongprot.h"
#endif /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */ 
#include "callstructuremanagementprot.h"
#include "pscallhelpergprot.h"
#endif /* __MMI_TOUCH_DIAL_SCREEN__ */ 

#ifdef __GDI_MEMORY_PROFILE_2__
#include "gdi_datatype.h"
#include "gdi_layer.h"
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

#ifdef __MMI_VIDEO_PLAYER__
#include "profilegprots.h"      /* to set volume */
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "mdi_video.h"
#endif /* __MMI_VIDEO_PLAYER__ */ 

#ifdef __MMI_SWFLASH__
#include "mdi_swflash.h"
#endif 
#include "dual_sim.h"
#include "gpioinc.h"   
#include "keybrd.h"   
#if defined(__MMI_MAINLCD_220X176__)
#include "mainmenudef.h"
#endif
#define CATEGORY17_TEXT_SCROLL_WAIT (1000)
#define CM_TEXT_SCROLL_GAP    (32)
#define CM_TEXT_SCROLL_SIZE   (3)
#define CM_TEXT_SCROLL_TIME   (100)
#define CM_TEXT_SCROLL_WAIT   (1000)
#define CM_TEXT_SCROLL_X      (0)
#define CM_TEXT_SCROLL_Y      (20)

/*-------------------------------
External Global variables
-------------------------------*/
extern BOOL r2lMMIFlag;
#ifdef __MMI_IDLE_FULL_SCREEN__
extern U16 on_idle_screen;
#endif
extern U16 gCurrLangIndex;  /* Need to Revise */

extern bitmap *current_LCD_device_bitmap;   /* Need to Revise */
extern bitmap main_LCD_device_bitmap;       /* Need to Revise */
extern bitmap sub_LCD_device_bitmap;        /* defined in wingui.c */

extern S32 wgui_image_clip_x1;
extern S32 wgui_image_clip_y1;
extern S32 wgui_image_clip_x2;
extern S32 wgui_image_clip_y2;
extern S32 _MMI_animated_icon_x;
extern S32 _MMI_animated_icon_y;

extern U8 gMMI_UI_U8_flag_1;
extern U8 gMMI_UI_U8_flag_2;
extern S16 status_icon;

extern PU8 _MMI_animated_icon;
extern U8 *_MMI_animated_icon_name;
extern S32 _MMI_animated_icon_x;
extern S32 _MMI_animated_icon_y;
extern UI_animated_image_handle _MMI_animated_icon_handle;
extern icontext_button MMI_softkeys[];

#ifdef __MMI_VIDEO_PLAYER__
extern mdi_video_info_struct wgui_video_info;
#endif 

#ifdef __MMI_SWFLASH__
extern mdi_swflash_info_struct wgui_swflash_info;
#endif 

#ifdef __GDI_MEMORY_PROFILE_2__
extern gdi_handle wgui_layer_1;
extern gdi_handle wgui_base_layer;
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

extern S16 MMI_current_input_ext_type;

/*-------------------------------
External Global Functions
-------------------------------*/
extern MMI_BOOL GetCallTimeDisplay(void);
extern void (*_MMI_hide_fixed_list_menu) (void);
extern void (*wgui_dialer_inputbox_RSK_function) (void);    /* Store the clipping value of current image */
extern void dialer_inputbox_handle_multitap_complete(S32 type);
extern void close_dialer_inputbox(void);
extern void leave_idle_screen(void);
extern void wgui_setup_dialer_inputbox(
                S32 x,
                S32 y,
                S32 width,
                S32 height,
                U8 *buffer,
                S32 buffer_size,
                U16 category_screen_ID,
                UI_string_type RSK_label,
                PU8 RSK_icon,
                U8 *history_buffer,
                U32 flags);
extern void draw_wallpaper(void);
extern void idle_screen_hide_status_icons_bar0(void);
extern void idle_screen_hide_status_icons_bar1(void);
extern pBOOL GetShowAOC(void);
extern void wgui_show_transparent_animation(void);
extern void wgui_set_animation_image_y(S32 x, S32 y, PU8 img);
extern void dialer_inputbox_handle_multitap_star_key_up(void);
extern void get_dialer_inputbox_category_history(U16 history_ID, U8 *history_buffer);
extern void wgui_fill_rectangle_clip(S32 x1, S32 y1, S32 x2, S32 y2, color_t c);
extern U8 mmi_kbn_key_tone_hdlr(KEYBRD_MESSAGE *eventKey);
extern void redraw_status_icon_bar(U8 j);
#ifdef __MMI_TOUCH_DIAL_SCREEN__
void setup_dialing_keypad(dialing_keypad_struct *dialing_keypad);
#endif 

#ifndef __MMI_MAINLCD_128X128__
extern void set_main_lcd_duration_position(U16 x, U16 y);
extern void dt_update_duration(void);
#endif /* __MMI_MAINLCD_128X128__ */ 

extern void redraw_call_cost_UI(void);
extern void reset_call_cost_UI(void);

extern void cat5_virtual_keypad_callback(void);
extern void cat33_hide_lsk(void);
extern void cat33_hide_rsk(void);
extern void wgui_clear_center_softkey(void);

#ifdef __MMI_T9__
#include "t9main.h"
extern void InuptMethodEnterCategory5(void);
#elif defined __MMI_ZI__
extern void ZiInuptMethodEnterCategory5(void);
#elif defined __MMI_KA__
extern void KonkaInuptMethodEnterCategory5(void);
#elif defined __MMI_ITAP__
extern void ItapInuptMethodEnterCategory5(void);
#endif
#ifdef __MMI_SWFLASH__
extern mdi_result SetupCat34Media(U16 bg_media_id, S8 *bg_media_filename);
extern void DrawCateSWFlashCategoryControlArea(dm_coordinates *coordinates);
#endif
#ifdef __MMI_GB__
extern void GbInuptMethodEnterCategory5(void);
#endif
/*-------------------------------
Global variables
-------------------------------*/
scrolling_text CM_scrolling_text;

S32 CM_text_offset_x = 0;
S32 CM_text_width = 0;
S32 CM_text_height = 0;
S32 CM_x = 0;
S32 CM_y = 0;
S32 CM_n_frame = 0;
S32 CM_total_frames = 0;
U8 CM_Screen_show_date_time_bar = 0;
BOOL CM_need_change_clip = FALSE;
S8 CM_string_buf[(MAX_IP_NUMBER_LEN + MAX_DIGIT) * 2];

BOOL cat_momt_is_aud_enable = FALSE;
BOOL cat_momt_is_visual_update = FALSE;
BOOL cat_momt_is_video_open = FALSE;
BOOL cat_momt_is_video_play = FALSE;
BOOL cat_momt_is_swflash_open = FALSE;
BOOL cat_momt_is_swflash_play = FALSE;
BOOL cat_momt_is_play_aud_when_start = FALSE;
BOOL cat_momt_is_aud_muted = FALSE;
BOOL cat_momt_is_show_default = FALSE;

S16 cat_momt_repeat_count = 0;
U16 cat_momt_resource_id = 0;
U16 cat_momt_default_image_id = 0;
PS8 cat_momt_resource_filename = NULL;
U32 cat_momt_video_play_time = 0;
U64 cat_momt_swflash_play_time = 0;
wgui_cate_momt_res_type_enum cat_momt_resource_type = 0;

GDI_HANDLE MOMT_animation_handle = GDI_ERROR_HANDLE;
MMI_BOOL isCallDisconnecting = MMI_FALSE;

/***************************************************************************** 
* Local Function
*****************************************************************************/
static void ShowMOMTCallScreen(
                U16 title_id,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U16 NotificationStringId,
                PU8 NameOrNumber,
                PU8 IP_Number,
                U16 default_image_id,
                U16 resource_id,
                PS8 resource_filename,
                wgui_cate_momt_res_type_enum resource_type,
                U16 repeat_count,              /* video only. 0 means infinite loop */ 
                BOOL is_visaul_update,         /* video only. update to LCM */ 
                BOOL is_video_aud,             /* video only. eanble video's audio */ 
                BOOL is_play_aud_when_start,   /* video only. play video's audio when start */ 
                PU8 history_buffer);

static void RedrawMOMTCallScreen(void);
static void ExitMOMTCallScreen(void);
static void SetMOMTCallImageClip(S32 image_width, S32 image_height);

#ifdef __MMI_VIDEO_PLAYER__
#if 0
static void MOMTPlayVideoResultCallback(MDI_RESULT result);
#endif
#endif 

#ifdef __MMI_SWFLASH__
static void MOMTPlaySWFlashResultCallback(MDI_RESULT result);
#endif 

static void DrawMOMTCallScreenControllArea(dm_coordinates *coordinate);

#ifdef __FT_IDLE_SMART_DIAL__ 
extern stFontAttribute wgui_dialer_box_f1;

UI_filled_area cat16_inputbox_BG_filler=
{
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
//(UI_image_type)__MAINLCD__THEMES__THEME1__SUBBKG_PBM,
NULL,
{8,35,93,100},
{8,35,93,100},
{8,35,93,100},
{8,35,93,100},
0
};

UI_dialer_input_box_theme  cat16_dialer_input_box_theme =

{

       &cat16_inputbox_BG_filler,
	&cat16_inputbox_BG_filler,
	&cat16_inputbox_BG_filler,

       {	0,0,0,100		},
	{	128,128,128,100	},
	{	0,0,0,100		},
	
	{	255,255,255,100	},
	{	51,88,171,100	},
	{	255,0,0,100		},
	&wgui_dialer_box_f1,
	1,
	0,
	'*'
};

#endif

/*****************************************************************************
 * FUNCTION
 *  CM_scrolling_text_timer_callback
 * DESCRIPTION
 *  callback function when scrolling text timer expires
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void CM_scrolling_text_timer_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_handle_scrolling_text(&CM_scrolling_text);
}   /* end of CM_scrolling_text_timer_callback */


/*****************************************************************************
 * FUNCTION
 *  handle_CM_text_scroll
 * DESCRIPTION
 *  handle the text scrolling of CM screens
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_CM_text_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    GDI_HANDLE previous_act_lcd = GDI_NULL_HANDLE;
#ifndef __MMI_MAINLCD_220X176__
    color_t bc = gui_color(0, 0, 0);
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_lcd_get_active(&previous_act_lcd);

    UI_set_main_LCD_graphics_context();
    gdi_layer_push_clip();
#ifndef __MMI_MAINLCD_220X176__
    gui_set_text_color(bc);
#else
    gui_set_text_color(current_UI_theme->multi_line_input_box_theme->normal_text_color);
#endif
    gui_set_font(current_UI_theme->multi_line_input_box_theme->text_font);

    if (r2lMMIFlag)
    {
        CM_text_offset_x += CM_TEXT_SCROLL_SIZE;
        if ((CM_TEXT_SCROLL_X + CM_text_offset_x) >= (CM_text_width + CM_TEXT_SCROLL_GAP))
        {
            CM_text_offset_x = CM_TEXT_SCROLL_X;
        }
    }
    else
    {
        CM_text_offset_x -= CM_TEXT_SCROLL_SIZE;
        if ((CM_text_offset_x + CM_text_width + CM_TEXT_SCROLL_GAP) < CM_TEXT_SCROLL_X)
        {
            CM_text_offset_x = CM_TEXT_SCROLL_X;
        }
    }

    gdi_layer_set_clip(0, CM_y, UI_device_width - 1, CM_y + CM_text_height - 1);
    gui_draw_filled_area(
        0,
        0,
        UI_device_width - 1,
        UI_device_height - 1,
        current_MMI_theme->CM_screen_background_filler);

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(CM_text_offset_x + UI_device_width, CM_y);
    }
    else
    {
        gui_move_text_cursor(CM_text_offset_x, CM_y);
    }

    gui_print_text(MMI_message_string);

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(CM_text_offset_x - (CM_text_width + CM_TEXT_SCROLL_GAP), CM_y);
    }
    else
    {
        if ((CM_text_offset_x + CM_text_width + CM_TEXT_SCROLL_GAP) < UI_device_width)
        {
            gui_move_text_cursor(CM_text_offset_x + CM_text_width + CM_TEXT_SCROLL_GAP, CM_y);
        }
    }

    gui_print_text(MMI_message_string);

    gui_start_timer(CM_TEXT_SCROLL_TIME, handle_CM_text_scroll);
    gdi_layer_pop_clip();
    gdi_layer_blt_previous(0, CM_y, UI_device_width - 1, CM_y + CM_text_height - 1);

    gdi_lcd_get_active(&previous_act_lcd);
    if (previous_act_lcd == GDI_LCD_MAIN_LCD_HANDLE)
    {
        UI_set_main_LCD_graphics_context();
    }
    else if (previous_act_lcd == GDI_LCD_SUB_LCD_HANDLE)
    {
        UI_set_sub_LCD_graphics_context();
    }

}   /* end of handle_CM_text_scroll */


/*****************************************************************************
 * FUNCTION
 *  CM_screens_hide_status_icons
 * DESCRIPTION
 *  Hide status icons function used in CM screens.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void CM_screens_hide_status_icons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = *current_MMI_theme->statusbar0_background_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_fill_rectangle_clip(0, 0, UI_device_width - 1, MMI_STATUS_BAR_HEIGHT, c);
}   /* end of CM_screens_hide_status_icons */


/*****************************************************************************
 * FUNCTION
 *  CM_screens_hide_date_time_display
 * DESCRIPTION
 *  Hide date/time function used in CM screens.
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End of x position
 *  y2      [IN]        End of y position
 * RETURNS
 *  void
 *****************************************************************************/
void CM_screens_hide_date_time_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = *current_MMI_theme->datetime_bar_background_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_fill_rectangle_clip(x1, y1, x2, y2, c);
}


/*****************************************************************************
 * FUNCTION
 *  CM_screens_hide_duration_display
 * DESCRIPTION
 *  Hide duration function used in CM screens.
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End of x position
 *  y2      [IN]        End of y position
 * RETURNS
 *  void
 *****************************************************************************/
void CM_screens_hide_duration_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = *current_MMI_theme->datetime_bar_duration_background_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_fill_rectangle_clip(x1, y1, x2, y2, c);
}   /* end of CM_screens_hide_duration_display */


/*****************************************************************************
 * FUNCTION
 *  CM_screens_draw_date_time_bar
 * DESCRIPTION
 *  Function used in CM screens to draw date-time bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void CM_screens_draw_date_time_bar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = *current_MMI_theme->datetime_bar_background_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_fill_rectangle_clip(
        0,
        main_LCD_dt_object.date.y,
        UI_device_width - 1,
        main_LCD_dt_object.time.y + main_LCD_dt_object.time.height - 1,
        c);
}   /* end of CM_screens_draw_date_time_bar */


/*****************************************************************************
 * FUNCTION
 *  CM_screens_draw_duration_bar
 * DESCRIPTION
 *  Function used in CM screens to draw duration bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void CM_screens_draw_duration_bar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = *current_MMI_theme->datetime_bar_duration_background_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_fill_rectangle_clip(
        0,
        main_LCD_dt_object.duration.y,
        UI_device_width - 1,
        main_LCD_dt_object.duration.y + main_LCD_dt_object.duration.height - 1,
        c);
}   /* end of CM_screens_draw_duration_bar */


/*****************************************************************************
 * FUNCTION
 *  CM_draw_scrolling_text_background
 * DESCRIPTION
 *  Ffunction used in CM screens to draw scrolling text background
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End of x position
 *  y2      [IN]        End of y position
 * RETURNS
 *  void
 *****************************************************************************/
void CM_draw_scrolling_text_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_filled_area *f = current_MMI_theme->CM_screen_background_filler;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, f);
}   /* end of CM_draw_scrolling_text_background */


/*****************************************************************************
 * FUNCTION
 *  dialing_screen_hide_left_softkey
 * DESCRIPTION
 *  Left softkey hide function of dialing screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dialing_screen_hide_left_softkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(
        MMI_left_softkey.x,
        MMI_left_softkey.y,
        MMI_left_softkey.x + MMI_left_softkey.width + 2,
        MMI_left_softkey.y + MMI_left_softkey.height + 2);
    gdi_draw_solid_rect(
        MMI_left_softkey.x,
        MMI_left_softkey.y,
        MMI_left_softkey.x + MMI_left_softkey.width + 2,
        MMI_left_softkey.y + MMI_left_softkey.height + 2,
        GDI_COLOR_TRANSPARENT);
    gdi_layer_pop_clip();
}   /* end of dialing_screen_hide_left_softkey */


/*****************************************************************************
 * FUNCTION
 *  dialing_screen_hide_right_softkey
 * DESCRIPTION
 *  Right softkey hide function of dialing screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dialing_screen_hide_right_softkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(
        MMI_right_softkey.x,
        MMI_right_softkey.y,
        MMI_right_softkey.x + MMI_right_softkey.width + 2,
        MMI_right_softkey.y + MMI_right_softkey.height + 2);
    gdi_draw_solid_rect(
        MMI_right_softkey.x,
        MMI_right_softkey.y,
        MMI_right_softkey.x + MMI_right_softkey.width + 2,
        MMI_right_softkey.y + MMI_right_softkey.height + 2,
        GDI_COLOR_TRANSPARENT);
    gdi_layer_pop_clip();
}   /* end of dialing_screen_hide_right_softkey */

#ifdef __MMI_TOUCH_DIAL_SCREEN__

#ifdef __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__
void (*dialing_keypad_call_handler) (void) = NULL;

void (*dialing_keypad_call2_handler)(void) = NULL;
void (*dialing_keypad_call3_handler)(void) = NULL;
void (*dialing_keypad_call4_handler)(void) = NULL;
void (*dialing_keypad_phonebook_handler) (void) = NULL;  


/*****************************************************************************
 * FUNCTION
 *  SetDialingKeypadCallHandler
 * DESCRIPTION
 *  Set hte callback function for Dialing Icon of touch dialing screen
 * PARAMETERS
 *  handler     [IN]        
 *  void (*handler)(void)(?)
 * RETURNS
 *  void
 *****************************************************************************/
void SetDialingKeypadCallHandler(void (*handler) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dialing_keypad_call_handler = handler;
}
//#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
/*****************************************************************************
 * FUNCTION
 *  mmi_idle_sim1_dialpad_call
 * DESCRIPTION
 *  Dual SIM: dial SIM1 call via call icon
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_idle_sim1_dialpad_call(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
    IdleDialPadCall();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_idle_sim2_dialpad_call
 * DESCRIPTION
 *  Dual SIM: dial SIM2 call via call icon
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_idle_sim2_dialpad_call(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
    IdleDialPadCall();
}
void mmi_idle_sim3_dialpad_call(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SIM3);
    IdleDialPadCall();
}
void mmi_idle_sim4_dialpad_call(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SIM4);
    IdleDialPadCall();
}

/*****************************************************************************
 * FUNCTION
 *  SetDialingKeypadCall2Handler
 * DESCRIPTION
 *  Set hte callback function for Dialing Icon 2 of touch dialing screen
 * PARAMETERS
 *  handler     [IN]        keypad call handle function
 * RETURNS
 *  void
 *****************************************************************************/
void SetDialingKeypadCall2Handler(void (*handler) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dialing_keypad_call2_handler = handler;
}
void SetDialingKeypadCall3Handler(void (*handler) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dialing_keypad_call3_handler = handler;
}
void SetDialingKeypadCall4Handler(void (*handler) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dialing_keypad_call4_handler = handler;
}
//#endif /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */

/*****************************************************************************
 * FUNCTION
 *  SetDialingKeypadPhonebookHandler
 * DESCRIPTION
 *  Set hte callback function for Phonebook Icon of touch dialing screen
 * PARAMETERS
 *  handler     [IN]        
 *  void (*handler)(void)(?)
 * RETURNS
 *  void
 *****************************************************************************/
void SetDialingKeypadPhonebookHandler(void (*handler) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dialing_keypad_phonebook_handler = handler;
}
#endif /* __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__ */ 


/*****************************************************************************
 * FUNCTION
 *  PlayDialKeyPadTone
 * DESCRIPTION
 *  Play key tone for dial keypad keys.
 * PARAMETERS
 *  key_type        [IN]        
 *  S16(?)          [IN]        Key_type
 * RETURNS
 *  void
 *****************************************************************************/
void PlayDialKeyPadTone(S16 key_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 key_id = g_dialing_keypad.key_type;
    KEYBRD_MESSAGE msg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg.nKeyCode = KEY_0;

    if (key_id >= MMI_DIALING_KEY_START && key_id < MMI_DIALING_KEY_STAR)
    {
        msg.nKeyCode = KEY_0 + key_id;
    }
    /* 0 */
    else if (key_id == MMI_DIALING_KEY_ZERO)
    {
        msg.nKeyCode = KEY_0;
    }
    /* star key */
    else if (key_id == MMI_DIALING_KEY_STAR)
    {
        msg.nKeyCode = KEY_STAR;
    }
    /* pound key */
    else if (key_id == MMI_DIALING_KEY_HASH)
    {
        msg.nKeyCode = KEY_POUND;
    }
    msg.nMsgType = key_type;
    mmi_kbn_key_tone_hdlr(&msg);
}


/*****************************************************************************
 * FUNCTION
 *  ExecuteDialKeyPadKeyHandler
 * DESCRIPTION
 *  Excute key handler for dial keypad keys.
 * PARAMETERS
 *  key_type        [IN]        
 *  S16(?)          [IN]        Key_type
 * RETURNS
 *  void
 *****************************************************************************/
void ExecuteDialKeyPadKeyHandler(S16 key_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 key_id = g_dialing_keypad.key_type;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* 1-9 */
    if (key_id >= MMI_DIALING_KEY_START && key_id < MMI_DIALING_KEY_STAR)
    {
        ExecuteCurrKeyHandler((S16) (KEY_0 + key_id), key_type);
        PlayDialKeyPadTone(key_type);
    }
    /* 0 */
    else if (key_id == MMI_DIALING_KEY_ZERO)
    {
        ExecuteCurrKeyHandler(KEY_0, key_type);
        PlayDialKeyPadTone(key_type);
    }
    /* star key */
    else if (key_id == MMI_DIALING_KEY_STAR)
    {
        ExecuteCurrKeyHandler(KEY_STAR, key_type);
        PlayDialKeyPadTone(key_type);
    }
    /* pound key */
    else if (key_id == MMI_DIALING_KEY_HASH)
    {
        ExecuteCurrKeyHandler(KEY_POUND, key_type);
        PlayDialKeyPadTone(key_type);
    }
}   /* end of ExecuteDialKeyPadKeyHandler */


/*****************************************************************************
 * FUNCTION
 *  Cate16CategoryControlAreaPenDownHandler
 * DESCRIPTION
 *  Pen down events handler function for category16 control area.
 * PARAMETERS
 *  point                       [IN]        
 *  mmi_pen_point_struct(?)     [IN]        Point
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL Cate16CategoryControlAreaPenDownHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret = MMI_FALSE;
    gui_pen_event_param_struct dialing_key_param;
    gui_dialing_key_pen_enum dialing_key_menu_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Get dialing screen  event type according to current state of dialing screen */
    ret = gui_dialing_screen_translate_pen_event(
            &g_dialing_keypad,
            point.x,
            point.y,
            MMI_PEN_EVENT_DOWN,
            &dialing_key_menu_event,
            &dialing_key_param);

    if (ret)
    {
        ExecuteDialKeyPadKeyHandler(KEY_EVENT_DOWN);
    }
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
    return ret;
}   /* end of Cate16CategoryControlAreaPenDownHandler */


/*****************************************************************************
 * FUNCTION
 *  Cate16CategoryControlAreaPenUpHandler
 * DESCRIPTION
 *  Pen up events handler function for category16 control area.
 * PARAMETERS
 *  point                       [IN]        
 *  mmi_pen_point_struct(?)     [IN]        Point
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL Cate16CategoryControlAreaPenUpHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret = MMI_FALSE;
    gui_pen_event_param_struct dialing_key_param;
    gui_dialing_key_pen_enum dialing_key_menu_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Get dialing screen  event type according to current state of dialing screen */
    ret = gui_dialing_screen_translate_pen_event(
            &g_dialing_keypad,
            point.x,
            point.y,
            MMI_PEN_EVENT_UP,
            &dialing_key_menu_event,
            &dialing_key_param);
    if (ret)
    {
        //              if(dialing_key_menu_event == GUI_DIALING_KEYPAD_PEN_INSERT_HASH ||
        //                      dialing_key_menu_event == GUI_DIALING_KEYPAD_PEN_INSERT_STAR )
        ExecuteDialKeyPadKeyHandler(KEY_EVENT_UP);

    #if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
        if (dialing_key_menu_event == GUI_DIALING_KEYPAD_PEN_INSERT_PHB)
        {
            if (dialing_keypad_phonebook_handler != NULL)
            {
                dialing_keypad_phonebook_handler();
            }
            else
            {
                mmi_phb_idle_enter_phb_list();
            }
        }
#if defined(__MMI_MAINLCD_220X176__)
        else if (dialing_key_menu_event == GUI_DIALING_KEYPAD_PEN_INSERT_PHB_SAVE)
        {
		IdleDialPadSavePhoneBook();
	 }
#endif 
        else if (dialing_key_menu_event == GUI_DIALING_KEYPAD_PEN_INSERT_CALL)
        {
            /* ASSERT(dialing_keypad_call_handler != NULL); */
            /* This handler might be null in flight mode, not necessary to assert */
			if ((mmi_bootup_get_active_flight_mode() == 0 || mmi_bootup_is_sim_valid() == 0) && (dialing_keypad_call_handler != NULL))
			{
				dialing_keypad_call_handler();
			}
        }
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
	    else if(dialing_key_menu_event == GUI_DIALING_KEYPAD_PEN_INSERT_CALL_SLAVE)
	    {
	        if (dialing_keypad_call2_handler != NULL)
	      	{
			    dialing_keypad_call2_handler();
	      	}
	    }
#endif /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */

    #endif /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */ 

    }
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
    return ret;
}   /* end of Cate16CategoryControlAreaPenUpHandler */


/*****************************************************************************
 * FUNCTION
 *  Cate16CategoryControlAreaPenMoveHandler
 * DESCRIPTION
 *  Pen move events handler function for category16 control area.
 * PARAMETERS
 *  point                       [IN]        
 *  mmi_pen_point_struct(?)     [IN]        Point
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL Cate16CategoryControlAreaPenMoveHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 x1, x2, y1, y2;
    gui_dialing_key_pen_enum menu_event;
    gui_pen_event_param_struct dialing_key_param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Get dialing screen  event type according to current state of dialing screen */
    gui_dialing_screen_translate_pen_event(
        &g_dialing_keypad,
        point.x,
        point.y,
        MMI_PEN_EVENT_MOVE,
        &menu_event,
        &dialing_key_param);
    if (menu_event == GUI_DIALING_KEYPAD_HIGHLIGHT_CHANGED || menu_event == GUI_DIALING_KEYPAD_PEN_INSERT_STAR)
    {
        PlayDialKeyPadTone(KEY_EVENT_UP);
        x1 = g_dialing_keypad.selected_key_x;
        y1 = g_dialing_keypad.selected_key_y;

    #if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
        if ( g_dialing_keypad.key_type >= MMI_DIALING_KEY_FUNC)
        {
            x2 = x1 + g_dialing_keypad.func_key_width - 1;
            y2 = y1 + g_dialing_keypad.func_key_height - 1;
        }
        else
    #endif /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */ 
        {
            x2 = x1 + g_dialing_keypad.key_width - 1;
            y2 = y1 + g_dialing_keypad.key_height - 1;
        }

		gdi_layer_push_clip();
		gdi_layer_set_clip(x1, y1,x2, y2);
			
#ifdef __GDI_MEMORY_PROFILE_2__
		gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
	
		//gdi_image_draw_id(0, MMI_DIALING_KEYPAD_LAYER_Y, IMG_DIALING_SCREEN);
#else 
        gdi_image_cache_bmp_draw(x1, y1, &(dialing_keypad.selected_key_bitmap));         
    #endif 
        gdi_layer_pop_clip();

        if (menu_event == GUI_DIALING_KEYPAD_PEN_INSERT_STAR)
        {
            dialer_inputbox_handle_multitap_star_key_up();
        }
    }

    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
    if (GetDTMFKeyUpFlag())
    {
        MakePsStopDTMFTone();
        SetDTMFKeyUpFlag(FALSE);
    }
    return MMI_TRUE;
}   /* end of Cate16CategoryControlAreaPenMoveHandler */


/*****************************************************************************
 * FUNCTION
 *  Cate16CategoryControlAreaPenRepeatHandler
 * DESCRIPTION
 *  Pen repeat events handler function for category16 control area.
 * PARAMETERS
 *  point                       [IN]        
 *  mmi_pen_point_struct(?)     [IN]        Point
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL Cate16CategoryControlAreaPenRepeatHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret;
    gui_pen_event_param_struct dialing_key_param;
    gui_dialing_key_pen_enum dialing_key_menu_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Get dialing screen  event type according to current state of dialing screen */
    ret = gui_dialing_screen_translate_pen_event(
            &g_dialing_keypad,
            point.x,
            point.y,
            MMI_PEN_EVENT_REPEAT,
            &dialing_key_menu_event,
            &dialing_key_param);

    if (ret)
    {
        ExecuteDialKeyPadKeyHandler(KEY_EVENT_REPEAT);
    }

    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
    return MMI_TRUE;
}   /* end of Cate16CategoryControlAreaPenRepeatHandler */


/*****************************************************************************
 * FUNCTION
 *  Cate16CategoryControlAreaPenLongTapHandler
 * DESCRIPTION
 *  Pen long tap events handler function for category16 control area.
 * PARAMETERS
 *  point                       [IN]        
 *  mmi_pen_point_struct(?)     [IN]        Point
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL Cate16CategoryControlAreaPenLongTapHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_pen_event_param_struct dialing_key_param;
    gui_dialing_key_pen_enum dialing_key_menu_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Get dialing screen  event type according to current state of dialing screen */
    gui_dialing_screen_translate_pen_event(
        &g_dialing_keypad,
        point.x,
        point.y,
        MMI_PEN_EVENT_LONG_TAP,
        &dialing_key_menu_event,
        &dialing_key_param);
    ExecuteDialKeyPadKeyHandler(KEY_EVENT_LONG_PRESS);

    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
    return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  Cate16CategoryControlAreaPenAbortHandler
 * DESCRIPTION
 *  Pen Abort events handler function for category16 control area.
 * PARAMETERS
 *  point                       [IN]        
 *  mmi_pen_point_struct(?)     [IN]        Point
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL Cate16CategoryControlAreaPenAbortHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PlayDialKeyPadTone(KEY_EVENT_UP);
    return MMI_TRUE;
}

#endif /* __MMI_TOUCH_DIAL_SCREEN__ */ 

#if (( defined ( __MMI_MAINLCD_176X220__ ) || defined (__MMI_MAINLCD_240X320__)||defined ( __MMI_MAINLCD_220X176__ ) ) && defined (__GDI_MEMORY_PROFILE_2__) )        


/*****************************************************************************
 * FUNCTION
 *  DrawCate16CategoryControlArea
 * DESCRIPTION
 *  Draws the category16 category control area
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
 extern void gui_reset_dialing_func_key_flag(void);
void DrawCate16CategoryControlArea(dm_coordinates *coordinate)
{
#ifdef __MMI_TOUCH_DIAL_SCREEN__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* initialize dialing_keypad */

    /* draw background image */
   TRACE_GDI_FUNCTION();
   mmi_trace(g_sw_GDI, " IMG_DIALING_SCREEN(%d) = 0x%x",IMG_DIALING_SCREEN,  (U8*) GetImage(IMG_DIALING_SCREEN));
    gdi_layer_push_clip();
    gdi_layer_reset_clip();
    gdi_layer_clear(GDI_COLOR_BLACK);
    gdi_image_draw_id(0, 0, IMG_DIALING_SCREEN);

#ifdef __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__
    gui_reset_dialing_func_key_flag();

     
	if (dialing_keypad_call_handler == NULL)
	{
       	gui_show_dialing_key(MMI_DIALING_KEY_CALL, FALSE);
	}
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
     if (dialing_keypad_call2_handler == NULL)
    {
        gui_show_dialing_key(MMI_DIALING_KEY_CALL_SLAVE, FALSE);
    }
#endif /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */

#endif /* __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__ */ 
     

    gdi_layer_pop_clip();

    /* show inputbox */
#else /* __MMI_TOUCH_DIAL_SCREEN__ */ 
    /*-----------------------------------------------------------------*/
    /* Local Variables                                                                           */
    /*-----------------------------------------------------------------*/
    S32 iwidth, iheight;
    S32 dial_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_clear(GDI_COLOR_TRANSPARENT);
    gdi_image_get_dimension_id(IMG_POPUP_TITLE, &iwidth, &iheight);
#ifdef __MMI_MAINLCD_220X176__	
    dial_height = UI_device_height - MMI_STATUS_BAR_HEIGHT - MMI_SOFTKEY_HEIGHT -iheight;
#else
    dial_height = (S32) (iwidth / (1.5));
#endif
    show_dialer_inputbox();
    gdi_layer_push_clip();
#ifdef __MMI_MAINLCD_220X176__	
    gdi_layer_set_clip(
        (UI_device_width >> 1) - (iwidth >> 1),
        MMI_STATUS_BAR_HEIGHT,
        (UI_device_width >> 1) + (iwidth >> 1),
        MMI_STATUS_BAR_HEIGHT+ iheight);
    gdi_image_draw_id(
        (UI_device_width >> 1) - (iwidth >> 1),
        MMI_STATUS_BAR_HEIGHT,
        IMG_POPUP_TITLE);
#else
#ifndef __T800_MAIN_MENU__
    gdi_layer_set_clip(
        (UI_device_width >> 1) - (iwidth >> 1),
        MMI_title_y + MMI_title_height + (MMI_content_height >> 1) - (dial_height >> 1) - iheight,
        (UI_device_width >> 1) + (iwidth >> 1),
        MMI_title_y + MMI_title_height + (MMI_content_height >> 1) - (dial_height >> 1));
#if !defined (__MMI_PROJECT_T33__) && !defined(MEM_ULC_3216)	//guoyt delete for invalid display(proj T33a)@20100323
	gdi_image_draw_id(
        (UI_device_width >> 1) - (iwidth >> 1),
        MMI_title_y + MMI_title_height + (MMI_content_height >> 1) - (dial_height >> 1) - iheight,
        IMG_POPUP_TITLE);
#endif
#endif
#endif
    gdi_layer_pop_clip();

    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt(wgui_layer_1, wgui_base_layer, 0, 0, 0, 0, UI_device_width - 1, UI_device_height - 1);
#endif /* __MMI_TOUCH_DIAL_SCREEN__ */ 
}   /* end of DrawCate16CategoryControlArea */


/*****************************************************************************
 * FUNCTION
 *  DrawCate16CategoryControlArea2
 * DESCRIPTION
 *  Draws the category16 category control area2
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate16CategoryControlArea2(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	TRACE_GDI_FUNCTION();
#ifdef __MMI_SWFLASH__
    if (GetCat34MediaType()== MMI_IDLE_BG_MEDIA_SWFLASH)
    {
        GDI_HANDLE act_layer;

        gdi_layer_get_active(&act_layer);
        SetSWFlashPlayerLayer(act_layer);
        DrawCateSWFlashCategoryControlArea(coordinate);
    }
    else
#endif
    {
        gdi_layer_set_source_key(FALSE, GDI_COLOR_TRANSPARENT);
        draw_wallpaper();
    }
}

#else /* (( defined ( __MMI_MAINLCD_176X220__ ) || defined (__MMI_MAINLCD_240X320__)||defined ( __MMI_MAINLCD_220X176__ ) ) && defined (__GDI_MEMORY_PROFILE_2__) ) */ 


/*****************************************************************************
 * FUNCTION
 *  DrawCate16CategoryControlArea
 * DESCRIPTION
 *  This function is used to draw the category controlled area of category16 screen
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate16CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_filled_area *f = current_MMI_theme->CM_screen_background_filler;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_reset_clip();
    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, f);

    show_status_icons();

#ifndef __MMI_CM_SCREEN_HIDE_DATE_TIME__
    CM_screens_draw_date_time_bar();
    show_main_LCD_dt_display();
#endif /* __MMI_CM_SCREEN_HIDE_DATE_TIME__ */ 
    show_dialer_inputbox();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}   /* end of DrawCate16CategoryControlArea */

#endif /* (( defined ( __MMI_MAINLCD_176X220__ ) || defined (__MMI_MAINLCD_240X320__)||defined ( __MMI_MAINLCD_220X176__ ) ) && defined (__GDI_MEMORY_PROFILE_2__) ) */ 


/*****************************************************************************
 * FUNCTION
 *  RegisterCategory16NavigationKeys
 * DESCRIPTION
 *  Register the keys ofdialing scrren.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterCategory16NavigationKeys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_dialer_inputbox_keys();
}   /* end of RegisterCategory16NavigationKeys */


/*****************************************************************************
 * FUNCTION
 *  SetCategory16RightSoftkeyFunction
 * DESCRIPTION
 *  set the RSK function of dialing scrren.
 * PARAMETERS
 *  f       [IN]        Function pointer
 *  k       [IN]        Event type
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory16RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(k);
    wgui_dialer_inputbox_RSK_function = f;
}   /* end of SetCategory16RightSoftkeyFunction */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory16Screen
 * DESCRIPTION
 *  Displays the category16 screen(Dialing Screen)
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  Buffer                  [IN]        Message string
 *  BufferLength            [IN]        Length of buffer
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/

#if defined (__MMI_WALLPAPER_ON_BOTTOM__)
static UI_image_ID_type  mm_bg_ID;
static S8 *mm_bg_filename;
static S32 mm_bg_x, mm_bg_y;
static U8 mm_bg_opacity;
static S32 mm_bg_flags;
#endif /* defined (__MMI_WALLPAPER_ON_BOTTOM__) */ 

void ShowCategory16Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *Buffer,
        U32 BufferLength,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

#if (!defined (__MMI_TOUCH_DIAL_SCREEN__) && ( defined ( __MMI_MAINLCD_128X160__ ) || defined (__MMI_MAINLCD_128X128__) ||defined ( __MMI_MAINLCD_160X128__ )||defined ( __MMI_MAINLCD_176X220__ )||defined ( __MMI_MAINLCD_220X176__ ) || defined (__MMI_MAINLCD_240X320__)) && defined (__GDI_MEMORY_PROFILE_2__) && !defined(__MMI_UI_DALMATIAN_IDLESCREEN__))
    S32 iheight = 0;
    S32 dial_height = 0;
    S32 iwidth = 0;
#endif /* (!defined (__MMI_TOUCH_DIAL_SCREEN__) && ( defined ( __MMI_MAINLCD_176X220__ )||defined ( __MMI_MAINLCD_220X176__ ) || defined (__MMI_MAINLCD_240X320__)) && defined (__GDI_MEMORY_PROFILE_2__) && !defined(__MMI_UI_DALMATIAN_IDLESCREEN__)) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);

#if ( defined (__MMI_MAINLCD_240X320__) && defined (__MMI_TOUCH_DIAL_SCREEN__) && defined  (__MMI_UI_SMALL_SCREEN_SUPPORT__) )
    set_small_screen();
#endif 

    gdi_layer_lock_frame_buffer();
    in_idle_screen();
    clear_key_handlers();
    clear_left_softkey();
    clear_right_softkey();
    change_right_softkey(right_softkey, right_softkey_icon);
    change_left_softkey(left_softkey, left_softkey_icon);
    register_left_softkey_handler();
    register_right_softkey_handler();

#ifdef __MMI_SWFLASH__
    SetupCat34Media(0xFFFF, NULL);
#endif

#ifdef __MMI_WALLPAPER_ON_BOTTOM__ //082806 240x320 matrix
	wgui_set_wallpaper_on_bottom(MMI_TRUE);
	dm_get_scr_bg_image(&mm_bg_ID, &mm_bg_filename, &mm_bg_x, &mm_bg_y, &mm_bg_opacity);
	dm_set_scr_bg_image(mm_bg_ID, mm_bg_filename, -1, -1, 128);
	dm_get_scr_bg_flag(&mm_bg_flags);
	dm_set_scr_bg_flag(mm_bg_flags | DM_SCR_BG_FORCE_SHOW);
#endif
		
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) && !defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__))        
    if (mmi_bootup_get_active_flight_mode() != 1)    
    {
#ifndef __MMI_MAINLCD_220X176__
        EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
#endif
    }
#endif /* ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) && !defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)) */ 

#if (((defined ( __MMI_MAINLCD_176X220__ )||defined ( __MMI_MAINLCD_220X176__ ) || defined (__MMI_MAINLCD_240X320__)||defined ( __MMI_MAINLCD_128X160__ )||defined ( __MMI_MAINLCD_160X128__ )||defined (__MMI_MAINLCD_128X128__)) && defined (__GDI_MEMORY_PROFILE_2__)) && !defined(__MMI_UI_DALMATIAN_IDLESCREEN__))

	#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
			    ShowStatusIconsTitle();
			    arrange_status_icons();
	#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
		#ifdef __MMI_IDLE_FULL_SCREEN__
			    on_idle_screen = 1;
		#endif
			     
			    set_on_idlescreen(1);
			     
		#ifndef __MMI_TOUCH_DIAL_SCREEN__
			    register_hide_status_icon_bar(0, idle_screen_hide_status_icons_bar0);
			    register_hide_status_icon_bar(1, MMI_dummy_function);
		#endif /* __MMI_TOUCH_DIAL_SCREEN__ */ 
			    set_status_icon_bar_clip(0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1);
			    show_status_icon_bar(0);
			    hide_status_icon_bar(1);
//qiff add for bug 9164
			    idle_screen_hide_status_icons_bar0();
			    redraw_status_icon_bar(0);
	#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 

#else /* (((defined ( __MMI_MAINLCD_176X220__ )||defined ( __MMI_MAINLCD_220X176__ ) || defined (__MMI_MAINLCD_240X320__)) && defined (__GDI_MEMORY_PROFILE_2__)) && !defined(__MMI_UI_DALMATIAN_IDLESCREEN__)) */ 

	#ifndef __MMI_TOUCH_DIAL_SCREEN__
	    set_main_LCD_dt_date_hide_function(CM_screens_hide_date_time_display);
	    set_main_LCD_dt_time_hide_function(CM_screens_hide_date_time_display);
	    set_dt_display(DT_MO_CALL_SCREEN);
	#endif /* __MMI_TOUCH_DIAL_SCREEN__ */ 
    	ShowStatusIconsTitle();

	#ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
	    arrange_status_icons();
	#endif 

#endif /* (((defined ( __MMI_MAINLCD_176X220__ )||defined ( __MMI_MAINLCD_220X176__ ) || defined (__MMI_MAINLCD_240X320__)) && defined (__GDI_MEMORY_PROFILE_2__)) && !defined(__MMI_UI_DALMATIAN_IDLESCREEN__)) */ 

#if (((defined ( __MMI_MAINLCD_176X220__ )||defined ( __MMI_MAINLCD_220X176__ ) || defined (__MMI_MAINLCD_240X320__)||defined ( __MMI_MAINLCD_128X160__ )||defined ( __MMI_MAINLCD_160X128__ )||defined (__MMI_MAINLCD_128X128__)) && defined (__GDI_MEMORY_PROFILE_2__)) && !defined(__MMI_UI_DALMATIAN_IDLESCREEN__))
	#if defined(__MMI_TOUCH_DIAL_SCREEN__) 
	    	/* initialize dialing_keypad */
	    	setup_dialing_keypad(&g_dialing_keypad);

	    	register_default_hide_softkeys();

		#ifdef __MMI_TOUCH_SCREEN__
			 //  entry_full_screen();
		#endif 

		#if defined (__MMI_MAINLCD_240X320__)
		    wgui_setup_dialer_inputbox(
		        MMI_DIALING_KEYPAD_LAYER_X,
		        MMI_DIALING_KEYPAD_LAYER_Y - MMI_DIALING_BOX_HEIGHT,
		        MMI_DIALING_KEYPAD_LAYER_WIDTH,
		        MMI_DIALING_BOX_HEIGHT,
		        (UI_buffer_type) Buffer,
		        BufferLength,
		        MMI_CATEGORY16_ID,
		        get_string(right_softkey),
		        get_image(right_softkey_icon),
		        history_buffer,
		        0);
		#else /* defined (__MMI_MAINLCD_240X320__) */ 
		    wgui_setup_dialer_inputbox(
		        0,
#ifdef __MMI_MAINLCD_220X176__
0,
#else
MMI_status_bar_height,
#endif
		        UI_device_width,
		        29,
		        (UI_buffer_type) Buffer,
		        BufferLength,
		        MMI_CATEGORY16_ID,
		        get_string(right_softkey),
		        get_image(right_softkey_icon),
		        history_buffer,
		        0);
		#endif /* defined (__MMI_MAINLCD_240X320__) */ 

	#else /* not define __MMI_TOUCH_DIAL_SCREEN__ */ 
		    disable_softkey_background();
		    register_hide_left_softkey(dialing_screen_hide_left_softkey);
		    register_hide_right_softkey(dialing_screen_hide_right_softkey);
		    gdi_image_get_dimension_id(IMG_POPUP_TITLE, &iwidth, &iheight);
#ifdef __MMI_MAINLCD_220X176__			
		    dial_height = UI_device_height - MMI_STATUS_BAR_HEIGHT - MMI_SOFTKEY_HEIGHT -iheight ;
		    wgui_setup_dialer_inputbox(
									        (UI_device_width >> 1) - (iwidth >> 1),
									        (MMI_STATUS_BAR_HEIGHT + iheight),
									        iwidth,
									        dial_height,
									        (UI_buffer_type) Buffer,
									        BufferLength,
									        MMI_CATEGORY16_ID,
									        get_string(right_softkey),
									        get_image(right_softkey_icon),
									        history_buffer,
		        						0);
#else
		#ifdef __MMI_MAINLCD_160X128__		
		    iheight = 0;
		    iwidth = UI_device_width;
		#endif	
			
		    dial_height = UI_device_height - MMI_STATUS_BAR_HEIGHT - MMI_SOFTKEY_HEIGHT -iheight ;
		    wgui_setup_dialer_inputbox(
#ifdef __T800_MAIN_MENU__
#if defined(__PROJECT_GALLITE_C01__)
											0,100,176,95,
#else
											0,105,176,95,
#endif
#else
									        (UI_device_width >> 1) - (iwidth >> 1),
									        (MMI_STATUS_BAR_HEIGHT + iheight),
									        iwidth,
									        dial_height,
#endif
									        (UI_buffer_type) Buffer,
									        BufferLength,
									        MMI_CATEGORY16_ID,
									        get_string(right_softkey),
									        get_image(right_softkey_icon),
									        history_buffer,
		        						0);
#endif			
    		    wgui_dialer_inputbox_set_border(MMI_TRUE, UI_COLOR_BLACK);
	#endif /* __MMI_TOUCH_DIAL_SCREEN__ */ 

#else /* (((defined ( __MMI_MAINLCD_176X220__ ) ||defined ( __MMI_MAINLCD_220X176__ )|| defined (__MMI_MAINLCD_240X320__)) && defined (__GDI_MEMORY_PROFILE_2__)) && !defined(__MMI_UI_DALMATIAN_IDLESCREEN__)) */ 

    register_default_hide_softkeys();
    wgui_setup_dialer_inputbox(
     					   0,
				        main_LCD_dt_object.time.y + main_LCD_dt_object.time.height,
				        UI_device_width,
				        UI_device_height - (MMI_button_bar_height + main_LCD_dt_object.time.y + main_LCD_dt_object.time.height),
				        (UI_buffer_type) Buffer,
				        BufferLength,
				        MMI_CATEGORY16_ID,
				        get_string(right_softkey),
				        get_image(right_softkey_icon),
				        history_buffer,
				        0);

#endif /* (((defined ( __MMI_MAINLCD_176X220__ )||defined ( __MMI_MAINLCD_220X176__ ) || defined (__MMI_MAINLCD_240X320__)) && defined (__GDI_MEMORY_PROFILE_2__)) && !defined(__MMI_UI_DALMATIAN_IDLESCREEN__)) */ 

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory16Screen;
    gdi_layer_restore_base_active();
    gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);

#if ( defined (__MMI_MAINLCD_240X320__) && defined (__MMI_TOUCH_DIAL_SCREEN__) )
    gdi_layer_push_clip();
    gdi_layer_reset_clip();

     
	#ifndef __MMI_LITE_DISPLAY__
    	gui_greyscale_rectangle(0, 0, MAIN_LCD_DEVICE_WIDTH - 1, MAIN_LCD_DEVICE_HEIGHT - 1, MMI_BG_GREYSCALE_VALUE, MMI_BG_GREYSCALE_BLACK_VALUE);  
	#endif 

   	 gdi_draw_solid_rect(
	        MMI_DIALING_KEYPAD_LAYER_X,
	        MMI_DIALING_KEYPAD_LAYER_Y,
	        MMI_DIALING_KEYPAD_LAYER_X + MMI_DIALING_KEYPAD_LAYER_WIDTH - 1,
	        MMI_DIALING_KEYPAD_LAYER_Y + MMI_DIALING_KEYPAD_LAYER_HEIGHT - 1,
	        GDI_COLOR_TRANSPARENT);
    gdi_layer_pop_clip();
#else /* ( defined (__MMI_MAINLCD_240X320__) && defined (__MMI_TOUCH_DIAL_SCREEN__) ) */ 
    gdi_layer_clear(GDI_COLOR_TRANSPARENT);
#endif /* ( defined (__MMI_MAINLCD_240X320__) && defined (__MMI_TOUCH_DIAL_SCREEN__) ) */ 

    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_register_category_controlled_callback(DrawCate16CategoryControlArea);
#if (( defined ( __MMI_MAINLCD_176X220__ ) ||defined ( __MMI_MAINLCD_220X176__ )|| defined (__MMI_MAINLCD_240X320__) ) && defined (__GDI_MEMORY_PROFILE_2__) )        
    dm_register_category_controlled2_callback(DrawCate16CategoryControlArea2);
#endif 
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY16_ID;
#ifndef __MMI_TOUCH_DIAL_SCREEN__
    dm_data.S32flags = DM_SPECIFIC_HIDE_STATUS_BAR;
#else 
    dm_data.S32flags = 0;
#endif 
    dm_setup_data(&dm_data);

#ifdef __MMI_TOUCH_DIAL_SCREEN__
    wgui_register_category_screen_control_area_pen_handlers(Cate16CategoryControlAreaPenDownHandler,  MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(Cate16CategoryControlAreaPenUpHandler, MMI_PEN_EVENT_UP);
    wgui_register_category_screen_control_area_pen_handlers(Cate16CategoryControlAreaPenMoveHandler, MMI_PEN_EVENT_MOVE);
    wgui_register_category_screen_control_area_pen_handlers(Cate16CategoryControlAreaPenRepeatHandler, MMI_PEN_EVENT_REPEAT);
    wgui_register_category_screen_control_area_pen_handlers(Cate16CategoryControlAreaPenLongTapHandler, MMI_PEN_EVENT_LONG_TAP);
    wgui_register_category_screen_control_area_pen_handlers(Cate16CategoryControlAreaPenAbortHandler,  MMI_PEN_EVENT_ABORT);
#endif /* __MMI_TOUCH_DIAL_SCREEN__ */ 

    dm_redraw_category_screen();

#ifdef __MMI_SWFLASH__
    HandleCat34MediaPlayFailed();
#endif
}   /* end of ShowCategory16Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory16Screen
 * DESCRIPTION
 *  Exits the category16 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory16Screen(void)
{
#if( defined (__MMI_MAINLCD_240X320__) && defined (__MMI_TOUCH_DIAL_SCREEN__) && defined (__MMI_UI_SMALL_SCREEN_SUPPORT__) )
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_small_screen();
#endif /* ( defined (__MMI_MAINLCD_240X320__) && defined (__MMI_TOUCH_DIAL_SCREEN__) && defined (__MMI_UI_SMALL_SCREEN_SUPPORT__) ) */ 

    gdi_layer_lock_frame_buffer();
    dialer_inputbox_handle_multitap_complete(WGUI_DIALER_BOX_ACTIVE_MULTITAP_ANY);
    reset_multitaps();
    close_dialer_inputbox();
    close_main_LCD_dt_display();
    close_status_icons();
    reset_softkeys();
#if (( defined ( __MMI_MAINLCD_128X160__)||defined ( __MMI_MAINLCD_160X128__)||defined ( __MMI_MAINLCD_176X220__ ) ||defined ( __MMI_MAINLCD_220X176__ )|| defined (__MMI_MAINLCD_240X320__)||defined (__MMI_MAINLCD_128X128__)) && defined (__GDI_MEMORY_PROFILE_2__) )
#ifdef __MMI_IDLE_FULL_SCREEN__
    on_idle_screen = 0;
#endif
     
    set_on_idlescreen(0);
     
    leave_idle_screen();
    enactive_main_lcd_update_date_time();
    enable_softkey_background();
    register_hide_status_icon_bar(1, MMI_dummy_function);
    register_hide_status_icon_bar(0, MMI_dummy_function);
#endif /* (( defined ( __MMI_MAINLCD_176X220__ )||defined ( __MMI_MAINLCD_220X176__ ) || defined (__MMI_MAINLCD_240X320__)) && defined (__GDI_MEMORY_PROFILE_2__) ) */ 

#ifdef __MMI_WALLPAPER_ON_BOTTOM__//082806 240x320 matrix
	dm_set_scr_bg_flag(mm_bg_flags);
	dm_set_scr_bg_image_no_draw(mm_bg_ID, mm_bg_filename, mm_bg_x, mm_bg_y, mm_bg_opacity);
	wgui_reset_wallpaper_on_bottom();
#endif

	gdi_layer_unlock_frame_buffer();

#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
#if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
    dialing_keypad_call_handler = NULL;
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
    dialing_keypad_call2_handler = NULL;
#endif /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
    dialing_keypad_phonebook_handler = NULL;     
#else /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */ 
    ResetCenterSoftkey();
#endif /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */ 
#endif /* ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__)) */ 

#ifdef __MMI_SWFLASH__
    if ( GetCat34MediaType() == MMI_IDLE_BG_MEDIA_SWFLASH )
    {
        ExitCategorySWFlashScreen();
    }
#endif
}   /* end of ExitCategory16Screen */


/*****************************************************************************
 * FUNCTION
 *  GetCategory16History
 * DESCRIPTION
 *  Get the category16 screen history
 * PARAMETERS
 *  history_buffer      [?]             
 *  U8(?)               [IN/OUT]        History_buffer
 * RETURNS
 *  U8*
 *****************************************************************************/
U8 *GetCategory16History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_dialer_inputbox_category_history(MMI_CATEGORY16_ID, history_buffer);
    return (history_buffer);
}   /* end of GetCategory16History */


/*****************************************************************************
 * FUNCTION
 *  GetCategory16HistorySize
 * DESCRIPTION
 *  Get the category16 screen history size
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
S32 GetCategory16HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return sizeof(dialer_inputbox_category_history);
}   /* end of GetCategory16HistorySize */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory17Screen
 * DESCRIPTION
 *  Show category ShowCategory17Screen screen.
 * PARAMETERS
 *  title_id                    [IN]        
 *  left_softkey                [IN]        Lsk string id
 *  left_softkey_icon           [IN]        Lsk image id
 *  right_softkey               [IN]        Rsk string id
 *  right_softkey_icon          [IN]        Rsk image id
 *  NotificationStringId        [IN]        Notification string, notification string
 *  NameOrNumber                [IN]        Name or number text, name or number text
 *  IP_Number                   [IN]        
 *  default_image_id            [IN]        
 *  resource_id                 [IN]        
 *  resource_filename           [IN]        
 *  resource_type               [IN]        
 *  repeat_count                [IN]        Repeat count, used for video only, 0 means infiniate loop
 *  is_visaul_update            [IN]        
 *  is_aud_enable               [IN]        
 *  is_play_aud_when_start      [IN]        
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory17Screen(
        U16 title_id,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 NotificationStringId,
        PU8 NameOrNumber,
        PU8 IP_Number,
        U16 default_image_id,
        U16 resource_id,
        PS8 resource_filename,
        wgui_cate_momt_res_type_enum resource_type,
        U16 repeat_count,
        BOOL is_visaul_update,
        BOOL is_aud_enable,
        BOOL is_play_aud_when_start,
        PU8 history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowMOMTCallScreen(
        title_id,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        NotificationStringId,
        NameOrNumber,
        IP_Number,
        default_image_id,
        resource_id,
        resource_filename,
        resource_type,
        repeat_count,
        is_visaul_update,
        is_aud_enable,
        is_play_aud_when_start,
        history_buffer);
}   /* end of ShowCategory17Screen */


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory17Screen
 * DESCRIPTION
 *  Redraw Category 17 screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory17Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawMOMTCallScreen();
}   /* end of RedrawCategory17Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory17Screen
 * DESCRIPTION
 *  Exit Category 17 screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory17Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ExitMOMTCallScreen();
}   /* end of ExitCategory17Screen */


/*****************************************************************************
 * FUNCTION
 *  StopCategory17Video
 * DESCRIPTION
 *  Stop Category 17 screen's video clip.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern MDI_RESULT mdi_video_ply_close_file(void);
void StopCategory17Video(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_VIDEO_PLAYER__
	  	/* enable key pad tone */
  	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
    if (cat_momt_is_video_open && cat_momt_is_video_play)
    {
        /* stop video playing */
        mdi_video_ply_stop();
        cat_momt_is_video_play = FALSE;
		mdi_video_ply_close_file();
		cat_momt_is_video_open = FALSE;
    }
#endif /* __MMI_VIDEO_PLAYER__ */ 
}   /* end of StopCategory17Video */


/*****************************************************************************
 * FUNCTION
 *  DisableCategory17Audio
 * DESCRIPTION
 *  Disable Category 17 screen video' audio
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisableCategory17Audio(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!cat_momt_is_show_default)
    {
        if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE) ||
            (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID))
        {
        #ifdef __MMI_VIDEO_PLAYER__
            if (cat_momt_is_video_open && cat_momt_is_video_play)
            {
                mdi_audio_set_mute(MDI_VOLUME_MEDIA, TRUE);
                cat_momt_is_aud_muted = TRUE;
            }
        #endif /* __MMI_VIDEO_PLAYER__ */ 
        }
        else if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE) ||
                 (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID))
        {
        #ifdef __MMI_SWFLASH__
            if (cat_momt_is_swflash_open && cat_momt_is_swflash_play)
            {
                mdi_audio_set_mute(MDI_VOLUME_MEDIA, TRUE);
                cat_momt_is_aud_muted = TRUE;
            }
        #endif /* __MMI_SWFLASH__ */ 

        }

    }

}


/*****************************************************************************
 * FUNCTION
 *  EnableCategory17Audio
 * DESCRIPTION
 *  Enable Category 17 screen video' audio
 * PARAMETERS
 *  void
 *  level(?)        [IN]        Audio level
 * RETURNS
 *  void
 *****************************************************************************/
void EnableCategory17Audio(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!cat_momt_is_show_default)
    {
        if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE) ||
            (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID))
        {
        #ifdef __MMI_VIDEO_PLAYER__
            if (cat_momt_is_video_open && cat_momt_is_video_play)
            {
                mdi_audio_set_mute(MDI_VOLUME_MEDIA, FALSE);
                cat_momt_is_aud_muted = FALSE;
            }
        #endif /* __MMI_VIDEO_PLAYER__ */ 
        }
        else if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE) ||
                 (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID))
        {
        #ifdef __MMI_SWFLASH__
            if (cat_momt_is_swflash_open && cat_momt_is_swflash_play)
            {
                mdi_audio_set_mute(MDI_VOLUME_MEDIA, FALSE);
                cat_momt_is_aud_muted = FALSE;
            }
        #endif /* __MMI_SWFLASH__ */ 
        }
    }

}


/*****************************************************************************
 * FUNCTION
 *  DisableCategory17VideoUpdate
 * DESCRIPTION
 *  Disable update video to LCM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisableCategory17VideoUpdate(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_VIDEO_PLAYER__
    if (cat_momt_is_video_open && cat_momt_is_video_play)
    {
        mdi_video_ply_set_lcm_update(FALSE);
    }
#endif /* __MMI_VIDEO__ */ /* __MMI_VIDEO_PLAYER__ */
}


/*****************************************************************************
 * FUNCTION
 *  EnableCategory17VideoUpdate
 * DESCRIPTION
 *  Enable update video to LCM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EnableCategory17VideoUpdate(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_VIDEO_PLAYER__
    if (cat_momt_is_video_open && cat_momt_is_video_play)
    {
        mdi_video_ply_set_lcm_update(TRUE);
    }
#endif /* __MMI_VIDEO_PLAYER__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  IsCategory17VideoValid
 * DESCRIPTION
 *  Call this function to check if Cat17 show video success or not
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *****************************************************************************/
MMI_BOOL IsCategory17VideoValid(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("[WGUI CAT CM] IsCategory17VideoValid()"));
    PRINT_INFORMATION(("[WGUI CAT CM] open %d", cat_momt_is_video_open));
    PRINT_INFORMATION(("[WGUI CAT CM] play %d", cat_momt_is_video_play));
#ifdef __MMI_VIDEO_PLAYER__
    if (cat_momt_is_video_open && cat_momt_is_video_play)
    {
        return MMI_TRUE;
    }
#endif /* __MMI_VIDEO_PLAYER__ */ 

    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  IsCategory17SWFlashValid
 * DESCRIPTION
 *  Call this function to check if Cat17 show swflash success or not
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *****************************************************************************/
MMI_BOOL IsCategory17SWFlashValid(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_SWFLASH__
    if (cat_momt_is_swflash_open && cat_momt_is_swflash_play)
    {
        return MMI_TRUE;
    }
#endif /* __MMI_SWFLASH__ */ 

    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory18Screen
 * DESCRIPTION
 *  Show Cateogry 18, for MO MT call. for call disconnecting screen
 * PARAMETERS
 *  title_id                    [IN]        
 *  left_softkey                [IN]        Lsk string id
 *  left_softkey_icon           [IN]        Lsk image id
 *  right_softkey               [IN]        Rsk string id
 *  right_softkey_icon          [IN]        Rsk image id
 *  NotificationStringId        [IN]        Notification string, notification string
 *  NameOrNumber                [IN]        Name or number text, name or number text
 *  IP_Number                   [IN]        
 *  image_id                    [IN]        Resource to be displayed (ID)
 *  image_filename              [IN]        Resource to be displayed (file)
 *  isDisconnecting             [IN]        
 *  history_buffer              [IN]        History buffer
 *  title_icon(?)               [IN]        Title image id
 *  title(?)                    [IN]        Title string id
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory18Screen(
        U16 title_id,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 NotificationStringId,
        PU8 NameOrNumber,
        PU8 IP_Number,
        U16 image_id,
        PS8 image_filename,
        MMI_BOOL isDisconnecting,
        PU8 history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    isCallDisconnecting = isDisconnecting;

    if (image_filename)
    {
        ShowMOMTCallScreen(
            title_id,
            left_softkey,
            left_softkey_icon,
            right_softkey,
            right_softkey_icon,
            NotificationStringId,
            NameOrNumber,
            IP_Number,
            image_id,
            0,
            image_filename,
            WGUI_CATE_MOMT_RES_TYPE_IMAGE_FILE,
            0,
            FALSE,
            FALSE,
            FALSE,
            history_buffer);

    }
    else
    {
        ShowMOMTCallScreen(
            title_id,
            left_softkey,
            left_softkey_icon,
            right_softkey,
            right_softkey_icon,
            NotificationStringId,
            NameOrNumber,
            IP_Number,
            image_id,
            image_id,
            NULL,
            WGUI_CATE_MOMT_RES_TYPE_IMAGE_ID,
            0,
            FALSE,
            FALSE,
            FALSE,
            history_buffer);
    }

}


MMI_BOOL CM_call_cost_show = MMI_FALSE;//102506 call cost
/*****************************************************************************
 * FUNCTION
 *  Cat19_hide_fixed_list
 * DESCRIPTION
 *  hide the fixed list of category 19
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Cat19_hide_fixed_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_fixed_list_menu.normal_filler->gc)
    {
        S32 x1 = MMI_fixed_list_menu.x + 2;
        S32 y1 = MMI_fixed_list_menu.y + 2;
        S32 x2 = x1 + MMI_fixed_list_menu.width - 8;
        S32 y2 = y1 + MMI_fixed_list_menu.height - 7;

        gdi_layer_push_clip();
        gdi_layer_reset_clip();
        gui_gradient_fill_rectangle(x1, y1, x2, y2, MMI_fixed_list_menu.normal_filler->gc, UI_GRADIENT_COLOR_VERTICAL);
        gdi_layer_pop_clip();
    }
    else
    {
        gdi_layer_push_clip();
        gdi_layer_set_clip(
            MMI_fixed_list_menu.x,
            MMI_fixed_list_menu.y,
            MMI_fixed_list_menu.x + MMI_fixed_list_menu.width - 1,
            MMI_fixed_list_menu.y + MMI_fixed_list_menu.height - 1);
        gui_draw_filled_area(
            0,
            0,
            UI_device_width - 1,
            UI_device_height - 1,
            current_MMI_theme->CM_screen_background_filler);
        gdi_layer_pop_clip();
    }
}   /* end of Cat19_hide_fixed_list */


/*****************************************************************************
 * FUNCTION
 *  DrawCate19CategoryControlArea
 * DESCRIPTION
 *  Show control area of category19
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  dm_coordinates    IN    coordinate(?)
 *****************************************************************************/
void DrawCate19CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory19Screen();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory19Screen
 * DESCRIPTION
 *  Redraw function of category19
 * PARAMETERS
 *  void
 * RETURNS
 *  dm_coordinates    IN    coordinate(?)
 *****************************************************************************/
void RedrawCategory19Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_filled_area *f = current_MMI_theme->CM_screen_background_filler;
    U8 aoc_flag;//102506 call cost

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_reset_clip();

    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, f);

#ifndef __MMI_CM_SCREEN_HIDE_DATE_TIME__
    CM_screens_draw_date_time_bar();
    show_main_LCD_dt_display();
#endif /* __MMI_CM_SCREEN_HIDE_DATE_TIME__ */ 

    if (!(FALSE == GetCallTimeDisplay()))
    {
        CM_screens_draw_duration_bar();
    }

    show_main_LCD_dt_display();

    /* 102506 call cost Start */
    aoc_flag = (U8) GetShowAOC();
    if (aoc_flag)
    {
        CM_call_cost_show = MMI_TRUE;
    redraw_call_cost_UI();
    }
    else
    {
        CM_call_cost_show = MMI_FALSE;
        reset_call_cost_UI();
    }
    //redraw_call_cost_UI();
    /* 102506 call cost End */
    
    show_fixed_list();

    gdi_layer_pop_clip();

}


/*****************************************************************************
 * FUNCTION
 *  Cat19_list_highlight_handler
 * DESCRIPTION
 *  highlight handler for category19
 * PARAMETERS
 *  item_index      [IN]        Index of item highlighted.
 * RETURNS
 *  void
 *****************************************************************************/
void Cat19_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    MMI_list_highlight_handler(item_index);
}   /* end of Cat19_list_highlight_handler */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory19Screen
 * DESCRIPTION
 *  Displays the active calls screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_calls         [IN]        
 *  CallList                [IN]        
 *  CallStates              [?]         
 *  CurrCallTime            [?]         
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 *  timer(?)                [IN]        Call duration timer
 *  calls(?)                [IN]        Number fo active calls
 *  Id(?)                   [IN]        Name Or Number identifying the call
 *  message(?)              [IN]        Message string
 *  Images(?)               [IN]        Images to show call state
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory19Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_calls,
        U8 **CallList,
        U16 *CallStates,
        MYTIME *CurrCallTime,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i, text_width, icon_position;
    S32 w, h, temp_w, temp_h;
    U8 h_flag;
    U8 *img = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    entry_full_screen();
    gdi_layer_lock_frame_buffer();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
#ifdef __FLIGHT_MODE_SUPPORT__
	if (mmi_bootup_get_active_flight_mode() != 1)
#endif
	    EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
#endif 

    set_main_LCD_dt_date_hide_function(CM_screens_hide_date_time_display);
    set_main_LCD_dt_time_hide_function(CM_screens_hide_date_time_display);

    if (!(FALSE == GetCallTimeDisplay()))
    {
        set_main_LCD_dt_duration_hide_function(CM_screens_hide_duration_display);
    }

    set_dt_duration(CurrCallTime);
    set_dt_display(DT_ACTIVE_CALL_SCREEN);

    ShowStatusIconsTitle();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_ALIGN_TO_TOP;
	
    /* resize_fixed_list(MMI_content_width+12,UI_device_height-(main_LCD_dt_object.duration.y+main_LCD_dt_object.duration.height)-MMI_button_bar_height);//+4); */
    resize_fixed_list(
        MMI_content_width + MMI_fixed_list_menu.vbar.width,
        UI_device_height - MMI_button_bar_height - (main_LCD_dt_object.duration.y + main_LCD_dt_object.duration.height));

    create_fixed_icontext_list_menuitems(1, 1);

    //      text_width=UI_device_width-16-6;
    //      icon_position=4+text_width;
    //      set_fixed_icontext_list_text_coordinates(0,2,0,text_width,18);
    //      set_fixed_icontext_list_icon_coordinates(0,icon_position,0,16,18);

    img = (U8*) GetImage(CallStates[0]);
    gui_measure_image(img, &w, &h);
    for (i = 1; i < number_of_calls; i++)
    {
        img = (U8*) GetImage(CallStates[i]);
        gui_measure_image(img, &temp_w, &temp_h);
        if (temp_w > w)
        {
            w = temp_w;
        }
        if (temp_h > h)
        {
            h = temp_h;
        }
    }

    if (w > MMI_MENUITEM_HEIGHT)
    {
        w = MMI_MENUITEM_HEIGHT;
    }
    if (h > MMI_MENUITEM_HEIGHT)
    {
        h = MMI_MENUITEM_HEIGHT;
    }

    text_width = UI_device_width - w - 6;
    icon_position = 4 + text_width;
    set_fixed_icontext_list_text_coordinates(0, 2, 0, text_width, MMI_MENUITEM_HEIGHT);
    set_fixed_icontext_list_icon_coordinates(0, icon_position, 0, w, h);

    associate_fixed_icontext_list_list();

    MMI_fixed_icontext_list_menuitem.flags &= ~UI_MENUITEM_MARQUEE_SCROLL;
    MMI_fixed_icontext_list_menuitem.flags |= UI_MENUITEM_TRUNCATE_CONTENT;

    for (i = 0; i < number_of_calls; i++)
    {
        fixed_icontext_list_item_insert(i);
        add_fixed_icontext_list_item_text(i, 0, (UI_string_type) CallList[i]);
        add_fixed_icontext_list_item_icon(i, 0, get_image(CallStates[i]));
    }

    MMI_current_menu_type = LIST_MENU;
    register_fixed_list_keys();
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR ; //qiff modify for bug 8858 (UI_LIST_MENU_DISABLE_BACKGROUND | UI_LIST_MENU_DISABLE_SCROLLBAR);
    register_hide_fixed_list(Cat19_hide_fixed_list);
    resize_fixed_icontext_list_menuitems(0, MMI_MENUITEM_HEIGHT);

    register_fixed_list_highlight_handler(Cat19_list_highlight_handler);

    h_flag = set_list_menu_category_history(MMI_CATEGORY19_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }

    /* resize_fixed_list(MMI_content_width+12,UI_device_height-(main_LCD_dt_object.duration.y+main_LCD_dt_object.duration.height)-MMI_button_bar_height); */
    resize_fixed_list(
        MMI_content_width + MMI_fixed_list_menu.vbar.width,
        UI_device_height - MMI_button_bar_height - (main_LCD_dt_object.duration.y + main_LCD_dt_object.duration.height));

    //move_fixed_list(0, UI_device_height - MMI_button_bar_height - MMI_fixed_list_menu.height - 1);
    move_fixed_list(0, UI_device_height - MMI_button_bar_height - MMI_fixed_list_menu.height);//082506 active calls

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory19Screen;
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory19History, GetCategory19HistorySize);
    dm_register_category_controlled_callback(DrawCate19CategoryControlArea);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY19_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
    
/* 102506 call cost Start */
#if 0
#endif
/* 102506 call cost End */
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory19Screen
 * DESCRIPTION
 *  Exits the Active_call screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory19Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    close_main_LCD_dt_display();
    close_status_icons();
    reset_softkeys();
    reset_menu_shortcut_handler();
    MMI_fixed_list_menu.flags &= ~(UI_LIST_MENU_DISABLE_BACKGROUND | UI_LIST_MENU_DISABLE_SCROLLBAR);
    _MMI_hide_fixed_list_menu = UI_dummy_function;
    reset_fixed_list();
    gdi_layer_unlock_frame_buffer();
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
    ResetCenterSoftkey();
#endif 
}   /* end of ExitCategory19Screen */


/*****************************************************************************
 * FUNCTION
 *  GetCategory19HistorySize
 * DESCRIPTION
 *  Gets the size of the history buffer for the Active calls screen
 * PARAMETERS
 *  void
 * RETURNS
 *  size of history buffer in U8s
 *****************************************************************************/
S32 GetCategory19HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (sizeof(list_menu_category_history));
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory19History
 * DESCRIPTION
 *  Gets the history buffer for the Active_calls screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory19History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history(MMI_CATEGORY19_ID, history_buffer);
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  DrawCate20CategoryControlArea
 * DESCRIPTION
 *  This function is used to draw the category controlled area of category20 screen
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate20CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory20Screen();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory20Screen
 * DESCRIPTION
 *  Redraws the single Active call screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory20Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 width, x = 0;
    UI_filled_area *f = current_MMI_theme->CM_screen_background_filler;
    U8 aoc_flag;//102506 call cost


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gui_hide_animations();

    gdi_layer_reset_clip();
    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, f);
#if defined(__MMI_MAINLCD_220X176__)
	MMI_title_string = NULL;
#endif
    show_title_status_icon();
    show_status_icons();

#ifdef __MMI_MAINLCD_128X128__
#ifndef __MMI_CM_SCREEN_HIDE_DATE_TIME__
    CM_screens_draw_date_time_bar();
    show_main_LCD_dt_display();
#endif /* __MMI_CM_SCREEN_HIDE_DATE_TIME__ */ 
#else /* __MMI_MAINLCD_128X128__ */ 
    set_main_lcd_duration_position(0, (U16) MMI_status_bar_height);
#endif /* __MMI_MAINLCD_128X128__ */ 

    if (!(FALSE == GetCallTimeDisplay()))
    {
        CM_screens_draw_duration_bar();
        //redraw_call_cost_UI();//102506 call cost
    }

    /* 102506 call cost Start */
    aoc_flag = (U8) GetShowAOC();
    if (aoc_flag)
    {
        CM_call_cost_show = MMI_TRUE;
        redraw_call_cost_UI();
    }
    else
    {
        CM_call_cost_show = MMI_FALSE;
        reset_call_cost_UI();
    }
    /* 102506 call cost End */

#ifdef __MMI_MAINLCD_128X128__
    show_main_LCD_dt_display();
#else /* __MMI_MAINLCD_128X128__ */ 
    if (!(FALSE == GetCallTimeDisplay()))
    {
        dt_update_duration();
    }
#endif /* __MMI_MAINLCD_128X128__ */ 

    if (MMI_message_string)
    {
        gui_set_font(current_MMI_theme->title_text_font);
        gui_measure_string(MMI_message_string, &width, &CM_text_height);
        if (width < UI_device_width)
        {
            x = (UI_device_width >> 1) - (width >> 1);
        }
        gdi_layer_set_clip(0, CM_y, (UI_device_width - 1), (CM_y + CM_text_height));
        gui_move_text_cursor(x, CM_y);
    #ifdef __MMI_MAINLCD_128X128__
        CM_scrolling_text.x = x;
    #else 
        CM_scrolling_text.x = 0;
    #endif 
#if defined(__MMI_MAINLCD_220X176__)
		CM_scrolling_text.text_color = gui_color(0,0,0);
#endif
        gui_show_scrolling_text(&CM_scrolling_text);
    }

    gdi_layer_push_clip();
    gdi_layer_set_clip(wgui_image_clip_x1, wgui_image_clip_y1, wgui_image_clip_x2, wgui_image_clip_y2);
    gdi_image_draw_animation(wgui_image_clip_x1, wgui_image_clip_y1, _MMI_animated_icon, &MOMT_animation_handle);
    gdi_layer_pop_clip();

    if (isCallDisconnecting)
    {
        MOMTHideAnimation();
    }

    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory20Screen
 * DESCRIPTION
 *  Displays the single active call screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  NameOrNumber            [?]         
 *  CallDuration            [?]         
 *  CallState               [IN]        
 *  isDisconnecting         [IN]        
 *  history_buffer          [IN]        History buffer
 *  timer(?)                [IN]        Call duration timer
 *  Id(?)                   [IN]        Name Or Number identifying the call
 *  Images(?)               [IN]        Images to show call state
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory20Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *NameOrNumber,
        MYTIME *CallDuration,
        U16 CallState,
        MMI_BOOL isDisconnecting,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

#ifdef __MMI_MAINLCD_128X128__
    S32 h;
#endif 
    S32 l = 0;
    S32 y = 0;
    S32 ty2 = 0;
    S32 sh = 0;
    S32 sw = 0;
    S32 tw = 0;
    S32 tx = 0;
    S32 ty = 0;
    S32 iwidth = 0;
    S32 iheight = 0;
    S32 content_height;
    PU8 img = get_image(CallState);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);

    gdi_layer_lock_frame_buffer();

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
#ifdef __FLIGHT_MODE_SUPPORT__
	if (mmi_bootup_get_active_flight_mode() != 1)
#endif
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
#endif 

    isCallDisconnecting = isDisconnecting;

    set_main_LCD_dt_date_hide_function(CM_screens_hide_date_time_display);
    set_main_LCD_dt_time_hide_function(CM_screens_hide_date_time_display);

    if (!(FALSE == GetCallTimeDisplay()))
    {
        set_main_LCD_dt_duration_hide_function(CM_screens_hide_duration_display);
    }

    set_dt_duration(CallDuration);
    set_dt_display(DT_ACTIVE_CALL_SCREEN);
    ty2 = main_LCD_dt_object.duration.y + main_LCD_dt_object.duration.height + 2;
    content_height = UI_device_height - MMI_button_bar_height - ty2;
    ShowStatusIconsTitle();

    gdi_image_get_dimension((U8*) img, &iwidth, &iheight);

    l = gui_strlen((UI_string_type) NameOrNumber);
    if (NameOrNumber)
    {
        MMI_message_string = (UI_string_type) NameOrNumber;
    }
    else
    {
        MMI_message_string = NULL;
    }
    gui_set_font(current_MMI_theme->title_text_font);
    gui_measure_string((UI_string_type) NameOrNumber, &sw, &sh);
    tx = (UI_device_width >> 1) - (sw >> 1);
    if (tx < 1)
    {
        tx = 1;
    }

#ifdef __MMI_MAINLCD_128X128__
    ty = UI_device_height - MMI_button_bar_height - sh - 2;
    tw = sw;
    if (tw > (UI_device_width - 2))
    {
        tw = UI_device_width - 2;
    }

 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
    gui_create_scrolling_text(
        &CM_scrolling_text,
        tx,
        ty,
        tw,
        sh,
        (UI_string_type) NameOrNumber,
        CM_scrolling_text_timer_callback,
        CM_draw_scrolling_text_background,
        *current_MMI_theme->CM_screen_text_color,
        gui_color(255, 255, 255));
#else /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 
    gui_create_scrolling_text(
        &CM_scrolling_text,
        tx,
        ty,
        tw,
        sh,
        (UI_string_type) NameOrNumber,
        CM_scrolling_text_timer_callback,
        CM_draw_scrolling_text_background,
        gui_color(0, 0, 0),
        gui_color(255, 255, 255));
#endif /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 
    gui_change_scrolling_text_font(&CM_scrolling_text, current_MMI_theme->title_text_font);
 
    CM_scrolling_text.flags &= ~UI_SCROLLING_TEXT_BORDERED_TEXT;
#ifdef __MMI_MAINLCD_220X176__
	CM_scrolling_text.text_font = MMI_medium_font;
#endif
#if defined(__MMI_MAINLCD_128X128__)
    h = iheight + 4;
#else
    h = sh + iheight + 4;
#endif
    y = ty2 + (content_height >> 1) - (h >> 1);
#else /* __MMI_MAINLCD_128X128__ */ 
    ty = MMI_status_bar_height + main_LCD_dt_object.duration.height + 1;         
    tw = sw;
    if (tw > (UI_device_width - 2))
    {
        tw = UI_device_width - 2;
    }
 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
    gui_create_scrolling_text(
        &CM_scrolling_text,
        0,
        ty,
        tw,
        sh,
        (UI_string_type) NameOrNumber,
        CM_scrolling_text_timer_callback,
        CM_draw_scrolling_text_background,
        *current_MMI_theme->CM_screen_text_color,
        gui_color(255, 255, 255));
#else /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 
    gui_create_scrolling_text(
        &CM_scrolling_text,
        0,
        ty,
        tw,
        sh,
        (UI_string_type) NameOrNumber,
        CM_scrolling_text_timer_callback,
        CM_draw_scrolling_text_background,
        gui_color(0, 0, 0),
        gui_color(255, 255, 255));
#endif /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 
    gui_change_scrolling_text_font(&CM_scrolling_text, current_MMI_theme->title_text_font);
 
    CM_scrolling_text.flags &= ~UI_SCROLLING_TEXT_BORDERED_TEXT;
#ifdef __MMI_MAINLCD_220X176__
	CM_scrolling_text.text_font = &MMI_medium_font;
    CM_scrolling_text.text_color = gui_color(255, 255, 255);
#endif

     
    //      y=UI_device_height-MMI_button_bar_height-iheight-1;
    y = UI_device_height - MMI_button_bar_height - iheight;
     
#endif /* __MMI_MAINLCD_128X128__ */ 

    wgui_image_clip_x1 = (UI_device_width - iwidth) >> 1;
    wgui_image_clip_x2 = (UI_device_width + iwidth) >> 1;
    wgui_image_clip_y1 = y;
     
#ifdef __MMI_MAINLCD_128X128__
    wgui_image_clip_y2 = y + iheight;
#else 
    wgui_image_clip_y2 = y + iheight - 1;
#endif 
     
    _MMI_animated_icon = img;

    gdi_layer_unlock_frame_buffer();

#ifndef __MMI_MAINLCD_128X128__
    deactive_main_lcd_update_date_time();
#endif 

    ExitCategoryFunction = ExitCategory20Screen;
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory20History, GetCategory20HistorySize);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY20_ID;
#if defined(__MMI_MAINLCD_220X176__) 
    dm_data.S32flags = DM_NO_FLAGS;
#else
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
#endif
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawCate20CategoryControlArea);
    dm_redraw_category_screen();

/* 102506 call cost Start */
#if 0
#endif
/* 102506 call cost End */
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory20Screen
 * DESCRIPTION
 *  Exits the single Active_call screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory20Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gui_cancel_timer(CM_scrolling_text_timer_callback);
    gui_cancel_timer(handle_CM_text_scroll);
    gui_hide_animations();

    MOMT_animation_handle = GDI_ERROR_HANDLE;
    isCallDisconnecting = MMI_FALSE;

    close_main_LCD_dt_display();
    close_status_icons();
    reset_softkeys();
    reset_singleline_inputbox();

#ifndef __MMI_MAINLCD_128X128__
    enactive_main_lcd_update_date_time();
#endif 

    gdi_layer_unlock_frame_buffer();
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
    ResetCenterSoftkey();
#endif 
}   /* end of ExitCategory20Screen */


/*****************************************************************************
 * FUNCTION
 *  GetCategory20HistorySize
 * DESCRIPTION
 *  Gets the size of the history buffer of category 20
 * PARAMETERS
 *  void
 * RETURNS
 *  S32         size of history
 *****************************************************************************/
S32 GetCategory20HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (2);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory20History
 * DESCRIPTION
 *  Gets category history buffer
 * PARAMETERS
 *  history_buffer      [IN]        History buffer
 * RETURNS
 *  U8*         history buffer
 *****************************************************************************/
U8 *GetCategory20History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        history_buffer[0] = (U8) MMI_CATEGORY20_ID;
        history_buffer[1] = (U8) 0x80;
    }
    return (history_buffer);
}


#ifdef __MMI_VIDEO_PLAYER__

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
static void MOMTVdoplyPlayFinishCallback(MDI_RESULT result)
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

        /* teminate condition */
        if (cat_momt_repeat_count == 0)
        {
            return;
        }
        /* calc remain count */
        if (cat_momt_repeat_count > 0)  /* play again */
        {
			#if 0
            if (cat_momt_repeat_count != 0xffff)
            {
                /* not infite play */
                cat_momt_repeat_count--;
            }
			#else
			cat_momt_repeat_count--;
			#endif
        }

        /* the callback function maybe already exit this category and close video file */
        /* check before play this video */
        if (cat_momt_is_video_open)
        {
            /* seek to first frame */
//			video_ret = mdi_video_ply_seek(0);
			video_ret = mdi_video_ply_seek_and_get_frame(0,wgui_layer_1);
//			COS_Sleep(300);

            if (video_ret < 0)
            {
                return;
            }

            /* start play again */
                    video_ret = mdi_video_ply_play(
                                wgui_layer_1,
                                GDI_LAYER_ENABLE_LAYER_0 | GDI_LAYER_ENABLE_LAYER_1,
                                GDI_LAYER_ENABLE_LAYER_1,
                                (U16) cat_momt_repeat_count,      /* this to recrod repeat count */
                                cat_momt_is_visual_update,  /* update to LCM or not */
                                MDI_DEVICE_SPEAKER_BOTH,    /* speaker & earphone */
                                MDI_VIDEO_LCD_ROTATE_0,     /* rotate */
                                100,                        /* 1x play speed */
                                MOMTVdoplyPlayFinishCallback);                      /* do not hook callback */

        }
    }

}
extern void mmi_vdo_get_display_rect(int *width, int *height);
#endif

/*****************************************************************************
* [MO/MT Cateogry Screen]
*
* This category is for MT/MO call screen.
*
*  (128x128)
*  **********************
*  *                    *  
*  * 200x/xx/xx         *
*  *  ****************  *
*  *  *              *  *
*  *  *              *  *
*  *  *           ------------ image or video
*  *  ****************  *
*  *                    *
*  * LSK            RSK *
*  **********************
*
*
*  (128x160) (176x220)
*  **********************
*  * ##################------ status bar
*  * Calling...         *
*  *  ****************  *
*  *  *              *  *
*  *  *              *  *
*  *  *           ----------- image or video
*  *  *              *  *
*  *  *              *  *
*  *  ****************  *
*  *     09xxxxxxxx---------- phone number or name
*  * LSK            RSK *
*  **********************
*
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowMOMTCallScreen
 * DESCRIPTION
 *  Show MO MT call cateogry.
 * PARAMETERS
 *  title_id                    [IN]        
 *  left_softkey                [IN]        Lsk string id
 *  left_softkey_icon           [IN]        Lsk image id
 *  right_softkey               [IN]        Rsk string id
 *  right_softkey_icon          [IN]        Rsk image id
 *  NotificationStringId        [IN]        Notification string, notification string
 *  NameOrNumber                [IN]        Name or number text, name or number text
 *  IP_Number                   [IN]        
 *  default_image_id            [IN]        
 *  resource_id                 [IN]        
 *  resource_filename           [IN]        Resource to be displayed (file)
 *  resource_type               [IN]        Resource type
 *  repeat_count                [IN]        Repeat count, used for video only, 0 means infiniate loop
 *  is_visaul_update            [IN]        Update video's visual
 *  is_aud_enable               [IN]        
 *  is_play_aud_when_start      [IN]        Play video at start
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
 
#ifdef __MMI_NOKIA_STYLE_N800__ 
extern color_t black_color;
#endif
static void ShowMOMTCallScreen(
                U16 title_id,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U16 NotificationStringId,
                PU8 NameOrNumber,
                PU8 IP_Number,
                U16 default_image_id,
                U16 resource_id,
                PS8 resource_filename,
                wgui_cate_momt_res_type_enum resource_type,
                U16 repeat_count,            /* video/swflash only. 0 means infinite loop */
                BOOL is_visaul_update,       /* video/swflash only. update to LCM */
                BOOL is_aud_enable,          /* video/swflash only. play video's audio */
                BOOL is_play_aud_when_start, /* video/swflash only. play audio when start */
                PU8 history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 fh, text_y;
    S32 text_width;
    S32 text_height;
    S32 text_offset_x;
    S32 text_offset_y;
    S32 image_spacing;
    S32 text_spaing;
    S32 string_width, string_height;
    S32 image_width, image_height;
    UI_string_type str;
    PU8 image_ptr;
    GDI_RESULT img_ret;
    MDI_RESULT mdi_ret = 0;

#ifdef __MMI_VIDEO_PLAYER__
    S32 resized_width = 0, resized_height = 0;
    S32 resized_offset_x = 0, resized_offset_y = 0;

#endif /* __MMI_VIDEO_PLAYER__ */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* init var */
    cat_momt_is_visual_update = is_visaul_update;
    cat_momt_is_aud_enable = is_aud_enable;
    cat_momt_is_play_aud_when_start = is_play_aud_when_start;

    cat_momt_is_video_open = FALSE;
    cat_momt_is_video_play = FALSE;
    cat_momt_is_swflash_open = FALSE;
    cat_momt_is_swflash_play = FALSE;
    cat_momt_is_aud_muted = FALSE;
    cat_momt_is_show_default = FALSE;

    /* spacing pixel from boundry */
    image_spacing = 2;
    text_spaing = 2;

    cat_momt_resource_filename = (PS8) resource_filename;
    cat_momt_resource_id = resource_id;
    cat_momt_default_image_id = default_image_id;
    cat_momt_resource_type = resource_type;

    cat_momt_repeat_count = (repeat_count == 0) ? 0xffff : repeat_count;        /* 0xff means infinte play */

    if (repeat_count != 0)
    {
        cat_momt_repeat_count--;
    }


    if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE) ||
        (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID))
    {
        /* video case */
    #ifdef __MMI_VIDEO_PLAYER__
        /* if is newly entry, do not need resume, play from start */
        if (history_buffer == NULL)
        {
            cat_momt_video_play_time = 0;
        }

        /* stop MMI LCM sleep */
        TurnOnBacklight(0);

        /* we need play video on an extra layer, so will use multi-layer */
        gdi_layer_multi_layer_enable();
        gdi_layer_get_base_handle(&wgui_base_layer);
        wgui_layer_1 = GDI_LAYER_EMPTY_HANDLE;
    #else /* __MMI_VIDEO_PLAYER__ */ 
        /* try to play video when no video function support */
        MMI_ASSERT(0);
    #endif /* __MMI_VIDEO_PLAYER__ */ 

    }
    else if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE) ||
             (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID))
    {
        /* swflash case */
    #ifdef __MMI_SWFLASH__
        /* if is newly entry, do not need resume, play from start */
        if (history_buffer == NULL)
        {
            cat_momt_swflash_play_time = 0;
        }

        /* stop MMI LCM sleep */
        TurnOnBacklight(0);

        /* we need play swflash on an extra layer, so will use multi-layer */
        gdi_layer_multi_layer_enable();
        gdi_layer_get_base_handle(&wgui_base_layer);
        wgui_layer_1 = GDI_LAYER_EMPTY_HANDLE;
    #else /* __MMI_SWFLASH__ */ 
        /* try to play swflash when no swflash function support */
        MMI_ASSERT(0);
    #endif /* __MMI_SWFLASH__ */ 

    }
    else if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_IMAGE_FILE) ||
             (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_IMAGE_ID))
    {
        /* image case */
        /* currently do nothing */
    }
    else
    {
        /* shall no enter here */
        MMI_ASSERT(0);
    }

    gdi_layer_lock_frame_buffer();

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    set_main_LCD_dt_date_hide_function(CM_screens_hide_date_time_display);
    set_main_LCD_dt_time_hide_function(CM_screens_hide_date_time_display);
    set_dt_display(DT_MT_CALL_SCREEN);

    MMI_title_icon = NULL;
    MMI_disable_title_shortcut_display = 1;

    /* draw title */
    if (title_id == 0)
    {
        status_icon = 1;
        ShowStatusIconsTitle();
    #ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
        arrange_status_icons();
    #endif 
    }
    else if (!((title_id == 0xffff)))
    {
        status_icon = 2;
        MMI_title_string = (UI_string_type) get_string(title_id);
    }
    else
    {
        status_icon = 0;
    }

    if (UI_device_height == 128)
    {
        text_y = MMI_status_bar_height + main_LCD_dt_object.date.height + main_LCD_dt_object.time.height + 2;
    }
    else
    {
        text_y = MMI_status_bar_height;
    }

   /****************************************************************************
   * Message String                                                               
   * gMMI_UI_U8_flag_1 = 0 (No "Calling..." String, for MT Call)
   * gMMI_UI_U8_flag_1 = 1 (With "Calling..." String, for MO Call)
   ****************************************************************************/

    /* support IP dialing */
    if (NameOrNumber)
    {
        memset((PS8) CM_string_buf, 0, (MAX_IP_NUMBER_LEN + MAX_DIGIT) * 2);
        gui_strcpy((UI_string_type) CM_string_buf, (UI_string_type) IP_Number);
        gui_strcat((UI_string_type) CM_string_buf, (UI_string_type) NameOrNumber);
        MMI_message_string = (UI_string_type) CM_string_buf;
    }
    else
    {
        MMI_message_string = NULL;
    }

    gui_set_font(&MMI_medium_font);
    gui_measure_string(MMI_message_string, &string_width, &string_height);

    text_offset_x = (UI_device_width >> 1) - (string_width >> 1);

    if (text_offset_x < 1)
    {
        text_offset_x = 1;
    }

    text_offset_y = UI_device_height - MMI_button_bar_height - string_height - text_spaing;
    text_width = string_width;

    if (text_width > (UI_device_width - text_spaing * 2))
    {
        text_width = UI_device_width - text_spaing * 2;
    }

    text_height = string_height;

    if (NotificationStringId != 0)
    {
        str = get_string(NotificationStringId);
        create_multiline_inputbox_set_buffer(
            str,
            gui_strlen((UI_string_type) str),
            gui_strlen((UI_string_type) str),
            0);
 
    #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
        MMI_multiline_inputbox.normal_text_color = *current_MMI_theme->CM_screen_text_color;
#elif defined(__MMI_BLACKBERRY_QWERTY__)
	MMI_multiline_inputbox.normal_text_color = gui_color(0,0,0);
    #endif
#if defined(__PROJECT_GALLITE_C01__) || defined(__MMI_USING_BLACK_THEME_BG__)
	MMI_multiline_inputbox.normal_text_color = gui_color(0,0,0);
#endif	
  
        MMI_multiline_inputbox.flags |=
            UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND |
            UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY;

        if (UI_device_height != 128)
        {
            MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY;
        }
		#ifdef __MMI_NOKIA_STYLE_N800__
	MMI_multiline_inputbox.normal_text_color = black_color;
	#endif
        MMI_multiline_inputbox.text_y = 1;
        show_multiline_inputbox_no_draw();
        fh = get_multiline_inputbox_line_height();

        resize_multiline_inputbox(UI_device_width, fh + MULTILINE_INPUTBOX_HEIGHT_PAD);
        gMMI_UI_U8_flag_1 = 1;  /* With "Calling" string,MO calling */
    }
    else
    {
        fh = Get_FontHeight(*UI_font, (U8) gCurrLangIndex);
        gMMI_UI_U8_flag_1 = 0;  /* Without "Calling" string, MT calling */
    }

   /****************************************************************************
   *  Get Image, Video or SWFlash info
   *****************************************************************************/

    /* prepare image, video or swflash */
    switch (cat_momt_resource_type)
    {
        case WGUI_CATE_MOMT_RES_TYPE_IMAGE_ID:
        {
            image_ptr = get_image(cat_momt_resource_id);
            gdi_image_get_dimension(image_ptr, &image_width, &image_height);
        }
            break;

        case WGUI_CATE_MOMT_RES_TYPE_IMAGE_FILE:
        {
            img_ret = gdi_image_get_dimension_file((PS8) resource_filename, &image_width, &image_height);

            if (img_ret < 0)
            {
                /* faild, need to play default image */
                cat_momt_is_show_default = TRUE;
                img_ret = gdi_image_get_dimension_id(cat_momt_default_image_id, &image_width, &image_height);
                MMI_ASSERT(img_ret >= 0);
            }
        }
            break;

    #ifdef __MMI_VIDEO_PLAYER__
        case WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE:
        {
            /* open with audio is on */
            mdi_ret = mdi_video_ply_open_clip_file(cat_momt_resource_filename, &wgui_video_info, cat_momt_is_aud_enable);

            mmi_trace(g_sw_CC,"MMI_Call: Func: %s mdi_ret:%d", __FUNCTION__, mdi_ret);
            cat_momt_is_video_open = (mdi_ret >= 0) ? TRUE : FALSE;

            if (mdi_ret >= 0)
            {
                mdi_audio_set_volume(MDI_VOLUME_MEDIA, GetRingVolumeLevel());

#if 0
                image_width = wgui_video_info.width;
                image_height = wgui_video_info.height;
#endif /* Guoj delete. It's not used now at 2010-3-25 */
				mmi_vdo_get_display_rect(&image_width, &image_height);

	            mmi_trace(g_sw_CC,"MMI_Call Func: %s image_width:%d image_height:%d", __FUNCTION__, image_width, image_height);
				wgui_video_info.width = image_width;
				wgui_video_info.height = image_height;
				
            }
            else
            {
                /* faild, need to play default image */
                cat_momt_is_show_default = TRUE;
                img_ret = gdi_image_get_dimension_id(cat_momt_default_image_id, &image_width, &image_height);
                MMI_ASSERT(img_ret >= 0);
            }

        }
            break;

        case WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID:
        {
            mdi_ret = mdi_video_ply_open_clip_id(cat_momt_resource_id, &wgui_video_info, cat_momt_is_aud_enable);

            cat_momt_is_video_open = (mdi_ret >= 0) ? TRUE : FALSE;
            if (mdi_ret >= 0)
            {
                mdi_audio_set_volume(MDI_VOLUME_MEDIA, GetRingVolumeLevel());

                image_width = wgui_video_info.width;
                image_height = wgui_video_info.height;
            }
            else
            {
                /* faild, need to play default image */
                cat_momt_is_show_default = TRUE;
                img_ret = gdi_image_get_dimension_id(cat_momt_default_image_id, &image_width, &image_height);
                MMI_ASSERT(img_ret >= 0);
            }
        }
            break;
    #endif /* __MMI_VIDEO_PLAYER__ */ 

    #ifdef __MMI_SWFLASH__
        case WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE:
        {
            if (cat_momt_is_aud_enable)
            {            
                mdi_ret = mdi_swflash_open_file(cat_momt_resource_filename, CIM_PLAYERMODE, &wgui_swflash_info);
            }
            else
            {
                mdi_ret = mdi_swflash_open_file(cat_momt_resource_filename, CIM_SCREENMODE, &wgui_swflash_info);
            }
            
            cat_momt_is_swflash_open = (mdi_ret >= 0) ? TRUE : FALSE;

            if (mdi_ret >= 0)
            {
                mdi_audio_set_volume(MDI_VOLUME_MEDIA, GetRingVolumeLevel());

                image_width = wgui_swflash_info.width;
                image_height = wgui_swflash_info.height;
            }
            else
            {
                /* faild, need to play default image */
                cat_momt_is_show_default = TRUE;
                img_ret = gdi_image_get_dimension_id(cat_momt_default_image_id, &image_width, &image_height);
                MMI_ASSERT(img_ret >= 0);
            }

        }
            break;

        case WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID:
        {
            if (cat_momt_is_aud_enable)
            {
                mdi_ret = mdi_swflash_open_id(cat_momt_resource_id, CIM_PLAYERMODE, &wgui_swflash_info);
            }
            else
            {
                mdi_ret = mdi_swflash_open_id(cat_momt_resource_id, CIM_SCREENMODE, &wgui_swflash_info);
            }

            cat_momt_is_swflash_open = (mdi_ret >= 0) ? TRUE : FALSE;

            if (mdi_ret >= 0)
            {
                mdi_audio_set_volume(MDI_VOLUME_MEDIA, GetRingVolumeLevel());

                image_width = wgui_swflash_info.width;
                image_height = wgui_swflash_info.height;
            }
            else
            {
                /* faild, need to play default image */
                cat_momt_is_show_default = TRUE;
                img_ret = gdi_image_get_dimension_id(cat_momt_default_image_id, &image_width, &image_height);
                MMI_ASSERT(img_ret >= 0);
            }
        }
            break;
    #endif /* __MMI_SWFLASH__ */ 

        default:
        {
            /* wrong resouce type */
            MMI_ASSERT(0);
        }
    }

   /********************************************************************************
   *  Calculate drawable region for Image and Video
   ********************************************************************************/
    SetMOMTCallImageClip(image_width, image_height);

   /********************************************************************************
   *  Create Video or SWFlash Layer 
   ********************************************************************************/
    if (!cat_momt_is_show_default)
    {
    #ifdef __MMI_VIDEO_PLAYER__
        /* video is successfully opened, create video playback layer */
        if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE) ||
            (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID))
        {
            if (mdi_ret >= 0)
            {
                /*Guojian Modify Start For incomingcall video #9287 Ver: GreenStone  on 2008-7-31 17:41 */
                if(CM_need_change_clip)
                /*Guojian Modify End  For incomingcall video #9287 Ver: GreenStone  on 2008-7-31 17:41 */
                {
                    /* video larger than drawable region, need resize */

                    /* calc draw size */
                    gdi_image_util_fit_bbox(
                        wgui_image_clip_x2 - wgui_image_clip_x1 + 1,
                        wgui_image_clip_y2 - wgui_image_clip_y1 + 1,
                        wgui_video_info.width,
                        wgui_video_info.height,
                        &resized_offset_x,
                        &resized_offset_y,
                        &resized_width,
                        &resized_height);
                }
                else
                {
                    /* video smaller than region - align at center */
                    mmi_trace(g_sw_CC,"Func: %s wgui_image_clip_x2:%d, wgui_image_clip_x1:%d wgui_video_info.width:%d", __FUNCTION__, wgui_image_clip_x2, wgui_image_clip_x1, wgui_video_info.width);
                    mmi_trace(g_sw_CC,"Func: %s wgui_image_clip_y2:%d, wgui_image_clip_y1:%d wgui_video_info.height:%d", __FUNCTION__, wgui_image_clip_y2, wgui_image_clip_y1, wgui_video_info.height);
                    resized_offset_x = ((wgui_image_clip_x2 - wgui_image_clip_x1 + 1) - wgui_video_info.width) >> 1;
                #ifdef __MMI_MAINLCD_128X128__
                    resized_offset_y = ((wgui_image_clip_y2 - wgui_image_clip_y1 + 1) - wgui_video_info.height) >> 1;
                #else 
                    resized_offset_y = ((wgui_image_clip_y2 - wgui_image_clip_y1 + 1) - wgui_video_info.height);
                #endif 

                    resized_width = wgui_video_info.width;
                    resized_height = wgui_video_info.height;
                }

                MMI_ASSERT(wgui_layer_1 == GDI_LAYER_EMPTY_HANDLE);

                /* create video playback layer */
                gdi_layer_create(
                    wgui_image_clip_x1 + resized_offset_x,
                    wgui_image_clip_y1 + resized_offset_y,
                    resized_width,
                    resized_height,
                    &wgui_layer_1);
		mdi_video_ply_fullscreen_display_set(resized_width,resized_height,wgui_image_clip_x1 + resized_offset_x,wgui_image_clip_y1 + resized_offset_y,0);
            }
        }
    #endif /* __MMI_VIDEO_PLAYER__ */ 

    #ifdef __MMI_SWFLASH__
        /* SWFlash is successfully opened, create video playback layer */
        if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE) ||
            (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID))
        {
            if (mdi_ret >= 0)
            {
                if (CM_need_change_clip)
                {
                    /* swflash larger than drawable region, need resize */

                    /* calc draw size */
                    gdi_image_util_fit_bbox(
                        wgui_image_clip_x2 - wgui_image_clip_x1 + 1,
                        wgui_image_clip_y2 - wgui_image_clip_y1 + 1,
                        wgui_swflash_info.width,
                        wgui_swflash_info.height,
                        &resized_offset_x,
                        &resized_offset_y,
                        &resized_width,
                        &resized_height);
                }
                else
                {
                    /* video smaller than region - align at center */
                    resized_offset_x = ((wgui_image_clip_x2 - wgui_image_clip_x1 + 1) - wgui_swflash_info.width) >> 1;
                #ifdef __MMI_MAINLCD_128X128__
                    resized_offset_y = ((wgui_image_clip_y2 - wgui_image_clip_y1 + 1) - wgui_swflash_info.height) >> 1;
                #else 
                    resized_offset_y = ((wgui_image_clip_y2 - wgui_image_clip_y1 + 1) - wgui_swflash_info.height);
                #endif 

                    resized_width = wgui_swflash_info.width;
                    resized_height = wgui_swflash_info.height;
                }

                MMI_ASSERT(wgui_layer_1 == GDI_LAYER_EMPTY_HANDLE);

                /* create video playback layer */
                gdi_layer_create(
                    wgui_image_clip_x1 + resized_offset_x,
                    wgui_image_clip_y1 + resized_offset_y,
                    resized_width,
                    resized_height,
                    &wgui_layer_1);
            }
        }
    #endif /* __MMI_SWFLASH__ */ /* __MMI_VIDEO_PLAYER__ */
    }

   /********************************************************************************
   *  Create scrolling text
   ********************************************************************************/
    if (UI_device_height == 128)
    {
        gui_create_scrolling_text(
            &CM_scrolling_text,
            (UI_device_width - text_width) >> 1,
            CM_y,
            text_width,
            text_height,
            MMI_message_string,
            CM_scrolling_text_timer_callback, CM_draw_scrolling_text_background,
        #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
            *current_MMI_theme->CM_screen_text_color,
        #else 
            gui_color(0, 0, 0),
        #endif 
            gui_color(0,0,0));
    }
    else
    {
        gui_create_scrolling_text(
            &CM_scrolling_text, 2 /* text_offset_x */ , CM_y + 1,
            text_width, text_height,
            MMI_message_string,
            CM_scrolling_text_timer_callback, CM_draw_scrolling_text_background,
        #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
            *current_MMI_theme->CM_screen_text_color,
        #else 
            gui_color(0, 0, 0),
        #endif 
            gui_color(0, 0,0));
    }

    CM_scrolling_text.flags &= ~UI_SCROLLING_TEXT_BORDERED_TEXT;

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitMOMTCallScreen;
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY18_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawMOMTCallScreenControllArea);
    dm_redraw_category_screen();

#ifdef __MMI_VIDEO_PLAYER__
    if (!cat_momt_is_show_default)
    {
        /* start play video */
        if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID) ||
            (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE))
        {
            if (cat_momt_is_video_open) /* open file successfully */
            {
				//video_ret = mdi_video_ply_seek(0);
				mdi_ret = mdi_video_ply_seek_and_get_frame(0, wgui_layer_1);
				mmi_trace(g_sw_CC,"mdi_video_ply_seek_and_get_frame+++++Func: %s mdi_ret=%d", __FUNCTION__, mdi_ret);
					
   //                         COS_Sleep( 500);
                if (mdi_ret >= 0)
                {
                   gdi_layer_set_blt_layer(wgui_base_layer, wgui_layer_1, 0, 0);

                    mdi_ret = mdi_video_ply_play(
                                wgui_layer_1,
                                GDI_LAYER_ENABLE_LAYER_0 | GDI_LAYER_ENABLE_LAYER_1,
                                GDI_LAYER_ENABLE_LAYER_1,
                                (U16) cat_momt_repeat_count,      /* this to recrod repeat count */
                                cat_momt_is_visual_update,  /* update to LCM or not */
                                MDI_DEVICE_SPEAKER_BOTH,    /* speaker & earphone */
                                MDI_VIDEO_LCD_ROTATE_0,     /* rotate */
                                100,                        /* 1x play speed */
                                MOMTVdoplyPlayFinishCallback);                      /* do not hook callback */
                }

                cat_momt_is_video_play = (mdi_ret >= 0) ? TRUE : FALSE;

                if (mdi_ret >= 0)
                {
                    /* if aud is on but not play when start, mute it */
                    if (cat_momt_is_aud_enable && !cat_momt_is_play_aud_when_start)
                    {
                        mdi_audio_set_mute(MDI_VOLUME_MEDIA, TRUE);
                        cat_momt_is_aud_muted = TRUE;
                    }
                }

                /* play failed */
                if (mdi_ret < 0)
                {
                    /* faild, need to play default image */
                    cat_momt_is_show_default = TRUE;
                    img_ret = gdi_image_get_dimension_id(cat_momt_default_image_id, &image_width, &image_height);
                    MMI_ASSERT(img_ret >= 0);
                }
            }
        }
        if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID) ||
            (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE))
        {
            gdi_layer_set_blt_layer(wgui_base_layer, wgui_layer_1, 0, 0);
        }
     }
#endif /* __MMI_VIDEO_PLAYER__ */ 


#ifdef __MMI_SWFLASH__
    if (!cat_momt_is_show_default)
    {
        if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID) ||
            (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE))
        {
            gdi_layer_set_blt_layer(wgui_base_layer, wgui_layer_1, 0, 0);
        }
     }
#endif /* __MMI_SWFLASH__ */
}


/*****************************************************************************
 * FUNCTION
 *  MOMTHideAnimation
 * DESCRIPTION
 *  Stop and grey MOMT Call image files
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MOMTHideAnimation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MOMT_animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(MOMT_animation_handle);
        MOMT_animation_handle = GDI_ERROR_HANDLE;
    }
    gui_cross_hatch_fill_rectangle(
        wgui_image_clip_x1,
        wgui_image_clip_y1,
        wgui_image_clip_x2,
        wgui_image_clip_y2,
        UI_COLOR_BLACK);
}


/*****************************************************************************
 * FUNCTION
 *  RedrawMOMTCallScreen
 * DESCRIPTION
 *  Redraw MOMT Call Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void RedrawMOMTCallScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_filled_area *bg_filler;
    S32 image_width;
    S32 image_height;
    S32 image_offset_x;
    S32 image_offset_y;
    S32 string_width;
    S32 string_height;
    S32 string_offset_x;
    S32 resized_width, resized_height;
    GDI_RESULT image_ret;

#if defined(__MMI_VIDEO_PLAYER__) || defined(__MMI_SWFLASH__)
    MDI_RESULT mdi_ret;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    UI_set_main_LCD_graphics_context();
    gui_hide_animations();

    /* fill with theme background color */
    bg_filler = current_MMI_theme->CM_screen_background_filler;

    gdi_layer_reset_clip();
    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, bg_filler);

    if (status_icon == 1)
    {
#if defined(__MMI_MAINLCD_220X176__)
		MMI_title_string = NULL;
#endif
        show_title_status_icon();
        show_status_icons();
    }
    else if (status_icon == 2)
    {
        draw_title();
        show_title_status_icon();
    }

#ifndef __MMI_CM_SCREEN_HIDE_DATE_TIME__
    if (UI_device_height == 128 && CM_Screen_show_date_time_bar == 1)
    {
        CM_screens_draw_date_time_bar();
        show_main_LCD_dt_display();
    }
#endif /* __MMI_CM_SCREEN_HIDE_DATE_TIME__ */ 

    if (!cat_momt_is_show_default)
    {

        if (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_IMAGE_ID) /* iamge from id */
        {
            gdi_layer_set_clip(wgui_image_clip_x1, wgui_image_clip_y1, wgui_image_clip_x2, wgui_image_clip_y2);
            gdi_image_draw_animation(
                wgui_image_clip_x1,
                wgui_image_clip_y1,
                (U8*) GetImage(cat_momt_resource_id),
                &MOMT_animation_handle);

            if (isCallDisconnecting)
            {
                MOMTHideAnimation();
            }
        }
        else if (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_IMAGE_FILE)  /* image from file */
        {
            image_ret = gdi_image_get_dimension_file((PS8) cat_momt_resource_filename, &image_width, &image_height);

            if (image_ret >= 0)
            {
                if (CM_need_change_clip)
                {
                    wgui_image_clip_x1 = 0;
                    wgui_image_clip_x2 = UI_device_width - 1;
                    gdi_layer_set_clip(wgui_image_clip_x1, wgui_image_clip_y1, wgui_image_clip_x2, wgui_image_clip_y2);

                    /* image larger than drawable region */
                    gdi_image_util_fit_bbox(
                        UI_device_width,
                        wgui_image_clip_y2 - wgui_image_clip_y1 + 1,
                        image_width,
                        image_height,
                        &image_offset_x,
                        &image_offset_y,
                        &resized_width,
                        &resized_height);

                    image_ret = gdi_anim_draw_file_resized(
                                    wgui_image_clip_x1 + image_offset_x,
                                    wgui_image_clip_y1 + image_offset_y,
                                    resized_width,
                                    resized_height,
                                    (PS8) cat_momt_resource_filename,
                                    &MOMT_animation_handle);

                    if (isCallDisconnecting)
                    {
                        MOMTHideAnimation();
                    }

                }
                else
                {
                    gdi_layer_set_clip(wgui_image_clip_x1, wgui_image_clip_y1, wgui_image_clip_x2, wgui_image_clip_y2);

                    image_ret = gdi_image_draw_animation_file(
                                    wgui_image_clip_x1 + ((wgui_image_clip_x2 - wgui_image_clip_x1 + 1 - image_width) >> 2),
                                    wgui_image_clip_y1 + ((wgui_image_clip_y2 - wgui_image_clip_y1 + 1 - image_height) >> 2),
                                    (PS8) cat_momt_resource_filename,
                                    &MOMT_animation_handle);
                }
            }

            if (image_ret < 0)
            {
                /* faild, need to play default image */
                cat_momt_is_show_default = TRUE;
            }

            if (isCallDisconnecting)
            {
                MOMTHideAnimation();
            }
        }
        else if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE) ||
                 (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID))
        {
        #ifdef __MMI_VIDEO_PLAYER__
            if (cat_momt_is_video_open) /* open file successfully */
            {
                /* seek to first frame */
                mdi_ret = mdi_video_ply_seek_and_get_frame(cat_momt_video_play_time, wgui_layer_1);

                if (mdi_ret < 0)
                {
                    /* faild, need to play default image */
                    cat_momt_is_show_default = TRUE;
                }
            }
            else
            {
                MMI_ASSERT(0);
            }
        #endif /* __MMI_VIDEO_PLAYER__ */ 
        }
        else if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE) ||
                 (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID))
        {
        #ifdef __MMI_SWFLASH__
            if (cat_momt_is_swflash_open)    /* open file successfully */
            {
                gdi_layer_set_blt_layer(wgui_base_layer, wgui_layer_1, 0, 0);                
                
                mdi_ret = mdi_swflash_play(
                            wgui_layer_1,                       /* layer */
                            0,                                  /* repeat_count */
                            MDI_DEVICE_SPEAKER_BOTH,            /* audio_path */
                            MOMTPlaySWFlashResultCallback);     /* play_result_callback */

                cat_momt_is_swflash_play = (mdi_ret >= 0) ? TRUE : FALSE;

                if (mdi_ret >= 0)
                {
                    /* if aud is on but not play when start, mute it */
                    if (cat_momt_is_aud_enable && !cat_momt_is_play_aud_when_start)
                    {
                        mdi_audio_set_mute(MDI_VOLUME_MEDIA, TRUE);
                        cat_momt_is_aud_muted = TRUE;
                    }
                }

                if (mdi_ret < 0)
                {
                    /* faild, need to play default image */
                    cat_momt_is_show_default = TRUE;
                }
            }
            else
            {
                MMI_ASSERT(0);
            }

        #endif /* __MMI_SWFLASH__ */ 
        }
        else
        {
            MMI_ASSERT(0);
        }
    }   /* not show default image */

    /* show default image if needed */
    if (cat_momt_is_show_default)
    {
        gdi_image_get_dimension_id(cat_momt_default_image_id, &image_width, &image_height);
       
        gui_draw_filled_area(wgui_image_clip_x1, wgui_image_clip_y1, wgui_image_clip_x2, wgui_image_clip_y2, bg_filler);//102306 momt call

        SetMOMTCallImageClip(image_width, image_height);
        gdi_layer_set_clip(wgui_image_clip_x1, wgui_image_clip_y1, wgui_image_clip_x2, wgui_image_clip_y2);

        gdi_image_draw_animation(
            wgui_image_clip_x1,
            wgui_image_clip_y1,
            (U8*) GetImage(cat_momt_default_image_id),
            &MOMT_animation_handle);

        if (isCallDisconnecting)
        {
            MOMTHideAnimation();
        }
    }

    if (gMMI_UI_U8_flag_1 == 1)
    {
        show_multiline_inputbox();
    }

    if (MMI_message_string)
    {
        gui_set_font(current_MMI_theme->title_text_font);
        gui_measure_string(MMI_message_string, &string_width, &string_height);

        if (string_width < UI_device_width)
        {
            string_offset_x = (UI_device_width >> 1) - (string_width >> 1);
        }

        gui_show_scrolling_text(&CM_scrolling_text);
    }

    show_softkey_background();
    show_left_softkey();
    show_right_softkey();

    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  SetMOMTCallImageClip
 * DESCRIPTION
 *  Set image clips for  MO MT call cateogryscreen
 * PARAMETERS
 *  image_width         [IN]        Image width
 *  image_height        [IN]        Image height
 * RETURNS
 *  void
 *****************************************************************************/
static void SetMOMTCallImageClip(S32 image_width, S32 image_height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 font_height;
    S32 text_y;
    S32 string_width;
    S32 string_height;
    S32 y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CM_need_change_clip = FALSE;

    if (gMMI_UI_U8_flag_1)
    {
        font_height = get_multiline_inputbox_line_height();
    }
    else
    {
        font_height = Get_FontHeight(*UI_font, (U8) gCurrLangIndex);
    }

    gui_set_font(&MMI_medium_font);
    gui_measure_string(MMI_message_string, &string_width, &string_height);

    if (UI_device_height == 128)
    {
        text_y = MMI_status_bar_height + main_LCD_dt_object.date.height + main_LCD_dt_object.time.height + 2;

        y = UI_device_height - MMI_button_bar_height - font_height - string_height - 1;
        move_multiline_inputbox(0, y);

        CM_y = UI_device_height - MMI_button_bar_height - string_height;

        /* calcaute drawable region for image and video */
        wgui_image_clip_x1 = 0;
        wgui_image_clip_x2 = UI_device_width - 1;
        wgui_image_clip_y1 = text_y + 2;

        if (gMMI_UI_U8_flag_1 == 1) /* with Calling string, MO */
        {
            wgui_image_clip_y2 = y - 1;
        }
        else    /* without Calling string, MT */
        {
            wgui_image_clip_y2 = CM_y - 1 - 2;
        }

        if (image_width <= UI_device_width)
        {
            wgui_image_clip_x1 = (UI_device_width - image_width) >> 1;
            wgui_image_clip_x2 = wgui_image_clip_x1 + image_width - 1;
        }
        else
        {
            wgui_image_clip_x1 = 0; /* indicate the input image is wider than displayable area */
            wgui_image_clip_x2 = UI_device_width - 1;
            CM_need_change_clip = TRUE;
        }

        if (wgui_image_clip_y2 - wgui_image_clip_y1 + 1 < image_height)
        {
            /* no enough space to display image */
            CM_Screen_show_date_time_bar = 0;
            wgui_image_clip_y1 = MMI_status_bar_height + 2;
            deactive_main_lcd_update_date_time();
            if (wgui_image_clip_y2 - wgui_image_clip_y1 + 1 < image_height)
            {
                CM_need_change_clip = TRUE;
            }
        }
        else
        {
            CM_Screen_show_date_time_bar = 1;
        }

    }
    else    /* UI_device_height == 160, 220 */
    {
        text_y = MMI_status_bar_height;

        deactive_main_lcd_update_date_time();
        move_multiline_inputbox(0, MMI_status_bar_height);

        CM_y = MMI_status_bar_height + font_height;

        /* calcaute drawable region for image and video */
        wgui_image_clip_x1 = 0;
        wgui_image_clip_x2 = UI_device_width - 1;

        mmi_trace(g_sw_CC,"Func: %s image_height:%d string_height:%d, MMI_button_bar_height:%d", __FUNCTION__, image_height , string_height, MMI_button_bar_height);
        if (image_height < UI_device_height - MMI_button_bar_height - (CM_y + 1 + string_height))
        {
 //           wgui_image_clip_y1 = UI_device_height - MMI_button_bar_height - image_height;
            wgui_image_clip_y1 = CM_y + 1 + string_height + 1 + (UI_device_height - MMI_button_bar_height - (CM_y + 1 + string_height) - image_height)/2;
            wgui_image_clip_y2 = wgui_image_clip_y1 + image_height - 1;
        }
        else
        {
            wgui_image_clip_y1 = CM_y + 1 + string_height + 1;  /* one pixel for gap */
            /* indicate the input image is higher than displayable area */
            wgui_image_clip_y2 = UI_device_height - MMI_button_bar_height - 1;
            CM_need_change_clip = TRUE;
        }
        if (image_width <= UI_device_width)
        {
            wgui_image_clip_x1 = (UI_device_width - image_width) >> 1;
            wgui_image_clip_x2 = wgui_image_clip_x1 + image_width - 1;
        }
        else
        {
            /* indicate the input image is wider than displayable area */
            wgui_image_clip_x1 = 0;
            wgui_image_clip_x2 = UI_device_width - 1;
            CM_need_change_clip = TRUE;
        }

    }
}


/*****************************************************************************
 * FUNCTION
 *  DrawMOMTCallScreenControllArea
 * DESCRIPTION
 *  draw MOMT screen control area
 * PARAMETERS
 *  coordinate      [IN]        Coordinae
 * RETURNS
 *  void
 *****************************************************************************/
void DrawMOMTCallScreenControllArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawMOMTCallScreen();
}


/*****************************************************************************
 * FUNCTION
 *  MOMTPlayVideoResultCallback
 * DESCRIPTION
 *  play video result callback function
 * PARAMETERS
 *  result      [IN]        Play vido result
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_VIDEO_PLAYER__
#if 0
static void MOMTPlayVideoResultCallback(MDI_RESULT result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* do nothing */

}
#endif
#endif /* __MMI_VIDEO_PLAYER__ */ 


/*****************************************************************************
 * FUNCTION
 *  MOMTPlaySWFlashResultCallback
 * DESCRIPTION
 *  play swflash result callback function
 * PARAMETERS
 *  result      [IN]        Play vido result
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_SWFLASH__
static void MOMTPlaySWFlashResultCallback(MDI_RESULT result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* do nothing */
}
#endif /* __MMI_SWFLASH__ */ 


/*****************************************************************************
 * FUNCTION
 *  ExitMOMTCallScreen
 * DESCRIPTION
 *  Exit MO MT call screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void ExitMOMTCallScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_set_main_LCD_graphics_context();
    close_status_icons();
    gui_hide_animations();

    MOMT_animation_handle = GDI_ERROR_HANDLE;
    isCallDisconnecting = MMI_FALSE;

    gui_cancel_timer(CM_scrolling_text_timer_callback);
    gui_cancel_timer(handle_CM_text_scroll);
    close_main_LCD_dt_display();
    reset_softkeys();
    reset_multitaps();
    reset_multiline_inputbox();
    enactive_main_lcd_update_date_time();

    if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE) ||
        (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID))
    {
    #ifdef __MMI_VIDEO_PLAYER__

        if (cat_momt_is_video_open)
        {
            /* get current play time */
            mdi_video_ply_get_cur_play_time((U32 *)&cat_momt_video_play_time);

            /* stop video playing */
            if (cat_momt_is_video_play)
            {
                mdi_video_ply_stop();
                cat_momt_is_video_play = FALSE;
                mdi_video_ply_seek_and_get_frame(cat_momt_video_play_time, wgui_layer_1);
            }

            if (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE)
            {
                mdi_video_ply_close_clip_file();
            }
            else
            {
                mdi_video_ply_close_clip_id();
            }
		  	/* enable key pad tone */
		  	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
            /* unmute */
            if (cat_momt_is_aud_muted)
            {
                cat_momt_is_aud_muted = FALSE;
                mdi_audio_set_mute(MDI_VOLUME_MEDIA, FALSE);
            }

            cat_momt_is_video_open = FALSE;
        }

        if (wgui_layer_1 != GDI_LAYER_EMPTY_HANDLE)
        {
            /* flatten layers */
            gdi_layer_flatten_previous_to_base();

            gdi_layer_free(wgui_layer_1);
            wgui_layer_1 = GDI_LAYER_EMPTY_HANDLE;
        }

        gdi_layer_set_blt_layer(wgui_base_layer, 0, 0, 0);
        gdi_layer_multi_layer_disable();

        /* allows MMI LCD sleep */
        TurnOffBacklight();
    #else /* __MMI_VIDEO_PLAYER__ */ 
        MMI_ASSERT(0);  /* try to play video when no video function support */
    #endif /* __MMI_VIDEO_PLAYER__ */ 
    }
    else if ((cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE) ||
             (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID))
    {
    #ifdef __MMI_SWFLASH__
        if (cat_momt_is_swflash_open)
        {
            /* stop video playing */
            if (cat_momt_is_swflash_play)
            {
                mdi_swflash_stop();
                cat_momt_is_video_play = FALSE;
            }

            if (cat_momt_resource_type == WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE)
            {
                mdi_swflash_close_file();
            }
            else
            {
                mdi_swflash_close_id();
            }

            /* unmute */
            if (cat_momt_is_aud_muted)
            {
                cat_momt_is_aud_muted = FALSE;
                mdi_audio_set_mute(MDI_VOLUME_MEDIA, FALSE);
            }

            cat_momt_is_video_open = FALSE;
        }

        if (wgui_layer_1 != GDI_LAYER_EMPTY_HANDLE)
        {
            /* flatten layers */
            gdi_layer_flatten_previous_to_base();

            gdi_layer_free(wgui_layer_1);
            wgui_layer_1 = GDI_LAYER_EMPTY_HANDLE;
        }

        gdi_layer_set_blt_layer(wgui_base_layer, 0, 0, 0);
        gdi_layer_multi_layer_disable();

        /* allows MMI LCD sleep */
        TurnOffBacklight();
    #else /* __MMI_SWFLASH__ */ 
        MMI_ASSERT(0);  /* try to play video when no video function support */
    #endif /* __MMI_SWFLASH__ */ /* __MMI_VIDEO_PLAYER__ */
    }

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  Cat403ListHighliteHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Cat403ListHighliteHandler(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_list_highlight_handler(index);
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory403TabSelectCallback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  select_callback     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory403TabSelectCallback(void (*select_callback) (int index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_set_horizontal_tab_bar_select_callback(select_callback);
}


/*****************************************************************************
 * FUNCTION
 *  Category403BlinkTab
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tab     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category403BlinkTab(S8 tab)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_horizontal_tab_bar_start_blinking(tab);
}


/*****************************************************************************
 * FUNCTION
 *  Category403UnBlinkTab
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tab     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category403UnBlinkTab(S8 tab)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_horizontal_tab_bar_stop_blinking(tab);
}


/*****************************************************************************
 * FUNCTION
 *  UpdateCategory403TabBar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title               [IN]        
 *  n_tabs              [IN]        
 *  highlighted_tab     [IN]        
 *  tab_items           [?]         [?]
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateCategory403TabBar(UI_string_type title, S8 n_tabs, S8 highlighted_tab, tab_bar_item_type *tab_items)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    wgui_close_horizontal_tab_bar();

    /* Setup horizontal tab bar */
    for (i = 0; i < n_tabs; i++)
    {
        MMI_tab_bar_items[i] = tab_items[i];
    }
    wgui_create_horizontal_tab_bar(TRUE, title, n_tabs, highlighted_tab, MMI_TRUE);

    gdi_layer_unlock_frame_buffer();

    wgui_show_horizontal_tab_bar();
}

void ExitCategory403Screen(void);


/*****************************************************************************
 * FUNCTION
 *  ShowCategory403Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [?]         [?]
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  n_tabs                  [IN]        
 *  highlighted_tab         [IN]        
 *  tab_items               [?]         [?]
 *  CallDuration            [?]         
 *  list_of_items           [IN]        
 *  list_of_icons           [?]         [?]
 *  number_of_items         [IN]        
 *  highlighted_item        [IN]        
 *  history_buffer          [?]         [?]
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory403Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S8 n_tabs,
        S8 highlighted_tab,
        tab_bar_item_type *tab_items,
        MYTIME *CallDuration,
        U8 **list_of_items,
        U16 *list_of_icons,
        S32 number_of_items,
        U16 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i = 0;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    MMI_title_string = NULL;
    MMI_title_icon = NULL;

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    for (i = 0; i < n_tabs; i++)
    {
        MMI_tab_bar_items[i] = tab_items[i];
    }

    wgui_create_horizontal_tab_bar(TRUE, (UI_string_type) title, n_tabs, highlighted_tab, MMI_FALSE);

    set_dt_duration(CallDuration);
    set_dt_display(DT_TABBED_CALL_SCREEN);

    /* List */
    create_fixed_icontext_menuitems();
    associate_fixed_icontext_list();
    MMI_current_menu_type = LIST_MENU;
    register_fixed_list_keys();
    resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);

     
    register_fixed_list_highlight_handler(Cat403ListHighliteHandler);
    set_fixed_icontext_positions(2, 0, 0, 0);

    for (i = 0; i < number_of_items; i++)
    {
        if (list_of_icons == NULL)
        {
            add_fixed_icontext_item((UI_string_type) list_of_items[i], 0);
        }
        else
        {
            add_fixed_icontext_item((UI_string_type) list_of_items[i], wgui_get_list_menu_icon(i, list_of_icons[i]));
        }
        wgui_pop_up_description_strings[i].text_strings[0] = NULL;
    }
    h_flag = set_list_menu_category_history(MMI_CATEGORY403_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory403Screen;
    /* dm_register_category_controlled_callback(category403_draw_controlled_area); */
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY403_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory403Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory403Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    close_main_LCD_dt_display();
    close_status_icons();
    reset_softkeys();
    reset_fixed_list();
    reset_pop_up_descriptions();
    wgui_close_horizontal_tab_bar();
}

UI_filled_area cat404_fibox={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};
UI_filled_area *cat404_fibox_backup = NULL;

/*****************************************************************************
 * FUNCTION
 *  ExitCategory404Screen
 * DESCRIPTION
 *  Exits the VoIP dialing screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory404Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_MMI_current_input_type();
    wgui_close_inputbox();
    MMI_multiline_inputbox.normal_filler = cat404_fibox_backup;
    reset_softkeys();
    reset_multitaps();
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    wgui_reset_wallpaper_on_bottom();
#endif
    wgui_unset_AP_required_input_mode_set_flag();
}


/*****************************************************************************
 * FUNCTION
 *  category404_hide_multitap
 * DESCRIPTION
 *  Hide function for multitap
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category404_hide_multitap(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_push_clip();
    gdi_layer_reset_clip();
    gdi_draw_solid_rect(
        0,
        MMI_multitap_y,
        UI_device_width - 1,
        MMI_multitap_y + MMI_multitap_height - 1,
        GDI_COLOR_TRANSPARENT);
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, MMI_multitap_y, UI_device_width - 1, MMI_multitap_y + MMI_multitap_height + 1);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory404Screen
 * DESCRIPTION
 *  Displays the category404 screen(VoIP Dialing Screen)
 * PARAMETERS
 *  title                   [?]         
 *  title_icon              [IN]        
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  bg_image                [IN]        Background image id
 *  Buffer                  [IN]        Message string
 *  BufferLength            [IN]        Length of buffer
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory404Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 bg_image,
        U8 *Buffer,
        U32 BufferLength,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S16 required_input_mode[]={
            INPUT_MODE_MULTITAP_UPPERCASE_ABC,
            INPUT_MODE_MULTITAP_LOWERCASE_ABC,
            INPUT_MODE_123,
            -1};
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* UI_UNUSED_PARAMETER(history_buffer); */

    gdi_layer_lock_frame_buffer();

    entry_full_screen();

    /* setup bg */
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    wgui_set_wallpaper_on_bottom(MMI_TRUE);
#endif 
    dm_add_image(get_image(bg_image), NULL, NULL);

    /* setup title */
    MMI_title_string = NULL;
    MMI_title_icon = NULL;

    /* menu shortcut */
    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;

    /* setup key handlers */
    clear_key_handlers();
    change_right_softkey(right_softkey, right_softkey_icon);
    change_left_softkey(left_softkey, left_softkey_icon);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_hide_softkey(cat33_hide_lsk, MMI_LEFT_SOFTKEY);
    register_hide_softkey(cat33_hide_rsk, MMI_RIGHT_SOFTKEY);
#ifdef __MMI_WGUI_CSK_ENABLE__
    register_hide_softkey(wgui_clear_center_softkey, MMI_CENTER_SOFTKEY);
#endif

    /* softkey */
    disable_softkey_background();

    /* status bar */
    show_status_icon_bar(0);
    register_hide_status_icon_bar(0,hide_status_icons_bar0_by_transparent);
    
    /* setup multiline inputbox */
#ifdef __MMI_T9__
    InuptMethodEnterCategory5();
#elif defined __MMI_ZI__
    ZiInuptMethodEnterCategory5();
#elif defined __MMI_KA__
    KonkaInuptMethodEnterCategory5();
#elif defined __MMI_ITAP__
    ItapInuptMethodEnterCategory5();
#elif defined __MMI_GB__
    GbInuptMethodEnterCategory5();
#endif 
    register_hide_multitap(category404_hide_multitap);
    
    wgui_fill_AP_required_input_mode_set(required_input_mode);
    wgui_set_AP_required_input_mode_set_flag();
    wgui_setup_inputbox(
        MMI_content_x + 10,
        MMI_content_y + 10,
        MMI_content_width - 20,
        get_menu_item_height() * 4,
        Buffer,
        BufferLength,
        MMI_CATEGORY404_ID,
        get_string(right_softkey),
        get_image(right_softkey_icon),
        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_USE_ONLY_ENGLISH_MODES,
        history_buffer,
        1);
    MMI_multiline_inputbox.flags |=
        UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR;
    /* disable new line symbol */
    MMI_current_input_ext_type = INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL;
    cat404_fibox_backup = MMI_multiline_inputbox.normal_filler;
    MMI_multiline_inputbox.normal_filler = &cat404_fibox;

    gdi_layer_unlock_frame_buffer();
    /* setup draw manager functions */
    ExitCategoryFunction = ExitCategory404Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32CatId = MMI_CATEGORY404_ID;
    dm_data.S32ScrId = GetActiveScreenId();
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND | DM_SHOW_VKPAD | DM_NO_TITLE | DM_SPECIFIC_HIDE_STATUS_BAR;
    dm_setup_data(&dm_data);
    dm_register_vkpad_callback(cat5_virtual_keypad_callback);
    dm_redraw_category_screen();
}


/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-12 10:42 */
#if ( !(defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
void	(*SimulateCenterKeyDail)(void)=UI_dummy_function;
MMI_key_event_type Keystyle;
MMI_BOOL SimulateCenterKeyDailPenDownFlag = FALSE;
#ifdef __MMI_MAINLCD_240X320__
#define  SIMULATE_CENTER_KEY_MIN_X   110
#define  SIMULATE_CENTER_KEY_MAX_X   129
#define  SIMULATE_CENTER_KEY_MIN_Y   297
#define  SIMULATE_CENTER_KEY_MAX_Y   319
#else
#ifdef __MMI_MAINLCD_176X220__
#define  SIMULATE_CENTER_KEY_MIN_X   78
#define  SIMULATE_CENTER_KEY_MAX_X   97
#define  SIMULATE_CENTER_KEY_MIN_Y   200
#define  SIMULATE_CENTER_KEY_MAX_Y   219
#else
#ifdef __MMI_MAINLCD_220X176__
#define  SIMULATE_CENTER_KEY_MIN_X   78
#define  SIMULATE_CENTER_KEY_MAX_X   97
#define  SIMULATE_CENTER_KEY_MIN_Y   200
#define  SIMULATE_CENTER_KEY_MAX_Y   219
#else
#define  SIMULATE_CENTER_KEY_MIN_X   54
#define  SIMULATE_CENTER_KEY_MAX_X   73
#define  SIMULATE_CENTER_KEY_MIN_Y   140
#define  SIMULATE_CENTER_KEY_MAX_Y   159
#endif
#endif
#endif
static MMI_BOOL need_center_key_flag = FALSE;

/*************************************************************************
* FUNCTION
*	SetNeedCenterKeyFlag
*
* DESCRIPTION
*	This function is to set center key flag for using simulate center key in list
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
VOID SetNeedCenterKeyFlag(MMI_BOOL flag)
{
  need_center_key_flag = flag;
}
/*************************************************************************
* FUNCTION
*	GetNeedCenterKeyFlag
*
* DESCRIPTION
*	This function is to get center key flag for using simulate center key in list
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
MMI_BOOL GetNeedCenterKeyFlag(void)
{
  return need_center_key_flag;
}
/*************************************************************************
* FUNCTION
*	SimulateCenterKeyDailPenDownHandler
*
* DESCRIPTION
*	This function is to deal with pen down event when pen down on simulate center key
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
MMI_BOOL SimulateCenterKeyDailPenDownHandler(mmi_pen_point_struct point )
{
     /*-----------------------------------------------------------------*/
   /* Local Variables                                                									*/
   /*-----------------------------------------------------------------*/
	MMI_BOOL ret = MMI_FALSE;
	SimulateCenterKeyDailPenDownFlag = TRUE;
	if((point.x>= SIMULATE_CENTER_KEY_MIN_X)&&(point.x<= SIMULATE_CENTER_KEY_MAX_X)&&
	(point.y>= SIMULATE_CENTER_KEY_MIN_Y)&&(point.y<= SIMULATE_CENTER_KEY_MAX_Y))
	{
		ret = MMI_TRUE;
	}
	if(ret)
	{
		gui_show_image(SIMULATE_CENTER_KEY_MIN_X , SIMULATE_CENTER_KEY_MIN_Y , (UI_image_type) GetImage(SIMULATE_CENTER_KEY_ICON2));
		gdi_layer_blt_previous(SIMULATE_CENTER_KEY_MIN_X,SIMULATE_CENTER_KEY_MIN_Y,SIMULATE_CENTER_KEY_MAX_X , SIMULATE_CENTER_KEY_MAX_Y);
		if(KEY_EVENT_DOWN == Keystyle)
		{
			SimulateCenterKeyDail();
		}
	}
	return ret;
}
/*************************************************************************
* FUNCTION
*	SimulateCenterKeyDailPenUpHandler
*
* DESCRIPTION
*	This function is to deal with pen up event when pen up from simulate center key
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
MMI_BOOL SimulateCenterKeyDailPenUpHandler(mmi_pen_point_struct point )
{
       /*-----------------------------------------------------------------*/
   /* Local Variables                                                									*/
   /*-----------------------------------------------------------------*/
	MMI_BOOL ret = MMI_FALSE;
	SimulateCenterKeyDailPenDownFlag = FALSE;

	if((point.x>= SIMULATE_CENTER_KEY_MIN_X)&&(point.x<= SIMULATE_CENTER_KEY_MAX_X)&&
	(point.y>= SIMULATE_CENTER_KEY_MIN_Y)&&(point.y<= SIMULATE_CENTER_KEY_MAX_Y))
	{
		ret = MMI_TRUE;
	}
	if(ret)
	{
		gui_show_image(SIMULATE_CENTER_KEY_MIN_X, SIMULATE_CENTER_KEY_MIN_Y, (UI_image_type) GetImage(SIMULATE_CENTER_KEY_ICON1));
		gdi_layer_blt_previous(SIMULATE_CENTER_KEY_MIN_X,SIMULATE_CENTER_KEY_MIN_Y,SIMULATE_CENTER_KEY_MAX_X , SIMULATE_CENTER_KEY_MAX_Y);
		if(KEY_EVENT_UP == Keystyle)
		{
			SimulateCenterKeyDail();
		}
	}
	return ret;
}
/*************************************************************************
* FUNCTION
*	SimulateCenterKeyDailPenMoveHandler
*
* DESCRIPTION
*	This function is to deal with pen move event when pen moving on simulate center key
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
MMI_BOOL SimulateCenterKeyDailPenMoveHandler(mmi_pen_point_struct point )
{
   /*-----------------------------------------------------------------*/
   /* Local Variables                                                									*/
   /*-----------------------------------------------------------------*/
	MMI_BOOL ret = MMI_FALSE;
	if((point.x>= SIMULATE_CENTER_KEY_MIN_X)&&(point.x<= SIMULATE_CENTER_KEY_MAX_X)&&
	(point.y>= SIMULATE_CENTER_KEY_MIN_Y)&&(point.y<= SIMULATE_CENTER_KEY_MAX_Y))
	{
		ret = MMI_TRUE;
	}
	if(ret)
	{
		SimulateCenterKeyDailPenDownFlag = TRUE;
		gui_show_image(SIMULATE_CENTER_KEY_MIN_X , SIMULATE_CENTER_KEY_MIN_Y , (UI_image_type) GetImage(SIMULATE_CENTER_KEY_ICON2));
	}
	else
	{
		SimulateCenterKeyDailPenDownFlag = FALSE;
		gui_show_image(SIMULATE_CENTER_KEY_MIN_X, SIMULATE_CENTER_KEY_MIN_Y, (UI_image_type) GetImage(SIMULATE_CENTER_KEY_ICON1));
	}
	gdi_layer_blt_previous(SIMULATE_CENTER_KEY_MIN_X,SIMULATE_CENTER_KEY_MIN_Y,SIMULATE_CENTER_KEY_MAX_X , SIMULATE_CENTER_KEY_MAX_Y);

	return ret;
}
/*************************************************************************
* FUNCTION
*	DrawSimulateCenterKey
*
* DESCRIPTION
*	This function is to draw simulate center key
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
VOID DrawSimulateCenterKey(void)
{
  if(SimulateCenterKeyDailPenDownFlag)
  {
    gui_show_image(SIMULATE_CENTER_KEY_MIN_X, SIMULATE_CENTER_KEY_MIN_Y, (UI_image_type) GetImage(SIMULATE_CENTER_KEY_ICON2));
  }
  else
  {
    gui_show_image(SIMULATE_CENTER_KEY_MIN_X, SIMULATE_CENTER_KEY_MIN_Y, (UI_image_type) GetImage(SIMULATE_CENTER_KEY_ICON1));
  }
}
/*************************************************************************
* FUNCTION
*	SetSimulateCenterKeyDailhandlers
*
* DESCRIPTION
*	This function is to set simulate center key handlers
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
VOID SetSimulateCenterKeyDailhandlers(void)
{
	DrawSimulateCenterKey();
	mmi_wgui_register_category_screen_control_area_pen_handlers(SimulateCenterKeyDailPenDownHandler, MMI_PEN_EVENT_DOWN);
	mmi_wgui_register_category_screen_control_area_pen_handlers(SimulateCenterKeyDailPenUpHandler, MMI_PEN_EVENT_UP);
	mmi_wgui_register_category_screen_control_area_pen_handlers(SimulateCenterKeyDailPenMoveHandler, MMI_PEN_EVENT_MOVE);
	gdi_layer_blt_previous(SIMULATE_CENTER_KEY_MIN_X,SIMULATE_CENTER_KEY_MIN_Y,SIMULATE_CENTER_KEY_MAX_X , SIMULATE_CENTER_KEY_MAX_Y);

}
/*************************************************************************
* FUNCTION
*	ClearSimulateCenterKeyDailhandlers
*
* DESCRIPTION
*	This function is to clear simulate center key handlers
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
VOID ClearSimulateCenterKeyDailhandlers(void)
{
	mmi_wgui_register_category_screen_control_area_pen_handlers( (mmi_pen_handler)MMI_dummy_function, MMI_PEN_EVENT_DOWN);
	mmi_wgui_register_category_screen_control_area_pen_handlers( (mmi_pen_handler)MMI_dummy_function, MMI_PEN_EVENT_UP);
	mmi_wgui_register_category_screen_control_area_pen_handlers( (mmi_pen_handler)MMI_dummy_function, MMI_PEN_EVENT_MOVE);
}
/*************************************************************************
* FUNCTION
*	SimulateCenterKeyDailFunction
*
* DESCRIPTION
*	This function is get simulate center key handlers, key type and set simulate center key handlers 
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
VOID SimulateCenterKeyDailFunction(void (*f)(void),MMI_key_event_type k)
{
	SimulateCenterKeyDail = f;
	Keystyle = k;
	SetSimulateCenterKeyDailhandlers();
}
#endif
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-12 10:42 */




