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
 * wgui_status_icons.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   This file is intends for status icon bars control.
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 */

/**********************************************************************************
   Filename:      wgui_status_icons.h
   Date Created:  August-07-2002
   Contains:      UI wrapper routines
               Status Icon wrappers
**********************************************************************************/

#ifndef __WGUI_STATUS_ICONS_H__
#define __WGUI_STATUS_ICONS_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

#include "wgui_categories.h"
#ifdef __MMI_TOUCH_SCREEN__

#ifndef _TOUCH_SCREEN_GPROT_H
#error "please include TouchScreenGprot.h before wgui_status_icons.h"
#endif 

#endif /* __MMI_TOUCH_SCREEN__ */ 
#define MAX_STATUS_ICON_BARS           3        /* 1 to ?         */
#define STATUS_ICON_TIMER_RATE            500   /* Milliseconds   */

    /* Bear, move from wgui_status_icons.c - for wgui.c to include */
#define MMI_N_STATUS_ICONS_LIST1 12
#define MMI_N_STATUS_ICONS_LIST2 8
#define MMI_N_STATUS_ICONS_LIST3 6

#ifdef __MMI_LCD_PARTIAL_ON__
#define MMI_N_STATUS_ICONS_PARTIAL_DISPLAY      4
    extern S16 MMI_status_icons_list_partial_display[MMI_N_STATUS_ICONS_PARTIAL_DISPLAY];
    extern S16 MMI_status_icons_partial_display_n_icons;
#endif /* __MMI_LCD_PARTIAL_ON__ */ 
    enum STATUS_ICON_FORCE_DISPLAY
    {
        DEFAULT_BOTH,
        MAINLCD_ONLY,
        SUBLCD_ONLY
    };

    /* Icon names that the applications must use when they call                                  
       ShowStatusIcon or HideStatusIcon                   */
    enum STATUS_ICON_LIST
    {
        STATUS_ICON_SIGNAL_STRENGTH = 0,
        STATUS_ICON_CARD2_SIGNAL_STRENGTH,
        STATUS_ICON_CARD3_SIGNAL_STRENGTH,
        STATUS_ICON_CARD4_SIGNAL_STRENGTH,
        STATUS_ICON_BATTERY_STRENGTH,
    #ifndef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__      
        STATUS_ICON_ALTERNATE_LINE_L1,
        STATUS_ICON_ALTERNATE_LINE_L2,
    #endif /* __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__ */ 
    #ifdef __DUAL_UG_MODE__  
        STATUS_ICON_3G,
        STATUS_ICON_2G,
    #endif /* __DUAL_UG_MODE__ */ 
        STATUS_ICON_ROAMING,
#ifdef __MMI_MULTI_SIM__
	STATUS_ICON_SLAVE_ROAMING,
	STATUS_ICON_SIM3_ROAMING,
	STATUS_ICON_SIM4_ROAMING,
#endif
        STATUS_ICON_CALL_DIVERT_L1L2,
#ifdef __MMI_MULTI_SIM__
        STATUS_ICON_SIM2_CALL_DIVERT_L1L2,
        STATUS_ICON_SIM3_CALL_DIVERT_L1L2,
        STATUS_ICON_SIM4_CALL_DIVERT_L1L2,
#endif
        STATUS_ICON_CALL_DIVERT_L1,
        STATUS_ICON_CALL_DIVERT_L2,
        STATUS_ICON_MUTE,
        STATUS_ICON_INCOMING_SMS,
        #ifdef __MMI_MULTI_SIM__
	 STATUS_ICON_SLAVE_INCOMING_SMS,
	 STATUS_ICON_SIM3_INCOMING_SMS,
	 STATUS_ICON_SIM4_INCOMING_SMS,
	 #endif
        STATUS_ICON_MMS_UNREAD,
        STATUS_ICON_MMS_BUSY,
        STATUS_ICON_VIBRATION,
        STATUS_ICON_SILENT,
        STATUS_ICON_RING,
        STATUS_ICON_VIBRATE_AND_RING,
        STATUS_ICON_VIBRATE_THEN_RING,
        STATUS_ICON_GPRS_SERVICE,
#ifdef __MMI_MULTI_SIM__
        STATUS_ICON_GPRS_SERVICE_2,
        STATUS_ICON_GPRS_SERVICE_3,
        STATUS_ICON_GPRS_SERVICE_4,
#endif
        STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR,   
#ifdef __MMI_MULTI_SIM__
        STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR_2,
        STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR_3,
        STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR_4,
#endif
        STATUS_ICON_EARPHONE_INDICATOR,
        STATUS_ICON_ALARM,
        STATUS_ICON_VOICE_RECORD,
        STATUS_ICON_KEYPAD_LOCK,
        STATUS_ICON_MISSED_CALL,
#ifdef __MMI_MULTI_SIM__
        STATUS_ICON_CARD2MISSED_CALL,
        STATUS_ICON_CARD3MISSED_CALL,
        STATUS_ICON_CARD4MISSED_CALL,
#endif
        STATUS_ICON_UNREAD_VOICE_L1L2,
        STATUS_ICON_UNREAD_VOICE_L1,
        STATUS_ICON_UNREAD_VOICE_L2,
        STATUS_ICON_UNREAD_FAX_L1L2,
        STATUS_ICON_UNREAD_FAX_L1,
        STATUS_ICON_UNREAD_FAX_L2,
        STATUS_ICON_UNREAD_EMAIL_L1L2,
        STATUS_ICON_UNREAD_EMAIL_L1,
        STATUS_ICON_UNREAD_EMAIL_L2,
        STATUS_ICON_IMPS_MSG,
        STATUS_ICON_IMPS_LOGIN,
        STATUS_ICON_WAP,                        /*2010-4-14 used for wap_push_indicator*/
        STATUS_ICON_POC,                         
        STATUS_ICON_BT,
        STATUS_ICON_BT_CON,
        STATUS_ICON_BT_ND,
        STATUS_ICON_IR,
        STATUS_ICON_STOP_WATCH,
        STATUS_ICON_CHAT_INDICATION_ROOM1,
        STATUS_ICON_CHAT_INDICATION_ROOM2,
        STATUS_ICON_NETWORK_CIPHER_GSM,          
        STATUS_ICON_NETWORK_CIPHER_GPRS,         
        STATUS_ICON_JAVA_PAUSE,                  
         
    #ifdef __MMI_BG_SOUND_EFFECT__
        STATUS_ICON_BG_SOUND,
    #endif 
    #ifdef __MMI_AUDIO_3D_EFFECT__
        STATUS_ICON_3D_SOUND,
    #endif 
    #ifdef __MMI_HOMEZONE_STATUS_ICON__
        STATUS_ICON_HOMEZONE,
        STATUS_ICON_CITYZONE,
    #endif /* __MMI_HOMEZONE_STATUS_ICON__ */ 
    #ifdef __CTM_SUPPORT__
        STATUS_ICON_TTY_INDICATOR,
    #endif 
         
        STATUS_ICON_SUBLCD_SIGNAL_STRENGTH,
        STATUS_ICON_SUBLCD_BATTERY_STRENGTH,
        STATUS_ICON_SUBLCD_SMS,
        STATUS_ICON_SUBLCD_RING,
        STATUS_ICON_SUBLCD_VIBRATE,
        STATUS_ICON_SUBLCD_VIBRATE_AND_RING,
        STATUS_ICON_SUBLCD_VIBRATE_THEN_RING,
        STATUS_ICON_SUBLCD_SILENT,
        STATUS_ICON_SUBLCD_ALARM,
        STATUS_ICON_SUBLCD_MISSED_CALL,
      	/*Wangzl Add Start For Ver: TBM_780  on 2007-8-8 11:13 */
	STATUS_ICON_SIM_SWITCH,
	/*Wangzl Add End  For Ver: TBM_780  on 2007-8-8 11:13 */
 	//STATUS_ICON_SUBLCD_MISSED_CALL,
       STATUS_ICON_REJECTED_CALL,//LICHENG
       STATUS_ICON_CARD2REJECTED_CALL, 
       STATUS_ICON_CARD3REJECTED_CALL, 
       STATUS_ICON_CARD4REJECTED_CALL, 

       STATUS_ICON_USB_CONNETCED,//LICHENG

        MAX_STATUS_ICONS
    };

    typedef struct _MMI_status_icon
    {
        S32 x, y;               /* Offset within a status icon bar                    */
        S32 width, height;      /* For quick lookup                             */
        UI_image_ID_type  icon_ID;    /* UI_image_ID_type  of the icon                         */
        U32 flags;              /* Internal flags: See flag names below               */
        U8 state;               /* For icons with multiple states                     */
        U8 group_ID;            /* 0: No group, 1,2,3 ... Groups                   */
        U8 priority;            /* 0: No priority, 1,2,3...Highest to Lowest priorities  */
        PU8 icon_data;          /* For quick lookup                             */
        S16 n_frames;           /* For quick lookup                             */
        S16 current_frame;      /* For animated icons                              */
    } MMI_status_icon;

#define  STATUS_ICON_ANIMATE              0x00000001
#define STATUS_ICON_REVERSE_ANIMATE       0x00000002
#define STATUS_ICON_FORWARD_ANIMATE       0x00000004
#define STATUS_ICON_BLINK              0x00000008
#define STATUS_ICON_DISABLED           0x00000010
#define STATUS_ICON_DISPLAY               0x00000020

    typedef struct _MMI_status_icon_bar
    {
        S32 x, y;                   /* Icons are offset from this position                */
        U32 flags;                  /* all the icons between them.                        */
        S32 x1, y1, x2, y2;         /* Area occupied by the status bar: Used for hiding      */
        U8 group_ID;                /* 0: No group, 1,2,3 ... Groups                   */
        S16 n_icons;                /* Number of icons in the list of icons               */
        S16 *icon_list;             /* Pointer to the list of icons (Icon IDs used here)     */
        bitmap *graphics_context;   /* Pointer to the graphics context (Sub/Main LCD)        */
        U8 *layer_buffer;           /* Layer buffer on which the status icon bar is drawn    */ //061506 double buffer
        GDI_HANDLE layer_handle;    /* Layer handle on which the status icon bar is drawn    */ 
    } MMI_status_icon_bar;

#define STATUS_ICON_BAR_DISPLAY           0x00000001
#define STATUS_ICON_BAR_ARRANGE_VERTICAL  0x00000002
#define STATUS_ICON_BAR_ARRANGE_HORIZONTAL   0x00000004
#define STATUS_ICON_BAR_ARRANGE_ASCENDING 0x00000000
#define STATUS_ICON_BAR_ARRANGE_DESCENDING   0x00000008
#define STATUS_ICON_BAR_ARRANGE_PRIORITY  0x00000010
#define STATUS_ICON_BAR_DISABLE_BACKGROUND   0x00000020
#define STATUS_ICON_BAR_ENABLE_DOUBLE_BUFFER   0x00000040

    /* Distance in pixels between a status icon and its adjacent status icons  */
#define STATUS_ICON_X_GAP              2
#define STATUS_ICON_Y_GAP              2
#define SUBLCD_STATUS_ICON_X_GAP       1
    extern MMI_status_icon MMI_status_icons[];
    extern MMI_status_icon_bar MMI_status_icon_bars[];

    /* Globals  */

    extern void (*MMI_hide_status_icon_bars[]) (void);
    extern U8 status_icon_display_ON;
    extern U8 status_icon_blink_animate_ON;
    extern U8 MMI_status_icons_blink_animate_ON_state;

    void register_hide_status_icon_bar(U8 status_bar_ID, void (*f) (void));
    extern void clear_hide_status_icon_bars(void);
    extern void arrange_status_icons(void);
    extern void show_status_icon(S16 icon_ID);
    extern void hide_status_icon(S16 icon_ID);
    extern void animate_status_icon(S16 icon_ID);
    extern void forward_animate_status_icon(S16 icon_ID);
    extern void reverse_animate_status_icon(S16 icon_ID);
    extern void blink_status_icon(S16 icon_ID);
    extern void disable_status_icon(S16 icon_ID);
    extern void enable_status_icon(S16 icon_ID);
    extern void set_status_icon_state(S16 icon_ID, U8 state);
    extern void reset_status_icon_state(S16 icon_ID);
    extern void set_status_icon_display(S32 state);
    extern void reset_status_icon_display(void);
    extern void show_status_icons(void);
    extern void hide_status_icons(void);
    extern void status_icon_timer_callback(void);
    extern void setup_status_icon_blink_animate(void);
    extern void show_status_icon_bar(U8 status_bar_ID);
    extern void hide_status_icon_bar(U8 status_bar_ID);
    extern void get_status_icon_bar_clip(U8 status_bar_ID, S32 *x1, S32 *y1, S32 *x2, S32 *y2);
    extern void set_status_icon_bar_clip(U8 status_bar_ID, S32 x1, S32 y1, S32 x2, S32 y2);
    extern void set_status_icon_bar_type(U8 status_bar_ID, U32 flags);
    extern U32 get_status_icon_bar_type(U8 status_bar_ID);//061306 dummy category
    extern void set_status_icon_bar_layer(U8 status_bar_ID, GDI_HANDLE gdi_layer);//061506 double buffer
    extern U32 get_status_icon_bar_layer(U8 status_bar_ID);
    extern void reset_status_icons(void);
    extern void initialize_status_icons(void);
    extern void close_status_icons(void);
    extern void refresh_status_icon_display(void);
 
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
    extern pBOOL whether_status_bar_at_bottom(void);
    extern void set_status_bar_at_bottom(void);
    extern void reset_status_bar_at_bottom(void);
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
 

#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
    extern void refresh_fixed_status_icon(S16 icon_id, S16 bar_id);
    extern S32 NumberOfDisplayedIcons(void);
    extern void ChangeStatusIconImageWithoutReArrange(S16 icon_ID, U16 image_ID);
#endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 

     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
#define STATUS_BAR_SHUFFLE_TIMER 2000
    /* void gui_set_status_bar_theme(MMI_status_icon_bar *b, UI_status_bar_theme *t); */
    extern void shuffle_status_bars(void);
    extern void swap_status_bar_positions(S32 *x, S32 *y);
    extern void enable_statusbar_fill_background(void);
    extern void disable_statusbar_fill_background(void);

#ifdef __MMI_MAINLCD_176X220__
#define STATUS_TITLE_GAP 0
#endif 

#ifdef __MMI_MAINLCD_220X176__
#define STATUS_TITLE_GAP 0
#endif 

     
#if defined(__MMI_MAINLCD_128X128__) || defined (__MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_160X128__)
     
#define STATUS_TITLE_GAP 2
#endif /* defined(__MMI_MAINLCD_128X128__) || defined (__MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_240X320__) */ 
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
    /* Application specific APIs  */

    /* void PopulateStatusIconResources(void); */
    extern void ShowStatusIcon(S16 icon_ID);
    extern void HideStatusIcon(S16 icon_ID);
    extern void UpdateStatusIcons(void);
    extern void UpdateSubLCDStatusIcons(void);
    extern void ChangeStatusIconLevel(S16 icon_ID, U8 level_percentage);
    extern void ChangeStatusIconImage(S16 icon_ID, U16 image_ID);        
    extern void AnimateStatusIcon(S16 icon_ID);
    extern void BlinkStatusIcon(S16 icon_ID);

     
    //void LockStatusIconDisplay(void);
    //void UnlockStatusIconDisplay(void);
     

    extern void arrange_status_icons1(void);

    /* CSD JL add 040311 new sublcd color interface screen */
    extern void show_sublcd_status_background(S32 start_x, S32 start_y, S32 end_x, S32 end_y, U8 is_status_bar);
    extern void show_mainlcd_status_bar_background(U16 start_x, U16 start_y, U16 end_x, U16 end_y);
    extern void set_status_icons_display(MMI_BOOL state);

    extern void wgui_status_icon_set_display_layer(GDI_HANDLE display_layer);

    extern GDI_HANDLE wgui_status_icon_get_display_layer(void);

    extern void wgui_status_icon_reset_display_layer(void);

#ifdef __MMI_TOUCH_SCREEN__
    typedef enum
    {
        WGUI_STATUS_ICON_BAR_PEN_SELECT_ICON,
        WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,
        WGUI_STATUS_ICON_BAR_PEN_EVENT_MAX,
        WGUI_STATUS_ICON_BAR_PEN_NONE
    } wgui_status_icon_bar_pen_enum;
    extern void wgui_register_status_icon_pen_event_hdlr(
                    S16 icon_id,
                    wgui_status_icon_bar_pen_enum event_type,
                    FuncPtr f);
    extern MMI_BOOL wgui_execute_status_icon_pen_event_hdlr(S16 icon_id, wgui_status_icon_bar_pen_enum event_type);
    extern void wgui_reset_status_icon_pen_event_hdlr(void);
    extern BOOL wgui_status_icon_translate_pen_event(
                    mmi_pen_event_type_enum pen_event,
                    S16 x,
                    S16 y,
                    S16 *icon_id,
                    wgui_status_icon_bar_pen_enum *event_type);
#endif /* __MMI_TOUCH_SCREEN__ */ 
#ifdef __cplusplus
}
#endif 

#endif /* __WGUI_STATUS_ICONS_H__ */ 

