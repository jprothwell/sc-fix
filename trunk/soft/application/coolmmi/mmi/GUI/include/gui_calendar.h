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
 *   gui_calendar.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  calendar related funtions
 * Author:
 * -------
 * -------
 *------------------------------------------------------------------------------
 * $Log$
 * 
 */

/**********************************************************************************
   Filename:      gui_calendar.h
   Date Created:  July-07-2003
   Contains:      Contains the calendar control
 
**********************************************************************************/

#include "mmi_features.h"
#include "gui.h"

#ifndef __GUI_CALENDAR_H__
#define __GUI_CALENDAR_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

#ifdef __MMI_TOUCH_SCREEN__

#include "kal_non_specific_general_types.h"
#include "touchscreengprot.h"

    typedef enum
    {
        // TODO: some events are useless
        GUI_CALENDAR_PEN_NONE,
        GUI_CALENDAR_PEN_HIGHLIGHT_CHANGED,
        GUI_CALENDAR_PEN_NEED_REDRAW,
        GUI_CALENDAR_PEN_ITEM_SELECTED
    } gui_calendar_pen_enum;

#endif /* __MMI_TOUCH_SCREEN__ */ 

#define ENABLE_CALENDAR_CONTROL     1

#if(ENABLE_CALENDAR_CONTROL)

    /* Externals used by the calendar            */
    extern void _show_bitmap(S32 x1, S32 y1, bitmap *b);
    extern void _get_bitmap(S32 x1, S32 y1, S32 x2, S32 y2, bitmap *b);

    /* Internal flags used by calendar control      */
#define MAX_CALENDAR_DAYS           32
#define CALENDAR_SUNDAY             0
#define  CALENDAR_MONDAY               1
#define CALENDAR_TUESDAY            2
#define CALENDAR_WEDNESDAY          3
#define CALENDAR_THURSDAY           4
#define CALENDAR_FRIDAY             5
#define CALENDAR_SATURDAY           6
#define CALENDAR_WEEK_START            CALENDAR_SUNDAY
#define CALENDAR_WEEK_END           CALENDAR_SATURDAY

    /* Flags used by the list of items (days)    */
#define  CALENDAR_NORMAL_DAY           0x00
#define CALENDAR_TASK_DAY           0x01
#define CALENDAR_CURRENT_DAY        0x02
#define CALENDAR_SPECIAL_DAY1       0x03
#define CALENDAR_SPECIAL_DAY2       0x04
#define CALENDAR_SPECIAL_DAY3       0x05
#define CALENDAR_SPECIAL_DAY4       0x06
#define CALENDAR_POPUP_DAY          0x80

    /* Flags used by the calendar control        */
#define CALENDAR_HIGHLIGHT_BACKUP      0x00000001
#define CALENDAR_POPUP_BACKUP       0x00000002
#define CALENDAR_ENABLE_POPUP       0x00000004
#define CALENDAR_HIDE_POPUP            0x00000008
#define CALENDAR_SHOW_INFO_BOX            0x00000010

    /* Theme structure for the calendar control  */
    typedef struct _calendar_theme
    {
        UI_font_type text_font;
        color_t normal_day_text_color;
        color_t task_day_text_color;
        color_t current_day_text_color;
        color_t selected_day_text_color;
        color_t week_start_day_text_color;
        color_t week_end_day_text_color;
        color_t special1_day_text_color;
        color_t special2_day_text_color;
        color_t special3_day_text_color;
        color_t special4_day_text_color;
        UI_filled_area *blank_day_filler;
        UI_filled_area *normal_day_filler;
        UI_filled_area *task_day_filler;
        UI_filled_area *current_day_filler;
        UI_filled_area *selected_day_filler;
        UI_filled_area *week_start_day_filler;
        UI_filled_area *week_end_day_filler;
        UI_filled_area *special1_day_filler;
        UI_filled_area *special2_day_filler;
        UI_filled_area *special3_day_filler;
        UI_filled_area *special4_day_filler;

    } calendar_theme;

    /* calendar control structure             */

    typedef struct _calendar_control
    {
        S32 x, y, x1, y1, x2, y2;
        S32 px1, py1, px2, py2;
        S32 cell_width, cell_height;
        S32 n_rows, n_columns;
        S32 start_column, end_column;
        S32 first_item, last_item;
        S32 highlighted_item;
        U32 flags;
        calendar_theme *theme;
        void (*month_previous_day_function) (void);
        void (*month_next_day_function) (void);
        void (*month_previous_week_function) (void);
        void (*month_next_week_function) (void);
    #ifdef __MMI_TOUCH_SCREEN__
        void (*previous_month_function) (void);
        void (*next_month_function) (void);
    #ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
        void (*previous_year_function) (void);
        void (*next_year_function) (void);
    #endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
    #endif /* __MMI_TOUCH_SCREEN__ */ 
        void (*popuptitle_lunarcalendar) (U8 u1, U8 u2, U8 u3, U8 u4);
        void (*draw_background_function) (S32 x1, S32 y1, S32 x2, S32 y2);
        void (*item_unhighlighted_function) (S32 index);
        void (*item_highlighted_function) (S32 index);
        void (*display_popup) (S32 day, S32 *x1, S32 *y1, S32 *x2, S32 *y2);
    #ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
        /* set by draw manager */
        S32 info_x1, info_y1, info_x2, info_y2;
        S32 max_n_info;
        /* set by application */
        S32 n_info;
        U16 *icon_list;
        U8 **description_list;
        void (*update_info_box) (S32 *p_n_info, U16 **p_icon_list, U8 ***p_description_list);
    #endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
        bitmap cursor_save_bitmap;
        bitmap popup_save_bitmap;
        U8 *list_of_items;
    #ifdef __MMI_TOUCH_SCREEN__
        S32 pen_down_on_highlighted_item;
    #endif 
    } calendar_control;

    extern void gui_create_calendar(
                    calendar_control *c,
                    S32 x,
                    S32 y,
                    S32 n_columns,
                    S32 n_rows,
                    S32 cell_width,
                    S32 cell_height);
    void gui_set_calendar_background_function(calendar_control *c, void (*f) (S32 x1, S32 y1, S32 x2, S32 y2));
    extern void gui_set_calendar_days(calendar_control *c, S32 first_dow, S32 last_day, U8 *list_of_items);
    extern void gui_show_calendar(calendar_control *c);
    extern void gui_calendar_refresh_highlight(
                    calendar_control *c,
                    S32 old_highlighted_item,
                    S32 new_highlighted_item);
    extern void gui_calendar_highlight_day(calendar_control *c, S32 day);
    extern void gui_calendar_previous_day(calendar_control *c);
    extern void gui_calendar_next_day(calendar_control *c);
    extern void gui_calendar_next_row(calendar_control *c);
    extern void gui_calendar_previous_row(calendar_control *c);
    extern void gui_move_calendar(calendar_control *c, S32 x, S32 y);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    extern void gui_calendar_draw_info_box(calendar_control *c);
#endif 

#ifdef __MMI_TOUCH_SCREEN__
    extern void gui_calendar_go_to_day(calendar_control *c, U8 index);
    extern BOOL gui_calendar_translate_pen_event(
                    calendar_control *c,
                    mmi_pen_event_type_enum pen_event,
                    S16 x,
                    S16 y,
                    gui_calendar_pen_enum *calendar_event);
    extern void gui_calendar_translate_pen_position(calendar_control *c, S32 x, S32 y, S32 *item_index);
#endif /* __MMI_TOUCH_SCREEN__ */ 
#endif /* (ENABLE_CALENDAR_CONTROL) */ 

#ifdef __cplusplus
}
#endif 

#endif /* __GUI_CALENDAR_H__ */ 

