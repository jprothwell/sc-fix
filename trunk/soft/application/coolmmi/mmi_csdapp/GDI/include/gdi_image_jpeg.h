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
 *  gdi_image_jpeg.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Image Jpeg header file.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _GDI_IMAGE_JPEG_H_
#define _GDI_IMAGE_JPEG_H_

extern BOOL gdi_image_is_jpeg_file(U8 *file_name);
extern GDI_RESULT gdi_image_jpeg_draw_handler(
                    U32 flag,
                    U32 frame_pos,
                    S32 x,
                    S32 y,
                    S32 w,
                    S32 h,
                    U8 *data_ptr,
                    U32 img_size);
extern GDI_RESULT gdi_image_jpeg_get_dimension_handler(
                    U32 flag,
                    U8 *data_ptr,
                    U32 img_size,
                    S32 *width,
                    S32 *height);
extern GDI_RESULT gdi_image_hwjpeg_draw_internal(
                            S32 ox,
                            S32 oy,
                            S32 resized_width,
                            S32 resized_height,
                            S32 output_clipx1,
                            S32 output_clipy1,
                            S32 output_clipx2,
                            S32 output_clipy2,
                            U8 *src,
                            U32 size,
                            BOOL is_file,
                            BOOL is_resized,
                            U8 *dest_buf,
                            S32 dest_buf_size,
                            S32 *ret_decoded_image_width,
                            gdi_color_format dest_buf_cf);
#endif /* _GDI_IMAGE_JPEG_H_ */ 

