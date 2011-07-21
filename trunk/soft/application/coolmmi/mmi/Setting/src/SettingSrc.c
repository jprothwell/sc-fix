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
 *	SettingSrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/***************** Settings Application implementation *********************/

/**************************************************************

	FILENAME	: setting.c

  	PURPOSE		: Settings application

 

	AUTHOR		: Vanita Jain

	DATE		: 

**************************************************************/
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"
#include "wgui_categories.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "settingstructs.h"
#include "settinggprots.h"
#include "simdetectiongprot.h"
#include "protocolevents.h"
#include "callfixeddial.h"
#include "barreddiallist.h"
#include "settingprofile.h"
#include "phonebookdef.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "commonscreens.h"
#include "mmi_features.h"
#include "callsetupenum.h"
#include "settingprot.h"
#include "callmanagementiddef.h"
#include "networksetupgprot.h"
#include "organizergprot.h"
#include "idleappdef.h"
#include "phonesetupgprots.h"
#include "mmi.h"    
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#undef __NEWSIMULATOR

// yuixang B	07.6.30 
#include "mmi_trace.h"
#include "cfw.h"

#include "callsetup.h"  
#include "wgui_status_icons.h"
#include "statusiconres.h"
//cong.li adds for singleSimCardWapMMS on 2009.07.08
#ifndef __MMI_MULTI_SIM__
#include "simdetectiondef.h"
#endif

extern void ShutdownSystemOperation(void);
extern UINT32 SRVAPI CFW_CfgGetSimSwitch(UINT8* n);
extern UINT32 SRVAPI CFW_CfgSetSimSwitch(UINT8 n);
// yuixang E	07.6.30 
#if defined(__MMI_PREFER_INPUT_METHOD__)
extern void HighlightInputMehtodSetting(void);
extern void SetInputMethodFromNVRAM(void);
extern void InitPreferedInputMethod(void);
#endif

#ifdef __MMI_MAINLCD_220X176__
extern void highlight_mainmenu_profiles(void);
#endif

#if defined(__MMI_APHORISM__)
/* under construction !*/
#endif


S8 gHour[MAX_LENGTH * ENCODING_LENGTH];
S8 gMinute[MAX_LENGTH * ENCODING_LENGTH];
S8 gDate[MAX_LENGTH * ENCODING_LENGTH];
S8 gMonth[MAX_LENGTH * ENCODING_LENGTH];
S8 gYear[MAX_LENGTH_YEAR * ENCODING_LENGTH];

S32 gCurrentTimeFormat = 0 ;
S32 gCurrentDateFormat = 0; 
U8 WelcomeText[(MAX_SETTING_WELCOME_TEXT +1) * ENCODING_LENGTH];
U8 gCurrentOnOff;

U8 ssc_disable_flag = MMI_FALSE;  

BOOL gSystemResetFlag = MMI_FALSE;  //2009/01/13
//xiaoke 08.12.31
#ifdef __DOUBLE_SIMCARD_211__    

#define 	TIME_FOR_RESET_AFTER_SWITCH		2000
UINT8 CURRENTSIM;
UINT8 CURRENTITEM;
void EntryHighlightSimSwitchMain(void )
	{
		U16 nStrItemList[MAX_SUB_MENUS];/* Stores the strings id of submenus returned */
		U16 nNumofItem;		 /* Stores no of children in the submenu*/
		U8* guiBuffer;		/* Buffer holding history data */
		mmi_trace(g_sw_SAT, "chenhe,The line is %d, the func is %s, the file is %s\r\n", __LINE__,__func__,__FILE__);
		
	CFW_CfgGetSimSwitch(&CURRENTSIM);
		//CURRENTSIM=0;
	EntryNewScreen(SCR_ID_SIMSWITCH_MAIN, NULL, EntryHighlightSimSwitchMain, NULL);
		/* Get current screen to gui buffer  for history purposes*/
		guiBuffer = GetCurrGuiBuffer(SCR_ID_SIMSWITCH_MAIN);	
		/* Retrieve no of child of menu item to be displayed */
		nNumofItem = GetNumOfChild(MAIN_MENU_SETTINGS_DOUBLE_SIMCARD_ID);
		/* Get attribute of menu to be displayed */
	//	nDispAttribute = GetDispAttributeOfItem(MENU8237_SCR8093_MNGCALL_MENU_WAIT);
		/* Retrieve string ids in sequence of given menu item to be displayed */
		GetSequenceStringIds(MAIN_MENU_SETTINGS_DOUBLE_SIMCARD_ID, nStrItemList);
		/* Set current parent id*/
		SetParentHandler(MAIN_MENU_SETTINGS_DOUBLE_SIMCARD_ID);
		/* Register highlight handler to be called in menu screen */
		RegisterHighlightHandler(ExecuteCurrHiliteHandler);
		/* Display Category15 Screen */
		ShowCategory15Screen(STR_SIM_SWITCH_MAIN, 0,
							  STR_GLOBAL_OK , IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  nNumofItem, nStrItemList, (U16 *)gIndexIconsImageList, 1,CURRENTSIM,guiBuffer);
		/* Register function with right softkey */
	//	mmi_trace(g_sw_SAT, "The MMI_fixed_list_menu.highlighted_item is 0x%x\r\n", MMI_fixed_list_menu.highlighted_item);
		
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
		return;
	}

void DisplayShutDownSystem(void)
{
	gSystemResetFlag = MMI_TRUE;
	ShutdownSystemOperation();
}
void EntrySwitchRsp()
	{
		UINT32 result;
		
		mmi_trace(g_sw_SAT, "The line is %d, the func is %s, the file is %s\r\n", __LINE__,__func__,__FILE__);
	//	mmi_trace(g_sw_SAT, "And the  is highlighted_item is 0x%x\r\n", MMI_fixed_list_menu.highlighted_item);
		if(CURRENTITEM!=CURRENTSIM)
			{
				result=CFW_CfgSetSimSwitch(CURRENTITEM);
				if(ERR_SUCCESS==result)
					{
			DisplayPopup((PU8)GetString(STR_SIMSWITCHDONE), IMG_GLOBAL_ACTIVATED, 0, ST_NOTIFYDURATION, ERROR_TONE);
			coolsand_UI_start_timer(TIME_FOR_RESET_AFTER_SWITCH,DisplayShutDownSystem);
					}
				else
		{
					DisplayPopup((PU8)GetString(STR_SIMSWITCHORFAIL), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
			}
			}
		else
			{
		DisplayPopup((PU8)GetString(STR_SIMSWITCHDONE), IMG_GLOBAL_ACTIVATED, 0, ST_NOTIFYDURATION, SUCCESS_TONE);
				//EntryHighlightSimSwitchMain();
		//GoBackToHistory(SCR_ID_SIMSWITCH_MAIN);
			}
	}
void SimSwitchOrNotScreen(void)
{
#if 0
	U8* guiBuffer = NULL;
#endif
	mmi_trace(g_sw_SAT, "The line is %d, the func is %s, the file is %s\r\n", __LINE__,__func__,__FILE__);
	#if 0
	EntryNewScreen(IDM_SIMSWITCHORNOT, NULL, SimSwitchOrNotScreen, NULL);
//	guiBuffer = GetCurrGuiBuffer(IDM_SIMSWITCHORNOT);	
	ClearHighlightHandler();
//	ClearAllKeyHandler();
	ShowCategory151Screen( 0, 0, 
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							(PU8)GetString(SCR_ID_SIMSWITCHORNOT), 0, 0);	//	guiBuffer
	//SetKeyHandler(DisplayIdleScreen,KEY_END,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntrySwitchRsp,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	// EntryHighlightSimSwitchMain
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);	// EntryHighlightSimSwitchMain
	
//	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		/* set the left/right arrow key function handlers */
//	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	#endif 	
	DisplayConfirm(STR_GLOBAL_YES,
						IMG_GLOBAL_YES,
						STR_GLOBAL_NO,
						IMG_GLOBAL_NO,
						get_string(STR_SIM_SWITCH_ASK),
						IMG_GLOBAL_QUESTION,
						WARNING_TONE);
	SetLeftSoftkeyFunction(EntrySwitchRsp,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	
}
 

void HighlightSwitch2First()
	{
		/* set the left/right soft key functions handlers */	
		CURRENTITEM=0;
		SetLeftSoftkeyFunction(SimSwitchOrNotScreen,KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		/* set the left/right arrow key function handlers */
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
		return;
	}
 void HighlightSwitch2Second()
	{
		CURRENTITEM=1;
		/* set the left/right soft key functions handlers */	
		SetLeftSoftkeyFunction(SimSwitchOrNotScreen,KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		/* set the left/right arrow key function handlers */
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
		return;

	}

 void HighlightSimSwitch()
	{
		/* Change left soft key icon and label */
		ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
		/* Change right soft key icon and label */
		ChangeRightSoftkey(STR_GLOBAL_BACK,0);
		/* set the left/right soft key functions handlers */	
		SetLeftSoftkeyFunction(EntryHighlightSimSwitchMain,KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		/* set the left/right arrow key function handlers */
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(EntryHighlightSimSwitchMain/*EntryScrSettingMenu*/, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);//dongwg modify
		return;
	}
/**************************************************************

	FUNCTION NAME		: InitSimSwitch(void)

  	PURPOSE				: initialise SIM switch.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitSimSwitch(void)
	{
	//xiaoke
	SetHiliteHandler(MAIN_MENU_SETTINGS_DOUBLE_SIMCARD_ID,HighlightSimSwitch);
	SetHiliteHandler(MENU_SETTINGS_DOUBLE_SIMCARD_SIMCARD1_ID,HighlightSwitch2First);
	SetHiliteHandler(MENU_SETTINGS_DOUBLE_SIMCARD_SIMCARD2_ID,HighlightSwitch2Second);
	//xiaoke
		return;	
	}

#endif 
// xiaoke 08.12.31
/**************************************************************

	FUNCTION NAME		: InitSettingApp(void)

  	PURPOSE				: initialise Setting application.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitSettingApp(void)
{
#ifdef __MMI_MAINLCD_220X176__
	SetHiliteHandler(MAIN_MENU_PROFILES_MENUID,highlight_mainmenu_profiles);
#endif
	SetHiliteHandler(MENU9102_INITIAL_SETUP, HighlightPhnsetScr);
	SetHiliteHandler(MENU9102_INITIAL_SETUP_NO_SIM, HighlightPhnsetScr);//added by zhoumn@2007/07/24
	SetHiliteHandler(MENU9141_TIME_AND_DATE, HighlightPhnsetTimeAndDate);
#if defined(__MMI_PREFER_INPUT_METHOD__)
    SetHiliteHandler(MENU_SETTING_PREFERED_INPUT_METHOD,  HighlightInputMehtodSetting);
#endif

	SetHiliteHandler(MENU_SETTING_RESTORE, HighlightRstScr);

	memset(WelcomeText,0,(MAX_SETTING_WELCOME_TEXT+1) * ENCODING_LENGTH);

	InitManageCalls();
	/*chenhe for greenstone. for the munu id IDM_SIMSWITCH,IDM_SIMCARD1,IDM_SIMCARD2 is define as a marco , so there is something wrong. 
	i comment it ;if we use this function ,we'd manage the muid by tools not defined the id by marco sololy*/	

//xiaoke 08.12.31
#ifdef __DOUBLE_SIMCARD_211__     
	InitSimSwitch();
#endif
//xiaoke 08.12.31

	//InitPhoneSetupApp();  
	InitNetworkSetupApp();
	InitSecuritySetupApp();
	PhnsetInitTimeNDate();
	InitFixedDialList();
	InitBarredDialList();
#ifdef __MMI_PREFER_INPUT_METHOD__
	InitPreferedInputMethod();
#endif
#ifdef __MMI_TTS_FEATURES__ 

    InitTTSPlayer();
#endif

}




/**************************************************************

	FUNCTION NAME		: GoBack2TheHistory(void)

  	PURPOSE				: going back to history by 1 level.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBack2TheHistory(void)
{
	GoBacknHistory(1);
}


/**************************************************************

	FUNCTION NAME		: EntryScrSettingMenu(void)

  	PURPOSE				: Entry screen for settings menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern BOOL HasValidSIM(void);
void EntryScrSettingMenu(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
//micha0819
	U16 SettingsSubmenuImageList[MAX_SUB_MENUS];
	EntryNewScreen(SCR9000_SETTINGS, NULL, EntryScrSettingMenu, NULL);
	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR9000_SETTINGS);
	
#if 1 //modified by zhoumn@2007/07/24
	if (!HasValidSIM())
	{
		nNumofItem = GetNumOfChild(MAIN_MENU_SETTINGS_MENUID_NO_SIM);
		GetSequenceStringIds(MAIN_MENU_SETTINGS_MENUID_NO_SIM, nStrItemList);
		GetSequenceImageIds(MAIN_MENU_SETTINGS_MENUID_NO_SIM, SettingsSubmenuImageList);
		SetParentHandler(MAIN_MENU_SETTINGS_MENUID_NO_SIM);
	}
	else
		{
		nNumofItem = GetNumOfChild(MAIN_MENU_SETTINGS_MENUID);
		GetSequenceStringIds(MAIN_MENU_SETTINGS_MENUID, nStrItemList);
		GetSequenceImageIds(MAIN_MENU_SETTINGS_MENUID, SettingsSubmenuImageList);
		SetParentHandler(MAIN_MENU_SETTINGS_MENUID);
	}
#else
    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MAIN_MENU_SETTINGS_MENUID);

	/* 4. Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MAIN_MENU_SETTINGS_MENUID);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MAIN_MENU_SETTINGS_MENUID,nStrItemList);

//micha0819
	GetSequenceImageIds(MAIN_MENU_SETTINGS_MENUID, SettingsSubmenuImageList);

	/* 6 Set current parent id*/
	SetParentHandler(MAIN_MENU_SETTINGS_MENUID);
#endif
	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* 8 Display Category1 Screen */

 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_SETTINGS_MENUID);
#endif
 

#ifdef __MMI_NUMBER_MENU__
	ShowCategory15Screen(STR_SCR9000_SETTINGS_CAPTION, IMG_SCR_SETTING_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);
#else
	ShowCategory15Screen(STR_SCR9000_SETTINGS_CAPTION, IMG_SCR_SETTING_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, SettingsSubmenuImageList,
						  LIST_MENU, 0, guiBuffer);
#endif
	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: IsShowWelcomeScr

  	PURPOSE				: API to return the status of Welcome Text Feature.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

	Author				: Vanita Jain

 

**************************************************************/

MMI_BOOL IsShowWelcomeScr1(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_WELCOME_TEXT, &data, DS_BYTE , &error);

	if(data == 0)
		return MMI_FALSE;
	else if(data == 0xff)
	{
		data = 0;
		WriteValue( NVRAM_SETTING_WELCOME_TEXT, &data, DS_BYTE , &error);
		return MMI_FALSE;
	}
	else
		return MMI_TRUE;
}

/**************************************************************

	FUNCTION NAME		: GetWelcomeText

  	PURPOSE				: API to return the Welcome Text.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: Nil

	RETURNS				: returns the welcome text string.

	Author				: Vanita Jain

 

**************************************************************/
S8* GetWelcomeText(void)
{
	U8 data[(MAX_SETTING_WELCOME_TEXT+1)*ENCODING_LENGTH];
	S16 error;
	S8 *gWelcomeText=NULL;


	memset(data,0,(MAX_SETTING_WELCOME_TEXT+1)*ENCODING_LENGTH);
	ReadRecord(NVRAM_EF_SETTING_LID, 1, data, MAX_SETTING_WELCOME_TEXT*ENCODING_LENGTH, &error);
	gWelcomeText = OslMalloc((MAX_SETTING_WELCOME_TEXT+1)*ENCODING_LENGTH);

	if(gWelcomeText == NULL)
	{
		return NULL;
	}
	memset(gWelcomeText,0,(MAX_SETTING_WELCOME_TEXT+1)*ENCODING_LENGTH);
	if(data[0] != 0xff || data[1] != 0xff )
	{
		pfnUnicodeStrcpy((PS8)gWelcomeText, (PS8)data);	
	}
	 return gWelcomeText;

}


#if defined(__MMI_APHORISM__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif


/**************************************************************

	FUNCTION NAME		: InitSettingBootup(void)

  	PURPOSE				: Initialise seetings for Bootup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitSettingBootup(void)
{
	/* Contrast level setting at bootup */
	PhnsetMainLCDContrast(7);
	PhnsetSubLCDContrast(7);
}


/**************************************************************

	FUNCTION NAME		: InitSettingNVRAM(void)

  	PURPOSE				: Initialise Settings 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifndef MMI_ON_WIN32
extern INT32 mmi_getCurrentPowerMode(void);
#endif
void InitSettingNVRAM(void)
{
	U8 data = 0;
	S16 error;
	/* Contrast level set to user value */
    #ifndef MMI_ON_WIN32
	if(mmi_getCurrentPowerMode()!=POWER_ON_EXCEPTION)
	{
		PhnsetRestoreToNvramContrast();
	}
    #endif
	PhnsetSetLangByNvramValue();
#if defined(__MMI_PREFER_INPUT_METHOD__)
	SetInputMethodFromNVRAM();
#endif

	 
        ReadValue( NVRAM_SSC_DISABLE_FLAG, &data, DS_BYTE , &error);
	if(data == 0xff)
        {
                data = 0;
		WriteValue( NVRAM_SSC_DISABLE_FLAG, &data, DS_BYTE , &error);
        }
        ssc_disable_flag = data;
         

	ReadValue( NVRAM_SETTING_STATUS_DT_TIME, &data, DS_BYTE , &error);
	if(data == 0xff)
	{
		data =1;
		WriteValue( NVRAM_SETTING_STATUS_DT_TIME, &data, DS_BYTE , &error);
	}
     //CurrentDtType = data;
	PhnsetSetShowDTStatus( data );
	 #if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
	PhnsetReadNvramCalibrationData();
	#endif
#if 0
	 ReadRecord(NVRAM_EF_LINESWITCH_LID, 1, (void *)&g_callset_cntx_p->LineID, sizeof(g_callset_cntx_p->LineID), &error); 
	
	 if(g_callset_cntx_p->LineID == LINE1)
	 {
	 		ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_L1);
	 }
	 else
	 {
	 		ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_L2);
	 }
#endif /* Guoj delete. It's not used now */
	 
}


/**************************************************************

	FUNCTION NAME		: GetSettingOptionsStatus(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GetSettingOptionsStatus(void)
{
	SECSETGetPIN1StatusReq();
	SECSETGetFdlBdlModeReq();
}


