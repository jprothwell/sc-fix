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
 *   gui_config.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   GUI Configuration
 * Author:
 * -------
 * -------
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
   Filename:      gui_config.h
   Date Created:  August-26-2002
   Contains:      Configuration for the UI system
**********************************************************************************/

#ifndef __GUI_CONFIG_H__
#define __GUI_CONFIG_H__

#include "mmi_features.h"

/* *FORMATTER-DISABLE* */

/**********************************************************************************************/
/*                                                     Dependency and Exclusive Rules								          */
/*                                                   Please Do Not Modify under this Line								   */
/**********************************************************************************************/

/*************************** Dependency Rule of Tab Support ***************************************/
/**/#if defined(__MMI_MAINMENU_TAB_SUPPORT__) || defined(__MMI_UI_CALL_LOG_IN_TAB_PANE__)		/**/
/**/	#define __MMI_UI_TAB_PANE__														/**/
/**/#endif																					/**/
/***************************** Do Not Modifiy ****************************************************/

/*************************** Dependency Rule of Wallpaper and List Background  ***********************/
/**/#if defined(__MMI_UI_LIST_CACHE_BACKGROUND__) && defined(__MMI_WALLPAPER_ON_BOTTOM__)	/**/
/**/	#undef __MMI_UI_LIST_CACHE_BACKGROUND__											/**/
/**/#endif																					/**/
/***************************** Do Not Modifiy ****************************************************/

/*************************** Dependency Rule of Two Line Menuitem  ***********************/
/**/#if !defined(__MMI_UI_TWO_LINE_MENUITEM_STYLES__) && (defined(__MMI_PHB_TWO_LINE_MENUITEM_DISPLAY__) || defined(__MMI_PHNSET_TWO_LINE_MENUITEM_DISPLAY__) || defined(__MMI_PROFILES_TWO_LINE_MENUITEM_DISPLAY__))
/**/#define __MMI_UI_TWO_LINE_MENUITEM_STYLES__
/**/#endif																					/**/
/***************************** Do Not Modifiy ****************************************************/

/*************************** Dependency Rule of Highlight Effect *************************************/
/**/#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__													/**/
/**/	#ifndef	__MMI_UI_LIST_HIGHLIGHT_EFFECT_TYPE__										/**/
/**/		#error "Please define __MMI_UI_LIST_HIGHLIGHT_EFFECT_TYPE__ in MMI_features_GUI.h"		/**/
/**/	#endif																				/**/
/**/	#if		__MMI_UI_LIST_HIGHLIGHT_EFFECT_TYPE__	==	HIGHLIGHT_EFFECT_TRANSPARENCY	/**/
/**/		#define UI_LIST_EFFECT_TYPE TRANSPARENCY										/**/
/**/		#define __MMI_UI_TRANSPARENT_EFFECT__										/**/
/**/	#elif	__MMI_UI_LIST_HIGHLIGHT_EFFECT_TYPE__	==	HIGHLIGHT_EFFECT_FLASH_FEELING	/**/
/**/		#define UI_LIST_EFFECT_TYPE FLASH_FEELING										/**/
/**/	#elif	__MMI_UI_LIST_HIGHLIGHT_EFFECT_TYPE__	==	HIGHLIGHT_EFFECT_ANIMATION		/**/
/**/		#define UI_LIST_EFFECT_TYPE ANIMATION											/**/
/**/	#elif	__MMI_UI_LIST_HIGHLIGHT_EFFECT_TYPE__	==	HIGHLIGHT_EFFECT_NONE			/**/
/**/		#define UI_LIST_EFFECT_TYPE NONE												/**/
/**/	#else																				/**/
/**/		#error "Please define __MMI_UI_LIST_HIGHLIGHT_EFFECT_TYPE__ in MMI_features_GUI.h"		/**/
/**/	#endif																				/**/
/**/#endif																					/**/
/***************************** Do Not Modifiy ****************************************************/

/*************************** Dependency Rule of Softkey*******************************************/
/**/#ifndef	__MMI_UI_SOFTKEY_STYLE__														/**/			
/**/	#error "Please define __MMI_UI_SOFTKEY_STYLE__ in MMI_features_GUI.h"						/**/
/**/#endif																					/**/
/**/#if		__MMI_UI_SOFTKEY_STYLE__	==	SOFTKEY_STYLE_DALMATIAN						/**/
/**/	#if defined(__MMI_MAINLCD_176X220__) && !defined(__MMI_WGUI_CSK_ENABLE__)				/**/
/**/		#define __MMI_UI_DALMATIAN_SOFTKEYS__										/**/	/* Detail Info: [2 layers of softkeys that can shuffle] */
/**/		#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__									/**/
/**/	#elif defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_WGUI_CSK_ENABLE__)			/**/
/**/		#define __MMI_UI_DALMATIAN_SOFTKEYS__										/**/	/* Detail Info: [2 layers of softkeys that can shuffle] */
/**/		#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__									/**/
/**/	#else																				/**/
/**/		#undef __MMI_UI_SOFTKEY_STYLE__													/**/
/**/		#define __MMI_UI_SOFTKEY_STYLE__ SOFTKEY_STYLE_NONE							/**/
/**/	#endif																				/**/
/**/#elif	__MMI_UI_SOFTKEY_STYLE__	==	SOFTKEY_STYLE_TECHNO							/**/
/**/	#if defined(__MMI_MAINLCD_176X220__) && !defined(__MMI_WGUI_CSK_ENABLE__)				/**/
/**/		#define __MMI_UI_TECHNO_SOFTKEYS__											/**/	/* Detail Info: [special softkey theme] */
/**/		#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__									/**/
/**/	#elif defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_WGUI_CSK_ENABLE__)			/**/
/**/		#define __MMI_UI_TECHNO_SOFTKEYS__											/**/	/* Detail Info: [special softkey theme] */
/**/		#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__									/**/
/**/	#else																				/**/
/**/		#undef __MMI_UI_SOFTKEY_STYLE__													/**/
/**/		#define __MMI_UI_SOFTKEY_STYLE__ SOFTKEY_STYLE_NONE							/**/
/**/	#endif																				/**/
/**/#endif																					/**/
/**/#if		__MMI_UI_SOFTKEY_STYLE__	==	SOFTKEY_STYLE_NONE								/**/
/**/	#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_128X160__)	||defined(__MMI_MAINLCD_220X176__) || defined(__MMI_MAINLCD_160X128__)	/**/
/**/		#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_2__									/**/	/* Detail Info: [Use image as background] */
/**/	#else																				/**/
/**/		#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__									/**/	/* Detail Info: [Old-style flat bar] */
/**/	#endif																				/**/
/**/#endif																					/**/
/***************************** Do Not Modifiy ****************************************************/

/*************************** Dependency Rule of Statusbar *****************************************/
/**/#ifndef	__MMI_UI_STATUSBAR_STYLE__														/**/						
/**/	#error "Please define __MMI_UI_STATUSBAR_STYLE__ in MMI_features_GUI.h"					/**/
/**/#endif																					/**/
/**/#if defined(__MMI_MAINLCD_240X320__)														/**/
/**/		#undef __MMI_UI_STATUSBAR_STYLE__												/**/
/**/		#define __MMI_UI_STATUSBAR_STYLE__ STATUSBAR_STYLE_TECHNO					/**/
/**/#endif																					/**/															
/**/#if		__MMI_UI_STATUSBAR_STYLE__	==	STATUSBAR_STYLE_DALMATIAN						/**/
/**/	#if defined(__MMI_MAINLCD_176X220__) ||defined(__MMI_MAINLCD_220X176__)                		/**/
/**/		#define __MMI_UI_DALMATIAN_STATUSBAR__										/**/	/* Detail Info: [2 layers of status bars that can shuffle] */
/**/	#else																				/**/
/**/		#undef __MMI_UI_STATUSBAR_STYLE__												/**/
/**/		#define __MMI_UI_STATUSBAR_STYLE__ STATUSBAR_STYLE_NONE						/**/
/**/	#endif																				/**/
/**/#elif	__MMI_UI_STATUSBAR_STYLE__	==	STATUSBAR_STYLE_TECHNO							/**/
/**/	#if defined(__MMI_MAINLCD_176X220__)||defined(__MMI_MAINLCD_220X176__)                        	/**/
/**/		#define __MMI_UI_TECHNO_STATUS_ICON__										/**/	/* Detail Info: [vertical signal strength and battery level status icons], force __MMI_UI_TECHNO_IDLESCREEN_BAR__ */
/**/		#ifndef __MMI_UI_TECHNO_IDLESCREEN_BAR__										/**/
/**/			#define __MMI_UI_TECHNO_IDLESCREEN_BAR__								/**/	/* Detail Info: [Techno styled idle screen bar containing clock]*/
/**/		#endif																			/**/
/**/	#else																				/**/
/**/		#undef __MMI_UI_STATUSBAR_STYLE__												/**/
/**/		#define __MMI_UI_STATUSBAR_STYLE__ STATUSBAR_STYLE_NONE						/**/
/**/	#endif																				/**/
/**/#endif																					/**/
/**/																						/**/
/**/#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) && defined(__MMI_UI_DALMATIAN_SOFTKEYS__) 	/**/
/**/	#define __MMI_UI_DALMATIAN_IDLESCREEN__											/**/	/* Detail Info: [idle screen with __MMI_UI_DALMATIAN_STATUSBAR__& __MMI_UI_DALMATIAN_SOFTKEYS__] */
/**/#endif																					/**/
/***************************** Do Not Modifiy ****************************************************/

/***************************** Dependency Rule of Status Bar on Bottom *****************************/
/**/#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) && !defined(__MMI_ANALOG_CLOCK__)
/**/	#error "Please define __MMI_ANALOG_CLOCK__ in MMI_features.h"
/**/#endif
/***************************** Do Not Modifiy ****************************************************/

/***************************** Dependency Rule of Status Bar on Bottom *****************************/
/**/#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_TECHNO_STATUS_ICON__) || defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) || defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__)
/**/	#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__												/**/
/**/		//#error "__MMI_UI_STATUS_BAR_AT_BOTTOM__ should not be defined in MMI_features_GUI.h because of dependency"
/**/		#undef __MMI_UI_STATUS_BAR_AT_BOTTOM__											/**/
/**/	#endif																				/**/
/**/#endif																					/**/
/***************************** Do Not Modifiy ****************************************************/

/**********************************************************************************************/
/*                       End of Dependency and Exclusive Rules								          */
/**********************************************************************************************/

/* *FORMATTER-ENABLE* */

#define __MMI_PLUTO_GPRS__
//#define __MMI_PLUTO_LISTBOX__
//#define __MMI_PLUTO_DOUBLEPX_ROUND_CORNER__
#define __MMI_INSCREEN_MULTILINE_TEXTBOX__
#ifdef __MMI_INSCREEN_MULTILINE_TEXTBOX__
#define INSCREEN_MUTLILINE_TITLE_ENABLE   1
#define INSCREEN_MULTILINE_SHOW_TITLE  1        /* 0  //Putting it 0 will display info bar. */
#endif /* __MMI_INSCREEN_MULTILINE_TEXTBOX__ */ 
#define __NEW_MULTI_LINE_INPUTBOX__

/* End */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

    /* Use this parameter to change the device's color depth */
    /* currently supported values are 1,2,16,24,32           */
    /* Note: This is different from the GUIs color depth     */

#ifdef __BW_MMI__
#ifdef _WIN32
#define DEVICE_COLOR_DEPTH          24
#else 
#define DEVICE_COLOR_DEPTH          1
#endif 
#else /* __BW_MMI__ */ 
#define DEVICE_COLOR_DEPTH          24
#endif /* __BW_MMI__ */ 

    /* Srollbar style                                                    */
    /* It can be defined in MMI_features.h or automatically decided here */
    /* Please note that UI_SCROLLBAR_STYLE_3 need additional images      */

#if defined(__MMI_UI_SCROLLBAR_DEFAULT_STYLE_3__)

    /* Thick scrollbar using predefined image */
#define UI_SCROLLBAR_STYLE_3

#elif defined(__MMI_UI_SCROLLBAR_DEFAULT_STYLE_2__)

    /* Rounded bar with arrows */
#define UI_SCROLLBAR_STYLE_2

#elif defined(__MMI_UI_SCROLLBAR_DEFAULT_STYLE_1__)

    /* Old-style flat scrollbar style */
#define UI_SCROLLBAR_STYLE_1

#elif !defined(MT6205B) && !defined(MT6205) && !defined(__MMI_MAINLCD_128X128__)

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_MAINLCD_240X320__)
#define UI_SCROLLBAR_STYLE_3
#else 
#define UI_SCROLLBAR_STYLE_2
#endif 

#else  

    /* MT6205 can also turn on the new UI styles but it is slightly slower and 
     * larger on 128x128. 
     * Developers can decide whether the performance degration is acceptable */
#define UI_SCROLLBAR_STYLE_1

#endif  

/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#ifdef __MMI_BLACK_BERRY_MAIN_MENU__
#define UI_SCROLLBAR_WIDTH       16
#else
/*- zhouqin 2011-02-16 modify for q6 320X240 */	
#if defined(UI_SCROLLBAR_STYLE_3)
    /* UI_SCROLLBAR_STYLE_3 use predefined image.
       If this value is changed, the scrollbar image should be changed, too. */

#ifdef __MMI_MAINLCD_240X320__
#define UI_SCROLLBAR_WIDTH       16
#else 
#define UI_SCROLLBAR_WIDTH       14
#endif 

#elif defined(__MMI_MAINLCD_240X320__)
#define UI_SCROLLBAR_WIDTH       10
#elif defined(__MMI_MAINLCD_176X220__)
#define UI_SCROLLBAR_WIDTH       8
#elif defined(__MMI_MAINLCD_220X176__)

#define UI_SCROLLBAR_WIDTH       12



#else 
#define UI_SCROLLBAR_WIDTH       6
#endif 
#endif/*+ zhouqin 2011-02-16 modify for q6 320X240 */

    /* Popup description style                                                 */
    /* It can be defined in MMI_features.h or automatically decided here.      */
    /* Please note that style 2 and 3 always use border color   */

#if defined(__MMI_UI_POPUP_DESCRIPTION_DEFAULT_STYLE_3__)

    /* Round corner with border and small arrow */
#define UI_POPUP_DESCRIPTION_STYLE_3

#elif defined(__MMI_UI_POPUP_DESCRIPTION_DEFAULT_STYLE_2__)

    /* Round corner with border and shadow */
#define UI_POPUP_DESCRIPTION_STYLE_2

#elif defined(__MMI_UI_POPUP_DESCRIPTION_DEFAULT_STYLE_1__)

    /* Old-style flat popup */
#define UI_POPUP_DESCRIPTION_STYLE_1

#elif !defined(MT6205B) && !defined(MT6205)

#define UI_POPUP_DESCRIPTION_STYLE_3

#else 

    /* MT6205 can also turn on the new UI styles. 
       Developers can decide whether the performance degration is acceptable */
#define UI_POPUP_DESCRIPTION_STYLE_1

#endif 

#if defined(__MMI_UI_SOFTKEY_DEFAULT_STYLE_2__)

    /* Use IMG_SOFTKEY_BAR as background image */
#define UI_SOFTKEY_STYLE_2

#elif defined(__MMI_UI_SOFTKEY_DEFAULT_STYLE_1__)

    /* Use gradient color */
#define UI_SOFTKEY_STYLE_1

#else 

    /* Use style 1 as default */
#define UI_SOFTKEY_STYLE_1

#endif 

    /* Use this parameter to specify that the device uses an */
    /* Indexed color system. Normally color depths <=8 use an   */
    /* Indexed color system.                           */
    /* Valid values are 0 and 1                        */
#define DEVICE_INDEXED_COLOR_SYSTEM    0

    /* Use this parameter to specify that the GUI uses an    */
    /* Indexed color system. Normally color depths <=8 use an   */
    /* Indexed color system                         */
    /* Valid values are 0 and 1                        */
#define UI_INDEXED_COLOR_SYSTEM        0

    /* Use this parameter to change the GUI color depth      */
    /* currently supported values are 1,2,16,24,32           */
    /* Note: This is the color depth used internally by the  */
    /* GUI. This is automatically converted to the device    */
    /* depth before displaying                         */
#define UI_COLOR_DEPTH              24

    /* Support for Keyboard: Simulator / Hardware            */
    /* Note: The simulator uses PC keyboard               */
    /* Valid values are 0 and 1                        */
#define UI_KEYBOARD_SUPPORT            1

    /* Support for Pointing device: Simulator / Hardware     */
    /* Note: The simulator uses PC mouse                  */
    /* Valid values are 0 and 1                        */
    /* Not available yet                            */
#define UI_POINTING_DEVICE_SUPPORT     0

    /* Support for Double buffering in the UI             */
    /* Valid values are 0 and 1. 1=Double buffering enabled  */
#define UI_DOUBLE_BUFFER_SUPPORT    1

    /* Support for two way or four way navigation            */
    /* These values should not be changed                 */
#define  UI_TWO_WAY_NAVIGATION_KEYS    1
#define UI_FOUR_WAY_NAVIGATION_KEYS    2
     
    /* ISSUE GPRS PPT 6 BOTTOM NAVIAGTION KEYS */
#define ENABLE_BOTTOM_NAVIGATION_KEYS  1
    /* ISSUE GPRS PPT 6 END */
    /* Support for two way or four way navigation            */
    /* Valid values are one of the above navigation types    */
#define UI_NAVIGATION_KEYS_TYPE        UI_FOUR_WAY_NAVIGATION_KEYS

    /* Quick startup build: For simulator testing            */
    /* Valid values are 0 and 1. If 1, quick start is enabled   */
#define BUILD_TYPE_X86WIN32_QUICK_START         0

    /* Disable redraw: For simulator testing              */
    /* Valid values are 0 and 1. If 1, Redraw is disabled    */
#define BUILD_TYPE_X86WIN32_DISABLE_REDRAW      0

    /* Double buffered graphics in Win32 platform            */
    /* Valid values are 0 and 1. 1=Double buffering enabled  */
#define BUILD_TYPE_X86WIN32_DOUBLE_BUFFERED     1

    /* 1: Simulates HW graphics on the simulator                      */
    /* 0: Uses simulator graphics                                  */
    /* Set this to 0 to build on the hardware */
#define WINGUI_SIMULATE_HW          0

    /* Supported Build Types                           */
    /* Value must be a word                         */
#define BUILD_TYPE_X86WIN32            0x00A1
#define BUILD_TYPE_HW_PLUTO            0x00A2

    /* Current MMI build type:                         */
    /* Value must be one of the above build types            */
#ifdef MMI_ON_WIN32
#define MMI_BUILD_TYPE              BUILD_TYPE_X86WIN32
#else 
#define MMI_BUILD_TYPE              BUILD_TYPE_HW_PLUTO
#endif 

    /* GUI objects, feature support                    */

    /* Image fill support (bitmap textures) in filled areas  */
    /* Valid values are 0 and 1                        */
#define UI_BITMAP_FILL_SUPPORT         0

    /* Gradient color fill support in filled areas           */
    /* Valid values are 0 and 1                        */
#define UI_GRADIENT_COLOR_FILL_SUPPORT 0

    /* Horizontal scrolling and Horizontal scrollbar support */
    /* in multi-line input box. Valid values are 0 and 1     */
#define UI_MULTILINE_INPUTBOX_HSCROLL  1

    /* Main menu category types                        */
#define UI_MAIN_MENU_CATEGORY_TYPE1    1
#define UI_MAIN_MENU_CATEGORY_TYPE2    2
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
#define UI_MAIN_MENU_CATEGORY_TYPE3    3         
#endif 

    /* Main menu category type: specifies what type of main  */
    /* menu will be displayed by the GUI                  */
    /* Valid values are one of the above main menu types     */
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
#define UI_MAIN_MENU_CATEGORY_TYPE     UI_MAIN_MENU_CATEGORY_TYPE3       
#else 
#define UI_MAIN_MENU_CATEGORY_TYPE     UI_MAIN_MENU_CATEGORY_TYPE2
#endif 

    /* Types of Main menu animation supported             */

    /* Normal animation: Frame 0 to Frame (n-1) for n frames.   */
    /* For icons that are not highlighted, Frame 0 will be      */
    /* displayed.                                   */
#define UI_MAIN_MENU_ANIMATION_TYPE1   1

    /* Highlight specific animation.                   */
    /* For highlighted iocns, frame 1 to frame (n-1) are used   */
    /* For icons that are not highlighted, Frame 0 will be      */
    /* displayed.                                   */
#define UI_MAIN_MENU_ANIMATION_TYPE2   2

    /* Type of main menu animation used by the GUI           */
#define UI_MAIN_MENU_ANIMATION_TYPE    UI_MAIN_MENU_ANIMATION_TYPE2

    /* Date and Time input type selectors.                */
    /* None of the following are implemented currently       */

    /* Day input category types: Do not modify these values  */
    /* Input box type                               */
#define UI_DAY_INPUT_CATEGORY_TYPE1    1
    /* Calendar matrix type                         */
#define UI_DAY_INPUT_CATEGORY_TYPE2    2
    /* Spin control type                            */
#define UI_DAY_INPUT_CATEGORY_TYPE3    3

    /* Type of Day input to use in the GUI                */
    /* Valid values are one of the above types               */
#define UI_DAY_INPUT_CATEGORY_TYPE     UI_DAY_INPUT_CATEGORY_TYPE1

    /* Month input category types: Do not modify these values   */
    /* Input box type                               */
#define UI_MONTH_INPUT_CATEGORY_TYPE1  1
    /* Drop down list type                             */
#define UI_MONTH_INPUT_CATEGORY_TYPE2  2
    /* Spin control type                            */
#define UI_MONTH_INPUT_CATEGORY_TYPE3  3

    /* Type of Month input to use in the GUI              */
    /* Valid values are one of the above types               */
#define UI_MONTH_INPUT_CATEGORY_TYPE   UI_MONTH_INPUT_CATEGORY_TYPE1

    /* Year input category types: Do not modify these values */
    /* Input box type                               */
#define UI_YEAR_INPUT_CATEGORY_TYPE1   1
    /* Drop down list type                             */
#define UI_YEAR_INPUT_CATEGORY_TYPE2   2
    /* Spin control type                            */
#define UI_YEAR_INPUT_CATEGORY_TYPE3   3

    /* Type of Year input to use in the GUI               */
    /* Valid values are one of the above types               */
#define UI_YEAR_INPUT_CATEGORY_TYPE    UI_YEAR_INPUT_CATEGORY_TYPE1

    /* Use these flags to turn off Pluto EMS library support */
#define UI_DISABLE_EMS_INPUT_BOX       0
#define UI_DISABLE_EMS_CATEGORY_SCREENS      0

    /* Used by UI_TEXT_MENUITEM_SCROLL_TYPE, don't modify    */
#define UI_TEXT_MENUITEM_SCROLL_TYPE_NONE       0
#define UI_TEXT_MENUITEM_SCROLL_TYPE_MARQUEE    1
#define UI_TEXT_MENUITEM_SCROLL_TYPE_TWO_DIRECTION 2

    /* Type of scrolling to use in text menuitems            */
    /* Valid values are one of the above symbols          */
#define UI_TEXT_MENUITEM_SCROLL_TYPE   UI_TEXT_MENUITEM_SCROLL_TYPE_MARQUEE

    /* Time for scrolling text in menu items              */
    /* Value must be an integer (milliseconds)               */
    /* Typical values: 100 to 500                      */
#define  UI_TEXT_MENUITEM_SCROLL_TIME  350

    /* Number of pixels by which the text scrolls            */
    /* in menu items.                               */
    /* Value must be an integer (units are in pixels)        */
    /* Typical values: 1 to 5                          */
#define UI_TEXT_MENUITEM_SCROLL_SIZE   3

    /* For Marquee type text scrolling in menu items, this      */
    /* specifies the blank gap that should appear at the end */
    /* of the string.                                  */
    /* Value must be an integer (units are in pixels)        */
    /* Typical values: 16 to 64 (This depends on font size)  */
#ifdef __MMI_MAINLCD_160X128__
#define UI_TEXT_MENUITEM_SCROLL_GAP    72
#else
#define UI_TEXT_MENUITEM_SCROLL_GAP    32
#endif
    /* This specifies the delay after which a highlighted item  */
    /* will scroll. This is a multiple of                 */
    /* UI_TEXT_MENUITEM_SCROLL_TIME                    */
    /* Value must be an integer. Typically 1, 2 -- etc.      */
    /* Depends on the value of UI_TEXT_MENUITEM_SCROLL_TIME  */
#define UI_TEXT_MENUITEM_SCROLL_PAUSE  4

    /* Enable or disable pop up descriptions              */
    /* 0: Disables, 1: Enables                         */
#define UI_ENABLE_POP_UP_DESCRIPTIONS  1

    /* Used by UI_POP_UP_DESCRIPTION_SCROLL_TYPE, don't modify  */
#define UI_POP_UP_DESCRIPTION_SCROLL_TYPE_NONE           0
#define UI_POP_UP_DESCRIPTION_SCROLL_TYPE_MARQUEE        1
#define UI_POP_UP_DESCRIPTION_SCROLL_TYPE_TWO_DIRECTION     2

    /* Type of scrolling to use in Pop up descriptions       */
    /* Valid values are one of the above symbols          */
#define UI_POP_UP_DESCRIPTION_SCROLL_TYPE UI_POP_UP_DESCRIPTION_SCROLL_TYPE_MARQUEE

    /* Enable or disable word-by-word text scrolling         */
    /* 0: Disables, 1: Enables                         */
#define UI_USE_WORD_SCROLLING_TEXT     0

     
    /* FRAME_SKIPPING switches    */
    /* 0: Disables, 1: Enables    */
#if defined(MT6205B)
#define UI_ENABLE_FRAME_SKIPPING                      0 //qiff disable the features on our platform, enhance gui display
#else 
#define UI_ENABLE_FRAME_SKIPPING                      0
#endif 
#define ENABLE_DIALER_INPUTBOX_FRAME_SKIPPING            1
#define ENABLE_LIST_MENU_FRAME_SKIPPING                  1
#define ENABLE_SINGLELINE_INPUTBOX_FRAME_SKIPPING        1
#define ENABLE_MULTILINE_INPUTBOX_FRAME_SKIPPING         1
#define ENABLE_DIRECT_MAIN_MENU_FRAME_SKIPPING           1
#define ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING 1
#if(!UI_DISABLE_EMS_INPUT_BOX)
#define ENABLE_EMS_INPUTBOX_FRAME_SKIPPING               1
#endif 
#define UI_FRAME_SKIP_TIMEOUT                         150
#define UI_FRAME_START_TIMEOUT                        100
#define DIRECT_MAIN_MENU_FRAME_SKIP_RATE              UI_FRAME_SKIP_TIMEOUT
     

    /* CSD JL030813, added for gif optimization */
#if !defined(__BW_MMI__) && !defined(WIN32)     /* defined(MMI_ON_HARDWARE_P) //Target version shall use this library */
#define USE_PLUTO_GIF_LIBRARY    1
#else /* PC and BW version use */
#define USE_PLUTO_GIF_LIBRARY    0
#endif 
    /* CSD end */

    /* Support for blinking cursors                    */
    /* 1: Enables blinking cursor, 0: disable             */

#define  UI_BLINKING_CURSOR_SUPPORT    1

#define UI_CURSOR_WAIT_TIME            1000
#define UI_CURSOR_BLINK_TIME        500

    /*  POPUP screen attribute  */
    // #define UI_POPUP_NOTIFYDURATION_TIME                 1000

     
     
    /* This macro is used for enabling the reuse of main menu shadow buffer. Make sure that 
       u check at all places if this is enabled for the buffer sizes being used. */
#define  __USE_MAIN_MENU_MATRIX_BUFFER 1

    /* Multi-layer configuration */
    /* sub menu */
#define UI_SUBMENU_OPACITY_BASED_LAYER 255
#define UI_SUBMENU_OPACITY_LAYER1 40
    /* Multilayer mask */
#define UI_MUL_BKG_MAINMENU      0x0001
#define UI_MUL_BKG_SUBMENU    0x0002
#define UI_MUL_ALL_ON            0xFFFF
#define UI_MUL_ALL_OFF           0x0000

#ifdef __MMI_LCD_PARTIAL_ON__
    /*
     * Height of the displayed region of  LCD partial on 
     * * This should be larger than font height and follow LCD partial display alignment
     * * 1. Dialer font has typical height 27
     * * 2. Most known LCM has partial display alignment < 4.
     */
#if  defined (__MMI_MAINLCD_128X160__)||defined (__MMI_MAINLCD_128X128__)
#define UI_LCD_PARTIAL_ON_HEIGHT 20
#else
#define UI_LCD_PARTIAL_ON_HEIGHT 32
#endif
#endif /* __MMI_LCD_PARTIAL_ON__ */ 

/* 070306 Alpha layer Start */
#ifndef __MMI_MAIN_BASE_LAYER_FORMAT__
#define __MMI_MAIN_BASE_LAYER_FORMAT__  GDI_COLOR_FORMAT_16
#endif
#ifndef __MMI_SUB_BASE_LAYER_FORMAT__
#define __MMI_SUB_BASE_LAYER_FORMAT__   GDI_COLOR_FORMAT_16
#endif
#ifndef __MMI_DEVICE_BMP_FORMAT__
#define __MMI_DEVICE_BMP_FORMAT__       16
#endif
#ifndef __MMI_IMAGE_HEADER_SIZE__
 #define __MMI_IMAGE_HEADER_SIZE__      6
#endif
/* 070306 Alpha layer End */

#ifdef __cplusplus
}
#endif 

#endif /* __GUI_CONFIG_H__ */ 

