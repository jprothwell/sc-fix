/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2002
*
*******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 * Wgui_categories_util.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   This file is intends for common utilities for categoires.
 * Author:
 * -------
 * -------
 *==============================================================================
 *     HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef __WGUI_CATEGORIES_UTIL_H__
#define __WGUI_CATEGORIES_UTIL_H__

#include "mmi_features.h"
#include "custmenures.h"
#include "gui_config.h"
#include "wgui.h"
#include "wgui_softkeys.h"
#include "wgui_menu_shortcuts.h"
#include "wgui_fixed_menus.h"
#include "wgui_fixed_menuitems.h"
#include "wgui_inputs.h"
#include "wgui_datetime.h"
#include "wgui_inline_edit.h"
/* For Dynamic menus */
#include "wgui_dynamic_menus.h"
#include "wgui_dynamic_menuitems.h"

#include "wgui_asyncdynamic_menus.h"
#include "wgui_asyncdynamic_menuitems.h"
#include "wgui_categories_defs.h"
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 
/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum
{CONF_NONE = 0,
    CONF_ACTIVE = 1,
    CONF_HOLD = 2
} CATEGORY_CM_CONFERENCE_STATES;

typedef struct _list_menu_category_history
{
    U16 history_ID;
#ifdef __MMI_DICTIONARY__
    S32 highlighted_item;
    S32 first_displayed_item;
    S32 last_displayed_item;
    S32 displayed_items;
#else /* __MMI_DICTIONARY__ */ 
    S16 highlighted_item;
    S16 first_displayed_item;
    S16 last_displayed_item;
    S16 displayed_items;
#endif /* __MMI_DICTIONARY__ */ 
    U32 flags;
    S8 state;   /* CSD 2004-02-13 Wilson, To keep current screen state in history buffer. */
} list_menu_category_history;

typedef struct _list_menu_category_history140
{
    U16 history_ID;
    S16 highlighted_item;
    S16 first_displayed_item;
    S16 last_displayed_item;
    S16 displayed_items;
    U32 flags;
    S32 no_of_items;
    S8 state;   /* CSD 2004-02-18 Wilson, To keep current screen state in history buffer. */
     
    U8 list_of_states[MAX_FIXED_TWOSTATE_MENU_ITEMS];
    //      fixed_twostate_menuitem_type Cat140_MMI_fixed_twostate_menuitems[MAX_FIXED_TWOSTATE_MENU_ITEMS];
     
} list_menu_category_history_cat140;

typedef struct _matrix_menu_category_history
{
    U16 history_ID;
    S16 highlighted_item;
    S16 highlighted_column;
    S16 highlighted_row;
    S16 displayed_columns;
    S16 displayed_rows;
    S16 first_displayed_column;
    S16 first_displayed_row;
    U32 flags;
} matrix_menu_category_history;

typedef struct _object_list_category_history
{
    U16 history_ID;
    S16 index;
} object_list_category_history;

typedef struct _list_matrix_menu_category_history
{
    list_menu_category_history list_menu_history;
    matrix_menu_category_history matrix_menu_history;
} list_matrix_menu_category_history;

#define MMI_MAX_SLIDE_CONTROL 3
typedef struct _slide_control_category_history
{
    U16 n_items;
    U16 history_ID;
    S32 value[MMI_MAX_SLIDE_CONTROL];
    U16 current_focus;
} slide_control_category_history;

/* SLIM Version Menu Style */
typedef enum
{
    WGUI_LIST_MENU_SLIM_STYLE_DEFAULT,  /* By system setting */
    WGUI_LIST_MENU_SLIM_STYLE_NO_ICON,  /* Disable icon display */
    WGUI_LIST_MENU_SLIM_STYLE_NUMBER,   /* Auto numbering */
    WGUI_LIST_MENU_SLIM_STYLE_DRAW_ICON
} wgui_list_menu_slim_style_enum;

 
/* TRANSITION_STYLES */
typedef enum
{
    WGUI_TRANSITION_NONE = 0,   /* No Transition */
    WGUI_TRANSITION_L2R = 1,    /* Lef to Right */
    WGUI_TRANSITION_R2L = 2,    /* Right to Left */
    WGUI_TRANSITION_T2D = 3,    /* Top To Down */
    WGUI_TRANSITION_D2T = 4     /* Down To Top */
} wgui_transition_styles;

 

 
/*   Animted title effect   */
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
#define TITLE_IMAGE_ANIMATION_TIMER 50
#define SHIFT_TITLE_IMAGE 5
typedef enum
{
    MMI_UI_TITLE_IMAGE_ANIMATE_ONCE = 0,    /* Title will animate once */
    MMI_UI_TITLE_IMAGE_ANIMATE_CONTINOUS    /* Title will animate continously */
} wgui_title_effect_styles;
#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
 

#define WGUI_ORDER_TEXT_IMAGE 0x00000000
#define WGUI_ORDER_IMAGE_TEXT 0x00000001

#define WGUI_MAX_POP_UP_DESCRIPTION_STRINGS      MAX_FIXED_ICONTEXT_MENU_ITEMS
#define GDI_TRANSFORM_GRADIENT_COLOR(Color)  gdi_act_color_from_rgb(255,Color.r,Color.g,Color.b),gdi_act_color_from_rgb(255,(Color.r>>1),(Color.g>>1),(Color.b>>1))

 
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
#define TITLE_TEXT_GAP 2
#endif 
 

#define MMI_BEFORE_SCREEN                             0x00000000
#define MMI_LEAVE_FULL_SCREEN                         0x00000001
#define MMI_LEAVE_FULL_IDLE_SCREEN                    0x00000002
#define MMI_IN_FULL_SCREEN                            0x00000004
#define MMI_IN_NORMAL_SCREEN                          0x00000008
#define MMI_IN_NORMAL_SCREEN_WITHOUT_STATUSBAR        0x00000010
#define MMI_IN_IDLE_SCREEN                            0x00000020
#define MMI_LEAVE_BG_ON_BOTTOM_SCREEN                 0x00000040         

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/
extern U32 category_screen_layout_flags;
extern S32 MMI_matrix_bar_height;
extern S32 MMI_matrix_bar_width;
extern S32 MMI_matrix_bar_x;
extern S32 MMI_matrix_bar_y;
extern S32 MMI_title_height;
extern S32 MMI_title_width;
extern S32 MMI_title_x;
extern S32 MMI_title_y;
extern S32 MMI_button_bar_height;
extern S32 MMI_thick_menuitem_height;
extern S32 MMI_menuitem_height;
extern void (*UI_hide_animation_frame_function) (void);

/* high level wrappers  */
extern UI_string_type MMI_title_string;
extern UI_string_type MMI_message_string;
extern PU8 MMI_title_icon;
extern PU8 MMI_title_icon2;
extern PU8 MMI_message_icon;
extern S32 MMI_menu_shortcut_number;
extern UI_string_type MMI_highlighted_item_text;

extern S32 MMI_current_menu_type;

/* extern S32 MMI_main_menu_type; */
extern S16 MMI_current_input_type;

/* extern S16 MMI_saved_input_type; */
extern U8 flag_scroll;
extern U8(*test_change_input_mode) (void);
extern U8 MMI_english_input_mode_flag;
extern stFontAttribute UI_DEFAULT_FONT;
extern void (*MMI_list_highlight_handler) (S32 item_index);
extern void (*MMI_matrix_highlight_handler) (S32 item_index);
extern void (*MMI_circular_3D_highlight_handler) (S32 item_index);

extern void (*wgui_inputbox_RSK_function) (void);

extern void (*ExitCategoryFunction) (void);
extern void (*RedrawCategoryFunction) (void);
extern void (*RedrawSpellingOrCandidateBoxesFunction) (void);
extern U8 *(*GetCategoryHistory) (U8 *history_buffer);
extern S32(*GetCategoryHistorySize) (void);

/* Globals  */
extern gdi_handle animation_handle;

/*** Store the value of height, width , x, y position of title bar ,status icons
popup dialog in global variables these valeu r also #define in wgui.h */

/* Bear - 040228 move init value to wgui.c */
extern S32 MMI_matrix_bar_height;
extern S32 MMI_matrix_bar_x;
extern S32 MMI_matrix_bar_y;
extern S32 MMI_matrix_bar_width;
extern S32 MMI_title_height;
extern S32 MMI_title_width;
extern S32 MMI_title_x;
extern S32 MMI_title_y;
extern S32 MMI_button_bar_height;
extern S32 MMI_menuitem_height;

extern S32 MMI_pop_up_dialog_x;
extern S32 MMI_pop_up_dialog_y;
extern S32 MMI_pop_up_dialog_width;
extern S32 MMI_pop_up_dialog_height;
extern S32 MMI_pop_up_dialog_full_height;

extern U8 wgui_switch_title_with_status_bar;    /* 0: Title, 1: Status icons  */
extern U8 wgui_category_screen_no_buttons;

/* Store the clipping value of current image */
extern S32 wgui_image_clip_x1;
extern S32 wgui_image_clip_y1;
extern S32 wgui_image_clip_x2;
extern S32 wgui_image_clip_y2;

/* Support for displaying animations in Category screens:
   These functions allow the display of a single animated
   icon in any category screen. The position of the icon
   is decided by the category screen                  */
extern PU8 _MMI_animated_icon;
extern U8 *_MMI_animated_icon_name;
extern S32 _MMI_animated_icon_x;
extern S32 _MMI_animated_icon_y;
extern UI_animated_image_handle _MMI_animated_icon_handle;

extern UI_pop_up_description wgui_pop_up_description;

/* store number of strigs of pop up */
extern S32 wgui_pop_up_descriptions_n_strings;

/* store current index of popup */
extern S32 wgui_current_pop_up_description_index;

/* store number of popup */
extern S32 wgui_pop_up_descriptions_n_items;

/* store flag of popup */
extern U8 wgui_pop_up_description_flag;

extern wgui_pop_up_description_string_type wgui_pop_up_description_strings[WGUI_MAX_POP_UP_DESCRIPTION_STRINGS];

extern U8 *checklist_category_menu_item_states;
extern S32 item_checked;
extern void (*checkbox_toggle_DONE_RSK_function) (void);
extern void (*checkbox_toggle_BACK_RSK_function) (void);

/** Store the title string of current main lcd scrren */
extern UI_string_type MMI_title_string;

/** Store the address of title icon display on current main lcd scrren title bar */
extern PU8 MMI_title_icon;

/** Store the address of title icon display on current main lcd scrren title bar */
extern PU8 MMI_title_icon2;

/* Store the scrolling text of title bar */
extern scrolling_text scrolling_title_text;

/* Scrolling title support */
/** global variables store value of x,y,width of title bar text , icon to scroll the text on title bar */
extern S32 scrolling_title_icon_x;
extern S32 scrolling_title_icon_y;

/* Store the message of current multibox dispaly on main lcd */
extern UI_string_type MMI_message_string;
extern UI_string_type MMI_message_string2;

/** Store the address of icon display on content area of current main lcd scrren */
extern PU8 MMI_message_icon;
extern PU8 MMI_message_icon2;

/* store teh current shorcut number display on right side of title bar in case of menu screen */
extern S32 MMI_menu_shortcut_number;

/* Store the text of current highlighted item in menu */
extern UI_string_type MMI_highlighted_item_text;

/* high level wrappers  */
extern PU8 MMI_mm_animation;

extern U8 gMMI_UI_U8_flag_1, gMMI_UI_U8_flag_2;
extern U16 gMMI_UI_U16_var_1, gMMI_UI_U16_var_2;
extern S32 gMMI_UI_S32_var_1, gMMI_UI_S32_var_2, gMMI_UI_S32_var_3, gMMI_UI_S32_var_4;
extern void *gMMI_UI_ptr_1, *gMMI_UI_ptr_2, *gMMI_UI_ptr_3, *gMMI_UI_ptr_4;

/* varbale of type slide_control used in slide control screen */
extern slide_control *MMI_current_slide_control;

/* extern slide_control MMI_slide_control1,MMI_slide_control2; */
extern slide_control MMI_slide_control[MMI_MAX_SLIDE_CONTROL];

/* store current slide control value */
extern S32 *MMI_current_slide_control_value;

#if (0)
#endif /* (0) */ 
extern S32 *MMI_slide_control_value[MMI_MAX_SLIDE_CONTROL];

extern S32 gOnFullScreen;

/* Add for Draw Manager */
extern S32 g_gui_current_catid;

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern U8 *dummy_get_history(U8 *buffer);
extern S32 dummy_get_history_size(void);

extern PU8 get_image(UI_image_ID_type  i);
extern U8 *get_audio(UI_image_ID_type  i, U8 *type, U32 *filelen);   
extern UI_string_type get_string(UI_image_ID_type  i);
extern UI_string_type get_string_lang(UI_image_ID_type  i, S8 *ssc_str);

extern void initialize_category_screens(void);
extern void reset_category_functions(void);
extern void SetCategoryScreenLayout(U32 flags);

extern void wgui_show_animation(void);
extern void wgui_show_transparent_animation(void);
extern void wgui_set_animation(S32 x, S32 y, UI_image_ID_type  img);
extern void wgui_set_animation_y(S32 x, S32 y, UI_image_ID_type  img);
extern void wgui_set_animation_image(S32 x, S32 y, PU8 img);
extern void wgui_set_animation_image_y(S32 x, S32 y, PU8 img);
extern void wgui_measure_set_animation(S32 x, S32 y, S32 *width, S32 *height, UI_image_ID_type  img);
extern void wgui_measure_set_animation_y(S32 x, S32 y, S32 *width, S32 *height, UI_image_ID_type  img);
extern void wgui_measure_set_animation_image(S32 x, S32 y, S32 *width, S32 *height, PU8 img);
extern void wgui_measure_set_animation_image_y(S32 x, S32 y, S32 *width, S32 *height, PU8 img);
extern void wgui_hide_animation(void);

extern void wgui_fill_rectangle_clip(S32 x1, S32 y1, S32 x2, S32 y2, color_t c);
extern void clear_screen(void);
extern void clear_screen_with_color(color_t c);
extern void clear_buttonbar(void);
extern void set_button_bar_height(S32 height);
extern void clear_contentarea(void);
extern void draw_arrow_indication(S8 left, S8 right, S8 up, S8 down);
extern void SetHighlightedItem(S32 item_index);
extern S32 GetHighlightedItem(void);
extern UI_string_type get_item_text(S32 index);
extern UI_string_type get_dynamic_item_text(S32 index);

#ifdef ENABLE_BOTTOM_NAVIGATION_KEYS
extern void gui_show_key_navigation(S32 index);
extern void gui_show_navigation_keys(void);
#endif /* ENABLE_BOTTOM_NAVIGATION_KEYS */ 
extern void DisableCategoryScreenScrollBar(void);
extern void SetupCategoryKeyHandlers(void);

extern void entry_full_screen(void);
extern void leave_full_screen(void);
extern void in_idle_screen(void);
extern void leave_idle_screen(void);
extern MMI_BOOL if_full_screen_hide_status_icons(void);
extern void reset_title_status_icon(void);
extern void close_title_status_icon(void);
extern void show_title_status_icon(void);

extern U8 history_valid(U16 history_ID, U16 category_screen_ID);
extern void change_list_menu_category_history(U8 *history_buffer, S32 new_highlighted_index, S32 new_n_items, U8 flag);
extern void create_list_menu_category_history(
                S32 history_ID,
                U8 *history_buffer,
                S32 new_highlighted_index,
                S32 new_n_items,
                U8 flag);
extern U8 set_list_menu_category_history(U16 history_ID, U8 *history_buffer);
extern void set_list_menu_category_history_state(U16 history_ID, U8 *history_buffer, S8 state);        /* CSD 2004-02-12 Wilson */
extern void change_cat184_list_menu_history_highlighted_item(S32 index, U8 *history_buffer);    
extern void change_cat84_list_menu_history_highlighted_item(S32 index, U8 *history_buffer);
extern void get_list_menu_category_history(U16 history_ID, U8 *history_buffer);
extern S32 get_list_menu_category_history_size(void);
extern S8 get_list_menu_category_history_state(U16 history_ID, U8 *history_buffer);    /* CSD 2004-02-12 Wilson */
extern U8 set_matrix_menu_category_history(U16 history_ID, U8 *history_buffer);
extern void get_matrix_menu_category_history(U16 history_ID, U8 *history_buffer);
extern U8 set_object_list_category_history(U16 history_ID, U8 *history_buffer, S32 *index);
extern void get_object_list_category_history(U16 history_ID, U8 *history_buffer, S32 *index);
extern void get_slide_control_category_history(U16 history_ID, U8 *history_buffer);
extern U8 set_slide_control_category_history(U16 history_ID, U8 *history_buffer);

 
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
extern U8 set_list_matrix_menu_category_history(U16 history_ID, U8 *history_buffer);
extern void get_list_matrix_menu_category_history(U16 history_ID, U8 *history_buffer);
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
 
extern U8 set_dialer_inputbox_category_history(U16 history_ID, U8 *history_buffer);
extern void get_dialer_inputbox_category_history(U16 history_ID, U8 *history_buffer);

extern void wgui_pen_set_curr_menuitem_wait_longpress(void);

extern void wgui_save_key_handlers(void);
extern void wgui_execute_key_handler(S32 key_code, S32 key_event);
extern void wgui_restore_key_handlers(void);

extern void hide_status_icons_bar0(void);
extern void hide_status_icons_bar1(void);

 
extern void hide_status_icons_bar0_by_transparent(void);
extern void wgui_set_wallpaper_on_bottom(MMI_BOOL state);
extern void wgui_reset_wallpaper_on_bottom(void);
extern MMI_BOOL wgui_is_wallpaper_on_bottom(void);

 

 
extern void wgui_set_disable_shortcut_display(void);
extern void wgui_reset_disable_shortcut_display(void);
extern BOOL wgui_is_disable_shortcut_display(void);

 

extern void ShowStatusIconsTitle(void);

extern void show_softkey_background(void);
extern void MMI_hide_left_softkey(void);
extern void MMI_hide_right_softkey(void);
extern void register_default_hide_softkeys(void);
extern void register_default_hide_softkeys_to_transparent(void);
extern void ChangeLeftSoftkey(U16 s, U16 i);
extern void ChangeRightSoftkey(U16 s, U16 i);
extern void EnableLeftSoftkey(U16 s, U16 i);
extern void DisableLeftSoftkey(U16 s, U16 i);
extern void EnableRightSoftkey(U16 s, U16 i);
extern void DisableRightSoftkey(U16 s, U16 i);
extern void SetLeftSoftkeyAlwaysKeyUpByPen(void);
extern void SetRightSoftkeyAlwaysKeyUpByPen(void);
extern void SetLeftSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
extern void SetRightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);

#ifdef __MMI_WGUI_CSK_ENABLE__
extern void ChangeCenterSoftkey(U16 s, U16 i);
extern void EnableCenterSoftkey(U16 s, U16 i);
extern void ResetCenterSoftkey(void);
extern void SetCenterSoftkeyAlwaysKeyUpByPen(void);
extern void SetCenterSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 

 
extern void ChangeLeftSoftkeyByStringLength(UI_string_type s, PU8 i);
extern void ChangeRightSoftkeyByStringLength(UI_string_type s, PU8 i);

extern void ChangeLeftSoftkeyByString(UI_string_type s, PU8 i);
extern void ChangeRightSoftkeyByString(UI_string_type s, PU8 i);

 

extern void resize_fixed_list_to_content_height(void);
extern void toggle_check_list_handle_left_softkey_up(void);
extern void special_check_list_handle_left_softkey_up(void);
extern void toggle_check_list_handle_item_select(S32 h);
void SetCheckboxToggleRightSoftkeyFunctions(void (*done_function) (void), void (*back_function) (void));
extern void standard_check_list_handle_item_select(S32 h);
extern void standard_check_list_handle_left_softkey_up(void);
extern void standard_check_list_handle_left_softkey_up(void);

extern void radio_list_handle_item_select(S32 h);
extern void radio_list_handle_item_select_no_draw(S32 h);

extern UI_string_type get_asyncdynamic_item_text(S32 index);
extern void asyncdynamic_list_highlight_handler(S32 item_index);
extern void set_asyncdynamic_pop_up_descriptions(S32 highlighted_item);

void RegisterHighlightHandler(void (*f) (S32 item_index));
extern void MMI_dummy_highlight_handler(S32 item_index);
extern void ClearHighlightHandler(void);

extern void standard_list_highlight_handler(S32 item_index);
extern void toggle_check_list_highlight_handler(S32 item_index);
extern void standard_check_list_highlight_handler(S32 item_index);
extern void standard_radio_list_highlight_handler(S32 item_index);
extern void inputbox_radio_list_highlight_handler(S32 item_index);
extern void dynamic_list_highlight_handler(S32 item_index);
extern void standard_stop_watch_list_highlight_handler(S32 item_index);
extern void standard_matrix_highlight_handler(S32 item_index);
extern void standard_animated_list_highlight_handler(S32 item_index);
extern void standard_animated_matrix_highlight_handler(S32 item_index);
extern void standard_noshortcut_matrix_highlight_handler(S32 item_index);

 
extern void standard_animated_matrix_highlight_handler_type2(S32 item_index);

// #endif

#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__  
extern void standard_animated_matrix_highlight_handler_full_background(S32 item_index);
#endif 

extern void wgui_display_fixed_text_list_pop_up_description(void);
extern void wgui_display_dynamic_text_list_pop_up_description(void);
extern void wgui_display_asyncdynamic_text_list_pop_up_description(void);
extern void wgui_handle_fixed_text_list_pop_up_description(S32 x, S32 y, S32 width, S32 height);
extern void set_pop_up_descriptions(S32 n_strings, S32 n_items, S32 highlighted_item);
extern void set_dynamic_pop_up_descriptions(S32 highlighted_item);
extern void reset_pop_up_descriptions(void);

#ifdef __MMI_UI_HINT_TOGGLE_TRANSITION__
extern void wgui_setup_pop_up_description_transition(void);
#endif 

extern void wgui_override_list_menu_slim_style(wgui_list_menu_slim_style_enum style);
extern void wgui_restore_list_menu_slim_style(void);
extern PU8 wgui_get_list_menu_icon(int index, UI_image_ID_type  default_image_id);
extern PU8 wgui_get_list_menu_icon_if_not_empty(int index, UI_image_ID_type  default_image_id);

extern void wgui_slide_control_set_focus(slide_control *s);
extern void wgui_show_slide_control(void);
extern void wgui_show_slide_control_focus(void);
extern void wgui_hide_slide_control_focus(void);
extern void wgui_slide_control_previous(void);
extern void wgui_slide_control_next(void);
extern void wgui_show_slide_control_value(S32 x1, S32 y1, S32 x2, S32 y2, S32 value);
extern void wgui_slide_control_set_value(S32 value);

extern void show_fixed_matrix_bar(void);

extern U16 GetRootTitleIcon(U16 ItemId);
extern void draw_title(void);
extern void wgui_stop_title_animation(void);
extern void draw_title_wap(void);    
extern void draw_title_left_align(void);

 
#ifdef __MMI_UI_TITLE_TRANSITION__
extern void set_title_transition_based_on_keys(void);
extern void set_title_transition(wgui_transition_styles style);
extern wgui_transition_styles get_title_transition(void);
extern void reset_title_transition(void);
extern void draw_transition_tite(void);

 
extern S32 is_mmi_title_transition_on_keys(void);

#define TITLE_TRANSITION_STYLE_NON_BLOCKING 1
#define TITLE_TRANSITION_NON_BLOCKING_TIMER 10

#if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
#define TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
#define MAX_TITLE_AREA_HEIGHT_FOR_TRANSITION (MMI_TITLE_HEIGHT + MMI_MENUITEM_HEIGHT)
#endif 
extern void draw_transition_title_non_blocking(void);
extern void wgui_stop_transition_title_non_blocking(void);
extern void save_copy_original_title_x(void);
extern S32 is_in_nonblocking_title_transition(void);
extern void wgui_title_tranisition_abort_cleanup_function(void);
extern void enable_title_transition(void);
extern void disable_title_transition(void);
extern S32 is_enable_title_transition(void);

#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
extern S32 get_prev_menu_shortcut(void);
extern void reset_prev_menu_shortcut(void);
extern void set_prev_menu_shortcut(S32 menu_shortcut_index);
extern void get_bitmap_for_title_transition(void);
#else /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
extern void set_previous_title_shortcut(S32 shortcut_no);
extern void draw_previous_title_for_transition(void);
#endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
#endif /* (TITLE_TRANSITION_STYLE_NON_BLOCKING) */ 
 
#else /* __MMI_UI_TITLE_TRANSITION__ */ 
#define TITLE_TRANSITION_STYLE_NON_BLOCKING 0
#define TITLE_TRANSITION_NON_BLOCKING_TIMER 10
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
 
extern void move_title(S32 S32X, S32 S32Y);
extern void resize_title(S32 S32Width, S32 S32Height);
extern void ChangeTitleString(U8 *string);
extern void ChangeTitleIcon(U16 image_ID);
extern void ChangeTitle(U16 image_ID, U8 *string);
extern void draw_scrolling_title(void);
extern void draw_scrolling_title_background(S32 x1, S32 y1, S32 x2, S32 y2);
extern void scrolling_title_timer_callback(void);
extern void setup_scrolling_title(void);
extern void close_scrolling_title(void);

extern void MMI_apply_current_theme(void);

extern void show_vertical_bar(S32 volume_level);
extern void show_vertical_bar_previous(void);
extern void show_vertical_bar_next(void);

extern void set_multiline_inputbox_change(void);
extern S32 get_mutliline_inputbox_change_status(void);
extern void set_multiline_inputbox_change_callback(void);

extern void fixed_matrix_loop_key_handler(S32 vkey_code, S32 key_state);
extern void register_fixed_matrix_loop_keys(void);

 
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
extern void show_signal_battery_icons(void);
#endif 
 
 
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
extern UI_string_type wgui_get_current_pop_up_description_string(void);

extern void set_force_all_hints_in_menuitem(void);
extern void reset_force_all_hints_in_menuitem(void);
extern S32 is_set_force_all_hints_in_menuitem(void);

extern void set_force_hints_on_highlight_in_menuitem(void);
extern void reset_force_hints_on_highlight_in_menuitem(void);
extern S32 is_set_force_hints_on_highlight_in_menuitem(void);

extern void set_force_icon_on_highlight_only_in_menuitem(void);
extern void reset_force_icon_on_highlight_only_in_menuitem(void);
extern S32 is_set_force_icon_on_highlight_only_in_menuitem(void);

extern void reset_all_force_flags_for_hints(void);
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
 

 
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
extern void wgui_stop_animate_title(void);
extern void wgui_start_animate_title(void);
extern void disable_animated_title_effect(void);
extern void enable_animated_title_effect(void);
extern void wgui_draw_animate_title_image(S32 x1, S32 y1, S32 x2, S32 y2);
extern void wgui_animate_title_image(void);
extern void wgui_draw_animate_title_image_frame_0(S32 x1, S32 y1, S32 x2, S32 y2);
extern S32 is_enable_animated_title_effect(void);
#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
 

 
#if defined(__MMI_TOUCH_SCREEN__)

extern FuncPtr gMMI_touch_title_button1_down_handler;
extern FuncPtr gMMI_touch_title_button1_up_handler;
extern FuncPtr gMMI_touch_title_button2_down_handler;
extern FuncPtr gMMI_touch_title_button2_up_handler;

extern void register_touch_button_handlers(FuncPtr f1_up, FuncPtr f1_down, FuncPtr f2_up, FuncPtr f2_down);
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

extern void wgui_set_touch_title_bar_buttons(void);
extern void wgui_reset_touch_title_bar_buttons(void);
extern MMI_BOOL wgui_is_touch_title_bar_buttons(void);

 
#ifdef __cplusplus
};
#endif /* __cplusplus */ 

#endif /* __WGUI_CATEGORIES_UTIL_H__ */ 

