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
 *	MMI_features_GUI.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   This file is intends for GUI features definition.
 * Author:
 * -------
 * -------
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef __MMI_FEATURES_GUI__
#define __MMI_FEATURES_GUI__

#ifndef __MMI_FEATURES__
	#error "Please Do Not include MMI_feautres_GUI.h directly. Include MMI_features.h first."
#endif

#include "guitypedef.h"



/*****************************************************************************
 [Framework]: GUI
 *****************************************************************************/

//#define __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__						/* Detail Info: [Support matrix main menu with full background. Title bar is an option] */
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	//#define __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__							/* Detail Info: [Support matrix main menu without title bar] */
#endif

#define __MMI_STATUS_BAR_NO_UNDERLINE__									/* Detail Info: []*/

#if !defined(__MMI_LITE_DISPLAY__)
	#define __MMI_ANIMATED_WALLPAPERS__										/* Detail Info: []*/
#endif

#if defined(WAP_SUPPORT)
	#define __MMI_WCSS_INPUT_FORMAT_SUPPORT__							/* Detail Info: [Editor support for -wap-input-format CSS format]*/
#endif

//#define __MMI_UI_SMALL_SCREEN_SUPPORT__ 									/* Detail Info: [For small screen support such as popup screen]*/
#if defined __MMI_UI_SMALL_SCREEN_SUPPORT__
   #if !defined(__MMI_TOUCH_SCREEN__) && (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__))
   	#define UI_SMALL_LIST_SCREEN											/* Detail Info: [For Small List Screen]*/
   	#define UI_SMALL_CATEGORY_EDITOR										/* Detail Info: [For Small Editor Screen]*/
   	#define UI_SMALL_PIN_EDITOR_SCREEN								   /* Detail Info: [For Small PIN editor Screen]*/
   	#if defined (__MMI_MESSAGES_EMS__) 
   		#define UI_SMALL_IMAGE_SELECTOR_SCREEN							/* Detail Info:	[For Small Image Select Screen]*/
   	#endif	
   #endif
#endif

#if defined (__MMI_TOUCH_DIAL_SCREEN__) && defined (__MMI_MAINLCD_240X320__)
	#define __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__					/* Detail Info: [Touch Dialing Screen with function keys] */
#elif defined (__MMI_TOUCH_DIAL_SCREEN__) && defined(__MMI_MAINLCD_220X176__)
	#define __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__
#elif defined (__MMI_TOUCH_DIAL_SCREEN__) && defined(__MMI_MAINLCD_176X220__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_) 
	#define __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__					/* Detail Info: [Touch Dialing Screen with function keys] */
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#elif defined (__MMI_TOUCH_DIAL_SCREEN__) && defined(__MMI_MAINLCD_320X240__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_) 
	#define __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__	
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif

#if ((MMI_SIM_NUMBER == 2) && defined(__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__))
	#if !defined(__DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
    	#define __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
	#endif
#endif
/* For transition effects */
#ifndef __MMI_DRAW_MANAGER__
	//#define __MMI_UI_TITLE_TRANSITION__									/* Detail Info: Title transition */
	//#define __MMI_UI_LIST_TRANSITION__										/* Detail Info: List transition */
#endif

#ifdef __MMI_TOUCH_SCREEN__
	#define __MMI_UI_HINT_TOGGLE_TRANSITION__							/* Detail Info: Play transition animation when hint text is changed */
#else
    #if !defined(__MMI_MAINLCD_128X128__) && !defined(__MMI_MAINLCD_128X160__) && !defined(__MMI_MAINLCD_160X128__)
	    #define __MMI_UI_HINTS_IN_MENUITEM__									/* Detail Info: Double lines of menuitem */
	#endif
#endif 

#if !defined(__MMI_LITE_DISPLAY__)
//	#define __MMI_UI_TRANSPARENT_EFFECT__									/* Detail Info: Transparent highlight of list menu*/
//	#define __MMI_UI_LIST_HIGHLIGHT_EFFECTS__  								/* Detail Info: [List Effect type, should be one of the following. TRANSPARENCY , FLASH_FEELING , ANIMATION , NONE  ] */
#endif

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__										/* The style can be one of the following: HIGHLIGHT_EFFECT_TRANSPARENCY, HIGHLIGHT_EFFECT_FLASH_FEELING, HIGHLIGHT_EFFECT_ANIMATION, HIGHLIGHT_EFFECT_NONE */
	#define __MMI_UI_LIST_HIGHLIGHT_EFFECT_TYPE__		HIGHLIGHT_EFFECT_FLASH_FEELING
#endif

//wuzc del for phk no need for default text 2008-04-24
#if 0
#define __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__						/* Detail Info: [Inline edit default text effect]*/
#endif


#ifdef DEMO_PROJECT
//#define __MMI_UI_TWO_LINE_MENUITEM_STYLES__								/* Detail Info: [2 line menuitem with Inline Select effect]*/
#endif

#define __MMI_COLOR_DIALLING_FONT__										/* Detail Info: []*/

#ifdef __MMI_MAINLCD_176X220__
 //  #define __MMI_16X16_CHINESE_FONT__   									/* Detail Info: []*/
#endif

/* Scrollbar style: if no option is defined, default style is automatically set. */
// #define __MMI_UI_SCROLLBAR_DEFAULT_STYLE_3__									/* Detail Info: [scrollbar styles requiring addition images] */
#define __MMI_UI_SCROLLBAR_DEFAULT_STYLE_2__									/* Detail Info: [Rounded bar with arrows] */
// #define __MMI_UI_SCROLLBAR_DEFAULT_STYLE_1__									/* Detail Info: [old style flat scrollbar] */

/* Popup description(hint) style: if no option is defined, default style is automatically set. */
// #define __MMI_UI_POPUP_DESCRIPTION_DEFAULT_STYLE_3__					    /* Detail Info: [Balloon style] */
 #define __MMI_UI_POPUP_DESCRIPTION_DEFAULT_STYLE_2__						/* Detail Info: [Round corner with border and shadow] */
// #define __MMI_UI_POPUP_DESCRIPTION_DEFAULT_STYLE_1__						/* Detail Info: [Old-style flat] */




#define __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__					/* Detail Info: [Enable signal strength status icon combine with line switch]*/
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined (__MMI_DRAW_MANAGER__) && (defined (__MMI_MAINLCD_240X320__) ||defined (__MMI_MAINLCD_320X240__))
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#define __MMI_UI_CALENDAR_WITH_INFO_BOX__							/* Detail Info: [Enable an infomation box embedded in calendar]*/
#endif

#if !defined(__MMI_MAINLCD_128X128__) && !defined(__MMI_MAINLCD_128X160__) && !defined(__MMI_MAINLCD_160X128__)
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-4-14 11:7 */
#if 0
#define __MMI_UI_STATUS_BAR_AT_BOTTOM__									/* Detail Info: [Idlescreen shows status icon on bottom and draw while segment lines]*/
#endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-4-14 11:7 */
#endif



/* Macros added for Dalmatian UI Style Start */
#define __MMI_UI_SOFTKEY_STYLE__	SOFTKEY_STYLE_NONE						/* The style can be one of the following: SOFTKEY_STYLE_NONE, SOFTKEY_STYLE_DALMATIAN, SOFTKEY_STYLE_TECHNO */

#define __MMI_UI_STATUSBAR_STYLE__	STATUSBAR_STYLE_NONE				/* The style can be one of the following: STATUSBAR_STYLE_NONE, STATUSBAR_STYLE_DALMATIAN, STATUSBAR_STYLE_TECHNO */

#ifdef __MMI_MAINLCD_176X220__
	//#define __MMI_UI_DALMATIAN_FIXEDLIST__									/* Detail Info: [drawing lines and grids for list and menuitems] */
	#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
		#define __MMI_UI_MM_LIST_NO_GRID__								/* Detail Info: [Don't draw grid on List main menu]*/
	#endif
	//#define __MMI_UI_DALMATIAN_EDITOR__									/* Detail Info: [showing a background image if there is no input] */
#endif

#ifdef __MMI_MAINLCD_220X176__
	//#define __MMI_UI_DALMATIAN_FIXEDLIST__									/* Detail Info: [drawing lines and grids for list and menuitems] */
	#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
		#define __MMI_UI_MM_LIST_NO_GRID__								/* Detail Info: [Don't draw grid on List main menu]*/
	#endif
	#define __MMI_UI_DALMATIAN_EDITOR__									/* Detail Info: [showing a background image if there is no input] */
#endif


#if (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_128X160__)||defined(__MMI_MAINLCD_220X176__)||defined(__MMI_MAINLCD_160X128__)) && !defined(__MMI_DRAW_MANAGER__)
	#define  __MMI_UI_DALMATIAN_CALENDAR__
	#ifdef __MMI_UI_DALMATIAN_CALENDAR__
		#define __MMI_UI_TRANSPARENT_EFFECT_IN_DALMATIAN_CALENDAR__
	#endif
#endif
/* Macros added for Dalmatian UI Style End */



/*  Macros added for Techno UI Style Start */
#ifdef __MMI_MAINLCD_240X320__
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-4-14 16:42 */
		#if 0
		#define __MMI_UI_TECHNO_IDLESCREEN_BAR__						/* Detail Info: [Techno styled idle screen bar containing clock]*/
		#endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-4-14 16:42 */
	#if defined (__MMI_TOUCH_SCREEN__)
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-3-14 15:22 */
		#if 0
		#define __MMI_TOUCH_IDLESCREEN_SHORTCUTS__						/* Detail Info: [Display Idle Screen shortcuts]*/
		#endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-3-14 15:22 */
	#endif
#endif

#ifndef __MMI_UI_TECHNO_STATUS_ICON__
	//#define __MMI_UI_ANIMATED_TITLE_EFFECT__								/* Detail Info: [Title Effect type, should be one of the following. ONCE , CONTINOUS ] */
#endif
#ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__										/* The style can be one of the following: ONCE, CONTINOUS */
	 #define __MMI_TITLE_EFFECT_TYPE__ ONCE
#endif
/*  Macros added for Techno UI Style End */



/* Mainmenu-related definition start */
/* Detail Info: [MainMenu type, should be one of the following. LIST , MATRIX , CIRCULAR_3D , PAGE , FIXED_GRID , ANI_BG_LIST] */
#define __MMI_VERSION_2__													/* Detail Info: [Turn on 12-grid main menu]*/

#if defined (__MMI_TOUCH_SCREEN__)

	//#define __MMI_BI_DEGREE_MAIN_MENU_STYLE__							/* Detail Info: [For Two Level main menu ] */
	//longfei add 
#endif
//#if	!defined( __MMI_BI_DEGREE_MAIN_MENU_STYLE__) && !defined(__MMI_MAINLCD_240X320__)

#if	!defined( __MMI_BI_DEGREE_MAIN_MENU_STYLE__)//082806 240x320 matrix
	#define __MMI_MATRIX_MAIN_MENU_OPTIMIZE__  
#if !defined(__MMI_RESOURCE_SLIM_ON_128X160__) && !defined(__MMI_MAINLCD_128X160__)	&& !defined(__MMI_MAINLCD_128X128__)				/* Detail Info: [For Speed Up matrix mainmenu display] */
	#define __MMI_MAINMENU_STYLE_CHANGE_EN__
#endif							/* Detail Info: [Enable changeable main menu style feature]*/
#ifdef __MMI_MAINLCD_128X128__
    #define __MMI_MAINMENU_TYPE__ PAGE 	
#else
    #define __MMI_MAINMENU_TYPE__ MATRIX 									/* Detail Info: [MainMenu type, should be one of the following. MATRIX , PAGE , LIST , CIRCULAR_3D , ROTATE , TAB , FIXED_GRID , ANI_BG_LIST] */
#endif																			/* Detail Info: [The default main menu type you chosed should be one of the following supported types] */
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_320X240__)
        #define __MMI_WALLPAPER_ON_BOTTOM__
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#ifdef __MMI_MAINLCD_220X176__ 
	#define __MMI_MAINMENU_STYLE_CHANGE_EN__
	#define __MMI_UI_CALENDAR_WITH_INFO_BOX__
	#define __MMI_WALLPAPER_ON_BOTTOM__
#else
	#define __MMI_UI_LIST_CACHE_BACKGROUND__							/* Defail Info: [cache list menu background in anther GDI layer. */
	#endif

	#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__								/* Detail Info: [Turn off the circular main menu style in changeable main menu style]*/
		#define __MMI_MAINMENU_MATRIX_SUPPORT__
		#define __MMI_MAINMENU_LIST_SUPPORT__
	#if !defined (__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__) && !defined(__MMI_MAINLCD_220X176__)
		#define __MMI_MAINMENU_PAGE_SUPPORT__
		#define __MMI_MAINMENU_CIRCULAR_SUPPORT__
		#define __MMI_MAINMENU_ROTATE_SUPPORT__
	#endif
		#if !defined (__MMI_TOUCH_SCREEN__) && !defined (__MMI_DRAW_MANAGER__)
			//#define __MMI_MAINMENU_TAB_SUPPORT__							/* Detail Info: Tab based mainmenu support*/
			#ifdef __MMI_MAINMENU_TAB_SUPPORT__
				#define __MMI_UI_TITLE_TRANSITION__
			#endif
		#endif
	#else
		#if defined(__MMI_UI_TECHNO_MAINMENU__) || defined(__MMI_UI_DALMATIAN_MAINMENU__)
			#undef __MMI_MAINMENU_TYPE__
			#define __MMI_MAINMENU_TYPE__ PAGE
		#endif
	#endif /* __MMI_MAINMENU_STYLE_CHANGE_EN__ */
#else
	#define __MMI_MAINMENU_TYPE__ LIST									/* Detail Info: [MainMenu type, should be one of the following. LIST , MATRIX , CIRCULAR_3D , PAGE , FIXED_GRID , ANI_BG_LIST] */
#endif
											
#ifndef __MMI_UI_DALMATIAN_MAINMENU__
	#define __MMI_UI_MM_HORIZONTAL_PAGE_STYLE__						/* Detail Info: Horizontally navigate PAGE main menu */
#endif
/* Mainmenu definition end */


#ifdef __MMI_MAINLCD_220X176__ 
	#define __MMI_UI_CALENDAR_WITH_INFO_BOX__
	#define __MMI_WALLPAPER_ON_BOTTOM__
#else
//	#define __MMI_WALLPAPER_ON_BOTTOM__
	#define __MMI_UI_LIST_CACHE_BACKGROUND__							/* Defail Info: [cache list menu background in anther GDI layer. */
#endif
#ifdef __MMI_EBOOK_READER__												
	#define __UI_PAGING_IN_MULTILINE_EDITOR__							/* Defail Info: [Supports paging in multiline editor in view mode] */
#endif
#if (!defined(__PROJECT_GALLITE_C01__) ) && (!defined(__PRJ_WITH_SPILCD__))
#define __MMI_CHANGE_WIN_WITH_DYNAMIC__
#endif
//#define __SG_MMI_MEDIA_SPECIAL_MENU__
#endif //__MMI_FEATURES_GUI__
