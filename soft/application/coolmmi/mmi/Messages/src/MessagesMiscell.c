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
 * MessagesMiscell.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for MMI SMS APP.
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/******************************************************************************

	FILENAME	: MessagesMiscell.c

  	PURPOSE		: 

 

	AUTHOR		: Magesh k 

	DATE		: 01-01-2003

******************************************************************************/

#ifdef __MOD_SMSAL__
/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

//... Add More MMI header
#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "unicodexdcl.h"
#include "timerevents.h"
#include "debuginitdef.h"

//...Add MSG header
#include "customer_ps_inc.h"
#include "mmi_msg_context.h"
#include "messagesl4def.h"
#include "messagesresourcedata.h"
#include "messagesmiscell.h"
#include "messagesexdcl.h"
#include "smsguiinterfaceprot.h"
#include "smsguiinterfacetype.h"
#include "smspshandler.h"
#include "cellbroadcasttype.h"
//#include "queuegprot.h"

#include "gui_ems.h"
#include "wgui_ems_categories.h"
#include "gsm7bitdefaultalphabet.h"
//
#include "csp.h"
#include "idleappdef.h"
#include "callmanagementgprot.h"
#include "callstructuremanagementprot.h"
#include "phonebookgprot.h"
#include "sscstringhandle.h"
#include "alarmframeworkprot.h"
#include "profilegprots.h"
#include "mmi_trace.h"
#include "gpioinc.h"
#include "keybrd.h"
#include "sublcdhistorygprot.h"
//
#ifdef __MMI_MESSAGES_CLUB__
#include "mmiapi_res.h"
#endif

#ifdef MMS_SUPPORT
#include "wap.h"
#endif

#ifdef __MMI_MESSAGES_CHAT__
#include "chatappgprot.h"
#include "chatappdef.h"
#include "chatgtype.h"
#endif
#if defined(__MMI_MESSAGES_TEMPLATE__)
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#if defined(_MUTILANG_TEMPLATE_)||defined(__MMI_MESSAGES_SIMCHINESE_TEMPLATE__)
#include "fontres.h"
#include "fontdcl.h"
#include "sscstringhandle.h"
#endif
#endif
#undef  __NEWSIMULATOR
#ifdef MMI_ON_HARDWARE_P
#include "nvram_user_defs.h"
#endif

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "mtpnp_ad_resdef.h"
#endif

#include "settinggenum.h"
#include "securitysetup.h"

#include "smsapi.h"
#include "smsstruct.h"
#include "audioplayerdef.h"  //added by jinzh:20070704
#include "audioplayertype.h"  //added by jinzh:20070704
#include "sms_common_setting.h" 
#include "servicedefs.h"
#ifdef __MMI_NOKIA_STYLE_N800__
#include "callhistoryenum.h"
#endif
#if defined(__MMI_MESSAGES_TEMPLATE__)
const U16 templateStrings [] =
{		
		STR_SMS_TEMPLATE_1,
		STR_SMS_TEMPLATE_2,
		STR_SMS_TEMPLATE_3,
		STR_SMS_TEMPLATE_4,
		STR_SMS_TEMPLATE_5,
		STR_SMS_TEMPLATE_6,
		STR_SMS_TEMPLATE_7,
		STR_SMS_TEMPLATE_8,
		STR_SMS_TEMPLATE_9,
		STR_SMS_TEMPLATE_10
};
#ifdef _MUTILANG_TEMPLATE_
//#define TOTAL_TEMPLATE_LANGUAGE			3
extern pBOOL IsTrChineseSet(void);
extern pBOOL IsSmChineseSet(void);
extern pBOOL IsFrenchSet(void);
extern pBOOL IsTurkishSet(void);
extern pBOOL IsThaiSet(void);
extern pBOOL IsArabicSet(void);
extern pBOOL IsPersianSet(void);
extern pBOOL IsRussianSet(void);
extern pBOOL IsVietnameseSet(void);
#ifdef __MMI_LANG_ITALIAN__
extern pBOOL IsItalianSet(void);
#endif
#ifdef __MMI_LANG_HINDI__
extern pBOOL IsHindiSet(void);
#endif
#ifdef __MMI_LANG_GERMAN__
extern pBOOL IsGermanSet(void);
#endif
#ifdef __MMI_LANG_SPANISH__
extern pBOOL IsSpanishSet(void);
#endif
#ifdef __MMI_LANG_INDONESIAN__
extern pBOOL IsIndonesianSet(void);
#endif
#ifdef __MMI_LANG_MALAY__
extern pBOOL IsMalaySet(void);
#endif
#endif //_MUTILANG_TEMPLATE_
#if defined(_MUTILANG_TEMPLATE_)||defined(__MMI_MESSAGES_SIMCHINESE_TEMPLATE__)
static U8	currLang[SSC_SIZE];
extern sLanguageDetails* gLanguageArray;
extern U16 gCurrLangIndex;
#endif //_MUTILANG_TEMPLATE_, __MMI_MESSAGES_SIMCHINESE_TEMPLATE__
#endif //__MMI_MESSAGES_TEMPLATE__
#ifdef __MMI_MESSAGES_CHAT__
static U8 NewChatMessageFromXxxStr [(MAX_PHONE_NUMBER_OR_NAME_LENGTH+1)*ENCODING_LENGTH];
U16 gChatMsgFlag;  
void GetNewChatMsgIndDisplayString (U8 *number, U8 length, U8 type);
PU8 GetNewChatSmsIndDisplayStr (void);
#endif
static FuncPtr endKeyDownFuncPtr=NULL;
static U16 messagesCurrScrnID;
static U8* strBuff=NULL;

extern audply_struct g_audply;    //Added by jinzh:20070704
extern U8 gKeyPadLockFlag;
extern msg_msgbox_info_struct msgbox_info;
extern PendingSaveSendDataStruct PendingSaveSendData;
extern U16 GetCmMarkerScrnID (void);
extern EMSData * GetEMSDataForView              (EMSData ** p, U8 force) ;
extern MMI_ALERT_TYPE GetMtCallAlertTypeEnum(void);
extern void mdi_audio_suspend_background_play(void);  //Added by jinzh:20070704
/*****************************************************************************
* FUNCTION
*  IsMMIInIdleState
* DESCRIPTION
*   Check if MMI is in idle state (idle screen or screen saver)
*
* PARAMETERS
*  none
* RETURNS
*  1: idle, 0: non-idle
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 IsMMIInIdleState (void)
{
	if(g_idle_context.IsOnIdleScreen || g_idle_context.ScreenSaverRunFlag)
		return 1;
	else
		return 0;
}

/*****************************************************************************
* FUNCTION
*  IsKeyPadLockState
* DESCRIPTION
*   Check if MMI is in keypad lock state
*
* PARAMETERS
*  none
* RETURNS
*  keypad lock flag (1: lock, 0: unlock)
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 IsKeyPadLockState (void)
{
	return gKeyPadLockFlag;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_phb_send_case
* DESCRIPTION
*   Set PhbSmsInterfaceState
*
* PARAMETERS
*  a  IN       PhbSmsInterfaceState
* RETURNS
*  none
* GLOBALS AFFECTED
*   g_msg_cntx.PhbSmsInterfaceState
*****************************************************************************/
void mmi_msg_set_phb_send_case (U8 PhbSmsInterfaceState)
{
	g_msg_cntx.PhbSmsInterfaceState=PhbSmsInterfaceState;
}

/*****************************************************************************
* FUNCTION
*  GetCurrEndKeyDownHandler
* DESCRIPTION
*   Get current end key down handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   endKeyDownFuncPtr
*****************************************************************************/
void GetCurrEndKeyDownHandler (void)
{
	endKeyDownFuncPtr = NULL;
	endKeyDownFuncPtr = GetKeyHandler (KEY_END, KEY_EVENT_DOWN);
	mmi_trace(g_sw_SMS, "MMI_SMS: GetCurrEndKeyDownHandler,endKeyDownFuncPtr is: 0x%x",endKeyDownFuncPtr);
}

/*****************************************************************************
* FUNCTION
*  ExecCurrEndKeyDownHandler
* DESCRIPTION
*   Execute current end key down handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExecCurrEndKeyDownHandler (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: ExecCurrEndKeyDownHandler,endKeyDownFuncPtr is: 0x%x",endKeyDownFuncPtr);
	if (endKeyDownFuncPtr!=NULL)
	(*endKeyDownFuncPtr) ();
}

/*****************************************************************************
* FUNCTION
*  GetMessagesCurrScrnID
* DESCRIPTION
*   Get MSG current screen ID
*
* PARAMETERS
*  none
* RETURNS
*  messagesCurrScrnID
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 GetMessagesCurrScrnID (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: GetMessagesCurrScrnID = %d", messagesCurrScrnID);
	return messagesCurrScrnID;
}

/*****************************************************************************
* FUNCTION
*  SetMessagesCurrScrnID
* DESCRIPTION
*   Set MSG current screen ID
*
* PARAMETERS
*  a  IN       screen ID
* RETURNS
*  none
* GLOBALS AFFECTED
*   messagesCurrScrnID
*****************************************************************************/
void SetMessagesCurrScrnID (U16 scrnID)
{
	messagesCurrScrnID=scrnID;
	mmi_trace(g_sw_SMS, "MMI_SMS: SetMessagesCurrScrnID= %d", messagesCurrScrnID);
}

/*****************************************************************************
* FUNCTION
*  SetMessagesScrnIdToDelHistoryNodes
* DESCRIPTION
*   Set MSG screen ID for delete start
*
* PARAMETERS
*  a  IN       start screen ID
* RETURNS
*  none
* GLOBALS AFFECTED
*   g_msg_cntx.MessagesScrnIdToDelHistoryNodes
*****************************************************************************/
void SetMessagesScrnIdToDelHistoryNodes (U16 StartScrnId)
{
	g_msg_cntx.MessagesScrnIdToDelHistoryNodes=StartScrnId;
	mmi_trace(g_sw_SMS, "MMI_SMS: SetMessagesScrnIdToDelHistoryNodes, screen id = %d", g_msg_cntx.MessagesScrnIdToDelHistoryNodes);	
}

/*****************************************************************************
* FUNCTION
*  SetMessagesScrnIdToDelHistoryNodes
* DESCRIPTION
*   Set MSG screen ID for delete start
*
* PARAMETERS
*  a  IN       start screen ID
* RETURNS
*  none
* GLOBALS AFFECTED
*   g_msg_cntx.MessagesScrnIdToDelHistoryNodes
*****************************************************************************/
void SetMessagesScrnIdDelUpToHistoryNodes (U16 StartScrnId)
{
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=StartScrnId;
	mmi_trace(g_sw_SMS, "MMI_SMS: SetMessagesScrnIdToDelHistoryNodes screen id = %d", g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes);	
}


/*****************************************************************************
* FUNCTION
*  DeleteMessagesHistoryNodes
* DESCRIPTION
*   Delete between MSG screens
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DeleteMessagesHistoryNodes (void)
{
	if(IsScreenPresent (g_msg_cntx.MessagesScrnIdToDelHistoryNodes) && IsScreenPresent (g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes))
	{
		if (DeleteBetweenScreen (g_msg_cntx.MessagesScrnIdToDelHistoryNodes, g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes))
		{
			mmi_trace(g_sw_SMS, "MMI_SMS: DeleteMessagesHistoryNodes, Deleteted scrn id from %d to %d", g_msg_cntx.MessagesScrnIdToDelHistoryNodes,g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes);		
			g_msg_cntx.MessagesScrnIdToDelHistoryNodes=MESSAGES_SCR_ID_DEFINES_MAX;
			g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=MESSAGES_SCR_ID_DEFINES_MAX;
			return ;
		}
	}
	mmi_trace(g_sw_SMS, "MMI_SMS: DeleteMessagesHistoryNodes, Not deleteted scrn id from %d to %d", g_msg_cntx.MessagesScrnIdToDelHistoryNodes,g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes);
}


/*****************************************************************************
* FUNCTION
*  Messages2GoBackHistory
* DESCRIPTION
*   Go back two screen history
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void Messages2GoBackHistory (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: Messages2GoBackHistory");
	GoBacknHistory (1);
}

/*****************************************************************************
* FUNCTION
*  Messages4GoBackHistory
* DESCRIPTION
*   Go back four screen history
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void Messages4GoBackHistory (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: Messages4GoBackHistory");
	GoBacknHistory (3);
}

/*****************************************************************************
* FUNCTION
*  GetChildMenuIDIndexByParentMenuID
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN       parent menu id
*  b  IN       child menu id
* RETURNS
*  index >= 0: child manu id is a child of parent menu id, -1: if not
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 GetChildMenuIDIndexByParentMenuID  (U16 parentMenuId, U16 childMenuId)
{
	S32 index=0;
	U16 nMenuItemList[MAX_SUB_MENUS];
	U16	numItems = GetNumOfChild (parentMenuId);
	GetSequenceItemIds (parentMenuId, nMenuItemList);

	for(index=0; index<numItems; index++)
	{
		if(nMenuItemList[index]==childMenuId)
		{
			return index;
		}
	}
	return -1;
}

/*****************************************************************************
* FUNCTION
*  GetHiliteIndex
* DESCRIPTION
*   Get current highlight index
*
* PARAMETERS
*  a  IN       current highlight index
* RETURNS
*  none
* GLOBALS AFFECTED
*   g_msg_cntx.currHiliteIndex
*****************************************************************************/
void GetHiliteIndex (S32 nIndex)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: GetHiliteIndex");
	g_msg_cntx.currHiliteIndex = nIndex;
}

/*****************************************************************************
* FUNCTION
*  GetMsgIndex
* DESCRIPTION
*   Get current highlight message box index
*
* PARAMETERS
*  a  IN       current highlight index
* RETURNS
*  none
* GLOBALS AFFECTED
*   g_msg_cntx.currBoxIndex
*****************************************************************************/
void GetMsgIndex (S32 nIndex)
{
	S32 m_nIndex = nIndex;
	
#ifdef __MMI_VIP_FUNCTION__
	m_nIndex = (S32)g_vip_context.SMSIndexMap[nIndex];
#endif

	g_msg_cntx.currBoxIndex	=	m_nIndex;

#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_SMS_Set_Current_Index(m_nIndex);
#endif
}


#if defined(__MMI_MESSAGES_EMS__)
/*****************************************************************************
* FUNCTION
*  EnableDisableAudioPlayback
* DESCRIPTION
*   Decide to enable/disable audio playback
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EnableDisableAudioPlayback (void)
{
	if(!isInCall())
		EMS_enable_audio_playback ();
	else
		EMS_disable_audio_playback();
}
#endif // defined(__MMI_MESSAGES_EMS__)

/*****************************************************************************
* FUNCTION
*  playIdleShortMessageTone
* DESCRIPTION
*   Play message arrival tone after 700ms
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void playIdleShortMessageTone()		//Added by jinzh:20070704
{
	if (!GetTotalCallCount())
	{
#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		switch(g_msg_cntx.newMsgSimID)
		{
			case 0:
				playRequestedTone (MESSAGE_TONE);
				break;
			case 1:
				playRequestedTone(CARD2_MESSAGE_TONE);
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
		}
#else
		playRequestedTone (MESSAGE_TONE);
#endif
	}
	else
		playRequestedTone (SMS_IN_CALL_TONE);
	
	#ifdef __MMI_MULTI_SIM__
		g_msg_cntx.newMsgSimID = 0;
	#endif
}
/*Jinzh Add Start For 6146 Ver: TBM780  on 2007-8-9 15:38 */
/*****************************************************************************
* FUNCTION
*	SmsStopVibratorThenRing
* DESCRIPTION
*   Play SMS tone After Vibrator
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void SmsStopVibratorThenRing(void)	
{	
	VibratorOff();
	coolsand_UI_cancel_timer(SmsStopVibratorThenRing);
	playIdleShortMessageTone();
}
/*Jinzh Add End  For 6146 Ver: TBM780  on 2007-8-9 15:38 */
/*****************************************************************************
* FUNCTION
*  PlayMessageArrivalTone
* DESCRIPTION
*   Play message arrival tone
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PlayMessageArrivalTone(void)
{
	MMI_ALERT_TYPE alertType;
	alertType = GetMtCallAlertTypeEnum();

	mmi_trace(g_sw_SMS," Func: %s ", __FUNCTION__);
	
	/*Jinzh Modify Start For 6146 Ver: TBM780  on 2007-8-9 15:31 */
	if (alertType == MMI_VIBRATION_AND_RING  || alertType == MMI_RING)
	{
		playIdleShortMessageTone();   //Changed by jinzh:20070704
	}
	if ((alertType == MMI_VIBRATION_ONLY) || (alertType == MMI_VIBRATION_AND_RING) )
	{
	   if((!g_alm_frm_cntx.IsAlmTonePlaying)&&(!GetTotalCallCount()))
			PlaySmsVibratorOnce();
	}
	/*Jinzh Modify End  For 6146 Ver: TBM780  on 2007-8-9 15:31 */

	/*Jinzh Add Start For 6146 Ver: TBM780  on 2007-8-9 15:33 */
	if(alertType == MMI_VIBRATION_THEN_RING)
	{

		if((!g_alm_frm_cntx.IsAlmTonePlaying)&&(!GetTotalCallCount()))
		{
			VibratorOn();
			coolsand_UI_start_timer(2000,SmsStopVibratorThenRing);
		}
	}		
	/*Jinzh Add End  For 6146 Ver: TBM780  on 2007-8-9 15:33 */
}

/*****************************************************************************
* FUNCTION
*  InitMessageProtocolEventHandlers
* DESCRIPTION
*   Init MSG protocol event handler when boot up
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void InitMessageProtocolEventHandlers (void)
{
	mmi_msg_init_msg_data();
	ResetCBInfoStructs();
	mmi_msg_set_protocol_event_handler();
	InitCellBroadcastProtocolEventHanler();//danger!! receiving new CB might cause reading nvram
}

/*****************************************************************************
* FUNCTION
*  InitMessagesApp
* DESCRIPTION
*   Init MSG application when go to opening screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void InitMessagesApp (void)
{
	TRACE_EF(1, "InitMessagesApp");
	InitEMSDataStruct ();
	mmi_msg_set_highlight_handler();
//	mmi_msg_set_protocol_event_handler_after_init ();
	InitCellBroadcastApp();
}

/*****************************************************************************
* FUNCTION
*  DeInitMessagesApp
* DESCRIPTION
*   Deinit MSG application
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DeInitMessagesApp (void)
{
#if defined(__MMI_MESSAGES_TEMPLATE__)
	DeInitTemplates ();
#endif // defined(__MMI_MESSAGES_TEMPLATE__)
	mmi_msg_deinit_msg_data();	
	DeInitCellBroadcastApp ();
	DeInitVoiceMailApp ();
	DeInitEMSDataStruct ();
}

/*****************************************************************************
* FUNCTION
*  RestoreMessagesSettings
* DESCRIPTION
*   Restore MSG settings
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void RestoreMessagesSettings (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: RestoreMessagesSettings");
	mmi_msg_restore_cb ();
}

/*****************************************************************************
* FUNCTION
*  IsMessagesReEntering
* DESCRIPTION
*   Check if MSG application is reentered
*
* PARAMETERS
*  none
* RETURNS
*  TRUE: MSG application is reentered, FALSE: MSG application is not reentered
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 IsMessagesReEntering (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: IsMessagesReEntering");
	if (!mmi_frm_sms_check_action_pending())
	{
		U16 startScrnId=0;
		U16 endScrnId=0;
		mmi_trace(g_sw_SMS, "MMI_SMS: Messages not Busy");
		
		#ifdef MMS_SUPPORT
		if ((IsScreenPresent(WAP_SCREEN_WIDGET))&&(!IsScreenPresent(SCR_ID_MSG_MAIN_MENU_NO_MMS)))
		{
		U16 ScrnId=0;
		mmi_trace(g_sw_SMS, "MMI_SMS: WAP_SCREEN_WIDGET exists and SCR_ID_MSG_MAIN_MENU_NO_MMS not exists");
		
		if(IsScreenPresent(SCR_ID_MSG_SMS_MAIN_MENU))
			ScrnId=SCR_ID_MSG_SMS_MAIN_MENU;
                else if(IsScreenPresent(SCR_ID_MSG_VOICE_MAIL_LIST))
                        ScrnId=SCR_ID_MSG_VOICE_MAIL_LIST;
		else if(IsScreenPresent(SCR_ID_MSG_INBOX_LIST)) 
			ScrnId=SCR_ID_MSG_INBOX_LIST;
		else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_LIST)) 
			ScrnId=SCR_ID_MSG_OUTBOX_LIST;
		else if(IsScreenPresent(SCR_ID_MSG_WRITE)) 
			ScrnId=SCR_ID_MSG_WRITE;
		else if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG)) 
			ScrnId=SCR_ID_MSG_INBOX_MSG;
		else if(IsScreenPresent(SCR_ID_MSG_DEFAULT_MSG))
			ScrnId=SCR_ID_MSG_DEFAULT_MSG;
	#ifdef __MMI_MESSAGES_CHAT__
		else if(IsScreenPresent(CHAT_SCREEN_ROOM_ID))
			ScrnId=CHAT_SCREEN_ROOM_ID;
	#endif
		if(ScrnId)
		{
			mmi_trace(g_sw_SMS, "MMI_SMS: sms screen exists");
			return TRUE;
		}
		return FALSE;
		}
		else
		#endif		
		{
		// get start screen id 
		if(IsScreenPresent(SCR_ID_MSG_MAIN_MENU_NO_MMS))
			startScrnId=SCR_ID_MSG_MAIN_MENU_NO_MMS;
		else if(IsScreenPresent(SCR_ID_MSG_MAIN_MENU))
			startScrnId=SCR_ID_MSG_MAIN_MENU;
                else if(IsScreenPresent(SCR_ID_MSG_VOICE_MAIL_LIST))
                        startScrnId=SCR_ID_MSG_VOICE_MAIL_LIST;
		else if(IsScreenPresent(SCR_ID_MSG_INBOX_LIST)) 
			startScrnId=SCR_ID_MSG_INBOX_LIST;
		else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_LIST)) 
			startScrnId=SCR_ID_MSG_OUTBOX_LIST;
		else if(IsScreenPresent(SCR_ID_MSG_WRITE)) 
			startScrnId=SCR_ID_MSG_WRITE;
		else if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG)) 
			startScrnId=SCR_ID_MSG_INBOX_MSG;
		else if(IsScreenPresent(SCR_ID_MSG_DEFAULT_MSG))
			startScrnId=SCR_ID_MSG_DEFAULT_MSG;
	#ifdef __MMI_MESSAGES_CHAT__
		else if(IsScreenPresent(CHAT_SCREEN_ROOM_ID))
			startScrnId=CHAT_SCREEN_ROOM_ID;
	#endif
		if(startScrnId)
		{
			mmi_trace(g_sw_SMS, "MMI_SMS: Start scrn id to be deleteted %d", startScrnId);
			GetPreviousScrnIdOf (GetCmMarkerScrnID(), &endScrnId);
			if(endScrnId)
			{
				mmi_trace(g_sw_SMS, "MMI_SMS: End scrn id to be deleteted %d", endScrnId);
				if(DeleteBetweenScreen (startScrnId, endScrnId))
				{
					g_msg_cntx.toDisplayMessageList=TO_DISPLAY_MESSAGE_LIST_NONE;
					mmi_frm_sms_delete_action_pending();
					mmi_trace(g_sw_SMS, "MMI_SMS: Deleteted scrn id from %d to %d and action", startScrnId,endScrnId);
					return FALSE;
				}
		        }
		}
		mmi_trace(g_sw_SMS, "MMI_SMS: Not deleteted scrn id from %d to %d", startScrnId,endScrnId);
		return FALSE;
		}
	}
	else
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: Messages state = %d", g_msg_cntx.msg_status);
		return TRUE;
	}
}

/*****************************************************************************
* FUNCTION
*  PreEntryScrMessagesMenuList
* DESCRIPTION
*   Before entry messages main menu, check if MSG application is reentered.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PreEntryScrMessagesMenuList (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: PreEntryScrMessagesMenuList");
	if(IsMessagesReEntering())
		DisplayPopup ((PU8)GetString (STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);		
	else
		/*Wangzl Add Start For 6677 Ver: TBM780  on 2007-9-13 15:34 */
		//EntryScrMessagesMenuList ();
#ifdef __MMI_MESSAGES_MULTI_APP__
		/*Wangzl Add Start For 6677 Ver: TBM780  on 2007-9-13 15:34 */
		//EntryScrMessagesMenuList ();
		EntrySMSMainMenuList();
		/*Wangzl Add End  For 6677 Ver: TBM780  on 2007-9-13 15:34 */
#else
		EntryScrMessagesMenuList ();
#endif
		/*Wangzl Add End  For 6677 Ver: TBM780  on 2007-9-13 15:34 */
}

/*****************************************************************************
* FUNCTION
*  EntryScrMessagesMenuList
* DESCRIPTION
*   Entry messages main menu screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrMessagesMenuList (void)
{ 
	U8* guiBuffer=NULL;
	U16 numItems=0;
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 itemIcons[MAX_SUB_MENUS];
	U16 screenId=SCR_ID_MSG_MAIN_MENU;
#ifdef __MMI_MESSAGES_MULTI_APP__
	//S32 menuItemId=-1;
#else
	U8* popUpList[MAX_SUB_MENUS];
	//S32 menuItemId=-1;
	//S32 menuItemId1=-1;
#endif	
	//S32 maskingByte=-1;
	U8** pPopUpList=NULL;
#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
#endif

#if defined(__MMI_MAINLCD_220X176__) || defined(__MMI_MAINLCD_160X128_)
	if (MTPNP_PFAL_Is_CardValid(0) == 0 && MTPNP_PFAL_Is_CardValid(1) == 0)
	{
		DisplayPopup( (PU8)GetString(SERVICES_NOT_PROVIDED_STRING_ID), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		return;
	};
#endif	
	
	AlmEnableSPOF();
#ifdef MMS_SUPPORT
	if((GetTotalCallCount())||(IsScreenPresent(WAP_SCREEN_WIDGET)))
		screenId=SCR_ID_MSG_MAIN_MENU_NO_MMS;
#endif
	EntryNewScreen(screenId, mmi_msg_exit_generic, EntryScrMessagesMenuList, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: EntryScrMessagesMenuList %d", screenId);
	SetParentHandler(MAIN_MENU_MESSAGES_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	mmi_msg_set_msg_menu_highlight_handler(); 
#ifdef __MMI_MESSAGES_MULTI_APP__
	#if defined(MMS_SUPPORT)
	//menuItemId=GetChildMenuIDIndexByParentMenuID(MAIN_MENU_MESSAGES_MENUID, MESSAGES_MENU_MMS_MENUID);
	//if(menuItemId!=-1)
	//{
		if(((GetTotalCallCount() && !GetWapCallPresent()))||(IsScreenPresent(WAP_SCREEN_WIDGET)))
			//ResetBit(maskingByte,menuItemId);
			mmi_frm_hide_menu_item(MESSAGES_MENU_MMS_MENUID);
		else
			mmi_frm_unhide_menu_item(MESSAGES_MENU_MMS_MENUID);
	//}
	#endif

	#if defined(__MMI_EMAIL__)
	//menuItemId=GetChildMenuIDIndexByParentMenuID(MAIN_MENU_MESSAGES_MENUID, MENU_ID_EMAIL_MAIN);
	//if(menuItemId!=-1)
	//{
		if(GetTotalCallCount() && (!GetWapCallPresent()))	/* CSD call */
			//ResetBit(maskingByte,menuItemId);
			mmi_frm_hide_menu_item(MENU_ID_EMAIL_MAIN);
		else
			mmi_frm_unhide_menu_item(MENU_ID_EMAIL_MAIN);
	//}
	#endif

	#if defined(__MMI_IMPS__)
	//menuItemId=GetChildMenuIDIndexByParentMenuID(MAIN_MENU_MESSAGES_MENUID, MENU_ID_IMPS_MAIN);
	//if(menuItemId!=-1)
	//{
		if(GetTotalCallCount() && (!GetWapCallPresent()))	/* CSD call */
			//ResetBit(maskingByte,menuItemId);
			mmi_frm_hide_menu_item(MENU_ID_IMPS_MAIN);
		else
			mmi_frm_unhide_menu_item(MENU_ID_IMPS_MAIN);
	//}
	#endif
	
#else
	// hint updation
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)!=MMI_FRM_SMS_INVALID_INDEX)
	{
		msgbox_info.totalinbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
		msgbox_info.totaloutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
		msgbox_info.totaldraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);	
	        mmi_msg_set_msg_num_hint (msgbox_info.totalinbox, msgbox_info.totaloutbox, msgbox_info.totaldraftbox);
		RefreshMessagesMenuList();
	}
	ConstructHintsList(MAIN_MENU_MESSAGES_MENUID, popUpList);

	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)!=MMI_FRM_SMS_INVALID_INDEX)
		pPopUpList = popUpList;
	else
		pPopUpList = NULL;


#endif /* MMS_SUPPORT */

	//MaskItems(itemIcons,(U8)numItems,maskingByte);
	//MaskHiliteItems(MAIN_MENU_MESSAGES_MENUID, maskingByte);
	//numItems=MaskItems(nStrItemList,(U8)numItems,maskingByte);

	 
	numItems = GetNumOfChild_Ext(MAIN_MENU_MESSAGES_MENUID); //modified by lin @2007/04/09
	GetSequenceStringIds_Ext(MAIN_MENU_MESSAGES_MENUID, nStrItemList); //modified by lin @2007/04/09
	GetSequenceImageIds_Ext(MAIN_MENU_MESSAGES_MENUID, itemIcons); //modified by lin @2007/04/09

	g_msg_cntx.currBoxIndex=0;
	g_msg_cntx.sendMessageCase=SEND_CASE_SEND_ONLY;
	g_msg_cntx.PhbSmsInterfaceState=MMI_SEND_ONLY;
	SetMessagesCurrScrnID(screenId);
	guiBuffer = GetCurrGuiBuffer(screenId);		
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_MESSAGES_MENUID);
#endif
 
#ifdef __MMI_NUMBER_MENU__
	ShowCategory52Screen(STR_MESSAGE_MAIN_MENU_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION, 
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (PU16)gIndexIconsImageList,
						  (U8 **) pPopUpList, 0, 0,
						  guiBuffer );
#else
	ShowCategory52Screen(STR_MESSAGE_MAIN_MENU_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION, 
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, itemIcons,
						  (U8 **) pPopUpList, 0, 0,
						  guiBuffer );
#endif

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

//	SMS use number	//
static BOOL IsValidPhnumChar(S32 ch);
/*****************************************************************************
* FUNCTION
*  IsAPhoneNumber
* DESCRIPTION
*   Check if input is a number string
*
* PARAMETERS
*  a  IN       string
* RETURNS
*  1: input is a number string, 0: input is not a number string
* GLOBALS AFFECTED
*   none
*****************************************************************************/
int IsAPhoneNumber (S8 * number)
{
	int c = *number;
	mmi_trace(g_sw_SMS, "MMI_SMS: IsAPhoneNumber");
	if (c != '+' && !IS_NUM(c)) 
		return 0;
	do 
	{
		number += ENCODING_LENGTH;
		c = *number;
		if( c && !IS_NUM(c) && !IsValidPhnumChar(c))
			return 0;
	}
	while (c);
	return 1;
}

/*****************************************************************************
* FUNCTION
*  IsValidPhnumChar
* DESCRIPTION
*   Some special characters should be in use number list as well
*
* PARAMETERS
*  a  IN       character
* RETURNS
*  1: input is a number string, 0: input is not a number string
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static BOOL IsValidPhnumChar(S32 ch)
{
	BOOL ret_value;

	switch(ch)
	{
	case '+':
	case '*':
	case '#':
	case 'p':
	case 'P':
	case 'w':
	case 'W':
		ret_value = TRUE;
		break;
	default :
		ret_value = FALSE;
		break;
	}
	return ret_value;
}

/*****************************************************************************
* FUNCTION
*  GetPhoneNumber
* DESCRIPTION
*   Get phone number if input is not a number string
*
* PARAMETERS
*  a  IN       string
* RETURNS
*  number string
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S8* GetPhoneNumber (S8 * number)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: GetPhoneNumber");
	if (number && !IsAPhoneNumber(number))
	{
		number = lookUpName(number);
	}
	return number;
}

/*****************************************************************************
* FUNCTION
*  AlreadyExists
* DESCRIPTION
*   Check if input number string already exists in use number list
*
* PARAMETERS
*  a  IN       number string
*  b  IN       current use number amount
* RETURNS
*  1: input string exists in use number list, 0: input string doesn't exist in use number list
* GLOBALS AFFECTED
*   none
*****************************************************************************/
int AlreadyExists (S8* thisNum, int n)
{
	n--;
	for (; n >= 0; n--)
	{
		if (!pfnUnicodeStrcmp(thisNum, g_msg_cntx.numbersList[n]))
			return 1;
	}
	return 0;
}

/*****************************************************************************
* FUNCTION
*  GetAsciiOrUCS2Char
* DESCRIPTION
*   Get Ascii or UCS2 character
*
* PARAMETERS
*  a  IN       address of string
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S16 GetAsciiOrUCS2Char (S8 * buf)
{
	return (*buf+((*(buf+1))<<8)) ;
}

/*****************************************************************************
* FUNCTION
*  GetAsciiOrUCS2Char_RV
* DESCRIPTION
*   Get Ascii or UCS2 character
*
* PARAMETERS
*  a  IN       address of string
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S16 GetAsciiOrUCS2Char_RV (U8 * buf)
{
	return (*(buf+1)+((*buf)<<8)) ;
}

/*****************************************************************************
* FUNCTION
*  PutAsciiOrUCS2Char
* DESCRIPTION
*   Put Ascii or UCS2 chatacter
*
* PARAMETERS
*  a  IN/OUT      address of string
*  b  IN       input character
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PutAsciiOrUCS2Char (S8 * buf, S16 c)
{
	*buf++ = c & 0xff ;
	*buf = c >> 8 ;
}

/*****************************************************************************
* FUNCTION
*  IsSymbolValid
* DESCRIPTION
*   Check if symbol is valid to insert use number
*
* PARAMETERS
*  a  IN       input character
*  b  IN       bracket waiting flag
*  c  IN       current use number record length
* RETURNS
*  1: symbol is valid, 0: symbol is not valid
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static U8 IsSymbolValid (int c, U8 inBracket, U8 numIndex)
{
	U8  status = 0;

	if (IS_NUM(c)) return 1;

	numIndex = numIndex / ENCODING_LENGTH;
	
	switch (c)
	{
		case '+':
			if (numIndex == 0)
				status = 1;
			break;
		case '(':
			status = 1;
			break;
		case ')':
			if (inBracket)
				status = 1;
			break;
		case 'p':
		case 'w':
		case '-':
			if (numIndex >= USE_NUM_MIN_LENGTH)
				status = 1;
			break;
		default:
			status = 0;
	}

	return status;
}

/*****************************************************************************
* FUNCTION
*  TruncateLastUnwantedSymbols
* DESCRIPTION
*   Truncate use number last unwanted symbols
*
* PARAMETERS
*  a  IN/OUT      address of string
*  b  IN       string length
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static void TruncateLastUnwantedSymbols (S8 * buf, int len)
{
	int i,last=0;
	int c;

	for (i = 0; i < len; i += ENCODING_LENGTH)
	{
		c = GetAsciiOrUCS2Char (& buf[i]);
		if ((c != 'p') && (c != 'w') && (c != '-'))
			last = i;
	}
	if (i > last) last += ENCODING_LENGTH;
	PutAsciiOrUCS2Char (& buf[last], '\0');
}

/*****************************************************************************
* FUNCTION
*  ObtainNumbersFromMessageText
* DESCRIPTION
*   Obtain number list from message content
*
* PARAMETERS
*  a  IN       message phone number
*  b  IN       message content
*  c  IN       message length
*  d  IN       number list start index
* RETURNS
*  current use number list amount
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 ObtainNumbersFromMessageText(char * phNumber, U8 * smsText, U16 bufLen, U8 startindex)
{
	int    i;
	int    c;
	S8    thisNum [(MAX_DIGITS+1)*ENCODING_LENGTH] ;
	int    pickNow = 0;
	U8    inBracket=0;
	int    thisNumIndex=0;	
	int	currNumListIndex=startindex;

	if(pfnUnicodeStrlen((S8*)phNumber))
	{
	phNumber = GetPhoneNumber (phNumber);

	if (phNumber)
	{
		if (g_msg_cntx.numbersList[currNumListIndex])
			OslMfree (g_msg_cntx.numbersList[currNumListIndex]);

		g_msg_cntx.numbersList[currNumListIndex] = OslMalloc((pfnUnicodeStrlen(phNumber)+2)*ENCODING_LENGTH) ;
		pfnUnicodeStrcpy((S8*)g_msg_cntx.numbersList[currNumListIndex], phNumber);
		currNumListIndex ++ ;
	}
	}
	if(currNumListIndex==MAX_USE_NUMBERS)
		return (U16)currNumListIndex;

	for (i = 0; i < bufLen;)
	{
		c = GetAsciiOrUCS2Char((S8*)& smsText[i]) ;
		i += ENCODING_LENGTH ;
		if (IsSymbolValid (c,inBracket,(U8)thisNumIndex) && (thisNumIndex < (MAX_DIGITS-1)*ENCODING_LENGTH))
		{
			if ((c!='-')&&(c!='(')&&(c!=')'))
			{
				PutAsciiOrUCS2Char (& thisNum[thisNumIndex], (S16) c) ;
				thisNumIndex += ENCODING_LENGTH;
			}
			if (c == '(') inBracket++;
			else if ((c == ')') && inBracket) inBracket--;
		}
		else
		{
			pickNow = 1;
			while (thisNumIndex && (i < bufLen))
			{
				if (!IS_NUM(c)) 
				{
					/* to handle the scenario number'+'number  ex. "123+123"  should be parsed as 123 and +123, instead of 123*/
					if(c=='+')
						i -= ENCODING_LENGTH;
				}
                                /* a stream of number that exceeds 40 digits, the number starting from 41 digits will become the next use number */
				else /* c is a num */
					i -= ENCODING_LENGTH;
				break;
			 	// c = GetAsciiOrUCS2Char((S8*) & smsText[i]);
				// i += ENCODING_LENGTH;
			}
		}

		if (i >= bufLen)
			pickNow = 1;

		if (pickNow)
		{
			int c1=0;

			pickNow = 0;
			if (thisNumIndex && (((c1 = GetAsciiOrUCS2Char((S8*)& thisNum[thisNumIndex-ENCODING_LENGTH])) == 'w')
							|| (c1 == 'p')))
				TruncateLastUnwantedSymbols (thisNum, thisNumIndex);
			
			PutAsciiOrUCS2Char (& thisNum[thisNumIndex], '\0') ;
			if (!AlreadyExists(thisNum, currNumListIndex))
			{
				if (thisNumIndex >= (USE_NUM_MIN_LENGTH*ENCODING_LENGTH))
				{
					if (g_msg_cntx.numbersList[currNumListIndex])
						OslMfree (g_msg_cntx.numbersList[currNumListIndex]);

					g_msg_cntx.numbersList[currNumListIndex] = OslMalloc((pfnUnicodeStrlen(thisNum)+2)*ENCODING_LENGTH) ;
					pfnUnicodeStrcpy((S8*)g_msg_cntx.numbersList[currNumListIndex], thisNum);
					currNumListIndex ++ ;
					if (currNumListIndex == MAX_USE_NUMBERS)
						break;
				}
			}
			thisNumIndex = 0;
		}
	}
	return (U16)currNumListIndex;
}

#ifdef __MMI_MESSAGES_CHAT__
/*****************************************************************************
* FUNCTION
*  ObtainChatNumbersFromMessageTextWrapper
* DESCRIPTION
*   Obtain number list from chat message content
*
* PARAMETERS
*  a  IN       message phone number
*  b  IN       message room index
* RETURNS
*  current use number list amount
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 ObtainChatNumbersFromMessageTextWrapper(char * phNumber,  S16 chatroomindex)
{
	U16 bufLen=0;
	U8 * smsText=NULL;
	S16 j;
	U16 currindex=0;
	for(j=0;j<GetNumOfMessage(chatroomindex);j++)
	{
		smsText=GetMemoryPointer(j,chatroomindex);
		bufLen=(S16)pfnUnicodeStrlen((PS8)smsText);
		bufLen=bufLen*ENCODING_LENGTH;
		if(j==0)
			currindex=ObtainNumbersFromMessageText(phNumber, smsText, bufLen, 0);
		else
			currindex=ObtainNumbersFromMessageText(NULL, smsText, bufLen, (U8)currindex);
		if(currindex==MAX_USE_NUMBERS)
			return currindex;
	}
        /* chat room message content is empty but the destination number should still be displayed in the use number list */
	if (GetNumOfMessage(chatroomindex)==0)
	{
		if(pfnUnicodeStrlen((S8*)phNumber))
		{
			phNumber = GetPhoneNumber (phNumber);
			if (phNumber)
			{
				if (g_msg_cntx.numbersList[currindex])
					OslMfree (g_msg_cntx.numbersList[currindex]);

				g_msg_cntx.numbersList[currindex] = OslMalloc((pfnUnicodeStrlen(phNumber)+2)*ENCODING_LENGTH) ;
				pfnUnicodeStrcpy((S8*)g_msg_cntx.numbersList[currindex], phNumber);
				currindex++ ;
			}
		}
	}
	return currindex;
}
/*****************************************************************************
* FUNCTION
*  GetNewChatSmsIndDisplayStr
* DESCRIPTION
*   Get chat message indication string for dislpay
*
* PARAMETERS
*  none
* RETURNS
*  NewChatMessageFromXxxStr
* GLOBALS AFFECTED
*   none
*****************************************************************************/
PU8 GetNewChatSmsIndDisplayStr (void)
{
	PRINT_INFORMATION (("*-------[MessagesIdleApp.c] GetNewSmsIndDisplayStr -------*\n"));
	return NewChatMessageFromXxxStr;
}

/*****************************************************************************
* FUNCTION
*  GetNewChatMsgIndDisplayString
* DESCRIPTION
*   Compose new chat message indication string
*
* PARAMETERS
*  a  IN       message phone number
*  b  IN       message number length
*  c  IN       message number type
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GetNewChatMsgIndDisplayString (U8 *number, U8 length, U8 type)
{
	PRINT_INFORMATION (("*-------[MessagesMiscell.c] GetNewMsgIndDisplayString -------*\n"));
	memset(&NewChatMessageFromXxxStr, 0, sizeof(NewChatMessageFromXxxStr));
	if(length)
	{
		PS8 phbName=NULL;
		PU8 pPhoneNumberPtr = ConvertL4NumberStructToDisplay (number,length,type);
		phbName = lookUpNumber ((S8*)pPhoneNumberPtr);
		if(phbName!=NULL){
			if( pfnUnicodeStrcmp(phbName,""))
					pfnUnicodeStrncpy((PS8)NewChatMessageFromXxxStr,(PS8) phbName, (U32)(sizeof(NewChatMessageFromXxxStr)/ENCODING_LENGTH)-1);
			else 
					pfnUnicodeStrncat ((PS8)NewChatMessageFromXxxStr, (PS8)pPhoneNumberPtr, (U32)(sizeof(NewChatMessageFromXxxStr)/ENCODING_LENGTH)-1);
		}
		else
			pfnUnicodeStrncat ((PS8)NewChatMessageFromXxxStr, (PS8)pPhoneNumberPtr, (U32)(sizeof(NewChatMessageFromXxxStr)/ENCODING_LENGTH)-1);
		if (pPhoneNumberPtr)
		{
			OslMfree (pPhoneNumberPtr);
			pPhoneNumberPtr = NULL;
		}
	}
}

/*****************************************************************************
* FUNCTION
*  DeleteChatMsg
* DESCRIPTION
*   Delete chat message
*
* PARAMETERS
*  a  IN 	message index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DeleteChatMsg(S32 index)
{
	g_msg_cntx.msg_ind_chat_index=(U16)index;
}

/*****************************************************************************
* FUNCTION
*  GetConcatenatedMessage
* DESCRIPTION
*   Concatenate message for chat room screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GetConcatenatedMessage(void)
{
	EMSData * pEMSdata ;

	GetEMSDataForView (& pEMSdata, 1); // force to clear old content and initialize

	EMSUnPack(pEMSdata,1,PendingSaveSendData.totalSegments,
                         PendingSaveSendData.TPUD_p,
                        PendingSaveSendData.TPUDLen);
	return;
}
#endif


#ifdef __MMI_MESSAGES_TEMPLATE__
/*****************************************************************************
* FUNCTION
*  ReadTemplateFromNVRAM
* DESCRIPTION
*   Read template from NVRAM
*
* PARAMETERS
*  a  IN       index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*Guojian Modify Start For 10181 Ver: GreenStone  on 2008-10-26 16:13 too slowly to load SMS templates*/
#ifdef NEVER
void ReadTemplateFromNVRAM (U16 index)
{
	S16	error;

	if (index >= NVRAM_SMS_RECORD_TOTAL)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: ERROR: ReadTemplateFromNVRAM: index out of bound");
		return ;
	}

	memset (g_msg_cntx.scratchTemplRec, 0 , NVRAM_SMS_TEMPL_RECORD_SIZE+ENCODING_LENGTH);
	if (ReadRecord (NVRAM_EF_SMS_LID, (U16)(index+1), g_msg_cntx.scratchTemplRec, NVRAM_SMS_TEMPL_RECORD_SIZE, &error) < 0)
		g_msg_cntx.scratchTemplRec[0] = '\0' ;
	return;
}
#endif /* NEVER */
void ReadTemplateFromNVRAM (U16 index, S8* strbuf)
{
	S16	error;

	if (index >= NVRAM_SMS_RECORD_TOTAL)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: ### ERROR: ReadTemplateFromNVRAM: index out of bound");
		return ;
	}
	if (ReadRecord (NVRAM_EF_SMS_LID, (U16)(index+1+1), strbuf, NVRAM_SMS_TEMPL_RECORD_SIZE * NUM_TEMPLATES, &error) < 0)
	{
		memset(strbuf, 0, NVRAM_SMS_TEMPL_RECORD_SIZE * NUM_TEMPLATES + ENCODING_LENGTH);
	}
	return;
}
/*Guojian Modify End  For 10181 Ver: GreenStone  on 2008-10-26 16:13 */

/*****************************************************************************
* FUNCTION
*  WriteTemplateToNVRAM
* DESCRIPTION
*   Write template to NVRAM
*
* PARAMETERS
*  a  IN       index
* RETURNS
*  TRUE: write success, FALSE: write fail
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*Guojian Modify Start For 10181 Ver: GreenStone  on 2008-10-26 16:13 too slowly to load SMS templates*/
#ifdef NEVER
U8 WriteTemplateToNVRAM (U16 index)
{
	S16	error;

	if (index >= NVRAM_SMS_RECORD_TOTAL)
		return FALSE;
	
	if (WriteRecord (NVRAM_EF_SMS_LID, (U16)(index+1+1), g_msg_cntx.scratchTemplRec, NVRAM_SMS_TEMPL_RECORD_SIZE, & error) != NVRAM_SMS_TEMPL_RECORD_SIZE)
		return FALSE;
	return TRUE;
}
#endif /* NEVER */
U8 WriteTemplateToNVRAM (U16 index, S8* str)
{
	S16	error;

	if (index >= NVRAM_SMS_RECORD_TOTAL)
		return FALSE;
	
	if (WriteRecord (NVRAM_EF_SMS_LID, (U16)(index+1+1), str, NVRAM_SMS_TEMPL_RECORD_SIZE * NUM_TEMPLATES, & error) != NVRAM_SMS_TEMPL_RECORD_SIZE * NUM_TEMPLATES)
		return FALSE;
	return TRUE;
}
/*Guojian Modify End  For 10181 Ver: GreenStone  on 2008-10-26 16:13 */

#if defined(_MUTILANG_TEMPLATE_)||defined(__MMI_MESSAGES_SIMCHINESE_TEMPLATE__)
/*****************************************************************************
* FUNCTION
*  SetTemplateLanguage
* DESCRIPTION
*   Set template language
*
* PARAMETERS
*  a  IN       language type
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SetTemplateLanguage (U8 langtype)
{
	U8	i=0;
	S16 	error ;
	S8	tempLang[SSC_SIZE];
	
	memset(tempLang, 0, SSC_SIZE);
	switch(langtype)
	{
		case MSG_TEMPLATE_LANG_ENGLISH:
			memcpy(tempLang, SSC_ENGLISH, SSC_SIZE);
			break;
		#ifdef __MMI_LANG_SM_CHINESE__
		case MSG_TEMPLATE_LANG_SIMCHINESE:
			memcpy(tempLang, SSC_SCHINESE, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_FRENCH__
		case MSG_TEMPLATE_LANG_FRENCH:
			memcpy(tempLang,SSC_FRENCH,SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_TURKISH__
		case MSG_TEMPLATE_LANG_TURKISH:
			memcpy(tempLang, SSC_TURKISH, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_INDONESIAN__
		case MSG_TEMPLATE_LANG_INDONESIAN:
			memcpy(tempLang, SSC_INDONESIAN, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_ARABIC__
		case MSG_TEMPLATE_LANG_ARABIC:
			memcpy(tempLang,SSC_ARABIC,SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_PERSIAN__
        		case MSG_TEMPLATE_LANG_PERSIAN:
			memcpy(tempLang, SSC_PERSIAN, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_RUSSIAN__
		case MSG_TEMPLATE_LANG_RUSSIAN:
			memcpy(tempLang,SSC_RUSSIAN,SSC_SIZE); 
			break;
		#endif
		#ifdef __MMI_LANG_VIETNAMESE__
		case MSG_TEMPLATE_LANG_VIETNAMESE:
			memcpy(tempLang, SSC_VIETNAMESE, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_TR_CHINESE__
		case MSG_TEMPLATE_LANG_TRACHINESE:
			memcpy(tempLang, SSC_TCHINESE, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_THAI__
		case MSG_TEMPLATE_LANG_THAI:
			memcpy(tempLang, SSC_THAI, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_ITALIAN__
		case MSG_TEMPLATE_LANG_ITALIAN:
			memcpy(tempLang, SSC_ITALIAN, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_HINDI__
		case MSG_TEMPLATE_LANG_HINDI:
			memcpy(tempLang, SSC_HINDI, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_GERMAN__
		case MSG_TEMPLATE_LANG_GERMAN:
			memcpy(tempLang, SSC_GERMAN, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_SPANISH__
		case MSG_TEMPLATE_LANG_SPANISH:
			memcpy(tempLang, SSC_SPANISH, SSC_SIZE);
			break;
		#endif
		#ifdef __MMI_LANG_MALAY__
		case MSG_TEMPLATE_LANG_MALAY:
			memcpy(tempLang, SSC_MALAY, SSC_SIZE);
			break;
		#endif		
	}
	for(i=0;i<MAX_LANGUAGES;i++)
	{
	   if(strcmp((PS8)gLanguageArray[i].aLangSSC,(S8*)tempLang)==0)
	   {
	   	if( gCurrLangIndex != i )
	      {
			SetCurrentLanguage(i);
			WriteValue( NVRAM_SETTING_LANG, &i, DS_BYTE , &error);
			break;
	      }
	   }
	}
}

/*****************************************************************************
* FUNCTION
*  UnsetTemplateLanguage
* DESCRIPTION
*   Unset template language
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void UnsetTemplateLanguage (void)
{
	U8 i=0;
	S16 	error ;
	for(i=0;i<MAX_LANGUAGES;i++)
	{
	   if(strcmp((PS8)gLanguageArray[i].aLangSSC,(PS8)currLang)==0)
	   {
	   	if( gCurrLangIndex != i )
	      {
		  	SetCurrentLanguage(i);
			WriteValue( NVRAM_SETTING_LANG, &i, DS_BYTE , &error);
			break;
	      }
	   }
	}
}
#endif

/*****************************************************************************
* FUNCTION
*  WriteDefaultTemplateToNVRAM
* DESCRIPTION
*   Write default template to NVRAM
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*Guojian Modify Start For 10181 Ver: GreenStone  on 2008-10-26 16:13 too slowly to load SMS templates*/
#ifdef NEVER
U8 WriteDefaultTemplateToNVRAM (void)
{
	U16 i=0;
	S8* tmpl_str;
	U8 result;
	for (i = 0; i < NUM_TEMPLATES; i++)
	{
		U16 templateindex = i + CalTemplateBase();
		tmpl_str = GetString(templateStrings[i]);
		memset(g_msg_cntx.scratchTemplRec, 0, NVRAM_SMS_TEMPL_RECORD_SIZE+ENCODING_LENGTH);
		if(tmpl_str)
			memcpy(g_msg_cntx.scratchTemplRec, tmpl_str, (pfnUnicodeStrlen(tmpl_str)+1)*ENCODING_LENGTH);
		result=WriteTemplateToNVRAM (templateindex);
		if(result==FALSE)
			return FALSE;
	}
	return TRUE;
}
#endif /* NEVER */
U8 WriteDefaultTemplateToNVRAM (void)
{
	U16 i=0;
	S8* p_temp;
	S8 tmpl_str[NUM_TEMPLATES * NVRAM_SMS_TEMPL_RECORD_SIZE];	
	U8 result;

	U16 templateindex = CalTemplateBase();
	memset(tmpl_str, 0, NVRAM_SMS_TEMPL_RECORD_SIZE*NUM_TEMPLATES);
	for (i = 0; i < NUM_TEMPLATES; i++)
	{	
		p_temp = GetString(templateStrings[i]);
		memcpy(tmpl_str+(i*NVRAM_SMS_TEMPL_RECORD_SIZE), p_temp, (pfnUnicodeStrlen(p_temp)+1)*ENCODING_LENGTH);
	}
		result=WriteTemplateToNVRAM (templateindex, (S8*)tmpl_str);
		if(result==FALSE)
			return FALSE;

	return TRUE;
}
/*Guojian Modify End  For 10181 Ver: GreenStone  on 2008-10-26 16:13 */

/*****************************************************************************
* FUNCTION
*  InitTemplates
* DESCRIPTION
*   Init templates
*
* PARAMETERS
*  none
* RETURNS
*  TRUE: init success, FALSE: init fail
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 InitTemplates(void)
{
	static int 		initDone = 0;
	static S8		templateSignature [NVRAM_SMS_TEMPL_RECORD_SIZE+1] = "AAA" ;
	S8 			buffer [NVRAM_SMS_TEMPL_RECORD_SIZE] ;
	S16 			error ;
	int			i=0;


	if (initDone)
		return TRUE;

	initDone = 1;


	if (ReadRecord (NVRAM_EF_SMS_LID, 1, buffer, NVRAM_SMS_TEMPL_RECORD_SIZE, & error)<0)
		buffer[0] = '\0' ;

	if (buffer[0] == 'A')
		return TRUE;
	else
		if (WriteRecord (NVRAM_EF_SMS_LID, 1, templateSignature, NVRAM_SMS_TEMPL_RECORD_SIZE, & error) < NVRAM_SMS_TEMPL_RECORD_SIZE)
			return FALSE;

#if defined (_MUTILANG_TEMPLATE_)||defined (__MMI_MESSAGES_SIMCHINESE_TEMPLATE__)
	memset(currLang, 0, SSC_SIZE);
	memcpy(currLang, (PS8)gLanguageArray[gCurrLangIndex].aLangSSC, SSC_SIZE);
#ifdef _MUTILANG_TEMPLATE_
	for(i=0; i<TOTAL_TEMPLATE_LANGUAGE; i++)
	{
		SetTemplateLanguage((U8)i);
		if(!WriteDefaultTemplateToNVRAM())
		{
			UnsetTemplateLanguage();
			return FALSE;
		}
	}
#else
	SetTemplateLanguage(MSG_TEMPLATE_LANG_SIMCHINESE);
	if(!WriteDefaultTemplateToNVRAM())
	{
		return FALSE;
	}
#endif

	UnsetTemplateLanguage();
#else //defined (_MUTILANG_TEMPLATE_)||defined (__MMI_MESSAGES_SIMCHINESE_TEMPLATE__)
	if(!WriteDefaultTemplateToNVRAM())
		return FALSE;
#endif
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  DeInitTemplates
* DESCRIPTION
*   Deinit templates
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*Guojian Modify Start For 10181 Ver: GreenStone  on 2008-10-26 16:12 too slowly to load SMS templates*/
#ifdef NEVER
void DeInitTemplates (void)
{
	int  i;
	for (i = 0; i < NUM_TEMPLATES; i++)
	{
		if (g_msg_cntx.templates[i])
		{
			OslMfree (g_msg_cntx.templates[i]);
			g_msg_cntx.templates[i] = NULL ;
		}	
	}
}
#endif /* NEVER */
void DeInitTemplates (void)
{
	int  i;
	OslMfree (g_msg_cntx.templates[0]);
	for (i = 0; i < NUM_TEMPLATES; i++)
	{
			g_msg_cntx.templates[i] = NULL ;
	}
}
/*Guojian Modify End  For 10181 Ver: GreenStone  on 2008-10-26 16:12 */

/*****************************************************************************
* FUNCTION
*  LoadTemplatesInDisplayList
* DESCRIPTION
*   Load templates to display list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*Guojian Modify Start For 10181 Ver: GreenStone  on 2008-10-26 16:12 too slowly to load SMS templates*/
#ifdef NEVER
void LoadTemplatesInDisplayList (void)
{
	U16  i;
	
	DeInitTemplates();
	for (i = 0; i < NUM_TEMPLATES; i++)
	{
		U16 templateindex = i+CalTemplateBase()+1;
		ReadTemplateFromNVRAM (templateindex);
		g_msg_cntx.templates[i] = OslMalloc(NVRAM_SMS_TEMPL_RECORD_SIZE+ENCODING_LENGTH);
		pfnUnicodeStrcpy(g_msg_cntx.templates[i], g_msg_cntx.scratchTemplRec);
		
	}
}
#endif /* NEVER */
void LoadTemplatesInDisplayList (void)
{
	U16  i =  0;
	S8 *p_temp;
	U16 templateindex = CalTemplateBase();

	DeInitTemplates();
	p_temp = OslMalloc(NVRAM_SMS_TEMPL_RECORD_SIZE*NUM_TEMPLATES + ENCODING_LENGTH);
	memset(p_temp, 0, NVRAM_SMS_TEMPL_RECORD_SIZE*NUM_TEMPLATES + ENCODING_LENGTH);
	ReadTemplateFromNVRAM (templateindex, p_temp);

 	for (i = 0; i < NUM_TEMPLATES; i++)
	{
		g_msg_cntx.templates[i] = p_temp + i*NVRAM_SMS_TEMPL_RECORD_SIZE;
	}
}
/*Guojian Modify End  For 10181 Ver: GreenStone  on 2008-10-26 16:12 */

/*****************************************************************************
* FUNCTION
*  GetTemplatesToEdit
* DESCRIPTION
*   Get templates to edit.
*
* PARAMETERS
*  a  IN/OUT      address of string
*  b  IN       current highlight index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GetTemplatesToEdit (S8* buffer, U32 index)
{
	if (g_msg_cntx.templates[index])
		pfnUnicodeStrcpy ((PS8)buffer,(PS8)g_msg_cntx.templates[index]);
	else
		buffer[0] = buffer [1] = '\0' ;
}

/*****************************************************************************
* FUNCTION
*  DeleteTemplate
* DESCRIPTION
*   Delete template
*
* PARAMETERS
*  a  IN       index
* RETURNS
*  TRUE: delete success, FALSE: delete fail
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*Guojian Modify Start For 10181 Ver: GreenStone  on 2008-10-26 16:10  too slowly to load SMS templates*/
#ifdef NEVER
U8 DeleteTemplate(U32 index)
{
	U16 templateindex = (U16)index+CalTemplateBase();
	U8 result;
	memset(g_msg_cntx.scratchTemplRec, 0, NVRAM_SMS_TEMPL_RECORD_SIZE+ENCODING_LENGTH);
	result=WriteTemplateToNVRAM (templateindex);
	return result;
}
#endif /* NEVER */
U8 DeleteTemplate(U32 index)
{
	U8 result = 0;
	U16 templateindex = CalTemplateBase();
	memset(g_msg_cntx.templates[index], 0, NVRAM_SMS_TEMPL_RECORD_SIZE);
	result=WriteTemplateToNVRAM (templateindex,g_msg_cntx.templates[0]);
	return result;
}
/*Guojian Modify End  For 10181Ver: GreenStone  on 2008-10-26 16:10 */

/*****************************************************************************
* FUNCTION
*  StoreTemplate
* DESCRIPTION
*   Store templates
*
* PARAMETERS
*  a  IN       index
*  b  IN       address of string
* RETURNS
*  TRUE: store success, FALSE: store fail
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#ifdef NEVER
U8 StoreTemplate (U32 index)
{
	U16 templateindex = (U16)index+CalTemplateBase();
	U8 result;
	result=WriteTemplateToNVRAM (templateindex);
	return result;
}
#endif /* NEVER */
U8 StoreTemplate (U32 index)
{
	U8 result = 0;
	U16 templateindex = CalTemplateBase();
	pfnUnicodeStrcpy ((PS8)g_msg_cntx.templates[index],g_msg_cntx.scratchTemplRec);
	result=WriteTemplateToNVRAM (templateindex,g_msg_cntx.templates[0]);
	return result;
}

/*****************************************************************************
* FUNCTION
*  CalTemplateBase
* DESCRIPTION
*   Calculate template base
*
* PARAMETERS
*  none
* RETURNS
*  template base
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 CalTemplateBase (void)
{
#ifdef _MUTILANG_TEMPLATE_
	U8 	tempLangOffset=0;
	U16 	tempBase=0;
	#ifdef __MMI_LANG_TR_CHINESE__
	if (IsTrChineseSet())
		tempLangOffset=(U8)MSG_TEMPLATE_LANG_TRACHINESE;
	else
	#endif
	#ifdef __MMI_LANG_SM_CHINESE__
	if (IsSmChineseSet())
		tempLangOffset=(U8)MSG_TEMPLATE_LANG_SIMCHINESE;
	else
	#endif
	#ifdef __MMI_LANG_FRENCH__
	if(IsFrenchSet())
		tempLangOffset=(U8)MSG_TEMPLATE_LANG_FRENCH;
	else
	#endif
	#ifdef __MMI_LANG_TURKISH__
	if(IsTurkishSet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_TURKISH;
	else
	#endif
	#ifdef __MMI_LANG_INDONESIAN__
	if(IsIndonesianSet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_INDONESIAN;
	else
	#endif
	#ifdef __MMI_LANG_ARABIC__
	if(IsArabicSet())
		tempLangOffset=(U8)MSG_TEMPLATE_LANG_ARABIC;
	else
	#endif
	#ifdef __MMI_LANG_PERSIAN__
	if(IsPersianSet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_PERSIAN;
	else
	#endif
	#ifdef __MMI_LANG_RUSSIAN__
	if(IsRussianSet())
		tempLangOffset=(U8)MSG_TEMPLATE_LANG_RUSSIAN;
	else
	#endif
	#ifdef __MMI_LANG_VIETNAMESE__
	if(IsVietnameseSet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_VIETNAMESE;
	else
	#endif
//kecx added on 20080714
	#ifdef __MMI_LANG_THAI__
	if(IsThaiSet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_THAI;
	else
	#endif
//kecx added on 20080714
	#ifdef __MMI_LANG_ITALIAN__
	if(IsItalianSet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_ITALIAN;
	else
	#endif
	#ifdef __MMI_LANG_HINDI__
	if(IsHindiSet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_HINDI;
	else
	#endif
	#ifdef __MMI_LANG_GERMAN__
	if(IsGermanSet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_GERMAN;
	else
	#endif
	#ifdef __MMI_LANG_SPANISH__
	if(IsSpanishSet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_SPANISH;
	else
	#endif
	#ifdef __MMI_LANG_MALAY__
	if(IsMalaySet())
		tempLangOffset = (U8)MSG_TEMPLATE_LANG_MALAY;
	else
	#endif
	tempLangOffset=(U8)MSG_TEMPLATE_LANG_ENGLISH;
	tempBase=tempLangOffset*NUM_TEMPLATES;
	return tempBase;
#else
	return 0;
#endif
}

#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_check_interrupt
* DESCRIPTION
*  check if there is an interrupt (incoming call ringing or alarm playing).
* PARAMETERS
*  none
* RETURNS
*  1: interrupt existed, 0: interrupt not existed
* GLOBALS AFFECTED
*   none
*****************************************************************************/

U8 mmi_msg_check_interrupt(void)
{
	if ((S16)GetIncomingCallHandle() != -1 || AlmIsTonePlaying())
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_check_interrupt return TRUE");
		return TRUE;
	}
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_need_clear_action_queue
* DESCRIPTION
*  check if the action queue is needed to be cleared to avoid the case that the screen 
*  is back to idle from input number editor without clearing the action queue.
* PARAMETERS
*  none
* RETURNS
*  1: cleared required, 0: cleared not required
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_msg_need_clear_action_queue (void)
{
	if(g_msg_cntx.msg_status != MSG_STATUS_IDLE)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_need_new_msg_ind
* DESCRIPTION
*   check if new msg indication is required
* PARAMETERS
*  none
* RETURNS
*  1: indication required, 0: indication not required
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_msg_need_new_msg_ind (void)
{
	/*Return false if the new msg is deleted by AT */
  	if(SMSAL_INVALID_INDEX == g_msg_cntx.msg_ind_index ) return g_msg_cntx.msg_ind_in_idle;
	if(mmi_frm_sms_msg_box[g_msg_cntx.msg_ind_index].startindex == MMI_FRM_SMS_INVALID_INDEX)
	{
		g_msg_cntx.msg_ind_in_idle=FALSE;
		g_msg_cntx.msg_ind_after_call=FALSE;		
		return KAL_FALSE;
	}
	else
		return g_msg_cntx.msg_ind_in_idle;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_need_new_msg_ind_call_end
* DESCRIPTION
*   Check if new msg indication after call end is required
*
* PARAMETERS
*  none
* RETURNS
*  1: indication required, 0: indication not required
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_msg_need_new_msg_ind_call_end (void)
{
	return g_msg_cntx.msg_ind_after_call;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_reset_new_msg_ind_call_end
* DESCRIPTION
*   Reset new msg indication after call end
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   g_msg_cntx.msg_ind_after_call
*****************************************************************************/
void mmi_msg_reset_new_msg_ind_call_end (void)
{
	g_msg_cntx.msg_ind_after_call=FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_need_class0_msg_ind
* DESCRIPTION
*   Check if class 0 message indication is required
*
* PARAMETERS
*  none
* RETURNS
*  1: indication required, 0: indication not required
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_msg_need_class0_msg_ind (void)
{
	return g_msg_cntx.msg_class0_ind;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_need_mem_full_ind
* DESCRIPTION
*   Check if msg full indication is required
*
* PARAMETERS
*  none
* RETURNS
*  1: indication required, 0: indication not required
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_msg_need_mem_full_ind (void)
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(mmi_msg_is_mem_full_for_simid(i))
			return MMI_TRUE;
	}
	return MMI_FALSE;
}
#ifdef __MMI_MULTI_SIM__
U8 mmi_msg_is_mem_full_for_simid(U8 nSimID)
{
	return g_msg_cntx.msg_full_ind[nSimID];
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_msg_need_mem_exceed_ind
* DESCRIPTION
*   Check if msg exceed indication is required
*
* PARAMETERS
*  none
* RETURNS
*  1: indication required, 0: indication not required
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern U8 g_msg_mem_exceed_ind_simID;
BOOL mmi_msg_need_mem_exceed_ind (void)
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_msg_cntx.msg_exceed_ind[i])
		{
			mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_need_mem_exceed_ind for sim%d", g_msg_mem_exceed_ind_simID);
			g_msg_mem_exceed_ind_simID = i;
			return MMI_TRUE;
		}
	}
	return MMI_FALSE;
}
/*****************************************************************************
* FUNCTION
*  mmi_msg_need_delivery_report_ind
* DESCRIPTION
*   Check if delivery report indication is required
*
* PARAMETERS
*  none
* RETURNS
*  non-0: indication required, 0: indication not required
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_msg_need_delivery_report_ind (void)
{
	return g_msg_cntx.msg_deliver_ind;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_need_waiting_ind
* DESCRIPTION
*   Check if msg waiting indication is required
*
* PARAMETERS
*  none
* RETURNS
*  non-0: indication required, 0: indication not required
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_msg_need_waiting_ind (void)
{
	return g_msg_cntx.msg_waiting_ind;
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_msg_waiting_ind
* DESCRIPTION
*   Indicate msg waiting
*
* PARAMETERS
*  a  IN       waiting msg type
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_msg_waiting_ind (U8 type)
{
	g_msg_cntx.msg_waiting_ind=type+1;
	TurnOnBacklight(1);

	if (IsMMIInIdleState()||(GetMessagesCurrScrnID()==SCR_ID_MSG_MSG_WAITING_IND)) 
	{
		PlayMessageArrivalTone();
		ClearKeyEvents();
		if(IsKeyPadLockState())
			EntryIdleScreen(); 
		else
			mmi_msg_entry_msg_waiting_ind();
	}
	else
	{
		/* do not interrupt a call, otherwise, incoming call's video and sound may be interrupted */
		if (!GetTotalCallCount() && !AlmIsTonePlaying())
			mmi_msg_entry_new_msg_popup(g_msg_cntx.msg_waiting_ind);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_msg_waiting_ind
* DESCRIPTION
*   Reset msg waiting ind before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_msg_waiting_ind (void)
{
	g_msg_cntx.msg_waiting_ind=0;
	if(strBuff!=NULL)
	{
		OslMfree(strBuff);
		strBuff=NULL;
	}	
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_msg_waiting_ind
* DESCRIPTION
*   Reset msg waiting ind before go to idle
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_end_key_from_msg_waiting_ind (void)
{
	g_msg_cntx.msg_waiting_ind=0;
	if(strBuff!=NULL)
	{
		OslMfree(strBuff);
		strBuff=NULL;
	}
	ExecCurrEndKeyDownHandler();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_msg_waiting_msg
* DESCRIPTION
*   Get msg waiting msg
*
* PARAMETERS
*  none
* RETURNS
*  string for msg waiting indication
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* mmi_msg_get_msg_waiting_msg (void)
{
	U8* tempStr;
	if(strBuff!=NULL)
	{
		OslMfree(strBuff);
		strBuff=NULL;
	}
	strBuff=OslMalloc(64);
	switch(g_msg_cntx.msg_waiting_ind)
	{
		case MSG_NEW_VOICEMAIL:		tempStr = (U8*)GetString ((U16)STR_VOICEMAIL_IND);	break;
		case MSG_NEW_FAX:				tempStr = (U8*)GetString ((U16)STR_FAX_IND);		break;
		case MSG_NEW_EMAIL:			tempStr = (U8*)GetString ((U16)STR_EMAIL_IND);		break;
		default:							tempStr = (U8*)GetString ((U16)STR_VOICEMAIL_IND);	break;
	}
	pfnUnicodeStrcpy((S8*)strBuff, (S8*)tempStr);
	return strBuff;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_msg_waiting_ind
* DESCRIPTION
*   Entry msg waiting indication screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_msg_waiting_ind (void)
{
	U8* tempstr=mmi_msg_get_msg_waiting_msg();
	EntryNewScreen(SCR_ID_MSG_MSG_WAITING_IND, mmi_msg_stop_msg_sub_anm, NULL, NULL);
	SetMessagesCurrScrnID(SCR_ID_MSG_MSG_WAITING_IND);
	ForceSubLCDScreen(mmi_msg_start_msg_sub_anm);

	
	GetCurrEndKeyDownHandler ();
	ShowCategory141Screen ( 0, 0, 0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						    		(U8*)tempstr, IMG_NEW_VOICEMAIL_NOTIFICATION_MSG, NULL); 
 
	SetRightSoftkeyFunction(mmi_msg_go_back_from_msg_waiting_ind, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_end_key_from_msg_waiting_ind, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_deliver_report_ind
* DESCRIPTION
*   Indicate delivery report
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_deliver_report_ind (void)
{
	g_msg_cntx.msg_deliver_ind++;
	TurnOnBacklight(1);

	if (IsMMIInIdleState()||(GetMessagesCurrScrnID()==SCR_ID_MSG_DELIVER_REPORT)) 
	{
		PlayMessageArrivalTone();
		ClearKeyEvents();
		if(IsKeyPadLockState())
			EntryIdleScreen(); 
		else
			mmi_msg_entry_deliver_report_ind();
	}
	else
	{
#if defined(__PROJECT_GALLITE_C01__)	
	       mmi_msg_set_msg_sim_id(MTPNP_AD_get_protocol_event_src_mode());
#endif		   
		/* do not interrupt a call, otherwise, incoming call's video and sound may be interrupted */
		if (!GetTotalCallCount() && !AlmIsTonePlaying())
			mmi_msg_entry_new_msg_popup(MSG_NEW_DELIVERY_REPORT);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_deliver_report_ind
* DESCRIPTION
*   Free delivery report before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_deliver_report_ind(void)
{
	if(g_msg_cntx.msg_deliver_ind!=0)
		g_msg_cntx.msg_deliver_ind--;
	if(g_msg_cntx.msg_deliver_report[g_msg_cntx.msg_deliver_ind]!=NULL)
	{
		OslMfree(g_msg_cntx.msg_deliver_report[g_msg_cntx.msg_deliver_ind]);
		g_msg_cntx.msg_deliver_report[g_msg_cntx.msg_deliver_ind]=NULL;
	}
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_deliver_report_ind
* DESCRIPTION
*   Free delivery report before go to idle
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_end_key_from_deliver_report_ind(void)
{
	if(g_msg_cntx.msg_deliver_ind!=0)
		g_msg_cntx.msg_deliver_ind--;
	if(g_msg_cntx.msg_deliver_report[g_msg_cntx.msg_deliver_ind]!=NULL)
	{
		OslMfree(g_msg_cntx.msg_deliver_report[g_msg_cntx.msg_deliver_ind]);
		g_msg_cntx.msg_deliver_report[g_msg_cntx.msg_deliver_ind]=NULL;
	}
	ExecCurrEndKeyDownHandler();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_deliver_report_status
* DESCRIPTION
*   Get delivery report status
*
* PARAMETERS
*  a  IN/OUT       status
*  b  IN/OUT       image ID
*  c  IN/OUT       string ID
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_get_deliver_report_status (U32 *status, U16 *imgID, U16 *strID)
{
	if(g_msg_cntx.msg_deliver_report[(g_msg_cntx.msg_deliver_ind-1)]->delivered==ST_COMP_MSG_RECV_BY_SME)
	{
		*status	=	1;
		*imgID	=	IMG_MESSAGE_SENT;
		*strID	=	STR_SUCCESS_CAPTION;
	}
	else
	{
		*status	=	0;
		*imgID	=	IMG_MESSAGE_UNSENT;
		*strID	=	STR_FAILURE_CAPTION;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_deliver_report_msg
* DESCRIPTION
*   Get delivery report msg
*
* PARAMETERS
*  none
* RETURNS
*  string for delivery report indication
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* mmi_msg_get_deliver_report_msg (void)
{
	static S8 buffer [120*ENCODING_LENGTH];
	S8 buf2 [10];
	S8 temp[MAX_DIGITS*ENCODING_LENGTH];
	S8* name;
	S8	ts_date [(MAX_TIMESTAMP_SIZE+1)*ENCODING_LENGTH];
	S8	ts_time [(MAX_TIMESTAMP_SIZE+1)*ENCODING_LENGTH];
	U8 type = g_msg_cntx.msg_deliver_report[(g_msg_cntx.msg_deliver_ind-1)]->type;
	AnsiiToUnicodeString (buf2, "\n");

	memset(temp, 0, MAX_DIGITS*ENCODING_LENGTH);
	if(type)
		AnsiiToUnicodeString(temp, "+");
	AnsiiNToUnicodeString((temp+type*ENCODING_LENGTH), (S8*)g_msg_cntx.msg_deliver_report[(g_msg_cntx.msg_deliver_ind-1)]->number, MSG_NUM_LEN);
	name = lookUpNumber (temp);
	if(pfnUnicodeStrlen(name))
		pfnUnicodeStrcpy(buffer, name);
	else
		pfnUnicodeStrcpy(buffer, temp);

	mmi_msg_get_msg_date_time(ts_date, ts_time, g_msg_cntx.msg_deliver_report[(g_msg_cntx.msg_deliver_ind-1)]->timestamp);
	pfnUnicodeStrcat (buffer, buf2);
	pfnUnicodeStrcat (buffer, ts_date) ;
	pfnUnicodeStrcat (buffer, buf2);
	pfnUnicodeStrcat (buffer, ts_time) ;
	pfnUnicodeStrcat (buffer, buf2);
	
	return (U8*)buffer;

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_deliver_report_ind
* DESCRIPTION
*   Entry delivery report indication screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_deliver_report_ind (void)
{
	U8* guiBuffer=NULL;
	U16 drStatusIconID=0;
	U16 drStatusStrID=0;
	U32 drStatus=0;
	EntryNewScreen(SCR_ID_MSG_DELIVER_REPORT, mmi_msg_exit_generic, NULL, NULL);
	SetMessagesCurrScrnID(SCR_ID_MSG_DELIVER_REPORT);
	mmi_msg_get_deliver_report_status (&drStatus, &drStatusIconID, &drStatusStrID);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_DELIVER_REPORT);

	GetCurrEndKeyDownHandler ();
	ShowCategory145Screen(((U8*)GetString (STR_STATUS_REPORT_MESSAGE_DELIVERD_MSG)), IMG_SMS_ENTRY_SCRN_CAPTION,
									STR_GLOBAL_OK, IMG_GLOBAL_OK, 0, 0, 
									drStatusIconID, ((U8*)GetString(drStatusStrID)),
									mmi_msg_get_deliver_report_msg(), drStatus, guiBuffer);

	SetLeftSoftkeyFunction(mmi_msg_go_back_from_deliver_report_ind, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_end_key_from_deliver_report_ind, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_mem_exceed_ind
* DESCRIPTION
*   Reset msg exceed flag before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   g_msg_cntx.msg_exceed_ind
*****************************************************************************/
void mmi_msg_go_back_from_mem_exceed_ind (void)
{
	g_msg_cntx.msg_exceed_ind[g_msg_mem_exceed_ind_simID]=FALSE;
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_mem_exceed_ind
* DESCRIPTION
*   Reset msg exceed flag before go to idle
*
* PARAMETERS
*  nnoe
* RETURNS
*  none
* GLOBALS AFFECTED
*   g_msg_cntx.msg_exceed_ind
*****************************************************************************/
void mmi_msg_end_key_from_mem_exceed_ind (void)
{
	g_msg_cntx.msg_exceed_ind[g_msg_mem_exceed_ind_simID]=FALSE;
	ExecCurrEndKeyDownHandler();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_mem_exceed_ind
* DESCRIPTION
*   Entry memory exceed indication
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_mem_exceed_ind (void)
{
	U16 messageStrID = STR_SMS_MEMORY_EXCEEDED;
	EntryNewScreen(SCR_ID_MSG_EXCEED_IND, NULL, mmi_msg_entry_mem_exceed_ind, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_mem_exceed_ind ");

	GetCurrEndKeyDownHandler();
	#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_mem_exceed_ind for sim %d", g_msg_mem_exceed_ind_simID);
	messageStrID = STRING_MTPNP_MASTER_SMS_MEMORY_EXCEEDED + g_msg_mem_exceed_ind_simID;
	#endif
	ShowCategory2Screen( STR_SCR6042_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
								0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								messageStrID, NULL);
	SetRightSoftkeyFunction(mmi_msg_go_back_from_mem_exceed_ind, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_go_back_from_mem_exceed_ind, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_end_key_from_mem_exceed_ind, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_class0_msg_ind
* DESCRIPTION
*   Indicate class 0 message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_class0_msg_ind (void)
{
	g_msg_cntx.msg_class0_ind=TRUE;
	TurnOnBacklight(1);
	if (IsMMIInIdleState()||GetMessagesCurrScrnID()==SCR_ID_MSG_CLASS0_MSG) 
	{
		PlayMessageArrivalTone ();
		ClearKeyEvents();
		if(IsKeyPadLockState ())
			EntryIdleScreen();
		else
			mmi_msg_entry_class0_msg_ind();
	}
	else
	{
		/* do not interrupt a call, otherwise, incoming call's video and sound may be interrupted */
		if (!GetTotalCallCount() && !AlmIsTonePlaying())
			mmi_msg_entry_new_msg_popup(MSG_NEW_MSG_CLASS0);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_class0_msg
* DESCRIPTION
*   Get class 0 message
*
* PARAMETERS
*  none
* RETURNS
*  EMSData bufForDisplay
* GLOBALS AFFECTED
*   none
*****************************************************************************/
EMSData* mmi_msg_get_class0_msg (void)
{
	EMSData * pEms ;
	U16 textLen=g_msg_cntx.msg_class0_p->length;
	textLen=(textLen/2);
	GetEMSDataForView(&pEms, 1);
	AddString(pEms, g_msg_cntx.msg_class0_p->data, textLen, NULL);
	GetEMSDataForView(& pEms, 0);	
	return pEms;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_class0_header
* DESCRIPTION
*   Get class 0 message header
*
* PARAMETERS
*  none
* RETURNS
*  class 0 message header
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* mmi_msg_get_class0_header (void)
{
	static S8 buffer [120*ENCODING_LENGTH];
	S8 buf2 [10];
	S8 temp[MAX_DIGITS*ENCODING_LENGTH];
	S8* name;
	S8	ts_date [(MAX_TIMESTAMP_SIZE+1)*ENCODING_LENGTH];
	S8	ts_time [(MAX_TIMESTAMP_SIZE+1)*ENCODING_LENGTH];

	AnsiiToUnicodeString (buf2, "\n");
	pfnUnicodeStrcpy (buffer, GetString (STR_CLASS0_MESSAGE_CAPTION));
	pfnUnicodeStrcat (buffer, buf2);

	pfnUnicodeStrcpy(temp, (S8*)g_msg_cntx.msg_class0_p->number);
	name = lookUpNumber (temp);
	if(pfnUnicodeStrlen(name))
		pfnUnicodeStrcat(buffer, name);
	else
		pfnUnicodeStrcat(buffer, temp);

	mmi_msg_get_msg_date_time(ts_date, ts_time, g_msg_cntx.msg_class0_p->timestamp);
	pfnUnicodeStrcat (buffer, buf2);
	pfnUnicodeStrcat (buffer, ts_date) ;
	pfnUnicodeStrcat (buffer, buf2);
	pfnUnicodeStrcat (buffer, ts_time) ;
	pfnUnicodeStrcat (buffer, buf2);
	
	return (U8*)buffer;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_class0_msg_ind
* DESCRIPTION
*   Reset class 0 message before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_class0_msg_ind(void)
{
	g_msg_cntx.msg_class0_ind=FALSE;
	if(g_msg_cntx.msg_class0_p!=NULL)
	{
		if(g_msg_cntx.msg_class0_p->data != NULL)
		{
			OslMfree(g_msg_cntx.msg_class0_p->data);
			g_msg_cntx.msg_class0_p->data=NULL;
		}
		OslMfree(g_msg_cntx.msg_class0_p);
		g_msg_cntx.msg_class0_p=NULL;
	}
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_class0_msg_ind
* DESCRIPTION
*   Reset class 0 message before go to idle
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_end_key_from_class0_msg_ind(void)
{
	g_msg_cntx.msg_class0_ind=FALSE;
	if(g_msg_cntx.msg_class0_p!=NULL)
	{
		if(g_msg_cntx.msg_class0_p->data != NULL)
		{
			OslMfree(g_msg_cntx.msg_class0_p->data);
			g_msg_cntx.msg_class0_p->data=NULL;
		}
		OslMfree(g_msg_cntx.msg_class0_p);
		g_msg_cntx.msg_class0_p=NULL;
	}
	ExecCurrEndKeyDownHandler();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_class0_msg_ind_when_kpl
* DESCRIPTION
*   Entry class
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_class0_msg_ind_when_kpl (U16 LSKStringtID, U16 LSKIconID, U16 RSKStringID, U16 RSKIconID)
{
	ShowCategory39Screen ( STR_CLASS0_MESSAGE_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
									LSKStringtID, LSKIconID, 
									RSKStringID, RSKIconID,
									mmi_msg_get_class0_msg(), mmi_msg_get_class0_header(), NULL); 
        /* prevent category scrolling */
	SetKeyHandler(NULL, KEY_UP_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_VOL_UP, KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_VOL_DOWN, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_class0_msg_ind
* DESCRIPTION
*   Entry class 0 message indication
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_class0_msg_ind (void)
{
	U8* guiBuffer=NULL;

	EntryNewScreen(SCR_ID_MSG_CLASS0_MSG, mmi_msg_stop_msg_sub_anm, NULL, NULL);	
	SetMessagesCurrScrnID(SCR_ID_MSG_CLASS0_MSG);
	ForceSubLCDScreen(mmi_msg_start_msg_sub_anm);	
	guiBuffer=GetCurrGuiBuffer(SCR_ID_MSG_CLASS0_MSG);

	GetCurrEndKeyDownHandler();	
	ShowCategory39Screen(STR_CLASS0_MESSAGE_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
								0, 0,
								STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,	
								mmi_msg_get_class0_msg(), mmi_msg_get_class0_header(), guiBuffer); 

	SetRightSoftkeyFunction(mmi_msg_go_back_from_class0_msg_ind, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_end_key_from_class0_msg_ind, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_new_msg_ind
* DESCRIPTION
*   Indicate new message
*
* PARAMETERS
 
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_new_msg_ind (U16 index)
{
#ifdef __MMI_MULTI_SIM__
	U8 nSimID;
#endif
	g_msg_cntx.msg_ind_index=index;  
	g_msg_cntx.msg_ind_in_idle=TRUE;
	
	if(GetTotalCallCount())
		g_msg_cntx.msg_ind_after_call=TRUE;
	
	TurnOnBacklight(1);
	#ifndef __MMI_MULTI_SIM__
	mmi_msg_set_msg_icon(FALSE, FALSE);
	#else
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_FALSE, MTPNP_FALSE, nSimID);
	#endif
#ifdef __MMI_NOKIA_STYLE_N800__
	if (IsMMIInIdleState()||GetMessagesCurrScrnID()==SCR_ID_MSG_NEW_MSG_IND||GetActiveScreenId()==ITEM_SCR_MISSED_CALL) 
#else	
	if (IsMMIInIdleState()||GetMessagesCurrScrnID()==SCR_ID_MSG_NEW_MSG_IND) 
#endif		
	{
		#ifdef __MMI_MULTI_SIM__
		nSimID = mmi_frm_msg_get_simid_by_list_index(index);
		mmi_msg_set_msg_sim_id(nSimID);
		#endif
		PlayMessageArrivalTone();		
		StartLEDPatternMessageIn ();
		StartTimer(MESSAGES_SUBLCD_TIMER_ID, MESSAGES_SUBLCD_TIME_OUT, mmi_msg_callback_msg_sub_anm);
		ClearKeyEvents();

		if(IsKeyPadLockState ())
			EntryIdleScreen (); 
		else
			mmi_msg_entry_new_msg_ind(); 
	}
	else
	{

		/* it is in the inbox list or the inbox list screen is in the history */
		if (GetMessagesCurrScrnID()==SCR_ID_MSG_INBOX_LIST || IsScreenPresent(SCR_ID_MSG_INBOX_LIST))
		{
			U8 type;
			U16 list_index;
			mmi_frm_sms_get_list_index(&type, &list_index, g_msg_cntx.msg_ind_index);
			/* highlighter will be moved one position down only when the new message lists in front of the current highlighter and the new message does not make any incomplete message complete in inbox */
			if (type==MMI_FRM_SMS_APP_INBOX && list_index<=g_msg_cntx.currBoxIndex && msgbox_info.totalinbox<mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX))
				g_msg_cntx.currBoxIndex++;
		}
		/* do not interrupt a call, otherwise, incoming call's video and sound may be interrupted */
		if (GetTotalCallCount())
		{
			/* in case it is in the sms main menu from the in-call option */
			if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)!=MMI_FRM_SMS_INVALID_INDEX)
			{
				msgbox_info.totalinbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
				msgbox_info.totaloutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
				msgbox_info.totaldraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);	
				mmi_msg_set_msg_num_hint (msgbox_info.totalinbox, msgbox_info.totaloutbox, msgbox_info.totaldraftbox);
				RefreshMessagesMenuList();
			}
			playRequestedTone(SMS_IN_CALL_TONE);
		}
		else if (!AlmIsTonePlaying())
		{
#if defined (__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else	
				#ifdef __MMI_MULTI_SIM__
				nSimID = mmi_frm_msg_get_simid_by_list_index(index);
				mmi_msg_set_msg_sim_id(nSimID);
				#endif
#ifdef __MMI_NOKIA_STYLE_N800__
    			mmi_msg_entry_new_msg_ind(); 
#else
				mmi_msg_entry_new_msg_popup(MSG_NEW_MSG_NORMAL);
#endif
#endif
#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
			mmi_msg_refresh_memory_status();
#endif
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_new_msg_ind_string
* DESCRIPTION
*   Get new msg indication string
*
* PARAMETERS
*  none
* RETURNS
*  new msg indication string
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* mmi_msg_get_new_msg_ind_string (void)
{
	static S8 buffer [120*ENCODING_LENGTH];
	S8 temp[MAX_DIGITS*ENCODING_LENGTH];
	S8* name;

	memset(buffer, 0, 120*ENCODING_LENGTH);
	memset(temp, 0, MAX_DIGITS*ENCODING_LENGTH);
	AnsiiToUnicodeString(temp, 
		(S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_NOBOX, g_msg_cntx.msg_ind_index));
	name = lookUpNumber (temp);
	if(pfnUnicodeStrlen(name))
		pfnUnicodeStrcpy(buffer, name);
	else
		pfnUnicodeStrcpy(buffer, temp);
	
	return (U8*)buffer;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_new_msg_ind
* DESCRIPTION
*   Reset new msg indication before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_new_msg_ind (void)
{
	g_msg_cntx.msg_ind_in_idle=FALSE;
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_new_msg_ind
* DESCRIPTION
*   Reset new msg indication before go to idle
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_end_key_from_new_msg_ind (void)
{
	g_msg_cntx.msg_ind_in_idle=FALSE;
	ExecCurrEndKeyDownHandler ();
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_new_msg_ind
* DESCRIPTION
*   Entry new message indication
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_new_msg_ind (void)
{
	U16 strIdMessageFrom = STR_NEW_MESSAGE_FROM_ID;
	EntryNewScreen(SCR_ID_MSG_NEW_MSG_IND, mmi_msg_stop_msg_sub_anm, NULL, NULL);
	SetMessagesCurrScrnID(SCR_ID_MSG_NEW_MSG_IND);
	ForceSubLCDScreen(mmi_msg_start_msg_sub_anm);	
	GetCurrEndKeyDownHandler();
	#ifdef __MMI_MULTI_SIM__
	#ifndef __MMI_DUAL_SIM_SINGLE_OPENED_
		strIdMessageFrom = STRING_MTPNP_NEW_MASTER_MESSAGE_FROM_ID + mmi_frm_msg_get_simid_by_list_index(g_msg_cntx.msg_ind_index);
	#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
	#endif

    mmi_msg_new_msg_ind_play_tone();
    
	g_msg_cntx.msg_ind_after_call=FALSE;	
	if(gKeyPadLockFlag==0)
	{
		ShowCategory154Screen(0, 0,
										STR_SCR6035_LSK, IMG_SMS_COMMON_NOIMAGE,
										STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,
										(PU8)GetString (strIdMessageFrom), (PU8)mmi_msg_get_new_msg_ind_string(), 
										IMG_NEW_MESSAGE_NOTIFICATION_MSG, NULL);
		SetRightSoftkeyFunction(mmi_msg_go_back_from_new_msg_ind, KEY_EVENT_UP);
		SetLeftSoftkeyFunction(mmi_msg_get_msg_new, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_end_key_from_new_msg_ind, KEY_END, KEY_EVENT_DOWN);
	}
	else // keypad is locked
	{
           #ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
			ShowCategory154Screen(0, 0, 0, 0, KEYPAD_LOCKED_RSK_TEXT, 0,
										(PU8)GetString (strIdMessageFrom), (PU8)mmi_msg_get_new_msg_ind_string(),
										IMG_NEW_MESSAGE_NOTIFICATION_MSG, NULL);
	   #else // __MMI_KEYPAD_LOCK_PATTERN_2__
			ShowCategory154Screen(0, 0, KEYPAD_LOCKED_RSK_TEXT, 0, 0, 0,
										(PU8)GetString (strIdMessageFrom), (PU8)mmi_msg_get_new_msg_ind_string(), 
										IMG_NEW_MESSAGE_NOTIFICATION_MSG, NULL);
	   #endif
	   	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_start_msg_sub_anm
* DESCRIPTION
*   Start sub-LCD msg animation
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_start_msg_sub_anm (void)
{
	if (!GetTotalCallCount ())
	{
		ExecSubLCDCurrExitHandler ();
		if(GetMessagesCurrScrnID()==SCR_ID_MSG_MSG_WAITING_IND)
			ShowCategory304Screen (IMG_NEW_VOICEMAIL_PIC_SUBLCD_MSG, NULL);			
		else
			ShowCategory304Screen (IMG_NEW_SMS_PIC_SUBLCD_MSG, NULL);			
		SetSubLCDExitHandler(mmi_msg_callback_msg_sub_anm);
//		StartLEDPatternMessageIn ();
//		StartTimer(MESSAGES_SUBLCD_TIMER_ID, MESSAGES_SUBLCD_TIME_OUT, mmi_msg_callback_msg_sub_anm);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_stop_msg_sub_anm
* DESCRIPTION
*   Stop sub-LCD msg animation
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_stop_msg_sub_anm (void)
{
    mmi_msg_new_msg_ind_stop_tone();
	SetMessagesCurrScrnID(0);	//messagesCurrScrnID
	mmi_msg_callback_msg_sub_anm();
	GoBackSubLCDHistory ();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_callback_msg_sub_anm
* DESCRIPTION
*   Call back sub-LCD msg animation
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_callback_msg_sub_anm (void)
{
	StopLEDPatternMessageIn ();
	StopTimer (MESSAGES_SUBLCD_TIMER_ID);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_exit_inbox_list_dummy
* DESCRIPTION
*   Dummy exit inbox list, for new message indication
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_exit_inbox_list_dummy (void)
{
	history_t currHistory;

#ifdef __MMI_MULTI_SIM__
	U8 simID = mmi_frm_msg_get_simid_by_list_index(g_msg_cntx.msg_ind_index);
	MTPNP_AD_SMS_Set_Filter_Rule(simID);
#endif
	
	memset (& currHistory, 0, sizeof (currHistory));
	currHistory.scrnID = SCR_ID_MSG_INBOX_LIST;
	
	SetMessagesCurrScrnID (0);	
	g_msg_cntx.MessagesScrnIdToDelHistoryNodes=SCR_ID_MSG_INBOX_LIST;
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_INBOX_LIST;
	
	

	currHistory.entryFuncPtr = mmi_msg_entry_inbox_list;
	memset ((S8*)currHistory.inputBuffer, 0, ENCODING_LENGTH);
	
	AddHistory (currHistory);
}


#ifdef __MMI_MESSAGES_CLUB__
#include "profilegprots.h"
#include "wgui_categories_inputs.h"
#ifndef NVRAM_MSG_CLUB_RECORD_SIZE
#define NVRAM_MSG_CLUB_RECORD_SIZE 	10
#endif
#ifndef NVRAM_MSG_CLUB_RECORD_TOTAL
#define NVRAM_MSG_CLUB_RECORD_TOTAL	1
#endif
#ifndef NVRAM_MSG_CLUB_NUM_RECORD_SIZE
#define NVRAM_MSG_CLUB_NUM_RECORD_SIZE	5
#endif
#ifndef NVRAM_MSG_CLUB_NUM_RECORD_TOTAL
#define NVRAM_MSG_CLUB_NUM_RECORD_TOTAL	1
#endif

#define MSG_CLUB_SCREEN_NUM		6
#define MSG_CLUB_MENUITEM_NUM	25
#define MSG_CLUB_INPUT_BUFF_LEN	50
#define MSG_CLUB_INPUT_DA_LEN		15
#define MSG_CLUB_INPUT_NAME_LEN	30
#define MSG_CLUB_INPUT_SMS_LEN	120
#define MSG_CLUB_INPUT_PWD_LEN	16
#define MSG_CLUB_INPUT_SEPEATE	 "#"


U8 msg_club_level_1_index = 0;
U8 msg_club_level_2_index = 0;
U8 msg_club_level_3_index = 0;

static msg_club_entry_menu_struct MSGClubScreen[MSG_CLUB_SCREEN_NUM];
static msg_club_entry_menu_struct MSGClubMenuItem[MSG_CLUB_MENUITEM_NUM];
static msg_club_entry_dy_db_zf_struct MSGClubSMSItem;
static msg_club_code_enum MSGClubInputType;
static msg_club_network_enum MSGClubNetwork;
static S8* MSGClubInputBuff;
static S8* MSGClubInputBuff2;
static U8 MSGClubInputLen;
static U8 MSGClubInputLen2;
static U16 MSGClubTitle;
static U16 MSGClubTitle2;
static U8 MSGClubCount=0;

extern S8 gHomePLMNNum[MMI_SIM_NUMBER][MAX_PLMN_LEN_MMI+1];
extern const msg_club_entry_menu_struct MenuMSGClub[];
extern const msg_club_entry_menu_struct MenuVoice[];
extern const msg_club_service_num_struct NumService[];
extern const msg_club_entry_dy_db_zf_struct MenuExtra[];
extern const U16 InputString[];
extern UI_string_type get_string_lang(UI_string_ID_type i, S8* ssc_str);


void mmi_msg_club_make_call (void)
{
	MakeCall(g_msg_cntx.smsPhoneNumber);
}


void mmi_msg_club_exit_make_call (void)
{
	stopRequestedTone(WARNING_TONE);
}

void mmi_msg_club_entry_make_call (void)
{
	S8			textbuf[MSG_CLUB_CODE_LENGTH];
	S8			textbufucs2[MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH];
	S16 			error;
	msg_club_entry_voice_struct* itemInfo=(msg_club_entry_voice_struct*)MSGClubScreen[MSGClubCount].childEntry;
	U16	numType=itemInfo[msg_club_level_2_index].codeType;
	
	EntryNewScreen(SCR_ID_MSG_CONFIRMATION, mmi_msg_club_exit_make_call, NULL, NULL);
	
	memset(textbuf, 0, MSG_CLUB_CODE_LENGTH);
	memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
	memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
	AnsiiToUnicodeString(textbufucs2, (S8*)itemInfo[msg_club_level_2_index].codeVoice.codeNum);
	pfnUnicodeStrcpy(g_msg_cntx.smsPhoneNumber, textbufucs2);
	
	if((numType & NUMBER_END)==NUMBER_END)
	{
	memset(textbuf, 0, MSG_CLUB_CODE_LENGTH);
	memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
	ReadRecord (NVRAM_EF_MSG_CLUB_NUM_LID, 1, textbuf, NVRAM_MSG_CLUB_NUM_RECORD_SIZE, & error);
	AnsiiToUnicodeString(textbufucs2, textbuf);
	pfnUnicodeStrcat(g_msg_cntx.smsPhoneNumber, textbufucs2);
	}

	ShowCategory154Screen(0, 0,
							STR_GLOBAL_YES, IMG_SMS_COMMON_NOIMAGE,
							STR_GLOBAL_NO, IMG_SMS_COMMON_NOIMAGE,
							(PU8)get_string_lang (STR_GLOBAL_DIAL, SSC_SCHINESE), 
							(PU8)g_msg_cntx.smsPhoneNumber, 
							IMG_GLOBAL_QUESTION, NULL);
/*
	ShowCategory164Screen(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  STR_GLOBAL_DIAL, IMG_GLOBAL_QUESTION, NULL); 
*/
	SetLeftSoftkeyFunction (mmi_msg_club_make_call, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	playRequestedTone(WARNING_TONE);
}

void mmi_msg_club_send_msg_rsp(void* number, module_type mod, U16 result)
{
	switch(result)
	{
		case MMI_FRM_SMS_OK:
			DisplayPopup ((PU8)GetString(STR_SMS_SEND_SUCCESS), IMG_SEND_SUCCESS_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
			mmi_frm_sms_delete_screen_history();
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_ABORT:
			DisplayPopup ((PU8)GetString (STR_ABORT_SENDING_SMS), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
			mmi_frm_sms_delete_screen_history();
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_NOTREADY:
			DisplayPopup ((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);			
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_RETURN:
		case MMI_FRM_SMS_END:
		break;
		default:
			DisplayPopup ((PU8)GetString(STR_SMS_SEND_FAILED), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			mmi_frm_sms_delete_screen_history();
			AlmEnableSPOF();		
		break;
	}
}

void mmi_msg_club_send_msg_req (void)
{
	S8			textbuf[MSG_CLUB_CODE_LENGTH];
	S8			textbufucs2[MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH];
	S8			codebufucs2[(MSG_CLUB_CODE_LENGTH+MSG_CLUB_INPUT_BUFF_LEN)*ENCODING_LENGTH];
	EMSData*	pEMS;
	S16 			error;
	byte			result;
	history_t		temphistory;
	U8 service = msg_club_level_3_index;
	U8 serviceNum[MSG_CLUB_LONG_CODE_LENGTH+1];
	U16 serviceType;

	U8 codeNum[MSG_CLUB_LONG_CODE_LENGTH];
	U16 codeType;

	U8 serviceCode[MSG_CLUB_SHORT_CODE_LENGTH*ENCODING_LENGTH];
	U8 servicePrefix=0;

	/****************phone number********************/
	memset(textbuf, 0, MSG_CLUB_CODE_LENGTH);
	memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
	memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
	memset(serviceNum, 0, MSG_CLUB_LONG_CODE_LENGTH+1);

	if((MSGClubNetwork==MSG_CLUB_CU)&&((MSGClubSMSItem.strInput_codeCU & 0x0f)>1))
		memcpy((S8*)&MSGClubSMSItem, (S8*)&MenuExtra[(MSGClubSMSItem.strInput_codeCU & 0x0f)], sizeof(msg_club_entry_dy_db_zf_struct));

	mmi_trace(g_sw_SMS, "MMI_SMS: service=%d", service);
	switch(service)
	{
		case MSG_CLUB_SERVICE_SUBSCRIBE:
			memcpy(serviceNum, NumService[MSGClubSMSItem.serverType].serverDY[MSGClubNetwork].codeNum, MSG_CLUB_LONG_CODE_LENGTH);
			serviceType=NumService[MSGClubSMSItem.serverType].serverDY[MSGClubNetwork].codeType;
			break;
		case MSG_CLUB_SERVICE_ORDER:
			memcpy(serviceNum, NumService[MSGClubSMSItem.serverType].serverDB[MSGClubNetwork].codeNum, MSG_CLUB_LONG_CODE_LENGTH);
			serviceType=NumService[MSGClubSMSItem.serverType].serverDB[MSGClubNetwork].codeType;
			mmi_trace(g_sw_SMS, "MMI_SMS:serviceType=%d*\n", serviceType);
			break;
		case MSG_CLUB_SERVICE_EXPLAIN:
			memcpy(serviceNum, NumService[MSGClubSMSItem.serverType].serverZF[MSGClubNetwork].codeNum, MSG_CLUB_LONG_CODE_LENGTH);
			serviceType=NumService[MSGClubSMSItem.serverType].serverZF[MSGClubNetwork].codeType;
			break;
		case MSG_CLUB_SERVICE_CANCEL:
			memcpy(serviceNum, NumService[MSGClubSMSItem.serverType].serverTD[MSGClubNetwork].codeNum, MSG_CLUB_LONG_CODE_LENGTH);
			serviceType=NumService[MSGClubSMSItem.serverType].serverTD[MSGClubNetwork].codeType;
			break;
	}

	/*service number*/
	AnsiiToUnicodeString((S8*)textbufucs2, (S8*)serviceNum);
			pfnUnicodeStrcat(g_msg_cntx.smsPhoneNumber, textbufucs2);
			
	if((serviceType & NUMBER_END)==NUMBER_END)
	{
			/*handset number*/
		memset(textbuf, 0, MSG_CLUB_CODE_LENGTH);
		memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
			ReadRecord (NVRAM_EF_MSG_CLUB_NUM_LID, 1, textbuf, NVRAM_MSG_CLUB_NUM_RECORD_SIZE, & error);
			AnsiiToUnicodeString(textbufucs2, textbuf);
			pfnUnicodeStrcat(g_msg_cntx.smsPhoneNumber, textbufucs2);

	}
	if((serviceType & CODE_END)==CODE_END)
	{
			memset(textbuf, 0, MSG_CLUB_CODE_LENGTH);
			memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
			AnsiiToUnicodeString(textbufucs2, mmi_msg_club_get_number_end());
			pfnUnicodeStrcat(g_msg_cntx.smsPhoneNumber, textbufucs2);
	}
	


	/*product code*/
	ReleaseEMSEditBuffer();
	memset(textbuf, 0, MSG_CLUB_CODE_LENGTH);
	memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
	memset(codebufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
	memset(serviceCode, 0, MSG_CLUB_SHORT_CODE_LENGTH*ENCODING_LENGTH);
	memset(codeNum, 0, MSG_CLUB_LONG_CODE_LENGTH-1);

	switch(service)
	{
		case MSG_CLUB_SERVICE_SUBSCRIBE:
			memcpy(codeNum, MSGClubSMSItem.codeDY.codeNum, MSG_CLUB_LONG_CODE_LENGTH-1);
			AnsiiToUnicodeString((S8*)serviceCode, mmi_msg_club_get_code_subscribe());
			codeType=MSGClubSMSItem.codeDY.codeType;
			break;
		case MSG_CLUB_SERVICE_ORDER:
			memcpy(codeNum, MSGClubSMSItem.codeDB.codeNum, MSG_CLUB_LONG_CODE_LENGTH-1);
			AnsiiToUnicodeString((S8*)serviceCode, mmi_msg_club_get_code_order());
			codeType=MSGClubSMSItem.codeDB.codeType;
			break;
		case MSG_CLUB_SERVICE_EXPLAIN:
			memcpy(codeNum, MSGClubSMSItem.codeZF.codeNum, MSG_CLUB_LONG_CODE_LENGTH-1);
			AnsiiToUnicodeString((S8*)serviceCode, mmi_msg_club_get_code_explain());
			codeType=MSGClubSMSItem.codeZF.codeType;
			break;
		case MSG_CLUB_SERVICE_CANCEL:
			memcpy(codeNum, MSGClubSMSItem.codeTD.codeNum, MSG_CLUB_SHORT_CODE_LENGTH);
			codeType=0;
			break;
	}
	if((codeType & PHONE_PRE)==PHONE_PRE)
	{
		memset(textbuf, 0, MSG_CLUB_CODE_LENGTH);
		memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
		memset(codebufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
		ReadRecord (NVRAM_EF_MSG_CLUB_LID, 1, textbuf, NVRAM_MSG_CLUB_RECORD_SIZE, & error);
		AnsiiToUnicodeString(codebufucs2, textbuf);
		servicePrefix=1;
	}

	if((codeType & NUMBER_PRE)==NUMBER_PRE)
	{
		memset(textbuf, 0, MSG_CLUB_CODE_LENGTH);
		memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
		ReadRecord (NVRAM_EF_MSG_CLUB_NUM_LID, 1, textbuf, NVRAM_MSG_CLUB_NUM_RECORD_SIZE, & error);
		AnsiiToUnicodeString(textbufucs2, textbuf);
		pfnUnicodeStrcat(codebufucs2, textbufucs2);
		servicePrefix=1;
	}

	if(servicePrefix==1)
	{
		memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
		AnsiiToUnicodeString(textbufucs2, mmi_msg_club_get_code_sms());
		pfnUnicodeStrcat(codebufucs2, textbufucs2);
	}		

	if((codeType & CODE_PRE)==CODE_PRE)
	{
		memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
		AnsiiToUnicodeString(textbufucs2, mmi_msg_club_get_code_dummy());
		pfnUnicodeStrcat(codebufucs2, textbufucs2);
	}

	if(servicePrefix==1)
	{
		memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
		AnsiiToUnicodeString(textbufucs2, mmi_msg_club_get_code_devide());
		pfnUnicodeStrcat(codebufucs2, textbufucs2);
	}
	
	if((codeType & SERVICE_PRE)==SERVICE_PRE)
		pfnUnicodeStrcat(codebufucs2, (S8*)serviceCode);

	memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
	AnsiiToUnicodeString(textbufucs2, (S8*)codeNum);
	pfnUnicodeStrcat(codebufucs2, textbufucs2);
	
	if((codeType & SER_END)==SER_END)
		pfnUnicodeStrcat(codebufucs2, (S8*)serviceCode);
	
	if((MSGClubInputType & INPUT_CODE)==INPUT_CODE)
		pfnUnicodeStrcat(codebufucs2, MSGClubInputBuff);
		
	if(MSGClubInputLen2)
	{
		memset(textbufucs2, 0, MSG_CLUB_CODE_LENGTH*ENCODING_LENGTH);
		AnsiiToUnicodeString((S8*)textbufucs2, MSG_CLUB_INPUT_SEPEATE);
		pfnUnicodeStrcat(codebufucs2, textbufucs2);
		pfnUnicodeStrcat(codebufucs2, MSGClubInputBuff2);
	}
	
	memset (& temphistory, 0, sizeof (temphistory));
	GetHistory (SCR_ID_MSG_WRITE, &temphistory);
	pEMS = GetEMSDataForEdit (0, 0);
	/*NOTE: change function name to a general one!!!!*/

	if((MSGClubInputType & INPUT_CHI)==INPUT_CHI)
		result = AppendEMSString (INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
					pEMS, (U8*)codebufucs2, SMSAL_UCS2_DCS, temphistory.guiBuffer  );
	else
	result = AppendEMSString (INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
					pEMS, (U8*)codebufucs2, SMSAL_DEFAULT_DCS, temphistory.guiBuffer  );

	if(result)
	{
		mmi_frm_sms_send_struct * sendData=OslMalloc(sizeof(mmi_frm_sms_send_struct));
		memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
		memset(sendData->number, 0, MAX_DIGITS_SMS);	
		if(pfnUnicodeStrlen((S8*)smsPhoneNumber))
			UnicodeToAnsii((S8*)sendData->number, (S8*)smsPhoneNumber);
		mmi_frm_sms_send_sms(mmi_msg_club_send_msg_rsp, MOD_MMI, sendData);
		OslMfree(sendData);
	}
	else
		DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);		
}

void mmi_msg_club_entry_input_sms (void)
{
	U8* guiBuffer;
	EntryNewScreen(SCR_ID_MSG_CLUB_SMS_INPUT, NULL, mmi_msg_club_entry_input_sms, NULL);		
	guiBuffer=GetCurrGuiBuffer(SCR_ID_MSG_CLUB_SMS_INPUT);
	if((MSGClubInputType & INPUT_PWD)==INPUT_PWD)
		ShowCategory5Screen(MSGClubTitle2, IMG_MESSAGE_CLUB_ID,
					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,INPUT_TYPE_PHONE_NUMBER, (U8*)MSGClubInputBuff2, MSGClubInputLen2+1, guiBuffer);
	else
		ShowCategory5Screen(MSGClubTitle2, IMG_MESSAGE_CLUB_ID,
					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, (U8*)MSGClubInputBuff2, MSGClubInputLen2+1, guiBuffer);
	SetLeftSoftkeyFunction (mmi_msg_club_send_msg, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_club_send_msg, KEY_SEND, KEY_EVENT_UP);
}

void mmi_msg_club_entry_input_done (void)
{
	if(MSGClubInputLen2)
		mmi_msg_club_entry_input_sms();
	else
		mmi_msg_club_send_msg();
}


void mmi_msg_club_entry_input(void)
{
	U8* guiBuffer;
	EntryNewScreen(SCR_ID_MSG_CLUB_NUM_INPUT, NULL, mmi_msg_club_entry_input, NULL);		
	guiBuffer=GetCurrGuiBuffer(SCR_ID_MSG_CLUB_NUM_INPUT);
	if(((MSGClubInputType & INPUT_NUM)==INPUT_NUM)||((MSGClubInputType & INPUT_DA) == INPUT_DA))
		ShowCategory5Screen(MSGClubTitle, IMG_MESSAGE_CLUB_ID,
					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,INPUT_TYPE_PHONE_NUMBER, (U8*)MSGClubInputBuff, MSGClubInputLen+1, guiBuffer);
	else
		ShowCategory5Screen(MSGClubTitle, IMG_MESSAGE_CLUB_ID,
					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, (U8*)MSGClubInputBuff, MSGClubInputLen+1, guiBuffer);
	SetLeftSoftkeyFunction (mmi_msg_club_entry_input_done, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_club_entry_input_done, KEY_SEND, KEY_EVENT_UP);
}


void mmi_msg_club_pre_entry_send_msg(void)
{
	MSGClubInputType=0;
	if(MSGClubInputBuff!=NULL)
	{
		OslMfree(MSGClubInputBuff);
		MSGClubInputBuff=NULL;
	}
	if(MSGClubInputBuff2!=NULL)
	{
		OslMfree(MSGClubInputBuff2);
		MSGClubInputBuff2=NULL;
	}
	if((msg_club_level_3_index==MSG_CLUB_SERVICE_SUBSCRIBE)
		&& ((MSGClubSMSItem.codeDY.codeType & INPUT_CODE)==INPUT_CODE))
		MSGClubInputType=MSGClubSMSItem.codeDY.codeType;
	if((msg_club_level_3_index==MSG_CLUB_SERVICE_ORDER)
		&& ((MSGClubSMSItem.codeDB.codeType & INPUT_CODE)==INPUT_CODE))
		MSGClubInputType=MSGClubSMSItem.codeDB.codeType;
	if((msg_club_level_3_index==MSG_CLUB_SERVICE_EXPLAIN)
		&& ((MSGClubSMSItem.codeZF.codeType & INPUT_CODE)==INPUT_CODE))
		MSGClubInputType=MSGClubSMSItem.codeZF.codeType;
	if(MSGClubInputType)
	{
		if((MSGClubInputType & INPUT_DA) == INPUT_DA)
		{
			MSGClubInputLen = MSG_CLUB_INPUT_DA_LEN;
			MSGClubTitle = STR_MSG_CLUB_ENTER_QQ;
		}
		else if((MSGClubInputType & INPUT_NAME) == INPUT_NAME)
		{
			MSGClubInputLen = MSG_CLUB_INPUT_NAME_LEN;
			MSGClubTitle = STR_MSG_CLUB_ENTER_NICKNAME;
		}
		else
		{
			MSGClubInputLen = MSG_CLUB_INPUT_BUFF_LEN;
			if((MSGClubInputType & INPUT_NUM) == INPUT_NUM)
				MSGClubTitle = STR_MSG_CLUB_ENTER_NUMBER;
			else
				MSGClubTitle = STR_MSG_CLUB_ENTER_NAME;				
		}
		MSGClubInputBuff = OslMalloc((MSGClubInputLen+1)*ENCODING_LENGTH);
		memset(MSGClubInputBuff, 0, (MSGClubInputLen+1)*ENCODING_LENGTH);

		if((MSGClubInputType & INPUT_SMS) == INPUT_SMS)
		{
			MSGClubInputLen2 = MSG_CLUB_INPUT_SMS_LEN;
			MSGClubTitle2 = STR_MSG_CLUB_ENTER_MSG;
		}
		else if((MSGClubInputType & INPUT_PWD) == INPUT_PWD)
		{
			MSGClubInputLen2 = MSG_CLUB_INPUT_PWD_LEN;
			MSGClubTitle2 = STR_MSG_CLUB_ENTER_PWD;
		}
		else
			MSGClubInputLen2 = 0;
		if(MSGClubInputLen2)
		{
			MSGClubInputBuff2 = OslMalloc((MSGClubInputLen2+1)*ENCODING_LENGTH);
			memset(MSGClubInputBuff2, 0, (MSGClubInputLen2+1)*ENCODING_LENGTH);
		}
		if((MSGClubSMSItem.strInput_codeCU & 0xf0)>0)
			mmi_msg_club_pre_entry_input();
		else
		mmi_msg_club_entry_input();
	}
	else
		mmi_msg_club_send_msg();
}


void mmi_msg_club_highlight_subscribe (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_club_pre_entry_send_msg, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	msg_club_level_3_index=MSG_CLUB_SERVICE_SUBSCRIBE;
}

void mmi_msg_club_highlight_order (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_club_pre_entry_send_msg, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	msg_club_level_3_index=MSG_CLUB_SERVICE_ORDER;
}

void mmi_msg_club_highlight_explain (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_club_pre_entry_send_msg, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	msg_club_level_3_index=MSG_CLUB_SERVICE_EXPLAIN;
}

void mmi_msg_club_highlight_quit (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_club_pre_entry_send_msg, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	msg_club_level_3_index=MSG_CLUB_SERVICE_CANCEL;
}

void mmi_msg_club_set_menu_highlight_handler(void)
{
	SetHiliteHandler (MENU_MSG_CLUB_SUBSCRIBE, mmi_msg_club_highlight_subscribe);
	SetHiliteHandler (MENU_MSG_CLUB_ORDER, mmi_msg_club_highlight_order);
	SetHiliteHandler (MENU_MSG_CLUB_EXPLAIN, mmi_msg_club_highlight_explain);
	SetHiliteHandler (MENU_MSG_CLUB_QUIT, mmi_msg_club_highlight_quit);
}


void mmi_msg_club_go_back_menu(void)
{
	memset((S8*)&MSGClubScreen[MSGClubCount], 0, sizeof(msg_club_entry_menu_struct));
	MSGClubCount--;
	GoBackHistory();
}


void mmi_msg_club_level_3_get_index (S32 nIndex)
{
	msg_club_level_3_index	=	(U8)nIndex;
}

void mmi_msg_club_level_2_get_index (S32 nIndex)
{
	msg_club_level_2_index	=	(U8)nIndex;
}


void mmi_msg_club_entry_voice_submenu (void)
{
	U8*	guiBuffer=NULL;
	U8*	itemList[MSG_CLUB_MENUITEM_NUM];
	U8	itemNum=MSGClubScreen[MSGClubCount].childNum;
	U8 i;
	U8* caption;
	msg_club_entry_voice_struct* itemInfo=(msg_club_entry_voice_struct*)MSGClubScreen[MSGClubCount].childEntry;

	if(MSGClubScreen[MSGClubCount].childNum==0)
	{
		msg_club_level_2_index=0;
		mmi_msg_club_entry_make_call();
		memset((S8*)&MSGClubScreen[MSGClubCount], 0, sizeof(msg_club_entry_menu_struct));
		MSGClubCount--;
		return;
	}


	EntryNewScreen(SCR_ID_MSG_CLUB_VOICE_SUBMENU, NULL, mmi_msg_club_entry_voice_submenu, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CLUB_VOICE_SUBMENU);		 
	for(i=0; i<MSGClubScreen[MSGClubCount].childNum; i++)
#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
		itemList[i]=(U8*)get_string_lang(itemInfo[i].stringID, SSC_ENGLISH);
#else
		itemList[i]=(U8*)GetString(itemInfo[i].stringID);
#endif	
	RegisterHighlightHandler (mmi_msg_club_level_2_get_index);
	
#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
		caption=(U8*)get_string_lang(MSGClubScreen[MSGClubCount].stringID, SSC_ENGLISH);
#else
		caption=(U8*)GetString(MSGClubScreen[MSGClubCount].stringID);
#endif	
	ShowCategory353Screen (caption, IMG_MESSAGE_CLUB_ID, 
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  itemNum, itemList, (U16 *)gIndexIconsImageList,
							  NULL , 0, 0, guiBuffer );
	
	SetLeftSoftkeyFunction (mmi_msg_club_entry_make_call, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_club_entry_make_call, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_club_go_back_menu, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_club_go_back_menu, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}



void mmi_msg_club_entry_level_3 (void)
{
	U8* guiBuffer;
	U16 nStrItemList[MAX_SUB_MENUS];
	U8*	itemList[MSG_CLUB_MENUITEM_NUM];
	U16 numItems;
	U32 menuItemId=-1;
	U32 menuItemId1=-1;	
	U32 maskingByte=-1;
	U8 i=0;
	U8* captionStr;
	
	EntryNewScreen(SCR_ID_MSG_CLUB_LEVEL_3, NULL, mmi_msg_club_entry_level_3, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CLUB_LEVEL_3);		
	SetParentHandler (MENU_MSG_CLUB_OPTION);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	numItems = GetNumOfChild (MENU_MSG_CLUB_OPTION);
	GetSequenceStringIds(MENU_MSG_CLUB_OPTION, nStrItemList);
	mmi_msg_club_set_menu_highlight_handler();

	menuItemId=GetChildMenuIDIndexByParentMenuID(MENU_MSG_CLUB_OPTION, MENU_MSG_CLUB_SUBSCRIBE);
	if(menuItemId!=-1)
	{
		if(strcmp((S8*)MSGClubSMSItem.codeDY.codeNum, "")==0)
				ResetBit(maskingByte,menuItemId/*1*/);
		else
		{
#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
			itemList[i]=(U8*)get_string_lang(STR_MSG_CLUB_SUBSCRIBE, SSC_ENGLISH);
#else
			itemList[i]=(U8*)GetString(STR_MSG_CLUB_SUBSCRIBE);
#endif
			i++;
		}			
	}
	menuItemId=GetChildMenuIDIndexByParentMenuID(MENU_MSG_CLUB_OPTION, MENU_MSG_CLUB_ORDER);
	if(menuItemId!=-1)
	{
		if(strcmp((S8*)MSGClubSMSItem.codeDB.codeNum, "")==0)
				ResetBit(maskingByte,menuItemId/*2*/);
		else
		{
#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
			itemList[i]=(U8*)get_string_lang(STR_MSG_CLUB_ORDER, SSC_ENGLISH);
#else
			itemList[i]=(U8*)GetString(STR_MSG_CLUB_ORDER);
#endif
			i++;
		}			
	}
	menuItemId=GetChildMenuIDIndexByParentMenuID(MENU_MSG_CLUB_OPTION, MENU_MSG_CLUB_EXPLAIN);
	if(menuItemId!=-1)
	{
		if(strcmp((S8*)MSGClubSMSItem.codeZF.codeNum, "")==0)
				ResetBit(maskingByte,menuItemId/*3*/);
		else
		{
#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
			itemList[i]=(U8*)get_string_lang(STR_MSG_CLUB_INQUIRE, SSC_ENGLISH);
#else
			itemList[i]=(U8*)GetString(STR_MSG_CLUB_INQUIRE);
#endif
			i++;
		}			
	}	
	menuItemId=GetChildMenuIDIndexByParentMenuID(MENU_MSG_CLUB_OPTION, MENU_MSG_CLUB_QUIT);
	if(menuItemId!=-1)
	{
		if(strcmp((S8*)MSGClubSMSItem.codeTD.codeNum, "")==0)
				ResetBit(maskingByte,menuItemId/*4*/);
		else
		{
#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
			itemList[i]=(U8*)get_string_lang(STR_MSG_CLUB_QUIT, SSC_ENGLISH);
#else		
			itemList[i]=(U8*)GetString(STR_MSG_CLUB_QUIT);
#endif
			i++;
		}			
	}	
	
	MaskHiliteItems(MENU_MSG_CLUB_OPTION, maskingByte);
	numItems=MaskItems(nStrItemList,(U8)numItems,maskingByte);


#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
	captionStr=(U8*)get_string_lang(MSGClubSMSItem.stringID, SSC_ENGLISH);
#else
	captionStr=(U8*)GetString(MSGClubSMSItem.stringID);
#endif

	ShowCategory353Screen(captionStr,	IMG_MESSAGE_CLUB_ID, 
						  	STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  	STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  	numItems, itemList, (U16 *)gIndexIconsImageList, NULL,
						  	0, 0,	guiBuffer );
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP); //CSD Jo
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


void mmi_msg_club_pre_entry_level_3(void)
{
	msg_club_entry_dy_db_zf_struct* itemInfo=(msg_club_entry_dy_db_zf_struct*)MSGClubScreen[MSGClubCount].childEntry;
	memcpy((S8*)&MSGClubSMSItem, (S8*)&itemInfo[msg_club_level_2_index], sizeof(msg_club_entry_dy_db_zf_struct));
	if((strcmp((S8*)MSGClubSMSItem.codeDY.codeNum, "")!=0)
		||(strcmp((S8*)MSGClubSMSItem.codeDB.codeNum, "")!=0))
		mmi_msg_club_entry_level_3();
	else
{
		if(strcmp((S8*)MSGClubSMSItem.codeZF.codeNum, "")!=0)
			msg_club_level_3_index=MSG_CLUB_SERVICE_EXPLAIN;
		else
			msg_club_level_3_index=MSG_CLUB_SERVICE_CANCEL;
		mmi_msg_club_pre_entry_send_msg();
	}
}

void mmi_msg_club_entry_level_2 (void)
{
	U8*	guiBuffer=NULL;
	S32	hiliteitem=0;	
	U8*	itemList[MSG_CLUB_MENUITEM_NUM];
	U8	itemNum=MSGClubScreen[MSGClubCount].childNum;
	U8	i;
	U8* captionStr;
	msg_club_entry_dy_db_zf_struct* itemInfo=(msg_club_entry_dy_db_zf_struct*)MSGClubScreen[MSGClubCount].childEntry;

	
	EntryNewScreen(SCR_ID_MSG_CLUB_LEVEL_2, NULL, mmi_msg_club_entry_level_2, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CLUB_LEVEL_2);		 
	for(i=0; i<MSGClubScreen[MSGClubCount].childNum; i++)
	{
#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
		itemList[i]=(U8*)get_string_lang(itemInfo[i].stringID, SSC_ENGLISH);
#else
		itemList[i]=(U8*)GetString(itemInfo[i].stringID);
#endif
	}
		
	RegisterHighlightHandler (mmi_msg_club_level_2_get_index);

#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
	captionStr=(U8*)get_string_lang(MSGClubMenuItem[msg_club_level_1_index].stringID, SSC_ENGLISH);
#else
	captionStr=(U8*)GetString(MSGClubMenuItem[msg_club_level_1_index].stringID);
#endif
	ShowCategory353Screen (captionStr, IMG_MESSAGE_CLUB_ID, 
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  itemNum, itemList, (U16 *)gIndexIconsImageList,
							  NULL , 0, 0, guiBuffer );
	
	SetLeftSoftkeyFunction (mmi_msg_club_pre_entry_level_3, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_club_pre_entry_level_3, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_club_go_back_menu, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_club_go_back_menu, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


void mmi_msg_club_level_1_get_index (S32 nIndex)
{
	msg_club_level_1_index	= (U8)nIndex;
}


void mmi_msg_club_pre_entry_menu(void)
{
	MSGClubCount++;
	memcpy((S8*)&MSGClubScreen[MSGClubCount], (S8*)&MSGClubMenuItem[msg_club_level_1_index], sizeof(msg_club_entry_menu_struct));
	if((MSGClubScreen[MSGClubCount].childType & MSG_CLUB_MENU)==MSG_CLUB_MENU)
		mmi_msg_club_entry_level_1();
	else if((MSGClubScreen[MSGClubCount].childType & MSG_CLUB_VOICE_MENU)==MSG_CLUB_VOICE_MENU)
		mmi_msg_club_entry_voice_submenu();
	else if(MSGClubScreen[MSGClubCount].childNum>0)
		mmi_msg_club_entry_level_2();
	else
{
		msg_club_entry_dy_db_zf_struct* itemInfo=(msg_club_entry_dy_db_zf_struct*)MSGClubScreen[MSGClubCount].childEntry;
		memcpy((S8*)&MSGClubSMSItem, (S8*)&itemInfo[0], sizeof(msg_club_entry_dy_db_zf_struct));
		memset((S8*)&MSGClubScreen[MSGClubCount], 0, sizeof(msg_club_entry_menu_struct));
		MSGClubCount--;
		if((strcmp((S8*)MSGClubSMSItem.codeDY.codeNum, "")!=0)
			||(strcmp((S8*)MSGClubSMSItem.codeDB.codeNum, "")!=0))
			mmi_msg_club_entry_level_3();
		else
		{
			if(strcmp((S8*)MSGClubSMSItem.codeZF.codeNum, "")!=0)
				msg_club_level_3_index=MSG_CLUB_SERVICE_EXPLAIN;
			else
				msg_club_level_3_index=MSG_CLUB_SERVICE_CANCEL;
			mmi_msg_club_pre_entry_send_msg();
		}
	}
}


void mmi_msg_club_entry_level_1 (void)
{
	U8*	guiBuffer=NULL;
	U8*	itemList[MSG_CLUB_MENUITEM_NUM];
	U8	itemNum=0;
	U8	i, j=0;
	U8* captionStr;
	msg_club_entry_menu_struct* itemInfo=(msg_club_entry_menu_struct*)MSGClubScreen[MSGClubCount].childEntry;
	U16	scrIndex = SCR_ID_MSG_CLUB_LEVEL_1+(U16)MSGClubCount;

	EntryNewScreen(scrIndex, NULL, mmi_msg_club_entry_level_1, NULL);
	guiBuffer = GetCurrGuiBuffer (scrIndex);	
	for(i=0; i<MSGClubScreen[MSGClubCount].childNum; i++)
	{
		if(((itemInfo[i].childType & MSG_CLUB_CT_ONLY)==MSG_CLUB_CT_ONLY)&&(MSGClubNetwork!=MSG_CLUB_CT))
			continue;
#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
		itemList[j]=(U8*)get_string_lang(itemInfo[i].stringID, SSC_ENGLISH);
#else
		itemList[j]=(U8*)GetString(itemInfo[i].stringID);
#endif
		memcpy((S8*)&MSGClubMenuItem[j], (S8*)&itemInfo[i], sizeof(msg_club_entry_menu_struct));
		j++;
	}
	itemNum=j;
	RegisterHighlightHandler (mmi_msg_club_level_1_get_index);

#ifdef __MMI_MESSAGES_CLUB_SIM_CHINESE__
	captionStr=(U8*)get_string_lang(MSGClubScreen[MSGClubCount].stringID, SSC_ENGLISH);
#else
	captionStr=(U8*)GetString(MSGClubScreen[MSGClubCount].stringID);
#endif

	ShowCategory353Screen (captionStr, IMG_MESSAGE_CLUB_ID, 
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  itemNum, itemList, (U16 *)gIndexIconsImageList,
							  NULL , 0, 0, guiBuffer );
	
	SetLeftSoftkeyFunction (mmi_msg_club_pre_entry_menu, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_club_pre_entry_menu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_club_go_back_menu, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_club_go_back_menu, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

	
void EntryMsgClub(void)
{
//micha0422
	if((strncmp(gHomePLMNNum[0], "46000",5)==0)
		||(strncmp(gHomePLMNNum[0], "46002",5)==0))
		MSGClubNetwork=MSG_CLUB_CT;
	else if(strncmp(gHomePLMNNum[0], "46001",5)==0)
		MSGClubNetwork=MSG_CLUB_CU;
	else
		MSGClubNetwork=MSG_CLUB_NETWORK_TOTAL;
	if(MSGClubNetwork==MSG_CLUB_NETWORK_TOTAL)
		DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);		
	else
	{
		MSGClubCount=0;
		memset(MSGClubScreen, 0, sizeof(msg_club_entry_menu_struct)*MSG_CLUB_SCREEN_NUM);
		MSGClubScreen[0].stringID=STR_MSG_CLUB;
		MSGClubScreen[0].childNum=mmi_msg_club_get_item_total();
		MSGClubScreen[0].childType=MSG_CLUB_MENU;
		MSGClubScreen[0].childEntry=(void*)MenuMSGClub;
		mmi_msg_club_entry_level_1();
	}

}

#endif

#endif /* __MOD_SMSAL__ */


