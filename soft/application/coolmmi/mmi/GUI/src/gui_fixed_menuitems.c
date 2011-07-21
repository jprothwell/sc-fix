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
 *   gui_fixed_menuitems.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Menu items in fixed menu - UI component
 *   [Text menu item]
 *   [Icontext menu item]
 *   [Icontext-list menu item]
 *   [Twostate menu item]
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

/**********************************************************************************
   Filename:      gui_fixed_menuitems.c
   Date Created:  July-22-2002
   Contains:      UI routines for Fixed Menuitems
**********************************************************************************/

#include "mmi_features.h"
#include "gui_fixed_menuitems.h"
#include "gui_common_menus.h"
#include "kal_non_specific_general_types.h"
#include "gdi_include.h"
#include "lcd_sw_rnd.h"
#include "wgui.h"
#include "custmenures.h"
/* For R2L characters */
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-7 15:26 */
#ifdef __MMI_BIDI_ALG__
#include "bididef.h"
#include "bidiprot.h"
#endif
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-7 15:26 */
#include "ucs2prot.h"
#include "wgui_fixed_menuitems.h"

#include "debuginitdef.h"

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
#include "mmi_features.h"       /* add for effect type entry for title & highlighter */
#include "wgui_categories_defs.h"
#include "wgui_categories_util.h"

/* For list highlighter effects */
static S32 flash_feel_last_frame = 0;
static S32 gblock_list_effect = 0;
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 

/* Text scrolling related data      */

S32 UI_text_menuitem_scroll_time = UI_TEXT_MENUITEM_SCROLL_TIME;
S32 UI_text_menuitem_scroll_size = UI_TEXT_MENUITEM_SCROLL_SIZE;
S32 UI_text_menuitem_scroll_gap = UI_TEXT_MENUITEM_SCROLL_GAP;
S32 UI_text_menuitem_scroll_pause = UI_TEXT_MENUITEM_SCROLL_PAUSE;

 
UI_filled_area colorangewgui_pop_up_dialog_background =
    {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_VERTICAL_FILL | UI_FILLED_AREA_SINGLE_BORDER,
    UI_NULL_IMAGE,
    NULL,
    {255, 220, 185, 100},
    {0, 0, 0, 0},
    {255, 163, 70, 100},
    {50, 50, 50, 100},
    0
};

extern PU8 MMI_mm_animation;

extern BOOL r2lMMIFlag;

#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)
#define GUI_TWO_LINE_TEXT_ICON_GAP 5
#endif 

#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__

BOOL inline_bottom_line_draw;
BOOL line_draw;

#define TEXT_ICON_GAP 5 /* for display gap of text from grid */
extern fixed_list_menu MMI_fixed_list_menu;
extern MMI_theme *current_MMI_theme;


/*****************************************************************************
 * FUNCTION
 *  gui_disable_fixed_list_line_draw
 * DESCRIPTION
 *  function for disable line draw
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_disable_fixed_list_line_draw(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    line_draw = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_enable_fixed_list_line_draw
 * DESCRIPTION
 *  function for disable line draw
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_enable_fixed_list_line_draw(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    line_draw = 1;
}


/*****************************************************************************
 * FUNCTION
 *  gui_disable_inline_bottom_line_draw
 * DESCRIPTION
 *  function for disable inline bottom line draw for inline item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_disable_inline_bottom_line_draw(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_bottom_line_draw = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_enable_inline_bottom_line_draw
 * DESCRIPTION
 *  function for inable bottom line draw for inline item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_enable_inline_bottom_line_draw(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_bottom_line_draw = 1;
}

#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 

/* Pop up description related data  */


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_display_pop_up_description
 * DESCRIPTION
 *  dummy function of display popup
 * PARAMETERS
 *  x           [IN]        Start x position
 *  y           [IN]        Start y position
 *  width       [IN]        Width of popup
 *  height      [IN]        Hieght of popup
 * RETURNS
 *  void
 *****************************************************************************/
void UI_dummy_display_pop_up_description(S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
    UI_UNUSED_PARAMETER(width);
    UI_UNUSED_PARAMETER(height);
}

#ifdef __MMI_UI_HINTS_IN_MENUITEM__


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_get_current_pop_up_description_string
 * DESCRIPTION
 *  dummy function to get popup description string
 * PARAMETERS
 *  void
 * RETURNS
 *  string to be displayed
 *****************************************************************************/
UI_string_type UI_dummy_get_current_pop_up_description_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return NULL;
}

UI_string_type(*gui_get_current_pop_up_description_string) (void) = UI_dummy_get_current_pop_up_description_string;
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

void (*gui_display_pop_up_description) (S32 x, S32 y, S32 width, S32 height) = UI_dummy_display_pop_up_description;

#ifdef __MMI_UI_TRANSPARENT_EFFECT__

static BOOL block_transparency = FALSE;


/*****************************************************************************
 * FUNCTION
 *  gui_block_transparency_effect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_block_transparency_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    block_transparency = TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  gui_unblock_transparency_effect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_unblock_transparency_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    block_transparency = FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_enable_transparent_effect
 * DESCRIPTION
 *  enable the transparent effect flag
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
UI_filled_area non_transparent_focussed_filler;
UI_filled_area *backup_focussed_filler;
S32 transparency_in_list = 0;
void gui_fixed_icontext_enable_transparent_effect(fixed_icontext_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (block_transparency)
    {
        return;
    }

    /* to enable transparency only when it is required */
    if (transparency_in_list)
    {
        m->focussed_filler = backup_focussed_filler;
    }
    transparency_in_list = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_disable_transparent_effect
 * DESCRIPTION
 *  Disable the transparent effect flag
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_disable_transparent_effect(fixed_icontext_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (block_transparency)
    {
        return;
    }
    /* disable transparency only when it is enabled */
    if (m->focussed_filler->flags & UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR)
    {
        transparency_in_list = 1;
        backup_focussed_filler = m->focussed_filler;
        non_transparent_focussed_filler = *backup_focussed_filler;
        m->focussed_filler = &non_transparent_focussed_filler;
        m->focussed_filler->flags = m->focussed_filler->flags & ~UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_list_enable_transparent_effect
 * DESCRIPTION
 *  enable the transparent effect flag
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_list_enable_transparent_effect(fixed_icontext_list_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (block_transparency)
    {
        return;
    }
    if (transparency_in_list)
    {
        m->focussed_filler = backup_focussed_filler;
    }
    transparency_in_list = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_list_disable_transparent_effect
 * DESCRIPTION
 *  Disable the transparent effect flag
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_list_disable_transparent_effect(fixed_icontext_list_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (block_transparency)
    {
        return;
    }
    if (m->focussed_filler->flags & UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR)
    {
        transparency_in_list = 1;
        backup_focussed_filler = m->focussed_filler;
        non_transparent_focussed_filler = *backup_focussed_filler;
        m->focussed_filler = &non_transparent_focussed_filler;
        m->focussed_filler->flags = m->focussed_filler->flags & ~UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_text_menuitem_enable_transparent_effect
 * DESCRIPTION
 *  enable the transparent effect flag
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_text_menuitem_enable_transparent_effect(fixed_text_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (block_transparency)
    {
        return;
    }
    if (transparency_in_list)
    {
        m->focussed_filler = backup_focussed_filler;
    }
    transparency_in_list = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_text_menuitem_disable_transparent_effect
 * DESCRIPTION
 *  Disable the transparent effect flag
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_text_menuitem_disable_transparent_effect(fixed_text_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (block_transparency)
    {
        return;
    }
    if (m->focussed_filler->flags & UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR)
    {
        transparency_in_list = 1;
        backup_focussed_filler = m->focussed_filler;
        non_transparent_focussed_filler = *backup_focussed_filler;
        m->focussed_filler = &non_transparent_focussed_filler;
        m->focussed_filler->flags = m->focussed_filler->flags & ~UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_twostate_menuitem_enable_transparent_effect
 * DESCRIPTION
 *  enable the transparent effect flag
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_twostate_menuitem_enable_transparent_effect(fixed_twostate_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (block_transparency)
    {
        return;
    }
    if (transparency_in_list)
    {
        m->focussed_filler = backup_focussed_filler;
    }
    transparency_in_list = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_twostate_menuitem_disable_transparent_effect
 * DESCRIPTION
 *  Disable the transparent effect flag
 * PARAMETERS
 *  m       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_twostate_menuitem_disable_transparent_effect(fixed_twostate_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (block_transparency)
    {
        return;
    }
    if (m->focussed_filler->flags & UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR)
    {
        transparency_in_list = 1;
        backup_focussed_filler = m->focussed_filler;
        non_transparent_focussed_filler = *backup_focussed_filler;
        m->focussed_filler = &non_transparent_focussed_filler;
        m->focussed_filler->flags = m->focussed_filler->flags & ~UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
    }
}

#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 

/* Fixed menuitem dummy functions: Do not delete any of these functions */


/*****************************************************************************
 * FUNCTION
 *  UI_fixed_menuitem_dummy_display_function
 * DESCRIPTION
 *  dummy func of UI_fixed_menuitem_dummy_display_function
 * PARAMETERS
 *  item                    [IN]        
 *  common_item_data        [IN]        
 *  x                       [IN]        
 *  y                       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_fixed_menuitem_dummy_display_function(void *item, void *common_item_data, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(common_item_data);
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
}


/*****************************************************************************
 * FUNCTION
 *  UI_fixed_menuitem_dummy_hide_function
 * DESCRIPTION
 *  dummy hide func
 * PARAMETERS
 *  item                    [IN]        
 *  common_item_data        [IN]        
 *  x                       [IN]        
 *  y                       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_fixed_menuitem_dummy_hide_function(void *item, void *common_item_data, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(common_item_data);
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
}


/*****************************************************************************
 * FUNCTION
 *  UI_fixed_menuitem_dummy_measure_function
 * DESCRIPTION
 *  dummy measure  func
 * PARAMETERS
 *  item                    [IN]        
 *  common_item_data        [IN]        
 *  width                   [?]         
 *  height                  [?]         
 *  x(?)                    [IN]        
 *  y(?)                    [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_fixed_menuitem_dummy_measure_function(void *item, void *common_item_data, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(common_item_data);
    UI_UNUSED_PARAMETER(width);
    UI_UNUSED_PARAMETER(height);
}


/*****************************************************************************
 * FUNCTION
 *  UI_fixed_menuitem_dummy_highlight_function
 * DESCRIPTION
 *  dummy highlight  func
 * PARAMETERS
 *  item                    [IN]        
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_fixed_menuitem_dummy_highlight_function(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(common_item_data);
}


/*****************************************************************************
 * FUNCTION
 *  UI_fixed_menuitem_dummy_remove_highlight_function
 * DESCRIPTION
 *  dummy remove highlight  func
 * PARAMETERS
 *  item                    [IN]        
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_fixed_menuitem_dummy_remove_highlight_function(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(common_item_data);
}


/*****************************************************************************
 * FUNCTION
 *  UI_fixed_menuitem_dummy_resize_function
 * DESCRIPTION
 *  dummy resize function
 * PARAMETERS
 *  x       [IN]        
 *  y       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_fixed_menuitem_dummy_resize_function(S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  UI_fixed_menuitem_dummy_pen_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [?]         
 *  item_x                  [IN]        
 *  item_y                  [IN]        
 *  pen_event               [IN]        
 *  x                       [IN]        
 *  y                       [IN]        
 *  menuitem_event          [?]         
 *  menuitem_param          [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL UI_fixed_menuitem_dummy_pen_function(
        void *item,
        void *common_item_data,
        S32 item_x,
        S32 item_y,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_list_pen_enum *menuitem_event,
        gui_pen_event_param_struct *menuitem_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(common_item_data);
    UI_UNUSED_PARAMETER(pen_event);
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
    UI_UNUSED_PARAMETER(menuitem_event);
    UI_UNUSED_PARAMETER(menuitem_param);
    return MMI_FALSE;
}
#endif /* __MMI_TOUCH_SCREEN__ */ 

/* GUI: fixed text menuitem functions              */

UI_fixed_text_menuitem_theme *current_fixed_text_menuitem_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_text_menuitem_theme
 * DESCRIPTION
 *  Applies the given theme to a fixed text menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed text menuitem object   (common data)
 *  t       [IN]        Is the theme
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
UI_filled_area fixed_text_menuitem_transparent_filled_area;
#endif 

void gui_set_fixed_text_menuitem_theme(fixed_text_menuitem *m, UI_fixed_text_menuitem_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->normal_filler = t->normal_filler;
    m->disabled_filler = t->disabled_filler;
    m->selected_filler = t->selected_filler;
    m->focussed_filler = t->focussed_filler;
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    /* This is to replace the UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR flag and alpha value from the themecomponent.h */
    if (!m->focussed_filler->flags & UI_FILLED_AREA_TYPE_BITMAP)
    {
        fixed_text_menuitem_transparent_filled_area = *(m->focussed_filler);
        m->focussed_filler = &fixed_text_menuitem_transparent_filled_area;
        fixed_text_menuitem_transparent_filled_area.flags |= UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
        fixed_text_menuitem_transparent_filled_area.c.alpha = HIGHLIGHTER_COLOR_OPACITY_VALUE;
    }
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
    m->normal_text_color = t->normal_text_color;
    m->disabled_text_color = t->disabled_text_color;
    m->selected_text_color = t->selected_text_color;
    m->focussed_text_color = t->focussed_text_color;
    m->text_font = t->text_font;
    m->flags |= t->flags;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_text_menuitem_current_theme
 * DESCRIPTION
 *  Applies the current theme to a fixed text menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed text menuitem object   (common data)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_text_menuitem_current_theme(fixed_text_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* set th normal filler color of menu item equal to current theme */
    m->normal_filler = current_fixed_text_menuitem_theme->normal_filler;

    m->disabled_filler = current_fixed_text_menuitem_theme->disabled_filler;
    m->selected_filler = current_fixed_text_menuitem_theme->selected_filler;
    m->focussed_filler = current_fixed_text_menuitem_theme->focussed_filler;
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    /* This is to replace the UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR flag and alpha value from the themecomponent.h */
    if (!m->focussed_filler->flags & UI_FILLED_AREA_TYPE_BITMAP)
    {
        fixed_text_menuitem_transparent_filled_area = *(m->focussed_filler);
        m->focussed_filler = &fixed_text_menuitem_transparent_filled_area;
        fixed_text_menuitem_transparent_filled_area.flags |= UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
    }
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
    m->normal_text_color = current_fixed_text_menuitem_theme->normal_text_color;
    m->disabled_text_color = current_fixed_text_menuitem_theme->disabled_text_color;
    m->selected_text_color = current_fixed_text_menuitem_theme->selected_text_color;
    m->focussed_text_color = current_fixed_text_menuitem_theme->focussed_text_color;
    m->text_font = current_fixed_text_menuitem_theme->text_font;
    m->flags |= current_fixed_text_menuitem_theme->flags;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_fixed_text_menuitem
 * DESCRIPTION
 *  Creates a fixed text menuitem
 * PARAMETERS
 *  m           [IN]        Is the fixed text menuitem object (common data)
 *  width       [IN]        Width is the width of the fixed text menuitem
 *  height      [IN]        Height is the height of the fixed text menuitem
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_fixed_text_menuitem(fixed_text_menuitem *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    m->flags = 0;
    m->ext_flags = 0;
    gui_set_fixed_text_menuitem_current_theme(m);
    m->text_x = 0;
    m->text_y = 0;
    m->scroll_x = 0;
    m->scroll_width = width;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_text_menuitem_set_text_position
 * DESCRIPTION
 *  Sets the text position within a fixed text menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed text menuitem object   (common data)
 *  x       [IN]        Is the position of the text (offset from the left-top corner of the menuitem)
 *  y       [IN]        Is the position of the text (offset from the left-top corner of the menuitem)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_text_menuitem_set_text_position(fixed_text_menuitem *m, S32 x, S32 y)
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
 *  gui_resize_fixed_text_menuitem
 * DESCRIPTION
 *  Changes the size of a fixed text menuitem
 * PARAMETERS
 *  m           [IN]        Is the fixed text menuitem object   (common data)
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_fixed_text_menuitem(fixed_text_menuitem *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    m->scroll_width = width;
}

/* Text scrolling related functions within the menu item */

fixed_text_menuitem *GUI_current_fixed_text_menuitem_common_data = NULL;
fixed_text_menuitem_type *GUI_current_fixed_text_menuitem_data = NULL;
U8 GUI_current_fixed_text_menuitem_scroll = 0;
U8 GUI_current_fixed_text_menuitem_scroll_direction;
S32 GUI_current_fixed_text_menuitem_x;
S32 GUI_current_fixed_text_menuitem_y;
S32 GUI_current_fixed_text_menuitem_scroll_counter = 0;
UI_string_type GUI_current_fixed_text_menuitem_scroll_text = NULL;
UI_string_type GUI_current_fixed_text_menuitem_scroll_text_end = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_text_menuitem_scroll_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_text_menuitem_scroll_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 flags = GUI_current_fixed_text_menuitem_common_data->flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags |= GUI_current_fixed_text_menuitem_data->flags;
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_reset_current_list_effect();
#endif 
    if (GUI_current_fixed_text_menuitem_scroll_counter)
    {
        GUI_current_fixed_text_menuitem_scroll_counter--;
    }
    else
    {
    #if(UI_USE_WORD_SCROLLING_TEXT)
        GUI_current_fixed_text_menuitem_scroll_text = GUI_current_fixed_text_menuitem_scroll_text_end;
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        if (flags & UI_MENUITEM_MARQUEE_SCROLL)
        {
            if (r2lMMIFlag)
            {
                GUI_current_fixed_text_menuitem_common_data->scroll_x += UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_text_menuitem_common_data->scroll_x >=
                    (GUI_current_fixed_text_menuitem_common_data->text_width + UI_text_menuitem_scroll_gap))
                {
                    GUI_current_fixed_text_menuitem_common_data->scroll_x = 0;
                }
            }
            else
            {
                GUI_current_fixed_text_menuitem_common_data->scroll_x -= UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_text_menuitem_common_data->scroll_x <=
                    (-(GUI_current_fixed_text_menuitem_common_data->text_width + UI_text_menuitem_scroll_gap)))
                {
                    GUI_current_fixed_text_menuitem_common_data->scroll_x = 0;
                }
            }
        }
        else
        {
            if (GUI_current_fixed_text_menuitem_scroll_direction == 0)
            {
                GUI_current_fixed_text_menuitem_common_data->scroll_x -= UI_text_menuitem_scroll_size;
                if ((GUI_current_fixed_text_menuitem_common_data->scroll_x +
                     GUI_current_fixed_text_menuitem_common_data->text_width) <=
                    (GUI_current_fixed_text_menuitem_common_data->scroll_width))
                {
                    GUI_current_fixed_text_menuitem_scroll_direction = 1;
                    GUI_current_fixed_text_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;
                }
            }
            else if (GUI_current_fixed_text_menuitem_scroll_direction == 1)
            {
                GUI_current_fixed_text_menuitem_common_data->scroll_x += UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_text_menuitem_common_data->scroll_x >= 0)
                {
                    GUI_current_fixed_text_menuitem_scroll_direction = 0;
                    GUI_current_fixed_text_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;
                }
            }
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        GUI_current_fixed_text_menuitem_common_data->ext_flags |= UI_MENUITEM_EXT_SHOW_IN_MARQUEE;

    #ifdef __MMI_UI_TRANSPARENT_EFFECT__
        if (!gui_is_current_transparency_with_multi_layer())
        {
            gui_fixed_text_menuitem_disable_transparent_effect(GUI_current_fixed_text_menuitem_common_data);    /* To disable transparency for scrolling text */
        }
    #endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 

        gui_show_fixed_text_menuitem(
            (void*)GUI_current_fixed_text_menuitem_data,
            (void*)GUI_current_fixed_text_menuitem_common_data,
            GUI_current_fixed_text_menuitem_x,
            GUI_current_fixed_text_menuitem_y);

    #ifdef __MMI_UI_TRANSPARENT_EFFECT__
        gui_fixed_text_menuitem_enable_transparent_effect(GUI_current_fixed_text_menuitem_common_data); /* To enable transparency */
    #endif 

        GUI_current_fixed_text_menuitem_common_data->ext_flags &= ~UI_MENUITEM_EXT_SHOW_IN_MARQUEE;
        gui_BLT_double_buffer(
            GUI_current_fixed_text_menuitem_x,
            GUI_current_fixed_text_menuitem_y,
            GUI_current_fixed_text_menuitem_x + GUI_current_fixed_text_menuitem_common_data->width - 1,
            GUI_current_fixed_text_menuitem_y + GUI_current_fixed_text_menuitem_common_data->height - 1);
    }
    gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_text_menuitem_scroll_handler);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_text_menuitem_start_scroll
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_text_menuitem_start_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_text_menuitem_type *t = (fixed_text_menuitem_type*) GUI_current_fixed_text_menuitem_data;
    fixed_text_menuitem *m = (fixed_text_menuitem*) GUI_current_fixed_text_menuitem_common_data;
    S32 w;
    U32 flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = m->flags;
    flags |= t->flags;
    gui_set_font(m->text_font);
    w = gui_get_string_width(t->text);
    m->text_width = w;
    m->scroll_x = 0;

    if (w >= (m->scroll_width - 1))
    {
    #if(UI_USE_WORD_SCROLLING_TEXT)
        GUI_current_fixed_text_menuitem_scroll_text = t->text;
        GUI_current_fixed_text_menuitem_scroll = 1;
        GUI_current_fixed_text_menuitem_scroll_direction = 0;
        GUI_current_fixed_text_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        if (gui_get_current_list_effect() && gblock_list_effect != 1)
        {
            gui_register_callback_menuitem_scroll(gui_fixed_text_menuitem_scroll_handler);
        }
        else
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
        {
            gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_text_menuitem_scroll_handler);
        }
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        if ((flags & UI_MENUITEM_MARQUEE_SCROLL) || (flags & UI_MENUITEM_TWO_DIRECTION_SCROLL))
        {
            GUI_current_fixed_text_menuitem_scroll = 1;
            GUI_current_fixed_text_menuitem_scroll_direction = 0;
            GUI_current_fixed_text_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;

        #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
            if (gui_get_current_list_effect() && gblock_list_effect != 1)
            {
                gui_register_callback_menuitem_scroll(gui_fixed_text_menuitem_scroll_handler);
            }
            else
        #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
            {
                gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_text_menuitem_scroll_handler);
            }
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
    }
    else
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    {
        gui_register_callback_menuitem_scroll(gui_dummy_scroll_handler_for_list_effects);
        GUI_current_fixed_text_menuitem_scroll = 0;
    }
#else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    {
        GUI_current_fixed_text_menuitem_scroll = 0;
    }
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_text_menuitem_stop_scroll
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_text_menuitem_stop_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GUI_current_fixed_text_menuitem_scroll)
    {
        gui_cancel_timer(gui_fixed_text_menuitem_scroll_handler);
        GUI_current_fixed_text_menuitem_scroll = 0;
    }

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_register_callback_menuitem_scroll(gui_dummy_scroll_handler_for_list_effects);
    gui_restore_current_list_effect();
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_fixed_text_menuitem
 * DESCRIPTION
 *  Displays the fixed text menuitem
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed text menuitem object
 *  common_item_data        [IN]        Is the fixed text menuitem object (common_data)
 *  x                       [IN]        Is the position at which the menuitem is displayed
 *  y                       [IN]        Is the position at which the menuitem is displayed
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_fixed_text_menuitem(void *item, void *common_item_data, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 text_x, text_y;
    color_t text_color;
    UI_filled_area *f = NULL;
    fixed_text_menuitem *m = (fixed_text_menuitem*) common_item_data;
    fixed_text_menuitem_type *mi = (fixed_text_menuitem_type*) item;
    U32 flags;
    UI_string_type _text;
    S32 sw, sh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = mi->flags;
    flags |= m->flags;
    _text = mi->text;

    if (flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        GUI_current_fixed_text_menuitem_x = x;
        GUI_current_fixed_text_menuitem_y = y;
    }

    /* Autocalculate the text position based on font   */
    gui_set_font(m->text_font);
    gui_measure_string(_text, &sw, &sh);
    if (flags & UI_MENUITEM_CENTER_TEXT_X)
    {
        if (m->width > sw)
        {
            m->text_x = (m->width >> 1) - (sw >> 1);
        }
        else
        {
            m->text_x = 0;
        }
    }
    if (flags & UI_MENUITEM_CENTER_TEXT_Y)
    {
        if (m->height > sh)
        {
            m->text_y = (m->height >> 1) - (sh >> 1);
        }
        else
        {
            m->text_y = 0;
        }
    }
    x1 = x;
    y1 = y;
    y2 = y1 + m->height - 1;
    x2 = x1 + m->width - 1;

    if (r2lMMIFlag)
    {
        text_x = m->width - 1 - m->text_x;
    }
    else
    {
        text_x = m->text_x;
    }
    text_y = m->text_y;

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    /* To hide the text of the menuitem up to the last frame for flash feel effect */
    if (gblock_list_effect == 0)
    {
        if (gui_get_current_list_effect() == MMI_UI_LIST_HIGHLIGHT_EFFECT_FLASH_FEELING)
        {
            if (flash_feel_last_frame == 0)
            {
                if (flags & UI_MENUITEM_STATE_FOCUSSED)
                {
                    flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY;
                }
            }
        }
    }
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 

    if (flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        text_x += m->scroll_x;
    }

    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && !(flags & UI_MENUITEM_DISABLE_HIGHLIGHT))
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS))
        {
            f = m->focussed_filler;
        }
        text_color = m->focussed_text_color;
    }
    else if (flags & UI_MENUITEM_STATE_DISABLED)
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND) && !(flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS))
        {
            f = m->disabled_filler;
        }
        text_color = m->focussed_text_color;
    }
    else if (flags & UI_MENUITEM_STATE_SELECTED)
    {
        f = m->selected_filler;
        text_color = m->selected_text_color;
    }
    else
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND) && !(flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS))
        {
            f = m->normal_filler;
        }
        text_color = m->normal_text_color;
    }
    if (f != NULL)
    {
   	#ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
   	#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        if (is_draw_next_frame_list_highlight_effect_value())
    #endif
        {
            g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_HIGHLIGHT_TEXT_ITEM;
        }
    #endif

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        set_start_position_and_item_parameters_for_list_highlighter_effect(item, common_item_data, x1, y1);
    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (r2lMMIFlag)
        {
            gui_draw_list_filled_area(x1 + 1, y1 + 1, x2, y2, f);
        }
        else
        {
            gui_draw_list_filled_area(x1, y1 + 1, x2 - 1, y2, f);
        }
    #else /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
        gui_draw_list_filled_area(x1, y1, x2, y2, f);
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    #else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
        gui_draw_filled_area(x1, y1, x2, y2, f);
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    }

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    /* To hide the text of the menuitem up to the last frame for flash feel effect */
    if (!
        ((gui_get_current_list_effect() == MMI_UI_LIST_HIGHLIGHT_EFFECT_FLASH_FEELING) &&
         (flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY) && (gblock_list_effect == 0)))
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    {
        gui_push_text_clip();
        gui_set_text_clip(x1, y1, x2, y2);
        gui_move_text_cursor(x1 + text_x, y1 + text_y);
        gui_set_line_height(sh);
        gui_set_text_color(text_color);

    #if(UI_USE_WORD_SCROLLING_TEXT)
        if ((flags & UI_MENUITEM_STATE_FOCUSSED) && GUI_current_fixed_text_menuitem_scroll)
        {
            GUI_current_fixed_text_menuitem_scroll_text_end = GUI_current_fixed_text_menuitem_scroll_text;
            if (gui_print_truncated_text2
                (x1 + text_x, y1 + text_y, m->width, &GUI_current_fixed_text_menuitem_scroll_text_end))
            {
                GUI_current_fixed_text_menuitem_scroll_text_end = _text;
            }
        }
        else
        {
            gui_print_truncated_text2(x1 + text_x, y1 + text_y, m->width, &_text);
        }
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        gui_print_text(_text);
        if ((flags & UI_MENUITEM_STATE_FOCUSSED) && (flags & UI_MENUITEM_MARQUEE_SCROLL) &&
            GUI_current_fixed_text_menuitem_scroll)
        {
            if (r2lMMIFlag)
            {
                gui_move_text_cursor(x1 + text_x - (m->text_width + UI_text_menuitem_scroll_gap), y1 + text_y);
            }
            else
            {
                gui_move_text_cursor(x1 + text_x + m->text_width + UI_text_menuitem_scroll_gap, y1 + text_y);
            }

            gui_set_line_height(sh);
            gui_print_text(_text);
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 

        gui_pop_text_clip();
    }
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && !(m->ext_flags & UI_MENUITEM_EXT_SHOW_IN_MARQUEE))
    {
        gui_display_pop_up_description(x, y, m->width, m->height);
    }
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_measure_fixed_text_menuitem
 * DESCRIPTION
 *  Measures a fixed text menuitem
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [IN]        Is the fixed text menuitem object (common_data)
 *  width                   [OUT]       
 *  height                  [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
void gui_measure_fixed_text_menuitem(void *item, void *common_item_data, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_text_menuitem *m = (fixed_text_menuitem*) common_item_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    *width = m->width;
    *height = m->height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_highlight_fixed_text_menuitem
 * DESCRIPTION
 *  Highlights a fixed text menuitem (not visually)
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed text menuitem object
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_highlight_fixed_text_menuitem(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_text_menuitem_type *t = (fixed_text_menuitem_type*) item;
    fixed_text_menuitem *m = (fixed_text_menuitem*) common_item_data;
    U32 flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = m->flags;
    flags |= t->flags;
    GUI_current_fixed_text_menuitem_common_data = m;
    GUI_current_fixed_text_menuitem_data = t;
    gui_fixed_text_menuitem_stop_scroll();
    gui_fixed_text_menuitem_start_scroll();

    if (t->flags & UI_MENUITEM_DISABLE_HIGHLIGHT)
    {
        return;
    }
    t->flags |= UI_MENUITEM_STATE_FOCUSSED;
}


/*****************************************************************************
 * FUNCTION
 *  gui_remove_highlight_fixed_text_menuitem
 * DESCRIPTION
 *  Removes the highlight of a fixed text menuitem (not visually)
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed text menuitem object
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_remove_highlight_fixed_text_menuitem(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_text_menuitem_type *t = (fixed_text_menuitem_type*) item;
    fixed_text_menuitem *m = (fixed_text_menuitem*) common_item_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(common_item_data);
    m->scroll_x = 0;
    t->flags &= ~UI_MENUITEM_STATE_FOCUSSED;
}

/* GUI: fixed icon text menuitem functions               */

UI_fixed_icontext_menuitem_theme *current_fixed_icontext_menuitem_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_icontext_menuitem_current_theme
 * DESCRIPTION
 *  Applies the current theme to a fixed icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed icontext menuitem object  (common data)
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
UI_filled_area fixed_icontext_menuitem_transparent_filled_area;
#endif 

void gui_set_fixed_icontext_menuitem_current_theme(fixed_icontext_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->disabled_filler = current_fixed_icontext_menuitem_theme->disabled_filler;
    m->disabled_text_color = current_fixed_icontext_menuitem_theme->disabled_text_color;
    m->flags |= current_fixed_icontext_menuitem_theme->flags;
    m->focussed_filler = current_fixed_icontext_menuitem_theme->focussed_filler;
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    /* This is to replace the UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR flag and alpha value from the themecomponent.h */
    if (!m->focussed_filler->flags & UI_FILLED_AREA_TYPE_BITMAP)
    {
        fixed_icontext_menuitem_transparent_filled_area = *(m->focussed_filler);
        m->focussed_filler = &fixed_icontext_menuitem_transparent_filled_area;
        fixed_icontext_menuitem_transparent_filled_area.flags |= UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
        fixed_icontext_menuitem_transparent_filled_area.c.alpha = HIGHLIGHTER_COLOR_OPACITY_VALUE;
    }
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
    m->focussed_text_color = current_fixed_icontext_menuitem_theme->focussed_text_color;
    m->normal_filler = current_fixed_icontext_menuitem_theme->normal_filler;
    m->normal_text_color = current_fixed_icontext_menuitem_theme->normal_text_color;
    m->selected_filler = current_fixed_icontext_menuitem_theme->selected_filler;
    m->selected_text_color = current_fixed_icontext_menuitem_theme->selected_text_color;
    m->text_font = current_fixed_icontext_menuitem_theme->text_font;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_icontext_menuitem_theme
 * DESCRIPTION
 *  Applies the given theme to a fixed icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed icontext menuitem object  (common data)
 *  t       [IN]        Is the theme
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_icontext_menuitem_theme(fixed_icontext_menuitem *m, UI_fixed_icontext_menuitem_theme *t)
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
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    /* This is to replace the UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR flag and alpha value from the themecomponent.h */
    if (!m->focussed_filler->flags & UI_FILLED_AREA_TYPE_BITMAP)
    {
        fixed_icontext_menuitem_transparent_filled_area = *(m->focussed_filler);
        m->focussed_filler = &fixed_icontext_menuitem_transparent_filled_area;
        fixed_icontext_menuitem_transparent_filled_area.flags |= UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
        fixed_icontext_menuitem_transparent_filled_area.c.alpha = HIGHLIGHTER_COLOR_OPACITY_VALUE;
    }
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
    m->focussed_text_color = t->focussed_text_color;
    m->normal_filler = t->normal_filler;
    m->normal_text_color = t->normal_text_color;
    m->selected_filler = t->selected_filler;
    m->selected_text_color = t->selected_text_color;
    m->text_font = t->text_font;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_fixed_icontext_menuitem
 * DESCRIPTION
 *  Creates a fixed icontext menuitem
 * PARAMETERS
 *  m           [IN]        Is the fixed icontext menuitem object (common data)
 *  width       [IN]        Width is the width of the fixed icontext menuitem
 *  height      [IN]        Height is the height of the fixed icontext menuitem
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_fixed_icontext_menuitem(fixed_icontext_menuitem *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    m->flags = 0;
    m->ext_flags = 0;
    m->icon_transparent_color = gui_transparent_color(0, 0, 0);
    gui_set_fixed_icontext_menuitem_current_theme(m);
    m->text_x = 0;
    m->text_y = 0;
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    m->icon_x = 2;  /* To display gap of icon from left side border */
#else 
    m->icon_x = 0;
#endif 
    m->icon_y = 0;
    m->scroll_x = 0;
    m->scroll_width = width;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_menuitem_set_text_position
 * DESCRIPTION
 *  Sets the text position within a fixed icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed icontext menuitem object  (common data)
 *  x       [IN]        Is the position of the text (offset from the left-top corner of the menuitem)
 *  y       [IN]        Is the position of the text (offset from the left-top corner of the menuitem)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_menuitem_set_text_position(fixed_icontext_menuitem *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->text_x = x;
    m->text_y = y;
    if (m->icon_x > m->text_x)
    {
        m->scroll_width = m->icon_x - m->text_x;
    }
    else
    {
        m->scroll_width = m->width - m->text_x;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_menuitem_set_icon_position
 * DESCRIPTION
 *  Sets the icon position within a fixed icontext menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed icontext menuitem object  (common data)
 *  x       [IN]        Is the position of the icon (offset from the left-top corner of the menuitem)
 *  y       [IN]        Is the position of the icon (offset from the left-top corner of the menuitem)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_menuitem_set_icon_position(fixed_icontext_menuitem *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->icon_x = x;
    m->icon_x = y;
    if (m->icon_x > m->text_x)
    {
        m->scroll_width = m->icon_x - m->text_x;
    }
    else
    {
        m->scroll_width = m->width - m->text_x;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_fixed_icontext_menuitem
 * DESCRIPTION
 *  Changes the size of a fixed icontext menuitem
 * PARAMETERS
 *  m           [IN]        Is the fixed icontext menuitem object  (common data)
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_fixed_icontext_menuitem(fixed_icontext_menuitem *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    if (m->icon_x > m->text_x)
    {
        m->scroll_width = m->icon_x - m->text_x;
    }
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    else
    {
        m->scroll_width = m->width - (m->text_x + TEXT_ICON_GAP);       /* As TEXT_ICON_GAP is add in text x to start show text */
    }
#else /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    else
    {
        m->scroll_width = m->width - m->text_x;
    }
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
     
}

/* Text scrolling related functions within the menu item */

fixed_icontext_menuitem *GUI_current_fixed_icontext_menuitem_common_data = NULL;
fixed_icontext_menuitem_type *GUI_current_fixed_icontext_menuitem_data = NULL;
U8 GUI_current_fixed_icontext_menuitem_scroll = 0;
U8 GUI_current_fixed_icontext_menuitem_scroll_direction;
S32 GUI_current_fixed_icontext_menuitem_x;
S32 GUI_current_fixed_icontext_menuitem_y;
S32 GUI_current_fixed_icontext_menuitem_scroll_counter = 0;
UI_string_type GUI_current_fixed_icontext_menuitem_scroll_text = NULL;
UI_string_type GUI_current_fixed_icontext_menuitem_scroll_text_end = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_menuitem_scroll_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_menuitem_scroll_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 flags = GUI_current_fixed_icontext_menuitem_common_data->flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_reset_current_list_effect();
#endif 

    if (!(flags & UI_MENUITEM_MARQUEE_SCROLL) && !(flags & UI_MENUITEM_TWO_DIRECTION_SCROLL))
    {
        return;
    }

    if (GUI_current_fixed_icontext_menuitem_scroll == 0)
    {
        return;
    }

    flags |= GUI_current_fixed_icontext_menuitem_data->flags;
    if (GUI_current_fixed_icontext_menuitem_scroll_counter)
    {
        GUI_current_fixed_icontext_menuitem_scroll_counter--;
    }
    else
    {
    #if(UI_USE_WORD_SCROLLING_TEXT)
        GUI_current_fixed_icontext_menuitem_scroll_text = GUI_current_fixed_icontext_menuitem_scroll_text_end;
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        if (flags & UI_MENUITEM_MARQUEE_SCROLL)
        {
            if (r2lMMIFlag)
            {
                GUI_current_fixed_icontext_menuitem_common_data->scroll_x += UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_icontext_menuitem_common_data->scroll_x >=
                    (GUI_current_fixed_icontext_menuitem_common_data->text_width + UI_text_menuitem_scroll_gap))
                {
                    GUI_current_fixed_icontext_menuitem_common_data->scroll_x = 0;
                }
            }
            else
            {
                GUI_current_fixed_icontext_menuitem_common_data->scroll_x -= UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_icontext_menuitem_common_data->scroll_x <=
                    (-(GUI_current_fixed_icontext_menuitem_common_data->text_width + UI_text_menuitem_scroll_gap)))
                {
                    GUI_current_fixed_icontext_menuitem_common_data->scroll_x = 0;
                }
            }
        }
        else
        {
            if (GUI_current_fixed_icontext_menuitem_scroll_direction == 0)
            {
                GUI_current_fixed_icontext_menuitem_common_data->scroll_x -= UI_text_menuitem_scroll_size;
                if ((GUI_current_fixed_icontext_menuitem_common_data->scroll_x +
                     GUI_current_fixed_icontext_menuitem_common_data->text_width) <=
                    (GUI_current_fixed_icontext_menuitem_common_data->scroll_width))
                {
                    GUI_current_fixed_icontext_menuitem_scroll_direction = 1;
                    GUI_current_fixed_icontext_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;
                }
            }
            else if (GUI_current_fixed_icontext_menuitem_scroll_direction == 1)
            {
                GUI_current_fixed_icontext_menuitem_common_data->scroll_x += UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_icontext_menuitem_common_data->scroll_x >= 0)
                {
                    GUI_current_fixed_icontext_menuitem_scroll_direction = 0;
                    GUI_current_fixed_icontext_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;
                }
            }
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        GUI_current_fixed_icontext_menuitem_common_data->ext_flags |= UI_MENUITEM_EXT_SHOW_IN_MARQUEE;

    #ifdef __MMI_UI_TRANSPARENT_EFFECT__
        if (!gui_is_current_transparency_with_multi_layer())
        {
            gui_fixed_icontext_disable_transparent_effect(GUI_current_fixed_icontext_menuitem_common_data);     /* To hide text below scrolling text */
        }
    #endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 

        gui_show_fixed_icontext_menuitem(
            (void*)GUI_current_fixed_icontext_menuitem_data,
            (void*)GUI_current_fixed_icontext_menuitem_common_data,
            GUI_current_fixed_icontext_menuitem_x,
            GUI_current_fixed_icontext_menuitem_y);

    #ifdef __MMI_UI_TRANSPARENT_EFFECT__
        gui_fixed_icontext_enable_transparent_effect(GUI_current_fixed_icontext_menuitem_common_data);  /* Enable after scrolling text */
    #endif 

        GUI_current_fixed_icontext_menuitem_common_data->ext_flags &= ~UI_MENUITEM_EXT_SHOW_IN_MARQUEE;

    #if defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) && defined(__MMI_UI_HINTS_IN_MENUITEM__)
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        if (GUI_current_fixed_icontext_menuitem_common_data->ext_flags & UI_MENUITEM_EXT_SHOW_TWO_LINE_SELECT)
        {
            gui_BLT_double_buffer(
                GUI_current_fixed_icontext_menuitem_x,
                GUI_current_fixed_icontext_menuitem_y,
                GUI_current_fixed_icontext_menuitem_x + GUI_current_fixed_icontext_menuitem_common_data->width - 1,
                GUI_current_fixed_icontext_menuitem_y + (GUI_current_fixed_icontext_menuitem_common_data->height << 1) - 1);
        }
        else if ((GUI_current_fixed_icontext_menuitem_common_data->ext_flags & UI_MENUITEM_SHOW_ALL_HINTS ||
                  ((GUI_current_fixed_icontext_menuitem_data->flags & UI_MENUITEM_STATE_FOCUSSED) &&
                   (GUI_current_fixed_icontext_menuitem_common_data->ext_flags & UI_MENUITEM_SHOW_HIGHLIGHTED_HINT))) &&
                 (gui_get_current_pop_up_description_string()))
        {
            gui_BLT_double_buffer(
                GUI_current_fixed_icontext_menuitem_x,
                GUI_current_fixed_icontext_menuitem_y,
                GUI_current_fixed_icontext_menuitem_x + GUI_current_fixed_icontext_menuitem_common_data->width - 1,
                GUI_current_fixed_icontext_menuitem_y + (GUI_current_fixed_icontext_menuitem_common_data->height << 1) - 1);
        }
        else
        {
            gui_BLT_double_buffer(
                GUI_current_fixed_icontext_menuitem_x,
                GUI_current_fixed_icontext_menuitem_y,
                GUI_current_fixed_icontext_menuitem_x + GUI_current_fixed_icontext_menuitem_common_data->width - 1,
                GUI_current_fixed_icontext_menuitem_y + GUI_current_fixed_icontext_menuitem_common_data->height - 1);
        }
    #endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
    #else /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) && defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
    #ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        if (GUI_current_fixed_icontext_menuitem_common_data->ext_flags & UI_MENUITEM_EXT_SHOW_TWO_LINE_SELECT)
        {
            gui_BLT_double_buffer(
                GUI_current_fixed_icontext_menuitem_x,
                GUI_current_fixed_icontext_menuitem_y,
                GUI_current_fixed_icontext_menuitem_x + GUI_current_fixed_icontext_menuitem_common_data->width - 1,
                GUI_current_fixed_icontext_menuitem_y + (GUI_current_fixed_icontext_menuitem_common_data->height << 1) - 1);
        }
        else
        {
            gui_BLT_double_buffer(
                GUI_current_fixed_icontext_menuitem_x,
                GUI_current_fixed_icontext_menuitem_y,
                GUI_current_fixed_icontext_menuitem_x + GUI_current_fixed_icontext_menuitem_common_data->width - 1,
                GUI_current_fixed_icontext_menuitem_y + GUI_current_fixed_icontext_menuitem_common_data->height - 1);
        }
    #endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
    #else /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 
         
         
    #ifdef __MMI_UI_HINTS_IN_MENUITEM__
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        if ((GUI_current_fixed_icontext_menuitem_common_data->ext_flags & UI_MENUITEM_SHOW_ALL_HINTS ||
             ((GUI_current_fixed_icontext_menuitem_data->flags & UI_MENUITEM_STATE_FOCUSSED) &&
              (GUI_current_fixed_icontext_menuitem_common_data->ext_flags & UI_MENUITEM_SHOW_HIGHLIGHTED_HINT))) &&
            (gui_get_current_pop_up_description_string()))
        {
            gui_BLT_double_buffer(
                GUI_current_fixed_icontext_menuitem_x,
                GUI_current_fixed_icontext_menuitem_y,
                GUI_current_fixed_icontext_menuitem_x + GUI_current_fixed_icontext_menuitem_common_data->width - 1,
                GUI_current_fixed_icontext_menuitem_y + (GUI_current_fixed_icontext_menuitem_common_data->height << 1) - 1);
        }
        else
        {
            gui_BLT_double_buffer(
                GUI_current_fixed_icontext_menuitem_x,
                GUI_current_fixed_icontext_menuitem_y,
                GUI_current_fixed_icontext_menuitem_x + GUI_current_fixed_icontext_menuitem_common_data->width - 1,
                GUI_current_fixed_icontext_menuitem_y + GUI_current_fixed_icontext_menuitem_common_data->height - 1);
        }
    #endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
    #else /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
         

    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gui_BLT_double_buffer(
            GUI_current_fixed_icontext_menuitem_x,
            GUI_current_fixed_icontext_menuitem_y,
            GUI_current_fixed_icontext_menuitem_x + GUI_current_fixed_icontext_menuitem_common_data->width - 1,
            GUI_current_fixed_icontext_menuitem_y + GUI_current_fixed_icontext_menuitem_common_data->height - 1);
    #endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
         
    #endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
         
         
    #endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 
    #endif /* defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) && defined(__MMI_UI_HINTS_IN_MENUITEM__) */ 
    }
    gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_icontext_menuitem_scroll_handler);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_menuitem_start_scroll
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_menuitem_start_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_icontext_menuitem_type *t = (fixed_icontext_menuitem_type*) GUI_current_fixed_icontext_menuitem_data;
    fixed_icontext_menuitem *m = (fixed_icontext_menuitem*) GUI_current_fixed_icontext_menuitem_common_data;
    S32 w;
    U32 flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = m->flags;
    flags |= t->flags;
    gui_set_font(m->text_font);
    w = gui_get_string_width(t->item_text);
    m->text_width = w;
    m->scroll_x = 0;

    if (w > (m->scroll_width))
    {
    #if(UI_USE_WORD_SCROLLING_TEXT)
        GUI_current_fixed_icontext_menuitem_scroll_text = t->item_text;
        GUI_current_fixed_icontext_menuitem_scroll = 1;
        GUI_current_fixed_icontext_menuitem_scroll_direction = 0;
        GUI_current_fixed_icontext_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        if (gui_get_current_list_effect() && gblock_list_effect != 1)
        {
            gui_register_callback_menuitem_scroll(gui_fixed_icontext_menuitem_scroll_handler);
        }
        else
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
        {
            gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_icontext_menuitem_scroll_handler);
        }
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        if ((flags & UI_MENUITEM_MARQUEE_SCROLL) || (flags & UI_MENUITEM_TWO_DIRECTION_SCROLL))
        {
            GUI_current_fixed_icontext_menuitem_scroll = 1;
            GUI_current_fixed_icontext_menuitem_scroll_direction = 0;
            GUI_current_fixed_icontext_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;

        #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
            if (gui_get_current_list_effect() && gblock_list_effect != 1)
            {
                gui_register_callback_menuitem_scroll(gui_fixed_icontext_menuitem_scroll_handler);
            }
            else
        #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
            {
                gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_icontext_menuitem_scroll_handler);
            }
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
    }
    else
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    {
        gui_register_callback_menuitem_scroll(gui_dummy_scroll_handler_for_list_effects);
        GUI_current_fixed_icontext_menuitem_scroll = 0;
    }
#else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    {
        GUI_current_fixed_icontext_menuitem_scroll = 0;
    }
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_menuitem_stop_scroll
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_menuitem_stop_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GUI_current_fixed_icontext_menuitem_scroll)
    {
        gui_cancel_timer(gui_fixed_icontext_menuitem_scroll_handler);
        GUI_current_fixed_icontext_menuitem_scroll = 0;
    }
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_register_callback_menuitem_scroll(gui_dummy_scroll_handler_for_list_effects);
    gui_restore_current_list_effect();
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
}

/* External used to display inline edit menu items */
void wgui_show_inline_edit_menuitem(S32 x1, S32 y1, S32 width, S32 height);


/*****************************************************************************
 * FUNCTION
 *  print_wrap_string
 * DESCRIPTION
 *  Displays the fixed icontext menuitem
 * PARAMETERS
 *  s                       [IN]        
 *  x                       [IN]        Is the position at which the menuitem is displayed
 *  y                       [IN]        Is the position at which the menuitem is displayed
 *  number_of_rows          [IN]        
 *  line_width              [IN]        
 *  item(?)                 [IN]        Is a specific fixed icontext menuitem object
 *  common_item_data(?)     [IN]        Is the fixed icontext menuitem object (common_data)
 * RETURNS
 *  void
 *****************************************************************************/
extern S32 MMI_current_menu_type;

extern fixed_matrix_menu MMI_fixed_matrix_menu;

extern gdi_handle animation_handle;

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
void print_wrap_string(UI_string_type s, S32 x, S32 y, S32 number_of_rows, S32 line_width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 line_height = 0, current_line_width = 0, char_width = 0, char_height = 0, cursor_pos_x = x, cursor_pos_y = y;
    U16 char_index = 0;
    S32 total_rows = 1;
    S32 es_width = 0, es_height = 0;
    UI_character_type es[] = { '.', '\0' };
    UI_character_type terminal[] = { '\0' };

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_measure_string(es, &es_width, &es_height);

    if (!s)
    {
        return;
    }

    gui_measure_string(s, (S32*) & char_width, (S32*) & char_height);
    if (char_width <= line_width)
    {
        if (!r2lMMIFlag)
        {
            cursor_pos_x = x + ((line_width - char_width) >> 1);
        }
        else
        {
            cursor_pos_x = x - ((line_width - char_width) >> 1);
        }
        gui_move_text_cursor(cursor_pos_x, cursor_pos_y);
        gui_print_text(s);
        return;
    }
    gui_move_text_cursor(x, y);
    gui_push_clip();
    gui_set_clip(x, y, x + line_width, y + char_height);
    while (s[char_index] != terminal[0] && total_rows <= number_of_rows)
    {
        gui_measure_character(s[char_index], (S32*) & char_width, (S32*) & char_height);
        if (current_line_width + char_width < line_width - es_width)
        {
            gui_print_character(s[char_index++]);
            if (!r2lMMIFlag)
            {
                cursor_pos_x += char_width;
            }
            else
            {
                cursor_pos_x -= char_width;
            }
            current_line_width += char_width;
            gui_move_text_cursor(cursor_pos_x, cursor_pos_y);
            if (line_height < char_height)
            {
                line_height = char_height;
            }
        }
        else
        {
            if (total_rows < number_of_rows)    /* still can add new lines */
            {
                if (line_height < char_height)
                {
                    line_height = char_height;
                }
                total_rows++;
                cursor_pos_x = x;
                cursor_pos_y += line_height + 1;
                current_line_width = 0;
                gui_move_text_cursor(cursor_pos_x, cursor_pos_y);
                gui_print_character(s[char_index++]);
                if (!r2lMMIFlag)
                {
                    cursor_pos_x += char_width;
                }
                else
                {
                    cursor_pos_x -= char_width;
                }
                current_line_width += char_width;
                gui_move_text_cursor(cursor_pos_x, cursor_pos_y);
            }
            else
            {
                gui_print_text((UI_string_type) es);
                break;
            }
        }
    }
    gui_pop_clip();
}
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
#if defined(__MMI_MAINLCD_128X128__)
#include "..\..\mainmenu\include\mainmenudef.h"
extern GDI_HANDLE fixed_list_menu_icon_layer_handle;
#endif

#if defined(__MMI_MAINLCD_160X128__)
typedef struct 
{
	S32 x;
	S32 y;
}img_coordinate;
/***********************************************************************************
FUNCTION DESCRIPTION:
	for the img size is 40X36(36*3>MMI_content_height),MMI_fixed_matrix_menu caculate configuration 
	will be displayed abnormal.we have to rectify hilight image coordinate through this function.
************************************************************************************/
void mmi_rectify_mainmenu_highlight_img_coordinate(S32 highlight_item,S32 *x,S32 *y)
{
	img_coordinate rectify_img_coordinate[12] = 
	{
		{0,15},
		{40,15},
		{80,15},
		{120,15},
		{0,45},
		{40,45},
		{80,45},
		{120,45},
		{0,75},
		{40,75},
		{80,75},
		{120,75}
	};
	
	*x = rectify_img_coordinate[highlight_item].x;
	*y = rectify_img_coordinate[highlight_item].y;

	return;
}

#endif
/*****************************************************************************
 * FUNCTION
 *  gui_show_fixed_icontext_menuitem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [?]         
 *  x                       [IN]        
 *  y                       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef __MMI_USE_DRAW_FOCUS_IN_MAINMENU__
 #include"mainmenudef.h"
  extern void gdi_draw_round_rect_with_transparent(	S32 x1, S32 y1, S32 x2, S32 y2, color_t	transparent_color,  gdi_color	frame_color, S32			frame_width);
#endif
#if defined(__PROJECT_GALLITE_C01__)
extern BOOL IsInDialerSrc(void);
extern BOOL IsFocusOnInputBox(void);
#endif
void gui_show_fixed_icontext_menuitem(void *item, void *common_item_data, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;

    /* relative x offset of text inside menu item. 
       (which might be different from m->text_x in Arabic or scrolling) */
    S32 text_x = 0;

    /* relative y offset of text inside menu item. */
    S32 text_y = 0;

#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    S32 hint_height = 0;
#endif 

#if (defined __MMI_UI_HINTS_IN_MENUITEM__ || defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__)
    MMI_BOOL show_hint_in_menuitem = MMI_FALSE;
#endif 

    S32 ani_w, ani_h;
    color_t text_color;
    UI_filled_area *f = NULL;
    fixed_icontext_menuitem *m = (fixed_icontext_menuitem*) common_item_data;
    fixed_icontext_menuitem_type *mi = (fixed_icontext_menuitem_type*) item;
    U32 flags;
    UI_string_type _text;
    PU8 _icon;
    U8 animation_flag = 0;
    S32 sw = 0, sh = 0, max_text_width = 0;
    S32 is_text_full_width = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = mi->flags;
    flags |= m->flags;
    _text = mi->item_text;
    _icon = mi->item_icon;
    x1 = x;
    y1 = y;
#if defined(__PROJECT_GALLITE_C01__)
    if(IsInDialerSrc())
        x2 = x1 + m->width - 1 - 5;
    else
#endif
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    if ((m->ext_flags & UI_MENUITEM_SHOW_ALL_HINTS) ||
        ((flags & UI_MENUITEM_STATE_FOCUSSED) && (m->ext_flags & UI_MENUITEM_SHOW_HIGHLIGHTED_HINT)))
    {
        if (gui_get_current_pop_up_description_string())
        {
            show_hint_in_menuitem = MMI_TRUE;
        }
    }
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

#if (defined __MMI_UI_HINTS_IN_MENUITEM__ || defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__)
    if (show_hint_in_menuitem
#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
        || (m->ext_flags & UI_MENUITEM_EXT_SHOW_TWO_LINE_SELECT)
#endif 
        )
    {
        gui_show_two_line_icontext_menuitem(item, common_item_data, x, y);
        return;
    }
#endif /* (defined __MMI_UI_HINTS_IN_MENUITEM__ || defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__) */ 

    if (flags & UI_MENUITEM_INLINE_EDIT_OBJECT)
    {
        if (flags & UI_MENUITEM_STATE_FOCUSSED)
        {
            /* Remove the display of background filler when the default text effect is enabled */
        #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
            if (!(mi->ext_flags & UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT))
        #endif 
            {
                if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS))
                {
                    gui_draw_filled_area(x1 + 2, y1, x2, y2, m->focussed_filler);
                }
            }
            /* display the focussed inline item with different coordinates when the default
               text effect is enabled. */
        #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
            if ((mi->ext_flags & UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT))
            {
                if (r2lMMIFlag)
                {
                    gui_display_pop_up_description(x + 2, y + 1, m->width - m->text_x, ((m->height - 1) - 1));
                }
                else
                {
                    gui_display_pop_up_description(x + m->text_x, y + 1, m->width - m->text_x, ((m->height - 1) - 1));
                }
            }
            else
        #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
            {
                gui_display_pop_up_description(x + 2, y, m->width - 2, m->height);
            }
        }
        else
        {
             
            gui_set_font(m->text_font);
             

            /* display the unfocussed inline item with different coordinates when the default
               text effect is enabled. */
        #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
            if ((mi->ext_flags & UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT))
            {
                if (r2lMMIFlag)
                {
                    wgui_show_inline_edit_menuitem(x + 2, y + 1, m->width - m->text_x, ((m->height - 1) - 1));
                }
                else
                {
                    wgui_show_inline_edit_menuitem(x + m->text_x, y + 1, m->width - m->text_x, ((m->height - 1) - 1));
                }
            }
            else
        #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
            {
                wgui_show_inline_edit_menuitem(x + 2, y, m->width - 2, m->height);
            }
        }

        return;
    }

    if (flags & UI_MENUITEM_STATE_FOCUSSED)
    {
    #ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
        g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_FOCUSED_ICONTEXT_ITEM;
    #endif	
        GUI_current_fixed_icontext_menuitem_x = x;
        GUI_current_fixed_icontext_menuitem_y = y;
    }

    if (!(flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY))
    {
        /* Autocalculate the text position based on font   */
        gui_set_font(m->text_font);
        gui_measure_string(_text, &sw, &sh);
        if (flags & UI_MENUITEM_CENTER_TEXT_X)
        {
            m->text_x = (m->width >> 1) - (sw >> 1);
        }
        if (flags & UI_MENUITEM_CENTER_TEXT_Y)
        {
            m->text_y = (m->height >> 1) - (sh >> 1);
        }
#ifdef __MMI_MAINLCD_220X176__		
        /*Show the text of item in the bottom. */
        if (m->ext_flags & UI_MENUITEM_EXT_BOTTOM_TEXT)
        {
            m->text_x = (m->width >> 1) - (sw >> 1);
            m->text_y = m->height  - sh-1;
        }		
#endif
    }

    /* Autocalculate the icon position                 */
    if (_icon != UI_NULL_IMAGE)
    {
        S32 icon_width, icon_height;

        gui_measure_image(_icon, &icon_width, &icon_height);

        if (flags & UI_MENUITEM_CENTER_ICON_X)
        {
            m->icon_x = (m->width >> 1) - (icon_width >> 1);
        }
        if (flags & UI_MENUITEM_CENTER_ICON_Y)
        {
            m->icon_y = (m->height >> 1) - (icon_height >> 1);
        }
    }

    /* Compute text_x and text_y */
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    if (MMI_current_menu_type == LIST_MENU)
    {
        text_x = m->text_x + TEXT_ICON_GAP; /* To show gap from grid */
    }
    else
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    {
        text_x = m->text_x;
    }
    text_y = m->text_y;

    /* For inline edit captions to appear from left when there is no icon   */
    if ((flags & UI_MENUITEM_DISABLE_ICON) && (_icon == UI_NULL_IMAGE))
    {
        is_text_full_width = 1;
        text_x = 2;
    }

    if (r2lMMIFlag)
    {
        text_x = m->width - 1 - text_x;
    }

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    if (MMI_current_menu_type == LIST_MATRIX_MENU)
    {
        is_text_full_width = 1;
    }
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    /* To hide the text and icon of the menuitem up to the last frame for flash feel effect */
    if (gblock_list_effect == 0)
    {
        if (gui_get_current_list_effect() == MMI_UI_LIST_HIGHLIGHT_EFFECT_FLASH_FEELING &&
            MMI_current_menu_type == LIST_MENU)
        {
            if (flash_feel_last_frame == 0)
            {
                if (flags & UI_MENUITEM_STATE_FOCUSSED)
                {
                    _icon = UI_NULL_IMAGE;
                    flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY;
                }
            }
        }
    }
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 

    /* Setup text scroll and background filler */
    if (flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        text_x += m->scroll_x;
    }
    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && (flags & UI_MENUITEM_FOCUSSED_ANIMATE))
    {
        animation_flag = 1;
    }
    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && !(flags & UI_MENUITEM_DISABLE_HIGHLIGHT))
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS))
        {
            f = m->focussed_filler;
        }
        text_color = m->focussed_text_color;
    }
    else if (flags & UI_MENUITEM_STATE_DISABLED)
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND) && !(flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS))
        {
            f = m->disabled_filler;
        }
        text_color = m->focussed_text_color;
    }
    else if (flags & UI_MENUITEM_STATE_SELECTED)
    {
        f = m->selected_filler;
        text_color = m->selected_text_color;
        if (flags & UI_MENUITEM_SELECTED_ANIMATE)
        {
            animation_flag = 1;
        }
    }
    else
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND) && !(flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS))
        {
            f = m->normal_filler;
        }
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        // if condition is not correct. It fails when any other flag is ON 
        // along with UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT.
        if (mi->ext_flags & UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT)
        {
            f = m->selected_filler;
        }
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
        text_color = m->normal_text_color;
    }
    gui_push_clip();

    if (MMI_current_menu_type == MATRIX_MENU)
    {
        if (animation_flag)
        {
            gui_measure_image(MMI_mm_animation, &ani_w, &ani_h);
            if (ani_w > m->width)
            {
                m->icon_x -= ((ani_w - m->width) >> 1);  
                //if ( ( x1+m->icon_x - ((ani_w-m->width)>>1) ) > MMI_fixed_matrix_menu.x )
                //  m->icon_x -= ((ani_w-m->width)>>1);
                //if ( x1+m->icon_x+ani_w > MMI_fixed_matrix_menu.x+MMI_fixed_matrix_menu.width )
                //  m->icon_x -= ((ani_w-m->width)>>1);
                if (x1 + m->icon_x < MMI_fixed_matrix_menu.x)
                {
                    m->icon_x = MMI_fixed_matrix_menu.x - x1;
                }
                if (x1 + m->icon_x + ani_w > MMI_fixed_matrix_menu.x + MMI_fixed_matrix_menu.width)
                {
                    m->icon_x = MMI_fixed_matrix_menu.x + MMI_fixed_matrix_menu.width - x1 - ani_w;
                }
            }
            if (ani_h > m->height)
            {
                m->icon_y -= ((ani_h - m->height) >> 1);     
                //if ( ( y1+m->icon_y - ((ani_h-m->height)>>1) ) > MMI_fixed_matrix_menu.y )
                //  m->icon_y -= ((ani_h-m->height)>>1);
                //if ( y1+m->icon_y+ani_h > MMI_fixed_matrix_menu.y+MMI_fixed_matrix_menu.height )
                //  m->icon_y -= ((ani_h-m->height)>>1);
                if (y1 + m->icon_y < MMI_fixed_matrix_menu.y)
                {
                    m->icon_y = MMI_fixed_matrix_menu.y - y1;
                }
                if (y1 + m->icon_y + ani_h > MMI_fixed_matrix_menu.y + MMI_fixed_matrix_menu.height)
                {
                    m->icon_y = MMI_fixed_matrix_menu.y + MMI_fixed_matrix_menu.height - y1 - ani_h;
                }
            }
            gui_set_clip(x1 + m->icon_x, y1 + m->icon_y, x1 + m->icon_x + ani_w - 1, y1 + m->icon_y + ani_h - 1);
        }
        else
        {
            gui_measure_image(_icon, &ani_w, &ani_h);
            gui_set_clip(x1 + m->icon_x, y1 + m->icon_y, x1 + m->icon_x + ani_w - 1, y1 + m->icon_y + ani_h - 1);
        }
    }
    else
    {
        if (MMI_current_menu_type == MATRIX_MENU_EMS)
        {
            gui_set_clip(x1 - 1, y1 - 1, x2 + 1, y2 + 1);
        }
        else
        {
            gui_set_clip(x1, y1, x2, y2);
        #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
            hint_height = 0;

            if (_icon != UI_NULL_IMAGE && line_draw && MMI_current_menu_type == LIST_MENU)
            {
                if (r2lMMIFlag)
                {
                    gui_draw_vertical_line(y1, y2 + hint_height, (x2 - (m->text_x + TEXT_ICON_GAP) + ((TEXT_ICON_GAP) >> 1)), current_MMI_theme->list_background_filler->border_color); /* Assume icon is square in shape add 2 for inline edit items having gap from grid */
                }
                else
                {
                    gui_draw_vertical_line(y1, y2 + hint_height, m->text_x - ((TEXT_ICON_GAP) >> 1) + 2, current_MMI_theme->list_background_filler->border_color);      /* Assume icon is square in shape add 2 for inline edit items having gap from grid */
                }
            }
        #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
        }
    }

    if (m->flags & UI_MENUITEM_DISABLE_HIGHLIGHT_DISPLAY)
    {
        f = NULL;
    }

    if (MMI_current_menu_type != MATRIX_MENU && f != NULL && MMI_current_menu_type != MATRIX_MENU_EMS)
    {
    #ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
        if (MMI_current_menu_type == LIST_MATRIX_MENU)
        {
        #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
            set_start_position_and_item_parameters_for_list_highlighter_effect(item, common_item_data, x1, y1);
        #endif 
            if (r2lMMIFlag)
            {
                gui_draw_filled_area(x1, y1 + text_y, x1 + text_x, y2, f);
            }
            else
            {
                gui_draw_filled_area(x1 + text_x, y1 + text_y, x2, y2, f);
            }
        }
        else
    #endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
        {
        #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
            // if condition is not correct. It fails when any other flag is ON 
            // along with UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT.
            if (mi->ext_flags & UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT)
            {
                /* gui_draw_filled_area(x1 + m->text_x - 1, y1 + 2, x2, y2-1, f); */
                if (r2lMMIFlag)
                {
                    gui_draw_filled_area(x1 + 1, y1 + 1, x2 - m->text_x + 1, y2 - 1, f);
                }
                else
                {
                    gui_draw_filled_area(x1 + m->text_x, y1 + 1, x2, y2 - 1, f);
                }
            }
            else
        #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
            {
           	#ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
           	#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
                if (is_draw_next_frame_list_highlight_effect_value())
            #endif
                {
                    g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_HIGHLIGHT_ICONTEXT_ITEM;
                }
            #endif	

            #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
                set_start_position_and_item_parameters_for_list_highlighter_effect(item, common_item_data, x1, y1);
            #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
                if (line_draw)
                {
                    gui_draw_list_filled_area(x1 + 1, y1 + 1, x2, y2, f);
                }
                else
            #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
                    gui_draw_list_filled_area(x1 + 1, y1, x2, y2, f);
            #else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
            #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
                if (line_draw && GUI_current_fixed_icontext_menuitem_scroll)
                {
                    gui_draw_filled_area(x1 + 1, y1 + 1, x2, y2, f);
                }
                else
                {
                    gui_draw_filled_area(x1 + 1, y1, x2, y2, f);
                }
            #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
#if defined(__PROJECT_GALLITE_C01__)
            if(!(IsInDialerSrc() && IsFocusOnInputBox()))
#endif
                gui_draw_filled_area(x1 + 1, y1, x2, y2, f);
            #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
            }
        }

    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw && MMI_current_menu_type == LIST_MENU)
        {
            if (!r2lMMIFlag)
            {
                gui_draw_vertical_line(MMI_fixed_list_menu.y + 1, MMI_fixed_list_menu.y + MMI_fixed_list_menu.height - 2, MMI_fixed_list_menu.x, current_MMI_theme->list_background_filler->border_color);      /* line from top to end at left end */
                gui_draw_vertical_line(MMI_fixed_list_menu.y + 1, MMI_fixed_list_menu.y + MMI_fixed_list_menu.height - 2, MMI_fixed_list_menu.vbar.x - 2, current_MMI_theme->list_background_filler->border_color);     /* line from top to end at right end */
            }
        }
        if (_icon != UI_NULL_IMAGE && line_draw && MMI_current_menu_type == LIST_MENU)
        {
            if (r2lMMIFlag)
            {
                gui_draw_vertical_line(y1, y2 + hint_height, (x2 - (m->text_x + TEXT_ICON_GAP) + ((TEXT_ICON_GAP) >> 1)) /*-(((TEXT_ICON_GAP )>>1)) */ , current_MMI_theme->list_background_filler->border_color);      /* Assume icon is square in shape add 2 for inline edit items having gap from grid */
            }
            else
            {
                gui_draw_vertical_line(y1, y2 + hint_height, m->text_x - ((TEXT_ICON_GAP) >> 1) + 2, current_MMI_theme->list_background_filler->border_color);  /* vertical partion between icon and text add 2 for inline edit items having gap from grid */
            }
        }
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    }

    if (_icon != UI_NULL_IMAGE && !((m->ext_flags & UI_MENUITEM_EXT_SHOW_IN_MARQUEE) && animation_flag))
    {
        if (mi->item_icon_handle != GDI_ERROR_HANDLE)
        {
            gdi_image_stop_animation(mi->item_icon_handle);
            mi->item_icon_handle = GDI_ERROR_HANDLE;
        }
        if (animation_flag)
        {
        #if(UI_MAIN_MENU_ANIMATION_TYPE == UI_MAIN_MENU_ANIMATION_TYPE1)
            /* to store animation handle. */
            gdi_image_draw_animation(x1 + m->icon_x, y1 + m->icon_y, _icon, &mi->item_icon_handle);
            /* mi->item_icon_handle=gui_show_transparent_animated_image(x1+m->icon_x,y1+m->icon_y,_icon,m->icon_transparent_color); */
        #elif(UI_MAIN_MENU_ANIMATION_TYPE==UI_MAIN_MENU_ANIMATION_TYPE2)
            //                      mi->item_icon_handle=gui_show_transparent_animated_image_frames(x1+m->icon_x,y1+m->icon_y,_icon,m->icon_transparent_color,1,0);
            //                      gdi_image_draw_animation_frames(x1+m->icon_x,y1+m->icon_y,mi->item_icon,(gdi_handle *)&mi->item_icon_handle,1);
            if (MMI_current_menu_type == MATRIX_MENU)
            {
#if defined(__MMI_MAINLCD_160X128__)       /*for the img size is 40X36(36*3>MMI_content_height),MMI_fixed_matrix_menu caculate configuration will be displayed abnormal.*/
		S32 img_x,img_y;
		mmi_rectify_mainmenu_highlight_img_coordinate(MMI_fixed_matrix_menu.highlighted_item,&img_x,&img_y);
		gdi_image_draw_animation(
                    img_x,
                    img_y,
                    MMI_mm_animation,
                    (gdi_handle*) & mi->item_icon_handle);
#else
                gdi_image_draw_animation(
                    x1 + m->icon_x,
                    y1 + m->icon_y,
                    MMI_mm_animation,
                    (gdi_handle*) & mi->item_icon_handle);
#endif
            }
            else if (MMI_current_menu_type == MATRIX_MENU_EMS)
            {
                /* extern gdi_color GDI_COLOR_BLACK; */
                gdi_image_draw_animation(
                    x1 + m->icon_x,
                    y1 + m->icon_y,
                    mi->item_icon,
                    (gdi_handle*) & mi->item_icon_handle);
                gdi_draw_rect(x1 - 1, y1 - 1, x2 + 1, y2 + 1, GDI_COLOR_BLACK);
            }
            else
            {
#if defined(__MMI_MAINLCD_128X128__)
                if (MAIN_MENU_SCREENID == GetActiveScreenId())
                {
                    gdi_layer_push_and_set_active(fixed_list_menu_icon_layer_handle);
                }
                gdi_image_draw_animation_once(
                                    x1 + m->icon_x,
                                    y1 + m->icon_y,
                                    mi->item_icon,
                                    (gdi_handle*) & mi->item_icon_handle);
                if (MAIN_MENU_SCREENID == GetActiveScreenId())
                {
                    gdi_layer_pop_and_restore_active();
                }                
#else
                gdi_image_draw_animation(
                    x1 + m->icon_x,
                    y1 + m->icon_y,
                    mi->item_icon,
                    (gdi_handle*) & mi->item_icon_handle);
#endif                
            }
        #endif /* UI_MAIN_MENU_ANIMATION_TYPE */
        }
        else if (m->flags & UI_MENUITEM_STATE_ANIMATE)
        {
            gui_show_transparent_animated_image_frame(
                x1 + m->icon_x,
                y1 + m->icon_y,
                _icon,
                m->icon_transparent_color,
                1);
            m->flags &= ~UI_MENUITEM_STATE_ANIMATE;
        }
        else if (MMI_current_menu_type == LIST_MENU)
        {
            S32 x_clip = 0, y_clip = 0;

            gui_push_clip();
            if (m->icon_x > 0)
            {
                x_clip = m->icon_x;
            }
            if (m->icon_y > 0)
            {
                y_clip = m->icon_y;
            }

            if (!r2lMMIFlag)
            {
                gui_set_clip(x1 + x_clip, y1 + y_clip, x1 + m->text_x - 2, y1 + m->height - 2);

                gdi_image_draw_animation_single_frame(x1 + m->icon_x, y1 + m->icon_y, (U8*) _icon, 0);
            }
            else
            {
                gui_set_clip(x2 - m->text_x + 2, y1 + y_clip, x2 - x_clip, y1 + m->height - 2);

                gdi_image_draw_animation_single_frame(x2 - m->text_x + 2, y1 + m->icon_y, (U8*) _icon, 0);
            }

            gui_pop_clip();
        }
        else
        {
         #ifdef __MMI_USE_DRAW_FOCUS_IN_MAINMENU__
         	{
		 if ( flags & UI_MENUITEM_STATE_FOCUSSED ) 
			{
			       S32 image_width = 0;
				S32 image_height = 0;
				color_t transparent_color  = {253, 121, 227, 35};
				gdi_handle 	activelayer;
				gdi_layer_get_active(&activelayer);  
				gui_measure_image(_icon,&image_width,&image_height);
			#ifdef __MMI_WALLPAPER_ON_BOTTOM__
			    //   gui_set_clip(x1 + m->icon_x, y1 + m->icon_y+1,x1 + m->icon_x+image_width-1,y1 + m->icon_y+image_height-1);
			   //    gdi_image_draw_id(0, 0, IMG_MAIN_MENU_BG2);
				gdi_image_layer_copy(x1 + m->icon_x, y1 + m->icon_y+1,x1 + m->icon_x+image_width-1,y1 + m->icon_y+image_height-1,activelayer,dm_get_scr_bg_layer());
			#endif
				gdi_draw_round_rect_with_transparent(x1 + m->icon_x, y1 + m->icon_y+1,x1 + m->icon_x+image_width-1,y1 + m->icon_y+image_height-1,transparent_color,gdi_act_color_from_rgb(255, 255, 125,255),3);
			}
         	}
	  #else
            /* gui_show_transparent_animated_image_frame(x1+m->icon_x,y1+m->icon_y,_icon,m->icon_transparent_color,0); */
            gdi_image_draw_animation_single_frame(x1 + m->icon_x, y1 + m->icon_y, (U8*) _icon, 0);
	  #endif
        }
    }

    if (!(flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY))
    {
        gui_push_text_clip();
        if (!(flags & UI_MENUITEM_DISABLE_TEXT_CLIP))
        {
            if (is_text_full_width)
            {
                /* FIXME. text_x might not be 0 */
                gui_set_text_clip(x1, y1, x2, y2);
                max_text_width = m->width;
            }
            else
            {
                if (m->icon_x > m->text_x)
                {
					#if !defined(__MMI_MAINLCD_220X176__) //FIXME: mainmenu string will not display
                    gui_set_text_clip(x1, y1, x1 + m->icon_x - 1, y2);
                    max_text_width = m->icon_x - m->text_x;
                    #endif
                }
                else
                {
                    if (!r2lMMIFlag)
                    {
                    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
                        gui_set_text_clip(x1 + m->text_x + TEXT_ICON_GAP, y1, x2 - 2, y2);
                    #else /* __MMI_UI_DALMATIAN_FIXEDLIST__ */
                        if (f && (f->flags & UI_FILLED_AREA_BORDER))
                        {
                            /* Leave space for border of background filler */
                            gui_set_text_clip(x1 + m->text_x, y1, x2 - 2, y2);
                        }
                        else
                        {
                            gui_set_text_clip(x1 + m->text_x, y1, x2, y2);
                        }
                    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */
                    }
                    else
                    {
                    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
                        gui_set_text_clip(x2 - m->width + 1 + TEXT_ICON_GAP, y1, x2 - m->text_x - TEXT_ICON_GAP, y2);
                    #else /* __MMI_UI_DALMATIAN_FIXEDLIST__ */
                        if (f && (f->flags & UI_FILLED_AREA_BORDER))
                        {
                            /* Leave space for border of background filler */
                            gui_set_text_clip(x1 + 2, y1, x2 - m->text_x, y2);
                        }
                        else
                        {
                            gui_set_text_clip(x1, y1, x2 - m->text_x, y2);
                        }
                    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */
                    }
                    max_text_width = x2 - (x1 + m->text_x) + 1;
                }
            }
        }
        else
        {
            max_text_width = m->width - m->text_x;
        }

        /* move the cursor by 3 pixels when the default text effect is enabled and the
           menu item is to be shown inside rectangle. */
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        if (mi->ext_flags & UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT)
        {
            if (!r2lMMIFlag)
            {
                gui_move_text_cursor(x1 + text_x + 3, y1 + text_y);
            }
            else
            {
                gui_move_text_cursor(x1 + text_x - 3, y1 + text_y);
            }
        }
        else
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
        {
            gui_move_text_cursor(x1 + text_x, y1 + text_y);
        }

        gui_set_line_height(sh);
        gui_set_text_color(text_color);
    #if(UI_USE_WORD_SCROLLING_TEXT)
        if ((flags & UI_MENUITEM_STATE_FOCUSSED) && GUI_current_fixed_icontext_menuitem_scroll)
        {
            GUI_current_fixed_icontext_menuitem_scroll_text_end = GUI_current_fixed_icontext_menuitem_scroll_text;
            if (gui_print_truncated_text2
                (x1 + text_x, y1 + text_y, max_text_width, &GUI_current_fixed_icontext_menuitem_scroll_text_end))
            {
                GUI_current_fixed_icontext_menuitem_scroll_text_end = _text;
            }
        }
        else
        {
            gui_print_truncated_text2(x1 + text_x, y1 + text_y, max_text_width, &_text);
        }
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 

        if ((flags & UI_MENUITEM_STATE_FOCUSSED) && (flags & UI_MENUITEM_MARQUEE_SCROLL) &&
            GUI_current_fixed_icontext_menuitem_scroll)
        {
        #ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
            g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_HIGHLIGHT_ICONTEXT_MARQUEE_SCROLL;
        #endif	
            /* fix long text in inline-select display overlap issue */
            if (!(mi->ext_flags & UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY))
            {
#if defined(__MMI_MAINLCD_160X128__)
		gui_set_font(&MMI_large_font);
#endif
                gui_print_text(_text);
            }
            if (!r2lMMIFlag)
            {
                gui_move_text_cursor(x1 + text_x + m->text_width + UI_text_menuitem_scroll_gap, y1 + text_y);
            }
            else
            {
                gui_move_text_cursor(x1 + text_x - (m->text_width + UI_text_menuitem_scroll_gap), y1 + text_y);
            }

            gui_set_line_height(sh);
            gui_print_text(_text);
        }
    #ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
        else if (MMI_current_menu_type == LIST_MATRIX_MENU)
        {
            print_wrap_string(_text, x1 + text_x, y1 + text_y, 1, max_text_width);
        }
    #endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
        else
        {
        #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
            /* 
             * Text is getting displayed behind the left button in case of Inline Selection
             * when the menu item is focussed. So remove the display of text when the item
             * is focussed and UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY flag is ON
             * for that item. 
             */
            if (!
                ((flags & UI_MENUITEM_STATE_FOCUSSED) &&
                 (mi->ext_flags & UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY)))
        #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
            {
#if defined(__MMI_MAINLCD_160X128__)
	if (!(m->ext_flags & UI_MENUITEM_SHOW_ICON_ONLY_ON_HIGHLIGHT))
#endif
#if defined(__MMI_MAINLCD_160X128__)
	{
		gui_set_font(&MMI_large_font);
#endif
		gui_print_text(_text);
#if defined(__MMI_MAINLCD_160X128__)
	}
#endif
            }
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        gui_pop_text_clip();
    }

    if (flags & UI_MENUITEM_DOTTED_UNDERLINE)
    {
        static const U8 dotted_line_bitvalues[] = {1, 1, 1, 1, 0, 0, 0};
        S32 r = max_text_width % 7; /* pattern width */

        if (r == 0)
        {
            r = 7;
        }
        gdi_draw_line_style(
            m->text_x,
            y2,
            m->text_x + max_text_width - 1 - r,
            y2,
            gdi_act_color_from_rgb(255, 112, 125, 240),
            sizeof(dotted_line_bitvalues),
            dotted_line_bitvalues);
    }

    gui_pop_clip();

#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && !(m->ext_flags & UI_MENUITEM_EXT_SHOW_IN_MARQUEE))
    {
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        /* pass the modified parameters in the function so that there is proper gapping
           between the menu items. */
        if (mi->ext_flags & UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT)
        {
            gui_display_pop_up_description(x, y + 1, m->width, ((m->height - 1) - 1));
        }
        else
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
        {
            gui_display_pop_up_description(x, y, m->width, m->height);
        }
    }
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 

#ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
    g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_HIGHLIGHT_NONE;
#endif	

}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_menuitem_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [?]         
 *  item_x                  [IN]        
 *  item_y                  [IN]        
 *  pen_event               [IN]        
 *  x                       [IN]        
 *  y                       [IN]        
 *  menuitem_event          [?]         
 *  menuitem_param          [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gui_fixed_icontext_menuitem_translate_pen_event(
        void *item,
        void *common_item_data,
        S32 item_x,
        S32 item_y,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_list_pen_enum *menuitem_event,
        gui_pen_event_param_struct *menuitem_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_wait_longpress_enum w;
    fixed_icontext_menuitem *m = (fixed_icontext_menuitem*) common_item_data;
    fixed_icontext_menuitem_type *mi = (fixed_icontext_menuitem_type*) item;
    U32 flags = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mi)
    {
        flags |= mi->flags;
    }
    if (m)
    {
        flags |= m->flags;
    }

    *menuitem_event = GUI_LIST_PEN_NONE;
    GUI_PEN_EVENT_PARAM_SET_VOID(menuitem_param);

    if (flags & UI_MENUITEM_PEN_USE_LONGPRESS)
    {
        w = gui_pen_wait_longpress(pen_event, x, y);

        if (w == GUI_WAIT_LONGPRESS_READY)
        {
            *menuitem_event = GUI_LIST_PEN_ITEM_SELECTED;
        }
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_hide_fixed_icontext_menuitem
 * DESCRIPTION
 *  Hides a fixed icontext menuitem
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed icontext menuitem object
 *  common_item_data        [IN]        Is the fixed icontext menuitem object (common_data)
 *  x                       [IN]        Is the position at which the menuitem was displayed
 *  y                       [IN]        Is the position at which the menuitem was displayed
 * RETURNS
 *  void
 *****************************************************************************/
void gui_hide_fixed_icontext_menuitem(void *item, void *common_item_data, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, iwidth, iheight, ix1, iy1, ix2, iy2;
    UI_filled_area *f;
    fixed_icontext_menuitem *m = (fixed_icontext_menuitem*) common_item_data;
    fixed_icontext_menuitem_type *mi = (fixed_icontext_menuitem_type*) item;
    U32 flags;
    PU8 _icon;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = mi->flags;
    flags |= m->flags;
    _icon = mi->item_icon;

    /* Autocalculate the icon position              */
    if (_icon != UI_NULL_IMAGE)
    {
        S32 icon_width, icon_height;

        gui_measure_image(_icon, &icon_width, &icon_height);
        if (flags & UI_MENUITEM_CENTER_ICON_X)
        {
            m->icon_x = (m->width >> 1) - (icon_width >> 1);
        }
        if (flags & UI_MENUITEM_CENTER_ICON_Y)
        {
            m->icon_y = (m->height >> 1) - (icon_height >> 1);
        }
    }

    x1 = x;
    y1 = y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && !(flags & UI_MENUITEM_DISABLE_HIGHLIGHT))
    {
        f = m->focussed_filler;
    }
    else if (flags & UI_MENUITEM_STATE_DISABLED)
    {
        f = m->disabled_filler;
    }
    else if (flags & UI_MENUITEM_STATE_SELECTED)
    {
        f = m->selected_filler;
    }
    else
    {
        f = m->normal_filler;
    }
    gui_set_clip(x1, y1, x2, y2);
    gui_measure_image(_icon, &iwidth, &iheight);
    ix1 = m->icon_x + x1;
    iy1 = m->icon_y + y1;
    ix2 = ix1 + iwidth - 1;
    iy2 = iy1 + iheight - 1;
    gui_push_clip();
    gui_set_clip(ix1, iy1, ix2, iy2);
    gui_draw_filled_area(x1, y1, x2, y2, f);
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_measure_fixed_icontext_menuitem
 * DESCRIPTION
 *  Measures a fixed icontext menuitem
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [IN]        Is the fixed icontext menuitem object (common_data)
 *  width                   [OUT]       
 *  height                  [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
void gui_measure_fixed_icontext_menuitem(void *item, void *common_item_data, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_icontext_menuitem *t = (fixed_icontext_menuitem*) common_item_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    *width = t->width;
    *height = t->height;

#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    // TODO: configure UI_MENUITEM_EXT_SHOW_TWO_LINE_SELECT in other place
    if (current_fixed_list_menuitem_display_index == -1 && gui_get_two_line_menuitem_height())
    {
        *height = *height << 1;
        t->ext_flags |= UI_MENUITEM_EXT_SHOW_TWO_LINE_SELECT;
        return;
    }
    else
    {
        t->ext_flags &= ~UI_MENUITEM_EXT_SHOW_TWO_LINE_SELECT;
    }
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    if (t->ext_flags & UI_MENUITEM_SHOW_HIGHLIGHTED_HINT && current_fixed_list_menuitem_display_index == -1 &&
        gui_get_current_pop_up_description_string())
    {
        *height = (*height) << 1;
    }
    else if (t->ext_flags & UI_MENUITEM_SHOW_ALL_HINTS && gui_get_current_pop_up_description_string())
    {
        *height = (*height) << 1;
    }
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_highlight_fixed_icontext_menuitem
 * DESCRIPTION
 *  Highlights a fixed icontext menuitem (not visually)
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed icontext menuitem object
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_highlight_fixed_icontext_menuitem(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_icontext_menuitem_type *t = (fixed_icontext_menuitem_type*) item;
    fixed_icontext_menuitem *m = (fixed_icontext_menuitem*) common_item_data;
    U32 flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = m->flags;
    flags |= t->flags;
    GUI_current_fixed_icontext_menuitem_common_data = m;
    GUI_current_fixed_icontext_menuitem_data = t;

#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    gui_set_current_two_line_menuitem_data();
#endif 

    gui_fixed_icontext_menuitem_stop_scroll();

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    gui_resize_fixed_icontext_menuitem(m, m->width, m->height);
    if (MMI_current_menu_type == LIST_MATRIX_MENU)
    {
        m->scroll_width = m->width - m->text_x;
    }
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

    gui_fixed_icontext_menuitem_start_scroll();
    if (t->flags & UI_MENUITEM_DISABLE_HIGHLIGHT)
    {
        return;
    }
    t->flags |= UI_MENUITEM_STATE_FOCUSSED;
    t->flags |= UI_MENUITEM_STATE_ANIMATE;
}


/*****************************************************************************
 * FUNCTION
 *  gui_remove_highlight_fixed_icontext_menuitem
 * DESCRIPTION
 *  Removes the highlight of a fixed icontext menuitem (not visually)
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed icontext menuitem object
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)
static scrolling_text gui_two_line_scroll_text;
#endif 
void gui_remove_highlight_fixed_icontext_menuitem(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_icontext_menuitem_type *t = (fixed_icontext_menuitem_type*) item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)
    gui_scrolling_text_stop(&gui_two_line_scroll_text);
#endif 

    UI_UNUSED_PARAMETER(common_item_data);
    t->flags &= ~UI_MENUITEM_STATE_FOCUSSED;
#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)
    gui_reset_current_two_line_menuitem_data();
#endif 
    t->flags &= ~UI_MENUITEM_STATE_ANIMATE;
    if (t->item_icon_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(t->item_icon_handle);
        t->item_icon_handle = GDI_ERROR_HANDLE;
    }
}

/* GUI: fixed icon text list menuitem functions             */

UI_fixed_icontext_list_menuitem_theme *current_fixed_icontext_list_menuitem_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_icontext_list_menuitem_current_theme
 * DESCRIPTION
 *  Applies the current theme to a fixed icontext list menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed icontext list menuitem object   (common data)
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
UI_filled_area fixed_icontext_list_menuitem_transparent_filled_area;
#endif 

void gui_set_fixed_icontext_list_menuitem_current_theme(fixed_icontext_list_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->disabled_filler = current_fixed_icontext_list_menuitem_theme->disabled_filler;
    m->flags |= current_fixed_icontext_list_menuitem_theme->flags;
    m->focussed_filler = current_fixed_icontext_list_menuitem_theme->focussed_filler;
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    if (!m->focussed_filler->flags & UI_FILLED_AREA_TYPE_BITMAP)
    {
        fixed_icontext_list_menuitem_transparent_filled_area = *(m->focussed_filler);
        m->focussed_filler = &fixed_icontext_list_menuitem_transparent_filled_area;
        fixed_icontext_list_menuitem_transparent_filled_area.flags |= UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
        fixed_icontext_list_menuitem_transparent_filled_area.c.alpha = HIGHLIGHTER_COLOR_OPACITY_VALUE;
    }
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
    m->normal_filler = current_fixed_icontext_list_menuitem_theme->normal_filler;
    m->selected_filler = current_fixed_icontext_list_menuitem_theme->selected_filler;
    for (i = 0; i < m->n_text_columns; i++)
    {
        m->normal_text_colors[i] = current_fixed_icontext_list_menuitem_theme->normal_text_colors[i];
        m->focussed_text_colors[i] = current_fixed_icontext_list_menuitem_theme->focussed_text_colors[i];
        m->selected_text_colors[i] = current_fixed_icontext_list_menuitem_theme->selected_text_colors[i];
        m->disabled_text_colors[i] = current_fixed_icontext_list_menuitem_theme->disabled_text_colors[i];
        m->text_fonts[i] = current_fixed_icontext_list_menuitem_theme->text_fonts[i];
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_icontext_list_menuitem_theme
 * DESCRIPTION
 *  Applies the given theme to a fixed icontext list menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed icontext list menuitem object   (common data)
 *  t       [IN]        Is the theme
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_icontext_list_menuitem_theme(
        fixed_icontext_list_menuitem *m,
        UI_fixed_icontext_list_menuitem_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->disabled_filler = t->disabled_filler;
    m->flags |= t->flags;
    m->focussed_filler = t->focussed_filler;
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    if (!m->focussed_filler->flags & UI_FILLED_AREA_TYPE_BITMAP)
    {
        fixed_icontext_list_menuitem_transparent_filled_area = *(m->focussed_filler);
        m->focussed_filler = &fixed_icontext_list_menuitem_transparent_filled_area;
        fixed_icontext_list_menuitem_transparent_filled_area.flags |= UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
        fixed_icontext_list_menuitem_transparent_filled_area.c.alpha = HIGHLIGHTER_COLOR_OPACITY_VALUE;
    }
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
    m->normal_filler = t->normal_filler;
    m->selected_filler = t->selected_filler;
    for (i = 0; i < m->n_text_columns; i++)
    {
        m->normal_text_colors[i] = t->normal_text_colors[i];
        m->focussed_text_colors[i] = t->focussed_text_colors[i];
        m->selected_text_colors[i] = t->selected_text_colors[i];
        m->disabled_text_colors[i] = t->disabled_text_colors[i];
        m->text_fonts[i] = t->text_fonts[i];
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_fixed_icontext_list_menuitem
 * DESCRIPTION
 *  Creates a fixed icontext list menuitem
 * PARAMETERS
 *  m                   [IN]        Is the fixed icontext list menuitem object (common data)
 *  width               [IN]        Width is the width of the fixed icontext list menuitem
 *  height              [IN]        Height is the height of the fixed icontext list menuitem
 *  n_text_columns      [IN]        Is the number of text columns
 *  n_icon_columns      [IN]        Is the number of icon columns
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_fixed_icontext_list_menuitem(
        fixed_icontext_list_menuitem *m,
        S32 width,
        S32 height,
        S32 n_text_columns,
        S32 n_icon_columns)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    m->flags = 0;
    m->ext_flags = 0;
    m->icon_transparent_color = gui_transparent_color(0, 0, 0);
    if (n_icon_columns > FIXED_ICONTEXT_LIST_MAX_ICON_COLUMNS)
    {
        n_icon_columns = FIXED_ICONTEXT_LIST_MAX_ICON_COLUMNS;
    }
    if (n_text_columns > FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS)
    {
        n_text_columns = FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS;
    }
    m->n_icon_columns = n_icon_columns;
    m->n_text_columns = n_text_columns;
    gui_set_fixed_icontext_list_menuitem_current_theme(m);
    m->scroll_x = 0;
    m->scroll_width = width;
    m->post_display_function = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_list_menuitem_set_post_display
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m                           [?]         
 *  post_display_function       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_list_menuitem_set_post_display(
        fixed_icontext_list_menuitem *m,
        void (*post_display_function) (void *item, void *common_item_data, S32 x, S32 y))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->post_display_function = post_display_function;
}

/* Text scrolling related functions within the menu item */

fixed_icontext_list_menuitem *GUI_current_fixed_icontext_list_menuitem_common_data = NULL;
fixed_icontext_list_menuitem_type *GUI_current_fixed_icontext_list_menuitem_data = NULL;
U8 GUI_current_fixed_icontext_list_menuitem_scroll = 0;
U8 GUI_current_fixed_icontext_list_menuitem_scroll_direction;
S32 GUI_current_fixed_icontext_list_menuitem_x;
S32 GUI_current_fixed_icontext_list_menuitem_y;
S32 GUI_current_fixed_icontext_list_menuitem_scroll_counter = 0;

/* Current text column for marqueeing. Auto selected with UI_MENUITEM_AUTO_CHOOSE_MARQUEE. */
S32 GUI_current_fixed_icontext_list_menuitem_column = 0;
UI_string_type GUI_current_fixed_icontext_list_menuitem_scroll_text = NULL;
UI_string_type GUI_current_fixed_icontext_list_menuitem_scroll_text_end = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_list_menuitem_scroll_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_list_menuitem_scroll_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 flags = GUI_current_fixed_icontext_list_menuitem_common_data->flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags |= GUI_current_fixed_icontext_list_menuitem_data->flags;
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_reset_current_list_effect();
#endif 
    if (GUI_current_fixed_icontext_list_menuitem_scroll_counter)
    {
        GUI_current_fixed_icontext_list_menuitem_scroll_counter--;
    }
    else
    {
    #if(UI_USE_WORD_SCROLLING_TEXT)
        GUI_current_fixed_icontext_list_menuitem_scroll_text = GUI_current_fixed_icontext_list_menuitem_scroll_text_end;
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        if (flags & UI_MENUITEM_MARQUEE_SCROLL)
        {
            if (!r2lMMIFlag)
            {
                GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x -= UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x <=
                    (-(GUI_current_fixed_icontext_list_menuitem_common_data->text_width + UI_text_menuitem_scroll_gap)))
                {
                    GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x = 0;
                }
            }
            else
            {
                GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x += UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x >=
                    (GUI_current_fixed_icontext_list_menuitem_common_data->text_width + UI_text_menuitem_scroll_gap))
                {
                    GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x = 0;
                }
            }
        }
        else
        {
            if (GUI_current_fixed_icontext_list_menuitem_scroll_direction == 0)
            {
                GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x -= UI_text_menuitem_scroll_size;
                if ((GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x +
                     GUI_current_fixed_icontext_list_menuitem_common_data->text_width) <=
                    (GUI_current_fixed_icontext_list_menuitem_common_data->scroll_width))
                {
                    GUI_current_fixed_icontext_list_menuitem_scroll_direction = 1;
                    GUI_current_fixed_icontext_list_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;
                }
            }
            else if (GUI_current_fixed_icontext_list_menuitem_scroll_direction == 1)
            {
                GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x += UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_icontext_list_menuitem_common_data->scroll_x >= 0)
                {
                    GUI_current_fixed_icontext_list_menuitem_scroll_direction = 0;
                    GUI_current_fixed_icontext_list_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;
                }
            }
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        GUI_current_fixed_icontext_list_menuitem_common_data->ext_flags |= UI_MENUITEM_EXT_SHOW_IN_MARQUEE;

    #ifdef __MMI_UI_TRANSPARENT_EFFECT__
        if (!gui_is_current_transparency_with_multi_layer())
        {
            gui_fixed_icontext_list_disable_transparent_effect(GUI_current_fixed_icontext_list_menuitem_common_data);   /* To hide text below scrolling text */
        }
    #endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 

        gui_show_fixed_icontext_list_menuitem(
            (void*)GUI_current_fixed_icontext_list_menuitem_data,
            (void*)GUI_current_fixed_icontext_list_menuitem_common_data,
            GUI_current_fixed_icontext_list_menuitem_x,
            GUI_current_fixed_icontext_list_menuitem_y);

    #ifdef __MMI_UI_TRANSPARENT_EFFECT__
        gui_fixed_icontext_list_enable_transparent_effect(GUI_current_fixed_icontext_list_menuitem_common_data);        /* To enable transparency */
    #endif 

        GUI_current_fixed_icontext_list_menuitem_common_data->ext_flags &= ~UI_MENUITEM_EXT_SHOW_IN_MARQUEE;
        gui_BLT_double_buffer(
            GUI_current_fixed_icontext_list_menuitem_x,
            GUI_current_fixed_icontext_list_menuitem_y,
            GUI_current_fixed_icontext_list_menuitem_x + GUI_current_fixed_icontext_list_menuitem_common_data->width - 1,
            GUI_current_fixed_icontext_list_menuitem_y + GUI_current_fixed_icontext_list_menuitem_common_data->height - 1);
    }
    gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_icontext_list_menuitem_scroll_handler);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_list_menuitem_reset_scroll_text_column
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_list_menuitem_reset_scroll_text_column(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* This cannot be done in gui_fixed_icontext_list_menuitem_stop_scroll() 
       because we need to keep text scroll column unchanged when highlight is switched */
    GUI_current_fixed_icontext_list_menuitem_column = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_list_menuitem_start_scroll
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_list_menuitem_start_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_icontext_list_menuitem_type *t =
        (fixed_icontext_list_menuitem_type*) GUI_current_fixed_icontext_list_menuitem_data;
    fixed_icontext_list_menuitem *m =
        (fixed_icontext_list_menuitem*) GUI_current_fixed_icontext_list_menuitem_common_data;
    S32 w;
    U32 flags;
    BOOL do_scroll = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = m->flags;
    flags |= t->flags;

    gui_add_cleanup_hook(gui_fixed_icontext_list_menuitem_reset_scroll_text_column);

    if (flags & UI_MENUITEM_AUTO_CHOOSE_MARQUEE)
    {
        S32 i;

        for (i = 0; i < m->n_text_columns; i++)
        {
            gui_set_font(m->text_fonts[i]);
            w = gui_get_string_width(t->item_texts[i]);
            m->text_width = w;
            m->scroll_x = 0;
            if (w >= m->text_coordinates[i].width - 1)
            {
                GUI_current_fixed_icontext_list_menuitem_column = i;
                m->scroll_width = m->text_coordinates[i].width;
                do_scroll = MMI_TRUE;
                break;
            }
        }
    }
    else
    {
        if (GUI_current_fixed_icontext_list_menuitem_column >= m->n_text_columns)
        {
            MMI_DBG_ASSERT(0);
            GUI_current_fixed_icontext_list_menuitem_column = 0;
        }

        gui_set_font(m->text_fonts[GUI_current_fixed_icontext_list_menuitem_column]);
        w = gui_get_string_width(t->item_texts[GUI_current_fixed_icontext_list_menuitem_column]);
        m->text_width = w;
        m->scroll_x = 0;
        if (w >= m->scroll_width - 1)
        {
            do_scroll = MMI_TRUE;
        }
    }

    if (do_scroll)
    {
    #if(UI_USE_WORD_SCROLLING_TEXT)
        GUI_current_fixed_icontext_list_menuitem_scroll_text =
            t->item_texts[GUI_current_fixed_icontext_list_menuitem_column];
        GUI_current_fixed_icontext_list_menuitem_scroll = 1;
        GUI_current_fixed_icontext_list_menuitem_scroll_direction = 0;
        GUI_current_fixed_icontext_list_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        if (gui_get_current_list_effect() && gblock_list_effect != 1)
        {
            gui_register_callback_menuitem_scroll(gui_fixed_icontext_list_menuitem_scroll_handler);
        }
        else
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
        {
            gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_icontext_list_menuitem_scroll_handler);
        }
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        if ((flags & UI_MENUITEM_MARQUEE_SCROLL) || (flags & UI_MENUITEM_TWO_DIRECTION_SCROLL))
        {
            GUI_current_fixed_icontext_list_menuitem_scroll = 1;
            GUI_current_fixed_icontext_list_menuitem_scroll_direction = 0;
            GUI_current_fixed_icontext_list_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;

        #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
            if (gui_get_current_list_effect() && gblock_list_effect != 1)
            {
                gui_register_callback_menuitem_scroll(gui_fixed_icontext_list_menuitem_scroll_handler);
            }
            else
        #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
            {
                gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_icontext_list_menuitem_scroll_handler);
            }
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
    }
    else
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    {
        gui_register_callback_menuitem_scroll(gui_dummy_scroll_handler_for_list_effects);
        GUI_current_fixed_icontext_list_menuitem_scroll = 0;
    }
#else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    {
        GUI_current_fixed_icontext_list_menuitem_scroll = 0;
    }
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_list_menuitem_stop_scroll
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_list_menuitem_stop_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GUI_current_fixed_icontext_list_menuitem_scroll)
    {
        gui_cancel_timer(gui_fixed_icontext_list_menuitem_scroll_handler);
        GUI_current_fixed_icontext_list_menuitem_scroll = 0;
    }

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_register_callback_menuitem_scroll(gui_dummy_scroll_handler_for_list_effects);
    gui_restore_current_list_effect();
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_list_menuitem_set_text_coordinates
 * DESCRIPTION
 *  Sets the co-ordinates for a text column
 * PARAMETERS
 *  m               [IN]        Is the fixed icontext list menuitem object (common data)
 *  text_column     [IN]        Is the column number of the text column (zero based)
 *  x               [IN]        Is the position of this text column within the menuitem
 *  y               [IN]        Is the position of this text column within the menuitem
 *  width           [IN]        Are the dimensions of the this text column
 *  height          [IN]        Are the dimensions of the this text column
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_list_menuitem_set_text_coordinates(
        fixed_icontext_list_menuitem *m,
        S32 text_column,
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
    if (text_column >= m->n_text_columns)
    {
        return;
    }
    m->text_coordinates[text_column].x = x;
    m->text_coordinates[text_column].y = y;
    m->text_coordinates[text_column].width = width;
    m->text_coordinates[text_column].height = height;
    if (text_column == GUI_current_fixed_icontext_list_menuitem_column)
    {
        m->scroll_width = width;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_icontext_list_menuitem_set_icon_coordinates
 * DESCRIPTION
 *  Sets the co-ordinates for a icon column
 * PARAMETERS
 *  m               [IN]        Is the fixed icontext list menuitem object (common data)
 *  icon_column     [IN]        Is the column number of the icon column (zero based)
 *  x               [IN]        Is the position of this icon column within the menuitem
 *  y               [IN]        Is the position of this icon column within the menuitem
 *  width           [IN]        Are the dimensions of the this icon column
 *  height          [IN]        Are the dimensions of the this icon column
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_icontext_list_menuitem_set_icon_coordinates(
        fixed_icontext_list_menuitem *m,
        S32 icon_column,
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
    if (icon_column >= m->n_icon_columns)
    {
        return;
    }
    m->icon_coordinates[icon_column].x = x;
    m->icon_coordinates[icon_column].y = y;
    m->icon_coordinates[icon_column].width = width;
    m->icon_coordinates[icon_column].height = height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_fixed_icontext_list_menuitem
 * DESCRIPTION
 *  Changes the size of a fixed icontext list menuitem
 * PARAMETERS
 *  m           [IN]        Is the fixed icontext list menuitem object   (common data)
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_fixed_icontext_list_menuitem(fixed_icontext_list_menuitem *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_fixed_icontext_list_menuitem
 * DESCRIPTION
 *  Displays the fixed icontext list menuitem
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed icontext list menuitem object
 *  common_item_data        [IN]        Is the fixed icontext list menuitem object (common_data)
 *  x                       [IN]        Is the position at which the menuitem is displayed
 *  y                       [IN]        Is the position at which the menuitem is displayed
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_fixed_icontext_list_menuitem(void *item, void *common_item_data, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 ix1, iy1, ix2, iy2;
    color_t text_color;
    UI_filled_area *f = NULL;
    fixed_icontext_list_menuitem *m = (fixed_icontext_list_menuitem*) common_item_data;
    fixed_icontext_list_menuitem_type *mi = (fixed_icontext_list_menuitem_type*) item;
    U32 flags;
    UI_string_type _text;
    PU8 _icon;
    U8 animation_flag = 0;
    U8 focussed_flag = 0;
    U8 disabled_flag = 0;
    U8 selected_flag = 0;
    S32 i;
    color_t c;
    S32 sw, sh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = mi->flags;
    flags |= m->flags;
    c = gui_color(0, 0, 0);

    if (flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        GUI_current_fixed_icontext_list_menuitem_x = x;
        GUI_current_fixed_icontext_list_menuitem_y = y;
    }

    x1 = x;
    y1 = y;
    y2 = y1 + m->height - 1;
    x2 = x1 + m->width - 1;

    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && !(flags & UI_MENUITEM_DISABLE_HIGHLIGHT))
    {
        f = m->focussed_filler;
        focussed_flag = 1;
        if (flags & UI_MENUITEM_FOCUSSED_ANIMATE)
        {
            animation_flag = 1;
        }
    }
    else if (flags & UI_MENUITEM_STATE_DISABLED)
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND))
        {
            f = m->disabled_filler;
        }
        disabled_flag = 1;
    }
    else if (flags & UI_MENUITEM_STATE_SELECTED)
    {
        f = m->selected_filler;
        if (flags & UI_MENUITEM_SELECTED_ANIMATE)
        {
            animation_flag = 1;
        }
        selected_flag = 1;
    }
    else
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND))
        {
            f = m->normal_filler;
        }
    }

    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);

    if (f != NULL && (!(flags & UI_MENUITEM_HIGHLIGHT_SWITCH_ICON)))
    {
    #ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        if (is_draw_next_frame_list_highlight_effect_value())
    #endif
        {
            g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_HIGHLIGHT_ICONTEXT_LIST;
        }
    #endif

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        set_start_position_and_item_parameters_for_list_highlighter_effect(item, common_item_data, x1, y1);
    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw)
        {
            gui_draw_list_filled_area(x1, y1 + 1, x2 - 1, y2, f);
        }
        else
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
        {
            gui_draw_list_filled_area(x1, y1, x2, y2, f);
        }
    #else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if ((line_draw && GUI_current_fixed_icontext_menuitem_scroll))
        {
            gui_draw_filled_area(x1, y1 + 1, x2 - 1, y2, f);
        }
        else
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
        {
            gui_draw_filled_area(x1, y1, x2, y2, f);
        }
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 

    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw)
        {
            gui_draw_vertical_line(
                MMI_fixed_list_menu.y + 1,
                MMI_fixed_list_menu.y + MMI_fixed_list_menu.height - 2,
                MMI_fixed_list_menu.x,
                current_MMI_theme->list_background_filler->border_color);
        }
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    }

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    /* To hide the text and icon of the menuitem up to the last frame for flash feel effect */
    if (gblock_list_effect == 0)
    {
        if (gui_get_current_list_effect() == MMI_UI_LIST_HIGHLIGHT_EFFECT_FLASH_FEELING &&
            MMI_current_menu_type == LIST_MENU)
        {
            if (flash_feel_last_frame == 0)
            {
                if (flags & UI_MENUITEM_STATE_FOCUSSED)
                {
                    flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY;
                }
            }
        }
    }
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 

    for (i = 0; i < m->n_icon_columns; i++)
    {
        /* For bi-degree main menu */
        /* selected item, should draw selected image */
        if (flags & UI_MENUITEM_HIGHLIGHT_SWITCH_ICON)
        {
            if (flags & UI_MENUITEM_STATE_FOCUSSED)
            {
                i++;
            }
            /* normal item, should clear previous drawn image */
            else
            {
            #ifdef __GDI_MEMORY_PROFILE_2__
                gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
            #endif 
                continue;
            }
        }

        if (mi->item_icon_handles[i] != GDI_ERROR_HANDLE)
        {
            gdi_image_stop_animation(mi->item_icon_handles[i]);
            mi->item_icon_handles[i] = GDI_ERROR_HANDLE;
        }
        _icon = mi->item_icons[i];
        /* _icon_handle=mi->item_icon_handles[i]; */

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        /* To hide the icon of the menuitem up to the last frame for flash feel effect */
        if (gblock_list_effect == 0)
        {
            if (gui_get_current_list_effect() == MMI_UI_LIST_HIGHLIGHT_EFFECT_FLASH_FEELING &&
                MMI_current_menu_type == LIST_MENU)
            {
                if (flash_feel_last_frame == 0)
                {
                    if (flags & UI_MENUITEM_STATE_FOCUSSED)
                    {
                        _icon = UI_NULL_IMAGE;
                    }
                }
            }
        }
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 

        /* Autocalculate the icon position              */
        if (_icon != UI_NULL_IMAGE)
        {
            S32 icon_width, icon_height;

            gui_measure_image(_icon, &icon_width, &icon_height);
            if (flags & UI_MENUITEM_CENTER_ICON_X)
            {
                m->icon_coordinates[i].x = (m->width >> 1) - (icon_width >> 1);
            }

            if (!(m->flags & MENU_MUTLIROW_ICON_LIST))
            {
                if (flags & UI_MENUITEM_CENTER_ICON_Y)
                {
                    m->icon_coordinates[i].y = (m->height >> 1) - (icon_height >> 1);
                }
            }
        }

        if (r2lMMIFlag)
        {
            ix1 = x2 - m->icon_coordinates[i].x - m->icon_coordinates[i].width + 1;
        }
        else
        {
            ix1 = m->icon_coordinates[i].x + x1;
        }

    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw && m->icon_coordinates[i].width)
        {
            gui_reset_clip();
            if (!r2lMMIFlag)
            {
                gui_draw_vertical_line(y1, y2, m->icon_coordinates[i].x - ((TEXT_ICON_GAP + 1) >> 1), current_MMI_theme->list_background_filler->border_color); /* Vertical partition for diff type */
            }
            else
            {
                gui_draw_vertical_line(y1, y2, ix1 - ((TEXT_ICON_GAP + 1) >> 1), current_MMI_theme->list_background_filler->border_color);      /* Vertical partition for diff type */
            }
        }
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 

        iy1 = m->icon_coordinates[i].y + y1;
        ix2 = ix1 + m->icon_coordinates[i].width - 1;
        iy2 = iy1 + m->icon_coordinates[i].height - 1;

        if (ix2 > x2)
        {
            ix2 = x2;
        }
        if (iy2 > y2)
        {
            iy2 = y2;
        }

        gui_set_clip(ix1, iy1, ix2, iy2);
        if (_icon != UI_NULL_IMAGE)
        {
            if (animation_flag)
            {
            #if(UI_MAIN_MENU_ANIMATION_TYPE == UI_MAIN_MENU_ANIMATION_TYPE1)
                gdi_image_draw_animation(ix1, iy1, _icon, &(mi->item_icon_handles[i]) /* &animation_handle */ );
                /* mi->item_icon_handles[i]=gui_show_transparent_animated_image(ix1,iy1,_icon,m->icon_transparent_color); */
            #elif(UI_MAIN_MENU_ANIMATION_TYPE==UI_MAIN_MENU_ANIMATION_TYPE2)
                gdi_image_draw_animation(ix1, iy1, _icon, &(mi->item_icon_handles[i]) /* &animation_handle */ );
                /* mi->item_icon_handles[i]=gui_show_transparent_animated_image_frames(ix1,iy1,_icon,m->icon_transparent_color,1,0); */
            #endif 
                m->flags |= UI_MENUITEM_STATE_ANIMATE;
            }
            else if (m->flags & UI_MENUITEM_STATE_ANIMATE)
            {
                gui_show_transparent_animated_image_frame(ix1, iy1, _icon, m->icon_transparent_color, 0);
                m->flags &= ~UI_MENUITEM_STATE_ANIMATE;
            }
            else
            {
                gui_show_transparent_animated_image_frame(ix1, iy1, _icon, m->icon_transparent_color, 0);
            }
        }
        /* For bi-degree main menu */
        if (flags & UI_MENUITEM_HIGHLIGHT_SWITCH_ICON)
        {
            break;
        }
    }

    gui_push_text_clip();
    gui_set_text_clip(x1, y1, x2, y2);   
    if (!(flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY))
    {
        for (i = 0; i < m->n_text_columns; i++)
        {
            _text = mi->item_texts[i];
            gui_set_font(m->text_fonts[i]);
            gui_measure_string(_text, &sw, &sh);
            if (flags & UI_MENUITEM_CENTER_TEXT_X)
            {
                m->text_coordinates[i].x = (m->width >> 1) - (sw >> 1);
                ix1 = m->text_coordinates[i].x + x1;
                ix2 = ix1 + m->text_coordinates[i].width - 1;
            }

            if (!(m->flags & MENU_MUTLIROW_ICON_LIST))
            {
                if (flags & UI_MENUITEM_CENTER_TEXT_Y)
                {
                    m->text_coordinates[i].y = (m->height >> 1) - (sh >> 1);
                    iy1 = m->text_coordinates[i].y + y1;
                    iy2 = iy1 + m->text_coordinates[i].height - 1;
                }
            }

            if (r2lMMIFlag)
            {
                ix1 = x2 - m->text_coordinates[i].x - m->text_coordinates[i].width + 1;
            }
            else
            {
                ix1 = m->text_coordinates[i].x + x1;
            }

            iy1 = m->text_coordinates[i].y + y1;
            ix2 = ix1 + m->text_coordinates[i].width - 1;
            iy2 = iy1 + m->text_coordinates[i].height - 1;

            if (ix2 > x2)
            {
                ix2 = x2;
            }
            if (iy2 > y2)
            {
                iy2 = y2;
            }

            if ((i == 0) && (flags & UI_MENUITEM_FILL_COL1))
            {
                int j;

                gui_set_clip((ix1 - x1), y1, ix2, y2);

                for (j = 0; j < FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS; j++)
                {
                    m->focussed_text_colors[j] = c;
                }
                gui_draw_filled_area((ix1 - x1), y1, ix2, y2, &colorangewgui_pop_up_dialog_background);
            }
            if ((i == 1) && (flags & UI_MENUITEM_FILL_COL2))
            {
                int j;

                gui_set_clip((ix1 - x1), y1, ix2, y2);

                for (j = 0; j < FIXED_ICONTEXT_LIST_MAX_TEXT_COLUMNS; j++)
                {
                    m->focussed_text_colors[j] = c;
                }
                gui_draw_filled_area((ix1 - x1), y1, ix2, y2, &colorangewgui_pop_up_dialog_background);
            }
            if (_text != NULL)
            {
                if (_text[0] != '\0')
                {
                    if (disabled_flag)
                    {
                        text_color = m->disabled_text_colors[i];
                    }
                    else if (focussed_flag)
                    {
                        text_color = m->focussed_text_colors[i];
                    }
                    else if (selected_flag)
                    {
                        text_color = m->selected_text_colors[i];
                    }
                    else
                    {
                        text_color = m->normal_text_colors[i];
                    }

                    if (!(flags & UI_MENUITEM_DISABLE_TEXT_CLIP))
                    {
                        gui_set_text_clip(ix1, iy1, ix2, iy2);
                    }
                    gui_set_text_color(text_color);

                    /* Added specifically to right justify second column in case of two column display with numbers */
                    if ((i == 1) && (flags & UI_MENUITEM_RIGHT_JUSTIFY_COL2))
                    {
                        if (r2lMMIFlag)
                        {
                            gui_move_text_cursor(ix1 + sw, iy1);
                        }
                        else
                        {
                            gui_move_text_cursor(ix2 - sw, iy1);
                        }

                        gui_set_line_height(sh);
                        gui_print_text(_text);
                    }
                    else
                    {
                        if ((flags & UI_MENUITEM_STATE_FOCUSSED) &&
                            ((i == GUI_current_fixed_icontext_list_menuitem_column) ||
                             (m->flags & MENU_MUTLIROW_ICON_LIST)))
                            /* Second Column Text Scroll Change For Multi Row List */
                        {
                            if (sw > m->text_coordinates[i].width)
                            {
                                if (r2lMMIFlag)
                                {
                                    gui_move_text_cursor(ix2 + m->scroll_x, iy1);
                                }
                                else
                                {
                                    gui_move_text_cursor(ix1 + m->scroll_x, iy1);
                                }
                            }
                            else
                            {
                                if (r2lMMIFlag)
                                {
                                    gui_move_text_cursor(ix2, iy1);
                                }
                                else
                                {
                                    gui_move_text_cursor(ix1, iy1);
                                }
                            }
                            gui_set_line_height(sh);

                            /* trucate too long number/name in call list. */
                            if (flags & UI_MENUITEM_TRUNCATE_CONTENT)
                            {
                                if (r2lMMIFlag)
                                {
                                    gui_print_truncated_text(ix2, iy1, m->text_coordinates[0].width, _text);
                                }
                                else
                                {
                                    gui_print_truncated_text(ix1, iy1, m->text_coordinates[0].width, _text);
                                }
                            }
                            else
                            {
                            #if(UI_USE_WORD_SCROLLING_TEXT)
                                if (GUI_current_fixed_icontext_list_menuitem_scroll)
                                {
                                    GUI_current_fixed_icontext_list_menuitem_scroll_text_end =
                                        GUI_current_fixed_icontext_list_menuitem_scroll_text;
                                    if (gui_print_truncated_text2
                                        (ix1 + m->scroll_x, iy1, ix2 - ix1 + 1,
                                         &GUI_current_fixed_icontext_list_menuitem_scroll_text_end))
                                    {
                                        GUI_current_fixed_icontext_menuitem_scroll_text_end = _text;
                                    }
                                }
                                else
                                {
                                    gui_print_truncated_text2(ix1 + m->scroll_x, iy1, ix2 - ix1 + 1, &_text);
                                }
                            #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
                                gui_print_text(_text);
                                if ((flags & UI_MENUITEM_MARQUEE_SCROLL) &&
                                    GUI_current_fixed_icontext_list_menuitem_scroll)
                                {
                                    if (sw > m->text_coordinates[i].width)
                                    {
                                        if (r2lMMIFlag)
                                        {
                                            gui_move_text_cursor(
                                                ix2 + m->scroll_x - (m->text_width + UI_text_menuitem_scroll_gap),
                                                iy1);
                                        }
                                        else
                                        {
                                            gui_move_text_cursor(
                                                ix1 + m->scroll_x + m->text_width + UI_text_menuitem_scroll_gap,
                                                iy1);
                                        }
                                        gui_set_line_height(sh);
                                        gui_print_text(_text);
                                    }
                                }
                            #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 

                            #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
                                if (line_draw)
                                {
                                    gui_reset_clip();
                                    if (!r2lMMIFlag)
                                    {
                                        gui_draw_vertical_line(y1, y2, m->text_coordinates[i].x - ((TEXT_ICON_GAP + 1) >> 1), current_MMI_theme->list_background_filler->border_color); /* Line before text item */
                                    }
                                    if (r2lMMIFlag && (m->n_text_columns > 1) && m->text_coordinates[i].width)
                                    {
                                        gui_draw_vertical_line(y1, y2, x2 - (m->text_coordinates[i].x - ((TEXT_ICON_GAP + 1) >> 1)), current_MMI_theme->list_background_filler->border_color);  /* Line before text item */
                                    }
                                }
                            #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
                            }
                        }
                        else
                        {
                            gui_set_line_height(sh);
                            if (flags & UI_MENUITEM_TRUNCATE_CONTENT)
                            {
                                if (r2lMMIFlag)
                                {
                                    gui_print_truncated_text(ix2, iy1, m->text_coordinates[0].width, _text);
                                }
                                else
                                {
                                    gui_print_truncated_text(ix1, iy1, m->text_coordinates[0].width, _text);
                                }
                            }
                            else
                            {
                                if (r2lMMIFlag)
                                {
                                    gui_move_text_cursor(ix2, iy1);
                                }
                                else
                                {
                                    gui_move_text_cursor(ix1, iy1);
                                }
                                gui_print_text(_text);
                            }
                        }
                    }
                }
            }

        #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
            if (line_draw)
            {
                gui_reset_clip();
                if (!r2lMMIFlag)
                {
                    gui_draw_vertical_line(y1, y2, m->text_coordinates[i].x - ((TEXT_ICON_GAP + 1) >> 1), current_MMI_theme->list_background_filler->border_color);     /* To handle scrolling text overlapping */
                }
                if (r2lMMIFlag && (m->n_text_columns > 1) && m->text_coordinates[i].width)
                {
                    gui_draw_vertical_line(y1, y2, x2 - (m->text_coordinates[i].x - ((TEXT_ICON_GAP + 1) >> 1)), current_MMI_theme->list_background_filler->border_color);      /* Line before text item */
                }
            }
        #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
        }
    }

    if (m->post_display_function)
    {
        m->post_display_function(item, common_item_data, x, y);
    }
    gui_pop_text_clip();
    gui_pop_clip();

#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && !(m->ext_flags & UI_MENUITEM_EXT_SHOW_IN_MARQUEE))
    {
        gui_display_pop_up_description(x, y, m->width, m->height);
    }
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_measure_fixed_icontext_list_menuitem
 * DESCRIPTION
 *  Measures a fixed icontext list menuitem
 * PARAMETERS
 *  item                    [?]         
 *  m                       [?]         
 *  width                   [OUT]       
 *  height                  [OUT]       
 *  common_item_data(?)     [IN]        Is the fixed icontext list menuitem object (common_data)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_measure_fixed_icontext_list_menuitem(void *item, void *m, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_icontext_list_menuitem *t = (fixed_icontext_list_menuitem*) m;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    *width = t->width;
    *height = t->height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_highlight_fixed_icontext_list_menuitem
 * DESCRIPTION
 *  Highlights a fixed icontext list menuitem (not visually)
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed icontext list menuitem object
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_highlight_fixed_icontext_list_menuitem(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_icontext_list_menuitem_type *t = (fixed_icontext_list_menuitem_type*) item;
    fixed_icontext_list_menuitem *m = (fixed_icontext_list_menuitem*) common_item_data;
    U32 flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = m->flags;
    flags |= t->flags;
    GUI_current_fixed_icontext_list_menuitem_common_data = m;
    GUI_current_fixed_icontext_list_menuitem_data = t;
    gui_fixed_icontext_list_menuitem_stop_scroll();
    gui_fixed_icontext_list_menuitem_start_scroll();
    if (t->flags & UI_MENUITEM_DISABLE_HIGHLIGHT)
    {
        return;
    }
    t->flags |= UI_MENUITEM_STATE_FOCUSSED;
}


/*****************************************************************************
 * FUNCTION
 *  gui_remove_highlight_fixed_icontext_list_menuitem
 * DESCRIPTION
 *  Removes the highlight of a fixed icontext list menuitem (not visually)
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed icontext list menuitem object
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_remove_highlight_fixed_icontext_list_menuitem(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = 0;
    fixed_icontext_list_menuitem_type *t = (fixed_icontext_list_menuitem_type*) item;
    fixed_icontext_list_menuitem *m = (fixed_icontext_list_menuitem*) common_item_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    t->flags &= ~UI_MENUITEM_STATE_FOCUSSED;
    for (i = 0; i < m->n_icon_columns; i++)
    {
        if (t->item_icon_handles[i] != GDI_ERROR_HANDLE)
        {
            gdi_image_stop_animation(t->item_icon_handles[i]);
            t->item_icon_handles[i] = GDI_ERROR_HANDLE;
        }
    }
}

/* GUI: fixed two state menuitem functions               */

UI_fixed_twostate_menuitem_theme *current_fixed_twostate_menuitem_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_twostate_menuitem_current_theme
 * DESCRIPTION
 *  Applies the current theme to a fixed twostate menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed twostate menuitem object  (common data)
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
UI_filled_area fixed_twostate_menuitem_transparent_filled_area;
#endif 

void gui_set_fixed_twostate_menuitem_current_theme(fixed_twostate_menuitem *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->disabled_filler = current_fixed_twostate_menuitem_theme->disabled_filler;
    m->disabled_text_color = current_fixed_twostate_menuitem_theme->disabled_text_color;
    m->flags |= current_fixed_twostate_menuitem_theme->flags;
    m->focussed_filler = current_fixed_twostate_menuitem_theme->focussed_filler;

#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    if (!m->focussed_filler->flags & UI_FILLED_AREA_TYPE_BITMAP)
    {
        fixed_twostate_menuitem_transparent_filled_area = *(m->focussed_filler);
        m->focussed_filler = &fixed_twostate_menuitem_transparent_filled_area;
        fixed_twostate_menuitem_transparent_filled_area.flags |= UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
        fixed_twostate_menuitem_transparent_filled_area.c.alpha = HIGHLIGHTER_COLOR_OPACITY_VALUE;
    }
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 

    m->focussed_text_color = current_fixed_twostate_menuitem_theme->focussed_text_color;
    m->normal_filler = current_fixed_twostate_menuitem_theme->normal_filler;
    m->normal_text_color = current_fixed_twostate_menuitem_theme->normal_text_color;
    m->selected_filler = current_fixed_twostate_menuitem_theme->selected_filler;
    m->selected_text_color = current_fixed_twostate_menuitem_theme->selected_text_color;
    m->text_font = current_fixed_twostate_menuitem_theme->text_font;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_fixed_twostate_menuitem_theme
 * DESCRIPTION
 *  Applies the given theme to a fixed twostate menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed twostate menuitem object  (common data)
 *  t       [IN]        Is the theme
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_fixed_twostate_menuitem_theme(fixed_twostate_menuitem *m, UI_fixed_twostate_menuitem_theme *t)
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

#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    if (!m->focussed_filler->flags & UI_FILLED_AREA_TYPE_BITMAP)
    {
        fixed_twostate_menuitem_transparent_filled_area = *(m->focussed_filler);
        m->focussed_filler = &fixed_twostate_menuitem_transparent_filled_area;
        fixed_twostate_menuitem_transparent_filled_area.flags |= UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
        fixed_twostate_menuitem_transparent_filled_area.c.alpha = HIGHLIGHTER_COLOR_OPACITY_VALUE;
    }
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 

    m->focussed_text_color = t->focussed_text_color;
    m->normal_filler = t->normal_filler;
    m->normal_text_color = t->normal_text_color;
    m->selected_filler = t->selected_filler;
    m->selected_text_color = t->selected_text_color;
    m->text_font = t->text_font;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_fixed_twostate_menuitem
 * DESCRIPTION
 *  Creates a fixed twostate menuitem
 * PARAMETERS
 *  m               [IN]        Is the fixed twostate menuitem object (common data)
 *  width           [IN]        Width is the width of the fixed twostate menuitem
 *  height          [IN]        Height is the height of the fixed twostate menuitem
 *  ON_icon         [IN]        
 *  OFF_icon        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_fixed_twostate_menuitem(fixed_twostate_menuitem *m, S32 width, S32 height, PU8 ON_icon, PU8 OFF_icon)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    m->flags = 0;
    m->ext_flags = 0;
    m->icon_transparent_color = gui_transparent_color(0, 0, 0);
    gui_set_fixed_twostate_menuitem_current_theme(m);
    m->OFF_icon = OFF_icon;
    m->ON_icon = ON_icon;
    m->icon_x = 0;
    m->icon_y = 0;
    /* Autocalculate the icon position              */
    if (m->OFF_icon != UI_NULL_IMAGE)
    {
        S32 icon_width, icon_height;

        gui_measure_image(m->OFF_icon, &icon_width, &icon_height);
        if (m->flags & UI_MENUITEM_CENTER_ICON_X)
        {
            m->icon_x = (m->width >> 1) - (icon_width >> 1);
        }
        if (m->flags & UI_MENUITEM_CENTER_ICON_Y)
        {
            m->icon_y = (m->height >> 1) - (icon_height >> 1);
        }
    }
    m->scroll_x = 0;
    m->scroll_width = width;
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_twostate_menuitem_set_text_position
 * DESCRIPTION
 *  Sets the text position within a fixed twostate menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed twostate menuitem object  (common data)
 *  x       [IN]        Is the position of the text (offset from the left-top corner of the menuitem)
 *  y       [IN]        Is the position of the text (offset from the left-top corner of the menuitem)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_twostate_menuitem_set_text_position(fixed_twostate_menuitem *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->text_x = x;
    m->text_y = y;
    if (m->icon_x > m->text_x)
    {
        m->scroll_width = m->icon_x - m->text_x;
    }
    else
    {
        m->scroll_width = m->width - m->text_x;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_twostate_menuitem_set_icon_position
 * DESCRIPTION
 *  Sets the icon position within a fixed twostate menuitem
 * PARAMETERS
 *  m       [IN]        Is the fixed twostate menuitem object  (common data)
 *  x       [IN]        Is the position of the icon (offset from the left-top corner of the menuitem)
 *  y       [IN]        Is the position of the icon (offset from the left-top corner of the menuitem)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_twostate_menuitem_set_icon_position(fixed_twostate_menuitem *m, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->icon_x = x;
    m->icon_x = y;
    if (m->icon_x > m->text_x)
    {
        m->scroll_width = m->icon_x - m->text_x;
    }
    else
    {
        m->scroll_width = m->width - m->text_x;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_twostate_menuitem_set_icons
 * DESCRIPTION
 *  sets the two state icons
 * PARAMETERS
 *  m               [IN]        Is the fixed twostate menuitem object (common_data)
 *  ON_icon         [IN]        Is the icon that is displayed when the item is selected
 *  OFF_icon        [IN]        Is the icon that is displayed when the item is not selected
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_twostate_menuitem_set_icons(fixed_twostate_menuitem *m, PU8 ON_icon, PU8 OFF_icon)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->OFF_icon = OFF_icon;
    m->ON_icon = ON_icon;
    /* Autocalculate the icon position              */
    if (m->OFF_icon != UI_NULL_IMAGE)
    {
        S32 icon_width, icon_height;

        gui_measure_image(m->OFF_icon, &icon_width, &icon_height);
        if (m->flags & UI_MENUITEM_CENTER_ICON_X)
        {
            m->icon_x = (m->width >> 1) - (icon_width >> 1);
        }
        if (m->flags & UI_MENUITEM_CENTER_ICON_Y)
        {
            m->icon_y = (m->height >> 1) - (icon_height >> 1);
        }
    }
    if (m->icon_x > m->text_x)
    {
        m->scroll_width = m->icon_x - m->text_x;
    }
    else
    {
        m->scroll_width = m->width - m->text_x;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_fixed_twostate_menuitem
 * DESCRIPTION
 *  Changes the size of a fixed twostate menuitem
 * PARAMETERS
 *  m           [IN]        Is the fixed twostate menuitem object  (common data)
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_fixed_twostate_menuitem(fixed_twostate_menuitem *m, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->width = width;
    m->height = height;
    if (m->icon_x > m->text_x)
    {
        m->scroll_width = m->icon_x - m->text_x;
    }
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    else
    {
        m->scroll_width = m->width - (m->text_x + TEXT_ICON_GAP);       /* As TEXT_ICON_GAP is add in text x to start show text */
    }
#else /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    else
    {
        m->scroll_width = m->width - m->text_x;
    }
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
     
}

/* Text scrolling related functions within the menu item */

fixed_twostate_menuitem *GUI_current_fixed_twostate_menuitem_common_data = NULL;
fixed_twostate_menuitem_type *GUI_current_fixed_twostate_menuitem_data = NULL;
U8 GUI_current_fixed_twostate_menuitem_scroll = 0;
U8 GUI_current_fixed_twostate_menuitem_scroll_direction;
S32 GUI_current_fixed_twostate_menuitem_x;
S32 GUI_current_fixed_twostate_menuitem_y;
S32 GUI_current_fixed_twostate_menuitem_scroll_counter = 0;
UI_string_type GUI_current_fixed_twostate_menuitem_scroll_text = NULL;
UI_string_type GUI_current_fixed_twostate_menuitem_scroll_text_end = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_twostate_menuitem_scroll_handler
 * DESCRIPTION
 *  Scoll the menu item text
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_twostate_menuitem_scroll_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 flags = GUI_current_fixed_twostate_menuitem_common_data->flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags |= GUI_current_fixed_twostate_menuitem_data->flags;

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_reset_current_list_effect();
#endif 

    if (GUI_current_fixed_twostate_menuitem_scroll_counter)
    {
        GUI_current_fixed_twostate_menuitem_scroll_counter--;
    }
    else
    {
    #if(UI_USE_WORD_SCROLLING_TEXT)
        GUI_current_fixed_twostate_menuitem_scroll_text = GUI_current_fixed_twostate_menuitem_scroll_text_end;
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        if (flags & UI_MENUITEM_MARQUEE_SCROLL)
        {
            if (r2lMMIFlag)
            {
                GUI_current_fixed_twostate_menuitem_common_data->scroll_x += UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_twostate_menuitem_common_data->scroll_x >=
                    (GUI_current_fixed_twostate_menuitem_common_data->text_width + UI_text_menuitem_scroll_gap))
                {
                    GUI_current_fixed_twostate_menuitem_common_data->scroll_x = 0;
                }
            }
            else
            {
                GUI_current_fixed_twostate_menuitem_common_data->scroll_x -= UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_twostate_menuitem_common_data->scroll_x <=
                    (-(GUI_current_fixed_twostate_menuitem_common_data->text_width + UI_text_menuitem_scroll_gap)))
                {
                    GUI_current_fixed_twostate_menuitem_common_data->scroll_x = 0;
                }
            }
        }
        else
        {
            if (GUI_current_fixed_twostate_menuitem_scroll_direction == 0)
            {
                GUI_current_fixed_twostate_menuitem_common_data->scroll_x -= UI_text_menuitem_scroll_size;
                if ((GUI_current_fixed_twostate_menuitem_common_data->scroll_x +
                     GUI_current_fixed_twostate_menuitem_common_data->text_width) <=
                    (GUI_current_fixed_twostate_menuitem_common_data->scroll_width))
                {
                    GUI_current_fixed_twostate_menuitem_scroll_direction = 1;
                    GUI_current_fixed_twostate_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;
                }
            }
            else if (GUI_current_fixed_twostate_menuitem_scroll_direction == 1)
            {
                GUI_current_fixed_twostate_menuitem_common_data->scroll_x += UI_text_menuitem_scroll_size;
                if (GUI_current_fixed_twostate_menuitem_common_data->scroll_x >= 0)
                {
                    GUI_current_fixed_twostate_menuitem_scroll_direction = 0;
                    GUI_current_fixed_twostate_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;
                }
            }
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        /* 
         * WAP might display two-state fixed list as list box outside of content region.
         * We should disable scrolling text when it locates outside the content region, 
         * otherwise it will overlap with the title bar or softkey.
         */
        if (GUI_current_fixed_twostate_menuitem_y >= MMI_content_y &&
            (GUI_current_fixed_twostate_menuitem_y + GUI_current_fixed_twostate_menuitem_common_data->height - 1)
            < (MMI_content_y + MMI_content_height))
        {
            GUI_current_fixed_twostate_menuitem_common_data->ext_flags |= UI_MENUITEM_EXT_SHOW_IN_MARQUEE;

        #ifdef __MMI_UI_TRANSPARENT_EFFECT__
            if (!gui_is_current_transparency_with_multi_layer())
            {
                gui_fixed_twostate_menuitem_disable_transparent_effect(GUI_current_fixed_twostate_menuitem_common_data);        /* To hide text below scrolling text */
            }
        #endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 

            gui_show_fixed_twostate_menuitem(
                (void*)GUI_current_fixed_twostate_menuitem_data,
                (void*)GUI_current_fixed_twostate_menuitem_common_data,
                GUI_current_fixed_twostate_menuitem_x,
                GUI_current_fixed_twostate_menuitem_y);

        #ifdef __MMI_UI_TRANSPARENT_EFFECT__
            gui_fixed_twostate_menuitem_enable_transparent_effect(GUI_current_fixed_twostate_menuitem_common_data);     /* To enable transparency */
        #endif 

            GUI_current_fixed_twostate_menuitem_common_data->ext_flags &= ~UI_MENUITEM_EXT_SHOW_IN_MARQUEE;
            gui_BLT_double_buffer(
                GUI_current_fixed_twostate_menuitem_x,
                GUI_current_fixed_twostate_menuitem_y,
                GUI_current_fixed_twostate_menuitem_x + GUI_current_fixed_twostate_menuitem_common_data->width - 1,
                GUI_current_fixed_twostate_menuitem_y + GUI_current_fixed_twostate_menuitem_common_data->height - 1);
        }
    }
    /* start timer of scrolling text */
    gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_twostate_menuitem_scroll_handler);
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_twostate_menuitem_start_scroll
 * DESCRIPTION
 *  start the scrolling of menu item text
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_twostate_menuitem_start_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_twostate_menuitem_type *t = (fixed_twostate_menuitem_type*) GUI_current_fixed_twostate_menuitem_data;
    fixed_twostate_menuitem *m = (fixed_twostate_menuitem*) GUI_current_fixed_twostate_menuitem_common_data;
    S32 w;
    U32 flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = m->flags;
    flags |= t->flags;
    gui_set_font(m->text_font);
    w = gui_get_string_width(t->item_text);
    m->text_width = w;
    m->scroll_x = 0;

    if (w >= (m->scroll_width - 1))
    {
    #if(UI_USE_WORD_SCROLLING_TEXT)
        GUI_current_fixed_twostate_menuitem_scroll_text = t->item_text;
        GUI_current_fixed_twostate_menuitem_scroll = 1;
        GUI_current_fixed_twostate_menuitem_scroll_direction = 0;
        GUI_current_fixed_twostate_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        if (gui_get_current_list_effect() && gblock_list_effect != 1)
        {
            gui_register_callback_menuitem_scroll(gui_fixed_twostate_menuitem_scroll_handler);
        }
        else
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
        {
            gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_twostate_menuitem_scroll_handler);
        }
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        if ((flags & UI_MENUITEM_MARQUEE_SCROLL) || (flags & UI_MENUITEM_TWO_DIRECTION_SCROLL))
        {
            GUI_current_fixed_twostate_menuitem_scroll = 1;
            GUI_current_fixed_twostate_menuitem_scroll_direction = 0;
            GUI_current_fixed_twostate_menuitem_scroll_counter = UI_text_menuitem_scroll_pause;

        #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
            if (gui_get_current_list_effect() && gblock_list_effect != 1)
            {
                gui_register_callback_menuitem_scroll(gui_fixed_twostate_menuitem_scroll_handler);
            }
            else
        #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
            {
                gui_start_timer(UI_text_menuitem_scroll_time, gui_fixed_twostate_menuitem_scroll_handler);
            }
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
    }
    else
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    {
        gui_register_callback_menuitem_scroll(gui_dummy_scroll_handler_for_list_effects);
        GUI_current_fixed_twostate_menuitem_scroll = 0;
    }
#else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    {
        GUI_current_fixed_twostate_menuitem_scroll = 0;
    }
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_twostate_menuitem_stop_scroll
 * DESCRIPTION
 *  stop the scrolling of menu item text
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_fixed_twostate_menuitem_stop_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GUI_current_fixed_twostate_menuitem_scroll)
    {
        gui_cancel_timer(gui_fixed_twostate_menuitem_scroll_handler);
        GUI_current_fixed_twostate_menuitem_scroll = 0;
    }
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_register_callback_menuitem_scroll(gui_dummy_scroll_handler_for_list_effects);
    gui_restore_current_list_effect();
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_fixed_twostate_menuitem
 * DESCRIPTION
 *  Displays the fixed twostate menuitem
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed twostate menuitem object
 *  common_item_data        [IN]        Is the fixed twostate menuitem object (common_data)
 *  x                       [IN]        Is the position at which the menuitem is displayed
 *  y                       [IN]        Is the position at which the menuitem is displayed
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_fixed_twostate_menuitem(void *item, void *common_item_data, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 text_x, text_y;
    color_t text_color;
    UI_filled_area *f = NULL;
    fixed_twostate_menuitem *m = (fixed_twostate_menuitem*) common_item_data;
    fixed_twostate_menuitem_type *mi = (fixed_twostate_menuitem_type*) item;
    U32 flags;
    UI_string_type _text;

    /* byte animation_flag=0; */
    S32 sw, sh;

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    PU8 temp_on_icon = m->ON_icon;
    PU8 temp_off_icon = m->OFF_icon;
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
#if(UI_USE_WORD_SCROLLING_TEXT)
    S32 text_width;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = mi->flags;
    flags |= m->flags;
    _text = mi->item_text;

    if (flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        GUI_current_fixed_twostate_menuitem_x = x;
        GUI_current_fixed_twostate_menuitem_y = y;
    }

    /* Autocalculate the text position based on font   */
    gui_set_font(m->text_font); /* set font */
    gui_measure_string(_text, &sw, &sh);
    if (flags & UI_MENUITEM_CENTER_TEXT_X)
    {
        m->text_x = (m->width >> 1) - (sw >> 1);
    }
    if (flags & UI_MENUITEM_CENTER_TEXT_Y)
    {
        m->text_y = (m->height >> 1) - (sh >> 1);
    }

    x1 = x;
    y1 = y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

    if (r2lMMIFlag)
    {
        text_x = x2 - m->text_x;    /* xt_x=m->width-2-m->text_x; */
    }
    else
    {
    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        text_x = m->text_x + TEXT_ICON_GAP; /* To display text grid gap */
    #else 
        text_x = m->text_x;
    #endif 
    }

    text_y = m->text_y;

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    /* To hide the text and icon of the menuitem up to the last frame for flash feel effect */
    if (gblock_list_effect == 0)
    {
        if (gui_get_current_list_effect() == MMI_UI_LIST_HIGHLIGHT_EFFECT_FLASH_FEELING &&
            MMI_current_menu_type == LIST_MENU)
        {
            if (flash_feel_last_frame == 0)
            {
                if (flags & UI_MENUITEM_STATE_FOCUSSED)
                {
                    temp_on_icon = UI_NULL_IMAGE;
                    temp_off_icon = UI_NULL_IMAGE;
                    flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY;
                }
            }
        }
    }
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 

    if (flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        text_x += m->scroll_x;
    }

    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && !(flags & UI_MENUITEM_DISABLE_HIGHLIGHT))
    {
        f = m->focussed_filler;
        text_color = m->focussed_text_color;
        /* if(flags & UI_MENUITEM_FOCUSSED_ANIMATE) animation_flag=1; */
    }
    else if (flags & UI_MENUITEM_STATE_DISABLED)
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND))
        {
            f = m->disabled_filler;
        }
        text_color = m->focussed_text_color;
    }
    else if (flags & UI_MENUITEM_STATE_SELECTED)
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND))
        {
            f = m->selected_filler;
        }
        text_color = m->selected_text_color;
        /* if(flags & UI_MENUITEM_SELECTED_ANIMATE) animation_flag=1; */
    }
    else
    {
        if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND))
        {
            f = m->normal_filler;
        }
        text_color = m->normal_text_color;
    }
    gui_push_clip();
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    if (line_draw)
    {
        if (GUI_current_fixed_twostate_menuitem_scroll)
        {
            gui_set_clip(x1, y1 + 1, x2 - 1, y2);
        }
        else
        {
            gui_set_clip(x1, y1, x2, y2);
        }
    }
#else /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    gui_set_clip(x1, y1, x2 + 2, y2 + 2);
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 

    if (f != NULL)
    {
 	#ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
 	#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        if (is_draw_next_frame_list_highlight_effect_value())
    #endif
        {
            g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_HIGHLIGHT_TWOSTATE_ITEM;
        }
    #endif

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        set_start_position_and_item_parameters_for_list_highlighter_effect(item, common_item_data, x1, y1);
    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw && MMI_current_menu_type == LIST_MENU)
        {
            if (GUI_current_fixed_twostate_menuitem_scroll)
            {
                gui_draw_list_filled_area(x1, y1 + 1, x2 - 1, y2, f);
            }
            else
            {
                gui_draw_list_filled_area(x1, y1 + 1, x2 - 1, y2, f);
            }
        }
    #else /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
        gui_draw_list_filled_area(x1, y1, x2, y2, f);
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    #else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw && MMI_current_menu_type == LIST_MENU)
        {
            if (GUI_current_fixed_twostate_menuitem_scroll)
            {
                gui_draw_filled_area(x1, y1, x2 - 1, y2, f);
            }
            else
            {
                gui_draw_filled_area(x1, y1, x2, y2, f);
            }
        }
    #else /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
        gui_draw_filled_area(x1, y1, x2, y2, f);
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
    }
    if (flags & UI_MENUITEM_STATE_SELECTED)
    {
    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        /* To hide the ON icon of the menuitem up to the last frame for flash feel effect */
        if (temp_on_icon != UI_NULL_IMAGE)
    #else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
        if (m->ON_icon != UI_NULL_IMAGE)
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
        {
            if (!r2lMMIFlag)
            {
                gui_show_transparent_image(x1 + m->icon_x, y1 + m->icon_y, m->ON_icon, m->icon_transparent_color);
            }
            else
            {
                gui_show_transparent_image(
                    x2 - m->icon_x - m->text_x + 5,
                    y1 + m->icon_y,
                    m->ON_icon,
                    m->icon_transparent_color);
            }
        }
    }
    else
    {
    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        /* To hide the ON icon of the menuitem up to the last frame for flash feel effect */
        if (temp_off_icon != UI_NULL_IMAGE)
    #else /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
        if (m->OFF_icon != UI_NULL_IMAGE)
    #endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
        {
            /* show transparent image */
            if (!r2lMMIFlag)
            {
                gui_show_transparent_image(x1 + m->icon_x, y1 + m->icon_y, m->OFF_icon, m->icon_transparent_color);
            }
            else
            {
                gui_show_transparent_image(
                    x2 - m->icon_x - m->text_x + 5,
                    y1 + m->icon_y,
                    m->OFF_icon,
                    m->icon_transparent_color);
            }
        }
    }
    if (!(flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY))
    {
        gui_push_text_clip();
        if (m->icon_x > m->text_x)
        {
            gui_set_text_clip(x1, y1, x2 - (m->width - m->icon_x), y2);
        #if(UI_USE_WORD_SCROLLING_TEXT)
            text_width = x2 - (m->width - m->icon_x) - x1 + 1;
        #endif 
        }
        else
        {
        #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
            gui_set_text_clip(x1 + m->text_x, y1, x2 - 2, y2);
        #else 
            gui_set_text_clip(x1 + m->text_x, y1, x2, y2);
        #endif 
        #if(UI_USE_WORD_SCROLLING_TEXT)
            text_width = x2 - (x1 + m->text_x) + 1;
        #endif 
        }

        if (r2lMMIFlag)
        {
            gui_set_text_clip(x1, y1, x2 - m->text_x, y2);
        }

        if (r2lMMIFlag)
        {
            gui_move_text_cursor(text_x, y1 + text_y);
        }
        else
        {
            gui_move_text_cursor(x1 + text_x, y1 + text_y);
        }

        gui_set_text_color(text_color); /* set text clor */
        gui_set_line_height(sh);
    #if(UI_USE_WORD_SCROLLING_TEXT)
        if ((flags & UI_MENUITEM_STATE_FOCUSSED) && GUI_current_fixed_twostate_menuitem_scroll)
        {
            GUI_current_fixed_twostate_menuitem_scroll_text_end = GUI_current_fixed_twostate_menuitem_scroll_text;
            if (gui_print_truncated_text2
                (x1 + text_x, y1 + text_y, text_width, &GUI_current_fixed_twostate_menuitem_scroll_text_end))
            {
                GUI_current_fixed_twostate_menuitem_scroll_text_end = _text;
            }
        }
        else
        {
            gui_print_truncated_text2(x1 + text_x, y1 + text_y, text_width, &_text);
        }
    #else /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        gui_print_text(_text);  /* print text */
        if ((flags & UI_MENUITEM_STATE_FOCUSSED) && (flags & UI_MENUITEM_MARQUEE_SCROLL) &&
            GUI_current_fixed_twostate_menuitem_scroll)
        {
            if (r2lMMIFlag)
            {
                gui_move_text_cursor(x1 + text_x - (m->text_width + UI_text_menuitem_scroll_gap), y1 + text_y);
            }
            else
            {
                gui_move_text_cursor(x1 + text_x + m->text_width + UI_text_menuitem_scroll_gap, y1 + text_y);
            }
            gui_set_line_height(sh);
            gui_print_text(_text);  /* print text */
        }
    #endif /* (UI_USE_WORD_SCROLLING_TEXT) */ 
        gui_pop_text_clip();

    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw)
        {
            gui_reset_clip();
            if (r2lMMIFlag)
            {
                gui_draw_vertical_line(y1, y2, (x2 - (m->text_x) + ((TEXT_ICON_GAP) >> 1)), current_MMI_theme->list_background_filler->border_color);   /* Assume icon is square in shape add 2 for inline edit items having gap from grid */
            }
            else
            {
                gui_draw_vertical_line(y1, y2, m->text_x - ((TEXT_ICON_GAP) >> 1), current_MMI_theme->list_background_filler->border_color);    /* vertical partion between icon and text add 2 for inline edit items having gap from grid */
            }
        }
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
    }

    gui_pop_clip();

#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    if ((flags & UI_MENUITEM_STATE_FOCUSSED) && !(m->ext_flags & UI_MENUITEM_EXT_SHOW_IN_MARQUEE))
    {
        gui_display_pop_up_description(x, y, m->width, m->height);      /* display popp description */
    }
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_fixed_twostate_menuitem_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [?]         
 *  item_x                  [IN]        
 *  item_y                  [IN]        
 *  pen_event               [IN]        
 *  x                       [IN]        
 *  y                       [IN]        
 *  menuitem_event          [?]         
 *  menuitem_param          [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gui_fixed_twostate_menuitem_translate_pen_event(
        void *item,
        void *common_item_data,
        S32 item_x,
        S32 item_y,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_list_pen_enum *menuitem_event,
        gui_pen_event_param_struct *menuitem_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret = MMI_TRUE, pen_inside = MMI_FALSE;
    S32 iwidth, iheight;

    fixed_twostate_menuitem *m = (fixed_twostate_menuitem*) common_item_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);

    *menuitem_event = GUI_LIST_PEN_NONE;
    GUI_PEN_EVENT_PARAM_SET_VOID(menuitem_param);

    if (m->OFF_icon != UI_NULL_IMAGE)
    {
        gui_measure_image(m->OFF_icon, &iwidth, &iheight);
    }
    else
    {
        iwidth = iheight = 0;
    }

    item_x += m->icon_x;
    item_y += m->icon_y;
    if (PEN_CHECK_BOUND(x, y, item_x, item_y, iwidth, iheight))
    {
        pen_inside = MMI_TRUE;
    }

    if (pen_event == MMI_PEN_EVENT_DOWN)
    {
        if (!pen_inside)
        {
            /* The following pen events will not be directed to twostate menuitem */
            ret = MMI_FALSE;
        }
    }
    else if (pen_event == MMI_PEN_EVENT_UP)
    {
        if (pen_inside)
        {
            /* Outside of the image, it is not controlled by menuitem */
            *menuitem_event = GUI_TWOSTATE_PEN_TOGGLE;
        }
    }

    return ret;
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_measure_fixed_twostate_menuitem
 * DESCRIPTION
 *  Measures a fixed twostate menuitem
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [IN]        Is the fixed twostate menuitem object (common_data)
 *  width                   [OUT]       
 *  height                  [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
void gui_measure_fixed_twostate_menuitem(void *item, void *common_item_data, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_twostate_menuitem *t = (fixed_twostate_menuitem*) common_item_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(item);
    *width = t->width;
    *height = t->height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_highlight_fixed_twostate_menuitem
 * DESCRIPTION
 *  Highlights a fixed twostate menuitem (not visually)
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed twostate menuitem object
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_highlight_fixed_twostate_menuitem(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_twostate_menuitem_type *t = (fixed_twostate_menuitem_type*) item;
    fixed_twostate_menuitem *m = (fixed_twostate_menuitem*) common_item_data;
    U32 flags;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flags = m->flags;
    flags |= t->flags;
    GUI_current_fixed_twostate_menuitem_common_data = m;
    GUI_current_fixed_twostate_menuitem_data = t;
    gui_fixed_twostate_menuitem_stop_scroll();
    gui_fixed_twostate_menuitem_start_scroll();
    if (t->flags & UI_MENUITEM_DISABLE_HIGHLIGHT)
    {
        return;
    }
    t->flags |= UI_MENUITEM_STATE_FOCUSSED;
}


/*****************************************************************************
 * FUNCTION
 *  gui_remove_highlight_fixed_twostate_menuitem
 * DESCRIPTION
 *  Removes the highlight of a fixed twostate menuitem (not visually)
 * PARAMETERS
 *  item                    [IN]        Is a specific fixed twostate menuitem object
 *  common_item_data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_remove_highlight_fixed_twostate_menuitem(void *item, void *common_item_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    fixed_twostate_menuitem_type *t = (fixed_twostate_menuitem_type*) item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(common_item_data);
    t->flags &= ~UI_MENUITEM_STATE_FOCUSSED;
}

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__

#define LIST_EFFECT_TYPE__(A) MMI_UI_LIST_HIGHLIGHT_EFFECT_##A
#define LIST_EFFECT_TYPE_(A)  LIST_EFFECT_TYPE__(A)
#define LIST_EFFECT_TYPE(A)      LIST_EFFECT_TYPE_(A)

/* 122205 MMI_features.h revise Start */
static gui_list_effect_styles goriginal_list_effect = LIST_EFFECT_TYPE_(UI_LIST_EFFECT_TYPE);
static gui_list_effect_styles current_list_effect = LIST_EFFECT_TYPE_(UI_LIST_EFFECT_TYPE);
static gui_list_effect_styles backup_current_list_effect = LIST_EFFECT_TYPE_(UI_LIST_EFFECT_TYPE);

/* 122205 MMI_features.h revise End */
static void *ghighlight_item;
static void *ghighlight_common_item_data;
static S32 xstart_effect, ystart_effect, xend_effect, yend_effect;
static S32 gstartX1, gstartY1;
static UI_filled_area backup_filler;

static S32 list_highlight_effect_current_frame = 0;
static is_draw_next_frame_list_highlight_effect = 0;

extern fixed_list_menu MMI_fixed_list_menu;

void (*gui_call_menuitem_scroll) (void);


/*****************************************************************************
 * FUNCTION
 *  gui_draw_list_highlight_effect_last_frame
 * DESCRIPTION
 *  Display the last frame of list highlight effect.
 * PARAMETERS
 *  void
 *  S32(?)      [IN]        X1,S32 y1,S32 x2,S32 y2,UI_filled_area *f
 * RETURNS
 *  void
 *****************************************************************************/
void gui_draw_list_highlight_effect_last_frame(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!ghighlight_item || !ghighlight_common_item_data)
    {
        MMI_DBG_ASSERT(0);
        return;
    }

    gui_lock_double_buffer();
    MMI_fixed_list_menu.item_display_function(ghighlight_item, ghighlight_common_item_data, gstartX1, gstartY1);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(xstart_effect, ystart_effect, xend_effect, yend_effect);
}


/*****************************************************************************
 * FUNCTION
 *  gui_draw_animated_highlight_filler
 * DESCRIPTION
 *  Draw the animated highlighter filler
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_draw_animated_highlight_filler(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 Max_frames;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    xstart_effect = x1;
    ystart_effect = y1;
    xend_effect = x2;
    yend_effect = y2;
    Max_frames = gui_image_n_frames(get_image(IMG_LIST_HIGHLIGHTER));   /* Read the no of frames in the gif image. */

    if (list_highlight_effect_current_frame == Max_frames)
    {
        gui_push_clip();
        gui_set_clip(x1, y1, x2, y2);
        gdi_image_draw_animation_single_frame(x1, y1, get_image(IMG_LIST_HIGHLIGHTER), 0);
        list_highlight_effect_current_frame++;
        gui_pop_clip();
        /* To call filler for last time. */
        gui_draw_list_highlight_effect_last_frame();
        list_highlight_effect_current_frame = 0;
        gui_scroll_start_handler_for_list_effects();
        return;
    }
    else if (list_highlight_effect_current_frame < Max_frames)
    {
        gui_push_clip();
        gui_set_clip(x1, y1, x2, y2);
        gui_show_animated_image_frame(x1, y1, get_image(IMG_LIST_HIGHLIGHTER), (S16) list_highlight_effect_current_frame);      /* Show frame by frame no. */
        list_highlight_effect_current_frame++;  /* Treat list_highlight_effect_current_frame as frame no. */
        gui_pop_clip();
        /* To call filler repeatedly */
        gui_start_timer(LIST_ANIMATION_HIGHLIGHTER_TIMER, gui_callback_highlighter_text_filler);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_draw_transparent_color_filler
 * DESCRIPTION
 *  Draw the transparent highlighter filler with increasing opacity
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_draw_transparent_color_filler(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    xstart_effect = x1;
    ystart_effect = y1;
    xend_effect = x2;
    yend_effect = y2;

    if (list_highlight_effect_current_frame + TRANSPARENCY_START_OPACITY_VALUE == 0)
    {
        /* Last alpha value is reached. nothing more to do. */
        list_highlight_effect_current_frame = 0;
        gui_scroll_start_handler_for_list_effects();
        return;
    }
    else
    {
        if ((list_highlight_effect_current_frame + TRANSPARENCY_START_OPACITY_VALUE) > MAX_OPACITY_VALUE_FACTOR)        /* For start Transparency from some values greater then 0. */
        {
            gui_push_clip();
            gui_set_clip(x1, y1, x2, y2);
            backup_filler = *f;
            /* To disable Transparent effect flag for flash feeling */
            if (backup_filler.flags & UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR && (wgui_is_wallpaper_on_bottom() == MMI_FALSE))     
            {
                backup_filler.flags &= ~UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
            }
            gui_draw_filled_area(x1, y1, x2, y2, &backup_filler);
            list_highlight_effect_current_frame = -TRANSPARENCY_START_OPACITY_VALUE;
            gui_pop_clip();
            /* To call filler repeatedly */
            gui_start_timer(LIST_TRANSPARENT_HIGHLIGHTER_TIMER, gui_callback_highlighter_text_filler);
        }
        else
        {
            gui_push_clip();
            gui_set_clip(x1, y1, x2, y2);
            backup_filler = *f;
        #if 0
        #else /* 0 */ 
            backup_filler.flags |= UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
            backup_filler.c.alpha =
                (list_highlight_effect_current_frame + TRANSPARENCY_START_OPACITY_VALUE) * MAX_OPACITY_VALUE_FACTOR;
        #endif /* 0 */ 
            gui_draw_filled_area(x1, y1, x2, y2, &backup_filler);
            list_highlight_effect_current_frame += JUMP_IN_HIGHLIGHTERS_EFFECTS_FRAME;
            gui_pop_clip();
            /* To call filler repeatedly */
            gui_start_timer(LIST_TRANSPARENT_HIGHLIGHTER_TIMER, gui_callback_highlighter_text_filler);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_draw_flash_feeling_filler
 * DESCRIPTION
 *  Draw the flash feeling highlighter filler with increasing width
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_draw_flash_feeling_filler(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 f_count;    /* f_count is the gap of texts upper and below positions from the top and bottom of the highlighter and uses as the no of frames */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    f_count = get_menu_item_height() >> 1;
    if (list_highlight_effect_current_frame > f_count)
    {
        list_highlight_effect_current_frame = 0;
        flash_feel_last_frame = 0;
        gui_scroll_start_handler_for_list_effects();
    }
    else if (list_highlight_effect_current_frame == f_count)
    {
        /* To achieve the original hilighter height */
        xstart_effect = x1;
        ystart_effect = y1;
        xend_effect = x2;
        yend_effect = y2;
        backup_filler = *f;
        /* To disable Transparent effect flag for flash feeling */
        if (backup_filler.flags & UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR && (wgui_is_wallpaper_on_bottom() == MMI_FALSE))         
        {
            backup_filler.flags &= ~UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
        }
        gui_draw_filled_area(x1, y1, x2, y2, &backup_filler);
        list_highlight_effect_current_frame++;
        flash_feel_last_frame = 1;
        gui_start_timer(LIST_FLASH_HIGHLIGHTER_TIMER, gui_callback_highlighter_text_filler);
    }
    else
    {
        xstart_effect = x1;
        ystart_effect = y1 + f_count - list_highlight_effect_current_frame;
        xend_effect = x2;
        yend_effect = y2 - f_count + list_highlight_effect_current_frame;
        gui_push_clip();
        gui_set_clip(x1, ystart_effect, x2, yend_effect);
        backup_filler = *f;
        /* To disable Transparent effect flag for flash feeling */
        if (backup_filler.flags & UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR)
        {
            backup_filler.flags &= ~UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
        }
        gui_draw_filled_area(x1, y1, x2, y2, &backup_filler);
        gui_pop_clip();
        list_highlight_effect_current_frame += JUMP_IN_HIGHLIGHTERS_EFFECTS_FRAME;
        if ((list_highlight_effect_current_frame) >= f_count)
        {
            list_highlight_effect_current_frame = f_count;  /* To handle counter if jump more then 1 in list_highlight_effect_current_frame */
        }
        /* To call filler repeatedly */
        gui_start_timer(LIST_FLASH_HIGHLIGHTER_TIMER, gui_callback_highlighter_text_filler);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_get_current_list_effect
 * DESCRIPTION
 *  Return the current list effect
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
S32 gui_get_current_list_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return current_list_effect;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_current_list_effect
 * DESCRIPTION
 *  Set the current list effect to filler recieved by function as argument
 * PARAMETERS
 *  list_style                      [IN]        
 *  wgui_list_effect_styles(?)      [IN]        Effect type
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_current_list_effect(gui_list_effect_styles list_style)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_list_effect = list_style;
    backup_current_list_effect = current_list_effect;
}


/*****************************************************************************
 * FUNCTION
 *  gui_reset_current_list_effect
 * DESCRIPTION
 *  Reset the current list effect to normal filler for some cases
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_reset_current_list_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (current_list_effect != MMI_UI_LIST_HIGHLIGHT_EFFECT_NONE)
    {
        backup_current_list_effect = current_list_effect;
    }
    current_list_effect = MMI_UI_LIST_HIGHLIGHT_EFFECT_NONE;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_current_list_effect_to_default
 * DESCRIPTION
 *  Set the current list effect to defult or the selected filler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_current_list_effect_to_default(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_list_effect = goriginal_list_effect;
    backup_current_list_effect = goriginal_list_effect; /* restore as current list effect is also set to default */
}


/*****************************************************************************
 * FUNCTION
 *  gui_restore_current_list_effect
 * DESCRIPTION
 *  Restore the current list effect from the backup_current_list_effect
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_restore_current_list_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_list_effect = backup_current_list_effect;
}


/*****************************************************************************
 * FUNCTION
 *  gui_block_list_effect
 * DESCRIPTION
 *  set the list effect Block flag off
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_block_list_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gblock_list_effect = 1;
    gui_reset_current_list_effect();
}


/*****************************************************************************
 * FUNCTION
 *  gui_unblock_list_effect
 * DESCRIPTION
 *  set the list effect Block flag on
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_unblock_list_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gblock_list_effect = 0;
    gui_restore_current_list_effect();
}


/*****************************************************************************
 * FUNCTION
 *  gui_draw_list_filled_area
 * DESCRIPTION
 *  This function decide the effect to be called according to current list effect flag
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_draw_list_filled_area(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gblock_list_effect == 1)
    {
        gui_reset_current_list_effect();    /* This is to block the effects in case of inline edit screens. */
    }
    switch (current_list_effect)
    {
        case MMI_UI_LIST_HIGHLIGHT_EFFECT_NONE:
		{
            /* Disable transparent effect if we do not have screen background layer. */
            UI_filled_area tf = *f;

	        if (!gui_is_current_transparency_with_multi_layer())
	        {
                tf.flags &= ~UI_FILLED_AREA_TYPE_TRANSPARENT_COLOR;
            }
            gui_draw_filled_area(x1, y1, x2, y2, &tf);
            break;
		}

        case MMI_UI_LIST_HIGHLIGHT_EFFECT_TRANSPARENCY:
            /* For increasing opacity highlighter */
            gui_draw_transparent_color_filler(x1, y1, x2, y2, f);
            break;

        case MMI_UI_LIST_HIGHLIGHT_EFFECT_FLASH_FEELING:
            /* For flash type of highlighter */
            gui_draw_flash_feeling_filler(x1, y1, x2, y2, f);
            break;

        case MMI_UI_LIST_HIGHLIGHT_EFFECT_ANIMATION:
            /* For showing any animation as a gif file in place of highlighter */
            gui_draw_animated_highlight_filler(x1, y1, x2, y2, f);
            break;

    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_callback_highlighter_text_filler
 * DESCRIPTION
 *  Display the all other things on highlighter then filler.
 * PARAMETERS
 *  void
 *  S32(?)      [IN]        X1,S32 y1,S32 x2,S32 y2,UI_filled_area *f
 * RETURNS
 *  void
 *****************************************************************************/
void gui_callback_highlighter_text_filler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    GDI_HANDLE lcd;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!ghighlight_item || !ghighlight_common_item_data)
    {
        MMI_DBG_ASSERT(0);
        return;
    }
    gdi_lcd_get_active(&lcd);
    gdi_lcd_set_active(MMI_fixed_list_menu.act_lcd_handle);

    gui_lock_double_buffer();
    is_draw_next_frame_list_highlight_effect = 1;
    gdi_layer_push_and_set_active(MMI_fixed_list_menu.act_layer_handle);
    MMI_fixed_list_menu.item_display_function(ghighlight_item, ghighlight_common_item_data, gstartX1, gstartY1);
    gdi_layer_pop_and_restore_active();
    is_draw_next_frame_list_highlight_effect = 0;
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(xstart_effect, ystart_effect, xend_effect, yend_effect);
    gdi_lcd_set_active(lcd);
}


/*****************************************************************************
 * FUNCTION
 *  gui_stop_list_highlight_effect
 * DESCRIPTION
 *  Close the timer associated with highlighter filler.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_stop_list_highlight_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flash_feel_last_frame = 0;
    list_highlight_effect_current_frame = 0;
    gui_cancel_timer(gui_callback_highlighter_text_filler);
}


/*****************************************************************************
 * FUNCTION
 *  gui_register_callback_menuitem_scroll
 * DESCRIPTION
 *  Set the associated scroll handler
 * PARAMETERS
 *  f       [IN]        )(void)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_register_callback_menuitem_scroll(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_call_menuitem_scroll = f;   /* set the  call function */
}


/*****************************************************************************
 * FUNCTION
 *  gui_scroll_start_handler_for_list_effects
 * DESCRIPTION
 *  Set the associated scroll handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_scroll_start_handler_for_list_effects(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gui_call_menuitem_scroll)
    {
        gui_call_menuitem_scroll(); /* call the corresponding scroll handler */
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_dummy_scroll_handler_for_list_effects
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_dummy_scroll_handler_for_list_effects(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* For the cases where text size is not suitable for scroll */
}


/*****************************************************************************
 * FUNCTION
 *  is_draw_next_frame_list_highlight_effect_value
 * DESCRIPTION
 *  Return the is_draw_next_frame_list_highlight_effect value
 * PARAMETERS
 *  void
 * RETURNS
 *  S32 is_draw_next_frame_list_highlight_effect
 *****************************************************************************/
S32 is_draw_next_frame_list_highlight_effect_value(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return is_draw_next_frame_list_highlight_effect;
}


/*****************************************************************************
 * FUNCTION
 *  set_start_position_and_item_parameters_for_list_highlighter_effect
 * DESCRIPTION
 *  Set the gstartX1 and gstartY1
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [?]         
 *  x                       [IN]        S32 y
 *  y                       [IN]        
 * RETURNS
 *  S32 is_draw_next_frame_list_highlight_effect(?)
 *****************************************************************************/
void set_start_position_and_item_parameters_for_list_highlighter_effect(
        void *item,
        void *common_item_data,
        S32 x,
        S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ghighlight_item = item;
    ghighlight_common_item_data = common_item_data;
    gstartX1 = x;
    gstartY1 = y;
}

#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 

#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_set_current_two_line_menuitem_data
 * DESCRIPTION
 *  dummy function to set two line menuitem data
 * PARAMETERS
 *  void
 * RETURNS
 *  string to be displayed(?)
 *****************************************************************************/
void UI_dummy_set_current_two_line_menuitem_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return;
}


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_reset_current_two_line_menuitem_data
 * DESCRIPTION
 *  dummy function to reset two line menuitem data
 * PARAMETERS
 *  void
 * RETURNS
 *  string to be displayed(?)
 *****************************************************************************/
void UI_dummy_reset_current_two_line_menuitem_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return;
}
#endif /* (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__) */ 

#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_show_two_line_menuitem_part
 * DESCRIPTION
 *  dummy function to show two line menuitem part
 * PARAMETERS
 *  void
 * RETURNS
 *  string to be displayed(?)
 *****************************************************************************/
void UI_dummy_show_two_line_menuitem_part(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return;
}


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_move_two_line_menuitem_part
 * DESCRIPTION
 *  dummy function to move two line menuitem part
 * PARAMETERS
 *  x       [IN]        
 *  y       [IN]        
 * RETURNS
 *  string to be displayed(?)
 *****************************************************************************/
void UI_dummy_move_two_line_menuitem_part(S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return;
}


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_resize_two_line_menuitem_part
 * DESCRIPTION
 *  dummy function to resize two line menuitem part
 * PARAMETERS
 *  x       [IN]        
 *  y       [IN]        
 * RETURNS
 *  string to be displayed(?)
 *****************************************************************************/
void UI_dummy_resize_two_line_menuitem_part(S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return;
}


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_get_two_line_menuitem_height
 * DESCRIPTION
 *  dummy function to whether the current menuitem is two line
 * PARAMETERS
 *  void
 * RETURNS
 *  string to be displayed
 *****************************************************************************/
pBOOL UI_dummy_get_two_line_menuitem_height(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_two_line_get_thumbnail_flags
 * DESCRIPTION
 *  dummy function that returns thumbnail flags
 * PARAMETERS
 *  void
 * RETURNS
 *  no thumbnail flag
 *****************************************************************************/
U8 UI_dummy_two_line_get_thumbnail_flags(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return 0;
}

/*----------------------------------------------------------------------------
The various callbakcs for two line menu item are defined below this line.
----------------------------------------------------------------------------*/
U8(*gui_two_line_get_thumbnail_flags) (void) = UI_dummy_two_line_get_thumbnail_flags;
void (*gui_reset_two_line) (void) = UI_dummy_function;
void (*gui_two_line_toggle_thumbnail_direction) (void) = UI_dummy_function;
void (*gui_set_current_two_line_menuitem_data) (void) = UI_dummy_set_current_two_line_menuitem_data;
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 

#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)
void (*gui_reset_current_two_line_menuitem_data) (void) = UI_dummy_set_current_two_line_menuitem_data;
#endif 

#if defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__
void (*gui_show_two_line_menuitem_part) (void) = UI_dummy_show_two_line_menuitem_part;
void (*gui_move_two_line_menuitem_part) (S32 x, S32 y) = UI_dummy_move_two_line_menuitem_part;
void (*gui_resize_two_line_menuitem_part) (S32 x, S32 y) = UI_dummy_resize_two_line_menuitem_part;

pBOOL(*gui_get_two_line_menuitem_height) (void) = UI_dummy_get_two_line_menuitem_height;
#endif /* defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 

// TODO: modify function name
BOOL gscrollimage = 0;
S32 gx=0;
S32 gy=0;

#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)


/*****************************************************************************
 * FUNCTION
 *  scrolling_text_draw_two_line_background
 * DESCRIPTION
 *  Function to display backgrounds for two line hint/select menuitem
 *  with scrolling
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 *  co(?)       [IN]        Ordinates for background draw area
 * RETURNS
 *  void
 *****************************************************************************/

void scrolling_text_draw_two_line_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!gui_is_current_transparency_with_multi_layer())
    {
        gui_fixed_icontext_disable_transparent_effect(&MMI_fixed_icontext_menuitem);    /* To hide text below scrolling text */
    }
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
gscrollimage = 1;
    gui_draw_filled_area(x1, y1, x2, y2, MMI_fixed_icontext_menuitem.focussed_filler);
gscrollimage = 0;
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    gui_fixed_icontext_enable_transparent_effect(&MMI_fixed_icontext_menuitem); /* To hide text below scrolling text */
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  line_scrolling_text_timer_handler
 * DESCRIPTION
 *  called on timer expiry to draw two line hint/select menuitem text
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void line_scrolling_text_timer_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_handle_scrolling_text(&gui_two_line_scroll_text);
}


/*****************************************************************************
 * FUNCTION
 *  two_line_menuitem_cleanup_function
 * DESCRIPTION
 *  clean up function mainly to stop scrolling text on category exit.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void two_line_menuitem_cleanup_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_scrolling_text_stop(&gui_two_line_scroll_text);
    gui_reset_current_two_line_menuitem_data();
#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    gui_reset_two_line();
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_two_line_icontext_menuitem
 * DESCRIPTION
 *  Function to display an two line hint/select menuitem
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [?]         
 *  x                       [IN]        
 *  y                       [IN]        
 *  pointer(?)              [IN]        To menuitem, common data and letft corner co-ordinates
 * RETURNS
 *  void
 *****************************************************************************/

void gui_show_two_line_icontext_menuitem(void *item, void *common_item_data, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /* Locals Start */
    S32 x1, y1, x2, y2;

    S32 text_x1 = 0;
    S32 text_x2 = 0;
    S32 text_y1 = 0;
    S32 text_y2 = 0;

    S32 icon_x = 0;
    S32 icon_y = 0;
    S32 icon_width = 0;
    S32 icon_height = 0;

    S32 image_width = 0;
    S32 image_height = 0;

    S32 sw = 0, sh = 0;
    U8 show_two_line_hints = 1;
/*    U8 thumbnail_flags = 0;*/ /*delete by panxu for unused variable*/
    U32 flags;

    UI_filled_area *f = NULL;
    fixed_icontext_menuitem *m = (fixed_icontext_menuitem*) common_item_data;
    fixed_icontext_menuitem_type *mi = (fixed_icontext_menuitem_type*) item;
    UI_string_type _text = NULL;

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    UI_string_type _hint_text = NULL;
#endif 
    PU8 _icon = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Locals End */
    gui_add_cleanup_hook(two_line_menuitem_cleanup_function); 
    flags = mi->flags;
    flags |= m->flags;
    _text = mi->item_text;
    _icon = mi->item_icon;
    x1 = x;
    y1 = y;
    x2 = x1 + m->width - 1;
    y2 = y1 + (m->height << 1) - 1;
gx = x;
gy = y;
    gui_lock_double_buffer();

#if defined(__MMI_UI_HINTS_IN_MENUITEM__) && defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__)
    if ((_hint_text = gui_get_current_pop_up_description_string()) == NULL ||
        (m->ext_flags & UI_MENUITEM_EXT_SHOW_TWO_LINE_SELECT))
#elif defined (__MMI_UI_HINTS_IN_MENUITEM__)
    if ((_hint_text = gui_get_current_pop_up_description_string()) == NULL)
#endif 
    {
        show_two_line_hints = 0;
    #ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
        thumbnail_flags = gui_two_line_get_thumbnail_flags();
    #endif 
    }

#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    if (r2lMMIFlag && thumbnail_flags)
    {
        gui_two_line_toggle_thumbnail_direction();
        thumbnail_flags = gui_two_line_get_thumbnail_flags();
    }
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_reset_current_list_effect();
#endif 

    /* Draw Filler Start */
    if (!(flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS) && (flags & UI_MENUITEM_STATE_FOCUSSED))
    {
        gui_push_clip();
        gui_set_clip(x1, y1, x2, y2);
        f = m->focussed_filler;

    #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
		/* Currently special list effect is not supported in two-line menu item */
        gui_draw_list_filled_area(x1, y1, x2, y2, f);
    #else
        gui_draw_filled_area(x1, y1, x2, y2, f);
    #endif

        gui_pop_clip();
    }
    /* Draw Filler End */

    if (!show_two_line_hints)
    {
    #ifdef __MMI_UI_TRANSPARENT_EFFECT__
        if (!gui_is_current_transparency_with_multi_layer())
        {
            gui_fixed_icontext_disable_transparent_effect(m);   /* To hide text below scrolling text */
        }
    #endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
    }

    if (_icon != UI_NULL_IMAGE)
    {
        gui_measure_image(_icon, &icon_width, &icon_height);
    }
    else
    {
        icon_width = icon_height = 0;
    }

    /* scenario 0 - No Image, 1 - LEFT Image,  2 - RIGHT Image */
    if (show_two_line_hints
#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
        || (thumbnail_flags == UI_TWO_LINE_MENUITEM_NO_THUMBNAIL)
#endif 
        )
    {
        image_width = image_height = 0;

        if (r2lMMIFlag)
        {
            /* Icon calculation */
            icon_x = x2 - m->text_x + 2;    /* Use text position to compute icon position */
            icon_y = y1;
            /* Text calculation */
            text_x1 = x1;
            text_x2 = x2 - m->text_x;
            text_y1 = y1;
            text_y2 = text_y1 + ((y2 - y1) >> 1);
        }
        else
        {
            /* Icon calculation */
            icon_x = x1 + m->icon_x;
            icon_y = y1;
            /* Text calculation */
            text_x1 = x1 + m->text_x;
            text_x2 = x2;
            text_y1 = y1;
            text_y2 = text_y1 + ((y2 - y1) >> 1);
        }
        if ((flags & UI_MENUITEM_DISABLE_ICON) && (_icon == UI_NULL_IMAGE))
        {
            if (r2lMMIFlag)
            {
                text_x2 = x2 - 2;
            }
            else
            {
                text_x1 = x1 + 2;
            }
        }
    }
#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    else if (thumbnail_flags & UI_TWO_LINE_MENUITEM_LEFT_ALIGN_THUMBNAIL)
    {
        image_width = image_height = (m->height << 1);
        /* Icon calculation */
        icon_x = x2 - m->text_x + 2;
        icon_y = y1;
        /* Text calculation */
        text_x1 = x1 + image_width + 2;
        text_x2 = x2 - (m->text_x + GUI_TWO_LINE_TEXT_ICON_GAP);
        text_y1 = y1;
        text_y2 = text_y1 + ((y2 - y1) >> 1);

        if ((flags & UI_MENUITEM_DISABLE_ICON) && (_icon == UI_NULL_IMAGE))
        {
            text_x2 = x2 - GUI_TWO_LINE_TEXT_ICON_GAP;
        }
    }
    else if (thumbnail_flags & UI_TWO_LINE_MENUITEM_RIGHT_ALIGN_THUMBNAIL)
    {
        image_width = image_height = (m->height << 1);

        /* Icon calculation */
        icon_x = x1 + m->icon_x;

        if ((flags & UI_MENUITEM_DISABLE_ICON) && (_icon == UI_NULL_IMAGE))
        {
            text_x1 = x1 + 2;
        }
        else
        {
            text_x1 = x1 + m->text_x;
        }

        icon_y = y1;
        /* Text calulation */
        text_x2 = x2 - image_width - 2;
        text_y1 = y1;
        text_y2 = text_y1 + ((y2 - y1) >> 1);
    }
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 

    /* Text Start */
    if (_text != NULL && (!(flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY)))
    {
        gui_push_text_clip();
        gui_set_text_clip(text_x1, text_y1, text_x2, text_y2);
        gui_set_font(MMI_fixed_icontext_menuitem.text_font);
        gui_measure_string((UI_string_type) _text, &sw, &sh);
        text_y1 += (m->height >> 1) - (sh >> 1);    /* Center Vertically */
        if (flags & UI_MENUITEM_STATE_FOCUSSED)
        {
   	    #ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
            g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_FOCUSED_TWO_LINE_ICONTEXT;
        #endif
            /* Use another scroll handler */
            gui_fixed_icontext_menuitem_stop_scroll();
            gui_set_text_color(MMI_fixed_icontext_menuitem.focussed_text_color);
        }
        else
        {
            gui_set_text_color(MMI_fixed_icontext_menuitem.normal_text_color);
        }

        if (sw < (text_x2 - text_x1 - 1) || (!(flags & UI_MENUITEM_STATE_FOCUSSED)))
        {
            if (r2lMMIFlag)
            {
                gui_move_text_cursor(text_x2, text_y1);
            }
            else
            {
                gui_move_text_cursor(text_x1, text_y1);
            }
            gui_print_text((UI_string_type) _text);
        }
        else
        {
            gui_create_scrolling_text(
                &gui_two_line_scroll_text,
                text_x1,
                text_y1,
                text_x2 - text_x1,
                sh + 1,
                (UI_string_type) _text,
                line_scrolling_text_timer_handler,
                scrolling_text_draw_two_line_background,
                MMI_fixed_icontext_menuitem.focussed_text_color,
                MMI_fixed_icontext_menuitem.focussed_text_color);
            gui_show_scrolling_text(&gui_two_line_scroll_text);
        }
        gui_pop_text_clip();

    }
    //Text End
    //Icon Start
    if (_icon != UI_NULL_IMAGE)
    {
        gui_push_clip();
        gui_measure_image(_icon, &sw, &sh);
        icon_y += (m->height >> 1) - (sh >> 1); /* Center Vertically */
        gui_set_clip(icon_x, icon_y, icon_x + sw, icon_y + sh);
        /* gui_show_transparent_animated_image_frame(icon_x,icon_y,_icon,m->icon_transparent_color,0); */
        gdi_image_draw_animation_single_frame(icon_x, icon_y, (U8*) _icon, 0);
        gui_pop_clip();
    }
    /* Icon End */
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    if (show_two_line_hints)
    {
        S32 hint_x1 = text_x1;  /* Use the same X position as item text */
        S32 hint_y1 = y + m->height;
        S32 hint_x2 = text_x2;
        S32 hint_y2 = hint_y1 + m->height - 1;

    #ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
        g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_HIGHLIGHT_TWO_LINE_HINTS;
    #endif
        gui_show_two_line_hint_part(hint_x1, hint_x2, hint_y1, hint_y2, _hint_text, flags);
        gui_unlock_double_buffer();
        return;
    }
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 
#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    /* Two Line Start */
    {
        gui_move_two_line_menuitem_part(x1 + 2, y1 + m->height);
        gui_resize_two_line_menuitem_part(x2 - x1 - 4, m->height);
        gui_show_two_line_menuitem_part();
        gui_unlock_double_buffer();
        return;
    }
    /* Two Line End */
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  gui_two_line_scroller_stop
 * DESCRIPTION
 *  Function to stop two line scroll timer
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_two_line_scroller_stop(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_scrolling_text_stop(&gui_two_line_scroll_text);
}
#endif /* (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__) */ 

