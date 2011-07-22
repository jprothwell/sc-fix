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

/**************************************************************

	FILENAME	: PopulateRes.h

  	PURPOSE		: Common Resource Populate Header

 

 

	DATE		: August-11-2003

**************************************************************/

#include "globaldefs.h"
#include "debuginitdef.h"

#include "custdatares.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "mainmenudef.h"
#include "globalmenuitems.h"

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "custresdef.h"

//PhoneBook
void PopulatePbResData(void);
void PopulateAdvancedPbResData(void);
void PopulateInlineEditRes(void);
void PopulateCopyAllResData(void);
void PopulateExtraNumberRes(void);
void PopulatePbookSettingsData(void);
void PopulatePhbExtraMainMenuRes(void);


//Profiles
void PopulateprofileMenu(void);

//SAT
void PopulateSATRes(void);

//Alarm
void PopulateAlarmRes(void);

//Help
void PopulateHelpResources(void);

//Organizer
void PopulateCalculator(void);
void PopulateCalorieData() ;
void PopulateHealthMensisRes(void);
void PopulateOrganizerRes(void);
void PopulateExtraRes(void);
void PopulateBMIData();
void PopulateCurrencyConvertor(void);
void PopulateUnitConvertor();
void mod_mmiPopulateWClockData(void);
void PopulateDictionaryData(void);
//DataAccount
void PopulateDataAccountMenuList(void);

//SSC
void PopulateSSCRes();

//GUI
void PopulateCommonCategoryResources(void);
void PopulateStatusIconResources(void);
void PopulateCircular3DMenuResources(void);
void PopulateThemeResources(void);

//ShortCuts
void PopulateShortcutsRes(void);

#if defined(__MMI_NITZ__)
//NITZHandler
void PopulateResourceNITZ(void) ;
#endif

//FunAndGames
void PopulateComposeRingTone(void);
void PopulateDownloadRes(void);
void PopulateRingComposer(void);
void populateFunAndGamesMenu(void);
void PopulateThemes(void);

 
//void PopulateStopwatchRes(void);


//Games
void PopulateGamesRes(void);
void PopulateF1RaceRes(void);
void PopulateTOHRes(void);
void PopulateRichesRes(void);
void PopulateColorBallRes(void);
void PopulateSnakeRes(void);
void PopulateSmashRes(void);
void PopulateJavaGameRes(void);

#ifdef __MMI_3D_GAME__
extern void Populate3DGameRes(void);
#endif 

#ifdef __MMI_MOTION_APP__
extern void PopulateMotionAppRes(void);
#endif 
//ToDoList
void PopulateToDoList(void);

//Common Screens
void PopulateCommonScreensResData(void);

//IdleDemo
void PopulateIdleMenuRes(void);
void PopulateMainDemoRes(void);

//MiscFramework
void PopulatePersonalizationRes(void);
void PopulateChgrPwrOnScrRes(void);
void PopulateQuickAccessData(void);
void PopulateSimDetectionMenuRes(void);
void populateOptionMenu(void);
void PopulateAphorismList(void);
void PopulatePLMNList(void);

//CSDApp
void populateEngineerModeMenu(void);
void populateFactoryModeMenu(void);
void PopulateInputMethodRes(void);
//CSD Brian added for voice memo, 2003/10/18
#ifdef __MMI_VOICE_MEMO__
void populateVoiceMemoMenu(void);
#endif

//Settings
void PopulateCallBarring();
void PopulateCallForward(void);
void PopulateCallWait();
void PopulateCallerId(void);
void PopulateSettingMenu(void);
void PopulateLineSwitch(void);
void populateManageCalls();
void PopulateNetworkSetupResData(void);
void PopulatePhoneSetupResData(void) ;
void PopulatePrefNwk(void);
void PopulateSpofRes(void);
void PopulateScreenSaverRes(void);
void PopulateWallpaper();
void PopulategrouplistMenu(void);
void PopulateIPNumber(void);

//CallManagement
void PopulateCmRedialScrData(void);
void PopulateCmResData (void);
void PopulateCmOutResData(void);

//Calls
void PopulateBarredDialList(void);
void PopulateFixedDialList(void) ;
void AddCallLogApplicationRes( void );
void PopulateCallTimes();
void PopulateCallCost(void );
void PopulateCallHistoryRes(void);
void PopulateResetCallRes();
void AddSMSCounterApplicationRes( void );

#ifdef __MMI_GPRS_FEATURES__
void AddGPRSCounterApplicationRes(void);
#endif


//Messages
void PopulateMessagesResData (void);
static void PopulateEMSResourceData (void);
void AddCBLanguagesString (void);

//EMS
void PopulateEMSObjectResources(void);

/* Unified Message */
extern void PopulateUnifiedMessageResData(void);

/* Unified Composer */
extern void PopulateUnifiedComposerResData(void);
//MainMenu
void PopulateMainMenuRes(void);

 
#ifdef __MMI_STOPWATCH__
void PopulateStopwatchRes(void);
#endif

#ifdef __MMI_EMAIL__
void EmailResourceData (void);
#endif // def __MMI_EMAIL__

#if defined(__MMI_SMART_MESSAGE_MT__) || ( defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))|| defined(__MMI_MYPICTURE__)
void PopulateNSMRes(void);
void PopulateNsmStrRes(void);
void PopulateNsmImgRes( void );
void PopulateNsmMenuItemRes(void);
#endif

#ifdef __MMI_IMPS__
void PopulateIMPSData(void);
#endif

 
#ifdef __MMI_POC__
extern void PoCResourceData(void);
#endif 

#ifdef __MMI_VOIP__
void VoIPResourceData(void);
#endif

#if (defined (JATAAYU_SUPPORT) && defined(MMS_SUPPORT))
extern void PopulateJaatayuMMSRes(void);
#endif 

#endif /* DEVELOPER_BUILD_FIRST_PASS */ 

