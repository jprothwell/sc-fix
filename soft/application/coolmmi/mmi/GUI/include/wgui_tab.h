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
 * wgui_tab.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   This file is intended for tab control upper layer wrappers and instance handling
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _WGUI_TAB_H
#define _WGUI_TAB_H

#include "gui_tab.h"
/***************************************************************************** 
* Define
*****************************************************************************/
#define     MAX_TAB_PANE_ITEMS   20

/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef struct _tab_pane_history
{
    S32 first_displayed_item;
    S32 last_displayed_item;
    S32 highlighted_item;
    S32 highlighted_pane;
    S32 parent_id;
    S32 type;
    void (*set_callback) (void);
    void (*set_data_callback) (void);
    void (*reset_data_callback) (void);
    void (*category_exit_callback) (void);
    void (*category_entry_callback) (void);
#ifdef MMI_TAB_PANE_LIST_HISTORY
    struct_tab_list_history tab_list_history;
#endif 
} struct_tab_pane_history;

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/
extern S32 MMI_tab_height;

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void create_two_icon_tab_pane(void);
extern void set_two_icon_tab_pane(void);
void register_set_tab_pane_callback(void (*callback) (void), void (*set_data_callback) (void),
                                    void (*reset_data_callback) (void));
extern void set_tab_pane(S32 number_of_items, U16 *list_of_items, U16 *list_of_icons, S32 default_item);
extern void add_item_tab_pane(UI_string_type text, PU8 icon);
extern void goto_tab_pane_item(S32 index);
extern void register_tab_pane_keys(void);
void set_tab_pane_hightlight_function(void (*f) (S32 item_index));
extern void standard_tab_pane_highlight_function(S32 index);
extern void tab_pane_goto_current_highlighted_item(void);
extern void tab_pane_hightlight_function(S32 index);
extern void show_tab_pane(void);
extern void show_tab_pane_title(void);
void register_tab_pane_hilite_handler(void (*f) (S32 item_index));
extern void save_current_parent_id(void);
extern void enable_tab_pane_display(void);
extern void disable_tab_pane_display(void);
extern void backup_current_entry_function(void);
extern void create_and_setup_two_icon_tab_pane(void);
extern S32 is_enabled_tab_pane(void);
extern void process_history_buffer(void);
extern U8 *get_tab_pane_and_parent_category_history(U8 *history_buffer);
extern void set_tab_pane_category_history(U8 *history_buffer);
extern S32 get_tab_pane_history_size(void);
extern void set_tab_pane_category_history(U8 *history_buffer);
extern void backup_current_entry_function(void);
extern void backup_current_category_function(void);
extern void create_and_setup_two_text_tab_pane(void);
extern void create_and_setup_four_icon_tab_pane(void);
extern void create_and_setup_three_icon_tab_pane(void);
extern void move_tab_pane(S32 x, S32 y);
extern void resize_tab_pane(S32 height, S32 width);
extern void enable_tab_pane_title(MMI_BOOL show_title);
extern void create_four_icontext_tab_pane(void);
extern void set_four_icontext_pane_theme_data(void);
extern void tab_pane_disable_function_exection(void);
extern S32 get_tab_pane_history_size(void);
extern U8 *get_tab_pane_history(U8 *history_buffer);
void set_tab_pane_category_exit_function(void (*f) (void));
void set_tab_pane_category_entry_function(void (*f) (void));
extern S32 get_current_tab_pane_height(void);
extern void create_and_setup_one_icon_tab_pane(void);
extern void tab_pane_modify_data(void);
void set_tab_pane_modify_data_function(void (*ptr) (void));
extern void reset_tab_pane_modify_data_function(void);
extern void set_tab_pane_modify_title_icon(void);
extern S32 is_tab_pane_displayed_on_current_screen(void);
extern void set_tab_pane_cyclic(void);
extern S32 set_list_in_tab_history(void);
#endif /* _WGUI_TAB_H */ 

