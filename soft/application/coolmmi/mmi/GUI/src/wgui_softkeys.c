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
 *   wgui_softkeys.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  softkey related funtions
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
 * 
 * 
 * 
 *******************************************************************************/

/**********************************************************************************
   Filename:      wgui_softkeys.c
   Date Created:  August-07-2002
   Contains:      UI wrapper routines for Softkeys
**********************************************************************************/
#include "mmi_features.h"
#include "wgui_softkeys.h"
#include "gui_themes.h"
#include "frameworkstruct.h"
#include "debuginitdef.h"
#include "gdi_include.h"
#include "wgui_status_icons.h"//061506 double buffer

/* Globals                 */
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
U8 is_button_bar_shuffled = 0;
extern MMI_white_color;
void (*coolsand_shuffle_draw_background) (void);
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 

/* softkey width */
S32 MMI_softkey_width = MMI_SOFTKEY_WIDTH;

/* softkey height */
S32 MMI_softkey_height = MMI_SOFTKEY_HEIGHT;

/* Softkey wrappers        */
icontext_button MMI_softkeys[WGUI_MAX_SOFTKEYS];

#define  MMI_left_softkey  MMI_softkeys[MMI_LEFT_SOFTKEY]
#define  MMI_right_softkey MMI_softkeys[MMI_RIGHT_SOFTKEY]

#ifdef __MMI_WGUI_CSK_ENABLE__
#define  MMI_center_softkey   MMI_softkeys[MMI_CENTER_SOFTKEY]
#endif 

/* LSK key handler */
U8 left_softkey_keyboard_handler_active = 0;

/* RSKk key handler */
U8 right_softkey_keyboard_handler_active = 0;

#ifdef __MMI_WGUI_CSK_ENABLE__
/* CSK key handler */
U8 center_softkey_keyboard_handler_active = 0;
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 

#define  WGUI_MAX_SOFTKEY_FUNCTIONS 3

void (*softkey_functions[WGUI_MAX_SOFTKEYS][WGUI_MAX_SOFTKEY_FUNCTIONS]) (void);

#define           left_softkey_functions  softkey_functions[MMI_LEFT_SOFTKEY]
#define           right_softkey_functions    softkey_functions[MMI_RIGHT_SOFTKEY]
#define           center_softkey_functions      softkey_functions[MMI_CENTER_SOFTKEY]

/* Functions to hide softkeys */
void (*hide_softkey[WGUI_MAX_SOFTKEYS]) (void);

#ifdef __MMI_T9__
extern void (*t9_inputbox_LSK_function) (void);
#endif 

void softkey_keyboard_handler(S32 vkey_code, S32 key_state);

extern UI_string_type get_string(UI_image_ID_type  i);
extern PU8 get_image(UI_image_ID_type  i);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
#include "mainmenudef.h"
extern U32 AtiveScreenIsIdle(void);
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */

/*****************************************************************************
 * FUNCTION
 *  clear_softkey_button_background_callback
 * DESCRIPTION
 *  Clear the background of softkey. Used of callback function in icontext_button.clear_background_fp
 * PARAMETERS
 *  button_object       [IN]        The target button
 * RETURNS
 *  void
 *****************************************************************************/
void clear_softkey_button_background_callback(void *button_object)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < WGUI_MAX_SOFTKEYS; i++)
    {
        if (button_object == (void*)&MMI_softkeys[i])
        {
            (hide_softkey[i]) ();
            return;
        }
    }
    MMI_DBG_ASSERT(0);
}


/*****************************************************************************
 * FUNCTION
 *  create_softkeys
 * DESCRIPTION
 *  Creates and set default value of softkeys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void create_softkeys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0, key = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* set theme of LSK */
    set_MMI_LSK_theme();
    /* careate button */
    gui_create_icontext_button(
        &MMI_softkeys[MMI_LEFT_SOFTKEY],
        0,
        UI_device_height - MMI_softkey_height,
        MMI_softkey_width,
        MMI_softkey_height,
        NULL,
        NULL);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)|| defined (__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_UI_TECHNO_SOFTKEYS__)
    MMI_softkeys[MMI_LEFT_SOFTKEY].flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_LEFT_ALIGN | UI_BUTTON_CENTER_TEXT_Y | UI_BUTTON_CENTER_TEXT_X |
         UI_BUTTON_CENTER_ICON_Y | UI_BUTTON_ALIGN_ICON_TEXT);
#elif defined(__MMI_UI_DALMATIAN_SOFTKEYS__)
    MMI_softkeys[MMI_LEFT_SOFTKEY].flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_CENTER_TEXT_Y | UI_BUTTON_CENTER_TEXT_X | UI_BUTTON_CENTER_ICON_Y |
         UI_BUTTON_ALIGN_ICON_TEXT);
#else 
    MMI_softkeys[MMI_LEFT_SOFTKEY].flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_LEFT_ALIGN | UI_BUTTON_CENTER_TEXT_Y | UI_BUTTON_CENTER_ICON_Y |
         UI_BUTTON_ALIGN_ICON_TEXT | UI_BUTTON_DISABLE_BACKGND_DISPLAY);
#endif 
#else /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_220X176__)*/ 
    MMI_softkeys[MMI_LEFT_SOFTKEY].flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_CENTER_TEXT_X | UI_BUTTON_CENTER_TEXT_Y | UI_BUTTON_CENTER_ICON_Y |
         UI_BUTTON_ALIGN_ICON_TEXT);
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_220X176__)*/ 

    /* set theme of RSK */
    set_MMI_RSK_theme();
    /* create RK button */

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    gui_create_icontext_button(
        &MMI_softkeys[MMI_RIGHT_SOFTKEY],
        UI_device_width - MMI_softkey_width,
        UI_device_height - MMI_softkey_height - (MMI_BUTTON_BAR_HEIGHT - MMI_softkey_height),
        MMI_softkey_width,
        MMI_softkey_height,
        NULL,
        NULL);
#else /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
    gui_create_icontext_button(
        &MMI_softkeys[MMI_RIGHT_SOFTKEY],
        UI_device_width - MMI_softkey_width,
        UI_device_height - MMI_softkey_height,
        MMI_softkey_width,
        MMI_softkey_height,
        NULL,
        NULL);
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)|| defined (__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_UI_TECHNO_SOFTKEYS__)
    MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_RIGHT_ALIGN | UI_BUTTON_CENTER_TEXT_X | UI_BUTTON_CENTER_TEXT_Y |
         UI_BUTTON_CENTER_ICON_Y | UI_BUTTON_ALIGN_ICON_TEXT);
#elif defined(__MMI_UI_DALMATIAN_SOFTKEYS__)
    MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_CENTER_TEXT_X | UI_BUTTON_CENTER_TEXT_Y | UI_BUTTON_CENTER_ICON_Y |
         UI_BUTTON_ALIGN_ICON_TEXT);
#else 
    MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_RIGHT_ALIGN | UI_BUTTON_CENTER_TEXT_Y | UI_BUTTON_CENTER_ICON_Y |
         UI_BUTTON_ALIGN_ICON_TEXT | UI_BUTTON_DISABLE_BACKGND_DISPLAY);
#endif 
#else /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__) */ 
    MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_CENTER_TEXT_X | UI_BUTTON_CENTER_TEXT_Y | UI_BUTTON_CENTER_ICON_Y |
         UI_BUTTON_ALIGN_ICON_TEXT);
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__) */ 

#ifdef __MMI_WGUI_CSK_ENABLE__
    /* create CSK button */
    gui_create_icontext_button(
        &MMI_softkeys[MMI_CENTER_SOFTKEY],
        MMI_softkey_width + ((UI_device_width - (MMI_softkey_width) * 3) >> 1),
        UI_device_height - MMI_softkey_height,
        MMI_softkey_width,
        MMI_softkey_height,
        NULL,
        NULL);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)|| defined (__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    MMI_center_softkey.flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_CENTER_TEXT_X | UI_BUTTON_CENTER_TEXT_Y | UI_BUTTON_CENTER_ICON_Y |
         UI_BUTTON_ALIGN_ICON_TEXT | UI_BUTTON_DISABLE_BACKGND_DISPLAY);
#else /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_220X176__)*/ 
    MMI_center_softkey.flags |=
        (UI_BUTTON_SHIFT_BUTTON1 | UI_BUTTON_CENTER_TEXT_Y | UI_BUTTON_CENTER_ICON_Y | UI_BUTTON_ALIGN_ICON_TEXT);
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_220X176__)*/ 
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 

    /* initilaize softkey functions to dummy func */
    for (key = 0; key < WGUI_MAX_SOFTKEYS; key++)
    {
        MMI_softkeys[key].clear_background_fp = clear_softkey_button_background_callback;
        hide_softkey[key] = UI_dummy_function;
        for (i = 0; i < WGUI_MAX_SOFTKEY_FUNCTIONS; i++)
        {
            softkey_functions[key][i] = UI_dummy_function;
        }
    }
    register_keyboard_global_key_handler(softkey_keyboard_handler);
}


/*****************************************************************************
 * FUNCTION
 *  restore_softkeys
 * DESCRIPTION
 *  set the softkeys to the MMI's default positions and sizes
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void restore_softkeys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 key = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (key = 0; key < WGUI_MAX_SOFTKEYS; key++)
    {
        gui_resize_icontext_button(&MMI_softkeys[key], MMI_softkey_width, MMI_softkey_height);
        gui_move_icontext_button(&MMI_softkeys[key], 0, UI_device_height - MMI_softkey_height);
        gui_set_icontext_button_current_theme(&MMI_softkeys[key]);
    }
}


/*****************************************************************************
 * FUNCTION
 *  resize_softkey_by_string_length
 * DESCRIPTION
 *  resize the left softkey width according the the string
 * PARAMETERS
 *  s       [IN]        String
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void resize_softkey_by_string_length(UI_string_type s, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 sw, sh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_set_font(MMI_softkeys[key].text_font);
    gui_measure_string(s, &sw, &sh);
    gui_resize_icontext_button(&MMI_softkeys[key], sw + WGUI_SOFTKEY_STRING_GAP, MMI_softkeys[key].height);
    gui_move_icontext_button(&MMI_softkeys[key], 0, UI_device_height - MMI_softkeys[key].height);
}


/*****************************************************************************
 * FUNCTION
 *  set_softkey_label
 * DESCRIPTION
 *  Changes the label of softkeys
 * PARAMETERS
 *  s       [IN]        String
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void set_softkey_label(UI_string_type s, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_softkeys[key].text = s;
    if (MMI_softkeys[key].normal_down_icon == NULL)
    {
        MMI_softkeys[key].flags &= ~UI_BUTTON_CENTER_ICON_X;
    #ifdef __MMI_WGUI_CSK_ENABLE__
        if (key == MMI_CENTER_SOFTKEY)
        {
            MMI_softkeys[key].flags |= UI_BUTTON_CENTER_TEXT_X;
        }
    #endif /* __MMI_WGUI_CSK_ENABLE__ */ 
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
    #if !(defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)|| defined (__MMI_MAINLCD_320X240__))
/*- zhouqin 2011-02-16 modify for q6 320X240 */
        MMI_softkeys[key].flags |= UI_BUTTON_CENTER_TEXT_X;
    #endif 
    }
    gui_resize_icontext_button(&MMI_softkeys[key], MMI_softkeys[key].width, MMI_softkeys[key].height);
}


/*****************************************************************************
 * FUNCTION
 *  set_softkey_icon
 * DESCRIPTION
 *  Changes the icon of softkeys
 * PARAMETERS
 *  icon        [IN]        Icon
 *  key         [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void set_softkey_icon(PU8 icon, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 new_icon = icon;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon != NULL) && (icon[0] == 0))
    {
        new_icon = NULL;
    }

    if (icon != NULL && icon == MMI_softkeys[key].normal_up_icon)
    {
        return;
    }

    /* set icon of LSK button */
    gui_icontext_button_set_icon(&MMI_softkeys[key], new_icon, gui_transparent_color(0, 0, 0));
    if (MMI_softkeys[key].text == NULL)
    {
    #if !(defined __MMI_UI_TECHNO_SOFTKEYS__ || defined __MMI_UI_DALMATIAN_SOFTKEYS__ )
        MMI_softkeys[key].flags &= ~UI_BUTTON_CENTER_TEXT_X;
    #endif 
        MMI_softkeys[key].flags |= UI_BUTTON_CENTER_ICON_X;
    }
    gui_resize_icontext_button(&MMI_softkeys[key], MMI_softkeys[key].width, MMI_softkeys[key].height);
}


/*****************************************************************************
 * FUNCTION
 *  change_softkey
 * DESCRIPTION
 *  Changes the string and icon of softkeys
 * PARAMETERS
 *  s       [IN]        String
 *  i       [IN]        Icon
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void change_softkey(U16 s, U16 i, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (s != STRING_ID_NULL)
    {
        UI_string_type str = get_string(s);

        set_softkey_label(str, key);
    }
    else
    {
        set_softkey_label(NULL, key);
    }

    if (i != IMAGE_ID_NULL)
    {
        PU8 img = get_image(i);

        set_softkey_icon(img, key);
    }
    else
    {
        set_softkey_icon(NULL, key);
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_softkey_function
 * DESCRIPTION
 *  Changes the handler of softkeys
 * PARAMETERS
 *  f       [IN]        Handler function
 *  k       [IN]        Key event type
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void set_softkey_function(void (*f) (void), MMI_key_event_type k, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (k >= WGUI_MAX_SOFTKEY_FUNCTIONS)
    {
        return;
    }

    switch (key)
    {
        case MMI_LEFT_SOFTKEY:
            if (k == KEY_LONG_PRESS)
            {
                SetKeyHandler(f, KEY_LSK, KEY_LONG_PRESS);
            }
            softkey_functions[key][k] = f;
			
            break;

        case MMI_RIGHT_SOFTKEY:
            if (k == KEY_LONG_PRESS)
            {
                SetKeyHandler(f, KEY_RSK, KEY_LONG_PRESS);
            }
            softkey_functions[key][k] = f;
            break;

        #ifdef __MMI_WGUI_CSK_ENABLE__
        case MMI_CENTER_SOFTKEY:
            if (k == KEY_LONG_PRESS)
            {
                SetKeyHandler(f, KEY_CSK, KEY_LONG_PRESS);
            }
            softkey_functions[key][k] = f;
            break;
        #endif /* __MMI_WGUI_CSK_ENABLE__ */ 
		case WGUI_MAX_SOFTKEYS:
		default:
			break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  get_softkey_function
 * DESCRIPTION
 *  get softkey handlers
 * PARAMETERS
 *  k       [IN]        Key event
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
FuncPtr get_softkey_function(MMI_key_event_type k, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (k >= WGUI_MAX_SOFTKEY_FUNCTIONS)
    {
        return NULL;
    }

    return softkey_functions[key][k];
}


/*****************************************************************************
 * FUNCTION
 *  execute_softkey_function
 * DESCRIPTION
 *  execute softkey handlers
 * PARAMETERS
 *  k       [IN]        Key event
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
 extern U8 change_window_style;
 #endif
void execute_softkey_function(MMI_key_event_type k, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (k >= WGUI_MAX_SOFTKEY_FUNCTIONS)
    {
        return;
    }
 #ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
    if(key == MMI_LEFT_SOFTKEY)
	{
//		change_window_style = 10;
	}
#endif
    if (softkey_functions[key][k] != NULL)
    {
		mmi_trace(1,"Key_Event: Func: %s softkey_functions=%p", __FUNCTION__,softkey_functions[key][k]);
		
        softkey_functions[key][k] ();
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_softkey
 * DESCRIPTION
 *  Sets the label and the function to be called for the softkey
 * PARAMETERS
 *  s       [IN]        New string to be used as the softkey label
 *  f       [IN]        Function that is called when a particular softkey event occurs
 *  k       [IN]        Key event
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void set_softkey(UI_string_type s, void (*f) (void), MMI_key_event_type k, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (k >= WGUI_MAX_SOFTKEY_FUNCTIONS)
    {
        return;
    }

    set_softkey_label(s, key);
    set_softkey_function(f, k, key);
}


/*****************************************************************************
 * FUNCTION
 *  clear_softkey_display_functions
 * DESCRIPTION
 *  Clear the functions of the softkey
 * PARAMETERS
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void clear_softkey_display_functions(WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    hide_softkey[key] = UI_dummy_function;
    for (i = 0; i < WGUI_MAX_SOFTKEY_FUNCTIONS; i++)
    {
        softkey_functions[key][i] = UI_dummy_function;
    }
}


/*****************************************************************************
 * FUNCTION
 *  show_softkey
 * DESCRIPTION
 *  Displays softkey
 * PARAMETERS
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void show_softkey(WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((MMI_softkeys[key].text == NULL) && (MMI_softkeys[key].normal_up_icon == NULL))
    {
        clear_softkey_handler(key);
        return;
    }

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
     switch (key)
    {
        case MMI_LEFT_SOFTKEY:

            gui_show_icontext_button(&MMI_softkeys[key]);
            break;

        case MMI_RIGHT_SOFTKEY:
            /* To show right key bottom aligned */
            if ((MMI_left_softkey.text == NULL) && (MMI_left_softkey.normal_up_icon == NULL))
            {
                MMI_right_softkey.y = MMI_left_softkey.y;
            }
            gui_show_icontext_button(&MMI_softkeys[key]);
            /*  Reset the right softkey position which is changed whn left is null. */
            if ((MMI_left_softkey.text == NULL) && (MMI_left_softkey.normal_up_icon == NULL))
            {
                MMI_right_softkey.y =
                    UI_device_height - MMI_softkey_height - (MMI_BUTTON_BAR_HEIGHT - MMI_softkey_height);
            }
            break;
    }
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__
        /*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#ifdef __MMI_BLACK_BERRY_MAIN_MENU__
	 if(GetActiveScreenId() != MAIN_MENU_SCREENID)
	#endif
         /*- zhouqin 2011-02-16 modify for q6 320X240 */
	  	gui_show_icontext_button(&MMI_softkeys[key]);
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  move_softkey
 * DESCRIPTION
 *  change the position of softkey
 * PARAMETERS
 *  x       [IN]        New x-coordinate of softkey
 *  y       [IN]        New y-coordinate of softkey
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void move_softkey(S32 x, S32 y, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_move_icontext_button(&MMI_softkeys[key], x, y);
}


/*****************************************************************************
 * FUNCTION
 *  resize_softkey
 * DESCRIPTION
 *  change the width and height of softkey
 * PARAMETERS
 *  width       [IN]        New width of softkey
 *  height      [IN]        New height of softkey
 *  key         [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void resize_softkey(S32 width, S32 height, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_resize_icontext_button(&MMI_softkeys[key], width, height);
}


/*****************************************************************************
 * FUNCTION
 *  redraw_softkey
 * DESCRIPTION
 *  Redraw softkey
 * PARAMETERS
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
#include "idleappdef.h"
#endif
void redraw_softkey(WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 idx = 0;//061506 double buffer

#ifndef __MMI_UI_DALMATIAN_SOFTKEYS__

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    hide_softkey[key] ();
    show_softkey(key);
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 

    switch (key)
    {
        case MMI_LEFT_SOFTKEY:
    #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
            if (!is_on_idlescreen())
            {
                show_softkey_background();
            }

            if ((MMI_left_softkey.text == NULL) && (MMI_left_softkey.normal_up_icon == NULL))
            {
                /* To draw right softkey always above the left key. */
                redraw_right_softkey();
                gdi_layer_blt_previous(MMI_softkeys[MMI_LEFT_SOFTKEY].x, UI_device_height - MMI_BUTTON_BAR_HEIGHT, UI_device_width - 1, UI_device_height - 1);   
                return;
            }
            show_softkey(key);
            redraw_right_softkey();
            gdi_layer_blt_previous(MMI_softkeys[MMI_LEFT_SOFTKEY].x, UI_device_height - MMI_BUTTON_BAR_HEIGHT, UI_device_width - 1, UI_device_height - 1);       
    #else /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
            gdi_layer_blt_previous(
                MMI_softkeys[MMI_LEFT_SOFTKEY].x,
                MMI_softkeys[MMI_LEFT_SOFTKEY].y,
                (MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width + 2),
                (MMI_softkeys[MMI_LEFT_SOFTKEY].y + MMI_softkeys[MMI_LEFT_SOFTKEY].height + 2));
    #endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
            break;

        case MMI_RIGHT_SOFTKEY:
        #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
            /* set the right key position to bottom aligned when left is NULL  */
            if ((MMI_left_softkey.text == NULL) && (MMI_left_softkey.normal_up_icon == NULL))
            {
                MMI_softkeys[MMI_RIGHT_SOFTKEY].y = UI_device_height - MMI_softkey_height;
            }
            show_softkey(key);
            gdi_layer_blt_previous(
                MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
                MMI_softkeys[MMI_RIGHT_SOFTKEY].y,
                (MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width + 2),
                (MMI_softkeys[MMI_RIGHT_SOFTKEY].y + MMI_softkeys[MMI_RIGHT_SOFTKEY].height + 2));
            /* Reset the right key positions in its original position. */
            if ((MMI_left_softkey.text == NULL) && (MMI_left_softkey.normal_up_icon == NULL))
            {
                MMI_softkeys[MMI_RIGHT_SOFTKEY].y =
                    UI_device_height - MMI_softkey_height - (MMI_BUTTON_BAR_HEIGHT - MMI_softkey_height);
            }
        #else /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
            gdi_layer_blt_previous(
                MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
                MMI_softkeys[MMI_RIGHT_SOFTKEY].y,
                (MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width + 2),
                (MMI_softkeys[MMI_RIGHT_SOFTKEY].y + MMI_softkeys[MMI_RIGHT_SOFTKEY].height + 2));
        #endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
            break;
        #ifdef __MMI_WGUI_CSK_ENABLE__
        case MMI_CENTER_SOFTKEY:
            gdi_layer_blt_previous(
                MMI_softkeys[MMI_CENTER_SOFTKEY].x,
                MMI_softkeys[MMI_CENTER_SOFTKEY].y,
                (MMI_softkeys[MMI_CENTER_SOFTKEY].x + MMI_softkeys[MMI_CENTER_SOFTKEY].width + 2),
                (MMI_softkeys[MMI_CENTER_SOFTKEY].y + MMI_softkeys[MMI_CENTER_SOFTKEY].height + 2));
            break;
        #endif /* __MMI_WGUI_CSK_ENABLE__ */ 
		case WGUI_MAX_SOFTKEYS:
		default:
			break;
    }

    /* 061506 double buffer Start */
    for (idx=0; idx<MAX_STATUS_ICON_BARS; idx++)
    {
        if(get_status_icon_bar_type((U8)idx)&STATUS_ICON_BAR_ENABLE_DOUBLE_BUFFER)
        {
            gdi_layer_push_and_set_active(get_status_icon_bar_layer((U8)idx));
            gdi_layer_toggle_double();
            gdi_layer_copy_double();//070306 copy double
            gdi_layer_pop_and_restore_active();
        }
    }
    /* 061506 double buffer End */
}


/*****************************************************************************
 * FUNCTION
 *  left_softkey_down
 * DESCRIPTION
 *  Handles left softkey down event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void left_softkey_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(MMI_softkeys[MMI_LEFT_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED))
    {                                       /* change the state of LSK button */
        MMI_softkeys[MMI_LEFT_SOFTKEY].flags |= UI_BUTTON_STATE_CLICKED;
        MMI_softkeys[MMI_LEFT_SOFTKEY].flags |= UI_BUTTON_STATE_DOWN;
    #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
        /* Set status of shuffling on click & call shuffle. */
        is_button_bar_shuffled = 0;
        gui_call_shuffle();
    #else /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 

#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
		if(IDLE_SCREEN_ID != GetActiveScreenId())
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#elif defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
		 if(AtiveScreenIsIdle()==0)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif
		redraw_softkey(MMI_LEFT_SOFTKEY);   /* redraw LSK */
        /* call function handlers of LSK down */
    #endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
        execute_softkey_function(KEY_EVENT_DOWN, MMI_LEFT_SOFTKEY);
    }
}


/*****************************************************************************
 * FUNCTION
 *  left_softkey_up
 * DESCRIPTION
 *  Handles left softkey up event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void left_softkey_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_softkeys[MMI_LEFT_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED)
    {   /* change the state of LSK button */
        MMI_softkeys[MMI_LEFT_SOFTKEY].flags &= ~UI_BUTTON_STATE_CLICKED;
        MMI_softkeys[MMI_LEFT_SOFTKEY].flags &= ~UI_BUTTON_STATE_DOWN;
    #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
        /* Reset status of shuffling on click & call shuffle. */
        is_button_bar_shuffled = 1;
        gui_call_shuffle();
    #else /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
		if(IDLE_SCREEN_ID != GetActiveScreenId())
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	      
#elif defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
		if(AtiveScreenIsIdle()==0)	
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif
        redraw_softkey(MMI_LEFT_SOFTKEY);
    #endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 

        /* call function handlers of LSK Up */
        execute_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY);
    }
}

/*****************************************************************************
 * FUNCTION
 *  left_softkey_up
 * DESCRIPTION
 *  Handles left softkey up event but no redraw
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void update_softkey_state_no_redraw( U16 KeyCode, U16 KeyType)
{
	if ( KeyCode == KEY_LSK )
	{
		if ( KeyType == KEY_EVENT_UP )
		{
			if (MMI_softkeys[MMI_LEFT_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED)
			{   /* change the state of LSK button */
				MMI_softkeys[MMI_LEFT_SOFTKEY].flags &= ~UI_BUTTON_STATE_CLICKED;
				MMI_softkeys[MMI_LEFT_SOFTKEY].flags &= ~UI_BUTTON_STATE_DOWN;
			}
		}
		else if ( KeyType == KEY_EVENT_DOWN )
		{
			if (!(MMI_softkeys[MMI_LEFT_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED))
			{
				/* change the state of LSK button */
				MMI_softkeys[MMI_LEFT_SOFTKEY].flags |= UI_BUTTON_STATE_CLICKED;
				MMI_softkeys[MMI_LEFT_SOFTKEY].flags |= UI_BUTTON_STATE_DOWN;
			}
		}
	}
	else if ( KeyCode == KEY_RSK )
	{
		if ( KeyType == KEY_EVENT_UP )
		{
			if (MMI_softkeys[MMI_RIGHT_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED)
			{   /* change the state of RSK button */
				MMI_softkeys[MMI_RIGHT_SOFTKEY].flags &= ~UI_BUTTON_STATE_CLICKED;
				MMI_softkeys[MMI_RIGHT_SOFTKEY].flags &= ~UI_BUTTON_STATE_DOWN;
			}
		}
		else if ( KeyType == KEY_EVENT_DOWN )
		{
			if (!(MMI_softkeys[MMI_RIGHT_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED))
			{
				/* change the state of RSK button */
				MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |= UI_BUTTON_STATE_CLICKED;
				MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |= UI_BUTTON_STATE_DOWN;
			}
		}
	}
}

/*****************************************************************************
 * FUNCTION
 *  right_softkey_down
 * DESCRIPTION
 *  Handles right softkey down event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void right_softkey_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(MMI_softkeys[MMI_RIGHT_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED))
    {
        /* change the state of RSK button */
        MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |= UI_BUTTON_STATE_CLICKED;
        MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |= UI_BUTTON_STATE_DOWN;
    #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
        /* Set status of shuffling on click & call shuffle. */
        is_button_bar_shuffled = 0;
        gui_call_shuffle(); /* shuffle_softkeys */
    #else /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
		if(IDLE_SCREEN_ID != GetActiveScreenId())
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#elif defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
		if(AtiveScreenIsIdle()==0)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif
        redraw_softkey(MMI_RIGHT_SOFTKEY);
    #endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
        execute_softkey_function(KEY_EVENT_DOWN, MMI_RIGHT_SOFTKEY);
    }
}


/*****************************************************************************
 * FUNCTION
 *  right_softkey_up
 * DESCRIPTION
 *  Handles right softkey up event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void right_softkey_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_softkeys[MMI_RIGHT_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED)
    {
        /* change the state of LSK button */
        MMI_softkeys[MMI_RIGHT_SOFTKEY].flags &= ~UI_BUTTON_STATE_CLICKED;
        MMI_softkeys[MMI_RIGHT_SOFTKEY].flags &= ~UI_BUTTON_STATE_DOWN;
    #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
        /* Reset status of shuffling on click & call shuffle. */
        is_button_bar_shuffled = 1;
        gui_call_shuffle();
    #else /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
		if(IDLE_SCREEN_ID != GetActiveScreenId())
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#elif defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
		if(AtiveScreenIsIdle()==0)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif
        redraw_softkey(MMI_RIGHT_SOFTKEY);
    #endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
        execute_softkey_function(KEY_EVENT_UP, MMI_RIGHT_SOFTKEY);
    }
}

#ifdef __MMI_WGUI_CSK_ENABLE__


/*****************************************************************************
 * FUNCTION
 *  center_softkey_down
 * DESCRIPTION
 *  Handles center softkey down event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void center_softkey_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(MMI_softkeys[MMI_CENTER_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED))
    {   /* change the state of LSK button */
        MMI_softkeys[MMI_CENTER_SOFTKEY].flags |= UI_BUTTON_STATE_CLICKED;
        MMI_softkeys[MMI_CENTER_SOFTKEY].flags |= UI_BUTTON_STATE_DOWN;
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
		if(MAIN_MENU_SCREENID != GetActiveScreenId())
#endif
        redraw_softkey(MMI_CENTER_SOFTKEY);
        /* call function handlers of LSK down  */
        execute_softkey_function(KEY_EVENT_DOWN, MMI_CENTER_SOFTKEY);
    }
}


/*****************************************************************************
 * FUNCTION
 *  center_softkey_up
 * DESCRIPTION
 *  Handles left softkey up event
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void center_softkey_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_softkeys[MMI_CENTER_SOFTKEY].flags & UI_BUTTON_STATE_CLICKED)
    {   /* change the state of LSK button */
        MMI_softkeys[MMI_CENTER_SOFTKEY].flags &= ~UI_BUTTON_STATE_CLICKED;
        MMI_softkeys[MMI_CENTER_SOFTKEY].flags &= ~UI_BUTTON_STATE_DOWN;
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
		if(MAIN_MENU_SCREENID != GetActiveScreenId())
#endif
        redraw_softkey(MMI_CENTER_SOFTKEY);
        /* call function handlers of LSK Up */
        execute_softkey_function(KEY_EVENT_UP, MMI_CENTER_SOFTKEY);
    }
}
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 


/*****************************************************************************
 * FUNCTION
 *  register_softkey_handler
 * DESCRIPTION
 *  Register handlers of softkeys
 * PARAMETERS
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void register_softkey_handler(WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((MMI_softkeys[key].text == NULL) && (MMI_softkeys[key].normal_up_icon == NULL))
    {
        return;
    }

    switch (key)
    {
        case MMI_LEFT_SOFTKEY:
            SetKeyHandler(left_softkey_down, KEY_LSK, KEY_EVENT_DOWN);
            SetKeyHandler(left_softkey_up, KEY_LSK, KEY_EVENT_UP);
            left_softkey_keyboard_handler_active = 1;
            break;

        case MMI_RIGHT_SOFTKEY:
            SetKeyHandler(right_softkey_down, KEY_RSK, KEY_EVENT_DOWN);
            SetKeyHandler(right_softkey_up, KEY_RSK, KEY_EVENT_UP);
            right_softkey_keyboard_handler_active = 1;
            break;

    #ifdef __MMI_WGUI_CSK_ENABLE__
        case MMI_CENTER_SOFTKEY:
            SetKeyHandler(center_softkey_down, KEY_CSK, KEY_EVENT_DOWN);
            SetKeyHandler(center_softkey_up, KEY_CSK, KEY_EVENT_UP);
            center_softkey_keyboard_handler_active = 1;
            break;
    #endif /* __MMI_WGUI_CSK_ENABLE__ */ 
		case WGUI_MAX_SOFTKEYS:
		default:
			break;
    }
}

#ifdef __MMI_WGUI_CSK_ENABLE__


/*****************************************************************************
 * FUNCTION
 *  register_center_softkey_to_enter_key
 * DESCRIPTION
 *  Register center softkey handlers to enter key
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_center_softkey_to_enter_key(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(center_softkey_up, KEY_ENTER, KEY_EVENT_UP);
    SetKeyHandler(center_softkey_down, KEY_ENTER, KEY_EVENT_DOWN);
}
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 


/*****************************************************************************
 * FUNCTION
 *  clear_softkey_handler
 * DESCRIPTION
 *  Register handlers of softkeys
 * PARAMETERS
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void clear_softkey_handler(WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (key)
    {
     	case MMI_LEFT_SOFTKEY:
	 	//case KEY_LSK:
            ClearKeyHandler(KEY_LSK, KEY_EVENT_DOWN);
            ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
            left_softkey_keyboard_handler_active = 0;
            break;

        case MMI_RIGHT_SOFTKEY:
	 	//case KEY_RSK:
            ClearKeyHandler(KEY_RSK, KEY_EVENT_DOWN);
            ClearKeyHandler(KEY_RSK, KEY_EVENT_UP);
            right_softkey_keyboard_handler_active = 0;
            break;

    #ifdef __MMI_WGUI_CSK_ENABLE__
        case MMI_CENTER_SOFTKEY:
	 	//case KEY_CSK:
            ClearKeyHandler(KEY_CSK, KEY_EVENT_DOWN);
            ClearKeyHandler(KEY_CSK, KEY_EVENT_UP);
            center_softkey_keyboard_handler_active = 0;
            break;
    #endif /* __MMI_WGUI_CSK_ENABLE__ */ 
		case WGUI_MAX_SOFTKEYS:
			break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  softkey_keyboard_handler
 * DESCRIPTION
 *  Keyboard handler (Simulator) for softkeys
 * PARAMETERS
 *  vkey_code       [IN]        Key type
 *  key_state       [IN]        Key event type
 * RETURNS
 *  void
 *****************************************************************************/
void softkey_keyboard_handler(S32 vkey_code, S32 key_state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 112:
                if (left_softkey_keyboard_handler_active)
                {
                    left_softkey_down();
                }
                break;
            case 113:
                if (right_softkey_keyboard_handler_active)
                {
                    right_softkey_down();
                }
                break;
        }
    }
    else
    {
        switch (vkey_code)
        {
            case 112:
                if (left_softkey_keyboard_handler_active)
                {
                    left_softkey_up();
                }
                break;
            case 113:
                if (right_softkey_keyboard_handler_active)
                {
                    right_softkey_up();
                }
                break;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  register_hide_softkey
 * DESCRIPTION
 *  Register handlers of hide softkeys
 * PARAMETERS
 *  f       [IN]        Callback function to hide softkey
 *  key     [IN]        Softkey enum
 * RETURNS
 *  void
 *****************************************************************************/
void register_hide_softkey(void (*f) (void), WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    hide_softkey[key] = f;
}


/*****************************************************************************
 * FUNCTION
 *  reset_softkeys
 * DESCRIPTION
 *  Sets the softkey states to normal and clears the hide functions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_softkeys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < WGUI_MAX_SOFTKEYS; i++)
    {
        MMI_softkeys[i].flags &= ~UI_BUTTON_STATE_CLICKED;
        MMI_softkeys[i].flags &= ~UI_BUTTON_STATE_DOWN;
        MMI_softkeys[i].flags &= ~UI_BUTTON_STATE_DISABLED;
        MMI_softkeys[i].flags &= ~UI_BUTTON_ALWAYS_SEND_PEN_UP_INSIDE;
        MMI_softkeys[i].flags &= ~UI_BUTTON_NO_SHIFT_BUTTON;
        hide_softkey[i] = UI_dummy_function;
    }
}


/*****************************************************************************
 * FUNCTION
 *  reset_softkey
 * DESCRIPTION
 *  Sets the softkey states to normal and clears the hide functions
 * PARAMETERS
 *  key     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void reset_softkey(WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_softkeys[key].flags &= ~UI_BUTTON_STATE_CLICKED;
    MMI_softkeys[key].flags &= ~UI_BUTTON_STATE_DOWN;
    MMI_softkeys[key].flags &= ~UI_BUTTON_STATE_DISABLED;
    MMI_softkeys[key].flags &= ~UI_BUTTON_ALWAYS_SEND_PEN_UP_INSIDE;
    /* Do not reset UI_BUTTON_NO_SHIFT_BUTTON and hide_softkey[] */
}


/*****************************************************************************
 * FUNCTION
 *  disable_softkey_background
 * DESCRIPTION
 *  Set some flags to disable background display of softkeys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void disable_softkey_background(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < WGUI_MAX_SOFTKEYS; i++)
    {
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */
    #if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)|| defined(__MMI_MAINLCD_320X240__)
    /*- zhouqin 2011-02-16 modify for q6 320X240 */
	MMI_softkeys[i].flags |= UI_BUTTON_NO_UNDER_BAR;
    #else 
        MMI_softkeys[i].flags |= UI_BUTTON_DISABLE_BACKGND_DISPLAY;
    #endif 
    }
}


/*****************************************************************************
 * FUNCTION
 *  enable_softkey_background
 * DESCRIPTION
 *  Set some flags to disable background display of softkeys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void enable_softkey_background(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < WGUI_MAX_SOFTKEYS; i++)
    {
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */
    #if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)|| defined(__MMI_MAINLCD_320X240__)
   /*- zhouqin 2011-02-16 modify for q6 320X240 */
	MMI_softkeys[i].flags &= ~UI_BUTTON_NO_UNDER_BAR;
    #else 
        MMI_softkeys[i].flags &= ~UI_BUTTON_DISABLE_BACKGND_DISPLAY;
    #endif 
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_softkey_flags_on
 * DESCRIPTION
 *  turn on flag of softkeys
 * PARAMETERS
 *  flag        [IN]        
 *  key         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_softkey_flags_on(S32 flag, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_softkeys[key].flags |= flag;
}


/*****************************************************************************
 * FUNCTION
 *  set_softkey_flags_off
 * DESCRIPTION
 *  turn off  flag of softkeys
 * PARAMETERS
 *  flag        [IN]        
 *  key         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_softkey_flags_off(S32 flag, WGUI_SOFTKEY_ENUM key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_softkeys[key].flags &= ~flag;
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  translate_softkey_pen_event
 * DESCRIPTION
 *  translate low level key events to softkey related events
 * PARAMETERS
 *  key                 [IN]            One of value of WGUI_SOFTKEY_ENUM
 *  pen_event           [IN]            Low level pen event type
 *  x                   [IN]            X-coordinate of pen event
 *  y                   [IN]            Y-coordinate of pen event
 *  button_event        [IN/OUT]        Softkey related event type
 * RETURNS
 *  void
 *****************************************************************************/
BOOL translate_softkey_pen_event(
        WGUI_SOFTKEY_ENUM key,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_button_pen_enum *button_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gui_icontext_button_translate_pen_event(&MMI_softkeys[key], pen_event, x, y, button_event);
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_softkey_get_dimension
 * DESCRIPTION
 *  get softkey dimenstion
 * PARAMETERS
 *  key         [IN]            One of value of WGUI_SOFTKEY_ENUM
 *  width       [IN/OUT]        Width of softkey
 *  height      [IN/OUT]        Height of softkey
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_softkey_get_dimension(WGUI_SOFTKEY_ENUM key, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *width = MMI_softkeys[key].width;
    *height = MMI_softkeys[key].height;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_softkey_get_position
 * DESCRIPTION
 *  get softkey position
 * PARAMETERS
 *  key         [IN]            One of value of WGUI_SOFTKEY_ENUM
 *  x           [IN/OUT]        X-position of softkey
 *  y           [?]             
 *  yx(?)       [IN/OUT]        Y-positino of softkey
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_softkey_get_position(WGUI_SOFTKEY_ENUM key, S32 *x, S32 *y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *x = MMI_softkeys[key].x;
    *y = MMI_softkeys[key].y;
}

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__


/*****************************************************************************
 * FUNCTION
 *  gui_register_button_shuffle
 * DESCRIPTION
 *  Registers the function to be called to shuffle the softkeys
 * PARAMETERS
 *  f               [IN]        
 *  function(?)     [IN]        Pointer
 * RETURNS
 *  void
 *****************************************************************************/
void gui_register_button_shuffle(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    coolsand_shuffle_draw_background = f;
}


/*****************************************************************************
 * FUNCTION
 *  gui_call_shuffle
 * DESCRIPTION
 *  Called the shuffle Registered function for the softkeys
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_call_shuffle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_push_clip();
    gdi_layer_set_clip(0, UI_device_height - MMI_BUTTON_BAR_HEIGHT, UI_device_width - 1, UI_device_height - 1);

    coolsand_shuffle_draw_background();

    if (is_button_bar_shuffled == 0)
    {
        MMI_right_softkey.y = UI_device_height - MMI_softkey_height;
        MMI_left_softkey.y = UI_device_height - MMI_softkey_height - (MMI_BUTTON_BAR_HEIGHT - MMI_softkey_height);

        show_right_softkey();
        show_left_softkey();

    }
    else
    {
        MMI_left_softkey.y = UI_device_height - MMI_softkey_height;
        MMI_right_softkey.y = UI_device_height - MMI_softkey_height - (MMI_BUTTON_BAR_HEIGHT - MMI_softkey_height);
        show_left_softkey();
        show_right_softkey();

    }
    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, UI_device_height - MMI_BUTTON_BAR_HEIGHT, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  shuffle_draw_white
 * DESCRIPTION
 *  Common Function for draw white background in shuffle
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void shuffle_draw_white(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_draw_solid_rect(
        0,
        UI_device_height - MMI_BUTTON_BAR_HEIGHT,
        UI_device_width - 1,
        UI_device_height - 1,
        GDI_COLOR_WHITE);
}
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 

