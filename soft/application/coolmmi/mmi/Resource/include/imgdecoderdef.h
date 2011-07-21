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
 *  ImgDecoderDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/Resource/include/ImgDecoderDef.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:02:50   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:43:12   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:28:12   zhangxb
 * Initial revision.
 * Revision 1.1.1.1  2006/06/08 12:43:25  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef __IMG_DECODER_DEF_H__
#define __IMG_DECODER_DEF_H__

#include "resbytestream.h"

#define WINGUI_DIRECT_IMAGE_DISPLAY				0
#define WINGUI_HW_COLOR_DEPTH_12				1
#define DEVICE_COLOR_DEPTH						12
#define	IMAGE_STATIC_ROW_DATA_SIZE				1024
#define IMAGE_STATIC_PALETTE_SIZE				768
#define IMAGE_STATIC_BUFFER_SIZE				409600
#define UI_UNUSED_PARAMETER(x)					(void)(x)

/*	MACROS defined by Pluto	*/

 
/* Original Macro Setting*/
/*
//#define  MMI_RGB_TO_HW_FORMAT(R,G,B)	( (((B)&0xf8)<<5)|(((R)&0xf8))|(((G)&0xe0)>>5)|(((G)&0x1c)<<11) )
#define  MMI_RGB_TO_HW_FORMAT(R,G,B)	( (((B)&0xf8)>>3)|(((G)&0xfc)<<3)|(((R)&0xf8)<<8) )
*/

/* New Macro Setting */
#if defined(MT6205B)
#if defined(KM122_LCM)
#define  MMI_RGB_TO_HW_FORMAT(R,G,B)   ( (((B)&0xf8)>>3)|(((G)&0xfc)<<3)|(((R)&0xf8)<<8) )
#else 
#define  MMI_RGB_TO_HW_FORMAT(R,G,B)   ( (((B)&0xf8)<<5)|(((R)&0xf8))|(((G)&0xe0)>>5)|(((G)&0x1c)<<11) )
#endif 
#elif (defined(MTKLCM_COLOR) || defined(INFOLCM) || defined(POWERTIP_LCM) || defined(SCORPIOB1_LCM)) && (defined(MT6208) || defined(MT6205) || defined(MT6218))
#define  MMI_RGB_TO_HW_FORMAT(R,G,B)   ( (((B)&0xf8)<<5)|(((R)&0xf8))|(((G)&0xe0)>>5)|(((G)&0x1c)<<11) )
#else 
#define  MMI_RGB_TO_HW_FORMAT(R,G,B)   ( (((B)&0xf8)>>3)|(((G)&0xfc)<<3)|(((R)&0xf8)<<8) )
#endif 
#define  MMI_RGB_TO_HW_FORMAT_1(A,R,G,B)  ( ((U16)(((S32)(R)+(S32)(G)+(S32)(B))/(S32)3)>=0x80)? 1:0 )
#define  MMI_RGB_TO_HW_FORMAT_16(A,R,G,B) MMI_RGB_TO_HW_FORMAT(R,G,B)
#define  MMI_RGB_TO_HW_FORMAT_24(A,R,G,B) ((R << 16) | (G << 8) | B)
#define  MMI_RGB_TO_HW_FORMAT_32(A,R,G,B) ((A << 24) | (R << 16) | (G << 8) | B)

/*	Macros to handle color omission transparency	*/
#define RGB_TRANSPARENT_COLOR_16				(0x1234)
#define TEST_RGB_TRANSPARENT_COLOR_16(value)	((value)==RGB_TRANSPARENT_COLOR_16)
#define SET_RGB_TRANSPARENT_COLOR_16(value)		((value)=RGB_TRANSPARENT_COLOR_16)
#define RGB_TRANSPARENT_COLOR_24                (0x123456)
#define TEST_RGB_TRANSPARENT_COLOR_24(value)    ((value)==RGB_TRANSPARENT_COLOR_24)
#define SET_RGB_TRANSPARENT_COLOR_24(value)     ((value)=RGB_TRANSPARENT_COLOR_24)
#define RGB_TRANSPARENT_COLOR_32                (0x12345678)
#define TEST_RGB_TRANSPARENT_COLOR_32(value)    ((value)==RGB_TRANSPARENT_COLOR_32)
#define SET_RGB_TRANSPARENT_COLOR_32(value)     ((value)=RGB_TRANSPARENT_COLOR_32)

typedef struct _bitmap
{
    S32 xsize;
    S32 ysize;
    S8 color_depth;
    S32 row_bytes;
    U8 *palette;
    U8 *data;
} bitmap;

/*	Support for BMP files		*/

typedef struct _bitmap_file_header
{
    U16 file_type;
    U32 file_size;
    U16 reserved1;
    U16 reserved2;
    U32 bitmap_offset;
} bitmap_file_header;

typedef struct _bitmap_info_header
{
    U32 header_size;
    U32 width;
    U32 height;
    U16 number_of_planes;
    U16 bits_per_pixel;
    U32 compression;
    U32 bitmap_size;
    U32 device_width;
    U32 device_height;
    U32 number_of_colors;
    U32 number_of_important_colors;
} bitmap_info_header;

extern void BMP_load_file_header(bitmap_file_header *h, bytestream *file);
extern void BMP_load_info_header(bitmap_info_header *h, bytestream *file);
extern U8 BMP_load(bytestream *file, bitmap *b, S8 color_depth);
#endif /* __IMG_DECODER_DEF_H__ */ 

