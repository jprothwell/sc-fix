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
 *  gdi_features.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Features Set.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _GDI_FEATURES_H_
#define _GDI_FEATURES_H_
#if !(defined(MT6205B) || defined(MT6208))
#define GDI_USING_MUTEX
#define GDI_USING_LAYER
#if defined(MT6228) || defined(MT6229) || defined(MT6230)
#define GDI_6_LAYERS
#define GDI_LAYER_HW_LAYER_COUNT       6
#else
#define GDI_4_LAYERS
#define GDI_LAYER_HW_LAYER_COUNT       4
#endif 
#define GDI_LAYER_TREE_MAX_DEPTH       8

#if defined(MT6226) || defined(MT6227) || defined(MT6228) || defined(MT6229) || defined(MT6226M) || defined(MT6230)
#define GDI_USING_LAYER_BACKGROUND
#endif 

#if defined(MT6226) || defined(MT6227) || defined(MT6228) || defined(MT6229) || defined(MT6226M) || defined(MT6230)
#define GDI_USING_LAYER_OUTPUT_ANOTHER_MEMORY
#endif 

#if defined(MT6228) || defined(MT6229) || defined(MT6230)
#define GDI_USING_TV_OUTPUT
#endif 
#else   /* in 6205B , code still use 4 layer, but no hardware layer feature */
#define GDI_LAYER_HW_LAYER_COUNT 4
#define GDI_LAYER_TREE_MAX_DEPTH 4

#endif   

#if defined(MT6228) || defined(MT6229) || defined(MT6230)
#define GDI_USING_LAYER_COLOR_FORMAT
#endif 

#ifdef USE_HW_GIF_DECODER_V1
#define GDI_USING_HW_GIF
#endif 

#ifdef USE_HW_GIF_DECODER_V2
#define GDI_USING_HW_GIF_V2
#endif 

#if defined(MT6219) || defined(MT6226) || defined(MT6227) || defined(MT6226M)
#define GDI_USING_2D_ENGINE
#endif 

#if defined(MT6228) || defined(MT6229) || defined(MT6230)
#define GDI_USING_2D_ENGINE_V2
#endif 

//disable LCD rotate
#if 0 
#define GDI_USING_LAYER_ROTATE
#endif 

#if defined(LCM_ROTATE_SUPPORT)
#define GDI_USING_LCD_ROTATE
#endif 

#if (defined(JPG_DECODE) || defined(JPG_ENCODE)) && (defined(MT6219) || defined(MT6226) || defined(MT6227) || defined(MT6228) || defined(MT6229) || defined(MT6226M)) || defined(MT6230)
#define GDI_USING_HW_RESIZER
#endif 

#if defined(M3D_MMI_SUPPORT)
#define GDI_USING_M3D
#endif 

#if defined(SVG_SUPPORT) && !defined(MMI_ON_WIN32)
#define GDI_USING_SVG
#endif

///////////////////////////////////////////////////////////////
/// JPEG
#if defined (JPG_DECODE) || defined (JPG_ENCODE)
#define GDI_USING_JPEG
#define GDI_USING_HW_JPEG
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-3-11 16:18 */
#if 0
#define GDI_USING_MEDIA_TASK_DECODER
#endif
  #if (defined (JPG_DECODE_USING_SOFTWARE) && defined (GDI_USING_HW_JPEG))
  #define GDI_USING_SW_JPEG
  #endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-3-11 16:18 */
#if defined(MT6219) || defined(MT6226) || defined(MT6227) || defined(MT6228) || defined(MT6229) || defined(MT6226M) || defined(MT6230)
#define GDI_HW_JPEG_SUPPORT_OUTPUT_CLIPPING
#endif 
#if defined(MT6228) || defined(MT6229) || defined(MT6230)
#define GDI_HW_JPEG_SUPPORT_OUTPUT_ANOTHER_BUFFER
#define GDI_HW_JPEG_SUPPORT_DIRECT_OUTPUT_BUFFER
#define GDI_HW_JPEG_SUPPORT_COLOR_FORMAT
#endif
#if defined(MT6218) || defined(MT6218B) || defined(MT6217)
#define GDI_HW_JPEG_PADDING_PITCH
#endif 
#endif /* defined (JPG_DECODE) || defined (JPG_ENCODE) */ 

//////////////////////////////////////////////////////////////
/// PNG
#if defined(USE_HW_PNG_DECODER_V1) || defined(USE_SW_PNG_DECODER)
#define GDI_USING_PNG
#define GDI_USING_HW_PNG
#endif /* USE_HW_PNG_DECODER_V1 */ 

//////////////////////////////////////////////////////////////
/// Palette control
#if defined(MT6217) || defined(MT6218B) || defined(MT6219)
#define GDI_SUPPORT_PALETTES     2
#elif  defined(MT6226) || defined(MT6227) || defined(MT6228) || defined(MT6229) || defined(MT6226M) || defined(MT6230)
#define GDI_SUPPORT_PALETTES     1
#endif 

// For MOS/Windows simulator.
#ifdef WIN32
#undef GDI_USING_HW_GIF
#undef GDI_USING_HW_GIF_V2
#undef GDI_USING_2D_ENGINE
/*#undef GDI_USING_HW_JPEG*/ /* delete by zhuoxz, 2009-10-10 */
#undef GDI_USING_HW_RESIZER
//#undef GDI_USING_HW_PNG
//#undef GDI_USING_LCD_ROTATE

#define GDI_USING_JPEG
#define GDI_USING_W32_JPEG
//#define GDI_USING_PNG
//#define GDI_USING_W32_PNG
#endif /* WIN32 */ 

/* For PC_SIMULATOR only */
#ifdef MMI_ON_WIN32
#undef GDI_USING_MEDIA_TASK_DECODER
#undef GDI_USING_JPEG
#undef GDI_USING_W32_JPEG
#undef GDI_USING_PNG
#undef GDI_USING_HW_PNG
#undef GDI_USING_W32_PNG
//#undef GDI_USING_LCD_ROTATE
#undef GDI_USING_M3D
#undef GDI_USING_2D_ENGINE_V2
#endif /* MMI_ON_WIN32 */ 

#endif /* _GDI_FEATURES_H_ */ 

