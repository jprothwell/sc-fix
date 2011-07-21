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
 *  gdi_
 * Project:
 * --------
 
 * Description:
 * ------------
 *  
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_features.h"
#include "gdi_internal.h"

#ifdef GDI_USING_HW_RESIZER

#include "med_api.h"
#include "med_main.h"
//#include "visual_comm.h"
#include "imgproc.h"
#include "imgdma.h"


/*****************************************************************************
 * FUNCTION
 *  gdi_resizer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src                 [?]         
 *  src_width           [IN]        
 *  src_height          [IN]        
 *  src_pitch           [IN]        
 *  dest                [?]         
 *  dest_width          [IN]        
 *  dest_height         [IN]        
 *  output_clip_x1      [IN]        
 *  output_clip_y1      [IN]        
 *  output_clip_x2      [IN]        
 *  output_clip_y2      [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_resizer(
            U8 *src,
            U16 src_width,
            U16 src_height,
            U16 src_pitch,
            U8 *dest,
            U16 dest_width,
            U16 dest_height,
            U16 output_clip_x1,
            U16 output_clip_y1,
            U16 output_clip_x2,
            U16 output_clip_y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_resizer)
    S32 ret;
    img_resize_struct resize;

    resize.src_image_address = (U32) src;
    resize.src_width = src_width;
    resize.src_height = src_height;
    resize.src_pitch = src_pitch;

    resize.dest_image_address = (U32) dest;
    resize.dest_buffer_size = dest_width * dest_height * 2; /* / TODO: in 6228 should consider color format */
    resize.target_width = dest_width;
    resize.target_height = dest_height;

    resize.image_clip_x1 = output_clip_x1;
    resize.image_clip_y1 = output_clip_y1;
    resize.image_clip_x2 = output_clip_x2;
    resize.image_clip_y2 = output_clip_y2;

    /* It should be blocking mode. */
    resize.blocking = KAL_TRUE;
    resize.seq_num = 0;
    resize.type = IBR1_TYPE_RGB565;
    resize.data_order = IBR1_ORDER_RGB888;

    /* temp patch */
#if (defined(MT6228) || defined(MT6229))
    resize.image_pitch_mode = FALSE;
    resize.image_data_format = IMGDMA_IBW_OUTPUT_RGB565;    /* or IMGDMA_IBW_OUTPUT_RGB888 */
    resize.image_pitch_bytes = dest_width;                  /* pitch width */
#endif   

    ret = media_img_resize(MOD_MMI, &resize);

    if (ret != MED_RES_OK)
    {
        GDI_RETURN(GDI_FAILED);
    }
    else
    {
        GDI_RETURN(GDI_SUCCEED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_resizer)
}

#endif /* GDI_USING_HW_RESIZER */ 


/*****************************************************************************
 * FUNCTION
 *  gdi_fast_semi_crc
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src         [?]         
 *  size        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U32 gdi_fast_semi_crc(U8 *src, U32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    register U32 crc, temp, i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    crc = 0;
    for (i = 0; i < size; i++)
    {
        if (crc >> 15)
        {
            temp = 1;
        }
        else
        {
            temp = 0;
        }
        crc = (((crc << 1) & 0x0ffff) + temp) ^ (U32) * src;
        src++;
    }
    return crc;
}

 
///To Do : Remove this when Rotate is Ready


/*****************************************************************************
 * FUNCTION
 *  gdi_catcher_get_rotate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  rotate_value        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 gdi_catcher_get_rotate(U8 rotate_value)
{
#if defined(GDI_USING_LAYER_ROTATE)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (rotate_value)
    {
        case LCD_LAYER_ROTATE_NORMAL:
        default:
            return GDI_LCD_LAYER_ROTATE_0;
        case LCD_LAYER_ROTATE_90:
            return GDI_LCD_LAYER_ROTATE_90;
        case LCD_LAYER_ROTATE_180:
            return GDI_LCD_LAYER_ROTATE_180;
        case LCD_LAYER_ROTATE_270:
            return GDI_LCD_LAYER_ROTATE_270;
        case LCD_LAYER_MIRROR:
            return GDI_LCD_LAYER_ROTATE_0_MIRROR;
        case LCD_LAYER_MIRROR_ROTATE_90:
            return GDI_LCD_LAYER_ROTATE_90_MIRROR;
        case LCD_LAYER_MIRROR_ROTATE_180:
            return GDI_LCD_LAYER_ROTATE_180_MIRROR;
        case LCD_LAYER_MIRROR_ROTATE_270:
            return GDI_LCD_LAYER_ROTATE_270_MIRROR;
    }
#else /* defined(GDI_USING_LAYER_ROTATE) */ 
    return GDI_LCD_LAYER_ROTATE_0;
#endif /* defined(GDI_USING_LAYER_ROTATE) */ 
}

 


/*****************************************************************************
 * FUNCTION
 *  gdi_catcher_fill_info
 * DESCRIPTION
 *  
 * PARAMETERS
 *  catcher_info        [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_catcher_fill_info(gdi_catcher_info_struct *catcher_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_catcher_fill_info)
    S32 i = 0;

    memset(catcher_info, 0, sizeof(gdi_catcher_info_struct));
     
#if defined(__MMI_SUBLCD__)
    catcher_info->lcd_count = 2;    /* gdi_lcd_get_count(); */
#else 
    catcher_info->lcd_count = 1;    /* gdi_lcd_get_count(); */
#endif 
    catcher_info->layer_count = 0;

    for (i = 0; i < GDI_LAYER_TOTAL_LAYER_COUNT; i++)
    {
        if (GDI_LAYER_GET_FLAG(&GDI_LAYERS[i], GDI_LAYER_FLAG_USED))
        {
            catcher_info->layer_array[catcher_info->layer_count].layer_handle = (gdi_handle) & GDI_LAYERS[i];
            catcher_info->layer_array[catcher_info->layer_count].width = GDI_LAYERS[i].width;
            catcher_info->layer_array[catcher_info->layer_count].height = GDI_LAYERS[i].height;
            catcher_info->layer_array[catcher_info->layer_count].clipx1 = GDI_LAYERS[i].clipx1;
            catcher_info->layer_array[catcher_info->layer_count].clipy1 = GDI_LAYERS[i].clipy1;
            catcher_info->layer_array[catcher_info->layer_count].clipx2 = GDI_LAYERS[i].clipx2;
            catcher_info->layer_array[catcher_info->layer_count].clipy2 = GDI_LAYERS[i].clipy2;
            catcher_info->layer_array[catcher_info->layer_count].buf_ptr = GDI_LAYERS[i].buf_ptr;
            catcher_info->layer_array[catcher_info->layer_count].buf_ptr1 = GDI_LAYERS[i].buf_ptr1;
            catcher_info->layer_array[catcher_info->layer_count].cf = GDI_LAYERS[i].vcf;
            catcher_info->layer_array[catcher_info->layer_count].layer_size = GDI_LAYERS[i].layer_size;
            catcher_info->layer_array[catcher_info->layer_count].offset_x = GDI_LAYERS[i].offset_x;
            catcher_info->layer_array[catcher_info->layer_count].offset_y = GDI_LAYERS[i].offset_y;
            catcher_info->layer_array[catcher_info->layer_count].source_key_value = gdi_layer_info[i].source_key;
            catcher_info->layer_array[catcher_info->layer_count].background = GDI_LAYERS[i].background;
            catcher_info->layer_array[catcher_info->layer_count].opacity_value = gdi_layer_info[i].opacity_value;
            catcher_info->layer_array[catcher_info->layer_count].rotate_value = GDI_LAYERS[i].rotate_value;
            catcher_info->layer_array[catcher_info->layer_count].opacity_enable = gdi_layer_info[i].opacity_enable;
            catcher_info->layer_array[catcher_info->layer_count].source_key_enable =
                gdi_layer_info[i].source_key_enable;
            catcher_info->layer_count++;
        }
    }
    for (i = 0; i < catcher_info->lcd_count; i++)
    {
        S32 j = 0;

        catcher_info->lcd_array[i].layer_count = 0;
        catcher_info->lcd_array[i].act_width = GDI_LCD_ARRAY[i].act_width;
        catcher_info->lcd_array[i].act_height = GDI_LCD_ARRAY[i].act_height;
        catcher_info->lcd_array[i].cf = GDI_LCD_ARRAY[i].cf;
        catcher_info->lcd_array[i].ROIx1 = GDI_LCD_ARRAY[i].blt_rect.x1;
        catcher_info->lcd_array[i].ROIy1 = GDI_LCD_ARRAY[i].blt_rect.y1;
        catcher_info->lcd_array[i].ROIx2 = GDI_LCD_ARRAY[i].blt_rect.x2;
        catcher_info->lcd_array[i].ROIy2 = GDI_LCD_ARRAY[i].blt_rect.y2;
        catcher_info->lcd_array[i].rotate_value = GDI_LCD_ARRAY[i].rotate_value;
        for (j = 0; j < GDI_LAYER_HW_LAYER_COUNT; j++)
        {
            if (GDI_LCD_ARRAY[i].blt_handle[j] != GDI_ERROR_HANDLE && GDI_LCD_ARRAY[i].blt_handle[j] != GDI_NULL_HANDLE)
            {
                S32 k = 0;

                for (k = 0; k < catcher_info->layer_count; k++)
                {
                    if (GDI_LCD_ARRAY[i].blt_handle[j] == catcher_info->layer_array[k].layer_handle)
                    {
                        catcher_info->lcd_array[i].blt_handle[catcher_info->lcd_array[i].layer_count] = k;
                        catcher_info->lcd_array[i].layer_count++;
                    }
                }
            }
        }
    }
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_catcher_fill_info)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_util_fit_box
 * DESCRIPTION
 *  play animation for certain frame by image ptr
 * PARAMETERS
 *  mode            [IN]        
 *  bbox_width      [IN]        
 *  bbox_height     [IN]        
 *  src_width       [IN]        
 *  src_height      [IN]        
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  dest_width      [IN]        
 *  dest_height     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_util_fit_box(
        S32 mode,
        S32 bbox_width,
        S32 bbox_height,
        S32 src_width,
        S32 src_height,
        S32 *offset_x,
        S32 *offset_y,
        S32 *dest_width,
        S32 *dest_height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_util_fit_bbox)

    if (mode == GDI_UTIL_MODE_LONG_SIDE_FIT || mode == GDI_UTIL_MODE_NO_RESIZE_OR_LONG_SIDE_FIT)
    {
        if (src_width == 0 || src_height == 0 || bbox_width == 0 || bbox_height == 0)
        {
            *dest_width = 0;
            *dest_height = 0;
        }
        else if (src_width * bbox_height > src_height * bbox_width)
        {
            *dest_width = bbox_width;
            *dest_height = (src_height * bbox_width / src_width==0)?(2*src_height * bbox_width / src_width>=1?1:0):(src_height * bbox_width / src_width);//chenhe add,20081106,for our chip dosen't support float acculate,i try to get 1 when the divide result is more than 0.5
        }
        else
        {
            *dest_width = (src_width * bbox_height / src_height==0)?(2*src_width * bbox_height/ src_height>=1?1:0):(src_width * bbox_height / src_height);//chenhe add,20081106,for our chip dosen't support float acculate,i try to get 1 when the divide result is more than 0.5
            *dest_height = bbox_height;

        }
    }
    else if (mode == GDI_UTIL_MODE_SHORT_SIDE_FIT || mode == GDI_UTIL_MODE_NO_RESIZE_OR_SHORT_SIDE_FIT)
    {
        if (src_width == 0 || src_height == 0 || bbox_width == 0 || bbox_height == 0)
        {
            *dest_width = 0;
            *dest_height = 0;
        }
        else if (src_width * bbox_height < src_height * bbox_width)
        {
            *dest_width = bbox_width;
            *dest_height = src_height * bbox_width / src_width;
        }
        else
        {
            *dest_width = src_width * bbox_height / src_height;
            *dest_height = bbox_height;
        }
    }

    if (mode == GDI_UTIL_MODE_NO_RESIZE_OR_LONG_SIDE_FIT || mode == GDI_UTIL_MODE_NO_RESIZE_OR_SHORT_SIDE_FIT)
    {
        if (src_width <= bbox_width && src_height <= bbox_height)
        {
            *dest_width = src_width;
            *dest_height = src_height;
        }
    }

    /* non-ratio resize if different is small. */
    {
        int diff;

        diff = GDI_ABS(*dest_height - bbox_height);
        if (diff <= 2 && *dest_width == bbox_width)
        {
            *dest_height = bbox_height;
        }

        diff = GDI_ABS(*dest_width - bbox_width);
        if (diff <= 2 && *dest_height == bbox_height)
        {
            *dest_width = bbox_width;
        }
    }

    *offset_x = (bbox_width - *dest_width) / 2;
    *offset_y = (bbox_height - *dest_height) / 2;

    GDI_EXIT_CRITICAL_SECTION(gdi_image_util_fit_bbox)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_util_fit_bbox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  bbox_width      [IN]        
 *  bbox_height     [IN]        
 *  src_width       [IN]        
 *  src_height      [IN]        
 *  offset_x        [?]         
 *  offset_y        [?]         
 *  dest_width      [?]         
 *  dest_height     [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_image_util_fit_bbox(
        S32 bbox_width,
        S32 bbox_height,
        S32 src_width,
        S32 src_height,
        S32 *offset_x,
        S32 *offset_y,
        S32 *dest_width,
        S32 *dest_height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_util_fit_box(
        GDI_UTIL_MODE_LONG_SIDE_FIT,
        bbox_width,
        bbox_height,
        src_width,
        src_height,
        offset_x,
        offset_y,
        dest_width,
        dest_height);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_util_check_decoder_cap
 * DESCRIPTION
 *  check gdi decoder's capability
 * PARAMETERS
 *  image_type          [IN]        
 *  image_width         [IN]        
 *  image_height        [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_util_check_decoder_cap(U16 image_type, S32 image_width, S32 image_height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_util_check_decoder_cap)
    if (image_type == GDI_IMAGE_TYPE_JPG_FILE)
    {
    #if defined(GDI_USING_HW_JPEG) || defined(GDI_USING_SW_JPEG)
        GDI_RETURN(GDI_SUCCEED);
    #else 
        GDI_RETURN(GDI_FAILED);
    #endif 
    }
    else
    {
        GDI_RETURN(GDI_SUCCEED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_image_util_check_decoder_cap)
}

S32 gdi_ucs2_stricmp(U16 *s1,U16 *s2)
{
	GDI_DEBUG_ASSERT(!((U32)s1 & 0x01));
	GDI_DEBUG_ASSERT(!((U32)s2 & 0x01));

	while (1)
	{
        U32 cmp = (U32)*s1 ^ (U32)*s2;
        if(cmp)
            if(cmp != 32 || 'A'>*s1 || *s1>'z' ||  'A'>*s2 || *s2>'z' )
                break;
		s2++;
		if ( *s1++=='\0' )
			return(0);
	}
      
   return( *s1 - *s2 );
}

