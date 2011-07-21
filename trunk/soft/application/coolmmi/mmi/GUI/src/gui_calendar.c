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
 *   gui_calendar.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Calendar - UI component
 * Author:
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
   Filename:      gui_calendar.c
   Date Created:  July-07-2003
   Contains:      Contains the calendar control
 
**********************************************************************************/

#include "gui.h"
#include "gui_themes.h"
#include "gui_calendar.h"
#include "wgui_calendar.h"
#include "wgui_softkeys.h"
#include "calendardef.h"
#include "debuginitdef.h"

extern void ChangeCategory82Title1(U8 *title);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
extern void ChangeCategory82Title1Month(U8 *title);
#endif 
 
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__)
 
extern void ChangeCategory82Title2(U8 *title);
#endif /* defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) */ 
extern void ChangeCategory82Title3(U8 *title);
extern void SetCalendarPopupDay(S32 day, U8 *message1, U8 *message2, U8 *message3);

 
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__)
 
extern U8 *Cat82_title1;
extern U8 *Cat82_title2;
extern U8 *Cat82_title3;

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
extern U8 *Cat82_title1_month;
#endif 
#endif /* defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__) */ 

extern BOOL r2lMMIFlag;

BOOL ClndPopupFlag = FALSE;

#if(ENABLE_CALENDAR_CONTROL)


/*****************************************************************************
 * FUNCTION
 *  _temp_calendar_default_background_function
 * DESCRIPTION
 *  fill teh claendar screen with default background
 * PARAMETERS
 *  x1      [IN]        Start x posiion
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void _temp_calendar_default_background_function(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = gui_color(192, 192, 200);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_reset_clip();
    gui_fill_rectangle(x1, y1, x2, y2, c);
}


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_function_u8u8u8u8
 * DESCRIPTION
 *  dummy function
 * PARAMETERS
 *  c1      [IN]        
 *  c2      [IN]        
 *  c3      [IN]        
 *  c4      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_dummy_function_u8u8u8u8(U8 c1, U8 c2, U8 c3, U8 c4)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(c1);
    UI_UNUSED_PARAMETER(c2);
    UI_UNUSED_PARAMETER(c3);
    UI_UNUSED_PARAMETER(c4);
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_calendar
 * DESCRIPTION
 *  Initializes a calendar control object
 * PARAMETERS
 *  c               [IN]        Pointer to a calendar control object (pre-allocated)
 *  x               [IN]        Left-Top corner location for the calendar control object
 *  y               [IN]        Left-Top corner location for the calendar control object
 *  n_columns       [IN]        Number of columns in the calendar control (Usually 7 for 7 days of the week)
 *  n_rows          [IN]        Number of rows in the calendar control (Usually 6)
 *  cell_width      [IN]        Width of each cell in pixels
 *  cell_height     [IN]        Height of each cell in pixels
 * RETURNS
 *  void
 *****************************************************************************/
extern calendar_theme _calendar_control_theme_data;
void gui_create_calendar(calendar_control *c, S32 x, S32 y, S32 n_columns, S32 n_rows, S32 cell_width, S32 cell_height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    c->x = x;
    c->y = y;
    c->n_columns = n_columns;                   /* set thenumber of columns */
    c->n_rows = n_rows;                         /* number of rows */
    c->cell_height = cell_height;               /* each cell height */
    c->cell_width = cell_width;                 /* each cell width */
    c->cursor_save_bitmap.data = NULL;
    c->popup_save_bitmap.data = NULL;
    c->x1 = x;                                  /* satrt x postion */
    c->y1 = y;                                  /* start y position */
    c->x2 = x + cell_width * n_columns - 1;     /* end x position */
    /* c->y2=y+cell_height*n_rows-1;        //end y position */
    c->y2 = y + cell_height * n_rows;
    c->draw_background_function = _temp_calendar_default_background_function;   /* set function pointer of deafult background */
    c->item_highlighted_function = UI_dummy_function_S32 ;
    c->item_unhighlighted_function = UI_dummy_function_S32 ;
    c->month_previous_day_function = UI_dummy_function;
    c->month_next_day_function = UI_dummy_function;
    c->month_previous_week_function = UI_dummy_function;
    c->month_next_week_function = UI_dummy_function;
    c->popuptitle_lunarcalendar = UI_dummy_function_u8u8u8u8;
    c->theme = &_calendar_control_theme_data;   /* _temp_calendar_control_theme; */
    c->list_of_items = NULL;
    c->highlighted_item = -1;
    c->flags = 0;
    c->display_popup = NULL;
#ifdef __MMI_TOUCH_SCREEN__
    c->pen_down_on_highlighted_item = 0;
    c->previous_month_function = UI_dummy_function;
    c->next_month_function = UI_dummy_function;
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    c->previous_year_function = UI_dummy_function;
    c->next_year_function = UI_dummy_function;
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
#endif /* __MMI_TOUCH_SCREEN__ */ 
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    c->info_x1 = 0;
    c->info_y1 = 0;
    c->info_x2 = 0;
    c->info_y2 = 0;
    c->max_n_info = 3;
    c->flags |= CALENDAR_SHOW_INFO_BOX;
    c->n_info = 0;
    c->icon_list = NULL;
    c->description_list = NULL;
    c->update_info_box = NULL;
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_calendar_background_function
 * DESCRIPTION
 *  Sets the background drawing function that a calendar control object should use
 * PARAMETERS
 *  c           [IN]        Pointer to a calendar control object
 *  f           [IN]        Is a pointer to the function which draws the background.
 *  y1(?)       [IN]        Left-Top corner of the drawing area
 *  x2(?)       [IN]        Right-Bottom corner of the drawing area
 *  This(?)     [IN]        Function takes the following parameters:
 *  x1(?)       [IN]        Left-Top corner of the drawing area
 *  y2(?)       [IN]        Right-Bottom corner of the drawing area
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_calendar_background_function(calendar_control *c, void (*f) (S32 x1, S32 y1, S32 x2, S32 y2))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    c->draw_background_function = f;    /* set function pointer */
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_calendar_days
 * DESCRIPTION
 *  Sets the first day of week, last day of month -- etc.
 * PARAMETERS
 *  c                   [IN]        Pointer to a calendar control object
 *  first_dow           [IN]        Is the first day of the week (See gui_calendar.h for the values)
 *  last_day            [IN]        Is the last day of the month (ex: 28, 29, 30, 31 -- etc)
 *  list_of_items       [IN]        This is the list of flags used. One entry for each day. (See gui_calendar.h for the values)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_calendar_days(calendar_control *c, S32 first_dow, S32 last_day, U8 *list_of_items)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    c->first_item = first_dow;                  /* set first day of calendar of current month */
    c->last_item = first_dow + last_day - 1;    /* setlast day of calendar of current minth */
    c->list_of_items = list_of_items;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_calendar
 * DESCRIPTION
 *  Displays the calendar control
 * PARAMETERS
 *  c       [IN]        Pointer to a calendar control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_calendar(calendar_control *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j;
    S32 x1, y1, x2, y2;
    U8 s[20];
    S32 sw, sh;
    S32 cell_width = c->cell_width;
    S32 cell_height = c->cell_height;
    S32 ox = c->x1;
    S32 oy = c->y1;
    S32 cell_center_x;
    S32 n_rows = c->n_rows;
    S32 n_columns = c->n_columns;
    S32 index = 0;
    S32 day = 1;
    S32 first_item = c->first_item;
    S32 last_item = c->last_item;
    UI_filled_area *blank_day_filler = c->theme->blank_day_filler;
    UI_filled_area *normal_day_filler = c->theme->normal_day_filler;
    UI_filled_area *task_day_filler = c->theme->task_day_filler;
    UI_filled_area *current_day_filler = c->theme->current_day_filler;
    UI_filled_area *week_start_day_filler = c->theme->week_start_day_filler;
    UI_filled_area *week_end_day_filler = c->theme->week_end_day_filler;
    color_t normal_day_text_color = c->theme->normal_day_text_color;
#if defined(__MMI_MAINLCD_160X128__)  || defined(__MMI_USING_BLACK_THEME_BG__)
    	color_t task_day_text_color = c->theme->normal_day_text_color;
#else
	color_t task_day_text_color = c->theme->task_day_text_color;
#endif
    color_t current_day_text_color = c->theme->current_day_text_color;
    color_t week_start_day_text_color = c->theme->week_start_day_text_color;
    color_t week_end_day_text_color = c->theme->week_end_day_text_color;
    U8 *list_of_items = c->list_of_items;
    UI_filled_area *special1_day_filler = c->theme->special1_day_filler;
    color_t special1_day_text_color = c->theme->special1_day_text_color;
    UI_filled_area *special2_day_filler = c->theme->special2_day_filler;
    color_t special2_day_text_color = c->theme->special2_day_text_color;
    UI_filled_area *special3_day_filler = c->theme->special3_day_filler;
    color_t special3_day_text_color = c->theme->special3_day_text_color;
    UI_filled_area *special4_day_filler = c->theme->special4_day_filler;
    color_t special4_day_text_color = c->theme->special4_day_text_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_push_text_clip();
    gui_set_clip(c->x1, c->y1, c->x2, c->y2);
    gui_set_font(&MMI_small_font);      /* set font to small */
    cell_center_x = cell_width >> 1;    /* caluclate teh center x of cell */

    c->draw_background_function(c->x1, c->y1, c->x2, c->y2);    /* draw bkg image of calenadra */
    if (list_of_items == NULL)
    {
        gdi_layer_pop_clip();
        gdi_layer_pop_text_clip();
        return;
    }
    y1 = oy;
    /* check type of day and fill that cell with particular color theme */
    for (j = 0; j < n_rows; j++)
    {
        x1 = ox;
        for (i = 0; i < n_columns; i++)
        {
            x2 = x1 + cell_width - 1;
            y2 = y1 + cell_height - 1;
            if ((index >= first_item) && (index <= last_item))
            {
                if ((list_of_items[day] & 0x3f) == CALENDAR_CURRENT_DAY)
                {
                    gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, current_day_filler);   /* fill color of current day */
                    if (i == CALENDAR_WEEK_START)
                    {
                        gui_set_text_color(week_start_day_text_color);  /* set teh text color */
                    }
                    else if (i == CALENDAR_WEEK_END)
                    {
                        gui_set_text_color(week_end_day_text_color);
                    }
                    else
                    {
                        gui_set_text_color(current_day_text_color);
                    }
                }
                else if ((list_of_items[day] & 0x3f) == CALENDAR_TASK_DAY)      /* if task is tore in particular day */
                {
                    gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, task_day_filler);
                    if (i == CALENDAR_WEEK_START)   /* if week start day */
                    {
                        gui_set_text_color(week_start_day_text_color);
                    }
                    else if (i == CALENDAR_WEEK_END)    /* if week end day */
                    {
                        gui_set_text_color(week_end_day_text_color);
                    }
                    else
                    {
                        gui_set_text_color(task_day_text_color);
                    }
                }
                else if ((list_of_items[day] & 0x3f) == CALENDAR_SPECIAL_DAY1)  /* if spevcila day1 */
                {
                    gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, special1_day_filler);
                    gui_set_text_color(special1_day_text_color);
                }
                else if ((list_of_items[day] & 0x3f) == CALENDAR_SPECIAL_DAY2)  /* if specila daye */
                {
                    gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, special2_day_filler);
                    gui_set_text_color(special2_day_text_color);
                }
                else if ((list_of_items[day] & 0x3f) == CALENDAR_SPECIAL_DAY3)  /* if special day 3 */
                {
                    gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, special3_day_filler);
                    gui_set_text_color(special3_day_text_color);
                }
                else if ((list_of_items[day] & 0x3f) == CALENDAR_SPECIAL_DAY4)  /* if special day 4 */
                {
                    gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, special4_day_filler);
                    gui_set_text_color(special4_day_text_color);
                }
                else if ((list_of_items[day] & 0x3f) == CALENDAR_NORMAL_DAY)    /* if normal day */
                {
                    if (i == CALENDAR_WEEK_START)
                    {
                        gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, week_start_day_filler);
                        gui_set_text_color(week_start_day_text_color);
                    }
                    else if (i == CALENDAR_WEEK_END)    /* if week end */
                    {
                        gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, week_end_day_filler);
                        gui_set_text_color(week_end_day_text_color);
                    }
                    else
                    {
                        gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, normal_day_filler);
                        gui_set_text_color(normal_day_text_color);
                    }
                }
                gdi_layer_push_text_clip();
                gdi_layer_set_text_clip(x1 + 1, y1, x2 - 1, y2);
                gui_itoa(day, (UI_string_type) s, 10);  /* convert day to string */
                /* mesaure height and wdth of current date string */
                gui_measure_string((UI_string_type) s, &sw, &sh);
                /* move cursor to x,y position */
                if (r2lMMIFlag)
                {
                    gui_move_text_cursor(x1 + cell_center_x - (sw >> 1) + sw, y1 + (cell_height >> 1) - (sh >> 1) + 1);
                }
                else
                {
                    gui_move_text_cursor(x1 + cell_center_x - (sw >> 1), y1 + (cell_height >> 1) - (sh >> 1) + 1);
                }
                /* print text */
                gui_print_text((UI_string_type) s);
                day++;
                gdi_layer_pop_text_clip();
            }
            else
            {
                gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, blank_day_filler);
            }
            x1 += cell_width;
            index++;
        }
        y1 += cell_height;
    }
    gdi_layer_pop_clip();
    gdi_layer_pop_text_clip();
    c->flags &= ~CALENDAR_HIGHLIGHT_BACKUP;
}


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_refresh_highlight
 * DESCRIPTION
 *  Internal function: Refreshes the calendar display (when a new day is highlighted)
 * PARAMETERS
 *  c                           [IN]        Pointer to a calendar control object
 *  old_highlighted_item        [IN]        Is the 0 based index of the previously highlighted item
 *  new_highlighted_item        [IN]        Is the 0 based index of the newly highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void gui_calendar_refresh_highlight(calendar_control *c, S32 old_highlighted_item, S32 new_highlighted_item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 row, col;
    S32 x1, y1, x2, y2;
    U8 s[20];
    S32 sw, sh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_push_text_clip();
    gui_set_clip(c->x1, c->y1, c->x2, c->y2);
    if (old_highlighted_item >= 0)
    {
        if (c->flags & CALENDAR_POPUP_BACKUP)
        {
            /* Temporarily disabled pop up bitmap backup since there is no available buffer to use on hardware    */
            /* _show_bitmap(c->px1,c->py1,&c->popup_save_bitmap);                                     */
        #if(UI_DOUBLE_BUFFER_SUPPORT)
            /* gdi_layer_blt_previous(c->px1,c->py1,c->px2,c->py2);                                */
        #endif 
            /* Temporary: Redraw the whole calendar when any item is highlighted                         */
            /* S32 h=c->highlighted_item; */
            c->flags |= CALENDAR_HIDE_POPUP;
            gui_show_calendar(c);
            c->flags &= ~CALENDAR_HIDE_POPUP;
            c->flags &= ~CALENDAR_HIGHLIGHT_BACKUP;
        #if(UI_DOUBLE_BUFFER_SUPPORT)
            gdi_layer_blt_previous(c->x1, c->y1, c->x2, c->y2);
        #endif 
            /* Temporary block ends                                                          */
            c->flags &= ~CALENDAR_POPUP_BACKUP;
        }
        if (c->flags & CALENDAR_HIGHLIGHT_BACKUP)
        {
            row = old_highlighted_item / c->n_columns;
            col = old_highlighted_item % c->n_columns;
            x1 = c->x1 + c->cell_width * col;
            y1 = c->y1 + c->cell_height * row;
            x2 = x1 + c->cell_width - 1;
            y2 = y1 + c->cell_height - 1;
            _show_bitmap(x1, y1, &c->cursor_save_bitmap);   /* show bitmap */
        #if(UI_DOUBLE_BUFFER_SUPPORT)
            gdi_layer_blt_previous(x1, y1, x2, y2 + 1);
        #endif 
            c->flags &= ~CALENDAR_HIGHLIGHT_BACKUP;
        }
    }
    if ((new_highlighted_item >= c->first_item) && (new_highlighted_item <= c->last_item))
    {
        c->flags |= CALENDAR_HIGHLIGHT_BACKUP;
        row = new_highlighted_item / c->n_columns;
        col = new_highlighted_item % c->n_columns;
        x1 = c->x1 + c->cell_width * col;
        y1 = c->y1 + c->cell_height * row;
        x2 = x1 + c->cell_width - 1;
        y2 = y1 + c->cell_height - 1;
        _get_bitmap(x1, y1, x2, y2 + 1, &c->cursor_save_bitmap);        /* get bitmap */

        /* Display the pop up if required   */
        if ((c->flags & CALENDAR_ENABLE_POPUP) &&
            (c->list_of_items[new_highlighted_item - c->first_item + 1] & CALENDAR_POPUP_DAY))
        {
            if (c->display_popup != NULL)
            {
                c->flags |= CALENDAR_POPUP_BACKUP;
                c->px1 = x1;
                c->py1 = y1;
                c->px2 = x2;
                c->py2 = y2;
                c->display_popup(c->highlighted_item - c->first_item + 1, &c->px1, &c->py1, &c->px2, &c->py2);
            }
        }

        /* Draw the new highlighted cell */
        gui_set_font(c->theme->text_font);  /* set font */
        gui_reset_clip();
        gui_draw_filled_area(x1, y1 + 1, x2, y2 + 1, c->theme->selected_day_filler);
        gui_set_text_color(c->theme->selected_day_text_color);
        gdi_layer_set_text_clip(x1 + 1, y1, x2 - 1, y2);
        gui_itoa(new_highlighted_item - c->first_item + 1, (UI_string_type) s, 10);
        /* measure width and height of current date of string */
        gui_measure_string((UI_string_type) s, &sw, &sh);
        /* move text cursor */
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(
                x1 + (c->cell_width >> 1) - (sw >> 1) + sw,
                y1 + (c->cell_height >> 1) - (sh >> 1) + ((c->cell_height - sh) % 2));
        }
        else
        {
            gui_move_text_cursor(
                x1 + (c->cell_width >> 1) - (sw >> 1),
                y1 + (c->cell_height >> 1) - (sh >> 1) + ((c->cell_height - sh) % 2));
        }
        /* print string */
        gui_print_text((UI_string_type) s);
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gdi_layer_blt_previous(x1, y1, x2, y2 + 1);
    #endif 
    }
    gdi_layer_pop_clip();
    gdi_layer_pop_text_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_highlight_day
 * DESCRIPTION
 *  Selects a given day on the calendar
 *  
 *  Automatically refreshes the display
 * PARAMETERS
 *  c       [IN]        Pointer to a calendar control object
 *  day     [IN]        Is the day to be selected
 * RETURNS
 *  void
 *****************************************************************************/
void gui_calendar_highlight_day(calendar_control *c, S32 day)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = c->highlighted_item; /* store current highlight day */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    c->highlighted_item = c->first_item + day - 1;
    if (old_highlighted_item != c->highlighted_item)
    {
        gui_calendar_refresh_highlight(c, old_highlighted_item, c->highlighted_item);
        c->item_unhighlighted_function(old_highlighted_item - c->first_item + 1);       /* call unhighligted function */
        c->item_highlighted_function(c->highlighted_item - c->first_item + 1);  /* call highlighted function */
    }

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    if ((c->flags & CALENDAR_SHOW_INFO_BOX) && (c->update_info_box != NULL))
    {
        c->update_info_box(&c->n_info, &c->icon_list, &c->description_list);
        gui_calendar_draw_info_box(c);
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_previous_day
 * DESCRIPTION
 *  Selects the previous day
 *  
 *  Automatically refreshes the display
 * PARAMETERS
 *  c       [IN]        Pointer to a calendar control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_calendar_previous_day(calendar_control *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = c->highlighted_item; /* stor current highlighted day */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (c->highlighted_item > c->first_item)
    {
        c->highlighted_item--;
        c->popuptitle_lunarcalendar(PREV_DAY, 0, 0, 0); /* set title according to current day */
        /* refresh calendar highlight */
        gui_calendar_refresh_highlight(c, old_highlighted_item, c->highlighted_item);
        /* call unhighlighted function */
        c->item_unhighlighted_function(old_highlighted_item - c->first_item + 1);
        /* call highlighted function */
        c->item_highlighted_function(c->highlighted_item - c->first_item + 1);
    }
    else
    {
        c->month_previous_day_function();   /* call previous day function */
    }

    if (ClndPopupFlag)
    {
        ClndPopupFlag = FALSE;
        return;
    }

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    if ((c->flags & CALENDAR_SHOW_INFO_BOX) && (c->update_info_box != NULL))
    {
        c->update_info_box(&c->n_info, &c->icon_list, &c->description_list);
        gui_calendar_draw_info_box(c);
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

     
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__)
     
    ChangeCategory82Title1(Cat82_title1);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    ChangeCategory82Title1Month(Cat82_title1_month);
#endif 

    ChangeCategory82Title2(Cat82_title2);
    ChangeCategory82Title3(Cat82_title3);
    show_left_softkey();
    show_right_softkey();
#endif /* defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) ||defined(__MMI_MAINLCD_220X176__) */ 
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_go_to_day
 * DESCRIPTION
 *  Go to the selected day of the month
 *  
 *  Automatically refreshes the display (Also Refreshes Lunar Calendar Titles)
 *  Useful for Touch Screen (where user can select any day with pen)
 * PARAMETERS
 *  c           [IN]        Pointer to a calendar control object
 *  index       [IN]        Day of the month
 * RETURNS
 *  void
 *****************************************************************************/
void gui_calendar_go_to_day(calendar_control *c, U8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 old_day;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    old_day = c->highlighted_item - c->first_item + 1;
    c->popuptitle_lunarcalendar(SELECTED_DAY, 0, index, old_day);       /* set the poup of calendar of current day */
    SetCalendarHighlightedDay(index);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    if ((c->flags & CALENDAR_SHOW_INFO_BOX) && (c->update_info_box != NULL))
    {
        c->update_info_box(&c->n_info, &c->icon_list, &c->description_list);
        gui_calendar_draw_info_box(c);
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

     
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) ||defined(__MMI_MAINLCD_220X176__)
     
    ChangeCategory82Title1(Cat82_title1);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    ChangeCategory82Title1Month(Cat82_title1_month);
#endif 

    ChangeCategory82Title2(Cat82_title2);
    ChangeCategory82Title3(Cat82_title3);
    show_left_softkey();
    show_right_softkey();
#endif /* defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__)*/ 
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_next_day
 * DESCRIPTION
 *  Selects the next day
 *  
 *  Automatically refreshes the display
 * PARAMETERS
 *  c       [IN]        Pointer to a calendar control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_calendar_next_day(calendar_control *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = c->highlighted_item; /* store current highlighted day */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (c->highlighted_item < c->last_item)
    {
        c->highlighted_item++;
        c->popuptitle_lunarcalendar(NEXT_DAY, 0, 0, 0); /* set the poup of calebdar of current day */
        gui_calendar_refresh_highlight(c, old_highlighted_item, c->highlighted_item);
        /* call unhighlighted function */
        c->item_unhighlighted_function(old_highlighted_item - c->first_item + 1);
        /* call highlighted func */
        c->item_highlighted_function(c->highlighted_item - c->first_item + 1);
    }
    else
    {
        c->month_next_day_function();   /* move to 1st day of next month */
    }

    if (ClndPopupFlag)
    {
        ClndPopupFlag = FALSE;
        return;
    }

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    if ((c->flags & CALENDAR_SHOW_INFO_BOX) && (c->update_info_box != NULL))
    {
        c->update_info_box(&c->n_info, &c->icon_list, &c->description_list);
        gui_calendar_draw_info_box(c);
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

     
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__)
     
    ChangeCategory82Title1(Cat82_title1);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    ChangeCategory82Title1Month(Cat82_title1_month);
#endif 

    ChangeCategory82Title2(Cat82_title2);
    ChangeCategory82Title3(Cat82_title3);
    show_left_softkey();
    show_right_softkey();
#endif /* defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_next_row
 * DESCRIPTION
 *  Selects a day in the next row of the calendar
 *  
 *  Automatically refreshes the display
 * PARAMETERS
 *  c       [IN]        Pointer to a calendar control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_calendar_next_row(calendar_control *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = c->highlighted_item; /* stor current highlihted day */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((c->highlighted_item + c->n_columns) <= c->last_item)
    {
        c->highlighted_item += c->n_columns;
        /* set popup f current day */
        c->popuptitle_lunarcalendar(NEXT_ROW, (U8) c->n_columns, 0, 0);
        /* refresgh calendar */
        gui_calendar_refresh_highlight(c, old_highlighted_item, c->highlighted_item);
        /* call unhighlighted func */
        c->item_unhighlighted_function(old_highlighted_item - c->first_item + 1);
        /* call highlighted func */
        c->item_highlighted_function(c->highlighted_item - c->first_item + 1);
    }
    else
    {
        c->month_next_week_function();  /* move to first week next month */
    }

    if (ClndPopupFlag)
    {
        ClndPopupFlag = FALSE;
        return;
    }

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    if ((c->flags & CALENDAR_SHOW_INFO_BOX) && (c->update_info_box != NULL))
    {
        c->update_info_box(&c->n_info, &c->icon_list, &c->description_list);
        gui_calendar_draw_info_box(c);
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

     
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__)
     
    ChangeCategory82Title1(Cat82_title1);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    ChangeCategory82Title1Month(Cat82_title1_month);
#endif 

    ChangeCategory82Title2(Cat82_title2);
    ChangeCategory82Title3(Cat82_title3);
    show_left_softkey();
    show_right_softkey();
#endif /* defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_previous_row
 * DESCRIPTION
 *  Selects a day in the previous row of the calendar
 *  
 *  Automatically refreshes the display
 * PARAMETERS
 *  c       [IN]        Pointer to a calendar control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_calendar_previous_row(calendar_control *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item = c->highlighted_item; /* store current day */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((c->highlighted_item - c->n_columns) >= c->first_item)
    {
        c->highlighted_item -= c->n_columns;
        /* set poup and titel of current day */
        c->popuptitle_lunarcalendar(PREV_ROW, (U8) c->n_columns, 0, 0);
        gui_calendar_refresh_highlight(c, old_highlighted_item, c->highlighted_item);
        /* call unhighlighted fun c */
        c->item_unhighlighted_function(old_highlighted_item - c->first_item + 1);
        /* call highlighted func */
        c->item_highlighted_function(c->highlighted_item - c->first_item + 1);
    }
    else
    {
        c->month_previous_week_function();  /* move to last week of previous month */
    }

    if (ClndPopupFlag)
    {
        ClndPopupFlag = FALSE;
        return;
    }

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    if ((c->flags & CALENDAR_SHOW_INFO_BOX) && (c->update_info_box != NULL))
    {
        c->update_info_box(&c->n_info, &c->icon_list, &c->description_list);
        gui_calendar_draw_info_box(c);
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

     
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__)
     
    ChangeCategory82Title1(Cat82_title1);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    ChangeCategory82Title1Month(Cat82_title1_month);
#endif 

    ChangeCategory82Title2(Cat82_title2);
    ChangeCategory82Title3(Cat82_title3);
    show_left_softkey();
    show_right_softkey();
#endif /* defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) || defined(__MMI_UI_DALMATIAN_CALENDAR__) || defined(__MMI_MAINLCD_220X176__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_calendar
 * DESCRIPTION
 *  Moves the calendar to a different position
 * PARAMETERS
 *  c       [IN]        Pointer to a calendar control object
 *  x       [IN]        Is the new location
 *  y       [IN]        Is the new location
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_calendar(calendar_control *c, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    c->x = x;                                       /* star x position of calndar */
    c->y = y;                                       /* satrt y position of calendar */
    c->x1 = x;
    c->y1 = y;
    c->x2 = x + c->cell_width * c->n_columns - 1;   /* end x positiobn of calendar */
    c->y2 = y + c->cell_height * c->n_rows - 1;     /* end y position of calendar */
    c->y2 += 1;                                      
}

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_draw_info_box
 * DESCRIPTION
 *  show info box embbed in the calendar
 * PARAMETERS
 *  c       [IN]        Pointer to a calendar control object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_calendar_draw_info_box(calendar_control *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 box_height, line_height;
    S32 max_n_line, n_line;
    S32 i;
    S32 offset_x = 5;
    S32 image_width, image_height;
    S32 string_width, string_height;
    S32 ix, iy, tx, ty;
    S32 line_start_y = c->info_y1 + 1;
    color_t bgc = {182, 200, 255, 100};
    color_t brdc = {82, 200, 255, 100};
    color_t linec = *(current_MMI_theme->UI_white_color);
    color_t tc = *(current_MMI_theme->UI_black_color);
    S8 *image;
#ifdef __MMI_MAINLCD_220X176__
	return;
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_set_font(&MMI_default_font);
    gui_set_text_color(tc);

    x1 = c->info_x1;
    y1 = c->info_y1;
    x2 = c->info_x2;
    y2 = c->info_y2;
    box_height = c->info_y2 - c->info_y1 - 1;   /* decrease 2 pixel for border lines */
    max_n_line = c->max_n_info;
    /* n_line = 3; */
    n_line = c->n_info;
    if (n_line > max_n_line)
    {
        n_line = max_n_line;
    }

    line_height = box_height / n_line;
    ix = x1 + offset_x;

    gdi_layer_push_clip();
    gui_set_clip(x1, y1, x2, y2);
    gui_draw_rectangle(x1, y1, x2, y2, brdc);
    gui_fill_rectangle(x1 + 1, y1 + 1, x2 - 1, y2 - 1, bgc);

    for (i = 0; i < max_n_line - 1; i++)
    {
        line_start_y += line_height;
        gui_draw_horizontal_line(x1 + offset_x, x2 - offset_x, line_start_y, linec);
    }

    line_start_y = c->info_y1 + 1;
    for (i = 0; i < n_line; i++)
    {
        image = GetImage(c->icon_list[i]);
        if(c->icon_list [i] == 0)
        {
            image_width = image_height = 0;
        }
        else
        {
            gui_measure_image((PU8) image, &image_width, &image_height);
        }
        iy = line_start_y + ((line_height - image_height) >> 1);
        gui_show_image(ix, iy, (PU8) image);

        gui_measure_string((UI_string_type) c->description_list[i], &string_width, &string_height);
        tx = ix + image_width + offset_x;
        ty = line_start_y + ((line_height - string_height) >> 1);
        gui_move_text_cursor(tx, ty);
        gui_print_text((UI_string_type) c->description_list[i]);

        line_start_y += line_height;
    }

    gdi_layer_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gdi_layer_blt_previous(x1, y1, x2, y2);
#endif 
}
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

#endif /* (ENABLE_CALENDAR_CONTROL) */ 

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_translate_pen_position
 * DESCRIPTION
 *  translate the pen position to the item
 * PARAMETERS
 *  c               [IN]        Pointer to a calendar control object
 *  x               [IN]        Pen position
 *  y               [IN]        Pen position
 *  item_index      [OUT]       Output the item index which is triggered
 * RETURNS
 *  void
 *****************************************************************************/
/* *item_index = 0 for fail */
void gui_calendar_translate_pen_position(calendar_control *c, S32 x, S32 y, S32 *item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 xi, yi, row = 1, col = 1, i = 1, j, first_row_count = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    xi = c->x1 + (c->first_item) * (c->cell_width);
    yi = c->y1;

    if (x < c->x1 || x > c->x2 || y < c->y1 || y > c->y2)
    {
        *item_index = 0;
        return;
    }

    for (j = yi; j < c->y2; j += c->cell_height)
    {
        if (y >= j && y < j + c->cell_height)
        {
            row = i;
            break;
        }
        i++;
    }
    i = 1;
    for (j = c->x1; j < c->x2; j += c->cell_width)
    {
        if (x >= j && x < j + c->cell_width)
        {
            col = i;
            break;
        }
        i++;
    }
    switch (c->first_item)
    {
        case 0:
            first_row_count = 7;
            break;
        case 1:
            first_row_count = 6;
            break;
        case 2:
            first_row_count = 5;
            break;
        case 3:
            first_row_count = 4;
            break;
        case 4:
            first_row_count = 3;
            break;
        case 5:
            first_row_count = 2;
            break;
        case 6:
            first_row_count = 1;
            break;
        default:
            MMI_ASSERT(0);

    }
    if (row == 1)
    {
        *item_index = col - c->first_item;
    }
    else
    {
        *item_index = (row - 2) * c->n_columns + first_row_count + col;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_calendar_translate_pen_event
 * DESCRIPTION
 *  translate the pen position to the event
 * PARAMETERS
 *  c                   [IN]        Pointer to a calendar control object
 *  pen_event           [IN]        Event type
 *  x                   [IN]        Pen position
 *  y                   [IN]        Pen position
 *  calendar_event      [OUT]       Event pointer
 * RETURNS
 *  void
 *****************************************************************************/
BOOL gui_calendar_translate_pen_event(
        calendar_control *c,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_calendar_pen_enum *calendar_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    BOOL ret;
    S32 item_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ret = MMI_TRUE;

    *calendar_event = GUI_CALENDAR_PEN_NONE;

    switch (pen_event)
    {
        case MMI_PEN_EVENT_DOWN:
            if (PEN_CHECK_BOUND(x, y, c->x1, c->y1, c->x2, c->y2))
            {
                gui_calendar_translate_pen_position(c, x, y, &item_index);
                if (item_index < 1 || item_index > c->last_item - c->first_item + 1)
                {
                    ret = MMI_FALSE;
                    break;
                }
                if (item_index == c->highlighted_item - c->first_item + 1)
                {
                    /* Down event on the highlighted item set flag to 1 so that now up event here will call LSK function */
                    c->pen_down_on_highlighted_item = 1;
                }
                if (item_index != c->highlighted_item - c->first_item + 1)
                {
                    gui_calendar_go_to_day(c, (U8) item_index);
                }
                *calendar_event = GUI_CALENDAR_PEN_HIGHLIGHT_CHANGED;

            }
            else
            {
                ret = MMI_FALSE;
            }
            break;

        case MMI_PEN_EVENT_MOVE:
            if (PEN_CHECK_BOUND(x, y, c->x1, c->y1, (c->x2 - c->x1 + 1), (c->y2 - c->y1 + 1)))
            {
                gui_calendar_translate_pen_position(c, x, y, &item_index);
                if (item_index != c->highlighted_item - c->first_item + 1)
                {
                    if (item_index < 1 || item_index > c->last_item - c->first_item + 1)
                    {
                        /* FIXME. handle this case better */
                        ret = MMI_FALSE;
                    }
                    else
                    {
                        gui_calendar_go_to_day(c, (U8) item_index);
                        *calendar_event = GUI_CALENDAR_PEN_HIGHLIGHT_CHANGED;
                    }
                    c->pen_down_on_highlighted_item = 0;
                }
            }
            else
            {
                c->pen_down_on_highlighted_item = 0;
                if (y < c->y1)
                {
                    gui_calendar_previous_row(&MMI_calendar_control);   /* move to previous row */
                    *calendar_event = GUI_CALENDAR_PEN_NEED_REDRAW;
                }
                else if (y > c->y2)
                {
                    gui_calendar_next_row(&MMI_calendar_control);
                    *calendar_event = GUI_CALENDAR_PEN_NEED_REDRAW;
                }
            }
            break;

        case MMI_PEN_EVENT_UP:
            if (PEN_CHECK_BOUND(x, y, c->x1, c->y1, c->x2, c->y2))
            {
                gui_calendar_translate_pen_position(c, x, y, &item_index);
                if (item_index < 1 || item_index > c->last_item - c->first_item + 1)
                {
                    ret = MMI_FALSE;
                    break;
                }
                if (item_index == c->highlighted_item - c->first_item + 1)
                {

                    if (c->pen_down_on_highlighted_item == 1)
                    {
                        *calendar_event = GUI_CALENDAR_PEN_ITEM_SELECTED;
                        c->pen_down_on_highlighted_item = 0;    /* Reset again */
                        break;
                    }

                }
            }
            else
            {
                ret = MMI_FALSE;
            }
            break;

        case MMI_PEN_EVENT_LONG_TAP:
            /* Do nothing */
            break;

        case MMI_PEN_EVENT_REPEAT:
            /* Do nothing */
            break;

        case MMI_PEN_EVENT_ABORT:
            /* Do nothing */
            break;

        default:
            MMI_ASSERT(0);
    }
    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 

