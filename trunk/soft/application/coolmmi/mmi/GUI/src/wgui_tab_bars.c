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
 *   wgui_tab_bars.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Tab Bar - Category component
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/*  Include: MMI header file */
#include "wgui_softkeys.h"
#include "wgui_tab_bars.h"
#include "gui_setting.h"
#include "debuginitdef.h"
#include "historygprot.h"
extern U16 title_bg_id;
#ifdef __MMI_UI_TAB_PANE__

/***************************************************************************** 
* Define
*****************************************************************************/
/* To restore the first displayed item after tab bar is scrolled with pen */
#define WGUI_TAB_BAR_RESTORE_DISPLAY_TIME       (3000)

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/

#ifdef __MMI_TOUCH_SCREEN__

/* Default value is -1. Used to store the display area of tab bar after pen is up. */
S16 MMI_horizontal_tab_bar_first_displayed_before_pen_down = -1;

/* Default value is -1. Used to set  the display area of tab bar when it enters a new screen. */
S16 MMI_horizontal_tab_bar_first_displayed_on_screen_switched = -1;

/* Used by wgui_horizontal_tab_bar_translate_pen_event() */
void (*MMI_horizontal_tab_bar_select_callback) (int index);

#endif /* __MMI_TOUCH_SCREEN__ */ 

/***************************************************************************** 
* Local Function
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_reset_title_bg_id
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_horizontal_tab_bar_reset_title_bg_id(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    title_bg_id = 0;
}

/***************************************************************************** 
* Global Variable
*****************************************************************************/

horizontal_tab_bar MMI_horizontal_tab_bar;

tab_bar_item_type MMI_tab_bar_items[UI_MAX_TAB_BAR_ITEMS];

/***************************************************************************** 
* Global Function
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  wgui_create_horizontal_tab_bar
 * DESCRIPTION
 *  Cerate horizontal tab bar component in category
 * PARAMETERS
 *  contain_hint            [IN]        Whether the tab bar contains hint area
 *  hint                    [IN]        
 *  n_item                  [IN]        
 *  focused_item            [IN]        
 *  keep_pen_context        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_create_horizontal_tab_bar(
        BOOL contain_hint,
        UI_string_type hint,
        S8 n_item,
        S8 focused_item,
        MMI_BOOL keep_pen_context)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_item > UI_MAX_TAB_BAR_ITEMS)
    {
        n_item = UI_MAX_TAB_BAR_ITEMS;
    }

    gui_create_horizontal_tab_bar(
        &MMI_horizontal_tab_bar,
        MMI_HORIZONTAL_TAB_BAR_X,
        MMI_HORIZONTAL_TAB_BAR_Y,
        MMI_HORIZONTAL_TAB_BAR_WIDTH,
        MMI_HORIZONTAL_TAB_BAR_TAB_AREA_HEIGHT,
        (S16) (contain_hint ? MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT : 0),
        MMI_HORIZONTAL_TAB_BAR_TAB_WIDTH,
        MMI_HORIZONTAL_TAB_BAR_FOCUSED_TAB_WIDTH,
        hint,
        n_item,
        focused_item,
        MMI_tab_bar_items);

    MMI_horizontal_tab_bar.flags |= UI_TAB_BAR_LOOP;
    gui_horizontal_tab_bar_set_icon_position(&MMI_horizontal_tab_bar, 13, 5);

#ifdef __MMI_TOUCH_SCREEN__
    if (!keep_pen_context)
    {
        MMI_horizontal_tab_bar_select_callback = NULL;
        MMI_horizontal_tab_bar_first_displayed_before_pen_down = -1;
        if (MMI_horizontal_tab_bar_first_displayed_on_screen_switched >= 0)
        {
            gui_set_horizontal_tab_bar_display_range(
                &MMI_horizontal_tab_bar,
                MMI_horizontal_tab_bar_first_displayed_on_screen_switched);
        }
    }
#endif /* __MMI_TOUCH_SCREEN__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_close_horizontal_tab_bar
 * DESCRIPTION
 *  Close the horizontal tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_close_horizontal_tab_bar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_tab_bar_stop_blinking(&MMI_horizontal_tab_bar);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_horizontal_tab_bar
 * DESCRIPTION
 *  Show the horizontal tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_horizontal_tab_bar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_show_horizontal_tab_bar(&MMI_horizontal_tab_bar, MMI_FALSE, MMI_FALSE);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(
        MMI_horizontal_tab_bar.x,
        MMI_horizontal_tab_bar.y,
        MMI_horizontal_tab_bar.x + MMI_horizontal_tab_bar.width - 1,
        MMI_horizontal_tab_bar.y + MMI_horizontal_tab_bar.height - 1);

    if (MMI_horizontal_tab_bar.theme->hint_bg_image && title_bg_id == 0)
    {
        title_bg_id = MMI_horizontal_tab_bar.theme->hint_bg_image;
        gui_add_cleanup_hook(wgui_horizontal_tab_bar_reset_title_bg_id);
    }

    gui_horizontal_tab_bar_start_blinking(&MMI_horizontal_tab_bar);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_horizontal_tab_bar_hint_area
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_horizontal_tab_bar_hint_area(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_show_horizontal_tab_bar(&MMI_horizontal_tab_bar, MMI_TRUE, MMI_FALSE);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(
        MMI_horizontal_tab_bar.x,
        MMI_horizontal_tab_bar.y + MMI_horizontal_tab_bar.tab_area_height,
        MMI_horizontal_tab_bar.x + MMI_horizontal_tab_bar.width - 1,
        MMI_horizontal_tab_bar.y + MMI_horizontal_tab_bar.height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_horizontal_tab_bar_tab_area
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 
 *  Used in category 435 that popup overlaps with hint area, and we do not want to redraw the
 *  whole screen. As a result, we redraw the tab area only.
 *****************************************************************************/
void wgui_show_horizontal_tab_bar_tab_area(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_show_horizontal_tab_bar(&MMI_horizontal_tab_bar, MMI_FALSE, MMI_TRUE);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(
        MMI_horizontal_tab_bar.x,
        MMI_horizontal_tab_bar.y,
        MMI_horizontal_tab_bar.x + MMI_horizontal_tab_bar.width - 1,
        MMI_horizontal_tab_bar.y + MMI_horizontal_tab_bar.tab_area_height - 1);

    gui_horizontal_tab_bar_start_blinking(&MMI_horizontal_tab_bar);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_move_horizontal_tab_bar
 * DESCRIPTION
 *  Move the horizontal tab bar
 * PARAMETERS
 *  x       [IN]        
 *  y       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_move_horizontal_tab_bar(S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_move_horizontal_tab_bar(&MMI_horizontal_tab_bar, x, y);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_goto_home
 * DESCRIPTION
 *  Focus the first item of horizontal tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_horizontal_tab_bar_goto_home(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_tab_bar_focus_first_item(&MMI_horizontal_tab_bar);
    wgui_show_horizontal_tab_bar();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_goto_end
 * DESCRIPTION
 *  Focus the last item of horizontal tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_horizontal_tab_bar_goto_end(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_tab_bar_focus_last_item(&MMI_horizontal_tab_bar);
    wgui_show_horizontal_tab_bar();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_goto_item
 * DESCRIPTION
 *  Focus certain item of horizontal tab bar
 * PARAMETERS
 *  index       [IN]        Item index
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_horizontal_tab_bar_goto_item(int index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_tab_bar_focus_item(&MMI_horizontal_tab_bar, (S8) index);
    wgui_show_horizontal_tab_bar();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_goto_next_item
 * DESCRIPTION
 *  Focus the next item of horizontal tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_horizontal_tab_bar_goto_next_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_tab_bar_focus_next_item(&MMI_horizontal_tab_bar);
    wgui_show_horizontal_tab_bar();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_goto_prev_item
 * DESCRIPTION
 *  Focus the previous item of horizontal tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_horizontal_tab_bar_goto_prev_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_tab_bar_focus_prev_item(&MMI_horizontal_tab_bar);
    wgui_show_horizontal_tab_bar();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_start_blinking
 * DESCRIPTION
 *  Start the blinking of certain tab
 * PARAMETERS
 *  index       [IN]        Index of the item
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_horizontal_tab_bar_start_blinking(int index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_tab_bar_items[index].flags |= UI_TAB_BAR_ITEM_STATE_BLINKING;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_stop_blinking
 * DESCRIPTION
 *  Stop the blinking of certain tab
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  index    IN    index of the item(?)
 *****************************************************************************/
void wgui_horizontal_tab_bar_stop_blinking(int index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 old_flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    old_flags = MMI_tab_bar_items[index].flags;

    MMI_tab_bar_items[index].flags &= ~UI_TAB_BAR_ITEM_STATE_BLINKING;

    /* We need to refresh the tab bar */
    if ((old_flags & UI_TAB_BAR_ITEM_STATE_BLINKING) &&
        gui_horizontal_tab_bar_is_item_displayed(&MMI_horizontal_tab_bar, index))
    {
        wgui_show_horizontal_tab_bar();
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_get_horizontal_tab_bar_history
 * DESCRIPTION
 *  Focus the previous item of horizontal tab bar
 * PARAMETERS
 *  history     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_get_horizontal_tab_bar_history(wgui_tab_bar_history_struct *history)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    history->flags = MMI_horizontal_tab_bar.flags;
    history->first_displayed_item = MMI_horizontal_tab_bar.first_displayed_item;
    history->last_displayed_item = MMI_horizontal_tab_bar.last_displayed_item;
    history->focused_item = MMI_horizontal_tab_bar.focused_item;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_set_horizontal_tab_bar_history
 * DESCRIPTION
 *  Focus the previous item of horizontal tab bar
 * PARAMETERS
 *  history     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_horizontal_tab_bar_history(wgui_tab_bar_history_struct *history)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_horizontal_tab_bar.flags = history->flags;
    MMI_horizontal_tab_bar.first_displayed_item = history->first_displayed_item;
    MMI_horizontal_tab_bar.last_displayed_item = history->last_displayed_item;
    MMI_horizontal_tab_bar.focused_item = history->focused_item;
}

#ifdef __MMI_WGUI_MINI_TAB_BAR__

gui_mini_tab_bar_struct wgui_mini_tab_bar;


/*****************************************************************************
 * FUNCTION
 *  wgui_init_mini_tab_bar
 * DESCRIPTION
 *  Initialize mini tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_init_mini_tab_bar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 idx = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_mini_tab_bar.flags = 0;
    wgui_mini_tab_bar.num_of_items = 0;

}


/*****************************************************************************
 * FUNCTION
 *  wgui_setup_mini_tab_bar
 * DESCRIPTION
 *  Setup mini tab bar data
 * PARAMETERS
 *  x                   [IN]        X-coordinate of mini tab bar
 *  y                   [IN]        Y-coordinate of mini tab bar
 *  width               [IN]        Width of mini tab bar
 *  height              [IN]        Height of mini tab bar
 *  bg_color            [IN]        Background color of mini tab bar
 *  selected_color      [IN]        Selected item color
 *  normal_color        [IN]        Normal item color
 *  count               [IN]        Number of items in mini tab bar
 *  menu_item_id        [IN]        Menu id array of each item in mini tab bar
 *  item_entry_func     [IN]        Entry function array of each item in mini tab bar
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_setup_mini_tab_bar(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        color_t bg_color,
        color_t selected_color,
        color_t normal_color,
        U8 count,
        U16 *menu_item_id,
        FuncPtr *item_entry_func)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 idx = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_mini_tab_bar.x = x;
    wgui_mini_tab_bar.y = y;
    wgui_mini_tab_bar.width = width;
    wgui_mini_tab_bar.height = height;
    wgui_mini_tab_bar.bg_color = bg_color;
    wgui_mini_tab_bar.selected_item_color = selected_color;
    wgui_mini_tab_bar.normal_item_color = normal_color;
    wgui_mini_tab_bar.flags = 0;

    wgui_mini_tab_bar.num_of_items = count;

    MMI_ASSERT(menu_item_id != NULL);

    for (idx = 0; idx < count; idx++)
    {
        wgui_mini_tab_bar.menu_item_id[idx] = menu_item_id[idx];
        if (item_entry_func != NULL)
        {
            wgui_mini_tab_bar.item_entry_func[idx] = item_entry_func[idx];
        }
    }

}


/*****************************************************************************
 * FUNCTION
 *  wgui_if_mini_tab_bar_enable
 * DESCRIPTION
 *  check if mini tab bar is enabled
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL MMI_TRUE for enabled mini tab bar, MMI_FALSE else
 *****************************************************************************/
MMI_BOOL wgui_if_mini_tab_bar_enable(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_mini_tab_bar.flags & WGUI_MINI_TAB_BAR_ENABLED)
    {
        return MMI_TRUE;
    }
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_if_mini_tab_bar_in_parent
 * DESCRIPTION
 *  check if current screen is the parent of mini tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL MMI_TRUE for yes, MMI_FALSE else
 *****************************************************************************/
MMI_BOOL wgui_if_mini_tab_bar_in_parent(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_mini_tab_bar.flags & WGUI_MINI_TAB_BAR_IN_PARENT)
    {
        return MMI_TRUE;
    }
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_enable_mini_tab_bar
 * DESCRIPTION
 *  Enable mini tab bar
 * PARAMETERS
 *  item_id     [IN]        Enable the mini tab bar and entry the item with id=item_id
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_enable_mini_tab_bar(U16 item_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 idx = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (idx = 0; idx < wgui_mini_tab_bar.num_of_items; idx++)
        if (wgui_mini_tab_bar.menu_item_id[idx] == item_id)
        {
            wgui_mini_tab_bar.curr_index = idx;
            wgui_mini_tab_bar.flags |= WGUI_MINI_TAB_BAR_ENABLED;
            return;
        }
    /* the item is not included in any mini_tab_bar */
    /* MMI_ASSERT(0); */
}


/*****************************************************************************
 * FUNCTION
 *  wgui_disable_mini_tab_bar
 * DESCRIPTION
 *  Disable mini tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_disable_mini_tab_bar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_mini_tab_bar.flags &= ~WGUI_MINI_TAB_BAR_ENABLED;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_mini_tab_bar_set_flag_on
 * DESCRIPTION
 *  API to turn on flag of mini tab bar
 * PARAMETERS
 *  flag        [IN]        Flag to be on
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_mini_tab_bar_set_flag_on(S32 flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_mini_tab_bar.flags |= flag;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_mini_tab_bar_set_flag_off
 * DESCRIPTION
 *  API to turn off flag of mini tab bar
 * PARAMETERS
 *  flag        [IN]        Flag to be off
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_mini_tab_bar_set_flag_off(S32 flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_mini_tab_bar.flags &= ~flag;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_register_mini_tab_bar_func
 * DESCRIPTION
 *  register items entry function of mini tab bar
 * PARAMETERS
 *  menu_item_id        [IN]        Array of ids of menu items
 *  item_entry_func     [IN]        Array of entry functions of menu items
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_register_mini_tab_bar_func(U16 *menu_item_id, FuncPtr item_entry_func[])
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 idx = 0;
    U8 count = wgui_mini_tab_bar.num_of_items;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(item_entry_func != NULL);

    for (idx = 0; idx < count; idx++)
    {
        if (wgui_mini_tab_bar.menu_item_id[idx] == menu_item_id[idx])
        {
            wgui_mini_tab_bar.item_entry_func[idx] = item_entry_func[idx];
            return;
        }
    }

    MMI_ASSERT(0);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_mini_tab_bar_goto_next
 * DESCRIPTION
 *  Goto next item of mini tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_mini_tab_bar_goto_next(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* increase index */
    if (wgui_mini_tab_bar.curr_index == wgui_mini_tab_bar.num_of_items - 1)
    {
        wgui_mini_tab_bar.curr_index = 0;
    }
    else
    {
        wgui_mini_tab_bar.curr_index++;
    }

    /* execute entry function */
    ExecuteItemHiliteHandler(wgui_mini_tab_bar.menu_item_id[wgui_mini_tab_bar.curr_index]);
    execute_left_softkey_function(KEY_EVENT_UP);

    /* delete toppest history */
    DeleteNHistory(1);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_mini_tab_bar_goto_prev
 * DESCRIPTION
 *  Goto previous item of mini tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_mini_tab_bar_goto_prev(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* decrease index */
    if (wgui_mini_tab_bar.curr_index == 0)
    {
        wgui_mini_tab_bar.curr_index = wgui_mini_tab_bar.num_of_items - 1;
    }
    else
    {
        wgui_mini_tab_bar.curr_index--;
    }

    /* execute entry function */
    ExecuteItemHiliteHandler(wgui_mini_tab_bar.menu_item_id[wgui_mini_tab_bar.curr_index]);
    execute_left_softkey_function(KEY_EVENT_UP);

    /* delete toppest history */
    DeleteNHistory(1);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_mini_tab_bar
 * DESCRIPTION
 *  Display the mini tab bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_mini_tab_bar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 idx = 0;
    S32 x1, x2, y1, y2;
    color_t bg_color, selected_color, normal_color;
    U16 bar_width, bar_height;
    U16 cell_width, cell_pad = 2, cell_count;
    U16 left_pad;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = wgui_mini_tab_bar.x;
    x2 = wgui_mini_tab_bar.x + wgui_mini_tab_bar.width - 1;
    y1 = wgui_mini_tab_bar.y;
    y2 = wgui_mini_tab_bar.y + wgui_mini_tab_bar.height - 1;
    cell_count = wgui_mini_tab_bar.num_of_items;
    bar_width = wgui_mini_tab_bar.width;
    bar_height = wgui_mini_tab_bar.height;
    bg_color = wgui_mini_tab_bar.bg_color;
    selected_color = wgui_mini_tab_bar.selected_item_color;
    normal_color = wgui_mini_tab_bar.normal_item_color;

    cell_width = (bar_width - cell_pad * (cell_count + 1)) / cell_count;
    left_pad = (bar_width - (cell_width * cell_count) - (cell_pad * (cell_count - 1))) >> 1;

    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_BLACK);//bg_color);

    for (idx = 0; idx < cell_count; idx++)
    {
        if (idx == wgui_mini_tab_bar.curr_index)    /* selected item */
        {
            gdi_draw_solid_rect(
                left_pad + (cell_pad + cell_width) * idx,
                y1 + 1,
                left_pad + cell_pad * idx + cell_width * (idx + 1),
                y2 - 1,
                GDI_COLOR_BLUE);//selected_color);
        }
        else    /* normal item */
        {
            gdi_draw_solid_rect(
                left_pad + (cell_pad + cell_width) * idx,
                y1 + 1,
                left_pad + cell_pad * idx + cell_width * (idx + 1),
                y2 - 1,
               GDI_COLOR_GREEN);// normal_color);
        }
    }

}


/*****************************************************************************
 * FUNCTION
 *  wgui_mini_tab_bar_get_index
 * DESCRIPTION
 *  Get the current index of mini tab bar.
 * PARAMETERS
 *  void
 * RETURNS
 *  U8    current item index of mini tab bar
 *****************************************************************************/
U8 wgui_mini_tab_bar_get_index(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_mini_tab_bar.curr_index;
}

#endif /* __MMI_WGUI_MINI_TAB_BAR__ */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_set_horizontal_tab_bar_select_callback
 * DESCRIPTION
 *  Focus the previous item of horizontal tab bar
 * PARAMETERS
 *  select_callback     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_horizontal_tab_bar_select_callback(void (*select_callback) (int index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_TOUCH_SCREEN__
    MMI_horizontal_tab_bar_select_callback = select_callback;
#endif 
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  wgui_enable_horizontal_tab_bar_pen_events
 * DESCRIPTION
 *  Handle pen events in horizontal tab bar and
 * PARAMETERS
 *  enable      [IN]        
 *  tb(?)
 *  pen_event(?)
 *  x(?)
 *  y(?)
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_enable_horizontal_tab_bar_pen_events(BOOL enable)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (enable)
    {
        MMI_horizontal_tab_bar.flags &= ~UI_TAB_BAR_DISABLE_PEN;
    }
    else
    {
        MMI_horizontal_tab_bar.flags |= UI_TAB_BAR_DISABLE_PEN;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_restore_offset_timer_hdlr
 * DESCRIPTION
 *  Handle pen events in horizontal tab bar and
 * PARAMETERS
 *  void
 *  tb(?)
 *  pen_event(?)
 *  x(?)
 *  y(?)
 * RETURNS
 *  void
 *****************************************************************************/
/* Timer handler to restore the display range of tab bar */
static void wgui_horizontal_tab_bar_restore_offset_timer_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_horizontal_tab_bar_first_displayed_before_pen_down >= 0)
    {
        gui_set_horizontal_tab_bar_display_range(
            &MMI_horizontal_tab_bar,
            MMI_horizontal_tab_bar_first_displayed_before_pen_down);
        wgui_show_horizontal_tab_bar_tab_area();

        MMI_horizontal_tab_bar_first_displayed_before_pen_down = -1;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_abort_restore_offset_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void wgui_horizontal_tab_bar_abort_restore_offset_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_horizontal_tab_bar_first_displayed_before_pen_down = -1;
}

/* Start timer to restore the display range of tab bar */


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_start_restore_offset_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void wgui_horizontal_tab_bar_start_restore_offset_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_horizontal_tab_bar_first_displayed_before_pen_down >= 0)
    {
        gui_start_timer(WGUI_TAB_BAR_RESTORE_DISPLAY_TIME, wgui_horizontal_tab_bar_restore_offset_timer_hdlr);
        gui_add_cleanup_hook(wgui_horizontal_tab_bar_abort_restore_offset_timer);
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_horizontal_tab_bar_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pen_event       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 * RETURNS
 *  
 *****************************************************************************/
BOOL wgui_horizontal_tab_bar_translate_pen_event(mmi_pen_event_type_enum pen_event, S16 x, S16 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    gui_tab_bar_pen_enum tab_event;
    gui_pen_event_param_struct tab_param;
    horizontal_tab_bar *tb = &MMI_horizontal_tab_bar;
    S32 prev_fdisp; /* first displayed item before pen down */
    S32 prev_focus; /* focused item before pen down */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    prev_fdisp = tb->first_displayed_item;
    prev_focus = tb->focused_item;
    ret = gui_horizontal_tab_bar_translate_pen_event(tb, pen_event, x, y, &tab_event, &tab_param);

    if (ret)
    {
        if (pen_event == MMI_PEN_EVENT_DOWN)
        {
            gui_cancel_timer(wgui_horizontal_tab_bar_restore_offset_timer_hdlr);
            if (MMI_horizontal_tab_bar_first_displayed_before_pen_down < 0)
            {
                MMI_horizontal_tab_bar_first_displayed_before_pen_down = prev_fdisp;
            }
        }

        if (tab_event == GUI_TAB_BAR_PEN_NEED_REDRAW)
        {
            wgui_show_horizontal_tab_bar_tab_area();
        }
        else if (tab_event == GUI_TAB_BAR_PEN_JUMP_TO_I)
        {
            if (MMI_horizontal_tab_bar_select_callback)
            {
                MMI_horizontal_tab_bar_first_displayed_on_screen_switched = tb->first_displayed_item;
                MMI_horizontal_tab_bar_select_callback(tab_param._u.i);
                MMI_horizontal_tab_bar_first_displayed_on_screen_switched = -1;
            }
        }

        if ((pen_event == MMI_PEN_EVENT_ABORT || pen_event == MMI_PEN_EVENT_UP) &&
            MMI_horizontal_tab_bar.focused_item == prev_focus &&
            MMI_horizontal_tab_bar.first_displayed_item != MMI_horizontal_tab_bar_first_displayed_before_pen_down)
        {
            if (gui_horizontal_tab_bar_is_item_displayed(tb, tb->focused_item))
            {
                MMI_horizontal_tab_bar_first_displayed_before_pen_down = -1;
            }
            else
            {
                wgui_horizontal_tab_bar_start_restore_offset_timer();
            }
        }

        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}

#endif /* __MMI_TOUCH_SCREEN__ */ 
#endif //#ifdef __MMI_UI_TAB_PANE__

