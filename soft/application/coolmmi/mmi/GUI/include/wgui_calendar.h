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
 *  wgui_calendar.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Calendar related categories.
 *  Author:
 * -------
 
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 
/**********************************************************************************
   Filename:      wgui_calendar.h
   Date Created:  July-07-2003
   Contains:      Contains the calendar control wrappers and category screens
**********************************************************************************/

#ifndef __WGUI_CALENDAR_H__
#define __WGUI_CALENDAR_H__

#include "wgui.h"
#include "gui_calendar.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

#if(ENABLE_CALENDAR_CONTROL)

    /* Wrappers to be used by applications and category screens */

    extern calendar_control MMI_calendar_control;
    extern U8 MMI_calendar_days[MAX_CALENDAR_DAYS];
    extern S32 MMI_calendar_highlighted_day;
    extern void (*MMI_calendar_control_highlight_handler) (S32 day);

#define  CLNDR_HINT_LEN             8*ENCODING_LENGTH
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
#define  CLNDR_TITLE_LEN            17*ENCODING_LENGTH
#else 
#define  CLNDR_TITLE_LEN            12*ENCODING_LENGTH
#endif 

#define  CLNDR_MAX_DAYS             31
#define  CLNDR_LAST_NAVI_YEAR       2030
#define  CLNDR_FIRST_NAVI_YEAR      1970

#define CALENDAR_N_COLUMNS          7
#ifdef __MMI_MAINLCD_160X128__
#define CALENDAR_N_ROWS             7
#else
#define CALENDAR_N_ROWS             6
#endif
#define  CALENDAR_CELL_WIDTH           18
#ifdef __MMI_MAINLCD_160X128__
#define CALENDAR_CELL_HEIGHT        11
#else
#define CALENDAR_CELL_HEIGHT        13
#endif
#define CALENDAR_X                  1
#define CALENDAR_Y                  28

#define   NEXT_DAY                  0
#define   PREV_DAY                  1
#define   NEXT_ROW                  2
#define   PREV_ROW                  3
#define   SELECTED_DAY              4
#ifdef __MMI_MAINLCD_160X128__
#define CALENDAR_DOW_TITLE_HEIGHT      13
#define CALENDAR_LUNAR_TITLE_HEIGHT    13
#else
#define CALENDAR_DOW_TITLE_HEIGHT      14
#define CALENDAR_LUNAR_TITLE_HEIGHT    15
#endif
#define CALENDAR_LUNAR_SYMBOL_WIDTH    20

#if defined (__MMI_MAINLCD_240X320__)
#define MMI_CALENDAR_X              5
#define MMI_CALENDAR_Y              62
#define MMI_CALENDAR_WIDTH       231
#define MMI_CALENDAR_HEIGHT         156
#define MMI_CALENDAR_LIMIT_Y        41
#define MMI_CALENDAR_LIMIT_HEIGHT      MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_CALENDAR_LIMIT_Y
#elif defined (__MMI_MAINLCD_176X220__)
#define MMI_CALENDAR_X              12
#define MMI_CALENDAR_Y              70
#define MMI_CALENDAR_WIDTH       154
#define MMI_CALENDAR_HEIGHT         108
#define MMI_CALENDAR_LIMIT_Y        55
#define MMI_CALENDAR_LIMIT_HEIGHT      MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_CALENDAR_LIMIT_Y
#elif defined (__MMI_MAINLCD_220X176__)
#define MMI_CALENDAR_X              10
#define MMI_CALENDAR_Y              70
#define MMI_CALENDAR_WIDTH       200
#define MMI_CALENDAR_HEIGHT         88
#define MMI_CALENDAR_LIMIT_Y        55
#define MMI_CALENDAR_LIMIT_HEIGHT      MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_CALENDAR_LIMIT_Y
#elif defined (__MMI_MAINLCD_128X160__)
#define MMI_CALENDAR_X              1
#define MMI_CALENDAR_Y              35
#define MMI_CALENDAR_WIDTH       126
#define MMI_CALENDAR_HEIGHT         91
#define MMI_CALENDAR_LIMIT_Y        17
#define MMI_CALENDAR_LIMIT_HEIGHT      MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_CALENDAR_LIMIT_Y
#elif defined (__MMI_MAINLCD_160X128__)
#ifdef __HORIZONTAL_LCD__
#define MMI_CALENDAR_X              33
#define MMI_CALENDAR_Y              35
#define MMI_CALENDAR_WIDTH       126
#define MMI_CALENDAR_HEIGHT         91
#define MMI_CALENDAR_LIMIT_Y        17
#else
#define MMI_CALENDAR_X              1
#define MMI_CALENDAR_Y              35
#define MMI_CALENDAR_WIDTH       126
#define MMI_CALENDAR_HEIGHT         91
#if defined(__MMI_MAINLCD_160X128__)
#define MMI_CALENDAR_LIMIT_Y        15
#else
#define MMI_CALENDAR_LIMIT_Y        17
#endif
#endif
#define MMI_CALENDAR_LIMIT_HEIGHT      MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_CALENDAR_LIMIT_Y
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#elif defined (__MMI_MAINLCD_320X240__)
#define MMI_CALENDAR_X              25
#define MMI_CALENDAR_Y              80//49
#define MMI_CALENDAR_WIDTH       280//320//317
#define MMI_CALENDAR_HEIGHT         160
#define MMI_CALENDAR_LIMIT_Y        55
#define MMI_CALENDAR_LIMIT_HEIGHT      MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_CALENDAR_LIMIT_Y
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#else 
#define MMI_CALENDAR_X              1
#define MMI_CALENDAR_Y              35
#define MMI_CALENDAR_WIDTH       126
#define MMI_CALENDAR_HEIGHT         60
#define MMI_CALENDAR_LIMIT_Y        17
#define MMI_CALENDAR_LIMIT_HEIGHT      MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_CALENDAR_LIMIT_Y
#endif 

#ifdef __MMI_TOUCH_SCREEN__
#define MMI_CALENDAR_ARROW_OFFSET   5
#endif 
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    typedef enum
    {
        DALMATIAN_CALENDAR_HIGHLIGHTER_MOVE_WITH_SELECT,
        DALMATIAN_CALENDAR_HIGHLIGHTER_MOVE_WITH_CURRENT    /* Animatian with gif */
    } wgui_dalmatian_calendar_highlighter_styles;

    extern color_t wgui_read_calendar_highlighter_horizontal_color(S32 month);
    extern color_t wgui_read_calendar_highlighter_vertical_color(S32 month);
    extern void set_current_month_year_for_dalmatian(S32 current_day, S32 current_month, S32 current_year);
    extern void draw_current_week_highlighter(calendar_control *c);
    extern void draw_current_week_highlighter_with_current_selection(void);
    extern U16 dalmatian_calendar_current_week_image(void);
    extern void set_dalmatian_calendar_highlighter_box(void);
    extern void set_dalmatian_calendar_highlighter_enable(void);
    extern void set_dalmatian_calendar_highlighter_disable(void);
    extern S32 dalmatian_calendar_highlighter_status(void);
    extern void set_dalmatian_current_week_num(void);
    extern void set_dalmatian_title_string(void);
    extern void get_current_date_highlighter_coordinates(calendar_control *c, S32 *x, S32 *y);
    extern void draw_current_day_highlighter(void);

#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    extern void shuffle_softkey_draw_dalmatian_calender_image(void);
#endif 

    extern void wgui_setup_calendar(
                    S32 x,
                    S32 y,
                    S32 cell_width,
                    S32 cell_height,
                    S32 first_dow,
                    S32 last_day,
                    S32 current_day,
                    S32 current_month,
                    S32 current_year,
                    U32 flags);
    extern void wgui_reset_calendar_params(S32 x, S32 y, S32 n_columns, S32 n_rows, S32 cell_width, S32 cell_height);
    extern void wgui_close_calendar(void);
    extern void wgui_show_calendar(void);
    extern void wgui_register_calendar_keys(void);
    extern void wgui_move_calendar(S32 x, S32 y);

    void SetCalendarPopupTitleLunarFunc(void (*popuptitle) (U8, U8, U8, U8));
    void SetCalendarMonthFunctions(void (*month_previous_day) (void), void (*month_next_day) (void),
                                   void (*month_previous_week) (void), void (*month_next_week) (void));
#ifdef __MMI_TOUCH_SCREEN__
    void SetCalendarMonthChangeFunctions(void (*previous_month) (void), void (*next_month) (void));
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    void SetCalendarYearChangeFunctions(void (*previous_year) (void), void (*next_year) (void));
#endif 
#endif /* __MMI_TOUCH_SCREEN__ */ 
    void RegisterCalendarUpdateHandler(void (*f) (S32 day, S32 month, S32 year));
    extern void SetCalendarDay(S32 day, U8 flag);
    void RegisterCalendarHighlightHandler(void (*f) (S32 index));
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    extern void wgui_set_calendar_info_box(
                    S32 InfoBoxX,
                    S32 InfoBoxY,
                    S32 InfoBoxWidth,
                    S32 InfoBoxHeight,
                    S32 InfoBoxMax,
                    U16 InfoBoxShow);
    void RegisterCalendarUpdateInfoBoxHandler(void (*f)
                                              (S32 *p_n_info, U16 **p_icon_list, U8 ***p_description_list));
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
    extern S32 GetCalendarHighlightedDay(void);
    extern void SetCalendarHighlightedDay(S32 day);
    extern void SetCalendarPopupDay(S32 day, U8 *message1, U8 *message2, U8 *message3);
    extern void SetCalendarBackground(U16 image_ID);
    extern void SetCalendarTitleImage(U16 image_ID);

    extern void RedrawCategory80Screen(void);
    extern void InitializeCategory80Screen(
                    S32 first_day_of_week,
                    S32 last_day_of_month,
                    S32 current_day,
                    S32 current_month,
                    S32 current_year);
    extern void ShowCategory80Screen(
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *history_buffer);
    extern void ExitCategory80Screen(void);
    extern S32 GetCategory80HistorySize(void);
    extern U8 *GetCategory80History(U8 *history_buffer);

    extern void RedrawCategory81Screen(void);
    extern void InitializeCategory81Screen(
                    S32 first_day_of_week,
                    S32 last_day_of_month,
                    S32 current_day,
                    S32 current_month,
                    S32 current_year);
    extern void ShowCategory81Screen(
                    U8 *title,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message1,
                    U8 *message2,
                    U8 *history_buffer);
    extern void ExitCategory81Screen(void);
    extern S32 GetCategory81HistorySize(void);
    extern U8 *GetCategory81History(U8 *history_buffer);

    extern void RedrawCategory82Screen(void);
    extern void InitializeCategory82Screen(
                    S32 first_day_of_week,
                    S32 last_day_of_month,
                    S32 current_day,
                    S32 current_month,
                    S32 current_year);
    extern void ShowCategory82Screen(
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *history_buffer);
    extern void ExitCategory82Screen(void);
    extern S32 GetCategory82HistorySize(void);
    extern U8 *GetCategory82History(U8 *history_buffer);
    extern void ChangeCategory82Title1(U8 *title);
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    extern void ChangeCategory82Title1Month(U8 *title);
#ifdef __MMI_TOUCH_SCREEN__
    extern void DisableCalendarTitle1Arrow(void);
    extern void ResetCalendarTitle1Arrow(void);
    extern void DisableCalendarTitle1MonthArrow(void);
    extern void ResetCalendarTitle1MonthArrow(void);
#endif /* __MMI_TOUCH_SCREEN__ */ 
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
    extern void ChangeCategory82Title2(U8 *title);
    extern void ChangeCategory82Title3(U8 *title);

    extern void wgui_calendar_next_month(void);
    extern void wgui_calendar_previous_month(void);

#if defined (__MMI_TOUCH_SCREEN__) && defined (__MMI_UI_CALENDAR_WITH_INFO_BOX__)
    extern void wgui_calendar_next_year(void);
    extern void wgui_calendar_previous_year(void);
#endif /* defined (__MMI_TOUCH_SCREEN__) && defined (__MMI_UI_CALENDAR_WITH_INFO_BOX__) */ 

    extern void wgui_calendar_next_month_day(void);
    extern void wgui_calendar_previous_month_day(void);
    extern void wgui_calendar_next_month_week(void);
    extern void wgui_calendar_previous_month_week(void);

#ifdef __MMI_TOUCH_SCREEN__
    extern BOOL wgui_calendar_translate_pen_event(
                    calendar_control *c,
                    mmi_pen_event_type_enum pen_event,
                    S16 x,
                    S16 y,
                    gui_calendar_pen_enum *calendar_event);
#endif /* __MMI_TOUCH_SCREEN__ */ 
#endif /* (ENABLE_CALENDAR_CONTROL) */ 

#ifdef __cplusplus
}
#endif 

#endif /* __WGUI_CALENDAR_H__ */ 

