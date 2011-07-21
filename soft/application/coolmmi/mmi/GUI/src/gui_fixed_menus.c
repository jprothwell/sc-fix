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
 *   gui_fixed_menus.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Fixed-list menu - UI component
 *   1. The number of menu items stays unchanged after the UI component is created.
 *   2. The storage of all menu item data is preallocated.
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
   Filename:      gui_fixed_menus.c
   Date Created:  July-22-2002
   Contains:      UI routines for Fixed Menus
**********************************************************************************/
#include "mmi_features.h"
#include "gui_config.h"
#include "gui_fixed_menus.h"
#include "custmenures.h"
#include "wgui_inline_edit.h"
#include "gui_windows.h"
#include "debuginitdef.h"
#include "gui_themes.h"
#include "wgui_categories_util.h"
#include "gui_setting.h"
#include "kal_non_specific_general_types.h"
#include "lcd_sw_rnd.h"
#include "gdi_include.h"
#include "wgui.h"

extern gdi_handle wgui_layer_1;
extern gdi_handle wgui_base_layer;
extern PU8 get_image(UI_image_ID_type );

#if defined(__MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__) && defined(__MMI_MATRIX_MAIN_MENU_OPTIMIZE__)
#define OPTIMIZE_MM_BUF_SIZE 7200
U8 opt_mm_buf[OPTIMIZE_MM_BUF_SIZE];
#endif /* defined(__MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__) && defined(__MMI_MATRIX_MAIN_MENU_OPTIMIZE__) */ 

/* 
 * Resident background filler cache of list menu shared among multiple screens 
 */

/* Enable resident filler cache if no hardware GIF and lcd size >= 176x220 */
#if (!defined(GDI_USING_HW_GIF) && !defined(GDI_USING_HW_GIF_V2)) && defined(__MMI_UI_LIST_CACHE_BACKGROUND__) && (!defined(__MMI_MAINLCD_128X128__) && !defined(__MMI_MAINLCD_128X160__))
//#define __MMI_UI_LIST_RESIDENT_FILLER_CACHE__
#endif 

#ifdef __MMI_UI_LIST_RESIDENT_FILLER_CACHE__

#define GUI_LIST_MENU_FILLER_CACHE_MAX_WIDTH    GDI_LCD_WIDTH

/* Sometimes menu height is slightly larger than MMI_CONTENT_HEIGHT */
#define GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT   (MMI_CONTENT_HEIGHT + 10)

#if (GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT > GDI_LCD_HEIGHT)
#undef GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT
#define GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT   GDI_LCD_HEIGHT
#endif /* (GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT > GDI_LCD_HEIGHT) */ 

/* Size of cache pool in byte */
#define GUI_LIST_MENU_FILLER_CACHE_SIZE   \
   ((GUI_LIST_MENU_FILLER_CACHE_MAX_WIDTH * GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT * GDI_MAINLCD_BIT_PER_PIXEL + 7) / 8)

/* Cached filler */
static UI_filled_area *g_gui_list_menu_filler_cache_target = NULL;

/* Cache pool of background filler (4-byte aligned for potential optimization) */
static U32 g_gui_list_menu_filler_cache[(GUI_LIST_MENU_FILLER_CACHE_SIZE + 3) / 4];

#endif /* __MMI_UI_LIST_RESIDENT_FILLER_CACHE__ */ 

S32 mmi_fixed_menu_bg_id = -1;
U16 g_ActiveScreenId;
void CSD_UI_reget_old_window();

/* for R2L characters */
extern BOOL r2lMMIFlag;

extern S32 menu_item_inline_multiline_box(void);

extern int gdi_layer_get_bit_per_pixel(void);
extern PU8 get_image(U16 i);

/* GUI: fixed list menu functions               */

UI_fixed_list_menu_theme *current_fixed_list_menu_theme = NULL;
S32 current_fixed_list_menuitem_display_index = -1;


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_menuitem_displayed_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_dummy_menuitem_displayed_function(S32 index, S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(index);
    UI_UNUSED_PARAMETER(x1);
    UI_UNUSED_PARAMETER(y1);
    UI_UNUSED_PARAMETER(x2);
    UI_UNUSED_PARAMETER(y2);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_list_menu_current_theme
 * DESCRIPTION
 *  Applies the current theme to a fixed list menu
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_list_menu_current_theme(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags |= current_fixed_list_menu_theme->flags;
    m->focussed_filler = current_fixed_list_menu_theme->focussed_filler;
    m->normal_filler = current_fixed_list_menu_theme->normal_filler;
    gui_resize_vertical_scrollbar(&m->vbar, current_fixed_list_menu_theme->vbar_width, m->vbar.height);
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_fixed_list_menu_theme->vbar_width, m->y);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_list_menu_theme
 * DESCRIPTION
 *  Applies the given theme to a fixed list menu
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 *  t       [IN]        Is the theme
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_list_menu_theme(fixed_list_menu *m, UI_fixed_list_menu_theme *t)
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
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_fixed_list_menu_theme->vbar_width, m->y);
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_fixed_list_menu
 * DESCRIPTION
 *  Creates a fixed list menu object
 * PARAMETERS
 *  m           [IN]        Is the fixed list menu object (pre-allocated)
 *  x           [IN]        Is the left-top corner position
 *  y           [IN]        Is the left-top corner position
 *  width       [IN]        Are the dimensions
 *  height      [IN]        Are the dimensions
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_fixed_list_menu(fixed_list_menu *m, S32 x, S32 y, S32 width, S32 height)
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
    m->spacing = 0;
    m->top = 0;
    gui_set_fixed_list_menu_current_theme(m);
    gui_create_vertical_scrollbar(
        &m->vbar,
        m->x + m->width - current_fixed_list_menu_theme->vbar_width,
        m->y,
        current_fixed_list_menu_theme->vbar_width,
        m->height);
    m->item_highlighted = UI_dummy_function_S32 ;
    m->item_unhighlighted = UI_dummy_function_S32 ;
    m->item_display_function = UI_fixed_menuitem_dummy_display_function;
    m->item_hide_function = UI_fixed_menuitem_dummy_display_function;
    m->item_measure_function = UI_fixed_menuitem_dummy_measure_function;
    m->item_highlight_function = UI_fixed_menuitem_dummy_highlight_function;
    m->item_remove_highlight_function = UI_fixed_menuitem_dummy_remove_highlight_function;
    m->item_resize_function = UI_fixed_menuitem_dummy_resize_function;
    m->items = NULL;
    m->common_item_data = NULL;
    m->item_displayed_callback = UI_dummy_menuitem_displayed_function;
    m->resized_before_locate = MMI_FALSE;
    m->act_layer_handle = GDI_LAYER_MAIN_BASE_LAYER_HANDLE;
    m->act_lcd_handle = GDI_LCD_MAIN_LCD_HANDLE;
#ifdef __MMI_TOUCH_SCREEN__
    m->pen_event_current_selected_callback_function = NULL;
    m->pen_event_default_selected_callback_function = NULL;
    m->pen_redraw_menu_function = NULL;
    m->item_pen_function = UI_fixed_menuitem_dummy_pen_function;
    m->disable_move_highlight = MMI_FALSE;
    m->disable_up_select = MMI_FALSE;
    m->pen_scroll_delay_time = 0;
    m->pen_scroll_after_delay = -1;
    memset(&(m->pen_state), 0, sizeof(gui_list_pen_state_struct));
#endif /* __MMI_TOUCH_SCREEN__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_fixed_list_menu
 * DESCRIPTION
 *  Changes the size of a fixed list menu
 * PARAMETERS
 *  m           [IN]        Is the fixed list menu object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_fixed_list_menu(fixed_list_menu *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    /* 
     * Example: change highlight in inline editor, list menu is resized 
     * according to virtual keyboard.
     * We have to recompute m->first_displayed_item
     */
    m->resized_before_locate = MMI_TRUE;
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_fixed_list_menu_theme->vbar_width, m->y);
    gui_resize_vertical_scrollbar(&m->vbar, m->vbar.width, m->height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_fixed_list_menu
 * DESCRIPTION
 *  Moves the fixed list menu to a different co-ordinate
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 *  x       [IN]        Is the new left-top corner position
 *  y       [IN]        Is the new left-top corner position
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_fixed_list_menu(fixed_list_menu *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->x = x;
    m->y = y;
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_fixed_list_menu_theme->vbar_width, m->y);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_locate_highlighted_item
 * DESCRIPTION
 *  Used to locate the highlighted item in the fixed list menu
 *  
 *  This is an internal function
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_locate_highlighted_item(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight;

#if 0
#endif /* 0 */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->flags & UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM)
    {
        S32 total_height = 0, i;
        U8 done = 0;
        S32 list_height = m->height;

        m->flags &= ~UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        m->first_displayed_item = m->highlighted_item;
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
            m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > list_height + 1)
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
        }
        total_height = 0;
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
            m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > list_height + 1)
            {
                done = 1;
                m->last_displayed_item = i - 1;
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
            m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > list_height + 1)
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
                m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
                total_height += iheight;
                if (total_height > list_height + 1)
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
    else if (m->flags & UI_LIST_MENU_CENTER_HIGHLIGHTED)
    {
        m->first_displayed_item = m->highlighted_item - 1;
        if (m->first_displayed_item < 0)
        {
            m->first_displayed_item = m->n_items - 1;
        }
        m->last_displayed_item = m->highlighted_item + 1;
        if (m->last_displayed_item >= m->n_items)
        {
            m->last_displayed_item = 0;
        }
    }
    else
    {
        if (m->highlighted_item < m->first_displayed_item)
        {
            m->first_displayed_item = m->highlighted_item;
        }
        else if (m->highlighted_item > m->last_displayed_item)
        {
            U8 done = 0;
            S32 total_height = 0, i;
            S32 list_height = m->height;

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
                m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
                total_height += iheight;
                if (total_height > list_height)
                {
                    done = 1;
                    m->first_displayed_item = i + 1;
                }
            }
        }
        else
        {
            U8 done = 0;
            S32 total_height = 0, i;
            S32 list_height = m->height;

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
                m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
                total_height += iheight;
                if (total_height > list_height)
                {
                    done = 1;
                    m->last_displayed_item = i - 1;
                }
            }
            if (!done)
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
                    m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
                    total_height += iheight;
                    if (total_height > list_height)
                    {
                        done = 1;
                        m->first_displayed_item = i + 1;
                        m->last_displayed_item = m->n_items - 1;
                        break;
                    }
                }
                if (!done)
                {
                    m->first_displayed_item = 0;
                    m->last_displayed_item = m->n_items - 1;
                }
            }
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
                    m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
                    total_height += iheight;
                    if (total_height > list_height)
                    {
                        done = 1;
                        m->first_displayed_item = i + 1;
                    }
                }
            }
        }
    }

    /* Located after resized */
    m->resized_before_locate = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_locate_previous_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_locate_previous_item(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->flags & UI_LIST_MENU_CENTER_HIGHLIGHTED)
    {
        if (m->highlighted_item == 0)
        {
            m->first_displayed_item = m->n_items - 1;
            m->last_displayed_item = 1;
        }
        else
        {
            m->first_displayed_item = m->highlighted_item - 1;
            if (m->first_displayed_item < 0)
            {
                m->first_displayed_item = m->n_items - 1;
            }
            m->last_displayed_item = m->highlighted_item + 1;
            if (m->last_displayed_item > m->n_items - 1)
            {
                m->last_displayed_item = 0;
            }
        }
    }
    else
    {
        if (m->highlighted_item < m->first_displayed_item)
        {
            m->first_displayed_item = m->highlighted_item;
        }
        else if (m->resized_before_locate)
        {
            gui_fixed_list_menu_locate_highlighted_item(m);
        }
    #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
        else if (m->highlighted_item == m->last_displayed_item - 1)
        {
            S32 iwidth, iheight;
            U8 done = 0;
            S32 total_height = 0, i;
            S32 list_height = m->height;

            current_fixed_list_menuitem_display_index = -1;
            for (i = m->last_displayed_item; i >= 0 && (!done); i--)
            {
                if (i == m->highlighted_item)
                {
                    current_fixed_list_menuitem_display_index = -1;
                }
                else
                {
                    current_fixed_list_menuitem_display_index = i;
                }
                m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
                total_height += iheight;
                if (total_height > list_height + 1)
                {
                    done = 1;
                    m->first_displayed_item = i + 1;
                }
            }
        }
    #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
    }

    /* Located after resized */
    m->resized_before_locate = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_locate_next_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_locate_next_item(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->flags & UI_LIST_MENU_CENTER_HIGHLIGHTED)
    {
        m->first_displayed_item = m->highlighted_item - 1;
        if (m->first_displayed_item < 0)
        {
            m->first_displayed_item = m->n_items - 1;
        }
        m->last_displayed_item = m->highlighted_item + 1;
        if (m->last_displayed_item >= m->n_items)
        {
            m->last_displayed_item = 0;
        }
    }
    else
    {
    #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
        if (m->highlighted_item >= m->last_displayed_item)
    #else 
        if (m->highlighted_item > m->last_displayed_item)
    #endif 
        {
            U8 done = 0;
            S32 total_height = 0, i;

            S32 list_height = m->height;

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
                m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
                total_height += iheight;
                if (total_height > list_height + 1)
                {
                    done = 1;
                    m->first_displayed_item = i + 1;
                }
            }
        }
        else if (m->resized_before_locate)
        {
            gui_fixed_list_menu_locate_highlighted_item(m);
        }
    }

    /* Located after resized */
    m->resized_before_locate = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  switch_fixed_list_highlighted_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m                           [?]         
 *  last_highlighted_item       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void switch_fixed_list_highlighted_item(fixed_list_menu *m, S32 last_highlighted_item)
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
            m->item_remove_highlight_function(m->items[last_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
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
 *  gui_fixed_list_menu_goto_item
 * DESCRIPTION
 *  Highlights a particular item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 *  i       [IN]        Is the index of the item to be highlighted (zero based)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_goto_item(fixed_list_menu *m, S32 i)
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
    gui_fixed_list_menu_locate_highlighted_item(m);

    switch_fixed_list_highlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_goto_next_item
 * DESCRIPTION
 *  Higlights the next item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_goto_next_item(fixed_list_menu *m)
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
        gui_fixed_list_menu_locate_previous_item(m);
    }
    else
    {
        m->highlighted_item++;
        gui_fixed_list_menu_locate_next_item(m);
    }

    switch_fixed_list_highlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_goto_previous_item
 * DESCRIPTION
 *  Higlights the previous item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_goto_previous_item(fixed_list_menu *m)
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
        gui_fixed_list_menu_locate_next_item(m);
    }
    else
    {
        m->highlighted_item--;
        gui_fixed_list_menu_locate_previous_item(m);
    }

    switch_fixed_list_highlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_goto_next_page
 * DESCRIPTION
 *  Higlights an item in the next page
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_goto_next_page(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_highlighted_item;
    S32 iwidth, iheight;
    U8 done = 0;
    S32 total_height = 0, i;
    S32 list_height = m->height - 4;

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
            m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > list_height + 1)
            {
                done = 1;
                m->last_displayed_item = i - 1;
            }
        }
    }
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
        m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
        total_height += iheight;
        if (total_height > list_height + 1)
        {
            done = 1;
            m->first_displayed_item = i + 1;
        }
    }
    m->highlighted_item = m->first_displayed_item;

    switch_fixed_list_highlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_goto_previous_page
 * DESCRIPTION
 *  Higlights an item in the previous page
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_goto_previous_page(fixed_list_menu *m)
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
    gui_fixed_list_menu_locate_highlighted_item(m);

    switch_fixed_list_highlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_goto_first_item
 * DESCRIPTION
 *  Higlights the first item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_goto_first_item(fixed_list_menu *m)
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
    gui_fixed_list_menu_locate_highlighted_item(m);

    switch_fixed_list_highlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_goto_last_item
 * DESCRIPTION
 *  Higlights the last item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_goto_last_item(fixed_list_menu *m)
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
    gui_fixed_list_menu_locate_highlighted_item(m);

    switch_fixed_list_highlighted_item(m, last_highlighted_item);
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_menu_background_filler_from_cache
 * DESCRIPTION
 *  Displays the menu background in another GDI layer
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 *  f       [IN]        Background filler
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_UI_LIST_CACHE_BACKGROUND__

/* Check compile option dependency */
#if !defined(__GDI_MEMORY_PROFILE_2__) || defined(__MMI_WALLPAPER_ON_BOTTOM__)
#error "__MMI_UI_LIST_CACHE_BACKGROUND__ require __GDI_MEMORY_PROFILE_2__"
#endif 

#ifdef __MMI_UI_LIST_RESIDENT_FILLER_CACHE__

void gui_show_menu_background_filler_from_cache(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!f)
    {
        return;
    }

    if (g_gui_list_menu_filler_cache_target != f)
    {
        /* Cache filler */
        gdi_handle cache_layer;

        g_gui_list_menu_filler_cache_target = f;

        gdi_layer_create_using_outside_memory(
            0,
            0,
            GUI_LIST_MENU_FILLER_CACHE_MAX_WIDTH,
            GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT,
            &cache_layer,
            (U8*) g_gui_list_menu_filler_cache,
            sizeof(g_gui_list_menu_filler_cache));

        gdi_layer_push_and_set_active(cache_layer);
        gui_draw_filled_area(
            0,
            0,
            GUI_LIST_MENU_FILLER_CACHE_MAX_WIDTH - 1,
            GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT - 1,
            f);
        gdi_layer_pop_and_restore_active();

        gdi_layer_free(cache_layer);
    }

    if ((x2 - x1 + 1 > GUI_LIST_MENU_FILLER_CACHE_MAX_WIDTH) || (y2 - y1 + 1 > GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT))
    {
        MMI_DBG_ASSERT(0);  /* invaild cache */
        gui_draw_filled_area(x1, y1, x2, y2, f);
    }
    else
    {
        PU8 dest;
        S32 dest_layer_width, dest_layer_height;
        gdi_rect_struct dest_clip;

        gdi_layer_get_buffer_ptr(&dest);
        gdi_layer_get_dimension(&dest_layer_width, &dest_layer_height);
        dest_clip.x1 = x1;
        dest_clip.y1 = y1;
        dest_clip.x2 = x2;
        dest_clip.y2 = y2;

        gdi_2d_memory_blt(
            (U8*) g_gui_list_menu_filler_cache,
            GUI_LIST_MENU_FILLER_CACHE_MAX_WIDTH,
            0,
            0,
            GUI_LIST_MENU_FILLER_CACHE_MAX_WIDTH,
            GUI_LIST_MENU_FILLER_CACHE_MAX_HEIGHT,
            dest,
            dest_layer_width,
            x1,
            y1,
            dest_clip);
    }
}

#endif /* __MMI_UI_LIST_RESIDENT_FILLER_CACHE__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_show_menu_background_in_layer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 *  f       [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_menu_background_in_layer(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 width = x2 - x1 + 1;
    S32 height = y2 - y1 + 1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*
     * We do not use gui_set_transparent_source_layer() in this function because 
     * not all list menu use gui_show_menu_background_in_layer(), then some menus
     * have transparency effect but others do not on platforms that
     * !defined(__MMI_WALLPAPER_ON_BOTTOM__) && defined (__MMI_UI_TRANSPARENT_EFFECT__)
     */
     
    if (!gui_is_entry_new_screen())
    {
        S32 layer_width, layer_height, layer_x, layer_y;

        gdi_layer_push_and_set_active(wgui_layer_1);

        gdi_layer_get_position(&layer_x, &layer_y);
        gdi_layer_get_dimension(&layer_width, &layer_height);

        if (layer_width != (x2 - x1 + 1) || layer_height != (y2 - y1 + 1))
        {
            MMI_DBG_ASSERT(0);
        }
        if (layer_x != x1 || layer_y != y1)
        {
            gdi_layer_set_position(x1, y1);
        }

        gdi_layer_pop_and_restore_active();

        if (dm_is_activated())
        {
            /* 
             * If draw manager is reset by RedrawCategoryScreen(), original information of draw manager layers are discarded
             * We need to configure the layer information again.
             */
            dm_add_blt_layer(wgui_layer_1, DM_LAYER_BOTTOM);
        }

        gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
    }
    else
    {
        gui_set_entry_new_screen(MMI_FALSE);

        gdi_layer_multi_layer_enable();
        gdi_layer_get_base_handle(&wgui_base_layer);

        /* Setup the base layer */
        gdi_layer_push_and_set_active(wgui_base_layer);
        gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	 //gdi_layer_set_opacity(TRUE, UI_SUBMENU_OPACITY_BASED_LAYER);
        gdi_layer_push_clip();
        gdi_layer_set_clip(x1, y1, x2, y2);
        gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
        gdi_layer_pop_clip();
        gdi_layer_pop_and_restore_active();

        /* Create multilayer buffer */
        if (wgui_layer_1 == GDI_LAYER_EMPTY_HANDLE)
        {
            if (dm_is_activated())
            {
                /* BLT layers are controlled by draw manager */
                dm_create_layer(x1, y1, width, height, &wgui_layer_1, DM_LAYER_BOTTOM);
            }
            else
            {
                gdi_layer_create(x1, y1, width, height, &wgui_layer_1);
                gdi_layer_set_blt_layer(wgui_layer_1, wgui_base_layer, 0, 0);
            }
        }

        gdi_layer_push_and_set_active(wgui_layer_1);
    #ifdef __MMI_UI_LIST_RESIDENT_FILLER_CACHE__
        gui_show_menu_background_filler_from_cache(0, 0, width - 1, height - 1, f);
    #else 
        gui_draw_filled_area(0, 0, width - 1, height - 1, f);
    #endif 
        gdi_layer_pop_and_restore_active();
    }
}

#endif /* __MMI_UI_LIST_CACHE_BACKGROUND__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_show_list_menu_background_outside_area
 * DESCRIPTION
 *  Show the extended menu background outside the menu display area. 
 *  For example, in handwriting mode of inline editor, we may want to draw 
 *  menu background in the bottom virtual keyboard area when the keyboard is hidden.
 *  Note: This API does not use any menu cache
 * PARAMETERS
 *  m       [IN]        Is the list menu object
 *  x1      [IN]        left-top x
 *  y1      [IN]        left-top y
 *  x2      [IN]        right-bottom x
 *  y2      [IN]        right-bottom y
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_list_menu_background_outside_area(fixed_list_menu *m, S32 x1, S32 y1, S32 x2, S32 y2)
{
    UI_filled_area *f;

    if (m->flags & UI_LIST_MENU_STATE_FOCUSSED)
    {
        f = m->focussed_filler;
    }
    else
    {
        f = m->normal_filler;
    }

    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);

    gui_draw_filled_area(m->x, m->y, x2, y2, f);
        
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_fixed_list_menu
 * DESCRIPTION
 *  Displays the fixed list menu
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
extern S32 MMI_current_menu_type;

#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
extern MMI_theme *current_MMI_theme;
extern BOOL line_draw;
extern fixed_list_menu MMI_fixed_list_menu;
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 

void gui_show_fixed_list_menu(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, y_offset;

#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    S32 ic, menu_grid_gap;
#endif 
    UI_filled_area *f;
    S32 i;
    S32 cx1, cy1, cx2, cy2;
    S32 tx1, ty1, tx2, ty2;
    S32 iwidth, iheight;
    U8 done = 0;
    S32 total_height, counter, list_height;
    U8 disable_draw = 0;
    MMI_BOOL show_scrollbar;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->flags & UI_LIST_MENU_DISABLE_DRAW)
    {
        disable_draw = 1;
    }
    gui_get_clip(&cx1, &cy1, &cx2, &cy2);
    gui_get_text_clip(&tx1, &ty1, &tx2, &ty2);

    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

    /* Use the original x1, y1, x2, y2 for background filler */

    if (m->flags & UI_LIST_MENU_STATE_FOCUSSED)
    {
        f = m->focussed_filler;
    }
    else
    {
        f = m->normal_filler;
    }

    if (!disable_draw && (!(m->flags & UI_LIST_MENU_DISABLE_BACKGROUND)) && MMI_current_menu_type != PAGE_MENU)
    {
        gui_set_clip(x1, y1, x2, y2);

        if (!(m->flags & UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER) && (wgui_is_wallpaper_on_bottom() == MMI_TRUE))
        {
            gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
             
        #if (defined (__MMI_UI_TRANSPARENT_EFFECT__) || defined (__MMI_UI_LIST_HIGHLIGHT_EFFECTS__))
             
            if (gui_get_transparent_source_layer() == GDI_LAYER_EMPTY_HANDLE)   /* 110705 WAP menu Clavin add */
            {
                gui_set_transparent_source_layer(dm_get_scr_bg_layer());        /* should remove */
            }
        #endif /* (defined (__MMI_UI_TRANSPARENT_EFFECT__) || defined (__MMI_UI_LIST_HIGHLIGHT_EFFECTS__)) */ 
        }
    #ifdef __MMI_UI_LIST_CACHE_BACKGROUND__
        else if (!(m->flags & UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER) &&
                 (dm_get_scr_bg_layer() == GDI_ERROR_HANDLE) && (gui_get_multilayer_mask() & UI_MUL_BKG_SUBMENU))
        {
#if (defined(__COOLSAND_SUBMENU_LEVEL2_POPMENU__)&&defined(__MMI_MAINLCD_220X176__))
			if(MMI_CATEGORY_POPUPMENU_ID == g_dm_data.S32CatId)
				gui_fill_rectangle(x1,y1,x2,y2,gui_color(0,103,206));
			else
#endif 
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
		/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-26 18:41 */
		#if 0 //def __MMI_WALLPAPER_ON_BOTTOM__
	               gdi_draw_solid_rect(x1, y1, x2, y2, gdi_act_color_from_rgb(255, c.r, c.g, c.b));
		#else
			gui_draw_filled_area(x1,y1,x2,y2,f);
		#endif
		/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-26 18:41 */
        #endif  
        }
    }

    if (m->flags & UI_LIST_MENU_ENABLE_TRANSITION)
    {
        color_t c = gui_color(255, 255, 255);

        gui_push_clip();
        gui_set_clip(x1, y1, x2, y2);
        gui_fill_rectangle(x1, y1, x2, y2, c);
        gui_pop_clip();
    }

    if (m->n_items <= 0)
    {
        return;
    }

    /* Check presence of scrollbar */
    show_scrollbar = MMI_FALSE;
    if (!(m->flags & UI_LIST_MENU_DISABLE_SCROLLBAR))
    {
        if (m->flags & UI_LIST_MENU_AUTO_DISABLE_SCROLLBAR)
        {
            if (m->first_displayed_item > 0)
            {
                show_scrollbar = MMI_TRUE;
            }
            else
            {
                /* FIXME. we need to compute m->displayed_items twice */
                gui_show_fixed_list_menu_no_draw(m);
                if (m->displayed_items >= m->n_items)
                {
                    show_scrollbar = MMI_TRUE;
                }
            }
        }
        else
        {
            show_scrollbar = MMI_TRUE;
        }
    }

    /* Configure region of menu items */
    if (show_scrollbar)
    {
        if (!r2lMMIFlag)
        {
            x2 -= m->vbar.width;
        }
        else
        {
            x1 += m->vbar.width;
        }
    }

    /* Display menu items */
    if (MMI_current_menu_type == PAGE_MENU)
    {
        if (!disable_draw)
        {
            m->item_display_function(m->items[m->highlighted_item], m->common_item_data, m->x, m->y);
        }
        return;
    }

    y1 = m->y + m->top;

    list_height = m->height;
    gui_set_text_clip(x1, y1, x2, y2);
    gui_set_clip(x1, y1, x2, y2);
    total_height = 0;
    counter = 0;
    current_fixed_list_menuitem_display_index = -1;

    if (m->flags & UI_LIST_MENU_CENTER_HIGHLIGHTED)
    {
    #if 0    
		#if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) || defined(__MMI_UI_HINTS_IN_MENUITEM__)
		#endif
    #endif /* 0 */ 
        m->item_measure_function(m->items[m->highlighted_item], m->common_item_data, &iwidth, &iheight);

        current_fixed_list_menuitem_display_index = m->first_displayed_item;
        m->item_display_function(m->items[m->first_displayed_item], m->common_item_data, x1, y1);

        current_fixed_list_menuitem_display_index = m->highlighted_item;
        m->item_display_function(m->items[m->highlighted_item], m->common_item_data, x1, y1 + iheight);

        current_fixed_list_menuitem_display_index = m->last_displayed_item;
        m->item_display_function(m->items[m->last_displayed_item], m->common_item_data, x1, y1 + iheight * 2);
    }
    else
    {
        for (i = m->first_displayed_item; (i < m->n_items && !done); i++)
        {
            if (i == m->highlighted_item)
            {
                current_fixed_list_menuitem_display_index = -1;
            }
            else
            {
                current_fixed_list_menuitem_display_index = i;
            }
            m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
            y_offset = total_height;
            /* total_height+=iheight+m->spacing; */
            total_height += iheight;
            if (total_height > list_height + 1)
            {
                done = 1;
                /* This is required so that in trnasition if theire is no area to display the menu item it is not displayed */
                if ((counter == 0) && !disable_draw && !(m->flags & UI_LIST_MENU_ENABLE_TRANSITION))
                {
                    m->item_display_function(m->items[i], m->common_item_data, x1, y_offset + y1);
                }
            }
            else
            {
                if (!disable_draw)
                {
                    m->item_display_function(m->items[i], m->common_item_data, x1, y_offset + y1);
                }
                counter++;
            }

        #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
            if (line_draw && MMI_current_menu_type == LIST_MENU)
            {
                if (!r2lMMIFlag)
                {
                    gui_draw_horizontal_line(
                        x1,
                        m->vbar.x - 2,
                        y_offset + y1,
                        current_MMI_theme->list_background_filler->border_color);
                }
                else
                {
                    gui_draw_horizontal_line(
                        x1,
                        x2,
                        y_offset + y1,
                        current_MMI_theme->list_background_filler->border_color);
                }
            }
        #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
        }

    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        ic = i;
        /* To fix the grid issue due to top variable value. */
        menu_grid_gap = MMI_fixed_list_menu.height % iheight;
        menu_grid_gap -= m->top;
        if (line_draw && MMI_current_menu_type == LIST_MENU)
        {
            // To grid for sub menu screens
            // Used -2 for proper area selection
            if (ic == m->n_items)
            {
                if (!r2lMMIFlag)
                {
                    gui_draw_horizontal_line(
                        x1,
                        m->vbar.x - 2,
                        y_offset + y1 + iheight,
                        current_MMI_theme->list_background_filler->border_color);
                }
                else
                {
                    gui_draw_horizontal_line(
                        x1,
                        x2,
                        y_offset + y1 + iheight,
                        current_MMI_theme->list_background_filler->border_color);
                }
            }
            if (!r2lMMIFlag)
            {
                if (ic == m->n_items && done)
                {
                    gui_draw_vertical_line(
                        y1,
                        y1 + y_offset,
                        m->vbar.x - 2,
                        current_MMI_theme->list_background_filler->border_color);
                }
                else
                {
                    gui_draw_vertical_line(
                        y1,
                        ((y1 + y_offset + iheight) > (UI_device_height - MMI_BUTTON_BAR_HEIGHT - menu_grid_gap) ? (UI_device_height - MMI_BUTTON_BAR_HEIGHT - menu_grid_gap) : (y1 + y_offset + iheight)),
                        m->vbar.x - 2,
                        current_MMI_theme->list_background_filler->border_color);
                }
            }
            else
            {
                if (i == m->n_items && done)
                {
                    gui_draw_vertical_line(
                        y1,
                        y1 + y_offset,
                        (m->vbar.x + m->vbar.width - m->width) + m->vbar.width,
                        current_MMI_theme->list_background_filler->border_color);
                }
                else
                {
                    gui_draw_vertical_line(
                        y1,
                        ((y1 + total_height) > (UI_device_height - MMI_BUTTON_BAR_HEIGHT - menu_grid_gap) ? (UI_device_height - MMI_BUTTON_BAR_HEIGHT - menu_grid_gap) : (y1 + total_height)),
                        (m->vbar.x + m->vbar.width - m->width) + m->vbar.width,
                        current_MMI_theme->list_background_filler->border_color);
                }
            }
        }
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 

        current_fixed_list_menuitem_display_index = -1;
        if (counter == 0)
        {
            m->last_displayed_item = m->first_displayed_item;
        }
        else
        {
            m->last_displayed_item = m->first_displayed_item + counter - 1;
        }
        m->displayed_items = counter;
    }

    gui_set_clip(cx1, cy1, cx2, cy2);
    gui_set_text_clip(tx1, ty1, tx2, ty2);

    /* Handle scrollbar */
    if (show_scrollbar)
    {
        /* FIXME. debug only. we can remove the check in the future */
        MMI_DBG_ASSERT(!(m->flags & UI_LIST_MENU_AUTO_DISABLE_SCROLLBAR) ||
                       (total_height > list_height + 1) || (m->first_displayed_item > 0));

        if (r2lMMIFlag)
        {
            gui_move_vertical_scrollbar(&m->vbar, m->x, m->y);
        }
        else
        {
            gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - m->vbar.width, m->y);
        }

        gui_set_vertical_scrollbar_range(&m->vbar, m->n_items);
        gui_set_vertical_scrollbar_scale(&m->vbar, m->displayed_items);
        gui_set_vertical_scrollbar_value(&m->vbar, m->first_displayed_item);

        if (!disable_draw)
        {
            gui_show_vertical_scrollbar(&m->vbar);
        }
    }

     
#if defined (__MMI_UI_DALMATIAN_FIXEDLIST__) && !defined(__MMI_TOUCH_SCREEN__)
    if (line_draw && MMI_current_menu_type == LIST_MENU)
         
    {
        gui_push_clip();
        if (!r2lMMIFlag)
        {
            gui_set_clip(m->vbar.x - 1, y1, m->vbar.x + m->vbar.width - 1, m->vbar.y + m->vbar.height - 1);
        }
        else
        {
            gui_set_clip(
                (m->vbar.x + m->vbar.width - m->width),
                y1,
                (m->vbar.x + m->vbar.width - m->width) + m->vbar.width,
                m->vbar.y + m->vbar.height - 1);
        }
        /* To grid for sub menu screens */
        if (!r2lMMIFlag)
        {
            gui_draw_vertical_line(y1, y1 + m->vbar.height - 4, m->vbar.x, current_MMI_theme->list_background_filler->border_color);    /* Start of scroll bar */
            gui_draw_vertical_line(y1, y1 + m->vbar.height - 4, (m->vbar.x + m->vbar.width) - 1, current_MMI_theme->list_background_filler->border_color);      /* End of Scroll bar */
            gui_draw_vertical_line(y1, y1 + m->vbar.height - 4, m->vbar.x - 1, current_MMI_theme->list_background_filler->c);   /* To show white gap betwen scroll bar & menu area */
        }
        else
        {
            gui_draw_vertical_line(y1, y1 + m->vbar.height - 4, m->vbar.x + m->vbar.width - m->width, current_MMI_theme->list_background_filler->border_color); /* Start of Scroll bar */
            gui_draw_vertical_line(y1, y1 + m->vbar.height - 4, (m->vbar.x + m->vbar.width - m->width) + m->vbar.width - 2, current_MMI_theme->list_background_filler->border_color);   /* End of Scroll bar */
        }
        gui_pop_clip();
        ic = 0;
    }
#endif /* defined (__MMI_UI_DALMATIAN_FIXEDLIST__) && !defined(__MMI_TOUCH_SCREEN__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_fixed_list_menu_no_draw
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_fixed_list_menu_no_draw(fixed_list_menu *m)
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
        if (i == m->highlighted_item)
        {
            current_fixed_list_menuitem_display_index = -1;
        }
        else
        {
            current_fixed_list_menuitem_display_index = i;
        }
        m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
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
    if (counter == 0)
    {
        m->last_displayed_item = m->first_displayed_item;
    }
    else
    {
        m->last_displayed_item = m->first_displayed_item + counter - 1;
    }
    m->displayed_items = counter;
    if (!(m->flags & UI_LIST_MENU_DISABLE_SCROLLBAR))
    {
        gui_set_vertical_scrollbar_range(&m->vbar, m->n_items);
        gui_set_vertical_scrollbar_scale(&m->vbar, m->displayed_items);
        gui_set_vertical_scrollbar_value(&m->vbar, m->first_displayed_item);
    }
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_translate_pen_position
 * DESCRIPTION
 *  Translate position to item index
 *  
 *  if y < minimum bound, *item_index is first_displayed_item - 1 or 0
 *  if y > maximum bound, *item_index is last_displayed_item + 1 or (n_items - 1)
 *  Caller of this function might be sensitive to the return result.
 *  Be careful when modifying this function.
 * PARAMETERS
 *  m               [IN]        Is the fixed list menu object
 *  y               [IN]        Y-axis coordinate
 *  item_index      [OUT]       
 * RETURNS
 *  whether the position is on menu items
 *****************************************************************************/
BOOL gui_fixed_list_menu_translate_pen_position(fixed_list_menu *m, S32 y, S32 *item_index)
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

    if (m->n_items <= 0)
    {
        *item_index = 0;
        /* ret is false */
    }
    else if (y < 0)
    {
        *item_index = (m->first_displayed_item > 0) ? m->first_displayed_item - 1 : 0;
        /* ret is false */
    }
    else if (y >= m->height)
    {
        *item_index = (m->last_displayed_item < m->n_items - 1) ? m->last_displayed_item + 1 : m->n_items - 1;
        /* ret is false */
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
            m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
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

/* 
 * IN: m, item_index
 * OUT: x, y
 */


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_get_menuitem_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m               [?]         
 *  item_index      [IN]        
 *  x               [?]         
 *  y               [?]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_fixed_list_menu_get_menuitem_position(fixed_list_menu *m, S32 item_index, S32 *x, S32 *y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, total_height = m->y;
    S32 iwidth, iheight;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *x = m->x;

    if (item_index <= m->first_displayed_item)
    {
        *y = total_height;
        return;
    }

    for (i = m->first_displayed_item; i <= m->last_displayed_item; i++)
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
        m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
        total_height += iheight;
        if (i + 1 == item_index)
        {
            *y = total_height;
            return;
        }
    }
    *y = total_height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_scroll_by_pen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m                   [?]         
 *  first_displayed     [IN]        
 *  menu_event          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_fixed_list_menu_scroll_by_pen(fixed_list_menu *m, S32 first_displayed, gui_list_pen_enum *menu_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 total_height = 0, i;
    S32 iwidth, iheight;
    S32 last_displayed = m->n_items - 1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (first_displayed > m->highlighted_item)
    {
        m->flags |= UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        gui_fixed_list_menu_goto_item(m, first_displayed);
        m->flags &= ~UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
    }
    else
    {
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
            m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > m->height + 1)
            {
                last_displayed = i - 1;
                break;
            }
        }
        if (i == m->n_items)
        {
            last_displayed = m->n_items - 1;
        }

        if (last_displayed < m->highlighted_item)
        {
            m->flags |= UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
            gui_fixed_list_menu_goto_item(m, last_displayed);
            m->flags &= ~UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
            *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
        }
        else
        {
            m->first_displayed_item = first_displayed;
            m->last_displayed_item = last_displayed;
            *menu_event = GUI_LIST_PEN_NEED_REDRAW;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_translate_pen_event
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
BOOL gui_fixed_list_menu_translate_pen_event(
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
    gui_list_pen_enum menuitem_event;
    gui_pen_event_param_struct menuitem_param;
    S32 item_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pen_state = &m->pen_state;
    ret = MMI_TRUE;

    *menu_event = GUI_LIST_PEN_NONE;

    if (m->flags & UI_LIST_MENU_DISABLE_PEN)
    {
        return MMI_FALSE;
    }

    if (pen_event != MMI_PEN_EVENT_DOWN && pen_state->pen_on_scrollbar)
    {
        gui_vertical_scrollbar_translate_pen_event(&m->vbar, pen_event, x, y, &scrollbar_event, &scrollbar_param);
        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
        {
            gui_fixed_list_menu_scroll_by_pen(m, scrollbar_param._u.i, menu_event);
        }
    }
    else if (pen_event != MMI_PEN_EVENT_DOWN && pen_state->handled_by_menu_item)
    {
        /* For example, tap down on checkbox of two-state menuitem, the subsequent move/up events are discarded */
        S32 item_x, item_y;

        gui_fixed_list_menu_get_menuitem_position(m, m->highlighted_item, &item_x, &item_y);
        m->item_pen_function(
            m->items[m->highlighted_item],
            m->common_item_data,
            item_x,
            item_y,
            pen_event,
            x,
            y,
            &menuitem_event,
            &menuitem_param);

        if (menuitem_event == GUI_TWOSTATE_PEN_TOGGLE)
        {
            menuitem_event = GUI_LIST_PEN_ITEM_SELECTED;
        }
        *menu_event = menuitem_event;
    }
    else
    {
        switch (pen_event)
        {
            case MMI_PEN_EVENT_DOWN:
                if (PEN_CHECK_BOUND(x, y, m->x, m->y, m->width, m->height))
                {
                    pen_state->handled_by_menu_item = 0;
                    pen_state->pen_on_scrollbar = 0;

                    if (m->n_items <= 0)
                    {
                        /* In current design, scrollbar is hidden if m->n_items == 0 */
                        ret = MMI_FALSE;
                    }
                    else if (!(m->flags & UI_LIST_MENU_DISABLE_SCROLLBAR) &&
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
                            gui_fixed_list_menu_scroll_by_pen(m, scrollbar_param._u.i, menu_event);
                        }
                    }
                    else
                    {
                        if (gui_fixed_list_menu_translate_pen_position(m, y, &item_index))
                        {
                            S32 item_x, item_y;

                            if (item_index != m->highlighted_item)
                            {
                                gui_fixed_list_menu_goto_item(m, item_index);
                                *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
                            }

                            /* Even if highlight is changed, we still need to invoke menuitem pen handler */
                            gui_fixed_list_menu_get_menuitem_position(m, item_index, &item_x, &item_y);
                            if (m->item_pen_function(
                                    m->items[item_index],
                                    m->common_item_data,
                                    item_x,
                                    item_y,
                                    MMI_PEN_EVENT_DOWN,
                                    x,
                                    y,
                                    &menuitem_event,
                                    &menuitem_param))
                            {
                                if (menuitem_event == GUI_TWOSTATE_PEN_TOGGLE)
                                {
                                    menuitem_event = GUI_LIST_PEN_ITEM_SELECTED;
                                }
                                /* original menu_event could be GUI_LIST_PEN_HIGHLIGHT_CHANGED */
                                if (menuitem_event != GUI_LIST_PEN_NONE)
                                {
                                    *menu_event = menuitem_event;
                                }

                                pen_state->handled_by_menu_item = 1;
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
                if (!m->disable_move_highlight)
                {
                    gui_fixed_list_menu_translate_pen_position(m, y, &item_index);
                    if (item_index != m->highlighted_item)
                    {
                        gui_fixed_list_menu_goto_item(m, item_index);
                        *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
                    }
                }
                break;

            case MMI_PEN_EVENT_UP:
                if (!m->disable_up_select)
                {
                    if (PEN_CHECK_BOUND(x, y, m->x, m->y, m->width, m->height))
                    {
                        gui_fixed_list_menu_translate_pen_position(m, y, &item_index);
                        if (item_index != m->highlighted_item)
                        {
                            if (!m->disable_move_highlight)
                            {
                                /* If MMI_PEN_EVENT_MOVE is not delivered before moving to this item */
                                gui_fixed_list_menu_goto_item(m, item_index);
                                *menu_event = GUI_LIST_PEN_HIGHLIGHT_CHANGED;
                            }
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
                }
                break;

            case MMI_PEN_EVENT_ABORT:
                /* Do nothing */
                break;

            default:
                MMI_ASSERT(0);
        }
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


/*****************************************************************************
 * FUNCTION
 *  gui_hide_fixed_list_menu_highlighted_item
 * DESCRIPTION
 *  Hides the highlighted item in a fixed list menu
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_hide_fixed_list_menu_highlighted_item(fixed_list_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, y_offset;
    S32 i;
    S32 cx1, cy1, cx2, cy2;
    S32 tx1, ty1, tx2, ty2;
    S32 iwidth, iheight;
    U8 done = 0;
    S32 total_height, counter, list_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_get_clip(&cx1, &cy1, &cx2, &cy2);
    gui_get_text_clip(&tx1, &ty1, &tx2, &ty2);
    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;
    x1 += 3;
    x2 -= (3 - m->vbar.width);
    y1 += 3;
    y2 -= 3;
    list_height = m->height - 4;
    gui_set_text_clip(x1, y1, x2, y2);
    gui_set_clip(x1, y1, x2, y2);
    total_height = 0;
    counter = 0;
    for (i = m->first_displayed_item; (i < m->n_items && !done); i++)
    {
        y_offset = total_height;
        if (i == m->highlighted_item)
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
            m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
        }
        if (total_height > list_height + 1)
        {
            done = 1;
            if ((counter == 0) && (i == m->highlighted_item))
            {
                m->item_hide_function(m->items[i], m->common_item_data, x1, y_offset + y1);
            }
        }
        else
        {
            if (i == m->highlighted_item)
            {
                m->item_hide_function(m->items[i], m->common_item_data, x1, y_offset + y1);
            }
            counter++;
        }
    }
    gui_set_clip(cx1, cy1, cx2, cy2);
    gui_set_text_clip(tx1, ty1, tx2, ty2);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_list_menu_item_functions
 * DESCRIPTION
 *  Sets the functions used to display generic menuitems
 * PARAMETERS
 *  m                                   [IN]        Is the fixed list menu object
 *  item_display_function               [IN]        Is the function used to display an item
 *  item_measure_function               [IN]        Is the function used to measure an item
 *  item_highlight_function             [OUT]       Is the function used to highlight an item
 *  item_remove_highlight_function      [IN]        Is the function used to remove highlight of an item
 *  item_hide_function                  [IN]        Is the function used to hide the menuitem
 *  item_resize_function                [IN]        
 *  width(?)                            [OUT]       Height      are the dimensions of the menuitem
 *  item(?)                             [IN]        Is the specific fixed menuitem
 *  x(?)                                [IN]        Position at which the menuitem was displayed
 *  y(?)                                [IN]        Position at which the menuitem was displayed
 *  common_item_data(?)                 [IN]        Common fixed menuitem
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_list_menu_item_functions(
        fixed_list_menu *m,
        void (*item_display_function) (void *item, void *common_item_data, S32 x, S32 y),
        void (*item_measure_function) (void *item, void *common_item_data, S32 *width, S32 *height),
        void (*item_highlight_function) (void *item, void *common_item_data),
        void (*item_remove_highlight_function) (void *item, void *common_item_data),
        void (*item_hide_function) (void *item, void *common_item_data, S32 x, S32 y),
        void (*item_resize_function) (S32 x, S32 y))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->item_display_function = item_display_function;
    m->item_measure_function = item_measure_function;
    m->item_highlight_function = item_highlight_function;
    m->item_remove_highlight_function = item_remove_highlight_function;
    m->item_hide_function = item_hide_function;
    m->item_resize_function = item_resize_function;
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_list_menu_set_pen_scroll_delay
 * DESCRIPTION
 *  Sets the delay time that menu scrolling after scrollbar is scrolled by pen.
 *  
 *  Typically used in dynamic and asyncdynamic list menu because it need to load
 *  data when menu is scrolled.
 *  
 *  THE SCROLL DELAY IS IMPLEMENTED IN DYNAMIC AND ASYNCDYNAMIC LIST ONLY.
 *  THIS IS DUMMY FUNCTION FOR API CONSISTENCY.
 * PARAMETERS
 *  m               [IN]        Is the fixed list menu object
 *  delay_time      [IN]        In milliseconds
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_list_menu_set_pen_scroll_delay(fixed_list_menu *m, S32 delay_time)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_DBG_ASSERT(delay_time < (S32) 0x0000FFFF);
    m->pen_scroll_delay_time = (S16) delay_time;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_list_menu_item_pen_function
 * DESCRIPTION
 *  Sets the pen handler of menu item
 * PARAMETERS
 *  m                           [IN]        Is the fixed list menu object
 *  item_pen_function           [IN]        Is the function functions used to handle pen events in menu item
 *  disable_move_highlight      [IN]        Do not send GUI_LIST_PEN_HIGHLIGHT_CHANGED on Pen Move and (sometimes) Pen Up
 *  disable_up_select           [IN]        Do not send GUI_LIST_PEN_ITEM_SELECTED on Pen Up.
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_list_menu_item_pen_function(
        fixed_list_menu *m,
        fixed_menuitem_pen_hdlr item_pen_function,
        BOOL disable_move_highlight,
        BOOL disable_up_select)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->item_pen_function = item_pen_function;
    m->disable_move_highlight = disable_move_highlight;
    m->disable_up_select = disable_up_select;
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_list_menu_common_item_data
 * DESCRIPTION
 *  Sets the common item data that the list of items should use
 * PARAMETERS
 *  m       [IN]        Is the fixed list menu object
 *  c       [IN]        Is the common item data
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_list_menu_common_item_data(fixed_list_menu *m, void *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->common_item_data = c;
}

/* GUI: fixed matrix menu functions             */

UI_fixed_matrix_menu_theme *current_fixed_matrix_menu_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_matrix_menu_current_theme
 * DESCRIPTION
 *  Applies the current theme to a fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_matrix_menu_current_theme(fixed_matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags |= current_fixed_matrix_menu_theme->flags;
    m->focussed_filler = current_fixed_matrix_menu_theme->focussed_filler;
    m->normal_filler = current_fixed_matrix_menu_theme->normal_filler;
    gui_resize_vertical_scrollbar(
        &m->vbar,
        current_fixed_matrix_menu_theme->vbar_width,
        m->height - current_fixed_matrix_menu_theme->hbar_height);
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_fixed_matrix_menu_theme->vbar_width, m->y);
    gui_resize_horizontal_scrollbar(
        &m->hbar,
        m->width - current_fixed_matrix_menu_theme->vbar_width,
        current_fixed_matrix_menu_theme->hbar_height);
    gui_move_horizontal_scrollbar(&m->hbar, m->x, m->y + m->height - current_fixed_matrix_menu_theme->hbar_height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_matrix_menu_theme
 * DESCRIPTION
 *  Applies the given theme to a fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 *  t       [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_matrix_menu_theme(fixed_matrix_menu *m, UI_fixed_matrix_menu_theme *t)
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
        current_fixed_matrix_menu_theme->vbar_width,
        m->height - current_fixed_matrix_menu_theme->hbar_height);
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_fixed_matrix_menu_theme->vbar_width, m->y);
    gui_resize_horizontal_scrollbar(
        &m->hbar,
        m->width - current_fixed_matrix_menu_theme->vbar_width,
        current_fixed_matrix_menu_theme->hbar_height);
    gui_move_horizontal_scrollbar(&m->hbar, m->x, m->y + m->height - current_fixed_matrix_menu_theme->hbar_height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_fixed_matrix_menu
 * DESCRIPTION
 *  Creates a fixed matrix menu
 * PARAMETERS
 *  m           [IN]        Is the fixed matrix menu object  (pre-allocated)
 *  x           [IN]        Is the left-top corner position
 *  y           [IN]        Is the left-top corner position
 *  width       [IN]        Are the dimensions
 *  height      [IN]        Are the dimensions
 * RETURNS
 *  void
 *****************************************************************************/
/* extern wgui_inline_item wgui_inline_items; */
extern U8 matrix_main_menu_highlight_bitmap_data[];
void gui_create_fixed_matrix_menu(fixed_matrix_menu *m, S32 x, S32 y, S32 width, S32 height)
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
    m->items = NULL;
    m->highlighted_item = 0;
    m->first_displayed_row = 0;
    m->first_displayed_column = 0;
    m->displayed_columns = 0;
    m->displayed_rows = 0;
    m->highlighted_column = 0;
    m->highlighted_row = 0;
    m->item_height = 0;
    m->item_width = 0;
    gui_set_fixed_matrix_menu_current_theme(m);
    gui_create_vertical_scrollbar(
        &m->vbar,
        m->x + m->width - current_fixed_matrix_menu_theme->vbar_width - 1,
        m->y + 2,
        current_fixed_matrix_menu_theme->vbar_width,
        m->height - current_fixed_matrix_menu_theme->hbar_height - 3);
    gui_create_horizontal_scrollbar(
        &m->hbar,
        m->x + 2,
        m->y + m->height - current_fixed_matrix_menu_theme->hbar_height - 1,
        m->width - current_fixed_matrix_menu_theme->vbar_width - 3,
        current_fixed_matrix_menu_theme->hbar_height);
    m->item_highlighted = UI_dummy_function_S32 ;
    m->item_unhighlighted = UI_dummy_function_S32 ;
    m->item_display_function = UI_fixed_menuitem_dummy_display_function;
    m->item_hide_function = UI_fixed_menuitem_dummy_display_function;
    m->item_measure_function = UI_fixed_menuitem_dummy_measure_function;
    m->item_highlight_function = UI_fixed_menuitem_dummy_highlight_function;
    m->item_remove_highlight_function = UI_fixed_menuitem_dummy_remove_highlight_function;
    m->trigger_top_function = UI_dummy_function;
    m->trigger_bottom_function = UI_dummy_function;
    m->last_hilited_x = -1;
    m->last_hilited_y = -1;
    m->last_highlighted_item = -1;
    m->cache_bmp_x1 = 0;
    m->cache_bmp_x2 = 0;
    m->cache_bmp_y1 = 0;
    m->cache_bmp_y2 = 0;

#if defined(__MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__) && defined(__MMI_MATRIX_MAIN_MENU_OPTIMIZE__)
    m->buffer.buf_ptr = opt_mm_buf;
#else 
    m->buffer.buf_ptr = (U8*) & matrix_main_menu_highlight_bitmap_data;
#endif 

#ifdef __MMI_TOUCH_SCREEN__
    m->pen_event_current_selected_callback_function = NULL;
    m->pen_event_default_selected_callback_function = NULL;
    memset(&(m->pen_state), 0, sizeof(gui_matrix_pen_state_struct));
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    m->up_ind_area.x = 0;
    m->up_ind_area.y = 0;
    m->up_ind_area.width = 0;
    m->up_ind_area.height = 0;
    m->up_ind_area.string = NULL;
    m->up_ind_area.img_id = 0;
    m->down_ind_area.x = 0;
    m->down_ind_area.y = 0;
    m->down_ind_area.width = 0;
    m->down_ind_area.height = 0;
    m->down_ind_area.string = NULL;
    m->down_ind_area.img_id = 0;
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
#endif /* __MMI_TOUCH_SCREEN__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_auto_calculate
 * DESCRIPTION
 *  Automatically calculate a fixed matrix menu
 *  (Decides the number of columns / rows to be displayed
 *  and the number of items per row or column)
 *  
 *  Internal function
 * PARAMETERS
 *  m           [IN]        Is the fixed matrix menu object
 *  width       [?]         
 *  height      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_auto_calculate(fixed_matrix_menu *m, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 menu_width, menu_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    if (m->flags & UI_MATRIX_MENU_SHOW_IND_AREA)
    {
        menu_height = y2 - y1 - MMI_BI_DEGREE_MAIN_MENU_ARROW_BAR_HEIGHT + 1;
    }
    else
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
        menu_height = y2 - y1 + 1;
    menu_width = x2 - x1 + 1;

    /* Autocalculate unspecified parameters               */
    if (m->n_rows == 0 && m->n_columns == 0)    /* both rows and columns is not specified by wgui */
    {
        S32 rows, columns, reminders;

        columns = menu_width / m->item_width;
        rows = coolsand_highdivide(m->n_items, columns);
        while ((columns * 2) / (rows * 3) > 1)
        {
            columns--;
            rows = coolsand_highdivide(m->n_items, columns);
        }
        reminders = m->n_items % columns;
        while (reminders != 0)
        {
            if (reminders + rows - 1 > columns - 1)
            {
                break;
            }
            else
            {
                columns--;
                rows = coolsand_highdivide(m->n_items, columns);
                reminders = m->n_items % columns;
            }
        }
        m->n_rows = rows;
        m->n_columns = columns;
        if (m->item_height <= 0)
        {
            m->item_height = menu_height / m->n_rows;
        }
        if (m->item_width <= 0)
        {
            m->item_width = menu_width / m->n_columns;
        }
    }
    else
    {
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

    *width = menu_width;
    *height = menu_height;

}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_auto_configure
 * DESCRIPTION
 *  Automatically configures a fixed matrix menu
 *  (Decides the number of columns / rows to be displayed
 *  and the number of items per row or column)
 *  
 *  Internal function
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_auto_configure(fixed_matrix_menu *m)
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
    if (m->n_items == 0)
    {
        return;
    }

    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

    menu_height = y2 - y1 + 1;
    menu_width = x2 - x1 + 1;

    gui_fixed_matrix_menu_auto_calculate(m, &menu_width, &menu_height);

#ifdef __MMI_MAINLCD_220X176__
    /*The total height( the image of menu item(47 *3 pixel)) is higher 1 pixel than the display region(140 p).*/
    menu_height++;
#endif

    /* Calculate the number of displayed rows and columns    */
    m->displayed_rows = menu_height / m->item_height;
    m->displayed_columns = menu_width / m->item_width;

    if (m->displayed_rows < m->n_rows)
    {
        m->flags |= UI_MATRIX_MENU_SHOW_IND_AREA;
        gui_fixed_matrix_menu_auto_calculate(m, &menu_width, &menu_height);
    }

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

#ifdef __MMI_MAINLCD_220X176__
    /*The total height( the image of menu item(47 *3 pixel)) is higher 1 pixel than the display region(140 p).*/
    menu_height++;
#endif

    /* Recalculate the number of rows that can be displayed  */
    m->displayed_rows = menu_height / m->item_height;
    m->displayed_columns = menu_width / m->item_width;

    /* Fix the number of displayed rows and columns       */
    if (m->displayed_columns > m->n_columns && 
		!(m->n_columns <= 1 && (m->flags & UI_MATRIX_MENU_ALIGN_LEFT_TOP)))
    {
        m->displayed_columns = m->n_columns;
    }
    if (m->displayed_rows > m->n_rows && 
		!(m->n_rows <= 1 && (m->flags & UI_MATRIX_MENU_ALIGN_LEFT_TOP)))
    {
        m->displayed_rows = m->n_rows;
    }
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    else
    {
        if (MMI_current_menu_type == LIST_MATRIX_MENU)
        {
            m->displayed_rows = menu_height / m->item_height;
        }
    }
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 

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
        gui_resize_vertical_scrollbar(&m->vbar, m->vbar.width, m->height);
    }
    if (show_hbar && (!show_vbar))
    {
        gui_resize_horizontal_scrollbar(&m->hbar, m->width, m->hbar.height);
    }
    if (show_vbar && show_hbar)
    {
        gui_resize_vertical_scrollbar(&m->vbar, m->vbar.width, m->height - m->hbar.height);
        gui_resize_horizontal_scrollbar(&m->hbar, m->width - m->vbar.width, m->hbar.height);
    }
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_fixed_matrix_menu_theme->vbar_width, m->y);
    gui_move_horizontal_scrollbar(&m->hbar, m->x, m->y + m->height - current_fixed_matrix_menu_theme->hbar_height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_setup
 * DESCRIPTION
 *  Used to set the number of rows, columns and item dimensions
 * PARAMETERS
 *  m               [IN]        Is the fixed matrix menu object
 *  item_width      [IN]        Is the width of each item  | Any one of these two should be specified
 *  item_height     [IN]        Is the height of each item | Any one of these two should be specified
 *  n_columns       [IN]        Is the number of columns   | and the other can be zero.
 *  n_rows          [IN]        Is the number of rows      | and the other can be zero.
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_setup(fixed_matrix_menu *m, S32 item_width, S32 item_height, S32 n_columns, S32 n_rows)
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
    gui_fixed_matrix_menu_auto_configure(m);
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_fixed_matrix_menu
 * DESCRIPTION
 *  Changes the size of a fixed matrix menu
 * PARAMETERS
 *  m           [IN]        Is the fixed matrix menu object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_fixed_matrix_menu(fixed_matrix_menu *m, S32 width, S32 height)
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
        current_fixed_matrix_menu_theme->vbar_width,
        m->height - current_fixed_matrix_menu_theme->hbar_height);
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_fixed_matrix_menu_theme->vbar_width, m->y);
    gui_resize_horizontal_scrollbar(
        &m->hbar,
        m->width - current_fixed_matrix_menu_theme->vbar_width,
        current_fixed_matrix_menu_theme->hbar_height);
    gui_move_horizontal_scrollbar(&m->hbar, m->x, m->y + m->height - current_fixed_matrix_menu_theme->hbar_height);
    gui_fixed_matrix_menu_auto_configure(m);
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_fixed_matrix_menu
 * DESCRIPTION
 *  Moves the fixed matrix menu to a new location
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 *  x       [IN]        Is the new left-top corner position
 *  y       [IN]        Is the new left-top corner position
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_fixed_matrix_menu(fixed_matrix_menu *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->x = x;
    m->y = y;
    gui_move_vertical_scrollbar(&m->vbar, m->x + m->width - current_fixed_matrix_menu_theme->vbar_width, m->y);
    gui_move_horizontal_scrollbar(&m->hbar, m->x, m->y + m->height - current_fixed_matrix_menu_theme->hbar_height);
    gui_fixed_matrix_menu_auto_configure(m);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_locate_highlighted_item
 * DESCRIPTION
 *  Used to locate the highlighted item in the fixed matrix menu
 *  
 *  Internal function
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_locate_highlighted_item(fixed_matrix_menu *m)
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

    if (m->flags & UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_ROW)
    {
        m->first_displayed_row = m->highlighted_row;
        if (m->first_displayed_row > m->n_rows - m->displayed_rows)
        {
            m->first_displayed_row = m->n_rows - m->displayed_rows;
        }
    }
    else if (m->flags & UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_ROW)
    {
        m->first_displayed_row = m->highlighted_row - m->displayed_rows + 1;
        if (m->first_displayed_row < 0)
        {
            m->first_displayed_row = 0;
        }
    }
    if (m->flags & UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_COL)
    {
        m->first_displayed_column = m->highlighted_column;
        if (m->first_displayed_column > m->n_columns - m->displayed_columns)
        {
            m->first_displayed_column = m->n_columns - m->displayed_columns;
        }
    }
    else if (m->flags & UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_COL)
    {
        m->first_displayed_column = m->highlighted_column - m->displayed_columns + 1;
        if (m->first_displayed_column < 0)
        {
            m->first_displayed_column = 0;
        }
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
 *  gui_fixed_matrix_menu_goto_next_item
 * DESCRIPTION
 *  Highlights the next item
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_next_item(fixed_matrix_menu *m)
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
        if (m->flags & UI_MATRIX_MENU_VERTICAL_TRIGGER)
        {
            if (m->trigger_bottom_function)
            {
                m->trigger_bottom_function();
            }
            return; /* exit point */
        }
        else if (m->flags & UI_MATRIX_MENU_LOOP)
        {
            m->highlighted_item = 0;
        }
        else
        {
            return;
        }
    }
    else
    {
        m->highlighted_item++;
    }
    gui_fixed_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_previous_item
 * DESCRIPTION
 *  Highlights the previous item
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_previous_item(fixed_matrix_menu *m)
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
        if (m->flags & UI_MATRIX_MENU_VERTICAL_TRIGGER)
        {
            if (m->trigger_top_function)
            {
                m->trigger_top_function();
            }
            return; /* exit point */
        }
        else if (m->flags & UI_MATRIX_MENU_LOOP)
        {
            m->highlighted_item = m->n_items - 1;
        }
        else
        {
            return;
        }
    }
    else
    {
        m->highlighted_item--;
    }
    gui_fixed_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_previous_row
 * DESCRIPTION
 *  Highlights an item in the previous row
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_previous_row(fixed_matrix_menu *m)
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

    /* Menu rotation */
    if ((m->flags & UI_MATRIX_MENU_VERTICAL_TRIGGER) && m->highlighted_row <= 0)
    {
        if (m->trigger_top_function)
        {
            m->trigger_top_function();
        }
        return; /* exit point */
    }
    else if (m->highlighted_row <= 0)
    {
        m->highlighted_row = m->n_rows - 1;

        /* matrix mainmenu vertical loop 1<->4<->7<->2<->5<->8... */
        if (m->flags & UI_MATRIX_MENU_VERTICAL_LOOP)
        {
            if (m->highlighted_column == 0)
            {
                m->highlighted_column = m->displayed_columns - 1;
            }
            else
            {
                m->highlighted_column -= 1;
            }
        }

        /* To avoid the blcok cursor go to non-existed item. */
        m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;

        /* To support that a row is not full of items */
    #if 0
    #endif /* 0 */ 

        if (m->highlighted_item > (m->n_items - 1))
        {
            m->highlighted_row -= 1;
            m->highlighted_item -= m->n_columns;
        }
        m->first_displayed_row = m->highlighted_row - m->displayed_rows + 1;
        if (m->first_displayed_row < 0)
        {
            m->first_displayed_row = 0;
        }
    }
    else
    {
        m->highlighted_row--;
        if (m->highlighted_row < m->first_displayed_row)
        {
            m->first_displayed_row = m->highlighted_row;
        }
    }

    m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;
    if (old_highlighted_item != m->highlighted_item)
    {
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_next_row
 * DESCRIPTION
 *  Highlights an item in the next row
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_next_row(fixed_matrix_menu *m)
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

    /* Menu rotation */
    if ((m->flags & UI_MATRIX_MENU_VERTICAL_TRIGGER) && m->highlighted_row >= (m->n_rows - 1))
    {
        if (m->trigger_bottom_function)
        {
            m->trigger_bottom_function();
        }
        return; /* exit point */
    }
    else if (m->highlighted_row >= (m->n_rows - 1))
    {
        m->highlighted_row = 0;
        m->first_displayed_row = 0;

        /* matrix mainmenu vertical loop 1<->4<->7<->2<->5<->8... */
        if (m->flags & UI_MATRIX_MENU_VERTICAL_LOOP)
        {
            m->highlighted_column += 1;
            if (m->highlighted_column >= m->displayed_columns)
            {
                m->highlighted_column = 0;
            }
        }
    }
    else
    {
        m->highlighted_row++;
        if (m->highlighted_row > (m->first_displayed_row + m->displayed_rows - 1))
        {
            m->first_displayed_row = (m->highlighted_row - m->displayed_rows + 1);
            if (m->first_displayed_row > (m->n_rows - m->displayed_rows))
            {
                m->first_displayed_row = (m->n_rows - m->displayed_rows);
            }
        }
    }

    m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;
    /* to support that a row is not full of items */
#if 0
#endif /* 0 */ 
    if (m->highlighted_item > (m->n_items - 1)) /* revert   */
    {
        m->highlighted_row = 0;

        /* matrix mainmenu vertical loop 1<->4<->7<->2<->5<->8... */
        if (m->flags & UI_MATRIX_MENU_VERTICAL_LOOP)
        {
            m->highlighted_column += 1;
            if (m->highlighted_column >= m->displayed_columns)
            {
                m->highlighted_column = 0;
            }
        }
        m->highlighted_item = m->highlighted_column;
        m->first_displayed_row = 0;
    }
    if (old_highlighted_item != m->highlighted_item)
    {
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_row
 * DESCRIPTION
 *  Highlights an row
 *  
 *  Does not redraw the fixed matrix menu
 *  Used in touch screen
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 *  r       [IN]        Row index
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_row(fixed_matrix_menu *m, S32 r)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;
    S32 highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((r < 0) || (r > (m->n_rows - 1)) || (r == m->highlighted_row))
    {
        return;
    }

    highlighted_item = m->highlighted_item;
    highlighted_item += (r - m->highlighted_row) * m->n_columns;
    if (highlighted_item >= m->n_items)
    {
        highlighted_item = m->n_items - 1;
    }
    m->highlighted_item = highlighted_item;

    gui_fixed_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_previous_column
 * DESCRIPTION
 *  Highlights an item in the previous column
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_previous_column(fixed_matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* For menu rotation */
    if (m->highlighted_column <= 0)
    {
        m->highlighted_column = m->n_columns - 1;
        m->first_displayed_column = m->highlighted_column - m->displayed_columns + 1;
    }
    else
    {
        m->highlighted_column--;
        if (m->highlighted_column < m->first_displayed_column)
        {
            m->first_displayed_column = m->highlighted_column;
        }
    }
    m->highlighted_item = (m->highlighted_row * m->n_columns) + m->highlighted_column;
    if (old_highlighted_item != m->highlighted_item)
    {
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_next_column
 * DESCRIPTION
 *  Highlights an item in the next column
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_next_column(fixed_matrix_menu *m)
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

    /* for menu rotation */
    if (m->highlighted_column >= (m->n_columns - 1))
    {
        m->highlighted_column = 0;
        m->first_displayed_column = 0;
    }
    else
    {
        m->highlighted_column++;
        if (m->highlighted_column > (m->first_displayed_column + m->displayed_columns - 1))
        {
            m->first_displayed_column = (m->highlighted_column - m->displayed_columns + 1);
            if (m->first_displayed_column > (m->n_columns - m->displayed_columns))
            {
                m->first_displayed_column = (m->n_columns - m->displayed_columns);
            }
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
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_column
 * DESCRIPTION
 *  Highlights an column
 *  
 *  Does not redraw the fixed matrix menu
 *  Used in touch screen
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 *  c       [IN]        Column index
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_column(fixed_matrix_menu *m, S32 c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;
    S32 highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((c < 0) || (c > (m->n_columns - 1)) || (c == m->highlighted_column))
    {
        return;
    }

    highlighted_item = m->highlighted_item;
    highlighted_item += c - m->highlighted_column;
    if (highlighted_item >= m->n_items)
    {
        highlighted_item = m->n_items - 1;
    }
    m->highlighted_item = highlighted_item;

    gui_fixed_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_first_item
 * DESCRIPTION
 *  Highlights the first item
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_first_item(fixed_matrix_menu *m)
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
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_last_item
 * DESCRIPTION
 *  Highlights the last item
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_last_item(fixed_matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->highlighted_item = m->n_items - 1;
    gui_fixed_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_next_page
 * DESCRIPTION
 *  Highlights an item in the next page
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_next_page(fixed_matrix_menu *m)
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
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_previous_page
 * DESCRIPTION
 *  Highlights an item in the previous page
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_previous_page(fixed_matrix_menu *m)
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
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_goto_item
 * DESCRIPTION
 *  Highlights a particular item
 *  
 *  Does not redraw the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 *  i       [IN]        Is the index of the item to be highlighted (zero based)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_matrix_menu_goto_item(fixed_matrix_menu *m, S32 i)
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
    gui_fixed_matrix_menu_locate_highlighted_item(m);
    if (old_highlighted_item != m->highlighted_item)
    {
        if ((old_highlighted_item >= 0) && (old_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[old_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_reset_fixed_matrix_col_rows
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_reset_fixed_matrix_col_rows(fixed_matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* m->n_columns=0; */
    m->n_rows = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_auto_disable_scrollbar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  
 *****************************************************************************/
S32 gui_fixed_matrix_auto_disable_scrollbar(fixed_matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(m->flags & UI_MATRIX_MENU_DISABLE_SCROLLBAR))
    {
        if (m->flags & UI_MATRIX_MENU_AUTO_DISABLE_SCROLLBAR)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}


#ifdef __MMI_MATIXMENU_WITH_MOVING__
fixed_matrix_menu *g_fixed_matrix_menu;
S32 Temp_xPos = 0;
S32 Temp_yPos = 0;
gdi_image_cache_bmp_struct Changematrix_menu_ptr;
void CSD_UI_show_fixed_matrix_menu_temp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, hilited_x1, hilited_x2, hilited_y1, hilited_y2;
    S32 new_hilited_x1, new_hilited_y1, new_column, new_row;
    S32 xoff, yoff, xoff2, yoff2, width, height;
    S32 first_displayed_item, last_displayed_item;
    U8 show_vbar = 0, show_hbar = 0;
    S32 xPos, xPos2;
    S32 yPos, yPos2;
    S32 vbar_x = 0, vbar_button_x = 0;
    S32 add_indexPos = 0;
     S32 add_step = 2;
	fixed_matrix_menu *m = g_fixed_matrix_menu;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

gui_lock_double_buffer();
    first_displayed_item = m->first_displayed_row * m->n_columns;
    last_displayed_item = first_displayed_item + (m->displayed_rows * m->n_columns);
    width = m->width;
    height = m->height;
    if (m->flags & UI_MATRIX_MENU_SHOW_VERTICAL_SCROLLBAR)
    {
        show_vbar = 1;
    }
    if (m->flags & UI_MATRIX_MENU_SHOW_HORIZONTAL_SCROLLBAR)
    {
        show_hbar = 1;
    }
    if (show_vbar)
    {
        width -= m->vbar.width - 1;
    }
    if (show_hbar)
    {
        height -= m->hbar.height - 1;
    }
    xoff = (width - (m->item_width * m->displayed_columns)) / (m->displayed_columns + 1);
    yoff = (height - (m->item_height * m->displayed_rows)) / (m->displayed_rows + 1);
    new_row = ((m->last_highlighted_item) / m->n_columns);
    new_column = m->last_highlighted_item - (new_row * m->n_rows);

    x1 = m->x;
    y1 = m->y;
    if (show_vbar && r2lMMIFlag)
    {
        x1 += m->vbar.width;
    }

    new_hilited_x1 = x1 + xoff + (new_column - m->first_displayed_column) * (m->item_width + xoff);
    new_hilited_y1 = y1 + yoff + (new_row - m->first_displayed_row) * (m->item_height + yoff);


    if (m->highlighted_item_width > m->item_width)
    {
        xoff2 = (m->highlighted_item_width - m->item_width);
    }
    else
    {
        xoff2 = 0;
    }
    if (m->highlighted_item_height > m->item_height)
    {
        yoff2 = (m->highlighted_item_height - m->item_height);
    }
    else
    {
        yoff2 = 0;
    }


   

    hilited_x1 = x1 + xoff + (m->highlighted_column - m->first_displayed_column) * (m->item_width + xoff);
    hilited_y1 = y1 + yoff + (m->highlighted_row - m->first_displayed_row) * (m->item_height + yoff);
    hilited_x2 = hilited_x1 + m->highlighted_item_width;
    hilited_y2 = hilited_y1 + m->highlighted_item_height;

    xPos = hilited_x1 - (xoff2 >> 1);
    xPos2 = xPos + m->highlighted_item_width - 1;
    yPos = hilited_y1 - (yoff2 >> 1);
    yPos2 = yPos + m->highlighted_item_height - 1;

    if (xPos < m->x)
    {
        xPos = m->x;
        xPos2 = xPos + m->highlighted_item_width - 1;
    }
    if (xPos2 > m->x + m->width - 1)
    {
        xPos2 = m->x + m->width - 1;
        xPos = xPos2 - m->highlighted_item_width + 1;
    }
    if (yPos < m->y)
    {
        yPos = m->y;
        yPos2 = yPos + m->highlighted_item_height - 1;
    }
    if (yPos2 > m->y + m->height - 1)
    {
        yPos2 = m->y + m->height - 1;
        yPos = yPos2 - m->highlighted_item_height + 1;
    }

		if(Changematrix_menu_ptr.buf_ptr == NULL)
			{
					    gdi_layer_push_clip();
					    gdi_layer_set_clip(m->cache_bmp_x1, m->cache_bmp_y1, m->cache_bmp_x2, m->cache_bmp_y2);
					    gdi_image_cache_bmp_draw(m->cache_bmp_x1, m->cache_bmp_y1, &m->buffer);
					    gdi_layer_pop_clip();
					    Changematrix_menu_ptr.height = MMI_fixed_matrix_menu.height;
					    Changematrix_menu_ptr.width = MMI_fixed_matrix_menu.width;
					    Changematrix_menu_ptr.buf_ptr = OslMalloc(Changematrix_menu_ptr.height*Changematrix_menu_ptr.width*2);
					    gdi_layer_push_clip();
					    gdi_layer_set_clip(MMI_fixed_matrix_menu.x,
									        MMI_fixed_matrix_menu.y,
									        MMI_fixed_matrix_menu.x + MMI_fixed_matrix_menu.width - 1,
									        MMI_fixed_matrix_menu.y + MMI_fixed_matrix_menu.height - 1);
					    gdi_image_cache_bmp_get(MMI_fixed_matrix_menu.x,
									        MMI_fixed_matrix_menu.y,
									        MMI_fixed_matrix_menu.x + MMI_fixed_matrix_menu.width - 1,
									        MMI_fixed_matrix_menu.y + MMI_fixed_matrix_menu.height - 1
									        , &Changematrix_menu_ptr);
					    gdi_layer_pop_clip();
			}


			
		    




if(Temp_xPos < xPos)
{
add_indexPos = (xPos - Temp_xPos)/add_step;
			
			if(add_indexPos<= add_step)
				{
					Temp_xPos = xPos;
				}
			else
				{
					Temp_xPos = Temp_xPos+add_indexPos;
				}
}
else if (Temp_xPos > xPos)
{
add_indexPos = (Temp_xPos -xPos)/add_step;
			
			if(add_indexPos<= add_step)
				{
					Temp_xPos = xPos;
				}
			else
				{
					Temp_xPos = Temp_xPos-add_indexPos;
				}
}

if(Temp_yPos < yPos)
{
add_indexPos = (yPos -Temp_yPos)/add_step;
			
			if(add_indexPos<= add_step)
				{
					Temp_yPos = yPos;
				}
			else
				{
					Temp_yPos = Temp_yPos+add_indexPos;
				}
}
else if (Temp_yPos > yPos)
{
add_indexPos = (Temp_yPos -yPos)/add_step;
			
			if(add_indexPos<= add_step)
				{
					Temp_yPos = yPos;
				}
			else
				{
					Temp_yPos = Temp_yPos-add_indexPos;
				}
}

	gdi_layer_push_clip();
	gdi_layer_set_clip(MMI_fixed_matrix_menu.x,
									        MMI_fixed_matrix_menu.y,
									        MMI_fixed_matrix_menu.x + MMI_fixed_matrix_menu.width - 1,
									        MMI_fixed_matrix_menu.y + MMI_fixed_matrix_menu.height - 1);
	gdi_image_cache_bmp_draw(MMI_fixed_matrix_menu.x,MMI_fixed_matrix_menu.y, &Changematrix_menu_ptr);
	gdi_layer_pop_clip();
    m->cache_bmp_x1 = xPos;
    m->cache_bmp_x2 = xPos2;
    m->cache_bmp_y1 = yPos;
    m->cache_bmp_y2 = yPos2;

    m->last_hilited_x = hilited_x1;
    m->last_hilited_y = hilited_y1;
    m->item_display_function(m->items[m->highlighted_item], m->common_item_data, Temp_xPos, Temp_yPos);
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
            m->vbar.x = vbar_x;
            m->vbar.scroll_button.x = vbar_button_x;
        }
        else
        {
            gui_show_vertical_scrollbar(&m->vbar);
        }
    }
    if (show_hbar)
    {
        gui_set_horizontal_scrollbar_range(&m->hbar, m->n_columns);
        gui_set_horizontal_scrollbar_scale(&m->hbar, m->displayed_columns);
        gui_set_horizontal_scrollbar_value(&m->hbar, m->first_displayed_column);
        gui_show_horizontal_scrollbar(&m->hbar);
    }

	
if((Temp_xPos == xPos)&&(Temp_yPos == yPos))
{
	coolsand_UI_cancel_timer(CSD_UI_show_fixed_matrix_menu_temp);
	#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
	CSD_UI_reget_old_window();
	#endif
}
else
{
	coolsand_UI_start_timer(1, CSD_UI_show_fixed_matrix_menu_temp);
		}

gui_unlock_double_buffer();
	 gui_BLT_double_buffer(
        MMI_fixed_matrix_menu.x,
        MMI_fixed_matrix_menu.y,
        MMI_fixed_matrix_menu.x + MMI_fixed_matrix_menu.width - 1,
        MMI_fixed_matrix_menu.y + MMI_fixed_matrix_menu.height - 1);


}

void CSD_UI_exit_matrix_menu_init(void)
{
	if(Changematrix_menu_ptr.buf_ptr != NULL)
		{
			OslMfree(Changematrix_menu_ptr.buf_ptr);
			Changematrix_menu_ptr.buf_ptr = NULL;
		}
	coolsand_UI_cancel_timer(CSD_UI_show_fixed_matrix_menu_temp);
}
void CSD_UI_show_fixed_matrix_menu(fixed_matrix_menu *m)
{
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
if(g_ActiveScreenId == GetActiveScreenId())
#endif
{
	coolsand_UI_cancel_timer(CSD_UI_show_fixed_matrix_menu_temp);
	g_fixed_matrix_menu = m;
	Temp_xPos = m->cache_bmp_x1;
	Temp_yPos = m->cache_bmp_y1;
	
	CSD_UI_show_fixed_matrix_menu_temp();
}
}
#else
/*****************************************************************************
 * FUNCTION
 *  CSD_UI_show_fixed_matrix_menu
 * DESCRIPTION
 *  Displays the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void CSD_UI_show_fixed_matrix_menu(fixed_matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, hilited_x1, hilited_x2, hilited_y1, hilited_y2;
    S32 new_hilited_x1, new_hilited_y1, new_column, new_row;
    S32 xoff, yoff, xoff2, yoff2, width, height;
    S32 first_displayed_item, last_displayed_item;
    U8 show_vbar = 0, show_hbar = 0;
    S32 xPos, xPos2;
    S32 yPos, yPos2;
    S32 vbar_x = 0, vbar_button_x = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    first_displayed_item = m->first_displayed_row * m->n_columns;
    last_displayed_item = first_displayed_item + (m->displayed_rows * m->n_columns);
    width = m->width;
    height = m->height;
    if (m->flags & UI_MATRIX_MENU_SHOW_VERTICAL_SCROLLBAR)
    {
        show_vbar = 1;
    }
    if (m->flags & UI_MATRIX_MENU_SHOW_HORIZONTAL_SCROLLBAR)
    {
        show_hbar = 1;
    }
    if (show_vbar)
    {
        width -= m->vbar.width - 1;
    }
    if (show_hbar)
    {
        height -= m->hbar.height - 1;
    }
    xoff = (width - (m->item_width * m->displayed_columns)) / (m->displayed_columns + 1);
    yoff = (height - (m->item_height * m->displayed_rows)) / (m->displayed_rows + 1);
    new_row = ((m->last_highlighted_item) / m->n_columns);
    new_column = m->last_highlighted_item - (new_row * m->n_rows);

    x1 = m->x;
    y1 = m->y;
    if (show_vbar && r2lMMIFlag)
    {
        x1 += m->vbar.width;
    }

    new_hilited_x1 = x1 + xoff + (new_column - m->first_displayed_column) * (m->item_width + xoff);
    new_hilited_y1 = y1 + yoff + (new_row - m->first_displayed_row) * (m->item_height + yoff);

    /* if ( m->last_highlighted_item<first_displayed_item || m->last_highlighted_item>last_displayed_item ) */
    if ((m->last_hilited_x != new_hilited_x1 && m->last_hilited_y != new_hilited_y1) ||
        (m->highlighted_item - m->last_highlighted_item) >= (m->displayed_columns * m->displayed_rows)
#ifdef __MMI_TOUCH_SCREEN__
        ||(m->pen_state.pen_on_horizontal_scrollbar||m->pen_state.pen_on_vertical_scrollbar)
#endif
        )

    {
        m->last_highlighted_item = -1;
        gui_show_fixed_matrix_menu(m);
        return;
    }
    if (m->highlighted_item_width > m->item_width)
    {
        xoff2 = (m->highlighted_item_width - m->item_width);
    }
    else
    {
        xoff2 = 0;
    }
    if (m->highlighted_item_height > m->item_height)
    {
        yoff2 = (m->highlighted_item_height - m->item_height);
    }
    else
    {
        yoff2 = 0;
    }

#if 0
#endif /* 0 */ 

    gdi_layer_push_clip();
    //gdi_layer_set_clip(m->last_hilited_x-xoff2, yPos,m->last_hilited_x+xoff2+m->highlighted_item_height, yPos2);
    //gdi_image_cache_bmp_draw(m->last_hilited_x-xoff2, yPos, &m->buffer);
    gdi_layer_set_clip(m->cache_bmp_x1, m->cache_bmp_y1, m->cache_bmp_x2, m->cache_bmp_y2);

#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
    MMI_ASSERT((m->cache_bmp_x2 - m->cache_bmp_x1 + 1) * (m->cache_bmp_y2 - m->cache_bmp_y1 +
                                                          1) * gdi_layer_get_bit_per_pixel() >> 3 <=
               OPTIMIZE_MM_BUF_SIZE);
#endif /* __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__ */ 
    gdi_image_cache_bmp_draw(m->cache_bmp_x1, m->cache_bmp_y1, &m->buffer);
    gdi_layer_pop_clip();

    hilited_x1 = x1 + xoff + (m->highlighted_column - m->first_displayed_column) * (m->item_width + xoff);
    hilited_y1 = y1 + yoff + (m->highlighted_row - m->first_displayed_row) * (m->item_height + yoff);
    hilited_x2 = hilited_x1 + m->highlighted_item_width;
    hilited_y2 = hilited_y1 + m->highlighted_item_height;

    xPos = hilited_x1 - (xoff2 >> 1);
    xPos2 = xPos + m->highlighted_item_width - 1;
    yPos = hilited_y1 - (yoff2 >> 1);
    yPos2 = yPos + m->highlighted_item_height - 1;

    if (xPos < m->x)
    {
        xPos = m->x;
        xPos2 = xPos + m->highlighted_item_width - 1;
    }
    if (xPos2 > m->x + m->width - 1)
    {
        xPos2 = m->x + m->width - 1;
        xPos = xPos2 - m->highlighted_item_width + 1;
    }
    if (yPos < m->y)
    {
        yPos = m->y;
        yPos2 = yPos + m->highlighted_item_height - 1;
    }
    if (yPos2 > m->y + m->height - 1)
    {
        yPos2 = m->y + m->height - 1;
        yPos = yPos2 - m->highlighted_item_height + 1;
    }

#if 0
#endif /* 0 */ 

    gdi_layer_push_clip();
    //gdi_layer_set_clip(hilited_x1-xoff2, yPos, hilited_x2+xoff2, yPos2);
    //gdi_image_cache_bmp_get(hilited_x1-xoff2, yPos, hilited_x2+xoff2, yPos2, &m->buffer);
    gdi_layer_set_clip(xPos, yPos, xPos2, yPos2);

#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
    MMI_ASSERT((xPos2 - xPos + 1) * (yPos2 - yPos + 1) * gdi_layer_get_bit_per_pixel() >> 3 <= OPTIMIZE_MM_BUF_SIZE);
#endif 
    gdi_image_cache_bmp_get(xPos, yPos, xPos2, yPos2, &m->buffer);
    gdi_layer_pop_clip();

    m->cache_bmp_x1 = xPos;
    m->cache_bmp_x2 = xPos2;
    m->cache_bmp_y1 = yPos;
    m->cache_bmp_y2 = yPos2;

    m->last_hilited_x = hilited_x1;
    m->last_hilited_y = hilited_y1;
    m->item_display_function(m->items[m->highlighted_item], m->common_item_data, hilited_x1, hilited_y1);
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
            m->vbar.x = vbar_x;
            m->vbar.scroll_button.x = vbar_button_x;
        }
        else
        {
            gui_show_vertical_scrollbar(&m->vbar);
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

#endif

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__


/*****************************************************************************
 * FUNCTION
 *  gui_setup_fixed_matrix_menu_ind_area
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ind_area        [?]         
 *  x               [IN]        
 *  y               [IN]        
 *  string          [?]         
 *  img_id          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_setup_fixed_matrix_menu_ind_area(gui_matrix_ind_area_struct *ind_area, S32 x, S32 y, U8 *string, U16 img_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ind_area->x = x;
    ind_area->y = y;

    ind_area->string = string;
    ind_area->img_id = img_id;
    if (string != NULL && img_id != 0)
    {
        MMI_ASSERT(0);
    }
#if (0)
#endif /* (0) */ 
    ind_area->width = 20;
    ind_area->height = 20;
}

#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_show_fixed_matrix_menu
 * DESCRIPTION
 *  Displays the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
 /* wangrui clean code 20090112*/
// extern U8 __MAINLCD__MAINMENU__MATRIX__MM_BG2_GIF[];
// extern U8 __MAINLCD__MAINMENU__MATRIX__MM_BG1_GIF[];
// extern U8 __MAINLCD__MAINMENU__MATRIX__MM_BG3_GIF[];
  
void gui_show_fixed_matrix_menu(fixed_matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, xoff, yoff, width, height;
    UI_filled_area *f;
    S32 i, j, k, o;
    S32 cx1, cy1, cx2, cy2;
    S32 tx1, ty1, tx2, ty2;
    S32 ix, iy, iwidth, iheight, ix2, iy2;
    S32 vbar_x = 0, vbar_button_x = 0;
    U8 show_vbar = 0, show_hbar = 0;
    S32 highlite_x = 0, highlite_y = 0;  

#ifdef __MMI_MATRIX_MAIN_MENU_OPTIMIZE__
    S32 xoff2, yoff2;
    S32 xPos, xPos2, yPos, yPos2;
#endif /* __MMI_MATRIX_MAIN_MENU_OPTIMIZE__ */ 
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    MMI_BOOL has_previous_page = MMI_FALSE, has_next_page = MMI_FALSE;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_get_clip(&cx1, &cy1, &cx2, &cy2);
    gui_get_text_clip(&tx1, &ty1, &tx2, &ty2);
    	TRACE_GDI_FUNCTION();

    if (m->flags & UI_MATRIX_MENU_SHOW_VERTICAL_SCROLLBAR)
    {
        show_vbar = 1;
    }
    if (m->flags & UI_MATRIX_MENU_SHOW_HORIZONTAL_SCROLLBAR)
    {
        show_hbar = 1;
    }

    if (r2lMMIFlag)
    {
        if (show_vbar)
        {
            x1 = m->x + m->vbar.width;
        }
        else
        {
            x1 = m->x;
        }
        x2 = UI_device_width;
    }
    else
    {
        x1 = m->x;
        x2 = x1 + m->width - 1;
    }

    y1 = m->y;
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    if (m->flags & UI_MATRIX_MENU_SHOW_IND_AREA)
    {
        y2 = y1 + m->height - 1 - MMI_BI_DEGREE_MAIN_MENU_ARROW_BAR_HEIGHT;
    }
    else
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
        y2 = y1 + m->height - 1;

    if (m->flags & UI_LIST_MENU_STATE_FOCUSSED)
    {
        f = m->focussed_filler;
    }
    else
    {
        f = m->normal_filler;
    }
    if (m->flags & UI_MATRIX_MENU_DISABLE_BACKGROUND)
    {
    	TRACE_GDI_FUNCTION();
        gdi_layer_push_clip();
        gdi_layer_set_clip(x1, y1, x2, y2);
        gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
        gdi_layer_pop_clip();
    }
    else
    {
        gui_set_clip(x1, y1, x2, y2);
        if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
        {
            	TRACE_GDI_FUNCTION();

            gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
        #if defined (__MMI_UI_TRANSPARENT_EFFECT__) || defined (__MMI_UI_LIST_HIGHLIGHT_EFFECTS__)
            /* gui_set_transparent_source_layer(dm_get_scr_bg_layer());//should remove */
        #endif 
        }
        else
        {
           
            UI_filled_area tmp_f = *f;
	     TRACE_GDI_FUNCTION();

            /* In current implementation, matrix menu does not support bordered background 
               very well because highlighted item is overlapped with border. */
            tmp_f.flags &= ~(UI_FILLED_AREA_SINGLE_BORDER | UI_FILLED_AREA_DOUBLE_BORDER);
	/* wangrui clean code 20090112*/
//		mmi_trace(g_sw_GDI, "tmp_f->b = 0x%x  0x%x  0x%x  0x%x",(S32)tmp_f.b, 
//			(S32)& __MAINLCD__MAINMENU__MATRIX__MM_BG1_GIF,
//			(S32)& __MAINLCD__MAINMENU__MATRIX__MM_BG2_GIF,
//			(S32)& __MAINLCD__MAINMENU__MATRIX__MM_BG3_GIF);
            gui_draw_filled_area(x1, y1, x2, y2, &tmp_f);
        }
    }
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    /* Start Draw Indication Arrows. */
    if (MMI_current_menu_type == LIST_MATRIX_MENU)
    {
        if (m->displayed_rows < m->n_rows)
        {
            S32 hor_gap = 0;

            has_previous_page = MMI_FALSE;
            has_next_page = MMI_FALSE;
            gdi_layer_push_clip();
            gdi_layer_reset_clip();
            if (m->first_displayed_row > 0) /* previous page exists */
            {
                has_previous_page = MMI_TRUE;
            }
            if ((m->first_displayed_row + m->displayed_rows - 1) < m->n_rows - 1)       /* next page exists */
            {
                has_next_page = MMI_TRUE;
            }
            if (has_previous_page && has_next_page)
            {
                hor_gap = (m->width - MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_IND_BG_SIZE * 2) / 3;
                gui_setup_fixed_matrix_menu_ind_area(
                    &(m->up_ind_area),
                    m->x + hor_gap,
                    y2 + 1,
                    (U8*) get_string(26000),
                    0);
                gui_setup_fixed_matrix_menu_ind_area(
                    &(m->down_ind_area),
                    m->x + (hor_gap * 2) + MMI_BI_DEGREE_MAIN_MENU_ARROW_IND_SIZE,
                    y2 + 1,
                    (U8*) get_string(26004),
                    0);
                gdi_draw_solid_rect(
                    m->x + hor_gap,
                    y2 + 1,
                    m->x + hor_gap + MMI_BI_DEGREE_MAIN_MENU_ARROW_IND_SIZE,
                    y2 + MMI_BI_DEGREE_MAIN_MENU_ARROW_BAR_HEIGHT,
                    GDI_COLOR_GREEN);
                gdi_draw_solid_rect(
                    m->x + (hor_gap * 2) + MMI_BI_DEGREE_MAIN_MENU_ARROW_IND_SIZE,
                    y2 - MMI_BI_DEGREE_MAIN_MENU_ARROW_BAR_HEIGHT + 1,
                    m->x + ((hor_gap + MMI_BI_DEGREE_MAIN_MENU_ARROW_IND_SIZE) * 2),
                    y2,
                    GDI_COLOR_GREEN);
            }
            else if (has_previous_page)
            {
                hor_gap = (m->width - MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_IND_BG_SIZE) >> 1;
                gui_setup_fixed_matrix_menu_ind_area(
                    &(m->up_ind_area),
                    m->x + hor_gap,
                    y2 + 1,
                    (U8*) get_string(26000),
                    0);
                gui_setup_fixed_matrix_menu_ind_area(&(m->down_ind_area), 0, 0, NULL, 0);
                gdi_draw_solid_rect(
                    m->x + hor_gap,
                    y2 + 1,
                    m->x + hor_gap + MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_IND_BG_SIZE,
                    y2 + MMI_BI_DEGREE_MAIN_MENU_ARROW_BAR_HEIGHT /* MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_HEIGHT */ ,
                    GDI_COLOR_GREEN);
            }
            else if (has_next_page)
            {
                hor_gap = (m->width - MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_IND_BG_SIZE) >> 1;
                gui_setup_fixed_matrix_menu_ind_area(
                    &(m->down_ind_area),
                    m->x + hor_gap,
                    y2 + 1,
                    (U8*) get_string(26000),
                    0);
                gui_setup_fixed_matrix_menu_ind_area(&(m->up_ind_area), 0, 0, NULL, 0);
                gdi_draw_solid_rect(
                    m->x + hor_gap,
                    y2 + 1,
                    m->x + hor_gap + MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_IND_BG_SIZE,
                    y2 + MMI_BI_DEGREE_MAIN_MENU_ARROW_BAR_HEIGHT /* MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_HEIGHT */ ,
                    GDI_COLOR_RED);
            }
            gdi_layer_pop_clip();
        }
    }
    /* End Draw Indication Arrows. */
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 

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

    if (r2lMMIFlag)
    {
        if (show_hbar)
        {
            y2 -= m->hbar.height - 1;
        }
    }
    else
    {
        if (show_vbar)
        {
            x2 -= m->vbar.width - 1;
        }
        if (show_hbar)
        {
            y2 -= m->hbar.height - 1;
        }
    }

    width = x2 - x1 + 1;
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    if (m->flags & UI_MATRIX_MENU_SHOW_IND_AREA)
    {
        y2 -= MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_IND_BG_SIZE;
    }
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
    height = y2 - y1 + 1;
    iwidth = m->item_width;
    iheight = m->item_height;

    gui_set_text_clip(x1, y1, x2, y2);
    gui_set_clip(x1, y1, x2, y2);
    /*
     * xoff=(width>>1)-((iwidth*m->displayed_columns)>>1);
     * yoff=(height>>1)-((iheight*m->displayed_rows)>>1);
     */
    xoff = (width - (iwidth * m->displayed_columns)) / (m->displayed_columns + 1);
    yoff = (height - (iheight * m->displayed_rows)) / (m->displayed_rows + 1);
    iy = y1 + yoff;

    for (j = 0; j < m->displayed_rows; j++)
    {
        ix = x1 + xoff;
        o = (m->first_displayed_row + j) * m->n_columns;
        for (i = 0; i < m->displayed_columns; i++)
        {
            k = o + (m->first_displayed_column + i);
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
            gdi_layer_push_clip();
            gui_set_clip(ix, iy, ix2, iy2);
            gui_set_text_clip(ix, iy, ix2, iy2);
            /* show selected item after all other items displayed */
            if (k == m->highlighted_item)
            {
                m->item_remove_highlight_function(m->items[k], m->common_item_data);
                m->item_display_function(m->items[k], m->common_item_data, ix, iy);
                m->item_highlight_function(m->items[k], m->common_item_data);
                highlite_x = ix;
                highlite_y = iy;
            }
            else
            {
                m->item_display_function(m->items[k], m->common_item_data, ix, iy);
            }
            gdi_layer_pop_clip();
            ix += iwidth + xoff;
        }
        iy += iheight + yoff;
    }

    /* EMS menu not optimized in current implementation, but it can be optimized */
    if (MMI_current_menu_type != MATRIX_MENU_EMS)
    {
    #ifdef __MMI_MATRIX_MAIN_MENU_OPTIMIZE__
        if (m->highlighted_item_width > m->item_width)
        {
            xoff2 = (m->highlighted_item_width - m->item_width);
        }
        else
        {
            xoff2 = 0;
        }
        if (m->highlighted_item_height > m->item_height)
        {
            yoff2 = (m->highlighted_item_height - m->item_height);
        }
        else
        {
            yoff2 = 0;
        }

        xPos = highlite_x - (xoff2 >> 1);
        xPos2 = xPos + m->highlighted_item_width - 1;
        yPos = highlite_y - (yoff2 >> 1);
        yPos2 = yPos + m->highlighted_item_height - 1;

        if (xPos < m->x)
        {
            xPos = m->x;
            xPos2 = xPos + m->highlighted_item_width - 1;
        }
        if (xPos2 > m->x + m->width - 1)
        {
            xPos2 = m->x + m->width - 1;
            xPos = xPos2 - m->highlighted_item_width + 1;
        }
        if (yPos < m->y)
        {
            yPos = m->y;
            yPos2 = yPos + m->highlighted_item_height - 1;
        }
        if (yPos2 > m->y + m->height - 1)
        {
            yPos2 = m->y + m->height - 1;
            yPos = yPos2 - m->highlighted_item_height + 1;
        }

    #if 0
    #endif /* 0 */ 

        //if ( highlite_y-yoff2 < y1 )
        //        gdi_image_cache_bmp_get(highlite_x-xoff2, y1, highlite_x+m->highlighted_item_width+xoff2,highlite_y+m->highlighted_item_height+yoff2,&m->buffer);
        //else if ( highlite_y+m->highlighted_item_height+yoff2 > y2 )
        //        gdi_image_cache_bmp_get(highlite_x-xoff2, highlite_y-yoff2, highlite_x+m->highlighted_item_width+xoff2,y2,&m->buffer);          
        //else
        //        gdi_image_cache_bmp_get(highlite_x-xoff2, highlite_y-yoff2, highlite_x+m->highlighted_item_width+xoff2,highlite_y+m->highlighted_item_height+yoff2,&m->buffer);

        gdi_layer_push_clip();
        gdi_layer_set_clip(xPos, yPos, xPos2, yPos2);
    #ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
        MMI_ASSERT((xPos2 - xPos + 1) * (yPos2 - yPos + 1) * gdi_layer_get_bit_per_pixel() >> 3 <=
                   OPTIMIZE_MM_BUF_SIZE);
    #endif /* __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__ */ 
        gdi_image_cache_bmp_get(xPos, yPos, xPos2, yPos2, &m->buffer);
        gdi_layer_pop_clip();

        m->cache_bmp_x1 = xPos;
        m->cache_bmp_x2 = xPos2;
        m->cache_bmp_y1 = yPos;
        m->cache_bmp_y2 = yPos2;
        m->last_hilited_x = highlite_x;
        m->last_hilited_y = highlite_y;
    #endif /* __MMI_MATRIX_MAIN_MENU_OPTIMIZE__ */ 
    }

    /* gui_set_clip(highlite_x,highlite_y,highlite_x+iwidth,highlite_y+iheight); */
    m->item_display_function(m->items[m->highlighted_item], m->common_item_data, highlite_x, highlite_y);
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
            m->vbar.x = vbar_x;
            m->vbar.scroll_button.x = vbar_button_x;
        }
        else
        {
            gui_show_vertical_scrollbar(&m->vbar);
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


/*****************************************************************************
 * FUNCTION
 *  gui_hide_fixed_matrix_menu_highlighted_item
 * DESCRIPTION
 *  Hides the highlighted item
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_hide_fixed_matrix_menu_highlighted_item(fixed_matrix_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, xoff, yoff, width, height;
    S32 i, j, k;
    S32 cx1, cy1, cx2, cy2;
    S32 tx1, ty1, tx2, ty2;
    S32 ix, iy, iwidth, iheight, ix2, iy2;
    U8 show_vbar = 0, show_hbar = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_get_clip(&cx1, &cy1, &cx2, &cy2);
    gui_get_text_clip(&tx1, &ty1, &tx2, &ty2);
    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

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
    x1 += 3;
    x2 -= 3;
    y1 += 3;
    y2 -= 3;
    if (show_vbar)
    {
        x2 -= m->vbar.width - 1;
    }
    if (show_hbar)
    {
        y2 -= m->hbar.height - 1;
    }
    iwidth = m->item_width;
    iheight = m->item_height;
    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    gui_set_text_clip(x1, y1, x2, y2);
    gui_set_clip(x1, y1, x2, y2);

    xoff = (width >> 1) - ((iwidth * m->displayed_columns) >> 1);
    yoff = (height >> 1) - ((iheight * m->displayed_rows) >> 1);
    iy = y1 + yoff;
    for (j = 0; j < m->displayed_rows; j++)
    {
        ix = x1 + xoff;
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
                m->item_hide_function(m->items[k], m->common_item_data, ix, iy);
            }
            ix += iwidth;
        }
        iy += iheight;
    }

    gui_set_clip(cx1, cy1, cx2, cy2);
    gui_set_text_clip(tx1, ty1, tx2, ty2);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_matrix_menu_item_functions
 * DESCRIPTION
 *  Sets the functions used to display generic menuitems
 * PARAMETERS
 *  m                                   [IN]        Is the fixed matrix menu object
 *  item_display_function               [IN]        Is the function used to display an item
 *  item_measure_function               [IN]        Is the function used to measure an item
 *  item_highlight_function             [OUT]       Is the function used to highlight an item
 *  item_remove_highlight_function      [IN]        Is the function used to remove highlight of an item
 *  item_hide_function                  [IN]        Is the function used to hide the menuitem
 *  width(?)                            [OUT]       Height      are the dimensions of the menuitem
 *  item(?)                             [IN]        Is the specific fixed menuitem
 *  x(?)                                [IN]        Position at which the menuitem was displayed
 *  y(?)                                [IN]        Position at which the menuitem was displayed
 *  common_item_data(?)                 [IN]        Common fixed menuitem
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_matrix_menu_item_functions(
        fixed_matrix_menu *m,
        void (*item_display_function) (void *item, void *common_item_data, S32 x, S32 y),
        void (*item_measure_function) (void *item, void *common_item_data, S32 *width, S32 *height),
        void (*item_highlight_function) (void *item, void *common_item_data),
        void (*item_remove_highlight_function) (void *item, void *common_item_data),
        void (*item_hide_function) (void *item, void *common_item_data, S32 x, S32 y))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->item_display_function = item_display_function;
    m->item_measure_function = item_measure_function;
    m->item_highlight_function = item_highlight_function;
    m->item_remove_highlight_function = item_remove_highlight_function;
    m->item_hide_function = item_hide_function;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_matrix_menu_common_item_data
 * DESCRIPTION
 *  Sets the common item data that the list of items should use
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 *  c       [IN]        Is the common item data
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_matrix_menu_common_item_data(fixed_matrix_menu *m, void *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->common_item_data = c;
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_vertical_scroll_by_pen
 * DESCRIPTION
 *  draws the idle screen depending upon the pen event occured
 * PARAMETERS
 *  m                       [IN]        Is matrix menu object's description
 *  first_displayed_row     [IN]        
 *  menu_event              [OUT]       
 *  first_displayed(?)      [IN]        Tells the area of the scrollbar at which pen event has happened
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_fixed_matrix_menu_vertical_scroll_by_pen(
                fixed_matrix_menu *m,
                S32 first_displayed_row,
                gui_matrix_pen_enum *menu_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_displayed_row;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (first_displayed_row > m->highlighted_row)
    {
        m->flags |= UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_ROW;
        gui_fixed_matrix_menu_goto_row(m, first_displayed_row);
        m->flags &= ~UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_ROW;
        *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
    }
    else
    {
        last_displayed_row = first_displayed_row + m->displayed_rows - 1;
        MMI_DBG_ASSERT(last_displayed_row <= m->n_rows - 1);

        if (last_displayed_row < m->highlighted_row)
        {
            m->flags |= UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_ROW;
            gui_fixed_matrix_menu_goto_row(m, last_displayed_row);
            m->flags &= ~UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_ROW;
            *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
        }
        else
        {
            /* Scrolling without changing highlight (this case only  happens with pen support) */
            m->first_displayed_row = first_displayed_row;

            /* Although highlighted item stays the same, we need to stop and restart the animation */
            m->item_remove_highlight_function(m->items[m->highlighted_item], m->common_item_data);
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);

            *menu_event = GUI_MATRIX_PEN_NEED_REDRAW;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_horizontal_scroll_by_pen
 * DESCRIPTION
 *  draws the idle screen depending upon the pen event occured
 * PARAMETERS
 *  m                           [IN]        Is matrix menu object's description
 *  first_displayed_column      [IN]        
 *  menu_event                  [OUT]       
 *  first_displayed(?)          [IN]        Tells the area of the scrollbar at which pen event has happened
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_fixed_matrix_menu_horizontal_scroll_by_pen(
                fixed_matrix_menu *m,
                S32 first_displayed_column,
                gui_matrix_pen_enum *menu_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_displayed_column;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (first_displayed_column > m->highlighted_column)
    {
        m->flags |= UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_COL;
        gui_fixed_matrix_menu_goto_column(m, first_displayed_column);
        m->flags &= ~UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_COL;
        *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
    }
    else
    {
        last_displayed_column = first_displayed_column + m->displayed_columns - 1;
        MMI_DBG_ASSERT(last_displayed_column <= m->n_columns - 1);

        if (last_displayed_column < m->highlighted_column)
        {
            m->flags |= UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_COL;
            gui_fixed_matrix_menu_goto_column(m, last_displayed_column);
            m->flags &= ~UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_COL;
            *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
        }
        else
        {
            /* Scrolling without changing highlight (this case only  happens with pen support) */
            m->first_displayed_column = first_displayed_column;

            /* Although highlighted item stays the same, we need to stop and restart the animation */
            m->item_remove_highlight_function(m->items[m->highlighted_item], m->common_item_data);
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);

            *menu_event = GUI_MATRIX_PEN_NEED_REDRAW;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_handle_pen_position
 * DESCRIPTION
 *  Handle the pen event (down/move/up), go to the menu item, and set the menu event
 * PARAMETERS
 *  m               [IN]        Is matrix menu object's description
 *  x               [IN]        Co-ordinates of the pen down point
 *  y               [IN]        Co-ordinates of the pen down point
 *  pen_event       [IN]        Only MMI_PEN_EVENT_DOWN, MMI_PEN_EVENT_MOVE, MMI_PEN_EVENT_UP is supported
 *  menu_event      [OUT]       Menu event
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_fixed_matrix_menu_handle_pen_position(
                fixed_matrix_menu *m,
                S32 x,
                S32 y,
                mmi_pen_event_type_enum pen_event,
                gui_matrix_pen_enum *menu_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*
     * Because the sampling rate of Pen Move events is not very high on hardware, 
     * * we might "jump" to a menu item aparted from the last highlighted menu item.
     */

    /* FIXME. we do not handle the case that hilighted menu item has bigger size, but it is usually okay */

    S32 xoff, yoff;
    S32 width, height;
    S32 iwidth, iheight;
    S32 new_row, new_column, item_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#define COMPUTE_NEW_ROW()  do {                                            \
      if (y < yoff)                                                           \
         new_row = m->first_displayed_row;                                    \
      else                                                                    \
         new_row = m->first_displayed_row + ((y - yoff) / (iheight + yoff));  \
      if (new_row > m->first_displayed_row + m->displayed_rows - 1)           \
         new_row = m->first_displayed_row + m->displayed_rows - 1;            \
   } while (0)

#define COMPUTE_NEW_COLUMN() do {                                                \
      if (x < xoff)                                                                 \
         new_column = m->first_displayed_column;                                    \
      else                                                                          \
         new_column = m->first_displayed_column + ((x - xoff) / (iwidth + xoff));   \
      if (new_column > m->first_displayed_column + m->displayed_columns - 1)        \
         new_column = m->first_displayed_column + m->displayed_columns - 1;         \
   } while (0)

    y -= m->y;
    x -= m->x;
    iwidth = m->item_width;
    iheight = m->item_height;
    width = m->width;
    height = m->height;
    if (m->flags & UI_MATRIX_MENU_SHOW_VERTICAL_SCROLLBAR)
    {
        width -= m->vbar.width;
    }
    if (m->flags & UI_MATRIX_MENU_SHOW_HORIZONTAL_SCROLLBAR)
    {
        height -= m->hbar.height;
    }
    xoff = (width - (iwidth * m->displayed_columns)) / (m->displayed_columns + 1);
    yoff = (height - (iheight * m->displayed_rows)) / (m->displayed_rows + 1);

    MMI_DBG_ASSERT(pen_event == MMI_PEN_EVENT_DOWN || pen_event == MMI_PEN_EVENT_MOVE || pen_event == MMI_PEN_EVENT_UP);

    /* These asumptions are mandatory */
    MMI_DBG_ASSERT(m->first_displayed_row >= 0 &&
                   m->first_displayed_column >= 0 &&
                   m->first_displayed_row <= m->n_rows - m->displayed_rows &&
                   m->first_displayed_column <= m->n_columns - m->displayed_columns);

    if (y < 0)  /* Scroll up */
    {
        /* drag & scroll only applies to Pen Move event */
        if (pen_event != MMI_PEN_EVENT_MOVE || m->first_displayed_row == 0)
        {
            *menu_event = GUI_MATRIX_PEN_NONE;
        }
        else
        {
            new_row = m->first_displayed_row - 1;

            COMPUTE_NEW_COLUMN();

            item_index = new_row * m->n_columns + new_column;

            m->flags |= UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_ROW;
            gui_fixed_matrix_menu_goto_item(m, item_index);
            m->flags &= ~UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_ROW;

            *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
        }
    }
    else if (x < 0) /* Scroll left */
    {
        /* drag & scroll only applies to Pen Move event */
        if (pen_event != MMI_PEN_EVENT_MOVE || m->first_displayed_column == 0)
        {
            *menu_event = GUI_MATRIX_PEN_NONE;
        }
        else
        {
            new_column = m->first_displayed_column - 1;

            COMPUTE_NEW_ROW();

            item_index = new_row * m->n_columns + new_column;

            m->flags |= UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_COL;
            gui_fixed_matrix_menu_goto_item(m, item_index);
            m->flags &= ~UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_COL;

            *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
        }
    }
    else if (y >= height)   /* Scroll down */
    {
        /* drag & scroll only applies to Pen Move event */
        if (pen_event != MMI_PEN_EVENT_MOVE || m->first_displayed_row == m->n_rows - m->displayed_rows)
        {
            *menu_event = GUI_MATRIX_PEN_NONE;
        }
        else
        {
            new_row = m->first_displayed_row + m->displayed_rows;

            COMPUTE_NEW_COLUMN();

            item_index = new_row * m->n_columns + new_column;

            if (item_index < m->n_items)    /* when n_items is not multiple of displayed_columns */
            {
                m->flags |= UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_ROW;
                gui_fixed_matrix_menu_goto_item(m, item_index);
                m->flags &= ~UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_ROW;

                *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
            }
            else
            {
                *menu_event = GUI_MATRIX_PEN_NONE;
            }
        }
    }
    else if (x >= width)    /* Scroll right */
    {
        /* drag & scroll only applies to Pen Move event */
        if (pen_event != MMI_PEN_EVENT_MOVE || m->first_displayed_column == m->n_columns - m->displayed_columns)
        {
            *menu_event = GUI_MATRIX_PEN_NONE;
        }
        else
        {
            new_column = m->first_displayed_column + m->displayed_columns;

            COMPUTE_NEW_ROW();

            item_index = new_row * m->n_columns + new_column;

            if (item_index < m->n_items)    /* when n_items is not multiple of displayed_columns */
            {
                m->flags |= UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_COL;
                gui_fixed_matrix_menu_goto_item(m, item_index);
                m->flags &= ~UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_COL;

                *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
            }
            else
            {
                *menu_event = GUI_MATRIX_PEN_NONE;
            }
        }

    }
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    else if (pen_event == MMI_PEN_EVENT_DOWN
             && (m->up_ind_area.string != NULL || m->up_ind_area.img_id != 0)
             && ((x + m->x) > m->up_ind_area.x && (x + m->x) < (m->up_ind_area.x + m->up_ind_area.width - 1))
             && ((y + m->y) > m->up_ind_area.y && (y + m->y) < (m->up_ind_area.y + m->up_ind_area.height - 1)))
    {

        new_row = m->first_displayed_row - m->displayed_rows;

        item_index = new_row * m->n_columns + m->first_displayed_column;

        m->flags |= UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_ROW;
        gui_fixed_matrix_menu_goto_item(m, item_index);
        m->flags &= ~UI_MATRIX_MENU_FIRST_SHIFT_HIGHLIGHTED_ROW;

        *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
    }
    else if (pen_event == MMI_PEN_EVENT_DOWN
             && (m->down_ind_area.string != NULL || m->down_ind_area.img_id != 0)
             && ((x + m->x) > m->down_ind_area.x && (x + m->x) < (m->down_ind_area.x + m->down_ind_area.width - 1))
             && ((y + m->y) > m->down_ind_area.y && (y + m->y) < (m->down_ind_area.y + m->down_ind_area.height - 1)))
    {
        new_row = m->first_displayed_row + m->displayed_rows;

        item_index = new_row * m->n_columns + m->first_displayed_column;

        if (item_index < m->n_items)    /* when n_items is not multiple of displayed_columns */
        {
            m->flags |= UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_ROW;
            gui_fixed_matrix_menu_goto_item(m, item_index);
            m->flags &= ~UI_MATRIX_MENU_LAST_SHIFT_HIGHLIGHTED_ROW;

            *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
        }
        else
        {
            *menu_event = GUI_MATRIX_PEN_NONE;
        }
    }
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
    else
    {
        S32 act_x = 0, act_y = 0;

        act_x = xoff * (m->displayed_columns) + (m->displayed_columns) * m->item_width;
        act_y = yoff * (m->displayed_rows) + (m->displayed_rows) * m->item_height;

        if (x > act_x || y > act_y)
        {
            *menu_event = GUI_MATRIX_PEN_NONE;
            return;
        }

        COMPUTE_NEW_ROW();
        COMPUTE_NEW_COLUMN();

        item_index = new_row * m->n_columns + new_column;
        if (item_index < m->n_items && item_index != m->highlighted_item)
        {
            gui_fixed_matrix_menu_goto_item(m, item_index);
            *menu_event = GUI_MATRIX_PEN_HIGHLIGHT_CHANGED;
        }
        else
        {
            if (pen_event == MMI_PEN_EVENT_UP)
            {
                if (m->pen_state.highlight_changed || (item_index >= m->n_items))
                {
                    *menu_event = GUI_MATRIX_PEN_NONE;
                }
                else
                {
                    *menu_event = GUI_MATRIX_PEN_ITEM_SELECTED;
                }
            }
            else
            {
                *menu_event = GUI_MATRIX_PEN_NONE;
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_matrix_menu_translate_pen_event
 * DESCRIPTION
 *  Translate the pen events occured
 *  
 *  horizontal scrolling inside matrix menu is not supported
 * PARAMETERS
 *  m               [IN]        Is matrix menu object's description
 *  pen_event       [IN]        One of the pen events like pen_down,pen_up etc
 *  x               [IN]        Co-ordinates of the current pen position
 *  y               [IN]        Co-ordinates of the current pen position
 *  menu_event      [OUT]       
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL gui_fixed_matrix_menu_translate_pen_event(
        fixed_matrix_menu *m,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_matrix_pen_enum *menu_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    gui_matrix_pen_state_struct *pen_state;
    gui_scrollbar_pen_enum scrollbar_event;
    gui_pen_event_param_struct scrollbar_param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pen_state = &m->pen_state;
    ret = MMI_TRUE;

    *menu_event = GUI_MATRIX_PEN_NONE;

    if (m->flags & UI_MATRIX_MENU_DISABLE_PEN)
    {
        return MMI_FALSE;
    }

    if (pen_event != MMI_PEN_EVENT_DOWN && pen_state->pen_on_vertical_scrollbar)
    {
        gui_vertical_scrollbar_translate_pen_event(&m->vbar, pen_event, x, y, &scrollbar_event, &scrollbar_param);
        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
        {
            gui_fixed_matrix_menu_vertical_scroll_by_pen(m, scrollbar_param._u.i, menu_event);
        }
    }
    else if (pen_event != MMI_PEN_EVENT_DOWN && pen_state->pen_on_horizontal_scrollbar)
    {
        gui_horizontal_scrollbar_translate_pen_event(&m->hbar, pen_event, x, y, &scrollbar_event, &scrollbar_param);
        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
        {
            gui_fixed_matrix_menu_horizontal_scroll_by_pen(m, scrollbar_param._u.i, menu_event);
        }
    }
    else
    {
        switch (pen_event)
        {
            case MMI_PEN_EVENT_DOWN:
                if (PEN_CHECK_BOUND(x, y, m->x, m->y, m->width, m->height))
                {
                    if ((m->flags & UI_MATRIX_MENU_SHOW_VERTICAL_SCROLLBAR) &&
                        gui_vertical_scrollbar_translate_pen_event(
                            &m->vbar,
                            MMI_PEN_EVENT_DOWN,
                            x,
                            y,
                            &scrollbar_event,
                            &scrollbar_param))
                    {
                        pen_state->pen_on_vertical_scrollbar = 1;
                        pen_state->pen_on_horizontal_scrollbar = 0;
                        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
                        {
                            gui_fixed_matrix_menu_vertical_scroll_by_pen(m, scrollbar_param._u.i, menu_event);
                        }
                    }
                    else if ((m->flags & UI_MATRIX_MENU_SHOW_HORIZONTAL_SCROLLBAR) &&
                             gui_horizontal_scrollbar_translate_pen_event(
                                &m->hbar,
                                MMI_PEN_EVENT_DOWN,
                                x,
                                y,
                                &scrollbar_event,
                                &scrollbar_param))
                    {
                        pen_state->pen_on_vertical_scrollbar = 0;
                        pen_state->pen_on_horizontal_scrollbar = 1;
                        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
                        {
                            gui_fixed_matrix_menu_horizontal_scroll_by_pen(m, scrollbar_param._u.i, menu_event);
                        }
                    }
                    else
                    {
                        pen_state->pen_on_vertical_scrollbar = 0;
                        pen_state->pen_on_horizontal_scrollbar = 0;

                        gui_fixed_matrix_menu_handle_pen_position(m, x, y, MMI_PEN_EVENT_DOWN, menu_event);
                    }

                    pen_state->first_highlighed_item = m->highlighted_item;
                    pen_state->highlight_changed = 0;
                }
                else
                {
                    ret = MMI_FALSE;
                }
                break;

            case MMI_PEN_EVENT_MOVE:
                gui_fixed_matrix_menu_handle_pen_position(m, x, y, MMI_PEN_EVENT_MOVE, menu_event);
                break;

            case MMI_PEN_EVENT_UP:
                gui_fixed_matrix_menu_handle_pen_position(m, x, y, MMI_PEN_EVENT_UP, menu_event);
                break;

            case MMI_PEN_EVENT_LONG_TAP:
                /* Do nothing */
                break;

            case MMI_PEN_EVENT_REPEAT:
                /* Do nothing */
                break;

            case MMI_PEN_EVENT_ABORT:
                /* Do nothing */
                break;

            default:
                MMI_ASSERT(0);

        }
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

/* GUI: drop down control functions */
#ifdef MMI_DROPDOWN_CONTROL
UI_drop_down_control_theme *current_drop_down_control_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_drop_down_control_current_theme
 * DESCRIPTION
 *  Sets the current theme of the drop down control
 * PARAMETERS
 *  o           [?]         
 *  m(?)        [IN]        Is the drop down control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_drop_down_control_current_theme(drop_down_control *o)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    o->normal_filler = current_drop_down_control_theme->normal_filler;
    o->disabled_filler = current_drop_down_control_theme->disabled_filler;
    o->focussed_filler = current_drop_down_control_theme->focussed_filler;
    o->clicked_filler = current_drop_down_control_theme->clicked_filler;
    o->normal_text_color = current_drop_down_control_theme->normal_text_color;
    o->disabled_text_color = current_drop_down_control_theme->disabled_text_color;
    o->focussed_text_color = current_drop_down_control_theme->focussed_text_color;
    o->clicked_text_color = current_drop_down_control_theme->clicked_text_color;
    o->text_font = current_drop_down_control_theme->text_font;
    o->flags |= current_drop_down_control_theme->flags;
    gui_set_icon_button_BG_theme(&o->down_button, current_drop_down_control_theme->down_button_theme);
    gui_icon_button_set_icon(
        &o->down_button,
        current_drop_down_control_theme->down_button_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_drop_down_control_theme
 * DESCRIPTION
 *  Sets the given theme of the drop down control
 * PARAMETERS
 *  o           [?]         
 *  t           [IN]        Is the drop down control theme
 *  m(?)        [IN]        Is the drop down control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_drop_down_control_theme(drop_down_control *o, UI_drop_down_control_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    o->normal_filler = t->normal_filler;
    o->disabled_filler = t->disabled_filler;
    o->focussed_filler = t->focussed_filler;
    o->clicked_filler = t->clicked_filler;
    o->normal_text_color = t->normal_text_color;
    o->disabled_text_color = t->disabled_text_color;
    o->focussed_text_color = t->focussed_text_color;
    o->clicked_text_color = t->clicked_text_color;
    o->text_font = t->text_font;
    o->flags |= t->flags;
    gui_set_icon_button_BG_theme(&o->down_button, t->down_button_theme);
    gui_icon_button_set_icon(&o->down_button, t->down_button_icon, UI_DEFAULT_TRANSPARENT_COLOR);
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_drop_down_control
 * DESCRIPTION
 *  Creates a drop down control object
 * PARAMETERS
 *  o           [?]         
 *  x           [IN]        Are the co-ordinates
 *  y           [IN]        Are the co-ordinates
 *  width       [IN]        Are the dimensions
 *  height      [IN]        Are the dimensions
 *  m(?)        [IN]        Is the drop down control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_drop_down_control(drop_down_control *o, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    o->x = x;
    o->y = y;
    o->width = width;
    o->height = height;
    o->flags = 0;
    gui_create_icon_button(&o->down_button, x + width - height + 1, y + 1, height - 2, height - 2, NULL);
    gui_set_drop_down_control_current_theme(o);
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_drop_down_control
 * DESCRIPTION
 *  Resizes a drop down control object
 * PARAMETERS
 *  o           [?]         
 *  width       [IN]        Are the new dimensions
 *  height      [IN]        Are the new dimensions
 *  m(?)        [IN]        Is the drop down control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_drop_down_control(drop_down_control *o, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    o->width = width;
    o->height = height;
    gui_resize_icon_button(&o->down_button, height - 2, height - 2);
    gui_move_icon_button(&o->down_button, o->x + width - height + 1, o->y + 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_drop_down_control
 * DESCRIPTION
 *  Moves a drop down control object
 * PARAMETERS
 *  o           [?]         
 *  x           [IN]        Are the new co-ordinates
 *  y           [IN]        Are the new co-ordinates
 *  m(?)        [IN]        Is the drop down control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_drop_down_control(drop_down_control *o, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    o->x = x;
    o->y = y;
    gui_move_icon_button(&o->down_button, o->x + o->width - o->height + 1, o->y + 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_drop_down_control
 * DESCRIPTION
 *  Displays a drop down control object
 * PARAMETERS
 *  o           [?]         
 *  m(?)        [IN]        Is the drop down control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_drop_down_control(drop_down_control *o)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    UI_filled_area *f;
    color_t text_color;
    S32 tx, ty, text_width, text_height;
    U32 flags = o->flags;
    U8 button_displayed = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (flags & UI_DROP_DOWN_CONTROL_DISABLE_DRAW)
    {
        return;
    }

    x1 = o->x;
    y1 = o->y;
    x2 = x1 + o->width - 1;
    y2 = y1 + o->height - 1;

    gui_push_clip();
    gui_push_text_clip();
    gui_set_clip(x1, y1, x2, y2);
    if (flags & UI_DROP_DOWN_CONTROL_STATE_FOCUSSED)
    {
        f = o->focussed_filler;
        text_color = o->focussed_text_color;
    }
    else if (flags & UI_DROP_DOWN_CONTROL_STATE_CLICKED)
    {
        f = o->clicked_filler;
        text_color = o->clicked_text_color;
    }
    else if (flags & UI_DROP_DOWN_CONTROL_STATE_DISABLED)
    {
        f = o->disabled_filler;
        text_color = o->disabled_text_color;
    }
    else
    {
        f = o->normal_filler;
        text_color = o->normal_text_color;
    }
    if (!(flags & UI_DROP_DOWN_CONTROL_DISABLE_BACKGROUND))
    {
        gui_draw_filled_area(x1, y1, x2, y2, f);
    }
    if (flags & UI_DROP_DOWN_CONTROL_SHOW_BUTTON_ON_FOCUS)
    {
        if ((flags & UI_DROP_DOWN_CONTROL_STATE_FOCUSSED) || (flags & UI_DROP_DOWN_CONTROL_STATE_CLICKED))
        {
            gui_show_icon_button(&o->down_button);
            button_displayed = 1;
        }
    }
    else
    {
        gui_show_icon_button(&o->down_button);
        button_displayed = 1;
    }
    if (o->text != NULL)
    {
        gui_set_font(o->text_font);
        gui_measure_string(o->text, &text_width, &text_height);
        ty = y1 + (o->height >> 1) - (text_height >> 1);
        if (flags & UI_DROP_DOWN_CONTROL_TEXT_RIGHT_JUSTIFY)
        {
            if (button_displayed)
            {
                tx = x2 - 4 - text_width - o->down_button.width;
            }
            else
            {
                tx = x2 - 4 - text_width;
            }
        }
        else if (flags & UI_DROP_DOWN_CONTROL_TEXT_CENTER_JUSTIFY)
        {
            if (button_displayed)
            {
                tx = x1 + ((o->width - o->down_button.width) >> 1) - (text_width >> 1);
            }
            else
            {
                tx = x1 + (o->width >> 1) - (text_width >> 1);
            }
        }
        else
        {
            tx = x1 + 5;
        }

        if (r2lMMIFlag)
        {
            gui_move_text_cursor(tx + text_width, ty);
        }
        else
        {
            gui_move_text_cursor(tx, ty);
        }

        gui_set_text_color(text_color);
        gui_set_line_height(text_height);
        gui_print_text(o->text);
    }

    gui_pop_text_clip();
    gui_pop_clip();
}

#endif //#ifdef MMI_DROPDOWN_CONTROL


/* GUI: horizontal select menu functions              */

UI_horizontal_select_theme *current_horizontal_select_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_select_current_theme
 * DESCRIPTION
 *  Applies the current theme to a horizontal select menu
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_select_current_theme(horizontal_select *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags |= current_horizontal_select_theme->flags;
    m->background_filler = current_horizontal_select_theme->background_filler;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_select_theme
 * DESCRIPTION
 *  Applies the given theme to a horizontal select menu
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 *  t       [IN]        Is the theme
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_select_theme(horizontal_select *m, UI_horizontal_select_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->flags |= t->flags;
    m->background_filler = t->background_filler;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_horizontal_select
 * DESCRIPTION
 *  Creates a horizontal select menu object
 * PARAMETERS
 *  m           [IN]        Is the horizontal select menu object   (pre-allocated)
 *  x           [IN]        Is the left-top corner position
 *  y           [IN]        Is the left-top corner position
 *  width       [IN]        Are the dimensions
 *  height      [IN]        Are the dimensions
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_horizontal_select(horizontal_select *m, S32 x, S32 y, S32 width, S32 height)
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
    m->n_items = 0;
    m->highlighted_item = 0;
    gui_set_horizontal_select_current_theme(m);
    m->clear_background_callback = NULL;
    m->item_highlighted = UI_dummy_function_S32 ;
    m->item_unhighlighted = UI_dummy_function_S32 ;
    m->item_display_function = UI_fixed_menuitem_dummy_display_function;
    m->item_hide_function = UI_fixed_menuitem_dummy_display_function;
    m->item_measure_function = UI_fixed_menuitem_dummy_measure_function;
    m->item_highlight_function = UI_fixed_menuitem_dummy_highlight_function;
    m->item_remove_highlight_function = UI_fixed_menuitem_dummy_remove_highlight_function;
    m->items = NULL;
    m->common_item_data = NULL;
    m->left_arrow_pressed = 0;
    m->right_arrow_pressed = 0;
    m->left_arrow_image = NULL;
    m->right_arrow_image = NULL;
    m->lx = 0;
    m->ly = 0;
    m->rx = 0;
    m->ry = 0;
    m->ix1 = 0;
    m->iy1 = 0;
    m->ix2 = m->width - 1;
    m->iy2 = m->height - 1;
    m->flags = UI_HORIZONTAL_SELECT_NO_ITEM_HIGHLIGHTED;
    m->item_displayed_callback = UI_dummy_menuitem_displayed_function;
#ifdef __MMI_TOUCH_SCREEN__
    memset(&m->pen_state, 0, sizeof(gui_horizontal_select_pen_state_struct));
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_horizontal_select
 * DESCRIPTION
 *  Changes the size of a horizontal select menu
 * PARAMETERS
 *  m           [IN]        Is the horizontal select menu object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_horizontal_select(horizontal_select *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    m->lx = 0;
    m->ly = 0;
    m->rx = 0;
    m->ry = 0;
    m->ix1 = 0;
    m->iy1 = 0;
    m->ix2 = m->width - 1;
    m->iy2 = m->height - 1;

    if (m->left_arrow_image != NULL)
    {
        gui_measure_image(m->left_arrow_image, &iwidth, &iheight);
        m->lx = 2;
        m->ly = (m->height >> 1) - (iheight >> 1);
        m->ix1 = iwidth + 4;
    }
    if (m->right_arrow_image != NULL)
    {
        gui_measure_image(m->right_arrow_image, &iwidth, &iheight);
        m->rx = m->width - iwidth - 2;
        m->ry = (m->height >> 1) - (iheight >> 1);
        m->ix2 = m->width - iwidth - 4;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_horizontal_select
 * DESCRIPTION
 *  Moves the horizontal select menu to a different co-ordinate
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 *  x       [IN]        Is the new left-top corner position
 *  y       [IN]        Is the new left-top corner position
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_horizontal_select(horizontal_select *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->x = x;
    m->y = y;
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_select_goto_item
 * DESCRIPTION
 *  Highlights a particular item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 *  i       [IN]        Is the index of the item to be highlighted (zero based)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_select_goto_item(horizontal_select *m, S32 i)
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
    if (last_highlighted_item != m->highlighted_item)
    {
        if ((last_highlighted_item >= 0) && (last_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[last_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(last_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_select_goto_next_item
 * DESCRIPTION
 *  Higlights the next item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_select_goto_next_item(horizontal_select *m)
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
    if (last_highlighted_item != m->highlighted_item)
    {
        if ((last_highlighted_item >= 0) && (last_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[last_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(last_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_select_goto_previous_item
 * DESCRIPTION
 *  Higlights the previous item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_select_goto_previous_item(horizontal_select *m)
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
    if (last_highlighted_item != m->highlighted_item)
    {
        if ((last_highlighted_item >= 0) && (last_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[last_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(last_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_select_goto_first_item
 * DESCRIPTION
 *  Higlights the first item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_select_goto_first_item(horizontal_select *m)
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
    if (last_highlighted_item != m->highlighted_item)
    {
        if ((last_highlighted_item >= 0) && (last_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[last_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(last_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_select_goto_last_item
 * DESCRIPTION
 *  Higlights the last item
 *  
 *  The fixed list is not redrawn
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_select_goto_last_item(horizontal_select *m)
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
    if (last_highlighted_item != m->highlighted_item)
    {
        if ((last_highlighted_item >= 0) && (last_highlighted_item < m->n_items))
        {
            m->item_remove_highlight_function(m->items[last_highlighted_item], m->common_item_data);
        }
        if ((m->highlighted_item >= 0) && (m->highlighted_item < m->n_items))
        {
            m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
        }
        m->item_unhighlighted(last_highlighted_item);
        m->item_highlighted(m->highlighted_item);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_horizontal_select
 * DESCRIPTION
 *  Displays the horizontal select menu
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_horizontal_select(horizontal_select *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    UI_filled_area *f = m->background_filler;
    S32 i;
    S32 cx1, cy1, cx2, cy2;
    S32 tx1, ty1, tx2, ty2;
    S32 iwidth, iheight;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
    g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_HIGHLIGHT_FOR_HOR_SELECT;
#endif
    gui_get_clip(&cx1, &cy1, &cx2, &cy2);
    gui_get_text_clip(&tx1, &ty1, &tx2, &ty2);
    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;
    gui_set_clip(x1, y1, x2 + 2, y2 + 2);
    if (!(m->flags & UI_LIST_MENU_DISABLE_BACKGROUND))
    {
         
        /* Now the inline item select will be shown with the reduced size of
           highlighted filler, so that only the text is shown as highlighted.
           Hence removing the check, for enabling it for all projects. */
         
        #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        /* pass the modified parameters in case when the Defualt Text Effect is enabled  so that there will be proper gap between the menu items. */
        if(IsInlineItemDefaultTextEnable()) 
        {
        	gui_draw_filled_area(x1 + m->ix1, y1, x1 + m->ix2 - 1, y2, f);
	  }
         else
         #endif  /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */
         {
         	gui_draw_filled_area(x1,y1,x2,y2,f);
	   }
         

    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw)
        {
            /* To draw the grid overlapped by the heilighter (filler) */
            if (y1 == MMI_fixed_list_menu.y + 1)    /* to draw line only for starting position */
            {
                gui_draw_horizontal_line(x1, x2, y1, current_MMI_theme->list_background_filler->border_color);
            }
            gui_draw_horizontal_line(x1, x2, y2, current_MMI_theme->list_background_filler->border_color);
            gui_draw_vertical_line(y1, y2, x2, current_MMI_theme->list_background_filler->border_color);
        }
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    }

    if (m->clear_background_callback)
    {
        m->clear_background_callback(x1, y1, x2, y2);
    }
    
    if ((m->n_items == 0) || (m->highlighted_item < 0) || (m->highlighted_item > (m->n_items - 1)))
    {
        gui_set_clip(cx1, cy1, cx2, cy2);
        gui_set_text_clip(tx1, ty1, tx2, ty2);
        return;
    }
    if (m->flags & UI_HORIZONTAL_SELECT_NO_ITEM_HIGHLIGHTED)
    {
        m->flags &= ~UI_HORIZONTAL_SELECT_NO_ITEM_HIGHLIGHTED;
        m->item_highlight_function(m->items[m->highlighted_item], m->common_item_data);
    }
    if (m->left_arrow_image != NULL)
    {
        gui_show_transparent_image(x1 + m->lx, y1 + m->ly + m->left_arrow_pressed, m->left_arrow_image, 0);
    }
    if (m->right_arrow_image != NULL)
    {
        gui_show_transparent_image(x1 + m->rx, y1 + m->ry + m->right_arrow_pressed, m->right_arrow_image, 0);
    }
    x1 = m->x + m->ix1;
    x2 = m->x + m->ix2;
    y1 = m->y + m->iy1;
    y2 = m->y + m->iy2;
    gui_set_text_clip(x1, y1, x2, y2);
    gui_set_clip(x1, y1, x2, y2);
    i = m->highlighted_item;
    {
        m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
        m->item_display_function(m->items[i], m->common_item_data, x1, y1);
        m->item_displayed_callback(i, x1, y1, x1 + iwidth - 1, y1 + iheight - 1);
    }
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    if (line_draw)
    {
        gui_push_clip();
        gui_set_clip(x1, y1, x2, y2);
        if (y1 == MMI_fixed_list_menu.y + 1)    /* To draw after the Text change. As text background again cross the line */
        {
            gui_draw_horizontal_line(x1, x2, y1, current_MMI_theme->list_background_filler->border_color);
        }
        gui_pop_clip();
    }
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    gui_set_clip(cx1, cy1, cx2, cy2);
    gui_set_text_clip(tx1, ty1, tx2, ty2);
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_redraw_horizontal_select
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_redraw_horizontal_select(horizontal_select *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_show_horizontal_select(m);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(m->x, m->y, m->x + m->width - 1, m->y + m->height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_select_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m                   [?]         
 *  pen_event           [IN]        
 *  x                   [IN]        
 *  y                   [IN]        
 *  select_event        [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gui_horizontal_select_translate_pen_event(
        horizontal_select *m,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_horizontal_select_pen_enum *select_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    S32 in_left_region = 0, in_right_region = 0;
    gui_horizontal_select_pen_state_struct *pen_state;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pen_state = &m->pen_state;
    ret = MMI_TRUE;
    *select_event = GUI_HORIZONTAL_SELECT_PEN_NONE;

    if (PEN_CHECK_BOUND(
            x,
            y,
            m->x + pen_state->left_region_x,
            m->y + pen_state->left_region_y,
            pen_state->left_region_width,
            pen_state->left_region_height))
    {
        in_left_region = 1;
    }
    else if (PEN_CHECK_BOUND(
                x,
                y,
                m->x + pen_state->right_region_x,
                m->y + pen_state->right_region_y,
                pen_state->right_region_width,
                pen_state->right_region_height))
    {
        in_right_region = 1;
    }

    switch (pen_event)
    {
        case MMI_PEN_EVENT_DOWN:
            MMI_DBG_ASSERT(!m->left_arrow_pressed && !m->right_arrow_pressed);
            pen_state->pen_down_left_arrow = 0;
            pen_state->pen_down_right_arrow = 0;
            if (in_left_region)
            {
                pen_state->pen_down_left_arrow = 1;
                m->left_arrow_pressed = 1;
                gui_redraw_horizontal_select(m);
            }
            else if (in_right_region)
            {
                pen_state->pen_down_right_arrow = 1;
                m->right_arrow_pressed = 1;
                gui_redraw_horizontal_select(m);
            }
            else if (!PEN_CHECK_BOUND(x, y, m->x, m->y, m->width, m->height))
            {
                ret = MMI_FALSE;
            }

            break;

        case MMI_PEN_EVENT_UP:
            if (pen_state->pen_down_left_arrow)
            {
                if (in_left_region)
                {
                    *select_event = GUI_HORIZONTAL_SELECT_PEN_PREV;
                }
            }
            else if (pen_state->pen_down_right_arrow)
            {
                if (in_right_region)
                {
                    *select_event = GUI_HORIZONTAL_SELECT_PEN_NEXT;
                }
            }

            if (m->left_arrow_pressed)
            {
                m->left_arrow_pressed = 0;
                gui_redraw_horizontal_select(m);
            }
            else if (m->right_arrow_pressed)
            {
                m->right_arrow_pressed = 0;
                gui_redraw_horizontal_select(m);
            }
            break;

        case MMI_PEN_EVENT_MOVE:
            if (pen_state->pen_down_left_arrow)
            {
                if (in_left_region)
                {
                    if (!m->left_arrow_pressed)
                    {
                        m->left_arrow_pressed = 1;
                        gui_redraw_horizontal_select(m);
                    }
                }
                else
                {
                    if (m->left_arrow_pressed)
                    {
                        m->left_arrow_pressed = 0;
                        gui_redraw_horizontal_select(m);
                    }
                }
            }
            else if (pen_state->pen_down_right_arrow)
            {
                if (in_right_region)
                {
                    if (!m->right_arrow_pressed)
                    {
                        m->right_arrow_pressed = 1;
                        gui_redraw_horizontal_select(m);
                    }
                }
                else
                {
                    if (m->right_arrow_pressed)
                    {
                        m->right_arrow_pressed = 0;
                        gui_redraw_horizontal_select(m);
                    }
                }
            }

            break;

        case MMI_PEN_EVENT_LONG_TAP:
            /* FALLTHROUGH no break */
        case MMI_PEN_EVENT_REPEAT:
            break;

        case MMI_PEN_EVENT_ABORT:
            if (m->left_arrow_pressed)
            {
                m->left_arrow_pressed = 0;
                gui_redraw_horizontal_select(m);
            }
            else if (m->right_arrow_pressed)
            {
                m->right_arrow_pressed = 0;
                gui_redraw_horizontal_select(m);
            }
            break;

        default:
            MMI_ASSERT(0);
    }

    return ret;

}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_hide_horizontal_select_highlighted_item
 * DESCRIPTION
 *  Hides the highlighted item in a horizontal select menu
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_hide_horizontal_select_highlighted_item(horizontal_select *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 i;
    S32 cx1, cy1, cx2, cy2;
    S32 tx1, ty1, tx2, ty2;
    S32 iwidth, iheight;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m->n_items == 0)
    {
        return;
    }
    if ((m->highlighted_item < 0) || (m->highlighted_item > (m->n_items - 1)))
    {
        return;
    }
    gui_get_clip(&cx1, &cy1, &cx2, &cy2);
    gui_get_text_clip(&tx1, &ty1, &tx2, &ty2);
    x1 = m->x;
    y1 = m->y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;
    gui_set_text_clip(x1, y1, x2, y2);
    gui_set_clip(x1, y1, x2, y2);
    i = m->highlighted_item;
    {
        m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
        m->item_hide_function(m->items[i], m->common_item_data, x1, y1);
    }
    gui_set_clip(cx1, cy1, cx2, cy2);
    gui_set_text_clip(tx1, ty1, tx2, ty2);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_select_clear_background_function
 * DESCRIPTION
 *  Sets the function used to clear background, 
 *  The function is typically used when UI_LIST_MENU_DISABLE_BACKGROUND is set.
 * 
 *  (Set 'clear_bg_function' to NULL to disable the callback)
 * PARAMETERS
 *  m                                   [IN]        Is the horizontal select menu object
 *  clear_bg_function                   [IN]        Callback to clear background
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_select_clear_background_function(
        horizontal_select *m,
        void (*clear_bg_function)(S32 x1, S32 y1, S32 x2, S32 y2))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->clear_background_callback = clear_bg_function;
}

/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_select_item_functions
 * DESCRIPTION
 *  Sets the functions used to display generic menuitems
 * PARAMETERS
 *  m                                   [IN]        Is the horizontal select menu object
 *  item_display_function               [IN]        Is the function used to display an item
 *  item_measure_function               [IN]        Is the function used to measure an item
 *  item_highlight_function             [OUT]       Is the function used to highlight an item
 *  item_remove_highlight_function      [IN]        Is the function used to remove highlight of an item
 *  item_hide_function                  [IN]        Is the function used to hide the menuitem
 *  width(?)                            [OUT]       Height      are the dimensions of the menuitem
 *  item(?)                             [IN]        Is the specific fixed menuitem
 *  x(?)                                [IN]        Position at which the menuitem was displayed
 *  y(?)                                [IN]        Position at which the menuitem was displayed
 *  common_item_data(?)                 [IN]        Common fixed menuitem
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_select_item_functions(
        horizontal_select *m,
        void (*item_display_function) (void *item, void *common_item_data, S32 x, S32 y),
        void (*item_measure_function) (void *item, void *common_item_data, S32 *width, S32 *height),
        void (*item_highlight_function) (void *item, void *common_item_data),
        void (*item_remove_highlight_function) (void *item, void *common_item_data),
        void (*item_hide_function) (void *item, void *common_item_data, S32 x, S32 y))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->item_display_function = item_display_function;
    m->item_measure_function = item_measure_function;
    m->item_highlight_function = item_highlight_function;
    m->item_remove_highlight_function = item_remove_highlight_function;
    m->item_hide_function = item_hide_function;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_select_common_item_data
 * DESCRIPTION
 *  Sets the common item data that the list of items should use
 * PARAMETERS
 *  m       [IN]        Is the horizontal select menu object
 *  c       [IN]        Is the common item data
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_select_common_item_data(horizontal_select *m, void *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->common_item_data = c;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_select_images
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m               [?]         
 *  left_arrow      [IN]        
 *  right_arrow     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_select_images(horizontal_select *m, PU8 left_arrow, PU8 right_arrow)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 width, height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->left_arrow_image = left_arrow;
    m->right_arrow_image = right_arrow;
    if (m->left_arrow_image != NULL)
    {
        gui_measure_image(m->left_arrow_image, &width, &height);

    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        m->lx = 0;
        m->ix1 = width + 2;
    #else /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
        m->lx = 2;
        m->ly = (m->height >> 1) - (height >> 1);
        m->ix1 = width + 4;
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
        m->ly = (m->height >> 1) - (height >> 1);

    #ifdef __MMI_TOUCH_SCREEN__
        m->pen_state.left_region_x = m->lx - GUI_HORIZONTAL_SELECT_PEN_VALID_REGION;
        if (m->pen_state.left_region_x < 0)
        {
            m->pen_state.left_region_x = 0;
        }
        m->pen_state.left_region_y = m->ly - GUI_HORIZONTAL_SELECT_PEN_VALID_REGION;
        if (m->pen_state.left_region_y < 0)
        {
            m->pen_state.left_region_y = 0;
        }
        m->pen_state.left_region_width = width + (GUI_HORIZONTAL_SELECT_PEN_VALID_REGION * 2);
        if (m->pen_state.left_region_x + m->pen_state.left_region_width >= (m->width >> 1))
        {
            m->pen_state.left_region_width = (m->width >> 1) - m->pen_state.left_region_x - 1;
        }
        m->pen_state.left_region_height = height + (GUI_HORIZONTAL_SELECT_PEN_VALID_REGION * 2);
        if (m->pen_state.left_region_y + m->pen_state.left_region_height >= m->height)
        {
            m->pen_state.left_region_height = m->height - m->pen_state.left_region_y - 1;
        }
    #endif /* __MMI_TOUCH_SCREEN__ */ 
    }
    if (m->right_arrow_image != NULL)
    {
        gui_measure_image(m->right_arrow_image, &width, &height);

    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        m->rx = m->width - width;
        m->ix2 = m->width - width - 2;
    #else /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
        m->rx = m->width - width - 2;
        m->ix2 = m->width - width - 4;
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 

        m->ry = (m->height >> 1) - (height >> 1);

    #ifdef __MMI_TOUCH_SCREEN__
        m->pen_state.right_region_x = m->rx - GUI_HORIZONTAL_SELECT_PEN_VALID_REGION;
        if (m->pen_state.right_region_x < (m->width >> 1))
        {
            m->pen_state.right_region_x = (m->width >> 1);
        }
        m->pen_state.right_region_y = m->ry - GUI_HORIZONTAL_SELECT_PEN_VALID_REGION;
        if (m->pen_state.right_region_y < 0)
        {
            m->pen_state.right_region_y = 0;
        }
        m->pen_state.right_region_width = width + (GUI_HORIZONTAL_SELECT_PEN_VALID_REGION * 2);
        if (m->pen_state.right_region_x + m->pen_state.right_region_width >= m->width)
        {
            m->pen_state.right_region_width = m->width - m->pen_state.right_region_x - 1;
        }
        m->pen_state.right_region_height = height + (GUI_HORIZONTAL_SELECT_PEN_VALID_REGION * 2);
        if (m->pen_state.right_region_y + m->pen_state.right_region_height >= m->height)
        {
            m->pen_state.right_region_height = m->height - m->pen_state.right_region_y - 1;
        }
    #endif /* __MMI_TOUCH_SCREEN__ */ 
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_menu_bg_id
 * DESCRIPTION
 *  set a specific image as fixed menu background instead of wallpaper
 * PARAMETERS
 *  image_id        [IN]        Image id
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_menu_bg_id(S32 image_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_fixed_menu_bg_id = image_id;
}


/*****************************************************************************
 * FUNCTION
 *  gui_reset_fiexed_menu_bg_id
 * DESCRIPTION
 *  reset a fixed menu background to default
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_reset_fiexed_menu_bg_id(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_fixed_menu_bg_id = -1;
}

#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
#include"idleappdef.h"
extern void DisplayIdleScreen(void);
/*****************************************************************************
 * FUNCTION
 *  CSD_UI_show_fixed_matrix_menu
 * DESCRIPTION
 *  Displays the fixed matrix menu
 * PARAMETERS
 *  m       [IN]        Is the fixed matrix menu object
 * RETURNS
 *  void
 *****************************************************************************/
void CSD_UI_cache_bmp_get(S32 x1, S32 y1, S32 x2, S32 y2, gdi_image_cache_bmp_struct *oldbmp_ptr,gdi_image_cache_bmp_struct *bmp_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    S32 i;
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    U8 *bmp_data_ptr;
    S32 U8_per_row;

    /* get src and dest buffer ptr */
    bmp_data_ptr = bmp_ptr->buf_ptr;            /* dest */
    layer_buf_ptr =  oldbmp_ptr->buf_ptr; /* src */
    layer_clip_x1 = 0;
    layer_clip_y1 = 0;
    layer_clip_x2 = oldbmp_ptr->width - 1;
    layer_clip_y2 = oldbmp_ptr->height - 1;
    layer_width = oldbmp_ptr->width;
    layer_height = oldbmp_ptr->height;

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        bmp_ptr->width = 0;
        bmp_ptr->height = 0;
        return;
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }

    U8_per_row = (bmp_ptr->width * GDI_LAYER.bits_per_pixel) >> 3;

    /* find source buffer start position of the active layer */
    layer_buf_ptr = oldbmp_ptr->buf_ptr +(((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3);       /* 16-bit */

if((bmp_ptr->width == LCD_WIDTH)&&(oldbmp_ptr->width == LCD_WIDTH)&&((x2 - x1 + 1)==LCD_WIDTH))
{
	memcpy(bmp_data_ptr, layer_buf_ptr, U8_per_row*bmp_ptr->height);
	return;
}

    /* fill bitmap buffer */
    for (i = 0; i < bmp_ptr->height; i++)
    {
        memcpy(bmp_data_ptr, layer_buf_ptr, U8_per_row);

        bmp_data_ptr =bmp_ptr->buf_ptr +i* U8_per_row;
        layer_buf_ptr = oldbmp_ptr->buf_ptr +((((i+start_y)*layer_width + start_x)* GDI_LAYER.bits_per_pixel) >> 3); /* just to next row */
    }

}

void CSD_UI_exchangeWin_bmp_ptr(gdi_image_cache_bmp_struct *oldbmp_ptr,gdi_image_cache_bmp_struct *newbmp_ptr,gdi_image_cache_bmp_struct *changebmp_ptr, U16 exchange_step)
{
	gdi_image_cache_bmp_struct *oldbmpptr = oldbmp_ptr;
	gdi_image_cache_bmp_struct *newbmpptr = newbmp_ptr;
	gdi_image_cache_bmp_struct *changebmpptr = changebmp_ptr;
	U16 exchangeStep = exchange_step;

	S32 tempxPos, tempyPos;
	S32 U8_per_point;
	U8 *layer_buf_ptr;
    	U8 *bmp_data_ptr;
	U8 *change_data_ptr;
	U16 temp_exchange_step;
	U32 TEMPy_changebmpptr = 0;	
	U32 TEMPy_newbmpptr = 0;	
	U32 TEMPy_oldbmpptr = 0;

	U8_per_point = (1* GDI_LAYER.bits_per_pixel) >> 3;
	bmp_data_ptr = newbmpptr->buf_ptr;
	layer_buf_ptr = oldbmpptr->buf_ptr;
	change_data_ptr = changebmpptr->buf_ptr;
	temp_exchange_step = exchangeStep;
	if(exchangeStep == 0)
		{
		memcpy(change_data_ptr, bmp_data_ptr, GDI_LCD_WIDTH * GDI_LCD_HEIGHT*2);
		return;
		}

for(tempyPos = 0;tempyPos < newbmpptr->height;tempyPos++)
		{
		TEMPy_changebmpptr = tempyPos * ((changebmpptr->width* GDI_LAYER.bits_per_pixel) >> 3);	
		TEMPy_newbmpptr =  tempyPos * ((newbmpptr->width* GDI_LAYER.bits_per_pixel) >> 3);	
	 	TEMPy_oldbmpptr = tempyPos * ((oldbmpptr->width * GDI_LAYER.bits_per_pixel) >> 3) ;
		for(tempxPos= 0;tempxPos < newbmpptr->width;)
			{
			if(temp_exchange_step)
				{
				memcpy(change_data_ptr, layer_buf_ptr, U8_per_point*exchangeStep);
				tempxPos = tempxPos + exchangeStep;
				temp_exchange_step = 0;
				}
			else
				{
				memcpy(change_data_ptr, bmp_data_ptr, U8_per_point);
				temp_exchange_step = exchangeStep;
				tempxPos++;
				}

				//temp_exchange_step--;
				change_data_ptr = changebmpptr->buf_ptr + TEMPy_changebmpptr + ((tempxPos * GDI_LAYER.bits_per_pixel) >> 3);;
				bmp_data_ptr = newbmpptr->buf_ptr + TEMPy_newbmpptr + ((tempxPos * GDI_LAYER.bits_per_pixel) >> 3);;
      				layer_buf_ptr  = oldbmpptr->buf_ptr + TEMPy_oldbmpptr + ((tempxPos * GDI_LAYER.bits_per_pixel) >> 3); /* just to next row */

					}
			}
		
}


void CSD_UI_change_bmp_ptr(gdi_image_cache_bmp_struct *oldbmp_ptr,gdi_image_cache_bmp_struct *newbmp_ptr)
{
	gdi_image_cache_bmp_struct *oldbmpptr = oldbmp_ptr;
	gdi_image_cache_bmp_struct *newbmpptr = newbmp_ptr;
	S32 xPos, yPos;
	S32 tempxPos, tempyPos;
	S32 addnojump_xPos = 0;
	S32 addnojump_yPos = 0;
	S32 add_xPos = 0;
	S32 add_yPos = 0;
	S32 add_xPos_ex = 0;
	S32 add_yPos_ex = 0;
	S32 addjump_xPos_ex = 0;
	S32 addjump_yPos_ex = 0;
	U8 width_equal_flg = 0;
	U8 height_equal_flg = 0;
	S32 U8_per_point;
	U8 *layer_buf_ptr;
    	U8 *bmp_data_ptr;
	U32 TEMPy_newbmpptr = 0;	
	U32 TEMPy_oldbmpptr = 0;	

	U8_per_point = (1* GDI_LAYER.bits_per_pixel) >> 3;
	bmp_data_ptr = newbmpptr->buf_ptr;
	layer_buf_ptr = oldbmpptr->buf_ptr;
	
if(oldbmpptr->width ==  newbmpptr->width)
{
	width_equal_flg = 1;
}
else if((oldbmpptr->width - newbmpptr->width) < newbmpptr->width)
{
	addnojump_xPos = newbmpptr->width/(oldbmpptr->width - newbmpptr->width);
}
else
{
	add_xPos = oldbmpptr->width/newbmpptr->width;
	add_xPos_ex = oldbmpptr->width%newbmpptr->width;
	if(add_xPos_ex != 0)
		addjump_xPos_ex = newbmpptr->width/add_xPos_ex;
}
if(oldbmpptr->height==newbmpptr->height)
{
	height_equal_flg = 1;
}
else if((oldbmpptr->height-newbmpptr->height) < newbmpptr->height)
{
	addnojump_yPos = newbmpptr->height/(oldbmpptr->height - newbmpptr->height);
}
else
{
	add_yPos = oldbmpptr->height/newbmpptr->height;
	add_yPos_ex = oldbmpptr->height%newbmpptr->height;
	if(add_yPos_ex != 0)
		addjump_yPos_ex = newbmpptr->width/add_yPos_ex;
}

for(yPos = 0,tempyPos = 0;tempyPos < newbmpptr->height;tempyPos++)
		{
		TEMPy_newbmpptr = tempyPos * ((newbmpptr->width* GDI_LAYER.bits_per_pixel) >> 3);
		TEMPy_oldbmpptr = yPos * ((oldbmpptr->width * GDI_LAYER.bits_per_pixel) >> 3);
		for(xPos = 0,tempxPos= 0;tempxPos < newbmpptr->width;tempxPos++)
			{
				if(width_equal_flg)
					{
					bmp_data_ptr = newbmpptr->buf_ptr + TEMPy_newbmpptr;
	      			layer_buf_ptr  = oldbmpptr->buf_ptr + TEMPy_oldbmpptr; /* just to next row */
					memcpy(bmp_data_ptr, layer_buf_ptr, ((newbmpptr->width* GDI_LAYER.bits_per_pixel) >> 3));
					break;
					}
				else
					{
				memcpy(bmp_data_ptr, layer_buf_ptr, U8_per_point);
				bmp_data_ptr = newbmpptr->buf_ptr + TEMPy_newbmpptr + ((tempxPos * GDI_LAYER.bits_per_pixel) >> 3);;
      				layer_buf_ptr  = oldbmpptr->buf_ptr + TEMPy_oldbmpptr + ((xPos * GDI_LAYER.bits_per_pixel) >> 3); /* just to next row */

				if(addnojump_xPos)
					{
						xPos++;
						addnojump_xPos--;
					}
				else
					{
					if(add_xPos)
						{
						if(add_xPos_ex)
							{
							if(addjump_xPos_ex)
								{
								addjump_xPos_ex--;
								xPos = xPos + add_xPos;
								}
							else
								{
								xPos++;
								addjump_xPos_ex = newbmpptr->width/add_xPos_ex;
								}
							}
						else
							{
							xPos = xPos + add_xPos;
							}
						}
					else
						{
					xPos = xPos + 2;
					addnojump_xPos = newbmpptr->width/(oldbmpptr->width - newbmpptr->width);
						}
					}
					}
			}
		if(height_equal_flg)
			{
			yPos++;
			}
		else
			{
		if(addnojump_yPos)
					{
						yPos++;
						addnojump_yPos--;
					}
				else
					{
					if(add_yPos)
						{
						if(add_yPos_ex)
							{
							if(addjump_yPos_ex)
								{
								addjump_yPos_ex--;
								yPos = yPos + add_yPos;
								}
							else
								{
								yPos++;
								addjump_yPos_ex = newbmpptr->height/add_yPos_ex;
								}
							}
						else
							{
							yPos = yPos + add_yPos;
							}
						}
					else
						{
					yPos = yPos + 2;
					addnojump_yPos = newbmpptr->height/(oldbmpptr->height - newbmpptr->height);
						}
					}
			}
		

}



}

/****************************************************************************
* Cache Bitmap
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_get
 * DESCRIPTION
 *  cache a region from a layer
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 *  bmp_ptr     [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
 extern void idle_screen_redraw(void);
#if defined(__PRJ_WITH_SPILCD__)
#ifdef CHIP_HAS_GOUDA
#else
 extern U16			simulator_Merge_buffer[GDI_LCD_WIDTH * GDI_LCD_HEIGHT];
#endif
#else
 U16*		simulator_Merge_buffer ;
#endif
extern GDI_RESULT gdi_Merge_layer_previous(S32 x1, S32 y1, S32 x2, S32 y2);
extern GDI_RESULT gdi_image_cache_bmp_get_win(gdi_image_cache_bmp_struct *bmp_ptr);
extern VOID layerMerge_2buffer(UINT16*buffer,UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy);
void gdi_image_layer_bmp_get(gdi_image_cache_bmp_struct *bmp_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/ 
#ifdef MMI_ON_HARDWARE_P
#ifdef __PRJ_WITH_SPILCD__
#ifdef CHIP_HAS_GOUDA
        layerMerge_2buffer((UINT16*)bmp_ptr->buf_ptr,0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
#else
      memcpy(bmp_ptr->buf_ptr, simulator_Merge_buffer, GDI_LCD_WIDTH * GDI_LCD_HEIGHT*2);
#endif
#else
	simulator_Merge_buffer = (U16*)bmp_ptr->buf_ptr;
	gdi_Merge_layer_previous(0,0,LCD_WIDTH-1,LCD_HEIGHT-1); 
#endif  
#else
	simulator_Merge_buffer = (U16*)bmp_ptr->buf_ptr;
	gdi_Merge_layer_previous(0,0,LCD_WIDTH-1,LCD_HEIGHT-1); 
#endif
  return;
}
U8 enablechange_window_flag = 0;
U8 enablechange_alloc_ok_flag = 0;
U8 arrived_idle_enablechange_window_flag = 0;
U8 change_window_style;

gdi_image_cache_bmp_struct oldWinBmpbuffer;
gdi_image_cache_bmp_struct newWinBmpbuffer;
gdi_image_cache_bmp_struct ChangeTbmp_ptr;
//	U8 newbuf_ptr[LCD_HEIGHT*LCD_WIDTH*2];
//	U8 oldbuf_ptr[LCD_HEIGHT*LCD_WIDTH*2];
#ifdef APPMEM_SUPPORT
extern unsigned long CSD_AllocMediaMemory(unsigned long clicks);
extern void CSD_FreeMediaMemory(unsigned long base);
void CSD_UI_Free_bmpbuf()
{
if(oldWinBmpbuffer.buf_ptr != NULL)
	CSD_FreeMediaMemory((unsigned long)oldWinBmpbuffer.buf_ptr);

if(newWinBmpbuffer.buf_ptr != NULL)
	CSD_FreeMediaMemory((unsigned long)newWinBmpbuffer.buf_ptr);
	
	oldWinBmpbuffer.buf_ptr = NULL;
       newWinBmpbuffer.buf_ptr = NULL;
	enablechange_alloc_ok_flag = 0;
}
void CSD_UI_Alloc_bmpbuf()
{

if(oldWinBmpbuffer.buf_ptr == NULL)
	oldWinBmpbuffer.buf_ptr =(U8 *) CSD_AllocMediaMemory(LCD_HEIGHT*LCD_WIDTH*2);

if(newWinBmpbuffer.buf_ptr == NULL)
       newWinBmpbuffer.buf_ptr = (U8 *)CSD_AllocMediaMemory(LCD_HEIGHT*LCD_WIDTH*2);

if((oldWinBmpbuffer.buf_ptr != NULL)&&(newWinBmpbuffer.buf_ptr != NULL))
{
		 enablechange_alloc_ok_flag = 1;
}
else
{
		enablechange_alloc_ok_flag = 0;
		CSD_UI_Free_bmpbuf();
}
}


#ifdef MMI_ON_WIN32
extern void InitAllocMediaMemory(void);
#endif
#endif

void CSD_UI_init_bmpbuf()
{
#ifdef MMI_ON_WIN32
InitAllocMediaMemory();
#endif
    oldWinBmpbuffer.height = LCD_HEIGHT;
    oldWinBmpbuffer.width = LCD_WIDTH;
    
    newWinBmpbuffer.height = LCD_HEIGHT;
    newWinBmpbuffer.width = LCD_WIDTH;
#ifdef APPMEM_SUPPORT
	CSD_UI_Alloc_bmpbuf();
#endif
//	oldWinBmpbuffer.buf_ptr = oldbuf_ptr;
 //   newWinBmpbuffer.buf_ptr = newbuf_ptr;
   
arrived_idle_enablechange_window_flag = 0;

	//   ChangeTbmp_ptr.buf_ptr = changebuf_ptr;
}




extern U16 GetActiveScreenId (void);
U16 g_OldActiveScreenId;
void CSD_UI_get_old_window()
{
    CSD_UI_Alloc_bmpbuf();
if(enablechange_alloc_ok_flag)
{
    g_OldActiveScreenId = GetActiveScreenId();
    gdi_layer_push_clip();
    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
   // gdi_image_cache_bmp_get(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, &oldWinBmpbuffer);
   gdi_image_layer_bmp_get(&oldWinBmpbuffer);
    gdi_layer_pop_clip();
}
   arrived_idle_enablechange_window_flag = 1;
}

void CSD_UI_reget_old_window()
{
	CSD_UI_Alloc_bmpbuf();
if(enablechange_alloc_ok_flag)
{
    g_OldActiveScreenId = GetActiveScreenId();

    gdi_layer_push_clip();
    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
  //  gdi_image_cache_bmp_get(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, &oldWinBmpbuffer);
  gdi_image_layer_bmp_get(&oldWinBmpbuffer);
    gdi_layer_pop_clip();
}
}
void CSD_UI_get_new_window()
{
CSD_UI_Alloc_bmpbuf();
if(enablechange_alloc_ok_flag)
{
    g_ActiveScreenId = GetActiveScreenId();
#if 0
if(GetActiveScreenId() == IDLE_SCREEN_ID)
{
	gdi_layer_push_clip();
    	gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
   	gdi_image_cache_bmp_get_win(&newWinBmpbuffer);
    	gdi_layer_pop_clip();

	
}
else
#endif
{
    gdi_layer_push_clip();
   gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
//    gdi_image_cache_bmp_get(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, &newWinBmpbuffer);
	gdi_image_layer_bmp_get(&newWinBmpbuffer);
    gdi_layer_pop_clip();
}
}
}

#ifdef    LCDSIZE_240_320
#define CHANGE_WINSTEP 7
#endif

#ifdef    LCDSIZE_176_220
#define CHANGE_WINSTEP 10
#endif

#ifdef    LCDSIZE_220_176
#define CHANGE_WINSTEP 10
#endif

#ifdef    LCDSIZE_160_128
#define CHANGE_WINSTEP 15
#endif

#ifdef    LCDSIZE_128_160
#define CHANGE_WINSTEP 15
#endif
     
#ifdef    LCDSIZE_128_128
#define CHANGE_WINSTEP 15
#endif

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#ifdef    LCDSIZE_320_240
#define CHANGE_WINSTEP 10
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#define CHANGE_SPEED 50
U8 change_win_step = CHANGE_WINSTEP;
U8 change_window_flag = 0;

void CSD_UI_right_moving_window_action(S32 right_offset_x,S32 x,S32 y,S32 w,S32 h)
{
       
gui_lock_double_buffer();

   ChangeTbmp_ptr.height = h;
    ChangeTbmp_ptr.width = right_offset_x;
if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH -ChangeTbmp_ptr.width,y,LCD_WIDTH-1,y + h - 1,&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,y,ChangeTbmp_ptr.width - 1,y + h - 1);
	    gdi_image_cache_bmp_draw(0,y, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
		
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
	}


    ChangeTbmp_ptr.height = h;
    ChangeTbmp_ptr.width =LCD_WIDTH -  right_offset_x;
    if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_cache_bmp_get(0,y,ChangeTbmp_ptr.width-1,y + h - 1,&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(right_offset_x,y,LCD_WIDTH - 1,y + h - 1);
	    gdi_image_cache_bmp_draw(right_offset_x,y, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}



 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

}

void CSD_UI_change_window_action_small(S32 small_w,S32 small_h,S32 x,S32 y,S32 w,S32 h)
{


gui_lock_double_buffer();
    gdi_draw_solid_rect(0, 0, 240, 320, GDI_COLOR_TRANSPARENT);
    ChangeTbmp_ptr.height = h - small_w;
    ChangeTbmp_ptr.width =w -  small_w;
    if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_change_bmp_ptr(&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 
}


void CSD_UI_left_moving_window_action(S32 left_offset_x,S32 x,S32 y,S32 w,S32 h)
{
      S32 temp_left_offset; 
gui_lock_double_buffer();
   
   temp_left_offset= LCD_WIDTH -  left_offset_x;
   ChangeTbmp_ptr.height = h;
    ChangeTbmp_ptr.width = temp_left_offset;
if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH -ChangeTbmp_ptr.width,y,LCD_WIDTH-1,y + h - 1,&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,y,ChangeTbmp_ptr.width - 1,y + h - 1);
	    gdi_image_cache_bmp_draw(0,y, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
		
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
}


    ChangeTbmp_ptr.height = h;
    ChangeTbmp_ptr.width =LCD_WIDTH -  temp_left_offset;
    if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_cache_bmp_get(0,y,ChangeTbmp_ptr.width-1,y + h - 1,&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(temp_left_offset,y,LCD_WIDTH - 1,y + h - 1);
	    gdi_image_cache_bmp_draw(temp_left_offset,y, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}



 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

}
void ZoomInterpolation(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight);
void ZoomNormal(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight);
void ZoomNormal3D1(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight,U16  offsetY);
void ZoomNormal3D2(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight,U16  offsetY);
void ZoomNormal3D3(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight,U16  offsetX);
void ZoomNormal3D4(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight,U16  offsetX);
void RotateNormal(U8* pixel, U16 width, U16 height,gdi_image_cache_bmp_struct* changePixel,U8  angle);

void CSD_UI_change_window_action(void)
{
       
	S32 tempwith = 40/CHANGE_WINSTEP*change_win_step + 100;
gui_lock_double_buffer();
    
    ChangeTbmp_ptr.height = LCD_HEIGHT;
    ChangeTbmp_ptr.width = tempwith;
if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  memset(ChangeTbmp_ptr.buf_ptr,0,ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_change_bmp_ptr(&oldWinBmpbuffer,&ChangeTbmp_ptr);
//ZoomNormal(oldWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.buf_ptr,oldWinBmpbuffer.width,oldWinBmpbuffer.height ,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height );
//ZoomNormal3D1(oldWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.buf_ptr,oldWinBmpbuffer.width,oldWinBmpbuffer.height ,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height );
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,ChangeTbmp_ptr.width - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
		
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
}


    ChangeTbmp_ptr.height = LCD_HEIGHT;
    ChangeTbmp_ptr.width =LCD_WIDTH -  tempwith;
    if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
		memset(ChangeTbmp_ptr.buf_ptr,0,ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_change_bmp_ptr(&newWinBmpbuffer,&ChangeTbmp_ptr);
//ZoomNormal(newWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.buf_ptr,newWinBmpbuffer.width,newWinBmpbuffer.height ,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height );
//ZoomNormal3D2(newWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.buf_ptr,newWinBmpbuffer.width,newWinBmpbuffer.height ,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height );

	    gdi_layer_push_clip();
	    gdi_layer_set_clip(tempwith,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(tempwith,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 1;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
//CSD_UI_change_window_action();
#else
//pixtel_UI_cancel_timer(CSD_UI_change_window_action);
#endif
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action();
#else
//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action);
CSD_UI_change_window_action();
#endif

	
//CSD_UI_change_window_action();
		}
}
void CSD_UI_change_window_action_01(void)
{
       
	S32 tempwidth = LCD_WIDTH/CHANGE_WINSTEP*change_win_step;
	S32 tempheight = LCD_HEIGHT/CHANGE_WINSTEP*change_win_step;
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();


    ChangeTbmp_ptr.height = LCD_HEIGHT - tempheight;
    ChangeTbmp_ptr.width =LCD_WIDTH -  tempwidth;
    if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_change_bmp_ptr(&newWinBmpbuffer,&ChangeTbmp_ptr);
	//   ZoomNormal(newWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.buf_ptr,newWinBmpbuffer.width,newWinBmpbuffer.height ,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height );
	    gdi_layer_push_clip();
	    gdi_layer_set_clip((LCD_WIDTH - ChangeTbmp_ptr.width)/2,(LCD_HEIGHT - ChangeTbmp_ptr.height)/2,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height);
	    gdi_image_cache_bmp_draw((LCD_WIDTH - ChangeTbmp_ptr.width)/2,(LCD_HEIGHT - ChangeTbmp_ptr.height)/2, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
//CSD_UI_change_window_action();
#else
//pixtel_UI_cancel_timer(CSD_UI_change_window_action_01);
#endif

	
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
	#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_01();
#else
//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_01);
CSD_UI_change_window_action_01();
#endif
	
//CSD_UI_change_window_action_01();
		}
}
void CSD_UI_change_window_action_02(void)
{
       
	S32 tempwith = LCD_WIDTH/CHANGE_WINSTEP*change_win_step;
gui_lock_double_buffer();
   	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();
		
    ChangeTbmp_ptr.height = LCD_HEIGHT;
    ChangeTbmp_ptr.width =LCD_WIDTH -  tempwith;
    if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_change_bmp_ptr(&newWinBmpbuffer,&ChangeTbmp_ptr);
//	   ZoomNormal(newWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.buf_ptr,newWinBmpbuffer.width,newWinBmpbuffer.height ,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height );

	    gdi_layer_push_clip();
	    gdi_layer_set_clip(tempwith,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(tempwith,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
//CSD_UI_change_window_action_02();
#else
coolsand_UI_cancel_timer(CSD_UI_change_window_action_02);
#endif

	
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_02();
#else
coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_02);
#endif

	
//CSD_UI_change_window_action_02();
		}
}
void CSD_UI_change_window_action_03(void)
{
       
	S32 tempheigt = LCD_HEIGHT/CHANGE_WINSTEP*change_win_step;

 	gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();
    ChangeTbmp_ptr.height = LCD_HEIGHT - tempheigt;
    ChangeTbmp_ptr.width =LCD_WIDTH;
    if(ChangeTbmp_ptr.height != 0)
    	{
   
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_change_bmp_ptr(&newWinBmpbuffer,&ChangeTbmp_ptr);
//	   ZoomNormal(newWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.buf_ptr,newWinBmpbuffer.width,newWinBmpbuffer.height ,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height );

	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,ChangeTbmp_ptr.height);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
	 
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}

gui_unlock_double_buffer();
	 
	 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, ChangeTbmp_ptr.height);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
//CSD_UI_change_window_action_02();
#else
coolsand_UI_cancel_timer(CSD_UI_change_window_action_03);
#endif

	
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_03();
#else
coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_03);
#endif

	
//CSD_UI_change_window_action_03();
		}
}
void CSD_UI_change_window_action_04(void)
{
       
	S32 tempheigt = LCD_HEIGHT/CHANGE_WINSTEP*change_win_step;

 	gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,LCD_HEIGHT - tempheigt,LCD_WIDTH - 1,tempheigt - 1);
	    gdi_image_cache_bmp_draw(0,LCD_HEIGHT - tempheigt, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();

	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - tempheigt);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();

      gui_unlock_double_buffer();
	 
	 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
//CSD_UI_change_window_action_03();
#else
coolsand_UI_cancel_timer(CSD_UI_change_window_action_04);
#endif

	
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_04();
#else
coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_04);
#endif

	
//CSD_UI_change_window_action_04();
		}
}
void CSD_UI_change_window_action_05(void)
{
       
	S32 tempwith = LCD_WIDTH/CHANGE_WINSTEP*change_win_step;
gui_lock_double_buffer();

   ChangeTbmp_ptr.height = LCD_HEIGHT;
    ChangeTbmp_ptr.width = tempwith;
if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH -ChangeTbmp_ptr.width,0,LCD_WIDTH-1,LCD_HEIGHT - 1,&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,ChangeTbmp_ptr.width - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
		
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
}


    ChangeTbmp_ptr.height = LCD_HEIGHT;
    ChangeTbmp_ptr.width =LCD_WIDTH -  tempwith;
    if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_cache_bmp_get(0,0,ChangeTbmp_ptr.width-1,LCD_HEIGHT - 1,&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(tempwith,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(tempwith,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}



 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
//CSD_UI_change_window_action_04();
#else
coolsand_UI_cancel_timer(CSD_UI_change_window_action_05);
#endif

	
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_05();
#else
//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_05);
CSD_UI_change_window_action_05();
#endif

	
//CSD_UI_change_window_action();
		}
}

void CSD_UI_change_window_action_06(void)
{
       
	S32 tempheigt = LCD_HEIGHT/CHANGE_WINSTEP*change_win_step;
gui_lock_double_buffer();

   ChangeTbmp_ptr.height = tempheigt;
    ChangeTbmp_ptr.width = LCD_WIDTH;
if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  
	    CSD_UI_cache_bmp_get(0,0,LCD_WIDTH-1,ChangeTbmp_ptr.height - 1,&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,LCD_HEIGHT - ChangeTbmp_ptr.height ,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,LCD_HEIGHT - ChangeTbmp_ptr.height, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
		
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
}


    ChangeTbmp_ptr.height = LCD_HEIGHT - tempheigt;
    ChangeTbmp_ptr.width =LCD_WIDTH;
    if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	    CSD_UI_cache_bmp_get(0,tempheigt,LCD_WIDTH-1,LCD_HEIGHT - 1,&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,ChangeTbmp_ptr.height - 1);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}



 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
//CSD_UI_change_window_action_05();
#else
//pixtel_UI_cancel_timer(CSD_UI_change_window_action_06);
#endif

	
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_06();
#else
//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_06);
CSD_UI_change_window_action_06();
#endif

	
//CSD_UI_change_window_action();
		}
}

void CSD_UI_change_window_action_07(void)
{
       
gui_lock_double_buffer();

   ChangeTbmp_ptr.height = LCD_HEIGHT;
   ChangeTbmp_ptr.width = LCD_WIDTH;

	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*4);
		
	    CSD_UI_exchangeWin_bmp_ptr(&oldWinBmpbuffer,&newWinBmpbuffer,&ChangeTbmp_ptr,change_win_step);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0 ,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
		
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);



 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
	//CSD_UI_change_window_action_05();
#else
	coolsand_UI_cancel_timer(CSD_UI_change_window_action_07);
#endif
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
	CSD_UI_change_window_action_07();
#else
	coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_07);
//CSD_UI_change_window_action_07();
#endif
		}
}

void CSD_UI_change_window_action_08(void)//open door
{
	U8 change_win_step_temp = 0;
       S32 tempwidtf = 0;
if(change_win_step ==  0)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
	change_win_step_temp = change_win_step - 1;
       tempwidtf = LCD_WIDTH/CHANGE_WINSTEP*change_win_step_temp/2;
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();

		
   ChangeTbmp_ptr.height = LCD_HEIGHT;
    ChangeTbmp_ptr.width = tempwidtf;
if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH/2 - tempwidtf,0,LCD_WIDTH/2,LCD_HEIGHT ,&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0 ,tempwidtf,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH/2 ,0,LCD_WIDTH/2+ tempwidtf,LCD_HEIGHT ,&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(LCD_WIDTH - tempwidtf,0 ,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(LCD_WIDTH - tempwidtf,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
}


 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
	//CSD_UI_change_window_action_05();
#else
//	pixtel_UI_cancel_timer(CSD_UI_change_window_action_08);
#endif
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
	CSD_UI_change_window_action_08();
#else
	//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_08);
CSD_UI_change_window_action_08();
#endif
		}
}
void CSD_UI_change_window_action_09(void)//close door
{
       U8 change_win_step_temp = 0; 
       S32 tempwidtf = 0;
	change_win_step_temp = CHANGE_WINSTEP - change_win_step;
       tempwidtf = LCD_WIDTH/CHANGE_WINSTEP*change_win_step_temp/2;
if(change_win_step_temp != 0)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();

		
   ChangeTbmp_ptr.height = LCD_HEIGHT;
    ChangeTbmp_ptr.width = tempwidtf;
if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH/2 - tempwidtf,0,LCD_WIDTH/2,LCD_HEIGHT,&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0 ,tempwidtf,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH/2 ,0,LCD_WIDTH/2+ tempwidtf,LCD_HEIGHT,&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(LCD_WIDTH - tempwidtf,0 ,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(LCD_WIDTH - tempwidtf,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

		if(ChangeTbmp_ptr.buf_ptr != NULL)
	     		 OslMfree(ChangeTbmp_ptr.buf_ptr);
}



 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
#ifdef MMI_ON_HARDWARE_P
	//CSD_UI_change_window_action_05();
#else
//	pixtel_UI_cancel_timer(CSD_UI_change_window_action_08);
#endif
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
	CSD_UI_change_window_action_09();
#else
	//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_08);
CSD_UI_change_window_action_09();
#endif
		}
}


extern GDI_RESULT gdi_image_cache_bmp_draw_part(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 bmpx1,S32 bmpy1,S32 bmpwidth,S32 bmpheight);

void CSD_UI_change_window_action_10(void)//Iphone open door
{
	U8 change_win_step_temp = 0;
       S32 tempwidtf = 0;
	S32 tempheight = 0;
if(change_win_step ==  0)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
	change_win_step_temp = change_win_step ;
       tempwidtf = LCD_WIDTH*change_win_step_temp/CHANGE_WINSTEP/2;
	tempheight = LCD_HEIGHT*change_win_step_temp/CHANGE_WINSTEP/2;
gui_lock_double_buffer();
gdi_layer_push_clip();
gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
gdi_image_cache_bmp_draw_part(0,0,&oldWinBmpbuffer,
	                                                   LCD_WIDTH/2 - tempwidtf,LCD_HEIGHT/2 - tempheight,LCD_WIDTH/2,LCD_HEIGHT/2 );
gdi_image_cache_bmp_draw_part(LCD_WIDTH - tempwidtf,LCD_HEIGHT - tempheight,&oldWinBmpbuffer,
	                                                  LCD_WIDTH/2 ,LCD_HEIGHT/2,LCD_WIDTH/2+ tempwidtf,LCD_HEIGHT/2+ tempheight );
gdi_image_cache_bmp_draw_part(0,LCD_HEIGHT - tempheight,&oldWinBmpbuffer,
	                                                  LCD_WIDTH/2 - tempwidtf,LCD_HEIGHT/2,LCD_WIDTH/2,LCD_HEIGHT/2+ tempheight );
   gdi_image_cache_bmp_draw_part(LCD_WIDTH - tempwidtf,0,&oldWinBmpbuffer,
   	                                                   LCD_WIDTH/2 ,LCD_HEIGHT/2 - tempheight,LCD_WIDTH/2+ tempwidtf,LCD_HEIGHT/2 );
gdi_layer_pop_clip();
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}

	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
	CSD_UI_change_window_action_10();
#else
//	coolsand_UI_start_timer(100, CSD_UI_change_window_action_10);
	CSD_UI_change_window_action_10();
#endif
		}
}

void CSD_UI_change_window_action_11(void)//Iphone close door
{
       U8 change_win_step_temp = 0; 
       S32 tempwidtf = 0;
	S32 tempheight = 0;
	change_win_step_temp = CHANGE_WINSTEP - change_win_step;
       tempwidtf = LCD_WIDTH*change_win_step_temp/CHANGE_WINSTEP/2;
	tempheight = LCD_HEIGHT*change_win_step_temp/CHANGE_WINSTEP/2;
if(change_win_step == 0)
{
	gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else if(change_win_step_temp != 0)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_image_cache_bmp_draw_part(0,0,&newWinBmpbuffer,LCD_WIDTH/2 - tempwidtf,LCD_HEIGHT/2 - tempheight,LCD_WIDTH/2,LCD_HEIGHT/2);
	    gdi_image_cache_bmp_draw_part(LCD_WIDTH - tempwidtf,LCD_HEIGHT - tempheight,&newWinBmpbuffer,LCD_WIDTH/2 ,LCD_HEIGHT/2,LCD_WIDTH/2+ tempwidtf,LCD_HEIGHT/2+ tempheight);
	    gdi_image_cache_bmp_draw_part(0,LCD_HEIGHT - tempheight,&newWinBmpbuffer,LCD_WIDTH/2 - tempwidtf,LCD_HEIGHT/2,LCD_WIDTH/2,LCD_HEIGHT/2+ tempheight  );
	    gdi_image_cache_bmp_draw_part(LCD_WIDTH - tempwidtf,0,&newWinBmpbuffer,LCD_WIDTH/2 ,LCD_HEIGHT/2 - tempheight,LCD_WIDTH/2+ tempwidtf,LCD_HEIGHT/2  );
	    gdi_layer_pop_clip();
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
	gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}


	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
	CSD_UI_change_window_action_11();
#else
	//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_08);
	CSD_UI_change_window_action_11();
#endif
		}
}
extern GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,U16 outWidth, U16 outHeight);

void CSD_UI_change_window_action_12(void)
{
       U8 change_win_step_temp = CHANGE_WINSTEP - change_win_step; 
	S32 tempwidth = LCD_WIDTH*change_win_step_temp/CHANGE_WINSTEP;
	S32 tempheight = LCD_HEIGHT*change_win_step_temp/CHANGE_WINSTEP;
gui_lock_double_buffer();
if(change_win_step == 0)
{
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
}
else
{
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
    ChangeTbmp_ptr.height = LCD_HEIGHT - tempheight;
    ChangeTbmp_ptr.width =LCD_WIDTH - tempwidth;
    if((ChangeTbmp_ptr.width != 0)&&(ChangeTbmp_ptr.height != 0))
    	{
//	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
//	    CSD_UI_change_bmp_ptr(&oldWinBmpbuffer,&ChangeTbmp_ptr);
//	    ZoomNormal(oldWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.buf_ptr,oldWinBmpbuffer.width,oldWinBmpbuffer.height ,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height );
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	gdi_image_cache_bmp_draw_ZoomNormal((LCD_WIDTH - ChangeTbmp_ptr.width)/2,(LCD_HEIGHT - ChangeTbmp_ptr.height)/2, 
		&oldWinBmpbuffer,ChangeTbmp_ptr.width, ChangeTbmp_ptr.height );
//	    gdi_image_cache_bmp_draw((LCD_WIDTH - ChangeTbmp_ptr.width)/2,(LCD_HEIGHT - ChangeTbmp_ptr.height)/2, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
	    
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}
}
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_12();
#else
//coolsand_UI_start_timer(100, CSD_UI_change_window_action_12);
CSD_UI_change_window_action_12();
#endif
	
//CSD_UI_change_window_action_01();
		}
}


void CSD_UI_change_window_action_13(void)
{
       
	S32 tempwidth = LCD_WIDTH*change_win_step/CHANGE_WINSTEP;
	S32 tempheight = LCD_HEIGHT*change_win_step/CHANGE_WINSTEP;
gui_lock_double_buffer();
 if(change_win_step != 0)
 	{
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();
 	}


    ChangeTbmp_ptr.height = LCD_HEIGHT - tempheight;
    ChangeTbmp_ptr.width =LCD_WIDTH -  tempwidth;
    if(ChangeTbmp_ptr.width != 0)
    	{
//	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  //  CSD_UI_change_bmp_ptr(&newWinBmpbuffer,&ChangeTbmp_ptr);
//	   ZoomNormal(newWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.buf_ptr,newWinBmpbuffer.width,newWinBmpbuffer.height ,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height );
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
		gdi_image_cache_bmp_draw_ZoomNormal((LCD_WIDTH - ChangeTbmp_ptr.width)/2,(LCD_HEIGHT - ChangeTbmp_ptr.height)/2, 
		&newWinBmpbuffer,ChangeTbmp_ptr.width, ChangeTbmp_ptr.height );
//	    gdi_image_cache_bmp_draw((LCD_WIDTH - ChangeTbmp_ptr.width)/2,(LCD_HEIGHT - ChangeTbmp_ptr.height)/2, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
    	}

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
	if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
	#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_13();
#else
//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_01);
CSD_UI_change_window_action_13();
#endif
	
//CSD_UI_change_window_action_01();
		}
}

void CSD_UI_change_window_action_14(void)
{
       
	S32 tempwith = LCD_WIDTH*change_win_step/CHANGE_WINSTEP;
gui_lock_double_buffer();
gdi_image_cache_bmp_draw_part(0,0,&oldWinBmpbuffer,LCD_WIDTH -tempwith,0,LCD_WIDTH-1,LCD_HEIGHT - 1);
gdi_image_cache_bmp_draw_part(tempwith,0,&newWinBmpbuffer,0,0,LCD_WIDTH -  tempwith-1,LCD_HEIGHT - 1);

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
	if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_14();
#else
//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_05);
CSD_UI_change_window_action_14();
#endif

	
//CSD_UI_change_window_action();
		}
}



void CSD_UI_change_window_action_15(void)
{
       U8 change_win_step_temp = CHANGE_WINSTEP - change_win_step; 
	S32 tempwith = LCD_WIDTH*change_win_step_temp/CHANGE_WINSTEP;
gui_lock_double_buffer();
gdi_image_cache_bmp_draw_part(0,0,&newWinBmpbuffer,LCD_WIDTH -tempwith,0,LCD_WIDTH-1,LCD_HEIGHT - 1);
gdi_image_cache_bmp_draw_part(tempwith,0,&oldWinBmpbuffer,0,0,LCD_WIDTH -  tempwith-1,LCD_HEIGHT - 1);
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
	if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_15();
#else
//coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_15);
CSD_UI_change_window_action_15();
#endif

	
//CSD_UI_change_window_action();
		}
}





void CSD_UI_change_window_action_16(void)
{
       U8 change_win_step_temp = CHANGE_WINSTEP - change_win_step; 
	S32 tempwith = LCD_WIDTH*change_win_step_temp/CHANGE_WINSTEP;
gui_lock_double_buffer();
if(change_win_step == CHANGE_WINSTEP)
{
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();
}
gdi_image_cache_bmp_draw_part(0,0,&newWinBmpbuffer,LCD_WIDTH -tempwith,0,LCD_WIDTH-1,LCD_HEIGHT - 1);
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_16();
#else
//coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_16);
CSD_UI_change_window_action_16();
#endif

	
//CSD_UI_change_window_action();
		}
}


void CSD_UI_change_window_action_17(void)
{
       
	S32 tempwith = LCD_WIDTH*change_win_step/CHANGE_WINSTEP;
gui_lock_double_buffer();
if(change_win_step == CHANGE_WINSTEP)
{
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();
}
gdi_image_cache_bmp_draw_part(tempwith,0,&newWinBmpbuffer,0,0,LCD_WIDTH -  tempwith-1,LCD_HEIGHT - 1);

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
	if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_17();
#else
//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_17);
CSD_UI_change_window_action_17();
#endif

		}
}

void CSD_UI_change_window_action_18(void)
{
       U8 change_win_step_temp = CHANGE_WINSTEP - change_win_step; 
	S32 tempheight = LCD_HEIGHT*change_win_step_temp/CHANGE_WINSTEP;
gui_lock_double_buffer();
if(change_win_step == CHANGE_WINSTEP)
{
    	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();
}	
    ChangeTbmp_ptr.height = tempheight;
    ChangeTbmp_ptr.width = LCD_WIDTH;
if(ChangeTbmp_ptr.height != 0)
    	{
//	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
//	  memset(ChangeTbmp_ptr.buf_ptr,0,ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
////	    CSD_UI_change_bmp_ptr(&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,ChangeTbmp_ptr.height - 1);
//	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	gdi_image_cache_bmp_draw_ZoomNormal(0,0, 
		&newWinBmpbuffer,ChangeTbmp_ptr.width, ChangeTbmp_ptr.height );

	    gdi_layer_pop_clip();
		
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
}

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_18();
#else
//coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_18);
CSD_UI_change_window_action_18();
#endif
		}
}

void CSD_UI_change_window_action_19(void)
{
       
	S32 tempheight = LCD_HEIGHT*change_win_step/CHANGE_WINSTEP;
gui_lock_double_buffer();

    	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
		
    ChangeTbmp_ptr.height = tempheight;
    ChangeTbmp_ptr.width = LCD_WIDTH;
if(ChangeTbmp_ptr.height != 0)
    	{
//	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
//	  memset(ChangeTbmp_ptr.buf_ptr,0,ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
//	    CSD_UI_change_bmp_ptr(&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,ChangeTbmp_ptr.height - 1);
//	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);

gdi_image_cache_bmp_draw_ZoomNormal(0,0, 
		&oldWinBmpbuffer,ChangeTbmp_ptr.width, ChangeTbmp_ptr.height );
	    gdi_layer_pop_clip();
		
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
}

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_19();
#else
//coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_19);
CSD_UI_change_window_action_19();
#endif
		}
}
extern GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal3D1(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 outwidth, S32 outheight,U16  offsetY);
extern GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal3D2(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 outwidth, S32 outheight,U16  offsetY);
extern GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal3D3(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 outwidth, S32 outheight,U16  offsetX);
extern GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal3D4(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 outwidth, S32 outheight,U16  offsetX);

void CSD_UI_change_window_action_20(void)
{

	S32 tempwith = LCD_WIDTH*change_win_step/CHANGE_WINSTEP ;

 if(change_win_step == 0)
 	{
 	gui_lock_double_buffer();
 	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
        gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 	}
 else
 	{
    	   
if(change_win_step > (CHANGE_WINSTEP/2))
{
if((tempwith != LCD_WIDTH)&&(tempwith != 0))
    	{
    	gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D1((LCD_WIDTH -tempwith)/2,0, &oldWinBmpbuffer,tempwith,LCD_HEIGHT,(CHANGE_WINSTEP -change_win_step)*2);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
        gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);		
	}
}
else
{
    if(((LCD_WIDTH -  tempwith) != LCD_WIDTH)&&((LCD_WIDTH -  tempwith) != 0))
    	{
    	   gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D2((tempwith)/2,0, &newWinBmpbuffer,(LCD_WIDTH -  tempwith),LCD_HEIGHT,change_win_step*2);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
        gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);	
    	}
}
 	}

 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_20();
#else
//coolsand_UI_start_timer(100, CSD_UI_change_window_action_20);
CSD_UI_change_window_action_20();
#endif
		}
}


void CSD_UI_change_window_action_21(void)
{
       
	S32 tempwith = LCD_WIDTH*change_win_step/CHANGE_WINSTEP ;

 if(change_win_step == 0)
 	{
 if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
 	}
 else
 	{
if(change_win_step > (CHANGE_WINSTEP/2))
{
if((tempwith != LCD_WIDTH)&&(tempwith != 0))
    	{
    	gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D2((LCD_WIDTH -tempwith)/2,0, &oldWinBmpbuffer,tempwith,LCD_HEIGHT,(CHANGE_WINSTEP -change_win_step)*2);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
 	gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);		
	}
}
else
{
    if(((LCD_WIDTH -  tempwith) != LCD_WIDTH)&&((LCD_WIDTH -  tempwith) != 0))
    	{
    	gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D1((tempwith)/2,0, &newWinBmpbuffer,(LCD_WIDTH -  tempwith),LCD_HEIGHT,change_win_step*2);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
 	gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    	}
}
 	}

 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_21();
#else
//pixtel_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action);
CSD_UI_change_window_action_21();
#endif
		}
}


void CSD_UI_change_window_action_22(void)
{
	S32 tempheight_ptr= 0;
       S32 tempheight_offset = 40;
	S32 tempheight = LCD_HEIGHT*change_win_step/CHANGE_WINSTEP ;

if(tempheight > tempheight_offset)
{
    tempheight_ptr = tempheight - tempheight_offset;
}
else
{
	tempheight_ptr = 0;
}
if(((CHANGE_WINSTEP -change_win_step) != 0)&&(tempheight_ptr != 0))
    	{
    gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0 ,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D3(0, 0, &oldWinBmpbuffer,LCD_WIDTH, tempheight_ptr,(CHANGE_WINSTEP -change_win_step)*2);
	    gdi_layer_pop_clip();
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	}

if((LCD_HEIGHT - tempheight) > tempheight_offset)
{
    tempheight_ptr = LCD_HEIGHT - tempheight - tempheight_offset;
}
else
{
	tempheight_ptr = 0;
}
    if((change_win_step != 0)&&(tempheight_ptr != 0))
    	{
    	gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0 ,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D4(0, LCD_HEIGHT -tempheight_ptr, &newWinBmpbuffer,LCD_WIDTH, tempheight_ptr,change_win_step*2);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    	}


 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_22();
#else
//coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_22);
CSD_UI_change_window_action_22();
#endif

		}
}



void CSD_UI_change_window_action_23(void)
{
	S32 tempheight_ptr= 0;
	S32 tempheight_offset = 40;
       U8 change_win_step_temp = CHANGE_WINSTEP - change_win_step; 
	S32 tempheight = LCD_HEIGHT*change_win_step_temp /CHANGE_WINSTEP ;

if(tempheight > tempheight_offset)
{
    tempheight_ptr = tempheight - tempheight_offset;
}
else
{
	tempheight_ptr = 0;
}
if(((CHANGE_WINSTEP -change_win_step_temp) != 0)&&(tempheight_ptr != 0))
    	{
    	gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0 ,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D3(0, 0, &oldWinBmpbuffer,LCD_WIDTH, tempheight_ptr,(CHANGE_WINSTEP -change_win_step_temp)*2);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
	 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	}

if((LCD_HEIGHT - tempheight) > tempheight_offset)
{
    tempheight_ptr = LCD_HEIGHT - tempheight - tempheight_offset;
}
else
{
	tempheight_ptr = 0;
}
    if((change_win_step_temp != 0)&&(tempheight_ptr != 0))
    	{
    	gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0 ,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D4(0, LCD_HEIGHT -tempheight_ptr, &newWinBmpbuffer,LCD_WIDTH, tempheight_ptr,change_win_step_temp*2);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
	 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    	}


 if(change_win_step == 0)
	{
	change_window_flag = 0;
	change_win_step = CHANGE_WINSTEP;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_23();
#else
//coolsand_UI_start_timer(50, CSD_UI_change_window_action_23);
CSD_UI_change_window_action_23();
#endif
		}
}

extern GDI_RESULT gdi_image_cache_bmp_draw_transparent(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr);
void CSD_UI_change_window_action_24(void)
{
	U8 CHANGE_WINSTEP_24 = 15;
	static U8 change_win_step_24 = 15;
	U8 change_win_step_24_temp ;
	gdi_image_cache_bmp_struct ChangeTbmp_ptr_temp = {0,0,0};
	gui_lock_double_buffer();

	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    //gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_WHITE);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	change_win_step_24_temp = CHANGE_WINSTEP_24 - change_win_step_24 + 1;
	ChangeTbmp_ptr_temp.height = LCD_HEIGHT*1/change_win_step_24_temp;
	ChangeTbmp_ptr_temp.width =LCD_WIDTH*1/change_win_step_24_temp;
    if(ChangeTbmp_ptr_temp.width != 0)
    	{
	    ChangeTbmp_ptr_temp.buf_ptr = OslMalloc(ChangeTbmp_ptr_temp.height*ChangeTbmp_ptr_temp.width*2);
	    ZoomNormal(oldWinBmpbuffer.buf_ptr,ChangeTbmp_ptr_temp.buf_ptr,newWinBmpbuffer.width,newWinBmpbuffer.height ,ChangeTbmp_ptr_temp.width,ChangeTbmp_ptr_temp.height );
    	}
	    RotateNormal(ChangeTbmp_ptr_temp.buf_ptr,ChangeTbmp_ptr_temp.width,ChangeTbmp_ptr_temp.height ,&ChangeTbmp_ptr,change_win_step_24 );
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw_transparent((LCD_WIDTH - ChangeTbmp_ptr.width)/2,(LCD_HEIGHT - ChangeTbmp_ptr.height)/2, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
	 if(ChangeTbmp_ptr_temp.buf_ptr != NULL)
	     		 OslMfree(ChangeTbmp_ptr_temp.buf_ptr);
		if(ChangeTbmp_ptr.buf_ptr != NULL)
	     		 OslMfree(ChangeTbmp_ptr.buf_ptr);


 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step_24 == 0)
	{
	change_window_flag = 0;
	change_win_step_24 = CHANGE_WINSTEP_24;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step_24--;
#ifdef MMI_ON_HARDWARE_P
	CSD_UI_change_window_action_24();
#else
//	coolsand_UI_start_timer(100, CSD_UI_change_window_action_24);
	CSD_UI_change_window_action_24();
#endif
		}
}



void CSD_UI_change_window_action_25(void)
{
       U8 CHANGE_WINSTEP_25 = 12;
	static U8 change_win_step_25 = 12;
	S32 tempwith = LCD_WIDTH*change_win_step_25/CHANGE_WINSTEP_25 ;
	S32 tempheight = LCD_HEIGHT*change_win_step_25/CHANGE_WINSTEP_25 ;
	U16  offsetY = 80;
gui_lock_double_buffer();
    	   gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    //gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
if(change_win_step_25==CHANGE_WINSTEP_25 )
{
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw_ZoomNormal3D1((LCD_WIDTH -tempwith),0, &oldWinBmpbuffer,LCD_WIDTH ,LCD_HEIGHT,offsetY);
	    gdi_layer_pop_clip();
}
else
{
if((ChangeTbmp_ptr.width != 0)&&(ChangeTbmp_ptr.height >offsetY*2))
{
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw_ZoomNormal3D1(0,(LCD_HEIGHT -tempheight)/2, &oldWinBmpbuffer,tempwith ,tempheight,offsetY);
	    gdi_layer_pop_clip();
}
else
{
	change_win_step_25 = 0;
}

}
 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step_25 == 0)
	{
	change_window_flag = 0;
	change_win_step_25 = CHANGE_WINSTEP_25;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step_25--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_25();
#else
//coolsand_UI_start_timer(50, CSD_UI_change_window_action_25);
CSD_UI_change_window_action_25();
#endif
		}
}




void CSD_UI_change_window_action_26(void)
{
	U8 CHANGE_WINSTEP_26 = 12;
	static U8 change_win_step_26 = 5;
       U8 change_win_step_temp = CHANGE_WINSTEP_26 - change_win_step_26; 
	S32 tempwith = LCD_WIDTH*change_win_step_temp/CHANGE_WINSTEP_26 ;
	S32 tempheight = LCD_HEIGHT*change_win_step_temp/CHANGE_WINSTEP_26 ;
	U16  offsetY = 80;


gui_lock_double_buffer();   
    	//   gdi_layer_push_clip();
	 //   gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	//    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	  //  gdi_layer_pop_clip();
    ChangeTbmp_ptr.height = tempheight;
    ChangeTbmp_ptr.width = tempwith;
	
if((ChangeTbmp_ptr.width != 0)&&(ChangeTbmp_ptr.height >offsetY*2))
{
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw_ZoomNormal3D1(0,(LCD_HEIGHT -tempheight)/2, &oldWinBmpbuffer,tempwith ,tempheight,offsetY);
	    gdi_layer_pop_clip();
}

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step_26 == 0)
	{
	change_window_flag = 0;
	change_win_step_26 = 5;
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gdi_handle	layer_handle;
gdi_layer_get_active(&layer_handle);
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);

}
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
	}
else
{
	change_win_step_26--;
#ifdef MMI_ON_HARDWARE_P
CSD_UI_change_window_action_26();
#else
//coolsand_UI_start_timer(10, CSD_UI_change_window_action_26);
CSD_UI_change_window_action_26();
#endif
		}
}
void rotate_angle(U16* dest_ptr, U16* src_ptr, U16 width, U16 height, U16 angle);
static const U32 iphone_rotate_cache_angle[12]=
{
	0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165
};
void CSD_UI_change_window_action_27(U8 rotate_type)
{
	U8 CHANGE_WINSTEP_27= 24;
	static U8 change_win_step_27 = 24;
	static U8 change27 = 0;
//	U8 change_win_step_27_temp ;
	U8 *layer_buf_ptr;
	gdi_layer_get_buffer_ptr(&layer_buf_ptr); 
         gui_lock_double_buffer();
	
	if(change27 > 11)
		{
		   change27 = 0;
		}

	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	if(rotate_type)
		{
	     rotate_angle((U16*)layer_buf_ptr, (U16*)oldWinBmpbuffer.buf_ptr, oldWinBmpbuffer.width, oldWinBmpbuffer.height,iphone_rotate_cache_angle[change27]);
		}
	else
		{
	     rotate_angle((U16*)layer_buf_ptr, (U16*)oldWinBmpbuffer.buf_ptr, oldWinBmpbuffer.width, oldWinBmpbuffer.height,iphone_rotate_cache_angle[11 - change27]);
		}
	    gdi_layer_pop_clip();
 gui_unlock_double_buffer();
 gdi_layer_blt_base_layer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step_27 == 0)
	{
	change_window_flag = 0;
	change_win_step_27 = CHANGE_WINSTEP_27;
	change27 = 0;
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_layer_pop_clip();
	 gui_unlock_double_buffer();
 gdi_layer_blt_base_layer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	CSD_UI_reget_old_window();
//	mmi_pen_unblock( );
//	GoBackHistory();
	}
else
{
	change_win_step_27--;
	change27++;
	change27++;
#ifdef MMI_ON_HARDWARE_P
	CSD_UI_change_window_action_27(rotate_type);
#else
//	coolsand_UI_start_timer(100, CSD_UI_change_window_action_24);
	CSD_UI_change_window_action_27(rotate_type);
#endif
		}
}



void CSD_UI_change_window_action_28_3D(void)
{
	U8 CHANGE_WINSTEP_28= 40;
	static U8 change_win_step_28 = 40;
	S32 tempwith = LCD_WIDTH/CHANGE_WINSTEP_28*change_win_step_28;
//di_handle	layer_handle;
//di_layer_get_active(&layer_handle);
if((change_win_step_28 != CHANGE_WINSTEP_28)&&(change_win_step_28))
{
gui_lock_double_buffer();

	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D1(0,0, &oldWinBmpbuffer,tempwith,LCD_HEIGHT,(CHANGE_WINSTEP_28 - change_win_step_28)*2);
	    gdi_image_cache_bmp_draw_ZoomNormal3D2(tempwith,0, &newWinBmpbuffer,LCD_WIDTH - tempwith,LCD_HEIGHT,change_win_step_28*2);
	    gdi_layer_pop_clip();

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 }
 if(change_win_step_28 == 0)
	{
	
	change_win_step_28 = CHANGE_WINSTEP_28;
#ifdef MMI_ON_HARDWARE_P
//CSD_UI_change_window_action();
#else
//pixtel_UI_cancel_timer(CSD_UI_change_window_action);
#endif

// gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
if(g_ActiveScreenId == IDLE_SCREEN_ID)
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
gui_lock_double_buffer();
idle_screen_redraw();
gui_unlock_double_buffer();
gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
else
{
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
//i_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);
	gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

}
	CSD_UI_reget_old_window();
	change_window_flag = 0;
	}
else
{
	change_win_step_28 = change_win_step_28 -10;
#ifdef MMI_ON_HARDWARE_P
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
COS_Sleep(20);
#endif
CSD_UI_change_window_action_28_3D();
#else
coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_28_3D);
//CSD_UI_change_window_action_28_3D();
#endif

	
//CSD_UI_change_window_action();
		}
}

void CSD_UI_change_window_action_29_3D(void)
{
	U8 CHANGE_WINSTEP_29= 40;
	static U8 change_win_step_29 = 40;
	S32 tempwith = LCD_WIDTH/CHANGE_WINSTEP_29*change_win_step_29;
	gdi_handle	layer_handle;
	gdi_layer_get_active(&layer_handle);
if((change_win_step_29 != CHANGE_WINSTEP_29)&&(change_win_step_29))
{
gui_lock_double_buffer();

	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_BLACK);
	    gdi_image_cache_bmp_draw_ZoomNormal3D1(0,0, &newWinBmpbuffer,LCD_WIDTH - tempwith,LCD_HEIGHT,(change_win_step_29)*2);
	    gdi_image_cache_bmp_draw_ZoomNormal3D2(LCD_WIDTH - tempwith,0, &oldWinBmpbuffer,tempwith,LCD_HEIGHT,(CHANGE_WINSTEP_29 - change_win_step_29)*2);
	    gdi_layer_pop_clip();

 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 }
 if(change_win_step_29 == 0)
	{
	change_window_flag = 0;
	change_win_step_29 = CHANGE_WINSTEP_29;
#ifdef MMI_ON_HARDWARE_P
//CSD_UI_change_window_action();
#else
//pixtel_UI_cancel_timer(CSD_UI_change_window_action);
#endif
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &newWinBmpbuffer);
	    gdi_layer_pop_clip();
	    
	 gui_unlock_double_buffer();
gdi_layer_blt(0,layer_handle,0,0,0,0,LCD_WIDTH,LCD_HEIGHT);
	CSD_UI_reget_old_window();
	}
else
{
	change_win_step_29 = change_win_step_29 -10;
#ifdef MMI_ON_HARDWARE_P
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
COS_Sleep(20);
#endif
CSD_UI_change_window_action_29_3D();
#else
coolsand_UI_start_timer(CHANGE_SPEED, CSD_UI_change_window_action_29_3D);
//CSD_UI_change_window_action_28_3D();
#endif

	
//CSD_UI_change_window_action();
		}
}
extern void gdi_darw_alpha_blending_screen(gdi_image_cache_bmp_struct *old_bmp_ptr,gdi_image_cache_bmp_struct *new_bmp_ptr,U32 transparent_color_a);
void CSD_UI_change_window_action_30(void)//窗口混色渐变
{
	static U8 change_win_step_30 = CHANGE_WINSTEP;

           gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	    gdi_darw_alpha_blending_screen(&oldWinBmpbuffer,&newWinBmpbuffer,(CHANGE_WINSTEP-change_win_step_30)*255/CHANGE_WINSTEP);
	    gdi_layer_pop_clip();
	    gui_unlock_double_buffer();
	    gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
 if(change_win_step_30 == 0)
 	{
 	change_win_step_30 = CHANGE_WINSTEP;
	CSD_UI_reget_old_window();
 	}
 else
 	{
 	change_win_step_30--;
 	CSD_UI_change_window_action_30();
 	}
}


// 静态成员变量初始化
U16	s_nOffsetX	= 0;
U16	s_nOffsetY	= 0;
U16	s_nPicWidth	= 0;
U16	s_nPicHeight = 0;
U16	s_nCDCWidth	= 0;
U16	s_nCDCHeight = 0;
U16 itemp = 0;
/******************************************************************************
*	作用:		向下扫描特效
******************************************************************************/
void ScanDownDisplay(void)
{
	
	// 扫描高度
	U16 nHeight = LCD_HEIGHT - 1;// MIN(s_nPicHeight, LCD_HEIGHT - 1);

	// 扫描宽度
	U16 nWidth = LCD_WIDTH - 1;//MIN(s_nPicWidth, LCD_WIDTH - 1);
	gui_lock_double_buffer();
	gdi_layer_push_clip();
	gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
//	gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_WHITE);
	gdi_layer_set_clip(s_nOffsetX,s_nOffsetY ,nWidth,itemp+s_nOffsetY);
	gdi_image_cache_bmp_draw(s_nOffsetX,s_nOffsetY , &newWinBmpbuffer);
	gdi_layer_pop_clip();
	gui_unlock_double_buffer();
 	gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, itemp+s_nOffsetY);
	
 if(itemp >= nHeight)
	{
	itemp = 0;
	change_window_flag = 1;
	CSD_UI_reget_old_window();
	}
else
{
	itemp = itemp+5;
	ScanDownDisplay();
		}
}

/******************************************************************************
*	作用:		垂直双重扫描特效
******************************************************************************/
void VSScanDisplay(void)
{
	// 扫描高度
	U16 nHeight = LCD_HEIGHT - 1;// MIN(s_nPicHeight, LCD_HEIGHT - 1);

	// 扫描宽度
	U16 nWidth = LCD_WIDTH - 1;//MIN(s_nPicWidth, LCD_WIDTH - 1);
	
	gui_lock_double_buffer();
	gdi_layer_push_clip();
	gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
//	gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_WHITE);
	 gdi_layer_pop_clip();

	 gdi_layer_push_clip();
	gdi_layer_set_clip(s_nOffsetX,s_nOffsetY ,nWidth,itemp+s_nOffsetY);
	gdi_image_cache_bmp_draw(s_nOffsetX,s_nOffsetY , &newWinBmpbuffer);
	 gdi_layer_pop_clip();
	 
	gdi_layer_push_clip();
	gdi_layer_set_clip(s_nOffsetX,nHeight - itemp + s_nOffsetY ,nWidth,nHeight);
	gdi_image_cache_bmp_draw(s_nOffsetX,s_nOffsetY , &newWinBmpbuffer);
	gdi_layer_pop_clip();
	
	gui_unlock_double_buffer();
 	gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

	 if(itemp >= nHeight/2)
	{
	itemp = 0;
	change_window_flag = 1;
	CSD_UI_reget_old_window();
	}
else
{
	itemp = itemp+5;
	VSScanDisplay();
		}
}


/******************************************************************************
*	作用:		向右移动特效
******************************************************************************/
void MoveRightDisplay(void)
{

	gui_lock_double_buffer();
	gdi_layer_push_clip();
	gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
//	gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_WHITE);
	 gdi_layer_pop_clip();

    ChangeTbmp_ptr.height = LCD_HEIGHT;
    ChangeTbmp_ptr.width = itemp;
if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH -ChangeTbmp_ptr.width,0,LCD_WIDTH-1,LCD_HEIGHT - 1,&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,ChangeTbmp_ptr.width - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
		
	if(ChangeTbmp_ptr.buf_ptr != NULL)
	      OslMfree(ChangeTbmp_ptr.buf_ptr);
}
	 
	gui_unlock_double_buffer();
 	gui_BLT_double_buffer(0, 0, itemp, LCD_HEIGHT - 1);

if(itemp >= LCD_WIDTH)
	{
	itemp = 0;
	change_window_flag = 1;
	CSD_UI_reget_old_window();
	}
else
{
	itemp = itemp+5;
	MoveRightDisplay();
		}
}


/******************************************************************************
*	作用:		水平双重移动特效
******************************************************************************/
void HSMoveDisplay(void)
{


gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    //gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_WHITE);
	    gdi_layer_pop_clip();

		
   ChangeTbmp_ptr.height = LCD_HEIGHT;
    ChangeTbmp_ptr.width = itemp;
if(ChangeTbmp_ptr.width != 0)
    	{
	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*2);
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH/2 - itemp,0,LCD_WIDTH/2,LCD_HEIGHT,&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0 ,itemp,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();
	  
	    CSD_UI_cache_bmp_get(LCD_WIDTH/2 ,0,LCD_WIDTH/2+ itemp,LCD_HEIGHT,&newWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(LCD_WIDTH - itemp,0 ,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(LCD_WIDTH - itemp,0, &ChangeTbmp_ptr);
	    gdi_layer_pop_clip();

		if(ChangeTbmp_ptr.buf_ptr != NULL)
	     		 OslMfree(ChangeTbmp_ptr.buf_ptr);
}



 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);


if(itemp >= LCD_WIDTH/2)
	{
	itemp = 0;
	change_window_flag = 1;
	CSD_UI_reget_old_window();
	}
else
{
	itemp = itemp+5;
	HSMoveDisplay();
		}	
}
/******************************************************************************
*	作用:			水平镜像变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
******************************************************************************/
void HMirrorTrans(U8* pixel, U8* tempPixel, U16 width, U16 height)
{


	// 方法一	通过映射关系进行变换
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;

	for ( j = 0; j < height; j++)
	{
		y = j;

		for ( i = 0; i < width; i++)
		{
			x = width - i - 1;

			copyPixel = pixel + j * width * U8_per_point + i * U8_per_point;
			objPixel = tempPixel + y * width * U8_per_point + x * U8_per_point;
			memcpy(objPixel, copyPixel, U8_per_point);
		}
	}
	
/*
	// 方法二	通过交换像素进行变换
	memcpy(tempPixel, pixel, width*height*2);

	for ( i = 0; i < height; i++)
	{
		for ( j = 0; j < width / 2; j++) 
		{
			swap(tempPixel[j*U8_per_point + i*width*U8_per_point],	 tempPixel[(i+1)*width*U8_per_point - j*U8_per_point - 4]);
			swap(tempPixel[j*U8_per_point + i*width*U8_per_point + 1], tempPixel[(i+1)*width*U8_per_point - j*U8_per_point - 3]);
			swap(tempPixel[j*U8_per_point + i*width*U8_per_point + 2], tempPixel[(i+1)*width*U8_per_point - j*U8_per_point - 2]);
			swap(tempPixel[j*U8_per_point + i*width*U8_per_point + 3], tempPixel[(i+1)*width*U8_per_point - j*U8_per_point - 1]);
		}
	}
	*/
}

/******************************************************************************
*	作用:			垂直镜像变换函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
******************************************************************************/
void VMirrorTrans(U8* pixel, U8* tempPixel, U16 width, U16 height)
{


	// 方法一	通过映射关系进行变换
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;

	for ( j = 0; j < height; j++)
	{
		y = height - j - 1;

		for ( i = 0; i < width; i++)
		{
			x = i;

			copyPixel = pixel + j * width * U8_per_point + i * U8_per_point;
			objPixel = tempPixel + y * width * U8_per_point + x * U8_per_point;
			memcpy(objPixel, copyPixel, U8_per_point);
		}
	}

/*	// 方法二	通过交换像素进行变换
	// 拷贝原始图像
	memcpy(tempPixel, pixel, size);

	for(int i = 0; i < width * 4; i+=4) 
	{
		for (int j = 0; j < height / 2; j++) 
		{
			swap(tempPixel[j*width*4 + i],	   tempPixel[(height-j-1)*width*4 + i]);
			swap(tempPixel[j*width*4 + i + 1], tempPixel[(height-j-1)*width*4 + i + 1]);
			swap(tempPixel[j*width*4 + i + 2], tempPixel[(height-j-1)*width*4 + i + 2]);
			swap(tempPixel[j*width*4 + i + 3], tempPixel[(height-j-1)*width*4 + i + 3]);
		}
	}*/
}


/******************************************************************************
*	作用:			图像的转置函数
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
******************************************************************************/
void Transpose(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight)
{

	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32		tempJ = 0;			// 存储中间值,提高函数速度
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;

	// 转置后图像的宽度和高度

	for ( j = 0; j < outHeight; j++)
	{
		x = j;

		// 可以提前计算与i无关的中间值
		// 可以大大减少重复计算
		tempJ = j * outWidth * U8_per_point;
		for ( i = 0; i < outWidth; i++) 
		{
			y = i;
			copyPixel = pixel + y * width * U8_per_point + x * U8_per_point;
			objPixel = tempPixel +  tempJ + i * U8_per_point;
			memcpy(objPixel, copyPixel, U8_per_point);
		}
	}

}


/******************************************************************************
*	作用:			图像的缩放函数(最临近插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		fx			水平缩放系数
*		fy			垂直缩放系数
******************************************************************************/
void ZoomNormal(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight)
{

	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;						// 变换后的像素横坐标
	U16		y = 0;						// 变换后的像素纵坐标
	U32	tempY;							// 存储中间值,提高函数速度
	U32	tempJ;							// 存储中间值,提高函数速度
	UINT32 Scale,Scaley,accux;
	U8*	tempcopyPixel = NULL;			// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置


	MMI_ASSERT(tempPixel == NULL); // for find crash problem
	MMI_ASSERT(pixel == NULL); //
	
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;
       
	Scaley=(height<<16)/outHeight;
	Scale=(width<<16)/outWidth;
	
	for ( j = 0; j < outHeight; j++)
	{
		// 获得临近像素的纵坐标
		y = ((j*Scale)>>16)+1;

		// 修正坐标
		if (y >= height)
			y--;

		// 计算与i,x无关的中间值
		tempY = y * width * U8_per_point;
		tempJ = j * outWidth * U8_per_point;
		tempcopyPixel = pixel + tempY;
		tempobjPixel = tempPixel + tempJ;

		accux = 65536;
		
		for ( i = outWidth-1; i >0 ; i--) 
		{
			x = accux>>16;
			
			copyPixel =tempcopyPixel + x * U8_per_point;
			*(U16*)(tempobjPixel) = *(U16*)(copyPixel);

			accux += Scale;
			tempobjPixel += U8_per_point;
		}

		x = accux>>16;
			
		if (x >= width)
			x--;
		
		copyPixel =tempcopyPixel + x * U8_per_point;
		*(U16*)(tempobjPixel) = *(U16*)(copyPixel);
		
	}
}




/******************************************************************************
*	作用:			图像的缩放函数(双线性插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		fx			水平缩放系数
*		fy			垂直缩放系数
******************************************************************************/
void ZoomInterpolation(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight)
{
#if 0
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i,k;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32	tempJ;				// 存储中间值,提高函数速度
	// 距离当前点最近的四个点坐标
	U16	x1, x2, y1, y2;
	// 对应四个坐标像素的取值
	U8 clr1, clr2, clr3, clr4;
	double u, v;
	double f1, f2;
#endif
#if 1
	U16 r,g,b,r1,r2,g1,g2,b1,b2;
	UINT32 offsetx,offsety;
	UINT32 Scale,Scaley,alphay,alphax;
	U16 temp;
	U16   *adp;
	U16   *adp1;
	U16 j,i;

	Scaley=(height<<16)/outHeight;
	Scale=(width<<16)/outWidth;
	if ( Scaley > Scale)
		Scale = Scaley;
	adp = (U16*)pixel;
		for(i=0;i<(UINT32)outHeight;i++)
		{
			offsety = ((i*Scale)>>16)+1;
			alphay = (i*Scale)&0xffff;
			if ( offsety > (height-1))
				offsety = height-1;

			for(j=0;j<(UINT32)outWidth;j++)
			{
				offsetx = ((j*Scale)>>16)+1;
				alphax = (j*Scale)&0xffff;
				if ( offsetx > (width-1))
					offsetx = width-1;
				adp1 = adp + offsety * width + offsetx;
				r1 = (*(adp1)& 0xf800 )>>8;
				g1 = ((*(adp1)& 0x7E0 )>>3);
				b1 = (*adp1  & 0x1F) << 3;
				r2 = (*(adp1-1)& 0xf800 )>>8;
				g2 = ((*(adp1-1)& 0x7E0 )>>3);
				b2 = (*(adp1-1)  & 0x1F) << 3;
				r = (r1 * alphax + r2 * (0x10000 - alphax)+32768) >>16;
				g = (g1 * alphax + g2 * (0x10000 - alphax)+32768) >>16;
				b = (b1 * alphax + b2 * (0x10000 - alphax)+32768) >>16;

				adp1 = adp1 - width;
				r1 = (*(adp1)& 0xf800 )>>8;
				g1 = ((*(adp1)& 0x7E0 )>>3);
				b1 = (*adp1  & 0x1F) << 3;
				r2 = (*(adp1-1)& 0xf800 )>>8;
				g2 = ((*(adp1-1)& 0x7E0 )>>3);
				b2 = (*(adp1-1)  & 0x1F) << 3;
				r1 = (r1 * alphax + r2 * (0x10000 - alphax)+32768) >>16;
				g1 = (g1 * alphax + g2 * (0x10000 - alphax)+32768) >>16;
				b1 = (b1 * alphax + b2 * (0x10000 - alphax)+32768) >>16;
				r = (r * alphay + r1 * (0x10000 - alphay)+32768) >>16;
				g = (g * alphay + g1 * (0x10000 - alphay)+32768) >>16;
				b = (b * alphay + b1 * (0x10000 - alphay)+32768) >>16;
				temp = ((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3));
				*tempPixel = temp&0xff;
				tempPixel++;
				*tempPixel = temp>>8;
				tempPixel++;
			}
		}

#endif
	
#if 0	
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;

	for ( j = 0; j < outHeight; j++)
	{
		y = j*height / (outHeight + 0.5);
		tempJ = j * outWidth * U8_per_point;

		for ( i = 0; i < outWidth; i++) 
		{
			x = i *width / (outWidth + 0.5);
			objPixel = tempPixel + tempJ + i * U8_per_point;

			x1 = (U16)x;
			x2 = x1 + 1;
			y1 = (U16)y;
			y2 = y1 + 1;

			u = x - x1;
			v = y - y1;

			// 对于32位真彩色需要分别计算三原色
			for ( k = 0; k < 3; k++)
			{
				// 如果坐标在图像的右下角
				if ( (x >= width - 1) && (y >= height - 1) )
				{
					clr1 = pixel[(y1 * width + x1) * U8_per_point + k];	// (x1, y1)
					objPixel[k] = clr1; 
				}
				// 如果图像在最后一列外侧
				else if ( x >= width - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * U8_per_point + k];	// (x1, y1)
					clr3 = pixel[(y2 * width + x1) * U8_per_point + k];	// (x1, y2)
					objPixel[k] = (U8)(clr1 * (1 - v) + clr3 * v); 
				}			
				// 如果图像在最后一行外侧
				else if ( y >= height - 1 )
				{
					clr1 = pixel[(y1 * width + x1) * U8_per_point + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * U8_per_point + k];	// (x2, y1)
					objPixel[k] = (U8)(clr1 * (1 - u) + clr2 * u);
				}
				else
				{
					clr1 = pixel[(y1 * width + x1) * U8_per_point + k];	// (x1, y1)
					clr2 = pixel[(y1 * width + x2) * U8_per_point + k];	// (x2, y1)
					clr3 = pixel[(y2 * width + x1) * U8_per_point + k];	// (x1, y2)
					clr4 = pixel[(y2 * width + x2) * U8_per_point + k];	// (x2, y2)

					

					f1 = clr1 * (1 - u) + clr2 * u;
					f2 = clr3 * (1 - u) + clr4 * u;
					objPixel[k] = (U8)(f1 * (1 - v) + f2 * v);
				}
			}
		}
	}
	#endif
}

#if 0
int signbits( S32 x )//符号位的个数
{
    int n;
    if ( 0 > x ) x = -(x+1);
    for( n = x ? 30 : 31; x >>= 1; --n );
    return n;
}
/************************************************************************/
/* this function can realize division and left shift                    */
/************************************************************************/
int div_320_320_lshift(S32 divd,S32 divs,S32 loop)
{
  S32 exp,dexp,sexp;
  S32 i,r2=1;
  unsigned int r3=0;


  dexp = signbits(divd);
  sexp = signbits(divs);

  divd = divd << (dexp);
  divs = divs << (sexp);
  
  while ((divs>>28)>=4)
  {
     divd = divd >> 1;
     divs = divs >> 1;
  }

  //exp  = dexp-sexp+28-loop+1;
  exp  = dexp-sexp-loop+30;
  
  for (i=0;i<30;i++) 
  {
    if (divs <= divd) 
	{
      divd = divd - divs;  
      r3 = r3 + r2;   
    }
    r3   <<= 1;         
    divd <<= 1;
  }
  
  if (exp>0)
  {
     r3= r3+(1<<(exp-1));
  }
  
  if (exp>=0) 
  {
	  r3=r3>>exp;    //loop - exp
  }
  else 
	  r3=r3<<(-exp);
  
  
  return (int)r3;
}
#endif
/******************************************************************************
*	作用:			图像的缩放函数(最临近插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		fx			水平缩放系数
*		fy			垂直缩放系数
******************************************************************************/
void ZoomNormal3D1(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight,U16  offsetY)
{

	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32	tempX;				// 存储中间值,提高函数速度
	U32	tempJ;				// 存储中间值,提高函数速?	
	U16 tempoutHeight;
	U16  offsetX = outWidth/offsetY;
	U16  tempoffsetX = offsetX;
	U16  tempoffsetY = 0;
	U32	tempW;
	U32	tempOW;	
	UINT32 Scale,Scaley;
	U8*	tempcopyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置
	U32	tempOW_t;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;
	tempoutHeight = outHeight;
		
	
	Scale=(width<<16)/outWidth;
	for ( j = outWidth - 1; j > 0; j--)
	{
		// 获得临近像素的横坐标
			x = ((j *Scale)>>16)+1;

			// 修正坐标
			if (x >= width)
				x--;
		if(tempoffsetX == 0)
			{
				tempoffsetX = offsetX;
				tempoutHeight = tempoutHeight - 2;
				if(tempoffsetY == offsetY)
					{
						tempoffsetY = offsetY;
					}
				else
					{
						tempoffsetY++;
					}
			}
		else
			{
				tempoffsetX--;
			}
		
		tempX = x  * U8_per_point;
		tempJ = j * U8_per_point;
		tempW = width * U8_per_point;
		tempOW = outWidth * U8_per_point;
		tempOW_t = tempOW*tempoffsetY;
		Scaley=(height<<16)/tempoutHeight;
		tempcopyPixel = pixel + tempX;
		tempobjPixel = tempPixel + tempJ + tempOW_t;
		for ( i = 0; i < tempoutHeight; i++) 
		{
			

			// 获得临近像素的纵坐标
		y = ((i* Scaley )>>16)+1;

		// 修正坐标
		if (y >= height)
			y--;
		
			copyPixel =  tempcopyPixel + y * tempW;
			objPixel =  tempobjPixel +i*tempOW;
			*(U16*)(objPixel) = *(U16*)(copyPixel);
		}
	}

}


/******************************************************************************
*	作用:			图像的缩放函数(最临近插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		fx			水平缩放系数
*		fy			垂直缩放系数
******************************************************************************/
void ZoomNormal3D2(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight,U16  offsetY)
{
#if 1
	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32	tempX;				// 存储中间值,提高函数速度
	U32	tempJ;				// 存储中间值,提高函数速?	
	U16 tempoutHeight;
	U16  offsetX = outWidth/offsetY;
	U16  tempoffsetX = offsetX;
	U16  tempoffsetY = 0;
	U32	tempW;
	U32	tempOW;	
	UINT32 Scale,Scaley;
	U8*	tempcopyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置
	U32	tempOW_t;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;
	tempoutHeight = outHeight;
		
	
	Scale=(width<<16)/outWidth;
	for ( j = 0; j < outWidth; j++)
	{
		// 获得临近像素的横坐标
			x = ((j *Scale)>>16)+1;

			// 修正坐标
			if (x >= width)
				x--;
		if(tempoffsetX == 0)
			{
				tempoffsetX = offsetX;
				tempoutHeight = tempoutHeight - 2;
				if(tempoffsetY == offsetY)
					{
						tempoffsetY = offsetY;
					}
				else
					{
						tempoffsetY++;
					}
			}
		else
			{
				tempoffsetX--;
			}
		
		tempX = x  * U8_per_point;
		tempJ = j * U8_per_point;
		tempW = width * U8_per_point;
		tempOW = outWidth * U8_per_point;
		tempOW_t = tempOW*tempoffsetY;
		Scaley=(height<<16)/tempoutHeight;
		tempcopyPixel = pixel + tempX;
		tempobjPixel = tempPixel + tempJ + tempOW_t;
		for ( i = 0; i < tempoutHeight; i++) 
		{
			

			// 获得临近像素的纵坐标
		y = ((i* Scaley )>>16)+1;

		// 修正坐标
		if (y >= height)
			y--;
		
			copyPixel =  tempcopyPixel + y * tempW;
			objPixel =  tempobjPixel +i*tempOW;
			*(U16*)(objPixel) = *(U16*)(copyPixel);
		}
	}
	#endif

}


/******************************************************************************
*	作用:			图像的缩放函数(最临近插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		fx			水平缩放系数
*		fy			垂直缩放系数
******************************************************************************/
void ZoomNormal3D3(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight,U16  offsetX)
{

	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32	tempY;				// 存储中间值,提高函数速度
	U32	tempJ;				// 存储中间值,提高函数速度
	UINT32 Scale,Scaley;
	U8*	tempcopyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置
	U16  offsetY = outHeight/offsetX;
	U16  tempoffsetX = 0;
	U16  tempoffsetY = offsetY;
	U16 tempoutWidth = outWidth;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;

	Scaley=(height<<16)/outHeight;
	
	for ( j = 0; j < outHeight; j++)
	{
		// 获得临近像素的纵坐标
		y = ((j*Scaley)>>16)+1;

		// 修正坐标
		if (y >= height)
			y--;
		
		if(tempoffsetY == 0)
			{
				tempoffsetY = offsetY;
				tempoutWidth = tempoutWidth - 2;
				if(tempoffsetX == offsetX)
					{
						tempoffsetX = offsetX;
					}
				else
					{
						tempoffsetX++;
					}
			}
		else
			{
				tempoffsetY--;
			}
		// 计算与i,x无关的中间值
		tempY = y * width * U8_per_point;
		tempJ = j * outWidth * U8_per_point;
		tempcopyPixel = pixel + tempY;
		tempobjPixel = tempPixel + tempJ+tempoffsetX* U8_per_point;
		Scale=(width<<16)/tempoutWidth;
		for ( i = 0; i < tempoutWidth; i++) 
		{
			// 获得临近像素的横坐标
			x = ((i*Scale)>>16)+1;

			// 修正坐标
			if (x >= width)
				x--;
			copyPixel =tempcopyPixel + x * U8_per_point;
			objPixel = tempobjPixel + i * U8_per_point;
			*(U16*)(objPixel) = *(U16*)(copyPixel);
		}
	}

}



/******************************************************************************
*	作用:			图像的缩放函数(最临近插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		fx			水平缩放系数
*		fy			垂直缩放系数
******************************************************************************/
void ZoomNormal3D4(U8* pixel, U8* tempPixel, U16 width, U16 height,U16 outWidth, U16 outHeight,U16  offsetX)
{

	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32	tempY;				// 存储中间值,提高函数速度
	U32	tempJ;				// 存储中间值,提高函数速度
	UINT32 Scale,Scaley;
	U8*	tempcopyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置
	U16  offsetY = outHeight/offsetX;
	U16  tempoffsetX = 0;
	U16  tempoffsetY = offsetY;
	U16 tempoutWidth = outWidth;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;

	Scaley=(height<<16)/outHeight;
	
	for ( j = outHeight - 1; j > 0; j--)
	{
		// 获得临近像素的纵坐标
		y = ((j*Scaley)>>16)+1;

		// 修正坐标
		if (y >= height)
			y--;
		
		if(tempoffsetY == 0)
			{
				tempoffsetY = offsetY;
				tempoutWidth = tempoutWidth - 2;
				if(tempoffsetX == offsetX)
					{
						tempoffsetX = offsetX;
					}
				else
					{
						tempoffsetX++;
					}
			}
		else
			{
				tempoffsetY--;
			}
		// 计算与i,x无关的中间值
		tempY = y * width * U8_per_point;
		tempJ = j * outWidth * U8_per_point;
		tempcopyPixel = pixel + tempY;
		tempobjPixel = tempPixel + tempJ+tempoffsetX* U8_per_point;
		Scale=(width<<16)/tempoutWidth;
		for ( i = 0; i < tempoutWidth; i++) 
		{
			// 获得临近像素的横坐标
			x = ((i*Scale)>>16)+1;

			// 修正坐标
			if (x >= width)
				x--;
			copyPixel =tempcopyPixel + x * U8_per_point;
			objPixel = tempobjPixel + i * U8_per_point;
			*(U16*)(objPixel) = *(U16*)(copyPixel);
		}
	}

}

/******************************************************************************
*	作用:			图像的旋转函数(最临近插值法)
*	参数:		
*		pixel		原始像素数组
*		tempPixel	保存变换后图像的像素数组
*		width		原始图像宽度
*		height		原始图像高度
*		outWidth	[out]输出图像的宽度
*		outHeight	[out]输出图像的高度
*		angle		旋转度数,以弧度表示
******************************************************************************/
double fabf(double a)
{
  if (a >= 0)
  {
    return a;
  }
  else
  {
    return 0 - a;
  }
}
#ifndef max
#define max(a,b) ((a>b) ? a : b)
#endif
static const S32 sinTbl[]= {
 0x000, 0x011, 0x023, 0x035, 0x047, 0x059, 0x06b, 0x07c, 0x08e, 0x0a0, // 000-010
 0x0b1, 0x0c3, 0x0d4, 0x0e6, 0x0f7, 0x109, 0x11a, 0x12b, 0x13c, 0x14d, // 010-020
 0x15e, 0x16e, 0x17f, 0x190, 0x1a0, 0x1b0, 0x1c0, 0x1d0, 0x1e0, 0x1f0, // 020-030
 0x1ff, 0x20f, 0x21e, 0x22d, 0x23c, 0x24b, 0x259, 0x268, 0x276, 0x284, // 030-040
 0x292, 0x29f, 0x2ad, 0x2ba, 0x2c7, 0x2d4, 0x2e0, 0x2ec, 0x2f8, 0x304, // 040-050
 0x310, 0x31b, 0x326, 0x331, 0x33c, 0x346, 0x350, 0x35a, 0x364, 0x36d, // 050-060
 0x376, 0x37f, 0x388, 0x390, 0x398, 0x3a0, 0x3a7, 0x3ae, 0x3b5, 0x3bb, // 060-070
 0x3c2, 0x3c8, 0x3cd, 0x3d3, 0x3d8, 0x3dd, 0x3e1, 0x3e5, 0x3e9, 0x3ed, // 070-080
 0x3f0, 0x3f3, 0x3f6, 0x3f8, 0x3fa, 0x3fc, 0x3fd, 0x3fe, 0x3ff, 0x3ff, // 080-090
 0x3ff, 0x3ff, 0x3ff, 0x3fe, 0x3fd, 0x3fc, 0x3fa, 0x3f8, 0x3f6, 0x3f3, // 090-100
 0x3f0, 0x3ed, 0x3e9, 0x3e5, 0x3e1, 0x3dd, 0x3d8, 0x3d3, 0x3cd, 0x3c8, // 100-110
 0x3c2, 0x3bb, 0x3b5, 0x3ae, 0x3a7, 0x3a0, 0x398, 0x390, 0x388, 0x37f, // 110-120
 0x376, 0x36d, 0x364, 0x35a, 0x350, 0x346, 0x33c, 0x331, 0x326, 0x31b, // 120-130
 0x310, 0x304, 0x2f8, 0x2ec, 0x2e0, 0x2d4, 0x2c7, 0x2ba, 0x2ad, 0x29f, // 130-140
 0x292, 0x284, 0x276, 0x268, 0x259, 0x24b, 0x23c, 0x22d, 0x21e, 0x20f, // 140-150
 0x200, 0x1f0, 0x1e0, 0x1d0, 0x1c0, 0x1b0, 0x1a0, 0x190, 0x17f, 0x16e, // 150-160
 0x15e, 0x14d, 0x13c, 0x12b, 0x11a, 0x109, 0x0f7, 0x0e6, 0x0d4, 0x0c3, // 160-170
 0x0b1, 0x0a0, 0x08e, 0x07c, 0x06b, 0x059, 0x047, 0x035, 0x023, 0x011, // 170-180
 0x000,-0x011,-0x023,-0x035,-0x047,-0x059,-0x06b,-0x07c,-0x08e,-0x0a0, // 180-190
-0x0b1,-0x0c3,-0x0d4,-0x0e6,-0x0f7,-0x109,-0x11a,-0x12b,-0x13c,-0x14d, // 190-200
-0x15e,-0x16e,-0x17f,-0x190,-0x1a0,-0x1b0,-0x1c0,-0x1d0,-0x1e0,-0x1f0, // 200-210
-0x1ff,-0x20f,-0x21e,-0x22d,-0x23c,-0x24b,-0x259,-0x268,-0x276,-0x284, // 210-220
-0x292,-0x29f,-0x2ad,-0x2ba,-0x2c7,-0x2d4,-0x2e0,-0x2ec,-0x2f8,-0x304, // 220-230
-0x310,-0x31b,-0x326,-0x331,-0x33c,-0x346,-0x350,-0x35a,-0x364,-0x36d, // 230-240
-0x376,-0x37f,-0x388,-0x390,-0x398,-0x3a0,-0x3a7,-0x3ae,-0x3b5,-0x3bb, // 240-250
-0x3c2,-0x3c8,-0x3cd,-0x3d3,-0x3d8,-0x3dd,-0x3e1,-0x3e5,-0x3e9,-0x3ed, // 250-260
-0x3f0,-0x3f3,-0x3f6,-0x3f8,-0x3fa,-0x3fc,-0x3fd,-0x3fe,-0x3ff,-0x3ff, // 260-270
-0x3ff,-0x3ff,-0x3ff,-0x3fe,-0x3fd,-0x3fc,-0x3fa,-0x3f8,-0x3f6,-0x3f3, // 270-280
-0x3f0,-0x3ed,-0x3e9,-0x3e5,-0x3e1,-0x3dd,-0x3d8,-0x3d3,-0x3cd,-0x3c8, // 280-290
-0x3c2,-0x3bb,-0x3b5,-0x3ae,-0x3a7,-0x3a0,-0x398,-0x390,-0x388,-0x37f, // 290-300
-0x376,-0x36d,-0x364,-0x35a,-0x350,-0x346,-0x33c,-0x331,-0x326,-0x31b, // 300-310
-0x310,-0x304,-0x2f8,-0x2ec,-0x2e0,-0x2d4,-0x2c7,-0x2ba,-0x2ad,-0x29f, // 310-320
-0x292,-0x284,-0x276,-0x268,-0x259,-0x24b,-0x23c,-0x22d,-0x21e,-0x20f, // 320-330
-0x200,-0x1f0,-0x1e0,-0x1d0,-0x1c0,-0x1b0,-0x1a0,-0x190,-0x17f,-0x16e, // 330-340
-0x15e,-0x14d,-0x13c,-0x12b,-0x11a,-0x109,-0x0f7,-0x0e6,-0x0d4,-0x0c3, // 340-350
-0x0b1,-0x0a0,-0x08e,-0x07c,-0x06b,-0x059,-0x047,-0x035,-0x023,-0x011, // 350-360
};	

static const S32 cosTbl[]= {
 0x400, 0x3ff, 0x3ff, 0x3fe, 0x3fd, 0x3fc, 0x3fa, 0x3f8, 0x3f6, 0x3f3, // 000-010
 0x3f0, 0x3ed, 0x3e9, 0x3e5, 0x3e1, 0x3dd, 0x3d8, 0x3d3, 0x3cd, 0x3c8, // 010-020
 0x3c2, 0x3bb, 0x3b5, 0x3ae, 0x3a7, 0x3a0, 0x398, 0x390, 0x388, 0x37f, // 020-030
 0x376, 0x36d, 0x364, 0x35a, 0x350, 0x346, 0x33c, 0x331, 0x326, 0x31b, // 030-040
 0x310, 0x304, 0x2f8, 0x2ec, 0x2e0, 0x2d4, 0x2c7, 0x2ba, 0x2ad, 0x29f, // 040-050
 0x292, 0x284, 0x276, 0x268, 0x259, 0x24b, 0x23c, 0x22d, 0x21e, 0x20f, // 050-060
 0x200, 0x1f0, 0x1e0, 0x1d0, 0x1c0, 0x1b0, 0x1a0, 0x190, 0x17f, 0x16e, // 060-070
 0x15e, 0x14d, 0x13c, 0x12b, 0x11a, 0x109, 0x0f7, 0x0e6, 0x0d4, 0x0c3, // 070-080
 0x0b1, 0x0a0, 0x08e, 0x07c, 0x06b, 0x059, 0x047, 0x035, 0x023, 0x011, // 080-090
 0x000,-0x011,-0x023,-0x035,-0x047,-0x059,-0x06b,-0x07c,-0x08e,-0x0a0, // 090-100
-0x0b1,-0x0c3,-0x0d4,-0x0e6,-0x0f7,-0x109,-0x11a,-0x12b,-0x13c,-0x14d, // 100-110
-0x15e,-0x16e,-0x17f,-0x190,-0x1a0,-0x1b0,-0x1c0,-0x1d0,-0x1e0,-0x1f0, // 110-120
-0x1ff,-0x20f,-0x21e,-0x22d,-0x23c,-0x24b,-0x259,-0x268,-0x276,-0x284, // 120-130
-0x292,-0x29f,-0x2ad,-0x2ba,-0x2c7,-0x2d4,-0x2e0,-0x2ec,-0x2f8,-0x304, // 130-140
-0x310,-0x31b,-0x326,-0x331,-0x33c,-0x346,-0x350,-0x35a,-0x364,-0x36d, // 140-150
-0x376,-0x37f,-0x388,-0x390,-0x398,-0x3a0,-0x3a7,-0x3ae,-0x3b5,-0x3bb, // 150-160
-0x3c2,-0x3c8,-0x3cd,-0x3d3,-0x3d8,-0x3dd,-0x3e1,-0x3e5,-0x3e9,-0x3ed, // 160-170
-0x3f0,-0x3f3,-0x3f6,-0x3f8,-0x3fa,-0x3fc,-0x3fd,-0x3fe,-0x3ff,-0x3ff, // 170-180
-0x3ff,-0x3ff,-0x3ff,-0x3fe,-0x3fd,-0x3fc,-0x3fa,-0x3f8,-0x3f6,-0x3f3, // 180-190
-0x3f0,-0x3ed,-0x3e9,-0x3e5,-0x3e1,-0x3dd,-0x3d8,-0x3d3,-0x3cd,-0x3c8, // 190-200
-0x3c2,-0x3bb,-0x3b5,-0x3ae,-0x3a7,-0x3a0,-0x398,-0x390,-0x388,-0x37f, // 200-210
-0x376,-0x36d,-0x364,-0x35a,-0x350,-0x346,-0x33c,-0x331,-0x326,-0x31b, // 210-220
-0x310,-0x304,-0x2f8,-0x2ec,-0x2e0,-0x2d4,-0x2c7,-0x2ba,-0x2ad,-0x29f, // 220-230
-0x292,-0x284,-0x276,-0x268,-0x259,-0x24b,-0x23c,-0x22d,-0x21e,-0x20f, // 230-240
-0x200,-0x1f0,-0x1e0,-0x1d0,-0x1c0,-0x1b0,-0x1a0,-0x190,-0x17f,-0x16e, // 240-250
-0x15e,-0x14d,-0x13c,-0x12b,-0x11a,-0x109,-0x0f7,-0x0e6,-0x0d4,-0x0c3, // 250-260
-0x0b1,-0x0a0,-0x08e,-0x07c,-0x06b,-0x059,-0x047,-0x035,-0x023,-0x011, // 260-270
 0x000, 0x011, 0x023, 0x035, 0x047, 0x059, 0x06b, 0x07c, 0x08e, 0x0a0, // 270-280
 0x0b1, 0x0c3, 0x0d4, 0x0e6, 0x0f7, 0x109, 0x11a, 0x12b, 0x13c, 0x14d, // 280-290
 0x15e, 0x16e, 0x17f, 0x190, 0x1a0, 0x1b0, 0x1c0, 0x1d0, 0x1e0, 0x1f0, // 290-300
 0x1ff, 0x20f, 0x21e, 0x22d, 0x23c, 0x24b, 0x259, 0x268, 0x276, 0x284, // 300-310
 0x292, 0x29f, 0x2ad, 0x2ba, 0x2c7, 0x2d4, 0x2e0, 0x2ec, 0x2f8, 0x304, // 310-320
 0x310, 0x31b, 0x326, 0x331, 0x33c, 0x346, 0x350, 0x35a, 0x364, 0x36d, // 320-330
 0x376, 0x37f, 0x388, 0x390, 0x398, 0x3a0, 0x3a7, 0x3ae, 0x3b5, 0x3bb, // 330-340
 0x3c2, 0x3c8, 0x3cd, 0x3d3, 0x3d8, 0x3dd, 0x3e1, 0x3e5, 0x3e9, 0x3ed, // 340-350
 0x3f0, 0x3f3, 0x3f6, 0x3f8, 0x3fa, 0x3fc, 0x3fd, 0x3fe, 0x3ff, 0x3ff, // 350-360
};
#ifdef    LCDSIZE_240_320
static const S32 rotate_width_cache[LCD_HEIGHT] = 
{
   0, 240, 480, 720, 960, 1200, 1440, 1680, 1920, 2160, // 000-010
 2400, 2640, 2880, 3120, 3360, 3600, 3840, 4080, 4320, 4560, // 010-020
 4800, 5040, 5280, 5520, 5760, 6000, 6240, 6480, 6720, 6960, // 020-030
 7200, 7440, 7680, 7920, 8160, 8400, 8640, 8880, 9120, 9360, // 030-040
 9600, 9840, 10080, 10320, 10560, 10800, 11040, 11280, 11520, 11760, // 040-050
 12000, 12240, 12480, 12720, 12960, 13200, 13440, 13680, 13920, 14160, // 050-060
 14400, 14640, 14880, 15120, 15360, 15600, 15840, 16080, 16320, 16560, // 060-070
 16800, 17040, 17280, 17520, 17760, 18000, 18240, 18480, 18720, 18960, // 070-080
 19200, 19440, 19680, 19920, 20160, 20400, 20640, 20880, 21120, 21360, // 080-090
 21600, 21840, 22080, 22320, 22560, 22800, 23040, 23280, 23520, 23760, // 090-100
 24000, 24240, 24480, 24720, 24960, 25200, 25440, 25680, 25920, 26160, // 100-110
 26400, 26640, 26880, 27120, 27360, 27600, 27840, 28080, 28320, 28560, // 110-120
 28800, 29040, 29280, 29520, 29760, 30000, 30240, 30480, 30720, 30960, // 120-130
 31200, 31440, 31680, 31920, 32160, 32400, 32640, 32880, 33120, 33360, // 130-140
 33600, 33840, 34080, 34320, 34560, 34800, 35040, 35280, 35520, 35760, // 140-150
 36000, 36240, 36480, 36720, 36960, 37200, 37440, 37680, 37920, 38160, // 150-160
 38400, 38640, 38880, 39120, 39360, 39600, 39840, 40080, 40320, 40560, // 160-170
 40800, 41040, 41280, 41520, 41760, 42000, 42240, 42480, 42720, 42960, // 170-180
 43200, 43440, 43680, 43920, 44160, 44400, 44640, 44880, 45120, 45360, // 180-190
 45600, 45840, 46080, 46320, 46560, 46800, 47040, 47280, 47520, 47760, // 190-200
 48000, 48240, 48480, 48720, 48960, 49200, 49440, 49680, 49920, 50160, // 200-210
 50400, 50640, 50880, 51120, 51360, 51600, 51840, 52080, 52320, 52560, // 210-220
 52800, 53040, 53280, 53520, 53760, 54000, 54240, 54480, 54720, 54960, // 220-230
 55200, 55440, 55680, 55920, 56160, 56400, 56640, 56880, 57120, 57360, // 230-240
 57600, 57840, 58080, 58320, 58560, 58800, 59040, 59280, 59520, 59760, // 240-250
 60000, 60240, 60480, 60720, 60960, 61200, 61440, 61680, 61920, 62160, // 250-260
 62400, 62640, 62880, 63120, 63360, 63600, 63840, 64080, 64320, 64560, // 260-270
 64800, 65040, 65280, 65520, 65760, 66000, 66240, 66480, 66720, 66960, // 270-280
 67200, 67440, 67680, 67920, 68160, 68400, 68640, 68880, 69120, 69360, // 280-290
 69600, 69840, 70080, 70320, 70560, 70800, 71040, 71280, 71520, 71760, // 290-300
 72000, 72240, 72480, 72720, 72960, 73200, 73440, 73680, 73920, 74160, // 300-310
 74400, 74640, 74880, 75120, 75360, 75600, 75840, 76080, 76320, 76560, // 310-320
 };
#endif
static S32 cosa_cache[LCD_HEIGHT];
static S32 sina_cache[LCD_HEIGHT];


void rotate_angle(U16* dest_ptr, U16* src_ptr, U16 width, U16 height, U16 angle)
{
	S32 f1, f2;
	S32 fCosa, fSina;
	S32 i, j, i0, j0;

	S32 temp1, temp2;

//	ASSERT(angle < 360);
	fSina = sinTbl[angle];
	fCosa = cosTbl[angle];

	cosa_cache[0] = 0;
	sina_cache[0] = 0;

	for(i = 1; i < height; i++)
	{
		cosa_cache[i] = cosa_cache[i - 1] + fCosa;
		sina_cache[i] = sina_cache[i - 1] + fSina;
	}

	f1 = (-cosa_cache[width - 1] - sina_cache[height - 1] + ((width - 1) << 10)) >> 1;
	f2 = ( sina_cache[width - 1] - cosa_cache[height - 1] + ((height - 1) << 10)) >> 1;

	for(i = 0; i < height; i++)
	{
		temp1 = cosa_cache[i] + f2 + 500;
		temp2 = sina_cache[i] + f1 + 500;
		for(j = 0; j < width; j++)
		{
			i0 = (temp1 - sina_cache[j]) >> 10;
			j0 = (temp2 + cosa_cache[j]) >> 10;

			if(j0 < 0 || j0 >= width || i0 < 0 || i0 >= height)
			{
				*dest_ptr = GDI_COLOR_BLACK;
			}
			else
			{
			#ifdef    LCDSIZE_240_320
				*dest_ptr = *(src_ptr + rotate_width_cache[i0] + j0);
			#else
				*dest_ptr = *(src_ptr + LCD_WIDTH*(i0) + j0);
			#endif
			}
			dest_ptr++;
		}
	}
}

void RotateNormal(U8* pixel, U16 width, U16 height,gdi_image_cache_bmp_struct* changePixel,U8  angle)
{
	// 原始图像四个角的坐标
	int srcX1, srcX2, srcX3, srcX4;
	int srcY1, srcY2, srcY3, srcY4;
	// 计算旋转角度的正弦和余弦值
	double gfSin[7] = {0,0.866,0.866,0,-0.866,-0.866,1};
	double gfCos[7] = {1,0.5,-0.5,-1,-0.5,0.5,0};
	double fSin = gfSin[angle];
	double fCos = gfCos[angle];

	// 图像经过旋转后四个角的坐标
	double tranX1, tranX2, tranX3, tranX4;
	double tranY1, tranY2, tranY3, tranY4;
	double num1,num2;
	double tempx,tempy;
	int tempWidth, tempoutWidth;

	U8*	copyPixel = NULL;	// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;	// 指向被复制像素的起始位置
	int		x = 0;				// 变换后的像素横坐标
	int		y = 0;				// 变换后的像素纵坐标
	int		i,j;
	U8* tempPixel;
	int outWidth, outHeight;
	S32 U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;
	srcX1 = 0;
	srcY1 = 0;
	srcX2 = width - 1;
	srcY2 = 0;
	srcX3 = 0;
	srcY3 = height - 1;
	srcX4 = width - 1;
	srcY4 = height - 1;

	

	tranX1 = fCos * srcX1 + fSin * srcY1;
	tranY1 = -fSin * srcX1 + fCos * srcY1;
	tranX2 = fCos * srcX2 + fSin * srcY2;
	tranY2 = -fSin * srcX2 + fCos * srcY2;
	tranX3 = fCos * srcX3 + fSin * srcY3;
	tranY3 = -fSin * srcX3 + fCos * srcY3;
	tranX4 = fCos * srcX4 + fSin * srcY4;
	tranY4 = -fSin * srcX4 + fCos * srcY3;

	// 计算旋转后图像的大小
	outWidth = (U16)( max( fabf(tranX4-tranX1), fabf(tranX3-tranX2) ) + 1.5 );
	outHeight = (U16)( max( fabf(tranY4-tranY1), fabf(tranY3-tranY2) ) + 1.5 );

	tempPixel = OslMalloc(outWidth*outHeight*U8_per_point);
	memset(tempPixel,200,outWidth*outHeight*U8_per_point);
	// 计算两个常量
	 num1 = -0.5*outWidth*fCos - 0.5*outHeight*fSin + 0.5*width;
	 num2 = 0.5*outWidth*fSin - 0.5*outHeight*fCos + 0.5*height;

	tempWidth = width * U8_per_point;
	tempoutWidth = outWidth * U8_per_point;
	for ( j = 0; j < outHeight; j++)
	{
		tempx = j * fSin + num1 + 0.5;
		tempy = j * fCos + num2 + 0.5;
		for ( i = 0; i < outWidth; i++) 
		{
			x = (int)(i * fCos + tempx);
			y = (int)(-i * fSin + tempy);

			// 修正坐标
			if (x == width)
				x--;
			if (y == height)
				y--;

			copyPixel = pixel + y * tempWidth + x * U8_per_point;
			objPixel = tempPixel + j * tempoutWidth + i * U8_per_point;

			if (x >= 0 && x < width && y >= 0 && y < height)
				{
				   *(U16*)(objPixel) = *(U16*)(copyPixel);
				}
		}
	}
	changePixel->buf_ptr = tempPixel;
	changePixel->width = outWidth;
	changePixel->height = outHeight;
}

void test_Display(void)
{

gdi_image_cache_bmp_struct ChangeTbmp_ptr_temp;
gui_lock_double_buffer();
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    //gdi_image_cache_bmp_draw(0,0, &oldWinBmpbuffer);
	    gdi_draw_solid_rect(0, 0 , UI_device_width-1, UI_device_height-1, GDI_COLOR_WHITE);
	    gdi_layer_pop_clip();
//	    ChangeTbmp_ptr.height = 160;
//           ChangeTbmp_ptr.width = 120;
//	    ChangeTbmp_ptr.buf_ptr = OslMalloc(ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*4);
//	    memset(ChangeTbmp_ptr.buf_ptr,255,ChangeTbmp_ptr.height*ChangeTbmp_ptr.width*4);
//	    CSD_UI_change_bmp(&newWinBmpbuffer,&ChangeTbmp_ptr);
	        ChangeTbmp_ptr_temp.height = LCD_HEIGHT*1/10;
    ChangeTbmp_ptr_temp.width =LCD_WIDTH*1/10;
    if(ChangeTbmp_ptr_temp.width != 0)
    	{
	    ChangeTbmp_ptr_temp.buf_ptr = OslMalloc(ChangeTbmp_ptr_temp.height*ChangeTbmp_ptr_temp.width*2);
//	    CSD_UI_change_bmp_ptr(&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    ZoomNormal(newWinBmpbuffer.buf_ptr,ChangeTbmp_ptr_temp.buf_ptr,newWinBmpbuffer.width,newWinBmpbuffer.height ,ChangeTbmp_ptr_temp.width,ChangeTbmp_ptr_temp.height );

    	}
//	    RotateNormal(ChangeTbmp_ptr_temp.buf_ptr,ChangeTbmp_ptr_temp.width,ChangeTbmp_ptr_temp.height ,&ChangeTbmp_ptr,5 );
//	    Transpose(ChangeTbmp_ptr.buf_ptr,oldWinBmpbuffer.buf_ptr,ChangeTbmp_ptr.width,ChangeTbmp_ptr.height ,oldWinBmpbuffer.width,oldWinBmpbuffer.height );
//	     memcpy(oldWinBmpbuffer.buf_ptr, ChangeTbmp_ptr.buf_ptr, LCD_WIDTH*LCD_HEIGHT*2);
	//	ChangeTbmp_ptr.height = LCD_HEIGHT;
       //       ChangeTbmp_ptr.width = LCD_WIDTH;
	//    CSD_UI_change_bmp_ptr(&oldWinBmpbuffer,&ChangeTbmp_ptr);
	    gdi_layer_push_clip();
	    gdi_layer_set_clip(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	    gdi_image_cache_bmp_draw(0,0, &ChangeTbmp_ptr_temp);
	    gdi_layer_pop_clip();
	 if(ChangeTbmp_ptr_temp.buf_ptr != NULL)
	     		 OslMfree(ChangeTbmp_ptr_temp.buf_ptr);
		if(ChangeTbmp_ptr.buf_ptr != NULL)
	     		 OslMfree(ChangeTbmp_ptr.buf_ptr);




 gui_unlock_double_buffer();
 gui_BLT_double_buffer(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

}
#include"mainmenudef.h"
#include"simdetectiondef.h"
#include"idleappdef.h"
#include"stopwatchdef.h"
#include"alarmdef.h"
#include"poweronoffdisplaydefs.h"
#include"commonscreensenum.h"
#include"messagesresourcedata.h"
#include"callmanagementiddef.h"
#include"phonebookdef.h"
void CSD_UI_change_window(void)
{
{
if((g_ActiveScreenId != g_OldActiveScreenId)&&
	(g_ActiveScreenId != POPUP_SCREENID)&&(g_OldActiveScreenId != POPUP_SCREENID)&&
#ifdef __MMI_STOPWATCH__	
	(g_ActiveScreenId != SCR_SPLIT_TIMING)&&(g_OldActiveScreenId != SCR_SPLIT_TIMING)&&
#endif
	(g_ActiveScreenId != SCR_ID_ALM_LIST)&&(g_OldActiveScreenId != SCR_ID_ALM_LIST) &&
	(g_ActiveScreenId != SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW)&&(g_OldActiveScreenId != SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW)&&
	(g_ActiveScreenId != SCR_CONFIRM_SCREEN)&&(g_OldActiveScreenId != SCR_CONFIRM_SCREEN)&&
	(g_ActiveScreenId != SCR_POPUP_SCREEN)&&(g_OldActiveScreenId != SCR_POPUP_SCREEN)&&
	(g_ActiveScreenId != SCR_ID_MSG_PROCESSING)&&(g_OldActiveScreenId != SCR_ID_MSG_PROCESSING)&&
	(g_ActiveScreenId != SCR_CM_REQUESTINGUSSD)&&(g_OldActiveScreenId != SCR_CM_REQUESTINGUSSD)&&
	(g_ActiveScreenId != SCR_ID_PHB_SORTING_EMAIL)&&(g_OldActiveScreenId != SCR_ID_PHB_SORTING_EMAIL)&&
	(g_ActiveScreenId != SCR_IN_PROGRESS_PHB)&&(g_OldActiveScreenId != SCR_IN_PROGRESS_PHB)&&
	(g_ActiveScreenId != SCR_COPY_ALL_PROGRESS)&&(g_OldActiveScreenId != SCR_COPY_ALL_PROGRESS)&&
	(g_ActiveScreenId != SCR_ID_MSG_SENDING)&&(g_OldActiveScreenId != SCR_ID_MSG_SENDING)&&
	(g_ActiveScreenId != IDLE_SCREEN_DIGIT_HANDLER_ID)&&(g_OldActiveScreenId != IDLE_SCREEN_DIGIT_HANDLER_ID)
)
{
if((enablechange_alloc_ok_flag)&&(arrived_idle_enablechange_window_flag)&&(enablechange_window_flag))
{

//if(g_ActiveScreenId != g_OldActiveScreenId)
/*
if(((g_ActiveScreenId == MAIN_MENU_SCREENID)||(g_OldActiveScreenId == MAIN_MENU_SCREENID))&&(g_ActiveScreenId != g_OldActiveScreenId))
{
	change_window_flag = 1;
	 CSD_UI_get_new_window();

change_window_style = 20;
}*/

switch (change_window_style)
    {
    
        case 0:
	//	  CSD_UI_change_window_action();
//        break;

        case 1:
	//	   CSD_UI_change_window_action_01();
//        break;

	 case 2:
	//	   CSD_UI_change_window_action_02();
//        break;
		
        case 3:
	//	   CSD_UI_change_window_action_03();
//break;
	case 4:
	//	   CSD_UI_change_window_action_04();
//        break;

	 case 5:
	//	   CSD_UI_change_window_action_05();
//break;
		
        case 6:
	//	  CSD_UI_change_window_action_06();
//        break;

	case 7:
	//	  CSD_UI_change_window_action_07();
//        break;

	case 8:
	//	  CSD_UI_change_window_action_08();
//        break;
	case 9:
	//	  CSD_UI_change_window_action_09();
        break;

	case 10:
		change_window_flag = 1;
		  CSD_UI_change_window_action_28_3D();
		  break;
	case 11:
		change_window_flag = 1;
		  CSD_UI_change_window_action_11();
		  break;
		  
	case 12:
		change_window_flag = 1;
		  CSD_UI_change_window_action_12();/*缩小*/
		  break;
	case 13:
		change_window_flag = 1;
		  CSD_UI_change_window_action_13();/*放大*/
		  break;
		  
	case 14:
		change_window_flag = 1;
		  CSD_UI_change_window_action_14();/*左移*/
		  break;
	case 15:
		change_window_flag = 1;
		  CSD_UI_change_window_action_15();/*右移*/
		  break;

	case 16:
		change_window_flag = 1;
		  CSD_UI_change_window_action_16();/*右移底层不动*/
		  break;
	case 17:
		change_window_flag = 1;
		  CSD_UI_change_window_action_17();/*左移底层不动*/
		  break;
	case 18:
		change_window_flag = 1;
		  CSD_UI_change_window_action_18();/*向下放*/
		  break;
	case 19:
		change_window_flag = 1;
		  CSD_UI_change_window_action_19();/*向上卷*/
		  break;
	case 20:
		change_window_flag = 1;
		  CSD_UI_change_window_action_20();/*顺时针旋转*/
		  break;
	case 21:
		change_window_flag = 1;
		  CSD_UI_change_window_action_21();/*逆时针旋转*/
		  break;
	case 22:
		change_window_flag = 1;
		  CSD_UI_change_window_action_22();/*上翻*/
		  break;
	case 23:
		change_window_flag = 1;
		  CSD_UI_change_window_action_23();/*下翻*/
		  break;
	case 24:
//		  CSD_UI_change_window_action_24();/*顺时针旋转缩小*/
		  break;
	case 25:
//		  CSD_UI_change_window_action_25();/*缩小消失*/
		  break;
	case 26:
//		  CSD_UI_change_window_action_26();/*缩小消失*/
		  break;
	case 28:
//		  CSD_UI_change_window_action_30();/*窗口混色渐变*/
		  break;

		  break;
		  
        default:
        break;
    }
}
}
}
}
#endif


