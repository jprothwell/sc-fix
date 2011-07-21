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
 *  wgui_categories_email.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  
 * Author:
 * -------
 *  
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#include "mmi_features.h"
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#undef __NEWSIMULATOR
#include "globaldefs.h"
#include "eventsgprot.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "wgui.h"
#include "wgui_softkeys.h"
#include "wgui_menu_shortcuts.h"
#include "wgui_fixed_menus.h"
#include "wgui_fixed_menuitems.h"
#include "wgui_inputs.h"
#include "wgui_status_icons.h"
#include "wgui_datetime.h"
#include "wgui_categories_defs.h"
#include "wgui_asyncdynamic_menus.h"
#include "wgui_asyncdynamic_menuitems.h"
#include "wgui_draw_manager.h"
#include "gdi_include.h"        /* include for graphic lib */
#include "wgui_touch_screen.h"  /* for touch screen apis */

extern UI_filled_area bluewgui_color_dialog_background;
extern BOOL r2lMMIFlag;

/*****************************Category 251 Start*******************************/
#define _EMAIL_ENABLE_SECOND_STR_SCROLL_  1
#define UI_CATEGORY251_INFOBOX_HEIGHT 32
static U8 *category_251_info_text1;
static U8 *category_251_info_text2;
static scrolling_text scroll_text_cat251;

#if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)
static scrolling_text scroll_text_cat251A;
#endif 

#ifdef __MMI_EMAIL__

/*****************************************************************************
 * FUNCTION
 *  cat251scrolling_timer
 * DESCRIPTION
 *  timer callback of category251 scrolling text
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8s of the history buffer(?)
 *****************************************************************************/
void cat251scrolling_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_handle_scrolling_text(&scroll_text_cat251);
}

#if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)


/*****************************************************************************
 * FUNCTION
 *  cat251scrolling_timerA
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat251scrolling_timerA(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_handle_scrolling_text(&scroll_text_cat251A);
}
#endif /* (_EMAIL_ENABLE_SECOND_STR_SCROLL_) */ 


/*****************************************************************************
 * FUNCTION
 *  cat251_scrolling_text_draw_background
 * DESCRIPTION
 *  draw background callback function of scrolling text of category251
 * PARAMETERS
 *  x1      [IN]        Start x posityion
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void cat251_scrolling_text_draw_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = gui_color(159, 159, 255);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_fill_rectangle(x1, y1, x2, y2, c);
}

#if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)


/*****************************************************************************
 * FUNCTION
 *  cat251_scrolling_text_draw_backgroundA
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void cat251_scrolling_text_draw_backgroundA(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = gui_color(159, 159, 255);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_fill_rectangle(x1, y1, x2, y2, c);
}
#endif /* (_EMAIL_ENABLE_SECOND_STR_SCROLL_) */ 


/*****************************************************************************
 * FUNCTION
 *  DrawCategory251InfoBox
 * DESCRIPTION
 *  Draw the info text box for category 251
 * PARAMETERS
 *  info_string1        [?]         
 *  info_string2        [?]         
 *  string(?)           [IN]        Screen to display.
 *  display(?)          [IN]        Screen to display.
 *  to(?)               [IN]        Screen to display.
 *  first(?)            [IN]        Screen to display.
 *  second(?)           [IN]        Screen to display.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCategory251InfoBox(U8 *info_string1, U8 *info_string2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c;
    stFontAttribute *f = (UI_font_type) (current_MMI_theme->title_text_font);
    S32 fh, w, w1, h, h1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_set_font(current_MMI_theme->title_text_font);

    fh = Get_CharHeightOfAllLang(f->size) * 2 + 5;

    gdi_layer_lock_frame_buffer();
    gdi_layer_reset_clip();
    gui_set_font(f);

    c = gui_color(255, 255, 255);
    gui_fill_rectangle(
        0,
        ((MMI_title_y + MMI_title_height)),
        (UI_device_width - 1),
        ((MMI_title_y + MMI_title_height) + fh - 1),
        c);
    bluewgui_color_dialog_background.flags |= UI_FILLED_AREA_BORDER;
    bluewgui_color_dialog_background.flags |= UI_FILLED_AREA_ROUNDED_BORDER;
    gui_draw_filled_area(
        1,
        ((MMI_title_y + MMI_title_height + 1)),
        (UI_device_width - 2),
        ((MMI_title_y + MMI_title_height) + fh - 2),
        &bluewgui_color_dialog_background);
    c = gui_color(0, 0, 0);

    gdi_layer_set_clip(
        1,
        ((MMI_title_y + MMI_title_height)),
        (UI_device_width - 2),
        ((MMI_title_y + MMI_title_height) + fh - 1));
    gui_set_text_color(c);
    gui_measure_string((UI_string_type) info_string1, &w, &h);
    if (h == 0)
    {
        h = Get_CharHeightOfAllLang(f->size);
    }

#if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)
    gui_measure_string((UI_string_type) info_string2, &w1, &h1);
#endif 

    if (w < UI_device_width - 10)
    {
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(UI_device_width - 4, ((MMI_title_y + MMI_title_height) + 2));
        }
        else
        {
            gui_move_text_cursor(5, ((MMI_title_y + MMI_title_height) + 2));
        }

        gui_print_text((UI_string_type) info_string1);

    #if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)
        if (w1 < UI_device_width - 10)
        {
    #endif /* (_EMAIL_ENABLE_SECOND_STR_SCROLL_) */ 
            if (r2lMMIFlag)
            {
                gui_move_text_cursor(UI_device_width - 4, (MMI_title_y + MMI_title_height + h + 3));
            }
            else
            {
                gui_move_text_cursor(5, (MMI_title_y + MMI_title_height + h + 3));
            }
            gui_print_text((UI_string_type) info_string2);

        #if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)
        }
        else
        {
            flag_scroll = TRUE;
            gui_create_scrolling_text(&scroll_text_cat251A, 5, (MMI_title_height + h + 3), (UI_device_width - 10), h, (UI_string_type) info_string2, cat251scrolling_timerA, cat251_scrolling_text_draw_backgroundA, gui_color(0, 0, 0), gui_color(255, 255, 255));      
            gui_show_scrolling_text(&scroll_text_cat251A);
        }
        #endif /* (_EMAIL_ENABLE_SECOND_STR_SCROLL_) */ 
    }
    else
    {

        flag_scroll = TRUE;
        gui_create_scrolling_text(&scroll_text_cat251, 5, (MMI_title_y + MMI_title_height + 2), (UI_device_width - 10), h, (UI_string_type) info_string1, cat251scrolling_timer, cat251_scrolling_text_draw_background, gui_color(0, 0, 0), gui_color(255, 255, 255));   
        gui_show_scrolling_text(&scroll_text_cat251);

    #if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)

        if (w1 < UI_device_width - 10)
        {
    #endif /* (_EMAIL_ENABLE_SECOND_STR_SCROLL_) */ 
            if (r2lMMIFlag)
            {
                gui_move_text_cursor(UI_device_width - 4, (MMI_title_y + MMI_title_height + h + 3));
            }
            else
            {
                gui_move_text_cursor(5, (MMI_title_y + MMI_title_height + h + 3));
            }
            gui_print_text((UI_string_type) info_string2);

        #if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)
        }
        else
        {
            flag_scroll = TRUE;
            gui_create_scrolling_text(&scroll_text_cat251A, 5, (MMI_title_height + h + 3), (UI_device_width - 10), h1, (UI_string_type) info_string2, cat251scrolling_timerA, cat251_scrolling_text_draw_backgroundA, gui_color(0, 0, 0), gui_color(255, 255, 255));     
            gui_show_scrolling_text(&scroll_text_cat251A);
        }
        #endif /* (_EMAIL_ENABLE_SECOND_STR_SCROLL_) */ 
    }
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory251InfoBox
 * DESCRIPTION
 *  Set the info text box strings to be display and redraw category 251
 * PARAMETERS
 *  infoText1       [?]         
 *  infoText2       [?]         
 *  string(?)       [IN]        Screen to display.
 *  display(?)      [IN]        Screen to display.
 *  to(?)           [IN]        Screen to display.
 *  first(?)        [IN]        Screen to display.
 *  second(?)       [IN]        Screen to display.
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory251InfoBox(U8 *infoText1, U8 *infoText2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    category_251_info_text1 = infoText1;
    gui_scrolling_text_stop(&scroll_text_cat251);
    category_251_info_text2 = infoText2;

#if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)
    gui_scrolling_text_stop(&scroll_text_cat251A);
#endif 

    DrawCategory251InfoBox(category_251_info_text1, category_251_info_text2);
}

/* Dynamic Multi String/Iconic List */
extern S32 gui_asyncdynamic_list_error;

/*----------------------------Async Dynamic Multi Row List-----------------------------*/
// *************************************************************************************//


/*****************************************************************************
 * FUNCTION
 *  ExitCategory261Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory261Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_asyncdynamic_list();
    reset_pop_up_descriptions();
}

// *************************************************************************************//


/*****************************************************************************
 * FUNCTION
 *  ShowCategory261Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [?]         
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  number_of_items         [IN]        
 *  get_item_func           [IN]        
 *  get_hint_func           [IN]        
 *  highlighted_item        [IN]        
 *  list_icon               [IN]        
 *  attach_icon             [IN]        
 *  history_buffer          [?]         
 *  no_of_string            [IN]        
 *  no_of_icons             [IN]        
 *  category_error_flag     [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory261Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        GetAsyncItemFuncPtr get_item_func,
        GetAsyncHintFuncPtr get_hint_func,
        S32 highlighted_item,
        U16 list_icon,
        U16 attach_icon,
        U8 *history_buffer,
        S32 no_of_string,
        S32 no_of_icons,
        S32 *category_error_flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 shortcut_width;
    U8 h_flag;
    PU8 img;
    S32 h, w, h1, w1, text_width;
#if defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_176X220__)|| defined(__MMI_MAINLCD_220X176__)
    S32 font_height_s, font_height_m;
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Reset The Error Variable */
    gui_asyncdynamic_list_error = 0;
    /* Set The Application Side Variable To True */
    *category_error_flag = 1;

    gdi_layer_lock_frame_buffer();

    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    create_fixed_icontext_list_menuitems(no_of_string, no_of_icons);

    img = (U8*) GetImage(list_icon);
    gui_measure_image(img, &w, &h);

    if (w == 0)
    {
        w = 30;
    }
    if (h == 0)
    {
        h = 30;
    }

    img = (U8*) GetImage(attach_icon);
    gui_measure_image(img, &w1, &h1);

    if (w1 == 0)
    {
        w1 = 9;
    }
    if (h1 == 0)
    {
        h1 = 9;
    }

    text_width = UI_device_width - MMI_fixed_list_menu.vbar.width - w - 10;

#if defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_176X220__)|| defined(__MMI_MAINLCD_220X176__)
    //S32 font_height_s, font_height_m;
    gui_set_font(&MMI_small_font);
    font_height_s = gui_get_character_height();
    gui_set_font(&MMI_medium_font);
    /* It may contain non-alphanumeric characters like Chinese */
	font_height_m = Get_CharHeightOfAllLang(MEDIUM_FONT);
    set_fixed_icontext_list_icon_coordinates(0, 0, 0, w, h);
    set_fixed_icontext_list_text_coordinates(0, (3 + w), 0, text_width, font_height_m);
    set_fixed_icontext_list_text_coordinates(1, (3 + w), font_height_m + 1, text_width, font_height_s);
    set_fixed_icontext_list_icon_coordinates(1, (3 + w), font_height_m + font_height_s, w1, h1);
    set_fixed_icontext_list_icon_coordinates(2, 3 + w + w1 + 2, font_height_m + font_height_s, w1, h1);
    set_fixed_icontext_list_icon_coordinates(3, 3 + w + w1 + 2 + w1 + 2, font_height_m + font_height_s, w1, h1);
#else /* __MMI_MAINLCD_240X320__ */ 
    set_fixed_icontext_list_icon_coordinates(0, 0, 0, w, h);
    set_fixed_icontext_list_text_coordinates(0, (3 + w), 0, text_width, 16);
    set_fixed_icontext_list_text_coordinates(1, (3 + w), 16, text_width, 10);
    set_fixed_icontext_list_icon_coordinates(1, (3 + w), 16 + 12, w1, h1);
    set_fixed_icontext_list_icon_coordinates(2, 3 + w + w1 + 2, 16 + 12, w1, h1);
    set_fixed_icontext_list_icon_coordinates(3, 3 + w + w1 + 2 + w1 + 2, 16 + 12, w1, h1);
#endif /* __MMI_MAINLCD_240X320__ */ 
    associate_fixed_multirow_list_list(MMI_MULTIROW_MENUITEM_HEIGHT);

    MMI_current_menu_type = LIST_MENU;

    MMI_fixed_list_menu.n_items = number_of_items;
    if (!init_asyncdynamic_item_buffer(number_of_items, get_item_func, get_hint_func))
    {
        gdi_layer_unlock_frame_buffer();
        return;
    }

    register_asyncdynamic_list_shortcut_handler();
    register_asyncdynamic_list_keys();
    shortcut_width = set_menu_item_count(number_of_items) + 7;
    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
    register_hide_menu_shortcut(draw_title);

    register_fixed_list_highlight_handler(asyncdynamic_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY261_ID, history_buffer);
    asyncdynamic_list_goto_item_no_redraw(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);
    if (gui_asyncdynamic_list_error)
    {
        gdi_layer_unlock_frame_buffer();
        return;
    }

    MMI_fixed_icontext_list_menuitem.flags |= UI_MENUITEM_RIGHT_JUSTIFY;

    MMI_title_string = (UI_string_type) title;
    MMI_title_icon = (PU8) get_image(title_icon);

    resize_asyncdynamic_icontext_menuitems_to_list_width();
    set_asyncdynamic_pop_up_descriptions(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory261Screen;

    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY261_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

    /* Reset The Application Side Flag */
    *category_error_flag = 0;
}

/*----------------------------------AsyncDynamic Iconic List---------------------------*/
// *************************************************************************************//


/*****************************************************************************
 * FUNCTION
 *  ExitCategory262Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory262Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_asyncdynamic_list();
    reset_pop_up_descriptions();
}

//*************************************************************************************//
//Work On Variable w and h to fix the cordinate issue.


/*****************************************************************************
 * FUNCTION
 *  ShowCategory262Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [?]         
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  number_of_items         [IN]        
 *  get_item_func           [IN]        
 *  get_hint_func           [IN]        
 *  highlighted_item        [IN]        
 *  list_icon               [IN]        
 *  history_buffer          [?]         
 *  category_error_flag     [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory262Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        GetAsyncItemFuncPtr get_item_func,
        GetAsyncHintFuncPtr get_hint_func,
        S32 highlighted_item,
        U16 list_icon,
        U8 *history_buffer,
        S32 *category_error_flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 shortcut_width;
    U8 h_flag;
    PU8 img;
    S32 h, w, text_width;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    gui_asyncdynamic_list_error = 0;
    *category_error_flag = 1;
    MMI_menu_shortcut_number = -1;
    MMI_title_string = (UI_string_type) title;
    MMI_title_icon = (PU8) get_image(title_icon);

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    create_fixed_icontext_list_menuitems(1, 1);
    img = (U8*) GetImage(list_icon);
    gdi_image_get_dimension(img, &w, &h);

    if (w == 0)
    {
        w = 15;
    }
    if (h == 0)
    {
        h = 15;
    }
    text_width = UI_device_width - MMI_fixed_list_menu.vbar.width - w - 10;
    set_fixed_icontext_list_icon_coordinates(0, 0, 0, w, h);
    set_fixed_icontext_list_text_coordinates(0, (3 + w), 0, text_width, get_menu_item_height());
    associate_fixed_icontext_list_list();
    MMI_current_menu_type = LIST_MENU;
    MMI_fixed_list_menu.n_items = number_of_items;

    if (!init_asyncdynamic_item_buffer(number_of_items, get_item_func, get_hint_func))
    {
        gdi_layer_unlock_frame_buffer();
        return;
    }

    register_asyncdynamic_list_shortcut_handler();
    register_asyncdynamic_list_keys();

    shortcut_width = set_menu_item_count(number_of_items) + 7;
    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);

    register_hide_menu_shortcut(draw_title);

    register_fixed_list_highlight_handler(asyncdynamic_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY262_ID, history_buffer);
    asyncdynamic_list_goto_item_no_redraw(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);
    if (gui_asyncdynamic_list_error)
    {
        gdi_layer_unlock_frame_buffer();
        return;
    }

    set_asyncdynamic_pop_up_descriptions(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory262Screen;

    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY262_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

    *category_error_flag = 0;
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory263Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory263Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_asyncdynamic_list();
    reset_pop_up_descriptions();
    gui_scrolling_text_stop(&scroll_text_cat251);
#if(_EMAIL_ENABLE_SECOND_STR_SCROLL_)
    gui_scrolling_text_stop(&scroll_text_cat251A);
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  DrawCate263CategoryControlArea
 * DESCRIPTION
 *  This function is used to draw the category controlled area of category263 screen
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate263CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    stFontAttribute *f = (UI_font_type) (current_MMI_theme->title_text_font);
    S32 fh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_set_font(current_MMI_theme->title_text_font);

    fh = Get_CharHeightOfAllLang(f->size) * 2 + 5;

    gui_set_font(f);
    coordinate->S16X = 0;
    coordinate->S16Y = (MMI_title_y + MMI_title_height);
    coordinate->S16Width = UI_device_width;
    coordinate->S16Height = fh;
    DrawCategory251InfoBox(category_251_info_text1, category_251_info_text2);
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory263InfoBox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  infoText1       [?]     
 *  infoText2       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory263InfoBox(U8 *infoText1, U8 *infoText2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory251InfoBox(infoText1, infoText2);
}

/* Eg:  Sent Folder */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory263Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [?]         
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  number_of_items         [IN]        
 *  get_item_func           [IN]        
 *  get_hint_func           [IN]        
 *  highlighted_item        [IN]        
 *  list_icon               [IN]        
 *  history_buffer          [?]         
 *  category_error_flag     [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory263Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        GetAsyncItemFuncPtr get_item_func,
        GetAsyncHintFuncPtr get_hint_func,
        S32 highlighted_item,
        U16 list_icon,
        U8 *history_buffer,
        S32 *category_error_flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 info_box_height;
    S32 shortcut_width, text_width;
    U8 h_flag;
    S32 w = 0, h = 0;
    U8 *img = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    /* Reset The Error Variable */
    gui_asyncdynamic_list_error = 0;
    /* Set The Application Side Variable To True */
    *category_error_flag = 1;

    MMI_menu_shortcut_number = -1;

    MMI_title_string = (UI_string_type) title;
    MMI_title_icon = (PU8) get_image(title_icon);

    info_box_height = UI_CATEGORY251_INFOBOX_HEIGHT;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    create_fixed_icontext_list_menuitems(1, 1);
    img = (U8*) GetImage(list_icon);
    gui_measure_image(img, &w, &h);

    if (w == 0)
    {
        w = 15;
    }
    if (h == 0)
    {
        h = 15;
    }

    text_width = UI_device_width - MMI_fixed_list_menu.vbar.width - w - 10;

    set_fixed_icontext_list_icon_coordinates(0, 0, 0, w, h);
    set_fixed_icontext_list_text_coordinates(0, (3 + w), 0, text_width, get_menu_item_height());

    associate_fixed_icontext_list_list();
    MMI_current_menu_type = LIST_MENU;

    MMI_fixed_list_menu.n_items = number_of_items;
    if (!init_asyncdynamic_item_buffer(number_of_items, get_item_func, get_hint_func))
    {
        gdi_layer_unlock_frame_buffer();
        return;
    }

    register_asyncdynamic_list_shortcut_handler();
    register_asyncdynamic_list_keys();

    shortcut_width = set_menu_item_count(number_of_items) + 7;
    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
    register_hide_menu_shortcut(gui_redraw_menu_shortcut);

    register_fixed_list_highlight_handler(asyncdynamic_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY263_ID, history_buffer);
    asyncdynamic_list_goto_item_no_redraw(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);
    if (gui_asyncdynamic_list_error)
    {
        gdi_layer_unlock_frame_buffer();
        return;
    }

    info_box_height = Get_CharHeightOfAllLang(current_MMI_theme->title_text_font->size) * 2 + 3;

    asyncdynamic_list_goto_item_no_redraw(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);

    if (gui_asyncdynamic_list_error)
    {
        gdi_layer_unlock_frame_buffer();
        return;
    }

    set_asyncdynamic_pop_up_descriptions(h_flag ? MMI_fixed_list_menu.highlighted_item : highlighted_item);

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory263Screen;

    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY263_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawCate263CategoryControlArea);
    dm_redraw_category_screen();

    *category_error_flag = 0;
}
#endif

