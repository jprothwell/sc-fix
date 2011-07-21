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
 *  wgui_categories_popup.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Popup Screen related categories.
 *  Author:
 * -------
 
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef WGUI_CATEGORIES_POPUP_H
#define WGUI_CATEGORIES_POPUP_H
#include "mmi_features.h"
#include "gui_data_types.h"
#include "custthemesres.h"

#ifdef __MMI_HANDWRITING_PAD__
#include "touchscreengprot.h"   /* for handwriting pad config */
#endif 

/*  POPUP screen attribute  */
#define UI_POPUP_NOTIFYDURATION_TIME            1000
#define UI_POPUP_NOTIFYDURATION_TIME_2000       2000

extern void wgui_draw_pop_up_screen_background(
                S32 x1,
                S32 y1,
                S32 x2,
                S32 y2,
                UI_filled_area *f,
                MMI_BOOL enable_3D_effect);
extern void wgui_draw_pop_up_screen_background_style2(
                S32 x1,
                S32 y1,
                S32 x2,
                S32 y2,
                UI_filled_area *f,
                MMI_BOOL top,
                MMI_BOOL bottom,
                MMI_BOOL left,
                MMI_BOOL right);

extern void RedrawCategory62Screen(void);
extern void ShowCategory62Screen(U16 message, U16 message_icon, U8 *history_buffer);
extern void ExitCategory62Screen(void);

extern void RedrawCategory63Screen(void);
extern void ShowCategory63Screen(U8 *message, U16 message_icon, U8 *history_buffer);
extern void ExitCategory63Screen(void);

extern void RedrawCategory64Screen(void);
extern void ShowCategory64Screen(U16 message, U16 message_icon, U8 *history_buffer);
extern void ExitCategory64Screen(void);

extern void ShowCategory65Screen(U8 *message, U16 message_icon, U8 *history_buffer);
extern void RedrawCategory65Screen(void);
extern void ExitCategory65Screen(void);

extern void ExitCategory121Screen(void);
extern void RedrawCategory121Screen(void);
extern void ShowCategory121Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *message,
                U16 message_icon,
                U8 *history_buffer,
                U8 flag);

extern void RedrawCategory123Screen(void);
extern void ExitCategory123Screen(void);
extern void ShowCategory123Screen(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U16 message,
                U16 message_icon,
                U8 *history_buffer);

extern void RedrawCategory124Screen(void);
extern void ExitCategory124Screen(void);
extern void ShowCategory124Screen(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *message,
                U16 message_icon,
                U8 *history_buffer);

extern void ShowCategory151Screen(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *message,
                U8 *message_icon,
                U8 *history_buffer);
extern void ShowCategory151Screen_ex(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *message,
                U8 *message_icon,
                U8 *history_buffer);
extern void ExitCategory151Screen(void);
extern void RedrawCategory151Screen(void);

extern void RedrawCategory162Screen(void);
extern void ShowCategory162Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U16 message,
                U16 message_icon,
                U8 *history_buffer);
extern void ExitCategory162Screen(void);

extern void RedrawCategory163Screen(void);
extern void ShowCategory163Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *message,
                U16 message_icon,
                U8 *history_buffer);
extern void ExitCategory163Screen(void);

extern void RedrawCategory164Screen(void);
extern void ShowCategory164Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U16 message,
                U16 message_icon,
                U8 *history_buffer);
extern void ExitCategory164Screen(void);

extern void ShowCategory165Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                UI_string_type message,
                U16 message_icon,
                U8 *history_buffer);
extern void RedrawCategory165Screen(void);
extern void ExitCategory165Screen(void);
extern void ShowCategory165Screen_ext(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                UI_string_type message,
                U16 message_icon,
                S32 duration,
                U8 *history_buffer);
extern void SetCat165TimerExpireFunc(FuncPtr f);

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
extern void UpdateCalibrationScreen(U16 image_id, S16 cali_x, S16 cali_y, U16 string, U16 state, BOOL redraw);
extern void ExitCalibrationScreen(void);
extern void RedrawCalibrationScreen(void);
extern void ShowCalibrationScreen(U16 image_id, S16 cali_x, S16 cali_y, U16 string, U16 state);

extern void ShowCategory264Screen(UI_string_type message, PU8 message_icon, U16 dial_icon, void *para, U8 *history_buffer);
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

#ifdef __MMI_HANDWRITING_PAD__
extern U16 UpdateTestControlAreaScreen(mmi_pen_point_struct pos, U16 string, U16 state, BOOL redraw);
extern void ExitTestControlAreaScreen(void);
extern void RedrawTestControlAreaScreen(void);
extern void ShowTestControlAreaScreen(mmi_pen_point_struct pos, U16 string, U16 state);

#endif /* __MMI_HANDWRITING_PAD__ */ 

#endif /* WGUI_CATEGORIES_POPUP_H */ 

