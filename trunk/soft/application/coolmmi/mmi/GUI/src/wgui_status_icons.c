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
 * wgui_status_icons.c
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
   Filename:      wgui_status_icons.c
   Date Created:  August-07-2002
   Contains:      UI wrapper routines
               Status Icon wrappers
**********************************************************************************/

#include "wgui_status_icons.h"
#include "gdi_include.h"
/* To be removed later..and put in boot up init file..Right now needed for 
   initializing status icon data */
#include "custdatares.h"
#include "statusiconres.h"
#include "mmi_features.h"
#include "simdetectiongexdcl.h"
#include "simdetectiongprot.h"
#include "idleappdef.h"
#ifdef __MMI_MULTI_SIM__
#include "mtpnp_ad_resdef.h"
#endif
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#include "touchscreengprot.h"
#endif /* __MMI_TOUCH_SCREEN__ */ 

#if defined(__MMI_UI_TECHNO_STATUS_ICON__)||defined(__MMI_MAINLCD_220X176__)
#include "mainmenudef.h"
#endif 

#if defined(__MMI_UI_STATUS_BAR_AT_BOTTOM__)
#include "wgui_categories_idlescreen.h"
#endif 

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
#include "thememanager.h"
#endif 

#include "callmanagementiddef.h"

#ifdef __MMI_MAINLCD_220X176__
#include "wgui_draw_manager.h"
#endif 
/* Externals for graphics context switching  */
extern bitmap main_LCD_device_bitmap;
extern bitmap sub_LCD_device_bitmap;
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__) || defined (__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
extern MMI_BOOL if_full_screen_hide_status_icons(void);
#endif 

extern MMI_BOOL PhnsetFlightModeHideButSaveStatusIcon(S16 icon_ID, U32 icon_flag);

 #ifdef __MMI_SCREEN_ROTATE__ // wangmg[3.1]
extern MMI_BOOL mmi_frm_is_screen_width_height_swapped(void);
#endif

#ifdef __FLIGHT_MODE_SUPPORT__
extern pBOOL gPowerOnWithFlightMode;
#endif 

#ifdef __MMI_MAINLCD_220X176__   //2010-9-19 fzb add
extern UI_image_ID_type  idlescreen_bar_id;
extern S32 is_on_idlescreen(void);
#endif

void UI_set_main_LCD_graphics_context(void);
void UI_set_sub_LCD_graphics_context(void);
extern bitmap *current_LCD_device_bitmap;
extern BOOL IsClamClose(void);
#ifdef __MMI_MULTI_SIM__
U8 IsReArrangeNeeded = 0;
#else
static U8 IsReArrangeNeeded = 0;
#endif

 
extern S32 MAIN_LCD_device_width;
extern S32 MAIN_LCD_device_height;

/* Setting this value to 1 will cause icons to be drawn from bottom baseline  */
/* Setting to 0 will cause icons to be drawn from center baseline          */
/* CSD added by JL for Joyce's request */
#define STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE     1

/* This is the customizable list of status icons                        */
/* Use (x,y)=(0,0) for automatic positioning. Automatic positioning is        */
/* only valid with Icon bars having STATUS_ICON_BAR_ARRANGE_XXXXXXXX       */
/* Set width=0, height=0 for all icons. These values are autocalculated    */
/* If there is no icon, use IMAGE_ID_NULL for the UI_image_ID_type  value           */
/* FLAGS:   Set status icon specific flags here. Usually this value is 0      */
/* STATE:   Set the default state of a multi-state status icon here           */
/* GROUP:   Set the group number for grouped icons here.                */
/*       0 = Icon is not grouped                                  */
/* PRIORITY:   Set the status icon priority here. If there are more status    */
/*          icons than what can be displayed in the status icon bar        */
/*          area, priority is used to determine which icons are            */
/*          displayed and which are not.                          */
/*          0 = Icon is always displayed                          */
/* IMAGE:   Pointer to Icons image data. Autocalculated. Always set to NULL      */
/* NFRAMES: Number of frames in the image. Autocalculated. Always set to 0    */

/* !!NOTE:  Currently the priority field is dummy, the icons need to be       */
/* manually arranged according to the desired priority                     */
/* Also, grouped icons need to be arranged consecutively in the list       */

MMI_status_icon MMI_status_icons[MAX_STATUS_ICONS] = 
{
	/* ICON_NUMBER    X     Y     WIDTH    HEIGHT      UI_image_ID_type                       FLAGS STATE GROUP PRIORITY IMAGE NFRAMES  */
	/* 0  */ {1, 2, 0, 0, IMG_SI_SIGNAL_STRENGTH, 0, 0, 0, 0, NULL, 0, },
	/* ???  */ {20, 2, 0, 0, IMG_SI_SIGNAL_STRENGTH, 0, 0, 0, 0, NULL, 0, },
	/* ???  */ {40, 2, 0, 0, IMG_SI_SIGNAL_STRENGTH, 0, 0, 0, 0, NULL, 0, },
	/* ???  */ {60, 2, 0, 0, IMG_SI_SIGNAL_STRENGTH, 0, 0, 0, 0, NULL, 0, },
#ifdef __MMI_MAINLCD_176X220__
	/* 1  */ {111, 1, 0, 0, IMG_SI_BATTERY_STRENGTH, 0, 0, 0, 0, NULL, 0, },
#elif defined(__MMI_MAINLCD_128X128__)
	/* 1  */ {111, 0, 0, 0, IMG_SI_BATTERY_STRENGTH, 0, 0, 0, 0, NULL, 0, },
#else
	/* 1  */ {111, 2, 0, 0, IMG_SI_BATTERY_STRENGTH, 0, 0, 0, 0, NULL, 0, },
#endif
#ifndef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__      
	/* 2  */ {0, 0, 0, 0, IMG_SI_LINE_L1, 0, 0, 1, 0, NULL, 0, },
	/* 3  */ {0, 0, 0, 0, IMG_SI_LINE_L2, 0, 0, 1, 0, NULL, 0, },
#endif /* __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__ */
#ifdef __DUAL_UG_MODE__  
	/* 4  */ {0, 0, 0, 0, IMG_SI_3G, 0, 0, 0, 0, NULL, 0, },
	/* 5  */ {0, 0, 0, 0, IMG_SI_2G, 0, 0, 0, 0, NULL, 0, },
#endif /* __DUAL_UG_MODE__ */
	/* 6  */ {0, 0, 0, 0, IMG_SI_ROAMING_INDICATOR, 0, 0, 0, 0, NULL, 0, },
#ifdef __MMI_MULTI_SIM__
	/**/	{0, 0, 0, 0, IMG_SI_SLAVE_ROAMING_INDICATOR, 0, 0, 0, 0, NULL, 0,},
			{0, 0, 0, 0, IMG_SI_SIM3_ROAMING_INDICATOR, 0, 0, 0, 0, NULL, 0,},
			{0, 0, 0, 0, IMG_SI_SIM4_ROAMING_INDICATOR, 0, 0, 0, 0, NULL, 0,},
#endif
	/* 7  */ {0, 0, 0, 0, IMG_SI_CALL_DIVERT_L1L2, 0, 0, 2, 0, NULL, 0, },
	#ifdef __MMI_MULTI_SIM__
			{0, 0, 0, 0, IMG_SLAVE_CALL_FORWARD, 0, 0, 0, 0, NULL, 0, },
			{0, 0, 0, 0, IMG_SLAVE_CALL_FORWARD, 0, 0, 0, 0, NULL, 0, },
			{0, 0, 0, 0, IMG_SLAVE_CALL_FORWARD, 0, 0, 0, 0, NULL, 0, },
	#endif
	/* 8  */ {0, 0, 0, 0, IMG_SI_CALL_DIVERT_L1, 0, 0, 2, 0, NULL, 0, },
	/* 9  */ {0, 0, 0, 0, IMG_SI_CALL_DIVERT_L2, 0, 0, 2, 0, NULL, 0, },
	/* 10 */ {0, 0, 0, 0, IMG_SI_MUTE, 0, 0, 0, 0, NULL, 0, },
#ifndef __MMI_MULTI_SIM__
	/* 11 */ {0, 0, 0, 0, IMG_SI_SMS_INDICATOR, 0, 0, 0, 0, NULL, 0, },
#else
	/* 11 */ {0, 0, 0, 0, IMG_SI_MASTER_SMS_INDICATOR, 0, 0, 0, 0, NULL, 0, },
	/* 11 */ {0, 0, 0, 0, IMG_SI_SLAVE_SMS_INDICATOR, 0, 0, 0, 0, NULL, 0, },
			{0, 0, 0, 0, IMG_SI_SIM3_SMS_INDICATOR, 0, 0, 0, 0, NULL, 0, },
			{0, 0, 0, 0, IMG_SI_SIM4_SMS_INDICATOR, 0, 0, 0, 0, NULL, 0, },
#endif
	/* 12 */ {0, 0, 0, 0, IMG_SI_MMS_UNREAD_INDICATOR, 0, 0, 0, 0, NULL, 0, },
	/* 13 */ {0, 0, 0, 0, IMG_SI_MMS_BUSY_INDICATOR, 0, 0, 0, 0, NULL, 0, },
	/* 14 */ {0, 0, 0, 0, IMG_SI_VIBRATE, 0, 0, 3, 0, NULL, 0, },
	/* 15 */ {0, 0, 0, 0, IMG_SI_SILENT, 0, 0, 3, 0, NULL, 0, },
	/* 16 */ {0, 0, 0, 0, IMG_SI_RING, 0, 0, 3, 0, NULL, 0, },
	/* 17 */ {0, 0, 0, 0, IMG_SI_VIBRATE_AND_RING, 0, 0, 3, 0, NULL, 0, },
	/* 18 */ {0, 0, 0, 0, IMG_SI_VIBRATE_THEN_RING, 0, 0, 3, 0, NULL, 0, },
#ifndef __MMI_MULTI_SIM__
	/* 19 */{0, 0, 0, 0, IMG_SI_GPRS_INDICATOR, 0, 0, 0, 0, NULL, 0, },
	/* 20 */{0, 0, 0, 0, IMG_SI_GPRS_ATT_NO_PDP_INDICATOR, 0, 0, 0, 0, NULL, 0, },
#else
	/* 19 */ {0, 0, 0, 0, IMG_SI_GPRS_SERVICE_SIM1, 0, 0, 0, 0, NULL, 0, },
	/* 19 */ {0, 0, 0, 0, IMG_SI_GPRS_SERVICE_SIM2, 0, 0, 0, 0, NULL, 0, },
	/* 19 */ {0, 0, 0, 0, IMG_SI_GPRS_SERVICE_SIM3, 0, 0, 0, 0, NULL, 0, },
	/* 19 */ {0, 0, 0, 0, IMG_SI_GPRS_SERVICE_SIM4, 0, 0, 0, 0, NULL, 0, },
	/* 20 */ {0, 0, 0, 0, IMG_SI_GPRS_ATT_NO_PDP_INDICATOR_SIM1, 0, 0, 0, 0, NULL, 0, },
	/* 20 */ {0, 0, 0, 0, IMG_SI_GPRS_ATT_NO_PDP_INDICATOR_SIM2, 0, 0, 0, 0, NULL, 0, },//G2
	/* 20 */ {0, 0, 0, 0, IMG_SI_GPRS_ATT_NO_PDP_INDICATOR_SIM3, 0, 0, 0, 0, NULL, 0, },
	/* 20 */ {0, 0, 0, 0, IMG_SI_GPRS_ATT_NO_PDP_INDICATOR_SIM4, 0, 0, 0, 0, NULL, 0, },
#endif
	 
	/* 21 */ {0, 0, 0, 0, IMG_SI_EARPHONE_INDICATOR, 0, 0, 0, 0, NULL, 0, },
	/* 22 */ {0, 0, 0, 0, IMG_SI_ALARM_ACTIVATED, 0, 0, 0, 0, NULL, 0, },
	/* 23 */ {0, 0, 0, 0, IMG_SI_VOICE_RECORD, 0, 0, 0, 0, NULL, 0, },
	/* 24 */ {0, 0, 0, 0, IMG_SI_KEYPAD_LOCK, 0, 0, 0, 0, NULL, 0, },
#ifndef __MMI_MULTI_SIM__
    /* 28 */ {0, 0, 0, 0, IMG_SI_MISSED_CALL_INDICATOR, 0, 0, 0, 0, NULL, 0,},
#else
    /*  */    {0, 0, 0, 0, IMG_SI_CARD1MISSED_CALL_INDICATOR, 0, 0, 0, 0, NULL, 0,},
    /*  */    {0, 0, 0, 0, IMG_SI_CARD2MISSED_CALL_INDICATOR, 0, 0, 0, 0, NULL, 0,},
    /*  */    {0, 0, 0, 0, IMG_SI_CARD3MISSED_CALL_INDICATOR, 0, 0, 0, 0, NULL, 0,},
    /*  */    {0, 0, 0, 0, IMG_SI_CARD4MISSED_CALL_INDICATOR, 0, 0, 0, 0, NULL, 0,},
#endif/*__MMI_MULTI_SIM__*/
	/* 26 */ {0, 0, 0, 0, IMG_SI_UNREAD_VOICE_L1L2, 0, 0, 4, 0, NULL, 0, },
	/* 27 */ {0, 0, 0, 0, IMG_SI_UNREAD_VOICE_L1, 0, 0, 4, 0, NULL, 0, },
	/* 28 */ {0, 0, 0, 0, IMG_SI_UNREAD_VOICE_L2, 0, 0, 4, 0, NULL, 0, },
	/* 29 */ {0, 0, 0, 0, IMG_SI_UNREAD_FAX_L1L2, 0, 0, 5, 0, NULL, 0, },
	/* 30 */ {0, 0, 0, 0, IMG_SI_UNREAD_FAX_L1, 0, 0, 5, 0, NULL, 0, },
	/* 31 */ {0, 0, 0, 0, IMG_SI_UNREAD_FAX_L2, 0, 0, 5, 0, NULL, 0, },
	/* 32 */ {0, 0, 0, 0, IMG_SI_UNREAD_EMAIL_L1L2, 0, 0, 6, 0, NULL, 0, },
	/* 33 */ {0, 0, 0, 0, IMG_SI_UNREAD_EMAIL_L1, 0, 0, 6, 0, NULL, 0, },
	/* 34 */ {0, 0, 0, 0, IMG_SI_UNREAD_EMAIL_L2, 0, 0, 6, 0, NULL, 0, },
	/* 35 */ {0, 0, 0, 0, IMG_SI_IMPS_MSG, 0, 0, 8, 0, NULL, 0, },
	/* 36 */ {0, 0, 0, 0, IMG_SI_IMPS_LOGIN, 0, 0, 8, 0, NULL, 0, },
	/* 37 */ {0, 0, 0, 0, IMG_SI_WAP, 0, 0, 0, 0, NULL, 0, },
	/* 38 */ {0, 0, 0, 0, IMG_SI_POC, 0, 0, 0, 0, NULL, 0, },
	 
	/* 39 */ {0, 0, 0, 0, IMG_SI_BT, 0, 0, 0, 0, NULL, 0, },
	/* 40 */ {0, 0, 0, 0, IMG_SI_BT_CON, 0, 0, 0, 0, NULL, 0, },
	/* 41 */ {0, 0, 0, 0, IMG_SI_BT_ND, 0, 0, 0, 0, NULL, 0, },
	/* 42 */ {0, 0, 0, 0, IMG_SI_IR_INDICATOR, 0, 0, 0, 0, NULL, 0, },
	/* 43 */ {0, 0, 0, 0, IMG_SI_STOPWATCH, 0, 0, 0, 0, NULL, 0, },
	/* 44 */ {0, 0, 0, 0, IMG_BLINKINGICON_CHAT_ROOM1, 0, 0, 0, 0, NULL, 0, },
 
	/* 45 */ {0, 0, 0, 0, IMG_BLINKINGICON_CHAT_ROOM2, 0, 0, 0, 0, NULL, 0, },
	/*  */
	/* 46 */ {0, 0, 0, 0, IMG_SI_NETWORK_CIPHER_GSM, 0, 0, 0, 0, NULL, 0, },
	 
	/* 47 */ {0, 0, 0, 0, IMG_SI_NETWORK_CIPHER_GPRS, 0, 0, 0, 0, NULL, 0, },
	 
	/* 48 */ {0, 0, 0, 0, IMG_SI_JAVA_PAUSE, 0, 0, 0, 0, NULL, 0, },
	 
	 
#ifdef __MMI_BG_SOUND_EFFECT__
    /* 49 */ {0, 0, 0, 0, IMG_SI_BGSND, 0, 0, 0, 0, NULL, 0,},
#endif 
#ifdef __MMI_AUDIO_3D_EFFECT__
    /* 50 */ {0, 0, 0, 0, IMG_SI_3DSND, 0, 0, 0, 0, NULL, 0,},
#endif 
#ifdef __MMI_HOMEZONE_STATUS_ICON__
    /* 51 */ {0, 0, 0, 0, IMG_SI_HZONE, 0, 0, 0, 0, NULL, 0,},
    /* 52 */ {0, 0, 0, 0, IMG_SI_CZONE, 0, 0, 0, 0, NULL, 0,},
#endif /* __MMI_HOMEZONE_STATUS_ICON__ */ 
#ifdef __CTM_SUPPORT__
    /* 53 */ {0, 0, 0, 0, IMG_SI_TTY_INDICATOR, 0, 0, 0, 0, NULL, 0,},
#endif 
     

    /* Sublcd Start */
    /* 54 */ {0, 0, 0, 0, IMG_SLSI_SIGNAL_STRENGTH, 0, 0, 0, 0, NULL, 0,},
    /* 55 */ {36, 0, 0, 0, IMG_SLSI_BATTERY_STRENGTH, 0, 0, 0, 0, NULL, 0,},
    /* 56 */ {11, 2, 0, 0, IMG_SLSI_SMS_INDICATOR, 0, 0, 0, 0, NULL, 0,},
    /* 57 */ {24, 1, 0, 0, IMG_SLSI_RING, 0, 0, 7, 0, NULL, 0,},
    /* 58 */ {24, 1, 0, 0, IMG_SLSI_VIBRATE, 0, 0, 7, 0, NULL, 0,},
    /* 59 */ {24, 1, 0, 0, IMG_SLSI_VIBRATE_AND_RING, 0, 0, 7, 0, NULL, 0,},
    /* 60 */ {24, 1, 0, 0, IMG_SLSI_VIBRATE_THEN_RING, 0, 0, 7, 0, NULL, 0,},
    /* 61 */ {24, 1, 0, 0, IMG_SLSI_SILENT, 0, 0, 7, 0, NULL, 0,},
    /* 62 */ {24, 1, 0, 0, IMG_SLSI_ALARM, 0, 0, 0, 2, NULL, 0,},
    /* 63 */ {24, 1, 0, 0, IMG_SLSI_MISSED_CALL, 0, 0, 0, 2, NULL, 0,},
    /*Wangzl Add Start For Ver: TBM_780  on 2007-8-8 11:12 */
    /*64*/{0,		0,		0,			0,			IMG_SI_SIM_SWITCH,						0,		0,		0,		0,			NULL,	0,	},
	/*Wangzl Add End  For Ver: TBM_780  on 2007-8-8 11:12 */
#ifdef __MMI_MULTI_SIM__
   /*	65*/{0,		0,		0,			0,			IMG_SI_CARD1REJECTED_CALL_INDICATOR,						0,		0,		0,		0,			NULL,	0,	},
   /*	65*/{0,		0,		0,			0,			IMG_SI_CARD2REJECTED_CALL_INDICATOR,						0,		0,		0,		0,			NULL,	0,	},
   /*	65*/{0,		0,		0,			0,			IMG_SI_CARD3REJECTED_CALL_INDICATOR,						0,		0,		0,		0,			NULL,	0,	},
   /*	65*/{0,		0,		0,			0,			IMG_SI_CARD4REJECTED_CALL_INDICATOR,						0,		0,		0,		0,			NULL,	0,	},
#else  /* No Defined __MMI_MULTI_SIM__ */
   /*	65*/{0,		0,		0,			0,			IMG_SLSI_REJECTED_CALL,						0,		0,		0,		0,			NULL,	0,	},
   /*	65*/{0,		0,		0,			0,			IMG_SLSI_REJECTED_CALL,						0,		0,		0,		0,			NULL,	0,	},
   /*	65*/{0,		0,		0,			0,			IMG_SLSI_REJECTED_CALL,						0,		0,		0,		0,			NULL,	0,	},
   /*	65*/{0,		0,		0,			0,			IMG_SLSI_REJECTED_CALL,						0,		0,		0,		0,			NULL,	0,	},
#endif /* __MMI_MULTI_SIM__ */
   /*licheng Add End  For Ver: Jasmine  on 2008-03-13 11:12 */
/*	66	*/	{		0,		0,		0,			0,			IMG_SLSI_USB_CONNECTED,						0,		0,		0,		0,			NULL,	0,	},
			/*licheng Add End  For Ver: Jasmine  on 2008-03-13 11:12 */
};

/* Pool of status icons used for the horizontal and vertical   */
/* status icon bars                                */

/* CSD added by JL 040228 optimize code size */
#define MMI_N_STATUS_ICONS_POOL1 ( sizeof(MMI_status_icons_pool1)/sizeof(MMI_status_icons_pool1[0]) )   /* 32 */
/* store id of main lcd icon */
const S16 MMI_status_icons_pool1[] = 
{
	STATUS_ICON_SIGNAL_STRENGTH,
	STATUS_ICON_BATTERY_STRENGTH,
	STATUS_ICON_CARD2_SIGNAL_STRENGTH,
	STATUS_ICON_CARD3_SIGNAL_STRENGTH,
	STATUS_ICON_CARD4_SIGNAL_STRENGTH,
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
	/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-5-9 17:0 */
	STATUS_ICON_SIM_SWITCH,
	/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-5-9 17:0 */
	STATUS_ICON_MISSED_CALL,
#ifdef __MMI_MULTI_SIM__
    STATUS_ICON_CARD2MISSED_CALL,
    STATUS_ICON_CARD3MISSED_CALL,
    STATUS_ICON_CARD4MISSED_CALL,
#endif
	STATUS_ICON_KEYPAD_LOCK,
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
	STATUS_ICON_WAP,
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
	STATUS_ICON_JAVA_PAUSE
	 
#ifdef __MMI_BG_SOUND_EFFECT__
        ,
    STATUS_ICON_BG_SOUND
#endif /* __MMI_BG_SOUND_EFFECT__ */ 
#ifdef __MMI_AUDIO_3D_EFFECT__
        ,
    STATUS_ICON_3D_SOUND
#endif /* __MMI_AUDIO_3D_EFFECT__ */ 
#ifdef __MMI_HOMEZONE_STATUS_ICON__
        ,
    STATUS_ICON_HOMEZONE,
    STATUS_ICON_CITYZONE
#endif /* __MMI_HOMEZONE_STATUS_ICON__ */ 
#ifdef __CTM_SUPPORT__
        ,
    STATUS_ICON_TTY_INDICATOR
#endif /* __CTM_SUPPORT__ */ 
         
,
	STATUS_ICON_REJECTED_CALL,
#ifdef __MMI_MULTI_SIM__
	STATUS_ICON_CARD2REJECTED_CALL,
	STATUS_ICON_CARD3REJECTED_CALL,
	STATUS_ICON_CARD4REJECTED_CALL,
#endif
	STATUS_ICON_USB_CONNETCED
};

/* CSD added by JL 040228 optimize code size */
#define MMI_N_STATUS_ICONS_POOL2 ( sizeof(MMI_status_icons_pool2)/sizeof(MMI_status_icons_pool2[0]) )   /* 8 */

const S16 MMI_status_icons_pool2[] = 
{
    STATUS_ICON_SUBLCD_SIGNAL_STRENGTH,
    STATUS_ICON_SUBLCD_BATTERY_STRENGTH,
    STATUS_ICON_SUBLCD_SMS,
    STATUS_ICON_SUBLCD_RING,
    STATUS_ICON_SUBLCD_VIBRATE,
    STATUS_ICON_SUBLCD_VIBRATE_AND_RING,
    STATUS_ICON_SUBLCD_VIBRATE_THEN_RING,
    STATUS_ICON_SUBLCD_SILENT,
    STATUS_ICON_SUBLCD_ALARM,
    STATUS_ICON_SUBLCD_MISSED_CALL
};

#ifdef __MMI_LCD_PARTIAL_ON__
#define MMI_N_STATUS_ICONS_POOL_PARTIAL_DISPLAY ( sizeof(MMI_status_icons_pool_partial_display)/sizeof(MMI_status_icons_pool_partial_display[0]) )

/* For incoming events */
const S16 MMI_status_icons_pool_partial_display[] = 
{
	STATUS_ICON_INCOMING_SMS,
	STATUS_ICON_MMS_UNREAD,
	STATUS_ICON_MISSED_CALL,
	STATUS_ICON_UNREAD_VOICE_L1L2,
	STATUS_ICON_UNREAD_VOICE_L1,
	STATUS_ICON_UNREAD_VOICE_L2,
	STATUS_ICON_UNREAD_EMAIL_L1L2,
	STATUS_ICON_UNREAD_EMAIL_L1,
	STATUS_ICON_UNREAD_EMAIL_L2,
	STATUS_ICON_IMPS_MSG,
	STATUS_ICON_WAP,
	STATUS_ICON_REJECTED_CALL,
	STATUS_ICON_USB_CONNETCED,
	#ifdef __MMI_MULTI_SIM__
	STATUS_ICON_SLAVE_INCOMING_SMS,
	STATUS_ICON_SIM3_INCOMING_SMS,
	STATUS_ICON_SIM4_INCOMING_SMS,
	STATUS_ICON_CARD2MISSED_CALL,
	STATUS_ICON_CARD3MISSED_CALL,
	STATUS_ICON_CARD4MISSED_CALL,
	STATUS_ICON_CARD2REJECTED_CALL,
	STATUS_ICON_CARD3REJECTED_CALL,
	STATUS_ICON_CARD4REJECTED_CALL,
	#endif
};
#endif /* __MMI_LCD_PARTIAL_ON__ */ 

S16 MMI_status_icons_list1[MMI_N_STATUS_ICONS_LIST1];

/* store icons id of vertical  status bar of main lcd */
S16 MMI_status_icons_list2[MMI_N_STATUS_ICONS_LIST2];

/* store icons id of horizontal status bar of sub lcd  */
S16 MMI_status_icons_list3[MMI_N_STATUS_ICONS_LIST3];

#ifdef __MMI_LCD_PARTIAL_ON__
S16 MMI_status_icons_list_partial_display[MMI_N_STATUS_ICONS_PARTIAL_DISPLAY];
S16 MMI_status_icons_partial_display_n_icons;
#endif /* __MMI_LCD_PARTIAL_ON__ */ 

/* This is the customizable list of status icon bars        */

MMI_status_icon_bar MMI_status_icon_bars[MAX_STATUS_ICON_BARS];
static S32 status_icon_display_area = DEFAULT_BOTH;

/* Globals  */
/* Hide functions need to take x1,y1,x2,y2 as parameters */
void (*MMI_hide_status_icon_bars[MAX_STATUS_ICON_BARS]) (void);
U8 status_icon_display_ON = 0;
U8 status_icon_blink_animate_ON = 0;
U8 MMI_status_icons_blink_animate_ON_state = 0;

 
 
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
/* UI_status_bar_theme *current_status_bar_theme = NULL; */
U8 is_status_bar_shuffled = 0;
static S32 background_fill = 1;
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
 
#define SUBLCD_ALPHA_VALUE 30

#ifdef __MMI_TOUCH_SCREEN__
FuncPtr MMI_status_icon_pen_event_hdlr[MAX_STATUS_ICONS][WGUI_STATUS_ICON_BAR_PEN_EVENT_MAX];
S16 pen_down_status_icon_id = -1;
#endif /* __MMI_TOUCH_SCREEN__ */ 

static pBOOL mmi_bootup_is_in_security_check()
{
	return g_pwr_context[0].PowerOnTillIdleApp;
}

#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
pBOOL g_status_bar_at_bottom = FALSE;


/*****************************************************************************
 * FUNCTION
 *  whether_status_bar_at_bottom
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
pBOOL whether_status_bar_at_bottom(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_status_bar_at_bottom;
}


/*****************************************************************************
 * FUNCTION
 *  set_status_bar_at_bottom
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_status_bar_at_bottom(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_status_bar_at_bottom = TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  reset_status_bar_at_bottom
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_status_bar_at_bottom(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_status_bar_at_bottom = FALSE;
}
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 



/*****************************************************************************
 * FUNCTION
 *  register_hide_status_icon_bar
 * DESCRIPTION
 *  register hide function of status bar
 * PARAMETERS
 *  status_bar_ID       [IN]        Bar id
 *  f                   [IN]        )(void)      :- function pointer
 *  horizontal(?)       [IN]        Bar :- sublcd
 *  vertical(?)         [IN]        Bar main lcd
 * RETURNS
 *  void
 *****************************************************************************/
void register_hide_status_icon_bar(U8 status_bar_ID, void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* register the hide function of status bar */
    MMI_hide_status_icon_bars[status_bar_ID] = f;
}


/*****************************************************************************
 * FUNCTION
 *  clear_hide_status_icon_bars
 * DESCRIPTION
 *  clear hide function of status bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_hide_status_icon_bars(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* set the hide function  to dummy function */
    for (i = 0; i < MAX_STATUS_ICON_BARS; i++)
    {
        if (MMI_status_icon_bars[i].graphics_context == current_LCD_device_bitmap)
        {
            MMI_hide_status_icon_bars[i] = UI_dummy_function;
        }
    }
}

#define TEST_STATUS_ICON_DISPLAY_ON(flags)   (flags & (STATUS_ICON_ANIMATE|STATUS_ICON_REVERSE_ANIMATE|STATUS_ICON_FORWARD_ANIMATE|STATUS_ICON_BLINK|STATUS_ICON_DISABLED|STATUS_ICON_DISPLAY))

 
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
extern void set_vertical_signal_status_icons(void);
extern void set_horizontal_signal_status_icons(void);
#endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
 


/*****************************************************************************
 * FUNCTION
 *  arrange_status_icons
 * DESCRIPTION
 *  calulate the position of status icon of both main lcd (horizontal/vertical)
 *  and sub lcd
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void arrange_status_icons(void)
{
#if(0)  /* Original implementation: Temporarily disabled for worse performance */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	S32 i,j,x,y,width,height;
	UI_image_type img;
	byte d=0;
	byte group_flag;

	for(j=0;j<MAX_STATUS_ICON_BARS;j++)
	{	if(MMI_status_icon_bars[j].group_ID>0) group_flag=1;
		else group_flag=0;
		if(MMI_status_icon_bars[j].flags & STATUS_ICON_BAR_ARRANGE_DESCENDING) d=1;
		x=y=0;
		if(MMI_status_icon_bars[j].flags & STATUS_ICON_BAR_ARRANGE_HORIZONTAL)
		{	if(d)   /*	Descending direction	*/
			{	for(i=0;i<MMI_status_icon_bars[j].n_icons;i++)
				{	if(MMI_status_icons[i].flags & (STATUS_ICON_DISPLAY|STATUS_ICON_BLINK|STATUS_ICON_ANIMATE))
					{	img=MMI_status_icons[i].icon_data;
						coolsand_UI_measure_image(img,&width,&height);
						x-=(width+STATUS_ICON_X_GAP);
						MMI_status_icons[i].x=x;
						MMI_status_icons[i].y=y;
					}
				}
			}
			else /*	Ascending direction		*/
			{	for(i=0;i<MMI_status_icon_bars[j].n_icons;i++)
				{	if(MMI_status_icons[i].flags & (STATUS_ICON_DISPLAY|STATUS_ICON_BLINK|STATUS_ICON_ANIMATE))
					{	img=MMI_status_icons[i].icon_data;
						coolsand_UI_measure_image(img,&width,&height);
						MMI_status_icons[i].x=x;
						MMI_status_icons[i].y=y;
						x+=(width+STATUS_ICON_X_GAP);
					}
				}
			}
		}
		else if(MMI_status_icon_bars[j].flags & STATUS_ICON_BAR_ARRANGE_VERTICAL)
		{	if(d)   /*	Descending direction	*/
			{	for(i=0;i<MMI_status_icon_bars[j].n_icons;i++)
				{	if(MMI_status_icons[i].flags & (STATUS_ICON_DISPLAY|STATUS_ICON_BLINK|STATUS_ICON_ANIMATE))
					{	img=MMI_status_icons[i].icon_data;
						coolsand_UI_measure_image(img,&width,&height);
						y-=(height+STATUS_ICON_Y_GAP);
						MMI_status_icons[i].x=x;
						MMI_status_icons[i].y=y;						
					}
				}
			}
			else /*	Ascending direction		*/
			{	for(i=0;i<MMI_status_icon_bars[j].n_icons;i++)
				{	if(MMI_status_icons[i].flags & (STATUS_ICON_DISPLAY|STATUS_ICON_BLINK|STATUS_ICON_ANIMATE))
					{	img=MMI_status_icons[i].icon_data;
						coolsand_UI_measure_image(img,&width,&height);
						MMI_status_icons[i].x=x;
						MMI_status_icons[i].y=y;						
						y+=(height+STATUS_ICON_Y_GAP);
					}
				}
			}
		}
	}
#else /* (0) */ /* Hardcoded implementation for better performance */
    S32 x1, y1, x2, y2, x, y, cx, cy;
    S32 i, j, k = 0, group_ID;
    U8 done = 0, inner_done, display_flag;
    S32 old_i;   

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Status bar 1: Horizontal bar  */

     
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
     
    if ((is_on_idlescreen() == 1) || (GetActiveScreenId() == MAIN_MENU_SCREENID))
         
    {
        set_vertical_signal_status_icons();
        ChangeStatusIconImageWithoutReArrange(STATUS_ICON_BATTERY_STRENGTH, IMG_SI_BATTERY_STRENGTH_IDLE);
        MMI_status_icon_bars[0].n_icons = 0;
    }
    else
    {
        set_horizontal_signal_status_icons();
        ChangeStatusIconImageWithoutReArrange(STATUS_ICON_BATTERY_STRENGTH, IMG_SI_BATTERY_STRENGTH);
        MMI_status_icons_list1[0] = STATUS_ICON_SIGNAL_STRENGTH;
        MMI_status_icons_list1[1] = STATUS_ICON_BATTERY_STRENGTH;
        MMI_status_icon_bars[0].n_icons = 2;
    }
     
#elif defined(__MMI_UI_DALMATIAN_STATUSBAR__)
     
    if (is_on_idlescreen() == 1)
         
    {
        MMI_status_icon_bars[0].n_icons = 0;    /* signal icon is now placed on title bar on idle screen. */
    }
    else
    {
        MMI_status_icons_list1[0] = STATUS_ICON_SIGNAL_STRENGTH;
        MMI_status_icon_bars[0].n_icons = 1;
    }
     
#else 
    MMI_status_icons_list1[0] = STATUS_ICON_SIGNAL_STRENGTH;
    MMI_status_icons_list1[1] = STATUS_ICON_BATTERY_STRENGTH;
    MMI_status_icon_bars[0].n_icons = 2;
#endif 
     

    x1 = MMI_status_icon_bars[0].x1;    /* start x position of horizontal status bar main lcd */
    y1 = MMI_status_icon_bars[0].y1;    /* start y position of horizontal status bar main lcd */
    x2 = MMI_status_icon_bars[0].x2;    /* end x position of horizontal status bar main lcd */
    y2 = MMI_status_icon_bars[0].y2;    /* end y position of horizontal status bar main lcd */

     
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
     
    if (is_on_idlescreen() == 0 && (!(GetActiveScreenId() == MAIN_MENU_SCREENID)))
         
    {
        /* Shift x1,x2 assuming signal strength and battery strength icons are at either ends  */
        x1 += MMI_status_icons[MMI_status_icons_list1[0]].width - 1 + STATUS_ICON_X_GAP;
        x2 = MMI_status_icons[MMI_status_icons_list1[1]].x - STATUS_ICON_X_GAP;
    }
     
#elif defined(__MMI_UI_DALMATIAN_STATUSBAR__)
     
    if (is_on_idlescreen() == 0)
         
    {
        x1 += MMI_status_icons[MMI_status_icons_list1[0]].width - 1 + STATUS_ICON_X_GAP;
    }
    if (is_status_bar_shuffled == 0)
    {
        x2 = UI_device_width - MMI_STATUS_BAR_WIDTH;
    }
    else
    {
        x2 -= 6;
    }
     
#else 
    /* Shift x1,x2 assuming signal strength and battery strength icons are at either ends  */
    x1 += MMI_status_icons[MMI_status_icons_list1[0]].width - 1 + STATUS_ICON_X_GAP;
    x2 = MMI_status_icons[MMI_status_icons_list1[1]].x - STATUS_ICON_X_GAP;
#endif 
     

#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
    cy = y2 - y1 + 1;
#else 
    cy = (y2 - y1 + 1) >> 1;
#endif 

     
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)

     
    if (is_on_idlescreen() == 1)
         
    {
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].x = 1;
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].y = MMI_status_bar_height
            + MMI_IDLESCREEN_BAR_HEIGHT - 2 - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].height;

        MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].x = MAIN_LCD_device_width
            - 1 - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width;
        MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].y = MMI_status_bar_height
            + MMI_IDLESCREEN_BAR_HEIGHT - 2 - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].height;
    }
    else if (GetActiveScreenId() == MAIN_MENU_SCREENID)
    {
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].x = 1;
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].y = MMI_status_bar_height
            + MMI_title_height - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].height - 1;
        MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].x = MAIN_LCD_device_width
            - 1 - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width;
        MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].y = MMI_status_bar_height
            + MMI_title_height - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].height - 1;
    }
    else
    {
    #if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].x = 1;
        MMI_status_icons[0].y = cy - MMI_status_icons[0].height;
        MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].x = MAIN_LCD_device_width
            - 1 - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width;
        MMI_status_icons[1].y = cy + 1 - MMI_status_icons[1].height;
        x2 = MMI_status_icons[MMI_status_icons_list1[1]].x - STATUS_ICON_X_GAP;
    #else /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
        MMI_status_icons[0].y = cy - (MMI_status_icons[0].height >> 1);
        MMI_status_icons[1].y = cy - (MMI_status_icons[1].height >> 1);
    #endif /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
    }
     
#elif defined(__MMI_UI_DALMATIAN_STATUSBAR__)
#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
    // set the position of signal strength icon.
     
    if (is_on_idlescreen() == 1)
         
    {
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].y =
            (MMI_title_height >> 1) - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].height >> 1);
    }
    else
    {
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].y = cy - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].height;
    }
#else /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
     
    if (is_on_idlescreen() == 1)
         
    {
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].y =
            (MMI_title_height >> 1) - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].height >> 1);
    }
    else
    {
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].y = cy - (MMI_status_icons[0].height >> 1);
    }
#endif /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
     
#else 
#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
 
    if (whether_status_bar_at_bottom())
  
    {
        MMI_status_icons[0].y =
            UI_device_height - MMI_button_bar_height - 1 - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].height + 2);
        MMI_status_icons[1].y =
            UI_device_height - MMI_button_bar_height - 1 - (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].height + 2);
    }
    else
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
    {
        MMI_status_icons[0].y = cy - MMI_status_icons[0].height - 1;//080206 status icon
        MMI_status_icons[1].y = cy - MMI_status_icons[1].height - 1;
    }
     
#else /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
 
    if (whether_status_bar_at_bottom())
  
    {
        MMI_status_icons[0].y =
            UI_device_height - MMI_button_bar_height - 1 - MMI_status_bar_height - 2 +
            ((MMI_status_bar_height - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].height) >> 1);
        MMI_status_icons[1].y =
            UI_device_height - MMI_button_bar_height - 1 - MMI_status_bar_height - 2 +
            ((MMI_status_bar_height - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].height) >> 1);
    }
    else
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
    {
        MMI_status_icons[0].y = cy - (MMI_status_icons[0].height >> 1);
        MMI_status_icons[1].y = cy - (MMI_status_icons[1].height >> 1);
    }
     
#endif /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
#endif 
     

    i = 2;
     
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)

     
    if ((is_on_idlescreen() == 1) || (GetActiveScreenId() == MAIN_MENU_SCREENID))
         
    {

        x = (MAIN_LCD_device_width >> 1) - ((NumberOfDisplayedIcons()
                                             * (MMI_status_icons[2].width + STATUS_ICON_X_GAP)) >> 1)
               /*- STATUS_ICON_X_GAP */ ;
        if (x < (x1 + 2))
        {
            x = x1 + 2;
        }
    }
    else
#endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
         
        x = x1 + STATUS_ICON_X_GAP;
    while (!done)
    {
        display_flag = 0;
        j = MMI_status_icons_pool1[i];
        old_i = i;   
        i++;
        if (i > MMI_N_STATUS_ICONS_POOL1)
        {
            break;
        }
        if (TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
        {
            k = j;
            display_flag = 1;
        }
        if ((MMI_status_icons[j].group_ID > 0))
        {
            group_ID = MMI_status_icons[j].group_ID;
            j++;
            inner_done = 0;
            while (!inner_done)
            {
                if (group_ID != MMI_status_icons[j].group_ID)
                {
                    break;
                }
                if ((!display_flag) && TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
                {
                    display_flag = 1;
                    k = j;
                }
                else
                {
                    i++;
                    j++;
                }
            }
        }
        if (display_flag)   /* if flagis set to display */
        {
            /* if((x+MMI_status_icons[k].width+STATUS_ICON_X_GAP)>x2) */
            if ((x + MMI_status_icons[k].width) > x2)    
            {                                           /* i--; */
                i = old_i;                               
                break;
            }
        #if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
             
        #if defined(__MMI_UI_DALMATIAN_STATUSBAR__) && (defined(__MMI_MAINLCD_176X220__)|| defined(__MMI_MAINLCD_220X176__))
            y = cy - MMI_status_icons[k].height - 1;
        #else /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) && (defined(__MMI_MAINLCD_176X220__)|| defined(__MMI_MAINLCD_220X176__)) */ 
             
        #ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
 
            if (whether_status_bar_at_bottom())
  
            {
                y = UI_device_height - MMI_button_bar_height - 1 - MMI_status_icons[k].height - 2;
            }
            else
        #endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
                 
                y = cy - MMI_status_icons[k].height - 1;//080206 status icon
        #endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) && (defined(__MMI_MAINLCD_176X220__)|| defined(__MMI_MAINLCD_220X176__)) */ 
             
        #else /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
             
        #ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
 
            if (whether_status_bar_at_bottom())
  
            {
                y = UI_device_height - MMI_button_bar_height - 1 - MMI_status_bar_height +
                    ((MMI_status_bar_height - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].height) >> 1);
            }
            else
        #endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
                 
                y = cy - (MMI_status_icons[k].height >> 1);
        #endif /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
            MMI_status_icons[k].x = x;
            MMI_status_icons[k].y = y;
            x += MMI_status_icons[k].width + STATUS_ICON_X_GAP;
            MMI_status_icons_list1[MMI_status_icon_bars[0].n_icons] = (S16) k;
            MMI_status_icon_bars[0].n_icons++;
            if (MMI_status_icon_bars[0].n_icons >= MMI_N_STATUS_ICONS_LIST1)
            {
                done = 1;
            }
        }
    }

    /* Status bar 2: Vertical bar */

#ifndef __MMI_BLACKBERRY_QWERTY__
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
     
    if (is_on_idlescreen() == 1)
         
    {
        MMI_status_icon_bars[1].n_icons = 0;    /* battery icon is now placed on title bar on idle screen. */
    }
    else
    {
        MMI_status_icons_list2[0] = STATUS_ICON_BATTERY_STRENGTH;
        MMI_status_icon_bars[1].n_icons = 1;
    }
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
    MMI_status_icon_bars[1].n_icons = 0;
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
    x1 = MMI_status_icon_bars[1].x1;    /* start x position of vertical  status bar main lcd */
    y1 = MMI_status_icon_bars[1].y1;    /* start y position of vertical status bar main lcd */
    x2 = MMI_status_icon_bars[1].x2;    /* end x position of vertical status bar main lcd */
    y2 = MMI_status_icon_bars[1].y2;    /* end y position of horizontal status bar main lcd */
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    /* to handle shuffling of status bar on idle screen. */
    if (is_status_bar_shuffled == 0)
    {
        x = 6;
    }
    else
    {
        x = 2 * MMI_STATUS_BAR_WIDTH - UI_device_width + STATUS_ICON_X_GAP;
    }
     
    if (is_on_idlescreen() == 0)
         
    {
        x2 = MMI_STATUS_BAR_WIDTH - MMI_status_icons[MMI_status_icons_list2[0]].width;
    }
    else
    {
        x2 = MMI_STATUS_BAR_WIDTH - 1;
    }
#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
     
    if (is_on_idlescreen() == 1)
         
    {
        MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].x =
            UI_device_width - 1 - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width;
        MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].y =
            (MMI_title_height >> 1) - (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].height >> 1);
    }
    else
    {
        MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].x = (UI_device_width - 1)
            - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width - MMI_status_icon_bars[1].x;
        MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].y = cy - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].height;
    }
#else /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
     
    if (is_on_idlescreen() == 1)
         
    {
        MMI_status_icons[1].y = MMI_title_height >> 1;
    }
    else
    {
        MMI_status_icons[1].y = cy - (MMI_status_icons[1].height >> 1);
    }
#endif /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
    cx = (x2 - x1 + 1) >> 1;
     
#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
 
    if (whether_status_bar_at_bottom())
  
    {
   
    #if defined(__MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_160X128__)
        y = UI_device_height - MMI_button_bar_height - MMI_status_bar_height -
            ((MMI_status_icons[1].height + (STATUS_ICON_Y_GAP) << 1) + STATUS_ICON_Y_GAP + 2) - 1;
    #elif  defined __MMI_MAINLCD_128X128__
        y = UI_device_height - MMI_button_bar_height - MMI_status_bar_height -
            ((MMI_status_icons[1].height + (STATUS_ICON_Y_GAP) << 1) + STATUS_ICON_Y_GAP + 2) - 1;
    #else /* __MMI_MAINLCD_128X160__ */ 
        y = UI_device_height - MMI_button_bar_height - MMI_status_bar_height -
            ((MMI_status_icons[1].height + (STATUS_ICON_Y_GAP) << 1) + STATUS_ICON_Y_GAP) - 1;
    #endif /* __MMI_MAINLCD_128X160__ */ 
 
    }
    else
#endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
    {
        y = STATUS_ICON_Y_GAP;
    }
     
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
    done = 0;
    while (!done)
    {
        display_flag = 0;
        j = MMI_status_icons_pool1[i];
        old_i = i;
        i++;
        if (i > MMI_N_STATUS_ICONS_POOL1)
        {
            break;
        }
        if (TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))     /* check if the flag is on /off */
        {
            k = j;
            display_flag = 1;
        }
        if ((MMI_status_icons[j].group_ID > 0))
        {
            group_ID = MMI_status_icons[j].group_ID;
            j++;
            inner_done = 0;
            while (!inner_done)
            {
                if (group_ID != MMI_status_icons[j].group_ID)
                {
                    break;
                }
                if ((!display_flag) && TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
                {
                    display_flag = 1;
                    k = j;
                }
                else
                {
                    i++;
                    j++;
                }
            }
        }
        if (display_flag)
        {
             
        #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
            /* to handle horizontal alignment of status bar. */
            if ((x + MMI_status_icons[k].width + STATUS_ICON_X_GAP) > x2)
        #else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
             
        #ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
            if ((y - MMI_status_icons[k].height) < y1)
        #else /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
            /* if((y1+y+MMI_status_icons[k].height+STATUS_ICON_Y_GAP)>y2) */
            if ((y1 + y + MMI_status_icons[k].height) > y2)  
        #endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
                 
        #endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
                 
            {                                               /* i--; */
                i = old_i;
                break;
            }
             
        #ifndef __MMI_UI_DALMATIAN_STATUSBAR__
            x = cx - (MMI_status_icons[k].width >> 1);
        #endif 
             
            MMI_status_icons[k].x = x;  /* set the x and y position of icon */
            MMI_status_icons[k].y = y;
             
        #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
            x += MMI_status_icons[k].width + STATUS_ICON_X_GAP;
        #else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
             
        #ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
 
            if (whether_status_bar_at_bottom())
  
            {
                y = MMI_status_icons[k].y - (MMI_status_icons[k].height + STATUS_ICON_Y_GAP);
            }
        #else /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
            y += MMI_status_icons[k].height + STATUS_ICON_Y_GAP;
        #endif /* __MMI_UI_STATUS_BAR_AT_BOTTOM__ */ 
             
        #endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
             
            MMI_status_icons_list2[MMI_status_icon_bars[1].n_icons] = (S16) k;
            MMI_status_icon_bars[1].n_icons++;
            if (MMI_status_icon_bars[1].n_icons >= MMI_N_STATUS_ICONS_LIST2)
            {
                done = 1;
            }
        }
    }

#endif
    /* Status bar 3: Sub LCD status bar */

     
    MMI_status_icons_list3[0] = STATUS_ICON_SUBLCD_SIGNAL_STRENGTH;
    MMI_status_icons_list3[1] = STATUS_ICON_SUBLCD_BATTERY_STRENGTH;
    MMI_status_icon_bars[2].n_icons = 2;

    x1 = MMI_status_icon_bars[2].x1;    /* start x position of status bar sub lcd */
    y1 = MMI_status_icon_bars[2].y1;    /* start y position of status bar sub lcd */
    x2 = MMI_status_icon_bars[2].x2;    /* end x position of status bar sub lcd */
    y2 = MMI_status_icon_bars[2].y2;    /* end y position of status bar sub lcd */

    /* Shift x1,x2 assuming signal strength and battery strength icons are at either ends  */
    x1 += MMI_status_icons[MMI_status_icons_list3[0]].width - 1 + SUBLCD_STATUS_ICON_X_GAP;
    x2 = MMI_status_icons[MMI_status_icons_list3[1]].x - SUBLCD_STATUS_ICON_X_GAP;
     

#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
    cy = y2 - y1 - 1;
#else 
    cy = (y2 - y1 + 1) >> 1;
#endif 

    x = x1 + SUBLCD_STATUS_ICON_X_GAP;

#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
    cy = MMI_status_icon_bars[2].y2 - MMI_status_icon_bars[2].y1 - 1;
#endif 

    done = 0;
    i = 2;
    while (!done)
    {
        display_flag = 0;
        j = MMI_status_icons_pool2[i];
        i++;
        if (i > MMI_N_STATUS_ICONS_POOL2)
        {
            break;
        }
        if (TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
        {
            k = j;
            display_flag = 1;
        }
        if ((MMI_status_icons[j].group_ID > 0))
        {
            group_ID = MMI_status_icons[j].group_ID;
            j++;
            inner_done = 0;
            while (!inner_done)
            {
                if (group_ID != MMI_status_icons[j].group_ID)
                {
                    break;
                }
                if ((!display_flag) && TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
                {
                    display_flag = 1;
                    k = j;
                }
                else
                {
                    i++;
                    j++;
                }
            }
        }
        if (display_flag)
        {
             
            //if ( x + MMI_status_icons[k].width+SUBLCD_STATUS_ICON_X_GAP > x2 )
            if (x + MMI_status_icons[k].width > x2)  
            {
                break;
            }
            MMI_status_icons_list3[MMI_status_icon_bars[2].n_icons] = (S16) k;
            MMI_status_icon_bars[2].n_icons++;
        #if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
            y = cy - MMI_status_icons[k].height;
        #else 
            y = cy - (MMI_status_icons[k].height >> 1);
        #endif 
            MMI_status_icons[k].x = x;
            MMI_status_icons[k].y = y;
            x += MMI_status_icons[k].width + SUBLCD_STATUS_ICON_X_GAP;
             
        }
    }

#ifdef __MMI_LCD_PARTIAL_ON__
    /* Extra status bar: Used in LCD partial display */
    MMI_status_icons_partial_display_n_icons = 0;
    done = 0;
    i = 0;
    while (!done)
    {
        display_flag = 0;
        j = MMI_status_icons_pool_partial_display[i];
        i++;

        if (i > MMI_N_STATUS_ICONS_POOL_PARTIAL_DISPLAY)
        {
            break;
        }

        if (TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
        {
            k = j;
            display_flag = 1;
        }
        if (display_flag)
        {
            MMI_status_icons_list_partial_display[MMI_status_icons_partial_display_n_icons] = (S16) k;
            MMI_status_icons_partial_display_n_icons++;
            if (MMI_status_icons_partial_display_n_icons >= MMI_N_STATUS_ICONS_PARTIAL_DISPLAY)
            {
                done = 1;
            }
        }
    }
#endif /* __MMI_LCD_PARTIAL_ON__ */ 
#endif /* (0) */ 
}


/*****************************************************************************
 * FUNCTION
 *  hide_status_icon_bars
 * DESCRIPTION
 *  call hide function of status bar (horizontal/vertical status bar of main
 *  lcd and horizontal status bar of sub lcd
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void hide_status_icon_bars(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__)	
    for (i = 0; i < 1; i++)
#else
    for (i = 0; i < MAX_STATUS_ICON_BARS; i++)
#endif		
    {
        /* if ( MMI_status_icon_bars[i].flags & STATUS_ICON_BAR_DISPLAY  && MMI_status_icon_bars[i].n_icons>0 ) */
        if (MMI_status_icon_bars[i].flags & STATUS_ICON_BAR_DISPLAY)     
        {
            if (MMI_status_icon_bars[i].graphics_context == &main_LCD_device_bitmap)
            {
                if (status_icon_display_area != SUBLCD_ONLY)
                {
                    MMI_hide_status_icon_bars[i] ();    /* execute the hide function of status bar */
                }
            }
            else if (MMI_status_icon_bars[i].graphics_context == &sub_LCD_device_bitmap)
            {
                if (status_icon_display_area != MAINLCD_ONLY)
                {
                    MMI_hide_status_icon_bars[i] ();    /* execute the hide function of status bar */
                }
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  refresh_fixed_status_icon
 * DESCRIPTION
 *  refrsh particulr icon of status bar
 * PARAMETERS
 *  icon_id     [IN]        
 *  bar_id      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_IDLE_FULL_SCREEN__
extern U16 on_idle_screen;
#endif 
extern void draw_wallpaper(void);
extern U16 GetActiveScreenId(void);

 

#ifdef __MMI_SUB_WALLPAPER__
 
extern int is_sub_idle_screen;
extern void cat312_hide_status_icon_bar(void);
#endif /* __MMI_SUB_WALLPAPER__ */ 

GDI_HANDLE wgui_status_icon_display_layer = GDI_LAYER_EMPTY_HANDLE;

void refresh_fixed_status_icon(S16 icon_id, S16 bar_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    // for refreshing the signal and battery icon on idle screen.
     
    if ((is_on_idlescreen()) ||
        ((status_icon_display_ON) && (MMI_status_icon_bars[bar_id].flags & STATUS_ICON_BAR_DISPLAY)))
         
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
    if (
	#ifdef __MMI_MAINLCD_220X176__
 		(g_dm_data.S32CatId != MMI_CATEGORY16_ID)
	#else
		(GetActiveScreenId() != IDLE_SCREEN_DIGIT_HANDLER_ID)
	#endif
	&&(status_icon_display_ON) && (MMI_status_icon_bars[bar_id].flags & STATUS_ICON_BAR_DISPLAY))
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
         
    {
        S32 x1, x2, y1, y2;

        /* backup active lcd */
        bitmap *saved_graphics_context = current_LCD_device_bitmap;
        color_t c;

         
    #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
        /* filler is not required for sub-LCD. */
        if (bar_id != 2)
            /* c = MMI_status_icon_bars[bar_id].normal_bar_filler->c; */
        {
            c = current_MMI_theme->LSK_up_filler->c;
        }
         
        if (is_on_idlescreen() == 1)
             
        {
            x1 = MMI_status_icons[icon_id].x;
            y1 = MMI_status_icons[icon_id].y;
        }
        else
        {
            x1 = MMI_status_icon_bars[bar_id].x + MMI_status_icons[icon_id].x;  /* x start position of icon */
            y1 = MMI_status_icon_bars[bar_id].y + MMI_status_icons[icon_id].y;  /* y start poition of icon */
        }
    #else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
        if (current_MMI_theme != NULL)
        {
            c = *current_MMI_theme->statusbar0_background_color;        /*  Color is currently hardcoded: must be calling hide function instead  */
        }
        else
        {
            c = gui_color(216, 212, 212);
        }

        /* set drawing region */
        x1 = MMI_status_icons[icon_id].x;   /* x start position of icon */
        y1 = MMI_status_icons[icon_id].y;   /* y start poition of icon */
    #endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
         

        x2 = x1 + MMI_status_icons[icon_id].width - 1;  /* x end position of icon */
        y2 = y1 + MMI_status_icons[icon_id].height - 1; /* y end position of icon */

        /* set active lcd */
        if (bar_id == 2)
        {
            UI_set_sub_LCD_graphics_context();  /* if barid is equal to 2 then switch the graphic context to sub lcd. */
        }
        else
        {
            UI_set_main_LCD_graphics_context(); /* switch graphic context to main lcd if bar id is eqaul to 0 or 1 */
        }

        if (wgui_status_icon_display_layer != GDI_LAYER_EMPTY_HANDLE)
        {
            gdi_layer_push_and_set_active(wgui_status_icon_display_layer);
        }
        /* lock frame buffer */
        gdi_layer_lock_frame_buffer();
        
        /* 061506 double buffer Start */
        if(get_status_icon_bar_layer((U8)bar_id)!=GDI_ERROR_HANDLE)
        {
            gdi_layer_push_and_set_active(get_status_icon_bar_layer((U8)bar_id));
        }
        /* 061506 double buffer End */

        /* set clip region */
        gui_push_clip();
        gui_set_clip(x1, y1, x2, y2);

        if (bar_id == 2)    /* SUBLCD status icons */
        {
        #ifdef __MMI_SUB_WALLPAPER__
            if (is_sub_idle_screen)
                 
            {
                gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
            }
             
            else
            {
                show_sublcd_status_background(x1, y1, x2, y2, MMI_TRUE);
            }
        #else /* __MMI_SUB_WALLPAPER__ */ 
            show_sublcd_status_background(x1, y1, x2, y2, MMI_TRUE);
        #endif /* __MMI_SUB_WALLPAPER__ */ 
        }
        else
        {
        #ifdef __MMI_IDLE_FULL_SCREEN__
            if (on_idle_screen == 0)
            {
                 
            #if defined(__MMI_UI_TECHNO_STATUS_ICON__)
                if (GetActiveScreenId() == MAIN_MENU_SCREENID)
                     
                {
                    gui_fill_rectangle(x1, y1, x2, y2, current_UI_theme->window_title_theme->active_filler->c);
                }
                 
                 
            #elif defined(__MMI_UI_DALMATIAN_STATUSBAR__)
                show_status_icons();
                 
            #else 
#if !defined(__MMI_MAINLCD_220X176__)//guoyt modify for useless rect background.  			
                 
                if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
                {
                    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
                }
                else
                {
                    gui_fill_rectangle(x1, y1, x2, y2, c);  /* fill the icon position with grey color */
                }
#endif				
		#if defined(__MMI_UI_STATUSBAR_USE_IMAGE_BACKGROUND__)
		  gui_show_image(x1, y1,(PU8)GetImage(IMG_SI_STATUSBAR_BG));
		#endif
                 
                 
                //  draw_wallpaper();
            #endif 
                 
            }
            else
            {
            #if defined(__MMI_UI_TECHNO_STATUS_ICON__)
                gui_show_image(0, MMI_status_bar_height, get_image(idlescreen_bar_id));//070306 Alpha layer
            #elif defined(__MMI_UI_DALMATIAN_STATUSBAR__)
                gui_draw_filled_area(
                    MMI_title_x,
                    MMI_title_y,
                    MMI_title_x + MMI_title_width - 1,
                    MMI_title_y + MMI_title_height - 1,
                    current_UI_theme->window_title_theme->active_filler);
                 
            #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
                if (is_enable_animated_title_effect())
                {
                    wgui_draw_animate_title_image_frame_0(
                        MMI_title_x,
                        MMI_title_y,
                        MMI_title_x + MMI_title_width - 1,
                        MMI_title_y + MMI_title_height - 1);
                }
            #endif /* __MMI_UI_ANIMATED_TITLE_EFFECT__ */ 
                 
            #else 
            #ifdef __GDI_MEMORY_PROFILE_2__
                /* gui_fill_rectangle(x1,y1,x2,y2,wgui_transparent_color); */
                gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);      
            #else /* __GDI_MEMORY_PROFILE_2__ */ 
                draw_wallpaper();
            #endif /* __GDI_MEMORY_PROFILE_2__ */ 
            #endif 
            }
        #else /* __MMI_IDLE_FULL_SCREEN__ */ 
            gui_fill_rectangle(x1, y1, x2, y2, c);  /* fill the icon position with grey color */
        #endif /* __MMI_IDLE_FULL_SCREEN__ */ 
        }

        /* show the icon */
    #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
        if (mmi_tm_does_theme_have_alt_image())
        {
            if (mmi_tm_is_theme_dirty_bit_set(GetImageIndex(MMI_status_icons[icon_id].icon_ID)))
            {
                MMI_status_icons[icon_id].icon_data = get_image(MMI_status_icons[icon_id].icon_ID);
                /* No need to calculate Image width, height and frames as they will not change */
            }
        }
    #endif /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 
         
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)||defined(__MMI_MAINLCD_220X176__)
        /* for showing the animated battery icon when charger is connected. */
        if (MMI_status_icons[icon_id].flags & STATUS_ICON_ANIMATE)
        {
            gui_show_transparent_animated_image_frame(
                x1,
                y1,
                MMI_status_icons[icon_id].icon_data,
                0,
                MMI_status_icons[icon_id].current_frame);
             
        #if 0
        #endif /* 0 */ 
             
        }
        else
    #endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
             
             
    #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
            /* for showing the animated battery icon when charger is connected. */
        if (MMI_status_icons[icon_id].flags & STATUS_ICON_ANIMATE)
        {
            gui_show_transparent_animated_image_frame(
                x1,
                y1,
                MMI_status_icons[icon_id].icon_data,
                0,
                MMI_status_icons[icon_id].current_frame);
             
        #if 0
        #endif /* 0 */ 
             
        }
        else
    #endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
             
            gdi_image_draw_animation_single_frame(
                x1,
                y1,
                MMI_status_icons[icon_id].icon_data,
                MMI_status_icons[icon_id].state);

        /* restoer clip region */
        gui_pop_clip();
        
        /* 061506 double buffer Start */
        if(get_status_icon_bar_layer((U8)bar_id)!=GDI_ERROR_HANDLE)
        {
            gdi_layer_pop_and_restore_active();
        }
        /* 061506 double buffer End */

        /* unlock and blt frame buffer */
        gdi_layer_unlock_frame_buffer();
        gui_BLT_double_buffer(x1, y1, x2, y2);

        /* 061506 double buffer Start */
        if(get_status_icon_bar_type((U8)bar_id)&STATUS_ICON_BAR_ENABLE_DOUBLE_BUFFER)
        {
            gdi_layer_push_and_set_active(get_status_icon_bar_layer((U8)bar_id));
            gdi_layer_toggle_double();
            gdi_layer_copy_double();//070306 copy double
            gdi_layer_pop_and_restore_active();
        }
        /* 061506 double buffer Start */

        if (wgui_status_icon_display_layer != GDI_LAYER_EMPTY_HANDLE)
        {
            gdi_layer_pop_and_restore_active();
        }

        /* restore active lcd according to backuped lcd */
        if (saved_graphics_context == &main_LCD_device_bitmap)
        {
            UI_set_main_LCD_graphics_context();
        }
        else if (saved_graphics_context == &sub_LCD_device_bitmap)
        {
            UI_set_sub_LCD_graphics_context();
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  refresh_status_icon_display
 * DESCRIPTION
 *  refrsh all status bar icons
 * PARAMETERS
 *  void
 *  S16(?)      [IN]        Bar_id
 * RETURNS
 *  void
 *****************************************************************************/
void refresh_status_icon_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 idx = 0;//061506 double buffer
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (status_icon_display_ON)
    {
        /* backup active lcd */
        bitmap *saved_graphics_context = current_LCD_device_bitmap;
        S32 i;

        if (wgui_status_icon_display_layer != GDI_LAYER_EMPTY_HANDLE)
        {
            gdi_layer_push_and_set_active(wgui_status_icon_display_layer);
        }
        /* lock frame buffer */
        gui_lock_double_buffer();

        /* hide all status bars */
        hide_status_icon_bars();

        /* show all status bar icons */
        show_status_icons();

        /* unlock frame buffer */
        gui_unlock_double_buffer();

#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__)	
        for (i = 0; i < 1; i++)
#else
        for (i = 0; i < MAX_STATUS_ICON_BARS; i++)
#endif			
        {
            if (MMI_status_icon_bars[i].flags & STATUS_ICON_BAR_DISPLAY)
            {
                if (MMI_status_icon_bars[i].graphics_context == &main_LCD_device_bitmap)
                {
                    if (status_icon_display_area != SUBLCD_ONLY)
                    {
                        UI_set_main_LCD_graphics_context(); /* switch to main lcd graphic context */
                        gui_BLT_double_buffer(
                            MMI_status_icon_bars[i].x1,
                            MMI_status_icon_bars[i].y1,
                            MMI_status_icon_bars[i].x2,
                            MMI_status_icon_bars[i].y2);
                    }
                }
                else if (MMI_status_icon_bars[i].graphics_context == &sub_LCD_device_bitmap)
                {
                    if (status_icon_display_area != MAINLCD_ONLY)
                    {
                        UI_set_sub_LCD_graphics_context();  /* switch to sub lcd graphic context */
                        gui_BLT_double_buffer(
                            MMI_status_icon_bars[i].x1,
                            MMI_status_icon_bars[i].y1,
                            MMI_status_icon_bars[i].x2,
                            MMI_status_icon_bars[i].y2);
                    }
                }
            }
        }

        /* 061506 double buffer Start */
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__)		
        for (i = 0; i < 1; i++)		
#else			
        for (idx=0; idx<MAX_STATUS_ICON_BARS; idx++)
#endif			
        {
            if(get_status_icon_bar_type((U8)idx)&STATUS_ICON_BAR_ENABLE_DOUBLE_BUFFER)
            {
                gdi_layer_push_and_set_active(get_status_icon_bar_layer((U8)idx));
                gdi_layer_toggle_double();
                gdi_layer_copy_double();//070306 copy double
                gdi_layer_pop_and_restore_active();
            }
        }
        /* 061506 double buffer Start */
    
        /* restore active lcd according to backuped lcd */
        if (saved_graphics_context == &main_LCD_device_bitmap)
        {
            UI_set_main_LCD_graphics_context();
        }
        else if (saved_graphics_context == &sub_LCD_device_bitmap)
        {
            UI_set_sub_LCD_graphics_context();
        }
        if (wgui_status_icon_display_layer != GDI_LAYER_EMPTY_HANDLE)
        {
            gdi_layer_pop_and_restore_active();
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  animate_status_icon
 * DESCRIPTION
 *  animate teh status icon
 * PARAMETERS
 *  icon_ID         [IN]        
 *  icon_id(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void animate_status_icon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    /* set the flaf of icon animated */
    MMI_status_icons[icon_ID].flags |= STATUS_ICON_ANIMATE; /* set the flag of icon to animate */

    /* set teh currenty frame to zero */
    MMI_status_icons[icon_ID].current_frame = 0;

    setup_status_icon_blink_animate();
}


/*****************************************************************************
 * FUNCTION
 *  forward_animate_status_icon
 * DESCRIPTION
 *  show the animation of icon such that it is moving forward direction
 * PARAMETERS
 *  icon_ID         [IN]        
 *  icon_id(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void forward_animate_status_icon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    /* set teh flag of icon to forward animate */
    MMI_status_icons[icon_ID].flags |= STATUS_ICON_FORWARD_ANIMATE;

    setup_status_icon_blink_animate();
}


/*****************************************************************************
 * FUNCTION
 *  reverse_animate_status_icon
 * DESCRIPTION
 *  show the animation of icon such that it is moving in reverse ditrection
 * PARAMETERS
 *  icon_ID         [IN]        
 *  icon_id(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void reverse_animate_status_icon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    /* set the flag of icon to reverse animate */
    MMI_status_icons[icon_ID].flags |= STATUS_ICON_REVERSE_ANIMATE;

    setup_status_icon_blink_animate();
}


/*****************************************************************************
 * FUNCTION
 *  blink_status_icon
 * DESCRIPTION
 *  set teh flag of icon as blink and shw the blinking icon
 * PARAMETERS
 *  icon_ID         [IN]        
 *  icon_id(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
	extern pBOOL mmi_bootup_is_sim_valid();
	extern pBOOL mmi_bootup_get_active_flight_mode();

void blink_status_icon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    /* 2005/10/20 Hide status icon in flight mode */
#ifdef __FLIGHT_MODE_SUPPORT__
    if (mmi_bootup_get_active_flight_mode() == 1 || mmi_bootup_is_sim_valid() == MMI_FALSE)
    {
        pBOOL rtn = PhnsetFlightModeHideButSaveStatusIcon(icon_ID, MMI_status_icons[icon_ID].flags | STATUS_ICON_BLINK);

        if (rtn == MMI_TRUE && (gPowerOnWithFlightMode == MMI_TRUE || mmi_bootup_is_sim_valid() == MMI_FALSE))
        {
            return;
        }
    }
#else /* __FLIGHT_MODE_SUPPORT__ */ 
    if (mmi_bootup_is_sim_valid() == MMI_FALSE)
    {
        if (PhnsetFlightModeHideButSaveStatusIcon(icon_ID, MMI_status_icons[icon_ID].flags | STATUS_ICON_BLINK) ==
            MMI_TRUE)
        {
            return;
        }
    }
#endif /* __FLIGHT_MODE_SUPPORT__ */ 

    /* set teh flag of icon to blink */
    MMI_status_icons[icon_ID].flags |= STATUS_ICON_BLINK;

    setup_status_icon_blink_animate();
}


/*****************************************************************************
 * FUNCTION
 *  disable_status_icon
 * DESCRIPTION
 *  set teh flag of icon as disable
 * PARAMETERS
 *  icon_ID         [IN]        
 *  icon_id(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void disable_status_icon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    /* set the flag of icon to disable */
    MMI_status_icons[icon_ID].flags |= STATUS_ICON_DISABLED;
}


/*****************************************************************************
 * FUNCTION
 *  enable_status_icon
 * DESCRIPTION
 *  set teh flag of icon as enable
 * PARAMETERS
 *  icon_ID         [IN]        
 *  icon_id(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void enable_status_icon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    /* set teh flag of icn to enbale */
    MMI_status_icons[icon_ID].flags &= ~STATUS_ICON_DISABLED;
}


/*****************************************************************************
 * FUNCTION
 *  set_status_icon_state
 * DESCRIPTION
 *  set the state of icon equal to valeu pass as parameter
 * PARAMETERS
 *  icon_ID         [IN]        
 *  state           [IN]        
 *  icon_id(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_status_icon_state(S16 icon_ID, U8 state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* set teh flag of icon equal to valeu pass as paraeter */
    MMI_status_icons[icon_ID].state = state;
}


/*****************************************************************************
 * FUNCTION
 *  reset_status_icon_state
 * DESCRIPTION
 *  set the state of icon equal to zero default value
 * PARAMETERS
 *  icon_ID         [IN]        
 *  icon_id(?)      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void reset_status_icon_state(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* set teh flag of icon to default 0 value */
    MMI_status_icons[icon_ID].state = 0;
}


/*****************************************************************************
 * FUNCTION
 *  fill_status_bar_color
 * DESCRIPTION
 *  fill teh clor of status bar as grey
 * PARAMETERS
 *  bar_id      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void fill_status_bar_color(U8 bar_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    color_t c;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (current_MMI_theme != NULL)
    {
        c = *current_MMI_theme->statusbar0_background_color;    /* gui_color(216,212,212); */
    }
    else
    {
        c = gui_color(216, 212, 212);
    }

    /* set clip region */
    gui_push_clip();
    get_status_icon_bar_clip(bar_id, &x1, &y1, &x2, &y2);   /* get teh clip values of bar_id */
    gui_set_clip(x1, y1, x2, y2);

    /* draw status icon bar */
    gui_fill_rectangle(x1, y1, x2, y2, c);  /* fill the bar with grey color */

    /* restore clip region */
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  redraw_status_icon_bar
 * DESCRIPTION
 *  redarw only the staus bar whose id is equl to valeu pass as parameter
 * PARAMETERS
 *  j               [IN]        
 *  bar_id(?)       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_status_icon_bar(U8 j)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* backup active lcd */
    bitmap *saved_graphics_context = current_LCD_device_bitmap;
    S32 i, id, sbx, sby;
    S32 ix1, iy1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_status_icon_bars[j].flags & STATUS_ICON_BAR_DISPLAY)
    {
        /* set active lcd according to different status icon bar */
        if (MMI_status_icon_bars[j].graphics_context == &main_LCD_device_bitmap)        /* check the graphicd context of bar id */
        {
            UI_set_main_LCD_graphics_context();
        }
        else if (MMI_status_icon_bars[j].graphics_context == &sub_LCD_device_bitmap)
        {
            UI_set_sub_LCD_graphics_context();
        }

        sbx = MMI_status_icon_bars[j].x;    /* store the start offset x position of bar */
        sby = MMI_status_icon_bars[j].y;    /* store the start offset y position of bar */

        if (MMI_status_icon_bars[j].icon_list != NULL)
        {
            for (i = 0; i < MMI_status_icon_bars[j].n_icons; i++)
            {
                id = MMI_status_icon_bars[j].icon_list[i];
            #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__

                if (mmi_tm_does_theme_have_alt_image())
                {
                    if (mmi_tm_is_theme_dirty_bit_set(GetImageIndex((U16) id)))
                    {
                        MMI_status_icons[j].icon_data = get_image((U16) id);
                        /* No need to calculate Image width, height and frames as they will not change */
                    }
                }
            #endif /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 

                if (MMI_status_icons[id].icon_ID != IMAGE_ID_NULL)
                {
                    ix1 = MMI_status_icons[id].x + sbx; /* start x position of icon */
                    iy1 = MMI_status_icons[id].y + sby; /* start y position of icon */

                    if (MMI_status_icons[id].flags & STATUS_ICON_ANIMATE)
                    {
                        /* show animated icon */
                        gui_show_transparent_animated_image_frame(
                            ix1,
                            iy1,
                            MMI_status_icons[id].icon_data,
                            0,
                            MMI_status_icons[id].state);
                    }
                    else if (MMI_status_icons[id].flags & STATUS_ICON_BLINK)
                    {
                        /* show blinking icon */
                        if (MMI_status_icons_blink_animate_ON_state)
                        {
                            gui_show_transparent_image(ix1, iy1, MMI_status_icons[id].icon_data, 0);
                        }
                    }
                    else
                    {
                        /* show normal icon */
                        if (MMI_status_icons[id].state > 0)
                            /* the icon have many frames for different levels */
                        {
                            gui_show_transparent_animated_image_frame(
                                ix1,
                                iy1,
                                MMI_status_icons[id].icon_data,
                                0,
                                MMI_status_icons[id].state);
                        }
                        else
                            /* the icon has only one level */
                        {
                            gui_show_transparent_image(ix1, iy1, MMI_status_icons[id].icon_data, 0);
                        }
                    }
                }
            }
        }
    }

    /* restore active lcd according to backuped lcd */
    if (saved_graphics_context == &main_LCD_device_bitmap)
    {
        UI_set_main_LCD_graphics_context(); /* set graphic context to main lcd */
    }
    else if (saved_graphics_context == &sub_LCD_device_bitmap)
    {
        UI_set_sub_LCD_graphics_context();  /* set graphic context to sub lcd */
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_status_icon_display
 * DESCRIPTION
 *  set status icons display state (MAINLCD_ONLY, SUBLCD_ONLY, DEFAULT_BOTH)
 * PARAMETERS
 *  state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_status_icon_display(S32 state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    status_icon_display_area = state;
}


/*****************************************************************************
 * FUNCTION
 *  reset_status_icon_display
 * DESCRIPTION
 *  reset status icons display to DEFAULT_BOTH
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_status_icon_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    status_icon_display_area = DEFAULT_BOTH;
}


/*****************************************************************************
 * FUNCTION
 *  show_status_icons
 * DESCRIPTION
 *  show all three status bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern idle_context_struct g_idle_context;
extern pBOOL mmi_bootup_is_idle_screen_reached();
extern pBOOL mmi_idle_is_on_idle_screen();
#if defined(__MMI_SCREEN_ROTATE__)
 extern void  draw_rotated_statusbar(void);
#endif
extern pBOOL mmi_bootup_is_in_security_check();

void show_status_icons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j, id, sbx, sby;
    U8 blink_animate_flag = 0;
    S32 x1, y1, x2, y2;
    S32 ix1, iy1;

    /* backup active lcd */
    bitmap *saved_graphics_context = current_LCD_device_bitmap;

     
    U8 old_signal_state_main = 0;    

#ifdef __MMI_SUBLCD__
    U8 old_signal_state_sub = 0;
#endif 

     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    /* for handling the shuffling of status bar on idle screen. */
    S32 initial, increment;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/


    if (is_status_bar_shuffled == 1)
    {
        initial = MAX_STATUS_ICON_BARS - 1;
        increment = -1;
    }
    else
    {
        initial = 0;
        increment = 1;
    }
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
#if defined(__MMI_SCREEN_ROTATE__)
  if(mmi_frm_is_screen_width_height_swapped())
    {
        draw_rotated_statusbar();
        return ;
  }
#endif    
     
    if (mmi_bootup_is_in_security_check() == MMI_TRUE)
    {
        old_signal_state_main = MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].state;
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].state = 0;
    #ifdef __MMI_SUBLCD__
        old_signal_state_sub = MMI_status_icons[STATUS_ICON_SUBLCD_SIGNAL_STRENGTH].state;
        MMI_status_icons[STATUS_ICON_SUBLCD_SIGNAL_STRENGTH].state = 0;
    #endif /* __MMI_SUBLCD__ */ 
    }
     

     
    if (IsReArrangeNeeded)
    {
        arrange_status_icons();
        IsReArrangeNeeded = 0;
    }
     

     
    //      gui_push_clip();
    //      gui_reset_clip();
     

     
#if ((defined __MMI_MAINLCD_176X220__)|| defined(__MMI_MAINLCD_220X176__)) && (defined __MMI_UI_DALMATIAN_STATUSBAR__)
    if (background_fill == 1)
    {
        if ((MMI_status_icon_bars[0].flags & STATUS_ICON_BAR_DISPLAY)
            || (MMI_status_icon_bars[1].flags & STATUS_ICON_BAR_DISPLAY))
            if (GetActiveScreenId() != IDLE_SCREEN_ID)
            {
                show_mainlcd_status_bar_background(0, 0, UI_device_width - 1, MMI_STATUS_BAR_HEIGHT - 1);
            }
    }
#endif /* ((defined __MMI_MAINLCD_176X220__)|| defined(__MMI_MAINLCD_220X176__)) && (defined __MMI_UI_DALMATIAN_STATUSBAR__) */ 
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    for (j = initial; j >= 0 && j < MAX_STATUS_ICON_BARS; j += increment)
#else 
    for (j = 0; j < MAX_STATUS_ICON_BARS; j++)
#endif 
         
    {
        if (MMI_status_icon_bars[j].flags & STATUS_ICON_BAR_DISPLAY)
        {
            if (MMI_status_icon_bars[j].graphics_context == &main_LCD_device_bitmap)
            {
                if (status_icon_display_area == SUBLCD_ONLY)
                {
                    continue;
                }
                UI_set_main_LCD_graphics_context(); /* set graphic context to main lcd */
                //if (!g_idle_context.IsOnIdleScreen)//092206 network search
                if ((!mmi_idle_is_on_idle_screen()) || (mmi_idle_is_on_idle_screen() && (!mmi_bootup_is_idle_screen_reached())))
                {
                    /* 061506 double buffer Start */
                    if(get_status_icon_bar_layer((U8)j)!=GDI_ERROR_HANDLE)
                    {
                        gdi_layer_push_and_set_active(get_status_icon_bar_layer((U8)j));
                    }
                    /* 061506 double buffer End */
                    MMI_hide_status_icon_bars[j] ();    /* call hide function of status bar */
                    /* 061506 double buffer Start */
                    if(get_status_icon_bar_layer((U8)j)!=GDI_ERROR_HANDLE)
                    {
                        gdi_layer_pop_and_restore_active();
                    }
                    /* 061506 double buffer End */
                }
            }
            else if (MMI_status_icon_bars[j].graphics_context == &sub_LCD_device_bitmap)
            {
                if (status_icon_display_area == MAINLCD_ONLY)
                {
                    continue;
                }
                /* if ( ! g_idle_context.IsOnIdleScreen ) */
                /* 061506 double buffer Start */
                if(get_status_icon_bar_layer((U8)j)!=GDI_ERROR_HANDLE)
                {
                    gdi_layer_push_and_set_active(get_status_icon_bar_layer((U8)j));
                }
                /* 061506 double buffer End */
                MMI_hide_status_icon_bars[j] ();    /* call hide function of sub lcd status bar */
                /* 061506 double buffer Start */
                if(get_status_icon_bar_layer((U8)j)!=GDI_ERROR_HANDLE)
                {
                    gdi_layer_pop_and_restore_active();
                }
                /* 061506 double buffer End */
                UI_set_sub_LCD_graphics_context();  /* set grahic context to sub lcd */
            }

            /* 061506 double buffer Start */
            if(get_status_icon_bar_layer((U8)j)!=GDI_ERROR_HANDLE)
            {
                gdi_layer_push_and_set_active(get_status_icon_bar_layer((U8)j));
            }
            /* 061506 double buffer End */
            
             
            gui_push_clip();
            gui_reset_clip();
             

            /* store coordinates of status icon bar */
            x1 = MMI_status_icon_bars[j].x1;
            y1 = MMI_status_icon_bars[j].y1;
            x2 = MMI_status_icon_bars[j].x2;
            y2 = MMI_status_icon_bars[j].y2;

            /* set clip region */
            gui_set_clip(x1, y1, x2, y2);

             
        #if defined(__MMI_UI_TECHNO_STATUS_ICON__)
            if (j == 0)
            {
                 
                 
                if (is_on_idlescreen() == 1);   /* do nothing */
                 
                //                                      gui_fill_rectangle(x1, y1, x2, y2,
                //                                                      (current_MMI_theme->formatted_inputbox_normal_filler->border_color));
                 
                else if (GetActiveScreenId() == MAIN_MENU_SCREENID)
                {
                    gui_fill_rectangle(
                        x1,
                        y1,
                        x2,
                        y2,
                          current_UI_theme->window_title_theme->active_filler->c);
                }
                 
                else
                {
                    gui_fill_rectangle(x1, y1, x2, y2, *(current_MMI_theme->statusbar0_background_color));
                }
            }
        #endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */
		

             
             
        #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
            /* gui_draw_filled_area(x1, y1, x2, y2, MMI_status_icon_bars[j].normal_bar_filler); */
            if (j == 0)
            {
                UI_filled_area Dalmatiian_LSK_up;

                Dalmatiian_LSK_up = *current_MMI_theme->LSK_up_filler;
                Dalmatiian_LSK_up.flags |= UI_FILLED_AREA_RIGHT_ROUNDED_BORDER;
                gui_draw_filled_area(x1, y1, x2, y2, &Dalmatiian_LSK_up);
            }
            else if (j == 1)
            {
                UI_filled_area Dalmatiian_RSK_up;

                Dalmatiian_RSK_up = *current_MMI_theme->RSK_up_filler;
                Dalmatiian_RSK_up.flags |= UI_FILLED_AREA_LEFT_ROUNDED_BORDER;
                gui_draw_filled_area(x1, y1, x2, y2, &Dalmatiian_RSK_up);
            }
        #endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
             
            /* store icon offset coordinates */
            sbx = MMI_status_icon_bars[j].x;
            sby = MMI_status_icon_bars[j].y;

            if (MMI_status_icon_bars[j].icon_list != NULL)
            {
                for (i = 0; i < MMI_status_icon_bars[j].n_icons; i++)
                {
                    id = MMI_status_icon_bars[j].icon_list[i];
                    if (MMI_status_icons[id].icon_ID != IMAGE_ID_NULL)
                    {
                    #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__

                        if (mmi_tm_does_theme_have_alt_image())
                        {
                            if (mmi_tm_is_theme_dirty_bit_set(GetImageIndex(MMI_status_icons[id].icon_ID)))
                            {
                                MMI_status_icons[id].icon_data = get_image(MMI_status_icons[id].icon_ID);
                                /* No need to calculate Image width, height and frames as they will not change */
                            }
                        }
                    #endif /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 

                        if (MMI_status_icons[id].flags & STATUS_ICON_REVERSE_ANIMATE)
                        {
                            /* do nothing */
                        }
                        else if (MMI_status_icons[id].flags & STATUS_ICON_FORWARD_ANIMATE)
                        {
                            /* do nothing */
                        }
                        else if (MMI_status_icons[id].flags & STATUS_ICON_ANIMATE)
                        {
                            blink_animate_flag = 1;

                            /* store position of the icon */
                            ix1 = MMI_status_icons[id].x + sbx;
                            iy1 = MMI_status_icons[id].y + sby;

                            gui_show_transparent_animated_image_frame(
                                ix1,
                                iy1,
                                MMI_status_icons[id].icon_data,
                                0,
                                MMI_status_icons[id].current_frame);

                             
                        #if 0
                        #endif /* 0 */ 
                             
                        }
                        else if (MMI_status_icons[id].flags & STATUS_ICON_BLINK)
                        {
                            blink_animate_flag = 1;
                            if (MMI_status_icons_blink_animate_ON_state)
                            {
                                /* store position of the icon */
                                ix1 = MMI_status_icons[id].x + sbx;
                                iy1 = MMI_status_icons[id].y + sby;

                                gui_show_transparent_image(ix1, iy1, MMI_status_icons[id].icon_data, 0);
                            }
                        }
                        else if (MMI_status_icons[id].flags & STATUS_ICON_DISPLAY)
                        {
                            /* store position of the icon */
                            ix1 = MMI_status_icons[id].x + sbx;
                            iy1 = MMI_status_icons[id].y + sby;

                            if (MMI_status_icons[id].state > 0)
                            {
                                gui_show_transparent_animated_image_frame(
                                    ix1,
                                    iy1,
                                    MMI_status_icons[id].icon_data,
                                    0,
                                    MMI_status_icons[id].state);
                            }
                            else
                            {
                                gui_show_transparent_image(ix1, iy1, MMI_status_icons[id].icon_data, 0);
                            }
                        }
                    }
                }
            }
             
            gui_pop_clip();
             
            
            /* 061506 double buffer Start */
            if(get_status_icon_bar_layer((U8)j)!=GDI_ERROR_HANDLE)
            {
                gdi_layer_pop_and_restore_active();
            }
            /* 061506 double buffer End */
        }
    }

     
    if (mmi_bootup_is_in_security_check() == MMI_TRUE)
    {
        MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].state = old_signal_state_main;
    #ifdef __MMI_SUBLCD__
        MMI_status_icons[STATUS_ICON_SUBLCD_SIGNAL_STRENGTH].state = old_signal_state_sub;
    #endif 
    }
     

    status_icon_display_ON = 1;
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
     
    if (is_on_idlescreen() == 1

	#ifdef __MMI_MAINLCD_220X176__
	#ifdef __FT_IDLE_SMART_DIAL__
 	|| g_dm_data.S32CatId == MMI_CATEGORY_SmartDial_ID
	#endif
	#endif
	)
         
    {
        if ((MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].flags & STATUS_ICON_ANIMATE)
            || (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].flags & STATUS_ICON_ANIMATE))
        {
            blink_animate_flag = 1;
        }
    }
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
    if (blink_animate_flag) /* if icon is animated */
    {
        if (status_icon_blink_animate_ON == 0)
        {
            setup_status_icon_blink_animate();
        }
        status_icon_blink_animate_ON = 1;
    }
    else
    {
        if (status_icon_blink_animate_ON == 1)
        {
            setup_status_icon_blink_animate();
        }
        status_icon_blink_animate_ON = 0;
    }
     
    /* gui_pop_clip(); */
     

    /* restore active lcd according to backuped lcd */
    if (saved_graphics_context == &main_LCD_device_bitmap)
    {
        UI_set_main_LCD_graphics_context();
    }
    else if (saved_graphics_context == &sub_LCD_device_bitmap)
    {
        UI_set_sub_LCD_graphics_context();
    }

#if defined(__MMI_MAINLCD_220X176__)
if(!mmi_idle_is_on_idle_screen())
{
		draw_title();
}
#endif
}


/*****************************************************************************
 * FUNCTION
 *  hide_status_icons
 * DESCRIPTION
 *  set the flag of all icons to hide
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void hide_status_icons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    U8 flag = 1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_STATUS_ICON_BARS; i++)
    {
        if (MMI_status_icon_bars[i].flags & STATUS_ICON_BAR_DISPLAY)
        {
            if (MMI_status_icon_bars[i].graphics_context == current_LCD_device_bitmap)
            {
                /* call hide fuction of status bar */
                MMI_hide_status_icon_bars[i] ();

                /* set the flag of bar to disable */
                MMI_status_icon_bars[i].flags &= ~STATUS_ICON_BAR_DISPLAY;
            }
            else if (MMI_status_icon_bars[i].flags & STATUS_ICON_BAR_DISPLAY)
            {
                flag = 0;
            }
        }
    }
    if (flag)
    {
        status_icon_display_ON = 0;
        if (status_icon_blink_animate_ON)
        {
            status_icon_blink_animate_ON = 0;
            gui_cancel_timer(status_icon_timer_callback);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  close_status_icons
 * DESCRIPTION
 *  set statu sicon bar flag to not to display,clear status bar hide function s
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void close_status_icons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    U8 flag = 1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_220X176__)|| defined (__MMI_MAINLCD_320X240__)  
/*- zhouqin 2011-02-16 modify for q6 320X240 */
/* if ( if_full_screen_hide_status_icons()== MMI_FALSE ) */
    return;
#endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_220X176__)*/ 
    for (i = 0; i < MAX_STATUS_ICON_BARS; i++)
    {
        if (MMI_status_icon_bars[i].graphics_context == current_LCD_device_bitmap)
        {
            MMI_status_icon_bars[i].flags &= ~STATUS_ICON_BAR_DISPLAY;
        }
        else if (MMI_status_icon_bars[i].flags & STATUS_ICON_BAR_DISPLAY)
        {
            flag = 0;
        }
    }
    if (flag)
    {
        status_icon_display_ON = 0;
        if (status_icon_blink_animate_ON)
        {
            status_icon_blink_animate_ON = 0;
            gui_cancel_timer(status_icon_timer_callback);
        }
    }
    clear_hide_status_icon_bars();  /* clear function handlers of hide function */
    gui_hide_animations();          /* stop animation of icons */
     
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    gui_cancel_timer(shuffle_status_bars);
#endif 
     
}


/*****************************************************************************
 * FUNCTION
 *  status_icon_timer_callback
 * DESCRIPTION
 *  timer call back of animated icon
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void status_icon_timer_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    for (i = 0; i < MAX_STATUS_ICONS; i++)
    {
        if (MMI_status_icons[i].flags & STATUS_ICON_ANIMATE)
        {
            MMI_status_icons[i].current_frame++;
            if (MMI_status_icons[i].current_frame >= MMI_status_icons[i].n_frames)
            {
                MMI_status_icons[i].current_frame = 0;
            }
        }
    }
     

     
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)||defined(__MMI_MAINLCD_220X176__)
    if ((MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].flags & STATUS_ICON_ANIMATE)
        || (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].flags & STATUS_ICON_ANIMATE))
    {
        status_icon_blink_animate_ON = 1;
         
    #ifndef __MMI_MAINLCD_220X176__
        if (is_on_idlescreen() == 1 || GetActiveScreenId() == MAIN_MENU_SCREENID)
	#else
			if(GetActiveScreenId() !=DTMF_SCR_DISPLAY_ID)
    #endif
             
        {
            refresh_fixed_status_icon(STATUS_ICON_BATTERY_STRENGTH, 0);
        }
    }
#endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
     
    /* cancel the timaer call back of animated icon */
    if (!status_icon_blink_animate_ON)
    {
        gui_cancel_timer(status_icon_timer_callback);
    }

    if (!status_icon_display_ON)
    {
        return;
    }

    if (MMI_status_icons_blink_animate_ON_state)
    {
        MMI_status_icons_blink_animate_ON_state = 0;
    }
    else
    {
        MMI_status_icons_blink_animate_ON_state = 1;
    }

     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    disable_statusbar_fill_background();
#endif 
     
    /* redraw the status icons */
#ifndef __MMI_MAINLCD_220X176__
    refresh_status_icon_display();
#endif
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
     
    if (is_on_idlescreen() == 1)
         
    {
        enable_statusbar_fill_background();
        if (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].flags & STATUS_ICON_ANIMATE)
        {
            refresh_fixed_status_icon(STATUS_ICON_BATTERY_STRENGTH, 1);
        }
    }
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     

    if (status_icon_blink_animate_ON)
    {
        gui_start_timer(STATUS_ICON_TIMER_RATE, status_icon_timer_callback);
    }
}


/*****************************************************************************
 * FUNCTION
 *  setup_status_icon_blink_animate
 * DESCRIPTION
 *  timer call back of blinking icon
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void setup_status_icon_blink_animate(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    U8 blink_animate_flag = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#if(!defined(__MMI_UI_TECHNO_STATUS_ICON__))
    if (!status_icon_display_ON)
    {
        return;
    }
#endif /* (!defined(__MMI_UI_TECHNO_STATUS_ICON__)) */ 
     

    for (i = 0; i < MAX_STATUS_ICONS; i++)
    {
        if ((MMI_status_icons[i].flags & STATUS_ICON_BLINK) || (MMI_status_icons[i].flags & STATUS_ICON_ANIMATE))
        {
            blink_animate_flag = 1;
            break;
        }
    }

    if (blink_animate_flag)
        /* start timer for blinking icon */
    {
        gui_start_timer(STATUS_ICON_TIMER_RATE, status_icon_timer_callback);
    }
    else
        /* stop timer for blinking icon */
    {
        gui_cancel_timer(status_icon_timer_callback);
    }
}


/*****************************************************************************
 * FUNCTION
 *  show_status_icon_bar
 * DESCRIPTION
 *  set the display flag of status bar
 * PARAMETERS
 *  status_bar_ID       [IN]        0 -horizontal bar main lcd
 *  horizontal(?)       [IN]        Bar sub lcd
 *  vertical(?)         [IN]        Bar main lcd
 * RETURNS
 *  void
 *****************************************************************************/
void show_status_icon_bar(U8 status_bar_ID)
{
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (status_bar_ID == 2)
    {
        return;
    }
#endif /* __MMI_SUBLCD_SHOW_ANALOG_CLOCK__ */ 
    /* set teh flag of bar id to display */
    MMI_status_icon_bars[status_bar_ID].flags |= STATUS_ICON_BAR_DISPLAY;
}


/*****************************************************************************
 * FUNCTION
 *  hide_status_icon_bar
 * DESCRIPTION
 *  hide the status bar
 * PARAMETERS
 *  status_bar_ID       [IN]        0 -horizontal bar main lcd
 *  horizontal(?)       [IN]        Bar sub lcd
 *  vertical(?)         [IN]        Bar main lcd
 * RETURNS
 *  void
 *****************************************************************************/
void hide_status_icon_bar(U8 status_bar_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_status_icon_bars[status_bar_ID].flags &= ~STATUS_ICON_BAR_DISPLAY;
}


/*****************************************************************************
 * FUNCTION
 *  set_status_icon_bar_type
 * DESCRIPTION
 *  hide the status bar
 * PARAMETERS
 *  status_bar_ID       [IN]        0 -horizontal bar main lcd
 *  flags               [IN]        
 *  horizontal(?)       [IN]        Bar sub lcd
 *  vertical(?)         [IN]        Bar main lcd
 * RETURNS
 *  void
 *****************************************************************************/
void set_status_icon_bar_type(U8 status_bar_ID, U32 flags)
{
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (status_bar_ID == 2)
    {
        return;
    }
#endif /* __MMI_SUBLCD_SHOW_ANALOG_CLOCK__ */ 
    MMI_status_icon_bars[status_bar_ID].flags = flags;
}


/* 061306 dummy category Start */
/*****************************************************************************
 * FUNCTION
 *  get_status_icon_bar_type
 * DESCRIPTION
 *  hide the status bar
 * PARAMETERS
 *  status_bar_ID       [IN]        0 -horizontal bar main lcd
 * RETURNS
 *  
 *****************************************************************************/
U32 get_status_icon_bar_type(U8 status_bar_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_status_icon_bars[status_bar_ID].flags;
}
/* 061306 dummy category End */


/*****************************************************************************
 * FUNCTION
 *  get_status_icon_bar_clip
 * DESCRIPTION
 *  get teh clipping valeu sof particaular status bar
 * PARAMETERS
 *  status_bar_ID       [IN]        0 -horizontal bar main lcd
 *  x1                  [IN]        Start x position
 *  y1                  [IN]        Start y position
 *  x2                  [IN]        End x position
 *  y2                  [IN]        End y position
 *  horizontal(?)       [IN]        Bar sub lcd
 *  vertical(?)         [IN]        Bar main lcd
 * RETURNS
 *  void
 *****************************************************************************/
void get_status_icon_bar_clip(U8 status_bar_ID, S32 *x1, S32 *y1, S32 *x2, S32 *y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *x1 = MMI_status_icon_bars[status_bar_ID].x1;
    *y1 = MMI_status_icon_bars[status_bar_ID].y1;
    *x2 = MMI_status_icon_bars[status_bar_ID].x2;
    *y2 = MMI_status_icon_bars[status_bar_ID].y2;
}


/*****************************************************************************
 * FUNCTION
 *  set_status_icon_bar_clip
 * DESCRIPTION
 *  set teh clipping valeu sof particaular status bar
 * PARAMETERS
 *  status_bar_ID       [IN]        0 -horizontal bar main lcd
 *  x1                  [IN]        Start x position
 *  y1                  [IN]        Start y position
 *  x2                  [IN]        End x position
 *  y2                  [IN]        End y position
 *  horizontal(?)       [IN]        Bar sub lcd
 *  vertical(?)         [IN]        Bar main lcd
 * RETURNS
 *  void
 *****************************************************************************/
void set_status_icon_bar_clip(U8 status_bar_ID, S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_status_icon_bars[status_bar_ID].x1 = x1;
    MMI_status_icon_bars[status_bar_ID].y1 = y1;
    MMI_status_icon_bars[status_bar_ID].x2 = x2;
    MMI_status_icon_bars[status_bar_ID].y2 = y2;
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    MMI_status_icon_bars[status_bar_ID].x = x1;
    MMI_status_icon_bars[status_bar_ID].y = y1;
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
}

/* 061506 double buffer Start */
/*****************************************************************************
 * FUNCTION
 *  set_status_icon_bar_layer
 * DESCRIPTION
 *  set gdi layer handle
 * PARAMETERS
 *  status_bar_ID       [IN]        0 -horizontal bar main lcd
 *  gdi_layer               [IN]        
 *  horizontal(?)       [IN]        Bar sub lcd
 *  vertical(?)         [IN]        Bar main lcd
 * RETURNS
 *  void
 *****************************************************************************/
void set_status_icon_bar_layer(U8 status_bar_ID, GDI_HANDLE gdi_layer)
{
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (status_bar_ID == 2)
    {
        return;
    }
#endif /* __MMI_SUBLCD_SHOW_ANALOG_CLOCK__ */ 
    MMI_status_icon_bars[status_bar_ID].layer_handle= gdi_layer;
}


/*****************************************************************************
 * FUNCTION
 *  get_status_icon_bar_layer
 * DESCRIPTION
 *  get gdi layer handle
 * PARAMETERS
 *  status_bar_ID       [IN]        0 -horizontal bar main lcd
 * RETURNS
 *  
 *****************************************************************************/
U32 get_status_icon_bar_layer(U8 status_bar_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_status_icon_bars[status_bar_ID].layer_handle;
}
/* 061506 double buffer End */


/*****************************************************************************
 * FUNCTION
 *  reset_status_icons
 * DESCRIPTION
 *  Function to be used by wgui_categories.
 *  This must be called in the exit function of the category
 *  screens that have displayed status icons and / or registered
 *  functions for hiding them.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_status_icons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    hide_status_icons();
    clear_hide_status_icon_bars();
}


/*****************************************************************************
 * FUNCTION
 *  initialize_status_icons
 * DESCRIPTION
 *  Called once during bootup to initialize Status icons
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
//Moved up
//extern S32 MAIN_LCD_device_width;
//extern S32 MAIN_LCD_device_height;

#ifdef __MMI_SUBLCD__
extern S32 SUB_LCD_device_width;
extern S32 SUB_LCD_device_height;
#endif /* __MMI_SUBLCD__ */ 

void initialize_status_icons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
    MMI_status_icon_bars[0].x = 0;
    MMI_status_icon_bars[0].y = 0;
    MMI_status_icon_bars[0].x1 = 0;
    MMI_status_icon_bars[0].y1 = 0;
    MMI_status_icon_bars[0].x2 = MAIN_LCD_device_width - 1;
    MMI_status_icon_bars[0].y2 = MMI_status_bar_height - 1;

    /* set the cliping values of vertical status bar of main lcd */
    MMI_status_icon_bars[1].x = 0;
    MMI_status_icon_bars[1].y = 0;
    MMI_status_icon_bars[1].x1 = 0;
    MMI_status_icon_bars[1].y1 = 0;
    MMI_status_icon_bars[1].x2 = 0;
    MMI_status_icon_bars[1].y2 = 0;
     
#elif defined( __MMI_UI_DALMATIAN_STATUSBAR__)
    MMI_status_icon_bars[0].x = 0;
    MMI_status_icon_bars[0].y = MMI_TITLE_HEIGHT + STATUS_TITLE_GAP;
    MMI_status_icon_bars[0].x1 = 0;
    MMI_status_icon_bars[0].y1 = MMI_TITLE_HEIGHT + STATUS_TITLE_GAP;
    MMI_status_icon_bars[0].x2 = MMI_STATUS_BAR_WIDTH - 1;
    MMI_status_icon_bars[0].y2 = MMI_status_icon_bars[0].y1 + MMI_SINGLE_STATUS_BAR_HEIGHT - 1;

    MMI_status_icon_bars[1].x = MAIN_LCD_device_width - MMI_STATUS_BAR_WIDTH;
    MMI_status_icon_bars[1].y = MMI_TITLE_HEIGHT + MMI_STATUS_BAR_HEIGHT
        - MMI_SINGLE_STATUS_BAR_HEIGHT + STATUS_TITLE_GAP;
    MMI_status_icon_bars[1].x1 = MAIN_LCD_device_width - MMI_STATUS_BAR_WIDTH;
    MMI_status_icon_bars[1].y1 = MMI_TITLE_HEIGHT + MMI_STATUS_BAR_HEIGHT
        - MMI_SINGLE_STATUS_BAR_HEIGHT + STATUS_TITLE_GAP;
    MMI_status_icon_bars[1].x2 = MAIN_LCD_device_width - 1;
    MMI_status_icon_bars[1].y2 = MMI_status_icon_bars[1].y1 + MMI_STATUS_BAR_HEIGHT - 1;

     
#else /* set the cliping values of horizontal status bar of main lcd */
    MMI_status_icon_bars[0].x = 0;
    MMI_status_icon_bars[0].y = 0;
    MMI_status_icon_bars[0].x1 = 0;
    MMI_status_icon_bars[0].y1 = 0;
    MMI_status_icon_bars[0].x2 = MAIN_LCD_device_width - 1;
    MMI_status_icon_bars[0].y2 = MMI_status_bar_height - 1;

    /* set the cliping values of vertical status bar of main lcd */
#if !defined(__MMI_NOKIA_STYLE_N800__)	
    MMI_status_icon_bars[1].x = MAIN_LCD_device_width - 17;
    MMI_status_icon_bars[1].y = MMI_status_bar_height;
    MMI_status_icon_bars[1].x1 = MAIN_LCD_device_width - 17;
    MMI_status_icon_bars[1].y1 = MMI_status_bar_height;
    MMI_status_icon_bars[1].x2 = MAIN_LCD_device_width - 1;
     
#endif
#ifdef __MMI_KLG__
    MMI_status_icon_bars[1].y2 = MAIN_LCD_device_height >> 1;
#else 
#if !defined(__MMI_NOKIA_STYLE_N800__)
	MMI_status_icon_bars[1].y2 = MAIN_LCD_device_height - MMI_button_bar_height - 1;
#endif
#endif 
     

#endif 
     

    /* set the hide functions of statuas bar to dummy function  */
    for (i = 0; i < MAX_STATUS_ICON_BARS; i++)
    {
        MMI_hide_status_icon_bars[i] = UI_dummy_function;
         
    #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
        /* MMI_status_icon_bars[i].normal_bar_filler = NULL; */
    #endif 
         
    }

    for (i = 0; i < MAX_STATUS_ICONS; i++)
    {
        MMI_status_icons[i].icon_data = get_image(MMI_status_icons[i].icon_ID);
        MMI_status_icons[i].n_frames = (S16) gui_image_n_frames(MMI_status_icons[i].icon_data);
        gui_measure_image(MMI_status_icons[i].icon_data, &MMI_status_icons[i].width, &MMI_status_icons[i].height);
    }
     
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
    MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].x = (MAIN_LCD_device_width - 1)
        - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width - MMI_status_icon_bars[1].x;
#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 

    /* set the start x position of battery icon of horizontal man lcd status bar */
    MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].x =
        MAIN_LCD_device_width - 1 - MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width;

#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
     
#ifdef __MMI_SUBLCD__
    /* set the cliping values of horizontal status bar of sub lcd */
    MMI_status_icon_bars[2].x = 0;
    MMI_status_icon_bars[2].y = 0;
    MMI_status_icon_bars[2].x1 = 0;
    MMI_status_icon_bars[2].y1 = 0;
    MMI_status_icon_bars[2].x2 = SUB_LCD_device_width - 1;
    MMI_status_icon_bars[2].y2 = 14;
    MMI_status_icons[STATUS_ICON_SUBLCD_SIGNAL_STRENGTH].x = 1;
    MMI_status_icons[STATUS_ICON_SUBLCD_BATTERY_STRENGTH].x =
        SUB_LCD_device_width - 1 - MMI_status_icons[STATUS_ICON_SUBLCD_BATTERY_STRENGTH].width;
#endif /* __MMI_SUBLCD__ */ 
}

 
#if 0
#endif /* 0 */ 
 


/*****************************************************************************
 * FUNCTION
 *  ShowStatusIcon
 * DESCRIPTION
 *  show particular icon
 * PARAMETERS
 *  icon_ID     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __FLIGHT_MODE_SUPPORT__
	extern pBOOL mmi_bootup_get_active_flight_mode();
	extern pBOOL mmi_bootup_is_sim_valid();
#endif
#ifdef __MMI_REMIND_LED__
extern VOID show_led_flash_start(U16 index);
extern VOID stop_led_flash(U16 index);
#endif
void ShowStatusIcon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    /* 2005/10/20 Hide status icon in flight mode */
#ifdef __FLIGHT_MODE_SUPPORT__
    if (mmi_bootup_get_active_flight_mode() == 1 || mmi_bootup_is_sim_valid() == MMI_FALSE)
    {
        pBOOL rtn = PhnsetFlightModeHideButSaveStatusIcon(icon_ID, MMI_status_icons[icon_ID].flags | STATUS_ICON_DISPLAY);
        if (rtn == MMI_TRUE && (gPowerOnWithFlightMode == MMI_TRUE || mmi_bootup_is_sim_valid() == MMI_FALSE))
        {
            return;
        }
    }
#else /* __FLIGHT_MODE_SUPPORT__ */ 
    if (mmi_bootup_is_sim_valid() == MMI_FALSE)
    {
        if (PhnsetFlightModeHideButSaveStatusIcon(icon_ID, MMI_status_icons[icon_ID].flags | STATUS_ICON_DISPLAY) ==
            MMI_TRUE)
        {
            return;
        }
    }
#endif /* __FLIGHT_MODE_SUPPORT__ */ 

    /* set the flag of icon to display on */
    MMI_status_icons[icon_ID].flags |= STATUS_ICON_DISPLAY;

    /* set rearrange flag on */
    IsReArrangeNeeded = 1;
#ifdef __MMI_REMIND_LED__
if(STATUS_ICON_MISSED_CALL <= icon_ID && STATUS_ICON_CARD4MISSED_CALL >= icon_ID)
{
	show_led_flash_start(0x0001);
}
else if(
	#ifdef __MMI_MULTI_SIM__
	STATUS_ICON_INCOMING_SMS == icon_ID
	#else
	(STATUS_ICON_INCOMING_SMS <= icon_ID)
	   &&(STATUS_ICON_SIM4_INCOMING_SMS >= icon_ID)
	#endif 
	)
{
	show_led_flash_start(0x0010);
}
#endif
}


/*****************************************************************************
 * FUNCTION
 *  HideStatusIcon
 * DESCRIPTION
 *  hide particular icon
 * PARAMETERS
 *  icon_ID     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __FLIGHT_MODE_SUPPORT__
	extern pBOOL mmi_bootup_get_active_flight_mode();
#endif
void HideStatusIcon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    /* set teh flag of icon to hide ...defualt value */
    MMI_status_icons[icon_ID].flags = 0;

    /* set rearrange flag on */
    IsReArrangeNeeded = 1;

    /* 2005/10/20 Hide status icon in flight mode */
#ifdef __FLIGHT_MODE_SUPPORT__
    if (mmi_bootup_get_active_flight_mode() == 1)
    {
        PhnsetFlightModeHideButSaveStatusIcon(icon_ID, 0);
    }
#endif /* __FLIGHT_MODE_SUPPORT__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  UpdateStatusIcons
 * DESCRIPTION
 *  rearrange and refresh status icons
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateStatusIcons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (IsReArrangeNeeded)
    {
        arrange_status_icons();
        IsReArrangeNeeded = 0;
    }
	
	
#if defined( __MMI_MAINLCD_220X176__)//||defined(__MMI_NOKIA_STYLE_N800__)
	if(is_on_idlescreen()==1)
#endif	
	{
	    refresh_status_icon_display();
	}

}


/*****************************************************************************
 * FUNCTION
 *  UpdateSubLCDStatusIcons
 * DESCRIPTION
 *  rearrange and refresh SubLCD status icons
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateSubLCDStatusIcons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_status_icon_display(SUBLCD_ONLY);
    UpdateStatusIcons();
    reset_status_icon_display();
}


/*****************************************************************************
 * FUNCTION
 *  ChangeStatusIconLevel
 * DESCRIPTION
 *  change the level of status icon like battery icon ,signal icon
 * PARAMETERS
 *  icon_ID                 [IN]        
 *  level_percentage        [IN]        Level of increase
 * RETURNS
 *  void
 *****************************************************************************/
extern pBOOL mmi_bootup_is_in_security_check();
void ChangeStatusIconLevel(S16 icon_ID, U8 level_percentage)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 n_frames = MMI_status_icons[icon_ID].n_frames;
    S32 frame_number;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_frames <= 1)
    {
        return;
    }

    frame_number = (level_percentage * (n_frames)) / 100;

    /* check frame number not bigger than frames of icons */
    if (frame_number > (n_frames - 1))
    {
        frame_number = n_frames - 1;
    }

    /* check frame number more than 0 */
    if (frame_number < 0)
    {
        frame_number = 0;
    }

    /* if level is the same, no need to refresh the icon */
    if (frame_number == MMI_status_icons[icon_ID].state)
    {
        return;
    }

#if 0
    if (mmi_bootup_is_in_security_check() == MMI_TRUE)
    {
         
        /* status_icon_display_locked=1; */
        if ((icon_ID == STATUS_ICON_BATTERY_STRENGTH) || (icon_ID == STATUS_ICON_SUBLCD_BATTERY_STRENGTH))
        {
            set_status_icon_state(icon_ID, (U8) frame_number);
        }
         
    }
    else
#endif        
    {
        //set the state of icon when MMI in not in power on procedure.
         
        set_status_icon_state(icon_ID, (U8) frame_number);
    }
     
    if (mmi_bootup_is_in_security_check() == MMI_FALSE)
    {
         
    #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
        if (icon_ID == STATUS_ICON_SIGNAL_STRENGTH)
        {
            refresh_fixed_status_icon(icon_ID, 0);
        }
        else if (icon_ID == STATUS_ICON_BATTERY_STRENGTH)
        {
            refresh_fixed_status_icon(icon_ID, 1);
        }
    #else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
	if ((icon_ID >=STATUS_ICON_SIGNAL_STRENGTH) && (icon_ID <= STATUS_ICON_BATTERY_STRENGTH))
	{
		refresh_fixed_status_icon(icon_ID, 0);
	}    
    #endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
         
        else if ((icon_ID == STATUS_ICON_SUBLCD_SIGNAL_STRENGTH) || (icon_ID == STATUS_ICON_SUBLCD_BATTERY_STRENGTH))
        {
            refresh_fixed_status_icon(icon_ID, 2);
        }
        else
            /* Before entry idlescreen, only signal and battery should be udpated. */
    #ifndef __MMI_SHOW_ALL_STATUS_ICONS_B4_IDLE___
        if (mmi_bootup_is_in_security_check() == MMI_FALSE)
    #endif 
            refresh_status_icon_display();
    }
}

 

/*****************************************************************************
 * FUNCTION
 *  ChangeStatusIconImage
 * DESCRIPTION
 *  change the image of status icon like signal icon with line switch
 * PARAMETERS
 *  icon_ID         [IN]        
 *  image_ID        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeStatusIconImage(S16 icon_ID, U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* HideStatusIcon(icon_ID); */
    MMI_status_icons[icon_ID].icon_data = get_image(image_ID);
    MMI_status_icons[icon_ID].n_frames = (S16) gui_image_n_frames(MMI_status_icons[icon_ID].icon_data);
    gui_measure_image(
        MMI_status_icons[icon_ID].icon_data,
        &MMI_status_icons[icon_ID].width,
        &MMI_status_icons[icon_ID].height);
    MMI_status_icons[icon_ID].icon_ID = image_ID;
    /* ShowStatusIcon(icon_ID); */

    /* set rearrange flag on */
    IsReArrangeNeeded = 1;

    /* update status icon */
     
}

 
 
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)


/*****************************************************************************
 * FUNCTION
 *  ChangeStatusIconImageWithoutReArrange
 * DESCRIPTION
 *  
 * PARAMETERS
 *  icon_ID         [IN]        
 *  image_ID        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ChangeStatusIconImageWithoutReArrange(S16 icon_ID, U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_status_icons[icon_ID].icon_data = get_image(image_ID);
    MMI_status_icons[icon_ID].n_frames = (S16) gui_image_n_frames(MMI_status_icons[icon_ID].icon_data);
    gui_measure_image(
        MMI_status_icons[icon_ID].icon_data,
        &MMI_status_icons[icon_ID].width,
        &MMI_status_icons[icon_ID].height);
    MMI_status_icons[icon_ID].icon_ID = image_ID;
}
#endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
 


/*****************************************************************************
 * FUNCTION
 *  AnimateStatusIcon
 * DESCRIPTION
 *  animate particular icon
 * PARAMETERS
 *  icon_ID     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void AnimateStatusIcon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowStatusIcon(icon_ID);

    /* animate the icon */
    animate_status_icon(icon_ID);
}


/*****************************************************************************
 * FUNCTION
 *  BlinkStatusIcon
 * DESCRIPTION
 *  blink particular icon
 * PARAMETERS
 *  icon_ID     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void BlinkStatusIcon(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowStatusIcon(icon_ID);

    /* blink the icon */
    blink_status_icon(icon_ID);
}


/*****************************************************************************
 * FUNCTION
 *  arrange_status_icons_sublcd
 * DESCRIPTION
 *  arrange status icon of sub lcd
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void arrange_status_icons_sublcd(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j, k = 0, group_ID;
    U8 done = 0, inner_done, display_flag;

#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
    S32 cy;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
    return;
#endif 
    GDI_LOCK;
    MMI_status_icon_bars[2].n_icons = 0;
    MMI_status_icons_list3[0] = STATUS_ICON_SUBLCD_SIGNAL_STRENGTH;
    MMI_status_icons_list3[1] = STATUS_ICON_SUBLCD_BATTERY_STRENGTH;

#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
    cy = MMI_status_icon_bars[2].y2 - MMI_status_icon_bars[2].y1 - 1;
#endif 

    done = 0;
    i = 0;
    while (!done)
    {
        display_flag = 0;
        j = MMI_status_icons_pool2[i];
        i++;
        if (i >= MMI_N_STATUS_ICONS_POOL2)
        {
            break;
        }
        if (TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
        {
            k = j;
            display_flag = 1;
        }
        if ((MMI_status_icons[j].group_ID > 0))
        {
            group_ID = MMI_status_icons[j].group_ID;
            j++;
            inner_done = 0;
            while (!inner_done)
            {
                if (group_ID != MMI_status_icons[j].group_ID)
                {
                    break;
                }
                if ((!display_flag) && TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
                {
                    display_flag = 1;
                    k = j;
                }
                else
                {
                    i++;
                    j++;
                }
            }
        }

        if (display_flag)
        {
            MMI_status_icons_list3[MMI_status_icon_bars[2].n_icons] = (S16) k;
            MMI_status_icon_bars[2].n_icons++;
        #if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
            MMI_status_icons[k].y = cy - MMI_status_icons[k].height;
        #endif 
        }
    }
    GDI_UNLOCK;
}


/*****************************************************************************
 * FUNCTION
 *  arrange_status_icons1
 * DESCRIPTION
 *  arrange status icon horizontal and vertical staus bar of main lcd
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void arrange_status_icons1(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, x, y, cy;
    S32 i, j, k = 0, group_ID;
    U8 done = 0, inner_done, display_flag;
    S32 old_i;   

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Status bar 1: Horizontal bar  */

    MMI_status_icons_list1[0] = STATUS_ICON_SIGNAL_STRENGTH;
    MMI_status_icons_list1[1] = STATUS_ICON_BATTERY_STRENGTH;
    MMI_status_icon_bars[0].n_icons = 2;

    x1 = MMI_status_icon_bars[0].x1;    /* store start x position of horizontal stats bar of main lcd */
    y1 = MMI_status_icon_bars[0].y1;    /* store start y position of horizontal stats bar of main lcd */
    x2 = MMI_status_icon_bars[0].x2;    /* store end x position of horizontal stats bar of main lcd */
    y2 = MMI_status_icon_bars[0].y2;    /* store end y position of horizontal stats bar of main lcd */

    /* Shift x1,x2 assuming signal strength and battery strength icons are at either ends  */
    x1 += MMI_status_icons[MMI_status_icons_list1[0]].width - 1 + STATUS_ICON_X_GAP;
    x2 = MMI_status_icons[MMI_status_icons_list1[1]].x - STATUS_ICON_X_GAP;
#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
    cy = y2 - y1 - 1;
#else 
    cy = (y2 - y1 + 1) >> 1;
#endif 

#if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
    MMI_status_icons[0].y = cy - MMI_status_icons[0].height;
    MMI_status_icons[1].y = cy - MMI_status_icons[1].height;
#else /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 
    MMI_status_icons[0].y = cy - (MMI_status_icons[0].height >> 1);
    MMI_status_icons[1].y = cy - (MMI_status_icons[1].height >> 1);
#endif /* (STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE) */ 

    i = 2;
    x = x1 + STATUS_ICON_X_GAP;
    while (!done)
    {
        display_flag = 0;
        j = MMI_status_icons_pool1[i];
        old_i = i;   
        i++;
        if (i > MMI_N_STATUS_ICONS_POOL1)
        {
            break;
        }
        if (TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
        {
            k = j;
            display_flag = 1;
        }
        if ((MMI_status_icons[j].group_ID > 0))
        {
            group_ID = MMI_status_icons[j].group_ID;
            j++;
            inner_done = 0;
            while (!inner_done)
            {
                if (group_ID != MMI_status_icons[j].group_ID)
                {
                    break;
                }
                if ((!display_flag) && TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[j].flags))
                {
                    display_flag = 1;
                    k = j;
                }
                else
                {
                    i++;
                    j++;
                }
            }
        }
        if (display_flag)
        {
            if ((x + MMI_status_icons[k].width + STATUS_ICON_X_GAP) > x2)
            {               /* i--; */
                i = old_i;   
                break;
            }
        #if(STATUS_ICONS_DRAW_FROM_BOTTOM_BASELINE)
            y = cy - MMI_status_icons[k].height;
        #else 
            y = cy - (MMI_status_icons[k].height >> 1);
        #endif 
            MMI_status_icons[k].x = x;  /* store x position of ion */
            MMI_status_icons[k].y = y;  /* stor y position of icon */
            x += MMI_status_icons[k].width + STATUS_ICON_X_GAP;
            MMI_status_icons_list1[MMI_status_icon_bars[0].n_icons] = (S16) k;
            MMI_status_icon_bars[0].n_icons++;
        }
    }

}


/*****************************************************************************
 * FUNCTION
 *  show_status_icon1
 * DESCRIPTION
 *  set teh flag of icon to show
 * PARAMETERS
 *  icon_ID     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void show_status_icon1(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    MMI_status_icons[icon_ID].flags |= STATUS_ICON_DISPLAY;
    arrange_status_icons1();
}

/* CSD added by JL 040302 for sublcd background display. */


/*****************************************************************************
 * FUNCTION
 *  show_sublcd_status_background
 * DESCRIPTION
 *  Displays the SUBLCD background screen
 * PARAMETERS
 *  start_x             [IN]        Left top position
 *  start_y             [IN]        Left top position
 *  end_x               [IN]        Right bottom position
 *  end_y               [IN]        Right bottom position
 *  is_status_bar       [IN]        Is status bar to draw or not.
 * RETURNS
 *  void
 *****************************************************************************/
extern S32 Sub_lcd_title_height;
void show_sublcd_status_background(S32 start_x, S32 start_y, S32 end_x, S32 end_y, U8 is_status_bar)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* CSD added by JL to draw all screen of background display. */
#if defined(__MMI_SUBLCD__)
    GDI_LOCK;
#if (defined (__MMI_SUBLCD_COLOR__) || defined (__MMI_FULL_SUBLCD_COLOR__))

    if (!is_status_bar)
    {
    #ifdef __MMI_KLG__
        UI_filled_area f = *current_MMI_theme->list_selected_item_filler;

        f.c.r = f.c.g = f.c.b = 0;
    #else /* __MMI_KLG__ */ 
        UI_filled_area f = *current_MMI_theme->list_selected_item_filler;

        f.c.alpha = SUBLCD_ALPHA_VALUE;
        f.c.r = ((f.c.r * f.c.alpha) + (255 * (100 - f.c.alpha))) / 100;
        f.c.g = ((f.c.g * f.c.alpha) + (255 * (100 - f.c.alpha))) / 100;
        f.c.b = ((f.c.b * f.c.alpha) + (255 * (100 - f.c.alpha))) / 100;
    #endif /* __MMI_KLG__ */ 
    #if defined(__MMI_SUBLCD_COLOR__)
        start_y = (U16) Sub_lcd_title_height;
    #endif 
        gui_push_clip();
        gui_set_clip(start_x, start_y, end_x, end_y);

         
        /* gui_draw_filled_area(start_x, start_y, end_x, end_y, &f); */
        gui_fill_rectangle(start_x, start_y, end_x, end_y, f.c);
        gui_pop_clip();
    }
    else
    {
        /* No process for status basr */
    #ifdef __MMI_KLG__
        color_t c = gui_color(0, 0, 0);
    #else 
        color_t c = gui_color(255, 255, 255);
    #endif 
        gui_push_clip();
        gui_set_clip(start_x, start_y, end_x, end_y);
        gui_fill_rectangle(start_x, start_y, end_x, end_y, c);
        gui_pop_clip();
    }
#else /* (defined (__MMI_SUBLCD_COLOR__) || defined (__MMI_FULL_SUBLCD_COLOR__)) */ 
    {
        color_t c = gui_color(255, 255, 255);

        gui_push_clip();
        gui_set_clip(start_x, start_y, end_x, end_y);
        gui_fill_rectangle(start_x, start_y, end_x, end_y, c);
        gui_pop_clip();
    }
#endif /* (defined (__MMI_SUBLCD_COLOR__) || defined (__MMI_FULL_SUBLCD_COLOR__)) */ 
    GDI_UNLOCK;
#endif /* defined(__MMI_SUBLCD__) */ 

}


/*****************************************************************************
 * FUNCTION
 *  show_mainlcd_status_bar_background
 * DESCRIPTION
 *  
 * PARAMETERS
 *  start_x     [IN]        
 *  start_y     [IN]        
 *  end_x       [IN]        
 *  end_y       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void show_mainlcd_status_bar_background(U16 start_x, U16 start_y, U16 end_x, U16 end_y)
/* Only for status icon bar 0 */
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c = {255, 255, 255, 100};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gui_set_clip_preset(start_x, start_y, end_x, end_y);
    gui_fill_rectangle(start_x, start_y, end_x, end_y, c);
    gdi_layer_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  set_status_icons_display
 * DESCRIPTION
 *  
 * PARAMETERS
 *  state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_status_icons_display(MMI_BOOL state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (state)
    {
        U8 blink_animate_flag = 0;
        S32 i;

        status_icon_display_ON = 1;

        for (i = 0; i < MAX_STATUS_ICONS; i++)
        {
            if ((MMI_status_icons[i].flags & STATUS_ICON_BLINK) || (MMI_status_icons[i].flags & STATUS_ICON_ANIMATE))
            {
                blink_animate_flag = 1;
                break;
            }
        }

        if (blink_animate_flag == 1)
        {
            gui_start_timer(STATUS_ICON_TIMER_RATE, status_icon_timer_callback);        /* start timer for blinking icon */
        }
    }
    else
    {
        status_icon_display_ON = 0;
        gui_cancel_timer(status_icon_timer_callback);
    }
}

 
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__


/*****************************************************************************
 * FUNCTION
 *  shuffle_status_bars
 * DESCRIPTION
 *  sets the status bar filler to currently selected theme.
 * PARAMETERS
 *  void
 *  pointer(?)      [IN]        To a UI_status_bar_theme
 * RETURNS
 *  void
 *****************************************************************************/
/*
 * void gui_set_status_bar_theme(MMI_status_icon_bar *b, UI_status_bar_theme *t)
 * {
 * b->normal_bar_filler = t->normal_bar_filler;
 * }
 */
/* function for shuffling the status bars on idle screen. */
void shuffle_status_bars(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (is_status_bar_shuffled == 0)
    {
        is_status_bar_shuffled = 1;
    }
    else
    {
        is_status_bar_shuffled = 0;
    }
    swap_status_bar_positions(&MMI_status_icon_bars[0].y, &MMI_status_icon_bars[1].y);
    swap_status_bar_positions(&MMI_status_icon_bars[0].y1, &MMI_status_icon_bars[1].y1);
    swap_status_bar_positions(&MMI_status_icon_bars[0].y2, &MMI_status_icon_bars[1].y2);
    IsReArrangeNeeded = 1;
    gui_lock_double_buffer();
    gui_push_clip();
    gui_set_clip(
        0,
        MMI_title_height + STATUS_TITLE_GAP,
        UI_device_width - 1,
        MMI_title_height + STATUS_TITLE_GAP + MMI_status_bar_height - 1);
    draw_wallpaper();
    UpdateStatusIcons();
    gui_pop_clip();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(
        0,
        MMI_title_height + STATUS_TITLE_GAP,
        UI_device_width - 1,
        MMI_title_height + STATUS_TITLE_GAP + MMI_status_bar_height);
     
    //gui_start_timer(STATUS_BAR_SHUFFLE_TIMER, shuffle_status_bars);
     
}


/*****************************************************************************
 * FUNCTION
 *  swap_status_bar_positions
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x       [?]     
 *  y       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void swap_status_bar_positions(S32 *x, S32 *y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 temp;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    temp = *x;
    *x = *y;
    *y = temp;
}


/*****************************************************************************
 * FUNCTION
 *  enable_statusbar_fill_background
 * DESCRIPTION
 *  enables the flag for showing the background of status bar.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void enable_statusbar_fill_background(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    if (is_on_idlescreen() == 1)
         
    {
        background_fill = 1;
    }
}


/*****************************************************************************
 * FUNCTION
 *  disable_statusbar_fill_background
 * DESCRIPTION
 *  disables the flag for showing the background of status bar.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void disable_statusbar_fill_background(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    if (is_on_idlescreen() == 1)
         
    {
        background_fill = 0;
    }
}
#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
 

 
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)


/*****************************************************************************
 * FUNCTION
 *  NumberOfDisplayedIcons
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 NumberOfDisplayedIcons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    S32 icondisplayed = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 2; i < MAX_STATUS_ICONS - 10; i++)
    {
        if (TEST_STATUS_ICON_DISPLAY_ON(MMI_status_icons[i].flags))
        {
            icondisplayed++;
        }
    }
    return (icondisplayed);
}
#endif /* defined(__MMI_UI_TECHNO_STATUS_ICON__) */ 
 

#ifdef __MMI_TOUCH_SCREEN__
MMI_BOOL gIsHandleStatusIconPenEvent = MMI_FALSE;


/*****************************************************************************
 * FUNCTION
 *  wgui_register_status_icon_pen_event_hdlr
 * DESCRIPTION
 *  register pen event handlers for status icons.
 * PARAMETERS
 *  icon_id         [IN]        Id of icon
 *  event_type      [IN]        
 *  f               [IN]        Event handler
 * RETURNS
 *  BOOL        if any status icon is touched(?)
 *****************************************************************************/
void wgui_register_status_icon_pen_event_hdlr(S16 icon_id, wgui_status_icon_bar_pen_enum event_type, FuncPtr f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_status_icon_pen_event_hdlr[icon_id][event_type] = f;
    gIsHandleStatusIconPenEvent = MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_execute_status_icon_pen_event_hdlr
 * DESCRIPTION
 *  execute pen event handlers for status icons.
 * PARAMETERS
 *  icon_id         [IN]        Id of icon
 *  event_type      [IN]        
 * RETURNS
 *  BOOL        if event handler exists
 *****************************************************************************/
MMI_BOOL wgui_execute_status_icon_pen_event_hdlr(S16 icon_id, wgui_status_icon_bar_pen_enum event_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_status_icon_pen_event_hdlr[icon_id][event_type] != NULL)
    {
        MMI_status_icon_pen_event_hdlr[icon_id][event_type] ();
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_reset_status_icon_pen_event_hdlr
 * DESCRIPTION
 *  Reset pen event handlers for status icons.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_reset_status_icon_pen_event_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 icon_id = 0, event_type = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (event_type = 0; event_type < WGUI_STATUS_ICON_BAR_PEN_EVENT_MAX; event_type++)
        for (icon_id = 0; icon_id < MAX_STATUS_ICONS; icon_id++)
        {
            MMI_status_icon_pen_event_hdlr[icon_id][event_type] = NULL;
        }

    gIsHandleStatusIconPenEvent = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_status_icon_translate_pen_position
 * DESCRIPTION
 *  Translate pen position for status icons.
 * PARAMETERS
 *  x       [IN]        Pen x-coordinate
 *  y       [IN]        Pen y-coordinate
 * RETURNS
 *  S16         icon_index
 *****************************************************************************/
S16 wgui_status_icon_translate_pen_position(S16 x, S16 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;
    S16 x1, y1, x2, y2;
    S16 icon_idx = -1, bar_idx = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (bar_idx = 0; bar_idx < MAX_STATUS_ICON_BARS; bar_idx++)
    {
        if (MMI_status_icon_bars[bar_idx].flags & STATUS_ICON_BAR_DISPLAY &&
            MMI_status_icon_bars[bar_idx].icon_list != NULL)
        {
            x1 = MMI_status_icon_bars[bar_idx].x1;
            y1 = MMI_status_icon_bars[bar_idx].y1;
            x2 = MMI_status_icon_bars[bar_idx].x2;
            y2 = MMI_status_icon_bars[bar_idx].y2;
            if (x >= x1 && x <= x2 && y >= y1 && y <= y2)   /* inside status icon bar */
            {
                for (i = 0; i < MMI_status_icon_bars[bar_idx].n_icons; i++)
                {
                    icon_idx = MMI_status_icon_bars[bar_idx].icon_list[i];
                    if (MMI_status_icons[icon_idx].icon_ID != IMAGE_ID_NULL &&
                        MMI_status_icons[icon_idx].flags & STATUS_ICON_DISPLAY)
                    {
                        x1 = MMI_status_icons[icon_idx].x;
                        x2 = x1 + MMI_status_icons[icon_idx].width - 1;
                        y1 = MMI_status_icons[icon_idx].y;
                        y2 = y1 + MMI_status_icons[icon_idx].height - 1;
                        if (x >= x1 && x <= x2 && y >= y1 && y <= y2)   /* pen on status icon */
                        {
                            return icon_idx;
                        }
                    }
                }
            }
        }
    }
    return -1;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_status_icon_translate_pen_event
 * DESCRIPTION
 *  Translate pen events for status icons.
 * PARAMETERS
 *  pen_event       [IN]            Type of pen event
 *  x               [IN]            Pen x-coordinate
 *  y               [IN]            Pen y-coordinate
 *  icon_id         [IN/OUT]        Id of icon be touched
 *  event_type      [?]             
 * RETURNS
 *  BOOL        if any status icon is touched
 *****************************************************************************/
BOOL wgui_status_icon_translate_pen_event(
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        S16 *icon_id,
        wgui_status_icon_bar_pen_enum *event_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_wait_longpress_enum w;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *event_type = WGUI_STATUS_ICON_BAR_PEN_NONE;
    *icon_id = -1;

    if (gIsHandleStatusIconPenEvent == MMI_FALSE)
    {
        return FALSE;
    }

    if (pen_down_status_icon_id < 0 && pen_event != MMI_PEN_EVENT_DOWN)
    {
        return FALSE;
    }

    if (pen_event == MMI_PEN_EVENT_DOWN)
    {
        pen_down_status_icon_id = wgui_status_icon_translate_pen_position(x, y);

        if (pen_down_status_icon_id < 0)
        {
            return FALSE;
        }
    }

    *icon_id = pen_down_status_icon_id;

    if (MMI_status_icon_pen_event_hdlr[pen_down_status_icon_id][WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON] != NULL)
    {
        w = gui_pen_wait_longpress(pen_event, x, y);
        if (w == GUI_WAIT_LONGPRESS_READY)
        {
            *event_type = WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON;
        }
    }

    if (pen_event == MMI_PEN_EVENT_UP && *event_type == WGUI_STATUS_ICON_BAR_PEN_NONE)
    {
        if (pen_down_status_icon_id == wgui_status_icon_translate_pen_position(x, y))
        {
            *event_type = WGUI_STATUS_ICON_BAR_PEN_SELECT_ICON;
        }
    }

    return TRUE;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 

 


/*****************************************************************************
 * FUNCTION
 *  get_status_icon_flag
 * DESCRIPTION
 *  
 * PARAMETERS
 *  icon_ID     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U32 get_status_icon_flag(S16 icon_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_status_icons[icon_ID].flags;
}


/*****************************************************************************
 * FUNCTION
 *  set_status_icon_flag
 * DESCRIPTION
 *  
 * PARAMETERS
 *  icon_ID     [IN]        
 *  value       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_status_icon_flag(S16 icon_ID, U32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((icon_ID < 0) || (icon_ID >= MAX_STATUS_ICONS))
    {
        return;
    }

    /* set the flag to the specific value */
    MMI_status_icons[icon_ID].flags = value;

    /* set rearrange flag on */
    IsReArrangeNeeded = 1;
}

 


/*****************************************************************************
 * FUNCTION
 *  wgui_status_icon_set_display_layer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  display_layer       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_status_icon_set_display_layer(GDI_HANDLE display_layer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_status_icon_display_layer = display_layer;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_status_icon_get_display_layer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
GDI_HANDLE wgui_status_icon_get_display_layer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_status_icon_display_layer;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_status_icon_reset_display_layer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_status_icon_reset_display_layer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_status_icon_display_layer = GDI_LAYER_EMPTY_HANDLE;
}

