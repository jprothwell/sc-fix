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
 * gui_tab.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   This file is intended for tab control lower layer irrespective of instance
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _GUI_TAB_H
#define _GUI_TAB_H

/***************************************************************************** 
* Define
*****************************************************************************/
#define     MMI_TAB_PANE_LIST_HISTORY  1
#define     MAX_TAB_ITEMS  12
#define     UI_TAB_CONTROL_NORMAL   0x00000000
#define     UI_TAB_CONTROL_CYCLIC   0x00000001
/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum
{
    TAB_CONTROL_WITH_TWO_ICON = 0,
    TAB_CONTROL_WITH_THREE_ICON,
    TAB_CONTROL_WITH_FOUR_ICON,
    TAB_CONTROL_WITH_TWO_TEXT,
    TAB_CONTROL_WITH_ONE_ICON
} enum_tab_control_type;

typedef struct
{
    PU8 highlight_image;
    PU8 un_highlight_image;
} struct_tab_icon_data;

typedef struct
{
    PU8 highlight_image;
    PU8 un_highlight_image;
} struct_tab_icon_theme;

typedef struct
{
    PU8 highlight_image;
    PU8 un_highlight_image;
    color_t un_highlight_text_color;
    color_t highlight_text_color;
    UI_font_type font;
} struct_tab_text_data;

typedef struct
{
    PU8 highlight_image;
    PU8 un_highlight_image;
    color_t text_color;
    UI_font_type font;
} struct_tab_text_theme;

typedef struct
{
    UI_filled_area *back_filler;
    UI_filled_area *high_filler;
    PU8 left_indicator;
    PU8 right_indicator;
    UI_filled_area *title_filler;
    color_t *text_color;
    color_t *text_border_color;
    UI_font_type text_font;
} struct_tab_icontext_data;

typedef struct
{
    UI_filled_area back_filler;
    S32 left_indicator_id;
    S32 right_indicator_id;
    UI_filled_area title_filler;
    color_t text_color;
    UI_font_type font;
} struct_tab_icontext_theme;

typedef struct
{
    UI_string_type item_text;
    PU8 item_icon;
} struct_tab_items;
typedef struct
{
    S16 highlighted_item[MAX_TAB_ITEMS];
    S16 first_displayed_item[MAX_TAB_ITEMS];
    S16 last_displayed_item[MAX_TAB_ITEMS];
} struct_tab_list_history;

typedef struct _struct_tab_control
{
    S32 x;
    S32 y;
    S32 width;
    S32 height;
    S32 type;   /* Value of this will be from the tab_control_type enum */
    S32 n_items;
    S32 first_displayed_item;
    S32 last_displayed_item;
    S32 highlighted_item;
    S32 highlighted_pane;
    S32 parent_id;
    S32 navigation_index;
    void (*set_callback) (void);
    void (*set_data_callback) (void);
    void (*reset_data_callback) (void);
    void (*category_entry_callback) (void);
    void (*category_exit_callback) (void);
    void (*draw_function) (struct _struct_tab_control *);
    void (*next_function) (struct _struct_tab_control *);
    void (*prev_function) (struct _struct_tab_control *);
    void (*highlight_function) (S32 index);
    void (*goto_function) (struct _struct_tab_control *, S32 index);
    void *type_data;
    struct_tab_items *item_data;
#ifdef MMI_TAB_PANE_LIST_HISTORY
    struct_tab_list_history tab_list_history;
#endif 
    S32 flags;
} struct_tab_control;

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void gui_create_tab_pane(
                struct_tab_control *tab_control,
                enum_tab_control_type type,
                S32 x,
                S32 y,
                S32 width,
                S32 height);
extern void gui_associate_tab_pane_type_data(struct_tab_control *tab_control, void *type_data);
extern void gui_associate_tab_pane_item_data(struct_tab_control *tab_control, struct_tab_items *item_data);
extern void gui_add_item_tab_pane(struct_tab_control *tab_control, UI_string_type text, PU8 icon);
extern void gui_show_tab_pane(struct_tab_control *tab_control);

extern void gui_show_two_icon_tab_pane(struct_tab_control *tab_control);

void gui_register_set_tab_pane_callback(struct_tab_control *tab_control, void (*callback) (void),
                                        void (*set_data_callback) (void), void (*reset_data_callback) (void));

extern void gui_go_to_item(struct_tab_control *tab_control, S32 index);

void gui_set_tab_pane_hightlight_fucntion(struct_tab_control *tab_control, void (*f) (S32 item_index));
extern void gui_highlight_function(struct_tab_control *tab_control, S32 index);

extern void gui_goto_two_item_tab_pane(struct_tab_control *tab_control, S32 index);
extern void gui_save_parent_id(struct_tab_control *tab_control, S32 parent_id);

extern void gui_next_two_item_tab_pane(struct_tab_control *tab_control);
extern void gui_prev_two_item_tab_pane(struct_tab_control *tab_control);

extern void gui_show_text_tab_pane(struct_tab_control *tab_control);
extern void gui_goto_three_item_tab_pane(struct_tab_control *tab_control, S32 index);
extern void gui_next_three_item_tab_pane(struct_tab_control *tab_control);
extern void gui_prev_three_item_tab_pane(struct_tab_control *tab_control);
extern void gui_tab_pane_next_item(struct_tab_control *tab_control);
extern void gui_tab_pane_prev_item(struct_tab_control *tab_control);
extern void gui_show_icon_tab_pane(struct_tab_control *tab_control);
extern void gui_goto_four_item_tab_pane(struct_tab_control *tab_control, S32 index);
extern void gui_next_four_item_tab_pane(struct_tab_control *tab_control);
extern void gui_prev_four_item_tab_pane(struct_tab_control *tab_control);
extern void gui_show_three_icon_tab_pane(struct_tab_control *tab_control);
extern void gui_move_tab_pane(struct_tab_control *tab_control, S32 x, S32 y);
extern void gui_resize_tab_pane(struct_tab_control *tab_control, S32 height, S32 width);
extern void gui_goto_one_item_tab_pane(struct_tab_control *tab_control, S32 index);
extern void gui_next_one_item_tab_pane(struct_tab_control *tab_control);
extern void gui_prev_one_item_tab_pane(struct_tab_control *tab_control);
void gui_set_tab_pane_category_exit_function(struct_tab_control *tab_control, void (*f) (void));
void gui_set_tab_pane_category_entry_function(struct_tab_control *tab_control, void (*f) (void));
extern void gui_set_tab_pane_flags(struct_tab_control *tab_control, S32 flags);

#endif /* _GUI_TAB_H */ 

