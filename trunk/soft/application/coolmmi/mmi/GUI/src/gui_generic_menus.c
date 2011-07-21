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
 *   gui_generic_menus.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Generic menu - UI component
 *   (deprecated & unused)
 * Author:
 * -------
 * -------
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
   Filename:      gui_generic_menus.c
   Date Created:  July-22-2002
   Contains:      UI routines for Generic Menus
**********************************************************************************/

#include "gui_generic_menus.h"

 
extern BOOL r2lMMIFlag;

/* CSD end */

/* Dummy functions required by the generic menu items: Do not remove any of these   */


/*****************************************************************************
 * FUNCTION
 *  UI_generic_menuitem_dummy_display_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  x           [IN]        
 *  y           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_generic_menuitem_dummy_display_function(void *item, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
}


/*****************************************************************************
 * FUNCTION
 *  UI_generic_menuitem_dummy_measure_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 *  x           [?]     
 *  y           [?]     
 *  width       [?]     
 *  height      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void UI_generic_menuitem_dummy_measure_function(void *item, S32 *x, S32 *y, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
    UI_UNUSED_PARAMETER(width);
    UI_UNUSED_PARAMETER(height);
}


/*****************************************************************************
 * FUNCTION
 *  UI_generic_menuitem_dummy_highlight_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void UI_generic_menuitem_dummy_highlight_function(void *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
}


/*****************************************************************************
 * FUNCTION
 *  UI_generic_menuitem_dummy_remove_highlight_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void UI_generic_menuitem_dummy_remove_highlight_function(void *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_generic_menuitem
 * DESCRIPTION
 *  Creates a generic menuitem
 * PARAMETERS
 *  m           [IN]        Is the generic menuitem (pre-allocated)
 *  item        [IN]        Is the specific menuitem (pre-allocated)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_generic_menuitem(UI_generic_menuitem *m, void *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->item = item;
    m->display_function = UI_generic_menuitem_dummy_display_function;
    m->measure_function = UI_generic_menuitem_dummy_measure_function;
    m->highlight_function = UI_generic_menuitem_dummy_highlight_function;
    m->remove_highlight_function = UI_generic_menuitem_dummy_remove_highlight_function;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_generic_menuitem_display_function
 * DESCRIPTION
 *  Sets the function to use to display a generic menuitem
 * PARAMETERS
 *  m                       [IN]        Is the generic menuitem
 *  display_function        [IN]        Is the function used to display the item
 *  item(?)                 [IN]        Is the specific menuitem
 *  x(?)                    [IN]        Is the position at which the menuitem is to be displayed
 *  This(?)                 [IN]        Has the following parameters:
 *  y(?)                    [IN]        Is the position at which the menuitem is to be displayed
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_generic_menuitem_display_function(
        UI_generic_menuitem *m,
        void (*display_function) (void *item, S32 x, S32 y))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->display_function = display_function;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_generic_menuitem_measure_function
 * DESCRIPTION
 *  Sets the function to use to measure a generic menuitem
 * PARAMETERS
 *  m                       [IN]        Is the generic menuitem
 *  measure_function        [IN]        Is the function used to measure the item
 *  width(?)                [OUT]       Height   are the dimensions of the specific menuitem
 *  item(?)                 [IN]        Is the specific menuitem
 *  x(?)                    [OUT]       Is the position at which the specific menuitem is displayed
 *  This(?)                 [IN]        Has the following parameters:
 *  y(?)                    [OUT]       Is the position at which the specific menuitem is displayed
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_generic_menuitem_measure_function(
        UI_generic_menuitem *m,
        void (*measure_function) (void *item, S32 *x, S32 *y, S32 *width, S32 *height))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->measure_function = measure_function;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_generic_menuitem_highlight_function
 * DESCRIPTION
 *  Sets the function to use to highlight a generic menuitem
 * PARAMETERS
 *  m                       [IN]        Is the generic menuitem
 *  highlight_function      [IN]        Is the function used to highlight the item
 *  item(?)                 [IN]        Is the specific menuitem
 *  This(?)                 [IN]        Has the following parameters:
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_generic_menuitem_highlight_function(UI_generic_menuitem *m, void (*highlight_function) (void *item))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->highlight_function = highlight_function;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_generic_menuitem_remove_highlight_function
 * DESCRIPTION
 *  Sets the function to use to remove highlight of a generic menuitem
 * PARAMETERS
 *  m                               [IN]        Is the generic menuitem
 *  remove_highlight_function       [IN]        Is the function used to remove highlight of the item
 *  item(?)                         [IN]        Is the specific menuitem
 *  This(?)                         [IN]        Has the following parameters:
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_generic_menuitem_remove_highlight_function(
        UI_generic_menuitem *m,
        void (*remove_highlight_function) (void *item))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->remove_highlight_function = remove_highlight_function;
}

/* GUI: list menu functions               */

UI_list_menu_theme *current_list_menu_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_list_menu_current_theme
 * DESCRIPTION
 *  Applies the current theme to a list menu
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_list_menu_current_theme(list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags |= current_list_menu_theme->flags;
    m->focussed_filler = current_list_menu_theme->focussed_filler;
    m->normal_filler = current_list_menu_theme->normal_filler;
    gui_resize_vertical_scrollbar(&m->vbar, current_list_menu_theme->vbar_width, m->vbar.height);
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_list_menu_theme->vbar_width - 1, m->y + 2);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_list_menu_theme
 * DESCRIPTION
 *  Applies the given theme to a list menu
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 *  t       [IN]        Is the theme
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_list_menu_theme(list_menu *m, UI_list_menu_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags |= t->flags;
    m->focussed_filler = t->focussed_filler;
    m->normal_filler = t->normal_filler;
    gui_resize_vertical_scrollbar(&m->vbar, t->vbar_width, m->vbar.height);
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_list_menu_theme->vbar_width - 1, m->y + 2);
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_list_menu
 * DESCRIPTION
 *  Creates a list menu object
 * PARAMETERS
 *  m           [IN]        Is the list menu object (pre-allocated)
 *  x           [IN]        Is the left-top corner position
 *  y           [IN]        Is the left-top corner position
 *  width       [IN]        Are the dimensions
 *  height      [IN]        Are the dimensions
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_list_menu(list_menu *m, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags = 0;
    m->x = x;
    m->y = y;
    m->width = width;
    m->height = height;
    m->n_items = 0;
    m->highlighted_item = 0;
    m->first_displayed_item = 0;
    m->last_displayed_item = 0;
    m->displayed_items = 0;
    gui_set_list_menu_current_theme(m);
    gui_create_vertical_scrollbar(
        &m->vbar,
        m->x + m->width - current_list_menu_theme->vbar_width - 1,
        m->y + 2,
        current_list_menu_theme->vbar_width,
        m->height - 4);
    m->item_highlighted = UI_dummy_function_S32 ;
    m->item_selected = UI_dummy_function_S32 ;
    m->item_unhighlighted = UI_dummy_function_S32 ;
    m->item_unselected = UI_dummy_function_S32 ;
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_list_menu
 * DESCRIPTION
 *  Changes the size of a list menu
 * PARAMETERS
 *  m           [IN]        Is the list menu object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_list_menu(list_menu *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_list_menu_theme->vbar_width - 1, m->y + 2);
    gui_resize_vertical_scrollbar(&m->vbar, m->vbar.width, m->height - 4);
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_list_menu
 * DESCRIPTION
 *  Moves the list menu to a different co-ordinate
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 *  x       [IN]        Is the new left-top corner position
 *  y       [IN]        Is the new left-top corner position
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_list_menu(list_menu *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->x = x;
    m->y = y;
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_list_menu_theme->vbar_width - 1, m->y + 2);
}


/*****************************************************************************
 * FUNCTION
 *  gui_list_menu_locate_highlighted_item
 * DESCRIPTION
 *  Used to locate the highlighted item in the list menu
 *  
 *  This is an internal function
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_list_menu_locate_highlighted_item(list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 ix, iy, iwidth, iheight;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_item < m->first_displayed_item)
    {
        m->first_displayed_item = m->highlighted_item;
    }
    else if (m->highlighted_item > m->last_displayed_item)
    {
        U8 done = 0;
        S32 total_height = 0, i;
        S32 list_height = m->height - 8;

        m->last_displayed_item = m->highlighted_item;
        for (i = m->last_displayed_item; i >= 0 && (!done); i--)
        {
            m->items[i].measure_function((void*)m->items[i].item, &ix, &iy, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > list_height)
            {
                done = 1;
                m->first_displayed_item = i + 1;
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_list_menu_goto_item
 * DESCRIPTION
 *  Highlights a particular item
 *  
 *  The list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 *  i       [IN]        Is the index of the item to be highlighted (zero based)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_list_menu_goto_item(list_menu *m, S32 i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((i < 0) || (i > m->n_items - 1))
    {
        return;
    }
    if (i == m->highlighted_item)
    {
        return;
    }
    last_highlighted_item = m->highlighted_item;
    m->highlighted_item = i;
    gui_list_menu_locate_highlighted_item(m);
    m->item_unhighlighted(last_highlighted_item);
    m->item_highlighted(m->highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_list_menu_goto_next_item
 * DESCRIPTION
 *  Highlights the next item in the list
 *  
 *  The list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_list_menu_goto_next_item(list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;
    U8 loop_flag = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_item >= (m->n_items - 1))
    {
        if (m->flags & UI_LIST_MENU_LOOP)
        {
            loop_flag = 1;
        }
        else
        {
            return;
        }
    }
    last_highlighted_item = m->highlighted_item;
    if (loop_flag)
    {
        m->highlighted_item = 0;
    }
    else
    {
        m->highlighted_item++;
    }
    gui_list_menu_locate_highlighted_item(m);
    m->item_unhighlighted(last_highlighted_item);
    m->item_highlighted(m->highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_list_menu_goto_previous_item
 * DESCRIPTION
 *  Highlights the previous item in the list
 *  
 *  The list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_list_menu_goto_previous_item(list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;
    U8 loop_flag = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_item <= 0)
    {
        if (m->flags & UI_LIST_MENU_LOOP)
        {
            loop_flag = 1;
        }
        else
        {
            return;
        }
    }
    last_highlighted_item = m->highlighted_item;
    if (loop_flag)
    {
        m->highlighted_item = m->n_items - 1;
    }
    else
    {
        m->highlighted_item--;
    }
    gui_list_menu_locate_highlighted_item(m);
    m->item_unhighlighted(last_highlighted_item);
    m->item_highlighted(m->highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_list_menu_goto_next_page
 * DESCRIPTION
 *  Highlights an item in the next page
 *  
 *  The list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_list_menu_goto_next_page(list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;
    S32 ix, iy, iwidth, iheight;
    U8 done = 0;
    S32 total_height = 0, i;
    S32 list_height = m->height - 8;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    last_highlighted_item = m->highlighted_item;
    m->first_displayed_item += m->displayed_items;
    if (m->first_displayed_item > (m->n_items - 1))
    {
        m->first_displayed_item = (m->n_items - 1);
        m->last_displayed_item = m->first_displayed_item;
    }
    else
    {
        for (i = m->first_displayed_item; (i < m->n_items) && (!done); i++)
        {
            m->items[i].measure_function((void*)m->items[i].item, &ix, &iy, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > list_height)
            {
                done = 1;
                m->last_displayed_item = i - 1;
            }
        }
    }
    for (i = m->last_displayed_item; i >= 0 && (!done); i--)
    {
        m->items[i].measure_function((void*)m->items[i].item, &ix, &iy, &iwidth, &iheight);
        total_height += iheight;
        if (total_height > list_height)
        {
            done = 1;
            m->first_displayed_item = i + 1;
        }
    }
    m->highlighted_item = m->first_displayed_item;
    m->item_unhighlighted(last_highlighted_item);
    m->item_highlighted(m->highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_list_menu_goto_previous_page
 * DESCRIPTION
 *  Highlights an item in the previous page
 *  
 *  The list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_list_menu_goto_previous_page(list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    last_highlighted_item = m->highlighted_item;
    m->first_displayed_item -= m->displayed_items;
    if (m->first_displayed_item < 0)
    {
        m->first_displayed_item = 0;
    }
    m->highlighted_item = m->first_displayed_item;
    if (last_highlighted_item == m->highlighted_item)
    {
        return;
    }
    gui_list_menu_locate_highlighted_item(m);
    m->item_unhighlighted(last_highlighted_item);
    m->item_highlighted(m->highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_list_menu_goto_first_item
 * DESCRIPTION
 *  Highlights the first item in the list
 *  
 *  The list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_list_menu_goto_first_item(list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_item == 0)
    {
        return;
    }
    last_highlighted_item = m->highlighted_item;
    m->highlighted_item = 0;
    gui_list_menu_locate_highlighted_item(m);
    m->item_unhighlighted(last_highlighted_item);
    m->item_highlighted(m->highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_list_menu_goto_last_item
 * DESCRIPTION
 *  Highlights the last item in the list
 *  
 *  The list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_list_menu_goto_last_item(list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_item == (m->n_items - 1))
    {
        return;
    }
    last_highlighted_item = m->highlighted_item;
    m->highlighted_item = (m->n_items - 1);
    gui_list_menu_locate_highlighted_item(m);
    m->item_unhighlighted(last_highlighted_item);
    m->item_highlighted(m->highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_list_menu
 * DESCRIPTION
 *  Displays the list menu
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_list_menu(list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, y_offset;
    UI_filled_area *f;
    S32 i;
    S32 cx1, cy1, cx2, cy2;
    S32 tx1, ty1, tx2, ty2;
    S32 ix, iy, iwidth, iheight;
    U8 done = 0;
    S32 total_height, counter, list_height;
    S32 vbar_x = 0, vbar_button_x = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_get_clip(&cx1, &cy1, &cx2, &cy2);
    gui_get_text_clip(&tx1, &ty1, &tx2, &ty2);
    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

	
    if (m->flags & UI_LIST_MENU_STATE_FOCUSSED)
    {
        f = m->focussed_filler;
    }
    else
    {
        f = m->normal_filler;
    }
    gui_set_clip(x1, y1, x2 + 2, y2 + 2);
    gui_draw_filled_area(x1, y1, x2, y2, f);
    x1 += 4;
    x2 -= (4 - m->vbar.width);
    y1 += 4;
    y2 -= 4;
    list_height = m->height - 8;
    gui_set_text_clip(x1, y1, x2, y2);
    gui_set_clip(x1, y1, x2, y2);
    total_height = 0;
    counter = 0;
    for (i = m->first_displayed_item; (i < m->n_items && !done); i++)
    {
        if (i == m->highlighted_item)
        {
            m->items[i].highlight_function((void*)m->items[i].item);
        }
        else
        {
            m->items[i].remove_highlight_function((void*)m->items[i].item);
        }
        m->items[i].measure_function((void*)m->items[i].item, &ix, &iy, &iwidth, &iheight);
        y_offset = total_height;
        total_height += iheight;
        if (total_height > list_height)
        {
            done = 1;
            if (counter == 0)
            {
                m->items[i].display_function((void*)m->items[i].item, x1, y_offset + y1);
            }
        }
        else
        {
            m->items[i].display_function((void*)m->items[i].item, x1, y_offset + y1);
            counter++;
        }
    }
    if (counter == 0)
    {
        m->last_displayed_item = m->first_displayed_item;
    }
    else
    {
        m->last_displayed_item = m->first_displayed_item + counter - 1;
    }
    m->displayed_items = counter;
    gui_set_clip(cx1, cy1, cx2, cy2);
    gui_set_text_clip(tx1, ty1, tx2, ty2);
    gui_set_vertical_scrollbar_range(&m->vbar, m->n_items);
    gui_set_vertical_scrollbar_scale(&m->vbar, m->displayed_items);
    gui_set_vertical_scrollbar_value(&m->vbar, m->first_displayed_item);
     
    if (r2lMMIFlag)
    {
        vbar_x = m->vbar.x;
        vbar_button_x = m->vbar.scroll_button.x;
        m->vbar.x = m->vbar.x + m->vbar.width - m->width;
        m->vbar.scroll_button.x = m->vbar.scroll_button.x + m->vbar.scroll_button.width - m->width;
        gui_show_vertical_scrollbar(&m->vbar);
    }
    else
    {
        gui_show_vertical_scrollbar(&m->vbar);
    }
    /* CSD end */
    if (r2lMMIFlag)
    {
        m->vbar.x = vbar_x;
        m->vbar.scroll_button.x = vbar_button_x;
    }

}

/* GUI: matrix menu functions             */

UI_matrix_menu_theme *current_matrix_menu_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_matrix_menu_current_theme
 * DESCRIPTION
 *  Applies the current theme to a matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_matrix_menu_current_theme(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags |= current_matrix_menu_theme->flags;
    m->focussed_filler = current_matrix_menu_theme->focussed_filler;
    m->normal_filler = current_matrix_menu_theme->normal_filler;
    gui_resize_vertical_scrollbar(
        &m->vbar,
        current_matrix_menu_theme->vbar_width,
        m->height - current_matrix_menu_theme->hbar_height - 3);
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_matrix_menu_theme->vbar_width - 1, m->y + 2);
    gui_resize_horizontal_scrollbar(
        &m->hbar,
        m->width - current_matrix_menu_theme->vbar_width - 3,
        current_matrix_menu_theme->hbar_height);
    gui_move_horizontal_scrollbar(&m->hbar, m->x + 2, m->y + m->height - current_matrix_menu_theme->hbar_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_matrix_menu_theme
 * DESCRIPTION
 *  Applies the given theme to a matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 *  t       [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_matrix_menu_theme(matrix_menu *m, UI_matrix_menu_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags |= t->flags;
    m->focussed_filler = t->focussed_filler;
    m->normal_filler = t->normal_filler;
    gui_resize_vertical_scrollbar(
        &m->vbar,
        current_matrix_menu_theme->vbar_width,
        m->height - current_matrix_menu_theme->hbar_height - 3);
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_matrix_menu_theme->vbar_width - 1, m->y + 2);
    gui_resize_horizontal_scrollbar(
        &m->hbar,
        m->width - current_matrix_menu_theme->vbar_width - 3,
        current_matrix_menu_theme->hbar_height);
    gui_move_horizontal_scrollbar(&m->hbar, m->x + 2, m->y + m->height - current_matrix_menu_theme->hbar_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_matrix_menu
 * DESCRIPTION
 *  Creates a matrix menu
 * PARAMETERS
 *  m           [IN]        Is the matrix menu object  (pre-allocated)
 *  x           [IN]        Is the left-top corner position
 *  y           [IN]        Is the left-top corner position
 *  width       [IN]        Are the dimensions
 *  height      [IN]        Are the dimensions
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_matrix_menu(matrix_menu *m, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags = 0;
    m->x = x;
    m->y = y;
    m->width = width;
    m->height = height;
    m->n_items = 0;
    m->n_columns = 0;
    m->n_rows = 0;
    m->highlighted_item = 0;
    m->first_displayed_row = 0;
    m->first_displayed_column = 0;
    m->displayed_columns = 0;
    m->displayed_rows = 0;
    m->highlighted_column = 0;
    m->highlighted_row = 0;
    m->item_height = 0;
    m->item_width = 0;
    gui_set_matrix_menu_current_theme(m);
    gui_create_vertical_scrollbar(
        &m->vbar,
        m->x + m->width - current_matrix_menu_theme->vbar_width - 1,
        m->y + 2,
        current_matrix_menu_theme->vbar_width,
        m->height - current_matrix_menu_theme->hbar_height - 3);
    gui_create_horizontal_scrollbar(
        &m->hbar,
        m->x + 2,
        m->y + m->height - current_matrix_menu_theme->hbar_height - 1,
        m->width - current_matrix_menu_theme->vbar_width - 3,
        current_matrix_menu_theme->hbar_height);
    m->item_highlighted = UI_dummy_function_S32 ;
    m->item_selected = UI_dummy_function_S32 ;
    m->item_unhighlighted = UI_dummy_function_S32 ;
    m->item_unselected = UI_dummy_function_S32 ;
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_auto_configure
 * DESCRIPTION
 *  Automatically configures a matrix menu
 *  (Decides the number of columns / rows to be displayed
 *  and the number of items per row or column)
 *  
 *  Internal function
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_auto_configure(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 menu_width, menu_height;
    U8 show_vbar = 0, show_hbar = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((m->n_columns <= 0) && (m->item_width <= 0))
    {
        return;
    }
    if ((m->n_rows <= 0) && (m->item_height <= 0))
    {
        return;
    }

    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

    if (m->n_items == 0)
    {
        return;
    }

    x1 += 4;
    x2 -= 4;
    y1 += 4;
    y2 -= 4;
    menu_height = y2 - y1 + 1;
    menu_width = x2 - x1 + 1;

    /* Autocalculate unspecified parameters               */
    if (m->item_height <= 0)
    {
        m->item_height = menu_height / m->n_rows;
    }
    if (m->n_rows <= 0)
    {
        m->n_rows = menu_height / m->item_height;
    }
    if (m->item_width <= 0)
    {
        m->item_width = menu_width / m->n_columns;
    }
    if (m->n_columns <= 0)
    {
        m->n_columns = menu_width / m->item_width;
    }

    /* Fix rows and columns to match the number of items     */
    if (m->n_items < m->n_columns)
    {
        m->n_rows = 1;
        m->n_columns = m->n_items;
    }
    else
    {
        if (m->n_rows > coolsand_highdivide(m->n_items, m->n_columns))
        {
            m->n_rows = coolsand_highdivide(m->n_items, m->n_columns);
        }
        if ((m->n_columns * m->n_rows) < m->n_items)
        {
            m->n_rows = coolsand_highdivide(m->n_items, m->n_columns);
        }
    }

    /* Calculate the number of displayed rows and columns    */
    m->displayed_rows = menu_height / m->item_height;
    m->displayed_columns = menu_width / m->item_width;

    if (!(m->flags & UI_MATRIX_MENU_DISABLE_SCROLLBAR))
    {
        if ((!(m->flags & UI_MATRIX_MENU_AUTO_DISABLE_SCROLLBAR)) ||
            ((m->flags & UI_MATRIX_MENU_AUTO_DISABLE_SCROLLBAR) && (m->n_rows > m->displayed_rows)))
        {
            show_vbar = 1;
        }
        if ((!(m->flags & UI_MATRIX_MENU_AUTO_DISABLE_SCROLLBAR)) ||
            ((m->flags & UI_MATRIX_MENU_AUTO_DISABLE_SCROLLBAR) && (m->n_columns > m->displayed_columns)))
        {
            show_hbar = 1;
        }
    }

    if (show_vbar)
    {
        x2 -= m->vbar.width;
    }
    if (show_hbar)
    {
        y2 -= m->hbar.height;
    }
    menu_height = y2 - y1 + 1;
    menu_width = x2 - x1 + 1;

    /* Recalculate the number of rows that can be displayed  */
    m->displayed_rows = menu_height / m->item_height;
    m->displayed_columns = menu_width / m->item_width;

    /* Fix the number of displayed rows and columns       */
    if (m->displayed_columns > m->n_columns)
    {
        m->displayed_columns = m->n_columns;
    }
    if (m->displayed_rows > m->n_rows)
    {
        m->displayed_rows = m->n_rows;
    }

    /* Enable/Disable scrollbars                       */
    if (show_vbar)
    {
        m->flags |= UI_MATRIX_MENU_SHOW_VERTICAL_SCROLLBAR;
    }
    else
    {
        m->flags &= ~UI_MATRIX_MENU_SHOW_VERTICAL_SCROLLBAR;
    }
    if (show_hbar)
    {
        m->flags |= UI_MATRIX_MENU_SHOW_HORIZONTAL_SCROLLBAR;
    }
    else
    {
        m->flags &= ~UI_MATRIX_MENU_SHOW_HORIZONTAL_SCROLLBAR;
    }

    if (show_vbar && (!show_hbar))
    {
        gui_resize_vertical_scrollbar(&m->vbar, m->vbar.width, m->height - 4);
    }
    if (show_hbar && (!show_vbar))
    {
        gui_resize_horizontal_scrollbar(&m->hbar, m->width - 4, m->hbar.height);
    }
    if (show_vbar && show_hbar)
    {
        gui_resize_vertical_scrollbar(&m->vbar, m->vbar.width, m->height - m->hbar.height - 3);
        gui_resize_horizontal_scrollbar(&m->hbar, m->width - m->vbar.width - 3, m->hbar.height);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_setup
 * DESCRIPTION
 *  Used to set the number of rows, columns and item dimensions
 * PARAMETERS
 *  m               [IN]        Is the matrix menu object
 *  item_width      [IN]        Is the width of each item  | Any one of these two should be specified
 *  item_height     [IN]        Is the height of each item | Any one of these two should be specified
 *  n_columns       [IN]        Is the number of columns   | and the other can be zero.
 *  n_rows          [IN]        Is the number of rows      | and the other can be zero.
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_setup(matrix_menu *m, S32 item_width, S32 item_height, S32 n_columns, S32 n_rows)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->item_width = item_width;
    m->item_height = item_height;
    m->n_columns = n_columns;
    m->n_rows = n_rows;
    gui_matrix_menu_auto_configure(m);
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_matrix_menu
 * DESCRIPTION
 *  Changes the size of the matrix menu
 * PARAMETERS
 *  m           [IN]        Is the matrix menu object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_matrix_menu(matrix_menu *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    gui_resize_vertical_scrollbar(
        &m->vbar,
        current_matrix_menu_theme->vbar_width,
        m->height - current_matrix_menu_theme->hbar_height - 3);
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_matrix_menu_theme->vbar_width - 1, m->y + 2);
    gui_resize_horizontal_scrollbar(
        &m->hbar,
        m->width - current_matrix_menu_theme->vbar_width - 3,
        current_matrix_menu_theme->hbar_height);
    gui_move_horizontal_scrollbar(&m->hbar, m->x + 2, m->y + m->height - current_matrix_menu_theme->hbar_height - 1);
    gui_matrix_menu_auto_configure(m);
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_matrix_menu
 * DESCRIPTION
 *  Moves a matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 *  x       [IN]        Is the new left-top corner position
 *  y       [IN]        Is the new left-top corner position
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_matrix_menu(matrix_menu *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->x = x;
    m->y = y;
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_matrix_menu_theme->vbar_width - 1, m->y + 2);
    gui_move_horizontal_scrollbar(&m->hbar, m->x + 2, m->y + m->height - current_matrix_menu_theme->hbar_height - 1);
    gui_matrix_menu_auto_configure(m);
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_locate_highlighted_item
 * DESCRIPTION
 *  Used to locate the highlighted item in the matrix menu
 *  
 *  Internal function
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_locate_highlighted_item(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->highlighted_column = m->highlighted_item % m->n_columns;
    m->highlighted_row = m->highlighted_item / m->n_columns;
    if (m->highlighted_column < 0)
    {
        m->highlighted_column = 0;
    }
    if (m->highlighted_row < 0)
    {
        m->highlighted_row = 0;
    }
    if (m->highlighted_column > (m->n_columns - 1))
    {
        m->highlighted_column = m->n_columns - 1;
    }
    if (m->highlighted_row > (m->n_rows - 1))
    {
        m->highlighted_row = m->n_rows - 1;
    }
    if (m->highlighted_column < m->first_displayed_column)
    {
        m->first_displayed_column = m->highlighted_column;
    }
    if (m->highlighted_row < m->first_displayed_row)
    {
        m->first_displayed_row = m->highlighted_row;
    }
    if (m->highlighted_column > (m->first_displayed_column + m->displayed_columns - 1))
    {
        m->first_displayed_column = (m->highlighted_column - m->displayed_columns + 1);
        if (m->first_displayed_column > (m->n_columns - m->displayed_columns))
        {
            m->first_displayed_column = (m->n_columns - m->displayed_columns);
        }
    }
    if (m->highlighted_row > (m->first_displayed_row + m->displayed_rows - 1))
    {
        m->first_displayed_row = (m->highlighted_row - m->displayed_rows + 1);
        if (m->first_displayed_row > (m->n_rows - m->displayed_rows))
        {
            m->first_displayed_row = (m->n_rows - m->displayed_rows);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_next_item
 * DESCRIPTION
 *  Highlights the next item in the matrix
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_next_item(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_item >= (m->n_items - 1))
    {
        return;
    }
    m->highlighted_item++;
    gui_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_previous_item
 * DESCRIPTION
 *  Highlights the previous item in the matrix
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_previous_item(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_item <= 0)
    {
        return;
    }
    m->highlighted_item--;
    gui_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_previous_row
 * DESCRIPTION
 *  Highlights an item in the previous row
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_previous_row(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_row <= 0)
    {
        return;
    }
    m->highlighted_row--;
    if (m->highlighted_row < m->first_displayed_row)
    {
        m->first_displayed_row = m->highlighted_row;
    }
    m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_next_row
 * DESCRIPTION
 *  Highlights an item in the next row
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_next_row(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item;
    S32 old_highlighted_row;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    old_highlighted_row = m->highlighted_row;
    old_highlighted_item = m->highlighted_item;
    if (m->highlighted_row >= (m->n_rows - 1))
    {
        return;
    }
    m->highlighted_row++;
    if (m->highlighted_row > (m->first_displayed_row + m->displayed_rows - 1))
    {
        m->first_displayed_row = (m->highlighted_row - m->displayed_rows + 1);
        if (m->first_displayed_row > (m->n_rows - m->displayed_rows))
        {
            m->first_displayed_row = (m->n_rows - m->displayed_rows);
        }
    }
    m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;
    if (m->highlighted_item > (m->n_items - 1)) /* revert   */
    {
        m->highlighted_row = old_highlighted_row;
        m->highlighted_item = old_highlighted_item;
    }
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_previous_column
 * DESCRIPTION
 *  Highlights an item in the previous column
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_previous_column(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_column <= 0)
    {
        return;
    }
    m->highlighted_column--;
    if (m->highlighted_column < m->first_displayed_column)
    {
        m->first_displayed_column = m->highlighted_column;
    }
    m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_next_column
 * DESCRIPTION
 *  Highlights an item in the next column
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_next_column(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item;
    S32 old_highlighted_column;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    old_highlighted_column = m->highlighted_column;
    old_highlighted_item = m->highlighted_item;
    if (m->highlighted_column >= (m->n_columns - 1))
    {
        return;
    }
    m->highlighted_column++;
    if (m->highlighted_column > (m->first_displayed_column + m->displayed_columns - 1))
    {
        m->first_displayed_column = (m->highlighted_column - m->displayed_columns + 1);
        if (m->first_displayed_column > (m->n_columns - m->displayed_columns))
        {
            m->first_displayed_column = (m->n_columns - m->displayed_columns);
        }
    }
    m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;
    if (m->highlighted_item > (m->n_items - 1)) /* revert   */
    {
        m->highlighted_column = old_highlighted_column;
        m->highlighted_item = old_highlighted_item;
    }
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_first_item
 * DESCRIPTION
 *  Highlights the first item
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_first_item(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->highlighted_column = 0;
    m->highlighted_row = 0;
    m->first_displayed_column = 0;
    m->first_displayed_row = 0;
    m->highlighted_item = 0;
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_last_item
 * DESCRIPTION
 *  Highlights the last item
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_last_item(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->highlighted_item = m->n_items - 1;
    gui_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_next_page
 * DESCRIPTION
 *  Highlights an item in the next page
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_next_page(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item;
    S32 old_highlighted_row;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    old_highlighted_row = m->highlighted_row;
    old_highlighted_item = m->highlighted_item;
    if (m->highlighted_row >= (m->n_rows - 1))
    {
        return;
    }
    m->first_displayed_row += m->displayed_rows;
    if (m->first_displayed_row > (m->n_rows - m->displayed_rows))
    {
        m->first_displayed_row = (m->n_rows - m->displayed_rows);
    }
    m->highlighted_row = m->first_displayed_row + m->displayed_rows - 1;
    m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;
    if (m->highlighted_item > (m->n_items - 1)) /* revert   */
    {
        m->highlighted_row = old_highlighted_row;
        m->highlighted_item = old_highlighted_item;
    }
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_previous_page
 * DESCRIPTION
 *  Highlights an item in the previous page
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_previous_page(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    old_highlighted_item = m->highlighted_item;
    if (m->highlighted_row <= 0)
    {
        return;
    }
    m->first_displayed_row -= m->displayed_rows;
    if (m->first_displayed_row < 0)
    {
        m->first_displayed_row = 0;
    }
    m->highlighted_row = m->first_displayed_row;
    m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_matrix_menu_goto_item
 * DESCRIPTION
 *  Highlights a given item
 *  
 *  Does not redraw the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 *  i       [IN]        Is the index of the item (zero based)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_matrix_menu_goto_item(matrix_menu *m, S32 i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((i < 0) || (i > (m->n_items - 1)))
    {
        return;
    }
    m->highlighted_item = i;
    gui_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_matrix_menu
 * DESCRIPTION
 *  Displays the matrix menu
 * PARAMETERS
 *  m       [IN]        Is the matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_matrix_menu(matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    UI_filled_area *f;
    S32 i, j, k;
    S32 cx1, cy1, cx2, cy2;
    S32 tx1, ty1, tx2, ty2;
    S32 ix, iy, iwidth, iheight, ix2, iy2;
    U8 show_vbar = 0, show_hbar = 0;
    S32 vbar_x = 0, vbar_button_x = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_get_clip(&cx1, &cy1, &cx2, &cy2);
    gui_get_text_clip(&tx1, &ty1, &tx2, &ty2);
    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;
    if (m->flags & UI_LIST_MENU_STATE_FOCUSSED)
    {
        f = m->focussed_filler;
    }
    else
    {
        f = m->normal_filler;
    }
    gui_set_clip(x1, y1, x2 + 2, y2 + 2);
    gui_draw_filled_area(x1, y1, x2, y2, f);

    if (m->flags & UI_MATRIX_MENU_SHOW_VERTICAL_SCROLLBAR)
    {
        show_vbar = 1;
    }
    if (m->flags & UI_MATRIX_MENU_SHOW_HORIZONTAL_SCROLLBAR)
    {
        show_hbar = 1;
    }

    if (m->n_items == 0)
    {
        return;
    }
    x1 += 4;
    x2 -= 4;
    y1 += 4;
    y2 -= 4;
    if (show_vbar)
    {
        x2 -= m->vbar.width;
    }
    if (show_hbar)
    {
        y2 -= m->hbar.height;
    }
    iwidth = m->item_width;
    iheight = m->item_height;

    gui_set_text_clip(x1, y1, x2, y2);
    gui_set_clip(x1, y1, x2, y2);

    iy = y1;
    for (j = 0; j < m->displayed_rows; j++)
    {
        ix = x1;
        for (i = 0; i < m->displayed_columns; i++)
        {
            k = ((m->first_displayed_row + j) * m->n_columns) + (m->first_displayed_column + i);
            if (k > (m->n_items - 1))
            {
                break;
            }
            ix2 = ix + iwidth - 1;
            iy2 = iy + iheight - 1;
            if (ix2 > x2)
            {
                ix2 = x2;
            }
            if (iy2 > y2)
            {
                iy2 = y2;
            }
            gui_set_clip(ix, iy, ix2, iy2);
            gui_set_text_clip(ix, iy, ix2, iy2);
            if (k == m->highlighted_item)
            {
                m->items[k].highlight_function((void*)m->items[k].item);
            }
            else
            {
                m->items[k].remove_highlight_function((void*)m->items[k].item);
            }
            m->items[k].display_function((void*)m->items[k].item, ix, iy);
            ix += iwidth;
        }
        iy += iheight;
    }

    gui_set_clip(cx1, cy1, cx2, cy2);
    gui_set_text_clip(tx1, ty1, tx2, ty2);
    if (show_vbar)
    {
        gui_set_vertical_scrollbar_range(&m->vbar, m->n_rows);
        gui_set_vertical_scrollbar_scale(&m->vbar, m->displayed_rows);
        gui_set_vertical_scrollbar_value(&m->vbar, m->first_displayed_row);
         
        if (r2lMMIFlag)
        {
            vbar_x = m->vbar.x;
            vbar_button_x = m->vbar.scroll_button.x;
            m->vbar.x = m->vbar.x + m->vbar.width - m->width;
            m->vbar.scroll_button.x = m->vbar.scroll_button.x + m->vbar.scroll_button.width - m->width;
            gui_show_vertical_scrollbar(&m->vbar);
        }
        else
        {
            gui_show_vertical_scrollbar(&m->vbar);
        }
        /* CSD end */
        if (r2lMMIFlag)
        {
            m->vbar.x = vbar_x;
            m->vbar.scroll_button.x = vbar_button_x;
        }
    }
    if (show_hbar)
    {
        gui_set_horizontal_scrollbar_range(&m->hbar, m->n_columns);
        gui_set_horizontal_scrollbar_scale(&m->hbar, m->displayed_columns);
        gui_set_horizontal_scrollbar_value(&m->hbar, m->first_displayed_column);
        gui_show_horizontal_scrollbar(&m->hbar);
    }
}

