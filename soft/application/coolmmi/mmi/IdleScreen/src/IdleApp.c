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
 *  IdleApp.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  This file is for The Idle screen application
 *
 * Author:
 * -------
 * -------
 * * 修改记录1：
 *       修改日期：20060512
 *       版 本 号：6205.v02
 *       修 改 人：苏世鹏
 *       修改内容：关闭include文件
 *------------------------------------------------------------------------------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/****************************************************************************
* Include Files                                                                
*****************************************************************************/

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr.h"
#include "l4dr1.h"

#include "mmi_trace.h"

#include "miscfunctions.h"
#include "keybrd.h"
#include "debuginitdef.h"
#include "gdi_include.h"   
#include "custdatares.h"
#include "custmenures.h"
#include "gui_themes.h"
#include "globalconstants.h"
#include "historygprot.h"
#include "datetimegprot.h"
#include "commonscreens.h" 					/* displaypopup */
#include "wgui_categories.h"
#include "wgui_categories_cm.h"
#include "wgui_categories_multimedia.h"
#include "wgui_categories_idlescreen.h"
#include "wgui_status_icons.h"
#include "wgui_softkeys.h"
#include "ucs2prot.h"
#include "worldclock.h"
#include "idleappdef.h"
#include "idleappprot.h"
#include "globaldefs.h"
#include "callhistorygprots.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "simdetectiongexdcl.h"
#include "simdetectiongprot.h"
#include "fontres.h"
#include "keybrd.h"
#include "callmanagementgprot.h"
#include "callhistorymain.h"				 /* chistresetcalllogindex */
#include "sscstringhandle.h"
#include "settinggprots.h"
#include "audioinc.h"
#include "gpioinc.h"
#include "settingprofile.h"
#include "unicodexdcl.h" 
#include "speeddial.h"
#include "messagesresourcedata.h"
#include "sublcdhistorygprot.h"
#include "wpssprotos.h"
#include "poweronchargerprot.h"
#include "funandgamesdefs.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "downloaddefs.h"
#include "quickaccessmenuitemprot.h"
#include "alarmframeworkprot.h"
#include "profilesgexdcl.h"
#include "phonebooktypes.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "phonesetupgprots.h"
#include "organizergprot.h"  				/* orgdeinit */
#include "settingdefs.h"					/* st_notifyduration define */
#include "settingprofile.h"					/* success_tone define */
#include "globalscrenum.h"					/* global_scr_dummy */
// Beginning--added by kecx for flashlight shortcut key on 20081120
#ifdef __MOD_FLASHLIGHT__
#include "organizerdef.h"
#endif
// end--added by kecx for flashlight shortcut key on 20081120
//extern void FlashlightOpen(void);

/*modified by wuzc at 20061220 for fs*/
//#include "fat_fs.h"
#include "filesystemdef.h"					/* file function */
/*modified by wuzc at 20061220 for fs*/

#include "filemanagergprot.h"				/* file path / file error */
#include "filemanagerdef.h"					/* error string id */
#include "statusiconres.h"
#if defined( __MMI_DATE_TIME_SETTING_REMINDER__)
#include "rtc_sw.h"
#endif

 
//#include "callslog.h"
#include "wgui_ems_categories.h"

 
#include "shortcutsprots.h"
// CSD end


//micha0601
#include "satgprots.h" 

#ifdef WAP_SUPPORT
#include "wapprot.h"
#endif

#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
#endif

#ifdef __MMI_IRDA_SUPPORT__
#include "irdammigprots.h"
#endif

#ifdef __MOD_SMSAL__
#include "messagesexdcl.h"
#include "smsapi.h"
#endif

#ifdef __MMI_CAMERA__
#include "lcd_if.h"						/* lcd layer enable flag */
#include "gdi_lcd_config.h"
#include "mmi_features_camera.h"
#include "mdi_datatype.h"
#include "mdi_camera.h"					/* camera lib */
#include "cameraapp.h"
#endif

#include "timerevents.h"
#include "wgui_status_icons.h"
#ifdef MMI_ON_HARDWARE_P
#include "custom_util.h"
#endif
#include "nitz.h"
 

#include "keybrd.h"
#include "osthreadinterfacegprot.h"
#include "eventsgprot.h"

 
#include "networksetupdefs.h"

//micha0628
#include "networksetupgprot.h"

#ifdef __MMI_IMPS__
#include "mmi_imps_gprot.h"
#endif

 
#ifdef __MMI_POC__
#include "pocgprot.h"
#endif

#ifdef __MMI_TOUCH_SCREEN__
#include "touchscreengprot.h"
#include "wgui_touch_screen.h"
#include "touch_screen_cfg.h"
#endif
#if (defined(__MMI_VCARD__) || defined(__MMI_VCALENDAR__)) && defined(MMI_ON_HARDWARE_P)
#include "vobjects.h"
#endif	

#ifdef __MMI_MULTI_SIM__
#include "mtpnp_ad_resdef.h"
#include "dual_sim.h"
#include "dual_sim_call.h"
#include "dual_sim_calllog.h"
#endif

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
#include "pdastyle.h"
#endif //__MMI_SYMBIAN_STYLE_IDLE__
/*- zhouqin 2011-02-16 modify for q6 320X240 */


#if defined(__IP_NUMBER__)
extern void SetTempUseIPNumber(pBOOL flag);
#endif

#ifdef __MMI_TTS_FEATURES__ 
extern void ejTTSPlayTime();
#endif

/*gaosh Add 
Start For 6192 Ver: TBM780_T_20070820_2029  on 2007-8-21 17:27 */
#include "commonscreens.h"
#include "messagesmiscell.h"
/*gaosh Add End  For 6192 Ver: TBM780_T_20070820_2029  on 2007-8-21 17:27 */
#include "usb_interface.h"

#include "soundrecorderdef.h"
#include "phonebookdef.h"
#include "fmradioprot.h"
#ifdef JATAAYU_SUPPORT
#include "ddl.h"
#include "jcal.h"

extern BOOL jmms_NeedNewMMSPopupOnIdle(void);
extern void jmms_entry_new_msg_ind (void);
#if defined(__MMI_MAINLCD_220X176__) && defined(__MMI_TOUCH_SCREEN__)
extern void mmi_e71_idle_lsk_down_info(void);
#endif
#ifdef JOTA_PROVISION_APP_ENABLE
extern BOOL jwap_NeedNewPushPopupOnIdle();
extern void jwap_New_Push_Msg_Ind();
JC_BOOLEAN prv_IsProvPushAvail();
void prv_MMIDisplayNewProvAlertScreen();

#endif

#endif
#undef __NEWSIMULATOR
/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/
#define SetLine1Text SetIdleScreenNetworkName
#define SetLine2Text SetIdleScreenNetworkStatus

/****************************************************************************
* Global Variable                                                           
*****************************************************************************/
extern U8 	gKeyPadLockFlag;
extern U16 	gCurrLangIndex;
extern U16 	gMaxDeployedLangs;
extern U8 	isIdleScreenReached;
extern S32 	MAIN_LCD_device_width;
extern U8 	screen_saver_type;

extern sLanguageDetails *gLanguageArray;

extern void EmergencyCallCheck(S16 keyCode, S16 keyType);
extern void mmi_java_send_mid_start_req(void);
extern BOOL is_phb_init_done(void);

extern U8 GetHandsetInPhone();
#if defined(__MMI_MAINLCD_160X128__)
extern void EntryScrProfilesActivate(void);
#endif
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
extern BOOL  gIsInSleepMode;
#endif
#if defined(__PROJECT_GALLITE_C01__)
BOOL IsFocusOnInputBox(void);
#endif

//micha0420
//extern U8 gSettings;
//micha0601
//extern FuncPtr GetPostponedSATScreen(void);
//extern void ClearPostponedSATScreen(void);

/****************************************************************************
* Global Function                                                           
*****************************************************************************/
extern void MMIProcessActionMsg(void);
extern U8* GetDeliverdReportMessage (void);
extern kal_bool INT_USBBoot(void);
extern EMSData* GetClass0MessagePtr(void);
extern U8* GetClass0MessageHeader (void);
extern BOOL IsMyTimerExist(U16 nTimerId);
extern BOOL IsClamClose(void);
#ifdef __PRJ_WITH_SPICAM__
extern void mmi_camera_entry_option_menu_screen(void);
#endif
#ifdef WAP_SUPPORT
extern void wap_internet_key_hdlr(void);
#endif

extern void SATEVDLIdleScreenAvaliable(void);

#ifdef __J2ME__
extern void j2me_enter_idle_screen_notify(void);
#endif

extern void mmiapi_enter_idle_screen_notify(void);

#ifdef __MMI_AUDIO_PLAYER__
extern BOOL mmi_audply_is_playing( void );
extern void mmi_audply_set_subLCD_shown( BOOL is_shown );
extern void mmi_audply_entry_main(void);
#endif /* __MMI_AUDIO_PLAYER__ */

#ifdef __MMI_FM_RADIO__
extern BOOL mmi_fmrdo_is_power_on( void );
extern void mmi_fmrdo_set_subLCD_shown( BOOL is_shown );
void mmi_fmrdo_power_on( BOOL turn_on );
#endif /* __MMI_FM_RADIO__ */

#if defined(__MMI_VRSD__)
extern void mmi_vrsd_rcg_pre_entry(void);
#endif
#if defined(__MMI_VRSD_DIAL__)
extern BOOL mmi_vrsddial_check_sim_change(void);
#endif

#if defined(__MMI_VRSI__)
extern void mmi_vrsi_rcg_pre_entry(void);
#endif

#ifdef MMI_ON_WIN32
extern void MMI_system_keybord_handler(s32 key_code,s32 key_state);
#endif
#if 0
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
extern BOOL mmi_phb_init_check_sim_change(void);
#endif
#endif
#ifdef __J2ME__
extern BOOL java_is_launch_after_install(void);
#endif

extern void EntryMainMenuFromIdleScreen(void);
#ifdef __MMI_MAINLCD_220X176__
extern void ClndrPreEntryApp(void);
extern void mmi_camera_entry_app_screen(void);
extern void EntrySMSMainMenuList(void);

extern void mmi_audply_entry_main(void);
extern void mmi_vdoply_entry_app(void);
extern void mmi_fng_entry_screen(void);
#endif

#if defined(__PROJECT_GALLITE_C01__)
extern void mmi_audply_entry_main(void);
#endif 

#ifdef __MMI_WGUI_MINI_TAB_BAR__
extern void ClndrPreEntryApp(void);
extern void fmgr_launch (void);
#endif
#ifdef __FT_IDLE_SMART_DIAL__  
S32 SmartDial_number_of_items;
S8 	 g_DialPadCallBuffer[MAX_DIAL_PAD * ENCODING_LENGTH];
extern U16 IsSmartDialStyle(void);
#endif
 
#ifdef __FLIGHT_MODE_SUPPORT__
extern void EntryPhnsetFlightMode(void);
extern void EntryScrSimOptionMenu(void);
extern void PhnsetFlightModeSaveStatusIconFlags(void);
void EntryFlightModeIdleScreen(void);
void EntryFlightModeKeypadLockScreen(void);
void FlightModeDigitHandler(void);
#endif
 

extern void InsertSimSetKeyPadLockFlag(void);
extern void ExitAutoTest(void);
#if defined(__MMI_MAINLCD_160X128__)
extern void mmi_audply_entry_main(void);
extern void EntryDMCHISTMainMenu(void);
#endif
#if defined(CAM_MD_SUPPORT)
extern void csd_start_sw_motionsensor(void);
extern void csd_stop_sw_motionsensor(void);
#endif
#ifdef __MMI_USE_LSK_AS_SEND2__
extern void DummyKeyHandler(void);
#endif

#if (defined( __MMI_ENGINEER_MODE__) || defined( __MMI_FACTORY_MODE__)  )
U8 gCurrentMode=NORMAL_MODE;
#endif

/***************************************************************************** 
* Local Variable
*****************************************************************************/
idle_context_struct  g_idle_context = 
{
	0, 	/* IsOnIdleScreen */ 
	SUBLCD_NOT_ON_IDLE, 	/* IsSubLcdOnIdleScreen */
	0, 	/* IsOnDialerScreen */
	0,		/* IsOnSimErrorScreen */
	0,		/* IsOnSimErrorDialerScreen */
	0,		/* IsScrSvrSleepIn */
	0, 	/* IsRSKPressOnScreenSaver */
	0, 	/* LongPressEnable */
	0, 	/* AvoidHistoryofDialerScreen */
	0, 	/* ScreenSaverRunFlag */
	0, 	/* ToNameScrFromIdleApp */
#ifdef	__MMI_KEYPAD_LOCK_PATTERN_2__ 
	0, 	/* ToMainMenuScrFromIdleApp */
#endif
	0,	/* RskPressedFromIdleApp */
	-1, 	/*IdleScreenDigitKeyCode */
	SET_CURRENT_PROFILE, /* ToggleProfile */
	0, 	/* AutoLockTimeOut */
	{0}  	/* all dialled digits from idle screen */
};

static U16 idleRStringtID=0 ;
static U16 idleLStringtID=0 ;
PUBLIC U8 gIsOnSearchingScreen = 0;
static BOOL g_scrsvr_is_image;
/*gaosh Add Start For Display_lock_unlock_popupscreen Ver: TBM780_T_20070824_1818  on 2007-8-27 15:35 */
static BOOL g_endkey_is_pressed_onidle;
static BOOL Is_KeyEnd_Press_OnIdle(void);
/*gaosh Add End  For Display_lock_unlock_popupscreen Ver: TBM780_T_20070824_1818  on 2007-8-27 15:35 */
/*Zhangm Add Start For 10936 Ver: GreenStone  on 2008-12-22 11:7 */
BOOL g_endkey_is_pressed_on_sim_error_src = FALSE;
BOOL Is_KeyEnd_Press_On_Sim_Error_Src(void);
/*Zhangm Add End  For 10936 Ver: GreenStone  on 2008-12-22 11:7 */
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
extern g_Symbian_idle_item  pda_idle_shortcut[];
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */

#if defined(__PROJECT_GALLITE_C01__) //__MMI_INTELIGENT_CALL_NUMBER__
U8 EnterDialScrByCenterSoftkeyFlg = 0;	/*liping 2008-07-01 Flag for indicate enter dial scrn by press center softkey in idle screen*/
//>>luchongguang 修改2008-10-24 : 进入智能搜索功能标志(1 : yes    0: no)
extern S8 *GetCallBuffer(); // qinjiangwei 



extern void mmi_dialer_entry_screen(U16 scr_id, 
					U16 left_softkey_id,
					U16 right_softkey_id, 
					U8 *buffer,
					S32 buff_len);


void mmi_dailer_exit_screen(void);		// added by qinjiangwei 2008/5/28

#endif

/*****************************************************************************
* Local Function 
*****************************************************************************/



 
/*****************************************************************************
* FUNCTION
*	 DeInitSpecialMode()
* DESCRIPTION
*  DeInit SpecialMode
* PARAMETERS
*	 nil
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void DeInitSpecialMode(void)
{
#ifdef __MMI_ENGINEER_MODE__
	if( gCurrentMode == ENGINEERING_MODE)
	{
		gCurrentMode = NORMAL_MODE;
		mdi_audio_resume_background_play();
		StartLEDPatternBackGround();
	}
#endif
}

//CSD end


/*****************************************************************************
* FUNCTION
*	 SearchingSrceenTimeOutHandler()
* DESCRIPTION
*   This function is a call back for searching screen expire
* PARAMETERS
*	 nil
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
 
void SearchingSrceenTimeOutHandler(void)
{
	U16 i;
	//恢复搜网中标记.
	gIsOnSearchingScreen = 0;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		g_pwr_context[i].NetWorkFlag = 1;
		g_pwr_context[i].IsCampOnTonePlayed = 1; // Campon tone will not be played after the timeout
	}
	if (g_idle_context.IsOnIdleScreen)
	{
		EntryIdleScreen();
	}
}


/*****************************************************************************
* FUNCTION
*	 IdleScreenDummyKeyHandler()
* DESCRIPTION
*   This function is a dummy handler for idle screen keys.
*   It just stops and starts the screen saver timer if necessary
* PARAMETERS
*	 nil
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static FuncPtr dummyKeyHandler[MAX_KEYS];
static void IdleScreenDummyKeyHandler(void)
{
	U8 ScreenSaverStatus, ScreenSaverWait;
	U16 keyCode, keyType;

	PhnsetGetScrSvrSetings(&ScreenSaverStatus, &ScreenSaverWait);

	GetkeyInfo(&keyCode, &keyType);

	if(g_idle_context.IsOnIdleScreen)
	{
		//postpone the screensaver after press keys
		if(dummyKeyHandler[keyCode]!=NULL &&!gKeyPadLockFlag)
			(*dummyKeyHandler[keyCode])();

		if (gKeyPadLockFlag && keyCode == KEY_END)
		{
			(*dummyKeyHandler[keyCode])();
		}
		if(dummyKeyHandler[keyCode] == NULL && keyCode == KEY_ENTER)
		{
			if(GetKeyHandler(KEY_ENTER, KEY_EVENT_UP) == NULL)
			{
				if(get_softkey_function(KEY_EVENT_DOWN, MMI_LEFT_SOFTKEY) == NULL || get_softkey_function(KEY_EVENT_DOWN, MMI_LEFT_SOFTKEY) == UI_dummy_function)
				{
					SetKeyHandler(get_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY), KEY_ENTER, KEY_EVENT_UP);
				}
				else
				{
					execute_softkey_function(KEY_EVENT_DOWN, MMI_LEFT_SOFTKEY);
				}
			}
		}
			
	}

	if(g_idle_context.IsOnIdleScreen)
	{
		StopTimer(SCREENSAVER_TIMER);
		StartTimer(SCREENSAVER_TIMER, ScreenSaverWait*1000, EntryIdleSetScreenSaver);
	}
}

void DisplayLockUnlockPopupScreen(void)
{
    if(gKeyPadLockFlag && g_idle_context.IsOnIdleScreen&&g_endkey_is_pressed_onidle) 
        DisplayPopup ((U8*)GetString (STR_GLOBAL_UNLOCK_POPUP), IMG_GLOBAL_UNLOCK_POPUP, 1, 1000/*MESSAGES_POPUP_TIME_OUT*/, (U8)EMPTY_LIST_TONE);
    else if(gKeyPadLockFlag && g_idle_context.IsOnSimErrorScreen&&g_endkey_is_pressed_on_sim_error_src) 
        DisplayPopup ((U8*)GetString (STR_GLOBAL_UNLOCK_POPUP), IMG_GLOBAL_UNLOCK_POPUP, 1, 1000/*MESSAGES_POPUP_TIME_OUT*/, (U8)EMPTY_LIST_TONE);

    if(!gKeyPadLockFlag&&g_idle_context.IsOnIdleScreen&&g_endkey_is_pressed_onidle&&(MTPNP_AD_Get_UsableSide_Number() != 0))
        DisplayPopup ((U8*)GetString (STR_GLOBAL_LOCK_POPUP), IMG_GLOBAL_LOCK_POPUP, 1, 1000/*MESSAGES_POPUP_TIME_OUT*/, (U8)EMPTY_LIST_TONE);
    else if(!gKeyPadLockFlag&&g_idle_context.IsOnSimErrorScreen&&g_endkey_is_pressed_on_sim_error_src&&(MTPNP_AD_Get_UsableSide_Number() != 0))
        DisplayPopup ((U8*)GetString (STR_GLOBAL_LOCK_POPUP), IMG_GLOBAL_LOCK_POPUP, 1, 1000/*MESSAGES_POPUP_TIME_OUT*/, (U8)EMPTY_LIST_TONE);
}

/*gaosh Add Start For Display_lock_unlock_popupscreen Ver: TBM780_T_20070824_1818  on 2007-8-27 15:35 */
static BOOL Is_KeyEnd_Press_OnIdle(void)
{
	U16 keyCode,keyType;
	
	GetkeyInfo(&keyCode, &keyType);

	if((keyCode == KEY_END)&&(keyType == KEY_EVENT_DOWN)&&g_idle_context.IsOnIdleScreen)
		return TRUE;
	else
		return FALSE;
}
/*gaosh Add End  For Display_lock_unlock_popupscreen Ver: TBM780_T_20070824_1818  on 2007-8-27 15:35 */

/*zhangm Add Start For 10936 Ver: GreenStone  on 2008-12-22 11:11 */
BOOL Is_KeyEnd_Press_On_Sim_Error_Src(void)
{
	U16 keyCode,keyType;
	
	GetkeyInfo(&keyCode, &keyType);

	if((keyCode == KEY_END)&&(keyType == KEY_EVENT_DOWN)&&g_idle_context.IsOnSimErrorScreen)
		return TRUE;
	else
		return FALSE;
}
/*zhangm Add End  For 10936 Ver: GreenStone  on 2008-12-22 11:11 */
/************************************************************************
	FUNCTION NAME		: DisplayIdleScreen()

  	PURPOSE				: Shows the idle screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DisplayIdleScreen(void)
{
	//micha0420
	extern U8 gCallCost;

	/* highlight indexes should be reset when press end key 
	for the call history module*/
	gCallCost = 0;
	
	#ifdef _KK_SUPPORT_ 
	{
		
		extern void  exit_kk_app_by_endkey(void);
		exit_kk_app_by_endkey();

	}
	#endif
     
	/*currRecdCallIndex = 0;
	currMissedCallIndex = 0;
	currDialCallIndex = 0;
	 
	#ifdef MXED_CALL_LOG
	currMixCallIndex = 0;
	#endif
	CurrentHighlight = 0;
	*/
	/*gaosh Add Start For Display_lock_unlock_popupscreen Ver: TBM780_T_20070824_1818  on 2007-8-27 15:34 */
	g_endkey_is_pressed_onidle=Is_KeyEnd_Press_OnIdle();
	/*gaosh Add End  For Display_lock_unlock_popupscreen Ver: TBM780_T_20070824_1818  on 2007-8-27 15:34 */

	/*zhangm Add Start For 10963 Ver: GreenStone  on 2008-12-22 11:21 */
	g_endkey_is_pressed_on_sim_error_src = Is_KeyEnd_Press_On_Sim_Error_Src();
	/*zhangm Add End  For 10963 Ver: GreenStone  on 2008-12-22 11:21 */

	CHISTResetCallLogIndex();
#if defined(__MMI_MAINLCD_220X176__)	
		if(is_on_idlescreen())//historyData[0].entryFuncPtr is NULL in idle screen 
			EntryIdleScreen();
		else
#endif	
	//To execute the root history.
	ExecuteRootMainHistoryScreen(NULL);

	ExitAutoTest(); //add by chenqiang for black light off when exit from auto test

	// TODO: 为何要关闭下面的代码?
	OrgDeInit();                //tangjie del 20060616  -----> chenqiang open it 20090608
	
#if (defined(__MMI_VCARD__) || defined(__MMI_VCALENDAR__)) && defined(MMI_ON_HARDWARE_P)
	mmi_vobj_deinit();       //renwy add 2008/08/19
#endif	

	AlmEnableSPOF();
}

static void idle_notify_bootup_ready(void) // this function is call by MMI
{
#ifdef WAP_SUPPORT
// TODO: 为何要关闭下面的代码?
//#ifdef MMI_ON_HARDWARE_P
	MYQUEUE	Message;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_WAP;
	Message.oslMsgId = MSG_ID_MMI_READY_NOTIFY_REQ;
	Message.oslDataPtr = (oslParaType *)NULL;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
//#endif
#endif
}

/**************************************************************

	FUNCTION NAME		: EntryIdleScreen

  	PURPOSE				: Entry function for idle screen
	

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void MMI_notify_bootup_ready(void);

#ifdef __MMI_IDLE_CAMERA_KEY_TO_STOP_AUDIOPLAYER__
static void CheckAndStopAudioPlayer(void)
{
#ifdef __MMI_AUDIO_PLAYER__
   if( mmi_audply_is_playing() )
      mmi_audply_do_stop_action();
   else
#endif
#ifdef __MMI_FM_RADIO__
   if( mmi_fmrdo_is_power_on() )
      mmi_fmrdo_power_on( FALSE );
   else
#endif

#ifdef __PRJ_WITH_SPICAM__
    mmi_camera_entry_option_menu_screen( );
#else
    mmi_camera_entry_app_screen( );
#endif
} 
#endif

//Lisen 01252005
U8 IdleScreenEmergencyCallCheckNKeypadLock(KEYBRD_MESSAGE *eventKey)
{
	if(gKeyPadLockFlag && (g_idle_context.IsOnIdleScreen || g_idle_context.IsOnSimErrorScreen)) 
	{
		EmergencyCallCheck(eventKey->nKeyCode, eventKey->nMsgType);
	}
	if((g_idle_context.AutoLockTimeOut!=0)&&(g_idle_context.IsOnIdleScreen || g_idle_context.IsOnSimErrorScreen))
	{
		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_EXECURKEY_HDLR, g_idle_context.AutoLockTimeOut));
		StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
		if(g_idle_context.IsOnIdleScreen)
			StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, IdleSetKeyPadLockFlag);
		if(g_idle_context.IsOnSimErrorScreen)  
			StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, InsertSimSetKeyPadLockFlag);
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*	 RedrawIdleScreen()
* DESCRIPTION
*   Entry idle screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void RedrawIdleScreen(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	IdleRefreshServiceIndicationArea();	
	
	#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) && !(defined __MMI_TOUCH_IDLESCREEN_SHORTCUTS__))	
#ifdef __FLIGHT_MODE_SUPPORT__
	if (mmi_bootup_get_active_flight_mode() != 1)
#endif
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
	#endif

	#ifdef __MMI_SWAP_LSK_RSK_IN_IDLE_SCREEN__
		    /*+ zhouqin 2011-02-16 modify for q6 320X240 */
			#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
					ShowCategorySymbianScreen(idleLStringtID, 
														IDLE_SCREEN_LSK_ICON, 
														idleRStringtID, 
														IDLE_SCREEN_RSK_ICON, 
														SYMBIAN_TOTAL_ITEMS, 
														pda_idle_shortcut, 
														0, 
														NULL);
			#else
		ShowCategory33Screen(idleLStringtID, IDLE_SCREEN_RSK_ICON,
					idleRStringtID, IDLE_SCREEN_LSK_ICON, NULL);
			#endif
	#else
		 	#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
				ShowCategorySymbianScreen(idleLStringtID, 
													IDLE_SCREEN_LSK_ICON, 
													idleRStringtID, 
													IDLE_SCREEN_RSK_ICON, 
													SYMBIAN_TOTAL_ITEMS, 
													pda_idle_shortcut, 
													0, 
													NULL);
			#else
		ShowCategory33Screen(idleLStringtID,IDLE_SCREEN_LSK_ICON,
					idleRStringtID, IDLE_SCREEN_RSK_ICON, NULL);
			#endif
		/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#endif
	IdleSetLRKey();	
}

/*****************************************************************************
 * FUNCTION
 *  mmi_idle_missed_call_check
 * DESCRIPTION
 *  Wrapper for idle screen callback
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static U8 mmi_idle_missed_call_check(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U8 i;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(GetMissedCallFlag()&&
			#ifdef __MMI_MULTI_SIM__
			MTPNP_PFAL_Is_Card_Usable(i)&&
			#endif
			CHISTGetMissedCallCountBeforeView(i))
		{
			return MMI_TRUE;
		}
	}

	SetMissedCallFlag(MMI_FALSE);
	return MMI_FALSE;
}


/*****************************************************************************
* FUNCTION
*	 EntryIdleScreen()
* DESCRIPTION
*   Entry idle screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#include "mci.h"
BOOL g_BeforeEntryIdleScr = 1;
BOOL g_UsbChargerPowerOn = 0;
BOOL do_usbplugout_later= 0;
#if (CSW_EXTENDED_API_PMD_USB == 0)
BOOL g_NeedReDetectUsb = 0;
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
#include "bootup.h"
extern void BatteryIndicationPopup(U16 stringId);
extern void SendMessageForUSB(UINT32 action);

#if defined __MMI_OPP_SUPPORT__ 
#include "btmmioppgprots.h"
#include "oppmmigprots.h"
#include "bluetooth_struct.h"
#include "filemgr.h"
#include "btmmiopp.h"
extern BOOL g_NeedRecheckOppRecv;
extern void mmi_opp_scr_recv_obj_ind(void);
#endif
#ifdef __MMI_MULTI_SIM__
void DisplaySMSNotUsableWarn(void)
{
	DisplayPopup((PU8)GetString(STRING_MTPNO_SMS_NOT_USABLE) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
}
#endif
#ifdef __MMI_MAINLCD_220X176__
	extern void EntrySMSMainMenuList(void);
	extern void EntryDMCHISTMainMenu(void);
#endif
#if defined(__MMI_SUPPORT_JME__)
#ifndef MMI_ON_WIN32

  extern void smarchbox_plugin_init();
#endif
#endif
void mmi_frm_sms_set_protocol_event_handler (void);
// add by wuys for patching QQ, 2010-06-09
#ifdef __MMI_SUPPORT_JME__
extern s8 vm_status;
extern void exitSmarchBox(void);
extern int smarchbox_attrsp;
#endif

void Modify_SmarchAttStatus(void)
{
#if defined (__MMI_SUPPORT_JME__ )&&!defined (MMI_ON_WIN32)
	mmi_trace(1, TSTXT("MMI_IDLE: Modify_SmarchAttStatus, smarchbox_attrsp: %d.\n") ,smarchbox_attrsp);    

	smarchbox_attrsp = 0;
	mmi_trace(1, TSTXT("MMI_IDLE: Modify_SmarchAttStatus, smarchbox_attrsp: %d.\n") ,smarchbox_attrsp);    

	return;
#else
    return;
#endif
}
//add end
#ifdef __MMI_ANALOGTV__
       extern void mmi_analogtv_entry_app();
#endif

#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
void CSD_UI_get_old_window();
#endif

#ifdef __MMI_MAINLCD_160X128__
void mmi_fmrdo_run_app( void );
void mmi_audply_entry_main(void);
#elif defined(PRJ_F021)  //__PROJECT_GALLITE_C01__
void mmi_fmrdo_run_app( void );
void mmi_audply_entry_main(void);
#if !defined(MMI_ON_WIN32)
#include "hal_gpio.h"
#endif
void mmi_lamp_run_app( void )
{
#if !defined(MMI_ON_WIN32)
	static BOOL lamp_on = TRUE;
	HAL_APO_ID_T	 lamp_ctrl;
	
	lamp_ctrl.gpoId=HAL_GPO_4;
	if ( lamp_on == TRUE )   {
	//	hal_GpioSetOut(lamp_ctrl.gpioId);
		hal_GpioSet(lamp_ctrl);
	}
	else   {
		hal_GpioClr(lamp_ctrl);
	//	hal_GpioSetIn(lamp_ctrl.gpioId);
	}
	lamp_on = !lamp_on;
#endif
}
#endif

void EntryIdleScreen(void)
{

	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

#if  (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) 
	U16 IdleScreenDigits[MAX_IDLE_SCREEN_DIGITS] = 
		{KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_STAR,KEY_POUND,KEY_CSK};
#else
	U16 IdleScreenDigits[MAX_IDLE_SCREEN_DIGITS] = 
		{KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_STAR,KEY_POUND};
#endif
	U8 ScreenSaverStatus, ScreenSaverWait;
	SETTING_AKPL_TIME GetAutoLockTimeOut;
	U16 NetWorkFlag = 0;
	U16 i;
	
	TBM_ENTRY(0x28D4);
	g_BeforeEntryIdleScr = 0;

	if (do_usbplugout_later)
	{
		do_usbplugout_later = 0;
		SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGOUT);
	}
	mmi_trace(g_sw_MAINENTRY, TSTXT("MMI_IDLE: EntryIdleScreen: Start.\n") );    


	#if defined( __MMI_DATE_TIME_SETTING_REMINDER__)       
	static BOOL datejust_reminder_shown = FALSE;
	#endif
	#ifdef _KK_SUPPORT_ 
	{
		
		extern void  exit_kk_app_by_endkey(void);
		exit_kk_app_by_endkey();

	}
	#endif
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
    initial_Symbian_Info(pda_idle_shortcut);
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	EntryNewScreen(IDLE_SCREEN_ID,ExitIdleScreen, NULL, NULL);
#if defined(CAM_MD_SUPPORT)
//    gui_start_timer(200, csd_start_sw_motionsensor);
    csd_start_sw_motionsensor();
#endif
//blow code cause keypad event turn backlight when hanset enter deep mode
//	TurnOnBacklight(1);

#if defined(__PROJECT_GALLITE_C01__)//(__MMI_INTELIGENT_CALL_NUMBER__)//>>Bug #6521:待机界面输入电话号码后无触屏拨号功能(更改为：如通过触摸屏进入触摸拨号界面，而通过按键则进入智能搜索界面) luchongguang add 2008-10-24
    EnterDialScrByCenterSoftkeyFlg = 0;
#endif

#ifdef __MMI_TOUCH_SCREEN__
    mmi_pen_register_abort_handler( (mmi_pen_hdlr)UI_dummy_function);
	mmi_pen_register_repeat_handler( (mmi_pen_hdlr)UI_dummy_function);
	mmi_pen_register_long_tap_handler( (mmi_pen_hdlr)UI_dummy_function);
#endif
	// TODO: 为何要关闭下面的代码?
	//tangjie del begin 20060616
	#if 0
	#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	if(is_redrawing_old_screens())
	{
		RedrawIdleScreen();
		return;
	}
	#endif
	#endif
	//tangjie del end 20060616

	g_pwr_context[0].PowerOnTillIdleApp=0;
	g_pwr_context[0].PowerOnPeriod=0;
	g_pwr_context[0].PrevScreenIndicator = ENTER_IDLE_SCR;



#if defined(__MMI_SUPPORT_JME__)
#ifdef __MMI_COMPRESS_CODE_SECTION__
  cdfu_jmetoc();
#endif
#ifndef MMI_ON_WIN32

 //   if (vm_status) 
    {
        gdi_layer_lock_frame_buffer();
        smarchbox_plugin_init();
        gdi_layer_unlock_frame_buffer();
        mmi_frm_sms_set_protocol_event_handler();
        mmi_trace(1, TSTXT("MMI_IDLE: 2 exitSmarchBox, vm_status: %d.\n") ,vm_status);    
    }
 
#endif
#endif
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
        RegisterHighlightHandler(Gui_Symbian_custom_highlight_handler);
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */

	DinitHistory();

	// TODO: 为何要关闭下面的代码?
	//tangjie del begin 20060616
    #if 1 
	#ifdef __J2ME__
		j2me_enter_idle_screen_notify();
	#endif
	#ifndef MMI_ON_WIN32
		//mmiapi_enter_idle_screen_notify(); //chenhe comment
	#endif
	/* EngineerMode/Factory Deinit */
	DeInitSpecialMode();

#ifndef MMI_ON_WIN32
	#if defined(__MMI_WEBCAM__) && defined(__MMI_USB_SUPPORT__)
	/* entry webcam if it is still active */
	if(mmi_usb_webcam_is_active())
	{	
		mmi_usb_webcam_entry_app_scr();
		return;
	}
	#endif /* __MMI_WEBCAM__ && __MMI_USB_SUPPORT__ */
#endif
	 
#ifdef __FLIGHT_MODE_SUPPORT__
  {
#ifdef __MMI_MULTI_SIM__
	if(MTPNP_AD_Is_Flight_Mode())
#else
	if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
#endif
	{

		if (!IS_IDLE_ARRIVED)
		{
			g_pwr_context[0].IdleScreenArrived = 1;
			g_pwr_context[0].NetWorkFlag = 1;
			g_pwr_context[0].InvalidSimShowed = 1;
			isIdleScreenReached = 1;
			TurnOffBacklight();
			StartLEDPatternBackGround();

			if( g_charbat_context.isChargerConnected )
				StartTimer(BOOTUP_CHARGER_DETECT_TIMER, 3000, ChargerDetectTimeoutHdlr );

#ifdef __MMI_USB_SUPPORT__
			PendingUsbDetectionHdlr();
#endif
			PendingAlarmReminder();

			idle_notify_bootup_ready(); // to WAP
			MMI_notify_bootup_ready();  // to L4C
		#ifdef __MMI_IMPS__
			mmi_imps_notify_idle_reached();
		#endif
		}

		/* screen saver in flight mode */
		PhnsetGetScrSvrSetings(&ScreenSaverStatus, &ScreenSaverWait);
		if( ScreenSaverStatus == 0xFF )
		{
			PhnsetRstScrSvrDefault();
		}
		if( ScreenSaverStatus == 1 )	//Screen Saver Is Active
		{
			StartTimer(SCREENSAVER_TIMER, ScreenSaverWait*1000, EntryIdleSetScreenSaver);
		}

		/* keypad lock in flight mode */
		if(!gKeyPadLockFlag)
		{
			GetAutoLockTimeOut=GetAutoKeypadLockTime();
			switch(GetAutoLockTimeOut)
			{
				case TIME_0:
				g_idle_context.AutoLockTimeOut=0;
				break;
				case TIME_5:
				g_idle_context.AutoLockTimeOut=5000;
				break;
				case TIME_30:
				g_idle_context.AutoLockTimeOut=30000;
				break;
				case TIME_60:
				g_idle_context.AutoLockTimeOut=60000;
				break;
				case TIME_2:
				g_idle_context.AutoLockTimeOut=300000;
				break;
			}
			
			if(g_idle_context.AutoLockTimeOut!=0)
				StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, IdleSetKeyPadLockFlag);
		}

		g_idle_context.IsOnIdleScreen = 1;
		g_idle_context.IsOnDialerScreen = 0;
		g_idle_context.LongPressEnable = 0;
		ClearAllKeyHandler();

		DeInitSpecialMode();

		#ifdef __J2ME__			
		if(java_is_launch_after_install())
		{
			mmi_java_send_mid_start_req();
		}
		else
		{
		#endif /* __J2ME__ */

			if(gKeyPadLockFlag == 0) /* keypad is not locked */
			{
				EntryFlightModeIdleScreen();
				SetGroupKeyHandler(HandleIdleScreenDigitEntry,IdleScreenDigits,MAX_IDLE_SCREEN_DIGITS-1,KEY_EVENT_DOWN);

				#if 1				
				#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
					#ifdef __MMI_MULTI_SIM__
					if(MTPNP_PFAL_Has_CardValid())
					#endif
					#if defined(__MMI_MAINLCD_220X176__)
					IdleSetLRKey();	
				#else

					RegisterDedicatedKeyHandlers();
				#endif
				#else
					SetKeyHandler(EntryShctInIdle,KEY_UP_ARROW,KEY_EVENT_DOWN);
				#endif
				#endif
				
				if( ScreenSaverStatus == 1 )	//Screen Saver Is Active
				{
					for (i=KEY_0; i<MAX_KEYS; i++)
					{
						dummyKeyHandler[i] = GetKeyHandler(i, KEY_EVENT_DOWN);
					        SetKeyHandler(IdleScreenDummyKeyHandler, i, KEY_EVENT_DOWN);
					}
				}
	
			}
			else
			{
				EntryFlightModeKeypadLockScreen();
				if( ScreenSaverStatus == 1 )	//Screen Saver Is Active
				{
					
					for (i=KEY_0; i<MAX_KEYS; i++)
					{
						if (GetKeyHandler(i,KEY_EVENT_DOWN) == NULL && GetKeyHandler(i,KEY_EVENT_UP) == NULL)
							SetKeyHandler(IdleScreenDummyKeyHandler, i, KEY_EVENT_DOWN);
					}
	
					// add this because of PowerAndEndKeyHandler 
					dummyKeyHandler[KEY_END] =  DisplayIdleScreen;
					SetKeyHandler(IdleScreenDummyKeyHandler, KEY_END, KEY_EVENT_DOWN);
				}
			}
		#ifdef __J2ME__	
		}
		#endif /* __J2ME__ */

		return;
	}
  }
	#endif // __FLIGHT_MODE_SUPPORT__
	#endif
    //tangjie del end 20060616


	mmi_phb_reset_scr_id();
	// TODO: 为何要关闭下面的代码?
	//tangjie add begin 20060616
	#if 0
	NETSETCheckAndRestoreNetwork();
	SATEVDLIdleScreenAvaliable();
	#endif
	//tangjie del end 20060616
	//tangjie del begin 20060616
	#if 0
	#ifdef WAP_SUPPORT
	#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	if (!is_redrawing_old_screens())
	#endif
	{
		widget_MMI_on_enter_idle_screen();
	}
	#endif /* WAP_SUPPORT */
	#ifdef __MOD_SMSAL__
		SetMessagesScrnIdToDelHistoryNodes(MESSAGES_SCR_ID_DEFINES_MAX);
		SetMessagesScrnIdDelUpToHistoryNodes(MESSAGES_SCR_ID_DEFINES_MAX);
	#endif
	#endif
    //tangjie del end 20060616


	OslDumpDataInFile();

	/****** This block carries all functions to be called when user enter idle screen first time.*/
	/* 
		idleScreenFirst = 0: Not yet enter idlescreen
				  1: Enter idlescreen has been reached
	*/
	
	if (!IS_IDLE_ARRIVED)
	{
		BOOL needCampOnTonePlay = MMI_FALSE;

		// TODO: 为何要关闭下面的代码?
		 
		g_pwr_context[0].IdleScreenArrived = 1;

		#if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)
		g_DirectMode = FALSE;
		#endif
		
		PendingAlarmReminder();
		StartLEDPatternBackGround();

		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			if(!g_pwr_context[i].IsCampOnTonePlayed && g_pwr_context[i].CurrentServiceType == FULL_SERVICE)
			{
				needCampOnTonePlay = MMI_TRUE;
				break;
			}
		}
		if(needCampOnTonePlay)   
		{
			playRequestedTone(CAMP_ON_TONE);
			for(i = 0; i < MMI_SIM_NUMBER; i++)
			{
				g_pwr_context[i].IsCampOnTonePlayed=1;
			}
		}
		#ifdef __MMI_POC__
		if (g_pwr_context[0].CurrentServiceType == FULL_SERVICE)
		{
			mmi_poc_poweron_reg();
		}
		#endif
	
		if( g_charbat_context.isChargerConnected )
			StartTimer(BOOTUP_CHARGER_DETECT_TIMER, 3000, ChargerDetectTimeoutHdlr );
		
		#ifdef __MMI_USB_SUPPORT__
			PendingUsbDetectionHdlr();
		#endif

		#ifdef WAP_SUPPORT
			MMIProcessActionMsg();	
		#endif

		// TODO: 为何要关闭下面的代码?
 		//idle_notify_bootup_ready();                    //tangjie del because this function related WAP 

		//MMI_notify_bootup_ready();               //tangjie del becuase this function related message
		#ifdef __MMI_IMPS__
		   mmi_imps_notify_idle_reached();
		#endif
	}

/********Check from setting that AutoLock is enabled or not************************/
	if(!gKeyPadLockFlag)
	{
		GetAutoLockTimeOut=GetAutoKeypadLockTime();
		switch(GetAutoLockTimeOut)
		{
			case TIME_0:
				PRINT_INFORMATION( ("[Keypad Lock] EntryIdleScreen: No Keypad Lock") );
				g_idle_context.AutoLockTimeOut=0;
				break;
			case TIME_5:
				g_idle_context.AutoLockTimeOut=5000;
				break;
			case TIME_30:
				g_idle_context.AutoLockTimeOut=30000;
				break;
			case TIME_60:
				g_idle_context.AutoLockTimeOut=60000;
				break;
			case TIME_2:
				g_idle_context.AutoLockTimeOut=300000;
				break;
		}
		
		if(g_idle_context.AutoLockTimeOut!=0)
		{
			PRINT_INFORMATION( ("[Keypad Lock] EntryIdleScreen: Timer %d start", g_idle_context.AutoLockTimeOut) );
			StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, IdleSetKeyPadLockFlag);
		}
	}
/**********************************************************************************/			

	g_idle_context.IsOnIdleScreen = 1;
	g_idle_context.IsOnDialerScreen = 0;
	
	ClearAllKeyHandler();
	mmi_frm_kbd_reg_pre_key_hdlr(IdleScreenEmergencyCallCheckNKeypadLock);

/* This checks if network attach indication has been recieved once after bootup.
   If yes then go to idle screen otherwise show network srch animation*/
   	for(i = 0; i < MMI_SIM_NUMBER; i++)
   	{
   		if(g_pwr_context[i].NetWorkFlag == 1)
   		{
   			NetWorkFlag = 1;
			break;
   		}
   	}
	if(NetWorkFlag == 1)	
	{

#if (CSW_EXTENDED_API_PMD_USB == 0)
	if(g_NeedReDetectUsb)
	{
		g_NeedReDetectUsb = 0;
		if (USB_DETECTED_OK == MCI_USBDetect())
			SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGIN); //chenhe,080607
		else
			BatteryIndicationPopup(STR_CHARGER_CONNECTED);
	}
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
	if(isIdleScreenReached == 0)
	{
		isIdleScreenReached = 1;
	}
/************************ For Screen Saver ***********************************************/
		PhnsetGetScrSvrSetings(&ScreenSaverStatus, &ScreenSaverWait);
		if( ScreenSaverStatus == 0xFF )
		{
			PhnsetRstScrSvrDefault();
		}
		if( ScreenSaverStatus == 1 )	//Screen Saver Is Active
		{
			StartTimer(SCREENSAVER_TIMER, ScreenSaverWait*1000, EntryIdleSetScreenSaver);
		}
/**********************************************************************************/			

		if( gKeyPadLockFlag	== 0)// keypad is not locked
		{
			/* if g_msg_cntx.msg_status is not in idle state, need to help SMS to clear action queue */
			if(mmi_msg_need_clear_action_queue())
			{
				mmi_frm_sms_delete_action_pending();
			}
		#ifdef __J2ME__			
			if(java_is_launch_after_install())
			{
				mmi_java_send_mid_start_req();
			}
			else
		#endif
				if (mmi_idle_missed_call_check())// checks if there is pending mised call.
				{
					EntryCHISTNotifyMissedCall();
				}
				else if (mmi_msg_need_class0_msg_ind())	// checks if there is pending class 0 message
				{
					mmi_msg_entry_class0_msg_ind();
				}
				else if (mmi_msg_need_new_msg_ind())// checks if there is pending new sms ind
				{
					mmi_msg_entry_new_msg_ind();
				}
				else if (mmi_msg_need_mem_exceed_ind()) // checks if there is pending memory exceeded ind
				{
					mmi_msg_entry_mem_exceed_ind();
				}
				else if (mmi_msg_need_delivery_report_ind())
				{
					mmi_msg_entry_deliver_report_ind();
				}
				else if (mmi_msg_need_waiting_ind()) // checks if there is pending waiting msg ind(voicemail, fax, email)
				{
					mmi_msg_entry_msg_waiting_ind();
				}
				#ifdef JATAAYU_SUPPORT
				#ifdef JATAAYU_SUPPORT_MMS
				else if(jmms_NeedNewMMSPopupOnIdle())
				{
					jmms_entry_new_msg_ind();
				}
				#endif
				#ifdef JWAP_PUSH_ENABLE	
                else if(jwap_NeedNewPushPopupOnIdle())
				{
					jwap_New_Push_Msg_Ind();
				}
				#endif
				#ifdef JOTA_PROVISION_APP_ENABLE
				else if(prv_IsProvPushAvail())
				{
					prv_MMIDisplayNewProvAlertScreen();
				}
				#endif //JOTA_PROVISION_APP_ENABLE

				#endif
				#ifdef __MMI_BT_SUPPORT__
				else if (mmi_opp_need_notify_scr_browse())
				{
					mmi_opp_entry_idle_notify_scr();
				}
				#endif
//Renwy add begine 2008/08/18
#if (defined(__MMI_VCARD__) || defined(__MMI_VCALENDAR__)) && defined(MMI_ON_HARDWARE_P)
			else if(mmi_vobj_is_pending_object()) // checks if there is pending object
			{
				mmi_vobj_entry_recv_object();
			}
#endif
//Renwy add end 2008/08/18
		#ifdef __MMI_IMPS__
			else if(mmi_imps_check_invite_in_idle())
			{
				/* dummy */
			}
		#endif

		// TODO: 为何要关闭下面的代码?
		//tangjie del 20060616
		#if 0
		#if (defined(__MMI_VCARD__) || defined(__MMI_VCALENDAR__)) && defined(MMI_ON_HARDWARE_P)
			else if (mmi_vobj_is_pending_object())	//Check if there is incoming vobject.
			{
				mmi_vobj_entry_recv_object();
			}
		#endif
		#endif
		//tangjie del end 20060616
		#if defined(__MMI_IRDA_SUPPORT__)
			else if ( mmi_irda_get_recv_files_cnt() > 0 )	//Check if pending files in received folder.
			{
				mmi_irda_entry_idle_notify_scr();
			}
		#endif
		/* shantanu - for saving themes rcvd through IRDA */
		#if (defined(__MMI_DOWNLOADABLE_THEMES_SUPPORT__) && defined(__MMI_IRDA_SUPPORT__))
			else if(mmi_irda_is_any_theme_file_rcv()==FILE_RECV)
			{
				mmi_irda_theme_show_idle_notify_scr();
			}				
		#endif
		/* shantanu - for saving themes rcvd through IRDA */
		// TODO: 为何要关闭下面的代码?
		#if defined(__MMI_NITZ__)
			else if(g_nitz_cntx.IsNITZPending) // It checks if NITZ indication has been received.
			{
				EntryNitzScreen();
			}
		#endif
			else if (mmi_msg_need_new_cb_ind()) // checks if there is pending CB message pending.
			{
				mmi_msg_entry_new_cb_ind ();
			}
		#ifdef WAP_SUPPORT
		#ifndef __MMI_UI_SMALL_SCREEN_SUPPORT__
			else if (widget_MMI_check_and_display_new_msg())
		#else
			/* If idlescreen is redrawn by small screen mechanism, we should not check new message */
			else if (!is_redrawing_old_screens() && widget_MMI_check_and_display_new_msg())
		#endif 
			{
				/* dummy */
			}
		#endif /* WAP_SUPPORT  */
    #if 0
		#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
			else if(mmi_phb_init_check_sim_change())
			{
				/* dummy */
			}
		#endif
    #endif
		#if defined(__MMI_VRSD_DIAL__)
			else if(mmi_vrsddial_check_sim_change())
			{	
				/* dummy */
			}
		#endif
			else /* Idle screen */
			{

				IdleRefreshServiceIndicationArea();		/* Refresh Service Indication Area */
#ifdef __MMI_MULTI_SIM__
				if (MTPNP_AD_Get_UsableSide_Number() == 0)
				{
					mmi_trace(g_sw_IDLE,"Func: %s STR_RSK_SOS", __FUNCTION__);
					idleRStringtID = STR_RSK_SOS;
					idleLStringtID = IDLE_SCREEN_LSK_TEXT;
//					idleLStringtID = STR_GLOBAL_OPTIONS;
				}
				else
#endif /* __MMI_MULTI_SIM__ */
				{
					mmi_trace(g_sw_IDLE,"Func: %s IDLE_SCREEN_RSK_TEXT", __FUNCTION__);
					idleRStringtID = IDLE_SCREEN_RSK_TEXT;
					idleLStringtID = IDLE_SCREEN_LSK_TEXT;
				}


				#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) && !(defined __MMI_TOUCH_IDLESCREEN_SHORTCUTS__))	 
#ifdef __FLIGHT_MODE_SUPPORT__
				if (mmi_bootup_get_active_flight_mode() != 1)
#endif
					EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
				#endif

				#ifdef __MMI_SWAP_LSK_RSK_IN_IDLE_SCREEN__
					ShowCategory33Screen(idleLStringtID, IDLE_SCREEN_RSK_ICON,
										idleRStringtID,	/* for __MANUAL_MODE_NW_SEL__ */
										IDLE_SCREEN_LSK_ICON,
										NULL);
				#else
				/*+ zhouqin 2011-02-16 modify for q6 320X240 */
				#ifdef __MMI_SYMBIAN_STYLE_IDLE__
				ShowCategorySymbianScreen(
							idleLStringtID, 
							IDLE_SCREEN_LSK_ICON, 
							idleRStringtID, 
							IDLE_SCREEN_RSK_ICON, 
							SYMBIAN_TOTAL_ITEMS, 
							pda_idle_shortcut, 
							0,
							NULL);
				#else
				/*- zhouqin 2011-02-16 modify for q6 320X240 */
					ShowCategory33Screen(idleLStringtID,IDLE_SCREEN_LSK_ICON,
										idleRStringtID, /* for __MANUAL_MODE_NW_SEL__ */
										IDLE_SCREEN_RSK_ICON,
										NULL);
				#endif/*+ zhouqin 2011-02-16 modify for q6 320X240 */
				#endif
				SetGroupKeyHandler(HandleIdleScreenDigitEntry,IdleScreenDigits,MAX_IDLE_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
				g_idle_context.LongPressEnable=0;
				IdleSetLRKey();	

				#ifdef __MMI_TOUCH_SCREEN__
					mmi_wgui_register_status_icon_pen_event_hdlr(STATUS_ICON_KEYPAD_LOCK, WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,IdleSetKeyPadLockFlag);
				#endif

				#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) && !(defined __MMI_TOUCH_IDLESCREEN_SHORTCUTS__))	 
					SetCenterSoftkeyFunction(HandleIdleScreenCSKHandler, KEY_EVENT_DOWN);
				#endif
				
				#ifdef	__MMI_TOUCH_DIAL_SCREEN__
				//BUG DIALING START 20050902
				/*	The dialled number is becoming corrupted because of resetting
					of the buffer used for storing the Dialled number. Now it
					will not be reset when is_redrawing_old_screens() flag is ON.	*/
					#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
						if(is_redrawing_old_screens() != 1)
					#endif
				//BUG DIALING END 20050902
						memset((U8*)g_idle_context.DialPadCallBuffer,0,sizeof((U8*)g_idle_context.DialPadCallBuffer));
				#endif		

				/* Voice Recognition */
				#if defined(__MMI_VRSD__)
					SetKeyHandler(mmi_vrsd_rcg_pre_entry, KEY_RSK, KEY_LONG_PRESS);
				#endif

				#if defined(__MMI_VRSI__)
					SetKeyHandler(mmi_vrsi_rcg_pre_entry, KEY_RSK, KEY_LONG_PRESS);
				#endif

				/* for 4-dedicated shortcuts */
				//tangjie del begin 20060616
				//open the switch by Jinzh:2007-5-14
				#if !defined(__MMI_NOKIA_STYLE_N800__) && !defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)				
				#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
					#if defined(__MMI_MULTI_SIM__) && !defined(__MMI_USE_LSK_AS_SEND2__)
					if(MTPNP_PFAL_Has_CardValid())
					#endif
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_SYMBIAN_STYLE_IDLE__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
					RegisterDedicatedKeyHandlers();
#endif
				#else
					SetKeyHandler(EntryShctInIdle,KEY_UP_ARROW,KEY_EVENT_DOWN);
				#endif
				#endif
				//end:open the switch by Jinzh:2007-5-14
				//tangjie del end 20060616

				/* sync the behavior with DisplayIdleScreen */
				CHISTResetCallLogIndex();

				#ifdef __MMI_KLG__
					SetKeyHandler(fmgr_launch,KEY_EXTRA_1,KEY_EVENT_UP);
					SetKeyHandler(ClndrPreEntryApp,KEY_EXTRA_2,KEY_EVENT_UP);
				#endif
#if defined(__PROJECT_GALLITE_C01__)
                #ifdef __MUSIC_KEY_SUPPORT__
                    SetKeyHandler(mmi_audply_entry_main,KEY_MUSIC,KEY_EVENT_UP);
                #endif
                #if defined(__MMI_SUPPORT_JME__) || defined(__J2ME__)
                {
                	#if	0
                	#ifndef	MMI_ON_WIN32
                	extern void jmetoc_qq_entry();
                    SetKeyHandler(jmetoc_qq_entry,KEY_QQ,KEY_EVENT_UP);
					#endif
					#else
					//extern void mmi_msg_pre_entry_inbox_list (void);
					extern void mmi_msg_entry_divided_inbox_menu(void);
					extern void mmi_msg_pre_entry_multi_inbox_list(void);
					//SetKeyHandler(mmi_msg_pre_entry_inbox_list,KEY_QQ,KEY_EVENT_UP);
					
					if(MTPNP_AD_Get_UsableSide_Number() > 1)
					{
						SetKeyHandler(mmi_msg_entry_divided_inbox_menu,KEY_QQ,KEY_EVENT_UP);
					}
					else if(MTPNP_AD_Get_UsableSide_Number() == 1)
					{
						SetKeyHandler(mmi_msg_pre_entry_multi_inbox_list,KEY_QQ,KEY_EVENT_UP);
					}						
					#endif
                }
                #endif
#endif
					/* sync the behavior with DisplayIdleScreen */
					CHISTResetCallLogIndex();

#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__
					SetKeyHandler(EntryCHISTTabPaneIdleScreen, KEY_SEND, KEY_EVENT_DOWN);  

#else
		#ifdef __MMI_MULTI_SIM__
				if(MTPNP_AD_Get_UsableSide_Number() > 0) 
				{
					SetKeyHandler(MTPNP_PFAL_ReadDualMixedCallLog, KEY_SEND, KEY_EVENT_DOWN);   
#if defined(TGT_GALLITE_G800)

					SetKeyHandler(MTPNP_PFAL_ReadDualMixedCallLog,KEY_SEND1,KEY_EVENT_DOWN);
					SetKeyHandler(MTPNP_PFAL_ReadDualMixedCallLog,KEY_SEND2,KEY_EVENT_DOWN);
#endif
				}
			#if(defined __SENDKEY2_SUPPORT__ &&(MMI_SIM_NUMBER == 2))
				#if !defined(__PROJECT_GALLITE_C01__)
				if ((MTPNP_AD_Get_UsableSide() ==MTPNP_AD_DUALSIM_USABLE) ||(MTPNP_AD_Get_UsableSide() ==MTPNP_AD_SIMCARD1_USABLE))
				{
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM1, KEY_SEND1, KEY_EVENT_DOWN);
				}
				else
				{
					SetKeyHandler(MTPNP_PFAL_CC_Sim1Invaild, KEY_SEND1, KEY_EVENT_DOWN);
				}
				if ((MTPNP_AD_Get_UsableSide() == MTPNP_AD_DUALSIM_USABLE) || (MTPNP_AD_Get_UsableSide() == MTPNP_AD_SIMCARD2_USABLE))
				{
				#ifdef __MMI_USE_LSK_AS_SEND2__
					SetLeftSoftkeyFunction(DummyKeyHandler,KEY_EVENT_UP); 
					SetLeftSoftkeyFunction(CHISTGetCallLogBySENDKey_SIM2,KEY_EVENT_DOWN); 
                            #endif
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM2, KEY_SEND2, KEY_EVENT_DOWN);
				}
				else
				{
				//	SetKeyHandler(MTPNP_PFAL_CC_Sim1Invaild, KEY_SEND1, KEY_EVENT_DOWN);
				#ifdef __MMI_USE_LSK_AS_SEND2__
                                    SetLeftSoftkeyFunction(MTPNP_PFAL_CC_Sim2Invaild, KEY_EVENT_DOWN);
                            #endif
					SetKeyHandler(MTPNP_PFAL_CC_Sim2Invaild, KEY_SEND2, KEY_EVENT_DOWN);
				}
				#else
				if(MTPNP_AD_Get_UsableSide() == MTPNP_AD_DUALSIM_USABLE)
				{
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM1, KEY_SEND1, KEY_EVENT_DOWN);
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM2, KEY_SEND2, KEY_EVENT_DOWN);
				}
				else if(MTPNP_AD_Get_UsableSide() == MTPNP_AD_SIMCARD1_USABLE)
				{
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM1, KEY_SEND1, KEY_EVENT_DOWN);
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM1, KEY_SEND2, KEY_EVENT_DOWN);
				}
				else if(MTPNP_AD_Get_UsableSide() == MTPNP_AD_SIMCARD2_USABLE)
				{
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM2, KEY_SEND1, KEY_EVENT_DOWN);
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM2, KEY_SEND2, KEY_EVENT_DOWN);
				}
				else
				{
					SetKeyHandler(MTPNP_PFAL_CC_Sim1Invaild, KEY_SEND1, KEY_EVENT_DOWN);
					SetKeyHandler(MTPNP_PFAL_CC_Sim2Invaild, KEY_SEND2, KEY_EVENT_DOWN);
				}
				#endif
            #elif (MMI_SIM_NUMBER == 3)
            {
                extern void MTPNP_PFAL_CC_Sim1Invaild();
                extern void MTPNP_PFAL_CC_Sim1Invaild();
                extern void MTPNP_PFAL_CC_Sim2Invaild();
                extern void MTPNP_PFAL_CC_Sim3Invaild();
                    
                if (MTPNP_AD_Get_CardStatus(0)==MTPNP_AD_SIMCARD_VALID)
                    SetKeyHandler(CHISTGetCallLogBySENDKey_SIM1, KEY_SEND1, KEY_EVENT_DOWN);
                else
                    SetKeyHandler(MTPNP_PFAL_CC_Sim1Invaild, KEY_SEND1, KEY_EVENT_DOWN);
                if (MTPNP_AD_Get_CardStatus(1)==MTPNP_AD_SIMCARD_VALID)
				    SetKeyHandler(CHISTGetCallLogBySENDKey_SIM2, KEY_SEND2, KEY_EVENT_DOWN);
                else
                    SetKeyHandler(MTPNP_PFAL_CC_Sim2Invaild, KEY_SEND2, KEY_EVENT_DOWN);
                if (MTPNP_AD_Get_CardStatus(2)==MTPNP_AD_SIMCARD_VALID)
                    SetKeyHandler(CHISTGetCallLogBySENDKey_SIM3, KEY_SEND3, KEY_EVENT_DOWN);
                else
                    SetKeyHandler(MTPNP_PFAL_CC_Sim3Invaild, KEY_SEND3, KEY_EVENT_DOWN);
            }
            #endif /* __SENDKEY2_SUPPORT__ */
		#else  /* No Defined __MMI_MULTI_SIM__ */
				SetKeyHandler(CHISTGetCallLogBySENDKey, KEY_SEND, KEY_EVENT_DOWN);  
		#endif /* __MMI_MULTI_SIM__ */
#endif

#if defined(WAP_SUPPORT) && defined(__MMI_WITH_WAP_KEY__)
					SetKeyHandler(wap_internet_key_hdlr, KEY_WAP, KEY_LONG_PRESS);

#endif

				/* camera fast access key */
				#ifdef __MMI_CAMERA__

					#ifndef __MMI_SCREEN_SNAPSHOT__ 
					
						#ifdef __MMI_IDLE_CAMERA_KEY_TO_STOP_AUDIOPLAYER__
							SetKeyHandler(CheckAndStopAudioPlayer, CAMERA_SHORTCUT_KEY, KEY_EVENT_DOWN);
						#else
                                        #ifdef __PRJ_WITH_SPICAM__
                                            SetKeyHandler(mmi_camera_entry_option_menu_screen, CAMERA_SHORTCUT_KEY, KEY_EVENT_DOWN);
                                        #else
                                            SetKeyHandler(mmi_camera_entry_app_screen, CAMERA_SHORTCUT_KEY, KEY_EVENT_DOWN);
                                        #endif
						#endif
						
					#else 
					
						#ifdef __MMI_IDLE_CAMERA_KEY_TO_STOP_AUDIOPLAYER__
							SetKeyHandler(CheckAndStopAudioPlayer, CAMERA_SHORTCUT_KEY, KEY_EVENT_UP);
						#else
                                        #ifdef __PRJ_WITH_SPICAM__
                                            SetKeyHandler(mmi_camera_entry_option_menu_screen, CAMERA_SHORTCUT_KEY, KEY_EVENT_UP);
                                        #else
				 			SetKeyHandler(mmi_camera_entry_app_screen, CAMERA_SHORTCUT_KEY, KEY_EVENT_UP);
                                        #endif
						#endif
						
					#endif /* __MMI_SCREEN_SNAPSHOT__ */
					
				#endif  /* __MMI_CAMERA__ */
				

				ForceSubLCDScreen(ShowIdleAppSubLcd);
				
				/* 
					Compensatory key handler registration for screen saver
					The idea here is to set those keys which are not set with handlers to a dummy handler. 
					But not that END key is still being set a handler (DisplayIdleScreen)  for KEY_EVENT_DOWN in PowerAndEndKeyHandler. 
				*/
				if( ScreenSaverStatus == 1 )	/* Screen Saver Is Active */
				{
					for (i=KEY_0; i<MAX_KEYS; i++)
					{
						dummyKeyHandler[i] = GetKeyHandler(i, KEY_EVENT_DOWN);
						SetKeyHandler(IdleScreenDummyKeyHandler, i, KEY_EVENT_DOWN);
					}	
				}
			}
		}
		else /*  Keyapd is locked */
		{
			U16 KeyLockLSKStringtID=0, KeyLockRSKStringID=0, KeyLockLSKIconID=0, KeyLockRSKIconID=0;

			 
			#ifndef __MMI_KEYPADLOCK_WITH_KEYTONE__
				mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
			#endif

			#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
				#ifdef __MMI_TOUCH_SCREEN__
					KeyLockRSKStringID = SCREEN_LOCKED_TEXT;
				#else
					KeyLockRSKStringID = KEYPAD_LOCKED_RSK_TEXT;
				#endif
				KeyLockRSKIconID = 0;
			#endif

			#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__
				#ifdef __MMI_TOUCH_SCREEN__
					KeyLockLSKStringtID = SCREEN_LOCKED_TEXT;
				#else
					KeyLockLSKStringtID= KEYPAD_LOCKED_RSK_TEXT;
				#endif
				KeyLockLSKIconID = 0;
			#endif

				/* if g_msg_cntx.msg_status is not in idle state, need to help SMS to clear action queue */
				if(mmi_msg_need_clear_action_queue())
				{
					mmi_frm_sms_delete_action_pending();
				}
			#ifdef __J2ME__			
				if(java_is_launch_after_install())
				{
					mmi_java_send_mid_start_req();
				}
				else
			#endif
			/* 
					During keyapad lock if Missed call, NITZ or any Message received 
					then screen will be changed to following.RSK will be 'Unlock' and
					instead of idle screen it will display information abt the indication received.
			*/
			if(mmi_idle_missed_call_check())
			{
				EntryCHISTNotifyMissedCall();
			}
			else if(mmi_msg_need_class0_msg_ind())	/* CLASS0 SMS IND */
			{
				mmi_msg_entry_class0_msg_ind_when_kpl( KeyLockLSKStringtID, KeyLockLSKIconID, KeyLockRSKStringID, KeyLockRSKIconID);
                                
			}
			else if(mmi_msg_need_new_msg_ind())	/* NEW SMS IND */
			{
				mmi_msg_entry_new_msg_ind();
			}
//Renwy add begine 2008/08/18
#if (defined(__MMI_VCARD__) || defined(__MMI_VCALENDAR__)) && defined(MMI_ON_HARDWARE_P)
			else if(mmi_vobj_is_pending_object()) // checks if there is pending object
			{
				mmi_vobj_entry_recv_object();
			}
#endif
//Renwy add end 2008/08/18
			else if (mmi_msg_need_mem_exceed_ind())
			{
				ShowCategory2Screen(	STR_SCR6042_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
							KeyLockLSKStringtID, KeyLockLSKIconID, 
							KeyLockRSKStringID, KeyLockRSKIconID,
							STR_SMS_MEMORY_EXCEEDED, NULL);
			}
			else if (mmi_msg_need_delivery_report_ind())
			{
				U16 drStatusIconID=0;
				U16 drStatusStrID=0;
				U32 drStatus=0;

				mmi_msg_get_deliver_report_status(&drStatus, &drStatusIconID, &drStatusStrID);
				ShowCategory145Screen( ((PU8)GetString(STR_STATUS_REPORT_MESSAGE_DELIVERD_MSG)), IMG_SMS_ENTRY_SCRN_CAPTION,
							KeyLockLSKStringtID, KeyLockLSKIconID, 
							KeyLockRSKStringID, KeyLockRSKIconID,
							drStatusIconID, ((PU8) GetString (drStatusStrID)),
							mmi_msg_get_deliver_report_msg(), drStatus, NULL);
			}
			else if(mmi_msg_need_waiting_ind())	/* WAITING MSG(voicemail, fax, page and e-mail) IND */
			{
				ShowCategory141Screen(	0, 0, 
							KeyLockLSKStringtID, KeyLockLSKIconID, 
							KeyLockRSKStringID, KeyLockRSKIconID,
							(PU8)mmi_msg_get_msg_waiting_msg (), IMG_NEW_VOICEMAIL_NOTIFICATION_MSG,
							NULL);
			}
			#ifdef JATAAYU_SUPPORT
			#ifdef JATAAYU_SUPPORT_MMS
			else if(jmms_NeedNewMMSPopupOnIdle())
			{
				jmms_entry_new_msg_ind();
			}
			#endif
			#ifdef JWAP_PUSH_ENABLE	
			else if(jwap_NeedNewPushPopupOnIdle())
			{
				jwap_New_Push_Msg_Ind();
			}
			#endif 
			#ifdef JOTA_PROVISION_APP_ENABLE
			else if(prv_IsProvPushAvail())
			{
				prv_MMIDisplayNewProvAlertScreen();
			}
			#endif //JOTA_PROVISION_APP_ENABLE
			#endif
			#ifdef __MMI_IMPS__
			else if(mmi_imps_check_invite_key_locked(KeyLockLSKIconID, KeyLockLSKStringtID, KeyLockRSKIconID, KeyLockRSKStringID))
			{
				/* dummy */
			}
			#endif /* __MMI_IMPS__ */
			#if defined(__MMI_IRDA_SUPPORT__)
			else if ( mmi_irda_get_recv_files_cnt() > 0 )	/* Check if pending files in received folder. */
			{
				mmi_irda_entry_idle_notify_scr_keylock(KeyLockLSKIconID, KeyLockLSKStringtID,
							   									KeyLockRSKIconID, KeyLockRSKStringID );
			}
			#endif /* __MMI_IRDA_SUPPORT__ */
			
			/* shantanu - for saving themes rcvd through IRDA */
			#if (defined(__MMI_DOWNLOADABLE_THEMES_SUPPORT__) && defined(__MMI_IRDA_SUPPORT__))
			else if(mmi_irda_is_any_theme_file_rcv()==FILE_RECV)
					{
					mmi_irda_theme_show_idle_notify_scr_keylock(\
					KeyLockLSKIconID, KeyLockLSKStringtID,\
					KeyLockRSKIconID, KeyLockRSKStringID );
					}				
			#endif
			// TODO: 为何要关闭下面的代码?
		#if defined(__MMI_NITZ__)
			else if(g_nitz_cntx.IsNITZPending) // It checks if NITZ indication has been received.
			{
				EntryNitzScreen();
			}
		#endif
			else if (mmi_msg_need_new_cb_ind())	/* CB-SMS IND */
			{
		    	mmi_msg_entry_new_cb_ind_when_kpl(KeyLockLSKStringtID, KeyLockLSKIconID, KeyLockRSKStringID, KeyLockRSKIconID);
			}
			#ifdef WAP_SUPPORT
			else if (widget_MMI_check_and_display_new_msg_when_keypad_lock(KeyLockLSKStringtID, KeyLockLSKIconID, KeyLockRSKStringID, KeyLockRSKIconID))
			{
				/*  Dummy */
			}
			#endif
			else
			{
				IdleRefreshServiceIndicationArea();
				/*+ zhouqin 2011-02-16 modify for q6 320X240 */
				#ifdef __MMI_SYMBIAN_STYLE_IDLE__
				ShowCategorySymbianScreen(
							KeyLockLSKStringtID, 
							KeyLockLSKIconID, 
							KeyLockRSKStringID, 
							KeyLockRSKIconID, 
							SYMBIAN_TOTAL_ITEMS, 
							pda_idle_shortcut, 
							0,
							NULL);
				#else
				/*- zhouqin 2011-02-16 modify for q6 320X240 */
				ShowCategory33Screen(KeyLockLSKStringtID, KeyLockLSKIconID, 
											KeyLockRSKStringID, KeyLockRSKIconID,
											NULL);
				#endif/*+ zhouqin 2011-02-16 modify for q6 320X240 */
			}
			
			#if 0 //键盘锁时，按任意键都不响应 //deleted by zhoumn@2007/07/20
				#if defined(__MMI_KEYPAD_LOCK_PROMPT_TEXT__)
					SetGroupKeyHandler(EntryScrKeyPadLockOnPessingAnyKey,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,(U16)KEY_EVENT_DOWN);
				#endif
			#endif
			
			#ifdef __MMI_TOUCH_SCREEN__
			mmi_pen_block();  
			#ifndef __MMI_KEYPAD_LOCK_PROMPT_TEXT__
			ClearKeyHandler(KEY_LSK,KEY_EVENT_DOWN);
			ClearKeyHandler(KEY_LSK,KEY_EVENT_UP);
			ClearKeyHandler(KEY_RSK,KEY_EVENT_DOWN);
			ClearKeyHandler(KEY_RSK,KEY_EVENT_UP);
			#endif
			//SetKeyHandler(EntryScrAfterPessingRSK,KEY_SEND,KEY_EVENT_DOWN); //解决自动键盘锁不能解锁的问题//deleted by zhoumn @2007/06/29
			#else
			#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
				SetRightSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_DOWN);
			#endif

			#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__ 
				SetLeftSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_DOWN);
			#endif
			#endif /* __MMI_TOUCH_SCREEN__ */

			IdleSetLRKey(); /* __MANUAL_MODE_NW_SEL__ */

			if(g_idle_context.IsRSKPressOnScreenSaver)
			{
			#if defined(__MMI_MAINLCD_220X176__) && defined(__MMI_TOUCH_SCREEN__)
				mmi_e71_idle_lsk_down_info();
			#else
				EntryScrAfterPessingRSK();
			#endif				
				g_idle_context.IsRSKPressOnScreenSaver=0;
			}

			/* 
				Compensatory key handler registration for screen saver
				The idea here is to set those keys which are not set with handlers to a dummy handler. 
				But note that END key is still being set a handler (DisplayIdleScreen)  for KEY_EVENT_DOWN in PowerAndEndKeyHandler. 
			*/
			if(ScreenSaverStatus == 1)	/* Screen Saver Is Active */
			{
				for(i=KEY_0; i<MAX_KEYS; i++)
				{
					if( (GetKeyHandler(i,KEY_EVENT_DOWN) == NULL) && (GetKeyHandler(i,KEY_EVENT_UP) == NULL) )
						SetKeyHandler(IdleScreenDummyKeyHandler, i, KEY_EVENT_DOWN);
				}
				/*  add this because of PowerAndEndKeyHandler */
				#ifndef __MMI_KEYPAD_LOCK_PROMPT_TEXT__
					 
					SetKeyHandler(IdleScreenDummyKeyHandler, KEY_END, KEY_EVENT_DOWN);
				#endif
			}
		}

		#if defined( __MMI_DATE_TIME_SETTING_REMINDER__)		
  			#ifdef MMI_ON_HARDWARE_P
				if(!datejust_reminder_shown && RTC_is_MS_FirstPowerOn())
			#else
				if(!datejust_reminder_shown)
			#endif
				{
					DisplayPopup((PU8)GetString(STR_DATE_TIME_UPDATE_REMINDER), IMG_GLOBAL_OK, 0, 2000, 0);
					datejust_reminder_shown = TRUE;
				}
		#endif /* __MMI_DATE_TIME_SETTING_REMINDER__ */
		
	}
	else /*  displays network seaching animation */
	{
	
		if(!gIsOnSearchingScreen)
		{
			StartTimer(SEARCHING_SCREEN_TIMER, 10000, SearchingSrceenTimeOutHandler );
			gIsOnSearchingScreen = 1;
		}
	
		if(gKeyPadLockFlag	== 0)
		{
			#ifdef __MMI_SWAP_LSK_RSK_IN_IDLE_SCREEN__                  
				ShowCategory120Screen(IDLE_SCREEN_RSK_TEXT,0,IDLE_SCREEN_LSK_TEXT,0,STR_IDLE_NETWORK_SRCH_SCR,IMG_IDLE_NETWORK_SRCH_SCR,NULL);
			#else
#if (0) //defined(__MMI_BLACKBERRY_QWERTY__)
				ShowCategory120Screen(IDLE_SCREEN_LSK_TEXT,0,IDLE_SCREEN_RSK_TEXT,0,STR_IDLE_NETWORK_SRCH_SCR,0,NULL);	
#else
				ShowCategory120Screen(IDLE_SCREEN_LSK_TEXT,0,IDLE_SCREEN_RSK_TEXT,0,STR_IDLE_NETWORK_SRCH_SCR,IMG_IDLE_NETWORK_SRCH_SCR,NULL);
#endif
			#endif
			SetGroupKeyHandler(HandleIdleScreenDigitEntry,IdleScreenDigits,MAX_IDLE_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
			g_idle_context.LongPressEnable=0;

			#ifdef __MMI_SWAP_LSK_RSK_IN_IDLE_SCREEN__                  
				SetRightSoftkeyFunction(EntryMainMenuFromIdleScreen,KEY_EVENT_UP);
				SetLeftSoftkeyFunction(mmi_phb_idle_enter_phb_list,KEY_EVENT_UP);
			#else
				SetLeftSoftkeyFunction(EntryMainMenuFromIdleScreen,KEY_EVENT_UP); 
				SetRightSoftkeyFunction(mmi_phb_idle_enter_phb_list,KEY_EVENT_UP);
			#endif

			/* Voice Recognition */
			#if defined(__MMI_VRSD__)
				SetKeyHandler(mmi_vrsd_rcg_pre_entry, KEY_RSK, KEY_LONG_PRESS);
			#endif

			#if defined(__MMI_VRSI__)
				SetKeyHandler(mmi_vrsi_rcg_pre_entry, KEY_RSK, KEY_LONG_PRESS);
			#endif

			// TODO: 为何要关闭下面的代码?
			//tangjie del begin 20060616
			#if 0
			#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
				RegisterDedicatedKeyHandlers();
			#else
				SetKeyHandler(EntryShctInIdle,KEY_UP_ARROW,KEY_EVENT_DOWN);
			#endif
			#endif
			//tangjie del end 20060616
			
			#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
				SetKeyHandler(EntryCHISTTabPaneIdleScreen,KEY_SEND,KEY_EVENT_DOWN);  
			#else
#ifdef __MMI_MULTI_SIM__
#ifdef TGT_GALLITE_G800
			        SetKeyHandler(MTPNP_PFAL_ReadDualMixedCallLog, KEY_SEND1, KEY_EVENT_DOWN);
			        SetKeyHandler(MTPNP_PFAL_ReadDualMixedCallLog, KEY_SEND2, KEY_EVENT_DOWN);

#else
//#define TGT_TD_4SIM
#ifdef  TGT_TD_4SIM
			        SetKeyHandler(MTPNP_PFAL_ReadDualMixedCallLog, KEY_SEND1, KEY_EVENT_DOWN);
			        SetKeyHandler(MTPNP_PFAL_ReadDualMixedCallLog, KEY_SEND2, KEY_EVENT_DOWN);
			        SetKeyHandler(MTPNP_PFAL_ReadDualMixedCallLog, KEY_CAMERA, KEY_EVENT_DOWN);
#else


			    if (MTPNP_AD_Get_UsableSide_Number() > 0) 
			    {
			        SetKeyHandler(MTPNP_PFAL_ReadDualMixedCallLog, KEY_SEND, KEY_EVENT_DOWN);   
			    }
		#if(defined __SENDKEY2_SUPPORT__ &&(MMI_SIM_NUMBER == 2))
				if(MTPNP_AD_Get_UsableSide() == MTPNP_AD_DUALSIM_USABLE)
				{
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM1, KEY_SEND1, KEY_EVENT_DOWN);
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM2, KEY_SEND2, KEY_EVENT_DOWN);
				}
				else if(MTPNP_AD_Get_UsableSide() == MTPNP_AD_SIMCARD1_USABLE)
				{
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM1, KEY_SEND1, KEY_EVENT_DOWN);
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM1, KEY_SEND2, KEY_EVENT_DOWN);
				}
				else if(MTPNP_AD_Get_UsableSide() == MTPNP_AD_SIMCARD2_USABLE)
				{
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM2, KEY_SEND1, KEY_EVENT_DOWN);
					SetKeyHandler(CHISTGetCallLogBySENDKey_SIM2, KEY_SEND2, KEY_EVENT_DOWN);
				}
				else
				{
					SetKeyHandler(MTPNP_PFAL_CC_Sim1Invaild, KEY_SEND1, KEY_EVENT_DOWN);
					SetKeyHandler(MTPNP_PFAL_CC_Sim2Invaild, KEY_SEND2, KEY_EVENT_DOWN);
				}
		#endif /* __SENDKEY2_SUPPORT__ */
#endif        
#endif
#else  /* No Defined __MMI_MULTI_SIM__ */
				SetKeyHandler(CHISTGetCallLogBySENDKey, KEY_SEND, KEY_EVENT_DOWN);  
#endif /* __MMI_MULTI_SIM__ */

			#endif

			#if defined(WAP_SUPPORT) && defined(__MMI_WITH_WAP_KEY__)
				SetKeyHandler(wap_internet_key_hdlr,KEY_WAP,KEY_LONG_PRESS);
			#endif

			ForceSubLCDScreen(ShowIdleAppSubLcd);
		}
		else
		{
			ShowCategory120Screen(0,0,KEYPAD_LOCKED_RSK_TEXT,0,STR_IDLE_NETWORK_SRCH_SCR,IMG_IDLE_NETWORK_SRCH_SCR,NULL);
			#ifdef __MMI_TOUCH_SCREEN__
				mmi_pen_block();  
				SetKeyHandler(EntryScrAfterPessingRSK,KEY_SEND,KEY_EVENT_DOWN);
			#else
				SetRightSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_DOWN);
			#endif
		}
      
		#if defined( __MMI_DATE_TIME_SETTING_REMINDER__)           
			#ifdef MMI_ON_HARDWARE_P
				if(!datejust_reminder_shown && RTC_is_MS_FirstPowerOn())
			#else
				if(!datejust_reminder_shown)
			#endif
				{
					DisplayPopup((PU8)GetString(STR_DATE_TIME_UPDATE_REMINDER), IMG_GLOBAL_OK, 0, 2000, 0);
					datejust_reminder_shown = TRUE;
				}
		#endif /* __MMI_DATE_TIME_SETTING_REMINDER__ */
	}

	if(GetPostponedSATScreen()) 
	{
		(*GetPostponedSATScreen())();
		ClearPostponedSATScreen();
	}
#ifdef __MMI_TOUCH_SCREEN__
    mmi_pen_enable_quick_launch( );
    #ifdef __MMI_MULTI_SIM__
    if(!mmi_frm_sms_is_sms_valid())
        mmi_pen_set_shortcut_entry(SHORTCUT_MESSAGES, DisplaySMSNotUsableWarn);
    else
    #endif
#ifdef __MMI_MESSAGES_MULTI_APP__
    mmi_pen_set_shortcut_entry(SHORTCUT_MESSAGES, EntrySMSMainMenuList);
#endif
#endif


#ifdef __MMI_MAINLCD_220X176__
    #ifdef __MMI_MULTI_SIM__
    if(!mmi_frm_sms_is_sms_valid())
	 SetKeyHandler(DisplaySMSNotUsableWarn, KEY_SMS, KEY_EVENT_UP);
    else
    #endif
#if defined(__MMI_MAINLCD_220X176__)
	if(!gKeyPadLockFlag)
#endif
	SetKeyHandler(EntrySMSMainMenuList, KEY_SMS, KEY_EVENT_UP);
	
	SetKeyHandler(EntryDMCHISTMainMenu, KEY_CLOG, KEY_EVENT_UP);
#if (defined __MMI_MAINLCD_220X176__)
		SetKeyHandler(NULL,KEY_POUND,KEY_EVENT_DOWN);
		SetKeyHandler(HandleIdleScreenDigitEntry,KEY_POUND,KEY_EVENT_UP);
		SetKeyHandler(IdleToggleProfile,KEY_POUND,KEY_LONG_PRESS);
#endif
#endif

#ifndef __MMI_NOKIA_STYLE_N800__
    if (gKeyPadLockFlag	== 0)// keypad is not locked
    {
#ifdef	__MMI_FM_RADIO__
        SetKeyHandler(mmi_fmrdo_run_app, KEY_FM, KEY_EVENT_UP);
#endif
        SetKeyHandler(mmi_audply_entry_main, KEY_MUSIC, KEY_EVENT_UP);
    }
#endif

#if defined __MMI_OPP_SUPPORT__ 
	if ( g_NeedRecheckOppRecv && MMI_OPP_CTX(cur_recv_file_cnt) > 0)
	{
	     	mmi_opp_scr_recv_obj_ind();
		g_NeedRecheckOppRecv = 0;
       }
#endif

#ifdef __MMI_ANALOGTV__
#ifndef TGT_TD_4SIM 
       ClearKeyHandler(KEY_CAMERA , KEY_EVENT_DOWN );
       ClearKeyHandler(KEY_CAMERA , KEY_EVENT_UP );
       ClearKeyHandler(KEY_CAMERA ,KEY_EVENT_REPEAT);
       ClearKeyHandler(KEY_CAMERA , KEY_EVENT_LONG_PRESS);
       if(gKeyPadLockFlag == 0)
       {
                 extern void mmi_analogtv_entry_app();
	     	SetKeyHandler(mmi_analogtv_entry_app , KEY_CAMERA, KEY_EVENT_UP );
       }
#endif
#endif

#if defined(__MMI_PROJECT_GS_HK603__)

       ClearKeyHandler(KEY_MUSIC , KEY_EVENT_DOWN );
       ClearKeyHandler(KEY_MUSIC , KEY_EVENT_UP );
       ClearKeyHandler(KEY_MUSIC ,KEY_EVENT_REPEAT);
       ClearKeyHandler(KEY_MUSIC , KEY_EVENT_LONG_PRESS);
       if(gKeyPadLockFlag == 0)
       {
                 extern void mmi_audply_entry_main();
	     	SetKeyHandler(mmi_audply_entry_main , KEY_MUSIC, KEY_EVENT_UP );
       }

#endif
#ifdef __MMI_TTS_FEATURES__ 
       	SetKeyHandler(ejTTSPlayTime, KEY_ENTER, KEY_LONG_PRESS);
#endif

#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
	CSD_UI_get_old_window();
#endif

		TBM_EXIT(0x28D4);
}

/**************************************************************

	FUNCTION NAME		: ExitIdleScreen

  	PURPOSE				: exit fn for idle screen 
						  

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil


	RETURNS				: void

 

**************************************************************/
 #ifdef __MMI_ANIMATION_DISPALY__
 extern void dm_animation_display_timer_func(viod);
 #endif
void ExitIdleScreen(void)
{
	history_t h;
	S16		nHistory =	0;
	pfnUnicodeStrcpy ((S8*)h.inputBuffer, (S8*)&nHistory);
	h.scrnID=IDLE_SCREEN_ID;
	h.entryFuncPtr=EntryIdleScreen;
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);

	g_idle_context.IsOnIdleScreen = 0;

	mmi_frm_kbd_reg_pre_key_hdlr(NULL);
        
	if( !g_idle_context.ScreenSaverRunFlag && !g_idle_context.IsOnIdleScreen)
	{
         PRINT_INFORMATION( ("[Keypad Lock] ExitIdleScreen: Timer %d Stop", g_idle_context.AutoLockTimeOut) );
		StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
	}

	StopTimer(SCREENSAVER_TIMER);
	StopTimer(KEYPADUNLOCKEDMESSAGE_TIMER);

 
#ifdef __MMI_TOUCH_SCREEN__
	if(gKeyPadLockFlag == 1)
		mmi_pen_unblock();  

	mmi_wgui_reset_status_icon_pen_event_hdlr();
#endif

#if defined(CAM_MD_SUPPORT)
 csd_stop_sw_motionsensor();
#endif
 
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
	ResetCenterSoftkey();
#endif
 #ifdef __MMI_ANIMATION_DISPALY__
coolsand_UI_cancel_timer(dm_animation_display_timer_func);
 #endif
}

#if defined(__PROJECT_GALLITE_C01__)
/*****************************************************************************
 * FUNCTION
 *  mmi_idle_remove_dialer_screen_exit_handler
 * DESCRIPTION
 *  This function removes the dialer screen from history. This function handles
 *  both the normal dialer screen and the SIM error dialer screen. The function
 *  can be called before or after the application shows their screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_idle_remove_dialer_screen_exit_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GetActiveScreenId() == IDLE_SCREEN_DIGIT_HANDLER_ID)
    {
        //SetCurrExitFuncPtr(NULL);
        g_idle_context.IsOnDialerScreen = 0;
        g_idle_context.IdleScreenDigitKeyCode = -1;
    }
    else if (GetActiveScreenId() == SCR_DIAL_SOS_NUM)
    {
        //SetCurrExitFuncPtr(NULL);
        g_idle_context.IsOnSimErrorDialerScreen = 0;
        g_idle_context.IdleScreenDigitKeyCode = -1;
    }
    else
    {
        DeleteScreenIfPresent(IDLE_SCREEN_DIGIT_HANDLER_ID);
        DeleteScreenIfPresent(SCR_DIAL_SOS_NUM);
    }
}
#endif

/*****************************************************************************
* FUNCTION
*	mmi_idle_check_nw_provider_name_length
* DESCRIPTION
*	Check if the input string length is larger than main lcd width
* PARAMETERS
*	U8*
* RETURNS
*	pBOOL
* GLOBALS AFFECTED
*	
*****************************************************************************/
extern void UI_set_font(UI_font_type f);
pBOOL mmi_idle_check_nw_provider_name_length(U8 *unicode_string)
{
	pBOOL rtn;
	S32 StringWidth, StringHeight;
	stFontAttribute prev_f;

	/* copy the system font to backup */
	memcpy( &prev_f, UI_font, sizeof(stFontAttribute) );

	/* set default font for length calculation */
	UI_set_font(&UI_DEFAULT_FONT);

	Get_StringWidthHeight(unicode_string, &StringWidth, &StringHeight);

	/* check the string length if it is larger than main lcd width */
	if( (StringWidth+1) >= MAIN_LCD_device_width - (ICON_WIDTH<<1) )
		rtn = MMI_FALSE;
	else
		rtn = MMI_TRUE;

	/* restore the system font */
	UI_set_font(&prev_f);
	return rtn;
}
#ifdef __MMI_MULTI_SIM__
static BOOL MTPNP_PFAL_Is_FlightMode(void)
{
	return MTPNP_AD_Is_Flight_Mode();
}
static void MTPNP_PFAL_Show_Idle_CardClosed(U8 nSimID)
{
	MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[nSimID], (MTPNP_CHAR *) GetString(STRING_MTPNP_CARD1_CLOSED+nSimID));
	SetLine1Text(gNWProviderName[nSimID], nSimID);
}

static void MTPNP_PFAL_Show_Idle_CardAbsent(U8 nSimID)
{
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[nSimID], (MTPNP_CHAR *) GetString(STR_SIM_INSERTION_MSG));
#else
	MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[nSimID], (MTPNP_CHAR *) GetString(STRING_MTPNP_CARD1ABSENT+nSimID));
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
	SetLine1Text(gNWProviderName[nSimID], nSimID);
}

static void MTPNP_PFAL_Show_Idle_CardBlocked(U8 nSimID)
{
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[nSimID], (MTPNP_CHAR *) GetString(STR_SIM_BLOCK_MSG));
#else
	MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[nSimID], (MTPNP_CHAR *) GetString(STRING_MTPNP_CARD1_SIM_BLOCK+nSimID));
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
	SetLine1Text(gNWProviderName[nSimID], nSimID);
}

void MTPNP_PFAL_Show_Idle_NW_Register(U8 nSimID)
{
	MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[nSimID], (MTPNP_CHAR *) GetString(STRING_MTPNP_SEARCHING_NW));
	SetLine1Text(gNWProviderName[nSimID], nSimID);
}
#endif

/**************************************************************

	FUNCTION NAME		: IdleRefreshServiceIndicationArea

  	PURPOSE				: To refresh Service indication Area 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil


	RETURNS				: void

 

**************************************************************/
void IdleRefreshServiceIndicationArea(void)
{
	U8 i;
	
	mmi_trace(g_sw_NW,"MMI_Network: Entry Func: %s ", __FUNCTION__);

	#ifdef __MMI_MULTI_SIM__
	if (MTPNP_PFAL_Is_FlightMode())
	#else
	if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)	
	#endif
	{
		pfnUnicodeStrncpy((PS8)gNWProviderName[0], (PS8)GetString(STR_MENU_FLIGHT_MODE), MAX_LENGTH_DISPLAY_NAME - 1);
		SetLine1Text(gNWProviderName[0], 0);
		for(i = 1; i < MMI_SIM_NUMBER; i++)
		{
			SetLine1Text(NULL, i);
		}
		return;
	}	
	
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
		for(i = 0; i < 1; i++)
#else
		for(i = 0; i < MMI_SIM_NUMBER; i++)
#endif
	{
		#ifdef __MMI_MULTI_SIM__
		////////////////////////////////////////
		//Show PLMN name and details, closed/absent/blocked.
		if(!(MTPNP_AD_Get_Startup_Mode()&(0x01<<i)))
		{
			MTPNP_PFAL_Show_Idle_CardClosed(i);
			continue;
		}
		else if (MTPNP_PFAL_Is_CardAbsent(i))
		{
			MTPNP_PFAL_Show_Idle_CardAbsent(i);
			continue;
		}
		else if (MTPNP_PFAL_Is_CardCHVBlocked(i))
		{
			MTPNP_PFAL_Show_Idle_CardBlocked(i);
			continue;
		}

		if (mmi_bootup_is_network_searching_complete(i) == MMI_FALSE)
		{
			MTPNP_PFAL_Show_Idle_NW_Register(i);
			continue;
		}
		#endif
		mmi_trace(g_sw_NW, "MMI_NetWork: g_pwr_context[i].CurrentServiceType is %d",g_pwr_context[i].CurrentServiceType);
		if (g_pwr_context[i].CurrentServiceType == FULL_SERVICE)
		{
			MMI_BOOL SHOW_OPN = MMI_TRUE;
			stFontAttribute	IdleScr_Medium_font = {0,0,0,MEDIUM_FONT,0,0};
			MMI_BOOL OPN_Exist;
			MMI_BOOL Short_OPN_Exist;
			MMI_BOOL SPN_Exist;
			MMI_BOOL MSISDN_Exist = FALSE;
			U8 Scenario;


			if (pfnUnicodeStrlen(gOwnerName[i]) != 0)
			{
				MSISDN_Exist = PhnsetGetShowOwnerNumStatus();
			}
			#if defined(__MMI_OPERATOR_NAME_MENU__)		
				SHOW_OPN = PhnsetGetShowOPNStatus();
			#endif
			//MSISDN_Exist = TRUE;                     //delete by chenqiang for right displaying owern number state
			//OPN*4 + SPN*2+ b1*1
			Scenario = ((gOpNameValid || gShortOpnameValid) ? 4 : 0) + ((gSpnValid[i]) ? 2 : 0) + (g_pwr_context[0].RequireRPLMN ? 1: 0);

			// OPN_Exist means gOpNameValid and the string is not NULL
			OPN_Exist = gOpNameValid && (pfnUnicodeStrlen( gOperatorName) != 0) && SHOW_OPN ;
			Short_OPN_Exist = gShortOpnameValid && (pfnUnicodeStrlen(gShortOperatorName) != 0) && SHOW_OPN;
			SPN_Exist = gSpnValid[i] && (pfnUnicodeStrlen(gServProvderName[i]) != 0) ;

			SetFont(IdleScr_Medium_font, (U8)gCurrLangIndex);
			mmi_trace(g_sw_NW,"MMI_Network: Func: %s Scenario = %d", __FUNCTION__, Scenario);

			if (IsChineseSet() /* Trad. Chinese and Simp. Chinese only */ )
			{
				// Line 1 = PLMN
				SetLine1Text(gNWProviderName[i], i);

				if (SPN_Exist)
				{
					// Line 2: SPN
					SetLine2Text((U8*)gServProvderName[i], i);
				}
				else if (MSISDN_Exist)
				{
					// Line 2: Owner Name
					SetLine2Text((U8*)gOwnerName[i], i);
				}
			}
			else/*Not Chinese*/
			{
				switch (Scenario) // OPN--SPN--b1
				{
					case 0: // OPN=OFF, SPN=OFF, b1=0
					case 1: // OPN=OFF, SPN=OFF, b1=1
						// Line 1 = PLMN
						SetLine1Text(gNWProviderName[i], i);
						if (MSISDN_Exist)
						{
							// Line 2: Owner Name
							SetLine2Text((U8*)gOwnerName[i],i);
						}
						break;

					case 2: // OPN=OFF, SPN=ON, b1=0
                                        if (MSISDN_Exist)
                                        {
                                            if (mmi_bootup_is_roaming() && (MMI_SIM_NUMBER <= 2))
                                            {
                                                SetLine1Text(gNWProviderName[i], i);  /* Line 1 = PLMN */
                                                if (SPN_Exist && pfnUnicodeStrcmp((S8*) gServProvderName, (S8*) gNWProviderName) != 0)
                                                {
                                                    SetLine2Text((U8*) gServProvderName[i], i);  /* Line 2 = SPN */
                                                }
                                                else
                                                {
                                                    SetLine2Text((U8*) gOwnerName[i], i); /* Line 2: Owner Name */
                                                }
                                            }
                                            else
                                            {
                                                SetLine2Text((U8*) gOwnerName[i], i); /* Line 2: Owner Name */
                                                if (SPN_Exist)
                                                {
                                                    SetLine1Text((U8*) gServProvderName[i], i); /* Line 1 = SPN */
                                                }
                                                else
                                                {
                                                    SetLine1Text(gNWProviderName[i], i);  /* Line 2 = PLMN */
                                                }
                                            }
                                        }
                                        else
                                        {
                                            if (mmi_bootup_is_roaming() && (MMI_SIM_NUMBER <= 2))//only two sim show spn & gplmn
                                            {
                                                SetLine1Text(gNWProviderName[i], i);  /* Line 1 = PLMN */
                                                if (SPN_Exist && pfnUnicodeStrcmp((S8*) gServProvderName[i], (S8*) gNWProviderName[i]) != 0)
                                                {
                                                    SetLine2Text((U8*) gServProvderName[i], i);  /* Line 2 = SPN */
                                                }
                                            }
                                            else
                                            {
                                                if (SPN_Exist)
                                                {
                                                    SetLine1Text((U8*) gServProvderName[i], i);  /* Line 1 = SPN */
                                                }
                                                else
                                                {
                                                    SetLine1Text(gNWProviderName[i], i);  /* Line 2 = NULL */
                                                }
                                            }
                                        }
                                        break;

					case 3: // OPN=OFF, SPN=ON, b1=1
						// Line 1 = PLMN
						// Line 2 = SPN
						SetLine1Text((U8*)gServProvderName[i],i);
                        
						if(MSISDN_Exist)
							SetLine2Text((U8*)gOwnerName[i],i); // Line 2: Owner Name
						else
							SetLine2Text((U8*)NULL,i);

						break;
					case 4: // OPN=ON, SPN=OFF, b1=0
					case 5: // OPN=ON, SPN=OFF, b1=1
						if (g_pwr_context[i].RoamingFlag) 
						{
							// Line 1 = PLMN
							SetLine1Text(gNWProviderName[i],i);

							// Line 2 = OPN
							if (OPN_Exist)
							{
								SetLine2Text((U8*)gOperatorName,i);
							}
							else if (Short_OPN_Exist)
							{
								SetLine2Text((U8*)gShortOperatorName,i);
							}
							else// They are both valid but NULL
							{
								
							}
						}
						else
						{
							// Line 1 = OPN
							if (OPN_Exist)
							{
								SetLine1Text((U8*)gOperatorName,i);
							}
							else if (Short_OPN_Exist)
							{
								SetLine1Text((U8*)gShortOperatorName,i);
							}
							else // They are both valid but NULL
							{   
								// Line 1 = PLMN
								SetLine1Text(gNWProviderName[i],i);
							}

							if (MSISDN_Exist)
							{
								// Line 2 = Owner Name
								SetLine2Text((U8*)gOwnerName[i],i);
							}
						}                     
						break;

					case 6: // OPN=ON, SPN=ON, b1=0
					case 7: // OPN=ON, SPN=ON, b1=1
						if (g_pwr_context[i].RoamingFlag) 
						{
							// Line 1 = PLMN
							// Line 2 = SPN
							SetLine1Text(gNWProviderName[i],i);
							if (SPN_Exist)
							{
								if(pfnUnicodeStrcmp((S8*)gServProvderName[i],(S8*)gNWProviderName[i])!=0)
								{
									SetLine2Text((U8*)gServProvderName[i],i);
								}
								else if(MSISDN_Exist)
								{
									SetLine2Text((U8*)gOwnerName[i],i);
								}
							}
						}
						else
						{
							if (OPN_Exist)
							{
								// Line 1 = OPN
								SetLine1Text((U8*)gOperatorName,i);

								if (SPN_Exist)
								{
									if(pfnUnicodeStrcmp((S8*)gServProvderName[i],(S8*)gOperatorName)!=0)
									{
										SetLine2Text((U8*)gServProvderName[i],i);// Line 2 = SPN
									}
									else if(MSISDN_Exist)
									{
										SetLine2Text((U8*)gOwnerName[i],i);//Line 2 = MSISDN
									}
								}

							}
							else if (Short_OPN_Exist)
							{
								// Line 1 = Short OPN
								SetLine1Text((U8*)gShortOperatorName,i);

								if (SPN_Exist)
								{
									if(pfnUnicodeStrcmp((S8*)gServProvderName[i],(S8*)gShortOperatorName)!=0)
									{
										SetLine2Text((U8*)gServProvderName[i],i);// Line 2 = SPN
									}
									else if(MSISDN_Exist)
									{
										SetLine2Text((U8*)gOwnerName[i],i);//Line 2 = MSISDN
									}
								}
							}
							else // They are both valid but NULL
							{   
								// Line 1 = PLMN
								SetLine1Text(gNWProviderName[i],i);

								if (SPN_Exist)
								{
									if(pfnUnicodeStrcmp((S8*)gServProvderName[i],(S8*)gNWProviderName[i])!=0)
									{
										SetLine2Text((U8*)gServProvderName[i],i);// Line 2 = SPN
									}
									else if(MSISDN_Exist)
									{
										SetLine2Text((U8*)gOwnerName[i],i);//Line 2 = MSISDN
									}
								}
							}
						}
						break;

					default:
						MMI_ASSERT(0);
						break;
				}	
			}
		}
	#ifdef __MANUAL_MODE_NW_SEL__
		else if ( g_pwr_context[i].CurrentServiceType == LIMITED_SERVICE )
		{
			if(g_pwr_context[i].PLMNSelectionMode == AUTOMATIC_MODE)	
			{
				if(g_pwr_context[i].InvalidSim)
				{
					/* L1: "Invalid SIM" */
					pfnUnicodeStrcpy((PS8)gNWProviderName[i],(PS8)GetString(STR_SIM_INVALID_SIM_TITLE));
					SetLine1Text(gNWProviderName[i],i);
				}
				else
				{
					/* L1: "Emergency" */
					pfnUnicodeStrcpy((PS8)gNWProviderName[i],(PS8)GetString(STR_LIMITED_SERVICE));
					SetLine1Text(gNWProviderName[i],i);
				}
			}
			else
			{
				/* L1: "Select Network" */
				 
				if( mmi_idle_check_nw_provider_name_length((PU8)GetString(STR_ID_NETSET_MANUAL_MODE_TITLE)) )
					pfnUnicodeStrcpy((PS8)gNWProviderName[i],(PS8)GetString(STR_ID_NETSET_MANUAL_MODE_TITLE));
				else
					pfnUnicodeStrcpy((PS8)gNWProviderName[i],(PS8)GetString(STR_ID_NETSET_MANUAL_MODE_SHORT));
				SetLine1Text(gNWProviderName[i],i);
				idleRStringtID = STR_GLOBAL_SELECT;
			}

		}
		else if (  g_pwr_context[i].CurrentServiceType == NO_SERVICE  )
		{
			/* L1: "No Service" */
		#ifdef __FLIGHT_MODE_SUPPORT__
			if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
			#ifdef __FLIGHT_MODE_RENAME_TO_ANTENNA_MODE__
				pfnUnicodeStrncpy((PS8)gNWProviderName[i], (PS8)GetString(STR_FLIGHT_MODE_IDLE), MAX_LENGTH_DISPLAY_NAME-1);
			#else
				pfnUnicodeStrncpy((PS8)gNWProviderName[i], (PS8)GetString(STR_MENU_FLIGHT_MODE), MAX_LENGTH_DISPLAY_NAME-1);
			#endif
			else
		#endif
				pfnUnicodeStrncpy((PS8)gNWProviderName[i],(PS8)GetString(STR_NO_SERVICE),MAX_LENGTH_DISPLAY_NAME-1);
			SetLine1Text(gNWProviderName[i],i);
		}
		else//temporarily remove the string
		{
			/* L1: "" */
			SetLine1Text((U8 *)NULL,i);
		}
	#else	/* ! __MANUAL_MODE_NW_SEL__*/
		else if ( g_pwr_context[i].CurrentServiceType == LIMITED_SERVICE )
		{
			mmi_trace(g_sw_NW,"MMI_Network: Func: %s LIMITED_SERVICE", __FUNCTION__);
			/* L1: "Limited Service" */
			pfnUnicodeStrcpy((PS8)gNWProviderName[i],(PS8)GetString(STR_LIMITED_SERVICE));
			SetLine1Text(gNWProviderName[i],i);
		}
		else /* if ( gNoService ) */
		{
			mmi_trace(g_sw_NW,"MMI_Network: Func: %s NO_SERVICE", __FUNCTION__);
			/* L1: "No Service" */
		#ifdef __FLIGHT_MODE_SUPPORT__
			#ifdef __MMI_MULTI_SIM__
			if (MTPNP_AD_Is_Flight_Mode())
			#else
			if (mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
			#endif
			#ifdef __FLIGHT_MODE_RENAME_TO_ANTENNA_MODE__
				pfnUnicodeStrncpy((PS8)gNWProviderName[i], (PS8)GetString(STR_FLIGHT_MODE_IDLE), MAX_LENGTH_DISPLAY_NAME - 1);
			#else
				pfnUnicodeStrncpy((PS8)gNWProviderName[i], (PS8)GetString(STR_MENU_FLIGHT_MODE), MAX_LENGTH_DISPLAY_NAME - 1);
			#endif
			else
		#endif
				pfnUnicodeStrncpy((PS8)gNWProviderName[i],(PS8)GetString(STR_NO_SERVICE),MAX_LENGTH_DISPLAY_NAME-1);
			SetLine1Text(gNWProviderName[i],i);
		}
	#endif /*__MANUAL_MODE_NW_SEL__*/
	}
}

/**************************************************************

	FUNCTION NAME		: IdleToggleProfile

  	PURPOSE				: Toggles profiles between silent mode and current activated profile on long press of # key.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void IdleToggleProfile(void)
{
	MMI_ALERT_TYPE alert_type;

	mmi_trace(g_sw_BT, "entry IdleToggleProfile::gactivatedprofile= %d", gactivatedprofile);
	g_pwr_context[0].SOSCallFlagTOAvoidHistorySave=1;//qiff add for bug:10715
	if(gactivatedprofile==5)
		g_idle_context.ToggleProfile = SET_SILENT_PROFILE;
	else
		g_idle_context.ToggleProfile = SET_CURRENT_PROFILE;

       ClearKeyEvents();

	if(g_idle_context.ToggleProfile == SET_CURRENT_PROFILE)
	{
		g_idle_context.ToggleProfile=SET_SILENT_PROFILE;
		ActivateSilentProfile();
	}
	else
	{
		g_idle_context.ToggleProfile=SET_CURRENT_PROFILE;
		DeactivateSilentProfile();
	}
	alert_type=GetMtCallAlertTypeEnum();
	/*Jinzh Add Start For 5979 Ver: TBM780  on 2007-8-6 19:25 */
	if(alert_type==MMI_VIBRATION_ONLY && 0 == GetHandsetInPhone()) 
	/*Jinzh Add End  For 5979 Ver: TBM780  on 2007-8-6 19:25 */
	{
		VibratorOn();
		StartTimer(TOGGLE_TIMER, TOGGLE_PROFILE_TIMEOUT,IdleStopToggleTimer);
	}

	/*Jinzh Add Start For 5979 Ver: TBM780  on 2007-8-6 19:26 */	
	#ifdef __MMI_BT_PROFILE__
    if ((1 == GetHandsetInPhone()) || mmi_profiles_bt_is_activated())
#else 
	if(1 == GetHandsetInPhone())
#endif 
		IdleDisableLongPress();
	/*Jinzh Add End  For 5979 Ver: TBM780  on 2007-8-6 19:26 */
}

/**************************************************************

	FUNCTION NAME		: IdleStopToggleTimer()

  	PURPOSE				: Stop toggle Profile timer of # key..

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void IdleStopToggleTimer(void)
{
	StopTimer(TOGGLE_TIMER);
	VibratorOff();
}
/**************************************************************

	FUNCTION NAME		: IdleCallSpeedDialOnLongPress()

  	PURPOSE				: Implements speed dial on idle screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void IdleCallSpeedDialOnLongPress(void)
{
	U16 Keycode,Keytype;
	S8* tempbuffer=NULL;
	GetkeyInfo(&Keycode,&Keytype);
	
	if((Keycode>=1) && (Keycode<=9))
	{
		if(!g_phb_cntx.phb_ready)
		{	
			mmi_phb_entry_not_ready();
			return;
		}
	
		tempbuffer=PhbGetSpeedDialNum(Keycode);
		if(tempbuffer != NULL)
		{
			pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,tempbuffer);
	#ifdef __MMI_MULTI_SIM__
            mmi_ucm_app_entry_dial_option((U8*) g_idle_context.DialPadCallBuffer, NULL, NULL);
	#else
            IdleDialPadCall();
	#endif

		}
		else
		{
			IdleShowScrNoSpeedDialNumSaved();
		}
	}
}

/**************************************************************

	FUNCTION NAME		: IdleShowScrNoSpeedDialNumSaved()

  	PURPOSE				: Displays pop up: no speed dial number saved for this digit.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleShowScrNoSpeedDialNumSaved(void)
{
	PRINT_INFORMATION(("@@@@@@ NO SPEED DIAL NUMBER SAVED @@@@@@@@@"));
	DeleteUptoScrID(IDLE_SCREEN_ID);
	DisplayPopup((U8*)GetString(STR_NO_SPEED_DIAL_NUM_SAVED),IMG_GLOBAL_WARNING,0,2000,(U8)WARNING_TONE);
}


/**************************************************************

	FUNCTION NAME		: IdleAbbrevDialling()

  	PURPOSE				: Quick acces to phonebook entries.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleAbbrevDialling(void)
{
	S8 tempbuffer[(MAX_LOCATION_DIGITS+1) * ENCODING_LENGTH];
	U8 count,i=0;
	U16 Location;
	memset(tempbuffer,0,((MAX_LOCATION_DIGITS+1) * ENCODING_LENGTH));

//micha1217, Don't do anything when shifting out digits
	if (pfnUnicodeStrlen((PS8)g_idle_context.DialPadCallBuffer) >= (MAX_DIAL_PAD -1 ))
		return;
	

	if((g_idle_context.DialPadCallBuffer[0]>='0')&&(g_idle_context.DialPadCallBuffer[0]<='9'))
	{
	/*This loop search for '#' in DialpadCallbuffer,if # is not found before MAX_LOCATION_DIGITS then return*/
		for(count=0;(g_idle_context.DialPadCallBuffer[count]!='#');count++)
		{
				if(count>(MAX_LOCATION_DIGITS * ENCODING_LENGTH-1) || 
				    g_idle_context.DialPadCallBuffer[count] =='*' )
				{
					return;
				}
				tempbuffer[i]=g_idle_context.DialPadCallBuffer[count];
				i++;
		}
		if(g_idle_context.DialPadCallBuffer[count+2] == 0) /* no other digits after the first found # */
		{
			Location=(U16)coolsand_UI_atoi((UI_string_type)tempbuffer);
			g_idle_context.AvoidHistoryofDialerScreen=1;
			mmi_phb_get_sim_entry_by_location((U16)Location);
		}
	}
	 
	/*else if(!SSCStringParsing3())
	{	
		SSCStringParsing1();
	}*/
	//way1 direct goto FM
	/*else
		{
			EntryFMMenu();
		}*/
	//way2 test SSCStringParsing1()
	else if(!SSCStringParsing3())
	{
		SSCStringParsing1();
	}
}

/**************************************************************

	FUNCTION NAME		: IdleShowScrInvalidLocation()

  	PURPOSE				: Displays popup for Invalid location

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleShowScrInvalidLocation(void)
{
	PRINT_INFORMATION(("@@@@@@ INVALID LOCATION IN PHONE BOOK @@@@@@@@@"));
	DisplayPopup((U8*)GetString(STR_INVALID_LOCATION),IMG_GLOBAL_WARNING,0,2000,(U8)WARNING_TONE);
}

/**************************************************************

	FUNCTION NAME		: IdleShowScrNoPhoneNumEntry()

  	PURPOSE				: Show pop up that no phone book entry saved on this location.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleShowScrNoPhoneNumEntry(void)
{
	PRINT_INFORMATION(("@@@@@@ NO PHONE NUMBER CORRESPONDING TO THIS LOCATION @@@@@@@@@"));
	DisplayPopup((U8*)GetString(STR_NO_PHONE_NUM),IMG_GLOBAL_WARNING,0,2000,(U8)WARNING_TONE);
}

/**************************************************************

	FUNCTION NAME		: IdleDialPadSavePhoneBook()

  	PURPOSE				: Save entered number in phonebook on press of LSK on dialler screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleDialPadSavePhoneBook(void)
{
	SaveNumberFromIdleScrn(g_idle_context.DialPadCallBuffer,0);
}


/**************************************************************

	FUNCTION NAME		: IdleDialPadCall

  	PURPOSE				: Make a call from scratch pad

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

extern void  PrintUnicodeNStr(U8* str, int n);
#ifdef __FT_IDLE_SMART_DIAL__
void IdleSmartDialPadCall(void)
{
	/* Call the API with collected digits(DialPadCallBuffer) to initiate MO call */
	g_idle_context.AvoidHistoryofDialerScreen=1;
#ifdef __IP_NUMBER__	
	SetTempUseIPNumber(FALSE);
#endif
	MakeCall(g_DialPadCallBuffer);
}
#endif
void IdleDialPadCall(void)
{
#if defined(__PROJECT_GALLITE_C01__)//def __MMI_INTELIGENT_CALL_NUMBER__
S8 *call_buffer_ptr;

    U16 Keycode, Keytype;
    GetkeyInfo(&Keycode, &Keytype);

    //dbg_print("\r\nIdleDialPadCall  %d,%d.",  Keycode, Keytype );

    call_buffer_ptr = GetCallBuffer();

    if(((PhbGetSpeedDialStatus() == MMI_TRUE) && (Keytype == KEY_LONG_PRESS)  && (Keycode != KEY_SEND)) 	 //nianqiang mod 20080609
	|| (call_buffer_ptr[0] == '\0')		 //qiujianbin
//>>Bug #6521:待机界面输入电话号码后无触屏拨号功能(更改为：如通过触摸屏进入触摸拨号界面，而通过按键则进入智能搜索界面) luchongguang removed 2008-10-24
	||(EnterDialScrByCenterSoftkeyFlg == 0)//add zengzhaoxin 20080818   //luchongguang 修改2008-10-30
	) 
    {
         call_buffer_ptr = g_idle_context.DialPadCallBuffer;
    }
#if defined(__PROJECT_GALLITE_C01__)
    if(IsFocusOnInputBox())
    {
         call_buffer_ptr = g_idle_context.DialPadCallBuffer;
    }
#endif 
//#endif


#ifdef __IP_NUMBER__
    SetTempUseIPNumber(MMI_FALSE);
#endif 

    /* to disable the dialing if no digit is present */
#ifdef __MMI_TOUCH_DIAL_SCREEN__
//	#ifdef __MMI_INTELIGENT_CALL_NUMBER__
            if (!SSCStringParsing2() && call_buffer_ptr[0] != '\0')	// modified by qinjiangwei 2008/5/28
//	#else 
//            if (!SSCStringParsing2() && g_idle_context.DialPadCallBuffer[0] != '\0')
//	#endif	
#else 
    if (!SSCStringParsing2())
#endif 
    {
    #ifdef __MMI_TOUCH_SCREEN__
        mmi_pen_reset();
    #endif
        ClearInputEventHandler(MMI_DEVICE_ALL);

        mmi_idle_remove_dialer_screen_exit_handler();
        MakeCall(call_buffer_ptr);		//qinjiangwei
    }

#else
	#ifdef __FT_IDLE_SMART_DIAL__	
	S8 tempDialPad[MAX_DIAL_PAD * ENCODING_LENGTH];
	memset(tempDialPad,0,sizeof(tempDialPad));
	#endif
	TBM_ENTRY(0x28D6);
	/* Call the API with collected digits(DialPadCallBuffer) to initiate MO call */
	g_idle_context.AvoidHistoryofDialerScreen=1;
#ifdef __IP_NUMBER__	
	SetTempUseIPNumber(FALSE);
#endif

/* to disable the dialing if no digit is present */
#ifdef __MMI_TOUCH_DIAL_SCREEN__
	if(!SSCStringParsing2() && g_idle_context.DialPadCallBuffer[0]!='\0')
#else
	if(!SSCStringParsing2())
#endif
	{
	//	MakeCall(g_idle_context.DialPadCallBuffer);
		#ifndef __FT_IDLE_SMART_DIAL__
	        MakeCall(g_idle_context.DialPadCallBuffer);
		#else
		if(IsSmartDialStyle())
		{
			
			if(SmartDial_number_of_items ==0)
			{
			      MakeCall(g_idle_context.DialPadCallBuffer);
			}
		    else
			{
	             DynamicListGetHighlightedItem((U8*)tempDialPad);
	             MakeCall(tempDialPad);
			}
		}
		else
		{
			MakeCall(g_idle_context.DialPadCallBuffer);
		}
		#endif
	}
	mmi_trace(g_sw_CC,"MMI_Call: Func: %s ", __FUNCTION__);
	PrintUnicodeNStr(g_idle_context.DialPadCallBuffer, 10);
	TBM_EXIT(0x28D6);
#endif
}

#ifdef __IP_NUMBER__
void DialIPNumber (void)
{
#ifdef __FT_IDLE_SMART_DIAL__	
	S8 tempDialPad[MAX_DIAL_PAD * ENCODING_LENGTH];
	memset(tempDialPad,0,sizeof(tempDialPad));	
#endif

   g_idle_context.AvoidHistoryofDialerScreen=1;
   SetTempUseIPNumber(TRUE);
#ifdef __FT_MMI_GUI_DIAL_STYLE__
		#ifdef __FT_IDLE_SMART_DIAL__
		if(IsSmartDialStyle())
		 {
			 if(SmartDial_number_of_items ==0)
			 {
			 	mmi_ucm_app_entry_dial_option((U8*) g_idle_context.DialPadCallBuffer, NULL, NULL);
			// 	MakeCall(g_idle_context.DialPadCallBuffer);
			 }
			else
			{
				   DynamicListGetHighlightedItem((U8*)tempDialPad);
				   mmi_ucm_app_entry_dial_option((U8*)tempDialPad, NULL, NULL);
				  // MakeCall(tempDialPad);
			  }
	   	}
		 else 
		 {
		   		mmi_ucm_app_entry_dial_option((U8*) g_idle_context.DialPadCallBuffer, NULL, NULL);
	//			  MakeCall(g_idle_context.DialPadCallBuffer);
		 }
		#else
      	  mmi_ucm_app_entry_dial_option((U8*) g_idle_context.DialPadCallBuffer, NULL, NULL);
		#endif
#else
       if(!SSCStringParsing2())
      MakeCall(g_idle_context.DialPadCallBuffer);
#endif
}
#endif
/**************************************************************

	FUNCTION NAME		: IdleScreenDigitHandler()

  	PURPOSE				: Inputs the digits of a phone number 	

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifdef __FT_IDLE_SMART_DIAL__ 
extern S32  OnChangeEdit(U8* KeyWord) ;
#endif
#if defined(__PROJECT_GALLITE_C01__)
extern BOOL IsInDialerSrc(void);

BOOL IsInDialerSrc(void)
{
    if(GetActiveScreenId() == IDLE_SCREEN_DIGIT_HANDLER_ID)
        return TRUE;
    else
        return FALSE;
}

U8 g_dialer_focus_on_inputbox = 1;
BOOL IsFocusOnInputBox(void)
{
    if(g_dialer_focus_on_inputbox == 1)
        return TRUE;
    else
        return FALSE;
}
#endif 

void IdleScreenDigitHandler(void)
{
	byte *history_buffer;
	MMI_BOOL i;
	
	U16 SpeedDialKeys[MAX_SPEED_DIAL_KEYS]={KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9};

#ifdef __MMI_TOUCH_DIAL_SCREEN__
	U16 TouchDialPadKeys[MAX_TOUCH_DIAL_PAD_KEYS] = {KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_STAR,KEY_POUND,KEY_CSK};
#endif	
#ifdef __FT_IDLE_SMART_DIAL__ 
		S32 number_of_items;
//		U8	tempCurSmartDialStatus =0;
//		S16 Err=0;
#endif

	TBM_ENTRY(0x28D5);

	EntryNewScreen(IDLE_SCREEN_DIGIT_HANDLER_ID, ExitIdleScreenDigitHandler, NULL, NULL);

	DeInitSpecialMode();
	
	g_idle_context.IsOnDialerScreen=1;
	
	DeleteScreenIfPresent(IDLE_SCREEN_DIGIT_HANDLER_ID);
	
	g_idle_context.AvoidHistoryofDialerScreen=0;
	history_buffer=GetCurrGuiBuffer(IDLE_SCREEN_DIGIT_HANDLER_ID);

#if defined  (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
    mmi_idle_set_dual_sim_dialing_keypad_call_handler(
         mmi_idle_sim1_dialpad_call ,
         mmi_idle_sim2_dialpad_call ,
         mmi_idle_sim3_dialpad_call ,
         mmi_idle_sim4_dialpad_call);
/*
    mmi_idle_set_dual_sim_dialing_keypad_call_handler(
        (MTPNP_AD_Get_UsableSide() >= MTPNP_AD_SIMCARD1_USABLE) ? mmi_idle_sim1_dialpad_call : EntryScrEmergencyCallDialing,
        (MTPNP_AD_Get_UsableSide() >= MTPNP_AD_SIMCARD1_USABLE) ? mmi_idle_sim2_dialpad_call : EntryScrEmergencyCallDialing);*/
#else
	SetDialingKeypadCallHandler(IdleDialPadCall);
#endif

#else
#ifdef __MMI_WGUI_CSK_ENABLE__
#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_SetCenterFunc(IdleDialPadCall);
	SetCenterSoftkeyFunction((MTPNP_AD_Get_UsableSide_Number() >= 0) ? MTPNP_PFAL_CC_CSKHandler : EntryScrEmergencyCallDialing, KEY_EVENT_UP);
#else  /* No Defined __MMI_MULTI_SIM__ */
	SetCenterSoftkeyFunction(IdleDialPadCall, KEY_EVENT_UP);
#endif /* __MMI_MULTI_SIM__ */
  #endif
	#endif
#ifdef __FT_IDLE_SMART_DIAL__ 
	if(IsSmartDialStyle())
	{
		number_of_items =  OnChangeEdit((U8*)g_idle_context.DialPadCallBuffer);
		ShowCategorySmartDialScreen(STR_GLOBAL_SAVE,
													0,
													STR_GLOBAL_BACK,
													0,
													number_of_items,
													mmi__smartdial_search_list_get_item, 
													mmi__smartdial_search_list_get_hint,
													0, 
													0,
													(U8*)g_idle_context.DialPadCallBuffer, 
													MAX_DIAL_PAD ,
													history_buffer );
	}
	else
#endif
#if defined(__PROJECT_GALLITE_C01__)//defined(__MMI_INTELIGENT_CALL_NUMBER__)//>>Bug #6521:待机界面输入电话号码后无触屏拨号功能(更改为：如通过触摸屏进入触摸拨号界面，而通过按键则进入智能搜索界面) luchongguang 修改 2008-10-24
    if( /*1 ==*/!EnterDialScrByCenterSoftkeyFlg )
    {
#endif
	ShowCategory16Screen(STR_GLOBAL_SAVE /*DIAL_PAD_MO_CALL_LSK_STR_ID*/,0,STR_GLOBAL_BACK,0,(U8*)g_idle_context.DialPadCallBuffer, MAX_DIAL_PAD ,history_buffer);
#if defined(__PROJECT_GALLITE_C01__)//defined(__MMI_INTELIGENT_CALL_NUMBER__)
    }
    else
    {
    	// added by qinjiangwei 
    	mmi_dialer_entry_screen(IDLE_SCREEN_DIGIT_HANDLER_ID,
    					STR_GLOBAL_SAVE, 
#if defined(__PROJECT_GALLITE_C01__)
    					STR_GLOBAL_CLEAR/*STR_GLOBAL_BACK*/, 
#else
    					STR_GLOBAL_BACK, 
#endif
    					(U8*) g_idle_context.DialPadCallBuffer,
    					MAX_DIAL_PAD);
    }
#endif


	if(g_idle_context.IdleScreenDigitKeyCode>=0)
		ExecuteCurrKeyHandler(g_idle_context.IdleScreenDigitKeyCode,KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(IdleDialPadSavePhoneBook,KEY_EVENT_UP);

#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_HandleSendKeys(((MTPNP_AD_Get_UsableSide_Number() > 0) ? IdleDialPadCall : EntryScrEmergencyCallDialing), KEY_EVENT_DOWN/*IdleDialPadCall*/);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)

	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
	SetKeyHandler(IdleDialPadSavePhoneBook,KEY_ENTER,KEY_EVENT_UP );
#endif
#else
	SetKeyHandler(IdleDialPadCall, KEY_SEND, KEY_EVENT_DOWN);
#endif

	SetKeyHandler(IdleAbbrevDialling, KEY_POUND, KEY_EVENT_UP);

	SetCategory16RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif	
	
#ifdef __MMI_STARKEY_TOGGLE_PROFILE__
       if(( g_idle_context.LongPressEnable==0 ) && !history_buffer) // Lisen  (!history_buffer) to check if it is back from history, see Cannon+ SCR 1847
#else

	#ifdef __MMI_TOUCH_DIAL_SCREEN__		
	/* history buffer condition removed as to restore the key handlers after popup */
	/* g_idle_context.IdleScreenDigitKeyCode=-1 means dial screen shows when user click on CSK */
	 
	// TODO: 为何要关闭下面的代码?
//		if((g_idle_context.LongPressEnable==0) && (g_idle_context.IdleScreenDigitKeyCode!=KEY_STAR) && (g_idle_context.IdleScreenDigitKeyCode!=-1) )
		if(g_idle_context.LongPressEnable==0 && (g_idle_context.IdleScreenDigitKeyCode==KEY_STAR) )
	#else
		if((g_idle_context.LongPressEnable==0) && !history_buffer && (g_idle_context.IdleScreenDigitKeyCode==KEY_STAR) )
	#endif
#endif
{
	/*zhangm Add Start For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
#if !defined(__MMI_MAINLCD_220X176__)	
		SetKeyHandler(IdleHandleStarKeyForKeypadLock,KEY_STAR,KEY_LONG_PRESS);
#endif
	/*zhangm end end For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
}

/*first press of any key on idle screen to enter dialer screen, gLongPressEnable will be 0; 
	so following keyhandlers will b registered. On up event of anykey will call DisableLongPress 
	which will make gLongPressEnable=1 and will enter this screen again and these key handlers will not register again.*/
#ifdef __MMI_STARKEY_TOGGLE_PROFILE__
       if(( g_idle_context.LongPressEnable==0 ) && !history_buffer) // Lisen  (!history_buffer) to check if it is back from history, see Cannon+ SCR 1847
#else

	#ifdef __MMI_TOUCH_DIAL_SCREEN__		
	/* history buffer condition removed as to restore the key handlers after popup */
	/* g_idle_context.IdleScreenDigitKeyCode=-1 means dial screen shows when user click on CSK */
	 
	// TODO: 为何要关闭下面的代码?
//		if((g_idle_context.LongPressEnable==0) && (g_idle_context.IdleScreenDigitKeyCode!=KEY_STAR) && (g_idle_context.IdleScreenDigitKeyCode!=-1) )
		if(g_idle_context.LongPressEnable==0 && (g_idle_context.IdleScreenDigitKeyCode!=KEY_STAR) )
	#else
		if((g_idle_context.LongPressEnable==0) && !history_buffer && (g_idle_context.IdleScreenDigitKeyCode!=KEY_STAR) )
	#endif
#endif
	{
		#ifdef __MMI_TOUCH_DIAL_SCREEN__
		if ( !history_buffer )
		#endif
			g_idle_context.AvoidHistoryofDialerScreen=1;
    #ifdef __MMI_MULTI_SIM__
        if(MTPNP_AD_Get_UsableSide_Number() > 0)
    #else
        if (mmi_bootup_is_sim_valid())
    #endif
		{	
			i=PhbGetSpeedDialStatus();
			if(i==MMI_TRUE)
			{
				SetGroupKeyHandler(IdleCallSpeedDialOnLongPress,SpeedDialKeys,MAX_SPEED_DIAL_KEYS-1,KEY_LONG_PRESS);
			}
			SetKeyHandler(DialVoiceMail,KEY_1,KEY_LONG_PRESS);
		}
#ifdef __MMI_STARKEY_TOGGLE_PROFILE__
		SetKeyHandler(IdleToggleProfile,KEY_STAR,KEY_LONG_PRESS);
#else
#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_160X128__)
		SetKeyHandler(IdleToggleProfile,KEY_POUND,KEY_LONG_PRESS);
#endif
#endif

#ifdef __MMI_POUNDKEY_ENTER_PROFILE__
#if !(defined __MMI_MAINLCD_220X176__)
		SetKeyHandler(EntryScrProfiles,KEY_POUND,KEY_LONG_PRESS);
#endif
#endif

		
#ifdef __MMI_TOUCH_DIAL_SCREEN__
		SetGroupKeyHandler(IdleDisableLongPress,TouchDialPadKeys,(U8)MAX_TOUCH_DIAL_PAD_KEYS,KEY_EVENT_UP);
#else
#if !defined(__PROJECT_GALLITE_C01__)
		SetGroupKeyHandler(IdleDisableLongPress,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,KEY_EVENT_UP);
#endif
#endif
              ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
        	SetKeyHandler(IdleDialPadSavePhoneBook,KEY_ENTER,KEY_EVENT_UP);

	}
	TBM_EXIT(0x28D5);
}

/**************************************************************

	FUNCTION NAME		: ExitIdleScreenDigitHandler()

  	PURPOSE				: exit fn for idle screen digit handle
	

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ExitIdleScreenDigitHandler(void)
{
	if(!g_idle_context.AvoidHistoryofDialerScreen)
	{
		history_t h;
		S16		nHistory =	0;
		pfnUnicodeStrcpy ((S8*)h.inputBuffer, (S8*)&nHistory);
		h.scrnID=IDLE_SCREEN_DIGIT_HANDLER_ID;
#ifdef __FLIGHT_MODE_SUPPORT__
		if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
			h.entryFuncPtr=FlightModeDigitHandler;
		else
#endif
			h.entryFuncPtr=IdleScreenDigitHandler;
		GetCategoryHistory(h.guiBuffer);
		AddHistory(h);
	}
	
	g_idle_context.IsOnDialerScreen=0;
	g_idle_context.IdleScreenDigitKeyCode=-1;
	
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
	ResetCenterSoftkey();
#endif

#if defined(__PROJECT_GALLITE_C01__)//defined(__MMI_INTELIGENT_CALL_NUMBER__)//>>Bug #6521:待机界面输入电话号码后无触屏拨号功能(更改为：如通过触摸屏进入触摸拨号界面，而通过按键则进入智能搜索界面) luchongguang 修改 2008-10-24
    if (EnterDialScrByCenterSoftkeyFlg == 1)
    {
        mmi_dailer_exit_screen();		//qinjiangwei
    }
#endif
}

/**************************************************************

	FUNCTION NAME		: HandleIdleScreenCSKHandler()

  	PURPOSE				: This function is entered when a key
						  is pressed at the Idle screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HandleIdleScreenCSKHandler(void)
{	
	memset(g_idle_context.DialPadCallBuffer, 0, sizeof(g_idle_context.DialPadCallBuffer));
	g_idle_context.IdleScreenDigitKeyCode=KEY_CSK;
	g_idle_context.LongPressEnable=1;

	IdleScreenDigitHandler();
}


/**************************************************************

	FUNCTION NAME		: HandleIdleScreenDigitEntry()

  	PURPOSE				: This function is entered when a key
						  is pressed at the Idle screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HandleIdleScreenDigitEntry(void)
{	
	U16 Keycode,Keytype;

        #if defined(__PROJECT_GALLITE_C01__)//defined(__MMI_INTELIGENT_CALL_NUMBER__)//>>Bug #6521:待机界面输入电话号码后无触屏拨号功能(更改为：如通过触摸屏进入触摸拨号界面，而通过按键则进入智能搜索界面) luchongguang 修改 2008-10-24
            EnterDialScrByCenterSoftkeyFlg = 1;//2;
        #endif
	GetkeyInfo(&Keycode,&Keytype);
	/*sprintf(g_idle_context.DialPadCallBuffer,"");*/ /*This will empty DialPadCallBuffer.*/
	memset(g_idle_context.DialPadCallBuffer, 0, sizeof(g_idle_context.DialPadCallBuffer));
	g_idle_context.IdleScreenDigitKeyCode=Keycode;

 
#ifdef __FLIGHT_MODE_SUPPORT__
	if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
		FlightModeDigitHandler();
	else
#endif
 
		IdleScreenDigitHandler();
}

/**************************************************************

	FUNCTION NAME		: IdleDisableLongPress()
	
  	PURPOSE				: Disbale some long press handlers on up event of key

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleDisableLongPress(void)
{
	g_idle_context.LongPressEnable=1;
#ifdef __FLIGHT_MODE_SUPPORT__
	if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
		FlightModeDigitHandler();
	else
#endif
		IdleScreenDigitHandler();
}

/**************************************************************

	FUNCTION NAME		: IdleSetKeyPadLockFlag

  	PURPOSE				: This function is called when time out
	                      occurs on idle screen to set key pad lock flag  

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleSetKeyPadLockFlag(void)
{
	/*U16 ScreenSaverLocks[MAX_KEYS]= {  KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
									KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
									KEY_LSK, KEY_RSK, KEY_UP_ARROW, 
									KEY_DOWN_ARROW, KEY_LEFT_ARROW, 
									KEY_RIGHT_ARROW, KEY_SEND, KEY_END, 
									KEY_CLEAR, KEY_STAR, KEY_POUND, KEY_VOL_UP,
									KEY_VOL_DOWN, KEY_QUICK_ACS, KEY_ENTER,
									KEYBOARD_KEYS };
*/
	/*zhoumn Add Start For Auto Keypad Lock Ver: TBM780  on 2007-8-1 16:8 */
	if(!is_phb_init_done()) 
	{
		StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
		StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, IdleSetKeyPadLockFlag);
		return;
	}
	/*zhoumn Add End  For Auto Keypad Lock  Ver: TBM780  on 2007-8-1 16:8 */
	PRINT_INFORMATION( ("[Keypad Lock] SetKeyPadLockFlag: gKeyPadLockFlag ON") );

	gKeyPadLockFlag = 1;	
	
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_block();
#endif


#ifdef __MMI_KEYPADLOCK_WITH_KEYTONE__
      
#else 
    mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
#endif
	
	if ( g_idle_context.ScreenSaverRunFlag != 1 )	
	{
		 
		EntryIdleScreen();
#ifdef __MMI_TOUCH_SCREEN__
	#if 0 //modified by zhoumn@2007/07/28
		SetKeyHandler(IdleRSKPressOnScreenSaver, KEY_SEND, KEY_EVENT_DOWN);
	#else
		SetKeyHandler(IdleRSKPressOnScreenSaver, KEY_RSK, KEY_EVENT_DOWN);
	#endif
#else /* __MMI_TOUCH_SCREEN__ */

		SetGroupKeyHandler(NULL, (PU16) PresentAllKeys/*ScreenSaverLocks*/, (U8)TOTAL_KEYS/*MAX_KEYS - 1*/,KEY_EVENT_DOWN);	
#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__  
		SetRightSoftkeyFunction(IdleRSKPressOnScreenSaver,KEY_EVENT_DOWN);
#endif

#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__  
		SetLeftSoftkeyFunction(IdleRSKPressOnScreenSaver,KEY_EVENT_DOWN);
#endif

#endif /* __MMI_TOUCH_SCREEN__ */
	}
	else
    {
		 
#ifdef __MMI_TOUCH_SCREEN__
	#if 0 //modified by zhoumn@2007/07/28
		SetKeyHandler(IdleRSKPressOnScreenSaver, KEY_SEND, KEY_EVENT_DOWN);
	#else 
		SetKeyHandler(IdleRSKPressOnScreenSaver, KEY_RSK, KEY_EVENT_DOWN);
	#endif
#else /* __MMI_TOUCH_SCREEN__ */

	#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__  
		SetKeyHandler(IdleRSKPressOnScreenSaver, KEY_RSK ,KEY_EVENT_DOWN);
	#endif

	#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__  
		SetKeyHandler(IdleRSKPressOnScreenSaver, KEY_LSK ,KEY_EVENT_DOWN);
	#endif

#endif /* __MMI_TOUCH_SCREEN__ */
	}

#ifdef __MMI_TOUCH_SCREEN__
	ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_LOCK);
	UpdateStatusIcons(); 
#else
	IdleSetStatusIcon(STATUS_ICON_KEYPAD_LOCK);	
#endif
	 

	PRINT_INFORMATION( ("[Keypad Lock] SetKeyPadLockFlag: Timer %d Expire", g_idle_context.AutoLockTimeOut) );
	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
	// TODO: 为何要关闭下面的代码?
 
	//TurnOffBacklight();
// CSD end
}


/**************************************************************

	FUNCTION NAME		: IdleSetStatusIcon

  	PURPOSE				: Show Icon

	INPUT PARAMETERS	: U8

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void IdleSetStatusIcon(S16 index)
{
	ShowStatusIcon(index);	
	UpdateStatusIcons();
}

/**************************************************************

	FUNCTION NAME		: IdleResetStatusIcon

  	PURPOSE				: Hide Icon  

	INPUT PARAMETERS	: U8

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void IdleResetStatusIcon(S16 index)
{
	HideStatusIcon(index);
	UpdateStatusIcons();
}

// TODO: 怎么回事?
#if 0  /*CSD 2004-07-19 Wilson, Remove Unused Function*/
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

#ifdef __MMI_TOUCH_SCREEN__
/*****************************************************************************
* FUNCTION
*	 mmi_scrsvr_pen_up_handler 
* DESCRIPTION
*   screen saver pen up handler 
* PARAMETERS
*	 void	
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_scrsvr_pen_up_handler(mmi_pen_point_struct point)
{
	DisplayIdleScreen();
}
#endif /* __MMI_TOUCH_SCREEN__ */

#ifdef __MMI_LCD_PARTIAL_ON__

/**************************************************************
	FUNCTION NAME		: LeavePartialOnScreenSaverIfOk
	
  	PURPOSE				: 
  	
	INPUT PARAMETERS	: nil
	
	OUTPUT PARAMETERS	: nil
	
	RETURNS				: void
	
 
**************************************************************/
void LeavePartialOnScreenSaverIfOk(void)
{
	if (GetExitScrnID() == SCREEN_SAVER_PARTIAL_ON_ID)
	{
		GoBackHistory();
	}
}


/**************************************************************
	FUNCTION NAME		: EnterPartialOnScreenSaverIfOk
	
  	PURPOSE				: 
  	
	INPUT PARAMETERS	: nil
	
	OUTPUT PARAMETERS	: nil
	
	RETURNS				: void
	
 
**************************************************************/
void EnterPartialOnScreenSaverIfOk(void)
{
	if (GetExitScrnID() != SCREEN_SAVER_PARTIAL_ON_ID)
	{
		EntryPartialOnScreenSaver();
	}
}

/**************************************************************
	FUNCTION NAME		: RedrawPartialOnScreenSaverIfOk
	
  	PURPOSE				: 
  	
	INPUT PARAMETERS	: nil
	
	OUTPUT PARAMETERS	: nil
	
	RETURNS				: void
	
 
**************************************************************/
void RedrawPartialOnScreenSaverIfOk(void)
{
	if (GetExitScrnID() == SCREEN_SAVER_PARTIAL_ON_ID)
	{
		UI_time t;
		GetDateTime(&t);
		Category161ChangeTime(&t);
		RedrawCategory161Screen();
	}
}

/**************************************************************
	FUNCTION NAME		: PartialOnScreenSaverGoBack
	
  	PURPOSE				: 
  	
	INPUT PARAMETERS	: nil
	
	OUTPUT PARAMETERS	: nil
	
	RETURNS				: void
	
 
**************************************************************/
static void PartialOnScreenSaverGoBack(void)
{
//	extern void	(*left_softkey_functions[])(void);
//	extern void (*right_softkey_functions[])(void);
	U16 keycode, keytype;

	GetkeyInfo(&keycode, &keytype);

	/* Flush key queue */
	ClearKeyEvents();
	GoBackHistory();


	/* Tricky. Check if the previous screen is keypad locked
	 * We can not use gKeyPadLockFlag nor g_idle_context.IsRSKPressOnScreenSaver
	 * because partial-on screen saver can display on screens other than idle screen.
	 */
#ifdef __MMI_TOUCH_SCREEN__
	if ( keycode == KEY_SEND && GetKeyHandler(keycode, keytype ) == EntryScrAfterPessingRSK )
		EntryScrAfterPessingRSK();

#else
#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__ 
	// TODO: 为何要关闭下面的代码?
	if ( (get_softkey_function(KEY_EVENT_DOWN, MMI_RIGHT_SOFTKEY) == EntryScrAfterPessingRSK)  && (keycode == KEY_RSK) )
//	if ((right_softkey_functions[KEY_EVENT_UP] == EntryScrAfterPessingRSK) && 
//		 (keycode == KEY_RSK))
	{
		EntryScrAfterPessingRSK();
	}
#endif

#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__ 
	// TODO: 为何要关闭下面的代码?
	if ( (get_softkey_function(KEY_EVENT_DOWN, MMI_LEFT_SOFTKEY) == EntryScrAfterPessingRSK) && (keycode == KEY_LSK) )
	{
		EntryScrAfterPessingRSK();
	}
#endif
#endif
}


/**************************************************************
	FUNCTION NAME		: EntryPartialOnScreenSaver
	
  	PURPOSE				: 
  	
	INPUT PARAMETERS	: nil
	
	OUTPUT PARAMETERS	: nil
	
	RETURNS				: void
	
 
**************************************************************/
static void ExitPartialOnScreenSaver(void)
{
	/* Tricky. We need at least one dummy exit handler. Otherwise, ExitCategoryFunction wont be invoked. */
	g_idle_context.ScreenSaverRunFlag = 0;

	// TODO: 为何要关闭下面的代码?
	// leave_full_screen(); //Already invoked in UI_common_screen_exit()
}

void EntryPartialOnScreenSaver(void)
{	
	UI_time t;

	// TODO: 为何要关闭下面的代码?
	//tangjie del begin 20060616
	#if 0
	#ifndef __MMI_SLIDE__
	if(IsClamClose())
	{
		return;
	}
	#endif
	g_idle_context.ScreenSaverRunFlag = 1;

	#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
	if(gIsInSleepMode)
	{
		mmi_scrsvr_lcd_sleepin_handler();
		g_idle_context.ScreenSaverRunFlag = 0;
		return;
	}
	#endif
	#endif
	//tangjie del end 20060616


	EntryNewScreen(SCREEN_SAVER_PARTIAL_ON_ID, ExitPartialOnScreenSaver, NULL, NULL);

	entry_full_screen();

	ClearAllKeyHandler();
	SetGroupKeyHandler(PartialOnScreenSaverGoBack, (PU16) PresentAllKeys, TOTAL_KEYS, KEY_EVENT_DOWN);

#ifdef __MMI_TOUCH_SCREEN__
	mmi_wgui_register_pen_up_handler(mmi_scrsvr_pen_up_handler);
	mmi_wgui_register_pen_down_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_move_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_abort_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_repeat_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_long_tap_handler(mmi_pen_dummy_hdlr);	
#endif /* __MMI_TOUCH_SCREEN__ */

	GetDateTime(&t);
	ShowCategory161Screen(&t);
}

#endif /* __MMI_LCD_PARTIAL_ON__ */


/*****************************************************************************
*
*	Screen Saver 
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_scrsvr_play_video_callback
* DESCRIPTION
*   play screen saver video result callback function
* PARAMETERS
*	 result		IN 	play video's reult
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_scrsvr_play_video_callback(MDI_RESULT result)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* treat as gif animation */
	if(result < 0)
	{
	 	g_idle_context.ScreenSaverRunFlag = 0; /* FALSE */

		TurnOnBacklight(1);  /* 1 means restart timer backlight */
		
		/* reset as default */
		PhnsetScreenSaverBadFileCallBack();
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_scrsvr_show_image_callback
* DESCRIPTION
*   show screen saver image result callback function
* PARAMETERS
*	 void	
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
/* 
	remove image display callback, when play image, even the image file is corrupted, 
	we shall not restore default, we shall let it play as much as possible 
*/
// TODO: 怎么回事?
#if 0
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







/*****************************************************************************
* FUNCTION
*	 mmi_scrsvr_lcd_sleepin
* DESCRIPTION
*   screen saver lcd sleep in handler (sleep in means turn off)
* PARAMETERS
*	 result		IN 	play video's reult
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_scrsvr_lcd_sleepin_handler(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_idle_context.ScreenSaverRunFlag == 1) /* == TRUE */
	    {





		/* force to enter a dummy screen - which will exit screensaver's screen */
		EntryNewScreen(IDLE_SCREEN_DUMMY_ID, NULL, NULL, NULL);





		
		/* entry full screen - force mainlcd status bar not update */
		entry_full_screen();

		/* clear full screen as black */
	/*WangRui rescover for fix bug 10328 , fixbug5860 in  mmi_scrsvr_lcd_sleepin_handler */
		
	//	gdi_layer_clear(GDI_COLOR_BLACK);
	/*WangRui rescover end*/

		g_idle_context.IsScrSvrSleepIn = 1; /* qiff modify for bug:10695,please keep it upon the entrynewscreen */

		/* force sub to play timer */
		ShowSubLCDScreen(ShowIdleAppSubLcd);

	 }
}



/*****************************************************************************
* FUNCTION
*	 mmi_scrsvr_lcd_sleepout_handler 
* DESCRIPTION
*   screen saver lcd sleep out handler (sleep out means turn on)
* PARAMETERS
*	 void	
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_scrsvr_lcd_sleepout_handler(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* check if it is in dummy screen */
	/* 
		this function maybe called by alarm or other interupt when they try to turn on backlight 
		when lcd is already sleep out, so add this protection 
	*/
	
	if(GetExitScrnID() != IDLE_SCREEN_DUMMY_ID)
		return;
	
	if(g_idle_context.IsScrSvrSleepIn == 1) /* == TRUE */
	{
		g_idle_context.IsScrSvrSleepIn = 0; 
		EntryIdleSetScreenSaver();
	}

}



/*****************************************************************************
* FUNCTION
*	 EntryIdleSetScreenSaver
* DESCRIPTION
*   Enter screen saver from idle screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void EntryIdleSetScreenSaver(void)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 			cur_screensaver;
	U8 			*guiBuffer;
	U16			res_type;
	PS8			filename_ptr;
	FS_HANDLE	fs_handle;

mmi_trace(g_sw_WpSsPwon, "gdm: in EntryIdleSetScreenSaver");

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	PRINT_INFORMATION(("Screen Saver Timeout"));
	if(!is_phb_init_done()) 
	{
		mmi_idle_restart_screensaver_timer();
		return;
	}

	/* if clam is close then want try to enter screen saver, do not enter */
// TODO: 为何要关闭下面的代码?	
/*wangrui reopen the code 20081202 for fix bug 10547*/
//tangjie del begin 20060616
#if 0
#ifndef __MMI_SLIDE__
	if(IsClamClose())
	{
		return;
	}
#endif
#endif	
	g_idle_context.ScreenSaverRunFlag = 1;

	#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
		if(gIsInSleepMode)
		{
			mmi_scrsvr_lcd_sleepin_handler();
			g_idle_context.ScreenSaverRunFlag = 0;
			return;
		}
	#endif

//tangjie del end 20060616
/*wangrui reopen the code 20081202 end*/
	
	g_idle_context.ScreenSaverRunFlag = 1; //added by zhoumn@2007/07/28
	
	EntryNewScreen(IDLE_SCREEN_SCREENSAVER_ID, ExitScreenSaver, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(IDLE_SCREEN_SCREENSAVER_ID);
	g_idle_context.IsRSKPressOnScreenSaver=0;

 mmi_trace(g_sw_WpSsPwon, "gdm:EntryIdleSetScreenSaver, g_phnset_cntx_p->curSsID=%d", g_phnset_cntx_p->curSsID);
PhnsetGetCurScrSvr(&cur_screensaver);

	if(PhnsetIsScrSvrAnalogClock())
	{
		screen_saver_type =  ANALOG_SCREEN_SAVER;
		ShowCategory131Screen(cur_screensaver,NULL);
	}
	else if(PhnsetIsScrSvrDigitalClock())
	{
		screen_saver_type =  DIGITAL_SCREEN_SAVER;
		ShowCategory131Screen(cur_screensaver,NULL);
	}
#ifdef __MMI_THEMES_V2_SUPPORT__ //ritesh
	else if(PhnsetIsScrSvrCurrentTheme())
	{
		ShowCategory166Screen(cur_screensaver, filename_ptr, NULL);	
	}
#endif //ritesh
	else
	{
		/* play image or video */

		/*******************************************/
		/* Get animation resource from phone setup */
		/*******************************************/
		
 mmi_trace(g_sw_WpSsPwon, "gdm:EntryIdleSetScreenSaver, cur_screensaver=%d", cur_screensaver);
		if(cur_screensaver == 0xff)	/* invalid format */
		{
			/* reset as default */
			PhnsetScreenSaverBadFileCallBack();
			return;
		}
		else if(cur_screensaver == 0x00 	/* source from file */
				|| cur_screensaver == 0x01)		/* short file name*/
		{
			cur_screensaver = 0x00;
		
 mmi_trace(g_sw_WpSsPwon, "gdm:EntryIdleSetScreenSaver, here++++++++++++++");
			/* filename buffer is allocated in phnset */
			filename_ptr 	= PhnsetGetCurScrSvrFileName();
			fs_handle 		= MMI_FS_Open(filename_ptr, FS_READ_ONLY);

			if(fs_handle < FS_NO_ERROR)
			{
				/* open file failed */
				/* reset as default */
				PhnsetScreenSaverBadFileCallBack();
				return;
			}
			else
			{
				/* open file successful */
				MMI_FS_Close(fs_handle);
			}
		}
		else
		{
			/* source from intenal resource */
			filename_ptr = NULL;
		}    

mmi_trace(g_sw_WpSsPwon, "gdm:EntryIdleSetScreenSaver,filename_ptr=");

		/*******************************************/
		/* Check if is image or video					 */
		/*******************************************/
		/* get file resource type */
		// TODO: 为何要关闭下面的代码?
		//tangjie del begin 20060616
		#if 1
		if(filename_ptr != NULL)
		{
			/* source from file */
			res_type = mmi_fmgr_get_file_group(filename_ptr);

mmi_trace(g_sw_WpSsPwon, "gdm:EntryIdleSetScreenSaver,res_type=%d",res_type);
			if(res_type == FMGR_GROUP_IMAGE)
			{
				g_scrsvr_is_image = TRUE;
			}
			else if(res_type == FMGR_GROUP_VIDEO)
			{
				g_scrsvr_is_image = FALSE;
			}
			else
			{
				/* unsupported file format ext */
				/* reset as default */
				PhnsetScreenSaverBadFileCallBack();
				return;
			}
		}			
		#endif
		//tangjie del end 20060616
		
		/* check internal resource type */
		if(filename_ptr == NULL)
		{
			 res_type = PhnsetGetDisplayType(cur_screensaver);
		
			if(res_type == PHNSET_TYPE_DISP_IMAGE)
			{
				g_scrsvr_is_image = TRUE;
			}
			else if(res_type == PHNSET_TYPE_DISP_VIDEO)
			{
				g_scrsvr_is_image = FALSE;
			}
			else
			{
				MMI_ASSERT(0);
			}
		}


		/*******************************************/
		/* Start to play									 */
		/*******************************************/
		mmi_trace(g_sw_WpSsPwon, "gdm: in EntryIdleSetScreenSaver() g_scrsvr_is_image=%d",g_scrsvr_is_image);
		if(g_scrsvr_is_image)
		{
			/* Play GIF animation or still image */
			ShowCategory166Screen(cur_screensaver, filename_ptr, NULL);	
		}
		else
		{
			/* Play video clip */
			#if defined(__MMI_VIDEO__)
			        /* suspend background audio */
			        mdi_audio_suspend_background_play();//qiff add for bug:10715

				/* entry video play full screen */
				ShowCategory224Screen(	cur_screensaver, 						/* video id */
										filename_ptr,							/* filename */
										0, 										/* repeat_count */ /* 0 means infinite */
										TRUE,									/* is_visaul_update */
										FALSE, 									/* qiff modify for bug:10630 */
										FALSE,									/* is_lcd_no_sleep */ /* need sleep in */
										GDI_COLOR_BLACK,						/* bg_color */
										mmi_scrsvr_play_video_callback,			/* (*play_finish_callback)(S16) */
										NULL);									/* gui_buffer */

				/* category 224 will stop lcd sleep mechansim, we must let MMI can sleep in */
				
			#else
				MMI_ASSERT(0);
			#endif /* __MMI_VIDEO__ */
		
		}
	}

#ifdef __MMI_MAINLCD_220X176__
	SetGroupKeyHandler(	NULL, 
								(PU16) PresentAllKeys/*ScreenSaverLocks*/, 
								(U8)TOTAL_KEYS/*MAX_KEYS - 3*/,
								KEY_EVENT_DOWN);
#endif	
	SetGroupKeyHandler(	IdleScreenSaverGroupKeyHandler, 
								(PU16) PresentAllKeys/*ScreenSaverLocks*/, 
								(U8)TOTAL_KEYS/*MAX_KEYS - 3*/,
								KEY_EVENT_DOWN);

#ifdef __MMI_TOUCH_SCREEN__
	mmi_wgui_register_pen_up_handler(mmi_scrsvr_pen_up_handler);
	mmi_wgui_register_pen_down_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_move_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_abort_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_repeat_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_long_tap_handler(mmi_pen_dummy_hdlr);	
#endif /* __MMI_TOUCH_SCREEN__ */

	/* keypad lock key handler */
	if( gKeyPadLockFlag	 != 0)		/*  If KeyPad Locked */
	{
#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__ 
		SetKeyHandler(IdleRSKPressOnScreenSaver, KEY_RSK, KEY_EVENT_DOWN);
#endif

#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__
		SetKeyHandler(IdleRSKPressOnScreenSaver, KEY_LSK, KEY_EVENT_DOWN);
#endif
	}
}

/**************************************************************

	FUNCTION NAME		: ExitScreenSaver

  	PURPOSE				: Exit fn for screen saver.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScreenSaver(void)
{
	g_idle_context.ScreenSaverRunFlag = 0;
	//qiff add for bug:10715
	if(g_idle_context.IsScrSvrSleepIn !=1 ) 
	       mdi_audio_resume_background_play();
}

/**************************************************************

	FUNCTION NAME		: IdleScreenSaverGroupKeyHandler

  	PURPOSE				: Group key handler fn for screen saver keys.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void IdleScreenSaverGroupKeyHandler(void)
{
	DisplayIdleScreen();
}
/**************************************************************

	FUNCTION NAME		: IdleRSKPressOnScreenSaver

  	PURPOSE				: This fn tells if RSk has been pressed on idle screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void IdleRSKPressOnScreenSaver(void)
{
	g_idle_context.IsRSKPressOnScreenSaver=1;
	EntryIdleScreen();
}


/**************************************************************

	FUNCTION NAME		: IsChineseSet

  	PURPOSE				: This function tells whether current lang is set as chinese or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsChineseSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_TCHINESE)==0)
		return 1;

     if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_SCHINESE)==0)
		return 1;
	 
	return 0;
}
 
/**************************************************************

	FUNCTION NAME		: IsTrChineseSet

  	PURPOSE				: This fn tells whether traditional chinese is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsTrChineseSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_TCHINESE)==0)
		return 1;

	return 0;
}

/**************************************************************

	FUNCTION NAME		: IsSmChineseSet

  	PURPOSE				: This fn tells whether simplified chinese is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsSmChineseSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_SCHINESE)==0)
		return 1;

	return 0;
}


/*Wangzl Add Start  Ver: Multi_Language  on 2008-1-15 16:38 */
/**************************************************************

	FUNCTION NAME		: IsFrenchSet

  	PURPOSE				: This fn tells whether French is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsFrenchSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_FRENCH)==0)
		return 1;
	return 0;
}
/*Wangzl Add End  Ver: Multi_Language  on 2008-1-15 16:38 */
#ifdef __MMI_LANG_TURKISH__
/**************************************************************

	FUNCTION NAME		: IsTurkishSet

  	PURPOSE				: This fn tells whether Turkish is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsTurkishSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_TURKISH)==0)
		return 1;
	return 0;
}
#endif
#ifdef __MMI_LANG_INDONESIAN__
/**************************************************************

	FUNCTION NAME		: IsIndonesianSet

  	PURPOSE				: This fn tells whether Indonesian is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsIndonesianSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_INDONESIAN)==0)
		return 1;
	return 0;
}
#endif
/**************************************************************

	FUNCTION NAME		: IsArabicSet

  	PURPOSE				: This fn tells whether Arabic is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsArabicSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_ARABIC)==0)
		return 1;
	return 0;
}

/**************************************************************

	FUNCTION NAME		: IsPersianSet

  	PURPOSE				: This fn tells whether Persian is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsPersianSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_PERSIAN)==0)
		return 1;
	return 0;
}

/**************************************************************

	FUNCTION NAME		: IsUrduSet

  	PURPOSE				: This fn tells whether Persian is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/
pBOOL IsUrduSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_URDU)==0)
		return 1;
	return 0;
}

/**************************************************************

	FUNCTION NAME		: IsRussianSet

  	PURPOSE				: This fn tells whether Russian is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsRussianSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_RUSSIAN)==0)
		return 1;
	return 0;
}
#ifdef __MMI_LANG_VIETNAMESE__
/**************************************************************

	FUNCTION NAME		: IsVietnameseSet

  	PURPOSE				: This fn tells whether Vietnamese is set or not

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsVietnameseSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_VIETNAMESE)==0)
		return 1;
	return 0;
}
#endif
//CSD end
//beginning Kecx added on 20080711
#ifdef __MMI_LANG_THAI__
/**************************************************************

	FUNCTION NAME		: IsThaiSet

  	PURPOSE				: This fn tells whether Thai is set or not

	INPUT PARAMETERS 	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsThaiSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_THAI)==0)
		return 1;
	return 0;
}
#endif

//CSD end
#ifdef __MMI_LANG_ITALIAN__
/**************************************************************

	FUNCTION NAME		: IsItalianSet

  	PURPOSE				: This fn tells whether Italian is set or not

	INPUT PARAMETERS 	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsItalianSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_ITALIAN)==0)
		return 1;
	return 0;
}
#endif
#ifdef __MMI_LANG_HINDI__
/**************************************************************

	FUNCTION NAME		: IsTHindiSet

  	PURPOSE				: This fn tells whether Hindi is set or not

	INPUT PARAMETERS 	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsHindiSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_HINDI)==0)
		return 1;
	return 0;
}
#endif
#ifdef __MMI_LANG_TELUGU__
/**************************************************************

	FUNCTION NAME		: IsTHindiSet

  	PURPOSE				: This fn tells whether Hindi is set or not

	INPUT PARAMETERS 	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsTeluguSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_TELUGU)==0)
		return 1;
	return 0;
}
#endif
#ifdef __MMI_LANG_GERMAN__
/**************************************************************

	FUNCTION NAME		: IsGermanSet

  	PURPOSE				: This fn tells whether German is set or not

	INPUT PARAMETERS 	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsGermanSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_GERMAN)==0)
		return 1;
	return 0;
}
#endif
#ifdef __MMI_LANG_SPANISH__
/**************************************************************

	FUNCTION NAME		: IsSpanishSet

  	PURPOSE				: This fn tells whether Spanish is set or not

	INPUT PARAMETERS 	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsSpanishSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_SPANISH)==0)
		return 1;
	return 0;
}
#endif
#ifdef __MMI_LANG_MALAY__
/**************************************************************

	FUNCTION NAME		: IsMalaySet

  	PURPOSE				: This fn tells whether Malay is set or not

	INPUT PARAMETERS 	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsMalaySet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_MALAY)==0)
		return 1;
	return 0;
}
#endif

#ifdef __MMI_LANG_MYANMAR__
pBOOL IsMyanmarSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC, SSC_MYANMAR)==0)
		return 1;
	return 0;
}
#endif
#ifdef __MMI_LANG_TAMIL__
pBOOL IsTamilSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC, SSC_TAMIL)==0)
		return 1;
	return 0;
}
#endif
#ifdef __MMI_LANG_PORTUGUESE__
/**************************************************************

	FUNCTION NAME		: IsPortugueseSet

  	PURPOSE				: This fn tells whether Portuguese is set or not

	INPUT PARAMETERS 	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsPortugueseSet(void)
{
	if(strcmp((PS8)gLanguageArray[gCurrLangIndex].aLangSSC,SSC_PORTUGUESE)==0)
		return 1;
	return 0;
}
#endif
/**************************************************************

	FUNCTION NAME		: ReadValueCityNVRAM

  	PURPOSE				: Read city values from NVRAM

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReadValueCityNVRAM(void)
{
	S16 error;
	U8 city_chosen1;
	U8 city_chosen2;
	
	ReadValue(NVRAM_DOWNLOAD_CITY1, &city_chosen1, DS_BYTE ,&error);
	
	if (city_chosen1==0xff)
	{		
		city_chosen1=PhnsetGetHomeCity();
		city_chosen1++;
		WriteValue(NVRAM_DOWNLOAD_CITY1,&city_chosen1,DS_BYTE,&error);
	}
	
	ReadValue(NVRAM_DOWNLOAD_CITY2, &city_chosen2, DS_BYTE ,&error);
	
	if (city_chosen2==0xff)
	{		
		city_chosen2=0;
		WriteValue(NVRAM_DOWNLOAD_CITY2,&city_chosen2,DS_BYTE,&error);
	}
}

/**************************************************************

	FUNCTION NAME		: EntryIdleAppSubLcdWithErrorMsg

  	PURPOSE				: Show Sub LCD idle screen with error message

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryIdleAppSubLcdWithErrorMsg(U8* message,U16 message_icon,U8* history_buffer)
{
    ExecSubLCDCurrExitHandler();
    DinitSubLCDHistory();
    ShowCategory301Screen(message, message_icon, history_buffer);
    SetSubLCDExitHandler(ExitSubLCDIdleScr);
}

/**************************************************************

	FUNCTION NAME		: ShowIdleAppSubLcd

  	PURPOSE				: Show Sub LCD idle screen  

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
	extern U8 gMMIClamState;
#endif
void ShowIdleAppSubLcd(void)
{
	// TODO: 为何要关闭下面的代码?
	//tangjie del begin 20060616
	#if 0
	S8 *CityAbbr1,*CityAbbr2;
	float32 time_difference;
	U8 city_chosen1;
	U8 city_chosen2;
	U8 home_city_sublcd;
	float32 diff1,diff2;
	S16 error=-1;
	BOOL show_category_333_screen = FALSE;

	ExecSubLCDCurrExitHandler();
	DinitSubLCDHistory();
	 
	/* Move to EntryIdleScreen() */
        //g_pwr_context.PrevScreenIndicator = ENTER_IDLE_SCR; 
        g_idle_context.IsSubLcdOnIdleScreen = SUBLCD_ON_IDLE_NORMAL;
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
	if ( gMMIClamState == 1 )
	{
//		gMMIClamState =0;
		ShowCategory310Screen((U8*)get_string(STR_GLOBAL_LOGO),0,NULL);
//		gMMIClamState=1;
	}
	else 
		ShowSubLCDAnalogClockScreen();
#else

#ifdef __MMI_AUDIO_PLAYER__
	if( mmi_audply_is_playing() )
		show_category_333_screen = TRUE;
#endif
#ifdef __MMI_FM_RADIO__
	if( mmi_fmrdo_is_power_on() )
		show_category_333_screen = TRUE;
#endif

	if( show_category_333_screen )
		ShowCategory333Screen();
	else
	{
	ReadValue(NVRAM_DOWNLOAD_CITY1, &city_chosen1, DS_BYTE ,&error);
	ReadValue(NVRAM_DOWNLOAD_CITY2, &city_chosen2, DS_BYTE ,&error);
	home_city_sublcd=PhnsetGetHomeCity();
	home_city_sublcd++;
	
	if (city_chosen1>57)	city_chosen1=1;	
	

	if (city_chosen2>57) city_chosen2=1;
	if((TimeZoneData[city_chosen1].CityAbbr!=STR_WCLOCK_CITY_ABBRE_NONE)&&(TimeZoneData[city_chosen2].CityAbbr!=STR_WCLOCK_CITY_ABBRE_NONE))
	{	
		
		CityAbbr1=GetString((U16)TimeZoneData[city_chosen1].CityAbbr);		
		CityAbbr2=GetString((U16)TimeZoneData[city_chosen2].CityAbbr);
		diff1=TimeZoneData[city_chosen1].CityTimeZone-TimeZoneData[home_city_sublcd].CityTimeZone;
		diff2=TimeZoneData[city_chosen2].CityTimeZone-TimeZoneData[home_city_sublcd].CityTimeZone;

		ShowCategory303Screen((U8*)CityAbbr1,(U8*)CityAbbr2,diff1,diff2,NULL);

	}
	else if(TimeZoneData[city_chosen1].CityAbbr!=STR_WCLOCK_CITY_ABBRE_NONE)	
	{
		CityAbbr1=GetString((U16)TimeZoneData[city_chosen1].CityAbbr);
		time_difference=TimeZoneData[city_chosen1].CityTimeZone-TimeZoneData[home_city_sublcd].CityTimeZone;
		ShowCategory312Screen((U8*)CityAbbr1,time_difference,NULL);

	}
	else if(TimeZoneData[city_chosen2].CityAbbr!=STR_WCLOCK_CITY_ABBRE_NONE)	
	{
		CityAbbr2=GetString((U16)TimeZoneData[city_chosen2].CityAbbr);
		time_difference=TimeZoneData[city_chosen2].CityTimeZone-TimeZoneData[home_city_sublcd].CityTimeZone;
		ShowCategory312Screen((U8*)CityAbbr2,time_difference,NULL);

		}
		else if ((TimeZoneData[city_chosen1].CityAbbr == STR_WCLOCK_CITY_ABBRE_NONE) && (TimeZoneData[city_chosen2].CityAbbr == STR_WCLOCK_CITY_ABBRE_NONE))
		{
			city_chosen1 = home_city_sublcd;
			CityAbbr1 = GetString((U16)TimeZoneData[city_chosen1].CityAbbr);
			time_difference = TimeZoneData[city_chosen1].CityTimeZone - TimeZoneData[home_city_sublcd].CityTimeZone;
			ShowCategory312Screen((U8*)CityAbbr1, time_difference, NULL);
		}
	}
#endif
	SetSubLCDExitHandler(ExitSubLCDIdleScr);
#endif
//tangjie del end 20060616
}


/**************************************************************

	FUNCTION NAME		: SubLCDIdleScreen

  	PURPOSE				: Displays the idle screen of sub-LCD

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 SubLCDIdleScreen(void *funcptr)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	//MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_SUBLCD_IDLE_SCRN_HDLR));
	PRINT_INFORMATION( ("SubLCDIdleScreen") );

	if( g_pwr_context[0].PrevScreenIndicator == ENTER_PIN_SCR)
	{
		EntryIdleAppSubLcdWithErrorMsg((PU8)GetString(STR_SUBLCD_ENTER_PIN_MSG),0,NULL);
	}
	else if((g_pwr_context[0].PrevScreenIndicator == ENTER_PUK_SCR )||(g_pwr_context[0].PrevScreenIndicator == ENTER_CONFIRM_PIN)||(g_pwr_context[0].PrevScreenIndicator == ENTER_NEW_PIN))
	{
		EntryIdleAppSubLcdWithErrorMsg((PU8)GetString(STR_SUBLCD_ENTER_PUK_MSG),0,NULL);
	}
	else if(g_pwr_context[0].PrevScreenIndicator == ENTER_PHONELOCK )
	{
		EntryIdleAppSubLcdWithErrorMsg((PU8)GetString(STR_SUBLCD_ENTER_PHONELOCK_MSG),0,NULL);
	}
	else if(g_pwr_context[0].PrevScreenIndicator == ENTER_SIM_INSERTION )
	{
		EntryIdleAppSubLcdWithErrorMsg((PU8)GetString(STR_SUBLCD_SIM_INSERTION_MSG),0,NULL);
	}
	else if(g_pwr_context[0].PrevScreenIndicator == ENTER_SIM_BLOCK )
	{
		EntryIdleAppSubLcdWithErrorMsg((PU8)GetString(STR_SUBLCD_SIM_BLOCK_MSG),0,NULL);
	}
	else if(g_pwr_context[0].PrevScreenIndicator == ENTER_IDLE_SCR )
	{
	/*	City names and time difference need to be added here	*/
		ShowIdleAppSubLcd();
	}
	else
	{
		ExecSubLCDCurrExitHandler();
		DinitSubLCDHistory();
		ShowCategory302Screen(NULL);
		SetSubLCDExitHandler(ExitSubLCDIdleScr);
	}
#endif

    return 0;
}


/**************************************************************

	FUNCTION NAME		: ExitSubLCDIdleScr

  	PURPOSE				: Exit sublcd idle screen  

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSubLCDIdleScr(void)
{
	SubLCDHistoryNode SubLCDHistory;

	g_idle_context.IsSubLcdOnIdleScreen = SUBLCD_NOT_ON_IDLE;

	SubLCDHistory.entryFuncPtr=ShowIdleAppSubLcd;

	AddSubLCDHistory(&SubLCDHistory);
}

U8 IdleIsPowerOnPeriod(void)
{
    return g_pwr_context[0].PowerOnPeriod;
}

 
/*****************************************************************************
* FUNCTION
*	 IdleSetLRKey()
* DESCRIPTION
*   Change Idlescreen's L or RSK according to network status
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

#if defined(__MMI_NOKIA_STYLE_N800__)||defined(__MMI_MAINLCD_220X176__)|| defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
extern void mmi_shortcut_right_move_event_up(void);
extern void mmi_shortcut_right_move_event_down(void);
extern void mmi_shortcut_left_move_event_up(void);
extern void mmi_shortcut_left_move_event_down(void);
extern void mmi_shortcut_down_move(void);
extern void mmi_shortcut_up_move(void);
extern void EntryMainMove(void);
extern void YIZHEMEZUFU(void);
extern void YITHEMEZUFU2(void);

#ifdef __PRJ_WITH_SPICAM__
extern void mmi_camera_entry_option_menu_screen(void);
#else
extern void mmi_camera_entry_app_screen(void);
#endif

extern void mmi_shortcut_enter_event_up(void);
extern  void EntrySMSMainMenuList(void);
#endif


void IdleSetLRKey(void)
{
	if(gKeyPadLockFlag)
	{
		#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__  
		ChangeRightSoftkey(KEYPAD_LOCKED_RSK_TEXT,0);
			idleRStringtID = KEYPAD_LOCKED_RSK_TEXT;
                    #endif
		#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__  
        		ChangeLeftSoftkey(KEYPAD_LOCKED_RSK_TEXT,0);
			idleLStringtID = KEYPAD_LOCKED_RSK_TEXT;
                    #endif

			

			#ifdef __MMI_TOUCH_SCREEN__
#if defined(__MMI_MAINLCD_220X176__)
				SetKeyHandler(mmi_e71_idle_lsk_down_info,KEY_LSK,KEY_EVENT_DOWN);
#else
				SetKeyHandler(EntryScrAfterPessingRSK,KEY_RSK,KEY_EVENT_DOWN);
#endif
			#else
				#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__  
				SetRightSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_DOWN);
				#endif

				#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__  
				SetLeftSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_DOWN);
				#endif
			#endif
		}
	else //keypad unlocked
	{
	#ifdef __MANUAL_MODE_NW_SEL__
		if(g_pwr_context[0].CurrentServiceType == LIMITED_SERVICE
		&& g_pwr_context[0].PLMNSelectionMode == MANUAL_MODE)
		{
			#ifdef __MMI_SWAP_LSK_RSK_IN_IDLE_SCREEN__
			SetRightSoftkeyFunction(EntryMainMenuFromIdleScreen,KEY_EVENT_UP);  
			ChangeLeftSoftkey(STR_GLOBAL_SELECT,0);
			SetLeftSoftkeyFunction(NETSETGetPLMNListCheck,KEY_EVENT_UP);
			#else
			SetLeftSoftkeyFunction(EntryMainMenuFromIdleScreen,KEY_EVENT_UP);  
			ChangeRightSoftkey(STR_GLOBAL_SELECT,0);
			SetRightSoftkeyFunction(NETSETGetPLMNListCheck,KEY_EVENT_UP);
			#endif
			idleRStringtID = STR_GLOBAL_SELECT;
		}
		else
	#endif
		{
			#ifdef __MMI_SWAP_LSK_RSK_IN_IDLE_SCREEN__
			SetRightSoftkeyFunction(EntryMainMenuFromIdleScreen,KEY_EVENT_UP);  
			ChangeLeftSoftkey(IDLE_SCREEN_LSK_TEXT,0);
			SetLeftSoftkeyFunction(mmi_phb_idle_enter_phb_list,KEY_EVENT_UP);
			#else
			 
// Beginning--added by kecx for flashlight shortcut key on 20081120
#ifdef __MOD_FLASHLIGHT__
		#if defined(__MMI_BLACKBERRY_QWERTY__)
			SetKeyHandler(SwitchFlashlightStatus,KEY_SPACE1,KEY_LONG_PRESS); 
		#else
            SetKeyHandler( SwitchFlashlightStatus,  KEY_UP_ARROW,  KEY_LONG_PRESS);
		#endif
#endif
// End--added by kecx for flashlight shortcut key on 20081120
#ifdef __MMI_MULTI_SIM__
			if (MTPNP_AD_Get_UsableSide_Number() == 0)
			{
				SetLeftSoftkeyFunction(EntryScrSimOptionMenu, KEY_EVENT_UP);
				#ifdef __MMI_TOUCH_SCREEN__
				mmi_pen_set_shortcut_entry(SHORTCUT_MAINMENU, EntryScrSimOptionMenu);
				#endif
			}
			else
			{
              #ifndef __MMI_USE_LSK_AS_SEND2__
				SetLeftSoftkeyFunction(EntryMainMenuFromIdleScreen, KEY_EVENT_UP);
              #else
              		SetLeftSoftkeyFunction(DummyKeyHandler,KEY_EVENT_UP); 
				SetLeftSoftkeyFunction(CHISTGetCallLogBySENDKey_SIM2,KEY_EVENT_DOWN); 
              #endif
				#ifdef __MMI_TOUCH_SCREEN__
				mmi_pen_set_shortcut_entry(SHORTCUT_MAINMENU, EntryMainMenuFromIdleScreen);
				#endif
			}

#else
			SetLeftSoftkeyFunction(EntryMainMenuFromIdleScreen, KEY_EVENT_UP);  
			#ifdef __MMI_TOUCH_SCREEN__
			mmi_pen_set_shortcut_entry(SHORTCUT_MAINMENU, EntryMainMenuFromIdleScreen);
			#endif

#endif

#ifdef __FLIGHT_MODE_SUPPORT__
			if (mmi_bootup_get_active_flight_mode() != FLIGHTMODE_SILENT)
#endif
			{
#ifdef __MMI_MULTI_SIM__
				if (MTPNP_AD_Get_UsableSide_Number() == 0)
				{
					ChangeRightSoftkey(STR_RSK_SOS, 0);
					idleRStringtID = STR_RSK_SOS;
//					idleLStringtID = STR_GLOBAL_OPTIONS;
					idleLStringtID = IDLE_SCREEN_LSK_TEXT;
					SetRightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);
					SetLeftSoftkeyFunction(EntryScrSimOptionMenu,KEY_EVENT_UP);
					#ifdef __MMI_TOUCH_SCREEN__
					mmi_pen_set_shortcut_entry(SHORTCUT_MAINMENU, EntryScrSimOptionMenu);
					#endif
				}
				else					
#endif
				{
					idleRStringtID = IDLE_SCREEN_RSK_TEXT;
					idleLStringtID = IDLE_SCREEN_LSK_TEXT;
					SetRightSoftkeyFunction(mmi_phb_idle_enter_phb_list, KEY_EVENT_UP);
				}
			}
		#ifdef __FLIGHT_MODE_SUPPORT__
		#ifdef __MMI_MULTI_SIM__
			if(MTPNP_AD_Is_Flight_Mode())
		#else
			if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
		#endif
			{
				SetLeftSoftkeyFunction(EntryScrSimOptionMenu,KEY_EVENT_UP);
			}
		#endif

#endif
		}
	}
#if defined(__MMI_NOKIA_STYLE_N800__)|| defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)||(defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_TOUCH_SCREEN__))

    if(gKeyPadLockFlag)
    {
        ClearKeyHandler(KEY_CLEAR,KEY_EVENT_UP);
        ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
        ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_UP);
       // #if !defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
        ClearKeyHandler(KEY_DOWN_ARROW, KEY_EVENT_UP);
        ClearKeyHandler(KEY_UP_ARROW, KEY_EVENT_UP);
       // #endif
        ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);

    }
    else
    {
        SetKeyHandler(mmi_shortcut_left_move_event_down,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(mmi_shortcut_left_move_event_up,KEY_LEFT_ARROW,KEY_EVENT_UP);
		SetKeyHandler(mmi_shortcut_right_move_event_down,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(mmi_shortcut_right_move_event_up,KEY_RIGHT_ARROW,KEY_EVENT_UP);
		SetKeyHandler(mmi_shortcut_down_move, KEY_DOWN_ARROW, KEY_EVENT_UP);
        SetKeyHandler(mmi_shortcut_up_move, KEY_UP_ARROW, KEY_EVENT_UP);
	#if defined(__MMI_BLACKBERRY_IDLE_SHORTCUT__)
        SetKeyHandler(mmi_shortcut_enter_event_up, KEY_ENTER, KEY_EVENT_UP);
		if(!gKeyPadLockFlag)
	{
		if(IsSilentModeActivated())
			SetKeyHandler(DeactivateSilentProfile, KEY_DOLLAR,KEY_EVENT_UP);	
		else
			SetKeyHandler(ActivateSilentProfile,KEY_DOLLAR,KEY_EVENT_UP);
		SetKeyHandler(mmi_audply_entry_main,KEY_MUSIC,KEY_EVENT_UP);
		SetKeyHandler(EntryDMCHISTMainMenu,KEY_SEND3,KEY_EVENT_UP);
	}
	#else
        YITHEMEZUFU2();
	#endif
    }
#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_idle_restart_keypad_lock_timer
* DESCRIPTION
*	
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	void
*****************************************************************************/
void mmi_idle_restart_keypad_lock_timer(void)
{
	if(gKeyPadLockFlag == 1)
		return;

	if( g_idle_context.AutoLockTimeOut != 0 && (g_idle_context.IsOnIdleScreen == 1 || g_idle_context.IsOnSimErrorScreen == 1) )
	{
		StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
		if( g_idle_context.IsOnIdleScreen == 1 )
			StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, IdleSetKeyPadLockFlag);
		if( g_idle_context.IsOnSimErrorScreen == 1 )
			StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, InsertSimSetKeyPadLockFlag);
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_idle_restart_screensaver_timer
* DESCRIPTION
*	
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	void
*****************************************************************************/
void mmi_idle_restart_screensaver_timer(void)
{
	U8 ScreenSaverStatus, ScreenSaverWait;

	if(g_idle_context.ScreenSaverRunFlag == 1) /* screensaver is running */
		return;

	if(g_idle_context.IsOnIdleScreen == 1)
	{
		PhnsetGetScrSvrSetings(&ScreenSaverStatus, &ScreenSaverWait);
		if( ScreenSaverStatus == 0xFF )
			PhnsetRstScrSvrDefault();

		if( ScreenSaverStatus == 1 ) /* screensaver is active */
		{
			StopTimer(SCREENSAVER_TIMER);
			StartTimer(SCREENSAVER_TIMER, ScreenSaverWait*1000, EntryIdleSetScreenSaver);
		}
	}
}

 
#ifdef __FLIGHT_MODE_SUPPORT__

/*****************************************************************************
* FUNCTION
*   EntryChangeFlightModeFromIdle
*****************************************************************************/
#ifdef __MMI_MULTI_SIM__
extern void EntryStartupModeSetScreen(void);
#endif
void EntryChangeFlightModeFromIdle(void)
{
	 
	// TODO: 为何要关闭下面的代码?
	//g_idle_context.ToNameScrFromIdleApp = 1;
#ifdef __MMI_MULTI_SIM__
	EntryStartupModeSetScreen();
#else
	g_idle_context.RskPressedFromIdleApp = 1;
	EntryPhnsetFlightMode();
#endif
}
/*****************************************************************************
* FUNCTION
*   DisplayFlightModeWarn
*****************************************************************************/
void DisplayFlightModeWarn(void)
{
	#if defined(__MMI_MULTI_SIM__)
	DisplayPopup((PU8)GetString(STRING_MTPNP_DM_FLIGHT) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
	#endif
}
/*****************************************************************************
* FUNCTION
*   EntryFlightModeIdleScreen
*****************************************************************************/

void EntryFlightModeIdleScreen(void)
{
	/* change current service type */
//	g_pwr_context.CurrentServiceType = NO_SERVICE;          delete for fixing bug 16650
	
	mmi_trace(g_sw_IDLE,"EntryFlightModeIdleScreen");

	//EntryNewScreen(SCR_FLIGHT_MODE_IDLE, EntryFlightModeIdleScreen, NULL, NULL);

	StopLEDPatternNoService();

	/* display "Flight Mode" in the service area of idle screen */
	IdleRefreshServiceIndicationArea();

#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
#ifdef __FLIGHT_MODE_SUPPORT__
	if (mmi_bootup_get_active_flight_mode() != 1)
#endif
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
#endif
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
			ShowCategorySymbianScreen(STR_GLOBAL_OPTIONS,
										IDLE_SCREEN_LSK_ICON, 
										STR_FLIGHT_MODE_RSK, 
										IDLE_SCREEN_RSK_ICON, 
										SYMBIAN_TOTAL_ITEMS, 
										pda_idle_shortcut, 
										0, 
										NULL);
	#else
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	ShowCategory33Screen(	/*IDLE_SCREEN_LSK_TEXT*/STR_GLOBAL_OPTIONS, IDLE_SCREEN_LSK_ICON, /* Menu */
				STR_FLIGHT_MODE_RSK/*IDLE_SCREEN_RSK_TEXT*/, IDLE_SCREEN_RSK_ICON, NULL);
	#endif/*+ zhouqin 2011-02-16 modify for q6 320X240 */

	/* reset icons to indicate no service available */
	ChangeStatusIconLevel(STATUS_ICON_SIGNAL_STRENGTH, 0); 
	ChangeStatusIconLevel(STATUS_ICON_SUBLCD_SIGNAL_STRENGTH, 0);
	//IdleResetStatusIcon(STATUS_ICON_ROAMING);

	SetLeftSoftkeyFunction(EntryScrSimOptionMenu,KEY_EVENT_UP);
#ifdef __MMI_SCREEN_ROTATE__
    if (mmi_frm_is_screen_width_height_swapped())
    {
        idle_screen_sat_display_text_background_redraw(
            MMI_ROTATED_BUTTON_BAR_X + 1 /* TEMP FIX */ ,
            MMI_ROTATED_BUTTON_BAR_Y,
            MMI_ROTATED_BUTTON_BAR_X + MMI_ROTATED_BUTTON_BAR_WIDTH - 1,
            MMI_ROTATED_BUTTON_BAR_Y + MMI_ROTATED_BUTTON_BAR_HEIGHT - 1);
    }
    else
#endif /* __MMI_SCREEN_ROTATE__ */ 
        idle_screen_sat_display_text_background_redraw(
            0,
            UI_device_height - MMI_button_bar_height,
            UI_device_width - 1,
            UI_device_height - 1);	

	idleLStringtID = IDLE_SCREEN_LSK_TEXT;
	idleRStringtID = IDLE_SCREEN_RSK_TEXT;
	ChangeLeftSoftkey(IDLE_SCREEN_LSK_TEXT, 0);//qiff add for greenstone: bug:10042
	ChangeRightSoftkey(IDLE_SCREEN_RSK_TEXT/* Mode STR_FLIGHT_MODE_RSK*/, 0);
	SetRightSoftkeyFunction(mmi_phb_idle_enter_phb_list,KEY_EVENT_UP);  

#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
	SetCenterSoftkeyFunction(FlightModeDigitHandler, KEY_EVENT_UP);
#endif

#ifdef	__MMI_TOUCH_DIAL_SCREEN__
	memset((U8*)g_idle_context.DialPadCallBuffer,0,sizeof((U8*)g_idle_context.DialPadCallBuffer));
#endif		

	/* Voice Recognition */
#if defined(__MMI_VRSD__)
	SetKeyHandler(mmi_vrsd_rcg_pre_entry, KEY_RSK, KEY_LONG_PRESS);
#endif

#if defined(__MMI_VRSI__)
	SetKeyHandler(mmi_vrsi_rcg_pre_entry, KEY_RSK, KEY_LONG_PRESS);
#endif	

#ifndef __MMI_NOKIA_STYLE_N800__
	#ifdef	__MMI_FM_RADIO__
    SetKeyHandler(mmi_fmrdo_run_app, KEY_FM, KEY_EVENT_UP);
	#endif
    SetKeyHandler(mmi_audply_entry_main, KEY_MUSIC, KEY_EVENT_UP);
#endif

#ifdef __MMI_CAMERA__
	#ifndef __MMI_SCREEN_SNAPSHOT__ 
		#ifdef __MMI_IDLE_CAMERA_KEY_TO_STOP_AUDIOPLAYER__
			SetKeyHandler(CheckAndStopAudioPlayer, CAMERA_SHORTCUT_KEY, KEY_EVENT_DOWN);
		#else
                #ifdef __PRJ_WITH_SPICAM__
                    SetKeyHandler(mmi_camera_entry_option_menu_screen, CAMERA_SHORTCUT_KEY, KEY_EVENT_DOWN);
                #else
                    SetKeyHandler(mmi_camera_entry_app_screen, CAMERA_SHORTCUT_KEY, KEY_EVENT_DOWN);
                #endif
		#endif
	#else 
		#ifdef __MMI_IDLE_CAMERA_KEY_TO_STOP_AUDIOPLAYER__
			SetKeyHandler(CheckAndStopAudioPlayer, CAMERA_SHORTCUT_KEY, KEY_EVENT_UP);
		#else
                #ifdef __PRJ_WITH_SPICAM__
                    SetKeyHandler(mmi_camera_entry_option_menu_screen, CAMERA_SHORTCUT_KEY, KEY_EVENT_DOWN);
                #else
                    SetKeyHandler(mmi_camera_entry_app_screen, CAMERA_SHORTCUT_KEY, KEY_EVENT_DOWN);
                #endif
		#endif
	#endif /* __MMI_SCREEN_SNAPSHOT__ */
#endif  /* __MMI_CAMERA__ */
#ifdef __MOD_FLASHLIGHT__
		#if defined(__MMI_BLACKBERRY_QWERTY__)
			SetKeyHandler(SwitchFlashlightStatus,KEY_SPACE1,KEY_LONG_PRESS); 
		#else
            SetKeyHandler(SwitchFlashlightStatus,  KEY_UP_ARROW,  KEY_LONG_PRESS);
		#endif
#endif
#if defined(__PROJECT_GALLITE_C01__)
	#ifdef PRJ_F021
		#ifdef MMI_FMRDO_SHORTCUT_SUPPORT
			SetKeyHandler(mmi_fmrdo_run_app,KEY_FM,KEY_EVENT_UP);
		#else				
			SetKeyHandler(mmi_audply_entry_main,KEY_FM,KEY_EVENT_UP);
		#endif
		//SetKeyHandler(mmi_lamp_run_app,KEY_ENTER,KEY_LONG_PRESS);
	#endif

    #ifdef __MUSIC_KEY_SUPPORT__
        SetKeyHandler(mmi_audply_entry_main,KEY_MUSIC,KEY_EVENT_UP);
    #endif
#endif
	ForceSubLCDScreen(ShowIdleAppSubLcd);
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_enable_quick_launch();
	mmi_pen_set_shortcut_entry(SHORTCUT_MAINMENU, EntryScrSimOptionMenu);
	mmi_pen_set_shortcut_entry(SHORTCUT_MESSAGES, DisplayFlightModeWarn);
#endif
}


/*****************************************************************************
* FUNCTION
*   EntryFlightModeKeypadLockScreen
*****************************************************************************/
void EntryFlightModeKeypadLockScreen(void)
{
	U16 KeyLockLSKStringID=0, KeyLockRSKStringID=0, KeyLockLSKIconID=0, KeyLockRSKIconID=0;

#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
#if defined(__MMI_MAINLCD_220X176__) && defined(__MMI_TOUCH_SCREEN__)
	KeyLockRSKStringID = SCREEN_LOCKED_TEXT; 
#else
	KeyLockRSKStringID = KEYPAD_LOCKED_RSK_TEXT;
#endif
	KeyLockRSKIconID = 0;
#endif

#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__
#if defined(__MMI_MAINLCD_220X176__) && defined(__MMI_TOUCH_SCREEN__)
	KeyLockLSKStringID = SCREEN_LOCKED_TEXT; 
#else
	KeyLockLSKStringID= KEYPAD_LOCKED_RSK_TEXT;
#endif
	KeyLockLSKIconID = 0;
#endif

	IdleRefreshServiceIndicationArea();
        /*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
		ShowCategorySymbianScreen(
				KeyLockLSKStringID,
				KeyLockLSKIconID,
				KeyLockRSKStringID,
				KeyLockRSKIconID,
				SYMBIAN_TOTAL_ITEMS, 
				pda_idle_shortcut, 
				0, 
				NULL);
	#else
	/*- zhouqin 2011-02-16 modify for q6 320X240 */

	ShowCategory33Screen(	KeyLockLSKStringID, KeyLockLSKIconID, 
				KeyLockRSKStringID, KeyLockRSKIconID, NULL);
	#endif /*+ zhouqin 2011-02-16 modify for q6 320X240 */
#ifndef __MMI_MAINLCD_220X176__
#if defined(__MMI_KEYPAD_LOCK_PROMPT_TEXT__)
	SetGroupKeyHandler(EntryScrKeyPadLockOnPessingAnyKey,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,(U16)KEY_EVENT_DOWN);
#endif
#endif
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_block();  
#endif

#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__  
	SetRightSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_UP);
#endif

#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__  
	SetLeftSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_UP);
#endif

#if defined(__MMI_MAINLCD_220X176__) && defined(__MMI_TOUCH_SCREEN__)
	SetKeyHandler(mmi_e71_idle_lsk_down_info,KEY_LSK,KEY_EVENT_UP);
#endif
	if(g_idle_context.IsRSKPressOnScreenSaver)
	{
		EntryScrAfterPessingRSK();
		g_idle_context.IsRSKPressOnScreenSaver=0;
	}
}


/*****************************************************************************
* FUNCTION
*   EntryFlightModeIdleScreen
*****************************************************************************/
void FlightModeDigitHandler(void)
{
	U8* guiBuffer;
#ifdef __MMI_TOUCH_DIAL_SCREEN__
	U16 TouchDialPadKeys[MAX_TOUCH_DIAL_PAD_KEYS] = {KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_STAR,KEY_POUND};
#endif

	EntryNewScreen(IDLE_SCREEN_DIGIT_HANDLER_ID, ExitIdleScreenDigitHandler, NULL, NULL);
#if defined  (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
    mmi_idle_set_dual_sim_dialing_keypad_call_handler(NULL, NULL,NULL,NULL);
#endif
#endif

	DeleteScreenIfPresent(IDLE_SCREEN_DIGIT_HANDLER_ID);

	g_idle_context.IsOnDialerScreen=1;
	g_idle_context.AvoidHistoryofDialerScreen=0;
	guiBuffer = GetCurrGuiBuffer(IDLE_SCREEN_DIGIT_HANDLER_ID);
#ifdef __FT_IDLE_SMART_DIAL__
	if(IsSmartDialStyle())
	{
		 ShowCategorySmartDialScreen(STR_GLOBAL_SAVE ,
		 											0,
		 											STR_GLOBAL_BACK,
		 											0,
		 											0,
		 											mmi__smartdial_search_list_get_item, 
		 											NULL,
		 											0,
		 											0,
		 											(U8*)g_idle_context.DialPadCallBuffer, 
		 											MAX_DIAL_PAD ,
		 											guiBuffer );
	}
	else
#endif
	ShowCategory16Screen(STR_GLOBAL_SAVE, 0, STR_GLOBAL_BACK, 0, (U8*)g_idle_context.DialPadCallBuffer, MAX_DIAL_PAD, guiBuffer);

	if(g_idle_context.IdleScreenDigitKeyCode>=0)
		ExecuteCurrKeyHandler(g_idle_context.IdleScreenDigitKeyCode, KEY_EVENT_DOWN);

	//SetKeyHandler(EntryScrEmergencyCallDialing,KEY_SEND,KEY_EVENT_UP); /* no emergency call allowed */
	SetCategory16RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	 
	SetLeftSoftkeyFunction(IdleDialPadSavePhoneBook,KEY_EVENT_UP);
	SetKeyHandler(IdleAbbrevDialling,KEY_POUND,KEY_EVENT_UP);

#ifdef __MMI_STARKEY_TOGGLE_PROFILE__
       if(( g_idle_context.LongPressEnable==0 ) && !history_buffer) // Lisen  (!history_buffer) to check if it is back from history, see Cannon+ SCR 1847
#else

	#ifdef __MMI_TOUCH_DIAL_SCREEN__		
		if((g_idle_context.LongPressEnable==0) && (g_idle_context.IdleScreenDigitKeyCode!=KEY_STAR) && (g_idle_context.IdleScreenDigitKeyCode!=-1) )
	#else
		if((g_idle_context.LongPressEnable==0) && (g_idle_context.IdleScreenDigitKeyCode!=KEY_STAR) && !guiBuffer)
	#endif
#endif
	{
		#ifdef __MMI_TOUCH_DIAL_SCREEN__
		if ( !guiBuffer )
		#endif
			g_idle_context.AvoidHistoryofDialerScreen=1;

#ifdef __MMI_STARKEY_TOGGLE_PROFILE__
		SetKeyHandler(IdleToggleProfile,KEY_STAR,KEY_LONG_PRESS);
#else
#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_160X128__) 
		SetKeyHandler(IdleToggleProfile,KEY_POUND,KEY_LONG_PRESS);
#endif
#endif

#ifdef __MMI_POUNDKEY_ENTER_PROFILE__
		SetKeyHandler(EntryScrProfiles,KEY_POUND,KEY_LONG_PRESS);
#endif
#ifdef __MMI_TOUCH_DIAL_SCREEN__
		SetGroupKeyHandler(IdleDisableLongPress,TouchDialPadKeys,(U8)MAX_TOUCH_DIAL_PAD_KEYS,KEY_EVENT_UP);
#else
		SetGroupKeyHandler(IdleDisableLongPress,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,KEY_EVENT_UP);
#endif
	}
 #ifdef __MMI_STARKEY_TOGGLE_PROFILE__
       if(( g_idle_context.LongPressEnable==0 ) && !guiBuffer) // Lisen  (!history_buffer) to check if it is back from history, see Cannon+ SCR 1847
#else

	#ifdef __MMI_TOUCH_DIAL_SCREEN__		
	/* history buffer condition removed as to restore the key handlers after popup */
	/* g_idle_context.IdleScreenDigitKeyCode=-1 means dial screen shows when user click on CSK */
	 
	// TODO: 为何要关闭下面的代码?
//		if((g_idle_context.LongPressEnable==0) && (g_idle_context.IdleScreenDigitKeyCode!=KEY_STAR) && (g_idle_context.IdleScreenDigitKeyCode!=-1) )
		if(g_idle_context.LongPressEnable==0 && (g_idle_context.IdleScreenDigitKeyCode==KEY_STAR) )
	#else
		if((g_idle_context.LongPressEnable==0) && !guiBuffer && (g_idle_context.IdleScreenDigitKeyCode==KEY_STAR) )
	#endif
#endif
{
	/*zhangm Add Start For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
		SetKeyHandler(IdleHandleStarKeyForKeypadLock,KEY_STAR,KEY_LONG_PRESS);
	/*zhangm end end For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
}                                                                                                                                          
#ifdef __MMI_TOUCH_DIAL_SCREEN__
	SetKeyHandler(IdleAbbrevDialling,KEY_POUND,KEY_EVENT_UP);
#endif
}

#endif


/*****************************************************************************
 * FUNCTION
 *  mmi_idle_is_allow_to_show_song_name
 * DESCRIPTION
 *  Is idle screen allow to display song name in line 3?
 * PARAMETERS
 *  void
 * RETURNS
 *  U8
 *****************************************************************************/
BOOL mmi_idle_is_allow_to_display_song_name(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_OP11_HOMESCREEN__
    if (mmi_hs_is_enable())
    {
        return MMI_FALSE;
    }
#endif /* __MMI_OP11_HOMESCREEN__ */

#ifdef __MMI_DUAL_SIM_MASTER__
    if (MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_DUALCARD || MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_FLIGHT)
    {
        return MMI_TRUE;
    }
    else if (MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD1 && MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_VALID)
    {
        return MMI_TRUE;
    }
    else if (MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD2 && MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_VALID)
    {
        return MMI_TRUE;
    }
#else /* __MMI_DUAL_SIM_MASTER__ */
    if (mmi_bootup_is_sim_valid())
    {
    	return MMI_TRUE;
    }
#endif /* __MMI_DUAL_SIM_MASTER__ */
#ifdef __MMI_SIMAP_SUPPORT__
    else if (mmi_bootup_get_active_bt_access_profile_mode())
    {
        return MMI_TRUE;
    }
#endif /* __MMI_SIMAP_SUPPORT__ */
#ifdef __MMI_WLAN_FEATURES__
    else if (mmi_netset_get_active_preferred_mode() != P_GSM_ONLY)
    {
    	return MMI_TRUE;
    }
#endif /* __MMI_WLAN_FEATURES__ */
    else
    {
    	return MMI_FALSE;
    }
}





 

