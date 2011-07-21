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
 *  gdi_font.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI low-level font library
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#include "gdi_internal.h"
#include "gd_primitive.h"

#include "fontres.h"

#ifdef GDI_USING_2D_ENGINE
//#include "2d_engine.h"
#endif 

const U32 gdi_tilt_table[61] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 2,
    2, 2, 2, 2, 3, 3, 3, 3, 4, 4,
    4, 4, 5, 5, 5, 5, 5, 6, 6, 6,
    6, 7, 7, 7, 7, 8, 8, 8, 8, 8,
    9, 9, 9, 9, 10, 10, 10, 10, 11, 11,
    11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13
};

#define MAX_TILT  13

static S32 text_clip_x1;    /* Left edge of text clip rectangle */
static S32 text_clip_y1;    /* Top edge of text clip rectangle  */
static S32 text_clip_x2;    /* Right edge of text clip rectangle   */
static S32 text_clip_y2;    /* Bottom edge of text clip rectangle  */

static gd_put_pixel_func put_pixel;

#define put_pixel_with_clip(x,y,c)                    \
   {                                                  \
      if((x >= text_clip_x1) && (x <= text_clip_x2)   \
      &&(y >= text_clip_y1) && (y <= text_clip_y2))   \
         put_pixel(x,y,c);                            \
   }                                                  \

#define GDI_CHECK_CHAR_CLIP(x,y,char_width,char_height,NULL_ACTION)                 \
   {                                                                                \
      if((x -1 > text_clip_x2   )    || (x+char_width  +MAX_TILT +1 < text_clip_x1) \
      ||(y -1 > text_clip_y2) || (y+char_height +1 < text_clip_y1))                 \
         NULL_ACTION;                                                               \
   }                                                                                \

#define __GDI_SHOW_CHAR__  gdi_show_char_no_clip
#define __GDI_SET_PIXEL__  put_pixel
#include "gdi_font_show_char.h"
#undef __GDI_SHOW_CHAR__
#undef __GDI_SET_PIXEL__

#define __GDI_SHOW_CHAR__  gdi_show_char_with_clip
#define __GDI_SET_PIXEL__  put_pixel_with_clip
#include "gdi_font_show_char.h"
#undef __GDI_SHOW_CHAR__
#undef __GDI_SET_PIXEL__


/*****************************************************************************
 * FUNCTION
 *  gdi_show_char
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  color               [IN]        
 *  font_data           [?]         
 *  font_data_size      [IN]        
 *  char_width          [IN]        
 *  char_height         [IN]        
 *  font_attr           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_show_char(
        S32 x,
        S32 y,
        gdi_color color,
        U8 *font_data,
        U32 font_data_size,
        U16 char_width,
        U16 char_height,
        U8 font_attr)
{
    /* hardware font engine */
    #ifdef GDI_USING_2D_ENGINE
    if (GDI_LAYER.bits_per_pixel == 16) /* only support 16 bit hardware accelerator */
        if (x >= 0 && y >= 0)           /* hardware accelerator only support plus coordinate value */
            if ((font_attr & (~FONTATTRIB_NORMAL)) == 0)
            {
                GDI_CHECK_CHAR_CLIP(x, y, char_width, char_height, return);
                 
                g2d_font_color(color);
                g2d_font_draw((U32) font_data, char_width, char_height, x, y);
                return;
            }
    #endif /* GDI_USING_2D_ENGINE */ 

    #ifdef GDI_USING_2D_ENGINE_V2_FONT
    if (gdi_2d_is_available() && !(font_attr & FONTATTRIB_UNDERLINE))
    {
        GDI_CHECK_CHAR_CLIP(x, y, char_width, char_height, return);
         
        if (font_attr & (FONTATTRIB_ITALIC | FONTATTRIB_OBLIQUE))
        {
            gdi_2d_set_tilt_table(gdi_tilt_table, char_height);
        }
        gdi_2d_draw_font(x, y, font_data, 0, char_width, char_height);
        return;
    }
    #endif /* GDI_USING_2D_ENGINE_V2_FONT */ 

    /* software font engine */
    if ((x - 1 < text_clip_x1) || (x + char_width + MAX_TILT + 1 > text_clip_x2)
        || (y - 1 < text_clip_y1) || (y + char_height + 1 > text_clip_y2))
    {
        GDI_CHECK_CHAR_CLIP(x, y, char_width, char_height, return);
        gdi_show_char_with_clip(x, y, color, font_data, font_data_size, char_width, char_height, font_attr);
    }
    else
    {
        gdi_show_char_no_clip(x, y, color, font_data, font_data_size, char_width, char_height, font_attr);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_show_char_bordered
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  text_color          [IN]        
 *  border_color        [IN]        
 *  font_data           [?]         
 *  font_data_size      [IN]        
 *  char_width          [IN]        
 *  char_height         [IN]        
 *  font_attr           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_show_char_bordered(
        S32 x,
        S32 y,
        gdi_color text_color,
        gdi_color border_color,
        U8 *font_data,
        U32 font_data_size,
        U16 char_width,
        U16 char_height,
        U8 font_attr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* hardware font engine */
#ifdef GDI_USING_2D_ENGINE
    if (GDI_LAYER.bits_per_pixel == 16) /* only support 16 bit hardware accelerator */
        if (x > 0 && y > 0)             /* hardware accelerator only support plus coordinate value */
            if ((font_attr & (~FONTATTRIB_NORMAL)) == 0)
            {
                g2d_font_color(border_color);
                g2d_font_draw((U32) font_data, char_width, char_height, x - 1, y - 1);
                g2d_font_draw((U32) font_data, char_width, char_height, x - 1, y + 1);
                g2d_font_draw((U32) font_data, char_width, char_height, x + 1, y - 1);
                g2d_font_draw((U32) font_data, char_width, char_height, x + 1, y + 1);
                g2d_font_draw((U32) font_data, char_width, char_height, x + 0, y - 1);
                g2d_font_draw((U32) font_data, char_width, char_height, x + 0, y + 1);
                g2d_font_draw((U32) font_data, char_width, char_height, x - 1, y + 0);
                g2d_font_draw((U32) font_data, char_width, char_height, x + 1, y + 0);

                g2d_font_color(text_color);

                g2d_font_draw((U32) font_data, char_width, char_height, x, y);
                return;
            }
#endif /* GDI_USING_2D_ENGINE */ 

#ifdef GDI_USING_2D_ENGINE_V2_FONT
    if (gdi_2d_is_available() && !(font_attr & FONTATTRIB_UNDERLINE))
    {
        gdi_2d_set_font_attr(
            font_attr & (FONTATTRIB_ITALIC | FONTATTRIB_OBLIQUE),      /* is_tilt */
            FALSE,  /* /TODO: is_bold always be FALSE, it should make some change to pixcomfontengine.c, and turn on this HW feature */
            TRUE,   /* is_border */
            FALSE,  /* don't enable bg_color, */
            text_color,
            border_color);
         
        if (font_attr & (FONTATTRIB_ITALIC | FONTATTRIB_OBLIQUE))
        {
            gdi_2d_set_tilt_table(gdi_tilt_table, char_height);
        }
        gdi_2d_draw_font(x, y, font_data, 0, char_width, char_height);
        return;
    }
#endif /* GDI_USING_2D_ENGINE_V2_FONT */ 

    /* software font engine */
    {
        U8 font_attr1 = font_attr &= ~FONTATTRIB_UNDERLINE;

        gdi_show_char(x - 1, y - 1, border_color, font_data, font_data_size, char_width, char_height, font_attr1);
        gdi_show_char(x - 1, y + 1, border_color, font_data, font_data_size, char_width, char_height, font_attr1);
        gdi_show_char(x + 1, y - 1, border_color, font_data, font_data_size, char_width, char_height, font_attr1);
        gdi_show_char(x + 1, y + 1, border_color, font_data, font_data_size, char_width, char_height, font_attr1);
        gdi_show_char(x + 0, y - 1, border_color, font_data, font_data_size, char_width, char_height, font_attr1);
        gdi_show_char(x + 0, y + 1, border_color, font_data, font_data_size, char_width, char_height, font_attr1);
        gdi_show_char(x - 1, y + 0, border_color, font_data, font_data_size, char_width, char_height, font_attr1);
        gdi_show_char(x + 1, y + 0, border_color, font_data, font_data_size, char_width, char_height, font_attr1);

        gdi_show_char(x, y, text_color, font_data, font_data_size, char_width, char_height, font_attr);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_font_begin
 * DESCRIPTION
 *  
 * PARAMETERS
 *  fg_color        [IN]        
 *  font_attr       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_font_begin(gdi_color fg_color, U8 font_attr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;

#ifdef GDI_USING_2D_ENGINE
    if (GDI_LAYER.bits_per_pixel == 16)
    {
        g2d_font_begin(
            (U32) GDI_LAYER.buf_ptr,
            GDI_LAYER.width,
            font_attr & (FONTATTRIB_ITALIC | FONTATTRIB_OBLIQUE),
            GDI_LAYER.clipx1,
            GDI_LAYER.clipy1,
            GDI_LAYER.clipx2,
            GDI_LAYER.clipy2);

        g2d_font_color(fg_color);
    }
#endif /* GDI_USING_2D_ENGINE */ 

#ifdef GDI_USING_2D_ENGINE_V2_FONT
    {
        gdi_2d_begin();
        if (gdi_2d_is_available())
        {
            gdi_2d_set_layer_as_dest_buffer((gdi_handle) gdi_act_layer);
            gdi_2d_set_font_attr(
                font_attr & (FONTATTRIB_ITALIC | FONTATTRIB_OBLIQUE),  /* is_tilt */
                FALSE,  /* /TODO: is_bold always be FALSE, it should make some change to pixcomfontengine.c, and turn on this HW feature */
                FALSE,  /* is_border */
                FALSE,  /* don't enable bg_color, */
                fg_color,
                0);
        }
    }
#endif /* GDI_USING_2D_ENGINE_V2_FONT */ 

    put_pixel = gd_put_pixel[gdi_act_layer->cf];

    text_clip_x1 = GDI_LAYER.clipx1;
    text_clip_y1 = GDI_LAYER.clipy1;
    text_clip_x2 = GDI_LAYER.clipx2;
    text_clip_y2 = GDI_LAYER.clipy2;

}


/*****************************************************************************
 * FUNCTION
 *  gdi_font_end
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_font_end(void)
{
#ifdef GDI_USING_2D_ENGINE
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GDI_LAYER.bits_per_pixel == 16)
    {
        g2d_font_end();
    }
#endif /* GDI_USING_2D_ENGINE */ 

#ifdef GDI_USING_2D_ENGINE_V2_FONT
    gdi_2d_end();
#endif 

    GDI_UNLOCK;
}

