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
 *  wgui_categories_sublcd.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Sublcd display related categories.
 *  Author:
 * -------
 
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef WGUI_CATEGORIES_SUBLCD_H
#define WGUI_CATEGORIES_SUBLCD_H

#include "mmi_features.h"

#define CATEGORY311_TEXT_SCROLL_GAP    (32)
#define CATEGORY311_TEXT_SCROLL_SIZE   (3)
#define CATEGORY311_TEXT_SCROLL_TIME   (700)
#define CATEGORY311_TEXT_SCROLL_WAIT   (1000)
#define CATEGORY311_TEXT_SCROLL_X      (0)
#define CATEGORY311_TEXT_SCROLL_Y      (20)

#ifdef __MMI_SUB_WALLPAPER__
#define MAX_SUB_WALLPAPER_FRAME        (20)
#endif 

extern void (*SUBLCD_ExitCategoryFunction) (void);
extern void (*SUBLCD_RedrawCategoryFunction) (void);
extern U8 *(*SUBLCD_GetCategoryHistory) (U8 *buffer);
extern S32(*SUBLCD_GetCategoryHistorySize) (void);

extern PU8 SUBLCD_MMI_animated_icon;
extern S32 SUBLCD_MMI_animated_icon_x;
extern S32 SUBLCD_MMI_animated_icon_y;
extern UI_animated_image_handle SUBLCD_MMI_animated_icon_handle;

#ifdef __MMI_SUBLCD__
extern S32 SUB_LCD_device_width;
extern S32 SUB_LCD_device_height;
#endif /* __MMI_SUBLCD__ */ 

extern void show_status_icon_sublcd(S16 icon_ID);
extern void SUBLCD_screens_show_status_icons(void);
extern void SUBLCD_screens_hide_status_icons(void);
extern void ShowSubLCDStatusIconsTitle(void);

extern void ShowCategory301Screen(U8 *message, U16 message_icon, U8 *history_buffer);
extern void ExitCategory301Screen(void);

extern void RedrawCategory302Screen(void);
extern void ShowCategory302Screen(U8 *history_buffer);
extern void ExitCategory302Screen(void);

extern void ShowCategory303Screen(
                U8 *message1,
                U8 *message2,
                FLOAT time_diff1,
                FLOAT time_diff2,
                U8 *history_buffer);
extern void ExitCategory303Screen(void);

extern void ShowCategory304Screen(U16 message_icon, U8 *history_buffer);
extern void ExitCategory304Screen(void);

extern void ShowCategory311Screen(U8 *message, U16 message_icon, U8 *history_buffer);
extern void ExitCategory311Screen(void);

extern void ShowCategory312Screen(U8 *message, FLOAT time_diff_312, U8 *history_buffer);
extern void ExitCategory312Screen(void);

extern void ShowCategory313Screen(U16 message_icon, U8 *history_buffer);
extern void ExitCategory313Screen(void);

extern void ShowCategory314Screen(
                U8 *title,
                U16 title_icon,
                S32 number_of_items,
                U16 *list_of_items,
                U16 *list_of_icons,
                U8 **list_of_descriptions,
                S32 highlighted_item,
                U8 *history_buffer);
extern void UpdateCategory314KeyStateInd(MMI_BOOL state);
extern void ShowSubLCDLogoString(void);

#ifdef __MMI_SUB_WALLPAPER__
extern void SUBLCD_UpdateWallpaper(void);
extern void SUBLCD_StartWallpaperAnimation(void);
extern void SUBLCD_StopWallpaperAnimation(void);
extern void SUBLCD_SetIdleScreenWallpaperID(U16 image_ID);
extern void SUBLCD_SetIdleScreenWallpaperName(UI_string_type name);
extern BOOL SUBLCD_CheckWallpaperFrameCount(UI_string_type name);
#endif /* __MMI_SUB_WALLPAPER__ */ 

extern void gui_rotate_180_sublcd(void);

#endif /* WGUI_CATEGORIES_SUBLCD_H */ 

