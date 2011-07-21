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
 *  gui_scrollbars.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  UI component - Scrollbar & Progress Indicator
 *  o Vertical scrollbar
 *  o Horizontal scrollbar 
 *  o Vertical Progress Indicator 
 *  o Horizontal Progress Indicator
 *  o Alternative Vertical Slider
 *  o Alternative Horizontal Slider
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#include "gui_scrollbars.h"
#include "wgui_categories_defs.h"
#include "gdi_include.h"
#include "debuginitdef.h"

/***************************************************************************** 
 * General Scrollbar Utility Functions
 *****************************************************************************/

/* 
 * Custom-draw function for scrollbar. 
 * We do not use image because scrollbar may have different width
 */

typedef enum
{
    GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL,
    GUI_SCROLLBAR_BUTTON_UP,
    GUI_SCROLLBAR_BUTTON_DOWN,
    GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL,
    GUI_SCROLLBAR_BUTTON_LEFT,
    GUI_SCROLLBAR_BUTTON_RIGHT
} gui_scrollbar_buttton_enum;

typedef enum
{
    GUI_SCROLLBAR_REGION_VERTICAL,
    GUI_SCROLLBAR_REGION_HORIZONTAL
} gui_scrollbar_region_enum;

#if defined(UI_SCROLLBAR_STYLE_3)

#define GUI_SCROLLBAR_MIN_HEIGHT_SCROLL_BUTTON  15

 
#define GUI_SCROLLBAR_MIN_HEIGHT_WITH_MARK_3 30
 
#define GUI_SCROLLBAR_MIN_HEIGHT_WITH_MARK_2 20
#define GUI_SCROLLBAR_MARK_WIDTH 6

/* Spacing of arrow and icon border */
#define GUI_SCROLLBAR_ARROW_GAP  3

/* 
 *Only GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL and GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL
 * use theme colors.
 */

#define GUI_SCROLLBAR_ARROW_SATURATION          200     /* large value */
#define GUI_SCROLLBAR_ARROW_LIGHTNESS           140     /* small value */

/* 
 * In current design, 176x220 use 14-pixel scrollbar. 240x320 use 16-pixel scrollbar 
 * There is only 10 graident colors inside 16-pixel scrollbar.
 * GUI_SCROLLBAR_NUM_GRADIENT_COLOR=16 is pretty enough for normal use.
 */
#define GUI_SCROLLBAR_NUM_GRADIENT_COLOR        16

#define GUI_SCROLLBAR_MAX_GRADIENT_SATURATION   230
#define GUI_SCROLLBAR_MIN_GRADIENT_SATURATION   160
#define GUI_SCROLLBAR_MAX_GRADIENT_LIGHTNESS    210
#define GUI_SCROLLBAR_MIN_GRADIENT_LIGHTNESS    160

#define GUI_SCROLLBAR_LIGHT_SATURATION          200     /* < GUI_SCROLLBAR_MAX_GRADIENT_SATURATION */
#define GUI_SCROLLBAR_LIGHT_LIGHTNESS           230     /* > GUI_SCROLLBAR_MAX_GRADIENT_LIGHTNESS */
#define GUI_SCROLLBAR_MEDIUM_SATURATION         180
#define GUI_SCROLLBAR_MEDIUM_LIGHTNESS          175
#define GUI_SCROLLBAR_DARK_SATURATION           150
#define GUI_SCROLLBAR_DARK_LIGHTNESS            145
#define GUI_SCROLLBAR_BORDER_SATURATION         250     /* large value */
#define GUI_SCROLLBAR_BORDER_LIGHTNESS          120     /* small value */

/* Use image to draw the scrollbar. */

/* Width of scrollbar */
static S32 g_mmi_gui_scrollbar_width = 0;

/* Height of up/down button, or width of left/right button */
static S32 g_mmi_gui_scrollbar_button_size;

/* The scrollbar background is composed by three different images. */
static S32 g_mmi_gui_scrollbar_bar_size1;
static S32 g_mmi_gui_scrollbar_bar_size2;
static S32 g_mmi_gui_scrollbar_bar_size3;

/* color of the arrow */
static S32 g_mmi_gui_scrollbar_arrow_cache_hue = -1;
static color_t g_mmi_gui_scrollbar_arrow_color;

/* color of the moveable part (scroll button) */
static S32 g_mmi_gui_scrollbar_cache_hue = -1;
static S32 g_mmi_gui_scrollbar_cache_num_gcolor = 0;

static color_t g_mmi_gui_scrollbar_border_color;
static color_t g_mmi_gui_scrollbar_dark_color;
static color_t g_mmi_gui_scrollbar_medium_color;
static color_t g_mmi_gui_scrollbar_light_color;
static color_t g_mmi_gui_scrollbar_gradient[GUI_SCROLLBAR_NUM_GRADIENT_COLOR];

/*
 * RGB <-> HLS color space conversion with cache to avoid floating point arithmetics
 */


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_RGB_to_HLS
 * DESCRIPTION
 *  
 * PARAMETERS
 *  rgb     [IN]        
 *  hls     [OUT]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_RGB_to_HLS(color_t rgb, UI_HLS_color *hls)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    static color_t cached_rgb = {0, 0, 0, 0};
    static UI_HLS_color cached_hls = {0, 0, 0};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (memcmp(&rgb, &cached_rgb, sizeof(color_t)) != 0)
    {
        cached_rgb = rgb;
        gui_RGB_to_HLS(rgb, &cached_hls);
    }
    *hls = cached_hls;
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_precache_image_size
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_precache_image_size(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!g_mmi_gui_scrollbar_width)
    {
        S32 iw, ih;

        gui_measure_image((PU8) GetImage(IMG_SCROLLBAR_BUTTON_UP), &iw, &ih);
        g_mmi_gui_scrollbar_width = iw;
        g_mmi_gui_scrollbar_button_size = ih;

        gui_measure_image((PU8) GetImage(IMG_SCROLLBAR_V_BAR_TOP), &iw, &ih);
        g_mmi_gui_scrollbar_bar_size1 = ih;

        gui_measure_image((PU8) GetImage(IMG_SCROLLBAR_V_BAR_MIDDLE), &iw, &ih);
        g_mmi_gui_scrollbar_bar_size2 = ih;

        gui_measure_image((PU8) GetImage(IMG_SCROLLBAR_V_BAR_BOTTOM), &iw, &ih);
        g_mmi_gui_scrollbar_bar_size3 = ih;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_precache_color1
 * DESCRIPTION
 *  Pre-cache color because the HLS colorspace conversion is slow
 * PARAMETERS
 *  theme_color     [IN]        Is the domination color the scrollbar
 *  width           [IN]        Scrollbar width
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_precache_color1(color_t theme_color, S32 width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_HLS_color hls;
    S32 i;
    S32 num_gradient_color = width - 4; /* two-pixel border each side */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (num_gradient_color > GUI_SCROLLBAR_NUM_GRADIENT_COLOR)
    {
        num_gradient_color = GUI_SCROLLBAR_NUM_GRADIENT_COLOR;
    }

    gui_scrollbar_RGB_to_HLS(theme_color, &hls);

    /* Recompute colors only if color hue is changed */
    if ((S32) hls.h != g_mmi_gui_scrollbar_cache_hue || num_gradient_color != g_mmi_gui_scrollbar_cache_num_gcolor)
    {
        g_mmi_gui_scrollbar_cache_num_gcolor = num_gradient_color;
        g_mmi_gui_scrollbar_cache_hue = hls.h;

        hls.s = GUI_SCROLLBAR_BORDER_SATURATION;
        hls.l = GUI_SCROLLBAR_BORDER_LIGHTNESS;
        gui_HLS_to_RGB(hls, &g_mmi_gui_scrollbar_border_color);

        hls.s = GUI_SCROLLBAR_DARK_SATURATION;
        hls.l = GUI_SCROLLBAR_DARK_LIGHTNESS;
        gui_HLS_to_RGB(hls, &g_mmi_gui_scrollbar_dark_color);

        hls.s = GUI_SCROLLBAR_MEDIUM_SATURATION;
        hls.l = GUI_SCROLLBAR_MEDIUM_LIGHTNESS;
        gui_HLS_to_RGB(hls, &g_mmi_gui_scrollbar_medium_color);

        hls.s = GUI_SCROLLBAR_LIGHT_SATURATION;
        hls.l = GUI_SCROLLBAR_LIGHT_LIGHTNESS;
        gui_HLS_to_RGB(hls, &g_mmi_gui_scrollbar_light_color);

        for (i = 0; i < num_gradient_color; i++)
        {
            S32 s, l;

            s = i * (GUI_SCROLLBAR_MAX_GRADIENT_SATURATION -
                     GUI_SCROLLBAR_MIN_GRADIENT_SATURATION) / (num_gradient_color - 1);
            s = GUI_SCROLLBAR_MAX_GRADIENT_SATURATION - s;
            l = i * (GUI_SCROLLBAR_MAX_GRADIENT_LIGHTNESS -
                     GUI_SCROLLBAR_MIN_GRADIENT_LIGHTNESS) / (num_gradient_color - 1);
            l = GUI_SCROLLBAR_MAX_GRADIENT_LIGHTNESS - l;
            MMI_DBG_ASSERT(s >= 0 && s <= 255);
            MMI_DBG_ASSERT(l >= 0 && l <= 255);
            hls.s = (U8) s;
            hls.l = (U8) l;
            gui_HLS_to_RGB(hls, &g_mmi_gui_scrollbar_gradient[i]);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_precache_color2
 * DESCRIPTION
 *  Pre-cache color because the HLS colorspace conversion is slow
 * PARAMETERS
 *  theme_color     [IN]        Is the domination color the scrollbar
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_precache_color2(color_t theme_color)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_HLS_color hls;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_scrollbar_RGB_to_HLS(theme_color, &hls);

    /* Recompute colors only if color hue is changed */
    if ((S32) hls.h != g_mmi_gui_scrollbar_arrow_cache_hue)
    {
        g_mmi_gui_scrollbar_arrow_cache_hue = (S32) hls.h;

        hls.s = GUI_SCROLLBAR_ARROW_SATURATION;
        hls.l = GUI_SCROLLBAR_ARROW_LIGHTNESS;
        gui_HLS_to_RGB(hls, &g_mmi_gui_scrollbar_arrow_color);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_get_minimum_size
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 gui_scrollbar_get_minimum_size(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 min;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_scrollbar_precache_image_size();
    min = g_mmi_gui_scrollbar_bar_size1 + g_mmi_gui_scrollbar_bar_size3;

    if (min < GUI_SCROLLBAR_MIN_HEIGHT_SCROLL_BUTTON)
    {
        min = GUI_SCROLLBAR_MIN_HEIGHT_SCROLL_BUTTON;
    }

    return min + ((g_mmi_gui_scrollbar_button_size + 2 /* 1+ 1 pixel spacing */ ) << 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_get_button_min_size
 * DESCRIPTION
 *  
 * PARAMETERS
 *  type            [IN]        
 *  bar_width       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static S32 gui_scrollbar_get_button_min_size(gui_scrollbar_buttton_enum type, S32 bar_width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 ret = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_scrollbar_precache_image_size();

    MMI_DBG_ASSERT(g_mmi_gui_scrollbar_width == bar_width);

    switch (type)
    {
        case GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL:
            ret = GUI_SCROLLBAR_MIN_HEIGHT_SCROLL_BUTTON;
            break;

        case GUI_SCROLLBAR_BUTTON_UP:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_DOWN:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_LEFT:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_RIGHT:
            ret = g_mmi_gui_scrollbar_button_size + 2 /* 1 + 1 pixel spacing */ ;
            break;

        default:
            MMI_ASSERT(0);
    }

    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_draw_region
 * DESCRIPTION
 *  
 * PARAMETERS
 *  type        [IN]        
 *  x           [IN]        
 *  y           [IN]        
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_draw_region(gui_scrollbar_region_enum type, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(x, y, x + width - 1, y + height - 1);

    if (type == GUI_SCROLLBAR_REGION_VERTICAL)
    {
        if (height <= g_mmi_gui_scrollbar_bar_size1 + g_mmi_gui_scrollbar_bar_size3)
        {
            gui_set_clip(x, y, x + width - 1, y + (height >> 1) - 1);
            gui_show_image(x, y, (PU8) GetImage(IMG_SCROLLBAR_V_BAR_TOP));
            gui_set_clip(x, y + (height >> 1), x + width - 1, y + height - 1);
            gui_show_image(x, y + height - g_mmi_gui_scrollbar_bar_size3, (PU8) GetImage(IMG_SCROLLBAR_V_BAR_BOTTOM));
        }
        else
        {
            S32 y1, y2, remain;

            y1 = y + g_mmi_gui_scrollbar_bar_size1;
            y2 = y + height - g_mmi_gui_scrollbar_bar_size3 - 1;
            gui_show_image(x, y, (PU8) GetImage(IMG_SCROLLBAR_V_BAR_TOP));
            gui_show_image(x, y2 + 1, (PU8) GetImage(IMG_SCROLLBAR_V_BAR_BOTTOM));
            gui_set_clip(x, y1, x + width - 1, y2);
            remain = y2 - y1 + 1;
            while (remain > 0)
            {
                gui_show_image(x, y1, (PU8) GetImage(IMG_SCROLLBAR_V_BAR_MIDDLE));
                y1 += g_mmi_gui_scrollbar_bar_size2;
                remain -= g_mmi_gui_scrollbar_bar_size2;
            }
        }
    }
    else
    {
        if (width <= g_mmi_gui_scrollbar_bar_size1 + g_mmi_gui_scrollbar_bar_size3)
        {
            gui_set_clip(x, y, x + (width >> 1) - 1, y + height - 1);
            gui_show_image(x, y, (PU8) GetImage(IMG_SCROLLBAR_H_BAR_LEFT));
            gui_set_clip(x + (width >> 1), y, x + width - 1, y + height - 1);
            gui_show_image(x + width - g_mmi_gui_scrollbar_bar_size3, y, (PU8) GetImage(IMG_SCROLLBAR_H_BAR_RIGHT));
        }
        else
        {
            S32 x1, x2, remain;

            x1 = x + g_mmi_gui_scrollbar_bar_size1;
            x2 = x + width - g_mmi_gui_scrollbar_bar_size3 - 1;
            gui_show_image(x, y, (PU8) GetImage(IMG_SCROLLBAR_H_BAR_LEFT));
            gui_show_image(x2 + 1, y, (PU8) GetImage(IMG_SCROLLBAR_H_BAR_RIGHT));
            gui_set_clip(x, y, x2, y + height - 1);
            remain = x2 - x1 + 1;
            while (remain > 0)
            {
                gui_show_image(x1, y, (PU8) GetImage(IMG_SCROLLBAR_H_BAR_MIDDLE));
                x1 += g_mmi_gui_scrollbar_bar_size2;
                remain -= g_mmi_gui_scrollbar_bar_size2;
            }
        }
    }

    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_draw_button
 * DESCRIPTION
 *  
 * PARAMETERS
 *  type        [IN]        
 *  b           [IN/OUT]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_draw_button(gui_scrollbar_buttton_enum type, icon_button *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c;
    S32 x, y, width, height;
    BOOL is_button_pressed = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_scrollbar_precache_image_size();

    c = b->normal_up_filler->c;
    x = b->x;
    y = b->y;
    width = b->width;
    height = b->height;

    if (width == 0 || height == 0)
    {
        return;
    }

    if ((b->flags & UI_BUTTON_SHIFT_BUTTON1) || (b->flags & UI_BUTTON_SHIFT_BUTTON2))
    {
        is_button_pressed = MMI_TRUE;
    }

    gui_push_clip();
    gui_set_clip(x, y, x + width - 1, y + height - 1);

    /* shift the position to keep one-pixel space */
    if (type == GUI_SCROLLBAR_BUTTON_LEFT || type == GUI_SCROLLBAR_BUTTON_RIGHT)
    {
        x++;
    }
    if (type == GUI_SCROLLBAR_BUTTON_UP || type == GUI_SCROLLBAR_BUTTON_DOWN)
    {
        y++;
    }

    switch (type)
    {
        case GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL:
        {
            S32 x2, y2, i, cnt;

            gui_scrollbar_precache_color1(c, width);

            if (type == GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL)
            {
                /* 1 pixel spacing in the top and bottom side */
                height--;
                y++;
            }
            else
            {
                /* 1 pixel spacing in the left and right side */
                width--;
                x++;
            }

            x2 = x + width - 1;
            y2 = y + height - 1;

            /* External border */
            gui_draw_horizontal_line(x + 2, x2 - 2, y, g_mmi_gui_scrollbar_border_color);
            gui_draw_horizontal_line(x + 2, x2 - 2, y2, g_mmi_gui_scrollbar_border_color);
            gui_draw_vertical_line(y + 2, y2 - 2, x, g_mmi_gui_scrollbar_border_color);
            gui_draw_vertical_line(y + 2, y2 - 2, x2, g_mmi_gui_scrollbar_border_color);
            gui_putpixel(x + 1, y + 1, g_mmi_gui_scrollbar_border_color);
            gui_putpixel(x + 1, y2 - 1, g_mmi_gui_scrollbar_border_color);
            gui_putpixel(x2 - 1, y + 1, g_mmi_gui_scrollbar_border_color);
            gui_putpixel(x2 - 1, y2 - 1, g_mmi_gui_scrollbar_border_color);
            /* Light border */
            gui_draw_vertical_line(y + 2, y2 - 2, x + 1, g_mmi_gui_scrollbar_light_color);
            gui_draw_horizontal_line(x + 2, x2 - 2, y + 1, g_mmi_gui_scrollbar_light_color);

            /* Gradient color fill */
            if (type == GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL)
            {
                for (i = y + 2, cnt = 0; i <= y2 - 2; i++, cnt++)
                {
                    if (cnt >= GUI_SCROLLBAR_NUM_GRADIENT_COLOR)
                    {
                        cnt = GUI_SCROLLBAR_NUM_GRADIENT_COLOR - 1;
                    }
                    gui_draw_horizontal_line(x + 2, x2 - 2, i, g_mmi_gui_scrollbar_gradient[cnt]);
                }

                /* Dark border */
                gui_draw_vertical_line(y + 2, y2 - 2, x2 - 1, g_mmi_gui_scrollbar_medium_color);
                gui_draw_horizontal_line(x + 2, x2 - 2, y2 - 1, g_mmi_gui_scrollbar_dark_color);
            }
            else
            {
                for (i = x + 2, cnt = 0; i <= x2 - 2; i++, cnt++)
                {
                    if (cnt >= GUI_SCROLLBAR_NUM_GRADIENT_COLOR)
                    {
                        cnt = GUI_SCROLLBAR_NUM_GRADIENT_COLOR - 1;
                    }
                    gui_draw_vertical_line(y + 2, y2 - 2, i, g_mmi_gui_scrollbar_gradient[cnt]);
                }

                /* Dark border */
                gui_draw_vertical_line(y + 2, y2 - 2, x2 - 1, g_mmi_gui_scrollbar_dark_color);
                gui_draw_horizontal_line(x + 2, x2 - 2, y2 - 1, g_mmi_gui_scrollbar_medium_color);
            }

             
            if (type == GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL)
            {
                S32 mark_y1, mark_y2, mark_x;

                mark_y1 = y + ((height - GUI_SCROLLBAR_MARK_WIDTH) >> 1);
                mark_y2 = mark_y1 + GUI_SCROLLBAR_MARK_WIDTH - 1;

                if (width > GUI_SCROLLBAR_MIN_HEIGHT_WITH_MARK_3)
                {
                    mark_x = x + (width >> 1) - 2;
                    for (i = 0; i < 3; i++)
                    {
                        gui_draw_vertical_line(mark_y1, mark_y2, mark_x++, g_mmi_gui_scrollbar_light_color);
                        gui_draw_vertical_line(mark_y1 + 1, mark_y2 + 1, mark_x++, g_mmi_gui_scrollbar_dark_color);
                    }
                }
                else if (width > GUI_SCROLLBAR_MIN_HEIGHT_WITH_MARK_2)
                {
                    mark_x = x + (width >> 1) - 1;
                    for (i = 0; i < 2; i++)
                    {
                        gui_draw_vertical_line(mark_y1, mark_y2, mark_x++, g_mmi_gui_scrollbar_light_color);
                        gui_draw_vertical_line(mark_y1 + 1, mark_y2 + 1, mark_x++, g_mmi_gui_scrollbar_dark_color);
                    }
                }
            }
            else
            {
                S32 mark_x1, mark_x2, mark_y;

                mark_x1 = x + ((width - GUI_SCROLLBAR_MARK_WIDTH) >> 1);
                mark_x2 = mark_x1 + GUI_SCROLLBAR_MARK_WIDTH - 1;

                if (height > GUI_SCROLLBAR_MIN_HEIGHT_WITH_MARK_3)
                {
                    mark_y = y + (height >> 1) - 2;
                    for (i = 0; i < 3; i++)
                    {
                        gui_draw_horizontal_line(mark_x1, mark_x2, mark_y++, g_mmi_gui_scrollbar_light_color);
                        gui_draw_horizontal_line(mark_x1 + 1, mark_x2 + 1, mark_y++, g_mmi_gui_scrollbar_dark_color);
                    }
                }
                else if (height > GUI_SCROLLBAR_MIN_HEIGHT_WITH_MARK_2)
                {
                    mark_y = y + (height >> 1) - 1;
                    for (i = 0; i < 2; i++)
                    {
                        gui_draw_horizontal_line(mark_x1, mark_x2, mark_y++, g_mmi_gui_scrollbar_light_color);
                        gui_draw_horizontal_line(mark_x1 + 1, mark_x2 + 1, mark_y++, g_mmi_gui_scrollbar_dark_color);
                    }
                }
            }

            break;
        }

        case GUI_SCROLLBAR_BUTTON_UP:
        case GUI_SCROLLBAR_BUTTON_LEFT:
        case GUI_SCROLLBAR_BUTTON_DOWN:
        case GUI_SCROLLBAR_BUTTON_RIGHT:
        {
            gui_scrollbar_precache_color2(c);

            if (is_button_pressed)
            {
                gui_show_image(x, y, (PU8) GetImage(IMG_SCROLLBAR_BUTTON_DOWN));
            }
            else
            {
                gui_show_image(x, y, (PU8) GetImage(IMG_SCROLLBAR_BUTTON_UP));
            }

            if (is_button_pressed)
            {
                y++;
            }

            if (type == GUI_SCROLLBAR_BUTTON_UP)
            {
                S32 arrow_width = width - (GUI_SCROLLBAR_ARROW_GAP << 1);
                S32 arrow_height;
                S32 x1, y1, i;

                arrow_width -= (arrow_width % 2);
                arrow_height = arrow_width >> 1;

                x1 = (width - arrow_width);
                if (x1 > 0)
                {
                    x1 >>= 1;
                }
                else
                {
                    x1 = 0;
                }
                x1 += x;

                y1 = (height - arrow_height);
                if (y1 > 0)
                {
                    y1 >>= 1;
                }
                else
                {
                    y1 = 0;
                }
                y1 += y + arrow_height - 1 - 1; /* -1 for center */

                for (i = 0; i < arrow_height; i++)
                {
                    gui_putpixel(x1, y1, g_mmi_gui_scrollbar_arrow_color);
                    gui_putpixel(x1 + 1, y1, g_mmi_gui_scrollbar_arrow_color);
                    y1--;
                    x1++;
                }
                y1 += 2;
                for (i = 1; i < arrow_height; i++)
                {
                    gui_putpixel(x1, y1, g_mmi_gui_scrollbar_arrow_color);
                    gui_putpixel(x1 + 1, y1, g_mmi_gui_scrollbar_arrow_color);
                    y1++;
                    x1++;
                }
            }
            else if (type == GUI_SCROLLBAR_BUTTON_DOWN)
            {
                S32 arrow_width = width - (GUI_SCROLLBAR_ARROW_GAP << 1);
                S32 arrow_height;
                S32 x1, y1, i;

                arrow_width -= (arrow_width % 2);
                arrow_height = arrow_width >> 1;

                x1 = (width - arrow_width);
                if (x1 > 0)
                {
                    x1 >>= 1;
                }
                else
                {
                    x1 = 0;
                }
                x1 += x;

                y1 = (height - arrow_height);
                if (y1 > 0)
                {
                    y1 >>= 1;
                }
                else
                {
                    y1 = 0;
                }
                y1 += y;

                for (i = 0; i < arrow_height; i++)
                {
                    gui_putpixel(x1, y1, g_mmi_gui_scrollbar_arrow_color);
                    gui_putpixel(x1 + 1, y1, g_mmi_gui_scrollbar_arrow_color);
                    y1++;
                    x1++;
                }
                y1 -= 2;
                for (i = 1; i < arrow_height; i++)
                {
                    gui_putpixel(x1, y1, g_mmi_gui_scrollbar_arrow_color);
                    gui_putpixel(x1 + 1, y1, g_mmi_gui_scrollbar_arrow_color);
                    y1--;
                    x1++;
                }
            }
            else if (type == GUI_SCROLLBAR_BUTTON_LEFT)
            {
                S32 arrow_height = height - (GUI_SCROLLBAR_ARROW_GAP << 1);
                S32 arrow_width;
                S32 x1, y1, i;

                arrow_height -= (arrow_height % 2);
                arrow_width = arrow_height >> 1;

                y1 = (height - arrow_height);
                if (y1 > 0)
                {
                    y1 >>= 1;
                }
                else
                {
                    y1 = 0;
                }
                y1 += y;

                x1 = (width - arrow_width);
                if (x1 > 0)
                {
                    x1 >>= 1;
                }
                else
                {
                    x1 = 0;
                }
                x1 += x + arrow_width - 1 - 1;  /* -1 for center */

                for (i = 0; i < arrow_width; i++)
                {
                    gui_putpixel(x1, y1, g_mmi_gui_scrollbar_arrow_color);
                    gui_putpixel(x1, y1 + 1, g_mmi_gui_scrollbar_arrow_color);
                    y1++;
                    x1--;
                }
                x1 += 2;
                for (i = 1; i < arrow_width; i++)
                {
                    gui_putpixel(x1, y1, g_mmi_gui_scrollbar_arrow_color);
                    gui_putpixel(x1, y1 + 1, g_mmi_gui_scrollbar_arrow_color);
                    y1++;
                    x1++;
                }
            }
            else
            {
                S32 arrow_height = height - (GUI_SCROLLBAR_ARROW_GAP << 1);
                S32 arrow_width;
                S32 x1, y1, i;

                arrow_height -= (arrow_height % 2);
                arrow_width = arrow_height >> 1;

                y1 = (height - arrow_height);
                if (y1 > 0)
                {
                    y1 >>= 1;
                }
                else
                {
                    y1 = 0;
                }
                y1 += y;

                x1 = (width - arrow_width);
                if (x1 > 0)
                {
                    x1 >>= 1;
                }
                else
                {
                    x1 = 0;
                }
                x1 += x;

                for (i = 0; i < arrow_width; i++)
                {
                    gui_putpixel(x1, y1, g_mmi_gui_scrollbar_arrow_color);
                    gui_putpixel(x1, y1 + 1, g_mmi_gui_scrollbar_arrow_color);
                    y1++;
                    x1++;
                }
                x1 -= 2;
                for (i = 1; i < arrow_width; i++)
                {
                    gui_putpixel(x1, y1, g_mmi_gui_scrollbar_arrow_color);
                    gui_putpixel(x1, y1 + 1, g_mmi_gui_scrollbar_arrow_color);
                    y1++;
                    x1--;
                }
            }

            break;
        }

        default:
            MMI_DBG_ASSERT(0);
    }

    gui_pop_clip();

}

#elif defined(UI_SCROLLBAR_STYLE_2)

/* 
 * We do not use gui_draw_filled_area and UI_FILLED_AREA_TYPE_CUSTOM_FILL_TYPE#
 * because up/down/left/right and horizontal/vertical scroll has up and down states, and 
 * we need to define lots of filler types.
 */


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_get_button_min_size
 * DESCRIPTION
 *  
 * PARAMETERS
 *  type            [IN]        
 *  bar_width       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static S32 gui_scrollbar_get_button_min_size(gui_scrollbar_buttton_enum type, S32 bar_width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 ret = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (type)
    {
        case GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL:
            /* "Expected" minimum size. However, the size of button may be larger. */
            ret = 12;
            break;

        case GUI_SCROLLBAR_BUTTON_UP:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_DOWN:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_LEFT:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_RIGHT:
            if (bar_width < 5)
            {
                /* Do not display these button at all */
                ret = 0;
            }
            else if ((bar_width % 2) == 0)
            {
                ret = bar_width;
            }
            else
            {
                /* height of button = width - 1.  But we need leave two pixels for room between scroll button */
                ret = bar_width + 1;
            }
            break;

        default:
            MMI_ASSERT(0);
    }

    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_adjust_luminance
 * DESCRIPTION
 *  
 * PARAMETERS
 *  luminance       [IN/OUT]         
 *  diff            [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_adjust_luminance(U8 *luminance, S16 diff)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 l = *luminance;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    l += diff;
    if (l > 255)
    {
        l = 255;
    }
    if (l < 0)
    {
        l = 0;
    }
    *luminance = (U8) l;
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_draw_button
 * DESCRIPTION
 *  
 * PARAMETERS
 *  type        [IN]        
 *  b           [IN/OUT]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_draw_button(gui_scrollbar_buttton_enum type, icon_button *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_HLS_color hls;
    color_t c;
    S32 x, y, width, height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x = b->x;
    y = b->y;
    width = b->width;
    height = b->height;
    c = b->normal_up_filler->c;

    /* Scroll button does not support highlight in UI_SCROLLBAR_STYLE_2 */
    if (type != GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL && type != GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL)
    {
        if (b->flags & UI_BUTTON_SHIFT_BUTTON1)
        {
            y++;
            gui_RGB_to_HLS(c, &hls);
            hls.l = (U8) ((U16) hls.l * 3 / 4);
            hls.s = (U8) ((U16) hls.s * 5 / 6);
            gui_HLS_to_RGB(hls, &c);
        }
        else if (b->flags & UI_BUTTON_SHIFT_BUTTON2)
        {
            y += 2;
            gui_RGB_to_HLS(c, &hls);
            hls.l = (U8) ((U16) hls.l * 3 / 4);
            hls.s = (U8) ((U16) hls.s * 5 / 6);
            gui_HLS_to_RGB(hls, &c);
        }
    }

    gui_push_clip();
    gui_set_clip(x, y, x + width - 1, y + height - 1);

    /* shift the position to leave one-pixel space */
    if (type == GUI_SCROLLBAR_BUTTON_LEFT || type == GUI_SCROLLBAR_BUTTON_RIGHT)
    {
        x++;
    }
    if (type == GUI_SCROLLBAR_BUTTON_UP || type == GUI_SCROLLBAR_BUTTON_DOWN)
    {
        y++;
    }

    switch (type)
    {
        case GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL:
            /* FALLTHROUGH no break */
        case GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL:
        {
            S32 i;
            S32 num_shadow_line;
            color_t cs[3];    /* shadow color */
            color_t cf;       /* fill color */

            gui_RGB_to_HLS(c, &hls);

            if (height < 5 || width < 5)
            {
                /* No room to display the bar in special style */
                gui_show_icon_button(b);
            }
            else
            {
                if ((type == GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL && width >= 9) ||
                    (type == GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL && height >= 9))
                {
                    gui_scrollbar_adjust_luminance(&hls.l, 10);
                    gui_HLS_to_RGB(hls, &cs[0]);
                    gui_scrollbar_adjust_luminance(&hls.l, 10);
                    gui_HLS_to_RGB(hls, &cs[1]);
                    gui_scrollbar_adjust_luminance(&hls.l, 10);
                    gui_HLS_to_RGB(hls, &cs[2]);
                    gui_scrollbar_adjust_luminance(&hls.l, 10);
                    gui_HLS_to_RGB(hls, &cf);
                    num_shadow_line = 3;
                }
                else
                {
                    gui_scrollbar_adjust_luminance(&hls.l, 10);
                    gui_HLS_to_RGB(hls, &cs[0]);
                    gui_scrollbar_adjust_luminance(&hls.l, 10);
                    gui_HLS_to_RGB(hls, &cs[1]);
                    gui_scrollbar_adjust_luminance(&hls.l, 20);
                    gui_HLS_to_RGB(hls, &cf);
                    num_shadow_line = 2;
                }

                if (type == GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL)
                {
                    S32 x2, y2;

                    if (!(width % 2))
                    {
                        width--;
                    }
                    if (height >= 8)
                    {
                        /* Leave 1-pixel space */
                        y++;
                        height -= 2;
                    }

                    x2 = x + width - 1;
                    y2 = y + height - 1;

                    if (width <= 5)
                    {
                        gui_draw_vertical_line(y + 1, y2 - 1, x, c);
                        gui_draw_vertical_line(y + 1, y2 - 1, x2, c);
                        gui_draw_horizontal_line(x + 1, x2 - 1, y, c);
                        gui_draw_horizontal_line(x + 1, x2 - 1, y2, c);
                        for (i = x + 1; i < x2 - num_shadow_line; i++)
                        {
                            gui_draw_vertical_line(y + 1, y2 - 1, i, cf);
                        }

                        for (i = 0; i < num_shadow_line; i++)
                        {
                            gui_draw_vertical_line(y + 1, y2 - 1, x2 - 1 - i, cs[i]);
                        }
                    }
                    else
                    {
#if defined(__MMI_MAINLCD_220X176__)
						x += 3;
						x2 -= 3;
#endif
                        gui_draw_vertical_line(y + 2, y2 - 2, x, c);
                        gui_draw_vertical_line(y + 2, y2 - 2, x2, c);
                        gui_draw_horizontal_line(x + 2, x2 - 2, y, c);
                        gui_draw_horizontal_line(x + 2, x2 - 2, y2, c);

                        for (i = x + 1; i < x2 - num_shadow_line; i++)
                        {
                            /*+ zhouqin 2011-02-16 modify for q6 320X240 */
                            #ifdef __MMI_BLACK_BERRY_MAIN_MENU__		
                                gui_draw_vertical_line(y + 1, y2 - 1, i, c);
                            #else
                                gui_draw_vertical_line(y + 1, y2 - 1, i, cf);
                            #endif
                            /*- zhouqin 2011-02-16 modify for q6 320X240 */
                        }

                        for (i = 0; i < num_shadow_line; i++)
                        {
                            /*+ zhouqin 2011-02-16 modify for q6 320X240 */
                            #ifdef __MMI_BLACK_BERRY_MAIN_MENU__		
                                gui_draw_vertical_line(y + 1, y2 - 1, x2 - 1 - i, c);
                            #else
                                gui_draw_vertical_line(y + 1, y2 - 1, x2 - 1 - i, cs[i]);
                            #endif
                            /*- zhouqin 2011-02-16 modify for q6 320X240 */
                        }

                        gui_putpixel(x + 1, y + 1, c);
                        gui_putpixel(x2 - 1, y + 1, c);
                        gui_putpixel(x + 1, y2 - 1, c);
                        gui_putpixel(x2 - 1, y2 - 1, c);
                    }
                }
                else
                {
                    S32 x2, y2;

                    if (!(height % 2))
                    {
                        height--;
                    }
                    if (width >= 8)
                    {
                        /* Leave 1-pixel space */
                        x++;
                        width -= 2;
                    }

                    x2 = x + width - 1;
                    y2 = y + height - 1;

                    if (width <= 5)
                    {
                        gui_draw_horizontal_line(x + 1, x2 - 1, y, c);
                        gui_draw_horizontal_line(x + 1, x2 - 1, y2, c);
                        gui_draw_vertical_line(y + 1, y2 - 1, x, c);
                        gui_draw_vertical_line(y + 1, y2 - 1, x2, c);
                        for (i = y + 1; i < y2 - num_shadow_line; i++)
                        {
                            gui_draw_horizontal_line(x + 1, x2 - 1, i, cf);
                        }

                        for (i = 0; i < num_shadow_line; i++)
                        {
                            gui_draw_horizontal_line(x + 1, x2 - 1, y2 - 1 - i, cs[i]);
                        }
                    }
                    else
                    {
                        gui_draw_horizontal_line(x + 2, x2 - 2, y, c);
                        gui_draw_horizontal_line(x + 2, x2 - 2, y2, c);
                        gui_draw_vertical_line(y + 2, y2 - 2, x, c);
                        gui_draw_vertical_line(y + 2, y2 - 2, x2, c);

                        for (i = y + 1; i < y2 - num_shadow_line; i++)
                        {
                            gui_draw_horizontal_line(x + 1, x2 - 1, i, cf);
                        }

                        for (i = 0; i < num_shadow_line; i++)
                        {
                            gui_draw_horizontal_line(x + 1, x2 - 1, y2 - 1 - i, cs[i]);
                        }

                        gui_putpixel(x + 1, y + 1, c);
                        gui_putpixel(x2 - 1, y + 1, c);
                        gui_putpixel(x + 1, y2 - 1, c);
                        gui_putpixel(x2 - 1, y2 - 1, c);
                    }
                }
            }
        }
            break;

        case GUI_SCROLLBAR_BUTTON_UP:
        {
            if ((width % 2) == 0)
            {
                width--;
            }

            if (width < 5 || (width == 5 && height < 4) || (width > 5 && height < width - 1))
            {
                /* Draw nothing */
            }
            else if (width == 5)
            {
                gui_putpixel(x, y + 2, c);
                gui_putpixel(x, y + 3, c);
                gui_putpixel(x + 1, y + 1, c);
                gui_putpixel(x + 1, y + 2, c);
                gui_putpixel(x + 2, y, c);
                gui_putpixel(x + 2, y + 1, c);
                gui_putpixel(x + 3, y + 1, c);
                gui_putpixel(x + 3, y + 2, c);
                gui_putpixel(x + 4, y + 2, c);
                gui_putpixel(x + 4, y + 3, c);
            }
            else
            {
                UI_HLS_color h_c;
                color_t cb;
                S32 i, hw;
                S32 x2, y2;

                /* Reduce the height */
                height = width - 1;

                x2 = x + width - 1;
                y2 = y + height - 1;

                gui_RGB_to_HLS(c, &h_c);
                h_c.l = (U8) (((U16) h_c.l * 2 + 255) / 3);
                gui_HLS_to_RGB(h_c, &cb);

                hw = (width >> 1);
                gui_draw_vertical_line(y + hw, y2, x, cb);
                for (i = 0; i < hw; i++)
                {
                    gui_putpixel(x + hw - i, y + i, cb);
                    gui_draw_vertical_line(y + i + 1, y2 - 1, x + hw - i, c);
                }
                for (i = 1; i < hw; i++)
                {
                    gui_putpixel(x + hw + i, y + i, cb);
                    gui_draw_vertical_line(y + i + 1, y2 - 1, x + hw + i, c);
                }
                gui_draw_vertical_line(y + hw, y2, x2, cb);
                gui_draw_horizontal_line(x, x2, y2, cb);
            }
        }
            break;

        case GUI_SCROLLBAR_BUTTON_LEFT:
        {
            if ((height % 2) == 0)
            {
                height--;
            }

            if (height < 5 || (height == 5 && width < 4) || (height > 5 && width < height - 1))
            {
                /* Draw nothing */
            }
            else if (height == 5)
            {
                gui_putpixel(x + 2, y, c);
                gui_putpixel(x + 3, y, c);
                gui_putpixel(x + 1, y + 1, c);
                gui_putpixel(x + 2, y + 1, c);
                gui_putpixel(x, y + 2, c);
                gui_putpixel(x + 1, y + 2, c);
                gui_putpixel(x + 1, y + 3, c);
                gui_putpixel(x + 2, y + 3, c);
                gui_putpixel(x + 2, y + 4, c);
                gui_putpixel(x + 3, y + 4, c);
            }
            else
            {
                UI_HLS_color h_color;
                color_t cb;
                S32 i, hh;
                S32 x2, y2;

                /* Reduce the width */
                width = height - 1;

                x2 = x + width - 1;
                y2 = y + height - 1;

                gui_RGB_to_HLS(c, &h_color);
                h_color.l = (U8) (((U16) h_color.l * 2 + 255) / 3);
                gui_HLS_to_RGB(h_color, &cb);

                hh = (height >> 1);
                gui_draw_horizontal_line(x + hh, x2, y, cb);
                for (i = 0; i < hh; i++)
                {
                    gui_putpixel(x + i, y + hh - i, cb);
                    gui_draw_horizontal_line(x + i + 1, x2 - 1, y + hh - i, c);
                }
                for (i = 1; i < hh; i++)
                {
                    gui_putpixel(x + i, y + hh + i, cb);
                    gui_draw_horizontal_line(x + i + 1, x2 - 1, y + hh + i, c);
                }
                gui_draw_horizontal_line(x + hh, x2, y2, cb);
                gui_draw_vertical_line(y, y2, x2, cb);
            }
        }
            break;

        case GUI_SCROLLBAR_BUTTON_DOWN:
        {
            S32 y1, y2;

            if ((width % 2) == 0)
            {
                width--;
            }

            // y2 = y + height - 1 - 1;
            // y1 = y2 - (width - 1) + 1;
            /* (width - 1) is the height drawn */
            y1 = y;
            y2 = y1 + (width - 1) - 1;

            if (width < 5 || (width == 5 && height < 4) || (width > 5 && height < width - 1))
            {
                /* Draw nothing */
            }
            else if (width == 5)
            {
                gui_putpixel(x, y2 - 2, c);
                gui_putpixel(x, y2 - 3, c);
                gui_putpixel(x + 1, y2 - 1, c);
                gui_putpixel(x + 1, y2 - 2, c);
                gui_putpixel(x + 2, y2, c);
                gui_putpixel(x + 2, y2 - 1, c);
                gui_putpixel(x + 3, y2 - 1, c);
                gui_putpixel(x + 3, y2 - 2, c);
                gui_putpixel(x + 4, y2 - 2, c);
                gui_putpixel(x + 4, y2 - 3, c);
            }
            else
            {
                UI_HLS_color hls_clr;
                color_t cb;
                S32 i, hw;

                /* Reduce the height */
                height = width - 1;

                gui_RGB_to_HLS(c, &hls_clr);
                hls_clr.l = (U8) (((U16) hls_clr.l * 2 + 255) / 3);
                gui_HLS_to_RGB(hls_clr, &cb);

                hw = (width >> 1);
                gui_draw_vertical_line(y1, y2 - hw, x, cb);
                for (i = 0; i < hw; i++)
                {
                    gui_putpixel(x + hw - i, y2 - i, cb);
                    gui_draw_vertical_line(y1 + 1, y2 - i - 1, x + hw - i, c);
                }
                for (i = 1; i < hw; i++)
                {
                    gui_putpixel(x + hw + i, y2 - i, cb);
                    gui_draw_vertical_line(y1 + 1, y2 - i - 1, x + hw + i, c);
                }
                gui_draw_vertical_line(y1, y2 - hw, x + width - 1, cb);
                gui_draw_horizontal_line(x, x + width - 1, y1, cb);
            }
        }
            break;

        case GUI_SCROLLBAR_BUTTON_RIGHT:
        {
            S32 x1, x2;

            if ((height % 2) == 0)
            {
                height--;
            }

            // x2 = x + width - 1 - 1;
            // x1 = x2 - (height - 1) + 1;
            /* (height - 1) is the width drawn */
            x1 = x;
            x2 = x + (height - 1) - 1;

            if (height < 5 || (height == 5 && width < 4) || (height > 5 && width < height - 1))
            {
                /* Draw nothing */
            }
            else if (height == 5)
            {
                gui_putpixel(x2 - 2, y, c);
                gui_putpixel(x2 - 3, y, c);
                gui_putpixel(x2 - 1, y + 1, c);
                gui_putpixel(x2 - 2, y + 1, c);
                gui_putpixel(x2, y + 2, c);
                gui_putpixel(x2 - 1, y + 2, c);
                gui_putpixel(x2 - 1, y + 3, c);
                gui_putpixel(x2 - 2, y + 3, c);
                gui_putpixel(x2 - 2, y + 4, c);
                gui_putpixel(x2 - 3, y + 4, c);
            }
            else
            {
                UI_HLS_color hls_color;
                color_t cb;
                S32 i, hh;

                /* Reduce the width */
                width = height - 1;

                gui_RGB_to_HLS(c, &hls_color);
                hls_color.l = (U8) (((U16) hls_color.l * 2 + 255) / 3);
                gui_HLS_to_RGB(hls_color, &cb);

                hh = (height >> 1);
                gui_draw_horizontal_line(x1, x2 - hh, y, cb);
                for (i = 0; i < hh; i++)
                {
                    gui_putpixel(x2 - i, y + hh - i, cb);
                    gui_draw_horizontal_line(x1 + 1, x2 - i - 1, y + hh - i, c);
                }
                for (i = 1; i < hh; i++)
                {
                    gui_putpixel(x2 - i, y + hh + i, cb);
                    gui_draw_horizontal_line(x1 + 1, x2 - i - 1, y + hh + i, c);
                }
                gui_draw_horizontal_line(x1, x2 - hh, y + height - 1, cb);
                gui_draw_vertical_line(y, y + height - 1, x1, cb);
            }
        }

            break;
    }

    gui_pop_clip();

}

#endif /* UI_SCROLLBAR_STYLE_2 */

/***************************************************************************** 
 * Vertical Scrollbar
 *****************************************************************************/

UI_vertical_scrollbar_theme *current_vertical_scrollbar_theme = NULL;

/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_scrollbar_current_theme
 * DESCRIPTION
 *  Applies the current theme for the vertical scrollbar
 * PARAMETERS
 *  v       [IN/OUT]        Is the vertical scrollbar object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_scrollbar_current_theme(vertical_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->focussed_bar_filler = current_vertical_scrollbar_theme->focussed_bar_filler;
    v->normal_bar_filler = current_vertical_scrollbar_theme->normal_bar_filler;
    v->disabled_bar_filler = current_vertical_scrollbar_theme->disabled_bar_filler;
    gui_set_icon_button_BG_theme(&v->up_button, current_vertical_scrollbar_theme->up_button_theme);
    gui_set_icon_button_BG_theme(&v->down_button, current_vertical_scrollbar_theme->down_button_theme);
    gui_set_icon_button_BG_theme(&v->scroll_button, current_vertical_scrollbar_theme->scroll_button_theme);
    gui_icon_button_set_icon(
        &v->up_button,
        current_vertical_scrollbar_theme->up_button_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
    gui_icon_button_set_icon(
        &v->down_button,
        current_vertical_scrollbar_theme->down_button_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
    gui_icon_button_set_icon(
        &v->scroll_button,
        current_vertical_scrollbar_theme->scroll_button_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
    v->flags |= current_vertical_scrollbar_theme->flags;
    if (v->flags & UI_VERTICAL_SCROLLBAR_BUTTON_SHIFT1)
    {
        v->up_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
        v->down_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
    }
    else if (v->flags & UI_VERTICAL_SCROLLBAR_BUTTON_SHIFT2)
    {
        v->up_button.flags |= UI_BUTTON_SHIFT_BUTTON2;
        v->down_button.flags |= UI_BUTTON_SHIFT_BUTTON2;
    }
    else
    {
        v->up_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        v->down_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        v->up_button.flags &= ~UI_BUTTON_SHIFT_BUTTON2;
        v->down_button.flags &= ~UI_BUTTON_SHIFT_BUTTON2;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_scrollbar_theme
 * DESCRIPTION
 *  Applies the given theme to the vertical scrollbar
 * PARAMETERS
 *  v       [IN/OUT]            Is the vertical scrollbar object
 *  t       [IN]         
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_scrollbar_theme(vertical_scrollbar *v, UI_vertical_scrollbar_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->focussed_bar_filler = t->focussed_bar_filler;
    v->normal_bar_filler = t->normal_bar_filler;
    v->disabled_bar_filler = t->disabled_bar_filler;
    gui_set_icon_button_BG_theme(&v->up_button, t->up_button_theme);
    gui_set_icon_button_BG_theme(&v->down_button, t->down_button_theme);
    gui_set_icon_button_BG_theme(&v->scroll_button, t->scroll_button_theme);
    gui_icon_button_set_icon(&v->up_button, t->up_button_icon, UI_DEFAULT_TRANSPARENT_COLOR);
    gui_icon_button_set_icon(&v->down_button, t->down_button_icon, UI_DEFAULT_TRANSPARENT_COLOR);
    gui_icon_button_set_icon(&v->scroll_button, t->scroll_button_icon, UI_DEFAULT_TRANSPARENT_COLOR);
    v->flags |= t->flags;
    if (v->flags & UI_VERTICAL_SCROLLBAR_BUTTON_SHIFT1)
    {
        v->up_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
        v->down_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
    }
    else if (v->flags & UI_VERTICAL_SCROLLBAR_BUTTON_SHIFT2)
    {
        v->up_button.flags |= UI_BUTTON_SHIFT_BUTTON2;
        v->down_button.flags |= UI_BUTTON_SHIFT_BUTTON2;
    }
    else
    {
        v->up_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        v->down_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        v->up_button.flags &= ~UI_BUTTON_SHIFT_BUTTON2;
        v->down_button.flags &= ~UI_BUTTON_SHIFT_BUTTON2;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_vertical_scrollbar
 * DESCRIPTION
 *  Creates a vertical scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical scrollbar object (pre-allocated)
 *  x           [IN]        Is the left-top corner
 *  y           [IN]        Is the left-top corner
 *  width       [IN]        Is the width of the vertical scrollbar (smaller)
 *  height      [IN]        Is the height of the vertical scrollbar (larger)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_vertical_scrollbar(vertical_scrollbar *v, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    S32 button_height;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->x = x;
    v->y = y;
    v->width = width;
    v->height = height;
    v->value = 0;
    v->range = 0;
    v->flags = 0;

#if defined(UI_SCROLLBAR_STYLE_1)
    gui_create_icon_button(
        &v->up_button,
        v->x,
        v->y,
        v->width,
        v->width,
        current_vertical_scrollbar_theme->up_button_icon);
    gui_create_icon_button(
        &v->down_button,
        v->x,
        v->y + v->height - v->width,
        v->width,
        v->width,
        current_vertical_scrollbar_theme->down_button_icon);
#elif defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    button_height = gui_scrollbar_get_button_min_size(GUI_SCROLLBAR_BUTTON_UP, v->width);
    if (button_height * 2 >= height)
    {
        button_height = 0;
    }
    gui_create_icon_button(
        &v->up_button,
        v->x,
        v->y,
        v->width,
        button_height,
        current_vertical_scrollbar_theme->up_button_icon);
    gui_create_icon_button(
        &v->down_button,
        v->x,
        v->y + v->height - button_height,
        v->width,
        button_height,
        current_vertical_scrollbar_theme->down_button_icon);
#endif 
    /* height of scroll button is set later */
    gui_create_icon_button(
        &v->scroll_button,
        v->x,
        v->y + v->up_button.height,
        v->width,
        v->width,
        current_vertical_scrollbar_theme->scroll_button_icon);
    gui_set_vertical_scrollbar_current_theme(v);

#ifdef __MMI_TOUCH_SCREEN__
    v->min_pen_offset = 1;
    v->pen_self_scroll = 1;
    memset(&(v->pen_state), 0, sizeof(gui_scrollbar_pen_state_struct));
#endif /* __MMI_TOUCH_SCREEN__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_scrollbar_range
 * DESCRIPTION
 *  Sets the range of a vertical scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical scrollbar object
 *  range       [IN]        (positive value: Example: Total number of items in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_scrollbar_range(vertical_scrollbar *v, S32 range)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (range < 0)
    {
        range = 0;
    }
    v->range = (U32) range;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_scrollbar_value
 * DESCRIPTION
 *  Sets the value of a vertical scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical scrollbar object
 *  value       [IN]        (positive value: Example: First displayed item in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_scrollbar_value(vertical_scrollbar *v, S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (value < 0)
    {
        value = 0;
    }
    v->value = (U32) value;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_scrollbar_scale
 * DESCRIPTION
 *  Sets the scale of a vertical scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical scrollbar object
 *  scale       [IN]        (positive value: Example: Number of displayed items in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_scrollbar_scale(vertical_scrollbar *v, S32 scale)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (scale < 0)
    {
        scale = 0;
    }
    v->scale = (U32) scale;
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_vertical_scrollbar
 * DESCRIPTION
 *  Moves the scrollbar to a differnt location
 * PARAMETERS
 *  v       [IN/OUT]    Is the vertical scrollbar object
 *  x       [IN]        Is the left-top corner of the scrollbar
 *  y       [IN]        Is the left-top corner of the scrollbar
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_vertical_scrollbar(vertical_scrollbar *v, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->x = x;
    v->y = y;
    v->up_button.x = x;
    v->down_button.x = x;
    v->up_button.y = y;
    v->down_button.y = v->y + v->height - v->down_button.height;
    v->scroll_button.x = x;
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_vertical_scrollbar
 * DESCRIPTION
 *  Changes the size of a scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical scrollbar object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_vertical_scrollbar(vertical_scrollbar *v, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    S32 button_height;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->width = width;
    v->height = height;
    v->up_button.width = width;
    v->down_button.width = width;
#if defined(UI_SCROLLBAR_STYLE_1)
    v->up_button.height = width;
    v->down_button.height = width;
#elif defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    button_height = gui_scrollbar_get_button_min_size(GUI_SCROLLBAR_BUTTON_UP, v->width);
    if (button_height * 2 >= height)
    {
        button_height = 0;
    }
    v->up_button.height = button_height;
    v->down_button.height = button_height;
#endif 
    v->up_button.x = v->x;
    v->down_button.x = v->x;
    v->up_button.y = v->y;
    v->down_button.y = v->y + v->height - v->down_button.height;
    v->scroll_button.x = v->x;
    v->scroll_button.width = v->width;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_vertical_scrollbar
 * DESCRIPTION
 *  Displays the vertical scrollbar
 * PARAMETERS
 *  v       [IN/OUT]            Is the vertical scrollbar object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_vertical_scrollbar(vertical_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, sbutton_height, sb_y, sb_yrange, r, min_height, max_height;
    U8 buttons_flag = 1;
    UI_filled_area *f;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (v->flags & UI_VERTICAL_SCROLLBAR_TYPE_BUTTONLESS)
    {
        buttons_flag = 0;
    }

    x1 = v->x;
    x2 = x1 + v->width - 1;
    y1 = v->y;
    y2 = y1 + v->height - 1;

    if (v->flags & UI_VERTICAL_SCROLLBAR_STATE_FOCUSSED)
    {
        f = v->focussed_bar_filler;
    }
    else if (v->flags & UI_VERTICAL_SCROLLBAR_STATE_DISABLED)
    {
        f = v->disabled_bar_filler;
    }
    else
    {
        f = v->normal_bar_filler;
    }
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
#if defined(UI_SCROLLBAR_STYLE_3) && defined(__MMI_WALLPAPER_ON_BOTTOM__)
    {
        BOOL source_key_enable;
        gdi_color source_key_value;

        gdi_layer_get_source_key(&source_key_enable, &source_key_value);
        if (source_key_enable)
        {
            gdi_draw_solid_rect(x1, y1, x2, y2, source_key_value);
        }
        else
        {
            /*
             * Tricky to handle scrollbar background here!
             * 
             * Some screens like Obigo or Jataayu WAP disable source key of layer, 
             * and WAP typically uses white background.
             *
             * If we use 'gui_draw_filled_area(x1, y1, x2, y2, f);' instead, it may 
             * produce weird display with inappropriate scrollbar background color 
             * created by MCT or PhoneSuite (Downloadable theme)
             *
             * We decide not to change WAP code, but use white color here for
             * __MMI_WALLPAPER_ON_BOTTOM__ (> 240x320 screen)
             *
             */
            gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_WHITE);
        }
    }
#else /* defined(UI_SCROLLBAR_STYLE_3) && defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
#if defined(__MMI_MAINLCD_220X176__)
	gui_line((x1+x2)>>1,y1,(x1+x2)>>1,y2,gui_color(128,255,255));
#else
    gui_draw_filled_area(x1, y1, x2, y2, f);
#endif
#endif /* defined(UI_SCROLLBAR_STYLE_3) && defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
    gui_pop_clip();

    if (buttons_flag)
    {
    #if defined(UI_SCROLLBAR_STYLE_1)
        gui_show_icon_button(&v->up_button);
        gui_show_icon_button(&v->down_button);
    #elif defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
#if !defined(__MMI_MAINLCD_220X176__)
        gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_UP, &v->up_button);
        gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_DOWN, &v->down_button);
#endif
    #endif 
        max_height = v->height - v->up_button.height - v->down_button.height;
    }
    else
    {
        max_height = v->height;
    }
    /* Show the scroll button based on range and value */
    r = v->range - v->scale;
#if defined(UI_SCROLLBAR_STYLE_1)
    min_height = v->width;
#elif defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    min_height = gui_scrollbar_get_button_min_size(GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL, v->width);
#endif 
    if (min_height > max_height)
    {
        min_height = max_height;
    }
    if (v->scale > 0)
    {
        if (v->range > 0)
        {
            sbutton_height = (v->scale * max_height) / v->range;
        }
        else
        {
            sbutton_height = max_height;
        }
    }
    else
    {
        sbutton_height = min_height;
    }
    if (sbutton_height < min_height)
    {
        sbutton_height = min_height;
    }
    if (sbutton_height > max_height)
    {
        sbutton_height = max_height;
    }
    sb_yrange = max_height - sbutton_height;
    if (r > 0)
    {
        /* In case of very large r & v->value */
        sb_y = (S32) ((S64) sb_yrange * v->value / r);
    }
    else
    {
        sb_y = 0;
    }
    if (sb_y < 0)
    {
        sb_y = 0;
    }
    if (sb_y > sb_yrange)
    {
        sb_y = sb_yrange;
    }

#ifdef __MMI_TOUCH_SCREEN__
    v->pen_state.remain_space = (S16) sb_yrange;
#endif 

    v->scroll_button.height = sbutton_height;
    v->scroll_button.y = sb_y + y1 + (buttons_flag ? v->up_button.height : 0);
#if defined(UI_SCROLLBAR_STYLE_1)
    gui_show_icon_button(&v->scroll_button);
#elif defined(UI_SCROLLBAR_STYLE_2)
    gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL, &v->scroll_button);
#elif defined(UI_SCROLLBAR_STYLE_3)
    if (buttons_flag)
    {
        gui_scrollbar_draw_region(
            GUI_SCROLLBAR_REGION_VERTICAL,
            v->x,
            v->up_button.y + v->up_button.height,
            v->width,
            v->down_button.y - v->up_button.y - v->up_button.height);
    }
    else
    {
        gui_scrollbar_draw_region(GUI_SCROLLBAR_REGION_VERTICAL, v->x, v->y, v->width, v->height);
    }
    gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_VERTICAL_SCROLL, &v->scroll_button);
#endif 
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_translate_pen_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  sbutton_pos         [IN]        
 *  remain_space        [IN]        
 *  range               [IN]        
 *  scale               [IN]        
 *  new_value           [OUT]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_translate_pen_position(
                S32 sbutton_pos /* relative position */ ,
                S16 remain_space,
                S32 range,
                S32 scale,
                U32 *new_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 r = range - scale;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (r < 0)
    {
        r = 0;
    }

    if (r == 0 || sbutton_pos <= 0)
    {
        *new_value = 0;
    }
    else if (sbutton_pos > remain_space)
    {
        *new_value = r;
    }
    else
    {
        /* 
         * sbutton_pos = round((remain_space * value) / r)
         * 
         *       ==> 
         *
         * new_value is round(sbutton_pos * r / remain_space) or +1 or -1
         * 
         */
        S32 new_sbutton_pos;
        S32 margin; /* 1/2 of jump distance when value is incremented by 1 */

        *new_value = (S32) ((S64) sbutton_pos * r / remain_space);

        margin = (remain_space / r) / 2;

        /* In case that (scale/range) is large, move within +-margin  */
        if (margin > 3 /* random-chosen magic number! */ )
        {
            new_sbutton_pos = (S32) ((S64) remain_space * (*new_value) / r);

            if (*new_value > 0 && sbutton_pos < new_sbutton_pos - margin)
            {
                (*new_value)--;
            }
            else if (*new_value < r && sbutton_pos > new_sbutton_pos + margin)
            {
                (*new_value)++;
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_translate_pen_more
 * DESCRIPTION
 *  
 * PARAMETERS
 *  value               [IN]        
 *  range               [IN]        
 *  scale               [IN]        
 *  diff                [IN]        
 *  scrollbar_event     [OUT]         
 *  scrollbar_param     [OUT]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_translate_pen_more(
                U32 value,
                U32 range,
                U32 scale,
                int diff,
                gui_scrollbar_pen_enum *scrollbar_event,
                gui_pen_event_param_struct *scrollbar_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int new_value = value + diff;
    int r = ((int)range - (int)scale);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (r < 0)
    {
        r = 0;
    }

    if (new_value > r)
    {
        new_value = r;
    }

    if (new_value != value)
    {
        *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
        GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, new_value);
    }
    else
    {
        *scrollbar_event = GUI_SCROLLBAR_PEN_NONE;
        GUI_PEN_EVENT_PARAM_SET_VOID(scrollbar_param);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_scrollbar_translate_pen_less
 * DESCRIPTION
 *  
 * PARAMETERS
 *  value               [IN]        
 *  diff                [IN]        
 *  scrollbar_event     [OUT]         
 *  scrollbar_param     [OUT]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_scrollbar_translate_pen_less(
                U32 value,
                int diff,
                gui_scrollbar_pen_enum *scrollbar_event,
                gui_pen_event_param_struct *scrollbar_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int new_value = value - diff;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (new_value < 0)
    {
        new_value = 0;
    }

    if (new_value != value)
    {
        *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
        GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, new_value);
    }
    else
    {
        *scrollbar_event = GUI_SCROLLBAR_PEN_NONE;
        GUI_PEN_EVENT_PARAM_SET_VOID(scrollbar_param);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_redraw_vertical_scrollbar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_redraw_vertical_scrollbar(vertical_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_show_vertical_scrollbar(v);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(v->x, v->y, v->x + v->width - 1, v->y + v->height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_vertical_scrollbar_button1_show_clicked
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_vertical_scrollbar_button1_show_clicked(vertical_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(v->up_button.flags & UI_BUTTON_SHIFT_BUTTON1))
    {
        v->up_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
        gui_redraw_vertical_scrollbar(v);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_vertical_scrollbar_button1_show_unclicked
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_vertical_scrollbar_button1_show_unclicked(vertical_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (v->up_button.flags & UI_BUTTON_SHIFT_BUTTON1)
    {
        v->up_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        gui_redraw_vertical_scrollbar(v);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_vertical_scrollbar_button2_show_clicked
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_vertical_scrollbar_button2_show_clicked(vertical_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(v->down_button.flags & UI_BUTTON_SHIFT_BUTTON1))
    {
        v->down_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
        gui_redraw_vertical_scrollbar(v);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_vertical_scrollbar_button2_show_unclicked
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_vertical_scrollbar_button2_show_unclicked(vertical_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (v->down_button.flags & UI_BUTTON_SHIFT_BUTTON1)
    {
        v->down_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        gui_redraw_vertical_scrollbar(v);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_vertical_scrollbar_set_minimum_pen_offset
 * DESCRIPTION
 *  Set the minimum scrolling offset in scrollbar.
 *  
 *  Used in GUI_SCROLLBAR_PEN_STATE_AREA1, GUI_SCROLLBAR_PEN_STATE_AREA2,
 *  GUI_SCROLLBAR_PEN_STATE_BUTTON1, GUI_SCROLLBAR_PEN_STATE_BUTTON2
 * PARAMETERS
 *  v               [IN/OUT]            Is the vertical scrollbar object
 *  min_offset      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_vertical_scrollbar_set_minimum_pen_offset(vertical_scrollbar *v, S32 min_offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->min_pen_offset = min_offset;
}


/*****************************************************************************
 * FUNCTION
 *  gui_vertical_scrollbar_set_pen_self_scroll
 * DESCRIPTION
 *  Setup whether the scrollbar is redrawn when it is scrolled by pen.
 *  If 'enable' is MMI_FALSE, the scrollbar is scrolled by its parent GUI component.
 * PARAMETERS
 *  v           [IN/OUT]            Is the horizontal scrollbar object
 *  enable      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_vertical_scrollbar_set_pen_self_scroll(vertical_scrollbar *v, BOOL enable)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (enable)
    {
        v->pen_self_scroll = 1;
    }
    else
    {
        v->pen_self_scroll = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_vertical_scrollbar_reconfigure_pen_value
 * DESCRIPTION
 *  Reconfigure scrollbar position if it is  scrolled passively by parent components
 *  
 *  Take inline editor as example, if gui_vertical_scrollbar_translate_pen_event() returns 1
 *  for GUI_SCROLLBAR_PEN_JUMP_TO_I, Inline editor might set scrollbar value to 0 instead of 1.
 *  As a result, scroll button might be moved away from the original pen down position,
 *  and this function should be used to fix internal scrollbar states.
 * PARAMETERS
 *  v       [IN/OUT]            Is the vertical scrollbar object
 *  x       [IN]        
 *  y       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_vertical_scrollbar_reconfigure_pen_value(vertical_scrollbar *v, S16 x, S16 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_scrollbar_pen_state_struct *pen_state;
    U32 new_value;
    S32 sbutton_y1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x);
    pen_state = &v->pen_state;

    if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_SBUTTON)
    {
        sbutton_y1 = v->scroll_button.y;

        gui_scrollbar_translate_pen_position(
            pen_state->initial_sbutton_pos + (y - pen_state->initial_pen_down_pos),
            pen_state->remain_space,
            v->range,
            v->scale,
            &new_value);

        if ((v->value < pen_state->previous_value && new_value > v->value) ||
            (v->value > pen_state->previous_value && new_value < v->value))
        {
            /* Note: y might not be on scroll button! */
            pen_state->initial_pen_down_pos = y;
            if (v->flags & UI_VERTICAL_SCROLLBAR_TYPE_BUTTONLESS)
            {
                pen_state->initial_sbutton_pos = (S16) (sbutton_y1 - v->y);
            }
            else
            {
                pen_state->initial_sbutton_pos = (S16) (sbutton_y1 - v->y - v->up_button.height);
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_vertical_scrollbar_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v                   [IN/OUT]         
 *  pen_event           [IN]        
 *  x                   [IN]        
 *  y                   [IN]        
 *  scrollbar_event     [OUT]         
 *  scrollbar_param     [OUT]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gui_vertical_scrollbar_translate_pen_event(
        vertical_scrollbar *v,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_scrollbar_pen_enum *scrollbar_event,
        gui_pen_event_param_struct *scrollbar_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*
     * If the Up/Down button is pressed or released, we need to gui_redraw_vertical_scrollbar()
     * even if GUI_SCROLLBAR_PEN_JUMP_TO_I is also returned.
     *
     * The reason is that parent component might discard the scrollbar event and not redraw itself.
     */

    BOOL ret;
    S32 sbutton_y1, sbutton_y2, up_button_y2, down_button_y1;
    gui_scrollbar_pen_state_struct *pen_state;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pen_state = &v->pen_state;
    ret = MMI_TRUE;
    sbutton_y1 = v->scroll_button.y;
    sbutton_y2 = sbutton_y1 + v->scroll_button.height - 1;
    up_button_y2 = v->up_button.y + v->up_button.height - 1;
    down_button_y1 = v->down_button.y;

    *scrollbar_event = GUI_SCROLLBAR_PEN_NONE;
    GUI_PEN_EVENT_PARAM_SET_VOID(scrollbar_param);

    switch (pen_event)
    {
        case MMI_PEN_EVENT_DOWN:
            if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height))
            {
                pen_state->initial_pen_down_pos = y;
                if (v->flags & UI_VERTICAL_SCROLLBAR_TYPE_BUTTONLESS)
                {
                    pen_state->initial_sbutton_pos = (S16) (sbutton_y1 - v->y);
                }
                else
                {
                    pen_state->initial_sbutton_pos = (S16) (sbutton_y1 - v->y - v->up_button.height);
                }
                pen_state->original_value = v->value;

                if (y <= up_button_y2)
                {
                    gui_vertical_scrollbar_button1_show_clicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON1;
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
                else if (y >= down_button_y1)
                {
                    gui_vertical_scrollbar_button2_show_clicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON2;
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
                else if (y >= sbutton_y1 && y <= sbutton_y2)
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_SBUTTON;
                }
                else if (y < sbutton_y1)
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA1;
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
                else
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA2;
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
            }
            else
            {
                ret = MMI_FALSE;
                pen_state->state = GUI_SCROLLBAR_PEN_STATE_INVALID;
            }
            break;

        case MMI_PEN_EVENT_LONG_TAP:
            /* FALLTHROUGH no break */
        case MMI_PEN_EVENT_REPEAT:
            if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON1)
            {
                gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON2)
            {
                gui_scrollbar_translate_pen_more(
                    v->value,
                    v->range,
                    v->scale,
                    v->min_pen_offset,
                    scrollbar_event,
                    scrollbar_param);
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA1)
            {
                if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) && (y < sbutton_y1))
                {
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
                else
                {
                    /* Scroll button is moved to pen position */
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA1_OUTSIDE;
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA2)
            {
                if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) && (y > sbutton_y2))
                {
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
                else
                {
                    /* Scroll button is moved to pen position */
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA2_OUTSIDE;
                }
            }
            break;

        case MMI_PEN_EVENT_UP:
            /* FALLTHROUGH no break */
        case MMI_PEN_EVENT_MOVE:
            if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON1)
            {
                if (pen_event == MMI_PEN_EVENT_UP)
                {
                    gui_vertical_scrollbar_button1_show_unclicked(v);
                }
                else if (!PEN_CHECK_BOUND
                         (x, y, v->up_button.x, v->up_button.y, v->up_button.width, v->up_button.height))
                {
                    gui_vertical_scrollbar_button1_show_unclicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON1_OUTSIDE;
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON1_OUTSIDE)
            {
                if (pen_event == MMI_PEN_EVENT_UP)
                {
                    /* Do nothing */
                    MMI_DBG_ASSERT(!(v->up_button.flags & UI_BUTTON_SHIFT_BUTTON1));
                }
                else if (PEN_CHECK_BOUND(x, y, v->up_button.x, v->up_button.y, v->up_button.width, v->up_button.height))
                {
                    gui_vertical_scrollbar_button1_show_clicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON1;
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON2)
            {
                if (pen_event == MMI_PEN_EVENT_UP)
                {
                    gui_vertical_scrollbar_button2_show_unclicked(v);
                }
                else if (!PEN_CHECK_BOUND
                         (x, y, v->down_button.x, v->down_button.y, v->down_button.width, v->down_button.height))
                {
                    gui_vertical_scrollbar_button2_show_unclicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON2_OUTSIDE;
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON2_OUTSIDE)
            {
                if (pen_event == MMI_PEN_EVENT_UP)
                {
                    /* Do nothing */
                    MMI_DBG_ASSERT(!(v->down_button.flags & UI_BUTTON_SHIFT_BUTTON1));
                }
                else if (PEN_CHECK_BOUND
                         (x, y, v->down_button.x, v->down_button.y, v->down_button.width, v->down_button.height))
                {
                    gui_vertical_scrollbar_button2_show_clicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON2;
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA1_OUTSIDE)
            {
                if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) && (y < sbutton_y1))
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA1;
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA1)
            {
                if (!PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) || !(y < sbutton_y1))
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA1_OUTSIDE;
                }
            }
            if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA2_OUTSIDE)
            {
                if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) && (y > sbutton_y2))
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA2;
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA2)
            {
                if (!PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) || !(y > sbutton_y2))
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA2_OUTSIDE;
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_SBUTTON)
            {
                if (PEN_ABS(x, v->x) > GUI_SCROLLBAR_VALID_RANGE)
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_SBUTTON_OUTSIDE;
                    /* Reset to original position */
                    *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
                    GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, (int)pen_state->original_value);
                }
                else
                {
                    U32 new_value;

                    gui_scrollbar_translate_pen_position(
                        pen_state->initial_sbutton_pos + (y - pen_state->initial_pen_down_pos),
                        pen_state->remain_space,
                        v->range,
                        v->scale,
                        &new_value);
                    if (new_value != v->value)
                    {
                        *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
                        GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, (int)new_value);
                    }
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_SBUTTON_OUTSIDE)
            {
                if (PEN_ABS(x, v->x) < GUI_SCROLLBAR_VALID_RANGE)
                {
                    U32 new_value;

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_SBUTTON;

                    gui_scrollbar_translate_pen_position(
                        pen_state->initial_sbutton_pos + (y - pen_state->initial_pen_down_pos),
                        pen_state->remain_space,
                        v->range,
                        v->scale,
                        &new_value);
                    if (new_value != v->value)
                    {
                        *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
                        GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, (int)new_value);
                    }
                }
            }

            if (pen_event == MMI_PEN_EVENT_UP)
            {
                pen_state->state = GUI_SCROLLBAR_PEN_STATE_INVALID;
            }

            break;

        case MMI_PEN_EVENT_ABORT:
            if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_SBUTTON)
            {
                /* Reset to original position */
                *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
                GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, (int)pen_state->original_value);
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON1)
            {
                gui_vertical_scrollbar_button1_show_unclicked(v);
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON2)
            {
                gui_vertical_scrollbar_button2_show_unclicked(v);
            }

            pen_state->state = GUI_SCROLLBAR_PEN_STATE_INVALID;
            break;

        default:
            MMI_ASSERT(0);
    }

    if (ret)
    {
        /*
         * Scrollbar does not change its value. Instead, parent components should change the value
         * * if GUI_SCROLLBAR_PEN_JUMP_TO_I is returned.
         */
        pen_state->previous_value = v->value;
    }

    if (ret && v->pen_self_scroll && *scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
    {
        v->value = scrollbar_param->_u.i;
        gui_redraw_vertical_scrollbar(v);
    }

    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 

/***************************************************************************** 
 * Horizontal Scrollbar
 *****************************************************************************/

UI_horizontal_scrollbar_theme *current_horizontal_scrollbar_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_scrollbar_current_theme
 * DESCRIPTION
 *  Applies the current theme for the horizontal scrollbar
 * PARAMETERS
 *  v       [IN/OUT]            Is the horizontal scrollbar object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_scrollbar_current_theme(horizontal_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->focussed_bar_filler = current_horizontal_scrollbar_theme->focussed_bar_filler;
    v->normal_bar_filler = current_horizontal_scrollbar_theme->normal_bar_filler;
    v->disabled_bar_filler = current_horizontal_scrollbar_theme->disabled_bar_filler;
    gui_set_icon_button_BG_theme(&v->left_button, current_horizontal_scrollbar_theme->left_button_theme);
    gui_set_icon_button_BG_theme(&v->right_button, current_horizontal_scrollbar_theme->right_button_theme);
    gui_set_icon_button_BG_theme(&v->scroll_button, current_horizontal_scrollbar_theme->scroll_button_theme);
    gui_icon_button_set_icon(
        &v->left_button,
        current_horizontal_scrollbar_theme->left_button_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
    gui_icon_button_set_icon(
        &v->right_button,
        current_horizontal_scrollbar_theme->right_button_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
    gui_icon_button_set_icon(
        &v->scroll_button,
        current_horizontal_scrollbar_theme->scroll_button_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
    v->flags |= current_horizontal_scrollbar_theme->flags;
    if (v->flags & UI_HORIZONTAL_SCROLLBAR_BUTTON_SHIFT1)
    {
        v->left_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
        v->right_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
    }
    else if (v->flags & UI_HORIZONTAL_SCROLLBAR_BUTTON_SHIFT2)
    {
        v->left_button.flags |= UI_BUTTON_SHIFT_BUTTON2;
        v->right_button.flags |= UI_BUTTON_SHIFT_BUTTON2;
    }
    else
    {
        v->left_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        v->right_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        v->left_button.flags &= ~UI_BUTTON_SHIFT_BUTTON2;
        v->right_button.flags &= ~UI_BUTTON_SHIFT_BUTTON2;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_scrollbar_theme
 * DESCRIPTION
 *  Applies the given theme to the horizontal scrollbar
 * PARAMETERS
 *  v       [IN/OUT]        Is the horizontal scrollbar object
 *  t       [IN]            Horizontal scrollbar theme
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_scrollbar_theme(horizontal_scrollbar *v, UI_horizontal_scrollbar_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->focussed_bar_filler = t->focussed_bar_filler;
    v->normal_bar_filler = t->normal_bar_filler;
    v->disabled_bar_filler = t->disabled_bar_filler;
    gui_set_icon_button_BG_theme(&v->left_button, t->left_button_theme);
    gui_set_icon_button_BG_theme(&v->right_button, t->right_button_theme);
    gui_set_icon_button_BG_theme(&v->scroll_button, t->scroll_button_theme);
    gui_icon_button_set_icon(&v->left_button, t->left_button_icon, UI_DEFAULT_TRANSPARENT_COLOR);
    gui_icon_button_set_icon(&v->right_button, t->right_button_icon, UI_DEFAULT_TRANSPARENT_COLOR);
    gui_icon_button_set_icon(&v->scroll_button, t->scroll_button_icon, UI_DEFAULT_TRANSPARENT_COLOR);
    v->flags |= t->flags;
    if (v->flags & UI_HORIZONTAL_SCROLLBAR_BUTTON_SHIFT1)
    {
        v->left_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
        v->right_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
    }
    else if (v->flags & UI_HORIZONTAL_SCROLLBAR_BUTTON_SHIFT2)
    {
        v->left_button.flags |= UI_BUTTON_SHIFT_BUTTON2;
        v->right_button.flags |= UI_BUTTON_SHIFT_BUTTON2;
    }
    else
    {
        v->left_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        v->right_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        v->left_button.flags &= ~UI_BUTTON_SHIFT_BUTTON2;
        v->right_button.flags &= ~UI_BUTTON_SHIFT_BUTTON2;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_horizontal_scrollbar
 * DESCRIPTION
 *  Creates a horizontal scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the horizontal scrollbar object (pre-allocated)
 *  x           [IN]        Is the left-top corner
 *  y           [IN]        Is the left-top corner
 *  width       [IN]        Is the width of the horizontal scrollbar (larger)
 *  height      [IN]        Is the height of the horizontal scrollbar (smaller)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_horizontal_scrollbar(horizontal_scrollbar *v, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    S32 button_width;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->x = x;
    v->y = y;
    v->width = width;
    v->height = height;
    v->value = 0;
    v->range = 0;
    v->flags = 0;

#if defined(UI_SCROLLBAR_STYLE_1)
    gui_create_icon_button(
        &v->left_button,
        v->x,
        v->y,
        v->height,
        v->height,
        current_horizontal_scrollbar_theme->left_button_icon);
    gui_create_icon_button(
        &v->right_button,
        v->x + v->width - v->height,
        v->y,
        v->height,
        v->height,
        current_horizontal_scrollbar_theme->right_button_icon);
#elif defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    button_width = gui_scrollbar_get_button_min_size(GUI_SCROLLBAR_BUTTON_LEFT, v->height);
    if (button_width * 2 >= width)
    {
        button_width = 0;
    }
    gui_create_icon_button(
        &v->left_button,
        v->x,
        v->y,
        button_width,
        v->height,
        current_horizontal_scrollbar_theme->left_button_icon);
    gui_create_icon_button(
        &v->right_button,
        v->x + v->width - button_width,
        v->y,
        button_width,
        v->height,
        current_horizontal_scrollbar_theme->right_button_icon);
#endif 
    /* width of scroll button is set later */
    gui_create_icon_button(
        &v->scroll_button,
        v->x + v->left_button.width,
        v->y,
        v->height,
        v->height,
        current_horizontal_scrollbar_theme->scroll_button_icon);
    gui_set_horizontal_scrollbar_current_theme(v);

#ifdef __MMI_TOUCH_SCREEN__
    v->min_pen_offset = 1;
    v->pen_self_scroll = 1;
    memset(&(v->pen_state), 0, sizeof(gui_scrollbar_pen_state_struct));
#endif /* __MMI_TOUCH_SCREEN__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_scrollbar_range
 * DESCRIPTION
 *  Sets the range of a horizontal scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the horizontal scrollbar object
 *  range       [IN]        (positive value: Example: Total number of items in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_scrollbar_range(horizontal_scrollbar *v, S32 range)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (range < 0)
    {
        range = 0;
    }
    v->range = (U32) range;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_scrollbar_value
 * DESCRIPTION
 *  Sets the value of a horizontal scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the horizontal scrollbar object
 *  value       [IN]        (positive value: Example: First displayed item in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_scrollbar_value(horizontal_scrollbar *v, S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (value < 0)
    {
        value = 0;
    }
    v->value = (U32) value;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_scrollbar_scale
 * DESCRIPTION
 *  Sets the scale of a horizontal scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the horizontal scrollbar object
 *  scale       [IN]        (positive value: Example: Number of displayed items in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_scrollbar_scale(horizontal_scrollbar *v, S32 scale)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (scale < 0)
    {
        scale = 0;
    }
    v->scale = (U32) scale;
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_horizontal_scrollbar
 * DESCRIPTION
 *  Moves the scrollbar to a differnt location
 * PARAMETERS
 *  v       [IN/OUT]    Is the horizontal scrollbar object
 *  x       [IN]        Is the left-top corner of the scrollbar
 *  y       [IN]        Is the left-top corner of the scrollbar
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_horizontal_scrollbar(horizontal_scrollbar *v, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->x = x;
    v->y = y;
    v->left_button.x = x;
    v->right_button.x = v->x + v->width - v->right_button.width;
    v->left_button.y = y;
    v->right_button.y = y;
    v->scroll_button.x = x = v->x + v->left_button.width;
    v->scroll_button.y = y;
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_horizontal_scrollbar
 * DESCRIPTION
 *  Changes the size of a scrollbar
 * PARAMETERS
 *  v           [IN/OUT]    Is the horizontal scrollbar object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_horizontal_scrollbar(horizontal_scrollbar *v, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    S32 button_width;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->width = width;
    v->height = height;
    v->left_button.height = height;
    v->right_button.height = height;
#if defined(UI_SCROLLBAR_STYLE_1)
    v->left_button.width = height;
    v->right_button.width = height;
#elif defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    button_width = gui_scrollbar_get_button_min_size(GUI_SCROLLBAR_BUTTON_LEFT, v->height);
    if (button_width * 2 >= width)
    {
        button_width = 0;
    }
    v->left_button.width = button_width;
    v->right_button.width = button_width;
#endif 
    v->left_button.x = v->x;
    v->right_button.x = v->x + v->width - v->right_button.width;
    v->left_button.y = v->y;
    v->right_button.y = v->y;
    v->scroll_button.y = v->y;
    v->scroll_button.height = v->height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_horizontal_scrollbar
 * DESCRIPTION
 *  Displays the horizontal scrollbar
 * PARAMETERS
 *  v       [IN/OUT]            Is the horizontal scrollbar object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_horizontal_scrollbar(horizontal_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, sbutton_width, sb_x, sb_xrange, r, min_width, max_width;
    U8 buttons_flag = 1;
    UI_filled_area *f;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (v->flags & UI_HORIZONTAL_SCROLLBAR_TYPE_BUTTONLESS)
    {
        buttons_flag = 0;
    }

    x1 = v->x;
    x2 = x1 + v->width - 1;
    y1 = v->y;
    y2 = y1 + v->height - 1;

    if (v->flags & UI_HORIZONTAL_SCROLLBAR_STATE_FOCUSSED)
    {
        f = v->focussed_bar_filler;
    }
    else if (v->flags & UI_HORIZONTAL_SCROLLBAR_STATE_DISABLED)
    {
        f = v->disabled_bar_filler;
    }
    else
    {
        f = v->normal_bar_filler;
    }
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
    /* Style 3 use three image to compose scrollbar background */
#if defined(UI_SCROLLBAR_STYLE_3) && defined(__MMI_WALLPAPER_ON_BOTTOM__)
    {
        BOOL source_key_enable;
        gdi_color source_key_value;

        gdi_layer_get_source_key(&source_key_enable, &source_key_value);
        if (source_key_enable)
        {
            gdi_draw_solid_rect(x1, y1, x2, y2, source_key_value);
        }
        else
        {
            /*
             * Please read the description in gui_show_vertical_scrollbar()
             */
            gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_WHITE);
        }
    }
#else /* defined(UI_SCROLLBAR_STYLE_3) && defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
    gui_draw_filled_area(x1, y1, x2, y2, f);
#endif /* defined(UI_SCROLLBAR_STYLE_3) && defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
    gui_pop_clip();

    if (buttons_flag)
    {
    #if defined(UI_SCROLLBAR_STYLE_1)
        gui_show_icon_button(&v->left_button);
        gui_show_icon_button(&v->right_button);
    #elif defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
        gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_LEFT, &v->left_button);
        gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_RIGHT, &v->right_button);
    #endif 
        max_width = v->width - v->left_button.width - v->right_button.width;
    }
    else
    {
        max_width = v->width;
    }

    /* Show the scroll button based on range and value */
    r = v->range - v->scale;
#if defined(UI_SCROLLBAR_STYLE_1)
    min_width = v->height;
#elif defined(UI_SCROLLBAR_STYLE_2) || defined(UI_SCROLLBAR_STYLE_3)
    min_width = gui_scrollbar_get_button_min_size(GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL, v->height);
#endif 
    if (min_width > max_width)
    {
        min_width = max_width;
    }
    if (v->scale > 0)
    {
        if (v->range > 0)
        {
            sbutton_width = (v->scale * max_width) / v->range;
        }
        else
        {
            sbutton_width = max_width;
        }
    }
    else
    {
        sbutton_width = min_width;
    }
    if (sbutton_width < min_width)
    {
        sbutton_width = min_width;
    }
    if (sbutton_width > max_width)
    {
        sbutton_width = max_width;
    }
    sb_xrange = max_width - sbutton_width;
    if (r > 0)
    {
        /* In case of very large r & v->value */
        sb_x = (S32) ((S64) sb_xrange * v->value / r);
    }
    else
    {
        sb_x = 0;
    }
    if (sb_x < 0)
    {
        sb_x = 0;
    }
    if (sb_x > sb_xrange)
    {
        sb_x = sb_xrange;
    }

#ifdef __MMI_TOUCH_SCREEN__
    v->pen_state.remain_space = (S16) sb_xrange;
#endif 

    v->scroll_button.width = sbutton_width;
    v->scroll_button.x = sb_x + x1 + (buttons_flag ? v->left_button.width : 0);
#if defined(UI_SCROLLBAR_STYLE_1)
    gui_show_icon_button(&v->scroll_button);
#elif defined(UI_SCROLLBAR_STYLE_2)
    gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL, &v->scroll_button);
#elif defined(UI_SCROLLBAR_STYLE_3)
    if (buttons_flag)
    {
        gui_scrollbar_draw_region(
            GUI_SCROLLBAR_REGION_HORIZONTAL,
            v->x + v->left_button.x + v->left_button.width,
            v->y,
            v->right_button.x - v->left_button.x - v->left_button.width,
            v->height);
    }
    else
    {
        gui_scrollbar_draw_region(GUI_SCROLLBAR_REGION_HORIZONTAL, v->x, v->y, v->width, v->height);
    }
    gui_scrollbar_draw_button(GUI_SCROLLBAR_BUTTON_HORIZONTAL_SCROLL, &v->scroll_button);
#endif 

}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_redraw_horizontal_scrollbar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_redraw_horizontal_scrollbar(horizontal_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_show_horizontal_scrollbar(v);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(v->x, v->y, v->x + v->width - 1, v->y + v->height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_scrollbar_button1_show_clicked
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_scrollbar_button1_show_clicked(horizontal_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(v->left_button.flags & UI_BUTTON_SHIFT_BUTTON1))
    {
        v->left_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
        gui_redraw_horizontal_scrollbar(v);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_scrollbar_button1_show_unclicked
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_scrollbar_button1_show_unclicked(horizontal_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (v->left_button.flags & UI_BUTTON_SHIFT_BUTTON1)
    {
        v->left_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        gui_redraw_horizontal_scrollbar(v);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_scrollbar_button2_show_clicked
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_scrollbar_button2_show_clicked(horizontal_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(v->right_button.flags & UI_BUTTON_SHIFT_BUTTON1))
    {
        v->right_button.flags |= UI_BUTTON_SHIFT_BUTTON1;
        gui_redraw_horizontal_scrollbar(v);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_scrollbar_button2_show_unclicked
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v       [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_horizontal_scrollbar_button2_show_unclicked(horizontal_scrollbar *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (v->right_button.flags & UI_BUTTON_SHIFT_BUTTON1)
    {
        v->right_button.flags &= ~UI_BUTTON_SHIFT_BUTTON1;
        gui_redraw_horizontal_scrollbar(v);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_scrollbar_set_minimum_pen_offset
 * DESCRIPTION
 *  Set the minimum scrolling offset in scrollbar.
 *  
 *  Used in GUI_SCROLLBAR_PEN_STATE_AREA1, GUI_SCROLLBAR_PEN_STATE_AREA2,
 *  GUI_SCROLLBAR_PEN_STATE_BUTTON1, GUI_SCROLLBAR_PEN_STATE_BUTTON2
 * PARAMETERS
 *  v               [IN/OUT]    Is the horizontal scrollbar object
 *  min_offset      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_scrollbar_set_minimum_pen_offset(horizontal_scrollbar *v, S32 min_offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->min_pen_offset = min_offset;
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_scrollbar_set_pen_self_scroll
 * DESCRIPTION
 *  Setup whether the scrollbar is redrawn when it is scrolled by pen.
 *  If 'enable' is MMI_FALSE, the scrollbar is scrolled by its parent GUI component.
 * PARAMETERS
 *  h           [IN/OUT]        Is the horizontal scrollbar object
 *  enable      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_scrollbar_set_pen_self_scroll(horizontal_scrollbar *h, BOOL enable)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (enable)
    {
        h->pen_self_scroll = 1;
    }
    else
    {
        h->pen_self_scroll = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_scrollbar_reconfigure_pen_value
 * DESCRIPTION
 *  Reconfigure scrollbar position if it is  scrolled passively by parent components
 *  
 *  Take inline editor as example, if gui_horizontal_scrollbar_translate_pen_event() returns 1
 *  for GUI_SCROLLBAR_PEN_JUMP_TO_I, Inline editor might set scrollbar value to 0 instead of 1.
 *  As a result, scroll button might be moved away from the original pen down position,
 *  and this function should be used to fix internal scrollbar states.
 * PARAMETERS
 *  v       [IN/OUT]    Is the horizontal scrollbar object
 *  x       [IN]        
 *  y       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_horizontal_scrollbar_reconfigure_pen_value(horizontal_scrollbar *v, S16 x, S16 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_scrollbar_pen_state_struct *pen_state;
    U32 new_value;
    S32 sbutton_x1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(y);
    pen_state = &v->pen_state;

    if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_SBUTTON)
    {
        sbutton_x1 = v->scroll_button.x;

        gui_scrollbar_translate_pen_position(
            pen_state->initial_sbutton_pos + (x - pen_state->initial_pen_down_pos),
            pen_state->remain_space,
            v->range,
            v->scale,
            &new_value);

        if ((v->value < pen_state->previous_value && new_value > v->value) ||
            (v->value > pen_state->previous_value && new_value < v->value))
        {
            /* Note: x might not be on scroll button! */
            pen_state->initial_pen_down_pos = x;
            if (v->flags & UI_HORIZONTAL_SCROLLBAR_TYPE_BUTTONLESS)
            {
                pen_state->initial_sbutton_pos = (S16) (sbutton_x1 - v->x);
            }
            else
            {
                pen_state->initial_sbutton_pos = (S16) (sbutton_x1 - v->x - v->left_button.width);
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_horizontal_scrollbar_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  v                   [IN/OUT]         
 *  pen_event           [IN]        
 *  x                   [IN]        
 *  y                   [IN]        
 *  scrollbar_event     [OUT]         
 *  scrollbar_param     [OUT]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gui_horizontal_scrollbar_translate_pen_event(
        horizontal_scrollbar *v,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_scrollbar_pen_enum *scrollbar_event,
        gui_pen_event_param_struct *scrollbar_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*
     * If the Left/Right button is pressed or released, we need to gui_redraw_horizontal_scrollbar()
     * even if GUI_SCROLLBAR_PEN_JUMP_TO_I is also returned.
     *
     * The reason is that parent component might discard the scrollbar event and not redraw itself.
     */

    BOOL ret;
    S32 sbutton_x1, sbutton_x2, left_button_x2, right_button_x1;
    gui_scrollbar_pen_state_struct *pen_state;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pen_state = &v->pen_state;
    ret = MMI_TRUE;
    sbutton_x1 = v->scroll_button.x;
    sbutton_x2 = sbutton_x1 + v->scroll_button.width - 1;
    left_button_x2 = v->left_button.x + v->left_button.width - 1;
    right_button_x1 = v->right_button.x;

    *scrollbar_event = GUI_SCROLLBAR_PEN_NONE;
    GUI_PEN_EVENT_PARAM_SET_VOID(scrollbar_param);

    switch (pen_event)
    {
        case MMI_PEN_EVENT_DOWN:
            if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height))
            {
                pen_state->initial_pen_down_pos = x;
                if (v->flags & UI_HORIZONTAL_SCROLLBAR_TYPE_BUTTONLESS)
                {
                    pen_state->initial_sbutton_pos = (S16) (sbutton_x1 - v->x);
                }
                else
                {
                    pen_state->initial_sbutton_pos = (S16) (sbutton_x1 - v->x - v->left_button.width);
                }
                pen_state->original_value = v->value;

                if (x <= left_button_x2)
                {
                    gui_horizontal_scrollbar_button1_show_clicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON1;
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
                else if (x >= right_button_x1)
                {
                    gui_horizontal_scrollbar_button2_show_clicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON2;
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
                else if (x >= sbutton_x1 && x <= sbutton_x2)
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_SBUTTON;
                }
                else if (x < sbutton_x1)
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA1;
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
                else
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA2;
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
            }
            else
            {
                ret = MMI_FALSE;
                pen_state->state = GUI_SCROLLBAR_PEN_STATE_INVALID;
            }
            break;

        case MMI_PEN_EVENT_LONG_TAP:
            /* FALLTHROUGH no break */
        case MMI_PEN_EVENT_REPEAT:
            if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON1)
            {
                gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON2)
            {
                gui_scrollbar_translate_pen_more(
                    v->value,
                    v->range,
                    v->scale,
                    v->min_pen_offset,
                    scrollbar_event,
                    scrollbar_param);
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA1)
            {
                if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) && (x < sbutton_x1))
                {
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
                else
                {
                    /* Scroll button is moved to pen position */
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA1_OUTSIDE;
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA2)
            {
                if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) && (x > sbutton_x2))
                {
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
                else
                {
                    /* Scroll button is moved to pen position */
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA2_OUTSIDE;
                }
            }
            break;

        case MMI_PEN_EVENT_UP:
            /* FALLTHROUGH no break */
        case MMI_PEN_EVENT_MOVE:
            if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON1)
            {
                if (pen_event == MMI_PEN_EVENT_UP)
                {
                    gui_horizontal_scrollbar_button1_show_unclicked(v);
                }
                else if (!PEN_CHECK_BOUND
                         (x, y, v->left_button.x, v->left_button.y, v->left_button.width, v->left_button.height))
                {
                    gui_horizontal_scrollbar_button1_show_unclicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON1_OUTSIDE;
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON1_OUTSIDE)
            {
                if (pen_event == MMI_PEN_EVENT_UP)
                {
                    /* Do nothing */
                    MMI_DBG_ASSERT(!(v->left_button.flags & UI_BUTTON_SHIFT_BUTTON1));
                }
                else if (PEN_CHECK_BOUND
                         (x, y, v->left_button.x, v->left_button.y, v->left_button.width, v->left_button.height))
                {
                    gui_horizontal_scrollbar_button1_show_clicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON1;
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON2)
            {
                if (pen_event == MMI_PEN_EVENT_UP)
                {
                    gui_horizontal_scrollbar_button2_show_unclicked(v);
                }
                else if (!PEN_CHECK_BOUND
                         (x, y, v->right_button.x, v->right_button.y, v->right_button.width, v->right_button.height))
                {
                    gui_horizontal_scrollbar_button2_show_unclicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON2_OUTSIDE;
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON2_OUTSIDE)
            {
                if (pen_event == MMI_PEN_EVENT_UP)
                {
                    /* Do nothing */
                    MMI_DBG_ASSERT(!(v->right_button.flags & UI_BUTTON_SHIFT_BUTTON1));
                }
                else if (PEN_CHECK_BOUND
                         (x, y, v->right_button.x, v->right_button.y, v->right_button.width, v->right_button.height))
                {
                    gui_horizontal_scrollbar_button2_show_clicked(v);

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_BUTTON2;
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA1_OUTSIDE)
            {
                if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) && (x < sbutton_x1))
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA1;
                    gui_scrollbar_translate_pen_less(v->value, v->min_pen_offset, scrollbar_event, scrollbar_param);
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA1)
            {
                if (!PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) || !(x < sbutton_x1))
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA1_OUTSIDE;
                }
            }
            if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA2_OUTSIDE)
            {
                if (PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) && (x > sbutton_x2))
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA2;
                    gui_scrollbar_translate_pen_more(
                        v->value,
                        v->range,
                        v->scale,
                        v->min_pen_offset,
                        scrollbar_event,
                        scrollbar_param);
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_AREA2)
            {
                if (!PEN_CHECK_BOUND(x, y, v->x, v->y, v->width, v->height) || !(x > sbutton_x2))
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_AREA2_OUTSIDE;
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_SBUTTON)
            {
                if (PEN_ABS(y, v->y) > GUI_SCROLLBAR_VALID_RANGE)
                {
                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_SBUTTON_OUTSIDE;
                    /* Reset to original position */
                    *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
                    GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, (int)pen_state->original_value);
                }
                else
                {
                    U32 new_value;

                    gui_scrollbar_translate_pen_position(
                        pen_state->initial_sbutton_pos + (x - pen_state->initial_pen_down_pos),
                        pen_state->remain_space,
                        v->range,
                        v->scale,
                        &new_value);
                    if (new_value != v->value)
                    {
                        *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
                        GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, (int)new_value);
                    }
                }
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_SBUTTON_OUTSIDE)
            {
                if (PEN_ABS(y, v->y) < GUI_SCROLLBAR_VALID_RANGE)
                {
                    U32 new_value;

                    pen_state->state = GUI_SCROLLBAR_PEN_STATE_SBUTTON;

                    gui_scrollbar_translate_pen_position(
                        pen_state->initial_sbutton_pos + (x - pen_state->initial_pen_down_pos),
                        pen_state->remain_space,
                        v->range,
                        v->scale,
                        &new_value);
                    if (new_value != v->value)
                    {
                        *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
                        GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, (int)new_value);
                    }
                }
            }

            if (pen_event == MMI_PEN_EVENT_UP)
            {
                pen_state->state = GUI_SCROLLBAR_PEN_STATE_INVALID;
            }

            break;

        case MMI_PEN_EVENT_ABORT:
            if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_SBUTTON)
            {
                /* Reset to original position */
                *scrollbar_event = GUI_SCROLLBAR_PEN_JUMP_TO_I;
                GUI_PEN_EVENT_PARAM_SET_INTEGER(scrollbar_param, (int)pen_state->original_value);
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON1)
            {
                gui_horizontal_scrollbar_button1_show_unclicked(v);
            }
            else if (pen_state->state == GUI_SCROLLBAR_PEN_STATE_BUTTON2)
            {
                gui_horizontal_scrollbar_button2_show_unclicked(v);
            }

            pen_state->state = GUI_SCROLLBAR_PEN_STATE_INVALID;
            break;

        default:
            MMI_ASSERT(0);
    }

    if (ret)
    {
        /*
         * Scrollbar does not change its value. Instead, parent components should change the value
         * * if GUI_SCROLLBAR_PEN_JUMP_TO_I is returned.
         */
        pen_state->previous_value = v->value;
    }

    if (ret && v->pen_self_scroll && *scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
    {
        v->value = scrollbar_param->_u.i;
        gui_redraw_horizontal_scrollbar(v);
    }

    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 

/***************************************************************************** 
 * Horizontal Progress Indicator
 *****************************************************************************/

UI_horizontal_progress_indicator_theme *current_horizontal_progress_indicator_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_progress_indicator_theme
 * DESCRIPTION
 *  Applies the current theme to the progress indicator object
 * PARAMETERS
 *  pi      [IN]        Is the horizontal progress indicator object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_progress_indicator_theme(horizontal_progress_indicator *pi)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pi->flags = current_horizontal_progress_indicator_theme->flags;
    pi->background_filler = current_horizontal_progress_indicator_theme->background_filler;
    pi->foreground_filler = current_horizontal_progress_indicator_theme->foreground_filler;
    pi->n_fillers = current_horizontal_progress_indicator_theme->n_fillers;
    pi->percentages = current_horizontal_progress_indicator_theme->percentages;
    pi->block_width = current_horizontal_progress_indicator_theme->block_width;
    pi->block_gap = current_horizontal_progress_indicator_theme->block_gap;
    pi->border_gap = current_horizontal_progress_indicator_theme->border_gap;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_horizontal_progress_indicator
 * DESCRIPTION
 *  creates a horizontal progress indicator object
 * PARAMETERS
 *  pi          [IN]        Is the horizontal progress indicator object (pre-allocated)
 *  x           [IN]        Left-top corner
 *  y           [IN]        Left-top corner
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_horizontal_progress_indicator(horizontal_progress_indicator *pi, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pi->x = x;
    pi->y = y;
    pi->width = width;
    pi->height = height;
    gui_set_horizontal_progress_indicator_theme(pi);
    pi->range = 100;
    pi->value = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_progress_indicator_range
 * DESCRIPTION
 *  Sets the range of a horizontal progress indicator object
 * PARAMETERS
 *  pi          [IN]        Is the horizontal progress indicator object
 *  range       [IN]        Is the range (positive number Ex: Available text length in an inputbox)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_progress_indicator_range(horizontal_progress_indicator *pi, S32 range)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (range < 0)
    {
        range = 0;
    }
    pi->range = (U32) range;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_progress_indicator_value
 * DESCRIPTION
 *  Sets the value of a horizontal progress indicator object
 * PARAMETERS
 *  pi          [IN]        Is the horizontal progress indicator object
 *  value       [IN]        Is the value (positive number Ex: current text length in an inputbox)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_progress_indicator_value(horizontal_progress_indicator *pi, S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (value < 0)
    {
        value = 0;
    }
    pi->value = (U32) value;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_horizontal_progress_indicator
 * DESCRIPTION
 *  Displays a horizontal progress indicator
 * PARAMETERS
 *  pi      [IN]        Is the horizontal progress indicator object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_horizontal_progress_indicator(horizontal_progress_indicator *pi)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    S32 ax1, ay1, ax2, ay2;
    S32 fx1, fy1, fx2, fy2;
    S32 fill_width, total_width;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = pi->x;
    y1 = pi->y;
    x2 = x1 + pi->width;
    y2 = y1 + pi->height;
    ax1 = x1 + pi->border_gap;
    ay1 = y1 + pi->border_gap;
    ax2 = x2 - pi->border_gap;
    ay2 = y2 - pi->border_gap;
    total_width = ax2 - ax1 + 1;
    if (pi->range == 0)
    {
        fill_width = total_width;
    }
    else
    {
        fill_width = pi->value * total_width / pi->range;
    }
    fx1 = ax1;
    fy1 = ay1;
    fy2 = ay2;

    gui_push_clip();
    gui_set_clip(x1, y1, x2 + 2, y2 + 2);
    gui_draw_filled_area(x1, y1, x2, y2, pi->background_filler);

    if (pi->value > 0)
    {
        if (pi->flags & UI_HORIZONTAL_PROGRESS_INDICATOR_TYPE_MULTICOLOR)
        {
            if (pi->flags & UI_HORIZONTAL_PROGRESS_INDICATOR_TYPE_STEP)
            {
                S32 block_width, block_gap, sum_of_widths;
                S32 i = 0, j = 0;
                UI_filled_area *f = &pi->foreground_filler[j];

                block_width = pi->block_width;
                block_gap = pi->block_gap;
                sum_of_widths = pi->percentages[j] * total_width / 100;
                while (i < fill_width)
                {
                    if (i > sum_of_widths)
                    {
                        j++;
                        sum_of_widths += pi->percentages[j] * total_width / 100;
                        if (j >= pi->n_fillers)
                        {
                            j = pi->n_fillers - 1;
                        }
                    }
                    f = &pi->foreground_filler[j];
                    fx1 = i + ax1;
                    fx2 = fx1 + block_width - 1;
                    if (fx2 > ax2)
                    {
                        fx2 = ax2;
                    }
                    gui_draw_filled_area(fx1, fy1, fx2, fy2, f);
                    i += block_width;
                    i += block_gap;
                }
            }
            else
            {
                S32 i = 0, j = 0, w, done = 0, sum_of_widths = 0;
                UI_filled_area *f;

                i = 0;
                for (j = 0; (j < pi->n_fillers) && (!done); j++)
                {
                    f = &pi->foreground_filler[j];
                    fx1 = i + ax1;
                    w = pi->percentages[j] * total_width / 100;
                    sum_of_widths += w;
                    if (sum_of_widths > fill_width)
                    {
                        w -= (sum_of_widths - fill_width);
                        done = 1;
                    }
                    fx2 = fx1 + w - 1;
                    if (fx2 > ax2)
                    {
                        fx2 = ax2;
                    }
                    gui_draw_filled_area(fx1, fy1, fx2, fy2, f);
                    i += w;
                }
            }
        }
        else
        {
            if (pi->flags & UI_HORIZONTAL_PROGRESS_INDICATOR_TYPE_STEP)
            {
                S32 block_width, block_gap;
                S32 i = 0;

                block_width = pi->block_width;
                block_gap = pi->block_gap;
                while (i < fill_width)
                {
                    fx1 = i + ax1;
                    fx2 = fx1 + block_width - 1;
                    if (fx2 > ax2)
                    {
                        fx2 = ax2;
                    }
                    gui_draw_filled_area(fx1, fy1, fx2, fy2, pi->foreground_filler);
                    i += block_width;
                    i += block_gap;
                }
            }
            else
            {
                fx2 = ax1 + fill_width;
                if (fx2 > ax2)
                {
                    fx2 = ax2;
                }
                gui_draw_filled_area(fx1, fy1, fx2, fy2, pi->foreground_filler);
            }
        }
    }
    gui_pop_clip();
}

/***************************************************************************** 
 * Vertical Progress Indicator
 *****************************************************************************/

UI_vertical_progress_indicator_theme *current_vertical_progress_indicator_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_progress_indicator_theme
 * DESCRIPTION
 *  Applies the current theme to the progress indicator object
 * PARAMETERS
 *  pi      [IN]        Is the vertical progress indicator object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_progress_indicator_theme(vertical_progress_indicator *pi)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pi->flags = current_vertical_progress_indicator_theme->flags;
    pi->background_filler = current_vertical_progress_indicator_theme->background_filler;
    pi->foreground_filler = current_vertical_progress_indicator_theme->foreground_filler;
    pi->n_fillers = current_vertical_progress_indicator_theme->n_fillers;
    pi->percentages = current_vertical_progress_indicator_theme->percentages;
    pi->block_height = current_vertical_progress_indicator_theme->block_height;
    pi->block_gap = current_vertical_progress_indicator_theme->block_gap;
    pi->border_gap = current_vertical_progress_indicator_theme->border_gap;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_vertical_progress_indicator
 * DESCRIPTION
 *  creates a vertical progress indicator object
 * PARAMETERS
 *  pi          [IN]        Is the vertical progress indicator object (pre-allocated)
 *  x           [IN]        Left-top corner
 *  y           [IN]        Left-top corner
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_vertical_progress_indicator(vertical_progress_indicator *pi, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    pi->x = x;
    pi->y = y;
    pi->width = width;
    pi->height = height;
    gui_set_vertical_progress_indicator_theme(pi);
    pi->range = 100;
    pi->value = 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_progress_indicator_range
 * DESCRIPTION
 *  Sets the range of a vertical progress indicator object
 * PARAMETERS
 *  pi          [IN]        Is the vertical progress indicator object
 *  range       [IN]        Is the range (positive number Ex: Available text length in an inputbox)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_progress_indicator_range(vertical_progress_indicator *pi, S32 range)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (range < 0)
    {
        range = 0;
    }
    pi->range = (U32) range;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_progress_indicator_value
 * DESCRIPTION
 *  Sets the value of a vertical progress indicator object
 * PARAMETERS
 *  pi          [IN]        Is the vertical progress indicator object
 *  value       [IN]        Is the value (positive number Ex: current text length in an inputbox)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_progress_indicator_value(vertical_progress_indicator *pi, S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (value < 0)
    {
        value = 0;
    }
    pi->value = (U32) value;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_vertical_progress_indicator
 * DESCRIPTION
 *  Displays a vertical progress indicator
 * PARAMETERS
 *  pi      [IN]        Is the vertical progress indicator object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_vertical_progress_indicator(vertical_progress_indicator *pi)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, fyh;
    S32 ax1, ay1, ax2, ay2;
    S32 fx1, fy1, fx2, fy2;
    S32 fill_height, total_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = pi->x;
    y1 = pi->y;
    x2 = x1 + pi->width;
    y2 = y1 + pi->height;
    ax1 = x1 + pi->border_gap;
    ay1 = y1 + pi->border_gap;
    ax2 = x2 - pi->border_gap;
    ay2 = y2 - pi->border_gap;
    total_height = ay2 - ay1 + 1;
    if (pi->range == 0)
    {
        fill_height = total_height;
    }
    else
    {
        fill_height = pi->value * total_height / pi->range;
    }
    fx1 = ax1;
    fy1 = ay1;
    fx2 = ax2;

    gui_push_clip();
    gui_set_clip(x1, y1, x2 + 2, y2 + 2);
    gui_draw_filled_area(x1, y1, x2, y2, pi->background_filler);

    if (pi->value > 0)
    {
        if (pi->flags & UI_VERTICAL_PROGRESS_INDICATOR_TYPE_MULTICOLOR)
        {
            if (pi->flags & UI_VERTICAL_PROGRESS_INDICATOR_TYPE_STEP)
            {
                S32 block_height, block_gap, sum_of_heights;
                S32 i = 0, j = 0;
                UI_filled_area *f = &pi->foreground_filler[j];

                block_height = pi->block_height;
                block_gap = pi->block_gap;
                sum_of_heights = pi->percentages[j] * total_height / 100;
                while (i < fill_height)
                {
                    if (i > sum_of_heights)
                    {
                        j++;
                        sum_of_heights += pi->percentages[j] * total_height / 100;
                        if (j >= pi->n_fillers)
                        {
                            j = pi->n_fillers - 1;
                        }
                    }
                    f = &pi->foreground_filler[j];
                    fy1 = i + ay1;
                    fy2 = fy1 + block_height - 1;
                    if (fy2 > ay2)
                    {
                        fy2 = ay2;
                    }
                    gui_draw_filled_area(fx1, fy1, fx2, fy2, f);
                    i += block_height;
                    i += block_gap;
                }
            }
            else
            {
                S32 i = 0, j = 0, w, done = 0, sum_of_heights = 0;
                UI_filled_area *f;

                i = 0;
                for (j = 0; (j < pi->n_fillers) && (!done); j++)
                {
                    f = &pi->foreground_filler[j];
                    fy1 = i + ay1;
                    w = pi->percentages[j] * total_height / 100;
                    sum_of_heights += w;
                    if (sum_of_heights > fill_height)
                    {
                        w -= (sum_of_heights - fill_height);
                        done = 1;
                    }
                    fy2 = fy1 + w - 1;
                    if (fy2 > ay2)
                    {
                        fy2 = ay2;
                    }
                    gui_draw_filled_area(fx1, fy1, fx2, fy2, f);
                    i += w;
                }
            }
        }
        else
        {
            if (pi->flags & UI_VERTICAL_PROGRESS_INDICATOR_TYPE_STEP)
            {
                S32 block_height, block_gap;
                S32 i = 0;

                block_height = pi->block_height;
                block_gap = pi->block_gap;
                while (i < fill_height)
                {
                    fy1 = i + ay1;
                    fy2 = fy1 + block_height - 1;
                    if (fy2 > ay2)
                    {
                        fy2 = ay2;
                    }
                    if (pi->flags & UI_VERTICAL_PROGRESS_INDICATOR_TYPE_REVERSE)
                    {
                        fyh = fy2 - fy1 + 1;
                        fy1 = (ay2 - fyh + 1) - i;
                        fy2 = ay2 - i;
                    }
                    gui_draw_filled_area(fx1, fy1, fx2, fy2, pi->foreground_filler);
                    i += block_height;
                    i += block_gap;
                }
            }
            else
            {
                fy2 = ay1 + fill_height;
                if (fy2 > ay2)
                {
                    fy2 = ay2;
                }
                if (pi->flags & UI_VERTICAL_PROGRESS_INDICATOR_TYPE_REVERSE)
                {
                    fyh = fy2 - fy1 + 1;
                    fy1 = ay2 - fyh + 1;
                    fy2 = ay2;
                }
                gui_draw_filled_area(fx1, fy1, fx2, fy2, pi->foreground_filler);
            }
        }
    }
    gui_pop_clip();
}


/***************************************************************************** 
 * Vertical Slider (Used by Multimedia Application)
 *****************************************************************************/

UI_vertical_slide_theme *current_vertical_slide_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_slide_current_theme
 * DESCRIPTION
 *  Applies the current theme for the vertical slide
 * PARAMETERS
 *  v       [IN/OUT]        Is the vertical slide object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_slide_current_theme(vertical_slide *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->focussed_bar_filler = current_vertical_slide_theme->focussed_bar_filler;
    v->normal_bar_filler = current_vertical_slide_theme->normal_bar_filler;
    v->disabled_bar_filler = current_vertical_slide_theme->disabled_bar_filler;
    gui_set_icon_button_BG_theme(&v->scroll_button, current_vertical_slide_theme->scroll_button_theme);
    gui_icon_button_set_icon(
        &v->scroll_button,
        current_vertical_slide_theme->scroll_button_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
    v->flags |= current_vertical_slide_theme->flags;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_slide_theme
 * DESCRIPTION
 *  Applies the given theme to the vertical slide
 * PARAMETERS
 *  v       [IN/OUT]        Is the vertical slide object
 *  t       [IN]         
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_slide_theme(vertical_slide *v, UI_vertical_slide_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->focussed_bar_filler = t->focussed_bar_filler;
    v->normal_bar_filler = t->normal_bar_filler;
    v->disabled_bar_filler = t->disabled_bar_filler;
    gui_set_icon_button_BG_theme(&v->scroll_button, t->scroll_button_theme);
    gui_icon_button_set_icon(&v->scroll_button, t->scroll_button_icon, UI_DEFAULT_TRANSPARENT_COLOR);
    v->flags |= t->flags;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_vertical_slide
 * DESCRIPTION
 *  Creates a vertical slide
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical slide object (pre-allocated)
 *  x           [IN]        Is the left-top corner
 *  y           [IN]        Is the left-top corner
 *  width       [IN]        Is the width of the vertical slide (smaller)
 *  height      [IN]        Is the height of the vertical slide (larger)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_vertical_slide(vertical_slide *v, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->x = x;
    v->y = y;
    v->width = width;
    v->height = height;
    v->value = 0;
    v->range = 0;
    v->flags = 0;
    gui_create_icon_button(
        &v->scroll_button,
        v->x,
        v->y,
        v->width,
        v->width,
        current_vertical_slide_theme->scroll_button_icon);
    gui_set_vertical_slide_current_theme(v);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_slide_range
 * DESCRIPTION
 *  Sets the range of a vertical slide
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical slide object
 *  range       [IN]        (positive value: Example: Total number of items in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_slide_range(vertical_slide *v, S32 range)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (range < 0)
    {
        range = 0;
    }
    v->range = (U32) range;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_slide_value
 * DESCRIPTION
 *  Sets the value of a vertical slide
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical slide object
 *  value       [IN]        (positive value: Example: First displayed item in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_slide_value(vertical_slide *v, S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (value < 0)
    {
        value = 0;
    }
    v->value = (U32) value;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_vertical_slide_scale
 * DESCRIPTION
 *  Sets the scale of a vertical slide
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical slide object
 *  scale       [IN]        (positive value: Example: Number of displayed items in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_vertical_slide_scale(vertical_slide *v, S32 scale)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (scale < 0)
    {
        scale = 0;
    }
    v->scale = (U32) scale;
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_vertical_slide
 * DESCRIPTION
 *  Moves the slide to a differnt location
 * PARAMETERS
 *  v       [IN/OUT]    Is the vertical slide object
 *  x       [IN]        Is the left-top corner of the slide
 *  y       [IN]        Is the left-top corner of the slide
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_vertical_slide(vertical_slide *v, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->x = x;
    v->y = y;
    v->scroll_button.x = x;
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_vertical_slide
 * DESCRIPTION
 *  Changes the size of a slide
 * PARAMETERS
 *  v           [IN/OUT]    Is the vertical slide object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_vertical_slide(vertical_slide *v, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    v->width = width;
    v->height = height;
    v->scroll_button.x = v->x;
    v->scroll_button.width = v->width;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_vertical_slide
 * DESCRIPTION
 *  Displays the vertical slide
 * PARAMETERS
 *  v       [IN/OUT]        Is the vertical slide object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_vertical_slide(vertical_slide *v)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, sbutton_height, sb_y, sb_yrange, r, min_height, max_height;
    UI_filled_area *f;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = v->x;
    y1 = v->y;
    x2 = x1 + v->width - 1;
    y2 = y1 + v->height - 1;
    if (v->flags & UI_VERTICAL_SLIDE_STATE_FOCUSSED)
    {
        f = v->focussed_bar_filler;
    }
    else if (v->flags & UI_VERTICAL_SLIDE_STATE_DISABLED)
    {
        f = v->disabled_bar_filler;
    }
    else
    {
        f = v->normal_bar_filler;
    }
    gui_push_clip();
    gui_set_clip(x1, y1, x2 + 2, y2 + 2);
    gui_draw_filled_area(x1, y1, x2, y2, f);
    gui_pop_clip();

    /* Show the scroll button based on range and value */
    r = v->range - v->scale;
    max_height = y2 - y1 + 1;
    min_height = v->width;
    if (v->scale > 0)
    {
        if (v->range > 0)
        {
            sbutton_height = (v->scale * max_height) / v->range;
        }
        else
        {
            sbutton_height = max_height;
        }
    }
    else
    {
        sbutton_height = min_height;
    }
    if (sbutton_height < min_height)
    {
        sbutton_height = min_height;
    }
    if (sbutton_height > max_height)
    {
        sbutton_height = max_height;
    }
    sb_yrange = max_height - sbutton_height;
    if (r > 0)
    {
        sb_y = (sb_yrange * v->value) / r;
    }
    else
    {
        sb_y = 0;
    }
    if (sb_y < 0)
    {
        sb_y = 0;
    }
    if (sb_y > sb_yrange)
    {
        sb_y = sb_yrange;
    }
    if (v->flags & UI_VERTICAL_SLIDE_TYPE_REVERSE)
    {
        sb_y = v->height - sb_y - v->scroll_button.height;
    }

    v->scroll_button.height = sbutton_height;
    v->scroll_button.y = sb_y + y1;
    gui_show_icon_button(&v->scroll_button);
}


/***************************************************************************** 
 * Horizontal Slider (Used by Multimedia Application)
 *****************************************************************************/

UI_horizontal_slide_theme *current_horizontal_slide_theme = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_slide_current_theme
 * DESCRIPTION
 *  Applies the current theme for the horizontal slide
 * PARAMETERS
 *  h       [IN/OUT]        Is the horizontal slide object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_slide_current_theme(horizontal_slide *h)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    h->focussed_bar_filler = current_horizontal_slide_theme->focussed_bar_filler;
    h->normal_bar_filler = current_horizontal_slide_theme->normal_bar_filler;
    h->disabled_bar_filler = current_horizontal_slide_theme->disabled_bar_filler;
    gui_set_icon_button_BG_theme(&h->scroll_button, current_horizontal_slide_theme->scroll_button_theme);
    gui_icon_button_set_icon(
        &h->scroll_button,
        current_horizontal_slide_theme->scroll_button_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
    h->flags |= current_horizontal_slide_theme->flags;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_slide_theme
 * DESCRIPTION
 *  Applies the given theme to the horizontal slide
 * PARAMETERS
 *  h       [IN/OUT]        Is the horizontal slide object
 *  t       [IN]         
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_slide_theme(horizontal_slide *h, UI_horizontal_slide_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    h->focussed_bar_filler = t->focussed_bar_filler;
    h->normal_bar_filler = t->normal_bar_filler;
    h->disabled_bar_filler = t->disabled_bar_filler;
    gui_set_icon_button_BG_theme(&h->scroll_button, t->scroll_button_theme);
    gui_icon_button_set_icon(&h->scroll_button, t->scroll_button_icon, UI_DEFAULT_TRANSPARENT_COLOR);
    h->flags |= t->flags;
}


/*****************************************************************************
 * FUNCTION
 *  gui_create_horizontal_slide
 * DESCRIPTION
 *  Creates a horizontal slide
 * PARAMETERS
 *  h           [IN/OUT]    Is the horizontal slide object (pre-allocated)
 *  x           [IN]        Is the left-top corner
 *  y           [IN]        Is the left-top corner
 *  width       [IN]        Is the width of the horizontal slide (smaller)
 *  height      [IN]        Is the height of the horizontal slide (larger)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_horizontal_slide(horizontal_slide *h, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    h->x = x;
    h->y = y;
    h->width = width;
    h->height = height;
    h->value = 0;
    h->range = 0;
    h->flags = 0;
    gui_create_icon_button(
        &h->scroll_button,
        h->x,
        h->y,
        h->height,
        h->height,
        current_horizontal_slide_theme->scroll_button_icon);
    gui_set_horizontal_slide_current_theme(h);
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_slide_range
 * DESCRIPTION
 *  Sets the range of a horizontal slide
 * PARAMETERS
 *  h           [IN/OUT]    Is the horizontal slide object
 *  range       [IN]        (positive value: Example: Total number of items in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_slide_range(horizontal_slide *h, S32 range)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (range < 0)
    {
        range = 0;
    }
    h->range = (U32) range;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_slide_value
 * DESCRIPTION
 *  Sets the value of a horizontal slide
 * PARAMETERS
 *  h           [IN/OUT]    Is the horizontal slide object
 *  value       [IN]        (positive value: Example: First displayed item in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_slide_value(horizontal_slide *h, S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (value < 0)
    {
        value = 0;
    }
    h->value = (U32) value;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_horizontal_slide_scale
 * DESCRIPTION
 *  Sets the scale of a horizontal slide
 * PARAMETERS
 *  h           [IN/OUT]    Is the horizontal slide object
 *  scale       [IN]        (positive value: Example: Number of displayed items in a list menu)
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_horizontal_slide_scale(horizontal_slide *h, S32 scale)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (scale < 0)
    {
        scale = 0;
    }
    h->scale = (U32) scale;
}


/*****************************************************************************
 * FUNCTION
 *  gui_move_horizontal_slide
 * DESCRIPTION
 *  Moves the slide to a differnt location
 * PARAMETERS
 *  h       [IN/OUT]    Is the horizontal slide object
 *  x       [IN]        Is the left-top corner of the slide
 *  y       [IN]        Is the left-top corner of the slide
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_horizontal_slide(horizontal_slide *h, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    h->x = x;
    h->y = y;
    h->scroll_button.x = x;
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_horizontal_slide
 * DESCRIPTION
 *  Changes the size of a slide
 * PARAMETERS
 *  h           [IN/OUT]    Is the horizontal slide object
 *  width       [IN]        Is the new width
 *  height      [IN]        Is the new height
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_horizontal_slide(horizontal_slide *h, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    h->width = width;
    h->height = height;
    h->scroll_button.x = h->x;
    h->scroll_button.width = h->width;
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_horizontal_slide
 * DESCRIPTION
 *  Displays the horizontal slide
 * PARAMETERS
 *  h       [IN/OUT]        Is the horizontal slide object
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_horizontal_slide(horizontal_slide *h)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, sbutton_width, sb_x, sb_xrange, r, min_width, max_width;
    UI_filled_area *f;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = h->x;
    y1 = h->y;
    x2 = x1 + h->width - 1;
    y2 = y1 + h->height - 1;
    if (h->flags & UI_HORIZONTAL_SLIDE_STATE_FOCUSSED)
    {
        f = h->focussed_bar_filler;
    }
    else if (h->flags & UI_HORIZONTAL_SLIDE_STATE_DISABLED)
    {
        f = h->disabled_bar_filler;
    }
    else
    {
        f = h->normal_bar_filler;
    }
    gui_push_clip();
    gui_set_clip(x1, y1, x2 + 2, y2 + 2);
    gui_draw_filled_area(x1, y1, x2, y2, f);
    gui_pop_clip();

    /* Show the scroll button based on range and value */
    r = h->range - h->scale;
    max_width = x2 - x1 + 1;
    min_width = h->height;
    if (h->scale > 0)
    {
        if (h->range > 0)
        {
            sbutton_width = (h->scale * max_width) / h->range;
        }
        else
        {
            sbutton_width = max_width;
        }
    }
    else
    {
        sbutton_width = min_width;
    }
    if (sbutton_width < min_width)
    {
        sbutton_width = min_width;
    }
    if (sbutton_width > max_width)
    {
        sbutton_width = max_width;
    }
    sb_xrange = max_width - sbutton_width;
    if (r > 0)
    {
        sb_x = (sb_xrange * h->value) / r;
    }
    else
    {
        sb_x = 0;
    }
    if (sb_x < 0)
    {
        sb_x = 0;
    }
    if (sb_x > sb_xrange)
    {
        sb_x = sb_xrange;
    }
    if (h->flags & UI_HORIZONTAL_SLIDE_TYPE_REVERSE)
    {
        sb_x = h->width - sb_x - h->scroll_button.width;
    }

    h->scroll_button.width = sbutton_width;
    h->scroll_button.x = sb_x + x1;
    gui_show_icon_button(&h->scroll_button);
}

