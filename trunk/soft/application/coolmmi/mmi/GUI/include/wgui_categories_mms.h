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

/*****************************************************************************
 * Filename:
 * ---------
 *  wgui_categories_MMS.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  MMS related categories. 
 *  [Category275]       MMS SMIL editor
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef __WGUI_CATEGORIES_MMS_H__
#define __WGUI_CATEGORIES_MMS_H__

#ifndef __WGUI_CATEGORIES_INPUTS_H__
#error "wgui_categories_inputs.h should be included before wgui_categories_MMS.h "
#endif 

/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/

typedef void (*wgui_cat275_draw_title_hdlr) (U32 identifier);
typedef void (*wgui_cat275_draw_area_hdlr) (U32 identifier, S32 y_offset, S32 clip_x1, S32 clip_y1, S32 clip_x2,
                                            S32 clip_y2);

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

extern void RedrawCategory275Screen(void);
extern void ShowCategory275Screen(
                U32 identifier,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                S16 input_type,
                U8 *buffer,
                S32 buffer_size,
                U16 vbar_width,
                wgui_cat275_draw_title_hdlr draw_title_fp,
                U16 draw_area_1_height,
                wgui_cat275_draw_area_hdlr draw_area_1_fp,
                U16 draw_area_2_height,
                wgui_cat275_draw_area_hdlr draw_area_2_fp,
                U8 *history_buffer);

extern void ShowCategory277Screen(
                U32 identifier,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                S16 input_type,
                U8 *buffer,
                S32 buffer_size,
                U16 vbar_width,
                wgui_cat275_draw_title_hdlr draw_title_fp,
                U16 draw_area_1_height,
                wgui_cat275_draw_area_hdlr draw_area_1_fp,
                U16 draw_area_2_height,
                wgui_cat275_draw_area_hdlr draw_area_2_fp,
                UI_font_type *text_font,
                color_t *text_color,
                color_t *bg_color,
                U8 *history_buffer);

#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__) || defined(__MMI_SMART_MESSAGE_MT__)
#endif /* defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__) || defined(__MMI_SMART_MESSAGE_MT__) */ 

extern void ExitCategory275Screen(void);
extern S32 GetCategory275HistorySize(void);
extern U8 *GetCategory275History(U8 *history_buffer);
void SetCategory275RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
extern void Category275GotoTop(void);
extern void Category275GotoBottom(void);
extern void Category275SetUnmodified(void);
extern BOOL Category275CheckIfModified(void);

extern color_t(*category275_get_bg_color) (void);         /* to set background color */
extern color_t(*category275_get_text_color) (void);       /* to set editor text color. */
extern UI_font_type(*category275_get_text_font) (void); /* to set editor text font. */

extern void RedrawCategory276Screen(void);
extern void ShowCategory276Screen(
                U8 *title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *history_buffer);
extern void DrawCat276SlidingBar(U8 percentage);
extern void ExitCategory276Screen(void);
extern void DrawCat276Title(U8 *title);
extern void dm_category_276_controlled_area(dm_coordinates *coordinate);
extern void register_category275_ui_callback_functions(
                void *bg_color_callback,
                void *text_color_callback,
                void *text_font_callback);

#endif /* __WGUI_CATEGORIES_MMS_H__ */ 

