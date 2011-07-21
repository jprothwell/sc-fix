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
 * wgui_categories_tab.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   This file is intended for tab control categories screen.
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
#include "mmi_features.h"
#include "gui_config.h"//051106 tab mainmenu
#ifdef __MMI_UI_TAB_PANE__
#include "custthemesres.h"
#include "gui_tab.h"
#include "gui.h"
#include "wgui_tab.h"
#include "wgui_categories.h"
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

/***************************************************************************** 
* Local Function
*****************************************************************************/

/***************************************************************************** 
* Global Variable
*****************************************************************************/
extern FuncPtr backup_exit_func_ptr;

/***************************************************************************** 
* Global Function
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  set_category_default_tab_pane
 * DESCRIPTION
 *  default reset category function for tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void set_category_default_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    move_tab_pane(0, MMI_title_y);
    MMI_title_y += MMI_tab_height;
    override_menu_shortcut_default_region(MMI_title_x, MMI_title_y, MMI_title_width, MMI_title_height);
}


/*****************************************************************************
 * FUNCTION
 *  reset_category_default_tab_pane
 * DESCRIPTION
 *  default reset category function for tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void reset_category_default_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_title_y -= MMI_tab_height;
    override_menu_shortcut_default_region(MMI_title_x, MMI_title_y, MMI_title_width, MMI_title_height);
}


/*****************************************************************************
 * FUNCTION
 *  show_category_two_icon_tab_pane
 * DESCRIPTION
 *  category for two icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void show_category_two_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    enable_tab_pane_display();
    create_and_setup_two_icon_tab_pane();
    set_category_default_tab_pane();
    tab_pane_goto_current_highlighted_item();
    register_tab_pane_keys();
    disable_tab_pane_display();
    set_tab_pane_category_entry_function(set_category_default_tab_pane);
    set_tab_pane_category_exit_function(reset_category_default_tab_pane);
}


/*****************************************************************************
 * FUNCTION
 *  show_category_two_text_tab_pane
 * DESCRIPTION
 *  category for two text tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void show_category_two_text_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    enable_tab_pane_display();
    create_and_setup_two_text_tab_pane();
    set_category_default_tab_pane();
    tab_pane_goto_current_highlighted_item();
    register_tab_pane_keys();
    disable_tab_pane_display();
    set_tab_pane_category_entry_function(set_category_default_tab_pane);
    set_tab_pane_category_exit_function(reset_category_default_tab_pane);
}


/*****************************************************************************
 * FUNCTION
 *  show_category_four_icon_tab_pane
 * DESCRIPTION
 *  category for four icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void show_category_four_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    enable_tab_pane_display();
    create_and_setup_four_icon_tab_pane();
    set_category_default_tab_pane();
    backup_exit_func_ptr = GetCurrExitFuncPtr();
    tab_pane_goto_current_highlighted_item();
    register_tab_pane_keys();
    disable_tab_pane_display();
    set_tab_pane_category_entry_function(set_category_default_tab_pane);
    set_tab_pane_category_exit_function(reset_category_default_tab_pane);
}


/*****************************************************************************
 * FUNCTION
 *  show_category_three_icon_tab_pane
 * DESCRIPTION
 *  category for three icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void show_category_three_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    enable_tab_pane_display();
    create_and_setup_three_icon_tab_pane();
    set_category_default_tab_pane();
    tab_pane_goto_current_highlighted_item();
    register_tab_pane_keys();
    disable_tab_pane_display();
    set_tab_pane_category_entry_function(set_category_default_tab_pane);
    set_tab_pane_category_exit_function(reset_category_default_tab_pane);
}


/*****************************************************************************
 * FUNCTION
 *  set_category_one_icon_tab_pane
 * DESCRIPTION
 *  category for one icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
UI_filled_area *backup_filler;
UI_filled_area new_title_filler;
S32 title_theme_changed = 0;
void set_category_one_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    resize_tab_pane(0, 0);
     
#ifdef __MMI_UI_TITLE_TRANSITION__
     
#if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
    enable_title_transition();
#endif 
     
    set_title_transition_based_on_keys();
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
     
    MMI_content_height -= MMI_MENUITEM_HEIGHT;
    MMI_title_height += MMI_MENUITEM_HEIGHT;
    override_menu_shortcut_default_region(MMI_title_x, MMI_title_y, MMI_title_width, MMI_title_height);
    title_theme_changed = 1;
    backup_filler = current_UI_theme->window_title_theme->active_filler;
    new_title_filler = *current_UI_theme->window_title_theme->active_filler;
    new_title_filler.flags &= ~UI_FILLED_AREA_TYPE_BITMAP;
    new_title_filler.flags |=
        UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_DOUBLE_BORDER | UI_FILLED_AREA_NO_VERTICAL_LINE;
    current_UI_theme->window_title_theme->active_filler = &new_title_filler;
    set_tab_pane_modify_data_function(set_tab_pane_modify_title_icon);
    set_tab_pane_cyclic();
}


/*****************************************************************************
 * FUNCTION
 *  reset_category_one_icon_tab_pane
 * DESCRIPTION
 *  category for one icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void reset_category_one_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_TITLE_TRANSITION__
     
#if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
    disable_title_transition();
#endif 
     
    reset_title_transition();
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
     
    MMI_content_height += MMI_MENUITEM_HEIGHT;
    MMI_title_height -= MMI_MENUITEM_HEIGHT;
    current_UI_theme->window_title_theme->active_filler = backup_filler;
    title_theme_changed = 0;
    reset_tab_pane_modify_data_function();
    override_menu_shortcut_default_region(MMI_title_x, MMI_title_y, MMI_title_width, MMI_title_height);
}


/*****************************************************************************
 * FUNCTION
 *  show_category_one_icon_tab_pane
 * DESCRIPTION
 *  category for one icon tab pane
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        
 *  b(?)        [IN]            
 * RETURNS
 *  void
 *****************************************************************************/
void show_category_one_icon_tab_pane(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    enable_tab_pane_display();
    create_and_setup_one_icon_tab_pane();
    set_category_one_icon_tab_pane();
    tab_pane_goto_current_highlighted_item();
    register_tab_pane_keys();
    disable_tab_pane_display();
    set_tab_pane_category_entry_function(set_category_one_icon_tab_pane);
    set_tab_pane_category_exit_function(reset_category_one_icon_tab_pane);
}


/*****************************************************************************
 * FUNCTION
 *  check_and_set_title_theme_changed
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void check_and_set_title_theme_changed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (title_theme_changed)
    {
        backup_filler = current_UI_theme->window_title_theme->active_filler;
        new_title_filler = *current_UI_theme->window_title_theme->active_filler;
        new_title_filler.flags &= ~UI_FILLED_AREA_TYPE_BITMAP;
        new_title_filler.flags |=
            UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_DOUBLE_BORDER | UI_FILLED_AREA_NO_VERTICAL_LINE;
        current_UI_theme->window_title_theme->active_filler = &new_title_filler;
    }
}

/*
 * void redraw_category_vertical_scroll_menu(void)
 * {
 * #if(UI_DOUBLE_BUFFER_SUPPORT)
 * gui_lock_double_buffer();
 * #endif
 * show_tab_pane();
 * #if(UI_DOUBLE_BUFFER_SUPPORT)
 * gui_unlock_double_buffer();
 * gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
 * #endif
 * }
 * 
 * U8*  get_history_category_vertical_scroll_menu(U8* history_buffer)
 * {
 * get_tab_pane_history(history_buffer);
 * return(history_buffer);
 * }
 * 
 * S32 get_history_size_category_vertical_scroll_menu(void)
 * {
 * return(get_tab_pane_history_size());
 * }
 * 
 * void show_category_vertical_scroll_menu(U16 n_items, U16 *str_list,U16 *img_list,
 * U16 hilite, U8 *buffer)
 * {
 * create_four_icontext_tab_pane();
 * move_tab_pane(0,UI_device_height-MMI_softkey_height-MMI_title_height-27);
 * resize_tab_pane(MMI_title_height+27,UI_device_width);
 * set_four_icontext_pane_theme_data();
 * set_tab_pane(n_items,str_list,img_list,hilite);
 * set_tab_pane_category_history(buffer);
 * set_tab_pane_hightlight_function(standard_tab_pane_highlight_function);
 * tab_pane_disable_function_exection();
 * tab_pane_goto_current_highlighted_item();
 * register_tab_pane_keys();
 * RedrawCategoryFunction=redraw_category_vertical_scroll_menu;
 * GetCategoryHistory=get_history_category_vertical_scroll_menu;
 * GetCategoryHistorySize=get_history_size_category_vertical_scroll_menu;
 * redraw_category_vertical_scroll_menu();
 * }
 */
#endif /* __MMI_UI_TAB_PANE__ */ 

