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
 *  gdi_layer.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Layer header file.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _GDI_LAYER_H_
#define _GDI_LAYER_H_
#include "lcd_if.h"
/* this data structure is defined in lcd_if.h, but will not include when build on older platform */
/* we should add by ourself */

#ifdef GDI_6_LAYERS
#define GDI_LAYER_ENABLE_LAYER_ALL        (LCD_LAYER0_ENABLE|LCD_LAYER1_ENABLE\
                                 |LCD_LAYER2_ENABLE|LCD_LAYER3_ENABLE         \
                                 |LCD_LAYER4_ENABLE|LCD_LAYER5_ENABLE         \
                                 )
#else /* GDI_6_LAYERS */ 
#define GDI_LAYER_ENABLE_LAYER_ALL        (LCD_LAYER0_ENABLE|LCD_LAYER1_ENABLE\
                                 |LCD_LAYER2_ENABLE|LCD_LAYER3_ENABLE         \
                                 )
#endif /* GDI_6_LAYERS */ 

/* user define handle */
#ifndef __MMI_SUBLCD__
#define GDI_LAYER_USER_ALLOCATE_HANDLE_START (1)
#else 
#define GDI_LAYER_USER_ALLOCATE_HANDLE_START (2)
#endif 
#define GDI_LAYER_USER_ALLOCATE_HANDLE_END   (GDI_LAYER_TOTAL_LAYER_COUNT-1)

#define GDI_LAYER_VALID_HANDLE_START            ((gdi_handle)&GDI_LAYERS[0])
/****************************************************************************
* Struct / Typedef                                                                 
*****************************************************************************/

extern lcd_layer_struct gdi_layer_info[GDI_LAYER_TOTAL_LAYER_COUNT];    /* this struct is used for output lcd */
extern lcd_layer_struct *gdi_act_layer_info;

#define GDI_LAYER_FLAG_USED               (1<<0)
#define GDI_LAYER_FLAG_DOUBLE_LAYER       (1<<1)
#define GDI_LAYER_FLAG_USE_OUTSIDE_MEMORY (1<<2)
#define GDI_LAYER_FLAG_FROZEN             (1<<3)

#define GDI_LAYER_CLEAR_FLAG(HANDLE,FLAG)    ((gdi_layer_struct*)HANDLE)->flag  &= ~(U32)(FLAG)
#define GDI_LAYER_SET_FLAG(HANDLE,FLAG)      ((gdi_layer_struct*)HANDLE)->flag  |= (FLAG)
#define GDI_LAYER_GET_FLAG(HANDLE,FLAG)      (((gdi_layer_struct*)HANDLE)->flag  & (FLAG))

#define GDI_LAYER_SET_CLIP(HANDLE,X1,Y1,X2,Y2)  \
   do                                           \
   {                                            \
      ((gdi_layer_struct*)HANDLE)->clipx1=X1;   \
      ((gdi_layer_struct*)HANDLE)->clipy1=Y1;   \
      ((gdi_layer_struct*)HANDLE)->clipx2=X2;   \
      ((gdi_layer_struct*)HANDLE)->clipy2=Y2;   \
   }while(0)

#define GDI_LAYER_RESET_CLIPS(HANDLE)  ((gdi_layer_struct*)HANDLE)->clips_top = 0

extern gdi_handle gdi_work_buffer_handle;
extern gd_replace_src_key_func gdi_act_replace_src_key;

/***************************************************************************** 
* Function 
*****************************************************************************/
/* layers related */
extern GDI_RESULT gdi_layer_init(void);

#endif /* _GDI_LAYER_H_ */ 

