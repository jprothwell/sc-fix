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
 *	SecuritySetup.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Security Setup related functions
 *
 * Author:
 * -------
 * -------
 *
*==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/


#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"
#include "mainmenudef.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "callsdefs.h"
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
#ifdef MMI_ON_WIN32
#include "nvram_data_items.h"
#endif
#include "commonscreens.h"
#include "mmi_features.h"
#include "callsetupenum.h"
#include "settingprot.h"
#include "callmanagementiddef.h"
#include "networksetupgprot.h"
#include "organizergprot.h"
#include "idleappdef.h"
#include "simdetectiongexdcl.h"
#include "settingsgexdcl.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "phonebookgprot.h"
//micha0915
#include "callmanagementgprot.h"
#include "securitysetup.h"
#include "debuginitdef.h"
#undef __NEWSIMULATOR

#include "mmiapi.h"
#include "mmi_trace.h"
#include "satdefs.h"
#include "simdetectionhwmapfile.h"//maning
#include "stack_config.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"//maning
#endif

#include "adp_events.h"

//added by zhoumn for PhoneLock @2007/3/27>>>start
U8 g_SimSuccessIndBuffered[MMI_SIM_NUMBER] = {0,};
extern U8 g_MyPrevSimStatus;
BOOL gSimReady[MMI_SIM_NUMBER] = {FALSE,};

extern BOOL g_FixedInit[]; //Vivian add for FDL
extern void SetMsgFlag(message_type msgFlag);
extern	MMISMUPASSWDREQIND gSMUPwdDataStruct[MMI_SIM_NUMBER];
extern	MMISMUFAILIND gSMUFailIndStruct[MMI_SIM_NUMBER];
SecuritySetupContext g_SecuritySetupContext;

U8 gSecSetupMenuSimID = 0;

/**************************************************************

	FUNCTION NAME		: InitSecuritySetupApp(void)

  	PURPOSE				: Initialize Security

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightBlackListNum(void);

/**************************************************************
	FUNCTION NAME		: WriteDefaultPhoneLockPwd
	PURPOSE			    : Write the phoneLock password to NVRAM.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	Author				: Panxu created, Man and Zhangxb modified.
 
**************************************************************/
static void WriteDefPhnLockPwd(void)
{
	char pStr[MAX_SIM_SETTING_PASSWD_LEN + 1] = "1234\0";
	INT32   ret = 0;
	UINT16 error = 0;
	char pTempBuff[MAX_SIM_SETTING_PASSWD_LEN + 1] = {0};
	
	ret =  ReadRecord(NVRAM_PHONELOCK_NVRAM_LID, 1, (void *)pTempBuff, (MAX_SIM_SETTING_PASSWD_LEN + 1), &error);

	if (ret <= 0)
		WriteRecord(NVRAM_PHONELOCK_NVRAM_LID, 1, (void *)pStr, MAX_SIM_SETTING_PASSWD_LEN + 1, &error);
}

/**************************************************************

	FUNCTION NAME		: ReadPhoneLockSetting

  	PURPOSE				: Load the status of phone lock from NVRAM.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: MMI_BOOL

	Author				: Zhangxb

 

**************************************************************/
MMI_BOOL ReadPhoneLockSetting(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_PHONELOCK, &data, DS_BYTE , &error);
	g_SecuritySetupContext.PhoneLockStatus = data;

	if(data == 1)
		return MMI_TRUE;
	else if(data == 0xff)
	{
		data = 0;
		WriteValue( NVRAM_SETTING_PHONELOCK, &data, DS_BYTE , &error);
		return MMI_FALSE;
	}
	else
		return MMI_FALSE;
}

#ifdef __MMI_VIP_FUNCTION__
extern void InitVIPFunction(void);
#endif
void InitSecuritySetupApp(void)
{
	U8 i;
	SetHiliteHandler(MENU9101_SECURITY, HighlightSecurity);
	SetHiliteHandler(MENU9101_SECURITY_NO_SIM, HighlightSecurity);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MENU9167_FIXED_DIAL + i, HighlightFixedDial);
		SetHintHandler(MENU9167_FIXED_DIAL + i, HintFdl);
		SetHiliteHandler(MENU9168_BARRED_DAIL + i, HighlightBarredDial);
		SetHintHandler(MENU9168_BARRED_DAIL + i,HintBdl);
		SetHiliteHandler(MENU9169_SIM_LOCK + i, HighlightSIMLock);
		SetHintHandler(MENU9169_SIM_LOCK + i, HintSimLock);
		SetHiliteHandler(MENU9174_CP_PIN1 + i,HighlightPin1Passwd);
		SetHiliteHandler(MENU9175_CP_PIN2 + i,HighlightPin2Passwd);
	}

	SetHiliteHandler(MENU_SETTING_FDL_MODE,HighlightFdlMode);
	SetHiliteHandler(MENU_SETTING_FDL_LIST,HighlightFdlList);

	SetHiliteHandler(MENU_SETTING_BDL_MODE,HighlightBdlMode);
	SetHiliteHandler(MENU_SETTING_BDL_LIST,HighlightBdlList);

	SetHintHandler(MENU_SETTING_FDL_MODE,HintFdlMode);
	SetHintHandler(MENU_SETTING_BDL_MODE,HintBdlMode);


	SetHiliteHandler(MENU9165_PHONE_LOCK, HighlightPhoneLock);

#ifdef __MMI_AUTO_KEYPAD_LOCK__ 
	SetHiliteHandler(MENU9178_AKPL_TIME, HighlightAutoKeyPadLock);
#endif

	SetHiliteHandler(MENU9170_CHANGE_PASSWORD,  HighlightChangePassword);

	SetHiliteHandler(MENU9170_CHANGE_PASSWORD_NO_SIM,  HighlightChangePassword);

	SetHiliteHandler(MENU9173_CP_PHONE_LOCK,HighlightPhonePasswd);
	SetHiliteHandler(MENU_SECURITY_SETTING_BLACK_LIST ,HighlightBlackListNum);

	SetHintHandler(MENU9165_PHONE_LOCK,HintPhoneLock);
#ifdef __MMI_AUTO_KEYPAD_LOCK__
	SetHintHandler(MENU9178_AKPL_TIME,HintAutoKeypadLock);
#endif

	g_SecuritySetupContext.SMULockSuccessCallBack = NULL;
	g_SecuritySetupContext.SMULockFailCallBack = NULL;	
	g_SecuritySetupContext.VerifyPassSuccessCallBack = NULL;

	//add by panxu for init the phone lock
	WriteDefPhnLockPwd();

	#ifdef __MMI_VIP_FUNCTION__
	InitVIPFunction();
	#endif
}

/*gaosh Add Start For rstdefault Ver: TBM780_R_V1.0.7_20071129_1740  on 2007-12-7 17:8 */
BOOL PhoneLockRstDefault(void)
{
	S16 error1,error2;
	U8 data_OFF 	= 0;
	char pStr[MAX_SIM_SETTING_PASSWD_LEN + 1] = "1234\0";
	
	WriteValue( NVRAM_SETTING_PHONELOCK, &data_OFF, DS_BYTE , &error1);
	WriteRecord(NVRAM_PHONELOCK_NVRAM_LID, 1, (void *)pStr, MAX_SIM_SETTING_PASSWD_LEN + 1, &error2);
	
	if((error1 == NVRAM_WRITE_SUCCESS)&&(error2 == NVRAM_WRITE_SUCCESS))
		return TRUE;
	else
		return FALSE;
}
/*gaosh Add End  For rstdefault Ver: TBM780_R_V1.0.7_20071129_1740  on 2007-12-7 17:8 */


/**************************************************************

	FUNCTION NAME		: HighlightSecurity(void)

  	PURPOSE				: HighLight Security-Setup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightSecurity(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntrySECSETMain,KEY_EVENT_UP);

	SetKeyHandler(EntrySECSETMain, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: HighlightSIMLock(void)

  	PURPOSE				: Highlight fn for SimLock Option

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightSIMLock(void)
{
	if(g_SecuritySetupContext.SimLockStatus[gSecSetupMenuSimID]  != CHV_DISABLED)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetLeftSoftkeyFunction(EntrySECSETSIMLockPIN1,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntrySECSETSIMLockPIN1,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: HighlightPhoneLock(void)

  	PURPOSE				: highlight phone lock option in security setup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightPhoneLock(void)
{
	if (IsPhoneLock())
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}

	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetLeftSoftkeyFunction(EntrySECSETPhoneLock,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntrySECSETPhoneLock,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: HighlightAutoKeyPadLock(void)

  	PURPOSE				: Highlight handler for Autokeypadlock.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/
#ifdef __MMI_AUTO_KEYPAD_LOCK__
void HighlightAutoKeyPadLock(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntrySECSETAKPLTime,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntrySECSETAKPLTime, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif


/**************************************************************

	FUNCTION NAME		: HighlightFixedDial(void)

  	PURPOSE				: Highlight handler for fixed dial.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightFixedDial(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */

	SetLeftSoftkeyFunction(SECSETGetFdlStatusReq,KEY_EVENT_UP);
	SetKeyHandler(SECSETGetFdlStatusReq, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HighlightFdlMode(void)

  	PURPOSE				: Highlight handler for FDL mode

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void HighlightFdlMode(void)
{

	if(g_SecuritySetupContext.FdlStatus[gSecSetupMenuSimID])
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}

	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	g_SecuritySetupContext.VerifyPassSuccessCallBack = SECSETSetFdlModeReq;
	SetLeftSoftkeyFunction(SECSETGetPIN2StatusReq,KEY_EVENT_UP);
	SetKeyHandler(SECSETGetPIN2StatusReq, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: HighlightFdlList(void)

  	PURPOSE				: Highlight handler for FDL list

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightFdlList(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	ChangeLeftSoftkey(STR_GLOBAL_OK,0);

	SetLeftSoftkeyFunction(EntryFdlList,KEY_EVENT_UP);
	SetKeyHandler(EntryFdlList, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: HighlightBarredDial(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightBarredDial(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(SECSETGetBdlStatusReq,KEY_EVENT_UP);
	SetKeyHandler(SECSETGetBdlStatusReq, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HighlightBdlMode(void)

  	PURPOSE				: Highlight handler for BDL mode.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightBdlMode(void)
{

	if(g_SecuritySetupContext.BdlStatus[gSecSetupMenuSimID])
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}

	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	g_SecuritySetupContext.VerifyPassSuccessCallBack = SECSETSetBdlModeReq;
	SetLeftSoftkeyFunction(SECSETGetPIN2StatusReq,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(SECSETGetPIN2StatusReq, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: HighlightBdlList(void)

  	PURPOSE				: Highlight handler for BDL list

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightBdlList(void)
{
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	ChangeLeftSoftkey(STR_GLOBAL_OK,0);

	SetLeftSoftkeyFunction(EntryBdlList,KEY_EVENT_UP);
	SetKeyHandler(EntryBdlList, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: HighlightChangePassword(void)

  	PURPOSE				: Highlight handler for Change password option.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightChangePassword(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntrySECSETChangePassword,KEY_EVENT_UP);
	SetKeyHandler(EntrySECSETChangePassword, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HighlightPin1Passwd(void)

  	PURPOSE				: Highlight handler for Change Pin1 option in change pwd screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightPin1Passwd(void)
{
	g_SecuritySetupContext.CurrentPassword =0;
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	SetKeyHandler(SECSETGetPIN1ForChangeReq, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(SECSETGetPIN1ForChangeReq,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HighlightPin2Passwd(void)

  	PURPOSE				: Highlight handler for Change Pin2 option in change pwd screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightPin2Passwd(void)
{
	g_SecuritySetupContext.CurrentPassword =1;
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
//micha0529
	SetKeyHandler(SECSETGetPIN2ForChangeReq, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	g_SecuritySetupContext.VerifyPassSuccessCallBack = ChangePasswordPIN2SuccessCallBack;
	SetLeftSoftkeyFunction(SECSETGetPIN2ForChangeReq,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HighlightPhonePasswd(void)

  	PURPOSE				: Highlight handler for Change phone lock option in change pwd screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightPhonePasswd(void)
{
	g_SecuritySetupContext.CurrentPassword =2;
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
//micha0529
	SetKeyHandler(EntrySECSETChangePWOldPWD, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntrySECSETChangePWOldPWD,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************

	FUNCTION NAME		: HintSimLock(void)

  	PURPOSE				: Hint Hnadler for Sim Lock.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintSimLock(U16 index)
{
	S32 hiliteItemID = 0;
	U8 offset = 0;
	
	hiliteItemID = GetSeqItemId_Ext((U16)MENU9101_SECURITY, (S16)index);
	offset = hiliteItemID - MENU9169_SIM_LOCK;
	
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(g_SecuritySetupContext.SimLockStatus[offset] == CHV_DISABLED)
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
	}
}

/**************************************************************

	FUNCTION NAME		: HintPhoneLock(void)

  	PURPOSE				: Hint handler for Phone lock.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintPhoneLock(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(IsPhoneLock())
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
	}
}


/**************************************************************

	FUNCTION NAME		: HintAutoKeypadLock(void)

  	PURPOSE				: Hint handler for AutoKeypad lock.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifdef __MMI_AUTO_KEYPAD_LOCK__ 
void HintAutoKeypadLock(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(0 != GetAutoKeypadLockTime())
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
	}
}
#endif


/**************************************************************

	FUNCTION NAME		: HintFdl(void)

  	PURPOSE				: Hint handler for FDL.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintFdl(U16 index)
{
	S32 hiliteItemID = 0;
	U8 offset = 0;
	
	hiliteItemID = GetSeqItemId_Ext((U16)MENU9101_SECURITY, (S16)index);
	offset = hiliteItemID - MENU9167_FIXED_DIAL;
		
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(g_SecuritySetupContext.FdlStatus[offset])
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
	}
}

/**************************************************************

	FUNCTION NAME		: HintBdl(void)

  	PURPOSE				: Hint handler for BDL

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintBdl(U16 index)
{
	S32 hiliteItemID = 0;
	U8 offset = 0;
	
	hiliteItemID = GetSeqItemId_Ext((U16)MENU9101_SECURITY, (S16)index);
	offset = hiliteItemID - MENU9168_BARRED_DAIL;

	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(g_SecuritySetupContext.BdlStatus[offset])
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
	}
}

/**************************************************************

	FUNCTION NAME		: HintFdlMode(void)

  	PURPOSE				: Hint handler for FDL mode.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintFdlMode(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);

	if(g_SecuritySetupContext.FdlStatus[gSecSetupMenuSimID])
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
	}
}

/**************************************************************

	FUNCTION NAME		: HintBdlMode(void)

  	PURPOSE				: Hint handler for BDL Mode.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintBdlMode(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(g_SecuritySetupContext.BdlStatus[gSecSetupMenuSimID])
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
	}
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETMain(void)

  	PURPOSE				: enter security-setup menu screen 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
BOOL gSecuritySetting = 0;

void SECSETMenuHighLightHandler(S32 hiliteid)
{
	S32 hiliteItemID = 0;
	//U8 offset = 0;
	
	hiliteItemID = GetSeqItemId_Ext((U16)MENU9101_SECURITY, (S16)hiliteid);
	
	//offset = hiliteItemID - MENU9167_FIXED_DIAL;
	
	//gSecSetupMenuSimID = offset%MMI_SIM_NUMBER;
	switch(hiliteItemID)
	{
		case MENU9167_FIXED_DIAL:
		case MENU9168_BARRED_DAIL:
		case MENU9169_SIM_LOCK:
			gSecSetupMenuSimID = 0;
			break;
		case MENU_SIM2_FIXED_DIAL:
		case MENU_SIM2_BARRED_DAIL:
		case MENU_SIM2_LOCK:
			gSecSetupMenuSimID = 1;
			break;
		case MENU_SIM3_FIXED_DIAL:
		case MENU_SIM3_BARRED_DAIL:
		case MENU_SIM3_LOCK:
			gSecSetupMenuSimID = 2;
			break;
		case MENU_SIM4_FIXED_DIAL:
		case MENU_SIM4_BARRED_DAIL:
		case MENU_SIM4_LOCK:
			gSecSetupMenuSimID = 3;
			break;
		default:
			gSecSetupMenuSimID = 0;
			break;
	}
	
	ExecuteCurrHiliteHandler(hiliteid);
}
void EntrySECSETMain(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U8* guiBuffer;       /* Buffer holding history data */
	U8 *SecurityType[MAX_SUB_MENUS];
	#ifdef __MMI_MULTI_SIM__
	U8 i;
	#endif

	EntryNewScreen(SCR_ID_SECSET_MAIN, NULL, EntrySECSETMain, NULL);
	gSecuritySetting = 1;

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_MAIN);	

	if (HasValidSIM())
	{
		#ifdef __MMI_MULTI_SIM__
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			mmi_frm_unhide_menu_item(MENU9169_SIM_LOCK + i);
			mmi_frm_unhide_menu_item(MENU9167_FIXED_DIAL + i);
			mmi_frm_unhide_menu_item(MENU9168_BARRED_DAIL + i);
			if(!MTPNP_PFAL_Is_CardValid(i))
			{
				mmi_frm_hide_menu_item(MENU9169_SIM_LOCK + i);
				mmi_frm_hide_menu_item(MENU9167_FIXED_DIAL + i);
				mmi_frm_hide_menu_item(MENU9168_BARRED_DAIL + i);
			}
		}
		#endif
		
		nNumofItem = GetNumOfChild_Ext(MENU9101_SECURITY);
		GetSequenceStringIds_Ext(MENU9101_SECURITY,nStrItemList);
		SetParentHandler(MENU9101_SECURITY);
		ConstructHintsList(MENU9101_SECURITY, SecurityType);
		
		RegisterHighlightHandler(SECSETMenuHighLightHandler);
	}
	else
	{
		nNumofItem = GetNumOfChild(MENU9101_SECURITY_NO_SIM);
		GetSequenceStringIds_Ext(MENU9101_SECURITY_NO_SIM,nStrItemList);
		SetParentHandler(MENU9101_SECURITY_NO_SIM);
		ConstructHintsList(MENU9101_SECURITY_NO_SIM, SecurityType);
		
		RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	}

	ShowCategory52Screen(  STR_SCR9002_SECURITY_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,(PU16)gIndexIconsImageList,
						  SecurityType,
						  0,0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETPhoneLock(void)

  	PURPOSE				: entry sceen to enter phonelock

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETPhoneLock(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U8* inputBuffer;

	EntryNewScreen(SCR_ID_SECSET_PHONE_LOCK, ExitSECSETPhoneLock, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_PHONE_LOCK);	
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_PHONE_LOCK);
	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.Password, ( S8*)inputBuffer);

	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_RESTORE_PHONE,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.Password,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);
	
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(SECSETSetPhoneLockReq,KEY_POUND,KEY_EVENT_UP);		
	SetLeftSoftkeyFunction(SECSETSetPhoneLockReq,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = SECSETSetPhoneLockReq;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.Password) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}


/**************************************************************

	FUNCTION NAME		: ExitSECSETPhoneLock

  	PURPOSE				: Exit screen for enter phone lock screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETPhoneLock(void)
{
	history_t   Scr; //* Variable to hold the history data
	Scr.scrnID = SCR_ID_SECSET_PHONE_LOCK;
	Scr.entryFuncPtr = EntrySECSETPhoneLock;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.Password);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETAKPLTime(void){}

  	PURPOSE				: Entry screen for Autokeypadlock Time.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifdef __MMI_AUTO_KEYPAD_LOCK__ 
void EntrySECSETAKPLTime(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */

	U8 data1=0;
	S16 error=0,error1=0;

	EntryNewScreen(SCR_ID_SECSET_AKPL_TIME, NULL, EntrySECSETAKPLTime, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_AKPL_TIME);	

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU9178_AKPL_TIME);

	/* 4. Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU9178_AKPL_TIME);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU9178_AKPL_TIME,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU9178_AKPL_TIME);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(AutoKeyPadLockTimeHandler);

	ReadValue( NVRAM_SETTING_AUTOKEYPADLOCK_TIME, &data1, DS_BYTE , &error1);
	if(error != NVRAM_READ_SUCCESS)
	{
	}

	if(data1 == 0xff)
	{
		data1=0;
		WriteValue( NVRAM_SETTING_AUTOKEYPADLOCK_TIME, &data1, DS_BYTE , &error1);
	}
	g_SecuritySetupContext.AutoKeyPadLockTime = data1;
	/* 8 Display Category11 Screen */
	ShowCategory11Screen(  STR_MENU9166_AUTO_KEYPAD_LOCK, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList,(U16)data1, guiBuffer);
	
	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SendAKPLTimeMessageAndShowScreen, KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}
#endif


/**************************************************************

	FUNCTION NAME		: AutoKeyPadLockTimeHandler(void)

  	PURPOSE				: Handler function when user select any of the given
						  options in autokeypad lock time screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/
void AutoKeyPadLockTimeHandler(S32 index)
{
	gCurrentOnOff = (U8)index;  //opened by zhoumn for bug 10121 @2007/03/15
}


/**************************************************************

	FUNCTION NAME		: SendAKPLTimeMessageAndShowScreen(void)

  	PURPOSE				: Set hint handler as On or Off for autokeypad lock.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void SendAKPLTimeMessageAndShowScreen(void)
{
	U8 data;
	S16 error;
	U16 index;
	index = GetCurrHiliteID();

	data = gCurrentOnOff;

	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);

	if(data)
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
		Category52ChangeItemDescription(2,hintData[index]);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
		Category52ChangeItemDescription(2,hintData[index]);
	}
	RedrawCategoryFunction();
	if(g_SecuritySetupContext.AutoKeyPadLockTime!=gCurrentOnOff)
	{
		WriteValue( NVRAM_SETTING_AUTOKEYPADLOCK_TIME, &data, DS_BYTE , &error);
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		DeleteNHistory(1);
	}
	else {
		GoBackHistory();
	}
}

#ifdef __MMI_MULTI_SIM__
void SECSETSetSIMIdforPIN(U8 simid)
{
	gSecSetupMenuSimID = simid;
}
#endif


/**************************************************************

	FUNCTION NAME		: SECSETGetPIN2StatusReq(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/
extern BOOL is_phb_init_Finishi(void);
void SECSETGetPIN2StatusReq(void)
{
	MYQUEUE msgStruct;
	module_type dest_mod_id = MOD_L4C + gSecSetupMenuSimID;

	if(gSimReady[gSecSetupMenuSimID] == FALSE)//man for sim init
	{
		DisplayPopup((PU8)GetString(SAT_WAIT_STRING_ID), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
		return;	
	}
	ClearAllKeyHandler();
	SetMsgFlag(PHB_CHECK_PIN2);//man
	SetProtocolEventHandlerExt(SECSETGetPIN2StatusRsp, PRT_MMI_SMU_CHECK_PIN_STATUS_RSP, gSecSetupMenuSimID);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = dest_mod_id;
	msgStruct.oslMsgId = PRT_MMI_SMU_CHECK_PIN_STATUS_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);


}

/**************************************************************

	FUNCTION NAME		: SECSETGetPIN2StatusRsp(void)

  	PURPOSE				: Response of request to get the status of PIN2.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/
void SECSETGetPIN2StatusRsp(void *info)
{

	SMUSTATUSRSP *resp;
	historyNode *History;
	U8 nsimID;
	resp = (SMUSTATUSRSP *)(info);
	nsimID = MTPNP_AD_get_protocol_event_src_mode();
	
	FillCountDownNumber(resp->chv_info,nsimID);
	if(resp->chv_status.chv2_status == CHV_BLOCKED)		
	{
		DisplayPopup((PU8)GetString(STR_SETTING_PIN2_BLK2) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		ExitSECSETPUK2();
		
		 if(GetHistoryScrID(SCR_ID_SECSET_PUK2,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
		 DeleteUptoScrID(SCR_ID_SECSET_PUK2);
	}
	else if(resp->chv_status.chv2_status == CHV_RETRY_ALLOW ||resp->chv_status.chv2_status == CHV_VERIFIED)
	{
		EntrySECSETPIN2();
	}
	else if(resp->chv_status.chv2_status == CHV_INVALID_TYPE)		
	{
		DisplayPopup((PU8)GetString(STR_ID_PIN2_NOT_SUPPORT) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
	}
	else if((resp->chv_status.chv2_status == CHV_DEAD) || (resp->chv_status.chv2_status == CHV_NOTINITED))
	{
		DisplayPopup((PU8)GetString(STR_ID_PIN2_NOT_SUPPORT) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
	}
	
}

/**************************************************************

	FUNCTION NAME		: SECSETGetPIN1StatusReq(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/
void SECSETGetPIN1StatusReq(void)
{
	MYQUEUE msgStruct;

	if(!gSimReady[gSecSetupMenuSimID])
	{
		DisplayPopup((PU8)GetString(SAT_WAIT_STRING_ID), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
		return;	
	}
	
	SetMsgFlag(CHECK_PIN1);

	SetProtocolEventHandlerExt(SECSETGetPIN1StatusRsp, PRT_MMI_SMU_CHECK_PIN_STATUS_RSP,gSecSetupMenuSimID);

	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C + gSecSetupMenuSimID;
	msgStruct.oslMsgId = PRT_MMI_SMU_CHECK_PIN_STATUS_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr= NULL;
	
	OslMsgSendExtQueue(&msgStruct);
}

/**************************************************************

	FUNCTION NAME		: SECSETGetPIN1StatusRsp(void)

  	PURPOSE				: Response of request to get the status of PIN1.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/
void SECSETGetPIN1StatusRsp(void *info)
{
	SMUSTATUSRSP *resp;
	U8 nSimID;
	
	resp = (SMUSTATUSRSP *)(info);
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	FillCountDownNumber(resp->chv_info, nSimID);
	g_SecuritySetupContext.SimLockStatus[nSimID] = resp->chv_status.chv1_status;
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETPIN2(void)

  	PURPOSE				: Entry fn to enter PIN2 for FDL.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETPIN2(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U8* inputBuffer;
	U8 PassCountdownDisp[MAX_PASS_COUNTDOWN_DISP];

	EntryNewScreen(SCR_ID_SECSET_PIN2, ExitSECSETPIN2, NULL, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_PIN2);	
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_PIN2);
	if(inputBuffer == NULL) 
		memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.Password, ( S8*)inputBuffer);		
	
	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	FillCountDownString(MMI_SETTING_TYPE_CHV2, PassCountdownDisp,gSecSetupMenuSimID);
	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SCR9027_PIN2_CAPTION,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.Password,
							(MAX_SIM_SETTING_PASSWD_LEN+1) , PassCountdownDisp, guiBuffer);

	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(VerifyPin2,KEY_POUND,KEY_EVENT_UP);	
	SetLeftSoftkeyFunction(VerifyPin2,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = VerifyPin2;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.Password) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETPIN2(void)

  	PURPOSE				: Exit Fixed dial menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETPIN2(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_PIN2;
	Scr.entryFuncPtr = EntrySECSETPIN2;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.Password);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}
/**************************************************************

	FUNCTION NAME		: VerifyPin2(void)

  	PURPOSE				: Verify Pin2.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void VerifyPin2(void)
{
	U8 tempBuffer[(MAX_SIM_SETTING_PASSWD_LEN+1)];

	memset(tempBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1));

	SECSETVerifyPINReq(MMI_SETTING_TYPE_CHV2, g_SecuritySetupContext.Password,  tempBuffer, NULL);
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETFixDial(void)

  	PURPOSE				: Entry screen for FDL option screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETFixDial(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 *FdlType[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_SECSET_FIX_DIAL, NULL, EntrySECSETFixDial, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_FIX_DIAL);	

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU9167_FIXED_DIAL);

	/* 4. Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU9167_FIXED_DIAL);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU9167_FIXED_DIAL,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU9167_FIXED_DIAL);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	/* 8 Construct the Hints List */
	ConstructHintsList(MENU9167_FIXED_DIAL, FdlType);

	/* 8 Display Category52 Screen */
	ShowCategory52Screen(  STR_MENU9167_FIXED_DIAL, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						    nNumofItem, nStrItemList,(PU16)gIndexIconsImageList,
							FdlType,
							0,0, guiBuffer);
						

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETBarDial(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETBarDial(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 *BdlType[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_SECSET_BAR_DIAL, NULL, EntrySECSETBarDial, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_BAR_DIAL);	
	
	 /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU9168_BARRED_DAIL);

	/* 4. Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU9168_BARRED_DAIL);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU9168_BARRED_DAIL,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU9168_BARRED_DAIL);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ConstructHintsList(MENU9168_BARRED_DAIL, BdlType);

	/* 8 Display Category52 Screen */
	ShowCategory52Screen(  STR_MENU9168_BARRED_DAIL,MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						    nNumofItem, nStrItemList,(PU16)gIndexIconsImageList,
							BdlType,
							0,0, guiBuffer);
	
	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: SECSETSetSIMLockReq(void)

  	PURPOSE				: A general fn to make Req for all type of Security options.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETSetSIMLockReq(void)
{
	MYQUEUE msgStruct;
	MMISMULOCKREQSTRUCT *smuLockReq;
	S8 pOutBuffer[(MAX_SIM_SETTING_PASSWD_LEN+1)+1];
	
	TRACE_SMU_FUNCTION();
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);
	memset(pOutBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1)+1);

	smuLockReq = OslConstructDataPtr(sizeof (MMISMULOCKREQSTRUCT));

	if(g_SecuritySetupContext.SimLockStatus[gSecSetupMenuSimID] != CHV_DISABLED)
	{
		smuLockReq->opcode = MMI_DISABLE;
	}
	else
	{
		smuLockReq->opcode = MMI_ENABLE;
	}

	smuLockReq->type = (U8)MMI_SETTING_TYPE_CHV1;

	memset(smuLockReq->passwd,0,(MAX_SIM_SETTING_PASSWD_LEN+1));

	UnicodeToAnsii(pOutBuffer,(PS8) g_SecuritySetupContext.Password );
	memcpy(smuLockReq->passwd, pOutBuffer,(MAX_SIM_SETTING_PASSWD_LEN+1));

	g_SecuritySetupContext.SMULockSuccessCallBack = SECSETSetSIMLockSuccess;
	g_SecuritySetupContext.SMULockFailCallBack = SECSETSetSIMLockFail;
	SetProtocolEventHandlerExt(SECSETSMULockRsp, PRT_MMI_SMU_LOCK_RSP,gSecSetupMenuSimID);

	TRACE_EF(g_sw_SMU, "the type = %d, the opcode = %d", smuLockReq->type, smuLockReq->opcode);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C + gSecSetupMenuSimID;
//micha1229
	msgStruct.oslMsgId = PRT_MMI_SMU_LOCK_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = (oslParaType *)smuLockReq;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);
}

/**************************************************************

	FUNCTION NAME		: SECSETSetSIMLockSuccess(void)

  	PURPOSE				: A general fn to make Req for all type of Security options.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETSetSIMLockSuccess(void)
{
	U16 index;
	U8 nSimIndex;
	
	index = GetCurrHiliteID();
	
	nSimIndex = MTPNP_AD_get_protocol_event_src_mode();
	
	if(g_SecuritySetupContext.SimLockStatus[nSimIndex] != CHV_DISABLED)
	{
		g_SecuritySetupContext.SimLockStatus[nSimIndex] = CHV_DISABLED;
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
		Category52ChangeItemDescription(0,hintData[index]);
		change_left_softkey(STR_GLOBAL_ON,0);
		DisplayPopup((PU8)GetString(STR_SETTING_SIM_UNLOCK) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
	}
	else
	{
		g_SecuritySetupContext.SimLockStatus[nSimIndex] = CHV_VERIFIED;
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
		Category52ChangeItemDescription(0,hintData[index]);
		change_left_softkey(STR_GLOBAL_OFF,0);
		DisplayPopup((PU8)GetString(STR_SETTING_SIM_LOCK) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
	}
	DeleteNHistory(1);
}

/**************************************************************

	FUNCTION NAME		: SECSETSetSIMLockFail(void)

  	PURPOSE				: A general fn to make Req for all type of Security options.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETSetSIMLockFail(void)
{
	historyNode *History;
	DisplayPopup((PU8)GetString(STR_WRONG_PIN) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
	if(GetHistoryScrID(SCR_ID_SECSET_SIM_LOCK_PIN1,&History)==ST_SUCCESS)
	{
		if(History->inputBuffer!=NULL)
			memset(History->inputBuffer,0,ENCODING_LENGTH);
		if(History->guiBuffer!=NULL)
			memset(History->guiBuffer,0,2);
	}	
}



/**************************************************************

	FUNCTION NAME		: SECSETSetPhoneLockReq(void)

  	PURPOSE				: A general fn to make Req for all type of Security options.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETSetPhoneLockReq(void)
{
	
	MYQUEUE msgStruct;
	MMISMULOCKREQSTRUCT *smuLockReq;
	S8 pOutBuffer[(MAX_SIM_SETTING_PASSWD_LEN+1)+1];
	
	TRACE_SMU_FUNCTION();
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);
	memset(pOutBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1)+1);

	smuLockReq = OslConstructDataPtr(sizeof (MMISMULOCKREQSTRUCT));

	if(IsPhoneLock())
	{
		smuLockReq->opcode = MMI_DISABLE;
	}
	else
	{
		smuLockReq->opcode = MMI_ENABLE;
	}
	
	smuLockReq->type = (U8)MMI_SETTING_TYPE_PHONELOCK;
	memset(smuLockReq->passwd,0,(MAX_SIM_SETTING_PASSWD_LEN+1));	
	UnicodeToAnsii(pOutBuffer,(PS8) g_SecuritySetupContext.Password );
	memcpy(smuLockReq->passwd, pOutBuffer,(MAX_SIM_SETTING_PASSWD_LEN+1));

	g_SecuritySetupContext.SMULockSuccessCallBack = SECSETSetPhoneLockSuccess;
	g_SecuritySetupContext.SMULockFailCallBack = SETSETSetPhoneLockFail;
	SetProtocolEventHandler(SECSETSMULockRsp, PRT_MMI_SMU_LOCK_RSP);

	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C;
//micha1229
	msgStruct.oslMsgId = PRT_MMI_SMU_LOCK_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = (oslParaType *)smuLockReq;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);
}


/**************************************************************

	FUNCTION NAME		: SECSETSetPhoneLockSuccess(void)

  	PURPOSE				: A general fn to make Req for all type of Security options.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETSetPhoneLockSuccess(void)
{
	U16 index;
	U8 data;
	S16 error = 0;

	index = GetCurrHiliteID();

	g_SecuritySetupContext.PhoneLockStatus = !g_SecuritySetupContext.PhoneLockStatus;
	data = g_SecuritySetupContext.PhoneLockStatus;
	WriteValue( NVRAM_SETTING_PHONELOCK, &data, DS_BYTE , &error);

	if(g_SecuritySetupContext.PhoneLockStatus == 0)
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
		Category52ChangeItemDescription(1,hintData[index]);
		change_left_softkey(STR_GLOBAL_ON,0);
		DisplayPopup((PU8)GetString(STR_SETTING_PHONE_UNLOCK) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
		Category52ChangeItemDescription(1,hintData[index]);
		change_left_softkey(STR_GLOBAL_OFF,0);
		DisplayPopup((PU8)GetString(STR_SETTING_PHONE_LOCK) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
	}
	DeleteScreenIfPresent(SCR_ID_SECSET_PHONE_LOCK);
}

/**************************************************************

	FUNCTION NAME		: SECSETSetPhoneLockFail(void)

  	PURPOSE				: A general fn to make Req for all type of Security options.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SETSETSetPhoneLockFail(void)
{
	historyNode *History;
	DisplayPopup((PU8)GetString(STR_SETTING_WRONG) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
	if(GetHistoryScrID(SCR_ID_SECSET_PHONE_LOCK,&History)==ST_SUCCESS)
	{
		if(History->inputBuffer!=NULL)
			memset(History->inputBuffer,0,ENCODING_LENGTH);
		if(History->guiBuffer!=NULL)
			memset(History->guiBuffer,0,2);
	}	
}

/**************************************************************

	FUNCTION NAME		: SECSETSMULockRsp(void *info) 

  	PURPOSE				: A general fn to make Req for all type of Security options.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void SECSETSMULockRsp(void *info) 
{
	historyNode *History;
	SMULOCKRSP *resp;
	U8 nSimID;
	
	ASSERT(info != NULL);
	TRACE_SMU_FUNCTION();
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	resp = (SMULOCKRSP *)(info);
	if (resp->cause != MMI_SETTING_TYPE_PHONELOCK)//man begin 
	{
		FillCountDownNumber(resp->chv_info,nSimID);
	}//man end
	
	
	if(resp->result == 1) {
		if (g_SecuritySetupContext.SMULockSuccessCallBack != NULL) {
			(*g_SecuritySetupContext.SMULockSuccessCallBack)();
			g_SecuritySetupContext.SMULockSuccessCallBack = NULL;
		}
	}
	else if(resp->result == 0 && resp->cause == MMI_SETTING_TYPE_UBCHV1)
	{
		ExitSECSETPUK1();
		DisplayPopup((PU8)GetString(STR_SETTING_PIN_BLK) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,	ERROR_TONE);
		DeleteUptoScrID(SCR_ID_SECSET_PUK1);

		if(GetHistoryScrID(SCR_ID_SECSET_PUK1,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
		g_SecuritySetupContext.SMULockSuccessCallBack = NULL;
		g_SecuritySetupContext.SMULockFailCallBack = NULL;
	}
	else 
	{
		if (g_SecuritySetupContext.SMULockFailCallBack != NULL) {
			(*g_SecuritySetupContext.SMULockFailCallBack)();
			g_SecuritySetupContext.SMULockFailCallBack = NULL;
		}
	}
	pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.Password,( S8*)"");
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETChangePassword(void)

  	PURPOSE				: Entry fn for change password option menu.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PWDChangeHighLightHandler(S32 hiliteid)
{
	S32 hiliteItemID = 0;
	//U8 offset = 0;
	
	hiliteItemID = GetSeqItemId_Ext((U16)MENU9170_CHANGE_PASSWORD, (S16)hiliteid);
	
	//offset = hiliteItemID - MENU9167_FIXED_DIAL;
	
	//gSecSetupMenuSimID = offset%MMI_SIM_NUMBER;
	switch(hiliteItemID)
	{
		case MENU9174_CP_PIN1:
		case MENU9175_CP_PIN2:
			gSecSetupMenuSimID = 0;
			break;
		case MENU_MTPNP_CP_PIN1:
		case MENU_MTPNP_CP_PIN2:
			gSecSetupMenuSimID = 1;
			break;
		case MENU_MTPNP_CP_SIM3_PIN1:
		case MENU_MTPNP_CP_SIM3_PIN2:
			gSecSetupMenuSimID = 2;
			break;
		case MENU_MTPNP_CP_SIM4_PIN1:
		case MENU_MTPNP_CP_SIM4_PIN2:
			gSecSetupMenuSimID = 3;
			break;
		default:
			gSecSetupMenuSimID = 0;
			break;
	}
	
	ExecuteCurrHiliteHandler(hiliteid);
}
void EntrySECSETChangePassword(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U8* guiBuffer;       /* Buffer holding history data */
	
	#ifdef __MMI_MULTI_SIM__
	U8 i;
	#endif

	EntryNewScreen(SCR_ID_SECSET_CHANGE_PASSWORD, NULL, EntrySECSETChangePassword, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_CHANGE_PASSWORD);	

	 if( HasValidSIM())
	 {
		#ifdef __MMI_MULTI_SIM__
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			mmi_frm_unhide_menu_item(MENU9174_CP_PIN1 + i);
			mmi_frm_unhide_menu_item(MENU9175_CP_PIN2 + i);
			if(MTPNP_PFAL_Is_CardAbsent(i))
			{
				mmi_frm_hide_menu_item(MENU9174_CP_PIN1 + i);
				mmi_frm_hide_menu_item(MENU9175_CP_PIN2 + i);
			}
		}
		#endif
		nNumofItem = GetNumOfChild_Ext(MENU9170_CHANGE_PASSWORD);
		GetSequenceStringIds_Ext(MENU9170_CHANGE_PASSWORD,nStrItemList);
		SetParentHandler(MENU9170_CHANGE_PASSWORD);
		RegisterHighlightHandler(PWDChangeHighLightHandler);
	}
	else
	{
		nNumofItem = GetNumOfChild(MENU9170_CHANGE_PASSWORD_NO_SIM);
		GetSequenceStringIds(MENU9170_CHANGE_PASSWORD_NO_SIM,nStrItemList);
		SetParentHandler(MENU9170_CHANGE_PASSWORD_NO_SIM);
		RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	}

	ShowCategory15Screen(STR_SCR9026_CHANGE_PASSWORD_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList,(PU16)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: SECSETGetPIN1ForChangeReq(void)

  	PURPOSE				: To get the status of Pin1/Pin2.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETGetPIN1ForChangeReq(void)
{
	MYQUEUE msgStruct;
	
	TRACE_SMU_FUNCTION();

	if(gSimReady[gSecSetupMenuSimID] == FALSE)
	{
		DisplayPopup((PU8)GetString(SAT_WAIT_STRING_ID), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
		return;	
	}

	SetMsgFlag(CHECK_PIN1);
	

	ClearAllKeyHandler();
	
	SetProtocolEventHandlerExt(SECSETGetPIN1ForChangeRsp, PRT_MMI_SMU_CHECK_PIN_STATUS_RSP,gSecSetupMenuSimID);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C + gSecSetupMenuSimID;

	msgStruct.oslMsgId = PRT_MMI_SMU_CHECK_PIN_STATUS_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);
}

/**************************************************************

	FUNCTION NAME		: SECSETGetPIN2ForChangeReq(void)

  	PURPOSE				: To get the status of Pin1/Pin2.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETGetPIN2ForChangeReq(void)
{
	MYQUEUE msgStruct;
	if(gSimReady[gSecSetupMenuSimID] == FALSE)//man for sim init
	{
		DisplayPopup((PU8)GetString(SAT_WAIT_STRING_ID), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
		return;	
	}
	SetMsgFlag(PHB_CHECK_PIN2);//man
	ClearAllKeyHandler();
	
	SetProtocolEventHandlerExt(SECSETGetPIN2ForChangeRsp, PRT_MMI_SMU_CHECK_PIN_STATUS_RSP, gSecSetupMenuSimID);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C + gSecSetupMenuSimID;
	msgStruct.oslMsgId = PRT_MMI_SMU_CHECK_PIN_STATUS_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);
}

/**************************************************************

	FUNCTION NAME		: SECSETGetPIN1ForChangeRsp(void)

  	PURPOSE				: Handles response of req to get the status of PIN1/PIN2.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETGetPIN1ForChangeRsp(void *info)
{
	
	SMUSTATUSRSP *resp;
	U8 nSimID;
	
	TRACE_SMU_FUNCTION();
	resp = (SMUSTATUSRSP *)(info);

	nSimID = MTPNP_AD_get_protocol_event_src_mode();

	FillCountDownNumber(resp->chv_info,nSimID);

	switch (resp->chv_status.chv1_status)
	{
	case CHV_DISABLED:
		DisplayPopup((PU8)GetString(STR_ENABLE_PIN) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		break;
	default:
		EntrySECSETChangePWOldPWD();
	break;
	}
}

/**************************************************************

	FUNCTION NAME		: SECSETGetPIN2ForChangeRsp(void)

  	PURPOSE				: Handles response of req to get the status of PIN1/PIN2.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETGetPIN2ForChangeRsp(void *info)
{
	SMUSTATUSRSP *resp;
	historyNode *History;
	U8 nSimID;
	resp = (SMUSTATUSRSP *)(info);

	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	FillCountDownNumber(resp->chv_info,nSimID);

	switch (resp->chv_status.chv2_status)
	{
	case CHV_BLOCKED:
		DisplayPopup((PU8)GetString(STR_SETTING_PIN2_BLK2) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		ExitSECSETPUK2();
		if(GetHistoryScrID(SCR_ID_SECSET_PUK2,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
		DeleteUptoScrID(SCR_ID_SECSET_PUK2);
	break;
	case CHV_DEAD:
	case CHV_INVALID_TYPE:
	case CHV_NOTINITED:
	{
		DisplayPopup((PU8)GetString(STR_ID_PIN2_NOT_SUPPORT) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
	}
	break;
	default:
		EntrySECSETChangePWOldPWD();
	break;
	}

}

/**************************************************************

	FUNCTION NAME		: EntrySECSETChangePWOldPWD(void)

  	PURPOSE				: Entry screen to get old pwd of Phonelock/pin1/pin2 from user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETChangePWOldPWD(void)
{
	U8* guiBuffer;
	U8* inputBuffer;
	U8 PassCountdownDisp[MAX_PASS_COUNTDOWN_DISP];	

	EntryNewScreen(SCR_ID_SECSET_CHANGEPW_OLD_PWD, ExitSECSETChangePWOldPWD, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_CHANGEPW_OLD_PWD); 
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_CHANGEPW_OLD_PWD);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.Password,0, (MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.Password, ( S8*)inputBuffer);

	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	switch(g_SecuritySetupContext.CurrentPassword)
	{
	case 0:
		FillCountDownString(MMI_SETTING_TYPE_CHV1, PassCountdownDisp,gSecSetupMenuSimID);
		ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SETTING_OLD_PIN,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.Password,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,PassCountdownDisp, guiBuffer);
	break;
	case 1:
		FillCountDownString(MMI_SETTING_TYPE_CHV2, PassCountdownDisp,gSecSetupMenuSimID);
		ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SETTING_OLD_PIN2,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.Password,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,PassCountdownDisp,guiBuffer);
	break;
	case 2:
			ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SCR9029_CURRENT_PW_CAPTION,
						INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.Password,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);
	break;
	}
	
	SetLeftSoftkeyFunction(EntrySECSETChangePWNewPWD,KEY_EVENT_UP);
	SetKeyHandler(EntrySECSETChangePWNewPWD,KEY_POUND,KEY_EVENT_UP);
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = EntrySECSETChangePWNewPWD;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.Password) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETChangePWOldPWD(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETChangePWOldPWD(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_CHANGEPW_OLD_PWD;
	Scr.entryFuncPtr = EntrySECSETChangePWOldPWD;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.Password);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETChangePWNewPWD(void)

  	PURPOSE				: Entry screen to enter new pwd for all options.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntrySECSETChangePWNewPWD(void)
{
	U8* guiBuffer;
	U8* inputBuffer;

	EntryNewScreen(SCR_ID_SECSET_CHANGEPW_NEW_PWD, ExitSECSETChangePWNewPWD, NULL, NULL);	

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_CHANGEPW_NEW_PWD); 
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_CHANGEPW_NEW_PWD);
	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.NewPassword,0, (MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.NewPassword, ( S8*)inputBuffer);

	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	switch(g_SecuritySetupContext.CurrentPassword)
	{
	case 0:
		ShowCategory111Screen(	STR_GLOBAL_OK , 
								IMG_GLOBAL_OK ,
								STR_GLOBAL_BACK, 
								IMG_GLOBAL_BACK,
								STR_SETTING_NEW_PIN,
								INPUT_TYPE_NUMERIC_PASSWORD,
								g_SecuritySetupContext.NewPassword,
								(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, NULL);
	break;
	case 1:
		ShowCategory111Screen(	STR_GLOBAL_OK , 
								IMG_GLOBAL_OK ,
								STR_GLOBAL_BACK, 
								IMG_GLOBAL_BACK,
								STR_SETTING_NEW_PIN2,
								INPUT_TYPE_NUMERIC_PASSWORD,
								g_SecuritySetupContext.NewPassword,
								(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);
	break;
	case 2:
		ShowCategory111Screen(	STR_GLOBAL_OK , 
								IMG_GLOBAL_OK ,
								STR_GLOBAL_BACK, 
								IMG_GLOBAL_BACK,
								STR_SCR9030_NEW_PW_CAPTION,
								INPUT_TYPE_NUMERIC_PASSWORD,
								g_SecuritySetupContext.NewPassword,
								(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);
	break;
	}
		
	SetLeftSoftkeyFunction(EntrySECSETChangePWCfmPWD,KEY_EVENT_UP);
	SetKeyHandler(EntrySECSETChangePWCfmPWD,KEY_POUND,KEY_EVENT_UP);
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = EntrySECSETChangePWCfmPWD;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.NewPassword) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETChangePWNewPWD

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETChangePWNewPWD(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_CHANGEPW_NEW_PWD;
	Scr.entryFuncPtr = EntrySECSETChangePWNewPWD;

	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.NewPassword);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETChangePWCfmPWD(void)

  	PURPOSE				: Entry screen to confirm new pwd for all options.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETChangePWCfmPWD(void)
{
	U8* guiBuffer;
	U8* inputBuffer;

	EntryNewScreen(SCR_ID_SECSET_CHANGEPW_CFM_PWD, ExitSECSETChangePWCfmPWD, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_CHANGEPW_CFM_PWD);
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_CHANGEPW_CFM_PWD);
	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.ConfirmPassword,0, (MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.ConfirmPassword, ( S8*)inputBuffer);

	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SCR9031_CONFIRM_PW_CAPTION,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.ConfirmPassword,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);
		
	SetLeftSoftkeyFunction(SendReqToChangePwd,KEY_EVENT_UP);
	SetKeyHandler(SendReqToChangePwd,KEY_POUND,KEY_EVENT_UP);
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = SendReqToChangePwd;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.ConfirmPassword) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETChangePWCfmPWD(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETChangePWCfmPWD(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_CHANGEPW_CFM_PWD;
	Scr.entryFuncPtr = EntrySECSETChangePWCfmPWD;

	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.ConfirmPassword);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

/**************************************************************

	FUNCTION NAME		: SendReqToChangePwd(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SendReqToChangePwd(void)
{
	MYQUEUE msgStruct;
	PASSWORDREQ *passWord;
	historyNode *History;

	S8 pOutBuffer[(MAX_SIM_SETTING_PASSWD_LEN+1)+1];
	memset(pOutBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1)+1);

//micha0716
//	passWord = OslConstructDataPtr(sizeof (PASSWORDREQ));

	if( (pfnUnicodeStrcmp((PS8)g_SecuritySetupContext.NewPassword,(PS8)g_SecuritySetupContext.ConfirmPassword) == 0 )){
//micha0716
		passWord = OslConstructDataPtr(sizeof (PASSWORDREQ));
		UnicodeToAnsii(pOutBuffer,(PS8) g_SecuritySetupContext.Password );
		memcpy(passWord->old, pOutBuffer,(MAX_SIM_SETTING_PASSWD_LEN+1));
		
		memset(pOutBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1)+1);
		UnicodeToAnsii(pOutBuffer, (PS8)g_SecuritySetupContext.NewPassword );
		memcpy(passWord->new1, pOutBuffer,(MAX_SIM_SETTING_PASSWD_LEN+1));
			
		memset(pOutBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1)+1);
		UnicodeToAnsii(pOutBuffer,(PS8) g_SecuritySetupContext.ConfirmPassword );
		memcpy(passWord->new2, pOutBuffer,(MAX_SIM_SETTING_PASSWD_LEN+1));

		switch(g_SecuritySetupContext.CurrentPassword){
		case 0:
			passWord->type = MMI_SETTING_TYPE_CHV1;
			SetProtocolEventHandlerExt(SECSETChangePasswordPIN1Rsp, PRT_MMI_SMU_CHANGE_PASSWORD_RSP,gSecSetupMenuSimID);
		break;
		case 1:
			passWord->type = MMI_SETTING_TYPE_CHV2;
			SetProtocolEventHandlerExt(SECSETChangePasswordPIN2Rsp, PRT_MMI_SMU_CHANGE_PASSWORD_RSP,gSecSetupMenuSimID);
		break;
		case 2: 
			passWord->type = MMI_SETTING_TYPE_PHONELOCK;
			SetProtocolEventHandler(SECSETChangePasswordPhoneRsp, PRT_MMI_SMU_CHANGE_PASSWORD_RSP);
		break;
		case 3:
			SECSETVerifyPINReq(MMI_SETTING_TYPE_UBCHV1, g_SecuritySetupContext.Password,  g_SecuritySetupContext.NewPassword, ValidateNewPin1Success);
			return;
		break;
		case 4:
			SECSETVerifyPINReq(MMI_SETTING_TYPE_UBCHV2, g_SecuritySetupContext.Password,  g_SecuritySetupContext.NewPassword, NULL);
			return;	
		break;	
		default:
		break;
		}

		ClearAllKeyHandler();
		
		msgStruct.oslSrcId = MOD_MMI;
		msgStruct.oslDestId = MOD_L4C + gSecSetupMenuSimID;
		msgStruct.oslMsgId = PRT_MMI_SMU_CHANGE_PASSWORD_REQ;
		msgStruct.oslSapId = INVALID_SAP;
		msgStruct.oslDataPtr = (oslParaType *)passWord;
		msgStruct.oslPeerBuffPtr= NULL;
		
		OslMsgSendExtQueue(&msgStruct);
	}
	else
	{
		memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
		memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);

		switch(g_SecuritySetupContext.CurrentPassword) {
		case 0:
		case 3:	
			DisplayPopup((PU8)GetString(STR_DIFF_PIN) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
			//DeleteUptoScrID(SCR_ID_SECSET_CHANGEPW_NEW_PWD);
			if ( IsScreenPresent(SCR_ID_SECSET_CHANGEPW_NEW_PWD))
			{
				DeleteUptoScrID(SCR_ID_SECSET_CHANGEPW_NEW_PWD);
			}
			else
			{
				DeleteUptoScrID(IDLE_SCREEN_ID);
			}
			if(GetHistoryScrID(SCR_ID_SECSET_CHANGEPW_NEW_PWD,&History)==ST_SUCCESS)
			{
				if(History->inputBuffer!=NULL)
					memset(History->inputBuffer,0,ENCODING_LENGTH);
				if(History->guiBuffer!=NULL)
					memset(History->guiBuffer,0,2);
			}
		break;
		case 1:
		case 4:
			DisplayPopup((PU8)GetString(STR_DIFF_PIN2) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
				if ( IsScreenPresent(SCR_ID_SECSET_CHANGEPW_NEW_PWD))
			{
				DeleteUptoScrID(SCR_ID_SECSET_CHANGEPW_NEW_PWD);
			}
			else
			{
				DeleteUptoScrID(IDLE_SCREEN_ID);
			}
			if(GetHistoryScrID(SCR_ID_SECSET_CHANGEPW_NEW_PWD,&History)==ST_SUCCESS)
			{
				if(History->inputBuffer!=NULL)
					memset(History->inputBuffer,0,ENCODING_LENGTH);
				if(History->guiBuffer!=NULL)
					memset(History->guiBuffer,0,2);
			}
		break;
		case 2:
			DisplayPopup((PU8)GetString(STR_DIFF_PHONE) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
			DeleteUptoScrID(SCR_ID_SECSET_CHANGEPW_NEW_PWD);
			if(GetHistoryScrID(SCR_ID_SECSET_CHANGEPW_NEW_PWD,&History)==ST_SUCCESS)
			{
				if(History->inputBuffer!=NULL)
					memset(History->inputBuffer,0,ENCODING_LENGTH);
				if(History->guiBuffer!=NULL)
					memset(History->guiBuffer,0,2);
			}
		break;
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SECSETChangePasswordPhoneRsp(void *info)

  	PURPOSE				: Response handler of change phonelock password req.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETChangePasswordPhoneRsp(void *info)
{
	RESULTSTRUCT *resp;
	historyNode *History;
	resp = (RESULTSTRUCT *)(info);

	memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);

	if(resp->result == 1)
	{
		DisplayPopup((PU8)GetString(STR_PHONE_CHANGE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		DeleteUptoScrID(SCR_ID_SECSET_CHANGE_PASSWORD);
	
	}
	else 
	{
		DisplayPopup((PU8)GetString(STR_SETTING_WRONG) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_SECSET_CHANGEPW_OLD_PWD);
		if(GetHistoryScrID(SCR_ID_SECSET_CHANGEPW_OLD_PWD,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
	}
	
}

/**************************************************************

	FUNCTION NAME		: SECSETChangePasswordPIN1Rsp

  	PURPOSE				: Response handler of change PIN1 password req.

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETChangePasswordPIN1Rsp(void *info)
{
	mmi_smu_change_password_rsp_struct *resp;
	historyNode *History;
	U8 nSimID;
	
	resp = (mmi_smu_change_password_rsp_struct *)(info);

	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	FillCountDownNumber(resp->chv_info,nSimID);

	memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);

	if(resp->result == 1)
	{
		DisplayPopup((PU8)GetString(STR_PIN_CHANGE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);

		if ( IsScreenPresent(SCR_ID_SECSET_CHANGE_PASSWORD)) {
			DeleteUptoScrID(SCR_ID_SECSET_CHANGE_PASSWORD);
		}
		else {
			DeleteUptoScrID(IDLE_SCREEN_ID);
		}
	}
	else if(resp->result == 0 && resp->cause == MMI_SETTING_TYPE_UBCHV1)
	{
		ExitSECSETPUK1();
		g_SecuritySetupContext.PINBlocked[nSimID] = 1;
		g_pwr_context[nSimID].IsSimValid = 0;	
		DisplayPopup((PU8)GetString(STR_SETTING_PIN_BLK) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		if(GetHistoryScrID(SCR_ID_SECSET_PUK1,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
		 DeleteUptoScrID(SCR_ID_SECSET_PUK1);
	}
	/*wangrui dele block 20081124, for fixbug10393*/
//qiff modify for bug:	10192
#if 0
	else if ((resp->result == 0)&&(resp->cause == MMI_SETTING_TYPE_NO_REQUIRED)) // resp->chv_status.chv1_status == CHV_DISABLED)
	{
		mmi_trace(1, TSTXT("resp->cause == MMI_SETTING_TYPE_NO_REQUIRED"));
		DisplayPopup((PU8) GetString(STR_ENABLE_PIN), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, ERROR_TONE);
	}
#endif
  /*wangrui dele block 20081124 end*/
	else
	{
		DisplayPopup((PU8)GetString(STR_WRONG_PIN) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);

		if ( IsScreenPresent(SCR_ID_SECSET_CHANGEPW_OLD_PWD)) {
			DeleteUptoScrID(SCR_ID_SECSET_CHANGEPW_OLD_PWD);
			if(GetHistoryScrID(SCR_ID_SECSET_CHANGEPW_OLD_PWD,&History)==ST_SUCCESS)
			{
				if(History->inputBuffer!=NULL)
					memset(History->inputBuffer,0,ENCODING_LENGTH);
				if(History->guiBuffer!=NULL)
					memset(History->guiBuffer,0,2);
			}
		}
		else {
			DeleteUptoScrID(IDLE_SCREEN_ID);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: ChangePasswordPIN2SuccessCallBack

  	PURPOSE				: Response handler of change Pin2 password req.

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ChangePasswordPIN2SuccessCallBack(void)
{
	if(!IsScreenPresent(SCR_ID_SECSET_CHANGE_PASSWORD)) {
		DisplayPopup((PU8)GetString(STR_PIN2_CHANGE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		DeleteUptoScrID(IDLE_SCREEN_ID);
	}
	else {
		DisplayPopup((PU8)GetString(STR_PIN2_CHANGE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		DeleteUptoScrID(SCR_ID_SECSET_CHANGE_PASSWORD);
	}
}

/**************************************************************

	FUNCTION NAME		: SECSETChangePasswordPIN2Rsp

  	PURPOSE				: Response handler of change Pin2 password req.

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETChangePasswordPIN2Rsp(void *info)
{
	mmi_smu_change_password_rsp_struct *resp;
	historyNode *History;
	U8 nSimID;
	resp = (mmi_smu_change_password_rsp_struct *)(info);
	nSimID = MTPNP_AD_get_protocol_event_src_mode();

	FillCountDownNumber(resp->chv_info,nSimID);

	if(!IsScreenPresent(SCR_ID_SECSET_CHANGE_PASSWORD))
	{
		if(resp->result == 1)
		{
			DisplayPopup((PU8)GetString(STR_PIN2_CHANGE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
			DeleteUptoScrID(IDLE_SCREEN_ID);
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_WRONG_PIN2) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
			DeleteUptoScrID(IDLE_SCREEN_ID);
		}
	}
	else
	{
		memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
		memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
		memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
		if(resp->result == 1)
		{
			ChangePasswordPIN2SuccessCallBack();
		}
		else if(resp->result == 0 && resp->cause == MMI_SETTING_TYPE_UBCHV2)
		{
//micha0529		
			DeleteUptoScrID(SCR_ID_SECSET_CHANGEPW_OLD_PWD);
			DeleteScreenIfPresent(SCR_ID_SECSET_CHANGEPW_OLD_PWD);
			ExitSECSETPUK2();
			DisplayPopup((PU8)GetString(STR_SETTING_PIN2_BLK) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
			if(GetHistoryScrID(SCR_ID_SECSET_PUK2,&History)==ST_SUCCESS)
			{
				if(History->inputBuffer!=NULL)
					memset(History->inputBuffer,0,ENCODING_LENGTH);
				if(History->guiBuffer!=NULL)
					memset(History->guiBuffer,0,2);
			}
			DeleteUptoScrID(SCR_ID_SECSET_PUK2);
		}		
		else
		{
			DisplayPopup((PU8)GetString(STR_WRONG_PIN2) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
			DeleteUptoScrID(SCR_ID_SECSET_CHANGEPW_OLD_PWD);
			if(GetHistoryScrID(SCR_ID_SECSET_CHANGEPW_OLD_PWD,&History)==ST_SUCCESS)
			{
				if(History->inputBuffer!=NULL)
					memset(History->inputBuffer,0,ENCODING_LENGTH);
				if(History->guiBuffer!=NULL)
					memset(History->guiBuffer,0,2);
			}
		}

	}
	
}

/**************************************************************

	FUNCTION NAME		: IsPhoneLock

  	PURPOSE				: Get the status of phone lock.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: MMI_BOOL

	Author				: Vanita Jain, XB Zhang

 

**************************************************************/
MMI_BOOL IsPhoneLock(void)
{
	return g_SecuritySetupContext.PhoneLockStatus == 1 ? MMI_TRUE : MMI_FALSE;
}

/**************************************************************

	FUNCTION NAME		: GetAutoKeypadLockTime

  	PURPOSE				: API to return the Autokeypadlock time.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

	Author				: Vanita Jain

 

**************************************************************/

SETTING_AKPL_TIME GetAutoKeypadLockTime(void)
{
	U8 data = 0;
	S16 error;
	
		ReadValue( NVRAM_SETTING_AUTOKEYPADLOCK_TIME, &data, DS_BYTE , &error);
		if(data == 0xff)
		{
			data =0;
			WriteValue( NVRAM_SETTING_AUTOKEYPADLOCK_TIME, &data, DS_BYTE , &error);
		}
		return (SETTING_AKPL_TIME)(data);
	
}



/**************************************************************

	FUNCTION NAME		: SettingChangePasswdPuk

  	PURPOSE				: Displays popup that Pin1/Pin2 changed.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

	Author				: Vanita Jain

 

**************************************************************/
void SettingChangePasswdPuk(void)
{
	if(g_SecuritySetupContext.CurrentPassword == 0)
	{
		DisplayPopup((PU8)GetString(STR_PIN_CHANGE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		GoBackToPasswordScr();
	}
	else
	{
		 DisplayPopup((PU8)GetString(STR_PIN2_CHANGE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		 GoBackToPasswordScr();
	}
}

/**************************************************************

	FUNCTION NAME		: GoBackToPasswordScr

  	PURPOSE				: Delete history upto Changepassword screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	Author				: Vanita Jain

 

**************************************************************/
void  GoBackToPasswordScr(void)
{
	DeleteUptoScrID(SCR_ID_SECSET_CHANGE_PASSWORD);
}


/**************************************************************

	FUNCTION NAME		: DialSoSPUK1()

  	PURPOSE				: Function takes care that from which buffer DialPadBuffer should be filled up. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DialSoSPUK1(void)
{
	pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,(S8*)g_SecuritySetupContext.NewPassword);
	EntryScrEmergencyCallDialing();
}

/**************************************************************

	FUNCTION NAME		: DialSoSNewPIN1()

  	PURPOSE				: Function takes care that from which buffer DialPadBuffer should be filled up. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DialSoSNewPIN1(void)
{
	pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,(S8*)g_SecuritySetupContext.Password);
	EntryScrEmergencyCallDialing();
}


/**************************************************************

	FUNCTION NAME		: DialSoSNewPIN1Conf()

  	PURPOSE				: Function takes care that from which buffer DialPadBuffer should be filled up. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DialSoSNewPIN1Conf(void)
{
	pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,(S8*)g_SecuritySetupContext.ConfirmPassword);
	EntryScrEmergencyCallDialing();
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETPUK1(void)

  	PURPOSE				: Entry fn for PUK.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETPUK1(void)
{
	
	U8* guiBuffer;
	U8* inputBuffer;
	U8 PassCountdownDisp[MAX_PASS_COUNTDOWN_DISP];		

	TRACE_SMU_FUNCTION();
	EntryNewScreen(SCR_ID_SECSET_PUK1, ExitSECSETPUK1, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_PUK1);	
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_PUK1);

	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.NewPassword, ( S8*)inputBuffer);		

	RegisterInputBoxValidationFunction(ScrSettingPukValidation);

	g_pwr_context[gSecSetupMenuSimID].IsSimValid = 0;		
	g_SecuritySetupContext.PINBlocked[gSecSetupMenuSimID] = 1;

	FillCountDownString(MMI_SETTING_TYPE_UBCHV1, PassCountdownDisp,gSecSetupMenuSimID);
	ShowCategory111Screen(	STR_GLOBAL_OK, /*OK*/
							IMG_GLOBAL_OK,
							STR_SETTING_SOS, 
							0,
							STR_SETTING_PUK,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.NewPassword,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,PassCountdownDisp, guiBuffer);

	SetCategory111RightSoftkeyFunction(ShowMakeSOSCallScr,KEY_EVENT_UP);
	SetKeyHandler(DialSoSPUK1,KEY_SEND,KEY_EVENT_UP);
#ifdef __SENDKEY2_SUPPORT__
	SetKeyHandler(DialSoSPUK1,KEY_SEND1, KEY_EVENT_DOWN);
	SetKeyHandler(DialSoSPUK1,KEY_SEND2, KEY_EVENT_DOWN);
#endif

	//SetKeyHandler(SSCHandleEnterPUK,KEY_STAR,KEY_EVENT_DOWN);//man for *
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);
	
	SetLeftSoftkeyFunction(EntrySECSETPUKNewPIN1,KEY_EVENT_UP);
	SetKeyHandler(EntrySECSETPUKNewPIN1,KEY_POUND,KEY_EVENT_UP);	
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = EntrySECSETPUKNewPIN1;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.NewPassword) < MMI_MIN_PUK_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETPUK1(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETPUK1(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_PUK1;
	Scr.entryFuncPtr = EntrySECSETPUK1;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.NewPassword);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}
/**************************************************************

	FUNCTION NAME		: EntrySECSETPUKNewPIN1(void)

  	PURPOSE				: Entry fn to enter new PIN

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETPUKNewPIN1(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U8* inputBuffer;
		
	TRACE_SMU_FUNCTION();
	EntryNewScreen(SCR_ID_SECSET_PUK_NEW_PIN1, ExitSECSETNewPIN1, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_PUK_NEW_PIN1);	
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_PUK_NEW_PIN1);

	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
	{
		memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.Password, ( S8*)inputBuffer);
	}
	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);

	ShowCategory111Screen(	STR_GLOBAL_OK, /*OK*/
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_NEW_PIN1,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.Password,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);

	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);

	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntrySECSETPUKConfirmPIN1,KEY_EVENT_UP);
	SetKeyHandler(EntrySECSETPUKConfirmPIN1,KEY_POUND,KEY_EVENT_UP);	
	SetKeyHandler(DialSoSNewPIN1,KEY_SEND,KEY_EVENT_UP);
#ifdef __SENDKEY2_SUPPORT__
	SetKeyHandler(DialSoSNewPIN1,KEY_SEND1, KEY_EVENT_DOWN);
	SetKeyHandler(DialSoSNewPIN1,KEY_SEND2, KEY_EVENT_DOWN);
#endif
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = EntrySECSETPUKConfirmPIN1;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.Password) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETNewPIN1(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETNewPIN1(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_PUK_NEW_PIN1;
	Scr.entryFuncPtr = EntrySECSETPUKNewPIN1;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.Password);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETPUKConfirmPIN1(void)

  	PURPOSE				: ENtry fn for confirm PIN.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntrySECSETPUKConfirmPIN1(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U8* inputBuffer;

	TRACE_SMU_FUNCTION();
	EntryNewScreen(SCR_ID_SECSET_PUK_CONFIRM_PIN1, ExitSECSETNewPIN1Confirm, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_PUK_CONFIRM_PIN1);	
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_PUK_CONFIRM_PIN1);
	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.ConfirmPassword, ( S8*)inputBuffer);

	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	ShowCategory111Screen(	STR_GLOBAL_OK, /*OK*/
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SCR9031_CONFIRM_PW_CAPTION,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.ConfirmPassword,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);
		
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);

	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(ValidateNewPin1,KEY_POUND,KEY_EVENT_UP);		
	SetLeftSoftkeyFunction(ValidateNewPin1,KEY_EVENT_UP);
	SetKeyHandler(DialSoSNewPIN1Conf,KEY_SEND,KEY_EVENT_UP);	
#ifdef __SENDKEY2_SUPPORT__
	SetKeyHandler(DialSoSNewPIN1Conf,KEY_SEND1, KEY_EVENT_DOWN);
	SetKeyHandler(DialSoSNewPIN1Conf,KEY_SEND2, KEY_EVENT_DOWN);
#endif
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = ValidateNewPin1;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.ConfirmPassword) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitNewPinConfirm(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETNewPIN1Confirm(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_PUK_CONFIRM_PIN1;
	Scr.entryFuncPtr = EntrySECSETPUKConfirmPIN1;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.ConfirmPassword);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

/**************************************************************

	FUNCTION NAME		: ValidateNewPin1(void)

  	PURPOSE				: validates the length of Both new pin and confirm pin and also
						  Check if both Pin matches.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ValidateNewPin1(void)
{
	historyNode *History;
	
	if( memcmp(g_SecuritySetupContext.Password,g_SecuritySetupContext.ConfirmPassword,((MAX_SIM_SETTING_PASSWD_LEN+1) - 1) * ENCODING_LENGTH ) == 0)
	{
		SECSETVerifyPINReq(MMI_SETTING_TYPE_UBCHV1, g_SecuritySetupContext.NewPassword,  g_SecuritySetupContext.Password, ValidateNewPin1Success);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_SCR9034_SECURITY_FAILURE_CAPTION) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_SECSET_PUK_NEW_PIN1);
		if(GetHistoryScrID(SCR_ID_SECSET_PUK_NEW_PIN1,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: ValidateNewPin1Success(void)

  	PURPOSE				: validates the length of Both new pin and confirm pin and also
						  Check if both Pin matches.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ValidateNewPin1Success(void)
{
		SECSETGetPIN1StatusReq();
		DisplayPopup((PU8)GetString(STR_SETTING_CODE_VERIFIED) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);		
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETPUK2(void)

  	PURPOSE				: Entry fn for PUK.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETPUK2(void)
{
	U8* guiBuffer;
	U8* inputBuffer;
	U8 PassCountdownDisp[MAX_PASS_COUNTDOWN_DISP];

	EntryNewScreen(SCR_ID_SECSET_PUK2, ExitSECSETPUK2, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_PUK2);	
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_PUK2);

	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.NewPassword, ( S8*)inputBuffer);		

	RegisterInputBoxValidationFunction(ScrSettingPukValidation);

	FillCountDownString(MMI_SETTING_TYPE_UBCHV2, PassCountdownDisp,gSecSetupMenuSimID);
	ShowCategory111Screen(	STR_GLOBAL_OK, /*OK*/
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SETTING_ENTER_PUK2,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.NewPassword,
							(MAX_SIM_SETTING_PASSWD_LEN+1) , PassCountdownDisp, guiBuffer);



	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(EntrySECSETPUK2NewPIN2,KEY_EVENT_UP);
	SetKeyHandler(EntrySECSETPUK2NewPIN2,KEY_POUND,KEY_EVENT_UP);	
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = EntrySECSETPUK2NewPIN2;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.NewPassword) < MMI_MIN_PUK_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETPUK2(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETPUK2(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_PUK2;
	Scr.entryFuncPtr = EntrySECSETPUK2;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.NewPassword);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETPUKNewPIN1(void)

  	PURPOSE				: Entry fn to enter new PIN

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETPUK2NewPIN2(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U8* inputBuffer;
		
	EntryNewScreen(SCR_ID_SECSET_PUK2_NEW_PIN2, ExitSECSETNewPIN2, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_PUK2_NEW_PIN2);	
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_PUK2_NEW_PIN2);

	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
	{
		memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.Password, ( S8*)inputBuffer);
	}
	
	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);

	ShowCategory111Screen(	STR_GLOBAL_OK, /*OK*/
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SETTING_ENTER_NEW_PIN2,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.Password,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);


	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntrySECSETPUK2ConfirmPIN2,KEY_EVENT_UP);
	SetKeyHandler(EntrySECSETPUK2ConfirmPIN2,KEY_POUND,KEY_EVENT_UP);
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = EntrySECSETPUK2ConfirmPIN2;
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.Password) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitNewPin2(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETNewPIN2(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_PUK2_NEW_PIN2;
	Scr.entryFuncPtr = EntrySECSETPUK2NewPIN2;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.Password);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETPUK2ConfirmPIN2(void)

  	PURPOSE				: ENtry fn for confirm PIN.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntrySECSETPUK2ConfirmPIN2(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U8* inputBuffer;

	EntryNewScreen(SCR_ID_SECSET_PUK_CONFIRM_PIN1, ExitSECSETNewPIN2Confirm, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_PUK2_CONFIRM_PIN2);	
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_PUK2_CONFIRM_PIN2);
	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.ConfirmPassword, ( S8*)inputBuffer);

	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	ShowCategory111Screen(	STR_GLOBAL_OK, /*OK*/
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SCR9031_CONFIRM_PW_CAPTION,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.ConfirmPassword,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);
		
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(ValidateNewPin2,KEY_POUND,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(ValidateNewPin2,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = ValidateNewPin2;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.ConfirmPassword) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: ExitNewPinConfirm2(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETNewPIN2Confirm(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_PUK2_CONFIRM_PIN2;
	Scr.entryFuncPtr = EntrySECSETPUK2ConfirmPIN2;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.ConfirmPassword);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

/**************************************************************

	FUNCTION NAME		: ValidateNewPin1(void)

  	PURPOSE				: validates the length of Both new pin and confirm pin and also
						  Check if both Pin matches.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ValidateNewPin2(void)
{
	historyNode *History;
	
	if( memcmp(g_SecuritySetupContext.Password,g_SecuritySetupContext.ConfirmPassword,((MAX_SIM_SETTING_PASSWD_LEN+1) - 1) * ENCODING_LENGTH ) == 0)
	{
		SECSETVerifyPINReq(MMI_SETTING_TYPE_UBCHV2, g_SecuritySetupContext.NewPassword,  g_SecuritySetupContext.Password, NULL);
	}
	else
	{
//micha0702
		DisplayPopup((PU8)GetString(STR_DIFF_PIN2) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_SECSET_PUK2_NEW_PIN2);
		if(GetHistoryScrID(SCR_ID_SECSET_PUK2_NEW_PIN2,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SECSETVerifyPINReq(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETVerifyPINReq(U8 pin_type, U8* ucs2_pin, U8* ucs2_new_pin, FuncPtr success_callback)
{

	U8 tempBuffer[(MAX_SIM_SETTING_PASSWD_LEN+1)];
	 
	MYQUEUE	Message; 
	MMIPHONEVERIFYREQ* local_data;

	TRACE_SMU_FUNCTION();
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);//So that user can't go to idle screen.
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C + gSecSetupMenuSimID;
	Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
	local_data = (MMIPHONEVERIFYREQ*) OslConstructDataPtr(sizeof (MMIPHONEVERIFYREQ));
	
	memset(local_data->pin,'\0',(MAX_SIM_SETTING_PASSWD_LEN+1));
	memset(local_data->new_pin,'\0',(MAX_SIM_SETTING_PASSWD_LEN+1));

	local_data->pin_type = pin_type; 		//_smu_security_type_enum
	
	memset(tempBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
	UnicodeNToAnsii((S8*)tempBuffer,(S8*)ucs2_pin,(MAX_SIM_SETTING_PASSWD_LEN+1) );		
	memcpy(local_data->pin,tempBuffer,(MAX_SIM_SETTING_PASSWD_LEN+1)); 

	memset(tempBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
	UnicodeNToAnsii((S8*)tempBuffer,(S8*)ucs2_new_pin,(MAX_SIM_SETTING_PASSWD_LEN+1) );		
	memcpy(local_data->new_pin,tempBuffer,(MAX_SIM_SETTING_PASSWD_LEN+1)); 

	if (success_callback != NULL) {
		g_SecuritySetupContext.VerifyPassSuccessCallBack = success_callback;
	}
	
	SetProtocolEventHandlerExt(SECSETVerifyPINRsp, PRT_MMI_SMU_VERIFY_PIN_RSP, gSecSetupMenuSimID);
	
	Message.oslDataPtr = (oslParaType *)local_data;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
}

void SetActiveScreenId (U16 scrnId);
void SECSETVerifyPINRsp(void *info)
{
	
	mmi_smu_verify_pin_rsp_struct *local_data = (mmi_smu_verify_pin_rsp_struct *)info;
	historyNode *History;
	U16 i;
	
	U8 nSimID;

	nSimID = MTPNP_AD_get_protocol_event_src_mode();

	if(local_data->next_type!= MMI_SETTING_TYPE_NO_REQUIRED &&local_data->next_type != MMI_SETTING_TYPE_PHONELOCK )
	{
		FillCountDownNumber(local_data->chv_info, nSimID);
	}
	
	g_SecuritySetupContext.PINBlocked[nSimID] = 0;
	g_pwr_context[nSimID].IsSimValid = 1;	
	mmi_trace(g_sw_SMU,"MMI_SIM: Func: %s local_data->next_type=%d", __FUNCTION__, local_data->next_type);
	
	switch (local_data->next_type)
	{
	case MMI_SETTING_TYPE_NO_REQUIRED:
		// Reset the processing flag when phone lock passed.
		if (local_data->result == 1 && g_pwr_context[0].PrevScreenIndicator == ENTER_PHONELOCK)
			g_pwr_context[nSimID].PhoneLockProcesing = FALSE;
		
		if (g_SecuritySetupContext.VerifyPassSuccessCallBack != NULL) 
		{
			mmi_trace(g_sw_SMU,"MMI_SIM: g_SecuritySetupContext.VerifyPassSuccessCallBack != NULL");
			(*g_SecuritySetupContext.VerifyPassSuccessCallBack)();
			g_SecuritySetupContext.VerifyPassSuccessCallBack = NULL;
		}
		else 
		{
			mmi_trace(g_sw_SMU,"MMI_SIM: [SecuritySetup.c] SECSETVerifyPINRsp ABNORMAL");
			//DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, 0);	//deleted by zhoumn for bug 10429 @2007/3/28		
			//DeleteUptoScrID(IDLE_SCREEN_ID);
			//DeleteScreenIfPresent(SCR_ENTER_PHONELOCK );
			SetActiveScreenId(0);
			
			for(i = 0; i < MMI_SIM_NUMBER; i++)
			{
				if(g_SimSuccessIndBuffered[i])
				{
					g_SimSuccessIndBuffered[i] = 0;
					MTPNP_AD_set_protocol_event_src_mode(i);
					if(g_pwr_context[nSimID].SimQueryFailIndFlag)
					{
						SimQueryFailureRsp(&(gSMUFailIndStruct[i]));
					}
					else
					{
						SimPasswdReqResponse(&(gSMUPwdDataStruct[i]));
					}
					break;
				}
			}
		}
		
		break;
	
	case MMI_SETTING_TYPE_PHONELOCK:
	 
		DisplayPopup((PU8)GetString(STR_SETTING_WRONG), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
		if (GetHistoryScrID(SCR_RESTORE, &History) == ST_SUCCESS)
		{	
			if (History->inputBuffer != NULL)
				memset(History->inputBuffer, 0, ENCODING_LENGTH);
			if(History->guiBuffer != NULL)
				memset(History->guiBuffer, 0, 2);
		}
		memset(gPhonePassword, 0, (MAX_SIM_SETTING_PASSWD_LEN + 1));
		memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
		break;
	
	case MMI_SETTING_TYPE_CHV1:
		DisplayPopup((PU8)GetString(STR_WRONG_PIN), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
		break;
	
	case MMI_SETTING_TYPE_CHV2:
		DisplayPopup((PU8)GetString(STR_WRONG_PIN2), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
		if (GetHistoryScrID(SCR_ID_SECSET_PIN2, &History) == ST_SUCCESS)
		{
			if(History->inputBuffer != NULL)
				memset(History->inputBuffer, 0, ENCODING_LENGTH);
			if(History->guiBuffer != NULL)
				memset(History->guiBuffer, 0, 2);
		}
		DeleteUptoScrID(SCR_ID_SECSET_PIN2);
		break;
		
	case MMI_SETTING_TYPE_UBCHV1:
			mmi_trace(g_sw_SMU,"MMI_SIM: MMI_SETTING_TYPE_UBCHV1");
			
			DisplayPopup((PU8)GetString(STR_SETTING_WRONG_PUK), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
			if (GetHistoryScrID(SCR_ID_SECSET_PUK1, &History) == ST_SUCCESS)
			{
				if (History->inputBuffer != NULL)
					memset(History->inputBuffer, 0, ENCODING_LENGTH);
				if (History->guiBuffer != NULL)
					memset(History->guiBuffer, 0, 2);
			}
			
			DeleteUptoScrID(SCR_ID_SECSET_PUK1);
			
			if(!IsScreenPresent(SCR_ID_SECSET_CHANGE_PASSWORD))
			{
				DeleteUptoScrID(IDLE_SCREEN_ID);
			}

			RemoveSSReqScr();
		break;
		
	case MMI_SETTING_TYPE_UBCHV2:
		if (GetExitScrnID() == SCR_ID_SECSET_PIN2) 
		{
			ClearExitHandler();
			ExitSECSETPUK2();
			DisplayPopup((PU8)GetString(STR_SETTING_PIN2_BLK), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_SETTING_WRONG_PUK2), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
			DeleteUptoScrID(SCR_ID_SECSET_PUK2);
			//micha0915
			RemoveSSReqScr();
		}
		
		if(!IsScreenPresent(SCR_ID_SECSET_CHANGE_PASSWORD))
		{
			DeleteUptoScrID(IDLE_SCREEN_ID);
		}
	
		if (GetHistoryScrID(SCR_ID_SECSET_PUK2, &History) == ST_SUCCESS)
		{
			if (History->inputBuffer != NULL)
				memset(History->inputBuffer, 0, ENCODING_LENGTH);
			if (History->guiBuffer != NULL)
				memset(History->guiBuffer, 0, 2);
		}
		break;
		
	case MMI_SETTING_TYPE_CHV1_BLOCKED:
		break;
		
	case MMI_SETTING_TYPE_CHV2_BLOCKED:
		 DisplayPopup((PU8)GetString(STR_SETTING_PIN2_BLOCK_MSG), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
			 DeleteNScrId(SCR_ID_SECSET_MAIN);
		break;
		
	case MMI_SETTING_TYPE_SIM_CARD_BLOCKED:
		 EntryScrnSimBlockRsp();
		break;
		
	case MMI_SETTING_TYPE_UBCHV2_BLOCKED:
		 DisplayPopup((PU8)GetString(STR_ID_PIN2_NOT_SUPPORT), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
			 DeleteNScrId(SCR_ID_SECSET_PUK2);
		break;
		
	case MMI_SETTING_TYPE_UBCHV1_CHV1_NOT_LOCK:
		break;
		
	case MMI_SETTING_TYPE_UNSPECIFIED:
		break;
		
   	default:
		break;
	}
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETSIMLockPIN1(void)

  	PURPOSE				: Entry fn to get SIM lock pwd from user.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntrySECSETSIMLockPIN1(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U8* inputBuffer;
	U8 PassCountdownDisp[MAX_PASS_COUNTDOWN_DISP];
    #if 0
	if(gSimReady[gSecSetupMenuSimID] == FALSE)//man for sim init
	{
		DisplayPopup((PU8)GetString(SAT_WAIT_STRING_ID), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
		return;	
	}
    #endif
	EntryNewScreen(SCR_ID_SECSET_SIM_LOCK_PIN1, ExitSECSETSIMLockPIN1, NULL, NULL);	

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_SIM_LOCK_PIN1);	
	inputBuffer = GetCurrInputBuffer(SCR_ID_SECSET_SIM_LOCK_PIN1);

	if(inputBuffer==NULL) 
		memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_SecuritySetupContext.Password, ( S8*)inputBuffer);

	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	
	FillCountDownString(MMI_SETTING_TYPE_CHV1, PassCountdownDisp,gSecSetupMenuSimID);
	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_SCR9028_PIN1_CAPTION,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_SecuritySetupContext.Password,
							(MAX_SIM_SETTING_PASSWD_LEN+1) , PassCountdownDisp, guiBuffer);
	
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(SECSETSetSIMLockReq,KEY_EVENT_UP);
	SetKeyHandler(SECSETSetSIMLockReq,KEY_POUND,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = SECSETSetSIMLockReq;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.Password) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
}


/**************************************************************

	FUNCTION NAME		: ExitSECSETSIMLockPIN1(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETSIMLockPIN1(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	Scr.scrnID = SCR_ID_SECSET_SIM_LOCK_PIN1;
	Scr.entryFuncPtr = EntrySECSETSIMLockPIN1;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_SecuritySetupContext.Password);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

void SECSETSetFdlModeReq(void)
{
	
	MYQUEUE msgStruct;
	MMISMUDIALMODEREQSTRUCT *dailStruct;
	module_type dest_mod_id = MOD_L4C + gSecSetupMenuSimID;

	TRACE_SMU_FUNCTION();
	dailStruct = OslConstructDataPtr(sizeof (MMISMUDIALMODEREQSTRUCT));

	if(g_SecuritySetupContext.FdlStatus[gSecSetupMenuSimID])
	{
		dailStruct->on_off = (U8)MMI_OFF_OPTION;
	}
	else
	{
		dailStruct->on_off = (U8)MMI_ON_OPTION;
	}

	dailStruct->type = MMI_TYPE_FDN;
	memset(dailStruct->pin,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
	SetProtocolEventHandlerExt(SECSETSetFdlModeRsp, PRT_MMI_SMU_SET_DIAL_MODE_RSP, gSecSetupMenuSimID);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = dest_mod_id;
//micha1229
	msgStruct.oslMsgId = PRT_MMI_SMU_SET_DIAL_MODE_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = (oslParaType *)dailStruct;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);
}

void SECSETSetFdlModeRsp(void *info)
{
	mmi_smu_set_dial_mode_rsp_struct *resp;
	U16 index;
	U8 nSimID;
	index = GetCurrHiliteID();
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);

	resp = (mmi_smu_set_dial_mode_rsp_struct *)(info);
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	if(resp->result == 1) 
	{

		g_SecuritySetupContext.FdlStatus[gSecSetupMenuSimID] = !g_SecuritySetupContext.FdlStatus[gSecSetupMenuSimID];	
	
	
		if(g_SecuritySetupContext.FdlStatus[gSecSetupMenuSimID])
		 {
			pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
			Category52ChangeItemDescription(0,hintData[index]);
			change_left_softkey(STR_GLOBAL_OFF,0);
			DisplayPopup((PU8)GetString(STR_FDL_IS_ON) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);

			//Vivian add for FDL
			g_FixedInit[nSimID] = TRUE;
			SECSETGetFdlEntriesReq();
			//Vivian add end for FDL
		}
		else
		{
			pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
			Category52ChangeItemDescription(0,hintData[index]);
			change_left_softkey(STR_GLOBAL_ON,0);
			DisplayPopup((PU8)GetString(STR_FDL_IS_OFF) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);

		}
		DeleteUptoScrID(SCR_ID_SECSET_FIX_DIAL);
	}
	else 
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteNHistory(1);
	}
}




void SECSETSetBdlModeReq(void)
{
	MYQUEUE msgStruct;
	MMISMUDIALMODEREQSTRUCT *dailStruct;

	TRACE_SMU_FUNCTION();
	dailStruct = OslConstructDataPtr(sizeof (MMISMUDIALMODEREQSTRUCT));

	if(g_SecuritySetupContext.BdlStatus[gSecSetupMenuSimID])
	{
		dailStruct->on_off = (U8)MMI_OFF_OPTION;
	}
	else
	{
		dailStruct->on_off = (U8)MMI_ON_OPTION;
	}

	dailStruct->type = MMI_TYPE_BDN;
	memset(dailStruct->pin,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
	SetProtocolEventHandlerExt(SECSETSetBdlModeRsp, PRT_MMI_SMU_SET_DIAL_MODE_RSP,gSecSetupMenuSimID);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C + gSecSetupMenuSimID;
//micha1229
	msgStruct.oslMsgId = PRT_MMI_SMU_SET_DIAL_MODE_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = (oslParaType *)dailStruct;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);
}


void SECSETSetBdlModeRsp(void *info)
{
	mmi_smu_set_dial_mode_rsp_struct *resp;
	U16 index;
	U8 nSimID;
	
	TRACE_SMU_FUNCTION();
	index = GetCurrHiliteID();
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);

	resp = (mmi_smu_set_dial_mode_rsp_struct *)(info);
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	if(resp->result == 1)
	{
		g_SecuritySetupContext.BdlStatus[nSimID] = !g_SecuritySetupContext.BdlStatus[nSimID];

		g_phb_cntx.phb_ready = FALSE;					
		if(g_SecuritySetupContext.BdlStatus[nSimID])
		{
			pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
			Category52ChangeItemDescription(0,hintData[index]);
			change_left_softkey(STR_GLOBAL_OFF,0);
			DisplayPopup((PU8)GetString(STR_FDL_IS_ON) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		}
		else
		{
			pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
			Category52ChangeItemDescription(0,hintData[index]);
			change_left_softkey(STR_GLOBAL_ON,0);
			DisplayPopup((PU8)GetString(STR_FDL_IS_OFF) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		}
		DeleteUptoScrID(SCR_ID_SECSET_FIX_DIAL);
	}
	else 
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteNHistory(1);
	}
}


/**************************************************************

	FUNCTION NAME		: ScrSettingPukValidation(void)

  	PURPOSE				: Fn for length validation at PUk screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ScrSettingPukValidation(U8* text,U8* cursor,S32 length)
{
//micha0606
	if((length) < MMI_MIN_PUK_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);		
	}
	else {
		EnableLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		SetKeyHandler(g_SecuritySetupContext.PasswdLenCheckCallBack,KEY_POUND,KEY_EVENT_UP);		
	}
}
#ifdef __MMI_MULTI_SIM__
extern BOOL gFDNAskState[];
U8 gFdlBdlModeReqSrc = 0;

void SECSETGetFdlBdlModeInd(void *info)
{
	U8 i;
	U8 nSimID;
	BOOL found = MMI_FALSE;
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(gFDNAskState[i])
		{
			found = MMI_TRUE;
			break;
		}
	}
	
	gFDNAskState[nSimID] = MMI_TRUE;
	
	if(!found)
	{
		gFdlBdlModeReqSrc = nSimID;
		SECSETGetFdlBdlModeReq();
	}

}
#endif

/**************************************************************

	FUNCTION NAME		: SECSETGetFdlBdlModeReq(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETGetFdlBdlModeReq(void)
{
	MYQUEUE msgStruct;
	#ifdef __MMI_MULTI_SIM__
	mmi_trace(g_sw_PHB, "PHB FDL Func: %s(%d) ", __FUNCTION__, gFdlBdlModeReqSrc);

	SetProtocolEventHandlerExt(SECSETGetFdlBdlModeRsp, PRT_MMI_SMU_GET_DIAL_MODE_RSP,gFdlBdlModeReqSrc);
	#else
	mmi_trace(g_sw_PHB, "PHB FDL Func: %s", __FUNCTION__);

	SetProtocolEventHandler(SECSETGetFdlBdlModeRsp, PRT_MMI_SMU_GET_DIAL_MODE_RSP);
	#endif
	msgStruct.oslSrcId = MOD_MMI;
	#ifdef __MMI_MULTI_SIM__
	msgStruct.oslDestId = MOD_L4C + gFdlBdlModeReqSrc;
	#else
	msgStruct.oslDestId = MOD_L4C;
	#endif
//micha1229
	msgStruct.oslMsgId = PRT_MMI_SMU_GET_DIAL_MODE_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);

}

/**************************************************************

	FUNCTION NAME		: SECSETGetFdlBdlModeRsp(void)

  	PURPOSE				: Response handler for req to know the status of FDL mode.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SECSETGetFdlBdlModeRsp(void *info)
{
	DIALMODESTATUSRSPSTRUCT *resp;
	U8 nSimID;
	#ifdef __MMI_MULTI_SIM__
	U8 i;
	BOOL found = MMI_FALSE;
	#endif
		
	resp = (DIALMODESTATUSRSPSTRUCT *)(info);

	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	mmi_trace(g_sw_PHB, "PHB FDL Func: %s(%d) ", __FUNCTION__, nSimID);
	
	switch(resp->dial_mode)
	{
	case MMI_TYPE_FDN:
		g_SecuritySetupContext.FdlStatus[nSimID] = 1;
		break;
	case MMI_TYPE_BDN:
		g_SecuritySetupContext.BdlStatus[nSimID] = 1;
		break;
	case MMI_TYPE_FDN_BDN:
		g_SecuritySetupContext.BdlStatus[nSimID] = 1;
		g_SecuritySetupContext.FdlStatus[nSimID] = 1;
		break;
	}
#ifdef __MMI_MULTI_SIM__

	gFDNAskState[nSimID] = MMI_FALSE;
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(gFDNAskState[i])
		{
			found = MMI_TRUE;
			break;
		}
	}
	if(found)
	{
		gFdlBdlModeReqSrc = i;
		SECSETGetFdlBdlModeReq();
	}
#endif /* __MMI_MULTI_SIM__ */
}



void FillCountDownNumber( sim_chv_info_struct	chv_info, U8 nSimID)
{

	mmi_trace(g_sw_SMU, "MMI_SIM: FillCountDownNumber, nsimID is %d", nSimID);
	mmi_trace(g_sw_SMU, "MMI_SIM: chv1_count = %d,chv2_count = %d, ubchv1_count = %d, ubchv2_count = %d",
						chv_info.chv1_count, chv_info.chv2_count, chv_info.ubchv1_count,chv_info.ubchv2_count);
	
	g_SecuritySetupContext.CHV1Count[nSimID] = chv_info.chv1_count;
	g_SecuritySetupContext.CHV2Count[nSimID] = chv_info.chv2_count;
	g_SecuritySetupContext.UBCHV1Count[nSimID] = chv_info.ubchv1_count;
	g_SecuritySetupContext.UBCHV2Count[nSimID] = chv_info.ubchv2_count;
}



void FillCountDownString( U8 TypeOfPass, U8* PassCountdownDisp, U8 nSimID)
{
	S8 temp1[10];
	S8 temp2[20];

	mmi_trace(g_sw_SMU, "MMI_SIM: FillCountDownString, TypeOfPass is:%d, nSimID is %d",TypeOfPass, nSimID);
	
	memset(temp1, 0, sizeof(temp1));
	memset(temp2, 0, sizeof(temp2));
	
	switch (TypeOfPass)
	{
	case MMI_SETTING_TYPE_CHV1:
		sprintf(temp1, "%d", g_SecuritySetupContext.CHV1Count[nSimID]);
		break;
	case MMI_SETTING_TYPE_CHV2:
		sprintf(temp1, "%d", g_SecuritySetupContext.CHV2Count[nSimID]);
		break;
	case MMI_SETTING_TYPE_UBCHV1:
		sprintf(temp1, "%d", g_SecuritySetupContext.UBCHV1Count[nSimID]);
		break;
	case MMI_SETTING_TYPE_UBCHV2:
		sprintf(temp1, "%d", g_SecuritySetupContext.UBCHV2Count[nSimID]);
		break;
	}
	
	mmi_trace(g_sw_SMU,"MMI_SIM: temp1 is:%s", temp1);
	
	AnsiiToUnicodeString(temp2, temp1 );
	pfnUnicodeStrcpy((S8*)PassCountdownDisp, ( S8*)GetString(STR_PASS_TRIALS_REMAIN));
	pfnUnicodeStrcat( (S8*)PassCountdownDisp, (S8*) temp2);
}


/**************************************************************

	FUNCTION NAME		: SECSETGetFdlStatusReq(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/
extern BOOL is_phb_init_Finishi(void);

void SECSETGetFdlStatusReq(void)
{
      /*wangrui modify  20081121 begin for fixbug 10431*/
	MYQUEUE msgStruct;
	mmi_smu_read_file_info_req_struct *fdlstatus;
	if((gSimReady[gSecSetupMenuSimID] == FALSE) ||!is_phb_init_Finishi())
        {
        	DisplayPopup((PU8)GetString(SAT_WAIT_STRING_ID), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
        	return;	
        }
	 /*wangrui modify  20081121 end*/

	gpFDLBDLContext = &(gFDLBDLContext[gSecSetupMenuSimID]);

	ClearAllKeyHandler();

	fdlstatus = OslConstructDataPtr(sizeof (mmi_smu_read_file_info_req_struct));
	fdlstatus->file_idx = (U8)70;//fdn...FILE_FDN_IDX
	SetProtocolEventHandlerExt(SECSETGetFdlStatusRsp, PRT_MMI_SMU_READ_FILE_INFO_RSP,gSecSetupMenuSimID);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C + gSecSetupMenuSimID;
//micha1229
	msgStruct.oslMsgId = PRT_MMI_SMU_READ_FILE_INFO_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = (oslParaType *)fdlstatus;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);
}

/**************************************************************

	FUNCTION NAME		: SECSETGetFdlStatusRsp(void)

  	PURPOSE				: Response for the request to get the status of FDL or BDL.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/

void SECSETGetFdlStatusRsp(void *info)
{
	mmi_smu_read_file_info_rsp_struct *resp;
	
	resp = (mmi_smu_read_file_info_rsp_struct *)(info);

	if(resp->result.flag == 0/* ok*/)
	{
		gpFDLBDLContext->MaxFDL = resp->num_of_rec;
		EntrySECSETFixDial();
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_NO_FDL) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
	}

}


/**************************************************************

	FUNCTION NAME		: SECSETGetBdlStatusReq(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/

void SECSETGetBdlStatusReq(void)
{
	MYQUEUE msgStruct;
	mmi_smu_read_file_info_req_struct *fdlstatus;

	  /*wangrui modify  20081121 begin for fixbug 10431*/
	if(!is_phb_init_Finishi())//man for sim init
        {
        	DisplayPopup((PU8)GetString(SAT_WAIT_STRING_ID), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
        	return;	
        }
	 /*wangrui modify  20081121 end*/
	 
	gpFDLBDLContext = &gFDLBDLContext[gSecSetupMenuSimID];

	ClearAllKeyHandler();

	fdlstatus = OslConstructDataPtr(sizeof (mmi_smu_read_file_info_req_struct));
	fdlstatus->file_idx = (U8)82;		//bdn...FILE_BDN_IDX
	SetProtocolEventHandlerExt(SECSETGetBdlStatusRsp, PRT_MMI_SMU_READ_FILE_INFO_RSP, gSecSetupMenuSimID);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C + gSecSetupMenuSimID;
//micha1229
	msgStruct.oslMsgId = PRT_MMI_SMU_READ_FILE_INFO_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = (oslParaType *)fdlstatus;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);

	
}

/**************************************************************

	FUNCTION NAME		: SECSETGetBdlStatusRsp(void)

  	PURPOSE				: Response for the request to get the status of FDL or BDL.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/

void SECSETGetBdlStatusRsp(void *info)
{
	mmi_smu_read_file_info_rsp_struct *resp;

	mmi_trace(g_sw_PHB,"PHB FDL Func: %s ", __FUNCTION__);
	
	resp = (mmi_smu_read_file_info_rsp_struct *)(info);
	
	if(resp->result.flag == 0/* ok*/)
	{
		gpFDLBDLContext->MaxBDL = resp->num_of_rec;
		EntrySECSETBarDial();
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_NO_FDL) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
	}
}


#ifdef __MMI_VIP_FUNCTION__
void HighlightVIPFunction(void);
void HighlihgtVIPStatus(void);
void HighlightVIPList(void);
void entry_VIP_function (void);
void HintVIPStatus(U16 index);
void EntryPassWordForVIP(void);
void EntryVIPConfirmPassWord(void);
void PreEntryVIPList(void);
void EntryVIPList(void);
void VIPPasswordError(void);
void HighlightVIPListEdit(void);
void VIPNumberEdit(void);
void HighlightVIPListErase(void);
void mmi_VIP_pre_erase_phone_number(void);
void mmi_VIP_Write_To_NVRAM(void);
void mmi_VIP_Read_From_NVRAM(void);
void TurnOnOffVIPFunction(void);

U8 g_sw_vip = 0;

vip_context_struct g_vip_context;
U8 g_vip_compare_length = 0xff;

void InitVIPFunction(void)
{
	SetHiliteHandler(MENU_SETTING_VIP_FUNCTION, HighlightVIPFunction);
	SetHiliteHandler(MENU_SETTING_VIP_STATUS, HighlihgtVIPStatus);
	SetHintHandler(MENU_SETTING_VIP_STATUS, HintVIPStatus);
	SetHiliteHandler(MENU_SETTING_VIP_LIST, HighlightVIPList);
	SetHiliteHandler(MENU_SETTING_VIP_EDIT, HighlightVIPListEdit);
	SetHiliteHandler(MENU_SETTING_VIP_ERASE, HighlightVIPListErase);

	memset(&g_vip_context, 0, sizeof(vip_context_struct));

	mmi_VIP_Read_From_NVRAM();
}
void HighlightVIPFunction(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
	SetLeftSoftkeyFunction(entry_VIP_function,KEY_EVENT_UP);
	SetKeyHandler(entry_VIP_function, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HintVIPStatus(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	mmi_trace(g_sw_vip, "MMI_VIP: HintVIPStatus, status is %d",g_vip_context.saved_data.status);
	if(g_vip_context.saved_data.status)
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON),MAX_SUBMENU_CHARACTERS);
	}
	else
	{
		pfnUnicodeStrncpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF),MAX_SUBMENU_CHARACTERS);
	}
}
void mmi_turn_off_VIP_function(void)
{
	g_vip_context.PasswdSucessCallBack = TurnOnOffVIPFunction;
	g_vip_context.PasswdFailCallBack = VIPPasswordError;
	EntryVIPConfirmPassWord();
}
void HighlihgtVIPStatus(void)
{
	mmi_trace(g_sw_vip, "MMI_VIP: HighlihgtVIPStatus, status is %d",g_vip_context.saved_data.status);
	if(g_vip_context.saved_data.status)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF, 0);
		SetLeftSoftkeyFunction(mmi_turn_off_VIP_function, KEY_EVENT_UP);
		SetKeyHandler(mmi_turn_off_VIP_function, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON, 0);
		SetLeftSoftkeyFunction(EntryPassWordForVIP, KEY_EVENT_UP);
		SetKeyHandler(EntryPassWordForVIP, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}

	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightVIPList(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
	SetLeftSoftkeyFunction(PreEntryVIPList, KEY_EVENT_UP);
	SetKeyHandler(PreEntryVIPList, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void HighlightVIPListEdit(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
	SetLeftSoftkeyFunction(VIPNumberEdit, KEY_EVENT_UP);
	SetKeyHandler(VIPNumberEdit, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void HighlightVIPListErase(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
	SetLeftSoftkeyFunction(mmi_VIP_pre_erase_phone_number, KEY_EVENT_UP);
	SetKeyHandler(mmi_VIP_pre_erase_phone_number, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void entry_VIP_function (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	U8 *VIPStatus[MAX_SUB_MENUS];

	mmi_trace(g_sw_vip, "MMI_VIP: entry_VIP_function");

	EntryNewScreen(SCR_SETTING_VIP_FUNCTION, NULL, entry_VIP_function, NULL);	

	guiBuffer = GetCurrGuiBuffer (SCR_SETTING_VIP_FUNCTION);

	numItems = GetNumOfChild (MENU_SETTING_VIP_FUNCTION);	
	GetSequenceStringIds(MENU_SETTING_VIP_FUNCTION, nStrItemList);	
	SetParentHandler (MENU_SETTING_VIP_FUNCTION);	

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);

	ConstructHintsList(MENU_SETTING_VIP_FUNCTION, VIPStatus);

	ShowCategory52Screen(STR_VIP_SETTING,	IMG_SETTING_SECURITY, 
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						numItems, nStrItemList, (U16 *)gIndexIconsImageList, VIPStatus,
						0, 0,	guiBuffer );
}

void TurnOnOffVIPFunction(void)
{
	if(g_vip_context.saved_data.status)
	{
		g_vip_context.saved_data.status = 0;
		mmi_trace(g_sw_vip, "MMI_VIP: turn off VIP Fucntion");
		DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0, ST_NOTIFYDURATION, SUCCESS_TONE);
	}
	else
	{
		g_vip_context.saved_data.status = 1;
		mmi_trace(g_sw_vip, "MMI_VIP: turn on VIP Fucntion");
		DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0, ST_NOTIFYDURATION, SUCCESS_TONE);
	}
	DeleteUptoScrID(SCR_SETTING_VIP_FUNCTION);
	mmi_VIP_Write_To_NVRAM();
}
void VIPPassWordNotAccordant(void)
{
	DisplayPopup((U8 *)GetString(STR_VIP_PASSWORD_NOT_ACCORDANT), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
}
void ExitPassWordForVIP(void)
{
	history_t   Scr;
	Scr.scrnID = SCR_SETTING_VIP_PASSWORD;
	Scr.entryFuncPtr = EntryPassWordForVIP;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_vip_context.saved_data.Password);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

void VIPPasswdScrSoftKeyFunc(U8* text,U8* cursor,S32 length)
{
	if((length) < MIN_VIP_SETTING_PASSWD_LEN)
	{
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	else
	{
		EnableLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		SetKeyHandler(g_SecuritySetupContext.PasswdLenCheckCallBack,KEY_POUND,KEY_EVENT_UP);
	}
}

void EntryPassWordForVIP(void)
{
	U8* guiBuffer;
	U8* inputBuffer;

	mmi_trace(g_sw_vip, "MMI_VIP: EntryPassWordForVIP");		
	EntryNewScreen(SCR_SETTING_VIP_PASSWORD, ExitPassWordForVIP, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_SETTING_VIP_PASSWORD);	
	inputBuffer = GetCurrInputBuffer(SCR_SETTING_VIP_PASSWORD);

	if(inputBuffer==NULL) 
		memset(g_vip_context.saved_data.Password,0,(MAX_VIP_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
	{
		memset(g_vip_context.saved_data.Password,0,(MAX_VIP_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
		pfnUnicodeStrcpy((S8*)g_vip_context.saved_data.Password, ( S8*)inputBuffer);
	}
	
	RegisterInputBoxValidationFunction(VIPPasswdScrSoftKeyFunc);
	
	ShowCategory111Screen(	STR_GLOBAL_OK,
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_VIP_INPUT_PASSWORD,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_vip_context.saved_data.Password,
							(MAX_VIP_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);

	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);

	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryVIPConfirmPassWord,KEY_EVENT_UP);
	SetKeyHandler(EntryVIPConfirmPassWord,KEY_POUND,KEY_EVENT_UP);	

	g_vip_context.PasswdLenCheckCallBack = EntryVIPConfirmPassWord;
	g_vip_context.PasswdSucessCallBack = TurnOnOffVIPFunction;
	g_vip_context.PasswdFailCallBack = VIPPassWordNotAccordant;
	
	if(pfnUnicodeStrlen((S8*)g_vip_context.saved_data.Password) < MIN_PASSWORD_LENGTH) 
	{
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

void ExitVIPPassWordConfirm(void)
{
	history_t   Scr; 
	Scr.scrnID = SCR_SETTING_VIP_PASSWORD_CONFIRM;
	Scr.entryFuncPtr = EntryVIPConfirmPassWord;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_vip_context.ConfirmPassword);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

void ValidateVIPPassword(void)
{
	
	if(memcmp(g_vip_context.saved_data.Password,g_vip_context.ConfirmPassword,MAX_VIP_SETTING_PASSWD_LEN * ENCODING_LENGTH) == 0)
	{
		if(g_vip_context.PasswdSucessCallBack != NULL)
		{
			mmi_trace(g_sw_vip, "MMI_VIP: ValidateVIPPassword, PasswdSucessCallBack is 0x%x",g_vip_context.PasswdSucessCallBack);
			g_vip_context.PasswdSucessCallBack();
		}
	}
	else
	{
		if(g_vip_context.PasswdFailCallBack != NULL)
		{
			mmi_trace(g_sw_vip, "MMI_VIP: ValidateVIPPassword, PasswdFailCallBack is 0x%x",g_vip_context.PasswdFailCallBack);
			g_vip_context.PasswdFailCallBack();
		}
	}
}


void EntryVIPConfirmPassWord(void)
{
	U8* guiBuffer;
	U8* inputBuffer;

	mmi_trace(g_sw_vip, "MMI_VIP: EntryVIPConfirmPassWord");
	
	EntryNewScreen(SCR_SETTING_VIP_PASSWORD_CONFIRM, ExitVIPPassWordConfirm, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_SETTING_VIP_PASSWORD_CONFIRM);	
	inputBuffer = GetCurrInputBuffer(SCR_SETTING_VIP_PASSWORD_CONFIRM);
	
	if(inputBuffer==NULL) 
		memset(g_vip_context.ConfirmPassword,0,(MAX_VIP_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)g_vip_context.ConfirmPassword, ( S8*)inputBuffer);

	RegisterInputBoxValidationFunction(VIPPasswdScrSoftKeyFunc);

	ShowCategory111Screen(	STR_GLOBAL_OK,
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_VIP_CONFIRM_PASSWORD,
							INPUT_TYPE_NUMERIC_PASSWORD,
							g_vip_context.ConfirmPassword,
							(MAX_VIP_SETTING_PASSWD_LEN+1) ,NULL, guiBuffer);
		
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);

	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(ValidateVIPPassword,KEY_POUND,KEY_EVENT_UP);		
	SetLeftSoftkeyFunction(ValidateVIPPassword,KEY_EVENT_UP);

	g_vip_context.PasswdLenCheckCallBack = ValidateVIPPassword;
	
	if(pfnUnicodeStrlen((S8*)g_vip_context.ConfirmPassword) < MIN_PASSWORD_LENGTH) 
	{
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}
void PreEntryVIPList(void)
{
	mmi_trace(g_sw_vip, "MMI_VIP: PreEntryVIPList, status is:%d",g_vip_context.saved_data.status);
	if(g_vip_context.saved_data.status)
	{
		g_vip_context.PasswdSucessCallBack = EntryVIPList;
		g_vip_context.PasswdFailCallBack = VIPPasswordError;
		EntryVIPConfirmPassWord();
	}
	else
	{
		EntryVIPList();
	}
}
void VIPListGetHiliteIndex (S32 nIndex)
{
	g_vip_context.currHiliteIndex = nIndex;
}
pBOOL mmi_vip_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	S8  temp[(MAX_PB_NUMBER_LENGTH + 1)*ENCODING_LENGTH];

	if((item_index < 0) || (item_index > MAX_NUMBER_VIP))
		return FALSE;
	
	pfnUnicodeStrcpy(temp, (S8*)g_vip_context.saved_data.VIPNumber[item_index]);

	if(pfnUnicodeStrlen(temp))
		pfnUnicodeStrcpy((S8*)str_buff, temp);
	else
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)GetString (STR_GLOBAL_EMPTY_LIST));
	
	*img_buff_p = get_image(gIndexIconsImageList[item_index]);
	return TRUE;
}
S32 mmi_vip_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	if((item_index < 0) || (item_index > MAX_NUMBER_VIP))
		return FALSE;

	return 0;
}

void mmi_vip_list_option (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_SETTING_VIP_LIST_OPTION, NULL, mmi_vip_list_option, NULL);	

	guiBuffer = GetCurrGuiBuffer (SCR_SETTING_VIP_LIST_OPTION);
	
	numItems = GetNumOfChild (MENU_SETTING_VIP_EDIT_OPT);	
	GetSequenceStringIds(MENU_SETTING_VIP_EDIT_OPT, nStrItemList);	
	SetParentHandler (MENU_SETTING_VIP_EDIT_OPT);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
		
	ShowCategory52Screen(STR_GLOBAL_OPTIONS,	IMG_SETTING_SECURITY, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

void EntryVIPList(void)
{
	U8*	guiBuffer=NULL;
	
	mmi_trace(g_sw_vip, "MMI_VIP: EntryVIPList");
	
	EntryNewScreen(SCR_SETTING_VIP_LIST, NULL, EntryVIPList, NULL);
	
	DeleteScreenIfPresent(SCR_SETTING_VIP_PASSWORD_CONFIRM);
	
	guiBuffer = GetCurrGuiBuffer (SCR_SETTING_VIP_LIST);

	RegisterHighlightHandler (VIPListGetHiliteIndex);

	ShowCategory184Screen(STR_VIP_SETTING, IMG_SETTING_SECURITY, 
							STR_GLOBAL_EDIT, 0,
							STR_GLOBAL_BACK, 0,
							MAX_NUMBER_VIP,
							mmi_vip_list_get_item, 
							mmi_vip_list_get_hint , 
							0, 
							guiBuffer);
	
	SetLeftSoftkeyFunction (mmi_vip_list_option, KEY_EVENT_UP);
	SetKeyHandler (mmi_vip_list_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
void VIPPasswordError(void)
{
	DisplayPopup((U8 *)GetString(STR_VIP_PASSWORD_ERROR), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
}

void mmi_vip_save_phone_number(void)
{
	memset((S8*)g_vip_context.saved_data.VIPNumber[g_vip_context.currHiliteIndex], 0, (MAX_PB_NUMBER_LENGTH + 1)*ENCODING_LENGTH);
	pfnUnicodeStrcpy((S8*)g_vip_context.saved_data.VIPNumber[g_vip_context.currHiliteIndex], (S8*)g_vip_context.editNumber);
	mmi_VIP_Write_To_NVRAM();
	GoBacknHistory (1);
}


void mmi_vip_phone_number_empty (void)
{
	ChangeLeftSoftkey (0, 0);
	
}
void mmi_vip_phone_number_not_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, 0);
	SetLeftSoftkeyFunction (mmi_vip_save_phone_number, KEY_EVENT_UP);
}

void mmi_vip_entry_phone_number (void)
{
	U8* 	guiBuffer;

	mmi_trace(g_sw_vip, "MMI_VIP: mmi_vip_entry_phone_number");
	EntryNewScreen(SCR_SETTING_VIP_NUMBER, NULL, mmi_vip_entry_phone_number, NULL);

	guiBuffer = GetCurrGuiBuffer (SCR_SETTING_VIP_NUMBER);
	
	RegisterInputBoxEmptyFunction (mmi_vip_phone_number_empty);
	RegisterInputBoxNotEmptyFunction (mmi_vip_phone_number_not_empty);

	ShowCategory5Screen (STR_VIP_SETTING, IMG_SETTING_SECURITY,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0, 
						  INPUT_TYPE_PHONE_NUMBER,
						  (U8*) g_vip_context.editNumber, MAX_PB_NUMBER_LENGTH, guiBuffer); 

	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}

void VIPNumberEdit(void)
{
	memset((S8*)g_vip_context.editNumber, 0, (MAX_PB_NUMBER_LENGTH + 1)*ENCODING_LENGTH);
	pfnUnicodeStrcpy((S8*)g_vip_context.editNumber, (S8*)g_vip_context.saved_data.VIPNumber[g_vip_context.currHiliteIndex]);
	mmi_vip_entry_phone_number();
}

void mmi_VIP_erase_phone_number(void)
{
	memset((S8*)g_vip_context.saved_data.VIPNumber[g_vip_context.currHiliteIndex], 0, (MAX_PB_NUMBER_LENGTH + 1)*ENCODING_LENGTH);
	DisplayPopup ((PU8)GetString (STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, (U8)SUCCESS_TONE);
	DeleteUptoScrID(SCR_SETTING_VIP_LIST);
	mmi_VIP_Write_To_NVRAM();
}

void VIPGoBack2History (void)
{
	GoBacknHistory (1);
}

void mmi_VIP_pre_erase_phone_number(void)
{
	if(pfnUnicodeStrlen((S8*)g_vip_context.saved_data.VIPNumber[g_vip_context.currHiliteIndex])==0)
	{
		DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, ST_NOTIFYDURATION, (U8)EMPTY_LIST_TONE);
	}
	else
	{
		DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,	
	                                STR_GLOBAL_NO, IMG_GLOBAL_NO,   
	                                get_string(STR_VIP_NUMBER_ERASE_Q),
	                                IMG_GLOBAL_QUESTION, WARNING_TONE);
		SetLeftSoftkeyFunction (mmi_VIP_erase_phone_number, KEY_EVENT_UP);
		SetRightSoftkeyFunction (VIPGoBack2History, KEY_EVENT_UP);
	}
}

void mmi_VIP_Write_To_NVRAM(void)
{
	S16	error;
	S32 ret;

	ret = WriteRecord(NVRAM_EF_VIP_FUNCTION_LID, (U16)1, &(g_vip_context.saved_data), NVRAM_EF_VIP_FUNCTION_SIZE, &error);
	mmi_trace(g_sw_vip,"MMI_VIP: mmi_VIP_Write_To_NVRAM, error is:%d,ret is:%d",error, ret);
}
void mmi_VIP_Read_From_NVRAM(void)
{
	S16	error;
	
	ReadRecord(NVRAM_EF_VIP_FUNCTION_LID,  (U16)1, (void*)&(g_vip_context.saved_data), NVRAM_EF_VIP_FUNCTION_SIZE, &error);
}
void mmi_VIP_read_compare_length(void)
{
	S16 pError;
	U8 comp_len;

	/*Already read value*/
	if (g_vip_compare_length != 0xff)
	{
		return;
	}

	ReadRecord(NVRAM_EF_PHB_COMPARE_DIGIT_LID, 1, (void*)&comp_len, 1, &pError);

	if(comp_len < 6)
	{
		g_vip_compare_length = 6;
	}
	else if (comp_len > 20)
	{
		g_vip_compare_length = 20;
	}

	else
	{
		g_vip_compare_length = comp_len;
	}
}
#define MMI_TRACE_VIP_NUMBER_DATA 0
BOOL mmi_VIP_is_number_suited(U8* number1, U8* number2)
{
	U8 num1_len, num2_len;
	U8 temp1[30];
	U8 temp2[30];
	
	#if MMI_TRACE_VIP_NUMBER_DATA
	U8 j;
	#endif
	
	memset(temp1, 0, sizeof(temp1));
	memset(temp2, 0, sizeof(temp2));
	num1_len = pfnUnicodeStrlen(number1);
	num2_len = pfnUnicodeStrlen(number2);

	
	mmi_trace(g_sw_vip, "MMI_VIP: mmi_VIP_is_number_suited, num1_len is %d, num2_len is %d,g_vip_compare_length is %d",num1_len,num2_len,g_vip_compare_length);
	if(num1_len >= g_vip_compare_length && num2_len >= g_vip_compare_length)
	{
		pfnUnicodeStrcpy(temp1, number1+(num1_len - g_vip_compare_length)*ENCODING_LENGTH);
		pfnUnicodeStrcpy(temp2, number2+(num2_len - g_vip_compare_length)*ENCODING_LENGTH);

		#if MMI_TRACE_VIP_NUMBER_DATA
		for(j = 0; j < g_vip_compare_length*2; j ++)
		{
			mmi_trace(g_sw_vip, "MMI_VIP: number1 is %d", *(temp1+j) - '0');
			mmi_trace(g_sw_vip, "MMI_VIP: number2 is %d", *(temp2+j) - '0');
		}
		#endif
		
		if(pfnUnicodeStrcmp(temp1, temp2) == 0)
			return MMI_TRUE;
	}
	return MMI_FALSE;
}
BOOL mmi_VIP_is_vip_number(U8* number)
{
	U8 i;
		
	mmi_trace(g_sw_vip, "MMI_VIP: mmi_VIP_is_vip_number");

	if(!g_vip_context.saved_data.status)
	{
		mmi_trace(g_sw_vip, "MMI_VIP: mmi_VIP_is_vip_number, status is off, resturn FALSE");
		return MMI_FALSE;
	}
	
	mmi_VIP_read_compare_length();
	
	for(i = 0; i < MAX_NUMBER_VIP; i++)
	{
		
		if(mmi_VIP_is_number_suited((U8*)number, (U8*)(g_vip_context.saved_data.VIPNumber[i])))
		{
			mmi_trace(g_sw_vip, "MMI_VIP: mmi_VIP_is_vip_number, return TRUE");
			return MMI_TRUE;
		}
	}
	mmi_trace(g_sw_vip, "MMI_VIP: mmi_VIP_is_vip_number, return FALSE");
	return MMI_FALSE;
	
}

#endif

