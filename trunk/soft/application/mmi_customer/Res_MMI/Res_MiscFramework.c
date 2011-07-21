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

/**************************************************************

	FILENAME	: Res_MiscFramework.c

  	PURPOSE		: Populate Resource for MiscFramework

	REMARKS		: nil

	AUTHOR		: Yogesh

	DATE		: August-15-2003

**************************************************************/
#include "stdc.h"
#include "mmi_features.h"

#include "customcfg.h"
#include "plmnenum.h"


#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#ifdef __MMI_SML_MENU__
#include "frameworkstruct.h"
#endif

#include "personalizationdef.h"
#include "bootup.h"
#include "quickaccessmenuitem_def.h"
#include "simdetectiondef.h"

//micha0406
#include "custresdef.h"

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"


/**************************************************************

	FUNCTION NAME		: PopulatePersonalizationRes()

  	PURPOSE				: Function to populate PERSONALIZATION Resources

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/

void PopulatePersonalizationRes(void)
{
	ADD_APPLICATION_STRING2(STR_ENTER_NP_MSG,"Enter NP Code:","Enter NP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_NSP_MSG,"Enter NSP Code:","Enter NSP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_CP_MSG,"Enter CP Code:","Enter CP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_SIMP_MSG,"Enter SIMP Code:","Enter SIMP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_SP_MSG,"Enter SP Code:","Enter SP Code Screen MSG.");

	ADD_APPLICATION_STRING2(STR_WRONG_SP_MSG,"Wrong SP Code!","Wrong SP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_WRONG_CP_MSG,"Wrong CP Code!","Wrong CP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_WRONG_NSP_MSG,"Wrong NSP Code!","Wrong NSP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_WRONG_NP_MSG,"Wrong NP Code!","Wrong NP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_WRONG_SIMP_MSG,"Wrong SIMP Code!","Wrong SIMP Code Screen MSG.");

/* SML menu implementation */
#ifdef __MMI_SML_MENU__
	ADD_APPLICATION_STRING2(STR_ENTER_NSSP_MSG,"Enter NS-SP Code:","Enter NSSP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_SIMC_MSG,"Enter SIM-C Code:","Enter SIMC Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_WRONG_NSSP_MSG,"Wrong NS-SP Code!","Wrong NSSP Code Screen MSG.");
	ADD_APPLICATION_STRING2(STR_WRONG_SIMC_MSG,"Wrong SIM-C Code!","Wrong SIMC Code Screen MSG.");

	ADD_APPLICATION_STRING2(STR_ID_CATEGORY_NP_LOCK,"Network Personalization","NP");
	ADD_APPLICATION_STRING2(STR_ID_CATEGORY_NSP_LOCK,"Network Subset Personalization","NSP");
	ADD_APPLICATION_STRING2(STR_ID_CATEGORY_CP_LOCK,"Corporate Personalization","CP");
	ADD_APPLICATION_STRING2(STR_ID_CATEGORY_SP_LOCK,"Service Provider Personalization","SP");
	ADD_APPLICATION_STRING2(STR_ID_CATEGORY_SIMP_LOCK,"SIM Personalization","SIMP");
	ADD_APPLICATION_STRING2(STR_ID_CATEGORY_NSP_SP_LOCK,"NS-SP Personalization","NS-SP");
	ADD_APPLICATION_STRING2(STR_ID_CATEGORY_SIMP_CP_LOCK,"SIM-C Personalization","SIM-C");

	ADD_APPLICATION_STRING2(STR_ID_SML_LOCK_MENU,"Lock","Lock");
	ADD_APPLICATION_STRING2(STR_ID_SML_UNLOCK_MENU,"Unlock","Unlock");
	ADD_APPLICATION_STRING2(STR_ID_SML_ADD_MENU,"Add Data","Add");
	ADD_APPLICATION_STRING2(STR_ID_SML_REMOVE_MENU,"Remove Data","Remove");
	ADD_APPLICATION_STRING2(STR_ID_SML_DEACTIVATE_MENU,"Permanent Unlock","Deactivate");

	ADD_APPLICATION_STRING2(STR_ID_SML_REBOOT_MSG,"Need to reboot now!","Reboot");
	ADD_APPLICATION_STRING2(STR_ID_SML_QUREY_MSG,"Automatically get data from SIM?","Query");
	ADD_APPLICATION_STRING2(STR_ID_SML_BLOCK_MSG,"Blocked!","Blocked");
	ADD_APPLICATION_STRING2(STR_ID_SML_NO_EF_GID,"No EF GID in this SIM!","No GID");
	ADD_APPLICATION_STRING2(STR_ID_SML_INVALID_DATA,"Invalid Data!","Invalid Data");
	ADD_APPLICATION_STRING2(STR_ID_SML_AUTOKEY_CONFIRM,"Automatically generate key?","Auto Key");
	ADD_APPLICATION_STRING2(STR_ID_LAST_RETRY_MSG,"WARNING!\n Last try!","Last try");
	ADD_APPLICATION_STRING2(STR_ID_GENERAL_RETRY_MSG,"Invalid key entered!","Invalid key");
	ADD_APPLICATION_STRING2(STR_ID_UNLOCK_FIRST_MSG,"Unlock the category first!","Unlock first");
	ADD_APPLICATION_STRING2(STR_ID_EMPTY_CATEGORY_MSG,"No category available!","No category available");
	ADD_APPLICATION_STRING2(STR_ID_NO_RECORD_MSG,"No space to add!","No space to add");
	ADD_APPLICATION_STRING2(STR_ID_NO_DATA_MSG,"No data!","No data");

	ADD_APPLICATION_STRING2(STR_ID_SML_CAT_LOCKED,"This category is locked!","Locked");
	ADD_APPLICATION_STRING2(STR_ID_SML_CAT_UNLOCKED,"This category is unlocked!","Unlocked");
	ADD_APPLICATION_STRING2(STR_ID_SML_CAT_AUTOLOCKED,"This category is in auto lock state!","Auto lock");

	ADD_APPLICATION_STRING2(STR_ID_SML_AUTOLOCKED,"This SIM is auto locked!","Auto lock");
	ADD_APPLICATION_STRING2(STR_ID_SML_FAIL_TO_AUTOLOCK,"Auto lock failed!","Auto lock");

	ADD_APPLICATION_STRING2(STR_ID_SML_NP_DATA,"MCC+MNC","IMSI");
	ADD_APPLICATION_STRING2(STR_ID_SML_NSP_DATA,"MCC+MNC+HLR","IMSI");
	ADD_APPLICATION_STRING2(STR_ID_SML_SP_DATA,"GID1","GID1");
	ADD_APPLICATION_STRING2(STR_ID_SML_CP_DATA,"GID2","GID2");
	ADD_APPLICATION_STRING2(STR_ID_SML_SIMP_DATA,"IMSI","IMSI");
#endif
}

/**************************************************************

	FUNCTION NAME		: PopulateChgrPwrOnScrRes(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/
	
void PopulateChgrPwrOnScrRes()
{
#ifdef __PRODUCT_QPAD__
	ADD_APPLICATION_IMAGE2(IMG_CHARGER_POWER_ON,CUST_IMG_PATH"/MainLCD/UIELEMENT/charger/qpad/charging.gif","	Image Associated with Power On by Charger Screen");
	ADD_APPLICATION_IMAGE2(IMG_BATTERY_FULL_CHARGED,CUST_IMG_PATH"/MainLCD/UIELEMENT/charger/qpad/charging_full.bmp","	Image Associated with Charger Fully charged");
#else
	ADD_APPLICATION_IMAGE2(IMG_CHARGER_POWER_ON,CUST_IMG_PATH"/MainLCD/UIELEMENT/charger/CHARGING.gif","	Image Associated with Power On by Charger Screen");
	ADD_APPLICATION_IMAGE2(IMG_BATTERY_FULL_CHARGED,CUST_IMG_PATH"/MainLCD/UIELEMENT/charger/CHARGF.bmp","	Image Associated with Charger Fully charged");
#endif

	ADD_APPLICATION_STRING2(STR_CHARGER_POWER_ON,"Charging","Message for Power On by Charger Screen");
	ADD_APPLICATION_STRING2(STR_BATTERY_FULL_CHARGED,"Charge Complete","String Associated with Battery Fully Charged Screen.");
	ADD_APPLICATION_STRING2(STR_OVER_VOLTAGE_BATTERY,"Warning!Over Battery Voltage!!","String Associated with Over Voltage Screen");
	ADD_APPLICATION_STRING2(STR_OVER_TEMP_BATTERY,"Warning! Over Battery Temperature!!","String Associated with Over Temperature Screen");
	ADD_APPLICATION_STRING2(STR_LOW_BATTERY,"Battery Low","String Associated with Low battery Screen");
	ADD_APPLICATION_STRING2(STR_INVALID_BATTERY,"Warning! Invalid Battery!","String Associated with Invalid battery Screen");
	ADD_APPLICATION_STRING2(STR_OVER_CURRENT_BATTERY,"Warning! Over Charge Current!","String Associated with Over Charger current Screen"); 
	ADD_APPLICATION_STRING2(STR_CHARGER_CONNECTED,"Charger Connected!","String Displyed when Charger Connected.");
	ADD_APPLICATION_STRING2(STR_CHARGER_REMOVED,"Charger Removed!","String Displyed when Charger Removed.");
      
	ADD_APPLICATION_STRING2(STR_USB_CHARGER_CONNECTED,"USB Connected!","String Displyed when Charger Connected.");
	ADD_APPLICATION_STRING2(STR_USB_CHARGER_REMOVED,"USB Removed!","String Displyed when Charger Removed.");
      
	ADD_APPLICATION_STRING2(STR_CHARGING_TIMEOUT,"Warning! Charging Fail!","String Displyed when Charging Timeout.");
	ADD_APPLICATION_STRING2(STR_INVALID_CHARGER,"Warning! Invaild Charger!","String Displyed when Invalid Charger.");
	ADD_APPLICATION_STRING2(STR_LOW_TEMP_BATTERY,"Warning! Low Battery Temperature!","String Displyed when Low Temperature.");
	ADD_APPLICATION_STRING2(STR_LOW_BATTERY_SHUTDOWN,"Low Battery Shutdown!","String Displyed when Low battery shutdown.");
	ADD_APPLICATION_STRING2(STR_LOW_CURRENT_BATTERY,"Warning! Low Charge Current!","String Displyed when charge current is low.");	
	ADD_APPLICATION_STRING2(STR_CHARGE_BAD_CONTACT,"Warning! Bad Contact -Charger!","String Displyed when charger bad contact.");	
	ADD_APPLICATION_STRING2(STR_BATTERY_BAD_CONTACT,"Warning! Bad Contact -Battery!","String Displyed when battery bad contact.");
        ADD_APPLICATION_STRING2(STR_LOW_BATTERY_MT_PROHIBIT,"Battery Low! Not able to dial!","String Displyed when battery low, not able to MT.");	

#ifdef __MMI_SUBLCD__
	ADD_APPLICATION_STRING2(STR_SUBLCD_ABNORMAL_BATTERY,"Warning! Abnormal Battery Status","#NO_TRANS# String Associated with Abnormal Battery Status on Sub LCD");
	ADD_APPLICATION_IMAGE2(IMG_SUBLCD_CHARGER_POWER_ON,CUST_IMG_PATH"/SUBLCD/Active/SB_char.gif","	Image Associated with SubLCD for Power On Charger Animation Screen.");
	ADD_APPLICATION_IMAGE2(IMG_SUBLCD_CHARGING_COMPLETE,CUST_IMG_PATH"/SUBLCD/Active/SB_charok.bmp","	SubLCD Image Associated with Charging Complete.");
	ADD_APPLICATION_IMAGE2(IMG_SUBLCD_ABNORMAL_BATTERY,CUST_IMG_PATH"/SUBLCD/Active/SB_charwg.bmp","	SubLCD Image Associated with abnormal battery Screen.");
#endif

}

/**************************************************************

	FUNCTION NAME		: PopulateQuickAccessMenu()

  	PURPOSE				: Populate Quick Access Menu Data

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: This function is developer specific call to automate
						  default resource data population 

**************************************************************/
void PopulateQuickAccessData(void)
{
	/*Populate Menu Items */

	ADD_APPLICATION_STRING2(KPAD_SCR_UNLOCKED_MESSAGE_RUNNING_STR,"KeyPad Unlocked!","KeyPad Unlock Scr Caption.");
	ADD_APPLICATION_IMAGE2(IMG_KPAD_SCR_UNLOCKED_HELP_MESSAGE,CUST_IMG_PATH"/MainLCD/Warningpage/notification/N_KEY.bmp","Img Associated with Keypad Lock Help Message Screen.");

#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__  //Robin 1223
	ADD_APPLICATION_STRING2(KPAD_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR,"Press # to Unlock Keypad","Key Pad Unlock Running Text.");
   #if defined(__MMI_KEYPAD_LOCK_PROMPT_TEXT__)				
	ADD_APPLICATION_STRING2(KPAD_SCR_UNLOCKED_FIRST_HELP_MESSAGE_RUNNING_STR,"Press RSK to Unlock Keypad","Key Pad Unlock Running Text.");
   #endif
#else  /* __MMI_KEYPAD_LOCK_PATTERN_2__ */
	ADD_APPLICATION_STRING2(KPAD_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR2,"Press * to Unlock Keypad","Key Pad Unlock Running Text.");
   #if defined(__MMI_KEYPAD_LOCK_PROMPT_TEXT__)				
	ADD_APPLICATION_STRING2(KPAD_SCR_UNLOCKED_FIRST_HELP_MESSAGE_RUNNING_STR2,"Press LSK to Unlock Keypad","Key Pad Unlock Running Text.");
   #endif
#endif
#ifdef __MMI_TOUCH_SCREEN__
	ADD_APPLICATION_STRING2(TOUCH_SCR_UNLOCKED_FIRST_HELP_MESSAGE_RUNNING_STR,"Press SEND key  to Unlock Keypad and Screen","Key Pad Unlock Running Text.");
	ADD_APPLICATION_STRING2(TOUCH_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR,"Click Here to Unlock", "prompt user to click the image");
	ADD_APPLICATION_IMAGE2(IMG_TOUCH_SCR_PEN_CLICK_AREA,CUST_IMG_PATH"/MainLCD/UIElement/Calibration/CALI_SMALL.gif","click the image to unlock screen");
#endif
}


//////////////////////////////////////////////////////////////////////////////////
void PopulateMiscFrameworkMenu(void)
{
//qiff append extra menu to no sim menu
#if defined __MMI_MAINLCD_240X320__
	ADD_APPLICATION_MENUITEM((MENU_SIM_OPTION_ID,0,8,
				MAIN_MENU_PHONEBOOK_NO_SIM_MENUID,
				MAIN_MENU_MULTIMEDIA_MENUID,
				MAIN_MENU_FUNANDGAMES_MENUID, 
				MAIN_MENU_ORGANIZER_MENUID,
				MAIN_MENU_SETTINGS_MENUID_NO_SIM,
				MAIN_MENU_FILE_MNGR_MENUID,
				MAIN_MENU_PROFILES_MENUID,
				MAIN_MENU_EXTRA_MENUID,
				SHOW,INSERTABLE,DISP_LIST,STR_GLOBAL_OPTIONS,MAIN_MENU_TITLE_ICON));
#elif defined PRODUCT_QPAD
	ADD_APPLICATION_MENUITEM((MENU_SIM_OPTION_ID,0,6,
				MAIN_MENU_PHONEBOOK_NO_SIM_MENUID,
				MAIN_MENU_ORGANIZER_MENUID,
				MAIN_MENU_SETTINGS_MENUID_NO_SIM,
				MENU_ID_FMRDO_MAIN,
				MAIN_MENU_ALARM_MENUID,
				MAIN_MENU_FUNANDGAMES_MENUID,
				SHOW,INSERTABLE,DISP_LIST,STR_GLOBAL_OPTIONS,MAIN_MENU_TITLE_ICON));
#else
	ADD_APPLICATION_MENUITEM((MENU_SIM_OPTION_ID,0,9,
				MAIN_MENU_PHONEBOOK_NO_SIM_MENUID,
				MENU_ID_CAMERA_APP,	
				MAIN_MENU_PROFILES_MENUID,
				MAIN_MENU_EXTRA_MENUID,
				MAIN_MENU_MULTIMEDIA_MENUID,
				MAIN_MENU_FUNANDGAMES_MENUID, 
				MAIN_MENU_ORGANIZER_MENUID,
				MAIN_MENU_SETTINGS_MENUID_NO_SIM,
				MAIN_MENU_FILE_MNGR_MENUID,
				SHOW,INSERTABLE,DISP_LIST,STR_GLOBAL_OPTIONS,MAIN_MENU_TITLE_ICON));

#endif
	
}

/**************************************************************


	FUNCTION NAME		: PopulateSimDetectionMenuRes()

  	PURPOSE				: To Initialize the resources for the 
						  idle app 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/

void PopulateSimDetectionMenuRes(void)
{
	PopulateMiscFrameworkMenu();
//micha0923, #169 begin
	ADD_APPLICATION_STRING2(STR_SIM_INVALID_SIM_TITLE,"Invalid SIM","Invalid SIM title.");
	ADD_APPLICATION_STRING2(STR_SIM_INVALID_SIM_TXT,"SIM card registration failed.","Invalid SIM text.");
//micha1217	
	ADD_APPLICATION_STRING2(STR_SIM_INVALID_SIM_SELECT_NETWORK_TXT,"Not avaliable for invalid SIM.","Invalid SIM network selection text.");
//micha0923, #169 begin

//	ADD_APPLICATION_STRING2(STR_SIM_INSERTION_RSK,"SOS","	RSK SOS screen String");
//	ADD_APPLICATION_IMAGE2(IMG_SIM_INSERTION_RSK,"../IMAGES/EmptyImage.bmp","	Image Associated with Insert Sim Screen RSK.");
	ADD_APPLICATION_STRING2(STR_SIM_INSERTION_MSG,"Insert SIM","Insert the SIM card MEssage for SIM screen.");

	#ifdef __MMI_DOUBLE_SIM__
	ADD_APPLICATION_STRING2(STR_INSERT_SIM_1,"Insert SIM 1","Insert SIM 1");
	ADD_APPLICATION_STRING2(STR_INSERT_SIM_2,"Insert SIM 2","Insert SIM 2");
	#endif

	ADD_APPLICATION_STRING2(STR_SIM_ACCESS_ERROR_MSG,"SIM Access Error","SIM Access Error Message for SIM screen.");
#ifdef __MMI_SUBLCD__
	ADD_APPLICATION_STRING2(STR_SUBLCD_SIM_INSERTION_MSG,"Insert SIM","#NO_TRANS# Insert the SIM card MEssage for SubLCD screen.");
	ADD_APPLICATION_STRING2(STR_SUBLCD_SIM_ACCESS_ERROR_MSG,"SIM Access Error","SIM Access Error Message for SIM screen.");
#endif

//	ADD_APPLICATION_STRING2(STR_SIM_BLOCK_RSK,"SOS"," Block SIM	RSK SOS screen String");
//	ADD_APPLICATION_IMAGE2(IMG_SIM_BLOCK_RSK,"../IMAGES/EmptyImage.bmp","	Image Associated with Block Sim Screen RSK.");
	ADD_APPLICATION_STRING2(STR_SIM_BLOCK_MSG,"SIM Blocked","Blocked SIM card Message for SIM screen.");
#ifdef __MMI_SUBLCD__
	ADD_APPLICATION_STRING2(STR_SUBLCD_SIM_BLOCK_MSG,"SIM Blocked","#NO_TRANS# Blocked SIM card Message for SUBLCD screen.");
#endif
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	ADD_APPLICATION_STRING2(STR_ENTER_CARD1_PIN_MSG,"Enter SIM1 PIN:","Enter Pin Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_CARD2_PIN_MSG,"Enter SIM2 PIN:","Enter Pin Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_SIM1_PUK_MSG,"Enter SIM1 PUK:","Enter PUK Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_SIM2_PUK_MSG,"Enter SIM2 PUK:","Enter PUK Screen MSG.");
	ADD_APPLICATION_STRING2(STR_SIM1_PIN_BLOCKED_PUK, "SIM1 PIN Blocked!","Running text on PIN Blocked Screen.");
	ADD_APPLICATION_STRING2(STR_SIM2_PIN_BLOCKED_PUK, "SIM2 PIN Blocked!","Running text on PIN Blocked Screen.");
	#if(MMI_SIM_NUMBER >= 3)
	ADD_APPLICATION_STRING2(STR_ENTER_CARD3_PIN_MSG,"Enter SIM3 PIN:","Enter Pin Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_SIM3_PUK_MSG,"Enter SIM3 PUK:","Enter PUK Screen MSG.");
	ADD_APPLICATION_STRING2(STR_SIM3_PIN_BLOCKED_PUK, "SIM3 PIN Blocked!","Running text on PIN Blocked Screen.");
	#endif
	#if(MMI_SIM_NUMBER >= 4)
	ADD_APPLICATION_STRING2(STR_ENTER_CARD4_PIN_MSG,"Enter SIM4 PIN:","Enter Pin Screen MSG.");
	ADD_APPLICATION_STRING2(STR_ENTER_SIM4_PUK_MSG,"Enter SIM4 PUK:","Enter PUK Screen MSG.");
	ADD_APPLICATION_STRING2(STR_SIM4_PIN_BLOCKED_PUK, "SIM4 PIN Blocked!","Running text on PIN Blocked Screen.");
	#endif
#else
	ADD_APPLICATION_STRING2(STR_ENTER_PIN_MSG,"Enter PIN:","Enter Pin Screen MSG.");
#endif
#ifdef __MMI_SUBLCD__
	ADD_APPLICATION_STRING2(STR_SUBLCD_ENTER_PIN_MSG,"Enter PIN","#NO_TRANS# Enter Pin SUB LCD Screen MSG.");
#endif
	ADD_APPLICATION_IMAGE2(IMG_ENTER_PIN_CAPTION,CUST_IMG_BASE_PATH"/EmptyImage.bmp","Img Associated with Enter PIN Caption");
//	ADD_APPLICATION_STRING2(STR_ENTER_PIN_RSK,"SOS","Make SOS Calls");
//	ADD_APPLICATION_IMAGE2(IMG_ENTER_PIN_RSK,"../IMAGES/EmptyImage.bmp","Img Associated with Enter PIN RSK.");

	/* Etry Scr Entr PUK*/
#ifdef __MMI_SUBLCD__
	ADD_APPLICATION_STRING2(STR_SUBLCD_ENTER_PUK_MSG, "Enter PUK","#NO_TRANS# Enter PUK SUBLCD Scr Caption.");
#endif
	ADD_APPLICATION_STRING2(STR_ENTER_PUK_MSG,"Enter PUK:","Enter PUK Screen MSG.");
//	ADD_APPLICATION_STRING2(STR_ENTER_PUK_RSK, "SOS", "Make SOS Calls.");

	/*Entry Scr PIN incorrect*/
	ADD_APPLICATION_STRING2(STR_INCORRECT_PIN, "Wrong PIN!","Running text on PIN comparison screen.");

	/*Entry Scr PUK incorrect*/
	ADD_APPLICATION_STRING2(STR_INCORRECT_PUK, "Wrong PUK!","Running text for incorrct PUK.");

	/*Entry Scr PIN blocked */
	ADD_APPLICATION_STRING2(STR_PIN_BLOCKED_PUK, "PIN Blocked!","Running text on PIN Blocked Screen.");

	/*Enty Scr Enter New PIN first Time */
	ADD_APPLICATION_STRING2(STR_ENTER_NEW_PIN_MSG,"Enter New PIN:","Enter New Pin First Time Screen MSG.");
	/*Enty Scr Enter New PIN Again ie second Time */
	ADD_APPLICATION_STRING2(STR_ENTER_PIN_AGAIN_MSG,"Enter Again:","Confirm Pin Screen MSG.");

	/*Enty Scr Enter Pins don't match */
	ADD_APPLICATION_STRING2(STR_PIN_DONOT_MATCH, "Different PIN code!","Running Text of PIN Mismatch Screen.");

	/*Making SOS call , you need to display Call and Drop on the Scr MakEmergencyCall*/
	/* Robin 0726, Use Global defination */
	//ADD_APPLICATION_STRING2(KPAD_SCR_EMERGENCY_CALL_PIN_PUK_CANCEL, "Clear","RSK ssociated with Drop emergency Call.");	

	/* Phone unlock*/
	ADD_APPLICATION_STRING2(STR_ENTER_PHONELOCK_MSG,"Enter Phone Lock Code:","Enter Phone Lock Screen MSG.");
#ifdef __MMI_SUBLCD__
	ADD_APPLICATION_STRING2(STR_SUBLCD_ENTER_PHONELOCK_MSG, "Enter Code","#NO_TRANS# Enter PhoneLock SUBLCD Scr MSG.");
#endif
	/* Incorrect Phone Unlock */
	ADD_APPLICATION_STRING2(STR_INCORRECT_PHONELOCK, "Wrong Password!" ,"Incorrect Phone Lock Running text.");

	ADD_APPLICATION_STRING2(STR_CODE_VERIFIED, "Code Accepted!" ,"String associated with Code Verified Screen.");

	ADD_APPLICATION_STRING2(STR_RSK_SOS, "SOS","RSK ssociated with All Pwd Req Screens.");	

	/* Entry Screen No Service Available*/
	
	ADD_APPLICATION_STRING2(STR_NO_SERVICE_RUNNING_TEXT,"Service Not Available!","No Service Running Text.");

	ADD_APPLICATION_STRING2(STR_NOT_SOS_NUMBER,"Emergency Call Only!","Not a SOS number screen Text.");

	/* Entry Scr for Invalid PUK , PIN , Phone Lock Length */
	ADD_APPLICATION_STRING2(STR_INVALID_PINPUKPLOCK_LENGTH_RUNNING_TEXT,"Invalid Length!","Running Text on Incorrect PIN/PUK length.");

	ADD_APPLICATION_STRING2(STR_DIAL_EMERGENCY_NUMBER,"Dial Emergency Number","Text on SOS Screen.");

	ADD_APPLICATION_STRING2(STR_INCORRECT_SSC,"Not Allowed!","Error Msg for Wrong SSC from Enter PUK Screen.");


//Robin 1217, micha1119	
	ADD_APPLICATION_STRING2(STR_LIMITED_SERVICE,"Emergency","Limited Service String.");

	ADD_APPLICATION_STRING2(STR_NO_SERVICE,"No Service","No Service String.");

	/*image assciated with welcome screen*/
	#ifdef LOW_COST_SUPPORT
	ADD_APPLICATION_IMAGE2(IMG_WELCOME_SCR,	CUST_IMG_PATH "/MainLCD/Active/poweronoff/logo.jpg","Img Associated with Welcome Screen.");
	#else
	ADD_APPLICATION_IMAGE2(IMG_WELCOME_SCR,	CUST_IMG_PATH"/MainLCD/Active/Greeting/GREET.gif","Img Associated with Welcome Screen.");
	#endif
//	ADD_APPLICATION_IMAGE2(IMG_SIM_INSERTION_MSG,CUST_IMG_PATH"/MainLCD/Warningpage/popupPage/PP_WARN.bmp","	Image Associated with Insert Sim Screen MSG.");
#if defined(__MMI_APHORISM__)
/* under construction !*/
#endif

#ifdef __MMI_SUBLCD__
	ADD_APPLICATION_IMAGE2(IMG_SUBLCD_POWER_ON_SCR,	CUST_IMG_PATH"/SubLCD/Active/SB_ON.gif","Img Associated with SUBLCD Power On Screen.");
#endif

#if defined(  __FS_CHECKDRIVE_SUPPORT__ )
	ADD_APPLICATION_STRING2(STR_NAND_CHECK_TITLE,"NAND Check","NAND Check");
	ADD_APPLICATION_STRING2(STR_NAND_CHECK,"NAND Check?","NAND Check?");
	ADD_APPLICATION_STRING2(STR_NAND_CHECK_PROCESSING,"Processing","Processing");
#endif
#if 0
	ADD_APPLICATION_STRING2(STR_ID_SIM_REFRESH,"SIM card is refreshing...","SIM REFRESH");

	ADD_APPLICATION_STRING2(STR_ID_NAND_QUOTA_OVER_DISK,"Quota over disk!","Quota over disk");
	ADD_APPLICATION_STRING2(STR_ID_NAND_QUOTA_USAGE_WARNING,"Quota usage warning!","Quota usage warning");
	ADD_APPLICATION_STRING2(STR_ID_NAND_DEVICE_NOT_SUPPORTED,"NAND device not support!","NAND device not support");
#endif
	/*POPULATE OPTIONS MENU*/
	populateOptionMenu();
//#if defined(COOLSAND) || defined(KLM)
#if defined(__MMI_APHORISM__)
/* under construction !*/
#endif
	PopulatePLMNList();
	PopulatePersonalizationRes();


}

/**************************************************************

	FUNCTION NAME		: populateOptionMenu(void)

  	PURPOSE				: populating various resources



	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/
void populateOptionMenu(void)
{
#if 0
#if defined(__MMI_VERSION_2__)
   ADD_APPLICATION_MENUITEM((MENU_SIM_OPTION_ID,0,
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
	1+
#endif
	8,
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
		MAIN_MENU_OPTION_AVZONE_MENUID,
#endif
		MAIN_MENU_PHONEBOOK_NO_SIM_MENUID,
		MAIN_MENU_MULTIMEDIA_MENUID,
		MAIN_MENU_FILE_MNGR_MENUID,
		MAIN_MENU_FUNANDGAMES_MENUID,
		MAIN_MENU_PROFILES_MENUID,
		MAIN_MENU_ORGANIZER_MENUID,
		MAIN_MENU_EXTRA_MENUID,
		MAIN_MENU_SETTINGS_NO_SIM_MENUID, /* diamond, 2005/12/23 Phone setup in option menu */
		SHOW,NONMOVEABLE,DISP_LIST,STR_GLOBAL_OPTIONS,0));
#else

	ADD_APPLICATION_MENUITEM((MENU_SIM_OPTION_ID,0,
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
	1+
#endif
#ifdef __MMI_TOUCH_SCREEN__
	1+
#endif
	5,
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
		MAIN_MENU_OPTION_AVZONE_MENUID,
#endif
		MAIN_MENU_PHONEBOOK_NO_SIM_MENUID,
		MAIN_MENU_MULTIMEDIA_MENUID,
		MAIN_MENU_FUNANDGAMES_MENUID, MAIN_MENU_PROFILES_MENUID, MAIN_MENU_ORGANIZER_MENUID,
#ifdef __MMI_TOUCH_SCREEN__
		MENU_SETTING_CALIBRATION,
#endif		
		SHOW,NONMOVEABLE,DISP_LIST,STR_GLOBAL_OPTIONS,0));
#endif
#endif
	ADD_APPLICATION_STRING2(STR_SIM_OPTION_CAPTION	,"Menu Options","caption string of option menu.");

#ifdef __MMI_MAINLCD_240X320__
#if 0
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_PHONEBOOK_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_PB.pbm", "phonebook icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_MESSAGES_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_MG.pbm", "message icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_FUNANDGAMES_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_FG.pbm", "fun and game icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_ORGANIZER_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_OR.pbm", "organizer icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_SETTINGS_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_ST.pbm", "setting icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_PROFILES_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_PR.pbm", "profile icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_SHORTCUTS_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_SC.pbm", "shortcut icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_SERVICES_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_SV.pbm", "service icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_CALL_HISTORY_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_CA.pbm", "call history icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_MULTIMEDIA_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_MM.pbm", "multimedia icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_FILEMNGR_ICON,	CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_FM.pbm", "file manager icon for sim option menu");
	ADD_APPLICATION_IMAGE2(IMG_ID_OPTION_MENU_EXTRA_ICON,		CUST_IMG_PATH"/MainLCD/Submenu/NoSIM/NS_EX.pbm", "extra icon for sim option menu");
#endif
#endif
}

/**************************************************************

	FUNCTION NAME		: PopulateAphorismList(void)

  	PURPOSE				: populating various resources



	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 
	
**************************************************************/
void PopulateAphorismList(void)
{
//#if defined(COOLSAND) || defined(KLM)	
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
#endif
}

/**************************************************************

	FUNCTION NAME		: PopulatePLMNList(void)

  	PURPOSE				: populating PLMN list



	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/
void PopulatePLMNList(void)
{
/* Do NOT modify the following line. It is a keyword for the PLMN automatic create tool */
/* plmncreate.exe: populate PLMN strings */
/* DONE */
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_1,"China Mobile","PLMN Name 46000.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_2,"China Unicom","PLMN Name 46001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_3,"FarEasTone","PLMN Name 46601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_4,"KG Telecom","PLMN Name 46688.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_5,"VIBO","PLMN Name 46689.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_6,"ChungHwa","PLMN Name 46692.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_7,"MobiTai","PLMN Name 46693.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_8,"Taiwan Mobile","PLMN Name 46697.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_9,"TransAsia","PLMN Name 46699.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_10,"CSL","PLMN Name 45400.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_11,"CSL","PLMN Name 45402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_12,"CSL","PLMN Name 45418.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_13,"3 HK","PLMN Name 45403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_14,"3 HK","PLMN Name 45404.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_15,"SmarToneVodafone","PLMN Name 45406.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_16,"New World","PLMN Name 45410.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_17,"CMCC","PLMN Name 45412.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_18,"SmarToneVodafone","PLMN Name 45415.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_19,"PCCW","PLMN Name 45416.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_20,"PCCW","PLMN Name 45419.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_21,"SmarToneVodafone","PLMN Name 45500.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_22,"CTM","PLMN Name 45501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_23,"CTM","PLMN Name 45504.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_24,"3 Macau","PLMN Name 45503.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_25,"3 Macau","PLMN Name 45505.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_26,"Vodafone IN","PLMN Name 40401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_27,"Airtel","PLMN Name 40402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_28,"Airtel","PLMN Name 40403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_29,"IDEA","PLMN Name 40404.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_30,"Vodafone IN","PLMN Name 40405.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_31,"IDEA","PLMN Name 40407.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_32,"Airtel","PLMN Name 40410.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_33,"Vodafone IN","PLMN Name 40411.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_34,"IDEA","PLMN Name 40412.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_35,"Vodafone IN","PLMN Name 40413.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_36,"IDEA","PLMN Name 40414.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_37,"Vodafone IN","PLMN Name 40415.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_38,"Aircel","PLMN Name 40417.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_39,"Reliance","PLMN Name 40418.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_40,"IDEA","PLMN Name 40419.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_41,"Vodafone IN","PLMN Name 40420.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_42,"Loop","PLMN Name 40421.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_43,"IDEA","PLMN Name 40422.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_44,"IDEA","PLMN Name 40424.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_45,"Aircel","PLMN Name 40425.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_46,"Vodafone IN","PLMN Name 40427.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_47,"Aircel","PLMN Name 40428.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_48,"Aircel","PLMN Name 40429.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_49,"Airtel","PLMN Name 40431.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_50,"Aircel","PLMN Name 40433.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_51,"CellOne","PLMN Name 40434.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_52,"Aircel","PLMN Name 40435.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_53,"Aircel","PLMN Name 40437.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_54,"CellOne","PLMN Name 40438.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_55,"Airtel","PLMN Name 40440.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_56,"Aircel","PLMN Name 40441.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_57,"Aircel","PLMN Name 40442.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_58,"Vodafone IN","PLMN Name 40443.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_59,"IDEA","PLMN Name 40444.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_60,"Airtel","PLMN Name 40445.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_61,"Vodafone IN","PLMN Name 40446.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_62,"Airtel","PLMN Name 40449.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_63,"CellOne","PLMN Name 40451.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_64,"CellOne","PLMN Name 40453.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_65,"CellOne","PLMN Name 40454.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_66,"CellOne","PLMN Name 40455.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_67,"IDEA","PLMN Name 40456.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_68,"CellOne","PLMN Name 40457.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_69,"CellOne","PLMN Name 40458.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_70,"CellOne","PLMN Name 40459.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_71,"Vodafone IN","PLMN Name 40460.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_72,"CellOne","PLMN Name 40462.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_73,"CellOne","PLMN Name 40464.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_74,"CellOne","PLMN Name 40466.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_75,"MTNL","PLMN Name 40468.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_76,"MTNL","PLMN Name 40469.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_77,"Airtel","PLMN Name 40470.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_78,"CellOne","PLMN Name 40471.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_79,"CellOne","PLMN Name 40472.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_80,"CellOne","PLMN Name 40473.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_81,"CellOne","PLMN Name 40474.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_82,"CellOne","PLMN Name 40475.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_83,"CellOne","PLMN Name 40476.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_84,"CellOne","PLMN Name 40477.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_85,"IDEA","PLMN Name 40478.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_86,"CellOne","PLMN Name 40479.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_87,"CellOne","PLMN Name 40480.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_88,"CellOne","PLMN Name 40481.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_89,"IDEA","PLMN Name 40482.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_90,"Vodafone IN","PLMN Name 40484.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_91,"Vodafone IN","PLMN Name 40486.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_92,"IDEA","PLMN Name 40487.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_93,"Vodafone IN","PLMN Name 40488.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_94,"IDEA","PLMN Name 40489.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_95,"Airtel","PLMN Name 40490.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_96,"Aircel","PLMN Name 40491.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_97,"Airtel","PLMN Name 40492.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_98,"Airtel","PLMN Name 40493.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_99,"Airtel","PLMN Name 40494.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_100,"Airtel","PLMN Name 40495.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_101,"Airtel","PLMN Name 40496.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_102,"Airtel","PLMN Name 40497.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_103,"Airtel","PLMN Name 40498.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_104,"Vodafone","PLMN Name 404030.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_105,"Reliance","PLMN Name 40501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_106,"Reliance","PLMN Name 40505.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_107,"Reliance","PLMN Name 40506.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_108,"Reliance","PLMN Name 40507.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_109,"Reliance","PLMN Name 40509.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_110,"Reliance","PLMN Name 40510.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_111,"Reliance","PLMN Name 40511.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_112,"Reliance","PLMN Name 40513.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_113,"Reliance","PLMN Name 40515.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_114,"Reliance","PLMN Name 40518.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_115,"Reliance","PLMN Name 40519.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_116,"Reliance","PLMN Name 40520.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_117,"Reliance","PLMN Name 40521.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_118,"Reliance","PLMN Name 40522.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_119,"Airtel","PLMN Name 40551.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_120,"Airtel","PLMN Name 40552.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_121,"Airtel","PLMN Name 40553.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_122,"Airtel","PLMN Name 40554.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_123,"Airtel","PLMN Name 40555.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_124,"Airtel","PLMN Name 40556.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_125,"Vodafone IN","PLMN Name 40566.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_126,"Vodafone IN","PLMN Name 40567.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_127,"IDEA","PLMN Name 40570.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_128,"Tata Docomo","PLMN Name 405025.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_129,"Tata Docomo","PLMN Name 405027.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_130,"Tata Docomo","PLMN Name 405029.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_131,"Tata Docomo","PLMN Name 405030.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_132,"Tata Docomo","PLMN Name 405031.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_133,"Tata Docomo","PLMN Name 405032.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_134,"Tata Docomo","PLMN Name 405034.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_135,"Tata Docomo","PLMN Name 405035.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_136,"Tata Docomo","PLMN Name 405036.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_137,"Tata Docomo","PLMN Name 405037.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_138,"Tata Docomo","PLMN Name 405038.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_139,"Tata Docomo","PLMN Name 405039.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_140,"Tata Docomo","PLMN Name 405041.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_141,"Tata Docomo","PLMN Name 405042.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_142,"Tata Docomo","PLMN Name 405043.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_143,"Tata Docomo","PLMN Name 405044.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_144,"Tata Docomo","PLMN Name 405045.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_145,"Tata Docomo","PLMN Name 405046.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_146,"Tata Docomo","PLMN Name 405047.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_147,"Vodafone IN","PLMN Name 405750.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_148,"Vodafone IN","PLMN Name 405751.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_149,"Vodafone IN","PLMN Name 405752.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_150,"Vodafone IN","PLMN Name 405753.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_151,"Vodafone IN","PLMN Name 405754.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_152,"Vodafone IN","PLMN Name 405755.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_153,"Vodafone IN","PLMN Name 405756.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_154,"IDEA","PLMN Name 405799.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_155,"Aircel","PLMN Name 405800.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_156,"Aircel","PLMN Name 405801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_157,"Aircel","PLMN Name 405803.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_158,"Aircel","PLMN Name 405805.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_159,"Aircel","PLMN Name 405809.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_160,"Aircel","PLMN Name 405810.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_161,"Aircel","PLMN Name 405811.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_162,"IDEA","PLMN Name 40585.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_163,"IDEA","PLMN Name 405852.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_164,"AWCC","PLMN Name 41201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_165,"Roshan","PLMN Name 41220.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_166,"Areeba","PLMN Name 41240.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_167,"Etisalat Af","PLMN Name 41250.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_168,"GramenPhone","PLMN Name 47001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_169,"AKTel","PLMN Name 47002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_170,"Banglalink","PLMN Name 47003.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_171,"Teletalk","PLMN Name 47004.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_172,"Warid Telecom","PLMN Name 47007.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_173,"BATELCO","PLMN Name 42601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_174,"zain BH","PLMN Name 42602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_175,"B-Mobile","PLMN Name 40211.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_176,"TashiCell","PLMN Name 40277.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_177,"B-mobile","PLMN Name 52802.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_178,"DSTCom","PLMN Name 52811.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_179,"Mobitel KH","PLMN Name 45601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_180,"hello","PLMN Name 45602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_181,"CADCOMMS","PLMN Name 45604.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_182,"STARCELL","PLMN Name 45605.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_183,"Mfone","PLMN Name 45618.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_184,"TLS-TT","PLMN Name 51402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_185,"INDOSAT","PLMN Name 51001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_186,"axis","PLMN Name 51008.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_187,"Telkomsel","PLMN Name 51010.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_188,"XL","PLMN Name 51011.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_189,"INDOSAT","PLMN Name 51021.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_190,"3","PLMN Name 51089.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_191,"MCI","PLMN Name 43211.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_192,"KISH","PLMN Name 43214.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_193,"MTCE","PLMN Name 43219.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_194,"Taliya","PLMN Name 43232.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_195,"IranCell","PLMN Name 43235.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_196,"ASIACELL","PLMN Name 41805.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_197,"zain IQ","PLMN Name 41820.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_198,"Orange","PLMN Name 42501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_199,"Orange","PLMN Name 42505.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_200,"Cellcom","PLMN Name 42502.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_201,"Pelephone","PLMN Name 42503.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_202,"eMobile","PLMN Name 44000.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_203,"NTT DoCoMo","PLMN Name 44010.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_204,"Softbank","PLMN Name 44020.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_205,"zain JO","PLMN Name 41601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_206,"Umniah","PLMN Name 41603.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_207,"Orange JO","PLMN Name 41677.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_208,"KTF","PLMN Name 45002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_209,"KTF","PLMN Name 45008.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_210,"SK Telecom","PLMN Name 45005.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_211,"zain KW","PLMN Name 41902.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_212,"Wataniya","PLMN Name 41903.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_213,"BITEL","PLMN Name 43701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_214,"MEGACOM","PLMN Name 43705.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_215,"LAO GSM","PLMN Name 45701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_216,"ETL","PLMN Name 45702.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_217,"LAO-ASIA","PLMN Name 45703.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_218,"TANGO LAO","PLMN Name 45708.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_219,"Alfa","PLMN Name 41501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_220,"MY MAXIS","PLMN Name 50212.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_221,"MY CELCOM 3G","PLMN Name 50213.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_222,"DiGi","PLMN Name 50216.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_223,"U MOBILE","PLMN Name 50218.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_224,"MY CELCOM","PLMN Name 50219.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_225,"Dhiraagu","PLMN Name 47201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_226,"WMOBILE","PLMN Name 47202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_227,"Unitel","PLMN Name 42888.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_228,"MobiCom","PLMN Name 42899.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_229,"MPT","PLMN Name 41401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_230,"NTC","PLMN Name 42901.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_231,"Mero Mobile","PLMN Name 42902.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_232,"OMAN MOBILE","PLMN Name 42202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_233,"nawras","PLMN Name 42203.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_234,"Mobilink","PLMN Name 41001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_235,"PK-UFONE","PLMN Name 41003.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_236,"ZONG","PLMN Name 41004.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_237,"Telenor PK","PLMN Name 41006.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_238,"WaridTel","PLMN Name 41007.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_239,"ISLACOM","PLMN Name 51501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_240,"Globe Telecom","PLMN Name 51502.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_241,"SMART","PLMN Name 51503.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_242,"Sun Cellular","PLMN Name 51505.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_243,"Smart","PLMN Name 51518.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_244,"Qat-Qtel","PLMN Name 42701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_245,"STC","PLMN Name 42001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_246,"mobily","PLMN Name 42003.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_247,"SingTel","PLMN Name 52502.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_248,"SingTel","PLMN Name 52501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_249,"M1","PLMN Name 52503.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_250,"SSTARHUB","PLMN Name 52505.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_251,"Mobitel","PLMN Name 41301.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_252,"SRI DIALOG","PLMN Name 41302.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_253,"SRI - Tigo","PLMN Name 41303.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_254,"SIR AIRTEL","PLMN Name 41305.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_255,"Hutch","PLMN Name 41308.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_256,"SyriaTel","PLMN Name 41701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_257,"Areeba","PLMN Name 41702.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_258,"Somoncom","PLMN Name 43601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_259,"INDIGO-T","PLMN Name 43602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_260,"INDIGO-T","PLMN Name 43612.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_261,"MLT","PLMN Name 43603.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_262,"Babilon","PLMN Name 43604.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_263,"BEELINE","PLMN Name 43605.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_264,"AIS GSM","PLMN Name 52001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_265,"TRUE","PLMN Name 52010.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_266,"TRUE","PLMN Name 52099.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_267,"TOT","PLMN Name 52015.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_268,"DTAC","PLMN Name 52018.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_269,"TH GSM 1800","PLMN Name 52023.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_270,"Turkcell","PLMN Name 28601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_271,"VODAFONE TR","PLMN Name 28602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_272,"AVEA","PLMN Name 28603.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_273,"ETISALAT","PLMN Name 42402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_274,"du","PLMN Name 42403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_275,"Mobifone","PLMN Name 45201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_276,"VINAPHONE","PLMN Name 45202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_277,"Viettel","PLMN Name 45204.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_278,"SabaFon","PLMN Name 42101.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_279,"MTN","PLMN Name 42102.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_280,"AMC - AL","PLMN Name 27601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_281,"vodafone AL","PLMN Name 27602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_282,"EAGLE AL","PLMN Name 27603.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_283,"MOBILAND","PLMN Name 21303.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_284,"Beeline AM","PLMN Name 28301.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_285,"ARM MTS","PLMN Name 28305.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_286,"A1","PLMN Name 23201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_287,"T-Mobile A","PLMN Name 23203.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_288,"Orange AT","PLMN Name 23205.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_289,"telering","PLMN Name 23207.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_290,"3 AT","PLMN Name 23210.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_291,"Orange AT","PLMN Name 23212.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_292,"AZE ACELL","PLMN Name 40001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_293,"AZE BKCELL","PLMN Name 40002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_294,"Nar","PLMN Name 40004.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_295,"Velcom","PLMN Name 25701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_296,"MTS","PLMN Name 25702.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_297,"Best","PLMN Name 25704.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_298,"Proximus","PLMN Name 20601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_299,"Mobistar","PLMN Name 20610.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_300,"BASE","PLMN Name 20620.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_301,"Eronet","PLMN Name 21803.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_302,"m:tel","PLMN Name 21805.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_303,"BH Mobile","PLMN Name 21890.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_304,"M-Tel BG","PLMN Name 28401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_305,"vivatel","PLMN Name 28403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_306,"BG GLOBUL","PLMN Name 28405.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_307,"T-Mobile","PLMN Name 21901.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_308,"Tele2","PLMN Name 21902.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_309,"VIPnet","PLMN Name 21910.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_310,"CYTAVODA","PLMN Name 28001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_311,"Areeba","PLMN Name 28010.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_312,"T-Mobile CZ","PLMN Name 23001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_313,"O2 CZ","PLMN Name 23002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_314,"Vodafone CZ","PLMN Name 23003.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_315,"TDC MOBIL","PLMN Name 23801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_316,"Sonofon","PLMN Name 23802.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_317,"3 DK","PLMN Name 23806.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_318,"TELIA DK","PLMN Name 23820.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_319,"Sonofon","PLMN Name 23877.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_320,"EE EMT","PLMN Name 24801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_321,"EE elisa","PLMN Name 24802.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_322,"TELE2","PLMN Name 24803.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_323,"Føroya Tele","PLMN Name 28801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_324,"KALL","PLMN Name 28802.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_325,"DNA","PLMN Name 24403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_326,"DNA","PLMN Name 24412.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_327,"FI Elisa","PLMN Name 24405.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_328,"FI AMT","PLMN Name 24414.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_329,"FI SONERA","PLMN Name 24491.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_330,"Orange F","PLMN Name 20801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_331,"SFR FR","PLMN Name 20810.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_332,"Bouygues FR","PLMN Name 20820.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_333,"GCELL","PLMN Name 28201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_334,"MAGTI","PLMN Name 28202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_335,"BEELINE","PLMN Name 28204.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_336,"T-Mobile D","PLMN Name 26201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_337,"Vodafone.de","PLMN Name 26202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_338,"E-Plus","PLMN Name 26203.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_339,"O2-de","PLMN Name 26207.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_340,"O2-de","PLMN Name 26208.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_341,"GIBTEL","PLMN Name 26601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_342,"COSMOTE GR","PLMN Name 20201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_343,"vodafone GR","PLMN Name 20205.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_344,"WIND GR","PLMN Name 20210.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_345,"TELE GRL","PLMN Name 29001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_346,"H pannon","PLMN Name 21601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_347,"T-Mobile H","PLMN Name 21630.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_348,"vodafone HU","PLMN Name 21670.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_349,"Siminn","PLMN Name 27401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_350,"Vodafone","PLMN Name 27402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_351,"Viking","PLMN Name 27404.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_352,"IceCell","PLMN Name 27407.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_353,"On-waves","PLMN Name 27408.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_354,"NOVA","PLMN Name 27411.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_355,"vodafone IRL","PLMN Name 27201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_356,"O2 IRL","PLMN Name 27202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_357,"METEOR IRL","PLMN Name 27203.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_358,"3 IRL","PLMN Name 27205.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_359,"TIM IT","PLMN Name 22201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_360,"vodafone IT","PLMN Name 22210.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_361,"WIND IT","PLMN Name 22288.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_362,"3 IT","PLMN Name 22299.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_363,"Beeline KZ","PLMN Name 40101.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_364,"KCELL KZ","PLMN Name 40102.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_365,"NEO KZ","PLMN Name 40177.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_366,"LMT LV","PLMN Name 24701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_367,"TELE2 LV","PLMN Name 24702.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_368,"BITE LV","PLMN Name 24705.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_369,"SwisscomFL","PLMN Name 29501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_370,"Orange FL","PLMN Name 29502.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_371,"FL1","PLMN Name 29505.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_372,"LI TANGO","PLMN Name 29577.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_373,"OMNITEL LT","PLMN Name 24601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_374,"LT BITE GSM","PLMN Name 24602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_375,"TELE2","PLMN Name 24603.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_376,"L LUXGSM","PLMN Name 27001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_377,"L TANGO","PLMN Name 27077.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_378,"L VOX","PLMN Name 27099.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_379,"T-Mobile MK","PLMN Name 29401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_380,"COSMOFON MKD","PLMN Name 29402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_381,"Vip MKD","PLMN Name 29403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_382,"vodafone MT","PLMN Name 27801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_383,"go mobile","PLMN Name 27821.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_384,"3GT MT","PLMN Name 27877.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_385,"Orange MD","PLMN Name 25901.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_386,"MOLDCELL MD","PLMN Name 25902.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_387,"EVENTIS MD","PLMN Name 25904.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_388,"Vodafone NL","PLMN Name 20404.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_389,"KPN NL","PLMN Name 20408.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_390,"Telfort NL","PLMN Name 20412.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_391,"T-Mobile NL","PLMN Name 20416.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_392,"Orange NL","PLMN Name 20420.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_393,"N Telenor","PLMN Name 24201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_394,"N NetCom","PLMN Name 24202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_395,"MTU","PLMN Name 24203.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_396,"Mobile Norway","PLMN Name 24205.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_397,"Plus","PLMN Name 26001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_398,"Era","PLMN Name 26002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_399,"Orange PL","PLMN Name 26003.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_400,"Play","PLMN Name 26006.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_401,"vodafone P","PLMN Name 26801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_402,"Optimus P","PLMN Name 26803.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_403,"TMN P","PLMN Name 26806.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_404,"RO Vodafone","PLMN Name 22601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_405,"RO COSMOTE","PLMN Name 22603.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_406,"RO DigiMobil","PLMN Name 22605.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_407,"RO ORANGE","PLMN Name 22610.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_408,"MTS RUS","PLMN Name 25001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_409,"MegaFon RUS","PLMN Name 25002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_410,"SCN RUS","PLMN Name 25004.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_411,"STC RUS","PLMN Name 25005.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_412,"BMT RUS","PLMN Name 25007.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_413,"DTC RUS","PLMN Name 25010.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_414,"AKOS RUS","PLMN Name 25012.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_415,"Kuban RUS","PLMN Name 25013.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_416,"SMARTS RUS","PLMN Name 25015.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_417,"NTC RUS","PLMN Name 25016.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_418,"Utel RUS","PLMN Name 25017.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_419,"INDIGO RUS","PLMN Name 25019.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_420,"TELE2 RUS","PLMN Name 25020.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_421,"Beeline RUS","PLMN Name 25028.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_422,"MOTIV RUS","PLMN Name 25035.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_423,"Utel RUS","PLMN Name 25039.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_424,"Beeline RUS","PLMN Name 25099.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_425,"Telenor","PLMN Name 22001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_426,"ProMonte","PLMN Name 22002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_427,"mt:s","PLMN Name 22003.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_428,"T-Mobile CG","PLMN Name 22004.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_429,"VIP Mobile","PLMN Name 22005.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_430,"Orange SK","PLMN Name 23101.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_431,"T-Mobile SK","PLMN Name 23102.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_432,"O2 SK","PLMN Name 23106.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_433,"Si.mobil","PLMN Name 29340.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_434,"Mobitel","PLMN Name 29341.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_435,"T-2","PLMN Name 29364.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_436,"Tuamobil","PLMN Name 29370.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_437,"vodafone ES","PLMN Name 21401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_438,"Orange","PLMN Name 21403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_439,"Yoigo","PLMN Name 21404.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_440,"movistar","PLMN Name 21407.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_441,"TELIA SE","PLMN Name 24001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_442,"3 SE","PLMN Name 24002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_443,"Sweden","PLMN Name 24004.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_444,"Sweden 3G","PLMN Name 24005.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_445,"COMVIQ SE","PLMN Name 24007.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_446,"Telenor SE","PLMN Name 24008.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_447,"SpringMobil SE","PLMN Name 24010.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_448,"Swisscom","PLMN Name 22801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_449,"Sunrise","PLMN Name 22802.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_450,"ORANGE","PLMN Name 22803.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_451,"In&Phone","PLMN Name 22807.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_452,"Tele2","PLMN Name 22808.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_453,"UMC","PLMN Name 25501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_454,"Beeline","PLMN Name 25502.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_455,"Kyivstar","PLMN Name 25503.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_456,"Uni","PLMN Name 25505.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_457,"life:)","PLMN Name 25506.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_458,"Utel","PLMN Name 25507.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_459,"UK01","PLMN Name 23401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_460,"O2 UK","PLMN Name 23410.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_461,"Vodafone UK","PLMN Name 23415.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_462,"3 UK","PLMN Name 23420.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_463,"T-Mobile UK","PLMN Name 23430.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_464,"T-Mobile UK","PLMN Name 23431.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_465,"T-Mobile UK","PLMN Name 23432.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_466,"Orange UK","PLMN Name 23433.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_467,"Beeline UZ","PLMN Name 43404.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_468,"UZB Ucell","PLMN Name 43405.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_469,"UZB MTS","PLMN Name 43407.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_470,"MTS TM","PLMN Name 43801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_471,"APUA","PLMN Name 344030.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_472,"bMobile","PLMN Name 344920.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_473,"Digicel","PLMN Name 344930.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_474,"Movistar","PLMN Name 72207.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_475,"Personal","PLMN Name 72234.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_476,"CLARO","PLMN Name 722310.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_477,"SETAR GSM","PLMN Name 36301.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_478,"BTL","PLMN Name 70267.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_479,"Digicel","PLMN Name 35001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_480,"M3","PLMN Name 35002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_481,"VIVA","PLMN Name 73601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_482,"EMOVIL","PLMN Name 73602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_483,"Telecel","PLMN Name 73603.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_484,"TIM BRASIL","PLMN Name 72402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_485,"TIM BRASIL","PLMN Name 72403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_486,"TIM BRASIL","PLMN Name 72404.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_487,"Claro","PLMN Name 72405.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_488,"VIVO","PLMN Name 72406.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_489,"VIVO","PLMN Name 72410.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_490,"VIVO","PLMN Name 72411.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_491,"VIVO","PLMN Name 72423.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_492,"Sercomtel","PLMN Name 72415.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_493,"Brasil Telecom","PLMN Name 72416.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_494,"AMAZONIA","PLMN Name 72424.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_495,"Oi","PLMN Name 72431.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_496,"CTBC","PLMN Name 72432.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_497,"CTBC","PLMN Name 72433.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_498,"CTBC","PLMN Name 72434.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_499,"FIRST","PLMN Name 302350.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_500,"Fido","PLMN Name 302370.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_501,"DMTS","PLMN Name 302380.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_502,"ROGERS","PLMN Name 302720.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_503,"ENTEL","PLMN Name 73001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_504,"movistar","PLMN Name 73002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_505,"CLARO CHL","PLMN Name 73003.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_506,"movistar","PLMN Name 732101.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_507,"TIGO","PLMN Name 732103.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_508,"Comcel","PLMN Name 732123.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_509,"I.C.E.","PLMN Name 71201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_510,"Cubacel","PLMN Name 36801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_511,"orange","PLMN Name 37001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_512,"CLARO DOM","PLMN Name 37002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_513,"movistar","PLMN Name 74000.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_514,"PORTA GSM","PLMN Name 74001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_515,"CLARO SLV","PLMN Name 70601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_516,"Digicel","PLMN Name 70602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_517,"TELEMOVIL","PLMN Name 70603.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_518,"CLARO HND","PLMN Name 708001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_519,"CELTELHND","PLMN Name 70802.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_520,"Digicel","PLMN Name 338050.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_521,"bmobile","PLMN Name 338180.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_522,"F-Orange","PLMN Name 34001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_523,"ONLY","PLMN Name 34002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_524,"DIGICEL","PLMN Name 34020.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_525,"TELCEL","PLMN Name 334020.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_526,"movistar","PLMN Name 33403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_527,"Digicel","PLMN Name 352030.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_528,"bmobile","PLMN Name 352110.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_529,"ClaroGTM","PLMN Name 70401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_530,"Tigo","PLMN Name 70402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_531,"movistar","PLMN Name 70403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_532,"DIGICEL","PLMN Name 73801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_533,"COMCEL","PLMN Name 37201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_534,"TELCELL GSM","PLMN Name 36251.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_535,"CT GSM","PLMN Name 36269.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_536,"ANT","PLMN Name 362951.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_537,"ClaroNIC","PLMN Name 71021.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_538,"ClaroNIC","PLMN Name 71073.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_539,"Móvil","PLMN Name 71401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_540,"VOX","PLMN Name 74401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_541,"Claro","PLMN Name 74402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_542,"movistar","PLMN Name 71606.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_543,"CLARO PER","PLMN Name 71610.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_544,"bmobile","PLMN Name 356110.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_545,"bmobile","PLMN Name 358110.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_546,"AMERIS","PLMN Name 30801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_547,"bmobile","PLMN Name 360110.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_548,"Telesur","PLMN Name 74602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_549,"bmobile","PLMN Name 37412.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_550,"T-Mobile","PLMN Name 31026.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_551,"T-Mobile","PLMN Name 31031.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_552,"T-Mobile","PLMN Name 310160.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_553,"T-Mobile","PLMN Name 310200.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_554,"T-Mobile","PLMN Name 310210.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_555,"T-Mobile","PLMN Name 310220.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_556,"T-Mobile","PLMN Name 310230.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_557,"T-Mobile","PLMN Name 310240.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_558,"T-Mobile","PLMN Name 310250.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_559,"T-Mobile","PLMN Name 310260.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_560,"T-Mobile","PLMN Name 310270.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_561,"T-Mobile","PLMN Name 310660.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_562,"AT&T","PLMN Name 310150.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_563,"AT&T","PLMN Name 310170.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_564,"AT&T","PLMN Name 310380.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_565,"AT&T","PLMN Name 310410.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_566,"Centennial","PLMN Name 310030.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_567,"Cell One","PLMN Name 310130.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_568,"UNICEL","PLMN Name 310890.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_569,"Movistar","PLMN Name 74807.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_570,"CLARO UY","PLMN Name 74810.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_571,"Digitel","PLMN Name 73402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_572,"movistar","PLMN Name 73404.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_573,"Telstra Mobile","PLMN Name 50501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_574,"Optus","PLMN Name 50502.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_575,"vodafone AU","PLMN Name 50503.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_576,"3","PLMN Name 50506.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_577,"Telecom","PLMN Name 54801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_578,"VODAFONE FJ","PLMN Name 54201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_579,"VINI","PLMN Name 54720.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_580,"TSKL","PLMN Name 54509.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_581,"FSM Telecom","PLMN Name 55001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_582,"MOBILIS","PLMN Name 54601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_583,"vodafone NZ","PLMN Name 53001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_584,"Telecom NZ","PLMN Name 53005.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_585,"Palau Mobile","PLMN Name 55280.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_586,"BMobile","PLMN Name 53701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_587,"SamoaTel","PLMN Name 54927.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_588,"BREEZE","PLMN Name 54001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_589,"U-CALL","PLMN Name 53901.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_590,"VUT SMILE","PLMN Name 54101.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_591,"Mobilis","PLMN Name 60301.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_592,"Djezzy","PLMN Name 60302.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_593,"Unitel","PLMN Name 63102.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_594,"TLCL-BEN","PLMN Name 61602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_595,"Areeba","PLMN Name 61603.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_596,"BBCOM","PLMN Name 61604.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_597,"GloBenin","PLMN Name 61605.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_598,"MASCOM","PLMN Name 65201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_599,"Orange","PLMN Name 65202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_600,"Zain","PLMN Name 61302.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_601,"Spacetel","PLMN Name 64201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_602,"Safaris","PLMN Name 64202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_603,"ONATEL","PLMN Name 64203.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_604,"MTN CAM","PLMN Name 62401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_605,"Orange CAM","PLMN Name 62402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_606,"CPV MOVEL","PLMN Name 62501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_607,"Telecel","PLMN Name 62302.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_608,"NationLink","PLMN Name 62304.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_609,"Zain","PLMN Name 62201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_610,"HURI","PLMN Name 654001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_611,"Zain","PLMN Name 62901.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_612,"Libertis","PLMN Name 62910.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_613,"Vodacom CD","PLMN Name 63001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_614,"SCELL CD","PLMN Name 63005.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_615,"EVATIS","PLMN Name 63801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_616,"MobiNiL","PLMN Name 60201.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_617,"Vodafone EG","PLMN Name 60202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_618,"Etisalat","PLMN Name 60203.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_619,"ETHMTN","PLMN Name 63601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_620,"Orange GQ","PLMN Name 62701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_621,"LIBERTIS","PLMN Name 62801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_622,"TELECEL","PLMN Name 62802.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_623,"ZAIN","PLMN Name 62803.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_624,"Gamcel","PLMN Name 60701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_625,"Africell","PLMN Name 60702.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_626,"Areeba","PLMN Name 62001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_627,"Onetouch","PLMN Name 62002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_628,"Tigo","PLMN Name 62003.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_629,"Sotelgui Guinea","PLMN Name 61102.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_630,"ACELL-CI","PLMN Name 61202.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_631,"Orange","PLMN Name 61203.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_632,"MTN CI","PLMN Name 61205.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_633,"Safaricom","PLMN Name 63902.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_634,"Zain","PLMN Name 63903.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_635,"Vodacom-LS","PLMN Name 65101.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_636,"Econet","PLMN Name 65102.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_637,"Al Madar","PLMN Name 60601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_638,"ZAIN","PLMN Name 64601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_639,"Orange","PLMN Name 64602.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_640,"TNL","PLMN Name 65001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_641,"Zain","PLMN Name 65010.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_642,"MALITEL ML","PLMN Name 61001.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_643,"ORANGE ML","PLMN Name 61002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_644,"MATTEL","PLMN Name 60901.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_645,"Mauritel","PLMN Name 60910.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_646,"Cellplus","PLMN Name 61701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_647,"EMTEL","PLMN Name 61710.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_648,"MEDITEL","PLMN Name 60400.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_649,"IAM","PLMN Name 60401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_650,"mCel","PLMN Name 64301.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_651,"VodaCom","PLMN Name 64304.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_652,"MTC","PLMN Name 64901.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_653,"Sonitel","PLMN Name 61401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_654,"Zain","PLMN Name 61402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_655,"Moov","PLMN Name 61403.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_656,"Zain NG","PLMN Name 62120.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_657,"MTN NG","PLMN Name 62130.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_658,"Mtel","PLMN Name 62140.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_659,"Orange","PLMN Name 64700.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_660,"OUTREMER","PLMN Name 64702.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_661,"SFR","PLMN Name 64710.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_662,"MTN","PLMN Name 63510.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_663,"CSTmovel","PLMN Name 62601.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_664,"ALIZE","PLMN Name 60801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_665,"SENTEL","PLMN Name 60802.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_666,"C&W SEY","PLMN Name 63301.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_667,"AIRTEL","PLMN Name 63310.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_668,"Zain","PLMN Name 61901.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_669,"MILLICOM","PLMN Name 61902.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_670,"Telesom","PLMN Name 63701.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_671,"SOMAFONE","PLMN Name 63704.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_672,"Nationlink","PLMN Name 63710.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_673,"Golis","PLMN Name 63730.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_674,"Telsom","PLMN Name 63782.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_675,"VodaCom-SA","PLMN Name 65501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_676,"Cell C","PLMN Name 65507.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_677,"MTN-SA","PLMN Name 65510.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_678,"Zain","PLMN Name 63401.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_679,"MTN","PLMN Name 63402.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_680,"MTN","PLMN Name 65310.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_681,"MOBITEL","PLMN Name 64002.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_682,"ZANTEL","PLMN Name 64003.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_683,"VodaCom","PLMN Name 64004.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_684,"Zain","PLMN Name 64005.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_685,"TOGO CELL","PLMN Name 61501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_686,"Telecel Togo","PLMN Name 61503.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_687,"TUNTEL","PLMN Name 60502.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_688,"TUNISIANA","PLMN Name 60503.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_689,"Zain","PLMN Name 64101.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_690,"MTN","PLMN Name 64110.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_691,"UT Mobile","PLMN Name 64111.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_692,"WaridTel","PLMN Name 64122.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_693,"Zain","PLMN Name 64501.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_694,"MTN","PLMN Name 64502.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_695,"NetOne","PLMN Name 64801.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_696,"TELECEL","PLMN Name 64803.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_697,"ECONET","PLMN Name 64804.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_698,"Vietnamobile VN","PLMN Name 45205.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_699,"Beeline VN","PLMN Name 45207.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_700,"Metfone","PLMN Name 45608.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_701,"Smart","PLMN Name 45606.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_702,"Beeline KH","PLMN Name 45609.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_703,"Airtel","PLMN Name 40406.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_704,"Airtel","PLMN Name 40416.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_705,"Airtel","PLMN Name 40416.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_706,"Airtel","PLMN Name 405153.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_707,"Airtel","PLMN Name 405151.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_708,"IDEA","PLMN Name 405845.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_709,"IDEA","PLMN Name 405846.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_710,"IDEA","PLMN Name 405849.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_711,"IDEA","PLMN Name 405850.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_712,"IDEA","PLMN Name 405848.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_713,"IDEA","PLMN Name 405853.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_714,"Vodafone IN","PLMN Name 40430.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_715,"Aircel","PLMN Name 405802.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_716,"Aircel","PLMN Name 405807.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_717,"Aircel","PLMN Name 405808.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_718,"Aircel","PLMN Name 405804.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_719,"Aircel","PLMN Name 405812.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_720,"Aircel","PLMN Name 405806.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_721,"Reliance","PLMN Name 40409.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_722,"Reliance","PLMN Name 40436.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_723,"Reliance","PLMN Name 40467.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_724,"Reliance","PLMN Name 40450.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_725,"Reliance","PLMN Name 40452.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_726,"Reliance","PLMN Name 40483.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_727,"Reliance","PLMN Name 40485.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_728,"Loop","PLMN Name 405857.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_729,"Loop","PLMN Name 405854.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_730,"Loop","PLMN Name 405855.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_731,"Loop","PLMN Name 405856.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_732,"Loop","PLMN Name 405858.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_733,"Loop","PLMN Name 405859.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_734,"Loop","PLMN Name 405860.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_735,"Loop","PLMN Name 405861.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_736,"Loop","PLMN Name 405862.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_737,"Loop","PLMN Name 405863.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_738,"Loop","PLMN Name 405865.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_739,"Loop","PLMN Name 405866.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_740,"Loop","PLMN Name 405867.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_741,"Loop","PLMN Name 405868.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_742,"Loop","PLMN Name 405869.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_743,"Loop","PLMN Name 405870.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_744,"Loop","PLMN Name 405871.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_745,"Loop","PLMN Name 405872.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_746,"Loop","PLMN Name 405873.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_747,"Loop","PLMN Name 405864.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_748,"Loop","PLMN Name 405874.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_749,"UniNor","PLMN Name 405844.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_750,"UniNor","PLMN Name 405819.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_751,"UniNor","PLMN Name 405875.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_752,"UniNor","PLMN Name 405876.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_753,"UniNor","PLMN Name 405927.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_754,"UniNor","PLMN Name 405813.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_755,"UniNor","PLMN Name 405814.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_756,"UniNor","PLMN Name 405815.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_757,"UniNor","PLMN Name 405820.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_758,"UniNor","PLMN Name 405821.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_759,"UniNor","PLMN Name 405929.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_760,"UniNor","PLMN Name 405926.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_761,"UniNor","PLMN Name 405877.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_762,"UniNor","PLMN Name 405878.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_763,"UniNor","PLMN Name 405816.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_764,"UniNor","PLMN Name 405817.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_765,"UniNor","PLMN Name 405925.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_766,"UniNor","PLMN Name 405879.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_767,"UniNor","PLMN Name 405818.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_768,"UniNor","PLMN Name 405822.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_769,"UniNor","PLMN Name 405880.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_770,"UniNor","PLMN Name 40581.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_771,"Datacom","PLMN Name 405826.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_772,"Datacom","PLMN Name 405823.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_773,"Datacom","PLMN Name 405824.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_774,"Datacom","PLMN Name 405825.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_775,"Datacom","PLMN Name 405827.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_776,"Datacom","PLMN Name 405828.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_777,"Datacom","PLMN Name 405829.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_778,"Datacom","PLMN Name 405830.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_779,"Datacom","PLMN Name 405831.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_780,"Datacom","PLMN Name 405832.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_781,"Datacom","PLMN Name 405834.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_782,"Datacom","PLMN Name 405835.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_783,"Datacom","PLMN Name 405836.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_784,"Datacom","PLMN Name 405837.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_785,"Datacom","PLMN Name 405838.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_786,"Datacom","PLMN Name 40524.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_787,"Datacom","PLMN Name 405839.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_788,"Datacom","PLMN Name 405840.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_789,"Datacom","PLMN Name 405841.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_790,"Datacom","PLMN Name 405842.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_791,"Datacom","PLMN Name 405833.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_792,"Datacom","PLMN Name 405843.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_793,"Tata Docomo","PLMN Name 405026.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_794,"Tata Docomo","PLMN Name 405033.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_795,"Tata Docomo","PLMN Name 405040.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_796,"S-Tel","PLMN Name 405881.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_797,"S-Tel","PLMN Name 405882.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_798,"S-Tel","PLMN Name 405883.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_799,"S-Tel","PLMN Name 405884.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_800,"S-Tel","PLMN Name 405886.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_801,"S-Tel","PLMN Name 405887.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_802,"Etisalat DB","PLMN Name 405913.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_803,"Etisalat DB","PLMN Name 405912.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_804,"Etisalat DB","PLMN Name 405930.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_805,"Etisalat DB","PLMN Name 405914.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_806,"Etisalat DB","PLMN Name 405915.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_807,"Etisalat DB","PLMN Name 405916.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_808,"Etisalat DB","PLMN Name 405917.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_809,"Etisalat DB","PLMN Name 405931.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_810,"Etisalat DB","PLMN Name 405918.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_811,"Etisalat DB","PLMN Name 405919.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_812,"Etisalat DB","PLMN Name 405920.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_813,"Etisalat DB","PLMN Name 405921.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_814,"Etisalat DB","PLMN Name 405922.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_815,"Etisalat DB","PLMN Name 405923.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_816,"Etisalat DB","PLMN Name 405924.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_817,"Etisalat DB","PLMN Name 40580.");
   ADD_APPLICATION_STRING2(STR_PLMN_NAME_818,"Videocon","PLMN Name 405932.");

}

#endif


