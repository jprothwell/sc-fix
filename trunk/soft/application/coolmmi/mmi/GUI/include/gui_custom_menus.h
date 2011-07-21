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
 *   gui_custom_menus.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Customized menu - UI component
 *   [3D circular menu]
 * Author:
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
   Filename:      gui_custom_menus.h
   Date Created:  Mar-07-2003
   Contains:      UI routines header for Custom Menus
**********************************************************************************/

/* 3D circular menu  */
#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)	&& !defined(__MMI_MAINLCD_128X160__)
#define ENABLE_CIRCULAR_3D_MENU     1
#define ENABLE_ROTATE_MENU    1
#else
#define ENABLE_CIRCULAR_3D_MENU     0
#define ENABLE_ROTATE_MENU    0
#endif
#ifndef __GUI_CUSTOM_MENUS_H__
#define __GUI_CUSTOM_MENUS_H__

#include "gui.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

#ifdef __MMI_TOUCH_SCREEN__
#include "kal_non_specific_general_types.h"
#include "touchscreengprot.h"

    typedef enum
    {
        GUI_3D_CIRCULAR_MENU_PEN_NONE,
        GUI_3D_CIRCULAR_MENU_PEN_GOTO_NEXT,
        GUI_3D_CIRCULAR_MENU_PEN_GOTO_PREVIOUS,
        GUI_3D_CIRCULAR_MENU_PEN_ITEM_SELECTED
    } gui_3D_circular_menu_pen_enum;

#endif /* __MMI_TOUCH_SCREEN__ */ 

#if((ENABLE_CIRCULAR_3D_MENU) || (ENABLE_ROTATE_MENU))

#define  UI_3D_CIRCULAR_MENU_DISABLE_BACKGROUND 0x00000001
#define UI_3D_CIRCULAR_MENU_TRANSITION_L1    0x00000010
#define UI_3D_CIRCULAR_MENU_TRANSITION_L2    0x00000020
#define UI_3D_CIRCULAR_MENU_TRANSITION_R1    0x00000040
#define UI_3D_CIRCULAR_MENU_TRANSITION_R2    0x00000080

#define MAX_3D_CIRCULAR_MENU_DISPLAYED_ITEMS 10

    typedef struct _UI_3D_circular_menu_theme
    {
        UI_filled_area *bg_filler;
        color_t text_color;
        color_t text_border;
        UI_font_type text_font;
        U32 flags;
    } UI_3D_circular_menu_theme;

    extern UI_3D_circular_menu_theme *current_3D_circular_menu_theme;

    typedef struct _CM3D_item_position
    {
        S32 x, y;
    } CM3D_item_position;

    typedef struct _UI_3D_circular_menu
    {
        S32 x, y;
        S32 width, height;
        UI_filled_area *bg_filler;
        color_t text_color;
        color_t text_border;
        UI_font_type text_font;
        U32 flags;
        S32 n_items;
        S32 highlighted_item;
        S32 item_to_highlight;
        S32 n_displayed_items;
        S32 n_item_images;
        PU8 *item_images;
        PU8 *common_images;
        CM3D_item_position *item_positions;
        UI_string_type *item_strings;
        void (*item_unhighlighted) (S32 i);
        void (*item_highlighted) (S32 i);
    #ifdef __MMI_TOUCH_SCREEN__
        BOOL pen_down_on_focused_item;
    #endif 
    } UI_3D_circular_menu;

    extern void gui_set_3D_circular_menu_current_theme(UI_3D_circular_menu *m);
    extern void gui_set_3D_circular_menu_theme(UI_3D_circular_menu *m, UI_3D_circular_menu_theme *t);
    extern void gui_create_3D_circular_menu(
                    UI_3D_circular_menu *m,
                    S32 x,
                    S32 y,
                    S32 width,
                    S32 height,
                    S32 n_displayed_items,
                    CM3D_item_position *item_positions,
                    S32 n_item_images,
                    PU8 *item_images,
                    PU8 *common_images);
    extern void gui_set_3D_circular_menu_items(UI_3D_circular_menu *m, S32 n_items, UI_string_type *items);
    extern void gui_resize_3D_circular_menu(UI_3D_circular_menu *m, S32 width, S32 height);
    extern void gui_move_3D_circular_menu(UI_3D_circular_menu *m, S32 x, S32 y);
    extern void gui_3D_circular_menu_goto_next_item(UI_3D_circular_menu *m);
    extern void gui_3D_circular_menu_goto_previous_item(UI_3D_circular_menu *m);
    extern void gui_3D_circular_menu_goto_previous_row(UI_3D_circular_menu *m);
    extern void gui_3D_circular_menu_goto_next_row(UI_3D_circular_menu *m);
    extern void gui_3D_circular_menu_goto_previous_column(UI_3D_circular_menu *m);
    extern void gui_3D_circular_menu_goto_next_column(UI_3D_circular_menu *m);
    extern void gui_3D_circular_menu_goto_item(UI_3D_circular_menu *m, S32 i);
    extern void gui_show_3D_circular_menu(UI_3D_circular_menu *m);
#ifdef __MMI_TOUCH_SCREEN__
    extern BOOL gui_3D_circular_menu_translate_pen_event(
                    UI_3D_circular_menu *m,
                    mmi_pen_event_type_enum pen_event,
                    S16 x,
                    S16 y,
                    gui_3D_circular_menu_pen_enum *menu_event);
#endif /* __MMI_TOUCH_SCREEN__ */ 

#endif /* ((ENABLE_CIRCULAR_3D_MENU) || (ENABLE_ROTATE_MENU)) */ 

#ifdef __cplusplus
}
#endif 

#endif /* __GUI_CUSTOM_MENUS_H__ */ 

