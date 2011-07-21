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
 *	 MMI_features_video.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Vdieo Feature Customization
 *
 * Author:
 * -------
 *
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/include/MMI_features_video.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:00:40   lin
 * »Ö¸´CSW¸üÐÂÇ°µÄ°æ±¾
 * 
 *    Rev 1.1   Nov 24 2006 18:41:08   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:26:40   zhangxb
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _VIDEO_FEATURES_H_
#define _VIDEO_FEATURES_H_

#include "mmi_features.h"
#ifdef __MMI_VIDEO__



#if defined(CAMERA_MODULE)
   /***************************************************************************** 
	* Video Recorder Feature
	*****************************************************************************/
	//jasper
	#define __VDOREC_FEATURE_ZOOM__
	#define __VDOREC_FEATURE_WB__
//	#define __VDOREC_FEATURE_BT__ //deleted by zhangl for vidrec
	#define __VDOREC_FEATURE_CT__
	#define __VDOREC_FEATURE_EV__	
//	#define __VDOREC_FEATURE_NIGHT__	//deleted by zhangl for vidrec
//	#define __VDOREC_FEATURE_LED_HIGHLIGHT__
//	#define __VDOREC_FEATURE_BANDING__                                                                 
//	#define __VDOREC_FEATURE_VIDEO_QTY__	//deleted by zhangl for vidrec
	#define __VDOREC_FEATURE_SIZE_LIMIT__
	#define __VDOREC_FEATURE_TIME_LIMIT__
	#define __VDOREC_FEATURE_RECORD_AUD__
//	#define __VDOREC_FEATURE_VIDEO_FORMAT__
	#define __VDOREC_FEATURE_EFFECT__
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    #define __VDOREC_FEATURE_STORAGE__        //added by guojian for MP4
#endif
/*- zhouqin modify for dual t-flash card 20110503*/		
		
	#if (defined(__MSDC_MS__) || defined(__MSDC_SD_MMC__))
		#define __VDOREC_FEATURE_STORAGE__
	#endif 
	#ifdef __MMI_TOUCH_SCREEN__
		#define __VDOREC_FEATURE_TOUCH_SCREEN__
	#endif /* __MMI_TOUCH_SCREEN__ */


    #if defined(__MMI_AUDIO_PLAYER__) && defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
//	  #define __VDOPLY_FEATURE_BT__
    #endif 
//	#define __VDOREC_FEATURE_MERGE_MENU__		//guojian delete for jasper	/* will merge "Camcoder Setting" & "Video Setting" into one */
	#define __VDOREC_FEATURE_RECORDING_ZOOM__	   /* allows zooming while recording */
//	#define __VDOREC_FEATURE_RECORDING_EV__		/* allow adjust ev while recording */
//	#define __VDOREC_FEATURE_RECORDING_EFECT__		/* allow adjust ev while recording */
//	#define __VDOREC_FEATURE_RECORDING_WB__		/* allow adjust ev while recording */
//	#define __VDOREC_FEATURE_RECORDING_CT__		/* allow adjust ev while recording */

	/***************************************************************************** 
	* Zoom
	*****************************************************************************/
	/**************ZOOM DEFINITION FOR GREENSTONE_M301******************/	
	#define VDOREC_FEATURE_ZOOM_COUNT			(4)

	#define VDOREC_FEATURE_ZOOM_0 				(0)
	#define VDOREC_FEATURE_ZOOM_1 				(1)
	#define VDOREC_FEATURE_ZOOM_2 				(2)
	#define VDOREC_FEATURE_ZOOM_3 				(3)
	#define VDOREC_FEATURE_ZOOM_4 				(0)
	#define VDOREC_FEATURE_ZOOM_5 				(0)
	#define VDOREC_FEATURE_ZOOM_6 				(0)
	#define VDOREC_FEATURE_ZOOM_7 				(0)
	#define VDOREC_FEATURE_ZOOM_8 				(0)
	#define VDOREC_FEATURE_ZOOM_9 				(0)
	#define VDOREC_FEATURE_ZOOM_10				(0)
	#define VDOREC_FEATURE_ZOOM_11				(0)
	#define VDOREC_FEATURE_ZOOM_12				(0)
	#define VDOREC_FEATURE_ZOOM_13				(0)
	#define VDOREC_FEATURE_ZOOM_14				(0)
	#define VDOREC_FEATURE_ZOOM_15				(0)

	/***************************************************************************** 
	* CamCoder Setting
	*****************************************************************************/
	/******** customizable led highlight *********/
	#define __VDOREC_FEATURE_LED_HIGHLIGHT_ON_OFF__
	//#define __VDOREC_FEATURE_LED_HIGHLIGHT_7_COLOR__


	/***************************************************************************** 
	* Video Recorder Setting
	*****************************************************************************/
	/******* customizable video qualtiy *********/
	//#define __VDOREC_FEATURE_VIDEO_QTY_FINE__
	#define __VDOREC_FEATURE_VIDEO_QTY_HIGH__
//	#define __VDOREC_FEATURE_VIDEO_QTY_NORMAL__
	#define __VDOREC_FEATURE_VIDEO_QTY_LOW__

	/******** customizable wb *********/
	#define __VDOREC_FEATURE_WB_AUTO__
	#define __VDOREC_FEATURE_WB_DAYLIGHT__
	#define __VDOREC_FEATURE_WB_TUNGSTEN__
	#define __VDOREC_FEATURE_WB_FLUORESCENT__
	#define __VDOREC_FEATURE_WB_CLOUD__
//	#define __VDOREC_FEATURE_WB_INCANDESCENCE__

	/******* customizable ev *********/
	/* default will have off [ev 0] and [ev +1] [ev -1] menu if __VDOREC_FEATURE_EV__ is turn on */
	#define __VDOREC_FEATURE_EV_2__
//	#define __VDOREC_FEATURE_EV_3__
//	#define __VDOREC_FEATURE_EV_4__

	/******* customizable effect for greenstone *********/
	  #define    __VDOREC_FEATURE_EFFECT_NORMAL__  //Õý³£
         #define    __VDOREC_FEATURE_EFFECT_ANTIQUE__//¸´¹Å
	  #define    __VDOREC_FEATURE_EFFECT_REDRISH__      // Æ«ºì
	  #define    __VDOREC_FEATURE_EFFECT_GREENRISH__//Æ«ÂÌ
	  #define   __VDOREC_FEATURE_EFFECT_BLUERISH__ //Æ«À¶
	  #define   __VDOREC_FEATURE_EFFECT_BLACKWHITE__ //ºÚ°×
	  #define   __VDOREC_FEATURE_EFFECT_NEGATIVE__   //¸ºÆ¬

#if 0

	/******* customizable effect *********/
	#define __VDOREC_FEATURE_EFFECT_NORMAL__
	#define __VDOREC_FEATURE_EFFECT_GRAYSCALE__
	#define __VDOREC_FEATURE_EFFECT_SEPIA__
	#define __VDOREC_FEATURE_EFFECT_SEPIA_GREEN__
	#define __VDOREC_FEATURE_EFFECT_SEPIA_BLUE__
	#define __VDOREC_FEATURE_EFFECT_COLOR_INVERT__
//	#define __VDOREC_FEATURE_EFFECT_GRAY_INVERT__
	//#define __VDOREC_FEATURE_EFFECT_BLACKBOARD__
	//#define __VDOREC_FEATURE_EFFECT_WHITEBOARD__
	#define __VDOREC_FEATURE_EFFECT_COPPER_CARVING__
	#define __VDOREC_FEATURE_EFFECT_BLUE_CARVING__
	//#define __VDOREC_FEATURE_EFFECT_EMBOSSMENT__
	//#define __VDOREC_FEATURE_EFFECT_CONTRAST__
	//#define __VDOREC_FEATURE_EFFECT_JEAN__ 	
	//#define __VDOREC_FEATURE_EFFECT_SKETCH__
	//#define __VDOREC_FEATURE_EFFECT_OIL__	
#endif

	/******* customizable size limit *********/
	#define VDOREC_FEATURE_SIZE_LIMIT_1 			(4*1024)			/* 4M */
	#define VDOREC_FEATURE_SIZE_LIMIT_2 			(16*1024)			/* 16M */
	#define VDOREC_FEATURE_SIZE_LIMIT_3 			(32*1024)		/* 32M */

	/******* customizable time limit *********/
	#define VDOREC_FEATURE_TIME_LIMIT_1 			(60)			/* 60 sec */
	#define VDOREC_FEATURE_TIME_LIMIT_2 			(10*60)		/* 10 m */
	#define VDOREC_FEATURE_TIME_LIMIT_3 			(30*60)			/* 30m */
	

	/******* SubLCD ********/
	#ifdef __MMI_SUBLCD__
	//	#define __VDOREC_FEATURE_SELF_RECORD__	/* not implement yet */
	#endif /* __MMI_SUBLCD__ */

#ifdef __MMI_BLACKBERRY_QWERTY__
	#define VDOREC_RECORD_KEY					(KEY_ENTER)
#else
    #define VDOREC_RECORD_KEY					(KEY_CAMERA)
#endif
	#define VDOREC_RECORD_EXTRA_KEY			(KEY_IP)				/* if no extra, use -1 */
	#define VDOREC_TOGGLE_SELF_RECORD_KEY	(KEY_STAR)
	
	#define VDOREC_EFFECT_KEY					(KEY_1)			/* if no use hotkey, set -1 */
	#define VDOREC_WB_KEY						(KEY_2)
	#define VDOREC_CT_KEY						(KEY_3)
	#define VDOREC_NIGHT_KEY					(KEY_4)
//	#define VDOREC_LED_HIGHLIGHT_KEY			(KEY_6)
	
	/***************************************************************************** 
	*	Default Setting 
	*****************************************************************************/
	/*********** Preview ***********/
	#define VDOREC_DEFAULT_SETTING_ZOOM					(0)  /* index - start from 0 */

	#define VDOREC_DEFAULT_SETTING_WB					VDOREC_SETTING_WB_SUNNY
	#define VDOREC_DEFAULT_SETTING_LED_HIGHLIGHT		VDOREC_SETTING_LED_HIGHLIGHT_OFF
	#define VDOREC_DEFAULT_SETTING_EV					GS_VIDREC_EV_P2	
	#define VDOREC_DEFAULT_SETTING_BT					GS_VIDREC_BT_L1
	#define VDOREC_DEFAULT_SETTING_CT					GS_VIDREC_CT_L3
	#define VDOREC_DEFAULT_SETTING_NIGHT				VDOREC_SETTING_NIGHT_OFF
	#define VDOREC_DEFAULT_SETTING_BANDING				VDOREC_SETTING_BANDING_60HZ

	#define VDOREC_DEFAULT_SETTING_EFFECT				VDOREC_SETTING_EFFECT_NORMAL

	#define VDOREC_DEFAULT_SETTING_VIDEO_SIZE			VDOREC_SETTING_VIDEO_SIZE_SQCIF
	#define VDOREC_DEFAULT_SETTING_VIDEO_QTY			VDOREC_SETTING_VIDEO_QTY_HIGH
	#define VDOREC_DEFAULT_SETTING_SIZE_LIMIT			VDOREC_SETTING_SIZE_LIMIT_NO_LIMIT
	#define VDOREC_DEFAULT_SETTING_TIME_LIMIT			VDOREC_SETTING_TIME_LIMIT_3
	#define VDOREC_DEFAULT_SETTING_RECORD_AUD			VDOREC_SETTING_RECORD_AUD_ON
	#define VDOREC_DEFAULT_SETTING_VIDEO_FORMAT		VDOREC_SETTING_VIDEO_FORMAT_MP4

	#define VDOREC_DEFAULT_SETTING_STORAGE				VDOREC_SETTING_STORAGE_MEMORY_CARD

	#define VDOREC_DEFAULT_PREVIEW_LED_VALUE			(80)
#if 0
	#define VDOREC_DEFAULT_SETTING_ZOOM					(0)  /* index - start from 0 */

	#define VDOREC_DEFAULT_SETTING_WB					VDOREC_SETTING_WB_AUTO
	#define VDOREC_DEFAULT_SETTING_LED_HIGHLIGHT		VDOREC_SETTING_LED_HIGHLIGHT_OFF
	#define VDOREC_DEFAULT_SETTING_EV					VDOREC_SETTING_EV_ZERO		
	#define VDOREC_DEFAULT_SETTING_NIGHT				VDOREC_SETTING_NIGHT_OFF
	#define VDOREC_DEFAULT_SETTING_BANDING				VDOREC_SETTING_BANDING_60HZ

	#define VDOREC_DEFAULT_SETTING_EFFECT				VDOREC_SETTING_EFFECT_NORMAL

	#define VDOREC_DEFAULT_SETTING_VIDEO_SIZE			VDOREC_SETTING_VIDEO_SIZE_QCIF
	#define VDOREC_DEFAULT_SETTING_VIDEO_QTY			VDOREC_SETTING_VIDEO_QTY_HIGH
	#define VDOREC_DEFAULT_SETTING_SIZE_LIMIT			VDOREC_SETTING_SIZE_LIMIT_NO_LIMIT
	#define VDOREC_DEFAULT_SETTING_TIME_LIMIT			VDOREC_SETTING_TIME_LIMIT_3
	#define VDOREC_DEFAULT_SETTING_RECORD_AUD			VDOREC_SETTING_RECORD_AUD_ON
	#define VDOREC_DEFAULT_SETTING_VIDEO_FORMAT		VDOREC_SETTING_VIDEO_FORMAT_MP4

	#define VDOREC_DEFAULT_SETTING_STORAGE				VDOREC_SETTING_STORAGE_PHONE

	#define VDOREC_DEFAULT_PREVIEW_LED_VALUE			(80)
#endif
	/* preview rotation */
	#if defined(SENSOR_ON_BODY)
		#if defined(SENSOR_ROTATE_0)
			#define VDOREC_MAINLCD_ROTATE					VDOREC_PREVIEW_ROTATE_0
			#define VDOREC_SUBLCD_CLAM_OPEN_ROTATE		VDOREC_PREVIEW_ROTATE_0_MIRROR
			#define VDOREC_SUBLCD_CLAM_CLOSE_ROTATE 	VDOREC_PREVIEW_ROTATE_0_MIRROR
		#elif defined(SENSOR_ROTATE_180)
			#define VDOREC_MAINLCD_ROTATE					VDOREC_PREVIEW_ROTATE_180
			#define VDOREC_SUBLCD_CLAM_OPEN_ROTATE		VDOREC_PREVIEW_ROTATE_180_MIRROR
			#define VDOREC_SUBLCD_CLAM_CLOSE_ROTATE 	VDOREC_PREVIEW_ROTATE_180_MIRROR
		#else
			/* rotate 90, 270 not used yet */
			#define VDOREC_MAINLCD_ROTATE					VDOREC_PREVIEW_ROTATE_0
			#define VDOREC_SUBLCD_CLAM_OPEN_ROTATE		VDOREC_PREVIEW_ROTATE_0_MIRROR
			#define VDOREC_SUBLCD_CLAM_CLOSE_ROTATE 	VDOREC_PREVIEW_ROTATE_0_MIRROR
		#endif
	#elif defined(SENSOR_ON_COVER)
		#if defined(SENSOR_ROTATE_0)
			#define VDOREC_MAINLCD_ROTATE					VDOREC_PREVIEW_ROTATE_0
			#define VDOREC_SUBLCD_CLAM_OPEN_ROTATE		VDOREC_PREVIEW_ROTATE_0_MIRROR
			#define VDOREC_SUBLCD_CLAM_CLOSE_ROTATE 	VDOREC_PREVIEW_ROTATE_180_MIRROR
		#elif defined(SENSOR_ROTATE_180)
			#define VDOREC_MAINLCD_ROTATE					VDOREC_PREVIEW_ROTATE_180
			#define VDOREC_SUBLCD_CLAM_OPEN_ROTATE		VDOREC_PREVIEW_ROTATE_180_MIRROR
			#define VDOREC_SUBLCD_CLAM_CLOSE_ROTATE 	VDOREC_PREVIEW_ROTATE_0_MIRROR
		#else
			/* rotate 90, 270 not used yet */
			#define VDOREC_MAINLCD_ROTATE					VDOREC_PREVIEW_ROTATE_0
			#define VDOREC_SUBLCD_CLAM_OPEN_ROTATE		VDOREC_PREVIEW_ROTATE_0_MIRROR
			#define VDOREC_SUBLCD_CLAM_CLOSE_ROTATE 	VDOREC_PREVIEW_ROTATE_180_MIRROR
		#endif
	#else /* not define SENSOR_ROTATION */
		#if defined(SENSOR_ROTATE_0)
			#define VDOREC_MAINLCD_ROTATE					VDOREC_PREVIEW_ROTATE_0
			#define VDOREC_SUBLCD_CLAM_OPEN_ROTATE		VDOREC_PREVIEW_ROTATE_0_MIRROR
			#define VDOREC_SUBLCD_CLAM_CLOSE_ROTATE 	VDOREC_PREVIEW_ROTATE_180_MIRROR
		#elif defined(SENSOR_ROTATE_180)
			#define VDOREC_MAINLCD_ROTATE					VDOREC_PREVIEW_ROTATE_180
			#define VDOREC_SUBLCD_CLAM_OPEN_ROTATE		VDOREC_PREVIEW_ROTATE_180_MIRROR
			#define VDOREC_SUBLCD_CLAM_CLOSE_ROTATE 	VDOREC_PREVIEW_ROTATE_0_MIRROR
		#else
			/* rotate 90, 270 not used yet */
			#define VDOREC_MAINLCD_ROTATE					VDOREC_PREVIEW_ROTATE_0
			#define VDOREC_SUBLCD_CLAM_OPEN_ROTATE		VDOREC_PREVIEW_ROTATE_0_MIRROR
			#define VDOREC_SUBLCD_CLAM_CLOSE_ROTATE 	VDOREC_PREVIEW_ROTATE_180_MIRROR
		#endif
	#endif /* SENSOR_ROTATION */


	/***************************************************************************** 
	* Video Player Feature
	*****************************************************************************/
	#ifdef __MMI_TOUCH_SCREEN__
		#define __VDOPLY_FEATURE_TOUCH_SCREEN__
	#endif /* __MMI_TOUCH_SCREEN__ */

//	#define __VDOPLY_FEATURE_SPEED__
	#define __VDOPLY_FEATURE_VOLUME__
    #if !defined(__MMI_MAINLCD_160X128__) && !defined(__MMI_MAINLCD_128X128__)
	#define __VDOPLY_FEATURE_FULLSCREEN__
    #endif
	//#define __VDOPLY_FEATURE_SNAPSHOT__

	#define VDOPLY_FEATURE_SPEED_COUNT 	(2)
	
	#define VDOPLY_FEATURE_SPEED_0		(-100)
	#define VDOPLY_FEATURE_SPEED_1		(100)
	#define VDOPLY_FEATURE_SPEED_2		(0)
	#define VDOPLY_FEATURE_SPEED_3		(0)
	#define VDOPLY_FEATURE_SPEED_4		(0)
	#define VDOPLY_FEATURE_SPEED_5		(0)
	#define VDOPLY_FEATURE_SPEED_6		(0)
	#define VDOPLY_FEATURE_SPEED_7		(0)
	#define VDOPLY_FEATURE_SPEED_8		(0)

	#define VDOPLY_DEFAULT_SPEED			(1)	 /* index - start from 0 */

#elif defined(ISP_SUPPORT)
#error
#endif /* CAMERA_MODULE, ISP_SUPPORT */


#endif /* __MMI_VIDEO__ */
#endif /* _VIDEO_FEATURES_H_ */


