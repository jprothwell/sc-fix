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
 * gui_dynamic_menus.c
 * Project:
 * --------
 *   PlutoMMI
 * Description:
 * ------------
 *   In order to use limited memory (size decided in compile-time) to load unlimited items (size decide in run-time) 
 *   to a category screen, a modification in MMI for dynamic item loading is required as the following.
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

#include "gui_asyncdynamic_menus.h"
#include "wgui_asyncdynamic_menuitems.h"
#include "debuginitdef.h"
#include "wgui_categories_util.h"

/* Variable which will be set in case of data loading failed from application */
S32 gui_asyncdynamic_list_error = 0;

extern BOOL r2lMMIFlag;

#ifdef __MMI_TOUCH_SCREEN__
static void gui_asyncdynamic_list_abort_scroll_timer(void);
#endif 


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_locate_highlighted_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_asyncdynamic_list_menu_locate_highlighted_item(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* FIXME. handle the case that m->height < iheight */
    S32 iwidth, iheight;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(m->flags & UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM) &&
        !(m->flags & UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM))
    {
        /* 
         * m->first_dispalyed_item & m->last_displayed_item is set properly in asyncdynamic_list_goto_item().
         *
         * However, the value of m->first_dispalyed_item & m->last_displayed_item is undefined 
         * on entering a new menu (asyncdynamic_list_goto_item_no_redraw()) without list menu history.
         *
         */
        if (m->first_displayed_item >= m->n_items || m->last_displayed_item >= m->n_items)
        {
            m->first_displayed_item = 0;
            m->last_displayed_item = 0;
        }

        if (m->highlighted_item < m->first_displayed_item)
        {
            m->flags |= UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        }
        else if (m->highlighted_item > m->last_displayed_item)
        {
            m->flags |= UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
        }
    }

    if (m->flags & UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM)
    {
        S32 total_height = 0, i;
        U8 done = 0;
        S32 list_height = m->height;

        m->flags &= ~UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        m->first_displayed_item = m->highlighted_item;

        /* Check if in the last page */
        for (i = m->n_items - 1; (i >= 0) && (!done); i--)
        {
            /* Use NULL to measure menu item height because the data of menu item might not be loaded yet */
             
             
        #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
             
            if (i == m->highlighted_item)
            {
                current_fixed_list_menuitem_display_index = -1;
            }
            else
            {
                current_fixed_list_menuitem_display_index = i;
            }
        #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
             
            m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
             
            /* if( total_height > list_height + 1 ) */
            if (total_height > list_height)
                 
            {
                if (m->first_displayed_item > i)
                {
                    done = 1;
                    m->first_displayed_item = i + 1;
                    m->last_displayed_item = m->n_items - 1;
                }
                break;
            }
        }

        /* Check if in the last page */
        if (!done && total_height <= list_height)
        {
            done = 1;
            m->first_displayed_item = 0;
            m->last_displayed_item = m->n_items - 1;
        }

        /* Align to the top */
        total_height = 0;
        if (!done)
        {
            for (i = m->first_displayed_item; (i < m->n_items) && (!done); i++)
            {
                 
                 
            #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
                 
                if (i == m->highlighted_item)
                {
                    current_fixed_list_menuitem_display_index = -1;
                }
                else
                {
                    current_fixed_list_menuitem_display_index = i;
                }
            #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
                 
                m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);
                total_height += iheight;
                 
                /* if( total_height > list_height + 1 ) */
                if (total_height > list_height)
                     
                {
                    done = 1;
                    m->last_displayed_item = i - 1;
                }
            }
            if (!done)
            {
                m->last_displayed_item = m->n_items - 1;
            }
        }
    }
    else if (m->flags & UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM)
    {
        S32 total_height = 0, i;
        U8 done = 0;
        S32 list_height = m->height;

        m->flags &= ~UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
        m->last_displayed_item = m->highlighted_item;

        /* Check if in the first page */
        for (i = 0; (i < m->n_items) && !done; i++)
        {
             
             
        #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
             
            if (i == m->highlighted_item)
            {
                current_fixed_list_menuitem_display_index = -1;
            }
            else
            {
                current_fixed_list_menuitem_display_index = i;
            }
        #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
             
            /* Use NULL to measure menu item height because the data of menu item might not be loaded yet */
            m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
             
            /* if( total_height > list_height + 1 ) */
            if (total_height > list_height)
                 
            {
                if (m->last_displayed_item < i)
                {
                    done = 1;
                    m->first_displayed_item = 0;
                    m->last_displayed_item = i - 1;
                }
                break;
            }
        }

        /* Check if in the last page */
        if (!done && total_height <= list_height)
        {
            done = 1;
            m->first_displayed_item = 0;
            m->last_displayed_item = m->n_items - 1;
        }

        /* Align to the bottom */
        total_height = 0;
        if (!done)
        {
            for (i = m->last_displayed_item; (i >= 0) && (!done); i--)
            {
                 
                 
            #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
                 
                if (i == m->highlighted_item)
                {
                    current_fixed_list_menuitem_display_index = -1;
                }
                else
                {
                    current_fixed_list_menuitem_display_index = i;
                }
            #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
                 
                m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);
                total_height += iheight;
                 
                /* if( total_height > list_height + 1 ) */
                if (total_height > list_height)
                     
                {
                    done = 1;
                    m->first_displayed_item = i + 1;
                }
            }
            if (!done)
            {
                m->first_displayed_item = 0;
            }
        }
    }
    else
    {
        /* 
         * Condition: m->first_displayed_item <= m->highlighted_item <= m->last_displayed_item 
         * Do not change m->first_displayed_item if no problem occurs
         */
        U8 done = 0;
        S32 total_height = 0, i;
        S32 list_height = m->height;

        /* Re-compute last_displayed_item */
        for (i = m->first_displayed_item; i < m->n_items && (!done); i++)
        {
             
             
        #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
             
            if (i == m->highlighted_item)
            {
                current_fixed_list_menuitem_display_index = -1;
            }
            else
            {
                current_fixed_list_menuitem_display_index = i;
            }
        #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
             
            m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);

            total_height += iheight;
             
            /* if( total_height > list_height + 1 ) */
            if (total_height > list_height)
                 
            {
                done = 1;
                m->last_displayed_item = i - 1;
            }
        }

        /* Check if in the last page */
        if (total_height < list_height)
        {
            total_height = 0;
            for (i = m->n_items - 1; (i >= 0) && (!done); i--)
            {
                 
                 
            #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
                 
                if (i == m->highlighted_item)
                {
                    current_fixed_list_menuitem_display_index = -1;
                }
                else
                {
                    current_fixed_list_menuitem_display_index = i;
                }
            #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
                 
                m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);

                total_height += iheight;
                 
                /* if( total_height > list_height + 1 ) */
                if (total_height > list_height)
                     
                {
                    if (m->first_displayed_item > i)
                    {
                        done = 1;
                        m->first_displayed_item = i + 1;
                        m->last_displayed_item = m->n_items - 1;
                    }
                    break;
                }
            }
            if (total_height <= list_height)
            {
                m->first_displayed_item = 0;
                m->last_displayed_item = m->n_items - 1;
            }
        }

        MMI_DBG_ASSERT(m->first_displayed_item <= m->highlighted_item);
        if (m->highlighted_item > m->last_displayed_item)
        {
            done = 0;
            total_height = 0;
            m->last_displayed_item = m->highlighted_item;
            for (i = m->last_displayed_item; i >= 0 && (!done); i--)
            {
                 
                 
            #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
                 
                if (i == m->highlighted_item)
                {
                    current_fixed_list_menuitem_display_index = -1;
                }
                else
                {
                    current_fixed_list_menuitem_display_index = i;
                }
            #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
                 
                m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);

                total_height += iheight;
                 
                /* if( total_height > list_height + 1 ) */
                if (total_height > list_height)
                     
                {
                    done = 1;
                    m->first_displayed_item = i + 1;
                }
            }
        }
    }

    /* Load one more item because it might be accessed in gui_show_asyncdynamic_list_menu() */
    if (!load_chunk_asyncdynamic_item_buffer
        (m->first_displayed_item, m->last_displayed_item - m->first_displayed_item + 2))
    {
        gui_asyncdynamic_list_error = 1;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_locate_previous_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_asyncdynamic_list_menu_locate_previous_item(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_item < m->first_displayed_item)
    {
        m->first_displayed_item = m->highlighted_item;
        if (!load_chunk_asyncdynamic_item_buffer(m->highlighted_item, 1))
        {
            gui_asyncdynamic_list_error = 1;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_locate_next_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_asyncdynamic_list_menu_locate_next_item(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->highlighted_item > m->last_displayed_item)
    {
        S32 total_height = 0, i;

        /* S32 list_height = m->height-4; */
        S32 list_height = m->height;     

        m->last_displayed_item = m->highlighted_item;
        for (i = m->last_displayed_item; i >= 0; i--)
        {
             
             
        #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
             
            if (i == m->highlighted_item)
            {
                current_fixed_list_menuitem_display_index = -1;
            }
            else
            {
                current_fixed_list_menuitem_display_index = i;
            }
        #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
             
            m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);

            total_height += iheight;
             
            /* if( total_height > list_height + 1 ) */
            if (total_height > list_height)
                 
            {
                m->first_displayed_item = i + 1;
                break;
            }
        }
        /* Load one more item because it might be accessed in gui_show_asyncdynamic_list_menu() */
        if (!load_chunk_asyncdynamic_item_buffer(m->highlighted_item, 2))
        {
            gui_asyncdynamic_list_error = 1;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  switch_asynchighlighted_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m                           [?]         
 *  last_highlighted_item       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void switch_asynchighlighted_item(fixed_list_menu *m, S32 last_highlighted_item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (last_highlighted_item != m->highlighted_item)
    {
        if ((last_highlighted_item >= 0) && (last_highlighted_item < m->n_items))
        {
            if (in_asyncdynamic_item_buffer(last_highlighted_item))
            {
                m->item_remove_highlight_function(
                    get_asyncdynamic_item_from_buffer(last_highlighted_item),
                    m->common_item_data);
            }
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(get_asyncdynamic_item_from_buffer(m->highlighted_item), m->common_item_data);
        }
        m->item_unhighlighted(last_highlighted_item);
    #ifdef __MMI_TOUCH_SCREEN__
        m->pen_event_current_selected_callback_function = m->pen_event_default_selected_callback_function;
    #endif 
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_goto_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]         
 *  i       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_asyncdynamic_list_menu_goto_item(fixed_list_menu *m, S32 i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_TOUCH_SCREEN__
    /* If highlight is changed by keypad, abort scroll timer */
    gui_asyncdynamic_list_abort_scroll_timer();
#endif /* __MMI_TOUCH_SCREEN__ */ 

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

    gui_asyncdynamic_list_menu_locate_highlighted_item(m);

    /* Return In Case Of Error */
    if (gui_asyncdynamic_list_error)
    {
        return;
    }

    switch_asynchighlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_goto_next_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_asyncdynamic_list_menu_goto_next_item(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;
    U8 loop_flag = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_TOUCH_SCREEN__
    /* If highlight is changed by keypad, abort scroll timer */
    gui_asyncdynamic_list_abort_scroll_timer();
#endif /* __MMI_TOUCH_SCREEN__ */ 

    if (m->highlighted_item >= m->n_items - 1)
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
        gui_asyncdynamic_list_menu_locate_highlighted_item(m);
    }
    else
    {
        m->highlighted_item++;
        gui_asyncdynamic_list_menu_locate_next_item(m);
    }
    /* Return In Case Of Error */
    if (gui_asyncdynamic_list_error)
    {
        return;
    }

    switch_asynchighlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_goto_previous_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_asyncdynamic_list_menu_goto_previous_item(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;
    U8 loop_flag = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_TOUCH_SCREEN__
    /* If highlight is changed by keypad, abort scroll timer */
    gui_asyncdynamic_list_abort_scroll_timer();
#endif /* __MMI_TOUCH_SCREEN__ */ 

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
        gui_asyncdynamic_list_menu_locate_highlighted_item(m);
    }
    else
    {
        m->highlighted_item--;
        gui_asyncdynamic_list_menu_locate_previous_item(m);
    }

    /* Return In Case Of Error */
    if (gui_asyncdynamic_list_error)
    {
        return;
    }

    switch_asynchighlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_asyncdynamic_list_menu
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_asyncdynamic_list_menu(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, y_offset;
    UI_filled_area *f;
    S32 i;
    S32 iwidth, iheight;
    fixed_icontext_list_menuitem_type *item_p;
    U8 done = 0;
    S32 total_height, counter, list_height;
    U8 disable_draw = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gui_asyncdynamic_list_error)
    {
        /* Context is invalid, and screen is exiting */
        MMI_DBG_ASSERT(0);
        return;
    }

    if (m->flags & UI_LIST_MENU_DISABLE_DRAW)
    {
        disable_draw = 1;
    }

    gui_push_clip();
    gui_push_text_clip();

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
    gui_set_clip(x1, y1, x2, y2);

    if (!disable_draw && !(m->flags & UI_LIST_MENU_DISABLE_BACKGROUND))
    {
        if (!(m->flags & UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER) && (wgui_is_wallpaper_on_bottom() == MMI_TRUE))
        {
            gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
        #if defined (__MMI_UI_TRANSPARENT_EFFECT__) || defined (__MMI_UI_LIST_HIGHLIGHT_EFFECTS__)
            if (gui_get_transparent_source_layer() == GDI_LAYER_EMPTY_HANDLE)   /* 110705 WAP menu Clavin add */
            {
                gui_set_transparent_source_layer(dm_get_scr_bg_layer());        /* should remove */
            }
        #endif /* defined (__MMI_UI_TRANSPARENT_EFFECT__) || defined (__MMI_UI_LIST_HIGHLIGHT_EFFECTS__) */ 
        }
    #ifdef __MMI_UI_LIST_CACHE_BACKGROUND__
        else if (!(m->flags & UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER) && (gui_get_multilayer_mask() & UI_MUL_BKG_SUBMENU))
        {
            gui_show_menu_background_in_layer(x1, y1, x2, y2, f);
        }
    #endif /* __MMI_UI_LIST_CACHE_BACKGROUND__ */ 
        else
        {
        #if !defined(MT6205B) && !defined(MT6208)
            gui_draw_filled_area(x1, y1, x2, y2, f);
        #else  
            /* For low-end phone, disable list menu background can greately improve menu performance. 
               We use the color of filler to draw the background (typically white). */
            color_t c = f->c;
            gdi_draw_solid_rect(x1, y1, x2, y2, gdi_act_color_from_rgb(255, c.r, c.g, c.b));
        #endif  
        }
    }

    if (m->n_items <= 0)
    {
        gui_pop_clip();
        gui_pop_text_clip();
        return;
    }

    if (!(m->flags & UI_LIST_MENU_DISABLE_SCROLLBAR))
    {
        if (r2lMMIFlag)
        {
            x1 += m->vbar.width;
        }
        else
        {
            x2 -= m->vbar.width;
        }
    }

    /* Modified to separate title bar and list menu */
    y1 = m->y + m->top;
    list_height = m->height;
    gui_set_text_clip(x1, y1, x2, y2);
    gui_set_clip(x1, y1, x2, y2);

    total_height = 0;
    counter = 0;
    current_fixed_list_menuitem_display_index = -1;
    for (i = m->first_displayed_item; (i < m->n_items && !done); i++)
    {
        item_p = get_asyncdynamic_item_from_buffer(i);
        current_fixed_list_menuitem_display_index = (i == m->highlighted_item) ? -1 : i;
        m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);

        y_offset = total_height;
        total_height += iheight;
         
        /* if( total_height > list_height + 1 ) */
        if (total_height > list_height)
             
        {
            done = 1;
            if ((counter == 0) && !disable_draw)
            {
                m->item_display_function(item_p, m->common_item_data, x1, y_offset + y1);
            }
        }
        else
        {
            if (!disable_draw)
            {
                m->item_display_function(item_p, m->common_item_data, x1, y_offset + y1);
            }
            counter++;
        }
    }
    current_fixed_list_menuitem_display_index = -1;
    if (counter == 0)
    {
        counter = 1;
    }
    m->last_displayed_item = m->first_displayed_item + counter - 1;
    m->displayed_items = counter;

    if (!disable_draw && !(m->flags & UI_LIST_MENU_DISABLE_SCROLLBAR))
    {
        S32 vbar_x = 0, vbar_button_x = 0;

        gui_set_vertical_scrollbar_range(&m->vbar, m->n_items);
        gui_set_vertical_scrollbar_scale(&m->vbar, m->displayed_items);
        gui_set_vertical_scrollbar_value(&m->vbar, m->first_displayed_item);
        gui_set_clip(m->x, m->y, m->x + m->width - 1, m->y + m->height - 1);

        if (m->flags & UI_LIST_MENU_AUTO_DISABLE_SCROLLBAR)
        {
            if (m->displayed_items < m->n_items)
            {
                if (r2lMMIFlag)
                {
                    vbar_x = m->vbar.x;
                    vbar_button_x = m->vbar.scroll_button.x;
                    m->vbar.x = m->vbar.x + m->vbar.width - m->width;
                    m->vbar.scroll_button.x = m->vbar.scroll_button.x + m->vbar.scroll_button.width - m->width;
                    gui_show_vertical_scrollbar(&m->vbar);
                    m->vbar.x = vbar_x;
                    m->vbar.scroll_button.x = vbar_button_x;
                }
                else
                {
                    gui_show_vertical_scrollbar(&m->vbar);
                }
            }
        }
        else
        {
            if (r2lMMIFlag)
            {
                vbar_x = m->vbar.x;
                vbar_button_x = m->vbar.scroll_button.x;
                m->vbar.x = m->vbar.x + m->vbar.width - m->width;
                m->vbar.scroll_button.x = m->vbar.scroll_button.x + m->vbar.scroll_button.width - m->width;
                gui_show_vertical_scrollbar(&m->vbar);
                m->vbar.x = vbar_x;
                m->vbar.scroll_button.x = vbar_button_x;
            }
            else
            {
                gui_show_vertical_scrollbar(&m->vbar);
            }
        }

    }

    gui_pop_clip();
    gui_pop_text_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_asyncdynamic_list_menu_no_draw
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_asyncdynamic_list_menu_no_draw(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    S32 iwidth, iheight;
    U8 done = 0;
    S32 total_height, counter, list_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    list_height = m->height;
    total_height = 0;
    counter = 0;
    current_fixed_list_menuitem_display_index = -1;
    for (i = m->first_displayed_item; (i < m->n_items && !done); i++)
    {
        current_fixed_list_menuitem_display_index = (i == m->highlighted_item) ? -1 : i;

        m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);

        total_height += iheight;
        if (total_height > list_height + 1)
        {
            done = 1;
        }
        else
        {
            counter++;
        }
    }
    current_fixed_list_menuitem_display_index = -1;
    m->last_displayed_item = (counter == 0) ? m->first_displayed_item : m->first_displayed_item + counter - 1;
    m->displayed_items = counter;
    if (!(m->flags & UI_LIST_MENU_DISABLE_SCROLLBAR))
    {
        gui_set_vertical_scrollbar_range(&m->vbar, m->n_items);
        gui_set_vertical_scrollbar_scale(&m->vbar, m->displayed_items);
        gui_set_vertical_scrollbar_value(&m->vbar, m->first_displayed_item);
    }
}

#ifdef __MMI_TOUCH_SCREEN__

static void gui_asyncdynamic_list_menu_scroll_by_pen(
                fixed_list_menu *m,
                S32 first_displayed,
                gui_list_pen_enum *menu_event);

/* Target menu for scroll timer */
static fixed_list_menu *gui_pen_scroll_asyncdynamic_menu = NULL;

/* Because loading data takes some time (depending on the applications using dynamic list), 
   we use a timer to reduce gui_asyncdynamic_list_menu_scroll_by_pen().  */


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_scroll_timer_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_asyncdynamic_list_menu_scroll_timer_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_list_menu *m = gui_pen_scroll_asyncdynamic_menu;
    gui_list_pen_enum menu_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_pen_scroll_asyncdynamic_menu = NULL;

    if (!m)
    {
        return;
    }

    if (gui_asyncdynamic_list_error)
    {
        return;
    }

    gui_asyncdynamic_list_menu_scroll_by_pen(m, m->pen_scroll_after_delay, &menu_event);

    if (gui_asyncdynamic_list_error)
    {
        return;
    }

    if (menu_event == GUI_LIST_PEN_HIGHLIGHT_CHANGED || menu_event == GUI_LIST_PEN_NEED_REDRAW)
    {
        if (m->pen_redraw_menu_function)
        {
            m->pen_redraw_menu_function();
        }
        else
        {
            MMI_DBG_ASSERT(0);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_abort_scroll_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_asyncdynamic_list_abort_scroll_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_pen_scroll_asyncdynamic_menu = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_start_scroll_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m           [?]         
 *  value       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_asyncdynamic_list_start_scroll_timer(fixed_list_menu *m, S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* assert that no two menu scroll at the same time */
    MMI_DBG_ASSERT(!gui_pen_scroll_asyncdynamic_menu || gui_pen_scroll_asyncdynamic_menu == m);

    m->pen_scroll_after_delay = value;
    gui_pen_scroll_asyncdynamic_menu = m;

    gui_cancel_timer(gui_asyncdynamic_list_menu_scroll_timer_hdlr);
    gui_start_timer(m->pen_scroll_delay_time, gui_asyncdynamic_list_menu_scroll_timer_hdlr);

    gui_add_cleanup_hook(gui_asyncdynamic_list_abort_scroll_timer);
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_translate_pen_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m               [?]         
 *  y               [IN]        
 *  item_index      [?]         
 * RETURNS
 *  
 *****************************************************************************/
static BOOL gui_asyncdynamic_list_menu_translate_pen_position(fixed_list_menu *m, S32 y, S32 *item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 total_height = 0, i;
    S32 iwidth, iheight;
    BOOL ret = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    y -= m->y;

    if (y < 0)
    {
        *item_index = (m->first_displayed_item > 0) ? m->first_displayed_item - 1 : 0;
    }
    else if (y >= m->height)
    {
        *item_index = (m->last_displayed_item < m->n_items - 1) ? m->last_displayed_item + 1 : m->n_items - 1;
    }
    else
    {
        *item_index = m->n_items - 1;
        for (i = m->first_displayed_item; i < m->n_items; i++)
        {
        #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
            if (i == m->highlighted_item)
            {
                current_fixed_list_menuitem_display_index = -1;
            }
            else
            {
                current_fixed_list_menuitem_display_index = i;
            }
        #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
            /* Use null because menu item might not be loaded yet */
            m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > y)
            {
                *item_index = i;
                ret = MMI_TRUE;
                break;
            }
        }
    }
    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_scroll_by_pen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m                   [?]         
 *  first_displayed     [IN]        
 *  menu_event          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_asyncdynamic_list_menu_scroll_by_pen(
                fixed_list_menu *m,
                S32 first_displayed,
                gui_list_pen_enum *menu_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 total_height = 0, i;
    S32 iwidth, iheight;
    S32 last_displayed;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (first_displayed > m->highlighted_item)
    {
        m->flags |= UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        gui_asyncdynamic_list_menu_goto_item(m, first_displayed);
        m->flags &= ~UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
    }
    else
    {
        last_displayed = m->n_items - 1;
        for (i = first_displayed; i < m->n_items; i++)
        {
        #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
            if (i == m->highlighted_item)
            {
                current_fixed_list_menuitem_display_index = -1;
            }
            else
            {
                current_fixed_list_menuitem_display_index = i;
            }
        #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
            /* Use null because menu item might not be loaded yet */
            m->item_measure_function(NULL, m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > m->height + 1)
            {
                last_displayed = i - 1;
                break;
            }
        }

        if (last_displayed < m->highlighted_item)
        {
            m->flags |= UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
            gui_asyncdynamic_list_menu_goto_item(m, last_displayed);
            m->flags &= ~UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
            *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
        }
        else
        {
            m->first_displayed_item = first_displayed;
            m->last_displayed_item = last_displayed;
            /* Handle this in similar way to asyncdynamic_list_goto_item_no_redraw */
            if (load_chunk_asyncdynamic_item_buffer
                (m->first_displayed_item, m->last_displayed_item - m->first_displayed_item + 2))
            {
                switch_asynchighlighted_item(m, -1);
            }
            else
            {
                gui_asyncdynamic_list_error = 1;
            }

            *menu_event = GUI_LIST_PEN_NEED_REDRAW;
        }
    }

    if (gui_asyncdynamic_list_error)
    {
        *menu_event = GUI_LIST_PEN_NONE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_asyncdynamic_list_menu_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m               [?]         
 *  pen_event       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  menu_event      [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gui_asyncdynamic_list_menu_translate_pen_event(
        fixed_list_menu *m,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_list_pen_enum *menu_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    gui_list_pen_state_struct *pen_state;
    gui_scrollbar_pen_enum scrollbar_event;
    gui_pen_event_param_struct scrollbar_param;
    S32 item_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pen_state = &m->pen_state;
    ret = MMI_TRUE;

    *menu_event = GUI_LIST_PEN_NONE;

    if (gui_asyncdynamic_list_error)
    {
        return MMI_FALSE;
    }

    if (pen_event != MMI_PEN_EVENT_DOWN && pen_state->pen_on_scrollbar)
    {
        gui_vertical_scrollbar_translate_pen_event(&m->vbar, pen_event, x, y, &scrollbar_event, &scrollbar_param);
        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
        {
            if (m->pen_scroll_delay_time > 0)   /* Delay time for scrollbar scrolling */
            {
                gui_asyncdynamic_list_start_scroll_timer(m, scrollbar_param._u.i);
            }
            else
            {
                gui_asyncdynamic_list_menu_scroll_by_pen(m, scrollbar_param._u.i, menu_event);
            }
        }
    }
    else
    {
        switch (pen_event)
        {
            case MMI_PEN_EVENT_DOWN:
                if (m->n_items <= 0)
                {
                    /* In current design, scrollbar is hiden if m->n_items == 0 */
                    ret = MMI_FALSE;
                }
                else if (PEN_CHECK_BOUND(x, y, m->x, m->y, m->width, m->height))
                {
                    pen_state->pen_on_scrollbar = 0;

                    if (!(m->flags & UI_LIST_MENU_DISABLE_SCROLLBAR) &&
                        gui_vertical_scrollbar_translate_pen_event(
                            &m->vbar,
                            MMI_PEN_EVENT_DOWN,
                            x,
                            y,
                            &scrollbar_event,
                            &scrollbar_param))
                    {
                        pen_state->pen_on_scrollbar = 1;
                        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
                        {
                            if (m->pen_scroll_delay_time > 0)   /* Delay time for scrollbar scrolling */
                            {
                                gui_asyncdynamic_list_start_scroll_timer(m, scrollbar_param._u.i);
                            }
                            else
                            {
                                gui_asyncdynamic_list_menu_scroll_by_pen(m, scrollbar_param._u.i, menu_event);
                            }
                        }
                    }
                    else
                    {
                        if (gui_asyncdynamic_list_menu_translate_pen_position(m, y, &item_index))
                        {
                            if (item_index != m->highlighted_item)
                            {
                                gui_asyncdynamic_list_menu_goto_item(m, item_index);
                                *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
                            }
                        }
                        else
                        {
                            /* # of menu items < # of displayable menu items */
                            ret = MMI_FALSE;
                        }
                    }
                    pen_state->first_highlighed_item = m->highlighted_item;
                    pen_state->highlight_changed = 0;
                }
                else
                {
                    ret = MMI_FALSE;
                }
                break;

            case MMI_PEN_EVENT_LONG_TAP:
                /* FALLTHROUGH no break */
            case MMI_PEN_EVENT_REPEAT:
                /* FALLTHROUGH no break */
            case MMI_PEN_EVENT_MOVE:
                gui_asyncdynamic_list_menu_translate_pen_position(m, y, &item_index);
                if (item_index != m->highlighted_item)
                {
                    gui_asyncdynamic_list_menu_goto_item(m, item_index);
                    *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
                }
                break;

            case MMI_PEN_EVENT_UP:
                if (PEN_CHECK_BOUND(x, y, m->x, m->y, m->width, m->height))
                {
                    gui_asyncdynamic_list_menu_translate_pen_position(m, y, &item_index);
                    if (item_index != m->highlighted_item)
                    {
                        /* If MMI_PEN_EVENT_MOVE is not delivered before moving to this item */
                        gui_asyncdynamic_list_menu_goto_item(m, item_index);
                        *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
                    }
                    else if (pen_state->highlight_changed)
                    {
                        *menu_event = GUI_LIST_PEN_NONE;
                    }
                    else
                    {
                        *menu_event = GUI_LIST_PEN_ITEM_SELECTED;
                    }
                }
                break;

            case MMI_PEN_EVENT_ABORT:
                /* Do nothing */
                break;

            default:
                MMI_ASSERT(0);
        }
    }

    if (gui_asyncdynamic_list_error)
    {
        *menu_event = GUI_LIST_PEN_NONE;
        return MMI_FALSE;
    }

    if (ret)
    {
        if (pen_state->first_highlighed_item != m->highlighted_item)
        {
            pen_state->highlight_changed = 1;
        }
    }
    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 

