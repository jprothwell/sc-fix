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
 *  gui_tab.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  UI component - Tab pane (__MMI_UI_TAB_PANE__)
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
#include "gui_config.h"

#ifdef __MMI_UI_TAB_PANE__
#include "gui_data_types.h"
#include "custthemesres.h"
#include "gui_tab.h"
#include "gui.h"
#include "gui_themes.h"
#include "wgui_categories.h"

/***************************************************************************** 
 * Define
 *****************************************************************************/
/* Defines for two icon tab pane */

#define     TWO_PANE_LEFT_ARROW_X         7
#define     TWO_PANE_LEFT_ARROW_Y         3

#define     TWO_PANE_RIGHT_ARROW_X        105
#define     TWO_PANE_RIGHT_ARROW_Y        3

#define     TWO_PANE_FIRST_PANE_X         21
#define     TWO_PANE_FIRST_PANE_Y         0

#define     TWO_PANE_SECOND_PANE_X        64
#define     TWO_PANE_SECOND_PANE_Y        0

#define     TWO_PANE_TAB_PANE_BOX_WIDTH   35
#define     TWO_PANE_TAB_PANE_BOX_HEIGHT  18

/***************************************************************************** 
 * Typedef 
 *****************************************************************************/

/***************************************************************************** 
 * Local Variable
 *****************************************************************************/

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
 *  gui_create_tab_pane
 * DESCRIPTION
 *  used to create the tab pabe
 * PARAMETERS
 *  tab_control     [IN/OUT]        tab control
 *  type            [IN]            type of tab pane
 *  x               [IN]            x position of tab pnae
 *  y               [IN]            y position of tab pnae
 *  width           [IN]            width of tab pane
 *  height          [IN]            heigth of tab pane
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_tab_pane(
        struct_tab_control *tab_control,
        enum_tab_control_type type,
        S32 x,
        S32 y,
        S32 width,
        S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->type = type;
    tab_control->x = x;
    tab_control->y = y;
    tab_control->width = width;
    tab_control->height = height;
    tab_control->n_items = 0;
    tab_control->set_callback = NULL;
    tab_control->category_exit_callback = NULL;
    tab_control->category_entry_callback = NULL;
    tab_control->highlighted_item = 0;
    tab_control->first_displayed_item = 0;
    tab_control->last_displayed_item = 0;
    tab_control->item_data = NULL;
    tab_control->type_data = NULL;
    tab_control->highlighted_pane = 0;
    tab_control->parent_id = 0;
    tab_control->flags = UI_TAB_CONTROL_NORMAL;
    memset(&tab_control->tab_list_history, 0, sizeof(struct_tab_list_history));
    switch (type)
    {
        case TAB_CONTROL_WITH_TWO_ICON:
            tab_control->draw_function = gui_show_icon_tab_pane;
            tab_control->goto_function = gui_goto_two_item_tab_pane;
            tab_control->next_function = gui_next_two_item_tab_pane;
            tab_control->prev_function = gui_prev_two_item_tab_pane;
            break;
        case TAB_CONTROL_WITH_TWO_TEXT:
            tab_control->draw_function = gui_show_text_tab_pane;
            tab_control->goto_function = gui_goto_two_item_tab_pane;
            tab_control->next_function = gui_next_two_item_tab_pane;
            tab_control->prev_function = gui_prev_two_item_tab_pane;
            break;

        case TAB_CONTROL_WITH_FOUR_ICON:
            tab_control->draw_function = gui_show_icon_tab_pane;
            tab_control->goto_function = gui_goto_four_item_tab_pane;
            tab_control->next_function = gui_next_four_item_tab_pane;
            tab_control->prev_function = gui_prev_four_item_tab_pane;
            break;
        case TAB_CONTROL_WITH_THREE_ICON:
            tab_control->draw_function = gui_show_icon_tab_pane;
            tab_control->goto_function = gui_goto_three_item_tab_pane;
            tab_control->next_function = gui_next_three_item_tab_pane;
            tab_control->prev_function = gui_prev_three_item_tab_pane;
            break;
        case TAB_CONTROL_WITH_ONE_ICON:
            tab_control->draw_function = gui_show_icon_tab_pane;
            tab_control->goto_function = gui_goto_one_item_tab_pane;
            tab_control->next_function = gui_next_one_item_tab_pane;
            tab_control->prev_function = gui_prev_one_item_tab_pane;
            break;
        default:
            tab_control->draw_function = gui_show_icon_tab_pane;
            tab_control->goto_function = gui_goto_two_item_tab_pane;
            tab_control->next_function = gui_next_two_item_tab_pane;
            tab_control->prev_function = gui_prev_two_item_tab_pane;
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_associate_tab_pane_type_data
 * DESCRIPTION
 *  Used to set data for tab control
 * PARAMETERS
 *  tab_control     [IN/OUT]        
 *  type_data       [IN]            pointer to data for tab pane. data varies from type to type
 * RETURNS
 *  void
 *****************************************************************************/
void gui_associate_tab_pane_type_data(struct_tab_control *tab_control, void *type_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->type_data = type_data;
}


/*****************************************************************************
 * FUNCTION
 *  gui_associate_tab_pane_item_data
 * DESCRIPTION
 *  Used to set item data for tab control
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  item_data       [IN]            pointer to item data for tab pane. this data depends on the items to be displayed.
 * RETURNS
 *  void
 *****************************************************************************/
void gui_associate_tab_pane_item_data(struct_tab_control *tab_control, struct_tab_items *item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->item_data = item_data;
}


/*****************************************************************************
 * FUNCTION
 *  gui_register_set_tab_pane_callback
 * DESCRIPTION
 *  Used to call backs for the tab pane
 * PARAMETERS
 *  tab_control             [IN/OUT]             
 *  callback                [IN]            call back for setting up tab pane
 *  set_data_callback       [IN]            call back for setting data for tab pane. specific to application
 *  reset_data_callback     [IN]            call back for resetting data for tab pane. specific to application
 * RETURNS
 *  void
 *****************************************************************************/
void gui_register_set_tab_pane_callback(
        struct_tab_control *tab_control,
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
    tab_control->set_callback = callback;
    tab_control->set_data_callback = set_data_callback;
    tab_control->reset_data_callback = reset_data_callback;
}


/*****************************************************************************
 * FUNCTION
 *  gui_add_item_tab_pane
 * DESCRIPTION
 *  Used to add items to tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  text            [IN]            text for tab pane
 *  icon            [IN]            icon for tab pane
 * RETURNS
 *  void
 *****************************************************************************/
void gui_add_item_tab_pane(struct_tab_control *tab_control, UI_string_type text, PU8 icon)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->item_data[tab_control->n_items].item_text = text;
    tab_control->item_data[tab_control->n_items].item_icon = icon;
    tab_control->n_items++;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_tab_pane
 * DESCRIPTION
 *  Used to show tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->draw_function)
    {
        tab_control->draw_function(tab_control);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_go_to_item
 * DESCRIPTION
 *  Used to go to some speciffic item in tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  index           [IN]            item on which to go
 * RETURNS
 *  void
 *****************************************************************************/
void gui_go_to_item(struct_tab_control *tab_control, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->goto_function)
    {
        tab_control->goto_function(tab_control, index);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_tab_pane_next_item
 * DESCRIPTION
 *  Used to go tonext item in tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_tab_pane_next_item(struct_tab_control *tab_control)
{
#ifdef MMI_TAB_PANE_LIST_HISTORY
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    list_menu_category_history list_history;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history(1, (U8*) & list_history);
    tab_control->tab_list_history.first_displayed_item[tab_control->highlighted_item]
        = list_history.first_displayed_item;
    tab_control->tab_list_history.highlighted_item[tab_control->highlighted_item] = list_history.highlighted_item;
    tab_control->tab_list_history.last_displayed_item[tab_control->highlighted_item] = list_history.last_displayed_item;
#endif /* MMI_TAB_PANE_LIST_HISTORY */ 
    if (tab_control->highlighted_item == tab_control->n_items - 1 && (tab_control->flags & UI_TAB_CONTROL_CYCLIC))
    {
        if (tab_control->goto_function)
        {
            tab_control->goto_function(tab_control, 0);
        }
        return;
    }
    if (tab_control->next_function)
    {
        tab_control->next_function(tab_control);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_tab_pane_prev_item
 * DESCRIPTION
 *  Used to go to previous item in tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_tab_pane_prev_item(struct_tab_control *tab_control)
{
#ifdef MMI_TAB_PANE_LIST_HISTORY
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    list_menu_category_history list_history;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history(1, (U8*) & list_history);
    tab_control->tab_list_history.first_displayed_item[tab_control->highlighted_item]
        = list_history.first_displayed_item;
    tab_control->tab_list_history.highlighted_item[tab_control->highlighted_item] = list_history.highlighted_item;
    tab_control->tab_list_history.last_displayed_item[tab_control->highlighted_item] = list_history.last_displayed_item;
#endif /* MMI_TAB_PANE_LIST_HISTORY */ 
    if (tab_control->highlighted_item == 0 && (tab_control->flags & UI_TAB_CONTROL_CYCLIC))
    {
        if (tab_control->goto_function)
        {
            tab_control->goto_function(tab_control, tab_control->n_items - 1);
        }
        return;
    }
    if (tab_control->prev_function)
    {
        tab_control->prev_function(tab_control);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_tab_pane_hightlight_fucntion
 * DESCRIPTION
 *  Used to set hightligth function for tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  f               [IN]            hightlight function
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_tab_pane_hightlight_fucntion(struct_tab_control *tab_control, void (*f) (S32 item_index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->highlight_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  gui_highlight_function
 * DESCRIPTION
 *  call the hightligth function for tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  index           [IN]            index of item to be highlighted
 * RETURNS
 *  void
 *****************************************************************************/
void gui_highlight_function(struct_tab_control *tab_control, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->highlight_function)
    {
        tab_control->highlight_function(index);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_tab_pane
 * DESCRIPTION
 *  call the move tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  x               [IN]            x position of tab pane
 *  y               [IN]            y position of tab pane
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_tab_pane(struct_tab_control *tab_control, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->x = x;
    tab_control->y = y;
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_tab_pane
 * DESCRIPTION
 *  call the resize tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  height          [IN]            height position of tab pane
 *  width           [IN]            width position of tab pane
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_tab_pane(struct_tab_control *tab_control, S32 height, S32 width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->height = height;
    tab_control->width = width;
}


/*****************************************************************************
 * FUNCTION
 *  gui_tab_pane_show_navigation_keys
 * DESCRIPTION
 *  Used to display navigation keys of the tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_tab_pane_show_navigation_keys(struct_tab_control *tab_control)
{
#if 0
#else /* 0 */ 
    S8 up, down, left, right;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    up = 1;
    down = 1;
    if (tab_control->flags & UI_TAB_CONTROL_CYCLIC)
    {
        left = right = 1;
    }
    else
    {
        if (tab_control->highlighted_item == 0)
        {
            left = 0;
        }
        else
        {
            left = 1;
        }
        if (tab_control->highlighted_item == tab_control->n_items - 1)
        {
            right = 0;
        }
        else
        {
            right = 1;
        }
    }
    draw_arrow_indication(left, right, up, down);
#endif /* 0 */ 

}

#if 0
#endif /* 0 */ 


/*****************************************************************************
 * FUNCTION
 *  gui_goto_two_item_tab_pane
 * DESCRIPTION
 *  go to function for two item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  index           [IN]            item to go to
 * RETURNS
 *  void
 *****************************************************************************/
void gui_goto_two_item_tab_pane(struct_tab_control *tab_control, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index < 0 || index > tab_control->n_items - 1)
    {
        return;
    }
    if (index == tab_control->n_items - 1)
    {
        tab_control->first_displayed_item = index - 1;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index;
        tab_control->highlighted_pane = 1;
    }
    else
    {
        tab_control->first_displayed_item = index;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index + 1;
        tab_control->highlighted_pane = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_next_two_item_tab_pane
 * DESCRIPTION
 *  next function function for two item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_next_two_item_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->highlighted_item == tab_control->n_items - 1)
    {
        return;
    }
    tab_control->highlighted_item++;
    if (tab_control->highlighted_pane == 0)
    {
        tab_control->highlighted_pane = 1;
    }
    else
    {
        tab_control->first_displayed_item = tab_control->last_displayed_item;
        tab_control->last_displayed_item = tab_control->highlighted_item;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_prev_two_item_tab_pane
 * DESCRIPTION
 *  previous function function for two item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_prev_two_item_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->highlighted_item == 0)
    {
        return;
    }
    tab_control->highlighted_item--;
    if (tab_control->highlighted_pane == 1)
    {
        tab_control->highlighted_pane = 0;
    }
    else
    {
        tab_control->last_displayed_item = tab_control->first_displayed_item;
        tab_control->first_displayed_item = tab_control->highlighted_item;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_save_parent_id
 * DESCRIPTION
 *  used to save parent id for tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  parent_id       [IN]            parent id of tab pane
 * RETURNS
 *  void
 *****************************************************************************/
void gui_save_parent_id(struct_tab_control *tab_control, S32 parent_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->parent_id = parent_id;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_text_tab_pane
 * DESCRIPTION
 *  used to show text tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_text_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    struct_tab_text_data *tab_pane_data = (struct_tab_text_data*) tab_control->type_data;
    S32 i;
    PU8 back_image;
    S32 x1, x2, y1, y2;
    S32 ix, iy;
    S32 swidth, sheight;
    S32 width, height;
    color_t text_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = tab_control->x;
    x2 = tab_control->x + tab_control->width - 1;
    y1 = tab_control->y;
    y2 = tab_control->y + tab_control->height - 1;
    gui_push_clip();
    gui_push_text_clip();
    gui_set_clip(x1, y1, x2, y2);
    gui_fill_rectangle(x1, y1, x2, y2, UI_COLOR_WHITE);
    for (i = tab_control->first_displayed_item; i <= tab_control->last_displayed_item; i++)
    {

        if (i == tab_control->highlighted_item)
        {
            back_image = tab_pane_data->highlight_image;
            text_color = tab_pane_data->highlight_text_color;
        }
        else
        {
            back_image = tab_pane_data->un_highlight_image;
            text_color = tab_pane_data->un_highlight_text_color;
        }
        gui_measure_image(back_image, &width, &height);
        y1 = y1 + tab_control->height - height;
        gui_set_clip(x1, y1, x1 + width, y1 + height);
        gui_show_image(x1, y1, back_image);
        gui_set_font(tab_pane_data->font);
        gui_set_text_color(text_color);
        gui_measure_string(tab_control->item_data[i].item_text, &swidth, &sheight);
        if (width > swidth)
        {
            ix = x1 + ((width - swidth) >> 1);
        }
        else
        {
            ix = x1;
        }
        if (height > sheight)
        {
            iy = y1 + ((height - sheight) >> 1);
        }
        else
        {
            iy = y1;
        }
        gui_set_text_clip(ix, iy, x1 + width, y1 + height);
        gui_move_text_cursor(ix, iy);
        gui_print_text(tab_control->item_data[i].item_text);
        x1 += width;
        y1 = tab_control->y;
    }
    gui_tab_pane_show_navigation_keys(tab_control);
    gui_pop_clip();
    gui_pop_text_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_goto_three_item_tab_pane
 * DESCRIPTION
 *  go to function for three item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  index           [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void gui_goto_three_item_tab_pane(struct_tab_control *tab_control, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index < 0 || index > tab_control->n_items - 1)
    {
        return;
    }
    if (index == tab_control->n_items - 1)
    {
        tab_control->first_displayed_item = index - 2;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index;
        tab_control->highlighted_pane = 2;
    }
    else if (index == tab_control->n_items - 2)
    {
        tab_control->first_displayed_item = index - 1;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index + 1;
        tab_control->highlighted_pane = 1;
    }
    else
    {
        tab_control->first_displayed_item = index;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index + 2;
        tab_control->highlighted_pane = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_next_three_item_tab_pane
 * DESCRIPTION
 *  next function for three item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_next_three_item_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->highlighted_item == tab_control->n_items - 1)
    {
        return;
    }
    tab_control->highlighted_item++;
    switch (tab_control->highlighted_pane)
    {
        case 0:
            tab_control->highlighted_pane = 1;
            break;
        case 1:
            tab_control->highlighted_pane = 2;
            break;
        default:
            tab_control->first_displayed_item += 1;
            tab_control->last_displayed_item = tab_control->highlighted_item;
            break;
    }

}


/*****************************************************************************
 * FUNCTION
 *  gui_prev_three_item_tab_pane
 * DESCRIPTION
 *  previous function for three item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_prev_three_item_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->highlighted_item == 0)
    {
        return;
    }
    tab_control->highlighted_item--;
    switch (tab_control->highlighted_pane)
    {
        case 2:
            tab_control->highlighted_pane = 1;
            break;
        case 1:
            tab_control->highlighted_pane = 0;
            break;
        default:
            tab_control->last_displayed_item -= 1;
            tab_control->first_displayed_item = tab_control->highlighted_item;
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_icon_tab_pane
 * DESCRIPTION
 *  show function for icon item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_icon_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    struct_tab_icon_data *tab_pane_data = (struct_tab_icon_data*) tab_control->type_data;
    S32 i;
    PU8 back_image;
    S32 x1, x2, y1, y2;
    S32 ix, iy;
    S32 iwidth, iheight;
    S32 width, height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!tab_control->width || !tab_control->height)
    {
        gui_tab_pane_show_navigation_keys(tab_control);
        return;
    }
    x1 = tab_control->x;
    x2 = tab_control->x + tab_control->width - 1;
    y1 = tab_control->y;
    y2 = tab_control->y + tab_control->height - 1;
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
    gui_fill_rectangle(x1, y1, x2, y2, UI_COLOR_WHITE);
    for (i = tab_control->first_displayed_item; i <= tab_control->last_displayed_item; i++)
    {

        if (i == tab_control->highlighted_item)
        {
            back_image = tab_pane_data->highlight_image;
        }
        else
        {
            back_image = tab_pane_data->un_highlight_image;
        }
        gui_measure_image(back_image, &width, &height);
        y1 = y1 + tab_control->height - height;
        gui_set_clip(x1, y1, x1 + width, y1 + height);
        gui_show_image(x1, y1, back_image);
        gui_measure_image(tab_control->item_data[i].item_icon, &iwidth, &iheight);
        if (width > iwidth)
        {
            ix = x1 + ((width - iwidth) >> 1);
        }
        else
        {
            ix = x1;
        }
        if (height > iheight)
        {
            iy = y1 + ((height - iheight) >> 1);
        }
        else
        {
            iy = y1;
        }
        gui_set_clip(ix, iy, x1 + width, y1 + height);
        gui_show_transparent_image(ix, iy, tab_control->item_data[i].item_icon, 0);
        x1 += width;
        y1 = tab_control->y;
    }
    gui_tab_pane_show_navigation_keys(tab_control);
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_goto_four_item_tab_pane
 * DESCRIPTION
 *  go to function for four item item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  index           [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void gui_goto_four_item_tab_pane(struct_tab_control *tab_control, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index < 0 || index > tab_control->n_items - 1)
    {
        return;
    }
    if (index == tab_control->n_items - 1)
    {
        tab_control->first_displayed_item = index - 3;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index;
        tab_control->highlighted_pane = 3;
    }
    else if (index == tab_control->n_items - 2)
    {
        tab_control->first_displayed_item = index - 2;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index + 1;
        tab_control->highlighted_pane = 2;
    }
    else if (index == tab_control->n_items - 3)
    {
        tab_control->first_displayed_item = index - 1;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index + 2;
        tab_control->highlighted_pane = 1;
    }
    else
    {
        tab_control->first_displayed_item = index;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index + 3;
        tab_control->highlighted_pane = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_next_four_item_tab_pane
 * DESCRIPTION
 *  next function for four item item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_next_four_item_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->highlighted_item == tab_control->n_items - 1)
    {
        return;
    }
    tab_control->highlighted_item++;
    switch (tab_control->highlighted_pane)
    {
        case 0:
            tab_control->highlighted_pane = 1;
            break;
        case 1:
            tab_control->highlighted_pane = 2;
            break;
        case 2:
            tab_control->highlighted_pane = 3;
            break;
        default:
            tab_control->first_displayed_item += 1;
            tab_control->last_displayed_item = tab_control->highlighted_item;
            break;
    }

}


/*****************************************************************************
 * FUNCTION
 *  gui_prev_four_item_tab_pane
 * DESCRIPTION
 *  previous function for four item item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_prev_four_item_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->highlighted_item == 0)
    {
        return;
    }
    tab_control->highlighted_item--;
    switch (tab_control->highlighted_pane)
    {
        case 3:
            tab_control->highlighted_pane = 2;
            break;
        case 2:
            tab_control->highlighted_pane = 1;
            break;
        case 1:
            tab_control->highlighted_pane = 0;
            break;
        default:
            tab_control->last_displayed_item -= 1;
            tab_control->first_displayed_item = tab_control->highlighted_item;
            break;
    }
}

#if 0
#endif /* 0 */ 
/*
 * void gui_show_four_icontext_tab_pane(struct_tab_control *tab_control)
 * {
 * struct_tab_icontext_data *tab_pane_data=(struct_tab_icontext_data *)tab_control->type_data;
 * S32 i,start_x;
 * color c={0,0,0,100};
 * gui_push_clip();
 * gui_push_text_clip();
 * gui_set_clip(tab_control->x,tab_control->y,
 * tab_control->x+tab_control->width-1,tab_control->y+tab_control->height-1);
 * 
 * gui_draw_filled_area(tab_control->x,tab_control->y,
 * tab_control->x+tab_control->width-1, 
 * tab_control->y+MMI_title_height-1,tab_pane_data->title_filler);
 * gui_set_text_clip(tab_control->x,tab_control->y,
 * tab_control->x+tab_control->width-1, 
 * tab_control->y+MMI_title_height-1);
 * gui_set_font((UI_font_type)tab_pane_data->text_font);
 * gui_set_text_color(*tab_pane_data->text_color);
 * gui_move_text_cursor(tab_control->x+5,tab_control->y+1);
 * gui_set_text_border_color(*tab_pane_data->text_border_color);
 * gui_print_truncated_borderd_text(tab_control->x+5,tab_control->y+3,tab_control->width-10,
 * tab_control->item_data[tab_control->highlighted_item].item_text);
 * 
 * gui_draw_filled_area(tab_control->x,tab_control->y+MMI_title_height,
 * tab_control->x+tab_control->width, 
 * tab_control->y+MMI_title_height+27,tab_pane_data->back_filler);
 * if(tab_control->highlighted_item!=0)
 * gui_show_transparent_image(tab_control->x,tab_control->y+MMI_title_height+7,tab_pane_data->left_indicator,0);
 * if(tab_control->highlighted_item!=tab_control->n_items-1)
 * gui_show_transparent_image(tab_control->x+116,tab_control->y+MMI_title_height+7,tab_pane_data->right_indicator,0);
 * 
 * for(i=0;i<tab_control->n_items;i++)
 * {
 * start_x=tab_control->x+13+(27*i);
 * if( start_x+23 > tab_control->x + tab_control->width) break;
 * gui_set_clip(start_x,tab_control->y+MMI_title_height+2,
 * start_x+23,tab_control->y+MMI_title_height+25);
 * if(tab_control->first_displayed_item+i==tab_control->highlighted_item)
 * gui_draw_filled_area(start_x,tab_control->y+MMI_title_height+2,
 * start_x+23,tab_control->y+MMI_title_height+23,
 * tab_pane_data->high_filler);
 * gui_show_transparent_image(start_x,tab_control->y+MMI_title_height+2,
 * tab_control->item_data[tab_control->first_displayed_item+i].item_icon,0);
 * }
 * gui_set_clip(tab_control->x,tab_control->y,
 * tab_control->x+tab_control->width-1,tab_control->y+tab_control->height-1);
 * gui_draw_rectangle(tab_control->x,tab_control->y,
 * tab_control->x+tab_control->width-1,tab_control->y+tab_control->height-1,
 * c);
 * gui_draw_rectangle(tab_control->x+1,tab_control->y+1,
 * tab_control->x+tab_control->width-2,tab_control->y+tab_control->height-2,
 * c);
 * gui_tab_pane_show_navigation_keys(tab_control);
 * gui_pop_clip();
 * gui_pop_text_clip();
 * }
 */


/*****************************************************************************
 * FUNCTION
 *  gui_goto_one_item_tab_pane
 * DESCRIPTION
 *  go to function for one item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  index           [IN]            item to go to
 * RETURNS
 *  void
 *****************************************************************************/
void gui_goto_one_item_tab_pane(struct_tab_control *tab_control, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index < 0 || index > tab_control->n_items - 1)
    {
        return;
    }
    if (index == tab_control->n_items - 1)
    {
        tab_control->first_displayed_item = index;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index;
        tab_control->highlighted_pane = 0;
    }
    else
    {
        tab_control->first_displayed_item = index;
        tab_control->highlighted_item = index;
        tab_control->last_displayed_item = index;
        tab_control->highlighted_pane = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_next_one_item_tab_pane
 * DESCRIPTION
 *  next function function for one item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_next_one_item_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->highlighted_item == tab_control->n_items - 1)
    {
        return;
    }
    tab_control->highlighted_item++;
    tab_control->first_displayed_item = tab_control->highlighted_item;
    tab_control->last_displayed_item = tab_control->highlighted_item;
}


/*****************************************************************************
 * FUNCTION
 *  gui_prev_one_item_tab_pane
 * DESCRIPTION
 *  previous function function for one item tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 * RETURNS
 *  void
 *****************************************************************************/
void gui_prev_one_item_tab_pane(struct_tab_control *tab_control)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tab_control->highlighted_item == 0)
    {
        return;
    }
    tab_control->highlighted_item--;
    tab_control->last_displayed_item = tab_control->highlighted_item;
    tab_control->first_displayed_item = tab_control->highlighted_item;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_tab_pane_category_exit_function
 * DESCRIPTION
 *  Used to set exit category function for tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  f               [IN]            category exit function
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_tab_pane_category_exit_function(struct_tab_control *tab_control, void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->category_exit_callback = f;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_tab_pane_category_entry_function
 * DESCRIPTION
 *  Used to set entry category function for tab pane
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  f               [IN]            category entry function
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_tab_pane_category_entry_function(struct_tab_control *tab_control, void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->category_entry_callback = f;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_tab_pane_flags
 * DESCRIPTION
 *  this function sets tab pane flags
 * PARAMETERS
 *  tab_control     [IN/OUT]             
 *  flags           [IN]                     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_tab_pane_flags(struct_tab_control *tab_control, S32 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tab_control->flags = flags;
}

#else /* __MMI_UI_TAB_PANE__ */ 

int g_gui_tab_dummy;    /* Get rid of compile warning */

#endif /* __MMI_UI_TAB_PANE__ */ 

