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
 *	 PopulateRes.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Populate Resource Engine
 *
 * Author:
 * -------
 * -------
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#define _POPULATE_RES_C

 /****************************************************************************
* Include Files                                                                
*****************************************************************************/
#include "mmi_features.h"
#include "globalmenuitems.h"
#include "custdatares.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "fontdcl.h"
#include "debuginitdef.h"
#include "callsdefs.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "wgui_categories_defs.h"
#include "satgprots.h"
#include "shortcutsgprots.h"
#include "bootup.h"

#ifdef DEVELOPER_BUILD_FIRST_PASS
/***************************************************************************** 
*
* FIRST_PASS is for Resgen on PC
*
*****************************************************************************/
#include "custresdef.h"
#include	"settingdefs.h"


/***************************************************************************** 
* Define
*****************************************************************************/
#define 	OUTPUT_IMAGE_ID_FILENAME 		CUST_IMG_PATH"\\\\images_file_list.ini"
#define 	OUTPUT_MENUTREE_ID_FILENAME 	"..\\..\\Customer\\CustResource\\CustMenuTreeID_Out.c"


/***************************************************************************** 
* Local Variable
*****************************************************************************/
/* for generate String list file in first pass */
FILE 	*StringListFile =NULL;
FILE 	*EnumListFile = NULL;
FILE 	*OutputImgIDFile = NULL;
FILE 	*OutputMenutreeIDFile = NULL;
FILE		*RepeatIDListFile = NULL; 
U16 	gCurrLangIndex;

U16	CurrMaxStringNum;
U32 	CurrTotalStrLen;
U16 	CurrMaxStringId;
U16 	CurrMaxStringIdSearch;

/***************************************************************************** 
* Local Function
*****************************************************************************/
void PopulateGlobalData(void);

/****************************************************************************
* Global Variable - Extern                                                                 
*****************************************************************************/
extern U16  CurrMaxImageId;
extern U16	CurrMaxImageNum;
 
extern U16  CurrMaxImageIdEXT; 
extern U16	CurrMaxImageNumEXT; 
 

extern U16 	CurrMaxMenuItemIndex;  
extern U16 	CurrMaxStringId;
extern HWND hWnd;

#ifdef MMI_ON_HARDWARE_P
	extern const CUSTOM_IMAGE_MAP ImageIdMap[];
	extern const CUSTOM_IMAGE_MAP ImageIdMapEXT[];
	extern const CUSTOM_MENU  		nCustMenus[MAX_MENU_ITEMS];
#else
	extern CUSTOM_IMAGE_MAP 		ImageIdMap[];
	extern CUSTOM_IMAGE_MAP 		ImageIdMapEXT[];
	extern CUSTOM_MENU  				nCustMenus[MAX_MENU_ITEMS];
#endif

#ifdef MMI_ON_HARDWARE_P
	const CUSTOM_STRING			*gpStringRes;
	const CUSTOM_STRING_MAP		*gpStringMapRes;
	const CUSTOM_STRMAP_SEARCH	*gpStrMapSearch;
	extern StringResList			*gStringList;
#else
	CUSTOM_STRING					*gpStringRes;
	CUSTOM_STRING_MAP				*gpStringMapRes;
	CUSTOM_STRMAP_SEARCH			*gpStrMapSearch;
	extern StringResList			*gStringList;
#endif

 
extern int toolFlag; 
 

/****************************************************************************
* Global Function - Extern                                                                 
*****************************************************************************/

/* Populate without compile option */
extern void PopulateMainMenuRes(void);
extern void PopulateIdleMenuRes(void);
extern void PopulatePbResData(void);
extern void PopulateMainDemoRes(void);
extern void PopulateCmResData(void);
extern void PopulateQuickAccessData(void);
extern void populateSettingMenu(void);
extern void populateFunAndGamesMenu(void);
extern void PopulateDownloadRes(void);
extern void PopulateJavaRes(void);
extern void PopulateResData(void);
extern void PopulateSimDetectionMenuRes(void);
extern void PopulateprofileMenu(void);
extern void PopulateSSCRes(void);
extern void PopulateCallHistoryRes(void);
extern void PopulateHelpResources(void);
extern void PopulateDataAccountMenuList(void);
extern void PopulateChgrPwrOnScrRes(void);
extern void PopulateOrganizerRes(void);
extern void PopulateAlarmRes(void);
extern void PopulateSATRes(void);
extern void PopulateSpofRes(void);
extern void PopulateResourcesCal(void);
extern void PopulateShortcutsRes(void);
extern void PopulateToDoList(void);
extern void PopulateScreenSaverRes(void);
extern void PopulateCommonScreensResData(void);
extern void PopulateEMSObjectResources(void);
extern void PopulateInputMethodRes(void);
extern void PopulatevObjectRes(void);
extern void PopulateVRSDRes(void);
extern void PopulateWapRes(void);
extern void PopulateMmiapiRes(void); 

/* Populate with compile option */
#if defined(__MMI_NITZ__)
	extern void PopulateResourceNITZ(void);
#endif

#if defined(__MMI_MESSAGES_CHAT__)
	extern void PopulateChatAppRes(void);
#endif

#if defined(__MMI_ENGINEER_MODE__)
	extern void populateEngineerModeMenu(void);
#endif

#if defined(__MMI_FACTORY_MODE__)
	extern void populateFactoryModeMenu(void);
#endif

#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)
	extern void PopulateComposeRingTone(void);
#endif

#if defined(__MMI_AUDIO_PLAYER__)
	extern void populateAudioPlayerMenu(void);
#endif

#if defined(__MMI_AB_REPEATER__)
	extern void populateABRepeaterMenu(void);
#endif

#if defined(__MMI_FILE_MANAGER__)
	extern void populateFileManagerMenu(void);
#endif

#if defined(__MMI_SOUND_RECORDER__)
	extern void populateSoundRecorderMenu(void);
#endif

#if defined(__MMI_CAMERA__)
	extern void PopulateCameraRes(void);
	extern void PopulateImageViewerRes(void);
#endif

#if defined(__MMI_VIDEO__)
	extern void PopulateVdoPlyRes(void);
	extern void PopulateVdoRecRes(void);
#endif

#if defined(__MMI_FM_RADIO__)
	extern void PopulateFMRadioRes(void);
#endif

#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
	extern void PopulateFMScheduleRecRes(void);
#endif

#if defined(__MOD_SMSAL__)
	extern void PopulateMessagesResData(void);
#endif

#if defined(__MMI_STOPWATCH__)
	extern void PopulateStopwatchRes(void);
#endif

#if defined(__MMI_PHOTOEDITOR__)
	extern void PopulatePhotoEditorRes(void);
#endif

#if defined(__MMI_EXT_DEVICE__)

   #ifdef __MMI_USB_SUPPORT__
   	extern void PopulateExtDevUSBRes(void);
   #endif

   #ifdef __MMI_IRDA_SUPPORT__
   	extern void PopulateExtDevIrDARes(void);
   #endif
   
   #ifdef __MMI_BT_SUPPORT__
  	 extern void PopulateExtDevBTRes(void);
   #endif
   
 
   #ifdef __MMI_POC__
      extern void PoCResourceData(void);
   #endif
   
#endif /* __MMI_EXT_DEVICE__ */


 
void ToolPopulateInitial(void)
{
	int i = 0,j;
	unsigned char a[4];
	char path[]=CUST_IMG_PATH;
	char cmd[512];

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

	ClearHWIMageFile();
	#ifndef __POPULATE_ENGINE_
	SendMessage(hWnd,WM_CLEARFILES,(WPARAM)0,(LPARAM)0);
	#endif
	//CurrMaxStringId=0;
	CurrMaxImageId=0;
	CurrMaxImageNum=0;
	CurrMaxMenuItemIndex = MAX_MENU_ITEMS;

	 
	CurrMaxImageIdEXT=0; 
	CurrMaxImageNumEXT=0;
	 
	
	/* to read from NVRAM if possible -vanita */
	gCurrLangIndex = 0;

	memset(nCustMenus,0,sizeof(CUSTOM_MENU) * MAX_MENU_ITEMS);

	for (i=0;i<MAX_IMAGE_IDS_SIZE;i++){
		ImageIdMap[i].nImageNum=-1;
		 
		ImageIdMapEXT[i].nImageNum=-1; 
		 
	}
}

void ToolPopulateDone(void)
{
	/*  for generate string list file in first pass */
	fwprintf(StringListFile,L"##number\t%d\r\n",CurrMaxStringId);
	fclose(StringListFile);
	fprintf(EnumListFile,"};\n");
	fclose(EnumListFile);

	 
	fclose(OutputImgIDFile);
	fclose(OutputMenutreeIDFile);
	fclose(RepeatIDListFile);  
	 
}
 



#define TYPE__(A) A##_MENU
#define TYPE_(A)	TYPE__(A)
#define TYPE(A) TYPE_(A)
/*****************************************************************************
* FUNCTION
*	 PopulateResData()
* DESCRIPTION
*   Populate Resource 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void PopulateResData(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	int i = 0,j;
	unsigned char a[4];
	char path[]=CUST_IMG_PATH;
	char cmd[512];
    
   FILE	*OutputImgDataFile=NULL; 
    

	sprintf(cmd,"copy_images.bat ");
	j=16;
	for (i=0;i<strlen(path);i++)
	{
		if ( ! (path[i]=='\\' && path[i+1]=='\\') )
		{
			cmd[j]=path[i];
			j++;
		}
	}
	cmd[j]='\0';
	system(cmd);

  	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	StringListFile=fopen(CUS_STR_RES_FILENAME,"wb");
  	if (StringListFile == NULL){
		printf("StringListFile could not open!\n");
  	}
	EnumListFile = fopen ("enum_list.h","w");
  	if (EnumListFile == NULL){
		printf("EnumListFile could not open!\n");
  	}
	fclose(EnumListFile);
	EnumListFile = fopen ("enum_list.h","a");
  	if (EnumListFile == NULL){
		printf("EnumListFile could not open with again!\n");
  	}

	 
	OutputImgIDFile = fopen (OUTPUT_IMAGE_ID_FILENAME,"w");
  	if (OutputImgIDFile == NULL){
		printf("OutputImgIDFile could not open!\n");
		exit(0);
  	}
	fclose(OutputImgIDFile);
	OutputImgIDFile = fopen (OUTPUT_IMAGE_ID_FILENAME,"a");
  	if (OutputImgIDFile == NULL){
		printf("OutputImgIDFile could not open again!\n");
  	}
	fprintf(OutputImgIDFile,"[Image File List]\n");
	OutputMenutreeIDFile = fopen (OUTPUT_MENUTREE_ID_FILENAME,"w");
  	if (OutputMenutreeIDFile == NULL){
		printf("OutputMenutreeIDFile could not open!\n");
  	}
	fclose(OutputMenutreeIDFile);
	OutputMenutreeIDFile = fopen (OUTPUT_MENUTREE_ID_FILENAME,"a");
  	if (OutputMenutreeIDFile == NULL){
		printf("OutputMenutreeIDFile could not open again!\n");
  	}
	 
	
	 
	RepeatIDListFile = fopen ("repeat_id_list","w");
  	if (RepeatIDListFile == NULL){
		printf("RepeatIDListFile could not open!\n");
  	}
	fclose(RepeatIDListFile);
	RepeatIDListFile = fopen ("repeat_id_list","a");
  	if (RepeatIDListFile == NULL){
		printf("RepeatIDListFile could not open again!\n");
  	}
	fprintf(RepeatIDListFile, "StrId\tEnum_Value\t\tString or Image\n");
	 
	
	
	a[0]=0xFF;
	a[1]=0xFE;
	a[2]=0x4E;
	a[3]=0x00;
	
	fprintf (StringListFile,a);
	fprintf (StringListFile,"%c",a[3]);
	fwprintf (StringListFile,L"Number Of Languages\t1\r\n");
	fwprintf (StringListFile,L"Enum Value\tID\tDescription\tEnglish\r\n");
	fprintf(EnumListFile,"const int ENUM_VALUE_MAP[]={\n");
	
	ClearHWIMageFile();
	#ifndef __POPULATE_ENGINE_
	SendMessage(hWnd,WM_CLEARFILES,(WPARAM)0,(LPARAM)0);
	#endif
	//CurrMaxStringId=0;
	CurrMaxImageId=0;
	CurrMaxImageNum=0;
	CurrMaxMenuItemIndex = MAX_MENU_ITEMS;
	
	 
	CurrMaxImageIdEXT=0; 
	CurrMaxImageNumEXT=0;
	 

	/* to read from NVRAM if possible -vanita */
	gCurrLangIndex = 0;

	memset(nCustMenus,0,sizeof(CUSTOM_MENU) * MAX_MENU_ITEMS);

	/*****************************************************************************
	* Add applcation
	*****************************************************************************/
	ADD_APPLICATION(APP_MAINMENU,"MainMenu Base",MAIN_MENU_BASE,MAIN_MENU_BASE_MAX);
	ADD_APPLICATION(APP_IDLEAPP,"IdleApp",IDLE_APP_BASE,IDLE_APP_BASE_MAX);
	ADD_APPLICATION(APP_BOOTUP,"BootUp",SIM_DETECTION_BASE,SIM_DETECTION_BASE_MAX);
	ADD_APPLICATION(APP_SIMTOOLKIT,"Sim Toolkit",SAT_BASE,SAT_BASE_MAX);
	ADD_APPLICATION(APP_GLOBALDEFS,"Global Defines",GLOBAL_BASE,GLOBAL_BASE_MAX);
	ADD_APPLICATION(APP_CHARGER,"Charger",CHARGER_BASE,CHARGER_BASE_MAX);
	ADD_APPLICATION(APP_UIFRAMEWORK,"UI Framework",CATEGORY_SCREENS_BASE,CATEGORY_SCREENS_BASE_MAX);


	ADD_APPLICATION(APP_PHONEBOOK,"PhoneBook",PHONE_BOOK_BASE,PHONE_BOOK_BASE_MAX);
	ADD_APPLICATION(APP_SHORTCUTS,"ShortCuts",SHORTCUTS_BASE,SHORTCUTS_BASE_MAX);
	ADD_APPLICATION(APP_FUNANDGAMES,"Fun Games",FUN_GAMES_BASE,FUN_GAMES_BASE_MAX);
	ADD_APPLICATION(APP_SERVICES,"Services",SERVICES_BASE,SERVICES_BASE_MAX);
	ADD_APPLICATION(APP_ORGANIZER,"Organizer",ORGANIZER_BASE,ORGANIZER_BASE_MAX);
	ADD_APPLICATION(APP_MESSAGES,"Messages",MESSAGES_BASE,MESSAGES_BASE_MAX);
	ADD_APPLICATION(APP_CHAT,"Chat",CHAT_BASE,CHAT_BASE_MAX);
	ADD_APPLICATION(APP_SETTINGS,"Settings",SETTINGS_BASE,SETTINGS_BASE_MAX);
	ADD_APPLICATION(APP_CALLMANAGEMENT,"CallManagement",CALL_MANAGEMENT_BASE,CALL_MANAGEMENT_BASE_MAX);
	ADD_APPLICATION(APP_DEMOAPP,"DemoApp",DEMO_APP_BASE,DEMO_APP_BASE_MAX);
	ADD_APPLICATION(APP_CALLHISTORY,"Call History",CALL_HISTORY_BASE ,CALL_HISTORY_BASE_MAX );
	ADD_APPLICATION(APP_CALLSETUP,"Call Setup",CALL_NEW_BASE,CALL_NEW_BASE_MAX);
	ADD_APPLICATION(APP_PROFILES,"Profiles",PROFILES_BASE,PROFILES_BASE_MAX);
	ADD_APPLICATION(APP_NETWORKSETUP,"Network Setup",NETWORK_SETUP_BASE,NETWORK_SETUP_BASE_MAX);
	ADD_APPLICATION(APP_DATAACCOUNT,"Data Account",DATA_ACCOUNT_BASE,DATA_ACCOUNT_BASE_MAX);
	ADD_APPLICATION(APP_PHONESETUP,"Phone Setup",PHONE_SETUP_BASE,PHONE_SETUP_BASE + 999);
	ADD_APPLICATION(APP_TODOLIST,"To do List",TO_DO_LIST_BASE,TO_DO_LIST_BASE_MAX );
	ADD_APPLICATION(APP_INPUTMETHOD,"INPUTMETHOD",INPUT_METHOD_BASE,INPUT_METHOD_BASE_MAX);
	ADD_APPLICATION(APP_VOICEMEMO,"Voice Memo",VOICE_MEMO_BASE,VOICE_MEMO_BASE_MAX);
	ADD_APPLICATION(APP_ENGINEERMODE,"Engineer Mode",ENGINEER_MODE_BASE,ENGINEER_MODE_BASE_MAX);
	ADD_APPLICATION(APP_CALENDAR,"Calendar",CAL_BASE,CAL_BASE_MAX);
	ADD_APPLICATION(APP_FACTORYMODE,"Factory Mode",FACTORY_MODE_BASE,FACTORY_MODE_BASE_MAX);
	ADD_APPLICATION(APP_GAMES,"Games",GAME_BASE,GAME_BASE_MAX);
	ADD_APPLICATION(APP_WAP,"Wap",WAP_BASE,WAP_BASE_MAX);
	ADD_APPLICATION(APP_PICTURE_EDITOR,"Image Editor",PICTURE_EDITOR_BASE,PICTURE_EDITOR_BASE_MAX);
	ADD_APPLICATION(APP_ALARM,"Alarm",ALARM_BASE,ALARM_BASE_MAX);
	ADD_APPLICATION(APP_CALCULATOR,"Calculator",CALCULATOR_BASE,CALCULATOR_BASE_MAX);
	ADD_APPLICATION(APP_UNITCONVETER,"Unit Converter",UNIT_CONVERTER_BASE,UNIT_CONVERTER_BASE_MAX);
	ADD_APPLICATION(APP_CURRENCYCONVERTER,"Currency Converter",CURRENCY_CONVERTER_BASE,CURRENCY_CONVERTER_BASE_MAX);
	ADD_APPLICATION(APP_WORLDCLOCK,"World Clock",WORLD_CLOCK_BASE,WORLD_CLOCK_BASE_MAX);
	ADD_APPLICATION(APP_HEALTHBMI,"Health BMI",HEALTH_BMI_BASE,HEALTH_BMI_BASE_MAX);
	ADD_APPLICATION(APP_HEALTHCALORIE,"Health Calorie",HEALTH_CALORIE_BASE,HEALTH_CALORIE_BASE_MAX);
	ADD_APPLICATION(APP_HEALTHMENSTRUAL,"Health Menstrual",HEALTH_MENSTRUAL_BASE,HEALTH_MENSTRUAL_BASE_MAX);
	ADD_APPLICATION(APP_SECURITY_SETUP,"Security Setup",SECURITY_SETUP_BASE,SECURITY_SETUP_BASE_MAX);
	ADD_APPLICATION(APP_FILEMANAGER,"File Manager",FILE_MANAGER_BASE,FILE_MANAGER_BASE_MAX);
	ADD_APPLICATION(APP_MULTIMEDIA,"Multimedia",MMEDIA_BASE,MMEDIA_BASE_MAX);
	
	#if defined(__DOWNLOAD__)
		ADD_APPLICATION(APP_DOWNLOAD,"Download", DOWNLOAD_BASE, DOWNLOAD_BASE_MAX );
	#endif
	
	#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)
		ADD_APPLICATION(APP_RINGTONECOMPOSER,"RingToneComposer",COMPOSE_RING_TONE_ENGINE_BASE,COMPOSE_RING_TONE_ENGINE_BASE_MAX);
	#endif

	#ifdef __MMI_STOPWATCH__
		ADD_APPLICATION(APP_STOPWATCH,"Stopwatch",STOPWATCH_BASE,STOPWATCH_BASE_MAX);
	#endif

	#if defined(__MMI_AUDIO_PLAYER__)
		ADD_APPLICATION(APP_AUDIOPLAYER,"Audio Player",AUDIO_PLAYER_BASE,AUDIO_PLAYER_BASE_MAX);
	#endif
	
	#if defined(__MMI_SOUND_RECORDER__)
		ADD_APPLICATION(APP_SOUNDRECORDER,"Sound Recorder",SOUND_RECORDER_BASE,SOUND_RECORDER_BASE_MAX);
	#endif

	#ifdef __MMI_CAMERA__
		ADD_APPLICATION(APP_CAMERA,"Camera",CAMERA_BASE,CAMERA_BASE_MAX);
		ADD_APPLICATION(APP_IMAGEVIEWER,"Image Viewer",IMAGE_VIEWER_BASE,IMAGE_VIEWER_BASE_MAX);
	#endif

	#ifdef __MMI_VIDEO__
		ADD_APPLICATION(APP_VDOPLY,"Video Player",VDOPLY_BASE,VDOPLY_BASE_MAX);
		ADD_APPLICATION(APP_VDOREC,"Video Recorder",VDOREC_BASE,VDOREC_BASE_MAX);
	#endif

	#if defined(__MMI_EXT_DEVICE__)
		ADD_APPLICATION(APP_EXTDEVICE,"External Device",EXT_DEVICE_BASE,EXT_DEVICE_BASE_MAX);
	#endif

	#if defined(__MMI_VERSION_2__)
		ADD_APPLICATION(APP_EXTRA,"Extra",EXTRA_APP_BASE,EXTRA_APP_BASE_MAX);
	#endif

	#ifdef __J2ME__
		ADD_APPLICATION(APP_JAVA,"JAVA",JAVA_BASE,JAVA_BASE_MAX);
	#endif

	#ifdef __MMI_EMAIL__
		ADD_APPLICATION(APP_EMAIL,"Email",EMAIL_BASE,EMAIL_BASE_MAX);
	#endif

	#ifdef __MMI_IMPS__
		ADD_APPLICATION(APP_IMPS,"IMPS",IMPS_BASE,IMPS_BASE_MAX);
	#endif
	
	#ifdef __MMI_FM_RADIO__
		ADD_APPLICATION(APP_FMRDO,"FM Radio",FM_RADIO_BASE,FM_RADIO_BASE_MAX);
	#endif

	#ifdef __MMI_FM_RADIO_SCHEDULE_REC__
		ADD_APPLICATION(APP_FMSCHEDULEREC,"FM Radio Schedule Record",FM_SCHEDULE_REC_BASE,FM_SCHEDULE_REC_BASE + FM_SCHEDULE_REC_BASE_MAX);
	#endif

	#ifdef __MMI_MESSAGES_CLUB__
		ADD_APPLICATION(APP_MESSAGE_CLUB,"Message Club",MESSAGE_CLUB_BASE,MESSAGE_CLUB_BASE_MAX);
	#endif

	for (i=0;i<MAX_IMAGE_IDS_SIZE;i++){
		ImageIdMap[i].nImageNum=-1;
		 
		ImageIdMapEXT[i].nImageNum=-1; 
		 
	}


	/* add an empty image icon */
	ADD_APPLICATION_IMAGE(40000, "..\\\\..\\\\Customer\\\\Images\\\\EmptyImage.bmp");


	/*****************************************************************************
	* Populate Resoruce
	*****************************************************************************/	
	 
	PRINT_INFORMATION(("Populating Images for multiple bins\n")); 
	PopulateMultiBinImg();
	 
	
	PRINT_INFORMATION(("Populating Golbal Resources\n"));
	PopulateGlobalData();
	
	PRINT_INFORMATION(("Populating Main Menu Resources\n"));
	PopulateMainMenuRes();
	
	PRINT_INFORMATION(("Populating Idle Menu Resources\n"));
	PopulateIdleMenuRes();
	
	PRINT_INFORMATION(("Populating Phone Book Resources\n"));
	PopulatePbResData();
	
	PRINT_INFORMATION(("Populating Main Demo Resources\n"));
	PopulateMainDemoRes();
	
	PRINT_INFORMATION(("Populating Setting Resources\n"));
	populateSettingMenu();
	
	PRINT_INFORMATION(("Populating Fun & Games Resources\n"));
	populateFunAndGamesMenu();
	
	PRINT_INFORMATION(("Populating SIM Detect Resources\n"));
	PopulateSimDetectionMenuRes();

	PRINT_INFORMATION(("Populating Charger Power On Resources\n"));
	PopulateChgrPwrOnScrRes();

	PRINT_INFORMATION(("Populating Common Screen Resources\n"));
	PopulateCommonScreensResData();
	
	PRINT_INFORMATION(("Populating EMS object Resources\n"));
	PopulateEMSObjectResources();

	PRINT_INFORMATION(("Populating CM Resources\n"));
	PopulateCmResData();

	PRINT_INFORMATION(("Populating Quick Access Resources\n"));
	PopulateQuickAccessData();

	PRINT_INFORMATION(("Populating Input Method Resources\n"));
	PopulateInputMethodRes();

	PRINT_INFORMATION(("Populating Profiles Resources\n"));
	PopulateprofileMenu();
	
	PRINT_INFORMATION(("Populating SSC Resources\n"));
	PopulateSSCRes();
	
	PRINT_INFORMATION(("Populating Call History Resources\n"));
	PopulateCallHistoryRes();

	PRINT_INFORMATION(("Populating Call History Resources\n"));
	PopulateDataAccountMenuList();

	PRINT_INFORMATION(("Populating Organizer Resources\n"));
	PopulateOrganizerRes();

	PRINT_INFORMATION(("Populating Alarm Resources\n"));
	PopulateAlarmRes();

	PRINT_INFORMATION(("Populating SPOF Resources\n"));
	PopulateSpofRes();

	PRINT_INFORMATION(("Populating Calendar Resources\n"));
	PopulateResourcesCal();

	PRINT_INFORMATION(("Populating ToDo Resources\n"));
	PopulateToDoList();

	PRINT_INFORMATION(("Populating vObjects Resource\n"));
	PopulatevObjectRes();

	PRINT_INFORMATION(("Populating Help Resources\n"));
	PopulateHelpResources();
	
	PRINT_INFORMATION(("Populating Download Resources\n"));
	PopulateDownloadRes();
	
	PRINT_INFORMATION(("Populating Screen Saver Resources\n"));
	PopulateScreenSaverRes();

	PRINT_INFORMATION(("Populating MMIAPI Resource\n"));
	PopulateMmiapiRes(); 

	#ifdef __MOD_SMSAL__
		PRINT_INFORMATION(("Populating Messages Resources\n"));
		PopulateMessagesResData();
	
		#ifdef __MMI_MESSAGES_CHAT__
		PopulateChatAppRes();
		#endif
	#endif /* __MOD_SMSAL__ */

	#ifdef __MMI_EMAIL__
		PRINT_INFORMATION(("Populating Email Resources\n"));
		EmailResourceData ();
	#endif 

	#ifdef __MMI_IMPS__
		PRINT_INFORMATION(("Populating IMPS Resources\n"));
		PopulateIMPSData();
	#endif
	
	#if 1bm,
	d
		PRINT_INFORMATION(("Populating SAT Resources\n"));
		PopulateSATRes();
	#endif

	#ifdef SHORTCUTS_APP
		PRINT_INFORMATION(("Populating Shortcuts Resources\n"));
		PopulateShortcutsRes();
	#endif

	#ifdef __MMI_ENGINEER_MODE__ 
		PRINT_INFORMATION(("Populating EM Resources\n"));
		populateEngineerModeMenu();
	#endif
	
	#ifdef __MMI_FACTORY_MODE__
		PRINT_INFORMATION(("Populating FM Resources\n"));
		populateFactoryModeMenu();
	#endif

	#ifdef __MMI_AUDIO_PLAYER__
		PRINT_INFORMATION(("Populating Audio Player Resources\n"));
		populateAudioPlayerMenu();
	#else
		PRINT_INFORMATION(("Populating Audio Simple Player Resources\n"));
		populateAudioPlayerMenu();
	#endif

#ifdef __MMI_AB_REPEATER__
		PRINT_INFORMATION(("Populating AB Repeater Resources\n"));
		populateABRepeaterMenu();
#endif

	#ifdef __MMI_SOUND_RECORDER__
		PRINT_INFORMATION(("Populating Sound Recorder Resources\n"));
		populateSoundRecorderMenu();
	#endif

	#ifdef __MMI_FILE_MANAGER__
	   PRINT_INFORMATION(("Populating File Manager Resources\n"));
	   populateFileManagerMenu();
	#endif

	#ifdef __MMI_VIDEO__
	   PRINT_INFORMATION(("Populating Video Player Resources\n"));
		PopulateVdoPlyRes();
		PRINT_INFORMATION(("Populating Video Recoder Resources\n"));
		PopulateVdoRecRes();
	#endif

	#ifdef __MMI_NITZ__
		PRINT_INFORMATION(("Populating NITZ Resources\n"));
		PopulateResourceNITZ();
	#endif
	
	#ifdef __MMI_PICTURE_EDITOR_SUPPORT__
		PRINT_INFORMATION(("End Populating Image Editor\n"));
		PopulatePictureEditor();
	#endif

	#ifdef __MMI_STOPWATCH__
		PRINT_INFORMATION(("Populating StopWatch Resources\n"));
		PopulateStopwatchRes();
	#endif

	#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)
		PRINT_INFORMATION(("Populating Compose RingTone\n"));
		PopulateComposeRingTone();
	#endif

	#ifdef __MMI_CAMERA__
		PRINT_INFORMATION(("Populating Camera Resource\n"));
		PopulateCameraRes();
		PopulateImageViewerRes();
	#endif

	#ifdef __MMI_FM_RADIO__
		PRINT_INFORMATION(("Populating FM Radio Resource\n"));
		PopulateFMRadioRes();
	#endif

	#ifdef __MMI_FM_RADIO_SCHEDULE_REC__
		PRINT_INFORMATION(("Populating Schedule FM Radio Record Resource\n"));
		PopulateFMScheduleRecRes();	
	#endif

	#if defined(__MMI_PHOTOEDITOR__) 
		PRINT_INFORMATION(("Populating PhotoEditor Resource\n"));
		PopulatePhotoEditorRes();
	#endif

	#if defined (__MMI_EXT_DEVICE__)
	   #ifdef __MMI_USB_SUPPORT__
		   PRINT_INFORMATION(("Populating USB Resource\n"));
		   PopulateExtDevUSBRes();
	   #endif

	   #ifdef __MMI_IRDA_SUPPORT__
		   PRINT_INFORMATION(("Populating IRDA Resource\n"));
	  		PopulateExtDevIrDARes();
	   #endif

	   #ifdef __MMI_BT_SUPPORT__
		   PRINT_INFORMATION(("Populating Bluetooth Resource\n"));
		   PopulateExtDevBTRes();
	   #endif
	#endif

	#if defined(__MMI_VRSD__)
		PRINT_INFORMATION(("Populating Voice Recognition SD Resource\n"));
		PopulateVRSDRes();
	#endif

	#if defined(__MMI_VRSI__)
		PRINT_INFORMATION(("Populating Voice Recognition SI Resource\n"));
		PopulateVRSIRes();	
	#endif	

	#if defined(WAP_SUPPORT) || defined(__MMI_MMS__) 
		PRINT_INFORMATION(("Populating WAP Resource\n"));
		PopulateWapRes();
	#endif

 
      #ifdef __MMI_POC__
      PRINT_INFORMATION(("Populating PoC Resources\n"));
      PoCResourceData();
      #endif


	#ifndef __POPULATE_ENGINE_
		SendMessage(hWnd,WM_GENSTRFILE,(WPARAM)0,(LPARAM)0);
		PRINT_INFORMATION(("after 1 sendmsg"));

		SendMessage(hWnd,WM_GENIMGFILE,(WPARAM)0,(LPARAM)0);
		PRINT_INFORMATION(("after 2 send msg "));

		SendMessage(hWnd,WM_GENMENUFILE,(WPARAM)0,(LPARAM)0);
		PRINT_INFORMATION(("after 3 send msg "));
	#endif

	
	//WRITE_STRING_RES_FILE;	/* Write the string resource file only for first pass */
	//WRITE_IMAGE_RES_FILE;		/* Write the image resource file only for first pass */
	//WRITE_MENU_RES_FILE;

	/*  for generate string list file in first pass */
	fwprintf(StringListFile,L"##number\t%d\r\n",CurrMaxStringId);
	fclose(StringListFile);
	fprintf(EnumListFile,"};\n");
	fclose(EnumListFile);

	 
	fclose(OutputImgIDFile);
	fclose(OutputMenutreeIDFile);
	fclose(RepeatIDListFile);  
	 
	 
if  ( TYPE(__MMI_MAINMENU_TYPE__) != MATRIX_MENU)
{
	OutputImgDataFile = fopen ( "..\\..\\Customer\\CustResource\\CustImgDataHW.h","a");
	if ( OutputImgDataFile == NULL )
	   printf ("OutputImageDataFile Open fail !\n");
	else
	{
			fprintf (OutputImgDataFile,"const U8 __MAINLCD__MAINMENU__MATRIX__MM_BG_GIF[]={0x00,0x00};\n\n");
			fclose (OutputImgDataFile);
   }
}
	 
}



/*****************************************************************************
* FUNCTION
*	 PopulateGlobalData()
* DESCRIPTION
*   Populate Global Resource 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void PopulateGlobalData()
{
	ADD_APPLICATION_STRING2(STR_GLOBAL_OK, "Ok","Global String- OK");
	ADD_APPLICATION_STRING2(STR_GLOBAL_BACK, "Back","Global String- Back");
	ADD_APPLICATION_STRING2(STR_GLOBAL_OPTIONS, "Option","Global String- Options");
	ADD_APPLICATION_STRING2(STR_GLOBAL_YES, "Yes","Global String- Yes");
	ADD_APPLICATION_STRING2(STR_GLOBAL_NO, "No","Global String- No");
	ADD_APPLICATION_STRING2(STR_GLOBAL_ON, "On","Global String- On");
	ADD_APPLICATION_STRING2(STR_GLOBAL_OFF, "Off","Global String- Off");
	ADD_APPLICATION_STRING2(STR_GLOBAL_CLEAR, "Clear","Global String- Clear");
	ADD_APPLICATION_STRING2(STR_GLOBAL_DONE, "Done","Global String- Done");
	ADD_APPLICATION_STRING2(STR_GLOBAL_SAVED, "Saved","Global String- Saved");
	ADD_APPLICATION_STRING2(STR_GLOBAL_SAVE, "Save","Global String- Save");
	ADD_APPLICATION_STRING2(STR_GLOBAL_SAVING, "Saving","Global String- Saving");
	ADD_APPLICATION_STRING2(STR_GLOBAL_NOT_DONE, "Not Done","Global String- Not Done");
	ADD_APPLICATION_STRING2(STR_GLOBAL_UNFINISHED, "Unfinished","Global String- Unfinished");
	ADD_APPLICATION_STRING2(STR_GLOBAL_DELETE, "Delete","Global String- Delete");
	ADD_APPLICATION_STRING2(STR_GLOBAL_CANCEL, "Cancel","Global String- Cancel");
	ADD_APPLICATION_STRING2(STR_GLOBAL_EMPTY, "Empty","Global String- Empty");
	ADD_APPLICATION_STRING2(STR_GLOBAL_EDIT, "Edit","Global String- Edit");
	ADD_APPLICATION_STRING2(STR_GLOBAL_ACTIVATE, "Activate","Global String- Activate");
	ADD_APPLICATION_STRING2(STR_GLOBAL_ACTIVATED, "Activated", "Global String- Activated");
	ADD_APPLICATION_STRING2(STR_GLOBAL_DETACTIVATE, "Deactivate","Global String- Deactivate");
	ADD_APPLICATION_STRING2(STR_EARPHONE_POPUP,"Earphone plugged in!","String Associated with Ear Phone Plug In Popup.");
	ADD_APPLICATION_STRING2(STR_HEADSET_DEACTIVATE_MESSAGE,"Earphone plugged Out","Earphone plugged Out");
	ADD_APPLICATION_STRING2(STR_GLOBAL_INPUT_METHOD, "Input Method","Global String- Input Method");
	ADD_APPLICATION_STRING2(STR_GLOBAL_USE_TEMPLATE, "Use Template","Global String- Use Template");
	ADD_APPLICATION_STRING2(STR_GLOBAL_SEND, "Send","Global String- Send");
	ADD_APPLICATION_STRING2(STR_GLOBAL_REPLY, "Reply","Global String- Reply");
	ADD_APPLICATION_STRING2(STR_GLOBAL_EMPTY_LIST, "<Empty>","Global String- <Empty>");
	ADD_APPLICATION_STRING2(STR_GLOBAL_ABORT, "Abort","Global String- Abort");
	ADD_APPLICATION_STRING2(STR_GLOBAL_ADD, "Add","Global String- Add");
	ADD_APPLICATION_STRING2(STR_GLOBAL_DELETE_ALL, "Delete All","Global String- Delete All");
	ADD_APPLICATION_STRING2(STR_GLOBAL_DIAL, "Dial","Global String- Dial");
	ADD_APPLICATION_STRING2(STR_GLOBAL_ERROR, "Error","Global String- Error");
	ADD_APPLICATION_STRING2(STR_GLOBAL_NORMAL, "Normal","Global String- Normal");
	ADD_APPLICATION_STRING2(STR_GLOBAL_NONE, "None","Global String- None");
	ADD_APPLICATION_STRING2(STR_GLOBAL_SELECT, "Select","Global String- Select");
	ADD_APPLICATION_STRING2(STR_GLOBAL_VIEW, "View","Global String- View");
	ADD_APPLICATION_STRING2(STR_GLOBAL_UNSUPPORTED_FORMAT, "Unsupported Format","Global String- Unsupported Format");
	ADD_APPLICATION_STRING2(STR_GLOBAL_NOT_AVAILABLE, "Not Available","Global String- Not Available");
	ADD_APPLICATION_STRING2(STR_GLOBAL_INVALID, "Invalid","Global String- Invalid");
	ADD_APPLICATION_STRING2(STR_GLOBAL_DRM_PROHIBITED, "DRM Prohibited","Global String- DRM Prohibited");
	ADD_APPLICATION_STRING2(STR_GLOBAL_FILENAME, "Filename","Global String- Filename");
	ADD_APPLICATION_STRING2(STR_GLOBAL_OPEN, "Open","Global String- Open");
	ADD_APPLICATION_STRING2(STR_GLOBAL_CLOSE, "Close","Global String- Close");
	ADD_APPLICATION_STRING2(STR_GLOBAL_QUIT, "Quit","Global String- Quit");
	ADD_APPLICATION_STRING2(STR_GLOBAL_SEARCH, "Search","Global String- Search");
	ADD_APPLICATION_STRING2(STR_GLOBAL_SEARCHING, "Searching","Global String- Searching");
	ADD_APPLICATION_STRING2(STR_GLOBAL_ABORTING, "Aborting","Global String- Aborting");
#ifndef MMI_ON_HARDWARE_P
	ADD_APPLICATION_STRING2(STR_GLOBAL_NOT_SUPPORT_AT_PC_SIMULATOR, "Not Support at PC Simulator","Global String- Not Support at PC Simulator");
#endif	
	ADD_APPLICATION_STRING2(STR_GLOBAL_SOC_ERROR, "Soc Error","Global String- Soc Error");

	ADD_APPLICATION_IMAGE2(IMG_BOOTUP_SCANNING_LOGO,CUST_IMG_PATH"\\\\MainLCD\\\\Active\\\\poweronoff\\\\logo.gif","Bootup Scanning Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_OK, "..\\\\..\\\\Customer\\\\Images\\\\EmptyImage.bmp","Global Image-Ok");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_BACK, "..\\\\..\\\\Customer\\\\Images\\\\EmptyImage.bmp","Global Image-Back");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_YES, "..\\\\..\\\\Customer\\\\Images\\\\EmptyImage.bmp","Global Image-Yes");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_NO, "..\\\\..\\\\Customer\\\\Images\\\\EmptyImage.bmp","Global Image-No");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_OPTIONS, "..\\\\..\\\\Customer\\\\Images\\\\EmptyImage.bmp","Global Image-Options");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L1, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\Listmenuicon\\\\L_NB1.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L2, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB2.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L3, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB3.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L4, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB4.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L5, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB5.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L6, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB6.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L7, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB7.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L8, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB8.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L9, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB9.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L10, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB10.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L11, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB11.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L12, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB12.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L13, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB13.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L14, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB14.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L15, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB15.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L16, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB16.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L17, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB17.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L18, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB18.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L19, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB19.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L20, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB20.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L21, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB21.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L22, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB22.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L23, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB23.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L24, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB24.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L25, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB25.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L26, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB26.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L27, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB27.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L28, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB28.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L29, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB29.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L30, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB30.PBM","Menu Image");
	//Fix Bug#12068 for M301WapMMS by cong.li on 2009.05.14.
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L31, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB31.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L32, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB32.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L33, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB33.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L34, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB34.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L35, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB35.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L36, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB36.PBM","Menu Image");	
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L37, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB37.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L38, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB38.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_L39, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\ListMenuIcon\\\\L_NB39.PBM","Menu Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_CLEAR, "..\\\\..\\\\Customer\\\\Images\\\\EmptyImage.bmp","Global Image-Clear");
	ADD_APPLICATION_IMAGE2(IMG_STATUS, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\Organizer\\\\Alarm\\\\OA_STATU.PBM","Status Image");
	ADD_APPLICATION_IMAGE2(IMG_VICON, CUST_IMG_PATH"\\\\MainLCD\\\\UIElement\\\\checkbox\\\\CK_V.bmp","Status Image");
	ADD_APPLICATION_IMAGE2(IMG_TIME, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\Organizer\\\\Alarm\\\\OA_TIME.PBM","Time Image");
	ADD_APPLICATION_IMAGE2(IMG_REPEAT, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\Organizer\\\\Alarm\\\\OA_REPT.PBM","Repeat Image");
	ADD_APPLICATION_IMAGE2(IMG_CAL, CUST_IMG_PATH"\\\\MainLCD\\\\SubMenu\\\\Settings\\\\DateAndTime\\\\ST_CALEN.PBM","Calendar Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_QUESTION, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_QUE.gif", "Question Mark image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_SAVE, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_SAV.GIF", "saved image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_DELETED, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_DEL.GIF", "deleted image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_EMPTY, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_EMP.gif", "empty image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_UNFINISHED, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_UNF.gif", "unfinished image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_WARNING, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_WARN.bmp", "warning image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_PROGRESS,CUST_IMG_PATH"\\\\MainLCD\\\\UIElement\\\\Progress\\\\PROGRESS.GIF","Progress Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_DEFAULT, "..\\\\..\\\\Customer\\\\Images\\\\Default.bmp", "default image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_ACTIVATED, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_DONE.gif", "Activated image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_ERASED, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_ERASE.gif", "Erased image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_ERROR, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_ERR.gif","Error Image");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_INFO, CUST_IMG_PATH"\\\\MainLCD\\\\Warningpage\\\\PopUpPage\\\\PP_INF.gif","Info Image");
	ADD_APPLICATION_IMAGE2(IMG_EARPHONE_POPUP_MAINLCD,CUST_IMG_PATH"\\\\MAINLCD\\\\WarningPage\\\\notification\\\\N_HDSET.BMP","Main LCD Image Associated with Ear Phone Plug In Popup.");
	ADD_APPLICATION_IMAGE2(IMG_EARPHONE_POPUP_SUBLCD,CUST_IMG_PATH"\\\\SUBLCD\\\\Active\\\\SB_EAR.BMP","Sub LCD Image Associated with Ear Phone Plug In Popup.");
	ADD_APPLICATION_IMAGE2(IMG_GLOBAL_SUB_MENU_BG,CUST_IMG_PATH"\\\\MAINLCD\\\\SUBMENU\\\\SB_BG.GIF","SubMenu BG Image.");
	#ifdef __MMI_SLIM_PROGRESS_ANIMATION__
		ADD_APPLICATION_IMAGE2(IMG_GLOBAL_LOADING,CUST_IMG_PATH"\\\\MainLCD\\\\UIElement\\\\Progress\\\\PROGRESS.GIF","Loading Progress Image");
	#else
		ADD_APPLICATION_IMAGE2(IMG_GLOBAL_LOADING,CUST_IMG_PATH"\\\\MainLCD\\\\UIElement\\\\Progress\\\\LOADING.GIF","Loading Progress Image");
	#endif
	ADD_APPLICATION_STRING2(STR_GLOBAL_TFLASH_PLUG_IN, "T-F card plug in.", "Global String- Please wait...");
	ADD_APPLICATION_STRING2(STR_GLOBAL_TFLASH_PLUG_OUT, "T-F card plug OUT.", "Global String- Please wait...");

}

#else /* !DEVELOPER_BUILD_FIRST_PASS */
/***************************************************************************** 
*
* Not FIRST_PASS, for MMI to get global resource info
*
*****************************************************************************/
U16	CurrMaxStringNum;
U32 	CurrTotalStrLen;
U16 	CurrMaxStringId;
U16 	CurrMaxStringIdSearch;
U16 	gCurrLangIndex;

#ifdef MMI_ON_HARDWARE_P
	const CUSTOM_STRING				*gpStringRes;
	const CUSTOM_STRING_MAP			*gpStringMapRes;
	const CUSTOM_STRMAP_SEARCH		*gpStrMapSearch;
	extern StringResList				*gStringList;

#else
	CUSTOM_STRING						*gpStringRes;
	CUSTOM_STRING_MAP					*gpStringMapRes;
	CUSTOM_STRMAP_SEARCH				*gpStrMapSearch;
	extern StringResList				*gStringList;
#endif /* MMI_ON_HARDWARE_P */

#if !defined(MMI_ON_HARDWARE_P) || defined(WIN32)
extern void InitializeResourceVariables(void);
#endif


/*****************************************************************************
* FUNCTION
*	 PopulateResGetBaseIDTable()
* DESCRIPTION
*   Dummy function to make ARM linker happy such that developers can access
*	 g_mmi_resource_base_table in Trace32/VC++ even though the variable is not used
*	 in other places.
* PARAMETERS
*	 nil
* RETURNS
*   void * 	- the address of base id table
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void *PopulateResGetBaseIDTable(void)
{
#if !defined(PRODUCTION_RELEASE)
	return (void *) g_mmi_resource_base_table;
#else
	return NULL;
#endif /* ~ PRODUCTION_RELEASE */
}


/*****************************************************************************
* FUNCTION
*	 PopulateResData()
* DESCRIPTION
*   Populate Resource, assign resource info to global variable
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void PopulateResData(void)
{
	/* to read from NVRAM if possible -vanita */
#if !defined(MMI_ON_HARDWARE_P) || defined(WIN32)
	InitializeResourceVariables();
#endif
	gCurrLangIndex = 0;
	gpStringRes = gStringList[gCurrLangIndex].pStringRes;
	gpStringMapRes = gStringList[gCurrLangIndex].pStringMap;
	gpStrMapSearch = gStringList[gCurrLangIndex].pStrMapSearch;
	CurrMaxStringNum = gStringList[gCurrLangIndex].nTotalStrRes;
	CurrMaxStringId = gStringList[gCurrLangIndex].nTotalStrMap;
	CurrMaxStringIdSearch=gStringList[gCurrLangIndex].nTotalStrMapSearch;
	return;
}

#endif /* DEVELOPER_BUILD_FIRST_PASS */


