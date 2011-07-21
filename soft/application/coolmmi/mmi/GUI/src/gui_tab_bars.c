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
 *   gui_tab_bars.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Tab Bar - UI component
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
#include "gui_themes.h"
#include "gui_tab_bars.h"
#include "wgui.h"
#include "wgui_tab_bars.h"
#include "wgui_menu_shortcuts.h"
#include "wgui_categories_util.h"
#include "debuginitdef.h"

extern BOOL r2lMMIFlag;

#ifdef __MMI_UI_TAB_PANE__

/*
 * TODO:
 * *
 * * If tab items is blinking but not displayed, the corresponding left/right arrows should be blinking, too.
 * *
 */

/***************************************************************************** 
* Define
*****************************************************************************/

/* Number of tabs to display ahead on scrolling */
#define UI_TAB_BAR_SHOW_AHEAD                (1)

#ifdef __MMI_MAINLCD_240X320__
#define UI_TAB_BAR_ARROW_ICON_GAP            (5)        /* be careful of overlapping with tab icon */
#else 
#define UI_TAB_BAR_ARROW_ICON_GAP            (3)
#endif 

#define UI_TAB_BAR_BLINKING_TIME             (500)

/* In previous design, focused tab item overlaps other tab items.
   However, it is not true anymore */
#undef UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/

/* Used for blinking */
static horizontal_tab_bar *UI_current_horizontal_tab_bar;

/***************************************************************************** 
* Local Function
*****************************************************************************/
/* 
 * Adjust first_displayed_item, last_displayed_item, n_displayable_item according to focused_item
 * when the tab bar is initially displayed.
 */


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_locate_focused_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_tab_bar_locate_focused_item(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tb->n_displayable_item = UI_device_width / tb->tab_width;

    if (tb->n_displayable_item > UI_TAB_BAR_SHOW_AHEAD)
    {
        tb->n_item_ahead = UI_TAB_BAR_SHOW_AHEAD;
    }
    else
    {
        tb->n_item_ahead = 0;
    }

    if (tb->n_items <= tb->n_displayable_item)
    {
        tb->first_displayed_item = 0;
        tb->last_displayed_item = tb->n_items - 1;
    }
    else if (tb->focused_item <= tb->n_item_ahead)
    {
        tb->first_displayed_item = 0;
        tb->last_displayed_item = tb->n_displayable_item - 1;
    }
    else if (tb->focused_item - tb->n_item_ahead + tb->n_displayable_item - 1 <= tb->n_items - 1)
    {
        tb->first_displayed_item = tb->focused_item - tb->n_item_ahead;
        tb->last_displayed_item = tb->first_displayed_item + tb->n_displayable_item - 1;
    }
    else
    {
        tb->first_displayed_item = tb->n_items - tb->n_displayable_item;
        tb->last_displayed_item = tb->n_items - 1;
    }
}

/* 
 * Adjust first_displayed_item, last_displayed_item, n_displayable_item according to focused_item
 * when the focus item index is incremented. 
 */


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_locate_next_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_tab_bar_locate_next_item(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tb->n_displayable_item == 0)
    {
        gui_horizontal_tab_bar_locate_focused_item(tb);
    }

    if ((tb->last_displayed_item == tb->n_items - 1) ||
        (tb->focused_item + tb->n_item_ahead <= tb->last_displayed_item))
    {
        /* Unnecessary to rotate the tab items */
        return;
    }

    if (tb->n_items <= tb->n_displayable_item)
    {
        tb->first_displayed_item = 0;
        tb->last_displayed_item = tb->n_items - 1;
    }
    else if (tb->focused_item + tb->n_item_ahead >= tb->n_items - 1)
    {
        tb->first_displayed_item = tb->n_items - tb->n_displayable_item;
        tb->last_displayed_item = tb->n_items - 1;
    }
    else if (tb->focused_item + tb->n_item_ahead <= tb->n_displayable_item - 1)
    {
        tb->first_displayed_item = 0;
        tb->last_displayed_item = tb->n_displayable_item - 1;
    }
    else
    {
        tb->last_displayed_item = tb->focused_item + tb->n_item_ahead;
        tb->first_displayed_item = tb->last_displayed_item + 1 - tb->n_displayable_item;
    }
}

/* 
 * Adjust first_displayed_item, last_displayed_item, n_displayable_item according to focused_item
 * when the focus item index is decremented. Try to show 'UI_TAB_BAR_SHOW_AHEAD' items ahead.
 * gui_horizontal_tab_bar_locate_focused_item() should be invoked before this API.
 */


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_locate_prev_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_tab_bar_locate_prev_item(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tb->n_displayable_item == 0)
    {
        gui_horizontal_tab_bar_locate_focused_item(tb);
    }

    if ((tb->first_displayed_item == 0) || (tb->focused_item >= tb->first_displayed_item + tb->n_item_ahead))
    {
        /* Unnecessary to rotate the tab items */
        return;
    }

    if (tb->n_items <= tb->n_displayable_item)
    {
        tb->first_displayed_item = 0;
        tb->last_displayed_item = tb->n_items - 1;
    }
    else if (tb->focused_item <= tb->n_item_ahead)
    {
        tb->first_displayed_item = 0;
        tb->last_displayed_item = tb->n_displayable_item - 1;
    }
    else if (tb->focused_item - tb->n_item_ahead + tb->n_displayable_item - 1 >= tb->n_items - 1)
    {
        tb->first_displayed_item = tb->n_items - tb->n_displayable_item;
        tb->last_displayed_item = tb->n_items - 1;
    }
    else
    {
        tb->first_displayed_item = tb->focused_item - tb->n_item_ahead;
        tb->last_displayed_item = tb->first_displayed_item + tb->n_displayable_item - 1;
    }
}


/*****************************************************************************
 * FUNCTION
 *  my_draw_image_filler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 *  imageid     [IN]        
 *  filler      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
static void my_draw_image_filler(S32 x1, S32 y1, S32 x2, S32 y2, U16 imageid, UI_filled_area *filler)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (imageid)
    {
        gui_show_transparent_image(x1, y1, (PU8) GetImage(imageid), UI_DEFAULT_TRANSPARENT_COLOR);
    }
    else if (filler)
    {
        gui_draw_filled_area(x1, y1, x2, y2, filler);
    }
}

/*
 * IN    tb, index
 * OUT   x1, x2
 */


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_get_item_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb          [?]         
 *  index       [IN]        
 *  x1          [?]         
 *  x2          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_tab_bar_get_item_position(horizontal_tab_bar *tb, S32 index, S32 *x1, S32 *x2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, x;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x = tb->x;
    for (i = tb->first_displayed_item; i <= tb->last_displayed_item; i++)
    {
        if (i == index)
        {
            *x1 = x;
            if (i == tb->focused_item)
            {
                *x2 = x + tb->focused_tab_width - 1;
            }
            else
            {
                *x2 = x + tb->tab_width - 1;
            }
            return;
        }

        if (i == tb->focused_item)
        {
            x += tb->focused_tab_width;
        }
        else
        {
            x += tb->tab_width;
        }
    }
    MMI_DBG_ASSERT(0);
    *x1 = *x2 = 0;
    return;
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_show_left_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb                  [?]         
 *  tab_y_offset        [IN]        
 *  x1                  [IN]        
 *  x2                  [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_tab_bar_show_left_arrow(horizontal_tab_bar *tb, S32 tab_y_offset, S32 x1, S32 x2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 img;
    S32 iw, ih;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x2);

    img = (PU8) GetImage(tb->theme->left_button_icon);
    if (img)
    {
        S32 icon_x, icon_y;

        gui_measure_image(img, &iw, &ih);
        icon_x = x1 + UI_TAB_BAR_ARROW_ICON_GAP;
        icon_y = tab_y_offset + (tb->tab_height >> 1) - (ih >> 1);
        gui_show_transparent_image(
            icon_x,
            icon_y + (tb->left_button_pressed ? 1 : 0),
            img,
            UI_DEFAULT_TRANSPARENT_COLOR);

    #ifdef __MMI_TOUCH_SCREEN__
        /* The pen area is larger than the icon itself */
        tb->pen_state.left_button_x = (S16) tb->x;
        tb->pen_state.left_button_y = (S16) tb->y;
        tb->pen_state.left_button_width = (S16) iw + (UI_TAB_BAR_ARROW_ICON_GAP << 1);
        tb->pen_state.left_button_height = (S16) tb->tab_area_height;
    #endif /* __MMI_TOUCH_SCREEN__ */ 
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_show_right_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb                  [?]         
 *  tab_y_offset        [IN]        
 *  x1                  [IN]        
 *  x2                  [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_tab_bar_show_right_arrow(horizontal_tab_bar *tb, S32 tab_y_offset, S32 x1, S32 x2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 img;
    S32 iw, ih;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x1);

    img = (PU8) GetImage(tb->theme->right_button_icon);
    if (img)
    {
        S32 icon_x, icon_y;

        gui_measure_image(img, &iw, &ih);
        icon_x = x2 - UI_TAB_BAR_ARROW_ICON_GAP - iw;
        icon_y = tab_y_offset + (tb->tab_height >> 1) - (ih >> 1);
        gui_show_transparent_image(
            icon_x,
            icon_y + (tb->right_button_pressed ? 1 : 0),
            img,
            UI_DEFAULT_TRANSPARENT_COLOR);

    #ifdef __MMI_TOUCH_SCREEN__
        /* The pen area is larger than the icon itself */
        tb->pen_state.right_button_x = (S16) icon_x - UI_TAB_BAR_ARROW_ICON_GAP;
        tb->pen_state.right_button_y = (S16) tb->y;
        tb->pen_state.right_button_width = (S16) iw + (UI_TAB_BAR_ARROW_ICON_GAP << 1);
        tb->pen_state.right_button_height = (S16) tb->tab_area_height;
    #endif /* __MMI_TOUCH_SCREEN__ */ 
    }
}

/* Show a (unfocused) tab item */


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_show_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb      [?]         
 *  i       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_tab_bar_show_item(horizontal_tab_bar *tb, int i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, tab_y_offset;
    BOOL blinking_displayed = FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((i == tb->focused_item) || (i < tb->first_displayed_item) || (i > tb->last_displayed_item))
    {
        return;
    }

    gui_horizontal_tab_bar_get_item_position(tb, i, &x1, &x2);
    y1 = tb->y;
    y2 = tb->y + tb->tab_area_height - 1;

    gui_set_clip(x1, y1, x2, y2);
    gui_set_text_clip(x1, y1, x2, y2);

    if ((tb->flags & UI_TAB_BAR_BLINKING_DISPLAYED) && (tb->items[i].flags & UI_TAB_BAR_ITEM_STATE_BLINKING))
    {
        blinking_displayed = TRUE;
    }

    tab_y_offset = y1 + (tb->focused_tab_height - tb->tab_height);

    if (i == 0)
    {
        if (blinking_displayed)
        {
            my_draw_image_filler(
                x1,
                tab_y_offset,
                x2,
                y2,
                tb->theme->blinking_l_tab_icon,
                tb->theme->blinking_l_tab_filler);
        }
        else
        {
            my_draw_image_filler(
                x1,
                tab_y_offset,
                x2,
                y2,
                tb->theme->normal_l_tab_icon,
                tb->theme->normal_l_tab_filler);
        }
    }
    else if (i == tb->n_items - 1)
    {
        if (blinking_displayed)
        {
            my_draw_image_filler(
                x1,
                tab_y_offset,
                x2,
                y2,
                tb->theme->blinking_r_tab_icon,
                tb->theme->blinking_r_tab_filler);
        }
        else
        {
            my_draw_image_filler(
                x1,
                tab_y_offset,
                x2,
                y2,
                tb->theme->normal_r_tab_icon,
                tb->theme->normal_r_tab_filler);
        }
    }
    else
    {
        if (blinking_displayed)
        {
            my_draw_image_filler(
                x1,
                tab_y_offset,
                x2,
                y2,
                tb->theme->blinking_tab_icon,
                tb->theme->blinking_tab_filler);
        }
        else
        {
            my_draw_image_filler(x1, tab_y_offset, x2, y2, tb->theme->normal_tab_icon, tb->theme->normal_tab_filler);
        }
    }

    if (tb->items[i].icon)
    {
        S32 iw, ih;

        gui_measure_image(tb->items[i].icon, &iw, &ih);
        gui_show_transparent_image(
            x1 + (tb->tab_width >> 1) - (iw >> 1),
            tab_y_offset + (tb->tab_height >> 1) - (ih >> 1),
            tb->items[i].icon,
            UI_DEFAULT_TRANSPARENT_COLOR);
    }

    if (tb->items[i].text)
    {
        gui_set_font(tb->theme->tab_text_font);
        gui_set_text_color(tb->theme->normal_tab_text_color);
        if (r2lMMIFlag)
        {
            gui_print_truncated_text(
                x1 + tb->tab_text_x + tb->tab_text_width - 1,
                y1 + tb->tab_text_y,
                tb->tab_text_width,
                tb->items[i].text);
        }
        else
        {
            gui_print_truncated_text(x1 + tb->tab_text_x, y1 + tb->tab_text_y, tb->tab_text_width, tb->items[i].text);
        }
    }

    if ((i == tb->first_displayed_item) && (i != 0))
    {
        gui_horizontal_tab_bar_show_left_arrow(tb, tab_y_offset, x1, x2);
    }

    if ((i == tb->last_displayed_item) && (i != tb->n_items - 1))
    {
        gui_horizontal_tab_bar_show_right_arrow(tb, tab_y_offset, x1, x2);
    }
}

/* Show a focused tab item */


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_show_focused_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_tab_bar_show_focused_item(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, tab_y_offset;
    int i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = tb->focused_item;

    if (i < tb->first_displayed_item || i > tb->last_displayed_item)
    {
        return;
    }

    gui_horizontal_tab_bar_get_item_position(tb, i, &x1, &x2);
    y1 = tb->y;
    y2 = tb->y + tb->tab_area_height - 1;

    gui_set_clip(x1, y1, x2, y2);
    gui_set_text_clip(x1, y1, x2, y2);

    if (i == 0)
    {
        my_draw_image_filler(x1, y1, x2, y2, tb->theme->focused_l_tab_icon, tb->theme->focused_l_tab_filler);
    }
    else if (i == tb->n_items - 1)
    {
        my_draw_image_filler(x1, y1, x2, y2, tb->theme->focused_r_tab_icon, tb->theme->focused_r_tab_filler);
    }
    else
    {
        my_draw_image_filler(x1, y1, x2, y2, tb->theme->focused_tab_icon, tb->theme->focused_tab_filler);
    }

    if (tb->items[i].icon)
    {
        S32 iw, ih;

        gui_measure_image(tb->items[i].icon, &iw, &ih);

        gui_show_transparent_image(
            x1 + (tb->focused_tab_width >> 1) - (iw >> 1),
            y1 + (tb->focused_tab_height >> 1) - (ih >> 1),
            tb->items[i].icon,
            UI_DEFAULT_TRANSPARENT_COLOR);
    }

    if (tb->items[i].text)
    {
        gui_set_font(tb->theme->focused_tab_text_font);
        gui_set_text_color(tb->theme->focused_tab_text_color);
        if (r2lMMIFlag)
        {
            gui_print_truncated_text(
                x1 + tb->tab_text_x + tb->tab_text_width - 1,
                y1 + tb->tab_text_y,
                tb->tab_text_width,
                tb->items[i].text);
        }
        else
        {
            gui_print_truncated_text(x1 + tb->tab_text_x, y1 + tb->tab_text_y, tb->tab_text_width, tb->items[i].text);
        }
    }

    tab_y_offset = y1 + (tb->focused_tab_height - tb->tab_height);

    if ((i == tb->first_displayed_item) && (i != 0))
    {
        gui_horizontal_tab_bar_show_left_arrow(tb, tab_y_offset, x1, x2);
    }

    if ((i == tb->last_displayed_item) && (i != tb->n_items - 1))
    {
        gui_horizontal_tab_bar_show_right_arrow(tb, tab_y_offset, x1, x2);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_blink_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_tab_bar_blink_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i;
    horizontal_tab_bar *tb = UI_current_horizontal_tab_bar;

#ifdef UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER
    BOOL redraw_focused_item = MMI_FALSE;
#endif 
    BOOL redraw_bar = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!tb)
    {
        return;
    }

    gui_lock_double_buffer();

    gui_push_clip();
    gui_push_text_clip();

    if (tb->flags & UI_TAB_BAR_BLINKING_DISPLAYED)
    {
        tb->flags &= ~UI_TAB_BAR_BLINKING_DISPLAYED;
    }
    else
    {
        tb->flags |= UI_TAB_BAR_BLINKING_DISPLAYED;
    }

    for (i = tb->first_displayed_item; i <= tb->last_displayed_item; i++)
    {
        if (tb->items[i].flags & UI_TAB_BAR_ITEM_STATE_BLINKING)
        {
        #ifdef UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER
            if ((i == tb->focused_item - 1) || (i == tb->focused_item + 1))
            {
                redraw_focused_item = TRUE;
            }
        #endif /* UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER */ 

            MMI_DBG_ASSERT(i != tb->focused_item);
            gui_horizontal_tab_bar_show_item(tb, i);
            redraw_bar = TRUE;
        }
    }

#ifdef UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER
    if (redraw_focused_item)
    {
        gui_horizontal_tab_bar_show_focused_item(tb);
    }
#endif /* UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER */ 

    gui_pop_text_clip();
    gui_pop_clip();
    gui_unlock_double_buffer();

    if (redraw_bar)
    {
        gui_BLT_double_buffer(tb->x, tb->y, tb->x + tb->width - 1, tb->y + tb->tab_area_height - 1);
    }
    gui_start_timer(UI_TAB_BAR_BLINKING_TIME, gui_horizontal_tab_bar_blink_hdlr);
}

/***************************************************************************** 
* Global Variable
*****************************************************************************/

UI_horizontal_tab_bar_theme *current_horizontal_tab_bar_theme = NULL;

/***************************************************************************** 
* Global Function
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_tab_bar_current_theme
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb          [?]             
 *  b(?)        [IN/OUT]        The tab bar control
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_tab_bar_current_theme(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tb->theme = current_horizontal_tab_bar_theme;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_tab_bar_theme
 * DESCRIPTION
 *  Apply the given theme to the horizontal tab bar
 * PARAMETERS
 *  tb      [IN/OUT]        The tab bar control
 *  t       [IN]            Theme (memory should not be released when the tab bar is still active)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_tab_bar_theme(horizontal_tab_bar *tb, UI_horizontal_tab_bar_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tb->theme = t;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_horizontal_tab_bar
 * DESCRIPTION
 *  Apply the given theme to the horizontal tab bar
 * PARAMETERS
 *  tb                      [IN/OUT]        The tab bar control
 *  x                       [IN]            
 *  y                       [IN]            
 *  width                   [IN]            
 *  tab_area_height         [IN]            
 *  hint_area_height        [IN]            
 *  tab_width               [IN]            
 *  focused_tab_width       [IN]            
 *  hint                    [IN]            
 *  n_item                  [IN]            
 *  focused_item            [IN]            
 *  items                   [?]             
 *  t(?)                    [IN]            Theme (memory should not be released when the tab bar is still active)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_horizontal_tab_bar(
        horizontal_tab_bar *tb,
        S32 x,
        S32 y,
        S16 width,
        S16 tab_area_height,
        S16 hint_area_height,
        S16 tab_width,
        S16 focused_tab_width,
        UI_string_type hint,
        S8 n_item,
        S8 focused_item,
        tab_bar_item_type *items)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 ih = 0, iw = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tb->x = x;
    tb->y = y;
    tb->width = width;
    tb->height = tab_area_height + hint_area_height;
    tb->flags = 0;

    if (focused_item >= n_item)
    {
        focused_item = 0;
    }

    /* mandatory */
    if (focused_tab_width < tab_width)
    {
        focused_tab_width = tab_width;
    }

    tb->hint = hint;

    tb->n_items = n_item;
    tb->focused_item = focused_item;
    tb->items = items;

    tb->first_displayed_item = 0;
    tb->last_displayed_item = 0;
    tb->n_displayable_item = 0;
    tb->n_item_ahead = 0;

    tb->tab_width = tab_width;
    tb->focused_tab_width = focused_tab_width;
    tb->tab_area_height = tab_area_height;
    tb->hint_area_height = hint_area_height;

    tb->left_button_pressed = 0;
    tb->right_button_pressed = 0;

    tb->tab_icon_x = tb->tab_icon_y = 0;
    tb->tab_text_x = tb->tab_text_y = 0;
    tb->tab_text_width = tb->tab_width;

    tb->theme = current_horizontal_tab_bar_theme;

    gui_set_horizontal_tab_bar_current_theme(tb);

    if (tb->theme->normal_tab_icon) /* use icon instead of filler */
    {
        gui_measure_image((PU8) GetImage(tb->theme->normal_tab_icon), &iw, &ih);
        tb->tab_height = (S16) ih;
        gui_measure_image((PU8) GetImage(tb->theme->focused_tab_icon), &iw, &ih);
        tb->focused_tab_height = (S16) ih;
    }
    else
    {
        tb->tab_height = tb->focused_tab_height = tab_area_height;
    }

    gui_horizontal_tab_bar_locate_focused_item(tb);

#ifdef __MMI_TOUCH_SCREEN__
    memset(&tb->pen_state, 0, sizeof(tb->pen_state));
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_horizontal_tab_bar
 * DESCRIPTION
 *  Change the size of a tab bar
 * PARAMETERS
 *  tb                      [IN/OUT]        The tab bar control
 *  width                   [IN]            New width
 *  tab_area_height         [IN]            
 *  hint_area_height        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_horizontal_tab_bar(horizontal_tab_bar *tb, S16 width, S16 tab_area_height, S16 hint_area_height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tb->width = width;
    tb->height = tab_area_height + hint_area_height;

    tb->tab_area_height = tab_area_height;
    tb->hint_area_height = hint_area_height;

    gui_horizontal_tab_bar_locate_focused_item(tb);
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_horizontal_tab_bar
 * DESCRIPTION
 *  Move the tab bar to different location
 * PARAMETERS
 *  tb      [IN/OUT]        The tab bar control
 *  x       [IN]            The new left-top corner
 *  y       [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_horizontal_tab_bar(horizontal_tab_bar *tb, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tb->x = x;
    tb->y = y;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_horizontal_tab_bar
 * DESCRIPTION
 *  Display the horizontal tab bar
 * PARAMETERS
 *  tb                      [IN/OUT]        The tab bar control
 *  disable_tab_area        [IN]            
 *  disable_hint_area       [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_horizontal_tab_bar(horizontal_tab_bar *tb, BOOL disable_tab_area, BOOL disable_hint_area)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i;
    S32 x1, y1, x2, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_push_text_clip();
    gui_set_clip(tb->x, tb->y, tb->x + tb->width - 1, tb->y + tb->height - 1);

    if (!disable_tab_area)
    {
        gui_draw_filled_area(
            tb->x,
            tb->y,
            tb->x + tb->width - 1,
            tb->y + tb->tab_area_height - 1,
            tb->theme->background_filler);

        for (i = tb->first_displayed_item; i <= tb->last_displayed_item; i++)
        {
            if (i != tb->focused_item)
            {
                gui_horizontal_tab_bar_show_item(tb, i);
            }
        }

        gui_horizontal_tab_bar_show_focused_item(tb);
    }

    if (!disable_hint_area && tb->hint_area_height)
    {
        S32 sw, sh;
        S32 sx, sy;
        S32 width;

        x1 = tb->x;
        y1 = tb->y + tb->tab_area_height;
        x2 = tb->x + tb->width - 1;
        y2 = y1 + tb->hint_area_height - 1;

        gui_set_clip(x1, y1, x2, y2);
        gui_set_text_clip(x1, y1, x2, y2);

        my_draw_image_filler(x1, y1, x2, y2, tb->theme->hint_bg_image, tb->theme->hint_filler);

        gui_set_font(tb->theme->hint_text_font);
        gui_measure_string(tb->hint, &sw, &sh);
        gui_set_text_color(tb->theme->hint_text_color);
        gui_set_text_border_color(tb->theme->hint_text_border_color);
        if (sh < tb->hint_area_height)
        {
            sy = y1 + ((tb->hint_area_height - sh) >> 1);
        }
        else
        {
            sy = y1;
        }

        if (!MMI_disable_title_shortcut_display && MMI_menu_shortcut_number >= 0)
        {
            width = tb->width - (MMI_menu_shortcut_box.width << 1);

            if (sw <= width)
            {
                sx = x1 + (tb->width >> 1) - (sw >> 1);
            }
            else
            {
                width = tb->width - MMI_menu_shortcut_box.width - tb->theme->hint_text_gap;

                if (width < sw)
                {
                    sx = x1 + tb->theme->hint_text_gap;
                    sw = width;
                }
                else
                {
                    sx = x1 + tb->theme->hint_text_gap + (width >> 1) - (sw >> 1);
                }
            }
        }
        else
        {
            width = tb->width - (tb->theme->hint_text_gap << 1);
            if (width < sw)
            {
                sx = x1 + tb->theme->hint_text_gap;
                sw = width;
            }
            else
            {
                sx = x1 + (tb->width >> 1) - (sw >> 1);
            }
        }

        if (tb->hint)
        {
            if (r2lMMIFlag)
            {
                gui_print_truncated_borderd_text(sx + sw - 1, sy, sw, tb->hint);
            }
            else
            {
                gui_print_truncated_borderd_text(sx, sy, sw, tb->hint);
            }
        }

        show_menu_shortcut();
    }

    gui_pop_text_clip();
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_set_icon_position
 * DESCRIPTION
 *  Set the icon position inside a tab item
 * PARAMETERS
 *  tb      [IN/OUT]        The tab bar control
 *  x       [IN]            The new left-top corner
 *  y       [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_set_icon_position(horizontal_tab_bar *tb, S16 x, S16 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tb->tab_icon_x = x;
    tb->tab_icon_y = y;
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_set_text_position
 * DESCRIPTION
 *  Set the text position inside a tab item
 * PARAMETERS
 *  tb          [IN/OUT]        The tab bar control
 *  x           [IN]            The new left-top corner
 *  y           [IN]            
 *  width       [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_set_text_position(horizontal_tab_bar *tb, S16 x, S16 y, S16 width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tb->tab_text_x = x;
    tb->tab_text_y = y;
    tb->tab_text_width = width;
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_set_blinking
 * DESCRIPTION
 *  Blinking a certain tab
 * PARAMETERS
 *  tb          [IN/OUT]        The tab bar control
 *  index       [IN]            The tab item index to blink
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_set_blinking(horizontal_tab_bar *tb, S8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index >= tb->n_items)
    {
        return;
    }

    tb->items[(U8)index].flags |= UI_TAB_BAR_ITEM_STATE_BLINKING;
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_unset_blinking
 * DESCRIPTION
 *  Stop blinking of a certain tab
 * PARAMETERS
 *  tb          [IN/OUT]        The tab bar control
 *  index       [IN]            The tab item index
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_unset_blinking(horizontal_tab_bar *tb, S8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index >= tb->n_items)
    {
        return;
    }

    tb->items[(U8)index].flags &= ~UI_TAB_BAR_ITEM_STATE_BLINKING;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_tab_bar_display_range
 * DESCRIPTION
 *  Set the display range of tab bar
 * PARAMETERS
 *  tb                          [IN/OUT]        The tab bar control
 *  first_displayed_item        [IN]            The first displayed tab item
 * RETURNS
 *  Wthether the display range is changed.
 *****************************************************************************/
BOOL gui_set_horizontal_tab_bar_display_range(horizontal_tab_bar *tb, S32 first_displayed_item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_displayed_item;
    S32 previous_value = tb->first_displayed_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (first_displayed_item < 0)
    {
        MMI_DBG_ASSERT(0);
        first_displayed_item = 0;
    }

    last_displayed_item = first_displayed_item + tb->n_displayable_item - 1;
    if (last_displayed_item >= tb->n_items)
    {
        last_displayed_item = tb->n_items - 1;
        first_displayed_item = last_displayed_item - tb->n_displayable_item + 1;
        if (first_displayed_item < 0)
        {
            first_displayed_item = 0;
        }
    }

    tb->first_displayed_item = (S8) first_displayed_item;
    tb->last_displayed_item = (S8) last_displayed_item;
    if (first_displayed_item == previous_value)
    {
        return MMI_FALSE;
    }
    else
    {
        return MMI_TRUE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_focus_item
 * DESCRIPTION
 *  Focus to a certain tab
 * PARAMETERS
 *  tb          [IN/OUT]        The tab bar control
 *  index       [IN]            The tab item index
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_focus_item(horizontal_tab_bar *tb, S8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index >= tb->n_items || index == tb->focused_item)
    {
        return;
    }

    tb->focused_item = index;
    gui_horizontal_tab_bar_locate_focused_item(tb);
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_focus_next_item
 * DESCRIPTION
 *  Focus to the next tab
 * PARAMETERS
 *  tb      [IN/OUT]        The tab bar control
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_focus_next_item(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tb->flags & UI_TAB_BAR_LOOP)
    {
        if (tb->focused_item == tb->n_items - 1)
        {
            tb->focused_item = 0;
            gui_horizontal_tab_bar_locate_focused_item(tb);
        }
        else
        {
            tb->focused_item++;
            gui_horizontal_tab_bar_locate_next_item(tb);
        }
    }
    else
    {
        if (tb->focused_item != tb->n_items - 1)
        {
            tb->focused_item++;
            gui_horizontal_tab_bar_locate_next_item(tb);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_focus_prev_item
 * DESCRIPTION
 *  Focus to the previous tab
 * PARAMETERS
 *  tb      [IN/OUT]        The tab bar control
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_focus_prev_item(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tb->flags & UI_TAB_BAR_LOOP)
    {
        if (tb->focused_item == 0)
        {
            tb->focused_item = tb->n_items - 1;
            gui_horizontal_tab_bar_locate_focused_item(tb);
        }
        else
        {
            tb->focused_item--;
            gui_horizontal_tab_bar_locate_prev_item(tb);
        }
    }
    else
    {
        if (tb->focused_item != 0)
        {
            tb->focused_item--;
            gui_horizontal_tab_bar_locate_prev_item(tb);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_focus_first_item
 * DESCRIPTION
 *  Focus to the first tab
 * PARAMETERS
 *  tb      [IN/OUT]        The tab bar control
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_focus_first_item(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_tab_bar_focus_item(tb, 0);
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_focus_last_item
 * DESCRIPTION
 *  Focus to the last tab
 * PARAMETERS
 *  tb      [IN/OUT]        The tab bar control
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_focus_last_item(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_tab_bar_focus_item(tb, (S8) (tb->n_items - 1));
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_is_item_displayed
 * DESCRIPTION
 *  Check if a tab bar item
 * PARAMETERS
 *  tb          [IN]        The tab bar control
 *  index       [IN]        Index of tab item
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL gui_horizontal_tab_bar_is_item_displayed(horizontal_tab_bar *tb, int index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index >= tb->first_displayed_item && index <= tb->last_displayed_item)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_start_blinking
 * DESCRIPTION
 *  Animate the blinking tabs
 * PARAMETERS
 *  tb      [IN]        The tab bar control
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_start_blinking(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_current_horizontal_tab_bar = tb;
    gui_start_timer(UI_TAB_BAR_BLINKING_TIME, gui_horizontal_tab_bar_blink_hdlr);
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_stop_blinking
 * DESCRIPTION
 *  Stop animation of the blinking tabs
 * PARAMETERS
 *  tb      [IN]        The tab bar control
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_stop_blinking(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(gui_horizontal_tab_bar_blink_hdlr);
    UI_current_horizontal_tab_bar = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  gui_setup_tab_item
 * DESCRIPTION
 *  Setup the data of a tab item
 * PARAMETERS
 *  item                [IN]        
 *  icon                [IN]        
 *  text                [IN]        
 *  enable_blinking     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_setup_tab_item(tab_bar_item_type *item, PU8 icon, UI_string_type text, MMI_BOOL enable_blinking)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->text = text;
    item->icon = icon;
    if (enable_blinking)
    {
        item->flags = UI_TAB_BAR_ITEM_STATE_BLINKING;
    }
    else
    {
        item->flags = UI_TAB_BAR_ITEM_STATE_NORMAL;
    }
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_redraw_left_button
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_redraw_left_button(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    if (tb->first_displayed_item == tb->focused_item)
    {
        gui_horizontal_tab_bar_show_focused_item(tb);
    }
    else
    {
        gui_horizontal_tab_bar_show_item(tb, tb->first_displayed_item);
    #ifdef UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER
        if (tb->focused_item == tb->first_displayed_item + 1)
        {
            /* We need to redisplay focused tab because it might overlap with other tabs */
            gui_horizontal_tab_bar_show_focused_item(tb);
        }
    #endif /* UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER */ 
    }
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(tb->x, tb->y, tb->x + tb->width - 1, tb->y + tb->tab_area_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_redraw_right_button
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_tab_bar_redraw_right_button(horizontal_tab_bar *tb)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    if (tb->last_displayed_item == tb->focused_item)
    {
        gui_horizontal_tab_bar_show_focused_item(tb);
    }
    else
    {
        gui_horizontal_tab_bar_show_item(tb, tb->last_displayed_item);
    #ifdef UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER
        if (tb->focused_item == tb->last_displayed_item - 1)
        {
            /* We need to redisplay focused tab because it might overlap with other tabs */
            gui_horizontal_tab_bar_show_focused_item(tb);
        }
    #endif /* UI_TAB_BAR_FOCUSED_ITEM_OVERLAP_OTHER */ 
    }
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(tb->x, tb->y, tb->x + tb->width - 1, tb->y + tb->tab_area_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_tab_bar_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tb                  [?]         
 *  pen_event           [IN]        
 *  x                   [IN]        
 *  y                   [IN]        
 *  tab_event           [?]         
 *  tab_event_param     [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gui_horizontal_tab_bar_translate_pen_event(
        horizontal_tab_bar *tb,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_tab_bar_pen_enum *tab_event,
        gui_pen_event_param_struct *tab_event_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    S32 in_left_button = 0, in_right_button = 0;
    S32 item_index = -1;
    gui_tab_bar_pen_state_struct *pen_state;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (tb->flags & UI_TAB_BAR_DISABLE_PEN)
    {
        return MMI_FALSE;
    }

    pen_state = &tb->pen_state;
    ret = MMI_TRUE;
    *tab_event = GUI_TAB_BAR_PEN_NONE;
    GUI_PEN_EVENT_PARAM_SET_VOID(tab_event_param);

    if (tb->first_displayed_item != 0 &&
        PEN_CHECK_BOUND(
            x,
            y,
            pen_state->left_button_x,
            pen_state->left_button_y,
            pen_state->left_button_width,
            pen_state->left_button_height))
    {
        in_left_button = 1;
    }
    else if (tb->last_displayed_item != (tb->n_items - 1) &&
             PEN_CHECK_BOUND(
                x,
                y,
                pen_state->right_button_x,
                pen_state->right_button_y,
                pen_state->right_button_width,
                pen_state->right_button_height))
    {
        in_right_button = 1;
    }
    else if (PEN_CHECK_BOUND(x, y, tb->x, tb->y, tb->width, tb->tab_area_height))
    {
        item_index = tb->first_displayed_item + ((x - tb->x) / tb->tab_width);
    }

    switch (pen_event)
    {
        case MMI_PEN_EVENT_DOWN:
            if (in_left_button)
            {
                pen_state->pen_down_left_button = 1;
                pen_state->pen_down_right_button = 0;
                pen_state->pen_down_tab_item_index = -1;

                tb->left_button_pressed = 1;
                gui_horizontal_tab_bar_redraw_left_button(tb);
            }
            else if (in_right_button)
            {
                pen_state->pen_down_left_button = 0;
                pen_state->pen_down_right_button = 1;
                pen_state->pen_down_tab_item_index = -1;

                tb->right_button_pressed = 1;
                gui_horizontal_tab_bar_redraw_right_button(tb);
            }
            else if (item_index >= 0)
            {
                pen_state->pen_down_left_button = 0;
                pen_state->pen_down_right_button = 0;
                pen_state->pen_down_tab_item_index = item_index;
                pen_state->focus_changed = 0;
            }
            else
            {
                ret = MMI_FALSE;
            }

            break;

        case MMI_PEN_EVENT_UP:
            if (pen_state->pen_down_left_button)
            {
                if (in_left_button)
                {
                    gui_set_horizontal_tab_bar_display_range(tb, (S32) tb->first_displayed_item - 1);
                    *tab_event = GUI_TAB_BAR_PEN_NEED_REDRAW;
                }
            }
            else if (pen_state->pen_down_right_button)
            {
                if (in_right_button)
                {
                    gui_set_horizontal_tab_bar_display_range(tb, (S32) tb->first_displayed_item + 1);
                    *tab_event = GUI_TAB_BAR_PEN_NEED_REDRAW;
                }
            }
            else if (pen_state->pen_down_tab_item_index >= 0)
            {
                /* Only if pen does not move outside the tab item between Pen Down and Pen Up */
                if (item_index == pen_state->pen_down_tab_item_index &&
                    !pen_state->focus_changed && item_index != tb->focused_item)
                {
                    *tab_event = GUI_TAB_BAR_PEN_JUMP_TO_I;
                    GUI_PEN_EVENT_PARAM_SET_INTEGER(tab_event_param, ((int)item_index));
                }
            }

            if (tb->left_button_pressed)
            {
                tb->left_button_pressed = 0;
                gui_horizontal_tab_bar_redraw_left_button(tb);
            }
            else if (tb->right_button_pressed)
            {
                tb->right_button_pressed = 0;
                gui_horizontal_tab_bar_redraw_right_button(tb);
            }
            break;

        case MMI_PEN_EVENT_MOVE:
            if (pen_state->pen_down_left_button)
            {
                if (in_left_button)
                {
                    if (!tb->left_button_pressed)
                    {
                        tb->left_button_pressed = 1;
                        gui_horizontal_tab_bar_redraw_left_button(tb);
                    }
                }
                else
                {
                    if (tb->left_button_pressed)
                    {
                        tb->left_button_pressed = 0;
                        gui_horizontal_tab_bar_redraw_left_button(tb);
                    }
                }
            }
            else if (pen_state->pen_down_right_button)
            {
                if (in_right_button)
                {
                    if (!tb->right_button_pressed)
                    {
                        tb->right_button_pressed = 1;
                        gui_horizontal_tab_bar_redraw_right_button(tb);
                    }
                }
                else
                {
                    if (tb->right_button_pressed)
                    {
                        tb->right_button_pressed = 0;
                        gui_horizontal_tab_bar_redraw_right_button(tb);
                    }
                }
            }
            else if (pen_state->pen_down_tab_item_index >= 0)
            {
                if (item_index != tb->focused_item)
                {
                    pen_state->focus_changed = 1;
                }
            }
            break;

        case MMI_PEN_EVENT_LONG_TAP:
            /* FALLTHROUGH no break */
        case MMI_PEN_EVENT_REPEAT:
            break;

        case MMI_PEN_EVENT_ABORT:
            if (tb->left_button_pressed)
            {
                tb->left_button_pressed = 0;
                gui_horizontal_tab_bar_redraw_left_button(tb);
            }
            else if (tb->right_button_pressed)
            {
                tb->right_button_pressed = 0;
                gui_horizontal_tab_bar_redraw_right_button(tb);
            }
            break;

        default:
            MMI_ASSERT(0);
    }

    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 
#endif /* __MMI_UI_TAB_PANE__*/

