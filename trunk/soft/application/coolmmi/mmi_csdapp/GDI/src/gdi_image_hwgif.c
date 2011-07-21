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
 *  gdi_image_hwgif.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Image Hardware Gif related.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#define __NEWSIMULATOR
#include "stdc.h"
//#include "mmi_features.h"
//#include "mmidatatype.h"
//#include "fat_fs.h"

#include "gdi_internal.h"
#include "gd_primitive.h"

#if 0
#if defined(CSD_TARGET)
#pragma Otime
#pragma no_check_stack
#endif /* defined(CSD_TARGET) */ 
#endif

extern U16 gdi_image_gif_transparent_index;
extern U8 gdi_image_gif_palette_size;

#if defined(GDI_USING_HW_GIF)

//#include "2d_engine.h"
#include "gif_decoder.h"

static U16 gct_palette[256];
static U16 lct_palette[256];

static gif_para_struct para;
static gif_info_struct info;
static int last_frame;

extern gdi_image_gif_struct gdi_gif_cache[];
extern S16 gdi_gif_stack[];
extern S32 GDI_TREE_BUFFER[];
extern void GIF_Config_resource(gif_resource_struct *resource);


/*****************************************************************************
 * FUNCTION
 *  gdi_image_hwgif_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_image_hwgif_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gif_resource_struct resource;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g2d_init();

    memset(&para, 0, sizeof(para));

    para.decode_test = KAL_FALSE;
    para.work_adrs = (kal_uint32) gdi_work_buffer;
    para.work_size = (kal_uint32) gdi_work_buffer_size;

    resource.stack = (kal_uint8*) gdi_gif_stack;
    resource.tree = (kal_uint8*) GDI_TREE_BUFFER;
    resource.GCT = (kal_uint8*) gct_palette;
    resource.LCT = (kal_uint8*) lct_palette;

    resource.stack_size = GDI_GIF_STACK_SIZE;
    resource.tree_size = GDI_GIF_TREE_SIZE;
    resource.GCT_size = 512;
    resource.LCT_size = 512;

    GIF_Config_resource(&resource);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_hwgif_display
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src                     [?]         
 *  src_size                [IN]        
 *  ox                      [IN]        
 *  oy                      [IN]        
 *  frame_number            [IN]        
 *  use_disposal_method     [IN]        
 *  is_transparent          [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_hwgif_display(
            U8 *src,
            U32 src_size,
            S32 ox,
            S32 oy,
            U16 frame_number,
            BOOL use_disposal_method,
            BOOL *is_transparent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    do
    {
        gdi_image_gif_transparent_index = 256;
        if (src != para.img_adrs || last_frame + 1 != frame_number)
        {
            para.img_adrs = src;
            para.img_size = src_size;
            para.reset = KAL_TRUE;
        }
        else
        {
            para.reset = KAL_FALSE;
        }
        last_frame = frame_number;

        if (use_disposal_method)
        {
            para.disposal_force = GIF_CSD_NO_ACTION;
        }
        else
        {
            para.disposal_force = GIF_NOT_DISPOSAL;
        }

        para.frame_number = frame_number;

        para.clip_x1 = GDI_LAYER.clipx1;
        para.clip_y1 = GDI_LAYER.clipy1;
        para.clip_x2 = GDI_LAYER.clipx2;
        para.clip_y2 = GDI_LAYER.clipy2;

        para.dest_x = ox;
        para.dest_y = oy;

        para.shadow_adrs = (kal_uint32) GDI_LAYER.buf_ptr;
        para.shadow_width = GDI_LAYER.width;
        para.shadow_height = GDI_LAYER.height;

        switch (GIF_Display_HW(&para, &info))
        {
            case GIF_FINISH_FRAME:
                result = GDI_SUCCEED;
                break;
            case GIF_LAST_FRAME:
                result = GDI_SUCCEED;
                break;
            case GIF_OUT_OF_RANGE:
                result = GDI_SUCCEED;
                break;
            case GIF_FORMAT_ERROR:
                result = GDI_FAILED;
                break;
                /*
                 * case GIF_NO_FRAME:
                 * case GIF_DECODE_TIMEOUT:
                 * case GIF_TRUNCATED_FILE:
                 * case GIF_MEM_INSUFFICEITN:
                 */
            default:
                result = GDI_FAILED;
                break;
        }
        gdi_anim_set_delay(info.delay_time * 10);

        gdi_image_gif_width = info.width;
        gdi_image_gif_height = info.height;
        *is_transparent = info.transparent_flag;
        if (info.transparent_flag)
        {
            gdi_image_gif_transparent_index = (U16) info.transparent_index;
        }

    } while (0);

    /* decode fail , force the reset the hw gif decoder. */
    if (result < 0)
    {
        para.img_adrs = NULL;
    }

    return result;
}

#endif /* defined(GDI_USING_HW_GIF) */ 

#ifdef GDI_USING_HW_GIF_V2

#include "fs_type.h"
#include "gif_decoder_v2.h"
#include "drm_gprot.h"

#include "med_api.h"
#include "med_main.h"
#include "med_struct.h"
static gif_info_struct gdi_image_hw_gif_v2_info;
static U32 gct_palette[256];
static U32 lct_palette[256];


/*****************************************************************************
 * FUNCTION
 *  gdi_image_hwgif_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_image_hwgif_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gif_init();
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_hwgif_display_internal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src                     [?]         
 *  size                    [IN]        
 *  ox                      [IN]        
 *  oy                      [IN]        
 *  resized_width           [IN]        
 *  resized_height          [IN]        
 *  frame_number            [IN]        
 *  is_file                 [IN]        
 *  is_resized              [IN]        
 *  use_disposal_method     [IN]        
 *  is_blocking             [IN]        
 *  transparent_enable      [IN]        
 *  callback                [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_hwgif_display_internal(
            U8 *src,
            U32 size,
            S32 ox,
            S32 oy,
            S32 resized_width,
            S32 resized_height,
            U16 frame_number,
            BOOL is_file,
            BOOL is_resized,
            BOOL use_disposal_method,
            BOOL is_blocking,
            BOOL transparent_enable,
            void (*callback) (GDI_RESULT result))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_hwgif_display_internal)
    media_img_decode_req_struct decoder;
    S32 ret;
    FS_HANDLE file_handle = 0;

    gdi_image_gif_transparent_index = 256;

    if (is_file)
    {
        file_handle = DRM_open_file((U16*) src, FS_READ_ONLY | FS_OPEN_NO_DIR, 0);
        if (file_handle < 0)
        {
            GDI_RETURN(GDI_FAILED);
        }

        //if(DRM_file_size(file_handle, (DWORD*)&size)<0)
        //     size = 0;
        decoder.media_mode = (kal_uint16) MED_MODE_FILE;
        decoder.cache_id = gdi_image_gif_get_cache_key_from_file(file_handle, (S8*) src, size);
        decoder.data = (void*)file_handle;
    }
    else
    {
        decoder.media_mode = (kal_uint16) MED_MODE_ARRAY;
        decoder.cache_id = (U32) src;
        decoder.data = src;
    }

    if (!is_resized)
    {
        resized_width = resized_height = 0; /* use orignal size */
    }

    decoder.media_type = (kal_uint16) MED_TYPE_GIF;
    decoder.file_size = size;
    decoder.blocking = is_blocking;
    decoder.callback = callback;

    decoder.display_width = (kal_uint16) resized_width;
    decoder.display_height = (kal_uint16) resized_height;

    decoder.use_disposal_method = use_disposal_method;

    decoder.start_x = ox;
    decoder.start_y = oy;

    decoder.image_clip_x1 = gdi_act_layer->clipx1;
    decoder.image_clip_y1 = gdi_act_layer->clipy1;
    decoder.image_clip_x2 = gdi_act_layer->clipx2;
    decoder.image_clip_y2 = gdi_act_layer->clipy2;

    decoder.image_buffer_p = gdi_act_layer->buf_ptr;
    decoder.image_buffer_size = gdi_act_layer->layer_size;
    decoder.image_buffer_width = gdi_act_layer->width;
    decoder.image_buffer_height = gdi_act_layer->height;

    if (gdi_act_layer_info->source_key_enable)
    {
        decoder.layer_src_key = gdi_act_layer_info->source_key;
    }
    else
    {
        decoder.layer_src_key = 0x01ffffff;
    }

    if (gdi_image_gif_cf_to_hw(gdi_act_layer->vcf, &decoder.image_buffer_format) != GDI_SUCCEED)
    {
        GDI_RETURN(GDI_FAILED);
    }

    decoder.gct_addr = gct_palette;
    decoder.lct_addr = lct_palette;
    decoder.frame_number = frame_number;
    decoder.transparent_enable = transparent_enable;
    decoder.gif_info = &gdi_image_hw_gif_v2_info;

    /* start decoding */

    ret = media_img_decode(MOD_MMI, &decoder);

    if (is_file && is_blocking)
    {
        DRM_close_file(file_handle);
    }

    if (ret != MED_RES_OK)
    {
        GDI_RETURN(GDI_FAILED);
    }

    if (is_blocking)
    {
        gdi_anim_set_delay(gdi_image_hw_gif_v2_info.delay_time * 10);
        gdi_image_gif_width = gdi_image_hw_gif_v2_info.lwidth;
        gdi_image_gif_height = gdi_image_hw_gif_v2_info.lheight;
         
        //if (is_file && gdi_act_layer_info->source_key_enable)
        //{
        //     gdi_act_replace_src_key(gdi_act_layer->buf_ptr,gdi_act_layer->width,ox,oy,gdi_act_layer->clipx1, gdi_act_layer->clipy1, gdi_act_layer->clipx2, gdi_act_layer->clipy2,gdi_act_layer_info->source_key,gdi_image_gif_width,gdi_image_gif_height);
        //}
    }
    /* Support Color Index Overlay */
    if (gdi_act_layer->vcf == GDI_COLOR_FORMAT_8)
    {
        int i = 0;
        U32 *active_palette;

        if (gdi_image_hw_gif_v2_info.LCT_flag)
        {
            active_palette = &lct_palette[0];
            gdi_image_gif_palette_size = gdi_image_hw_gif_v2_info.LCT_size;
        }
        else
        {
            active_palette = &gct_palette[0];
            gdi_image_gif_palette_size = gdi_image_hw_gif_v2_info.GCT_size;
        }
        for (i = 0; i < gdi_image_gif_palette_size; i++)
        {
            gdi_layer_set_palette(i, active_palette[i]);
        }

        if (gdi_image_hw_gif_v2_info.transparent_flag)
        {
            gdi_image_gif_transparent_index = (U16) gdi_image_hw_gif_v2_info.transparent_index;
        }

    }
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_hwgif_display_internal)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_hwgif_display
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src                     [?]         
 *  src_size                [IN]        
 *  ox                      [IN]        
 *  oy                      [IN]        
 *  frame_number            [IN]        
 *  use_disposal_method     [IN]        
 *  is_transparent          [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_hwgif_display(
            U8 *src,
            U32 src_size,
            S32 ox,
            S32 oy,
            U16 frame_number,
            BOOL use_disposal_method,
            BOOL *is_transparent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_hwgif_display)
    GDI_RESULT result = gdi_image_hwgif_display_internal(
                            src,
                            src_size,
                            ox,
                            oy,
                            0,
                            0,      /* no resize */
                            frame_number,
                            FALSE,  /* no src file */
                            FALSE,  /* no resize */
                            use_disposal_method,
                            TRUE,   /* blocking */
                            TRUE,   /* transpanent enable */
                            NULL /* no callback function */ );

    if (result >= 0)
    {
        *is_transparent = gdi_image_hw_gif_v2_info.transparent_flag;
    }
    GDI_RETURN(result);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_hwgif_display)
}
#endif /* GDI_USING_HW_GIF_V2 */ 


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_cf_to_hw
 * DESCRIPTION
 *  
 * PARAMETERS
 *  cf          [IN]        
 *  hwgif       [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_cf_to_hw(gdi_color_format cf, U8 *hwgif)
{
#ifdef GDI_USING_HW_GIF_V2
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (cf)
    {
        case GDI_COLOR_FORMAT_8:
            *hwgif = GIF_OUT_INDEX;
            break;
        case GDI_COLOR_FORMAT_16:
            *hwgif = GIF_OUT_RGB565;
            break;
        case GDI_COLOR_FORMAT_24:
            *hwgif = GIF_OUT_RGB888;
            break;
        default:
            return GDI_FAILED;  /* hardware gif decoder didn't support this color format */
    }
    return GDI_SUCCEED;

#else /* GDI_USING_HW_GIF_V2 */ 
    return GDI_FAILED;
#endif /* GDI_USING_HW_GIF_V2 */ 
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_cf_from_hw
 * DESCRIPTION
 *  
 * PARAMETERS
 *  hwcf        [IN]        
 *  cf          [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_cf_from_hw(U8 hwcf, gdi_color_format *cf)
{
#ifdef GDI_USING_HW_GIF_V2
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (hwcf)
    {
        case GIF_OUT_INDEX:
            *cf = GDI_COLOR_FORMAT_8;
            break;
        case GIF_OUT_RGB565:
            *cf = GDI_COLOR_FORMAT_16;
            break;
        case GIF_OUT_RGB888:
            *cf = GDI_COLOR_FORMAT_24;
            break;
        default:
            return GDI_FAILED;
    }
    return GDI_SUCCEED;
#else /* GDI_USING_HW_GIF_V2 */ 
    return GDI_FAILED;
#endif /* GDI_USING_HW_GIF_V2 */ 
}

