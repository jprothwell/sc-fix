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
 *   wgui_calendar.c
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
 * 
 * Go to 1970/1/1 or 2030/12/31 when highlighted date is out of the boundary.
 * 
 * 
 * 
 * 
 * 
 * 
 * Modified right arrow key blt region
 * 
 * 
 * 
 * 
 */

/**********************************************************************************
   Filename:      wgui_calendar.c
   Date Created:  July-07-2003
   Contains:      Contains the calendar control wrappers and category screens
**********************************************************************************/

/* Wrappers functions for use by category screens and applications   */

#define __NEWSIMULATOR
#include "globaldefs.h"
#include "custdatares.h"
#include "custdataprots.h"
#include "gui_themes.h"
#include "wgui_categories.h"
#include "wgui_status_icons.h"
#include "wgui_softkeys.h"
#include "wgui_calendar.h"
#include "mmi_features.h"
#include "debuginitdef.h"
#include "wgui_draw_manager.h"
#include "datetimegprot.h"
#include "datetimetype.h"
#include "wgui_categories_defs.h"
#include "wgui_softkeys.h"
#include "wgui_categories_defs.h"
#include "calendardef.h"        /* calendar image */
#include "calendarprot.h"
#if(defined(MEM_ULC_3216) || defined(__MMI_RESOURCE_SLIM_ON_128X160__))
#include "wallpaperdefs.h"
#endif

#undef __NEWSIMULATOR

#include "mmi_trace.h"
#if(ENABLE_CALENDAR_CONTROL)

#ifdef __MMI_TOUCH_SCREEN__
static S32 calendar_arrow_width, calendar_arrow_height;
#endif 

#define CALENDAR_FULL_SCREEN_BACKGROUND      1
#define CALENDAR_BORDERED_TITLES       1
/* Calendar popup widht */
#define CALENDAR_POPUP_WIDTH           100      /* Maximum width allowed for calendar pop up                            */
/* Calendar popup height */
#define CALENDAR_POPUP_HEIGHT          100      /* Maximum height allowed for calendar pop up                              */
/* start x position of calendar popup */
#define CALENDAR_POPUP_X               1        /* If using Fixed position, (x,y) are relative / absolute values to screen corners  */
/* start y position of calendar popup */
#define CALENDAR_POPUP_Y               0        /* Otherwise, (x,y) are relative to the highlighted day on calendar           */
#define CALENDAR_POPUP_FIXED_POSITION     0     /* 1:Show at fixed location (fixed with respect to screen corners)               */
#define CALENDAR_POPUP_FIXED_RIGHT        0     /* 1:Show to right side of screen, 0:Show to left side                        */
#define CALENDAR_POPUP_FIXED_BOTTOM       0     /* 1:Show to bottom of screen, 0:Show from top of screen                   */
/* popup time duration */
#define CALENDAR_POPUP_WAIT_TIME       2000     /* Milliseconds before the pop up is displayed after highlighting a day       */

/* External Global variables and funtion declaration */
extern S32 UI_get_font_height(UI_font_type f);
extern BOOL r2lMMIFlag;
extern pBOOL IsChineseSet(void);
extern U8 matrix_main_menu_highlight_bitmap_data[];
extern icontext_button MMI_softkeys[];
#if defined(__MMI_NOKIA_STYLE_N800__)
extern const U8 IMG_BACKGROUND_2Theme[];
#endif
/* global variable of calendar_control */
calendar_control MMI_calendar_control;

/* store monthly days */
U8 MMI_calendar_days[MAX_CALENDAR_DAYS];

/* store hightlighted day */
S32 MMI_calendar_highlighted_day = -1;

/* Lunar calendar specific title display  */
U8 *Cat82_title1;
U8 *Cat82_title2;
U8 *Cat82_title3;

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
U8 *Cat82_title1_month;

#ifdef __MMI_TOUCH_SCREEN__
BOOL Cat82_title1_arrow_disable = MMI_FALSE;
BOOL Cat82_title1_month_arrow_disable = MMI_FALSE;
#endif /* __MMI_TOUCH_SCREEN__ */ 
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

U8 MMI_calendar_title_string[CLNDR_TITLE_LEN];

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
U8 MMI_calendar_title_string_month[CLNDR_TITLE_LEN];    /* get from getstring() so don't need buffer */
#endif 

S16 MMI_calendar_current_month = -1;
S16 MMI_calendar_current_year = -1;
BOOL lunar_calendar_present = MMI_FALSE;

static void UI_dummy_function_S32S32S32(S32 day, S32 month, S32 year);
static void (*MMI_calendar_control_update_handler) (S32 day, S32 month, S32 year) = UI_dummy_function_S32S32S32;

/* a func pointer of calendar control highhlight handler */
void (*MMI_calendar_control_highlight_handler) (S32 day) = UI_dummy_function_S32 ;

/* global varibales of mesage for lunar calendar */
U8 *MMI_calendar_messages1 = NULL;
U8 *MMI_calendar_messages2 = NULL;
U8 *MMI_calendar_messages3 = NULL;

pBOOL calendar_show_title2 = 1;
pBOOL calendar_show_title3 = 1;

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
pBOOL calendar_show_title_month = 1;
#endif 
extern S32 UI_get_font_height(UI_font_type f);
UI_font_type calendar_title_font;

extern BOOL 			r2lMMIFlag;

extern pBOOL IsChineseSet(void);

extern byte matrix_main_menu_highlight_bitmap_data[]; 
extern icontext_button	MMI_softkeys[]; 
void SetCategory82Title1(U8 *title);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
void SetCategory82Title1Month(U8 *title);
#endif 

/* Category functions for calendar related screens    */
S32 cat82_title1_x1;    /* start x position of lunar calendar title1 */
S32 cat82_title1_y1;    /* start y position of lunar calendar title1 */
S32 cat82_title1_x2;    /* end x position of lunar calendar title1 */
S32 cat82_title1_y2;    /* end y  position of lunar calendar title1 */

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
S32 cat82_title1_month_x1;  /* start x position of lunar calendar title1 month */
S32 cat82_title1_month_y1;  /* start y position of lunar calendar title1 month */
S32 cat82_title1_month_x2;  /* end x position of lunar calendar title1 month */
S32 cat82_title1_month_y2;  /* end y  position of lunar calendar title1 month */
UI_font_type calendar_title_month_font;
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

S32 cat82_title2_x1;    /* start x position of lunar calendar title2 */
S32 cat82_title2_y1;    /* start y position of lunar calendar title2 */
S32 cat82_title2_x2;    /* end x position of lunar calendar title2 */
S32 cat82_title2_y2;    /* end y position of lunar calendar title2 */
S32 cat82_title3_x1;    /* start x position of lunar calendar title3 */
S32 cat82_title3_y1;    /* start y position of lunar calendar title3 */
S32 cat82_title3_x2;    /* end x position of lunar calendar title3 */
S32 cat82_title3_y2;    /* end y  position of lunar calendar title 3 */

/* global variable store the calendar bkg image */
PU8 calendar_control_background_image = NULL;

/* global variable store the calendar title image */
PU8 calendar_control_title_image = NULL;

/* global variable store strat y position of calndar bkg */
S32 calendar_control_background_y1 = 0;

/* global variable store end y position of calndar bkg */
S32 calendar_control_background_y2 = 0;

/* store calendar popup start x position */
S32 *wgui_calendar_ppx1 = NULL;

/* store calendar popup end x position */
S32 *wgui_calendar_ppx2 = NULL;

/* store calendar popup start y position */
S32 *wgui_calendar_ppy1 = NULL;

/* store calendar popup end y  position */
S32 *wgui_calendar_ppy2 = NULL;

/* current day */
S32 wgui_calendar_pp_day = -1;

/* Health calendar message1 */
UI_string_type cat81_message1 = NULL;

/* Health calendar message2 */
UI_string_type cat81_message2 = NULL;

#ifdef __MMI_MAINLCD_220X176__
/* global variable store strat x position of calndar bkg */
#define calendar_control_background_begin_x  0
/* global variable store strat y position of calndar bkg */
#define calendar_control_background_begin_y 0//MMI_STATUS_BAR_HEIGHT-2

#endif

/* Functions to setup the background image and display theme for the calendar */
/* theme colr of calendar normal day */
const UI_filled_area _calendar_normal_day_filler = {UI_FILLED_AREA_TYPE_NO_BACKGROUND,
    NULL,
    NULL,
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    0
};

/* theme color of calendar task day */
const UI_filled_area _calendar_task_day_filler = {UI_FILLED_AREA_TYPE_NO_BACKGROUND | UI_FILLED_AREA_SINGLE_BORDER,
    NULL,
    NULL,
#if defined(__MMI_MAINLCD_160X128__)
    {100, 100, 0, 0},
#else
    {0, 0, 0, 0},
#endif
    {0, 0, 0, 0},
    {255, 0, 0, 100},
    {0, 0, 0, 0},
    0
};

/* theme color of calendar selected  day */
const UI_filled_area _calendar_selected_day_filler = {UI_FILLED_AREA_TYPE_COLOR,
    NULL,
    NULL,
    {20, 124, 86, 100},
    {0, 0, 0, 0},
    {255, 0, 0, 100},
    {0, 0, 0, 0},
    0
};

/* theme color of calendar current  day */
const UI_filled_area _calendar_current_day_filler = {UI_FILLED_AREA_TYPE_NO_BACKGROUND | UI_FILLED_AREA_SINGLE_BORDER,
    NULL,
    NULL,
    {20, 124, 86, 100},
    {0, 0, 0, 0},
    {0, 192, 0, 100},
    {0, 0, 0, 0},
    0
};

/* special day theme color , Move to wgui_calendar.c */
const UI_filled_area _calendar_special1_day_filler = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    NULL,
    NULL,
    {255, 100, 100, 100},
    {0, 0, 0, 0},
    {255, 100, 100, 100},
    {0, 0, 0, 0},
    0
};

#ifdef __MMI_MAINLCD_220X176__
const UI_filled_area _calendar_special2_day_filler = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    NULL,
    NULL,
    {0, 255, 0, 100},
    {0, 0, 0, 0},
    {0, 255, 0, 100},
    {0, 0, 0, 0},
    0
};
#else
const UI_filled_area _calendar_special2_day_filler = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    NULL,
    NULL,
    {255, 200, 100, 100},
    {0, 0, 0, 0},
    {255, 200, 100, 100},
    {0, 0, 0, 0},
    0
};
#endif

const UI_filled_area _calendar_special3_day_filler = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    NULL,
    NULL,
    {255, 255, 100, 100},
    {0, 0, 0, 0},
    {255, 255, 100, 100},
    {0, 0, 0, 0},
    0
};

const UI_filled_area _calendar_special4_day_filler = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    NULL,
    NULL,
    {205, 234, 180, 100},
    {0, 0, 0, 0},
    {151, 171, 160, 100},
    {0, 0, 0, 0},
    0
};

/* theme color of calendar  */
calendar_theme _calendar_control_theme_data = 
{
    &MMI_small_font,
#if defined(MEM_ULC_3216) || defined(__MMI_USING_BLACK_THEME_BG__)
	{255, 255, 255, 100},
#else
    {0, 0, 0, 100},
#endif
    {0, 0, 0, 100},
#if defined(MEM_ULC_3216) || defined(__MMI_USING_BLACK_THEME_BG__)
	{255, 255, 255, 100},
#else
	{0, 0, 0, 100},
#endif
    {255, 255, 255, 100},
    {200, 0, 0, 100},
    {0, 128, 0, 100},
#if !defined(__MMI_MAINLCD_160X128__)
    {0, 0, 0, 100},
    {0, 0, 0, 100},
    {0, 0, 0, 100},
    {0, 0, 0, 100},
#else
    {255, 255, 255, 100},
    {255, 255, 255, 100},
    {255, 255, 255, 100},
    {255, 255,255, 100},
#endif
    (UI_filled_area *)&_calendar_normal_day_filler,
    (UI_filled_area *)&_calendar_normal_day_filler,
    (UI_filled_area *)&_calendar_task_day_filler,
    (UI_filled_area *)&_calendar_current_day_filler,
    (UI_filled_area *)&_calendar_selected_day_filler,
    (UI_filled_area *)&_calendar_normal_day_filler,
    (UI_filled_area *)&_calendar_normal_day_filler,
    (UI_filled_area *)&_calendar_special1_day_filler,
    (UI_filled_area *)&_calendar_special2_day_filler,
    (UI_filled_area *)&_calendar_special3_day_filler,
    (UI_filled_area *)&_calendar_special4_day_filler,
};


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_function_S32 S32S32
 * DESCRIPTION
 *  
 * PARAMETERS
 *  day         [IN]        
 *  month       [IN]        
 *  year        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_dummy_function_S32S32S32(S32 day, S32 month, S32 year)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(day);
    UI_UNUSED_PARAMETER(month);
    UI_UNUSED_PARAMETER(year);
}

#ifdef __MMI_UI_DALMATIAN_CALENDAR__
 
#if ( defined (__MMI_MAINLCD_128X128__) || defined (__MMI_MAINLCD_128X160__)  || defined(__MMI_MAINLCD_160X128__))
#define DALMATIAN_CALENDAR_WEEK_IMAGE_START 2
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#elif defined( __MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#define DALMATIAN_CALENDAR_WEEK_IMAGE_START 4
#elif defined( __MMI_MAINLCD_220X176__)
#define DALMATIAN_CALENDAR_WEEK_IMAGE_START 4
#endif 
 
static S32 gdalmatian_current_month = 0;
static S32 gdalmatian_current_year = 0;
static S32 gdalmatian_current_day = 0;
static S32 gdalmatian_week_num = 1;
static S32 gdalmatian_calendar_highlighter = 1;

wgui_dalmatian_calendar_highlighter_styles dalmatian_calendar_highlighter_style =
    DALMATIAN_CALENDAR_HIGHLIGHTER_MOVE_WITH_SELECT;

UI_filled_area *backup_selected_day_filler;
UI_filled_area image_box_selected_day_filler;

typedef struct _calendar_highlighter
{
    color_t horizontal;
    color_t vertical;
} calendar_highlighter;

typedef struct _calendar_highlighter_box
{
    color_t box_color;
} calendar_highlighter_box;

/* Use diff structure for make seperation easy if required i.e. if border is not required */
typedef struct _calendar_highlighter_box_border
{
    color_t box_border_color;
} calendar_highlighter_box_border;

calendar_highlighter color_opacity_lookup_Table_calendar_highlighter[] = 
{
    {{51, 126, 184, 50}, {141, 165, 186, 60}},
    {{253, 72, 53, 50}, {253, 113, 113, 60}},
    {{253, 146, 60, 50}, {251, 209, 82, 60}},
    {{156, 93, 51, 50}, {253, 212, 143, 60}},
    {{251, 216, 51, 50}, {248, 183, 8, 60}},
    {{153, 209, 56, 50}, {220, 252, 2, 60}},
    {{95, 184, 51, 50}, {171, 249, 36, 60}},
    {{54, 233, 253, 50}, {80, 198, 247, 60}},
    {{251, 154, 184, 50}, {253, 52, 114, 60}},
    {{160, 125, 204, 50}, {177, 78, 188, 60}},
    {{160, 56, 81, 50}, {215, 65, 100, 60}},
    {{51, 126, 184, 50}, {141, 165, 186, 60}}
};

calendar_highlighter_box color_opacity_lookup_Table_calendar_highlighter_box[] = 
{
    {128, 128, 128, 100},
    {214, 4, 4, 100},
    {252, 154, 13, 100},
    {132, 54, 3, 100},
    {250, 119, 3, 100},
    {92, 143, 4, 100},
    {108, 174, 5, 100},
    {4, 120, 169, 100},
    {200, 4, 64, 100},
    {173, 86, 247, 100},
    {205, 25, 63, 100},
    {128, 128, 128, 100}
};

calendar_highlighter_box_border color_opacity_lookup_Table_calendar_highlighter_box_border[] = 
{
    {1, 6, 0, 100},
    {3, 0, 0, 100},
    {168, 69, 3, 100},
    {1, 6, 0, 100},
    {90, 28, 1, 100},
    {1, 6, 0, 100},
    {11, 53, 2, 100},
    {2, 46, 90, 100},
    {102, 4, 50, 100},
    {37, 3, 51, 100},
    {90, 28, 1, 100},
    {1, 6, 0, 100}
};


/*****************************************************************************
 * FUNCTION
 *  wgui_read_calendar_highlighter_horizontal_color
 * DESCRIPTION
 *  Returns the highlighter color of horizontal bar
 * PARAMETERS
 *  month       [IN]        
 * RETURNS
 *  color
 *****************************************************************************/
color_t wgui_read_calendar_highlighter_horizontal_color(S32 month)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return color_opacity_lookup_Table_calendar_highlighter[month].horizontal;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_read_calendar_highlighter_vertical_color
 * DESCRIPTION
 *  Returns the highlighter color of vertical bar
 * PARAMETERS
 *  month       [IN]        
 * RETURNS
 *  color
 *****************************************************************************/
color_t wgui_read_calendar_highlighter_vertical_color(S32 month)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return color_opacity_lookup_Table_calendar_highlighter[month].vertical;
}


/*****************************************************************************
 * FUNCTION
 *  set_current_month_year_for_dalmatian
 * DESCRIPTION
 *  Set the globel values for the current date
 * PARAMETERS
 *  current_day         [IN]        S32 current_month S32 current_year
 *  current_month       [IN]        
 *  current_year        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_current_month_year_for_dalmatian(S32 current_day, S32 current_month, S32 current_year)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdalmatian_current_day = current_day;
    gdalmatian_current_month = current_month;
    gdalmatian_current_year = current_year;
}


/*****************************************************************************
 * FUNCTION
 *  draw_current_week_highlighter
 * DESCRIPTION
 *  Draw the week highlighter for current date
 * PARAMETERS
 *  c       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void draw_current_week_highlighter(calendar_control *c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, x2, y1, y2;
    S32 cell_width = c->cell_width;
    S32 cell_height = c->cell_height;
    S32 n_rows = c->n_rows;
    S32 n_columns = c->n_columns;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_calendar_current_month == gdalmatian_current_month && MMI_calendar_current_year == gdalmatian_current_year)
    {
        get_current_date_highlighter_coordinates(&MMI_calendar_control, &x1, &y1);
        x2 = x1 + cell_width;
        y2 = y1 + cell_height - 1;
         
    #ifdef __MMI_UI_TRANSPARENT_EFFECT_IN_DALMATIAN_CALENDAR__
         
        gui_transparent_color_filler(
            x1 - 1,
            c->y,
            x2,
            c->y2,
            wgui_read_calendar_highlighter_vertical_color(MMI_calendar_current_month - 1));
        gui_transparent_color_filler(
            c->x + 1,
            y1 + 1,
            c->x + ((cell_width) * n_columns) - 1,
            y2 + 1,
            wgui_read_calendar_highlighter_horizontal_color(MMI_calendar_current_month - 1));
    #else /* __MMI_UI_TRANSPARENT_EFFECT_IN_DALMATIAN_CALENDAR__ */ 
        gui_draw_rectangle(
            x1 - 1,
            c->y,
            x2,
            c->y2,
            wgui_read_calendar_highlighter_vertical_color(MMI_calendar_current_month - 1));
        gui_draw_rectangle(
            c->x + 1,
            y1 + 1,
            c->x + ((cell_width) * n_columns) - 1,
            y2 + 1,
            wgui_read_calendar_highlighter_horizontal_color(MMI_calendar_current_month - 1));
    #endif /* __MMI_UI_TRANSPARENT_EFFECT_IN_DALMATIAN_CALENDAR__ */ 
        /* For current date reselection. */
        gui_draw_rectangle(x1, y1 + 1, x2 - 1, y2 + 1, MMI_calendar_control.theme->current_day_filler->c);
        return;
    }
}


/*****************************************************************************
 * FUNCTION
 *  get_current_date_highlighter_coordinates
 * DESCRIPTION
 *  Return the date highlighter coordinates
 * PARAMETERS
 *  c       [IN]        
 *  x       [OUT]       
 *  y       [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
void get_current_date_highlighter_coordinates(calendar_control *c, S32 *x, S32 *y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j, x1, x2, y1, y2;
    S32 cell_width = c->cell_width;
    S32 cell_height = c->cell_height;
    S32 ox = c->x1;
    S32 oy = c->y1;
    S32 n_rows = c->n_rows;
    S32 n_columns = c->n_columns;
    S32 index = 0;
    U8 *list_of_items = c->list_of_items;
    S32 current_day = 1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    y1 = oy;
    for (j = 0; j < n_rows; j++)
    {
        x1 = ox;
        for (i = 0; i < n_columns; i++)
        {
            x2 = x1 + cell_width - 1;
            y2 = y1 + cell_height - 1;
            if ((index >= c->first_item) && (index <= c->last_item))
            {
                if ((list_of_items[current_day] & 0x3f) == CALENDAR_CURRENT_DAY)
                {
                    *x = x1;
                    *y = y1;
                    return;
                }
                current_day++;
            }
            x1 += cell_width;
            index++;
        }
        y1 += cell_height;
    }

}


/*****************************************************************************
 * FUNCTION
 *  draw_current_day_highlighter
 * DESCRIPTION
 *  Draw the date highlighter for current date
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_current_day_highlighter(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    color_t day_highlighter_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    day_highlighter_color = MMI_calendar_control.theme->current_day_filler->c;
    get_current_date_highlighter_coordinates(&MMI_calendar_control, &x1, &y1);
    x2 = x1 + MMI_calendar_control.cell_width;
    y2 = y1 + MMI_calendar_control.cell_height - 1;
    gui_draw_rectangle(x1, y1 + 1, x2 - 1, y2 + 1, day_highlighter_color);

}


/*****************************************************************************
 * FUNCTION
 *  draw_current_week_highlighter_with_current_selection
 * DESCRIPTION
 *  Draw the week highlighter for current date selected
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_current_week_highlighter_with_current_selection(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 row, col;
    S32 x11, y11, x21, y21;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_push_clip();
    gui_reset_clip();

     
#if	defined (__MMI_MAINLCD_128X160__) || defined (__MMI_MAINLCD_128X128__)
    gui_push_clip();
    gui_set_clip(0, 0, MMI_calendar_control.x2, MMI_calendar_control.y2);
#endif /* __MMI_MAINLCD_128X160__ */ 
     
#ifdef __MMI_MAINLCD_220X176__
    gui_show_image(calendar_control_background_begin_x, calendar_control_background_begin_y, calendar_control_background_image);
#else
    gui_show_image(0, 0, calendar_control_background_image);
#endif
    gui_show_calendar(&MMI_calendar_control);
     
#if	defined (__MMI_MAINLCD_128X160__) || defined (__MMI_MAINLCD_128X128__)
    gui_pop_clip();
#endif 
     

    row = MMI_calendar_control.highlighted_item / MMI_calendar_control.n_columns;
    col = MMI_calendar_control.highlighted_item % MMI_calendar_control.n_columns;

    x11 = MMI_calendar_control.x1 + MMI_calendar_control.cell_width * col;
    y11 = MMI_calendar_control.y1 + MMI_calendar_control.cell_height * row;
    x21 = x11 + MMI_calendar_control.cell_width - 1;
    y21 = y11 + MMI_calendar_control.cell_height - 1;
     
#ifdef __MMI_UI_TRANSPARENT_EFFECT_IN_DALMATIAN_CALENDAR__
     
    gui_transparent_color_filler(
        x11 - 1,
        MMI_calendar_control.y,
        x21,
        MMI_calendar_control.y2,
        wgui_read_calendar_highlighter_vertical_color(MMI_calendar_current_month - 1));
    gui_transparent_color_filler(
        MMI_calendar_control.x + 1,
        y11 + 1,
        MMI_calendar_control.x + ((MMI_calendar_control.cell_width) * MMI_calendar_control.n_columns) - 1,
        y21 + 1,
        wgui_read_calendar_highlighter_horizontal_color(MMI_calendar_current_month - 1));
#else /* __MMI_UI_TRANSPARENT_EFFECT_IN_DALMATIAN_CALENDAR__ */ 
    gui_draw_rectangle(
        x11 - 1,
        MMI_calendar_control.y,
        x21,
        MMI_calendar_control.y2,
        wgui_read_calendar_highlighter_vertical_color(MMI_calendar_current_month - 1));
    gui_draw_rectangle(
        MMI_calendar_control.x + 1,
        y11 + 1,
        MMI_calendar_control.x + ((MMI_calendar_control.cell_width) * MMI_calendar_control.n_columns) - 1,
        y21 + 1,
        wgui_read_calendar_highlighter_horizontal_color(MMI_calendar_current_month - 1));
#endif /* __MMI_UI_TRANSPARENT_EFFECT_IN_DALMATIAN_CALENDAR__ */ 
    gui_calendar_refresh_highlight(&MMI_calendar_control, 0, MMI_calendar_control.highlighted_item);
    draw_current_day_highlighter();

    gdi_layer_pop_clip();
    gdi_layer_unlock_frame_buffer();
}


/*****************************************************************************
 * FUNCTION
 *  set_dalmatian_calendar_highlighter_box
 * DESCRIPTION
 *  Draw the highlighter box for current date selected
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_dalmatian_calendar_highlighter_box(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* for color filler */
    backup_selected_day_filler = MMI_calendar_control.theme->selected_day_filler;
    image_box_selected_day_filler = *backup_selected_day_filler;
    MMI_calendar_control.theme->selected_day_filler = &image_box_selected_day_filler;
    MMI_calendar_control.theme->selected_day_filler->c =
        color_opacity_lookup_Table_calendar_highlighter_box[MMI_calendar_current_month - 1].box_color;
    MMI_calendar_control.theme->selected_day_filler->border_color =
        color_opacity_lookup_Table_calendar_highlighter_box_border[MMI_calendar_current_month - 1].box_border_color;
    MMI_calendar_control.theme->selected_day_filler->flags = UI_FILLED_AREA_TYPE_COLOR;
    MMI_calendar_control.theme->selected_day_filler->flags =
        MMI_calendar_control.theme->selected_day_filler->flags | UI_FILLED_AREA_SINGLE_BORDER;
}


/*****************************************************************************
 * FUNCTION
 *  dalmatian_calendar_current_week_image
 * DESCRIPTION
 *  Draw the week highlighter image for current date selected
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
U16 dalmatian_calendar_current_week_image(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_JAN + MMI_calendar_current_month - 1);
}


/*****************************************************************************
 * FUNCTION
 *  set_dalmatian_calendar_highlighter_enable
 * DESCRIPTION
 *  It enable the flag gdalmatian_calendar_highlighter
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_dalmatian_calendar_highlighter_enable(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdalmatian_calendar_highlighter = 1;
}


/*****************************************************************************
 * FUNCTION
 *  set_dalmatian_calendar_highlighter_disable
 * DESCRIPTION
 *  It disable the flag gdalmatian_calendar_highlighter
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_dalmatian_calendar_highlighter_disable(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdalmatian_calendar_highlighter = 0;
}


/*****************************************************************************
 * FUNCTION
 *  dalmatian_calendar_highlighter_status
 * DESCRIPTION
 *  It returns the value of flag gdalmatian_calendar_highlighter
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
S32 dalmatian_calendar_highlighter_status(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdalmatian_calendar_highlighter;
}


/*****************************************************************************
 * FUNCTION
 *  set_dalmatian_current_week_num
 * DESCRIPTION
 *  It returns the value of flag gdalmatian_calendar_highlighter
 * PARAMETERS
 *  void
 * RETURNS
 *  S32(?)
 *****************************************************************************/
void set_dalmatian_current_week_num(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, no_of_days, is_leap, week_day, numdays_inmonth, day_on_firstjan, day_on_last_day;
    calendar_control *c = &MMI_calendar_control;
    S32 handler_sunday_saterday = 2;
    S32 no_of_days_inweek = 7;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    no_of_days = 0;
    is_leap = 0;

    for (i = MMI_calendar_current_month - 1; i > 0; i--)
    {

        /* get the num of days in todays month */
        numdays_inmonth = LastDayOfMonth((U8) i, (U16) MMI_calendar_current_year);
        no_of_days += numdays_inmonth;
    }

    day_on_firstjan = DOW(MMI_calendar_current_year, 1, 1);
    day_on_last_day = DOW(MMI_calendar_current_year, (U8) MMI_calendar_current_month, 1);

    /* Total no of days/ weekdays */

    no_of_days += (day_on_firstjan - handler_sunday_saterday + c->highlighted_item - day_on_last_day + 1);

    week_day = (no_of_days) / no_of_days_inweek;

    gdalmatian_week_num = week_day + 1;

}


/*****************************************************************************
 * FUNCTION
 *  set_dalmatian_title_string
 * DESCRIPTION
 *  It sets the Title string
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_dalmatian_title_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 tmpString[CLNDR_TITLE_LEN];
    S8 tmpString1[CLNDR_TITLE_LEN];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    set_dalmatian_current_week_num();
     
    memset(MMI_calendar_title_string, 0, sizeof(MMI_calendar_title_string));
     
    //String should be fit to all screen sizes
    sprintf(tmpString, " %d ", MMI_calendar_current_year);
    AnsiiToUnicodeString(tmpString1, tmpString);
    UCS2Strcat((S8*) MMI_calendar_title_string, (S8*) tmpString1);
    UCS2Strcat((S8*) MMI_calendar_title_string, (S8*) get_string((UI_image_ID_type ) (STR_GUI_MONTH_BASE + MMI_calendar_current_month)));    
    sprintf(tmpString, " (%d)", gdalmatian_week_num);
     
    AnsiiToUnicodeString(tmpString1, tmpString);
    UCS2Strcat((S8*) MMI_calendar_title_string, (S8*) tmpString1);

    MMI_title_string = (UI_string_type) MMI_calendar_title_string;

    if (!(lunar_calendar_present && IsChineseSet()))
    {
        SetCategory82Title1((U8*) MMI_title_string);
    }
    else
    {
        Cat82_title2 = (U8*) MMI_calendar_title_string;
    }
}
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__


/*****************************************************************************
 * FUNCTION
 *  shuffle_softkey_draw_dalmatian_calender_image
 * DESCRIPTION
 *  shuffle the softkey by drawing the calendar background
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void shuffle_softkey_draw_dalmatian_calender_image(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MAINLCD_220X176__	
    gui_show_image(calendar_control_background_begin_x, calendar_control_background_begin_y, calendar_control_background_image);
#else
    gui_show_image(0, 0, calendar_control_background_image);
#endif
}
#endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCalendarCommomCategoryScreen
 * DESCRIPTION
 *  key board handler of calendar
 * PARAMETERS
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  history_buffer          [?]         
 *  S32(?)                  [IN]        Key_state  key state
 * RETURNS
 *  void
 *****************************************************************************/
#include"mainmenudef.h"
void ShowCalendarCommomCategoryScreen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 tmpString[CLNDR_TITLE_LEN];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_SCREEN_STRINGS((2, left_softkey, right_softkey));
    ADD_SCREEN_IMAGES((2, left_softkey_icon, right_softkey_icon));
    entry_full_screen();
    UI_UNUSED_PARAMETER(history_buffer);
    gdi_layer_lock_frame_buffer();

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    memset(MMI_calendar_title_string, 0, sizeof(MMI_calendar_title_string));
    memset(MMI_calendar_title_string_month, 0, sizeof(MMI_calendar_title_string));
    sprintf(tmpString, "%d", MMI_calendar_current_year);
    AnsiiToUnicodeString((S8*) MMI_calendar_title_string, tmpString);
    if (IsChineseSet())
    {
        sprintf(tmpString, "%d", MMI_calendar_current_month);
        AnsiiToUnicodeString((S8*) MMI_calendar_title_string_month, tmpString);
    }
    else
    {
        pfnUnicodeStrcpy(
            (S8*) MMI_calendar_title_string_month,
            GetString((U16) (STR_GUI_MONTH_BASE + MMI_calendar_current_month)));
    }
#else /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
    memset(MMI_calendar_title_string, 0, sizeof(MMI_calendar_title_string));
    if (IsChineseSet())
    {
        sprintf(tmpString, "%d %02d", MMI_calendar_current_year, MMI_calendar_current_month);
        AnsiiToUnicodeString((S8*) MMI_calendar_title_string, tmpString);
    }
    else
    {
        sprintf(tmpString, "%d ", MMI_calendar_current_year);
        AnsiiToUnicodeString((S8*) MMI_calendar_title_string, tmpString);
        pfnUnicodeStrcat(
            (S8*) MMI_calendar_title_string,
            GetString((U16) (STR_GUI_MONTH_BASE + MMI_calendar_current_month)));
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

    if (lunar_calendar_present)
    {
        Cat82_title2 = MMI_calendar_title_string;
    }
    else
    {
        MMI_title_string = (UI_string_type) MMI_calendar_title_string;
    }

#if defined(__MMI_USING_BLACK_THEME_BG__)
	SetCalendarBackground(IMG_MAIN_MENU_MATRIX_BG);
#elif (defined(MEM_ULC_3216) || defined(__MMI_RESOURCE_SLIM_ON_128X160__))
	SetCalendarBackground((U16)WALLPAPER_1);
#elif defined (__MMI_MAINLCD_220X176__) 
	SetCalendarBackground(IMG_MAIN_MENU_BG1);
#else
#ifdef __MMI_SLIM_CALENDAR__
    SetCalendarBackground((U16) IMG_CALENDAR_JAN);
#else 
    SetCalendarBackground((U16) (IMG_CALENDAR_JAN + MMI_calendar_current_month - 1));
#endif 
#endif


    clear_category_screen_key_handlers();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();
    /* register calendat key handlers */
    wgui_register_calendar_keys();
    gdi_layer_unlock_frame_buffer();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_keyboard_key_handler
 * DESCRIPTION
 *  key board handler of calendar
 * PARAMETERS
 *  vkey_code       [IN]        Key code
 *  key_state       [IN]        Key state
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_keyboard_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
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
            case 37:
                gui_calendar_previous_day(&MMI_calendar_control);       /* move to previous day */
                break;
            case 39:
                gui_calendar_next_day(&MMI_calendar_control);   /* move to next day */
                break;
            case 38:
                gui_calendar_previous_row(&MMI_calendar_control);       /* move to previous row */
                break;
            case 40:
                gui_calendar_next_row(&MMI_calendar_control);   /* move to next row */
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_control_highlight_handler
 * DESCRIPTION
 *  highlight handler of calendar
 * PARAMETERS
 *  day     [IN]        Selected day
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_control_highlight_handler(S32 day)
{   /* store selected day */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_calendar_highlighted_day = day;
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    if (dalmatian_calendar_highlighter_status() == 1)
    {
        set_dalmatian_calendar_highlighter_box();
    }
    if (dalmatian_calendar_highlighter_style == DALMATIAN_CALENDAR_HIGHLIGHTER_MOVE_WITH_SELECT &&
        dalmatian_calendar_highlighter_status() == 1)
    {
        draw_current_week_highlighter_with_current_selection();
    }
    set_dalmatian_current_week_num();
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    /* highlight the selected day */
    MMI_calendar_control_highlight_handler(day);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_control_update_handler
 * DESCRIPTION
 *  update the content of the calendar control
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void wgui_calendar_control_update_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 DayOnFirst, NumDaysInMonth;
    S32 i;
    S8 tmpString[CLNDR_TITLE_LEN];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DayOnFirst = DOW(MMI_calendar_current_year, (U8) MMI_calendar_current_month, 1);
    /* get the num of days in todays month */
    NumDaysInMonth = LastDayOfMonth((U8) MMI_calendar_current_month, MMI_calendar_current_year);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    memset(MMI_calendar_title_string, 0, sizeof(MMI_calendar_title_string));
    memset(MMI_calendar_title_string_month, 0, sizeof(MMI_calendar_title_string));
    sprintf(tmpString, "%d", MMI_calendar_current_year);
    AnsiiToUnicodeString((S8*) MMI_calendar_title_string, tmpString);
    if (IsChineseSet())
    {
        if (lunar_calendar_present)
        {
            Cat82_title2 = MMI_calendar_title_string;
        }
        else
        {
            MMI_title_string = (UI_string_type) MMI_calendar_title_string;
        }
        sprintf(tmpString, "%d", MMI_calendar_current_month);
        AnsiiToUnicodeString((S8*) MMI_calendar_title_string_month, tmpString);
    }
    else
    {
        MMI_title_string = (UI_string_type) MMI_calendar_title_string;
        pfnUnicodeStrcpy(
            (S8*) MMI_calendar_title_string_month,
            GetString((U16) (STR_GUI_MONTH_BASE + MMI_calendar_current_month)));
    }
#else /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
    memset(MMI_calendar_title_string, 0, sizeof(MMI_calendar_title_string));
    if (IsChineseSet())
    {
        sprintf(tmpString, "%d %02d", MMI_calendar_current_year, MMI_calendar_current_month);
        AnsiiToUnicodeString((S8*) MMI_calendar_title_string, tmpString);
        if (lunar_calendar_present)
        {
            Cat82_title2 = MMI_calendar_title_string;
        }
        else
        {
            MMI_title_string = (UI_string_type) MMI_calendar_title_string;
        }
    }
    else
    {
        sprintf(tmpString, "%d ", MMI_calendar_current_year);
        AnsiiToUnicodeString((S8*) MMI_calendar_title_string, tmpString);
        pfnUnicodeStrcat(
            (S8*) MMI_calendar_title_string,
            GetString((U16) (STR_GUI_MONTH_BASE + MMI_calendar_current_month)));
        MMI_title_string = (UI_string_type) MMI_calendar_title_string;
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

#if defined(__MMI_USING_BLACK_THEME_BG__)
	SetCalendarBackground(IMG_MAIN_MENU_MATRIX_BG);
#elif (defined(MEM_ULC_3216) || defined(__MMI_RESOURCE_SLIM_ON_128X160__))
    SetCalendarBackground((U16)WALLPAPER_1);	
#elif defined __MMI_MAINLCD_220X176__	
	calendar_control_background_image = get_image((U16) IMG_MAIN_MENU_BG1);
#else
#ifdef __MMI_SLIM_CALENDAR__
    calendar_control_background_image = get_image((U16) IMG_CALENDAR_JAN);
#else 
    calendar_control_background_image = get_image((U16) (IMG_CALENDAR_JAN + MMI_calendar_current_month - 1));
#endif 
#endif


    for (i = 0; i < MAX_CALENDAR_DAYS; i++)
    {
        MMI_calendar_days[i] = CALENDAR_NORMAL_DAY; /* Clear all flags before updating */
    }

    MMI_calendar_control_update_handler(
        MMI_calendar_highlighted_day,
        MMI_calendar_current_month,
        MMI_calendar_current_year);

    gui_set_calendar_days(&MMI_calendar_control, DayOnFirst, NumDaysInMonth, MMI_calendar_days);

    if (IsChineseSet() && lunar_calendar_present)
    {
        RedrawCategory82Screen();
    }
    else
    {
        RedrawCategory80Screen();
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_display_calendar_pop_up
 * DESCRIPTION
 *  create popup dialog box of calendar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_display_calendar_pop_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* These can be obtained from theme data  */
    const UI_filled_area popup_filler = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
        UI_NULL_IMAGE,
        NULL,
        {47, 55, 143, 80},
        {0, 0, 0, 0},
        {0, 0, 0, 100},
        {0, 0, 0, 0},
        0
    };
    color_t tc = *(current_MMI_theme->UI_white_color);
    UI_font_type text_font = &MMI_medium_font;

    S32 x1 = *wgui_calendar_ppx1;
    S32 y1 = *wgui_calendar_ppy1;
    S32 x2 = *wgui_calendar_ppx2;
    S32 px1, py1, px2, py2;
    S32 width, height, y;
    S32 w1, w2, w3, h1, h2, h3;

    /* set message 1 of popup */
    UI_string_type msg1 = (UI_string_type) MMI_calendar_messages1;

    /* set message 2 of popup */
    UI_string_type msg2 = (UI_string_type) MMI_calendar_messages2;

    /* set message 3 of popup */
    UI_string_type msg3 = (UI_string_type) MMI_calendar_messages3;

#ifdef __MMI_MAINLCD_160X128__
	if (y1 > 128-48-20)
		y1= 128-48-20;
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* set font of popup */
    gui_set_font(text_font);
    if ((msg1 == NULL) && (msg2 == NULL) && (msg3 == NULL))
    {
        MMI_calendar_control.flags &= ~CALENDAR_POPUP_BACKUP;
        return;
    }
    if (msg1 == NULL)
    {
        h1 = 0;
        w1 = 0;
    }
    else
    {
        gui_measure_string(msg1, &w1, &h1); /* measue width and height of msg1 */
    }
    if (msg2 == NULL)
    {
        h2 = 0;
        w2 = 0;
    }
    else
    {
        gui_measure_string(msg2, &w2, &h2); /* measue width and height of msg1 */
    }
    if (msg3 == NULL)
    {
        h3 = 0;
        w3 = 0;
    }
    else
    {
        gui_measure_string(msg3, &w3, &h3); /* measue width and height of msg1 */
    }
    height = h1 + h2 + h3 + 8;
    width = w1;
    if (w2 > width)
    {
        width = w2;
    }
    if (w3 > width)
    {
        width = w3;
    }
    width += 8;
    if (width > CALENDAR_POPUP_WIDTH)
    {
        width = CALENDAR_POPUP_WIDTH;
    }
    if (height > CALENDAR_POPUP_HEIGHT)
    {
        height = CALENDAR_POPUP_HEIGHT;
    }
#if(CALENDAR_POPUP_FIXED_POSITION)
    /* Not complete: Overlaps need to be handled */
#if(CALENDAR_POPUP_FIXED_RIGHT)
    px1 = CALENDAR_POPUP_X - width;
#else 
    px1 = CALENDAR_POPUP_X;
#endif 
#if(CALENDAR_POPUP_FIXED_BOTTOM)
    py1 = CALENDAR_POPUP_Y - height;
#else 
    py1 = CALENDAR_POPUP_Y;
#endif 
#else /* (CALENDAR_POPUP_FIXED_POSITION) */ 
    px1 = x2 + CALENDAR_POPUP_X;
    py1 = y1 + CALENDAR_POPUP_Y;
#endif /* (CALENDAR_POPUP_FIXED_POSITION) */ 
    if ((px1 + width - 1) > MMI_calendar_control.x2)
    {
        px1 = x1 - width - CALENDAR_POPUP_X;
        if (px1 < MMI_calendar_control.x1)
        {
            px1 = MMI_calendar_control.x1;
        }
    }
    if ((py1 + height - 1) > MMI_calendar_control.y2)
    {
        py1 = MMI_calendar_control.y2 - height;
        if (py1 < MMI_calendar_control.y1)
        {
            py1 = MMI_calendar_control.y1;
        }
    }
    px2 = px1 + width - 1;
    py2 = py1 + height - 1;
    gui_reset_clip();
    /* Temporarily disabled pop up bitmap backup since there is no available buffer to use on hardware    */
    /* _get_bitmap(px1,py1,px2,py2,&MMI_calendar_control.popup_save_bitmap);                        */
    MMI_calendar_control.flags |= CALENDAR_POPUP_BACKUP;
    /* fill the color of popup */
    gui_draw_filled_area(px1, py1, px2, py2, (UI_filled_area*)&popup_filler);
    gdi_layer_set_text_clip(px1, py1, px2, py2);
    /* set text color */
    gui_set_text_color(tc);
    y = py1 + 4;
    if (msg1 != NULL)
    {
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(px1 + 4 + w1, y);
        }
        else
        {
            gui_move_text_cursor(px1 + 4, y);
        }
        gui_print_text(msg1);
        y += h1;
    }
    if (msg2 != NULL)
    {
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(px1 + 4 + w2, y);
        }
        else
        {
            gui_move_text_cursor(px1 + 4, y);
        }
        gui_print_text(msg2);
        y += h2;
    }
    if (msg3 != NULL)
    {
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(px1 + 4 + w3, y);
        }
        else
        {
            gui_move_text_cursor(px1 + 4, y);
        }
        gui_print_text(msg3);
        y += h3;
    }
    *wgui_calendar_ppx1 = px1;
    *wgui_calendar_ppy1 = py1;
    *wgui_calendar_ppx2 = px2;
    *wgui_calendar_ppy2 = py2;
    gdi_layer_blt_previous(px1, py1, px2, py2);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_calendar_pop_up
 * DESCRIPTION
 *  Show calendar popup start time of diosplay popup
 * PARAMETERS
 *  day         [IN]        Highlighted day
 *  x1_p        [IN]        Start x position of popup
 *  y1_p        [IN]        Start y position of popup
 *  x2_p        [IN]        End x position of popup
 *  y2_p        [IN]        End y position of popup
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_calendar_pop_up(S32 day, S32 *x1_p, S32 *y1_p, S32 *x2_p, S32 *y2_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_calendar_ppx1 = x1_p;
    wgui_calendar_ppy1 = y1_p;
    wgui_calendar_ppx2 = x2_p;
    wgui_calendar_ppy2 = y2_p;
    wgui_calendar_pp_day = day;
    MMI_calendar_control.flags &= ~CALENDAR_POPUP_BACKUP;
    /* start timer of display popup */
    gui_start_timer(CALENDAR_POPUP_WAIT_TIME, wgui_display_calendar_pop_up);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_setup_calendar
 * DESCRIPTION
 *  create calendar ..set the values of calendar control
 * PARAMETERS
 *  x                   [IN]        Start x position of calendar
 *  y                   [IN]        Start y position of calendar
 *  cell_width          [IN]        One cell width
 *  cell_height         [IN]        One cell height
 *  first_dow           [IN]        First day of month
 *  last_day            [IN]        Last day of month
 *  current_day         [IN]        Current day
 *  current_month       [IN]        Current month
 *  current_year        [IN]        Current year
 *  flags               [IN]        Flags for enable popup
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_setup_calendar(
        S32 x,
        S32 y,
        S32 cell_width,
        S32 cell_height,
        S32 first_dow,
        S32 last_day,
        S32 current_day,
        S32 current_month,
        S32 current_year,
        U32 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* initilize the calendar daya */
    for (i = 0; i < MAX_CALENDAR_DAYS; i++)
    {
        MMI_calendar_days[i] = CALENDAR_NORMAL_DAY;
    }
    /* create calendar */
    gui_create_calendar(&MMI_calendar_control, x, y, CALENDAR_N_COLUMNS, CALENDAR_N_ROWS, cell_width, cell_height);

    MMI_calendar_current_month = current_month;
    MMI_calendar_current_year = current_year;

    /* set the bitmap of calndar */
#ifdef MMI_ON_HARDWARE_P
    MMI_calendar_control.cursor_save_bitmap.data = (U8*) matrix_main_menu_highlight_bitmap_data;
#else 
    MMI_calendar_control.cursor_save_bitmap.data = (U8*) gui_malloc(cell_width * 16 * (cell_height + 1));
#endif 
    gui_set_calendar_days(&MMI_calendar_control, first_dow, last_day, MMI_calendar_days);
    /* set current day */
    MMI_calendar_highlighted_day = current_day;
    if (MMI_calendar_highlighted_day < 1)
    {
        MMI_calendar_highlighted_day = 1;
    }
    if (MMI_calendar_highlighted_day > last_day)
    {
        MMI_calendar_highlighted_day = last_day;
    }
    /* set function pointer of highlight function */
    MMI_calendar_control.item_highlighted_function = wgui_calendar_control_highlight_handler;
    if (flags & CALENDAR_ENABLE_POPUP)  /* check if display popup is enable or not */
    {
        MMI_calendar_control.flags |= CALENDAR_ENABLE_POPUP;
        /* Temporarily disabled pop up bitmap backup since there is no available buffer to use on hardware                */
        MMI_calendar_control.display_popup = wgui_show_calendar_pop_up;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_move_calendar
 * DESCRIPTION
 *  move calendar to particular x,y position
 * PARAMETERS
 *  x       [IN]        Start x position of calendar
 *  y       [IN]        Start y position of calendar
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_move_calendar(S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_move_calendar(&MMI_calendar_control, x, y);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_reset_calendar_params
 * DESCRIPTION
 *  resets calendars to new parameters
 * PARAMETERS
 *  x               [IN]        Start x position of calendar
 *  y               [IN]        Start y position of calendar
 *  n_columns       [IN]        No of columns of calendar
 *  n_rows          [IN]        No of rows of calendar
 *  cell_width      [IN]        Width of each cell of calendar
 *  cell_height     [IN]        Height of each cell of calendar
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_reset_calendar_params(S32 x, S32 y, S32 n_columns, S32 n_rows, S32 cell_width, S32 cell_height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_calendar_control.n_columns = n_columns;
    MMI_calendar_control.n_rows = n_rows;
    MMI_calendar_control.cell_width = cell_width;
    MMI_calendar_control.cell_height = cell_height;
    gui_move_calendar(&MMI_calendar_control, x, y);
}

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__


/*****************************************************************************
 * FUNCTION
 *  wgui_set_calendar_info_box
 * DESCRIPTION
 *  set calendars info box
 * PARAMETERS
 *  InfoBoxX            [IN]        Start x position of infobox
 *  InfoBoxY            [IN]        Start y position of infobox
 *  InfoBoxWidth        [IN]        Width of info box
 *  InfoBoxHeight       [IN]        Hegiht of infobox
 
 *  InfoBoxShow         [IN]        Flag to decide if show info box or not
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_calendar_info_box(
        S32 InfoBoxX,
        S32 InfoBoxY,
        S32 InfoBoxWidth,
        S32 InfoBoxHeight,
        S32 InfoBoxMax,
        U16 InfoBoxShow)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_calendar_control.info_x1 = InfoBoxX;
    MMI_calendar_control.info_y1 = InfoBoxY;
    MMI_calendar_control.info_x2 = InfoBoxX + InfoBoxWidth - 1;
    MMI_calendar_control.info_y2 = InfoBoxY + InfoBoxHeight - 1;
    MMI_calendar_control.max_n_info = InfoBoxMax;

    if (InfoBoxMax)
    {
        MMI_calendar_control.flags |= CALENDAR_SHOW_INFO_BOX;
    }
    else
    {
        MMI_calendar_control.flags &= ~CALENDAR_SHOW_INFO_BOX;
    }
}
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_show_calendar
 * DESCRIPTION
 *  show calendar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_calendar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* show calendar */
    gui_show_calendar(&MMI_calendar_control);
    MMI_calendar_control.highlighted_item = -1;
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    if (dalmatian_calendar_highlighter_style == DALMATIAN_CALENDAR_HIGHLIGHTER_MOVE_WITH_CURRENT &&
        MMI_calendar_current_month == gdalmatian_current_month && (dalmatian_calendar_highlighter_status() == 1))
    {
        draw_current_week_highlighter(&MMI_calendar_control);
    }
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    /* highlight day of calendar */
    gui_calendar_highlight_day(&MMI_calendar_control, MMI_calendar_highlighted_day);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_close_calendar
 * DESCRIPTION
 *  close calndar deallocate the memory allocated by calendar
 *  stop the popup timer
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_close_calendar(void)
{   /* free memory allocated by calendar bitmap */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* set the calendar highlight handler to dummy func */
    MMI_calendar_control_highlight_handler = UI_dummy_function_S32 ;
    MMI_calendar_control_update_handler = UI_dummy_function_S32S32S32;
    if (MMI_calendar_control.flags & CALENDAR_ENABLE_POPUP)
    {
        /* Temporarily disabled pop up bitmap backup since there is no available buffer to use on hardware                */
        gui_cancel_timer(wgui_display_calendar_pop_up); /* stop the timer */
    }

    calendar_control_background_image = NULL;
    calendar_control_title_image = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_previous_day
 * DESCRIPTION
 *  Move highlight to previous day
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_previous_day(void)
{
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    set_dalmatian_calendar_highlighter_box();
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    gui_calendar_previous_day(&MMI_calendar_control);
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width, UI_device_height);
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_next_day
 * DESCRIPTION
 *  Move highlight to next day
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_next_day(void)
{
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    set_dalmatian_calendar_highlighter_box();
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    gui_calendar_next_day(&MMI_calendar_control);
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width, UI_device_height);
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_previous_row
 * DESCRIPTION
 *  Move highlight to previous row
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_previous_row(void)
{
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    set_dalmatian_calendar_highlighter_box();
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    gui_calendar_previous_row(&MMI_calendar_control);
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width, UI_device_height);
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_next_row
 * DESCRIPTION
 *  Move highlight to next row
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_next_row(void)
{
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    set_dalmatian_calendar_highlighter_box();
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    gui_calendar_next_row(&MMI_calendar_control);
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width, UI_device_height);
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_register_calendar_keys
 * DESCRIPTION
 *  register calendar key handlers
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_register_calendar_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_keyboard_key_handler(wgui_calendar_keyboard_key_handler);
    SetKeyHandler(wgui_calendar_previous_day, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(wgui_calendar_next_day, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(wgui_calendar_previous_row, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(wgui_calendar_next_row, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
}

/* High level functions for use by applications */


/*****************************************************************************
 * FUNCTION
 *  SetCalendarPopupTitleLunarFunc
 * DESCRIPTION
 *  set the function pointer of popuptitle of lunar calendar
 * PARAMETERS
 *  popuptitle      [IN]        ) (U8 ,U8 ) a function pointer with 2 U* parmeters
 * RETURNS
 *  void
 *****************************************************************************/
void SetCalendarPopupTitleLunarFunc(void (*popuptitle) (U8, U8, U8, U8))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (popuptitle != NULL)
    {
        MMI_calendar_control.popuptitle_lunarcalendar = popuptitle;
    }
}


/*****************************************************************************
 * FUNCTION
 *  SetCalendarMonthFunctions
 * DESCRIPTION
 *  Register function pointers of months navigation ofcalendar
 * PARAMETERS
 *  month_previous_day      [IN]        )(void) a function pointer which handle navigation to months previous day
 *  month_next_day          [IN]        )(void)     a function pointer which handle navigation to months next  day
 *  month_previous_week     [IN]        )(void)   a function pointer which handle navigation to months previous week
 *  month_next_week         [IN]        )(void)    a function pointer which handle navigation to months next week
 * RETURNS
 *  void
 *****************************************************************************/
void SetCalendarMonthFunctions(
        void (*month_previous_day) (void),
        void (*month_next_day) (void),
        void (*month_previous_week) (void),
        void (*month_next_week) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (month_previous_day != NULL)
    {
        MMI_calendar_control.month_previous_day_function = month_previous_day;
    }
    if (month_next_day != NULL)
    {
        MMI_calendar_control.month_next_day_function = month_next_day;
    }
    if (month_previous_week != NULL)
    {
        MMI_calendar_control.month_previous_week_function = month_previous_week;
    }
    if (month_next_week != NULL)
    {
        MMI_calendar_control.month_next_week_function = month_next_week;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_previous_month
 * DESCRIPTION
 *  Side up key handler, go to 1st day of previous month.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_previous_month(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_calendar_current_year != CLNDR_FIRST_NAVI_YEAR || MMI_calendar_current_month != 1)
    {
        MYTIME myTime, incTime, resultTime;;
        memset(&myTime, 0, sizeof(MYTIME));
        memset(&incTime, 0, sizeof(MYTIME));

        myTime.nDay = 1;
        myTime.nMonth = (U8) MMI_calendar_current_month;
        myTime.nYear = MMI_calendar_current_year;

        incTime.nMonth = 1;

        DecrementTime(myTime, incTime, &resultTime);
        MMI_calendar_highlighted_day = resultTime.nDay;
        MMI_calendar_current_month = resultTime.nMonth;
        MMI_calendar_current_year = resultTime.nYear;
    }
    else
    {
        MMI_calendar_highlighted_day = 1;
        MMI_calendar_current_month = 12;
        MMI_calendar_current_year = CLNDR_LAST_NAVI_YEAR;
    }

    wgui_calendar_control_update_handler();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_next_month
 * DESCRIPTION
 *  Side down key handler, go to 1st day of next month.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_next_month(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_calendar_current_year != CLNDR_LAST_NAVI_YEAR || MMI_calendar_current_month != 12)
    {
        MYTIME myTime, incTime, resultTime;;
        memset(&myTime, 0, sizeof(MYTIME));
        memset(&incTime, 0, sizeof(MYTIME));

        myTime.nDay = 1;
        myTime.nMonth = (U8) MMI_calendar_current_month;
        myTime.nYear = MMI_calendar_current_year;

        incTime.nMonth = 1;
        resultTime = IncrementTime(myTime, incTime);
        MMI_calendar_highlighted_day = resultTime.nDay;
        MMI_calendar_current_month = resultTime.nMonth;
        MMI_calendar_current_year = resultTime.nYear;
    }
    else
    {
        MMI_calendar_highlighted_day = 1;
        MMI_calendar_current_month = 1;
        MMI_calendar_current_year = CLNDR_FIRST_NAVI_YEAR;
    }

    wgui_calendar_control_update_handler();
}

#if defined (__MMI_TOUCH_SCREEN__) && defined (__MMI_UI_CALENDAR_WITH_INFO_BOX__)


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_previous_year
 * DESCRIPTION
 *  Side up key handler, go to 1st day of previous year.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_previous_year(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_calendar_current_year != CLNDR_FIRST_NAVI_YEAR)
    {
        MYTIME myTime, incTime, resultTime;;
        memset(&myTime, 0, sizeof(MYTIME));
        memset(&incTime, 0, sizeof(MYTIME));

        myTime.nDay = 1;
        myTime.nMonth = (U8)MMI_calendar_current_month;
        myTime.nYear = MMI_calendar_current_year;

        incTime.nYear = 1;

        DecrementTime(myTime, incTime, &resultTime);
        MMI_calendar_highlighted_day = resultTime.nDay;
        MMI_calendar_current_month = resultTime.nMonth;
        MMI_calendar_current_year = resultTime.nYear;
    }
    else
    {
        MMI_calendar_highlighted_day = 1;
        MMI_calendar_current_year = CLNDR_LAST_NAVI_YEAR;
    }

    wgui_calendar_control_update_handler();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_next_year
 * DESCRIPTION
 *  Side down key handler, go to 1st day of next year.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_next_year(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_calendar_current_year != CLNDR_LAST_NAVI_YEAR)
    {
        MYTIME myTime, incTime, resultTime;;
        memset(&myTime, 0, sizeof(MYTIME));
        memset(&incTime, 0, sizeof(MYTIME));

        myTime.nDay = 1;
        myTime.nMonth = (U8)MMI_calendar_current_month;
        myTime.nYear = MMI_calendar_current_year;

        incTime.nYear = 1;
        resultTime = IncrementTime(myTime, incTime);
        MMI_calendar_highlighted_day = resultTime.nDay;
        MMI_calendar_current_month = resultTime.nMonth;
        MMI_calendar_current_year = resultTime.nYear;
    }
    else
    {
        MMI_calendar_highlighted_day = 1;
        MMI_calendar_current_year = CLNDR_FIRST_NAVI_YEAR;
    }

    wgui_calendar_control_update_handler();
}
#endif /* defined (__MMI_TOUCH_SCREEN__) && defined (__MMI_UI_CALENDAR_WITH_INFO_BOX__) */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_next_month_day
 * DESCRIPTION
 *  Side down key handler, go to 1st day of next month.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_next_month_day(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_calendar_current_year != CLNDR_LAST_NAVI_YEAR || MMI_calendar_current_month != 12)
    {
        MYTIME myTime, incTime, resultTime;

        memset(&myTime, 0, sizeof(MYTIME));
        memset(&incTime, 0, sizeof(MYTIME));

        myTime.nDay = 1;
        myTime.nMonth = (U8) MMI_calendar_current_month;
        myTime.nYear = MMI_calendar_current_year;

        incTime.nMonth = 1;
        resultTime = IncrementTime(myTime, incTime);
        MMI_calendar_highlighted_day = resultTime.nDay;
        MMI_calendar_current_month = resultTime.nMonth;
        MMI_calendar_current_year = resultTime.nYear;
    }
    else
    {
        MMI_calendar_highlighted_day = 1;
        MMI_calendar_current_month = 1;
        MMI_calendar_current_year = CLNDR_FIRST_NAVI_YEAR;
    }

    wgui_calendar_control_update_handler();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_previous_month_day
 * DESCRIPTION
 *  Side down key handler, go to 1st day of previous month.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_previous_month_day(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_calendar_current_year != CLNDR_FIRST_NAVI_YEAR || MMI_calendar_current_month != 1)
    {
        MYTIME myTime, incTime, resultTime;

        memset(&myTime, 0, sizeof(MYTIME));
        memset(&incTime, 0, sizeof(MYTIME));

        myTime.nDay = 1;
        myTime.nMonth = (U8) MMI_calendar_current_month;     
        myTime.nYear = MMI_calendar_current_year;

        incTime.nDay = 1;
        DecrementTime(myTime, incTime, &resultTime);
        MMI_calendar_highlighted_day = resultTime.nDay;
        MMI_calendar_current_month = resultTime.nMonth;
        MMI_calendar_current_year = resultTime.nYear;
    }
    else
    {
        MMI_calendar_highlighted_day = 31;
        MMI_calendar_current_month = 12;
        MMI_calendar_current_year = CLNDR_LAST_NAVI_YEAR;
    }

    wgui_calendar_control_update_handler();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_next_month_week
 * DESCRIPTION
 *  go to the same DoW of next week
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_next_month_week(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_calendar_current_year != CLNDR_LAST_NAVI_YEAR || MMI_calendar_current_month != 12)
    {
        MYTIME myTime, incTime, resultTime;

        memset(&myTime, 0, sizeof(MYTIME));
        memset(&incTime, 0, sizeof(MYTIME));

        myTime.nDay = MMI_calendar_highlighted_day;
        myTime.nMonth = (U8) MMI_calendar_current_month;
        myTime.nYear = MMI_calendar_current_year;

        incTime.nDay = 7;

        resultTime = IncrementTime(myTime, incTime);

        MMI_calendar_highlighted_day = resultTime.nDay;
        MMI_calendar_current_month = resultTime.nMonth;
        MMI_calendar_current_year = resultTime.nYear;
    }
    else
    {
        MMI_calendar_highlighted_day = 1;
        MMI_calendar_current_month = 1;
        MMI_calendar_current_year = CLNDR_FIRST_NAVI_YEAR;
    }

    wgui_calendar_control_update_handler();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_previous_month_week
 * DESCRIPTION
 *  go to the same DoW of previous week
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_calendar_previous_month_week(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_calendar_current_year != CLNDR_FIRST_NAVI_YEAR || MMI_calendar_current_month != 1)
    {
        MYTIME myTime, incTime, resultTime;

        memset(&myTime, 0, sizeof(MYTIME));
        memset(&incTime, 0, sizeof(MYTIME));

        myTime.nDay = MMI_calendar_highlighted_day;
        myTime.nMonth = (U8) MMI_calendar_current_month;
        myTime.nYear = MMI_calendar_current_year;

        incTime.nDay = 7;
        DecrementTime(myTime, incTime, &resultTime);
        MMI_calendar_highlighted_day = resultTime.nDay;
        MMI_calendar_current_month = resultTime.nMonth;
        MMI_calendar_current_year = resultTime.nYear;
    }
    else
    {
        MMI_calendar_highlighted_day = 31;
        MMI_calendar_current_month = 12;
        MMI_calendar_current_year = CLNDR_LAST_NAVI_YEAR;
    }

    wgui_calendar_control_update_handler();
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  SetCalendarMonthChangeFunctions
 * DESCRIPTION
 *  Register function pointers of previous/next month navigation of calendar
 * PARAMETERS
 *  previous_month      [IN]        )(void)      a function pointer which handle navigation to previous month
 *  next_month          [IN]        
 *  void(?)             [IN]        (*month_next_day)(void)     a function pointer which handle navigation to next month
 * RETURNS
 *  void
 *****************************************************************************/
void SetCalendarMonthChangeFunctions(void (*previous_month) (void), void (*next_month) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (previous_month != NULL)
    {
        MMI_calendar_control.previous_month_function = previous_month;
    }
    if (next_month != NULL)
    {
        MMI_calendar_control.next_month_function = next_month;
    }
}

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__


/*****************************************************************************
 * FUNCTION
 *  SetCalendarYearChangeFunctions
 * DESCRIPTION
 *  Register function pointers of previous/next year navigation of calendar
 * PARAMETERS
 *  previous_year       [IN]        )(void)       a function pointer which handle navigation to previous year
 *  next_year           [IN]        )(void)    a function pointer which handle navigation to next year
 * RETURNS
 *  void
 *****************************************************************************/
void SetCalendarYearChangeFunctions(void (*previous_year) (void), void (*next_year) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (previous_year != NULL)
    {
        MMI_calendar_control.previous_year_function = previous_year;
    }
    if (next_year != NULL)
    {
        MMI_calendar_control.next_year_function = next_year;
    }
}
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  RegisterCalendarHighlightHandler
 * DESCRIPTION
 *  Register calendar highlight function
 * PARAMETERS
 *  f       [IN]        )(S32 index) function pointer
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterCalendarHighlightHandler(void (*f) (S32 index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (f != NULL)
    {
        MMI_calendar_control_highlight_handler = f;
    }
}


/*****************************************************************************
 * FUNCTION
 *  RegisterCalendarUpdateHandler
 * DESCRIPTION
 *  Register calendar update function
 * PARAMETERS
 *  f       [IN]        )(S32 day,S32 month,S32 year)  function pointer
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterCalendarUpdateHandler(void (*f) (S32 day, S32 month, S32 year))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (f != NULL)
    {
        MMI_calendar_control_update_handler = f;
    }
}

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__


/*****************************************************************************
 * FUNCTION
 *  RegisterCalendarUpdateInfoBoxHandler
 * DESCRIPTION
 *  Register calendar update info box function
 * PARAMETERS
 *  f           [IN]        )(S32 *p_n_info, U16 ** p_icon_list, U8 ***p_description_list)  function pointer
 *  this(?)     [IN]        Function should update the value pointed by each pointer
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterCalendarUpdateInfoBoxHandler(void (*f) (S32 *p_n_info, U16 **p_icon_list, U8 ***p_description_list))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (f != NULL)
    {
        MMI_calendar_control.update_info_box = f;
    }
}
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 


/*****************************************************************************
 * FUNCTION
 *  SetCalendarDay
 * DESCRIPTION
 *  set the flag of particular day of month
 * PARAMETERS
 *  day         [IN]        Day
 *  flags       [IN]        Flag (normal/current/task day )
 * RETURNS
 *  void
 *****************************************************************************/
void SetCalendarDay(S32 day, U8 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_calendar_days[day] |= flags;
}


/*****************************************************************************
 * FUNCTION
 *  GetCalendarHighlightedDay
 * DESCRIPTION
 *  get highlighted day
 * PARAMETERS
 *  void
 * RETURNS
 *  S32 :- highlight day
 *****************************************************************************/
S32 GetCalendarHighlightedDay(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (MMI_calendar_highlighted_day);
}


/*****************************************************************************
 * FUNCTION
 *  SetCalendarHighlightedDay
 * DESCRIPTION
 *  set calendar highlighted day
 * PARAMETERS
 *  day     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCalendarHighlightedDay(S32 day)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (day >= 0 && day < MAX_CALENDAR_DAYS)
    {
        MMI_calendar_highlighted_day = day;
        gui_calendar_highlight_day(&MMI_calendar_control, MMI_calendar_highlighted_day);
    }
}


/*****************************************************************************
 * FUNCTION
 *  SetCalendarPopupDay
 * DESCRIPTION
 *  set calendar poup messages and flag of day to calendar popup day
 * PARAMETERS
 *  day             [IN]        Popup day
 *  message1        [IN]        Mesage 1
 *  message2        [IN]        Message 2
 *  message3        [IN]        Message 3
 * RETURNS
 *  void
 *****************************************************************************/
void SetCalendarPopupDay(S32 day, U8 *message1, U8 *message2, U8 *message3)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (day >= 0 && day < MAX_CALENDAR_DAYS)
    {
        MMI_calendar_days[day] |= CALENDAR_POPUP_DAY;
        MMI_calendar_messages1 = message1;
        MMI_calendar_messages2 = message2;
        MMI_calendar_messages3 = message3;
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_calendar_control_background_limits
 * DESCRIPTION
 *  set calendar bkg y positions
 * PARAMETERS
 *  y1      [IN]        Start y position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void set_calendar_control_background_limits(S32 y1, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    calendar_control_background_y1 = y1;
    calendar_control_background_y2 = y2;
}


/*****************************************************************************
 * FUNCTION
 *  calendar_control_header_display
 * DESCRIPTION
 *  set calendar header
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 * RETURNS
 *  void
 *****************************************************************************/
extern U16 gCurrLangIndex;
void calendar_control_header_display(S32 x1, S32 y1)
{
	#if 0 
	#ifdef __MMI_LANG_ENGLISH__
	U8 *DOW_En[]=
	{	

		(U8*)"S\0\0",
		(U8*)"M\0\0",
		(U8*)"T\0\0",
		(U8*)"W\0\0",
		(U8*)"T\0\0",
		(U8*)"F\0\0",
		(U8*)"S\0\0",
	};
	#endif
	#endif
	#if 0 //chenhe
	#ifdef __MMI_LANG_FRENCH__
	U8 *DOW_Fr[]=
	{
		(U8*)"D\0\0",
		(U8*)"L\0\0",
		(U8*)"M\0\0",
		(U8*)"M\0\0",
		(U8*)"J\0\0",
		(U8*)"V\0\0",
		(U8*)"S\0\0",
	};
	#endif
	#endif
	U8 *DOW_ML[]=
	{	
		(U8*)GetString(STR_CLNDR_WEEK_SUNDAY),
		(U8*)GetString(STR_CLNDR_WEEK_MONDAY),
		(U8*)GetString(STR_CLNDR_WEEK_TUESDAY),
		(U8*)GetString(STR_CLNDR_WEEK_WEDNESDAY),
		(U8*)GetString(STR_CLNDR_WEEK_THURSDAY),
		(U8*)GetString(STR_CLNDR_WEEK_FRIDAY),
		(U8*)GetString(STR_CLNDR_WEEK_SATURDAY)
	};
	U8 **gpDOW[]=
	{
		DOW_ML
//panx modified on 20080710
	};
	
	U8 **pDOW = gpDOW[0];
	color_t c1=coolsand_UI_color(255,255,255);
	color_t c2=coolsand_UI_color(0,0,0);
    S32 w, cw, i, x, tx;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* set font */
    gui_set_font(MMI_calendar_control.theme->text_font);
    /* set text color */
    gui_set_text_color(c1);
    /* set text border color */
    gui_set_text_border_color(c2);
    cw = MMI_calendar_control.cell_width;
    x = x1;
    for (i = 0; i < 7; i++)
    {
        w = gui_get_character_width((UI_character_type) pDOW[i][0]);
        tx = x + (cw >> 1) - (w >> 1);
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(tx + w, y1);
        }
        else
        {
            gui_move_text_cursor(tx, y1);
        }
        gui_print_bordered_text((UI_string_type) pDOW[i]);
        x += cw;
    }
}


/*****************************************************************************
 * FUNCTION
 *  calendar_control_background_display_function
 * DESCRIPTION
 *  set calendar bkg image
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void calendar_control_background_display_function(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x = x1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    y1 = calendar_control_background_y1;
    y2 = calendar_control_background_y2;
    x1 = 0;
    x2 = UI_device_width - 1;
#if(CALENDAR_FULL_SCREEN_BACKGROUND)
    if (MMI_calendar_control.flags & CALENDAR_HIDE_POPUP)
    {
        gui_set_clip(x1, y1, x2, y2);
    }
    else
    {
        gui_set_clip(x1, 0, x2, y2);
    }
    gdi_layer_set_text_clip(x, y1, x2, y2);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)||defined(__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    gui_reset_clip();
#endif 

#ifdef __MMI_MAINLCD_220X176__	
    gui_show_image(x1, calendar_control_background_begin_y, calendar_control_background_image);
#else
#if defined(__MMI_NOKIA_STYLE_N800__)
	gui_show_image(x1, 0,(PU8)IMG_BACKGROUND_2Theme);
#else
    gui_show_image(x1, 0, calendar_control_background_image);
#endif
#endif

#else /* (CALENDAR_FULL_SCREEN_BACKGROUND) */ 
    gui_set_clip(x1, y1, x2, y2);
    gdi_layer_set_text_clip(x, y1, x2, y2);
    gui_show_image(x1, y1, calendar_control_background_image);
#endif /* (CALENDAR_FULL_SCREEN_BACKGROUND) */ 
    if (calendar_control_title_image != NULL)
#if ( defined  (__MMI_MAINLCD_128X128__) || defined (__MMI_MAINLCD_128X160__) )
         
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
        gui_show_transparent_image(
            DALMATIAN_CALENDAR_WEEK_IMAGE_START,
            MMI_calendar_control.y - CALENDAR_DOW_TITLE_HEIGHT + 2,
            calendar_control_title_image,
            0);
#else /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
        gui_show_transparent_image(x1, y1 + 1, calendar_control_title_image, 0);
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
     
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#elif defined( __MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)||defined(__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
         
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
        gui_show_transparent_image(DALMATIAN_CALENDAR_WEEK_IMAGE_START, y1 + 1, calendar_control_title_image, 0);
#else
#if defined(__MMI_NOKIA_STYLE_N800__)
		if(NULL != calendar_control_title_image)
		{
			S32 w,h;
			gui_measure_image(calendar_control_title_image,&w,&h);
			gui_show_transparent_image((UI_device_width - w) >> 1, y1 + 1, calendar_control_title_image, 0);
		}
#else
        /*+ zhouqin 2011-02-16 modify for q6 320X240 */
         #ifdef __MMI_MAINLCD_320X240__
		gui_show_transparent_image(40, y1 + 5, calendar_control_title_image, 0);
	 #else
        	gui_show_transparent_image(MMI_calendar_control.x1, y1 + 1, calendar_control_title_image, 0);
	 #endif
        /*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif
#endif 
     
#else 
        gui_show_transparent_image(x1, y1 + 1, calendar_control_title_image, 0);
#endif 
    else
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
         
#ifdef __MMI_MAINLCD_176X220__
        calendar_control_header_display(x, cat82_title1_y2 + 5);
#elif defined(__MMI_MAINLCD_220X176__)
        calendar_control_header_display(x, cat82_title1_y2 + 5);
#elif defined(__MMI_MAINLCD_128X160__)
        calendar_control_header_display(x, y1 + 6);
#elif defined(__MMI_MAINLCD_160X128__)
        calendar_control_header_display(x, y1 + 6);
#elif defined(__MMI_MAINLCD_128X128__)
        calendar_control_header_display(x, y1 + 6);
#endif 
     
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#elif defined(__MMI_MAINLCD_320X240__)
        calendar_control_header_display(x+3, y1 + 5);
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#else /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
        calendar_control_header_display(x, y1 + 5);
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
 }


/*****************************************************************************
 * FUNCTION
 *  SetCalendarBackground
 * DESCRIPTION
 *  set image of calendar bkg
 * PARAMETERS
 *  image_ID        [IN]        
 *  imageid(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCalendarBackground(U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_calendar_control.theme = &_calendar_control_theme_data;
    MMI_calendar_control.draw_background_function = calendar_control_background_display_function;
#ifdef __MMI_THEMES_V2_SUPPORT__
    calendar_control_background_image = get_image(image_ID);
#endif 
    calendar_control_background_image = get_image(image_ID);
}


/*****************************************************************************
 * FUNCTION
 *  SetCalendarTitleImage
 * DESCRIPTION
 *  set the image of title of calendar
 * PARAMETERS
 *  image_ID        [IN]        
 *  imageid(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCalendarTitleImage(U16 image_ID)
{
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    calendar_control_title_image = get_image(dalmatian_calendar_current_week_image());
#else /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    calendar_control_title_image = get_image(image_ID);
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  Calendar_hide_left_softkey
 * DESCRIPTION
 *  Hides the left softkey
 *  
 *  Calendar hide left softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Calendar_hide_left_softkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gui_set_clip(
        MMI_softkeys[MMI_LEFT_SOFTKEY].x,
        UI_device_height - MMI_button_bar_height,
        MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width + 2,
        UI_device_height - 1);
#if(CALENDAR_FULL_SCREEN_BACKGROUND)
#ifdef __MMI_MAINLCD_220X176__	
    gui_show_image(calendar_control_background_begin_x, calendar_control_background_begin_y, calendar_control_background_image);
#else
    gui_show_image(0, 0, calendar_control_background_image);
#endif
#else 
    gui_show_image(0, calendar_control_background_y1, calendar_control_background_image);
#endif 
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  Calendar_hide_right_softkey
 * DESCRIPTION
 *  Hides the right softkey
 *  
 *  Calendar hide right softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Calendar_hide_right_softkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gui_set_clip(
        MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
        UI_device_height - MMI_button_bar_height,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width + 2,
        UI_device_height - 1);
#if(CALENDAR_FULL_SCREEN_BACKGROUND)
#ifdef __MMI_MAINLCD_220X176__	
    gui_show_image(calendar_control_background_begin_x, calendar_control_background_begin_y, calendar_control_background_image);
#else
    gui_show_image(0, 0, calendar_control_background_image);
#endif
#else 
    gui_show_image(0, calendar_control_background_y1, calendar_control_background_image);
#endif 
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory80Screen
 * DESCRIPTION
 *  Redraws the Calendar App category screen solar calendar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory80Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
#if !defined(__MMI_USING_BLACK_THEME_BG__)	
    clear_screen();
#endif
    gdi_layer_push_clip();
    gdi_layer_push_text_clip();
    gui_reset_clip();
    gui_set_font(&MMI_default_font);
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    set_dalmatian_calendar_highlighter_box();
    SetCalendarTitleImage(dalmatian_calendar_current_week_image());
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    wgui_show_calendar();
    ChangeCategory82Title1(Cat82_title1);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    ChangeCategory82Title1Month(Cat82_title1_month);
#endif 

#if ( defined (__MMI_MAINLCD_128X128__) || defined (__MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_160X128__))
    show_softkey_background();
#elif defined( __MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined( __MMI_MAINLCD_220X176__)
    disable_softkey_background();
#else 
    show_softkey_background();
#endif 

    show_left_softkey();
    show_right_softkey();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
    gdi_layer_pop_clip();
    gdi_layer_pop_text_clip();
}


/*****************************************************************************
 * FUNCTION
 *  InitializeCategory80Screen
 * DESCRIPTION
 *  Initializes the Calendar App category screen
 *  
 *  Must be called once before calling ShowCategory80Screen
 * PARAMETERS
 *  first_day_of_week       [IN]        See the constants in wgui_calendar.h
 *  last_day_of_month       [IN]        Ex: 28, 29, 30 or 31
 *  current_day             [IN]        
 *  current_month           [IN]        
 *  current_year            [IN]        
 *  INT(?)                  [IN]        Current day          Current day number (ex: 1 to 31)
 * RETURNS
 *  void
 *****************************************************************************/
void InitializeCategory80Screen(
        S32 first_day_of_week,
        S32 last_day_of_month,
        S32 current_day,
        S32 current_month,
        S32 current_year)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    lunar_calendar_present = FALSE;
    wgui_setup_calendar(
        0,
        0,
        UI_device_width / CALENDAR_N_COLUMNS,
        UI_device_height / CALENDAR_N_ROWS,
        first_day_of_week,
        last_day_of_month,
        current_day,
        current_month,
        current_year,
        0);
     
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    set_current_month_year_for_dalmatian(current_day, current_month, current_year);
#endif 
     
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory80Screen
 * DESCRIPTION
 *  Displays the Calendar App category screen
 *  
 *  Must call InitializeCategory80Screen before calling this screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  history_buffer          [IN]        History buffer
 *  STRING(?)               [IN]        Title             Title string (ex: Month and Year)
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory80Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCalendarCommomCategoryScreen(
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        history_buffer);

#ifdef __MMI_MAINLCD_240X320__
    register_hide_left_softkey(Calendar_hide_left_softkey);
    register_hide_right_softkey(Calendar_hide_right_softkey);
//#else /* __MMI_MAINLCD_240X320__ */ //083006 calendar softkey
#elif defined(__MMI_MAINLCD_176X220__)&&defined(__MMI_NOKIA_STYLE_N800__)
    disable_softkey_background();
#elif defined(__MMI_MAINLCD_176X220__) && defined(__MMI_PROJECT_T33__) 
    register_hide_left_softkey(Calendar_hide_left_softkey);
    register_hide_right_softkey(Calendar_hide_right_softkey);
#else
    disable_softkey_background();
#endif /* __MMI_MAINLCD_240X320__ */ 

    SetCategory82Title1((U8*) MMI_title_string);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    SetCategory82Title1Month((U8*) MMI_calendar_title_string_month);
#endif 

    ExitCategoryFunction = ExitCategory80Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCategoryFunction = dm_redraw_category_screen;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY80_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory80Screen
 * DESCRIPTION
 *  Exits the Calendar App category screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory80Screen(void)
{   /* close calenar ..stop popup timer */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_close_calendar();
    leave_full_screen();
    enable_softkey_background();
    reset_softkeys();   /* reset softkeys */

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    gui_register_button_shuffle(shuffle_draw_white);    /* register the background drower */
#endif 
#if defined(__MMI_UI_CALENDAR_WITH_INFO_BOX__) && defined (__MMI_TOUCH_SCREEN__)
    ResetCalendarTitle1Arrow();
    ResetCalendarTitle1MonthArrow();
#endif /* defined(__MMI_UI_CALENDAR_WITH_INFO_BOX__) && defined (__MMI_TOUCH_SCREEN__) */ 

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  InitializeCategory81Screen
 * DESCRIPTION
 *  Initializes the Calendar App category screen
 *  
 *  Must be called once before calling ShowCategory81Screen
 * PARAMETERS
 *  first_day_of_week       [IN]        See the constants in wgui_calendar.h
 *  last_day_of_month       [IN]        Ex: 28, 29, 30 or 31
 *  current_day             [IN]        
 *  current_month           [IN]        Current month (ex: 1 to 12)
 *  current_year            [IN]        Current  year (ex: 1970 to 2030)
 *  INT(?)                  [IN]        Current day          Current day number (ex: 1 to 31)
 * RETURNS
 *  void
 *****************************************************************************/
void InitializeCategory81Screen(
        S32 first_day_of_week,
        S32 last_day_of_month,
        S32 current_day,
        S32 current_month,
        S32 current_year)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_setup_calendar(
        0,
        0,
        UI_device_width / CALENDAR_N_COLUMNS,
        UI_device_height / CALENDAR_N_ROWS,
        first_day_of_week,
        last_day_of_month,
        current_day,
        current_month,
        current_year,
        0);
     
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    set_current_month_year_for_dalmatian(current_day, current_month, current_year);
#endif 
     
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory81Screen
 * DESCRIPTION
 *  Displays the Health Calendar App category screen
 * PARAMETERS
 *  title                   [IN]        Title string (ex: Month and Year)
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message1                [IN]        Sub title message 1
 *  message2                [IN]        Sub title message 2
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory81Screen(
        U8 *title,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message1,
        U8 *message2,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    color_t c = *(current_MMI_theme->UI_black_color);
    color_t wc = *(current_MMI_theme->UI_white_color);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_reset_context();

    ShowCalendarCommomCategoryScreen(
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        history_buffer);

#ifdef __MMI_MAINLCD_240X320__
    register_hide_left_softkey(Calendar_hide_left_softkey);
    register_hide_right_softkey(Calendar_hide_right_softkey);
//#else /* __MMI_MAINLCD_240X320__ */ //083006 calendar softkey
#elif defined(__MMI_MAINLCD_176X220__)
    disable_softkey_background();
#elif defined(__MMI_MAINLCD_220X176__)
    disable_softkey_background();
#endif /* __MMI_MAINLCD_240X320__ */ 

    SetCategory82Title1((U8*) MMI_title_string);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
#ifdef __MMI_TOUCH_SCREEN__
    DisableCalendarTitle1Arrow();
#endif 
    SetCategory82Title1Month((U8*) MMI_calendar_title_string_month);
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

    ExitCategoryFunction = ExitCategory81Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCategoryFunction = dm_redraw_category_screen;
    dm_add_back_fill_area(MMI_calendar_control.theme->special2_day_filler);
    dm_add_rectangle(c, 0);
    dm_add_back_fill_area(MMI_calendar_control.theme->special1_day_filler);
    dm_add_rectangle(c, 0);
    dm_add_string((UI_string_type) message1, &MMI_default_font, wc, c, NULL);
    dm_add_string((UI_string_type) message2, &MMI_default_font, wc, c, NULL);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY81_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory81Screen
 * DESCRIPTION
 *  Exits the Health Calendar App category screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory81Screen(void)
{   /* close health calendar */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_close_calendar();
    leave_full_screen();
    reset_softkeys();
    enable_softkey_background();
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    set_dalmatian_calendar_highlighter_enable();
#endif 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    gui_register_button_shuffle(shuffle_draw_white);    /* register the background drower */
#endif 

#if defined(__MMI_UI_CALENDAR_WITH_INFO_BOX__) && defined (__MMI_TOUCH_SCREEN__)
    ResetCalendarTitle1Arrow();
    ResetCalendarTitle1MonthArrow();
#endif /* defined(__MMI_UI_CALENDAR_WITH_INFO_BOX__) && defined (__MMI_TOUCH_SCREEN__) */ 

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory82Title1
 * DESCRIPTION
 *  set title 1 of lunar calendar
 * PARAMETERS
 *  title       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory82Title1(U8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat82_title1 = title;
}

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__


/*****************************************************************************
 * FUNCTION
 *  SetCategory82Title1Month
 * DESCRIPTION
 *  set title 1 month of lunar calendar
 * PARAMETERS
 *  title       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory82Title1Month(U8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat82_title1_month = title;
}
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 


/*****************************************************************************
 * FUNCTION
 *  SetCategory82Title2
 * DESCRIPTION
 *  set title 2 of lunar calendar
 * PARAMETERS
 *  title       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory82Title2(U8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat82_title2 = title;
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory82Title3
 * DESCRIPTION
 *  set title 3 of lunar calendar
 * PARAMETERS
 *  title       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory82Title3(U8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat82_title3 = title;
}


/*****************************************************************************
 * FUNCTION
 *  ChangeCategory82Title1
 * DESCRIPTION
 *  change title 1 of lunar calendar and display
 * PARAMETERS
 *  title       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeCategory82Title1(U8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if defined(__PROJECT_GALLITE_C01__)
    S32 x1 = 0;//cat82_title1_x1;
    S32 y1 = cat82_title1_y1;
    S32 x2 = UI_device_width -1;//cat82_title1_x2;
#else
    S32 x1 = cat82_title1_x1;
    S32 y1 = cat82_title1_y1;
    S32 x2 = cat82_title1_x2;
#endif
    S32 y2 = cat82_title1_y2;
    S32 w, h;
    color_t tc = *(current_MMI_theme->UI_black_color);

#if(CALENDAR_BORDERED_TITLES)
    color_t wc = *(current_MMI_theme->UI_white_color);
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat82_title1 = title;
    gdi_layer_push_clip();
    gdi_layer_push_text_clip();
    gui_reset_clip();
    gdi_layer_set_text_clip(x1, y1, x2, y2);
#if(CALENDAR_FULL_SCREEN_BACKGROUND)
    gui_set_clip(x1, y1, x2, y2);
#if !defined(__MMI_MAINLCD_176X220__) && !defined(__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)
    gui_show_image(x1, 0, calendar_control_background_image);
#else 
#ifdef __MMI_MAINLCD_220X176__	
    gui_show_image(calendar_control_background_begin_x, calendar_control_background_begin_y, calendar_control_background_image);
#else
#if !defined(__MMI_NOKIA_STYLE_N800__)
    gui_show_image(0, 0, calendar_control_background_image);
#else
	gdi_image_draw(0,0,(U8*)IMG_BACKGROUND_2Theme);//for i have not proper image resource for area(x1, y1, x2, y2),may be drawing IMG_BACKGROUND_2Theme is not a goog enough idea.
#endif
#endif 

#endif 
#else /* (CALENDAR_FULL_SCREEN_BACKGROUND) */ 
    gui_fill_rectangle(x1, y1, x2, y2, tc);
#endif /* (CALENDAR_FULL_SCREEN_BACKGROUND) */ 

#ifdef __MMI_TOUCH_SCREEN__
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    if (!(lunar_calendar_present && IsChineseSet()))
    {
        if (!Cat82_title1_arrow_disable)    /* Year arrows on title 1 */
        {
            gui_measure_image((PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
            gui_show_transparent_image(
                x1 + MMI_CALENDAR_ARROW_OFFSET,
                y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
                (PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE),
                0);
            gui_measure_image((PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
            gui_show_transparent_image(
                x2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
                y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
                (PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE),
                0);
        }
    }
#else /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
    if (!(lunar_calendar_present && IsChineseSet()))
    {
        gui_measure_image((PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
        gui_show_transparent_image(
            x1 + MMI_CALENDAR_ARROW_OFFSET,
            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
            (PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE),
            0);
        gui_measure_image((PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
        gui_show_transparent_image(
            x2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
            (PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE),
            0);
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
#endif /* __MMI_TOUCH_SCREEN__ */ 
    gui_set_font(calendar_title_font);
    /* measure width and height ot title 1 in pixels */
    gui_measure_string((UI_string_type) Cat82_title1, &w, &h);
    /* move text cursor */

	gdi_layer_reset_clip();
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1) + w, y1 + ((y2 - y1 + 1) >> 1) - (h >> 1));
    }
    else
    {
        gui_move_text_cursor(x1 + 1, y1 + 1);
    }
    set_dalmatian_title_string();
#else /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1) + w, y1 + ((y2 - y1 + 1) >> 1) - (h >> 1));
    }
    else
    {
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1), y1 + ((y2 - y1 + 1) >> 1) - (h >> 1));
    }

#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
#if(CALENDAR_BORDERED_TITLES)
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    gui_set_font(&MMI_medium_font);
    gui_set_text_border_color(wc);
    gui_set_text_color(tc);
#else /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    gui_set_text_border_color(tc);
    gui_set_text_color(wc);
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    gui_print_bordered_text((UI_string_type) Cat82_title1);
#else /* (CALENDAR_BORDERED_TITLES) */ 
    gui_set_text_color(tc);
    /* pritnt title 1 */
    gui_print_text((UI_string_type) Cat82_title1);
#endif /* (CALENDAR_BORDERED_TITLES) */ 
    gdi_layer_blt_previous(x1, y1, x2, y2);
    gdi_layer_pop_clip();
    gdi_layer_pop_text_clip();
}

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__


/*****************************************************************************
 * FUNCTION
 *  ChangeCategory82Title1Month
 * DESCRIPTION
 *  change title 1 month of lunar calendar and display
 * PARAMETERS
 *  title       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeCategory82Title1Month(U8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1 = cat82_title1_month_x1;
    S32 y1 = cat82_title1_month_y1;
    S32 x2 = cat82_title1_month_x2;
    S32 y2 = cat82_title1_month_y2;
    S32 w, h;
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined  __MMI_MAINLCD_320X240__
    color_t tc = *(current_MMI_theme->UI_white_color);
    color_t tmp_c = *(current_MMI_theme->UI_black_color);
#else
    color_t tc = *(current_MMI_theme->UI_black_color);
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#if(CALENDAR_BORDERED_TITLES)
    color_t wc = *(current_MMI_theme->UI_white_color);
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (calendar_show_title_month == 0) /* Don't wanna show month info on title 1 : Month */
    {
        return;
    }

    Cat82_title1_month = title;
    gdi_layer_push_clip();
    gdi_layer_push_text_clip();

    if ((lunar_calendar_present && IsChineseSet()) && calendar_show_title3)
    {
        x2 = cat82_title3_x1 - 1;
    }

    gui_reset_clip();
    gdi_layer_set_text_clip(x1, y1, x2, y2);

    if (lunar_calendar_present && IsChineseSet())   /* Shown by lunar calnedar */
    {
    #if(CALENDAR_FULL_SCREEN_BACKGROUND)
        gui_set_clip(x1, y1, x2, y2);
	#ifdef __MMI_MAINLCD_220X176__
        gui_show_image(calendar_control_background_begin_x, calendar_control_background_begin_y, calendar_control_background_image);
	#else
	gui_show_image(0, 0, calendar_control_background_image);
	#endif
     #else
	 color_t bc = {182, 200, 255, 100};
        gui_fill_rectangle(x1, y1, x2, y2, bc);
    #endif /* (CALENDAR_FULL_SCREEN_BACKGROUND) */ 
        gui_set_font(&MMI_default_font);
    }
    else    /* Shown by normal calnedar */
    {
    #if(CALENDAR_FULL_SCREEN_BACKGROUND)
        gui_set_clip(x1, y1, x2, y2);
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */
    #if !defined(__MMI_MAINLCD_176X220__) && !defined(__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)&&!defined(__MMI_MAINLCD_320X240__)
    /*- zhouqin 2011-02-16 modify for q6 320X240 */
	gui_show_image(cat82_title1_x1, 0, calendar_control_background_image);
    #else 
	#ifdef __MMI_MAINLCD_220X176__	
	    gui_show_image(calendar_control_background_begin_x, calendar_control_background_begin_y, calendar_control_background_image);
	#else
        gui_show_image(0, 0, calendar_control_background_image);
    #endif 
    #endif 
    #else /* (CALENDAR_FULL_SCREEN_BACKGROUND) */ 
        gui_fill_rectangle(x1, y1, x2, y2, tc);
    #endif /* (CALENDAR_FULL_SCREEN_BACKGROUND) */ 
        gui_set_font(calendar_title_month_font);
    }

#ifdef __MMI_TOUCH_SCREEN__
    if (!Cat82_title1_month_arrow_disable)
    {
        gui_measure_image((PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
        gui_show_transparent_image(
            x1 + MMI_CALENDAR_ARROW_OFFSET,
            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
            (PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE),
            0);
        gui_measure_image((PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
        gui_show_transparent_image(
            x2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
            (PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE),
            0);
    }
#endif /* __MMI_TOUCH_SCREEN__ */ 
    /* measure width and height ot title 1 in pixels */
    gui_measure_string((UI_string_type) Cat82_title1_month, &w, &h);
    /* move text cursor */

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1) + w, y1 + ((y2 - y1 + 1) >> 1) - (h >> 1));
    }
    else
    {
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1), y1 + ((y2 - y1 + 1) >> 1) - (h >> 1));
    }

#if(CALENDAR_BORDERED_TITLES)
    if (lunar_calendar_present && IsChineseSet())
    {
        gui_set_text_color(tc);
        gui_print_text((UI_string_type) Cat82_title1_month);
    }
    else
    {
         /*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined  __MMI_MAINLCD_320X240__
        gui_set_text_border_color(tmp_c);
        gui_set_text_color(wc);
	#else
	gui_set_text_border_color(tc);
        gui_set_text_color(wc);
	#endif
        /*- zhouqin 2011-02-16 modify for q6 320X240 */
        gui_print_bordered_text((UI_string_type) Cat82_title1_month);
    }
#else /* (CALENDAR_BORDERED_TITLES) */ 
    gui_set_text_color(tc);
    /* pritnt title 1 */
    gui_print_text((UI_string_type) Cat82_title1_month);
#endif /* (CALENDAR_BORDERED_TITLES) */ 
    gdi_layer_blt_previous(x1, y1, x2, y2);
    gdi_layer_pop_clip();
    gdi_layer_pop_text_clip();
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  DisableCalendarTitle1Arrow
 * DESCRIPTION
 *  disable arrows functionning in title1
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisableCalendarTitle1Arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat82_title1_arrow_disable = MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  ResetCalendarTitle1Arrow
 * DESCRIPTION
 *  reset arrows functionning in title1
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ResetCalendarTitle1Arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat82_title1_arrow_disable = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  DisableCalendarTitle1MonthArrow
 * DESCRIPTION
 *  disable arrows functionning in title1 month
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisableCalendarTitle1MonthArrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat82_title1_month_arrow_disable = MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  ResetCalendarTitle1MonthArrow
 * DESCRIPTION
 *  reset arrows functionning in title1 month
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ResetCalendarTitle1MonthArrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat82_title1_month_arrow_disable = MMI_FALSE;
}
#endif /* __MMI_TOUCH_SCREEN__ */ 
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 


/*****************************************************************************
 * FUNCTION
 *  ChangeCategory82Title2
 * DESCRIPTION
 *  change title 2 of lunar calendar and display
 * PARAMETERS
 *  title       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeCategory82Title2(U8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1 = cat82_title2_x1;
    S32 y1 = cat82_title2_y1;
    S32 x2 = cat82_title2_x2;

#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    S32 y2 = cat82_title2_y1 + 18;
#else 
    S32 y2 = cat82_title2_y2;
#endif 
    S32 w, h;
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined  (__MMI_MAINLCD_320X240__)
   color_t tc = *(current_MMI_theme->UI_white_color);
#else
    color_t tc = *(current_MMI_theme->UI_black_color);
#endif

#if !defined (__MMI_MAINLCD_220X176__) &&  !defined (__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    color_t bc = {182, 200, 255, 100};
#endif 

#ifdef __MMI_MAINLCD_160X128__
	return;
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Update Title2 and Title3 For Lunar calendar only */
    if (!lunar_calendar_present)
    {
        return;
    }

    if (calendar_show_title2 == 0)
    {
        return;
    }
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    set_dalmatian_title_string();
#else /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    Cat82_title2 = title;

#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    gdi_layer_push_clip();
    gdi_layer_push_text_clip();

#ifndef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    if (calendar_show_title3)
    {
        x2 = cat82_title3_x1 - 1;
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

    gui_reset_clip();
    gdi_layer_set_text_clip(x1, y1, x2, y2);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if !defined (__MMI_MAINLCD_220X176__) &&  !defined (__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    gui_fill_rectangle(x1, y1, x2, y2, bc);
#endif 

#ifdef __MMI_TOUCH_SCREEN__
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    if ((lunar_calendar_present && IsChineseSet())) /* Year arrow on title 2 */
    {
        if (!Cat82_title1_arrow_disable)
        {
            gui_measure_image((PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
            gui_show_transparent_image(
                x1 + MMI_CALENDAR_ARROW_OFFSET,
                y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
                (PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE),
                0);
            gui_measure_image((PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
            gui_show_transparent_image(
                x2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
                y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
                (PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE),
                0);
        }
    }
#else /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
    if ((lunar_calendar_present && IsChineseSet())) /* Year/Month  arrow on title 2 */
    {
        if (calendar_show_title3)
        {
            x2 = cat82_title3_x1 - 1;   /* Align Title2 contents in middle if Title3 is present as we want to show left and right arrow keys on title2 */
        }
        gui_measure_image((PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
        gui_show_transparent_image(
            x1 + MMI_CALENDAR_ARROW_OFFSET,
            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
            (PU8) GetImage(CAL_LEFT_ARROW_UP_IMAGE),
            0);
        gui_measure_image((PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE), &calendar_arrow_width, &calendar_arrow_height);
        gui_show_transparent_image(
            x2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
            (PU8) GetImage(CAL_RIGHT_ARROW_UP_IMAGE),
            0);
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
#endif /* __MMI_TOUCH_SCREEN__ */ 
    /* set text font */
    gui_set_font(&MMI_default_font);
    /* set text color */
    gui_set_text_color(tc);
    /* measue width and hieht of title 2 of lunar calendar */
    gui_measure_string((UI_string_type) Cat82_title2, &w, &h);

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1) + w, y1 + ((y2 - y1 + 1) >> 1) - (h >> 1));
    }
    else
    {
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1), y1 + ((y2 - y1 + 1) >> 1) - (h >> 1));
    }

    gui_print_text((UI_string_type) Cat82_title2);
    gdi_layer_blt_previous(x1, y1, x2, y2);
    gdi_layer_pop_clip();
    gdi_layer_pop_text_clip();
}


/*****************************************************************************
 * FUNCTION
 *  ChangeCategory82Title3
 * DESCRIPTION
 *  change title 3 of lunar calendar and display
 * PARAMETERS
 *  title       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeCategory82Title3(U8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1 = cat82_title3_x1;
    S32 y1 = cat82_title3_y1;
    S32 x2 = cat82_title3_x2;

#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    S32 y2 = cat82_title3_y1 + 18;
#else 
    S32 y2 = cat82_title3_y2;
#endif 
    S32 w, h;
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#if defined (__MMI_MAINLCD_220X176__)||defined (__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */	
    color_t temp_c = {0,0,0,100};
    color_t bc = temp_c;
#else
    color_t bc = *(current_MMI_theme->title_text_border_color);
#endif
    color_t tc = *(current_MMI_theme->title_text_color);
    color_t bb = *(current_MMI_theme->title_text_border_color);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Update Title2 and Title3 For Lunar calendar only */
    if (!lunar_calendar_present)
    {
        return;
    }

    if (calendar_show_title3 == 0)
    {
        return;
    }
    Cat82_title3 = title;
    gdi_layer_push_clip();
    gdi_layer_push_text_clip();
    gui_reset_clip();
    gdi_layer_set_text_clip(x1, y1, x2, y2);
    gui_fill_rectangle(x1, y1, x2, y2, bc);
    gui_set_font(&MMI_default_font);
    gui_set_text_color(tc);
    gui_measure_string((UI_string_type) Cat82_title3, &w, &h);

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1) + w, y1 + ((y2 - y1 + 1) >> 1) - (h >> 1));
    }
    else
    {
#ifdef __MMI_MAINLCD_160X128__
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1), y1 + ((y2 - y1 + 1) >> 1) - (h >> 1)+1);
#else
        gui_move_text_cursor(x1 + ((x2 - x1 + 1) >> 1) - (w >> 1), y1 + ((y2 - y1 + 1) >> 1) - (h >> 1));
#endif
    }

    gui_print_text((UI_string_type) Cat82_title3);
    gui_draw_rectangle(x1, y1, x2, y2, bb);
    gdi_layer_blt_previous(x1, y1, x2, y2);
    gdi_layer_pop_clip();
    gdi_layer_pop_text_clip();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory82Screen
 * DESCRIPTION
 *  Redraws the Calendar App category screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory82Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
#if !defined(__MMI_USING_BLACK_THEME_BG__)
    clear_screen();
#endif
    gui_reset_clip();
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    set_dalmatian_calendar_highlighter_box();
    SetCalendarTitleImage(dalmatian_calendar_current_week_image());
#endif /* __MMI_UI_DALMATIAN_CALENDAR__ */ 
    wgui_show_calendar();
    ChangeCategory82Title1(Cat82_title1);
    ChangeCategory82Title2(Cat82_title2);
    ChangeCategory82Title3(Cat82_title3);
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    ChangeCategory82Title1Month(Cat82_title1_month);
#endif 

#if ( defined (__MMI_MAINLCD_128X128__) || defined (__MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_160X128__))
    show_softkey_background();
#elif defined( __MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)
    disable_softkey_background();
#else 
    show_softkey_background();
#endif 
    show_left_softkey();
    show_right_softkey();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  InitializeCategory82Screen
 * DESCRIPTION
 *  Initializes the Calendar App category screen
 *  
 *  Must be called once before calling ShowCategory82Screen
 * PARAMETERS
 *  first_day_of_week       [IN]        See the constants in wgui_calendar.h
 *  last_day_of_month       [IN]        Ex: 28, 29, 30 or 31
 *  current_day             [IN]        
 *  current_month           [IN]        
 *  current_year            [IN]        
 *  INT(?)                  [IN]        Current day          Current day number (ex: 1 to 31)
 * RETURNS
 *  void
 *****************************************************************************/
void InitializeCategory82Screen(
        S32 first_day_of_week,
        S32 last_day_of_month,
        S32 current_day,
        S32 current_month,
        S32 current_year)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_setup_calendar(
        0,
        0,
        UI_device_width / CALENDAR_N_COLUMNS,
        UI_device_height / CALENDAR_N_ROWS,
        first_day_of_week,
        last_day_of_month,
        current_day,
        current_month,
        current_year,
        CALENDAR_ENABLE_POPUP);
    lunar_calendar_present = MMI_TRUE;

     
#ifdef __MMI_UI_DALMATIAN_CALENDAR__
    set_current_month_year_for_dalmatian(current_day, current_month, current_year);
#endif 
     
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory82Screen
 * DESCRIPTION
 *  Displays the Calendar App category screen
 *  
 *  Must call InitializeCategory82Screen before calling this screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  history_buffer          [IN]        History buffer
 *  STRING(?)               [IN]        Title             Title string (ex: Month and Year)
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory82Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCalendarCommomCategoryScreen(
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        history_buffer);

#ifdef __MMI_MAINLCD_240X320__
    register_hide_left_softkey(Calendar_hide_left_softkey);
    register_hide_right_softkey(Calendar_hide_right_softkey);
//#else /* __MMI_MAINLCD_240X320__ */ //083006 calendar softkey
#elif defined(__MMI_MAINLCD_176X220__)
    disable_softkey_background();
#elif defined(__MMI_MAINLCD_220X176__)
    disable_softkey_background();
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#elif defined(__MMI_MAINLCD_320X240__)
  disable_softkey_background();
/*- zhouqin 2011-02-16 modify for q6 320X240 */	
#endif /* __MMI_MAINLCD_240X320__ */ 

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    SetCategory82Title1Month((U8*) MMI_calendar_title_string_month);
#endif 

    RedrawCategoryFunction = dm_redraw_category_screen;
    ExitCategoryFunction = ExitCategory82Screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY82_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory82Screen
 * DESCRIPTION
 *  Exits the Calendar App category screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory82Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_close_calendar();
    leave_full_screen();
    enable_softkey_background();
    reset_softkeys();
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
    gui_register_button_shuffle(shuffle_draw_white);    /* register the background drower */
#endif 
    lunar_calendar_present = MMI_FALSE;

#if defined(__MMI_UI_CALENDAR_WITH_INFO_BOX__) && defined (__MMI_TOUCH_SCREEN__)
    ResetCalendarTitle1Arrow();
    ResetCalendarTitle1MonthArrow();
#endif /* defined(__MMI_UI_CALENDAR_WITH_INFO_BOX__) && defined (__MMI_TOUCH_SCREEN__) */ 

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}

#ifdef __MMI_TOUCH_SCREEN__
static BOOL g_calendar_pen_event = MMI_FALSE;


/*****************************************************************************
 * FUNCTION
 *  wgui_calendar_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  c                   [?]         
 *  pen_event           [IN]        
 *  x                   [IN]        
 *  y                   [IN]        
 *  calendar_event      [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL wgui_calendar_translate_pen_event(
        calendar_control *c,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_calendar_pen_enum *calendar_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* If event is on Title of Calendar, handle that otherwise pass control to gui layer of calendar for handling pen events  */

    BOOL ret;
    S32 x1, y1, x2, y2;

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    S32 xx1, yy1, xx2, yy2;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    /* for month info */
    x1 = cat82_title1_month_x1;
    y1 = cat82_title1_month_y1;
    x2 = cat82_title1_month_x2;
    y2 = cat82_title1_month_y2;
    /* for year info */
    if (lunar_calendar_present && IsChineseSet())   /* year info on title 1 */
    {
        xx1 = cat82_title2_x1;
        yy1 = cat82_title2_y1;
        xx2 = cat82_title2_x2;
        yy2 = cat82_title2_y2;
    }
    else    /* year info on title 2 */
    {
        xx1 = cat82_title1_x1;
        yy1 = cat82_title1_y1;
        xx2 = cat82_title1_x2;
        yy2 = cat82_title1_y2;
    }
#else /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
    if (lunar_calendar_present && IsChineseSet())   /* Title2 will be used to display the year and month */
    {
        x1 = cat82_title2_x1;
        y1 = cat82_title2_y1;
        x2 = cat82_title2_x2;
        x2 = (calendar_show_title3) ? cat82_title3_x1 - 1 : cat82_title2_x2;
        y2 = cat82_title2_y2;
    }
    else    /* Title1 will be used to display the year and month */
    {
        x1 = cat82_title1_x1;
        y1 = cat82_title1_y1;
        x2 = cat82_title1_x2;
        y2 = cat82_title1_y2;
    }
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

    ret = MMI_TRUE;

    *calendar_event = GUI_CALENDAR_PEN_NONE;

    switch (pen_event)
    {
        case MMI_PEN_EVENT_DOWN:
            g_calendar_pen_event = MMI_FALSE;
            if (PEN_CHECK_BOUND(x, y, x1, y1, (x2 - x1), (y2 - y1)))    /* Event is on the Title of the calendar */
            {
            #ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
                if (!Cat82_title1_month_arrow_disable)  /* check if month info is disabled, do not check in original mode */
            #endif 
                {
                    if (PEN_CHECK_BOUND
                        (x, y, x1 + MMI_CALENDAR_ARROW_OFFSET, y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
                         calendar_arrow_width, calendar_arrow_height))
                    {
                        /* First Arrow Pressed */
                        gdi_layer_lock_frame_buffer();
                        gui_show_transparent_image(
                            x1 + MMI_CALENDAR_ARROW_OFFSET,
                            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
                            (PU8) GetImage(CAL_LEFT_ARROW_DOWN_IMAGE),
                            0);
                        gdi_layer_unlock_frame_buffer();
                        gdi_layer_blt_previous(
                            x1 + MMI_CALENDAR_ARROW_OFFSET,
                            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
                            x1 + 2 * calendar_arrow_width + MMI_CALENDAR_ARROW_OFFSET,
                            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1) + calendar_arrow_height);
                        c->previous_month_function();
                    }
                    if (PEN_CHECK_BOUND
                        (x, y, x2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
                         y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1), calendar_arrow_width,
                         calendar_arrow_height))
                    {
                        /* Second Arrow Pressed */
                        gdi_layer_lock_frame_buffer();
                        gui_show_transparent_image(
                            x2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
                            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
                            (PU8) GetImage(CAL_RIGHT_ARROW_DOWN_IMAGE),
                            0);
                        gdi_layer_unlock_frame_buffer();
                        gdi_layer_blt_previous(
                            x2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
                            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1),
                            x2 - MMI_CALENDAR_ARROW_OFFSET,
                            y1 + ((y2 - y1 + 1) >> 1) - (calendar_arrow_height >> 1) + calendar_arrow_height);
                        c->next_month_function();
                    }
                }
            }
        #ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
            else if (PEN_CHECK_BOUND(x, y, xx1, yy1, (xx2 - xx1), (yy2 - yy1))) /* Event is on the Title of the calendar */
            {
                if (!Cat82_title1_arrow_disable)    /* check if year info is disabled */
                {
                    if (PEN_CHECK_BOUND
                        (x, y, xx1 + MMI_CALENDAR_ARROW_OFFSET,
                         yy1 + ((yy2 - yy1 + 1) >> 1) - (calendar_arrow_height >> 1), calendar_arrow_width,
                         calendar_arrow_height))
                    {
                        /* First Arrow Pressed */
                        gdi_layer_lock_frame_buffer();
                        gui_show_transparent_image(
                            xx1 + MMI_CALENDAR_ARROW_OFFSET,
                            yy1 + ((yy2 - yy1 + 1) >> 1) - (calendar_arrow_height >> 1),
                            (PU8) GetImage(CAL_LEFT_ARROW_DOWN_IMAGE),
                            0);
                        gdi_layer_unlock_frame_buffer();
                        gdi_layer_blt_previous(
                            xx1 + MMI_CALENDAR_ARROW_OFFSET,
                            yy1 + ((yy2 - yy1 + 1) >> 1) - (calendar_arrow_height >> 1),
                            xx1 + 2 * calendar_arrow_width + MMI_CALENDAR_ARROW_OFFSET,
                            yy1 + ((yy2 - yy1 + 1) >> 1) - (calendar_arrow_height >> 1) + calendar_arrow_height);
                        c->previous_year_function();
                    }
                    if (PEN_CHECK_BOUND
                        (x, y, xx2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
                         yy1 + ((yy2 - yy1 + 1) >> 1) - (calendar_arrow_height >> 1), calendar_arrow_width,
                         calendar_arrow_height))
                    {
                        /* Second Arrow Pressed */
                        gdi_layer_lock_frame_buffer();
                        gui_show_transparent_image(
                            xx2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
                            yy1 + ((yy2 - yy1 + 1) >> 1) - (calendar_arrow_height >> 1),
                            (PU8) GetImage(CAL_RIGHT_ARROW_DOWN_IMAGE),
                            0);
                        gdi_layer_unlock_frame_buffer();
                        gdi_layer_blt_previous(
                            xx2 - calendar_arrow_width - MMI_CALENDAR_ARROW_OFFSET,
                            yy1 + ((yy2 - yy1 + 1) >> 1) - (calendar_arrow_height >> 1),
                            xx2 - MMI_CALENDAR_ARROW_OFFSET,
                            yy1 + ((yy2 - yy1 + 1) >> 1) - (calendar_arrow_height >> 1) + calendar_arrow_height);
                        c->next_year_function();
                    }
                }
            }
        #endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
            else
            {
                /* Give control to gui layer */
                ret = gui_calendar_translate_pen_event(c, pen_event, x, y, calendar_event);
                if (ret)
                {
                    g_calendar_pen_event = MMI_TRUE;
                }
            }
            break;

        case MMI_PEN_EVENT_UP:
            if (g_calendar_pen_event)
            {
                ret = gui_calendar_translate_pen_event(c, pen_event, x, y, calendar_event);
            }
            break;

        case MMI_PEN_EVENT_MOVE:
            if (g_calendar_pen_event)
            {
                ret = gui_calendar_translate_pen_event(c, pen_event, x, y, calendar_event);
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

#endif /* (ENABLE_CALENDAR_CONTROL) */ 

