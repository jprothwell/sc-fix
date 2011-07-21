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
 *  gd_2d_engine.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI 2D Engine
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
#include "gd_2d_engine.h"


/*****************************************************************************
 * FUNCTION
 *  gd_bitblt_memory
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_ptr             [?]         
 *  src_pitch           [IN]        
 *  src_x               [IN]        
 *  src_y               [IN]        
 *  src_width           [IN]        
 *  src_height          [IN]        
 *  src_color_mode      [IN]        
 *  dest_ptr            [?]         
 *  dest_pitch          [IN]        
 *  dest_x              [IN]        
 *  dest_y              [IN]        
 *  dest_color_mode     [IN]        
 *  clipx1              [IN]        
 *  clipy1              [IN]        
 *  clipx2              [IN]        
 *  clipy2              [IN]        
 *  src_key_enable      [IN]        
 *  src_key             [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gd_bitblt_memory(
        U8 *src_ptr,
        U32 src_pitch,
        S16 src_x,
        S16 src_y,
        U16 src_width,
        U16 src_height,
        U8 src_color_mode,
        U8 *dest_ptr,
        U32 dest_pitch,
        S16 dest_x,
        S16 dest_y,
        U8 dest_color_mode,
        S16 clipx1,
        S16 clipy1,
        S16 clipx2,
        S16 clipy2,
        kal_bool src_key_enable,
        kal_uint32 src_key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* software bitblt */
    {
        S16 dx1, dy1, dx2, dy2;

        /*----------------------------------------------------------------*/
        /* Code Body                                                      */
        /*----------------------------------------------------------------*/
        dx1 = dest_x;
        dy1 = dest_y;
        dx2 = dx1 + src_width - 1;
        dy2 = dy1 + src_height - 1;

        /* calculate valid clipping area */
        GDI_CLIP_TWO_RECT(dx1, dy1, dx2, dy2, clipx1, clipy1, clipx2, clipy2, return);

        /* calculate new src_x,src_y */
        src_x += dx1 - dest_x;
        src_y += dy1 - dest_y;

        {
            int y, x;

            if (!src_key_enable && src_color_mode == dest_color_mode)
            {
                U32 bytes_per_pixel, valid_bytes;
                U8 *src = src_ptr + src_y * src_pitch + src_x;
                U8 *dest = dest_ptr + dy1 * dest_pitch + dx1;

                bytes_per_pixel = gdi_bits_per_pixel(src_color_mode) >> 3;
                valid_bytes = (dx2 - dx1 + 1) * bytes_per_pixel;

                for (y = dy1; y <= dy2; y++)
                {
                    memcpy(dest, src, valid_bytes);
                    dest += dest_pitch;
                    src += src_pitch;
                }
            }
            else
            {
                S32 width = dx2 - dx1 + 1;
                S32 height = dy2 - dy1 + 1;

               S32 m_src_width = src_pitch * 8 / gdi_bits_per_pixel(src_color_mode);
                S32 dest_width = dest_pitch * 8 / gdi_bits_per_pixel(dest_color_mode);

                gd_get_buf_pixel_func src_get = gd_get_buf_pixel[src_color_mode];
                gd_put_buf_pixel_func dest_put = gd_put_buf_pixel[dest_color_mode];
                gd_color_to_rgb_func color_to_rgb_src = gd_color_to_rgb[src_color_mode];
                gd_color_from_rgb_func color_from_rgb_dest = gd_color_from_rgb[dest_color_mode];

                for (y = 0; y < height; y++)
                {
                    for (x = 0; x < width; x++)
                    {
                        gdi_color color_t = src_get(src_ptr, m_src_width, src_x + x, src_y + y);

                        if (src_color_mode != dest_color_mode)
                        {
                            U32 A, R, G, B;

                            color_to_rgb_src(&A, &R, &G, &B, color_t);
                            color_t = color_from_rgb_dest(A, R, G, B);
                        }
                        if (color_t != src_key || !src_key_enable)
                        {
                            dest_put(dest_ptr, dest_width, dx1 + x, dy1 + y, color_t);
                        }
                    }
                }
            }
        }
    }
}

