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
 *  gdi_image_bmp.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Image Bitmap related.
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
#include "drm_gprot.h"

#if 0
#if defined(CSD_TARGET)
#pragma Otime
#pragma no_check_stack
#endif /* defined(CSD_TARGET) */ 
#endif        
/****************************************************************************
* Struct
*****************************************************************************/

typedef struct
{
    U16 bfType;
    U32 bfSize;
    U16 bfReserved1;
    U16 bfReserved2;
    U32 bfOffBits;
} bitmap_file_header;

typedef struct
{
    U32 biSize;
    U32 biWidth;
    U32 biHeight;
    U16 biPlanes;
    U16 biBitCount;
    U32 biCompression;
    U32 biSizeImage;
    U32 biXPelsPerMeter;
    U32 biYPelsPerMeter;
    U32 biClrUsed;
    U32 biClrImportant;
} bitmap_info_header;

typedef struct
{
    U32 biSize;
    U16 biWidth;
    U16 biHeight;
    U16 biPlanes;
    U16 biBitCount;
} bitmap_core_header;

#define GDI_BI_RGB  0
#define GDI_BI_RLE8  1
#define GDI_BI_RLE4  2
#define GDI_BI_BITFIELDS  3

////////////////////////////////////////////////////////////////////////////////////////////
/// BMP file internal decoder
////////////////////////////////////////////////////////////////////////////////////////////
#undef INIT_SRC
#undef GET_byte
#undef GET_word
#undef GET_U24
#undef GET_dword
#undef FLUSH
#undef SEEK
#undef IS_FROM_FILE
#undef GDI_IMAGE_BMP_DRAW_INTERNAL
#undef GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL
#undef COMPRESSION_MODE

#define SUPPORT_BMP_COMPRESSION

#ifdef SUPPORT_BMP_COMPRESSION

// 0: uncompressed
// 1: 256 color bitmap
// 2: 16 color bitmap
// 3: bitfields data
static BOOL gdi_image_bmp_compression;
static S32 gdi_image_bmp_width;
static S32 gdi_image_bmp_height;
static S32 gdi_image_bmp_src_x;
static S32 gdi_image_bmp_src_y;
static U8 gdi_image_bmp_run_length;
static U8 gdi_image_bmp_run_length_used;
static U8 gdi_image_bmp_code;

#define COMPRESSION_MODE(n,width,height) \
   do                                    \
   {                                     \
      gdi_image_bmp_compression = n;     \
      gdi_image_bmp_src_x = 0;           \
      gdi_image_bmp_src_y = height-1;    \
      gdi_image_bmp_width = width;       \
      gdi_image_bmp_height = height;     \
      gdi_image_bmp_run_length = 0;      \
   }while(0)


/*****************************************************************************
 * FUNCTION
 *  GET_byte_FROM_RUN_LENGTH
 * DESCRIPTION
 *  
 * PARAMETERS
 *  sx      [IN]        
 *  sy      [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 GET_byte_FROM_RUN_LENGTH(S32 sx, S32 sy)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    static BOOL is_raw_mode = FALSE;
    U8 ret_value = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_image_bmp_run_length == 0)
    {
        GDI_BYTESTREAM_GET_BYTE_FILE(gdi_image_bmp_run_length);
        if (gdi_image_bmp_run_length != 0)  /* encoded mdoe */
        {
            is_raw_mode = FALSE;
            gdi_image_bmp_run_length = GDI_MIN(gdi_image_bmp_run_length, gdi_image_bmp_width - sx);
            gdi_image_bmp_run_length_used = 0;
            GDI_BYTESTREAM_GET_BYTE_FILE(gdi_image_bmp_code);
        }
        else    /* escape mode */
        {
            is_raw_mode = TRUE;
            GDI_BYTESTREAM_GET_BYTE_FILE(gdi_image_bmp_run_length);
            switch (gdi_image_bmp_run_length)
            {
                case 1:
                    gdi_image_bmp_src_x = -1;
                    gdi_image_bmp_run_length = 0;
                    return 0;
                case 0: /* end of line */
                    gdi_image_bmp_src_x = 0;
                    gdi_image_bmp_src_y--;
                    gdi_image_bmp_run_length = 0;
                    return 0;
                case 2: /* delta mode */
                {
                    U8 dx, dy;

                    GDI_BYTESTREAM_GET_BYTE_FILE(dx);
                    GDI_BYTESTREAM_GET_BYTE_FILE(dy);
                    gdi_image_bmp_src_x += dx;
                    gdi_image_bmp_src_y -= dy;
                }
                    gdi_image_bmp_run_length = 0;
                    return 0;
                default:    /* abolutte */
                    gdi_image_bmp_run_length = GDI_MIN(gdi_image_bmp_run_length, gdi_image_bmp_width - sx);
                    gdi_image_bmp_run_length_used = 0;
                    break;
            }
        }
    }

    if (is_raw_mode)
    {
        if (gdi_image_bmp_compression == GDI_BI_RLE8)
        {
            GDI_BYTESTREAM_GET_BYTE_FILE(ret_value);
        }
        else
        {
            GDI_BYTESTREAM_GET_BYTE_FILE(gdi_image_bmp_code);
            ret_value = gdi_image_bmp_code;
            gdi_image_bmp_src_x++;
            gdi_image_bmp_run_length--;
            gdi_image_bmp_run_length_used++;
        }

        gdi_image_bmp_run_length--;
        gdi_image_bmp_run_length_used++;
        ++gdi_image_bmp_src_x;

        if (gdi_image_bmp_run_length == 0)  /* read pad U8. */
        {
            if (gdi_image_bmp_compression == GDI_BI_RLE8)
            {
                if (gdi_image_bmp_run_length_used & 1)
                {
                    GDI_BYTESTREAM_FLUSH_FILE(1);
                }
            }
            else
            {
                if ((gdi_image_bmp_run_length_used & 0x03) == 1 || (gdi_image_bmp_run_length_used & 0x03) == 2)
                {
                    GDI_BYTESTREAM_FLUSH_FILE(1);
                }
            }
        }
    }
    else
    {
        if (gdi_image_bmp_compression == GDI_BI_RLE8)
        {
            ret_value = gdi_image_bmp_code;
        }
        else
        {
            ret_value = gdi_image_bmp_code;
            gdi_image_bmp_run_length--;
            gdi_image_bmp_run_length_used++;
            ++gdi_image_bmp_src_x;
        }

        gdi_image_bmp_run_length--;
        gdi_image_bmp_run_length_used++;
        ++gdi_image_bmp_src_x;
    }
    return ret_value;
}

#define GET_byte(SRC,X)                                   \
   {                                                    \
      if(gdi_image_bmp_compression == GDI_BI_RLE4       \
      ||gdi_image_bmp_compression == GDI_BI_RLE8)       \
      {                                                 \
         while(1)                                       \
         {                                              \
            if(gdi_image_bmp_src_y >=pos_y)             \
            {                                           \
               if(gdi_image_bmp_src_y> pos_y            \
               ||gdi_image_bmp_src_x<= pos_x)           \
                  X=GET_byte_FROM_RUN_LENGTH(pos_x,pos_y);\
               else                                     \
                  break;                                \
            }                                           \
            else                                        \
               break;                                   \
         }                                              \
      }                                                 \
      else                                              \
      {                                                 \
         GDI_BYTESTREAM_GET_BYTE_FILE(X)                \
      }                                                 \
   }

#define FLUSH(SRC,X)                                \
   {                                                \
      if(gdi_image_bmp_compression==GDI_BI_RGB      \
      ||gdi_image_bmp_compression==GDI_BI_BITFIELDS)\
      {                                             \
         GDI_BYTESTREAM_FLUSH_FILE(X);              \
      }                                             \
   }
#else /* SUPPORT_BMP_COMPRESSION */ 
#define GET_byte(SRC,X)                        GDI_BYTESTREAM_GET_BYTE_FILE(X)
#define FLUSH(SRC,X)                         GDI_BYTESTREAM_FLUSH_FILE(X)
#endif /* SUPPORT_BMP_COMPRESSION */ 



#define GET_word(SRC,X)                       GDI_BYTESTREAM_GET_WORD_FILE(X)
#define GET_U24(SRC,X)                       GDI_BYTESTREAM_GET_3BYTES_FILE(X)
#define GET_dword(SRC,X)                       GDI_BYTESTREAM_GET_DWORD_FILE(X)

#define IS_FROM_FILE                1
#define INIT_SRC(SRC,INPUT)
#define SEEK(SRC,BMP_SRC,OFFSET)             gdi_bytestream_seek_file(OFFSET)

//////////////////////////////////
//// GENERAL FILE RESIZER DECODER
/////////////////////////////////
#define GDI_IMAGE_BMP_DRAW_INTERNAL             gdi_image_bmp_draw_internal_from_file
#define GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL gdi_image_bmp_get_dimension_internal_from_file
#define GDI_IMAGE_BMP_USING_RESIZER
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_USING_RESIZER
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL
#undef  GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL
//////////////////////////////////
//// COLOR_FORMAT  FILE NON_RESIZER DECODER
/////////////////////////////////
#undef  GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL    /* we don't need get_dimension for each color format */

#if GDI_MAINLCD_BIT_PER_PIXEL == 1 || !defined(GDI_USING_LAYER)
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_MAINLCD
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_file_main
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL
#elif GDI_MAINLCD_BIT_PER_PIXEL == 16
#define gdi_image_bmp_draw_internal_from_file_main gdi_image_bmp_draw_internal_from_file_16
#elif GDI_MAINLCD_BIT_PER_PIXEL == 24
#define gdi_image_bmp_draw_internal_from_file_main gdi_image_bmp_draw_internal_from_file_24
#elif GDI_MAINLCD_BIT_PER_PIXEL == 32
#define gdi_image_bmp_draw_internal_from_file_main gdi_image_bmp_draw_internal_from_file_32
#endif 

#ifdef __MMI_SUBLCD__
#if GDI_SUBLCD_BIT_PER_PIXEL == 1 || !defined(GDI_USING_LAYER)
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_SUBLCD
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_file_sub
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL
#elif GDI_SUBLCD_BIT_PER_PIXEL == 16
#define gdi_image_bmp_draw_internal_from_file_sub  gdi_image_bmp_draw_internal_from_file_16
#elif GDI_SUBLCD_BIT_PER_PIXEL == 24
#define gdi_image_bmp_draw_internal_from_file_sub  gdi_image_bmp_draw_internal_from_file_24
#elif GDI_SUBLCD_BIT_PER_PIXEL == 32
#define gdi_image_bmp_draw_internal_from_file_sub  gdi_image_bmp_draw_internal_from_file_32
#endif 
#endif /* __MMI_SUBLCD__ */ 

// we must provide each color format decoder
// color format 8 bit
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_8
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_file_8
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL

/* color format 16 bit */
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_16
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_file_16
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL

/* color format 24 bit */
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_24
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_file_24
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL
/* color format 32 bit */
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_32
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_file_32
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL

////////////////////////////////////////////////////////////////////////////////////////////
/// BMP mem internal decoder
////////////////////////////////////////////////////////////////////////////////////////////

#undef INIT_SRC
#undef GET_byte
#undef GET_word
#undef GET_U24
#undef GET_dword
#undef FLUSH
#undef SEEK
#undef IS_FROM_FILE
#undef GDI_IMAGE_BMP_DRAW_INTERNAL
#undef GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL
#undef COMPRESSION_MODE

/* source from memory, will not support compression mode, because we need speed up decode time. */
#undef SUPPORT_BMP_COMPRESSION
#define SUPPORT_BMP_FORMAT_565
#define INIT_SRC(SRC,INPUT)   U8 *SRC = INPUT;

#define GET_byte(SRC,X)     \
   {                      \
      X =   ((U8) *SRC++);\
   }

#define GET_word(SRC,X)         \
   {                           \
      X =   ((U16) (U8)*SRC++);\
      X |=((U16) *SRC++)<<8;   \
   }
#define GET_U24(SRC,X)           \
   {                             \
      X =   ((U32) (U8)*SRC++);  \
      X |=((U32) (U8)*SRC++)<<8; \
      X |=((U32) (U8)*SRC++)<<16;\
   }

#define GET_dword(SRC,X)           \
   {                             \
      X =   ((U32) (U8)*SRC++);  \
      X |=((U32) (U8)*SRC++)<<8; \
      X |=((U32) (U8)*SRC++)<<16;\
      X |=((U32) (U8)*SRC++)<<24;\
   }

//////////////////////////////////
//// GENERAL MEM RESIZER DECODER
/////////////////////////////////
#define COMPRESSION_MODE(n,width,height)
#define IS_FROM_FILE                0
#define FLUSH(SRC,X)                SRC+=X
#define SEEK(SRC,BMP_SRC,OFFSET)    SRC = BMP_SRC+OFFSET
#define GDI_IMAGE_BMP_DRAW_INTERNAL             gdi_image_bmp_draw_internal_from_mem
#define GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL gdi_image_bmp_get_dimension
#define GDI_IMAGE_BMP_USING_RESIZER
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_USING_RESIZER
#undef GDI_IMAGE_BMP_DRAW_INTERNAL
#undef GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL

#if 0
#if defined(CSD_TARGET)
#pragma arm section code = "INTERNCODE"
#endif
#endif

//////////////////////////////////
//// COLOR_FORMAT  MEM NON_RESIZER DECODER
/////////////////////////////////

#if GDI_MAINLCD_BIT_PER_PIXEL == 1 || !defined(GDI_USING_LAYER)
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_MAINLCD
#define GDI_IMAGE_BMP_DRAW_INTERNAL             gdi_image_bmp_draw_internal_from_mem_main
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL
#elif GDI_MAINLCD_BIT_PER_PIXEL == 16
#define gdi_image_bmp_draw_internal_from_mem_main  gdi_image_bmp_draw_internal_from_mem_16
#elif GDI_MAINLCD_BIT_PER_PIXEL == 24
#define gdi_image_bmp_draw_internal_from_mem_main  gdi_image_bmp_draw_internal_from_mem_24
#elif GDI_MAINLCD_BIT_PER_PIXEL == 32
#define gdi_image_bmp_draw_internal_from_mem_main  gdi_image_bmp_draw_internal_from_mem_32
#endif 

#ifdef __MMI_SUBLCD__
#if GDI_SUBLCD_BIT_PER_PIXEL == 1 || !defined(GDI_USING_LAYER)
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_SUBLCD
#define GDI_IMAGE_BMP_DRAW_INTERNAL             gdi_image_bmp_draw_internal_from_mem_sub
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL
#elif GDI_SUBLCD_BIT_PER_PIXEL == 16
#define gdi_image_bmp_draw_internal_from_mem_sub   gdi_image_bmp_draw_internal_from_mem_16
#elif GDI_SUBLCD_BIT_PER_PIXEL == 24
#define gdi_image_bmp_draw_internal_from_mem_sub   gdi_image_bmp_draw_internal_from_mem_24
#elif GDI_SUBLCD_BIT_PER_PIXEL == 32
#define gdi_image_bmp_draw_internal_from_mem_sub   gdi_image_bmp_draw_internal_from_mem_32
#endif 
#endif /* __MMI_SUBLCD__ */ 

// we must provide each color format decoder
// color format 8 bit
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_8
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_mem_8
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL

/* color format 16 bit */
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_16
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_mem_16
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL

/* color format 24 bit */
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_24
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_mem_24
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL
/* color format 32 bit */
#define GDI_IMAGE_BMP_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_32
#define GDI_IMAGE_BMP_DRAW_INTERNAL          gdi_image_bmp_draw_internal_from_mem_32
#include "gdi_image_bmp_codec.h"
#undef GDI_IMAGE_BMP_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_BMP_DRAW_INTERNAL

#if 0
#if defined(CSD_TARGET)
#pragma arm section code
#endif
#endif


typedef GDI_RESULT(*gdi_image_bmp_draw_func) (S32 offset_x, S32 offset_y,
                                              BOOL is_resized, S32 resized_width, S32 resized_height,
                                              U8 *BMP_src, U32 size);

/* / Reduce the code size */
#if !defined(GDI_USING_LAYER_COLOR_FORMAT)
#if GDI_MAINLCD_BIT_PER_PIXEL != 8 && GDI_SUBLCD_BIT_PER_PIXEL != 8
#define gdi_image_bmp_draw_internal_from_mem_8  (gdi_image_bmp_draw_func)gd_null_pointer_function
#define gdi_image_bmp_draw_internal_from_file_8 (gdi_image_bmp_draw_func)gd_null_pointer_function
#endif /* GDI_MAINLCD_BIT_PER_PIXEL != 8 && GDI_SUBLCD_BIT_PER_PIXEL != 8 */ 
#if GDI_MAINLCD_BIT_PER_PIXEL != 16 && GDI_SUBLCD_BIT_PER_PIXEL != 16
#define gdi_image_bmp_draw_internal_from_mem_16 (gdi_image_bmp_draw_func)gd_null_pointer_function
#define gdi_image_bmp_draw_internal_from_file_16   (gdi_image_bmp_draw_func)gd_null_pointer_function
#endif /* GDI_MAINLCD_BIT_PER_PIXEL != 16 && GDI_SUBLCD_BIT_PER_PIXEL != 16 */ 
#if GDI_MAINLCD_BIT_PER_PIXEL != 24 && GDI_SUBLCD_BIT_PER_PIXEL != 24
#define gdi_image_bmp_draw_internal_from_mem_24 (gdi_image_bmp_draw_func)gd_null_pointer_function
#define gdi_image_bmp_draw_internal_from_file_24   (gdi_image_bmp_draw_func)gd_null_pointer_function
#endif /* GDI_MAINLCD_BIT_PER_PIXEL != 24 && GDI_SUBLCD_BIT_PER_PIXEL != 24 */ 
#if GDI_MAINLCD_BIT_PER_PIXEL != 32 && GDI_SUBLCD_BIT_PER_PIXEL != 32
#define gdi_image_bmp_draw_internal_from_mem_32 (gdi_image_bmp_draw_func)gd_null_pointer_function
#define gdi_image_bmp_draw_internal_from_file_32   (gdi_image_bmp_draw_func)gd_null_pointer_function
#endif /* GDI_MAINLCD_BIT_PER_PIXEL != 32 && GDI_SUBLCD_BIT_PER_PIXEL != 32 */ 
#endif /* !defined(GDI_USING_LAYER_COLOR_FORMAT) */ 

/* decoder table */
const static gdi_image_bmp_draw_func gdi_image_bmp_draw_mem_decoders[] = 
{
    (gdi_image_bmp_draw_func) gd_null_pointer_function, /* 1 */
    gdi_image_bmp_draw_internal_from_mem_8,             /* 8 */
    gdi_image_bmp_draw_internal_from_mem_16,            /* 16 */
    gdi_image_bmp_draw_internal_from_mem_24,            /* 24 */
    gdi_image_bmp_draw_internal_from_mem_32,            /* 32 */
    gdi_image_bmp_draw_internal_from_mem_main,          /* main */
#ifdef __MMI_SUBLCD__
    gdi_image_bmp_draw_internal_from_mem_sub
#else 
    (gdi_image_bmp_draw_func) gd_null_pointer_function, /* sub */
#endif 
};

const static gdi_image_bmp_draw_func gdi_image_bmp_draw_file_decoders[] = 
{
    (gdi_image_bmp_draw_func) gd_null_pointer_function, /* 1 */
    gdi_image_bmp_draw_internal_from_file_8,            /* 8 */
    gdi_image_bmp_draw_internal_from_file_16,           /* 16 */
    gdi_image_bmp_draw_internal_from_file_24,           /* 24 */
    gdi_image_bmp_draw_internal_from_file_32,           /* 32 */
    gdi_image_bmp_draw_internal_from_file_main,         /* main */
#ifdef __MMI_SUBLCD__
    gdi_image_bmp_draw_internal_from_file_sub
#else 
    (gdi_image_bmp_draw_func) gd_null_pointer_function, /* sub */
#endif 
};


/*****************************************************************************
 * FUNCTION
 *  gdi_image_bmp_draw
 * DESCRIPTION
 *  draw bmp from memory
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  BMP_src         [IN]        
 *  size            [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_bmp_draw(S32 offset_x, S32 offset_y, U8 *BMP_src, U32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_bmp_draw)
    GDI_RETURN(gdi_image_bmp_draw_mem_decoders[gdi_act_layer->cf] (offset_x, offset_y, FALSE, 0, 0, BMP_src, size));
    GDI_EXIT_CRITICAL_SECTION(gdi_image_bmp_draw)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_bmp_draw_file
 * DESCRIPTION
 *  draw bmp from file
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  filename        [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_bmp_draw_file(S32 offset_x, S32 offset_y, S8 *filename)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_bmp_draw_file)
    GDI_RESULT ret;

    if (filename == NULL || gdi_bytestream_create_file((PS8) filename) != GDI_SUCCEED)
    {
        GDI_RETURN(GDI_FAILED);
    }

    GDI_TRY
    {
        ret = gdi_image_bmp_draw_file_decoders[gdi_act_layer->cf] (offset_x, offset_y, FALSE, 0, 0, (U8*) filename, 0);
    }
    GDI_CATCH_BEGIN
    {
        ret = GDI_FAILED;
    }
    GDI_CATCH_END gdi_bytestream_free();

    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_bmp_draw_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_bmp_draw_resized
 * DESCRIPTION
 *  draw resized bmp from memory
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  resized_width       [IN]        
 *  resized_height      [IN]        
 *  BMP_src             [IN]        
 *  size                [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_bmp_draw_resized(
            S32 offset_x,
            S32 offset_y,
            S32 resized_width,
            S32 resized_height,
            U8 *BMP_src,
            U32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_bmp_draw_resized)
    GDI_RETURN(gdi_image_bmp_draw_internal_from_mem(
                offset_x,
                offset_y,
                TRUE,
                resized_width,
                resized_height,
                BMP_src,
                size));
    GDI_EXIT_CRITICAL_SECTION(gdi_image_bmp_draw_resized)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_bmp_draw_resized_file
 * DESCRIPTION
 *  draw resized bmp from file
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  resized_width       [IN]        
 *  resized_height      [IN]        
 *  filename            [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_bmp_draw_resized_file(
            S32 offset_x,
            S32 offset_y,
            S32 resized_width,
            S32 resized_height,
            S8 *filename)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_bmp_draw_resized_file)
    GDI_RESULT ret;

    if (filename == NULL || gdi_bytestream_create_file((PS8) filename) != GDI_SUCCEED)
    {
        GDI_RETURN(GDI_FAILED);
    }

    GDI_TRY
    {
        ret = gdi_image_bmp_draw_internal_from_file(
                offset_x,
                offset_y,
                TRUE,
                resized_width,
                resized_height,
                (U8*) filename,
                0);
    }
    GDI_CATCH_BEGIN
    {
        ret = GDI_FAILED;
    }
    GDI_CATCH_END gdi_bytestream_free();

    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_bmp_draw_resized_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_bmp_get_dimension_file
 * DESCRIPTION
 *  get dimension of a bmp from file
 * PARAMETERS
 *  filename        [IN]        
 *  width           [OUT]       
 *  height          [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_bmp_get_dimension_file(S8 *filename, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_bmp_get_dimension_file)
    GDI_RESULT ret;

    if (filename == NULL || gdi_bytestream_create_file((PS8) filename) != GDI_SUCCEED)
    {
        *width = *height = 0;
        GDI_RETURN(GDI_FAILED);
    }

    GDI_TRY
    {
        ret = gdi_image_bmp_get_dimension_internal_from_file((U8*) filename, width, height);
    }
    GDI_CATCH_BEGIN
    {
        *width = *height = 0;
        ret = GDI_FAILED;
    }
    GDI_CATCH_END gdi_bytestream_free();

    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_bmp_get_dimension_file)
}

/****************************************************************************
* Cache Bitmap
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_get
 * DESCRIPTION
 *  cache a region from a layer
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 *  bmp_ptr     [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_get(S32 x1, S32 y1, S32 x2, S32 y2, gdi_image_cache_bmp_struct *bmp_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_get)
    S32 i;
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    U8 *bmp_data_ptr;
    S32 U8_per_row;

    /* get src and dest buffer ptr */
    bmp_data_ptr = bmp_ptr->buf_ptr;            /* dest */
    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* src */

    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        bmp_ptr->width = 0;
        bmp_ptr->height = 0;
        GDI_RETURN(GDI_SUCCEED);
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }

    /* fill bitmap info */
    bmp_ptr->width = end_x - start_x + 1;
    bmp_ptr->height = end_y - start_y + 1;

    U8_per_row = (bmp_ptr->width * GDI_LAYER.bits_per_pixel) >> 3;

    /* find source buffer start position of the active layer */
    layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;       /* 16-bit */

    /* fill bitmap buffer */
    for (i = 0; i < bmp_ptr->height; i++)
    {
        memcpy(bmp_data_ptr, layer_buf_ptr, U8_per_row);

        bmp_data_ptr += U8_per_row;
        layer_buf_ptr += (layer_width * GDI_LAYER.bits_per_pixel) >> 3; /* just to next row */
    }

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_get)
}
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
/****************************************************************************
* Cache Bitmap
*****************************************************************************/

/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_get
 * DESCRIPTION
 *  cache a region from a layer
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 *  bmp_ptr     [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/

GDI_RESULT gdi_image_cache_bmp_get_win(gdi_image_cache_bmp_struct *bmp_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_get)
 
    U8 *layer_buf_ptr;
    U8 *bmp_data_ptr;


    /* get src and dest buffer ptr */
    bmp_data_ptr = bmp_ptr->buf_ptr;            /* dest */
    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* src */
    memcpy(bmp_data_ptr, layer_buf_ptr, GDI_LCD_WIDTH * GDI_LCD_HEIGHT*2);


    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_get)
}
#endif
/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_draw
 * DESCRIPTION
 *  draw cached image to a layer
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  bmp_ptr         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal3D1(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 outwidth, S32 outheight,U16  offsetY)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    S32 x1, y1, x2, y2;

	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32	tempX;				// 存储中间值,提高函数速度
	U32	tempJ;				// 存储中间值,提高函数速?	
	U16 tempoutHeight;
	U16  offsetX = outwidth/offsetY;
	U16  tempoffsetX = offsetX;
	U16  tempoffsetY = 0;
	U32	tempW;
	U32	tempOW;	
	UINT32 Scale,Scaley;
	U8*	tempcopyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置
	U32	tempOW_t;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;
	tempoutHeight = outheight;

    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* dest */
    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);
	
   x1 = offset_x;
    y1 = offset_y;

    x2 = x1 + outwidth - 1;
    y2 = y1 + outheight - 1;

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }
	 layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;
	Scale=(layer_width<<16)/outwidth;
	for ( j = outwidth - 1; j > 0; j--)
	{
		// 获得临近像素的横坐标
			x = ((j *Scale)>>16)+1;

			// 修正坐标
			if (x >= layer_width)
				x--;
		if(tempoffsetX == 0)
			{
				tempoffsetX = offsetX;
				tempoutHeight = tempoutHeight - 2;
				if(tempoffsetY == offsetY)
					{
						tempoffsetY = offsetY;
					}
				else
					{
						tempoffsetY++;
					}
			}
		else
			{
				tempoffsetX--;
			}
		
		tempX = x  * U8_per_point;
		tempJ = j * U8_per_point;
		tempW = bmp_ptr->width* U8_per_point;
		tempOW = layer_width * U8_per_point;
		tempOW_t = tempOW*tempoffsetY;
		Scaley=(layer_height<<16)/tempoutHeight;
		tempcopyPixel = bmp_ptr->buf_ptr+ tempX;
		tempobjPixel = layer_buf_ptr + tempJ + tempOW_t;
		for ( i = 0; i < tempoutHeight; i++) 
		{
			

			// 获得临近像素的纵坐标
		y = ((i* Scaley )>>16)+1;

		// 修正坐标
		if (y >= layer_height)
			y--;
		
			copyPixel =  tempcopyPixel + y * tempW;
			objPixel =  tempobjPixel +i*tempOW;
			*(U16*)(objPixel) = *(U16*)(copyPixel);
		}
	}


    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
}







/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_draw
 * DESCRIPTION
 *  draw cached image to a layer
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  bmp_ptr         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal3D2(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 outwidth, S32 outheight,U16  offsetY)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    S32 x1, y1, x2, y2;

	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32	tempX;				// 存储中间值,提高函数速度
	U32	tempJ;				// 存储中间值,提高函数速?	
	U16 tempoutHeight;
	U16  offsetX = outwidth/offsetY;
	U16  tempoffsetX = offsetX;
	U16  tempoffsetY = 0;
	U32	tempW;
	U32	tempOW;	
	UINT32 Scale,Scaley;
	U8*	tempcopyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置
	U32	tempOW_t;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;
	tempoutHeight = outheight;

    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* dest */
    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);
	
   x1 = offset_x;
    y1 = offset_y;

    x2 = x1 + outwidth - 1;
    y2 = y1 + outheight - 1;

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }
	 layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;
	Scale=(layer_width<<16)/outwidth;
	for ( j = 0; j < outwidth; j++)
	{
		// 获得临近像素的横坐标
			x = ((j *Scale)>>16)+1;

			// 修正坐标
			if (x >= layer_width)
				x--;
		if(tempoffsetX == 0)
			{
				tempoffsetX = offsetX;
				tempoutHeight = tempoutHeight - 2;
				if(tempoffsetY == offsetY)
					{
						tempoffsetY = offsetY;
					}
				else
					{
						tempoffsetY++;
					}
			}
		else
			{
				tempoffsetX--;
			}
		
		tempX = x  * U8_per_point;
		tempJ = j * U8_per_point;
		tempW = bmp_ptr->width* U8_per_point;
		tempOW = layer_width * U8_per_point;
		tempOW_t = tempOW*tempoffsetY;
		Scaley=(layer_height<<16)/tempoutHeight;
		tempcopyPixel = bmp_ptr->buf_ptr+ tempX;
		tempobjPixel = layer_buf_ptr + tempJ + tempOW_t;
		for ( i = 0; i < tempoutHeight; i++) 
		{
			

			// 获得临近像素的纵坐标
		y = ((i* Scaley )>>16)+1;

		// 修正坐标
		if (y >= layer_height)
			y--;
		
			copyPixel =  tempcopyPixel + y * tempW;
			objPixel =  tempobjPixel +i*tempOW;
			*(U16*)(objPixel) = *(U16*)(copyPixel);
		}
	}


    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_draw
 * DESCRIPTION
 *  draw cached image to a layer
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  bmp_ptr         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal3D3(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 outwidth, S32 outheight,U16  offsetX)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    S32 x1, y1, x2, y2;

	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32	tempY;				// 存储中间值,提高函数速度
	U32	tempJ;				// 存储中间值,提高函数速度
	UINT32 Scale,Scaley;
	U8*	tempcopyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置
	U16  offsetY = outheight/offsetX;
	U16  tempoffsetX = 0;
	U16  tempoffsetY = offsetY;
	U16 tempoutWidth = outwidth;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;
    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* dest */
    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);
	
   x1 = offset_x;
    y1 = offset_y;

    x2 = x1 + outwidth - 1;
    y2 = y1 + outheight - 1;

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }



	layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;

	Scaley=(bmp_ptr->height<<16)/outheight;
	
	for ( j = 0; j < outheight; j++)
	{
		// 获得临近像素的纵坐标
		y = ((j*Scaley)>>16)+1;

		// 修正坐标
		if (y >= layer_height)
			y--;
		
		if(tempoffsetY == 0)
			{
				tempoffsetY = offsetY;
				tempoutWidth = tempoutWidth - 2;
				if(tempoffsetX == offsetX)
					{
						tempoffsetX = offsetX;
					}
				else
					{
						tempoffsetX++;
					}
			}
		else
			{
				tempoffsetY--;
			}
		// 计算与i,x无关的中间值
		tempY = y * bmp_ptr->width * U8_per_point;
		tempJ = j * bmp_ptr->width* U8_per_point;
		tempcopyPixel = bmp_ptr->buf_ptr + tempY;
		tempobjPixel = layer_buf_ptr + tempJ+tempoffsetX* U8_per_point;
		Scale=(bmp_ptr->width<<16)/tempoutWidth;
		for ( i = 0; i < tempoutWidth; i++) 
		{
			// 获得临近像素的横坐标
			x = ((i*Scale)>>16)+1;

			// 修正坐标
			if (x >= layer_width)
				x--;
			copyPixel =tempcopyPixel + x * U8_per_point;
			objPixel = tempobjPixel + i * U8_per_point;
			*(U16*)(objPixel) = *(U16*)(copyPixel);
		}


}

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
}



/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_draw
 * DESCRIPTION
 *  draw cached image to a layer
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  bmp_ptr         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal3D4(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 outwidth, S32 outheight,U16  offsetX)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    S32 x1, y1, x2, y2;

	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	objPixel = NULL;				// 指向被复制像素的起始位置
	S32 U8_per_point;
	U16 j,i;
	U16		x = 0;							// 变换后的像素横坐标
	U16		y = 0;							// 变换后的像素纵坐标
	U32	tempY;				// 存储中间值,提高函数速度
	U32	tempJ;				// 存储中间值,提高函数速度
	UINT32 Scale,Scaley;
	U8*	tempcopyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置
	U16  offsetY = outheight/offsetX;
	U16  tempoffsetX = 0;
	U16  tempoffsetY = offsetY;
	U16 tempoutWidth = outwidth;
	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;
    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* dest */
    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);
	
   x1 = offset_x;
    y1 = offset_y;

    x2 = x1 + outwidth - 1;
    y2 = y1 + outheight - 1;

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }



	layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;

	Scaley=(bmp_ptr->height<<16)/outheight;
	
	for ( j = outheight - 1; j > 0; j--)
	{
		// 获得临近像素的纵坐标
		y = ((j*Scaley)>>16)+1;

		// 修正坐标
		if (y >= layer_height)
			y--;
		
		if(tempoffsetY == 0)
			{
				tempoffsetY = offsetY;
				tempoutWidth = tempoutWidth - 2;
				if(tempoffsetX == offsetX)
					{
						tempoffsetX = offsetX;
					}
				else
					{
						tempoffsetX++;
					}
			}
		else
			{
				tempoffsetY--;
			}
		// 计算与i,x无关的中间值
		tempY = y * bmp_ptr->width * U8_per_point;
		tempJ = j * layer_width* U8_per_point;
		tempcopyPixel = bmp_ptr->buf_ptr + tempY;
		tempobjPixel = layer_buf_ptr + tempJ+tempoffsetX* U8_per_point;
		Scale=(bmp_ptr->width<<16)/tempoutWidth;
		for ( i = 0; i < tempoutWidth; i++) 
		{
			// 获得临近像素的横坐标
			x = ((i*Scale)>>16)+1;

			// 修正坐标
			if (x >= layer_width)
				x--;
			copyPixel =tempcopyPixel + x * U8_per_point;
			objPixel = tempobjPixel + i * U8_per_point;
			*(U16*)(objPixel) = *(U16*)(copyPixel);
		}


}

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_draw
 * DESCRIPTION
 *  draw cached image to a layer
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  bmp_ptr         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_draw_transparent(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
    S32 i,j;
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    U8 *bmp_data_ptr;
    S32 x1, y1, x2, y2;
    S32 vlaid_width;
    S32 U8_per_row;
    U8 *temp_layer_buf_ptr;
    U8 *temp_bmp_data_ptr;
    /* get src and dest buffer ptr */
    bmp_data_ptr = bmp_ptr->buf_ptr;            /* src */
    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* dest */

    U8_per_row = (bmp_ptr->width * GDI_LAYER.bits_per_pixel) >> 3;

    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);

    x1 = offset_x;
    y1 = offset_y;

    x2 = x1 + bmp_ptr->width - 1;
    y2 = y1 + bmp_ptr->height - 1;

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }

    /* find update width */
    vlaid_width = end_x - start_x + 1;

    /* shift to bmp buffer start position */
    bmp_data_ptr += (start_y - y1) * U8_per_row + (start_x - x1);

    /* shift to layer buffer start position */
    layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;

    /* fill bitmap buffer */
    for (i = start_y; i <= end_y; i++)
    {
    	for (j = 0; j <= vlaid_width; j++)
    		{
    		temp_layer_buf_ptr = layer_buf_ptr +((j*GDI_LAYER.bits_per_pixel)>>3);
		temp_bmp_data_ptr = bmp_data_ptr +((j*GDI_LAYER.bits_per_pixel)>>3);
    		if(*temp_bmp_data_ptr != 200)
    			{
    		 	 *(U16*)temp_layer_buf_ptr = *(U16*)temp_bmp_data_ptr;
    			}
    		}
		bmp_data_ptr += U8_per_row;
        	layer_buf_ptr += (layer_width * GDI_LAYER.bits_per_pixel) >> 3;
    }

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
}



/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_draw
 * DESCRIPTION
 *  draw cached image to a layer
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  bmp_ptr         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_draw_action_likeIphone( gdi_image_cache_bmp_struct *bmp_ptr,S32 width,S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
    S32 i;
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
//    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    U8 *layer_buf_ptr1;
    U8 *layer_buf_ptr2;
    U8 *layer_buf_ptr3;
    U8 *layer_buf_ptr4;
    U8 *bmp_data_ptr;
    U8 *bmp_data_ptr1;
    U8 *bmp_data_ptr2;
    U8 *bmp_data_ptr3;
    U8 *bmp_data_ptr4;
    S32 bmpx1,bmpy1;
    S32 bmpx2,bmpy2;
    S32 bmpx3,bmpy3;
    S32 bmpx4,bmpy4;
	
    S32 start_x1,start_y1;
    S32 start_x2,start_y2;
    S32 start_x3,start_y3;
    S32 start_x4,start_y4;
//    S32 x1, y1, x2, y2;
    S32 vlaid_width;
    S32 U8_per_row;
    S32 U8_layer_per_row;
    S32 U8_per_point = GDI_LAYER.bits_per_pixel>>3;
    S32 half_width = LCD_WIDTH>>1;
    S32 half_height = LCD_HEIGHT>>1;

    bmpx1 = half_width - width;
    bmpy1 = half_height - height;
    start_x1 = 0;
    start_y1 = 0;
	
    bmpx2 = half_width;
    bmpy2 = half_height;
    start_x2 = LCD_WIDTH - width;
    start_y2 = LCD_HEIGHT - height;

    bmpx3 = half_width - width;
    bmpy3 = half_height;
    start_x3 = 0;
    start_y3 = LCD_HEIGHT - height;

    bmpx4 = half_width;
    bmpy4 = half_height - height;
    start_x4 = LCD_WIDTH - width;
    start_y4 = 0;
    /* get src and dest buffer ptr */
    bmp_data_ptr = bmp_ptr->buf_ptr;            /* src */
    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* dest */

    U8_per_row = (bmp_ptr->width * U8_per_point);

    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);
    U8_layer_per_row = (layer_width * U8_per_point);
    /* find update width */
    vlaid_width = ((width)* U8_per_point);

    /* shift to bmp buffer start position */
    bmp_data_ptr1 = bmp_data_ptr + (bmpy1 * U8_per_row + ((bmpx1* U8_per_point)));
    bmp_data_ptr2 = bmp_data_ptr + (bmpy2 * U8_per_row + ((bmpx2* U8_per_point)));
    bmp_data_ptr3 = bmp_data_ptr + (bmpy3 * U8_per_row + ((bmpx3* U8_per_point)));
    bmp_data_ptr4 = bmp_data_ptr + (bmpy4 * U8_per_row + ((bmpx4* U8_per_point)));

    /* shift to layer buffer start position */
    //layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;
    layer_buf_ptr1 = layer_buf_ptr + (((start_y1 * layer_width + start_x1) * U8_per_point) );
    layer_buf_ptr2 = layer_buf_ptr + (((start_y2 * layer_width + start_x2) * U8_per_point) );
    layer_buf_ptr3 = layer_buf_ptr + (((start_y3 * layer_width + start_x3) * U8_per_point) );
    layer_buf_ptr4 = layer_buf_ptr + (((start_y4 * layer_width + start_x4) * U8_per_point) );


    /* fill bitmap buffer */
    for (i = 0; i <= height -1; i++)
    {
		memcpy(layer_buf_ptr1, bmp_data_ptr1, vlaid_width );
		memcpy(layer_buf_ptr2, bmp_data_ptr2, vlaid_width );
		memcpy(layer_buf_ptr3, bmp_data_ptr3, vlaid_width );
		memcpy(layer_buf_ptr4, bmp_data_ptr4, vlaid_width );
		bmp_data_ptr1 += U8_per_row;
        		layer_buf_ptr1 += U8_layer_per_row;
		bmp_data_ptr2 += U8_per_row;
        		layer_buf_ptr2+= U8_layer_per_row;
		bmp_data_ptr3 += U8_per_row;
        		layer_buf_ptr3 += U8_layer_per_row;
		bmp_data_ptr4 += U8_per_row;
        		layer_buf_ptr4 += U8_layer_per_row;
    }

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_draw
 * DESCRIPTION
 *  draw cached image to a layer
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  bmp_ptr         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_draw_part(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,S32 bmpx1,S32 bmpy1,S32 bmpx2,S32 bmpy2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
    S32 i;
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    U8 *bmp_data_ptr;
    S32 x1, y1, x2, y2;
    S32 vlaid_width;
    S32 U8_per_row;
    S32 U8_layer_per_row;
    /* get src and dest buffer ptr */
    bmp_data_ptr = bmp_ptr->buf_ptr;            /* src */
    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* dest */

    U8_per_row = (bmp_ptr->width * GDI_LAYER.bits_per_pixel) >> 3;

    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);
    U8_layer_per_row = (layer_width * GDI_LAYER.bits_per_pixel) >> 3;
    x1 = offset_x;
    y1 = offset_y;

    x2 = x1 + bmpx2 - bmpx1  - 1;
    y2 = y1 + bmpy2 - bmpy1 - 1;

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }

    /* find update width */
    vlaid_width = ((end_x - start_x + 1)* GDI_LAYER.bits_per_pixel)>>3;

    /* shift to bmp buffer start position */
    bmp_data_ptr += bmpy1 * U8_per_row + ((bmpx1* GDI_LAYER.bits_per_pixel)>>3);

    /* shift to layer buffer start position */
    layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;


    /* fill bitmap buffer */
    for (i = start_y; i <= end_y; i++)
    {
		memcpy(layer_buf_ptr, bmp_data_ptr, vlaid_width );
		bmp_data_ptr += U8_per_row;
        	layer_buf_ptr += U8_layer_per_row;
    }

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
}
/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_draw
 * DESCRIPTION
 *  draw cached image to a layer
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  bmp_ptr         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_draw(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
    S32 i;
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    U8 *bmp_data_ptr;
    S32 x1, y1, x2, y2;
    S32 vlaid_width;
    S32 U8_per_row;

    /* get src and dest buffer ptr */
    bmp_data_ptr = bmp_ptr->buf_ptr;            /* src */
    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* dest */

    U8_per_row = (bmp_ptr->width * GDI_LAYER.bits_per_pixel) >> 3;

    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);

    x1 = offset_x;
    y1 = offset_y;

    x2 = x1 + bmp_ptr->width - 1;
    y2 = y1 + bmp_ptr->height - 1;

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }

    /* find update width */
    vlaid_width = end_x - start_x + 1;

    /* shift to bmp buffer start position */
    bmp_data_ptr += (start_y - y1) * U8_per_row + (start_x - x1);

    /* shift to layer buffer start position */
    layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;

    /* fill bitmap buffer */
    for (i = start_y; i <= end_y; i++)
    {
        memcpy(layer_buf_ptr, bmp_data_ptr, (vlaid_width * GDI_LAYER.bits_per_pixel) >> 3);
        bmp_data_ptr += U8_per_row;
        layer_buf_ptr += (layer_width * GDI_LAYER.bits_per_pixel) >> 3;
    }

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_cache_bmp_draw
 * DESCRIPTION
 *  draw cached image to a layer
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  bmp_ptr         [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_cache_bmp_draw_ZoomNormal(S32 offset_x, S32 offset_y, gdi_image_cache_bmp_struct *bmp_ptr,U16 outWidth, U16 outHeight)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
    S32 j,i;
    S32 layer_width;
    S32 layer_height;
    S32 layer_clip_x1;
    S32 layer_clip_x2;
    S32 layer_clip_y1;
    S32 layer_clip_y2;
    S32 start_x, end_x, start_y, end_y;
    U8 *layer_buf_ptr;
    U8 *bmp_data_ptr;
    S32 x1, y1, x2, y2;
    S32 vlaid_width;
    S32 U8_per_row;
	
    	U8*	copyPixel = NULL;				// 指向原始图像中需要拷贝像素的起始位置
	S32 U8_per_point;
	U16		x = 0;						// 变换后的像素横坐标
	U16		y = 0;						// 变换后的像素纵坐标
	U32	tempY;							// 存储中间值,提高函数速度
	U32	tempJ;							// 存储中间值,提高函数速度
	UINT32 Scale,Scaley,accux;
	U8*	tempcopyPixel = NULL;			// 指向原始图像中需要拷贝像素的起始位置
	U8*	tempobjPixel = NULL;				// 指向被复制像素的起始位置

    /* get src and dest buffer ptr */
    bmp_data_ptr = bmp_ptr->buf_ptr;            /* src */
    gdi_layer_get_buffer_ptr(&layer_buf_ptr);   /* dest */

    U8_per_row = (bmp_ptr->width * GDI_LAYER.bits_per_pixel) >> 3;

    gdi_layer_get_clip(&layer_clip_x1, &layer_clip_y1, &layer_clip_x2, &layer_clip_y2);
    gdi_layer_get_dimension(&layer_width, &layer_height);

    x1 = offset_x;
    y1 = offset_y;

    x2 = x1 + bmp_ptr->width - 1;
    y2 = y1 + bmp_ptr->height - 1;

    /* out of clip region */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    /* Calculate valid region */
    if (x1 < layer_clip_x1)
    {
        start_x = layer_clip_x1;
    }
    else
    {
        start_x = x1;
    }

    if (x2 > layer_clip_x2)
    {
        end_x = layer_clip_x2;
    }
    else
    {
        end_x = x2;
    }

    if (y1 < layer_clip_y1)
    {
        start_y = layer_clip_y1;
    }
    else
    {
        start_y = y1;
    }

    if (y2 > layer_clip_y2)
    {
        end_y = layer_clip_y2;
    }
    else
    {
        end_y = y2;
    }
    /* find update width */
    vlaid_width = end_x - start_x + 1;

    /* shift to bmp buffer start position */
    bmp_data_ptr += (start_y - y1) * U8_per_row + (start_x - x1);

    /* shift to layer buffer start position */
    layer_buf_ptr += ((start_y * layer_width + start_x) * GDI_LAYER.bits_per_pixel) >> 3;

	U8_per_point = (GDI_LAYER.bits_per_pixel) >> 3;
       
	Scaley=(bmp_ptr->height<<16)/outHeight;
	Scale=(bmp_ptr->width<<16)/outWidth;
	
	for ( j = 0; j < outHeight; j++)
	{
		// 获得临近像素的纵坐标
		y = ((j*Scale)>>16)+1;

		// 修正坐标
		if (y >= bmp_ptr->height)
			y--;

		// 计算与i,x无关的中间值
		tempY = y * bmp_ptr->width * U8_per_point;
		tempJ = j * layer_width * U8_per_point;
		tempcopyPixel = bmp_data_ptr + tempY;
		tempobjPixel = layer_buf_ptr + tempJ;

		accux = 65536;
		
		for ( i = outWidth-1; i >0 ; i--) 
		{
			x = accux>>16;
			
			copyPixel =tempcopyPixel + x * U8_per_point;
			*(U16*)(tempobjPixel) = *(U16*)(copyPixel);

			accux += Scale;
			tempobjPixel += U8_per_point;
		}

		x = accux>>16;
			
		if (x >= bmp_ptr->width)
			x--;
		
		copyPixel =tempcopyPixel + x * U8_per_point;
		*(U16*)(tempobjPixel) = *(U16*)(copyPixel);
		
	}


    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_cache_bmp_draw)
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_bmp_draw_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  flag            [IN]        
 *  frame_pos       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  w               [IN]        
 *  h               [IN]        
 *  data_ptr        [?]         
 *  img_size        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_bmp_draw_handler(U32 flag, U32 frame_pos, S32 x, S32 y, S32 w, S32 h, U8 *data_ptr, U32 img_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if (flag & GDI_IMAGE_CODEC_FLAG_IS_FILE)
    {
        if (w && h)
        {
            return gdi_image_bmp_draw_resized_file(x, y, w, h, (S8*) data_ptr);
        }
        else
        {
            return gdi_image_bmp_draw_file(x, y, (S8*) data_ptr);
        }
    }
    else
    {
        if (w && h)
        {
            return gdi_image_bmp_draw_resized(x, y, w, h, data_ptr, img_size);
        }
        else
        {
		mmi_trace(g_sw_GDI, "data_ptr = 0x%x, data_size = %d", data_ptr,img_size);
            return gdi_image_bmp_draw(x, y, data_ptr, img_size);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_bmp_get_dimension_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  flag            [IN]        
 *  data_ptr        [?]         
 *  img_size        [IN]        
 *  width           [?]         
 *  height          [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_bmp_get_dimension_handler(U32 flag, U8 *data_ptr, U32 img_size, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (flag & GDI_IMAGE_CODEC_FLAG_IS_FILE)
    {
        return gdi_image_bmp_get_dimension_file((S8*) data_ptr, width, height);
    }
    else
    {
        return gdi_image_bmp_get_dimension(data_ptr, width, height);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_is_bmp_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  file_name       [?]     
 * RETURNS
 *  
 *****************************************************************************/
BOOL gdi_image_is_bmp_file(U8 *file_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FS_HANDLE f;
    U8 buf[2];
    INT32 len;
    U32 result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    f = DRM_open_file((UINT8*) file_name, FS_READ_ONLY | FS_OPEN_NO_DIR, 0);
    if (f < 0)
    {
        return FALSE;
    }

    result = DRM_read_file(f, buf, 2, (INT32*)&len);

    DRM_close_file(f);

    if (result == FS_NO_ERROR)
        if (len == 2)
            if (buf[0] == 'B' && buf[1] == 'M')
            {
                return TRUE;
            }
    return FALSE;
}

