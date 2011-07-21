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
 *   gui_generic_menuitems.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Generic menu items - UI component
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
   Filename:      gui_generic_menuitems.c
   Date Created:  July-22-2002
   Contains:      UI routines for Generic Menuitems
**********************************************************************************/

#include "gui_generic_menuitems.h"
#include "gui_common_menus.h"

 
extern BOOL r2lMMIFlag;

/* CSD end */

/* GUI: text menuitem functions              */

UI_text_menuitem_theme *current_text_menuitem_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_text_menuitem_current_theme
 * DESCRIPTION
 *  Applies the current theme to a text menuitem
 * PARAMETERS
 *  m       [IN]        Is the text menuitem object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_text_menuitem_current_theme(text_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->disabled_filler = current_text_menuitem_theme->disabled_filler;
    m->disabled_text_color = current_text_menuitem_theme->disabled_text_color;
    m->flags |= current_text_menuitem_theme->flags;
    m->focussed_filler = current_text_menuitem_theme->focussed_filler;
    m->focussed_text_color = current_text_menuitem_theme->focussed_text_color;
    m->normal_filler = current_text_menuitem_theme->normal_filler;
    m->normal_text_color = current_text_menuitem_theme->normal_text_color;
    m->selected_filler = current_text_menuitem_theme->selected_filler;
    m->selected_text_color = current_text_menuitem_theme->selected_text_color;
    m->text_font = current_text_menuitem_theme->text_font;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_text_menuitem_theme
 * DESCRIPTION
 *  Applies the given theme to a text menuitem
 * PARAMETERS
 *  m       [IN]        Is the text menuitem object
 *  t       [IN]        Is the theme
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_text_menuitem_theme(text_menuitem *m, UI_text_menuitem_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->disabled_filler = t->disabled_filler;
    m->disabled_text_color = t->disabled_text_color;
    m->flags |= t->flags;
    m->focussed_filler = t->focussed_filler;
    m->focussed_text_color = t->focussed_text_color;
    m->normal_filler = t->normal_filler;
    m->normal_text_color = t->normal_text_color;
    m->selected_filler = t->selected_filler;
    m->selected_text_color = t->selected_text_color;
    m->text_font = t->text_font;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_text_menuitem
 * DESCRIPTION
 *  Creates a text menuitem
 * PARAMETERS
 *  m           [IN]        Is the text menuitem object
 *  x           [IN]        Is the left-top corner
 *  y           [IN]        Is the left-top corner
 *  width       [IN]        Width is the width of the text menuitem
 *  height      [IN]        Height is the height of the text menuitem
 *  text        [IN]        Is the text displayed for this menuitem
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_text_menuitem(text_menuitem *m, S32 x, S32 y, S32 width, S32 height, UI_string_type text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->x = x;   /* start x position of text of menu item */
    m->y = y;   /* start y poistion of text menu itemm */
    m->width = width;
    m->height = height;
    m->text = text;
    m->flags = 0;

    gui_set_text_menuitem_current_theme(m);
    /* Autocalculate the text position based on font   */
    if (m->flags & UI_MENUITEM_CENTER_TEXT_X)
    {
        S32 string_width;

        gui_set_font(m->text_font);
        string_width = gui_get_string_width(text);
        m->text_x = (width >> 1) - (string_width >> 1);
    }
    else
    {
        m->text_x = 0;
    }
    if (m->flags & UI_MENUITEM_CENTER_TEXT_Y)
    {
        S32 string_height;

        gui_set_font(m->text_font);
        string_height = gui_get_character_height();
        m->text_y = (height >> 1) - (string_height >> 1);
    }
    else
    {
        m->text_y = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_text_menuitem_set_text_position
 * DESCRIPTION
 *  Sets the text position within a text menuitem
 * PARAMETERS
 *  m       [IN]        Is the text menuitem object
 *  x       [IN]        Is the position of the text (offset from the left-top corner of the text menuitem)
 *  y       [IN]        Is the position of the text (offset from the left-top corner of the text menuitem)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_text_menuitem_set_text_position(text_menuitem *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->text_x = x;
    m->text_y = y;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_text_menuitem
 * DESCRIPTION
 *  Displays the text menuitem
 * PARAMETERS
 *  m       [IN]        Is the text menuitem object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_text_menuitem(text_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 text_x, text_y;
    color_t text_color;
    UI_filled_area *f;
    S32 w;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;
    text_x = m->text_x;
    text_y = m->text_y;

    if (m->flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        if (m->flags & UI_MENUITEM_FOCUSSED_SHIFT1)
        {
            x1 += 2;
            y1 += 2;
            x2 += 2;
            y2 += 2;
        }
        else if (m->flags & UI_MENUITEM_FOCUSSED_SHIFT2)
        {
            x1 += 1;
            y1 += 1;
            x2 += 1;
            y2 += 1;
        }
        f = m->focussed_filler;
        text_color = m->focussed_text_color;
    }
    else if (m->flags & UI_MENUITEM_STATE_DISABLED)
    {
        f = m->disabled_filler;
        text_color = m->focussed_text_color;
    }
    else if (m->flags & UI_MENUITEM_STATE_SELECTED)
    {
        f = m->selected_filler;
        text_color = m->selected_text_color;
    }
    else
    {
        f = m->normal_filler;
        text_color = m->normal_text_color;
    }
    gui_draw_filled_area(x1, y1, x2, y2, f);
    gui_push_text_clip();
    gui_set_text_clip(x1, y1, x2, y2);
     
    w = gui_get_string_width((UI_string_type) m->text);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + text_x + w, y1 + text_y);
    }
    else
    {
        gui_move_text_cursor(x1 + text_x, y1 + text_y);
    }
    /* CSD end */

    gui_set_text_color(text_color);
    gui_set_font(m->text_font);
    gui_print_text(m->text);
    gui_pop_text_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_text_menuitem_xy
 * DESCRIPTION
 *  Displays the text menuitem
 * PARAMETERS
 *  m       [IN]        Is the text menuitem object
 *  x       [IN]        Is the location at which to display
 *  y       [IN]        Is the location at which to display
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_text_menuitem_xy(text_menuitem *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 text_x, text_y;
    color_t text_color;
    UI_filled_area *f;
    S32 w;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = m->x + x;
    y1 = m->y + y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;
    text_x = m->text_x;
    text_y = m->text_y;

    if (m->flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        if (m->flags & UI_MENUITEM_FOCUSSED_SHIFT1)
        {
            x1 += 2;
            y1 += 2;
            x2 += 2;
            y2 += 2;
        }
        else if (m->flags & UI_MENUITEM_FOCUSSED_SHIFT2)
        {
            x1 += 1;
            y1 += 1;
            x2 += 1;
            y2 += 1;
        }
        f = m->focussed_filler;
        text_color = m->focussed_text_color;
    }
    else if (m->flags & UI_MENUITEM_STATE_DISABLED)
    {
        f = m->disabled_filler;
        text_color = m->focussed_text_color;
    }
    else if (m->flags & UI_MENUITEM_STATE_SELECTED)
    {
        f = m->selected_filler;
        text_color = m->selected_text_color;
    }
    else
    {
        f = m->normal_filler;
        text_color = m->normal_text_color;
    }
    gui_push_text_clip();
    gui_set_text_clip_preset(x1, y1, x2, y2);
    gui_push_clip();
    gui_set_clip_preset(x1, y1, x2 + 2, y2 + 2);
    gui_draw_filled_area(x1, y1, x2, y2, f);
     
    w = gui_get_string_width((UI_string_type) m->text);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + text_x + w, y1 + text_y);
    }
    else
    {
        gui_move_text_cursor(x1 + text_x, y1 + text_y);
    }
    /* CSD end */
    gui_set_text_color(text_color);
    gui_set_font(m->text_font);
    gui_print_text(m->text);
    gui_pop_text_clip();
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_vshow_text_menuitem
 * DESCRIPTION
 *  Displays the text menuitem
 *  
 *  Generic menuitem implementation
 * PARAMETERS
 *  m       [IN]        Is the text menuitem object
 *  x       [IN]        Is the location at which to display
 *  y       [IN]        Is the location at which to display
 * RETURNS
 *  void
 *****************************************************************************/
void gui_vshow_text_menuitem(void *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_show_text_menuitem_xy((text_menuitem*) m, x, y);
}


/*****************************************************************************
 * FUNCTION
 *  gui_measure_text_menuitem
 * DESCRIPTION
 *  Measures a text menuitem
 *  
 *  Generic menuitem implementation
 * PARAMETERS
 *  m           [IN]        Is the text menuitem object
 *  x           [?]         
 *  y           [?]         
 *  width       [OUT]       
 *  height      [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
void gui_measure_text_menuitem(void *m, S32 *x, S32 *y, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    text_menuitem *t = (text_menuitem*) m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *x = t->x;
    *y = t->y;
    *width = t->width;
    *height = t->height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_highlight_text_menuitem
 * DESCRIPTION
 *  Highlights a text menuitem (not visually)
 *  
 *  Generic menuitem implementation
 * PARAMETERS
 *  m       [IN]        Is the text menuitem object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_highlight_text_menuitem(void *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    text_menuitem *t = (text_menuitem*) m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (t->flags & UI_MENUITEM_DISABLE_HIGHLIGHT)
    {
        return;
    }
    t->flags |= UI_MENUITEM_STATE_FOCUSSED;
}


/*****************************************************************************
 * FUNCTION
 *  gui_remove_highlight_text_menuitem
 * DESCRIPTION
 *  Removes highlight of a text menuitem (not visually)
 *  
 *  Generic menuitem implementation
 * PARAMETERS
 *  m       [IN]        Is the text menuitem object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_remove_highlight_text_menuitem(void *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    text_menuitem *t = (text_menuitem*) m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    t->flags &= ~UI_MENUITEM_STATE_FOCUSSED;
}

/* GUI: icon text menuitem functions               */

UI_icontext_menuitem_theme *current_icontext_menuitem_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_icontext_menuitem_current_theme
 * DESCRIPTION
 *  Applies the current theme to a icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the icontext menuitem object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_icontext_menuitem_current_theme(icontext_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->disabled_filler = current_icontext_menuitem_theme->disabled_filler;
    m->disabled_text_color = current_icontext_menuitem_theme->disabled_text_color;
    m->flags |= current_icontext_menuitem_theme->flags;
    m->focussed_filler = current_icontext_menuitem_theme->focussed_filler;
    m->focussed_text_color = current_icontext_menuitem_theme->focussed_text_color;
    m->normal_filler = current_icontext_menuitem_theme->normal_filler;
    m->normal_text_color = current_icontext_menuitem_theme->normal_text_color;
    m->selected_filler = current_icontext_menuitem_theme->selected_filler;
    m->selected_text_color = current_icontext_menuitem_theme->selected_text_color;
    m->text_font = current_icontext_menuitem_theme->text_font;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_icontext_menuitem_theme
 * DESCRIPTION
 *  Applies the given theme to a icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the icontext menuitem object
 *  t       [IN]        Is the theme
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_icontext_menuitem_theme(icontext_menuitem *m, UI_icontext_menuitem_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->disabled_filler = t->disabled_filler;
    m->disabled_text_color = t->disabled_text_color;
    m->flags |= t->flags;
    m->focussed_filler = t->focussed_filler;
    m->focussed_text_color = t->focussed_text_color;
    m->normal_filler = t->normal_filler;
    m->normal_text_color = t->normal_text_color;
    m->selected_filler = t->selected_filler;
    m->selected_text_color = t->selected_text_color;
    m->text_font = t->text_font;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_icontext_menuitem
 * DESCRIPTION
 *  Creates an icontext menuitem
 * PARAMETERS
 *  m               [IN]        Is the icontext menuitem object
 *  x               [IN]        Is the left-top corner
 *  y               [IN]        Is the left-top corner
 *  width           [IN]        Width is the width of the icontext menuitem
 *  height          [IN]        Height is the height of the icontext menuitem
 *  text            [IN]        Is the text displayed for this menuitem
 *  item_icon       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_icontext_menuitem(
        icontext_menuitem *m,
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        UI_string_type text,
        PU8 item_icon)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->x = x;
    m->y = y;
    m->width = width;
    m->height = height;
    m->text = text;
    m->flags = 0;
    m->item_icon = item_icon;
    gui_set_icontext_menuitem_current_theme(m);
    /* Autocalculate the text position based on font   */
    if (m->flags & UI_MENUITEM_CENTER_TEXT_X)
    {
        S32 string_width;

        gui_set_font(m->text_font);
        string_width = gui_get_string_width(text);
        m->text_x = (width >> 1) - (string_width >> 1);
    }
    else
    {
        m->text_x = 0;
    }
    if (m->flags & UI_MENUITEM_CENTER_TEXT_Y)
    {
        S32 string_height;

        gui_set_font(m->text_font);
        string_height = gui_get_character_height();
        m->text_y = (height >> 1) - (string_height >> 1);
    }
    else
    {
        m->text_y = 0;
    }
    /* Autocalculate the icon position              */
    if (m->item_icon != UI_NULL_IMAGE)
    {
        S32 icon_width, icon_height;

        gui_measure_image(m->item_icon, &icon_width, &icon_height);
        if (m->flags & UI_MENUITEM_CENTER_ICON_X)
        {
            m->icon_x = (width >> 1) - (icon_width >> 1);
        }
        else
        {
            m->icon_x = 0;
        }
        if (m->flags & UI_MENUITEM_CENTER_ICON_Y)
        {
            m->icon_y = (height >> 1) - (icon_height >> 1);
        }
        else
        {
            m->icon_y = 0;
        }
        m->icon_transparent_color = gui_transparent_color(0, 0, 0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_icontext_menuitem
 * DESCRIPTION
 *  Changes the size of an icontext menuitem
 * PARAMETERS
 *  m           [IN]        Is the text menuitem object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_icontext_menuitem(icontext_menuitem *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    /* Autocalculate the text position based on font   */
    if (m->flags & UI_MENUITEM_CENTER_TEXT_X)
    {
        S32 string_width;

        gui_set_font(m->text_font);
        string_width = gui_get_string_width(m->text);
        m->text_x = (width >> 1) - (string_width >> 1);
    }
    else
    {
        m->text_x = 0;
    }
    if (m->flags & UI_MENUITEM_CENTER_TEXT_Y)
    {
        S32 string_height;

        gui_set_font(m->text_font);
        string_height = gui_get_character_height();
        m->text_y = (height >> 1) - (string_height >> 1);
    }
    else
    {
        m->text_y = 0;
    }
    /* Autocalculate the icon position              */
    if (m->item_icon != UI_NULL_IMAGE)
    {
        S32 icon_width, icon_height;

        gui_measure_image(m->item_icon, &icon_width, &icon_height);
        if (m->flags & UI_MENUITEM_CENTER_ICON_X)
        {
            m->icon_x = (width >> 1) - (icon_width >> 1);
        }
        else
        {
            m->icon_x = 0;
        }
        if (m->flags & UI_MENUITEM_CENTER_ICON_Y)
        {
            m->icon_y = (height >> 1) - (icon_height >> 1);
        }
        else
        {
            m->icon_y = 0;
        }
        m->icon_transparent_color = gui_transparent_color(0, 0, 0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_icontext_menuitem_set_text_position
 * DESCRIPTION
 *  Sets the text position within a icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the icontext menuitem object
 *  x       [IN]        Is the position of the text (offset from the left-top corner of the menuitem)
 *  y       [IN]        Is the position of the text (offset from the left-top corner of the menuitem)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_icontext_menuitem_set_text_position(icontext_menuitem *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->text_x = x;
    m->text_y = y;
}


/*****************************************************************************
 * FUNCTION
 *  gui_icontext_menuitem_set_icon_position
 * DESCRIPTION
 *  Sets the icon position within a icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the icontext menuitem object
 *  x       [IN]        Is the position of the icon (offset from the left-top corner of the menuitem)
 *  y       [IN]        Is the position of the icon (offset from the left-top corner of the menuitem)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_icontext_menuitem_set_icon_position(icontext_menuitem *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->icon_x = x;
    m->icon_x = y;
}


/*****************************************************************************
 * FUNCTION
 *  gui_icontext_menuitem_set_icon
 * DESCRIPTION
 *  Sets the icon to be displayed in a icontext menuitem
 * PARAMETERS
 *  m                           [IN]        Is the icontext menuitem object
 *  item_icon                   [IN]        Is the icon to be displayed
 *  icon_transparent_color      [IN]        Is the transparent color value for the
 * RETURNS
 *  void
 *****************************************************************************/
void gui_icontext_menuitem_set_icon(
        icontext_menuitem *m,
        PU8 item_icon,
        UI_transparent_color_type icon_transparent_color)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(icon_transparent_color);
    m->item_icon = item_icon;
    /* Autocalculate the icon position              */
    if (m->item_icon != UI_NULL_IMAGE)
    {
        S32 icon_width, icon_height;

        gui_measure_image(m->item_icon, &icon_width, &icon_height);
        if (m->flags & UI_MENUITEM_CENTER_ICON_X)
        {
            m->icon_x = (m->width >> 1) - (icon_width >> 1);
        }
        else
        {
            m->icon_x = 0;
        }
        if (m->flags & UI_MENUITEM_CENTER_ICON_Y)
        {
            m->icon_y = (m->height >> 1) - (icon_height >> 1);
        }
        else
        {
            m->icon_y = 0;
        }
        m->icon_transparent_color = gui_transparent_color(0, 0, 0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_icontext_menuitem
 * DESCRIPTION
 *  Displays the icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the icontext menuitem object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_icontext_menuitem(icontext_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 text_x, text_y;
    color_t text_color;
    UI_filled_area *f;

    /* byte animation_flag=0; */
    S32 w;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;
    text_x = m->text_x;
    text_y = m->text_y;

    if (m->flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        if (m->flags & UI_MENUITEM_FOCUSSED_SHIFT1)
        {
            x1 += 2;
            y1 += 2;
            x2 += 2;
            y2 += 2;
        }
        else if (m->flags & UI_MENUITEM_FOCUSSED_SHIFT2)
        {
            x1 += 1;
            y1 += 1;
            x2 += 1;
            y2 += 1;
        }
        f = m->focussed_filler;
        text_color = m->focussed_text_color;
        /* if(m->flags & UI_MENUITEM_FOCUSSED_ANIMATE) animation_flag=1; */
    }
    else if (m->flags & UI_MENUITEM_STATE_DISABLED)
    {
        f = m->disabled_filler;
        text_color = m->focussed_text_color;
    }
    else if (m->flags & UI_MENUITEM_STATE_SELECTED)
    {
        f = m->selected_filler;
        text_color = m->selected_text_color;
        /* if(m->flags & UI_MENUITEM_SELECTED_ANIMATE) animation_flag=1; */
    }
    else
    {
        f = m->normal_filler;
        text_color = m->normal_text_color;
    }
    gui_push_clip();
    gui_set_clip(x1, y1, x2 + 2, y2 + 2);
    gui_draw_filled_area(x1, y1, x2, y2, f);
    if (m->item_icon != UI_NULL_IMAGE)
    {
        gui_show_transparent_image(x1 + m->icon_x, y1 + m->icon_y, m->item_icon, m->icon_transparent_color);
    }
    if (!(m->flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY))
    {
        gui_push_text_clip();
        gui_set_text_clip(x1, y1, x2, y2);
         
        w = gui_get_string_width((UI_string_type) m->text);
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(x1 + text_x + w, y1 + text_y);
        }
        else
        {
            gui_move_text_cursor(x1 + text_x, y1 + text_y);
        }
        /* CSD end */
        gui_set_text_color(text_color);
        gui_set_font(m->text_font);
        gui_print_text(m->text);
        gui_pop_text_clip();
    }
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_icontext_menuitem_xy
 * DESCRIPTION
 *  Displays the icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the icontext menuitem object
 *  x       [IN]        Is the location at which to display
 *  y       [IN]        Is the location at which to display
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_icontext_menuitem_xy(icontext_menuitem *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 text_x, text_y;
    color_t text_color;
    UI_filled_area *f;

    /* byte animation_flag=0; */
    S32 w;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = m->x + x;
    y1 = m->y + y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;
    text_x = m->text_x;
    text_y = m->text_y;

    if (m->flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        if (m->flags & UI_MENUITEM_FOCUSSED_SHIFT1)
        {
            x1 += 2;
            y1 += 2;
            x2 += 2;
            y2 += 2;
        }
        else if (m->flags & UI_MENUITEM_FOCUSSED_SHIFT2)
        {
            x1 += 1;
            y1 += 1;
            x2 += 1;
            y2 += 1;
        }
        f = m->focussed_filler;
        text_color = m->focussed_text_color;
        /* if(m->flags & UI_MENUITEM_FOCUSSED_ANIMATE) animation_flag=1; */
    }
    else if (m->flags & UI_MENUITEM_STATE_DISABLED)
    {
        f = m->disabled_filler;
        text_color = m->focussed_text_color;
    }
    else if (m->flags & UI_MENUITEM_STATE_SELECTED)
    {
        f = m->selected_filler;
        text_color = m->selected_text_color;
        /* if(m->flags & UI_MENUITEM_SELECTED_ANIMATE) animation_flag=1; */
    }
    else
    {
        f = m->normal_filler;
        text_color = m->normal_text_color;
    }
    gui_push_clip();
    gui_set_clip_preset(x1, y1, x2 + 2, y2 + 2);
    gui_draw_filled_area(x1, y1, x2, y2, f);
    if (m->item_icon != UI_NULL_IMAGE)
    {
        gui_show_transparent_image(x1 + m->icon_x, y1 + m->icon_y, m->item_icon, m->icon_transparent_color);
    }
    if (!(m->flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY))
    {
        gui_push_text_clip();
        gui_set_text_clip_preset(x1, y1, x2, y2);
         
        w = gui_get_string_width((UI_string_type) m->text);
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(x1 + text_x + w, y1 + text_y);
        }
        else
        {
            gui_move_text_cursor(x1 + text_x, y1 + text_y);
        }
        /* CSD end */
        gui_set_text_color(text_color);
        gui_set_font(m->text_font);
        gui_print_text(m->text);
        gui_pop_text_clip();
    }
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_vshow_icontext_menuitem
 * DESCRIPTION
 *  Displays the icontext menuitem
 *  
 *  Generic menuitem implementation
 * PARAMETERS
 *  m       [IN]        Is the icontext menuitem object
 *  x       [IN]        Is the location at which to display
 *  y       [IN]        Is the location at which to display
 * RETURNS
 *  void
 *****************************************************************************/
void gui_vshow_icontext_menuitem(void *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_show_icontext_menuitem_xy((icontext_menuitem*) m, x, y);
}


/*****************************************************************************
 * FUNCTION
 *  gui_measure_icontext_menuitem
 * DESCRIPTION
 *  Measures a icontext menuitem
 *  
 *  Generic menuitem implementation
 * PARAMETERS
 *  m           [IN]        Is the icontext menuitem object
 *  x           [?]         
 *  y           [?]         
 *  width       [OUT]       
 *  height      [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
void gui_measure_icontext_menuitem(void *m, S32 *x, S32 *y, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    icontext_menuitem *t = (icontext_menuitem*) m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *x = t->x;
    *y = t->y;
    *width = t->width;
    *height = t->height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_highlight_icontext_menuitem
 * DESCRIPTION
 *  Highlights a icontext menuitem (not visually)
 *  
 *  Generic menuitem implementation
 * PARAMETERS
 *  m       [IN]        Is the icontext menuitem object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_highlight_icontext_menuitem(void *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    icontext_menuitem *t = (icontext_menuitem*) m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (t->flags & UI_MENUITEM_DISABLE_HIGHLIGHT)
    {
        return;
    }
    t->flags |= UI_MENUITEM_STATE_FOCUSSED;
}


/*****************************************************************************
 * FUNCTION
 *  gui_remove_highlight_icontext_menuitem
 * DESCRIPTION
 *  Removes highlight of a icontext menuitem (not visually)
 *  
 *  Generic menuitem implementation
 * PARAMETERS
 *  m       [IN]        Is the icontext menuitem object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_remove_highlight_icontext_menuitem(void *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    icontext_menuitem *t = (icontext_menuitem*) m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    t->flags &= ~UI_MENUITEM_STATE_FOCUSSED;
}

