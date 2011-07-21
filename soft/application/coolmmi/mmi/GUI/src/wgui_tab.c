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
 * wgui_tab.c
 * Project:
 * --------
 
 * Description:
 * ------------
 * This file is intended for tab control upper layer wrappers and instance handling.
 * Note: wgui_tab_bars.c is another implementation of tab control. 
 * wgui_tab.c will deprecate wgui_tab_bars.c in the future.
 * Author:
 * -------
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/*  Include: MMI header file */
#include "mmi_features.h"
#include "gui_config.h"//051106 tab mainmenu
#ifdef __MMI_UI_TAB_PANE__

#include "wgui_categories.h"
#include "gui_data_types.h"
#include "custthemesres.h"
#include "gui_tab.h"
#include "wgui_tab.h"
#include "historygprot.h"
extern S32 UI_device_width;

#include "wgui_categories_defs.h"
#include "gdi_include.h"

/*  Include: PS header file */

/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/
struct_tab_control MMI_tab_control;
struct_tab_icon_data MMI_tab_icon_data;
struct_tab_text_data MMI_tab_text_data;

/* struct_tab_icontext_data     MMI_tab_icontext_data; */
struct_tab_items MMI_tab_items[MAX_TAB_ITEMS];
S32 MMI_tab_height = MMI_TAB_HEIGHT;
void (*tab_pane_modify_data_ptr) (void);
void (*MMI_tab_pane_callback) (void);
void (*MMI_set_tab_pane_data_callback) (void);
void (*MMI_reset_tab_pane_data_callback) (void);
S32 MMI_enable_tab_pane_display = 0;
FuncPtr backup_entry_func_ptr;
FuncPtr backup_exit_func_ptr;
U8 *(*backup_category_func_ptr) (U8 *buffer);
S32 enable_function_execution = 1;
S32 allow_history_saving = 1;
S32 disable_tab_pane_register = 0;
S32 set_list_history = 0;

/*----------------------------------------------------------------------------
Function    :  MMI_tab_highlight_handler
Description    :  Function pointer store the address of function which get
               executed when any item in tab get high lighted
Input Parameters: S32 item _index :- store the current index of item highlighted
Output Parameters:   none
Returns:       void
----------------------------------------------------------------------------*/
void (*MMI_tab_highlight_handler) (S32 item_index);

/***************************************************************************** 
* Local Function
*****************************************************************************/

/***************************************************************************** 
* Global Variable
*****************************************************************************/

/***************************************************************************** 
* Global Function
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  create_two_icon_tab_pane
 * DESCRIPTION
 *  creates two icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_two_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_create_tab_pane(
        &MMI_tab_control,
        TAB_CONTROL_WITH_TWO_ICON,
        0,
        MMI_title_y + MMI_title_height,
        UI_device_width,
        MMI_tab_height);
    enable_function_execution = 1;
}


/*****************************************************************************
 * FUNCTION
 *  create_two_text_tab_pane
 * DESCRIPTION
 *  creates two text tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_two_text_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_create_tab_pane(
        &MMI_tab_control,
        TAB_CONTROL_WITH_TWO_TEXT,
        0,
        MMI_title_y + MMI_title_height,
        UI_device_width,
        MMI_tab_height);
    enable_function_execution = 1;
}


/*****************************************************************************
 * FUNCTION
 *  create_four_icon_tab_pane
 * DESCRIPTION
 *  creates four icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_four_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_create_tab_pane(
        &MMI_tab_control,
        TAB_CONTROL_WITH_FOUR_ICON,
        0,
        MMI_title_y + MMI_title_height,
        UI_device_width,
        MMI_tab_height);
    enable_function_execution = 1;
}

/*
 * void create_four_icontext_tab_pane(void)
 * {
 * gui_create_tab_pane(&MMI_tab_control,TAB_CONTROL_WITH_FOUR_ICONTEXT,
 * 0,MMI_title_height,UI_device_width,MMI_tab_height);
 * enable_function_execution=1;
 * }
 */


/*****************************************************************************
 * FUNCTION
 *  create_three_icon_tab_pane
 * DESCRIPTION
 *  creates three icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_three_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_create_tab_pane(
        &MMI_tab_control,
        TAB_CONTROL_WITH_THREE_ICON,
        0,
        MMI_title_y + MMI_title_height,
        UI_device_width,
        MMI_tab_height);
    enable_function_execution = 1;
}


/*****************************************************************************
 * FUNCTION
 *  create_one_icon_tab_pane
 * DESCRIPTION
 *  creates one icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_one_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_create_tab_pane(
        &MMI_tab_control,
        TAB_CONTROL_WITH_ONE_ICON,
        0,
        MMI_title_y + MMI_title_height,
        UI_device_width,
        MMI_tab_height);
    enable_function_execution = 1;
}


/*****************************************************************************
 * FUNCTION
 *  move_tab_pane
 * DESCRIPTION
 *  moves tab pane
 * PARAMETERS
 *  x           [IN]            
 *  y           [IN]            
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void move_tab_pane(S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_move_tab_pane(&MMI_tab_control, x, y);
}


/*****************************************************************************
 * FUNCTION
 *  resize_tab_pane
 * DESCRIPTION
 *  resizes tab pane
 * PARAMETERS
 *  height      [IN]            
 *  width       [IN]            
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void resize_tab_pane(S32 height, S32 width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_resize_tab_pane(&MMI_tab_control, height, width);
}


/*****************************************************************************
 * FUNCTION
 *  set_two_tab_pane_theme_data
 * DESCRIPTION
 *  sets theme data for two icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_two_tab_pane_theme_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_associate_tab_pane_type_data(&MMI_tab_control, &MMI_tab_icon_data);
    MMI_tab_icon_data.highlight_image = get_image(TAB_PANE_ICON_HIGHLIGHT);
    MMI_tab_icon_data.un_highlight_image = get_image(TAB_PANE_ICON_UN_HIGHLIGHT);
}


/*****************************************************************************
 * FUNCTION
 *  set_two_icon_tab_pane
 * DESCRIPTION
 *  sets two icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_two_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_register_set_tab_pane_callback(
        &MMI_tab_control,
        MMI_tab_pane_callback,
        MMI_set_tab_pane_data_callback,
        MMI_reset_tab_pane_data_callback);
    if (MMI_set_tab_pane_data_callback)
    {
        (*MMI_set_tab_pane_data_callback) ();
    }
    (*MMI_tab_pane_callback) ();
    set_two_tab_pane_theme_data();
}


/*****************************************************************************
 * FUNCTION
 *  set_two_text_tab_pane_theme_data
 * DESCRIPTION
 *  sets theme for two text tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_two_text_tab_pane_theme_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_associate_tab_pane_type_data(&MMI_tab_control, &MMI_tab_text_data);
    MMI_tab_text_data.highlight_image = get_image(TAB_PANE_TEXT_HIGHLIGHT);
    MMI_tab_text_data.un_highlight_image = get_image(TAB_PANE_TEXT_UN_HIGHLIGHT);
    MMI_tab_text_data.font = &MMI_small_font;
    MMI_tab_text_data.highlight_text_color = gui_color(2, 29, 80);
    MMI_tab_text_data.un_highlight_text_color = gui_color(95, 26, 2);
}


/*****************************************************************************
 * FUNCTION
 *  set_two_text_tab_pane
 * DESCRIPTION
 *  sets two text tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_two_text_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_register_set_tab_pane_callback(
        &MMI_tab_control,
        MMI_tab_pane_callback,
        MMI_set_tab_pane_data_callback,
        MMI_reset_tab_pane_data_callback);
    if (MMI_set_tab_pane_data_callback)
    {
        (*MMI_set_tab_pane_data_callback) ();
    }
    (*MMI_tab_pane_callback) ();
    set_two_text_tab_pane_theme_data();
}


/*****************************************************************************
 * FUNCTION
 *  set_four_icon_pane_theme_data
 * DESCRIPTION
 *  sets theme for four icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_four_icon_pane_theme_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_associate_tab_pane_type_data(&MMI_tab_control, &MMI_tab_icon_data);
    MMI_tab_icon_data.highlight_image = get_image(TAB_PANE_ICON_HIGHLIGHT);
    MMI_tab_icon_data.un_highlight_image = get_image(TAB_PANE_ICON_UN_HIGHLIGHT);
}


/*****************************************************************************
 * FUNCTION
 *  set_four_icon_tab_pane
 * DESCRIPTION
 *  sets four icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_four_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_register_set_tab_pane_callback(
        &MMI_tab_control,
        MMI_tab_pane_callback,
        MMI_set_tab_pane_data_callback,
        MMI_reset_tab_pane_data_callback);
    if (MMI_set_tab_pane_data_callback)
    {
        (*MMI_set_tab_pane_data_callback) ();
    }
    (*MMI_tab_pane_callback) ();
    set_four_icon_pane_theme_data();
}

/*
 * void set_four_icontext_pane_theme_data(void)
 * {
 * gui_associate_tab_pane_type_data(&MMI_tab_control,
 * &MMI_tab_text_data);
 * }
 * 
 * 
 * void set_four_icontext_tab_pane(void)
 * {
 * gui_register_set_tab_pane_callback(&MMI_tab_control, MMI_tab_pane_callback,
 * MMI_set_tab_pane_data_callback,
 * MMI_reset_tab_pane_data_callback);
 * (*MMI_set_tab_pane_data_callback)();
 * (*MMI_tab_pane_callback)();
 * set_four_icon_pane_theme_data();
 * }
 */


/*****************************************************************************
 * FUNCTION
 *  set_three_icon_pane_theme_data
 * DESCRIPTION
 *  sets theme data three icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_three_icon_pane_theme_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_associate_tab_pane_type_data(&MMI_tab_control, &MMI_tab_icon_data);
    MMI_tab_icon_data.highlight_image = get_image(TAB_PANE_ICON_HIGHLIGHT);
    MMI_tab_icon_data.un_highlight_image = get_image(TAB_PANE_ICON_UN_HIGHLIGHT);
}


/*****************************************************************************
 * FUNCTION
 *  set_three_icon_tab_pane
 * DESCRIPTION
 *  sets three icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_three_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_register_set_tab_pane_callback(
        &MMI_tab_control,
        MMI_tab_pane_callback,
        MMI_set_tab_pane_data_callback,
        MMI_reset_tab_pane_data_callback);
    if (MMI_set_tab_pane_data_callback)
    {
        (*MMI_set_tab_pane_data_callback) ();
    }
    (*MMI_tab_pane_callback) ();
    set_three_icon_pane_theme_data();
}


/*****************************************************************************
 * FUNCTION
 *  set_one_icon_pane_theme_data
 * DESCRIPTION
 *  sets theme data one icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_one_icon_pane_theme_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_associate_tab_pane_type_data(&MMI_tab_control, &MMI_tab_icon_data);
    MMI_tab_icon_data.highlight_image = get_image(TAB_PANE_ICON_HIGHLIGHT);
    MMI_tab_icon_data.un_highlight_image = get_image(TAB_PANE_ICON_UN_HIGHLIGHT);
}


/*****************************************************************************
 * FUNCTION
 *  set_one_icon_tab_pane
 * DESCRIPTION
 *  sets one icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_one_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_register_set_tab_pane_callback(
        &MMI_tab_control,
        MMI_tab_pane_callback,
        MMI_set_tab_pane_data_callback,
        MMI_reset_tab_pane_data_callback);
    if (MMI_set_tab_pane_data_callback)
    {
        (*MMI_set_tab_pane_data_callback) ();
    }
    (*MMI_tab_pane_callback) ();
    set_one_icon_pane_theme_data();
}


/*****************************************************************************
 * FUNCTION
 *  register_set_tab_pane_callback
 * DESCRIPTION
 *  register call backs for tab pane
 * PARAMETERS
 *  callback                [IN]            
 *  set_data_callback       [IN]            
 *  reset_data_callback     [IN]            
 *  a(?)                    [IN/OUT]        
 *  b(?)                    [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void register_set_tab_pane_callback(
        void (*callback) (void),
        void (*set_data_callback) (void),
        void (*reset_data_callback) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_tab_pane_callback = callback;
    MMI_set_tab_pane_data_callback = set_data_callback;
    MMI_reset_tab_pane_data_callback = reset_data_callback;
}


/*****************************************************************************
 * FUNCTION
 *  set_tab_pane
 * DESCRIPTION
 *  set function for tab pane
 * PARAMETERS
 *  number_of_items     [IN]            
 *  list_of_items       [?]             
 *  list_of_icons       [?]             
 *  default_item        [IN]            
 *  a(?)                [IN/OUT]        
 *  b(?)                [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_tab_pane(S32 number_of_items, U16 *list_of_items, U16 *list_of_icons, S32 default_item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_associate_tab_pane_item_data(&MMI_tab_control, (struct_tab_items*) & MMI_tab_items);
    for (i = 0; i < number_of_items; i++)
    {
        add_item_tab_pane(get_string(list_of_items[i]), get_image(list_of_icons[i]));
    }
    goto_tab_pane_item(default_item);
    save_current_parent_id();
}


/*****************************************************************************
 * FUNCTION
 *  add_item_tab_pane
 * DESCRIPTION
 *  add item to tab pane
 * PARAMETERS
 *  text        [IN]            
 *  icon        [IN]            
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void add_item_tab_pane(UI_string_type text, PU8 icon)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_tab_control.n_items == MAX_TAB_ITEMS)
    {
        return;
    }
    gui_add_item_tab_pane(&MMI_tab_control, text, icon);
}


/*****************************************************************************
 * FUNCTION
 *  goto_tab_pane_item
 * DESCRIPTION
 *  go to a item in tab pane
 * PARAMETERS
 *  index       [IN]            
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void goto_tab_pane_item(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_go_to_item(&MMI_tab_control, index);
}


/*****************************************************************************
 * FUNCTION
 *  tab_pane_goto_previous_item
 * DESCRIPTION
 *  go to a previous item in tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void tab_pane_goto_previous_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_tab_control.highlighted_item == 0 && !(MMI_tab_control.flags & UI_TAB_CONTROL_CYCLIC))
    {
        return;
    }
    set_list_history = 1;
    if (enable_function_execution)
    {
        enable_tab_pane_display();
        /* MMI_tab_control.highlighted_item--;
           goto_tab_pane_item(MMI_tab_control.highlighted_item); */
        allow_history_saving = 0;
        gui_tab_pane_prev_item(&MMI_tab_control);
        disable_history_saving();
        SetParentHandler((S16) MMI_tab_control.parent_id);
        tab_pane_goto_current_highlighted_item();
        enable_history_saving();
        register_tab_pane_keys();
        disable_tab_pane_display();
    }
    else
    {
        gui_tab_pane_prev_item(&MMI_tab_control);
        tab_pane_goto_current_highlighted_item();
        register_tab_pane_keys();
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gui_lock_double_buffer();
    #endif 
        show_tab_pane();
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gui_unlock_double_buffer();
        gui_BLT_double_buffer(
            MMI_tab_control.x,
            MMI_tab_control.y,
            MMI_tab_control.x + MMI_tab_control.width - 1,
            MMI_tab_control.y + MMI_tab_control.height - 1);
    #endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
    }
    set_list_history = 0;
}


/*****************************************************************************
 * FUNCTION
 *  tab_pane_goto_next_item
 * DESCRIPTION
 *  go to a next item in tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void tab_pane_goto_next_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_tab_control.highlighted_item == MMI_tab_control.n_items - 1 &&
        !(MMI_tab_control.flags & UI_TAB_CONTROL_CYCLIC))
    {
        return;
    }
    set_list_history = 1;
    if (enable_function_execution)
    {
        enable_tab_pane_display();
        /* MMI_tab_control.highlighted_item++;
           goto_tab_pane_item(MMI_tab_control.highlighted_item); */
        allow_history_saving = 0;
        gui_tab_pane_next_item(&MMI_tab_control);
        disable_history_saving();
        SetParentHandler((S16) MMI_tab_control.parent_id);
        tab_pane_goto_current_highlighted_item();
        enable_history_saving();
        register_tab_pane_keys();
        disable_tab_pane_display();
    }
    else
    {
        gui_tab_pane_next_item(&MMI_tab_control);
        tab_pane_goto_current_highlighted_item();
        register_tab_pane_keys();
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gui_lock_double_buffer();
    #endif 
        show_tab_pane();
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gui_unlock_double_buffer();
        gui_BLT_double_buffer(
            MMI_tab_control.x,
            MMI_tab_control.y,
            MMI_tab_control.x + MMI_tab_control.width - 1,
            MMI_tab_control.y + MMI_tab_control.height - 1);
    #endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
    }
    set_list_history = 0;
}


/*****************************************************************************
 * FUNCTION
 *  tab_pane_goto_current_highlighted_item
 * DESCRIPTION
 *  go to a curent hightlighted item in tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void tab_pane_goto_current_highlighted_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
    set_prev_menu_shortcut(MMI_menu_shortcut_number);
#else 
    set_previous_title_shortcut(MMI_menu_shortcut_number);
#endif 
#endif /* (TITLE_TRANSITION_STYLE_NON_BLOCKING) */ 
     
    if (enable_function_execution)
    {
        gdi_layer_lock_frame_buffer();
        tab_pane_hightlight_function(MMI_tab_control.highlighted_item);
        gdi_layer_unlock_frame_buffer();
        SetCurrExitFuncPtr(backup_exit_func_ptr);
        if (backup_category_func_ptr)
        {
            GetCategoryHistory = backup_category_func_ptr;
        }
        execute_left_softkey_function(KEY_EVENT_UP);
        backup_current_entry_function();
        backup_current_category_function();
    }
    else
    {
        tab_pane_hightlight_function(MMI_tab_control.highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_tab_pane_hightlight_function
 * DESCRIPTION
 *  sets tab pane hightlight function
 * PARAMETERS
 *  f           [IN]            
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_tab_pane_hightlight_function(void (*f) (S32 item_index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_set_tab_pane_hightlight_fucntion(&MMI_tab_control, f);
}


/*****************************************************************************
 * FUNCTION
 *  tab_pane_hightlight_function
 * DESCRIPTION
 *  highligth function for tab pane
 * PARAMETERS
 *  index       [IN]            
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void tab_pane_hightlight_function(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_highlight_function(&MMI_tab_control, index);
}


/*****************************************************************************
 * FUNCTION
 *  disable_tab_pane_keys
 * DESCRIPTION
 *  disable tab pane keys registeration. this is required when entry gunction is not directly called
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void disable_tab_pane_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    disable_tab_pane_register = 1;
}


/*****************************************************************************
 * FUNCTION
 *  register_tab_pane_keys
 * DESCRIPTION
 *  registers tab pane keys
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void register_tab_pane_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!disable_tab_pane_register)
    {
        SetKeyHandler(tab_pane_goto_previous_item, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(tab_pane_goto_next_item, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(NULL, KEY_LEFT_ARROW, KEY_EVENT_UP);
        SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_UP);
    }
    disable_tab_pane_register = 0;
}


/*****************************************************************************
 * FUNCTION
 *  register_tab_pane_hilite_handler
 * DESCRIPTION
 *  registers tab pane highlight handler
 * PARAMETERS
 *  f           [IN]            
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void register_tab_pane_hilite_handler(void (*f) (S32 item_index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_tab_highlight_handler = f;
}


/*****************************************************************************
 * FUNCTION
 *  standard_tab_pane_highlight_function
 * DESCRIPTION
 *  standard tab pane highlight handler
 * PARAMETERS
 *  index       [IN]            
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void standard_tab_pane_highlight_function(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_tab_highlight_handler(index);
}


/*****************************************************************************
 * FUNCTION
 *  enable_tab_pane_display
 * DESCRIPTION
 *  enables tab pane display
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void enable_tab_pane_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_enable_tab_pane_display = 1;
}


/*****************************************************************************
 * FUNCTION
 *  disable_tab_pane_display
 * DESCRIPTION
 *  disables tab pane display
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void disable_tab_pane_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_enable_tab_pane_display = 0;
}


/*****************************************************************************
 * FUNCTION
 *  is_enabled_tab_pane
 * DESCRIPTION
 *  used to check whether tab pane is enabled or not
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  
 *****************************************************************************/
S32 is_enabled_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_enable_tab_pane_display;
}


/*****************************************************************************
 * FUNCTION
 *  show_tab_pane
 * DESCRIPTION
 *  shows tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void show_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_show_tab_pane(&MMI_tab_control);
}


/*****************************************************************************
 * FUNCTION
 *  save_current_parent_id
 * DESCRIPTION
 *  saves cureent parent id
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void save_current_parent_id(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_save_parent_id(&MMI_tab_control, GetParentHandler());
}
extern U8 *(*GetCategoryHistory) (U8 *buffer);


/*****************************************************************************
 * FUNCTION
 *  backup_current_category_function
 * DESCRIPTION
 *  backups current category history fucntion
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void backup_current_category_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    backup_category_func_ptr = GetCategoryHistory;
    GetCategoryHistory = get_tab_pane_and_parent_category_history;
}


/*****************************************************************************
 * FUNCTION
 *  tab_set_from_history
 * DESCRIPTION
 *  entry function if tab pane is enabled on some screen
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void tab_set_from_history(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_TITLE_TRANSITION__
    S32 title_trans;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    set_list_history = 1;

    enable_tab_pane_display();
    process_history_buffer();
    if (MMI_tab_control.category_entry_callback)
    {
        MMI_tab_control.category_entry_callback();
    }
    SetParentHandler((S16) MMI_tab_control.parent_id);
     
#ifdef __MMI_UI_TITLE_TRANSITION__
    title_trans = get_title_transition();
    if (title_trans)
    {
        reset_title_transition();
    }
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
     
    if (enable_function_execution)
    {
        disable_history_saving();
        gdi_layer_lock_frame_buffer();
        tab_pane_hightlight_function(MMI_tab_control.highlighted_item);
        gdi_layer_unlock_frame_buffer();
        execute_left_softkey_function(KEY_EVENT_UP);
        backup_current_entry_function();
        backup_current_category_function();
        enable_history_saving();
    }
    register_tab_pane_keys();
    disable_tab_pane_display();
     
#ifdef __MMI_UI_TITLE_TRANSITION__
    if (title_trans)
    {
        set_title_transition(title_trans);
    }
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
     
    set_list_history = 0;
}


/*****************************************************************************
 * FUNCTION
 *  tab_exit_function
 * DESCRIPTION
 *  exit function if tab pane is enabled on some screen
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void tab_exit_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_tab_control.reset_data_callback)
    {
        MMI_tab_control.reset_data_callback();
    }
    if (backup_exit_func_ptr)
    {
        backup_exit_func_ptr();
    }
    if (MMI_tab_control.category_exit_callback)
    {
        MMI_tab_control.category_exit_callback();
    }
    backup_exit_func_ptr = NULL;
    allow_history_saving = 1;
    SetKeyHandler(NULL, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  backup_current_exit_function
 * DESCRIPTION
 *  backups current exit function
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void backup_current_exit_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    backup_exit_func_ptr = GetCurrExitFuncPtr();
    SetCurrExitFuncPtr(tab_exit_function);
}


/*****************************************************************************
 * FUNCTION
 *  backup_current_entry_function
 * DESCRIPTION
 *  backups current entry function
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void backup_current_entry_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    backup_entry_func_ptr = GetCurrEntryFuncPtr();
    SetCurrEntryFuncPtr(tab_set_from_history);
    backup_current_exit_function();
}


/*****************************************************************************
 * FUNCTION
 *  create_and_setup_two_icon_tab_pane
 * DESCRIPTION
 *  creates abd setups two icon tab panae
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_and_setup_two_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    create_two_icon_tab_pane();
    set_two_icon_tab_pane();
    set_tab_pane_hightlight_function(standard_tab_pane_highlight_function);
}


/*****************************************************************************
 * FUNCTION
 *  create_and_setup_two_text_tab_pane
 * DESCRIPTION
 *  creates abd setups two text tab panae
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_and_setup_two_text_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    create_two_text_tab_pane();
    set_two_text_tab_pane();
    set_tab_pane_hightlight_function(standard_tab_pane_highlight_function);
}


/*****************************************************************************
 * FUNCTION
 *  create_and_setup_four_icon_tab_pane
 * DESCRIPTION
 *  creates abd setups four icon tab panae
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_and_setup_four_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    create_four_icon_tab_pane();
    set_four_icon_tab_pane();
    set_tab_pane_hightlight_function(standard_tab_pane_highlight_function);
}


/*****************************************************************************
 * FUNCTION
 *  create_and_setup_three_icon_tab_pane
 * DESCRIPTION
 *  creates and setups three icon tab panae
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_and_setup_three_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    create_three_icon_tab_pane();
    set_three_icon_tab_pane();
    set_tab_pane_hightlight_function(standard_tab_pane_highlight_function);
}


/*****************************************************************************
 * FUNCTION
 *  create_and_setup_one_icon_tab_pane
 * DESCRIPTION
 *  creates and setups three icon tab panae
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void create_and_setup_one_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    create_one_icon_tab_pane();
    set_one_icon_tab_pane();
    set_tab_pane_hightlight_function(standard_tab_pane_highlight_function);
}


/*****************************************************************************
 * FUNCTION
 *  process_history_buffer
 * DESCRIPTION
 *  process tab pane history when the tab pane needs to be displayed from hsitory
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void process_history_buffer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *history_buffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset((void*)&MMI_tab_control, 0, sizeof(MMI_tab_control));
    history_buffer = GetCurrGuiBuffer(GetCurrScrnId());
    set_tab_pane_category_history(history_buffer);
    history_buffer += get_tab_pane_history_size();
    change_curr_gui_buffer(history_buffer);

}


/*****************************************************************************
 * FUNCTION
 *  get_tab_pane_history_size
 * DESCRIPTION
 *  returns tab pane history size
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  
 *****************************************************************************/
S32 get_tab_pane_history_size(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (sizeof(struct_tab_pane_history));
}


/*****************************************************************************
 * FUNCTION
 *  get_tab_pane_history
 * DESCRIPTION
 *  returns tab pane history
 * PARAMETERS
 *  history_buffer      [?]             
 *  a(?)                [IN/OUT]        
 *  b(?)                [IN]            
 * RETURNS
 *  
 *****************************************************************************/
U8 *get_tab_pane_history(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {

        struct_tab_pane_history *tab_history = (struct_tab_pane_history*) history_buffer;

        tab_history->first_displayed_item = MMI_tab_control.first_displayed_item;
        tab_history->highlighted_item = MMI_tab_control.highlighted_item;
        tab_history->last_displayed_item = MMI_tab_control.highlighted_item;
        tab_history->parent_id = MMI_tab_control.parent_id;
        tab_history->set_callback = MMI_tab_control.set_callback;
        tab_history->set_data_callback = MMI_tab_control.set_data_callback;
        tab_history->reset_data_callback = MMI_tab_control.reset_data_callback;
        tab_history->type = MMI_tab_control.type;
        tab_history->highlighted_pane = MMI_tab_control.highlighted_pane;
        tab_history->category_exit_callback = MMI_tab_control.category_exit_callback;
        tab_history->category_entry_callback = MMI_tab_control.category_entry_callback;
        tab_history->tab_list_history = MMI_tab_control.tab_list_history;
    }
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  get_tab_pane_and_parent_category_history
 * DESCRIPTION
 *  new history size function registered by tab pane
 * PARAMETERS
 *  history_buffer      [?]             
 *  a(?)                [IN/OUT]        
 *  b(?)                [IN]            
 * RETURNS
 *  
 *****************************************************************************/
U8 *get_tab_pane_and_parent_category_history(U8 *history_buffer)
{
#ifdef MMI_TAB_PANE_LIST_HISTORY
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    {
        list_menu_category_history list_history;

        /*----------------------------------------------------------------*/
        /* Code Body                                                      */
        /*----------------------------------------------------------------*/
        get_list_menu_category_history(1, (U8*) & list_history);
        MMI_tab_control.tab_list_history.first_displayed_item[MMI_tab_control.highlighted_item]
            = list_history.first_displayed_item;
        MMI_tab_control.tab_list_history.highlighted_item[MMI_tab_control.highlighted_item]
            = list_history.highlighted_item;
        MMI_tab_control.tab_list_history.last_displayed_item[MMI_tab_control.highlighted_item]
            = list_history.last_displayed_item;
    }
#endif /* MMI_TAB_PANE_LIST_HISTORY */ 
    get_tab_pane_history(history_buffer);
#ifndef MMI_TAB_PANE_LIST_HISTORY
    backup_category_func_ptr(history_buffer + get_tab_pane_history_size());
#else 
    memset(history_buffer + get_tab_pane_history_size(), 0, 4);
#endif 
    return (history_buffer);

}


/*****************************************************************************
 * FUNCTION
 *  set_tab_pane_category_history
 * DESCRIPTION
 *  sets tab pane history
 * PARAMETERS
 *  history_buffer      [?]             
 *  a(?)                [IN/OUT]        
 *  b(?)                [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_tab_pane_category_history(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        struct_tab_pane_history *tab_history = (struct_tab_pane_history*) history_buffer;

        register_set_tab_pane_callback(
            tab_history->set_callback,
            tab_history->set_data_callback,
            tab_history->reset_data_callback);
        switch (tab_history->type)
        {
            case TAB_CONTROL_WITH_TWO_ICON:
                create_and_setup_two_icon_tab_pane();
                break;
            case TAB_CONTROL_WITH_TWO_TEXT:
                create_and_setup_two_text_tab_pane();
                break;
            case TAB_CONTROL_WITH_FOUR_ICON:
                create_and_setup_four_icon_tab_pane();
                break;
            case TAB_CONTROL_WITH_THREE_ICON:
                create_and_setup_three_icon_tab_pane();
                break;
            case TAB_CONTROL_WITH_ONE_ICON:
                create_and_setup_one_icon_tab_pane();
                break;
        }
        MMI_tab_control.first_displayed_item = tab_history->first_displayed_item;
        MMI_tab_control.highlighted_item = tab_history->highlighted_item;
        MMI_tab_control.highlighted_item = tab_history->last_displayed_item;
        MMI_tab_control.parent_id = tab_history->parent_id;
        MMI_tab_control.highlighted_pane = tab_history->highlighted_pane;
        MMI_tab_control.category_exit_callback = tab_history->category_exit_callback;
        MMI_tab_control.category_entry_callback = tab_history->category_entry_callback;
        MMI_tab_control.tab_list_history = tab_history->tab_list_history;
    }
}


/*****************************************************************************
 * FUNCTION
 *  tab_pane_disable_function_exection
 * DESCRIPTION
 *  disbale tab pane function execution on highlighting
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void tab_pane_disable_function_exection(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    enable_function_execution = 0;
}


/*****************************************************************************
 * FUNCTION
 *  set_pre_tab_pane_display
 * DESCRIPTION
 *  tab pane function to be called in function entry if the entry function is not called directly.
 *  needs to be called before EntryNewScreen
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_pre_tab_pane_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetCurrEntryFuncPtr(backup_entry_func_ptr);
    if (!allow_history_saving)
    {
        disable_history_saving();
    }
    SetCurrExitFuncPtr(backup_exit_func_ptr);
    if (backup_category_func_ptr)
    {
        GetCategoryHistory = backup_category_func_ptr;
    }
    enable_tab_pane_display();
}


/*****************************************************************************
 * FUNCTION
 *  set_post_tab_pane_display
 * DESCRIPTION
 *  tab pane function to be called in function entry if the entry function is not called directly.
 *  needs to be called after the keys have been registered.
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_post_tab_pane_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    backup_current_entry_function();
    backup_current_category_function();
    register_tab_pane_keys();
    disable_tab_pane_display();
    enable_history_saving();
}


/*****************************************************************************
 * FUNCTION
 *  set_tab_pane_category_exit_function
 * DESCRIPTION
 *  sets tab pane category exit function
 * PARAMETERS
 *  f           [IN]            
 *  a(?)        [IN/OUT]        F: category exit function
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_tab_pane_category_exit_function(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_set_tab_pane_category_exit_function(&MMI_tab_control, f);
}


/*****************************************************************************
 * FUNCTION
 *  set_tab_pane_category_entry_function
 * DESCRIPTION
 *  sets tab pane category entry function
 * PARAMETERS
 *  f           [IN]            
 *  a(?)        [IN/OUT]        F: category entry function
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_tab_pane_category_entry_function(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_set_tab_pane_category_entry_function(&MMI_tab_control, f);
}


/*****************************************************************************
 * FUNCTION
 *  get_current_tab_pane_height
 * DESCRIPTION
 *  returns current tab pane height
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        NONE
 *  b(?)        [IN]            
 * RETURNS
 *  
 *****************************************************************************/
S32 get_current_tab_pane_height(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_tab_control.height;
}


/*****************************************************************************
 * FUNCTION
 *  is_tab_pane_displayed_on_current_screen
 * DESCRIPTION
 *  this function is used to cjeck if tab pane is being displayed on current screen
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  
 *****************************************************************************/
S32 is_tab_pane_displayed_on_current_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GetCurrEntryFuncPtr() == tab_set_from_history)
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
 *  set_list_in_tab_history
 * DESCRIPTION
 *  this function is used to set the list history when scrolling left to right
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  
 *****************************************************************************/
S32 set_list_in_tab_history(void)
{
#ifndef MMI_TAB_PANE_LIST_HISTORY
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return 0;
#else /* MMI_TAB_PANE_LIST_HISTORY */ 
    if (set_list_history)
    {
        if (MMI_tab_control.tab_list_history.highlighted_item[MMI_tab_control.highlighted_item] == 0
            && MMI_tab_control.tab_list_history.first_displayed_item[MMI_tab_control.highlighted_item] == 0
            && MMI_tab_control.tab_list_history.last_displayed_item[MMI_tab_control.highlighted_item] == 0)
        {
            return 0;
        }
        MMI_fixed_list_menu.highlighted_item =
            MMI_tab_control.tab_list_history.highlighted_item[MMI_tab_control.highlighted_item];
        MMI_fixed_list_menu.first_displayed_item =
            MMI_tab_control.tab_list_history.first_displayed_item[MMI_tab_control.highlighted_item];
        MMI_fixed_list_menu.last_displayed_item =
            MMI_tab_control.tab_list_history.last_displayed_item[MMI_tab_control.highlighted_item];
        return 1;
    }
    return 0;
#endif /* MMI_TAB_PANE_LIST_HISTORY */ 
}


/*****************************************************************************
 * FUNCTION
 *  tab_pane_modify_data
 * DESCRIPTION
 *  this function is used to modify the data when any category is changed if required by tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void tab_pane_modify_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_pane_modify_data_ptr)
    {
        tab_pane_modify_data_ptr();
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_tab_pane_modify_data_function
 * DESCRIPTION
 *  this function is used to set the tab pane modify function by category screen
 * PARAMETERS
 *  ptr         [IN]            
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_tab_pane_modify_data_function(void (*ptr) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_pane_modify_data_ptr = ptr;
}


/*****************************************************************************
 * FUNCTION
 *  reset_tab_pane_modify_data_function
 * DESCRIPTION
 *  this function sets the tab pane modify function to NULL
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void reset_tab_pane_modify_data_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_pane_modify_data_ptr = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  set_tab_pane_modify_title_icon
 * DESCRIPTION
 *  this function sets the tab pane modify function to NULL
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_tab_pane_modify_title_icon(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_icon = MMI_tab_control.item_data[MMI_tab_control.highlighted_item].item_icon;
}


/*****************************************************************************
 * FUNCTION
 *  set_tab_pane_cyclic
 * DESCRIPTION
 *  this function sets the tab pane in cyclic mode
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_tab_pane_cyclic(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_set_tab_pane_flags(&MMI_tab_control, MMI_tab_control.flags | UI_TAB_CONTROL_CYCLIC);
}

#endif /* __MMI_UI_TAB_PANE__ */ 

