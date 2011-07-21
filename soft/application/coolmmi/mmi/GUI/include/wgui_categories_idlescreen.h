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
 *  wgui_categories_idlescreen.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Idle Screen related categories.
 *  Author:
 * -------
 
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef __WGUI_CATEGORIES_IDLESCREEN_H__
#define __WGUI_CATEGORIES_IDLESCREEN_H__

extern void SetIdleScreenWallpaper(U16 image_ID);
extern void SetIdleScreenNetworkName(U8 *s, U8 nSimID);
extern void SetIdleScreenNetworkStatus(U8 *s, U8 nSimID);

//disable the feature for reduce memory cost
#define  __MMI_SAVE_WP_SPEEDUP__  /*gdm modify to add  the switch of __MMI_SAVE_WP_SPEEDUP__*/

extern void RedrawCategory33Screen(void);
extern void ShowCategory33Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *history_buffer);
extern void ExitCategory33Screen(void);

extern void RedrawCategory86Screen(void);

 
extern void ShowCategory86Screen(U16 title, U16 title_icon, U16 left_softkey, U16 left_softkey_icon, U16 right_softkey, U16 right_softkey_icon, U16 img_id, U8 *network_string, U8 *history_buffer, U8 show_network_name);     

 

extern void ExitCategory86Screen(void);

extern void ShowCategory106Screen(U16 icon_screensaver, U8 *filename, U8 *history_buffer);
extern void RedrawCategory106Screen(void);
extern void ExitCategory106Screen(void);

extern void ShowCategory128Screen(
                U16,
                U16,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U16 screen_img,
                U8 date_flag,
                U8 time_flag,
                U8 service_provider,
                U8 *history_buffer,
                U8);
extern void RedrawCategory128Screen(void);
extern void ExitCategory128Screen(void);

extern void ShowCategory131Screen(U16 icon_screensaver, U8 *history_buffer);
extern void RedrawCategory131Screen(void);
extern void ExitCategory131Screen(void);
extern void idle_screen_sat_display_text_background_redraw(S32 x1, S32 y1, S32 x2, S32 y2);

 
#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__)
#define BAR_SHUFFLE_TIMER 2000
extern void start_shuffle(void);
extern void close_shuffle(void);
#endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__) */ 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
extern void shuffle_draw_wallpaper(void);
#endif 
 
#ifdef __MMI_LCD_PARTIAL_ON__
extern void ShowCategory161Screen(UI_time *t);
extern void RedrawCategory161Screen(void);
extern void ExitCategory161Screen(void);
extern void Category161ChangeTime(UI_time *t);
#endif /* __MMI_LCD_PARTIAL_ON__ */ 

 
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
extern void wgui_set_wallpaper_change(void);
extern U8 wgui_is_wallpaper_change(void);
extern void wgui_reset_wallpaper_change(void);
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 

extern void cat86_hide_analog_clock_display(S32 x1, S32 y1, S32 x2, S32 y2);
extern void category128_hide_dt_display(S32 x1, S32 y1, S32 x2, S32 y2);
extern void draw_wallpaper(void);

 
#ifdef __MMI_TOUCH_IDLESCREEN_SHORTCUTS__
extern S32 max_touch_idle_screen_shortcut_height(void);
#endif 
 

#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
extern void show_boundries_to_status_bar_at_bottom_idlescreen(void);
#endif 
extern S32 is_on_idlescreen(void);
extern void set_on_idlescreen(S32);

/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 14:28 */
extern void SetIdleScreenWallpaperID(U16 image_ID/*, MMI_BOOL is_default*/);
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 14:28 */
extern void SetIdleScreenWallpaperName(UI_string_type name);
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 14:30 */
extern void wgui_save_wallpaper_to_speedup(void/*MMI_BOOL is_default*/);
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 14:30 */
extern void wgui_stop_wallpaper(void);
extern MMI_BOOL wgui_check_wallpaper_frame_count(S8 *name, S8 **error_str);
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
extern void SetListWallpaperID(U16 image_ID);
#endif

pBOOL mmi_bootup_get_active_flight_mode();
pBOOL mmi_bootup_is_sim_valid();
#endif /* __WGUI_CATEGORIES_IDLESCREEN_H__ */ 

