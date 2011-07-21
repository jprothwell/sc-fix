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
 *   gui_fixed_menuitems.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Menu items in fixed menu - UI component
 *   [Text menu item]
 *   [Icontext menu item]
 *   [Icontext-list menu item]
 *   [Twostate menu item]
 * Author:
 * -------
 * -------
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
   Filename:      gui_fixed_menuitems.h
   Date Created:  August-13-2002
   Contains:      UI routines header for Fixed Menuitems
**********************************************************************************/

#ifndef __GUI_FIXED_MENUITEMS_H__
#define __GUI_FIXED_MENUITEMS_H__

#include "gui.h"
#include "gdi_include.h"
 
#include "gui_theme_struct.h"
 
#include "gui_fixed_menus.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

#ifdef __MMI_TOUCH_SCREEN__
#include "kal_non_specific_general_types.h"
#include "touchscreengprot.h"
#endif /* __MMI_TOUCH_SCREEN__ */ 

    extern void UI_dummy_display_pop_up_description(S32 x, S32 y, S32 width, S32 height);
    extern void (*gui_display_pop_up_description) (S32 x, S32 y, S32 width, S32 height);

     
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    extern UI_string_type UI_dummy_get_current_pop_up_description_string(void);
    extern UI_string_type(*gui_get_current_pop_up_description_string) (void);
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
     
    /* Fixed text menuitem                       */

    typedef struct _fixed_text_menuitem_type
    {
        UI_string_type text;
        U32 flags;
    } fixed_text_menuitem_type;
    extern UI_fixed_text_menuitem_theme *current_fixed_text_menuitem_theme;

    typedef struct _fixed_text_menuitem
    {
        S32 width, height;
        UI_filled_area *normal_filler;
        UI_filled_area *disabled_filler;
        UI_filled_area *selected_filler;
        UI_filled_area *focussed_filler;
        U32 flags;
        U32 ext_flags;
        S32 text_x, text_y;
        color_t normal_text_color;
        color_t disabled_text_color;
        color_t selected_text_color;
        color_t focussed_text_color;
        UI_font_type text_font;
        S32 scroll_x, scroll_width;
        S32 text_width;
    } fixed_text_menuitem;

    extern void gui_set_fixed_text_menuitem_theme(fixed_text_menuitem *m, UI_fixed_text_menuitem_theme *t);
    extern void gui_set_fixed_text_menuitem_current_theme(fixed_text_menuitem *m);
    extern void gui_create_fixed_text_menuitem(fixed_text_menuitem *m, S32 width, S32 height);
    extern void gui_fixed_text_menuitem_set_text_position(fixed_text_menuitem *m, S32 x, S32 y);
    extern void gui_resize_fixed_text_menuitem(fixed_text_menuitem *m, S32 width, S32 height);
    extern void gui_show_fixed_text_menuitem(void *item, void *common_item_data, S32 x, S32 y);
    extern void gui_measure_fixed_text_menuitem(void *item, void *common_item_data, S32 *width, S32 *height);
    extern void gui_highlight_fixed_text_menuitem(void *item, void *common_item_data);
    extern void gui_remove_highlight_fixed_text_menuitem(void *item, void *common_item_data);
    extern void gui_fixed_text_menuitem_start_scroll(void);
    extern void gui_fixed_text_menuitem_stop_scroll(void);

    /* UI fixed icontext menuitem structure         */

    typedef struct _fixed_icontext_menuitem_type
    {
        PU8 item_icon;
        /* UI_animated_image_handle     item_icon_handle; */
        gdi_handle item_icon_handle;
        UI_string_type item_text;
        U32 flags;
        U32 ext_flags;
    } fixed_icontext_menuitem_type;

    extern UI_fixed_icontext_menuitem_theme *current_fixed_icontext_menuitem_theme;

    typedef struct _fixed_icontext_menuitem
    {
        S32 width, height;
        UI_filled_area *normal_filler;
        UI_filled_area *disabled_filler;
        UI_filled_area *selected_filler;
        UI_filled_area *focussed_filler;
        U32 flags;
        U32 ext_flags;
        S32 text_x, text_y;
        color_t normal_text_color;
        color_t disabled_text_color;
        color_t selected_text_color;
        color_t focussed_text_color;
        UI_font_type text_font;
        S32 icon_x, icon_y;
        UI_transparent_color_type icon_transparent_color;
        S32 scroll_x, scroll_width;
        S32 text_width;
    } fixed_icontext_menuitem;

    extern void gui_set_fixed_icontext_menuitem_current_theme(fixed_icontext_menuitem *m);
    extern void gui_set_fixed_icontext_menuitem_theme(
                    fixed_icontext_menuitem *m,
                    UI_fixed_icontext_menuitem_theme *t);
    extern void gui_create_fixed_icontext_menuitem(fixed_icontext_menuitem *m, S32 width, S32 height);
    extern void gui_fixed_icontext_menuitem_set_text_position(fixed_icontext_menuitem *m, S32 x, S32 y);
    extern void gui_fixed_icontext_menuitem_set_icon_position(fixed_icontext_menuitem *m, S32 x, S32 y);
    extern void gui_resize_fixed_icontext_menuitem(fixed_icontext_menuitem *m, S32 width, S32 height);
    extern void gui_show_fixed_icontext_menuitem(void *item, void *common_item_data, S32 x, S32 y);
    extern void gui_hide_fixed_icontext_menuitem(void *item, void *common_item_data, S32 x, S32 y);
    extern void gui_measure_fixed_icontext_menuitem(void *item, void *common_item_data, S32 *width, S32 *height);
    extern void gui_highlight_fixed_icontext_menuitem(void *item, void *common_item_data);
    extern void gui_remove_highlight_fixed_icontext_menuitem(void *item, void *common_item_data);
    extern void gui_fixed_icontext_menuitem_start_scroll(void);
    extern void gui_fixed_icontext_menuitem_stop_scroll(void);

#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)
    extern void gui_show_two_line_icontext_menuitem(void *item, void *common_item_data, S32 x, S32 y);
    extern void gui_two_line_scroller_stop(void);
#endif /* (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__) */ 

#ifdef __MMI_TOUCH_SCREEN__
    extern BOOL gui_fixed_icontext_menuitem_translate_pen_event(
                    void *item,
                    void *common_item_data,
                    S32 item_x,
                    S32 item_y,
                    mmi_pen_event_type_enum pen_event,
                    S16 x,
                    S16 y,
                    gui_list_pen_enum *menuitem_event,
                    gui_pen_event_param_struct *menuitem_param);
#endif /* __MMI_TOUCH_SCREEN__ */ 

    /* UI fixed icontext list menuitem structure    */
    typedef struct _fixed_icontext_list_menuitem_type
    {
        PU8 item_icons[FIXED_ICONTEXT_LIST_MAX_ICON_COLUMNS];
        /* UI_animated_image_handle     item_icon_handles[FIXED_ICONTEXT_LIST_MAX_ICON_COLUMNS]; */
        gdi_handle item_icon_handles[FIXED_ICONTEXT_LIST_MAX_ICON_COLUMNS];
        UI_string_type item_texts[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
        U32 flags;
    } fixed_icontext_list_menuitem_type;
    extern UI_fixed_icontext_list_menuitem_theme *current_fixed_icontext_list_menuitem_theme;

    typedef struct _fixed_icontext_list_menuitem
    {
        S32 width, height;
        UI_filled_area *normal_filler;
        UI_filled_area *disabled_filler;
        UI_filled_area *selected_filler;
        UI_filled_area *focussed_filler;
        U32 flags;
        U32 ext_flags;
        S32 n_text_columns;
        S32 n_icon_columns;
        UI_object_coordinates text_coordinates[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
        color_t normal_text_colors[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
        color_t disabled_text_colors[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
        color_t selected_text_colors[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
        color_t focussed_text_colors[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
        UI_font_type text_fonts[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
        UI_object_coordinates icon_coordinates[FIXED_ICONTEXT_LIST_MAX_ICON_COLUMNS];
        UI_transparent_color_type icon_transparent_color;
        S32 scroll_x, scroll_width;
        S32 text_width;
        void (*post_display_function) (void *item, void *common_item_data, S32 x, S32 y);
    } fixed_icontext_list_menuitem;

    extern void gui_set_fixed_icontext_list_menuitem_current_theme(fixed_icontext_list_menuitem *m);
    extern void gui_set_fixed_icontext_list_menuitem_theme(
                    fixed_icontext_list_menuitem *m,
                    UI_fixed_icontext_list_menuitem_theme *t);
    extern void gui_create_fixed_icontext_list_menuitem(
                    fixed_icontext_list_menuitem *m,
                    S32 width,
                    S32 height,
                    S32 n_text_columns,
                    S32 n_icon_columns);
    void gui_fixed_icontext_list_menuitem_set_post_display(fixed_icontext_list_menuitem *m,
                                                           void (*post_display_function) (void *item,
                                                                                          void *common_item_data, S32 x,
                                                                                          S32 y));
    extern void gui_fixed_icontext_list_menuitem_set_text_coordinates(
                    fixed_icontext_list_menuitem *m,
                    S32 text_column,
                    S32 x,
                    S32 y,
                    S32 width,
                    S32 height);
    extern void gui_fixed_icontext_list_menuitem_set_icon_coordinates(
                    fixed_icontext_list_menuitem *m,
                    S32 icon_column,
                    S32 x,
                    S32 y,
                    S32 width,
                    S32 height);
    extern void gui_resize_fixed_icontext_list_menuitem(fixed_icontext_list_menuitem *m, S32 width, S32 height);
    extern void gui_show_fixed_icontext_list_menuitem(void *item, void *common_item_data, S32 x, S32 y);
    extern void gui_measure_fixed_icontext_list_menuitem(void *item, void *common_item_data, S32 *width, S32 *height);
    extern void gui_highlight_fixed_icontext_list_menuitem(void *item, void *common_item_data);
    extern void gui_remove_highlight_fixed_icontext_list_menuitem(void *item, void *common_item_data);
    extern void gui_fixed_icontext_list_menuitem_start_scroll(void);
    extern void gui_fixed_icontext_list_menuitem_stop_scroll(void);

    /* UI fixed twostate menuitem structure         */

    typedef struct _fixed_twostate_menuitem_type
    {
        UI_string_type item_text;
        U32 flags;
    } fixed_twostate_menuitem_type;
    extern UI_fixed_twostate_menuitem_theme *current_fixed_twostate_menuitem_theme;

    typedef struct _fixed_twostate_menuitem
    {
        S32 width, height;
        UI_filled_area *normal_filler;
        UI_filled_area *disabled_filler;
        UI_filled_area *selected_filler;
        UI_filled_area *focussed_filler;
        U32 flags;
        U32 ext_flags;
        S32 text_x, text_y;
        color_t normal_text_color;
        color_t disabled_text_color;
        color_t selected_text_color;
        color_t focussed_text_color;
        UI_font_type text_font;
        S32 icon_x, icon_y;
        PU8 ON_icon;
        PU8 OFF_icon;
        UI_transparent_color_type icon_transparent_color;
        S32 scroll_x, scroll_width;
        S32 text_width;
    } fixed_twostate_menuitem;

 
    typedef enum
    {
        ICON_TEXT_ICON = 0,
        ICON_ICON_TEXT,
        TEXT_ICON_ICON
    } CAT73DISPLAY;

    extern void gui_set_fixed_twostate_menuitem_current_theme(fixed_twostate_menuitem *m);
    extern void gui_set_fixed_twostate_menuitem_theme(
                    fixed_twostate_menuitem *m,
                    UI_fixed_twostate_menuitem_theme *t);
    extern void gui_create_fixed_twostate_menuitem(
                    fixed_twostate_menuitem *m,
                    S32 width,
                    S32 height,
                    PU8 ON_icon,
                    PU8 OFF_icon);
    extern void gui_fixed_twostate_menuitem_set_text_position(fixed_twostate_menuitem *m, S32 x, S32 y);
    extern void gui_fixed_twostate_menuitem_set_icon_position(fixed_twostate_menuitem *m, S32 x, S32 y);
    extern void gui_fixed_twostate_menuitem_set_icons(fixed_twostate_menuitem *m, PU8 ON_icon, PU8 OFF_icon);
    extern void gui_resize_fixed_twostate_menuitem(fixed_twostate_menuitem *m, S32 width, S32 height);
    extern void gui_show_fixed_twostate_menuitem(void *item, void *common_item_data, S32 x, S32 y);
    extern void gui_measure_fixed_twostate_menuitem(void *item, void *common_item_data, S32 *width, S32 *height);
    extern void gui_highlight_fixed_twostate_menuitem(void *item, void *common_item_data);
    extern void gui_remove_highlight_fixed_twostate_menuitem(void *item, void *common_item_data);
    extern void gui_fixed_twostate_menuitem_start_scroll(void);
    extern void gui_fixed_twostate_menuitem_stop_scroll(void);
#ifdef __MMI_TOUCH_SCREEN__
    extern BOOL gui_fixed_twostate_menuitem_translate_pen_event(
                    void *item,
                    void *common_item_data,
                    S32 item_x,
                    S32 item_y,
                    mmi_pen_event_type_enum pen_event,
                    S16 x,
                    S16 y,
                    gui_list_pen_enum *menuitem_event,
                    gui_pen_event_param_struct *menuitem_param);
#endif /* __MMI_TOUCH_SCREEN__ */ 

     
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
     
#define HIGHLIGHTER_COLOR_OPACITY_VALUE 85      /* To set the highlighter opacity factor value. It should have any value from 0 to 100 */
     
    extern void gui_block_transparency_effect(void);
    extern void gui_unblock_transparency_effect(void);
    extern void gui_fixed_icontext_enable_transparent_effect(fixed_icontext_menuitem *m);
    extern void gui_fixed_icontext_disable_transparent_effect(fixed_icontext_menuitem *m);
    extern void gui_fixed_icontext_list_enable_transparent_effect(fixed_icontext_list_menuitem *m);
    extern void gui_fixed_icontext_list_disable_transparent_effect(fixed_icontext_list_menuitem *m);
    extern void gui_fixed_text_menuitem_enable_transparent_effect(fixed_text_menuitem *m);
    extern void gui_fixed_text_menuitem_disable_transparent_effect(fixed_text_menuitem *m);
    extern void gui_fixed_twostate_menuitem_enable_transparent_effect(fixed_twostate_menuitem *m);
    extern void gui_fixed_twostate_menuitem_disable_transparent_effect(fixed_twostate_menuitem *m);

#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
     

     

    /* LIST HIGHLIGHTER EFFECTS */
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    typedef enum
    {
        MMI_UI_LIST_HIGHLIGHT_EFFECT_NONE = 0,          /* No Effect */
        MMI_UI_LIST_HIGHLIGHT_EFFECT_TRANSPARENCY = 1,  /* Transparent with increasing opacity */
        MMI_UI_LIST_HIGHLIGHT_EFFECT_FLASH_FEELING = 2, /* Flash effect with increase in Filler area */
        MMI_UI_LIST_HIGHLIGHT_EFFECT_ANIMATION = 3      /* Animatian with gif */
    } gui_list_effect_styles;

     
#define LIST_TRANSPARENT_HIGHLIGHTER_TIMER 175
#define LIST_FLASH_HIGHLIGHTER_TIMER 150
#define LIST_ANIMATION_HIGHLIGHTER_TIMER 175
     

#define MAX_OPACITY_VALUE_FACTOR 10
#define TRANSPARENCY_START_OPACITY_VALUE 1
#define JUMP_IN_HIGHLIGHTERS_EFFECTS_FRAME 4
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    /* function definitions */
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    extern void gui_callback_highlighter_text_filler(void);
    extern void gui_stop_list_highlight_effect(void);
    extern void gui_draw_animated_highlight_filler(
                    S32 Xstart_effect,
                    S32 Ystart_effect,
                    S32 Xend_effect,
                    S32 Yend_effect,
                    UI_filled_area *f);
    extern void gui_draw_transparent_color_filler(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f);
    extern void gui_draw_flash_feeling_filler(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f);
    extern void gui_reset_current_list_effect(void);
    extern void gui_set_current_list_effect_to_default(void);
    extern void gui_draw_list_filled_area(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f);
    extern void gui_scroll_start_handler_for_list_effects(void);
    extern S32 gui_get_current_list_effect(void);
    extern void gui_dummy_scroll_handler_for_list_effects(void);
    void gui_register_callback_menuitem_scroll(void (*f) (void));
    extern void gui_unblock_list_effect(void);
    extern void gui_block_list_effect(void);
    extern void gui_draw_list_highlight_effect_last_frame(void);
    extern void gui_set_current_list_effect(gui_list_effect_styles list_style);
    extern void gui_restore_current_list_effect(void);
    extern S32 is_draw_next_frame_list_highlight_effect_value(void);
    extern void set_start_position_parameter_for_list_highlighter_effect(S32 x, S32 y);
    extern void set_start_position_and_item_parameters_for_list_highlighter_effect(
                    void *item,
                    void *common_item_data,
                    S32 x,
                    S32 y);
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 

     

     
#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    extern void UI_dummy_show_two_line_menuitem_part(void);
    extern void (*gui_set_current_two_line_menuitem_data) (void);
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 

#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)
    extern void (*gui_reset_current_two_line_menuitem_data) (void);
#endif 

#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    extern pBOOL UI_dummy_get_two_line_menuitem_height(void);
    extern void UI_dummy_set_current_two_line_menuitem_data(void);
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 

#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)
    extern void UI_dummy_reset_current_two_line_menuitem_data(void);
#endif 

#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
 
    extern void (*gui_show_two_line_menuitem_part) (void);
    extern void UI_dummy_move_two_line_menuitem_part(S32 x, S32 y);
    extern void UI_dummy_resize_two_line_menuitem_part(S32 x, S32 y);
    extern void (*gui_move_two_line_menuitem_part) (S32 x, S32 y);
    extern void (*gui_resize_two_line_menuitem_part) (S32 x, S32 y);
    extern pBOOL(*gui_get_two_line_menuitem_height) (void);
    extern void (*gui_reset_two_line) (void);
    extern void (*gui_two_line_toggle_thumbnail_direction) (void);
    extern U8(*gui_two_line_get_thumbnail_flags) (void);
    extern U8 UI_dummy_two_line_get_thumbnail_flags(void);
    extern void two_line_menuitem_cleanup_function(void); 
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 
     
#ifdef __cplusplus
}
#endif 

#endif /* __GUI_FIXED_MENUITEMS_H__ */ 

