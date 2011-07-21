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
 *   wgui_tab_bars.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Tab Bar - Category component
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _WGUI_TAB_BARS_H
#define _WGUI_TAB_BARS_H

#include "wgui.h"
#include "gui_tab_bars.h"

/***************************************************************************** 
* Define
*****************************************************************************/
#if defined(__MMI_MAINLCD_220X176__)

#define __MMI_WGUI_MINI_TAB_BAR__
#endif

#define UI_MAX_TAB_BAR_ITEMS     20

#ifdef __MMI_WGUI_MINI_TAB_BAR__

#define MAX_MINI_TAB_BAR_ITEMS   16
#define WGUI_MINI_TAB_BAR_ENABLED   0x00000001
#define WGUI_MINI_TAB_BAR_IN_PARENT 0x00000002

#endif /* __MMI_WGUI_MINI_TAB_BAR__ */ 

/***************************************************************************** 
* Typedef 
*****************************************************************************/

typedef struct
{
    U32 flags;
    S8 first_displayed_item;
    S8 last_displayed_item;
    S8 focused_item;
} wgui_tab_bar_history_struct;

#ifdef __MMI_WGUI_MINI_TAB_BAR__

typedef struct _MINI_TAB_BAR_STRUCT
{
    S32 x, y;                                           /* coordinates */
    S32 width, height;                                  /* size */
    color_t bg_color;                                     /* background color */
    color_t selected_item_color;                          /* selected item color */
    color_t normal_item_color;                            /* normal item color */
    U8 num_of_items;                                    /* Number of items in tab */
    U8 curr_index;                                      /* currently highlighted item index (start from 0) */
    U16 menu_item_id[MAX_MINI_TAB_BAR_ITEMS];           /* ID of each menuitem in tab */
    FuncPtr item_entry_func[MAX_MINI_TAB_BAR_ITEMS];    /* Entry function of each menuitem in tab */
    S32 flags;                                          /* Information & state flag */
} gui_mini_tab_bar_struct;

#endif /* __MMI_WGUI_MINI_TAB_BAR__ */ 

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

extern horizontal_tab_bar MMI_horizontal_tab_bar;
extern tab_bar_item_type MMI_tab_bar_items[UI_MAX_TAB_BAR_ITEMS];

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void wgui_create_horizontal_tab_bar(
                BOOL contain_hint,
                UI_string_type hint,
                S8 n_item,
                S8 focused_item,
                MMI_BOOL keep_pen_context);
extern void wgui_close_horizontal_tab_bar(void);
extern void wgui_show_horizontal_tab_bar(void);
extern void wgui_show_horizontal_tab_bar_hint_area(void);
extern void wgui_show_horizontal_tab_bar_tab_area(void);
extern void wgui_move_horizontal_tab_bar(S32 x, S32 y);

extern void wgui_horizontal_tab_bar_goto_home(void);
extern void wgui_horizontal_tab_bar_goto_end(void);
extern void wgui_horizontal_tab_bar_goto_item(int index);
extern void wgui_horizontal_tab_bar_goto_next_item(void);
extern void wgui_horizontal_tab_bar_goto_prev_item(void);

extern void wgui_horizontal_tab_bar_start_blinking(int index);
extern void wgui_horizontal_tab_bar_stop_blinking(int index);

extern void wgui_get_horizontal_tab_bar_history(wgui_tab_bar_history_struct *history);
extern void wgui_set_horizontal_tab_bar_history(wgui_tab_bar_history_struct *history);

void wgui_set_horizontal_tab_bar_select_callback(void (*select_callback) (int index));

#ifdef __MMI_TOUCH_SCREEN__
extern void wgui_enable_horizontal_tab_bar_pen_events(BOOL enable);
extern BOOL wgui_horizontal_tab_bar_translate_pen_event(mmi_pen_event_type_enum pen_event, S16 x, S16 y);
#endif /* __MMI_TOUCH_SCREEN__ */ 

#ifdef __MMI_WGUI_MINI_TAB_BAR__

extern void wgui_init_mini_tab_bar(void);
extern void wgui_setup_mini_tab_bar(
                S32 x,
                S32 y,
                S32 width,
                S32 height,
                color_t bg_color,
                color_t selected_color,
                color_t normal_color,
                U8 count,
                U16 *menu_item_id,
                FuncPtr *item_entry_func);
extern MMI_BOOL wgui_if_mini_tab_bar_enable(void);
extern MMI_BOOL wgui_if_mini_tab_bar_in_parent(void);
extern void wgui_enable_mini_tab_bar(U16 item_id);
extern void wgui_disable_mini_tab_bar(void);
extern void wgui_mini_tab_bar_set_flag_on(S32 flag);
extern void wgui_mini_tab_bar_set_flag_off(S32 flag);
extern void wgui_register_mini_tab_bar_func(U16 *menu_item_id, FuncPtr item_entry_func[]);
extern void wgui_mini_tab_bar_goto_next(void);
extern void wgui_mini_tab_bar_goto_prev(void);
extern void wgui_show_mini_tab_bar(void);
extern U8 wgui_mini_tab_bar_get_index(void);

#endif /* __MMI_WGUI_MINI_TAB_BAR__ */ 

#endif /* _WGUI_TAB_BARS_H */ 

