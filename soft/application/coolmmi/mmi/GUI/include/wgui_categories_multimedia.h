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
*
 * Filename:
 * ---------
 *  wgui_categories_multimedia.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  MultiMedia related categories.
 *  [Category221]       Empty Screen
 *  [Category222]       View Image Screen
 *  [Category223]       Play Audio Screen
 *  [Category224]       Play Video Full Screen
 *  [Category225]       Play Video Standard Screen (With title & softkey)
 *  [Category331][SUB]  Emptry Sublcd Screen
 *  [Category332][SUB]  Draw a image at center of Sublcd Screen
 *  [Category333][SUB]  Play Audio Sublcd Screen
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef __WGUI_CATEGORIES_MULTIMEDIA_H__
#define __WGUI_CATEGORIES_MULTIMEDIA_H__

/***************************************************************************** 
* Define
*****************************************************************************/
#ifdef __MMI_VIDEO_PLAYER__//061506 double buffer
#include "lcd_sw_rnd.h"
#include "gdi_lcd_config.h"
#include "gdi_include.h"
#define BLACK_BUFFER_SIZE   ((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)
#else
#define BLACK_BUFFER_SIZE   (0)
#endif
#include "gui_setting.h"
#include "wgui.h"
#define STATUS_ICON_BAR_DOUBLE_BUFFER_SIZE  (((MAIN_LCD_DEVICE_WIDTH*MMI_STATUS_BAR_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)*2)

typedef enum _cat226_screen_layout_enum
{
    VERTICAL_NORMAL,
    VERTICAL_FULLSCREEN,
    HORIZONTAL_NORMAL,
    HORIZONTAL_FULLSCREEN
} cat226_screen_layout_enum;

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

/* Empty screen category - With title bar and softkey */
extern void ShowCategory221Screen(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                gdi_color background_color,
                FuncPtr redraw_content_callback);

/* View image category */
gdi_handle cat222_get_image_handle(void);//070706
extern void ShowCategory222Screen(U16 title,
                                  U16 title_icon,
                                  U16 left_softkey,
                                  U16 left_softkey_icon,
                                  U16 right_softkey,
                                  U16 right_softkey_icon,
                                  gdi_color background_color,
                                  S8 *button_string,
                                  S8 *file_name_ptr, BOOL is_short, void (*decode_result_callback) (S32));

/* Play audio category */
extern void ShowCategory223Screen(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U16 message_icon,
                U16 flag,
                U32 duration,
                U8 *history_buffer);

/* play fullscreen video */
extern void ShowCategory224Screen(U16 video_id,
                                  PS8 video_filename,
                                  U16 repeat_count,
                                  BOOL is_visaul_update,
                                  BOOL is_play_audio,
                                  BOOL is_lcd_no_sleep,
                                  GDI_COLOR bg_color, void (*play_finish_callback) (S32), void *gui_buffer);

extern void StopCategory224Video(void);

/* play video on standard screen (title & softkey) */
extern void ShowCategory225Screen(U16 title_str,
                                  U16 title_icon,
                                  U16 lsk_str,
                                  U16 lsk_icon,
                                  U16 rsk_str,
                                  U16 rsk_icon,
                                  U16 video_id,
                                  PS8 video_filename,
                                  U16 repeat_count,
                                  BOOL is_visual_update,
                                  BOOL is_play_audio,
                                  BOOL is_lcd_no_sleep,
                                  GDI_COLOR bg_color, void (*play_finish_callback) (S32), void *gui_buffer);

/* [Category 226] */
extern void ShowCategory226Screen(U16 left_softkey, U16 left_softkey_icon, U16 right_softkey, U16 right_softkey_icon,
                                  U16 bg_image_id, UI_string_type str_1, UI_string_type str_2,
                                  U16 left_arrow_up, U16 left_arrow_down, FuncPtr event_callback1,
                                  U16 right_arrow_up, U16 right_arrow_down, FuncPtr event_callback2,
                                  U16 icon1_up, U16 icon1_down, FuncPtr event_callback3,
                                  U16 icon2_up, U16 icon2_down, FuncPtr event_callback4,
                                  U16 icon3_up, U16 icon3_down, FuncPtr event_callback5,
                                  U16 icon4_up, U16 icon4_down, FuncPtr event_callback6,
                                  GDI_HANDLE *area1_layer, void (*area1_call_back) (dm_coordinates *coordinate),
                                  GDI_HANDLE *area2_layer, void (*area2_call_back) (dm_coordinates *coordinate),
                                  cat226_screen_layout_enum layout);

extern void ShowCategory227Screen(U8 *title_string,
                                  U16 title_icon,
                                  U16 left_softkey,
                                  U16 left_softkey_icon,
                                  U16 right_softkey,
                                  U16 right_softkey_icon, void (*display_call_back) (dm_coordinates *coordinate));

#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
extern void Cat226RegisterPenHandlers(U16 ctrl_area_idx, mmi_pen_handler pen_hdlr, mmi_pen_event_type_enum event_type);
#endif /* __MMI_TOUCH_SCREEN__ */ 

/* Empty Sublcd category */
extern void ShowCategory331Screen(void);

/* Draw a image at center Sublcd category */
extern void ShowCategory332Screen(UI_image_ID_type  image_id);

/* Play audio Sublcd category */
extern void ShowCategory333Screen(void);

#ifdef __MMI_VIDEO_PLAYER__
extern void ResumeCategory224Video(void);
extern void PauseCategory224Video(void);
#endif /* __MMI_VIDEO_PLAYER__ */ 


#ifdef __MMI_SWFLASH__
extern void SetSWFlashPlayerLayer(GDI_HANDLE layer_handle);

extern void CatSWFlashPlayEnd(void);

extern void ShowCategory229Screen(
            U8 *title_str,
            U16 title_icon,
            U16 lsk_str,
            U16 lsk_icon,
            U16 rsk_str,
            U16 rsk_icon,
            U16 resource_id,
            PS8 resource_filename,
            U16 repeat_count,
            MMI_BOOL is_play_audio,
            MMI_BOOL is_vibrate_on,
            MMI_BOOL is_interaction_on,
            MMI_BOOL is_lcd_no_sleep,
            MMI_BOOL is_full_screen,
            GDI_COLOR bg_color,
            void (*play_finish_callback) (S32 ret),
            void *gui_buffer);

extern void ExitCategorySWFlashScreen(void);

extern S32 GetCat34MediaType(void);

extern void SetCat34MediaType(S32 media_type);

extern void ShowCategory34Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 bg_media_id,
        S8 *bg_media_filename,
        U8 *history_buffer);

extern void HandleCat34MediaPlayFailed(void);

#endif
#endif /* __WGUI_CATEGORIES_MULTIMEDIA_H__ */ 

