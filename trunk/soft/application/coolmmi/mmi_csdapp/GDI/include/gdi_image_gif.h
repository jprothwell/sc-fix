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
 *  gdi_image_gif.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Image Gif header file.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _GDI_IMAGE_GIF_H_
#define _GDI_IMAGE_GIF_H_

#ifdef DUAL_LCD
#define GIF_CACHE_COUNT (4)
#else 
#ifndef MEM_ULC_3216
#define GIF_CACHE_COUNT (8)
#else
#define GIF_CACHE_COUNT (2)
#endif
#endif 

#define GIF_CACHE_MAIN_LCD_IDX   (0)
#define GIF_CACHE_SUB_LCD_IDX    (1)

#define  MAX_GIF_FRAME_SUM          64

#define GDI_GIF_STACK_SIZE (0x1000+4)
#define GDI_GIF_TREE_SIZE ((0x4000+4))
/****************************************************************************
* Struct
*****************************************************************************/
typedef struct
{
    U32 cache[MAX_GIF_FRAME_SUM + 1];
    gdi_color palette[256];
    U16 image_width;
    U16 image_height;
    U16 latest_frame;
    U8 disposal[MAX_GIF_FRAME_SUM + 2];
    U8 bg_color_idx;
    BOOL prev_need_restore_bg;
    BOOL need_restore_bg;
    BOOL has_gct;
} gdi_image_gif_struct;

extern U16 gdi_image_gif_width;
extern U16 gdi_image_gif_height;

typedef BOOL(*gdi_imaeg_gif_callback_ptr) (int, int);

/***************************************************************************** 
* Function 
*****************************************************************************/
extern void gdi_image_hwgif_init(void);
extern GDI_RESULT gdi_image_hwgif_display(
                    U8 *src,
                    U32 src_size,
                    S32 ox,
                    S32 oy,
                    U16 frame_number,
                    BOOL use_disposal_method,
                    BOOL *is_transparent);
#if defined(GDI_USING_HW_GIF_V2)
extern GDI_RESULT gdi_image_hwgif_display_internal(U8 *src, U32 size,
                                                   S32 ox, S32 oy,
                                                   S32 resized_width, S32 resized_height,
                                                   U16 frame_number,
                                                   BOOL is_file,
                                                   BOOL is_resized,
                                                   BOOL use_disposal_method,
                                                   BOOL is_blocking,
                                                   BOOL transparent_enable, void (*callback) (GDI_RESULT result));
extern GDI_RESULT gdi_image_gif_cf_to_hw(gdi_color_format cf, U8 *hwgif);
extern GDI_RESULT gdi_image_gif_cf_from_hw(U8 hwcf, gdi_color_format *cf);
#endif /* defined(GDI_USING_HW_GIF_V2) */ 

extern void gdi_image_gif_set_progress_callback(gdi_imaeg_gif_callback_ptr ptr);

extern GDI_RESULT gdi_image_direct_gif(
                    S32 ox,
                    S32 oy,
                    U8 *GIF_src,
                    U32 gif_size,
                    U16 frame_number,
                    BOOL use_disposal_method,
                    BOOL use_background_color,
                    gdi_color background_color,
                    BOOL *is_trans);

extern GDI_RESULT gdi_image_gif_draw(S32 ox, S32 oy, U8 *GIF_src, U32 size, U16 frame_number);
extern GDI_RESULT gdi_image_gif_draw_file(S32 ox, S32 oy, const S8 *filename, U16 frame_number);
extern GDI_RESULT gdi_image_gif_draw_resized(
                    S32 ox,
                    S32 oy,
                    S32 resized_width,
                    S32 resized_height,
                    U8 *GIF_src,
                    U32 size,
                    U16 frame_number);
extern GDI_RESULT gdi_image_gif_draw_resized_file(
                    S32 ox,
                    S32 oy,
                    S32 resized_width,
                    S32 resized_height,
                    const S8 *filename,
                    U16 frame_number);
extern GDI_RESULT gdi_image_gif_draw_transparent_color(S32 ox, S32 oy, U8 *GIF_src, U32 size, U16 frame_number);

extern GDI_RESULT gdi_image_gif_get_dimension(U8 *GIF_src, S32 *width, S32 *height);
extern GDI_RESULT gdi_image_gif_get_dimension_file(const S8 *filename, S32 *width, S32 *height);

/* extern void                  gdi_image_gif_reset(void); */
extern BOOL gdi_image_gif_hit_cache(
                U32 cache_id,
                U16 frame_number,
                S16 *cache_index,
                S32 *frame_counter,
                S32 *offset,
                BOOL update);

extern GDI_RESULT gdi_image_gif_get_dimension_mem_internal(U8 *src, U32 size, S32 *width, S32 *height);
extern GDI_RESULT gdi_image_gif_get_dimension_file_internal(U8 *src, U32 size, S32 *width, S32 *height);
extern GDI_RESULT gdi_image_gif_draw_mem_internal(
                    S32 ox,
                    S32 oy,
                    BOOL is_resized,
                    S32 resized_width,
                    S32 resized_height,
                    U8 *src,
                    U32 size,
                    U32 cache_id,
                    U16 frame_number,
                    BOOL use_disposal_method,
                    BOOL transparent_enable);
extern GDI_RESULT gdi_image_gif_draw_file_internal(
                    S32 ox,
                    S32 oy,
                    BOOL is_resized,
                    S32 resized_width,
                    S32 resized_height,
                    U8 *src,
                    U32 size,
                    U32 cache_id,
                    U16 frame_number,
                    BOOL use_disposal_method,
                    BOOL transparent_enable);
extern U32 gdi_image_gif_get_cache_key_from_file(FS_HANDLE file_handle, const S8 *filename, U32 file_size);

extern GDI_RESULT gdi_image_gif_draw_handler(
                    U32 flag,
                    U32 frame_pos,
                    S32 x,
                    S32 y,
                    S32 w,
                    S32 h,
                    U8 *data_ptr,
                    U32 img_size);
extern GDI_RESULT gdi_image_gif_get_dimension_handler(U32 flag, U8 *data_ptr, U32 img_size, S32 *width, S32 *height);
extern GDI_RESULT gdi_image_gif_get_frame_count_handler(U32 flag, U8 *data_ptr, U32 img_size, S32 *frame_count);
extern BOOL gdi_image_is_gif_file(U8 *file_name);
extern GDI_RESULT gdi_image_gif_get_pos_info_handler(
                    U32 flag,
                    U8 *data_ptr,
                    U32 img_size,
                    BOOL quick_mode,
                    S32 *total_pos,
                    gdi_anim_pos_type_enum *pos_type);
#endif /* _GDI_IMAGE_GIF_H_ */ 

