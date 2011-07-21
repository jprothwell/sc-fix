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
 *   wgui_softkeys.h
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
 */

/**********************************************************************************
   Filename:      wgui_softkeys.h
   Date Created:  August-07-2002
   Contains:      UI wrapper routines header for softkeys
**********************************************************************************/

#ifndef __WGUI_SOFTKEYS_H__
#define __WGUI_SOFTKEYS_H__

#include "mmi_features.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

#include "wgui.h"

#define  MMI_left_softkey  MMI_softkeys[MMI_LEFT_SOFTKEY]
#define MMI_right_softkey  MMI_softkeys[MMI_RIGHT_SOFTKEY]

#ifdef __MMI_WGUI_CSK_ENABLE__
#define  MMI_center_softkey   MMI_softkeys[MMI_CENTER_SOFTKEY]
#endif 
#define WGUI_SOFTKEY_STRING_GAP  4
    typedef enum
    {
        MMI_LEFT_SOFTKEY,
        MMI_RIGHT_SOFTKEY,
    #ifdef __MMI_WGUI_CSK_ENABLE__
        MMI_CENTER_SOFTKEY,
    #endif 
        WGUI_MAX_SOFTKEYS
    } WGUI_SOFTKEY_ENUM;

    extern S32 MMI_softkey_width;
    extern S32 MMI_softkey_height;

    extern U8 left_softkey_keyboard_handler_active;
    extern U8 right_softkey_keyboard_handler_active;
#ifdef __MMI_GUI_CSK_ENABLE__
    extern U8 center_softkey_keyboard_handler_active;
#endif 

#define resize_left_softkey_by_string_length( S )  resize_softkey_by_string_length(S,MMI_LEFT_SOFTKEY)
#define resize_right_softkey_by_string_length( S ) resize_softkey_by_string_length(S,MMI_RIGHT_SOFTKEY)
#define resize_left_softkey(WIDTH, HEIGHT )     resize_softkey(WIDTH, HEIGHT,MMI_LEFT_SOFTKEY)
#define resize_right_softkey( WIDTH, HEIGHT )      resize_softkey(WIDTH, HEIGHT,MMI_RIGHT_SOFTKEY)
#define set_left_softkey_label( S )             set_softkey_label(S,MMI_LEFT_SOFTKEY)
#define set_right_softkey_label( S )            set_softkey_label(S,MMI_RIGHT_SOFTKEY)
#define set_left_softkey_icon( ICON )           set_softkey_icon(ICON,MMI_LEFT_SOFTKEY)
#define set_right_softkey_icon( ICON )          set_softkey_icon(ICON,MMI_RIGHT_SOFTKEY)
#define change_left_softkey(S,I)             change_softkey(S,I,MMI_LEFT_SOFTKEY)
#define change_right_softkey(S,I)               change_softkey(S,I,MMI_RIGHT_SOFTKEY)
#define set_left_softkey_function(FUNC, KEY_TYPE)  set_softkey_function(FUNC,KEY_TYPE,MMI_LEFT_SOFTKEY)
#define set_right_softkey_function(FUNC, KEY_TYPE) set_softkey_function(FUNC,KEY_TYPE,MMI_RIGHT_SOFTKEY)
#define execute_left_softkey_function(KEY_TYPE) execute_softkey_function(KEY_TYPE,MMI_LEFT_SOFTKEY)
#define execute_right_softkey_function(KEY_TYPE)   execute_softkey_function(KEY_TYPE,MMI_RIGHT_SOFTKEY)
#define set_left_softkey(S, FUNC, KEY_TYPE)     set_softkey(S, FUNC, KEY_TYPE, MMI_LEFT_SOFTKEY)
#define set_right_softkey(S, FUNC, KEY_TYPE)    set_softkey(S, FUNC, KEY_TYPE, MMI_RIGHT_SOFTKEY)
#define clear_left_softkey()                 clear_softkey_display_functions(MMI_LEFT_SOFTKEY)
#define clear_right_softkey()                clear_softkey_display_functions(MMI_RIGHT_SOFTKEY)
#define show_left_softkey()                  show_softkey(MMI_LEFT_SOFTKEY)
#define show_right_softkey()                 show_softkey(MMI_RIGHT_SOFTKEY)
#define redraw_left_softkey()                redraw_softkey(MMI_LEFT_SOFTKEY)
#define redraw_right_softkey()               redraw_softkey(MMI_RIGHT_SOFTKEY)
#ifdef __MMI_WGUI_CSK_ENABLE__ //082106 redraw ceter key
#define redraw_center_softkey()               redraw_softkey(MMI_CENTER_SOFTKEY)
#endif
#define register_hide_left_softkey(FUNC)        register_hide_softkey(FUNC, MMI_LEFT_SOFTKEY)
#define register_hide_right_softkey(FUNC)       register_hide_softkey(FUNC, MMI_RIGHT_SOFTKEY)
#define register_left_softkey_handler()         register_softkey_handler(MMI_LEFT_SOFTKEY)
#define register_right_softkey_handler()        register_softkey_handler(MMI_RIGHT_SOFTKEY)
#define clear_left_softkey_handler()            clear_softkey_handler(MMI_LEFT_SOFTKEY)
#define clear_right_softkey_handler()           clear_softkey_handler(MMI_RIGHT_SOFTKEY)
#define reset_left_softkey()                 reset_softkey(MMI_LEFT_SOFTKEY)
#define reset_right_softkey()                reset_softkey(MMI_RIGHT_SOFTKEY)

    extern void create_softkeys(void);
    extern void register_softkey_handler(WGUI_SOFTKEY_ENUM key);
    extern void register_center_softkey_to_enter_key(void);

    extern void set_softkey_label(UI_string_type s, WGUI_SOFTKEY_ENUM key);
    extern void set_softkey_icon(PU8 i, WGUI_SOFTKEY_ENUM key);
    extern void change_softkey(U16 s, U16 i, WGUI_SOFTKEY_ENUM key);
    void set_softkey_function(void (*f) (void), MMI_key_event_type k, WGUI_SOFTKEY_ENUM key);
    extern void execute_softkey_function(MMI_key_event_type k, WGUI_SOFTKEY_ENUM key);
    extern FuncPtr get_softkey_function(MMI_key_event_type k, WGUI_SOFTKEY_ENUM key);
    void set_softkey_function(void (*f) (void), MMI_key_event_type k, WGUI_SOFTKEY_ENUM key);
    extern void show_softkey(WGUI_SOFTKEY_ENUM key);
    extern void redraw_softkey(WGUI_SOFTKEY_ENUM key);
    void register_hide_softkey(void (*f) (void), WGUI_SOFTKEY_ENUM key);
    extern void clear_softkey_display_functions(WGUI_SOFTKEY_ENUM key);
    extern void resize_softkey_by_string_length(UI_string_type s, WGUI_SOFTKEY_ENUM key);
    extern void left_softkey_down(void);
    extern void left_softkey_up(void);
    extern void right_softkey_down(void);
    extern void right_softkey_up(void);
    extern void update_softkey_state_no_redraw( U16 KeyCode, U16 KeyType);
    extern void reset_softkeys(void);
    extern void reset_softkey(WGUI_SOFTKEY_ENUM key);
    extern void clear_softkey_handler(WGUI_SOFTKEY_ENUM key);
    extern void enable_softkey_background(void);
    extern void disable_softkey_background(void);

    /* For Draw Manager  */
    extern void move_softkey(S32 x, S32 y, WGUI_SOFTKEY_ENUM key);
    extern void resize_softkey(S32 width, S32 height, WGUI_SOFTKEY_ENUM key);

    extern void restore_softkeys(void);
    extern void set_softkey_flags_on(S32 flag, WGUI_SOFTKEY_ENUM key);
    extern void set_softkey_flags_off(S32 flag, WGUI_SOFTKEY_ENUM key);

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    void gui_register_button_shuffle(void (*f) (void));
    extern void gui_call_shuffle(void);
    extern void shuffle_draw_white(void);
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 

#ifdef __MMI_TOUCH_SCREEN__
    extern BOOL translate_softkey_pen_event(
                    WGUI_SOFTKEY_ENUM key,
                    mmi_pen_event_type_enum pen_event,
                    S16 x,
                    S16 y,
                    gui_button_pen_enum *button_event);
#endif /* __MMI_TOUCH_SCREEN__ */ 

    extern void wgui_softkey_get_dimension(WGUI_SOFTKEY_ENUM key, S32 *width, S32 *height);
    extern void wgui_softkey_get_position(WGUI_SOFTKEY_ENUM key, S32 *x, S32 *y);

#ifdef __cplusplus
}
#endif 

#endif /* __WGUI_SOFTKEYS_H__ */ 

