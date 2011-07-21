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
 *   wgui_categories_MM.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Main Menu related categories.
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"

#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "debuginitdef.h"
#undef __NEWSIMULATOR

#include "globaldefs.h"
#include "gui_data_types.h"
#include "gui_themes.h"
#include "gui.h"
#ifdef __MMI_UI_TECHNO_STATUS_ICON__
#include "gui_setting.h"
#endif 
#include "custdataprots.h"
#include "custmenures.h"
#include "eventsdef.h"
#include "custthemesres.h"
#include "mainmenudef.h"
#include "mainmenutype.h"
#include "gdi_include.h"
#include "lcd_sw_rnd.h"
#include "wgui.h"
#include "wgui_inputs.h"
#include "wgui_categories.h"
#include "wgui_softkeys.h"
#include "wgui_fixed_menus.h"
#include "wgui_categories_defs.h"
#include "wgui_draw_manager.h"
#include "wgui_categories_mm.h"
#ifdef __MMI_WGUI_MINI_TAB_BAR__
#include "wgui_tab_bars.h"
#endif 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
#include "wgui_categories_util.h"
#endif 
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif 
#include "satgprots.h"


/************ Global Variables *************/
extern S32 wgui_current_pop_up_description_index;
extern color_t active_title_txt_border_col;
extern BOOL r2lMMIFlag;

#ifdef __MMI_UI_TITLE_TRANSITION__
extern void set_title_transition(wgui_transition_styles style);
extern void reset_title_transition(void);
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
#ifdef __MMI_UI_MM_HORIZONTAL_PAGE_STYLE__
extern void register_horizontal_fixed_list_keys(void);
#endif 
extern void standard_animated_matrix_highlight_handler_type2(S32 item_index);
extern void CSD_show_animation_frames(S32 x, S32 y, U8 *image, S32 start_frame);

UI_string_type MMI_bg_string;
PU8 MMI_bg_image;
PU8 MMI_bg_animation;

#if (ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)

/* Direct main menu implementation                                */
/* Matrix main menu: Fixed number of items with all items being visible */

#if defined(__MMI_MAINLCD_240X320__)
#define MAIN_MENU_NORMAL_IMAGE_CLIP_WIDTH    (80)
#define MAIN_MENU_NORMAL_IMAGE_CLIP_HEIGHT   (60)
#define MAIN_MENU_NORMAL_IMAGE_WIDTH         (80)
#define MAIN_MENU_NORMAL_IMAGE_HEIGHT        (60)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH    (80)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT   (60)
#elif defined(__MMI_MAINLCD_220X176__)
#define MAIN_MENU_NORMAL_IMAGE_CLIP_WIDTH    (80)
#define MAIN_MENU_NORMAL_IMAGE_CLIP_HEIGHT      (50)
#define MAIN_MENU_NORMAL_IMAGE_WIDTH         (80)
#define MAIN_MENU_NORMAL_IMAGE_HEIGHT        (50)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH    (80)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT      (50)
#elif defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
#define MAIN_MENU_NORMAL_IMAGE_CLIP_WIDTH    (41)
#define MAIN_MENU_NORMAL_IMAGE_CLIP_HEIGHT   (37)
#define MAIN_MENU_NORMAL_IMAGE_WIDTH         (40)
#define MAIN_MENU_NORMAL_IMAGE_HEIGHT        (36)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH    (40)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT   (36)
#else
#define MAIN_MENU_NORMAL_IMAGE_CLIP_WIDTH    (60)
#if defined(__PROJECT_GALLITE_C01__)
#define MAIN_MENU_NORMAL_IMAGE_CLIP_HEIGHT      (60)
#define MAIN_MENU_NORMAL_IMAGE_WIDTH         (60)
#define MAIN_MENU_NORMAL_IMAGE_HEIGHT        (60)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH    (60)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT      (60)
#else
#define MAIN_MENU_NORMAL_IMAGE_CLIP_HEIGHT      (56)
#define MAIN_MENU_NORMAL_IMAGE_WIDTH         (60)
#define MAIN_MENU_NORMAL_IMAGE_HEIGHT        (56)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH    (60)
#define MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT      (56)
#endif
#endif
#define MAIN_MENU_N_ROWS                     (3)
#define MAIN_MENU_N_COLUMNS                  (3)
#define MAIN_MENU_MAX_ITEMS                  (MAIN_MENU_N_ROWS*MAIN_MENU_N_COLUMNS)
#define MAIN_MENU_HIGHLIGHT_OFFSET_X            ((MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH>>1)-(MAIN_MENU_NORMAL_IMAGE_WIDTH>>1))
#define MAIN_MENU_HIGHLIGHT_OFFSET_Y            ((MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT>>1)-(MAIN_MENU_NORMAL_IMAGE_HEIGHT>>1))
#define MAIN_MENU_HIGHLIGHTED_GRID_COLOR     {  0,0,0,100      }
#define MAIN_MENU_NORMAL_GRID_COLOR          {  160,160,160,100   }
#ifdef __MMI_MAINLCD_220X176__
#define MAIN_MENU_X1                      (0)
#define MAIN_MENU_Y1                      (MMI_TITLE_HEIGHT+1)
#define MAIN_MENU_X2                      (160)
#define MAIN_MENU_Y2                      (MMI_TITLE_HEIGHT+100+1)
#else
#define MAIN_MENU_X1                      (0)
#define MAIN_MENU_Y1                      (MMI_TITLE_HEIGHT+1)
#define MAIN_MENU_X2                      (127)
#define MAIN_MENU_Y2                      (MMI_TITLE_HEIGHT+100+1)
#endif

#define MAIN_MENU_ITEM_HIGHLIGHTED           0x00000001
#define MAIN_MENU_LOOP                       0x00000002
#define MAIN_MENU_DISPLAY_GRID               0x00000004
#define MAIN_MENU_DISPLAY_ITEM_GRID          0x00000008

extern U16 title_bg_id;
extern icontext_button MMI_softkeys[];

extern void _show_bitmap(S32 x1, S32 y1, bitmap *b);
extern void _get_bitmap(S32 x1, S32 y1, S32 x2, S32 y2, bitmap *b);
extern void (*MMI_matrix_highlight_handler) (S32 index);

 #ifdef __MMI_SCREEN_ROTATE__ // wangmg[3.1]
extern MMI_BOOL mmi_frm_is_screen_width_height_swapped(void);
#endif
U8 matrix_main_menu_highlight_bitmap_data[(MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH) * (MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT) *
                                          GDI_MAINLCD_BIT_PER_PIXEL / 8];
/* structure of matrix menu */
/* Store current main menu type..matrix,list,circular */
S32 MMI_main_menu_type = MATRIX_MENU;
U16 gMainMenuAnimation[MAX_MAIN_MENU_ITEMS];
U16 gMainMenuTitleIcon[MAX_MAIN_MENU_ITEMS];
S32 main_menu_index = -1;

PU8 _wgui_main_menu_items[MAIN_MENU_MAX_ITEMS];
matrix_main_menu _wgui_main_menu;
UI_string_type _wgui_main_menu_item_strings[MAIN_MENU_MAX_ITEMS];

#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING))
S32 wgui_matrix_main_menu_frame_counter = 0;
extern matrix_main_menu _wgui_main_menu;
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
BOOL inMatrixNoTitleFlag = FALSE;
#endif 
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
void (*Cat414MatrixExitHandler) (S32 index) = NULL;
#endif 

/* 082806 240x320 matrix Start */
#if defined (__MMI_WALLPAPER_ON_BOTTOM__)
static UI_image_ID_type  mm_bg_ID;
static S8 *mm_bg_filename;
static S32 mm_bg_x, mm_bg_y;
static U8 mm_bg_opacity;
static S32 mm_bg_flags;
#endif /* defined (__MMI_WALLPAPER_ON_BOTTOM__) */ 
/* 082806 240x320 matrix End */

/*****************************************************************************
 * FUNCTION
 *  setup_main_menu
 * DESCRIPTION
 *  set main menu values such as number sof itens, size of image
 * PARAMETERS
 *  m                   [IN]        Menu
 *  x                   [IN]        Start of x position
 *  y                   [IN]        Start of y position
 *  n_items             [IN]        Number of items
 *  list_of_items       [IN]        Images of menus
 * RETURNS
 *  void
 *****************************************************************************/
void setup_main_menu(matrix_main_menu *m, S32 x, S32 y, S32 n_items, PU8 *list_of_items)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    m->x = x;
    m->y = y;
    if (n_items < 0)
    {
        n_items = 0;
    }
    if (n_items > MAIN_MENU_MAX_ITEMS)
    {
        n_items = MAIN_MENU_MAX_ITEMS;
    }
    m->flags = 0;
    m->n_items = n_items;
    m->list_of_items = list_of_items;
    m->highlighted_item = -1;
    m->item_highlighted = UI_dummy_function_S32 ;
    m->item_unhighlighted = UI_dummy_function_S32 ;
    m->highlight_area.color_depth = 16;
    m->highlight_area.palette = NULL;
    m->highlight_area.data = matrix_main_menu_highlight_bitmap_data;
    m->highlight_area.xsize = MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH;
    m->highlight_area.ysize = MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT;
    m->highlight_area.row_bytes = m->highlight_area.xsize * 2;
}


/*****************************************************************************
 * FUNCTION
 *  show_main_menu
 * DESCRIPTION
 *  show main menu
 * PARAMETERS
 *  m       [IN]        Menu
 * RETURNS
 *  void
 *****************************************************************************/
void show_main_menu(matrix_main_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, row, column, x, y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    row = column = 0;
    gui_reset_clip();
    x = m->x;
    y = m->y;
    for (i = 0; i < m->n_items; i++)
    {
        gui_set_clip(x, y, x + MAIN_MENU_NORMAL_IMAGE_CLIP_WIDTH - 1, y + MAIN_MENU_NORMAL_IMAGE_CLIP_HEIGHT - 1);
        gui_show_image(
            x + DIRECT_MAIN_MENU_FIRST_FRAME_SHIFT_X,
            y + DIRECT_MAIN_MENU_FIRST_FRAME_SHIFT_Y,
            m->list_of_items[i]);
        column++;
        x += MAIN_MENU_NORMAL_IMAGE_WIDTH;
        if (column >= MAIN_MENU_N_COLUMNS)
        {
            column = 0;
            x = m->x;
            row++;
            y += MAIN_MENU_NORMAL_IMAGE_HEIGHT;
            if (row >= MAIN_MENU_N_ROWS)
            {
                break;
            }
        }
    }
    m->flags &= ~MAIN_MENU_ITEM_HIGHLIGHTED;
    if (m->flags & MAIN_MENU_DISPLAY_GRID)
    {
        color_t c = MAIN_MENU_NORMAL_GRID_COLOR;
        S32 ii, xx, yy, x1, y1, x2, y2;

        gui_reset_clip();
        xx = x1 = m->x;
        yy = y1 = m->y;
        x2 = x1 + (MAIN_MENU_NORMAL_IMAGE_WIDTH * MAIN_MENU_N_COLUMNS);
        y2 = y1 + (MAIN_MENU_NORMAL_IMAGE_HEIGHT * MAIN_MENU_N_ROWS);
        for (ii = 0; ii < 4; ii++)
        {
            gui_draw_horizontal_line(x1, x2, yy, c);
            gui_draw_vertical_line(y1, y2, xx, c);
            xx += MAIN_MENU_NORMAL_IMAGE_WIDTH;
            yy += MAIN_MENU_NORMAL_IMAGE_HEIGHT;
        }
    }
}

#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING))


/*****************************************************************************
 * FUNCTION
 *  update_main_menu_highlight
 * DESCRIPTION
 *  Enlarge the image of highlighted main menu item
 * PARAMETERS
 *  m       [IN]        Menu
 * RETURNS
 *  void
 *****************************************************************************/
void update_main_menu_highlight(matrix_main_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 row, column, x, y, tx, ty;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_reset_clip();
    /* Display the screen bitmap grabbed earlier                */
    if (m->flags & MAIN_MENU_ITEM_HIGHLIGHTED)
    {
        _show_bitmap(m->save_x, m->save_y, &m->highlight_area);
    }
    if ((m->highlighted_item < 0) || (m->highlighted_item >= m->n_items))
    {
        m->flags &= ~MAIN_MENU_ITEM_HIGHLIGHTED;
        return;
    }
    /* Calculate (x,y) for the highlight                        */
    row = m->highlighted_item / MAIN_MENU_N_COLUMNS;
    column = m->highlighted_item % MAIN_MENU_N_COLUMNS;
    ty = row * MAIN_MENU_NORMAL_IMAGE_HEIGHT;
    tx = column * MAIN_MENU_NORMAL_IMAGE_WIDTH;
    x = tx - MAIN_MENU_HIGHLIGHT_OFFSET_X;
    y = ty - MAIN_MENU_HIGHLIGHT_OFFSET_Y;
    if (m->flags & MAIN_MENU_DISPLAY_ITEM_GRID)
    {
        x -= 1;
        y -= 1;
    }

#if(DIRECT_MAIN_MENU_BOUNDARY_SPACING)
    if (x < 0)
    {
        x = 0;
    }
    if ((x + MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH) > (MAIN_MENU_N_COLUMNS * MAIN_MENU_NORMAL_IMAGE_WIDTH))
    {
        x = MAIN_MENU_N_COLUMNS * MAIN_MENU_NORMAL_IMAGE_WIDTH - MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH;
    }
    if (y < 0)
    {
        y = 0;
    }
    if ((y + MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT) > (MAIN_MENU_N_ROWS * MAIN_MENU_NORMAL_IMAGE_HEIGHT - 1))
    {
        y = MAIN_MENU_N_ROWS * MAIN_MENU_NORMAL_IMAGE_HEIGHT - MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT - 1;
    }
#else /* (DIRECT_MAIN_MENU_BOUNDARY_SPACING) */ 
    if (x < -1)
    {
        x = -1;
    }
    if ((x + MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH) > (MAIN_MENU_N_COLUMNS * MAIN_MENU_NORMAL_IMAGE_WIDTH + 1))
    {
        x = MAIN_MENU_N_COLUMNS * MAIN_MENU_NORMAL_IMAGE_WIDTH - MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH + 1;
    }
    if (y < -1)
    {
        y = -1;
    }
    if ((y + MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT) > (MAIN_MENU_N_ROWS * MAIN_MENU_NORMAL_IMAGE_HEIGHT))
    {
        y = MAIN_MENU_N_ROWS * MAIN_MENU_NORMAL_IMAGE_HEIGHT - MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT;
    }
#endif /* (DIRECT_MAIN_MENU_BOUNDARY_SPACING) */ 

    x += m->x;
    y += m->y;
    if (m->flags & MAIN_MENU_DISPLAY_ITEM_GRID)
    {   /* Grab a new screen bitmap for the portion that will be affected */
        _get_bitmap(
            x,
            y,
            x + MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH,
            y + MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT,
            &m->highlight_area);
        m->save_x = x;
        m->save_y = y;
        /* Display the highlighted item */
        CSD_show_animation_frames(x, y, m->list_of_items[m->highlighted_item], 1);
    }
    else
    {   /* Grab a new screen bitmap for the portion that will be affected */
        _get_bitmap(
            x,
            y,
            x + MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH - 1,
            y + MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT - 1,
            &m->highlight_area);
        m->save_x = x;
        m->save_y = y;
        /* Display the highlighted item */
        CSD_show_animation_frames(x, y, m->list_of_items[m->highlighted_item], 1);
    }
    m->flags |= MAIN_MENU_ITEM_HIGHLIGHTED;

    gdi_layer_blt_previous(MAIN_MENU_X1, MAIN_MENU_Y1 - 1, MAIN_MENU_X2, MAIN_MENU_Y2);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_matrix_main_menu_end_frame
 * DESCRIPTION
 *  show the frames of main menu animation image of high lighted item
 * PARAMETERS
 *  void
 *  matrix_main_menu(?)     [IN]        *m  :- menu
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_matrix_main_menu_end_frame(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_matrix_main_menu_frame_counter >= 1)
    {
        update_main_menu_highlight(&_wgui_main_menu);
        wgui_matrix_main_menu_frame_counter = 0;
        gui_start_timer(DIRECT_MAIN_MENU_FRAME_SKIP_RATE, wgui_matrix_main_menu_end_frame);
    }
}


/*****************************************************************************
 * FUNCTION
 *  show_main_menu_highlight
 * DESCRIPTION
 *  show main menu highlihgted image
 * PARAMETERS
 *  m       [IN]        Menu
 * RETURNS
 *  void
 *****************************************************************************/
void show_main_menu_highlight(matrix_main_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_matrix_main_menu_frame_counter > 0)
    {
        wgui_matrix_main_menu_frame_counter++;
    }
    else
    {
        wgui_matrix_main_menu_frame_counter = 1;
        gui_start_timer(UI_FRAME_START_TIMEOUT, wgui_matrix_main_menu_end_frame);
    }
}

#else /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 


/*****************************************************************************
 * FUNCTION
 *  show_main_menu_highlight
 * DESCRIPTION
 *  Enlarge the image of highlighted main menu item
 * PARAMETERS
 *  m       [IN]        Menu
 * RETURNS
 *  void
 *****************************************************************************/
void show_main_menu_highlight(matrix_main_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 row, column, x, y, tx, ty;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_reset_clip();
    /* Display the screen bitmap grabbed earlier                */
    if (m->flags & MAIN_MENU_ITEM_HIGHLIGHTED)
    {
        _show_bitmap(m->save_x, m->save_y, &m->highlight_area);
    }
    if ((m->highlighted_item < 0) || (m->highlighted_item >= m->n_items))
    {
        m->flags &= ~MAIN_MENU_ITEM_HIGHLIGHTED;
        return;
    }
    /* Calculate (x,y) for the highlight                        */
    row = m->highlighted_item / MAIN_MENU_N_COLUMNS;
    column = m->highlighted_item % MAIN_MENU_N_COLUMNS;
    ty = row * MAIN_MENU_NORMAL_IMAGE_HEIGHT;
    tx = column * MAIN_MENU_NORMAL_IMAGE_WIDTH;
    x = tx - MAIN_MENU_HIGHLIGHT_OFFSET_X;
    y = ty - MAIN_MENU_HIGHLIGHT_OFFSET_Y;
    if (m->flags & MAIN_MENU_DISPLAY_ITEM_GRID)
    {
        x -= 1;
        y -= 1;
    }
    if (x < 0)
    {
        x = 0;
    }
    if (y < 0)
    {
        y = 0;
    }
    if ((x + MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH) > (MAIN_MENU_N_COLUMNS * MAIN_MENU_NORMAL_IMAGE_WIDTH))
    {
        x = MAIN_MENU_N_COLUMNS * MAIN_MENU_NORMAL_IMAGE_WIDTH - MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH;
    }
    if ((y + MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT) > (MAIN_MENU_N_ROWS * MAIN_MENU_NORMAL_IMAGE_HEIGHT))
    {
        y = MAIN_MENU_N_ROWS * MAIN_MENU_NORMAL_IMAGE_HEIGHT - MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT;
    }
    x += m->x;
    y += m->y;
    if (m->flags & MAIN_MENU_DISPLAY_ITEM_GRID)
    {   /* Grab a new screen bitmap for the portion that will be affected */
        _get_bitmap(
            x,
            y,
            x + MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH,
            y + MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT,
            &m->highlight_area);
        m->save_x = x;
        m->save_y = y;
        /* Display the highlighted item                          */
        CSD_show_animation_frames(x, y, m->list_of_items[m->highlighted_item], 1);
    }
    else
    {   /* Grab a new screen bitmap for the portion that will be affected */
        _get_bitmap(
            x,
            y,
            x + MAIN_MENU_HIGHLIGHTED_IMAGE_WIDTH - 1,
            y + MAIN_MENU_HIGHLIGHTED_IMAGE_HEIGHT - 1,
            &m->highlight_area);
        m->save_x = x;
        m->save_y = y;
        /* Display the highlighted item                          */
        CSD_show_animation_frames(x, y, m->list_of_items[m->highlighted_item], 1);
    }
    m->flags |= MAIN_MENU_ITEM_HIGHLIGHTED;
}

#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 


/*****************************************************************************
 * FUNCTION
 *  main_menu_previous_row
 * DESCRIPTION
 *  move to previous row of main menu item
 * PARAMETERS
 *  m       [IN]        Menu
 * RETURNS
 *  void
 *****************************************************************************/
void main_menu_previous_row(matrix_main_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 row, column;
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    row = old_highlighted_item / MAIN_MENU_N_COLUMNS;
    column = old_highlighted_item % MAIN_MENU_N_COLUMNS;
    if (m->flags & MAIN_MENU_LOOP)
    {
        if (row <= 0)
        {
            row = MAIN_MENU_N_ROWS - 1;
        }
        else
        {
            row--;
        }

    #ifdef __MMI_MAIN_MENU_CYCLE__
        if (row == (MAIN_MENU_N_ROWS - 1))
            if (column > 0)
            {
                column--;
            }
            else
            {
                column = MAIN_MENU_N_COLUMNS - 1;
            }
    #endif /* __MMI_MAIN_MENU_CYCLE__ */ 
    }
    else
    {
        if (row > 0)
        {
            row--;
        }
    }
    m->highlighted_item = row * MAIN_MENU_N_COLUMNS + column;
    if (m->highlighted_item > (m->n_items - 1))
    {
        m->highlighted_item = m->n_items - 1;
    }
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
        gui_hide_animations();
        show_main_menu_highlight(m);
    }
}


/*****************************************************************************
 * FUNCTION
 *  main_menu_next_row
 * DESCRIPTION
 *  move to next  row of main menu item
 * PARAMETERS
 *  m       [IN]        Menu
 * RETURNS
 *  void
 *****************************************************************************/
void main_menu_next_row(matrix_main_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 row, column;
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    row = old_highlighted_item / MAIN_MENU_N_COLUMNS;
    column = old_highlighted_item % MAIN_MENU_N_COLUMNS;
    if (m->flags & MAIN_MENU_LOOP)
    {
        if (row >= (MAIN_MENU_N_ROWS - 1))
        {
            row = 0;
        }
        else
        {
            row++;
        }
    #ifdef __MMI_MAIN_MENU_CYCLE__
        if (row == 0)
        {
            column = (column + 1) % MAIN_MENU_N_COLUMNS;
        }
    #endif /* __MMI_MAIN_MENU_CYCLE__ */ 
    }
    else
    {
        if (row < (MAIN_MENU_N_ROWS - 1))
        {
            row++;
        }
    }
    m->highlighted_item = row * MAIN_MENU_N_COLUMNS + column;
    if (m->highlighted_item > (m->n_items - 1))
    {
        m->highlighted_item = m->n_items - 1;
    }
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
        gui_hide_animations();
        show_main_menu_highlight(m);
    }
}


/*****************************************************************************
 * FUNCTION
 *  main_menu_previous_column
 * DESCRIPTION
 *  move to previous column of main menu item
 * PARAMETERS
 *  m       [IN]        Menu
 * RETURNS
 *  void
 *****************************************************************************/
void main_menu_previous_column(matrix_main_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 row, column;
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    row = old_highlighted_item / MAIN_MENU_N_COLUMNS;
    column = old_highlighted_item % MAIN_MENU_N_COLUMNS;
    if (m->flags & MAIN_MENU_LOOP)
    {
        if (column <= 0)
        {
            column = MAIN_MENU_N_COLUMNS - 1;
            if (row <= 0)
            {
                row = MAIN_MENU_N_ROWS - 1;
            }
            else
            {
                row--;
            }
        }
        else
        {
            column--;
        }
    }
    else
    {
        if (column > 0)
        {
            column--;
        }
    }
    m->highlighted_item = row * MAIN_MENU_N_COLUMNS + column;
    if (m->highlighted_item > (m->n_items - 1))
    {
        m->highlighted_item = m->n_items - 1;
    }
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
        gui_hide_animations();
        show_main_menu_highlight(m);
    }
}


/*****************************************************************************
 * FUNCTION
 *  main_menu_next_column
 * DESCRIPTION
 *  move to next  column of main menu item
 * PARAMETERS
 *  m       [IN]        Menu
 * RETURNS
 *  void
 *****************************************************************************/
void main_menu_next_column(matrix_main_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 row, column;
    S32 old_highlighted_item = m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    row = old_highlighted_item / MAIN_MENU_N_COLUMNS;
    column = old_highlighted_item % MAIN_MENU_N_COLUMNS;
    if (m->flags & MAIN_MENU_LOOP)
    {
        if (column >= (MAIN_MENU_N_COLUMNS - 1))
        {
            column = 0;
            if (row >= (MAIN_MENU_N_ROWS - 1))
            {
                row = 0;
            }
            else
            {
                row++;
            }
        }
        else
        {
            column++;
        }
    }
    else
    {
        if (column < (MAIN_MENU_N_COLUMNS - 1))
        {
            column++;
        }
    }
    m->highlighted_item = row * MAIN_MENU_N_COLUMNS + column;
    if (m->highlighted_item > (m->n_items - 1))
    {
        m->highlighted_item = m->n_items - 1;
    }
    if (old_highlighted_item != m->highlighted_item)
    {
        m->item_unhighlighted(old_highlighted_item);
        m->item_highlighted(m->highlighted_item);
        gui_hide_animations();
        show_main_menu_highlight(m);
    }
}


/*****************************************************************************
 * FUNCTION
 *  main_menu_highlight_item
 * DESCRIPTION
 *  handle main menu highlighted item
 * PARAMETERS
 *  m           [IN]        Index of highlighted item
 *  index       [IN]        Index of highlighted item
 *  menu(?)     [IN]        Index of highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void main_menu_highlight_item(matrix_main_menu *m, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((index < 0) || (index >= m->n_items))
    {
        return;
    }
    if (index == m->highlighted_item)
    {
        return;
    }
    old_highlighted_item = m->highlighted_item;
    m->highlighted_item = index;
    m->item_unhighlighted(old_highlighted_item);
    m->item_highlighted(m->highlighted_item);
    gui_hide_animations();
    show_main_menu_highlight(m);
}


/*****************************************************************************
 * FUNCTION
 *  refresh_main_menu
 * DESCRIPTION
 *  refresh main menu
 * PARAMETERS
 *  m       [IN]        Menu
 * RETURNS
 *  void
 *****************************************************************************/
void refresh_main_menu(matrix_main_menu *m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
    show_main_menu(m);
    show_main_menu_highlight(m);
}

/* Main menu wrappers   */


/*****************************************************************************
 * FUNCTION
 *  wgui_main_menu_previous_row
 * DESCRIPTION
 *  move to main enu previous row
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_main_menu_previous_row(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    main_menu_previous_row(&_wgui_main_menu);
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING))
#else /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
    gdi_layer_blt_previous(MAIN_MENU_X1, MAIN_MENU_Y1, MAIN_MENU_X2, MAIN_MENU_Y2);
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_main_menu_next_row
 * DESCRIPTION
 *  move to main menu next row
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_main_menu_next_row(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    main_menu_next_row(&_wgui_main_menu);
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING))
#else /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
    gdi_layer_blt_previous(MAIN_MENU_X1, MAIN_MENU_Y1, MAIN_MENU_X2, MAIN_MENU_Y2);
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_main_menu_previous_column
 * DESCRIPTION
 *  move to main menu previous column
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_main_menu_previous_column(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    main_menu_previous_column(&_wgui_main_menu);
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING))
#else /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
    gdi_layer_blt_previous(MAIN_MENU_X1, MAIN_MENU_Y1, MAIN_MENU_X2, MAIN_MENU_Y2);
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_main_menu_next_column
 * DESCRIPTION
 *  move to main menu next column
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_main_menu_next_column(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    main_menu_next_column(&_wgui_main_menu);
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING))
#else /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
    gdi_layer_blt_previous(MAIN_MENU_X1, MAIN_MENU_Y1, MAIN_MENU_X2, MAIN_MENU_Y2);
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_main_menu_goto_item
 * DESCRIPTION
 *  highlight particular menu item
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_main_menu_goto_item(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    main_menu_highlight_item(&_wgui_main_menu, index);
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING))
#else /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
    gdi_layer_blt_previous(MAIN_MENU_X1, MAIN_MENU_Y1, MAIN_MENU_X2, MAIN_MENU_Y2);
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_main_menu_key_handler
 * DESCRIPTION
 *  set the key handlers of main menu
 * PARAMETERS
 *  vkey_code       [IN]        Key code
 *  key_state       [IN]        Key state
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_main_menu_key_handler(S32 vkey_code, S32 key_state)
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
                wgui_main_menu_previous_column();
                break;
            case 39:
                wgui_main_menu_next_column();
                break;
            case 38:
                wgui_main_menu_previous_row();
                break;
            case 40:
                wgui_main_menu_next_row();
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
 *  wgui_register_main_menu_keys
 * DESCRIPTION
 *  register main menu keys handlers
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_register_main_menu_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(wgui_main_menu_previous_row, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(wgui_main_menu_next_row, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(wgui_main_menu_previous_column, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(wgui_main_menu_next_column, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(wgui_main_menu_previous_column, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(wgui_main_menu_next_column, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    register_keyboard_key_handler(wgui_main_menu_key_handler);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_clear_main_menu_keys
 * DESCRIPTION
 *  clear key handlers of main menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_clear_main_menu_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearKeyHandler(KEY_UP_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_main_menu_highlight_handler
 * DESCRIPTION
 *  main menu highlight handler
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_main_menu_highlight_handler(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = index + 1;
    if (index >= 0 && index <= (_wgui_main_menu.n_items))
    {
        MMI_highlighted_item_text = _wgui_main_menu_item_strings[index];
        MMI_title_string = MMI_highlighted_item_text;
    }
#if(!DISABLE_MAIN_MENU_HIGHLIGHT_TITLE_UPDATE)
    draw_title();
#endif 
    MMI_matrix_highlight_handler(index);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_register_main_menu_shortcut_handler
 * DESCRIPTION
 *  main menu shorcut  handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_register_main_menu_shortcut_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_menu_shortcut_keys();
    register_menu_shortcut_handler(wgui_main_menu_goto_item);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_setup_main_menu
 * DESCRIPTION
 *  Create main menu
 * PARAMETERS
 *  n_items                 [IN]        Number of items
 *  item_list               [IN]        Item list
 *  image_list              [IN]        Image list
 *  highlighted_item        [IN]        Highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_setup_main_menu(S32 n_items, U16 *item_list, U16 *image_list, S32 highlighted_item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; (i < n_items) && (i < MAIN_MENU_MAX_ITEMS); i++)
    {
        _wgui_main_menu_items[i] = (PU8) GetImage(image_list[i]);
        _wgui_main_menu_item_strings[i] = (UI_string_type) GetString(item_list[i]);
    }

#ifdef __MMI_MERGE_SAT_TO_MAINMENU__
#ifndef WAP_SUPPORT
#ifndef __MMI_MULTI_SIM__
    if (IsSATPresent(0))
    {
        _wgui_main_menu_item_strings[7] = (UI_string_type) GetSATMainMenuAlphaID();
    }
#endif /* __MMI_MULTI_SIM__ */ 
#endif /* WAP_SUPPORT */ 
#endif /* __MMI_MERGE_SAT_TO_MAINMENU__ */ 

    setup_main_menu(&_wgui_main_menu, 1, (MMI_title_y + MMI_title_height) + 1, n_items, _wgui_main_menu_items);
    _wgui_main_menu.flags |= MAIN_MENU_LOOP;
    _wgui_main_menu.highlighted_item = highlighted_item;
    _wgui_main_menu.item_highlighted = wgui_main_menu_highlight_handler;
    wgui_main_menu_highlight_handler(highlighted_item);
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING))
    wgui_matrix_main_menu_frame_counter = 0;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_redraw_main_menu
 * DESCRIPTION
 *  redraw main menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_redraw_main_menu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    refresh_main_menu(&_wgui_main_menu);
}


/*****************************************************************************
 * FUNCTION
 *  set_matrix_main_menu_category_history
 * DESCRIPTION
 *  set main menu matirx history
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
U8 set_matrix_main_menu_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        matrix_main_menu_category_history *h = (matrix_main_menu_category_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            _wgui_main_menu.highlighted_item = h->highlighted_item;
            _wgui_main_menu.flags = h->flags;
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  get_matrix_main_menu_category_history
 * DESCRIPTION
 *  get main menu matirx history
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void get_matrix_main_menu_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        U16 hID = (U16) (history_ID | 0x8000);
        matrix_main_menu_category_history *h = (matrix_main_menu_category_history*) history_buffer;

        h->history_ID = hID;
        h->highlighted_item = (S16) _wgui_main_menu.highlighted_item;
        h->flags = _wgui_main_menu.flags;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_close_main_menu
 * DESCRIPTION
 *  close main menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_close_main_menu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_clear_main_menu_keys();
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING))
    gui_cancel_timer(wgui_matrix_main_menu_end_frame);
    wgui_matrix_main_menu_frame_counter = 0;
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING)) */ 
}

#endif /* (ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION) */ 

/* For 3D circular main menu */
extern gdi_handle animation_handle;

#ifdef __MMI_UI_TECHNO_STATUS_ICON__
extern void arrange_status_icons();
extern void show_status_icons();
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
#if((ENABLE_CIRCULAR_3D_MENU) || (ENABLE_ROTATE_MENU))
extern void stop_rotate_circular_menu(void);


/*****************************************************************************
 * FUNCTION
 *  set_circular_menu_category_history
 * DESCRIPTION
 *  set main menu matirx history
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
U8 set_circular_menu_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        circular_menu_category_history *h = (circular_menu_category_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            wgui_CM3D_menu.highlighted_item = h->highlighted_item;
            wgui_CM3D_menu.flags = h->flags;
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  get_circular_menu_category_history
 * DESCRIPTION
 *  get main menu matirx history
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void get_circular_menu_category_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        U16 hID = (U16) (history_ID | 0x8000);
        circular_menu_category_history *h = (circular_menu_category_history*) history_buffer;

        h->history_ID = hID;
        h->highlighted_item = (S16) wgui_CM3D_menu.highlighted_item;
        h->flags = wgui_CM3D_menu.flags;
    }
}
#endif 

/*****************************************************************************
 * FUNCTION
 *  draw_bg_animation
 * DESCRIPTION
 *  Displays the background image for new mainmenu category
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_bg_animation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iw, ih;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_measure_image(MMI_bg_animation, &iw, &ih);
    if (animation_handle != GDI_ERROR_HANDLE)
    {
        gdi_image_stop_animation(animation_handle);
    }
    wgui_set_animation_image(MMI_content_width - iw, UI_device_height - MMI_button_bar_height - ih, MMI_bg_animation);
    gui_set_clip(
        MMI_content_width - iw,
        UI_device_height - MMI_button_bar_height - ih,
        MMI_content_width - 1,
        UI_device_height - MMI_button_bar_height);
#if defined(MMI_ON_HARDWARE_P) && USE_PLUTO_GIF_LIBRARY
    CSD_show_animation(MMI_content_width - iw, UI_device_height - MMI_button_bar_height - ih, MMI_bg_animation);
#else /* defined(MMI_ON_HARDWARE_P) && USE_PLUTO_GIF_LIBRARY */ 
    gdi_image_draw_animation(
        MMI_content_width - iw,
        UI_device_height - MMI_button_bar_height - ih,
        MMI_bg_animation,
        &animation_handle);
#endif /* defined(MMI_ON_HARDWARE_P) && USE_PLUTO_GIF_LIBRARY */ 
}


/*****************************************************************************
 * FUNCTION
 *  draw_bg
 * DESCRIPTION
 *  Displays the background image for new mainmenu category
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_bg(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight, character_height, tx, w;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    gdi_layer_push_clip();
    gdi_layer_push_text_clip();
    gui_set_clip(0, 0, UI_device_width - 1, UI_device_height - 1);
    gdi_layer_set_text_clip(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1);
    if (MMI_bg_image != NULL)
    {
        gui_measure_image(MMI_bg_image, &iwidth, &iheight);
        gui_show_image(0, 0, MMI_bg_image);
    }

    if (!r2lMMIFlag)
    {
        tx = 2;
    }
    else
    {
        tx = MMI_title_width - 2;
    }

    if (MMI_bg_string != NULL)  /* Fix for long titles  */
    {
        S32 tw;

        if (!r2lMMIFlag)
        {
            tw = MMI_title_width - tx;
        }
        else
        {
            tw = tx;
        }

        gui_set_font((UI_font_type) (current_MMI_theme->window_title_font));
        gui_measure_string(MMI_bg_string, &w, &character_height);
        gui_set_text_color(*current_MMI_theme->title_text_color);
        gui_set_text_border_color(*current_MMI_theme->title_text_border_color);
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(
                MMI_title_x + tx + w,
                MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
        }
        else
        {
            gui_move_text_cursor(MMI_title_x + tx, MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)));
        }
        if (!MMI_disable_title_shortcut_display && MMI_menu_shortcut_number >= 0)
        {
            tw -= MMI_menu_shortcut_box.width;
            if (w < tw)
            {
                S32 _tx = tx;

                if (!r2lMMIFlag)
                {
                    tx = tx + (tw >> 1) - (w >> 1); /* Centers the title */
                    if (tx < _tx)
                    {
                        tx = _tx;
                    }
                }
                else
                {
                    tx = (MMI_title_width >> 1) - (w >> 1);
                    tx += w;
                    if (tx > _tx)
                    {
                        tx = _tx;
                    }
                }
            }
        }
        else
        {
            if (w < tw)
            {
                S32 _tx = tx;

                if (!r2lMMIFlag)
                {
                    tx = (MMI_title_width >> 1) - (w >> 1);
                    if (tx < _tx)
                    {
                        tx = _tx;
                    }
                }
                else
                {
                    tx = (MMI_title_width >> 1) - (w >> 1);
                    tx += w;
                    if (tx > _tx)
                    {
                        tx = _tx;
                    }
                }
            }
        }
    #if defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__)
        gui_print_truncated_borderd_text(
            MMI_title_x + tx,
            MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)),
            tw,
            MMI_bg_string);
    #else /* defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__) */ 
        gui_print_truncated_text(
            MMI_title_x + tx,
            MMI_title_y + ((MMI_title_height >> 1) - (character_height >> 1)),
            tw,
            MMI_bg_string);
    #endif /* defined(__MMI_DEFAULT_THEME_3__) || defined(__MMI_DEFAULT_THEME_1__) */ 
    }
    gdi_layer_unlock_frame_buffer();
    show_menu_shortcut();
    gdi_layer_blt_previous(
        MMI_title_x,
        MMI_title_y,
        MMI_title_x + MMI_title_width - 1,
        MMI_title_y + MMI_title_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  standard_circular_3d_highlight_handler
 * DESCRIPTION
 *  System Highlight handler of 3d circular list
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_circular_3d_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    MMI_circular_3D_highlight_handler(item_index);
}


/*****************************************************************************
 * FUNCTION
 *  standard_bg_ani_list_highlight_handler
 * DESCRIPTION
 *  System Highlight handler of bg_ani_list
 *  
 *  Internal function
 * PARAMETERS
 *  item_index      [IN]        Index of current highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void standard_bg_ani_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    MMI_bg_string = MMI_highlighted_item_text;
    MMI_bg_image = get_image((UI_image_ID_type ) (MAIN_MENU_BG_IMAGE_BASE + item_index));
#ifdef __MMI_MAINLCD_220X176__
    MMI_bg_animation = get_image((UI_image_ID_type ) (MAIN_MENU_ICON_IMGBASE + item_index));
#else
    switch (item_index)
    {
        case 0:
        case 1:
            MMI_bg_animation = get_image((UI_image_ID_type ) (MAIN_MENU_PHONEBOOK_ICON + item_index));
            break;
        case 2:
            MMI_bg_animation = get_image((UI_image_ID_type ) (MAIN_MENU_CALL_HISTORY_ICON));
            break;
        case 3:
            MMI_bg_animation = get_image((UI_image_ID_type ) (MAIN_MENU_SETTINGS_ICON));
            break;
        case 4:
            MMI_bg_animation = get_image((UI_image_ID_type ) (MAIN_MENU_FUNANDGAMES_ICON));
            break;
        case 5:
            MMI_bg_animation = get_image((UI_image_ID_type ) (MAIN_MENU_PROFILES_ICON));
            break;
        case 6:
            MMI_bg_animation = get_image((UI_image_ID_type ) (MAIN_MENU_ORGANIZER_ICON));
            break;
        case 7:
            MMI_bg_animation = get_image((UI_image_ID_type ) (MAIN_MENU_SERVICES_ICON));
            break;
        case 8:
            MMI_bg_animation = get_image((UI_image_ID_type ) (MAIN_MENU_SHORTCUTS_ICON));
            break;

    }
#endif
    ExecuteCurrHiliteHandler(item_index);
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_pop_up_description_stop_scroll();
    wgui_current_pop_up_description_index = item_index;
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 
}


/*****************************************************************************
 * FUNCTION
 *  hide_main_menu_animation_frame
 * DESCRIPTION
 *  Hides the animating icon in a menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void hide_main_menu_animation_frame(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (MMI_main_menu_type)
    {
        case LIST_MENU:
            break;
        case MATRIX_MENU:
            gui_hide_fixed_matrix_menu_highlighted_item(&MMI_fixed_matrix_menu);
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  draw_main_menu_list_type_title
 * DESCRIPTION
 *  Redraws the category14 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void draw_main_menu_list_type_title(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_title_status_icon();
    draw_title();
}


/*****************************************************************************
 * FUNCTION
 *  category14_main_menu_list_highlight_handler
 * DESCRIPTION
 *  highlight handler for main menu lsit of category 14
 * PARAMETERS
 *  item_index      [IN]        Index of current highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void category14_main_menu_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
    if (MMI_main_menu_type == PAGE_MENU)
    {
        wgui_stop_transition_title_non_blocking();
        reset_title_transition();
    }
#ifndef TITLE_TRANSITION_WITH_PREVIOUS_TITLE_MOVEMENT
    set_previous_title_shortcut(MMI_menu_shortcut_number);
#endif 
#endif /* (TITLE_TRANSITION_STYLE_NON_BLOCKING) */ 
     
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    MMI_title_string = MMI_highlighted_item_text;
    gdi_layer_lock_frame_buffer();
    MMI_list_highlight_handler(item_index);
    if (!menu_shortcut_handler_display)
    {
        draw_main_menu_list_type_title();
    }
    gui_hide_animations();

#if (!defined(__MMI_UI_TECHNO_MAINMENU__))
    MMI_fixed_list_menu.item_display_function(
                            MMI_fixed_list_menu.items[MMI_fixed_list_menu.highlighted_item],
                            MMI_fixed_list_menu.common_item_data,
                            MMI_fixed_list_menu.x,
                            MMI_fixed_list_menu.y);
#endif /* (!defined(__MMI_UI_TECHNO_MAINMENU__)) */ 

    gdi_layer_unlock_frame_buffer();
#ifndef __MMI_UI_DALMATIAN_MAINMENU__
#ifdef __MMI_UI_TITLE_TRANSITION__
     
    if (MMI_main_menu_type == PAGE_MENU && is_mmi_title_transition_on_keys() == 0)
         
    {
        U16 keyCode, keyType;

        GetkeyInfo(&keyCode, &keyType);
    #ifdef __MMI_UI_MM_HORIZONTAL_PAGE_STYLE__
        if (keyCode == KEY_RIGHT_ARROW)
    #else 
        if (keyCode == KEY_DOWN_ARROW || keyCode == KEY_VOL_DOWN)
    #endif 
        {
            set_title_transition(WGUI_TRANSITION_L2R);
            draw_title();
             
        #if(!TITLE_TRANSITION_STYLE_NON_BLOCKING)
            reset_title_transition();
        #endif 
             
        }
        else
    #ifdef __MMI_UI_MM_HORIZONTAL_PAGE_STYLE__
        if (keyCode == KEY_LEFT_ARROW)
    #else 
        if (keyCode == KEY_UP_ARROW || keyCode == KEY_VOL_UP)
    #endif 
        {

            set_title_transition(WGUI_TRANSITION_R2L);
            draw_title();
             
        #if(!TITLE_TRANSITION_STYLE_NON_BLOCKING)
            reset_title_transition();
        #endif 
             
        }

    }
     
    else if (MMI_main_menu_type == PAGE_MENU && is_mmi_title_transition_on_keys() == 1)
    {
        draw_title();
    #if(!TITLE_TRANSITION_STYLE_NON_BLOCKING)
        reset_title_transition();
    #endif 
    }
     
#endif /* __MMI_UI_TITLE_TRANSITION__ */ 
#endif /* __MMI_UI_DALMATIAN_MAINMENU__ */ 
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  initialize_mainmenu_title_icons
 * DESCRIPTION
 *  Called once during bootup to initialize category screens
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void initialize_mainmenu_title_icons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = 0;
    UI_image_ID_type  mm_iconID[MAX_MAIN_MENU_ITEMS];

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
    U8 j = 0;
    U8 num_of_group = 0;
    U8 num_of_child = 0;
    U16 total_number = 0;
    U16 menu_id = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    num_of_group = (U8) GetNumOfChild(IDLE_SCREEN_MENU_ID); /* 101705 */
    for (i = 0; i < num_of_group; i++)
    {
        menu_id = GetSeqItemId(IDLE_SCREEN_MENU_ID, (U16) i);
        num_of_child = (U8) GetNumOfChild(menu_id); /* 101705 */
        GetSequenceImageIds(menu_id, mm_iconID);
        for (j = 0; j < num_of_child; j++)
        {
            MMI_ASSERT(total_number < MAX_MAIN_MENU_ITEMS);
            gMainMenuTitleIcon[total_number++] = MAIN_MENU_TITLE_ICON_BASE + mm_iconID[j] - MAIN_MENU_ICON_IMGBASE;
        }
    }
#else /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
    GetSequenceImageIds(IDLE_SCREEN_MENU_ID, mm_iconID);
    for (i = 0; i < MAX_MAIN_MENU_ITEMS; i++)
    {
    	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#if defined  __MMI_BLACK_BERRY_MAIN_MENU__	
		gMainMenuTitleIcon[i] = 0;
	#else
        gMainMenuTitleIcon[i] = MAIN_MENU_TITLE_ICON_BASE + mm_iconID[i] - MAIN_MENU_ICON_IMGBASE;
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */	
    }
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 
}

#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__


/*****************************************************************************
 * FUNCTION
 *  matrix_menu_full_background_show_softkey_background
 * DESCRIPTION
 *  Draws a background behind the softkeys for matrix_main_menu_no_titlebar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void matrix_menu_full_background_show_softkey_background(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gui_set_clip(0, UI_device_height - MMI_button_bar_height, UI_device_width - 1, UI_device_height - 1);
    gdi_image_draw_id(0, UI_device_height - MMI_button_bar_height, MAIN_MENU_MATRIX_SOFTKEY_BG);
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  matrix_menu_full_background_hide_left_softkey
 * DESCRIPTION
 *  Hides the left softkey for matrix_main_menu_no_titlebar
 *  
 *  Default hide left softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void matrix_menu_full_background_hide_left_softkey(void)
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
        MMI_softkeys[MMI_LEFT_SOFTKEY].y,
        MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width - 1,
        MMI_softkeys[MMI_LEFT_SOFTKEY].y + MMI_softkeys[MMI_LEFT_SOFTKEY].height - 1);
    gdi_image_draw_id(0, UI_device_height - MMI_button_bar_height, MAIN_MENU_MATRIX_SOFTKEY_BG);
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  matrix_menu_full_background_hide_right_softkey
 * DESCRIPTION
 *  Hides the right softkey for matrix_main_menu_no_titlebar
 *  
 *  Default hide left softkey function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void matrix_menu_full_background_hide_right_softkey(void)
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
        MMI_softkeys[MMI_RIGHT_SOFTKEY].y,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width - 1,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].y + MMI_softkeys[MMI_RIGHT_SOFTKEY].height - 1);
    gdi_image_draw_id(0, UI_device_height - MMI_button_bar_height, MAIN_MENU_MATRIX_SOFTKEY_BG);
    gdi_layer_pop_clip();
}
#endif /* __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__ */ 

/*****************************************************************************
 * FUNCTION
 *  ShowCategory14Screen
 * DESCRIPTION
 *  Displays the category14 screen . this is a scrren to show main menu
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items in the menu
 *  list_of_items           [IN]        Array of items
 *  list_of_icons           [IN]        Array of icons
 *  flags                   [IN]        (see explanation below)
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 *  LIST_MENU(?)            [IN]        
 *  Values(?)               [IN]        Allowed for these 8 bits are:
 *  MATRIX_MENU(?)          [IN]        
 *  The(?)                  [IN]        Last 8 bits of flags represent the type of the menu.
 * RETURNS
 *  void
 *****************************************************************************/
 /*+ zhouqin 2011-02-16 modify for q6 320X240 */
#ifdef __MMI_BLACK_BERRY_MAIN_MENU__
extern  U16 bbm_mainmenu_idx[];
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */

void ShowCategory14Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_items,
        U16 *list_of_icons,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i, shortcut_width;
    U8 h_flag;

#if(ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)
    S32 h_item;
#endif 
    S32 ixsize, iysize;
   mmi_trace(g_sw_GDI, "@@@@get_image(IMG_MAIN_MENU_BG) = 0x%x,IMG_MAIN_MENU_BG=%d", (U32)get_image(IMG_MAIN_MENU_BG), IMG_MAIN_MENU_BG);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_block_list_effect();
#endif 
    gdi_layer_lock_frame_buffer();
    UI_disable_alignment_timers();
    MMI_menu_shortcut_number = -1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    create_fixed_icontext_menuitems();
    MMI_main_menu_type = flags & 0xf;
    MMI_current_menu_type = flags & 0x0f;
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

#ifdef __MMI_UI_TECHNO_STATUS_ICON__
    MMI_title_x = MMI_TITLE_X + MMI_SIGNAL_WIDTH;
    MMI_title_width = UI_device_width - (2 * MMI_SIGNAL_WIDTH);
    MMI_title_height = 26;
    MMI_content_y = MMI_content_y - MMI_TITLE_HEIGHT + MMI_title_height;
    MMI_content_height = MMI_content_height + MMI_TITLE_HEIGHT - MMI_title_height;
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
    switch (MMI_main_menu_type)
    {
        case ANI_BG_LIST_MENU:
            MMI_fixed_list_menu.flags |= UI_LIST_MENU_LOOP;
            MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR;
            MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_BACKGROUND;
            MMI_fixed_list_menu.flags |= UI_LIST_MENU_CENTER_HIGHLIGHTED;
            MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_DISABLE_HIGHLIGHT_DISPLAY;
            MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY;
            resize_fixed_list(40, MMI_content_height + 2);
            associate_fixed_icontext_list();
            MMI_fixed_icontext_menuitem.flags &= ~UI_MENUITEM_MARQUEE_SCROLL;
            MMI_fixed_icontext_menuitem.flags &= ~UI_MENUITEM_TWO_DIRECTION_SCROLL;
            for (i = 0; i < number_of_items; i++)
            {
                add_fixed_icontext_item(get_string(list_of_items[i]), get_image(list_of_icons[i]));

            #ifdef __MMI_MERGE_SAT_TO_MAINMENU__
            #ifndef WAP_SUPPORT
            #ifndef __MMI_MULTI_SIM__
                if (IsSATPresent(0) && list_of_items[i] == MAIN_MENU_SERVICES_TEXT)
                {
                    MMI_fixed_icontext_menuitems[(*fixed_icontext_menu_n_items) - 1].item_text =
                        (UI_string_type) GetSATMainMenuAlphaID();
                }
            #endif /* __MMI_MULTI_SIM__ */ 
            #endif /* WAP_SUPPORT */ 
            #endif /* __MMI_MERGE_SAT_TO_MAINMENU__ */ 

            }
            register_fixed_list_shortcut_handler();
            register_fixed_list_keys();

            shortcut_width = set_menu_item_count(number_of_items) + 7;
            resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
            move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);

            resize_fixed_icontext_menuitems(0, MMI_content_height / 3);
            set_fixed_icontext_positions(MMI_ICONTEXT_MENUITEM_HEIGHT + 2, 0, 1, 0);
            move_fixed_list(0, (MMI_title_y + MMI_title_height));

            resize_fixed_list(40, MMI_content_height + 2);

            register_fixed_list_highlight_handler(standard_bg_ani_list_highlight_handler);
            MMI_current_menu_type = ANI_BG_LIST_MENU;
            h_flag = set_list_menu_category_history(MMI_CATEGORY14_LIST_ID, history_buffer);
            MMI_fixed_list_menu.flags &= ~UI_LIST_MENU_DISABLE_SCROLLBAR;
            if (h_flag)
            {
                fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
            }
            else
            {
                if (history_buffer != NULL && main_menu_index != -1)
                {
                    highlighted_item = main_menu_index;
                }
                fixed_list_goto_item_no_redraw(highlighted_item);
            }
            break;

        case LIST_MENU:
        #ifdef __MMI_UI_MM_LIST_NO_GRID__
            gui_disable_fixed_list_line_draw();
        #endif 

            resize_fixed_list(MMI_content_width, MMI_content_height );
            associate_fixed_icontext_list();
            set_MMI_list_main_menu_theme();
        // qiff del here for use default theme when mainmenu use style-list
        //    gui_set_fixed_list_menu_theme(&MMI_fixed_list_menu, current_UI_theme->fixed_list_menu_theme);
            gui_set_fixed_icontext_menuitem_theme(
                &MMI_fixed_icontext_menuitem,
                current_UI_theme->fixed_icontext_menuitem_theme);
            for (i = 0; i < number_of_items; i++)
            {
                add_fixed_icontext_item(get_string(list_of_items[i]), wgui_get_list_menu_icon(i, list_of_icons[i]));
            #ifdef __MMI_MERGE_SAT_TO_MAINMENU__
            #ifndef WAP_SUPPORT
            #ifndef __MMI_MULTI_SIM__
                if (IsSATPresent(0) && list_of_items[i] == MAIN_MENU_SERVICES_TEXT)
                {
                    MMI_fixed_icontext_menuitems[(*fixed_icontext_menu_n_items) - 1].item_text =
                        (UI_string_type) GetSATMainMenuAlphaID();
                }
            #endif /* __MMI_MULTI_SIM__ */ 
            #endif /* WAP_SUPPORT */ 
            #endif /* __MMI_MERGE_SAT_TO_MAINMENU__ */ 
            #ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
                gMainMenuTitleIcon[i] = MAIN_MENU_TITLE_ICON_BASE + list_of_icons[i] - MAIN_MENU_LIST_ICON_IMGBASE;
            #else 
                gMainMenuTitleIcon[i] = MAIN_MENU_TITLE_ICON_BASE + list_of_icons[i] - MAIN_MENU_ICON_IMGBASE;
            #endif 
            }
            register_fixed_list_shortcut_handler();
            register_fixed_list_keys();
            shortcut_width = set_menu_item_count(number_of_items) + 7;
            resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
            move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
            register_hide_menu_shortcut(draw_title);
            gui_measure_image(MMI_fixed_icontext_menuitems[0].item_icon, &ixsize, &iysize);
            set_fixed_icontext_positions(ixsize + 2, 0, 1, 0);
        #ifdef __MMI_MAINLCD_240X320__
            resize_fixed_icontext_menuitems(0, MMI_content_height / 6);
        #else /* __MMI_MAINLCD_240X320__ */ 
            resize_fixed_icontext_menuitems(0, MMI_content_height / 3);
        #endif /* __MMI_MAINLCD_240X320__ */ 
            title_bg_id = IMG_TITLEBAR_BG;

            register_fixed_list_highlight_handler(standard_list_highlight_handler);
            MMI_current_menu_type = LIST_MENU;
            resize_fixed_list_to_content_height();
            h_flag = set_list_menu_category_history(MMI_CATEGORY14_LIST_ID, history_buffer);
            MMI_fixed_list_menu.flags &= ~UI_LIST_MENU_DISABLE_SCROLLBAR;
            if (h_flag)
            {
                fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
            }
            else
            {
                if (history_buffer != NULL && main_menu_index != -1)
                {
                    highlighted_item = main_menu_index;
                }
                fixed_list_goto_item_no_redraw(highlighted_item);
            }
            resize_fixed_icontext_menuitems_to_list_width();
            dm_data.S32CatId = MMI_CATEGORY14_LIST_ID;
            break;

        case PAGE_MENU:
            /* Temporary:  Needs to be enhanced */
            set_MMI_list_main_menu_theme();
            gui_set_fixed_list_menu_theme(&MMI_fixed_list_menu, current_UI_theme->fixed_list_menu_theme);
            gui_set_fixed_icontext_menuitem_theme(
                &MMI_fixed_icontext_menuitem,
                current_UI_theme->fixed_icontext_menuitem_theme);

        #ifdef __MMI_UI_TECHNO_MAINMENU__
            create_techno_main_menu(number_of_items, list_of_icons);
        #endif 
             
        #if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
            enable_title_transition();
        #endif 
             
            resize_fixed_list(MMI_content_width, MMI_content_height);
            associate_fixed_icontext_list();
            MMI_fixed_icontext_menuitem.flags &= ~UI_MENUITEM_MARQUEE_SCROLL;
            MMI_fixed_icontext_menuitem.flags &= ~UI_MENUITEM_TWO_DIRECTION_SCROLL;
            MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_DISABLE_TEXT_CLIP;
            for (i = 0; i < number_of_items; i++)
            {
                add_fixed_icontext_item(get_string(list_of_items[i]), get_image(list_of_icons[i]));

            #ifdef __MMI_MERGE_SAT_TO_MAINMENU__
            #ifndef WAP_SUPPORT
            #ifndef __MMI_MULTI_SIM__
                if (IsSATPresent(0) && list_of_items[i] == MAIN_MENU_SERVICES_TEXT)
                {
                    MMI_fixed_icontext_menuitems[(*fixed_icontext_menu_n_items) - 1].item_text =
                        (UI_string_type) GetSATMainMenuAlphaID();
                }
            #endif /* __MMI_MULTI_SIM__ */ 
            #endif /* WAP_SUPPORT */ 
            #endif /* __MMI_MERGE_SAT_TO_MAINMENU__ */ 
            #ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
                gMainMenuTitleIcon[i] = MAIN_MENU_TITLE_ICON_BASE + list_of_icons[i] - MAIN_MENU_PAGE_ICON_IMGBASE;
            #else 
                gMainMenuTitleIcon[i] = MAIN_MENU_TITLE_ICON_BASE + list_of_icons[i] - MAIN_MENU_ICON_IMGBASE;
            #endif 
            }
            register_fixed_list_shortcut_handler();
#if 0 			//use up /down /volume keys to navigator
        #ifdef __MMI_UI_MM_HORIZONTAL_PAGE_STYLE__
            register_horizontal_fixed_list_keys();
        #else 
            register_fixed_list_keys();
        #endif 
#endif				
			 register_fixed_list_keys();

            shortcut_width = set_menu_item_count(number_of_items) + 7;
            resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
            move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
            register_hide_menu_shortcut(draw_main_menu_list_type_title);
            move_fixed_list(0, (MMI_title_y + MMI_title_height));
            gui_measure_image(get_image(list_of_icons[0]), &ixsize, &iysize);
            resize_fixed_list(MMI_content_width, MMI_content_height);
            register_fixed_list_highlight_handler(category14_main_menu_list_highlight_handler);
#if defined(__MMI_MAINLCD_128X128__)
            resize_fixed_icontext_menuitems(LCD_WIDTH,LCD_HEIGHT);
            set_fixed_icontext_positions(0, 0, 0, 0);
#else          
            resize_fixed_icontext_menuitems(MMI_content_width, MMI_content_height);
            set_fixed_icontext_positions(0, 0, 0, (MMI_content_height - iysize) >> 1);
#endif          
            MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_DISABLE_HIGHLIGHT | UI_MENUITEM_DISABLE_TEXT_DISPLAY;
            MMI_current_menu_type = PAGE_MENU;
            h_flag = set_list_menu_category_history(MMI_CATEGORY14_LIST_ID, history_buffer);
            MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR;
            if (h_flag)
            {
                fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
            }
            else
            {
                if (history_buffer != NULL && main_menu_index != -1)
                {
                    MMI_fixed_list_menu.highlighted_item = main_menu_index;
                    MMI_fixed_list_menu.first_displayed_item = main_menu_index;
                    MMI_fixed_list_menu.last_displayed_item = main_menu_index;
                    highlighted_item = main_menu_index;
                }
                fixed_list_goto_item_no_redraw(highlighted_item);
            }
        #if (!defined(__MMI_UI_TECHNO_MAINMENU__) && !defined(__MMI_UI_DALMATIAN_MAINMENU__))
            MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_FOCUSSED_ANIMATE;
        #endif 
            resize_fixed_icontext_menuitems_to_list_width();
            dm_data.S32CatId = MMI_CATEGORY14_LIST_ID;     
            break;

        case FIXED_GRID_MENU:
        #if(ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)

            h_flag = set_matrix_main_menu_category_history(MMI_CATEGORY14_MATRIX_ID, history_buffer);
            MMI_fixed_list_menu.flags &= ~UI_LIST_MENU_DISABLE_SCROLLBAR;
            if (h_flag)
            {
                h_item = _wgui_main_menu.highlighted_item;
            }
            else
            {
                h_item = highlighted_item;
            }
            MMI_disable_title_shortcut_display = 0;
            wgui_setup_main_menu(9, list_of_items, list_of_icons, h_item);
            wgui_register_main_menu_keys();
            wgui_register_main_menu_shortcut_handler();
            shortcut_width = set_menu_item_count(number_of_items) + 7;
            resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
            move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
            register_hide_menu_shortcut(draw_title);
            dm_data.S32CatId = MMI_CATEGORY14_MATRIX_ID;
            break;
        #endif /* (ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION) */ 
        case MATRIX_MENU:

            associate_fixed_icontext_matrix();
            for (i = 0; i < number_of_items; i++)
            {
                add_fixed_icontext_item(get_string(list_of_items[i]), get_image(list_of_icons[i]));
            #ifdef __MMI_MERGE_SAT_TO_MAINMENU__
            #ifndef WAP_SUPPORT
	     #ifndef __MMI_MULTI_SIM__
                if (IsSATPresent(0) && list_of_items[i] == MAIN_MENU_SERVICES_TEXT)
                {
                    MMI_fixed_icontext_menuitems[(*fixed_icontext_menu_n_items) - 1].item_text =
                        (UI_string_type) GetSATMainMenuAlphaID();
                }
            #endif		
            #endif /* WAP_SUPPORT */ 
            #endif /* __MMI_MERGE_SAT_TO_MAINMENU__ */ 

            #ifdef __MMI_MAINLCD_220X176__	
			gMainMenuAnimation[i] =MAIN_MENU_MATRIX_CALL_HISTORY_ICON_FOCUS +i;
            /*+ zhouqin 2011-02-16 modify for q6 320X240 */
            #elif defined (__MMI_BLACK_BERRY_MAIN_MENU__)		
			gMainMenuAnimation[i] = MAIN_MENU_ICON_ANI_01 + bbm_mainmenu_idx[i]-MAIN_MENU_ICON_01;
            /*- zhouqin 2011-02-16 modify for q6 320X240 */
            #else
                gMainMenuAnimation[i] = MAIN_MENU_ICON_ANI_IMGBASE + list_of_icons[i] - MAIN_MENU_ICON_IMGBASE;
            #endif		 
            #ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
                gMainMenuTitleIcon[i] = MAIN_MENU_TITLE_ICON_BASE + list_of_icons[i] - MAIN_MENU_MATRIX_ICON_IMGBASE;
	    /*+ zhouqin 2011-02-16 modify for q6 320X240 */	
            #elif defined  __MMI_BLACK_BERRY_MAIN_MENU__	
                gMainMenuTitleIcon[i] = 0;
	    /*- zhouqin 2011-02-16 modify for q6 320X240 */	 
	     #else	
                gMainMenuTitleIcon[i] = MAIN_MENU_TITLE_ICON_BASE + list_of_icons[i] - MAIN_MENU_ICON_IMGBASE;
            #endif 

            }
            register_fixed_matrix_shortcut_handler();
            MMI_fixed_matrix_menu.flags |= UI_MATRIX_MENU_LOOP | UI_MATRIX_MENU_FOR_MAINMENU;
        #if defined(__MMI_MAINMENU_MATRIX_LOOP__)
            {
                MMI_fixed_matrix_menu.flags |= UI_MATRIX_MENU_VERTICAL_LOOP;
            }
        #endif /* defined(__MMI_MAINMENU_MATRIX_LOOP__) */ 
            register_fixed_matrix_loop_keys();
            MMI_current_menu_type = MATRIX_MENU;

        #if(UI_MAIN_MENU_CATEGORY_TYPE == UI_MAIN_MENU_CATEGORY_TYPE1)

            register_fixed_matrix_highlight_handler(standard_animated_matrix_highlight_handler);
            move_fixed_matrix(0, (MMI_title_y + MMI_title_height) + 1);
            resize_fixed_matrix(MMI_content_width, MMI_content_height - MMI_matrix_bar_height);
            if (number_of_items > 9)
            {
                ixsize = (MMI_content_width - MMI_fixed_matrix_menu.vbar.width - 6) / 3;
            }
            else
            {
                ixsize = (MMI_content_width - 6) / 3;
            }
            iysize = (MMI_content_height - MMI_matrix_bar_height - 6) / 3;
            configure_fixed_matrix(ixsize, iysize, 3, 0);
            move_menu_shortcut_handler(MMI_matrix_bar_x, MMI_matrix_bar_y);
            shortcut_width = set_menu_item_count(number_of_items) + 7;
            resize_menu_shortcut_handler(shortcut_width, MMI_matrix_bar_height);
            register_hide_menu_shortcut(show_fixed_matrix_bar);
            set_fixed_icontext_positions(0, 0, 0, 0);
            MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY;

        #elif(UI_MAIN_MENU_CATEGORY_TYPE==UI_MAIN_MENU_CATEGORY_TYPE2)

            set_MMI_matrix_main_menu_theme();
            gui_set_fixed_matrix_menu_theme(&MMI_fixed_matrix_menu, current_UI_theme->fixed_matrix_menu_theme);
            gui_set_fixed_icontext_menuitem_theme(
                &MMI_fixed_icontext_menuitem,
                current_UI_theme->fixed_icontext_menuitem_theme);
            register_fixed_matrix_highlight_handler(standard_animated_matrix_highlight_handler_type2);
#ifdef __MMI_MAINLCD_220X176__
            /*No title.*/
            move_fixed_matrix(0, MMI_title_y);
            resize_fixed_matrix(MMI_content_width, MMI_content_height+MMI_title_height);
#else
            move_fixed_matrix(0, (MMI_title_y + MMI_title_height));
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
            resize_fixed_matrix(MMI_content_width, UI_device_height-MMI_TITLE_HEIGHT-MMI_title_height);
#else
            resize_fixed_matrix(MMI_content_width, MMI_content_height);
#endif
#endif

	#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
	if (number_of_items == 12) 
	{
		ixsize = 40;
		iysize = 30;
	}
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#elif defined  (__MMI_BLACK_BERRY_MAIN_MENU__)	
		if(number_of_items>12)
		{
			ixsize = 53;//(MMI_content_width - MMI_fixed_matrix_menu.vbar.width - 6) / 5;
			iysize =52;// (MMI_content_height - 6) / 3;
		}
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#else 		
            if (number_of_items > 12) //qiff for 3x4 grid mainmenu style
            {
                ixsize = (MMI_content_width - MMI_fixed_matrix_menu.vbar.width-6) / 3;
				iysize = (MMI_content_height  -6) / 4;
            }
#ifdef __MMI_MAINLCD_220X176__
            else if(number_of_items == 12)//4x3 grid mainmenu style
	     	{
				ixsize = 55;//(MMI_content_width - MMI_fixed_matrix_menu.vbar.width-6) / 4;
				iysize = 45;//(MMI_content_height  -6) / 3;
            }		
#endif			
            else
            {
                ixsize = (MMI_content_width - 6) / 3;
		  iysize = (MMI_content_height - 6) / 3;
				gui_measure_image(get_image(list_of_icons[0]), &ixsize, &iysize); 
            }
	#endif
            /* use image height as item height */
 //          gui_measure_image(get_image(list_of_icons[0]), &ixsize, &iysize);  //guojian deleted for jassmine 20080610 选中时无法盖住地图的问题
 #ifdef __MMI_SCREEN_ROTATE__ // wangmg[3.1]
			if(mmi_frm_is_screen_width_height_swapped())
				configure_fixed_matrix(ixsize, iysize, 4, 0);
			else
		#endif
			#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
				configure_fixed_matrix(ixsize, iysize, 4, 3);
			#else
#ifdef __MMI_MAINLCD_220X176__
           configure_fixed_matrix(ixsize, iysize, 4, 0);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#elif defined ( __MMI_BLACK_BERRY_MAIN_MENU__)
           configure_fixed_matrix(ixsize, iysize, 5, 0);
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#else
           configure_fixed_matrix(ixsize, iysize, 3, 0);
#endif
#endif
            shortcut_width = set_menu_item_count(number_of_items) + 7;
            resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
            move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
            register_hide_menu_shortcut(draw_title);
            set_fixed_icontext_positions(0, 0, 0, 0);
#ifndef __MMI_MAINLCD_220X176__
            MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY;
#else
    	     /*While the item highlights,it just shows the icon without text.*/
    	     MMI_fixed_icontext_menuitem.ext_flags |= UI_MENUITEM_SHOW_ICON_ONLY_ON_HIGHLIGHT;
    	     /*Show the text of item in the bottom. */
    	     MMI_fixed_icontext_menuitem.ext_flags |= UI_MENUITEM_EXT_BOTTOM_TEXT;
#endif
#if defined(__PROJECT_GALLITE_C01__)
            MMI_disable_title_shortcut_display = 1;
#else
            MMI_disable_title_shortcut_display = 0;
#endif
        #ifdef __MMI_WALLPAPER_ON_BOTTOM__ //082806 240x320 matrix
#if defined(__MMI_SCREEN_ROTATE__)
			if(mmi_frm_is_screen_width_height_swapped()){
				wgui_set_wallpaper_on_bottom(MMI_TRUE);
			}else{
#endif
  /*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if (defined __MMI_NOKIA_STYLE_N800__) ||(defined __MMI_MAINLCD_220X176__)||(defined __MMI_BLACK_BERRY_MAIN_MENU__)
  /*- zhouqin 2011-02-16 modify for q6 320X240 */
               title_bg_id = 0;
        #else
		 title_bg_id = MAIN_MENU_MATRIX_TITLE_BG;
            
        #endif
            wgui_set_wallpaper_on_bottom(MMI_TRUE);
	     TRACE_GDI_FUNCTION();
            dm_get_scr_bg_image(&mm_bg_ID, &mm_bg_filename, &mm_bg_x, &mm_bg_y, &mm_bg_opacity);
#if (defined __MMI_NOKIA_STYLE_N800__) ||(defined __MMI_MAINLCD_220X176__)
#else
            dm_set_scr_bg_image(IMG_MAIN_MENU_MATRIX_BG, NULL, -1, -1, 255);
#endif
			dm_get_scr_bg_flag(&mm_bg_flags);
            dm_set_scr_bg_flag(mm_bg_flags | DM_SCR_BG_FORCE_SHOW);
#if defined(__MMI_SCREEN_ROTATE__)
			}
#endif
        #endif 

        #elif defined(__MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__) && (UI_MAIN_MENU_CATEGORY_TYPE==UI_MAIN_MENU_CATEGORY_TYPE3)

            register_hide_left_softkey(matrix_menu_full_background_hide_left_softkey);
            register_hide_right_softkey(matrix_menu_full_background_hide_right_softkey);

            set_MMI_matrix_main_menu_theme();
            gui_set_fixed_matrix_menu_theme(&MMI_fixed_matrix_menu, current_UI_theme->fixed_matrix_menu_theme);
            gui_set_fixed_icontext_menuitem_theme(
                &MMI_fixed_icontext_menuitem,
                current_UI_theme->fixed_icontext_menuitem_theme);
            register_fixed_matrix_highlight_handler(standard_animated_matrix_highlight_handler_full_background);

        #ifdef __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__
            move_fixed_matrix(0, 0);
            resize_fixed_matrix(MMI_content_width, MMI_content_height + MMI_title_height);
        #else /* __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__ */ 
            move_fixed_matrix(0, (MMI_title_y + MMI_title_height));
            resize_fixed_matrix(MMI_content_width, MMI_content_height);
        #endif /* __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__ */ 

            if (number_of_items > 9)
            {
                ixsize = (MMI_content_width - MMI_fixed_matrix_menu.vbar.width - 6) / 3;
            }
            else
            {
                ixsize = (MMI_content_width - 6) / 3;
            }
            iysize = (MMI_content_height - 6) / 3;
            /* use image height as item height */
            gui_measure_image(get_image(list_of_icons[0]), &ixsize, &iysize);
            configure_fixed_matrix(ixsize, iysize, 3, 0);
            shortcut_width = set_menu_item_count(number_of_items) + 7;  /* keep for calculate the buffer size */

        #ifdef __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__
            disable_menu_shortcut_box_display = 1;  /* don't show shortcut box */
            MMI_softkeys[MMI_LEFT_SOFTKEY].clicked_down_text_color = MMI_softkeys[MMI_LEFT_SOFTKEY].normal_up_text_color = gui_color(255, 255, 50);     /* customizable */
            MMI_softkeys[MMI_RIGHT_SOFTKEY].clicked_down_text_color = MMI_softkeys[MMI_RIGHT_SOFTKEY].normal_up_text_color = gui_color(255, 255, 255);  /* customizable */
            inMatrixNoTitleFlag = TRUE;
        #else /* __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__ */ 
            resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
            move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
            register_hide_menu_shortcut(draw_title);
            disable_menu_shortcut_box_display = 0;  /* don't show shortcut box */
            inMatrixNoTitleFlag = FALSE;
        #endif /* __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__ */ 

            set_fixed_icontext_positions(0, 0, 0, 0);
            MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY;
            MMI_softkeys[MMI_LEFT_SOFTKEY].flags |= UI_BUTTON_DISABLE_BACKGND_DISPLAY;  /* don't clean each softkey becuase of cleaning all softkey background */
            MMI_softkeys[MMI_RIGHT_SOFTKEY].flags |= UI_BUTTON_DISABLE_BACKGND_DISPLAY; /* don't clean each softkey becuase of cleaning all softkey background */
        #endif 

            h_flag = set_matrix_menu_category_history(MMI_CATEGORY14_MATRIX_ID, history_buffer);
            if (h_flag)
        #ifdef __MMI_WGUI_MINI_TAB_BAR__
                if (wgui_if_mini_tab_bar_in_parent() == MMI_TRUE)
                {
                    S32 idx = (S32) wgui_mini_tab_bar_get_index();

                    fixed_matrix_goto_item_no_redraw(idx);
                }
                else
        #endif /* __MMI_WGUI_MINI_TAB_BAR__ */ 
                    fixed_matrix_goto_item_no_redraw(MMI_fixed_matrix_menu.highlighted_item);
            else
            {
                if (history_buffer != NULL && main_menu_index != -1)
                {
                    highlighted_item = main_menu_index;
                }
                fixed_matrix_goto_item_no_redraw(highlighted_item);
            }
#ifndef __MMI_USE_DRAW_FOCUS_IN_MAINMENU__
            MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_FOCUSSED_ANIMATE;
#endif
            SetKeyHandler(fixed_matrix_goto_previous_item, KEY_VOL_UP, KEY_EVENT_DOWN);
            SetKeyHandler(fixed_matrix_goto_next_item, KEY_VOL_DOWN, KEY_EVENT_DOWN);
            dm_data.S32CatId = MMI_CATEGORY14_MATRIX_ID;
            break;

        #if(ENABLE_CIRCULAR_3D_MENU)
        case CIRCULAR_3D_MENU:
            entry_full_screen();
            MMI_current_menu_type = CIRCULAR_3D_MENU;
            create_circular_3d_menu(number_of_items, list_of_items, list_of_icons);
            register_circular_3d_menu_keys();
            register_circular_3d_highlight_handler(standard_circular_3d_highlight_handler);
            register_3d_menu_shortcut_handler();
            shortcut_width = set_menu_item_count(number_of_items) + 7;
            resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
            move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
            register_hide_menu_shortcut(show_circular_3d_menu);
            h_flag = set_circular_menu_category_history(MMI_CATEGORY14_CIRCULAR_ID, history_buffer);
            if (h_flag)
            {
                circular_3d_menu_goto_item_no_redraw(wgui_CM3D_menu.highlighted_item);
            }
            else
            {
                if (history_buffer != NULL && main_menu_index != -1)
                {
                    highlighted_item = main_menu_index;
                }
                circular_3d_menu_goto_item_no_redraw(highlighted_item);
            }
            dm_data.S32CatId = MMI_CATEGORY14_CIRCULAR_ID;
            break;
        #endif /* (ENABLE_CIRCULAR_3D_MENU) */ 
        #if(ENABLE_ROTATE_MENU)
        case ROTATE_MENU:
            entry_full_screen();
            MMI_current_menu_type = ROTATE_MENU;
            highlighted_item = number_of_items - 1;
            create_circular_3d_menu(number_of_items, list_of_items, list_of_icons);
            register_rotate_menu_keys();
            register_circular_3d_highlight_handler(standard_circular_3d_highlight_handler);
            register_3d_menu_shortcut_handler();
            shortcut_width = set_menu_item_count(number_of_items) + 7;
            resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
        #ifdef ROTATE_VERT_LINE_RIGHT
            move_menu_shortcut_handler(0, MMI_title_y);
        #else 
            move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
        #endif 
            register_hide_menu_shortcut(show_circular_3d_menu);

            h_flag = set_circular_menu_category_history(MMI_CATEGORY14_ROTATE_ID, history_buffer);
            if (h_flag)
            {
                circular_3d_menu_goto_item_no_redraw(wgui_CM3D_menu.highlighted_item);
            }
            else
            {
                if (history_buffer != NULL && main_menu_index != -1)
                {
                    highlighted_item = main_menu_index - 1;
                }
                circular_3d_menu_goto_item_no_redraw(highlighted_item);
            }
            dm_data.S32CatId = MMI_CATEGORY14_ROTATE_ID;
            break;
        #endif /* (ENABLE_ROTATE_MENU) */ 
    }

#ifdef __MMI_UI_TECHNO_STATUS_ICON__
    leave_full_screen();
#endif 
    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory14Screen;
    UI_hide_animation_frame_function = hide_main_menu_animation_frame;
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory14History, GetCategory14HistorySize);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;

    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory14Screen
 * DESCRIPTION
 *  Exits the category14 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef __MMI_MATIXMENU_WITH_MOVING__
 extern void CSD_UI_exit_matrix_menu_init(void);
 #endif
void ExitCategory14Screen(void)
{
#if((ENABLE_CIRCULAR_3D_MENU) || (ENABLE_ROTATE_MENU))
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_main_menu_type == CIRCULAR_3D_MENU || MMI_main_menu_type == ROTATE_MENU)
    {
        stop_rotate_circular_menu();
    }
#endif /* ((ENABLE_CIRCULAR_3D_MENU) || (ENABLE_ROTATE_MENU)) */ 
    UI_enable_alignment_timers();
    ClearHighlightHandler();
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
    UI_hide_animation_frame_function = UI_dummy_function;
    gui_hide_animations();
#ifdef __MMI_UI_TECHNO_STATUS_ICON__
    MMI_title_x = MMI_TITLE_X;
    MMI_title_width = UI_device_width;
    MMI_title_height = MMI_TITLE_HEIGHT;
    MMI_content_y = MMI_CONTENT_Y;
    MMI_content_height = MMI_CONTENT_HEIGHT;
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
    switch (MMI_current_menu_type)
    {
        case ANI_BG_LIST_MENU:
            MMI_fixed_list_menu.flags &= ~UI_LIST_MENU_CENTER_HIGHLIGHTED;
            MMI_fixed_list_menu.flags &= ~UI_LIST_MENU_DISABLE_SCROLLBAR;
            MMI_fixed_list_menu.flags &= ~UI_LIST_MENU_DISABLE_BACKGROUND;
            set_MMI_list_sub_menu_theme();
            gui_set_fixed_list_menu_theme(&MMI_fixed_list_menu, current_UI_theme->fixed_list_menu_theme);
            gui_set_fixed_icontext_menuitem_theme(
                &MMI_fixed_icontext_menuitem,
                current_UI_theme->fixed_icontext_menuitem_theme);
            break;

        case LIST_MENU:
        case PAGE_MENU:
            /* disable lines grids on this category screen */
    #ifdef __MMI_UI_MM_LIST_NO_GRID__
            gui_enable_fixed_list_line_draw();
    #endif 

            MMI_fixed_icontext_menuitem.flags &= ~UI_MENUITEM_FOCUSSED_ANIMATE;
            set_MMI_list_sub_menu_theme();
            gui_set_fixed_list_menu_theme(&MMI_fixed_list_menu, current_UI_theme->fixed_list_menu_theme);
            gui_set_fixed_icontext_menuitem_theme(
                &MMI_fixed_icontext_menuitem,
                current_UI_theme->fixed_icontext_menuitem_theme);
    #ifdef __MMI_MAINLCD_240X320__
            title_bg_id = 0;
    #endif 
             
    #if(TITLE_TRANSITION_STYLE_NON_BLOCKING)
            disable_title_transition();
    #endif 
             
            break;

        case FIXED_GRID_MENU:
    #if(ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)
            wgui_close_main_menu();
            break;
    #endif /* (ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION) */ 
        case MATRIX_MENU:
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined(__MMI_BLACK_BERRY_MAIN_MENU__) || defined(__PROJECT_GALLITE_C01__)
	gdi_pop_and_restore_alpha_blending_source_layer();
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
		#ifdef __MMI_MATIXMENU_WITH_MOVING__
		CSD_UI_exit_matrix_menu_init();
		#endif
            MMI_fixed_icontext_menuitem.flags &= ~UI_MENUITEM_FOCUSSED_ANIMATE;
            MMI_fixed_matrix_menu.flags &= ~UI_MATRIX_MENU_FOR_MAINMENU;
    #if(UI_MAIN_MENU_CATEGORY_TYPE == UI_MAIN_MENU_CATEGORY_TYPE2)
            set_MMI_matrix_sub_menu_theme();
            gui_set_fixed_matrix_menu_theme(&MMI_fixed_matrix_menu, current_UI_theme->fixed_matrix_menu_theme);
            gui_set_fixed_icontext_menuitem_theme(
                &MMI_fixed_icontext_menuitem,
                current_UI_theme->fixed_icontext_menuitem_theme);
        #ifdef __MMI_WALLPAPER_ON_BOTTOM__//082806 240x320 matrix
            title_bg_id = 0;
            dm_set_scr_bg_flag(mm_bg_flags);
            //dm_set_scr_bg_image(mm_bg_ID, mm_bg_filename, mm_bg_x, mm_bg_y, mm_bg_opacity);
            dm_set_scr_bg_image_no_draw(mm_bg_ID, mm_bg_filename, mm_bg_x, mm_bg_y, mm_bg_opacity);
            wgui_reset_wallpaper_on_bottom();
        #endif 
    #elif defined(__MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__) && (UI_MAIN_MENU_CATEGORY_TYPE==UI_MAIN_MENU_CATEGORY_TYPE3)
            set_MMI_matrix_sub_menu_theme();
            gui_set_fixed_matrix_menu_theme(&MMI_fixed_matrix_menu, current_UI_theme->fixed_matrix_menu_theme);
            gui_set_fixed_icontext_menuitem_theme(
                &MMI_fixed_icontext_menuitem,
                current_UI_theme->fixed_icontext_menuitem_theme);
            disable_menu_shortcut_box_display = 0;
            MMI_softkeys[MMI_LEFT_SOFTKEY].flags &= ~UI_BUTTON_DISABLE_BACKGND_DISPLAY;
            MMI_softkeys[MMI_RIGHT_SOFTKEY].flags &= ~UI_BUTTON_DISABLE_BACKGND_DISPLAY;
            restore_softkeys();
            inMatrixNoTitleFlag = FALSE;
    #endif 
            break;
    }
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    reset_fixed_matrix();
#ifdef __MMI_UI_TECHNO_STATUS_ICON__
    arrange_status_icons();
    leave_full_screen();
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */ 
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory14HistorySize
 * DESCRIPTION
 *  Gets the history buffer size for the category14 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8s of the history buffer
 *****************************************************************************/
S32 GetCategory14HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (MMI_current_menu_type)
    {
        case ANI_BG_LIST_MENU:
        case LIST_MENU:
        case PAGE_MENU:
            return (sizeof(list_menu_category_history));
            break;

        case FIXED_GRID_MENU:
    #if(ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)
            return (sizeof(matrix_main_menu_category_history));
            break;
    #endif /* (ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION) */ 

        case MATRIX_MENU:
            return (sizeof(matrix_menu_category_history));
            break;
    #if(ENABLE_CIRCULAR_3D_MENU)
        case CIRCULAR_3D_MENU:
            return (sizeof(circular_menu_category_history));
            break;
    #endif /* (ENABLE_CIRCULAR_3D_MENU) */ 
    #if(ENABLE_ROTATE_MENU)
        case ROTATE_MENU:
            return (sizeof(circular_menu_category_history));
            break;
    #endif /* (ENABLE_ROTATE_MENU) */ 
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory14History
 * DESCRIPTION
 *  Gets the history buffer for the category14 screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory14History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (MMI_current_menu_type)
    {
        case ANI_BG_LIST_MENU:
        case LIST_MENU:
        case PAGE_MENU:
            get_list_menu_category_history(MMI_CATEGORY14_LIST_ID, history_buffer);
            break;

        case FIXED_GRID_MENU:
    #if(ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION)
            get_matrix_main_menu_category_history(MMI_CATEGORY14_MATRIX_ID, history_buffer);
            break;
    #endif /* (ENABLE_DIRECT_MAIN_MENU_IMPLEMENTATION) */ 
        case MATRIX_MENU:
            get_matrix_menu_category_history(MMI_CATEGORY14_MATRIX_ID, history_buffer);
            break;
    #if(ENABLE_CIRCULAR_3D_MENU)
        case CIRCULAR_3D_MENU:
            get_circular_menu_category_history(MMI_CATEGORY14_CIRCULAR_ID, history_buffer);
            break;
    #endif /* (ENABLE_CIRCULAR_3D_MENU) */ 
    #if(ENABLE_ROTATE_MENU)
        case ROTATE_MENU:
            get_circular_menu_category_history(MMI_CATEGORY14_ROTATE_ID, history_buffer);
            break;
    #endif /* (ENABLE_ROTATE_MENU) */ 
    }
    return (history_buffer);
}

/* for new list_matrix menu style */
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__

#if defined(__MMI_MAINLCD_240X320__)
#define MMI_BI_DEGREE_MAIN_MENU_LIST_WIDTH   65
#define MMI_BI_DEGREE_MAIN_MENU_COLUMN_COUNT 2
#define MMI_BI_DEGREE_MAIN_MENU_ROW_COUNT 2
#define MMI_BI_DEGREE_MAIN_MENU_ROW_RATIO 3
#else /* defined(__MMI_MAINLCD_240X320__) */ 
#define MMI_BI_DEGREE_MAIN_MENU_LIST_WIDTH   40
#define MMI_BI_DEGREE_MAIN_MENU_COLUMN_COUNT 2
#define MMI_BI_DEGREE_MAIN_MENU_ROW_COUNT 2
#define MMI_BI_DEGREE_MAIN_MENU_ROW_RATIO 2
#endif /* defined(__MMI_MAINLCD_240X320__) */ 
#define MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_HEIGHT 20
#define MMI_BI_DEGREE_MAIN_MENU_GAP    3
#define MMI_BI_DEGREE_MAIN_MENU_ICON_TEXT_GAP 3

extern void stop_fixed_icontext_menuitem_animation(S32 index);
extern void wgui_set_title_bknd_display(MMI_BOOL state);    /* 101705 */

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
extern void set_fixed_icontext_list_type2(void);
extern void create_fixed_icontext_list_menuitems2(S32 n_text_columns, S32 n_icon_columns);
extern void associate_fixed_icontext_list_list2(void);
extern void restore_fixed_icontext_list_type(void);
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 

extern gdi_handle wgui_layer_1;
extern UI_image_ID_type  idle_screen_wallpaper_ID;

MMI_BOOL back_from_history = MMI_FALSE;


/*****************************************************************************
 * FUNCTION
 *  Cat414MatrixHighlightHandler
 * DESCRIPTION
 *  highlight handler for main menu lsit of category 414 on bi degree main menu
 * PARAMETERS
 *  index       [IN]        Of current highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void Cat414MatrixHighlightHandler(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_matrix_highlight_handler(index);
    register_fixed_matrix_loop_keys();  /* for right arrow keys */
}


/*****************************************************************************
 * FUNCTION
 *  ShowCat414Matrix
 * DESCRIPTION
 *  Displays the category414 matrix menu
 * PARAMETERS
 *  number_of_items         [IN]        Number of items in the menu
 *  list_of_items           [IN]        Array of items
 *  list_of_icons           [IN]        Array of icons
 *  flags                   [IN]        
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCat414Matrix(
        S32 number_of_items,
        U16 *list_of_items,
        U16 *list_of_icons,
        S32 flags,
        S32 highlighted_item,
        matrix_menu_category_history *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    S32 symbol_width = 0, symbol_height = 0, width = 0, height = 0, str_width = 0, str_height = 0;
    S32 item_width = 0, item_height = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat414MatrixExitHandler = stop_fixed_icontext_menuitem_animation;
    gdi_layer_lock_frame_buffer();
    PowerAndEndKeyHandler();

    MMI_current_menu_type = LIST_MATRIX_MENU;
    create_fixed_icontext_menuitems();
    associate_fixed_icontext_matrix();

#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    enable_fixed_icontext_menuitem_transparent_effect();
    disable_fixed_icontext_menuitem_transparent_effect();
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 

    resize_fixed_matrix(
        UI_device_width - MMI_BI_DEGREE_MAIN_MENU_LIST_WIDTH,
        UI_device_height - MMI_title_height - MMI_button_bar_height - MMI_BI_DEGREE_MAIN_MENU_NAVI_BAR_HEIGHT);
    for (i = 0; i < number_of_items; i++)
    {
        PU8 img = (PU8) get_image(list_of_icons[i]);

        add_fixed_icontext_item((UI_string_type) get_string(list_of_items[i]), img);
        gui_measure_image(img, &symbol_width, &symbol_height);
        if (symbol_width > width)
        {
            width = symbol_width;
        }
        if (symbol_height > height)
        {
            height = symbol_height;
        }
    }
    gui_set_font(&MMI_default_font);
    gui_measure_string(get_string(list_of_items[0]), &str_width, &str_height);

    MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_FOCUSSED_ANIMATE | UI_MENUITEM_MARQUEE_SCROLL;
    MMI_fixed_matrix_menu.flags |=
        UI_MATRIX_MENU_VERTICAL_TRIGGER | UI_MATRIX_MENU_DISABLE_SCROLLBAR | UI_MATRIX_MENU_DISABLE_BACKGROUND | 
        UI_MATRIX_MENU_ALIGN_LEFT_TOP;
    register_fixed_matrix_loop_keys();
    register_fixed_matrix_highlight_handler(Cat414MatrixHighlightHandler);

    item_width =
        ((UI_device_width - MMI_BI_DEGREE_MAIN_MENU_LIST_WIDTH) / MMI_BI_DEGREE_MAIN_MENU_COLUMN_COUNT) -
        MMI_BI_DEGREE_MAIN_MENU_GAP;
    item_height = MMI_fixed_matrix_menu.height / MMI_BI_DEGREE_MAIN_MENU_ROW_RATIO;
    if ((item_height - height - str_height - MMI_BI_DEGREE_MAIN_MENU_ICON_TEXT_GAP) <= 1)       /* can't set iy=0; */
    {
        set_fixed_icontext_positions(1, item_height - str_height, 0, 1);
    }
    else
    {
        set_fixed_icontext_positions(
            1,
            item_height - str_height,
            0,
            (item_height - height - str_height - MMI_BI_DEGREE_MAIN_MENU_ICON_TEXT_GAP) >> 1);
    }
    configure_fixed_matrix(
        item_width,
        item_height,
        MMI_BI_DEGREE_MAIN_MENU_COLUMN_COUNT,
        MMI_BI_DEGREE_MAIN_MENU_ROW_COUNT);
    move_fixed_matrix(MMI_BI_DEGREE_MAIN_MENU_LIST_WIDTH, MMI_title_height);

    if (back_from_history == MMI_TRUE)
    {
        set_matrix_menu_category_history(MMI_CATEGORY414_ID, (U8*) history_buffer);
        fixed_matrix_goto_item_no_redraw(MMI_fixed_matrix_menu.highlighted_item);
        back_from_history = MMI_FALSE;
    }
    else
    {
        fixed_matrix_goto_item_no_redraw(highlighted_item);
    }

    MMI_disable_title_shortcut_display = 1;
    show_fixed_matrix();
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(
        MMI_fixed_matrix_menu.x,
        MMI_fixed_matrix_menu.y,
        MMI_fixed_matrix_menu.x + MMI_fixed_matrix_menu.width - 1,
        MMI_fixed_matrix_menu.y + MMI_fixed_matrix_menu.height);

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory414Screen
 * DESCRIPTION
 *  Exits the category414 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory414Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_enable_alignment_timers();
    ClearHighlightHandler();

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();   
#endif 

    UI_hide_animation_frame_function = UI_dummy_function;
    gui_hide_animations();

#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    enable_fixed_icontext_menuitem_transparent_effect();
#endif 

    MMI_fixed_icontext_menuitem.flags &= ~UI_MENUITEM_FOCUSSED_ANIMATE | UI_MENUITEM_MARQUEE_SCROLL;
    MMI_fixed_icontext_list_menuitem.flags &= ~UI_MENUITEM_DISABLE_TEXT_DISPLAY | UI_MENUITEM_HIGHLIGHT_SWITCH_ICON;
    MMI_fixed_matrix_menu.flags &= ~(UI_MATRIX_MENU_LOOP | UI_MATRIX_MENU_VERTICAL_TRIGGER | UI_MATRIX_MENU_ALIGN_LEFT_TOP);
    MMI_fixed_matrix_menu.flags &= ~UI_MATRIX_MENU_DISABLE_SCROLLBAR;
    MMI_fixed_matrix_menu.flags &= ~UI_MATRIX_MENU_DISABLE_BACKGROUND;
    MMI_fixed_matrix_menu.trigger_top_function = UI_dummy_function;
    MMI_fixed_matrix_menu.trigger_bottom_function = UI_dummy_function;
    gui_set_fixed_list_menu_theme(&MMI_fixed_list_menu, current_UI_theme->fixed_list_menu_theme);
    gui_set_fixed_icontext_list_menuitem_theme(
        &MMI_fixed_icontext_list_menuitem,
        current_UI_theme->fixed_icontext_list_menuitem_theme);
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    enable_softkey_background();
    gdi_layer_multi_layer_disable();
    wgui_set_title_bknd_display(MMI_TRUE);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory414HistorySize
 * DESCRIPTION
 *  Gets the history buffer size for the category414 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8s of the history buffer
 *****************************************************************************/
S32 GetCategory414HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return sizeof(list_matrix_menu_category_history);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory414History
 * DESCRIPTION
 *  Gets the history buffer for the category14 screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory414History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_matrix_menu_category_history(MMI_CATEGORY414_ID, history_buffer);
    return history_buffer;
}


/*****************************************************************************
 * FUNCTION
 *  cat14_list_highlight_handler
 * DESCRIPTION
 *  highlight handler for main menu lsit of category 14 on bi degree
 * PARAMETERS
 *  index       [IN]        Of current highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void cat14_list_highlight_handler(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (Cat414MatrixExitHandler != NULL)
    {
        Cat414MatrixExitHandler(MMI_fixed_matrix_menu.highlighted_item);
    }
    MMI_list_highlight_handler(index);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_list_matrix_menu_hide_lsk
 * DESCRIPTION
 *  left softkey hide function on bi degree
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_list_matrix_menu_hide_lsk(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(0, UI_device_height - MMI_button_bar_height, UI_device_width - 1, UI_device_height - 1);
    gdi_draw_solid_rect(
        MMI_softkeys[MMI_LEFT_SOFTKEY].x,
        UI_device_height - MMI_button_bar_height,
        MMI_softkeys[MMI_LEFT_SOFTKEY].x + MMI_softkeys[MMI_LEFT_SOFTKEY].width,
        UI_device_height - 1,
        GDI_COLOR_TRANSPARENT);
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_list_matrix_menu_hide_rsk
 * DESCRIPTION
 *  right softkey hide function on bi degree
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_list_matrix_menu_hide_rsk(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip(0, UI_device_height - MMI_button_bar_height, UI_device_width - 1, UI_device_height - 1);
    gdi_draw_solid_rect(
        MMI_softkeys[MMI_RIGHT_SOFTKEY].x,
        UI_device_height - MMI_button_bar_height,
        MMI_softkeys[MMI_RIGHT_SOFTKEY].x + MMI_softkeys[MMI_RIGHT_SOFTKEY].width,
        UI_device_height - 1,
        GDI_COLOR_TRANSPARENT);
    gdi_layer_pop_clip();
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  Cate414CategoryControlAreaPenDownHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate414CategoryControlAreaPenDownHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_general_pen_matrix_menu_hdlr(point, MMI_PEN_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  Cate414CategoryControlAreaPenUpHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate414CategoryControlAreaPenUpHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_general_pen_matrix_menu_hdlr(point, MMI_PEN_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  Cate414CategoryControlAreaPenAbortHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate414CategoryControlAreaPenAbortHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_general_pen_matrix_menu_hdlr(point, MMI_PEN_EVENT_ABORT);
}


/*****************************************************************************
 * FUNCTION
 *  Cate414CategoryControlAreaPenLongTapHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate414CategoryControlAreaPenLongTapHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_general_pen_matrix_menu_hdlr(point, MMI_PEN_EVENT_LONG_TAP);
}


/*****************************************************************************
 * FUNCTION
 *  Cate414CategoryControlAreaPenRepeatHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate414CategoryControlAreaPenRepeatHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_general_pen_matrix_menu_hdlr(point, MMI_PEN_EVENT_REPEAT);
}


/*****************************************************************************
 * FUNCTION
 *  Cate414CategoryControlAreaPenMoveHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cate414CategoryControlAreaPenMoveHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_general_pen_matrix_menu_hdlr(point, MMI_PEN_EVENT_MOVE);
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory414Screen
 * DESCRIPTION
 *  Displays the category414 screen . this is a scrren to show main menu
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon displayed with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Left softkey icon
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Right softkey icon
 *  number_of_items         [IN]        Number of items in the menu
 *  list_of_items           [IN]        Array of items
 *  list_of_icons           [IN]        Array of icons
 *  list_of_icons2          [IN]        Array of icons
 *  list_of_menuitem_id     [IN]        
 *  flags                   [IN]        
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory414Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_items,
        U16 *list_of_icons,
        U16 *list_of_icons2,
        U16 *list_of_menuitem_id,
        S32 flags,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 i;
    U8 h_flag;
    S32 ixsize, iysize;
    gdi_handle base_layer_handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_block_list_effect();
#endif 

    Cat414MatrixExitHandler = NULL;
    UI_disable_alignment_timers();
    MMI_menu_shortcut_number = -1;
    entry_full_screen();
    disable_softkey_background();
    wgui_set_title_bknd_display(MMI_FALSE);
    gdi_layer_get_base_handle(&base_layer_handle);
    gdi_layer_set_active(base_layer_handle);
    gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
    gdi_layer_clear(GDI_COLOR_TRANSPARENT);

    MMI_title_string = get_string(title);
    MMI_title_icon = get_image(title_icon);

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    register_hide_left_softkey(wgui_list_matrix_menu_hide_lsk);
    register_hide_right_softkey(wgui_list_matrix_menu_hide_rsk);
    create_fixed_icontext_menuitems();
    set_fixed_icontext_list_type2();
    create_fixed_icontext_list_menuitems2(1, 2);

    gui_set_fixed_list_menu_theme(&MMI_fixed_list_menu, current_UI_theme->fixed_list_menu_theme);
    gui_set_fixed_icontext_list_menuitem_theme(
        &MMI_fixed_icontext_list_menuitem,
        current_UI_theme->fixed_icontext_list_menuitem_theme);

    associate_fixed_icontext_list_list2();
    wgui_register_list_item_selected_callback_all(UI_dummy_function);

#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    disable_fixed_icontext_menuitem_transparent_effect();
#endif 

    gui_measure_image(get_image(list_of_icons2[0]), &ixsize, &iysize);
    set_fixed_icontext_list_text_coordinates(0, 0, 0, 0, 0);
    set_fixed_icontext_list_icon_coordinates(0, 0, 0, ixsize, iysize);
    set_fixed_icontext_list_icon_coordinates(1, 0, 0, ixsize, iysize);

    for (i = 0; i < number_of_items; i++)
    {
        fixed_icontext_list_item_insert(i);
        add_fixed_icontext_list_item_icon(i, 0, get_image(list_of_icons[i]));
        add_fixed_icontext_list_item_icon(i, 1, get_image(list_of_icons2[i]));
        add_fixed_icontext_list_item_text(i, 0, NULL);
    }

    register_fixed_list_highlight_handler(cat14_list_highlight_handler);

    resize_fixed_icontext_list_menuitems(ixsize, iysize);

    MMI_fixed_icontext_list_menuitem.flags |= UI_MENUITEM_DISABLE_TEXT_DISPLAY | UI_MENUITEM_HIGHLIGHT_SWITCH_ICON;
    MMI_current_menu_type = LIST_MENU;
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR | UI_LIST_MENU_DISABLE_BACKGROUND;
    MMI_fixed_matrix_menu.trigger_top_function = fixed_list_goto_previous_item;
    MMI_fixed_matrix_menu.trigger_bottom_function = fixed_list_goto_next_item;

    h_flag = set_list_matrix_menu_category_history(MMI_CATEGORY414_ID, history_buffer);
    if (h_flag)
    {
        back_from_history = MMI_TRUE;
    }
    else
    {
        back_from_history = MMI_FALSE;
    }

    gdi_layer_unlock_frame_buffer();
    restore_fixed_icontext_list_type();
    ExitCategoryFunction = ExitCategory414Screen;

    dm_add_image(get_image(IMG_MAIN_MENU_BG), NULL, NULL);
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_category_screen_control_area_pen_handlers(
        Cate414CategoryControlAreaPenDownHandler,
        MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(Cate414CategoryControlAreaPenUpHandler, MMI_PEN_EVENT_UP);
    wgui_register_category_screen_control_area_pen_handlers(
        Cate414CategoryControlAreaPenMoveHandler,
        MMI_PEN_EVENT_MOVE);
    wgui_register_category_screen_control_area_pen_handlers(
        Cate414CategoryControlAreaPenLongTapHandler,
        MMI_PEN_EVENT_LONG_TAP);
    wgui_register_category_screen_control_area_pen_handlers(
        Cate414CategoryControlAreaPenRepeatHandler,
        MMI_PEN_EVENT_REPEAT);
    wgui_register_category_screen_control_area_pen_handlers(
        Cate414CategoryControlAreaPenAbortHandler,
        MMI_PEN_EVENT_ABORT);
#endif /* __MMI_TOUCH_SCREEN__ */ 
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory414History, GetCategory414HistorySize);
    dm_register_category_controlled_callback(NULL);
    dm_data.S32CatId = MMI_CATEGORY414_ID;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}
#endif /* __MMI_BI_DEGREE_MAIN_MENU_STYLE__ */ 

