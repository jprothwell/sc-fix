/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
*	Res_FMRadio.c
*
* Project:
* --------
 
*
* Description:
* ------------
*   FM Radio resource link
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log$
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/
#include "cs_types.h"
#include "custdatares.h"
#include "mmi_features.h" 

#include "stdc.h"
#include "customcfg.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#include "funandgamesdefs.h"
#include "factorymodedef.h"
#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"

#ifdef __MMI_FM_RADIO__
#include "fmradiodef.h"


#define __MMI_FM_RADIO_LOUD_SPEAK__ 

/////////////////////////////////////////////////////////////////////////
void PopulateFMRadioRes(void)
{
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_TITLE, "FM Radio", "FM Radio String" );
	//ADD_APPLICATION_STRING2( STR_ID_FMRDO_MENU, "FM Radio", "FM Radio String" );  
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_CHANNEL_LIST, "Channel List", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_MANUAL_INPUT, "Manual Input", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_PRESET_AUTO_SEARCH, "Preset Auto Search", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_SETTINGS, "Settings", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_BACKGROUND_PLAY, "Background Play", "FM Radio String" );
#ifndef __MMI_FM_SLIM_SKIN__
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_SKIN, "Skin", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_SKIN_1, "Skin 1", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_SKIN_2, "Skin 2", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_SKIN_3, "Skin 3", "FM Radio String" );
#endif    
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_PLAY, "Play", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_EDIT, "Edit", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_EMPTY, "<Empty>", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_PRESET_LIST, "Preset List", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_SEARCHING, "Searching", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_CHANNEL_NAME, "Channel Name", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_FREQUENCY, "Frequency", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_VALID_FM_BANDWIDTH, "Frequency: 87.5 ~ 108.0", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_VALID_FM_DECIMAL, "One Digit Decimal Only", "FM Radio String" );	
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_NEW_CHANNEL, "New Channel", "FM Radio String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_POWER_ON_FIRST, "Please Power On First", "FM Radio String" );
#ifdef __MMI_FM_RADIO_ON_SUBLCD__
//del by reslim v0.1     ADD_APPLICATION_STRING2( STR_ID_FM_RADIO_ON_SUBLCD_MODE_SELECT, "MODE", "FM Radio String" );
//del by reslim v0.1    ADD_APPLICATION_STRING2( STR_ID_FM_RADIO_ON_SUBLCD_STEP_MODE, "STEP MODE", "FM Radio String" );
//del by reslim v0.1    ADD_APPLICATION_STRING2( STR_ID_FM_RADIO_ON_SUBLCD_CHANNEL_MODE, "CHANNEL MODE", "FM Radio String" );
//del by reslim v0.1    ADD_APPLICATION_STRING2( STR_ID_FM_RADIO_ON_SUBLCD_SEARCH_MODE, "SEARCH MODE", "FM Radio String" );
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_FM_RADIO_ON_SUBLCD_BACK_TO_MAINMENU,"Exit","FM Radio String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_FM_RADIO_ON_SUBLCD_CHANNEL_NAME_CH,"CH","FM Radio String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_FM_RADIO_ON_SUBLCD_CHANNEL_NAME_RADIO,"Radio","FM Radio String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_FM_RADIO_ON_SUBLCD_CHANNEL_NAME_SEARCH,"Search","FM Radio String");
#endif  /*__MMI_FM_RADIO_ON_SUBLCD__*/
#ifdef __MMI_FM_RADIO_BIND_EARPHONE__
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_PLEASE_PLUG_IN_EARPHONE, "Please Plug in Earphone", "FM Radio String" );
#endif
#if defined(__MMI_FM_RADIO_LOUD_SPEAK__)
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_AUDIO_OUT,	"Loud Speaker",	"FM Radio String" );	
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_RECORD,	"Record",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_APPEND,	"Append",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_PAUSE,	"Pause",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_STOP,	"Stop",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_RESUME,	"Resume",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_FILE_LIST,	"File list",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_RECORD_FORMAT,	"Record Format",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_RECORD_STORAGE,	"Record Storage",	"FM Radio Record String" );

	ADD_APPLICATION_STRING2( STR_ID_FMRDO_FORMAT_AMR,	"AMR",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_FORMAT_WAV,	"WAV",	"FM Radio Record String" );
//del by reslim v0.1	ADD_APPLICATION_STRING2( STR_ID_FMRDO_FORMAT_AWB,	"AWB",	"FM Radio Record String" );
//	ADD_APPLICATION_STRING2( STR_ID_FMRDO_STORAGE_PHONE,	"Phone",	"FM Radio Record String" );
//	ADD_APPLICATION_STRING2( STR_ID_FMRDO_STORAGE_CARD,	"Card",	"FM Radio Record String" );

	ADD_APPLICATION_STRING2( STR_ID_FMRDO_BAD_FORMAT,	"Bad Format",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_INVALID_FORMAT,	"Invalid Format",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_DEVICE_BUSY,	"Device Busy",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_OPEN_FILE_ERROR,	"Can't Open",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_NO_CARD,	"Drive Not Found",	"FM Radio Record String" );
	ADD_APPLICATION_STRING2( STR_ID_FMRDO_WRITE_PROTECTION_ERROR,	"Write Protection!",	"FM Radio Record String" );	
//del by reslim v0.1	ADD_APPLICATION_STRING2( STR_ID_FMRDO_RECORD_IN_BT_SCO,	"Can not record in bluetooth profile",	"FM Radio Record String" );	
#endif

#if defined __PRODUCT_QPAD__
	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_MAIN, IDLE_SCREEN_MENU_ID, 0,
		SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, 0, MAIN_MENU_FM_RADIO_TEXT,MAIN_MENU_FM_RADIO_ICON));
#else
/* level 1 */
	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_MAIN, MAIN_MENU_MULTIMEDIA_MENUID, 0,
		SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, 0, STR_ID_FMRDO_TITLE, IMG_ID_FMRDO_APP ));
#endif

/* level 2 */
	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_MAIN_OPTION, MENU_ID_FMRDO_MAIN, FMRDO_MAIN_OPTION_TOTAL,
		MENU_ID_FMRDO_CHANNEL_LIST,
		MENU_ID_FMRDO_MANUAL_INPUT,
		MENU_ID_FMRDO_PRESET_AUTO_SEARCH,
		MENU_ID_FMRDO_SETTINGS,
#if defined(__MMI_FM_RADIO_RECORD__)
		MENU_ID_FMRDO_RECORD,
		MENU_ID_FMRDO_APPEND,
		MENU_ID_FMRDO_FILE_LIST,
#endif
		SHOW, NONMOVEABLE, 0, STR_GLOBAL_OPTIONS, IMG_ID_FMRDO_APP ));

#if defined(__MMI_FM_RADIO_RECORD__)		
	ADD_APPLICATION_MENUITEM(  (MENU_ID_FMRDO_EDIT_FILENAME_OPTIONS,	0, 	2,  
		MENU_ID_FMRDO_EDIT_FILENAME_SAVE,
		MENU_ID_FMRDO_EDIT_FILENAME_INPUT_METHOD,
		SHOW, NONMOVEABLE, DISP_LIST,
		STR_GLOBAL_OPTIONS, IMG_ID_FMRDO_APP));
#endif

/* level 3 */
	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_CHANNEL_LIST, MENU_ID_FMRDO_MAIN_OPTION, 0,
		SHOW, MOVEABLEWITHINPARENT, 0, STR_ID_FMRDO_CHANNEL_LIST, IMG_GLOBAL_L1 ));
		
	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_MANUAL_INPUT, MENU_ID_FMRDO_MAIN_OPTION, 0,
		SHOW, MOVEABLEWITHINPARENT, 0, STR_ID_FMRDO_MANUAL_INPUT, IMG_GLOBAL_L2 ));
		
	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_PRESET_AUTO_SEARCH, MENU_ID_FMRDO_MAIN_OPTION, 0,
		SHOW, MOVEABLEWITHINPARENT, 0, STR_ID_FMRDO_PRESET_AUTO_SEARCH, IMG_GLOBAL_L3 ));
		
	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_SETTINGS, MENU_ID_FMRDO_MAIN_OPTION, 0,
		SHOW, MOVEABLEWITHINPARENT, 0, STR_ID_FMRDO_SETTINGS, IMG_GLOBAL_L4 ));

#if defined(__MMI_FM_RADIO_RECORD__)
	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_RECORD, MENU_ID_FMRDO_MAIN_OPTION, 0,
		SHOW, MOVEABLEWITHINPARENT, 0, STR_ID_FMRDO_RECORD, IMG_GLOBAL_L5 ));

	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_APPEND, MENU_ID_FMRDO_MAIN_OPTION, 0,
		SHOW, MOVEABLEWITHINPARENT, 0, STR_ID_FMRDO_APPEND, IMG_GLOBAL_L6 ));

	ADD_APPLICATION_MENUITEM((MENU_ID_FMRDO_FILE_LIST, MENU_ID_FMRDO_MAIN_OPTION, 0,
		SHOW, MOVEABLEWITHINPARENT, 0, STR_ID_FMRDO_FILE_LIST, IMG_GLOBAL_L7 ));

	ADD_APPLICATION_MENUITEM(  (MENU_ID_FMRDO_EDIT_FILENAME_SAVE,	MENU_ID_FMRDO_EDIT_FILENAME_OPTIONS, 0, 
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_SAVE, IMG_ID_FMRDO_APP));

	ADD_APPLICATION_MENUITEM( (MENU_ID_FMRDO_EDIT_FILENAME_INPUT_METHOD,	MENU_ID_FMRDO_EDIT_FILENAME_OPTIONS, 0, 
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST,	STR_GLOBAL_INPUT_METHOD, IMG_ID_FMRDO_APP));
#endif
		
#ifdef __MMI_MAINLCD_220X176__
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_APP, CUST_IMG_PATH"/MainLCD/FMRadio/SB_FMradio.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_BACKGROUND_PLAY, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_BackgroundPlay.gif", "FM Radio Image" );
	//ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_SKIN.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_CHANNEL_NAME, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_ChannelName.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_FREQUENCY, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_Frequency.gif", "FM Radio Image" );

#if defined(__MMI_FM_RADIO_LOUD_SPEAK__)
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_AUDIO_OUT, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_LoudSpeaker.gif", "FM Radio Image" );
#endif

#if defined(__MMI_FM_RADIO_RECORD__)
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_RECORD_FORMAT, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_Format.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_RECORD_STORAGE, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_Storage.gif", "FM Radio Image" );	
#endif	

	/* Skin 1 */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    /* Load all image in one skin at once in ENFB project */
    SetENFBAssociatedID();
#endif	
	/*Background*/
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_BACKGROUND, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_background.bmp", "FM Radio Image" );

	/*FM Icon*/
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_FM, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_fm.bmp", "FM Radio Image" );

	/*Tuner Scale*/
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_TUNER_SCALE, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_tuner_scale.bmp", "FM Radio Image" );

	/*Volume*/
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_L_0, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_left_0.bmp", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_L_1, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_left_1.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_L_2, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_left_2.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_L_3, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_left_3.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_L_4, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_left_4.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_L_5, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_left_5.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_L_6, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_left_6.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_L_7, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_left_7.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_R_0, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_right_0.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_R_1, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_right_1.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_R_2, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_right_2.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_R_3, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_right_3.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_R_4, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_right_4.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_R_5, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_right_5.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_R_6, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_right_6.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_R_7, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_volume_right_7.gif", "FM Radio Image" );
	
	/*Tuner Scale Indicator*/
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_tuner_scale_indicator.bmp", "FM Radio Image" );

	/*Power On/Off Button*/
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_power_on_up.bmp", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_power_on_down.bmp", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_UP, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_power_off_up.bmp", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/qpad/Skin1/FMR_power_off_down.bmp", "FM Radio Image" );

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    ResetENFBAssociatedID();
#endif

#else /*__PRODUCT_QPAD__*/
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_APP, CUST_IMG_PATH"/MainLCD/FMRadio/SB_FMradio.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_BACKGROUND_PLAY, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_BackgroundPlay.gif", "FM Radio Image" );
	//ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_SKIN.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_CHANNEL_NAME, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_ChannelName.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_FREQUENCY, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_Frequency.gif", "FM Radio Image" );

#if defined(__MMI_FM_RADIO_LOUD_SPEAK__)
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_AUDIO_OUT, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_LoudSpeaker.gif", "FM Radio Image" );
#endif

#if defined(__MMI_FM_RADIO_RECORD__)
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_RECORD_FORMAT, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_Format.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_RECORD_STORAGE, CUST_IMG_PATH"/MainLCD/FMRadio/FMR_Storage.gif", "FM Radio Image" );	
#endif	

	/* Skin 1 */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    /* Load all image in one skin at once in ENFB project */
    SetENFBAssociatedID();
#endif	
#if defined(__MMI_FM_RADIO_RECORD__)	
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_BACKGROUND, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_background_rec.jpg", "FM Radio Image" );
#else
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_BACKGROUND, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_background.jpg", "FM Radio Image" );
#endif	
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_TUNER_SCALE, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_tuner_scale.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_volume.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_tuner_scale_indicator.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_power_on_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_power_on_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_power_off_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_power_off_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_search_on_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_search_on_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_search_off_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_search_off_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_right_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_right_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_left_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_left_down.gif", "FM Radio Image" );
#ifdef __MMI_FM_RADIO_ANIMATION__
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_ANIMATION, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_animation.gif", "FM Radio Image" );
#endif
#ifdef __MMI_TOUCH_SCREEN__
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_INC_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_volume_inc_up.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_INC_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_volume_inc_down.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_DEC_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_volume_dec_up.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_VOLUME_DEC_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_volume_dec_down.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_SETTING_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_setting_up.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_SETTING_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_setting_down.pbm", "FM Radio Image" );
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_REC, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_rec.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_REC_PAUSE, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_rec_pause.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_0, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_0.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_1, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_1.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_2, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_2.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_3, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_3.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_4, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_4.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_5, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_5.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_6, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_6.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_7, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_7.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_8, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_8.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_9, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_9.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_1_NUM_COL, CUST_IMG_PATH"/MainLCD/FMRadio/Skin1/FMR_num_col.gif", "FM Radio Image" );
#endif
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    ResetENFBAssociatedID();
#endif

	/* Skin 2 */
#ifndef __MMI_FM_SLIM_SKIN__
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    /* Load all image in one skin at once in ENFB project */
    SetENFBAssociatedID();
#endif	
#if defined(__MMI_FM_RADIO_RECORD__)	
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_BACKGROUND, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_background_rec.jpg", "FM Radio Image" );
#else
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_BACKGROUND, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_background.gif", "FM Radio Image" );
#endif	
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_TUNER_SCALE, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_tuner_scale.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_VOLUME, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_volume.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_TUNER_SCALE_INDICATOR, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_tuner_scale_indicator.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_power_on_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_power_on_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_power_off_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_power_off_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_search_on_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_search_on_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_search_off_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_search_off_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_right_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_right_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_left_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_left_down.gif", "FM Radio Image" );
#ifdef __MMI_FM_RADIO_ANIMATION__
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_ANIMATION, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_animation.gif", "FM Radio Image" );
#endif
#ifdef __MMI_TOUCH_SCREEN__
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_VOLUME_INC_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_volume_inc_up.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_VOLUME_INC_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_volume_inc_down.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_VOLUME_DEC_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_volume_dec_up.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_VOLUME_DEC_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_volume_dec_down.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_SETTING_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_setting_up.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_SETTING_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_setting_down.pbm", "FM Radio Image" );
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_REC, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_rec.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_REC_PAUSE, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_rec_pause.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_0, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_0.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_1, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_1.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_2, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_2.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_3, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_3.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_4, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_4.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_5, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_5.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_6, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_6.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_7, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_7.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_8, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_8.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_9, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_9.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_2_NUM_COL, CUST_IMG_PATH"/MainLCD/FMRadio/Skin2/FMR_num_col.gif", "FM Radio Image" );
#endif
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    ResetENFBAssociatedID();
#endif

	/* Skin 3 */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    /* Load all image in one skin at once in ENFB project */
    SetENFBAssociatedID();
#endif	
#if defined(__MMI_FM_RADIO_RECORD__)
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_BACKGROUND, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_background_rec.jpg", "FM Radio Image" );
#else
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_BACKGROUND, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_background.gif", "FM Radio Image" );
#endif	
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_TUNER_SCALE, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_tuner_scale.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_VOLUME, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_volume.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_TUNER_SCALE_INDICATOR, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_tuner_scale_indicator.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_power_on_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_power_on_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_power_off_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_power_off_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_search_on_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_search_on_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_search_off_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_search_off_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_right_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_right_down.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_left_up.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_left_down.gif", "FM Radio Image" );
#ifdef __MMI_FM_RADIO_ANIMATION__
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_ANIMATION, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_animation.gif", "FM Radio Image" );
#endif
#ifdef __MMI_TOUCH_SCREEN__
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_VOLUME_INC_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_volume_inc_up.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_VOLUME_INC_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_volume_inc_down.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_VOLUME_DEC_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_volume_dec_up.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_VOLUME_DEC_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_volume_dec_down.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_SETTING_UP, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_setting_up.pbm", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_SETTING_DOWN, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_setting_down.pbm", "FM Radio Image" );
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_REC, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_rec.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_REC_PAUSE, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_rec_pause.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_0, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_0.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_1, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_1.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_2, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_2.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_3, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_3.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_4, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_4.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_5, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_5.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_6, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_6.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_7, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_7.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_8, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_8.gif", "FM Radio Image" );
//del by reslim v0.1	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_9, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_9.gif", "FM Radio Image" );
	ADD_APPLICATION_IMAGE2( IMG_ID_FMRDO_SKIN_3_NUM_COL, CUST_IMG_PATH"/MainLCD/FMRadio/Skin3/FMR_num_col.gif", "FM Radio Image" );
#endif
#ifdef __MMI_FM_RADIO_ON_SUBLCD__
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_BACKGROUND,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_background.jpg","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_VOLUME,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_volume.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_TUNER_SCALE,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_tuner_scale.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_TUNER_SCALE_INDICATOR,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_tuner_scale_indicator.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_POWER_BUTTON_ON_UP,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_power_on_up.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_POWER_BUTTON_ON_DOWN,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_power_on_down.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_POWER_BUTTON_OFF_UP,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_power_off_up.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_POWER_BUTTON_OFF_DOWN,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_power_off_down.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_SEARCH_BUTTON_ON_UP,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/step.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_SEARCH_BUTTON_ON_DOWN,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/channel.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_SEARCH_BUTTON_OFF_UP,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/search.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_SEARCH_BUTTON_OFF_DOWN,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/search.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_STEP_UP_BUTTON_UP,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_right_up.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_STEP_UP_BUTTON_DOWN,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_right_down.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_STEP_DOWN_BUTTON_UP,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_left_up.gif","FM Radio Image");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_ID_FMRDO_SUB_SKIN_STEP_DOWN_BUTTON_DOWN,CUST_IMG_PATH"/SubLCD/Application/FMRadio/skin/FMR_left_down.gif","FM Radio Image");
#endif /*__MMI_FM_RADIO_ON_SUBLCD__*/
       
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    ResetENFBAssociatedID();
#endif
#endif /* #ifndef __MMI_FM_SLIM_SKIN__ */
#endif /*__PRODUCT_QPAD__*/
}
#endif // __MMI_FM_RADIO__

#endif // DEVELOPER_BUILD_FIRST_PASS


