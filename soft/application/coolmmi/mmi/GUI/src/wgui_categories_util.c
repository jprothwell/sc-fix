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

/*******************************************************************************
 * Filename:
 * ---------
 *  wgui_categories_util.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  
 * Author:
 * -------
 *  
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

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
#include "wrappergprot.h"
#include "gui_themes.h"
#include "gui_setting.h"
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
#include "wgui_dynamic_menus.h"
#include "wgui_dynamic_menuitems.h"
#include "wgui_asyncdynamic_menus.h"
#include "wgui_asyncdynamic_menuitems.h"
#include "wgui_virtual_keyboard.h"
#include "gdi_include.h"

// #include "callstructuremanagementprot.h"
#include "idleappdef.h"
#include "idleappprot.h"
#include "wallpaperdefs.h"
#include "screensaverdefs.h"
#include "debuginitdef.h"
#include "mainmenudef.h"
#include "gdi_include.h"        /* include for graphic lib */
#include "lcd_sw_rnd.h"
#include "settinggprots.h"
#include "shortcutsdefs.h"
#include "simdetectiongexdcl.h"
#include "phonebooktypes.h"
#include "wgui_draw_manager.h"  /* add for draw manager */
#include "wgui_touch_screen.h"  /* for touch screen apis */

 
#include "mmithemes.h"
 
 
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
#include "mmi_features.h"       /* add for effect type entry for title & highlighter */
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

 
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 11:19 */
#ifdef __MMI_BIDI_ALG__
#include "bididef.h"
#include "bidiprot.h"
#endif
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 11:19 */
#include "ucs2prot.h"
#include "wgui_categories_util.h"
/* CSD end */

#include "wgui_draw_manager.h"

#ifdef __MMI_UI_TAB_PANE__
#include "wgui_tab.h"
#endif 

#include "screenrotationgprot.h"

#include "scrmemmgrgprot.h"
#if defined(__MMI_TOUCH_SCREEN__)
#include "editorpen.h"
#endif
#include "wgui_tab_bars.h"
#ifdef __MMI_MAINLCD_220X176__
#include "calendardef.h"
#define __MMI_TITLE_BAR_NO_ICON_SHCT__

#endif

#if defined(__MMI_NOKIA_STYLE_N800__)&&defined(JATAAYU_SUPPORT)
#include "jwap_str_id.h"
#endif
/* Slim Version Menu Style */
static wgui_list_menu_slim_style_enum wgui_current_list_menu_slim_style = WGUI_LIST_MENU_SLIM_STYLE_DEFAULT;

/* Globals  */
gdi_handle animation_handle = GDI_ERROR_HANDLE;

/*** Store the value of height, width , x, y position of title bar ,status icons
    popup dialog in global variables these valeu r also #define in wgui.h */

/* Bear - 040228 move init value to wgui.c */
S32 MMI_matrix_bar_height;
S32 MMI_matrix_bar_x;
S32 MMI_matrix_bar_y;
S32 MMI_matrix_bar_width;
S32 MMI_title_height;
S32 MMI_title_width;
S32 MMI_title_x;
S32 MMI_title_y;
S32 MMI_button_bar_height;
S32 MMI_menuitem_height;

S32 MMI_pop_up_dialog_x;
S32 MMI_pop_up_dialog_y;
S32 MMI_pop_up_dialog_width;
S32 MMI_pop_up_dialog_height;
S32 MMI_pop_up_dialog_full_height;

U8 wgui_switch_title_with_status_bar = 0;   /* 0: Title, 1: Status icons  */
U8 wgui_category_screen_no_buttons = 0;

/* Store the clipping value of current image */
S32 wgui_image_clip_x1 = 0;
S32 wgui_image_clip_y1 = 0;
S32 wgui_image_clip_x2 = 0;
S32 wgui_image_clip_y2 = 0;

/* Support for displaying animations in Category screens:
   These functions allow the display of a single animated
   icon in any category screen. The position of the icon
   is decided by the category screen                  */
PU8 _MMI_animated_icon = NULL;
U8 *_MMI_animated_icon_name;
S32 _MMI_animated_icon_x;
S32 _MMI_animated_icon_y;
UI_animated_image_handle _MMI_animated_icon_handle = NULL;

UI_pop_up_description wgui_pop_up_description;

/* store number of strigs of pop up */
S32 wgui_pop_up_descriptions_n_strings = 0;

/* store current index of popup */
S32 wgui_current_pop_up_description_index = -1;

/* store number of popup */
S32 wgui_pop_up_descriptions_n_items = 0;

/* store flag of popup */
U8 wgui_pop_up_description_flag = 0;

wgui_pop_up_description_string_type wgui_pop_up_description_strings[WGUI_MAX_POP_UP_DESCRIPTION_STRINGS];

U8 *checklist_category_menu_item_states = NULL;
S32 item_checked = 0;
void (*checkbox_toggle_DONE_RSK_function) (void) = NULL;
void (*checkbox_toggle_BACK_RSK_function) (void) = NULL;

/** Store the title string of current main lcd scrren */
UI_string_type MMI_title_string = NULL;

/** Store the address of title icon display on current main lcd scrren title bar */
PU8 MMI_title_icon = NULL;

/** Animation handle of title icon */
GDI_HANDLE MMI_title_icon_ani_handle = GDI_ERROR_HANDLE;

/** Store the address of title icon display on current main lcd scrren title bar */
PU8 MMI_title_icon2 = NULL;

/* Store the scrolling text of title bar */
scrolling_text scrolling_title_text;

/* Scrolling title support */
/** global variables store value of x,y,width of title bar text , icon to scroll the text on title bar */
S32 scrolling_title_icon_x = 0;
S32 scrolling_title_icon_y = 0;

/* Store the message of current multibox dispaly on main lcd */
UI_string_type MMI_message_string = NULL;
UI_string_type MMI_message_string2 = NULL;

/** Store the address of icon display on content area of current main lcd scrren */
PU8 MMI_message_icon = NULL;
PU8 MMI_message_icon2 = NULL;

/* store teh current shorcut number display on right side of title bar in case of menu screen */
S32 MMI_menu_shortcut_number = -1;

/* Store the text of current highlighted item in menu */
UI_string_type MMI_highlighted_item_text = NULL;

/* high level wrappers  */
PU8 MMI_mm_animation = NULL;

U8 gMMI_UI_U8_flag_1 = 0, gMMI_UI_U8_flag_2 = 0;
U16 gMMI_UI_U16_var_1 = 0, gMMI_UI_U16_var_2 = 0;
S32 gMMI_UI_S32_var_1 = 0, gMMI_UI_S32_var_2 = 0, gMMI_UI_S32_var_3 = 0, gMMI_UI_S32_var_4 = 0;
void *gMMI_UI_ptr_1 = NULL, *gMMI_UI_ptr_2 = NULL, *gMMI_UI_ptr_3 = NULL, *gMMI_UI_ptr_4 = NULL;

/* varbale of type slide_control used in slide control screen */
slide_control *MMI_current_slide_control = NULL;

/* slide_control MMI_slide_control1,MMI_slide_control2; */
slide_control MMI_slide_control[MMI_MAX_SLIDE_CONTROL];

/* store current slide control value */
S32 *MMI_current_slide_control_value = NULL;
S32 *MMI_slide_control_value[MMI_MAX_SLIDE_CONTROL];

MMI_BOOL gMMI_title_bknd_display = MMI_TRUE;

MMI_BOOL gMMI_wallpaper_on_bottom = MMI_FALSE;       
MMI_BOOL gMMI_disable_shortcut_display = MMI_FALSE;  

FuncPtr gExitFullScreenCallBack = NULL;
S32 gOnFullScreen = 0x00000000;

#define MMI_BEFORE_SCREEN                             0x00000000
#define MMI_LEAVE_FULL_SCREEN                         0x00000001
#define MMI_LEAVE_FULL_IDLE_SCREEN                    0x00000002
#define MMI_IN_FULL_SCREEN                            0x00000004
#define MMI_IN_NORMAL_SCREEN                          0x00000008
#define MMI_IN_NORMAL_SCREEN_WITHOUT_STATUSBAR        0x00000010
#define MMI_IN_IDLE_SCREEN                            0x00000020
#define MMI_LEAVE_BG_ON_BOTTOM_SCREEN                 0x00000040         

/* Add for Draw Manager */
S32 g_gui_current_catid = 0;

U16 title_bg_id = 0;     

 
#ifdef __MMI_BIDI_ALG__
extern U8 visual_str[];
#endif 
extern BOOL r2lMMIFlag;

/* CSD end */
extern U16 gMainMenuAnimation[];
extern U16 gMainMenuTitleIcon[];
extern S32 volume_level_UI;

extern bitmap main_LCD_device_bitmap;
extern bitmap sub_LCD_device_bitmap;
extern bitmap *current_LCD_device_bitmap;

/* store Main lcd width */
extern S32 MAIN_LCD_device_width;

/* store Main LCD height */
extern S32 MAIN_LCD_device_height;

/* store the function pointers of keys */
extern FuncPtr currKeyFuncPtrs[MAX_KEYS][MAX_KEY_TYPE];

/* store the ui key function pointers just a copy of currKeyFuncPtrs */
FuncPtr wgui_key_handlers[MAX_KEYS][MAX_KEY_TYPE];

extern void (*hide_softkey[]) (void);
extern icontext_button MMI_softkeys[];

#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
extern BOOL inMatrixNoTitleFlag;     
#endif 
 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
extern void gui_register_button_shuffle(void (*f) (void));
extern void shuffle_draw_white(void);
extern void (*coolsand_shuffle_draw_background) (void);
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
 

 
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__

#define TITLE_EFFECT_TYPE__(A)      MMI_UI_TITLE_IMAGE_ANIMATE_##A
#define TITLE_EFFECT_TYPE_(A)    TITLE_EFFECT_TYPE__(A)
#define TITLE_EFFECT_TYPE(A)     TITLE_EFFECT_TYPE_(A)

wgui_title_effect_styles title_effect_style = TITLE_EFFECT_TYPE(__MMI_TITLE_EFFECT_TYPE__);
S32 animate_title_move = 0;
S32 is_animated_title_effect = 1;
S32 title_animation_current_frame_count = 0;
S32 MMI_draw_title_next_frame = 0;
S32 backup_title_animation_current_frame_count = 0;
#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
 

/***************************************************************************** 
*  Dummy functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  dummy_get_history
 * DESCRIPTION
 *  Dummy function to get history
 * PARAMETERS
 *  buffer      [IN]        History buffer
 * RETURNS
 *  byte*
 *****************************************************************************/
U8 *dummy_get_history(U8 *buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (buffer);
}   /* end of dummy_get_hisgory */


/*****************************************************************************
 * FUNCTION
 *  dummy_get_history_size
 * DESCRIPTION
 *  Dummy function to get history size
 * PARAMETERS
 *  void
 * RETURNS
 *  size of history
 *****************************************************************************/
S32 dummy_get_history_size(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (0);
}   /* end of dummy_get_history_size */

/***************************************************************************** 
*  Get Resource
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  get_string_lang
 * DESCRIPTION
 *  Wrapper for GetStringLang() function, to get string for specific language
 * PARAMETERS
 *  i           [IN]        String id
 *  ssc_str     [IN]        Ssc string to identify language
 * RETURNS
 *  pointer to the string corresponding to the given string ID and give sscstring of language
 *****************************************************************************/
UI_string_type get_string_lang(UI_image_ID_type  i, S8 *ssc_str)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 lang_index;
    UI_string_type s;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    lang_index = GetLanguageIndex(ssc_str);
    if (lang_index == 0xFFFF)
    {
        return NULL;
    }
    else
    {
        s = (UI_string_type) GetStringLang((U16) i, (U16) (lang_index));
    }
    if (s == NULL)
    {
        return NULL;
    }
    if (s[0] == '\0')
    {
        return NULL;
    }
    return ((UI_string_type) s);
}   /* end of get_string_lang */


/*****************************************************************************
 * FUNCTION
 *  get_string
 * DESCRIPTION
 *  Wrapper for GetString() function, to get string for given id
 * PARAMETERS
 *  i       [IN]        String id
 * RETURNS
 *  pointer to the string corresponding to the given string ID
 *****************************************************************************/
UI_string_type get_string(UI_image_ID_type  i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_string_type s = (UI_string_type) GetString((U16) i);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (s == NULL)
    {
        return (NULL);
    }
    if (s[0] == '\0')
    {
        return (NULL);
    }
    return ((UI_string_type) s);
}


/*****************************************************************************
 * FUNCTION
 *  get_image
 * DESCRIPTION
 *  Wrapper for GetImage() function
 * PARAMETERS
 *  i       [IN]        Image id
 * RETURNS
 *  pointer to the image corresponding to the given image ID
 *****************************************************************************/
PU8 get_image(UI_image_ID_type  i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 s;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (i == 0)
    {
        return (NULL);
    }
    s = (PU8) GetImage((U16) i);
    if (s == NULL)
    {
        return (NULL);
    }
    if (s[0] == '\0')
    {
        return (NULL);
    }
    return ((PU8) s);
}

 


/*****************************************************************************
 * FUNCTION
 *  get_audio
 * DESCRIPTION
 *  Wrapper for GetAudio() function
 * PARAMETERS
 *  i           [IN]        Audio id
 *  type        [OUT]       Audio type
 *  filelen     [OUT]       File length
 * RETURNS
 *  pointer to the audio raw data corresponding to the given audio ID
 *****************************************************************************/
U8 *get_audio(UI_image_ID_type  i, U8 *type, U32 *filelen)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *s = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(type != NULL && filelen != NULL);

    *type = 0;;
    *filelen = 0;

    if (i == 0)
    {
        return (NULL);
    }

/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-3-11 17:56 */
    #if 0
    s = (U8*) GetAudio((U16) i);
    #endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-3-11 17:56 */
    if (s == NULL)
    {
        return (NULL);
    }
    if (s[0] == '\0')
    {
        return (NULL);
    }

    *type = s[0];

    *filelen |= (U32) (s[1]);
    *filelen |= (U32) ((U32) s[2] << 8);
    *filelen |= (U32) ((U32) s[3] << 16);
    *filelen |= (U32) ((U32) s[4] << 24);

    return (s + 8);
}

 

/***************************************************************************** 
* Category screen wrappers
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*  MMI_list_highlight_handler
*
* DESCRIPTION
*   Function pointer store the address of function which get executed
*  when any item in list get high lighted
*
* PARAMETERS
*  item _index IN store the current index of item highlighted
* 
* RETURNS
*  void
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void (*MMI_list_highlight_handler) (S32 item_index);

/*****************************************************************************
* FUNCTION
*  MMI_matrix_highlight_handler
*
* DESCRIPTION
*   Function pointer store the address of function which get executed
*  when any item in matrix list get high lighted
*
* PARAMETERS
*  item _index IN store the current index of item highlighted
* 
* RETURNS
*  void
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void (*MMI_matrix_highlight_handler) (S32 item_index);

/*****************************************************************************
* FUNCTION
*  MMI_circular_3D_highlight_handler
*
* DESCRIPTION
*   Function pointer store the address of function which get executed
*  when any item in 3D circular list get high lighted
*
* PARAMETERS
*  item _index IN store the current index of item highlighted
* 
* RETURNS
*  void
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void (*MMI_circular_3D_highlight_handler) (S32 item_index);

/* For the main LCD  */
/*****************************************************************************
* FUNCTION
*  ExitCategoryFunction
*
* DESCRIPTION
*   Function pointer store the address of redraw category function of current
*  main lcd category scrren
*
* PARAMETERS
* 
* RETURNS
*  void
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void (*ExitCategoryFunction) (void) = UI_dummy_function;

/*****************************************************************************
* FUNCTION
*  RedrawCategoryFunction
*
* DESCRIPTION
*   Function pointer store the address of redraw category function of current
*  main lcd category scrren
*
* PARAMETERS
* 
* RETURNS
*  void
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void (*RedrawCategoryFunction) (void) = UI_dummy_function;

/*****************************************************************************
* FUNCTION
*  RedrawCategoryFunction
*
* DESCRIPTION
*   Function pointer store the address of getcategory history function of current
*  main lcd category scrren
*
* PARAMETERS
*  buffer   IN buffer store the valeu of history 
*
* RETURNS
*  U8*                        
*
* GLOBALS AFFECTED
*   buffer 
*****************************************************************************/
U8 *(*GetCategoryHistory) (U8 *buffer) = dummy_get_history;

/*****************************************************************************
* FUNCTION
*  RedrawInputMethodBoxesFunction
*
* DESCRIPTION
*   Function pointer store the address of RedrawInputMethodBoxesFunction
*
* PARAMETERS
* 
* RETURNS
*  void
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void (*RedrawSpellingOrCandidateBoxesFunction) (void) = UI_dummy_function;

/*****************************************************************************
* FUNCTION
*  GetCategoryHistorySize
*
* DESCRIPTION
*   Function pointer store the address of getcategory history size  function of current
*  main lcd category scrren
*
* PARAMETERS
* 
* RETURNS
*  S32   return size of history buffer
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32(*GetCategoryHistorySize) (void) = dummy_get_history_size;

#if (0)
#endif /* (0) */ 


/*****************************************************************************
 * FUNCTION
 *  initialize_category_screens
 * DESCRIPTION
 *  Called once during bootup to initialize category screens
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void initialize_category_screens(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RegisterHighlightHandler(MMI_dummy_highlight_handler);

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;

    SUBLCD_ExitCategoryFunction = MMI_dummy_function;
    SUBLCD_RedrawCategoryFunction = MMI_dummy_function;
    SUBLCD_GetCategoryHistory = dummy_get_history;
    SUBLCD_GetCategoryHistorySize = dummy_get_history_size;
}   /* end of initialize_category_screens */

extern void UpdateUITheme(void);

 


/*****************************************************************************
 * FUNCTION
 *  gui_set_softkey_coordinate
 * DESCRIPTION
 *  Reset the coordinates of the softkeys on screen exit function(Specially to handle Asyn events. As key up is not called at that time.)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
extern U8 is_button_bar_shuffled;

void gui_set_softkey_coordinate(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (is_button_bar_shuffled == 0)
    {
        MMI_right_softkey.y = UI_device_height - MMI_softkey_height;
        MMI_left_softkey.y = UI_device_height - MMI_softkey_height - (MMI_BUTTON_BAR_HEIGHT - MMI_softkey_height);

    }
    else
    {
        MMI_left_softkey.y = UI_device_height - MMI_softkey_height;
        MMI_right_softkey.y = UI_device_height - MMI_softkey_height - (MMI_BUTTON_BAR_HEIGHT - MMI_softkey_height);

    }

}

#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
 


/*****************************************************************************
 * FUNCTION
 *  UI_common_screen_pre_exit
 * DESCRIPTION
 *  Failsafe screen exit function *before* screen exit function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 
 *  Some functions should not be invoked after screen exit function.
 *  (e.g. BLT on layers that are already released.)
 *****************************************************************************/
void UI_common_screen_pre_exit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_TOUCH_SCREEN__
    mmi_pen_reset();
#endif 
     
    gui_pre_cleanup();
     
    if (!dm_is_activated())
    {
        dm_reset_context_ext();
    }
}


/*****************************************************************************
 * FUNCTION
 *  UI_common_screen_exit
 * DESCRIPTION
 *  Failsafe screen exit function *after* screen exit function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void UI_common_screen_exit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ExitCategoryFunction != NULL)
    {
        ExitCategoryFunction();
    }
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_general_pen_handler();
    wgui_reset_list_item_selected_callback();
    wgui_reset_matrix_item_selected_callback();
#endif /* __MMI_TOUCH_SCREEN__ */ 
#ifdef __MMI_TOUCH_SCREEN__
     
    mmi_pen_editor_exit_screen();
#endif /* __MMI_TOUCH_SCREEN__ */ 

    gui_cleanup();

     
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
    if (is_enable_animated_title_effect())
    {
        wgui_stop_animate_title();  /* stop move title */
    }
#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
     
    ClearHighlightHandler();
    gui_hide_animations();
    animation_handle = GDI_ERROR_HANDLE;
    if (MMI_title_icon_ani_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(MMI_title_icon_ani_handle);
        MMI_title_icon_ani_handle = GDI_ERROR_HANDLE;
    }
    StopMyTimer(BLINKING_CURSOR);
#ifdef __MMI_THEMES_APPLICATION__
#ifndef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
    UpdateUITheme();
#endif 
#endif /* __MMI_THEMES_APPLICATION__ */ 
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;

    /* set multilayer mask on/off */
#if defined (__GDI_MEMORY_PROFILE_2__)
    {
    #if defined(__MMI_SUBMENU_USE_MM_IMAGE__)
        gui_set_multilayer_mask(gui_get_multilayer_mask() | UI_MUL_BKG_MAINMENU | UI_MUL_BKG_SUBMENU);
        if (mmi_bootup_is_searching_or_idle_reached() == MMI_FALSE)
        {
            gui_set_multilayer_mask(gui_get_multilayer_mask() & (~UI_MUL_BKG_SUBMENU));
        }
        else
        {
            gui_set_multilayer_mask(gui_get_multilayer_mask() | UI_MUL_BKG_SUBMENU);
        }
    #elif defined(__MMI_UI_LIST_CACHE_BACKGROUND__)
        gui_set_multilayer_mask(gui_get_multilayer_mask() | UI_MUL_BKG_SUBMENU);
    #else 
        gui_set_multilayer_mask(gui_get_multilayer_mask() & (~UI_MUL_BKG_MAINMENU) & (~UI_MUL_BKG_SUBMENU));
    #endif 
    }
#endif /* defined (__GDI_MEMORY_PROFILE_2__) */ 

    /* free all layer to be base layer */
    gui_free_multilayer_screen();
    gui_set_entry_new_screen(MMI_TRUE);
     
    if (wgui_is_wallpaper_on_bottom())
    {
        gOnFullScreen = MMI_LEAVE_BG_ON_BOTTOM_SCREEN;
        wgui_reset_wallpaper_on_bottom();    
    }
     
    gdi_image_gif_reset();
    gui_reset_clip();
    gui_reset_text_clip();
    reset_softkeys();
/*+ zhouqin 2011-02-16 modify for q6 320X240 */     
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)|| defined (__MMI_MAINLCD_320X240__)    
/*- zhouqin 2011-02-16 modify for q6 320X240 */
 if (gExitFullScreenCallBack != NULL)
    {
        gExitFullScreenCallBack();
        gExitFullScreenCallBack = NULL;
    }
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) */ 
     
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    is_button_bar_shuffled = 1;
    gui_set_softkey_coordinate();   /* Specially for Asyn events */
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
     

     
#ifdef __MMI_UI_TAB_PANE__
    ThemeAlarmCallBackUpdateUITheme();
#endif 
     

#ifdef __MMI_SCREEN_ROTATE__
    mmi_frm_reset_screen_rotation();
#endif 

    MMI_ASSERT(mmi_frm_scrmem_is_all_free());
}   /* end of UI_common_screen_exit */


/*****************************************************************************
 * FUNCTION
 *  reset_category_functions
 * DESCRIPTION
 *  Calls the reset functions of all MMI objects and category screens
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_category_functions(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    reset_fixed_matrix();
    reset_multitaps();
    reset_singleline_inputbox();
    reset_multiline_inputbox();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
}   /* end of reset_category_functions */

U32 category_screen_layout_flags;


/*****************************************************************************
 * FUNCTION
 *  SetCategoryScreenLayout
 * DESCRIPTION
 *  set the layout of category screen
 * PARAMETERS
 *  flags       [IN]        Vaule to set flags
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategoryScreenLayout(U32 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    category_screen_layout_flags = flags;
}   /* end of SetCategoryScreenLayout */

/***************************************************************************** 
* Start : Animation control
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  wgui_show_animation
 * DESCRIPTION
 *  Displays an animated icon that was set previously. If GIF library of
 *  pluto is enabled then it calls pluto animation function
 *  otherwise coolsand animation function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_animation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gdi_handle lcd_h;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_lcd_get_active(&lcd_h);

    if (lcd_h == GDI_LCD_MAIN_LCD_HANDLE)
    {
        gdi_image_draw_animation(_MMI_animated_icon_x, _MMI_animated_icon_y, _MMI_animated_icon, NULL);
    }
    else if (lcd_h == GDI_LCD_SUB_LCD_HANDLE)
    {
        gdi_image_draw_animation(
            SUBLCD_MMI_animated_icon_x,
            SUBLCD_MMI_animated_icon_y,
            SUBLCD_MMI_animated_icon,
            NULL);
    }
}   /* end of wgui_show_animation */


/*****************************************************************************
 * FUNCTION
 *  wgui_show_transparent_animation
 * DESCRIPTION
 *  Displays an animated icon that was set previously, with transparency
 *  If GIF library of pluto is enabled then it calls pluto animation function
 *  otherwise coolsand animation function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_transparent_animation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gdi_handle lcd_h;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_lcd_get_active(&lcd_h);

    if (lcd_h == GDI_LCD_MAIN_LCD_HANDLE)
    {
        gdi_image_draw_animation(_MMI_animated_icon_x, _MMI_animated_icon_y, _MMI_animated_icon, NULL);
    }
    else if (lcd_h == GDI_LCD_SUB_LCD_HANDLE)
    {
        gdi_image_draw_animation(
            SUBLCD_MMI_animated_icon_x,
            SUBLCD_MMI_animated_icon_y,
            SUBLCD_MMI_animated_icon,
            NULL);
    }
}   /* end of wgui_show_transparent_animation */


/*****************************************************************************
 * FUNCTION
 *  wgui_set_animation
 * DESCRIPTION
 *  Sets up the animated icon to be displayed in the category screens
 * PARAMETERS
 *  x           [IN]        X-coordinate of animation
 *  y           [IN]        Y-coordinate of animation
 *  img         [IN]        
 *  imag(?)     [IN]        Id of animation
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_animation(S32 x, S32 y, UI_image_ID_type  img)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_set_animation_image(x, y, get_image(img));
}   /* end of wgui_set_animation */


/*****************************************************************************
 * FUNCTION
 *  wgui_set_animation_y
 * DESCRIPTION
 *  Sets up the animated icon to be displayed in the category screens
 * PARAMETERS
 *  x           [IN]        Coordinates around which the icon is centered horizontally
 *  y           [IN]        Position from which the top of the icon is drawn
 *  img         [IN]        
 *  imag(?)     [IN]        Id of animation
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_animation_y(S32 x, S32 y, UI_image_ID_type  img)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_set_animation_image_y(x, y, get_image(img));
}   /* end of wgui_set_animation_y */


/*****************************************************************************
 * FUNCTION
 *  wgui_set_animation_image
 * DESCRIPTION
 *  Sets up the animated icon to be displayed in the category screens
 * PARAMETERS
 *  x           [IN]        Coordinates around which the icon is centered horizontally
 *  y           [IN]        Position from which of the icon is drawn centered.
 *  img         [IN]        
 *  imag(?)     [IN]        The icon to be displayed
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_animation_image(S32 x, S32 y, PU8 img)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 width, height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (UI_test_sub_LCD_graphics_context())
    {
        SUBLCD_MMI_animated_icon = img;
        if (SUBLCD_MMI_animated_icon == NULL)
        {
            return;
        }
        gui_measure_image(SUBLCD_MMI_animated_icon, &width, &height);
        SUBLCD_MMI_animated_icon_x = x - (width >> 1);
        SUBLCD_MMI_animated_icon_y = y - (height >> 1);
    }
    else
    {
        _MMI_animated_icon = img;
        if (_MMI_animated_icon == NULL)
        {
            return;
        }
        gui_measure_image(_MMI_animated_icon, &width, &height);
        _MMI_animated_icon_x = x - (width >> 1);
        _MMI_animated_icon_y = y - (height >> 1);
    }
}   /* end of wgui_set_animation_image */


/*****************************************************************************
 * FUNCTION
 *  wgui_set_animation_image_y
 * DESCRIPTION
 *  Sets up the animated icon to be displayed in the category screens
 * PARAMETERS
 *  x           [IN]        Coordinates around which the icon is centered horizontally
 *  y           [IN]        Position from which the top of the icon is drawn
 *  img         [IN]        
 *  imag(?)     [IN]        The icon to be displayed
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_animation_image_y(S32 x, S32 y, PU8 img)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 width, height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (UI_test_sub_LCD_graphics_context())
    {
        SUBLCD_MMI_animated_icon = img;
        if (SUBLCD_MMI_animated_icon == NULL)
        {
            return;
        }
        gui_measure_image(SUBLCD_MMI_animated_icon, &width, &height);
        SUBLCD_MMI_animated_icon_x = x - (width >> 1);
        SUBLCD_MMI_animated_icon_y = y;

    }
    else
    {
        _MMI_animated_icon = img;
        if (_MMI_animated_icon == NULL)
        {
            return;
        }
        gui_measure_image(_MMI_animated_icon, &width, &height);
        _MMI_animated_icon_x = x - (width >> 1);
        _MMI_animated_icon_y = y;
    }
}   /* end of wgui_set_animation_image_y */


/*****************************************************************************
 * FUNCTION
 *  wgui_measure_set_animation
 * DESCRIPTION
 *  Sets up the animated icon to be displayed in the category screens
 * PARAMETERS
 *  x           [IN]        Coordinates around which the icon is centered horizontally
 *  y           [IN]        Position from which the icon is drawn centered vertically.
 *  width       [?]         
 *  height      [?]         
 *  img         [IN]        
 *  imag(?)     [IN]        The id of icon to be displayed
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_measure_set_animation(S32 x, S32 y, S32 *width, S32 *height, UI_image_ID_type  img)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_measure_set_animation_image(x, y, width, height, get_image(img));
}


/*****************************************************************************
 * FUNCTION
 *  wgui_measure_set_animation_y
 * DESCRIPTION
 *  Sets up the animated icon to be displayed in the category screens
 * PARAMETERS
 *  x           [IN]        Coordinates around which the icon is centered horizontally
 *  y           [IN]        Position from which the top of the icon is drawn
 *  width       [?]         
 *  height      [?]         
 *  img         [IN]        
 *  imag(?)     [IN]        The id of icon to be displayed
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_measure_set_animation_y(S32 x, S32 y, S32 *width, S32 *height, UI_image_ID_type  img)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_measure_set_animation_image_y(x, y, width, height, get_image(img));
}


/*****************************************************************************
 * FUNCTION
 *  wgui_measure_set_animation_image
 * DESCRIPTION
 *  Sets up the animated icon to be displayed in the category screens
 * PARAMETERS
 *  x           [IN]        Coordinates around which the icon is centered horizontally
 *  y           [IN]        Position from which the icon is drawn centered vertically.
 *  width       [?]         
 *  height      [?]         
 *  img         [IN]        
 *  imag(?)     [IN]        The icon to be displayed
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_measure_set_animation_image(S32 x, S32 y, S32 *width, S32 *height, PU8 img)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *width = 0;
    *height = 0;

    if (UI_test_sub_LCD_graphics_context())
    {
        SUBLCD_MMI_animated_icon = img;
        if (SUBLCD_MMI_animated_icon == NULL)
        {
            return;
        }
        gui_measure_image(SUBLCD_MMI_animated_icon, width, height);
        SUBLCD_MMI_animated_icon_x = x - ((*width) >> 1);
        SUBLCD_MMI_animated_icon_y = y - ((*height) >> 1);
    }
    else
    {
        _MMI_animated_icon = img;
        if (_MMI_animated_icon == NULL)
        {
            return;
        }
        gui_measure_image(_MMI_animated_icon, width, height);
        _MMI_animated_icon_x = x - ((*width) >> 1);
        _MMI_animated_icon_y = y - ((*height) >> 1);
    }
}   /* end of wgui_measure_set_animation_image */


/*****************************************************************************
 * FUNCTION
 *  wgui_measure_set_animation_image_y
 * DESCRIPTION
 *  Sets up the animated icon to be displayed in the category screens
 * PARAMETERS
 *  x           [IN]        Coordinates around which the icon is centered horizontally
 *  y           [IN]        Position from which the top of the icon is drawn
 *  width       [?]         
 *  height      [?]         
 *  img         [IN]        
 *  imag(?)     [IN]        The icon to be displayed
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_measure_set_animation_image_y(S32 x, S32 y, S32 *width, S32 *height, PU8 img)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *width = 0;
    *height = 0;
    /* check whether the current graphic context is sub lcd */
    if (UI_test_sub_LCD_graphics_context())
    {
        SUBLCD_MMI_animated_icon = img;
        if (SUBLCD_MMI_animated_icon == NULL)
        {
            return;
        }
        gui_measure_image(SUBLCD_MMI_animated_icon, width, height);
        SUBLCD_MMI_animated_icon_x = x - ((*width) >> 1);
        SUBLCD_MMI_animated_icon_y = y;
    }
    else
    {
        _MMI_animated_icon = img;
        if (_MMI_animated_icon == NULL)
        {
            return;
        }
        gui_measure_image(_MMI_animated_icon, width, height);
        _MMI_animated_icon_x = x - ((*width) >> 1);
        _MMI_animated_icon_y = y;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_hide_animation
 * DESCRIPTION
 *  Hides the Category screen animation. Stop the animation
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_hide_animation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (UI_test_sub_LCD_graphics_context())
    {
        gui_hide_animated_image(0, 0, SUBLCD_MMI_animated_icon_handle);
    }
    else
    {
        gui_hide_animated_image(0, 0, _MMI_animated_icon_handle);
    }
}

/***************************************************************************** 
* Common Utility
*****************************************************************************/
#if (0)
#endif /* (0) */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_fill_rectangle_clip
 * DESCRIPTION
 *  set clip and draw rectangle
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 *  c       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_fill_rectangle_clip(S32 x1, S32 y1, S32 x2, S32 y2, color_t c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(x1, y1, x2, y2);
    gui_fill_rectangle(x1, y1, x2, y2, c);
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  clear_screen_with_color
 * DESCRIPTION
 *  Clears the screen with specific color
 * PARAMETERS
 *  c       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void clear_screen_with_color(color_t c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) || defined (__MMI_MAINLCD_320X240__)  
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
    if ((gOnFullScreen & MMI_IN_NORMAL_SCREEN)) /* current screen is not full screen */
    {
        gui_set_clip(0, MMI_title_y, UI_device_width - 1, UI_device_height - 1);
        gui_fill_rectangle(0, MMI_title_y, UI_device_width - 1, UI_device_height - 1, c);
    }
    else
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) */ 
    {
        gui_set_clip(0, 0, UI_device_width - 1, UI_device_height - 1);
        gui_fill_rectangle(0, 0, UI_device_width - 1, UI_device_height - 1, c);
    }
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  clear_screen
 * DESCRIPTION
 *  Clears the screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_screen(void)
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
    clear_screen_with_color(c);
}


/*****************************************************************************
 * FUNCTION
 *  clear_buttonbar
 * DESCRIPTION
 *  Clears the button bar portion of the screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_buttonbar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = gui_color(255, 255, 255);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_reset_clip();
    if (UI_device_height == 128)
    {
        gui_fill_rectangle(0, UI_device_height - MMI_softkey_height - 10, UI_device_width - 1, UI_device_height - 1, c);
    }
    else
 
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
  
    if (whether_no_small_screen())
   
    {
        gui_fill_rectangle(0, UI_device_height - MMI_softkey_height, UI_device_width - 1, UI_device_height - 1, c);      
    }
    else
#endif /* __MMI_UI_SMALL_SCREEN_SUPPORT__ */ 
 
        gui_fill_rectangle(0, UI_device_height - MMI_softkey_height - 4, UI_device_width - 1, UI_device_height - 1, c);
}


/*****************************************************************************
 * FUNCTION
 *  set_button_bar_height
 * DESCRIPTION
 *  This function is used to set the button bar height
 * PARAMETERS
 *  height                      [IN]            
 *  UICtrlAccessPtr_p(?)        [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_image_info_struct I  N/OUT image area coordinate info.(?)
 * RETURNS
 *  void
 *****************************************************************************/
void set_button_bar_height(S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_button_bar_height = height;
}


/*****************************************************************************
 * FUNCTION
 *  clear_contentarea
 * DESCRIPTION
 *  Clears the content area.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_contentarea(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = gui_color(255, 255, 255);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_reset_clip();
    gui_fill_rectangle(0, (MMI_title_y + MMI_title_height), UI_device_width, UI_device_height - MMI_softkey_height, c);
}


/*****************************************************************************
 * FUNCTION
 *  draw_arrow_indication
 * DESCRIPTION
 *  Displays the arror key indication image
 * PARAMETERS
 *  left        [IN]        1:show, 0: hide
 *  right       [IN]        1:show, 0: hide
 *  up          [IN]        1:show, 0: hide
 *  down        [IN]        1:show, 0: hide
 * RETURNS
 *  void
 *****************************************************************************/
#define WGUI_COLOR_TO_GDI_FORMAT(c) gdi_act_color_from_rgb(255,c.r, c.g, c.b)
void draw_arrow_indication(S8 left, S8 right, S8 up, S8 down)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__
     
    S32 center_x = 0, center_y = 0;
    S32 area_width = 20, area_height = MMI_button_bar_height - 2;
    S32 arrow_height = 5;
	
#ifdef __MMI_MAINLCD_220X176__
	color_t content = {255,102,0,0};
#else
    color_t content = current_MMI_theme->list_selected_item_filler->c;
#endif
    color_t line = {0, 0, 0, 0};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (left == 0 && right == 0 && up == 0 && down == 0)
    {
        return;
    }
    center_x = UI_device_width >> 1;
    center_y = UI_device_height - (MMI_button_bar_height >> 1);

     
    gui_push_clip();
    gui_set_clip(0, UI_device_height - MMI_button_bar_height, UI_device_width, UI_device_height);
     

    if (up >= 1)
    {
        gdi_draw_line(
            center_x - 3,
            center_y - ((area_height >> 1) - arrow_height),
            center_x + 3,
            center_y - ((area_height >> 1) - arrow_height),
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x - 3,
            center_y - ((area_height >> 1) - arrow_height) - 1,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x + 3,
            center_y - ((area_height >> 1) - arrow_height) - 1,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x - 2,
            center_y - ((area_height >> 1) - arrow_height) - 2,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x + 2,
            center_y - ((area_height >> 1) - arrow_height) - 2,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x - 1,
            center_y - ((area_height >> 1) - arrow_height) - 3,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x + 1,
            center_y - ((area_height >> 1) - arrow_height) - 3,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x, center_y - ((area_height >> 1) - arrow_height) - 4, WGUI_COLOR_TO_GDI_FORMAT(line));

        gdi_draw_point(
            center_x - 2,
            center_y - ((area_height >> 1) - arrow_height) - 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x - 1,
            center_y - ((area_height >> 1) - arrow_height) - 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x, center_y - ((area_height >> 1) - arrow_height) - 1, WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + 1,
            center_y - ((area_height >> 1) - arrow_height) - 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + 2,
            center_y - ((area_height >> 1) - arrow_height) - 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x - 1,
            center_y - ((area_height >> 1) - arrow_height) - 2,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x, center_y - ((area_height >> 1) - arrow_height) - 2, WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + 1,
            center_y - ((area_height >> 1) - arrow_height) - 2,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x, center_y - ((area_height >> 1) - arrow_height) - 3, WGUI_COLOR_TO_GDI_FORMAT(content));
    }
    if (down >= 1)
    {
        gdi_draw_line(
            center_x - 3,
            center_y + ((area_height >> 1) - arrow_height),
            center_x + 3,
            center_y + ((area_height >> 1) - arrow_height),
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x - 3,
            center_y + ((area_height >> 1) - arrow_height) + 1,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x + 3,
            center_y + ((area_height >> 1) - arrow_height) + 1,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x - 2,
            center_y + ((area_height >> 1) - arrow_height) + 2,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x + 2,
            center_y + ((area_height >> 1) - arrow_height) + 2,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x - 1,
            center_y + ((area_height >> 1) - arrow_height) + 3,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(
            center_x + 1,
            center_y + ((area_height >> 1) - arrow_height) + 3,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x, center_y + ((area_height >> 1) - arrow_height) + 4, WGUI_COLOR_TO_GDI_FORMAT(line));

        gdi_draw_point(
            center_x - 2,
            center_y + ((area_height >> 1) - arrow_height) + 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x - 1,
            center_y + ((area_height >> 1) - arrow_height) + 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x, center_y + ((area_height >> 1) - arrow_height) + 1, WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + 1,
            center_y + ((area_height >> 1) - arrow_height) + 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + 2,
            center_y + ((area_height >> 1) - arrow_height) + 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x - 1,
            center_y + ((area_height >> 1) - arrow_height) + 2,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x, center_y + ((area_height >> 1) - arrow_height) + 2, WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + 1,
            center_y + ((area_height >> 1) - arrow_height) + 2,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x, center_y + ((area_height >> 1) - arrow_height) + 3, WGUI_COLOR_TO_GDI_FORMAT(content));
    }
    if (left >= 1)
    {
        gdi_draw_line(
            center_x - ((area_width >> 1) - arrow_height),
            center_y - 3,
            center_x - ((area_width >> 1) - arrow_height),
            center_y + 3,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 1, center_y - 3, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 1, center_y + 3, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 2, center_y - 2, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 2, center_y + 2, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 3, center_y - 1, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 3, center_y + 1, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 4, center_y, WGUI_COLOR_TO_GDI_FORMAT(line));

        gdi_draw_point(
            center_x - ((area_width >> 1) - arrow_height) - 1,
            center_y - 2,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x - ((area_width >> 1) - arrow_height) - 1,
            center_y - 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 1, center_y, WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x - ((area_width >> 1) - arrow_height) - 1,
            center_y + 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x - ((area_width >> 1) - arrow_height) - 1,
            center_y + 2,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x - ((area_width >> 1) - arrow_height) - 2,
            center_y - 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 2, center_y, WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x - ((area_width >> 1) - arrow_height) - 2,
            center_y + 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x - ((area_width >> 1) - arrow_height) - 3, center_y, WGUI_COLOR_TO_GDI_FORMAT(content));

    }
    if (right >= 1)
    {
        gdi_draw_line(
            center_x + ((area_width >> 1) - arrow_height),
            center_y - 3,
            center_x + ((area_width >> 1) - arrow_height),
            center_y + 3,
            WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 1, center_y - 3, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 1, center_y + 3, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 2, center_y - 2, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 2, center_y + 2, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 3, center_y - 1, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 3, center_y + 1, WGUI_COLOR_TO_GDI_FORMAT(line));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 4, center_y, WGUI_COLOR_TO_GDI_FORMAT(line));

        gdi_draw_point(
            center_x + ((area_width >> 1) - arrow_height) + 1,
            center_y - 2,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + ((area_width >> 1) - arrow_height) + 1,
            center_y - 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 1, center_y, WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + ((area_width >> 1) - arrow_height) + 1,
            center_y + 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + ((area_width >> 1) - arrow_height) + 1,
            center_y + 2,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + ((area_width >> 1) - arrow_height) + 2,
            center_y - 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 2, center_y, WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(
            center_x + ((area_width >> 1) - arrow_height) + 2,
            center_y + 1,
            WGUI_COLOR_TO_GDI_FORMAT(content));
        gdi_draw_point(center_x + ((area_width >> 1) - arrow_height) + 3, center_y, WGUI_COLOR_TO_GDI_FORMAT(content));

    }

    gui_pop_clip();  
     
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
     
}

#if(ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)
extern void wgui_main_menu_goto_item(S32 index);
#endif 


/*****************************************************************************
 * FUNCTION
 *  SetHighlightedItem
 * DESCRIPTION
 *  Highlights a given item in a menu screen
 * PARAMETERS
 *  item_index      [IN]        Index (zero based) of the item to be highlighted
 * RETURNS
 *  void
 *****************************************************************************/
void SetHighlightedItem(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (MMI_current_menu_type)
    {
        case PAGE_MENU:
        case LIST_MENU:
        case ANI_BG_LIST_MENU:
            fixed_list_goto_item(item_index);
            break;

        case FIXED_GRID_MENU:
    #if(ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)
            wgui_main_menu_goto_item(item_index);
            break;
    #endif /* (ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION) */ 

        case MATRIX_MENU:
            fixed_matrix_goto_item(item_index);
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  GetHighlightedItem
 * DESCRIPTION
 *  Returns the index of the highlighted item
 * PARAMETERS
 *  void
 * RETURNS
 *  Returns the index of the highlighted item
 *****************************************************************************/
S32 GetHighlightedItem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (MMI_current_menu_type)
    {
        case PAGE_MENU:
        case LIST_MENU:
        case ANI_BG_LIST_MENU:
            return (MMI_fixed_list_menu.highlighted_item);

        case FIXED_GRID_MENU:
    #if(ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)
            return (_wgui_main_menu.highlighted_item);
    #endif 
        case MATRIX_MENU:
            return (MMI_fixed_matrix_menu.highlighted_item);
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  get_item_text
 * DESCRIPTION
 *  Gets the text of the item at the given index in the currently displayed menu
 * PARAMETERS
 *  index       [IN]        (zero based) index of the item whose text needs to be retrieved
 * RETURNS
 *  a pointer to the text of the specified item
 *****************************************************************************/
UI_string_type get_item_text(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (MMI_current_menu_type)
    {
        case LIST_MENU:
        case PAGE_MENU:
        case ANI_BG_LIST_MENU:
            if (index >= 0 && index < MMI_fixed_list_menu.n_items)
            {
                if (MMI_fixed_list_menu.common_item_data == (void*)&MMI_fixed_text_menuitem)
                {
                    return (MMI_fixed_text_menuitems[index].text);
                }
                else if (MMI_fixed_list_menu.common_item_data == (void*)&MMI_fixed_icontext_menuitem)
                {
                    return (MMI_fixed_icontext_menuitems[index].item_text);
                }
                else if (MMI_fixed_list_menu.common_item_data == (void*)&MMI_fixed_icontext_list_menuitem)
                {
                    return (MMI_fixed_icontext_list_menuitems[index].item_texts[0]);
                }
            }
            break;

        case FIXED_GRID_MENU:
        #if(ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)
            if (index >= 0 && index < _wgui_main_menu.n_items)
            {
                return (_wgui_main_menu_item_strings[index]);
            }
            break;
        #endif /* (ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION) */ 
        case MATRIX_MENU:
        case MATRIX_MENU_EMS:
            if (index >= 0 && index < MMI_fixed_matrix_menu.n_items)
            {
                if (MMI_fixed_matrix_menu.common_item_data == (void*)&MMI_fixed_text_menuitem)
                {
                    return (MMI_fixed_text_menuitems[index].text);
                }
                else if (MMI_fixed_matrix_menu.common_item_data == (void*)&MMI_fixed_icontext_menuitem)
                {
                    return (MMI_fixed_icontext_menuitems[index].item_text);
                }
                else if (MMI_fixed_matrix_menu.common_item_data == (void*)&MMI_fixed_icontext_list_menuitem)
                {
                    return (MMI_fixed_icontext_list_menuitems[index].item_texts[0]);
                }
            }
            break;

        #if(ENABLE_CIRCULAR_3D_MENU)
        case CIRCULAR_3D_MENU:
            if (index >= 0 && index < wgui_CM3D_n_items)
            {
                return (wgui_CM3D_items[index]);
            }
            break;
        #endif /* (ENABLE_CIRCULAR_3D_MENU) */ 
        #if(ENABLE_ROTATE_MENU)  
        case ROTATE_MENU:
            if (index >= 0 && index < wgui_CM3D_n_items)
            {
                return (wgui_CM3D_items[index]);
            }
            break;
        #endif /* (ENABLE_ROTATE_MENU) */ 
    }
    return (NULL);
}


/*****************************************************************************
 * FUNCTION
 *  get_dynamic_item_text
 * DESCRIPTION
 *  get dynamic item text
 * PARAMETERS
 *  index       [IN]        Index of list
 * RETURNS
 *  UI_string_type
 *****************************************************************************/
UI_string_type get_dynamic_item_text(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_icontext_menuitem_type *item_p = get_dynamic_item_from_buffer(index);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (item_p != NULL)
    {
        return item_p->item_text;
    }

    return NULL;
}

#ifdef ENABLE_BOTTOM_NAVIGATION_KEYS
//to enable the navigation keys on the soft key bar

/*****************************************************************************
 * FUNCTION
 *  gui_show_key_navigation
 * DESCRIPTION
 *  show key navigation
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_key_navigation(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 navi_key_w, navi_key_h;
    S32 navi_key_x, navi_key_y;
    S32 height_img;
    PU8 img;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    height_img = UI_device_height - MMI_button_bar_height;
#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_lock_double_buffer();
#endif 

    /* show up arrow or not */
    if (index & 1)
    {
        img = (U8*) (GetImage(UIELEMENT_ARROW_UP));
        gui_measure_image(img, &navi_key_w, &navi_key_h);
        navi_key_x = (UI_device_width >> 1) - (navi_key_w >> 1);
        navi_key_y = height_img;
        gui_show_transparent_image(navi_key_x, navi_key_y, img, gui_transparent_color(0, 0, 0));
    }

    /* show down arrow or not */
    if (index & 2)
    {
        img = (U8*) (GetImage(UIELEMENT_ARROW_DOWN));
        gui_measure_image(img, &navi_key_w, &navi_key_h);
        navi_key_x = (UI_device_width >> 1) - (navi_key_w >> 1);
        navi_key_y = height_img + 12;
        /* navi_key_y= height_img + navi_key_h1+navi_key_h1((MMI_button_bar_height >>1) - (navi_key_h>>1)); */
        gui_show_transparent_image(navi_key_x, navi_key_y, img, gui_transparent_color(0, 0, 0));
    }
    /* show left arrow or not */
    if (index & 4)
    {
        img = (U8*) (GetImage(UIELEMENT_ARROW_LEFT));
        gui_measure_image(img, &navi_key_w, &navi_key_h);
        navi_key_x = (UI_device_width >> 1) - (navi_key_w >> 1) - 6;
        navi_key_y = height_img + 6;
        gui_show_transparent_image(navi_key_x, navi_key_y, img, gui_transparent_color(0, 0, 0));
        /* img =GetImage(UIELEMENT_ARROW_LEFT); */

    }
    /* show right arrow or not */
    if (index & 8)
    {
        img = (U8*) (GetImage(UIELEMENT_ARROW_RIGHT));
        gui_measure_image(img, &navi_key_w, &navi_key_h);
        navi_key_x = (UI_device_width >> 1) - (navi_key_w >> 1) + 6;
        navi_key_y = height_img + 6;
        gui_show_transparent_image(navi_key_x, navi_key_y, img, gui_transparent_color(0, 0, 0));
        gui_BLT_double_buffer(navi_key_x, navi_key_y, navi_key_x + navi_key_h - 1, navi_key_y + navi_key_h - 1);
    }

    /* gui_show_transparent_image(navi_key_x,navi_key_y, img, gui_transparent_color(0,0,0)); */
#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(MMI_SOFTKEY_WIDTH, height_img, MMI_SOFTKEY_WIDTH + 28, height_img + 17);
#endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 

}


/*****************************************************************************
 * FUNCTION
 *  gui_show_navigation_keys
 * DESCRIPTION
 *  show navigation keys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_navigation_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 keys_navigation = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((currKeyFuncPtrs[KEY_UP_ARROW][KEY_EVENT_DOWN] != 0) || (currKeyFuncPtrs[KEY_UP_ARROW][KEY_EVENT_UP] != 0))
    {
        keys_navigation += SHOW_ARROW_UP;
    }
    /* gui_show_key_navigation(SHOW_ARROW_UP); */
    if ((currKeyFuncPtrs[KEY_DOWN_ARROW][KEY_EVENT_DOWN] != 0) || (currKeyFuncPtrs[KEY_DOWN_ARROW][KEY_EVENT_UP] != 0))
    {
        keys_navigation += SHOW_ARROW_DOWN;
    }
    /* gui_show_key_navigation(SHOW_ARROW_DOWN); */
    if ((currKeyFuncPtrs[KEY_LEFT_ARROW][KEY_EVENT_DOWN] != 0) || (currKeyFuncPtrs[KEY_LEFT_ARROW][KEY_EVENT_UP] != 0))
    {
        keys_navigation += SHOW_ARROW_LEFT;
    }

    if ((currKeyFuncPtrs[KEY_RIGHT_ARROW][KEY_EVENT_DOWN] != 0) ||
        (currKeyFuncPtrs[KEY_RIGHT_ARROW][KEY_EVENT_UP] != 0))
    {
        keys_navigation += SHOW_ARROW_RIGHT;
    }
    /* gui_show_key_navigation(SHOW_ARROW_RIGHT); */

    gui_show_key_navigation(keys_navigation);
}
#endif /* ENABLE_BOTTOM_NAVIGATION_KEYS */ 


/*****************************************************************************
 * FUNCTION
 *  DisableCategoryScreenScrollBar
 * DESCRIPTION
 *  Disable showing scroll bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisableCategoryScreenScrollBar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flag_scroll = TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  SetupCategoryKeyHandlers
 * DESCRIPTION
 *  Common functions to clear all key handlers and setup default softkey handlers
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SetupCategoryKeyHandlers(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();
#ifdef __MMI_WGUI_CSK_ENABLE__
    clear_softkey_display_functions(MMI_CENTER_SOFTKEY);
#endif 
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();
}

/***************************************************************************** 
* UI_STYLE_3 Full Screen Control Related Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  set_exit_full_screen_call_back
 * DESCRIPTION
 *  set callback function when exit full screen
 * PARAMETERS
 *  f       [IN]        Callback function
 * RETURNS
 *  void
 *****************************************************************************/
void set_exit_full_screen_call_back(FuncPtr f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gExitFullScreenCallBack = f;
}


/*****************************************************************************
 * FUNCTION
 *  entry_full_screen
 * DESCRIPTION
 *  turn on gOnFullScreen flag, to redraw status icons
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void entry_full_screen(void)
{
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) ||defined(__MMI_MAINLCD_320X240__)        
/*- zhouqin 2011-02-16 modify for q6 320X240 */

/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    hide_status_icon_bar(0);
    hide_status_icon_bar(1);
    gOnFullScreen = MMI_IN_FULL_SCREEN;
    set_exit_full_screen_call_back(leave_full_screen);
    dm_set_scr_bg_redraw(MMI_TRUE);
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  leave_full_screen
 * DESCRIPTION
 *  turn on gIsRedrawStatusIcons flag, to redraw status icons
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void leave_full_screen(void)
{
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) ||defined(__MMI_MAINLCD_320X240__)       
/*- zhouqin 2011-02-16 modify for q6 320X240 */

/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_status_icon_bar(0);
    gOnFullScreen |= MMI_LEAVE_FULL_SCREEN;
    gOnFullScreen &= ~MMI_IN_FULL_SCREEN;
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) */ 
}
void show_hide_status_icons_bar0(void);


/*****************************************************************************
 * FUNCTION
 *  in_idle_screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void in_idle_screen(void)
{
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_128X160__) ||defined(__MMI_MAINLCD_160X128__)||defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)||defined (__MMI_MAINLCD_128X128__) || defined (__MMI_MAINLCD_320X240__)  
/*- zhouqin 2011-02-16 modify for q6 320X240 */

/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gOnFullScreen = MMI_IN_IDLE_SCREEN;
    show_status_icon_bar(0);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#ifdef __MMI_SYMBIAN_STYLE_IDLE__
	hide_status_icon_bar(1);
#else
	show_status_icon_bar(1);
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    dm_set_scr_bg_redraw(MMI_TRUE);
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
}


/*****************************************************************************
 * FUNCTION
 *  leave_idle_screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void leave_idle_screen(void)
{
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_128X160__)||defined(__MMI_MAINLCD_160X128__)||defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)||defined (__MMI_MAINLCD_128X128__) ||defined (__MMI_MAINLCD_320X240__) 
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    hide_status_icon_bar(1);
#ifdef __MMI_IDLE_FULL_SCREEN__
    gOnFullScreen |= MMI_LEAVE_FULL_IDLE_SCREEN;
    gOnFullScreen |= MMI_LEAVE_FULL_SCREEN;
    gOnFullScreen &= ~MMI_IN_IDLE_SCREEN;
#endif /* __MMI_IDLE_FULL_SCREEN__ */ 
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
}


/*****************************************************************************
 * FUNCTION
 *  if_full_screen_hide_status_icons
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL if_full_screen_hide_status_icons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    if (gOnFullScreen == MMI_IN_NORMAL_SCREEN_WITHOUT_STATUSBAR)
    {
        return MMI_TRUE;
    }
    if (gOnFullScreen == MMI_IN_IDLE_SCREEN)
    {
        return MMI_FALSE;
    }
    /* just leave from full screen (except full idlescreen ) */
    if (gOnFullScreen & MMI_LEAVE_FULL_SCREEN && !(gOnFullScreen & MMI_LEAVE_FULL_IDLE_SCREEN))
    {
        return MMI_TRUE;
    }
    /* now in full screen */
    if (gOnFullScreen & MMI_IN_FULL_SCREEN)
    {
        return MMI_TRUE;
    }
    return MMI_FALSE;
}

 


/*****************************************************************************
 * FUNCTION
 *  reset_title_status_icon
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_title_status_icon(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__) || defined (__MMI_MAINLCD_320X240__)   
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    gOnFullScreen = MMI_IN_FULL_SCREEN | MMI_LEAVE_FULL_SCREEN;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  close_title_status_icon
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void close_title_status_icon(void)
{
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)|| defined (__MMI_MAINLCD_320X240__)   
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gOnFullScreen = MMI_IN_NORMAL_SCREEN_WITHOUT_STATUSBAR;
    hide_status_icon_bar(0);
    hide_status_icon_bar(1);
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
}

 


/*****************************************************************************
 * FUNCTION
 *  show_title_status_icon
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_title_status_icon(void)
{
 /*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)||defined(__MMI_MAINLCD_320X240__)    
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
 
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    if (!whether_no_small_screen())
        /* if ( (gOnFullScreen & MMI_LEAVE_FULL_SCREEN) || gOnFullScreen==MMI_BEFORE_SCREEN || gOnFullScreen==MMI_IN_FULL_SCREEN || gOnFullScreen==MMI_LEAVE_BG_ON_BOTTOM_SCREEN  || (gOnFullScreen==MMI_IN_NORMAL_SCREEN && wgui_is_wallpaper_on_bottom()))*/
#endif /* __MMI_UI_SMALL_SCREEN_SUPPORT__ */ 
 
    {
        gOnFullScreen = MMI_IN_NORMAL_SCREEN;
    #if defined(__MMI_UI_TECHNO_STATUS_ICON__) || defined(__MMI_UI_DALMATIAN_STATUSBAR__)
        ShowStatusIconsTitle();
        arrange_status_icons();
    #else /* defined(__MMI_UI_TECHNO_STATUS_ICON__) || defined(__MMI_UI_DALMATIAN_STATUSBAR__) */ 
        show_status_icon_bar(0);

         
         
    #if 0
    #endif /* 0 */ 
        register_hide_status_icon_bar(0, hide_status_icons_bar0);
    #endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) || defined(__MMI_UI_DALMATIAN_STATUSBAR__) */ 
        show_status_icons();
    }
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
}

/***************************************************************************** 
* History related wrappers
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  history_valid
 * DESCRIPTION
 *  Returns true if the history ID is valid
 * PARAMETERS
 *  history_ID              [IN]        Is the value obtained by reading the history buffer
 *  category_screen_ID      [IN]        Is the category screen's ID
 * RETURNS
 *  true  if the given history ID is valid
 *  false if the given history ID is not valid
 *****************************************************************************/
U8 history_valid(U16 history_ID, U16 category_screen_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((history_ID & 0x8000) && ((history_ID & 0x7fff) == category_screen_ID))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  change_list_menu_category_history
 * DESCRIPTION
 *  change the highlight index of category screen
 * PARAMETERS
 *  history_buffer              [IN]        History buffer
 *  new_highlighted_index       [IN]        New highlighted index
 *  new_n_items                 [IN]        Number of items
 *  flag                        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void change_list_menu_category_history(U8 *history_buffer, S32 new_highlighted_index, S32 new_n_items, U8 flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((new_highlighted_index < 0) || (new_highlighted_index > (new_n_items - 1)))
    {
        return;
    }
    if (history_buffer != NULL)
    {
        S32 first_displayed_item;
        list_menu_category_history *h = (list_menu_category_history*) history_buffer;

        first_displayed_item = h->first_displayed_item;
        if (new_highlighted_index < first_displayed_item)
        {
            h->first_displayed_item = (S16) new_highlighted_index;
        }
        if (new_highlighted_index == (new_n_items - 1))
        {
            h->last_displayed_item = new_highlighted_index - 1;
        }
        h->highlighted_item = (S16) new_highlighted_index;
        if (flag)
        {
            h->first_displayed_item = (S16) new_highlighted_index;
            h->flags |= UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  create_list_menu_category_history
 * DESCRIPTION
 *  change the highlight index of category screen
 * PARAMETERS
 *  history_ID                  [IN]        Histiry id
 *  history_buffer              [IN]        History buffer
 *  new_highlighted_index       [IN]        New highlighted index
 *  new_n_items                 [IN]        Number of items
 *  flag                        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void create_list_menu_category_history(
        S32 history_ID,
        U8 *history_buffer,
        S32 new_highlighted_index,
        S32 new_n_items,
        U8 flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((new_highlighted_index < 0) || (new_highlighted_index > (new_n_items - 1)))
    {
        return;
    }
    if (history_buffer != NULL)
    {
        list_menu_category_history *h = (list_menu_category_history*) history_buffer;

        h->first_displayed_item = 0;
        h->highlighted_item = (S16) new_highlighted_index;
        h->history_ID = (U16) (history_ID | 0x8000);
        h->flags = current_UI_theme->fixed_list_menu_theme->flags;
        h->displayed_items = 0;
        h->last_displayed_item = 0;
        if (new_highlighted_index < h->first_displayed_item)
        {
            h->first_displayed_item = (S16) new_highlighted_index;
        }
        if (new_highlighted_index == (new_n_items - 1))
        {
            h->last_displayed_item = new_highlighted_index - 1;
        }
        h->highlighted_item = (S16) new_highlighted_index;
        if (flag)
        {
            h->first_displayed_item = (S16) new_highlighted_index;
            h->flags |= UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_list_menu_category_history
 * DESCRIPTION
 *  Sets the history data for the list menu from the given history buffer
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer from which history data is taken
 * RETURNS
 *  Returns true if the history ID is valid
 *****************************************************************************/
U8 set_list_menu_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        list_menu_category_history *h = (list_menu_category_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            MMI_fixed_list_menu.highlighted_item = h->highlighted_item;
            MMI_fixed_list_menu.first_displayed_item = h->first_displayed_item;
            MMI_fixed_list_menu.last_displayed_item = h->last_displayed_item;
            MMI_fixed_list_menu.displayed_items = h->displayed_items;
            MMI_fixed_list_menu.flags = h->flags;
            return (1);
        }
         
    #ifdef __MMI_UI_TAB_PANE__
        else
        {
            if (is_enabled_tab_pane())
            {
                return (set_list_in_tab_history());
            }
        }
    #endif /* __MMI_UI_TAB_PANE__ */ 
         
    }
     
#ifdef __MMI_UI_TAB_PANE__
    else
    {
        if (is_enabled_tab_pane())
        {
            return (set_list_in_tab_history());
        }
    }
#endif /* __MMI_UI_TAB_PANE__ */ 
     
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  change_cat184_list_menu_history_highlighted_item
 * DESCRIPTION
 *  Change the highlighted item stored in history data. (for SMS updating list)
 * PARAMETERS
 *  index               [IN]        New index
 *  history_buffer      [IN]        The history buffer wanted to be changed.
 * RETURNS
 *  void
 *****************************************************************************/
void change_cat184_list_menu_history_highlighted_item(S32 index, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        list_menu_category_history *h = (list_menu_category_history*) history_buffer;

        if (history_valid(h->history_ID, MMI_CATEGORY184_ID))
        {
            h->highlighted_item = index;
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  change_cat84_list_menu_history_highlighted_item
 * DESCRIPTION
 *  Change the highlighted item stored in history data. (for MMS updating list)
 * PARAMETERS
 *  index               [IN]        New index
 *  history_buffer      [IN]        The history buffer wanted to be changed.
 * RETURNS
 *  void
 *****************************************************************************/
void change_cat84_list_menu_history_highlighted_item(S32 index, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        list_menu_category_history *h = (list_menu_category_history*) history_buffer;

        if (history_valid(h->history_ID, MMI_CATEGORY84_ID))
        {
            h->highlighted_item = index;
        }
    }
}

/* CSD 2004-02-13 Wilson, Set and Get state of multiine inputbox history buffer */


/*****************************************************************************
 * FUNCTION
 *  set_list_menu_category_history_state
 * DESCRIPTION
 *  Sets the state of history data for state list
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer into which history data is written
 *  state               [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_list_menu_category_history_state(U16 history_ID, U8 *history_buffer, S8 state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        list_menu_category_history *h = (list_menu_category_history*) history_buffer;

        h->state = state;
    }

}


/*****************************************************************************
 * FUNCTION
 *  get_list_menu_category_history
 * DESCRIPTION
 *  Gets the history data for list menu and stores into the given history buffer
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer into which history data is written
 * RETURNS
 *  void
 *****************************************************************************/
void get_list_menu_category_history(U16 history_ID, U8 *history_buffer)
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
        list_menu_category_history *h = (list_menu_category_history*) history_buffer;

        h->history_ID = hID;
    #ifndef __MMI_DICTIONARY__
        h->highlighted_item = (S16) MMI_fixed_list_menu.highlighted_item;
        h->first_displayed_item = (S16) MMI_fixed_list_menu.first_displayed_item;
        h->last_displayed_item = (S16) MMI_fixed_list_menu.last_displayed_item;
        h->displayed_items = (S16) MMI_fixed_list_menu.displayed_items;
    #else /* __MMI_DICTIONARY__ */ 
        h->highlighted_item = /* (S16) */ MMI_fixed_list_menu.highlighted_item;
        h->first_displayed_item = /* (S16) */ MMI_fixed_list_menu.first_displayed_item;
        h->last_displayed_item = /* (S16) */ MMI_fixed_list_menu.last_displayed_item;
        h->displayed_items = /* (S16) */ MMI_fixed_list_menu.displayed_items;
    #endif /* __MMI_DICTIONARY__ */ 
        h->flags = MMI_fixed_list_menu.flags;
        h->state = (S8) (-1);
    }
}


/*****************************************************************************
 * FUNCTION
 *  get_list_menu_category_history_size
 * DESCRIPTION
 *  Gets the history data size for the list menu
 * PARAMETERS
 *  void
 * RETURNS
 *  Returns the size of list menu history
 *****************************************************************************/
S32 get_list_menu_category_history_size(void)
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
 *  get_list_menu_category_history_state
 * DESCRIPTION
 *  Gets the history data for list menu and stores into the given history buffer
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer into which history data is written
 * RETURNS
 *  void
 *****************************************************************************/
S8 get_list_menu_category_history_state(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        list_menu_category_history *h = (list_menu_category_history*) history_buffer;

        return h->state;
    }
    return (S8) (-1);
}


/*****************************************************************************
 * FUNCTION
 *  set_matrix_menu_category_history
 * DESCRIPTION
 *  Sets the history data for the matrix menu from the given history buffer
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer from which history data is taken
 * RETURNS
 *  Returns true if the history ID is valid
 *****************************************************************************/
U8 set_matrix_menu_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        matrix_menu_category_history *h = (matrix_menu_category_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            MMI_fixed_matrix_menu.highlighted_item = h->highlighted_item;
            MMI_fixed_matrix_menu.highlighted_column = h->highlighted_column;
            MMI_fixed_matrix_menu.highlighted_row = h->highlighted_row;
            MMI_fixed_matrix_menu.displayed_columns = h->displayed_columns;
            MMI_fixed_matrix_menu.displayed_rows = h->displayed_rows;
            MMI_fixed_matrix_menu.first_displayed_column = h->first_displayed_column;
            MMI_fixed_matrix_menu.first_displayed_row = h->first_displayed_row;
            MMI_fixed_matrix_menu.flags = h->flags;
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  get_matrix_menu_category_history
 * DESCRIPTION
 *  Gets the history data for the matrix menu and stores into the given history buffer
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer into which history data is written
 * RETURNS
 *  void
 *****************************************************************************/
void get_matrix_menu_category_history(U16 history_ID, U8 *history_buffer)
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
        matrix_menu_category_history *h = (matrix_menu_category_history*) history_buffer;

        h->history_ID = hID;
        h->highlighted_item = (S16) MMI_fixed_matrix_menu.highlighted_item;
        h->highlighted_column = (S16) MMI_fixed_matrix_menu.highlighted_column;
        h->highlighted_row = (S16) MMI_fixed_matrix_menu.highlighted_row;
        h->displayed_columns = (S16) MMI_fixed_matrix_menu.displayed_columns;
        h->displayed_rows = (S16) MMI_fixed_matrix_menu.displayed_rows;
        h->first_displayed_column = (S16) MMI_fixed_matrix_menu.first_displayed_column;
        h->first_displayed_row = (S16) MMI_fixed_matrix_menu.first_displayed_row;
        h->flags = MMI_fixed_matrix_menu.flags;
    }
}

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__


/*****************************************************************************
 * FUNCTION
 *  set_list_matrix_menu_category_history
 * DESCRIPTION
 *  Sets the history data for the list_matrix main menu from the given history buffer
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer from which history data is taken
 * RETURNS
 *  Returns true if the history ID is valid
 *****************************************************************************/
U8 set_list_matrix_menu_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 flag1 = 0, flag2 = 0;
    list_matrix_menu_category_history *h = (list_matrix_menu_category_history*) history_buffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer == NULL)
    {
        return 0;
    }
    flag1 = set_list_menu_category_history(history_ID, (U8*) & (h->list_menu_history));
    flag2 = set_matrix_menu_category_history(history_ID, (U8*) & (h->matrix_menu_history));
    if (flag1 || flag2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  get_list_matrix_menu_category_history
 * DESCRIPTION
 *  Gets the history data for the list_matrix main menu and stores into the given history buffer
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer into which history data is written
 * RETURNS
 *  void
 *****************************************************************************/
void get_list_matrix_menu_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    list_matrix_menu_category_history *h = (list_matrix_menu_category_history*) history_buffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history(history_ID, (U8*) & (h->list_menu_history));
    get_matrix_menu_category_history(history_ID, (U8*) & (h->matrix_menu_history));
}
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_pen_set_curr_menuitem_wait_longpress
 * DESCRIPTION
 *  Set the current highlighted item to use Wait-Longpress Animation on pen events
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_pen_set_curr_menuitem_wait_longpress(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Only support icontext menu item in current design */
    MMI_ASSERT(MMI_fixed_list_menu.common_item_data == (void*)&MMI_fixed_icontext_menuitem);
    MMI_fixed_icontext_menuitems[MMI_fixed_list_menu.highlighted_item].flags |= UI_MENUITEM_PEN_USE_LONGPRESS;
}


/*****************************************************************************
 * FUNCTION
 *  set_object_list_category_history
 * DESCRIPTION
 *  set history of object list
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer from which history data is taken
 *  index               [IN]        
 * RETURNS
 *  byte
 *****************************************************************************/
U8 set_object_list_category_history(U16 history_ID, U8 *history_buffer, S32 *index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        object_list_category_history *h = (object_list_category_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            if (index != NULL)
            {
                *index = h->index;
            }
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  get_object_list_category_history
 * DESCRIPTION
 *  get history of object list
 * PARAMETERS
 *  history_ID          [IN]        Is the ID used by category screens
 *  history_buffer      [IN]        Is the buffer from which history data is taken
 *  index               [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void get_object_list_category_history(U16 history_ID, U8 *history_buffer, S32 *index)
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
        object_list_category_history *h = (object_list_category_history*) history_buffer;

        h->history_ID = hID;
        if (index != NULL)
        {
            h->index = (S16) * index;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  get_slide_control_category_history
 * DESCRIPTION
 *  set teh history of slide control screen
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [IN]        
 *  byte(?)             [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
void get_slide_control_category_history(U16 history_ID, U8 *history_buffer)
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
        U16 idx = 0;
        slide_control_category_history *h = (slide_control_category_history*) history_buffer;

        h->history_ID = hID;
        for (idx = 0; idx < dm_get_slide_control_count(); idx++)
        {
            h->value[idx] = *MMI_slide_control_value[idx];
            h->n_items = dm_get_slide_control_count();
            if (MMI_current_slide_control == &MMI_slide_control[idx])
            {
                h->current_focus = idx;
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_slide_control_category_history
 * DESCRIPTION
 *  set teh history of slide control screen
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [IN]        
 *  byte(?)             [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
U8 set_slide_control_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        slide_control_category_history *h = (slide_control_category_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            U16 idx = 0;

            for (idx = 0; idx < h->n_items; idx++)
            {
                *MMI_slide_control_value[idx] = h->value[idx];
            }
            MMI_current_slide_control = &MMI_slide_control[h->current_focus];
            MMI_current_slide_control_value = MMI_slide_control_value[h->current_focus];
            return (U8) (h->n_items);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  set_dialer_inputbox_category_history
 * DESCRIPTION
 *  get teh history of dialer inputbox screen
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [IN]        
 *  byte(?)             [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
U8 set_dialer_inputbox_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        dialer_inputbox_category_history *h = (dialer_inputbox_category_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            MMI_dialer_inputbox.cursor_x = h->cursor_x;
            MMI_dialer_inputbox.cursor_y = h->cursor_y;
            MMI_dialer_inputbox.current_text_p = h->current_text_p;
            MMI_dialer_inputbox.last_position_p = h->last_position_p;
            MMI_dialer_inputbox.n_lines = h->n_lines;
            MMI_dialer_inputbox.UCS2_count = h->UCS2_count;
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  get_dialer_inputbox_category_history
 * DESCRIPTION
 *  get teh history of dialer inputbox screen
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [IN]        
 *  byte(?)             [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
void get_dialer_inputbox_category_history(U16 history_ID, U8 *history_buffer)
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
        dialer_inputbox_category_history *h = (dialer_inputbox_category_history*) history_buffer;

        h->history_ID = hID;
        h->cursor_x = MMI_dialer_inputbox.cursor_x;
        h->cursor_y = MMI_dialer_inputbox.cursor_y;
        h->current_text_p = MMI_dialer_inputbox.current_text_p;
        h->last_position_p = MMI_dialer_inputbox.last_position_p;
        h->n_lines = MMI_dialer_inputbox.n_lines;
        h->UCS2_count = MMI_dialer_inputbox.UCS2_count;
    }
}

/***************************************************************************** 
* Key Handlers
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  wgui_save_key_handlers
 * DESCRIPTION
 *  copy all function handlers of keys from currKeyFuncPtrs to wgui_key_handlers
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_save_key_handlers(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_KEYS; i++)
    {
        for (j = 0; j < MAX_KEY_TYPE; j++)
        {
            wgui_key_handlers[i][j] = currKeyFuncPtrs[i][j];
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_execute_key_handler
 * DESCRIPTION
 *  execute key handler function
 * PARAMETERS
 *  key_code        [IN]        
 *  key_event       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_execute_key_handler(S32 key_code, S32 key_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FuncPtr currFuncPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((key_code >= 0 && key_code < MAX_KEYS) && (key_event >= 0 && key_event < MAX_KEY_TYPE))
    {
        currFuncPtr = wgui_key_handlers[key_code][key_event];
        if (currFuncPtr)
        {
            (*currFuncPtr) ();
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_restore_key_handlers
 * DESCRIPTION
 *  restore key handlers
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_restore_key_handlers(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_KEYS; i++)
    {
        for (j = 0; j < MAX_KEY_TYPE; j++)
        {
            currKeyFuncPtrs[i][j] = wgui_key_handlers[i][j];
        }
    }
}

/***************************************************************************** 
* Start : status icons control
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  hide_status_icons_bar0
 * DESCRIPTION
 *  hide function of horizontal status bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void hide_status_icons_bar0(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* 061506 double buffer Start */
    if(get_status_icon_bar_layer(0)!=GDI_ERROR_HANDLE)
    {
        gdi_layer_push_and_set_active(get_status_icon_bar_layer(0));
    }
    /* 061506 double buffer End */
    
    gui_push_clip();
    get_status_icon_bar_clip(0, &x1, &y1, &x2, &y2);
    gui_set_clip(x1, y1, x2, y2);
     
    if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
    {
    	#ifdef  __MMI_MAINLCD_160X128__
	    if (GetActiveScreenId( ) != IDLE_SCREEN_ID)
		gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_BLACK);
	    else	
	#endif	
        gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
    }
    else
    {
#if defined(__MMI_MAINLCD_220X176__)
		gdi_image_draw_id(0,0,IMG_NOKIA_E_SERIES_STATUS_BG);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
 #elif defined(__MMI_MAINLCD_320X240__)
		gdi_image_draw_id(0,0,IMG_TITLEBAR_BG);
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#else
        color_t c = *current_MMI_theme->statusbar0_background_color;

        gui_fill_rectangle(x1, y1, x2, y2, c);
#endif
    }
     
    gui_pop_clip();
    
    /* 061506 double buffer Start */
    if(get_status_icon_bar_layer(0)!=GDI_ERROR_HANDLE)
    {
        gdi_layer_pop_and_restore_active();
    }
    /* 061506 double buffer End */
}


/*****************************************************************************
 * FUNCTION
 *  hide_status_icons_bar1
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void hide_status_icons_bar1(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_filled_area *f = current_MMI_theme->network_screen_background_filler;
    S32 x1, x2, y1, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    get_status_icon_bar_clip(1, &x1, &y1, &x2, &y2);
#if defined(__PROJECT_GALLITE_C01__)
	x2 += 4;
#endif
    gui_set_clip(x1, y1, x2, y2);
    gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, f);
    gui_pop_clip();
}

 


/*****************************************************************************
 * FUNCTION
 *  hide_status_icons_bar0_by_transparent
 * DESCRIPTION
 *  hide function of horizontal status bar by transparent color
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void hide_status_icons_bar0_by_transparent(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    get_status_icon_bar_clip(0, &x1, &y1, &x2, &y2);
    gui_set_clip(x1, y1, x2, y2);
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
    gui_pop_clip();
}

 


/*****************************************************************************
 * FUNCTION
 *  ShowStatusIconsTitle
 * DESCRIPTION
 *  show staus bar on main lcd screen
 * PARAMETERS
 *  void
 *  S16(?)      [IN]        Icon_ID
 * RETURNS
 *  void
 *****************************************************************************/
void ShowStatusIconsTitle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    set_status_icon_bar_clip(0, 0, 0, MMI_STATUS_BAR_WIDTH - 1, MMI_SINGLE_STATUS_BAR_HEIGHT - 1);
    set_status_icon_bar_clip(
        1,
        UI_device_width - MMI_STATUS_BAR_WIDTH,
        MMI_status_bar_height - MMI_SINGLE_STATUS_BAR_HEIGHT,
        UI_device_width - 1,
        MMI_status_bar_height - 1);
    show_status_icon_bar(0);
    show_status_icon_bar(1);
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
     
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
    if (GetActiveScreenId() == MAIN_MENU_SCREENID)
    {
        set_status_icon_bar_clip(
            0,
            MMI_SIGNAL_WIDTH,
            0,
            UI_device_width - MMI_SIGNAL_WIDTH - 1,
            MMI_status_bar_height - 1);
    }
    else
    {
        set_status_icon_bar_clip(0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1);
    }
#else /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
    register_hide_status_icon_bar(0, hide_status_icons_bar0);
    set_status_icon_bar_clip(0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1);
#endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
     
    show_status_icon_bar(0);
    hide_status_icon_bar(1);
     
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
}

/***************************************************************************** 
* Start : Softkey control
*****************************************************************************/

#if defined(__MMI_SCREEN_ROTATE__)
#include "bootup.h"
#include "statusiconres.h"
#include "vdorecscrenum.h"
void draw_rotated_title(void)
{

    if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
    {
        gdi_draw_solid_rect(MMI_title_x, MMI_title_y, MMI_title_x+MMI_title_width-1, MMI_title_y+MMI_title_height -1, GDI_COLOR_TRANSPARENT);
    }
    else
    {
   
        gdi_draw_solid_rect(MMI_title_x, MMI_title_y, MMI_title_x+MMI_title_width-1, MMI_title_y+MMI_title_height -1, GDI_COLOR_TRANSPARENT);
      //  gui_show_image(0, UI_device_height - MMI_button_bar_height, (PU8) GetImage(IMG_SOFTKEY_BAR));
      //  gui_show_image(0, UI_device_height - MMI_button_bar_height, (PU8) GetImage(IMG_SOFTKEY_BAR));  //Wufang MODI   2007-4-28
	}

	if ((NULL != MMI_title_string)&&(!is_on_idlescreen()))
	{
            gui_set_font(&MMI_medium_font);
            gui_push_text_clip();
	
		if(!r2lMMIFlag)
		{
			gui_set_text_clip(MMI_title_x, MMI_title_y, MMI_title_x+MMI_title_width-1, MMI_title_y+MMI_title_height-1);
			gui_move_text_cursor(MMI_title_x, MMI_title_y);
		}
		else
			{
				gui_set_text_clip(MMI_title_x, MMI_title_y, MMI_title_x+MMI_title_width-1, MMI_title_y+MMI_title_height-1);
				gui_move_text_cursor(MMI_title_x+MMI_title_width-1, MMI_title_y);
			}
		
	gui_set_text_color(UI_COLOR_WHITE);
	gui_print_bordered_text(MMI_title_string);
	gui_pop_text_clip();
    }

}

U8 draw_rotated_statusbar(void)
{
    S32 ix1, iy1 , id;
	S32 image_width , image_height ;
	int nIconCountOther = 0 ;
	int i,j ;
	int blink_animate_flag = 0 ;

	if(g_idle_context.ScreenSaverRunFlag)
		return 0;

	gui_push_clip();
	gui_reset_clip();
	
	for (j = 0; j < MAX_STATUS_ICON_BARS; j++)
	{
		for (i = 0; i < MMI_status_icon_bars[j].n_icons; i++)
		{
			id = MMI_status_icon_bars[j].icon_list[i];
			if (MMI_status_icons[id].icon_ID != IMAGE_ID_NULL && (MMI_status_icons[id].flags & STATUS_ICON_DISPLAY))
			{
#if 0		/* wanglei fix for bug17744 	*/
				/* store position of the icon */
				if( (MMI_status_icons[id].icon_ID == IMG_SI_SIGNAL_STRENGTH_L1)||
					(MMI_status_icons[id].icon_ID == IMG_SI_SIGNAL_STRENGTH_L2))
				{
					gui_measure_image((PU8) GetImage((U16) MMI_status_icons[id].icon_ID), &image_width, &image_height);
					gui_set_clip(0, 0, UI_device_width-1, MMI_ROTATED_TITLE_HEIGHT);
	
					ix1 = MMI_status_icons[id].x;
					iy1 = MMI_status_icons[id].y;
				}
				else if(id== STATUS_ICON_BATTERY_STRENGTH)
				{
					gui_set_clip(0, 0, UI_device_width-1, UI_device_height-1);
					ix1 = MMI_status_icons[id].x - MMI_ROTATED_BUTTON_BAR_WIDTH;//UI_device_width - MMI_status_icons[id].width - 1;
					iy1 = MMI_status_icons[id].y = 0;//0;	
				}
				else
				{	
					gui_set_clip(0, 0, UI_device_width-1, MMI_ROTATED_TITLE_HEIGHT);
					nIconCountOther++ ;
					gui_measure_image((PU8) GetImage((U16) MMI_status_icons[id].icon_ID), &image_width, &image_height);
	
					ix1 = ix2 ;
					iy1 = 1;
					ix2 = ix1+image_width ;
	
				}
#else
				if(id== STATUS_ICON_BATTERY_STRENGTH)
				{
					gui_set_clip(0, UI_device_height-MMI_ROTATED_BUTTON_BAR_HEIGHT-1, UI_device_width, UI_device_height-1);
					ix1 = MMI_status_icons[id].x - MMI_ROTATED_BUTTON_BAR_WIDTH;//UI_device_width - MMI_status_icons[id].width - 1;
					iy1 = MMI_status_icons[id].y = 0;//0;	
				}
				else
				{	
					gui_set_clip(0, 0, UI_device_width-1, MMI_ROTATED_TITLE_HEIGHT);
					nIconCountOther++ ;
					gui_measure_image((PU8) GetImage((U16) MMI_status_icons[id].icon_ID), &image_width, &image_height);
	
					ix1 = MMI_status_icons[id].x;
					iy1 = MMI_status_icons[id].y;	
				}                          
#endif
	
				if (MMI_status_icons[id].flags & STATUS_ICON_ANIMATE)
				{
					   blink_animate_flag = 1;
	
					   gui_show_transparent_animated_image_frame(
						   ix1,
						   iy1,
						   MMI_status_icons[id].icon_data,
						   0,
						   MMI_status_icons[id].current_frame); 	   
					   
				}
				else if (MMI_status_icons[id].flags & STATUS_ICON_DISPLAY)
				{
					if (MMI_status_icons[id].state > 0)
					{
						gui_show_transparent_animated_image_frame(
							ix1,
							iy1,
							MMI_status_icons[id].icon_data,
							0,
							MMI_status_icons[id].state);
					}
					else if (MMI_status_icons[id].flags & STATUS_ICON_BLINK)
					{
						blink_animate_flag = 1;
						if(MMI_status_icons_blink_animate_ON_state)
							gui_show_transparent_image(ix1, iy1, MMI_status_icons[id].icon_data, 0);
					}
					else
					{
						gui_show_transparent_image(ix1, iy1, MMI_status_icons[id].icon_data, 0);
					}
				}					
			}
		}

	}
	gui_pop_clip();
    
    if (blink_animate_flag) /* if icon is animated */
    {
        if (status_icon_blink_animate_ON == 0)
        {
            setup_status_icon_blink_animate();
        }
        status_icon_blink_animate_ON = 1;
    }
    else
    {
        if (status_icon_blink_animate_ON == 1)
        {
            setup_status_icon_blink_animate();
        }
        status_icon_blink_animate_ON = 0;
    }

	return blink_animate_flag;


}
#endif


/*****************************************************************************
 * FUNCTION
 *  show_softkey_background_with_clipping
 * DESCRIPTION
 *  Draws a background behind the softkeys with clipping
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void show_softkey_background_with_clipping(S32 x1, S32 y1, S32 x2, S32 y2)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	color_t c;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	c = *(current_MMI_theme->button_bar_color);
	
#ifdef __MMI_SCREEN_ROTATE__
	if (mmi_frm_is_screen_width_height_swapped())
	{
		gui_push_clip();
		gui_reset_clip();
		if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
			gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
		else
		{
			if(SCR_ID_VDOREC_APP != GetCurrScrnId())
			{
				//gui_show_image(0, 0, (PU8)get_image(IMG_SCREEN_ROTATE_TITLE));
				//gui_show_image(0, UI_device_height-MMI_ROTATED_BUTTON_BAR_HEIGHT-1, (PU8)get_image(IMG_ROTATED_BUTTONBAR));
			}
		}
		draw_rotated_statusbar();
		gui_pop_clip();
		return;
	}
#endif /* __MMI_SCREEN_ROTATE__ */ 

	gui_push_clip();
	gui_set_clip(x1, y1, x2, y2);
#if defined(UI_SOFTKEY_STYLE_2)
	//color_t c;
	UI_UNUSED_PARAMETER(c);
	if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
	{
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#ifdef __MMI_BLACK_BERRY_MAIN_MENU__
		if (GetActiveScreenId() != MAIN_MENU_SCREENID && GetActiveScreenId() != IDLE_SCREEN_ID)
			gdi_image_draw_id(0,UI_device_height - MMI_button_bar_height,IMG_SOFTKEY_BAR);
	#else
		gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	}
	else
	{
#if defined( __MMI_MAINLCD_220X176__  )
	gdi_image_draw_id(0,UI_device_height - MMI_button_bar_height,IMG_NOKIA_E_SERIES_BUTTON_BAR_BG);
	//gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
#else
#if defined(__MMI_TOUCH_SCREEN__)	//guoyt added@20100827 for virtual keyboard edge often conflict with softkey background edge.
		if((MMI_CATEGORY28_ID == g_dm_data.S32CatId) || (MMI_CATEGORY5_ID == g_dm_data.S32CatId) || (MMI_CATEGORY57_ID == g_dm_data.S32CatId))
			gui_show_image(0, UI_device_height - MMI_button_bar_height + 1, (PU8) GetImage(IMG_SOFTKEY_BAR));
		else
#endif			
#if defined(__PROJECT_GALLITE_C01__)
		gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
#else
			gui_show_image(0, UI_device_height - MMI_button_bar_height, (PU8) GetImage(IMG_SOFTKEY_BAR));
#endif
#endif
	}
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#elif defined( __MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)||defined (__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
	{
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#ifdef __MMI_BLACK_BERRY_MAIN_MENU__
		if (GetActiveScreenId() != MAIN_MENU_SCREENID && GetActiveScreenId() != IDLE_SCREEN_ID)
			gdi_image_draw_id(0,UI_device_height - MMI_button_bar_height,IMG_SOFTKEY_BAR);
	#else
		gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	}
	else
	#if defined(__MMI_UI_TECHNO_SOFTKEYS__)
	gui_fill_rectangle(x1, y1, x2, y2, *(current_MMI_theme->button_bar_color));
	#elif defined( __MMI_UI_DALMATIAN_SOFTKEYS__)
	gui_fill_rectangle(0,
					UI_device_height - MMI_button_bar_height,
					UI_device_width - 1,
					UI_device_height - 1,
					c);
	#else 
	gdi_draw_gradient_rect(x1, y1, x2, y2, GDI_TRANSFORM_GRADIENT_COLOR(c), 0, 0, GDI_GRADIENT_RECT_FLIP);
	#endif
#else
	gui_fill_rectangle(x1, y1, x2, y2, c);
#endif 

	gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  show_softkey_background
 * DESCRIPTION
 *  Draws a background behind the softkeys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_softkey_background(void)
{
#ifdef __MMI_SCREEN_ROTATE__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mmi_frm_is_screen_width_height_swapped())
    {
        show_softkey_background_with_clipping(
            MMI_ROTATED_BUTTON_BAR_X + 1 /* TEMP FIX */ ,
            MMI_ROTATED_BUTTON_BAR_Y,
            MMI_ROTATED_BUTTON_BAR_X + MMI_ROTATED_BUTTON_BAR_WIDTH - 1,
            MMI_ROTATED_BUTTON_BAR_Y + MMI_ROTATED_BUTTON_BAR_HEIGHT - 1);
    }
    else
#endif /* __MMI_SCREEN_ROTATE__ */ 
        show_softkey_background_with_clipping(
            0,
            UI_device_height - MMI_button_bar_height,
            UI_device_width - 1,
            UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_clear_softkey
 * DESCRIPTION
 *  Hides the softkeys using transparent color
 *  
 *  Default hide left softkey function
 * PARAMETERS
 *  key     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_clear_softkey(WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (key)
    {
        case MMI_LEFT_SOFTKEY:
    #ifdef __MMI_SCREEN_ROTATE__
            if (mmi_frm_is_screen_width_height_swapped())
            {
                gdi_draw_solid_rect(
                    MMI_softkeys[MMI_LEFT_SOFTKEY].x,
                    MMI_softkeys[MMI_LEFT_SOFTKEY].y,
                    MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width + 2,
                    MMI_softkeys[MMI_LEFT_SOFTKEY].y + MMI_softkeys[MMI_LEFT_SOFTKEY].height + 2,
                    GDI_COLOR_TRANSPARENT);
            }
            else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
                gdi_draw_solid_rect(
                    MMI_softkeys[MMI_LEFT_SOFTKEY].x,
                    UI_device_height - MMI_button_bar_height,
                    MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width + 2,
                    UI_device_height - 1,
                    GDI_COLOR_TRANSPARENT);
            break;

        case MMI_RIGHT_SOFTKEY:
        #ifdef __MMI_SCREEN_ROTATE__
            if (mmi_frm_is_screen_width_height_swapped())
            {
                gdi_draw_solid_rect(
                    MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
                    MMI_softkeys[MMI_RIGHT_SOFTKEY].y,
                    MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width + 2,
                    MMI_softkeys[MMI_RIGHT_SOFTKEY].y + MMI_softkeys[MMI_RIGHT_SOFTKEY].height + 2,
                    GDI_COLOR_TRANSPARENT);
            }
            else
        #endif /* __MMI_SCREEN_ROTATE__ */ 
                gdi_draw_solid_rect(
                    MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
                    UI_device_height - MMI_button_bar_height,
                    MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width + 2,
                    UI_device_height - 1,
                    GDI_COLOR_TRANSPARENT);
            break;
        #ifdef __MMI_WGUI_CSK_ENABLE__
        case MMI_CENTER_SOFTKEY:
        #ifdef __MMI_SCREEN_ROTATE__
            if (mmi_frm_is_screen_width_height_swapped())
            {
                gdi_draw_solid_rect(
                    MMI_softkeys[MMI_CENTER_SOFTKEY].x,
                    MMI_softkeys[MMI_CENTER_SOFTKEY].y,
                    MMI_softkeys[MMI_CENTER_SOFTKEY].x + MMI_softkeys[MMI_CENTER_SOFTKEY].width + 2,
                    MMI_softkeys[MMI_CENTER_SOFTKEY].y + MMI_softkeys[MMI_CENTER_SOFTKEY].height + 2,
                    GDI_COLOR_TRANSPARENT);
            }
            else
        #endif /* __MMI_SCREEN_ROTATE__ */ 
                gdi_draw_solid_rect(
                    MMI_softkeys[MMI_CENTER_SOFTKEY].x,
                    UI_device_height - MMI_button_bar_height,
                    MMI_softkeys[MMI_CENTER_SOFTKEY].x + MMI_softkeys[MMI_CENTER_SOFTKEY].width + 2,
                    UI_device_height - 1,
                    GDI_COLOR_TRANSPARENT);
            break;
        #endif /* __MMI_WGUI_CSK_ENABLE__ */ 
        default:
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_clear_left_softkey
 * DESCRIPTION
 *  Hides the left softkey using transparent color
 *  
 *  Default hide left softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_clear_left_softkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_clear_softkey(MMI_LEFT_SOFTKEY);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_clear_right_softkey
 * DESCRIPTION
 *  Hides the left softkey using transparent color
 *  
 *  Default hide left softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_clear_right_softkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_clear_softkey(MMI_LEFT_SOFTKEY);
}

#ifdef __MMI_WGUI_CSK_ENABLE__


/*****************************************************************************
 * FUNCTION
 *  wgui_clear_center_softkey
 * DESCRIPTION
 *  Hides the left softkey using transparent color
 *  
 *  Default hide left softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_clear_center_softkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_clear_softkey(MMI_CENTER_SOFTKEY);
}
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_hide_left_softkey
 * DESCRIPTION
 *  Hides the left softkey
 *  
 *  Default hide left softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_hide_left_softkey(void)
{
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)|| defined (__MMI_MAINLCD_320X240__)  
/*- zhouqin 2011-02-16 modify for q6 320X240 */
/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(MMI_softkeys[MMI_LEFT_SOFTKEY].flags & UI_BUTTON_NO_UNDER_BAR))
#else /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
    if (!(MMI_softkeys[MMI_LEFT_SOFTKEY].flags & UI_BUTTON_DISABLE_BACKGND_DISPLAY))
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) */ 
    {
    #ifdef __MMI_SCREEN_ROTATE__
        if (mmi_frm_is_screen_width_height_swapped())
        {
            show_softkey_background_with_clipping(
                MMI_softkeys[MMI_LEFT_SOFTKEY].x,
                MMI_softkeys[MMI_LEFT_SOFTKEY].y,
                MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width + 2,
                MMI_softkeys[MMI_LEFT_SOFTKEY].y + MMI_softkeys[MMI_LEFT_SOFTKEY].height + 2);
        }
        else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
            show_softkey_background_with_clipping(
                MMI_softkeys[MMI_LEFT_SOFTKEY].x,
                MMI_softkeys[MMI_LEFT_SOFTKEY].y,
                MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width + 2,
                UI_device_height - 1);
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_hide_right_softkey
 * DESCRIPTION
 *  Hides the right softkey
 *  
 *  Default hide right softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_hide_right_softkey(void)
{
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)||defined(__MMI_MAINLCD_320X240__)     
/*- zhouqin 2011-02-16 modify for q6 320X240 */
/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(MMI_softkeys[MMI_RIGHT_SOFTKEY].flags & UI_BUTTON_NO_UNDER_BAR))
#else /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
    if (!(MMI_softkeys[MMI_RIGHT_SOFTKEY].flags & UI_BUTTON_DISABLE_BACKGND_DISPLAY))
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
    {
    #ifdef __MMI_SCREEN_ROTATE__
        if (mmi_frm_is_screen_width_height_swapped())
        {
            show_softkey_background_with_clipping(
                MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
                MMI_softkeys[MMI_RIGHT_SOFTKEY].y,
                MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width + 2,
                MMI_softkeys[MMI_RIGHT_SOFTKEY].y + MMI_softkeys[MMI_RIGHT_SOFTKEY].height + 2);
        }
        else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
            show_softkey_background_with_clipping(
                MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
                MMI_softkeys[MMI_RIGHT_SOFTKEY].y,
                MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width + 2,
                UI_device_height - 1);
    }
}

#ifdef __MMI_WGUI_CSK_ENABLE__


/*****************************************************************************
 * FUNCTION
 *  wgui_hide_center_softkey
 * DESCRIPTION
 *  Hides the center softkey
 *  
 *  Default hide center softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_hide_center_softkey(void)
{
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)||defined(__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(MMI_softkeys[MMI_CENTER_SOFTKEY].flags & UI_BUTTON_NO_UNDER_BAR))
#else /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
    if (!(MMI_softkeys[MMI_CENTER_SOFTKEY].flags & UI_BUTTON_DISABLE_BACKGND_DISPLAY))
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) */ 
    {
    #ifdef __MMI_SCREEN_ROTATE__
        if (mmi_frm_is_screen_width_height_swapped())
        {
            show_softkey_background_with_clipping(
                MMI_softkeys[MMI_CENTER_SOFTKEY].x,
                MMI_softkeys[MMI_CENTER_SOFTKEY].y,
                MMI_softkeys[MMI_CENTER_SOFTKEY].x + MMI_softkeys[MMI_CENTER_SOFTKEY].width + 2,
                MMI_softkeys[MMI_CENTER_SOFTKEY].y + MMI_softkeys[MMI_CENTER_SOFTKEY].height + 2);
        }
        else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
            show_softkey_background_with_clipping(
                MMI_softkeys[MMI_CENTER_SOFTKEY].x,
                UI_device_height - MMI_button_bar_height,
                MMI_softkeys[MMI_CENTER_SOFTKEY].x + MMI_softkeys[MMI_CENTER_SOFTKEY].width + 2,
                UI_device_height - 1);
    }
}

#endif /* __MMI_WGUI_CSK_ENABLE__ */ 


/*****************************************************************************
 * FUNCTION
 *  register_default_hide_softkeys
 * DESCRIPTION
 *  Registers the default functions to hide the softkeys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_default_hide_softkeys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_hide_left_softkey(wgui_hide_left_softkey);
    register_hide_right_softkey(wgui_hide_right_softkey);
#ifdef __MMI_WGUI_CSK_ENABLE__
    register_hide_softkey(wgui_hide_center_softkey, MMI_CENTER_SOFTKEY);
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  register_default_hide_softkeys_to_transparent
 * DESCRIPTION
 *  Registers the default functions to hide the softkeys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_default_hide_softkeys_to_transparent(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_hide_softkey(wgui_clear_left_softkey, MMI_LEFT_SOFTKEY);
    register_hide_softkey(wgui_clear_right_softkey, MMI_RIGHT_SOFTKEY);
#ifdef __MMI_WGUI_CSK_ENABLE__
    register_hide_softkey(wgui_clear_center_softkey, MMI_CENTER_SOFTKEY);
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  ChangeLeftSoftkey
 * DESCRIPTION
 *  Changes the left softkey label and icon
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeLeftSoftkey(U16 s, U16 i)
{
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_shct_cntx.IsKeyRetrieving)
    {
        return;
    }
#endif /* defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__) */ 
     
#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__
    hide_softkey[MMI_LEFT_SOFTKEY] ();
#endif 
     
    change_left_softkey(s, i);
    redraw_softkey(MMI_LEFT_SOFTKEY);
}


/*****************************************************************************
 * FUNCTION
 *  ChangeRightSoftkey
 * DESCRIPTION
 *  Changes the right softkey label and icon
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeRightSoftkey(U16 s, U16 i)
{
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_shct_cntx.IsKeyRetrieving)
    {
        return;
    }
#endif /* defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__) */ 
     
#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__
    hide_softkey[MMI_RIGHT_SOFTKEY] ();
#endif 
     
    change_right_softkey(s, i);
    redraw_right_softkey();
}

#ifdef __MMI_WGUI_CSK_ENABLE__


/*****************************************************************************
 * FUNCTION
 *  ChangeCenterSoftkey
 * DESCRIPTION
 *  Changes the left softkey label and icon
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeCenterSoftkey(U16 s, U16 i)
{
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_shct_cntx.IsKeyRetrieving)
    {
        return;
    }
#endif /* defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__) */ 
    hide_softkey[MMI_CENTER_SOFTKEY] ();
    change_softkey(s, i, MMI_CENTER_SOFTKEY);
    redraw_softkey(MMI_CENTER_SOFTKEY);
}
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 
 


/*****************************************************************************
 * FUNCTION
 *  ChangeLeftSoftkeyByStringLength
 * DESCRIPTION
 *  Changes the left softkey label and icon and also adjust the width according to the string
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeLeftSoftkeyByStringLength(UI_string_type s, PU8 i)
{
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_shct_cntx.IsKeyRetrieving)
    {
        return;
    }
#endif /* defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__) */ 

    gui_lock_double_buffer();
     
#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__
    hide_softkey[MMI_LEFT_SOFTKEY] ();
#endif 
     
    resize_left_softkey_by_string_length(s);
    set_left_softkey_label(s);
    set_left_softkey_icon(i);
    redraw_left_softkey();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, UI_device_height - MMI_button_bar_height, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ChangeRightSoftkeyByStringLength
 * DESCRIPTION
 *  Changes the right softkey label and icon and also adjust the width according to the string
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeRightSoftkeyByStringLength(UI_string_type s, PU8 i)
{
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_shct_cntx.IsKeyRetrieving)
    {
        return;
    }
#endif /* defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__) */ 

    gui_lock_double_buffer();
     
#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__
    hide_softkey[MMI_RIGHT_SOFTKEY] ();
#endif 
     
    resize_right_softkey_by_string_length(s);
    set_right_softkey_label(s);
    set_right_softkey_icon(i);
    redraw_right_softkey();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, UI_device_height - MMI_button_bar_height, UI_device_width - 1, UI_device_height - 1);
}

 

/*****************************************************************************
 * FUNCTION
 *  ChangeLeftSoftkeyByString
 * DESCRIPTION
 *  Changes the left softkey label using a dynamic string
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeLeftSoftkeyByString(UI_string_type s, PU8 i)
{
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_shct_cntx.IsKeyRetrieving)
    {
        return;
    }
#endif /* defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__) */ 

    gui_lock_double_buffer();
     
#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__
    hide_softkey[MMI_LEFT_SOFTKEY] ();
#endif 
     
    set_left_softkey_label(s);
    set_left_softkey_icon(i);
    redraw_left_softkey();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, UI_device_height - MMI_button_bar_height, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ChangeRightSoftkeyByString
 * DESCRIPTION
 *  Changes the right softkey label using a dynamic string
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeRightSoftkeyByString(UI_string_type s, PU8 i)
{
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_shct_cntx.IsKeyRetrieving)
    {
        return;
    }
#endif /* defined(__MMI_DEDICATED_KEY_SHORTCUTS__) && !defined(__DISABLE_SHORTCUTS_IMPL__) */ 

    gui_lock_double_buffer();
     
#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__
    hide_softkey[MMI_RIGHT_SOFTKEY] ();
#endif 
     
    set_right_softkey_label(s);
    set_right_softkey_icon(i);
    redraw_right_softkey();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, UI_device_height - MMI_button_bar_height, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  EnableLeftSoftkey
 * DESCRIPTION
 *  Enable the left soft key , set string and icon of LSK and display LSK
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void EnableLeftSoftkey(U16 s, U16 i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (s != STRING_ID_NULL)
    {
        UI_string_type str = get_string(s);

        set_left_softkey_label(str);
    }
    if (i != IMAGE_ID_NULL)
    {
        PU8 img = get_image(i);

        set_left_softkey_icon(img);
    }
    MMI_softkeys[MMI_LEFT_SOFTKEY].flags &= ~UI_BUTTON_STATE_DISABLED;
    redraw_left_softkey();
    register_left_softkey_handler();
}


/*****************************************************************************
 * FUNCTION
 *  DisableLeftSoftkey
 * DESCRIPTION
 *  Disable the left soft key , set string and icon of LSK and display LSK
 *  with disable color
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void DisableLeftSoftkey(U16 s, U16 i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (s != STRING_ID_NULL)
    {
        UI_string_type str = get_string(s);

        set_left_softkey_label(str);
    }
    if (i != IMAGE_ID_NULL)
    {
        PU8 img = get_image(i);

        set_left_softkey_icon(img);
    }
    MMI_softkeys[MMI_LEFT_SOFTKEY].flags |= UI_BUTTON_STATE_DISABLED;
    MMI_softkeys[MMI_LEFT_SOFTKEY].disabled_up_filler = current_MMI_theme->LSK_disabled_filler;
    MMI_softkeys[MMI_LEFT_SOFTKEY].disabled_down_filler = current_MMI_theme->LSK_disabled_filler;
    redraw_left_softkey();
    clear_left_softkey_handler();
}


/*****************************************************************************
 * FUNCTION
 *  EnableRightSoftkey
 * DESCRIPTION
 *  Enable the right soft key , set string and icon of RSK and display RSK
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void EnableRightSoftkey(U16 s, U16 i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (s != STRING_ID_NULL)
    {
        UI_string_type str = get_string(s);

        set_right_softkey_label(str);
    }
    if (i != IMAGE_ID_NULL)
    {
        PU8 img = get_image(i);

        set_right_softkey_icon(img);
    }
    MMI_softkeys[MMI_RIGHT_SOFTKEY].flags &= ~UI_BUTTON_STATE_DISABLED;
    redraw_right_softkey();
    register_right_softkey_handler();
}


/*****************************************************************************
 * FUNCTION
 *  DisableRightSoftkey
 * DESCRIPTION
 *  Disable the right soft key , set string and icon of RSK and display RSK
 *  with disable color
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void DisableRightSoftkey(U16 s, U16 i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (s != STRING_ID_NULL)
    {
        UI_string_type str = get_string(s);

        set_right_softkey_label(str);
    }
    if (i != IMAGE_ID_NULL)
    {
        PU8 img = get_image(i);

        set_right_softkey_icon(img);
    }
    MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |= UI_BUTTON_STATE_DISABLED;
    MMI_softkeys[MMI_RIGHT_SOFTKEY].disabled_up_filler = current_MMI_theme->LSK_disabled_filler;
    MMI_softkeys[MMI_RIGHT_SOFTKEY].disabled_down_filler = current_MMI_theme->LSK_disabled_filler;
    redraw_right_softkey();
    clear_right_softkey_handler();
}

#ifdef __MMI_WGUI_CSK_ENABLE__


/*****************************************************************************
 * FUNCTION
 *  EnableCenterSoftkey
 * DESCRIPTION
 *  Enable the left soft key , set string and icon of LSK and display LSK
 * PARAMETERS
 *  s       [IN]        Is the string to be displayed in the softkey
 *  i       [IN]        Is the image to be displayed in the softkey
 * RETURNS
 *  void
 *****************************************************************************/
void EnableCenterSoftkey(U16 s, U16 i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (s != STRING_ID_NULL)
    {
        UI_string_type str = get_string(s);

        set_softkey_label(str, MMI_CENTER_SOFTKEY);
    }
    if (i != IMAGE_ID_NULL)
    {
        PU8 img = get_image(i);

        set_softkey_icon(img, MMI_CENTER_SOFTKEY);
    }
    MMI_softkeys[MMI_CENTER_SOFTKEY].flags &= ~UI_BUTTON_STATE_DISABLED;
    /* redraw_softkey(MMI_CENTER_SOFTKEY); */
    register_softkey_handler(MMI_CENTER_SOFTKEY);
}


/*****************************************************************************
 * FUNCTION
 *  ResetCenterSoftkey
 * DESCRIPTION
 *  Disable center softkey display
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ResetCenterSoftkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    change_softkey(0, 0, MMI_CENTER_SOFTKEY);
}
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 

/*****************************************************************************
 * FUNCTION
 *  SetLeftSoftkeyAlwaysKeyUpByPen
 * DESCRIPTION
 *  Emulate Key Up event even when pen is up outside the LSK button
 *  (By default, Key Up is only generated when pen is up inside a button)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SetLeftSoftkeyAlwaysKeyUpByPen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_softkey_flags_on(UI_BUTTON_ALWAYS_SEND_PEN_UP_INSIDE, MMI_LEFT_SOFTKEY);
}


/*****************************************************************************
 * FUNCTION
 *  SetRightSoftkeyAlwaysKeyUpByPen
 * DESCRIPTION
 *  Emulate Key Up event even when pen is up outside the RSK button
 *  (By default, Key Up is only generated when pen is up inside a button)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SetRightSoftkeyAlwaysKeyUpByPen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_softkey_flags_on(UI_BUTTON_ALWAYS_SEND_PEN_UP_INSIDE, MMI_RIGHT_SOFTKEY);
}

#ifdef __MMI_WGUI_CSK_ENABLE__


/*****************************************************************************
 * FUNCTION
 *  SetCenterSoftkeyAlwaysKeyUpByPen
 * DESCRIPTION
 *  Emulate Key Up event even when pen is up outside the center softkey
 *  (By default, Key Up is only generated when pen is up inside a button)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SetCenterSoftkeyAlwaysKeyUpByPen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_softkey_flags_on(UI_BUTTON_ALWAYS_SEND_PEN_UP_INSIDE, MMI_CENTER_SOFTKEY);
}
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 


/*****************************************************************************
 * FUNCTION
 *  SetLeftSoftkeyFunction
 * DESCRIPTION
 *  Sets the function that is called when a particular softkey event occurs
 * PARAMETERS
 *  f       [IN]        Is the function that is called
 *  k       [IN]        Is the key event (up, down...)
 * RETURNS
 *  void
 *****************************************************************************/
void SetLeftSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_left_softkey_handler();
    set_left_softkey_function(f, k);
    SetInputboxLSKFunction(f);
}


/*****************************************************************************
 * FUNCTION
 *  SetRightSoftkeyFunction
 * DESCRIPTION
 *  Sets the function that is called when a particular softkey event occurs
 * PARAMETERS
 *  f       [IN]        Is the function that is called
 *  k       [IN]        Is the key event (up, down...)
 * RETURNS
 *  void
 *****************************************************************************/
void SetRightSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_right_softkey_handler();
    set_right_softkey_function(f, k);
}

#ifdef __MMI_WGUI_CSK_ENABLE__
 

/*****************************************************************************
 * FUNCTION
 *  SetCenterSoftkeyFunction
 * DESCRIPTION
 *  Sets the function that is called when a particular softkey event occurs
 * PARAMETERS
 *  f       [IN]        Is the function that is called
 *  k       [IN]        Is the key event (up, down...)
 * RETURNS
 *  void
 *****************************************************************************/
void SetCenterSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_softkey_handler(MMI_CENTER_SOFTKEY);
    set_softkey_function(f, k, MMI_CENTER_SOFTKEY);
}
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 

/***************************************************************************** 
* Common List Related Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  resize_fixed_list_to_content_height
 * DESCRIPTION
 *  resize fixed list to content width ,height
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void resize_fixed_list_to_content_height(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_resize_fixed_list_menu(&MMI_fixed_list_menu, MMI_fixed_list_menu.width, MMI_content_height);
}

/***************************************************************************** 
* Check List Control Related Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  toggle_check_list_handle_left_softkey_up
 * DESCRIPTION
 *  Chaneg the string of lsk according to stat of check box of current
 *  highlight item and chaneg the string of RSK to Done and
 *  set the function handler of RSK
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void toggle_check_list_handle_left_softkey_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* get index of current highlight item, */
    S32 i = GetHighlightedItem();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item_checked = 1;   /* some item has been checked or unchecked */

    standard_check_list_handle_item_select(i);
    if (checklist_category_menu_item_states != NULL)
    {
        checklist_category_menu_item_states[i] = get_fixed_twostate_item_state(i);
    }
    /* Item is set to check */
    if (checklist_category_menu_item_states[i] == 1)
    {
        ChangeLeftSoftkey(WGUI_CATEGORY_OFF_STRING_ID, 0);
    }

    /* item is set to off */
    if (checklist_category_menu_item_states[i] == 0)
    {
        ChangeLeftSoftkey(WGUI_CATEGORY_ON_STRING_ID, 0);
    }

    ChangeRightSoftkey(WGUI_CATEGORY_DONE_STRING_ID, 0);
    set_right_softkey_function(checkbox_toggle_DONE_RSK_function, KEY_EVENT_UP);

}


/*****************************************************************************
 * FUNCTION
 *  special_check_list_handle_left_softkey_up
 * DESCRIPTION
 *  Chaneg the string of lsk according to stat of check box of current
 *  highlight item and chaneg the string of RSK to Done and
 *  set the function handler of RSK
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void special_check_list_handle_left_softkey_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = GetHighlightedItem();
    S32 j;
    U8 check_flag;
    U8 uncheck_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item_checked = 1;   /* some item has been checked or unchecked */

    if (i >= 0 && i < (*fixed_twostate_menu_n_items))
    {
        toggle_fixed_twostate_item(i);
        if (checklist_category_menu_item_states != NULL)
        {
            checklist_category_menu_item_states[i] = get_fixed_twostate_item_state(i);
        }
        if (i > 0)
        {
            check_flag = 1;
            uncheck_flag = 1;
            for (j = 1; j < (*fixed_twostate_menu_n_items); j++)
            {
                if (!(MMI_fixed_twostate_menuitems[j].flags & UI_MENUITEM_STATE_SELECTED))
                {
                    check_flag = 0;
                }
                else
                {
                    uncheck_flag = 0;
                }
            }
            if ((check_flag) && (!uncheck_flag))
            {
                MMI_fixed_twostate_menuitems[0].flags |= UI_MENUITEM_STATE_SELECTED;
                if (checklist_category_menu_item_states != NULL)
                {
                    checklist_category_menu_item_states[0] = 1;
                }
            }
            else if ((!check_flag) && (!uncheck_flag))
            {
                MMI_fixed_twostate_menuitems[0].flags &= ~UI_MENUITEM_STATE_SELECTED;
                if (checklist_category_menu_item_states != NULL)
                {
                    checklist_category_menu_item_states[0] = 0;
                }
            }
            else if ((!check_flag) && (uncheck_flag))
            {
                MMI_fixed_twostate_menuitems[0].flags &= ~UI_MENUITEM_STATE_SELECTED;
                checklist_category_menu_item_states[0] = 0;
            }
        }
    }
    if (i == 0)
    {
        check_flag = get_fixed_twostate_item_state(0);
        if (check_flag)
        {
            if (checklist_category_menu_item_states != NULL)
            {
                for (j = 0; j < (*fixed_twostate_menu_n_items); j++)
                {
                    MMI_fixed_twostate_menuitems[j].flags |= UI_MENUITEM_STATE_SELECTED;
                    checklist_category_menu_item_states[j] = 1;
                }
            }
            else
            {
                for (j = 0; j < (*fixed_twostate_menu_n_items); j++)
                {
                    MMI_fixed_twostate_menuitems[j].flags |= UI_MENUITEM_STATE_SELECTED;
                }
            }
        }
        else
        {
            if (checklist_category_menu_item_states != NULL)
            {
                for (j = 0; j < (*fixed_twostate_menu_n_items); j++)
                {
                    MMI_fixed_twostate_menuitems[j].flags &= ~UI_MENUITEM_STATE_SELECTED;
                    checklist_category_menu_item_states[j] = 0;
                }
            }
            else
            {
                for (j = 0; j < (*fixed_twostate_menu_n_items); j++)
                {
                    MMI_fixed_twostate_menuitems[j].flags &= ~UI_MENUITEM_STATE_SELECTED;
                }
            }
        }
    }
    else
    {
        if (checklist_category_menu_item_states != NULL)
        {
        }
    }
    show_fixed_list();
    if (checklist_category_menu_item_states[i] == 1)
    {
        ChangeLeftSoftkey(WGUI_CATEGORY_OFF_STRING_ID, 0);
    }
    if (checklist_category_menu_item_states[i] == 0)
    {
        ChangeLeftSoftkey(WGUI_CATEGORY_ON_STRING_ID, 0);
    }
    ChangeRightSoftkey(WGUI_CATEGORY_DONE_STRING_ID, 0);
    set_right_softkey_function(checkbox_toggle_DONE_RSK_function, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  toggle_check_list_handle_item_select
 * DESCRIPTION
 *  Display the showrcut number on right iside of title of selected
 *  item in check list screen
 * PARAMETERS
 *  h       [IN]        Index of highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void toggle_check_list_handle_item_select(S32 h)
{   /* S32        i=GetHighlightedItem(); */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (h >= 0 && h < (*fixed_twostate_menu_n_items))
    {
        standard_check_list_handle_item_select(h);

        if (checklist_category_menu_item_states != NULL)
        {
            checklist_category_menu_item_states[h] = get_fixed_twostate_item_state(h);
        }

        if (checklist_category_menu_item_states[h] == 1)
        {
            ChangeLeftSoftkey(WGUI_CATEGORY_OFF_STRING_ID, 0);
        }

        if (checklist_category_menu_item_states[h] == 0)
        {
            ChangeLeftSoftkey(WGUI_CATEGORY_ON_STRING_ID, 0);
        }

        ChangeRightSoftkey(WGUI_CATEGORY_DONE_STRING_ID, 0);
        set_right_softkey_function(checkbox_toggle_DONE_RSK_function, KEY_EVENT_UP);
    }

}


/*****************************************************************************
 * FUNCTION
 *  SetCheckboxToggleRightSoftkeyFunctions
 * DESCRIPTION
 *  SEt the RSK function phandlers of check box screen
 * PARAMETERS
 *  done_function       [IN]        
 *  back_function       [IN]        
 *  S32(?)              [IN]        H:- index of highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void SetCheckboxToggleRightSoftkeyFunctions(void (*done_function) (void), void (*back_function) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    checkbox_toggle_DONE_RSK_function = done_function;
    checkbox_toggle_BACK_RSK_function = back_function;
}


/*****************************************************************************
 * FUNCTION
 *  standard_check_list_handle_item_select
 * DESCRIPTION
 *  Handles the selection of an item in a check list and draw the check list
 *  
 *  Toggles the given item between selected and unselected modes
 * PARAMETERS
 *  h       [IN]        Is the index of the item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_check_list_handle_item_select(S32 h)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (h >= 0 && h < (*fixed_twostate_menu_n_items))
    {
        toggle_fixed_twostate_item(h);
        redraw_fixed_list();
    }
}


/*****************************************************************************
 * FUNCTION
 *  standard_check_list_handle_left_softkey_up
 * DESCRIPTION
 *  Handles Check List Selection/Deselection
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void standard_check_list_handle_left_softkey_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = GetHighlightedItem();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    standard_check_list_handle_item_select(i);
    if (checklist_category_menu_item_states != NULL)
    {
        checklist_category_menu_item_states[i] = get_fixed_twostate_item_state(i);
    }
}

/***************************************************************************** 
* Radio List Control Related Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  radio_list_handle_item_select
 * DESCRIPTION
 *  Handles the selection of an item in a radio list and draw the list again
 *  
 *  Unselects all other items and selects the given item
 * PARAMETERS
 *  h       [IN]        Is the index of the item
 * RETURNS
 *  void
 *****************************************************************************/
void radio_list_handle_item_select(S32 h)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (h >= 0 && h < (*fixed_twostate_menu_n_items))
    {
        for (i = 0; i < (*fixed_twostate_menu_n_items); i++)
        {
            unselect_fixed_twostate_item(i);
        }
        select_fixed_twostate_item(h);
        redraw_fixed_list();
    }
}


/*****************************************************************************
 * FUNCTION
 *  radio_list_handle_item_select_no_draw
 * DESCRIPTION
 *  Handles the selection of an item in a radio list but not draw the list
 *  
 *  Unselects all other items and selects the given item
 * PARAMETERS
 *  h       [IN]        Is the index of the item
 * RETURNS
 *  void
 *****************************************************************************/
void radio_list_handle_item_select_no_draw(S32 h)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (h >= 0 && h < (*fixed_twostate_menu_n_items))
    {
        for (i = 0; i < (*fixed_twostate_menu_n_items); i++)
        {
            unselect_fixed_twostate_item(i);
        }
        select_fixed_twostate_item(h);
    }
}

/***************************************************************************** 
* AsyncDynamic List Control Related Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  get_asyncdynamic_item_text
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  UI_string_type
 *****************************************************************************/
UI_string_type get_asyncdynamic_item_text(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_icontext_list_menuitem_type *item_p = get_asyncdynamic_item_from_buffer(index);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (item_p != NULL)
    {
        return item_p->item_texts[0];
    }

    return NULL;
}


/*****************************************************************************
 * FUNCTION
 *  asyncdynamic_list_highlight_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void asyncdynamic_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_asyncdynamic_item_text(item_index);
    if (!menu_shortcut_handler_display)
    {
        gui_redraw_menu_shortcut();
    }
    MMI_list_highlight_handler(item_index);
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}

 
#ifdef __MMI_UI_HINTS_IN_MENUITEM__


/*****************************************************************************
 * FUNCTION
 *  wgui_get_current_asyncdynamic_pop_up_description_string
 * DESCRIPTION
 *  returns the current popup description string for asyncdynamic menu
 * PARAMETERS
 *  void
 * RETURNS
 *  current_popup_description_string
 *****************************************************************************/
UI_string_type wgui_get_current_asyncdynamic_pop_up_description_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 array_index, index, hints;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (current_fixed_list_menuitem_display_index == -1)
    {
        index = MMI_fixed_list_menu.highlighted_item;
    }
    else
    {
        index = MMI_fixed_list_menu.highlighted_item;
    }
    array_index = (index - asyncdynamic_item_buffer.head_item_index +
                   asyncdynamic_item_buffer.head) & MASK_ASYNCDYNAMIC_ITEMS_BUFF;
    hintDataPtrs[array_index] = hintData[array_index];
    if (asyncdynamic_item_buffer.hint_func)
    {
        hints = asyncdynamic_item_buffer.hint_func(index, (UI_string_type*) & hintDataPtrs[array_index]);
    }
    else
    {
        return NULL;
    }
    if (hints)
    {
        return (UI_string_type) hintDataPtrs[array_index];
    }
    else
    {
        return NULL;
    }
}
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
 

extern UI_pop_up_description_theme MMI_pop_up_description_theme;


/*****************************************************************************
 * FUNCTION
 *  set_asyncdynamic_pop_up_descriptions
 * DESCRIPTION
 *  
 * PARAMETERS
 *  highlighted_item        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_asyncdynamic_pop_up_descriptions(S32 highlighted_item)
{
#if( UI_ENABLE_POP_UP_DESCRIPTIONS )
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_HINT_NUM; i++)
    {
        hintDataPtrs[i] = hintData[i];
    }
    wgui_current_pop_up_description_index = highlighted_item;
    current_pop_up_description_theme = &MMI_pop_up_description_theme;
    current_pop_up_description_theme->text_font = &MMI_default_font;
    gui_create_pop_up_description(&wgui_pop_up_description, 0, 0, 0, 0);
#if( UI_POP_UP_DESCRIPTION_SCROLL_TYPE == UI_POP_UP_DESCRIPTION_SCROLL_TYPE_MARQUEE )
    wgui_pop_up_description.flags |= UI_POP_UP_DESCRIPTION_MARQUEE_SCROLL;
#elif( UI_POP_UP_DESCRIPTION_SCROLL_TYPE == UI_POP_UP_DESCRIPTION_SCROLL_TYPE_TWO_DIRECTION )
    wgui_pop_up_description.flags |= UI_POP_UP_DESCRIPTION_TWO_DIRECTION_SCROLL;
#endif 
    gui_display_pop_up_description = wgui_handle_fixed_text_list_pop_up_description;
     
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    gui_get_current_pop_up_description_string = wgui_get_current_asyncdynamic_pop_up_description_string;
#endif 
     
#endif /* ( UI_ENABLE_POP_UP_DESCRIPTIONS ) */ 
}

/***************************************************************************** 
* Highlight Handler Related Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  RegisterHighlightHandler
 * DESCRIPTION
 *  Registers the highlight handler to be called in menu screens
 * PARAMETERS
 *  f           [IN]        Is a pointer to a highlight handler function
 *  This(?)     [IN]        Is a zero based index of the highlighted menuitem.
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterHighlightHandler(void (*f) (S32 item_index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_list_highlight_handler = f;
    MMI_matrix_highlight_handler = f;
    MMI_circular_3D_highlight_handler = f;
}


/*****************************************************************************
 * FUNCTION
 *  MMI_dummy_highlight_handler
 * DESCRIPTION
 *  Dummy function of highlight handler
 * PARAMETERS
 *  item_index      [IN]        
 *  index(?)        [IN]        Store teh current index
 * RETURNS
 *  Returns the index of the highlighted item(?)
 *****************************************************************************/
void MMI_dummy_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item_index);
}


/*****************************************************************************
 * FUNCTION
 *  ClearHighlightHandler
 * DESCRIPTION
 *  Clears the highlight handler that was previously registered
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ClearHighlightHandler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_list_highlight_handler = MMI_dummy_highlight_handler;
    MMI_matrix_highlight_handler = MMI_dummy_highlight_handler;
    MMI_circular_3D_highlight_handler = MMI_dummy_highlight_handler;
}


/*****************************************************************************
 * FUNCTION
 *  standard_list_highlight_handler
 * DESCRIPTION
 *  System Highlight handler
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    if (!menu_shortcut_handler_display)
    {
        gui_redraw_menu_shortcut();
    }
     
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    gdi_layer_lock_frame_buffer();  /* to avoid flickering when pressing arrow key. */
#endif 
     
    MMI_list_highlight_handler(item_index);
     
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    gdi_layer_unlock_frame_buffer();
    gui_BLT_double_buffer(0, UI_device_height - MMI_BUTTON_BAR_HEIGHT, UI_device_width - 1, UI_device_height - 1);
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
     
#if (UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}


/*****************************************************************************
 * FUNCTION
 *  toggle_check_list_highlight_handler
 * DESCRIPTION
 *  Chaneg the string of lsk according to stat of check box of current
 *  highlight item to on or off
 * PARAMETERS
 *  item_index      [IN]        
 *  item(?)         [IN]        Index :- index of highlight item
 * RETURNS
 *  void
 *****************************************************************************/
S32 category_140_itemselected;
void toggle_check_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    category_140_itemselected = item_index;
#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_lock_double_buffer();
#endif 
    if (!menu_shortcut_handler_display)
    {
        gui_redraw_menu_shortcut();
    }
    MMI_list_highlight_handler(item_index);

    if (checklist_category_menu_item_states != NULL)
    {
        checklist_category_menu_item_states[item_index] = get_fixed_twostate_item_state(item_index);
    }

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_unlock_double_buffer();
#endif 

    if (checklist_category_menu_item_states[item_index] == 0)
    {
        ChangeLeftSoftkey(WGUI_CATEGORY_ON_STRING_ID, 0);
    }
    else if (checklist_category_menu_item_states[item_index] == 1)
    {
        ChangeLeftSoftkey(WGUI_CATEGORY_OFF_STRING_ID, 0);
    }

#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}


/*****************************************************************************
 * FUNCTION
 *  standard_check_list_highlight_handler
 * DESCRIPTION
 *  System Highlight handler
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_check_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    if (!menu_shortcut_handler_display)
    {
        gui_redraw_menu_shortcut();
    }
    MMI_list_highlight_handler(item_index);
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
}


/*****************************************************************************
 * FUNCTION
 *  standard_radio_list_highlight_handler
 * DESCRIPTION
 *  System Highlight handler of check box  list
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_radio_list_highlight_handler(S32 item_index)
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
    if (!menu_shortcut_handler_display)
    {
        gui_redraw_menu_shortcut();
    }
    MMI_list_highlight_handler(item_index);
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
}


/*****************************************************************************
 * FUNCTION
 *  inputbox_radio_list_highlight_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inputbox_radio_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifndef __MMI_RADIO_BUTTON_HIGHLIGHT_NO_SELECT__         
    radio_list_handle_item_select_no_draw(item_index);
#endif 

    MMI_highlighted_item_text = get_item_text(item_index);
    gui_lock_double_buffer();

    MMI_list_highlight_handler(item_index);
    gui_unlock_double_buffer();
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}


/*****************************************************************************
 * FUNCTION
 *  dynamic_list_highlight_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void dynamic_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_dynamic_item_text(item_index);
    if (!menu_shortcut_handler_display)
    {
        gui_redraw_menu_shortcut();
    }
    MMI_list_highlight_handler(item_index);
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}


/*****************************************************************************
 * FUNCTION
 *  standard_stop_watch_list_highlight_handler
 * DESCRIPTION
 *  Stop Watch Highlight handler
 *  
 *  Same as standard list highlight handler. Except it does not
 *  redraw the the title bar,
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_stop_watch_list_highlight_handler(S32 item_index)
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
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
}


/*****************************************************************************
 * FUNCTION
 *  standard_matrix_highlight_handler
 * DESCRIPTION
 *  System Highlight handler of matrix lits
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_matrix_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    gui_lock_double_buffer();
    if (!menu_shortcut_handler_display)
    {
        show_fixed_matrix_bar();
    }
    MMI_matrix_highlight_handler(item_index);
    gui_unlock_double_buffer();
}


/*****************************************************************************
 * FUNCTION
 *  standard_animated_list_highlight_handler
 * DESCRIPTION
 *  System Highlight handler
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_animated_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    gui_lock_double_buffer();
    if (!menu_shortcut_handler_display)
    {
        gui_redraw_menu_shortcut();
    }
    MMI_list_highlight_handler(item_index);
    gui_hide_animations();
    gui_unlock_double_buffer();
}


/*****************************************************************************
 * FUNCTION
 *  standard_animated_matrix_highlight_handler
 * DESCRIPTION
 *  System Highlight handler
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_animated_matrix_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    gui_lock_double_buffer();
    if (!menu_shortcut_handler_display)
    {
        show_fixed_matrix_bar();
    }
    MMI_matrix_highlight_handler(item_index);
    gui_hide_animations();
    gui_unlock_double_buffer();
}


/*****************************************************************************
 * FUNCTION
 *  standard_noshortcut_matrix_highlight_handler
 * DESCRIPTION
 *  System Highlight handler
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_noshortcut_matrix_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    MMI_matrix_highlight_handler(item_index);
}

 


/*****************************************************************************
 * FUNCTION
 *  standard_animated_matrix_highlight_handler_type2
 * DESCRIPTION
 *  System Highlight handler used by Main menu type2.
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
 
//Removed
//#if defined __MMI_LANG_HINDI__ && defined __MMI_LANG_ARABIC__
//S32 ARABIC_DISABLE = 0;
//S32 HINDI_DISABLE = 0;
//extern BOOL  r2lMMIFlag;
//#endif
//Removed
 
void standard_animated_matrix_highlight_handler_type2(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
     
    MMI_mm_animation = get_image(gMainMenuAnimation[item_index]);
     
    MMI_title_string = MMI_highlighted_item_text;
     
    //Removed
    //#if defined __MMI_LANG_HINDI__ && defined __MMI_LANG_ARABIC__
    //      if(UI_HINDI_CHARACTER_UCS2_RANGE(MMI_title_string[0]))
    //      {
    //              HINDI_DISABLE=0;
    //              ARABIC_DISABLE=1;
    //      }
    //      else if(r2lMMIFlag)
    //      {
    //              HINDI_DISABLE=1;
    //              ARABIC_DISABLE=0;
    //      }
    //      else
    //      {
    //              HINDI_DISABLE=0;
    //              ARABIC_DISABLE=0;
    //      }
    //#endif
    //Removed
     
#ifndef __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__
if (!(MMI_fixed_matrix_menu.flags & UI_MATRIX_MENU_FOR_MAINMENU))
{
    draw_title();
}
#endif
	
    MMI_matrix_highlight_handler(item_index);
}

// #endif

 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__


/*****************************************************************************
 * FUNCTION
 *  standard_animated_matrix_highlight_handler_full_background
 * DESCRIPTION
 *  System Highlight handler used by Main menu no titlebar.
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlight item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_animated_matrix_highlight_handler_full_background(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
     
    MMI_mm_animation = get_image(gMainMenuAnimation[item_index]);
     

    if (!inMatrixNoTitleFlag)
    {
        MMI_title_string = MMI_highlighted_item_text;
        draw_title();
    }

    MMI_matrix_highlight_handler(item_index);
}
#endif /* __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__ */ 
 

/***************************************************************************** 
* Popup Description Related Functions.
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  wgui_display_fixed_text_list_pop_up_description
 * DESCRIPTION
 *  Diaply pop up list
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_display_fixed_text_list_pop_up_description(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, yy;
    S32 x = wgui_pop_up_description.x;
    S32 y = wgui_pop_up_description.y;
    S32 width = wgui_pop_up_description.width;
    S32 height = wgui_pop_up_description.height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!wgui_pop_up_description_flag)
    {
        return;
    }
    if (wgui_current_pop_up_description_index < 0 ||
        wgui_pop_up_description_strings[wgui_current_pop_up_description_index].text_strings[0] == NULL ||
        *(wgui_pop_up_description_strings[wgui_current_pop_up_description_index].text_strings[0]) == 0)
    {
        wgui_pop_up_description_flag = 0;
        return;
    }
    if ((wgui_current_pop_up_description_index >= 0) &&
        (wgui_current_pop_up_description_index < wgui_pop_up_descriptions_n_items))
    {
        if (wgui_pop_up_descriptions_n_strings > 0)
        {
            for (i = 0; i < wgui_pop_up_descriptions_n_strings; i++)
            {
                gui_set_pop_up_description_text(
                    &wgui_pop_up_description,
                    i,
                    wgui_pop_up_description_strings[wgui_current_pop_up_description_index]. text_strings[i]);
            }
            if (gui_autoresize_pop_up_description(&wgui_pop_up_description, width, height))
            {
                yy = y + height;
                if ((yy + wgui_pop_up_description.height) > (MMI_fixed_list_menu.y + MMI_fixed_list_menu.height))
                {
                    yy = y - wgui_pop_up_description.height;
                    wgui_pop_up_description.flags |= UI_POP_UP_DESCRIPTION_ABOVE_MENUITEM;
                }
                else
                {
                    wgui_pop_up_description.flags &= ~UI_POP_UP_DESCRIPTION_ABOVE_MENUITEM;
                }
                 
                if (!r2lMMIFlag)
                {
                    gui_move_pop_up_description(
                        &wgui_pop_up_description,
                        x + width - wgui_pop_up_description.width,
                        yy);
                }
                else
                {
                    gui_move_pop_up_description(&wgui_pop_up_description, UI_device_width - width, yy);
                }
                /* CSD end */
                gui_show_pop_up_description(&wgui_pop_up_description);
                gui_pop_up_description_start_scroll(&wgui_pop_up_description);
            }
        }
    }
    wgui_pop_up_description_flag = 0;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_display_dynamic_text_list_pop_up_description
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_display_dynamic_text_list_pop_up_description(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, yy;
    S32 x = wgui_pop_up_description.x;
    S32 y = wgui_pop_up_description.y;
    S32 width = wgui_pop_up_description.width;
    S32 height = wgui_pop_up_description.height;
    S32 n_hints;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!wgui_pop_up_description_flag)
    {
         
        gui_pop_up_description_stop_scroll();
        /* CSD end */
        return;
    }
    if (wgui_current_pop_up_description_index >= 0 && dynamic_item_buffer.hint_func != NULL)
    {
        if ((n_hints = dynamic_item_buffer.hint_func(wgui_current_pop_up_description_index, (UI_string_type*) hintDataPtrs)) > 0)
        {
            wgui_pop_up_description.n_strings = n_hints;
            for (i = 0; i < n_hints; i++)
            {
                wgui_pop_up_description.text_strings[i] = (UI_string_type) hintDataPtrs[i];
            }
            if (gui_autoresize_pop_up_description(&wgui_pop_up_description, width, height))
            {
                yy = y + height;
                if ((yy + wgui_pop_up_description.height) > (MMI_fixed_list_menu.y + MMI_fixed_list_menu.height))
                {
                    yy = y - wgui_pop_up_description.height;
                    wgui_pop_up_description.flags |= UI_POP_UP_DESCRIPTION_ABOVE_MENUITEM;
                }
                else
                {
                    wgui_pop_up_description.flags &= ~UI_POP_UP_DESCRIPTION_ABOVE_MENUITEM;
                }

                 
                if (!r2lMMIFlag)
                {
                    gui_move_pop_up_description(
                        &wgui_pop_up_description,
                        x + width - wgui_pop_up_description.width,
                        yy);
                }
                else
                {
                    gui_move_pop_up_description(&wgui_pop_up_description, UI_device_width - width, yy);
                }
                /* CSD end */
                gui_show_pop_up_description(&wgui_pop_up_description);
                gui_pop_up_description_start_scroll(&wgui_pop_up_description);
            }
        }
    }
    wgui_pop_up_description_flag = 0;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_display_asyncdynamic_text_list_pop_up_description
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_display_asyncdynamic_text_list_pop_up_description(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, yy;
    S32 x = wgui_pop_up_description.x;
    S32 y = wgui_pop_up_description.y;
    S32 width = wgui_pop_up_description.width;
    S32 height = wgui_pop_up_description.height;
    S32 n_hints;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!wgui_pop_up_description_flag)
    {
        return;
    }
    if (wgui_current_pop_up_description_index >= 0 && asyncdynamic_item_buffer.hint_func != NULL)
    {
        if ((n_hints = asyncdynamic_item_buffer.hint_func(
                                                    wgui_current_pop_up_description_index,
                                                    (UI_string_type*) hintDataPtrs)) > 0)
        {
            wgui_pop_up_description.n_strings = n_hints;
            for (i = 0; i < n_hints; i++)
            {
                wgui_pop_up_description.text_strings[i] = (UI_string_type) hintDataPtrs[i];
            }
            if (gui_autoresize_pop_up_description(&wgui_pop_up_description, width, height))
            {
                yy = y + height;
                if ((yy + wgui_pop_up_description.height) > (MMI_fixed_list_menu.y + MMI_fixed_list_menu.height))
                {
                    yy = y - wgui_pop_up_description.height;
                    wgui_pop_up_description.flags |= UI_POP_UP_DESCRIPTION_ABOVE_MENUITEM;
                }
                else
                {
                    wgui_pop_up_description.flags &= ~UI_POP_UP_DESCRIPTION_ABOVE_MENUITEM;
                }

                 
                if (!r2lMMIFlag)
                {
                    gui_move_pop_up_description(
                        &wgui_pop_up_description,
                        x + width - wgui_pop_up_description.width,
                        yy);
                }
                else
                {
                    gui_move_pop_up_description(&wgui_pop_up_description, UI_device_width - width, yy);
                }
                /* CSD end */
                gui_show_pop_up_description(&wgui_pop_up_description);
                gui_pop_up_description_start_scroll(&wgui_pop_up_description);
            }
        }
    }
    wgui_pop_up_description_flag = 0;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_handle_fixed_text_list_pop_up_description
 * DESCRIPTION
 *  set teh x,y width,height flag of pop up
 * PARAMETERS
 *  x           [IN]        Start x position of pop up
 *  y           [IN]        Start y position of pop up
 *  width       [IN]        Width of pop up
 *  height      [IN]        Height of pop up
 * RETURNS
 *  void
 *****************************************************************************/
 
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
extern S32 is_draw_next_frame_list_highlight_effect_value(void);
#endif 
 
void wgui_handle_fixed_text_list_pop_up_description(S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    if (is_draw_next_frame_list_highlight_effect_value() == 1)
    {
        return; /* To restrict the change in the pop up positions */
    }
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
     
    wgui_pop_up_description.x = x;
    wgui_pop_up_description.y = y;
    wgui_pop_up_description.width = width;
    wgui_pop_up_description.height = height;
    wgui_pop_up_description_flag = 1;
}

 
#ifdef __MMI_UI_HINTS_IN_MENUITEM__


/*****************************************************************************
 * FUNCTION
 *  wgui_get_current_pop_up_description_string
 * DESCRIPTION
 *  returns the current popup sescription string
 * PARAMETERS
 *  void
 * RETURNS
 *  current_popup_description_string
 *****************************************************************************/
UI_string_type wgui_get_current_pop_up_description_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (current_fixed_list_menuitem_display_index == -1)
    {
        return wgui_pop_up_description_strings[MMI_fixed_list_menu.highlighted_item].text_strings[0];
    }
    else
    {
        return wgui_pop_up_description_strings[current_fixed_list_menuitem_display_index].text_strings[0];
    }
}
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
 


/*****************************************************************************
 * FUNCTION
 *  set_pop_up_descriptions
 * DESCRIPTION
 *  set popup description of current high light item
 * PARAMETERS
 *  n_strings               [IN]        Number of strings
 *  n_items                 [IN]        Number if items
 *  highlighted_item        [IN]        Highlight item
 * RETURNS
 *  void
 *****************************************************************************/
extern UI_pop_up_description_theme MMI_pop_up_description_theme;

void set_pop_up_descriptions(S32 n_strings, S32 n_items, S32 highlighted_item)
{
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_pop_up_descriptions_n_strings = n_strings;
    wgui_current_pop_up_description_index = highlighted_item;
    wgui_pop_up_descriptions_n_items = n_items;
    current_pop_up_description_theme = &MMI_pop_up_description_theme;
    current_pop_up_description_theme->text_font = &MMI_default_font;
    gui_create_pop_up_description(&wgui_pop_up_description, 0, 0, 0, 0);
#if(UI_POP_UP_DESCRIPTION_SCROLL_TYPE == UI_POP_UP_DESCRIPTION_SCROLL_TYPE_MARQUEE)
    wgui_pop_up_description.flags |= UI_POP_UP_DESCRIPTION_MARQUEE_SCROLL;
#elif(UI_POP_UP_DESCRIPTION_SCROLL_TYPE==UI_POP_UP_DESCRIPTION_SCROLL_TYPE_TWO_DIRECTION)
    wgui_pop_up_description.flags |= UI_POP_UP_DESCRIPTION_TWO_DIRECTION_SCROLL;
#endif 
    gui_set_pop_up_description_n_strings(&wgui_pop_up_description, n_strings);
    gui_display_pop_up_description = wgui_handle_fixed_text_list_pop_up_description;
     
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    gui_get_current_pop_up_description_string = wgui_get_current_pop_up_description_string;
#endif 
     
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}

 
#ifdef __MMI_UI_HINTS_IN_MENUITEM__


/*****************************************************************************
 * FUNCTION
 *  wgui_get_current_dynamic_pop_up_description_string
 * DESCRIPTION
 *  returns the current popup description string for dynamic menu
 * PARAMETERS
 *  void
 * RETURNS
 *  current_popup_description_string
 *****************************************************************************/
UI_string_type wgui_get_current_dynamic_pop_up_description_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 array_index, index, hints;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (current_fixed_list_menuitem_display_index == -1)
    {
        index = MMI_fixed_list_menu.highlighted_item;
    }
    else
    {
        index = MMI_fixed_list_menu.highlighted_item;
    }
    array_index = (index - dynamic_item_buffer.head_item_index + dynamic_item_buffer.head) & MASK_DYNAMIC_ITEMS_BUFF;
    hintDataPtrs[array_index] = hintData[array_index];
    if (dynamic_item_buffer.hint_func)
    {
        hints = dynamic_item_buffer.hint_func(index, (UI_string_type*) & hintDataPtrs[array_index]);
    }
    else
    {
        return NULL;
    }
    if (hints)
    {
        return (UI_string_type) hintDataPtrs[array_index];
    }
    else
    {
        return NULL;
    }

}
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
 


/*****************************************************************************
 * FUNCTION
 *  set_dynamic_pop_up_descriptions
 * DESCRIPTION
 *  
 * PARAMETERS
 *  highlighted_item        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_dynamic_pop_up_descriptions(S32 highlighted_item)
{
#if( UI_ENABLE_POP_UP_DESCRIPTIONS )
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_HINT_NUM; i++)
    {
        hintDataPtrs[i] = hintData[i];
    }
    wgui_current_pop_up_description_index = highlighted_item;
    current_pop_up_description_theme = &MMI_pop_up_description_theme;
    current_pop_up_description_theme->text_font = &MMI_default_font;
    gui_create_pop_up_description(&wgui_pop_up_description, 0, 0, 0, 0);
#if( UI_POP_UP_DESCRIPTION_SCROLL_TYPE == UI_POP_UP_DESCRIPTION_SCROLL_TYPE_MARQUEE )
    wgui_pop_up_description.flags |= UI_POP_UP_DESCRIPTION_MARQUEE_SCROLL;
#elif( UI_POP_UP_DESCRIPTION_SCROLL_TYPE == UI_POP_UP_DESCRIPTION_SCROLL_TYPE_TWO_DIRECTION )
    wgui_pop_up_description.flags |= UI_POP_UP_DESCRIPTION_TWO_DIRECTION_SCROLL;
#endif 
    gui_display_pop_up_description = wgui_handle_fixed_text_list_pop_up_description;
     
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    gui_get_current_pop_up_description_string = wgui_get_current_dynamic_pop_up_description_string;
#endif 
     
#endif /* ( UI_ENABLE_POP_UP_DESCRIPTIONS ) */ 
}


/*****************************************************************************
 * FUNCTION
 *  reset_pop_up_descriptions
 * DESCRIPTION
 *  reset poup descriptions to default values
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_pop_up_descriptions(void)
{
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_pop_up_descriptions_n_strings = 0;
    wgui_current_pop_up_description_index = -1;
    wgui_pop_up_descriptions_n_items = 0;
    gui_display_pop_up_description = UI_dummy_display_pop_up_description;
     
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    gui_get_current_pop_up_description_string = UI_dummy_get_current_pop_up_description_string;
#endif 
     
    gui_pop_up_description_stop_scroll();
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}

#ifdef __MMI_UI_HINT_TOGGLE_TRANSITION__


/*****************************************************************************
 * FUNCTION
 *  wgui_setup_pop_up_description_transition
 * DESCRIPTION
 *  Start transition animation of popup description
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_setup_pop_up_description_transition(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_setup_pop_up_description_transition(&wgui_pop_up_description);
}

#endif /* __MMI_UI_HINT_TOGGLE_TRANSITION__ */ 

/***************************************************************************** 
* Slim List Menu Related Functions.
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  wgui_override_list_menu_slim_style
 * DESCRIPTION
 *  Override the current list menu icon style (Typically used at screen entry function)
 * PARAMETERS
 *  style       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_override_list_menu_slim_style(wgui_list_menu_slim_style_enum style)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_current_list_menu_slim_style = style;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_restore_list_menu_slim_style
 * DESCRIPTION
 *  Override the current list menu icon style (Typically used at screen entry function)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_restore_list_menu_slim_style(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_current_list_menu_slim_style = WGUI_LIST_MENU_SLIM_STYLE_DEFAULT;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_get_list_menu_icon
 * DESCRIPTION
 *  Return the list menu itm icon according to MMI configuration
 * PARAMETERS
 *  index                   [IN]        
 *  default_image_id        [IN]        
 * RETURNS
 *  image data
 *****************************************************************************/
PU8 wgui_get_list_menu_icon(int index, UI_image_ID_type  default_image_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_list_menu_slim_style_enum style;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_current_list_menu_slim_style == WGUI_LIST_MENU_SLIM_STYLE_DEFAULT)
    {
    #if defined ( __MMI_SLIM_LISTMENU_NUMBER__ )
        style = WGUI_LIST_MENU_SLIM_STYLE_NUMBER;
    #elif defined ( __MMI_SLIM_LISTMENU_NO_ICON__ )
        style = WGUI_LIST_MENU_SLIM_STYLE_NO_ICON;
    #else 
        style = WGUI_LIST_MENU_SLIM_STYLE_DRAW_ICON;
    #endif 
    }
    else
    {
        style = wgui_current_list_menu_slim_style;
    }

    switch (style)
    {
        case WGUI_LIST_MENU_SLIM_STYLE_NUMBER:
#if defined(__PROJECT_GALLITE_C01__)
            if(0xFFFF == default_image_id)
            {
                return get_image(0);
            }
			else if((default_image_id & 0x8000) == 0x8000)
            {
                default_image_id &= 0x7FFF;
                return get_image(default_image_id);            
            }
            else
            {
                return get_image((UI_image_ID_type ) (IMG_GLOBAL_L1 + index));
            }
#else
           return get_image((UI_image_ID_type ) (IMG_GLOBAL_L1 + index));
#endif
        case WGUI_LIST_MENU_SLIM_STYLE_NO_ICON:
            return get_image(0);

        case WGUI_LIST_MENU_SLIM_STYLE_DRAW_ICON:
        default:
            return get_image(default_image_id);
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_get_list_menu_icon_if_not_empty
 * DESCRIPTION
 *  Return the list menu itm icon according to MMI configuration
 *  For WGUI_LIST_MENU_SLIM_STYLE_NUMBER, we only set default icon
 *  if original icon is not emty.
 * PARAMETERS
 *  index                   [IN]        
 *  default_image_id        [IN]        
 * RETURNS
 *  image data
 *****************************************************************************/
PU8 wgui_get_list_menu_icon_if_not_empty(int index, UI_image_ID_type  default_image_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_list_menu_slim_style_enum style;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_current_list_menu_slim_style == WGUI_LIST_MENU_SLIM_STYLE_DEFAULT)
    {
    #if defined ( __MMI_SLIM_LISTMENU_NUMBER__ )
        style = WGUI_LIST_MENU_SLIM_STYLE_NUMBER;
    #elif defined ( __MMI_SLIM_LISTMENU_NO_ICON__ )
        style = WGUI_LIST_MENU_SLIM_STYLE_NO_ICON;
    #else 
        style = WGUI_LIST_MENU_SLIM_STYLE_DRAW_ICON;
    #endif 
    }
    else
    {
        style = wgui_current_list_menu_slim_style;
    }

    switch (style)
    {
        case WGUI_LIST_MENU_SLIM_STYLE_NUMBER:
            /* We cannot simply test default_image_id == 0 because some applications use non-zero image id 
               to indicate empty images. */
            if (get_image(default_image_id))
            {
                return get_image((UI_image_ID_type ) (IMG_GLOBAL_L1 + index));
            }
            else
            {
                return get_image(0);
            }

        case WGUI_LIST_MENU_SLIM_STYLE_NO_ICON:
            return get_image(0);

        case WGUI_LIST_MENU_SLIM_STYLE_DRAW_ICON:
        default:
            return get_image(default_image_id);
    }
}

/***************************************************************************** 
* Slide Control Related Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  wgui_slide_control_set_focus
 * DESCRIPTION
 *  set teh focus on particular slide
 * PARAMETERS
 *  s       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_slide_control_set_focus(slide_control *s)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_current_slide_control = s;
    if (s == &MMI_slide_control[0])
    {
        MMI_current_slide_control_value = MMI_slide_control_value[0];
    }
    else if (s == &MMI_slide_control[1])
    {
        MMI_current_slide_control_value = MMI_slide_control_value[1];
    }
    else if (s == &MMI_slide_control[2])
    {
        MMI_current_slide_control_value = MMI_slide_control_value[2];
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_slide_control
 * DESCRIPTION
 *  show slide control
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_slide_control(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_slide_control_label(MMI_current_slide_control);
    show_slide_control(MMI_current_slide_control);
    show_slide_control_value(MMI_current_slide_control);
    gdi_layer_blt_previous(
        MMI_current_slide_control->x,
        MMI_current_slide_control->y,
        MMI_current_slide_control->x + MMI_current_slide_control->bg_image_width - 1,
        MMI_current_slide_control->y + MMI_current_slide_control->bg_image_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_update_slide_control
 * DESCRIPTION
 *  update slide control slider and value
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_update_slide_control(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_slide_control(MMI_current_slide_control);
    show_slide_control_value(MMI_current_slide_control);
    gdi_layer_blt_previous(
        MMI_current_slide_control->x,
        MMI_current_slide_control->y,
        MMI_current_slide_control->x + MMI_current_slide_control->bg_image_width - 1,
        MMI_current_slide_control->y + MMI_current_slide_control->bg_image_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_slide_control_focus
 * DESCRIPTION
 *  set the foucs on particular slide control
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_slide_control_focus(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_slide_control_focus_area(MMI_current_slide_control);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_hide_slide_control_focus
 * DESCRIPTION
 *  hide slide control
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_hide_slide_control_focus(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    hide_slide_control_focus_area(MMI_current_slide_control);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_slide_control_previous
 * DESCRIPTION
 *  moveto previous slide control on screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_slide_control_previous(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    slide_control_previous(MMI_current_slide_control);
    wgui_update_slide_control();
    *MMI_current_slide_control_value = MMI_current_slide_control->current_value;

    if (MMI_current_slide_control->change_callback)
    {
        MMI_current_slide_control->change_callback(*MMI_current_slide_control_value);
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_slide_control_next
 * DESCRIPTION
 *  moveto next slide control on screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_slide_control_next(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    slide_control_next(MMI_current_slide_control);
    wgui_update_slide_control();
    *MMI_current_slide_control_value = MMI_current_slide_control->current_value;

    if (MMI_current_slide_control->change_callback)
    {
        MMI_current_slide_control->change_callback(*MMI_current_slide_control_value);
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_slide_control_set_value
 * DESCRIPTION
 *  
 * PARAMETERS
 *  value       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_slide_control_set_value(S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_slide_control_value(MMI_current_slide_control, value);
    wgui_update_slide_control();
    *MMI_current_slide_control_value = MMI_current_slide_control->current_value;

    if (MMI_current_slide_control->change_callback)
    {
        MMI_current_slide_control->change_callback(*MMI_current_slide_control_value);
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_slide_control_value
 * DESCRIPTION
 *  show the values of slide control
 * PARAMETERS
 *  x1          [IN]        Start x position
 *  y1          [IN]        Stary y position
 *  x2          [IN]        End x position
 *  y2          [IN]        End y position
 *  value       [IN]        Value shown on slide
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_slide_control_value(S32 x1, S32 y1, S32 x2, S32 y2, S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 w, h;
    color_t c;
    U8 ts[32];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_itoa(value, (UI_string_type) ts, 10);
    gui_reset_clip();
    gui_reset_text_clip();
    gui_set_font(&MMI_default_font);
    w = gui_get_string_width((UI_string_type) ts);
    h = gui_get_character_height();
    /* gui_draw_filled_area(x1,y1,x2,y2,&wgui_slide_value_background_filler); */
    c = gui_color(0, 0, 0);
    gui_set_text_color(c);
    gui_move_text_cursor(x1 + ((x2 - x1) >> 1) - (w >> 1), y1 + ((y2 - y1) >> 1) - (h >> 1));
    gui_print_text((UI_string_type) ts);
    gui_BLT_double_buffer(x1, y1, x2, y2);
}


/*****************************************************************************
 * FUNCTION
 *  show_fixed_matrix_bar
 * DESCRIPTION
 *  Displays the menu bar for the fixed matrix menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_fixed_matrix_bar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c;
    stFontAttribute *f1 = (UI_font_type) (current_MMI_theme->menu_bar_index_font);
    stFontAttribute *f2 = (UI_font_type) (current_MMI_theme->menu_bar_item_font);
    S32 w;
    UI_character_type s[32];
    S32 character_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_push_text_clip();
    gui_set_clip(
        MMI_matrix_bar_x,
        MMI_matrix_bar_y,
        MMI_matrix_bar_x + MMI_matrix_bar_width - 1,
        MMI_matrix_bar_y + MMI_matrix_bar_height - 1);
    gui_set_text_clip(
        MMI_matrix_bar_x,
        MMI_matrix_bar_y,
        MMI_matrix_bar_x + MMI_matrix_bar_width - 1,
        MMI_matrix_bar_y + MMI_matrix_bar_height - 1);
    c.alpha = 100;
    c.r = 255;
    c.g = 255;
    c.b = 255;
    gui_fill_rectangle(
        MMI_matrix_bar_x,
        MMI_matrix_bar_y,
        MMI_matrix_bar_x + MMI_matrix_bar_width - 1,
        MMI_matrix_bar_y + MMI_matrix_bar_height - 1,
        c);
    c.r = 0;
    c.g = 0;
    c.b = 0;
    gui_draw_rectangle(
        MMI_matrix_bar_x,
        MMI_matrix_bar_y,
        MMI_matrix_bar_x + MMI_matrix_bar_width - 1,
        MMI_matrix_bar_y + MMI_matrix_bar_height - 1,
        c);
    c = *current_MMI_theme->menubar_item_background_color;
    gui_fill_rectangle(
        MMI_matrix_bar_x + 20,
        MMI_matrix_bar_y + 2,
        MMI_matrix_bar_x + MMI_matrix_bar_width - 3,
        MMI_matrix_bar_y + MMI_matrix_bar_height - 3,
        c);
    c = *current_MMI_theme->menubar_index_background_color;
    gui_fill_rectangle(
        MMI_matrix_bar_x + 2,
        MMI_matrix_bar_y + 2,
        MMI_matrix_bar_x + 18,
        MMI_matrix_bar_y + MMI_matrix_bar_height - 3,
        c);
    gui_itoa(MMI_menu_shortcut_number, (UI_string_type) s, 10);
    c = *current_MMI_theme->menubar_index_text_color;
    gui_set_text_color(c);
    gui_set_font(f1);
    gui_measure_string((UI_string_type) s, &w, &character_height);
    gui_move_text_cursor(
        MMI_matrix_bar_x + 10 - (w >> 1),
        MMI_matrix_bar_y + ((MMI_matrix_bar_height) >> 1) - (character_height >> 1));
    gui_set_line_height(character_height);
    gui_print_text(s);
    c = *current_MMI_theme->menubar_item_text_color;
    gui_set_text_color(c);
    gui_set_font(f2);
    gui_move_text_cursor(
        MMI_matrix_bar_x + 22,
        MMI_matrix_bar_y + ((MMI_matrix_bar_height) >> 1) - (character_height >> 1));
    gui_print_text(MMI_highlighted_item_text);
    gui_pop_text_clip();
    gui_pop_clip();
}

/***************************************************************************** 
* Title & Scrolling Title Control Related Functions.
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  GetRootTitleIcon
 * DESCRIPTION
 *  Get Root MenuItem TitleIcon Id in mainmenu
 * PARAMETERS
 *  ItemId          [IN]        
 *  MenuItem(?)     [IN]        Id
 * RETURNS
 *  TitleIcon id
 *****************************************************************************/
U16 GetRootTitleIcon(U16 ItemId)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 idx;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    idx = GetRootParentIndex(ItemId);
    if (idx == 0xFF)
    {
        return 0;
    }
    return gMainMenuTitleIcon[idx];
}


/*****************************************************************************
 * FUNCTION
 *  wgui_set_title_bknd_display
 * DESCRIPTION
 *  set the display state of title background
 * PARAMETERS
 *  state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_title_bknd_display(MMI_BOOL state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gMMI_title_bknd_display = state;
}

 


/*****************************************************************************
 * FUNCTION
 *  wgui_set_wallpaper_on_bottom
 * DESCRIPTION
 *  set the flag of wallpaper on bottom
 * PARAMETERS
 *  state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_wallpaper_on_bottom(MMI_BOOL state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gMMI_wallpaper_on_bottom = state;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_reset_wallpaper_on_bottom
 * DESCRIPTION
 *  reset the flag of wallpaper on bottom
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_reset_wallpaper_on_bottom(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gMMI_wallpaper_on_bottom = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_is_wallpaper_on_bottom
 * DESCRIPTION
 *  is the flag of wallpaper on bottom set?
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL
 *****************************************************************************/
MMI_BOOL wgui_is_wallpaper_on_bottom(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gMMI_wallpaper_on_bottom;
}

 

 


/*****************************************************************************
 * FUNCTION
 *  wgui_set_disable_shortcut_display
 * DESCRIPTION
 *  set the flag of shortcut display
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_disable_shortcut_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gMMI_disable_shortcut_display = MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_reset_disable_shortcut_display
 * DESCRIPTION
 *  reset the flag of shortcut display
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_reset_disable_shortcut_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gMMI_disable_shortcut_display = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_is_disable_shortcut_display
 * DESCRIPTION
 *  is the flag of shortcut display set?
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL wgui_is_disable_shortcut_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gMMI_disable_shortcut_display;
}

 

 
#ifdef __MMI_UI_TITLE_TRANSITION__
#ifdef MMI_ON_WIN32
 
#define MMI_UI_TITLE_TRANSITION_R2L_COUNT 5;//UI_device_width;
#define MMI_UI_TITLE_TRANSITION_L2R_COUNT 5;//UI_device_width;
 
#else /* MMI_ON_WIN32 */ 
 
#define MMI_UI_TITLE_TRANSITION_L2R_COUNT 5;//20;
#define MMI_UI_TITLE_TRANSITION_R2L_COUNT 5;//25;
 
#endif /* MMI_ON_WIN32 */ 
wgui_transition_styles MMI_title_transition_style = WGUI_TRANSITION_NONE;
S32 MMI_check_for_transition = 1;

 
 
S32 MMI_title_transition_on_keys = 0;

 
 


/*****************************************************************************
 * FUNCTION
 *  set_title_transition
 * DESCRIPTION
 *  This functions sets the transition style for title
 * PARAMETERS
 *  style       [IN]        The style in which the title should transiate R2L or L2R
 * RETURNS
 *  void
 *****************************************************************************/
void set_title_transition(wgui_transition_styles style)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_transition_style = style;
}


/*****************************************************************************
 * FUNCTION
 *  get_title_transition
 * DESCRIPTION
 *  This functions returns the transition style for title
 * PARAMETERS
 *  void
 * RETURNS
 *  The style in which the title is transiating R2L or L2R
 *****************************************************************************/
wgui_transition_styles get_title_transition(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_title_transition_style;
}


/*****************************************************************************
 * FUNCTION
 *  reset_title_transition
 * DESCRIPTION
 *  This functions resets the transition style for title to none
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_title_transition(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_transition_style = WGUI_TRANSITION_NONE;
}

 


/*****************************************************************************
 * FUNCTION
 *  set_title_transition_based_on_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_title_transition_based_on_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_transition_on_keys = 1;
    MMI_title_transition_style = WGUI_TRANSITION_R2L;
}

 
 


/*****************************************************************************
 * FUNCTION
 *  is_mmi_title_transition_on_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 is_mmi_title_transition_on_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_title_transition_on_keys;
}


/*****************************************************************************
 * FUNCTION
 *  reset_mmi_title_transition_on_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_mmi_title_transition_on_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_transition_on_keys = 0;
}

 


/*****************************************************************************
 * FUNCTION
 *  draw_transition_title
 * DESCRIPTION
 *  This functions draws the transition title
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_transition_title(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
#if 0
#endif /* 0 */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     

    switch (MMI_title_transition_style)
    {
        case WGUI_TRANSITION_R2L:
        {
            S32 prev_x = MMI_title_x;
            S32 prev_width = MMI_title_width;
            S32 i, transition_count;
            S32 transition_resolution;

             
            i = transition_count = MMI_UI_TITLE_TRANSITION_R2L_COUNT;
             
            transition_resolution = MMI_title_width / transition_count;
            MMI_title_x = MMI_title_x + MMI_title_width - 1;
            while (i - 1)
            {
                MMI_title_x -= transition_resolution;
                override_menu_shortcut_default_region(MMI_title_x, MMI_title_y, MMI_title_width, MMI_title_height);

                draw_title();
                i--;
            }
            MMI_title_width = prev_width;
            MMI_title_x = prev_x;
             
            override_menu_shortcut_default_region(MMI_title_x, MMI_title_y, MMI_title_width, MMI_title_height);
             
        }
            break;
        case WGUI_TRANSITION_L2R:
        {
            S32 prev_x = MMI_title_x;
            S32 prev_width = MMI_title_width;
            S32 i, transition_count;
            S32 transition_resolution;

             
            i = transition_count = MMI_UI_TITLE_TRANSITION_L2R_COUNT;
             
            transition_resolution = MMI_title_width / transition_count;
            MMI_title_x -= MMI_title_width;
            while (i - 1)
            {
                MMI_title_x += transition_resolution;
                override_menu_shortcut_default_region(MMI_title_x, MMI_title_y, MMI_title_width, MMI_title_height);
                draw_title();
                i--;
            }
            MMI_title_width = prev_width;
            MMI_title_x = prev_x;
            override_menu_shortcut_default_region(MMI_title_x, MMI_title_y, MMI_title_width, MMI_title_height);

        }
            break;
    }
}

 
#if(TITLE_TRANSITION_STYLE_NON_BLOCKING)

#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
U8 bitmap_data_for_title_transition[LCD_WIDTH * MAX_TITLE_AREA_HEIGHT_FOR_TRANSITION * 2];      /* for 16 bit */
bitmap g_title_bitmap_for_transition;
S32 prev_menu_shortcut = -1;
#else /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
S32 transition_required = 0;
UI_string_type g_prev_temp_title_string = NULL;
PU8 g_prev_temp_title_icon = NULL;
S32 temp_last_title_shortcut = -1;
#endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
static S32 g_prev_title_x;
static S32 g_in_title_transition = 0;
static S32 title_transition_enable = 0;

/* Store the title string of previous scrren */
UI_string_type g_prev_title_string = NULL;

/* Store the address of title icon display on previous main lcd scrren title bar */
PU8 g_prev_title_icon = NULL;


/*****************************************************************************
 * FUNCTION
 *  wgui_title_tranisition_abort_cleanup_function
 * DESCRIPTION
 *  This functions is for clean up for title transition with timer
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_title_tranisition_abort_cleanup_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_stop_transition_title_non_blocking();
}


/*****************************************************************************
 * FUNCTION
 *  draw_transition_title_non_blocking
 * DESCRIPTION
 *  This functions draws the transition title with timer
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_transition_title_non_blocking(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    static S32 transition_count;
    static S32 transition_resolution;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
    g_title_bitmap_for_transition.data = bitmap_data_for_title_transition;
#endif 
    switch (MMI_title_transition_style)
    {
        case WGUI_TRANSITION_R2L:
        {
            transition_count = MMI_UI_TITLE_TRANSITION_R2L_COUNT;
            if (is_in_nonblocking_title_transition() == 0)
            {
                g_in_title_transition = 1;
            #ifndef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
                draw_previous_title_for_transition();
            #endif 
                transition_resolution = MMI_title_width / transition_count;
                MMI_title_x = MMI_title_x + MMI_title_width - 1;
                gui_add_pre_cleanup_hook(wgui_title_tranisition_abort_cleanup_function);
            }
            MMI_title_x -= transition_resolution;
            move_menu_shortcut_handler(MMI_title_x + MMI_title_width - MMI_menu_shortcut_box.width, MMI_title_y);
            gui_lock_double_buffer();
        #ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
            _show_bitmap((MMI_title_x - MMI_title_width), MMI_title_y, &g_title_bitmap_for_transition);
        #endif 
            draw_title();
            gui_unlock_double_buffer();
            gui_BLT_double_buffer(
                g_prev_title_x,
                MMI_title_y,
                g_prev_title_x + MMI_title_width,
                MMI_title_y + MMI_title_height - 1);
            if ((MMI_title_x - transition_resolution) > 0)
            {
                gui_start_timer(TITLE_TRANSITION_NON_BLOCKING_TIMER, draw_transition_title_non_blocking);
            }
            else
            {
                move_menu_shortcut_handler(MMI_title_width - MMI_menu_shortcut_box.width, MMI_title_y);
            #ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
                if (get_prev_menu_shortcut() != -1)
                {
                    reset_prev_menu_shortcut();
                }
            #endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
                wgui_stop_transition_title_non_blocking();
                gui_BLT_double_buffer(
                    MMI_title_x,
                    MMI_title_y,
                    MMI_title_x + MMI_title_width,
                    MMI_title_y + MMI_title_height - 1);
            }
            break;
        }
        case WGUI_TRANSITION_L2R:
        {
            transition_count = MMI_UI_TITLE_TRANSITION_L2R_COUNT;
            if (is_in_nonblocking_title_transition() == 0)
            {
                g_in_title_transition = 1;
            #ifndef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
                draw_previous_title_for_transition();
            #endif 
                transition_resolution = MMI_title_width / transition_count;
                MMI_title_x -= MMI_title_width + 1;
                gui_add_pre_cleanup_hook(wgui_title_tranisition_abort_cleanup_function);
            }

            MMI_title_x += transition_resolution;
            gui_lock_double_buffer();
        #ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
            _show_bitmap((MMI_title_x + MMI_title_width), MMI_title_y, &g_title_bitmap_for_transition);
        #endif 
            draw_title();
            gui_unlock_double_buffer();
            gui_BLT_double_buffer(
                g_prev_title_x,
                MMI_title_y,
                g_prev_title_x + MMI_title_width,
                MMI_title_y + MMI_title_height - 1);
            if ((MMI_title_x + transition_resolution) < 0)
            {
                gui_start_timer(TITLE_TRANSITION_NON_BLOCKING_TIMER, draw_transition_title_non_blocking);
            }
            else
            {
            #ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
                if (get_prev_menu_shortcut() != -1)
                {
                    reset_prev_menu_shortcut();
                }
            #endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
                wgui_stop_transition_title_non_blocking();
                gui_BLT_double_buffer(
                    MMI_title_x,
                    MMI_title_y,
                    MMI_title_x + MMI_title_width,
                    MMI_title_y + MMI_title_height - 1);
            }
            break;
        }
    }

}


/*****************************************************************************
 * FUNCTION
 *  wgui_stop_transition_title_non_blocking
 * DESCRIPTION
 *  This functions is for stopping the transition title with timer
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_stop_transition_title_non_blocking(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
    S32 temp_menu_shortcut;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(draw_transition_title_non_blocking);
    if (is_in_nonblocking_title_transition() == 1)
    {
        MMI_title_x = g_prev_title_x;
        gui_lock_double_buffer();
    #ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
        if (get_prev_menu_shortcut() != -1)
        {
            temp_menu_shortcut = MMI_menu_shortcut_number;
            MMI_menu_shortcut_number = get_prev_menu_shortcut();
            draw_title();
            MMI_menu_shortcut_number = temp_menu_shortcut;
        }
        else
    #endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
        {
            draw_title();
        }
        gui_unlock_double_buffer();
        g_in_title_transition = 0;
    }
#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
    g_title_bitmap_for_transition.data = NULL;
#else 
    transition_required = 0;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  save_copy_original_title_x
 * DESCRIPTION
 *  This functions is for restoring the original title position.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void save_copy_original_title_x(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_prev_title_x = MMI_title_x;
}


/*****************************************************************************
 * FUNCTION
 *  is_in_nonblocking_title_transition
 * DESCRIPTION
 *  This functions returns the flag for nonblocking effect
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
S32 is_in_nonblocking_title_transition(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_in_title_transition;
}


/*****************************************************************************
 * FUNCTION
 *  enable_title_transition
 * DESCRIPTION
 *  This functions is to enable title transition
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void enable_title_transition(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    title_transition_enable = 1;
    save_copy_original_title_x();
    g_prev_title_string = NULL;
    g_prev_title_icon = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  disable_title_transition
 * DESCRIPTION
 *  This functions is to disable title transition
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void disable_title_transition(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    title_transition_enable = 0;
    reset_title_transition();
    reset_mmi_title_transition_on_keys();
}


/*****************************************************************************
 * FUNCTION
 *  is_enable_title_transition
 * DESCRIPTION
 *  This functions is to check title transition enable/disable
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
S32 is_enable_title_transition(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return title_transition_enable;
}

#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT


/*****************************************************************************
 * FUNCTION
 *  get_bitmap_for_title_transition
 * DESCRIPTION
 *  This functions is for getting the bitmap for previous title
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void get_bitmap_for_title_transition(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_title_bitmap_for_transition.data = bitmap_data_for_title_transition;
    _get_bitmap(
        g_prev_title_x,
        MMI_title_y,
        g_prev_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1,
        &g_title_bitmap_for_transition);
}


/*****************************************************************************
 * FUNCTION
 *  get_prev_menu_shortcut
 * DESCRIPTION
 *  This functions is to get previous menu shortcut value
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
S32 get_prev_menu_shortcut(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return prev_menu_shortcut;
}


/*****************************************************************************
 * FUNCTION
 *  reset_prev_menu_shortcut
 * DESCRIPTION
 *  This functions is to reset prev_menu_shortcut value
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_prev_menu_shortcut(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    prev_menu_shortcut = -1;
}


/*****************************************************************************
 * FUNCTION
 *  set_prev_menu_shortcut
 * DESCRIPTION
 *  This functions is to set prev_menu_shortcut value
 * PARAMETERS
 *  menu_shortcut_index     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_prev_menu_shortcut(S32 menu_shortcut_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    prev_menu_shortcut = menu_shortcut_index;
}
#else /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 


/*****************************************************************************
 * FUNCTION
 *  set_previous_title_shortcut
 * DESCRIPTION
 *  
 * PARAMETERS
 *  shortcut_no     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_previous_title_shortcut(S32 shortcut_no)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    temp_last_title_shortcut = shortcut_no;
}


/*****************************************************************************
 * FUNCTION
 *  draw_previous_title_for_transition
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_previous_title_for_transition(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 temp_no = -1;
    wgui_transition_styles temp_transition_style = WGUI_TRANSITION_NONE;
    UI_string_type temp_title_string = NULL;
    PU8 temp_title_icon = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    temp_no = MMI_menu_shortcut_number;
    MMI_menu_shortcut_number = temp_last_title_shortcut;
    temp_title_string = MMI_title_string;
    temp_title_icon = MMI_title_icon;
    MMI_title_string = g_prev_temp_title_string;
    MMI_title_icon = g_prev_temp_title_icon;

    draw_title();

    MMI_menu_shortcut_number = temp_no;
    MMI_title_string = temp_title_string;
    MMI_title_icon = temp_title_icon;
}
#endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
#endif /* (TITLE_TRANSITION_STYLE_NON_BLOCKING) */ 
 
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
 

 
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__


/*****************************************************************************
 * FUNCTION
 *  gui_scroll_image_factor
 * DESCRIPTION
 *  return increment in position
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
S32 gui_scroll_image_factor(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return animate_title_move;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_draw_animate_title_image
 * DESCRIPTION
 *  Displays the moving image for moving title
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_draw_animate_title_image(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight, no_frame;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_measure_image(get_image(IMG_TITLE_SCROLL), &iwidth, &iheight);
    if (iheight < y2 - y1)
    {
        y1 = y1 + (((y2 - y1 + 1) - iheight) >> 1);
    }
    no_frame = gui_image_n_frames(get_image(IMG_TITLE_SCROLL));
    if (no_frame == 1)
    {
        gui_show_image(x1 + animate_title_move, y1, get_image(IMG_TITLE_SCROLL));
        if ((animate_title_move >= (MMI_title_x + MMI_title_width - 1)) &&
            title_effect_style == MMI_UI_TITLE_IMAGE_ANIMATE_CONTINOUS)
        {
            animate_title_move = animate_title_move - (MMI_title_x + MMI_title_width - 1) - iwidth;
        }
        else
        {
            animate_title_move = animate_title_move + SHIFT_TITLE_IMAGE;
        }
        gui_start_timer(TITLE_IMAGE_ANIMATION_TIMER, wgui_animate_title_image);
    }
    else
    {
        gui_show_animated_image_frame(x1, y1, get_image(IMG_TITLE_SCROLL), 0);
        if (title_animation_current_frame_count != 0)
        {
            gui_show_animated_image_frame(x1, y1, get_image(IMG_TITLE_SCROLL), (S16) title_animation_current_frame_count);      /* Show frame by frame no. */
        }
        if (title_animation_current_frame_count == no_frame)
        {
            title_animation_current_frame_count = -1;
        }
        gui_start_timer(TITLE_IMAGE_ANIMATION_TIMER, wgui_animate_title_image);
        if (title_effect_style == MMI_UI_TITLE_IMAGE_ANIMATE_ONCE && title_animation_current_frame_count == -1)
        {
            return;
        }
        title_animation_current_frame_count++;
    }
    backup_title_animation_current_frame_count = title_animation_current_frame_count;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_draw_animate_title_image_frame_0
 * DESCRIPTION
 *  Displays the first frame of moving image for moving title
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_draw_animate_title_image_frame_0(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight, no_frame;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_measure_image(get_image(IMG_TITLE_SCROLL), &iwidth, &iheight);
    if (iheight < y2 - y1)
    {
        y1 = y1 + (((y2 - y1 + 1) - iheight) >> 1);
    }
    no_frame = gui_image_n_frames(get_image(IMG_TITLE_SCROLL));
    if (no_frame == 1)
    {
        gui_show_image(x1 + animate_title_move, y1, get_image(IMG_TITLE_SCROLL));
    }
    else if (no_frame > 1)
    {
        gui_show_animated_image_frame(x1, y1, get_image(IMG_TITLE_SCROLL), 0);
        if (title_animation_current_frame_count != -1 &&
            (backup_title_animation_current_frame_count != -1 && backup_title_animation_current_frame_count != 0))
        {
            gui_show_animated_image_frame(x1, y1, get_image(IMG_TITLE_SCROLL), backup_title_animation_current_frame_count - 1); /* title_animation_current_frame_count-1); */
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_animate_title_image
 * DESCRIPTION
 *  Call draw title & set reset the next frame variable to check, Internal & external call
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void show_network_detail_on_animated_highlighter_on_idlescreen(void);
void wgui_animate_title_image(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_draw_title_next_frame = 1;
    gui_lock_double_buffer();
    draw_title();
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
     
    if (is_on_idlescreen())
         
    {
        show_network_detail_on_animated_highlighter_on_idlescreen();
    }
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1);
    MMI_draw_title_next_frame = 0;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_stop_animate_title
 * DESCRIPTION
 *  Close the move title timer
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_stop_animate_title(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    animate_title_move = 0;
    backup_title_animation_current_frame_count = title_animation_current_frame_count;
    title_animation_current_frame_count = 0;
    gui_cancel_timer(wgui_animate_title_image);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_start_animate_title
 * DESCRIPTION
 *  Start to move title timer
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_start_animate_title(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_start_timer(TITLE_IMAGE_ANIMATION_TIMER, wgui_animate_title_image);
}


/*****************************************************************************
 * FUNCTION
 *  is_enable_animated_title_effect
 * DESCRIPTION
 *  Tell about enabling or disabling of Title effect.
 * PARAMETERS
 *  void
 * RETURNS
 *  S32 enable_animated_title_effect
 *****************************************************************************/
S32 is_enable_animated_title_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return is_animated_title_effect;
}


/*****************************************************************************
 * FUNCTION
 *  disable_animated_title_effect
 * DESCRIPTION
 *  Disables animation effect
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void disable_animated_title_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    is_animated_title_effect = 0;
}


/*****************************************************************************
 * FUNCTION
 *  enable_animated_title_effect
 * DESCRIPTION
 *  Disables animation effect
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void enable_animated_title_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    is_animated_title_effect = 1;
}

#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
 
 
#ifdef __MMI_TITLE_BAR_NO_ICON_SHCT__
BOOL title_icon_shct = FALSE;
#else 
BOOL title_icon_shct = TRUE;
#endif 


/*****************************************************************************
 * FUNCTION
 *  draw_title_wap
 * DESCRIPTION
 *  Displays the title in wap
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_title_wap(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    title_icon_shct = TRUE;
    draw_title();

#ifdef __MMI_TITLE_BAR_NO_ICON_SHCT__
    title_icon_shct = FALSE;
#endif 
}

 
#if defined(__MMI_MAINLCD_220X176__) || defined(__PROJECT_GALLITE_C01__)
#define CAT222_MAX_TITLE_LEN	256	
static BOOL cat222_title_scrolling = FALSE;
scrolling_text cat222_title_scrolling_text;
UI_character_type g_cat222_title_text[CAT222_MAX_TITLE_LEN];
void cat222_title_text_stop_scrolling(void)
{
	gui_scrolling_text_stop(&cat222_title_scrolling_text);
	cat222_title_scrolling = FALSE;
}

void cat222_scrolling_text_timer_callback(void)
{
	gui_handle_scrolling_text(&cat222_title_scrolling_text);
}

void cat222_draw_scrolling_text_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
	if(cat222_title_scrolling)
	{
		gui_push_clip();
		gui_set_clip(x1,y1,x2,y2);
		//gui_draw_filled_area(x1,y1,x2,y2,&M_filler);
#if !defined(__PROJECT_GALLITE_C01__)
		gdi_image_draw_id(0,0,IMG_NOKIA_E_SERIES_STATUS_BG);
#else
		gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
#endif
		gui_pop_clip();		
	}
	else
	{
		gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
	}
}
#endif
/*****************************************************************************
 * FUNCTION
 *  draw_title
 * DESCRIPTION
 *  Displays the title
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_title(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight, character_height, tx, w, iwidth2;
    UI_font_type saved_font = UI_font;

     
#ifdef __MMI_UI_TECHNO_STATUS_ICON__
    UI_filled_area title_filler;
#endif 
     

     
#ifdef __MMI_UI_TITLE_TRANSITION__
     
    S32 no_title_blt = 0;

     
     
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__PROJECT_GALLITE_C01__)
	title_icon_shct = FALSE;
#endif

#ifdef __MMI_SCREEN_ROTATE__
if(mmi_frm_is_screen_width_height_swapped())
{
	if((gOnFullScreen & MMI_IN_FULL_SCREEN) && GetActiveScreenId() != SCR_ID_AUDPLY_MAIN)
		return;
	#if 0
	ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH,IMG_SI_SIGNAL_STRENGTH_ROTATED);
	ChangeStatusIconImage(STATUS_ICON_BATTERY_STRENGTH,IMG_SI_BATTERY_STRENGTH_ROTATED); 
	
    MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].x = MMI_ROTATED_SIG_X;
    MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].y = NOKIA_STYLE_ROTATED_SIG_Y;
    MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].x = NOKIA_STYLE_ROTATED_BAT_X;
    MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].y = NOKIA_STYLE_ROTATED_BAT_Y;
	#endif
    //FIXME:Check this condition judgement!
    if((!is_on_idlescreen()&&
            (MMI_title_string == get_string(STR_CHARGER_CONNECTED)))
            || ((GetActiveScreenId() != POPUP_SCREENID)
            && (GetActiveScreenId()!= IDLE_SCREEN_SCREENSAVER_ID))
      )

	draw_rotated_title();
	show_status_icon_bar(0);
	draw_rotated_statusbar();
	return;
}
#endif

#if (defined (__MMI_UI_DALMATIAN_STATUSBAR__) && (defined (__MMI_MAINLCD_176X220__)||defined(__MMI_MAINLCD_220X176__)))
     
    if (is_on_idlescreen() == 1)
         
    {
        MMI_title_y = 0;
    }
    else
    {
        MMI_title_y = MMI_STATUS_BAR_HEIGHT;
    }
#endif /* (defined (__MMI_UI_DALMATIAN_STATUSBAR__) && (defined (__MMI_MAINLCD_176X220__)||defined(__MMI_MAINLCD_220X176__))) */ 
     

     
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
    if (MMI_draw_title_next_frame == 0 && is_enable_animated_title_effect())
    {
        wgui_stop_animate_title();
    }
#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
     

     
#ifdef __MMI_UI_TITLE_TRANSITION__
     
     
#if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
    if (is_enable_title_transition() && is_in_nonblocking_title_transition() == 0)
    {
        if (g_prev_title_string == NULL && g_prev_title_icon == NULL)
        {
            g_prev_title_string = MMI_title_string;
            g_prev_title_icon = MMI_title_icon;
        }
        else if (g_prev_title_string != MMI_title_string || g_prev_title_icon != MMI_title_icon)
        {
        #ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
            get_bitmap_for_title_transition();
        #else /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
            transition_required = 1;
            g_prev_temp_title_string = g_prev_title_string;
            g_prev_temp_title_icon = g_prev_title_icon;
        #endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
            g_prev_title_string = MMI_title_string;
            g_prev_title_icon = MMI_title_icon;
        }
    }
#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
    if (is_in_nonblocking_title_transition() == 0 && g_title_bitmap_for_transition.data != NULL)
#else 
    if (is_in_nonblocking_title_transition() == 0 && transition_required == 1)
#endif 
    {
#endif /* (TITLE_TRANSITION_STYLE_NON_BLOCKING) */ 
         
        if (MMI_check_for_transition && MMI_title_transition_style)
        {
             
            if (MMI_title_transition_on_keys)
            {
                U16 keyCode, keyType;

                GetkeyInfo(&keyCode, &keyType);
                if (keyCode == KEY_RIGHT_ARROW)
                {
                    set_title_transition(WGUI_TRANSITION_R2L);

                    MMI_check_for_transition = 0;
                     
                #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
                    disable_animated_title_effect();
                #endif 
                     
                     
                #if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
                    draw_transition_title_non_blocking();
                #else 
                    draw_transition_title();
                #endif 
                     
                     
                #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
                    enable_animated_title_effect();
                #endif 
                     
                    MMI_check_for_transition = 1;
                }
                else if (keyCode == KEY_LEFT_ARROW)
                {
                    set_title_transition(WGUI_TRANSITION_L2R);
                    MMI_check_for_transition = 0;
                     
                #if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
                    draw_transition_title_non_blocking();
                #else 
                    draw_transition_title();
                #endif 
                     
                    MMI_check_for_transition = 1;
                }
                else
                {
                    no_title_blt = 1;
                }
            }
            else
            {
                 
                MMI_check_for_transition = 0;
                 
            #if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
                draw_transition_title_non_blocking();
            #else 
                draw_transition_title();
            #endif 
                 
                MMI_check_for_transition = 1;
            }
             
        }
         
         
    #if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
    }
    else
    {
    #ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
        if (g_title_bitmap_for_transition.data == NULL && is_in_nonblocking_title_transition() == 0 &&
            (MMI_check_for_transition && MMI_title_transition_style))
    #else /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
        if (is_in_nonblocking_title_transition() == 0 && transition_required == 0 &&
            (MMI_check_for_transition && MMI_title_transition_style))
    #endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
        {
            no_title_blt = 1;
        }
    }
    #endif /* (TITLE_TRANSITION_STYLE_NON_BLOCKING) */ 
     
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
     

     
#ifdef __MMI_UI_TECHNO_STATUS_ICON__
     
    title_filler = *(current_UI_theme->window_title_theme->active_filler);
     
    if (GetActiveScreenId() == MAIN_MENU_SCREENID)
    {
        title_filler.flags &= ~UI_FILLED_AREA_TYPE_BITMAP;
        title_filler.flags |= UI_FILLED_AREA_TYPE_COLOR;
         
        title_filler.c = (current_UI_theme->window_title_theme->active_filler->c);
         
    }
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
     

    gui_lock_double_buffer();
    gui_push_clip();
    gui_push_text_clip();
    if ((gOnFullScreen & MMI_IN_FULL_SCREEN) && !(gOnFullScreen & MMI_LEAVE_FULL_IDLE_SCREEN) &&
        !(gOnFullScreen & MMI_IN_NORMAL_SCREEN_WITHOUT_STATUSBAR))
    {
        gui_set_clip(0, 0, UI_device_width - 1, UI_device_height - 1);
        gui_set_text_clip(MMI_title_x, 0, MMI_title_x + MMI_title_width - 1, MMI_title_height - 1);
         
        if (gMMI_title_bknd_display == MMI_TRUE)
        {
            if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
            {
                gdi_draw_solid_rect(MMI_title_x, 0, MMI_title_x + MMI_title_width - 1, MMI_title_height - 1, GDI_COLOR_TRANSPARENT);   
		#if !(defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__))		
                /* 070505 Calvin modified */
                if (title_bg_id != 0)
                {
#if !defined(__PROJECT_GALLITE_C01__)
                    gdi_image_draw_id(MMI_title_x, 0, title_bg_id);
#endif
                }
                else
                {
                     
                    gui_draw_filled_area(
                        MMI_title_x,
                        0,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_height - 1,
                        current_UI_theme->window_title_theme->active_filler);
                     
                }
                 
		#endif
            }
            else
            {
                gdi_draw_solid_rect(MMI_title_x, 0, MMI_title_x + MMI_title_width - 1, MMI_title_height - 1, GDI_COLOR_WHITE);   
                 
            #if defined (__MMI_UI_TECHNO_STATUS_ICON__)
                gui_draw_filled_area(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1,
                    &title_filler);
            #else /* defined (__MMI_UI_TECHNO_STATUS_ICON__) */ 
                gui_draw_filled_area(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1,
                    current_UI_theme->window_title_theme->active_filler);
                 
            #endif /* defined (__MMI_UI_TECHNO_STATUS_ICON__) */ 
                 
            }
             
        #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
            if (is_enable_animated_title_effect())
            {
                if (title_effect_style == MMI_UI_TITLE_IMAGE_ANIMATE_ONCE && title_animation_current_frame_count == -1)
                {
                    wgui_draw_animate_title_image_frame_0(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1);
                }
                else
                {
                    wgui_draw_animate_title_image(
                        MMI_title_x,
                        0,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_height - 1);
                }
            }
        #endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
             

        }
        else
        {
            gdi_draw_solid_rect(
                MMI_title_x,
                0,
                MMI_title_x + MMI_title_width - 1,
                MMI_title_height - 1,
                GDI_COLOR_TRANSPARENT);
        }
         
    }
    else
    {
        gui_set_clip(0, MMI_title_y, UI_device_width - 1, MMI_title_y + MMI_title_height - 1);
        gui_set_text_clip(
            MMI_title_x,
            MMI_title_y,
            MMI_title_x + MMI_title_width - 1,
            MMI_title_y + MMI_title_height - 1);
         
        if (gMMI_title_bknd_display == MMI_TRUE)
        {
            if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
            {
                gdi_draw_solid_rect(MMI_title_x, MMI_title_y, MMI_title_x + MMI_title_width - 1, MMI_title_y + MMI_title_height - 1, GDI_COLOR_TRANSPARENT);    
#if !(defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__))	
				/* 070505 Calvin modified */
                if (title_bg_id != 0)
                {
#if !defined(__PROJECT_GALLITE_C01__)
                    gdi_image_draw_id(MMI_title_x, MMI_title_y, title_bg_id);
#endif
                }
                else
                {
#ifndef __MMI_MAINLCD_220X176__
                     
                    gui_draw_filled_area(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1,
                        current_UI_theme->window_title_theme->active_filler);
#endif
                     
                }
                 
#endif
            }
            else
            {
 #if defined(__MMI_MAINLCD_220X176__)
 		gui_set_clip(MMI_title_x, MMI_title_y, MMI_title_x + MMI_title_width - 1, MMI_title_y + MMI_title_height - 1);
		gdi_image_draw_id(0,0,IMG_NOKIA_E_SERIES_STATUS_BG);
  /*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#elif defined(__MMI_MAINLCD_320X240__)
		gui_set_clip(MMI_title_x, MMI_title_y, MMI_title_x + MMI_title_width - 1, MMI_title_y + MMI_title_height - 1);
		gdi_image_draw_id(0,0,IMG_TITLEBAR_BG);
  /*- zhouqin 2011-02-16 modify for q6 320X240 */	    	
#else
                gdi_draw_solid_rect(MMI_title_x, MMI_title_y, MMI_title_x + MMI_title_width - 1, MMI_title_y + MMI_title_height - 1, GDI_COLOR_WHITE);   

                if (title_bg_id != 0)
                {
#if !defined(__PROJECT_GALLITE_C01__)
                    gdi_image_draw_id(MMI_title_x, MMI_title_y, title_bg_id);
#endif
                }
                else
                {
                     
                #if defined (__MMI_UI_TECHNO_STATUS_ICON__)
                    gui_draw_filled_area(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1,
                        &title_filler);
                #else /* defined (__MMI_UI_TECHNO_STATUS_ICON__) */ 
                    gui_draw_filled_area(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1,
                        current_UI_theme->window_title_theme->active_filler);
                     
                #endif /* defined (__MMI_UI_TECHNO_STATUS_ICON__) */ 
                     
                }
#endif
            }
             
        #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
            if (is_enable_animated_title_effect())
            {
                if (title_effect_style == MMI_UI_TITLE_IMAGE_ANIMATE_ONCE && title_animation_current_frame_count == -1)
                {
                    wgui_draw_animate_title_image_frame_0(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1);
                }
                else
                {
                    wgui_draw_animate_title_image(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1);
                }
            }
        #endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
             
        }
        else
        {
            gdi_draw_solid_rect(
                MMI_title_x,
                MMI_title_y,
                MMI_title_x + MMI_title_width - 1,
                MMI_title_y + MMI_title_height - 1,
                GDI_COLOR_TRANSPARENT);
        }
         
    }
#if defined(__MMI_NOKIA_STYLE_N800__)&&defined(JATAAYU_SUPPORT)
	if(SCR_ID_WAP_HOMEPAGE_SCREEN == GetActiveScreenId())
		title_icon_shct = TRUE;
	else
		title_icon_shct = FALSE;
#endif
    if (title_icon_shct == FALSE)
    {
        MMI_title_icon = NULL;
        MMI_title_icon2 = NULL;
        if (!r2lMMIFlag)
        {
            tx = 2;
        }
        else
        {
            tx = MMI_title_width - 1 - 2;
        }
        MMI_disable_title_shortcut_display = 1;
        disable_menu_shortcut_box_display = 1;
#ifndef  __MMI_NOKIA_STYLE_N800__     
    #ifdef __MMI_TITLE_BAR_NO_ICON_SHCT__
        if (wgui_if_mini_tab_bar_enable())
        {
            gdi_image_get_dimension((U8*) get_image(LEFT_RED_ARROW), &iwidth, &iheight);
            gdi_image_draw_id(tx, (MMI_title_height - iheight) >> 1, LEFT_RED_ARROW);
            gdi_image_draw_id(MMI_title_width - tx - iwidth - 1, (MMI_title_height - iheight) >> 1, RIGHT_RED_ARROW);
        }
    #endif /* __MMI_TITLE_BAR_NO_ICON_SHCT__ */ 
#endif    
    }
    else
    {
        if (MMI_title_icon != NULL)
        {
            if (MMI_title_icon_ani_handle != GDI_ERROR_HANDLE)
            {
                gdi_image_stop_animation(MMI_title_icon_ani_handle);
                MMI_title_icon_ani_handle = GDI_ERROR_HANDLE;
            }
            gdi_image_get_dimension((U8*) MMI_title_icon, &iwidth, &iheight);

            if (!r2lMMIFlag)
            {
            #ifdef __MMI_UI_TECHNO_STATUS_ICON__
                if (GetActiveScreenId() == MAIN_MENU_SCREENID)
                {
                    gdi_image_draw_animation(
                        MMI_title_x + 1,
                        MMI_title_y + ((MMI_title_height) - (iheight)),
                        (U8*) MMI_title_icon,
                        &MMI_title_icon_ani_handle);
                }
                else
            #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
                    gdi_image_draw_animation(
                        MMI_title_x + 1,
                        MMI_title_y + ((MMI_title_height >> 1) - (iheight >> 1)),
                        (U8*) MMI_title_icon,
                        &MMI_title_icon_ani_handle);

                if ((iwidth == 0) || (iheight == 0))
                {
                    tx = 2;
                }
                else
                {
                    tx = iwidth + 3;
                }
            }
            else
            {
            #ifdef __MMI_UI_TECHNO_STATUS_ICON__
                if (GetActiveScreenId() == MAIN_MENU_SCREENID)
                {
                    gdi_image_draw_animation(
                        MMI_title_width - iwidth - 1,
                        MMI_title_y + ((MMI_title_height) - (iheight + TITLE_TEXT_GAP)),
                        (U8*) MMI_title_icon,
                        &MMI_title_icon_ani_handle);
                }
                else
            #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
                    gdi_image_draw_animation(
                        MMI_title_width - iwidth - 1,
                        MMI_title_y + ((MMI_title_height >> 1) - (iheight >> 1)),
                        (U8*) MMI_title_icon,
                        &MMI_title_icon_ani_handle);

                if ((iwidth == 0) || (iheight == 0))
                {
                    tx = MMI_title_width - 1 - 2;
                }
                else
                {
                    tx = MMI_title_width - 1 - iwidth - 3;
                }
            }
        }
        else
        {
            if (!r2lMMIFlag)
            {
                tx = 2;
            }
            else
            {
                tx = MMI_title_width - 1 - 2;
            }
        }

        if (MMI_title_icon2 != NULL)
        {
            gui_measure_image(MMI_title_icon2, &iwidth2, &iheight);
            if (iwidth2 > 0 && iheight > 0)
            {
                if (!r2lMMIFlag)
                {
                    gui_show_transparent_image(
                        MMI_title_x + MMI_title_width - iwidth2 - 1 /* leave one pixel */ ,
                        MMI_title_y + ((MMI_title_height >> 1) - (iheight >> 1)),
                        MMI_title_icon2,
                        gui_transparent_color(0, 0, 0));
                }
                else
                {
                    gui_show_transparent_image(
                        MMI_title_x + 1 /* leave one pixel */ ,
                        MMI_title_y + ((MMI_title_height >> 1) - (iheight >> 1)),
                        MMI_title_icon2,
                        gui_transparent_color(0, 0, 0));
                }
            }
        }
    }

    if (MMI_title_string != NULL)   /* Fix for long titles  */
    {
        S32 tw;

        if (!r2lMMIFlag)
        {
            tw = MMI_title_width - tx;
        }
        else
        {
            tw = tx + 1;
        }
        gui_set_font((UI_font_type) (current_MMI_theme->window_title_font));
        gui_measure_string(MMI_title_string, &w, &character_height);
        w += 2; /* for bordered text display */
         
    #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
        gui_set_text_color(gui_color(255, 255, 255));
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */
    #elif defined __MMI_BLACK_BERRY_MAIN_MENU__
	gui_set_text_color(*current_MMI_theme->LSK_down_text_color);
    /*- zhouqin 2011-02-16 modify for q6 320X240 */
    #else 
	if (GetActiveScreenId() == MAIN_MENU_SCREENID)
         {
        	gui_set_text_color(*current_MMI_theme->LSK_down_text_color);
	}
	else {
		gui_set_text_color(*current_MMI_theme->title_text_color);
	}
    #endif 
         
        if ((gOnFullScreen & MMI_IN_FULL_SCREEN || gOnFullScreen & MMI_LEAVE_FULL_SCREEN) &&
            !(gOnFullScreen & MMI_LEAVE_FULL_IDLE_SCREEN))
        {
            gui_move_text_cursor(MMI_title_x + tx, ((MMI_title_height >> 1) - (character_height >> 1)));
        }
        else
        {
            gui_move_text_cursor(MMI_title_x + tx, MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
        }
        if (MMI_title_icon2 && iwidth2 > 0 && iheight > 0)
        {
            tw -= (iwidth2 + 1 + 4);
            if (w < tw)
            {
                S32 _tx = tx;

                if (!r2lMMIFlag)
                {
                    tx = tx + (tw >> 1) - (w >> 1);
                    if (tx < _tx)
                    {
                        tx = _tx;
                    }
                }
                else
                {
                    tx = tx - ((tw >> 1) - (w >> 1));
                    if (tx > _tx)
                    {
                        tx = _tx;
                    }
                }
            }
        }
         
        //#ifndef __MMI_TITLE_BAR_NO_ICON_SHCT__
        //else if(!MMI_disable_title_shortcut_display && MMI_menu_shortcut_number>=0)
        else if (title_icon_shct == TRUE && (!MMI_disable_title_shortcut_display && MMI_menu_shortcut_number >= 0))
        {
            tw -= MMI_menu_shortcut_box.width;
            if (w < tw)
            {
                S32 _tx = tx;

                 
                if (!r2lMMIFlag)
                {
                    tx = tx + (tw >> 1) - (w >> 1);
                    if (tx < _tx)
                    {
                        tx = _tx;
                    }
                }
                else
                {
                    tx = tx - ((tw >> 1) - (w >> 1));
                    if (tx > _tx)
                    {
                        tx = _tx;
                    }
                }
                /* CSD end */
            }
        }
        // #endif /* __MMI_TITLE_BAR_NO_ICON_SHCT__ */
         
        else
        {
            tw -= MMI_TITLE_PADDING;
            if (w < tw)
            {
                S32 _tx = tx;

                 
                if (!r2lMMIFlag)
                {
                    tx = tx + (tw >> 1) - (w >> 1);
                    if (tx < _tx)
                    {
                        tx = _tx;
                    }
                }
                else
                {
                    tx = tx - ((tw >> 1) - (w >> 1));
                    if (tx > _tx)
                    {
                        tx = _tx;
                    }
                }
                /* CSD end */
            }
        }
    #if defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__)
         

    #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
        gui_set_text_border_color(*current_MMI_theme->title_text_color);
		
   #else 
        gui_set_text_border_color(*current_MMI_theme->title_text_border_color);
    #endif 
         
        if ((gOnFullScreen & MMI_IN_FULL_SCREEN || gOnFullScreen & MMI_LEAVE_FULL_SCREEN) &&
            !(gOnFullScreen & MMI_LEAVE_FULL_IDLE_SCREEN))
        {
             
            //This is required so that when title is in transition truncated text is not displayed.
        #ifdef __MMI_UI_TITLE_TRANSITION__
            if (!MMI_check_for_transition)
            {
                gui_move_text_cursor(MMI_title_x + tx, ((MMI_title_height >> 1) - (character_height >> 1)));
                gui_print_bordered_text(MMI_title_string);
            }
            else
        #endif /* __MMI_UI_TITLE_TRANSITION__ */ 
                 
                 
        #ifdef __MMI_UI_TECHNO_STATUS_ICON__
            if (GetActiveScreenId() == MAIN_MENU_SCREENID)
            {
                gui_print_truncated_borderd_text(
                    MMI_title_x + tx,
                    ((MMI_title_height) - (character_height) - TITLE_TEXT_GAP),
                    tw,
                    MMI_title_string);
            }
            else
        #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
                 
            {
                gui_print_truncated_borderd_text(
                    MMI_title_x + tx,
                    ((MMI_title_height >> 1) - (character_height >> 1)),
                    tw,
                    MMI_title_string);
            }
        }
        else
        {
             
        #ifdef __MMI_UI_TITLE_TRANSITION__
            if (!MMI_check_for_transition)
            {
                gui_move_text_cursor(
                    MMI_title_x + tx,
                    MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
                gui_print_bordered_text(MMI_title_string);
            }
            else
        #endif /* __MMI_UI_TITLE_TRANSITION__ */ 
                 
                 
        #ifdef __MMI_UI_TECHNO_STATUS_ICON__
            if (GetActiveScreenId() == MAIN_MENU_SCREENID)
            {
                gui_print_truncated_borderd_text(
                    MMI_title_x + tx,
                    MMI_title_y + ((MMI_title_height) - (character_height) - TITLE_TEXT_GAP),
                    tw,
                    MMI_title_string);
            }
            else
        #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
                 
            {
#if defined(__MMI_MAINLCD_220X176__)
				if(MMI_CATEGORY222_ID == g_dm_data.S32CatId)
				{
					S32 sw,sh;
					memcpy(g_cat222_title_text,MMI_title_string,CAT222_MAX_TITLE_LEN);
					gui_measure_string(g_cat222_title_text,&sw,&sh);
					if(tw<sw)//scrolling title string.
					{
						/*if(cat222_title_scrolling)
						{
							cat222_title_text_stop_scrolling();
							gui_print_truncated_text(
								MMI_title_x + tx,
								MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)),
								tw,
								g_cat222_title_text);
						}*/
						cat222_title_text_stop_scrolling();
						cat222_title_scrolling=TRUE;
						
						gui_create_scrolling_text(
						   &cat222_title_scrolling_text,
						   MMI_title_x + tx,
						   MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)),
						   tw,
						   sh ,
						   g_cat222_title_text,//(UI_string_type)GetString(STR_SMS_TEMPLATE_1),//L"shawshank redemptionhopeisagoodthingandno",//(UI_string_type)get_string(GAME_PUZZLE_BASE+6),//MMI_title_string,
						   cat222_scrolling_text_timer_callback,
						   cat222_draw_scrolling_text_background,
						   gui_color(255,255,255),
						   gui_color(255,255,255));
						gui_show_scrolling_text(&cat222_title_scrolling_text);
					}
					else
					{
						gui_move_text_cursor(MMI_title_x + tx,MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
						gui_set_text_color(gui_color(255,255,255));
						gui_print_text(g_cat222_title_text);
					}
				}
				else
				{
					gui_set_text_color(gui_color(255,255,255));
					gui_set_font(&UI_DEFAULT_FONT);
					gui_print_truncated_text(
						MMI_title_x + tx,
						MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)),
						tw,
						MMI_title_string);
				}
#else
                gui_print_truncated_borderd_text(
                    MMI_title_x + tx,
                    MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)),
                    tw,
                    MMI_title_string);
#endif
            }
        }
    #else /* defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__) */ 
        if ((gOnFullScreen & MMI_IN_FULL_SCREEN || gOnFullScreen & MMI_LEAVE_FULL_SCREEN) &&
            !(gOnFullScreen & MMI_LEAVE_FULL_IDLE_SCREEN))
        {
             
        #ifdef __MMI_UI_TITLE_TRANSITION__
            if (!MMI_check_for_transition)
            {
                gui_move_text_cursor(MMI_title_x + tx, ((MMI_title_height >> 1) - (character_height >> 1)));
                gui_print_bordered_text(MMI_title_string);
            }
            else
        #endif /* __MMI_UI_TITLE_TRANSITION__ */ 
                 
                 
        #ifdef __MMI_UI_TECHNO_STATUS_ICON__
            if (GetActiveScreenId() == MAIN_MENU_SCREENID)
            {
                gui_print_truncated_borderd_text(
                    MMI_title_x + tx,
                    ((MMI_title_height) - (character_height) - TITLE_TEXT_GAP),
                    tw,
                    MMI_title_string);
            }
            else
        #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
                 
            {
                gui_print_truncated_text(
                    MMI_title_x + tx,
                    ((MMI_title_height >> 1) - (character_height >> 1)),
                    tw,
                    MMI_title_string);
            }
        }
        else
        {
             
        #ifdef __MMI_UI_TITLE_TRANSITION__
            if (!MMI_check_for_transition)
            {
                gui_move_text_cursor(
                    MMI_title_x + tx,
                    MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
                gui_print_bordered_text(MMI_title_string);
            }
            else
        #endif /* __MMI_UI_TITLE_TRANSITION__ */ 
                 
                 
        #ifdef __MMI_UI_TECHNO_STATUS_ICON__
            if (GetActiveScreenId() == MAIN_MENU_SCREENID)
            {
                gui_print_truncated_borderd_text(
                    MMI_title_x + tx,
                    MMI_title_y + ((MMI_title_height) - (character_height) - TITLE_TEXT_GAP),
                    tw,
                    MMI_title_string);
            }
            else
        #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
                 
            {
                gui_print_truncated_text(
                    MMI_title_x + tx,
                    MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)),
                    tw,
                    MMI_title_string);
            }
        }
    #endif /* defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__) */ 
    }
    gui_pop_text_clip();
    gui_pop_clip();
    gui_set_font(saved_font);
    gui_unlock_double_buffer();
    if (MMI_title_icon2 == NULL)
    {
         
    #ifdef __MMI_UI_TITLE_TRANSITION__
         
        if (MMI_check_for_transition && !no_title_blt)
             
    #endif /* __MMI_UI_TITLE_TRANSITION__ */ 
             
             

    #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
            if (is_enable_animated_title_effect() && MMI_draw_title_next_frame == 0)
    #endif 
                 
                gui_hide_menu_shortcut();
        show_menu_shortcut();
    }
     
#if defined(__MMI_UI_TITLE_TRANSITION__) && defined(__MMI_UI_TECHNO_STATUS_ICON__)
    if (!MMI_check_for_transition)
    {
        show_signal_battery_icons();
    }
#endif /* defined(__MMI_UI_TITLE_TRANSITION__) && defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
     
     
#ifdef __MMI_UI_TITLE_TRANSITION__
    if (no_title_blt)
    {
        return;
    }
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
     
    if ((gOnFullScreen & MMI_IN_FULL_SCREEN || gOnFullScreen & MMI_LEAVE_FULL_SCREEN) &&
        !(gOnFullScreen & MMI_LEAVE_FULL_IDLE_SCREEN))
    {
        gui_BLT_double_buffer(MMI_title_x, 0, MMI_title_x + MMI_title_width - 1, MMI_title_height - 1);
    }
    else
    {
        gui_BLT_double_buffer(
            MMI_title_x,
            MMI_title_y,
            MMI_title_x + MMI_title_width - 1,
            MMI_title_y + MMI_title_height - 1);
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_stop_title_animation
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_stop_title_animation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_title_icon_ani_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(MMI_title_icon_ani_handle);
        MMI_title_icon_ani_handle = GDI_ERROR_HANDLE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  draw_title_left_align
 * DESCRIPTION
 *  draw title left align
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_title_left_align(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight, character_height, tx;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_push_clip();
    gui_push_text_clip();
    gui_set_clip(0, 0, UI_device_width - 1, UI_device_height - 1);
    gui_set_text_clip(MMI_title_x, MMI_title_y, MMI_title_x + MMI_title_width - 1, MMI_title_y + MMI_title_height - 1);
     
    gui_draw_filled_area(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1,
        current_UI_theme->window_title_theme->active_filler);
     
    tx = 2;
    gui_set_font((UI_font_type) (current_MMI_theme->window_title_font));
    character_height = gui_get_character_height();
    gui_set_text_color(*current_MMI_theme->title_text_color);
    gui_move_text_cursor(MMI_title_x + tx, MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
    if (MMI_title_icon != NULL)
    {
        gui_measure_image(MMI_title_icon, &iwidth, &iheight);
         
        if (!r2lMMIFlag)
        {
            gui_show_transparent_image(
                MMI_title_x + 1,
                MMI_title_y + ((MMI_title_height >> 1) - (iheight >> 1)),
                MMI_title_icon,
                gui_transparent_color(0, 0, 0));
            if ((iwidth == 0) || (iheight == 0))
            {
                tx = 2;
            }
            else
            {
                tx = iwidth + 3;
            }

        }
        else
        {
            gui_show_transparent_image(
                MMI_title_width - iwidth - 1,
                MMI_title_y + ((MMI_title_height >> 1) - (iheight >> 1)),
                MMI_title_icon,
                gui_transparent_color(0, 0, 0));
            if ((iwidth == 0) || (iheight == 0))
            {
                tx = MMI_title_width - 2;
            }
            else
            {
                tx = MMI_title_width - iwidth - 3;
            }

        }
        /* CSD end */
    }
    else
    {
         
        if (!r2lMMIFlag)
        {
            tx = 2;
        }
        else
        {
            tx = MMI_title_width - 2;
        }
        /* CSD end */
    }

    if (MMI_title_string != NULL)   /* Fix for long titles  */
    {
         
        S32 tw;

        if (!r2lMMIFlag)
        {
            tw = MMI_title_width - tx;
        }
        else
        {
            tw = tx;
        }
        /* CSD end */
        if (!MMI_disable_title_shortcut_display && MMI_menu_shortcut_number >= 0)
        {
            tw -= MMI_menu_shortcut_box.width;
        }

        gui_print_truncated_text(
            MMI_title_x + tx,
            MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)),
            tw,
            MMI_title_string);
    }
    gui_pop_text_clip();
    gui_pop_clip();
    gui_unlock_double_buffer();
    show_menu_shortcut();
    gui_BLT_double_buffer(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1);
}

/* For Draw Manager  */


/*****************************************************************************
 * FUNCTION
 *  move_title
 * DESCRIPTION
 *  Change the position of title
 * PARAMETERS
 *  S32X        [IN]        
 *  S32Y        [IN]        
 *  S32x(?)     [IN]        
 *  S32y(?)     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void move_title(S32 S32X, S32 S32Y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_x = S32X;
    MMI_title_y = S32Y;
}

/* Add for Draw Manager */


/*****************************************************************************
 * FUNCTION
 *  resize_title
 * DESCRIPTION
 *  Change the size of title
 * PARAMETERS
 *  S32Width        [IN]        
 *  S32Height       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void resize_title(S32 S32Width, S32 S32Height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_width = S32Width;
    MMI_title_height = S32Height;
}


/*****************************************************************************
 * FUNCTION
 *  ChangeTitleString
 * DESCRIPTION
 *  Chaneg title stribg
 * PARAMETERS
 *  string              [?]         
 *  title_string(?)     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeTitleString(U8 *string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_string = (UI_string_type) string;
}


/*****************************************************************************
 * FUNCTION
 *  ChangeTitleIcon
 * DESCRIPTION
 *  Chaneg title icon
 * PARAMETERS
 *  image_ID        [IN]        
 *  new(?)          [IN]        Image id
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeTitleIcon(U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_icon = (PU8) get_image(image_ID);
}


/*****************************************************************************
 * FUNCTION
 *  ChangeTitle
 * DESCRIPTION
 *  Chaneg title icon and string
 * PARAMETERS
 *  image_ID        [IN]        
 *  string          [IN]        Of title bar
 *  image_id(?)     [IN]        Of title icon
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeTitle(U16 image_ID, U8 *string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_icon = (PU8) get_image(image_ID);
    MMI_title_string = (UI_string_type) string;
}

/* Start scrolling title control */


/*****************************************************************************
 * FUNCTION
 *  draw_scrolling_title
 * DESCRIPTION
 *  Show scrolling title on title bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_scrolling_title(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 width = 0, height, tw = 0, th = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_reset_clip();
    gui_set_clip(MMI_title_x, MMI_title_y, MMI_title_x + MMI_title_width - 1, MMI_title_y + MMI_title_height - 1);       
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    // Draw title image
     
    gui_draw_filled_area(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1,
        current_UI_theme->window_title_theme->active_filler);
     
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
    gui_draw_filled_area(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1,
        current_UI_theme->window_title_theme->active_filler);
     
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
#if !defined(__PROJECT_GALLITE_C01__)
	MMI_title_icon = NULL;
#endif
    if (MMI_title_icon != NULL)
    {
        x1 = MMI_title_x + scrolling_title_icon_x;
        y1 = MMI_title_y + scrolling_title_icon_y;
        x2 = x1 + MMI_title_width - 1;
        y2 = y1 + (MMI_title_y + MMI_title_height) - 1;
        gui_set_clip(x1, y1, x2, y2);
        gui_measure_image(MMI_title_icon, &width, &height);

         
        if (!r2lMMIFlag)
        {
            gui_show_transparent_image(
                MMI_title_x + scrolling_title_icon_x,
                MMI_title_y + scrolling_title_icon_y,
                MMI_title_icon,
                gui_transparent_color(0, 0, 0));
            if ((width == 0) || (height == 0))
            {
                scrolling_title_text.x = 2;
            }
            else
            {
                scrolling_title_text.x = width + 3;
            }

        }
        else
        {
            gui_show_transparent_image(
                MMI_title_width - width - 1,
                MMI_title_y + scrolling_title_icon_y,
                MMI_title_icon,
                gui_transparent_color(0, 0, 0));
            if ((width == 0) || (height == 0))
            {
                scrolling_title_text.x = MMI_title_width - 2;
            }
            else
            {
                scrolling_title_text.x = MMI_title_width - width - 3;
            }

        }
        /* CSD end */
    }
    else
    {
         
        if (!r2lMMIFlag)
        {
            scrolling_title_text.x = 2;
        }
        else
        {
            scrolling_title_text.x = MMI_title_width - 2;
        }
        /* CSD end */
    }

    if (MMI_title_string != NULL && (gui_strlen(MMI_title_string) != 0))
    {
        gui_measure_string(scrolling_title_text.text, &tw, &th);
        if (!MMI_disable_title_shortcut_display && MMI_menu_shortcut_number >= 0)
        {
            scrolling_title_text.width = MMI_title_width - MMI_menu_shortcut_box.width - width;
        }
         
        if (r2lMMIFlag)
        {
            if (tw < scrolling_title_text.width)
            {
                scrolling_title_text.x = ((MMI_title_width - tw) >> 1) + tw - scrolling_title_text.width;
            }
        }
        else
        {
            if (((MMI_title_width - tw) >> 1) > scrolling_title_text.x)
            {
                scrolling_title_text.x = (MMI_title_width - tw) >> 1;
            }
        }
        /* CSD end */

         
    #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
        /* As title has the image in background & scrolling takes long time, so need less scroll time to set. */
        scrolling_title_text.scroll_time = 50;
    #endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
         
        gui_show_scrolling_text(&scrolling_title_text);
    }
    show_menu_shortcut();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  draw_scrolling_title_background
 * DESCRIPTION
 *  Fill the title bar with default color when text is scrolling
 * PARAMETERS
 *  x1      [IN]        Start x,y position of title bar
 *  y1      [IN]        Start x,y position of title bar
 *  x2      [IN]        Start x,y position of title bar
 *  y2      [IN]        Start x,y position of title bar
 * RETURNS
 *  void
 *****************************************************************************/
void draw_scrolling_title_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    gdi_draw_solid_rect(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1,
        GDI_COLOR_TRANSPARENT);    
    gui_draw_filled_area(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1,
        current_UI_theme->window_title_theme->active_filler);
     
}


/*****************************************************************************
 * FUNCTION
 *  scrolling_title_timer_callback
 * DESCRIPTION
 *  scrolling title bat timer callback
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void scrolling_title_timer_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_handle_scrolling_text(&scrolling_title_text);
}


/*****************************************************************************
 * FUNCTION
 *  setup_scrolling_title
 * DESCRIPTION
 *  set all parameters of scrolling title bar like scrolling text,width,icon
 *  scrolling timer call back .
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void setup_scrolling_title(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight, character_height, tx, tw, w;
    S32 scrolling_title_icon_width = 0;
    S32 scrolling_title_icon_height = 0;
    S32 scrolling_title_text_x;
    S32 scrolling_title_text_y;
    S32 scrolling_title_text_width;
    S32 scrolling_title_text_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    scrolling_title_icon_x = -2;
    scrolling_title_icon_y = 0;
    if (MMI_title_icon != NULL)
    {
        gui_measure_image(MMI_title_icon, &iwidth, &iheight);
        scrolling_title_icon_width = iwidth;
        scrolling_title_icon_height = iheight;
        if (scrolling_title_icon_width > (MMI_title_y + MMI_title_height))
        {
            scrolling_title_icon_width = (MMI_title_y + MMI_title_height);
        }
        if (scrolling_title_icon_height > (MMI_title_y + MMI_title_height))
        {
            scrolling_title_icon_height = (MMI_title_y + MMI_title_height);
        }
        scrolling_title_icon_y = ((MMI_title_height - scrolling_title_icon_height) >> 1);
        scrolling_title_icon_x = 1;
    }
    /* micha0602, JL */
    if (MMI_title_string != NULL && (gui_strlen(MMI_title_string) != 0))
    {
        scrolling_title_text_x = scrolling_title_icon_x + scrolling_title_icon_width + 3;
        gui_set_font((UI_font_type) (current_MMI_theme->window_title_font));
        gui_measure_string(MMI_title_string, &w, &character_height);
        tx = scrolling_title_text_x;
        scrolling_title_text_y = MMI_title_y + (MMI_title_height >> 1) - (character_height >> 1);
        tw = MMI_title_width - tx;
        gui_set_text_color(*current_MMI_theme->title_text_color);
        gui_move_text_cursor(MMI_title_x + tx, MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
        if (w < tw)
        {
            S32 _tx = tx;

            tx = (MMI_title_width >> 1) - (w >> 1);
            if (tx < _tx)
            {
                tx = _tx;
            }
        }
        tw = MMI_title_width - tx - 2;
        scrolling_title_text_width = tw;
        scrolling_title_text_height = character_height;
        scrolling_title_text_x = tx;
        /* micha1111, michaSATc3, #212 */
        gui_create_scrolling_text(
            &scrolling_title_text,
            scrolling_title_text_x,
            scrolling_title_text_y,
            scrolling_title_text_width,
            scrolling_title_text_height,
            MMI_title_string,
            scrolling_title_timer_callback,
            draw_scrolling_title_background,
            gui_color(0, 0, 0),
            gui_color(255, 255, 255));
         
    #ifndef __MMI_UI_DALMATIAN_STATUSBAR__
        scrolling_title_text.scroll_pause <<= 1;
    #else 
        scrolling_title_text.scroll_pause >>= 1;    /* to Make scrolling fast */
    #endif 
         
#ifdef __MMI_MAINLCD_220X176__
	scrolling_title_text.text_color = *current_MMI_theme->title_text_color;
#else
    #if defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__)
        scrolling_title_text.flags |= UI_SCROLLING_TEXT_BORDERED_TEXT;
        scrolling_title_text.text_color = *current_MMI_theme->title_text_color;
        scrolling_title_text.text_border_color = *current_MMI_theme->title_text_border_color;
    #else /* defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__) */ 
        scrolling_title_text.text_color = *current_MMI_theme->title_text_color;
    #endif /* defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__) */ 
#endif
    }
}


/*****************************************************************************
 * FUNCTION
 *  close_scrolling_title
 * DESCRIPTION
 *  Stop scrolling title bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void close_scrolling_title(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(scrolling_title_timer_callback);
}

/***************************************************************************** 
* Theme related wrappers
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  MMI_apply_current_theme
 * DESCRIPTION
 *  Applies the current MMI theme to all MMI objects
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void wgui_setup_inline_edit_themes(void);
extern void (*SUBLCD_RedrawCategoryFunction) (void);

void MMI_apply_current_theme(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_set_icontext_button_theme(&MMI_softkeys[MMI_LEFT_SOFTKEY], current_UI_theme->icontext_button_theme);
    gui_set_icontext_button_theme(&MMI_softkeys[MMI_RIGHT_SOFTKEY], current_UI_theme->icontext_button_theme);
    gui_set_fixed_list_menu_theme(&MMI_fixed_list_menu, current_UI_theme->fixed_list_menu_theme);
    gui_set_fixed_text_menuitem_theme(&MMI_fixed_text_menuitem, current_UI_theme->fixed_text_menuitem_theme);
    gui_set_fixed_icontext_menuitem_theme(
        &MMI_fixed_icontext_menuitem,
        current_UI_theme->fixed_icontext_menuitem_theme);
    gui_set_fixed_icontext_list_menuitem_theme(
        &MMI_fixed_icontext_list_menuitem,
        current_UI_theme->fixed_icontext_list_menuitem_theme);
    gui_set_single_line_input_box_theme(&MMI_singleline_inputbox, current_UI_theme->single_line_input_box_theme);
    gui_set_multi_line_input_box_theme(&MMI_multiline_inputbox, current_UI_theme->multi_line_input_box_theme);
    gui_set_fixed_matrix_menu_theme(&MMI_fixed_matrix_menu, current_UI_theme->fixed_matrix_menu_theme);
    gui_set_fixed_list_menu_theme(&MMI_fixed_list_menu, current_UI_theme->fixed_list_menu_theme);
    gui_set_vertical_scrollbar_theme(&MMI_fixed_list_menu.vbar, current_UI_theme->vertical_scrollbar_theme);
    gui_set_vertical_scrollbar_theme(&MMI_multiline_inputbox.vbar, current_UI_theme->vertical_scrollbar_theme);
    gui_set_vertical_scrollbar_theme(&MMI_fixed_matrix_menu.vbar, current_UI_theme->vertical_scrollbar_theme);
    gui_set_horizontal_scrollbar_theme(&MMI_fixed_matrix_menu.hbar, current_UI_theme->horizontal_scrollbar_theme);
    set_MMI_LSK_theme();
    gui_set_icontext_button_theme(&MMI_softkeys[MMI_LEFT_SOFTKEY], current_UI_theme->icontext_button_theme);
    set_MMI_RSK_theme();
    gui_set_icontext_button_theme(&MMI_softkeys[MMI_RIGHT_SOFTKEY], current_UI_theme->icontext_button_theme);
    set_MMI_regular_button_theme();
    set_MMI_multitap_theme();
    gui_set_virtual_keyboard_theme(&MMI_virtual_keyboard, current_UI_theme->virtual_keyboard_theme);

    /* This assumes that MMI_apply_current_theme is called only from the theme setting menu which uses a list menu */
    set_MMI_current_fixed_text_menuitem_theme_list();
    gui_set_fixed_twostate_menuitem_current_theme(&MMI_fixed_twostate_menuitem);
    wgui_setup_inline_edit_themes();
    SUBLCD_RedrawCategoryFunction();
}

/***************************************************************************** 
* Multiline inputbox Related Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  set_multiline_inputbox_change
 * DESCRIPTION
 *  set the change status of multiline inputbox
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_multiline_inputbox_change(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_multi_line_input_box_change(&MMI_multiline_inputbox);
 
    if (MMI_multi_line_inputbox_present)
    {
        set_wgui_inline_list_menu_status(1);
    }
}


/*****************************************************************************
 * FUNCTION
 *  get_mutliline_inputbox_change_status
 * DESCRIPTION
 *  To get The MultiLine Inputbox Change Status at exit handler of application
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
S32 get_mutliline_inputbox_change_status(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_multiline_inputbox.editor_changed;
}


/*****************************************************************************
 * FUNCTION
 *  set_multiline_inputbox_change_callback
 * DESCRIPTION
 *  Set multiline box contents changed
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_multiline_inputbox_change_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_multiline_inputbox.editor_changed = 1;
}


/*****************************************************************************
 * FUNCTION
 *  fixed_matrix_loop_key_handler
 * DESCRIPTION
 *  handle matrix menu key handling ..up /down,
 * PARAMETERS
 *  vkey_code       [IN]        Key code
 *  key_state       [IN]        Key state
 * RETURNS
 *  void
 *****************************************************************************/
void fixed_matrix_loop_key_handler(S32 vkey_code, S32 key_state)
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
        MMI_fixed_matrix_menu.last_highlighted_item = MMI_fixed_matrix_menu.highlighted_item;
        switch (vkey_code)
        {
            case 37:
                fixed_matrix_goto_previous_item();
                break;
            case 39:
                fixed_matrix_goto_next_item();
                break;
            case 38:
                fixed_matrix_goto_previous_row();
                break;
            case 40:
                fixed_matrix_goto_next_row();
                break;
            case 36:
                fixed_matrix_goto_home();
                break;
            case 35:
                fixed_matrix_goto_end();
                break;
            case 33:
                fixed_matrix_goto_previous_page();
                break;
            case 34:
                fixed_matrix_goto_next_page();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  register_fixed_matrix_loop_keys
 * DESCRIPTION
 *  register key handlers of matrix menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_fixed_matrix_loop_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_keyboard_key_handler(fixed_matrix_loop_key_handler);
    SetKeyHandler(fixed_matrix_goto_previous_row, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(fixed_matrix_goto_next_row, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(fixed_matrix_goto_previous_item, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(fixed_matrix_goto_next_item, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

 
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)


/*****************************************************************************
 * FUNCTION
 *  show_signal_battery_icons
 * DESCRIPTION
 *  shows the battery and signal icons in techno UI style
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_signal_battery_icons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    if (GetActiveScreenId() == MAIN_MENU_SCREENID)
    {
        gui_fill_rectangle(
            MMI_SIGNAL_X,
            MMI_SIGNAL_Y,
            MMI_SIGNAL_X + MMI_SIGNAL_WIDTH - 1,
            MMI_SIGNAL_Y + MMI_status_bar_height + MMI_title_height - 1,
              current_UI_theme->window_title_theme->active_filler->c);
         
        gui_fill_rectangle(
            MMI_SIGNAL_X + MMI_SIGNAL_WIDTH + MMI_title_width,
            MMI_SIGNAL_Y,
            MAIN_LCD_device_width - 1,
            MMI_SIGNAL_Y + MMI_status_bar_height + MMI_title_height - 1,
              current_UI_theme->window_title_theme->active_filler->c);
         
    }

     
    if (is_on_idlescreen() == 1 || GetActiveScreenId() == MAIN_MENU_SCREENID)
         
    {
        refresh_fixed_status_icon(STATUS_ICON_SIGNAL_STRENGTH, 0);
        refresh_fixed_status_icon(STATUS_ICON_BATTERY_STRENGTH, 0);
    }
    gui_pop_clip();
}
#endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
 
 
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
S32 force_hints_on_highlight_in_menuitem = 0;
S32 force_icon_on_highlight_only_in_menuitem = 0;
S32 force_all_hints_in_menuitem = 0;


/*****************************************************************************
 * FUNCTION
 *  set_force_all_hints_in_menuitem
 * DESCRIPTION
 *  Forces Hints on all hints within menu item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_force_all_hints_in_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    force_all_hints_in_menuitem = 1;
}


/*****************************************************************************
 * FUNCTION
 *  reset_force_all_hints_in_menuitem
 * DESCRIPTION
 *  Resets Hints on all hints within menu item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_force_all_hints_in_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    force_all_hints_in_menuitem = 0;
}


/*****************************************************************************
 * FUNCTION
 *  is_set_force_all_hints_in_menuitem
 * DESCRIPTION
 *  Tells whether Force Hints is on all hints within menu item
 * PARAMETERS
 *  void
 * RETURNS
 *  TRUE/FALSE
 *****************************************************************************/
S32 is_set_force_all_hints_in_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return force_all_hints_in_menuitem;
}


/*****************************************************************************
 * FUNCTION
 *  set_force_hints_on_highlight_in_menuitem
 * DESCRIPTION
 *  Forces Hints on highlight only within menu item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_force_hints_on_highlight_in_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    force_hints_on_highlight_in_menuitem = 1;
}


/*****************************************************************************
 * FUNCTION
 *  reset_force_hints_on_highlight_in_menuitem
 * DESCRIPTION
 *  Resets Hints on highlight only within menu item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_force_hints_on_highlight_in_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    force_hints_on_highlight_in_menuitem = 0;

}


/*****************************************************************************
 * FUNCTION
 *  is_set_force_hints_on_highlight_in_menuitem
 * DESCRIPTION
 *  Tells whether Force Hints is on highlight only within menu item
 * PARAMETERS
 *  void
 * RETURNS
 *  TRUE/FALSE
 *****************************************************************************/
S32 is_set_force_hints_on_highlight_in_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return force_hints_on_highlight_in_menuitem;
}


/*****************************************************************************
 * FUNCTION
 *  set_force_icon_on_highlight_only_in_menuitem
 * DESCRIPTION
 *  Forces Icon on highlight only within menu item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_force_icon_on_highlight_only_in_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    force_icon_on_highlight_only_in_menuitem = 1;
}


/*****************************************************************************
 * FUNCTION
 *  reset_force_icon_on_highlight_only_in_menuitem
 * DESCRIPTION
 *  Resets Icon on highlight only within menu item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_force_icon_on_highlight_only_in_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    force_icon_on_highlight_only_in_menuitem = 0;
}


/*****************************************************************************
 * FUNCTION
 *  is_set_force_icon_on_highlight_only_in_menuitem
 * DESCRIPTION
 *  Tells whether Force Icon on highlight only within menu item
 * PARAMETERS
 *  void
 * RETURNS
 *  TRUE/FALSE
 *****************************************************************************/
S32 is_set_force_icon_on_highlight_only_in_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return force_icon_on_highlight_only_in_menuitem;
}


/*****************************************************************************
 * FUNCTION
 *  reset_all_force_flags_for_hints
 * DESCRIPTION
 *  Resets all forced hint styles.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_all_force_flags_for_hints(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    force_icon_on_highlight_only_in_menuitem = 0;
    force_all_hints_in_menuitem = 0;
    force_hints_on_highlight_in_menuitem = 0;
}
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
 

MMI_BOOL gMMI_touch_title_buttons = MMI_FALSE;


/*****************************************************************************
 * FUNCTION
 *  wgui_set_touch_title_bar_buttons
 * DESCRIPTION
 *  Sets the flag for showing the buttons in title bar.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_touch_title_bar_buttons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gMMI_touch_title_buttons = MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_reset_touch_title_bar_buttons
 * DESCRIPTION
 *  Resets the flag for showing the buttons in title bar.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_reset_touch_title_bar_buttons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gMMI_touch_title_buttons = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_is_touch_title_bar_buttons
 * DESCRIPTION
 *  Checks for the flag for showing the buttons in title bar.
 * PARAMETERS
 *  void
 * RETURNS
 *  TRUE/FALSE
 *****************************************************************************/
MMI_BOOL wgui_is_touch_title_bar_buttons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (gMMI_touch_title_buttons);
}

#if defined(__MMI_TOUCH_SCREEN__)
/* function pointers for storing the title buttons handlers */
FuncPtr gMMI_touch_title_button1_down_handler;
FuncPtr gMMI_touch_title_button1_up_handler;
FuncPtr gMMI_touch_title_button2_down_handler;
FuncPtr gMMI_touch_title_button2_up_handler;


/*****************************************************************************
 * FUNCTION
 *  register_touch_button_handlers
 * DESCRIPTION
 *  Register the button handlers for UP and DOWN event.
 * PARAMETERS
 *  f1_up       [IN]        UP event handler of 1st button.
 *  f1_down     [IN]        DOWN event handler of 1st button.
 *  f2_up       [IN]        UP event handler of 2nd button.
 *  f2_down     [IN]        DOWN event handler of 2nd button.
 * RETURNS
 *  void
 *****************************************************************************/
void register_touch_button_handlers(FuncPtr f1_up, FuncPtr f1_down, FuncPtr f2_up, FuncPtr f2_down)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gMMI_touch_title_button1_up_handler = f1_up;
    gMMI_touch_title_button1_down_handler = f1_down;
    gMMI_touch_title_button2_up_handler = f2_up;
    gMMI_touch_title_button2_down_handler = f2_down;
}
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

