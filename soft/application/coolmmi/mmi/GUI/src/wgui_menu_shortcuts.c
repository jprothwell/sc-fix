/*******************************************************************************
*  Modification Notice:
*  --------------------------
*  This software is modified by COOLSAND Inc. and the information contained
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
*******************************************************************************/

 /*******************************************************************************
 * Filename:
 * ---------
 *   wgui_menu_shortcuts.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  shortcuts related funtions
 * Author:
 * -------
 * -------
 *------------------------------------------------------------------------------
 * $Log$
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * using 5 pixels as gap
 * 
 * 
 * 
 * 
 * Fill with white color before draw title image because the image may be transparent.
 ****************************************************************************/

/**********************************************************************************
   Filename:      wgui_menu_shortcuts.c
   Date Created:  August-07-2002
   Contains:      UI wrapper routines for Menu shortcuts
**********************************************************************************/

#include "gui_themes.h"
#include "wgui_menu_shortcuts.h"
#include "wgui_categories_util.h"

#ifdef __MMI_UI_TECHNO_STATUS_ICON__
#include "mainmenudef.h"
#endif 

extern BOOL r2lMMIFlag;

/* Menu shortcut wrappers              */

/* 
 * Global Variable
 */
UI_shortcut_box_theme *current_shortcut_box_theme = NULL;
U8 menu_shortcut_handler_display = 0;
U8 disable_menu_shortcut_box_display = 0;
single_line_input_box MMI_menu_shortcut_box;

extern U16 title_bg_id;


/*****************************************************************************
 * FUNCTION
 *  dummy_menu_goto_item
 * DESCRIPTION
 *  Dummy function used in MMI
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void dummy_menu_goto_item(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(index);
}


/*****************************************************************************
 * FUNCTION
 *  dummy_hide_menu_shortcut
 * DESCRIPTION
 *  Dummy function used in MMI
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dummy_hide_menu_shortcut(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

/* 
 * Local Variable
 */
static UI_character_type MMI_list_shortcut_buffer[MMI_MENU_SHORTCUT_BUFFER_LENGTH];
static S32 MMI_menu_shortcut_timeout = MMI_MENU_SHORTCUT_TIMEOUT;
static U8 menu_shortcut_length = 3;

/* Before BLT in menu_shortcut_handler_timeout() */
static void (*menu_shortcut_goto_item) (S32 index) = dummy_menu_goto_item;

/* After BLT in menu_shortcut_handler_timeout() */
static void (*menu_shortcut_item_selected) (S32 index) = dummy_menu_goto_item;

/* Callback to hide shortcut box */
static void (*hide_menu_shortcut) (void) = dummy_hide_menu_shortcut;
static S32 menu_shortcut_item_highlighted = 0;

/* 
 * Define
 */
#ifdef __MMI_MAINLCD_240X320__
#define MMI_SHORTCUT_TEXT_BORDER_X_GAP 5
#else 
#define MMI_SHORTCUT_TEXT_BORDER_X_GAP 2
#endif 


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_handler_timeout
 * DESCRIPTION
 *  This function is called when the menu shortcut handler times out
 *  
 *  Internal function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void menu_shortcut_handler_timeout(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_display = 0;
    gui_single_line_input_box_clear(&MMI_menu_shortcut_box);
if(menu_shortcut_item_highlighted<= MMI_fixed_matrix_menu.n_items)
{
    gdi_layer_lock_frame_buffer();
	
    hide_menu_shortcut();
    #if 0
    gdi_layer_blt_previous(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1);
#endif
    menu_shortcut_goto_item(menu_shortcut_item_highlighted);
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
    if (is_enable_animated_title_effect())
    {
        wgui_start_animate_title();
    }
#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 

    menu_shortcut_item_selected(menu_shortcut_item_highlighted);
    gdi_layer_unlock_frame_buffer();
    /* BLT together with the previous BLT region */
    gdi_layer_blt_previous(
        0,
        0,
        UI_device_width- 1,
        UI_device_height- 1);
}
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_handler_keyinput
 * DESCRIPTION
 *  This function is called when a menu shortcut key is pressed
 *  
 *  Internal function
 * PARAMETERS
 *  c       [IN]        The character that is input
 * RETURNS
 *  void
 *****************************************************************************/
void menu_shortcut_handler_keyinput(UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x, y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
    if (is_enable_animated_title_effect())
    {
        wgui_stop_animate_title();
    }
#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 

    if (MMI_menu_shortcut_number <= 0)
    {
        return;
    }

    if (gui_single_line_input_box_test_overflow(&MMI_menu_shortcut_box))
    {
        menu_shortcut_handler_timeout();
    }
    else if (menu_shortcut_handler_display == 0 && c == '0' &&
             gui_single_line_input_box_test_first_position(&MMI_menu_shortcut_box))
    {
        return;
    }
    else
    {
        S32 i;

        if (menu_shortcut_handler_display == 0)
        {
            menu_shortcut_handler_display = 1;
            gui_single_line_input_box_clear(&MMI_menu_shortcut_box);
        }

        gdi_layer_lock_frame_buffer();
        gui_single_line_input_box_insert_character(&MMI_menu_shortcut_box, c);
        i = gui_atoi((UI_string_type) MMI_menu_shortcut_box.text);
        menu_shortcut_item_highlighted = i - 1;
        gui_start_timer(MMI_menu_shortcut_timeout, menu_shortcut_handler_timeout);

        if (!disable_menu_shortcut_box_display && MMI_title_icon2 == NULL)
        {
            gui_hide_menu_shortcut();
            MMI_menu_shortcut_box.normal_text_color = current_shortcut_box_theme->active_text_color;
            MMI_menu_shortcut_box.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_BACKGROUND;

        #ifdef __MMI_UI_TECHNO_STATUS_ICON__
            gui_set_clip(
                MMI_menu_shortcut_box.x,
                MMI_menu_shortcut_box.y,
                MMI_menu_shortcut_box.x + MMI_menu_shortcut_box.width - 1,
                MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1);
            if (GetActiveScreenId() == MAIN_MENU_SCREENID)
            {
                gui_fill_rectangle(
                    MMI_menu_shortcut_box.x,
                    MMI_menu_shortcut_box.y,
                    MMI_menu_shortcut_box.x + MMI_menu_shortcut_box.width - 1,
                    MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1,
                    (current_UI_theme->window_title_theme->active_filler->c));
                MMI_menu_shortcut_box.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_BACKGROUND;
            }
        #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 

        #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
            /* As area is refill on dalmatian so need to draw shortcut */
            gui_set_clip(
                MMI_menu_shortcut_box.x,
                MMI_menu_shortcut_box.y,
                MMI_menu_shortcut_box.x + MMI_menu_shortcut_box.width - 1,
                MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1);
            gui_draw_filled_area(
                MMI_title_x,
                MMI_title_y,
                MMI_title_x + MMI_title_width - 1,
                MMI_title_y + MMI_title_height - 1,
                current_UI_theme->window_title_theme->active_filler);
        #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
            if (is_enable_animated_title_effect())
            {
                wgui_draw_animate_title_image_frame_0(
                    MMI_title_x,
                    MMI_title_y,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_y + MMI_title_height - 1);
            }
        #endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
        #endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 

            if (r2lMMIFlag)
            {
                x = MMI_menu_shortcut_box.x;
                y = MMI_menu_shortcut_box.y;
                move_menu_shortcut_handler(MMI_title_x, MMI_title_y);
                gui_show_single_line_input_box(&MMI_menu_shortcut_box);
                move_menu_shortcut_handler(MMI_title_width - MMI_menu_shortcut_box.width, MMI_title_y);
            }
            else
            {
                gui_show_single_line_input_box(&MMI_menu_shortcut_box);
            }

            MMI_menu_shortcut_box.flags &= ~UI_SINGLE_LINE_INPUT_BOX_DISABLE_BACKGROUND;

        #ifdef __MMI_UI_TECHNO_STATUS_ICON__
            if (GetActiveScreenId() == MAIN_MENU_SCREENID)
            {
                MMI_menu_shortcut_box.flags &= ~UI_SINGLE_LINE_INPUT_BOX_DISABLE_BACKGROUND;
            }
        #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
        }

        gdi_layer_unlock_frame_buffer();
        gdi_layer_blt_previous(
            MMI_menu_shortcut_box.x,
            MMI_menu_shortcut_box.y,
            MMI_menu_shortcut_box.x + MMI_menu_shortcut_box.width - 1,
            MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1);
    }
}

/*----------------------------------------------------------------------------
   The following functions are internally used and are not user callable.
----------------------------------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_1_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_1_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '1');
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_2_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_2_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '2');
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_3_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_3_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '3');
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_4_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_4_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '4');
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_5_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_5_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '5');
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_6_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_6_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '6');
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_7_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_7_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '7');
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_8_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_8_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '8');
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_9_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_9_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '9');
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_0_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void menu_shortcut_0_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_handler_keyinput((U8) '0');
}


/*****************************************************************************
 * FUNCTION
 *  create_menu_shortcut_handler
 * DESCRIPTION
 *  Creates the menu shortcut handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void create_menu_shortcut_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_MMI_singleline_inputbox_shortcut_indicator_theme();
    gui_create_single_line_input_box_use_buffer(
        &MMI_menu_shortcut_box,
        MMI_title_x + MMI_title_width - 30,
        MMI_title_y,
        36,
        20,
        MMI_list_shortcut_buffer,
        MMI_MENU_SHORTCUT_BUFFER_LENGTH / ENCODING_LENGTH);
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    MMI_menu_shortcut_box.flags |=
        (UI_SINGLE_LINE_INPUT_BOX_CENTER_Y | UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW |
         UI_SINGLE_LINE_INPUT_BOX_CENTER_JUSTIFY | UI_SINGLE_LINE_INPUT_BOX_DISABLE_BACKGROUND);
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
    MMI_menu_shortcut_box.flags |=
        (UI_SINGLE_LINE_INPUT_BOX_CENTER_Y | UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW |
         UI_SINGLE_LINE_INPUT_BOX_CENTER_JUSTIFY);
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
    set_MMI_singleline_inputbox_regular_theme();
}


/*****************************************************************************
 * FUNCTION
 *  menu_shortcut_keyboard_input_handler
 * DESCRIPTION
 *  Keyboard input (simulator) handler for menu shortcuts
 *  
 *  Internal function
 * PARAMETERS
 *  keyc        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void menu_shortcut_keyboard_input_handler(S32 keyc)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (keyc >= '0' && keyc <= '9')
    {
        menu_shortcut_handler_keyinput((U8) keyc);
    }
}


/*****************************************************************************
 * FUNCTION
 *  register_menu_shortcut_keys
 * DESCRIPTION
 *  Registers the keys used by the menu shortcut handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_menu_shortcut_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_keyboard_input_handler(menu_shortcut_keyboard_input_handler);
    SetKeyHandler(menu_shortcut_0_key_down, KEY_0, KEY_EVENT_DOWN);
    SetKeyHandler(menu_shortcut_1_key_down, KEY_1, KEY_EVENT_DOWN);
    SetKeyHandler(menu_shortcut_2_key_down, KEY_2, KEY_EVENT_DOWN);
    SetKeyHandler(menu_shortcut_3_key_down, KEY_3, KEY_EVENT_DOWN);
    SetKeyHandler(menu_shortcut_4_key_down, KEY_4, KEY_EVENT_DOWN);
    SetKeyHandler(menu_shortcut_5_key_down, KEY_5, KEY_EVENT_DOWN);
    SetKeyHandler(menu_shortcut_6_key_down, KEY_6, KEY_EVENT_DOWN);
    SetKeyHandler(menu_shortcut_7_key_down, KEY_7, KEY_EVENT_DOWN);
    SetKeyHandler(menu_shortcut_8_key_down, KEY_8, KEY_EVENT_DOWN);
    SetKeyHandler(menu_shortcut_9_key_down, KEY_9, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  register_menu_shortcut_handler
 * DESCRIPTION
 *  Registers the function to be called when a shortcut
 *  is being entered.
 * PARAMETERS
 *  f               [IN]        Is the function that is called as the user enters each
 *  digit(?)        [IN]        Of the shortcut number
 *  is(?)           [IN]        The index number of the menu item's shortcut (zero based)
 *  This(?)         [IN]        Function is called with the parameter i, which
 * RETURNS
 *  void
 *****************************************************************************/
void register_menu_shortcut_handler(void (*f) (S32 i))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_item_selected = dummy_menu_goto_item;
    menu_shortcut_goto_item = f;
}


/*****************************************************************************
 * FUNCTION
 *  register_menu_shortcut_selected
 * DESCRIPTION
 *  Registers the function to be called when menuitem is selected.
 * PARAMETERS
 *  f           [IN]        Is the function that is called when menuitem is selected.
 *  is(?)       [IN]        The index number of the menu item's shortcut (zero based)
 *  This(?)     [IN]        Function is called with the parameter i, which
 * RETURNS
 *  void
 *****************************************************************************/
void register_menu_shortcut_selected(void (*f) (S32 i))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_item_selected = f;
}


/*****************************************************************************
 * FUNCTION
 *  register_menu_shortcut_selected_only
 * DESCRIPTION
 *  Registers the function to be called to load items on demand.
 * PARAMETERS
 *  f           [IN]        Is the function that is called to load items on demand.
 *  is(?)       [IN]        The index number of the menu item's shortcut (zero based)
 *  This(?)     [IN]        Function is called with the parameter i, which
 * RETURNS
 *  void
 *****************************************************************************/
/* for category184screen to load items on demand */
void register_menu_shortcut_selected_only(void (*f) (S32 i))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_shortcut_item_selected = f;
    menu_shortcut_goto_item = dummy_menu_goto_item;
}


/*****************************************************************************
 * FUNCTION
 *  move_menu_shortcut_handler
 * DESCRIPTION
 *  Moves the menu shortcut indicator to a new position
 * PARAMETERS
 *  x       [IN]        Is the new left-top corner position
 *  y       [IN]        Is the new left-top corner position
 * RETURNS
 *  void
 *****************************************************************************/
void move_menu_shortcut_handler(S32 x, S32 y)
{
#ifdef __MMI_UI_TECHNO_STATUS_ICON__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GetActiveScreenId() == MAIN_MENU_SCREENID)
    {
        MMI_menu_shortcut_box.x = x - MMI_SIGNAL_WIDTH;
    }
    else
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
    {
        MMI_menu_shortcut_box.x = x;
    }
    MMI_menu_shortcut_box.y = y;
}


/*****************************************************************************
 * FUNCTION
 *  resize_menu_shortcut_handler
 * DESCRIPTION
 *  Resizes the menu shortcut indicator
 * PARAMETERS
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void resize_menu_shortcut_handler(S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_resize_single_line_input_box(&MMI_menu_shortcut_box, width, height);
}


/*****************************************************************************
 * FUNCTION
 *  override_menu_shortcut_default_region
 * DESCRIPTION
 *  Setup the background region where menu shortcut is drawn on it.
 * PARAMETERS
 *  x           [IN]        
 *  y           [IN]        
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void override_menu_shortcut_default_region(S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Deprecated function */
}


/*****************************************************************************
 * FUNCTION
 *  register_hide_menu_shortcut
 * DESCRIPTION
 *  Registers a function to be used to hide a menu shortcut
 *  
 *  gui_hide_menu_shortcut() does not really use it.
 *  This is used only for hiding the inputbox when user press digit keys
 * PARAMETERS
 *  f                   [IN]        Is the function that is called when ever the menu shortcut
 *  indicator(?)        [IN]        Needs to be hidden.
 * RETURNS
 *  void
 *****************************************************************************/
void register_hide_menu_shortcut(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    hide_menu_shortcut = f;
}


/*****************************************************************************
 * FUNCTION
 *  set_menu_shortcut_length
 * DESCRIPTION
 *  Sets the maximum number of digits the user can enter for a particular menu's shortcuts.
 * PARAMETERS
 *  l       [IN]        Is the maximum number of digits in the shortcut
 * RETURNS
 *  void
 *****************************************************************************/
void set_menu_shortcut_length(S32 l)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_box.allocated_length = (l + 1) * ENCODING_LENGTH;
    MMI_menu_shortcut_box.available_length = (l + 1) * ENCODING_LENGTH;
    menu_shortcut_length = (U8) l;
}


/*****************************************************************************
 * FUNCTION
 *  reset_menu_shortcut_handler
 * DESCRIPTION
 *  Clears keyboard and keypad handlers registerd with the menu shortcut handler.
 *  Also clears the menu handling and hide functions.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_menu_shortcut_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(menu_shortcut_handler_timeout);
    menu_shortcut_item_selected = dummy_menu_goto_item;
    menu_shortcut_goto_item = dummy_menu_goto_item;
    hide_menu_shortcut = dummy_hide_menu_shortcut;
    menu_shortcut_handler_display = 0;
    gui_single_line_input_box_clear(&MMI_menu_shortcut_box);
    clear_keyboard_input_handler();
    ClearKeyHandler(KEY_0, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_1, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_2, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_3, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_4, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_5, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_6, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_7, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_8, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_9, KEY_EVENT_DOWN);
}

/***************************************************************************** 
 * Category-level functions
 * The design is slightly different than the original design. 
 *****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  set_menu_item_count
 * DESCRIPTION
 *  Sets the number of digits allowed in the menu shortcut
 *  handler. This is based on the number of items in the menu.
 * PARAMETERS
 *  n_items     [IN]        Is the number of menu items in a screen
 * RETURNS
 *  Width in pixels of the rectangle required to
 *  display the menu shortcut number.
 *****************************************************************************/
S32 set_menu_item_count(S32 n_items)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_character_type s[8];
    S32 l, w;
    UI_font_type f = UI_font;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	if(n_items < 10)
		n_items += 10;
    gui_itoa(n_items, (UI_string_type) s, 10);
    l = gui_strlen((UI_string_type) s);
    set_menu_shortcut_length(l);
    gui_set_font(&MMI_medium_font);
    gui_measure_string((UI_string_type) s, &w, &l);
    gui_set_font(f);
    return w;
}


/*****************************************************************************
 * FUNCTION
 *  gui_hide_menu_shortcut
 * DESCRIPTION
 *  Hide the menu shortcut
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_hide_menu_shortcut(void)
{
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_measure_image(get_image(IMG_TITLE_SCROLL), &iwidth, &iheight);
#endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 

    if (MMI_disable_title_shortcut_display)
    {
        return;
    }
    if (MMI_menu_shortcut_number <= 0)
    {
        return;
    }

    gdi_layer_push_clip();
    gdi_layer_lock_frame_buffer();

#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)
    if ((gOnFullScreen & MMI_IN_FULL_SCREEN || gOnFullScreen & MMI_LEAVE_FULL_SCREEN) &&
        !(gOnFullScreen & MMI_LEAVE_FULL_IDLE_SCREEN))
    {
        if (r2lMMIFlag)
        {
            /* In rotation mode, title width < screen width. */
            gui_set_clip_with_bounding_box(
                MMI_title_x,
                MMI_menu_shortcut_box.y,
                MMI_title_x + MMI_menu_shortcut_box.width - 1,
                MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1,
                MMI_title_x,
                MMI_title_y,
                MMI_title_x + MMI_title_width - 1,
                MMI_title_y + MMI_title_height - 1);
            if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
            {
                gdi_draw_solid_rect(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1,
                    GDI_COLOR_TRANSPARENT);
#if !(defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__))	
                if (title_bg_id != 0)
                {
#if !defined(__PROJECT_GALLITE_C01__)
                    gdi_image_draw_id(MMI_title_x, 0, title_bg_id);
#endif
                }
                else
                {
                    gui_draw_filled_area(
                        MMI_title_x,
                        0,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_height - 1,
                        current_UI_theme->window_title_theme->active_filler);
                }
#endif
            }
            else
            {
                gdi_draw_solid_rect(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1,
                    GDI_COLOR_WHITE);
                gui_draw_filled_area(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1,
                    current_UI_theme->window_title_theme->active_filler);
            }
        #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
            if (is_enable_animated_title_effect())
            {
                wgui_draw_animate_title_image_frame_0(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1);
            }
        #endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
        }
        else
        {
            /* In rotation mode, title width < screen width. */
            gui_set_clip_with_bounding_box(
                MMI_menu_shortcut_box.x,
                MMI_menu_shortcut_box.y,
                MMI_menu_shortcut_box.x + MMI_menu_shortcut_box.width - 1,
                MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1,
                MMI_title_x,
                MMI_title_y,
                MMI_title_x + MMI_title_width - 1,
                MMI_title_y + MMI_title_height - 1);
            if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
            {
                gdi_draw_solid_rect(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1,
                    GDI_COLOR_TRANSPARENT);
#if !(defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__))	
                if (title_bg_id != 0)
                {
#if !defined(__PROJECT_GALLITE_C01__)
                    gdi_image_draw_id(MMI_title_x, 0, title_bg_id);
#endif
                }
                else
                {
                    gui_draw_filled_area(
                        MMI_title_x,
                        0,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_height - 1,
                        current_UI_theme->window_title_theme->active_filler);
                }
#endif
            }
            else
            {
                gdi_draw_solid_rect(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1,
                    GDI_COLOR_WHITE);
                gui_draw_filled_area(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1,
                    current_UI_theme->window_title_theme->active_filler);
            }
        #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
            if (is_enable_animated_title_effect())
            {
                wgui_draw_animate_title_image_frame_0(
                    MMI_title_x,
                    0,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_height - 1);
            }
        #endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
        }
    }
    else
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__) */ 
    {
        if (r2lMMIFlag)
        {
            /* In rotation mode, title width < screen width. */
            gui_set_clip_with_bounding_box(
                MMI_title_x,
                MMI_menu_shortcut_box.y,
                MMI_title_x + MMI_menu_shortcut_box.width - 1,
                MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1,
                MMI_title_x,
                MMI_title_y,
                MMI_title_x + MMI_title_width - 1,
                MMI_title_y + MMI_title_height - 1);
            if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
            {
                gdi_draw_solid_rect(
                    MMI_title_x,
                    MMI_title_y,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_y + MMI_title_height - 1,
                    GDI_COLOR_TRANSPARENT);
#if !(defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__))	
                if (title_bg_id != 0)
                {
#if !defined(__PROJECT_GALLITE_C01__)
                    gdi_image_draw_id(MMI_title_x, MMI_title_y, title_bg_id);
#endif
                }
                else
                {
                    gui_draw_filled_area(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1,
                        current_UI_theme->window_title_theme->active_filler);
                }
#endif
            }
            else
            {
                gdi_draw_solid_rect(
                    MMI_title_x,
                    MMI_title_y,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_y + MMI_title_height - 1,
                    GDI_COLOR_WHITE);

            #ifdef __MMI_UI_TECHNO_STATUS_ICON__
                if (GetActiveScreenId() == MAIN_MENU_SCREENID)
                {
                    gui_fill_rectangle(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1,
                        (current_UI_theme->window_title_theme->active_filler->c));
                }
                else
            #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
                {
                    gui_draw_filled_area(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1,
                        current_UI_theme->window_title_theme->active_filler);
                }
            }

        #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
            if (is_enable_animated_title_effect())
            {
                wgui_draw_animate_title_image_frame_0(
                    MMI_title_x,
                    MMI_title_y,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_y + MMI_title_height - 1);
            }
        #endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
        }
        else
        {
            /* In rotation mode, title width < screen width. */
            gui_set_clip_with_bounding_box(
                MMI_menu_shortcut_box.x,
                MMI_menu_shortcut_box.y,
                MMI_menu_shortcut_box.x + MMI_menu_shortcut_box.width - 1,
                MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1,
                MMI_title_x,
                MMI_title_y,
                MMI_title_x + MMI_title_width - 1,
                MMI_title_y + MMI_title_height - 1);
            if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
            {
                gdi_draw_solid_rect(
                    MMI_title_x,
                    MMI_title_y,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_y + MMI_title_height - 1,
                    GDI_COLOR_TRANSPARENT);
#if !(defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__))	
                if (title_bg_id != 0)
                {
#if !defined(__PROJECT_GALLITE_C01__)
                    gdi_image_draw_id(MMI_title_x, MMI_title_y, title_bg_id);
#endif
                }
                else
                {
                    gui_draw_filled_area(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1,
                        current_UI_theme->window_title_theme->active_filler);
                }
#endif
            }
            else
            {
                gdi_draw_solid_rect(
                    MMI_title_x,
                    MMI_title_y,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_y + MMI_title_height - 1,
                    GDI_COLOR_TRANSPARENT);

            #ifdef __MMI_UI_TECHNO_STATUS_ICON__
                if (GetActiveScreenId() == MAIN_MENU_SCREENID)
                {
                    gui_fill_rectangle(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1,
                        (current_UI_theme->window_title_theme->active_filler->c));
                }
                else
            #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
                {
                    gui_draw_filled_area(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1,
                        current_UI_theme->window_title_theme->active_filler);
                }

            #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
                if (is_enable_animated_title_effect())
                {
                    wgui_draw_animate_title_image_frame_0(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1);
                }
            #endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
            }
        }
    }
    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(
        MMI_menu_shortcut_box.x,
        MMI_menu_shortcut_box.y,
        MMI_menu_shortcut_box.x + MMI_menu_shortcut_box.width,
        MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  show_menu_shortcut
 * DESCRIPTION
 *  Displays the shortcut number of the highlighted item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_menu_shortcut(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_character_type s[16];
    stFontAttribute *f = (UI_font_type) (current_MMI_theme->title_text_font);
    S32 l, character_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_disable_title_shortcut_display)
    {
        return;
    }
    if (MMI_menu_shortcut_number <= 0)
    {
        return;
    }

    gdi_layer_push_text_clip();

    gui_itoa(MMI_menu_shortcut_number, (UI_string_type) s, 10);
    gui_set_font(f);
    gdi_layer_set_text_clip(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1);
    gui_set_text_color(current_shortcut_box_theme->normal_text_color);
    if (current_shortcut_box_theme->use_border)
    {
        gui_set_text_border_color(current_shortcut_box_theme->normal_text_border_color);
    }
    gui_measure_string((UI_string_type) s, &l, &character_height);

    if (!r2lMMIFlag)
    {
    #ifdef __MMI_UI_TECHNO_STATUS_ICON__
        if (GetActiveScreenId() == MAIN_MENU_SCREENID)
        {
            gui_move_text_cursor(
                MMI_title_x + MMI_title_width - l - MMI_SHORTCUT_TEXT_BORDER_X_GAP,
                MMI_title_y + ((MMI_title_height) - (character_height + TITLE_TEXT_GAP)));
        }
        else
    #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
        {
            gui_move_text_cursor(
                MMI_title_x + MMI_title_width - l - MMI_SHORTCUT_TEXT_BORDER_X_GAP,
                MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
        }
    }
    else
    {
    #ifdef __MMI_UI_TECHNO_STATUS_ICON__
        if (GetActiveScreenId() == MAIN_MENU_SCREENID)
        {
            gui_move_text_cursor(
                MMI_title_x + MMI_SHORTCUT_TEXT_BORDER_X_GAP + l,
                MMI_title_y + ((MMI_title_height) - (character_height + TITLE_TEXT_GAP)));
        }
        else
    #endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
        {
            gui_move_text_cursor(
                MMI_title_x + MMI_SHORTCUT_TEXT_BORDER_X_GAP + l,
                MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
        }
    }

    gui_set_line_height(character_height);
    if (current_shortcut_box_theme->use_border)
    {
        gui_print_bordered_text(s);
    }
    else
    {
        gui_print_text(s);
    }

    gui_pop_text_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_redraw_menu_shortcut
 * DESCRIPTION
 *  Displays the shortcut number of the highlighted item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_redraw_menu_shortcut(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
    S32 temp_menu_shortcut;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    gdi_layer_lock_frame_buffer();
    gui_hide_menu_shortcut();
     
#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
    if (get_prev_menu_shortcut() != -1)
    {
        temp_menu_shortcut = MMI_menu_shortcut_number;
        MMI_menu_shortcut_number = get_prev_menu_shortcut();
    }
#endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
     
    show_menu_shortcut();
     
#ifdef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
    if (get_prev_menu_shortcut() != -1)
    {
        MMI_menu_shortcut_number = temp_menu_shortcut;
        reset_prev_menu_shortcut();
    }
#endif /* TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT */ 
     
    gdi_layer_unlock_frame_buffer();

    if (r2lMMIFlag)
    {
        gdi_layer_blt_previous(
            MMI_title_x,
            MMI_menu_shortcut_box.y,
            MMI_title_x + MMI_menu_shortcut_box.width - 1,
            MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1);
    }
    else
    {
        gdi_layer_blt_previous(
            MMI_menu_shortcut_box.x,
            MMI_menu_shortcut_box.y,
            MMI_menu_shortcut_box.x + MMI_menu_shortcut_box.width,
            MMI_menu_shortcut_box.y + MMI_menu_shortcut_box.height - 1);
    }
}

