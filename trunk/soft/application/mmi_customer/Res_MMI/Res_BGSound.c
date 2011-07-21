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
*	Res_BGSound.c
*
* Project:
* --------
*
* Description:
* ------------
*   Background sound resource link
*
* Author:
* -------
*
 *============================================================================
 *             HISTORY
 *------------------------------------------------------------------------------
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 *============================================================================
 ****************************************************************************/
#include "cs_types.h"
#include "custdatares.h"
#include "mmi_features.h" 

#include "stdc.h"
#include "customcfg.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"

#ifdef __MMI_BG_SOUND_EFFECT__
#include "bgsounddef.h"

void PopulateBGSoundRes(void)
{
	/*Main setup menu*/
	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_CALL_SETUP,MENU_SETTING_SOUND_EFFECT,3,
		MENU_ID_BGSND_CALL_SETUP_STATUS,
		MENU_ID_BGSND_CALL_SETUP_SETTING,
		MENU_ID_BGSND_CALL_SETUP_LIST,
		SHOW, NONMOVEABLE, DISP_LIST, STR_ID_BGSND_BG_SOUND,IMG_ID_BGSND_APP));

	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_CALL_SETUP_STATUS, MENU_ID_BGSND_CALL_SETUP, 0,
		SHOW, NONMOVEABLE, 0, STR_ID_BGSND_MO_STATUS, 0 ));
	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_CALL_SETUP_SETTING, MENU_ID_BGSND_CALL_SETUP, 0,
		SHOW, NONMOVEABLE, 0, STR_ID_BGSND_PLAYBACK_SETUP, 0 ));
	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_CALL_SETUP_LIST, MENU_ID_BGSND_CALL_SETUP, 0,
		SHOW, NONMOVEABLE, 0, STR_ID_BGSND_EFFECT_LIST, 0 ));

	/*External Effect Option Menu*/
	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_EXT_EFFECT_OPTION, 0, 3,
		MENU_ID_BGSND_EXT_EFFECT_SELECT,
		MENU_ID_BGSND_EXT_EFFECT_EDIT,
		MENU_ID_BGSND_EXT_EFFECT_DELETE,
		SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_OPTIONS,0));
		
	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_EXT_EFFECT_SELECT, MENU_ID_BGSND_EXT_EFFECT_OPTION, 0,
		SHOW, NONMOVEABLE, 0, STR_GLOBAL_SELECT, 0 ));
	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_EXT_EFFECT_EDIT, MENU_ID_BGSND_EXT_EFFECT_OPTION, 0,
		SHOW, NONMOVEABLE, 0, STR_GLOBAL_EDIT, 0 ));		
	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_EXT_EFFECT_DELETE, MENU_ID_BGSND_EXT_EFFECT_OPTION, 0,
		SHOW, NONMOVEABLE, 0, STR_GLOBAL_DELETE, 0 ));

	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_EXT_EFFECT_OPTION_ADD, 0, 1,
		MENU_ID_BGSND_EXT_EFFECT_ADD,
		SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_OPTIONS,0));

	ADD_APPLICATION_MENUITEM((MENU_ID_BGSND_EXT_EFFECT_ADD, MENU_ID_BGSND_EXT_EFFECT_OPTION_ADD, 0,
		SHOW, NONMOVEABLE, 0, STR_GLOBAL_ADD, 0 ));


	/*String*/
	ADD_APPLICATION_STRING2(STR_ID_BGSND_BG_SOUND,			"Calling Sound Effect",		"BG Sound Setting" );
	ADD_APPLICATION_STRING2(STR_ID_BGSND_STATUS,				"Status",						"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_MO_STATUS,			"Select when make call",		"BG Sound Setting" );
	ADD_APPLICATION_STRING2(STR_ID_BGSND_PLAYBACK_SETUP,	"Playback Setting",			"BG Sound Setting" );
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_LIST,			"Effect List",					"BG Sound Setting" );	

	ADD_APPLICATION_STRING2(STR_ID_BGSND_ONCE,					"Once",						"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_INTERVAL,				"Interval",						"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_CONTINUE,				"Continue",					"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_INTERVAL_CAP,		"Interval [1 ~ 120 sec]",		"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_INTERVAL_CAP_SHORT,		"Interval",		"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_VOLUME_CAP,			"Volume [0(mute) ~ 7]",	"BG Sound Setting" );
	ADD_APPLICATION_STRING2(STR_ID_BGSND_VOLUME_CAP_SHORT,	"Volume",	"BG Sound Setting" );			
	ADD_APPLICATION_STRING2(STR_ID_BGSND_INVALID_INTERVAL,	"Invalid Interval (1 ~ 120 sec)",		"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_INVALID_VOLUME,		"Invalid Volume (0 ~ 7)",	"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_INVALID_FILE,			"Invalid Background Sound Format",	"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_DEL_FILE_CONFIRM,	"Delete File Link",	"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_OPEN_FILE_FAIL,		"Open File Fail",	"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_PLAY_FAIL,				"Play Effect Fail",	"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_LIST_FULL,				"Effect List Full",	"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_FILE_DUPLICATED,				"File Duplicated",	"BG Sound Setting" );
	ADD_APPLICATION_STRING2(STR_ID_BGSND_CTM_MODE_ON,				"Can not use in text phone mode",	"BG Sound Setting" );

	/*Sound effect list*/
	ADD_APPLICATION_STRING2(STR_ID_BGSND_NO_EFFECT,	"No Effect",		"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_1,	"Effect01",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_2,	"Effect02",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_3,	"Effect03",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_4,	"Effect04",			"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_5,	"Effect05",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_6,	"Effect06",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_7,	"Effect07",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_8,	"Effect08",			"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_9,	"Effect09",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_10,	"Effect10",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_11,	"Effect11",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_12,	"Effect12",			"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_13,	"Effect13",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_14,	"Effect14",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_15,	"Effect15",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_16,	"Effect16",			"BG Sound Setting" );		
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_17,	"Effect17",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_18,	"Effect18",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_19,	"Effect19",			"BG Sound Setting" );	
	ADD_APPLICATION_STRING2(STR_ID_BGSND_EFFECT_20,	"Effect20",			"BG Sound Setting" );		


	/*Imaeg*/
	ADD_APPLICATION_IMAGE2( IMG_ID_BGSND_APP, CUST_IMG_PATH"/MainLCD/Submenu/Settings/SoundEffect/ST_BGplay.pbm", "BG Sound Image" );

	/*Audio*/
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT1, CUST_ADO_PATH"/BGSound/Effect01.wav", "BG Sound Audio Res");
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT2, CUST_ADO_PATH"/BGSound/Effect02.wav", "BG Sound Audio Res");
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT3, CUST_ADO_PATH"/BGSound/Effect03.wav", "BG Sound Audio Res");
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT4, CUST_ADO_PATH"/BGSound/Effect04.wav", "BG Sound Audio Res");
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT5, CUST_ADO_PATH"/BGSound/Effect05.wav", "BG Sound Audio Res");
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT6, CUST_ADO_PATH"/BGSound/Effect06.wav", "BG Sound Audio Res");
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT7, CUST_ADO_PATH"/BGSound/Effect07.wav", "BG Sound Audio Res");
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT8, CUST_ADO_PATH"/BGSound/Effect08.wav", "BG Sound Audio Res");
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT9, CUST_ADO_PATH"/BGSound/Effect09.wav", "BG Sound Audio Res");
	ADD_APPLICATION_AUDIO2(AUD_ID_BGSND_EFFECT10, CUST_ADO_PATH"/BGSound/Effect10.wav", "BG Sound Audio Res");
}

#endif // __MMI_BG_SOUND_EFFECT__
#endif // DEVELOPER_BUILD_FIRST_PASS

