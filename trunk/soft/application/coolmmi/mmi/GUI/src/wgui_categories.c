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
 * wgui_categories.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Ordinary categories.
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
/**********************************************************************************
   Filename:      wgui_categories.c
 
   Date Created:  August-07-2002
   Contains:      UI wrapper routines
               High Level Category screen wrappers.
**********************************************************************************/

#include "mmi_features.h"
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#undef __NEWSIMULATOR
#include "globaldefs.h"
#include "eventsgprot.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "historygprot.h"
#include "frameworkstruct.h"
#include "timerevents.h"
#include "eventsdef.h"
#include "unicodexdcl.h"
#include "gui_data_types.h"
#include "gui_setting.h"
#include "gui_themes.h"
#include "wgui.h"
#include "wgui_softkeys.h"
#include "wgui_menu_shortcuts.h"
#include "wgui_fixed_menus.h"
#include "wgui_fixed_menuitems.h"
#include "wgui_inputs.h"
#include "wgui_status_icons.h"
#include "wgui_datetime.h"
#include "wgui_categories_defs.h"
#include "wgui_categories_idlescreen.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_mm.h"
#include "wgui_categories_sublcd.h"
#include "wgui_categories_util.h"
#include "wgui_dynamic_menus.h"
#include "wgui_dynamic_menuitems.h"
#include "wgui_asyncdynamic_menus.h"
#include "wgui_asyncdynamic_menuitems.h"
#include "wgui_draw_manager.h"
#include "idleappdef.h"
#include "idleappprot.h"
#include "wallpaperdefs.h"
#include "screensaverdefs.h"
#include "debuginitdef.h"
#include "mainmenudef.h"
#include "gdi_include.h"        /* include for graphic lib */
#include "settinggprots.h"
#include "shortcutsdefs.h"
#include "simdetectiongexdcl.h"
#include "phonebooktypes.h"
#include "wgui_draw_manager.h"  /* add for draw manager */
#include "wgui_touch_screen.h"  /* for touch screen apis */
#include "wgui_categories_util.h"       
#ifdef __MMI_UI_TAB_PANE__
#include "wgui_tab.h"
#endif 
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
#include "audioplayerprot.h"
#include "audioplayermainscreen.h"
#endif /* __MMI_AUDIO_PLAYER__ */ 
#if defined(__MMI_MAINLCD_220X176__)
#include "callhistoryenum.h"
#endif
 
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-7 15:26 */
#ifdef __MMI_BIDI_ALG__
#include "bididef.h"
#include "bidiprot.h"
#endif
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-7 15:26 */
#include "ucs2prot.h"
/* CSD end */

#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
#endif /* defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__) */ 
#if defined(__MMI_TOUCH_SCREEN__)
#include "editorpen.h"
#include "wgui_virtual_keyboard.h"
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

#ifdef __MMI_EDITABLE_AUDIO_EQUALIZER__
#ifdef __MMI_TOUCH_SCREEN__
#include "touchscreengprot.h"
#endif 
#include "soundeffect.h"
#endif /* __MMI_EDITABLE_AUDIO_EQUALIZER__ */ 
#include "mmi_trace.h"
 
#if defined(__MMI_TOUCH_SCREEN__) //&& defined(__MMI_WALLPAPER_ON_BOTTOM__)
#include "volumehandler.h"
#endif 
 
#if (defined(CSD_TARGET) && !defined(__MAUI_BASIC__) && !defined(__L1_STANDALONE__) && !defined(EMPTY_MMI))
#include "bmt.h"
//#include "pwic.h"
extern kal_bool custom_cfg_gpio_set_level(kal_uint8 gpio_dev_type, kal_uint8 gpio_dev_level);
extern void setup_UI_wrappers(void);
#endif /* (defined(CSD_TARGET) && !defined(__MAUI_BASIC__) && !defined(__L1_STANDALONE__) && !defined(EMPTY_MMI)) */ 

#define MMI_CAT401_X_OFFSET   17

/* for NFB progress bar images */
extern U8 *nfb_progress_images[];

#ifdef __MMI_BIDI_ALG__
extern U8 visual_str[];
#endif 
extern BOOL r2lMMIFlag;

/* image store the image data of left arrow key */
extern U8 inline_select_left_arrow_image[];

/* image store the image data of right arrow key */
extern U8 inline_select_right_arrow_image[];
extern U8 matrix_main_menu_highlight_bitmap_data[];
extern S32 GUI_current_fixed_icontext_list_menuitem_column;

extern U16 StopMyTimer(U16 nTimerId);
extern U16 StartMyTimerInt(U16 nTimerId, U32 nTimeDuration, oslTimerFuncPtr TimerExpiry, U8 alignment);

/* Externals for graphics context switching  */
extern bitmap main_LCD_device_bitmap;
extern bitmap sub_LCD_device_bitmap;
extern bitmap *current_LCD_device_bitmap;

/* store Main lcd width */
extern S32 MAIN_LCD_device_width;

/* store Main LCD height */
extern S32 MAIN_LCD_device_height;

extern BOOL dynamic_item_text_align_left;

extern FuncPtr gExitFullScreenCallBack;

void UI_set_main_LCD_graphics_context(void);
void UI_set_sub_LCD_graphics_context(void);

extern pBOOL GetShowAOC(void);
extern S32 UI_get_font_height(UI_font_type f);

extern void CSD_show_animation(S32 x, S32 y, U8 *image);
extern void CSD_show_animation_frames(S32 x, S32 y, U8 *image, S32 start_frame);
extern void UI_draw_horizontal_line(S32 x1, S32 x2, S32 y, color_t c);
extern void UI_putpixel(S32 x, S32 y, color_t c);
extern void UI_draw_vertical_line(S32 y1, S32 y2, S32 x, color_t c);
extern void wgui_setup_singleline_inputbox(
                S32 x,
                S32 y,
                S32 width,
                S32 height,
                U8 *buffer,
                S32 buffer_size,
                U16 category_screen_ID,
                UI_string_type RSK_label,
                PU8 RSK_icon,
                U16 input_type,
                U8 *history_buffer,
                U8 information_bar_flag);
extern void wgui_close_singleline_inputbox(void);
extern void get_singleline_inputbox_category_history(U16 history_ID, U8 *history_buffer, S16 input_type);
extern void move_slide_control_value(slide_control *s, S32 x, S32 y);

extern void (*ExitCategoryFunction) (void);

#ifdef __MMI_UI_LIST_TRANSITION__
extern void set_list_transition(wgui_transition_styles style);
extern wgui_transition_styles get_list_transition(void);
extern void reset_list_transition(void);
#endif /* __MMI_UI_LIST_TRANSITION__ */ 

extern void dm_set_scr_bg_image_no_draw(U16 image_id, S8 *file_name, S32 x, S32 y, U8 opacity);

#ifdef __MMI_T9__
#include "t9main.h"     /* t9synt9withinputbox() */
extern void InuptMethodEnterCategory5(void);
#elif defined __MMI_ZI__
extern void ZiInuptMethodEnterCategory5(void);
#elif defined __MMI_KA__
extern void KonkaInuptMethodEnterCategory5(void);
#elif defined __MMI_ITAP__
extern void ItapInuptMethodEnterCategory5(void);
#endif 
#ifdef __MMI_GB__
extern void GbInuptMethodEnterCategory5(void);
#endif

U8 category89_string_list[MAX_CATEGORY89_STRINGS][MAX_CATEGORY89_STRING_LENGTH];

#ifdef __MMI_CALORIE__
#endif /* __MMI_CALORIE__ */ 

/* Store current menu type..matrix,list,circular */
S32 MMI_current_menu_type = MATRIX_MENU;

extern S32 volume_level_UI;

#ifdef __MMI_IDLE_FULL_SCREEN__
U16 on_idle_screen = 0;
#endif 

U8 Category154Align = 0;
U8 cat9_show_message = 0;

/* a flagused to check if text dispaly in category125 is scrolling or not */
U8 flag_scroll = FALSE;

/* used by category125 to display valeu in dispaly valeu bar beliow title bar */
U8 *disp_valuestr = NULL;

/* used by categopry 125 */
U8 *disp_barstr = NULL, *left_value = NULL, *right_value = NULL;

S16 status_icon = 0;

/* global variable store the image height,width ,and no_of_pixtel */
S32 no_of_pixel = 0;

/* store category142 image list */
U16 *category_142_image_list = NULL;

/* store the category142 image list inex */
U8 **category_142_image_file_name_list = NULL;

/* store the category142 image filename list inex */
S32 *category_142_image_list_index = NULL;

/* store number of items of category 142 */
S32 category_142_image_list_n_items = 0;

/* vertical scroll bar of category 142 */
vertical_scrollbar category142_vbar;

/* list of titles of category 142 */
U8 **category_142_list_of_titles = NULL;

#ifdef __MMI_EDITABLE_AUDIO_EQUALIZER__
equalizer_setting_info g_audio_equalizer_current_setting_info;  /* to store the values of currently accessed equal8izer setting */
float one_pixel_represents; /* one pixel in each equalizer bar represents what value.... */
float temp_float_var;
extern void mmi_settings_audio_equalizer_change_sound_level(S16 change);

extern U16 g_audio_equalizer_bar_start_x, g_audio_equalizer_bar_start_y;
extern U16 g_audio_equalizer_bar_width, g_audio_equalizer_bar_height, g_audio_equalizer_dist_btw_bars;
extern U16 g_audio_equalizer_scrollable_ht, g_audio_equalizer_unit_scroll;

#endif /* __MMI_EDITABLE_AUDIO_EQUALIZER__ */ 

/* category 154 message 1 y postition */
S32 category_154_message1_y = 0;

/* category 154 message 2 y position  */
S32 category_154_message2_y = 0;

scrolling_text scroll_text_cat125;

UI_string_type category155_slide_string = NULL;

#ifdef __FT_IDLE_SMART_DIAL__ 
int	(*catSmartDial_search_function)(U8*);
#endif

#if defined (__MMI_WALLPAPER_ON_BOTTOM__)
static UI_image_ID_type  idle_scr_bg_ID;
static S8 *idle_scr_bg_filename;
static S32 idle_scr_bg_x, idle_scr_bg_y;
static U8 idle_scr_bg_opacity;
static S32 idle_scr_bg_flags;//082906 scr_bg
#endif /* defined (__MMI_WALLPAPER_ON_BOTTOM__) */ 

extern U16 title_bg_id;

U16 Cat402PercentageBarId = 0;

/* Start of Common List Category Screen   */
extern PU8 subMenuDataPtrs[MAX_SUB_MENUS];

#ifdef __J2ME__
extern gdi_handle jui_layer_handle;
extern void ExitCategory176Screen(void);
#endif

/* 061306 dummy category Start */
U32 status_bar_flag[MAX_STATUS_ICON_BARS];
#ifdef __FT_IDLE_SMART_DIAL__ 
extern stFontAttribute wgui_dialer_box_f1;

UI_filled_area catSmartDial_inputbox_BG_filler=
{
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,	
NULL,
{8, 35, 93, 100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0

};
UI_filled_area catSmartDial_DynList_BG_filler=
{
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,		
NULL,
{8, 35, 93, 100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0

};

UI_dialer_input_box_theme  catSmartdial_dialer_input_box_theme =

{

       &catSmartDial_inputbox_BG_filler,
	&catSmartDial_inputbox_BG_filler,
	&catSmartDial_inputbox_BG_filler,

       {	255,255,255,100		},
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

UI_fixed_list_menu_theme catSmartdial_DynList_theme=
{	(UI_filled_area*)&catSmartDial_DynList_BG_filler,
	(UI_filled_area*)&catSmartDial_DynList_BG_filler,
	5,
	UI_LIST_MENU_LOOP
};

#endif

void redraw_multiline_inputbox_new(void);

/*****************************************************************************
 * FUNCTION
 *  ExitCategoryDummyScreen
 * DESCRIPTION
 *  Exits the dummy category screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategoryDummyScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_status_icon_bar_type(0, status_bar_flag[0]);
    set_status_icon_bar_type(1, status_bar_flag[1]);
}   /* end of ExitCategoryDummyScreen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategoryDummyScreen
 * DESCRIPTION
 *  Set some global settings for category without drawing anything on the screen
 * PARAMETERS
 *  
 * RETURNS
 *  
 *****************************************************************************/
void ShowCategoryDummyScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    status_bar_flag[0] = get_status_icon_bar_type(0);
    hide_status_icon_bar(0);
    status_bar_flag[1] = get_status_icon_bar_type(1);
    hide_status_icon_bar(1);

    ExitCategoryFunction = ExitCategoryDummyScreen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}   /* end of ShowCategoryDummyScreen */
/* 061306 dummy category End */


/*****************************************************************************
 * FUNCTION
 *  RedrawListCategoryScreen
 * DESCRIPTION
 *  Redraw category function of current main lcd category scrren using list menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawListCategoryScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if 0//def __MMI_UI_TITLE_TRANSITION__
    S32 no_transition_in_title = 0;

    {
    #ifdef __MMI_UI_TAB_PANE__
        if (is_enabled_tab_pane())
        {
            tab_pane_modify_data();
        }
    #endif /* __MMI_UI_TAB_PANE__ */ 
        if ((gOnFullScreen & MMI_IN_FULL_SCREEN || gOnFullScreen & MMI_LEAVE_FULL_SCREEN) &&
            !(gOnFullScreen & MMI_LEAVE_FULL_IDLE_SCREEN))
        {
            no_transition_in_title = 1;
        }
        if (get_title_transition() != WGUI_TRANSITION_NONE && !no_transition_in_title)
        {
            draw_title();
        }
    }
#else /* __MMI_UI_TITLE_TRANSITION__ */ 
#ifdef __MMI_UI_TAB_PANE__
    if (is_enabled_tab_pane())
    {
        tab_pane_modify_data();
    }
#endif /* __MMI_UI_TAB_PANE__ */ 
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
#ifdef __MMI_UI_LIST_TRANSITION__
    if (get_list_transition() == WGUI_TRANSITION_T2D)
    {
        draw_title();
        show_fixed_list();
    }
#endif /* __MMI_UI_LIST_TRANSITION__ */ 

    gdi_layer_lock_frame_buffer();
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_pop_up_description_stop_scroll();
#endif 

#ifdef __MMI_WGUI_MINI_TAB_BAR__
    if (wgui_if_mini_tab_bar_enable() == MMI_TRUE)
    {
        move_fixed_list(MMI_fixed_list_menu.x, MMI_content_y + 5);
        resize_fixed_list(MMI_fixed_list_menu.width, MMI_fixed_list_menu.height - 5);
        wgui_show_mini_tab_bar();
    }
#endif /* __MMI_WGUI_MINI_TAB_BAR__ */ 
    clear_buttonbar();
    show_title_status_icon();

#ifdef __MMI_UI_TITLE_TRANSITION__
    if (no_transition_in_title || get_title_transition() == WGUI_TRANSITION_NONE)
#endif 

         
        //For transition, title function call should be from here 
        draw_title();
     

#ifdef __MMI_UI_LIST_TRANSITION__
    if (get_list_transition() == WGUI_TRANSITION_NONE)
#endif 
        show_fixed_list();
    show_softkey_background();
    show_left_softkey();
    show_right_softkey();
#ifdef __MMI_UI_TAB_PANE__
    if (is_enabled_tab_pane())
    {
        show_tab_pane();
    }
#endif /* __MMI_UI_TAB_PANE__ */ 
    gdi_layer_unlock_frame_buffer();

#ifdef __MMI_UI_LIST_TRANSITION__
    if (get_list_transition() == WGUI_TRANSITION_D2T)
    {
        show_fixed_list();
    }
#endif /* __MMI_UI_LIST_TRANSITION__ */ 

     
#if defined(__MMI_UI_LIST_TRANSITION__) && (LIST_TRANSITION_STYLE_NON_BLOCKING == 1)
    if (get_list_transition() == WGUI_TRANSITION_T2D && is_in_nonblocking_list_transition() == 1)
    {
        gdi_layer_blt_previous(0, get_original_prev_list_y_value(), UI_device_width - 1, UI_device_height - 1);
    }
    else
#endif /* defined(__MMI_UI_LIST_TRANSITION__) && (LIST_TRANSITION_STYLE_NON_BLOCKING == 1) */ 
         
        gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}   /* end of RedrawListCategoryScreen */


/*****************************************************************************
 * FUNCTION
 *  ExitListCategoryScreen
 * DESCRIPTION
 *  Exit category function of current main lcd category scrren using list menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitListCategoryScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_disable_title_shortcut_display = 0;  
    disable_menu_shortcut_box_display = 0;
	wgui_reset_disable_shortcut_display();
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    reset_pop_up_descriptions();

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    reset_all_force_flags_for_hints();
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

}   /* end of ExitListCategoryScreen */


/*****************************************************************************
 * FUNCTION
 *  GetListCategoryHistory
 * DESCRIPTION
 *  Get category history function of current main lcd category scrren
 *  using list menu
 * PARAMETERS
 *  history_buffer      [?]         
 *  history(?)          [IN]        Buffer store the valeu of history
 * RETURNS
 *  void
 *****************************************************************************/
U8 *GetListCategoryHistory(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history((U16) g_gui_current_catid, history_buffer);
    return (history_buffer);
}   /* end of GetListCategoryHistory */


/*****************************************************************************
 * FUNCTION
 *  SetListScreenFunctions
 * DESCRIPTION
 *  Common functions to set ExitCategory, RedrawCategory, GetCategoryHistory
 *  GetCategoryHistorySize functions.
 * PARAMETERS
 *  cat_id      [IN]        Current category id
 * RETURNS
 *  void
 *****************************************************************************/
void SetListScreenFunctions(S32 cat_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ExitCategoryFunction = ExitListCategoryScreen;
    RedrawCategoryFunction = RedrawListCategoryScreen;
    GetCategoryHistory = GetListCategoryHistory;
    GetCategoryHistorySize = get_list_menu_category_history_size;
    g_gui_current_catid = cat_id;
}   /* end of SetListScreenFunctions */


/*****************************************************************************
 * FUNCTION
 *  ShowListCategoryScreen
 * DESCRIPTION
 *  Common function to show category screens containing list menu.
 * PARAMETERS
 *  title                   [IN]        String of title
 *  title_icon              [IN]        Icon of title
 *  left_softkey            [IN]        String of left softkey
 *  left_softkey_icon       [IN]        Icon of left softkey
 *  right_softkey           [IN]        String of right softkey
 *  right_softkey_icon      [IN]        Icon of right softkey
 *  number_of_items         [IN]        Number of items in list menu
 * RETURNS
 *  void
 *****************************************************************************/
void ShowListCategoryScreen(
        UI_string_type title,
        PU8 title_icon,
        UI_string_type left_softkey,
        PU8 left_softkey_icon,
        UI_string_type right_softkey,
        PU8 right_softkey_icon,
        S32 number_of_items)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 shortcut_width;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    /* Title */
    MMI_title_string = title;
    MMI_title_icon = title_icon;

    /* ShortCut */
    if (!wgui_is_disable_shortcut_display())
    {
        MMI_menu_shortcut_number = -1;
        register_fixed_list_shortcut_handler();
        shortcut_width = set_menu_item_count(number_of_items) + 7;
        resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
        move_menu_shortcut_handler(MMI_title_x + MMI_title_width - MMI_menu_shortcut_box.width, MMI_title_y);
        register_hide_menu_shortcut(gui_redraw_menu_shortcut);
    }
    else
    {
        MMI_disable_title_shortcut_display = 1;
        disable_menu_shortcut_box_display = 1;
    }

    /* Softkeys */
    set_left_softkey_label(left_softkey);
    set_left_softkey_icon(left_softkey_icon);
    set_right_softkey_label(right_softkey);
    set_right_softkey_icon(right_softkey_icon);
    SetupCategoryKeyHandlers();

    /* List */
    MMI_current_menu_type = LIST_MENU;
    register_fixed_list_keys();
    resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);

    resize_fixed_icontext_menuitems(MMI_fixed_list_menu.width - MMI_fixed_list_menu.vbar.width - 1, MMI_fixed_icontext_menuitem.height);         
    register_fixed_list_highlight_handler(standard_list_highlight_handler);
    set_fixed_icontext_positions(MMI_MENUITEM_HEIGHT + 2, 0, 1, 0);

    gdi_layer_unlock_frame_buffer();
}   /* end of ShowListCategoryScreen */

/* End of Common List Category Screen  */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory1Screen
 * DESCRIPTION
 *  Displays the category1 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items
 *  list_of_items           [IN]        List of text items
 *  highlighted_item        [IN]        Index of the item to be highlighted by default
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory1Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_items,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < number_of_items; i++)
    {
        subMenuDataPtrs[i] = (PU8) get_string(list_of_items[i]);
    }

#ifdef __MMI_UI_LIST_TRANSITION__
    if (!history_buffer)
    {
        set_list_transition(WGUI_TRANSITION_D2T);
    }
#endif /* __MMI_UI_LIST_TRANSITION__ */ 

    ShowCategory6Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        subMenuDataPtrs,
        NULL,
        highlighted_item,
        history_buffer);

#ifdef __MMI_UI_LIST_TRANSITION__
#if(!LIST_TRANSITION_STYLE_NON_BLOCKING)
    reset_list_transition();
#endif 
#endif /* __MMI_UI_LIST_TRANSITION__ */ 
}   /* end of ShowCategory1Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory2Screen
 * DESCRIPTION
 *  Displays the category2 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Message string
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory2Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *message_str = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    message_str = (U8*) get_string((UI_image_ID_type ) message);
    ShowCategory7Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        message_str,
        history_buffer);
}   /* end of ShowCategory2Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory6Screen
 * DESCRIPTION
 *  Displays the category6 screen
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon shown with the title
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Icon for the left softkey
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Icon for the right softkey
 *  number_of_items             [IN]        Number of items
 *  list_of_items               [IN]        List of text items.
 *  list_of_descriptions        [IN]        
 *  highlighted_item            [IN]        This is the default item that will be highlighted (if there is no history)
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory6Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U8 **list_of_descriptions,
        S32 highlighted_item,
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

    resize_fixed_list(MMI_content_width, MMI_content_height);    
    create_fixed_text_menuitems();
    associate_fixed_text_list();

    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    if (list_of_descriptions == NULL)
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_text_item((UI_string_type) list_of_items[i]);
            wgui_pop_up_description_strings[i].text_strings[0] = NULL;
        }
    }
    else
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_text_item((UI_string_type) list_of_items[i]);
            wgui_pop_up_description_strings[i].text_strings[0] = (UI_string_type) list_of_descriptions[i];
        }
    }

    resize_fixed_text_menuitems_to_list_width();
    resize_fixed_text_menuitems(0, MMI_MENUITEM_HEIGHT);     

    h_flag = set_list_menu_category_history(MMI_CATEGORY6_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    if (list_of_descriptions != NULL)
    {
        set_pop_up_descriptions(1, number_of_items, MMI_fixed_list_menu.highlighted_item);
    }

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitListCategoryScreen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dm_get_category_history;
    GetCategoryHistorySize = dm_get_category_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY6_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory6Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory7Screen
 * DESCRIPTION
 *  Displays the category7 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        Notification message (string. Not string ID)
 *  history_buffer          [IN]        History buffer
 *  right_softkey_iconIN Icon for the right softkey(?)
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory7Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);

    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_message_string = (UI_string_type) (message);
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);

#ifdef __MMI_MAINLCD_220X176__
{
	extern color_t white_color;
	MMI_multiline_inputbox.normal_text_color = white_color;
}
#endif

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory7Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dm_get_category_history;
    GetCategoryHistorySize = dm_get_category_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY7_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory7Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory7Screen
 * DESCRIPTION
 *  Exits the category7 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory7Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_softkeys();
}   /* end of ExitCategory7Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory8Screen
 * DESCRIPTION
 *  Displays the category8 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        Notification message
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  image(?)                [IN]        Notification image
 *  right_softkey_iconIN Icon for the right softkey(?)
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory8Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCategory66Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        (U8*) get_string(message),
        message_icon,
        history_buffer);
}   /* end of ShowCategory8Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory208Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [?]         
 *  message_icon            [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory208Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S8 *message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCategory66Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        (U8*) message,
        message_icon,
        history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory9Screen
 * DESCRIPTION
 *  Displays the category9 screen
 * PARAMETERS
 *  message             [IN]        Message string
 *  message_icon        [IN]        
 *  history_buffer      [IN]        History buffer
 *  icon(?)             [IN]        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory9Screen(U16 message, U16 message_icon, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_reset_context();
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();
    entry_full_screen();
    MMI_menu_shortcut_number = -1;
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();

    if (message != 0)
    {
        MMI_message_string = (UI_string_type) get_string(message);
        l = gui_strlen(MMI_message_string);
        create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
        MMI_multiline_inputbox.flags |=
            (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY |
             UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);
    }
    else
    {
        MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    }
    dm_add_image(get_image(message_icon), NULL, NULL);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = UI_dummy_function;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dm_get_category_history;
    GetCategoryHistorySize = dm_get_category_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY9_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory9Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory9Screen
 * DESCRIPTION
 *  Exits the category9 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory9Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
}   /* end of ExitCategory9Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory11Screen
 * DESCRIPTION
 *  Displays the category11 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items
 *  list_of_items           [IN]        List of text items
 *  highlighted_item        [IN]        Zero based index of the highlighed item (used if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory11Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_items,
        U16 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < number_of_items; i++)
    {
        subMenuDataPtrs[i] = (PU8) get_string(list_of_items[i]);
    }
    ShowCategory36Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        subMenuDataPtrs,
        highlighted_item,
        history_buffer);

}   /* end of ShowCategory11Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory12Screen
 * DESCRIPTION
 *  (two state menuitem)
 *  Displays the Category12 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items
 *  list_of_items           [IN]        List of text items
 *  list_of_states          [IN]        Array containing the states of the items
 *  highlighted_item        [IN]        Zero based index of the highlighed item (used if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory12Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U8 *list_of_states,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    /* Setup menuitem */
    create_fixed_twostate_menuitems(get_image(CHECKBOX_ON_IMAGE_ID), get_image(CHECKBOX_OFF_IMAGE_ID));
    associate_fixed_twostate_list();

    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    resize_fixed_twostate_menuitems(0, MMI_MENUITEM_HEIGHT);     
    set_fixed_twostate_positions(MMI_MENUITEM_HEIGHT, 0, 1, 0);  

    set_left_softkey_function(standard_check_list_handle_left_softkey_up, KEY_EVENT_UP);
    checklist_category_menu_item_states = list_of_states;
    for (i = 0; i < number_of_items; i++)
    {
        add_fixed_twostate_item((UI_string_type) list_of_items[i]);
        if (list_of_states[i])
        {
            select_fixed_twostate_item(i);
        }
    }

    /* Read History */
    h_flag = set_list_menu_category_history(MMI_CATEGORY12_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }

     
    /* register_menu_shortcut_selected(standard_check_list_handle_item_select); */
    resize_fixed_twostate_menuitems_to_list_width();

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitListCategoryScreen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dm_get_category_history;
    GetCategoryHistorySize = dm_get_category_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY12_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory12Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory13Screen
 * DESCRIPTION
 *  Displays the Category13 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items
 *  list_of_items           [IN]        List of text items
 *  list_of_states          [IN]        Array containing the states of the items
 *  highlighted_item        [IN]        Zero based index of the highlighed item (used if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory13Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_items,
        U8 *list_of_states,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < number_of_items; i++)
    {
        subMenuDataPtrs[i] = (PU8) get_string(list_of_items[i]);
    }

    ShowCategory12Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        subMenuDataPtrs,
        list_of_states,
        highlighted_item,
        history_buffer);
}   /* end of ShowCategory13Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory15Screen
 * DESCRIPTION
 *  Displays the category15 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items in the menu
 *  list_of_items           [IN]        Array of items
 *  list_of_icons           [IN]        Array of icons
 *  flags                   [IN]        (see explanation below)
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 *  LIST_MENU(?)            [IN]        
 *  Values(?)               [IN]        Allowed for these 8 bits are:
 *  MATRIX_MENU(?)          [IN]        
 *  The(?)                  [IN]        Last 8 bits of flags represent the type of the menu.
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory15Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_items,
        U16 *list_of_icons,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef UI_SMALL_LIST_SCREEN
    if (whether_no_small_screen())
    {
        ShowCategory10Screen(
            title,
            title_icon,
            left_softkey,
            left_softkey_icon,
            right_softkey,
            right_softkey_icon,
            number_of_items,
            list_of_items,
            list_of_icons,
            highlighted_item,
            history_buffer);
    }
    else
#endif /* UI_SMALL_LIST_SCREEN */ 
    {

        /*----------------------------------------------------------------*/
        /* Local Variables                                                */
        /*----------------------------------------------------------------*/
        dm_data_struct dm_data;
        S32 i, shortcut_width, ixsize, iysize;
        U8 h_flag;

        /*----------------------------------------------------------------*/
        /* Code Body                                                      */
        /*----------------------------------------------------------------*/
	 TRACE_GDI_FUNCTION();
        gdi_layer_lock_frame_buffer();

        MMI_menu_shortcut_number = -1;
        change_left_softkey(left_softkey, left_softkey_icon);
        change_right_softkey(right_softkey, right_softkey_icon);
        SetupCategoryKeyHandlers();

        create_fixed_icontext_menuitems();
        MMI_current_menu_type = flags & 0x0f;
        MMI_title_string = (UI_string_type) get_string(title);
        MMI_title_icon = (PU8) get_image(title_icon);

        switch (MMI_current_menu_type)
        {
            case LIST_MENU:
                /* resize_fixed_list(MMI_content_width,MMI_content_height+2); */
		 TRACE_GDI_FUNCTION();
                resize_fixed_list(MMI_content_width, MMI_content_height);        
                associate_fixed_icontext_list();
                for (i = 0; i < number_of_items; i++)
                {
                    add_fixed_icontext_item(get_string(list_of_items[i]), wgui_get_list_menu_icon(i, list_of_icons[i]));

                }
                register_fixed_list_shortcut_handler();
                register_fixed_list_keys();
                shortcut_width = set_menu_item_count(number_of_items) + 7;
                resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
                move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
                register_hide_menu_shortcut(gui_redraw_menu_shortcut);
                 
                resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);
#if defined(__MMI_NOKIA_STYLE_N800__)||defined(__PROJECT_GALLITE_C01__)
                set_fixed_icontext_positions(MMI_MENUITEM_HEIGHT + 2, 0, 1, 0);
#else
                set_fixed_icontext_positions(MMI_ICONTEXT_MENUITEM_HEIGHT + 2, 0, 1, 0);

#endif
                register_fixed_list_highlight_handler(standard_list_highlight_handler);
                h_flag = set_list_menu_category_history(MMI_CATEGORY15_LIST_ID, history_buffer);
                if (h_flag)
                {
                    fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
                }
                else
                {
                    fixed_list_goto_item_no_redraw(highlighted_item);
                }
                break;

            case MATRIX_MENU:
                associate_fixed_icontext_matrix();
                for (i = 0; i < number_of_items; i++)
                {
                    add_fixed_icontext_item(get_string(list_of_items[i]), get_image(list_of_icons[i]));
                }
                register_fixed_matrix_shortcut_handler();
                MMI_fixed_matrix_menu.flags |= UI_MATRIX_MENU_LOOP;
                register_fixed_matrix_loop_keys();
                register_fixed_matrix_highlight_handler(standard_animated_matrix_highlight_handler_type2);

                if (number_of_items > 9)
                {
                    ixsize = (MMI_content_width - MMI_fixed_matrix_menu.vbar.width - 6) / 3;
                }
                else
                {
                    ixsize = (MMI_content_width - 6) / 3;
                }
                iysize = (MMI_content_height - 6) / 3;
                configure_fixed_matrix(ixsize, iysize, 3, 0);
                shortcut_width = set_menu_item_count(number_of_items) + 7;
                resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
                move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
                register_hide_menu_shortcut(gui_redraw_menu_shortcut);
                set_fixed_icontext_positions(0, 0, 0, 0);
                MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY;
                MMI_disable_title_shortcut_display = 0;
                h_flag = set_matrix_menu_category_history(MMI_CATEGORY15_MATRIX_ID, history_buffer);
                if (h_flag)
                {
                    fixed_matrix_goto_item_no_redraw(MMI_fixed_matrix_menu.highlighted_item);
                }
                else
                {
                    fixed_matrix_goto_item_no_redraw(highlighted_item);
                }
                break;
        }

        gdi_layer_unlock_frame_buffer();

        ExitCategoryFunction = ExitCategory15Screen;
        RedrawCategoryFunction = dm_redraw_category_screen;
        GetCategoryHistory = dm_get_category_history;
        GetCategoryHistorySize = dm_get_category_history_size;
        dm_data.S32ScrId = (S32) GetActiveScreenId();
        if ((MMI_current_menu_type == LIST_MENU) || (MMI_current_menu_type == PAGE_MENU))
        {
            dm_data.S32CatId = MMI_CATEGORY15_LIST_ID;
        }
        else
        {
            dm_data.S32CatId = MMI_CATEGORY15_MATRIX_ID;
        }
        dm_data.S32flags = 0;
        dm_setup_data(&dm_data);
        dm_redraw_category_screen();

 
    }
  
}   /* end of ShowCategory15Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory15Screen
 * DESCRIPTION
 *  Exits the category15 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory15Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
#if defined(__PROJECT_GALLITE_C01__)

    switch (MMI_current_menu_type)
    {
        case MATRIX_MENU:
		/*+ zhouqin 2011-02-16 modify for q6 320X240 */
		gdi_pop_and_restore_alpha_blending_source_layer();
		/*- zhouqin 2011-02-16 modify for q6 320X240 */
		break;

		default:
		break;
	}
#endif
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    reset_fixed_matrix();
 
#ifdef UI_SMALL_LIST_SCREEN
    restore_small_screen();
#endif 
  
}

#ifdef UI_SMALL_LIST_SCREEN      

UI_filled_area *fixed_list_backup_filler;
S32 cat10_list_item_num = 0;


/*****************************************************************************
 * FUNCTION
 *  ExitCategory10Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory10Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    reset_small_screen();
#endif 
    MMI_fixed_list_menu.normal_filler = fixed_list_backup_filler;

#ifdef __MMI_WALLPAPER_ON_BOTTOM__//092906 popup title
    title_bg_id = 0;
#endif

    show_status_icon_bar(0);//052906 early status icon

    ExitListCategoryScreen();
    entry_full_screen();
}

#define UI_SMALL_LIST_SCREEN_WITHOUT_TITLE
#define MMI_SMALL_SCREEN_MAX_LIST_ITEMS_COUNT 5


/*****************************************************************************
 * FUNCTION
 *  DrawCate10CategoryControlArea
 * DESCRIPTION
 *  
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate10CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, x2, y1, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* UNUSEDPARAMETER(coordinate); */

    gui_greyscale_rectangle(0, 0, UI_device_width - 1, UI_device_height - 1, 20, 1);

    x1 = MMI_SMALL_LIST_LEFT_OFFSET - 1;
    x2 = MMI_SMALL_LIST_SCREEN_WIDTH + 2;

#ifdef UI_SMALL_LIST_SCREEN_WITHOUT_TITLE
    y1 = UI_device_height - MMI_button_bar_height - (4 + get_menu_item_height() * cat10_list_item_num + 4 + 5) - 2;
#else /* UI_SMALL_LIST_SCREEN_WITHOUT_TITLE */ 
    y1 = UI_device_height - MMI_button_bar_height - MMI_title_height - (4 +
                                                                        get_menu_item_height() * cat10_list_item_num +
                                                                        4 + 5) - 2;
#endif /* UI_SMALL_LIST_SCREEN_WITHOUT_TITLE */ 
    y2 = UI_device_height - MMI_button_bar_height;

    gui_draw_filled_area(x1, y1, x2, y2, (current_MMI_theme->small_list_screen_border_filler));

}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory10Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  number_of_items         [IN]        
 *  list_of_items           [?]         
 *  list_of_icons           [?]         
 *  highlighted_item        [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory10Screen(
        U16 title,
        U16 title_icon,
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
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i, icon_width = 0, icon_height = 0;
    color_t c = *(current_MMI_theme->UI_black_color);
    U8 h_flag, flag = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_x = MMI_SMALL_SCREEN_X_OFFSET + 1;
    MMI_title_y += MMI_title_height + MMI_SMALL_SCREEN_Y_OFFSET;
    MMI_title_width -= (2 * MMI_SMALL_SCREEN_X_OFFSET + 1);

    gdi_layer_lock_frame_buffer();

#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
    gui_add_cleanup_hook(disable_resized_small_screen);
#endif /* __MMI_UI_SMALL_SCREEN_SUPPORT__ */ 

    hide_status_icon_bar(0);//052906 early status icon
    
    create_fixed_icontext_menuitems();
    associate_fixed_icontext_list();

    fixed_list_backup_filler = MMI_fixed_list_menu.normal_filler;
    MMI_fixed_list_menu.normal_filler = (current_MMI_theme->small_list_menu_normal_filler);

    wgui_set_disable_shortcut_display();
    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    if (list_of_icons == NULL)
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_icontext_item(get_string(list_of_items[i]), NULL);
        }
    }
    else
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_icontext_item(get_string(list_of_items[i]), wgui_get_list_menu_icon(i, list_of_icons[i]));
            if (list_of_icons[i] != 0)
            {
                flag = 1;
                gui_measure_image(get_image(list_of_icons[i]), &icon_width, &icon_height);
            }
        }
    }

    if (number_of_items <= MMI_SMALL_SCREEN_MAX_LIST_ITEMS_COUNT)
    {
        cat10_list_item_num = number_of_items;
        MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR;
        resize_fixed_list(MMI_SMALL_LIST_SCREEN_WIDTH - 3, get_menu_item_height() * number_of_items);
        resize_fixed_icontext_menuitems(MMI_SMALL_LIST_SCREEN_WIDTH - 3, 0);
        move_fixed_list(
            MMI_SMALL_LIST_LEFT_OFFSET + 1,
            UI_device_height - MMI_button_bar_height - (4 + get_menu_item_height() * number_of_items + 4 + 5) + 4);
    }
    else
    {
        cat10_list_item_num = MMI_SMALL_SCREEN_MAX_LIST_ITEMS_COUNT;
        resize_fixed_list(
            MMI_SMALL_LIST_SCREEN_WIDTH - 3,
            get_menu_item_height() * MMI_SMALL_SCREEN_MAX_LIST_ITEMS_COUNT);
        resize_fixed_icontext_menuitems(MMI_SMALL_LIST_SCREEN_WIDTH - 3 - MMI_fixed_list_menu.vbar.width, 0);
        move_fixed_list(
            MMI_SMALL_LIST_LEFT_OFFSET + 1,
            UI_device_height - MMI_button_bar_height - (4 + get_menu_item_height() * MMI_SMALL_SCREEN_MAX_LIST_ITEMS_COUNT + 4 + 5) + 4);
    }

    h_flag = set_list_menu_category_history(MMI_CATEGORY10_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }

#ifdef __MMI_WALLPAPER_ON_BOTTOM__//092906 popup title
    title_bg_id = IMG_FLEXIBLE_TITLEBAR_BG;
#endif

    gdi_layer_unlock_frame_buffer();

    RedrawCategoryFunction = dm_redraw_category_screen;
    ExitCategoryFunction = ExitCategory10Screen;
    GetCategoryHistory = dm_get_category_history;
    GetCategoryHistorySize = dm_get_category_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY10_ID;
    dm_data.S32flags = 0;
    dm_add_rectangle(c, DM_RECTANGLE_FILL_GRAYSCALE);
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawCate10CategoryControlArea);
    dm_redraw_category_screen();

}
#endif /* UI_SMALL_LIST_SCREEN */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory21Screen
 * DESCRIPTION
 *  Displays the category21 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        Notification message (string. Not string ID)
 *  history_buffer          [IN]        History buffer
 *  right_softkey_iconIN Icon for the right softkey(?)
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory21Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();
    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_message_string = (UI_string_type) get_string(message);
    create_multiline_inputbox_set_buffer(
        MMI_message_string,
        gui_strlen((UI_string_type) MMI_message_string),
        gui_strlen((UI_string_type) MMI_message_string),
        0);
    SetKeyHandler(multiline_inputbox_previous_line, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(multiline_inputbox_next_line, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND);
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dm_get_category_history;
    GetCategoryHistorySize = dm_get_category_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY21_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory21Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory22Screen
 * DESCRIPTION
 *  Displays the category22 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  num_of_symbols          [IN]        Number of symbols
 *  MMI_symbols             [IN]        Symbols specific to application
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
extern color_t black_color;
void ShowCategory22Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 num_of_symbols,
        U8 **MMI_symbols,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
	
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_block_list_effect();
#endif 
     

    gdi_layer_lock_frame_buffer();
    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    create_fixed_text_menuitems();
    associate_fixed_text_matrix();
#if !defined(__MMI_USING_BLACK_THEME_BG__)	
    MMI_fixed_text_menuitem.normal_text_color = black_color;
#endif

    for (i = 0; i < num_of_symbols; i++)
    {
        add_fixed_text_item((UI_string_type) MMI_symbols[i]);
    }
    configure_fixed_matrix(19, 21, 0, 0);
    register_fixed_matrix_loop_keys();
    //register_fixed_matrix_highlight_handler(standard_noshortcut_matrix_highlight_handler);
    MMI_current_menu_type = MATRIX_MENU;
    MMI_fixed_matrix_menu.flags |= UI_MATRIX_MENU_LOOP;
    h_flag = set_matrix_menu_category_history(MMI_CATEGORY22_ID, history_buffer);
    if (h_flag)
    {
        fixed_matrix_goto_item_no_redraw(MMI_fixed_matrix_menu.highlighted_item);
    }
    else
    {
        fixed_matrix_goto_item_no_redraw(highlighted_item);
    }
    MMI_disable_title_shortcut_display = 1;
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    gdi_layer_unlock_frame_buffer();

	ExitCategoryFunction=ExitCategory22Screen;
	RedrawCategoryFunction=dm_redraw_category_screen;
	GetCategoryHistory = dm_get_category_history;
	GetCategoryHistorySize = dm_get_category_history_size;
	dm_data.S32ScrId = (S32) GetActiveScreenId();
	dm_data.S32CatId = MMI_CATEGORY22_ID;
	dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
	dm_setup_data(&dm_data);
	dm_redraw_category_screen();

}   /* end of ShowCategory22Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory22Screen
 * DESCRIPTION
 *  Exits the category22 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory22Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
     
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_matrix();
}   /* end of ExitCategory22Screen */


/*****************************************************************************
 * FUNCTION
 *  GetCategory22HistorySize
 * DESCRIPTION
 *  Get category22 history size.
 * PARAMETERS
 *  void
 * RETURNS
 *  the size of history buffer
 *****************************************************************************/
S32 GetCategory22HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    // TODO: need to revise.
    return (20);
}   /* end of GetCategory22HistorySize */


/*****************************************************************************
 * FUNCTION
 *  GetCategory22History
 * DESCRIPTION
 *  Get category22 history.
 * PARAMETERS
 *  history_buffer      [IN]        History buffer
 * RETURNS
 *  hitsory buffer
 *****************************************************************************/
U8 *GetCategory22History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_matrix_menu_category_history(MMI_CATEGORY22_ID, history_buffer);
    return (history_buffer);
}   /* end of GetCategory22History */


/*****************************************************************************
 * FUNCTION
 *  category32_keyboard_key_handler
 * DESCRIPTION
 *  Key handler of category32 for WIN32
 * PARAMETERS
 *  vkey_code       [IN]        Type of key
 *  key_state       [IN]        State of key
 * RETURNS
 *  void
 *****************************************************************************/
void category32_keyboard_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                multiline_inputbox_previous_line();
                break;
            case 39:
                multiline_inputbox_next_line();
                break;
            case 38:
                fixed_list_goto_previous_item();
                break;
            case 40:
                fixed_list_goto_next_item();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}   /* end of category32_keyboard_key_handler */


/*****************************************************************************
 * FUNCTION
 *  category32_list_highlight_handler
 * DESCRIPTION
 *  high light handler of category32
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void category32_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    gdi_layer_lock_frame_buffer();
    MMI_list_highlight_handler(item_index);
    gdi_layer_unlock_frame_buffer();
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory32Screen
 * DESCRIPTION
 *  Displays the Dynamic menu screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items in the menu
 *  list_of_items           [IN]        Array of items
 *  list_of_icons           [IN]        Array of icons
 *  flags                   [IN]        Flags (see explanation below)
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory32Screen(
        U8 *title,
        PU8 title_icon,
        U8 *left_softkey,
        PU8 left_softkey_icon,
        U8 *right_softkey,
        PU8 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        PU8 *list_of_icons,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i, l, shortcut_width;
    S32 x = 24;
    U8 flag = 0;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(flags);
    gdi_layer_lock_frame_buffer();
    entry_full_screen();
    MMI_disable_title_shortcut_display = 1;
    MMI_menu_shortcut_number = -1;

    set_left_softkey_label((UI_string_type) left_softkey);
    set_left_softkey_icon(left_softkey_icon);
    set_right_softkey_label((UI_string_type) right_softkey);
    set_right_softkey_icon(right_softkey_icon);
    SetupCategoryKeyHandlers();

    shortcut_width = set_menu_item_count(number_of_items) + 7;
    create_fixed_icontext_menuitems();
    resize_fixed_list(MMI_content_width, UI_device_height - (MMI_title_height << 1) - MMI_button_bar_height);
    associate_fixed_icontext_list();
    for (i = 0; i < number_of_items; i++)
    {
        add_fixed_icontext_item((UI_string_type) list_of_items[i], list_of_icons[i]);
        if (list_of_icons[i] != UI_NULL_IMAGE)
        {
            flag = 1;
        }
    }
    register_fixed_list_keys();
    resize_fixed_icontext_menuitems(0, get_menu_item_height());
    if (flag)
    {
        set_fixed_icontext_positions(MMI_MENUITEM_HEIGHT + 2, 0, 1, 0);
    }
    else
    {
        set_fixed_icontext_positions(1, 0, 1, 0);
        MMI_fixed_icontext_menuitem.scroll_width = MMI_fixed_icontext_menuitem.width - 2;
    }

    register_fixed_list_highlight_handler(category32_list_highlight_handler);
    MMI_current_menu_type = LIST_MENU;
    h_flag = set_list_menu_category_history(MMI_CATEGORY32_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    MMI_title_string = (UI_string_type) title;
    wgui_set_animation_image(x >> 1, (MMI_title_y + MMI_title_height), (PU8) title_icon);
    if (title_icon == NULL)
    {
        x = 0;
    }

    l = gui_strlen(MMI_title_string);
    create_multiline_inputbox_set_buffer(MMI_title_string, l, l, 0);
    MMI_multiline_inputbox.flags |= (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE);  /* |UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND); */
    SetKeyHandler(multiline_inputbox_previous_line, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(multiline_inputbox_next_line, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    register_keyboard_key_handler(category32_keyboard_key_handler);
    disable_menu_shortcut_box_display = 1;
    register_fixed_list_shortcut_handler();
    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory32Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dm_get_category_history;
    GetCategoryHistorySize = dm_get_category_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY32_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory32Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory32Screen
 * DESCRIPTION
 *  Exits the dynamic menu screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory32Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    disable_menu_shortcut_box_display = 0;
#if(UI_BLINKING_CURSOR_SUPPORT)
    StopMyTimer(BLINKING_CURSOR);
#endif 
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
}   /* end of ExitCategory32Screen */


/*****************************************************************************
 * FUNCTION
 *  GetCategory32History
 * DESCRIPTION
 *  Get category32 history.
 * PARAMETERS
 *  history_buffer      [IN]        History buffer
 * RETURNS
 *  hitsory buffer
 *****************************************************************************/
U8 *GetCategory32History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history(MMI_CATEGORY32_ID, history_buffer);
    return (history_buffer);
}   /* end of GetCategory32History */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory36Screen
 * DESCRIPTION
 *  Displays the Dynamic radio list screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items
 *  list_of_items           [IN]        List of text items
 *  highlighted_item        [IN]        Zero based index of the highlighed item (used if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory36Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U16 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    create_fixed_twostate_menuitems(get_image(RADIO_ON_IMAGE_ID), get_image(RADIO_OFF_IMAGE_ID));
    associate_fixed_twostate_list();

    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    resize_fixed_twostate_menuitems(0, MMI_MENUITEM_HEIGHT);
    set_fixed_twostate_positions(MMI_MENUITEM_HEIGHT, 0, 1, 0);

    for (i = 0; i < number_of_items; i++)
    {
        add_fixed_twostate_item((UI_string_type) list_of_items[i]);
    }

    register_fixed_list_highlight_handler(standard_radio_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY36_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    select_fixed_twostate_item(MMI_fixed_list_menu.highlighted_item);
    resize_fixed_twostate_menuitems_to_list_width();

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitListCategoryScreen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dm_get_category_history;
    GetCategoryHistorySize = dm_get_category_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY36_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory36Screen */

/*****************************************************************************
* Category52ChangeItemText
*  GetCategory32History
*
* DESCRIPTION
*   change the text of menu item of particular index of category52.
*
* PARAMETERS
*  index IN item index
*  text  IN item text
*
* RETURNS
*  hitsory buffer
*
* GLOBALS AFFECTED
* 
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  Category52ChangeItemText
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 *  text        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void Category52ChangeItemText(S32 index, U8 *text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_icontext_menuitems[index].item_text = (UI_string_type) text;
}   /* end of Category52ChangeItemText */

/*****************************************************************************
* Category52ChangeItemText
*  Category52ChangeItemIcon
*
* DESCRIPTION
*   change the icon of menu item of particular index of category52
*
* PARAMETERS
*  index IN item index
*  image_ID IN item image_ID
*
* RETURNS
*  hitsory buffer
*
* GLOBALS AFFECTED
* 
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  Category52ChangeItemIcon
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index           [IN]        
 *  image_ID        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category52ChangeItemIcon(S32 index, U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_icontext_menuitems[index].item_icon = (PU8) get_image(image_ID);
}   /* end of Category52ChangeItemIcon */


/*****************************************************************************
 * FUNCTION
 *  Category52ChangeItemDescription
 * DESCRIPTION
 *  change the popup text of menu item of particular index of category52
 * PARAMETERS
 *  index       [IN]        Item index
 *  text        [IN]        Description text
 * RETURNS
 *  hitsory buffer(?)
 *****************************************************************************/
void Category52ChangeItemDescription(S32 index, U8 *text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_pop_up_description_strings[index].text_strings[0] = (UI_string_type) text;

#ifdef __MMI_UI_HINT_TOGGLE_TRANSITION__
    wgui_setup_pop_up_description_transition();
#endif 
}   /* end of Category52ChangeItemDescription */


/*****************************************************************************
 * FUNCTION
 *  Category52ChangeItemDescriptionNoAnimation
 * DESCRIPTION
 *  change the popup text of menu item of particular index of category52 but
 *  we do not start transition animation even if __MMI_UI_HINT_TOGGLE_TRANSITION__
 * PARAMETERS
 *  index       [IN]        Item index
 *  text        [IN]        Description text
 * RETURNS
 *  hitsory buffer(?)
 *****************************************************************************/
void Category52ChangeItemDescriptionNoAnimation(S32 index, U8 *text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_pop_up_description_strings[index].text_strings[0] = (UI_string_type) text;
}   /* end of Category52ChangeItemDescriptionNoAnimation */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory52Screen
 * DESCRIPTION
 *  Displays the category52 screen ( List menu with description )
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon displayed with the title
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Left softkey icon
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Right softkey icon
 *  number_of_items             [IN]        Number of items in the menu
 *  list_of_items               [IN]        Array of items
 *  list_of_icons               [IN]        Array of icons
 *  list_of_descriptions        [IN]        Array of Pop up description strings
 *  flags                       [IN]        Flags (see explanation below)
 *  highlighted_item            [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory52Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_items,
        U16 *list_of_icons,
        U8 **list_of_descriptions,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < number_of_items; i++)
    {
        subMenuDataPtrs[i] = (PU8) get_string(list_of_items[i]);
    }
    ShowCategory353Screen(
        (U8*) GetString(title),
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        subMenuDataPtrs,
        list_of_icons,
        list_of_descriptions,
        flags,
        highlighted_item,
        history_buffer);

}   /* end of ShowCategory52Screen */

/*****************************************************************************
* Category53ChangeItemText
*  Category53ChangeItemDescription
*
* DESCRIPTION
*   change the popup text of menu item of particular index of category53
*
* PARAMETERS
*  index IN item index
*  text  IN description text
*
* RETURNS
*  hitsory buffer
*
* GLOBALS AFFECTED
* 
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  Category53ChangeItemDescription
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 *  text        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void Category53ChangeItemDescription(S32 index, U8 *text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_pop_up_description_strings[index].text_strings[0] = (UI_string_type) text;

#ifdef __MMI_UI_HINT_TOGGLE_TRANSITION__
    wgui_setup_pop_up_description_transition();
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory53Screen
 * DESCRIPTION
 *  Displays the category53 screen ( List menu with description )
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon displayed with the title
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Left softkey icon
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Right softkey icon
 *  number_of_items             [IN]        Number of items in the menu
 *  list_of_items               [IN]        Array of items
 *  list_of_icons               [IN]        Array of icons
 *  list_of_descriptions        [IN]        Array of Pop up description strings
 *  flags                       [IN]        Flags (see explanation below)
 *  highlighted_item            [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory53Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U16 *list_of_icons,
        U8 **list_of_descriptions,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCategory353Screen(
        (U8*) GetString(title),
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        list_of_items,
        list_of_icons,
        list_of_descriptions,
        flags,
        highlighted_item,
        history_buffer);
}   /* end of ShowCategory53Screen */

void ExitCategory353Screen(void)
{
    close_scrolling_title();
	ExitListCategoryScreen();
}
/*****************************************************************************
 * FUNCTION
 *  ShowCategory353Screen
 * DESCRIPTION
 *  Displays the category353 screen ( List menu with description )
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon displayed with the title
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Left softkey icon
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Right softkey icon
 *  number_of_items             [IN]        Number of items in the menu
 *  list_of_items               [IN]        Array of items
 *  list_of_icons               [IN]        Array of icons
 *  list_of_descriptions        [IN]        Array of Pop up description strings
 *  flags                       [IN]        Flags (see explanation below)
 *  highlighted_item            [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory353Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U16 *list_of_icons,
        U8 **list_of_descriptions,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;
    UI_string_type s[10];
    S32 l, character_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    TRACE_GDI_FUNCTION();
    create_fixed_icontext_menuitems();
#ifdef __MMI_MAINLCD_220X176__
    resize_fixed_list(MMI_content_width, MMI_content_height);        
#endif	
    associate_fixed_icontext_list();

    ShowListCategoryScreen(
        (UI_string_type) title,
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    if (list_of_descriptions == NULL)
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_icontext_item((UI_string_type) list_of_items[i], wgui_get_list_menu_icon(i, list_of_icons[i]));
            wgui_pop_up_description_strings[i].text_strings[0] = NULL;
        }
    }
    else
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_icontext_item((UI_string_type) list_of_items[i], wgui_get_list_menu_icon(i, list_of_icons[i]));
            wgui_pop_up_description_strings[i].text_strings[0] = (UI_string_type) list_of_descriptions[i];
        }
    }
    h_flag = set_list_menu_category_history(MMI_CATEGORY52_ID, history_buffer);

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    if (list_of_descriptions != NULL)
    {
        S32 hints = 0;

        for (i = 0; i < number_of_items; i++)
        {
            if (wgui_pop_up_description_strings[i].text_strings[0])
            {
                hints = 1;
                break;
            }
        }
        if (hints)
        {
            if (is_set_force_icon_on_highlight_only_in_menuitem())
            {
                wgui_show_icon_only_highlight_in_icontext_menuitem();
            }
            if (is_set_force_all_hints_in_menuitem())
            {
                wgui_enable_hints_in_icontext_menuitem();
            }
            else if (is_set_force_hints_on_highlight_in_menuitem())
            {
                wgui_enable_hint_highlight_in_icontext_menuitem();
            }
            else
            {
                wgui_enable_hint_highlight_in_icontext_menuitem();
            }

            set_pop_up_descriptions(1, number_of_items, MMI_fixed_list_menu.highlighted_item);
        #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
            gui_block_list_effect();
        #endif 
        }
    }
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    MMI_title_string = (UI_string_type) title;
    MMI_title_icon = (PU8) get_image(title_icon);
    set_pop_up_descriptions(1, number_of_items, MMI_fixed_list_menu.highlighted_item);

#if defined(__PROJECT_GALLITE_C01__)
    setup_scrolling_title();

	gui_itoa(10, (UI_string_type) s, 10);
    gui_measure_string((UI_string_type) s, &l, &character_height);
	resize_menu_shortcut_handler(l, MMI_title_height);

#endif
    gdi_layer_unlock_frame_buffer();

#if defined(__PROJECT_GALLITE_C01__)
    ExitCategoryFunction = ExitCategory353Screen;//ExitListCategoryScreen;
#else
    ExitCategoryFunction = ExitListCategoryScreen;
#endif

    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY52_ID;
#if defined(__PROJECT_GALLITE_C01__)
    dm_data.S32flags = DM_SCROLL_TITLE;
#else
    dm_data.S32flags = 0;
#endif

    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory353Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory55Screen
 * DESCRIPTION
 *  Displays the category55 screen
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon shown with the title
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Icon for the left softkey
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Icon for the right softkey
 *  number_of_items             [IN]        Number of items
 *  list_of_items               [IN]        List of text items.
 *  list_of_descriptions        [IN]        
 *  highlighted_item            [IN]        This is the default item that will be highlighted (if there is no history)
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory55Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_items,
        U8 **list_of_descriptions,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < number_of_items; i++)
    {
        subMenuDataPtrs[i] = (PU8) get_string(list_of_items[i]);
    }
    ShowCategory6Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        subMenuDataPtrs,
        list_of_descriptions,
        highlighted_item,
        history_buffer);

}   /* end of ShowCategory55Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory56Screen
 * DESCRIPTION
 *  Displays the category56 screen
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon shown with the title
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Icon for the left softkey
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Icon for the right softkey
 *  number_of_items             [IN]        Number of items
 *  list_of_items               [IN]        List of text items.
 *  list_of_descriptions        [IN]        
 *  highlighted_item            [IN]        This is the default item that will be highlighted (if there is no history)
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory56Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U8 **list_of_descriptions,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCategory6Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        list_of_items,
        list_of_descriptions,
        highlighted_item,
        history_buffer);
}   /* end of ShowCategory56Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory59Screen
 * DESCRIPTION
 *  Displays the category59 screen ( confirm image with text )
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  message                 [IN]        Notification message
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  image(?)                [IN]        Notification image
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory59Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_reset_context();

    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_message_string = (UI_string_type) message;
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    dm_add_image(get_image(message_icon), NULL, NULL);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY);

    gdi_layer_unlock_frame_buffer();

    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY59_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory59Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory59Screen
 * DESCRIPTION
 *  Exits the category59 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory59Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
}   /* end of ExitCategory59Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory61Screen
 * DESCRIPTION
 *  Displays the category61 screen (Aphorisms screen)
 * PARAMETERS
 *  message             [IN]        Message string
 *  message_icon        [IN]        Message icon
 *  history_buffer      [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory61Screen(U8 *message, U16 message_icon, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l, width, height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_reset_context();
    UI_UNUSED_PARAMETER(history_buffer);

    gdi_layer_lock_frame_buffer();

    entry_full_screen();
    MMI_menu_shortcut_number = -1;
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();
    MMI_message_string = (UI_string_type) message;
    MMI_message_icon = (PU8) get_image(message_icon);
    gui_measure_image(MMI_message_icon, &width, &height);
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
        | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;
    dm_add_image(MMI_message_icon, NULL, NULL);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = MMI_dummy_function;
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY61_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory61Screen */

 
#ifdef __MMI_MAINLCD_240X320__
extern UI_filled_area wgui_pop_up_dialog_background;
#endif 


/*****************************************************************************
 * FUNCTION
 *  cat66_update_progress_string
 * DESCRIPTION
 *  Update status of progress for category66screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 extern UI_filled_area wgui_pop_up_dialog_background;
void cat66_update_progress_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_push_clip();
    gdi_layer_set_clip(
        MMI_multiline_inputbox.x,
        MMI_multiline_inputbox.y,
        MMI_multiline_inputbox.x + MMI_multiline_inputbox.width - 1,
        MMI_multiline_inputbox.y + MMI_multiline_inputbox.height - 1);
#ifdef __MMI_MAINLCD_240X320__
    gui_fill_rectangle(
        MMI_multiline_inputbox.x,
        MMI_multiline_inputbox.y,
        MMI_multiline_inputbox.x + MMI_multiline_inputbox.width - 1,
        MMI_multiline_inputbox.y + MMI_multiline_inputbox.height - 1,
        wgui_pop_up_dialog_background.c);
#else /* __MMI_MAINLCD_240X320__ */ 
#if defined(__MMI_MAINLCD_220X176__)
	gdi_image_draw_id(MMI_multiline_inputbox.x,
        			MMI_multiline_inputbox.y,
        			IMG_POPUP_BG);
#else
#ifdef __MMI_NOKIA_STYLE_N800__
    gui_fill_rectangle(
        MMI_multiline_inputbox.x,
        MMI_multiline_inputbox.y,
        MMI_multiline_inputbox.x + MMI_multiline_inputbox.width - 1,
        MMI_multiline_inputbox.y + MMI_multiline_inputbox.height - 1,
        wgui_pop_up_dialog_background.c);
#else
    gui_fill_rectangle(
        MMI_multiline_inputbox.x,
        MMI_multiline_inputbox.y,
        MMI_multiline_inputbox.x + MMI_multiline_inputbox.width - 1,
        MMI_multiline_inputbox.y + MMI_multiline_inputbox.height - 1,
	#ifdef __MMI_USING_BLACK_THEME_BG__
		gui_color(0,0,255));
	#else
        UI_COLOR_WHITE);
	#endif
#endif
#endif
#endif /* __MMI_MAINLCD_240X320__ */ 
    show_multiline_inputbox();
    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(
        MMI_multiline_inputbox.x,
        MMI_multiline_inputbox.y,
        MMI_multiline_inputbox.x + MMI_multiline_inputbox.width - 1,
        MMI_multiline_inputbox.y + MMI_multiline_inputbox.height - 1);
}   /* end of cat66_update_progress_string */

 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory66Screen
 * DESCRIPTION
 *  Displays the category66 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Notification message
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  image(?)                [IN]        Notification image
 * RETURNS
 *  void
 *****************************************************************************/
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
extern void	PopupChangeCharColor();
#endif
#ifdef __MMI_NOKIA_STYLE_N800__
 extern color_t black_color;
#endif

void ShowCategory66Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    U8 dm_screen_type = DM_TITLE_AREA_NORMAL;
    S32 l = 0;
    PU8 image;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_reset_context();

#if defined (__MMI_MAINLCD_240X320__) && defined(__MMI_UI_SMALL_SCREEN_SUPPORT__)
    set_small_screen();
    entry_full_screen();
#endif /* defined (__MMI_MAINLCD_240X320__) && defined(__MMI_UI_SMALL_SCREEN_SUPPORT__) */ 

    UI_UNUSED_PARAMETER(history_buffer);

    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();
    dm_data.S32flags = 0;
    if ((left_softkey == 0) && (left_softkey_icon == 0) && (right_softkey == 0) && (right_softkey_icon == 0))
    {
        dm_data.S32flags |= DM_NO_SOFTKEY;
    }
    else
    {
        dm_data.S32flags &= ~DM_NO_SOFTKEY;
        change_left_softkey(left_softkey, left_softkey_icon);
        change_right_softkey(right_softkey, right_softkey_icon);
        register_left_softkey_handler();
        register_right_softkey_handler();
        register_default_hide_softkeys();
    }
    if ((title == 0) && (title_icon == 0))
    {
        dm_screen_type = DM_TITLE_AREA_STATUS_ICON_ONLY;
        ShowStatusIconsTitle();
    }
    else if (!((title == 0xffff) && (title_icon == 0xffff)))
    {
        dm_screen_type = DM_TITLE_AREA_NORMAL;
        MMI_title_string = (UI_string_type) get_string(title);
        MMI_title_icon = (PU8) get_image(title_icon);
    }
    else
    {
        dm_screen_type = DM_TITLE_AREA_NONE;
    }
    MMI_message_string = (UI_string_type) message;
    image = (PU8) get_image(message_icon);
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
        | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;
    dm_add_image(image, NULL, NULL);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	PopupChangeCharColor();
#endif
#ifdef __MMI_NOKIA_STYLE_N800__
      MMI_multiline_inputbox.cursor_color=black_color;
#endif
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory66Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    switch (dm_screen_type)
    {
        case DM_TITLE_AREA_NONE:
            dm_data.S32CatId = MMI_CATEGORY66_NONE_ID;
            break;
        case DM_TITLE_AREA_STATUS_ICON_ONLY:
            dm_data.S32CatId = MMI_CATEGORY66_STATUS_ICON_ID;
            break;
        case DM_TITLE_AREA_NORMAL:
            dm_data.S32CatId = MMI_CATEGORY66_NORMAL_ID;
            break;
    }
#ifndef __MMI_MAINLCD_240X320__
    dm_data.S32flags |= DM_CLEAR_SCREEN_BACKGROUND;
#endif 
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory66Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory66Screen
 * DESCRIPTION
 *  Exits the category66 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory66Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined (__MMI_MAINLCD_240X320__) && defined(__MMI_UI_SMALL_SCREEN_SUPPORT__)
    reset_small_screen();
#endif 
    close_status_icons();
    gui_hide_animations();
    wgui_category_screen_no_buttons = 0;
    category_screen_layout_flags = 0;
}   /* end of ExitCategory66Screen */


/*****************************************************************************
 * FUNCTION
 *  wgui_hide_multitap
 * DESCRIPTION
 *  Hide function for multitap
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_hide_multitap(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    c.r = 255;
    c.g = 255;
    c.b = 255;
    c.alpha = 100;
    gui_reset_clip();
    gui_fill_rectangle(
        MMI_multitap_x,
        MMI_multitap_y,
        MMI_multitap_x + MMI_multitap_width + 1,
        MMI_multitap_y + MMI_multitap_height + 1,
        c);
}   /* end of wgui_hide_multitap */


/*****************************************************************************
 * FUNCTION
 *  SetCategory69RightSoftkeyFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  f       [IN]        
 *  k       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory69RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(k);
    wgui_singleline_inputbox_RSK_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  cat69_virtual_keypad_callback
 * DESCRIPTION
 *  Displays the category69 screen
 * PARAMETERS
 *  void
 *  BYTE(?)                                     [IN]        *        history_buffer    history buffer
 *  INPUT_TYPE_ALPHANUMERIC_SENTENCECASE(?)     [IN]        
 *  INPUT_TYPE(?)                               [IN]        Can take the following values:
 *  INPUT_TYPE_ALPHANUMERIC_LOWERCASE(?)        [IN]        
 *  INPUT_TYPE_NUMERIC(?)                       [IN]        
 *  below(?)                                    [IN]        
 *  INPUT_TYPE_ALPHANUMERIC_PASSWORD(?)         [IN]        (rare).
 *  INT(?)                                      [IN]        Buffer_size       Size of the buffer.
 *  INPUT_TYPE_ALPHANUMERIC_UPPERCASE(?)        [IN]        
 *  UI_image_ID_type (?)                              [IN]        Message,       Message
 *  INPUT_TYPE_NUMERIC_PASSWORD(?)              [IN]        
 *  For(?)                                      [IN]        Example, 5 means only 5 characters can be input in this screen.
 *  BUFFER(?)                                   [IN]        Buffer            Buffer the input box should use.
 * RETURNS
 *  void
 *****************************************************************************/
extern void ZiInuptMethodEnterSinglelineInputBox(void);
void cat69_virtual_keypad_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__MMI_TOUCH_SCREEN__)
    mmi_pen_editor_clear_and_show_virtual_keyboard_area();
#endif 
}

/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-17 14:54 */
#ifdef __MMI_GB__
extern void GbInuptMethodEnterSinglelineInputBox(void);
#endif
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-17 14:54 */

/*****************************************************************************
 * FUNCTION
 *  ShowCategory69Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        
 *  input_type              [IN]        
 *  buffer                  [?]         
 *  buffer_size             [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef __MMI_NOKIA_STYLE_N800__
 extern UI_single_line_input_box_theme PIN_singleline_inputbox_theme ;
 #endif
void ShowCategory69Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        S16 input_type,
        U8 *buffer,
        S32 buffer_size,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 h = 0, len;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(message);
    gdi_layer_lock_frame_buffer();
#ifdef __MMI_ZI__
    ZiInuptMethodEnterSinglelineInputBox();
#endif 

/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-17 14:55 */
#ifdef __MMI_GB__
	GbInuptMethodEnterSinglelineInputBox();
#endif
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-17 14:55 */

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    if (title == 0)
    {
        dm_data.S32flags = DM_NO_TITLE;
    }
    else
    {
        dm_data.S32flags = 0;
        MMI_title_string = get_string(title);
        MMI_title_icon = get_image(title_icon);
    }
    MMI_message_string = (UI_string_type) get_string(message);
    len = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, len, len, 0);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);
    wgui_setup_singleline_inputbox(
        4,
        MMI_multiline_inputbox.y + MMI_multiline_inputbox.height,
        UI_device_width - 8,
        h,
        buffer,
        buffer_size,
        MMI_CATEGORY111_ID,
        get_string(right_softkey),
        get_image(right_softkey_icon),
        input_type,
        history_buffer,
        0);
#ifdef __MMI_NOKIA_STYLE_N800__
        gui_set_single_line_input_box_theme(&MMI_singleline_inputbox, &PIN_singleline_inputbox_theme);
#endif		

    register_hide_multitap(wgui_hide_multitap);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory69Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY69_ID;
    dm_data.S32flags |= DM_CLEAR_SCREEN_BACKGROUND;
    dm_data.S32flags |= DM_SHOW_VKPAD;
    dm_register_vkpad_callback(cat69_virtual_keypad_callback);
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory69Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory69Screen
 * DESCRIPTION
 *  Exits the category69 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory69Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_MMI_current_input_type();
    wgui_switch_title_with_status_bar = 0;
    reset_softkeys();
    reset_multitaps();
    reset_multiline_inputbox();
    wgui_close_singleline_inputbox();
}   /* end of ExitCategory69Screen */

/* Temporary hardcoded filler for Pop up dialog screens  */
UI_filled_area bluewgui_color_dialog_background = {UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
    {159, 159, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};
//#ifdef __MMI_MENSTRUAL__
/* Only used in cat72(Menstrual) */
UI_filled_area greygui_pop_up_dialog_background = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    UI_NULL_IMAGE,
    NULL,
    {192, 192, 192, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};

/* Only used in cat72(Menstrual) */
UI_filled_area purplegui_pop_up_dialog_background = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    UI_NULL_IMAGE,
    NULL,
    {149, 43, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};

/* Only used in cat72(Menstrual) */
UI_filled_area whitegui_pop_up_dialog_background = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    UI_NULL_IMAGE,
    NULL,
    {255, 255, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};


/*****************************************************************************
 * FUNCTION
 *  DrawCate72CategoryControlArea
 * DESCRIPTION
 *  Draws the category72 category control area
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate72CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory72Screen();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory72Screen
 * DESCRIPTION
 *  Redraws the category 72 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory72Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x2 = 0, w = 0, h = 0;
    S8 ascii_percent[50];
    S8 uni_percent[100];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(ascii_percent, 0, 50);

    gdi_layer_reset_clip();

    if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
    {
        gdi_draw_solid_rect(
            0,
            (MMI_title_y + MMI_title_height),
            UI_device_width,
            (UI_device_height - MMI_button_bar_height - 1),
            GDI_COLOR_TRANSPARENT);
    }
    else
    {
        gui_draw_filled_area(
            0,
            (MMI_title_y + MMI_title_height),
            UI_device_width,
            (UI_device_height - MMI_button_bar_height - 1),
            &greygui_pop_up_dialog_background);
    }

    gui_draw_filled_area(
        wgui_image_clip_x1,
        wgui_image_clip_y1,
        wgui_image_clip_x2,
        wgui_image_clip_y2,
        &whitegui_pop_up_dialog_background);

    x2 = wgui_image_clip_x1 + no_of_pixel;

    gui_draw_filled_area(
        wgui_image_clip_x1,
        wgui_image_clip_y1,
        x2,
        wgui_image_clip_y2,
        &purplegui_pop_up_dialog_background);

    sprintf((PS8) ascii_percent, "%d", (S16) no_of_pixel);
    strcat((PS8) ascii_percent, (PS8) "%");
    gui_set_text_color(UI_COLOR_BLACK);

    gui_measure_string((UI_string_type) uni_percent, &w, &h);

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(((UI_device_width >> 1) - 10) + w, (wgui_image_clip_y2 + 5));
    }
    else
    {
        gui_move_text_cursor(((UI_device_width >> 1) - 10), (wgui_image_clip_y2 + 5));
    }

    memset(uni_percent, 0, 100);
    AnsiiToUnicodeString((PS8) uni_percent, (PS8) ascii_percent);
    gui_print_text((UI_string_type) uni_percent);

    show_multiline_inputbox();

}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory72Screen
 * DESCRIPTION
 *  Displays the category72 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Message string
 *  percentage_value        [IN]        Percentage value (0 to 100)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory72Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        S32 percentage_value,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l, fh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_message_string = (UI_string_type) message;
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);
    move_multiline_inputbox(0, (UI_device_height>>1)-20);
  //  move_multiline_inputbox(0, (MMI_title_y + MMI_title_height) + 2);
    resize_multiline_inputbox(MMI_multiline_inputbox.width, MMI_content_height);
    show_multiline_inputbox_no_draw();
    gui_set_font(MMI_multiline_inputbox.text_font);
    fh = get_multiline_inputbox_line_height();
    resize_multiline_inputbox(
        MMI_multiline_inputbox.width,
        (MMI_multiline_inputbox.n_lines * fh) + MULTILINE_INPUTBOX_HEIGHT_PAD + MMI_multiline_inputbox.text_y);

    wgui_image_clip_x1 = ((UI_device_width >> 1) - 50);
    wgui_image_clip_y1 = (U16) (MMI_multiline_inputbox.y + MMI_multiline_inputbox.height);
    wgui_image_clip_x2 = ((UI_device_width >> 1) + 50);
    wgui_image_clip_y2 = (wgui_image_clip_y1 + 20);

    no_of_pixel = percentage_value;

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory72Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY72_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawCate72CategoryControlArea);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory72Screen
 * DESCRIPTION
 *  Exits the category72 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory72Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
}
//#endif /* __MMI_MENSTRUAL__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory73Screen
 * DESCRIPTION
 *  Displays the category73 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items
 *  list_of_items           [IN]        List of text items
 *  list_of_icons1          [IN]        List for first column of icons
 *  list_of_icons2          [IN]        List for second column of icons
 *  highlighted_item        [IN]        Default item to be highlighted(if there is no history)
 *  history_buffer          [IN]        History buffer
 *  u8display_flag          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory73Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U16 *list_of_icons1,
        U16 *list_of_icons2,
        S32 highlighted_item,
        U8 *history_buffer,
        U8 u8display_flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 text_width;
    S32 i = 0, w1 = 0, w2 = 0, h1 = 0, h2 = 0, temp_w = 0, temp_h = 0;
    U8 h_flag;
    U8 *img = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    create_fixed_icontext_list_menuitems(1, 2);
    associate_fixed_icontext_list_list();

    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    resize_fixed_icontext_list_menuitems(0, MMI_MENUITEM_HEIGHT);

    text_width = (UI_device_width - MMI_fixed_list_menu.vbar.width - 57);
    img = (U8*) GetImage(list_of_icons1[0]);
    gui_measure_image(img, &w1, &h1);

    for (i = 1; i < number_of_items; i++)
    {
        img = (U8*) GetImage(list_of_icons1[i]);
        gui_measure_image(img, &temp_w, &temp_h);
        if (temp_w > w1)
        {
            w1 = temp_w;
        }
        if (temp_h > h1)
        {
            h1 = temp_h;
        }

    }

    if (w1 == 0)
    {
#if defined(__PROJECT_GALLITE_C01__)
        w1 = 0;
#else
        w1 = 9;
#endif
    }
    if (h1 == 0)
    {
        h1 = 18;
    }

    temp_w = temp_h = 0;

    img = (U8*) GetImage(list_of_icons2[0]);
    gui_measure_image(img, &w2, &h2);

    for (i = 1; i < number_of_items; i++)
    {
        img = (U8*) GetImage(list_of_icons2[i]);
        gui_measure_image(img, &temp_w, &temp_h);
        if (temp_w > w2)
        {
            w2 = temp_w;
        }
        if (temp_h > h2)
        {
            h2 = temp_h;
        }

    }

    switch (u8display_flag)
    {
        case ICON_TEXT_ICON:
            set_fixed_icontext_list_icon_coordinates(0, 2, 0, w1, h1);
            if ((w2 == 0) && (h2 == 0))
            {
                text_width = (UI_device_width - MMI_fixed_list_menu.vbar.width - (w1 + 10));
            }
             
            set_fixed_icontext_list_text_coordinates(0, (w1 + 5), 0, text_width, MMI_MENUITEM_HEIGHT);
             
            if ((w2 > 0) && (h2 > 0))
            {
                set_fixed_icontext_list_icon_coordinates(1, (w1 + 10 + text_width), 0, w2, h2);
            }
            break;

        case ICON_ICON_TEXT:
            if (w2 == 0)
            {
                w2 = 9;
            }
            if (h2 == 0)
            {
                h2 = 18;
            }
#if defined(__PROJECT_GALLITE_C01__)
            set_fixed_icontext_list_icon_coordinates(0, 1, 0, w1, h1);
            set_fixed_icontext_list_icon_coordinates(1, (w1 + 1), 0, w2, h2);
#else
            set_fixed_icontext_list_icon_coordinates(0, 2, 0, w1, h1);
            set_fixed_icontext_list_icon_coordinates(1, (w1 + 5), 0, w2, h2);
#endif

             
#if defined(__PROJECT_GALLITE_C01__)
            set_fixed_icontext_list_text_coordinates(
                0,
                (w1 + w2 + 12),
                0,
                UI_device_width - (w1 + w2 + 12) - MMI_fixed_list_menu.vbar.width,
                MMI_MENUITEM_HEIGHT);
             
#else
            set_fixed_icontext_list_text_coordinates(
                0,
                (w1 + w2 + 10),
                0,
                UI_device_width - (w1 + w2 + 10) - MMI_fixed_list_menu.vbar.width,
                MMI_MENUITEM_HEIGHT);
#endif

             
            break;

        case TEXT_ICON_ICON:
            if (w2 == 0)
            {
                w2 = 9;
            }
            if (h2 == 0)
            {
                h2 = 18;
            }
             
            set_fixed_icontext_list_text_coordinates(0, 2, 0, text_width, MMI_MENUITEM_HEIGHT);
             
            set_fixed_icontext_list_icon_coordinates(1, (text_width + 5), 0, w1, h1);
            set_fixed_icontext_list_icon_coordinates(0, (w1 + text_width + 10), 0, w2, h2);
            break;
    }

    for (i = 0; i < number_of_items; i++)
    {
        fixed_icontext_list_item_insert(i);
        add_fixed_icontext_list_item_text(i, 0, (UI_string_type) list_of_items[i]);
        add_fixed_icontext_list_item_icon(i, 0, get_image(list_of_icons1[i]));
        add_fixed_icontext_list_item_icon(i, 1, wgui_get_list_menu_icon_if_not_empty(i, list_of_icons2[i]));
    }

    MMI_fixed_icontext_list_menuitem.flags |= UI_MENUITEM_RIGHT_JUSTIFY;

    h_flag = set_list_menu_category_history(MMI_CATEGORY73_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitListCategoryScreen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY73_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ChangeCategory74MessageString
 * DESCRIPTION
 *  Change the content of string used in category74.
 * PARAMETERS
 *  new_string      [IN]        New content of string
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeCategory74MessageString(U8 *new_string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_multiline_inputbox.text = (UI_buffer_type) new_string;
    MMI_multiline_inputbox.cursor_p = new_string;
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    gui_show_multi_line_input_box(&MMI_multiline_inputbox);
    MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
}   /* end of ChangeCategory74MessageString */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory74Screen
 * DESCRIPTION
 *  Displays the category74 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  buffer                  [IN]        Buffer the input box should use
 *  buffer_size             [IN]        Size of the buffer
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
extern color_t black_color;
#endif 
#if defined(TGT_GALLITE_G800)
extern color_t white_color;
#endif

void ShowCategory74Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *buffer,
        S32 buffer_size,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;
    S16 input_type = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    l = gui_strlen((UI_string_type) buffer);
    create_multiline_inputbox_set_buffer((UI_string_type) buffer, buffer_size, l, 0);
    register_multiline_inputbox_viewer_keys();
    MMI_multiline_inputbox.flags |= (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE);
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
#if !defined(__MMI_USING_BLACK_THEME_BG__)
    MMI_multiline_inputbox.normal_text_color = black_color;
#endif	
#endif 
#if defined(TGT_GALLITE_G800)
    MMI_multiline_inputbox.normal_text_color = white_color;
#endif

    set_multiline_inputbox_category_history(MMI_CATEGORY74_ID, history_buffer, &input_type);
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory74Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY74_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory74Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory74Screen
 * DESCRIPTION
 *  Exits the category74 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory74Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_softkeys();
    reset_multitaps();
    reset_multiline_inputbox();
}   /* end of ExitCategory74Screen */

#ifdef __UI_PAGING_IN_MULTILINE_EDITOR__
#ifdef __MMI_TOUCH_SCREEN__
S32 MMI_content_height_prev;
S32 MMI_content_y_prev;
#endif 
/* for category 75 and 77 */
static U8 old_font_size = 0;
static S8 old_font_style = 0;
extern gui_page_info_app gui_page_info;

/*****************************************************************************
 * FUNCTION
 *  ShowCategory75Screen
 * DESCRIPTION
 *  Displays the category75 screen
 * PARAMETERS
 *  name                    [?]         
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  buffer                  [IN]        Buffer the input box should use
 *  buffer_size             [IN]        Size of the buffer
 *  history_buffer          [IN]        History buffer
 *  page_info_app           [IN]        
 *  title(?)                [IN]        Title for the screen
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory75Screen(
        U16 *name,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *buffer,
        S32 buffer_size,
        U8 *history_buffer,
        gui_page_info_app **page_info_app)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;
    S16 input_type = 0;
    gui_page_info_app *page_info_d = *page_info_app;
    dm_coordinates gui_multiline_get_coordinates;
    S32 shortcut_width;
//qiff modify for bug 9077
#if defined(__MMI_TOUCH_SCREEN__)
   // mmi_pen_handwriting_area_struct stroke_area;
   // S32 inputbox_y = (MMI_title_y + MMI_title_height);
  //  const gui_virtual_keyboard_pen_enum disable_backspace_list[] = {GUI_VKBD_PEN_BAKSPACE, GUI_VKBD_PEN_NONE};
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
#ifdef __MMI_TOUCH_SCREEN__
    wgui_paging_set_full_screen_flag(FALSE);
#endif 

//qiff modify for bug 9077
    dm_get_control_coordinates_from_category(
        -1,
        MMI_CATEGORY75_ID,
        DM_CATEGORY_CONTROLLED_AREA,
        -1,
        &gui_multiline_get_coordinates);
#ifdef __MMI_TOUCH_SCREEN__
 //   gui_multiline_get_coordinates.S16Height = gui_multiline_get_coordinates.S16Height - 4;
#endif 
    wgui_dm_set_multiline_data(&gui_multiline_get_coordinates);
    gui_lock_double_buffer();
    MMI_multiline_inputbox.jump_to_offset_flag = 0;
    MMI_menu_shortcut_number = -1;
    shortcut_width = set_menu_item_count(100)+7;
    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    MMI_disable_title_shortcut_display = 0;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) name;
    MMI_title_icon = (PU8) get_image(title_icon);

    //l = gui_strlen((UI_string_type) buffer);
    //create_multiline_inputbox_set_buffer((UI_string_type) buffer, buffer_size, l, 0);
//qiff modify for bug 9077
#if 0 //def __MMI_TOUCH_SCREEN__
    stroke_area.x1 = 0;
    stroke_area.x2 = 0 + MMI_content_width - MMI_fixed_list_menu.vbar.width;
    stroke_area.y1 = 0;
    stroke_area.y2 = inputbox_y;

    register_keyboard_input_handler(singleline_inputbox_numeric_keyboard_input_handler);
 //   mmi_pen_editor_set_vk_keys(MMI_FALSE, NULL, disable_backspace_list); //temp delete 20080521 by guojian
 #if 0  //temp delete 20080618 by guojian  ȥ����������vkeypad
    mmi_pen_editor_setup_input_box(
        &stroke_area,
        NULL,
        INPUT_TYPE_NUMERIC,
        MMI_FALSE,
        PEN_EDITOR_PAGING_IN_MULTLINE_EDITOR);
#endif

#endif /* __MMI_TOUCH_SCREEN__ */ 

    wgui_dm_set_multiline_data(&gui_multiline_get_coordinates);
    l = gui_strlen((UI_string_type) buffer);
    create_multiline_inputbox_set_buffer((UI_string_type) buffer, buffer_size, l, 0);
    wgui_paging_multiline_set_page(page_info_d->start_id, page_info_d->end_id);
    wgui_paging_multiline_set_font(page_info_d->font_size, page_info_d->font_style);
    wgui_paging_multiline_set_get_page_callback(page_info_d->get_page_cb);

    register_multiline_inputbox_viewer_keys();
    MMI_multiline_inputbox.flags |= (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE);  /* |UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND); */
    MMI_multiline_inputbox.flags &= (0x8080);
    if (page_info_d->highlight_string == 1)
    {
        MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_WORD_HIGHLIGHT;
        wgui_paging_highlight_search_string(page_info_d->jump_offset_from_app, page_info_d->search_str_len);
    }
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
#endif 
    MMI_multiline_inputbox.ext_flags |= GUI_MULTI_LINE_INPUT_BOX_VIEW_PAGING_MODE;
    MMI_multiline_inputbox.page_offset = page_info_d->text_offset;
    MMI_multiline_inputbox.jump_offset_from_app = page_info_d->jump_offset_from_app;
    MMI_multiline_inputbox.search_flag_is_on = page_info_d->search_flag_is_on;
    MMI_multiline_inputbox.size_of_total_data = page_info_d->doc_size;
    MMI_multiline_inputbox.override_direction = page_info_d->override_direction ? 2 : 1;
//qiff modify for bug 9077
    resize_multiline_inputbox(MMI_multiline_inputbox_width, MMI_multiline_inputbox_height); //chenhe delete for bug #9337 

    show_multiline_inputbox_no_draw();

    wgui_paging_set_scrollbar_range(page_info_d->doc_size);
//qiff modify for bug 9077
    set_multiline_inputbox_category_history(MMI_CATEGORY75_ID, history_buffer, &input_type);
    gui_unlock_double_buffer();

#ifdef __T800_MAIN_MENU__
    dm_set_scr_bg_image_opacity(170);
#endif

    ExitCategoryFunction = ExitCategory75Screen;
    wgui_paging_multiline_set_scroll_info(&page_info_d);
    wgui_paging_set_show_percentage_flag();
    if(old_font_size != page_info_d->font_size || old_font_style != page_info_d->font_style)
        gui_page_info.last_showed_percentage = 0;

    if (old_font_size == page_info_d->font_size && old_font_style == page_info_d->font_style)
    {
        if (page_info_d->text_offset != 0 || page_info_d->fixed_view_flag)
        {
            if (MMI_multiline_inputbox.text_offset_y != page_info_d->last_text_offset_y)
            {
                MMI_multiline_inputbox.text_offset_y = page_info_d->last_text_offset_y;
            }
        } 
    }
    if (page_info_d->text_offset)
    {
        wgui_paging_multiline_set_percentage(page_info_d->start_id, page_info_d->text_offset);
        if (MMI_multiline_inputbox.edit_height >= MMI_multiline_inputbox.text_height)
        {
            wgui_paging_multiline_set_percentage(page_info_d->start_id, 0);
        }
    }
    else
    {
        show_multiline_inputbox_no_draw();
        if (MMI_multiline_inputbox.search_flag_is_on)
        {
            show_multiline_inputbox_no_draw();
            wgui_paging_multiline_set_percentage(page_info_d->start_id, MMI_multiline_inputbox.text_offset);
        }
        else
        {
            wgui_paging_multiline_set_percentage(page_info_d->start_id, page_info_d->jump_offset_from_app);
        }
    }
    old_font_size = page_info_d->font_size;
    old_font_style = page_info_d->font_style;    
    if (page_info_d->jump_to_end == 0)
    {
        show_multiline_inputbox_no_draw();
        wgui_paging_jump_to_end();
        wgui_paging_multiline_inputbox_set_percentage_scale(COMPLETE_PERCENTAGE_SCALE);
    }
    if (page_info_d->text_offset != 0)
    {
        if (MMI_multiline_inputbox.text_offset_y != page_info_d->last_text_offset_y)
        {
            MMI_multiline_inputbox.text_offset_y = page_info_d->last_text_offset_y;
        }
    }
gdi_layer_unlock_frame_buffer();
#ifdef __MMI_TOUCH_SCREEN__
    dm_register_category_controlled_callback(RedrawCategoryControlledArea75Screen);
    wgui_register_category_screen_control_area_pen_handlers(wgui_paging_pen_down_hdlr, MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(wgui_paging_pen_up_hdlr, MMI_PEN_EVENT_UP);
    wgui_register_category_screen_control_area_pen_handlers(wgui_paging_pen_move_hdlr, MMI_PEN_EVENT_MOVE);
#endif /* __MMI_TOUCH_SCREEN__ */ 
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
#ifdef __MMI_TOUCH_SCREEN__
    dm_data.S32CatId = MMI_CATEGORY75_ID;
#else 
    dm_data.S32CatId = MMI_CATEGORY74_ID;
#endif 
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
#ifdef __MMI_TOUCH_SCREEN__
 //   dm_data.S32flags |= DM_SHOW_VKPAD;  //guojian deleted for jassmine 20080527 ����ʾvkpad
 //   dm_register_vkpad_callback(wgui_paging_virtual_keypad_callback);  //guojian deleted for jassmine 20080527 ����ʾvkpad
#endif /* __MMI_TOUCH_SCREEN__ */ 
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

    MMI_multiline_inputbox.page_offset = MMI_multiline_inputbox.jump_offset_from_app = 0;

    /* wgui_paging_multiline_set_scroll_info(&page_info_d); */
    if (page_info_d->scroll_flag == WGUI_PAGING_AUTO_SCROLL_ON && page_info_d->scroll_speed != 0)
    {
        wgui_paging_start_text_scroll();
    }
    else
    {
        wgui_paging_stop_text_scroll();
    }
    MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_WORD_HIGHLIGHT;
    
    if (page_info_d->scroll_type == WGUI_PAGING_SCROLL_LINE_BY_LINE)
    {
        SetKeyHandler(wgui_paging_multiline_inputbox_previous_line_in_paging, KEY_UP_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_next_line_in_paging_scroll, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_previous_line_in_paging, KEY_VOL_UP, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_next_line_in_paging_scroll, KEY_VOL_DOWN, KEY_EVENT_DOWN);

    }
    else
    {   
        SetKeyHandler(wgui_paging_multiline_inputbox_previous_page, KEY_UP_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_next_page, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_previous_page, KEY_VOL_UP, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_next_page, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    }
}   /* end of ShowCategory75Screen */

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  RedrawCategoryControlledArea75Screen
 * DESCRIPTION
 *  redraw function for category 75 in touch screen
 * PARAMETERS
 *  coordinate      [IN]     
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategoryControlledArea75Screen(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    redraw_multiline_inputbox();
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  ExitCategory75Screen
 * DESCRIPTION
 *  Exits the category75 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory75Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_softkeys();
    reset_multitaps();
    reset_multiline_inputbox();

    MMI_multiline_inputbox.previous_shared_bytes = 0;
    MMI_disable_title_shortcut_display = 1;
    wgui_paging_reset_show_percentage_flag();
    wgui_paging_stop_text_scroll();
    
#ifdef __T800_MAIN_MENU__
    dm_set_scr_bg_image_opacity(255);
#endif
//qiff modify for bug 9077
#if 0 //def __MMI_TOUCH_SCREEN__
    mmi_pen_editor_close_input_box();
#endif 
    MMI_multiline_inputbox.ext_flags &= ~GUI_MULTI_LINE_INPUT_BOX_VIEW_PAGING_MODE;
}   /* end of ExitCategory75Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory77Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  name                    [?]         
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  buffer                  [?]         
 *  buffer_size             [IN]        
 *  history_buffer          [?]         
 *  page_info_app           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory77Screen(
        U16 *name,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *buffer,
        S32 buffer_size,
        U8 *history_buffer,
        gui_page_info_app **page_info_app)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;
    S16 input_type = 0;
    gui_page_info_app *page_info_d = *page_info_app;
    dm_coordinates gui_multiline_get_coordinates;
#if 0 //defined(__MMI_TOUCH_SCREEN__)  //qiff remove for EBOOK don't support TP
    mmi_pen_handwriting_area_struct stroke_area;
    const gui_virtual_keyboard_pen_enum disable_backspace_list[] = {GUI_VKBD_PEN_BAKSPACE, GUI_VKBD_PEN_NONE};
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

#if 0 //def __MMI_TOUCH_SCREEN__  //qiff remove for EBOOK don't support TP
    S32 inputbox_y = MMI_title_y + MMI_title_height;
    wgui_paging_set_full_screen_flag(TRUE);
    gui_multiline_get_coordinates.S16X = 0;
    gui_multiline_get_coordinates.S16Y = 0;
    gui_multiline_get_coordinates.S16Width = MAIN_LCD_DEVICE_WIDTH;
    gui_multiline_get_coordinates.S16Height = MAIN_LCD_DEVICE_HEIGHT - MMI_title_y - 3;
    MMI_content_height_prev = MMI_content_height;
    MMI_content_y_prev = MMI_content_y;
    MMI_content_y = 0;
    MMI_content_height = MAIN_LCD_DEVICE_HEIGHT;
#else /* __MMI_TOUCH_SCREEN__ */ 
//qiff modify for bug 9077
#ifdef __MMI_TOUCH_SCREEN__
    dm_get_control_coordinates_from_category(
        -1,
        MMI_CATEGORY77_ID,
        DM_CATEGORY_CONTROLLED_AREA,
        -1,
        &gui_multiline_get_coordinates);
#else
    dm_get_control_coordinates_from_category(
        -1,
        MMI_CATEGORY77_ID,
        DM_MULTILINE_INPUTBOX1,
        -1,
        &gui_multiline_get_coordinates);
#endif
#endif /* __MMI_TOUCH_SCREEN__ */ 
    wgui_dm_set_multiline_data(&gui_multiline_get_coordinates);

    gui_lock_double_buffer();
    entry_full_screen();
    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 0;

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) name;
    MMI_title_icon = (PU8) get_image(title_icon);

    l = gui_strlen((UI_string_type) buffer);
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR;
    create_multiline_inputbox_set_buffer((UI_string_type) buffer, buffer_size, l, 0);
#if 0 //def __MMI_TOUCH_SCREEN__  //qiff remove for EBOOK don't support TP
    stroke_area.x1 = 0;
    stroke_area.x2 = 0 + MMI_content_width - MMI_fixed_list_menu.vbar.width;
    stroke_area.y1 = 0;
    stroke_area.y2 = inputbox_y;


    register_keyboard_input_handler(singleline_inputbox_numeric_keyboard_input_handler);
 //   mmi_pen_editor_set_vk_keys(MMI_FALSE, NULL, disable_backspace_list);   //temp delete 20080521 by guojian
    mmi_pen_editor_setup_input_box(
        &stroke_area,
        NULL,
        INPUT_TYPE_NUMERIC,
        MMI_FALSE,
        PEN_EDITOR_PAGING_IN_MULTLINE_EDITOR);

#endif /* __MMI_TOUCH_SCREEN__ */ 
    wgui_paging_multiline_set_page(page_info_d->start_id, page_info_d->end_id);
    wgui_paging_multiline_set_font(page_info_d->font_size, page_info_d->font_style);
    wgui_paging_multiline_set_get_page_callback(page_info_d->get_page_cb);
    register_multiline_inputbox_viewer_keys();
    MMI_multiline_inputbox.flags |= (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE);  /* |UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND); */
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
#endif 
    MMI_multiline_inputbox.ext_flags |= GUI_MULTI_LINE_INPUT_BOX_VIEW_PAGING_MODE;
    if (page_info_d->highlight_string == 1)
    {
        MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_WORD_HIGHLIGHT;
        wgui_paging_highlight_search_string(page_info_d->jump_offset_from_app, page_info_d->search_str_len);
    }
    /* MMI_multiline_inputbox.ext_flags |=GUI_MULTI_LINE_INPUT_BOX_FULL_SCREEN; */

    MMI_multiline_inputbox.page_offset = page_info_d->text_offset;
    MMI_multiline_inputbox.jump_offset_from_app = page_info_d->jump_offset_from_app;
    MMI_multiline_inputbox.override_direction = page_info_d->override_direction ? 2 : 1;
//qiff modify for bug 9077
MMI_multiline_inputbox.height = MAIN_LCD_DEVICE_HEIGHT;
MMI_multiline_inputbox.x = 0;
MMI_multiline_inputbox.y = 0;
resize_multiline_inputbox(MMI_multiline_inputbox_width, MMI_multiline_inputbox_height);

    show_multiline_inputbox_no_draw();
    wgui_paging_set_scrollbar_range(page_info_d->doc_size);
    wgui_paging_reset_show_percentage_flag();
    if (page_info_d->jump_to_end == 0)
    {
        wgui_paging_jump_to_end();
    }
    set_multiline_inputbox_category_history(MMI_CATEGORY77_ID, history_buffer, &input_type);
    gui_unlock_double_buffer();

#ifdef __T800_MAIN_MENU__
    dm_set_scr_bg_image_opacity(170);
#endif

    ExitCategoryFunction = ExitCategory77Screen;
    wgui_paging_multiline_set_scroll_info(&page_info_d);
    if (page_info_d->text_offset)
    {
        wgui_paging_multiline_set_percentage(page_info_d->start_id, page_info_d->text_offset);
    }
    else
    {
        show_multiline_inputbox_no_draw();
        if (MMI_multiline_inputbox.search_flag_is_on)
        {
            wgui_paging_multiline_set_percentage(page_info_d->start_id, MMI_multiline_inputbox.text_offset);
        }
        else
        {
            wgui_paging_multiline_set_percentage(page_info_d->start_id, page_info_d->jump_offset_from_app);
        }
    }
    if (page_info_d->text_offset != 0)
    {
        if (MMI_multiline_inputbox.text_offset_y != page_info_d->last_text_offset_y)
        {
            MMI_multiline_inputbox.text_offset_y = page_info_d->last_text_offset_y;
        }
    }
#ifdef __MMI_TOUCH_SCREEN__ 
    dm_register_category_controlled_callback(RedrawCategoryControlledArea75Screen);
    wgui_register_category_screen_control_area_pen_handlers(wgui_paging_pen_down_hdlr, MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(wgui_paging_pen_move_hdlr, MMI_PEN_EVENT_MOVE);
    wgui_register_category_screen_control_area_pen_handlers(wgui_paging_pen_up_hdlr, MMI_PEN_EVENT_UP);
#endif /* __MMI_TOUCH_SCREEN__ */ 
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY77_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
#if 0 //def __MMI_TOUCH_SCREEN__  //qiff remove for EBOOK don't support TP
   dm_data.S32flags |= DM_SHOW_VKPAD;  //guojian deleted for jassmine 20080527 ����ʾvkpad
    
 dm_register_vkpad_callback(wgui_paging_virtual_keypad_callback);

#endif /* __MMI_TOUCH_SCREEN__ */ 
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

    /* wgui_paging_multiline_set_scroll_info(&page_info_d); */

    MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_WORD_HIGHLIGHT;
    if (page_info_d->scroll_type == WGUI_PAGING_SCROLL_LINE_BY_LINE)
    {
        SetKeyHandler(wgui_paging_multiline_inputbox_previous_line_in_paging, KEY_UP_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_next_line_in_paging_scroll, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_previous_line_in_paging, KEY_VOL_UP, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_next_line_in_paging_scroll, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    }
    else
    {
        SetKeyHandler(wgui_paging_multiline_inputbox_previous_page, KEY_UP_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_next_page, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_previous_page, KEY_VOL_UP, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_paging_multiline_inputbox_next_page, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    }
    if (page_info_d->scroll_flag == WGUI_PAGING_AUTO_SCROLL_ON && page_info_d->scroll_speed != 0)
    {
        wgui_paging_start_text_scroll();
    }
    else
    {
        wgui_paging_stop_text_scroll();
    }

}   /* end of ShowCategory77Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory77Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory77Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_softkeys();
    reset_multitaps();
    reset_multiline_inputbox();
    leave_full_screen();
    MMI_multiline_inputbox.previous_shared_bytes = 0;
    MMI_disable_title_shortcut_display = 1;
    wgui_paging_stop_text_scroll();
    wgui_paging_reset_show_percentage_flag();
    
#ifdef __T800_MAIN_MENU__
    dm_set_scr_bg_image_opacity(255);
#endif    

#if 0 //def __MMI_TOUCH_SCREEN__ //qiff remove for EBOOK don't support TP
    MMI_content_height = MMI_content_height_prev;
    MMI_content_y = MMI_content_y_prev;
    mmi_pen_editor_close_input_box();
#endif /* __MMI_TOUCH_SCREEN__ */ 
    MMI_multiline_inputbox.ext_flags &= ~GUI_MULTI_LINE_INPUT_BOX_VIEW_PAGING_MODE;
}   /* end of ExitCategory77Screen */

#endif /* __UI_PAGING_IN_MULTILINE_EDITOR__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory76Screen
 * DESCRIPTION
 *  Displays the category74 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  buffer                  [IN]        Buffer the input box should use
 *  buffer_size             [IN]        Size of the buffer
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory76Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *buffer,
        S32 buffer_size,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;
    S16 input_type = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    l = gui_strlen((UI_string_type) buffer);
    create_multiline_inputbox_set_buffer((UI_string_type) buffer, buffer_size, l, 0);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE |
         UI_MULTI_LINE_INPUT_BOX_DRAW_SEPARATOR);
    register_multiline_inputbox_viewer_keys();

    set_multiline_inputbox_category_history(MMI_CATEGORY76_ID, history_buffer, &input_type);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory74Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY76_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory76Screen */


/*****************************************************************************
 * FUNCTION
 *  GetCategory76History
 * DESCRIPTION
 *  Gets the history buffer for category76 screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  Pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory76History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_multiline_inputbox_category_history(MMI_CATEGORY74_ID, history_buffer, MMI_current_input_type);
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  ChangeCategory78MessageString
 * DESCRIPTION
 *  
 * PARAMETERS
 *  new_string      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeCategory78MessageString(U8 *new_string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_multiline_inputbox.text = (UI_buffer_type) new_string;
    MMI_multiline_inputbox.cursor_p = new_string;
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    gui_show_multi_line_input_box(&MMI_multiline_inputbox);
    MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory78Screen
 * DESCRIPTION
 *  Displays the category78 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  icon_callback           [IN]        
 *  buffer                  [IN]        Buffer the input box should use.
 *  buffer_size             [IN]        Size of the buffer.
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory78Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        multi_line_input_box_icon_hdlr icon_callback,
        U8 *buffer,
        S32 buffer_size,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;
    S16 input_type = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    l = gui_strlen((UI_string_type) buffer);
    create_multiline_inputbox_set_buffer((UI_string_type) buffer, buffer_size, l, 0);
    MMI_multiline_inputbox.flags |= (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW |
                                     UI_MULTI_LINE_INPUT_BOX_RICHTEXT | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE);

#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
#endif 
 
    MMI_multiline_inputbox.icon_callback = icon_callback;
    register_multiline_inputbox_viewer_keys();
    set_multiline_inputbox_category_history(MMI_CATEGORY78_ID, history_buffer, &input_type);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory78Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY78_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory78Screen
 * DESCRIPTION
 *  Exits the category78 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory78Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_softkeys();
    reset_multitaps();
    reset_multiline_inputbox();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory105Screen
 * DESCRIPTION
 *  Displays the category105 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  volume_level            [?]         
 *  history_buffer          [IN]        History buffer
 *  volume_level_UI(?)      [IN]        Volume level
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory105Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 *volume_level,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

#if defined(__MMI_TOUCH_SCREEN__)
    U16 btn_up_id, btn_down_id;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_reset_context();
    UI_UNUSED_PARAMETER(history_buffer);

    gdi_layer_lock_frame_buffer();

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_menu_shortcut_number = -1;

    volume_level_UI = (*volume_level);
    if (volume_level_UI < 0)
    {
        volume_level_UI = 0;
    }
#ifdef __MMI_MAINLCD_220X176__		
    if (volume_level_UI > 9)
    {
        volume_level_UI = 9;
    }
#else
    if (volume_level_UI > 7)
    {
        volume_level_UI = 7;
    }
#endif
    dm_data.S32flags = 0;
    if ((left_softkey == 0) && (left_softkey_icon == 0) && (right_softkey == 0) && (right_softkey_icon == 0))
    {
        dm_data.S32flags |= DM_NO_SOFTKEY;
    }
    else
    {
        dm_data.S32flags &= ~DM_NO_SOFTKEY;
    }

     
#ifndef  __MMI_WALLPAPER_ON_BOTTOM__
#ifdef __MMI_USE_DM_VOLUME_LEVEL_DISPLAY__
	dm_add_image(get_image(WGUI_VOLUME_LEVEL0), NULL, NULL);
#else
    dm_add_image(get_image(WGUI_VOLUME_LEVEL0+ volume_level_UI), NULL, NULL);
#endif
#endif
     

    SetKeyHandler(show_vertical_bar_previous, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    SetKeyHandler(show_vertical_bar_next, KEY_VOL_UP, KEY_EVENT_DOWN);
    gdi_layer_unlock_frame_buffer();

     
#if defined(__MMI_TOUCH_SCREEN__)
    btn_up_id = dm_add_button(
                    NULL,
                    get_image(WGUI_VOLUME_UP_UP_IMAGE_ID),
                    get_image(WGUI_VOLUME_UP_DOWN_IMAGE_ID),
                    NULL);
    btn_down_id = dm_add_button(
                    NULL,
                    get_image(WGUI_VOLUME_DOWN_UP_IMAGE_ID),
                    get_image(WGUI_VOLUME_DOWN_DOWN_IMAGE_ID),
                    NULL);
#endif
 #if defined(__MMI_WALLPAPER_ON_BOTTOM__)&&!defined(__MMI_USE_DM_VOLUME_LEVEL_DISPLAY__)
    ExitCategoryFunction = ExitCategory105Screen;
#else /* defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
    ExitCategoryFunction = UI_dummy_function;
#endif /* defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
     

    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);

     
#if defined(__MMI_TOUCH_SCREEN__) //&& defined(__MMI_WALLPAPER_ON_BOTTOM__)
    dm_register_button_functions(btn_up_id, KEY_EVENT_UP, ResetCallbackTimer);
    dm_register_button_functions(btn_up_id, KEY_EVENT_DOWN, ShowVerticalBarUp);
    dm_register_button_functions(btn_up_id, KEY_REPEAT, CallShowVerticalBarMax);
    dm_register_button_functions(btn_down_id, KEY_EVENT_UP, ResetCallbackTimer);
    dm_register_button_functions(btn_down_id, KEY_EVENT_DOWN, ShowVerticalBarDown);
    dm_register_button_functions(btn_down_id, KEY_REPEAT, CallShowVerticalBarMin);
#endif /* defined(__MMI_TOUCH_SCREEN__) && defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
     

    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY105_ID;
    dm_data.S32flags |= DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
 #ifndef __MMI_USE_DM_VOLUME_LEVEL_DISPLAY__   
#if  defined(__MMI_WALLPAPER_ON_BOTTOM__)
    dm_get_scr_bg_image(&idle_scr_bg_ID, &idle_scr_bg_filename, &idle_scr_bg_x, &idle_scr_bg_y, &idle_scr_bg_opacity);
    dm_set_scr_bg_image((UI_image_ID_type ) (WGUI_VOLUME_LEVEL0 + volume_level_UI), NULL, -1, -1, 255);
    dm_get_scr_bg_flag(&idle_scr_bg_flags);//082906 scr_bg
    dm_set_scr_bg_flag(idle_scr_bg_flags | DM_SCR_BG_FORCE_SHOW);
#endif /* defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
#endif 
    dm_redraw_category_screen();

}

#if defined(__MMI_WALLPAPER_ON_BOTTOM__)


/*****************************************************************************
 * FUNCTION
 *  ExitCategory105Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory105Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_set_scr_bg_flag(idle_scr_bg_flags);//082906 scr_bg
    dm_set_scr_bg_image_no_draw(
        idle_scr_bg_ID,
        idle_scr_bg_filename,
        idle_scr_bg_x,
        idle_scr_bg_y,
        idle_scr_bg_opacity);
}
#endif /* defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 


/*****************************************************************************
 * FUNCTION
 *  change_cat105_image
 * DESCRIPTION
 *  
 * PARAMETERS
 *  img     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void change_cat105_image(PU8 img)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_change_image(img, NULL, 0);
}

/***************************************************************************** 
* Vertical_Bar Related Functions.
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  show_vertical_bar
 * DESCRIPTION
 *  Show volume vertical bar
 * PARAMETERS
 *  volume_level        [IN]        Volume level
 * RETURNS
 *  void
 *****************************************************************************/
void show_vertical_bar(S32 volume_level)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    volume_level_UI = volume_level;

    if (volume_level_UI < 0)
    {
        volume_level_UI = 0;
    }

#ifdef __MMI_MAINLCD_220X176__		
    if (volume_level_UI > 9)
    {
        volume_level_UI = 9;
    }
#else
    if (volume_level_UI > 7)
    {
        volume_level_UI = 7;
    }
#endif

     
#if defined(__MMI_WALLPAPER_ON_BOTTOM__)&&!defined(__MMI_USE_DM_VOLUME_LEVEL_DISPLAY__)
    dm_set_scr_bg_image((UI_image_ID_type ) (WGUI_VOLUME_LEVEL0 + volume_level_UI), NULL, -1, -1, 255);
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
#else /* defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
#ifndef __MMI_USE_DM_VOLUME_LEVEL_DISPLAY__
	change_cat105_image(get_image(WGUI_VOLUME_LEVEL0+volume_level_UI));
#endif
    
    RedrawCategoryFunction();
#endif /* defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 

}   /* end of show_vertical_bar */


/*****************************************************************************
 * FUNCTION
 *  show_vertical_bar_previous
 * DESCRIPTION
 *  Show volume vertical bar downgraded by 1
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_vertical_bar_previous(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_vertical_bar(--volume_level_UI);
}


/*****************************************************************************
 * FUNCTION
 *  show_vertical_bar_next
 * DESCRIPTION
 *  Show volume vertical bar upgraded by 1
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_vertical_bar_next(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_vertical_bar(++volume_level_UI);
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory120Screen
 * DESCRIPTION
 *  Exits the Network searching screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory120Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#if defined(__MMI_UI_STATUS_BAR_AT_BOTTOM__)
 
    //in_network_search_screen=0;
    reset_status_bar_at_bottom();
 
    set_status_icon_bar_clip(0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1);
    arrange_status_icons();
#endif /* defined(__MMI_UI_STATUS_BAR_AT_BOTTOM__) */ 
     
    close_main_LCD_dt_display();
    gui_hide_animations();
    reset_softkeys();
    register_hide_status_icon_bar(1, MMI_dummy_function);
    close_status_icons();
}


/*****************************************************************************
 * FUNCTION
 *  cat_120_hide_date_time_display
 * DESCRIPTION
 *  hide function of date/time of category120
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void cat_120_hide_date_time_display(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if !defined(__MMI_MAINLCD_160X128__)
    UI_filled_area *f = current_MMI_theme->network_screen_background_filler;
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(x1, y1, x2, y2);
#if defined(__MMI_MAINLCD_160X128__)
	gdi_draw_solid_rect(0, 0, UI_device_width - 1, UI_device_height - 1,GDI_COLOR_TRANSPARENT);
#else
    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, f);
#endif
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory120Screen
 * DESCRIPTION
 *  Displays the category120 screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Id of message to be displayed
 *  message_icon            [IN]        Id of message image to be displayed
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
 /*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
extern BOOL IsPopupSavedCharColor();
extern void PopupSaveCharColor();
extern void PopupChangeCharColor();
extern void PopupResetCharColor();
#endif

void ShowCategory120Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_reset_context();
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    set_main_LCD_dt_date_hide_function(cat_120_hide_date_time_display);
    set_main_LCD_dt_time_hide_function(cat_120_hide_date_time_display);
     
#if defined(__MMI_UI_STATUS_BAR_AT_BOTTOM__)
    set_dt_display(DT_IDLE_SCREEN_2);
 
    //in_network_search_screen=1;
    set_status_bar_at_bottom();
 
#else /* defined(__MMI_UI_STATUS_BAR_AT_BOTTOM__) */ 
    set_dt_display(DT_NETWORK_SEARCH);
#endif /* defined(__MMI_UI_STATUS_BAR_AT_BOTTOM__) */ 
     

    MMI_message_string = (UI_string_type) get_string(message);
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);
    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	PopupChangeCharColor();
#endif

     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    ShowStatusIconsTitle();
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
    register_hide_status_icon_bar(0, hide_status_icons_bar0);
    register_hide_status_icon_bar(1, hide_status_icons_bar1);
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    set_status_icon_bar_clip(
        0,
        0,
        UI_device_height - 1 - MMI_button_bar_height - MMI_status_bar_height,
        UI_device_width - 1,
        UI_device_height - 1 - MMI_button_bar_height - 1);
    set_status_icon_bar_clip(
        1,
        UI_device_width - 16,
        MMI_status_bar_height,
        UI_device_width - 1,
        UI_device_height - 1 - MMI_button_bar_height - MMI_status_bar_height - (STATUS_ICON_Y_GAP << 1) - 1);
    arrange_status_icons();
#else /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
    set_status_icon_bar_clip(0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1);
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
     
    show_status_icon_bar(0);
  //  show_status_icon_bar(1);
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory120Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY120_ID;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}

#ifdef __MMI_BMI__

 
/* For properly placing the strings, images and boxes, the screen is divided
   into blocks. */
#define NUMBER_OF_X_BLOCKS 7
#define NUMBER_OF_Y_BLOCKS 15
 

/* Only used in cat122(BMI) */
UI_filled_area pinkwgui_pop_up_dialog_background = {UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
    {255, 236, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};

/* Used in cat122(BMI) */
UI_filled_area yellowwgui_pop_up_dialog_background = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    UI_NULL_IMAGE,
    NULL,
    {249, 244, 171, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};

/* Only used in cat122(BMI) */
UI_filled_area orangewgui_without_boder_pop_up_dialog_background = {UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
    {242, 174, 33, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};


/*****************************************************************************
 * FUNCTION
 *  DrawCate122CategoryControlArea
 * DESCRIPTION
 *  Draws the category122 category control area
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate122CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory122Screen();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory122Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory122Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 fh, fw;
    S32 x1, y1, x2, y2;
    S32 block_size_x = MMI_content_width / NUMBER_OF_X_BLOCKS;
    S32 block_size_y = MMI_content_height / NUMBER_OF_Y_BLOCKS;
    S32 gap_x = (MMI_content_width % NUMBER_OF_X_BLOCKS) >> 1;
    S32 gap_y = (MMI_content_height % NUMBER_OF_Y_BLOCKS) >> 1;

    color_t c = *current_MMI_theme->title_text_color;
    color_t bc = *current_MMI_theme->title_text_border_color;//082606 calendar text
    stFontAttribute *f = &MMI_medium_font;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined( __MMI_MAINLCD_220X176__  )//wangmg
	c.r = c.g = c.b = 0;
	//bc.r = bc.g = bc.b = 0;
#endif
    gui_set_font(f);
    gui_set_text_color(c);
    gui_set_text_border_color(bc);

    gdi_layer_lock_frame_buffer();

    gdi_layer_push_clip();
    gdi_layer_reset_clip();

    wgui_show_transparent_animation();

    y1 = MMI_content_y + gap_y + block_size_y;
    y2 = y1 + (3 * block_size_y) - 1;

    gui_measure_image(_MMI_animated_icon, &fw, &fh);
    x1 = _MMI_animated_icon_x;


    if (fw > 0)
    {
        x1 += (fw + 10);
    }

    gui_measure_string(MMI_message_string, &fw, &fh);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + fw, y2 - fh);
    }
    else
    {
        gui_move_text_cursor(x1, y2 - fh);
    }

 //   gui_print_text(MMI_message_string);
 //   gui_print_bordered_text(MMI_message_string);    modified by renyh for greenstone 2008/08/04

 //   x1 += fw + 5;  modified by renyh for greenstone 2008/08/04
    x1 = MMI_content_x + gap_x + (block_size_x * 3.5);    //add by renyh for greenstone 2008/08/04

    gui_measure_string((UI_string_type) disp_barstr, &fw, &fh);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1+ fw, y2 - fh);
    }
    else
    {
        gui_move_text_cursor(x1 - fw/2 , y2 - fh);    //modified by renyh for greenstone 2008/08/04
    }

//    gui_print_text((UI_string_type) disp_barstr);
    gui_print_bordered_text((UI_string_type) disp_barstr);

    x1 = MMI_content_x + gap_x + (block_size_x / 2);
    y1 = y2 + (block_size_y);
    x2 = x1 + (6 * block_size_x) - 1;
    y2 = y1 + (3 * block_size_y) - 1;

    gui_draw_filled_area(
        x1 + (2 * block_size_x) - 5,
        y1,
        x2 - (2 * block_size_x) + 5,
        y2,
        &yellowwgui_pop_up_dialog_background);

    gui_measure_string((UI_string_type) disp_valuestr, &fw, &fh);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + (3 * block_size_x) + (fw >> 1), ((y1 >> 1) + (y2 >> 1)) - (fh >> 1));
    }
    else
    {
        gui_move_text_cursor(x1 + (3 * block_size_x) - (fw >> 1), ((y1 >> 1) + (y2 >> 1)) - (fh >> 1));
    }

    //gui_print_text((UI_string_type) disp_valuestr);
    gui_print_bordered_text((UI_string_type) disp_valuestr);

    x1 = MMI_content_x + gap_x + (block_size_x / 2);
    y1 = MMI_content_y + gap_y + (9 * block_size_y);
    x2 = x1 + (6 * block_size_x);
    y2 = y1 + (3 * block_size_y) - 1;

    gui_draw_filled_area(x1, y1, x2, y2, &yellowwgui_pop_up_dialog_background);
    //gui_draw_vertical_line(y1, y2, x1 + (2 * block_size_x), c);
    //gui_draw_vertical_line(y1, y2, x1 + (4 * block_size_x), c);
    gui_draw_vertical_line(y1, y2, x1 + (2 * block_size_x), yellowwgui_pop_up_dialog_background.border_color);//082606 calendar line
    gui_draw_vertical_line(y1, y2, x1 + (4 * block_size_x), yellowwgui_pop_up_dialog_background.border_color);

    /* wgui_image_clip_x1 stores the percentage value for deciding the position
       of highlighted block. */
    x1 = x1 + (wgui_image_clip_x1 * (2 * block_size_x)) + 1;
    x2 = x1 + (2 * block_size_x) - 2;
    gui_draw_filled_area(x1, y1 + 1, x2, y2 - 1, &orangewgui_without_boder_pop_up_dialog_background);

    x1 = MMI_content_x + gap_x + (block_size_x / 2);
    y1 = y1 + (4 * block_size_y);
    x2 = x1 + (6 * block_size_x) - 1;
    y2 = y1 + block_size_y;

    gui_measure_string((UI_string_type) left_value, &fw, &fh);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + (2 * block_size_x) + (fw >> 1), y1);
    }
    else
    {
        gui_move_text_cursor(x1 + (2 * block_size_x) - (fw >> 1), y1);
    }

    //gui_print_text((UI_string_type) left_value);
    gui_print_bordered_text((UI_string_type) left_value);

    gui_measure_string((UI_string_type) right_value, &fw, &fh);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + (4 * block_size_x) + (fw >> 1), y1);
    }
    else
    {
        gui_move_text_cursor(x1 + (4 * block_size_x) - (fw >> 1), y1);
    }

    //gui_print_text((UI_string_type) right_value);
    gui_print_bordered_text((UI_string_type) right_value);

    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory122Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        
 *  percentage_value        [IN]        
 *  history_buffer          [?]         
 *  disp_val                [?]         
 *  bar_status              [?]         
 *  side_icon               [IN]        
 *  left_limit              [?]         
 *  right_limit             [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory122Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        S32 percentage_value,
        U8 *history_buffer,
        S8 *disp_val,
        U8 *bar_status,
        U16 side_icon,
        U8 *left_limit,
        U8 *right_limit)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 fw, fh;
    S32 total_width = 0;
    //fix bug-11838 of CS2324B Project on 20090601 by Liuxl
    //replace total_width with following two variables to get the centeral position of side icon 
    S32 fw_disp_barstr = 0, fw_side_icon = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);

    MMI_menu_shortcut_number = -1;
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    MMI_message_string = (UI_string_type) get_string(message);

    disp_valuestr = (PU8) disp_val;
    disp_barstr = (PU8) bar_status;

    /* store the percentage value in Global variable, so that is can be used
       in the redraw category. */
    wgui_image_clip_x1 = percentage_value;

    left_value = (PU8) left_limit;
    right_value = (PU8) right_limit;

    gui_measure_string(MMI_message_string, &fw, &fh);
    total_width += fw;
    gui_measure_string((UI_string_type) disp_barstr, &fw, &fh);
    fw_disp_barstr = fw;
    total_width += fw;
    gui_measure_image((PU8) get_image(side_icon), &fw, &fh);
    fw_side_icon = fw;
    total_width += fw;

    /* add gaps between image and strings */
    /*
    if (fw > 0)
    {
        total_width += (10 + 5);
    }
    else
    {
        total_width += 5;
    }
    */

    //fix bug-11838 of CS2324B Project on 20090601 by Liuxl
    //(UI_device_width >> 1)--the centeral position of screen, (fw_disp_barstr >> 1)--the half width of disp_barstr, (fw_side_icon >> 1)-- the half width of side icon, 10--gaps between side icon and disp_barstr 
    //the centeral position of side position=the centeral position of screen -the half width of disp_barstr -gaps between side icon and disp_barstr - the half width of side icon 
    wgui_set_animation_y(
        (UI_device_width >> 1) - (fw_disp_barstr >> 1) - 10 - (fw_side_icon >> 1) ,
        MMI_content_y + ((MMI_content_height % NUMBER_OF_Y_BLOCKS) >> 1) + (4 * (MMI_content_height / NUMBER_OF_Y_BLOCKS)) - 1 - fh,
        side_icon);

    ExitCategoryFunction = ExitCategory122Screen;
    ExitCategoryFunction = UI_dummy_function;
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_register_category_controlled_callback(DrawCate122CategoryControlArea);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY122_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}

 


/*****************************************************************************
 * FUNCTION
 *  ExitCategory122Screen
 * DESCRIPTION
 *  Exits the category122 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory122Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
    reset_softkeys();
}
#endif /* __MMI_BMI__ */ 

#ifdef __MMI_CALORIE__
#endif /* __MMI_CALORIE__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory184Screen
 * DESCRIPTION
 *  Displays the category184 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items to be displayed
 *  get_item_func           [IN]        Function pointer to get next displayed items
 *  get_hint_func           [IN]        Function pointer to get next displayed hint data
 *  highlighted_item        [IN]        Index of item to be highlighted
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory184Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        GetItemFuncPtr get_item_func,
        GetHintFuncPtr get_hint_func,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCategory185Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        get_item_func,
        get_hint_func,
        highlighted_item,
        MMI_FALSE,
        UI_dummy_function,
        history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory185Screen
 * DESCRIPTION
 *  Similar to Category 184 but data caching is disabled in dynamic list menu.
 *  
 *  Designed for JAM (Java application manager) because Java adopts special architecture.
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  number_of_items         [IN]        
 *  get_item_func           [IN]        
 *  get_hint_func           [IN]        
 *  highlighted_item        [IN]        
 *  disable_data_cache      [IN]        Whether caching of dynamic data is disabled
 *  flush_data_callback     [IN]        Callback function invoked before data is reloaded
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory185Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        GetItemFuncPtr get_item_func,
        GetHintFuncPtr get_hint_func,
        S32 highlighted_item,
        MMI_BOOL disable_data_cache,
        void (*flush_data_callback) (void),
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 shortcut_width;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_menu_shortcut_number = -1;

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    create_fixed_icontext_menuitems();

    MMI_current_menu_type = LIST_MENU;

    resize_fixed_list(MMI_content_width, MMI_content_height);
    associate_fixed_icontext_list();

    if (disable_data_cache)
    {
        MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_CACHE_DYNAMIC_DATA;
    }

    init_dynamic_item_buffer(number_of_items, get_item_func, get_hint_func, flush_data_callback);
    MMI_fixed_list_menu.n_items = number_of_items;

    register_dynamic_list_shortcut_handler();
    register_dynamic_list_keys();
    shortcut_width = set_menu_item_count(number_of_items) + 7;
    resize_menu_shortcut_handler(shortcut_width, (MMI_title_height));
    move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
    register_hide_menu_shortcut(draw_title);
    resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);
#ifdef __MMI_NOKIA_STYLE_N800__
    set_fixed_icontext_positions(MMI_MENUITEM_HEIGHT + 2, 0, 1, 0);
#else
#if defined(__PROJECT_GALLITE_C01__) 
    set_fixed_icontext_positions(MMI_ICONTEXT_MENUITEM_HEIGHT + 2 + 6, 0, 1, 0);
#else
    set_fixed_icontext_positions(MMI_ICONTEXT_MENUITEM_HEIGHT + 2, 0, 1, 0);
#endif

#endif

    register_fixed_list_highlight_handler(dynamic_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY184_ID, history_buffer);

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    if (get_hint_func)
    {
        if (is_set_force_icon_on_highlight_only_in_menuitem())
        {
            wgui_show_icon_only_highlight_in_icontext_menuitem();
        }
        if (is_set_force_all_hints_in_menuitem())
        {
            wgui_enable_hints_in_icontext_menuitem();
        }
        else if (is_set_force_hints_on_highlight_in_menuitem())
        {
            wgui_enable_hint_highlight_in_icontext_menuitem();
        }
        else
        {
            wgui_enable_hint_highlight_in_icontext_menuitem();
        }

        set_dynamic_pop_up_descriptions(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);      /* ????? */
    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        gui_block_list_effect();     
    #endif 
    }
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
    dynamic_list_goto_item_no_redraw(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);
    set_dynamic_pop_up_descriptions(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);  /* ????? */

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory184Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY184_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory184Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory184Screen
 * DESCRIPTION
 *  Exits category 184 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory184Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_dynamic_list();
    reset_pop_up_descriptions();

    dynamic_item_text_align_left = FALSE;

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    reset_all_force_flags_for_hints();
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

}

/*****************************************************************************
* Category 284
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowCategory284Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  number_of_items         [IN]        
 *  get_item_func           [IN]        
 *  get_hint_func           [IN]        
 *  highlighted_item        [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory284Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        GetItemFuncPtr get_item_func,
        GetHintFuncPtr get_hint_func,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dynamic_item_text_align_left = TRUE;
    ShowCategory184Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        get_item_func,
        get_hint_func,
        highlighted_item,
        history_buffer);
}

/*****************************************************************************
* Category 186
* Dynamic list menu supporting on/off toggling
*****************************************************************************/

/* Context of category 186 */
typedef struct
{
    /* Array of 0-1 states */
    U8 *states;
    /* callback function to get text  */
      pBOOL(*get_text_callback) (S32 item_index, UI_string_type str_buff);
    /* Whether data is modified */
    MMI_BOOL modified;
} wgui_cat186_context_struct;

/* Context pointer of category 186 */
static wgui_cat186_context_struct *wgui_cat186_p;

/* Function Pointer to save the extra LSK handler function for Category187 */
FuncPtr wgui_cat_187_lsk_handler = NULL;

/*****************************************************************************
 * FUNCTION
 *  Category186GetItem
 * DESCRIPTION
 *  Common dynamic item data callback for Category 186
 * PARAMETERS
 *  item_index          [IN]        Menu item index
 *  str_buff            [IN]        Text buffer
 *  img_buff_p          [IN]        Pointer to image buffer
 *  str_img_mask        [IN]        Mask (unused)
 * RETURNS
 *  pBOOL
 *****************************************************************************/
pBOOL Category186GetItem(S32 item_index, UI_string_type str_buff, PU8 *img_buff_p, U8 str_img_mask)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(wgui_cat186_p);
    UI_UNUSED_PARAMETER(str_img_mask);

    if (wgui_cat186_p->states[item_index])
    {
        *img_buff_p = get_image(CHECKBOX_ON_IMAGE_ID);
    }
    else
    {
        *img_buff_p = get_image(CHECKBOX_OFF_IMAGE_ID);
    }

    return wgui_cat186_p->get_text_callback(item_index, str_buff);
}


/*****************************************************************************
 * FUNCTION
 *  Category186ToggleCheckBox
 * DESCRIPTION
 *  Toggle on/off of the current highlighted checkbox
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Category186ToggleCheckBox(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 index = GetHighlightedItem();
    U8 state;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(wgui_cat186_p);

    if (wgui_cat186_p->states[index])
    {
        state = wgui_cat186_p->states[index] = 0;
    }
    else
    {
        state = wgui_cat186_p->states[index] = 1;
    }

    if (!wgui_cat186_p->modified)
    {
        wgui_cat186_p->modified = MMI_TRUE;
        ChangeRightSoftkey(WGUI_CATEGORY_DONE_STRING_ID, 0);
        set_right_softkey_function(checkbox_toggle_DONE_RSK_function, KEY_EVENT_UP);
    }

    /* Reload data & invoke list highlight handler */
    dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);

    show_dynamic_list();
    
    if(wgui_cat_187_lsk_handler != NULL)
    {
        wgui_cat_187_lsk_handler();
    }
}


/*****************************************************************************
 * FUNCTION
 *  Category186ListHighlightHdlr
 * DESCRIPTION
 *  Exit category function for category 186
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category186ListHighlightHdlr(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(wgui_cat186_p);

    if (wgui_cat186_p->states[item_index])
    {
        ChangeLeftSoftkey(WGUI_CATEGORY_OFF_STRING_ID, 0);
    }
    else
    {
        ChangeLeftSoftkey(WGUI_CATEGORY_ON_STRING_ID, 0);
    }

    dynamic_list_highlight_handler(item_index);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory186History
 * DESCRIPTION
 *  Get history data of category 186
 * PARAMETERS
 *  history_buffer      [IN]        Buffer to store history data
 * RETURNS
 *  void
 *****************************************************************************/
U8 *GetCategory186History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(wgui_cat186_p);

    if (wgui_cat186_p->modified)
    {
        *((S32*) history_buffer) = 1;
    }
    else
    {
        *((S32*) history_buffer) = 0;
    }
    get_list_menu_category_history(MMI_CATEGORY186_ID, history_buffer + sizeof(S32));
    return history_buffer;
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory186HistorySize
 * DESCRIPTION
 *  Get history data of category 186
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 GetCategory186HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return sizeof(S32) + get_list_menu_category_history_size();
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory186History
 * DESCRIPTION
 *  Set history data of category 186
 * PARAMETERS
 *  history_buffer      [IN]        Buffer to store history data
 * RETURNS
 *  Whether the operation succeeds
 *****************************************************************************/
U8 SetCategory186History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(wgui_cat186_p);

    if (history_buffer)
    {
        if (*((S32*) history_buffer))
        {
            wgui_cat186_p->modified = MMI_TRUE;
        }
        else
        {
            wgui_cat186_p->modified = MMI_FALSE;
        }
        return set_list_menu_category_history(MMI_CATEGORY186_ID, history_buffer + sizeof(S32));
    }
    else
    {
        return 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory186Screen
 * DESCRIPTION
 *  Exit category function for category 186
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory186Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(wgui_cat186_p);

    ExitCategory184Screen();

    OslMfree(wgui_cat186_p);
    wgui_cat186_p = NULL;
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  Category186MenuItemTranslatePenEvent
 * DESCRIPTION
 *  Pen event handler for menu item because we want to emulate twostate menu item
 *  with icontext menu item
 *  
 *  Reference: gui_fixed_twostate_menuitem_translate_pen_event()
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [?]         
 *  item_x                  [IN]        
 *  item_y                  [IN]        
 *  pen_event               [IN]        
 *  x                       [IN]        
 *  y                       [IN]        
 *  menuitem_event          [?]         
 *  menuitem_param          [?]         
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL Category186MenuItemTranslatePenEvent(
        void *item,
        void *common_item_data,
        S32 item_x,
        S32 item_y,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_list_pen_enum *menuitem_event,
        gui_pen_event_param_struct *menuitem_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret = MMI_TRUE, pen_inside = MMI_FALSE;
    S32 iwidth, iheight;

    fixed_icontext_menuitem *m = (fixed_icontext_menuitem*) common_item_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);

    *menuitem_event = GUI_LIST_PEN_NONE;
    GUI_PEN_EVENT_PARAM_SET_VOID(menuitem_param);

    gui_measure_image((PU8) get_image(CHECKBOX_OFF_IMAGE_ID), &iwidth, &iheight);

    item_x += m->icon_x;
    item_y += m->icon_y;
    if (PEN_CHECK_BOUND(x, y, item_x, item_y, iwidth, iheight))
    {
        pen_inside = MMI_TRUE;
    }

    if (pen_event == MMI_PEN_EVENT_DOWN)
    {
        if (!pen_inside)
        {
            /* The following pen events will not be directed to twostate menuitem */
            ret = MMI_FALSE;
        }
    }
    else if (pen_event == MMI_PEN_EVENT_UP)
    {
        if (pen_inside)
        {
            /* Outside of the image, it is not controlled by menuitem */
            *menuitem_event = GUI_TWOSTATE_PEN_TOGGLE;
        }
    }

    return ret;
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory186Screen
 * DESCRIPTION
 *  Displays the category186 screen. A dynamic menu handling on/off toggling.
 *  Use SetCheckboxToggleRightSoftkeyFunctions() *before* this function.
 *  
 *  Reference: ShowCategory185Screen(), ShowCategory140Screen()
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  number_of_items         [IN]        Number of items to be displayed
 *  get_text_callback       [IN]        Callback function to get the text of menu item
 *  get_hint_func           [IN]        Function pointer to get next displayed hint data
 *  list_of_states          [IN]        On/off states
 *  highlighted_item        [IN]        Index of item to be highlighted
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory186Screen(
        U16 title,
        U16 title_icon,
        S32 number_of_items,
        pBOOL(*get_text_callback) (S32 item_index, UI_string_type str_buff),
        GetHintFuncPtr get_hint_func,
        U8 *list_of_states,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	ShowCategory187Screen(
        title,
        title_icon,
        number_of_items,
        get_text_callback,
        get_hint_func,
        list_of_states,
        highlighted_item,
		NULL,
        history_buffer);
}
/*****************************************************************************
 * FUNCTION
 *  ShowCategory187Screen
 * DESCRIPTION
 *  Displays the category187 screen. A dynamic menu handling on/off toggling.
 *  Use SetCheckboxToggleRightSoftkeyFunctions() *before* this function.
 *  
 *  Reference: ShowCategory185Screen(), ShowCategory140Screen()
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  number_of_items         [IN]        Number of items to be displayed
 *  get_text_callback       [IN]        Callback function to get the text of menu item
 *  get_hint_func           [IN]        Function pointer to get next displayed hint data
 *  list_of_states          [IN]        On/off states
 *  highlighted_item        [IN]        Index of item to be highlighted
 *  lsk_handler_func        [IN]        This is an extra lsk handler.Needs to be registered to add extra functionality along with toggle.
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory187Screen(
        U16 title,
        U16 title_icon,
        S32 number_of_items,
        pBOOL(*get_text_callback) (S32 item_index, UI_string_type str_buff),
        GetHintFuncPtr get_hint_func,
        U8 *list_of_states,
        S32 highlighted_item,
		FuncPtr lsk_handler_func,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 shortcut_width;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Setup context */
    if (!wgui_cat186_p)
    {
        wgui_cat186_p = OslMalloc(sizeof(wgui_cat186_context_struct));
    }
    wgui_cat186_p->get_text_callback = get_text_callback;
    wgui_cat186_p->states = list_of_states;
    wgui_cat186_p->modified = MMI_FALSE;

    /* Draw screen */
    gdi_layer_lock_frame_buffer();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_menu_shortcut_number = -1;

    change_left_softkey(STR_GLOBAL_ON, 0);
    change_right_softkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    SetupCategoryKeyHandlers();

    create_fixed_icontext_menuitems();

    MMI_current_menu_type = LIST_MENU;

    resize_fixed_list(MMI_content_width, MMI_content_height);
    associate_fixed_icontext_list();
#ifdef __MMI_TOUCH_SCREEN__
    gui_set_fixed_list_menu_item_pen_function(
        &MMI_fixed_list_menu,
        Category186MenuItemTranslatePenEvent,
        MMI_FALSE,
        MMI_TRUE);
#endif /* __MMI_TOUCH_SCREEN__ */ 

    /* Since we need to change on/off image frequently */
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_CACHE_DYNAMIC_DATA;

    init_dynamic_item_buffer(number_of_items, Category186GetItem, get_hint_func, UI_dummy_function);
    MMI_fixed_list_menu.n_items = number_of_items;

    register_dynamic_list_shortcut_handler();
    register_dynamic_list_keys();
    shortcut_width = set_menu_item_count(number_of_items) + 7;
    resize_menu_shortcut_handler(shortcut_width, (MMI_title_height));
    move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
    register_hide_menu_shortcut(draw_title);
    resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);
    set_fixed_icontext_positions(MMI_ICONTEXT_MENUITEM_HEIGHT + 2, 0, 1, 0);

    register_fixed_list_highlight_handler(Category186ListHighlightHdlr);
    h_flag = SetCategory186History(history_buffer);

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    if (get_hint_func)
    {
        if (is_set_force_icon_on_highlight_only_in_menuitem())
        {
            wgui_show_icon_only_highlight_in_icontext_menuitem();
        }
        if (is_set_force_all_hints_in_menuitem())
        {
            wgui_enable_hints_in_icontext_menuitem();
        }
        else if (is_set_force_hints_on_highlight_in_menuitem())
        {
            wgui_enable_hint_highlight_in_icontext_menuitem();
        }
        else
        {
            wgui_enable_hint_highlight_in_icontext_menuitem();
        }

        set_dynamic_pop_up_descriptions(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);      /* ????? */
    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        gui_block_list_effect();
    #endif 
    }
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

    if(lsk_handler_func != NULL)
    {
        wgui_cat_187_lsk_handler = lsk_handler_func;
    }
    else
    {
        wgui_cat_187_lsk_handler = NULL;
    }
	
    set_left_softkey_function(Category186ToggleCheckBox, KEY_EVENT_UP);
    if (wgui_cat186_p->modified)
    {
        ChangeRightSoftkey(WGUI_CATEGORY_DONE_STRING_ID, 0);
        set_right_softkey_function(checkbox_toggle_DONE_RSK_function, KEY_EVENT_UP);
    }
    else
    {
        ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
        set_right_softkey_function(checkbox_toggle_BACK_RSK_function, KEY_EVENT_UP);
    }

    dynamic_list_goto_item_no_redraw(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);

    set_dynamic_pop_up_descriptions(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);  /* ????? */

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory186Screen;
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory186History, GetCategory186HistorySize);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY186_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}

/*****************************************************************************
* Category 84
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowCategory84Screen
 * DESCRIPTION
 *  Displays the category84 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items to be displayed
 *  list_of_items           [IN]        Array of items to be displayed.
 *  list_of_icons           [IN]        Array of icons to be displayed.
 *  flags                   [IN]        Represent the type of the menu. LIST_MENU, MATRIX_MENU
 *  highlighted_item        [IN]        Index of item to be highlighted
 *  history_buffer          [IN]        History buffer
 *  Values allowed for these 8 bits are:(?)
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory84Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U16 *list_of_icons,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    create_fixed_icontext_menuitems();
    associate_fixed_icontext_list();

    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    for (i = 0; i < number_of_items; i++)
    {
        add_fixed_icontext_item((UI_string_type) list_of_items[i], wgui_get_list_menu_icon(i, list_of_icons[i]));
    }
    resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);
#ifdef __MMI_NOKIA_STYLE_N800__
    set_fixed_icontext_positions(MMI_MENUITEM_HEIGHT + 2, 0, 1, 0);
#else
    set_fixed_icontext_positions(MMI_ICONTEXT_MENUITEM_HEIGHT + 2, 0, 1, 0);

#endif

    MMI_current_menu_type = LIST_MENU;
    h_flag = set_list_menu_category_history(MMI_CATEGORY84_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitListCategoryScreen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY84_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory84Screen */


/*****************************************************************************
 * FUNCTION
 *  SetCategory111RightSoftkeyFunction
 * DESCRIPTION
 *  Register RSK function handler of category111 screen
 * PARAMETERS
 *  f               [IN]        
 *  k               [IN]        
 *  function(?)     [IN]        Pointer
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory111RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(k);
    wgui_singleline_inputbox_RSK_function = f;
}

/* filled colr scheme for category111 */
UI_filled_area PIN_inputbox_BG_filler = 
{
    UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER | UI_FILLED_AREA_SHADOW,
    UI_NULL_IMAGE,
    NULL,
    {255, 222, 255, 100},
    {0, 0, 0, 0},
    {255, 0, 0, 100},
    {255, 0, 0, 100},
    0
};

/* theme of single input box of enter pin screen */

UI_single_line_input_box_theme PIN_singleline_inputbox_theme = { &PIN_inputbox_BG_filler,
    &PIN_inputbox_BG_filler,
    &PIN_inputbox_BG_filler,
#ifdef __MMI_MAINLCD_220X176__
    {255, 255, 255, 100},
#else
    {0, 0, 0, 100},
#endif    
    {128, 128, 128, 100},
    {0, 0, 0, 100},
    {255, 255, 255, 100},
    {51, 88, 171, 100},
    {255, 0, 0, 100},
    &UI_DEFAULT_FONT,
    1,
    UI_SINGLE_LINE_INPUT_BOX_CENTER_Y,
    '*'
};

#ifdef __MMI_TOUCH_SCREEN__
S32 passwd_keybd_width;
S32 passwd_keybd_height;


/*****************************************************************************
 * FUNCTION
 *  DrawCate111CategoryControlArea
 * DESCRIPTION
 *  Draws the category111 category control area
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate111CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 passwd_keybd_start_x = 0, passwd_keybd_start_y = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_image_get_dimension_id(IMG_PASSWD_KEYBOARD, &passwd_keybd_width, &passwd_keybd_height);
    passwd_keybd_start_x = (UI_device_width - passwd_keybd_width) >> 1;
    passwd_keybd_start_y = UI_device_height - MMI_button_bar_height - 1 - passwd_keybd_height;
    gdi_image_draw(passwd_keybd_start_x, passwd_keybd_start_y, get_image(IMG_PASSWD_KEYBOARD));

}


/*****************************************************************************
 * FUNCTION
 *  Cate111CategoryControlAreaPenDownHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate111CategoryControlAreaPenDownHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 key_number = -1;
    S32 passwd_keybd_start_x = (UI_device_width - passwd_keybd_width) >> 1;
    S32 passwd_keybd_start_y = UI_device_height - MMI_button_bar_height - 1 - passwd_keybd_height;
    S32 offset_x = point.x - passwd_keybd_start_x;
    S32 offset_y = point.y - passwd_keybd_start_y;
    S32 pressed_area_x1 = 0, pressed_area_x2 = 0;
    S32 pressed_area_y1 = 0, pressed_area_y2 = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_singleline_inputbox.text_length >= MMI_singleline_inputbox.available_length)
    {
        return MMI_FALSE;
    }

    if (offset_x < 0 || offset_y < 0 || offset_x >= passwd_keybd_width || offset_y >= passwd_keybd_height)
    {
        return MMI_FALSE;
    }

    gdi_layer_lock_frame_buffer();
    if (offset_x == passwd_keybd_width - 1)
    {
        key_number = 4;
    }
    else
    {
        key_number = offset_x / (passwd_keybd_width / 5);
    }
    if (offset_y > (passwd_keybd_height >> 1))
    {
        key_number += 5;
    }
    if (key_number < 5)
    {
        pressed_area_x1 = passwd_keybd_start_x + key_number * (passwd_keybd_width / 5);
        pressed_area_x2 = passwd_keybd_start_x + (key_number + 1) * (passwd_keybd_width / 5);
        pressed_area_y1 = passwd_keybd_start_y;
        pressed_area_y2 = passwd_keybd_start_y + (passwd_keybd_height >> 1);
    }
    else
    {
        pressed_area_x1 = passwd_keybd_start_x + (key_number - 5) * (passwd_keybd_width / 5);
        pressed_area_x2 = passwd_keybd_start_x + (key_number - 5 + 1) * (passwd_keybd_width / 5);
        pressed_area_y1 = passwd_keybd_start_y + (passwd_keybd_height >> 1);
        pressed_area_y2 = passwd_keybd_start_y + passwd_keybd_height;
    }
    gdi_layer_push_clip();
    gdi_layer_set_clip(pressed_area_x1, pressed_area_y1, pressed_area_x2, pressed_area_y2);
    gdi_image_draw(passwd_keybd_start_x, passwd_keybd_start_y, get_image(IMG_PASSWD_KEYBOARD_PRESS));
    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(pressed_area_x1, pressed_area_y1, pressed_area_x2, pressed_area_y2);
    singleline_inputbox_handle_key_down(KEY_0 + key_number);
    return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  Cate111CategoryControlAreaPenUpHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate111CategoryControlAreaPenUpHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 passwd_keybd_start_x = (UI_device_width - passwd_keybd_width) >> 1;
    S32 passwd_keybd_start_y = UI_device_height - MMI_button_bar_height - 1 - passwd_keybd_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_singleline_inputbox.text_length > MMI_singleline_inputbox.available_length)
    {
        return MMI_FALSE;
    }
    gdi_layer_lock_frame_buffer();
    gdi_layer_push_clip();
    gdi_layer_set_clip(
        passwd_keybd_start_x,
        passwd_keybd_start_y,
        passwd_keybd_start_x + passwd_keybd_width,
        passwd_keybd_start_y + passwd_keybd_height);
    gdi_image_draw(passwd_keybd_start_x, passwd_keybd_start_y, get_image(IMG_PASSWD_KEYBOARD));
    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(
        passwd_keybd_start_x,
        passwd_keybd_start_y,
        passwd_keybd_start_x + passwd_keybd_width,
        passwd_keybd_start_y + passwd_keybd_height);
    return MMI_TRUE;
}
#endif /* __MMI_TOUCH_SCREEN__ */ 

#ifdef UI_SMALL_PIN_EDITOR_SCREEN
UI_filled_area small_pin_editor_background_filler;


/*****************************************************************************
 * FUNCTION
 *  ShowCategory112Screen
 * DESCRIPTION
 *  Displays the category112 screen:- main used for enter pin screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Message
 *  input_type              [IN]        Type of input to use (See the list below)
 *  buffer                  [IN]        Buffer the input box should use.
 *  buffer_size             [IN]        Size of the buffer.
 *  message2                [?]         
 *  history_buffer          [IN]        History buffer INPUT_TYPE_NUMERIC INPUT_TYPE_ALPHANUMERIC_UPPERCASE INPUT_TYPE_ALPHANUMERIC_LOWERCASE INPUT_TYPE_ALPHANUMERIC_SENTENCECASE INPUT_TYPE_NUMERIC_PASSWORD INPUT_TYPE_ALPHANUMERIC_PASSWORD (rare).
 *  title_icon(?)           [IN]        Icon shown with the title
 *  title(?)                [IN]        Title for the screen
 *  INPUT_TYPE can take the following values:(?)
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory112Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        S16 input_type,
        U8 *buffer,
        S32 buffer_size,
        U8 *message2,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    color_t c = *(current_MMI_theme->UI_black_color);
    S32 len;
    S32 single_line_inputbox_height = 22;
    S32 dialog_width = 0, dialog_height = 0, dialog_x = 0, dialog_y = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
#ifdef __MMI_ZI__
    ZiInuptMethodEnterSinglelineInputBox();
#endif 
#ifdef __MMI_GB__
	GbInuptMethodEnterSinglelineInputBox();
#endif

#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
 
    gui_add_cleanup_hook(disable_resized_small_screen);
  
#endif /* __MMI_UI_SMALL_SCREEN_SUPPORT__ */ 

    hide_status_icon_bar(0);//052906 early status icon

    dialog_height =
        UI_device_height - (MMI_title_y + MMI_title_height + 2 * MMI_SMALL_SCREEN_Y_OFFSET + MMI_button_bar_height);
    dialog_width = UI_device_width - 2 * MMI_SMALL_SCREEN_X_OFFSET;
    dialog_x = MMI_SMALL_SCREEN_X_OFFSET;
    dialog_y = MMI_title_y + MMI_title_height + MMI_SMALL_SCREEN_Y_OFFSET;

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    SetupCategoryKeyHandlers();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    MMI_message_string = (UI_string_type) message;
    len = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, len, len, 0);
    small_pin_editor_background_filler = *current_MMI_theme->PIN_screen_background_filler;
    small_pin_editor_background_filler.flags =
        UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER | UI_FILLED_AREA_ROUNDED_BORDER;
    MMI_multiline_inputbox.flags |=
        UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW /*| UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY */|//081206 enter pin
        UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR |
        UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;

    dm_add_string((UI_string_type) message2, &MMI_medium_font, UI_COLOR_BLACK, UI_COLOR_BLACK, NULL);
    dm_data.S32flags = DM_NO_FLAGS;

    wgui_setup_singleline_inputbox(
        dialog_x + 4,
        MMI_multiline_inputbox.y + MMI_multiline_inputbox.height,
        dialog_width - 8,
        single_line_inputbox_height,
        buffer,
        buffer_size,
        MMI_CATEGORY112_ID,
        get_string(right_softkey),
        get_image(right_softkey_icon),
        input_type,
        history_buffer,
        0);
    gui_set_single_line_input_box_theme(&MMI_singleline_inputbox, &PIN_singleline_inputbox_theme);

    register_hide_multitap(wgui_hide_multitap);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory112Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY112_ID;
    dm_add_back_fill_area(&small_pin_editor_background_filler);
    dm_add_rectangle(c, DM_RECTANGLE_FILL_GRAYSCALE);
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory112Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory112Screen
 * DESCRIPTION
 *  Exits category 182 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory112Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    reset_small_screen();
#endif 
    show_status_icon_bar(0);//052906 early status icon
    reset_multiline_inputbox();
    set_MMI_current_input_type();
    reset_softkeys();
    reset_multitaps();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    gui_set_single_line_input_box_current_theme(&MMI_singleline_inputbox);
    wgui_close_singleline_inputbox();
    entry_full_screen();
}

#endif /* UI_SMALL_PIN_EDITOR_SCREEN */ 

 
S32 Cat111CountDownValue;
FuncPtr Cat111TimerExpireCallBack = UI_dummy_function;
UI_filled_area pin_editor_background_filler;//083106 pin editor


/*****************************************************************************
 * FUNCTION
 *  SetCat111TimerExpireFunc
 * DESCRIPTION
 *  Set cat 111 countdown callback function
 * PARAMETERS
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCat111TimerExpireFunc(FuncPtr f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat111TimerExpireCallBack = f;
}


/*****************************************************************************
 * FUNCTION
 *  Cat111CountDownCallBack
 * DESCRIPTION
 *  cat 111 countdown callback function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern UI_font_type UI_font;
extern color_t UI_text_color;
void Cat111CountDownCallBack(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 buf[6];
    UI_font_type saved_font = UI_font;
    color_t saved_color = UI_text_color;
    S32 count_y;
    S32 sw, sh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (Cat111CountDownValue == 0)
    {
        StopTimer(POPUP_TIMER);
        if (Cat111TimerExpireCallBack != NULL)
        {
            Cat111TimerExpireCallBack();
        }
        Cat111TimerExpireCallBack = UI_dummy_function;
    }
    else
    {
        gui_itoa(Cat111CountDownValue, (UI_string_type) buf, 10);
        gui_set_font(&MMI_medium_font);
        gui_set_text_color(UI_COLOR_BLACK);

        gui_measure_string((UI_string_type) buf, &sw, &sh);
    #ifdef __MMI_TOUCH_SCREEN__
        count_y = UI_device_height - MMI_button_bar_height - passwd_keybd_height - sh - (UI_device_height / 10);
    #else 
        count_y = UI_device_height - MMI_button_bar_height - sh - 1;
    #endif 
        gdi_layer_push_clip();
    
    #if (defined (__MMI_MAINLCD_176X220__)) || (defined (__MMI_MAINLCD_240X320__))||(defined (__MMI_MAINLCD_220X176__))//083106 pin editor
        if (wgui_is_wallpaper_on_bottom())
        {
            gdi_draw_solid_rect(0, count_y, UI_device_width - 1, count_y + sh - 1, GDI_COLOR_TRANSPARENT);
        }
        else
        {
            gdi_draw_solid_rect(0, count_y, UI_device_width - 1, count_y + sh - 1, GDI_COLOR_WHITE);
        }
    #else
        gui_draw_filled_area(0, count_y, UI_device_width - 1, count_y + sh - 1, &pin_editor_background_filler);
    #endif

        gui_move_text_cursor((UI_device_width - sw) >> 1, count_y);
        gui_print_text((UI_string_type) buf);
        gdi_layer_pop_clip();
        gui_BLT_double_buffer(0, count_y, UI_device_width - 1, count_y + sh - 1);
        gui_set_font(saved_font);
        gui_set_text_color(saved_color);
        Cat111CountDownValue--;
        StartTimer(POPUP_TIMER, 1000, Cat111CountDownCallBack);
    }
}

 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory111Screen
 * DESCRIPTION
 *  Displays the category111 screen:- main used for enter pin screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Message
 *  input_type              [IN]        Type of input to use (See the list below)
 *  buffer                  [IN]        Buffer the input box should use.
 *  buffer_size             [IN]        Size of the buffer.
 *  message2                [?]         
 *  history_buffer          [IN]        History buffer INPUT_TYPE_NUMERIC INPUT_TYPE_ALPHANUMERIC_UPPERCASE INPUT_TYPE_ALPHANUMERIC_LOWERCASE INPUT_TYPE_ALPHANUMERIC_SENTENCECASE INPUT_TYPE_NUMERIC_PASSWORD INPUT_TYPE_ALPHANUMERIC_PASSWORD (rare).
 *  title_icon(?)           [IN]        Icon shown with the title
 *  title(?)                [IN]        Title for the screen
 *  INPUT_TYPE can take the following values:(?)
 * RETURNS
 *  void
 *****************************************************************************/
 
void ShowCategory111Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        S16 input_type,
        U8 *buffer,
        S32 buffer_size,
        U8 *message2,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCategory111Screen_ext(
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        (U8*) get_string(message),
        input_type,
        buffer,
        buffer_size,
        message2,
        0,
        history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory111Screen_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [?]         
 *  input_type              [IN]        
 *  buffer                  [?]         
 *  buffer_size             [IN]        
 *  message2                [?]         
 *  duration                [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
	#include "settingdefs.h"
#ifdef __MMI_NOKIA_STYLE_N800__
extern color_t black_color;
#endif
void ShowCategory111Screen_ext(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *mess,
        S16 input_type,
        U8 *buff,
        S32 buffer_size,
        U8 *message2,
        S32 p_duration,
        U8 *history_buffer)
{
#ifdef UI_SMALL_PIN_EDITOR_SCREEN
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
 
    if (whether_no_small_screen())
  
    {
        ShowCategory112Screen(
            left_softkey,
            left_softkey_icon,
            right_softkey,
            right_softkey_icon,
            mess,
            input_type,
            buff,
            buffer_size,
            message2,
            history_buffer);
    }
    else
#endif /* UI_SMALL_PIN_EDITOR_SCREEN */ 
    {
        /*----------------------------------------------------------------*/
        /* Local Variables                                                */
        /*----------------------------------------------------------------*/
        dm_data_struct dm_data;
        S32 h, len1;

        /*----------------------------------------------------------------*/
        /* Code Body                                                      */
        /*----------------------------------------------------------------*/
        dm_reset_context();
        gdi_layer_lock_frame_buffer();
    #ifdef __MMI_ZI__
        ZiInuptMethodEnterSinglelineInputBox();
    #endif

     /*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-17 14:56 */
     #ifdef __MMI_GB__
	GbInuptMethodEnterSinglelineInputBox();
     #endif
     /*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-17 14:56 */
	 
        MMI_menu_shortcut_number = -1;
        MMI_disable_title_shortcut_display = 1;
        change_left_softkey(left_softkey, left_softkey_icon);
        change_right_softkey(right_softkey, right_softkey_icon);
        SetupCategoryKeyHandlers();

        ShowStatusIconsTitle();
        
    #if (!defined (__MMI_MAINLCD_176X220__)) && (!defined (__MMI_MAINLCD_240X320__))&&(!defined (__MMI_MAINLCD_220X176__))
        pin_editor_background_filler = *current_MMI_theme->PIN_screen_background_filler;//083106 pin editor
        pin_editor_background_filler.flags = UI_FILLED_AREA_TYPE_COLOR;
        dm_add_back_fill_area(&pin_editor_background_filler);
    #endif
        
         
        MMI_message_string = (UI_string_type) mess;
        len1 = gui_strlen(MMI_message_string);
        create_multiline_inputbox_set_buffer(MMI_message_string, len1, len1, 0);
        MMI_multiline_inputbox.flags |=
            (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND |
             UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE);

//for t800
#ifdef __MMI_NOKIA_STYLE_N800__
		MMI_multiline_inputbox.normal_text_color = black_color;
#endif		
        h = 22;
        wgui_setup_singleline_inputbox(
            4,
            MMI_multiline_inputbox.y + MMI_multiline_inputbox.height,
            UI_device_width - 8,
            h,
            buff,
            buffer_size,
            MMI_CATEGORY111_ID,
            get_string(right_softkey),
            get_image(right_softkey_icon),
            input_type,
            history_buffer,
            0);
#if !defined(__MMI_USING_BLACK_THEME_BG__)		
        gui_set_single_line_input_box_theme(&MMI_singleline_inputbox, &PIN_singleline_inputbox_theme);
#endif		
	#if defined(__MMI_LANG_FRENCH__)&& (defined(__MMI_MAINLCD_128X160__)||defined(__MMI_MAINLCD_160X128__)||defined (__MMI_MAINLCD_128X128__))
	if(SCR_ID_SECSET_SIM_LOCK_PIN1 == GetActiveScreenId())
		dm_add_string((UI_string_type)message2, &MMI_small_font, UI_COLOR_BLACK, UI_COLOR_BLACK, NULL);
	else
		dm_add_string((UI_string_type)message2, &MMI_medium_font, UI_COLOR_BLACK, UI_COLOR_BLACK, NULL);
	#else
		#ifdef __MMI_MAINLCD_220X176__
		dm_add_string((UI_string_type)message2, &MMI_medium_font, *(current_MMI_theme->UI_white_color), *(current_MMI_theme->UI_black_color), NULL);
		#else
		dm_add_string((UI_string_type)message2, &MMI_medium_font, UI_COLOR_BLACK, UI_COLOR_BLACK, NULL);
		#endif
	#endif
	/*Wangzl Modify End for 7339 Ver: Multi_Language  on 2008-1-29 16:46 */
        register_hide_multitap(wgui_hide_multitap);

        gdi_layer_unlock_frame_buffer();

        ExitCategoryFunction = ExitCategory111Screen;
        dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    #ifdef __MMI_TOUCH_SCREEN__
        dm_register_category_controlled_callback(DrawCate111CategoryControlArea);
        wgui_register_category_screen_control_area_pen_handlers(
            Cate111CategoryControlAreaPenDownHandler,
            MMI_PEN_EVENT_DOWN);
        wgui_register_category_screen_control_area_pen_handlers(
            Cate111CategoryControlAreaPenUpHandler,
            MMI_PEN_EVENT_UP);
    #endif /* __MMI_TOUCH_SCREEN__ */ 
        dm_data.S32ScrId = (S32) GetActiveScreenId();
        dm_data.S32CatId = MMI_CATEGORY111_ID;
        dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND | DM_NO_TITLE;
        dm_setup_data(&dm_data);
        dm_redraw_category_screen();
    
        Cat111CountDownValue = p_duration;
        if (Cat111CountDownValue != 0)
        {
            Cat111CountDownCallBack();
        }
    }
}   /* end of ShowCategory111Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory111Screen
 * DESCRIPTION
 *  Exits category 184 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory111Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_multiline_inputbox();
    set_MMI_current_input_type();
    Cat111TimerExpireCallBack = UI_dummy_function;
    reset_softkeys();
    clear_left_softkey();
    clear_right_softkey();
    reset_multitaps();
    close_status_icons();
    gui_set_single_line_input_box_current_theme(&MMI_singleline_inputbox);
    wgui_close_singleline_inputbox();
 
#ifdef UI_SMALL_LIST_SCREEN
    restore_small_screen();
#endif 
  
}

#ifdef __MMI_ENGINEER_MODE__


/*****************************************************************************
 * FUNCTION
 *  ShowCategory204Screen
 * DESCRIPTION
 *  Displays the category204 screen
 * PARAMETERS
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Icon for the Left softkey
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Icon for the right softkey
 *  NotificationStringId        [IN]        Id of string displayed in title
 *  value                       [IN]        The value of string displayed in single-line inputbox
 *  imageId                     [IN]        Id of image to be displayed.
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory204Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 NotificationStringId,
        U16 value,
        U16 imageId,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    UI_string_type text[5];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(NotificationStringId);

    gui_itoa(value, (UI_string_type) text, 10);
    create_singleline_inputbox_set_buffer(
        (UI_string_type) text,
        gui_strlen((UI_string_type) text) + 1,
        gui_strlen((UI_string_type) text),
        gui_strlen((UI_string_type) text));
    set_singleline_inputbox_mask(0);
    MMI_singleline_inputbox.flags |=
        (UI_SINGLE_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW |
         UI_SINGLE_LINE_INPUT_BOX_CENTER_JUSTIFY);

    gdi_layer_unlock_frame_buffer();
    ExitCategoryFunction = ExitCategory204Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY204_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* ShowCategory204Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory204Screen
 * DESCRIPTION
 *  Exits the category204 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory204Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
    reset_softkeys();
    reset_singleline_inputbox();
}   /* end of ExitCategory204Screen */

/* temp solution */
#ifdef __MMI_DRAW_MANAGER__
#undef __MMI_DRAW_MANAGER__
#define __MMI_DIS_DRAW_MANAGER__
#endif /* __MMI_DRAW_MANAGER__ */ 
#ifndef __MMI_DRAW_MANAGER__

/* 081206 cat205 Start */
extern PU8 _MMI_animated_icon;
extern S32 _MMI_animated_icon_x;
extern S32 _MMI_animated_icon_y;
/* 081206 cat205 End */

/*****************************************************************************
 * FUNCTION
 *  RedrawCategory205Screen
 * DESCRIPTION
 *  redraw category 205
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory205Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gui_hide_animations();
    clear_screen();
    gdi_layer_push_clip();
    gdi_layer_set_clip(0, 0, UI_device_width - 1, UI_device_height - MMI_button_bar_height - 1);
    gdi_layer_reset_clip();
    //wgui_show_transparent_animation();//081206 cat205
    gdi_image_draw(_MMI_animated_icon_x, _MMI_animated_icon_y, (U8*) _MMI_animated_icon);
    gdi_layer_pop_clip();
    show_multiline_inputbox();
    show_softkey_background();
    show_left_softkey();
    show_right_softkey();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}
#endif /* __MMI_DRAW_MANAGER__ */ 


/*****************************************************************************
 * FUNCTION
 *  ExitCategory205Screen
 * DESCRIPTION
 *  Exits the category205 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory205Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
    reset_softkeys();
    close_status_icons();
}   /* end of ExitCategory205Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory205Screen
 * DESCRIPTION
 *  Displays the category205 screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        String to be displayed.
 *  message_icon            [IN]        Id of image to be displayed.
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef TGT_GALLITE_G800
    extern color_t black_color;
#endif
void ShowCategory205Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        UI_string_type message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_DRAW_MANAGER__
    dm_data_struct dm_data;
#endif 
    S32 l, fh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_DRAW_MANAGER__
    dm_reset_context();
#endif 
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();
    MMI_menu_shortcut_number = -1;

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_message_string = (UI_string_type) message;
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);
    
#if defined(TGT_GALLITE_G800) || defined(__MMI_USING_BLACK_THEME_BG__)
    MMI_multiline_inputbox.normal_text_color = black_color;
#endif

    gui_set_font(MMI_multiline_inputbox.text_font);

#ifdef __MMI_DRAW_MANAGER__
    dm_add_image(get_image(message_icon), NULL, NULL);
#else /* __MMI_DRAW_MANAGER__ */ 
    move_multiline_inputbox(0, (MMI_title_y + MMI_title_height) + 2);
    resize_multiline_inputbox(MMI_multiline_inputbox.width, MMI_content_height);
    show_multiline_inputbox_no_draw();
    fh = get_multiline_inputbox_line_height();
    resize_multiline_inputbox(
        MMI_multiline_inputbox.width,
        (MMI_multiline_inputbox.n_lines * fh) + MULTILINE_INPUTBOX_HEIGHT_PAD + MMI_multiline_inputbox.text_y);
    wgui_set_animation((UI_device_width >> 1), ((UI_device_height - MMI_button_bar_height) >> 1), message_icon);
#endif /* __MMI_DRAW_MANAGER__ */ 
    register_hide_status_icon_bar(0, MMI_dummy_function);
    register_hide_status_icon_bar(1, MMI_dummy_function);
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory205Screen;
#ifdef __MMI_DRAW_MANAGER__
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY205_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
#else /* __MMI_DRAW_MANAGER__ */ 
    RedrawCategoryFunction = RedrawCategory205Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCategoryFunction();
#endif /* __MMI_DRAW_MANAGER__ */ 
}   /* end of ShowCategory205Screen */

#endif /* __MMI_ENGINEER_MODE__ */ 

#ifdef __MMI_DIS_DRAW_MANAGER__
#define __MMI_DRAW_MANAGER__
#undef __MMI_DIS_DRAW_MANAGER__
#endif /* __MMI_DIS_DRAW_MANAGER__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory109Screen
 * DESCRIPTION
 *  (twostate menuitem with hint, and no LSK function)
 *  Displays the category109 screen
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon shown with the title
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Icon for the Left softkey
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Icon for the right softkey
 *  number_of_items             [IN]        
 *  list_of_items               [IN]        
 *  list_of_descriptions        [IN]        
 *  highlighted_item            [IN]        
 *  history_buffer              [IN]        History buffer
 *  buffer(?)                   [IN]        Buffer the input box should use
 *  buffer_size(?)              [IN]        Size of the buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory109Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U8 **list_of_descriptions,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    create_fixed_twostate_menuitems(get_image(RADIO_ON_IMAGE_ID), get_image(RADIO_OFF_IMAGE_ID));
    associate_fixed_twostate_list();
    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    resize_fixed_twostate_menuitems(0, MMI_MENUITEM_HEIGHT);
    set_fixed_twostate_positions(MMI_MENUITEM_HEIGHT, 0, 1, 0);

    /* Softkeys */
    clear_left_softkey_handler();

    if (list_of_descriptions == NULL)
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_twostate_item((UI_string_type) list_of_items[i]);
            wgui_pop_up_description_strings[i].text_strings[0] = NULL;
        }
    }
    else
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_twostate_item((UI_string_type) list_of_items[i]);
            wgui_pop_up_description_strings[i].text_strings[0] = (UI_string_type) list_of_descriptions[i];
        }
    }

    register_fixed_list_highlight_handler(standard_radio_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY109_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    select_fixed_twostate_item(MMI_fixed_list_menu.highlighted_item);
    set_pop_up_descriptions(1, number_of_items, MMI_fixed_list_menu.highlighted_item);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitListCategoryScreen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY109_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory109Screen */


/*****************************************************************************
 * FUNCTION
 *  category88_handle_down_key
 * DESCRIPTION
 *  handle key down of category88
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category88_handle_down_key(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_hide_slide_control_focus();
    if (MMI_current_slide_control == &MMI_slide_control[dm_get_slide_control_count() - 1])
    {
        MMI_current_slide_control = &MMI_slide_control[0];
    }
    else
    {
        MMI_current_slide_control++;
    }
    wgui_slide_control_set_focus(MMI_current_slide_control);
    wgui_show_slide_control_focus();
}   /* end of category88_handle_down_key */


/*****************************************************************************
 * FUNCTION
 *  category88_handle_up_key
 * DESCRIPTION
 *  handle key up of category88
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category88_handle_up_key(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_hide_slide_control_focus();
    if (MMI_current_slide_control == &MMI_slide_control[0])
    {
        MMI_current_slide_control = &MMI_slide_control[dm_get_slide_control_count() - 1];
    }
    else
    {
        MMI_current_slide_control--;
    }
    wgui_slide_control_set_focus(MMI_current_slide_control);
    wgui_show_slide_control_focus();
}   /* end of category88_handle_up_key */


/*****************************************************************************
 * FUNCTION
 *  RegisterCategory88ChangeCallbacks
 * DESCRIPTION
 *  register category88 chaneg call back functions
 * PARAMETERS
 *  f1          [IN]        
 *  f2          [IN]        Callback2 function pointer
 *  f(?)        [IN]        Callback1 function pointer
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterCategory88ChangeCallbacks(void (*f1) (S32 value), void (*f2) (S32 value))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_slide_control[0].change_callback = f1;
    MMI_slide_control[1].change_callback = f2;
}   /* end of RegisterCategory88ChangeCallbacks */


/*****************************************************************************
 * FUNCTION
 *  category88_keyboard_key_handler
 * DESCRIPTION
 *  keyboard handler of category88(Only for Win32 environment)
 * PARAMETERS
 *  vkey_code       [IN]        Key code to identify the key
 *  key_state       [IN]        The state of key
 * RETURNS
 *  void
 *****************************************************************************/
void category88_keyboard_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                wgui_slide_control_previous();
                break;
            case 39:
                wgui_slide_control_next();
                break;
            case 38:
                category88_handle_up_key();
                break;
            case 40:
                category88_handle_down_key();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}   /* end of category88_keyboard_key_handler */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory88Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  bg_image_id             [IN]        
 *  n_items                 [IN]        
 *  string_list             [IN]        
 *  lower_limit             [?]         
 *  upper_limit             [?]         
 *  current_value           [IN]        
 *  cb_func                 [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory88Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 bg_image_id,
        U16 n_items,
        U8 **string_list,
        S32 *lower_limit,
        S32 *upper_limit,
        S32 **current_value,
        void (**cb_func) (S32 value),
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 idx = 0;
    dm_data_struct dm_data;
    U8 h_flag;

     
    color_t c = *(current_MMI_theme->UI_black_color);
    color_t wc = *(current_MMI_theme->UI_white_color);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(n_items <= MMI_MAX_SLIDE_CONTROL);

    dm_reset_context();
    UI_UNUSED_PARAMETER(history_buffer);

    MMI_ASSERT(lower_limit != NULL);
    MMI_ASSERT(upper_limit != NULL);
    MMI_ASSERT(current_value != NULL);

    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    MMI_title_string = get_string(title);
    MMI_title_icon = get_image(title_icon);

    h_flag = set_slide_control_category_history((U16) (MMI_CATEGORY88_1ITEM_ID + n_items - 1), (U8*) history_buffer);
    if (h_flag)
    {
        /* the number of slide controls should be the same as the number kept in history */
        MMI_ASSERT(h_flag == n_items);

        for (idx = 0; idx < n_items; idx++)
        {
            *current_value[idx] = *MMI_slide_control_value[idx];
        }
    }
    else
    {
        for (idx = 0; idx < n_items; idx++)
        {
            MMI_slide_control_value[idx] = (current_value[idx]);
        }

        MMI_current_slide_control = &MMI_slide_control[0];
        MMI_current_slide_control_value = (S32*) MMI_slide_control_value[0];            
    }

    /* Avoid NULL pointer in global context */
    MMI_ASSERT(MMI_current_slide_control && MMI_current_slide_control_value);

    for (idx = 0; idx < n_items; idx++)
    {
        if (string_list)
        {
            dm_add_string((UI_string_type) string_list[idx], &MMI_default_font, c, wc, NULL);
            if (cb_func != NULL)
            {
                dm_add_slide_control(
                    lower_limit[idx],
                    upper_limit[idx],
                    *current_value[idx],
                    cb_func[idx],
                    string_list[idx]);
            }
            else
            {
                dm_add_slide_control(lower_limit[idx], upper_limit[idx], *current_value[idx], NULL, string_list[idx]);
            }
        }
        else
        {
            if (cb_func != NULL)
            {
                dm_add_slide_control(lower_limit[idx], upper_limit[idx], *current_value[idx], cb_func[idx], NULL);
            }
            else
            {
                dm_add_slide_control(lower_limit[idx], upper_limit[idx], *current_value[idx], NULL, NULL);
            }

        }
    }
    if (bg_image_id != 0)
    {
        dm_add_image(get_image(bg_image_id), NULL, NULL);
    }

    SetKeyHandler(wgui_slide_control_previous, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(wgui_slide_control_next, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(category88_handle_up_key, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(category88_handle_down_key, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    SetKeyHandler(category88_handle_up_key, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(category88_handle_down_key, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    register_keyboard_key_handler(category88_keyboard_key_handler);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory88Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    /*wuzc Modify Start For GDI/GUI Portting Ver:preferred net  slid value error   on 2008-4-28 18:17 */
    //dm_data.S32CatId = MMI_CATEGORY88_1ITEM_ID + n_items - 1;
    dm_data.S32CatId = MMI_CATEGORY87_ID/*MMI_CATEGORY88_1ITEM_ID + n_items - 1*/;
    /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-4-28 18:17 */
    dm_data.S32ScrId = GetActiveScreenId();
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory88Screen
 * DESCRIPTION
 *  Exits the Double Slide control screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory88Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_message_string = NULL;
    MMI_message_string2 = NULL;
    reset_softkeys();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory87Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  l_limit                 [IN]        
 *  u_limit                 [IN]        
 *  current_value           [?]         
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory87Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 l_limit,
        S32 u_limit,
        S32 *current_value,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S32 lower_limit[1], upper_limit[1];
    S32 *curr_value[1];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    lower_limit[0] = l_limit;
    upper_limit[0] = u_limit;
    curr_value[0] = current_value;
    ShowCategory88Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        0,
        1,
        NULL,
        lower_limit,
        upper_limit,
        curr_value,
        NULL,
        history_buffer);
}   /* end of ShowCategory87Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory89Screen
 * DESCRIPTION
 *  Displays the Numbered Items screen(with description)
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon displayed with the title
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Left softkey icon
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Right softkey icon
 *  number_of_items             [IN]        Number of items in the menu
 *  list_of_items               [IN]        Array of items
 *  list_of_descriptions        [IN]        Array of Pop up description strings
 *  flags                       [IN]        Flags (see explanation below)
 *  highlighted_item            [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory89Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U8 **list_of_descriptions,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    create_fixed_icontext_list_menuitems(2, 0);
    GUI_current_fixed_icontext_list_menuitem_column = 1;
    set_fixed_icontext_list_text_coordinates(0, 1, 0, 30, MMI_MENUITEM_HEIGHT);
    /* Need to modify */
    set_fixed_icontext_list_text_coordinates(
        1,
        30,
        0,
        UI_device_width - 30 - 4 - MMI_fixed_list_menu.vbar.width,
        MMI_MENUITEM_HEIGHT);
    associate_fixed_icontext_list_list();

    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    for (i = 0; i < number_of_items; i++)
    {
        fixed_icontext_list_item_insert(i);
        gui_itoa(i + 1, (UI_string_type) category89_string_list[i], 10);
        gui_strcat((UI_string_type) category89_string_list[i], (UI_string_type) ".\0");
        add_fixed_icontext_list_item_text(i, 0, (UI_string_type) category89_string_list[i]);
        add_fixed_icontext_list_item_text(i, 1, (UI_string_type) list_of_items[i]);
        if (list_of_descriptions == NULL)
        {
            wgui_pop_up_description_strings[i].text_strings[0] = NULL;
        }
        else
        {
            wgui_pop_up_description_strings[i].text_strings[0] = (UI_string_type) list_of_descriptions[i];
        }

    }

    h_flag = set_list_menu_category_history(MMI_CATEGORY89_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    set_pop_up_descriptions(1, number_of_items, MMI_fixed_list_menu.highlighted_item);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitListCategoryScreen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY89_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory89Screen
 * DESCRIPTION
 *  Exits the Numbered Items screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory89Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GUI_current_fixed_icontext_list_menuitem_column = 0;
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_pop_up_descriptions();
    reset_fixed_list();
}

#ifdef __MMI_CALORIE__
#endif /* __MMI_CALORIE__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory119Screen
 * DESCRIPTION
 *  Displays the category119 screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  message                 [?]         
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  icon(?)                 [IN]        Message Icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory119Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();
    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    MMI_message_string = (UI_string_type) message;
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	hide_status_icon_bar(0);
	PopupChangeCharColor();
#endif
    dm_add_image(get_image(message_icon), NULL, NULL);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory119Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY119_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory119Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory119Screen
 * DESCRIPTION
 *  Exits the category119 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory119Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
}   /* end of ExitCategory119Screen */


/*****************************************************************************
 * FUNCTION
 *  cat129_clear_icon_bg
 * DESCRIPTION
 *  Clear button background function
 * PARAMETERS
 *  button      [IN]        Button pointer
 * RETURNS
 *  void
 *****************************************************************************/
void cat129_clear_icon_bg(void *btn)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    icontext_button *b = (icontext_button*) btn;
#ifdef __MMI_MAINLCD_240X320__
    S32 x1 = b->x -2;
    S32 y1 = b->y -2;
    S32 x2 = b->x + b->width - 1+2;
    S32 y2 = b->y + b->height - 1+2;
#else
    S32 x1 = b->x ;
    S32 y1 = b->y ;
    S32 x2 = b->x + b->width - 1;
    S32 y2 = b->y + b->height - 1;
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(x1, y1, x2, y2);
#ifdef __MMI_MAINLCD_240X320__
     gdi_image_draw_id(MMI_title_x, MMI_title_y, title_bg_id);
#else
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
#endif
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  cat129_get_image_handle
 * DESCRIPTION
 *  get the animation handle for cat 129
 * PARAMETERS
 *  void
 * RETURNS
 *  gdi_handle
 *****************************************************************************/
gdi_handle cat129_get_image_handle(void)//070706
{
    return dm_get_image_handle(0);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory129Screen
 * DESCRIPTION
 *  Displays the category129 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  image(?)                [IN]        Animation image
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory129Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message_icon,
        U8 *history_buffer)
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
#ifdef __MMI_NOKIA_STYLE_N800__	
    entry_full_screen();
#endif
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) title;
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_message_string = (UI_string_type) NULL;

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
      dm_get_scr_bg_image(&idle_scr_bg_ID, &idle_scr_bg_filename, &idle_scr_bg_x, &idle_scr_bg_y, &idle_scr_bg_opacity);
      dm_set_scr_bg_image(message_icon, NULL, -1, -1, 255);
	  wgui_set_wallpaper_on_bottom(MMI_TRUE);
	  dm_add_image(get_image(message_icon), NULL, NULL);
//#else
#ifdef __MMI_NOKIA_STYLE_N800__
    dm_add_image(get_image(message_icon), NULL, NULL);
 //   wgui_set_wallpaper_on_bottom(MMI_TRUE);
#endif
#else
    dm_add_image(get_image(message_icon), NULL, NULL);
    wgui_set_wallpaper_on_bottom(MMI_TRUE);
#endif
     
     
    //check if the title bar buttons are enabled for the particular screen.
    if (wgui_is_touch_title_bar_buttons())
    {
       TRACE_GDI_FUNCTION();
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
#ifndef __MMI_MAINLCD_220X176__
        title_bg_id = IMAGE_VIEW_TITLEBAR_BG;   /* for showing different title bar. */
#endif
    }
     
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory129Screen;

#if defined(__MMI_TOUCH_SCREEN__)
    if (wgui_is_touch_title_bar_buttons())
    {
        /* register the handler for pen event down istead of pen event up */
        dm_register_button_functions(btn_prev_item_id, KEY_EVENT_UP, gMMI_touch_title_button1_down_handler);
        dm_register_button_functions(btn_next_item_id, KEY_EVENT_UP, gMMI_touch_title_button2_down_handler);
    }
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY129_ID;
    dm_setup_data(&dm_data);
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
      dm_set_scr_bg_image(message_icon, NULL, -1, -1, 255);
#endif
    dm_redraw_category_screen();

}   /* end of ShowCategory129Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory129Screen
 * DESCRIPTION
 *  Exits the category129 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory129Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();

    title_bg_id = 0;
    wgui_reset_touch_title_bar_buttons();
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
	if (wgui_is_wallpaper_change())
	{
		U8 dummy_opacity;
		dm_get_scr_bg_image(&idle_scr_bg_ID, &idle_scr_bg_filename, &idle_scr_bg_x, &idle_scr_bg_y, &dummy_opacity);
		//dm_set_scr_bg_image(idle_scr_bg_ID, idle_scr_bg_filename, idle_scr_bg_x, idle_scr_bg_y, idle_scr_bg_opacity);
		wgui_reset_wallpaper_change();
	}
	else
	{
		dm_set_scr_bg_image_no_draw(idle_scr_bg_ID, idle_scr_bg_filename, idle_scr_bg_x, idle_scr_bg_y, idle_scr_bg_opacity);
	}
#endif
    
}   /* end of ExitCategory129Screen */


/*****************************************************************************
 * FUNCTION
 *  set_list_menu_category140_history
 * DESCRIPTION
 *  Sets the history data for the list menu of category140 (check box list)
 *  from the given history buffer
 * PARAMETERS
 *  history_ID          [IN]        The ID used by category screens
 *  history_buffer      [IN]        Buffer from which history data is taken
 * RETURNS
 *  byte              Returns true if the history ID is valid
 *****************************************************************************/
U8 set_list_menu_category140_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        list_menu_category_history_cat140 *h = (list_menu_category_history_cat140*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            MMI_fixed_list_menu.highlighted_item = h->highlighted_item;
            MMI_fixed_list_menu.first_displayed_item = h->first_displayed_item;
            MMI_fixed_list_menu.last_displayed_item = h->last_displayed_item;
            MMI_fixed_list_menu.displayed_items = h->displayed_items;
            MMI_fixed_list_menu.flags = h->flags;
            item_checked = h->state;
            for (i = 0; i < h->no_of_items; i++)
            {
                if (h->list_of_states[i] == 1)
                {
                    MMI_fixed_twostate_menuitems[i].flags |= UI_MENUITEM_STATE_SELECTED;
                }
                else
                {
                    MMI_fixed_twostate_menuitems[i].flags &= ~UI_MENUITEM_STATE_SELECTED;
                }
            }
            return (1);
        }
    }
    return (0);
}   /* end of set_list_menu_category140_history */


/*****************************************************************************
 * FUNCTION
 *  get_list_menu_category_history140
 * DESCRIPTION
 *  Gets the history data for the list menu of category140 (check box list)
 *  from the given history buffer
 * PARAMETERS
 *  history_ID          [IN]        The ID used by category screens
 *  history_buffer      [IN]        Buffer from which history data is taken
 * RETURNS
 *  void
 *****************************************************************************/
void get_list_menu_category_history140(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        U16 hID = (U16) (history_ID | 0x8000);
        S32 i = 0;

        list_menu_category_history_cat140 *h = (list_menu_category_history_cat140*) history_buffer;

        h->history_ID = hID;
        h->highlighted_item = (S16) MMI_fixed_list_menu.highlighted_item;
        h->first_displayed_item = (S16) MMI_fixed_list_menu.first_displayed_item;
        h->last_displayed_item = (S16) MMI_fixed_list_menu.last_displayed_item;
        h->displayed_items = (S16) MMI_fixed_list_menu.displayed_items;
        h->flags = MMI_fixed_list_menu.flags;
        h->no_of_items = MMI_fixed_list_menu.n_items;
        h->state = (S8) item_checked;
        for (i = 0; i < MMI_fixed_list_menu.n_items; i++)
        {
            if (MMI_fixed_twostate_menuitems[i].flags & UI_MENUITEM_STATE_SELECTED)
            {
                h->list_of_states[i] = 1;
            }
            else
            {
                h->list_of_states[i] = 0;
            }
        }
    }
}   /* end of get_list_menu_category_history140 */


/*****************************************************************************
 * FUNCTION
 *  GetCategory140HistorySize
 * DESCRIPTION
 *  Gets the history buffer size for Category140 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
S32 GetCategory140HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (sizeof(list_menu_category_history_cat140));
}   /* end of GetCategory140HistorySize */


/*****************************************************************************
 * FUNCTION
 *  GetCategory140History
 * DESCRIPTION
 *  Gets the history buffer for Category140 screen
 * PARAMETERS
 *  history_buffer      [IN]        History_buffer is a pointer to the buffer where the history data is to be stored      (pre-allocated)
 * RETURNS
 *  void
 *****************************************************************************/
U8 *GetCategory140History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history140(MMI_CATEGORY140_ID, history_buffer);
    return (history_buffer);
}   /* end of GetCategory140History */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory140Screen
 * DESCRIPTION
 *  Displays the Category140 screen
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon displayed with the title
 *  number_of_items             [IN]        Number of items in the menu
 *  list_of_items               [IN]        Array of items
 *  list_of_states              [?]         
 *  highlighted_item            [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer              [IN]        History buffer
 *  left_softkey_icon(?)        [IN]        Left softkey icon
 *  left_softkey(?)             [IN]        Left softkey label
 *  right_softkey_icon(?)       [IN]        Right softkey icon
 *  right_softkey(?)            [IN]        Right softkey label
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory140Screen(
        U16 title,
        U16 title_icon,
        S32 number_of_items,
        U8 **list_of_items,
        U8 *list_of_states,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    create_fixed_twostate_menuitems(get_image(CHECKBOX_ON_IMAGE_ID), get_image(CHECKBOX_OFF_IMAGE_ID));
    associate_fixed_twostate_list();

    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(STR_GLOBAL_OK),
        get_image(0),
        get_string(STR_GLOBAL_DONE),
        get_image(0),
        number_of_items);

    resize_fixed_twostate_menuitems(0, MMI_MENUITEM_HEIGHT);
    set_fixed_twostate_positions(MMI_MENUITEM_HEIGHT, 0, 1, 0);

    set_left_softkey_function(toggle_check_list_handle_left_softkey_up, KEY_EVENT_UP);

    checklist_category_menu_item_states = list_of_states;
    h_flag = set_list_menu_category140_history(MMI_CATEGORY140_ID, history_buffer);

    if (h_flag)
    {
        for (i = 0; i < number_of_items; i++)
        {
            if (MMI_fixed_twostate_menuitems[i].flags & UI_MENUITEM_STATE_SELECTED)
            {
                list_of_states[i] = 1;
            }
            else
            {
                list_of_states[i] = 0;
            }
        }

    }

    for (i = 0; i < number_of_items; i++)
    {
        add_fixed_twostate_item((UI_string_type) list_of_items[i]);
        if (list_of_states[i])
        {
            select_fixed_twostate_item(i);
        }
    }

    register_fixed_list_highlight_handler(toggle_check_list_highlight_handler);

    if (h_flag && item_checked)
    {   /* 2004-02-18, Check status in history if history is not null */
        change_right_softkey(STR_GLOBAL_DONE, 0);
        set_right_softkey_function(checkbox_toggle_DONE_RSK_function, KEY_EVENT_UP);
    }
    else
    {
        change_right_softkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
        set_right_softkey_function(checkbox_toggle_BACK_RSK_function, KEY_EVENT_UP);
        item_checked = 0;
    }
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    resize_fixed_twostate_menuitems_to_list_width();

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitListCategoryScreen;
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory140History, GetCategory140HistorySize);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY140_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory140Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory140Screen
 * DESCRIPTION
 *  Exits the category140 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory140Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
}   /* end of ExitCategory140Screen */

/* structure usedto store one big line in to two lines */

typedef struct _split_lines
{
    U8 str_line1[126];
    U8 str_line2[126];
    U8 no_of_lines;
} SPILTLINES;

SPILTLINES *split_buffer = NULL;


/*****************************************************************************
 * FUNCTION
 *  ShowCategory141Screen
 * DESCRIPTION
 *  Displays the Category141 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  message                 [IN]        Notification message
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  image(?)                [IN]        Notification image
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory141Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);

    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    gui_set_font(&MMI_medium_font);
    if (title == 0)
    {
        wgui_switch_title_with_status_bar = 1;
        ShowStatusIconsTitle();
    }
    else
    {
        MMI_title_string = get_string(title);
        MMI_title_icon = get_image(title_icon);
        wgui_switch_title_with_status_bar = 0;
    }
    l = gui_strlen((UI_string_type) message);
    create_multiline_inputbox_set_buffer((UI_string_type) message, l, l, 0);
    MMI_multiline_inputbox.flags |=
        UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY |
        UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE |
        UI_MULTI_LINE_INPUT_BOX_AUTO_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_USE_ENCODING_BASED_LENGTH;
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
#endif 

    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory141Screen;

    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    if (wgui_switch_title_with_status_bar == 1)
    {
        dm_data.S32CatId = MMI_CATEGORY141_STATUS_ICON_ID;
    }
    else
    {
        dm_data.S32CatId = MMI_CATEGORY141_ID;
    }
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory141Screen
 * DESCRIPTION
 *  Exits the Message Received screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory141Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_switch_title_with_status_bar)
    {
        close_status_icons();
        wgui_switch_title_with_status_bar = 0;
    }
    gui_hide_animations();
}   /* end of ExitCategory141Screen */


/*****************************************************************************
 * FUNCTION
 *  category_142_redraw_highlighted_item
 * DESCRIPTION
 *  Redraw the highlighted item(selected image) of category 142.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category_142_redraw_highlighted_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 index, img_width, img_height, vbar_x = 0, vbar_button_x = 0;
    PU8 img;
    color_t c;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(animation_handle);
    }
    if (category_142_image_list != NULL)
    {
        if (category_142_image_list_index != NULL)
        {
            index = *category_142_image_list_index;
        }
        else
        {
            index = 0;
        }

        if (!wgui_switch_title_with_status_bar && (category_142_list_of_titles != NULL))
        {
            MMI_title_string = (UI_string_type) category_142_list_of_titles[index];
            draw_title();
        }
        c = gui_color(255, 255, 255);
        gdi_layer_push_clip();

        if (r2lMMIFlag)
        {
            gdi_layer_set_clip(
                current_MMI_theme->scrollbar_size,
                (MMI_title_y + MMI_title_height),
                UI_device_width - 1,
                UI_device_height - MMI_button_bar_height - 1);
        }
        else
        {
            gdi_layer_set_clip(
                0,
                (MMI_title_y + MMI_title_height),
                UI_device_width - current_MMI_theme->scrollbar_size - 1,
                UI_device_height - MMI_button_bar_height - 1);
        }

        gui_fill_rectangle(
            0,
            (MMI_title_y + MMI_title_height),
            UI_device_width - 1,
            UI_device_height - MMI_button_bar_height - 1,
            c);
        if (category_142_image_list[index])
        {
            img = get_image(category_142_image_list[index]);
        }
        else
        {
            img = category_142_image_file_name_list[index];
        }
        gui_measure_image(img, &img_width, &img_height);
        if (r2lMMIFlag)
        {
            if (gdi_image_draw_animation((UI_device_width - img_width + current_MMI_theme->scrollbar_size) >> 1, (UI_device_height -MMI_button_bar_height -img_height + MMI_title_y + MMI_title_height) >> 1 , (U8*) img, &animation_handle) < 0)       
            {
                animation_handle = GDI_ERROR_HANDLE;
            }
        }
        else
        {
            if (gdi_image_draw_animation((UI_device_width - img_width) >> 1, (UI_device_height -MMI_button_bar_height -img_height + MMI_title_y + MMI_title_height) >> 1, (U8*) img, &animation_handle) < 0)   
            {
                animation_handle = GDI_ERROR_HANDLE;
            }
        }
        gdi_layer_pop_clip();

        /* Uncomment below line for auto-disable scrollbar    */
        /*    if(category_142_image_list_n_items>1)        */
        {
            gui_set_vertical_scrollbar_range(&category142_vbar, category_142_image_list_n_items);
            gui_set_vertical_scrollbar_scale(&category142_vbar, 1);
            gui_set_vertical_scrollbar_value(&category142_vbar, index);

            if (r2lMMIFlag)
            {
                vbar_x = category142_vbar.x;
                vbar_button_x = category142_vbar.scroll_button.x;
                category142_vbar.x = category142_vbar.x + category142_vbar.width - (UI_device_width - 1);
                category142_vbar.scroll_button.x =
                    category142_vbar.scroll_button.x + category142_vbar.scroll_button.width - (UI_device_width - 1);
                gui_show_vertical_scrollbar(&category142_vbar);
            }
            else
            {
                gui_show_vertical_scrollbar(&category142_vbar);
            }

            if (r2lMMIFlag)
            {
                category142_vbar.x = vbar_x;
                category142_vbar.scroll_button.x = vbar_button_x;
            }
        }
    }
    gdi_layer_blt_previous(
        0,
        (MMI_title_y + MMI_title_height),
        UI_device_width - 1,
        UI_device_height - MMI_button_bar_height);
}   /* end of category_142_redraw_highlighted_item */


/*****************************************************************************
 * FUNCTION
 *  category_142_highlight_handler
 * DESCRIPTION
 *  hightlight handler of category 142
 * PARAMETERS
 *  index       [IN]        The index of highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void category_142_highlight_handler(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = index + 1;
    if (!menu_shortcut_handler_display)
    {
        gui_redraw_menu_shortcut();
    }
    MMI_list_highlight_handler(index);
}   /* end of category_142_highlight_handler */


/*****************************************************************************
 * FUNCTION
 *  category_142_goto_item
 * DESCRIPTION
 *  go to specific item of category 142
 * PARAMETERS
 *  index       [IN]        The index of item to go
 * RETURNS
 *  void
 *****************************************************************************/
void category_142_goto_item(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (category_142_image_list_index != NULL)
    {
        if (index < 0)
        {
            return; /* index=0; */
        }
        if (index > (category_142_image_list_n_items - 1))
        {
            return; /* index=category_142_image_list_n_items-1;  */
        }
        *category_142_image_list_index = index;
    }
    category_142_highlight_handler(index);
    category_142_redraw_highlighted_item();
}   /* end of category_142_goto_item */


/*****************************************************************************
 * FUNCTION
 *  category_142_goto_previous_item
 * DESCRIPTION
 *  go to previous item of category142
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category_142_goto_previous_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 index = *category_142_image_list_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    index--;
    if (index < 0)
    {
        index = category_142_image_list_n_items - 1;
    }
    *category_142_image_list_index = index;
    category_142_redraw_highlighted_item();
    category_142_highlight_handler(index);
}   /* end of category_142_goto_previous_item */


/*****************************************************************************
 * FUNCTION
 *  category_142_goto_next_item
 * DESCRIPTION
 *  go to next item of category142
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category_142_goto_next_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 index = *category_142_image_list_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    index++;
    if (index > (category_142_image_list_n_items - 1))
    {
        index = 0;
    }
    *category_142_image_list_index = index;
    category_142_redraw_highlighted_item();
    category_142_highlight_handler(index);
}   /* end of category_142_goto_next_item */


/*****************************************************************************
 * FUNCTION
 *  category_142_key_handler
 * DESCRIPTION
 *  handle key handlers of catefory 142
 * PARAMETERS
 *  vkey_code       [IN]        To identify the key
 *  key_state       [IN]        The state of the key
 * RETURNS
 *  void
 *****************************************************************************/
void category_142_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:    /* left */
                break;
            case 39:    /* right */
                break;
            case 38:
                category_142_goto_previous_item();
                break;
            case 40:
                category_142_goto_next_item();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}   /* end of category_142_key_handler */

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  category_142_pen_down_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pos     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category_142_pen_down_hdlr(mmi_pen_point_struct pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_scrollbar_pen_enum scrollbar_event;
    gui_pen_event_param_struct scrollbar_param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (PEN_CHECK_BOUND
        (pos.x, pos.y, category142_vbar.x, category142_vbar.y, category142_vbar.width, category142_vbar.height))
    {
        gui_vertical_scrollbar_translate_pen_event(
            &category142_vbar,
            MMI_PEN_EVENT_DOWN,
            pos.x,
            pos.y,
            &scrollbar_event,
            &scrollbar_param);
        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
        {
            category_142_goto_item((S32) scrollbar_param._u.i);
        }
        return MMI_TRUE;
    }
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  category_142_pen_move_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pos     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category_142_pen_move_hdlr(mmi_pen_point_struct pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_scrollbar_pen_enum scrollbar_event;
    gui_pen_event_param_struct scrollbar_param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (PEN_CHECK_BOUND
        (pos.x, pos.y, category142_vbar.x, category142_vbar.y, category142_vbar.width, category142_vbar.height))
    {
        gui_vertical_scrollbar_translate_pen_event(
            &category142_vbar,
            MMI_PEN_EVENT_MOVE,
            pos.x,
            pos.y,
            &scrollbar_event,
            &scrollbar_param);
        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
        {
            category_142_goto_item((S32) scrollbar_param._u.i);
        }
        return MMI_TRUE;
    }
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  category_142_pen_repeat_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pos     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category_142_pen_repeat_hdlr(mmi_pen_point_struct pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_scrollbar_pen_enum scrollbar_event;
    gui_pen_event_param_struct scrollbar_param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (PEN_CHECK_BOUND
        (pos.x, pos.y, category142_vbar.x, category142_vbar.y, category142_vbar.width, category142_vbar.height))
    {
        gui_vertical_scrollbar_translate_pen_event(
            &category142_vbar,
            MMI_PEN_EVENT_REPEAT,
            pos.x,
            pos.y,
            &scrollbar_event,
            &scrollbar_param);
        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
        {
            category_142_goto_item((S32) scrollbar_param._u.i);
        }
        return MMI_TRUE;
    }
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  category_142_pen_up_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pos     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category_142_pen_up_hdlr(mmi_pen_point_struct pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_scrollbar_pen_enum scrollbar_event;
    gui_pen_event_param_struct scrollbar_param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (PEN_CHECK_BOUND
        (pos.x, pos.y, category142_vbar.x, category142_vbar.y, category142_vbar.width, category142_vbar.height))
    {
        gui_vertical_scrollbar_translate_pen_event(
            &category142_vbar,
            MMI_PEN_EVENT_UP,
            pos.x,
            pos.y,
            &scrollbar_event,
            &scrollbar_param);
        return MMI_TRUE;
    }
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  category_142_pen_abort_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pos     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category_142_pen_abort_hdlr(mmi_pen_point_struct pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_scrollbar_pen_enum scrollbar_event;
    gui_pen_event_param_struct scrollbar_param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (PEN_CHECK_BOUND
        (pos.x, pos.y, category142_vbar.x, category142_vbar.y, category142_vbar.width, category142_vbar.height))
    {
        gui_vertical_scrollbar_translate_pen_event(
            &category142_vbar,
            MMI_PEN_EVENT_ABORT,
            pos.x,
            pos.y,
            &scrollbar_event,
            &scrollbar_param);
        return MMI_TRUE;
    }
    return MMI_FALSE;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  DrawCate142CategoryControlArea
 * DESCRIPTION
 *  
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate142CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    category_142_redraw_highlighted_item();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory142Screen
 * DESCRIPTION
 *  Displays the picture list screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  list_of_titles          [IN]        List of string to displayed as image title
 *  list_of_images          [IN]        List of image IDs
 *  file_list_of_images     [IN]        List of image file name
 *  n_items                 [IN]        Number of images
 *  index                   [IN]        Currently displayed image index
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory142Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 **list_of_titles,
        U16 *list_of_images,
        U8 **file_list_of_images,
        S32 n_items,
        S32 *index,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 shortcut_width;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();
    MMI_menu_shortcut_number = -1;
    if (n_items < 2)//082406 shortcut
    {
        MMI_disable_title_shortcut_display = 1;
    }
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    if (title == NULL)
    {
        wgui_switch_title_with_status_bar = 1;
        ShowStatusIconsTitle();
    }
    else
    {
        MMI_title_icon = get_image(title_icon);
        wgui_switch_title_with_status_bar = 0;
    }

    MMI_menu_shortcut_number = *index + 1;
    category_142_image_list_index = index;
    h_flag = set_object_list_category_history(MMI_CATEGORY142_ID, history_buffer, category_142_image_list_index);
    if (h_flag)
    {
        *index = *category_142_image_list_index;
    }

    category_142_list_of_titles = list_of_titles;
    MMI_title_string = (UI_string_type) title;
    category_142_image_list = list_of_images;
    category_142_image_file_name_list = file_list_of_images;
    category_142_image_list_n_items = n_items;

    gui_create_vertical_scrollbar(
        &category142_vbar,
        UI_device_width - current_MMI_theme->scrollbar_size,
        (MMI_title_y + MMI_title_height),
        current_MMI_theme->scrollbar_size,
        MMI_content_height);
    register_fixed_list_shortcut_handler();
    shortcut_width = set_menu_item_count(n_items) + 7;
    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
    register_hide_menu_shortcut(gui_redraw_menu_shortcut);

    register_keyboard_key_handler(category_142_key_handler);
    register_menu_shortcut_keys();
    register_menu_shortcut_handler(category_142_goto_item);
    SetKeyHandler(category_142_goto_previous_item, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(category_142_goto_next_item, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(category_142_goto_previous_item, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(category_142_goto_next_item, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory142Screen;

    dm_setup_category_functions(dm_redraw_category_screen, GetCategory142History, dm_get_category_history_size);
    dm_register_category_controlled_callback(DrawCate142CategoryControlArea);
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_category_screen_control_area_pen_handlers(category_142_pen_down_hdlr, MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(category_142_pen_move_hdlr, MMI_PEN_EVENT_MOVE);
    wgui_register_category_screen_control_area_pen_handlers(category_142_pen_repeat_hdlr, MMI_PEN_EVENT_REPEAT);
    wgui_register_category_screen_control_area_pen_handlers(category_142_pen_up_hdlr, MMI_PEN_EVENT_UP);
    wgui_register_category_screen_control_area_pen_handlers(category_142_pen_abort_hdlr, MMI_PEN_EVENT_ABORT);
#endif /* __MMI_TOUCH_SCREEN__ */ 
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    if (wgui_switch_title_with_status_bar == 1)
    {
        dm_data.S32CatId = MMI_CATEGORY142_STATUS_ICON_ID;
    }
    else
    {
        dm_data.S32CatId = MMI_CATEGORY142_ID;
    }
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory142Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory142Screen
 * DESCRIPTION
 *  Exits the picture list screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory142Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_switch_title_with_status_bar)
    {
        close_status_icons();
        wgui_switch_title_with_status_bar = 0;
    }
    category_142_image_list = NULL;
    category_142_image_file_name_list = NULL;
    category_142_image_list_index = NULL;
    category_142_image_list_n_items = 0;
    category_142_list_of_titles = NULL;
    animation_handle = GDI_ERROR_HANDLE;
    gui_hide_animations();
}   /* end of ExitCategory142Screen */


/*****************************************************************************
 * FUNCTION
 *  GetCategory142History
 * DESCRIPTION
 *  Gets the history buffer for the picture list screen
 * PARAMETERS
 *  history_buffer      [IN]        Buffer to store history
 * RETURNS
 *  U8*                  pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory142History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_object_list_category_history(MMI_CATEGORY142_ID, history_buffer, category_142_image_list_index);
    return (history_buffer);
}

extern void gui_fixed_icontext_list_menuitem_scroll_handler(void);


/*****************************************************************************
 * FUNCTION
 *  DrawCate150CategoryControlArea
 * DESCRIPTION
 *  
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate150CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MMI_fixed_list_menu.n_items; i++)
    {
        MMI_fixed_list_menu.item_remove_highlight_function(
                                MMI_fixed_list_menu.items[i],
                                MMI_fixed_list_menu.common_item_data);
    }
    move_fixed_list(coordinate->S16X, coordinate->S16Y);
    show_fixed_list();
    gui_cancel_timer(gui_fixed_icontext_list_menuitem_scroll_handler);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory150Screen
 * DESCRIPTION
 *  Displays the category screen 153
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items
 *  list_of_items1          [IN]        Strings of item1
 *  list_of_items2          [IN]        Strings if item2
 *  highlighted_item        [IN]        Highlighted index
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory150Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items1,
        U8 **list_of_items2,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i, w;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    create_fixed_icontext_list_menuitems(2, 0);
    /* This might be a problem for draw manager. */
    w = UI_device_width - MMI_fixed_list_menu.vbar.width - 4;
     
    //      set_fixed_icontext_list_text_coordinates(0,2,0,w>>1,18);
    //---

    set_fixed_icontext_list_text_coordinates(0, 2, 0, (w >> 1), MMI_MENUITEM_HEIGHT);
    set_fixed_icontext_list_text_coordinates(1, (w >> 1), 0, (w >> 1), MMI_MENUITEM_HEIGHT);

    associate_fixed_icontext_list_list();

    ShowListCategoryScreen(
        get_string(title),
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    if (wgui_is_disable_shortcut_display() == MMI_TRUE)
    {
        MMI_disable_title_shortcut_display = 1;
        disable_menu_shortcut_box_display = 1;
    }

    MMI_fixed_icontext_list_menuitem.flags |= (UI_MENUITEM_RIGHT_JUSTIFY_COL2 | UI_MENUITEM_DISABLE_TEXT_CLIP);

    for (i = 0; i < number_of_items; i++)
    {
        fixed_icontext_list_item_insert(i);
        add_fixed_icontext_list_item_text(i, 0, (UI_string_type) list_of_items1[i]);
        add_fixed_icontext_list_item_text(i, 1, (UI_string_type) list_of_items2[i]);
    }

    h_flag = set_list_menu_category_history(MMI_CATEGORY150_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }

    resize_fixed_icontext_list_menuitems_to_list_width();

    for (i = 0; i < MMI_fixed_list_menu.n_items; i++)
    {
        MMI_fixed_list_menu.item_remove_highlight_function(
                                MMI_fixed_list_menu.items[i],
                                MMI_fixed_list_menu.common_item_data);
    }

    gui_cancel_timer(gui_fixed_icontext_list_menuitem_scroll_handler);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitListCategoryScreen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_register_category_controlled_callback(DrawCate150CategoryControlArea);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY150_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory150Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory150Screen
 * DESCRIPTION
 *  Exits the Games high scores category screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory150Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_icontext_list_menuitem_theme.flags &= ~(UI_MENUITEM_RIGHT_JUSTIFY_COL2 | UI_MENUITEM_DISABLE_TEXT_CLIP);
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();

    if (wgui_is_disable_shortcut_display() == MMI_TRUE)
    {
        wgui_reset_disable_shortcut_display();
        MMI_disable_title_shortcut_display = 0;
        disable_menu_shortcut_box_display = 0;
    }

}   /* end of ExitCategory150Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory152Screen
 * DESCRIPTION
 *  Exit category152 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory152Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_softkeys();
}   /* end of ExitCategory152Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory152Screen
 * DESCRIPTION
 *  Displays the category screen 152
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  message                 [IN]        Message
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
extern void PopupChangeCharColor(void);
#endif

void ShowCategory152Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);

    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_message_string = (UI_string_type) (message);
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
#endif 

    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND | DM_LEFT_ALIGN_TITLE;

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	PopupChangeCharColor();
#endif

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory152Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY152_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory152Screen */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory153Screen
 * DESCRIPTION
 *  Displays the category screen 153
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items
 *  list_of_items1          [IN]        Strings of item1
 *  list_of_items2          [IN]        Strings if item2
 *  highlighted_item        [IN]        Highlighted index
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory153Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items1,
        U8 **list_of_items2,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    highlighted_item = -1;
     
    /* gdi_layer_lock_frame_buffer(); */
    wgui_set_disable_shortcut_display();
    ShowCategory150Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        list_of_items1,
        list_of_items2,
        highlighted_item,
        history_buffer);
    /* Disable pen on list menu */
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_PEN;

    /* gdi_layer_unlock_frame_buffer(); */
     
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
    clear_key_handlers();
}   /* end of ShowCategory153Screen */


/*****************************************************************************
 * FUNCTION
 *  DrawCate154CategoryControlArea
 * DESCRIPTION
 *  
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate154CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 w1, h1, w2, h2, x;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_reset_clip();
    gui_set_font(current_UI_theme->multi_line_input_box_theme->text_font);
    gui_measure_string(MMI_message_string, &w1, &h1);
    gui_measure_string(MMI_message_string2, &w2, &h2);
#if defined(__MMI_MAINLCD_220X176__)
	if(ITEM_SCR_MISSED_CALL == g_dm_data.S32ScrId)
	{
		category_154_message1_y = (UI_device_height>>1) - h1 - 2;;
		category_154_message2_y = (UI_device_height>>1) + 2;
		gui_set_text_color(gui_color(255,255,255));
	}	
	else
#endif	
#if defined(__MMI_USING_BLACK_THEME_BG__)		    
    gui_set_text_color(*(current_MMI_theme->UI_white_color));
#else
    gui_set_text_color(*(current_MMI_theme->UI_black_color));
#endif
    if (Category154Align)
    {
        x = 2;
    }
    else
    {
        x = (UI_device_width >> 1) - (w1 >> 1);
        if (x < 2)
        {
            x = 2;
        }
    }

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x + gui_get_string_width((UI_string_type) MMI_message_string), category_154_message1_y);
    }
    else
    {
        gui_move_text_cursor(x, category_154_message1_y);
    }

    gui_set_line_height(h1);
    gui_print_text(MMI_message_string);
    if (Category154Align)
    {
        x = 2;
    }
    else
    {
        x = (UI_device_width >> 1) - (w2 >> 1);
        if (x < 2)
        {
            x = 2;
        }
    }

    if (r2lMMIFlag)
    {
        if (gui_get_string_width((UI_string_type) MMI_message_string2) >= UI_device_width)
        {
            x = UI_device_width - 1;
        }
        else
        {
            x += gui_get_string_width((UI_string_type) MMI_message_string2);
        }
    }

    gui_print_truncated_text(x, category_154_message2_y, UI_device_width - 2, MMI_message_string2);
    gdi_layer_set_clip(
        0,
        (MMI_title_y + MMI_title_height) + 1,
        UI_device_width - 1,
        UI_device_height - MMI_button_bar_height - 1);
#if defined(__MMI_MAINLCD_220X176__)
	if(ITEM_SCR_MISSED_CALL != g_dm_data.S32ScrId)
#endif
    wgui_show_transparent_animation();
}


/*****************************************************************************
 * FUNCTION
 *  Setcategory154LeftAlign
 * DESCRIPTION
 *  Set content of category154 left alignment
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Setcategory154LeftAlign(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Category154Align = 1;
}


/*****************************************************************************
 * FUNCTION
 *  ChangeCategory154Data
 * DESCRIPTION
 *  Change data of category154
 * PARAMETERS
 *  message1            [IN]        Data pointer of message1
 *  message2            [IN]        Data pointer of message2
 *  message_icon        [IN]        Id of message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeCategory154Data(U8 *message1, U8 *message2, U16 message_icon)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 w1, h1, w2, h2, w3, h3, y, th, rh;
    PU8 image;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_message_string = (UI_string_type) message1;
    MMI_message_string2 = (UI_string_type) message2;
    gui_set_font(current_UI_theme->multi_line_input_box_theme->text_font);
    gui_measure_string(MMI_message_string, &w1, &h1);
    gui_measure_string(MMI_message_string2, &w2, &h2);
    image = get_image(message_icon);
    gui_measure_image(image, &w3, &h3);
    th = h1 + h2 + h3 + 2;
    y = (MMI_title_y + MMI_title_height) + 1 + ((MMI_content_height - 1) >> 1) - (th >> 1);
    if (th < MMI_content_height)
    {
        rh = (MMI_content_height - th) >> 2;
    }
    else
    {
        rh = 0;
    }
    y -= rh;
    if (y < ((MMI_title_y + MMI_title_height) + 1))
    {
        y = (MMI_title_y + MMI_title_height) + 1;
    }
    category_154_message1_y = y;
    category_154_message2_y = y + h1;
    y += h1 + h2 + rh + 2;
    wgui_set_animation_image_y((UI_device_width >> 1), y, image);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory154Screen
 * DESCRIPTION
 *  Displays the Message Received screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message1                [IN]        Notification message 1
 *  message2                [IN]        Notification message 2
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Image          Notification image
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory154Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message1,
        U8 *message2,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 w1, h1, w2, h2, w3, h3, y, th, rh;
    PU8 image;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();
    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    if (title == 0)
    {
        wgui_switch_title_with_status_bar = 1;
        ShowStatusIconsTitle();
    }
    else
    {
        MMI_title_string = get_string(title);
        MMI_title_icon = get_image(title_icon);
        wgui_switch_title_with_status_bar = 0;
    }
    MMI_message_string = (UI_string_type) message1;
    MMI_message_string2 = (UI_string_type) message2;
    gui_set_font(current_UI_theme->multi_line_input_box_theme->text_font);
    gui_measure_string(MMI_message_string, &w1, &h1);
    gui_measure_string(MMI_message_string2, &w2, &h2);
    image = get_image(message_icon);
    gui_measure_image(image, &w3, &h3);
    th = h1 + 1 + h2 + 2 + h3;
    y = (MMI_title_y + MMI_title_height) + 1 + ((MMI_content_height - 1) >> 1) - (th >> 1);
    if (th < MMI_content_height)
    {
        rh = (MMI_content_height - th) >> 2;
    }
    else
    {
        rh = 0;
    }
    y -= rh;
    if (y < ((MMI_title_y + MMI_title_height) + 1))
    {
        y = (MMI_title_y + MMI_title_height) + 1;
    }
#if defined(__PROJECT_GALLITE_C01__)    
    if (y > (MMI_content_height + MMI_title_y))
    {
        y = (MMI_content_height + MMI_title_y) >> 1;
    }   
#endif    
    category_154_message1_y = y;
    category_154_message2_y = y + h1 + 1;
    y += h1 + 1 + h2 + rh + 2;
    wgui_set_animation_image_y((UI_device_width >> 1), y, image);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory154Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_register_category_controlled_callback(DrawCate154CategoryControlArea);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    if (wgui_switch_title_with_status_bar == 1)
    {
        dm_data.S32CatId = MMI_CATEGORY154_STATUS_ICON_ID;
    }
    else
    {
        dm_data.S32CatId = MMI_CATEGORY154_ID;
    }
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
#if defined(__MMI_MAINLCD_220X176__)
	set_status_icons_display(MMI_FALSE);
#endif
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory154Screen
 * DESCRIPTION
 *  Exits the Message Received screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory154Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_switch_title_with_status_bar)
    {
        close_status_icons();
        wgui_switch_title_with_status_bar = 0;
    }
    Category154Align = 0;
    MMI_message_string = NULL;
    MMI_message_string2 = NULL;
    gui_hide_animations();
}

#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
#endif /* defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) */ 

/*****************************************************************************
* [Category166]
*
* This category draws a full screen animation (align at center of screen)
* Used by power on/off animation
* This screen also able to mimic a static image with animation's behavior
*
*  **********************
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  *     Animation      *
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  *                    *
*  **********************
*
*****************************************************************************/
static void (*cat166_animation_complete_callback) (GDI_RESULT result);
static PS8 cat166_animation_name;
static U16 cat166_animation_id;
 S32 cat166_image_offset_x;
 S32 cat166_image_offset_y;
 S32 cat166_image_width;
 S32 cat166_image_height;
static BOOL cat166_is_gif_anim;
static U16 cat166_need_resize;
static BOOL cat166_is_invalid;


/*****************************************************************************
 * FUNCTION
 *  Cat166AnimationMimicTimer
 * DESCRIPTION
 *  Mimic animation's play complete call back function for static images
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void Cat166AnimationMimicTimer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Mimic GDI to return last frame callback */
    if (cat166_animation_complete_callback != NULL)
    {
        if (cat166_is_invalid)
        {
            /* error file */
            cat166_animation_complete_callback(-1);
        }
        else
        {
            cat166_animation_complete_callback(GDI_GIF_LAST_FRAME);
            gui_start_timer(1000, Cat166AnimationMimicTimer);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  DrawCate166CategoryControlArea
 * DESCRIPTION
 *  
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate166CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory166Screen();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory166Screen
 * DESCRIPTION
 *  Redraws sategory 166 screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory166Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    GDI_RESULT result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   TRACE_SYS_FUNCTION();
    gui_hide_animations();
    gdi_layer_clear(GDI_COLOR_BLACK);
    gui_reset_clip();

    if (cat166_animation_name != NULL)
    {
        /* source from file */
        if (cat166_is_gif_anim)
        {
            /* animation, use GDI's auto play animation */
            gdi_image_set_animation_last_frame_callback(cat166_animation_complete_callback);
            
            /* 070406 SVG support Start */
            if (cat166_need_resize)
            {
                result = gdi_image_draw_animation_resized_file(
                            cat166_image_offset_x,
                            cat166_image_offset_y,
                            cat166_image_width,
                            cat166_image_height,
                            (U8*) cat166_animation_name,
                            NULL);
            }
            else
            {
            result = gdi_image_draw_animation_file(
                        cat166_image_offset_x,
                        cat166_image_offset_y,
                        (U8*) cat166_animation_name,
                        NULL);
            }
            /* 070406 SVG support End */

            /* if is 1 frame gif, treat as still image */
            if (result == GDI_IMAGE_IS_STILL_IMAGE)
            {
                gui_start_timer(200, Cat166AnimationMimicTimer);
            }
        }
        else
        {
            if (cat166_need_resize)
            {
                /* static image */
                result = gdi_image_draw_resized_file(
                            cat166_image_offset_x,
                            cat166_image_offset_y,
                            cat166_image_width,
                            cat166_image_height,
                            cat166_animation_name);

                if (result < 0)
                {
                    /* invalid file */
                    cat166_is_invalid = TRUE;
                }

                gui_start_timer(200, Cat166AnimationMimicTimer);
            }
            else
            {
                /* static image */
                result = gdi_image_draw_file(cat166_image_offset_x, cat166_image_offset_y, cat166_animation_name);

                if (result < 0)
                {
                    /* invalid file */
                    cat166_is_invalid = TRUE;
                }

                gui_start_timer(200, Cat166AnimationMimicTimer);
            }
        }
    }
    else
    {
        /* source from id */
        if (cat166_is_gif_anim)
        {
            /* animation, use GDI's auto play animation */
            gdi_image_set_animation_last_frame_callback(cat166_animation_complete_callback);
	    TRACE_SYS_FUNCTION();
            result = gdi_image_draw_animation(cat166_image_offset_x, cat166_image_offset_y, (U8*) GetImage(cat166_animation_id), NULL);         
           	    TRACE_SYS_FUNCTION();
		    mmi_trace(g_sw_GDI,"result = %d", result);
            /* if is 1 frame gif, treat as still image */
            if (result == GDI_IMAGE_IS_STILL_IMAGE)
            {
                gui_start_timer(200, Cat166AnimationMimicTimer);
            }
        }
        else
        {
            /* static image */
            gdi_image_draw_id(cat166_image_offset_x, cat166_image_offset_y, cat166_animation_id);
            gui_start_timer(200, Cat166AnimationMimicTimer);
        }
    }
     TRACE_SYS_FUNCTION();

    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory166Screen
 * DESCRIPTION
 *  Show sategory 166 screen.
 * PARAMETERS
 *  image_id                        [IN]        Image id
 *  image_filename                  [IN]        Image_filename
 *  animation_complete_callback     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory166Screen(U16 image_id, S8 *image_filename, void (*animation_complete_callback) (GDI_RESULT))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 image_width;
    S32 image_height;
    GDI_RESULT ret;

 
    U16 img_type;

  

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* 
     * If image_filename != NULL, means will use image from file as first priority.
     * We will use image_id when the file is not able to play correctly. 
     */

    /* init var */
    cat166_animation_complete_callback = animation_complete_callback;

    cat166_animation_id = image_id;
    cat166_animation_name = image_filename;
    cat166_image_offset_x = 0;
    cat166_image_offset_y = 0;
    cat166_is_gif_anim = FALSE;
    cat166_need_resize = FALSE;
    cat166_is_invalid = FALSE;
#ifdef __MMI_NOKIA_STYLE_N800__
    dm_reset_context();
#endif
    /* full screen category */
    entry_full_screen();

    /* clear all key handlers */
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();

   /*** get image parameters ***/

    /* source from file */
    if (cat166_animation_name != NULL)
    {
        ret = gdi_image_get_dimension_file(cat166_animation_name, &image_width, &image_height);

        if (ret < 0)
        {
            /* set filename to NULL, will skip play this file */
            cat166_animation_name = NULL;
            gdi_layer_clear(GDI_COLOR_BLACK);
            gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

            if (cat166_animation_complete_callback != NULL)
            {
                cat166_animation_complete_callback(-1);
            }

            return;
        }
        else
        {
            /* check if is gif animation or not */

            switch (gdi_image_get_type_from_file(cat166_animation_name))
            {
                case GDI_IMAGE_TYPE_JPG_FILE:
                case GDI_IMAGE_TYPE_PNG_FILE://082906 png resize
                #if 1//gdm
                case GDI_IMAGE_TYPE_DEVICE_BITMAP:
                case GDI_IMAGE_TYPE_DEVICE_BITMAP_SEQUENCE:
                #endif
                    cat166_is_gif_anim = FALSE;
                    cat166_need_resize = TRUE;
                    break;

                case GDI_IMAGE_TYPE_GIF_FILE:
                case GDI_IMAGE_TYPE_SVG_FILE://070406 SVG support
                    cat166_is_gif_anim = TRUE;
                    cat166_need_resize = TRUE;//070406 resize screensaver
                    break;

                case GDI_IMAGE_TYPE_M3D_FILE:
                    cat166_is_gif_anim = TRUE;
                    cat166_need_resize = FALSE;		
                    break;

                default:
                    cat166_is_gif_anim = FALSE;
                    cat166_need_resize = FALSE;
                    break;
            }

            if (cat166_need_resize == TRUE)
            {
                if ((image_width <= UI_device_width) && (image_height <= UI_device_height))
                {
                    cat166_need_resize = FALSE;
                }
            }
        }
    }

    /* source from ID */
    if (cat166_animation_id != 0)
    {
        /* we assume internal resource is always correct */
        gdi_image_get_dimension_id(cat166_animation_id, &image_width, &image_height);

        /* check if is gif animation or not */
 
        img_type = gdi_image_get_type_from_id(cat166_animation_id);
    #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
        if (img_type == IMAGE_TYPE_GIF || img_type == IMAGE_TYPE_GIF_FILE_OFFSET || img_type == IMAGE_TYPE_M3D)
    #else /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 
        if (img_type == IMAGE_TYPE_GIF || img_type == IMAGE_TYPE_M3D)
    #endif /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 
 
        {
            cat166_is_gif_anim = TRUE;
        }
        else
        {
            cat166_is_gif_anim = FALSE; /* still image */
        }
    }
	
    /* calc image draw pos, check if need resize */
    if (cat166_need_resize)
    {

        gdi_image_util_fit_bbox(
            UI_device_width,
            UI_device_height,
            image_width,
            image_height,
            &cat166_image_offset_x,
            &cat166_image_offset_y,
            &cat166_image_width,
            &cat166_image_height);
    }
    else
    {
        cat166_image_offset_x = (UI_device_width - image_width) >> 1;
        cat166_image_offset_y = (UI_device_height - image_height) >> 1;
    }
	mmi_trace(g_sw_GDI, "$$$cat166_is_gif_anim = %d", cat166_is_gif_anim);
    ExitCategoryFunction = ExitCategory166Screen;
    dm_data.S32CatId = MMI_CATEGORY166_ID;
    dm_data.S32flags = DM_NO_FLAGS;
    dm_data.S32ScrId = GetActiveScreenId();
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawCate166CategoryControlArea);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory166Screen
 * DESCRIPTION
 *  Exit Category166 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory166Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* stop animtion */
    gdi_image_stop_animation_all();
    gui_cancel_timer(Cat166AnimationMimicTimer);

    cat166_animation_complete_callback = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  StopCategory166Animation
 * DESCRIPTION
 *  Stop Category166 screen's animation
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void StopCategory166Animation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* stop animtion */
    gdi_image_stop_animation_all();
    gui_cancel_timer(Cat166AnimationMimicTimer);
}

/* Category 170: SAT list menu screen (Equivalent to Cat-32)         */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory170Screen
 * DESCRIPTION
 *  Displays the Dynamic menu screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items
 *  list_of_items           [IN]        Array of items
 *  list_of_icons           [IN]        Array of icons
 *  flags                   [IN]        Flags (Always set to 0. Reserved for future use.)
 *  highlighted_item        [IN]        Highlighted index
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory170Screen(
        U8 *title,
        PU8 title_icon,
        U8 *left_softkey,
        PU8 left_softkey_icon,
        U8 *right_softkey,
        PU8 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        PU8 *list_of_icons,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 flag = 0;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(flags);
    MMI_disable_title_shortcut_display = 0;
    disable_menu_shortcut_box_display = 0;

    gdi_layer_lock_frame_buffer();

    create_fixed_icontext_menuitems();
    resize_fixed_list(MMI_content_width, MMI_content_height);
    associate_fixed_icontext_list();

    ShowListCategoryScreen(
        (UI_string_type) title,
        title_icon,
        (UI_string_type) left_softkey,
        left_softkey_icon,
        (UI_string_type) right_softkey,
        right_softkey_icon,
        number_of_items);

    for (i = 0; i < number_of_items; i++)
    {
        add_fixed_icontext_item((UI_string_type) list_of_items[i], list_of_icons[i]);
        if (list_of_icons[i] != UI_NULL_IMAGE)
        {
            flag = 1;
        }
    }
    resize_fixed_icontext_menuitems(0, get_menu_item_height());
    if (flag)
    {
        set_fixed_icontext_positions(MMI_MENUITEM_HEIGHT + 2, 0, 1, 0);
    }
    else
    {
        set_fixed_icontext_positions(1, 0, 1, 0);
        MMI_fixed_icontext_menuitem.scroll_width = MMI_fixed_icontext_menuitem.width - 2;
    }

    h_flag = set_list_menu_category_history(MMI_CATEGORY170_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory170Screen;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY170_ID;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory170Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory170Screen
 * DESCRIPTION
 *  Exits the dynamic menu screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory170Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    disable_menu_shortcut_box_display = 0;
    close_scrolling_title();
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
}   /* end of ExitCategory170Screen */

/* Category 172: SAT Radio list (Equivalent to Cat-107)           */


/*****************************************************************************
 * FUNCTION
 *  category172_list_highlight_handler
 * DESCRIPTION
 *  highlight handler of category 172
 * PARAMETERS
 *  item_index      [IN]        Index of highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void category172_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
#ifndef __MMI_RADIO_BUTTON_HIGHLIGHT_NO_SELECT__
    radio_list_handle_item_select_no_draw(item_index);
#endif 
    MMI_highlighted_item_text = get_item_text(item_index);
    gdi_layer_lock_frame_buffer();
    MMI_list_highlight_handler(item_index);
    gdi_layer_unlock_frame_buffer();
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}   /* end of category172_list_highlight_handler */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory172Screen
 * DESCRIPTION
 *  Displays the Dynamic radio list
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items
 *  list_of_items           [IN]        Array of items
 *  flags                   [IN]        Flags (Always set to 0. Reserved for future use.)
 *  highlighted_item        [IN]        Highlighted index
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory172Screen(
        U8 *title,
        PU8 title_icon,
        U8 *left_softkey,
        PU8 left_softkey_icon,
        U8 *right_softkey,
        PU8 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(flags);
    MMI_disable_title_shortcut_display = 1;
    disable_menu_shortcut_box_display = 1;

    gdi_layer_lock_frame_buffer();

    create_fixed_twostate_menuitems(get_image(RADIO_ON_IMAGE_ID), get_image(RADIO_OFF_IMAGE_ID));
    resize_fixed_list(MMI_content_width, MMI_content_height);
    associate_fixed_twostate_list();
    ShowListCategoryScreen(
        (UI_string_type) title,
        title_icon,
        (UI_string_type) left_softkey,
        left_softkey_icon,
        (UI_string_type) right_softkey,
        right_softkey_icon,
        number_of_items);

    resize_fixed_twostate_menuitems(0, MMI_MENUITEM_HEIGHT);
    set_fixed_twostate_positions(MMI_MENUITEM_HEIGHT, 0, 1, 0);

    register_hide_menu_shortcut(UI_dummy_function);

    for (i = 0; i < number_of_items; i++)
    {
        add_fixed_twostate_item((UI_string_type) list_of_items[i]);
    }

    register_fixed_list_highlight_handler(category172_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY172_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    select_fixed_twostate_item(MMI_fixed_list_menu.highlighted_item);
    setup_scrolling_title();

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory172Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY172_ID;
    dm_data.S32flags = DM_SCROLL_TITLE;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory172Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory172Screen
 * DESCRIPTION
 *  Exits the dynamic menu screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory172Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    disable_menu_shortcut_box_display = 0;
    close_scrolling_title();
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
}   /* end of ExitCategory172Screen */

/* Category 174: SAT list menu screen (Equivalent to Cat-32), with pop up hints  */


/*****************************************************************************
 * FUNCTION
 *  Category174ChangeItemText
 * DESCRIPTION
 *  chaneg text of particalue menu item ofcategory 174
 * PARAMETERS
 *  index       [IN]        Index of the menuitem
 *  text        [IN]        String of menuitem text
 * RETURNS
 *  void
 *****************************************************************************/
void Category174ChangeItemText(S32 index, U8 *text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_icontext_menuitems[index].item_text = (UI_string_type) text;
}   /* end of Category174ChangeItemText */


/*****************************************************************************
 * FUNCTION
 *  Category174ChangeItemIcon
 * DESCRIPTION
 *  chaneg icon of particalue menu item ofcategory 174
 * PARAMETERS
 *  index           [IN]        Index of the menuitem
 *  image_ID        [IN]        Image id of menuitem icon
 * RETURNS
 *  void
 *****************************************************************************/
void Category174ChangeItemIcon(S32 index, U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_icontext_menuitems[index].item_icon = (PU8) get_image(image_ID);
}   /* end of Category174ChangeItemIcon *//* 0331 */


/*****************************************************************************
 * FUNCTION
 *  Category174ChangeItemDescription
 * DESCRIPTION
 *  chaneg description of popup of particalue menu item ofcategory 174
 * PARAMETERS
 *  index       [IN]        Index of the menuitem
 *  text        [IN]        String value of hint data of menuitem.
 * RETURNS
 *  void
 *****************************************************************************/
void Category174ChangeItemDescription(S32 index, U8 *text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_pop_up_description_strings[index].text_strings[0] = (UI_string_type) text;

#ifdef __MMI_UI_HINT_TOGGLE_TRANSITION__
    wgui_setup_pop_up_description_transition();
#endif 
}   /* end of Category174ChangeItemDescription */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory174Screen
 * DESCRIPTION
 *  Displays the Dynamic menu screen with description.
 * PARAMETERS
 *  title                       [IN]        Title for the screen
 *  title_icon                  [IN]        Icon displayed with the title
 *  left_softkey                [IN]        Left softkey label
 *  left_softkey_icon           [IN]        Left softkey icon
 *  right_softkey               [IN]        Right softkey label
 *  right_softkey_icon          [IN]        Right softkey icon
 *  number_of_items             [IN]        Number of items
 *  list_of_items               [IN]        Array of items
 *  list_of_icons               [IN]        Array of icons
 *  list_of_descriptions        [IN]        
 *  flags                       [IN]        Flags (Always set to 0. Reserved for future use.)
 *  highlighted_item            [IN]        Highlighted index
 *  history_buffer              [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory174Screen(
        U8 *title,
        PU8 title_icon,
        U8 *left_softkey,
        PU8 left_softkey_icon,
        U8 *right_softkey,
        PU8 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        PU8 *list_of_icons,
        U8 **list_of_descriptions,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 flag = 0;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(flags);
    MMI_disable_title_shortcut_display = 1;
    disable_menu_shortcut_box_display = 1;

    gdi_layer_lock_frame_buffer();

    create_fixed_icontext_menuitems();
    associate_fixed_icontext_list();

    ShowListCategoryScreen(
        (UI_string_type) title,
        title_icon,
        (UI_string_type) left_softkey,
        left_softkey_icon,
        (UI_string_type) right_softkey,
        right_softkey_icon,
        number_of_items);

    setup_scrolling_title();

    if (list_of_descriptions == NULL)
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_icontext_item((UI_string_type) list_of_items[i], (PU8) list_of_icons[i]);
            wgui_pop_up_description_strings[i].text_strings[0] = NULL;
            if (list_of_icons[i] != UI_NULL_IMAGE)
            {
                flag = 1;
            }
        }
    }
    else
    {
        for (i = 0; i < number_of_items; i++)
        {
            add_fixed_icontext_item((UI_string_type) list_of_items[i], (PU8) list_of_icons[i]);
            wgui_pop_up_description_strings[i].text_strings[0] = (UI_string_type) list_of_descriptions[i];
            if (list_of_icons[i] != UI_NULL_IMAGE)
            {
                flag = 1;
            }
        }
    }
    resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);     
    if (flag)
    {
        set_fixed_icontext_positions(MMI_MENUITEM_HEIGHT + 2, 0, 1, 0);
    }
    else
    {
        set_fixed_icontext_positions(1, 0, 1, 0);
        MMI_fixed_icontext_menuitem.scroll_width = MMI_fixed_icontext_menuitem.width - 2;
    }

    h_flag = set_list_menu_category_history(MMI_CATEGORY174_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    set_pop_up_descriptions(1, number_of_items, MMI_fixed_list_menu.highlighted_item);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory174Screen;
    dm_data.S32flags = DM_SCROLL_TITLE;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY174_ID;
    dm_data.S32flags |= DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory174Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory174Screen
 * DESCRIPTION
 *  Exits the dynamic menu screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory174Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_disable_title_shortcut_display = 0;
    disable_menu_shortcut_box_display = 0;

    close_scrolling_title();
    ClearHighlightHandler();
    reset_softkeys();
    reset_pop_up_descriptions();
    reset_menu_shortcut_handler();
    reset_fixed_list();
}   /* end of ExitCategory174Screen */

#ifdef __J2ME__
/*****************************************************************************
 * FUNCTION
 *  RedrawCategory176Screen
 * DESCRIPTION
 *  Redraws the dynamic menu screen (For SIM Application Toolkit)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
//////////////////////////////////////////////////////////////////////
///
///     Category 176 is used by java.
///                     It is modify from Category 170.. 
///                     and remove keypad register, scrolling title, shortcut, blt out
///             
void RedrawCategory176Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_filled_area *f = current_MMI_theme->general_background_filler;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    clear_buttonbar();
    gui_reset_clip();
    gui_draw_filled_area(0, 0, 24, MMI_multiline_inputbox.height, f);
    draw_title();
    move_fixed_list(0,MMI_title_height);
    show_fixed_list();
    show_softkey_background();
    show_left_softkey();
    show_right_softkey();
    gdi_layer_unlock_frame_buffer();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory176Screen
 * DESCRIPTION
 *  Displays the Dynamic menu screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items
 *  list_of_items           [IN]        Array of items
 *  list_of_icons           [IN]        Array of icons
 *  flags                   [IN]        Flags (Always set to 0. Reserved for future use.)
 *  highlighted_item        [IN]        Highlighted index
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory176Screen(
        U8 *title,
        PU8 title_icon,
        U8 *left_softkey,
        PU8 left_softkey_icon,
        U8 *right_softkey,
        PU8 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items,
        PU8 *list_of_icons,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    U8 flag = 0;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(flags);
    MMI_disable_title_shortcut_display = 1;
    gdi_layer_lock_frame_buffer();

    create_fixed_icontext_menuitems();
    associate_fixed_icontext_list();
    MMI_fixed_icontext_menuitem.flags &= ~UI_MENUITEM_MARQUEE_SCROLL;

    /* Title */
    MMI_title_string = (UI_string_type) title;
    MMI_title_icon = title_icon;

    /* ShortCut */
    MMI_menu_shortcut_number = -1;

    /* Softkeys */
    set_left_softkey_label((UI_string_type) left_softkey);
    set_left_softkey_icon(left_softkey_icon);
    set_right_softkey_label((UI_string_type) right_softkey);
    set_right_softkey_icon(right_softkey_icon);

    /* List */
    MMI_current_menu_type = LIST_MENU;
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_block_list_effect();
#endif 

    for (i = 0; i < number_of_items; i++)
    {
        add_fixed_icontext_item((UI_string_type) list_of_items[i], list_of_icons[i]);
        if (list_of_icons[i] != UI_NULL_IMAGE)
        {
            flag = 1;
        }
    }

    resize_fixed_icontext_menuitems(0, MMI_ICONTEXT_MENUITEM_HEIGHT);
    if (flag)
    {
        set_fixed_icontext_positions(MMI_MENUITEM_HEIGHT + 2, 0, 1, 0);
    }
    else
    {
        set_fixed_icontext_positions(1, 0, 1, 0);
        MMI_fixed_icontext_menuitem.scroll_width = MMI_fixed_icontext_menuitem.width - 2;
    }

    h_flag = set_list_menu_category_history(MMI_CATEGORY176_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }

    gui_set_multilayer_mask(gui_get_multilayer_mask() & ~UI_MUL_BKG_SUBMENU);
    gui_set_multilayer_mask(gui_get_multilayer_mask() & ~UI_MUL_BKG_MAINMENU);

    gdi_layer_unlock_frame_buffer();
    MMI_fixed_list_menu.act_lcd_handle = GDI_LCD_MAIN_LCD_HANDLE;
    MMI_fixed_list_menu.act_layer_handle = jui_layer_handle;

    SetListScreenFunctions(MMI_CATEGORY176_ID);
    RedrawCategoryFunction = RedrawCategory176Screen;
    ExitCategoryFunction = ExitCategory176Screen;
    RedrawCategoryFunction();

}   /* end of ShowCategory176Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory176Screen
 * DESCRIPTION
 *  Exits the dynamic menu screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory176Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    disable_menu_shortcut_box_display = 0;
    close_scrolling_title();
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    MMI_fixed_list_menu.act_lcd_handle = GDI_LCD_MAIN_LCD_HANDLE;
    MMI_fixed_list_menu.act_layer_handle = GDI_LAYER_MAIN_BASE_LAYER_HANDLE;
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
}
#endif

/*****************************************************************************
 * FUNCTION
 *  redraw_fixed_list_and_title_bar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_fixed_list_and_title_bar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if(UI_BLINKING_CURSOR_SUPPORT)
    StopMyTimer(BLINKING_CURSOR);
#endif 
    draw_title();
    redraw_fixed_list();
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetRightSoftkeyFunction(UI_dummy_function, KEY_EVENT_DOWN);

}

/* Phonebook   */


/*****************************************************************************
 * FUNCTION
 *  show_phonebook_list_inputbox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pbName      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void show_phonebook_list_inputbox(PU8 pbName)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S32 inputbox_x, inputbox_y, inputbox_width, inputbox_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_setup_input_information(0, 1, UI_device_width >> 1, (MMI_title_y + MMI_title_height) - 2);
    inputbox_x = wgui_inputbox_information_bar_width;
    inputbox_y = 0;
    inputbox_width = UI_device_width - wgui_inputbox_information_bar_width;
    inputbox_height = MMI_title_height;
    wgui_setup_singleline_inputbox(
        inputbox_x,
        inputbox_y,
        inputbox_width,
        inputbox_height,
        pbName,
        7,
        MMI_CATEGORY53_ID,
        get_string(STR_GLOBAL_BACK),
        get_image(IMG_GLOBAL_BACK),
        INPUT_TYPE_ALPHANUMERIC_LOWERCASE | INPUT_TYPE_USE_ENCODING_BASED_LENGTH,
        NULL,
        1);
    wgui_singleline_inputbox_RSK_function = redraw_fixed_list_and_title_bar;
}

/* CSD end */
static S32 input_box_y = 0, input_box_x = 0;
static U16 cat200_search_img = 0;

S32(*cat200_search_function) (U8 *);

UI_filled_area yellowcat200wgui_color_dialog_background = {UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
    {238, 243, 33, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};

UI_filled_area cat200_inputbox_BG_filler = 
{
    UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
    {255, 222, 255, 100},
    {0, 0, 0, 0},
    {255, 255, 255, 100},
    {255, 255, 255, 100},
    0
};

/* theme of single input box of enter pin screen */

UI_single_line_input_box_theme cat200_singleline_inputbox_theme = { &cat200_inputbox_BG_filler,
    &cat200_inputbox_BG_filler,
    &cat200_inputbox_BG_filler,
    {0, 0, 0, 100},
    {128, 128, 128, 100},
    {0, 0, 0, 100},
    {255, 255, 255, 100},
    {51, 88, 171, 100},
    {255, 0, 0, 100},
    &UI_DEFAULT_FONT,
    1,
    UI_SINGLE_LINE_INPUT_BOX_CENTER_Y,
    '*'
};


/*****************************************************************************
 * FUNCTION
 *  SetCategory200LeftSoftkeyFunction
 * DESCRIPTION
 *  set the right soft key function handler of category 200
 * PARAMETERS
 *  f               [IN]        
 *  k               [IN]        
 *  function(?)     [IN]        Pointer
 *  key(?)          [IN]        Event type
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory200LeftSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_singleline_inputbox_LSK_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory200RightSoftkeyFunction
 * DESCRIPTION
 *  set the right soft key function handler of category 200
 * PARAMETERS
 *  f               [IN]        
 *  k               [IN]        
 *  function(?)     [IN]        Pointer
 *  key(?)          [IN]        Event type
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory200RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(k);
    wgui_singleline_inputbox_RSK_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  RegisterCat200SearchFunction
 * DESCRIPTION
 *  search function register by application
 * PARAMETERS
 *  search_function     [IN]        
 *  function(?)         [IN]        Pointer
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterCat200SearchFunction(S32(*search_function) (U8 *))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (search_function)
    {
        cat200_search_function = search_function;
    }
}

BOOL(*wgui_check_application_present) (S32 *max_number) = NULL;


/*****************************************************************************
 * FUNCTION
 *  Register_appl_present_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  application_present_function        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Register_appl_present_function(BOOL(*application_present_function) (S32 *max_number))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_check_application_present = application_present_function;
}


/*****************************************************************************
 * FUNCTION
 *  refresh_search_list_change_list
 * DESCRIPTION
 *  input call back function of category200 multi tap
 * PARAMETERS
 *  no_entries                  [IN]        
 *  UI_character_type(?)        [IN]        Inp
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef __MMI_VIP_FUNCTION__
 #include "securitysetup.h"
 extern void mmi_VIP_get_phb_suited_number_qsearch(U16 qCount);
 #endif
void refresh_search_list_change_list(S32 no_entries)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 shortcut_width;
    S32 max_entries = 0;
    BOOL showOption, application_present;
	U16 nVIPSuited = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	#ifdef __MMI_VIP_FUNCTION__
	mmi_VIP_get_phb_suited_number_qsearch(no_entries);
	nVIPSuited = g_vip_context.nPHBSuited;
	#endif
    MMI_fixed_list_menu.highlighted_item = 0;
    MMI_fixed_list_menu.n_items = no_entries - nVIPSuited;
    MMI_fixed_list_menu.displayed_items = no_entries - nVIPSuited;

	

    if (wgui_check_application_present)
    {
        application_present = wgui_check_application_present(&max_entries);
    }
    else
    {
        application_present = MMI_FALSE;
    }

    if (application_present)
    {

        shortcut_width = set_menu_item_count(max_entries) + 7;  /* number of words goes beyond this limit, and increases at the run time */
    }
    else
    {
        shortcut_width = set_menu_item_count(no_entries) + 7;
    }

    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    move_menu_shortcut_handler(UI_device_width - shortcut_width, MMI_title_y);

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    /* to handle an issue on empty search ie no entery in phonebook of same name */
    show_softkey_background();
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 

    if (application_present)
    {
        showOption = no_entries >= 0 ? MMI_TRUE : MMI_FALSE;
    }
    else
    {
        showOption = no_entries > 0 ? MMI_TRUE : MMI_FALSE;
    }

    if (showOption)
    {
        set_left_softkey_label(wgui_singleline_inputbox_LSK_label_string);
        set_left_softkey_icon(wgui_singleline_inputbox_LSK_label_icon);
        redraw_left_softkey();
        register_left_softkey_handler();
        set_left_softkey_function(wgui_singleline_inputbox_LSK_function, KEY_EVENT_UP);
        set_left_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
        dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
        draw_title();
    }
    else
    {
        set_left_softkey_label(NULL);
        set_left_softkey_icon(NULL);
        redraw_left_softkey();
        set_left_softkey_function(UI_dummy_function, KEY_EVENT_UP);
        set_left_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
        gui_fixed_icontext_menuitem_stop_scroll();
        gui_pop_up_description_stop_scroll();
        MMI_menu_shortcut_number = 0;
        draw_title();
        wgui_current_pop_up_description_index = -1;
    }
    show_dynamic_list();
}


/*****************************************************************************
 * FUNCTION
 *  refresh_search_list_change_list_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  no_entries              [IN]        
 *  highlighted_item        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void refresh_search_list_change_list_ext(S32 no_entries, S32 highlighted_item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 shortcut_width, max_entries = 0;
    BOOL showOption, application_present;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_list_menu.highlighted_item = highlighted_item;
    MMI_fixed_list_menu.n_items = no_entries;
    MMI_fixed_list_menu.displayed_items = no_entries;

    if (wgui_check_application_present)
    {
        application_present = wgui_check_application_present(&max_entries);     /* true in case of dictionary */
    }
    else
    {
        application_present = MMI_FALSE;
    }

    if (application_present)
    {

        shortcut_width = set_menu_item_count(max_entries) + 7;  /* number of words goes beyond this limit, and increases at the run time */
    }
    else
    {
        shortcut_width = set_menu_item_count(no_entries) + 7;
    }

    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    move_menu_shortcut_handler(UI_device_width - shortcut_width, MMI_title_y);
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    /* to handle an issue on empty search ie no entery in phonebook of same name */
    show_softkey_background();
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 

    if (application_present)
    {
        showOption = no_entries >= 0 ? MMI_TRUE : MMI_FALSE;
    }
    else
    {
        showOption = no_entries > 0 ? MMI_TRUE : MMI_FALSE;
    }

    if (showOption)
    {
        set_left_softkey_label(wgui_singleline_inputbox_LSK_label_string);
        set_left_softkey_icon(wgui_singleline_inputbox_LSK_label_icon);
        redraw_left_softkey();
        register_left_softkey_handler();
        set_left_softkey_function(wgui_singleline_inputbox_LSK_function, KEY_EVENT_UP);
        set_left_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
        dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
        draw_title();
    }
    else
    {
        set_left_softkey_label(NULL);
        set_left_softkey_icon(NULL);
        redraw_left_softkey();
        set_left_softkey_function(UI_dummy_function, KEY_EVENT_UP);
        set_left_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
        gui_fixed_icontext_menuitem_stop_scroll();
        gui_pop_up_description_stop_scroll();
        MMI_menu_shortcut_number = 0;
        draw_title();
        wgui_current_pop_up_description_index = -1;
    }
    show_dynamic_list();
}


/*****************************************************************************
 * FUNCTION
 *  refresh_search_list
 * DESCRIPTION
 *  
 * PARAMETERS
 *  name        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void refresh_search_list(U8 *name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 no_entries = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    no_entries = cat200_search_function(name);
    load_dynamic_item_buffer(no_entries);

    if (no_entries >= 0)
    {
        refresh_search_list_change_list(no_entries);

        gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
    }
}


/*****************************************************************************
 * FUNCTION
 *  Cat200multitap_inputcallback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  inp     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Cat200multitap_inputcallback(UI_character_type inp)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 no_entries = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (inp != 0)
    {
        gui_single_line_input_box_insert_multitap_character(&MMI_singleline_inputbox, inp);
    }
    if (!gui_single_line_input_box_test_overflow(&MMI_singleline_inputbox))
    {
        if (cat200_search_function)
        {
            no_entries = cat200_search_function(MMI_singleline_inputbox.text);
            load_dynamic_item_buffer(no_entries);

            if (no_entries >= 0)
            {
                refresh_search_list_change_list(no_entries);

            }
            singleline_inputbox_input_callback();
            redraw_singleline_inputbox();
            gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  Cat200multitap_input_complete_callback
 * DESCRIPTION
 *  input call back complete function of category200 multi tap
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Cat200multitap_input_complete_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_confirm_multitap_character(&MMI_singleline_inputbox);
    singleline_inputbox_input_callback();
    redraw_singleline_inputbox();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}

extern void wgui_redraw_singleline_input_information_bar(void);

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  Cate200CategoryControlAreaPenDownHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate200CategoryControlAreaPenDownHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret = MMI_FALSE;
    gui_list_pen_enum menu_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_list_menu.pen_redraw_menu_function = show_dynamic_list;
    gui_vertical_scrollbar_set_pen_self_scroll(&MMI_fixed_list_menu.vbar, MMI_TRUE);
    gui_fixed_list_menu_set_pen_scroll_delay(&MMI_fixed_list_menu, GUI_DYNAMIC_LIST_PEN_SCROLL_DELAY);

    ret = gui_dynamic_list_menu_translate_pen_event(
            &MMI_fixed_list_menu,
            MMI_PEN_EVENT_DOWN,
            point.x,
            point.y,
            &menu_event);
    if (ret)
    {
        if (menu_event == GUI_LIST_PEN_HIGHLIGHT_CHANGED || menu_event == GUI_LIST_PEN_NEED_REDRAW)
        {
            show_dynamic_list();
        }
        return MMI_TRUE;
    }

    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  Cate200CategoryControlAreaPenUpHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate200CategoryControlAreaPenUpHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_general_pen_dynamic_list_menu_hdlr(point, MMI_PEN_EVENT_UP);

    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  Cate200CategoryControlAreaPenMoveHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate200CategoryControlAreaPenMoveHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_general_pen_dynamic_list_menu_hdlr(point, MMI_PEN_EVENT_MOVE);
    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  Cate200CategoryControlAreaPenLongTapHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate200CategoryControlAreaPenLongTapHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_general_pen_dynamic_list_menu_hdlr(point, MMI_PEN_EVENT_LONG_TAP);
    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  Cate200CategoryControlAreaPenRepeatHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate200CategoryControlAreaPenRepeatHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_general_pen_dynamic_list_menu_hdlr(point, MMI_PEN_EVENT_REPEAT);
    return ret;
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  RedrawCategoryControlledArea200Screen
 * DESCRIPTION
 *  redraw category200
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategoryControlledArea200Screen(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t ccblack = {108, 215, 2, 100};
    color_t c;
    S32 temp_w = 0, temp_h = 0, image_y = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    c.r = 255;
    c.g = 255;
    c.b = 255;
    c.alpha = 100;

    gdi_layer_lock_frame_buffer();

    gui_reset_clip();
    gdi_layer_reset_clip();
//guojian added for bug 8789 B
#if defined(__MMI_TOUCH_SCREEN__)
    resize_fixed_list(
        MMI_content_width,
        (UI_device_height - MMI_button_bar_height - (MMI_title_y + MMI_title_height) - MMI_SINGLELINE_INPUTBOX_HEIGHT - INFORMATION_BAR_HEIGHT - 2  -1 - MMI_virtual_keyboard.height - 1));
    input_box_y = ((MMI_title_y + MMI_title_height) + INFORMATION_BAR_HEIGHT + MMI_fixed_list_menu.height + 1 );

    move_singleline_inputbox(input_box_x, input_box_y);
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
//guojian added for bug 8789 E

    wgui_redraw_singleline_input_information_bar();

    show_dynamic_list();

    gui_reset_clip();
#if defined(__MMI_TOUCH_SCREEN__)
    //input_box_y = ((MMI_title_y + MMI_title_height) + INFORMATION_BAR_HEIGHT + MMI_fixed_list_menu.height + 1);

    gui_fill_rectangle(0, (input_box_y- 1), UI_device_width, (input_box_y + MMI_SINGLELINE_INPUTBOX_HEIGHT+ 4), c);
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gui_draw_rectangle(
        2,
        (input_box_y - 1),
        (UI_device_width - 2),
        (input_box_y + MMI_SINGLELINE_INPUTBOX_HEIGHT+1),
        ccblack);
    gdi_layer_set_clip(4, input_box_y, (input_box_x ), (input_box_y + MMI_SINGLELINE_INPUTBOX_HEIGHT));

#if !defined(MEM_ULC_3216)
    gui_measure_image(get_image(cat200_search_img), &temp_w, &temp_h);
    image_y = ((MMI_SINGLELINE_INPUTBOX_HEIGHT -temp_h) >> 1);
    gui_show_image(4, (input_box_y + image_y), get_image(cat200_search_img));
#endif

#if defined(__MMI_TOUCH_SCREEN__)
  StopMyTimer(BLINKING_CURSOR);//2008-7-4 fengzb fix bug 8791
    mmi_pen_editor_clear_and_show_virtual_keyboard_area();
    redraw_singleline_inputbox();
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

    //In Category 200, redraw_category_screen() is being called here instead of ShowCategory, 
    //since RedrawCategory200Screen is doing many other things like drawing image etc.
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory200Screen
 * DESCRIPTION
 *  exit function of category200
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory200Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    set_MMI_current_input_type();
    reset_multitaps();
    reset_dynamic_list();
    reset_pop_up_descriptions();
    wgui_close_singleline_inputbox();
    cat200_search_function = NULL;
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    reset_all_force_flags_for_hints();
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  GetCategory200HistorySize
 * DESCRIPTION
 *  Gets the history buffer size of category 200
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8s of the history buffer
 *****************************************************************************/
S32 GetCategory200HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (((sizeof(list_menu_category_history) + 3) / 4) * 4 + sizeof(singleline_inputbox_category_history));
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory200History
 * DESCRIPTION
 *  Gets the history buffer of category200
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  return history buffer
 *****************************************************************************/
U8 *GetCategory200History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 s;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history(MMI_CATEGORY200_ID, history_buffer);
    s = sizeof(list_menu_category_history);
    s = (s + 3) / 4;
    s *= 4;
    get_singleline_inputbox_category_history(MMI_CATEGORY200_ID, (U8*) (history_buffer + s), MMI_current_input_type);
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  cat200_virtual_keypad_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat200_virtual_keypad_callback(void)
{
#if defined(__MMI_TOUCH_SCREEN__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 fh = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*
    S32 search_box_height = MMI_SINGLELINE_INPUTBOX_HEIGHT;
    resize_fixed_list(
        MMI_content_width,
        (UI_device_height - MMI_button_bar_height - (MMI_title_y + MMI_title_height) - search_box_height - INFORMATION_BAR_HEIGHT - 2 - fh - 1 - MMI_virtual_keyboard.height - 1));
    */
     
    gui_dynamic_list_menu_locate_highlighted_item(&MMI_fixed_list_menu);
    input_box_y = ((MMI_title_y + MMI_title_height) + INFORMATION_BAR_HEIGHT + MMI_fixed_list_menu.height + 1 + fh);
    move_singleline_inputbox(input_box_x, input_box_y);

    RedrawCategoryControlledArea200Screen(NULL);
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
}

 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory200Screen
 * DESCRIPTION
 *  category screen of phone search
 * PARAMETERS
 *  title                   [IN]        Title id
 *  title_icon              [IN]        Title icon
 *  left_softkey            [IN]        Left soft key label
 *  left_softkey_icon       [IN]        Left soft key icon
 *  right_softkey           [IN]        Right soft key label
 *  right_softkey_icon      [IN]        Right soft key icon
 *  number_of_items         [IN]        Number of items
 *  get_item_func           [IN]        
 *  get_hint_func           [IN]        
 *  highlighted_item        [IN]        Highlighted item index
 *  search_image            [IN]        Seach image
 *  buffer                  [IN]        Buffer
 *  buffer_max_length       [IN]        
 *  history_buffer          [IN]        Histpry buffer
 *  top_msg(?)              [IN]        Message display below title bar
 *  list_of_items(?)        [IN]        List of items
 
 *  list_of_icons1(?)       [IN]        List of icons
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory200Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        GetItemFuncPtr get_item_func,
        GetHintFuncPtr get_hint_func,
        S32 highlighted_item,
        U16 search_image,
        U8 *buffer,
        S32 buffer_max_length,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 temp_w = 0, temp_h = 0;
    S32 fh = 0;
    S32 input_box_width = 0, search_box_height = MMI_SINGLELINE_INPUTBOX_HEIGHT;
    S32 shortcut_width;
    U8 h_flag;
    U8 *img = NULL;
    BOOL application_present;
    S32 max_entries = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    create_fixed_icontext_menuitems();

    MMI_current_menu_type = LIST_MENU;
    associate_fixed_icontext_list();
    init_dynamic_item_buffer(number_of_items, get_item_func, get_hint_func, UI_dummy_function);
    MMI_fixed_list_menu.n_items = number_of_items;
    if (wgui_check_application_present)
    {
        application_present = wgui_check_application_present(&max_entries);     /* true in case of dictionary */
    }
    else
    {
        application_present = MMI_FALSE;
    }

    if (application_present)
    {
        shortcut_width = set_menu_item_count(max_entries) + 7;  /* number of words goes beyond this limit, and increases at the run time */
    }
    else
    {
        shortcut_width = set_menu_item_count(number_of_items) + 7;
    }

    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    move_menu_shortcut_handler(UI_device_width - shortcut_width, MMI_title_y);
    register_hide_menu_shortcut(gui_redraw_menu_shortcut);

    register_dynamic_list_keys();

#ifdef __MMI_TOUCH_SCREEN__
    /* Because the list menu is resized according to virtual keyboard */
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_ALIGN_TO_TOP;
#endif /* __MMI_TOUCH_SCREEN__ */ 

#ifdef __MMI_UI_LIST_CACHE_BACKGROUND__
    /* Virtual keyboard height might be changed. We cannot put menu background in layer */
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER;
#endif /* __MMI_UI_LIST_CACHE_BACKGROUND__ */ 

    resize_fixed_list(
        MMI_content_width,
        (UI_device_height - MMI_button_bar_height - (MMI_title_y + MMI_title_height) - search_box_height - INFORMATION_BAR_HEIGHT - 2 - fh - 1));

    input_box_y = ((MMI_title_y + MMI_title_height) + INFORMATION_BAR_HEIGHT + MMI_fixed_list_menu.height + 1 + fh);

    move_fixed_list(0, ((MMI_title_y + MMI_title_height) + fh + INFORMATION_BAR_HEIGHT));
    register_fixed_list_highlight_handler(dynamic_list_highlight_handler);
	#if defined(MEM_ULC_3216)
    input_box_width = (UI_device_width - 5 - 4);
    input_box_x = 5;
	#else
    img = (U8*) GetImage(search_image);
    gui_measure_image(img, &temp_w, &temp_h);
    input_box_width = (UI_device_width - (5 + temp_w + 1) - 4);
    input_box_x = 5 + temp_w + 1;
	#endif


    h_flag = set_list_menu_category_history(MMI_CATEGORY200_ID, history_buffer);
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    if (get_hint_func)
    {
        if (is_set_force_icon_on_highlight_only_in_menuitem())
        {
            wgui_show_icon_only_highlight_in_icontext_menuitem();
        }
        if (is_set_force_all_hints_in_menuitem())
        {
            wgui_enable_hints_in_icontext_menuitem();
        }
        else if (is_set_force_hints_on_highlight_in_menuitem())
        {
            wgui_enable_hint_highlight_in_icontext_menuitem();
        }
        else
        {
            wgui_enable_hint_highlight_in_icontext_menuitem();
        }

        if (h_flag)
        {
            set_dynamic_pop_up_descriptions(MMI_fixed_list_menu.highlighted_item);
        }
        else
        {
            set_dynamic_pop_up_descriptions(highlighted_item);
        }

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        gui_block_list_effect();
    #endif 
    }
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

    if (h_flag)
    {
        S32 s = sizeof(list_menu_category_history);

        s = (s + 3) / 4;
        s *= 4;
        dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
        wgui_setup_singleline_inputbox(
            input_box_x,
            (input_box_y),
            input_box_width,
            search_box_height,
            buffer,
            buffer_max_length,
            MMI_CATEGORY200_ID,
            get_string(right_softkey),
            get_image(right_softkey_icon),
            INPUT_TYPE_MMI_MULTITAP_BPMF,
            (U8*) (history_buffer + s),
            1);
        if (cat200_search_function)
        {
            if (!application_present)
            {
                S32 no_entries;

                no_entries = cat200_search_function((U8*) MMI_singleline_inputbox.text);
                load_dynamic_item_buffer(no_entries);
                refresh_search_list_change_list_ext(no_entries, MMI_fixed_list_menu.highlighted_item);

            }

        }
    }
    else
    {
        dynamic_list_goto_item_no_redraw(highlighted_item);
        wgui_setup_singleline_inputbox(
            input_box_x,
            (input_box_y),
            input_box_width,
            search_box_height,
            buffer,
            buffer_max_length,
            MMI_CATEGORY200_ID,
            get_string(right_softkey),
            get_image(right_softkey_icon),
            INPUT_TYPE_MMI_MULTITAP_BPMF,
            NULL,
            1);
    }
#if defined(__MMI_TOUCH_SCREEN__)
    resize_fixed_list(
        MMI_content_width,
        (UI_device_height - MMI_button_bar_height - (MMI_title_y + MMI_title_height) - search_box_height - INFORMATION_BAR_HEIGHT - 2 - fh - 1 - MMI_virtual_keyboard.height - 1));
    input_box_y = ((MMI_title_y + MMI_title_height) + INFORMATION_BAR_HEIGHT + MMI_fixed_list_menu.height + 1 + fh);

    move_singleline_inputbox(input_box_x, input_box_y);
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gui_set_single_line_input_box_theme(&MMI_singleline_inputbox, &cat200_singleline_inputbox_theme);
    cat200_search_img = search_image;

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    resize_dynamic_icontext_menuitems_to_list_width();
    set_fixed_icontext_positions(MMI_ICONTEXT_MENUITEM_HEIGHT + 2, 0, 1, 0);//111106 dynamic icontext
    if (h_flag)
    {
        set_dynamic_pop_up_descriptions(MMI_fixed_list_menu.highlighted_item);

        if (application_present)
        {
            if (MMI_fixed_list_menu.highlighted_item <= 0)
            {
                ClearKeyHandler(KEY_UP_ARROW, KEY_EVENT_DOWN);
                ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_DOWN);
            }
        }

    }
    else
    {
        set_dynamic_pop_up_descriptions(highlighted_item);
    }

    singleline_inputbox_input_callback();
    wgui_singleline_inputbox_LSK_label_icon = get_image(left_softkey_icon);
    wgui_singleline_inputbox_LSK_label_string = get_string(left_softkey);
    if (!h_flag)
    {
        if (number_of_items > 0)
        {
            set_left_softkey_label(wgui_singleline_inputbox_LSK_label_string);
            set_left_softkey_icon(wgui_singleline_inputbox_LSK_label_icon);
            redraw_left_softkey();
            register_left_softkey_handler();
            set_left_softkey_function(wgui_singleline_inputbox_LSK_function, KEY_EVENT_UP);
            set_left_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
        }
        else
        {
            set_left_softkey_label(NULL);
            set_left_softkey_icon(NULL);
            redraw_left_softkey();
            set_left_softkey_function(UI_dummy_function, KEY_EVENT_UP);
            set_left_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
        }
    }

    gdi_layer_unlock_frame_buffer();
    ExitCategoryFunction = ExitCategory200Screen;
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory200History, GetCategory200HistorySize);    
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_data.S32flags |= DM_SHOW_VKPAD;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY200_ID;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(RedrawCategoryControlledArea200Screen);
    dm_register_vkpad_callback(cat200_virtual_keypad_callback);
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_category_screen_control_area_pen_handlers(
        Cate200CategoryControlAreaPenDownHandler,
        MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(Cate200CategoryControlAreaPenUpHandler, MMI_PEN_EVENT_UP);
    wgui_register_category_screen_control_area_pen_handlers(
        Cate200CategoryControlAreaPenMoveHandler,
        MMI_PEN_EVENT_MOVE);
    wgui_register_category_screen_control_area_pen_handlers(
        Cate200CategoryControlAreaPenLongTapHandler,
        MMI_PEN_EVENT_LONG_TAP);
    wgui_register_category_screen_control_area_pen_handlers(
        Cate200CategoryControlAreaPenRepeatHandler,
        MMI_PEN_EVENT_REPEAT);
#endif /* __MMI_TOUCH_SCREEN__ */ 
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  DrawCatEMSCategoryControlledArea
 * DESCRIPTION
 *  
 * PARAMETERS
 *  coordinates     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCatEMSCategoryControlledArea(dm_coordinates *coordinates)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_singleline_inputbox();
}   /* end of DrawCatEMSCategoryControlledArea */

/***************************************************************************
Function Name: EMS_matrix_highlight_handler
***************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  EMS_matrix_highlight_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_matrix_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    extern color_t MMI_white_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);

    MMI_singleline_inputbox.text = (U8*) MMI_highlighted_item_text;
 
    //It is necessary to set the last_position_p also when the text pointer is changed.
    MMI_singleline_inputbox.last_position_p =
        MMI_singleline_inputbox.text + (gui_strlen((UI_string_type) MMI_singleline_inputbox.text) << 1);
 
    gui_fill_rectangle(
        MMI_singleline_inputbox.x,
        MMI_singleline_inputbox.y,
        MMI_singleline_inputbox.x + MMI_singleline_inputbox.width,
        MMI_singleline_inputbox.y + MMI_singleline_inputbox.height,
        MMI_white_color);
    redraw_singleline_inputbox();
    MMI_matrix_highlight_handler(item_index);

}

/***************************************************************************
Function Name: ShowCategoryEMSScreen
***************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ShowCategoryEMSScreen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  num_of_symbols          [IN]        
 *  symbols                 [?]         
 *  descriptions            [?]         
 *  highlighted_item        [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategoryEMSScreen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 num_of_symbols,
        UI_image_ID_type  *symbols,
        UI_image_ID_type  *descriptions,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i, symbol_width = 0, symbol_height = 0, width = 0, height = 0;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    MMI_title_string = get_string(title);

    create_fixed_icontext_menuitems();
    associate_fixed_icontext_matrix();
    for (i = 0; i < num_of_symbols; i++)
    {
        PU8 img = (PU8) get_image(symbols[i]);

        add_fixed_icontext_item((UI_string_type) get_string(descriptions[i]), img);
        gui_measure_image(img, &symbol_width, &symbol_height);
        if (symbol_width > width)
        {
            width = symbol_width;
        }
        if (symbol_height > height)
        {
            height = symbol_width;
        }
    }
    configure_fixed_matrix(width, height, 0, 0);
    MMI_fixed_matrix_menu.height =
        MMI_fixed_matrix_menu.n_rows * (MMI_fixed_matrix_menu.width / MMI_fixed_matrix_menu.n_columns);
    MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_FOCUSSED_ANIMATE | UI_MENUITEM_DISABLE_TEXT_DISPLAY;
    register_fixed_matrix_loop_keys();

    register_fixed_matrix_highlight_handler(EMS_matrix_highlight_handler);
    MMI_current_menu_type = MATRIX_MENU_EMS;
    MMI_fixed_matrix_menu.flags |= UI_MATRIX_MENU_LOOP;
    h_flag = set_matrix_menu_category_history(MMI_CATEGORY22_ID, history_buffer);
    if (h_flag)
    {
        MMI_highlighted_item_text = get_item_text(MMI_fixed_matrix_menu.highlighted_item);
        create_singleline_inputbox_set_buffer(
            (UI_string_type) MMI_highlighted_item_text,
            gui_strlen((UI_string_type) MMI_highlighted_item_text) + 1,
            gui_strlen((UI_string_type) MMI_highlighted_item_text),
            gui_strlen((UI_string_type) MMI_highlighted_item_text));
        set_singleline_inputbox_mask(0);
        MMI_singleline_inputbox.flags |=
            (UI_SINGLE_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW |
             UI_SINGLE_LINE_INPUT_BOX_CENTER_JUSTIFY);
        fixed_matrix_goto_item_no_redraw(MMI_fixed_matrix_menu.highlighted_item);
    }
    else
    {
        MMI_highlighted_item_text = get_item_text(highlighted_item);
        create_singleline_inputbox_set_buffer(
            (UI_string_type) MMI_highlighted_item_text,
            gui_strlen((UI_string_type) MMI_highlighted_item_text) + 1,
            gui_strlen((UI_string_type) MMI_highlighted_item_text),
            gui_strlen((UI_string_type) MMI_highlighted_item_text));
        set_singleline_inputbox_mask(0);
        MMI_singleline_inputbox.flags |=
            (UI_SINGLE_LINE_INPUT_BOX_DISABLE_BACKGROUND | UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW |
             UI_SINGLE_LINE_INPUT_BOX_CENTER_JUSTIFY);
        fixed_matrix_goto_item_no_redraw(highlighted_item);
    }

    MMI_disable_title_shortcut_display = 1;

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory22Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY_EMS_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawCatEMSCategoryControlledArea);
    dm_redraw_category_screen();

}   /* end of ShowCategoryEMSScreen */


/*****************************************************************************
 * FUNCTION
 *  GetCategory400History
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *GetCategory400History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return history_buffer;
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory400HistorySize
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 GetCategory400HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return 0;
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory400Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory400Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t cb = gui_color(0, 0, 0);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_push_clip();
    gdi_layer_set_clip(0, 0, UI_device_width - 1, UI_device_height - 1);
    clear_screen();
    show_title_status_icon();
    draw_title();
    gui_draw_rectangle(
        MMI_content_x,
        MMI_content_y,
        MMI_content_x + MMI_content_width - 1,
        MMI_content_y + MMI_content_height - 1,
        cb);
    show_softkey_background();
    show_left_softkey();
    show_right_softkey();
    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory400Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory400Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_disable_title_shortcut_display = 0;
    disable_menu_shortcut_box_display = 0;
    reset_softkeys();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory400Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory400Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_disable_title_shortcut_display = 1;
    disable_menu_shortcut_box_display = 1;
    gdi_layer_lock_frame_buffer();
    clear_category_screen_key_handlers();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory400Screen;
    RedrawCategoryFunction = RedrawCategory400Screen;
    GetCategoryHistory = GetCategory400History;
    GetCategoryHistorySize = GetCategory400HistorySize;
    RedrawCategory400Screen();
}

#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__)
    #if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
    #else /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__) */ 
    #endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__) */ 
    #if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
    #else /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__) */ 
    #endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__) */ 
    #if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
    #else /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__) */ 
    #endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__) */ 
    #if defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__)
    #if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
    #else /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__) */ 
    #endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) || defined(__MMI_UI_DALMATIAN_MAINMENU__) || defined(__MMI_UI_DALMATIAN_FIXEDLIST__) || defined(__MMI_UI_DALMATIAN_EDITOR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__) */ 
    #else /* defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__) */ 
    #endif /* defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__) */ 
#endif /* (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__) */ 

#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
#endif /* (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory354Screen
 * DESCRIPTION
 *  Displays the category354 screen ( List menu with two text rows )
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items in the menu
 *  list_of_items1          [IN]        String array1 of items
 *  list_of_items2          [IN]        String array2 of items
 *  list_of_icons           [IN]        Array of icons
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory354Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U8 **list_of_items1,
        U8 **list_of_items2,
        U16 *list_of_icons,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    S32 img_width, img_height;
    S32 str_width, str_height;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    create_fixed_icontext_list_menuitems(2, 1);
    associate_fixed_icontext_list_list();

    ShowListCategoryScreen(
        (UI_string_type) title,
        get_image(title_icon),
        get_string(left_softkey),
        get_image(left_softkey_icon),
        get_string(right_softkey),
        get_image(right_softkey_icon),
        number_of_items);

    /* we need two lines */
    resize_fixed_icontext_list_menuitems(0, get_menu_item_height() << 1);

    gdi_image_get_dimension_id(list_of_icons[0], &img_width, &img_height);
    gui_measure_string((UI_string_type) list_of_items1[0], &str_width, &str_height);
    set_fixed_icontext_list_icon_coordinates(0, 1, 1, img_width, img_height);
    set_fixed_icontext_list_text_coordinates(
        0,
        20,
        (get_menu_item_height() - str_height) >> 1,
        MMI_fixed_icontext_list_menuitem.width - 20,
        get_menu_item_height());
    set_fixed_icontext_list_text_coordinates(
        1,
        20,
        get_menu_item_height() + ((get_menu_item_height() - str_height) >> 1),
        MMI_fixed_icontext_list_menuitem.width - 20,
        get_menu_item_height());

    for (i = 0; i < number_of_items; i++)
    {
        fixed_icontext_list_item_insert(i);
        add_fixed_icontext_list_item_icon(i, 0, wgui_get_list_menu_icon(i, list_of_icons[i]));
        add_fixed_icontext_list_item_text(i, 0, (UI_string_type) list_of_items1[i]);
        add_fixed_icontext_list_item_text(i, 1, (UI_string_type) list_of_items2[i]);
        wgui_pop_up_description_strings[i].text_strings[0] = NULL;
    }

    MMI_fixed_icontext_list_menuitem.flags &= ~UI_MENUITEM_CENTER_TEXT_Y;
    MMI_fixed_icontext_list_menuitem.flags &= ~UI_MENUITEM_CENTER_ICON_Y;
    MMI_fixed_icontext_list_menuitem.flags |= UI_MENUITEM_MARQUEE_SCROLL;
    MMI_fixed_icontext_list_menuitem.flags |= UI_MENUITEM_AUTO_CHOOSE_MARQUEE;

    h_flag = set_list_menu_category_history(MMI_CATEGORY52_ID, history_buffer);
    if (h_flag)
    {
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    MMI_title_string = (UI_string_type) title;
    MMI_title_icon = (PU8) get_image(title_icon);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory354Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32flags = 0;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY52_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory354Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory354Screen
 * DESCRIPTION
 *  Exits the category354 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory354Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    MMI_fixed_icontext_list_menuitem.flags |= UI_MENUITEM_CENTER_TEXT_Y;
    MMI_fixed_icontext_list_menuitem.flags |= UI_MENUITEM_CENTER_ICON_Y;
    MMI_fixed_icontext_list_menuitem.flags &= ~UI_MENUITEM_MARQUEE_SCROLL;
    MMI_fixed_icontext_list_menuitem.flags &= ~UI_MENUITEM_AUTO_CHOOSE_MARQUEE;
}

#ifdef __MMI_EDITABLE_AUDIO_EQUALIZER__


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory334Screen
 * DESCRIPTION
 *  Redraws the category334 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory334Screen()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 aud_eq_bg_img = NULL;
    PU8 aud_eq_active_bar_img = NULL;
    PU8 aud_eq_sound_level_ind_img = NULL;
    S32 index;
    S32 width = 0, height = 0, width_active = 0, height_active = 0;
    long start_x_slider, middle_point_y_of_bar;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    clear_screen();
    show_title_status_icon();
    draw_title();
    /* get the images required */
    aud_eq_bg_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_BG);
    aud_eq_active_bar_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_ACTIVE_BAR);
    aud_eq_sound_level_ind_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_VAL_INDICATOR);

    /* calcualte how much one pixel represents in a bar */
    temp_float_var = (float)256 / (float)g_audio_equalizer_scrollable_ht;
    one_pixel_represents = 10 * temp_float_var;

    gdi_layer_push_clip();
    /* show the background image */
    if (!wgui_category_screen_no_buttons)
    {
        gdi_layer_set_clip(
            0,
            (MMI_title_y + MMI_title_height),
            UI_device_width - 1,
            (UI_device_height - MMI_button_bar_height));
        gui_show_transparent_image(0, (MMI_title_y + MMI_title_height), aud_eq_bg_img, gui_transparent_color(0, 0, 0));
        show_softkey_background();
        show_left_softkey();
        show_right_softkey();
    }
    else
    {
        gdi_layer_set_clip(0, (MMI_title_y + MMI_title_height), UI_device_width - 1, (UI_device_height - 1));
        gui_show_transparent_image(0, (MMI_title_y + MMI_title_height), aud_eq_bg_img, gui_transparent_color(0, 0, 0));
    }

    /* measure the slider image. */
    gui_measure_image(aud_eq_sound_level_ind_img, &width, &height);

    /* show the active bar...the x coordinate is based on the difference in width of the active and inactive bar. */
    gui_measure_image(aud_eq_active_bar_img, &width_active, &height_active);
    gui_show_image(
        g_audio_equalizer_bar_start_x - (width_active - g_audio_equalizer_bar_width) / 2 + g_audio_equalizer_current_setting_info.active_bar * (g_audio_equalizer_bar_width + g_audio_equalizer_dist_btw_bars),
        (MMI_title_y + MMI_title_height + g_audio_equalizer_bar_start_y),
        aud_eq_active_bar_img);

    //show the indicator for sound levels.
    //calculate the middle point of a bar
    middle_point_y_of_bar =
        MMI_title_y + MMI_title_height + g_audio_equalizer_bar_start_y + (g_audio_equalizer_bar_height - 1) / 2;
    start_x_slider = g_audio_equalizer_bar_start_x - (width - g_audio_equalizer_bar_width) / 2;
    for (index = 0; index < NUM_OF_BARS_IN_A_SETTING; index++)
    {
        long y_coord =
            middle_point_y_of_bar -
            (long)((float)g_audio_equalizer_current_setting_info.sound_level[index] / one_pixel_represents) -
            height / 2;
        if (g_audio_equalizer_current_setting_info.sound_level[index] == 0 ||
            g_audio_equalizer_current_setting_info.sound_level[index] > 0)
        {
            y_coord++;
        }
        gui_show_image(start_x_slider, y_coord, aud_eq_sound_level_ind_img);
        start_x_slider += (g_audio_equalizer_bar_width + g_audio_equalizer_dist_btw_bars);

    }

    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory334Screen
 * DESCRIPTION
 *  Exitss the category334 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory334Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

#ifdef __MMI_DRAW_MANAGER__


/*****************************************************************************
 * FUNCTION
 *  dm_category_334_controlled_area
 * DESCRIPTION
 *  Redraws the category334 screen using Draw Manager
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void dm_category_334_controlled_area(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory334Screen();
}

#endif /* __MMI_DRAW_MANAGER__ */ 

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  category334_get_bar_index
 * DESCRIPTION
 *  Gets the bar index of the bar which contains the input point.
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  S8 index of the bar
 *****************************************************************************/
S8 category334_get_bar_index(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 i, j;
    S16 dx;
    S32 width, height;
    PU8 aud_eq_sound_level_ind_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_VAL_INDICATOR);
    mmi_pen_polygon_area_struct bar_area;
    mmi_pen_point_struct bar_vertex[4];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_measure_image(aud_eq_sound_level_ind_img, &width, &height);
    /* the limits of the width of a bar are taken as the width of the slider. slider width is assumed to be the maximum, bcos it should be. */
    if (width < g_audio_equalizer_bar_width)
    {
        width = g_audio_equalizer_bar_width;
    }

    /* set bar area */
    bar_area.num = 4;
    bar_area.points = (mmi_pen_point_struct*) & bar_vertex;
    bar_vertex[0].x = (S16) g_audio_equalizer_bar_start_x - (width - g_audio_equalizer_bar_width) / 2;
    bar_vertex[0].y = (S16) MMI_title_y + MMI_title_height + g_audio_equalizer_bar_start_y;
    bar_vertex[1].x = bar_vertex[0].x + width - 1;
    bar_vertex[1].y = bar_vertex[0].y;
    bar_vertex[2].x = bar_vertex[1].x;
    bar_vertex[2].y = bar_vertex[0].y + g_audio_equalizer_bar_height - 1;
    bar_vertex[3].x = bar_vertex[0].x;
    bar_vertex[3].y = bar_vertex[2].y;
    dx = (S16) g_audio_equalizer_bar_width + g_audio_equalizer_dist_btw_bars;

    for (i = 0; i < NUM_OF_BARS_IN_A_SETTING; i++)
    {
        if (mmi_pen_check_inside_polygon(&bar_area, point))
        {
            return i;
        }
        for (j = 0; j < 4; j++)
        {
            bar_vertex[j].x += dx;
        }
    }
    return (S8) - 1;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_audio_equalizer_activate_bar
 * DESCRIPTION
 *  Activates the bar having index same as the input argument.
 * PARAMETERS
 *  bar_index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_audio_equalizer_activate_bar(U8 bar_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    long start_x_img, start_y_img, middle_pt_y_in_bar;
    S16 dx;
    S32 bar_width, width, height, width_active, height_active;
    PU8 aud_eq_active_bar_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_ACTIVE_BAR);
    PU8 aud_eq_sound_level_ind_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_VAL_INDICATOR);
    PU8 aud_eq_bg_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_BG);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_push_clip();

    /* miscellaneous calculations */
    gui_measure_image(aud_eq_sound_level_ind_img, &width, &height);
    bar_width = g_audio_equalizer_bar_width;
    if (width > g_audio_equalizer_bar_width)
    {
        bar_width = width;
    }
    dx = (S16) g_audio_equalizer_bar_width + g_audio_equalizer_dist_btw_bars;

    /* inactivate the already active bar. */
    start_x_img =
        g_audio_equalizer_bar_start_x - (bar_width - g_audio_equalizer_bar_width) / 2 +
        dx * g_audio_equalizer_current_setting_info.active_bar;
    start_y_img = MMI_title_y + MMI_title_height + g_audio_equalizer_bar_start_y;
    middle_pt_y_in_bar = start_y_img + (g_audio_equalizer_bar_height - 1) / 2;

    gdi_layer_set_clip(
        start_x_img,
        start_y_img,
        start_x_img + bar_width - 1,
        start_y_img + g_audio_equalizer_bar_height - 1);

    /* 1. show the inactive bar in place of active bar... */
    gui_show_transparent_image(0, MMI_title_y + MMI_title_height, aud_eq_bg_img, gui_transparent_color(0, 0, 0));

    /* 2. show the slider control */
    start_x_img =
        g_audio_equalizer_bar_start_x - (width - g_audio_equalizer_bar_width) / 2 +
        dx * g_audio_equalizer_current_setting_info.active_bar;
    start_y_img =
        middle_pt_y_in_bar -
        (long)((float)g_audio_equalizer_current_setting_info.
               sound_level[g_audio_equalizer_current_setting_info.active_bar] / one_pixel_represents + height / 2);
    if (g_audio_equalizer_current_setting_info.sound_level[g_audio_equalizer_current_setting_info.active_bar] >= 0)
    {
        start_y_img++;
    }
    gui_show_image(start_x_img, start_y_img, aud_eq_sound_level_ind_img);

    /* activate the new bar */
    g_audio_equalizer_current_setting_info.active_bar = bar_index;

    start_x_img =
        g_audio_equalizer_bar_start_x - (bar_width - g_audio_equalizer_bar_width) / 2 +
        dx * g_audio_equalizer_current_setting_info.active_bar;
    start_y_img = MMI_title_y + MMI_title_height + g_audio_equalizer_bar_start_y;
    gdi_layer_set_clip(
        start_x_img,
        start_y_img,
        start_x_img + bar_width - 1,
        start_y_img + g_audio_equalizer_bar_height - 1);

    /* Step 1.paste the inactive bar image.... */
    gui_show_transparent_image(0, MMI_title_y + MMI_title_height, aud_eq_bg_img, gui_transparent_color(0, 0, 0));
    /* Step 2.paste the active bar image */
    gui_measure_image(aud_eq_active_bar_img, &width_active, &height_active);
    start_x_img =
        g_audio_equalizer_bar_start_x - (width_active - g_audio_equalizer_bar_width) / 2 +
        dx * g_audio_equalizer_current_setting_info.active_bar;
    gui_show_image(start_x_img, start_y_img, aud_eq_active_bar_img);

    /* Step 3.paste the slider */
    start_x_img =
        g_audio_equalizer_bar_start_x - (width - g_audio_equalizer_bar_width) / 2 +
        dx * g_audio_equalizer_current_setting_info.active_bar;
    start_y_img =
        middle_pt_y_in_bar -
        (long)((float)g_audio_equalizer_current_setting_info.
               sound_level[g_audio_equalizer_current_setting_info.active_bar] / one_pixel_represents + height / 2);
    if (g_audio_equalizer_current_setting_info.sound_level[g_audio_equalizer_current_setting_info.active_bar] >= 0)
    {
        start_y_img++;
    }
    gui_show_image(start_x_img, start_y_img, aud_eq_sound_level_ind_img);

    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_audio_equalizer_change_sound_level
 * DESCRIPTION
 *  change the sound level on the screen using the sound level value.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_audio_equalizer_change_sound_level(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 aud_eq_active_bar_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_ACTIVE_BAR);
    PU8 aud_eq_sound_level_ind_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_VAL_INDICATOR);
    PU8 aud_eq_bg_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_BG);
    long start_x_img, start_y_img, middle_pt_y_in_bar;
    S32 bar_width, width, height, width_active, height_active;
    S16 dx = (S16) (g_audio_equalizer_bar_width + g_audio_equalizer_dist_btw_bars);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_push_clip();

    /* miscellaneous calculations */
    gui_measure_image(aud_eq_sound_level_ind_img, &width, &height);
    bar_width = g_audio_equalizer_bar_width;
    if (width > g_audio_equalizer_bar_width)
    {
        bar_width = width;
    }

    /* firt paste the inactive image on the active bar. */
    start_x_img =
        g_audio_equalizer_bar_start_x - (bar_width - g_audio_equalizer_bar_width) / 2 +
        dx * g_audio_equalizer_current_setting_info.active_bar;
    start_y_img = MMI_title_y + MMI_title_height + g_audio_equalizer_bar_start_y;
    middle_pt_y_in_bar = start_y_img + (g_audio_equalizer_bar_height - 1) / 2;
    gdi_layer_set_clip(
        start_x_img,
        start_y_img,
        start_x_img + bar_width - 1,
        start_y_img + g_audio_equalizer_bar_height - 1);
    gui_show_transparent_image(0, MMI_title_y + MMI_title_height, aud_eq_bg_img, gui_transparent_color(0, 0, 0));

    /* paste the active bar image on the already active bar */
    gui_measure_image(aud_eq_active_bar_img, &width_active, &height_active);
    start_x_img =
        g_audio_equalizer_bar_start_x - (width_active - g_audio_equalizer_bar_width) / 2 +
        dx * g_audio_equalizer_current_setting_info.active_bar;
    gui_show_image(start_x_img, start_y_img, aud_eq_active_bar_img);

    /* paste the slider */
    start_x_img =
        g_audio_equalizer_bar_start_x - (width - g_audio_equalizer_bar_width) / 2 +
        dx * g_audio_equalizer_current_setting_info.active_bar;
    start_y_img =
        middle_pt_y_in_bar -
        (long)((float)g_audio_equalizer_current_setting_info.
               sound_level[g_audio_equalizer_current_setting_info.active_bar] / one_pixel_represents + height / 2);
    if (g_audio_equalizer_current_setting_info.sound_level[g_audio_equalizer_current_setting_info.active_bar] >= 0)
    {
        start_y_img++;
    }
    gui_show_image(start_x_img, start_y_img, aud_eq_sound_level_ind_img);
    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  category334_pen_down_handler
 * DESCRIPTION
 *  Handler for pen down event.
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  MMI_BOOL
 *****************************************************************************/
MMI_BOOL category334_pen_down_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 bar_index;
    S16 dx;
    S32 width, height;
    PU8 aud_eq_sound_level_ind_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_VAL_INDICATOR);
    mmi_pen_polygon_area_struct area;
    mmi_pen_point_struct vertex[4];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* measure the slider image. */
    gui_measure_image(aud_eq_sound_level_ind_img, &width, &height);
    if (width < g_audio_equalizer_bar_width)
    {
        width = (S32) g_audio_equalizer_bar_width;
    }

    /* set bar area */
    dx = (S16) g_audio_equalizer_bar_width + g_audio_equalizer_dist_btw_bars;
    area.num = 4;
    area.points = (mmi_pen_point_struct*) & vertex;
    vertex[0].x = (S16) g_audio_equalizer_bar_start_x - (width - g_audio_equalizer_bar_width) / 2;
    vertex[0].y = (S16) MMI_title_y + MMI_title_height + g_audio_equalizer_bar_start_y;
    vertex[1].x = vertex[0].x + dx * (NUM_OF_BARS_IN_A_SETTING - 1) + width - 1;
    vertex[1].y = vertex[0].y;
    vertex[2].x = vertex[1].x;
    vertex[2].y = vertex[0].y + g_audio_equalizer_bar_height - 1;
    vertex[3].x = vertex[0].x;
    vertex[3].y = vertex[2].y;

    /* check if hit in the bar area: */
    if (!mmi_pen_check_inside_polygon(&area, point))
    {
        return FALSE;
    }

    //now since the pen down has been done in the bar area.
    //check whether some frequency bar has been hit or not...if hit then activate it(if not activated earlier). 
    if ((bar_index = category334_get_bar_index(point)) != (S8) - 1)
    {
        S16 middle_pt_y_in_bar, slider_center_y, slider_start_y, slider_end_y, start_x;

        /* activate the bar if not active... */
        if (bar_index != g_audio_equalizer_current_setting_info.active_bar)
        {
            wgui_audio_equalizer_activate_bar(bar_index);
        }

        //change the sound level if the bar has been hit at a place which is different from original sound level of the bar
        //1. check if the pen down been done on the slider itself or some other place on the bar.
        //get the original position of the slider from sound level.
        middle_pt_y_in_bar = vertex[0].y + (g_audio_equalizer_bar_height - 1) / 2;
        slider_center_y =
            middle_pt_y_in_bar -
            (S16) ((float)g_audio_equalizer_current_setting_info.sound_level[bar_index] / one_pixel_represents);

        slider_start_y = slider_center_y - height / 2;
        slider_end_y = slider_center_y + height / 2;
        start_x = vertex[0].x + dx * bar_index;
        if (((point.x >= start_x) && (point.x < start_x + width)) &&
            ((point.y >= slider_start_y) && (point.y <= slider_end_y)))
        {
            return TRUE;    /* we don't need to do anything in this case since pen down is on the slider itself */
        }                   /* if hit on the slider area. */
        else                /* if hit somewhere else on the bar.then the slider is to be shifted to new position. */
        {
            /* find the difference between new and original. */
            S16 difference;

            /* modify point.y if it is between the start y of the bar and the start y + height/2, where height is height of the slider. */
            if (point.y < middle_pt_y_in_bar - (g_audio_equalizer_scrollable_ht) / 2)
            {
                point.y = middle_pt_y_in_bar - (g_audio_equalizer_scrollable_ht) / 2;
            }
            else if (point.y > middle_pt_y_in_bar + (g_audio_equalizer_scrollable_ht) / 2)
            {
                point.y = middle_pt_y_in_bar + (g_audio_equalizer_scrollable_ht) / 2;
            }
            /* find the difference between the place hit and the currenly occupied coordinates of slider. */
            difference = slider_center_y - point.y;

            mmi_settings_audio_equalizer_change_sound_level((S16) ((float)difference * one_pixel_represents));
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  category334_pen_move_handler
 * DESCRIPTION
 *  Handler for pen move event.
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  MMI_BOOL
 *****************************************************************************/
MMI_BOOL category334_pen_move_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 bar_start_x, bar_start_y, middle_pt_y_in_bar, dx;
    S32 width, height;
    PU8 aud_eq_sound_level_ind_img = (PU8) GetImage(SOUND_EFFECT_AUDIO_EQ_VAL_INDICATOR);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* measure the slider image. */
    gui_measure_image(aud_eq_sound_level_ind_img, &width, &height);
    if (width < g_audio_equalizer_bar_width)
    {
        width = (S32) g_audio_equalizer_bar_width;
    }

    dx = (S16) (g_audio_equalizer_bar_width + g_audio_equalizer_dist_btw_bars);
    bar_start_x =
        g_audio_equalizer_bar_start_x - (width - g_audio_equalizer_bar_width) / 2 +
        dx * g_audio_equalizer_current_setting_info.active_bar;
    bar_start_y = MMI_title_y + MMI_title_height + g_audio_equalizer_bar_start_y;
    //check that whether the point is inside the active bar area.
    //the concept being used is that the slider movement is to be done only in case the point is with in bar area
    //and only vertical component is to be used for movement.
    middle_pt_y_in_bar = bar_start_y + (g_audio_equalizer_bar_height - 1) / 2;
    if (point.x >= bar_start_x && point.x < bar_start_x + width &&
        point.y >= (middle_pt_y_in_bar - g_audio_equalizer_scrollable_ht / 2) &&
        point.y <= (middle_pt_y_in_bar + g_audio_equalizer_scrollable_ht / 2))
    {
        /* 1. Find the the middle point of the slider present on the bar. */
        S16 mid_pt_slider =
            middle_pt_y_in_bar -
            (S16) ((float)g_audio_equalizer_current_setting_info.
                   sound_level[g_audio_equalizer_current_setting_info.active_bar] / one_pixel_represents);
        if (mid_pt_slider != point.y)   /* the case when the sound level is to be modified and the slider shifted. */
        {
            mmi_settings_audio_equalizer_change_sound_level((S16)
                                                            ((float)(mid_pt_slider - point.y) * one_pixel_represents));
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory334Screen
 * DESCRIPTION
 *  Show category funciton for equalizer screen
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  values                  [?]         
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory334Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        signed char *values,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_DRAW_MANAGER__
    dm_data_struct dm_data;
#endif 
    S8 index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();
    SetupCategoryKeyHandlers();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_menu_shortcut_number = -1;

    if ((left_softkey == 0) && (left_softkey_icon == 0) && (right_softkey == 0) && (right_softkey_icon == 0))
    {
        wgui_category_screen_no_buttons = 1;
    }
    else
    {
        wgui_category_screen_no_buttons = 0;
    }

    for (index = 0; index < NUM_OF_BARS_IN_A_SETTING; index++)
    {
        g_audio_equalizer_current_setting_info.sound_level[index] = 10 * values[index];
    }

    gdi_layer_unlock_frame_buffer();

#ifndef __MMI_DRAW_MANAGER__
    ExitCategoryFunction = ExitCategory334Screen;
    RedrawCategoryFunction = RedrawCategory334Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCategoryFunction();
#else /* __MMI_DRAW_MANAGER__ */ 
#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_DRAW_MANAGER__)
    wgui_register_category_screen_control_area_pen_handlers(category334_pen_down_handler, MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(category334_pen_move_handler, MMI_PEN_EVENT_MOVE);
#endif /* (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_DRAW_MANAGER__) */ 
    ExitCategoryFunction = ExitCategory334Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCategoryFunction = dm_redraw_category_screen;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY334_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(dm_category_334_controlled_area);
    dm_redraw_category_screen();
#endif /* __MMI_DRAW_MANAGER__ */ 
}

#endif /* __MMI_EDITABLE_AUDIO_EQUALIZER__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory401Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  percentage      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory401Screen(U16 percentage)
{
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-3-10 18:42 */
#if 0
#if (defined(CSD_TARGET) && !defined(__MAUI_BASIC__) && !defined(__L1_STANDALONE__) && !defined(EMPTY_MMI))
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 bar_width = 0, bar_height = 0;
    S32 bar_x = 0, bar_y = 0;
    S32 fill_width = 0;
    S32 str_width = 0, str_height = 0;
    S32 str_x = 0, str_y = 0;
    U8 *bar_bg_image = nfb_progress_images[0];
    U16 digit[3], idx = 0;
    S32 digit_width[3];
    S32 digit_height[3];
    static U8 isInited = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (bmt_is_wdt_power() == KAL_TRUE || BMT.PWRon == RTCPWRON)
    {
        return;
    }

    if (!isInited)
    {
        isInited = 1;
        gdi_init();
        custom_cfg_gpio_set_level(GPIO_DEV_LED_MAINLCD, 5);
        setup_UI_wrappers();
    }

    gdi_layer_lock_frame_buffer();
    gdi_layer_clear(GDI_COLOR_WHITE);
    gdi_layer_push_clip();
    gdi_layer_reset_clip();

    /* percentage images */

    /* decide which images are used */
    if (percentage == 100)
    {
        digit[2] = 1;
        digit[1] = 0;
        digit[0] = 0;
    }
    else if (percentage == 0)
    {
        digit[2] = 0;
        digit[1] = 0;
        digit[0] = 0;
    }
    else if (percentage > 0 && percentage < 100)
    {
        digit[2] = 0;
        digit[1] = percentage / 10;
        digit[0] = percentage - (digit[1] * 10);
    }
    else
    {
        MMI_ASSERT(percentage <= 100);
    }

    /* calculate digit image width and height */
    gdi_image_get_dimension(nfb_progress_images[11], &str_width, &str_height);

    for (idx = 2; idx > 0; idx--)
    {
        if (digit[idx] != 0 || digit[2] == 1)
        {
            gdi_image_get_dimension(nfb_progress_images[digit[idx] + 1], &digit_width[idx], &digit_height[idx]);
            str_width += digit_width[idx];
        }
    }
    gdi_image_get_dimension(nfb_progress_images[digit[0] + 1], &digit_width[0], &digit_height[0]);

    /* decide digit images position */
    str_x = (UI_device_width - str_width) >> 1;
    str_y = (UI_device_height >> 1) + 2;

    /* show the digit images */
    for (idx = 2; idx > 0; idx--)
    {
        if (digit[idx] != 0 || digit[2] == 1)
        {
            gdi_image_draw(str_x, str_y, nfb_progress_images[digit[idx] + 1]);
            str_x += digit_width[idx];
        }
    }
    gdi_image_draw(str_x, str_y, nfb_progress_images[digit[0] + 1]);
    str_x += digit_width[0];
    gdi_image_draw(str_x, str_y, nfb_progress_images[11]);

    /* Progress bar background image */
    gdi_image_get_dimension(bar_bg_image, &bar_width, &bar_height);
    bar_x = (UI_device_width - bar_width - 2 /* gap */ ) >> 1;
    bar_y = (UI_device_height >> 1) - bar_height;

    gdi_layer_set_clip(bar_x, bar_y, bar_x + bar_width - 1, bar_y + bar_height - 1);

    /* Progress bar fill area */
    fill_width = ((bar_width - (MMI_CAT401_X_OFFSET << 1)) * (S32) percentage) / 100;
    gdi_draw_solid_rect(
        bar_x + MMI_CAT401_X_OFFSET,
        bar_y,
        bar_x + MMI_CAT401_X_OFFSET + fill_width - 1,
        bar_y + bar_height - 1,
        GDI_COLOR_RED);

    gdi_image_draw(bar_x, bar_y, bar_bg_image);

    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

#endif /* (defined(CSD_TARGET) && !defined(__MAUI_BASIC__) && !defined(__L1_STANDALONE__) && !defined(EMPTY_MMI)) */ 
#endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-3-10 18:42 */
}


/*****************************************************************************
 * FUNCTION
 *  cat201_register_language_changed_indicator
 * DESCRIPTION
 *  
 * PARAMETERS
 *  FuncPtr     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
BOOL(*wgui_check_language_changed) (U8 input_type) = NULL;

void cat201_register_language_changed_indicator(BOOL(*pfunc) (U8 input_type))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_check_language_changed = pfunc;
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory201Screen
 * DESCRIPTION
 *  exit function of category201
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
/* Begin AP input mode set */
extern U8 MMI_AP_set_input_mode_set;

/* End AP input mode set */

void ExitCategory201Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    set_MMI_current_input_type();
    reset_multitaps();
    reset_dynamic_list();
    reset_pop_up_descriptions();
    wgui_close_inputbox();
    /* Begin AP input mode set */
    if (MMI_AP_set_input_mode_set)
    {
        wgui_unset_AP_required_input_mode_set_flag();
    }
    /* End AP input mode set */
    cat200_search_function = NULL;
    wgui_check_language_changed = NULL;
    wgui_reset_multiline_inputbox_register_up_down_key_funcptr();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory201HistorySize
 * DESCRIPTION
 *  Gets the history buffer size of category 200
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8s of the history buffer
 *****************************************************************************/
S32 GetCategory201HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    return (((sizeof(list_menu_category_history) + 3) / 4) * 4 + sizeof(multiline_inputbox_category_history));
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory201History
 * DESCRIPTION
 *  Gets the history buffer of category200
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  return history buffer
 *****************************************************************************/
U8 *GetCategory201History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
    S32 s;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history(MMI_CATEGORY201_ID, history_buffer);
    s = sizeof(list_menu_category_history);
    s = (s + 3) / 4;
    s *= 4;
    get_multiline_inputbox_category_history(MMI_CATEGORY201_ID, (U8*) (history_buffer + s), MMI_current_input_type);
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory201RightSoftkeyFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  f       [IN]        
 *  k       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory201RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(k);
    wgui_inputbox_RSK_function = f;
}

 


/*****************************************************************************
 * FUNCTION
 *  cat201_virtual_keypad_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat201_virtual_keypad_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__MMI_TOUCH_SCREEN__)
    mmi_pen_editor_resize_multiline_input_box_for_vk();
#endif 
}

 


/*****************************************************************************
 * FUNCTION
 *  refresh_category201_search_list_change_list
 * DESCRIPTION
 *  input call back function of category200 multi tap
 * PARAMETERS
 *  no_entries                  [IN]        
 *  UI_character_type(?)        [IN]        Inp
 * RETURNS
 *  void
 *****************************************************************************/
void refresh_category201_search_list_change_list(S32 no_entries)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 shortcut_width;
    S32 max_entries = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_list_menu.highlighted_item = 0;
    MMI_fixed_list_menu.n_items = no_entries;
    MMI_fixed_list_menu.displayed_items = no_entries;

    if (wgui_check_application_present)
    {
        wgui_check_application_present(&max_entries);
    }

    shortcut_width = set_menu_item_count(max_entries) + 7;  /* number of element increases at the run time */
    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    move_menu_shortcut_handler(UI_device_width - shortcut_width, MMI_title_y);

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    show_softkey_background();
#endif 
    if (no_entries > 0)
    {
        dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
        draw_title();    
    }
    else
    {
        gui_fixed_icontext_menuitem_stop_scroll();
        gui_pop_up_description_stop_scroll();
        MMI_menu_shortcut_number = 0;
        draw_title();
        wgui_current_pop_up_description_index = -1;
    }
    show_dynamic_list();
}


/*****************************************************************************
 * FUNCTION
 *  refresh_category201_search_list
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void refresh_category201_search_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 no_entries = 0;
    U8 *name = MMI_multiline_inputbox.text;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    no_entries = cat200_search_function(name);
    load_dynamic_item_buffer(no_entries);

    if (no_entries >= 0)
    {
        refresh_category201_search_list_change_list(no_entries);

        gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
    }
}

 


/*****************************************************************************
 * FUNCTION
 *  cat201_register_key_func
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat201_register_key_func(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* when first word is highlighted, user should not be able to go to last item by pressing up arrow key. */
    if (MMI_fixed_list_menu.highlighted_item != 0)
    {
        SetKeyHandler(dynamic_list_goto_previous_item, KEY_UP_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(dynamic_list_goto_previous_item, KEY_VOL_UP, KEY_EVENT_DOWN);
    }

    SetKeyHandler(dynamic_list_goto_next_item, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(dynamic_list_goto_next_item, KEY_VOL_DOWN, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory201Screen
 * DESCRIPTION
 *  category screen of phone search
 * PARAMETERS
 *  title                       [IN]        Title id
 *  title_icon                  [IN]        Title icon
 *  left_softkey                [IN]        Left soft key label
 *  left_softkey_icon           [IN]        Left soft key icon
 *  right_softkey               [IN]        Right soft key label
 *  right_softkey_icon          [IN]        Right soft key icon
 *  number_of_items             [IN]        Number of items
 *  get_item_func               [IN]        
 *  get_hint_func               [IN]        
 *  highlighted_item            [IN]        Highlighted item index
 *  search_image                [IN]        Seach image
 *  buffer                      [IN]        Buffer
 *  buffer_max_length           [IN]        
 *  input_type                  [IN]        
 *  history_buffer              [IN]        Histpry buffer
 *  required_input_mode_set     [?]         
 *  top_msg(?)                  [IN]        Message display below title bar
 *  list_of_items(?)            [IN]        List of items
 
 *  list_of_icons1(?)           [IN]        List of icons
 * RETURNS
 *  void
 *****************************************************************************/
extern U8 wgui_multiline_inputbox_input_type;
extern void cat5_virtual_keypad_callback(void);
extern void wgui_redraw_input_information_bar(void);
void ShowCategory201Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        GetItemFuncPtr get_item_func,
        GetHintFuncPtr get_hint_func,
        S32 highlighted_item,
        U16 search_image,
        U8 *buffer,
        S32 buffer_max_length,
        S16 input_type,
        U8 *history_buffer,
        S16 *required_input_mode_set)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    U8 h_flag;
    S32 temp_w = 0, temp_h = 0;
    U8 *img = NULL;
    S32 input_box_width = 0;
    BOOL languageChanged = FALSE;
    S32 iwidth, iheight, max_entries = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    /* Begin AP input mode set */
    if (required_input_mode_set != NULL)
    {
        wgui_fill_AP_required_input_mode_set(required_input_mode_set);
        wgui_set_AP_required_input_mode_set_flag();
    }
    /* End AP input mode set */

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

    create_fixed_icontext_menuitems();

    MMI_current_menu_type = LIST_MENU;
    associate_fixed_icontext_list();
    MMI_disable_title_shortcut_display = 1;
    if (history_buffer)
    {
        if (wgui_multiline_inputbox_input_type != 0)
        {
            languageChanged = wgui_check_language_changed((U8) wgui_multiline_inputbox_input_type);

        }
    }
    init_dynamic_item_buffer(number_of_items, get_item_func, get_hint_func, UI_dummy_function);
    MMI_fixed_list_menu.n_items = number_of_items;
    if (wgui_check_application_present)
    {
        wgui_check_application_present(&max_entries);
    }

    /*
     * shortcut_width=set_menu_item_count(max_entries)+7; //number of element increases at the run time 
     * resize_menu_shortcut_handler(shortcut_width,MMI_title_height);
     * move_menu_shortcut_handler(UI_device_width-shortcut_width, MMI_title_y);
     * register_hide_menu_shortcut(gui_redraw_menu_shortcut);
     */
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_ALIGN_TO_TOP | UI_LIST_MENU_DISABLE_SCROLLBAR;

    register_fixed_list_highlight_handler(dynamic_list_highlight_handler);
    img = (U8*) GetImage(search_image);
    gui_measure_image(img, &temp_w, &temp_h);
    input_box_width = (UI_device_width - (5 + temp_w + 1) - 4);

    h_flag = set_list_menu_category_history(MMI_CATEGORY201_ID, history_buffer);

    if (h_flag)
    {
        S32 s = sizeof(list_menu_category_history);

        s = (s + 3) / 4;
        s *= 4;
        MMI_fixed_list_menu.item_measure_function(NULL, MMI_fixed_list_menu.common_item_data, &iwidth, &iheight);
        MMI_fixed_list_menu.height = iheight * MMI_fixed_list_menu.displayed_items + 10;        /* need to handle this in more cleaner way, it is 22X4 +10 =98, as we only show 4 rows */
        if (wgui_multiline_inputbox_input_type != 0)
        {
            if (languageChanged)
            {
                MMI_fixed_list_menu.highlighted_item = 0;
            }
        }
        dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
        wgui_setup_inputbox_ext(
            0,
            0,
            UI_device_width - temp_w,
            temp_h,
            buffer,
            buffer_max_length,
            MMI_CATEGORY201_ID,
            get_string(right_softkey),
            get_image(right_softkey_icon),
            input_type,
            history_buffer + s,
            1,
            UI_MULTI_LINE_INPUT_BOX_AUTO_DISABLE_SCROLLBAR,
            0,
            0,
            0,
            0,
            NULL);

        if (wgui_multiline_inputbox_input_type != 0)
        {
            if (languageChanged)
            {
                multiline_inputbox_delete_all_characters();
                set_right_softkey_function(GoBackHistory, KEY_EVENT_UP);
            }
        }

         
        if (cat200_search_function)
        {
            //S32 no_entries;
            //no_entries = /*cat200_search_function((U8*)MMI_multiline_inputbox.text)*/number_of_items;
            //load_dynamic_item_buffer(no_entries );
            //refresh_search_list_change_list_ext(no_entries,MMI_fixed_list_menu.highlighted_item);
        }
    }
    else
    {
        wgui_setup_inputbox_ext(
            0,
            0,
            UI_device_width - temp_w,
            temp_h,
            buffer,
            buffer_max_length,
            MMI_CATEGORY201_ID,
            get_string(right_softkey),
            get_image(right_softkey_icon),
            input_type,
            NULL,
            1,
            UI_MULTI_LINE_INPUT_BOX_AUTO_DISABLE_SCROLLBAR,
            0,
            0,
            0,
            0,
            NULL);
        dynamic_list_goto_item_no_redraw(highlighted_item);
    }

    /* multiline inputbox */
    inputbox_change_mode = change_multiline_inputbox_input_mode;
    MMI_multiline_inputbox.change_callback = refresh_category201_search_list;
    cat200_search_img = search_image;
    wgui_set_multiline_inputbox_register_up_down_key_funcptr(cat201_register_key_func);
    register_hide_multitap(category5_hide_multitap);

    /* up and down keys are handled by list menu */
    register_dynamic_list_keys();

    MMI_title_string = (UI_string_type) (get_string(title));
    MMI_title_icon = (PU8) (get_image(title_icon));

    resize_dynamic_icontext_menuitems_to_list_width();
    set_fixed_icontext_positions(MMI_ICONTEXT_MENUITEM_HEIGHT + 2, 0, 1, 0);//111106 dynamic icontext
    if (h_flag)
    {
        set_dynamic_pop_up_descriptions(MMI_fixed_list_menu.highlighted_item);
        if (MMI_fixed_list_menu.highlighted_item <= 0)
        {
            ClearKeyHandler(KEY_UP_ARROW, KEY_EVENT_DOWN);
            ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_DOWN);
        }

    }
    else
    {
        set_dynamic_pop_up_descriptions(highlighted_item);
    }
    wgui_multiline_inputbox_input_type = 0;
    wgui_singleline_inputbox_LSK_label_icon = get_image(left_softkey_icon);
    wgui_singleline_inputbox_LSK_label_string = get_string(left_softkey);

    gdi_layer_unlock_frame_buffer();
    ExitCategoryFunction = ExitCategory201Screen;
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory201History, GetCategory201HistorySize);
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_data.S32flags |= DM_SHOW_VKPAD;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY201_ID;
    dm_setup_data(&dm_data);
#ifdef __MMI_TOUCH_SCREEN__
    dm_register_vkpad_callback(cat201_virtual_keypad_callback);
#endif 
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategoryControlledArea201Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategoryControlledArea201Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    wgui_redraw_input_information_bar();
    show_dynamic_list();
#if defined(__MMI_TOUCH_SCREEN__)
    mmi_pen_editor_clear_and_show_virtual_keyboard_area();
    redraw_multiline_inputbox();
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}

#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__


/*****************************************************************************
 * FUNCTION
 *  ShowCategory273Screen
 * DESCRIPTION
 *  A category to display thumbnail images in menu item.
 *  * PARAMETERS
 *  *  title                IN Title for the screen
 *  *  title_icon           IN Icon shown with the title
 *  *  left_softkey         IN Left softkey label
 *  *  left_softkey_icon    IN Icon for the Left softkey
 *  *  right_softkey        IN right softkey label
 *  *  right_softkey_icon   IN Icon for the right softkey
 *  *  number_of_items      IN number of items to be displayed
 *  *  get_item_func        IN function pointer to get next displayed items
 *  *  get_display_style_func        IN function pointer to get the display style of next item
 *  *  get_properties_func                          IN      function pointer to get the properties to highlighted menu item
 *  *  highlighted_item     IN index of item to be highlighted
 *  *  history_buffer       IN history buffer
 *  *  disable_data_cache      IN whether caching of dynamic data is disabled
 *  *  flush_data_callback     IN callback function invoked before data is reloaded
 *  *
 *  * RETURNS
 *  *  void
 * PARAMETERS
 *  title                       [IN]        
 *  title_icon                  [IN]        
 *  left_softkey                [IN]        
 *  left_softkey_icon           [IN]        
 *  right_softkey               [IN]        
 *  right_softkey_icon          [IN]        
 *  number_of_items             [IN]        
 *  get_item_func               [IN]        
 *  get_display_style_func      [IN]        
 *  get_properties_func         [IN]        
 *  highlighted_item            [IN]        
 *  history_buffer              [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory273Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        GetItemFuncPtr get_item_func,
        wgui_get_display_style get_display_style_func,
        wgui_get_two_line_menu_item_properties get_properties_func,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 shortcut_width;
    U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    wgui_set_pfn_to_get_display_style(get_display_style_func);
    wgui_set_pfn_to_get_current_menu_item_properties(get_properties_func);
    gui_add_cleanup_hook(wgui_two_line_reset_all_pointers); 
    if (history_buffer)
    {
        wgui_two_line_disable_complete_callback();
    }
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    create_fixed_icontext_menuitems();
    MMI_current_menu_type = LIST_MENU;
    resize_fixed_list(MMI_content_width, MMI_content_height);
    associate_fixed_icontext_list();
    init_dynamic_item_buffer(number_of_items, get_item_func, NULL, UI_dummy_function);
    MMI_fixed_list_menu.n_items = number_of_items;
    register_dynamic_list_shortcut_handler();
    register_dynamic_list_keys();
    shortcut_width = set_menu_item_count(number_of_items) + 7;
    resize_menu_shortcut_handler(shortcut_width, (MMI_title_height));
    move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
    register_hide_menu_shortcut(draw_title);
    resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);
    set_fixed_icontext_positions(MMI_ICONTEXT_MENUITEM_HEIGHT + 2, 0, 1, 0);
    // TODO: temp removed

    register_fixed_list_highlight_handler(dynamic_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY273_ID, history_buffer);
    dynamic_list_goto_item_no_redraw(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);
    set_dynamic_pop_up_descriptions(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);
    gdi_layer_unlock_frame_buffer();
    ExitCategoryFunction = ExitCategory273Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY273_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}   /* end of ShowCategory273Screen */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory273Screen
 * DESCRIPTION
 *  Exits the Category 273 Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory273Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_dynamic_list();
    reset_pop_up_descriptions();
    dynamic_item_text_align_left = FALSE;
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    reset_all_force_flags_for_hints();
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
}
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 


/* barcode */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory228Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  buffer                  [?]         
 *  buffer_size             [IN]        
 *  hilite_str_list         [?]         
 *  hilite_str_number       [IN]        
 *  hilite_cb               [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
	extern void register_multiline_inputbox_hilite_keys(void);
void ShowCategory228Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *buffer,
        S32 buffer_size,
        HILITE_STR hilite_str_list[],
        U16 hilite_str_number,
        void (*hilite_cb) (S32 idx),
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;
    S16 input_type = 0;
    U8 h = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    l = gui_strlen((UI_string_type) buffer);
    create_multiline_inputbox_set_buffer((UI_string_type) buffer, buffer_size, l, 0);
    register_multiline_inputbox_viewer_keys();
    register_multiline_inputbox_hilite_keys();
    MMI_multiline_inputbox.hilite_list = hilite_str_list;
    MMI_multiline_inputbox.hilite_str_number = hilite_str_number;
    MMI_multiline_inputbox.current_hilite_idx = 0;

#ifdef __MMI_TOUCH_SCREEN__
    register_multiline_inputbox_hilite_pen_event_handler();
#endif 

    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE |
         UI_MULTI_LINE_INPUT_BOX_WORD_HIGHLIGHT);
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
#endif 
    MMI_multiline_inputbox.hilite_cb = hilite_cb;
    h = set_multiline_inputbox_category_history(MMI_CATEGORY228_ID, history_buffer, &input_type);
    if (hilite_str_number != 0)
    {
	    if (h)
	    {
	        gui_multi_line_input_box_go_to_hilite(
	            &MMI_multiline_inputbox,
	            (U16)MMI_multiline_inputbox.current_hilite_idx,
	            MMI_TRUE);
	    }
	    else
	    {
	        gui_multi_line_input_box_go_to_hilite(&MMI_multiline_inputbox, 0, MMI_FALSE);
	    }
    }
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory74Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY228_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}


void ExitCategory402Screen(void);
/*****************************************************************************
 * FUNCTION
 *  Cat402ScrolTextBg
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1                  [IN]        start x
 *  y1                  [IN]        start y
 *  x2                  [IN]        end x
 *  y2                  [IN]        end y
 * RETURNS
 *  void
 *****************************************************************************/
void Cat402ScrolTextBg(S32 x1, S32 y1, S32 x2, S32 y2)
{
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_WHITE);
}

/*****************************************************************************
 * FUNCTION
 *  ShowCategory402Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [?]         
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  string                  [?]         
 *  percentage              [IN]
 *  progress_string         [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory402Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *string,
        U16 percentage,
        U8 *progress_string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    dm_data_struct dm_data;
    color_t text_color = gui_color(0, 0, 0);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_string = (UI_string_type) title;
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    dm_add_scroll_text((UI_string_type) string, NULL, Cat402ScrolTextBg, text_color, text_color);
    Cat402PercentageBarId = dm_add_percentage_bar(
                                percentage,
                                progress_string,
                                UI_COLOR_RED,
                                IMG_PERCENTAGE_BAR_BG_IMAGE,
                                IMG_PERCENTAGE_BAR_LEFT_IMAGE,
                                IMG_PERCENTAGE_BAR_RIGHT_IMAGE);

    ExitCategoryFunction = ExitCategory402Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY402_ID;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  UpdateCategory402Value
 * DESCRIPTION
 *  
 * PARAMETERS
 *  value       [IN]    
 *  string      [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateCategory402Value(U16 value, U8 *string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_update_percentage_bar_value(Cat402PercentageBarId, value, string);
    dm_redraw_percentage_bar(Cat402PercentageBarId);
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory402Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory402Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat402PercentageBarId = 0;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory501Screen
 * DESCRIPTION
 *  Displays the category222 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the Left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Message string
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory501Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *message_str = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    message_str = (U8*) get_string((UI_image_ID_type ) message);
    ShowCategory7Screen(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        message_str,
        history_buffer);
}   /* end of ShowCategory501Screen */




/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-12 11:51 */
void (*category155_change_callback)(s32 value)=NULL;
/*****************************************************************************
* FUNCTION
*  category155_draw_slide_value_callback
*
* DESCRIPTION
*   show category155 slide value callback for Draw Manager
*
* PARAMETERS
*  coordinate			IN	coordinate of slide control
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
* 
*****************************************************************************/
void category155_draw_slide_value(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	s32 x1,y1,x2,y2;
    S32 string_width=0,string_height=0;
	s32 index=0;
	U8	ts[32];
	dm_coordinates coordinate;

#ifdef __MMI_MAINLCD_220X176__
	if(category155_change_callback!=NULL) 
		category155_change_callback(MMI_slide_control[0].current_value);	
	return;
#endif
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for ( index=0;index<MMI_MAX_SLIDE_CONTROL; index++)
		if ( MMI_current_slide_control == &(MMI_slide_control[index]) )
		{
			dm_get_control_coordinates_from_category(-1,MMI_CATEGORY87_ID,DM_SLIDE_CONTROL,index,&coordinate);
			break;
		}
	gui_itoa(MMI_current_slide_control->upper_limit,(UI_string_type)ts,10);
	gui_measure_string((UI_string_type)ts,&string_width,&string_height);

	x1=coordinate.S16X+((coordinate.S16Width-string_width)>>1)-20;//blank before text
	x2=x1+string_width+40-1;//blank after text
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined __MMI_MAINLCD_320X240__
	y1=coordinate.S16Y+MMI_current_slide_control->bg_image_height+30-12;
	#else
	y1=coordinate.S16Y+MMI_current_slide_control->bg_image_height+30;
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	y2=y1+18;

	wgui_show_slide_control_value(x1,y1,x2,y2,MMI_current_slide_control->current_value);

	if(category155_change_callback!=NULL) category155_change_callback(MMI_slide_control[0].current_value);

	wgui_show_slide_control_focus();
}

/*****************************************************************************
* FUNCTION
*  category155_draw_slide_value
*
* DESCRIPTION
*   show category155 slide value callback
*
* PARAMETERS
*  NULL
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
* 
*****************************************************************************/
void category155_draw_slide_value_callback(dm_coordinates *coordinate_set)
{
	UI_UNUSED_PARAMETER(coordinate_set);

	category155_draw_slide_value();
}
/*****************************************************************************
* FUNCTION
*  category155_keyboard_key_handler
*
* DESCRIPTION
*   register key handlers ofcategory 155
*
* PARAMETERS
*  vkey_code			IN	specify the key
*  key_state			IN	state of the key
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
* 
*****************************************************************************/
void category155_keyboard_key_handler(s32 vkey_code,s32 key_state)
{
#if(MMI_BUILD_TYPE==BUILD_TYPE_X86WIN32)
	if(key_state)
	{	switch(vkey_code)
		{	case 37:	wgui_slide_control_previous();
						break;
			case 39:	wgui_slide_control_next();
						break;
			case 38:	//up
						break;
			case 40:	//down
						break;
		}
	}
#else
	UI_UNUSED_PARAMETER(vkey_code);
	UI_UNUSED_PARAMETER(key_state);
#endif
}/* end of category155_keyboard_key_handler */

extern void ExitCategory155Screen();
U16 TitlePlaceFlag = 0 ;

/*****************************************************************************
* FUNCTION			
*  ShowCategory88Screen(slide bar)
*
* DESCRIPTION
*  Displays the Double Slide control screen
*
* PARAMETERS
*  title					IN	Title for the screen
*  title_icon			IN	Icon shown with the title
*  left_softkey			IN	Left softkey label
*  left_softkey_icon	IN	Icon for the Left softkey
*  right_softkey		IN	right softkey label
*  right_softkey_icon	IN	Icon for the right softkey
*  string1				IN	title of slide bar 1
*  string2				IN	title of slide bar 2
*  l_limit				IN	Upper limit value of slide bar 
*  u_limit				IN	Lower limit value of slide bar
*  current_value		IN	current value of slide bar
*  history_buffer		IN	history buffer
*
* RETURNS                 
*  void
*
* GLOBALS AFFECTED
*  ExitCategoryFunction, RedrawCategoryFunction, GetCategoryHistory
*  GetCategoryHistorySize, g_dm_data_struct
* *****************************************************************************/
void ShowCategory155Screen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U16 bg_image_ID,U16 message,S32 l_limit,S32 u_limit,S32 *current_value,U8* history_buffer)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	dm_data_struct	dm_data;
	byte			h_flag;
  s32 BG_ICON;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	UI_UNUSED_PARAMETER(history_buffer);

if(bg_image_ID == 0)
{
    BG_ICON = (s32)current_MMI_theme->list_background_filler->b;
}
else
{
   BG_ICON = (s32)get_image(bg_image_ID);
}

	gui_lock_double_buffer();
	category155_change_callback=NULL;
	MMI_menu_shortcut_number=-1;
	change_left_softkey(left_softkey,left_softkey_icon);
	change_right_softkey(right_softkey,right_softkey_icon);
	SetupCategoryKeyHandlers();
	MMI_title_string=get_string(title);
	MMI_title_icon=get_image(title_icon);
	h_flag=set_slide_control_category_history(MMI_CATEGORY155_ID,history_buffer);
	if(h_flag)
		*current_value=*MMI_slide_control_value[0];
	
	category155_slide_string=(UI_string_type)get_string(message);

//	dm_add_image(get_image(bg_image_ID), NULL,NULL);
	dm_add_image((PU8)BG_ICON, NULL,NULL);
	dm_add_image(get_image(WGUI_SLIDE_CONTROL_BG_IMAGE_ID), NULL,NULL);
	dm_add_slide_control(l_limit,u_limit,*current_value,(void (*)(S32))category155_draw_slide_value,NULL);
	dm_register_category_controlled_callback(category155_draw_slide_value_callback);
	dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
	MMI_slide_control_value[0]=current_value;
	MMI_slide_control_value[1]=current_value;
	wgui_slide_control_set_focus(&MMI_slide_control[0]);
	SetKeyHandler(wgui_slide_control_previous,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(wgui_slide_control_next,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	register_keyboard_key_handler((void (*)(S32, S32))category155_keyboard_key_handler);
	gui_unlock_double_buffer();

	ExitCategoryFunction=ExitCategory155Screen;
	dm_setup_category_functions(dm_redraw_category_screen,dm_get_category_history,dm_get_category_history_size);
	dm_data.S32ScrId = (s32)GetActiveScreenId();
	dm_data.S32CatId = MMI_CATEGORY155_ID;
	dm_data.S32flags = 0;
	dm_setup_data(&dm_data);
	dm_redraw_category_screen();	
}/* end of ShowCategory155Screen */
/*****************************************************************************
* FUNCTION
*  RegisterCategory155ChangeCallback
*
* DESCRIPTION
*  register change call back function of category 155
*
* PARAMETERS
*  f			IN	callback function
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
* 
*****************************************************************************/
void RegisterCategory155ChangeCallback(void (*f)(s32 value))
{	
	category155_change_callback=f;
}

/*****************************************************************************
* FUNCTION
*  ExitCategory155Screen
*
* DESCRIPTION
*   Exits the Single Slide control screen
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
* 
*****************************************************************************/
void ExitCategory155Screen(void)
{	category155_change_callback=NULL;
	reset_softkeys();
}/* end of ExitCategory155Screen */

#if	defined(__PROJECT_GALLITE_C01__)//1//def __MMI_INTELIGENT_CALL_NUMBER__

#if defined(__DIAL_FONT_USE_PIC__)
BOOL pic_font_flag = MMI_FALSE;//MMI_FALSE;
BOOL Is_category658_pic_font(void)
{
   return pic_font_flag;
}
#endif
void wgui_setup_dialer_inputbox(
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



void register_dailer_input_change_callback();
void dailer_new_input_change_callback(void);


////////////////
// category658 function start here.
////////

///
static void (*cb_dialer_input_old)(void) = NULL;


/* Begin AP input mode set */
extern U8 MMI_AP_set_input_mode_set;

/* End AP input mode set */
void refresh_category658_search_list(void);
static MMI_BOOL is_category658_timer_started = MMI_FALSE;

///
void ExitCategory658Screen(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	gui_cancel_timer(refresh_category658_search_list);
	is_category658_timer_started = MMI_FALSE;
#if defined(__DIAL_FONT_USE_PIC__)
     pic_font_flag = MMI_FALSE;//MMI_FALSE;
#endif	 	
	
	ClearHighlightHandler();
	

	reset_menu_shortcut_handler();
	reset_fixed_list();
	reset_dynamic_list();
	reset_pop_up_descriptions();

	dialer_inputbox_handle_multitap_complete(WGUI_DIALER_BOX_ACTIVE_MULTITAP_ANY);
	//reset_multitaps();
	close_dialer_inputbox();
	//close_main_LCD_dt_display();
	close_status_icons();
	reset_softkeys();
#if defined(__PROJECT_GALLITE_C01__)	
	set_status_icons_display(MMI_TRUE);
	wgui_close_singleline_inputbox();
	MMI_fixed_list_menu.flags |= UI_LIST_MENU_LOOP;
#endif	
	MMI_dialer_inputbox.change_callback = cb_dialer_input_old;

	dynamic_item_text_align_left = FALSE;
	/* End AP input mode set */
	cat200_search_function = NULL;
	ExitCategoryFunction = MMI_dummy_function;
	RedrawCategoryFunction = MMI_dummy_function;
	GetCategoryHistory = dummy_get_history;
	GetCategoryHistorySize = dummy_get_history_size;
    
#if defined __MMI_GW_NOKIA_IDLESCREEN__
#if defined(__Q10_YXD__)
    g_hide_tile_status_icon = FALSE;
#endif    
#endif
}



///
U8 *GetCategory658History(U8 *history_buffer)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 s;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	get_list_menu_category_history(MMI_CATEGORY658_ID, history_buffer);
	s = sizeof(list_menu_category_history);
	s = (s + 3) / 4;
	s *= 4;
	
	get_dialer_inputbox_category_history(MMI_CATEGORY658_ID, (U8*) (history_buffer + s));
	
	return (history_buffer);
}


///
S32 GetCategory658HistorySize(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	 
	return (((sizeof(list_menu_category_history) + 3) / 4) * 4 + sizeof(dialer_inputbox_category_history));
}

/* store the string of RSK of dialer input box */
extern UI_string_type wgui_dialer_inputbox_RSK_label_string;
/* store the icon of RSK of dialer input box */
extern PU8 wgui_dialer_inputbox_RSK_label_icon;
extern void mmi_dailer_word_hdlr(S32 index);
///
void refresh_category658_search_list_change_list(S32 no_entries, S32 highlight)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	//S32 shortcut_width;
	//S32 max_entries = 0;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	MMI_fixed_list_menu.highlighted_item = highlight;
	MMI_fixed_list_menu.n_items = no_entries;
	MMI_fixed_list_menu.displayed_items = no_entries;

	set_left_softkey_label(wgui_singleline_inputbox_LSK_label_string);
	set_left_softkey_icon(wgui_singleline_inputbox_LSK_label_icon);
	redraw_left_softkey();

	redraw_right_softkey();
	menu_shortcut_handler_display = 1;
	if (no_entries > 0)
	{
		dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);

		 mmi_dailer_word_hdlr(0);
	}
	else
	{
		gui_fixed_icontext_menuitem_stop_scroll();
		gui_pop_up_description_stop_scroll();

		wgui_current_pop_up_description_index = -1;
	}
	
	show_dynamic_list();
}


///
void refresh_category658_search_list(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 no_entries = 0;
	U8 *name = MMI_singleline_inputbox.text;

	  dynamic_item_text_align_left = TRUE;
	  
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	no_entries = cat200_search_function(name);
	load_dynamic_item_buffer(no_entries);
	
	//if (no_entries >= 0)
	{
//		if (is_category658_timer_started)
		{
			refresh_category658_search_list_change_list(no_entries, 0);
		}
	}
    
       if (0 == name[0])//(NEW_DIAL_SCREEN)
       {
           SetKeyHandler(GoBackHistory, KEY_RSK, KEY_EVENT_UP);
       }	
       
	is_category658_timer_started = MMI_FALSE;
}


///
void cat658_dailer_inputbox_text_change(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
//	if (!is_category658_timer_started)
	{
		refresh_category658_search_list();
	}
	
//	gui_cancel_timer(refresh_category658_search_list);
//	gui_start_timer(200, refresh_category658_search_list);
	is_category658_timer_started = MMI_TRUE;
}

///
void cat658_register_key_func(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* when first word is highlighted, user should not be able to go to last item by pressing up arrow key. */
	if (MMI_fixed_list_menu.highlighted_item != 0 || MMI_fixed_list_menu.n_items == 0)
	{
		SetKeyHandler(dynamic_list_goto_previous_item, KEY_UP_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(dynamic_list_goto_previous_item, KEY_VOL_UP, KEY_EVENT_DOWN);
	}
	else
	{
		/* input method will register key event, disable here */
		SetKeyHandler(UI_dummy_function, KEY_UP_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(UI_dummy_function, KEY_VOL_UP, KEY_EVENT_DOWN);
	}
	
	SetKeyHandler(dynamic_list_goto_next_item, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(dynamic_list_goto_next_item, KEY_VOL_DOWN, KEY_EVENT_DOWN);
}


extern void wgui_singleline_inputbox_phone_number_input_handle_multitap_star_key_down(void);
extern void wgui_singleline_inputbox_phone_number_input_handle_multitap_star_key_up(void);
extern multitap_input wgui_inputbox_multitap_star_key;
extern UI_character_type wgui_inputbox_phone_number_star_key_string[];
///
 void ShowCategory658Screen(
	 U16 title,
	 U16 title_icon,
	 U16 left_softkey,
	 U16 left_softkey_icon,
	 U16 right_softkey,
	 U16 right_softkey_icon,
	 S32 number_of_items,
	 GetItemFuncPtr get_item_func,
	 GetHintFuncPtr get_hint_func,
	 S32 highlighted_item,
	 U16 search_image,
	 U8 *buffer,
	 S32 buffer_max_length,
	 U8 *history_buffer)
 {
	 /*----------------------------------------------------------------*/
	 /* Local Variables                                                */
	 /*----------------------------------------------------------------*/
	 dm_data_struct dm_data;
	 U8 h_flag;
	 BOOL application_present;
	 S32 max_entries = 0;
	 S16 dailer_input_x;
	 S16 dailer_input_y;
	 

#if defined __MMI_GW_NOKIA_IDLESCREEN__
#if defined(__Q10_YXD__)
    g_hide_tile_status_icon = TRUE;
#endif    
#endif

	  dynamic_item_text_align_left = TRUE;
	  
#if defined(__DIAL_FONT_USE_PIC__)
     pic_font_flag = MMI_TRUE;
#endif	 
	 /*----------------------------------------------------------------*/
	 /* Code Body                                                      */
	 /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);

    register_left_softkey_handler();
    register_right_softkey_handler();

    SetupCategoryKeyHandlers();

    wgui_singleline_inputbox_LSK_label_icon = get_image(left_softkey_icon);
    wgui_singleline_inputbox_LSK_label_string = get_string(left_softkey);

    create_fixed_icontext_menuitems();

    MMI_current_menu_type = LIST_MENU;
    associate_fixed_icontext_list();
    init_dynamic_item_buffer(number_of_items, get_item_func, get_hint_func, UI_dummy_function);
    MMI_fixed_list_menu.n_items = number_of_items;

       ////////////////// ShowStatusIconsTitle();
        //show_status_icon_bar(1);    //HEZHIPENG 2008-10-20 Bug #6349 ����ˮƽ��״̬����ʾ����ʱ,��ֱ��״̬����Ч,���༭��һͨ����¼ʱ����ֱ��״̬����ͼ����绰��¼��ʾ�ص� ��
	application_present = MMI_FALSE;
	 
	 if (application_present)
	 {
		 /* number of words goes beyond this limit, and increases at the run time */
		 set_menu_item_count(max_entries);
	 }
	 else
	 {
		 set_menu_item_count(number_of_items);
	 }
	 
#if defined(__PROJECT_GALLITE_C01__) 
	 mmi_dailer_word_hdlr(0);
#else
	 register_dynamic_list_keys();
#endif 
	 
#ifdef __MMI_TOUCH_SCREEN__
	 /* Because the list menu is resized according to virtual keyboard */
	 MMI_fixed_list_menu.flags |= UI_LIST_MENU_ALIGN_TO_TOP;
#endif /* __MMI_TOUCH_SCREEN__ */ 
	 
#ifdef __MMI_UI_LIST_CACHE_BACKGROUND__
	 /* Virtual keyboard height might be changed. We cannot put menu background in layer */
	 MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER;
#endif /* __MMI_UI_LIST_CACHE_BACKGROUND__ */ 

	 MMI_fixed_list_menu.flags |= UI_MENUITEM_DISABLE_ICON;
#if defined(__PROJECT_GALLITE_C01__) 
        MMI_fixed_list_menu.flags &= ~UI_LIST_MENU_LOOP;
#endif 

	dailer_input_x = 0;
	dailer_input_y = 0;
	 
#if defined(__PROJECT_GALLITE_C01__) 
	 register_fixed_list_highlight_handler(mmi_dailer_word_hdlr);
#else
	 register_fixed_list_highlight_handler(dynamic_list_highlight_handler);
#endif 
	 h_flag = set_list_menu_category_history(MMI_CATEGORY658_ID, history_buffer);
	 
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
	 if (get_hint_func)
	 {
		 if (is_set_force_icon_on_highlight_only_in_menuitem())
		 {
			 wgui_show_icon_only_highlight_in_icontext_menuitem();
		 }
		 if (is_set_force_all_hints_in_menuitem())
		 {
			 wgui_enable_hints_in_icontext_menuitem();
		 }
		 else if (is_set_force_hints_on_highlight_in_menuitem())
		 {
			 wgui_enable_hint_highlight_in_icontext_menuitem();
		 }
		 else
		 {
			 wgui_enable_hint_highlight_in_icontext_menuitem();
		 }
		 
		 if (h_flag)
		 {
			 set_dynamic_pop_up_descriptions(MMI_fixed_list_menu.highlighted_item);
		 }
		 else
		 {
			 set_dynamic_pop_up_descriptions(highlighted_item);
		 }
		 
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
		 gui_block_list_effect();
#endif 
	 }
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
#if defined(__PROJECT_GALLITE_C01__) 
        wgui_setup_singleline_inputbox(
            0,
            106,
            UI_device_width - 1,
            32,
            buffer,
            buffer_max_length,
            MMI_CATEGORY658_ID,
            get_string(right_softkey),
            get_image(right_softkey_icon),
            INPUT_TYPE_NUMERIC,
            history_buffer,
            0);
        gui_set_single_line_input_box_theme(&MMI_singleline_inputbox, &PIN_singleline_inputbox_theme);

            gui_create_multitap_input(
                &wgui_inputbox_multitap_star_key,
                0,
                0,
                0,
                0,
                (UI_string_type) wgui_inputbox_phone_number_star_key_string);
            gui_set_multitap_input_callbacks(
                &wgui_inputbox_multitap_star_key,
                singleline_inputbox_multitap_input,
                singleline_inputbox_multitap_input_complete);
#else
	 
	 if (h_flag)
	 {
		 S32 s = sizeof(list_menu_category_history);
		 
		 s = (s + 3) / 4;
		 s *= 4;
		dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
		wgui_setup_dialer_inputbox(
					0,
                #if 0
					MMI_STATUS_BAR_HEIGHT,
                #else
					0,
                #endif
					UI_device_width,
					DAILING_INPUTBOX_HEIGHT,
					(UI_buffer_type) buffer,
					buffer_max_length,
					MMI_CATEGORY658_ID,
					get_string(right_softkey),
					get_image(right_softkey_icon),
					history_buffer + s,
					0);
	
		 if (cat200_search_function)
		 {
		 	// ���ڻ������Ӽ�¼��������������ͷ��ʼ
		 	S32 item = 0; //MMI_fixed_list_menu.highlighted_item;
			
			 if (!application_present)
			 {
				 S32 no_entries;
				 
				 no_entries = cat200_search_function((U8*) MMI_dialer_inputbox.text);
				 load_dynamic_item_buffer(no_entries);
				 refresh_category658_search_list_change_list(no_entries, item);
			 }
			 
		 }
		 
	 }
	 else
	 {
		dynamic_list_goto_item_no_redraw(highlighted_item);
		wgui_setup_dialer_inputbox(
					0,
					0,
					UI_device_width,
					DAILING_INPUTBOX_HEIGHT,
					(UI_buffer_type) buffer,
					buffer_max_length,
					 MMI_CATEGORY658_ID,
					get_string(right_softkey),
					get_image(right_softkey_icon),
					history_buffer,
					0);

	 }
#endif	 
#if defined(__MMI_TOUCH_SCREEN__) 

		resize_fixed_list(
		 MMI_content_width,		
		UI_device_height - MMI_SOFTKEY_HEIGHT - DAILING_DYNAMIC_LIST_Y);	 
	   move_fixed_list(dailer_input_x, DAILING_DYNAMIC_LIST_Y );
	 	
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

#if defined(__PROJECT_GALLITE_C01__) 
       register_dialer_inputbox_dialer_keys_special();//(NEW_DIAL_SCREEN)	 
#endif	 
	 
	 MMI_title_string = (UI_string_type) get_string(title);
	 MMI_title_icon = (PU8) get_image(title_icon);
	 
	 resize_dynamic_icontext_menuitems_to_list_width();
#if defined(__PROJECT_GALLITE_C01__) 
       resize_fixed_icontext_menuitems(MMI_fixed_list_menu.width - 4, 24);//(NEW_DIAL_SCREEN)
#endif	 


	// register callback function
#if defined(__PROJECT_GALLITE_C01__) 
      register_singleline_inputbox_input_callback(refresh_category658_search_list);//(NEW_DIAL_SCREEN)
#else
	register_dailer_input_change_callback();
#endif	 

	 gdi_layer_unlock_frame_buffer();
	 
#if defined(__PROJECT_GALLITE_C01__)	
	set_status_icons_display(MMI_FALSE);
#endif	

	 ExitCategoryFunction = ExitCategory658Screen;
	 dm_setup_category_functions(dm_redraw_category_screen, GetCategory658History, GetCategory658HistorySize);    
#if defined(__PROJECT_GALLITE_C01__) 
	 dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND/* | DM_NO_TITLE*/;//(NEW_DIAL_SCREEN)
#else
	 dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND | DM_NO_TITLE;//|DM_NO_BLT;
#endif	 
	 dm_data.S32ScrId = (S32) GetActiveScreenId();
	 dm_data.S32CatId = MMI_CATEGORY658_ID;
	 dm_setup_data(&dm_data);
	 dm_redraw_category_screen();	 
}


////
void register_dailer_input_change_callback()
{
	if (MMI_dialer_inputbox.change_callback == dailer_new_input_change_callback)
	{
		return;
	}
	
	cb_dialer_input_old = MMI_dialer_inputbox.change_callback;
	MMI_dialer_inputbox.change_callback = dailer_new_input_change_callback;

}


///
void dailer_new_input_change_callback(void)
{
	// �ȵ��þɵ� dailer �ı��仯 callback
//	(*cb_dialer_input_old)();

	// �������ǵ�cb����
	cat658_dailer_inputbox_text_change();
}

// added by qinjiangwei ended
#endif


/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-12 11:51 */
#ifdef __FT_IDLE_SMART_DIAL__ 
#include "phonebookprot.h"
UI_filled_area	*fixed_list_smartdial_backup_filler;
void RegisterCatSmartDialSearchFunction(S32 (*search_function)(U8 *))
{
	if(search_function)
	{
		catSmartDial_search_function = search_function;
	}
}


#ifdef __SLT_MMI_SMARTDIAL_TOUCHPANEL__
MMI_BOOL CateSmartDialCategoryControlAreaPenDownHandler(mmi_pen_point_struct point )
{
	MMI_BOOL ret = MMI_FALSE;
	gui_list_pen_enum menu_event;
	
	MMI_fixed_list_menu.pen_redraw_menu_function = show_dynamic_list;
	gui_vertical_scrollbar_set_pen_self_scroll(&MMI_fixed_list_menu.vbar, MMI_TRUE);
	gui_fixed_list_menu_set_pen_scroll_delay(&MMI_fixed_list_menu, GUI_DYNAMIC_LIST_PEN_SCROLL_DELAY);

	ret = gui_dynamic_list_menu_translate_pen_event(&MMI_fixed_list_menu, 
			MMI_PEN_EVENT_DOWN, point.x, point.y, &menu_event);

	if (ret)
	{
		if (menu_event == GUI_LIST_PEN_HIGHLIGHT_CHANGED || 
			menu_event == GUI_LIST_PEN_NEED_REDRAW)
		{
			show_dynamic_list();
		}
		return MMI_TRUE;
	}

	return MMI_FALSE;
}

MMI_BOOL CateSmartDialCategoryControlAreaPenUpHandler(mmi_pen_point_struct point )
{
	MMI_BOOL ret = MMI_FALSE;
	gui_list_pen_enum menu_event;

	ret=gui_dynamic_list_menu_translate_pen_event(&MMI_fixed_list_menu, 
		MMI_PEN_EVENT_UP, point.x, point.y, &menu_event);
	if (ret )
	{
		if (menu_event == GUI_LIST_PEN_HIGHLIGHT_CHANGED || 
			menu_event == GUI_LIST_PEN_NEED_REDRAW ||
			menu_event == GUI_LIST_PEN_ITEM_SELECTED)
		{
			MMI_fixed_list_menu.pen_redraw_menu_function();
		}
		/*
		if (menu_event == GUI_LIST_PEN_ITEM_SELECTED)
		{
			IdleDialPadCall();
		}
		*/
		return MMI_TRUE;
	}

	return MMI_FALSE;
}

MMI_BOOL CateSmartDialCategoryControlAreaPenMoveHandler(mmi_pen_point_struct point )
{
	MMI_BOOL ret = MMI_FALSE;

	wgui_general_pen_dynamic_list_menu_hdlr(point, MMI_PEN_EVENT_MOVE);
	return ret;
}

MMI_BOOL CateSmartDialCategoryControlAreaPenLongTapHandler(mmi_pen_point_struct point )
{
	MMI_BOOL ret = MMI_FALSE;

	wgui_general_pen_dynamic_list_menu_hdlr(point, MMI_PEN_EVENT_LONG_TAP);
	return ret;
}

MMI_BOOL CateSmartDialCategoryControlAreaPenRepeatHandler(mmi_pen_point_struct point )
{
	MMI_BOOL ret = MMI_FALSE;

	wgui_general_pen_dynamic_list_menu_hdlr(point, MMI_PEN_EVENT_REPEAT);
	return ret;
}
#endif
extern S32  OnChangeEdit(U8* KeyWord) ;
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
void ExitCategorySmartDialScreen(void);
void RedrawCategorySmartDialScreen(dm_coordinates* coordinate);
void ShowCategorySmartDialScreen(U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,S32 number_of_items,
	                                                                    GetItemFuncPtr get_item_func, GetHintFuncPtr	get_hint_func,
	                                                                         S32 highlighted_item,	U16 search_image,	U8 * buffer ,	U32 BufferLength,U8* history_buffer )
{
  /*-----------------------------------------------------------------*/
   /* Local Variables																		*/
   /*-----------------------------------------------------------------*/
   dm_data_struct dm_data;
    byte  h_flag;
//    int i;
	//U16 diaboxheight = MMI_DIALING_BOX_HEIGHT;
//int number_of_items=20;
//	U16 diaboxheight = 50;

   /*-----------------------------------------------------------------*/
   /* Code Body																			*/
   /*-----------------------------------------------------------------*/

    MMI_disable_title_shortcut_display=1;
	disable_menu_shortcut_box_display=1;
	
	#ifdef __FT_MMI_SMART_DIALER_SCREEN__
	menu_shortcut_handler_display = 1;
	#endif
	
    UI_UNUSED_PARAMETER(history_buffer);
	gui_lock_double_buffer();
	//gdi_layer_lock_frame_buffer();
	
	clear_key_handlers();
	clear_left_softkey();
	clear_right_softkey();
	change_right_softkey(right_softkey,right_softkey_icon);
	change_left_softkey(left_softkey,left_softkey_icon);
	register_left_softkey_handler();
	register_right_softkey_handler();

	register_default_hide_softkeys();
	//kal_prompt_trace(MOD_NVRAM,"xujuntrace ShowCategorySmartDialScreen");

	RegisterCatSmartDialSearchFunction(OnChangeEdit);
#ifndef __MMI_MAINLCD_220X176__
	
	ShowStatusIconsTitle();
	#endif

	#ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
		arrange_status_icons();
	#endif
#if defined(__MMI_MAINLCD_220X176__)	
	MMI_title_string = NULL;
	show_status_icons();
	#endif
//dialer inputbox
	#ifdef __FT_MMI_SMART_DIALER_SCREEN__
	wgui_setup_dialer_inputbox(0,MMI_STATUS_BAR_HEIGHT+1, UI_device_width, MMI_DIALING_BOX_HEIGHT ,(UI_buffer_type)buffer,BufferLength,MMI_CATEGORY_SmartDial_ID,get_string(right_softkey),get_image(right_softkey_icon),history_buffer,0);
	#else
	wgui_setup_dialer_inputbox(0,MMI_TITLE_HEIGHT-2,UI_device_width,UI_device_height/3-16,(UI_buffer_type)buffer,BufferLength,MMI_CATEGORY_SmartDial_ID,get_string(right_softkey),get_image(right_softkey_icon),history_buffer,0);
	#endif
   
   MMI_dialer_inputbox.background_filler = catSmartdial_dialer_input_box_theme.normal_filler;

//List
	create_fixed_icontext_menuitems();
       MMI_current_menu_type=LIST_MENU;
     
	associate_fixed_icontext_list();	
	
      init_dynamic_item_buffer(number_of_items, get_item_func, get_hint_func,UI_dummy_function);
	MMI_fixed_list_menu.n_items = number_of_items;
	register_dynamic_SmartDial_list_keys();
	
	#ifdef __FT_MMI_SMART_DIALER_SCREEN__
       resize_fixed_list(MMI_content_width,UI_device_height - MMI_STATUS_BAR_HEIGHT-MMI_DIALING_BOX_HEIGHT - MMI_button_bar_height);
	move_fixed_list(0,MMI_DIALING_BOX_HEIGHT+MMI_STATUS_BAR_HEIGHT);
	register_fixed_list_highlight_handler(standard_list_highlight_handler);
	fixed_list_smartdial_backup_filler = MMI_fixed_list_menu.normal_filler;

       MMI_fixed_list_menu.normal_filler=catSmartdial_DynList_theme.normal_filler;
       //MMI_fixed_list_menu.focussed_filler=catSmartdial_DynList_theme.focussed_filler;	
	#else
       resize_fixed_list(MMI_content_width,UI_device_height -UI_device_height/3+16-MMI_TITLE_HEIGHT - MMI_button_bar_height+2);
	move_fixed_list(0,UI_device_height/3+MMI_TITLE_HEIGHT-16-2);
	//register_fixed_list_highlight_handler(dynamic_list_highlight_handler);
	register_fixed_list_highlight_handler(standard_list_highlight_handler);
	fixed_list_smartdial_backup_filler = MMI_fixed_list_menu.normal_filler;

       MMI_fixed_list_menu.normal_filler=catSmartdial_DynList_theme.normal_filler;
       //MMI_fixed_list_menu.focussed_filler=catSmartdial_DynList_theme.focussed_filler;
	#endif
	

  //    jump to that item and highlight it.
	h_flag=set_list_menu_category_history(MMI_CATEGORY_SmartDial_ID,history_buffer);
	  if(h_flag) 
      {
         s32 s = sizeof(list_menu_category_history);
         s = (s+3)/4;
         s *= 4;
         dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    
          
         //kal_prompt_trace(MOD_NVRAM,"xujuntrace catsmartdial_search_function");
         if (catSmartDial_search_function)
         {
            S32 no_entries;
            no_entries = catSmartDial_search_function(MMI_dialer_inputbox.text);
            load_dynamic_item_buffer(no_entries );
	     show_dynamic_list();
     
         }
	}
      else 
      {
         dynamic_list_goto_item_no_redraw(highlighted_item);
        
      }

      if( h_flag )
      {
         set_dynamic_pop_up_descriptions(MMI_fixed_list_menu.highlighted_item);
      }
      else 
      {
         set_dynamic_pop_up_descriptions(highlighted_item);
      }
	gui_unlock_double_buffer();

       //gdi_layer_unlock_frame_buffer();
	

	ExitCategoryFunction = ExitCategorySmartDialScreen;

      dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
	dm_data.S32flags=DM_CLEAR_SCREEN_BACKGROUND;
	dm_data.S32ScrId = (S32)GetActiveScreenId();
	dm_data.S32CatId = MMI_CATEGORY_SmartDial_ID;
	dm_setup_data(&dm_data);
	dm_register_category_controlled_callback(RedrawCategorySmartDialScreen);

#ifdef __SLT_MMI_SMARTDIAL_TOUCHPANEL__
	wgui_register_category_screen_control_area_pen_handlers(CateSmartDialCategoryControlAreaPenDownHandler,MMI_PEN_EVENT_DOWN);
	wgui_register_category_screen_control_area_pen_handlers(CateSmartDialCategoryControlAreaPenUpHandler,MMI_PEN_EVENT_UP);
	wgui_register_category_screen_control_area_pen_handlers(CateSmartDialCategoryControlAreaPenMoveHandler,MMI_PEN_EVENT_MOVE);
	wgui_register_category_screen_control_area_pen_handlers(CateSmartDialCategoryControlAreaPenLongTapHandler,MMI_PEN_EVENT_LONG_TAP);
	wgui_register_category_screen_control_area_pen_handlers(CateSmartDialCategoryControlAreaPenRepeatHandler,MMI_PEN_EVENT_REPEAT);
	#endif
	dm_redraw_category_screen();

}/* end of ShowCategorySmartdialScreen */

/*****************************************************************************
* FUNCTION
*  ExitCategorySmartdialScreen
*
* DESCRIPTION
*   Exits the categorySmartDial screen
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
* 
*****************************************************************************/
extern void dialer_inputbox_handle_multitap_complete(S32 type);
extern void close_dialer_inputbox(void);
void ExitCategorySmartDialScreen(void)
{
	gui_lock_double_buffer();
	dialer_inputbox_handle_multitap_complete(WGUI_DIALER_BOX_ACTIVE_MULTITAP_ANY);
	reset_multitaps();
	close_dialer_inputbox();
	//close_main_LCD_dt_display();
	MMI_fixed_list_menu.normal_filler =fixed_list_smartdial_backup_filler ;
	reset_fixed_list();
	
	reset_dynamic_list();
	catSmartDial_search_function = NULL;
	//kal_prompt_trace(MOD_NVRAM,"xujuntrace ExitCategorySmartDialScreen");
	close_status_icons();
	reset_softkeys();
#if ( defined ( __MMI_UI_STYLE_3__ ) && defined (__GDI_MEMORY_PROFILE_2__) )
	leave_idle_screen();
	enactive_main_lcd_update_date_time();
	enable_softkey_background();
	on_idle_screen=0;
	register_hide_status_icon_bar(1,MMI_dummy_function);
	register_hide_status_icon_bar(0,MMI_dummy_function);
#endif
	gui_unlock_double_buffer();

	#ifdef __FT_MMI_SMART_DIALER_SCREEN__
	menu_shortcut_handler_display = 0;
	#endif


}/* end of ExitCategorySmartDialScreen */
void RedrawCategorySmartDialScreen(dm_coordinates* coordinate)
{
    
	gdi_layer_lock_frame_buffer();
	gdi_layer_reset_clip();
	//pixtel_UI_draw_filled_area(0,0,UI_device_width-1,UI_device_height-1,f);
	#if 1//ndef __SLT_MMI_SMARTDIAL_TOUCHPANEL__
	show_status_icons();
	show_dialer_inputbox();
	#endif
       show_dynamic_list();
	 
	#if 1//ndef __SLT_MMI_SMARTDIAL_TOUCHPANEL__  	 
	show_softkey_background();
	show_left_softkey();
	show_right_softkey();
	#endif
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0,0,UI_device_width-1,UI_device_height-1);
}

void refresh_smartdial_search_list(U8 * number)
{
	S32 no_entries = 0;

	no_entries = catSmartDial_search_function(number);
	//kal_prompt_trace(MOD_NVRAM,"xujuntrace smartdial_search_list %d",no_entries);
	load_dynamic_item_buffer(no_entries );

	if(no_entries >=0)
	{
		//refresh_search_list_change_list(no_entries);
		MMI_fixed_list_menu.highlighted_item = 0;
		MMI_fixed_list_menu.n_items = no_entries;
		MMI_fixed_list_menu.displayed_items = no_entries;
		dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
		show_dynamic_list();
		
		gui_BLT_double_buffer(0,0,UI_device_width -1 ,UI_device_height -1);
	}
}
#endif


