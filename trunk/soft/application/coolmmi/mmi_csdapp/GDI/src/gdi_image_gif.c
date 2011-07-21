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
 *  gdi_image_gif.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Image Gif related.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#define  __NEWSIMULATOR
#include "drm_gprot.h"

#include "gdi_internal.h"
#include "gd_primitive.h"

#include "gui.h"        /* for timer */
#include "unicodexdcl.h"
/* DRM_REPLACE */
#include "ucs2prot.h"
#ifdef GDI_USING_HW_GIF
#include "gif_decoder.h"
#endif 

#ifdef GDI_USING_HW_GIF_V2
#include "gif_decoder_v2.h"
#endif

#include "filesystemdef.h"

#if 0
#if defined(CSD_TARGET)
#pragma Otime
#pragma no_check_stack
#endif /* defined(CSD_TARGET) */ 
#endif

#define __align(x)


extern void gif_reset_cache(void);
/****************************************************************************
* Variable ( will extern )
*****************************************************************************/
/*
 * extern varialbe for delay time, and size 
 */
/*
 * Used by GIF animation. It should be moved to global context 
 */
U16 gdi_image_gif_width;
U16 gdi_image_gif_height;
BOOL gdi_image_gif_decoder_is_bypass_color = FALSE;
gdi_color gdi_image_gif_decoder_bypass_color;
gdi_color *current_palette;
U16 gdi_image_gif_transparent_index;
U8 gdi_image_gif_palette_size;

/****************************************************************************
* Local Variable
*****************************************************************************/
gdi_image_gif_struct gdi_gif_cache[GIF_CACHE_COUNT];
static S32 gif_cache_last_index[2] = {0, 0};    /* offset from cache begin */
static U32 gdi_image_gif_codec_flag = 0;

__align(4)  
     S16 gdi_gif_stack[GDI_GIF_STACK_SIZE / sizeof(S16)];
     S32 GDI_TREE_BUFFER[GDI_GIF_TREE_SIZE / sizeof(S32)];
     S16 *gdi_gif_suffix = (S16 *) GDI_TREE_BUFFER;
     S16 *gdi_gif_prefix = (S16 *) (((U8 *) GDI_TREE_BUFFER) + sizeof(GDI_TREE_BUFFER) / 2);

#define GIF_CSD_stack   gdi_gif_stack
#define GIF_CSD_suffix  gdi_gif_suffix
#define GIF_CSD_prefix  gdi_gif_prefix

BOOL(*gdi_image_gif_progress_callback) (int x, int y) = NULL;

//////////////////////////////////////////////////////////////////////////////
///  GIF MEMORY DECODER
//////////////////////////////////////////////////////////////////////////////

     static U8 *GIF_SRC;
     static U8 *GIF_SRC_BEGIN;
     static U8 *GIF_SRC_END;
     static U32 GIF_SRC_SIZE;

#define INIT_SRC(INPUT,SIZE)                     \
   {                                             \
      GIF_SRC = GIF_SRC_BEGIN = INPUT;           \
      GIF_SRC_SIZE = SIZE;                       \
      GIF_SRC_END = GIF_SRC_BEGIN + GIF_SRC_SIZE;\
   }                                             \

#define PEEK_byte(X)               X = *GIF_SRC
#define PEEK_byte_N(X,OFFSET)         X = *(GIF_SRC+OFFSET)
#define GETS(POINTER,ARRAY,N) \
   {                          \
      POINTER = GIF_SRC;      \
      GIF_SRC+=N;             \
   }

#define GET_byte(X)             \
   {                          \
      X =   ((U8) *GIF_SRC++);\
   }

#define GET_word(X)                 \
   {                               \
      X =   ((U16) (U8)*GIF_SRC++);\
      X |=((U16) *GIF_SRC++)<<8;   \
   }

#define GET_U24(X)                   \
   {                                 \
      X =   ((U32) (U8)*GIF_SRC++);  \
      X |=((U32) (U8)*GIF_SRC++)<<8; \
      X |=((U32) (U8)*GIF_SRC++)<<16;\
   }

#define GET_dword(X)                   \
   {                                 \
      X =   ((U32) (U8)*GIF_SRC++);  \
      X |=((U32) (U8)*GIF_SRC++)<<8; \
      X |=((U32) (U8)*GIF_SRC++)<<16;\
      X |=((U32) (U8)*GIF_SRC++)<<24;\
   }

#define TELL()       (GIF_SRC - GIF_SRC_BEGIN)
#define FLUSH(X)        GIF_SRC+=X
#define SEEK(OFFSET)    GIF_SRC = GIF_SRC_BEGIN+OFFSET
#define IS_EOF()        (GIF_SRC>= GIF_SRC_END)

//////////////////////////////////////////////////////////////////////////////
/// GENERAL MEMORY GIF DECODER (SUPPORT NON_RESIZER/RESIZER/COLOR FORMAT)
//////////////////////////////////////////////////////////////////////////////
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_mem_internal
#define GDI_IMAGE_GIF_GET_DIMENSION_INTERNAL gdi_image_gif_get_dimension_mem_internal
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_mem_codec
#define GDI_IMAGE_GIF_USING_RESIZER
#define GDI_IMAGE_GIF_USING_INTERRAM
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_USING_RESIZER
#undef GDI_IMAGE_GIF_USING_INTERRAM
#undef GDI_IMAGE_GIF_DRAW_INTERNAL
#undef GDI_IMAGE_GIF_GET_DIMENSION_INTERNAL
//////////////////////////////////////////////////////////////////////////////
/// SPECIFY COLOR FORMAT MEMORY GIF DECODER (SUPPORT NON_RESIZER)
//////////////////////////////////////////////////////////////////////////////
#if GDI_MAINLCD_BIT_PER_PIXEL == 1 || !defined(GDI_USING_LAYER)
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT     GDI_COLOR_FORMAT_MAINLCD
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_mem_main
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_mem_codec_main
#define GDI_IMAGE_GIF_USING_INTERRAM
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_USING_INTERRAM
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL
#elif GDI_MAINLCD_BIT_PER_PIXEL == 16
#define gdi_image_gif_draw_internal_from_mem_main  gdi_image_gif_draw_internal_from_mem_16
#elif GDI_MAINLCD_BIT_PER_PIXEL == 24
#define gdi_image_gif_draw_internal_from_mem_main  gdi_image_gif_draw_internal_from_mem_24
#elif GDI_MAINLCD_BIT_PER_PIXEL == 32
#define gdi_image_gif_draw_internal_from_mem_main  gdi_image_gif_draw_internal_from_mem_32
#endif 

#ifdef __MMI_SUBLCD__
#if GDI_SUBLCD_BIT_PER_PIXEL == 1 || !defined(GDI_USING_LAYER)
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT     GDI_COLOR_FORMAT_SUBLCD
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_mem_sub
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_mem_codec_sub
#define GDI_IMAGE_GIF_USING_INTERRAM
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_USING_INTERRAM
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL
#elif GDI_SUBLCD_BIT_PER_PIXEL == 16
#define gdi_image_gif_draw_internal_from_mem_sub   gdi_image_gif_draw_internal_from_mem_16
#elif GDI_SUBLCD_BIT_PER_PIXEL == 24
#define gdi_image_gif_draw_internal_from_mem_sub   gdi_image_gif_draw_internal_from_mem_24
#elif GDI_SUBLCD_BIT_PER_PIXEL == 32
#define gdi_image_gif_draw_internal_from_mem_sub   gdi_image_gif_draw_internal_from_mem_32
#endif 
#endif /* __MMI_SUBLCD__ */ 

// we must provide each color format decoder
// color format 8 bit
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_8
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_mem_8
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_mem_codec_8
#define GDI_IMAGE_GIF_USING_INTERRAM
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_USING_INTERRAM
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL

/* color format 16 bit */
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_16
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_mem_16
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_mem_codec_16
#define GDI_IMAGE_GIF_USING_INTERRAM
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_USING_INTERRAM
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL

/* color format 24 bit */
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_24
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_mem_24
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_mem_codec_24
#define GDI_IMAGE_GIF_USING_INTERRAM
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_USING_INTERRAM
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL
/* color format 32 bit */
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_32
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_mem_32
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_mem_codec_32
#define GDI_IMAGE_GIF_USING_INTERRAM
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_USING_INTERRAM
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL

#undef GET_byte
#undef PEEK_byte
#undef PEEK_byte_N
#undef GET_word
#undef GET_U24
#undef GET_dword
#undef GETS
#undef FLUSH
#undef SEEK
#undef TELL
#undef IS_EOF
#undef INIT_SRC

//////////////////////////////////////////////////////////////////////////////
///  GIF FILE DECODER (SUPPORT NON_RESIZER/RESIZER/COLOR FORMAT)
//////////////////////////////////////////////////////////////////////////////
#define INIT_SRC(INPUT,SIZE)
#define PEEK_byte(X)               GDI_BYTESTREAM_PEEK_BYTE_FILE(X)
#define PEEK_byte_N(X,OFFSET)         GDI_BYTESTREAM_PEEK_BYTE_N_FILE(X,OFFSET)
#define GETS(POINTER,ARRAY,N)                \
   {                                         \
      POINTER = ARRAY;                       \
      GDI_BYTESTREAM_GET_ARRAY_FILE(ARRAY,N);\
   }

#define GET_byte(X)             GDI_BYTESTREAM_GET_BYTE_FILE(X)
#define GET_word(X)               GDI_BYTESTREAM_GET_WORD_FILE(X)
#define GET_U24(X)               GDI_BYTESTREAM_GET_3BYTES_FILE(X)
#define GET_dword(X)               GDI_BYTESTREAM_GET_DWORD_FILE(X)
#define TELL()                GDI_BYTESTREAM_TELL_FILE
#define FLUSH(X)                 GDI_BYTESTREAM_FLUSH_FILE(X)
#define SEEK(OFFSET)             gdi_bytestream_seek_file(OFFSET)
#define IS_EOF()                 GDI_BYTESTREAM_IS_EOF_FILE

#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_file_internal
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_file_codec
#define GDI_IMAGE_GIF_GET_DIMENSION_INTERNAL gdi_image_gif_get_dimension_file_internal
#define GDI_IMAGE_GIF_USING_RESIZER
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_USING_RESIZER
#undef GDI_IMAGE_GIF_DRAW_INTERNAL
#undef GDI_IMAGE_GIF_GET_DIMENSION_INTERNAL

//////////////////////////////////////////////////////////////////////////////
/// SPECIFY COLOR FORMAT FILE GIF DECODER (SUPPORT NON_RESIZER)
//////////////////////////////////////////////////////////////////////////////
#if GDI_MAINLCD_BIT_PER_PIXEL == 1 || !defined(GDI_USING_LAYER)
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT     GDI_COLOR_FORMAT_MAINLCD
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_file_main
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_file_codec_main
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL
#elif GDI_MAINLCD_BIT_PER_PIXEL == 16
#define gdi_image_gif_draw_internal_from_file_main gdi_image_gif_draw_internal_from_file_16
#elif GDI_MAINLCD_BIT_PER_PIXEL == 24
#define gdi_image_gif_draw_internal_from_file_main gdi_image_gif_draw_internal_from_file_24
#elif GDI_MAINLCD_BIT_PER_PIXEL == 32
#define gdi_image_gif_draw_internal_from_file_main gdi_image_gif_draw_internal_from_file_32
#endif 

#ifdef __MMI_SUBLCD__
#if GDI_SUBLCD_BIT_PER_PIXEL == 1 || !defined(GDI_USING_LAYER)
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT     GDI_COLOR_FORMAT_SUBLCD
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_file_sub
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_file_codec_sub
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL
#elif GDI_SUBLCD_BIT_PER_PIXEL == 16
#define gdi_image_gif_draw_internal_from_file_sub  gdi_image_gif_draw_internal_from_file_16
#elif GDI_SUBLCD_BIT_PER_PIXEL == 24
#define gdi_image_gif_draw_internal_from_file_sub  gdi_image_gif_draw_internal_from_file_24
#elif GDI_SUBLCD_BIT_PER_PIXEL == 32
#define gdi_image_gif_draw_internal_from_file_sub  gdi_image_gif_draw_internal_from_file_32
#endif 
#endif /* __MMI_SUBLCD__ */ 

// we must provide each color format decoder
// color format 8 bit
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_8
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_file_8
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_file_codec_8
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL

/* color format 16 bit */
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_16
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_file_16
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_file_codec_16
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL

/* color format 24 bit */
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_24
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_file_24
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_file_codec_24
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL
/* color format 32 bit */
#define GDI_IMAGE_GIF_CODEC_COLOR_FORMAT        GDI_COLOR_FORMAT_32
#define GDI_IMAGE_GIF_DRAW_INTERNAL          gdi_image_gif_draw_internal_from_file_32
#define GDI_IMAGE_GIF_CODEC                  gdi_image_gif_draw_internal_from_file_codec_32
#include "gdi_image_gif_codec.h"
#undef GDI_IMAGE_GIF_CODEC
#undef GDI_IMAGE_GIF_CODEC_COLOR_FORMAT
#undef  GDI_IMAGE_GIF_DRAW_INTERNAL

#undef GET_byte
#undef PEEK_byte
#undef PEEK_byte_N
#undef GET_word
#undef GET_U24
#undef GET_dword
#undef GETS
#undef FLUSH
#undef SEEK
#undef TELL
#undef IS_EOF
#undef INIT_SRC

/****************************************************************************
* Macro
*****************************************************************************/
#define GET_byte()        gdi_bytestream_get_byte()
#define PEEK_byte()       gdi_bytestream_peek_byte()
#define PEEK_byte_N(_i)   gdi_bytestream_peek_byte_n(_i)
#define GET_word()       gdi_bytestream_get_word()
#define GET_dword()       gdi_bytestream_get_dword()
#define GETS(ARRAY,N)      gdi_bytestream_get_array(ARRAY,N)
#define FLUSH(count)    gdi_bytestream_flush(count)
#define SEEK(offset)    gdi_bytestream_seek(offset)
#define TELL()          gdi_bytestream_tell()
#define IS_EOF()        gdi_bytestream_is_eof()

#define GIF_INITIALIZE_DECODER(size)                     \
{   GIF_current_code_size=(S16)(size+1);                 \
   GIF_top_slot=(S16)(1<<GIF_current_code_size);         \
    GIF_clear_code=(S16)(1<<size);                       \
    GIF_end_code=(S16)(GIF_clear_code+1);                \
    GIF_slot=(GIF_new_codes)=(S16)(GIF_end_code+1);      \
   bitstream_available_bits=0;                           \
   bytestream_available=0;                               \
   bitstream=0;                                          \
}                                                        \

#define GIF_GET_NEXT_CODE(ret)                                                \
{                                                                             \
   while(GIF_current_code_size> bitstream_available_bits)                     \
   {                                                                          \
      if(bytestream_available==0) bytestream_available=GET_byte();              \
      bitstream |= GET_byte()<<bitstream_available_bits;                        \
      bitstream_available_bits +=8;                                           \
      bytestream_available--;                                                 \
   }                                                                          \
   bitstream_available_bits -= GIF_current_code_size;                         \
   ret = (S16) bitstream & ((1<<GIF_current_code_size)-1);                    \
   bitstream >>= GIF_current_code_size;                                       \
}

     typedef GDI_RESULT(*gdi_image_gif_draw_func) (S32 ox, S32 oy,
                                                   BOOL is_resized, S32 resized_width, S32 resized_height,
                                                   U8 *src, U32 size,
                                                   U32 cache_id,
                                                   U16 frame_number, BOOL use_disposal_method, BOOL transparent_enable);

/* / Reduce the code size */
#if !defined(GDI_USING_LAYER_COLOR_FORMAT)
#if GDI_MAINLCD_BIT_PER_PIXEL != 8 && GDI_SUBLCD_BIT_PER_PIXEL != 8
#define gdi_image_gif_draw_internal_from_mem_8  (gdi_image_gif_draw_func)gd_null_pointer_function
#define gdi_image_gif_draw_internal_from_file_8 (gdi_image_gif_draw_func)gd_null_pointer_function
#endif /* GDI_MAINLCD_BIT_PER_PIXEL != 8 && GDI_SUBLCD_BIT_PER_PIXEL != 8 */ 
#if GDI_MAINLCD_BIT_PER_PIXEL != 16 && GDI_SUBLCD_BIT_PER_PIXEL != 16
#define gdi_image_gif_draw_internal_from_mem_16 (gdi_image_gif_draw_func)gd_null_pointer_function
#define gdi_image_gif_draw_internal_from_file_16   (gdi_image_gif_draw_func)gd_null_pointer_function
#endif /* GDI_MAINLCD_BIT_PER_PIXEL != 16 && GDI_SUBLCD_BIT_PER_PIXEL != 16 */ 
#if GDI_MAINLCD_BIT_PER_PIXEL != 24 && GDI_SUBLCD_BIT_PER_PIXEL != 24
#define gdi_image_gif_draw_internal_from_mem_24 (gdi_image_gif_draw_func)gd_null_pointer_function
#define gdi_image_gif_draw_internal_from_file_24   (gdi_image_gif_draw_func)gd_null_pointer_function
#endif /* GDI_MAINLCD_BIT_PER_PIXEL != 24 && GDI_SUBLCD_BIT_PER_PIXEL != 24 */ 
#if GDI_MAINLCD_BIT_PER_PIXEL != 32 && GDI_SUBLCD_BIT_PER_PIXEL != 32
#define gdi_image_gif_draw_internal_from_mem_32 (gdi_image_gif_draw_func)gd_null_pointer_function
#define gdi_image_gif_draw_internal_from_file_32   (gdi_image_gif_draw_func)gd_null_pointer_function
#endif /* GDI_MAINLCD_BIT_PER_PIXEL != 32 && GDI_SUBLCD_BIT_PER_PIXEL != 32 */ 
#endif /* !defined(GDI_USING_LAYER_COLOR_FORMAT) */ 

/* decoder table */

     const static gdi_image_gif_draw_func gdi_image_gif_draw_mem_decoders[] = 
     {
         (gdi_image_gif_draw_func) gd_null_pointer_function,    /* 1 */
         gdi_image_gif_draw_internal_from_mem_8,    /* 8 */
         gdi_image_gif_draw_internal_from_mem_16,   /* 16 */
         gdi_image_gif_draw_internal_from_mem_24,   /* 24 */
         gdi_image_gif_draw_internal_from_mem_32,   /* 32 */
         gdi_image_gif_draw_internal_from_mem_main, /* main */
     #ifdef __MMI_SUBLCD__
         gdi_image_gif_draw_internal_from_mem_sub
     #else 
         (gdi_image_gif_draw_func) gd_null_pointer_function,    /* sub */
     #endif 
     };

const static gdi_image_gif_draw_func gdi_image_gif_draw_file_decoders[] = 
{
    (gdi_image_gif_draw_func) gd_null_pointer_function, /* 1 */
    gdi_image_gif_draw_internal_from_file_8,            /* 8 */
    gdi_image_gif_draw_internal_from_file_16,           /* 16 */
    gdi_image_gif_draw_internal_from_file_24,           /* 24 */
    gdi_image_gif_draw_internal_from_file_32,           /* 32 */
    gdi_image_gif_draw_internal_from_file_main,         /* main */
#ifdef __MMI_SUBLCD__
    gdi_image_gif_draw_internal_from_file_sub
#else 
    (gdi_image_gif_draw_func) gd_null_pointer_function, /* sub */
#endif 
};


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_cache_key_from_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  file_handle     [IN]        
 *  filename        [IN]        
 *  file_size       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U32 gdi_image_gif_get_cache_key_from_file(FS_HANDLE file_handle, const S8 *filename, U32 file_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* Compute the cache key by filename and file size */
    U32 cache_id;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(CSD_TARGET)
    if (MMI_FS_GetFirstCluster(file_handle, &cache_id) == FS_NO_ERROR)
    {
        cache_id += ((U32) (filename[0])) << 24;    /* first char usually is DRIVE name */
        return cache_id;
    }
#endif /* defined(CSD_TARGET) */ 

    cache_id = (U32) gdi_fast_semi_crc((U8*) filename, (UCS2Strlen(filename) + 1) * 2);
    cache_id ^= (U32) file_size;
    return cache_id;
}

#if 0
/*
 * Start of internal RAM 
 */
#if defined(CSD_TARGET)
#pragma arm section code = "INTERNCODE"
#endif 
#endif


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_hit_cache
 * DESCRIPTION
 *  
 * PARAMETERS
 *  cache_id            [IN]        
 *  frame_number        [IN]        
 *  cache_index         [?]         
 *  frame_counter       [?]         
 *  offset              [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gdi_image_gif_hit_cache(U32 cache_id, U16 frame_number, S16 *cache_index, S32 *frame_counter, S32 *offset, BOOL update)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* initialize the cache index begin */
    S32 cache_index_begin;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_SUBLCD__
    if (GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        *cache_index = (S16) gif_cache_last_index[1];
        cache_index_begin = GIF_CACHE_COUNT / 2;
    }
    else
#endif /* __MMI_SUBLCD__ */ 
    {
        *cache_index = (S16) gif_cache_last_index[0];
        cache_index_begin = 0;
    }
    /* search the cache index */
    {
        S32 i;

        for (i = 0; i < GIF_CACHE_COUNT / 2; i++)
        {
            if (gdi_gif_cache[cache_index_begin + (*cache_index)].cache[0] == cache_id)
            {
                break;
            }
            (*cache_index)++;
            (*cache_index) %= GIF_CACHE_COUNT / 2;
        }

        /* can't find it from cache */
        if (i == GIF_CACHE_COUNT / 2)
        {
            (*cache_index)++;
            (*cache_index) %= GIF_CACHE_COUNT / 2;
        }

        /* save the offset to cache_last_index[] */
    #ifdef __MMI_SUBLCD__
        if (GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE)
        {
            gif_cache_last_index[1] = *cache_index;
        }
        else
    #endif /* __MMI_SUBLCD__ */ 
            gif_cache_last_index[0] = *cache_index;

        (*cache_index) += (S16) cache_index_begin;
    }

    // /////////////////////////////////////////////////////////
    // try to hit the cache data
    if (cache_id && gdi_gif_cache[*cache_index].cache[0] == cache_id)
    {
        S32 i = (frame_number >= MAX_GIF_FRAME_SUM) ? MAX_GIF_FRAME_SUM - 1 : frame_number;

        if ((frame_number >= MAX_GIF_FRAME_SUM) && (frame_number >= gdi_gif_cache[*cache_index].latest_frame) &&(gdi_gif_cache[*cache_index].latest_frame))
        {
            i = MAX_GIF_FRAME_SUM;
        }

        *frame_counter = 0;
        for (; i > 0; i--)
        {
            if (gdi_gif_cache[*cache_index].cache[i])
            {
                if (i < MAX_GIF_FRAME_SUM && (frame_number < gdi_gif_cache[*cache_index].latest_frame || !gdi_gif_cache[*cache_index].latest_frame))
                {
                    *frame_counter = i;
                }
                else
                {
                    *frame_counter = gdi_gif_cache[*cache_index].latest_frame;
                }
                break;
            }
        }
        if (i > 0)
        {
            *offset = gdi_gif_cache[*cache_index].cache[i];
            if (i < MAX_GIF_FRAME_SUM && (frame_number < gdi_gif_cache[*cache_index].latest_frame || !gdi_gif_cache[*cache_index].latest_frame))
            {
                gdi_gif_cache[*cache_index].need_restore_bg = gdi_gif_cache[*cache_index].disposal[i - 1];
            }
            else
            {
                gdi_gif_cache[*cache_index].need_restore_bg = gdi_gif_cache[*cache_index].disposal[i];
            }
        }
        else
        {
            *offset = 0;
        }

        return TRUE;
    }
    else if (update)
    {
         
        memset(gdi_gif_cache[*cache_index].cache, 0, sizeof(gdi_image_gif_struct));
        gdi_gif_cache[*cache_index].cache[0] = cache_id;
        gdi_gif_cache[*cache_index].latest_frame = 0;
        /* value */
        *frame_counter = 0;
        *offset = 0;

        return FALSE;
    }
    return FALSE;

}

#if 0
#if defined(CSD_TARGET)
#pragma arm section code
#endif
#endif



/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_dummy_decoder
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gdi_image_gif_dummy_decoder(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 code, fc, oc, c, size;
    S16 *stack_pointer;
    S16 GIF_current_code_size, GIF_clear_code, GIF_end_code, GIF_new_codes, GIF_top_slot, GIF_slot;
    S32 bitstream_available_bits;
    S32 bytestream_available;
    U32 bitstream;
    S32 x, y, xsize, ysize;
    S32 want_pixels;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x = GET_word();
    y = GET_word();
    xsize = GET_word();
    ysize = GET_word();
    want_pixels = xsize * ysize;

    {
        S32 n;

        n = GET_byte();
        if (IS_EOF())
        {
            GDI_RAISE(1);
        }

        if (n & 0x80)
        {
            n = 1 << ((n & 0x7) + 1);

            /*
             * skip reading palette 
             */
            FLUSH(n * 3);
        }
    }

    size = GET_byte();

    GIF_INITIALIZE_DECODER(size);
    oc = fc = 0;
    stack_pointer = GIF_CSD_stack;

    /* begin decoding */
    while (1)
    {
        GIF_GET_NEXT_CODE(c);

        if (c == GIF_end_code)
        {
            break;
        }

        if (IS_EOF())
        {
            GDI_RAISE(1);
        }

        if (c == GIF_clear_code)
        {
            GIF_current_code_size = (S16) (size + 1);
            GIF_slot = GIF_new_codes;
            GIF_top_slot = (S16) (1 << GIF_current_code_size);

            GIF_GET_NEXT_CODE(c);

            if (c == GIF_end_code)
            {
                break;
            }

            if (IS_EOF())
            {
                GDI_RAISE(1);
            }

            if (c >= GIF_slot)
            {
                c = 0;
            }

            oc = fc = c;
            if ((--want_pixels) < 0)
            {
                return;
            }
        }
        else
        {
            code = c;
            if (code >= GIF_slot)
            {
                code = oc;
                *stack_pointer++ = fc;
            }

            while (code >= GIF_new_codes)
            {
				if(stack_pointer >= (S16*)(((U8*)GIF_CSD_stack) + sizeof(GIF_CSD_stack)))
					GDI_RAISE(1);
					
                *stack_pointer++ = GIF_CSD_suffix[code];
                code = GIF_CSD_prefix[code];
            }

            *stack_pointer++ = code;
            if (GIF_slot < GIF_top_slot)
            {
                GIF_CSD_suffix[GIF_slot] = fc = code;
                GIF_CSD_prefix[GIF_slot++] = oc;
                oc = c;
            }

            if (GIF_slot >= GIF_top_slot)
            {
                if (GIF_current_code_size < 12)
                {
                    GIF_top_slot <<= 1;
                    GIF_current_code_size++;
                }
            }

            while (stack_pointer > GIF_CSD_stack)
            {
                if ((--want_pixels) < 0)
                {
                    return;
                }
                --stack_pointer;
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_frame_count_internal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  frame_count     [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_get_frame_count_internal(S32 *frame_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 gif_done;
    S32 palette_count;
    U16 transparent_index = 256;
    gdi_handle lcd_handle;
	S16 img_w,img_h;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*
     * init the return frame_count value 
     */
    *frame_count = 0;

    /*
     * get active lcd handle 
     */
    gdi_lcd_get_active(&lcd_handle);

    transparent_index = 256;    /* assign an invalid color index value */

    /*
     * read the GIF file signature 
     */
    if (GET_byte() != 'G' || GET_byte() != 'I' || GET_byte() != 'F')
    {
        return GDI_FAILED;
    }

    /*
     * skip dimension value 
     */
//	FLUSH(7);
	FLUSH(3);
	img_w = GET_word();
	img_h = GET_word();
	if(img_w<0 || img_h<0 || img_w >=10240 || img_h >= 10240)
		return GDI_FAILED;


    /*
     * read the global color palette 
     */
    palette_count = GET_byte();
    FLUSH(2);

    if (palette_count & 0x80)
    {
        palette_count = 1 << ((palette_count & 0x7) + 1);
        FLUSH(palette_count * 3);
    }
    // /////////////////////////////////////////////////////////
    // start decoding
    gif_done = 0;

    do
    {
        switch (GET_byte())
        {
            case '!':   /* Process the GIF extension block */
                if (GET_byte() == 0xf9)
                {
                    FLUSH(6);
                }
                else
                {
                    S32 offset;

                    while (1)
                    {
                        offset = GET_byte();
                        if (offset == 0)
                        {
                            break;
                        }
                        FLUSH(offset);
                    }
                }
                break;
            case ',':
                gdi_image_gif_dummy_decoder();

                (*frame_count) += 1;

                /*
                 * peek if is last frame 
                 */
                if ((PEEK_byte() == '\0') && (PEEK_byte_N(1) == ';'))
                {
                    return GDI_SUCCEED; /* GDI_GIF_LAST_FRAME;; */
                }
                break;
            case '\0':
                if (IS_EOF())
                {
                    return GDI_SUCCEED; /* GDI_GIF_LAST_FRAME;; */
                }
                break;
            case ';':
                return GDI_SUCCEED; /* GDI_GIF_LAST_FRAME;; */
                break;
        }
    }
    while (!gif_done && !IS_EOF());

    return GDI_SUCCEED;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_draw
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ox                  [IN]        
 *  oy                  [IN]        
 *  GIF_src             [?]         
 *  size                [IN]        
 *  frame_number        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_draw(S32 ox, S32 oy, U8 *GIF_src, U32 size, U16 frame_number)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_draw)
    GDI_RESULT ret;

#if defined(GDI_USING_HW_GIF) || defined(GDI_USING_HW_GIF_V2)
    /* try hardware gif decoder first */
#ifdef GDI_USING_MEDIA_TASK_DECODER
    if (!gdi_image_decoder_is_created && 
        GDI_COLOR_FORMAT_32 != gdi_act_layer->vcf && 
        GDI_COLOR_FORMAT_1 != gdi_act_layer->vcf &&
        !(gdi_image_gif_codec_flag & GDI_IMAGE_CODEC_FLAG_USE_SW_DECODE))  /* can't run with async image decoder */
#endif 
    {
        BOOL is_transparent;

        ret = gdi_image_hwgif_display(GIF_src, size, ox, oy, frame_number, TRUE, &is_transparent);
        /* if(ret>=0) */
        GDI_RETURN(ret);
    }
#endif /* defined(GDI_USING_HW_GIF) || defined(GDI_USING_HW_GIF_V2) */ 
    GDI_TRY
    {
        ret = gdi_image_gif_draw_mem_decoders[gdi_act_layer->cf] (ox, oy, FALSE, 0, 0,  /* not resize */
                                                                  GIF_src, size,        /* source */
                                                                  (U32) GIF_src,        /* cache id use source address */
                                                                  frame_number, TRUE,   /* don't use disposal method */
                                                                  TRUE  /* transparent enable */
            );
    }
    GDI_CATCH_BEGIN
    {
        ret = GDI_FAILED;
    }
    GDI_CATCH_END GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_draw)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_draw_transparent_color
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ox                  [IN]        
 *  oy                  [IN]        
 *  GIF_src             [?]         
 *  size                [IN]        
 *  frame_number        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_draw_transparent_color(S32 ox, S32 oy, U8 *GIF_src, U32 size, U16 frame_number)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_draw_transparent_color)
    GDI_RESULT ret;

#if defined(GDI_USING_HW_GIF_V2)
    /* try hardware gif decoder first */
#ifdef GDI_USING_MEDIA_TASK_DECODER
    if (!gdi_image_decoder_is_created && 
        GDI_COLOR_FORMAT_32 != gdi_act_layer->vcf && 
        GDI_COLOR_FORMAT_1 != gdi_act_layer->vcf &&
        !(gdi_image_gif_codec_flag & GDI_IMAGE_CODEC_FLAG_USE_SW_DECODE))  /* can't run with async image decoder */
#endif 
    {
        ret = gdi_image_hwgif_display_internal(
                GIF_src,
                size,
                ox,
                oy,
                0,
                0,
                frame_number,
                FALSE,  /* is_file */
                FALSE,  /* is_resized */
                TRUE,   /* use_disposal_method */
                TRUE,   /* is_blocking, */
                FALSE,  /* transparent enable */
                NULL);

        /* if(ret>=0) */
        GDI_RETURN(ret);
    }
#endif /* defined(GDI_USING_HW_GIF_V2) */ 
    GDI_TRY
    {
        ret = gdi_image_gif_draw_mem_decoders[gdi_act_layer->cf] (ox, oy, FALSE, 0, 0,  /* not resize */
                                                                  GIF_src, size,        /* source */
                                                                  (U32) GIF_src,        /* cache id use source address */
                                                                  frame_number, TRUE,   /* don't use disposal method */
                                                                  FALSE /* transparent enable */
            );
    }
    GDI_CATCH_BEGIN
    {
        ret = GDI_FAILED;
    }
    GDI_CATCH_END GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_draw_transparent_color)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_draw_resized
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ox                  [IN]        
 *  oy                  [IN]        
 *  resized_width       [IN]        
 *  resized_height      [IN]        
 *  GIF_src             [?]         
 *  size                [IN]        
 *  frame_number        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_draw_resized(
            S32 ox,
            S32 oy,
            S32 resized_width,
            S32 resized_height,
            U8 *GIF_src,
            U32 size,
            U16 frame_number)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_draw_resized)
    GDI_RESULT ret;

#if defined(GDI_USING_HW_GIF_V2)
    /* try hardware gif decoder first */
#ifdef GDI_USING_MEDIA_TASK_DECODER
    if (!gdi_image_decoder_is_created && 
        GDI_COLOR_FORMAT_32 != gdi_act_layer->vcf && 
        GDI_COLOR_FORMAT_1 != gdi_act_layer->vcf &&
        !(gdi_image_gif_codec_flag & GDI_IMAGE_CODEC_FLAG_USE_SW_DECODE))  /* can't run with async image decoder */
#endif 
    {
        ret = gdi_image_hwgif_display_internal(
                (U8*) GIF_src,
                size,
                ox,
                oy,
                resized_width,
                resized_height,
                frame_number,
                FALSE,  /* is_file */
                TRUE,   /* is_resized */
                TRUE,   /* use_disposal_method */
                TRUE,   /* is_blocking, */
                TRUE,   /* transparent enable */
                NULL);

        /* if(ret>=0) */
        {
            GDI_RETURN(ret);
        }
    }
#endif /* defined(GDI_USING_HW_GIF_V2) */ 

    GDI_TRY
    {
        ret = gdi_image_gif_draw_mem_internal(
                ox,
                oy,
                TRUE,
                resized_width,
                resized_height, /* not resize */
                GIF_src,
                size,           /* source */
                (U32) GIF_src,  /* cache id use source address */
                frame_number,
                TRUE,           /* don't use disposal method */
                TRUE /* transparent enable */ );
    }
    GDI_CATCH_BEGIN
    {
        ret = GDI_FAILED;
    }
    GDI_CATCH_END GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_draw_resized)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_draw_with_background_color
 * DESCRIPTION
 *  Displays a given frame of the GIF file, and full layer will be filled with background color
 * PARAMETERS
 *  GIF_src                 [?]         
 *  size                    [IN]        
 *  frame_number            [IN]        Frame number to load for animated GIF.
 *  background_color        [IN]        
 *  is_transparent          [?]         
 *  file(?)                 [IN]        The GIF file in bytestream format
 *  oy(?)                   [IN]        Position at which the GIF image is to be displayed
 *  ox(?)                   [IN]        Position at which the GIF image is to be displayed
 *  color_depth(?)          [IN]        Output color depth.
 * RETURNS
 *  0  if GIF_load failed (invalid GIF image)
 *  1  if GIF_load is successful
 *****************************************************************************/
GDI_RESULT gdi_image_gif_draw_with_background_color(
            U8 *GIF_src,
            U32 size,
            U16 frame_number,
            gdi_color background_color,
            BOOL *is_transparent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_draw_with_background_color)
    GDI_RESULT ret;

    gdi_image_gif_codec_flag = gdi_image_codec_get_flag();
    gdi_image_gif_reset();  

#if defined(GDI_USING_HW_GIF)
    /* try hardware gif decoder first */
    if (!(gdi_image_gif_codec_flag & GDI_IMAGE_CODEC_FLAG_USE_SW_DECODE))
    {
        /* fill the frame buffer with 0xFFFF color (pure white) */
        gdi_memset16((U8*) gdi_act_layer->buf_ptr, GIF_SOFTKEY, gdi_act_layer->layer_size);

        ret = gdi_image_hwgif_display(GIF_src, size, 0, 0, frame_number, TRUE, is_transparent);
        if (*is_transparent == TRUE)
        {
            unsigned short *tempBuffer = (unsigned short*)gdi_act_layer->buf_ptr;
            int count = gdi_act_layer->width * gdi_act_layer->height;

            while (count--)
            {
                if (*tempBuffer == GIF_SOFTKEY)
                {
                    *tempBuffer = background_color;
                }
                tempBuffer++;
            }
        }
        if(ret>=0)
        {
            gdi_image_gif_codec_flag = 0;
            GDI_RETURN(ret);
        }
    }
#endif /* defined(GDI_USING_HW_GIF) */ 

    /* fill the frame buffer with background color */
    GDI_ASSERT(gdi_act_layer->bits_per_pixel == 16);
    gdi_memset16((U8*) gdi_act_layer->buf_ptr, (U16) background_color, gdi_act_layer->layer_size);     /* FIXME */

    GDI_TRY
    {
        gdi_image_gif_decoder_is_bypass_color = TRUE;
        gdi_image_gif_decoder_bypass_color = background_color;
        ret = gdi_image_gif_draw_mem_internal(
                0,
                0,
                FALSE,
                0,
                0,              /* not resize */
                GIF_src,
                size,           /* source */
                (U32) GIF_src,  /* cache id use source address */
                frame_number,
                TRUE,           /* don't use disposal method */
                TRUE /* transparent enable */ );
    }
    GDI_CATCH_BEGIN
    {
        ret = GDI_FAILED;
    }
    GDI_CATCH_END gdi_image_gif_decoder_is_bypass_color = FALSE;
    gdi_image_gif_codec_flag = 0;
    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_draw_with_background_color)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_draw_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ox                  [IN]        
 *  oy                  [IN]        
 *  filename            [IN]        
 *  frame_number        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_draw_file(S32 ox, S32 oy, const S8 *filename, U16 frame_number)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_draw_file)
    GDI_RESULT ret;
//    BOOL decode_by_sw = FALSE;
    U32 cache_id;
//    S16 lcd_idx;
//    S32 offset;
//    S32 frame_counter = 0;

    if (filename == NULL || gdi_bytestream_create_file((PS8) filename) != GDI_SUCCEED)
    {
        GDI_RETURN(GDI_FAILED);
    }
#if defined(GDI_USING_HW_GIF_V2)
    cache_id = gdi_image_gif_get_cache_key_from_file(gdi_bytestream_handle, filename, gdi_bytestream_file_size);
    decode_by_sw = gdi_image_gif_hit_cache(cache_id, frame_number, &lcd_idx, &frame_counter, &offset, FALSE);
    /* try hardware gif decoder first */
#ifdef GDI_USING_MEDIA_TASK_DECODER
    if (!gdi_image_decoder_is_created && 
        GDI_COLOR_FORMAT_32 != gdi_act_layer->vcf && 
        GDI_COLOR_FORMAT_1 != gdi_act_layer->vcf &&
        !(gdi_image_gif_codec_flag & GDI_IMAGE_CODEC_FLAG_USE_SW_DECODE)&&
        !decode_by_sw)  /* can't run with async image decoder */
#endif 
    {
        ret = gdi_image_hwgif_display_internal(
                (U8*) filename,
                0,
                ox,
                oy,
                0,
                0,
                frame_number,
                TRUE,   /* is_file */
                FALSE,  /* is_resized */
                TRUE,   /* use_disposal_method */
                TRUE,   /* is_blocking, */
                TRUE,   /* transparent enable */
                NULL);

        if(ret >= 0 || (frame_number > 0))
        {
            gdi_bytestream_free();
            GDI_RETURN(ret);
        }
    }
#endif /* defined(GDI_USING_HW_GIF_V2) */ 

    GDI_TRY
    {
        //U32  redefined  
			cache_id = gdi_image_gif_get_cache_key_from_file(gdi_bytestream_handle, filename, gdi_bytestream_file_size);

        /* ret = gdi_image_gif_draw_file_internal( */
        ret = gdi_image_gif_draw_file_decoders[gdi_act_layer->cf] (ox, oy, FALSE, 0, 0, /* not resize */
                                                                   0, 0,        /* no source in memory */
                                                                   cache_id,    /* cache id use source address */
                                                                   frame_number, TRUE,  /* don't use disposal method */
                                                                   TRUE);
    }
    GDI_CATCH_BEGIN
    {
        ret = GDI_FAILED;
    }
    GDI_CATCH_END gdi_image_gif_progress_callback = NULL;

    gdi_bytestream_free();

    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_draw_file)
}

GDI_RESULT


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_draw_resized_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ox                  [IN]        
 *  oy                  [IN]        
 *  resized_width       [IN]        
 *  resized_height      [IN]        
 *  filename            [IN]        
 *  frame_number        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
gdi_image_gif_draw_resized_file(
    S32 ox,
    S32 oy,
    S32 resized_width,
    S32 resized_height,
    const S8 *filename,
    U16 frame_number)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_draw_resized_file)

    GDI_RESULT ret;

#if 0//defined(GDI_USING_HW_GIF_V2)
#ifdef GDI_USING_MEDIA_TASK_DECODER
#endif 
#endif /* defined(GDI_USING_HW_GIF_V2) */ 

    if (filename == NULL || gdi_bytestream_create_file((PS8) filename) != GDI_SUCCEED)
    {
        GDI_RETURN(GDI_FAILED);
    }

    GDI_TRY
    {
        U32 cache_id = gdi_image_gif_get_cache_key_from_file(gdi_bytestream_handle, filename, gdi_bytestream_file_size);

        ret = gdi_image_gif_draw_file_internal(
                ox,
                oy,
                TRUE,
                resized_width,
                resized_height, /* not resize */
                0,
                0,              /* no source in memory */
                cache_id,       /* cache id use source address */
                frame_number,
                TRUE,           /* don't use disposal method */
                TRUE);
    }
    GDI_CATCH_BEGIN
    {
        ret = GDI_FAILED;
    }
    GDI_CATCH_END gdi_image_gif_progress_callback = NULL;

    gdi_bytestream_free();

    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_draw_resized_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_dimension
 * DESCRIPTION
 *  
 * PARAMETERS
 *  GIF_src     [?]     
 *  width       [?]     
 *  height      [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_get_dimension(U8 *GIF_src, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_get_dimension)
    GDI_RETURN(gdi_image_gif_get_dimension_mem_internal(GIF_src, 14, width, height));
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_get_dimension)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_dimension_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  filename        [IN]        
 *  width           [?]         
 *  height          [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_get_dimension_file(const S8 *filename, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_get_dimension_file)
    GDI_RESULT ret;

    if (filename == NULL || gdi_bytestream_create_file((PS8) filename) != GDI_SUCCEED)
    {
        *width = 0;
        *height = 0;
        return GDI_FAILED;
    }

    GDI_TRY
    {
        ret = gdi_image_gif_get_dimension_file_internal(0, 0, width, height);
    }
    GDI_CATCH_BEGIN
    {
        *width = 0;
        *height = 0;
        ret = GDI_FAILED;
    }
    GDI_CATCH_END gdi_bytestream_free();

    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_get_dimension_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_frame_count
 * DESCRIPTION
 *  
 * PARAMETERS
 *  GIF_src         [?]         
 *  size            [IN]        
 *  frame_count     [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_get_frame_count(U8 *GIF_src, U32 size, S32 *frame_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_get_frame_count)
    GDI_RESULT ret;

    *frame_count = 0;

    if (gdi_bytestream_create(GIF_src, size) != GDI_SUCCEED)
    {
        GDI_RETURN(GDI_FAILED);
    }

    GDI_TRY
    {
        ret = gdi_image_gif_get_frame_count_internal(frame_count);
    }
    GDI_CATCH_BEGIN
    {
        /*
         * keep the available frame count? 
         */
        /* *frame_count = 0; */
        ret = GDI_FAILED;
    }
    GDI_CATCH_END gdi_bytestream_free();

    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_get_frame_count)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_frame_count_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  filename        [IN]        
 *  frame_count     [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_get_frame_count_file(const S8 *filename, S32 *frame_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_get_frame_count_file)
    GDI_RESULT ret;

    *frame_count = 0;

    if (filename == NULL || gdi_bytestream_create_file((PS8) filename) != GDI_SUCCEED)
    {
        GDI_RETURN(GDI_FAILED);
    }

    GDI_TRY
    {
        ret = gdi_image_gif_get_frame_count_internal(frame_count);
    }
    GDI_CATCH_BEGIN
    {
        /*
         * keep the available frame count? 
         */
        /* *frame_count = 0; */
        ret = GDI_FAILED;
    }
    GDI_CATCH_END gdi_bytestream_free();

    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_get_frame_count_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_reset
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_image_gif_reset(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_reset)
    int i;

    for (i = 0; i < GIF_CACHE_COUNT; i++)
    {
        gdi_gif_cache[i].cache[0] = 0;
    }
	#ifdef GDI_USING_HW_GIF_V2
		gif_reset_cache();
	#endif
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_reset)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_set_progress_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ptr     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_image_gif_set_progress_callback(gdi_imaeg_gif_callback_ptr ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_image_gif_progress_callback = ptr;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_transparent_index
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
U16 gdi_image_gif_get_transparent_index(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_get_transparent_index)
    GDI_RETURN(gdi_image_gif_transparent_index);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_get_transparent_index)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_palette_size
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
U8 gdi_image_gif_get_palette_size(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_gif_get_palette_size)
    GDI_RETURN(gdi_image_gif_palette_size);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_gif_get_palette_size)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_draw_handler
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
GDI_RESULT gdi_image_gif_draw_handler(U32 flag, U32 frame_pos, S32 x, S32 y, S32 w, S32 h, U8 *data_ptr, U32 img_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 frame_number = (U16) frame_pos;
    GDI_RESULT result = GDI_FAILED;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_image_gif_codec_flag = flag;
    if (flag & GDI_IMAGE_CODEC_FLAG_IS_FILE)
    {
        if (w && h)
        {
            result = gdi_image_gif_draw_resized_file(x, y, w, h, (S8*) data_ptr, frame_number);
        }
        else
        {
            result = gdi_image_gif_draw_file(x, y, (S8*) data_ptr, frame_number);
        }
    }
    else
    {
        if (w && h)
        {
            result = gdi_image_gif_draw_resized(x, y, w, h, data_ptr, img_size, frame_number);
        }
        else
        {
            result = gdi_image_gif_draw(x, y, data_ptr, img_size, frame_number);
        }
    }
    gdi_image_gif_codec_flag = 0;
    return result;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_dimension_handler
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
GDI_RESULT gdi_image_gif_get_dimension_handler(U32 flag, U8 *data_ptr, U32 img_size, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (flag & GDI_IMAGE_CODEC_FLAG_IS_FILE)
    {
        return gdi_image_gif_get_dimension_file((S8*) data_ptr, width, height);
    }
    else
    {
        return gdi_image_gif_get_dimension(data_ptr, width, height);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_frame_count_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  flag            [IN]        
 *  data_ptr        [?]         
 *  img_size        [IN]        
 *  frame_count     [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_get_frame_count_handler(U32 flag, U8 *data_ptr, U32 img_size, S32 *frame_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (flag & GDI_IMAGE_CODEC_FLAG_IS_FILE)
    {
        return gdi_image_gif_get_frame_count_file((S8*) data_ptr, frame_count);
    }
    else
    {
        return gdi_image_gif_get_frame_count(data_ptr, img_size, frame_count);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_gif_get_pos_info_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  flag            [IN]        
 *  data_ptr        [?]         
 *  img_size        [IN]        
 *  quick_mode      [IN]        
 *  total_pos       [?]         
 *  pos_type        [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_gif_get_pos_info_handler(
            U32 flag,
            U8 *data_ptr,
            U32 img_size,
            BOOL quick_mode,
            S32 *total_pos,
            gdi_anim_pos_type_enum *pos_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!quick_mode)
    {
        *pos_type = GDI_ANIM_POS_TYPE_INDEX;
        if (flag & GDI_IMAGE_CODEC_FLAG_IS_FILE)
        {
            return gdi_image_gif_get_frame_count_file((S8*) data_ptr, (S32*) total_pos);
        }
        else
        {
            return gdi_image_gif_get_frame_count(data_ptr, img_size, (S32*) total_pos);
        }
    }
    return GDI_FAILED;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_is_gif_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  file_name       [?]     
 * RETURNS
 *  
 *****************************************************************************/
BOOL gdi_image_is_gif_file(U8 *file_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FS_HANDLE f;
    U8 buf[3];
    INT32 len;
    U32 result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    f = DRM_open_file((U8*) file_name, FS_READ_ONLY | FS_OPEN_NO_DIR, 0);
    if (f < 0)
    {
        return FALSE;
    }

    result = DRM_read_file(f, buf, 3, &len);

    DRM_close_file(f);

    if (result == FS_NO_ERROR)
        if (len == 3)
            if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
            {
                return TRUE;
            }
    return FALSE;
}

