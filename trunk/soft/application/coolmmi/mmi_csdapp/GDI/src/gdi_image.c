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
 *  gdi_image.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI image drawing functions..
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
#include "gdi_util.h"

#include "gui.h"
#include "drm_gprot.h"

/* GraphicLib's header */
#ifdef __MMI_TVOUT__
#include "mdi_datatype.h"
#include "mdi_tv.h"
#endif /* __MMI_TVOUT__ */ 

#include "unicodexdcl.h"

// #include "fat_fs.h"
#include "med_api.h"    /* media task */
#include "med_main.h"

#include "debuginitdef.h"
#include "wrappergprot.h"
#include "ucs2prot.h"

const U16 *gdi_image_ext_name[] = 
{
    // #define GDI_IMAGE_TYPE_INVALID                               0
    (U16 *)L"",
    // #define GDI_IMAGE_TYPE_BMP                                   1
    (U16 *)L".bmp",
    // #define GDI_IMAGE_TYPE_BMP_SEQUENCE                          2
    (U16 *)L".bmp",
    // #define GDI_IMAGE_TYPE_GIF                                   3
    (U16 *)L".gif",
    // #define GDI_IMAGE_TYPE_DEVICE_BITMAP                         4
    (U16 *)L".pbm",
    // #define GDI_IMAGE_TYPE_DEVICE_BITMAP_SEQUENCE                5
    (U16 *)L".pbm",
    // #define GDI_IMAGE_TYPE_BMP_FILE                              6
    (U16 *)L".bmp",
    // #define GDI_IMAGE_TYPE_GIF_FILE                              7
    (U16 *)L".gif",
    // #define GDI_IMAGE_TYPE_WBMP_FILE                             8
    (U16 *)L".wbmp",
    // #define GDI_IMAGE_TYPE_JPG                                   9
    (U16 *)L".jpg",
    // #define GDI_IMAGE_TYPE_JPG_FILE                              10
    (U16 *)L".jpg",
    // #define GDI_IMAGE_TYPE_WBMP                                  11
    (U16 *)L".wbmp",
    // #define GDI_IMAGE_TYPE_AVI                                   12
    (U16 *)L".avi",
    // #define GDI_IMAGE_TYPE_AVI_FILE                              13
    (U16 *)L".avi",
    // #define GDI_IMAGE_TYPE_3GP                                   14
    (U16 *)L".3gp",
    // #define GDI_IMAGE_TYPE_3GP_FILE                              15
    (U16 *)L".3gp",
    // #define GDI_IMAGE_TYPE_MP4                                   16
    (U16 *)L".mp4",
    // #define GDI_IMAGE_TYPE_MP4_FILE                              17
    (U16 *)L".mp4",
    // #define GDI_IMAGE_TYPE_JPG_SEQUENCE                          18
    (U16 *)L".jpg",
    // #define GDI_IMAGE_TYPE_PNG                                   19
    (U16 *)L".png",
    // #define GDI_IMAGE_TYPE_PNG_FILE                              20
    (U16 *)L".png",
    // #define GDI_IMAGE_TYPE_PNG_SEQUENCE                          21
    (U16 *)L".png",
    // #define GDI_IMAGE_TYPE_DEVICE_BMP_FILE                       22
    (U16 *)L".pbm",
    // #define GDI_IMAGE_TYPE_BMP_FILE_OFFSET                       23
    (U16 *)L".bmp",
    // #define GDI_IMAGE_TYPE_GIF_FILE_OFFSET                       24
    (U16 *)L".gif",
    // #define GDI_IMAGE_TYPE_M3D                                   25
    (U16 *)L".m3d",
    // #define GDI_IMAGE_TYPE_M3D_FILE                              26
    (U16 *)L".m3d",
    // #define GDI_IMAGE_TYPE_SVG                                   27
    (U16 *)L".svg",
    // #define GDI_IMAGE_TYPE_SVG_FILE                              28
    (U16 *)L".svg",
    //#define GDI_IMAGE_TYPE_SWFLASH                                 29
    (U16 *)L".cmp",
    //#define GDI_IMAGE_TYPE_SWFLASH_FILE                            30
    (U16 *)L".cmp",
    //#define GDI_IMAGE_TYPE_JPG_FILE_OFFSET                        31
    (U16 *)L".jpg",
    //#define GDI_IMAGE_TYPE_PNG_FILE_OFFSET                        32
    (U16 *)L".png",
    //#define GDI_IMAGE_TYPE_DEVICE_BMP_FILE_OFFSET                 33
    (U16 *)L".pbm",
    //#define GDI_IMAGE_TYPE_WBMP_FILE_OFFSET                       34
    (U16 *)L".wbmp",
    //#define GDI_IMAGE_TYPE_M3D_FILE_OFFSET                        35
    (U16 *)L".m3d",
    //#define GDI_IMAGE_TYPE_SVG_FILE_OFFSET                        36
    (U16 *)L".svg",
    //#define GDI_IMAGE_TYPE_SWFLASH_FILE_OFFSET                    37
    (U16 *)L".cmp",

    //#define GDI_IMAGE_TYPE_SUM                                    38
    (U16 *)L""
};

#define GDI_IMAGE_ENCODER_JPEG_RESIZER_UPPER_BOUND (640*480*GDI_MAINLCD_BIT_PER_PIXEL)>>3

/****************************************************************************
* Local Variable
*****************************************************************************/
#if defined(GDI_USING_LAYER)
    #if (GDI_WORK_BUFFER_FORMAT == GDI_COLOR_FORMAT_32)
        #define GDI_WORK_BUFFER_SIZE (((GDI_LCD_WIDTH + 16) * (GDI_LCD_HEIGHT + 16) * 32) >> 3) 
    #elif (GDI_WORK_BUFFER_FORMAT == GDI_COLOR_FORMAT_24)
        #define GDI_WORK_BUFFER_SIZE (((GDI_LCD_WIDTH + 16) * (GDI_LCD_HEIGHT + 16) * 24) >> 3)
    #elif (GDI_WORK_BUFFER_FORMAT == GDI_COLOR_FORMAT_16)
        #define GDI_WORK_BUFFER_SIZE (((GDI_LCD_WIDTH + 16) * (GDI_LCD_HEIGHT + 16) * 16) >> 3)
    #else
        #define GDI_WORK_BUFFER_SIZE (((GDI_LCD_WIDTH + 16) * (GDI_LCD_HEIGHT + 16) * GDI_MAINLCD_BIT_PER_PIXEL) >> 3)
    #endif

U32 gdi_work_buffer[GDI_WORK_BUFFER_SIZE/4];
U32 gdi_work_buffer_size = GDI_WORK_BUFFER_SIZE;
#endif 

const gdi_image_codecs_struct gdi_image_codecs[GDI_IMAGE_TYPE_SUM + 1] = 
{
    // #define GDI_IMAGE_TYPE_INVALID                                       0
    {NULL, NULL, NULL, NULL, NULL, NULL},
    // #define GDI_IMAGE_TYPE_BMP                                           1
    {
     NULL,
     gdi_image_bmp_draw_handler,
     NULL,
     gdi_image_bmp_get_dimension_handler,
     NULL,
     NULL},
    // #define GDI_IMAGE_TYPE_BMP_SEQUENCE                          2
    {
     NULL,
     gdi_image_sequence_draw_handler,
     NULL,
     gdi_image_sequence_get_dimension_handler,
     gdi_image_sequence_get_pos_info_handler,
     NULL},
    // #define GDI_IMAGE_TYPE_GIF                                                   3
    {
     NULL,
     gdi_image_gif_draw_handler,
     NULL,
     gdi_image_gif_get_dimension_handler,
     gdi_image_gif_get_pos_info_handler,
     NULL},

    // #define GDI_IMAGE_TYPE_DEVICE_BITMAP                         4
    {
     NULL,
     gdi_image_dev_bmp_draw_handler,
     NULL,
     gdi_image_dev_bmp_get_dimension_handler,
     NULL,
     NULL,
     },
    // #define GDI_IMAGE_TYPE_DEVICE_BITMAP_SEQUENCE        5
    {
     NULL,
     gdi_image_sequence_draw_handler,
     NULL,
     gdi_image_sequence_get_dimension_handler,
     gdi_image_sequence_get_pos_info_handler,
     NULL},

    // #define GDI_IMAGE_TYPE_BMP_FILE                                      6
    {
     NULL,
     gdi_image_bmp_draw_handler,
     NULL,
     gdi_image_bmp_get_dimension_handler,
     NULL,
     gdi_image_is_bmp_file,
     },
    // #define GDI_IMAGE_TYPE_GIF_FILE                                      7
    {
     NULL,
     gdi_image_gif_draw_handler,
     NULL,
     gdi_image_gif_get_dimension_handler,
     gdi_image_gif_get_pos_info_handler,
     gdi_image_is_gif_file},
    // #define GDI_IMAGE_TYPE_WBMP_FILE                                     8
    {
     NULL,
     gdi_image_wbmp_draw_handler,
     NULL,
     gdi_image_wbmp_get_dimension_handler,
     NULL,
     gdi_image_is_wbmp_file},
    // #define GDI_IMAGE_TYPE_JPG                                                   9
    {
     NULL,
     gdi_image_jpeg_draw_handler,
     NULL,
     gdi_image_jpeg_get_dimension_handler,
     NULL,
     NULL},
    // #define GDI_IMAGE_TYPE_JPG_FILE                                      10
    {
     NULL,
     gdi_image_jpeg_draw_handler,
     NULL,
     gdi_image_jpeg_get_dimension_handler,
     NULL,
     gdi_image_is_jpeg_file},
    // #define GDI_IMAGE_TYPE_WBMP                                          11
    {
     NULL,
     gdi_image_wbmp_draw_handler,
     NULL,
     gdi_image_wbmp_get_dimension_handler,
     NULL,
     NULL},
    // #define GDI_IMAGE_TYPE_AVI                                                   12
    {NULL, NULL, NULL, NULL, NULL, NULL},
    // #define GDI_IMAGE_TYPE_AVI_FILE                                      13
    {NULL, NULL, NULL, NULL, NULL, NULL},
    // #define GDI_IMAGE_TYPE_3GP                                                   14
    {NULL, NULL, NULL, NULL, NULL, NULL},
    // #define GDI_IMAGE_TYPE_3GP_FILE                                      15
    {NULL, NULL, NULL, NULL, NULL, NULL},
    // #define GDI_IMAGE_TYPE_MP4                                           16
    {NULL, NULL, NULL, NULL, NULL, NULL},
    // #define GDI_IMAGE_TYPE_MP4_FILE                                      17
    {NULL, NULL, NULL, NULL, NULL, NULL},
    // #define GDI_IMAGE_TYPE_JPG_SEQUENCE                          18
    {
     NULL,
     gdi_image_sequence_draw_handler,
     NULL,
     gdi_image_sequence_get_dimension_handler,
     gdi_image_sequence_get_pos_info_handler,
     NULL},
    // #define GDI_IMAGE_TYPE_PNG                                           19
    {
     NULL,
     gdi_image_png_draw_handler,
     NULL,
     gdi_image_png_get_dimension_handler,
     NULL,
     NULL},
    // #define GDI_IMAGE_TYPE_PNG_FILE                                      20
    {
     NULL,
     gdi_image_png_draw_handler,
     NULL,
     gdi_image_png_get_dimension_handler,
     NULL,
     gdi_image_is_png_file},
    // #define GDI_IMAGE_TYPE_PNG_SEQUENCE                          21
    {
     NULL,
     gdi_image_sequence_draw_handler,
     NULL,
     gdi_image_sequence_get_dimension_handler,
     gdi_image_sequence_get_pos_info_handler,
     NULL},
    // #define GDI_IMAGE_TYPE_DEVICE_BMP_FILE                       22
    {
     NULL,
     gdi_image_dev_bmp_draw_handler,
     NULL,
     gdi_image_dev_bmp_get_dimension_handler,
     NULL,
     gdi_image_is_device_bmp_file},
    // #define GDI_IMAGE_TYPE_BMP_FILE_OFFSET                       23
    {
     NULL,
     gdi_image_virtualfile_draw_handler,
     NULL,
     gdi_image_virtualfile_get_dimension_handler,
     gdi_image_virtualfile_get_pos_info_handler,
     NULL},
    // #define GDI_IMAGE_TYPE_GIF_FILE_OFFSET                       24
    {
     NULL,
     gdi_image_virtualfile_draw_handler,
     NULL,
     gdi_image_virtualfile_get_dimension_handler,
     gdi_image_virtualfile_get_pos_info_handler,
     NULL},
    //#define GDI_IMAGE_TYPE_M3D                                 25
    //#define GDI_IMAGE_TYPE_M3D_FILE                         26
 #ifdef GDI_USING_M3D
    {
     gdi_image_m3d_draw_before_handler,
     gdi_image_m3d_draw_handler,
     gdi_image_m3d_draw_after_handler,
     gdi_image_m3d_get_dimension_handler,
     gdi_image_m3d_get_pos_info_handler,
     NULL},
    {
     gdi_image_m3d_draw_before_handler,
     gdi_image_m3d_draw_handler,
     gdi_image_m3d_draw_after_handler,
     gdi_image_m3d_get_dimension_handler,
     gdi_image_m3d_get_pos_info_handler,
     gdi_image_is_m3d_file},
 #else /* GDI_USING_M3D */ 
    {NULL, NULL, NULL, NULL, NULL, NULL},
    {NULL, NULL, NULL, NULL, NULL, NULL},
 #endif /* GDI_USING_M3D */ 

    // #define GDI_IMAGE_TYPE_SVG                                   27
    // #define GDI_IMAGE_TYPE_SVG_FILE                              28
#ifdef GDI_USING_SVG
    {
        gdi_image_svg_draw_before_handler,
        gdi_image_svg_draw_handler,
        gdi_image_svg_draw_after_handler,
        gdi_image_svg_get_dimension_handler,
        gdi_image_svg_get_pos_info_handler,
        NULL
    },
    {
        gdi_image_svg_draw_before_handler,
        gdi_image_svg_draw_handler,
        gdi_image_svg_draw_after_handler,
        gdi_image_svg_get_dimension_handler,
        gdi_image_svg_get_pos_info_handler,
        gdi_image_is_svg_file
    },
#else
    {NULL, NULL, NULL, NULL, NULL, NULL},
    {NULL, NULL, NULL, NULL, NULL, NULL},
#endif
    //#define GDI_IMAGE_TYPE_MFLASH                                 29
    //#define GDI_IMAGE_TYPE_MFLASH_FILE                            30
    {NULL, NULL, NULL, NULL, NULL, NULL},
    {NULL, NULL, NULL, NULL, NULL, NULL},
        
    //#define GDI_IMAGE_TYPE_JPG_FILE_OFFSET                        31
    //#define GDI_IMAGE_TYPE_PNG_FILE_OFFSET                        32
    //#define GDI_IMAGE_TYPE_DEVICE_BMP_FILE_OFFSET                 33
    //#define GDI_IMAGE_TYPE_WBMP_FILE_OFFSET                       34
    //#define GDI_IMAGE_TYPE_M3D_FILE_OFFSET                        35
    //#define GDI_IMAGE_TYPE_SVG_FILE_OFFSET                        36
    //#define GDI_IMAGE_TYPE_SWFLASH_FILE_OFFSET                    37
    {NULL,gdi_image_virtualfile_draw_handler,NULL,gdi_image_virtualfile_get_dimension_handler,gdi_image_virtualfile_get_pos_info_handler,NULL},
    {NULL,gdi_image_virtualfile_draw_handler,NULL,gdi_image_virtualfile_get_dimension_handler,gdi_image_virtualfile_get_pos_info_handler,NULL},
    {NULL,gdi_image_virtualfile_draw_handler,NULL,gdi_image_virtualfile_get_dimension_handler,gdi_image_virtualfile_get_pos_info_handler,NULL},
    {NULL,gdi_image_virtualfile_draw_handler,NULL,gdi_image_virtualfile_get_dimension_handler,gdi_image_virtualfile_get_pos_info_handler,NULL},
    {NULL,gdi_image_virtualfile_draw_handler,NULL,gdi_image_virtualfile_get_dimension_handler,gdi_image_virtualfile_get_pos_info_handler,NULL},
    {NULL,gdi_image_virtualfile_draw_handler,NULL,gdi_image_virtualfile_get_dimension_handler,gdi_image_virtualfile_get_pos_info_handler,NULL},
    {NULL,gdi_image_virtualfile_draw_handler,NULL,gdi_image_virtualfile_get_dimension_handler,gdi_image_virtualfile_get_pos_info_handler,NULL},
    // #define GDI_IMAGE_TYPE_SUM                                   38
    {NULL, NULL, NULL, NULL, NULL, NULL}
};

BOOL gdi_memory_output = FALSE;
U16 gdi_memory_output_width;
U16 gdi_memory_output_height;
U32 gdi_memory_output_buffer_address;
U32 gdi_memory_output_buffer_size;
static U32 gdi_image_codec_flag_stack[8] = {0};
static U32 gdi_image_codec_flag_index = 0;
#ifdef __NATIVE_JAVA__
U32	gdi_image_decoder_frame_buf[(((240+16)*(320+16)*GDI_MAINLCD_BIT_PER_PIXEL)>>3)/4];
#else
#if defined(__PROJECT_GALLITE_C01__)
U32	gdi_image_decoder_frame_buf[(((GDI_LCD_WIDTH)*(GDI_LCD_HEIGHT)*GDI_MAINLCD_BIT_PER_PIXEL)>>3)/4];
#else
U32	gdi_image_decoder_frame_buf[(((GDI_LCD_WIDTH+16)*(GDI_LCD_HEIGHT+16)*GDI_MAINLCD_BIT_PER_PIXEL)>>3)/4];
#endif
#endif
/****************************************************************************
* Gif Anim Internal Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gdi_image_read_length
 * DESCRIPTION
 *  Read the image length from  image header .
 * PARAMETERS
 *  image       [IN]        
 * RETURNS
 *  S32
 *****************************************************************************/
S32 gdi_image_read_length(U8 *image)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 rvalue = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (image == NULL)
    {
        return 0;
    }
    else
    {
        rvalue |= (S32) (image[2]);
        rvalue |= (S32) ((S32) image[3] << 8);
        rvalue |= (S32) ((S32) image[4] << 16);
        rvalue |= (S32) ((S32) image[5] << 24);
        return (((S32) rvalue));
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_init
 * DESCRIPTION
 *  Init GDI Iamge module.
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_init)

#ifdef GDI_USING_MEDIA_TASK_DECODER
    gdi_image_decoder_init();
#endif 
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_init)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_id
 * DESCRIPTION
 *  call this function after all drawing fucntions.
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  image_id        [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
 #include "wgui_categories_defs.h"  

GDI_RESULT gdi_image_draw_id(S32 offset_x, S32 offset_y, U16 image_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
/* wangrui clean code 20090112*/
//   if (IMG_DIALING_SCREEN == image_id)
//   {
//	return gdi_image_codec_draw(
//		        GDI_IMAGE_SRC_FROM_RESOURCE,
//		        (U8*) keypadbg ,
//		        GDI_IMAGE_TYPE_INVALID,
//		        0,
//		        offset_x,
//		        offset_y,
//		        0,
//		        0,
//		        0,
//		        0);
//   }
//   else
   //{
	return gdi_image_codec_draw(
	            GDI_IMAGE_SRC_FROM_RESOURCE,
	            (U8*)GetImage(image_id),
	            GDI_IMAGE_TYPE_INVALID,
	            0,
	            offset_x,
	            offset_y,
	            0,
	            0,
	            0,
	            0);  
//	}    ///* wangrui clean code 20090112*/
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_id_with_transparent_color
 * DESCRIPTION
 *  
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  image_id        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_draw_id_with_transparent_color(S32 offset_x, S32 offset_y, U16 image_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            (U8*) GetImage(image_id),
            GDI_IMAGE_TYPE_INVALID,
            0,
            offset_x,
            offset_y,
            0,
            0,
            GDI_IMAGE_CODEC_FLAG_DISABLE_SRC_KEY,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw
 * DESCRIPTION
 *  call this function after all drawing fucntions.
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  image_ptr       [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_draw(S32 offset_x, S32 offset_y, U8 *image_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            image_ptr,
            GDI_IMAGE_TYPE_INVALID,
            0,
            offset_x,
            offset_y,
            0,
            0,
            0,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  offset_x                [IN]        
 *  offset_y                [IN]        
 *  image_data_ptr          [?]         
 *  image_identifier        [IN]        
 *  image_length            [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_draw_ext(S32 offset_x, S32 offset_y, U8 *image_data_ptr, U8 image_identifier, S32 image_length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_MEMORY,
            image_data_ptr,
            image_identifier,
            image_length,
            offset_x,
            offset_y,
            0,
            0,
            0,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_file
 * DESCRIPTION
 *  Draw image from file
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  image_name      [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_draw_file(S32 offset_x, S32 offset_y, S8 *image_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_FILE,
            (U8*) image_name,
            GDI_IMAGE_TYPE_INVALID,
            0,
            offset_x,
            offset_y,
            0,
            0,
            0,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_resized_id
 * DESCRIPTION
 *  Draw resized image from id
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  resized_width       [IN]        
 *  resized_height      [IN]        
 *  image_id            [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_draw_resized_id(S32 offset_x, S32 offset_y, S32 resized_width, S32 resized_height, U16 image_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            (U8*) GetImage(image_id),
            GDI_IMAGE_TYPE_INVALID,
            0,
            offset_x,
            offset_y,
            resized_width,
            resized_height,
            0,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_resized
 * DESCRIPTION
 *  Draw resized image from memory
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  resized_width       [IN]        
 *  resized_height      [IN]        
 *  image_ptr           [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_draw_resized(S32 offset_x, S32 offset_y, S32 resized_width, S32 resized_height, U8 *image_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            image_ptr,
            GDI_IMAGE_TYPE_INVALID,
            0,
            offset_x,
            offset_y,
            resized_width,
            resized_height,
            0,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_resized_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  offset_x                [IN]        
 *  offset_y                [IN]        
 *  resized_width           [IN]        
 *  resized_height          [IN]        
 *  image_data_ptr          [?]         
 *  image_identifier        [IN]        
 *  image_length            [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_draw_resized_ext(
            S32 offset_x,
            S32 offset_y,
            S32 resized_width,
            S32 resized_height,
            U8 *image_data_ptr,
            U8 image_identifier,
            S32 image_length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_MEMORY,
            image_data_ptr,
            image_identifier,
            image_length,
            offset_x,
            offset_y,
            resized_width,
            resized_height,
            0,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_resized_file
 * DESCRIPTION
 *  Draw resized image from file
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  resized_width       [IN]        
 *  resized_height      [IN]        
 *  image_name          [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_draw_resized_file(
            S32 offset_x,
            S32 offset_y,
            S32 resized_width,
            S32 resized_height,
            S8 *image_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_FILE,
            (U8*) image_name,
            GDI_IMAGE_TYPE_INVALID,
            0,
            offset_x,
            offset_y,
            resized_width,
            resized_height,
            0,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_animation_single_frame_id
 * DESCRIPTION
 *  draw a single frame of a animation
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  image_id            [IN]        
 *  single_frame        [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_draw_animation_single_frame_id(S32 offset_x, S32 offset_y, U16 image_id, U16 single_frame)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            (U8*) GetImage(image_id),
            GDI_IMAGE_TYPE_INVALID,
            0,
            offset_x,
            offset_y,
            0,
            0,
            0,
            (U32) single_frame);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_draw_animation_single_frame
 * DESCRIPTION
 *  draw a single frame of a animation
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  image_ptr           [?]         
 *  single_frame        [IN]        
 *  image_id(?)         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_draw_animation_single_frame(S32 offset_x, S32 offset_y, U8 *image_ptr, U16 single_frame)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_draw(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            image_ptr,
            GDI_IMAGE_TYPE_INVALID,
            0,
            offset_x,
            offset_y,
            0,
            0,
            0,
            (U32) single_frame);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_dimension_id
 * DESCRIPTION
 *  stop a animation by handle
 * PARAMETERS
 *  image_id        [IN]        
 *  width           [OUT]       
 *  height          [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_get_dimension_id(U16 image_id, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_get_dimension(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            (U8*) GetImage(image_id),
            GDI_IMAGE_TYPE_INVALID,
            0,
            width,
            height,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_dimension_file
 * DESCRIPTION
 *  get image dimension from a file
 * PARAMETERS
 *  image_name      [?]         
 *  width           [OUT]       
 *  height          [OUT]       
 *  word(?)          [IN]        Image_id
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_get_dimension_file(S8 *image_name, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_get_dimension(
            GDI_IMAGE_SRC_FROM_FILE,
            (U8*) image_name,
            GDI_IMAGE_TYPE_INVALID,
            0,
            width,
            height,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_dimension
 * DESCRIPTION
 *  get image dimesion from resource
 * PARAMETERS
 *  image_ptr       [IN]        
 *  width           [OUT]       
 *  height          [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_get_dimension(U8 *image_ptr, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_get_dimension(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            image_ptr,
            GDI_IMAGE_TYPE_INVALID,
            0,
            width,
            height,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_dimension_mem
 * DESCRIPTION
 *  get image dimesion from memory
 * PARAMETERS
 *  image_ptr       [IN]        
 *  width           [OUT]       
 *  height          [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_get_dimension_mem(U8 img_type,U8 *img_ptr,S32 img_size,S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_get_dimension(
            GDI_IMAGE_SRC_FROM_MEMORY,
            img_ptr,
            img_type,
            img_size,
            width,
            height,
            0);
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_frame_count_id
 * DESCRIPTION
 *  get frame count of an animation from id
 * PARAMETERS
 *  image_id        [IN]        
 *  frame_count     [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_get_frame_count_id(U16 image_id, S32 *frame_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_get_frame_count(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            (U8*) GetImage(image_id),
            GDI_IMAGE_TYPE_INVALID,
            0,
            frame_count,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_frame_count
 * DESCRIPTION
 *  get frame count of an animation from memory
 * PARAMETERS
 *  image_ptr       [IN]        
 *  frame_count     [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_get_frame_count(U8 *image_ptr, S32 *frame_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_get_frame_count(
            GDI_IMAGE_SRC_FROM_RESOURCE,
            image_ptr,
            GDI_IMAGE_TYPE_INVALID,
            0,
            frame_count,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_frame_count_file
 * DESCRIPTION
 *  get frame count of an animation from file
 * PARAMETERS
 *  image_ptr       [IN]        
 *  frame_count     [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_get_frame_count_file(U8 *image_ptr, S32 *frame_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_codec_get_frame_count(
            GDI_IMAGE_SRC_FROM_FILE,
            image_ptr,
            GDI_IMAGE_TYPE_INVALID,
            0,
            frame_count,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_type_from_id
 * DESCRIPTION
 *  check gdi decoder's capability
 * PARAMETERS
 *  image_id        [IN]        
 * RETURNS
 *  U16
 *****************************************************************************/
U16 gdi_image_get_type_from_id(U16 image_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_get_type_from_id)
    U8 image_identifier;
    PU8 image_ptr = (PU8) GetImage(image_id);

    if (image_ptr == NULL)
    {
        GDI_RETURN(GDI_IMAGE_TYPE_INVALID);
    }

    image_identifier = image_ptr[0];

    GDI_RETURN((U16) image_identifier);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_get_type_from_id)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_type_from_file_internal
 * DESCRIPTION
 *  get image type from file
 * PARAMETERS
 *  file_name       [IN]        
 * RETURNS
 *  U16
 *****************************************************************************/
U16 gdi_image_get_type_from_file_internal(S8 *file_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	S32 type;
    U16 *ext;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /* null string */
    if (file_name == NULL)
    {
        return (GDI_IMAGE_TYPE_INVALID);
    }

    /* search the '.' from end of file_name */
    {
        U16 *pos = (U16*) file_name;

        ext = NULL;
        while (*pos != 0)
        {
            if (*pos == (U16)_T_MMI'.')
            {
                ext = pos + 1;
            }
            pos++;
        }
    }

    if (ext)    /* have ext name */
    {
		for(type=1;type<GDI_IMAGE_TYPE_SUM;type++)
			if(gdi_image_codecs[type].is_match_file_type)
				if(gdi_ucs2_stricmp((U16*)ext,(U16*)(gdi_image_ext_name[type]+1))==0)
					if(gdi_image_codecs[type].is_match_file_type((U8*)file_name))
						return type;
    }

	// try again without ext information
	for(type=1;type<GDI_IMAGE_TYPE_SUM;type++)
		if(gdi_image_codecs[type].is_match_file_type)
			if(gdi_image_codecs[type].is_match_file_type((U8*)file_name))
				return type;

    return (GDI_IMAGE_TYPE_INVALID);

#undef IC
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_type_from_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  file_name       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U16 gdi_image_get_type_from_file(S8 *file_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_get_type_from_file)
    GDI_RETURN(gdi_image_get_type_from_file_internal(file_name));
    GDI_EXIT_CRITICAL_SECTION(gdi_image_get_type_from_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_ext_name_from_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  file_name       [?]     
 * RETURNS
 *  
 *****************************************************************************/
const U16 *gdi_image_get_ext_name_from_file(S8 *file_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_get_ext_name_from_file)
    GDI_RETURN_TYPE(const U16 *, gdi_image_ext_name[gdi_image_get_type_from_file_internal(file_name)]);

    GDI_EXIT_CRITICAL_SECTION(gdi_image_get_ext_name_from_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_ext_name_from_file_type
 * DESCRIPTION
 *  
 * PARAMETERS
 *  file_type       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
const U16 *gdi_image_get_ext_name_from_file_type(U16 file_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_get_ext_name_from_file_type)
    GDI_ASSERT(file_type < GDI_IMAGE_TYPE_SUM);
    GDI_RETURN_TYPE(const U16 *, gdi_image_ext_name[file_type]);

    GDI_EXIT_CRITICAL_SECTION(gdi_image_get_ext_name_from_file_type)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_output_another_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  enable      [IN]        
 *  src         [?]         
 *  size        [IN]        
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_output_another_buffer(BOOL enable, U8 *src, U32 size, U16 width, U16 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_output_another_buffer)
#ifdef GDI_HW_JPEG_SUPPORT_OUTPUT_ANOTHER_BUFFER
    gdi_memory_output = enable;
    gdi_memory_output_width = width;
    gdi_memory_output_height = height;
    gdi_memory_output_buffer_size = size;
    gdi_memory_output_buffer_address = (U32) src;
    GDI_RETURN(GDI_SUCCEED);
#else /* GDI_HW_JPEG_SUPPORT_OUTPUT_ANOTHER_BUFFER */ 
    GDI_RETURN(GDI_FAILED);
#endif /* GDI_HW_JPEG_SUPPORT_OUTPUT_ANOTHER_BUFFER */ 
    GDI_EXIT_CRITICAL_SECTION(gdi_image_output_another_buffer)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_buf_len_from_id
 * DESCRIPTION
 *  
 * PARAMETERS
 *  image_id        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
S32 gdi_image_get_buf_len_from_id(U16 image_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_get_buf_len_id)
    S32 image_length;
    U8 image_identifier;
    PU8 image_ptr = (PU8) GetImage(image_id);

    image_identifier = image_ptr[0];

    image_length = gdi_image_read_length(image_ptr);

    if (image_identifier == GDI_IMAGE_TYPE_JPG_SEQUENCE
        || image_identifier == GDI_IMAGE_TYPE_PNG_SEQUENCE || image_identifier == GDI_IMAGE_TYPE_BMP_SEQUENCE)
    {
        U32 *len = (U32*) (image_ptr + 8 + 4);

        image_length = *len;
    }
    GDI_RETURN(image_length);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_get_buf_len_id)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_get_buf_ptr_from_id
 * DESCRIPTION
 *  
 * PARAMETERS
 *  image_id        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 *gdi_image_get_buf_ptr_from_id(U16 image_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_get_buf_ptr_id)
    U8 image_identifier;
    PU8 image_ptr = (PU8) GetImage(image_id);

    image_identifier = image_ptr[0];

    if (image_identifier == GDI_IMAGE_TYPE_GIF)
    {
        GDI_RETURN_TYPE(PU8, image_ptr + 8);
    }
    else if (image_identifier == GDI_IMAGE_TYPE_JPG_SEQUENCE
             || image_identifier == GDI_IMAGE_TYPE_PNG_SEQUENCE || image_identifier == GDI_IMAGE_TYPE_BMP_SEQUENCE)
    {
        GDI_RETURN_TYPE(PU8, image_ptr + 8 + image_ptr[1] * 12);
    }
    else
    {
        GDI_RETURN_TYPE(PU8, image_ptr + 6);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_image_get_buf_ptr_id)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_encode_file_to_jpeg
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_file_name       [?]         
 *  dest_file_name      [?]         
 *  dest_width          [IN]        
 *  dest_height         [IN]        
 *  buf_ptr             [?]         
 *  buf_size            [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_encode_file_to_jpeg(
            S8 *src_file_name,
            S8 *dest_file_name,
            S32 dest_width,
            S32 dest_height,
            U8 *buf_ptr,
            S32 buf_size)
{
    GDI_ENTER_CRITICAL_SECTION(gdi_image_encode_file_to_jpeg)
#if defined(JPG_ENCODE) && defined(MMI_ON_HARDWARE_P)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    img_encode_struct encode_data;
    gdi_handle dest_layer;
    U32 file_size;
    S32 ret;
    GDI_RESULT decode_result = GDI_FAILED;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (buf_size > GDI_IMAGE_ENCODER_JPEG_RESIZER_UPPER_BOUND)
    {
        GDI_RETURN(GDI_IMAGE_ENCODER_ERR_MEMORY_NOT_ENOUGH);
    }

    ASSERT(buf_ptr != NULL);

    gdi_layer_create_using_outside_memory(0, 0, dest_width, dest_height, &dest_layer, (PU8) buf_ptr, (S32) buf_size);
    gdi_layer_push_and_set_active(dest_layer);
    gdi_layer_clear(GDI_COLOR_WHITE);
    gdi_image_clear_work_buffer();
    decode_result = gdi_image_draw_resized_file(0, 0, dest_width, dest_height, src_file_name);
    gdi_layer_pop_and_restore_active();
     
    gdi_layer_free(dest_layer);
    /* Check decode statre if failed return directly */
    if (GDI_SUCCEED != decode_result)
    {
        GDI_RETURN(decode_result);
    }
    encode_data.image_width = dest_width;
    encode_data.image_height = dest_height;

    encode_data.image_quality = CAM_JPG_QTY_HIGH;
    encode_data.image_buffer_p = (void*)buf_ptr;
    encode_data.image_buffer_size = buf_size;

    encode_data.target_width = dest_width;
    encode_data.target_height = dest_height;
    encode_data.media_mode = MED_MODE_FILE;
    encode_data.media_type = MED_TYPE_JPG;

    encode_data.data = (void*)dest_file_name;
    encode_data.file_size_p = (kal_uint32*) & file_size;
    encode_data.blocking = (kal_bool) TRUE;         /* blocking */
    encode_data.seq_num = (kal_uint16) 0;           /* not used */
    encode_data.file_buffer_len = (kal_uint32) 0;   /* not used */
    encode_data.gray_mode = 0;

#if (defined(MT6228) || defined(MT6229))
    encode_data.thumbnail_mode = FALSE;
    encode_data.overlay_frame_mode = FALSE;
#endif   

    ret = media_img_encode(MOD_MMI, &encode_data);

    if (ret == MED_RES_OK)
    {
        GDI_RETURN(GDI_SUCCEED);
    }
    else if (ret == MED_RES_DISC_FULL)
    {
        GDI_RETURN(GDI_IMAGE_ENCODER_ERR_DISK_FULL);
    }
    else if (ret == MED_RES_WRITE_PROTECTION)
    {
        GDI_RETURN(GDI_IMAGE_ENCODER_ERR_WRITE_PROTECTION);
    }
    else if (ret == MED_RES_NO_DISC)
    {
        GDI_RETURN(GDI_IMAGE_ENCODER_ERR_NO_DISK);
    }
    else
    {
        GDI_RETURN(GDI_FAILED);
    }

#else /* defined(JPG_ENCODE) && defined(MMI_ON_HARDWARE_P) */ 
    GDI_RETURN(GDI_FAILED);
#endif /* defined(JPG_ENCODE) && defined(MMI_ON_HARDWARE_P) */ 
    GDI_EXIT_CRITICAL_SECTION(gdi_image_encode_file_to_jpeg)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_parse_resource_internal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  res_src         [?]         
 *  data_ptr        [IN]        
 *  img_type        [?]         
 *  img_size        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gdi_image_parse_resource_internal(U8 *res_src, U8 **data_ptr, U8 *img_type, S32 *img_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *img_type = res_src[0];
    *img_size =
        (S32) (res_src[2]) | (S32) ((S32) res_src[3] << 8) | (S32) ((S32) res_src[4] << 16) | (S32) ((S32) res_src[5] <<
                                                                                                     24);

    switch (*img_type)
    {
        case GDI_IMAGE_TYPE_GIF:
        case GDI_IMAGE_TYPE_M3D:
        case GDI_IMAGE_TYPE_PNG:
        case GDI_IMAGE_TYPE_JPG_SEQUENCE:
        case GDI_IMAGE_TYPE_PNG_SEQUENCE:
        case GDI_IMAGE_TYPE_BMP_SEQUENCE:
            *data_ptr = res_src + 8;
            break;
	    case GDI_IMAGE_TYPE_BMP_FILE_OFFSET:
	    case GDI_IMAGE_TYPE_GIF_FILE_OFFSET:
        case GDI_IMAGE_TYPE_JPG_FILE_OFFSET:
        case GDI_IMAGE_TYPE_PNG_FILE_OFFSET:
        case GDI_IMAGE_TYPE_DEVICE_BMP_FILE_OFFSET:
        case GDI_IMAGE_TYPE_WBMP_FILE_OFFSET:
        case GDI_IMAGE_TYPE_M3D_FILE_OFFSET:
        case GDI_IMAGE_TYPE_SVG_FILE_OFFSET:
        case GDI_IMAGE_TYPE_SWFLASH_FILE_OFFSET:
	    	*img_size = (((S32)res_src[2])<<16)  |  (((S32)res_src[3])<<8)  |  (((S32)res_src[4]));
    		*data_ptr = res_src;
		    break;
        default:
            *data_ptr = res_src + 6;
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_parse_info
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_type        [IN]        
 *  img_src         [?]         
 *  data_ptr        [IN]        
 *  img_type        [?]         
 *  img_size        [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_parse_info(
            gdi_image_src_enum src_type,
            U8 *img_src,
            U8 **data_ptr,
            U8 *img_type,
            S32 *img_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (img_src == NULL)
    {
        return (GDI_IMAGE_ERR_NULL_IMG_PTR);
    }

    /* Detect image type if unknown */

    *data_ptr = img_src;    /* default data is from begin of img_src */
    switch (src_type)
    {
        case GDI_IMAGE_SRC_FROM_RESOURCE:
            gdi_image_parse_resource_internal(img_src, data_ptr, img_type, img_size);
            break;
        case GDI_IMAGE_SRC_FROM_MEMORY:
            if (*img_type == GDI_IMAGE_TYPE_INVALID)
            {
                return (GDI_IMAGE_ERR_INVALID_IMG_TYPE);
            }
            break;
        case GDI_IMAGE_SRC_FROM_FILE:
            if (*img_type == GDI_IMAGE_TYPE_INVALID)
            {
                *img_type = gdi_image_get_type_from_file_internal((S8*) img_src);
            }
            break;
        default:
            GDI_ASSERT(0);  /* unknown src type */
    }
    return (GDI_SUCCEED);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_codec_get_dimension
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_type        [IN]        
 *  img_src         [?]         
 *  img_type        [IN]        
 *  img_size        [IN]        
 *  width           [?]         
 *  height          [?]         
 *  flag            [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_codec_get_dimension(
            gdi_image_src_enum src_type,
            U8 *img_src,
            U8 img_type,
            S32 img_size,
            S32 *width,
            S32 *height,
            U32 flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_codec_get_dimension)
    U8 *data_ptr;
    GDI_RESULT ret;
    const gdi_image_codecs_struct *codec;

    ret = gdi_image_parse_info(src_type, img_src, &data_ptr, &img_type, &img_size);
    if (ret != GDI_SUCCEED)
    {
        GDI_RETURN(ret);
    }

    if (GDI_IMAGE_TYPE_INVALID == img_type || img_type >= GDI_IMAGE_TYPE_SUM)
    {
        GDI_RETURN(GDI_IMAGE_ERR_INVALID_IMG_TYPE);
    }

    codec = &gdi_image_codecs[img_type];

    if (src_type == GDI_IMAGE_SRC_FROM_FILE)
    {
        flag |= GDI_IMAGE_CODEC_FLAG_IS_FILE;
    }

    ret = GDI_IMAGE_ERR_NO_DECODER_SUPPORT;
    *width = *height = 0;
    if (codec->get_dimension)
        ret = codec->get_dimension(flag, data_ptr, img_size, width, height);
    
    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_codec_get_dimension)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_codec_get_frame_count
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_type        [IN]        
 *  img_src         [?]         
 *  img_type        [IN]        
 *  img_size        [IN]        
 *  frame_count     [?]         
 *  flag            [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_codec_get_frame_count(
            gdi_image_src_enum src_type,
            U8 *img_src,
            U8 img_type,
            S32 img_size,
            S32 *frame_count,
            U32 flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_codec_get_frame_count)
    U8 *data_ptr;
    GDI_RESULT ret;
    const gdi_image_codecs_struct *codec;
    gdi_anim_pos_type_enum pos_type;

    ret = gdi_image_parse_info(src_type, img_src, &data_ptr, &img_type, &img_size);
    if (ret != GDI_SUCCEED)
    {
        GDI_RETURN(ret);
    }

    if (GDI_IMAGE_TYPE_INVALID == img_type || img_type >= GDI_IMAGE_TYPE_SUM)
    {
        GDI_RETURN(GDI_IMAGE_ERR_INVALID_IMG_TYPE);
    }

    codec = &gdi_image_codecs[img_type];

    if (src_type == GDI_IMAGE_SRC_FROM_FILE)
    {
        flag |= GDI_IMAGE_CODEC_FLAG_IS_FILE;
    }

    ret = GDI_SUCCEED;  /* normal is single frame if no decoder */
    if (codec->get_pos_info)
    {
        ret = codec->get_pos_info(flag, data_ptr, img_size, FALSE, frame_count, &pos_type);
    }
    else
    {
        *frame_count = 1;
    }

    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_codec_get_frame_count)
}

/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-11 15:14 */
U32 kal_ticks_to_milli_secs (U32 ticks )
{
  /*wangrui modify begin 20081121 for fix 10429  */
   return(ticks*0.06103515625);
  /*wangrui modify end*/
}
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-11 15:14 */


/*****************************************************************************
 * FUNCTION
 *  gdi_image_codec_draw
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_type        [IN]        
 *  img_src         [?]         
 *  img_type        [IN]        
 *  img_size        [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  w               [IN]        
 *  h               [IN]        
 *  flag            [IN]        
 *  frame_pos       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
  extern U8 __MAINLCD__MAINMENU__MATRIX__MM_BG1_GIF[];

GDI_RESULT gdi_image_codec_draw(
            gdi_image_src_enum src_type,
            U8 *img_src,
            U8 img_type,
            S32 img_size,
            S32 x,
            S32 y,
            S32 w,
            S32 h,
            U32 flag,
            U32 frame_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_codec_draw)
    U8 *data_ptr;
    GDI_RESULT ret,ret1;
    const gdi_image_codecs_struct *codec;
	U32 start_time, end_time;
    U32 decode_ms = 0;
    U32 delay_time = 0;

    ret = gdi_image_parse_info(src_type, img_src, &data_ptr, &img_type, &img_size);
    if (ret != GDI_SUCCEED)
    {
        GDI_RETURN(ret);
    }

    if (GDI_IMAGE_TYPE_INVALID == img_type || img_type >= GDI_IMAGE_TYPE_SUM)
    {
        GDI_RETURN(GDI_IMAGE_ERR_INVALID_IMG_TYPE);
    }
   
    codec = &gdi_image_codecs[img_type];


    if (src_type == GDI_IMAGE_SRC_FROM_FILE)
    {
        flag |= GDI_IMAGE_CODEC_FLAG_IS_FILE;
    }

    ret = GDI_SUCCEED;

	kal_get_time(&start_time);

    if (codec->draw_before)
        if ((flag & GDI_IMAGE_CODEC_FLAG_DISABLE_DRAW_BEFORE) == 0)
            ret = codec->draw_before(flag, frame_pos, x, y, w, h, data_ptr, img_size);

    if (ret >= GDI_SUCCEED)
    if (codec->draw)
        if ((flag & GDI_IMAGE_CODEC_FLAG_DISABLE_DRAW) == 0)
            ret = codec->draw(flag, frame_pos, x, y, w, h, data_ptr, img_size);
//gdi_image_bmp_draw_handler
	
    if (codec->draw_after)
        if ((flag & GDI_IMAGE_CODEC_FLAG_DISABLE_DRAW_AFTER) == 0)
        {
            ret1 = codec->draw_after(flag, frame_pos, x, y, w, h, data_ptr, img_size);
            if (ret >= GDI_SUCCEED && ret1 <GDI_SUCCEED)
                ret =ret1;
        }
		
	kal_get_time(&end_time);
    decode_ms = kal_ticks_to_milli_secs(end_time - start_time);

    delay_time = gdi_anim_get_delay();
    if (delay_time < decode_ms)
    {
        gdi_anim_set_delay(1);
    }
    else
    {
        gdi_anim_set_delay(delay_time - decode_ms);
    }
    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_codec_draw)
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_clear_work_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 * RETURNS
 *  
 *****************************************************************************/
void gdi_image_clear_work_buffer(void)
{
#if defined(GDI_USING_LAYER)
    gdi_memset16((U8*)gdi_work_buffer,0xffff,gdi_work_buffer_size);
#endif
}
/*****************************************************************************
 * FUNCTION
 *  gdi_image_estimate_time
 * DESCRIPTION
 *  
 * PARAMETERS
 * RETURNS
 *  
 *****************************************************************************/

U32 gdi_image_estimate_decoding_time(U8 img_type,S32 width,S32 height)
{
    U32 ms=0;
    switch(img_type)
    {
    case GDI_IMAGE_TYPE_DEVICE_BITMAP:
    default:
        ms = 300;
        break;
    }
    ms *= width*height;
    ms /=176*220;

    #if defined(MCU_13M)
        ms = ms *52/13;
    #elif defined(MCU_26M)
        ms = ms *52/26;
    #elif defined(MCU_39M)
        ms = ms *52/39;
    #elif defined(MCU_52M)
        ms = ms;
    #elif defined(MCU_104M)
        ms = ms *52/104;
    #else
	 /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-11 10:57 */
	#if 0
	//#error Unknown MCU clock
	#else
	//jade xcpu 52M
        ms = ms *52/52;
	#endif
        /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-11 10:57 */
    #endif
    return ms;
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_estimate_show_fullscreen_time
 * DESCRIPTION
 *  
 * PARAMETERS
 * RETURNS
 *  
 *****************************************************************************/

U32 gdi_image_estimate_show_fullscreen_time(void)
{
    return gdi_image_estimate_decoding_time(GDI_IMAGE_TYPE_DEVICE_BITMAP,GDI_LCD_WIDTH,GDI_LCD_HEIGHT);
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_codec_set_flag_begin
 * DESCRIPTION
 *  
 * PARAMETERS
 * RETURNS
 *  
 *****************************************************************************/
void gdi_image_codec_set_flag_begin(U32 flag)
{
    GDI_ASSERT(gdi_image_codec_flag_index < (sizeof(gdi_image_codec_flag_stack) / sizeof(U32)));
    GDI_LOCK;
    gdi_image_codec_flag_index++;
    gdi_image_codec_flag_stack[gdi_image_codec_flag_index] = gdi_image_codec_flag_stack[gdi_image_codec_flag_index - 1] | flag;
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_codec_set_flag_end
 * DESCRIPTION
 *  
 * PARAMETERS
 * RETURNS
 *  
 *****************************************************************************/
void gdi_image_codec_set_flag_end(void)
{
    GDI_ASSERT(gdi_image_codec_flag_index > 0);
    gdi_image_codec_flag_index--;
    GDI_UNLOCK;
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_codec_get_flag
 * DESCRIPTION
 *  
 * PARAMETERS
 * RETURNS
 *  
 *****************************************************************************/
U32 gdi_image_codec_get_flag(void)
{
    return gdi_image_codec_flag_stack[gdi_image_codec_flag_index];
}
U16 gdi_get_image_type(U8 *file_buf)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	if(file_buf)
	{
		if (file_buf[0] == 'B' && file_buf[1] == 'M') //bmp
		{
			return GDI_IMAGE_TYPE_BMP;
		}
		if (file_buf[0] == 'G' && file_buf[1] == 'I' && file_buf[2] == 'F')  //gif
		{
			return GDI_IMAGE_TYPE_GIF;
		}
		if (file_buf[0] == 0 && file_buf[1] == 0) //wbmp
		{
			return GDI_IMAGE_TYPE_WBMP;
		}
		if (file_buf[0] == 0xff && file_buf[1] == 0xd8 && file_buf[2] == 0xff) //jpeg
		{
			return GDI_IMAGE_TYPE_JPG;
		}
		if (memcmp(file_buf, "\x89PNG\x0d\x0a\x1a\x0a", 8) == 0) //png
		{
			return GDI_IMAGE_TYPE_PNG;
		}
	}
	return GDI_IMAGE_TYPE_INVALID;
}
