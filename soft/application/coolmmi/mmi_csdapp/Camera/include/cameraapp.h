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
 *	 CameraApp.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Define Camera related data type
 *
 * Author:
 * -------
 *	                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _CAMERA_DATATYPE_H_
#define _CAMERA_DATATYPE_H_

#include "mmi_features.h"
#ifdef __MMI_CAMERA__

#include "resource_camera_skins.h"		/* ui custom config */
#ifdef __CAMERA_FEATURE_TOUCH_SCREEN__
	#include "wgui_touch_screen.h"
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ */

#include "gdi_datatype.h" /* wufasong added 2007.03.02 */
#include "mdi_datatype.h" /* wufasong added 2007.03.02 */
 
/****************************************************************************
* Define                                                            
*****************************************************************************/
/* Storage file path */
/* wufasong updated 2007.01.22 */
//#define CAMERA_STORAGE_FILEPATH_PHONE				FMGR_DEFAULT_FOLDER_PHOTOS
//#define CAMERA_STORAGE_FILEPATH_MEMORY_CARD		FMGR_DEFAULT_FOLDER_PHOTOS
//#define CAMERA_STORAGE_UNSAVE_FILENAME				FMGR_TEMP_FILE_CAMERA
extern const U8 CAMERA_STORAGE_FILEPATH_PHONE[];
extern const U8 CAMERA_STORAGE_FILEPATH_MEMORY_CARD[];
//extern U8 CAMERA_STORAGE_UNSAVE_FILENAME[];

/* capture to memory - [burst shot] and [multi shot] */ 
#define CAMERA_MEMORY_CAPTURE_IMAGE_COUNT			(12)	 

#define CAMERA_HINT_CHAR_COUNT					(25)
#define CAMERA_HINT_BUF_SIZE						((CAMERA_HINT_CHAR_COUNT+1)*ENCODING_LENGTH)

#define MMI_CAMERA_APP_WATCHDOG_DURATION (1000*60*2)
#define MMI_CAMERA_APP_POLLING_DURATION (2000)

#define MMI_CAMERA_DELETE_CONFIRM_WATCHDOG_DURATION (5000)
#define MMI_CAMERA_DELETE_CONFIRM_POLLING_DURATION (1000)
/* rotate */
#define CAMERA_PREVIEW_ROTATE_0 					MDI_CAMERA_PREVIEW_ROTATE_0
#define CAMERA_PREVIEW_ROTATE_90					MDI_CAMERA_PREVIEW_ROTATE_90
#define CAMERA_PREVIEW_ROTATE_180 				MDI_CAMERA_PREVIEW_ROTATE_180
#define CAMERA_PREVIEW_RORATE_270				MDI_CAMERA_PREVIEW_ROTATE_270
#define CAMERA_PREVIEW_ROTATE_0_MIRROR 		MDI_CAMERA_PREVIEW_ROTATE_0_MIRROR
#define CAMERA_PREVIEW_ROTATE_90_MIRROR 		MDI_CAMERA_PREVIEW_ROTATE_90_MIRROR
#define CAMERA_PREVIEW_ROTATE_180_MIRROR		MDI_CAMERA_PREVIEW_ROTATE_180_MIRROR
#define CAMERA_PREVIEW_ROTATE_270_MIRROR		MDI_CAMERA_PREVIEW_ROTATE_270_MIRROR

#define CAMERA_LCM_ROTATE_0 						MDI_CAMERA_LCM_ROTATE_0
#define CAMERA_LCM_ROTATE_90						MDI_CAMERA_LCM_ROTATE_90
#define CAMERA_LCM_ROTATE_180 					MDI_CAMERA_LCM_ROTATE_180
#define CAMERA_LCM_RORATE_270						MDI_CAMERA_LCM_ROTATE_270
#define CAMERA_LCM_ROTATE_0_MIRROR 				MDI_CAMERA_LCM_ROTATE_0_MIRROR
#define CAMERA_LCM_ROTATE_90_MIRROR 			MDI_CAMERA_LCM_ROTATE_90_MIRROR
#define CAMERA_LCM_ROTATE_180_MIRROR			MDI_CAMERA_LCM_ROTATE_180_MIRROR
#define CAMERA_LCM_ROTATE_270_MIRROR			MDI_CAMERA_LCM_ROTATE_270_MIRROR

#define CAMERA_SETTING_DUMMY	(0)

//wufasong added 2007.01.18
#define CAMERA_WARNING_POPUPDURATION  5 /* 5 seconds */

/* Debug info */
/*
#define CAMERA_CHECK_STATE(_STATE_)				MMI_ASSERT(g_camera_cntx.app_state == _STATE_);
#define CAMERA_ENTER_STATE(_STATE_)				{\
																	CAMERA_TRACE_EXIT_STATE();\
																	g_camera_cntx.app_next_state = _STATE_;\
																	mmi_camera_exit_current_state();\
																	CAMERA_TRACE_ENTER_STATE();\
																	mmi_camera_enter_state(_STATE_);\
															}


#define CAMERA_TRACE_FUNC_INIT()					kal_trace(MMI_TRACE_G8_MEDIA, MMI_CAMERA_FUNC_INIT);							
#define CAMERA_TRACE_FUNC_ENTER_APP()			kal_trace(MMI_TRACE_G8_MEDIA, MMI_CAMERA_FUNC_ENTER_APP);
#define CAMERA_TRACE_FUNC_EXIT_APP()			kal_trace(MMI_TRACE_G8_MEDIA, MMI_CAMERA_FUNC_EXIT_APP);
#define CAMERA_TRACE_FUNC_ENTER_SUBLCD()		kal_trace(MMI_TRACE_G8_MEDIA, MMI_CAMERA_FUNC_ENTER_SUBLCD);
#define CAMERA_TRACE_FUNC_EXIT_SUBLCD()		kal_trace(MMI_TRACE_G8_MEDIA, MMI_CAMERA_FUNC_EXIT_SUBLCD);

#define CAMERA_TRACE_CLAM_CLOSE_EVENT()		kal_trace(MMI_TRACE_G8_MEDIA, MMI_CAMERA_CLAM_CLOSE_EVENT);
#define CAMERA_TRACE_CLAM_OPEN_EVENT()			kal_trace(MMI_TRACE_G8_MEDIA, MMI_CAMERA_CLAM_OPEN_EVENT);




#define CAMERA_TRACE_ENTER_STATE()				kal_trace(MMI_TRACE_G8_MEDIA, MMI_CAMERA_ENTER_STATE, g_camera_cntx.app_state);
#define CAMERA_TRACE_EXIT_STATE()				kal_trace(MMI_TRACE_G8_MEDIA, MMI_CAMERA_EXIT_STATE, g_camera_cntx.app_state);
*/



#define CAMERA_CHECK_STATE(_STATE_)				MMI_ASSERT(g_camera_cntx.app_state == _STATE_);
#define CAMERA_ENTER_STATE(_STATE_)				{\
																	CAMERA_TRACE_EXIT_STATE();\
																	g_camera_cntx.app_next_state = _STATE_;\
																	mmi_camera_exit_current_state();\
																	CAMERA_TRACE_ENTER_STATE();\
																	mmi_camera_enter_state(_STATE_);\
															}


#define CAMERA_TRACE_FUNC_INIT()					{}					
#define CAMERA_TRACE_FUNC_ENTER_APP()			{}	
#define CAMERA_TRACE_FUNC_EXIT_APP()			{}	
#define CAMERA_TRACE_FUNC_ENTER_SUBLCD()		{}	
#define CAMERA_TRACE_FUNC_EXIT_SUBLCD()		{}	

#define CAMERA_TRACE_CLAM_CLOSE_EVENT()		{}	
#define CAMERA_TRACE_CLAM_OPEN_EVENT()			{}	

#define CAMERA_TRACE_ENTER_STATE()				{}	
#define CAMERA_TRACE_EXIT_STATE()				{}	



/****************************************************************************
* Typedef                                                      
*****************************************************************************/
/*********** CAPTURE PATH ***********/
typedef enum {

	CAPTURE_FROM_MEMORY,
	CAPTURE_FROM_LAYER,
	
	NUM_OF_CAPTURE_PATH
}camera_capture_path;

/*********** WB ***********/
typedef enum {

//#ifdef __CAMERA_FEATURE_WB_AUTO__
	CAMERA_SETTING_WB_AUTO,/*Auto */
//#endif 

//#ifdef __CAMERA_FEATURE_WB_DAYLIGHT__
	CAMERA_SETTING_WB_SUNNY,/*ÇçÌìÌ«Ñô¹â*/
//#endif 

//#ifdef __CAMERA_FEATURE_WB_TUNGSTEN__
	CAMERA_SETTING_WB_TUNGSTEN,/*ÎÙË¿µÆ*/
//#endif 

//#ifdef __CAMERA_FEATURE_WB_OFFICE__
	CAMERA_SETTING_WB_OFFICE,/*ÈÕ¹âµÆ*/	
//#endif 

//#ifdef __CAMERA_FEATURE_WB_FLUORESCENT__
	CAMERA_SETTING_WB_CLOUDY,/*ÒõÌì*/
//#endif 



	CAMERA_SETTING_WB_COUNT
} camera_setting_wb_enum;
 

/*********** Flash ***********/
typedef enum {
	CAMERA_SETTING_FLASH_OFF,
	CAMERA_SETTING_FLASH_ON,
	#ifdef __CAMERA_FEATURE_FLASH_AUTO__
		CAMERA_SETTING_FLASH_AUTO,
	#endif  
	
	#ifdef __CAMERA_FEATURE_FLASH_REDEYE__
		CAMERA_SETTING_FLASH_REDEYE,
	#endif  
	
	CAMERA_SETTING_FLASH_COUNT
} camera_setting_flash_enum;


/*********** LED Highlight ***********/
typedef enum {

	CAMERA_SETTING_LED_HIGHLIGHT_OFF,
//#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT_ON_OFF__
	CAMERA_SETTING_LED_HIGHLIGHT_ON,
//#endif 

//#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT_7_COLOR__
	CAMERA_SETTING_LED_HIGHLIGHT_WHITE,			
	CAMERA_SETTING_LED_HIGHLIGHT_RED,			
	CAMERA_SETTING_LED_HIGHLIGHT_GREEN,				
	CAMERA_SETTING_LED_HIGHLIGHT_BLUE,					
	CAMERA_SETTING_LED_HIGHLIGHT_YELLOW,						
	CAMERA_SETTING_LED_HIGHLIGHT_PURPLE,							
	CAMERA_SETTING_LED_HIGHLIGHT_CYAN,	
//#endif 

	CAMERA_SETTING_LED_HIGHLIGHT_COUNT
} camera_setting_led_highlight_enum;


/*********** Shutter Sound ***********/
typedef enum {

#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_OFF__			
	CAMERA_SETTING_SHUTTER_SOUND_OFF,
#endif 

#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_1__	
	CAMERA_SETTING_SHUTTER_SOUND_1,
#endif 

#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_2__
	CAMERA_SETTING_SHUTTER_SOUND_2,
#endif 

#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_3__
	CAMERA_SETTING_SHUTTER_SOUND_3,
#endif 
	CAMERA_SETTING_SHUTTER_SOUND_COUNT
} camera_setting_shutter_sound_enum;



/*********** EV ***********/
typedef enum {

//#ifdef __CAMERA_FEATURE_EV_4__
	//CAMERA_SETTING_EV_N4,
//#endif 

//#ifdef __CAMERA_FEATURE_EV_3__	
	
	//CAMERA_SETTING_EV_N3,
//#endif 

//#ifdef __CAMERA_FEATURE_EV_2__	
	CAMERA_SETTING_EV_N2,
//#endif
  
	CAMERA_SETTING_EV_N1,
	CAMERA_SETTING_EV_0,
	CAMERA_SETTING_EV_P1,

//#ifdef __CAMERA_FEATURE_EV_2__		
	CAMERA_SETTING_EV_P2,
//#endif 

//#ifdef __CAMERA_FEATURE_EV_3__		
	//CAMERA_SETTING_EV_P3,
//#endif

//#ifdef __CAMERA_FEATURE_EV_4__
	//CAMERA_SETTING_EV_P4,
//#endif 
	CAMERA_SETTING_EV_COUNT

} camera_setting_ev_enum;



/*********** Night ***********/
typedef enum {
	CAMERA_SETTING_NIGHT_OFF = 0, 
	CAMERA_SETTING_NIGHT_ON,
	CAMERA_SETTING_NIGHT_COUNT
} camera_setting_night_enum;



/*********** Closeup ***********/
typedef enum {
	CAMERA_SETTING_CLOSEUP_OFF = 0,
	CAMERA_SETTING_CLOSEUP_ON,
	CAMERA_SETTING_CLOSEUP_COUNT
} camera_setting_closeup_enum;



/*********** Banding ***********/
typedef enum {
	CAMERA_SETTING_BANDING_50HZ = 0, 
	CAMERA_SETTING_BANDING_60HZ,
	CAMERA_SETTING_BANDING_COUNT
} camera_setting_banding_enum;



/*********** Effect ***********/
typedef enum {

#ifdef __CAMERA_FEATURE_EFFECT_NORMAL__
	CAMERA_SETTING_EFFECT_NORMAL,
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_GRAYSCALE__
	CAMERA_SETTING_EFFECT_GRAYSCALE,
#endif

#ifdef __CAMERA_FEATURE_EFFECT_SEPIA__
	CAMERA_SETTING_EFFECT_SEPIA,
#endif

#ifdef __CAMERA_FEATURE_EFFECT_SEPIA_GREEN__
	CAMERA_SETTING_EFFECT_SEPIA_GREEN,
#endif

#ifdef __CAMERA_FEATURE_EFFECT_SEPIA_BLUE__
	CAMERA_SETTING_EFFECT_SEPIA_BLUE,
#endif

#ifdef __CAMERA_FEATURE_EFFECT_COLOR_INVERT__
	CAMERA_SETTING_EFFECT_COLOR_INVERT,
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_GRAY_INVERT__ 
	CAMERA_SETTING_EFFECT_GRAY_INVERT,
#endif

#ifdef __CAMERA_FEATURE_EFFECT_BLACKBOARD__ 
	CAMERA_SETTING_EFFECT_BLACKBOARD,
#endif

#ifdef __CAMERA_FEATURE_EFFECT_WHITEBOARD__ 
	CAMERA_SETTING_EFFECT_WHITEBOARD,
#endif

#ifdef __CAMERA_FEATURE_EFFECT_COPPER_CARVING__ 
	CAMERA_SETTING_EFFECT_COPPER_CARVING,
#endif

#ifdef __CAMERA_FEATURE_EFFECT_BLUE_CARVING__ 
	CAMERA_SETTING_EFFECT_BLUE_CARVING,
#endif										

#ifdef __CAMERA_FEATURE_EFFECT_EMBOSSMENT__ 
	CAMERA_SETTING_EFFECT_EMBOSSMENT,
#endif	

#ifdef __CAMERA_FEATURE_EFFECT_CONTRAST__ 
	CAMERA_SETTING_EFFECT_CONTRAST,
#endif	

#ifdef __CAMERA_FEATURE_EFFECT_JEAN__ 
	CAMERA_SETTING_EFFECT_JEAN,
#endif		

#ifdef __CAMERA_FEATURE_EFFECT_SKETCH__ 
	CAMERA_SETTING_EFFECT_SKETCH,
#endif	

#ifdef __CAMERA_FEATURE_EFFECT_OIL__ 
	CAMERA_SETTING_EFFECT_OIL,
#endif											

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ 
	CAMERA_SETTING_EFFECT_CUSTOM_FX_1,
#endif	

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_2__ 
	CAMERA_SETTING_EFFECT_CUSTOM_FX_2,
#endif	

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_3__ 
	CAMERA_SETTING_EFFECT_CUSTOM_FX_3,
#endif	


/////added  by gugogi
#ifdef  __CAMERA_FEATURE_EFFECT_ANTIQUE__
     CAMERA_SETTING_EFFECT_ANTIQUE,
#endif

#ifdef  __CAMERA_FEATURE_EFFECT_REDRISH__
    CAMERA_SETTING_EFFECT_REDRISH,
#endif

#ifdef  __CAMERA_FEATURE_EFFECT_GREENRISH__
    CAMERA_SETTING_EFFECT_GREENRISH,
#endif

#ifdef  __CAMERA_FEATURE_EFFECT_BLUERISH__
    CAMERA_SETTING_EFFECT_BLUERISH,
#endif


#ifdef __CAMERA_FEATURE_EFFECT_BLACKWHITE__
     CAMERA_SETTING_EFFECT_BLACKWHITE,
#endif


#ifdef  __CAMERA_FEATURE_EFFECT_NEGATIVE__
    CAMERA_SETTING_EFFECT_NEGATIVENESS,
#endif


	CAMERA_SETTING_EFFECT_COUNT

} camera_setting_effect_setting_enum;
 


/*********** Image Size ***********/
/*gaosh Modify Start For 5841 Ver: TBM_780  on 2007-8-3 12:53 */
typedef enum {

#ifdef __CAMERA_FEATURE_IMAGE_SIZE_SS__
	CAMERA_SETTING_IMAGE_SIZE_SS,
#endif 

#ifdef __CAMERA_FEATURE_IMAGE_SIZE_S__
#if defined(MEM_ULC_3216)
CAMERA_SETTING_IMAGE_SIZE_S,     //deleted by kecx for fixing bug 13004 on 20090818
#endif
#endif 

#ifdef __CAMERA_FEATURE_IMAGE_SIZE_M__
	CAMERA_SETTING_IMAGE_SIZE_M,
#endif 

#ifdef __CAMERA_FEATURE_IMAGE_SIZE_L__
	CAMERA_SETTING_IMAGE_SIZE_L,
#endif 

#ifdef __CAMERA_FEATURE_IMAGE_SIZE_LL__
	CAMERA_SETTING_IMAGE_SIZE_LL,
#endif 
	
	CAMERA_SETTING_IMAGE_SIZE_COUNT
} camera_setting_image_size_enum;
/*gaosh Modify End  For 5841 Ver: TBM_780  on 2007-8-3 12:53 */



/*********** Image Qty ***********/
typedef enum {

//#ifdef __CAMERA_FEATURE_IMAGE_QTY_LOW__
	//CAMERA_SETTING_IMAGE_QTY_LOW,
//#endif 

//#ifdef __CAMERA_FEATURE_IMAGE_QTY_NORMAL__
	CAMERA_SETTING_IMAGE_QTY_NORMAL,
//#endif 

//#ifdef __CAMERA_FEATURE_IMAGE_QTY_HIGH__
	CAMERA_SETTING_IMAGE_QTY_HIGH,
//#endif 

//#ifdef __CAMERA_FEATURE_IMAGE_QTY_FINE__
	//CAMERA_SETTING_IMAGE_QTY_FINE,
//#endif 
	
	CAMERA_SETTING_IMAGE_QTY_COUNT
} camera_setting_image_qty_enum;



/******* [Add Frame Menu] ********/
typedef enum {

	CAMERA_SETTING_ADD_FRAME_OFF,

#ifdef __CAMERA_FEATURE_ADD_FRAME_1__
	CAMERA_SETTING_ADD_FRAME_1,
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_2__
	CAMERA_SETTING_ADD_FRAME_2,
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_3__
	CAMERA_SETTING_ADD_FRAME_3,
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_4__
	CAMERA_SETTING_ADD_FRAME_4,
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_5__
	CAMERA_SETTING_ADD_FRAME_5,		
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_6__
	CAMERA_SETTING_ADD_FRAME_6,		
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_7__
	CAMERA_SETTING_ADD_FRAME_7,		
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_8__
	CAMERA_SETTING_ADD_FRAME_8,		
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_9__
	CAMERA_SETTING_ADD_FRAME_9,		
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_10__
	CAMERA_SETTING_ADD_FRAME_10,
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_CUSTOM_1__
	CAMERA_SETTING_ADD_FRAME_CUSTOM_1,
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_CUSTOM_2__
	CAMERA_SETTING_ADD_FRAME_CUSTOM_2,
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_CUSTOM_3__
	CAMERA_SETTING_ADD_FRAME_CUSTOM_3,
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_CUSTOM_4__
	CAMERA_SETTING_ADD_FRAME_CUSTOM_4,
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME_CUSTOM_5__
	CAMERA_SETTING_ADD_FRAME_CUSTOM_5,
#endif 

	CAMERA_SETTING_ADD_FRAME_COUNT

} camera_setting_add_frame_enum;



/*********** Cont Shot ***********/
typedef enum {

	CAMERA_SETTING_CONT_SHOT_OFF,
	CAMERA_SETTING_CONT_SHOT_1,

	#ifdef __CAMERA_FEATURE_CONT_SHOT_2__
		CAMERA_SETTING_CONT_SHOT_2,
	#endif

	#ifdef __CAMERA_FEATURE_CONT_SHOT_3__
		CAMERA_SETTING_CONT_SHOT_3,
	#endif

	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
		CAMERA_SETTING_CONT_SHOT_BURST,
	#endif
	
	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		CAMERA_SETTING_CONT_SHOT_TILE,
	#endif
	
	CAMERA_SETTING_CONT_SHOT_COUNT
} camera_setting_cont_shot_enum;




/*********** Delay Timer ***********/
typedef enum {

	CAMERA_SETTING_DELAY_TIMER_OFF,
	CAMERA_SETTING_DELAY_TIMER_1,

	#ifdef __CAMERA_FEATURE_DELAY_TIMER_2__
		CAMERA_SETTING_DELAY_TIMER_2,
	#endif

	#ifdef __CAMERA_FEATURE_DELAY_TIMER_3__
		CAMERA_SETTING_DELAY_TIMER_3,
	#endif
	CAMERA_SETTING_DELAY_TIMER_COUNT
} camera_setting_delay_timer_enum;


/*********** AE Meter ***********/
typedef enum {

	//#ifdef __CAMERA_FEATURE_AE_METER_AUTO__
		CAMERA_SETTING_AE_METER_AUTO,
	//#endif

	//#ifdef __CAMERA_FEATURE_AE_METER_SPOT__
		CAMERA_SETTING_AE_METER_SPOT,
	//#endif

	//#ifdef __CAMERA_FEATURE_AE_METER_CENTER__
		CAMERA_SETTING_AE_METER_CENTER,
	//#endif

	//#ifdef __CAMERA_FEATURE_AE_METER_AVERAGE__
		CAMERA_SETTING_AE_METER_AVERAGE,
	//#endif
	CAMERA_SETTING_AE_METER_COUNT
} camera_setting_ae_meter_enum;

//add by WeiD
typedef enum {
	CAMERA_SETTING_BRIGHT_1 = 0,
	CAMERA_SETTING_BRIGHT_2,
	CAMERA_SETTING_BRIGHT_3,
	CAMERA_SETTING_BRIGHT_4,
	CAMERA_SETTING_BRIGHT_5,
	CAMERA_SETTING_BT_COUNT
}cool_camera_setting_brightness_enum;


typedef enum {
	CAMERA_SETTING_CONTRAST_1 = 0,
	CAMERA_SETTING_CONTRAST_2,
	CAMERA_SETTING_CONTRAST_3,
	CAMERA_SETTING_CONTRAST_4,
	CAMERA_SETTING_CONTRAST_5,
	CAMERA_SETTING_CT_COUNT
}cool_camera_setting_contrast_enum;


//add by WeiD
/*********** Shot mode ***********/
typedef enum {

	#ifdef __CAMERA_FEATURE_SHOT_MODE_AUTO__
		CAMERA_SETTING_SHOT_MODE_AUTO,
	#endif

	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI__
		CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI,
	#endif

	#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI__
		CAMERA_SETTING_SHOT_MODE_APERTURE_PRI,
	#endif

	#ifdef __CAMERA_FEATURE_SHOT_MODE_PORTRAIT__
		CAMERA_SETTING_SHOT_MODE_PORTRAIT,
	#endif

	#ifdef __CAMERA_FEATURE_SHOT_MODE_LANDSCAPE__
		CAMERA_SETTING_SHOT_MODE_LANDSCAPE,
	#endif

	#ifdef __CAMERA_FEATURE_SHOT_MODE_SPORT__
		CAMERA_SETTING_SHOT_MODE_SPORT,
	#endif

	#ifdef __CAMERA_FEATURE_SHOT_MODE_FLOWER__
		CAMERA_SETTING_SHOT_MODE_FLOWER,
	#endif

	#ifdef __CAMERA_FEATURE_SHOT_MODE_NIGHT__
		CAMERA_SETTING_SHOT_MODE_NIGHT,
	#endif
	
	CAMERA_SETTING_SHOT_MODE_COUNT
} camera_setting_shot_mode_enum;


/*********** SHOT MODE: SHUTTER PRI ***********/
typedef enum {
	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_0__
	CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI_0,
	#endif
	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_1__
	CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI_1,
	#endif
	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_2__
	CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI_2,
	#endif
	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_3__
	CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI_3,
	#endif
	#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_4__
	CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI_4,
	#endif
	CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI_COUNT
}camera_setting_shot_mode_shutter_pri_enum;


/*********** SHOT MODE: APERTUNE PRI ***********/
typedef enum {
	#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI_0__
	CAMERA_SETTING_SHOT_MODE_APERTURE_PRI_0,
	#endif
	#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI_1__
	CAMERA_SETTING_SHOT_MODE_APERTURE_PRI_1,
	#endif
	#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI_2__
	CAMERA_SETTING_SHOT_MODE_APERTURE_PRI_2,
	#endif
	CAMERA_SETTING_SHOT_MODE_APERTURE_PRI_COUNT
}camera_setting_shot_mode_aperture_pri_enum;



/*********** ISO ***********/
typedef enum {

	#ifdef __CAEMRA_FEATURE_ISO_AUTO__
		CAMERA_SETTING_ISO_AUTO,
	#endif

	#ifdef __CAEMRA_FEATURE_ISO_100__
		CAMERA_SETTING_ISO_100,
	#endif

	#ifdef __CAEMRA_FEATURE_ISO_200__
		CAMERA_SETTING_ISO_200,
	#endif

	#ifdef __CAEMRA_FEATURE_ISO_400__
		CAMERA_SETTING_ISO_400,
	#endif

	CAMERA_SETTING_ISO_COUNT
} camera_setting_iso_enum;



/*********** Storage ***********/
typedef enum {
	CAMERA_SETTING_STORAGE_PHONE,
	CAMERA_SETTING_STORAGE_MEMORY_CARD,
	/*+ zhouqin modify for dual t-flash card 20110503*/
	#if defined(DUAL_TFLASH_SUPPORT)
	CAMERA_SETTING_STORAGE_MEMORY_SEC_CARD,
	#endif
	/*- zhouqin modify for dual t-flash card 20110503*/
	CAMERA_SETTING_STORAGE_COUNT
} camera_setting_storage_enum;




#ifdef __CAMERA_FEATURE_OSD_MENU__
	/*********** OSD Menu ***********/
	typedef enum {
			CAMERA_OSD_MENU_IMAGE_SIZE = 0,
			CAMERA_OSD_MENU_IMAGE_QTY,
				
		#ifdef __CAMERA_FEATURE_WB__
			CAMERA_OSD_MENU_WB,
		#endif 

		#ifdef __CAMERA_FEATURE_EFFECT__
			CAMERA_OSD_MENU_EFFECT,
		#endif 	

		#ifdef __CAMERA_FEATURE_NIGHT__
			CAMERA_OSD_MENU_NIGHT,
		#endif 	

		#ifdef __CAMERA_FEATURE_ADD_FRAME__
			CAMERA_OSD_MENU_ADD_FRAME,
		#endif 	

		#ifdef __CAMERA_FEATURE_CONT_SHOT__
			CAMERA_OSD_MENU_CONT_SHOT,
		#endif
		
		#ifdef __CAMERA_FEATURE_DELAY_TIMER__
			CAMERA_OSD_MENU_DELAY_TIMER,
		#endif 	

		#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
			CAMERA_OSD_MENU_LED_HIGHLIGHT,
		#endif 

		#ifdef __CAMERA_FEATURE_SHUTTER_SOUND__
			CAMERA_OSD_MENU_SHUTTER_SOUND,
		#endif 

		#ifdef __CAMERA_FEATURE_BANDING__
			CAMERA_OSD_MENU_BANDING,
		#endif 

		#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
			CAMERA_OSD_MENU_SELF_CAPTURE,
		#endif 

		#ifdef __CAMERA_FEATURE_STORAGE__
			CAMERA_OSD_MENU_STORAGE,
		#endif 	
		
		CAMERA_OSD_MENU_COUNT
	} camera_osd_menu_enum;
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*********** OSD Status Icons ***********/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
	typedef enum {
		CAMERA_ICONBAR_ZOOM,
		CAMERA_ICONBAR_EV,			
		CAMERA_ICONBAR_EFFECT,		
		CAMERA_ICONBAR_WB,
		CAMERA_ICONBAR_FLASH,
		CAMERA_ICONBAR_LED_HIGHLIGHT,
		CAMERA_ICONBAR_NIGHT,
		CAMERA_ICONBAR_CLOSEUP,
		CAMERA_ICONBAR_ADD_FRAME,		
		CAMERA_ICONBAR_CONT_SHOT,	
		CAMERA_ICONBAR_DELAY_TIMER,
		CAMERA_ICONBAR_IMAGE_SIZE,
		CAMERA_ICONBAR_IMAGE_QTY,
		CAMERA_ICONBAR_STORAGE,

		/* icon count */
		CAMERA_ICONBAR_ITEM_COUNT
	} camera_iconbar_enum;
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */


typedef struct {
	U8 r;
	U8 g;
	U8 b;
} camera_flash_struct;


/* Setting are value will be store in nvram */
/* NVRAM ITEM SIZE: 84 byte */
typedef struct {

	/* camera setting - 16 byte */
	U16	wb;
	U16	flash;
	U16	led_highlight;
	U16	shutter_sound;
	U16	ev;	
	U16	night;
	U16	closeup;
	U16	banding;

	/* image setting - 4 byte */ 
	U16	image_size;
	U16	image_qty;
	
	/* others - 12 byte */
	U16	effect;
	U16	add_frame;
	U16	cont_shot;
	U16	delay_timer;
	U16	storage;
	U16	zoom_idx;	

	/* custom effect - 24 byte */ 
	U16	fx_brightness[3];
	U16	fx_saturation[3];
	U16	fx_contrast[3];
	U16	fx_hue[3];		
	
	/* extra - 6 byte */
	U16	iso;
	U16	ae_meter;
	U16	shot_mode;

	/* shot mode setting - 4 byte*/
	U16	shutter_pri;
	U16	aperture_pri;
	
	/* reserve - 10 byte */
	U16	reserve5;    //for brightness
#if defined(__MMI_MAINLCD_220X176__)
	U16 contrast;
#else
	U16	reserve6;	//for contrast
#endif
	U16	reserve7;
	U16	reserve8;
	U16	reserve9;

	/* flash & LED - 8 byte */
	U16		preview_flash_value;
	U16		capture_flash_value;
	U16		preview_led_value;
	U16		capture_led_value;
	
} camera_setting_struct;


typedef struct {
	U32 offset;
	U32 size;
} camera_memory_capture_struct;


/* multimedia ui component */

typedef struct {
	U16	image_id;
	S32	offset_x;
	S32	offset_y;
	S32	width;
	S32	height;
} CAM_UI_IMG;


typedef struct {
	U16		image_id;
	S32		image_width;
	S32		image_height;
	S32		tile_offset_x;
	S32		tile_offset_y;	
	S32		tile_width;
	S32		tile_height;	
} CAM_UI_IMG_TILE;


typedef struct {
	CAM_UI_IMG			left;
	CAM_UI_IMG			right;	
	CAM_UI_IMG_TILE	center;

	S32					offset_x;
	S32					offset_y;	
	S32					width;
	S32					height;
} CAM_UI_TITLE;


typedef struct {
	CAM_UI_IMG			left;
	CAM_UI_IMG			right;	
	CAM_UI_IMG			chat_mark;
	CAM_UI_IMG_TILE	center;
	CAM_UI_IMG			up_arrow;
	CAM_UI_IMG			up_arrow_sel;
	CAM_UI_IMG			up_arrow_dis;
	CAM_UI_IMG			down_arrow;
	CAM_UI_IMG			down_arrow_sel;
	CAM_UI_IMG			down_arrow_dis;

	S32					offset_x;
	S32					offset_y;	
	S32					width;
	S32					height;
} CAM_UI_BOTTOM;


typedef struct {
	CAM_UI_IMG_TILE	left;
	CAM_UI_IMG_TILE	right;	
	CAM_UI_IMG_TILE	center;	
	
	S32					offset_x;
	S32					offset_y;	
	S32					width;
	S32					height;
} CAM_UI_BODY;

typedef struct {
	S32	offset_x;
	S32	offset_y;
	S32	width;
	S32	height;
	BOOL	is_press;
} camera_ui_touch_struct;


typedef struct { 
	gdi_handle							base_layer_handle;
	gdi_handle							add_frame_layer_handle;
	gdi_handle							tileshot_preview_layer_handle;
	gdi_handle							tileshot_result_layer_handle;	
	gdi_handle							osd_layer_handle;		
	gdi_handle							sublcd_base_layer_handle;
	gdi_handle							preview_layer_handle;
	MDI_RESULT							last_error;
	
	camera_setting_struct			setting;

	camera_osd_layeout_struct 		*active_camera_osd_ptr;
	U8										osd_rotate;
	U8										previous_osd_rotate;

	U8										lcm_rotate;
	
	U32									UI_device_height;
	U32									UI_device_width;
	
	U32									osd_UI_device_width;
	U32									osd_UI_device_height;
	U32									base_UI_device_width;
	U32									base_UI_device_height;
	
	camera_capture_path				capture_path;
	U16									previous_image_size;
	U16									shot_mode_shutter_pri_str_id[5];	
	U16									shot_mode_aperture_pri_str_id[5];	

	S32									capture_countdown;
	PS8									storage_filepath;
	PU8									osd_layer_buf_ptr;
	PU8									tileshot_result_buf_ptr;
	
	U16									preview_width;
	U16									preview_height;
	U16									image_width;
	U16									image_height;
	U16									zoom_limit;
	U16									image_size_before_burst;
	U16									app_state;
	U16									app_prev_state;
	U16									app_next_state;
	S16									cont_shot_count;
	S16									cont_shot_total_count;	
	PU8									capture_buf_ptr;
	U32									capture_size;

	U8										remain_percent;	
	BOOL									is_storage_path_changed;
	BOOL									is_lsk_pressed;
	BOOL									is_rsk_pressed;	
	BOOL									is_up_arrow_pressed;
	BOOL									is_down_arrow_pressed;
	BOOL									is_capturing;
	BOOL									is_clam_close;
	BOOL									is_force_self_capture;
	BOOL									is_setting_loaded;
	BOOL									is_mainlcd_resource;
	BOOL									is_sublcd_resource;
	BOOL									is_fullscreen;
	BOOL									is_sublcd_display;
	BOOL									is_tileshot;
	BOOL									is_dis_clam_open;
	BOOL									is_cat65_show;
	BOOL									is_hide_osd;
	BOOL									is_multi_sound_open; 

	S8										hint1_buf[CAMERA_HINT_BUF_SIZE];
	S8										hint2_buf[CAMERA_HINT_BUF_SIZE];
	U16									fade_time;
	U16									image_size_str_id[5];	/* LL, L, M, S, SS */
	U16									image_size_img_id[5];	/* LL, L, M, S, SS */	
	U16									selected_effect;
	U16									selected_shot_mode;	
	U8										tile_cur_num;
	mdi_handle							multi_sound_dummy_bg;
	mdi_handle 							multi_sound_capture0;
	mdi_handle 							multi_sound_capture1;
	mdi_handle 							multi_sound_capture2;	
	U16									multi_sound_cap_idx;

#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST__)
	mdi_camera_jpegs_struct			burst_jpg_data;
	U16									burst_save_index;
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */


#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
	U16									iconbar_item[CAMERA_ICONBAR_ITEM_COUNT];
	S32 									iconbar_start_idx;
	S32 									iconbar_selected_idx;
	S32									iconbar_item_count;
	S32									iconbar_item_per_page;	
	S32									iconbar_item_width;
	S32									iconbar_arrow_width;
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */

#ifdef __CAMERA_FEATURE_OSD_MENU__
	S32									iconlist_offset_y;
	BOOL									is_need_clear_all;
	BOOL									is_left_arrow_pressed;
	BOOL									is_right_arrow_pressed;	
	BOOL									is_iconlist_moving;
	BOOL									is_iconlist_init;
	BOOL									is_poplist_init;
	BOOL									is_card_present;
	BOOL									is_touch_pen_down;
	UI_string_type						list_str_ptr[32];
	CAM_UI_IMG							*last_touch_img_ptr;
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


	U16									touch_object;
	camera_ui_touch_struct			touch_lsk;
	camera_ui_touch_struct			touch_rsk;

	camera_ui_touch_struct			touch_effect;
	camera_ui_touch_struct			touch_wb;
	camera_ui_touch_struct			touch_flash;
	camera_ui_touch_struct			touch_led_highlight;
	camera_ui_touch_struct			touch_night;
	camera_ui_touch_struct			touch_image_size;
	camera_ui_touch_struct			touch_image_qty;
	camera_ui_touch_struct			touch_cont_shot;
	camera_ui_touch_struct			touch_delay_timer;	
	camera_ui_touch_struct			touch_zoom_inc;
	camera_ui_touch_struct			touch_zoom_dec;
	camera_ui_touch_struct			touch_ev_inc;
	camera_ui_touch_struct			touch_ev_dec;	
	camera_ui_touch_struct			touch_capture;	
	camera_ui_touch_struct			touch_shot_mode;
	camera_ui_touch_struct			touch_iso;	
	camera_ui_touch_struct			touch_ae_meter;	
	camera_ui_touch_struct			touch_storage;		
#if defined(__MMI_MAINLCD_220X176__)
	camera_ui_touch_struct			touch_contrast;
#endif

#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
	U16									iconbar_item[CAMERA_ICONBAR_ITEM_COUNT];
	S32 									iconbar_start_idx;
	S32 									iconbar_selected_idx;
	S32									iconbar_item_count;
	S32									iconbar_item_per_page;	
	S32									iconbar_item_width;
	S32									iconbar_arrow_width;	
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */


#ifdef __MMI_SCREEN_SNAPSHOT__
	BOOL									is_screen_snapshot;
#endif /* __MMI_SCREEN_SNAPSHOT__ */

#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    S32 app_timer_count;
    S32 delete_confirm_timer_count;
#endif

} camera_context_struct;


typedef struct {
	CAM_UI_TITLE	title;
	CAM_UI_BOTTOM	bottom;
	CAM_UI_BODY		body;
	
	S32				offset_x;
	S32				offset_y;	
	S32				width;
	S32				height;
	
	U16				highlighted_index;
	U16				page_start_index;
	U16				item_per_page;	
	U16				total_item_count;
	gdi_color		highlight_color;
	gdi_color		bg_color;	
	void				(*highlight_hdlr)(S32);
	
} CAM_UI_POPLIST;


typedef struct {
	CAM_UI_IMG		img_l_arrow;
	CAM_UI_IMG		img_l_arrow_sel;
	CAM_UI_IMG		img_l_arrow_dis;	
	CAM_UI_IMG		img_r_arrow;
	CAM_UI_IMG		img_r_arrow_sel;
	CAM_UI_IMG		img_r_arrow_dis;	
	CAM_UI_IMG		img_hide_tap;	

	CAM_UI_IMG		img_icon[15];					 
	CAM_UI_IMG		img_icon_sel[15];				 	
	BOOL				is_image_icon_update[15];

	S32				total_icon_count;
	S32				icon_per_page;	
	S32				highlighted_index;
	S32				page_start_index;				/* start icon index of this page */
	
	S32				offset_x;
	S32				offset_y;	
	S32				width;
	S32				height;
} CAM_UI_ICONLIST;


typedef enum
{
	CAMERA_TOUCH_RSK,
	CAMERA_TOUCH_LSK,
	CAMERA_TOUCH_EFFECT,
	CAMERA_TOUCH_WB,
	CAMERA_TOUCH_FLASH,
	CAMERA_TOUCH_LED_HIGHLIGHT,
	CAMERA_TOUCH_NIGHT,
	CAMERA_TOUCH_IMAGE_SIZE,
	CAMERA_TOUCH_IMAGE_QTY,
	CAMERA_TOUCH_CONT_SHOT,
	CAMERA_TOUCH_DELAY_TIMER,
	CAMERA_TOUCH_STORAGE,
	CAMERA_TOUCH_ZOOM_INC,	/* zoom in */
	CAMERA_TOUCH_ZOOM_DEC,	/* zoom out */
	CAMERA_TOUCH_EV_INC,
	CAMERA_TOUCH_EV_DEC,	
	CAMERA_TOUCH_CAPTURE,
	CAMERA_TOUCH_SHOT_MODE,
	CAMERA_TOUCH_ISO,
	CAMERA_TOUCH_AE_METER,
#if defined(__MMI_MAINLCD_220X176__)	
	CAMERA_TOUCH_CONTRAST,
#endif	
	CAMERA_TOUCH_NONE
}camera_touch_obj_enum;

/* camera application state */
typedef enum{
	CAMERA_STATE_EXIT = 0,						/* 0 */
		
	/* main lcd */
	CAMERA_STATE_PREVIEW,						/* 1 */
	CAMERA_STATE_COUNTDOWN,						/* 2 */
	CAMERA_STATE_CONTSHOT,						/* 3 */
	CAMERA_STATE_BURSTSHOT,						/* 4 */
	CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM,	/* 5 */	
	CAMERA_STATE_BURSTSHOT_SAVING,			/* 6 */
	CAMERA_STATE_SAVE_DONE,						/* 7 */
	CAMERA_STATE_SAVE_CONFIRM,					/* 8 */
	CAMERA_STATE_OSD_MENU,						/* 9 */

	/* toggle popup */
	CAMERA_STATE_TOGGLE_TO_SUB,				/* 10 */
	CAMERA_STATE_TOGGLE_TO_MAIN,				/* 11 */	

	/* sub lcd */
	CAMERA_STATE_SUB_PREVIEW,					/* 12 */
	CAMERA_STATE_SUB_COUNTDOWN,				/* 13 */
	CAMERA_STATE_SUB_CONTSHOT,					/* 14 */
	CAMERA_STATE_SUB_BURSTSHOT,				/* 15 */	
	CAMERA_STATE_SUB_BURSTSHOT_SAVING		/* 16 */

} camera_state_enum;



/****************************************************************************
* Extern Global Variable                                                            
*****************************************************************************/
extern camera_context_struct	g_camera_cntx;


/****************************************************************************
* Extern Global Function                                                            
*****************************************************************************/
extern void mmi_camera_init_app(void);
extern void mmi_camera_entry_app_screen(void);

extern void mmi_camera_restore_setting(void);
extern void mmi_camera_load_setting(void);
extern void mmi_camera_store_setting(void);
extern void mmi_camera_preview_update_capture_size(void);

extern void mmi_camera_check_osd_orientation(void);
extern void mmi_camera_check_sensor_orientation(void);
extern void mmi_camera_check_tileshot_orientation(void);

extern BOOL	mmi_camera_get_storage_file_path(PS8 filepath);
extern BOOL	mmi_camera_check_and_restore_valid_drv(void);

extern void mmi_camera_capture_play_sound(void);

extern BOOL mmi_camera_get_storage_file_path(PS8 filepath);
extern S32	mmi_camera_create_file_dir(PS8 filepath);

#ifdef __MMI_SUBLCD__
extern void mmi_camera_sublcd_draw_icon(void);
#endif /* __MMI_SUBLCD__ */
#if 0
extern BOOL mmi_get_Camera_running_state(void);
#endif



#endif /* __MMI_CAMERA__ */
#endif /* _CAMERA_DATATYPE_H_ */


