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
 *  gdi_image.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Image header file.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _GDI_IMAGE_H_
#define _GDI_IMAGE_H_

//#include " gdi_include.h"
//#define GDI_MAINLCD_BIT_PER_PIXEL                  DRV_MAINLCD_BIT_PER_PIXEL

/****************************************************************************
* Define                                                              
*****************************************************************************/
/****************************************************************************
* Struct / Typedef                                                                 
*****************************************************************************/

typedef struct
{
    GDI_RESULT(*draw_before) (U32 flag, U32 frame_pos, S32 x, S32 y, S32 w, S32 h, U8 *data_ptr, U32 img_size);
    GDI_RESULT(*draw) (U32 flag, U32 frame_pos, S32 x, S32 y, S32 w, S32 h, U8 *data_ptr, U32 img_size);
    GDI_RESULT(*draw_after) (U32 flag, U32 frame_pos, S32 x, S32 y, S32 w, S32 h, U8 *data_ptr, U32 img_size);
    GDI_RESULT(*get_dimension) (U32 flag, U8 *data_ptr, U32 img_size, S32 *width, S32 *height);
    GDI_RESULT(*get_pos_info) (U32 flag, U8 *data_ptr, U32 img_size, BOOL quick_mode, S32 *total_pos,
                               gdi_anim_pos_type_enum *pos_type);
    BOOL(*is_match_file_type) (U8 *file_name);
} gdi_image_codecs_struct;

#if defined(GDI_USING_LAYER)
    #if (GDI_WORK_BUFFER_FORMAT == GDI_COLOR_FORMAT_32)
        extern U32 gdi_work_buffer[(((GDI_LCD_WIDTH + 16) * (GDI_LCD_HEIGHT + 16) * 32) >> 3) / 4];
    #elif (GDI_WORK_BUFFER_FORMAT == GDI_COLOR_FORMAT_24)
        extern U32 gdi_work_buffer[(((GDI_LCD_WIDTH + 16) * (GDI_LCD_HEIGHT + 16) * 24) >> 3) / 4];
    #elif (GDI_WORK_BUFFER_FORMAT == GDI_COLOR_FORMAT_16)
        extern U32 gdi_work_buffer[(((GDI_LCD_WIDTH + 16) * (GDI_LCD_HEIGHT + 16) * 16) >> 3) / 4];
    #else
        extern U32 gdi_work_buffer[(((GDI_LCD_WIDTH + 16) * (GDI_LCD_HEIGHT + 16) * /*GDI_MAINLCD_BIT_PER_PIXEL*/16) >> 3) / 4];
    #endif
#endif 

extern const gdi_image_codecs_struct gdi_image_codecs[GDI_IMAGE_TYPE_SUM + 1];
extern const U16 *gdi_image_ext_name[];
extern BOOL gdi_memory_output;
extern U16 gdi_memory_output_width;
extern U16 gdi_memory_output_height;
extern U32 gdi_memory_output_buffer_address;
extern U32 gdi_memory_output_buffer_size;

/****************************************************************************
* Function Declar                                                              
*****************************************************************************/
/* init function */
extern GDI_RESULT gdi_image_init(void);
extern GDI_RESULT gdi_image_parse_info(
                    gdi_image_src_enum src_type,
                    U8 *img_src,
                    U8 **data_ptr,
                    U8 *img_type,
                    S32 *img_size);
extern GDI_RESULT gdi_image_codec_draw(
                    gdi_image_src_enum src_type,
                    U8 *img_src,
                    U8 img_type,
                    S32 img_size,
                    S32 x,
                    S32 y,
                    S32 w,
                    S32 h,
                    U32 flag,
                    U32 frame_pos);

extern GDI_RESULT gdi_image_codec_get_dimension(
                    gdi_image_src_enum src_type,
                    U8 *img_src,
                    U8 img_type,
                    S32 img_size,
                    S32 *width,
                    S32 *height,
                    U32 flag);
extern GDI_RESULT gdi_image_codec_get_frame_count(
                    gdi_image_src_enum src_type,
                    U8 *img_src,
                    U8 img_type,
                    S32 img_size,
                    S32 *frame_count,
                    U32 flag);
extern U32 gdi_image_codec_get_flag(void);
#endif /* _GDI_IMAGE_H_ */ 

