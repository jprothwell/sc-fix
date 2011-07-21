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
 *   gui_theme_struct.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Basic theme data structure 
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

#ifndef THEME_STRUCT_H
#define  THEME_STRUCT_H
#include "custthemesres.h"
typedef struct _UI_button_theme
{
    UI_filled_area *normal_up_filler;
    UI_filled_area *normal_down_filler;
    UI_filled_area *disabled_up_filler;
    UI_filled_area *disabled_down_filler;
    UI_filled_area *focussed_up_filler;
    UI_filled_area *focussed_down_filler;
    UI_filled_area *clicked_up_filler;
    UI_filled_area *clicked_down_filler;
} UI_button_theme;

typedef struct _UI_text_button_theme
{
    color_t normal_up_text_color;
    color_t normal_down_text_color;
    color_t disabled_up_text_color;
    color_t disabled_down_text_color;
    color_t focussed_up_text_color;
    color_t focussed_down_text_color;
    color_t clicked_up_text_color;
    color_t clicked_down_text_color;
    UI_font_type text_font;
    U32 flags;
} UI_text_button_theme;

typedef struct _UI_icon_button_theme
{
    UI_filled_area *normal_up_filler;
    UI_filled_area *normal_down_filler;
    UI_filled_area *disabled_up_filler;
    UI_filled_area *disabled_down_filler;
    UI_filled_area *focussed_up_filler;
    UI_filled_area *focussed_down_filler;
    UI_filled_area *clicked_up_filler;
    UI_filled_area *clicked_down_filler;
    U32 flags;
} UI_icon_button_theme;

typedef struct _UI_icontext_button_theme
{
    UI_filled_area *normal_up_filler;
    UI_filled_area *normal_down_filler;
    UI_filled_area *disabled_up_filler;
    UI_filled_area *disabled_down_filler;
    UI_filled_area *focussed_up_filler;
    UI_filled_area *focussed_down_filler;
    UI_filled_area *clicked_up_filler;
    UI_filled_area *clicked_down_filler;
    color_t normal_up_text_color;
    color_t normal_down_text_color;
    color_t disabled_up_text_color;
    color_t disabled_down_text_color;
    color_t focussed_up_text_color;
    color_t focussed_down_text_color;
    color_t clicked_up_text_color;
    color_t clicked_down_text_color;
    UI_font_type text_font;
    U32 flags;
} UI_icontext_button_theme;

typedef struct _UI_vertical_scrollbar_theme
{
    UI_filled_area *focussed_bar_filler;
    UI_filled_area *normal_bar_filler;
    UI_filled_area *disabled_bar_filler;
    UI_button_theme *up_button_theme;
    UI_button_theme *down_button_theme;
    UI_button_theme *scroll_button_theme;
    PU8 up_button_icon;
    PU8 down_button_icon;
    PU8 scroll_button_icon;
    U32 flags;
} UI_vertical_scrollbar_theme;

typedef struct _UI_horizontal_scrollbar_theme
{
    UI_filled_area *focussed_bar_filler;
    UI_filled_area *normal_bar_filler;
    UI_filled_area *disabled_bar_filler;
    UI_button_theme *left_button_theme;
    UI_button_theme *right_button_theme;
    UI_button_theme *scroll_button_theme;
    PU8 left_button_icon;
    PU8 right_button_icon;
    PU8 scroll_button_icon;
    U32 flags;
} UI_horizontal_scrollbar_theme;

typedef struct _UI_horizontal_progress_indicator_theme
{
    U32 flags;
    UI_filled_area *background_filler;
    UI_filled_area *foreground_filler;
    S32 n_fillers;
    U8 *percentages;
    S32 block_width;
    S32 block_gap;
    S32 border_gap;
} UI_horizontal_progress_indicator_theme;

typedef struct _UI_vertical_progress_indicator_theme
{
    U32 flags;
    UI_filled_area *background_filler;
    UI_filled_area *foreground_filler;
    S32 n_fillers;
    U8 *percentages;
    S32 block_height;
    S32 block_gap;
    S32 border_gap;
} UI_vertical_progress_indicator_theme;

typedef struct _UI_vertical_slide_theme
{
    UI_filled_area *focussed_bar_filler;
    UI_filled_area *normal_bar_filler;
    UI_filled_area *disabled_bar_filler;
    UI_button_theme *scroll_button_theme;
    PU8 scroll_button_icon;
    U32 flags;
} UI_vertical_slide_theme;

typedef struct _UI_horizontal_slide_theme
{
    UI_filled_area *focussed_bar_filler;
    UI_filled_area *normal_bar_filler;
    UI_filled_area *disabled_bar_filler;
    UI_button_theme *scroll_button_theme;
    PU8 scroll_button_icon;
    U32 flags;
} UI_horizontal_slide_theme;

typedef struct _UI_multitap_input_theme
{
    UI_filled_area *f;
    color_t selector_color;
    color_t normal_text_color;
    color_t selected_text_color;
    UI_font_type text_font;
    S8 text_gap;
    U32 flags;
} UI_multitap_input_theme;

typedef struct _UI_single_line_input_box_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    color_t normal_text_color;
    color_t disabled_text_color;
    color_t selected_text_color;
    color_t selection_text_color;
    color_t selection_color;
    color_t cursor_color;
    UI_font_type text_font;
    S8 text_gap;
    U32 flags;
    U8 mask_character;
} UI_single_line_input_box_theme;

typedef struct _UI_multi_line_input_box_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    color_t special_text_color;
    color_t normal_text_color;
    color_t disabled_text_color;
    color_t selected_text_color;
    color_t selection_text_color;
    color_t selection_color;
    color_t boundary_color;
    color_t cursor_color;
    UI_font_type text_font;
    S8 text_gap;
    U32 flags;
    U8 mask_character;
    S32 vbar_width;
} UI_multi_line_input_box_theme;

typedef struct _UI_dialer_input_box_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    color_t normal_text_color;
    color_t disabled_text_color;
    color_t selected_text_color;
    color_t selection_text_color;
    color_t selection_color;
    color_t cursor_color;
    UI_font_type text_font;
    S8 text_gap;
    U32 flags;
    U8 mask_character;
} UI_dialer_input_box_theme;

typedef struct _UI_text_menuitem_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    UI_filled_area *focussed_filler;
    color_t normal_text_color;
    color_t disabled_text_color;
    color_t selected_text_color;
    color_t focussed_text_color;
    UI_font_type text_font;
    U32 flags;
} UI_text_menuitem_theme;

typedef struct _UI_icontext_menuitem_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    UI_filled_area *focussed_filler;
    color_t normal_text_color;
    color_t disabled_text_color;
    color_t selected_text_color;
    color_t focussed_text_color;
    UI_font_type text_font;
    U32 flags;
} UI_icontext_menuitem_theme;

typedef struct _UI_fixed_text_menuitem_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    UI_filled_area *focussed_filler;
    color_t normal_text_color;
    color_t disabled_text_color;
    color_t selected_text_color;
    color_t focussed_text_color;
    UI_font_type text_font;
    U32 flags;
} UI_fixed_text_menuitem_theme;

typedef struct _UI_fixed_icontext_menuitem_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    UI_filled_area *focussed_filler;
    color_t normal_text_color;
    color_t disabled_text_color;
    color_t selected_text_color;
    color_t focussed_text_color;
    UI_font_type text_font;
    U32 flags;
} UI_fixed_icontext_menuitem_theme;

/* Minimum 4 icons are required for email/IMPS */
#define FIXED_ICONTEXT_LIST_MAX_ICON_COLUMNS 4
/*
 * "At-most" 2 text columns are supported for dynamic list menu because 
 * * the implementation uses subMenuData[] and hintData[] to hold text data.
 */
#define FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS 2

typedef struct _UI_fixed_icontext_list_menuitem_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    UI_filled_area *focussed_filler;
    color_t normal_text_colors[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
    color_t disabled_text_colors[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
    color_t selected_text_colors[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
    color_t focussed_text_colors[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
    UI_font_type text_fonts[FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS];
    U32 flags;
} UI_fixed_icontext_list_menuitem_theme;

typedef struct _UI_fixed_twostate_menuitem_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    UI_filled_area *focussed_filler;
    color_t normal_text_color;
    color_t disabled_text_color;
    color_t selected_text_color;
    color_t focussed_text_color;
    UI_font_type text_font;
    U32 flags;
} UI_fixed_twostate_menuitem_theme;

typedef struct _UI_list_menu_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *focussed_filler;
    S32 vbar_width;
    U32 flags;
} UI_list_menu_theme;

typedef struct _UI_matrix_menu_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *focussed_filler;
    S32 vbar_width;
    S32 hbar_height;
    U32 flags;
} UI_matrix_menu_theme;

typedef struct _UI_fixed_list_menu_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *focussed_filler;
    S32 vbar_width;
    U32 flags;
} UI_fixed_list_menu_theme;

typedef struct _UI_fixed_matrix_menu_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *focussed_filler;
    S32 vbar_width;
    S32 hbar_height;
    U32 flags;
} UI_fixed_matrix_menu_theme;

typedef struct _UI_drop_down_control_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *focussed_filler;
    UI_filled_area *clicked_filler;
    color_t normal_text_color;
    color_t disabled_text_color;
    color_t focussed_text_color;
    color_t clicked_text_color;
    UI_font_type text_font;
    U32 flags;
    UI_button_theme *down_button_theme;
    PU8 down_button_icon;
} UI_drop_down_control_theme;

typedef struct _UI_horizontal_select_theme
{
    UI_filled_area *background_filler;
    U32 flags;
} UI_horizontal_select_theme;

typedef struct _UI_window_title_theme
{
    color_t active_text_color;
    color_t inactive_text_color;
    color_t focussed_text_color;
    color_t selected_text_color;
    UI_font_type text_font;
    UI_filled_area *active_filler;
    UI_filled_area *inactive_filler;
    UI_filled_area *focussed_filler;
    UI_filled_area *selected_filler;
} UI_window_title_theme;

typedef struct _UI_pop_up_description_theme
{
    color_t text_color;
    UI_font_type text_font;
    UI_filled_area *background_filler;
    U32 flags;
} UI_pop_up_description_theme;

typedef struct _UI_information_bar_theme
{
    color_t background_color;
} UI_information_bar_theme;

typedef struct _UI_EMS_input_box_theme
{
    UI_filled_area *normal_filler;
    UI_filled_area *disabled_filler;
    UI_filled_area *selected_filler;
    color_t normal_text_color;
    color_t selected_text_color;
    color_t selection_color;
    color_t cursor_color;
    color_t boundary_color;   
    U32 flags;
    S32 vbar_width;
} UI_EMS_input_box_theme;

typedef struct _UI_shortcut_box_theme
{
    color_t normal_text_color;
    color_t normal_text_border_color;
    color_t active_text_color;
    color_t active_text_border_color;
    S8 use_border;
} UI_shortcut_box_theme;

typedef struct _UI_horizontal_tab_bar_theme
{
    UI_filled_area *background_filler;

    UI_font_type tab_text_font;
    UI_font_type focused_tab_text_font;

    color_t focused_tab_text_color;
    color_t normal_tab_text_color;
    color_t blinking_tab_text_color;

    UI_font_type hint_text_font;
    color_t hint_text_color;
    color_t hint_text_border_color;
    S8 hint_text_gap;
    U16 hint_bg_image;
    UI_filled_area *hint_filler;

    /* Blinking and normal tab has the same size */
    UI_filled_area *normal_tab_filler;
    UI_filled_area *normal_l_tab_filler;
    UI_filled_area *normal_r_tab_filler;
    U16 normal_tab_icon;
    U16 normal_l_tab_icon;
    U16 normal_r_tab_icon;

    UI_filled_area *blinking_tab_filler;
    UI_filled_area *blinking_l_tab_filler;
    UI_filled_area *blinking_r_tab_filler;
    U16 blinking_tab_icon;
    U16 blinking_l_tab_icon;
    U16 blinking_r_tab_icon;

    /* Focused tab might be larger than normal tab */
    UI_filled_area *focused_tab_filler;
    UI_filled_area *focused_l_tab_filler;
    UI_filled_area *focused_r_tab_filler;
    U16 focused_tab_icon;
    U16 focused_l_tab_icon;
    U16 focused_r_tab_icon;

    U16 left_button_icon;
    U16 right_button_icon;
} UI_horizontal_tab_bar_theme;

typedef struct _UI_virtual_keyboard_theme
{
    UI_filled_area *key_down_filler;
    color_t key_down_text_color;
    color_t key_up_text_color;
    color_t dead_key_down_text_color;
    color_t dead_key_up_text_color;
    color_t disp_area_text_color;
    UI_font_type text_font;
    U32 flags;
} UI_virtual_keyboard_theme;

#endif /* THEME_STRUCT_H */ 

