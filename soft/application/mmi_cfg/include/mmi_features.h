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
 *	MMI_features.h
 *
 * Project:
 * --------
 *
 * Description:
 * ------------
 *   This file is intends for MMI features definition.
 *
 * Author:
 * -------
 *	
 *------------------------------------------------------------------------------
 *==============================================================================
 * 				HISTORY
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/include/MMI_features.h-arc  $
 * 
 *    Rev 1.15   Mar 02 2007 10:12:20   lin
 * 打开ip拨号功能
 * 
 *    Rev 1.14   Feb 02 2007 16:52:18   lin
 * 打开download铃声宏开关
 * 
 *    Rev 1.13   Jan 26 2007 11:16:30   lin
 * 打开__MMI_AUDIO_PLAYER__  宏
 * 
 *    Rev 1.12   Jan 17 2007 13:34:08   lin
 * 将__MMI_DRAW_MANAGER__宏关闭
 * 
 *    Rev 1.11   Jan 16 2007 11:55:46   lin
 * 支持FMGR模块和MMIFS模块
 * 
 *    Rev 1.10   Dec 21 2006 12:23:06   lin
 * 添加拼音排序的宏
 * 
 *    Rev 1.9   Dec 11 2006 11:55:30   lin
 * 打开宏,修改有问题的代码
 * 
 *    Rev 1.8   Nov 30 2006 11:00:40   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.6   Nov 24 2006 18:41:08   lin
 * No change.
 * 
 *    Rev 1.5   Nov 24 2006 10:01:34   lin
 * 保存通话记录后，通话记录模块里名字不能更新
 * 
 *    Rev 1.4   Nov 23 2006 18:01:26   lin
 * 加入__MMI_SIDEKEY_SUPPRESS_RING__标号
 * 
 *    Rev 1.3   Nov 14 2006 16:54:04   lin
 * 打开一个宏
 * 
 *    Rev 1.2   Nov 13 2006 21:39:48   lin
 * 修改了一个宏
 * 
 *    Rev 1.1   Nov 13 2006 21:33:00   lin
 * 定义了几个宏
 * 
 *    Rev 1.0   Nov 13 2006 15:26:38   zhangxb
 *
 *------------------------------------------------------------------------------
 *==============================================================================
 *******************************************************************************/
/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-06-15, Li Tao      close the define __MMI_CH_MIXED_CALL_LOG__
  02.01.002  2006-06-28, Han ShiLin, modify MAX_PHB_SIM_ENTRY from 255 to 260
 ************************************************************************/

#ifndef __MMI_FEATURES__
#define __MMI_FEATURES__

#define __MMI_SIDEKEY_SUPPRESS_RING__								//vivian add for side key
#define __FLIGHT_MODE_SUPPORT__ //chenhe for jasper
#define UPDATE_SW

/*---------------------------[DEPENDENCY DEFINE]-----------------------------*
 *---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*
 *------ Those compile options will be common referenced within this file,---*
 *------ we shall define at the dependency group to guarantee compile     ---*
 *------ option on/off works fine.                                        ---*
 *---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/

/*-----------------------------                 -----------------------------*/
/*---------------------------[Begin Product]----------------------------*/
/*-----------------------------                 -----------------------------*/
/*Define the related macro for the QPAD product.
If you can find the right category(such as: Framework, Application and so on. ) in this file,
you should add the macro to the right category, Or else you define them here.
The macro PRODUCT_QPAD was defined in the file conversion.mk .*/
#ifdef PRODUCT_QPAD
#define __PRODUCT_QPAD__
#endif /*PRODUCT_QPAD*/

#ifdef __GENERAL_TTS__
#define __MMI_TTS_FEATURES__
#endif

#ifdef __PRODUCT_QPAD__
#define __MMI_FULL_KEYPAD__
#define __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__
#define __MMI_MODULE_NO_TITLEBAR__
#endif

#ifdef __MMI_FULL_KEYPAD__
#define __MMI_WITH_IME_KEY__
#endif
/*-----------------------------                 -----------------------------*/
/*----------------------------[End Product]----------------------------*/
/*-----------------------------                 -----------------------------*/

/*-----------------------------                 -----------------------------*/
/*-----------------------------[Begin Framework]-----------------------------*/
/*-----------------------------                 -----------------------------*/

/*****************************************************************************
 [Framework]: GUI
 *****************************************************************************/
// dingjian 20071217 for lcd szie is 176*220
//#if ( defined(CSDLCM_COLOR) || defined(TOPPOLY_LCM) || defined(FIREFLY_LCM) || defined (MILAN_LCM) || defined (J121_DEMO_05B_LCM) || defined (FS01_LCM) )
#ifdef LCDSIZE_128_160
	#define	__MMI_MAINLCD_128X160__											/* Detail Info: [Main LCD is 128x160]*/
#endif // LCDSIZE_128_160   
//#elif ( defined (RENESAS_LCM) || defined (SAPPHIRE28_LCM))
#ifdef LCDSIZE_240_320
	#define	__MMI_MAINLCD_240X320__ 											/* Detail Info: [Main LCD is 240x320]*/
#endif // LCDSIZE_240_320    
//#elif ( defined(SONY_LCM) || defined (DRAGONFLY_LCM) || defined(TOP_6227_LCM) )
#ifdef LCDSIZE_176_220
	#define	__MMI_MAINLCD_176X220__											/* Detail Info: [Main LCD is 176x220]*/
#endif // LCDSIZE_176_220    

/*cong.li added for product QPAD[220*176] on 2009.08.11. */
#ifdef LCDSIZE_220_176
	#define	__MMI_MAINLCD_220X176__											/* Detail Info: [Main LCD is 220x176]*/
#endif // LCDSIZE_220_176    

//#else
	//#define	__MMI_MAINLCD_128X128__											/* Detail Info: [Main LCD is 128x128]*/
//#endif

#ifdef LCDSIZE_128_160
//support 7+1 flash size on 128x160 screen add by qiff
//#define __MMI_RESOURCE_SLIM_ON_128X160__
#endif

#ifdef __MMI_RESOURCE_SLIM_ON_128X160__											/* Detail Info: [Main LCD is 128x160]*/
	#define __MMI_SLIM_CALENDAR__
#endif

/**********whether to use flash fs for multimedia************/
#define COOLSAND_MULTIMEDIA_USE_FLASHFS

#ifdef __MMI_NO_SIDEKEY__
#define __NO_SIDE_VOLUME_KEY_STAR_POUND__ 
#endif
/*****************************************************************************
 [Framework]: Languages
 *****************************************************************************/
#define __MMI_LANG_ENGLISH__                                  /* Detail Info: [to turn on English language, English is default language] */
#ifdef __MMI_LARGE_EN_FONTS__
#define __MMI_14_EN_FONT__ 
#define __MMI_20_EN_FONT__
#define __MMI_22_EN_FONT__
#endif

//#define __MMI_GB_V5__

#if 0 /* these define moved to mmi config file */
//#define __MMI_LANG_TR_CHINESE__										  /* Detail Info: [to turn on Tr Chinese language] */
#define __MMI_LANG_SM_CHINESE__										  /* Detail Info: [to turn on Sm Chinese language] */

#define __MMI_LANG_RUSSIAN__
#define __MMI_LANG_FRENCH__                                /*turn on French language*/
#define __MMI_LANG_ITALIAN__                                /*turn on Italian language*/
#define BIDI_SUPPORT                                                /*turn on Arabic language */
#define __MMI_LANG_THAI__                                   /*trun on thai language*/
#define __MMI_LANG_VIETNAMESE__
#endif
#if defined(__MMI_LANG_PERSIAN__)
#define __MMI_MULTITAP_PERSIAN__
#define __MMI_SMART_PERSIAN__
#endif

#if defined(__MMI_LANG_URDU__)
#define __MMI_MULTITAP_URDU__
#define __MMI_SMART_URDU__
#if defined(__MMI_SMART_URDU__)&& !defined(__MMI_GB_V5__)
#error "__MMI_SMART_URDU__ need define __MMI_GB_V5__"
#endif
#endif

#if defined(__MMI_LANG_BENGALI__)
#define __MMI_MULTITAP_BENGALI__
#define __MMI_SMART_BENGALI__
#if defined(__MMI_SMART_BENGALI__)&& !defined(__MMI_GB_V5__)
#error "__MMI_SMART_BENGALI__ need define __MMI_GB_V5__"
#endif
#endif

#if defined(__MMI_LANG_PUNJABI__)
#define __MMI_MULTITAP_PUNJABI__
#define __MMI_SMART_PUNJABI__
#if defined(__MMI_SMART_PUNJABI__)&&!defined(__MMI_GB_V5__)
#error "__MMI_SMART_PUNJABI need define __MMI_GB_V5__"
#endif
#endif


#if defined(__MMI_LANG_TAMIL__)
#define __MMI_MULTITAP_TAMIL__
#define __MMI_SMART_TAMIL__
#endif


#if defined(__MMI_LANG_MYANMAR__)
#define __MMI_MULTITAP_MYANMAR__
#endif

#if defined(__MMI_LANG_RUSSIAN__)
#define __MMI_MULTITAP_RUSSIAN__
#define __MMI_SMART_RUSSIAN__
#endif
#if defined(__MMI_LANG_FRENCH__)
#define __MMI_MULTITAP_FRENCH__
#define __MMI_SMART_FRENCH__
#endif

#if defined(__MMI_LANG_TURKISH__)
#define __MMI_MULTITAP_TURKISH__
#define __MMI_SMART_TURKISH__
#endif

// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_LANG_ITALIAN__)
#define __MMI_MULTITAP_ITALIAN__
#define __MMI_SMART_ITALIAN__
#endif
// end--added by kecx for adding italian on 20081223

#if defined(__MMI_LANG_THAI__)
#define __MMI_MULTITAP_THAI__
#define __MMI_SMART_THAI__
#endif

#if defined(__MMI_LANG_SM_CHINESE__)
#define __MMI_GB_SM_CHINESE__
#endif

//#define __MMI_LANG_ENGLISH_ABC_abc_SUPPORT_OTHER_LANGS__

#if !defined(NEPTUNE_COLOR_BB) && !defined(MT6205B)&& !defined(__MMI_HANWANG__) && !defined(__MMI_PENPOWER__)
	//#define __MMI_LANG_SPANISH__										  /* Detail Info: [to turn on Spanish language] */
	//#define __MMI_LANG_FRENCH__											  /* Detail Info: [to turn on French language] */
	//#define __MMI_LANG_GERMAN__											  /* Detail Info: [to turn on German language] */
	//#define __MMI_LANG_ITALIAN__										  /* Detail Info: [to turn on Italic language] */
	//#define __MMI_LANG_RUSSIAN__                                /* Detail Info: [to turn on Russian language] */
	//#define __MMI_LANG_THAI__											  /* Detail Info: [to turn on Thai language] */
	//#define __MMI_LANG_TURKISH__										  /* Detail Info: [to turn on Turkish language] */
	//#define __MMI_LANG_MALAY__											  /* Detail Info: [to turn on Malay language]*/
	//#define __MMI_LANG_VIETNAMESE__									  /* Detail Info: [to turn on Vietnamese language] */
#if defined(MILAN_BB)
	//#define __MMI_LANG_DANISH__											  /* Detail Info: [to turn on Danish language] */
	//#define __MMI_LANG_PORTUGUESE__										  /* Detail Info: [to turn on Portuguese language] */
	//#define __MMI_LANG_INDONESIAN__									     /* Detail Info: [to turn on Indonesian language] */
#endif

#endif

#if defined(JUPITER_PLUS_EVB) && !defined(__AMNT__)&& !defined(__MMI_HANWANG__) && !defined(__MMI_PENPOWER__)
//#define __MMI_LANG_HINDI__												  /* Detail Info: [to turn on Hindi language]*/
//#define __MMI_HINDI_ALG__											  	  /* Detail Info: [Hindi needs hindi font algorithm]*/
#endif

#if defined(BIDI_SUPPORT) && !defined(__AMNT__)&& !defined(__MMI_HANWANG__) //&& !defined(__MMI_PENPOWER__) delete by panxu
	#define __MMI_LANG_ARABIC__										  /* Detail Info: [to turn on Arabic language]*/
#endif

#if defined(__MMI_LANG_ARABIC__)
#define __MMI_SMART_ARABIC__
#endif

#if defined(__MMI_LANG_PORTUGUESE__)
#define __MMI_MULTITAP_PORTUGUESE__
#endif


#if defined(__MMI_LANG_INDONESIAN__)
	#define __MMI_MULTITAP_INDONESIAN__												/* Detail Info: [turn on Multitap Indonesian inputs]*/
	#define __MMI_SMART_INDONESIAN__
#endif

#if defined(__MMI_LANG_MALAY__)
	#define __MMI_MULTITAP_MALAY__												/* Detail Info: [turn on Multitap Indonesian inputs]*/
	#define __MMI_SMART_MALAY__
#endif

//#if defined(__MMI_LANG_DANISH__)
//   #define __MMI_MULTITAP_DANISH__										/* Detail Info: [turn on T9 Danish inputs]*/
//#endif

#if defined(__MMI_LANG_VIETNAMESE__)
	#define __MMI_MULTITAP_VIETNAMESE__
	#define __MMI_SMART_VIETNAMESE__
#endif

#if defined(__MMI_LANG_HINDI__)
	#define __MMI_SMART_HINDI__
#endif

#if defined(__MMI_LANG_GERMAN__)
#define	__MMI_SMART_GERMAN__
#define __MMI_MULTITAP_GERMAN__
#endif

#if defined(__MMI_LANG_SPANISH__)
#define __MMI_SMART_SPANISH__
#define __MMI_MULTITAP_SPANISH__
#endif

//qiff add multi-input method support
#if defined(__MMI_LANG_TELUGU__)
#define __MMI_MULTITAP_TELUGU__
#define __MMI_SMART_TELUGU__
#endif
#if defined(__MMI_LANG_KANNADA__)
#define __MMI_MULTITAP_KANNADA__
#endif
#if defined(__MMI_LANG_MALAYALAM__)
#define __MMI_MULTITAP_MALAYALAM__
#endif
#if defined(__MMI_LANG_ORIYA__)
#define __MMI_MULTITAP_ORIYA__
#endif
#if defined(__MMI_LANG_MARATHI__)
#define __MMI_MULTITAP_MARATHI__
#endif
//qiff add end


#if defined(__MMI_LANG_ARABIC__)||defined(__MMI_LANG_PERSIAN__)
#define __MMI_BIDI_ALG__											  /* Detail Info: [Arabic needs bidi algorithm]*/
#define __MMI_ARSHAPING_ENGINE__
#endif
/*------------------------------               ------------------------------*/
/*------------------------------[End Framework]------------------------------*/
/*------------------------------               ------------------------------*/

/*-----------------------------                   ---------------------------*/
/*-----------------------------[Begin Application]---------------------------*/
/*-----------------------------                   ---------------------------*/
/*****************************************************************************
 [Application]: File Manager
 *****************************************************************************/
#ifndef FMT_NOT_PRESENT
   /*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 11:29 */
   #define __MMI_FILE_MANAGER__  //dyj del 20060514
   /*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 11:29 */
   #define __MMI_SHOW_FILE_EXT__											/* Detail Info: []*/
   //#define __MMI_SHOW_DAF_FILE_EXT__ 								/* Detail Info: [Default hide mp3 extension] */
										/* Detail Info: []*/
   #if defined(MT6205B)
      #define __MMI_SLIM_FILE_MANAGER__								/* Detail Info: []*/
   #endif
#endif

/*****************************************************************************
 [Application]: Services
 *****************************************************************************/
/* WAP: [Teleca WAP support]
 */
#if defined(WAP_SUPPORT) && !defined(__MMI_FILE_MANAGER__)
  #define __MMI_WAP__												/* Detail Info: [Enable Fun&Games download application]*/
#endif

#if defined(FIREFLY_BB) || defined(DRAGONFLY_BB)
  #define __MMI_WITH_WAP_KEY__										/* Detail Info: [WAP key]*/
#endif

#define FACTORY_CC_AUTO_DAILED                                                                         /*Detail Info: For the factory auto dailed test*/
/*------------------------------                 ----------------------------*/
/*------------------------------[End Application]----------------------------*/
/*------------------------------                 ----------------------------*/

/*-----------------------------                  ----------------------------*/
/*-----------------------------[Begin Multimedia]----------------------------*/
/*-----------------------------                  ----------------------------*/
#define __MMI_DATA_STORE__                              /* Detail Info: []*/

/*------------------------------                -----------------------------*/
/*------------------------------[End Multimedia]-----------------------------*/
/*------------------------------                -----------------------------*/

#define __MMI_NUMBER_MENU__
/*--------------------------[INDEPENDENCY DEFINE]----------------------------*
 *---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*
 *------ Those compile options will be not referenced within this file,   ---*
 *------ we can guarantee compile options work fine within independency   ---*
 *------ relationship. ------------------------------------------------------*
 *---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/

/*-----------------------------                 -----------------------------*/
/*-----------------------------[Begin Framework]-----------------------------*/
/*-----------------------------                 -----------------------------*/

/*****************************************************************************
 [Framework]: SUBLCD
 *****************************************************************************/
#ifdef DUAL_LCD
   #define __MMI_SUBLCD__													/* Detail Info: [This is for sublcd screen define]*/
   #define __MMI_SUBLCD_SHOW_TIME__										/* Detail Info: [sublcd to show out the time at idle mode]*/
   #define __MMI_SUBLCD_SHOW_DATE__										/* Detail Info: [sublcd to show out the date at idle mode]*/
//   #define __MMI_SUBLCD_SHOW_WORLD_CLOCK__							/* Detail Info: [sublcd to show out the world clock at idle mode]*/
   #define __MMI_SUBLCD_SHOW_ONE_CITY__								/* Detail Info: [sublcd to show out the city at idle mode]*/
   #ifdef COLOR_SUBLCD
      #define __MMI_SUBLCD_COLOR__										/* Detail Info: [color sublcd screen]*/
		#define __MMI_SUB_WALLPAPER__										/* Detail Info: [sublcd wallpaper support]*/
   #endif
#endif

/*****************************************************************************
 [Framework]: DRAW MANAGER
******************************************************************************/
//#if defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_176X220__)
  #define __MMI_DRAW_MANAGER__	  										/* Detail Info: [For enable draw manager] */  //ssp del 20060527
  /*fengzb and wangrui add 20070614  for using mmi_awmange*/
  #define __GDI_MEMORY_PROFILE_2__
//#endif

/*****************************************************************************
 [Framework]: TOUCH SCREEN
 *****************************************************************************/
 #define  CSD_SYSSERV_DEBUG                 //chenqiang add
#ifdef TOUCH_PANEL_SUPPORT
	#define __MMI_TOUCH_SCREEN__											/* Detail Info: [For enable touch screen support] */
#endif

#ifdef __MMI_TOUCH_SCREEN__
	#define __MMI_TOUCH_DIAL_SCREEN__
	#define __MMI_HAND_WRITING__											/*Detail Info:[For enable hand writing support]*/
	#define __MMI_PENPOWER__												/*Detail Info:[Use mengtian for hand writing lib support]*/
	#if defined ( __MMI_MAINLCD_176X220__) ||defined(__MMI_MAINLCD_240X320__)||defined ( __MMI_MAINLCD_220X176__)
	#define __MMI_WGUI_CSK_ENABLE__
	#endif
	#ifdef __MMI_LANG_ARABIC__
	//#define __MMI_ARABIC_VK__
	#endif
	#ifdef __MMI_LANG_FRENCH__
	//#define __MMI_FRENCH_VK__
	#endif
	#ifdef __MMI_LANG_ITALIAN__
	//#define __MMI_ITALIAN_VK__
	#endif
	#ifdef __MMI_LANG_RUSSIAN__
	//#define __MMI_RUSSIAN_VK__
	#endif
	#ifdef __MMI_LANG_GERMAN__
	//#define __MMI_GERMAN_VK__
	#endif
	#ifdef __MMI_LANG_TURKISH__
	//#define __MMI_TURKISH_VK__
	#endif
	#ifdef __MMI_LANG_VIETNAMESE__
	//#define __MMI_VIETNAMESE_VK__
	#endif
	#ifdef __MMI_LANG_THAI__
	//#define __MMI_THAI_VK__
	#endif
	#ifdef __MMI_LANG_TR_CHINESE__
	//#define __MMI_TR_CHINESE_VK__
	#endif
	#ifdef __MMI_LANG_SPANISH__
	//#define __MMI_SPANISH_VK__
	#endif
	#ifdef __MMI_LANG_PORTUGUESE__
	//#define __MMI_PORTUGUESE_VK__
	#endif
	#ifdef __MMI_LANG_MALAY__
	//#define __MMI_MALAY_VK__
	#endif
	#ifdef __MMI_LANG_PERSIAN__
	//#define __MMI_PERSIAN_VK__
	#endif
	#ifdef __MMI_LANG_INDONESIAN__
	//#define __MMI_INDONESIAN_VK__
	#endif
	#ifdef __MMI_LANG_HINDI__
	//#define __MMI_HINDI_VK__
	#endif
#endif

/*****************************************************************************
 [Framework]: SCREEN ROTATION
 *****************************************************************************/
#ifdef HORIZONTAL_CAMERA
#define __MMI_SCREEN_ROTATE__												/* Detail Info: [Screen rotation] */
#define __MMI_SCREEN_ROTATE_DEGREE_270__								/* Detail Info: [Supports screen rotation of 270 degree ] */
#endif


/*****************************************************************************
 [Framework]: GUI
 *****************************************************************************/
/* Detail Info: [MainMenu type, should be one of the following. LIST , MATRIX , CIRCULAR_3D , PAGE , FIXED_GRID , ANI_BG_LIST] */




#if defined(__MMI_MAINLCD_128X160__)
   #define __MMI_UI_STYLE_2__												/* Detail Info: [128x160 size screen]*/
#elif defined(__MMI_MAINLCD_176X220__)
   #define __MMI_UI_STYLE_3__												/* Detail Info: [176x220 size screen]*/
#elif defined(__MMI_MAINLCD_220X176__)
   #define __MMI_UI_STYLE_3__												/* Detail Info: [220x176 size screen]*/   
#elif defined (__MMI_MAINLCD_240X320__)
	#define __MMI_UI_STYLE_4__												/* Detail Info: [240x320 size screen]*/
#else
   #define __MMI_UI_STYLE_1__												/* Detail Info: [128x128 size screen]*/
#endif

/*****************************************************************************
 [Framework]:	Optional UI style from UI factory supported only 176x220 currently

 Options:		__MMI_UI_DALMATIAN_IDLESCREEN__							Detail Info: [idle screen with __MMI_UI_DALMATIAN_STATUSBAR__& __MMI_UI_DALMATIAN_SOFTKEYS__]
 					__MMI_UI_DALMATIAN_STATUSBAR__						Detail Info: [2 layers of status bars that can shuffle]
 					__MMI_UI_DALMATIAN_SOFTKEYS__						Detail Info: [2 layers of softkeys that can shuffle]
 			__MMI_UI_DALMATIAN_MAINMENU__								Detail Info: [balls movement when going next or previous in PAGE mainmenu]
 			__MMI_UI_DALMATIAN_FIXEDLIST__								Detail Info: [drawing lines and grids for list and menuitems]
 			__MMI_UI_DALMATIAN_EDITOR__								Detail Info: [showing a background image if there is no input]

 			__MMI_UI_TECHNO_STATUS_ICON__								Detail Info: [vertical signal strength and battery level status icons], force __MMI_UI_TECHNO_IDLESCREEN_BAR__
 					__MMI_UI_TECHNO_IDLESCREEN_BAR__					Detail Info: [clock, date/time, network name and vertical status icons in a bar on idelscreen]
 			__MMI_UI_TECHNO_SOFTKEYS__									Detail Info: [special softkey theme]
 			__MMI_UI_TECHNO_MAINMENU__									Detail Info: [PAGE mainmenu image transition effect]

Dependency:	__MMI_UI_DALMATIAN_IDLESCREEN__ 	needs
					__MMI_UI_DALMATIAN_STATUSBAR__ & __MMI_UI_DALMATIAN_SOFTKEYS__
			__MMI_UI_TECHNO_STATUS_ICON__ 		needs
					__MMI_UI_TECHNO_IDLESCREEN_BAR__

Exclusion:	__MMI_UI_DALMATIAN_STATUSBAR__		v.s	__MMI_UI_TECHNO_STATUS_ICON__ (__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			__MMI_UI_DALMATIAN_SOFTKEYS__		v.s	__MMI_UI_TECHNO_SOFTKEYS__
			__MMI_UI_DALMATIAN_IDLESCREEN__	v.s	__MMI_UI_TECHNO_STATUS_ICON__ (__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			__MMI_UI_DALMATIAN_IDLESCREEN__	v.s	__MMI_UI_TECHNO_SOFTKEYS__
			__MMI_UI_DALMATIAN_MAINMENU__		v.s	__MMI_UI_TECHNO_MAINMENU__
*****************************************************************************/
// Macros added for Dalmatian UI Style
#ifdef __MMI_MAINLCD_176X220__
//PMT VIKAS START 20050913
//DALMATIAN FEATURES HAVE BEEN DISTRIBUTED AS PER UI STYLES
	//#define __MMI_UI_DALMATIAN_MAINMENU__
	//#define  __MMI_UI_DALMATIAN_CALENDAR__
#endif
//#define __MMI_UI_DALMATIAN_STATUSBAR__
#ifndef __MMI_WGUI_CSK_ENABLE__
	//#define __MMI_UI_DALMATIAN_SOFTKEYS__
	//#define __MMI_UI_DALMATIAN_IDLESCREEN__
	#ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
		#define __MMI_UI_DALMATIAN_STATUSBAR__
		#define __MMI_UI_DALMATIAN_SOFTKEYS__
	#endif
#endif
//Dalmatian Fixlist and Editor is for all type of screen size
//#define __MMI_UI_DALMATIAN_FIXEDLIST__
//#define __MMI_UI_DALMATIAN_EDITOR__
//PMT VIKAS END 20050913
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
	#define __MMI_UI_MM_LIST_NO_GRID__									/* Detail Info: [Don't draw grid on List main menu]*/
#endif

// Macros added for Techno UI Style
#ifdef __MMI_MAINLCD_176X220__
   #ifndef __MMI_WGUI_CSK_ENABLE__
		//#define __MMI_UI_TECHNO_SOFTKEYS__								/* Detail Info: [Techno styled softkeys]*/
	#endif
	//#define __MMI_UI_TECHNO_MAINMENU__									/* Detail Info: [Techno styled page mainmenu]*/
	//#define __MMI_UI_TECHNO_IDLESCREEN_BAR__							/* Detail Info: [Techno styled idle screen bar containing clock]*/
	//#define __MMI_UI_TECHNO_STATUS_ICON__								/* Detail Info: [Techno styled vertical signal and baterry status icons]*/
#endif



//Exclusion for Dalmatian and Techno
#if defined (__MMI_UI_DALMATIAN_STATUSBAR__) || defined (__MMI_UI_DALMATIAN_IDLESCREEN__)
	#undef __MMI_UI_TECHNO_STATUS_ICON__
	#undef __MMI_UI_TECHNO_IDLESCREEN_BAR__
#endif
#if defined (__MMI_UI_DALMATIAN_SOFTKEYS__) || defined (__MMI_UI_DALMATIAN_IDLESCREEN__)
	#undef __MMI_UI_TECHNO_SOFTKEYS__
#endif
#if defined (__MMI_UI_DALMATIAN_MAINMENU__)
	#undef __MMI_UI_TECHNO_MAINMENU__
#endif


/* Tab pane not integrated with touch screen yet */
#if !defined (__MMI_TOUCH_SCREEN__) && !defined (__MMI_DRAW_MANAGER__)
	//#define __MMI_UI_TAB_PANE__                                                                                             /* Detail Info: Tab view support*/  //ssp del 20060527
#endif /* ~ __MMI_TOUCH_SCREEN__ */



//#define __MMI_UI_LIST_HIGHLIGHT_EFFECTS__  								/* Detail Info: [List Effect type, should be one of the following. TRANSPARENCY , FLASH_FEELING , ANIMATION , NONE  ] */
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    #define __MMI_UI_TRANSPARENT_EFFECT__
	 #define __MMI_LIST_EFFECT_TYPE__ TRANSPARENCY
	//#define __MMI_LIST_EFFECT_TYPE__ FLASH_FEELING
	//#define __MMI_LIST_EFFECT_TYPE__ ANIMATION
	//#define __MMI_LIST_EFFECT_TYPE__  NONE
#endif



#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_128X160__)||defined(__MMI_MAINLCD_220X176__)	
#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_2__							/* Detail Info: [Use image as background] */
#else
#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__							/* Detail Info: [Old-style flat bar] */
#endif

#ifdef __MMI_UI_TECHNO_SOFTKEYS__
	#ifdef __MMI_UI_SOFTKEY_DEFAULT_STYLE_2__
		#undef __MMI_UI_SOFTKEY_DEFAULT_STYLE_2__
	#endif
	#ifndef __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__
		#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__
	#endif
#endif

#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
	#ifdef __MMI_UI_SOFTKEY_DEFAULT_STYLE_2__
		#undef __MMI_UI_SOFTKEY_DEFAULT_STYLE_2__
	#endif
	#ifndef __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__
		#define __MMI_UI_SOFTKEY_DEFAULT_STYLE_1__
	#endif
#endif



#ifdef __MMI_MAINMENU_TAB_SUPPORT__
	#define __MMI_UI_TAB_PANE__
	#define __MMI_UI_TITLE_TRANSITION__
#endif

#if !defined(__MMI_MAINLCD_128X128__)
//#define __MMI_UI_TRANSPARENT_EFFECT__									/* Detail Info: Transparent highlight of list menu*/
#endif

//#define __MMI_ANALOG_CLOCK__				// zrx del 20060524								/* Detail Info: [Enable analog clock wallpaper]*/
//#define __MMI_DIGITAL_CLOCK__			      // zrx del 20060524									/* Detail Info: [Enable digital clock wallpaper]*/
 
//#define __MMI_SEPARATE_DT_SETTING__
#define __MMI_POWER_ON_OFF_DISPLAY__									/* Detail Info: [Power on off display] */

#if (defined(MP4_CODEC)  || defined(MJPG_SUPPORT))
	#if defined(__MMI_POWER_ON_OFF_DISPLAY__)
	//	#define __MMI_POWER_ON_OFF_DISPLAY_VIDEO__					/* Detail Info: [Power on off video display] */
	#endif
//	#define __MMI_SCREENSAVER_DISPLAY_VIDEO__							/* Detail Info: [Screensaver video display] */
#endif


#define __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__					/* Detail Info: [Enable signal strength status icon combine with line switch]*/

#if defined (__MMI_DRAW_MANAGER__) && defined (__MMI_MAINLCD_240X320__)
	//#define __MMI_WALLPAPER_ON_BOTTOM__									/* Detail Info: [Use an image on a new layer as the background]*/
#endif

#if defined (__MMI_DRAW_MANAGER__) && defined (__MMI_MAINLCD_240X320__)
	//#define __MMI_UI_CALENDAR_WITH_INFO_BOX__							/* Detail Info: [Enable an infomation box embedded in calendar]*/
#endif

#if !defined(__MMI_WALLPAPER_ON_BOTTOM__) && (!defined(MT6205B) && !defined(MT6208))
// Experimental feature. Only enabled in internal projects.
#ifdef DEMO_PROJECT
#define __MMI_UI_LIST_CACHE_BACKGROUND__								/* Defail Info: [cache list menu background in anther GDI layer. */
#endif /* DEMO_PROJECT */
#endif

/*****************************************************************************
 [Framework]: Font BDF Sets
 *****************************************************************************/
#if defined(__MMI_LANG_SM_CHINESE__)
	#define __MMI_CHAR_SET_GB2312__										/* Detail Info: [used by internal project, to choose different Chinese font database]*/
#ifdef __MMI_LARGE_FONTS__
	#define __MMI_20X20_CHINESE_FONT__ 
#endif
#endif

#if defined(__MMI_LANG_TR_CHINESE__)
	#define __MMI_CHAR_SET_BIG5__											/* Detail Info: [used by internal project, to choose different Chinese font database]*/
#endif
/*****************************************************************************
 [Framework]: Input Methods
 *****************************************************************************/
/* Input Method
 */
#if defined(__MMI_T9__)

#define __MMI_T9_V7__

#if !defined(__MMI_TOUCH_SCREEN__)
   #if defined(__MMI_LANG_ENGLISH__)
      #define __MMI_T9_ENGLISH__												/* Detail Info: [turn on T9 English inputs]*/
   #endif /* __MMI_LANG_ENGLISH__ */
#endif

   #if defined(__MMI_LANG_TR_CHINESE__)
   	#define __MMI_T9_TR_CHINESE__										/* Detail Info: [turn on T9 Tr Chinese inputs]*/
   #endif /* __MMI_LANG_TR_CHINESE__ */

   #if defined(__MMI_LANG_SM_CHINESE__)
   	#define __MMI_T9_SM_CHINESE__										/* Detail Info: [turn on T9 Sm Chinese inputs]*/
   #endif /* __MMI_LANG_SM_CHINESE__ */

   #define __MMI_T9_MULTITAP_PHONETIC_INPUT__						/* Detail Info: [turn on TR/SM Chinese multitap phonetic inputs]*/
#if !defined(__MMI_TOUCH_SCREEN__)
   #define __MMI_T9_SMART_PHONETIC_INPUT__							/* Detail Info: [turn on TR/SM Chinese smart phonetic inputs]*/
#endif

#if !defined(__MMI_TOUCH_SCREEN__)
   #if defined(__MMI_LANG_SPANISH__)
   	#define __MMI_T9_SPANISH__											/* Detail Info: [turn on T9 Spanish inputs]*/
   #endif /* __MMI_LANG_SPANISH__ */

	#if defined(__MMI_LANG_DANISH__) && defined(__MMI_T9_V7__)
   	#define __MMI_T9_DANISH__										/* Detail Info: [turn on T9 Danish inputs]*/
   #endif /* __MMI_LANG_DANISH__ */

   #if defined(__MMI_LANG_FRENCH__)
      #define __MMI_T9_FRENCH__										/* Detail Info: [turn on T9 French inputs]*/
   #endif /* __MMI_LANG_FRENCH__ */

   #if defined(__MMI_LANG_GERMAN__)
      #define __MMI_T9_GERMAN__										/* Detail Info: [turn on T9 German inputs]*/
   #endif /* __MMI_LANG_GERMAN__ */

   #if defined(__MMI_LANG_ITALIAN__)
      #define __MMI_T9_ITALIAN__										/* Detail Info: [turn on T9 Italic inputs]*/
   #endif /* __MMI_LANG_ITALIAN__ */

   #if defined(__MMI_LANG_VIETNAMESE__) && defined(__MMI_T9_V7__)
      #define __MMI_T9_VIETNAMESE__									/* Detail Info: [turn on T9 Vietnamese inputs]*/
   #endif

   #if defined(__MMI_LANG_INDONESIAN__) && defined(__MMI_T9_V7__)
      #define __MMI_T9_INDONESIAN__										/* Detail Info: [turn on T9 Indonesian inputs]*/
   #endif /* __MMI_LANG_INDONESIAN__ */

   #if defined(__MMI_LANG_THAI__) && defined(__MMI_T9_V7__)
      #define __MMI_T9_THAI__										/* Detail Info: [turn on T9 Thai inputs]*/
   #endif /* __MMI_LANG_THAI__ */

   #if defined(__MMI_LANG_RUSSIAN__) && defined(__MMI_T9_V7__)
      #define __MMI_T9_RUSSIAN__										/* Detail Info: [turn on T9 Thai inputs]*/
   #endif /* __MMI_LANG_RUSSIAN__ */

   #if defined(__MMI_LANG_PORTUGUESE__)&& defined(__MMI_T9_V7__)
      #define __MMI_T9_PORTUGUESE__									/* Detail Info: [turn on T9 Portuguese inputs]*/
   #endif /* __MMI_LANG_PORTUGUESE__ */

#endif

#elif defined(__MMI_ZI__)

#if !defined(__MMI_TOUCH_SCREEN__)
   #if defined(__MMI_LANG_ENGLISH__)
      #define __MMI_ZI_PRC_ENGLISH__										/* Detail Info: [turn on Zi English inputs]*/
   #endif /* __MMI_LANG_ENGLISH__ */
#endif

   #if defined(__MMI_LANG_TR_CHINESE__)
   	#define __MMI_ZI_TR_CHINESE__										/* Detail Info: [turn on Zi Tr Chinese inputs]*/
   #endif /* __MMI_LANG_TR_CHINESE__ */

   #if defined(__MMI_LANG_SM_CHINESE__)
   	#define __MMI_ZI_SM_CHINESE__										/* Detail Info: [turn on Zi Sm Chinese inputs]*/
   #endif /* __MMI_LANG_SM_CHINESE__ */

   #define __MMI_ZI_MULTITAP_PHONETIC_INPUT__						/* Detail Info: [turn on TR/SM Chinese multitap phonetic inputs]*/
#if !defined(__MMI_TOUCH_SCREEN__)
   #define __MMI_ZI_SMART_PHONETIC_INPUT__							/* Detail Info: [turn on TR/SM Chinese smart phonetic inputs]*/
#endif

#if !defined(__MMI_TOUCH_SCREEN__)
   #if defined(__MMI_LANG_THAI__)
   	#define __MMI_ZI_THAI__												/* Detail Info: [turn on Zi Thai inputs]*/
   #endif /* __MMI_LANG_THAI__ */

   #if defined(__MMI_LANG_RUSSIAN__)
   	#define __MMI_ZI_RUSSIAN__											/* Detail Info: [turn on Zi Russian inputs]*/
   #endif /* __MMI_LANG_RUSSIAN__ */

   #if defined(__MMI_LANG_SPANISH__)
   	#define __MMI_ZI_SA_SPANISH__										/* Detail Info: [turn on Zi South American Spanish inputs]*/
   #endif /* __MMI_LANG_SPANISH__ */
//PMT VIKASG START 20050729
	//#if defined(__MMI_LANG_DANISH__)
   //	#define __MMI_ZI_DANISH__										/* Detail Info: [turn on Zi South American Spanish inputs]*/
   //#endif /* __MMI_LANG_DANISH__ */
//PMT VIKASG END 20050729
   #if defined(__MMI_LANG_FRENCH__)
      #define __MMI_ZI_EU_FRENCH__										/* Detail Info: [turn on Zi EU French inputs]*/
   #endif /* __MMI_LANG_FRENCH__ */

   #if defined(__MMI_LANG_GERMAN__)
      #define __MMI_ZI_GERMAN__											/* Detail Info: [turn on Zi German inputs]*/
   #endif /* __MMI_LANG_GERMAN__ */

   #if defined(__MMI_LANG_ITALIAN__)
      #define __MMI_ZI_ITALIAN__											/* Detail Info: [turn on Zi Italic inputs]*/
   #endif /* __MMI_LANG_ITALIAN__ */

   #if defined(__MMI_LANG_PORTUGUESE__)
      #define __MMI_ZI_SA_PORTUGUESE__									/* Detail Info: [turn on Zi South American Portuguese inputs]*/
   #endif /* __MMI_LANG_PORTUGUESE__ */

   #if defined(__MMI_LANG_TURKISH__)
      #define __MMI_ZI_TURKISH__											/* Detail Info: [turn on Zi Turkish inputs]*/
   #endif /* __MMI_LANG_TURKISH__ */

   #if defined(__MMI_LANG_INDONESIAN__)
      #define __MMI_ZI_INDONESIAN__									  /* Detail Info: [to turn on Zi Indonesian inputs] */
   #endif /* __MMI_LANG_INDONESIAN__ */

   #if defined(__MMI_LANG_MALAY__)
   	#define __MMI_ZI_MALAY__										  /* Detail Info: [to turn on Zi Malay inputs]*/
   #endif /* __MMI_LANG_MALAY__ */

   #if defined(__MMI_LANG_VIETNAMESE__)
   	#define __MMI_ZI_VIETNAMESE__									  /* Detail Info: [to turn on Zi Vietnamese inputs] */
   #endif /* __MMI_LANG_VIETNAMESE__ */

   #if defined(__MMI_LANG_ARABIC__)
      #define __MMI_ZI_ARABIC__											/* Detail Info: [turn on Zi Arabic inputs]*/
   #endif /* __MMI_LANG_ARABIC__*/
#endif

#elif defined(__MMI_ITAP__)

#if !defined(__MMI_TOUCH_SCREEN__)
   #if defined(__MMI_LANG_ENGLISH__)
      #define __MMI_ITAP_ENGLISH__										/* Detail Info: [turn on iTap English inputs]*/
   #endif /* __MMI_LANG_ENGLISH__ */
#endif

   #if defined(__MMI_LANG_TR_CHINESE__)
      #define __MMI_ITAP_TR_CHINESE__										/* Detail Info: [turn on iTap Tr Chinese inputs]*/
   #endif

   #if defined(__MMI_LANG_SM_CHINESE__)
      #define __MMI_ITAP_SM_CHINESE__										/* Detail Info: [turn on iTap Sm Chinese inputs]*/
   #endif

#if !defined(__MMI_TOUCH_SCREEN__)
   #define __MMI_ITAP_SMART_PHONETIC_INPUT__						/* Detail Info: [turn on TR/SM Chinese smart phonetic inputs]*/
#endif
#endif

#if !defined(__MMI_TOUCH_SCREEN__)
#if defined(__MMI_LANG_THAI__)
	#define __MMI_MULTITAP_THAI__												/* Detail Info: [turn on Multitap Hindi inputs]*/
#endif /* __MMI_LANG_THAI__ */

#if defined(__MMI_LANG_HINDI__)
	//#define __MMI_MULTITAP_HINDI__												/* Detail Info: [turn on Multitap Hindi inputs]*/
#endif/* __MMI_LANG_HINDI__ */
#endif 

#define __MMI_PREFER_INPUT_METHOD__  									/* Detail Info: [turn on preferred input method setting menu in phone setup]*/
#if defined(CANNON_PLUS_BB) || defined(MILAN_BB)
  #define __MMI_MULTITAP_KEY_0__											/* Detail Info: [change dialling screen mulitap + p w from key star to key-zero]*/
#endif

#if defined(DRAGONFLY_BB) || defined(FIREFLY_BB) || defined (FIREFLY17_DEMO_BB)
  #define __MMI_WITH_C_KEY__												/* Detail Info: [turn on C key function]*/
#endif

#ifndef __MMI_TOUCH_SCREEN__
#define __MMI_FULL_SCREEN_EDITOR__										/* Detail Info: [Hide multitap line in the bottom of input box by default]*/
#endif /* ~ __MMI_TOUCH_SCREEN__ */

 
//tangjie add begin 20060802
#if defined(__MMI_GB__)
   #if defined(__MMI_LANG_ENGLISH__)
      #define __MMI_GB_ENGLISH__												/* Detail Info: [turn on T9 English inputs]*/
   #endif /* __MMI_LANG_ENGLISH__ */

   #if defined(__MMI_LANG_TR_CHINESE__)
   	#define __MMI_GB_TR_CHINESE__										/* Detail Info: [turn on T9 Tr Chinese inputs]*/
   #endif /* __MMI_LANG_TR_CHINESE__ */

   #if defined(__MMI_LANG_SM_CHINESE__)
   	#define __MMI_GB_SM_CHINESE__										/* Detail Info: [turn on T9 Sm Chinese inputs]*/
   #endif /* __MMI_LANG_SM_CHINESE__ */
    #define __MMI_GB_SMART_PHONETIC_INPUT__							/* Detail Info: [turn on TR/SM Chinese smart phonetic inputs]*/

#if defined(__MMI_LANG_PORTUGUESE__)
#define __MMI_GB_SMART_PORTUGUESE__
#endif

#endif
//tangjie add end 20060802

/*------------------------------               ------------------------------*/
/*------------------------------[End Framework]------------------------------*/
/*------------------------------               ------------------------------*/

/*-----------------------------                   ---------------------------*/
/*-----------------------------[Begin Application]---------------------------*/
/*-----------------------------                   ---------------------------*/

/*****************************************************************************
 [Application]: Bootup/Idle
 *****************************************************************************/
#define __MMI_IDLE_FULL_SCREEN__											/* Detail Info: []*/
#define __MMI_KEYPAD_LOCK_PATTERN_1__									/* Detail Info: [Define keypad lock pattern as RSK (Name)->#]*/
#ifdef __MMI_TOUCH_SCREEN__
#define __MMI_KEYPAD_LOCK_PROMPT_TEXT__
#endif
#define __MMI_DEFAULT_MAIN_MENU_HIGHLIGHT_CENTER__ //added by lin @20070815
/*****************************************************************************
 [Application]: Engineer/Factory Mode
 *****************************************************************************/
// ukyo 070425 open for EM/FM
#define __MMI_FACTORY_MODE__												/* Detail Info: []*/  //ssp del 20060526
#define __MMI_ENGINEER_MODE__												/* Detail Info: []*/  //ssp del 20060526
//#define __DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__    // zrx del 20060526 					/* Detail Info: [for fast entry to Factory mode]*/
// TODO: ukyo KKK 070514 
//#define __MOD_NVRAM__
//#define __EM_MODE__
/*****************************************************************************
 [Application]: GPIO/External device
 *****************************************************************************/
#if defined(__PHONE_CLAMSHELL__)
	#define __MMI_CLAMSHELL__												/* Detail Info: [Phone is clamshell one]*/
	#define __MMI_CLAMCLOSE_BACK2IDLE__
#endif
#if defined(__PHONE_SLIDE__)
	#define __MMI_SLIDE__													/* Detail Info: [Phone is a slide one]*/
	#define __MMI_CLAMSHELL__												/* Detail Info: [Phone is clamshell one]*/
	#define __MMI_CLAMCLOSE_BACK2IDLE__
#endif
#define __MMI_STATUS_ICON_EN_EARPHONE__
#ifdef __MMI_CLAMSHELL__
#define __MMI_SYNC_LCD_GPIO_MODE__
#endif

/* diamond, 2005/06/14 keypad lock is combined with clam shell */
#ifdef DEMO_PROJECT
	#ifdef __MMI_CLAMSHELL__
		#ifdef __MMI_SLIDE__
			#define __MMI_AUTO_KEYPAD_LOCK__
		#endif
	#else
		#define __MMI_AUTO_KEYPAD_LOCK__
	#endif
#else
	#define __MMI_AUTO_KEYPAD_LOCK__
#endif
/* end, diamond */


//#define __MMI_GPIO_DISABLE_SLEEP_MODE__								/* Detail Info: [For certain LCM that can retain display when backlight is off] */

/* __MMI_LCD_PARTIAL_ON__ can be used on any bar-type project, but it is recommended to be enabled with
 * __MMI_GPIO_DISABLE_SLEEP_MODE__ for power-saving issue because backlight can be completely turned off.
 */
#ifndef __MMI_CLAMSHELL__
#define __MMI_LCD_PARTIAL_ON__											/* Detail Info: [LCD partial-display screen saver] */
#if defined (__MMI_GPIO_DISABLE_SLEEP_MODE__)
#define __MMI_LCD_PARTIAL_ON_ENABLED__									/* Detail Info: [Enable LCD partial-display screen saver by default] */
#endif /* __MMI_GPIO_DISABLE_SLEEP_MODE__ */
#endif /* ~ __MMI_CLAMSHELL__ */

/* USB: [USB device configure as Mass Storage and COM emulator]
 */
#ifdef __USB_ENABLE__
   #ifndef __MMI_EXT_DEVICE__
      #define __MMI_EXT_DEVICE__											/* Detail Info: [Turn on external device features, before USB, IrDA, Bluetooth turn on, shall enable this compile option first]*/
   #endif
   #define __MMI_USB_SUPPORT__
   #ifdef __P_PROPRIETARY_COPYRIGHT__
   	#define __MMI_USB_COPY_RIGHT__
   #endif										/* Detail Info: [MMI support USB feature]*/
#endif  /* __USB_ENABLE__ */

/***************************************************************************************************************/
/***********************************    BT SUPPORT    *********************************************************/
/***************************************************************************************************************/
#ifdef __MMI_SUPPORT_BLUETOOTH__

#define __BTCOOLSAND__

#define __BT_HFG_PROFILE__
#define __BT_OPP_PROFILE__
#define __BT_FTS_PROFILE__
//#define __BT_FTC_PROFILE__
#define __BT_A2DP_PROFILE__
#define __BT_AVRCP_PROFILE__
//#define  __BT_AUDIO_VIA_SCO__
      
#if defined(__BTCOOLSAND__)

   #ifndef __MMI_EXT_DEVICE__
      #define __MMI_EXT_DEVICE__											/* Detail Info: []*/
   #endif
#define __MMI_BT_SUPPORT__ 												/* Detail Info: [For general bt MMI screen, menu, string, image]*/ 
#define __MMI_BT_COOLSAND_SUPPORT__											/* Detail Info: [For bchs]*/
#define __MMI_EM_BT_RF_TEST__
#define __MMI_EM_BT_GET_CHIP_VERION__
#define __BT_RDA5868__
#endif



#ifdef __MMI_BT_SUPPORT__ 		                                                                    //RDA Bluetooth										

#ifdef __BT_HFG_PROFILE__
#define __MMI_HFP_SUPPORT__												/* Detail Info: [For bchs hfg]*/
//#define __MMI_HSP_SUPPORT__												/* Detail Info: [For bchs ag]*/
#endif
#ifdef __BT_SPP_PROFILE__
#define __MMI_SPP_SUPPORT__												/* Detail Info: [For bchs spp]*/
#endif
#ifdef __BT_DUN_PROFILE__
#define __MMI_DUN_SUPPORT__												/* Detail Info: [For bchs dun]*/
#endif
#ifdef __BT_OPP_PROFILE__
#define __MMI_OPP_SUPPORT__												/* Detail Info: [For bchs opp]*/
#endif
#ifdef __BT_FTS_PROFILE__
#define __MMI_FTS_SUPPORT__												/* Detail Info: [For bchs fts]*/
#endif
#ifdef __BT_FTC_PROFILE__
#define __MMI_FTC_SUPPORT__												/* Detail Info: [For bchs ftc]*/
#define __MMI_FTC_MULTIPULL_SUPPORT__
#endif
#ifdef __BT_A2DP_PROFILE__
#define __MMI_A2DP_SUPPORT__												/* Detail Info: [For bchs a2dp]*/
#endif
#ifdef __BT_AVRCP_PROFILE__												/* Detail Info: [For bchs avrcp]*/
#define __MMI_AVRCP_SUPPORT__
#endif
#ifdef __BT_SIM_PROFILE__												/* Detail Info: [For simap]*/
#define __MMI_SIMAP_SUPPORT__
#endif
#ifdef __BT_FAX_PROFILE__												/* Detail Info: [For fax]*/
#define __MMI_FAX_SUPPORT__
#endif

#ifdef __BT_HIDD_PROFILE__												/* Detail Info: [For hid device]*/
#define __MMI_HIDD_SUPPORT__
#endif


/* prohibit some services while a2dp is active */
#if defined(__MMI_A2DP_SUPPORT__) && defined(__MMI_BT_BCHS_SUPPORT__)
#define __MMI_BT_PROHIBIT_OPP__
#define __MMI_BT_PROHIBIT_FTP__
#define __MMI_BT_PROHIBIT_SPP__
#define __MMI_BT_PROHIBIT_DUN__
#endif

#endif



 #endif
/***************************************************************************************************************/
/***************************************************************************************************************/
/***************************************************************************************************************/
 

/* IrDA: [Transmit/Recv object via Infrared]
 */
#ifdef __IRDA_SUPPORT__

   #ifndef __MMI_EXT_DEVICE__
      #define __MMI_EXT_DEVICE__											/* Detail Info: [Turn on external device features, before USB, IrDA, Bluetooth turn on, shall enable this compile option first]*/
   #endif

   #ifdef __FS_CARD_SUPPORT__
      #define __MMI_IRDA_CARD_SUPPORT__ //Priyesh - Memory Card Support
   #endif

   #define __MMI_IRDA_SUPPORT__											/* Detail Info: [MMI support IrDA feature]*/
   #define __MMI_IRDA_RECEIVED_NOTIFY_IN_IDLE__						/* Detail Info: [Enable notify screen in idle after IrDA received files]*/
   #define __MMI_IRDA_RECEIVED_NOTIFY_POPUP__

#endif  /* __IRDA_SUPPORT__ */


#if defined(TFT_MAINLCD)
   #define __MMI_NO_CONTRAST_LEVEL__   									/* Detail Info: [TFT, No contrast setup function] */
#endif

/* Memory Card
 */
#define __MMI_MEMORY_CARD_SUPPORT__										/* Detail Info: [Support Memory Card]*/
#define __MMI_SLEEP_OUT_BACKLIGHT_ON__  								/* Detail Info: [Backligh control]*/

/*****************************************************************************
 [Application]: PhoneBook
 *****************************************************************************/
#define __IP_NUMBER__
#define __MMI_PHB_CALLERGROUP_MEMBER_LIST__									/* Detail Info: [Edit member list in calle group]*/
#define __MMI_PHB_COMPARE_9_DIGIT__											/* Detail Info: [Compare 9 digits in number field for call]*/
#define __MMI_PHB_QUICK_SEARCH__												/* Detail Info: [Enable Quick search feature]*/
#define __MMI_PHB_ADV_NUM_MATCH_MOBILE__									/* Detail Info: [Compare number filed in advance way for call]*/
#define __MMI_PHB_DELETE_ALL_CHECK_PHONE_LOCK__							/* Detail Info: [Check phone lock when delete all]*/
//#define __MMI_PHB_RSK_QUICK_SEARCH__											/* Detail Info: [Link idle screen RSK to quick search, Must define __MMI_PHB_QUICK_SEARCH__]*/
#define __MMI_PHB_PINYIN_SORT__												/* Detail Info: [Sort phonebook list in Pinyin order]*/
#define __MMI_PHB_SOS_NUM_EDIT__												/* Detail Info: [Allow sos number editable]*/
#define __MMI_PHB_DISPLAY_ONE_MSISDN__										/* Detail Info: [Display one MSISDN only]*/
#define __MMI_PHB_SEND_SMS_FROM_PHB__										/* Detail Info: [Send SMS from phonebook]*/
#if defined(JATAAYU_SUPPORT)
//#define __MMI_PHB_SEND_MMS_FROM_PHB__										/* Detail Info: [Send MMS from phonebook]*/
#endif
//#define __MMI_PHB_COMBINE_COPY_DUPLICATE__									/* Detail Info: [Combine copy and duplicate as one item]*/
//#define __MMI_PHB_NAME_AND_NUM_REQUIRE__										/* Detail Info: [Both name and number required to add a new entry]*/
#define __MMI_PHB_CALLERGROUP_IN_SIM__										/* Detail Info: [Can associate caller group info for entry in SIM card]*/
//added by jackie on 20061205 for add sim entry at caller group
//#define __MMI_PHB_CALLERGROUP_IN_SIM__										/* Detail Info: [Can associate caller group info for entry in SIM card]*/
//#define __MMI_PHB_UPDATE_CALL_LOG_AFTER_SAVE__										/* Detail Info: [Update call log data after save an entry to phb from call log]*/
//#define __MMI_PHB_NO_OPTIONAL_FIELD__										/* Detail Info: [Disable optional field in phonebook.]*/
#define __MMI_CALLERGROUP_NO_ALERT__										/* Detail Info: [No alert setting in caller group]*/
#define __MMI_PHB_CALLER_RES_SETTING__										/* Detail Info: [Select resource first then forward to phonebook entry]*/
#define __MMI_PHB_BIRTHDAY_FIELD__											/* Detail Info: [Turn on birthday field in phonebook field]*/
#define __MMI_PHB_UPDATE_CALL_LOG_AFTER_SAVE__			/*Update Call Log after save number*/
#if defined(__IP_NUMBER__)
	#define __MMI_PHB_IP_DIAL_FROM_PHB__											/* Detail Info: [Make IP dial from phonebook]*/
#endif

#if (defined(MP4_CODEC) || defined(MJPG_SUPPORT)) && defined(__MMI_DATA_STORE__) && defined(__MMI_FILE_MANAGER__)
  //#define __MMI_INCOMING_CALL_VIDEO__                                   /* Detail Info: [Show video for calling screen]*/
#endif

#if defined(__MMI_FILE_MANAGER__)
   #define __MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__ 				/* Detail Info: [Show Picture from file for calling screen]*/
#endif

#define __MMI_VCARD__															/* Detail Info: [Enable vCard feature]*/  //ssp delete 20060512
/*****************************************************************************
 [Application]: SMS/EMS/Email/IMPS
 *****************************************************************************/
/* SMS/EMS/CB/VoiceMail
 */
#ifdef __MOD_SMSAL__
   /*Wangzl Add Start For 5924 Ver: TBM_780  on 2007-8-7 10:1 */
   #define __MMI_MESSAGE_DIFFERENTIATE_STORAGE__
   /*Wangzl Add End  For 5924 Ver: TBM_780  on 2007-8-7 10:1 */
   #define __MMI_MESSAGES_CONCAT_SMS__	//wangwb @2007.06.12 open								/* Detail Info: [1.Concatenated Messages are  supported] */
   //#define __MMI_MESSAGES_EMS__	// wangbei close 20060621										/* Detail Info: [2.EMS, including text format, can be turned off]*/
   #define __MMI_MESSAGES_TEMPLATE__									/* Detail Info: [3.Preset Message can be removed] */
   #define __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__			/* Detail Info: [4.Support SMS storage only in SIM(Thus Preferred Storage can be removed), 10.No Memory Status is shown] */
   #define __MMI_MESSAGES_CB_MSG_STORED_LIST__						/* Detail Info: [7.CB support only Receive Mode and Channel List. No CB messages are stored] */
   #define __MMI_MESSAGES_CB_CHNL_WITH_NAME__						/* Detail Info: [8.For Channel List channel numbers are displayed. (No channel names are stored)]*/
   #define __MMI_MESSAGES_TWO_CB_MODES_DISPLAY__					/* Detail Info: [Support immediate mode and normal mode cb display ]*/
   #define __MMI_MESSAGES_SEND_BY_GROUP__								/* Detail Info: [9.Send by Group and] */
   #define __MMI_MESSAGES_SEND_TO_MANY__								/* Detail Info: [9.Send to Many] */
   #define __MMI_MESSAGES_SEND_TO_MANY_BY_LIST__					/* Detail Info: []*/
   #define __MMI_MESSAGES_INSERT_FROM_PHB__
   #define _MUTILANG_TEMPLATE_											/* Detail Info: []*/
   #if defined (__MMI_MESSAGES_SEND_TO_MANY__)||defined(__MMI_MESSAGES_SEND_BY_GROUP__)
   #define __MMI_MESSAGES_CONTINUE_SEND_AFTER_FAIL__
   #endif
 #if !defined(NEPTUNE_COLOR_BB)
   #define __MMI_MESSAGES_CHAT__											/* Detail Info: []*/
 #endif
   #define __MMI_MESSAGES_CHAT__
   #define __MMI_MESSAGES_SEND_BY_GROUP__

 #if !defined(MT6205B)
   #define __MMI_MESSAGES_COPY__											/* Detail Info: [ SMS COPY and MOVE function]*/
   #if defined(WAP_SUPPORT)
      #define __MMI_MESSAGES_USE_URL__                                                                        /* Detail Info: [Extract url from SMS context and link it to WAP browser]*/
   #endif
 #endif // #if !defined(MT6205B)

 #ifdef DEMO_PROJECT
   #define __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__				/* Detail Info: [Single delivery report for concatenated SMS]*/
 #endif  // #ifdef DEMO_PROJECT

#endif // #ifdef __MOD_SMSAL__
/* Email
 */
#ifdef __EMAIL__
	#define __MMI_EMAIL__													/* Detail Info: [Turn on Email application]*/
	#ifdef __NO_IMAP__
		#define __MMI_SLIM_EMAIL_NO_IMAP4__									/* Detail Info: [Turn off IMAP4 support]*/
	#endif
#endif

/* IMPS
 */
#ifdef __IMPS__
	#define	__MMI_IMPS__														/* Detail Info: [Turn on IMPS application]*/
#endif

/* Auxiliary compile option
*/
#if defined(MMS_SUPPORT) || defined(__MMI_MESSAGES_CHAT__) || defined(__MMI_EMAIL__) || defined(__MMI_IMPS__)
	#define __MMI_MESSAGES_MULTI_APP__                          /* Detail Info: [Auxiliary compile option to indicate multiple applications turned on in Message menu]*/
#endif



/*****************************************************************************
 [Application]: Call Logs, Call Management
 *****************************************************************************/
/* Call History */
// zrx add 20060612
#define PHB_LN_ENTRY 10

//#define MAX_PHB_SIM_ENTRY   255		//2006-06-28, Han ShiLin, modify MAX_PHB_SIM_ENTRY from 255 to 260
#define MAX_PHB_SIM_ENTRY   260
#define  MAX_PHB_SIM2_ENTRY 260

#ifdef PHB_LN_ENTRY
   #if 0
   #define MAX_PHB_LN_ENTRY 10
   #else
   #define MAX_PHB_LN_ENTRY 20
   #endif
 #endif

 //#define CSD_NEW_API                            //tangjie del 20060615
// zrx add end
#if (MAX_PHB_LN_ENTRY <= 20)
	//2007-06-15, Li Tao
	#define __MMI_CH_MIXED_CALL_LOG__                           /* Detail Info: [Turn on Mixed Call Log after pressing SEND key in idle screen] */
#endif
	#define __MMI_CH_DELETE_ALL_LOGS__                          /* Detail Info: [Turn on Delete All Logs in call history -> delete call log]*/
#ifdef DEMO_PROJECT
        #define __MMI_CH_QUICK_END__
#endif

#if defined(MT6205B)
   #define __MMI_CM_DATA_CALL_ONLY_PLAY_TONE__
#endif

#define __MMI_MT_CALL_SPEAKER_MUTE__    // incomming call mute    guojian added 20081231
#define __MMI_CALL_MO_LOUDSPK			//outgoing call loud speaker

#define __MMI_AUTO_PLUS_ENABLE__
/*****************************************************************************
 [Application]: Organizer
 *****************************************************************************/
/* Organizer
 */
 #define __MMI_OPERATOR_H__
 #ifdef __MMI_OPERATOR_H__
 	#define  _OPERATOR_TEST_
	#define  _MMI_OPERATOR_
#endif
#ifdef __MOD_FLASHLIGHT__                                    /* Detail Info: [Turn on Flashlight application] added by kecx for flashlight function on 20081110 */
	#define __MMI_FLASHLIGHT__
#endif
#define __MMI_BMI__														/* Detail Info: [Turn on BMI application]*/
#define __MMI_MENSTRUAL__												/* Detail Info: [Turn on Menstrual application]*/
#define __MMI_CALCULATOR__												/* Detail Info: [Turn on Calculator application]*/
#if defined (__MMI_TOUCH_SCREEN__) && defined (__MMI_CALCULATOR__)
	#define __MMI_TOUCH_CALCULATOR__
#endif
#define __MMI_TODOLIST__													/* Detail Info: [Turn on To Do List application]*/
#ifdef __MMI_TODOLIST__
	//#define __MMI_VCALENDAR__												/* Detail Info: [Turn on vCalendar Application]*/
#endif
#define __MMI_CALENDAR__													/* Detail Info: [Turn on Calendar application]*/
#ifdef __MMI_CALENDAR__
	#define __MMI_SPECIFIC_CALENDAR__									/* Detail Info: [Display solarterm by lunar date]*/
#endif
#define __MMI_UNIT_CONVERTER__											/* Detail Info: [Turn on Unit Converter]*/
#define __MMI_CURRENCY_CONVERTER__										/* Detail Info: [Turn on Currency Converter]*/
#define __MMI_WORLD_CLOCK__												/* Detail Info: [Turn on World Clock]*/
#define __MMI_WCLOCK_CITY_LIST_3__										/* Detail Info: [Use City List 3 as default world clock]*/
//#define __MMI_CHANGABLE_FONT__											/* Detail Info: []*/
/* NITZ
 */
#ifdef __MMI_WORLD_CLOCK__
   #define __MMI_NITZ__														/* Detail Info: [Turn on NITZ feature]*/
   #define __MMI_AUTOTIMEZONE_IN_DATEANDTIME__
#endif
//qiff add alarm smooze feature 2008-12-11
#define __MMI_ALM_SNOOZE_SETTING__

#define __MMI_EBOOK_READER__
/*****************************************************************************
 [Application]: Fun & Game
 *****************************************************************************/

#define __MMI_GAME__ 			
/* Detail Info: [Turn On/Off Games] */
#ifdef __MMI_GAME__

		#define 	__MMI_GAME_MULTICHANNEL_SOUND__
	#if !defined(MT6205B)												/* need multi-layer */
	#endif /* !MT62105B */

	#if defined(__MMI_MAINLCD_128X160__)
		/* 128x160 games */
		//#define __MMI_GAME_COPTER_128x160__							/* Detail Info: [Copter Game 					(128x160)] */
		//#define __MMI_GAME_SUBMARINE_128x160__						/* Detail Info: [Copter (Submarine) Game 	(128x160)] */
		//#define __MMI_GAME_JET_128x160__									/* Detail Info: [Copter (Jet) Game 			(128x160)] */
		#define __MMI_GAME_PUZZLE_128x160__								/* Detail Info: [Puzzle Game 					(128x160)] */
		//#define __MMI_GAME_CHICKEN_128x160__							/* Detail Info: [Stair (Chicken) Game 		(128x160)] */
		//#define __MMI_GAME_ROBOT_128x160__							/* Detail Info: [Stair (Robot) Game 		(128x160)] */
		//#define __MMI_GAME_PANDA_128x160__							/* Detail Info: [Panda Game 					(128x160)] */
		//#define __MMI_GAME_MONKEY_128x160__								/* Detail Info: [Panda (Monkey) Game 		(128x160)] */
		//#define __MMI_GAME_UFO_128x160__									/* Detail Info: [Ufo Game						(128x160)] */
	#elif defined(__MMI_MAINLCD_176X220__)
		/* 176x220 Games */
		//#define __MMI_GAME_DANCE_176x220__							/* Detail Info: [Dance Game					(176x220)] */
		// dingjian 20071217 for lcd szie is 176*220
		//#define __MMI_GAME_COPTER_176x220__								/* Detail Info: [Copter Game					(176x220)] */
		//#define __MMI_GAME_SUBMARINE_176x220__						/* Detail Info: [Copter (Submarine) Game	(176x220)] */
		//#define __MMI_GAME_JET_176x220__								/* Detail Info: [Copter (Jet) Game			(176x220)] */
		#define __MMI_GAME_PUZZLE_176x220__								/* Detail Info: [Puzzle Game 					(176x220)] */
		//#define __MMI_GAME_ROBOT_176x220__							/* Detail Info: [Stair (Robot) Game 		(176x220)] */
		// dingjian 20071217 for lcd szie is 176*220
		//#define __MMI_GAME_PANDA_176x220__								/* Detail Info: [Panda Game 					(176x220)] */
		//#define __MMI_GAME_MONKEY_176x220__							/* Detail Info: [Panda (Monkey) Game 		(176x220)] */
		#define __MMI_GAME_UFO_176x220__									/* Detail Info: [Ufo Game 						(176x220)] */
	#elif defined(__MMI_MAINLCD_220X176__)
		/* 220x176 Games */
		//#define __MMI_GAME_DANCE_220x176__							/* Detail Info: [Dance Game						(220x176)] */
		//#define __MMI_GAME_COPTER_220x176__							/* Detail Info: [Copter Game					(220x176)] */
		//#define __MMI_GAME_SUBMARINE_220x176__						/* Detail Info: [Copter (Submarine) Game			(220x176)] */
		//#define __MMI_GAME_JET_220x176__								/* Detail Info: [Copter (Jet) Game				(220x176)] */
		#define __MMI_GAME_PUZZLE_220x176__							/* Detail Info: [Puzzle Game 					(220x176)] */
		//#define __MMI_GAME_ROBOT_176x220__							/* Detail Info: [Stair (Robot) Game 				(220x176)] */
		//#define __MMI_GAME_PANDA_220x176__							/* Detail Info: [Panda Game 					(220x176)] */
		//#define __MMI_GAME_MONKEY_220x176__							/* Detail Info: [Panda (Monkey) Game 			(220x176)] */
		#define __MMI_GAME_UFO_220x176__								/* Detail Info: [Ufo Game 						(220x176)] */		
	#elif defined(__MMI_MAINLCD_240X320__)
		/* 240x320 Games */
// close for lcd 240*320
// dingjian 20071026
		//#define __MMI_GAME_MAGICSUSHI__								/* Detail Info: [Magicsushi Game 			(240x320)] */
		#define __MMI_GAME_PUZZLE_240x320__							/* Detail Info: [Puzzle Game 			(240x320)] */
		//#define __MMI_GAME_MAJUNG_240x320__                                           /* Detail Info: [Mah-jong Game 			(240x320)] */
	#else
		/* 128x128 games */
		//#define __MMI_GAME_F1RACE__										/* Detail Info: [F1 Race Game					(128x128)] */
		#define __MMI_GAME_HANOI__											/* Detail Info: [Hanoi Game 					(128x128)] */
		#define __MMI_GAME_RICHES__										/* Detail Info: [Riches Game					(128x128)] */
		//#define __MMI_GAME_COLORBALLS__									/* Detail Info: [ColorBall Game			 	(128x128)] */
		//#define __MMI_GAME_SNAKE__											/* Detail Info: [Snake Game					(128x128)] */
		//#define __MMI_GAME_SMASH__											/* Detail Info: [Smash Game 					(128x128)] */
		//#define __MMI_GAME_DOLL__											/* Detail Info: [Doll Game 					(128x128)] */
		//#define __MMI_GAME_COPTER_128x128__							/* Detail Info: [Copter Game 					(128x128)] */
		//#define __MMI_GAME_SUBMARINE_128x128__						/* Detail Info: [Copter (Submarine) Game 	(128x128)] */
		//#define __MMI_GAME_JET_128x128__									/* Detail Info: [Copter (Jet) Game 			(128x128)] */
		#define __MMI_GAME_PUZZLE_128x128__								/* Detail Info: [Puzzle Game 					(128x128)] */
		//#define __MMI_GAME_CHICKEN_128x128__							/* Detail Info: [Stair (Chicken) Game		(128x128)] */
		#define __MMI_GAME_PANDA_128x128__								/* Detail Info: [Panda Game 					(128x128)] */
		//#define __MMI_GAME_MONKEY_128x128__							/* Detail Info: [Panda (Monkey) Game		(128x128)] */
		#define __MMI_GAME_UFO_128x128__									/* Detail Info: [Ufo Game 						(128x128)] */

		/* 128x96 games */
		//#define __MMI_GAME_NINJA_128x96__								/* Detail Info: [Ninja Game 	(128x96)] */
		#define __MMI_GAME_MAJUNG_128x96__								/* Detail Info: [Majung Game 	(128x96)] */
		//#define __MMI_GAME_MEMORY_128x96__								/* Detail Info: [Memory Game 	(128x96)] */
	#endif
#endif /* __MMI_GAME__ */

//#define __MMI_IMELODY_SUPPORT__		// zrx del 20060612									/* Detail Info: [Support IMelody]*/

/* Ring composer
 */
//#define __MMI_RING_COMPOSER__			//dyj del 20060514									/* Detail Info: [Turn on Ring Comopser]*/
#if (defined  __MMI_TOUCH_SCREEN__) && (defined __MMI_RING_COMPOSER__)
#define __MMI_TOUCH_RING_COMPOSER__
#endif
/* Theme
 */
#define __MMI_THEMES_APPLICATION__										/* Detail Info: [Turn on Theme application]*/
/* GUI
 */
#define __MMI_DEFAULT_THEME_1__											/* Detail Info: []*/
/* Download
 */
#if (defined(__MMI_MESSAGES_EMS__) || defined(__MMI_WAP__)) && !defined(__MMI_FILE_MANAGER__)
  // #define __DOWNLOAD__		//dyj del 20060514													/* Detail Info: [Turn on Download application]*/
#endif
/* Stopwatch
 */
 #define __MMI_STOPWATCH__	//guojian added for jasper 20080225
#if !defined(MT6205B)
	#define __MMI_STOPWATCH__													/* Detail Info: [Enalbe Stopwatch Application]*/
#endif
/* Picture Editor
 */
#if !defined(MT6205B)
	#define __MMI_PICTURE_EDITOR_SUPPORT__									/* Detail Info: []*/
#endif


/*****************************************************************************
 [Application]: Profiles
 *****************************************************************************/
#define __MMI_PROFILE_EXTMELODY_SUPPORT__ 							/* Detail Info: [For profile set digital audio] */
//#define __MMI_STATUS_LED__													/* Detail Info: [Enable Light item in profile menu]*/
#if defined( __MMI_A2DP_SUPPORT__ ) || defined( __MMI_HFP_SUPPORT__	)	
#define __MMI_BT_PROFILE__													/* Detail Info: [MMI BT Profile]*/
#endif

#if defined(__BT_AUDIO_VIA_SCO__) && defined(__MMI_HFP_SUPPORT__)
#define __MMI_BT_AUDIO_VIA_SCO__
#endif

#if defined(__BT_FM_VIA_SCO__) && defined(__MMI_HFP_SUPPORT__)
#define __MMI_BT_FM_VIA_SCO__
#endif

/* For KeyPad Tone Human Speech
 */
//#define __MMI_KEYPAD_TONE_HUMAN_VOICE_1__
//#define __MMI_KEYPAD_TONE_HUMAN_VOICE_2__
//#define __MMI_KEYPAD_TONE_HUMAN_VOICE_3__
//#define __MMI_KEYPAD_TONE_HUMAN_VOICE_4__
//#define __MMI_KEYPAD_TONE_HUMAN_VOICE_5__
//#define __MMI_KEYPAD_TONE_HUMAN_VOICE_6__
#ifdef DIGIT_TONE_SUPPORT
#define __MMI_HUMAN_VOICE_KEYPAD_TONE__
#endif
#if defined(DIGIT_TONE_SUPPORT) || defined(SIMPLE_TTS)
#define __MMI_INTELLIGENT_CALL_ALERT__
#endif
#if defined(CUST_KEYPAD_TONE_SUPPORT)
#define __MMI_CUST_KEYPAD_TONE__
#endif

/*****************************************************************************
 [Application]: Settings
 *****************************************************************************/

/*****************************************************************************
 [Application]: Services
 *****************************************************************************/
/* MMS
 */
#ifdef MMS_SUPPORT
   #define __MMI_MMS__										/* Detail Info: [Teleca MMS support]*/
   #define __MMI_STATUS_ICON_MMS__								/* Detail Info: [MMS status icons]*/
   #define __MMI_MMS_TEMPLATES_NUM__ 5								/* Detail Info: [MMS templates number]*/
   #define __MMI_MMS_SIGNATURE__								/* Detail Info: [MMS Auto Signature]*/
#endif /* MMS_SUPPORT */

//micha_PoC
#ifdef __POC__
#define __MMI_POC__
#endif


/*****************************************************************************
 [Application]: Encoding
 *****************************************************************************/
#if (defined(__MMI_LANG_TR_CHINESE__) && defined(__MMI_LANG_SM_CHINESE__) )
#define __MMI_SET_DEF_ENCODING_TYPE__  								/* Detail Info: [PhoneSetup set default encoding type] */
#endif

#if defined(__MMI_LANG_ARABIC__)
//#define __MMI_CHSET_ARABIC_ISO__											/* Detail Info: [Turn on Arabic ISO charset]*/
//#define __MMI_CHSET_ARABIC_WIN__											/* Detail Info: [Turn on Arabic WIN charset]*/
#endif
//#define __MMI_CHSET_BALTIC_ISO__											/* Detail Info: [Turn on Baltic ISO charset]*/
//#define __MMI_CHSET_BALTIC_WIN__											/* Detail Info: [Turn on Arabic WIN charset]*/
#if defined(__MMI_LANG_TR_CHINESE__)
	#define __MMI_CHSET_BIG5__													/* Detail Info: [Turn on GIB5 charset]*/
#endif
#if defined(__MMI_LANG_SM_CHINESE__)
	#define __MMI_CHSET_GB2312__												/* Detail Info: [Turn on GB2312 charset]*/
#endif
//#define __MMI_CHSET_CEURO_ISO__											/* Detail Info: [Turn on Centrual European ISO charset]*/
//#define __MMI_CHSET_CEURO_WIN__											/* Detail Info: [Turn on Centrual European WIN charset]*/
//#define __MMI_CHSET_GREEK_ISO__											/* Detail Info: [Turn on Greek ISO charset]*/
//#define __MMI_CHSET_GREEK_WIN__											/* Detail Info: [Turn on Greek WIN charset]*/
//#define __MMI_CHSET_HEBREW_ISO__											/* Detail Info: [Turn on Hebrew ISO charset]*/
//#define __MMI_CHSET_HEBREW_WIN__											/* Detail Info: [Turn on Hebrew WIN charset]*/
//#define __MMI_CHSET_LATIN_ISO__											/* Detail Info: [Turn on Latin ISO charset]*/
//#define __MMI_CHSET_NORDIC_ISO__											/* Detail Info: [Turn on Nordic ISO charset]*/
//#define __MMI_CHSET_SEURO_ISO__											/* Detail Info: [Turn on Sourth European ISO charset]*/
#if defined(__MMI_LANG_TURKISH__)
//#define __MMI_CHSET_TURKISH_ISO__    									/* Detail Info: [Turn on Turkish ISO charset]*/
//#define __MMI_CHSET_TURKISH_WIN__										/* Detail Info: [Turn on Turkish WIN charset]*/
#endif
//#define __MMI_CHSET_WESTERN_ISO__										/* Detail Info: [Turn on Western ISO charset]*/
#define __MMI_CHSET_WESTERN_WIN__										/* Detail Info: [Turn on Western WIN charset]*/

/*****************************************************************************
 [Application]: MISC
 *****************************************************************************/
#ifdef __GPRS_MODE__
   #define __MMI_GPRS_FEATURES__											/* Detail Info: [Turn On MMI GPRS Features]*/
#endif

#define _NETWORK_CIPHER_SUPPORT_

/*------------------------------                 ----------------------------*/
/*------------------------------[End Application]----------------------------*/
/*------------------------------                 ----------------------------*/

/*-----------------------------                  ----------------------------*/
/*-----------------------------[Begin Multimedia]----------------------------*/
/*-----------------------------                  ----------------------------*/
/*****************************************************************************
 [Multimedia]: Multimedia
 *****************************************************************************/
#ifdef FM_RADIO_ENABLE
   #define __MMI_FM_RADIO__
   #define __MMI_FM_RADIO_BIND_EARPHONE__												/* Detail Info: []*/

//   #define __MMI_ALM_AUDIO_OPTIONS__		//guojian deleted for jassmine alarm 20080408										/* Detail Info: [Set FM Radio as alarm]*/
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	#define __MMI_ALM_CUSTOMIZE_TONE__											/* Detail Info: [Move alarm ring tone setting from profile to alarm]*/
#endif

#ifdef FM_RADIO_RECORD
   #define __MMI_FM_RADIO_RECORD__												/* Detail Info: [FM Radio Recording]*/
   #define __MMI_FM_RADIO_SCHEDULE_REC__											/* Detail Info: [Schedule FM Radio Recording]*/
#endif

#ifdef __MMI_MAINLCD_240X320__
	#define __MMI_FM_RADIO_ANIMATION__												/* Detail Info: [Animation background for FM Radio]*/
#endif
#endif

#define __MMI_SLIM_AUDIO_PLAYER__

//lyric don't support other lcd size except 240x320 now 2009-11-17
#if (defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_176X220__))
#define  __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
#endif

#ifdef DAF_DECODE
   /* Audio Player
   */
   #define __MMI_AUDIO_PLAYER__									/* Detail Info: []*/
   #define __MMI_DEDICATED_KEY_SHORTCUTS__							/* Detail Info: []*/
   #define __MMI_AUDIO_PLAYER_DETAIL_SHOW_MORE_INFO__				/* Detail Info: []*/
   /*
   * Audio Player Skin Animation feature
   */
   #ifdef __MMI_MAINLCD_176X220__
      #define __MMI_AUDIO_PLAYER_ANIMATION__
   #endif
   #ifdef __MMI_MAINLCD_220X176__
      #define __MMI_AUDIO_PLAYER_ANIMATION__
      #define __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__											/* Detail Info: []*/
   #endif
   /* AB Repeater
   */
  // #define __MMI_AB_REPEATER__
#endif
#if defined(__MMI_MAINLCD_176X220__)||defined(__MMI_MAINLCD_240X320__)
#define __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__											/* Detail Info: []*/
#endif
/* Sound Recorder
 */
#define __MMI_SOUND_RECORDER__	 									/* Detail Info: []*/  //ssp delete 20060512
//#define __MMI_SOUND_RECORDER_STYLE_X__
#if defined( DEDI_AMR_REC )
#define __MMI_DEDI_AMR_REC__           /* Detail Info: [to turn on AMR record format in dedicated mode] */
#endif


/* Camera App
 */
#if ( defined(CAMERA_MODULE) || defined(ISP_SUPPORT) )
   #define __MMI_CAMERA__											/* Detail Info: [Camera App: (1)Camera , (2)Image Viewer] */
	//#define __MMI_CAMERA_23KEY__								/* Detail Info: [Only 23Keys, will use RSK to capture, IDLE key to leave] */
   	//#define __MMI_PHOTOEDITOR__				//added by guojian for jasmine 20080305				/* Detail Info: [Photo Editor App] */

/* camera app will exit preview if no operation in specific time */
#define __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__

	#if (!defined(MT6205B) && !defined(MT6208) && !defined(MT6217) && !defined(MT6218B))
   	#define __MMI_PHOTOEDITOR__								/* Detail Info: [Photo Editor App] */
        #endif
#endif

/* TV Out
 */
#if defined(MT6228) || defined(MT6229)
	#define __MMI_TVOUT__
#endif

/* WebCam
 */
#if defined(WEBCAM_SUPPORT)
	#define __MMI_WEBCAM__
#endif

/* Video App
 */
#if ( (defined(ISP_SUPPORT)&&(MP4_CODEC)) || (defined(CAMERA_MODULE) && defined(MJPG_SUPPORT)) )
	   #define __MMI_VIDEO__										/* Detail Info: [Video App: (1)Video Recoder, (2)Video Player] */
	//#define __MMI_VIDEO_23KEY__									/* Detail Info: [Only 23Keys, will use RSK to record, IDLE key to leave] */
	  #define __MMI_VIDEO_PLAYER__
#endif

/* GDI memory profile
 */
/* turn multilayer for pc simulator */
#ifdef MMI_ON_WIN32
   	#define __GDI_MEMORY_PROFILE_2__									/* Detail Info: [support 2 main lcd size layer, decoder resizer(media task), camera] */
#endif
#if ( !defined(MT6205B) && !defined(MT6208))

   #if (	defined(JPG_DECODE) )
   //	#define __GDI_MEMORY_PROFILE_2__									/* Detail Info: [support 2 main lcd size layer, decoder resizer(media task), camera] */
   #else
   	//#define __GDI_MEMORY_PROFILE_1__									/* Detail Info: []*/
   #endif

#else
   #define __GDI_MEMORY_PROFILE_1__										/* Detail Info: [base layer only] */
#endif
#define __GDI_MEMORY_PROFILE_1__   // zrx add 20060526
#ifdef DEMO_PROJECT
//#define __MMI_SMARTMESSAGE__
//#define __MMI_SMART_MESSAGE_MO__
//#define __MMI_SMART_MESSAGE_MT__
//#define __MMI_MYPICTURE__
//#defined __MMI_MYTONE__	/* Only in Mytone.c and Mytone.h */
#if defined (__MMI_SMART_MESSAGE_MO__) || defined (__MMI_SMART_MESSAGE_MT__)
#define __MMI_OPERATOR_LOGO__
#define __MMI_CLI_ICON__
#endif
#endif
#if !defined(__MMI_MESSAGES_EMS__) && !defined(__MMI_SMART_MESSAGE_MO__)
	#define __MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__
#endif
/*
 * Sound Effect / Audio EQ features
 */
 #define __MMI_SOUND_EFFECT__    //guojian added for jasper 20080228
#if !defined(MT6205B) && !defined(MT6208)
#define __MMI_SOUND_EFFECT__
#endif

/* Voice Recognition: Depends on VR SD engine
 */
#if defined(VR_ENABLE)
   #define __MMI_VRSD__												/* Detail Info: [To turn on voice recognition feature and its main function]*/
#endif

#if defined(__MMI_VRSD__)
	#define __MMI_VRSD_DIAL__										/* Detail Info: [To turn on voice dial feature]*/
	#define __MMI_VRSD_REDIAL__									/* Detail Info: [To turn on voice redial feature]*/
	#define __MMI_VRSD_PROFILE__								/* Detail Info: [To turn on voice profile feature]*/
	#if defined(__MMI_AUDIO_PLAYER__)
		#define __MMI_VRSD_AUDPLAYER__								/* Detail Info: [To turn on voice audio player feature]*/
	#endif
	#define __MMI_VRSD_SHORTCUT__								/* Detail Info: [To turn on voice shortcut feature]*/
#endif

/* Voice Recognition: Depends on VR SI engine (Speaker Independent)
 */
#if defined( VRSI_ENABLE)
	#define __MMI_VRSI__						/* Detail Info: [Turn on vrsi engine if Cyberon engine is on.]*/
	#define __MMI_VRSI_DIGIT_ADAPT__		/* Detail Info: [For Digit Adapt]*/
	#define __MMI_VRSI_TRAIN_TAG__		/* Detail Info: [For Training SD Tag]*/
#endif
#define __MMI_CM_BLACK_LIST__   //added by lin for black list@20070614


#if (MMI_SIM_NUMBER == 1)
#define MULTI_MENU(menu) menu
#define MULTI_STRING(string) string
#elif (MMI_SIM_NUMBER == 2)
#define __MMI_DUAL_PROFILE_SUPPORT__
#define MULTI_MENU(menu) menu,\
							menu + 1
#define MULTI_STRING(string) string,\
							string + 1
#elif (MMI_SIM_NUMBER == 3)
#define MULTI_MENU(menu) menu,\
							menu + 1,\
							menu + 2
#define MULTI_STRING(string) string,\
							string + 1,\
							string + 2
#elif (MMI_SIM_NUMBER == 4)
#define MULTI_MENU(menu) menu,\
							menu + 1,\
							menu + 2,\
							menu + 3
#define MULTI_STRING(string) string,\
							string + 1,\
							string + 2,\
							string + 3
#endif



#define __MMI_NOTEPAD__

//disable LCD rotate
#if 0
#ifndef __MMI_SCREEN_ROTATE__
#define __MMI_SCREEN_ROTATE__
#endif

#if defined(__MMI_SCREEN_ROTATE__) && !defined(__MMI_WALLPAPER_ON_BOTTOM__)
#define __MMI_WALLPAPER_ON_BOTTOM__
#endif
#endif

#endif /* MMI_FEATURES_H */

#ifndef __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__
	#define __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__   //view missed call icon on idle when reboot
#endif
#define __MMI_KEYPADLOCK_WITH_KEYTONE__

//#define __MMI_STOP_SIGNAL_REPORT__
//#define __MMI_STATUS_LED__
//#define __FT_IDLE_SMART_DIAL__
//#define __FT_MMI_DIALING_SCREEN_EXT__
#ifdef __FT_MMI_DIALING_SCREEN_EXT__
#undef __FT_IDLE_SMART_DIAL__
#undef __FT_MMI_SMART_DIALER_SCREEN__
#undef __FT_MMI_SMARTDIAL_TOUCHPANEL__
//#define __FT_ENHANCED_DIALNUMBER_STYLE__
#define __FT_MMI_DIALING_SCREEN_EXT_UNABLE_DIAL_ICON__
#endif
#ifdef __FT_IDLE_SMART_DIAL__
	#define __FT_MMI_SMART_DIALER_SCREEN__		
#ifdef __MMI_TOUCH_SCREEN__
	//#define __FT_MMI_SMARTDIAL_TOUCHPANEL__ 
	#define __SLT_MMI_SMARTDIAL_TOUCHPANEL__
#endif
#endif
//#define DOUBLE_SIM_CARD_SUPPORT    //added by renyh for greenstone @ 2008-08-15
/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-14 9:49 */
#include "mmi_features_gui.h"
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-14 9:49 */
/*------------------------------                -----------------------------*/
/*------------------------------[End Multimedia]-----------------------------*/
/*------------------------------                -----------------------------*/
#define coolsand_UI_3D_circular_menu_goto_item                     gui_3D_circular_menu_goto_item                     
#define coolsand_UI_3D_circular_menu_goto_next_item                gui_3D_circular_menu_goto_next_item                
#define coolsand_UI_3D_circular_menu_goto_previous_item            gui_3D_circular_menu_goto_previous_item            
#define coolsand_UI_3D_circular_menu_translate_pen_event           gui_3D_circular_menu_translate_pen_event           
#define coolsand_UI_add_cleanup_hook                               gui_add_cleanup_hook                               
#define coolsand_UI_add_item_tab_pane                              gui_add_item_tab_pane                              
#define coolsand_UI_add_pre_cleanup_hook                           gui_add_pre_cleanup_hook                           
#define coolsand_UI_associate_tab_pane_item_data                   gui_associate_tab_pane_item_data                   
#define coolsand_UI_associate_tab_pane_type_data                   gui_associate_tab_pane_type_data                   
#define coolsand_UI_asyncdynamic_list_abort_scroll_timer           gui_asyncdynamic_list_abort_scroll_timer           
#define coolsand_UI_asyncdynamic_list_menu_goto_item               gui_asyncdynamic_list_menu_goto_item               
#define coolsand_UI_asyncdynamic_list_menu_goto_next_item          gui_asyncdynamic_list_menu_goto_next_item          
#define coolsand_UI_asyncdynamic_list_menu_goto_previous_item      gui_asyncdynamic_list_menu_goto_previous_item      
#define coolsand_UI_asyncdynamic_list_menu_locate_highlighted_item gui_asyncdynamic_list_menu_locate_highlighted_item 
#define coolsand_UI_asyncdynamic_list_menu_locate_next_item        gui_asyncdynamic_list_menu_locate_next_item        
#define coolsand_UI_asyncdynamic_list_menu_locate_previous_item    gui_asyncdynamic_list_menu_locate_previous_item    
#define coolsand_UI_asyncdynamic_list_menu_scroll_by_pen           gui_asyncdynamic_list_menu_scroll_by_pen           
#define coolsand_UI_asyncdynamic_list_menu_scroll_timer_hdlr       gui_asyncdynamic_list_menu_scroll_timer_hdlr       
#define coolsand_UI_asyncdynamic_list_menu_translate_pen_event     gui_asyncdynamic_list_menu_translate_pen_event     
#define coolsand_UI_asyncdynamic_list_menu_translate_pen_position  gui_asyncdynamic_list_menu_translate_pen_position  
#define coolsand_UI_asyncdynamic_list_start_scroll_timer           gui_asyncdynamic_list_start_scroll_timer           
#define coolsand_UI_autoresize_pop_up_description                  gui_autoresize_pop_up_description                  
#define coolsand_UI_blend_two_color                                gui_blend_two_color                                
#define coolsand_UI_block_list_effect                              gui_block_list_effect                              
#define coolsand_UI_block_transparency_effect                      gui_block_transparency_effect                      
#define coolsand_UI_break_inline_caption                           gui_break_inline_caption                           
#define coolsand_UI_button_show_down_by_pen                        gui_button_show_down_by_pen                        
#define coolsand_UI_button_show_up_by_pen                          gui_button_show_up_by_pen                          
#define coolsand_UI_button_translate_pen_event                     gui_button_translate_pen_event                     
#define coolsand_UI_calendar_draw_info_box                         gui_calendar_draw_info_box                         
#define coolsand_UI_calendar_go_to_day                             gui_calendar_go_to_day                             
#define coolsand_UI_calendar_highlight_day                         gui_calendar_highlight_day                         
#define coolsand_UI_calendar_next_day                              gui_calendar_next_day                              
#define coolsand_UI_calendar_next_row                              gui_calendar_next_row                              
#define coolsand_UI_calendar_previous_day                          gui_calendar_previous_day                          
#define coolsand_UI_calendar_previous_row                          gui_calendar_previous_row                          
#define coolsand_UI_calendar_refresh_highlight                     gui_calendar_refresh_highlight                     
#define coolsand_UI_calendar_translate_pen_event                   gui_calendar_translate_pen_event                   
#define coolsand_UI_calendar_translate_pen_position                gui_calendar_translate_pen_position                
#define coolsand_UI_callback_highlighter_text_filler               gui_callback_highlighter_text_filler               
#define coolsand_UI_change_multitap_input_state                    gui_change_multitap_input_state                    
#define coolsand_UI_change_scrolling_text                          gui_change_scrolling_text                          
#define coolsand_UI_change_scrolling_text_font                     gui_change_scrolling_text_font                     
#define coolsand_UI_change_WCSS_input_method                       gui_change_WCSS_input_method                       
#define coolsand_UI_cleanup                                        gui_cleanup                                        
#define coolsand_UI_color_slider_create                            gui_color_slider_create                            
#define coolsand_UI_color_slider_hide                              gui_color_slider_hide                              
#define coolsand_UI_color_slider_set_filler                        gui_color_slider_set_filler                        
#define coolsand_UI_color_slider_set_hide_function                 gui_color_slider_set_hide_function                 
#define coolsand_UI_color_slider_set_range                         gui_color_slider_set_range                         
#define coolsand_UI_color_slider_set_value                         gui_color_slider_set_value                         
#define coolsand_UI_color_slider_show                              gui_color_slider_show                              
#define coolsand_UI_create_3D_circular_menu                        gui_create_3D_circular_menu                        
#define coolsand_UI_create_button                                  gui_create_button                                  
#define coolsand_UI_create_calendar                                gui_create_calendar                                
#define coolsand_UI_create_dialer_input_box                        gui_create_dialer_input_box                        
#define coolsand_UI_create_dialer_input_box_set_buffer             gui_create_dialer_input_box_set_buffer             
#define coolsand_UI_create_dialer_input_box_use_buffer             gui_create_dialer_input_box_use_buffer             
#define coolsand_UI_create_drop_down_control                       gui_create_drop_down_control                       
#define coolsand_UI_create_EMS_input_box                           gui_create_EMS_input_box                           
#define coolsand_UI_create_fixed_icontext_list_menuitem            gui_create_fixed_icontext_list_menuitem            
#define coolsand_UI_create_fixed_icontext_menuitem                 gui_create_fixed_icontext_menuitem                 
#define coolsand_UI_create_fixed_list_menu                         gui_create_fixed_list_menu                         
#define coolsand_UI_create_fixed_matrix_menu                       gui_create_fixed_matrix_menu                       
#define coolsand_UI_create_fixed_text_menuitem                     gui_create_fixed_text_menuitem                     
#define coolsand_UI_create_fixed_twostate_menuitem                 gui_create_fixed_twostate_menuitem                 
#define coolsand_UI_create_generic_menuitem                        gui_create_generic_menuitem                        
#define coolsand_UI_create_horizontal_progress_indicator           gui_create_horizontal_progress_indicator           
#define coolsand_UI_create_horizontal_scrollbar                    gui_create_horizontal_scrollbar                    
#define coolsand_UI_create_horizontal_select                       gui_create_horizontal_select                       
#define coolsand_UI_create_horizontal_slide                        gui_create_horizontal_slide                        
#define coolsand_UI_create_horizontal_tab_bar                      gui_create_horizontal_tab_bar                      
#define coolsand_UI_create_icontext_button                         gui_create_icontext_button                         
#define coolsand_UI_create_icontext_menuitem                       gui_create_icontext_menuitem                       
#define coolsand_UI_create_icon_button                             gui_create_icon_button                             
#define coolsand_UI_create_list_menu                               gui_create_list_menu                               
#define coolsand_UI_create_matrix_menu                             gui_create_matrix_menu                             
#define coolsand_UI_create_multitap_input                          gui_create_multitap_input                          
#define coolsand_UI_create_multi_line_input_box                    gui_create_multi_line_input_box                    
#define coolsand_UI_create_multi_line_input_box_set_buffer         gui_create_multi_line_input_box_set_buffer         
#define coolsand_UI_create_multi_line_input_box_set_buffer_no_text gui_create_multi_line_input_box_set_buffer_no_text_reset
#define coolsand_UI_create_multi_line_input_box_use_buffer         gui_create_multi_line_input_box_use_buffer         
#define coolsand_UI_create_pop_up_description                      gui_create_pop_up_description                      
#define coolsand_UI_create_scrolling_text                          gui_create_scrolling_text                          
#define coolsand_UI_create_single_line_input_box                   gui_create_single_line_input_box                   
#define coolsand_UI_create_single_line_input_box_set_buffer        gui_create_single_line_input_box_set_buffer        
#define coolsand_UI_create_single_line_input_box_set_buffer_first_ gui_create_single_line_input_box_set_buffer_first_position
#define coolsand_UI_create_single_line_input_box_use_buffer        gui_create_single_line_input_box_use_buffer        
#define coolsand_UI_create_tab_pane                                gui_create_tab_pane                                
#define coolsand_UI_create_text_button                             gui_create_text_button                             
#define coolsand_UI_create_text_menuitem                           gui_create_text_menuitem                           
#define coolsand_UI_create_vertical_progress_indicator             gui_create_vertical_progress_indicator             
#define coolsand_UI_create_vertical_scrollbar                      gui_create_vertical_scrollbar                      
#define coolsand_UI_create_vertical_slide                          gui_create_vertical_slide                          
#define coolsand_UI_create_virtual_keyboard                        gui_create_virtual_keyboard                        
#define coolsand_UI_custom_fill_area_type1and2                     gui_custom_fill_area_type1and2                     
#define coolsand_UI_custom_fill_area_type2                         gui_custom_fill_area_type2                         
#define coolsand_UI_delete_character_from_buffer                   gui_delete_character_from_buffer                   
#define coolsand_UI_destroy_dialer_input_box                       gui_destroy_dialer_input_box                       
#define coolsand_UI_destroy_multi_line_input_box                   gui_destroy_multi_line_input_box                   
#define coolsand_UI_destroy_single_line_input_box                  gui_destroy_single_line_input_box                  
#define coolsand_UI_dialer_inputbox_locate_cursor                  gui_dialer_inputbox_locate_cursor                  
#define coolsand_UI_dialer_input_box_clear                         gui_dialer_input_box_clear                         
#define coolsand_UI_dialer_input_box_confirm_multitap_character    gui_dialer_input_box_confirm_multitap_character    
#define coolsand_UI_dialer_input_box_delete_all                    gui_dialer_input_box_delete_all                    
#define coolsand_UI_dialer_input_box_delete_character              gui_dialer_input_box_delete_character              
#define coolsand_UI_dialer_input_box_delete_current_character      gui_dialer_input_box_delete_current_character      
#define coolsand_UI_dialer_input_box_get_text_length               gui_dialer_input_box_get_text_length               
#define coolsand_UI_dialer_input_box_goto_first_character          gui_dialer_input_box_goto_first_character          
#define coolsand_UI_dialer_input_box_goto_last_character           gui_dialer_input_box_goto_last_character           
#define coolsand_UI_dialer_input_box_insert_character              gui_dialer_input_box_insert_character              
#define coolsand_UI_dialer_input_box_insert_multitap_character     gui_dialer_input_box_insert_multitap_character     
#define coolsand_UI_dialer_input_box_next                          gui_dialer_input_box_next                          
#define coolsand_UI_dialer_input_box_next_line                     gui_dialer_input_box_next_line                     
#define coolsand_UI_dialer_input_box_previous                      gui_dialer_input_box_previous                      
#define coolsand_UI_dialer_input_box_prev_line                     gui_dialer_input_box_prev_line                     
#define coolsand_UI_dialer_input_box_show_cursor                   gui_dialer_input_box_show_cursor                   
#define coolsand_UI_dialer_input_box_test_first_position           gui_dialer_input_box_test_first_position           
#define coolsand_UI_dialer_input_box_test_last_character_position  gui_dialer_input_box_test_last_character_position  
#define coolsand_UI_dialer_input_box_test_last_position            gui_dialer_input_box_test_last_position            
#define coolsand_UI_dialer_input_box_test_last_position_overflow   gui_dialer_input_box_test_last_position_overflow   
#define coolsand_UI_dialer_input_box_test_overflow                 gui_dialer_input_box_test_overflow                 
#define coolsand_UI_dialer_input_box_toggle_insert_mode            gui_dialer_input_box_toggle_insert_mode            
#define coolsand_UI_dialing_key_select                             gui_dialing_key_select                             
#define coolsand_UI_dialing_key_unselect                           gui_dialing_key_unselect                           
#define coolsand_UI_dialing_screen_translate_pen_event             gui_dialing_screen_translate_pen_event             
#define coolsand_UI_dialing_screen_translate_pen_position          gui_dialing_screen_translate_pen_position          
#define coolsand_UI_disable_fixed_list_line_draw                   gui_disable_fixed_list_line_draw                   
#define coolsand_UI_disable_inline_bottom_line_draw                gui_disable_inline_bottom_line_draw                
#define coolsand_UI_disable_scroller_for_hints                     gui_disable_scroller_for_hints                     
#define coolsand_UI_draw_animated_highlight_filler                 gui_draw_animated_highlight_filler                 
#define coolsand_UI_draw_filled_area                               gui_draw_filled_area                               
#define coolsand_UI_draw_flash_feeling_filler                      gui_draw_flash_feeling_filler                      
#define coolsand_UI_draw_gradient_rounded_rectangle                gui_draw_gradient_rounded_rectangle                
#define coolsand_UI_draw_inscreen_multi_line_input_box_title       gui_draw_inscreen_multi_line_input_box_title       
#define coolsand_UI_draw_list_filled_area                          gui_draw_list_filled_area                          
#define coolsand_UI_draw_list_highlight_effect_last_frame          gui_draw_list_highlight_effect_last_frame          
#define coolsand_UI_draw_rounded_corner                            gui_draw_rounded_corner                            
#define coolsand_UI_draw_transparent_color_filler                  gui_draw_transparent_color_filler                  
#define coolsand_UI_dummy_scroll_handler_for_list_effects          gui_dummy_scroll_handler_for_list_effects          
#define coolsand_UI_dyamic_list_menu_scroll_timer_hdlr             gui_dyamic_list_menu_scroll_timer_hdlr             
#define coolsand_UI_dyamic_list_start_scroll_timer                 gui_dyamic_list_start_scroll_timer                 
#define coolsand_UI_dynamic_list_abort_scroll_timer                gui_dynamic_list_abort_scroll_timer                
#define coolsand_UI_dynamic_list_menu_get_menuitem_position        gui_dynamic_list_menu_get_menuitem_position        
#define coolsand_UI_dynamic_list_menu_goto_item                    gui_dynamic_list_menu_goto_item                    
#define coolsand_UI_dynamic_list_menu_goto_next_item               gui_dynamic_list_menu_goto_next_item               
#define coolsand_UI_dynamic_list_menu_goto_previous_item           gui_dynamic_list_menu_goto_previous_item           
#define coolsand_UI_dynamic_list_menu_locate_highlighted_item      gui_dynamic_list_menu_locate_highlighted_item      
#define coolsand_UI_dynamic_list_menu_locate_next_item             gui_dynamic_list_menu_locate_next_item             
#define coolsand_UI_dynamic_list_menu_locate_previous_item         gui_dynamic_list_menu_locate_previous_item         
#define coolsand_UI_dynamic_list_menu_scroll_by_pen                gui_dynamic_list_menu_scroll_by_pen                
#define coolsand_UI_dynamic_list_menu_translate_pen_event          gui_dynamic_list_menu_translate_pen_event          
#define coolsand_UI_dynamic_list_menu_translate_pen_position       gui_dynamic_list_menu_translate_pen_position       
#define coolsand_UI_EMS_breakable_character                        gui_EMS_breakable_character                        
#define coolsand_UI_EMS_cancel_current_position                    gui_EMS_cancel_current_position                    
#define coolsand_UI_EMS_cancel_current_position                    gui_EMS_cancel_current_position                    
#define coolsand_UI_EMS_change_DCS                                 gui_EMS_change_DCS                                 
#define coolsand_UI_EMS_check_input_test_overflow                  gui_EMS_check_input_test_overflow                  
#define coolsand_UI_EMS_current_character                          gui_EMS_current_character                          
#define coolsand_UI_ems_data_apply_shaping                         gui_ems_data_apply_shaping                         
#define coolsand_UI_EMS_inputbox_add_object                        gui_EMS_inputbox_add_object                        
#define coolsand_UI_EMS_inputbox_empty                             gui_EMS_inputbox_empty                             
#define coolsand_UI_EMS_input_box_abort_scroll_timer               gui_EMS_input_box_abort_scroll_timer               
#define coolsand_UI_EMS_input_box_add_string                       gui_EMS_input_box_add_string                       
#define coolsand_UI_EMS_input_box_add_string                       gui_EMS_input_box_add_string                       
#define coolsand_UI_EMS_input_box_backspace                        gui_EMS_input_box_backspace                        
#define coolsand_UI_EMS_input_box_back_cursor_for_ESC_character    gui_EMS_input_box_back_cursor_for_ESC_character    
#define coolsand_UI_EMS_input_box_confirm_multitap_character       gui_EMS_input_box_confirm_multitap_character       
#define coolsand_UI_EMS_input_box_delete                           gui_EMS_input_box_delete                           
#define coolsand_UI_EMS_input_box_delete                           gui_EMS_input_box_delete                           
#define coolsand_UI_EMS_input_box_delete_all                       gui_EMS_input_box_delete_all                       
#define coolsand_UI_EMS_input_box_display_object                   gui_EMS_input_box_display_object                   
#define coolsand_UI_EMS_input_box_display_object_pic               gui_EMS_input_box_display_object_pic               
#define coolsand_UI_EMS_input_box_equal_text_format                gui_EMS_input_box_equal_text_format                
#define coolsand_UI_EMS_input_box_find_paragraph_range             gui_EMS_input_box_find_paragraph_range             
#define coolsand_UI_EMS_input_box_forward_cursor                   gui_EMS_input_box_forward_cursor                   
#define coolsand_UI_EMS_input_box_forward_cursor                   gui_EMS_input_box_forward_cursor                   
#define coolsand_UI_EMS_input_box_get_next_object                  gui_EMS_input_box_get_next_object                  
#define coolsand_UI_EMS_input_box_get_next_object_for_input_method gui_EMS_input_box_get_next_object_for_input_method 
#define coolsand_UI_EMS_input_box_get_previous_object              gui_EMS_input_box_get_previous_object              
#define coolsand_UI_EMS_input_box_get_previous_text_alignment      gui_EMS_input_box_get_previous_text_alignment      
#define coolsand_UI_EMS_input_box_highlight_cursor_end             gui_EMS_input_box_highlight_cursor_end             
#define coolsand_UI_EMS_input_box_highlight_cursor_number          gui_EMS_input_box_highlight_cursor_number          
#define coolsand_UI_EMS_input_box_highlight_cursor_start           gui_EMS_input_box_highlight_cursor_start           
#define coolsand_UI_EMS_input_box_insert_character                 gui_EMS_input_box_insert_character                 
#define coolsand_UI_EMS_input_box_insert_extension_character       gui_EMS_input_box_insert_extension_character       
#define coolsand_UI_EMS_input_box_insert_multitap_character        gui_EMS_input_box_insert_multitap_character        
#define coolsand_UI_EMS_input_box_insert_new_line                  gui_EMS_input_box_insert_new_line                  
#define coolsand_UI_EMS_input_box_insert_new_line                  gui_EMS_input_box_insert_new_line                  
#define coolsand_UI_EMS_input_box_insert_new_paragraph             gui_EMS_input_box_insert_new_paragraph             
#define coolsand_UI_EMS_input_box_insert_text_alignment            gui_EMS_input_box_insert_text_alignment            
#define coolsand_UI_EMS_input_box_locate_cursor                    gui_EMS_input_box_locate_cursor                    
#define coolsand_UI_EMS_input_box_measure_object                   gui_EMS_input_box_measure_object                   
#define coolsand_UI_EMS_input_box_next                             gui_EMS_input_box_next                             
#define coolsand_UI_EMS_input_box_next                             gui_EMS_input_box_next                             
#define coolsand_UI_EMS_input_box_next_line                        gui_EMS_input_box_next_line                        
#define coolsand_UI_EMS_input_box_previous                         gui_EMS_input_box_previous                         
#define coolsand_UI_EMS_input_box_previous                         gui_EMS_input_box_previous                         
#define coolsand_UI_EMS_input_box_previous_line                    gui_EMS_input_box_previous_line                    
#define coolsand_UI_EMS_input_box_remove_extension_character       gui_EMS_input_box_remove_extension_character       
#define coolsand_UI_call_shuffle                                   gui_call_shuffle                                   

#define coolsand_UI_alternate_cross_hatch_fill_rectangle                      gui_alternate_cross_hatch_fill_rectangle                              
#define coolsand_UI_alternate_hatch_fill_rectangle                            gui_alternate_hatch_fill_rectangle                                    
#define coolsand_UI_asyncdynamic_list_error                                   gui_asyncdynamic_list_error                                           
#define coolsand_UI_atoi                                                      gui_atoi                                                              
#define coolsand_UI_BLT_double_buffer                                         gui_BLT_double_buffer                                                 
#define coolsand_UI_bopomo_custom_key_layout                                  gui_bopomo_custom_key_layout                                          
#define coolsand_UI_bopomo_custom_key_layout                                  gui_bopomo_custom_key_layout                                          
#define coolsand_UI_bopomo_custom_key_layout                                  gui_bopomo_custom_key_layout                                          
#define coolsand_UI_bopomo_custom_key_layout                                  gui_bopomo_custom_key_layout                                          
#define coolsand_UI_bopomo_matrix_layout                                      gui_bopomo_matrix_layout                                              
#define coolsand_UI_bopomo_matrix_layout                                      gui_bopomo_matrix_layout                                              
#define coolsand_UI_call_menuitem_scroll                                      gui_call_menuitem_scroll                                              
#define coolsand_UI_cancel_timer                                              gui_cancel_timer                                                      
#define coolsand_UI_color                                                     gui_color                                                             
#define coolsand_UI_color32                                                   gui_color32                                                           
#define coolsand_UI_color_RGB                                                 gui_color_RGB                                                         
#define coolsand_UI_cross_hatch_fill_rectangle                                gui_cross_hatch_fill_rectangle                                        
#define coolsand_UI_dead_key_symbol_table                                     gui_dead_key_symbol_table                                             
#define coolsand_UI_dead_key_symbol_table                                     gui_dead_key_symbol_table                                             
#define coolsand_UI_display_pop_up_description                                gui_display_pop_up_description                                        
#define coolsand_UI_draw_horizontal_line                                      gui_draw_horizontal_line                                              
#define coolsand_UI_draw_hori_line_type                                       gui_draw_hori_line_type                                               
#define coolsand_UI_draw_rectangle                                            gui_draw_rectangle                                                    
#define coolsand_UI_draw_rectangle_type                                       gui_draw_rectangle_type                                               
#define coolsand_UI_draw_vertical_line                                        gui_draw_vertical_line                                                
#define coolsand_UI_draw_vert_line_type                                       gui_draw_vert_line_type                                               
#define coolsand_UI_european_symbols                                          gui_european_symbols                                                  
#define coolsand_UI_european_symbols                                          gui_european_symbols                                                  
#define coolsand_UI_extended_custom_key_layout                                gui_extended_custom_key_layout                                        
#define coolsand_UI_extended_custom_key_layout                                gui_extended_custom_key_layout                                        
#define coolsand_UI_extended_custom_key_layout                                gui_extended_custom_key_layout                                        
#define coolsand_UI_extended_custom_key_layout                                gui_extended_custom_key_layout                                        
#define coolsand_UI_extended_custom_key_layout                                gui_extended_custom_key_layout                                        
#define coolsand_UI_extended_matrix_layout                                    gui_extended_matrix_layout                                            
#define coolsand_UI_extended_matrix_layout                                    gui_extended_matrix_layout                                            
#define coolsand_UI_fill_rectangle                                            gui_fill_rectangle                                                    
#define coolsand_UI_free                                                      gui_free                                                              
#define coolsand_UI_get_character_height                                      gui_get_character_height                                              
#define coolsand_UI_get_character_width                                       gui_get_character_width                                               
#define coolsand_UI_get_clip                                                  gui_get_clip                                                          
#define coolsand_UI_get_current_pop_up_description_string                     gui_get_current_pop_up_description_string                             
#define coolsand_UI_get_next_character                                        gui_get_next_character                                                
#define coolsand_UI_get_previous_character                                    gui_get_previous_character                                            
#define coolsand_UI_get_string_height                                         gui_get_string_height                                                 
#define coolsand_UI_get_string_width                                          gui_get_string_width                                                  
#define coolsand_UI_get_string_width_n                                        gui_get_string_width_n                                                
#define coolsand_UI_get_string_width_w                                        gui_get_string_width_w                                                
#define coolsand_UI_get_string_width_wn                                       gui_get_string_width_wn                                               
#define coolsand_UI_get_text_clip                                             gui_get_text_clip                                                     
#define coolsand_UI_get_two_line_menuitem_height                              gui_get_two_line_menuitem_height                                      
#define coolsand_UI_hatch_fill_rectangle                                      gui_hatch_fill_rectangle                                              
#define coolsand_UI_hide_animated_image                                       gui_hide_animated_image                                               
#define coolsand_UI_hide_animations                                           gui_hide_animations                                                   
#define coolsand_UI_hilite_underline_color                                    gui_hilite_underline_color                                            
#define coolsand_UI_image_clip_x1                                             gui_image_clip_x1                                                     
#define coolsand_UI_image_clip_x2                                             gui_image_clip_x2                                                     
#define coolsand_UI_image_clip_y1                                             gui_image_clip_y1                                                     
#define coolsand_UI_image_clip_y2                                             gui_image_clip_y2                                                     
#define coolsand_UI_image_frame_count                                         gui_image_frame_count                                                 
#define coolsand_UI_image_frame_idx                                           gui_image_frame_idx                                                   
#define coolsand_UI_image_id                                                  gui_image_id                                                          
#define coolsand_UI_image_layer                                               gui_image_layer                                                       
#define coolsand_UI_image_n_frames                                            gui_image_n_frames                                                    
#define coolsand_UI_image_offset_x                                            gui_image_offset_x                                                    
#define coolsand_UI_image_offset_y                                            gui_image_offset_y                                                    
#define coolsand_UI_itoa                                                      gui_itoa                                                              
#define coolsand_UI_keyboard_bopomo_string                                    gui_keyboard_bopomo_string                                            
#define coolsand_UI_keyboard_bopomo_string                                    gui_keyboard_bopomo_string                                            
#define coolsand_UI_keyboard_chinese_symbol_string                            gui_keyboard_chinese_symbol_string                                    
#define coolsand_UI_keyboard_chinese_symbol_string                            gui_keyboard_chinese_symbol_string                                    
#define coolsand_UI_keyboard_empty_tray_string                                gui_keyboard_empty_tray_string                                        
#define coolsand_UI_keyboard_empty_tray_string                                gui_keyboard_empty_tray_string                                        
#define coolsand_UI_keyboard_english_lowercase_string                         gui_keyboard_english_lowercase_string                                 
#define coolsand_UI_keyboard_english_lowercase_string                         gui_keyboard_english_lowercase_string                                 
#define coolsand_UI_keyboard_english_uppercase_string                         gui_keyboard_english_uppercase_string                                 
#define coolsand_UI_keyboard_english_uppercase_string                         gui_keyboard_english_uppercase_string                                 
#define coolsand_UI_keyboard_european_symbols_string                          gui_keyboard_european_symbols_string                                  
#define coolsand_UI_keyboard_european_symbols_string                          gui_keyboard_european_symbols_string                                  
#define coolsand_UI_keyboard_french_lowercase_string                          gui_keyboard_french_lowercase_string                                  
#define coolsand_UI_keyboard_french_lowercase_string                          gui_keyboard_french_lowercase_string                                  
#define coolsand_UI_keyboard_french_uppercase_string                          gui_keyboard_french_uppercase_string                                  
#define coolsand_UI_keyboard_french_uppercase_string                          gui_keyboard_french_uppercase_string                                  
#define coolsand_UI_keyboard_german_lowercase_string                          gui_keyboard_german_lowercase_string                                  
#define coolsand_UI_keyboard_german_lowercase_string                          gui_keyboard_german_lowercase_string                                  
#define coolsand_UI_keyboard_german_uppercase_string                          gui_keyboard_german_uppercase_string                                  
#define coolsand_UI_keyboard_german_uppercase_string                          gui_keyboard_german_uppercase_string                                  
#define coolsand_UI_keyboard_italian_lowercase_string                         gui_keyboard_italian_lowercase_string                                 
#define coolsand_UI_keyboard_italian_lowercase_string                         gui_keyboard_italian_lowercase_string                                 
#define coolsand_UI_keyboard_italian_uppercase_string                         gui_keyboard_italian_uppercase_string                                 
#define coolsand_UI_keyboard_italian_uppercase_string                         gui_keyboard_italian_uppercase_string                                 
#define coolsand_UI_keyboard_number_dot_tray_string                           gui_keyboard_number_dot_tray_string                                   
#define coolsand_UI_keyboard_number_dot_tray_string                           gui_keyboard_number_dot_tray_string                                   
#define coolsand_UI_keyboard_number_string                                    gui_keyboard_number_string                                            
#define coolsand_UI_keyboard_number_string                                    gui_keyboard_number_string                                            
#define coolsand_UI_keyboard_number_tray_string                               gui_keyboard_number_tray_string                                       
#define coolsand_UI_keyboard_number_tray_string                               gui_keyboard_number_tray_string                                       
#define coolsand_UI_keyboard_phone_number_string                              gui_keyboard_phone_number_string                                      
#define coolsand_UI_keyboard_phone_number_string                              gui_keyboard_phone_number_string                                      
#define coolsand_UI_keyboard_pinyin_string                                    gui_keyboard_pinyin_string                                            
#define coolsand_UI_keyboard_pinyin_string                                    gui_keyboard_pinyin_string                                            
#define coolsand_UI_keyboard_portuguese_lowercase_string                      gui_keyboard_portuguese_lowercase_string                              
#define coolsand_UI_keyboard_portuguese_lowercase_string                      gui_keyboard_portuguese_lowercase_string                              
#define coolsand_UI_keyboard_portuguese_uppercase_string                      gui_keyboard_portuguese_uppercase_string                              
#define coolsand_UI_keyboard_portuguese_uppercase_string                      gui_keyboard_portuguese_uppercase_string                              
#define coolsand_UI_keyboard_russian_lowercase_string                         gui_keyboard_russian_lowercase_string                                 
#define coolsand_UI_keyboard_russian_lowercase_string                         gui_keyboard_russian_lowercase_string                                 
#define coolsand_UI_keyboard_russian_uppercase_string                         gui_keyboard_russian_uppercase_string                                 
#define coolsand_UI_keyboard_russian_uppercase_string                         gui_keyboard_russian_uppercase_string                                 
#define coolsand_UI_keyboard_spanish_lowercase_string                         gui_keyboard_spanish_lowercase_string                                 
#define coolsand_UI_keyboard_spanish_lowercase_string                         gui_keyboard_spanish_lowercase_string                                 
#define coolsand_UI_keyboard_spanish_uppercase_string                         gui_keyboard_spanish_uppercase_string                                 
#define coolsand_UI_keyboard_spanish_uppercase_string                         gui_keyboard_spanish_uppercase_string                                 
#define coolsand_UI_keyboard_symbol_string                                    gui_keyboard_symbol_string                                            
#define coolsand_UI_keyboard_tray_string                                      gui_keyboard_tray_string                                              
#define coolsand_UI_keyboard_tray_string                                      gui_keyboard_tray_string                                              
#define coolsand_UI_keyboard_turkish_lowercase_string                         gui_keyboard_turkish_lowercase_string                                 
#define coolsand_UI_keyboard_turkish_lowercase_string                         gui_keyboard_turkish_lowercase_string                                 
#define coolsand_UI_keyboard_turkish_uppercase_string                         gui_keyboard_turkish_uppercase_string                                 
#define coolsand_UI_keyboard_turkish_uppercase_string                         gui_keyboard_turkish_uppercase_string                                 
#define coolsand_UI_line                                                      gui_line                                                              
#define coolsand_UI_lock_double_buffer                                        gui_lock_double_buffer                                                
#define coolsand_UI_measure_character                                         gui_measure_character                                                 
#define coolsand_UI_measure_image                                             gui_measure_image                                                     
#define coolsand_UI_measure_string                                            gui_measure_string                                                    
#define coolsand_UI_measure_string_n                                          gui_measure_string_n                                                  
#define coolsand_UI_measure_string_w                                          gui_measure_string_w                                                  
#define coolsand_UI_measure_string_wn                                         gui_measure_string_wn                                                 
#define coolsand_UI_move_text_cursor                                          gui_move_text_cursor                                                  
#define coolsand_UI_move_two_line_menuitem_part                               gui_move_two_line_menuitem_part                                       
#define coolsand_UI_number_custom_key_layout                                  gui_number_custom_key_layout                                          
#define coolsand_UI_number_custom_key_layout                                  gui_number_custom_key_layout                                          
#define coolsand_UI_number_custom_key_layout                                  gui_number_custom_key_layout                                          
#define coolsand_UI_number_custom_key_layout                                  gui_number_custom_key_layout                                          
#define coolsand_UI_number_custom_key_layout                                  gui_number_custom_key_layout                                          
#define coolsand_UI_number_matrix_layout                                      gui_number_matrix_layout                                              
#define coolsand_UI_number_matrix_layout                                      gui_number_matrix_layout                                              
#define coolsand_UI_page_info                                                 gui_page_info                                                         
#define coolsand_UI_pen_scroll_asyncdynamic_menu                              gui_pen_scroll_asyncdynamic_menu                                      
#define coolsand_UI_pen_scroll_dynamic_menu                                   gui_pen_scroll_dynamic_menu                                           
#define coolsand_UI_pen_scroll_EMS_input_box                                  gui_pen_scroll_EMS_input_box                                          
#define coolsand_UI_pen_scroll_multi_line_input_box                           gui_pen_scroll_multi_line_input_box                                   
#define coolsand_UI_pinyin_custom_key_layout                                  gui_pinyin_custom_key_layout                                          
#define coolsand_UI_pinyin_custom_key_layout                                  gui_pinyin_custom_key_layout                                          
#define coolsand_UI_pinyin_custom_key_layout                                  gui_pinyin_custom_key_layout                                          
#define coolsand_UI_pinyin_custom_key_layout                                  gui_pinyin_custom_key_layout                                          
#define coolsand_UI_pinyin_custom_key_layout                                  gui_pinyin_custom_key_layout                                          
#define coolsand_UI_pinyin_matrix_layout                                      gui_pinyin_matrix_layout                                              
#define coolsand_UI_pop_clip                                                  gui_pop_clip                                                          
#define coolsand_UI_pop_text_clip                                             gui_pop_text_clip                                                     
#define coolsand_UI_printf                                                    gui_printf                                                            
#define coolsand_UI_print_bordered_character                                  gui_print_bordered_character                                          
#define coolsand_UI_print_bordered_stacked_character                          gui_print_bordered_stacked_character                                  
#define coolsand_UI_print_bordered_text                                       gui_print_bordered_text                                               
#define coolsand_UI_print_bordered_text_n                                     gui_print_bordered_text_n                                             
#define coolsand_UI_print_character                                           gui_print_character                                                   
#define coolsand_UI_pinyin_matrix_layout                                      gui_pinyin_matrix_layout                                              

#define coolsand_UI_set_single_line_input_box_current_theme                           gui_set_single_line_input_box_current_theme                     
#define coolsand_UI_set_single_line_input_box_theme                                   gui_set_single_line_input_box_theme                             
#define coolsand_UI_create_single_line_input_box                                      gui_create_single_line_input_box                                
#define coolsand_UI_create_single_line_input_box_use_buffer                           gui_create_single_line_input_box_use_buffer                     
#define coolsand_UI_create_single_line_input_box_set_buffer                           gui_create_single_line_input_box_set_buffer                     
#define coolsand_UI_single_line_input_box_show_cursor                                 gui_single_line_input_box_show_cursor                           
#define coolsand_UI_show_single_line_input_box                                        gui_show_single_line_input_box                                  
#define coolsand_UI_destroy_single_line_input_box                                     gui_destroy_single_line_input_box                               
#define coolsand_UI_single_line_input_box_delete_character                            gui_single_line_input_box_delete_character                      
#define coolsand_UI_single_line_input_box_insert_character                            gui_single_line_input_box_insert_character                      
#define coolsand_UI_single_line_input_box_insert_multitap_character                   gui_single_line_input_box_insert_multitap_character             
#define coolsand_UI_single_line_input_box_confirm_multitap_character                  gui_single_line_input_box_confirm_multitap_character            
#define coolsand_UI_single_line_input_box_get_previous_character                      gui_single_line_input_box_get_previous_character                
#define coolsand_UI_single_line_input_box_previous                                    gui_single_line_input_box_previous                              
#define coolsand_UI_single_line_input_box_next                                        gui_single_line_input_box_next                                  
#define coolsand_UI_resize_single_line_input_box                                      gui_resize_single_line_input_box                                
#define coolsand_UI_move_single_line_input_box                                        gui_move_single_line_input_box                                  
#define coolsand_UI_single_line_input_box_delete_all                                  gui_single_line_input_box_delete_all                            
#define coolsand_UI_single_line_input_box_delete_current_character                    gui_single_line_input_box_delete_current_character              
#define coolsand_UI_single_line_input_box_toggle_insert_mode                          gui_single_line_input_box_toggle_insert_mode                    
#define coolsand_UI_single_line_input_box_goto_first_character                        gui_single_line_input_box_goto_first_character                  
#define coolsand_UI_single_line_input_box_goto_last_character                         gui_single_line_input_box_goto_last_character                   
#define coolsand_UI_single_line_input_box_clear                                       gui_single_line_input_box_clear                                 
#define coolsand_UI_single_line_input_box_test_overflow                               gui_single_line_input_box_test_overflow                         
#define coolsand_UI_single_line_input_box_test_first_position                         gui_single_line_input_box_test_first_position                   
#define coolsand_UI_single_line_input_box_test_last_position                          gui_single_line_input_box_test_last_position                    
#define coolsand_UI_single_line_input_box_test_last_position_overflow                 gui_single_line_input_box_test_last_position_overflow           
#define coolsand_UI_single_line_input_box_get_text_length                             gui_single_line_input_box_get_text_length                       
#define coolsand_UI_single_line_input_box_test_last_character_position                gui_single_line_input_box_test_last_character_position          
#define coolsand_UI_create_single_line_input_box_set_buffer_first_position            gui_create_single_line_input_box_set_buffer_first_position      
#define coolsand_UI_single_line_input_box_highlight_cursor_end                        gui_single_line_input_box_highlight_cursor_end                  
#define coolsand_UI_single_line_input_box_highlight_cursor_number                     gui_single_line_input_box_highlight_cursor_number               
#define coolsand_UI_single_line_input_box_highlight_cursor_start                      gui_single_line_input_box_highlight_cursor_start                
#define coolsand_UI_single_line_input_box_insert_string                               gui_single_line_input_box_insert_string                         
#define coolsand_UI_single_line_input_box_next_steps                                  gui_single_line_input_box_next_steps                            
#define coolsand_UI_single_line_input_box_previous_steps                              gui_single_line_input_box_previous_steps                        
#define coolsand_UI_single_line_input_box_translate_pen_event                         gui_single_line_input_box_translate_pen_event                   
#define coolsand_UI_show_single_line_input_box_ext                                    gui_show_single_line_input_box_ext                              
#define  coolsand_UI_virtual_keyboard_translate_pen_event                          gui_virtual_keyboard_translate_pen_event                 
#define  coolsand_UI_unlock_double_buffer                                          gui_unlock_double_buffer                                 
#define  coolsand_UI_strlen                                                        gui_strlen                                               
#define  coolsand_UI_strcpy                                                        gui_strcpy                                               
#define  coolsand_UI_strcat                                                        gui_strcat                                               
#define  coolsand_UI_start_timer                                                   gui_start_timer                                          
#define  coolsand_UI_sprintf                                                       gui_sprintf                                              
#define  coolsand_UI_single_line_input_box_locate_cursor                           gui_single_line_input_box_locate_cursor                  
#define  coolsand_UI_single_line_input_box_get_remaining_length                    gui_single_line_input_box_get_remaining_length           
#define  coolsand_UI_show_virtual_keyboard                                         gui_show_virtual_keyboard                                
#define  coolsand_UI_show_vertical_scrollbar                                       gui_show_vertical_scrollbar                              
#define  coolsand_UI_show_transparent_image                                        gui_show_transparent_image                               
#define  coolsand_UI_show_scrolling_text                                           gui_show_scrolling_text                                  
#define  coolsand_UI_show_multi_line_input_box_ext                                 gui_show_multi_line_input_box_ext                        
#define  coolsand_UI_show_multi_line_input_box                                     gui_show_multi_line_input_box                            
#define  coolsand_UI_show_image                                                    gui_show_image                                           
#define  coolsand_UI_show_EMS_input_box_ext                                        gui_show_EMS_input_box_ext                               
#define  coolsand_UI_show_EMS_input_box                                            gui_show_EMS_input_box                                   
#define  coolsand_UI_set_vertical_scrollbar_value                                  gui_set_vertical_scrollbar_value                         
#define  coolsand_UI_set_vertical_scrollbar_scale                                  gui_set_vertical_scrollbar_scale                         
#define  coolsand_UI_set_vertical_scrollbar_range                                  gui_set_vertical_scrollbar_range                         
#define  coolsand_UI_set_text_color                                                gui_set_text_color                                       
#define  coolsand_UI_set_text_clip                                                 gui_set_text_clip                                        
#define  coolsand_UI_set_text_border_color                                         gui_set_text_border_color                                
#define  coolsand_UI_set_multitap_input_callbacks                                  gui_set_multitap_input_callbacks                         
#define  coolsand_UI_set_line_height                                               gui_set_line_height                                      
#define  coolsand_UI_set_font                                                      gui_set_font                                             
#define  coolsand_UI_set_entry_new_screen                                          gui_set_entry_new_screen                                 
#define  coolsand_UI_set_EMS_input_box_data                                        gui_set_EMS_input_box_data                               
#define  coolsand_UI_set_EMS_input_box_current_theme                               gui_set_EMS_input_box_current_theme                      
#define  coolsand_UI_set_clip                                                      gui_set_clip                                             
#define  coolsand_UI_scrolling_text_stop                                           gui_scrolling_text_stop                                  
#define  coolsand_UI_resize_multi_line_input_box                                   gui_resize_multi_line_input_box                          
#define  coolsand_UI_resize_EMS_input_box                                          gui_resize_EMS_input_box                                 
#define  coolsand_UI_reset_text_clip                                               gui_reset_text_clip                                      
#define  coolsand_UI_reset_clip                                                    gui_reset_clip                                           
#define  coolsand_UI_push_text_clip                                                gui_push_text_clip                                       
#define  coolsand_UI_push_clip                                                     gui_push_clip                                            
#define  coolsand_UI_print_truncated_text                                          gui_print_truncated_text                                 
#define  coolsand_UI_print_truncated_borderd_text                                  gui_print_truncated_borderd_text                         
#define  coolsand_UI_print_text                                                    gui_print_text                                           
#define  coolsand_UI_multitap_input_translate_pen_position                         gui_multitap_input_translate_pen_position                
#define  coolsand_UI_multitap_input_complete                                       gui_multitap_input_complete                              
#define  coolsand_UI_multi_line_input_box_translate_pen_event                      gui_multi_line_input_box_translate_pen_event             
#define  coolsand_UI_multi_line_input_box_set_pen_scroll_delay                     gui_multi_line_input_box_set_pen_scroll_delay            
#define  coolsand_UI_multi_line_input_box_previous                                 gui_multi_line_input_box_previous                        
#define  coolsand_UI_multi_line_input_box_next                                     gui_multi_line_input_box_next                            
#define  coolsand_UI_multi_line_input_box_insert_newline                           gui_multi_line_input_box_insert_newline                  
#define  coolsand_UI_multi_line_input_box_insert_character                         gui_multi_line_input_box_insert_character                
#define  coolsand_UI_multi_line_input_box_get_text_length                          gui_multi_line_input_box_get_text_length                 
#define  coolsand_UI_multi_line_input_box_get_remaining_length                     gui_multi_line_input_box_get_remaining_length            
#define  coolsand_UI_multi_line_input_box_delete_character                         gui_multi_line_input_box_delete_character                
#define  coolsand_UI_move_multi_line_input_box                                     gui_move_multi_line_input_box                            
#define  coolsand_UI_move_EMS_input_box                                            gui_move_EMS_input_box                                   
#define  coolsand_UI_malloc                                                        gui_malloc                                               
#define  coolsand_UI_layer_transparent                                             gui_layer_transparent                                    
#define  coolsand_UI_is_entry_new_screen                                           gui_is_entry_new_screen                                  
#define  coolsand_UI_handle_scrolling_text                                         gui_handle_scrolling_text                                
#define  coolsand_UI_fixed_list_menu_locate_highlighted_item                       gui_fixed_list_menu_locate_highlighted_item              
#define  coolsand_UI_fixed_list_menu_goto_item                                     gui_fixed_list_menu_goto_item                            
#define  coolsand_UI_EMS_input_box_translate_pen_event                             gui_EMS_input_box_translate_pen_event                    
#define  coolsand_UI_EMS_input_box_toggle_insert_mode                              gui_EMS_input_box_toggle_insert_mode                     
#define  coolsand_UI_EMS_input_box_set_text_format_cursor_position                 gui_EMS_input_box_set_text_format_cursor_position        
#define  coolsand_UI_EMS_input_box_set_pen_scroll_delay                            gui_EMS_input_box_set_pen_scroll_delay                                                                
#define coolsand_UI_color                                             gui_color

#define  mmi_wgui_test_object_position                                           wgui_test_object_position                                   
#define  mmi_wgui_reset_status_icon_pen_event_hdlr                               wgui_reset_status_icon_pen_event_hdlr                       
#define  mmi_wgui_reset_list_item_selected_callback                              wgui_reset_list_item_selected_callback                      
#define  mmi_wgui_register_status_icon_pen_event_hdlr                            wgui_register_status_icon_pen_event_hdlr                    
#define  mmi_wgui_register_pen_up_handler                                        wgui_register_pen_up_handler                                
#define  mmi_wgui_register_pen_repeat_handler                                    wgui_register_pen_repeat_handler                            
#define  mmi_wgui_register_pen_move_handler                                      wgui_register_pen_move_handler                              
#define  mmi_wgui_register_pen_long_tap_handler                                  wgui_register_pen_long_tap_handler                          
#define  mmi_wgui_register_pen_down_handler                                      wgui_register_pen_down_handler                              
#define  mmi_wgui_register_pen_abort_handler                                     wgui_register_pen_abort_handler                             
#define  mmi_wgui_register_list_item_selected_callback_all                       wgui_register_list_item_selected_callback_all               
#define  mmi_wgui_register_list_item_selected_callback                           wgui_register_list_item_selected_callback                   
#define  mmi_wgui_general_pen_up_hdlr                                            wgui_general_pen_up_hdlr                                    
#define  mmi_wgui_general_pen_repeat_hdlr                                        wgui_general_pen_repeat_hdlr                                
#define  mmi_wgui_general_pen_move_hdlr                                          wgui_general_pen_move_hdlr                                  
#define  mmi_wgui_general_pen_long_tap_hdlr                                      wgui_general_pen_long_tap_hdlr                              
#define  mmi_wgui_general_pen_down_hdlr                                          wgui_general_pen_down_hdlr                                  
#define  mmi_wgui_general_pen_abort_hdlr                                         wgui_general_pen_abort_hdlr   
#define  mmi_wgui_test_rsk_position                                              wgui_test_rsk_position
#define  mmi_wgui_test_lsk_position                                              wgui_test_lsk_position
#define mmi_wgui_component_info_struct                           wgui_component_info_struct
#define mmi_gui_pen_event_param_struct                           gui_pen_event_param_struct
#define mmi_gui_virtual_keyboard_language_enum                   gui_virtual_keyboard_language_enum
#define MMI_GUI_VIRTUAL_KEYBOARD_MAX_LANG                        GUI_VIRTUAL_KEYBOARD_MAX_LANG
#define MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY                      GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY
#define mmi_gui_button_pen_enum                                  gui_button_pen_enum
#define mmi_gui_virtual_keyboard_pen_enum                        gui_virtual_keyboard_pen_enum
#define MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH                         GUI_VIRTUAL_KEYBOARD_ENGLISH
#define MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE                GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL                           GUI_VIRTUAL_KEYBOARD_SYMBOL
#define MMI_GUI_VIRTUAL_KEYBOARD_TRAY                             GUI_VIRTUAL_KEYBOARD_TRAY
#define MMI_GUI_VIRTUAL_KEYBOARD_NUMBER                           GUI_VIRTUAL_KEYBOARD_NUMBER
#define MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER                     GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER
#define MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY                       GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY
#define MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY                      GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY
#define MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY                  GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY
#define MMI_GUI_VIRTUAL_KEYBOARD_PINYIN                           GUI_VIRTUAL_KEYBOARD_PINYIN
#define MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO                           GUI_VIRTUAL_KEYBOARD_BOPOMO
#define MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL                     GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL
#define MMI_GUI_VIRTUAL_KEYBOARD_SPANISH                          GUI_VIRTUAL_KEYBOARD_SPANISH
#define MMI_GUI_VIRTUAL_KEYBOARD_SPANISH_UPPERCASE                  GUI_VIRTUAL_KEYBOARD_SPANISH_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_FRENCH                             GUI_VIRTUAL_KEYBOARD_FRENCH
#define MMI_GUI_VIRTUAL_KEYBOARD_FRENCH_UPPERCASE                   GUI_VIRTUAL_KEYBOARD_FRENCH_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_GERMAN                           GUI_VIRTUAL_KEYBOARD_GERMAN
#define MMI_GUI_VIRTUAL_KEYBOARD_GERMAN_UPPERCASE                 GUI_VIRTUAL_KEYBOARD_GERMAN_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_ITALIAN                          GUI_VIRTUAL_KEYBOARD_ITALIAN
#define MMI_GUI_VIRTUAL_KEYBOARD_ITALIAN_UPPERCASE                GUI_VIRTUAL_KEYBOARD_ITALIAN_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_RUSSIAN                            GUI_VIRTUAL_KEYBOARD_RUSSIAN
#define MMI_GUI_VIRTUAL_KEYBOARD_RUSSIAN_UPPERCASE                  GUI_VIRTUAL_KEYBOARD_RUSSIAN_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_TURKISH                          GUI_VIRTUAL_KEYBOARD_TURKISH
#define MMI_GUI_VIRTUAL_KEYBOARD_TURKISH_UPPERCASE                GUI_VIRTUAL_KEYBOARD_TURKISH_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_PORTUGUESE                       GUI_VIRTUAL_KEYBOARD_PORTUGUESE
#define MMI_GUI_VIRTUAL_KEYBOARD_PORTUGUESE_UPPERCASE               GUI_VIRTUAL_KEYBOARD_PORTUGUESE_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_EUROPEAN_SYMBOLS                 GUI_VIRTUAL_KEYBOARD_EUROPEAN_SYMBOLS
#define MMI_GUI_VIRTUAL_KEYBOARD_ARABIC 					GUI_VIRTUAL_KEYBOARD_ARABIC
#define MMI_GUI_VIRTUAL_KEYBOARD_ARABIC_SYMBOLS 		GUI_VIRTUAL_KEYBOARD_ARABIC_SYMBOLS 
#define MMI_GUI_VIRTUAL_KEYBOARD_ARABIC_NUMERIC 		GUI_VIRTUAL_KEYBOARD_ARABIC_NUMERIC
#define MMI_GUI_VIRTUAL_KEYBOARD_VIETNAMESE					GUI_VIRTUAL_KEYBOARD_VIETNAMESE
#define MMI_GUI_VIRTUAL_KEYBOARD_VIETNAMESE_UPPERCASE		GUI_VIRTUAL_KEYBOARD_VIETNAMESE_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_THAI					GUI_VIRTUAL_KEYBOARD_THAI
#define MMI_GUI_VIRTUAL_KEYBOARD_THAI_SYMBOL			GUI_VIRTUAL_KEYBOARD_THAI_SYMBOL
#define MMI_GUI_VIRTUAL_KEYBOARD_MALAY					GUI_VIRTUAL_KEYBOARD_MALAY
#define MMI_GUI_VIRTUAL_KEYBOARD_MALAY_UPPERCASE		GUI_VIRTUAL_KEYBOARD_MALAY_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_PERSIAN				GUI_VIRTUAL_KEYBOARD_FARSI
#define MMI_GUI_VIRTUAL_KEYBOARD_PERSIAN_SYMBOL		GUI_VIRTUAL_KEYBOARD_FARSI_SYMBOL
#define MMI_GUI_VIRTUAL_KEYBOARD_INDONESIAN					GUI_VIRTUAL_KEYBOARD_INDONESIAN
#define MMI_GUI_VIRTUAL_KEYBOARD_INDONESIAN_UPPERCASE		GUI_VIRTUAL_KEYBOARD_INDONESIAN_UPPERCASE
#define MMI_GUI_VIRTUAL_KEYBOARD_HINDI					GUI_VIRTUAL_KEYBOARD_HINDI
#define MMI_GUI_VIRTUAL_KEYBOARD_HINDI_SYMBOLS			GUI_VIRTUAL_KEYBOARD_HINDI_SYMBOLS

//qiff add multi-input method support
#define MMI_GUI_VIRTUAL_KEYBOARD_TAMIL    GUI_VIRTUAL_KEYBOARD_TAMIL_SYMBOLS
#define MMI_GUI_VIRTUAL_KEYBOARD_BENGALI   GUI_VIRTUAL_KEYBOARD_BENGALI_SYMBOLS
#define MMI_GUI_VIRTUAL_KEYBOARD_PUNJABI   GUI_VIRTUAL_KEYBOARD_PUNJABI_SYMBOLS
#define MMI_GUI_VIRTUAL_KEYBOARD_TELUGU    GUI_VIRTUAL_KEYBOARD_TELUGU_SYMBOLS
#define MMI_GUI_VIRTUAL_KEYBOARD_KANNADA    GUI_VIRTUAL_KEYBOARD_KANNADA_SYMBOLS
#define MMI_GUI_VIRTUAL_KEYBOARD_MALAYALAM    GUI_VIRTUAL_KEYBOARD_MALAYALAM_SYMBOLS
#define MMI_GUI_VIRTUAL_KEYBOARD_ORIYA    GUI_VIRTUAL_KEYBOARD_ORIYA_SYMBOLS
//qiff add end
#define MMI_GUI_VIRTUTAL_KEYBOARD_MARATHI GUI_VIRTUAL_KEYBOARD_MARATHI_SYMBOLS

#define MMI_GUI_VIRTUAL_KEYBOARD_MAX_LANG                         GUI_VIRTUAL_KEYBOARD_MAX_LANG
#define mmi_gui_single_line_input_box_pen_enum                    gui_single_line_input_box_pen_enum
#define mmi_gui_single_line_input_box_pen_state_struct                gui_single_line_input_box_pen_state_struct
#define mmi_gui_multi_line_input_box_pen_enum                         gui_multi_line_input_box_pen_enum
#define mmi_gui_multi_line_input_box_pen_state_struct                 gui_multi_line_input_box_pen_state_struct
#define mmi_gui_EMS_input_box_pen_enum                                 gui_EMS_input_box_pen_enum 
/*Guojian Add Start For Touch Ver: Jassmine  on 2008-6-25 16:46 */
#define mmi_wgui_register_category_screen_control_area_pen_handlers                                 wgui_register_category_screen_control_area_pen_handlers   
/*Guojian Add End  For Touch Ver: Jassmine  on 2008-6-25 16:46 */

#define  TRACE_GDI_FUNCTION()

