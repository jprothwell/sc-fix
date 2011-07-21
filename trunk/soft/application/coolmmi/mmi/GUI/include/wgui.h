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
 *	 wgui.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Common definitions for GUI.
 *
 *  Author:
 * -------
 
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 
/**********************************************************************************
	Filename:		wgui.h
	Date Created:	August-07-2002
	Contains:		UI wrapper routines header
					Contains several parts and has been split to multiple
					code files. See other files named wgui_*.c and wgui_*.h
					This file contains common functions and data (prototypes only).
**********************************************************************************/

#ifndef __WGUI_H__
#define __WGUI_H__

#include "mmi_features.h"
#include "eventsgprot.h"
#include "globalconstants.h"
#include "datetimetype.h"

#include "gui.h"
#include "gui_buttons.h"
#include "gui_scrollbars.h"
#include "gui_inputs.h"
#include "gui_generic_menuitems.h"
#include "gui_fixed_menuitems.h"
#include "gui_generic_menus.h"
#include "gui_fixed_menus.h"
#include "gui_windows.h"
// dingjian 20071026
#include "gui_setting.h"
//#include "gdi_lcd_config.h"

 
#include "gui_dynamic_menus.h"
 

#ifdef __MMI_DRAW_MANAGER__
#include "wgui_draw_manager.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
** Typedef
*/
typedef S32			MMI_key_code_type;
typedef S32			MMI_key_event_type;
typedef U16			UI_string_ID_type;
typedef U16			UI_image_ID_type;
typedef U16			UI_audio_ID_type; 
typedef MYTIME		UI_time;

/*
** Define
*/
#define KEYBOARD_KEY_DOWN									256
#define KEYBOARD_KEY_UP										257
#define STRING_ID_NULL										0
#define IMAGE_ID_NULL										0
#define	MMI_BG_GREYSCALE_VALUE							(40) 
#define	MMI_BG_GREYSCALE_BLACK_VALUE					(40)
/*	Settings							*/
/*	Timeouts: All in milliseconds		*/

#define MMI_MENU_SHORTCUT_TIMEOUT		(600)	
#define MMI_MULTITAP_TIMEOUT			(600)

/*	Dimensions and co-ordinates			*/
/*	All values in pixels				*/
 
#ifdef __MMI_UI_TAB_PANE__
#define		MMI_TAB_HEIGHT				(18)
#endif
 
#define MMI_SUBLCD_TITLE_HEIGHT 15
/* UI_STYLE_1 for 128x128 screen */
#ifdef __MMI_MAINLCD_128X128__
//START 
#if defined( __MMI_UI_SMALL_SCREEN_SUPPORT__)
#define MMI_SMALL_SCREEN_X_OFFSET	(9)
#define MMI_SMALL_SCREEN_Y_OFFSET	(9)
#define MMI_SMALL_LIST_LEFT_OFFSET 2
#define MMI_SMALL_LIST_SCREEN_WIDTH ((MAIN_LCD_DEVICE_WIDTH*3)/4)
#endif	/* UI_SMALL_CATEGORY_EDITOR*/
//END 
#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
	#define MMI_STATUS_BAR_WIDTH				(76)
   	#define MMI_STATUS_BAR_HEIGHT				(20)
	#define MMI_SINGLE_STATUS_BAR_HEIGHT		(16)
#else
   	#define MMI_STATUS_BAR_HEIGHT				(16)
#endif

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
	#define MMI_BUTTON_BAR_HEIGHT			(20)
	#define MMI_SOFTKEY_WIDTH				(74)
	#define MMI_SOFTKEY_HEIGHT				(16)
	#define BAR_CONTENT_GAP					(2)
#else
	#define MMI_BUTTON_BAR_HEIGHT				(18)
	#define MMI_SOFTKEY_WIDTH				(50)
	#define MMI_SOFTKEY_HEIGHT				(18)
#endif

    #define MMI_TITLE_PADDING           (0)
	#define MMI_TITLE_X					(0)
	#define MMI_TITLE_Y					(0)
	#define MMI_TITLE_WIDTH					(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_TITLE_HEIGHT				(18)
	#define MMI_CONTENT_X					(0)
	#define MMI_CONTENT_Y					(MMI_TITLE_HEIGHT)
	#define MMI_CONTENT_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_HEIGHT				(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT)
 
    #define MMI_IDLE_EXTRA_TEXT_OFFSET      (32)     


 
	#define MMI_POP_UP_DIALOG_WIDTH				(MAIN_LCD_DEVICE_WIDTH-20)
	#define MMI_POP_UP_DIALOG_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-6)
	#define MMI_POP_UP_DIALOG_X				((MAIN_LCD_DEVICE_WIDTH>>1)-(MMI_POP_UP_DIALOG_WIDTH>>1))
	#define MMI_POP_UP_DIALOG_Y				(MMI_TITLE_HEIGHT+3)
	#define MMI_POP_UP_DIALOG_FULL_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-(MMI_BUTTON_BAR_HEIGHT>>1)-MMI_TITLE_HEIGHT)
 
#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__)
	#define MMI_MENUITEM_HEIGHT				(17)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(17)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(17)
#else
	#define MMI_MENUITEM_HEIGHT				(18)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(18)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(18)
#endif
 
	#define MMI_MULTIROW_MENUITEM_HEIGHT			(39)
#ifdef __MMI_UI_TAB_PANE__
#define		MMI_TAB_HEIGHT									(18)
#endif
 
#if 0
	#ifdef __MMI_EDITABLE_AUDIO_EQUALIZER__
	#endif
#endif
/* UI_STYLE_2 for 128x160 screen */
#elif defined __MMI_MAINLCD_128X160__
//START 
#if defined( __MMI_UI_SMALL_SCREEN_SUPPORT__)
#define MMI_SMALL_SCREEN_X_OFFSET	(9)
#define MMI_SMALL_SCREEN_Y_OFFSET	(11)
#define MMI_SMALL_LIST_LEFT_OFFSET 2
#define MMI_SMALL_LIST_SCREEN_WIDTH ((MAIN_LCD_DEVICE_WIDTH*3)/4)
#endif	/* UI_SMALL_CATEGORY_EDITOR*/
//END 
#if defined( __MMI_UI_DALMATIAN_STATUSBAR__)
	#define MMI_STATUS_BAR_WIDTH				(76)
   	#define MMI_STATUS_BAR_HEIGHT				(22)
	#define MMI_SINGLE_STATUS_BAR_HEIGHT		(18)
	#define MMI_TITLE_HEIGHT				(22)
#else
	#define MMI_TITLE_HEIGHT				(19) //changed by panxu 2007-8-16 for the new title bar image
	#define MMI_STATUS_BAR_HEIGHT				(18)
#endif
 
        #define MMI_TITLE_PADDING                           (0)
	#define MMI_TITLE_X					(0)
	#define MMI_TITLE_Y					(0)
	#define MMI_TITLE_WIDTH					(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_X					(0)
	#define MMI_CONTENT_Y					(MMI_TITLE_HEIGHT)
	#define MMI_CONTENT_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_HEIGHT				(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT)
 
       #define MMI_IDLE_EXTRA_TEXT_OFFSET             (40)     //added by kecx for fixing bug 12401 on 20090623

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
	#define MMI_BUTTON_BAR_HEIGHT			(22)
	#define MMI_SOFTKEY_WIDTH				(74)
	#define MMI_SOFTKEY_HEIGHT				(18)
	#define BAR_CONTENT_GAP					(2)
#else
	#define MMI_BUTTON_BAR_HEIGHT				(19)  //changde by panxu 2007-8-16
	#define MMI_SOFTKEY_WIDTH				(50) 
	#define MMI_SOFTKEY_HEIGHT				(19)
#endif
 
	#define MMI_POP_UP_DIALOG_WIDTH				(MAIN_LCD_DEVICE_WIDTH-20)
	#define MMI_POP_UP_DIALOG_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-6)
	#define MMI_POP_UP_DIALOG_X				((MAIN_LCD_DEVICE_WIDTH>>1)-(MMI_POP_UP_DIALOG_WIDTH>>1))
	#define MMI_POP_UP_DIALOG_Y				(MMI_TITLE_HEIGHT+3)
	#define MMI_POP_UP_DIALOG_FULL_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-(MMI_BUTTON_BAR_HEIGHT>>1)-MMI_TITLE_HEIGHT)
	#define MMI_MENUITEM_HEIGHT				(19)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(17)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(17)
	#define MMI_MULTIROW_MENUITEM_HEIGHT			(39)
 
#ifdef __MMI_UI_TAB_PANE__
#define		MMI_TAB_HEIGHT									(20)
#endif
 
#if 0
	#ifdef __MMI_EDITABLE_AUDIO_EQUALIZER__
	#endif
#endif

/* UI_STYLE_2 for 160x128 screen */
#elif defined __MMI_MAINLCD_160X128__
//START 
#if defined( __MMI_UI_SMALL_SCREEN_SUPPORT__)
#define MMI_SMALL_SCREEN_X_OFFSET	(9)
#define MMI_SMALL_SCREEN_Y_OFFSET	(11)
#define MMI_SMALL_LIST_LEFT_OFFSET 2
#define MMI_SMALL_LIST_SCREEN_WIDTH ((MAIN_LCD_DEVICE_WIDTH*3)/4)
#endif	/* UI_SMALL_CATEGORY_EDITOR*/
//END 
#if defined( __MMI_UI_DALMATIAN_STATUSBAR__)
	#define MMI_STATUS_BAR_WIDTH				(76)
   	#define MMI_STATUS_BAR_HEIGHT				(22)
	#define MMI_SINGLE_STATUS_BAR_HEIGHT		(18)
	#define MMI_TITLE_HEIGHT				(22)
#else
	#define MMI_TITLE_HEIGHT				(18) 
	#define MMI_STATUS_BAR_HEIGHT				(18)
#endif
 
        #define MMI_TITLE_PADDING                           (0)
	#define MMI_TITLE_X					(0)
	#define MMI_TITLE_Y					(0)
	#define MMI_TITLE_WIDTH					(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_X					(0)
	#define MMI_CONTENT_Y					(MMI_TITLE_HEIGHT)
	#define MMI_CONTENT_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_HEIGHT				(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT)
 
       #define MMI_IDLE_EXTRA_TEXT_OFFSET             (40)     //added by kecx for fixing bug 12401 on 20090623

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
	#define MMI_BUTTON_BAR_HEIGHT			(22)
	#define MMI_SOFTKEY_WIDTH				(74)
	#define MMI_SOFTKEY_HEIGHT				(18)
	#define BAR_CONTENT_GAP					(2)
#else
	#define MMI_BUTTON_BAR_HEIGHT				(19)  //changde by panxu 2007-8-16
	#define MMI_SOFTKEY_WIDTH				(50) 
	#define MMI_SOFTKEY_HEIGHT				(19)
#endif
 
	#define MMI_POP_UP_DIALOG_WIDTH				(MAIN_LCD_DEVICE_WIDTH-20)
	#define MMI_POP_UP_DIALOG_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-6)
	#define MMI_POP_UP_DIALOG_X				((MAIN_LCD_DEVICE_WIDTH>>1)-(MMI_POP_UP_DIALOG_WIDTH>>1))
	#define MMI_POP_UP_DIALOG_Y				(MMI_TITLE_HEIGHT+3)
	#define MMI_POP_UP_DIALOG_FULL_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-(MMI_BUTTON_BAR_HEIGHT>>1)-MMI_TITLE_HEIGHT)
#ifdef __MMI_MAINLCD_160X128__
	#define MMI_MENUITEM_HEIGHT				(22)
#else
	#define MMI_MENUITEM_HEIGHT				(19)
#endif
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(17)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(17)
	#define MMI_MULTIROW_MENUITEM_HEIGHT			(39)
 
#ifdef __MMI_UI_TAB_PANE__
#define		MMI_TAB_HEIGHT									(20)
#endif
 
#if 0
	#ifdef __MMI_EDITABLE_AUDIO_EQUALIZER__
	#endif
#endif

/* UI_STYLE_3 for 176x220 screen */
#elif defined __MMI_MAINLCD_176X220__

//START 
#if defined( __MMI_UI_SMALL_SCREEN_SUPPORT__)
#define MMI_SMALL_SCREEN_X_OFFSET	(12)
#define MMI_SMALL_SCREEN_Y_OFFSET	(15)
#define MMI_SMALL_LIST_LEFT_OFFSET 2
#define MMI_SMALL_LIST_SCREEN_WIDTH ((MAIN_LCD_DEVICE_WIDTH*3)/4)
#endif	/* UI_SMALL_CATEGORY_EDITOR*/
//END 
#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
 
	#define MMI_IDLESCREEN_BAR_HEIGHT			(60)//51
	#define MMI_SIGNAL_X						(0)
	#define MMI_SIGNAL_Y						(0)
	#ifdef __MMI_UI_TECHNO_STATUS_ICON__
	#define MMI_SIGNAL_WIDTH					(12)
	#else
		#define MMI_SIGNAL_WIDTH					(5)
	#endif

	#define ANALOG_BACKGROUND_X					(MMI_SIGNAL_X + MMI_SIGNAL_WIDTH)
	#define ANALOG_BACKGROUND_Y					(MMI_STATUS_BAR_HEIGHT)//0
	#define ANALOG_BACKGROUND_WIDTH				(60)//47
 
#endif

 
   #if defined( __MMI_UI_DALMATIAN_STATUSBAR__)
	#define MMI_STATUS_BAR_WIDTH				(104)
   	#define MMI_STATUS_BAR_HEIGHT				(23)
	#define MMI_SINGLE_STATUS_BAR_HEIGHT		(19)
   #else
#ifdef __MMI_NOKIA_STYLE_N800__
	#define MMI_STATUS_BAR_HEIGHT				(16) //nokia ui needed
#else
	#define MMI_STATUS_BAR_HEIGHT				(18)
#endif
   #endif
 
 
       #define MMI_TITLE_PADDING                           (0)
	#define MMI_TITLE_X					(0)
	#define MMI_TITLE_Y					(MMI_STATUS_BAR_HEIGHT)
	#define MMI_TITLE_WIDTH					(MAIN_LCD_DEVICE_WIDTH)

#ifdef __T800_MAIN_MENU__
#ifdef __MMI_20X20_CHINESE_FONT__
	#define MMI_TITLE_HEIGHT				(24)
#else
	#define MMI_TITLE_HEIGHT				(18)	
#endif    
#else
	#define MMI_TITLE_HEIGHT				(24)	
#endif    

	#define MMI_CONTENT_X					(0)
	#define MMI_CONTENT_Y					(MMI_TITLE_Y+MMI_TITLE_HEIGHT)
	#define MMI_CONTENT_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
#ifdef __MMI_NOKIA_STYLE_N800__
	#define MMI_CONTENT_HEIGHT				(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-MMI_STATUS_BAR_HEIGHT+1)
#else
	#define MMI_CONTENT_HEIGHT				(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-MMI_STATUS_BAR_HEIGHT)
#endif
 
         
        //Dalmatian UI Style has separate button height and  width
       #define MMI_IDLE_EXTRA_TEXT_OFFSET             (55)     //added by kecx for fixing bug 12401 on 20090623

#if defined(__MMI_UI_TECHNO_SOFTKEYS__)
	#define MMI_SOFTKEY_WIDTH				(75)
  	    #define MMI_BUTTON_BAR_HEIGHT				(20)
	    #define MMI_SOFTKEY_HEIGHT				(20)
#elif defined( __MMI_UI_DALMATIAN_SOFTKEYS__)
	    #define MMI_BUTTON_BAR_HEIGHT			(24)
	    #define MMI_SOFTKEY_WIDTH				(104)
	    #define MMI_SOFTKEY_HEIGHT				(19)
#else
		#define MMI_SOFTKEY_WIDTH				(70) 
		#ifdef __MMI_20X20_CHINESE_FONT__
	    #define MMI_BUTTON_BAR_HEIGHT				(24)
		#define MMI_SOFTKEY_HEIGHT				(24)
		#else
	    #define MMI_BUTTON_BAR_HEIGHT				(20)
	    #define MMI_SOFTKEY_HEIGHT				(20)
		#endif
#endif
 
        #if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || (defined __MMI_UI_DALMATIAN_SOFTKEYS__)
	     #define BAR_CONTENT_GAP					(1)
        #endif
         
#ifdef __MMI_NOKIA_STYLE_N800__
	#define MMI_POP_UP_DIALOG_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_POP_UP_DIALOG_HEIGHT			95
	#define MMI_POP_UP_DIALOG_X				0
	#define MMI_POP_UP_DIALOG_Y				105
#else
	#define MMI_POP_UP_DIALOG_WIDTH				(MAIN_LCD_DEVICE_WIDTH-40)
	#define MMI_POP_UP_DIALOG_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-MMI_TITLE_Y-20)
	#define MMI_POP_UP_DIALOG_X				((MAIN_LCD_DEVICE_WIDTH>>1)-(MMI_POP_UP_DIALOG_WIDTH>>1))
	#define MMI_POP_UP_DIALOG_Y				(MMI_TITLE_Y+(MMI_TITLE_HEIGHT+10))
#endif
	#define MMI_POP_UP_DIALOG_FULL_HEIGHT			MMI_POP_UP_DIALOG_HEIGHT
        
#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__)
 
//To maintain a gap from bottom
	#define MMI_MENUITEM_HEIGHT				(20)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(20)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(20)
 
#else
	#define MMI_MENUITEM_HEIGHT				(22)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(22)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(22)
#endif
#ifdef __T800_MAIN_MENU__
#ifdef MMI_MENUITEM_HEIGHT
#undef MMI_MENUITEM_HEIGHT
#undef MMI_ICONTEXT_MENUITEM_HEIGHT
#undef MMI_INLINE_EDIT_MENUITEM_HEIGHT
#endif
#define MMI_MENUITEM_HEIGHT                             (32)
#define MMI_ICONTEXT_MENUITEM_HEIGHT                 (21)
#define MMI_INLINE_EDIT_MENUITEM_HEIGHT              (30)
#endif
         
	#define MMI_MULTIROW_MENUITEM_HEIGHT			(39)

	#define MMI_ROTATED_LCD_WIDTH								MAIN_LCD_DEVICE_HEIGHT
	#define MMI_ROTATED_LCD_HEIGHT							MAIN_LCD_DEVICE_WIDTH

	#define MMI_ROTATED_TITLE_WIDTH							155
	#define MMI_ROTATED_TITLE_HEIGHT							MMI_TITLE_HEIGHT

	#define MMI_ROTATED_TITLE_X								10
	#define MMI_ROTATED_TITLE_Y								((MMI_ROTATED_LCD_HEIGHT -  MMI_ROTATED_CONTENT_HEIGHT - MMI_ROTATED_TITLE_HEIGHT)/2)

	#define MMI_ROTATED_CONTENT_X					0
	#define MMI_ROTATED_CONTENT_Y					(MMI_ROTATED_TITLE_Y + MMI_ROTATED_TITLE_HEIGHT)
	#define MMI_ROTATED_CONTENT_WIDTH				(MAIN_LCD_DEVICE_HEIGHT)  // liuxy 20080228 (230)
	#define MMI_ROTATED_CONTENT_HEIGHT				(120)

	#define MMI_ROTATED_POP_UP_DIALOG_X						(MMI_ROTATED_CONTENT_X + 7)
	#define MMI_ROTATED_POP_UP_DIALOG_Y						(MMI_ROTATED_TITLE_Y + MMI_ROTATED_TITLE_HEIGHT + 7)
	#define MMI_ROTATED_POP_UP_DIALOG_WIDTH				(MMI_ROTATED_CONTENT_WIDTH - 15)
	#define MMI_ROTATED_POP_UP_DIALOG_HEIGHT				(MMI_ROTATED_LCD_HEIGHT - MMI_ROTATED_POP_UP_DIALOG_Y - 20)
	#define MMI_ROTATED_POP_UP_DIALOG_FULL_HEIGHT		(MMI_ROTATED_POP_UP_DIALOG_HEIGHT)

	#define MMI_ROTATED_BUTTON_BAR_X					(MMI_ROTATED_CONTENT_X + MMI_ROTATED_CONTENT_WIDTH -MMI_ROTATED_BUTTON_BAR_WIDTH)
	#define MMI_ROTATED_BUTTON_BAR_Y					0
	#define MMI_ROTATED_BUTTON_BAR_WIDTH				50
	#define MMI_ROTATED_BUTTON_BAR_HEIGHT				MMI_ROTATED_LCD_HEIGHT
	#define MMI_ROTATED_SOFTKEY_WIDTH					MMI_ROTATED_BUTTON_BAR_WIDTH
	#define MMI_ROTATED_SOFTKEY_HEIGHT					MMI_SOFTKEY_HEIGHT
#ifdef __MMI_UI_TAB_PANE__
#define		MMI_TAB_HEIGHT									(22)
#endif
 
/* UI_STYLE_3 for 220x176 screen */
#elif defined __MMI_MAINLCD_220X176__


#if defined( __MMI_UI_SMALL_SCREEN_SUPPORT__)
#define MMI_SMALL_SCREEN_X_OFFSET	(12)
#define MMI_SMALL_SCREEN_Y_OFFSET	(15)
#define MMI_SMALL_LIST_LEFT_OFFSET 2
#define MMI_SMALL_LIST_SCREEN_WIDTH ((MAIN_LCD_DEVICE_WIDTH*3)/4)
#endif	/* UI_SMALL_CATEGORY_EDITOR*/

#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)

	#define MMI_IDLESCREEN_BAR_HEIGHT			(60)//51
	#define MMI_SIGNAL_X						(0)
	#define MMI_SIGNAL_Y						(0)
	#ifdef __MMI_UI_TECHNO_STATUS_ICON__
	#define MMI_SIGNAL_WIDTH					(12)  
	#else
		#define MMI_SIGNAL_WIDTH					(5)
	#endif

	#define ANALOG_BACKGROUND_X					(MMI_SIGNAL_X + MMI_SIGNAL_WIDTH)
	#define ANALOG_BACKGROUND_Y					(MMI_STATUS_BAR_HEIGHT)//0
	#define ANALOG_BACKGROUND_WIDTH				(60)//47

#endif


   #if defined( __MMI_UI_DALMATIAN_STATUSBAR__)
	#define MMI_STATUS_BAR_WIDTH				(104)
   	#define MMI_STATUS_BAR_HEIGHT				(23)
	#define MMI_SINGLE_STATUS_BAR_HEIGHT		(19)
   #else
	#define MMI_STATUS_BAR_HEIGHT				(24)  //(18) fzb
   #endif

        #define MMI_TITLE_PADDING                           (0)
	#define MMI_TITLE_X					(24)
	#define MMI_TITLE_Y					(0)  //(MMI_STATUS_BAR_HEIGHT)  // 2010-9-19 fzb 
	#define MMI_TITLE_WIDTH					(184)
	#define MMI_TITLE_HEIGHT				(MMI_STATUS_BAR_HEIGHT)//songhui
	#define MMI_CONTENT_X					(0)
	#define MMI_CONTENT_Y					(MMI_TITLE_Y+MMI_TITLE_HEIGHT)  //fzb
	#define MMI_CONTENT_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_HEIGHT				(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_STATUS_BAR_HEIGHT)
        //Dalmatian UI Style has separate button height and  width
       #define MMI_IDLE_EXTRA_TEXT_OFFSET             (55)     //added by kecx for fixing bug 12401 on 20090623

#if defined(__MMI_UI_TECHNO_SOFTKEYS__)
	#define MMI_SOFTKEY_WIDTH					(75)
	#define MMI_BUTTON_BAR_HEIGHT				(20)
	#define MMI_SOFTKEY_HEIGHT				(20)
#elif defined( __MMI_UI_DALMATIAN_SOFTKEYS__)
	    #define MMI_BUTTON_BAR_HEIGHT			(24)
	    #define MMI_SOFTKEY_WIDTH				(104)
	    #define MMI_SOFTKEY_HEIGHT				(19)
#else
	#define MMI_BUTTON_BAR_HEIGHT				(18)
	#define MMI_SOFTKEY_WIDTH					(70)
	#define MMI_SOFTKEY_HEIGHT				(18)
#endif

        #if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || (defined __MMI_UI_DALMATIAN_SOFTKEYS__)
	     #define BAR_CONTENT_GAP					(1)
        #endif
	#define MMI_POP_UP_DIALOG_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_POP_UP_DIALOG_HEIGHT			((UI_DEVICE_HEIGHT>>1)-MMI_BUTTON_BAR_HEIGHT)
	#define MMI_POP_UP_DIALOG_X					0
	#define MMI_POP_UP_DIALOG_Y					(UI_DEVICE_HEIGHT>>1)
	#define MMI_POP_UP_DIALOG_FULL_HEIGHT		MMI_POP_UP_DIALOG_HEIGHT
#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_SOFTKEYS__)

//To maintain a gap from bottom
	#define MMI_MENUITEM_HEIGHT				(20)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(20)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(20)

#else
	#define MMI_MENUITEM_HEIGHT						(24)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(24)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(24)
#endif

	#define MMI_MULTIROW_MENUITEM_HEIGHT			(39)

	#define MMI_ROTATED_LCD_WIDTH								MAIN_LCD_DEVICE_HEIGHT
	#define MMI_ROTATED_LCD_HEIGHT							MAIN_LCD_DEVICE_WIDTH

	#define MMI_ROTATED_TITLE_WIDTH							155
	#define MMI_ROTATED_TITLE_HEIGHT							MMI_TITLE_HEIGHT

	#define MMI_ROTATED_CONTENT_WIDTH						155
	#define MMI_ROTATED_CONTENT_HEIGHT						(MMI_CONTENT_HEIGHT-MMI_MENUITEM_HEIGHT)

	#define MMI_ROTATED_TITLE_X								10
	#define MMI_ROTATED_TITLE_Y								((MMI_ROTATED_LCD_HEIGHT -  MMI_ROTATED_CONTENT_HEIGHT - MMI_ROTATED_TITLE_HEIGHT)/2)

	#define MMI_ROTATED_CONTENT_X								10
	#define MMI_ROTATED_CONTENT_Y								(MMI_ROTATED_TITLE_Y + MMI_ROTATED_TITLE_HEIGHT)

	#define MMI_ROTATED_POP_UP_DIALOG_X						(MMI_ROTATED_CONTENT_X + 7)
	#define MMI_ROTATED_POP_UP_DIALOG_Y						(MMI_ROTATED_TITLE_Y + MMI_ROTATED_TITLE_HEIGHT + 7)
	#define MMI_ROTATED_POP_UP_DIALOG_WIDTH				(MMI_ROTATED_CONTENT_WIDTH - 15)
	#define MMI_ROTATED_POP_UP_DIALOG_HEIGHT				(MMI_ROTATED_LCD_HEIGHT - MMI_ROTATED_POP_UP_DIALOG_Y - 20)
	#define MMI_ROTATED_POP_UP_DIALOG_FULL_HEIGHT		(MMI_ROTATED_POP_UP_DIALOG_HEIGHT)

	#define MMI_ROTATED_BUTTON_BAR_X							(MMI_ROTATED_CONTENT_X + MMI_ROTATED_CONTENT_WIDTH + 3 /* leave space for border frame */)
	#define MMI_ROTATED_BUTTON_BAR_Y							0
	#define MMI_ROTATED_BUTTON_BAR_WIDTH					(MMI_ROTATED_LCD_WIDTH - MMI_ROTATED_CONTENT_X - MMI_ROTATED_CONTENT_WIDTH - 3)
	#define MMI_ROTATED_BUTTON_BAR_HEIGHT					MMI_ROTATED_LCD_HEIGHT
	#define MMI_ROTATED_SOFTKEY_WIDTH						MMI_ROTATED_BUTTON_BAR_WIDTH
	#define MMI_ROTATED_SOFTKEY_HEIGHT						MMI_SOFTKEY_HEIGHT
#ifdef __MMI_UI_TAB_PANE__
#define		MMI_TAB_HEIGHT									(22)
#endif

/* UI_STYLE_4 for 240x320 screen */
#elif defined __MMI_MAINLCD_240X320__
//START 
#if defined( __MMI_UI_SMALL_SCREEN_SUPPORT__)
#define MMI_SMALL_SCREEN_X_OFFSET	(16)
#define MMI_SMALL_SCREEN_Y_OFFSET	(22)
#define MMI_SMALL_LIST_LEFT_OFFSET 2
#define MMI_SMALL_LIST_SCREEN_WIDTH ((MAIN_LCD_DEVICE_WIDTH*3)/4)
#endif	/* UI_SMALL_CATEGORY_EDITOR*/
#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
	#define MMI_IDLESCREEN_BAR_HEIGHT			(80) 
	#define MMI_SIGNAL_X						(0)
	#define MMI_SIGNAL_Y						(0)

	#ifdef __MMI_UI_TECHNO_STATUS_ICON__
		#define MMI_SIGNAL_WIDTH					(20)
	#else
		#define MMI_SIGNAL_WIDTH					(5)
	#endif

	#define ANALOG_BACKGROUND_X					(MMI_SIGNAL_X + MMI_SIGNAL_WIDTH)
	#define ANALOG_BACKGROUND_Y					(MMI_STATUS_BAR_HEIGHT)
	#define ANALOG_BACKGROUND_WIDTH				(80) 
#endif
 
 
#if defined( __MMI_UI_DALMATIAN_STATUSBAR__)
	#define MMI_STATUS_BAR_WIDTH				(135)
   	#define MMI_STATUS_BAR_HEIGHT				(28)
	#define MMI_SINGLE_STATUS_BAR_HEIGHT		(24)
#else
	#define MMI_STATUS_BAR_HEIGHT				(26) // dingjian 20080221
#endif
 
        #define MMI_TITLE_PADDING                           (4)
	#define MMI_TITLE_X					(0)
	#define MMI_TITLE_Y					(MMI_STATUS_BAR_HEIGHT)
	#define MMI_TITLE_WIDTH					(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_X					(0)
	#define MMI_CONTENT_Y					(MMI_TITLE_Y+MMI_TITLE_HEIGHT)
	#define MMI_CONTENT_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_HEIGHT				(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-MMI_STATUS_BAR_HEIGHT)
 

       #define MMI_IDLE_EXTRA_TEXT_OFFSET             (80)     //added by kecx for fixing bug 12401 on 20090623

#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || (defined __MMI_UI_DALMATIAN_SOFTKEYS__)
	#define MMI_BUTTON_BAR_HEIGHT				(28)
	#define MMI_SOFTKEY_WIDTH				(135)
	#define MMI_SOFTKEY_HEIGHT				(24)
	#define MMI_TITLE_HEIGHT				(40)
	#define BAR_CONTENT_GAP					(1)
#else
        #define MMI_BUTTON_BAR_HEIGHT				(26)// dingjian 20080221
        #define MMI_SOFTKEY_WIDTH				(70) 
        #define MMI_SOFTKEY_HEIGHT				(24) 
        #define MMI_TITLE_HEIGHT				(26)// dingjian 20080221
#endif
 
	#define MMI_POP_UP_DIALOG_WIDTH				(MAIN_LCD_DEVICE_WIDTH-22) 
	#define MMI_POP_UP_DIALOG_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-MMI_TITLE_Y-62) 
	#define MMI_POP_UP_DIALOG_X				((MAIN_LCD_DEVICE_WIDTH>>1)-(MMI_POP_UP_DIALOG_WIDTH>>1))
	#define MMI_POP_UP_DIALOG_Y				(MMI_TITLE_Y+(MMI_TITLE_HEIGHT+20)) 
	#define MMI_POP_UP_DIALOG_FULL_HEIGHT			MMI_POP_UP_DIALOG_HEIGHT
 
#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || (defined __MMI_UI_DALMATIAN_SOFTKEYS__)
	#define MMI_MENUITEM_HEIGHT				(29)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(29)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(29)
#else
	#define MMI_MENUITEM_HEIGHT				(30) 
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(30)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(30)
#endif
 
	#define MMI_MULTIROW_MENUITEM_HEIGHT			(39)
#if defined(__MMI_SCREEN_ROTATE__)
		#define MMI_ROTATED_LCD_WIDTH						(MAIN_LCD_DEVICE_HEIGHT)
		#define MMI_ROTATED_LCD_HEIGHT					(MAIN_LCD_DEVICE_WIDTH)
		#define MMI_ROTATED_TITLE_WIDTH					(200) // liuxy 20080228 (230)
		#define MMI_ROTATED_TITLE_HEIGHT					(20)

		#define MMI_ROTATED_CONTENT_WIDTH				(MAIN_LCD_DEVICE_HEIGHT)  // liuxy 20080228 (230)
		#define MMI_ROTATED_CONTENT_HEIGHT				(180) // liuxy 20080228 (MMI_CONTENT_HEIGHT-(MMI_MENUITEM_HEIGHT << 1))

		#define MMI_ROTATED_TITLE_X						(20)
		#define MMI_ROTATED_TITLE_Y						(0) // liuxy 20080228  ((MMI_ROTATED_LCD_HEIGHT -  MMI_ROTATED_CONTENT_HEIGHT - MMI_ROTATED_TITLE_HEIGHT)/2)

		#define MMI_ROTATED_CONTENT_X						(0)   // liuxy 20080228(15)
		#define MMI_ROTATED_CONTENT_Y						(32)  // liuxy 20080228 (MMI_ROTATED_TITLE_Y + MMI_ROTATED_TITLE_HEIGHT)

		#define MMI_ROTATED_POP_UP_DIALOG_X				(MMI_ROTATED_CONTENT_X + 10)
		#define MMI_ROTATED_POP_UP_DIALOG_Y				(MMI_ROTATED_TITLE_Y + MMI_ROTATED_TITLE_HEIGHT + 10)
		#define MMI_ROTATED_POP_UP_DIALOG_WIDTH		(MMI_ROTATED_CONTENT_WIDTH - 15)
		#define MMI_ROTATED_POP_UP_DIALOG_HEIGHT		(MMI_ROTATED_LCD_HEIGHT - MMI_ROTATED_POP_UP_DIALOG_Y - 20)
		#define MMI_ROTATED_POP_UP_DIALOG_FULL_HEIGHT	(MMI_ROTATED_POP_UP_DIALOG_HEIGHT)

		#define MMI_ROTATED_BUTTON_BAR_X				(MMI_ROTATED_CONTENT_X + MMI_ROTATED_CONTENT_WIDTH -60) // liuxy 20080228	(MMI_ROTATED_CONTENT_X + MMI_ROTATED_CONTENT_WIDTH + 3 /* leave space for border frame */)
		#define MMI_ROTATED_BUTTON_BAR_Y					(0)
		#define MMI_ROTATED_BUTTON_BAR_WIDTH			(60) // liuxy 20080228 (MMI_ROTATED_LCD_WIDTH - MMI_ROTATED_CONTENT_X - MMI_ROTATED_CONTENT_WIDTH - 3)
		#define MMI_ROTATED_BUTTON_BAR_HEIGHT			(MMI_ROTATED_LCD_HEIGHT)
		#define MMI_ROTATED_SOFTKEY_WIDTH				(MMI_ROTATED_BUTTON_BAR_WIDTH)
		#define MMI_ROTATED_SOFTKEY_HEIGHT				(MMI_SOFTKEY_HEIGHT)
#endif
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
/* UI_STYLE_4 for 320x240 screen */
#elif defined __MMI_MAINLCD_320X240__
//START 
#if defined( __MMI_UI_SMALL_SCREEN_SUPPORT__)
#define MMI_SMALL_SCREEN_X_OFFSET	(16)
#define MMI_SMALL_SCREEN_Y_OFFSET	(22)
#define MMI_SMALL_LIST_LEFT_OFFSET 2
#define MMI_SMALL_LIST_SCREEN_WIDTH ((MAIN_LCD_DEVICE_WIDTH*3)/4)
#endif	/* UI_SMALL_CATEGORY_EDITOR*/
#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
	#define MMI_IDLESCREEN_BAR_HEIGHT			(65) 
	#define MMI_SIGNAL_X						(0)
	#define MMI_SIGNAL_Y						(0)

	#ifdef __MMI_UI_TECHNO_STATUS_ICON__
		#define MMI_SIGNAL_WIDTH					(20)
	#else
		#define MMI_SIGNAL_WIDTH					(5)
	#endif

	#define ANALOG_BACKGROUND_X					(MMI_SIGNAL_X + MMI_SIGNAL_WIDTH)
	#define ANALOG_BACKGROUND_Y					(MMI_STATUS_BAR_HEIGHT)
	#define ANALOG_BACKGROUND_WIDTH				(80) 
#endif
 
 
#if defined( __MMI_UI_DALMATIAN_STATUSBAR__)
	#define MMI_STATUS_BAR_WIDTH				(135)
   	#define MMI_STATUS_BAR_HEIGHT				(28)
	#define MMI_SINGLE_STATUS_BAR_HEIGHT		(24)
#elif  defined (__MMI_SYMBIAN_STYLE_IDLE__)
   	#define MMI_STATUS_BAR_HEIGHT				(18)
#else
	#define MMI_STATUS_BAR_HEIGHT				(26) // dingjian 20080221
#endif
 
        #define MMI_TITLE_PADDING                           (4)
	#define MMI_TITLE_X					(0)
	#define MMI_TITLE_Y					(MMI_STATUS_BAR_HEIGHT)
	#define MMI_TITLE_WIDTH					(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_X					(0)
	#define MMI_CONTENT_Y					(MMI_TITLE_Y+MMI_TITLE_HEIGHT)
	#define MMI_CONTENT_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_HEIGHT			       (MAIN_LCD_DEVICE_HEIGHT-MMI_STATUS_BAR_HEIGHT-MMI_TITLE_HEIGHT-MMI_BUTTON_BAR_HEIGHT)
	 

       #define MMI_IDLE_EXTRA_TEXT_OFFSET             (80)     //added by kecx for fixing bug 12401 on 20090623

#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || (defined __MMI_UI_DALMATIAN_SOFTKEYS__)
	#define MMI_BUTTON_BAR_HEIGHT				(28)
	#define MMI_SOFTKEY_WIDTH				(135)
	#define MMI_SOFTKEY_HEIGHT				(24)
	#define MMI_TITLE_HEIGHT				(40)
	#define BAR_CONTENT_GAP					(1)
	#elif defined ( __MMI_SYMBIAN_STYLE_IDLE__)	
	#define MMI_BUTTON_BAR_HEIGHT				(24)
	#define MMI_SOFTKEY_WIDTH				(135)
	#define MMI_SOFTKEY_HEIGHT				(24)
	#define MMI_TITLE_HEIGHT				(24)
	#define BAR_CONTENT_GAP 				(1)
#else
        #define MMI_BUTTON_BAR_HEIGHT				(26)// dingjian 20080221
        #define MMI_SOFTKEY_WIDTH				(70) 
        #define MMI_SOFTKEY_HEIGHT				(24) 
        #define MMI_TITLE_HEIGHT				(26)// dingjian 20080221
#endif


 
	#define MMI_POP_UP_DIALOG_WIDTH				(MAIN_LCD_DEVICE_WIDTH-22) 
	#define MMI_POP_UP_DIALOG_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-MMI_TITLE_Y-62) 
	#define MMI_POP_UP_DIALOG_X				((MAIN_LCD_DEVICE_WIDTH>>1)-(MMI_POP_UP_DIALOG_WIDTH>>1))
	#define MMI_POP_UP_DIALOG_Y				(MMI_TITLE_Y+(MMI_TITLE_HEIGHT+20)) 
	#define MMI_POP_UP_DIALOG_FULL_HEIGHT			MMI_POP_UP_DIALOG_HEIGHT
 
#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || (defined __MMI_UI_DALMATIAN_SOFTKEYS__)
	#define MMI_MENUITEM_HEIGHT				(29)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(29)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(29)
#else
	#define MMI_MENUITEM_HEIGHT				(30) 
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(30)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(30)
#endif
 
	#define MMI_MULTIROW_MENUITEM_HEIGHT			(44)
#if defined(__MMI_SCREEN_ROTATE__)
		#define MMI_ROTATED_LCD_WIDTH						(MAIN_LCD_DEVICE_HEIGHT)
		#define MMI_ROTATED_LCD_HEIGHT					(MAIN_LCD_DEVICE_WIDTH)
		#define MMI_ROTATED_TITLE_WIDTH					(200) // liuxy 20080228 (230)
		#define MMI_ROTATED_TITLE_HEIGHT					(20)

		#define MMI_ROTATED_CONTENT_WIDTH				(MAIN_LCD_DEVICE_HEIGHT)  // liuxy 20080228 (230)
		#define MMI_ROTATED_CONTENT_HEIGHT				(180) // liuxy 20080228 (MMI_CONTENT_HEIGHT-(MMI_MENUITEM_HEIGHT << 1))

		#define MMI_ROTATED_TITLE_X						(20)
		#define MMI_ROTATED_TITLE_Y						(0) // liuxy 20080228  ((MMI_ROTATED_LCD_HEIGHT -  MMI_ROTATED_CONTENT_HEIGHT - MMI_ROTATED_TITLE_HEIGHT)/2)

		#define MMI_ROTATED_CONTENT_X						(0)   // liuxy 20080228(15)
		#define MMI_ROTATED_CONTENT_Y						(32)  // liuxy 20080228 (MMI_ROTATED_TITLE_Y + MMI_ROTATED_TITLE_HEIGHT)

		#define MMI_ROTATED_POP_UP_DIALOG_X				(MMI_ROTATED_CONTENT_X + 10)
		#define MMI_ROTATED_POP_UP_DIALOG_Y				(MMI_ROTATED_TITLE_Y + MMI_ROTATED_TITLE_HEIGHT + 10)
		#define MMI_ROTATED_POP_UP_DIALOG_WIDTH		(MMI_ROTATED_CONTENT_WIDTH - 15)
		#define MMI_ROTATED_POP_UP_DIALOG_HEIGHT		(MMI_ROTATED_LCD_HEIGHT - MMI_ROTATED_POP_UP_DIALOG_Y - 20)
		#define MMI_ROTATED_POP_UP_DIALOG_FULL_HEIGHT	(MMI_ROTATED_POP_UP_DIALOG_HEIGHT)

		#define MMI_ROTATED_BUTTON_BAR_X				(MMI_ROTATED_CONTENT_X + MMI_ROTATED_CONTENT_WIDTH -60) // liuxy 20080228	(MMI_ROTATED_CONTENT_X + MMI_ROTATED_CONTENT_WIDTH + 3 /* leave space for border frame */)
		#define MMI_ROTATED_BUTTON_BAR_Y					(0)
		#define MMI_ROTATED_BUTTON_BAR_WIDTH			(60) // liuxy 20080228 (MMI_ROTATED_LCD_WIDTH - MMI_ROTATED_CONTENT_X - MMI_ROTATED_CONTENT_WIDTH - 3)
		#define MMI_ROTATED_BUTTON_BAR_HEIGHT			(MMI_ROTATED_LCD_HEIGHT)
		#define MMI_ROTATED_SOFTKEY_WIDTH				(MMI_ROTATED_BUTTON_BAR_WIDTH)
		#define MMI_ROTATED_SOFTKEY_HEIGHT				(MMI_SOFTKEY_HEIGHT)
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */

#else
	#define MMI_STATUS_BAR_HEIGHT				(16)
	#define MMI_TITLE_X					(0)
	#define MMI_TITLE_Y					(0)
	#define MMI_TITLE_WIDTH					(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_TITLE_HEIGHT				(18)
       #define MMI_IDLE_EXTRA_TEXT_OFFSET             (80)     //added by kecx for fixing bug 12401 on 20090623

	#define MMI_CONTENT_X					(0)
	#define MMI_CONTENT_Y					(MMI_TITLE_Y+MMI_TITLE_HEIGHT)
	#define MMI_CONTENT_WIDTH				(MAIN_LCD_DEVICE_WIDTH)
	#define MMI_CONTENT_HEIGHT				(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT)
	#define MMI_BUTTON_BAR_HEIGHT				(18)
	#define MMI_SOFTKEY_WIDTH				(50) 
	#define MMI_SOFTKEY_HEIGHT				(18)
	#define MMI_POP_UP_DIALOG_WIDTH				(MAIN_LCD_DEVICE_WIDTH-20)
	#define MMI_POP_UP_DIALOG_HEIGHT			(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_TITLE_HEIGHT-6)
	#define MMI_POP_UP_DIALOG_X				((MAIN_LCD_DEVICE_WIDTH>>1)-(MMI_POP_UP_DIALOG_WIDTH>>1))
	#define MMI_POP_UP_DIALOG_Y				(MMI_TITLE_HEIGHT+3)
	#define MMI_POP_UP_DIALOG_FULL_HEIGHT			(MAIN_LCD_DEVICE_WIDTH-(MMI_BUTTON_BAR_HEIGHT>>1)-MMI_TITLE_HEIGHT)

	#define MMI_MENUITEM_HEIGHT				(18)
	#define MMI_ICONTEXT_MENUITEM_HEIGHT			(18)
	#define MMI_INLINE_EDIT_MENUITEM_HEIGHT			(18)
	#define MMI_MULTIROW_MENUITEM_HEIGHT		(39)
#endif

//common for all UI_STYLE
#define MMI_MATRIX_BAR_WIDTH			(MAIN_LCD_DEVICE_WIDTH)
#define MMI_MATRIX_BAR_HEIGHT			(19)
#define MMI_MATRIX_BAR_X				(0)
#define MMI_MATRIX_BAR_Y				(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_MATRIX_BAR_HEIGHT+1)

#define MMI_MULTITAP_X					(2)
#define MMI_MULTITAP_WIDTH				(MAIN_LCD_DEVICE_WIDTH-4)
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_320X240__) 
/*- zhouqin 2011-02-16 modify for q6 320X240 */
   #define MMI_MULTITAP_HEIGHT				(24)
#else
#ifdef	__MMI_20X20_CHINESE_FONT__
   #define MMI_MULTITAP_HEIGHT				(24)
#else
   #define MMI_MULTITAP_HEIGHT				(17)
#endif   
#endif

 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
#define MMI_MULTITAP_Y					(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_MULTITAP_HEIGHT -1)
#else
 
//#define MMI_MULTITAP_Y					(UI_device_height-MMI_BUTTON_BAR_HEIGHT-MMI_MULTITAP_HEIGHT-1)
#define MMI_MULTITAP_Y					(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-MMI_MULTITAP_HEIGHT)
 
#endif
 

#define MMI_SINGLELINE_INPUTBOX_X		(5)
#define MMI_SINGLELINE_INPUTBOX_Y		(40)
#define MMI_SINGLELINE_INPUTBOX_WIDTH	(MAIN_LCD_DEVICE_WIDTH-10)
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_240X320__)  || defined(__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#define MMI_SINGLELINE_INPUTBOX_HEIGHT	(26)
#else
#define MMI_SINGLELINE_INPUTBOX_HEIGHT	(20)
#endif

#define MMI_MULTILINE_INPUTBOX_X		(0)
#define MMI_MULTILINE_INPUTBOX_Y		(20)
#define MMI_MULTILINE_INPUTBOX_WIDTH	(MAIN_LCD_DEVICE_WIDTH)
#define MMI_MULTILINE_INPUTBOX_HEIGHT	(MAIN_LCD_DEVICE_HEIGHT-82)

#define MMI_GENERAL_INPUTBOX_X			(0)
#define MMI_GENERAL_INPUTBOX_Y			(20)
#define MMI_GENERAL_INPUTBOX_WIDTH		(MAIN_LCD_DEVICE_WIDTH)
#define MMI_GENERAL_INPUTBOX_HEIGHT		(MAIN_LCD_DEVICE_HEIGHT-82)

/* Tab bar */
#if defined(__MMI_MAINLCD_240X320__)
	#define MMI_HORIZONTAL_TAB_BAR_TAB_WIDTH			(60)
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#elif defined( __MMI_MAINLCD_320X240__)
	#define MMI_HORIZONTAL_TAB_BAR_TAB_WIDTH			(60)
/*- zhouqin 2011-02-16 modify for q6 320X240 */	
#elif defined( __MMI_MAINLCD_176X220__)
	#define MMI_HORIZONTAL_TAB_BAR_TAB_WIDTH			(44)
#elif defined( __MMI_MAINLCD_220X176__)
	#define MMI_HORIZONTAL_TAB_BAR_TAB_WIDTH			(44)	
#else
	#define MMI_HORIZONTAL_TAB_BAR_TAB_WIDTH			(42)
#endif

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_240X320__)  || defined(__MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#define MMI_HORIZONTAL_TAB_BAR_FOCUSED_TAB_WIDTH	(60)
#else
#define MMI_HORIZONTAL_TAB_BAR_FOCUSED_TAB_WIDTH	(44)
#endif

#define MMI_HORIZONTAL_TAB_BAR_X							(MMI_TITLE_X)
#define MMI_HORIZONTAL_TAB_BAR_Y							(MMI_TITLE_Y)
#define MMI_HORIZONTAL_TAB_BAR_WIDTH					(MMI_TITLE_WIDTH)
#if defined(__MMI_MAINLCD_240X320__)
#define MMI_HORIZONTAL_TAB_BAR_TAB_AREA_HEIGHT		(28)
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#elif defined(__MMI_MAINLCD_320X240__) 
    #define MMI_HORIZONTAL_TAB_BAR_TAB_AREA_HEIGHT          (36)
/*- zhouqin 2011-02-16 modify for q6 320X240 */	
#else
#define MMI_HORIZONTAL_TAB_BAR_TAB_AREA_HEIGHT		(20)
#endif

#if defined(__MMI_MAINLCD_240X320__)
	#define MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT	(26)
#elif defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_128X160__)
#define MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT					(20)
#elif defined(__MMI_MAINLCD_220X176__) || defined(__MMI_MAINLCD_160X128__)
#define MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT					(20)
#elif defined(__MMI_MAINLCD_128X128__)
#define MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT					(20)
#else
#define MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT					(0)
#endif

#define MMI_CONTENT_WIDTH_WITH_H_TAB				(MMI_content_width)
#define MMI_CONTENT_HEIGHT_WITH_H_TAB				(MMI_content_height + MMI_TITLE_HEIGHT - MMI_HORIZONTAL_TAB_BAR_TAB_AREA_HEIGHT - MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT)
#define MMI_CONTENT_X_WITH_H_TAB						(MMI_content_x)
#define MMI_CONTENT_Y_WITH_H_TAB						(MMI_content_y + MMI_HORIZONTAL_TAB_BAR_TAB_AREA_HEIGHT + MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT - MMI_TITLE_HEIGHT)

#define MMI_DROPDOWN_MENUITEM_HEIGHT	(18)
#if !defined(__MMI_TOUCH_SCREEN__)
	 
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_320X240__) 
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
		#define INFORMATION_BAR_HEIGHT 					(22)
	#else
#ifdef __MMI_NOKIA_STYLE_N800__
		#define INFORMATION_BAR_HEIGHT 						(22)
#else
		#define INFORMATION_BAR_HEIGHT 						(18)

#endif
	#endif
#else
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_320X240__) 
/*- zhouqin 2011-02-16 modify for q6 320X240 */
 
#define INFORMATION_BAR_HEIGHT (22)
#else
#define INFORMATION_BAR_HEIGHT (18)
#endif
#endif
//CSD added by JL 031118 for compiler issue at WAP.
#define MMI_POP_UP_DIALOG_FULL_Y		(((MAIN_LCD_DEVICE_HEIGHT-MMI_POP_UP_DIALOG_FULL_HEIGHT)>>1) + 6)


 

    #if defined (__MMI_MAINLCD_240X400__)
        #define MMI_DIALING_KEYPAD_COLOMNS                      (3)
        #define MMI_DIALING_KEYPAD_ROWS                         (4)
        #define MMI_DIALING_KEYPAD_LAYER_WIDTH                  (240)
        #define MMI_DIALING_KEYPAD_LAYER_HEIGHT                 (400)
        #define MMI_DIALING_KEYPAD_LAYER_X                      (0)
        #define MMI_DIALING_KEYPAD_LAYER_Y                      (0)
        #define MMI_DIALING_KEYPAD_X                            (2)
        #define MMI_DIALING_KEYPAD_Y                            (115)
        #define MMI_DIALING_KEYPAD_WIDTH                        (240)
        #define MMI_DIALING_KEYPAD_HEIGHT                       (400)
        #define MMI_DIALING_KEY_WIDTH                           (77)
        #define MMI_DIALING_KEY_HEIGHT                          (49)
        #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (2)
        #define MMI_DIALING_KEY_VERTICAL_GAP                    (2)
        #define MMI_DIALING_BOX_X                               (19)
        #define MMI_DIALING_BOX_Y                               (37)
        #define MMI_DIALING_BOX_WIDTH                           (202)
        #define MMI_DIALING_BOX_HEIGHT                          (64)
        /* For function key on idlescreen */
        #if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
            #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (2)
            #define MMI_DIALING_KEYPAD_FUNC_ROWS                (1)
            #define MMI_DIALING_KEYPAD_FUNC_X                   (9)
            #define MMI_DIALING_KEYPAD_FUNC_Y                   (321)
            #define MMI_DIALING_FUNC_KEY_WIDTH                  (108)
            #define MMI_DIALING_FUNC_KEY_HEIGHT                 (47)
            #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (6)
            #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (0)
        #endif
    #elif defined (__MMI_MAINLCD_320X240__)
        #define MMI_DIALING_KEYPAD_COLOMNS                      (3)
        #define MMI_DIALING_KEYPAD_ROWS                         (4)
        #define MMI_DIALING_KEYPAD_LAYER_WIDTH                  (320)
        #define MMI_DIALING_KEYPAD_LAYER_HEIGHT                 (195)
        #define MMI_DIALING_KEYPAD_LAYER_X                      (0)//((MAIN_LCD_DEVICE_WIDTH-MMI_DIALING_KEYPAD_LAYER_WIDTH)>>1)
        #define MMI_DIALING_KEYPAD_LAYER_Y                      (68)//(((MAIN_LCD_DEVICE_HEIGHT-MMI_DIALING_KEYPAD_LAYER_HEIGHT)>>1)+20)
        #define MMI_DIALING_KEYPAD_X                            (0)
        #define MMI_DIALING_KEYPAD_Y                            (0)
        #define MMI_DIALING_KEYPAD_WIDTH                        (320)
        #define MMI_DIALING_KEYPAD_HEIGHT                       (143)
        #define MMI_DIALING_KEY_WIDTH                           (80)
        #define MMI_DIALING_KEY_HEIGHT                          (35)
        #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (1)
        #define MMI_DIALING_KEY_VERTICAL_GAP                    (1)
        #define MMI_DIALING_BOX_HEIGHT                          (50)
        /* For function key on idlescreen */
        #if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
            #ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (1)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (4)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (240)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (0)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (80)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (35)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (1)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (1)
            #else /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (1)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (2)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (216)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (22)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (32)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (33)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (18)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (28)            
            #endif /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
        #endif /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */
    #elif defined (__MMI_MAINLCD_240X320__)
        #define MMI_DIALING_KEYPAD_COLOMNS                      (3)
        #define MMI_DIALING_KEYPAD_ROWS                         (4)
        #define MMI_DIALING_KEYPAD_LAYER_WIDTH                  (240)
        #define MMI_DIALING_KEYPAD_LAYER_HEIGHT                 (200)
#ifndef __MMI_PROJECT_GS_HK603__		
        #define MMI_DIALING_KEYPAD_LAYER_X                   0 
#else
        #define MMI_DIALING_KEYPAD_LAYER_X                      ((MAIN_LCD_DEVICE_WIDTH-MMI_DIALING_KEYPAD_LAYER_WIDTH)>>1)
#endif
        #define MMI_DIALING_KEYPAD_LAYER_Y                      (((MAIN_LCD_DEVICE_HEIGHT-MMI_DIALING_KEYPAD_LAYER_HEIGHT)>>1)+32+2)
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
        #define MMI_DIALING_KEYPAD_X                            (13)
        #define MMI_DIALING_KEYPAD_Y                            (4)
#else
        #define MMI_DIALING_KEYPAD_X                            (7)
        #define MMI_DIALING_KEYPAD_Y                            (1)
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
        #define MMI_DIALING_KEYPAD_WIDTH                        (240)
	 #define MMI_DIALING_KEYPAD_HEIGHT                       (200)
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
        #define MMI_DIALING_KEY_WIDTH                           (39)
        #define MMI_DIALING_KEY_HEIGHT                          (38)
        #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (10)
        #define MMI_DIALING_KEY_VERTICAL_GAP                    (5)
#else
        #define MMI_DIALING_KEY_WIDTH                           (74)
        #define MMI_DIALING_KEY_HEIGHT                          (36)
        #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (2)
        #define MMI_DIALING_KEY_VERTICAL_GAP                    (0)
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
        #define MMI_DIALING_BOX_X                               (0)
        #define MMI_DIALING_BOX_Y                               (0)
        #define MMI_DIALING_BOX_WIDTH                           (0)
        #define MMI_DIALING_BOX_HEIGHT                          (65+2)
        /* For function key on idlescreen */
        #if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
            #ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (3)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (1)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (7)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (235 - MMI_DIALING_KEYPAD_LAYER_Y+4)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (74)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (36)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (2)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (0)
            #else   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */    
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (1)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (2)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (165)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (33)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (37)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (37)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (12)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (27)
            #endif   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */    
        #endif /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */
    #elif defined (__MMI_MAINLCD_176X220__)
        /* For function key on idlescreen */
        #if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
            #define MMI_DIALING_KEYPAD_COLOMNS                      (3)
            #define MMI_DIALING_KEYPAD_ROWS                         (4)
            #define MMI_DIALING_KEYPAD_LAYER_WIDTH                  (176)
            #define MMI_DIALING_KEYPAD_LAYER_HEIGHT                 (200)
            #define MMI_DIALING_KEYPAD_LAYER_X                      (0)
            #define MMI_DIALING_KEYPAD_LAYER_Y                      (50)
#if defined(__DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__)
            #define MMI_DIALING_KEYPAD_X                            (4)
            #define MMI_DIALING_KEYPAD_Y                            (9)
            #define MMI_DIALING_KEYPAD_WIDTH                        (169)
#else
			#define MMI_DIALING_KEYPAD_X							(7)
			#define MMI_DIALING_KEYPAD_Y							(11)
			#define MMI_DIALING_KEYPAD_WIDTH						(126)
#endif
			#define MMI_DIALING_KEYPAD_HEIGHT                       (130)
            #define MMI_DIALING_KEY_WIDTH                           (40)
            #define MMI_DIALING_KEY_HEIGHT                          (31)
#if defined(__DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__)			
            #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (5)
            #define MMI_DIALING_KEY_VERTICAL_GAP                    (5)
#else
			#define MMI_DIALING_KEY_HORIZONTAL_GAP					(3)
			#define MMI_DIALING_KEY_VERTICAL_GAP					(3)
#endif
            #define MMI_DIALING_BOX_X                               (0)
            #define MMI_DIALING_BOX_Y                               (0)    
            #define MMI_DIALING_BOX_WIDTH                           (0)
            #define MMI_DIALING_BOX_HEIGHT                          (33)
            #ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (1)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (3)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (139)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (12)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (31)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (39)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (0)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (6)
            #else   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */        
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (1)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (2)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (165)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (33)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (37)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (37)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (12)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (27)
            #endif   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */        
        #else /* __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__ */     
            #define MMI_DIALING_KEYPAD_COLOMNS                      (3)
            #define MMI_DIALING_KEYPAD_ROWS                         (4)
            #define MMI_DIALING_KEYPAD_LAYER_WIDTH                  (176)
            #define MMI_DIALING_KEYPAD_LAYER_HEIGHT                 (200)
            #define MMI_DIALING_KEYPAD_LAYER_X                      (0)
            #define MMI_DIALING_KEYPAD_LAYER_Y                      (50)
            #define MMI_DIALING_KEYPAD_X                            (18)
            #define MMI_DIALING_KEYPAD_Y                            (7)
            #define MMI_DIALING_KEYPAD_WIDTH                        (145)
            #define MMI_DIALING_KEYPAD_HEIGHT                       (137)
            #define MMI_DIALING_KEY_WIDTH                           (39)
            #define MMI_DIALING_KEY_HEIGHT                          (31)
            #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (13)
            #define MMI_DIALING_KEY_VERTICAL_GAP                    (5)
            #define MMI_DIALING_BOX_X                               (0)
            #define MMI_DIALING_BOX_Y                               (0)    
            #define MMI_DIALING_BOX_WIDTH                           (0)
            #define MMI_DIALING_BOX_HEIGHT                          (33)
        #endif /* __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__ */
        #if	defined(__PROJECT_GALLITE_C01__)//def __MMI_INTELIGENT_CALL_NUMBER__
                #define DAILING_INPUTBOX_Y      (0)//42
		#define DAILING_INPUTBOX_HEIGHT	(103/*115*/)//75  
		#define DAILING_INPUTBOX_WIDTH  (MMI_CONTENT_WIDTH)
		#define DAILING_DYNAMIC_LIST_Y        (DAILING_INPUTBOX_HEIGHT + DAILING_INPUTBOX_Y)
        #endif /* __MMI_INTELIGENT_CALL_NUMBER__ */
    #elif defined (__MMI_MAINLCD_220X176__)
        /* For function key on idlescreen */
        #if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
            #define MMI_DIALING_KEYPAD_COLOMNS                      (3)
            #define MMI_DIALING_KEYPAD_ROWS                         (4)
            #define MMI_DIALING_KEYPAD_LAYER_WIDTH                  (200)
            #define MMI_DIALING_KEYPAD_LAYER_HEIGHT                 (176)
            #define MMI_DIALING_KEYPAD_LAYER_X                      (0)
            #define MMI_DIALING_KEYPAD_LAYER_Y                      (50)
            #define MMI_DIALING_KEYPAD_X                            (7)
            #define MMI_DIALING_KEYPAD_Y                            (11)
            #define MMI_DIALING_KEYPAD_WIDTH                        (126)
            #define MMI_DIALING_KEYPAD_HEIGHT                       (130)
            #define MMI_DIALING_KEY_WIDTH                           (40)
            #define MMI_DIALING_KEY_HEIGHT                          (31)
            #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (3)
            #define MMI_DIALING_KEY_VERTICAL_GAP                    (3)
            #define MMI_DIALING_BOX_X                               (0)
            #define MMI_DIALING_BOX_Y                               (0)    
            #define MMI_DIALING_BOX_WIDTH                           (0)
            #define MMI_DIALING_BOX_HEIGHT                          (33)
            #ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (1)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (3)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (139)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (15)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (31)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (39)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (0)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (5)
            #else   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */        
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (1)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (2)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (165)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (33)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (37)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (37)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (12)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (27)
            #endif   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */        
        #else /* __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__ */     
            #define MMI_DIALING_KEYPAD_COLOMNS                      (3)
            #define MMI_DIALING_KEYPAD_ROWS                         (4)
            #define MMI_DIALING_KEYPAD_LAYER_WIDTH                  (200)
            #define MMI_DIALING_KEYPAD_LAYER_HEIGHT                 (176)
            #define MMI_DIALING_KEYPAD_LAYER_X                      (0)
            #define MMI_DIALING_KEYPAD_LAYER_Y                      (50)
            #define MMI_DIALING_KEYPAD_X                            (16)
            #define MMI_DIALING_KEYPAD_Y                            (7)
            #define MMI_DIALING_KEYPAD_WIDTH                        (145)
            #define MMI_DIALING_KEYPAD_HEIGHT                       (137)
            #define MMI_DIALING_KEY_WIDTH                           (39)
            #define MMI_DIALING_KEY_HEIGHT                          (31)
            #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (14)
            #define MMI_DIALING_KEY_VERTICAL_GAP                    (5)
            #define MMI_DIALING_BOX_X                               (0)
            #define MMI_DIALING_BOX_Y                               (0)    
            #define MMI_DIALING_BOX_WIDTH                           (0)
            #define MMI_DIALING_BOX_HEIGHT                          (33)
        #endif /* __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__ */		
    #else /* LCD szie */
        #if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
            #define MMI_DIALING_KEYPAD_COLOMNS                      (3)
            #define MMI_DIALING_KEYPAD_ROWS                         (4)
            #define MMI_DIALING_KEYPAD_LAYER_WIDTH                  (128)
            #define MMI_DIALING_KEYPAD_LAYER_HEIGHT                 (160)
            #define MMI_DIALING_KEYPAD_LAYER_X                      (0)
            #define MMI_DIALING_KEYPAD_LAYER_Y                      (MMI_DIALING_BOX_HEIGHT)
            #define MMI_DIALING_KEYPAD_X                            (6)
            #define MMI_DIALING_KEYPAD_Y                            (7)
            #define MMI_DIALING_KEYPAD_WIDTH                        (112)
            #define MMI_DIALING_KEYPAD_HEIGHT                       (110)
            #define MMI_DIALING_KEY_WIDTH                           (27)
            #define MMI_DIALING_KEY_HEIGHT                          (22)
            #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (3)
            #define MMI_DIALING_KEY_VERTICAL_GAP                    (3)
            #define MMI_DIALING_BOX_X                               (0)
            #define MMI_DIALING_BOX_Y                               (0)    
            #define MMI_DIALING_BOX_WIDTH                           (0)
            #define MMI_DIALING_BOX_HEIGHT                          (30)
            /* For function key on idlescreen */
            #ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (1)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (3)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (98)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (14)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (25)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (25)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (4)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (4)
            #else   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */        
                #define MMI_DIALING_KEYPAD_FUNC_COLOMNS             (1)
                #define MMI_DIALING_KEYPAD_FUNC_ROWS                (2)
                #define MMI_DIALING_KEYPAD_FUNC_X                   (98)
                #define MMI_DIALING_KEYPAD_FUNC_Y                   (14)
                #define MMI_DIALING_FUNC_KEY_WIDTH                  (25)
                #define MMI_DIALING_FUNC_KEY_HEIGHT                 (25)
                #define MMI_DIALING_FUNC_KEY_HORIZONTAL_GAP         (4)
                #define MMI_DIALING_FUNC_KEY_VERTICAL_GAP           (4)
            #endif   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */    
        #else /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */
            #define MMI_DIALING_KEYPAD_COLOMNS                      (3)
            #define MMI_DIALING_KEYPAD_ROWS                         (4)
            #define MMI_DIALING_KEYPAD_LAYER_WIDTH                  (128)
            #define MMI_DIALING_KEYPAD_LAYER_HEIGHT                 (160)
            #define MMI_DIALING_KEYPAD_LAYER_X                      (0)
            #define MMI_DIALING_KEYPAD_LAYER_Y                      (MMI_DIALING_BOX_HEIGHT)
            #define MMI_DIALING_KEYPAD_X                            (8)
            #define MMI_DIALING_KEYPAD_Y                            (0)
            #define MMI_DIALING_KEYPAD_WIDTH                        (112)
            #define MMI_DIALING_KEYPAD_HEIGHT                       (110)
            #define MMI_DIALING_KEY_WIDTH                           (31)
            #define MMI_DIALING_KEY_HEIGHT                          (25)
            #define MMI_DIALING_KEY_HORIZONTAL_GAP                  (9)
            #define MMI_DIALING_KEY_VERTICAL_GAP                    (3)
            #define MMI_DIALING_BOX_X                               (0)
            #define MMI_DIALING_BOX_Y                               (0)    
            #define MMI_DIALING_BOX_WIDTH                           (0)
            #define MMI_DIALING_BOX_HEIGHT                          (30)
        #endif /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */
    #endif  /* LCD szie */

 

/* Upper bound of the number of menu items in one screen */
#define	MMI_MAX_MENUITEMS_IN_CONTENT			(MMI_CONTENT_HEIGHT/MMI_MENUITEM_HEIGHT + 2 /* the magic number 2 is for safety. At least 1. */)

#ifdef __MMI_TOUCH_SCREEN__
#define	MMI_PASSWD_KEYBOARD_START_X		0
#define	MMI_PASSWD_KEYBOARD_START_Y		138
#endif

#define	MMI_BG_GREYSCALE_VALUE							(40) 
#define	MMI_BG_GREYSCALE_BLACK_VALUE					(40)
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#if (defined __MMI_NOKIA_STYLE_N800__) ||(defined __MMI_MAINLCD_220X176__) ||(defined __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */	
#define	MMI_EDITOR_BG_OPACITY				(170) 
#else
#define	MMI_EDITOR_BG_OPACITY				(40) 

#endif
 
#if defined(__MMI_DRAW_MANAGER__)

// Macros defined for title bar buttons used in Touch Panel support.
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_320X240__)
    #define MMI_TITLEBAR_BUTTON_GAP                         (22)
    #define MMI_TITLEBAR_BUTTON_WIDTH                       (19)
    #define MMI_TITLEBAR_BUTTON_HEIGHT                      (17)
#else
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined(__MMI_UI_STYLE_4__)
		#define MMI_TITLEBAR_BUTTON_GAP 20
		#define MMI_TITLEBAR_BUTTON_WIDTH 19
		#define MMI_TITLEBAR_BUTTON_HEIGHT 17

	#elif defined(__MMI_UI_STYLE_3__)
		#define MMI_TITLEBAR_BUTTON_GAP 10
		#define MMI_TITLEBAR_BUTTON_WIDTH 15
		#define MMI_TITLEBAR_BUTTON_HEIGHT 15
 
/* This should not be used, or resolution compile option is not correct */
	#else 
		#define MMI_TITLEBAR_BUTTON_GAP 1
		#define MMI_TITLEBAR_BUTTON_WIDTH 15
		#define MMI_TITLEBAR_BUTTON_HEIGHT 15
 

	#endif
#endif	/*+ zhouqin 2011-02-16 modify for q6 320X240 */

#endif
 

#if	defined(__PROJECT_GALLITE_C01__)//def __MMI_INTELIGENT_CALL_NUMBER__//wen 0418
/*******************
 * MainMenu Layout 
 *******************/

#define MMI_MAINMENU_TITLE_X                                (MMI_TITLE_X)
#define MMI_MAINMENU_TITLE_WIDTH                            (MMI_TITLE_WIDTH)
#define MMI_MAINMENU_CONTENT_X                              (MMI_CONTENT_X)
#define MMI_MAINMENU_CONTENT_WIDTH                          (MMI_CONTENT_WIDTH)

#ifdef GUI_MAINMENU_SHOW_STATUS_ICON
#define MMI_MAINMENU_TITLE_Y                                (MMI_STATUS_BAR_HEIGHT)
#else 
    #define MMI_MAINMENU_TITLE_Y                            (0)
#endif

    #define MMI_MAINMENU_TITLE_HEIGHT                       (18)


#define MMI_MAINMENU_CONTENT_Y                              (MMI_MAINMENU_TITLE_Y + MMI_MAINMENU_TITLE_HEIGHT)
#define MMI_MAINMENU_CONTENT_HEIGHT                         (MAIN_LCD_DEVICE_HEIGHT - MMI_BUTTON_BAR_HEIGHT - MMI_MAINMENU_CONTENT_Y )

/*******************
 * SubMenu Layout
 *******************/

#define MMI_SUBMENU_TITLE_X                                 (MMI_TITLE_X)
#define MMI_SUBMENU_TITLE_WIDTH                             (MMI_TITLE_WIDTH)
#define MMI_SUBMENU_CONTENT_X                               (MMI_CONTENT_X)
#define MMI_SUBMENU_CONTENT_WIDTH                           (MMI_CONTENT_WIDTH)

#ifdef GUI_SUBMENU_SHOW_STATUS_ICON
#define MMI_SUBMENU_TITLE_Y                                 (MMI_STATUS_BAR_HEIGHT)
#else 
    #define MMI_SUBMENU_TITLE_Y                             (0)
#endif

    #define MMI_SUBMENU_TITLE_HEIGHT                        (18)

#define MMI_SUBMENU_CONTENT_Y                               (MMI_SUBMENU_TITLE_Y + MMI_SUBMENU_TITLE_HEIGHT)
#define MMI_SUBMENU_CONTENT_HEIGHT                          (MAIN_LCD_DEVICE_HEIGHT - MMI_BUTTON_BAR_HEIGHT - MMI_SUBMENU_CONTENT_Y )

/*******************
 * Common Layout
 *******************/

#define MMI_COMMON_TITLE_X                                  (MMI_TITLE_X)
#define MMI_COMMON_TITLE_WIDTH                              (MMI_TITLE_WIDTH)
#define MMI_COMMON_CONTENT_X                                (MMI_CONTENT_X)
#define MMI_COMMON_CONTENT_WIDTH                            (MMI_CONTENT_WIDTH)

#ifdef GUI_COMMON_SHOW_STATUS_ICON
#define MMI_COMMON_TITLE_Y                                  (MMI_STATUS_BAR_HEIGHT)
#else 
#define MMI_COMMON_TITLE_Y                                  (0)
#endif

    #define MMI_COMMON_TITLE_HEIGHT                         (18)

    
#define MMI_COMMON_CONTENT_Y                                (MMI_COMMON_TITLE_Y + MMI_COMMON_TITLE_HEIGHT)
#define MMI_COMMON_CONTENT_HEIGHT                           (MAIN_LCD_DEVICE_HEIGHT - MMI_BUTTON_BAR_HEIGHT - MMI_COMMON_CONTENT_Y )

#endif


/*
** Extern Global Variable
*/
extern U8 MMI_disable_title_shortcut_display;

extern S32	MMI_status_bar_height;
extern S32	MMI_content_x;
extern S32	MMI_content_y;
extern S32	MMI_content_width;
extern S32	MMI_content_height;
extern S32	MMI_pop_up_dialog_x;
extern S32	MMI_pop_up_dialog_y;
extern S32	MMI_pop_up_dialog_width;
extern S32	MMI_pop_up_dialog_height;
extern S32	MMI_pop_up_dialog_full_height;

/*
** Extern Global Function
*/

extern void wgui_init(void);
extern void MMI_dummy_function(void);
extern void register_keyboard_key_handler(void (*f)(S32 vkey_code,S32 key_state));
extern void register_keyboard_input_handler(void (*f)(S32 vkey_code));
extern void register_keyboard_global_key_handler(void (*f)(S32 vkey_code,S32 key_state));
extern void register_keyboard_global_input_handler(void (*f)(S32 vkey_code));
extern void clear_keyboard_key_handler(void);
extern void clear_keyboard_input_handler(void);
extern void clear_keyboard_global_key_handler(void);
extern void clear_keyboard_global_input_handler(void);

/*	key handling wrappers	*/

extern void clear_key_handlers(void);
extern void clear_category_screen_key_handlers(void);

extern void register_key_input_handler(void (*f)(MMI_key_code_type key_code,MMI_key_event_type key_event));
extern void register_key_up_handler(void (*f)(MMI_key_code_type key_code));
extern void register_key_down_handler(void (*f)(MMI_key_code_type key_code));
extern void register_MMI_key_input_handler(void);
extern void register_MMI_stroke_key_input_handler(void);
#ifdef __MMI_MULTITAP_THAI__
extern void register_MMI_key_input_extend_handler(void);
#endif
#ifdef __MMI_FULL_KEYPAD__
extern void setCharSet(U8 charset);
extern U8 getCharSet(void);
extern void MMI_Shift_Key_Pressed(void);
extern void setShiftOn(BOOL onoff);
extern UI_character_type MMI_get_char_by_key_code(S32 key, U8 charset);
extern void MMI_clear_all_input_key_handler(void);
#endif

extern void clear_key_input_handler(void);
extern void clear_key_down_handler(void);
extern void clear_key_up_handler(void);
extern void clear_MMI_key_input_handler(void);

extern void MMI_key_input_handler(MMI_key_code_type key_code,MMI_key_event_type key_event);

void fmsr_increment_ticks(UI_time *t);
BOOL UI_is_timer_start(void (*callback)(void));
#ifdef __cplusplus
};
#endif

#endif
