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
 *  wgui..c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  key_handlers wrappers and initialize UI..
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 */

/**********************************************************************************
   Filename:      wgui.c
   Date Created:  August-07-2002
   Contains:      UI wrapper routines
               Contains several parts and has been split to multiple
               code files. See other files named wgui_*.c and wgui_*.h
               This file contains common functions and data.
**********************************************************************************/

#define __NEWSIMULATOR  /* NVRAM_REVISE */
/* Include: MMI header file */
#include "gui_config.h"
#include "gui_themes.h"
#include "wgui.h"
#include "wgui_softkeys.h"
#include "wgui_menu_shortcuts.h"
#include "wgui_fixed_menus.h"
#include "wgui_fixed_menuitems.h"
#include "wgui_status_icons.h"
#include "wgui_inputs.h"
#include "wgui_virtual_keyboard.h"
#include "idleappdef.h"
#include "wgui_categories.h"
#include "gui_setting.h"
#include "ucs2prot.h"
#include "funandgamesdefs.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"

#if(!UI_DISABLE_EMS_INPUT_BOX)
#include "gui_ems.h"
#endif 
#undef __NEWSIMULATOR   /* NVRAM_REVISE */

/* 
 */
static void (*MMI_keyboard_key_handler) (S32 vkey_code, S32 key_state) = NULL;
static void (*MMI_keyboard_input_handler) (S32 vkey_code) = NULL;
static void (*MMI_keyboard_global_key_handler) (S32 vkey_code, S32 key_state) = NULL;
static void (*MMI_keyboard_global_input_handler) (S32 vkey_code) = NULL;
static void (*MMI_key_input_handler_function) (MMI_key_code_type key_code, MMI_key_event_type key_event);
static void (*MMI_key_down_handler_function) (MMI_key_code_type key_code);
static void (*MMI_key_up_handler_function) (MMI_key_code_type key_code);

/* 
 */
U8 MMI_disable_title_shortcut_display = 0;
S32 MMI_status_bar_height;
S32 MMI_content_x;
S32 MMI_content_y;
S32 MMI_content_width;
S32 MMI_content_height;

extern MMI_theme theme_Default;
extern MMI_theme **MMI_themes;

/* 
 */

extern int ssss(int event);

/*****************************************************************************
 *****************************************************************************/
void MMI_dummy_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

/* Functions for keyboard Support   */


/*****************************************************************************
 * FUNCTION
 *  dummy_keyboard_key_handler
 * DESCRIPTION
 *  Dummy keyboard key handler
 * PARAMETERS
 *  vkey_code       [IN]        Key_code
 *  key_state       [IN]        Key_state
 * RETURNS
 *  void
 *****************************************************************************/
void dummy_keyboard_key_handler(S32 vkey_code, S32 key_state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
}


/*****************************************************************************
 * FUNCTION
 *  dummy_keyboard_input_handler
 * DESCRIPTION
 *  Dummy keyboard input handler
 * PARAMETERS
 *  vkey_code       [IN]        Key_code
 * RETURNS
 *  void
 *****************************************************************************/
void dummy_keyboard_input_handler(S32 vkey_code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(vkey_code);
}


/*****************************************************************************
 * FUNCTION
 *  keyboard_key_handler
 * DESCRIPTION
 *  Handles key events (UP/DOWN)
 * PARAMETERS
 *  vkey_code       [IN]        Key's scan code
 *  key_state       [IN]        Key's state (up or down)
 * RETURNS
 *  void
 *****************************************************************************/
void keyboard_key_handler(S32 vkey_code, S32 key_state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 state = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Perform scan code and key state mapping here */
    if (key_state == KEYBOARD_KEY_DOWN)
    {
        state = 1;
    }
    else if (key_state == KEYBOARD_KEY_UP)
    {
        state = 0;
    }
    if (MMI_keyboard_key_handler != NULL)
    {
        MMI_keyboard_key_handler(vkey_code, state);
    }
}


/*****************************************************************************
 * FUNCTION
 *  keyboard_input_handler
 * DESCRIPTION
 *  Handles keyboard input
 * PARAMETERS
 *  vkey_code       [IN]        Key's character code (ASCII)
 * RETURNS
 *  void
 *****************************************************************************/
void keyboard_input_handler(S32 vkey_code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_keyboard_input_handler != NULL)
    {
        MMI_keyboard_input_handler(vkey_code);
    }
}


/*****************************************************************************
 * FUNCTION
 *  keyboard_global_key_handler
 * DESCRIPTION
 *  Handles key events (UP/DOWN)
 * PARAMETERS
 *  vkey_code       [IN]        Key's scan code
 *  key_state       [IN]        Key's state (up or down)
 * RETURNS
 *  void
 *****************************************************************************/
void keyboard_global_key_handler(S32 vkey_code, S32 key_state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 state = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Perform scan code and key state mapping here */
    if (key_state == KEYBOARD_KEY_DOWN)
    {
        state = 1;
    }
    else if (key_state == KEYBOARD_KEY_UP)
    {
        state = 0;
    }

    if (MMI_keyboard_global_key_handler != NULL)
    {
        MMI_keyboard_global_key_handler(vkey_code, state);
    }
}


/*****************************************************************************
 * FUNCTION
 *  keyboard_global_input_handler
 * DESCRIPTION
 *  Handles keyboard input
 * PARAMETERS
 *  vkey_code       [IN]        Key's character code (ASCII)
 * RETURNS
 *  void
 *****************************************************************************/
void keyboard_global_input_handler(S32 vkey_code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_keyboard_global_input_handler != NULL)
    {
        MMI_keyboard_global_input_handler(vkey_code);
    }
}


/*****************************************************************************
 * FUNCTION
 *  register_keyboard_key_handler
 * DESCRIPTION
 *  Registers a function to be called for key handling
 * PARAMETERS
 *  f       [IN]        Function to be called for key handling
 * RETURNS
 *  void
 *****************************************************************************/
void register_keyboard_key_handler(void (*f) (S32 vkey_code, S32 key_state))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_keyboard_key_handler = f;
}


/*****************************************************************************
 * FUNCTION
 *  register_keyboard_input_handler
 * DESCRIPTION
 *  Registers a function to be called for key input
 * PARAMETERS
 *  f       [IN]        Function that is called upon key events
 * RETURNS
 *  void
 *****************************************************************************/
void register_keyboard_input_handler(void (*f) (S32 vkey_code))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_keyboard_input_handler = f;
}


/*****************************************************************************
 * FUNCTION
 *  register_keyboard_global_key_handler
 * DESCRIPTION
 *  Registers a function to be called for key handling
 * PARAMETERS
 *  f       [IN]        Function that is called upon key events
 * RETURNS
 *  void
 *****************************************************************************/
void register_keyboard_global_key_handler(void (*f) (S32 vkey_code, S32 key_state))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_keyboard_global_key_handler = f;
}


/*****************************************************************************
 * FUNCTION
 *  register_keyboard_global_input_handler
 * DESCRIPTION
 *  Register_keyboard_global_input_handler
 * PARAMETERS
 *  f       [IN]        Function that is called upon key events
 * RETURNS
 *  void
 *****************************************************************************/
void register_keyboard_global_input_handler(void (*f) (S32 vkey_code))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_keyboard_global_input_handler = f;
}


/*****************************************************************************
 * FUNCTION
 *  clear_keyboard_key_handler
 * DESCRIPTION
 *  Clears the function called to handle key events
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_keyboard_key_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_keyboard_key_handler = dummy_keyboard_key_handler;
}


/*****************************************************************************
 * FUNCTION
 *  clear_keyboard_input_handler
 * DESCRIPTION
 *  Clears the function called to handle key input
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_keyboard_input_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_keyboard_input_handler = dummy_keyboard_input_handler;
}


/*****************************************************************************
 * FUNCTION
 *  clear_keyboard_global_key_handler
 * DESCRIPTION
 *  Clears the function called to handle key events
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_keyboard_global_key_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_keyboard_global_key_handler = dummy_keyboard_key_handler;
}


/*****************************************************************************
 * FUNCTION
 *  clear_keyboard_global_input_handler
 * DESCRIPTION
 *  Clears the function called to handle key input
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_keyboard_global_input_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_keyboard_global_input_handler = dummy_keyboard_input_handler;
}

/* Key handling wrappers      */


/*****************************************************************************
 * FUNCTION
 *  clear_key_handlers
 * DESCRIPTION
 *  Clears the key handlers
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
	extern void ClearInputEventHandler(U16 device);
void clear_key_handlers(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    clear_softkey_handler(KEY_LSK);
    clear_softkey_handler(KEY_RSK);
#ifdef __MMI_WGUI_CSK_ENABLE__
    clear_softkey_handler(KEY_CSK);
#endif 
    register_keyboard_key_handler(dummy_keyboard_key_handler);
    register_keyboard_input_handler(dummy_keyboard_input_handler);
    register_keyboard_global_input_handler(dummy_keyboard_input_handler);
    ClearInputEventHandler(MMI_DEVICE_KEY);
}


/*****************************************************************************
 * FUNCTION
 *  clear_category_screen_key_handlers
 * DESCRIPTION
 *  Clears the key handlers used in category screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_category_screen_key_handlers(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    clear_softkey_handler(KEY_LSK);
    clear_softkey_handler(KEY_RSK);
#ifdef __MMI_WGUI_CSK_ENABLE__
    clear_softkey_handler(KEY_CSK);
#endif 
    register_keyboard_key_handler(dummy_keyboard_key_handler);
    register_keyboard_input_handler(dummy_keyboard_input_handler);
}


/*****************************************************************************
 * FUNCTION
 *  register_key_input_handler
 * DESCRIPTION
 *  Registers a key input handler for a keypad key
 * PARAMETERS
 *  f       [IN]        Function that is called on receiving key events
 * RETURNS
 *  void
 *****************************************************************************/
void register_key_input_handler(void (*f) (MMI_key_code_type key_code, MMI_key_event_type key_event))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  register_key_up_handler
 * DESCRIPTION
 *  Registers handler for the key up events of all keys
 * PARAMETERS
 *  f       [IN]        Function that is called whenever any key is released
 * RETURNS
 *  void
 *****************************************************************************/
void register_key_up_handler(void (*f) (MMI_key_code_type key_code))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_up_handler_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  register_key_down_handler
 * DESCRIPTION
 *  Registers handler for the key down events of all keys
 * PARAMETERS
 *  f       [IN]        Function that is called whenever any key is pressed
 * RETURNS
 *  void
 *****************************************************************************/
void register_key_down_handler(void (*f) (MMI_key_code_type key_code))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_down_handler_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  clear_key_input_handler
 * DESCRIPTION
 *  Clears the key input handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_key_input_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler_function = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  clear_key_down_handler
 * DESCRIPTION
 *  Clears the key down handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_key_down_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_down_handler_function = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  clear_key_up_handler
 * DESCRIPTION
 *  Clears the key up handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_key_up_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_up_handler_function = NULL;
}

/*----------------------------------------------------------------------------
   The following functions are used internally and are not meant to be called
   by other functions.
----------------------------------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  MMI_key_input_handler
 * DESCRIPTION
 *  Handler key input events
 * PARAMETERS
 *  key_code        [IN]        
 *  key_event       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_input_handler(MMI_key_code_type key_code, MMI_key_event_type key_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_key_input_handler_function != NULL)
    {
        MMI_key_input_handler_function(key_code, key_event);
    }

    switch (key_event)
    {
        case KEY_EVENT_DOWN:
            if (MMI_key_down_handler_function != NULL)
            {
                MMI_key_down_handler_function(key_code);
            }
            break;
        case KEY_EVENT_UP:
            if (MMI_key_up_handler_function != NULL)
            {
                MMI_key_up_handler_function(key_code);
            }
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_0_down
 * DESCRIPTION
 *  Handle key 0 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_0_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_0, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_1_down
 * DESCRIPTION
 *  Handle key 1 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_1_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_1, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_2_down
 * DESCRIPTION
 *  Handle key 2 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_2_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_2, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_3_down
 * DESCRIPTION
 *  Handle key 3 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_3_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_3, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_4_down
 * DESCRIPTION
 *  Handle key 4 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_4_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_4, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_5_down
 * DESCRIPTION
 *  Handle key 5 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_5_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_5, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_6_down
 * DESCRIPTION
 *  Handle key 6 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_6_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_6, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_7_down
 * DESCRIPTION
 *  Handle key 7 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_7_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_7, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_8_down
 * DESCRIPTION
 *  Handle key 8 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_8_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_8, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_9_down
 * DESCRIPTION
 *  Handle key 9 pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_9_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_9, KEY_EVENT_DOWN);
}

/* multitap for star and Pound */


/*****************************************************************************
 * FUNCTION
 *  MMI_key_star_down
 * DESCRIPTION
 *  Handle key star pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_star_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_STAR, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_pound_down
 * DESCRIPTION
 *  Handle key pound pressed event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_pound_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_POUND, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_0_up
 * DESCRIPTION
 *  Handle key 0 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_0_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_0, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_1_up
 * DESCRIPTION
 *  Handle key 1 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_1_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_1, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_2_up
 * DESCRIPTION
 *  Handle key 2 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_2_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_2, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_3_up
 * DESCRIPTION
 *  Handle key 3 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_3_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_3, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_4_up
 * DESCRIPTION
 *  Handle key 4 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_4_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_4, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_5_up
 * DESCRIPTION
 *  Handle key 5 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_5_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_5, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_6_up
 * DESCRIPTION
 *  Handle key 6 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_6_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_6, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_7_up
 * DESCRIPTION
 *  Handle key 7 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_7_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_7, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_8_up
 * DESCRIPTION
 *  Handle key 8 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_8_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_8, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_9_up
 * DESCRIPTION
 *  Handle key 9 release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_9_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_9, KEY_EVENT_UP);
}

/* multitap for star and pound */


/*****************************************************************************
 * FUNCTION
 *  MMI_key_star_up
 * DESCRIPTION
 *  Handle key star release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_star_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_STAR, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  MMI_key_pound_up
 * DESCRIPTION
 *  Handle key pound release event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MMI_key_pound_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_key_input_handler(KEY_POUND, KEY_EVENT_UP);
}
#ifdef __MMI_FULL_KEYPAD__

void MMI_key_A_down(void)
{
	MMI_key_input_handler(KEY_A, KEY_EVENT_DOWN);
}

void MMI_key_B_down(void)
{
	MMI_key_input_handler(KEY_B, KEY_EVENT_DOWN);
}

void MMI_key_C_down(void)
{
	MMI_key_input_handler(KEY_C, KEY_EVENT_DOWN);
}

void MMI_key_D_down(void)
{
	MMI_key_input_handler(KEY_D, KEY_EVENT_DOWN);
}

void MMI_key_E_down(void)
{
	MMI_key_input_handler(KEY_E, KEY_EVENT_DOWN);
}

void MMI_key_F_down(void)
{
	MMI_key_input_handler(KEY_F, KEY_EVENT_DOWN);
}

void MMI_key_G_down(void)
{
	MMI_key_input_handler(KEY_G, KEY_EVENT_DOWN);
}

void MMI_key_H_down(void)
{
	MMI_key_input_handler(KEY_H, KEY_EVENT_DOWN);
}

void MMI_key_I_down(void)
{
	MMI_key_input_handler(KEY_I, KEY_EVENT_DOWN);
}

void MMI_key_J_down(void)
{
	MMI_key_input_handler(KEY_J, KEY_EVENT_DOWN);
}

void MMI_key_K_down(void)
{
	MMI_key_input_handler(KEY_K, KEY_EVENT_DOWN);
}

void MMI_key_L_down(void)
{
	MMI_key_input_handler(KEY_L, KEY_EVENT_DOWN);
}

void MMI_key_M_down(void)
{
	MMI_key_input_handler(KEY_M, KEY_EVENT_DOWN);
}

void MMI_key_N_down(void)
{
	MMI_key_input_handler(KEY_N, KEY_EVENT_DOWN);
}

void MMI_key_O_down(void)
{
	MMI_key_input_handler(KEY_O, KEY_EVENT_DOWN);
}

void MMI_key_P_down(void)
{
	MMI_key_input_handler(KEY_P, KEY_EVENT_DOWN);
}

void MMI_key_Q_down(void)
{
	MMI_key_input_handler(KEY_Q, KEY_EVENT_DOWN);
}

void MMI_key_R_down(void)
{
	MMI_key_input_handler(KEY_R, KEY_EVENT_DOWN);
}

void MMI_key_S_down(void)
{
	MMI_key_input_handler(KEY_S, KEY_EVENT_DOWN);
}

void MMI_key_T_down(void)
{
	MMI_key_input_handler(KEY_T, KEY_EVENT_DOWN);
}

void MMI_key_U_down(void)
{
	MMI_key_input_handler(KEY_U, KEY_EVENT_DOWN);
}

void MMI_key_V_down(void)
{
	MMI_key_input_handler(KEY_V, KEY_EVENT_DOWN);
}

void MMI_key_W_down(void)
{
	MMI_key_input_handler(KEY_W, KEY_EVENT_DOWN);
}

void MMI_key_X_down(void)
{
	MMI_key_input_handler(KEY_X, KEY_EVENT_DOWN);
}

void MMI_key_Y_down(void)
{
	MMI_key_input_handler(KEY_Y, KEY_EVENT_DOWN);
}

void MMI_key_Z_down(void)
{
	MMI_key_input_handler(KEY_Z, KEY_EVENT_DOWN);
}
void MMI_key_COMMA_down(void)
{
	MMI_key_input_handler(KEY_COMMA, KEY_EVENT_DOWN);
}

void MMI_key_FULLSTOP_down(void)
{
	MMI_key_input_handler(KEY_FULLSTOP, KEY_EVENT_DOWN);
}

void MMI_key_dollar_down(void)
{
	wgui_inputbox_insert_symbol('$');
}

#ifdef __MMI_GB__
extern BOOL EnterSymboFlag;
#endif
void MMI_key_NEWLINE_down(void)
{
	#ifdef __MMI_GB__
	EnterSymboFlag = TRUE;
	#endif
	wgui_inputbox_insert_symbol('\n');
}

void MMI_key_SPACE_down(void)
{
	wgui_inputbox_insert_symbol(' ');
}

void MMI_key_AND_down(void)
{
	wgui_inputbox_insert_symbol('&');
}

void MMI_key_INTEGRR_down(void)
{
	wgui_inputbox_insert_symbol('?');
}

typedef struct
{
	S32 keycode;
	UI_character_type upperChar;
	UI_character_type lowerChar;
	UI_character_type symbol;
}FullKeyPadChar;
FullKeyPadChar fullkeypadcharmap[] = 
#if defined(__MMI_BLACKBERRY_QWERTY__)
{
	{KEY_A,'A','a','*'},
	{KEY_B,'B','b','!'},
	{KEY_C,'C','c','9'},
	{KEY_D,'D','d','5'},
	{KEY_E,'E','e','2'},
	{KEY_F,'F','f','6'},
	{KEY_G,'G','g','/'},
	{KEY_H,'H','h',':'},
	{KEY_I,'I','i','-'},
	{KEY_J,'J','j',';'},
	{KEY_K,'K','k','\''},
	{KEY_L,'L','l','"'},
	{KEY_M,'M','m','.'},
	{KEY_N,'N','n',','},
	{KEY_O,'O','o','+'},
	{KEY_P,'P','p','@'},
	{KEY_Q,'Q','q','#'},
	{KEY_R,'R','r','3'},
	{KEY_S,'S','s','4'},
	{KEY_T,'T','t','('},
	{KEY_U,'U','u','_'},
	{KEY_V,'V','v','?'},
	{KEY_W,'W','w','1'},
	{KEY_X,'X','x','8'},
	{KEY_Y,'Y','y',')'},
	{KEY_Z,'Z','z','7'},
};
#else
{
	{KEY_A, 'A', 'a', 0},
	{KEY_B, 'B', 'b', '9' },
	{KEY_C, 'C', 'c', '7'},
	{KEY_D, 'D', 'd', '4'},
	{KEY_E, 'E', 'e', '1'},
	{KEY_F, 'F', 'f', '5'},
	{KEY_G, 'G', 'g', '6'},
	{KEY_H, 'H', 'h', 0},
	{KEY_I, 'I', 'i', '-'},
	{KEY_J, 'J', 'j', 0},
	{KEY_K, 'K', 'k', '_'},
	{KEY_L, 'L', 'l', '\\'},
	{KEY_M, 'M', 'm', 0},
	{KEY_N, 'N', 'n', '#'},
	{KEY_O, 'O', 'o', '+'},
	{KEY_P, 'P', 'p', '='},
	{KEY_Q, 'Q', 'q', '!'},
	{KEY_R, 'R', 'r', '2'},
	{KEY_S, 'S', 's', 0},
	{KEY_T, 'T', 't', '3'},
	{KEY_U, 'U', 'u', 0},
	{KEY_V, 'V', 'v', '8'},
	{KEY_W, 'W', 'w', '"'},
	{KEY_X, 'X', 'x', '*'},
	{KEY_Y, 'Y', 'y', 0},
	{KEY_Z, 'Z', 'z', 0},
	{KEY_COMMA, ',', ',', ';'},
	{KEY_FULLSTOP, '.', '.', ':'},
	{KEY_0, '/', '@','0'},
};
#endif
#define MAX_CHAR_NUMBER (sizeof(fullkeypadcharmap)/sizeof(fullkeypadcharmap[0]))

U8 MMI_current_char_set = 0;
BOOL isShiftOn = FALSE;

void setCharSet(U8 charset)
{
	MMI_current_char_set = charset;
}

U8 getCharSet(void)
{
	return MMI_current_char_set;
}

void setShiftOn(BOOL onoff)
{
	isShiftOn = onoff;
}

BOOL isShiftKeyOn(void)
{
	return isShiftOn;
}

void MMI_Shift_Key_Pressed(void)
{
	setShiftOn(TRUE);
}

UI_character_type MMI_get_char_by_key_code(S32 key, U8 charset)
{
	S32 i;
	for(i = 0; i < MAX_CHAR_NUMBER; i++)
	{
		if(fullkeypadcharmap[i].keycode == key)
		{
			if(isShiftKeyOn() && fullkeypadcharmap[i].symbol != 0)
			{
				setShiftOn(FALSE);
				return fullkeypadcharmap[i].symbol;
			}
			setShiftOn(FALSE);
			switch(charset)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
					return fullkeypadcharmap[i].upperChar;
				case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
				case INPUT_MODE_MMI_LOWERCASE_ABC:
					return fullkeypadcharmap[i].lowerChar;
				default:
					break;
			}	
		}
	}
	return 0;
}

void MMI_clear_all_input_key_handler(void)
{
	U16 keyCode = 0;
	U16 keyType = 0;
	for (keyCode = KEY_A; keyCode <= KEY_Z; keyCode++)
	{
		for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			ClearKeyHandler(keyCode, keyType);
		}
	}

	for (keyCode = KEY_0; keyCode <= KEY_9; keyCode++)
	{
		for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			ClearKeyHandler(keyCode, keyType);
		}
	}

	for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
	{
		ClearKeyHandler(KEY_STAR, keyType);
	}

	for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
	{
		ClearKeyHandler(KEY_POUND, keyType);
	}
}
#endif
#ifdef __MMI_MULTITAP_THAI__


/*****************************************************************************
 * FUNCTION
 *  register_MMI_key_input_extend_handler
 * DESCRIPTION
 *  Register key handlers for start and pound key.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_MMI_key_input_extend_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(MMI_key_star_down, KEY_STAR, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_star_up, KEY_STAR, KEY_EVENT_UP);

    SetKeyHandler(MMI_key_pound_down, KEY_POUND, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_pound_up, KEY_POUND, KEY_EVENT_UP);
}
#endif /* __MMI_MULTITAP_THAI__ */ 

#ifdef __MMI_BLACKBERRY_QWERTY__
/*****************************************************************************
 * FUNCTION
 *  register_MMI_key_input_handler
 * DESCRIPTION
 *  Register key handlers for all digit keys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_MMI_key_number_input_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(MMI_key_0_down, KEY_0, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_6_down, KEY_6, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_7_down, KEY_7, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_8_down, KEY_8, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_9_down, KEY_9, KEY_EVENT_DOWN);

    SetKeyHandler(MMI_key_0_down, KEY_0, KEY_REPEAT);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_REPEAT);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_REPEAT);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_REPEAT);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_REPEAT);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_REPEAT);
    SetKeyHandler(MMI_key_6_down, KEY_6, KEY_REPEAT);
    SetKeyHandler(MMI_key_7_down, KEY_7, KEY_REPEAT);
    SetKeyHandler(MMI_key_8_down, KEY_8, KEY_REPEAT);
    SetKeyHandler(MMI_key_9_down, KEY_9, KEY_REPEAT);

    SetKeyHandler(MMI_key_0_up, KEY_0, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_1_up, KEY_1, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_2_up, KEY_2, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_3_up, KEY_3, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_4_up, KEY_4, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_5_up, KEY_5, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_6_up, KEY_6, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_7_up, KEY_7, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_8_up, KEY_8, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_9_up, KEY_9, KEY_EVENT_UP);
}
#endif

/*****************************************************************************
 * FUNCTION
 *  register_MMI_key_input_handler
 * DESCRIPTION
 *  Register key handlers for all digit keys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_MMI_key_input_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    #ifndef __MMI_FULL_KEYPAD__
    SetKeyHandler(MMI_key_0_down, KEY_0, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_6_down, KEY_6, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_7_down, KEY_7, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_8_down, KEY_8, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_9_down, KEY_9, KEY_EVENT_DOWN);

    SetKeyHandler(MMI_key_0_down, KEY_0, KEY_REPEAT);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_REPEAT);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_REPEAT);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_REPEAT);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_REPEAT);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_REPEAT);
    SetKeyHandler(MMI_key_6_down, KEY_6, KEY_REPEAT);
    SetKeyHandler(MMI_key_7_down, KEY_7, KEY_REPEAT);
    SetKeyHandler(MMI_key_8_down, KEY_8, KEY_REPEAT);
    SetKeyHandler(MMI_key_9_down, KEY_9, KEY_REPEAT);

    SetKeyHandler(MMI_key_0_up, KEY_0, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_1_up, KEY_1, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_2_up, KEY_2, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_3_up, KEY_3, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_4_up, KEY_4, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_5_up, KEY_5, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_6_up, KEY_6, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_7_up, KEY_7, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_8_up, KEY_8, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_9_up, KEY_9, KEY_EVENT_UP);
    #else
	if( MMI_current_multitap_mode == INPUT_MODE_123)
	{
#if defined(__MMI_BLACKBERRY_QWERTY__)
		SetKeyHandler(MMI_key_0_down, KEY_0, KEY_EVENT_DOWN);// 0
		SetKeyHandler(MMI_key_W_down,KEY_W, KEY_EVENT_DOWN);// 1
		SetKeyHandler(MMI_key_E_down, KEY_E, KEY_EVENT_DOWN);// 2
		SetKeyHandler(MMI_key_R_down, KEY_R, KEY_EVENT_DOWN);// 3
		SetKeyHandler(MMI_key_S_down, KEY_S, KEY_EVENT_DOWN);// 4
		SetKeyHandler(MMI_key_D_down, KEY_D, KEY_EVENT_DOWN);// 5
		SetKeyHandler(MMI_key_F_down, KEY_F, KEY_EVENT_DOWN);// 6
		SetKeyHandler(MMI_key_Z_down, KEY_Z, KEY_EVENT_DOWN);// 7
		SetKeyHandler(MMI_key_X_down, KEY_X, KEY_EVENT_DOWN); // 8
		SetKeyHandler(MMI_key_C_down, KEY_C, KEY_EVENT_DOWN);// 9
		SetKeyHandler(MMI_key_A_down, KEY_A, KEY_EVENT_DOWN);// *
		SetKeyHandler(MMI_key_Q_down, KEY_Q, KEY_EVENT_DOWN); //#
#else
		SetKeyHandler(MMI_key_E_down, KEY_E, KEY_EVENT_DOWN);// 1
		SetKeyHandler(MMI_key_R_down, KEY_R, KEY_EVENT_DOWN);// 2
		SetKeyHandler(MMI_key_T_down, KEY_T, KEY_EVENT_DOWN);// 3
		SetKeyHandler(MMI_key_D_down, KEY_D, KEY_EVENT_DOWN);// 4
		SetKeyHandler(MMI_key_F_down, KEY_F, KEY_EVENT_DOWN);// 5
		SetKeyHandler(MMI_key_G_down, KEY_G, KEY_EVENT_DOWN); //6
		SetKeyHandler(MMI_key_X_down, KEY_X, KEY_EVENT_DOWN); // *
		SetKeyHandler(MMI_key_C_down, KEY_C, KEY_EVENT_DOWN);// 7
		SetKeyHandler(MMI_key_V_down, KEY_V, KEY_EVENT_DOWN);// 8
		SetKeyHandler(MMI_key_B_down, KEY_B, KEY_EVENT_DOWN);// 9
		SetKeyHandler(MMI_key_N_down, KEY_N, KEY_EVENT_DOWN);// #
		SetKeyHandler(MMI_key_0_down, KEY_0, KEY_EVENT_DOWN);// 0
		//SetKeyHandler(GbKeyBackSpaceHandler, KEY_BACKSPACE1, KEY_EVENT_DOWN);// back space
#endif			
	}
	else
	{
		SetKeyHandler(MMI_key_A_down, KEY_A, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_B_down, KEY_B, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_C_down, KEY_C, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_D_down, KEY_D, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_E_down, KEY_E, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_F_down, KEY_F, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_G_down, KEY_G, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_H_down, KEY_H, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_I_down, KEY_I, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_J_down, KEY_J, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_K_down, KEY_K, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_L_down, KEY_L, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_M_down, KEY_M, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_N_down, KEY_N, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_O_down, KEY_O, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_P_down, KEY_P, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_Q_down, KEY_Q, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_R_down, KEY_R, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_S_down, KEY_S, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_T_down, KEY_T, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_U_down, KEY_U, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_V_down, KEY_V, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_W_down, KEY_W, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_X_down, KEY_X, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_Y_down, KEY_Y, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_Z_down, KEY_Z, KEY_EVENT_DOWN);

		SetKeyHandler(MMI_key_COMMA_down, KEY_COMMA, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_FULLSTOP_down, KEY_FULLSTOP, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_NEWLINE_down, KEY_NEWLINE, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_0_down, KEY_0, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_SPACE_down, KEY_SPACE1, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_AND_down, KEY_AND, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_key_INTEGRR_down, KEY_INTEGRR, KEY_EVENT_DOWN);
		SetKeyHandler(MMI_Shift_Key_Pressed, KEY_SHIFT1, KEY_EVENT_DOWN);
#if defined(__MMI_BLACKBERRY_QWERTY__)
		SetKeyHandler(MMI_key_dollar_down,KEY_DOLLAR,KEY_EVENT_DOWN);
#endif
		//SetKeyHandler(GbKeyBackSpaceHandler, KEY_BACKSPACE1, KEY_EVENT_DOWN);
	}
 
    #endif
}


/*****************************************************************************
 * FUNCTION
 *  register_MMI_stroke_key_input_handler
 * DESCRIPTION
 *  Register key handlers for stroke input type
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_MMI_stroke_key_input_handler(void)
{
#if defined(__MMI_T9__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(UI_dummy_function, KEY_0, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_EVENT_DOWN);
    SetKeyHandler(UI_dummy_function, KEY_6, KEY_EVENT_DOWN);
    SetKeyHandler(UI_dummy_function, KEY_7, KEY_EVENT_DOWN);
    SetKeyHandler(UI_dummy_function, KEY_8, KEY_EVENT_DOWN);
    SetKeyHandler(UI_dummy_function, KEY_9, KEY_EVENT_DOWN);

    SetKeyHandler(UI_dummy_function, KEY_0, KEY_REPEAT);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_REPEAT);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_REPEAT);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_REPEAT);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_REPEAT);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_REPEAT);
    SetKeyHandler(UI_dummy_function, KEY_6, KEY_REPEAT);
    SetKeyHandler(UI_dummy_function, KEY_7, KEY_REPEAT);
    SetKeyHandler(UI_dummy_function, KEY_8, KEY_REPEAT);
    SetKeyHandler(UI_dummy_function, KEY_9, KEY_REPEAT);

    SetKeyHandler(UI_dummy_function, KEY_0, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_1_up, KEY_1, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_2_up, KEY_2, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_3_up, KEY_3, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_4_up, KEY_4, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_5_up, KEY_5, KEY_EVENT_UP);
    SetKeyHandler(UI_dummy_function, KEY_6, KEY_EVENT_UP);
    SetKeyHandler(UI_dummy_function, KEY_7, KEY_EVENT_UP);
    SetKeyHandler(UI_dummy_function, KEY_8, KEY_EVENT_UP);
    SetKeyHandler(UI_dummy_function, KEY_9, KEY_EVENT_UP);
#elif defined(__MMI_ZI__)
    SetKeyHandler(UI_dummy_function, KEY_0, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_6_down, KEY_6, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_7_down, KEY_7, KEY_EVENT_DOWN);
    SetKeyHandler(UI_dummy_function, KEY_8, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_9_down, KEY_9, KEY_EVENT_DOWN);

    SetKeyHandler(UI_dummy_function, KEY_0, KEY_REPEAT);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_REPEAT);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_REPEAT);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_REPEAT);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_REPEAT);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_REPEAT);
    SetKeyHandler(MMI_key_6_down, KEY_6, KEY_REPEAT);
    SetKeyHandler(MMI_key_7_down, KEY_7, KEY_REPEAT);
    SetKeyHandler(UI_dummy_function, KEY_8, KEY_REPEAT);
    SetKeyHandler(MMI_key_9_down, KEY_9, KEY_REPEAT);

    SetKeyHandler(UI_dummy_function, KEY_0, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_1_up, KEY_1, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_2_up, KEY_2, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_3_up, KEY_3, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_4_up, KEY_4, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_5_up, KEY_5, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_6_up, KEY_6, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_7_up, KEY_7, KEY_EVENT_UP);
    SetKeyHandler(UI_dummy_function, KEY_8, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_9_up, KEY_9, KEY_EVENT_UP);
#elif defined(__MMI_ITAP__)
    SetKeyHandler(UI_dummy_function, KEY_0, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_6_down, KEY_6, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_7_down, KEY_7, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_8_down, KEY_8, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_9_down, KEY_9, KEY_EVENT_DOWN);

    SetKeyHandler(UI_dummy_function, KEY_0, KEY_REPEAT);
    SetKeyHandler(MMI_key_1_down, KEY_1, KEY_REPEAT);
    SetKeyHandler(MMI_key_2_down, KEY_2, KEY_REPEAT);
    SetKeyHandler(MMI_key_3_down, KEY_3, KEY_REPEAT);
    SetKeyHandler(MMI_key_4_down, KEY_4, KEY_REPEAT);
    SetKeyHandler(MMI_key_5_down, KEY_5, KEY_REPEAT);
    SetKeyHandler(MMI_key_6_down, KEY_6, KEY_REPEAT);
    SetKeyHandler(MMI_key_7_down, KEY_7, KEY_REPEAT);
    SetKeyHandler(MMI_key_8_down, KEY_8, KEY_REPEAT);
    SetKeyHandler(MMI_key_9_down, KEY_9, KEY_REPEAT);

    SetKeyHandler(UI_dummy_function, KEY_0, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_1_up, KEY_1, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_2_up, KEY_2, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_3_up, KEY_3, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_4_up, KEY_4, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_5_up, KEY_5, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_6_up, KEY_6, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_7_up, KEY_7, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_8_up, KEY_8, KEY_EVENT_UP);
    SetKeyHandler(MMI_key_9_up, KEY_9, KEY_EVENT_UP);
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  clear_MMI_key_input_handler
 * DESCRIPTION
 *  Clear key handlers for input
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_MMI_key_input_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    #ifndef __MMI_FULL_KEYPAD__
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

    ClearKeyHandler(KEY_0, KEY_EVENT_UP);
    ClearKeyHandler(KEY_1, KEY_EVENT_UP);
    ClearKeyHandler(KEY_2, KEY_EVENT_UP);
    ClearKeyHandler(KEY_3, KEY_EVENT_UP);
    ClearKeyHandler(KEY_4, KEY_EVENT_UP);
    ClearKeyHandler(KEY_5, KEY_EVENT_UP);
    ClearKeyHandler(KEY_6, KEY_EVENT_UP);
    ClearKeyHandler(KEY_7, KEY_EVENT_UP);
    ClearKeyHandler(KEY_8, KEY_EVENT_UP);
    ClearKeyHandler(KEY_9, KEY_EVENT_UP);
    #else
	U16 keyCode = 0;
	U16 keyType = 0;
	for (keyCode = KEY_A; keyCode <= KEY_Z; keyCode++)
	{
		for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			ClearKeyHandler(keyCode, keyType);
		}
	}

	for (keyCode = KEY_0; keyCode <= KEY_9; keyCode++)
	{
		for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			ClearKeyHandler(keyCode, keyType);
		}
	}

	for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
	{
		ClearKeyHandler(KEY_STAR, keyType);
	}

	for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
	{
		ClearKeyHandler(KEY_POUND, keyType);
	}
    #endif

}

/* Initialization / termination wrappers  */


/*****************************************************************************
 * FUNCTION
 *  wgui_init
 * DESCRIPTION
 *  Initializes the GUI wrappers
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_MMI_theme((MMI_theme*) MMI_themes[0]);

    gui_initialize_default_themes();

#if(!UI_DISABLE_EMS_INPUT_BOX)
    gui_intialize_EMS();
#endif 

    register_keyboard_key_handler(dummy_keyboard_key_handler);
    register_keyboard_input_handler(dummy_keyboard_input_handler);
    register_keyboard_global_key_handler(dummy_keyboard_key_handler);

    create_softkeys();
    create_menu_shortcut_handler();
    create_fixed_list();
    create_fixed_matrix();
    create_multitaps();
    create_virtual_keyboard();

    clear_key_input_handler();
    clear_key_down_handler();
    clear_key_up_handler();
    initialize_status_icons();
    hide_multitap = UI_dummy_function;
    register_multiline_inputbox_input_callback(UI_dummy_function);
}
