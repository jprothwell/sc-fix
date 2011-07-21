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
 *   wgui_menu_shortcuts.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  shortcut related funtions
 * Author:
 * -------
 * -------
 *------------------------------------------------------------------------------
 * $Log$
 * 
 * 
 * 
 */

/**********************************************************************************
   Filename:      wgui_menu_shortcuts.h
   Date Created:  August-07-2002
   Contains:      UI wrapper routines header for Menu Shortcuts
**********************************************************************************/

#ifndef __WGUI_MENU_SHORTCUTS_H__
#define __WGUI_MENU_SHORTCUTS_H__

#include "wgui.h"
#include "gui_theme_struct.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

    /* Menu shortcut wrappers              */
#define MMI_MENU_SHORTCUT_BUFFER_LENGTH            8
    extern single_line_input_box MMI_menu_shortcut_box;
    extern U8 menu_shortcut_handler_display;
    extern U8 disable_menu_shortcut_box_display;
    extern UI_shortcut_box_theme *current_shortcut_box_theme;

    extern void menu_shortcut_handler_timeout(void);
    extern void menu_shortcut_handler_keyinput(UI_character_type c);
    extern void create_menu_shortcut_handler(void);
    extern void menu_shortcut_keyboard_input_handler(S32 keyc);
    extern void register_menu_shortcut_keys(void);
    void register_menu_shortcut_handler(void (*f) (S32 i));
    void register_menu_shortcut_selected(void (*f) (S32 i));
    void register_menu_shortcut_selected_only(void (*f) (S32 i));

    extern void move_menu_shortcut_handler(S32 x, S32 y);
    extern void resize_menu_shortcut_handler(S32 width, S32 height);
    extern void override_menu_shortcut_default_region(S32 x, S32 y, S32 width, S32 height);

    void register_hide_menu_shortcut(void (*f) (void));
    extern void set_menu_shortcut_length(S32 l);
    extern void reset_menu_shortcut_handler(void);

    extern S32 set_menu_item_count(S32 n_items);
    extern void gui_hide_menu_shortcut(void);
    extern void gui_redraw_menu_shortcut(void);
    extern void show_menu_shortcut(void);

#ifdef __cplusplus
}
#endif 

#endif /* __WGUI_MENU_SHORTCUTS_H__ */ 

