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
 *   gui_generic_menus.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Generic menu - UI component
 *   (deprecated & unused)
 * Author:
 * -------
 * -------
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
   Filename:      gui_generic_menus.h
   Date Created:  August-13-2002
   Contains:      UI routines header for Generic Menuitems
**********************************************************************************/

#ifndef __GUI_GENERIC_MENUS_H__
#define __GUI_GENERIC_MENUS_H__

#include "gui.h"
#include "gui_scrollbars.h"
#include "gui_common_menus.h"
 
#include "gui_theme_struct.h"
 
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

    /* UI generic menu item structure               */

    typedef struct _UI_generic_menuitem
    {
        void *item;
        void (*display_function) (void *item, S32 x, S32 y);
        void (*measure_function) (void *item, S32 *x, S32 *y, S32 *width, S32 *height);
        void (*highlight_function) (void *item);
        void (*remove_highlight_function) (void *item);
    } UI_generic_menuitem;

    extern void gui_create_generic_menuitem(UI_generic_menuitem *m, void *item);
    void gui_set_generic_menuitem_display_function(UI_generic_menuitem *m,
                                                   void (*display_function) (void *item, S32 x, S32 y));
    void gui_set_generic_menuitem_measure_function(UI_generic_menuitem *m,
                                                   void (*measure_function) (void *item, S32 *x, S32 *y, S32 *width,
                                                                             S32 *height));
    void gui_set_generic_menuitem_highlight_function(UI_generic_menuitem *m, void (*highlight_function) (void *item));
    void gui_set_generic_menuitem_remove_highlight_function(UI_generic_menuitem *m,
                                                            void (*remove_highlight_function) (void *item));

    /* UI List menu structure                    */
    extern UI_list_menu_theme *current_list_menu_theme;

    typedef struct _list_menu
    {
        S32 x, y;
        S32 width, height;
        UI_filled_area *normal_filler;
        UI_filled_area *focussed_filler;
        U32 flags;
        vertical_scrollbar vbar;
        S32 n_items;
        S32 displayed_items;
        S32 first_displayed_item;
        S32 last_displayed_item;
        S32 highlighted_item;
        UI_generic_menuitem *items;
        void (*item_unhighlighted) (S32 i);
        void (*item_highlighted) (S32 i);
        void (*item_selected) (S32 i);
        void (*item_unselected) (S32 i);
    } list_menu;

    extern void gui_set_list_menu_current_theme(list_menu *m);
    extern void gui_set_list_menu_theme(list_menu *m, UI_list_menu_theme *t);
    extern void gui_create_list_menu(list_menu *m, S32 x, S32 y, S32 width, S32 height);
    extern void gui_resize_list_menu(list_menu *m, S32 width, S32 height);
    extern void gui_move_list_menu(list_menu *m, S32 x, S32 y);
    extern void gui_list_menu_locate_highlighted_item(list_menu *m);
    extern void gui_list_menu_goto_item(list_menu *m, S32 i);
    extern void gui_list_menu_goto_next_item(list_menu *m);
    extern void gui_list_menu_goto_previous_item(list_menu *m);
    extern void gui_list_menu_goto_next_page(list_menu *m);
    extern void gui_list_menu_goto_previous_page(list_menu *m);
    extern void gui_list_menu_goto_first_item(list_menu *m);
    extern void gui_list_menu_goto_last_item(list_menu *m);
    extern void gui_show_list_menu(list_menu *m);

    /* UI matrix menu structure                  */
    extern UI_matrix_menu_theme *current_matrix_menu_theme;

    typedef struct _matrix_menu
    {
        S32 x, y;
        S32 width, height;
        UI_filled_area *normal_filler;
        UI_filled_area *focussed_filler;
        U32 flags;
        vertical_scrollbar vbar;
        horizontal_scrollbar hbar;
        S32 n_columns;
        S32 n_rows;
        S32 n_items;
        S32 first_displayed_column;
        S32 first_displayed_row;
        S32 displayed_columns;
        S32 displayed_rows;
        S32 highlighted_column;
        S32 highlighted_row;
        S32 highlighted_item;
        S32 item_width;
        S32 item_height;
        UI_generic_menuitem *items;
        void (*item_unhighlighted) (S32 i);
        void (*item_highlighted) (S32 i);
        void (*item_selected) (S32 i);
        void (*item_unselected) (S32 i);
    } matrix_menu;

    extern void gui_set_matrix_menu_current_theme(matrix_menu *m);
    extern void gui_set_matrix_menu_theme(matrix_menu *m, UI_matrix_menu_theme *t);
    extern void gui_create_matrix_menu(matrix_menu *m, S32 x, S32 y, S32 width, S32 height);
    extern void gui_matrix_menu_auto_configure(matrix_menu *m);
    extern void gui_matrix_menu_setup(matrix_menu *m, S32 item_width, S32 item_height, S32 n_columns, S32 n_rows);
    extern void gui_resize_matrix_menu(matrix_menu *m, S32 width, S32 height);
    extern void gui_move_matrix_menu(matrix_menu *m, S32 x, S32 y);
    extern void gui_matrix_menu_locate_highlighted_item(matrix_menu *m);
    extern void gui_matrix_menu_goto_next_item(matrix_menu *m);
    extern void gui_matrix_menu_goto_previous_item(matrix_menu *m);
    extern void gui_matrix_menu_goto_previous_row(matrix_menu *m);
    extern void gui_matrix_menu_goto_next_row(matrix_menu *m);
    extern void gui_matrix_menu_goto_previous_column(matrix_menu *m);
    extern void gui_matrix_menu_goto_next_column(matrix_menu *m);
    extern void gui_matrix_menu_goto_first_item(matrix_menu *m);
    extern void gui_matrix_menu_goto_last_item(matrix_menu *m);
    extern void gui_matrix_menu_goto_next_page(matrix_menu *m);
    extern void gui_matrix_menu_goto_previous_page(matrix_menu *m);
    extern void gui_matrix_menu_goto_item(matrix_menu *m, S32 i);
    extern void gui_show_matrix_menu(matrix_menu *m);

#ifdef __cplusplus
}
#endif 

#endif /* __GUI_GENERIC_MENUS_H__ */ 

