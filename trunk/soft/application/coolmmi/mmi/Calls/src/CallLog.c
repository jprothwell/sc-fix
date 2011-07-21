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
 * CallLog.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for .
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: CallsLog.c

  	PURPOSE		: Call Setting Application

 

 

	DATE		: 10/22/2003

**************************************************************/

/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-06-14, Li Tao      implement the new feature "SendSMS"
 ************************************************************************/
 
#ifndef _CALL_LOG_C
#define _CALL_LOG_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "l4c_aux_struct.h"
#include "mmi_data_types.h"

//... Add More MMI header
#include "eventsgprot.h"
#include "historygprot.h"
#include "sublcdhistorygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories_popup.h"
#include "wgui_categories.h"
#include "wgui_datetime.h"
#include "custmenures.h"
#include "unicodexdcl.h"
#include "queuegprot.h"
#include "protocolevents.h"
#include "debuginitdef.h"
#include "idleappprot.h"
#include "idleappdef.h"
#include "wgui_status_icons.h"
#include "commonscreens.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "nvramtype.h"
#include "alarmprot.h"
#include "alarmframeworkprot.h"
#include "datetimetype.h"
#include "datetimegprot.h"
#include "timerevents.h"

#include "mainmenudef.h"
#include "shortcutsdefs.h"
#include "callmanagementstruct.h"
#include "callmanagementgprot.h"
#include "phonebookdef.h"
#include "phonebooktypes.h"
#include "phonebookgprot.h"
#include "phonebookprot.h"

#include "callhistorymain.h"
#include "callhistoryenum.h"
#include "callhistorygprots.h"
#include "callsstruct.h"
#include "callslog.h"
#include "callsreset.h"
#include "callmanagementiddef.h" //for error_unrecognized_number
#include "managecallmain.h"

#include "operator.h"
#include "mmi_trace.h"		//2007-06-14, li tao

#include "settinggenum.h"
#include "securitysetup.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#include "dual_sim_calllog.h"
#include "mtpnp_ad_resdef.h"
#endif /* __MMI_MULTI_SIM__ */
//2007-06-14, Li Tao
#if defined(__MOD_SMSAL__)
#include "messagesexdcl.h"
#include "messagesresourcedata.h"
#endif
#include "messagesmiscell.h"		//include this file controlled from the features???
#undef __NEWSIMULATOR 

extern U16 gKeyPadLockFlag;

#define CHIST_MSG_LENGTH 60
U8 CHISTMsgStr[CHIST_MSG_LENGTH * MMI_SIM_NUMBER];

U8 addToHistory = 1;
U8 nMissedCallNumBkp;

#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
#include "wgui_categories_tab.h"
S32 call_log_on_idle=0;

/*************************************************************************

	FUNCTION NAME		: SetCHISTTabPaneIdleScreen

  	PURPOSE				: Set tab pane function for call history in tab pane

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetCHISTTabPaneIdleScreen(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; 
	U16 nNumofItem;
	#ifdef __MMI_CH_MIXED_CALL_LOG__
		U16 ImageItemList[]=
		{
			TAB_MIXED_CALLS,
			TAB_MISSED_CALLS,
			TAB_RECIEVED_CALLS,
			TAB_DIALED_CALLS
		};
	#else
		U16 ImageItemList[]=
		{
			TAB_MISSED_CALLS,
			TAB_RECIEVED_CALLS,
			TAB_DIALED_CALLS
		};
	#endif
	nNumofItem = GetNumOfChild(IDLE_SCREEN_CALL_HISTORY_SEND);
	GetSequenceStringIds(IDLE_SCREEN_CALL_HISTORY_SEND,nStrItemList);

	SetParentHandler(IDLE_SCREEN_CALL_HISTORY_SEND);
	register_tab_pane_hilite_handler(ExecuteCurrHiliteHandler);
	set_tab_pane(nNumofItem,nStrItemList,ImageItemList,0);
}
/*************************************************************************

	FUNCTION NAME		: SetDataCHISTTabPaneIdleScreen

  	PURPOSE				: Set data function for call history in tab pane

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetDataCHISTTabPaneIdleScreen(void)
{
	call_log_on_idle=1;
}
/*************************************************************************

	FUNCTION NAME		: ResetDataCHISTTabPaneIdleScreen

  	PURPOSE				: Reset data function for call history in tab pane

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ResetDataCHISTTabPaneIdleScreen(void)
{
	call_log_on_idle=0;
}
/*************************************************************************

	FUNCTION NAME		: EntryCHISTTabPaneIdleScreen

  	PURPOSE				: Entry Screen for IDLE_SCREEN_CALL_HISTORY_SEND Menu

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTTabPaneIdleScreen(void)
{
	if (chis_p->isReqSent == TRUE)
		return;
	if (chis_p->isCallLogReady == TRUE && chis_p->isFirstTime == FALSE)
	{
		if (chis_p->process == CALL_LOG_PROC_WRITE)
		{
			PRINT_INFORMATION(("still writing, wait till write complete then read!!!!"));
			chis_p->process = CALL_LOG_PROC_SYNC;
			return;
		}
		chis_p->currRecvdCallIndex = 0;
		chis_p->currMissedCallIndex = 0;
		chis_p->currDialedCallIndex = 0;
		#ifdef __MMI_CH_MIXED_CALL_LOG__
		chis_p->currMixedCallIndex = 0;
		#endif
		register_set_tab_pane_callback(SetCHISTTabPaneIdleScreen,
			SetDataCHISTTabPaneIdleScreen,
			ResetDataCHISTTabPaneIdleScreen);
		#ifdef __MMI_CH_MIXED_CALL_LOG__
			show_category_four_icon_tab_pane();
		#else
			show_category_three_icon_tab_pane();
		#endif
	}
	else
	{
	   EntryNewScreen(SCR_CALL_LOG_NOT_READY, ExitCHISTProcessingAnimation,
	               NULL, NULL);

	   StartTimer (PHB_NOTIFYDURATION_TIMER, PHB_NOTIFY_TIMEOUT,
	               CHISTAnimationTimeOutHdlr);

	   ShowCategory66Screen(STR_SCR8057_CAPTION, IMG_SCR8057_CAPTION,
	                        0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                        (PU8)GetString(STR_CALLS_IN_PROGRESS_TEXT),
	                        IMG_PROCESSING_PHONEBOOK, NULL);

	   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	}
	return;	
}
/*************************************************************************

	FUNCTION NAME		: HighlightCHISTMixedCalls

  	PURPOSE				: Hilite Handler for Mixed Call logs menu item

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTMixedCalls(void)
{
	//SetKeyHandler( CHISTGetCallLogBySENDKey,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	disable_tab_pane_keys();
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTGetCallLogBySENDKey, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

#endif

extern void HightlightDialedAddToBlackList(void);
extern void HightlightReceivedAddToBlackList(void);
extern void HightlightMissedAddToBlackList(void);
extern void HightlightRejectedAddToBlackList(void);
extern void highlight_mainmenu_callhistory(void);
extern void HighlightCHISTMissedNotify(void);
extern void HintCHISTMissedNotify(U16 index);
/*************************************************************************

	FUNCTION NAME		: InitCallLogs

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitCallLogs( void )
{
	U8 i;
	
	#ifdef __MMI_MULTI_SIM__
	SetHiliteHandler(MENU_MTPNP_DM_CALL_HISTORY, HighlightDMCHISTMainMenu);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MENU_ID_CHIST_DUAL_CARD_MIXED_CALL_LOG_MASTER + i, HighlightCHISTDualMixedCallLog);
		SetHiliteHandler(MENU_MTPNP_MISSED_CALL_NOTIFY_MASTER + i, HighlightCHISTMissedNotify);
		SetHintHandler(MENU_MTPNP_MISSED_CALL_NOTIFY_MASTER + i, HintCHISTMissedNotify);
	}
	#endif
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt( ReadyLogIndication,PRT_MMI_PHB_LAST_NUMBER_READY_IND,i);
		#ifdef __IP_NUMBER__
		SetHiliteHandler( MISSED_CALL_MENU_IP_DIAL + i, HighlightMissedIpDial);
		SetHiliteHandler( RECEIVED_CALL_MENU_IP_DIAL + i, HighlightReceivedIpDial);
		SetHiliteHandler( DIALEDCALL_MENU_IP_DIAL + i, HighlightDialedIpDial);
		SetHiliteHandler( REJECTED_CALL_MENU_IP_DIAL + i, HighlightRejectedIpDial);
		#endif
	}
	
	SetHiliteHandler( MENU8103_MISSED_CALLS, HighlightCHISTMissedCalls );
	SetHiliteHandler( MENU8101_RECEIVED_CALLS, HighlightCHISTRecvdCalls );
	SetHiliteHandler( MENU8102_DIALED_CALLS, HighlightCHISTDialedCalls );
	SetHiliteHandler( MENU8104_REJECTED_CALLS, HighlightCHISTRejectedCalls );

	SetHintHandler( MENU8103_MISSED_CALLS, HintCHISTMissedCalls );
	SetHintHandler( MENU8101_RECEIVED_CALLS, HintCHISTRecvdCalls );
	SetHintHandler( MENU8102_DIALED_CALLS, HintCHISTDialedCalls );
	SetHintHandler( MENU8104_REJECTED_CALLS, HintCHISTRejectedCalls );
	
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	//SetHiliteHandler( IDLE_SCREEN_CALL_HISTORY_SEND, HighlightCHISTIdleScreen );
	SetHiliteHandler( MENU8100_MIXED_CALLS, HighlightCHISTMixedCalls );
#endif	
  	
	SetHiliteHandler( RECEIVED_CALL_MENU_CALL, HighlightCHISTCallRecvdNum );
	SetHiliteHandler( RECEIVED_CALL_MENU_SAVE, HighlightCHISTSaveRecvdNum );
	SetHiliteHandler( RECEIVED_CALL_MENU_DELETE, HighlightCHISTDeleteRecvdNum );
	SetHiliteHandler( RECEIVED_CALL_MENU_SENDSMS, HighlightCHISTSendSMSRecvdNum );	 //2007-06-14, Li Tao
	SetHiliteHandler( RECEIVED_CALL_MENU_EDIT, HighlightCHISTEditRecvdNum );
	SetHiliteHandler( DIALED_CALL_MENU_CALL, HighlightCHISTCallDialedNum );
	SetHiliteHandler( DIALED_CALL_MENU_SAVE, HighlightCHISTSaveDialedNum );
	SetHiliteHandler( DIALED_CALL_MENU_DELETE, HighlightCHISTDeleteDialedNum );
	SetHiliteHandler( DIALED_CALL_MENU_SENDSMS, HighlightCHISTSendSMSDialedNum ); 	//2007-06-14, Li Tao
	SetHiliteHandler( DIALED_CALL_MENU_EDIT, HighlightCHISTEditDialedNum );
	SetHiliteHandler( MISSED_CALL_MENU_CALL, HighlightCHISTCallMissedNum );
	SetHiliteHandler( MISSED_CALL_MENU_SAVE, HighlightCHISTSaveMissedNum );
	SetHiliteHandler( MISSED_CALL_MENU_DELETE, HighlightCHISTDeleteMissedNum );
	SetHiliteHandler( MISSED_CALL_MENU_SENDSMS, HighlightCHISTSendSMSMissedNum );	//2007-06-14, Li Tao
	SetHiliteHandler( MISSED_CALL_MENU_EDIT, HighlightCHISTEditMissedNum );
  	SetHiliteHandler( REJECTED_CALL_MENU_CALL, HighlightCHISTCallRejectedNum );
	SetHiliteHandler( REJECTED_CALL_MENU_SAVE, HighlightCHISTSaveRejectedNum );
	SetHiliteHandler( REJECTED_CALL_MENU_DELETE, HighlightCHISTDeleteRejectedNum );
	SetHiliteHandler( REJECTED_CALL_MENU_SENDSMS, HighlightCHISTSendSMSRejectedNum );	//2007-06-14, Li Tao
	SetHiliteHandler( REJECTED_CALL_MENU_EDIT, HighlightCHISTEditRejectedNum );
    
#if defined(__MMI_PHB_SEND_MMS_FROM_PHB__)
	SetHiliteHandler( RECEIVED_CALL_MENU_SENDMMS, HighlightCHISTSendMMSRecvdNum );
	SetHiliteHandler( DIALED_CALL_MENU_SENDMMS, HighlightCHISTSendMMSDialedNum );
	SetHiliteHandler( MISSED_CALL_MENU_SENDMMS, HighlightCHISTSendMMSMissedNum );
	SetHiliteHandler( REJECTED_CALL_MENU_SENDMMS, HighlightCHISTSendMMSRejectedNum );
#endif

#ifdef __MMI_CM_BLACK_LIST__
  SetHiliteHandler( MISSED_CALL_MENU_ADD_TO_BLACKLIST, HightlightMissedAddToBlackList );
  SetHiliteHandler( RECEIVED_CALL_MENU_ADD_TO_BLACKLIST,HightlightReceivedAddToBlackList );
  SetHiliteHandler( DIALED_CALL_MENU_ADD_TO_BLACKLIST, HightlightDialedAddToBlackList );
  SetHiliteHandler( REJECTED_CALL_MENU_ADD_TO_BLACKLIST, HightlightRejectedAddToBlackList );
#endif

#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	SetHiliteHandler(RECEIVED_CALL_MENU_INFO, HighlightCHISTInfoRecvdNum);
	SetHiliteHandler(DIALED_CALL_MENU_INFO, HighlightCHISTInfoDialedNum);
	SetHiliteHandler(MISSED_CALL_MENU_INFO, HighlightCHISTInfoMissedNum);
#endif

  InitCallHistoryContext();
   return;
}
#ifdef __IP_NUMBER__
void HighlightMissedIpDial( void )
{
	SetLeftSoftkeyFunction(CHISTCallMissedNumByIP,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void HighlightDialedIpDial( void )
{
	SetLeftSoftkeyFunction(CHISTCallDialedNumByIP,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void HighlightReceivedIpDial( void )
{
	SetLeftSoftkeyFunction(CHISTCallRecvdNumByIP,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
void HighlightRejectedIpDial( void )
{
	SetLeftSoftkeyFunction(CHISTCallRejectedNumByIP,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

#endif
/**************************************************************

	FUNCTION NAME		: HighlightCHISTDialedCalls

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTDialedCalls( void )
{

#ifdef __MMI_CH_MIXED_CALL_LOG__
	chis_p->isMixedStyle = FALSE;
#endif
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	if(!call_log_on_idle)
#endif
	{
		SetKeyHandler( ReqReadDialedNumByKey,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	}
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	else
	{
		disable_tab_pane_keys();
	}
#endif
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( ReqReadDialedNumByKey, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTMissedCalls

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTMissedCalls( void )
{
#ifdef __MMI_CH_MIXED_CALL_LOG__
	chis_p->isMixedStyle = FALSE;
#endif
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	if(!call_log_on_idle)
#endif
	{
		SetKeyHandler( ReqReadMissedNumByKey,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	}
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	else
	{
		disable_tab_pane_keys();
	}
#endif
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( ReqReadMissedNumByKey, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTRejectedCalls

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTRejectedCalls( void )
{

#ifdef __MMI_CH_MIXED_CALL_LOG__
	chis_p->isMixedStyle = FALSE;
#endif
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	if(!call_log_on_idle)
#endif
	{
		SetKeyHandler( ReqReadRejectedNumByKey,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	}
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	else
	{
		disable_tab_pane_keys();
	}
#endif
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( ReqReadRejectedNumByKey, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTRecvdCalls

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTRecvdCalls( void )
{
#ifdef __MMI_CH_MIXED_CALL_LOG__
	chis_p->isMixedStyle = FALSE;
#endif
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	if(!call_log_on_idle)
#endif
	{
		SetKeyHandler( ReqReadRecvdNumByKey,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	}
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	else
	{
		disable_tab_pane_keys();
	}
#endif
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( ReqReadRecvdNumByKey, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTCallDialedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTCallDialedNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
#ifdef __MMI_MULTI_SIM__
	SetLeftSoftkeyFunction( MTPNP_PFAL_Call_Dialed, KEY_EVENT_UP );
#else
	SetLeftSoftkeyFunction( CHISTCallDialedNum, KEY_EVENT_UP );
#endif
#ifdef __MMI_IP_KEY__
	/* under construction !*/
#endif		
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTSaveDialedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTSaveDialedNum( void )
{
	SetKeyHandler( CHISTSaveDialedNum,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSaveDialedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTDeleteDialedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTDeleteDialedNum( void )
{
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( EntryCHISTDeleteDialedCallConfirm, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

//2007-06-14, Li Tao
/**************************************************************

	FUNCTION NAME		: HighlightCHISTSendSMSDialedNum

  	PURPOSE				: Dialed call list option SendSMS handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTSendSMSDialedNum( void )
{
	//mmi_trace(1, "***** the current enter is ***************HighlightCHISTSendSMSDialedNum***");
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSendSMSDialedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTEditDialedNum

  	PURPOSE				: Dialed call list option Edit handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTEditDialedNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTEditDialedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
/**************************************************************

	FUNCTION NAME		: HighlightCHISTInfoDialedNum

  	PURPOSE				: Dialed call list option Info handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTInfoDialedNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( EntryCHISTViewDialedCall, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}
#endif
/**************************************************************

	FUNCTION NAME		: HighlightCHISTCallMissedNum

  	PURPOSE				: Missed call list option call handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTCallMissedNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
#ifdef __MMI_MULTI_SIM__
	SetLeftSoftkeyFunction( MTPNP_PFAL_Call_Missed, KEY_EVENT_UP );
#else
	SetLeftSoftkeyFunction( CHISTCallMissedNum, KEY_EVENT_UP );
#endif
#ifdef __MMI_IP_KEY__
	/* under construction !*/
#endif		
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void HighlightCHISTCallRejectedNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
#ifdef __MMI_MULTI_SIM__
	SetLeftSoftkeyFunction( MTPNP_PFAL_Call_Rejected, KEY_EVENT_UP );
#else
	SetLeftSoftkeyFunction( CHISTCallRejectedNum, KEY_EVENT_UP );
#endif
#ifdef __MMI_IP_KEY__
	/* under construction !*/
#endif		
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTSaveMissedNum

  	PURPOSE				: Missed call list option save handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTSaveMissedNum( void )
{
	SetKeyHandler( CHISTSaveMissedNum,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSaveMissedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTDeleteMissedNum

  	PURPOSE				: Missed call list option delete handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTDeleteMissedNum( void )
{
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( EntryCHISTDeleteMissedCallConfirm, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

//2007-06-14, Li Tao
/**************************************************************

	FUNCTION NAME		: HighlightCHISTSendSMSMissedNum

  	PURPOSE				: Missed call list option SendSMS handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTSendSMSMissedNum( void )
{
	//mmi_trace(1, "***** the current enter is ***************HighlightCHISTSendSMSMissedNum***");
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSendSMSMissedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTEditMissedNum

  	PURPOSE				: Missed call list option Edit handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTEditMissedNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTEditMissedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
/**************************************************************

	FUNCTION NAME		: HighlightCHISTInfoMissedNum

  	PURPOSE				: Missed call list option Info handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTInfoMissedNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( EntryCHISTViewMissedCall, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}
#endif
/**************************************************************

	FUNCTION NAME		: HighlightCHISTCallRecvdNum

  	PURPOSE				: Recvd call list option Call handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTCallRecvdNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
#ifdef __MMI_MULTI_SIM__
	SetLeftSoftkeyFunction( MTPNP_PFAL_Call_Recved, KEY_EVENT_UP );
#else
	SetLeftSoftkeyFunction( CHISTCallRecvdNum, KEY_EVENT_UP );
#endif
#ifdef __MMI_IP_KEY__
	/* under construction !*/
#endif		
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTSaveRecvdNum

  	PURPOSE				: Recvd call list option Save to phonebook handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTSaveRecvdNum( void )
{
	SetKeyHandler( CHISTSaveRecvdNum,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSaveRecvdNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void HighlightCHISTSaveRejectedNum( void )
{
	SetKeyHandler( CHISTSaveRejectedNum,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSaveRejectedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

	return;
}


/**************************************************************

	FUNCTION NAME		: HighlightCHISTDeleteRecvdNum

  	PURPOSE				: Recvd call list option Delete handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTDeleteRecvdNum( void )
{
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( EntryCHISTDeleteRecvdCallConfirm, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void HighlightCHISTDeleteRejectedNum( void )
{

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( EntryCHISTDeleteRejectedCallConfirm, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

	return;
}

//2007-06-14, Li Tao
/**************************************************************

	FUNCTION NAME		: HighlightCHISTSendSMSRecvdNum

  	PURPOSE				: Recvd call list option SendSMS handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTSendSMSRecvdNum( void )
{
	//mmi_trace(1, "***** the current enter is ***************HighlightCHISTSendSMSRecvdNum***");
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSendSMSRecvdNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void HighlightCHISTSendSMSRejectedNum( void )
{

	//mmi_trace(1, "***** the current enter is ***************HighlightCHISTSendSMSRecvdNum***");
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSendSMSRejectedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

#if defined(__MMI_PHB_SEND_MMS_FROM_PHB__)
void HighlightCHISTSendMMSRecvdNum( void )
{
	//mmi_trace(1, "***** the current enter is ***************HighlightCHISTSendSMSRecvdNum***");
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSendMMSRecvdNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void HighlightCHISTSendMMSRejectedNum( void )
{
	//mmi_trace(1, "***** the current enter is ***************HighlightCHISTSendSMSRecvdNum***");
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSendMMSRejectedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void HighlightCHISTSendMMSMissedNum( void )
{
	//mmi_trace(1, "***** the current enter is ***************HighlightCHISTSendSMSRecvdNum***");
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSendMMSMissedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void HighlightCHISTSendMMSDialedNum( void )
{
	//mmi_trace(1, "***** the current enter is ***************HighlightCHISTSendSMSRecvdNum***");
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTSendMMSDialedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}
#endif

/**************************************************************

	FUNCTION NAME		: HighlightCHISTEditRecvdNum

  	PURPOSE				: Recvd call list option Edit handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTEditRecvdNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTEditRecvdNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}
void HighlightCHISTEditRejectedNum( void )
{

	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( CHISTEditRejectedNum, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

	return;
}

#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
/**************************************************************

	FUNCTION NAME		: HighlightCHISTInfoRecvdNum

  	PURPOSE				: Recvd call list option Info handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTInfoRecvdNum( void )
{
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction( EntryCHISTViewRecvdCall, KEY_EVENT_UP );
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}
#endif
/**************************************************************

	FUNCTION NAME		: HintCHISTDialedCalls

  	PURPOSE				: fill in hint buffer for dialed calls

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTDialedCalls(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	memset( (PS8)hintData[index], 0, sizeof(hintData[index]));

	if(chis_p->nDialedCalls == 0)
	{
		pfnUnicodeStrncpy( (S8 *)hintData[index],(S8 *)GetString( STR_NO_ENTRY ), MAX_GET_STRING_LEN );
	}
	else
	{
		sprintf( (S8 *)value, "%d ", chis_p->nDialedCalls );
		AnsiiToUnicodeString( (PS8)hintData[index], (PS8)value);
		pfnUnicodeStrncat( (S8 *)hintData[index], (S8 *)GetString( STR_ENTRIES ),MAX_GET_STRING_LEN);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: HintCHISTMissedCalls

  	PURPOSE				: fill in hint buffer for missed calls

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTMissedCalls(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	memset( (PS8)hintData[index], 0, sizeof(hintData[index]));

	if(chis_p->nMissedCalls == 0)
	{
		pfnUnicodeStrncpy( (S8 *)hintData[index],
		                   (S8 *)GetString( STR_NO_ENTRY ), MAX_GET_STRING_LEN );
	}
	else
	{
		sprintf( (S8 *)value, "%d ", chis_p->nMissedCalls );
		AnsiiToUnicodeString( (PS8)hintData[index], (PS8)value);
		pfnUnicodeStrncat( (S8 *)hintData[index],
		                   (S8 *)GetString( STR_ENTRIES ), MAX_GET_STRING_LEN);
	}
	return;
}

void HintCHISTRejectedCalls(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	memset( (PS8)hintData[index], 0, sizeof(hintData[index]));

	if(chis_p->nRejectedCalls == 0)
	{
		pfnUnicodeStrncpy( (S8 *)hintData[index], (S8 *)GetString( STR_NO_ENTRY ), MAX_GET_STRING_LEN );
	}
	else
	{
		sprintf( (S8 *)value, "%d ", chis_p->nRejectedCalls );
		AnsiiToUnicodeString( (PS8)hintData[index], (PS8)value);
		pfnUnicodeStrncat( (S8 *)hintData[index], (S8 *)GetString( STR_ENTRIES ), MAX_GET_STRING_LEN);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: HintCHISTRecvdCalls

  	PURPOSE				: fill in hint buffer for recvd calls

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTRecvdCalls(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	memset( (PS8)hintData[index], 0, sizeof(hintData[index]));

	if(chis_p->nRecvdCalls == 0)
	{
		pfnUnicodeStrncpy( (S8 *)hintData[index],(S8 *)GetString( STR_NO_ENTRY ), MAX_GET_STRING_LEN );
	}
	else
	{
		sprintf( (S8 *)value, "%d ", chis_p->nRecvdCalls );
		AnsiiToUnicodeString( (PS8)hintData[index], (PS8)value);
		pfnUnicodeStrncat( (S8 *)hintData[index],(S8 *)GetString( STR_ENTRIES ), MAX_GET_STRING_LEN);
	}
	return;
}
//#define __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
#ifdef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
U8 CHISViewIndex = 0;

/**************************************************************

	FUNCTION NAME		: CHISViewCtrlByRgihtArrowKey

  	PURPOSE				: switch in 3 type call log

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISViewCtrlByRgihtArrowKey(void)
{
	CHISViewIndex++;
	if(CHISViewIndex ==3) 
		CHISViewIndex = 0;
	switch(CHISViewIndex)
	{
		case 0:
			ReqReadDialedNumByKey();
			break;
		case 1:
			ReqReadRecvdNumByKey();
			break;
		case 2:
			ReqReadMissedNumByKey();
			break;
		default:
			break;
	}
}

/**************************************************************

	FUNCTION NAME		: CHISViewCtrlByLeftArrowKey

  	PURPOSE				: switch in 3 type call log

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISViewCtrlByLeftArrowKey(void)
{
	if(CHISViewIndex ==0)
	{
		CHISViewIndex = 2;
	}
	else
	{
		CHISViewIndex --;
	}
	switch(CHISViewIndex)
	{
		case 0:
			ReqReadDialedNumByKey();
			break;
		case 1:
			ReqReadRecvdNumByKey();
			break;
		case 2:
			ReqReadMissedNumByKey();
			break;
		default:
			break;
	}
}
#endif
/**************************************************************

	FUNCTION NAME		: EntryCHISTViewDialedCallList

  	PURPOSE				: Dialed call list viewer screen entry function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTViewDialedCallList( void )
{

	U8*	phoneNumbers[TOT_SIZE_OF_DIALED_LIST]; 
	U8*	phoneNames[TOT_SIZE_OF_DIALED_LIST]; 
	
	U8*	guiBuffer;
	U16	Cat23DemoIcons[TOT_SIZE_OF_DIALED_LIST];  
	
	U8    count;

	U8 nSuited = 0;

	if (isInCall())
		return;

	if ( chis_p->nDialedCalls )
	{
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
		if(call_log_on_idle)
		{
		   set_pre_tab_pane_display();		
		   EntryNewScreen(SCR8055_CALLS_LIST_DIALED, NULL,
						ReqReadDialedNum, NULL);
		}
		else
#endif
              {
	   /* from menu or from shorcut */
		if (IsScreenPresent(MAIN_MENU_SCREENID) || IsScreenPresent(SCR_ID_SHCT_IDLE_SHORTCUT) ||
		   (GetExitScrnID() == SCR_ID_SHCT_IDLE_SHORTCUT))
	   	        EntryNewScreen(SCR8055_CALLS_LIST_DIALED, NULL, ReqReadDialedNum, NULL);
		else
			EntryNewScreen(SCR8055_CALLS_LIST_DIALED, NULL, CHISTGetCallLogBySENDKey, NULL);
              }

		guiBuffer = GetCurrGuiBuffer( SCR8055_CALLS_LIST_DIALED );
		RegisterHighlightHandler( CHISTGetCurrDialedCallIndex );
		CHISTPopulateDialedCallList( phoneNumbers, phoneNames );
		
		#ifdef __MMI_VIP_FUNCTION__
		nSuited = g_vip_context.nCallLogSuited;
		#endif

		for( count = 0; count < chis_p->nDialedCalls - nSuited; count++)
		{
		#ifdef __MMI_MULTI_SIM__
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			Cat23DemoIcons[count] = IMG_MENU8102_DIALED_CALLS;
			#else
			Cat23DemoIcons[count] = IMG_MTPNP_CARD1_MENU_DIALED_CALLS + chis_p->nSimId;
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		#else
			Cat23DemoIcons[count] = IMG_MENU8102_DIALED_CALLS;
		#endif
		}

	#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		ShowCategory53Screen( STR_SCR8056_CAPTION, IMG_SCR8056_CAPTION,
								    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK,
								    IMG_GLOBAL_BACK, chis_p->nDialedCalls - nSuited, phoneNames,
		                      (U16*)Cat23DemoIcons,phoneNumbers,0,
		                      chis_p->currDialedCallIndex, guiBuffer );
   #else
		ShowCategory53Screen( STR_SCR8056_CAPTION, IMG_SCR8056_CAPTION,
		                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
		                      IMG_GLOBAL_BACK, chis_p->nDialedCalls - nSuited, phoneNames,
		                      (U16*)Cat23DemoIcons,phoneNumbers,0,
		                      chis_p->currDialedCallIndex, guiBuffer );
	#endif

    
#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif		
		#ifndef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		#else
		SetKeyHandler( CHISViewCtrlByLeftArrowKey,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		#endif
	#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		SetKeyHandler( EntryCHISTDialedListOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetLeftSoftkeyFunction( EntryCHISTDialedListOptions, KEY_EVENT_UP );
	#else
		#ifndef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		SetKeyHandler( EntryCHISTViewDialedCall,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		#else
		SetKeyHandler(CHISViewCtrlByRgihtArrowKey, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		#endif
		SetLeftSoftkeyFunction( EntryCHISTViewDialedCall, KEY_EVENT_UP );
	#endif

#ifdef __MMI_USE_LSK_AS_SEND2__
    #ifdef __MMI_WGUI_CSK_ENABLE__
	       SetCenterSoftkeyFunction( EntryCHISTViewDialedCall, KEY_EVENT_UP );
    #endif
              SetKeyHandler(EntryCHISTViewDialedCall,KEY_ENTER,KEY_EVENT_UP);
#endif
		SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#ifdef __MMI_MULTI_SIM__
		MTPNP_PFAL_CC_HandleSendKeys(CHISTCallDialedNum,KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
		ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
	#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		SetKeyHandler(EntryCHISTDialedListOptions,KEY_ENTER,KEY_EVENT_UP );
	#else
		SetKeyHandler(EntryCHISTViewDialedCall,KEY_ENTER,KEY_EVENT_UP );
	#endif
#endif
#else
		SetKeyHandler(CHISTCallDialedNum, KEY_SEND, KEY_EVENT_DOWN);
#endif


#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
		if(call_log_on_idle)
		{
			set_post_tab_pane_display();		
		}
#endif
	#ifdef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		DeleteScreenIfPresent(SCR8054_CALLS_LIST_RECEIVED);
		DeleteScreenIfPresent(SCR8056_CALLS_LIST_MISSED);
		DeleteScreenIfPresent(SCR8058_CALLS_LIST_REJECTED);
	#endif

   	}
	else
	{
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__
		if(call_log_on_idle)
		{
			set_pre_tab_pane_display();	
			EntryNewScreen(SCR8055_CALLS_LIST_DIALED, NULL, ReqReadDialedNum, NULL);
			guiBuffer = GetCurrGuiBuffer( SCR8055_CALLS_LIST_DIALED );
			//phoneNames[0]=(PU8)GetString(STR_EMPTY_LIST_NOTIFICATION);
			//Cat23DemoIcons[0]=0;
			ShowCategory53Screen( STR_SCR8056_CAPTION, IMG_SCR8056_CAPTION,
								    0, 0, STR_GLOBAL_BACK,IMG_GLOBAL_BACK, 
								    chis_p->nDialedCalls - nSuited, NULL,NULL,NULL,0,0, guiBuffer );
			/*ShowCategory56Screen( STR_SCR8056_CAPTION, IMG_SCR8056_CAPTION,
		                      0,0, STR_GLOBAL_BACK,
		                      IMG_GLOBAL_BACK, 1, phoneNames,
		                      NULL,
		                      0, guiBuffer );*/
			SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
			set_post_tab_pane_display();
   }
	else
#endif
	{
		DisplayPopup((PU8)GetString(STR_EMPTY_LIST_NOTIFICATION) ,
		             IMG_GLOBAL_EMPTY, 1,
		             CS_NOTIFYDURATION,EMPTY_LIST_TONE);
	}
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTViewDialedCall

  	PURPOSE				: To show the screen containing details
						  of the dialed call.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTViewDialedCall( void )
{

	U8*	guiBuffer;
	U8	   index, noItems, start=0;
#ifdef __PHB_LN_SINGLE_COUNT__
	U16	Cat23DemoIcons[5];
#else
	U16	Cat23DemoIcons[6];
#endif

	EntryNewScreen(SCR8008_CALL_DIALED, ExitCHISTViewDialedCall,
	               NULL, NULL);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	addToHistory = 1;
#endif
	guiBuffer = GetCurrGuiBuffer( SCR8008_CALL_DIALED );
	index = chis_p->currDialedCallIndex;

   if (chis_p->dialedCalls[index].startTime.nDay != 0)
   {
      date_string(&chis_p->dialedCalls[index].startTime,
                  (UI_string_type)subMenuDataPtrs[start],DT_IDLE_SCREEN);
	   Cat23DemoIcons[start++] = IMG_DATE_ENTRY;
      time_string(&chis_p->dialedCalls[index].startTime,
                  (UI_string_type)subMenuDataPtrs[start],DT_IDLE_SCREEN);
	   Cat23DemoIcons[start++] = IMG_TIME_ENTRY;
   }

	if( chis_p->dialedCalls[index].pbName[0] != 0)
	{
	   Cat23DemoIcons[start] = IMG_NAME_ENTRY;
		pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[start++],
		                  (S8 *)chis_p->dialedCalls[index].pbName);
		Cat23DemoIcons[start] = IMG_NUMBER_ENTRY;

		if( chis_p->dialedCalls[index].number[0] != 0)
		   pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[start++],
		                     (S8 *)chis_p->dialedCalls[index].number );
		else
			pfnUnicodeStrncpy( (S8 *)subMenuDataPtrs[start++],
			                   (S8 *)GetString(STR_NO_NUMBER),
			                   MAX_GET_STRING_LEN );

#ifndef __PHB_LN_SINGLE_COUNT__
		sprintf( (S8 *)subMenuDataPtrs[5], "%d ",
		         chis_p->dialedCalls[index].no_of_times );
		Cat23DemoIcons[start] = IMG_COUNT_ENTRY;
		AnsiiToUnicodeString( (PS8)subMenuDataPtrs[start],
		                      (PS8)subMenuDataPtrs[5]);
		pfnUnicodeStrncat( (S8 *)subMenuDataPtrs[start++],
		                   (S8 *)GetString( STR_TIMES ),
		                   MAX_GET_STRING_LEN);
#endif
	}
	else
	{
		Cat23DemoIcons[start] = IMG_NUMBER_ENTRY;

		if( chis_p->dialedCalls[index].number[0] != 0)
		   pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[start++],
		                     (S8 *)chis_p->dialedCalls[index].number );
		else
			pfnUnicodeStrncpy( (S8 *)subMenuDataPtrs[start++],
			                   (S8 *)GetString(STR_NO_NUMBER),
			                   MAX_GET_STRING_LEN );

#ifndef __PHB_LN_SINGLE_COUNT__
		sprintf( (S8 *)subMenuDataPtrs[4], "%d ",
		         chis_p->dialedCalls[index].no_of_times );
		Cat23DemoIcons[start] = IMG_COUNT_ENTRY;
		AnsiiToUnicodeString( (PS8)subMenuDataPtrs[start],
		                      (PS8)subMenuDataPtrs[4]);
		pfnUnicodeStrncat( (S8 *)subMenuDataPtrs[start++],
		                   (S8 *)GetString( STR_TIMES ), MAX_GET_STRING_LEN);
#endif
	}
	 if (mmi_dt_mytime_2_utc_sec(&chis_p->dialedCalls[index].duration) != 0)
    {
        U8 value[20];
        sprintf(
            (S8*) value,
            "%02d:%02d:%02d",
            chis_p->dialedCalls[index].duration.nHour,
            chis_p->dialedCalls[index].duration.nMin,
            chis_p->dialedCalls[index].duration.nSec);
        AnsiiToUnicodeString((S8*) subMenuDataPtrs[start], (S8*) value);
        Cat23DemoIcons[start++] = IMG_ID_CHIST_DURATION;
    }
	noItems = start;
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	ShowCategory84Screen(STR_SCR8069_CAPTION1, IMG_SCR8069_CAPTION,
		                  0, 0,
							   STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						      noItems, subMenuDataPtrs,
		                  Cat23DemoIcons, LIST_MENU, 0, guiBuffer );
#else
	ShowCategory84Screen(STR_SCR8069_CAPTION1, IMG_SCR8069_CAPTION,
		                  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						      STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						      noItems, subMenuDataPtrs,
		                  Cat23DemoIcons, LIST_MENU, 0, guiBuffer );
#endif


#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif		
	
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	SetKeyHandler( EntryCHISTDialedListOptions,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction( EntryCHISTDialedListOptions, KEY_EVENT_UP );
#endif
   #ifdef __MMI_USE_LSK_AS_SEND2__
    #ifdef __MMI_WGUI_CSK_ENABLE__
        SetCenterSoftkeyFunction( EntryCHISTDialedListOptions, KEY_EVENT_UP );
    #endif
        SetKeyHandler(EntryCHISTDialedListOptions,KEY_ENTER,KEY_EVENT_UP);
  #endif
      
      
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_HandleSendKeys(CHISTCallDialedNum,KEY_EVENT_DOWN);
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
	SetKeyHandler(EntryCHISTDialedListOptions,KEY_ENTER,KEY_EVENT_UP );
#else
	SetKeyHandler(CHISTCallDialedNum, KEY_SEND, KEY_EVENT_DOWN);
#endif

	return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTViewDialedCall

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTViewDialedCall( void )
{
	history_t	currHistory;
	S16		nHistory	= 0;

	currHistory.scrnID = SCR8008_CALL_DIALED;
	currHistory.entryFuncPtr = EntryCHISTViewDialedCall;
	pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
	GetCategoryHistory( currHistory.guiBuffer );
	if ( addToHistory )
	   AddHistory( currHistory );
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTDialedListOptions

  	PURPOSE				: To show a screen displaying options
						  available on dialed call list.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTDialedListOptions( void )
{

	U16	nStrItemList[MAX_SUB_MENUS];
	U16	nNumofItem;
	U8*	guiBuffer;

	EntryNewScreen(SCR8009_DIALED_CALL_OPTIONS, ExitCHISTDialedListOptions,
	               NULL, NULL);
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	addToHistory = 1;
#endif
	guiBuffer = GetCurrGuiBuffer( SCR8009_DIALED_CALL_OPTIONS );

#ifdef __IP_NUMBER__
 	SetIPMenuOption(DIALEDCALL_MENU_IP_DIAL);
#endif

	nNumofItem = GetNumOfChild_Ext( DIALED_CALL_LIST_OPTIONS );
	GetSequenceStringIds_Ext( DIALED_CALL_LIST_OPTIONS, nStrItemList );
	SetParentHandler( DIALED_CALL_LIST_OPTIONS );
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif

	if( (chis_p->dialedCalls[chis_p->currDialedCallIndex].number[0] == 0) &&
		 (chis_p->dialedCalls[chis_p->currDialedCallIndex].pbName[0] == 0) )
		nNumofItem -= 3;

	ShowCategory15Screen(STR_SCR8070_CAPTION, IMG_SCR8070_CAPTION,
	                     STR_GLOBAL_OK, IMG_GLOBAL_OK,
	                       STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							     nNumofItem, nStrItemList,
						        (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTDialedListOptions

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTDialedListOptions( void )
{
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__   
   if (IsScreenPresent(SCR8008_CALL_DIALED))
   {
#endif
	   history_t	currHistory;
   	S16		nHistory	= 0;

	   currHistory.scrnID = SCR8009_DIALED_CALL_OPTIONS;
   	currHistory.entryFuncPtr = EntryCHISTDialedListOptions;
	   pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
   	GetCategoryHistory( currHistory.guiBuffer );
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	if ( addToHistory )
#endif
	   AddHistory( currHistory );
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
   }
#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTDeleteDialedCallConfirm

  	PURPOSE				: To delete the current active dialed call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTDeleteDialedCallConfirm( void )
{

	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO,
		get_string(STR_ENTRY_DELETED_CONFIRM), IMG_GLOBAL_QUESTION, WARNING_TONE);


	SetLeftSoftkeyFunction(CHISTDeleteDialedNum,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTViewRejectedCallList

  	PURPOSE				: Rejected call list viewer screen entry function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTViewRejectedCallList( void )
{
	U8*	phoneNumbers[TOT_SIZE_OF_REJECTED_LIST];
	U8*	phoneNames[TOT_SIZE_OF_REJECTED_LIST];
	U8*	guiBuffer;
	U16	Cat23DemoIcons[TOT_SIZE_OF_REJECTED_LIST];
	U8    count;
	U8 nSuited = 0;

	if (isInCall())
		return;

	if( chis_p->nRejectedCallBeforeView != 0 )
	{
		chis_p->nRejectedCallBeforeView = 0;
		mmi_chist_rearrange_rejected_call_status_icon();
		if ( IsScreenPresent( SCR8058_CALLS_LIST_REJECTED) )
			DeleteUptoScrID(SCR_CALL_HISTORY_MAIN_MENU);
	}

	if ( chis_p->nRejectedCalls )
	{
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__		   
	   if(call_log_on_idle)
	   {
		   set_pre_tab_pane_display();
	   }
#endif
	   EntryNewScreen(SCR8058_CALLS_LIST_REJECTED, NULL,
	               ReqReadRejectedNum, NULL);

		guiBuffer = GetCurrGuiBuffer( SCR8058_CALLS_LIST_REJECTED );
		RegisterHighlightHandler( CHISTGetCurrRejectedCallIndex );
		memset(Cat23DemoIcons, 0, sizeof(Cat23DemoIcons));
		CHISTPopulateRejectedCallList( phoneNumbers, phoneNames );
		#ifdef __MMI_VIP_FUNCTION__
		nSuited = g_vip_context.nCallLogSuited;
		#endif
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
		  //HideStatusIcon( STATUS_ICON_SUBLCD_REJECTED_CALL );
                UpdateSubLCDStatusIcons();
#endif

		for( count = 0; count < chis_p->nRejectedCalls - nSuited; count++)
		{
		#ifdef __MMI_MULTI_SIM__
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			Cat23DemoIcons[count] = IMG_MENU8104_REJECTED_CALLS;
			#else
			Cat23DemoIcons[count] = IMG_MTPNP_CARD1_MENU_REJECTED_CALLS + chis_p->nSimId;
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		#else /* __MMI_MULTI_SIM__ */
			Cat23DemoIcons[count] = IMG_MENU8104_REJECTED_CALLS;
		#endif /* __MMI_MULTI_SIM__ */
		}

#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		ShowCategory53Screen( STR_MENU_REJECT_CALL, IMG_SCR8054_CAPTION,
								    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK,
								    IMG_GLOBAL_BACK, chis_p->nRejectedCalls - nSuited, phoneNames,
								    (U16*)Cat23DemoIcons,phoneNumbers,
								    0, chis_p->currRejectedCallIndex, guiBuffer );
#else
		ShowCategory53Screen( STR_MENU_REJECT_CALL, IMG_SCR8054_CAPTION,
		                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
		                      IMG_GLOBAL_BACK, chis_p->nRejectedCalls - nSuited, phoneNames,
		                      (U16*)Cat23DemoIcons,phoneNumbers,
		                      0, chis_p->currRejectedCallIndex, guiBuffer );
#endif

#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif		
		#ifndef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		#else
		SetKeyHandler( CHISViewCtrlByLeftArrowKey,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		#endif
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		SetKeyHandler( EntryCHISTRejectedListOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetLeftSoftkeyFunction( EntryCHISTRejectedListOptions, KEY_EVENT_UP );
#else
		#ifndef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		SetKeyHandler( EntryCHISTViewRejectedCall,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		#else
		SetKeyHandler( CHISViewCtrlByRgihtArrowKey,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		#endif
		SetLeftSoftkeyFunction( EntryCHISTViewRejectedCall, KEY_EVENT_UP );
#endif
 #ifdef __MMI_USE_LSK_AS_SEND2__
    #ifdef __MMI_WGUI_CSK_ENABLE__
              SetCenterSoftkeyFunction( EntryCHISTViewRejectedCall, KEY_EVENT_UP );
    #endif
              SetKeyHandler(EntryCHISTViewRejectedCall,KEY_ENTER,KEY_EVENT_UP);
#endif
		SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#ifdef __MMI_MULTI_SIM__
	    MTPNP_PFAL_CC_HandleSendKeys(CHISTCallRejectedNum,KEY_EVENT_DOWN);
	    ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	    SetKeyHandler(EntryCHISTRejectedListOptions,KEY_ENTER,KEY_EVENT_UP );
#else
	    SetKeyHandler(EntryCHISTViewRejectedCall,KEY_ENTER,KEY_EVENT_UP );
#endif

#else
	    SetKeyHandler(CHISTCallRejectedNum, KEY_SEND, KEY_EVENT_DOWN);
#endif


#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	   if(call_log_on_idle)
	   {
		   set_post_tab_pane_display();
	   }
#endif
	#ifdef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		DeleteScreenIfPresent(SCR8056_CALLS_LIST_MISSED);
		DeleteScreenIfPresent(SCR8055_CALLS_LIST_DIALED);
    DeleteScreenIfPresent(SCR8054_CALLS_LIST_RECEIVED);
	#endif
	}
	else
	{
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__
	   if(call_log_on_idle)
	   {
		set_pre_tab_pane_display();
		EntryNewScreen(SCR8058_CALLS_LIST_REJECTED NULL, ReqReadRecvdNum, NULL);
		guiBuffer = GetCurrGuiBuffer( SCR8058_CALLS_LIST_REJECTED );
		//phoneNames[0]=(PU8)GetString(STR_EMPTY_LIST_NOTIFICATION);
		//Cat23DemoIcons[0]=0;
		ShowCategory53Screen( STR_MENU_REJECT_CALL, IMG_SCR8054_CAPTION,
								    0, 0, STR_GLOBAL_BACK,IMG_GLOBAL_BACK, 
								    chis_p->nRejectedCalls - nSuited, NULL,NULL,NULL,0,0, guiBuffer );
		/*ShowCategory56Screen( STR_SCR8054_CAPTION, IMG_SCR8054_CAPTION,
		               0,0, STR_GLOBAL_BACK,
		               IMG_GLOBAL_BACK, 1, phoneNames,
		               NULL,
		               0, guiBuffer );*/
		SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
		set_post_tab_pane_display();
	   }
	   else
#endif
	   {
   mmi_trace(1,"	DisplayPopup((PU8)GetString(STR_EMPTY_LIST_NOTIFICATION),@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
		DisplayPopup((PU8)GetString(STR_EMPTY_LIST_NOTIFICATION),
		             IMG_GLOBAL_EMPTY, 1,
		             CS_NOTIFYDURATION, EMPTY_LIST_TONE);
	}
	}
	return;
}


/**************************************************************

	FUNCTION NAME		: EntryCHISTViewMissedCallList

  	PURPOSE				: Missed call list viewer screen entry function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearSoundWarnningTimer(void);
void EntryCHISTViewMissedCallList( void )
{
	U8*	phoneNumbers[TOT_SIZE_OF_MISSED_LIST];
	U8*	phoneNames[TOT_SIZE_OF_MISSED_LIST];
	U8*	guiBuffer;
	U16	Cat23DemoIcons[TOT_SIZE_OF_MISSED_LIST];
	U8    count;
	U8 nSuited = 0;

	if (isInCall())
		return;

	if( chis_p->nMissedCallBeforeView != 0 )
	{
		ClearSoundWarnningTimer();
		chis_p->nMissedCallBeforeView = 0;
		mmi_chist_rearrange_missed_call_status_icon();
#ifdef __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__
   	 	mmi_chist_set_num_missed_call_before_view();
#endif

		   /* re-enter:
		      call history menu -> missed list -> X missed calls -> missed list */
		if ( IsScreenPresent( SCR8056_CALLS_LIST_MISSED ) )
			DeleteUptoScrID(SCR_CALL_HISTORY_MAIN_MENU);
	}

	if ( chis_p->nMissedCalls )
	{
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	if(call_log_on_idle)
	{
	   set_pre_tab_pane_display();
	}
#endif
	   EntryNewScreen(SCR8056_CALLS_LIST_MISSED, NULL,
	               ReqReadMissedNum, NULL);

		guiBuffer = GetCurrGuiBuffer( SCR8056_CALLS_LIST_MISSED );
		RegisterHighlightHandler( CHISTGetCurrMissedCallIndex );
		CHISTPopulateMissedCallList( phoneNumbers, phoneNames );
		#ifdef __MMI_VIP_FUNCTION__
		nSuited = g_vip_context.nCallLogSuited;
		#endif
                HideStatusIcon( STATUS_ICON_MISSED_CALL );
 
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
		HideStatusIcon( STATUS_ICON_SUBLCD_MISSED_CALL );
                UpdateSubLCDStatusIcons();
#endif
 
		for( count = 0; count < chis_p->nMissedCalls; count++)
		{
		#ifdef __MMI_MULTI_SIM__
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			Cat23DemoIcons[count] = IMG_MENU8103_MISSED_CALLS;
			#else
			Cat23DemoIcons[count] = IMG_MTPNP_CARD1_MENU_MISSED_CALLS + chis_p->nSimId;
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		#else /* __MMI_MULTI_SIM__ */
			Cat23DemoIcons[count] = IMG_MENU8103_MISSED_CALLS;
		#endif /* __MMI_MULTI_SIM__ */
		}

#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		ShowCategory53Screen( STR_SCR8055_CAPTION, IMG_SCR8055_CAPTION,
								    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK,
								    IMG_GLOBAL_BACK, chis_p->nMissedCalls -nSuited, phoneNames,
								    (U16*)Cat23DemoIcons, phoneNumbers, 0,
								    chis_p->currMissedCallIndex, guiBuffer );
#else
		ShowCategory53Screen( STR_SCR8055_CAPTION, IMG_SCR8055_CAPTION,
		                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
		                      IMG_GLOBAL_BACK, chis_p->nMissedCalls - nSuited, phoneNames,
		                      (U16*)Cat23DemoIcons, phoneNumbers, 0,
		                      chis_p->currMissedCallIndex, guiBuffer );
#endif

#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif		
		#ifndef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		#else
		SetKeyHandler( CHISViewCtrlByLeftArrowKey,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		#endif
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		SetKeyHandler( EntryCHISTMissedListOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetLeftSoftkeyFunction( EntryCHISTMissedListOptions, KEY_EVENT_UP );
#else
		#ifndef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		SetKeyHandler( EntryCHISTViewMissedCall,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		#else
		SetKeyHandler( CHISViewCtrlByRgihtArrowKey,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		#endif
		SetLeftSoftkeyFunction( EntryCHISTViewMissedCall, KEY_EVENT_UP );
#endif 
#ifdef __MMI_USE_LSK_AS_SEND2__
    #ifdef __MMI_WGUI_CSK_ENABLE__
               SetCenterSoftkeyFunction( EntryCHISTViewMissedCall, KEY_EVENT_UP );
    #endif
              SetKeyHandler(EntryCHISTViewMissedCall,KEY_ENTER,KEY_EVENT_UP);
#endif
		SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#ifdef __MMI_MULTI_SIM__
	    MTPNP_PFAL_CC_HandleSendKeys(CHISTCallMissedNum,KEY_EVENT_DOWN);
	    ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	    SetKeyHandler(EntryCHISTMissedListOptions,KEY_ENTER,KEY_EVENT_UP );
#else
	    SetKeyHandler(EntryCHISTViewMissedCall,KEY_ENTER,KEY_EVENT_UP );
#endif 

#else
	    SetKeyHandler(CHISTCallMissedNum, KEY_SEND, KEY_EVENT_DOWN);
#endif


#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	if(call_log_on_idle)
	{
	   set_post_tab_pane_display();
	}
#endif
	#ifdef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		DeleteScreenIfPresent(SCR8054_CALLS_LIST_RECEIVED);
		DeleteScreenIfPresent(SCR8055_CALLS_LIST_DIALED);
		DeleteScreenIfPresent(SCR8058_CALLS_LIST_REJECTED);
	#endif
   }
   else
   {
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__
	if(call_log_on_idle)
	{
	   set_pre_tab_pane_display();
	   EntryNewScreen(SCR8056_CALLS_LIST_MISSED, NULL, ReqReadMissedNum, NULL);
	   guiBuffer = GetCurrGuiBuffer( SCR8056_CALLS_LIST_MISSED );
	   //phoneNames[0]=(PU8)GetString(STR_EMPTY_LIST_NOTIFICATION);
	   //Cat23DemoIcons[0]=0;
	   ShowCategory53Screen( STR_SCR8055_CAPTION, IMG_SCR8055_CAPTION,
								    0, 0, STR_GLOBAL_BACK,IMG_GLOBAL_BACK, 
								    chis_p->nMissedCalls - nSuited, NULL,NULL,NULL,0,0, guiBuffer );

	   /*ShowCategory56Screen( STR_SCR8055_CAPTION, IMG_SCR8055_CAPTION,
	               0,0, STR_GLOBAL_BACK,
	               IMG_GLOBAL_BACK, 1, phoneNames,
	               NULL,
	               0, guiBuffer );*/
	   SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	   set_post_tab_pane_display();
	}
	else
#endif
	{
		DisplayPopup((PU8)GetString(STR_EMPTY_LIST_NOTIFICATION),
		             IMG_GLOBAL_EMPTY, 1,CS_NOTIFYDURATION,EMPTY_LIST_TONE);
	}
   }

#ifdef __MMI_MULTI_SIM__
	DeleteScreenIfPresent(SCR_MTPNP_CALL_HISTORY_NOTIFY);
#endif

	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTViewMissedCall

  	PURPOSE				: To show the screen containing details
						  of the missed call.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTViewMissedCall( void )
{
	U8*	guiBuffer;
	U8	   noItems, index;
#ifdef __PHB_LN_SINGLE_COUNT__
	U16	Cat23DemoIcons[4];
#else
	U16	Cat23DemoIcons[5];
#endif

mmi_trace(1,"EntryCHISTViewMissedCall###############################################################\n");

	EntryNewScreen(SCR8520_CALL_MISSED, ExitCHISTViewMissedCall,
	               NULL, NULL);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	addToHistory = 1;
#endif
	guiBuffer = GetCurrGuiBuffer( SCR8520_CALL_MISSED );
	index = chis_p->currMissedCallIndex;

   date_string(&chis_p->missedCalls[index].startTime,
               (UI_string_type)subMenuDataPtrs[0],DT_IDLE_SCREEN);
	time_string(&chis_p->missedCalls[index].startTime,
	            (UI_string_type)subMenuDataPtrs[1],DT_IDLE_SCREEN);

	if( chis_p->missedCalls[index].pbName[0] != 0 )
	{
		pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[2],
		                  (S8 *)chis_p->missedCalls[index].pbName);

		if( chis_p->missedCalls[index].number[0] != 0 )
		   pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[3],
		                     (S8 *)chis_p->missedCalls[index].number );
		else
			pfnUnicodeStrncpy( (S8 *)subMenuDataPtrs[3],
			                   (S8 *)GetString(STR_NO_NUMBER),
			                   MAX_GET_STRING_LEN );
#ifdef __PHB_LN_SINGLE_COUNT__
		noItems = 4;
#else
		sprintf( (S8 *)subMenuDataPtrs[5], "%d ",
		         chis_p->missedCalls[index].no_of_times );
		AnsiiToUnicodeString( (PS8)subMenuDataPtrs[4],
		                      (PS8)subMenuDataPtrs[5]);
		pfnUnicodeStrncat( (S8 *)subMenuDataPtrs[4],
		                   (S8 *)GetString( STR_TIMES ),
		                   MAX_GET_STRING_LEN );

		noItems = 5;
		Cat23DemoIcons[4] = IMG_COUNT_ENTRY;
#endif
		Cat23DemoIcons[0] = IMG_DATE_ENTRY;
		Cat23DemoIcons[1] = IMG_TIME_ENTRY;
		Cat23DemoIcons[2] = IMG_NAME_ENTRY;
		Cat23DemoIcons[3] = IMG_NUMBER_ENTRY;
	}
	else
	{
		if( chis_p->missedCalls[index].number[0] != 0 )
		   pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[2],
		                     (S8 *)chis_p->missedCalls[index].number );
		else
			pfnUnicodeStrncpy( (S8 *)subMenuDataPtrs[2],
			                   (S8 *)GetString(STR_NO_NUMBER),
			                   MAX_GET_STRING_LEN );
#ifdef __PHB_LN_SINGLE_COUNT__
		noItems = 3;
#else
		sprintf( (S8 *)subMenuDataPtrs[4], "%d ",
		         chis_p->missedCalls[index].no_of_times );
		AnsiiToUnicodeString( (PS8)subMenuDataPtrs[3],
		                      (PS8)subMenuDataPtrs[4]);
		pfnUnicodeStrncat( (S8 *)subMenuDataPtrs[3],
		                   (S8 *)GetString( STR_TIMES ),
		                   MAX_GET_STRING_LEN);

		noItems = 4;
		Cat23DemoIcons[3] = IMG_COUNT_ENTRY;
#endif
		Cat23DemoIcons[0] = IMG_DATE_ENTRY;
		Cat23DemoIcons[1] = IMG_TIME_ENTRY;
		Cat23DemoIcons[2] = IMG_NUMBER_ENTRY;
	}
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	ShowCategory84Screen( STR_SCR8069_CAPTION1, IMG_SCR8069_CAPTION,
							    0, 0,
						       STR_GLOBAL_BACK, IMG_GLOBAL_BACK, noItems,
						       subMenuDataPtrs, Cat23DemoIcons,LIST_MENU,
						       0, guiBuffer );
#else
	ShowCategory84Screen( STR_SCR8069_CAPTION1, IMG_SCR8069_CAPTION,
		                  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						      STR_GLOBAL_BACK, IMG_GLOBAL_BACK, noItems,
						      subMenuDataPtrs, Cat23DemoIcons,LIST_MENU,
						      0, guiBuffer );
#endif

#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif		
	
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	SetKeyHandler( EntryCHISTMissedListOptions,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction( EntryCHISTMissedListOptions, KEY_EVENT_UP );
#endif
#ifdef  __MMI_USE_LSK_AS_SEND2__
    #ifdef __MMI_WGUI_CSK_ENABLE__
        SetCenterSoftkeyFunction( EntryCHISTMissedListOptions, KEY_EVENT_UP );
    #endif
        SetKeyHandler(EntryCHISTMissedListOptions,KEY_ENTER,KEY_EVENT_UP);
#endif
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#ifdef __MMI_MULTI_SIM__
	    MTPNP_PFAL_CC_HandleSendKeys(CHISTCallMissedNum,KEY_EVENT_DOWN);

		ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
		
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
		SetKeyHandler(EntryCHISTMissedListOptions,KEY_ENTER,KEY_EVENT_UP );
#endif
#ifdef  __MMI_USE_LSK_AS_SEND2__
		SetKeyHandler(EntryCHISTMissedListOptions,KEY_ENTER,KEY_EVENT_UP );
#endif

#else
		    SetKeyHandler(CHISTCallMissedNum, KEY_SEND, KEY_EVENT_DOWN);
#endif

	return;
}

void EntryCHISTViewRejectedCall( void )
{
	U8*	guiBuffer;
	U8	   noItems, index;
#ifdef __PHB_LN_SINGLE_COUNT__
	U16	Cat23DemoIcons[4];
#else
	U16	Cat23DemoIcons[5];
#endif

mmi_trace(1,"EntryCHISTViewRejectedCall###############################################################\n");

	EntryNewScreen(SCR8522_CALL_REJECTED, ExitCHISTViewRejectedCall,
	               NULL, NULL);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	addToHistory = 1;
#endif
	guiBuffer = GetCurrGuiBuffer( SCR8522_CALL_REJECTED );
	index = chis_p->currRejectedCallIndex;

   date_string(&chis_p->rejectedCalls[index].startTime,
               (UI_string_type)subMenuDataPtrs[0],DT_IDLE_SCREEN);
	time_string(&chis_p->rejectedCalls[index].startTime,
	            (UI_string_type)subMenuDataPtrs[1],DT_IDLE_SCREEN);

	if( chis_p->rejectedCalls[index].pbName[0] != 0 )
	{
		pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[2],
		                  (S8 *)chis_p->rejectedCalls[index].pbName);

		if( chis_p->rejectedCalls[index].number[0] != 0 )
		   pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[3],
		                     (S8 *)chis_p->rejectedCalls[index].number );
		else
			pfnUnicodeStrncpy( (S8 *)subMenuDataPtrs[3],
			                   (S8 *)GetString(STR_NO_NUMBER),
			                   MAX_GET_STRING_LEN );
#ifdef __PHB_LN_SINGLE_COUNT__
		noItems = 4;
#else
		sprintf( (S8 *)subMenuDataPtrs[5], "%d ",
		         chis_p->rejectedCalls[index].no_of_times );
		AnsiiToUnicodeString( (PS8)subMenuDataPtrs[4],
		                      (PS8)subMenuDataPtrs[5]);
		pfnUnicodeStrncat( (S8 *)subMenuDataPtrs[4],
		                   (S8 *)GetString( STR_TIMES ),
		                   MAX_GET_STRING_LEN );

		noItems = 5;
		Cat23DemoIcons[4] = IMG_COUNT_ENTRY;
#endif
		Cat23DemoIcons[0] = IMG_DATE_ENTRY;
		Cat23DemoIcons[1] = IMG_TIME_ENTRY;
		Cat23DemoIcons[2] = IMG_NAME_ENTRY;
		Cat23DemoIcons[3] = IMG_NUMBER_ENTRY;
	}
	else
	{
		if( chis_p->rejectedCalls[index].number[0] != 0 )
		   pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[2],
		                     (S8 *)chis_p->rejectedCalls[index].number );
		else
			pfnUnicodeStrncpy( (S8 *)subMenuDataPtrs[2],
			                   (S8 *)GetString(STR_NO_NUMBER),
			                   MAX_GET_STRING_LEN );
#ifdef __PHB_LN_SINGLE_COUNT__
		noItems = 3;
#else
		sprintf( (S8 *)subMenuDataPtrs[4], "%d ",
		         chis_p->rejectedCalls[index].no_of_times );
		AnsiiToUnicodeString( (PS8)subMenuDataPtrs[3],
		                      (PS8)subMenuDataPtrs[4]);
		pfnUnicodeStrncat( (S8 *)subMenuDataPtrs[3],
		                   (S8 *)GetString( STR_TIMES ),
		                   MAX_GET_STRING_LEN);

		noItems = 4;
		Cat23DemoIcons[3] = IMG_COUNT_ENTRY;
#endif
		Cat23DemoIcons[0] = IMG_DATE_ENTRY;
		Cat23DemoIcons[1] = IMG_TIME_ENTRY;
		Cat23DemoIcons[2] = IMG_NUMBER_ENTRY;
	}
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	ShowCategory84Screen( STR_SCR8069_CAPTION1, IMG_SCR8069_CAPTION,
							    0, 0,
						       STR_GLOBAL_BACK, IMG_GLOBAL_BACK, noItems,
						       subMenuDataPtrs, Cat23DemoIcons,LIST_MENU,
						       0, guiBuffer );
#else
	ShowCategory84Screen( STR_SCR8069_CAPTION1, IMG_SCR8069_CAPTION,
		                  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						      STR_GLOBAL_BACK, IMG_GLOBAL_BACK, noItems,
						      subMenuDataPtrs, Cat23DemoIcons,LIST_MENU,
						      0, guiBuffer );
#endif

#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif		
	
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	SetKeyHandler( EntryCHISTRejectedListOptions,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction( EntryCHISTRejectedListOptions, KEY_EVENT_UP );
#endif
#ifdef __MMI_USE_LSK_AS_SEND2__
    #ifdef __MMI_WGUI_CSK_ENABLE__
        SetCenterSoftkeyFunction( EntryCHISTRejectedListOptions, KEY_EVENT_UP );
    #endif
        SetKeyHandler(EntryCHISTRejectedListOptions,KEY_ENTER,KEY_EVENT_UP);
#endif
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#ifdef __MMI_MULTI_SIM__
	    MTPNP_PFAL_CC_HandleSendKeys(CHISTCallRejectedNum,KEY_EVENT_DOWN);
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	SetKeyHandler(EntryCHISTRejectedListOptions,KEY_ENTER,KEY_EVENT_UP );
#endif

#else 
	    SetKeyHandler(CHISTCallRejectedNum, KEY_SEND, KEY_EVENT_DOWN);
#endif

	return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTViewMissedCall

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTViewMissedCall( void )
{
	history_t	currHistory;
	S16		nHistory	= 0;

	currHistory.scrnID = SCR8520_CALL_MISSED;
	currHistory.entryFuncPtr = EntryCHISTViewMissedCall;
	pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
	GetCategoryHistory( currHistory.guiBuffer );
	if ( addToHistory )
	   AddHistory( currHistory );
	return;
}

void ExitCHISTViewRejectedCall( void )
{
	history_t	currHistory;
	S16		nHistory	= 0;

	currHistory.scrnID = SCR8522_CALL_REJECTED;
	currHistory.entryFuncPtr = EntryCHISTViewRejectedCall;
	pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
	GetCategoryHistory( currHistory.guiBuffer );
	if ( addToHistory )
	   AddHistory( currHistory );
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTMissedListOptions

  	PURPOSE				: To show a screen displaying options
						  available on Missed call list.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTMissedListOptions( void )
{
	U16	nStrItemList[MAX_SUB_MENUS];
	U16	nNumofItem;
	U8*	guiBuffer;

	EntryNewScreen(SCR8521_MISSED_CALL_OPTIONS, ExitCHISTMissedListOptions, NULL, NULL);
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	addToHistory = 1;
#endif
	guiBuffer = GetCurrGuiBuffer( SCR8521_MISSED_CALL_OPTIONS );

#ifdef __IP_NUMBER__
 	SetIPMenuOption(MISSED_CALL_MENU_IP_DIAL);
#endif

	nNumofItem = GetNumOfChild_Ext( MISSED_CALL_LIST_OPTIONS );
	GetSequenceStringIds_Ext( MISSED_CALL_LIST_OPTIONS, nStrItemList );
	SetParentHandler( MISSED_CALL_LIST_OPTIONS );
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif

	nMissedCallNumBkp = chis_p->nMissedCalls;

	if (chis_p->missedCalls[chis_p->currMissedCallIndex].number[0] == 0)
		nNumofItem -= 3;

	ShowCategory15Screen(STR_SCR8071_CAPTION, IMG_SCR8071_CAPTION,
	                     STR_GLOBAL_OK, IMG_GLOBAL_OK,
	                     STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							   nNumofItem, nStrItemList,
						      (U16 *)gIndexIconsImageList, 1, 0, guiBuffer );

	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void EntryCHISTRejectedListOptions( void )
{
	U16	nStrItemList[MAX_SUB_MENUS];
	U16	nNumofItem;
	U8*	guiBuffer;
mmi_trace(1,"EntryCHISTRejectedListOptions###############################################################\n");

	EntryNewScreen(SCR8523_REJECTED_CALL_OPTIONS, ExitCHISTRejectedListOptions,
	               NULL, NULL);
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	addToHistory = 1;
#endif
	guiBuffer = GetCurrGuiBuffer( SCR8523_REJECTED_CALL_OPTIONS );

#ifdef __IP_NUMBER__
 	SetIPMenuOption(REJECTED_CALL_MENU_IP_DIAL);
#endif

	nNumofItem = GetNumOfChild_Ext( REJECTED_CALL_LIST_OPTIONS );
	GetSequenceStringIds_Ext( REJECTED_CALL_LIST_OPTIONS, nStrItemList );
	SetParentHandler( REJECTED_CALL_LIST_OPTIONS );
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif

	nMissedCallNumBkp = chis_p->nRejectedCalls;

	if (chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number[0] == 0)
		nNumofItem -= 3;

	ShowCategory15Screen(STR_RejectedSCR_CAPTION, IMG_SCR8071_CAPTION,
	                     STR_GLOBAL_OK, IMG_GLOBAL_OK,
	                     STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							   nNumofItem, nStrItemList,
						      (U16 *)gIndexIconsImageList, 1, 0, guiBuffer );

	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTMissedListOptions

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTMissedListOptions( void )
{
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
   if (IsScreenPresent(SCR8520_CALL_MISSED))
   {
#endif
   	history_t	currHistory;
	   S16		nHistory	= 0;

   	currHistory.scrnID = SCR8521_MISSED_CALL_OPTIONS;
	   currHistory.entryFuncPtr = EntryCHISTMissedListOptions;
   	pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
	   GetCategoryHistory( currHistory.guiBuffer );
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	if ( addToHistory )
#endif
   	AddHistory( currHistory );
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__   
   }
#endif
	return;
}

void ExitCHISTRejectedListOptions( void )
{
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
   if (IsScreenPresent(SCR8522_CALL_REJECTED))
   {
#endif
   	history_t	currHistory;
	   S16		nHistory	= 0;

   	currHistory.scrnID = SCR8523_REJECTED_CALL_OPTIONS;
	   currHistory.entryFuncPtr = EntryCHISTRejectedListOptions;
   	pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
	   GetCategoryHistory( currHistory.guiBuffer );
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	if ( addToHistory )
#endif
   	AddHistory( currHistory );
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__   
   }
#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTDeleteMissedCallConfirm

  	PURPOSE				: To delete the current active dialed call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTDeleteMissedCallConfirm( void )
{

	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO,
		get_string(STR_ENTRY_DELETED_CONFIRM), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(CHISTDeleteMissedNum,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}

/*****************************************************************************
 * FUNCTION
 *  EntryCHISTNotifyMissedCall
 * DESCRIPTION
 *  Notify user of missed call presence when
 *  entering IDLE screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryCHISTNotifyMissedCall(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
#ifdef __MMI_MULTI_SIM__
	U8 MissedCallFlag = MMI_FALSE; // if is more than one card has missed calls
	UI_character_type temp[10];
	U8 tmp_blank[5];
	U8 tmp_enter[5];
	U8 tmp_colon[10];
	U8 i;
	U8 MissedCallBeforeView;
	BOOL bFirstLine = MMI_TRUE;
	U8 nTotalMissed = 0;
#endif

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
#ifdef __MMI_MULTI_SIM__
	EntryNewScreen(ITEM_SCR_MISSED_CALL, ExitCHISTNotifyMissedCall, NULL, NULL);
	memset(CHISTMsgStr, 0, sizeof(CHISTMsgStr));

	AnsiiToUnicodeString(tmp_blank, " ");
	AnsiiToUnicodeString(tmp_colon, ": ");
	AnsiiToUnicodeString(tmp_enter, "\n");
	#ifndef __MMI_MAINLCD_128X160__
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(MTPNP_PFAL_Is_Card_Usable(i))
		{
			MissedCallBeforeView = CHISTGetMissedCallCountBeforeView(i);
			if(MissedCallBeforeView != 0)
			{
				if(!bFirstLine)
				{
					pfnUnicodeStrcat((PS8) CHISTMsgStr, (PS8)tmp_enter);
					MissedCallFlag = MMI_TRUE;
				}
				else
				{
					bFirstLine = MMI_FALSE;
				}
				
				#if !defined( __MMI_DUAL_SIM_SINGLE_OPENED_)
				pfnUnicodeStrcat((PS8) CHISTMsgStr, GetString(STRING_MTPNP_SIM1+i));
				pfnUnicodeStrcat((PS8) CHISTMsgStr, (PS8) tmp_colon);
				#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */

				memset(temp, 0, sizeof(temp));
				coolsand_UI_itoa(MissedCallBeforeView, temp, 10);
				pfnUnicodeStrcat((PS8) CHISTMsgStr, (const S8 *)temp);
				pfnUnicodeStrcat((PS8) CHISTMsgStr, (PS8) tmp_blank);

				if (MissedCallBeforeView == 1)
				{
					pfnUnicodeStrcat((PS8) CHISTMsgStr, (PS8) GetString(STR_MISSED_CALL));
				}
				else
				{
					pfnUnicodeStrcat((PS8) CHISTMsgStr, (PS8) GetString(STR_MISSED_CALLS));
				}
			}
			
		}
	}
	#else/*__MMI_MAINLCD_128X160__*/
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		nTotalMissed += CHISTGetMissedCallCountBeforeView(i);
	}
	if(nTotalMissed == 1)
	{
		pfnUnicodeStrcat((PS8) CHISTMsgStr, (PS8) GetString(STR_MISSED_CALL));
	}
	else
	{
		pfnUnicodeStrcat((PS8) CHISTMsgStr, (PS8) GetString(STR_MISSED_CALLS));
	}
	
	bFirstLine = MMI_FALSE;
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(MTPNP_PFAL_Is_Card_Usable(i))
		{
			MissedCallBeforeView = CHISTGetMissedCallCountBeforeView(i);
			if(MissedCallBeforeView != 0)
			{
				if(!bFirstLine)
				{
					pfnUnicodeStrcat((PS8) CHISTMsgStr, (PS8)tmp_enter);
					MissedCallFlag = MMI_TRUE;
				}
				else
				{
					bFirstLine = MMI_FALSE;
				}
				
				#if !defined( __MMI_DUAL_SIM_SINGLE_OPENED_)
				pfnUnicodeStrcat((PS8) CHISTMsgStr, GetString(STRING_MTPNP_SIM1+i));
				pfnUnicodeStrcat((PS8) CHISTMsgStr, (PS8) tmp_colon);
				#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */

				memset(temp, 0, sizeof(temp));
				coolsand_UI_itoa(MissedCallBeforeView, temp, 10);
				pfnUnicodeStrcat((PS8) CHISTMsgStr, (const S8 *)temp);
			}
			
		}
	}
	#endif/*__MMI_MAINLCD_128X160__*/
#else /* __MMI_MULTI_SIM__ */
	EntryNewScreen(ITEM_SCR_MISSED_CALL, ExitCHISTNotifyMissedCall, NULL, NULL);

	memset(CHISTMsgStr, 0, sizeof(CHISTMsgStr));
	coolsand_UI_itoa(chis_p->nMissedCallBeforeView, (UI_character_type*) CHISTMsgStr, 10);

	pfnUnicodeStrcat((PS8) CHISTMsgStr, " ");
	if (chis_p->nMissedCallBeforeView == 1)
	{
		UCS2Strncat((PS8) CHISTMsgStr, (PS8) GetString(STR_MISSED_CALL), 20);
	}
	else
	{
		UCS2Strncat((PS8) CHISTMsgStr, (PS8) GetString(STR_MISSED_CALLS), 20);
	}
#endif /* __MMI_MULTI_SIM__ */

	if (gKeyPadLockFlag == 0)
	{
		chis_p->saveMissedScrn = TRUE;
		ShowCategory141Screen( 0, 0,
						MISSED_LSK_STR, 0, STR_GLOBAL_BACK, 0, 
						(U8*)CHISTMsgStr, MISSED_LSK_ICN, NULL );
		SetKeyHandler(CHISTIgnoreMissedCallScreen, KEY_END, KEY_EVENT_DOWN);

		#ifndef __MMI_MULTI_SIM__
		SetLeftSoftkeyFunction(CHISTReadMissedNum, KEY_EVENT_UP);
		#else
		SetLeftSoftkeyFunction(MTPNP_PFAL_ReadDualModeMissedNum, KEY_EVENT_UP);
		#endif
		SetRightSoftkeyFunction(CHISTIgnoreMissedCallScreen, KEY_EVENT_UP);
		#ifndef __MMI_MULTI_SIM__
		SetMissedCallFlag(FALSE);
		#else
		if (!MissedCallFlag)
		{
			SetMissedCallFlag(FALSE);
		}
		#endif
		chis_p->currMissedCallIndex = 0;
	}
	else // keypad is locked
	{
		chis_p->saveMissedScrn = FALSE;
		#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
		ShowCategory141Screen(0, 0, 0, 0, KEYPAD_LOCKED_RSK_TEXT, 0, (U8*)CHISTMsgStr,
							MISSED_LSK_ICN, NULL);
		#else // __MMI_KEYPAD_LOCK_PATTERN_2__
		ShowCategory141Screen(0, 0, KEYPAD_LOCKED_RSK_TEXT, 0, 0, 0, (U8*)CHISTMsgStr, 
							MISSED_LSK_ICN, NULL);
		#endif
		ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
  	 }
   	 return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTNotifyMissedCall(void)

	PURPOSE				: This function is the exit screen for the Active Call and
						  incoming options

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

RETURNS				: void

 

**************************************************************/
void ExitCHISTNotifyMissedCall(void)
{
	if(chis_p->saveMissedScrn == TRUE)
	{
	   U16 nHistory = 0;
   	history_t scr101;

		pfnUnicodeStrcpy((S8*)scr101.inputBuffer, (PS8)&nHistory);
		scr101.scrnID = ITEM_SCR_MISSED_CALL;
		scr101.entryFuncPtr = EntryCHISTNotifyMissedCall;
		GetCategoryHistory(scr101.guiBuffer);
		AddHistory(scr101);
	}/*else
	{
		GoBackSubLCDHistory();
	}
	return;*/
}

/**************************************************************

	FUNCTION NAME		: EntrySubCHISTNotifyMissedCall

	PURPOSE				: Entry screen for Sub Lcd Missed Call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
extern FuncPtr GetCurrSubLCDScreenFunc(void);
void EntrySubCHISTNotifyMissedCall(void)
{
    if( (GetCurrSubLCDScreenFunc() == ExitSubCHISTNotifyMissedCall) )
     return;
    
	ExecSubLCDCurrExitHandler();

	//DeleteSubLCDScreen(SUB_LCD_ITEM_SCR_MISSED_CALL);
	ShowCategory304Screen(IMG_CM_STATE_MISSED,NULL);
        /* do not add sub LCD to history, the main screen will force to show sub LCD anyway */
	//SetSubLCDExitHandler(ExitSubCHISTNotifyMissedCall);
	return;
}

/**************************************************************

	FUNCTION NAME		: ExitSubCHISTNotifyMissedCall

	PURPOSE				: Exit Handler for Sub Lcd Missed Call Screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ExitSubCHISTNotifyMissedCall(void)
{
	if (chis_p->saveMissedScrn == TRUE && 
		!IsSubLCDScreenPresent(SUB_LCD_ITEM_SCR_MISSED_CALL))
	{
	   SubLCDHistoryNode SubLCDHistory;

		SubLCDHistory.entryFuncPtr=EntrySubCHISTNotifyMissedCall;
		AddSubLCDHistoryWithScrID(&SubLCDHistory,SUB_LCD_ITEM_SCR_MISSED_CALL);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTViewRecvdCallList

  	PURPOSE				: Recvd call list viewer screen entry function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTViewRecvdCallList( void )
{
	U8*	phoneNumbers[TOT_SIZE_OF_RECVD_LIST];
	U8*	phoneNames[TOT_SIZE_OF_RECVD_LIST];
	U8*	guiBuffer;
	U16	Cat23DemoIcons[TOT_SIZE_OF_RECVD_LIST];
	U8    count;
	U8 nSuited = 0;

	if (isInCall())
		return;

	if ( chis_p->nRecvdCalls )
	{
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__		   
	   if(call_log_on_idle)
	   {
		   set_pre_tab_pane_display();
	   }
#endif
	   EntryNewScreen(SCR8054_CALLS_LIST_RECEIVED, NULL,
	               ReqReadRecvdNum, NULL);

		guiBuffer = GetCurrGuiBuffer( SCR8054_CALLS_LIST_RECEIVED );
		RegisterHighlightHandler( CHISTGetCurrRecvdCallIndex );
		memset(Cat23DemoIcons, 0, sizeof(Cat23DemoIcons));
		CHISTPopulateRecvdCallList( phoneNumbers, phoneNames );
		#ifdef __MMI_VIP_FUNCTION__
		nSuited = g_vip_context.nCallLogSuited;
		#endif

		for( count = 0; count < chis_p->nRecvdCalls - nSuited; count++)
		{
		#ifdef __MMI_MULTI_SIM__
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			Cat23DemoIcons[count] = IMG_MENU8101_RECEIVED_CALLS;
			#else
			Cat23DemoIcons[count] = IMG_MTPNP_CARD1_MENU_RECEIVED_CALLS + chis_p->nSimId;
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		#else /* __MMI_MULTI_SIM__ */
			Cat23DemoIcons[count] = IMG_MENU8101_RECEIVED_CALLS;
		#endif /* __MMI_MULTI_SIM__ */

		}

#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		ShowCategory53Screen( STR_SCR8054_CAPTION, IMG_SCR8054_CAPTION,
								    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK,
								    IMG_GLOBAL_BACK, chis_p->nRecvdCalls - nSuited, phoneNames,
								    (U16*)Cat23DemoIcons,phoneNumbers,
								    0, chis_p->currRecvdCallIndex, guiBuffer );
#else
		ShowCategory53Screen( STR_SCR8054_CAPTION, IMG_SCR8054_CAPTION,
		                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
		                      IMG_GLOBAL_BACK, chis_p->nRecvdCalls - nSuited, phoneNames,
		                      (U16*)Cat23DemoIcons,phoneNumbers,
		                      0, chis_p->currRecvdCallIndex, guiBuffer );
#endif


#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif		
		#ifndef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		#else
		SetKeyHandler( CHISViewCtrlByLeftArrowKey,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		#endif
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		SetKeyHandler( EntryCHISTRecvdListOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetLeftSoftkeyFunction( EntryCHISTRecvdListOptions, KEY_EVENT_UP );
#else
		#ifndef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		SetKeyHandler( EntryCHISTViewRecvdCall,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		#else
		SetKeyHandler( CHISViewCtrlByRgihtArrowKey,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		#endif
		SetLeftSoftkeyFunction( EntryCHISTViewRecvdCall, KEY_EVENT_UP );
#endif
#ifdef __MMI_USE_LSK_AS_SEND2__
    #ifdef __MMI_WGUI_CSK_ENABLE__
             SetCenterSoftkeyFunction( EntryCHISTViewRecvdCall, KEY_EVENT_UP );
    #endif
              SetKeyHandler(EntryCHISTViewRecvdCall,KEY_ENTER,KEY_EVENT_UP);
#endif
		SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#ifdef __MMI_MULTI_SIM__
		MTPNP_PFAL_CC_HandleSendKeys(CHISTCallRecvdNum,KEY_EVENT_DOWN);
		ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);

#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		SetKeyHandler(EntryCHISTRecvdListOptions,KEY_ENTER,KEY_EVENT_UP );
#else
		SetKeyHandler(EntryCHISTViewRecvdCall,KEY_ENTER,KEY_EVENT_UP );
#endif


#else
	    SetKeyHandler(CHISTCallRecvdNum, KEY_SEND, KEY_EVENT_DOWN);
#endif

#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	   if(call_log_on_idle)
	   {
		   set_post_tab_pane_display();
	   }
#endif
	#ifdef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
		DeleteScreenIfPresent(SCR8056_CALLS_LIST_MISSED);
		DeleteScreenIfPresent(SCR8055_CALLS_LIST_DIALED);
		DeleteScreenIfPresent(SCR8058_CALLS_LIST_REJECTED);
	#endif
	}
	else
	{
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__
	   if(call_log_on_idle)
	   {
		set_pre_tab_pane_display();
		EntryNewScreen(SCR8054_CALLS_LIST_RECEIVED, NULL, ReqReadRecvdNum, NULL);
		guiBuffer = GetCurrGuiBuffer( SCR8054_CALLS_LIST_RECEIVED );
		//phoneNames[0]=(PU8)GetString(STR_EMPTY_LIST_NOTIFICATION);
		//Cat23DemoIcons[0]=0;
		ShowCategory53Screen( STR_SCR8054_CAPTION, IMG_SCR8054_CAPTION,
								    0, 0, STR_GLOBAL_BACK,IMG_GLOBAL_BACK, 
								    chis_p->nRecvdCalls - nSuited, NULL,NULL,NULL,0,0, guiBuffer );
		/*ShowCategory56Screen( STR_SCR8054_CAPTION, IMG_SCR8054_CAPTION,
		               0,0, STR_GLOBAL_BACK,
		               IMG_GLOBAL_BACK, 1, phoneNames,
		               NULL,
		               0, guiBuffer );*/
		SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
		set_post_tab_pane_display();
	   }
	   else
#endif
	   {
		DisplayPopup((PU8)GetString(STR_EMPTY_LIST_NOTIFICATION),
		             IMG_GLOBAL_EMPTY, 1,
		             CS_NOTIFYDURATION, EMPTY_LIST_TONE);
	}
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTViewRecvdCall

  	PURPOSE				: To show the screen containing details
						  of the recvd call.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTViewRecvdCall( void )
{
	U8*	guiBuffer;
	U8	   index, noItems;
#ifdef __PHB_LN_SINGLE_COUNT__
	U16	Cat23DemoIcons[5];
#else
	U16	Cat23DemoIcons[6];
#endif

	EntryNewScreen(SCR8006_CALL_RECEIVED, ExitCHISTViewRecvdCall,
	               NULL, NULL);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	addToHistory = 1;
#endif
	guiBuffer = GetCurrGuiBuffer( SCR8006_CALL_RECEIVED );
	index = chis_p->currRecvdCallIndex;

   date_string(&chis_p->recvdCalls[index].startTime,
	            (UI_string_type)subMenuDataPtrs[0],DT_IDLE_SCREEN);
	time_string(&chis_p->recvdCalls[index].startTime,
	            (UI_string_type)subMenuDataPtrs[1],DT_IDLE_SCREEN);

	if( chis_p->recvdCalls[index].pbName[0] != 0 )
	{
		pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[2],
		                  (S8 *)chis_p->recvdCalls[index].pbName);

		if( chis_p->recvdCalls[index].number[0] != 0 )
		   pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[3],
		                     (S8 *)chis_p->recvdCalls[index].number );
		else
			pfnUnicodeStrncpy( (S8 *)subMenuDataPtrs[3],
			                   (S8 *)GetString(STR_NO_NUMBER),
			                   MAX_GET_STRING_LEN );

#ifdef __PHB_LN_SINGLE_COUNT__
		noItems = 4;
#else
		sprintf( (S8 *)subMenuDataPtrs[5], "%d ",
		         chis_p->recvdCalls[index].no_of_times );

		AnsiiToUnicodeString( (PS8)subMenuDataPtrs[4],
		                      (PS8)subMenuDataPtrs[5] );
		pfnUnicodeStrncat( (S8 *)subMenuDataPtrs[4],
		                   (S8 *)GetString( STR_TIMES ),
		                   MAX_GET_STRING_LEN );
		noItems = 5;
		Cat23DemoIcons[4] = IMG_COUNT_ENTRY;
#endif
		Cat23DemoIcons[0] = IMG_DATE_ENTRY;
		Cat23DemoIcons[1] = IMG_TIME_ENTRY;
		Cat23DemoIcons[2] = IMG_NAME_ENTRY;
		Cat23DemoIcons[3] = IMG_NUMBER_ENTRY;
	}
	else
	{
		if( chis_p->recvdCalls[index].number[0] != 0)
			pfnUnicodeStrcpy( (S8 *)subMenuDataPtrs[2],
			                  (S8 *)chis_p->recvdCalls[index].number );
		else
			pfnUnicodeStrncpy( (S8 *)subMenuDataPtrs[2],
			                   (S8 *)GetString(STR_NO_NUMBER),
			                   MAX_GET_STRING_LEN );
#ifdef __PHB_LN_SINGLE_COUNT__
		noItems = 3;
#else
		sprintf( (S8 *)subMenuDataPtrs[4], "%d ",
		         chis_p->recvdCalls[index].no_of_times );
		AnsiiToUnicodeString( (PS8)subMenuDataPtrs[3],
		                      (PS8)subMenuDataPtrs[4]);
		pfnUnicodeStrncat( (S8 *)subMenuDataPtrs[3],
		                   (S8 *)GetString( STR_TIMES ),
		                   MAX_GET_STRING_LEN);
		noItems = 4;
		Cat23DemoIcons[3] = IMG_COUNT_ENTRY;
#endif

		Cat23DemoIcons[0] = IMG_DATE_ENTRY;
		Cat23DemoIcons[1] = IMG_TIME_ENTRY;
		Cat23DemoIcons[2] = IMG_NUMBER_ENTRY;
	}

    if (mmi_dt_mytime_2_utc_sec(&chis_p->recvdCalls[index].duration) != 0)
    {
        U8 value[20];
		U8 nDuration = noItems++;
        sprintf(
            (S8*) value,
            "%02d:%02d:%02d",
            chis_p->recvdCalls[index].duration.nHour,
            chis_p->recvdCalls[index].duration.nMin,
            chis_p->recvdCalls[index].duration.nSec);
        AnsiiToUnicodeString((S8*) subMenuDataPtrs[nDuration], (S8*) value);
        Cat23DemoIcons[nDuration] = IMG_ID_CHIST_DURATION;
    }
	 
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	ShowCategory84Screen( STR_SCR8069_CAPTION1, IMG_SCR8069_CAPTION,
							    0, 0,
						       STR_GLOBAL_BACK, IMG_GLOBAL_BACK, noItems,
						       subMenuDataPtrs, Cat23DemoIcons,LIST_MENU,
						       0, guiBuffer );
#else
	ShowCategory84Screen( STR_SCR8069_CAPTION1, IMG_SCR8069_CAPTION,
		                   STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						      STR_GLOBAL_BACK, IMG_GLOBAL_BACK, noItems,
						      subMenuDataPtrs, Cat23DemoIcons,LIST_MENU,
						      0, guiBuffer );
#endif


#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif		
	
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	SetKeyHandler( EntryCHISTRecvdListOptions,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction( EntryCHISTRecvdListOptions, KEY_EVENT_UP );
#endif
#ifdef __MMI_USE_LSK_AS_SEND2__
    #ifdef __MMI_WGUI_CSK_ENABLE__
        SetCenterSoftkeyFunction( EntryCHISTRecvdListOptions, KEY_EVENT_UP );
    #endif
        SetKeyHandler(EntryCHISTRecvdListOptions,KEY_ENTER,KEY_EVENT_UP);
#endif
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#ifdef __MMI_MULTI_SIM__
	    MTPNP_PFAL_CC_HandleSendKeys(CHISTCallRecvdNum,KEY_EVENT_DOWN);
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
	SetKeyHandler(EntryCHISTRecvdListOptions,KEY_ENTER,KEY_EVENT_UP );
#endif

#else
	    SetKeyHandler(CHISTCallRecvdNum, KEY_SEND, KEY_EVENT_DOWN);
#endif

	return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTViewRecvdCall

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTViewRecvdCall( void )
{
	history_t	currHistory;
	S16		nHistory	= 0;

	currHistory.scrnID = SCR8006_CALL_RECEIVED;
	currHistory.entryFuncPtr = EntryCHISTViewRecvdCall;
	pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
	GetCategoryHistory( currHistory.guiBuffer );
	if ( addToHistory )
	   AddHistory( currHistory );
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTRecvdListOptions

  	PURPOSE				: To show a screen displaying options
						  available on Recvd call list.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTRecvdListOptions( void )
{
	U16	nStrItemList[MAX_SUB_MENUS];
	U16	nNumofItem;
	U8*	guiBuffer;

	EntryNewScreen(SCR8007_RECEIVED_CALL_OPTIONS, ExitCHISTRecvdListOptions,
	               NULL, NULL);
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	addToHistory = 1;
#endif
	guiBuffer = GetCurrGuiBuffer( SCR8007_RECEIVED_CALL_OPTIONS );

#ifdef __IP_NUMBER__
 	SetIPMenuOption(RECEIVED_CALL_MENU_IP_DIAL);
#endif
	nNumofItem = GetNumOfChild_Ext( RECEIVED_CALL_LIST_OPTIONS );
	GetSequenceStringIds_Ext( RECEIVED_CALL_LIST_OPTIONS, nStrItemList );
	SetParentHandler( RECEIVED_CALL_LIST_OPTIONS );
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif

	if (chis_p->recvdCalls[chis_p->currRecvdCallIndex].number[0] == 0)
		nNumofItem -= 3;

	ShowCategory15Screen(STR_SCR8069_CAPTION, IMG_SCR8069_CAPTION,
	                     			STR_GLOBAL_OK, IMG_GLOBAL_OK,
	                     STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							   nNumofItem, nStrItemList,
						      (U16 *)gIndexIconsImageList, 1, 0, guiBuffer );

	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTRecvdListOptions

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTRecvdListOptions( void )
{
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__
   if (IsScreenPresent(SCR8006_CALL_RECEIVED))
   {
#endif
	   history_t	currHistory;
	   S16		nHistory	= 0;

   	currHistory.scrnID = SCR8007_RECEIVED_CALL_OPTIONS;
	   currHistory.entryFuncPtr = EntryCHISTRecvdListOptions;
   	pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
	   GetCategoryHistory( currHistory.guiBuffer );
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	if (addToHistory)
#endif
   	AddHistory( currHistory );
#ifndef __MMI_CH_OPTIONS_INCLUDE_INFO__   	
   }
#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTDeleteRecvdCallConfirm

  	PURPOSE				: To delete the current active dialed call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTDeleteRecvdCallConfirm( void )
{

	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO,
		get_string(STR_ENTRY_DELETED_CONFIRM), IMG_GLOBAL_QUESTION, WARNING_TONE);


	SetLeftSoftkeyFunction(CHISTDeleteRecvdNum,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}

void EntryCHISTDeleteRejectedCallConfirm( void )
{

	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO,
		get_string(STR_ENTRY_DELETED_CONFIRM), IMG_GLOBAL_QUESTION, WARNING_TONE);


	SetLeftSoftkeyFunction(CHISTDeleteRejectedNum,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTAnimationTimeOutHdlr

  	PURPOSE				: Time out handler for progressing screen before 
  							  call log is ready right after power on

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTAnimationTimeOutHdlr(void)
{
   StopTimer(PHB_NOTIFYDURATION_TIMER);
   GoBackHistory();
   return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTProcessingAnimation

  	PURPOSE				: Exit handler of call log progressing screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTProcessingAnimation(void)
{
	StopTimer(PHB_NOTIFYDURATION_TIMER);
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqReadDialedNumByKey

  	PURPOSE				: Check if call log is ready before read dialed calls

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqReadDialedNumByKey(void)
{
	mmi_trace(1,"ReqReadDialedNumByKey");

	if (chis_p->isReqSent == TRUE)
		return;

	if (chis_p->isCallLogReady == TRUE && chis_p->isFirstTime == FALSE)
	{
		/* ready and auto read call log is done */
		ReqReadDialedNum();
		chis_p->isReqSent = TRUE;
	}
	else
	{
		EntryNewScreen(SCR_CALL_LOG_NOT_READY, ExitCHISTProcessingAnimation, NULL, NULL);

		StartTimer (PHB_NOTIFYDURATION_TIMER, PHB_NOTIFY_TIMEOUT, CHISTAnimationTimeOutHdlr);

		ShowCategory66Screen(STR_SCR8056_CAPTION, IMG_SCR8056_CAPTION,
				                0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				                (PU8)GetString(STR_CALLS_IN_PROGRESS_TEXT),
				                IMG_PROCESSING_PHONEBOOK, NULL);

		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqReadDialedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqReadDialedNum(void)
{

	MYQUEUE Message;
	GET_LAST_NUMBER_REQ   *dataPtr;

#ifdef __MMI_MULTI_SIM__
	mmi_trace(TRUE," Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif

#ifndef __MMI_CH_MIXED_CALL_LOG__
	chis_p->dialedCalls=(LOG_CALL *)chis_p->callsLog;
#endif

	dataPtr = (GET_LAST_NUMBER_REQ *) OslConstructDataPtr(sizeof(GET_LAST_NUMBER_REQ));

	dataPtr->type = PHB_LND;
	if (chis_p->sequenceId == 0)
	{
		chis_p->sequenceId = 1;
	}
	dataPtr->seq_id = chis_p->sequenceId;

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_GET_LAST_NUMBER_REQ;
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

	if( chis_p->isFirstTime == FALSE )
	{
		AlmDisableExpiryHandler();
		ClearAllKeyHandler();
	}

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspReadDialedNum, PRT_MMI_PHB_GET_LAST_NUMBER_RSP );
#else 
	SetProtocolEventHandler( RspReadDialedNum, PRT_MMI_PHB_GET_LAST_NUMBER_RSP );
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: RspReadDialedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspReadDialedNum(void *info)
{

	GET_LAST_NUMBER_RSP *rsp;
	U8 index;
	U8 nSimID;
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	#ifdef __MMI_MULTI_SIM__
	SetCHISContext(nSimID);
	#endif
	
	rsp = (GET_LAST_NUMBER_RSP *)info;
#ifdef __MMI_MULTI_SIM__
	mmi_trace(TRUE," Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif
	if( rsp->result.flag == CALL_LOG_RESULT_OK )
	{
		if (rsp->type != PHB_LND)
			return;

		if (chis_p->sequenceId == 1)
		{
			chis_p->nDialedCalls = 0;
			memset ((PS8)chis_p->dialedCalls, 0, sizeof (LOG_CALL)*TOT_SIZE_OF_DIALED_LIST);
		}
		for( index = 0; index < rsp->no_list; index ++ )
		{
			CHISTExtractPSCallLog(&chis_p->dialedCalls[chis_p->nDialedCalls], &rsp->list[index]);
			chis_p->nDialedCalls++;
		}
#if (MAX_PHB_LN_ENTRY > 20)
		if (rsp->more_data)
		{
			chis_p->sequenceId++;
			ASSERT(chis_p->sequenceId<4);
			ReqReadDialedNum();
			return;
		}
#endif
	}
	chis_p->sequenceId = 0;

#ifndef __MMI_CH_MIXED_CALL_LOG__
	if( chis_p->isFirstTime == TRUE )
#else
	if ( chis_p->isFirstTime == TRUE || chis_p->isMixedStyle == TRUE )
#endif
	{
		ReqReadMissedNum();
		return;
	}

	chis_p->isReqSent = FALSE;
	EntryCHISTViewDialedCallList();
	AlmEnableExpiryHandler();
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqWriteCallLog

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqWriteCallLog(CALL_INFO *dailCall, U8 type)
{
	MYQUEUE Message;
	SET_LAST_NUMBER_REQ   *dataPtr;
	U8 *number;

   switch (type)
   {
      case PHB_LND:
#ifdef __MMI_MULTI_SIM__
	      MTPNP_CHISSetProtocolEventHandler( RspWriteDialedNum,  PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#else
	      SetProtocolEventHandler( RspWriteDialedNum, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#endif
         number = (U8 *)dailCall->num;
	   chis_p->process = CALL_LOG_PROC_WRITE;
         break;
      case PHB_LNM:
#ifdef __MMI_MULTI_SIM__
         MTPNP_CHISSetProtocolEventHandler( RspWriteMissedNum, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#else
         SetProtocolEventHandler( RspWriteMissedNum, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#endif
         number = (U8 *)dailCall->number;
#ifdef __MMI_CH_MIXED_CALL_LOG__
	   chis_p->process = CALL_LOG_PROC_WRITE;
#endif
         break;
 case PHB_LNRJ:
#ifdef __MMI_MULTI_SIM__
         MTPNP_CHISSetProtocolEventHandler( RspWriteRejectedNum, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#else
         SetProtocolEventHandler( RspWriteRejectedNum, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#endif
         number = (U8 *)dailCall->number;
#ifdef __MMI_CH_MIXED_CALL_LOG__
	   chis_p->process = CALL_LOG_PROC_WRITE;
#endif
         break;
      case PHB_LNR:
#ifdef __MMI_MULTI_SIM__
         MTPNP_CHISSetProtocolEventHandler( RspWriteRecvdNum, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#else
         SetProtocolEventHandler( RspWriteRecvdNum, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#endif
         number = (U8 *)dailCall->number;
#ifdef __MMI_CH_MIXED_CALL_LOG__
	   chis_p->process = CALL_LOG_PROC_WRITE;
#endif
         break;
      default:
         return;
   }

   if (chis_p->isCallLogReady == FALSE)
	   return;

   dataPtr = (SET_LAST_NUMBER_REQ *)
	               OslConstructDataPtr(sizeof(SET_LAST_NUMBER_REQ));
	dataPtr->type = type;
	dataPtr->no_data = 0;
	dataPtr->index = 0xff;

	memset ((U8 *)&dataPtr->entry.tel, 0, sizeof(CALL_LOG_NUMBER_STRUCT));
	memset ((U8 *)&dataPtr->entry.alpha_id, 0, sizeof(CALL_LOG_NAME_STRUCT));

	if (number[0]!=0)
	{
	   if ( number[0] == '+' )
		{
			dataPtr->entry.tel.type = 145;
			UnicodeToAnsii((PS8)dataPtr->entry.tel.number, (PS8)(number+2));
	   }
		else
		{
			dataPtr->entry.tel.type = 129;
			UnicodeToAnsii((PS8)dataPtr->entry.tel.number,(PS8)(number));
	   }
		dataPtr->entry.tel.length = strlen((PS8)dataPtr->entry.tel.number);
	}

	if(pfnUnicodeStrlen((PS8)dailCall->pBname) > 0)
	{
	   dataPtr->entry.alpha_id.name_dcs = dailCall->name_dcs;
	   if (dailCall->name_dcs == MMI_PHB_ASCII)
	   {
	      UnicodeToAnsii((PS8)dataPtr->entry.alpha_id.name,
	                     (PS8)(dailCall->pBname));
	      dataPtr->entry.alpha_id.name_length =
	         strlen((PS8)dataPtr->entry.alpha_id.name);
	   }
	   else
	   {
	      U8 i=0, len;

	      len = pfnUnicodeStrlen((PS8)dailCall->pBname)*ENCODING_LENGTH;
	      if (len > MAX_PB_NAME_LENGTH)
	      {
	         len = MAX_PB_NAME_LENGTH;
	      }
	      while( i != len )
         {
            /* big endian <-> little endian */
            dataPtr->entry.alpha_id.name[i] = dailCall->pBname[i+1];
            dataPtr->entry.alpha_id.name[i+1] = dailCall->pBname[i];
            i+=2;
         }
         dataPtr->entry.alpha_id.name[i] = '\0';
         dataPtr->entry.alpha_id.name[i+1] = '\0';
	      dataPtr->entry.alpha_id.name_length =
	         pfnUnicodeStrlen((PS8)dataPtr->entry.alpha_id.name) *
	            ENCODING_LENGTH;
	   }
	}

   ConvertMYTIME2RTC(&dataPtr->entry.time, &dailCall->start_time);
	dataPtr->entry.call_duration = 0;
	dataPtr->entry.count = 1;

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_SET_LAST_NUMBER_REQ;
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

   OslMsgSendExtQueue(&Message);
   return;
}

/**************************************************************

	FUNCTION NAME		: RspWriteDialedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspWriteDialedNum(void *info)
{
	SET_LAST_NUMBER_RSP *rsp;

	rsp = (SET_LAST_NUMBER_RSP *)info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK && chis_p->nDialedCalls < TOT_SIZE_OF_DIALED_LIST)
	{
#if (TOT_SIZE_OF_DIALED_LIST <= 20)
		chis_p->nDialedCalls = (U8)rsp->no_list;
#else
		chis_p->nDialedCalls++;
#endif
	}
	
	if (chis_p->process == CALL_LOG_PROC_SYNC)
	{
		PRINT_INFORMATION(("set dialed rsp comes before read rsp!!!! re-sync."));
		chis_p->process = CALL_LOG_PROC_NONE;
		CHISTGetCallLogBySENDKey();
	}
	chis_p->process = CALL_LOG_PROC_NONE;
		
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTSyncCallLogNeeded

 
  							  user saves call log number to phonebook with name

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL CHISTSyncCallLogNeeded (U8 length, U8 type, PU8 number)
{
   LOG_CALL *record;

	switch (chis_p->curCallLogType)
	{
	   case PHB_LNM:
      	record = &chis_p->missedCalls[chis_p->currMissedCallIndex];
      	break;
      case PHB_LNR:
      	record = &chis_p->recvdCalls[chis_p->currRecvdCallIndex];
      	break;      
      case PHB_LND:    
      default:
      	record = &chis_p->dialedCalls[chis_p->currDialedCallIndex];
      	break;
	}
   
   if (pfnUnicodeStrcmp((PS8)number, (PS8)record->number) == 0)
      return TRUE;
      return FALSE;
}

/**************************************************************

	FUNCTION NAME		: ReqUpdateCallLogName

 
  	
	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqUpdateCallLogName(U8 length, U8 dcs, PU8 name)
{
	MYQUEUE Message;
	SET_LAST_NUMBER_REQ   *dataPtr;
	LOG_CALL *record;

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_SET_LAST_NUMBER_REQ;
	dataPtr =  (SET_LAST_NUMBER_REQ *) OslConstructDataPtr(sizeof(SET_LAST_NUMBER_REQ));
	dataPtr->type = chis_p->curCallLogType;
	dataPtr->no_data = 0;
	switch (dataPtr->type)
	{
		case PHB_LNM:
			record = &chis_p->missedCalls[chis_p->currMissedCallIndex];
			dataPtr->index = chis_p->currMissedCallIndex;
		break;
		case PHB_LND:
			record = &chis_p->dialedCalls[chis_p->currDialedCallIndex];
			dataPtr->index = chis_p->currDialedCallIndex;
		break;
		case PHB_LNR:
			record = &chis_p->recvdCalls[chis_p->currRecvdCallIndex];
			dataPtr->index = chis_p->currRecvdCallIndex;
		break;      
		default:
			record = &chis_p->dialedCalls[chis_p->currDialedCallIndex];
			dataPtr->index = chis_p->currDialedCallIndex;
		break;
	}
	record->nameDCS = dcs;
	record->nameLength = length;
	pfnUnicodeStrcpy ((PS8)record->pbName, (PS8)name);

	memset ((U8 *)&dataPtr->entry.tel, 0, sizeof(CALL_LOG_NUMBER_STRUCT));
	memset ((U8 *)&dataPtr->entry.alpha_id, 0, sizeof(CALL_LOG_NAME_STRUCT));

	if (record->number[0]!=0)
	{
		if ( record->number[0] == '+' )
		{
			dataPtr->entry.tel.type = 145;
			UnicodeToAnsii((PS8)dataPtr->entry.tel.number, (PS8)(record->number+2));
		}
		else
		{
			dataPtr->entry.tel.type = 129;
			UnicodeToAnsii((PS8)dataPtr->entry.tel.number,(PS8)(record->number));
		}
		dataPtr->entry.tel.length = strlen((PS8)dataPtr->entry.tel.number);
	}

	if (record->pbName[0] != 0)
	{
		dataPtr->entry.alpha_id.name_dcs = record->nameDCS;
		if (record->nameDCS == MMI_PHB_ASCII)
		{
			UnicodeToAnsii((PS8)dataPtr->entry.alpha_id.name,
					(PS8)(record->pbName));
			dataPtr->entry.alpha_id.name_length =
					strlen((PS8)dataPtr->entry.alpha_id.name);
		}
		else
		{
			U8 i=0, len;

			len = (U8)pfnUnicodeStrlen((PS8)record->pbName)*ENCODING_LENGTH;
			while( i != len )
		{
			/* big endian <-> little endian */
			dataPtr->entry.alpha_id.name[i] = record->pbName[i+1];
			dataPtr->entry.alpha_id.name[i+1] = record->pbName[i];
			i+=2;
		}
			dataPtr->entry.alpha_id.name[i] = '\0';
			dataPtr->entry.alpha_id.name[i+1] = '\0';
			dataPtr->entry.alpha_id.name_length = (U8)
				(pfnUnicodeStrlen((PS8)dataPtr->entry.alpha_id.name) *
					ENCODING_LENGTH);
	   }
	}

   ConvertMYTIME2RTC(&dataPtr->entry.time, &record->startTime);
	dataPtr->entry.count = (U8)record->no_of_times;
	dataPtr->entry.call_duration = (kal_int32)mmi_dt_mytime_2_utc_sec(&record->duration);
	
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspUpdateCallLogName, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#else
	SetProtocolEventHandler( RspUpdateCallLogName, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#endif
	OslMsgSendExtQueue(&Message);

}

void ReqUpdateCallLogDuration(CALL_INFO *dailCall, U8 type)
{
	MYQUEUE Message;
	SET_LAST_NUMBER_REQ   *dataPtr;
	U8 *number;

	mmi_trace(g_sw_CALLLOG,"ReqUpdateCallLogDuration() type=%d", type);
	
   switch (type)
   {
      case PHB_LND:
#ifdef __MMI_MULTI_SIM__
	      MTPNP_CHISSetProtocolEventHandler( NULL, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#else
	      SetProtocolEventHandler( NULL, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#endif
         number = (U8 *)dailCall->num;
         break;
		 		 
      case PHB_LNR:
#ifdef __MMI_MULTI_SIM__
         MTPNP_CHISSetProtocolEventHandler( NULL, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#else
         SetProtocolEventHandler( NULL, PRT_MMI_PHB_SET_LAST_NUMBER_RSP );
#endif
         number = (U8 *)dailCall->number;
         break;
      default:
         return;
   }

   	if (chis_p->isCallLogReady == FALSE)
	   	return;

   	dataPtr = (SET_LAST_NUMBER_REQ *)
	               	OslConstructDataPtr(sizeof(SET_LAST_NUMBER_REQ));
	dataPtr->type = type;
	dataPtr->no_data = 0;
	dataPtr->index = 0xff;

	memset ((U8 *)&dataPtr->entry.tel, 0, sizeof(CALL_LOG_NUMBER_STRUCT));
	memset ((U8 *)&dataPtr->entry.alpha_id, 0, sizeof(CALL_LOG_NAME_STRUCT));

	if (number[0]!=0)
	{
	   if ( number[0] == '+' )
		{
			dataPtr->entry.tel.type = 145;
			UnicodeToAnsii((PS8)dataPtr->entry.tel.number, (PS8)(number+2));
	   }
		else
		{
			dataPtr->entry.tel.type = 129;
			UnicodeToAnsii((PS8)dataPtr->entry.tel.number,(PS8)(number));
	   }
		dataPtr->entry.tel.length = strlen((PS8)dataPtr->entry.tel.number);
	}

	if(pfnUnicodeStrlen((PS8)dailCall->pBname) > 0)
	{
	   dataPtr->entry.alpha_id.name_dcs = dailCall->name_dcs;
	   if (dailCall->name_dcs == MMI_PHB_ASCII)
	   {
	      UnicodeToAnsii((PS8)dataPtr->entry.alpha_id.name,
	                     (PS8)(dailCall->pBname));
	      dataPtr->entry.alpha_id.name_length =
	         strlen((PS8)dataPtr->entry.alpha_id.name);
	   }
	   else
	   {
	      U8 i=0, len;

	      len = pfnUnicodeStrlen((PS8)dailCall->pBname)*ENCODING_LENGTH;
	      if (len > MAX_PB_NAME_LENGTH)
	      {
	         len = MAX_PB_NAME_LENGTH;
	      }
	      while( i != len )
         {
            /* big endian <-> little endian */
            dataPtr->entry.alpha_id.name[i] = dailCall->pBname[i+1];
            dataPtr->entry.alpha_id.name[i+1] = dailCall->pBname[i];
            i+=2;
         }
         dataPtr->entry.alpha_id.name[i] = '\0';
         dataPtr->entry.alpha_id.name[i+1] = '\0';
	      dataPtr->entry.alpha_id.name_length =
	         pfnUnicodeStrlen((PS8)dataPtr->entry.alpha_id.name) *
	            ENCODING_LENGTH;
	   }
	}

   ConvertMYTIME2RTC(&dataPtr->entry.time, &dailCall->start_time);
	
	dataPtr->entry.call_duration = 
		mmi_dt_mytime_2_utc_sec(&dailCall->end_time) -mmi_dt_mytime_2_utc_sec(&dailCall->start_time);
   
	dataPtr->entry.count = 0;  //no used
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_SET_LAST_NUMBER_REQ;
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

   OslMsgSendExtQueue(&Message);
   return;
}

/**************************************************************

	FUNCTION NAME		: RspUpdateCallLogName

  	PURPOSE				: Response handler for update call log data
  	
	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspUpdateCallLogName(void *info)
{
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 
	               1,CS_NOTIFYDURATION,SUCCESS_TONE);
}

/**************************************************************

	FUNCTION NAME		: ReqDeleteDialedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqDeleteCallLog(U8 type, U8 index, U8 no_data)
{
	MYQUEUE Message;
	DEL_LAST_NUMBER_REQ   *dataPtr;

   switch (type)
   {
      case PHB_LND:
#ifdef __MMI_MULTI_SIM__
	      MTPNP_CHISSetProtocolEventHandler( RspDeleteDialedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#else
	      SetProtocolEventHandler( RspDeleteDialedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#endif
	      break;
	   case PHB_LNM:
#ifdef __MMI_MULTI_SIM__
	      MTPNP_CHISSetProtocolEventHandler( RspDeleteMissedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#else
	      SetProtocolEventHandler( RspDeleteMissedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#endif
	      break;
		case PHB_LNR:
#ifdef __MMI_MULTI_SIM__
	      MTPNP_CHISSetProtocolEventHandler( RspDeleteRecvdNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#else
	      SetProtocolEventHandler( RspDeleteRecvdNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#endif
	      break;
    case PHB_LNRJ:
#ifdef __MMI_MULTI_SIM__
        MTPNP_CHISSetProtocolEventHandler( RspDeleteRejectedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#else
        SetProtocolEventHandler( RspDeleteRejectedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#endif
        break;

	   default:
	      return;
   }

	dataPtr = (DEL_LAST_NUMBER_REQ *)
	               OslConstructDataPtr(sizeof(DEL_LAST_NUMBER_REQ));
	dataPtr->type = type;
	dataPtr->index = index;
	dataPtr->no_data = no_data;

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_DEL_LAST_NUMBER_REQ;
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

	ClearAllKeyHandler();
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: RspDeleteDialedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspDeleteDialedNum(void *info)
{
	S32 i;
	DEL_LAST_NUMBER_RSP *rsp;

	rsp = (DEL_LAST_NUMBER_RSP *)info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK )
	{
		for ( i = chis_p->currDialedCallIndex;
		         i < ( chis_p->nDialedCalls - 1 ) ; i++ )
			memcpy( &chis_p->dialedCalls[i], &chis_p->dialedCalls[i+1],
			        sizeof( DIALED_CALL ) );
		chis_p->nDialedCalls--;
		if ((chis_p->currDialedCallIndex >= chis_p->nDialedCalls) &&
		    (chis_p->nDialedCalls != 0))
			chis_p->currDialedCallIndex = chis_p->nDialedCalls - 1;

		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}

	if( chis_p->nDialedCalls == 0)
	{
#ifdef __MMI_CH_MIXED_CALL_LOG__
      if (!IsScreenPresent(SCR8055_CALLS_LIST_DIALED))
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		DeleteScreenIfPresent( SCR8009_DIALED_CALL_OPTIONS );
#else
      DeleteBetweenScreen( SCR8008_CALL_DIALED, SCR8009_DIALED_CALL_OPTIONS );
#endif
      else
#endif
		DeleteBetweenScreen( SCR8055_CALLS_LIST_DIALED, SCR8009_DIALED_CALL_OPTIONS );
   }
	else
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		DeleteScreenIfPresent( SCR8009_DIALED_CALL_OPTIONS );
#else
		DeleteBetweenScreen( SCR8008_CALL_DIALED, SCR8009_DIALED_CALL_OPTIONS);
#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqReadMissedNumByKey

  	PURPOSE				: Read missed call data when enter menu

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqReadMissedNumByKey(void)
{
	if (chis_p->isReqSent == TRUE)
		return;
	if (chis_p->isCallLogReady == TRUE && chis_p->isFirstTime == FALSE)
	{
		/* ready and auto read call log is done */
		ReqReadMissedNum();
		chis_p->isReqSent = TRUE;
	}
	else
	{
		EntryNewScreen(SCR_CALL_LOG_NOT_READY, ExitCHISTProcessingAnimation, NULL, NULL);

		StartTimer (PHB_NOTIFYDURATION_TIMER, PHB_NOTIFY_TIMEOUT, CHISTAnimationTimeOutHdlr);

		ShowCategory66Screen(STR_SCR8055_CAPTION, IMG_SCR8055_CAPTION,
							0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							(PU8)GetString(STR_CALLS_IN_PROGRESS_TEXT),
							IMG_PROCESSING_PHONEBOOK, NULL);

		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqReadRejectedNumByKey

  	PURPOSE				: Read Rejected call data when enter menu

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqReadRejectedNumByKey(void)
{
	if (chis_p->isReqSent == TRUE)
		return;
	if (chis_p->isCallLogReady == TRUE && chis_p->isFirstTime == FALSE)
	{
		/* ready and auto read call log is done */
		ReqReadRejectedNum();
		chis_p->isReqSent = TRUE;
	}
	else
	{
		EntryNewScreen(SCR_CALL_LOG_NOT_READY, ExitCHISTProcessingAnimation, NULL, NULL);

		StartTimer (PHB_NOTIFYDURATION_TIMER, PHB_NOTIFY_TIMEOUT, CHISTAnimationTimeOutHdlr);

		ShowCategory66Screen(STR_MENU_REJECT_CALL, IMG_SCR8055_CAPTION,
							0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							(PU8)GetString(STR_CALLS_IN_PROGRESS_TEXT),
							IMG_PROCESSING_PHONEBOOK, NULL);

		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqReadRejectedNum

  	PURPOSE				: Read Rejected call log from protocol

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqReadRejectedNum(void)
{
	MYQUEUE Message;
	GET_LAST_NUMBER_REQ   *dataPtr;
	
#ifdef __MMI_MULTI_SIM__
	mmi_trace(TRUE," Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif

#ifndef __MMI_CH_MIXED_CALL_LOG__
	chis_p->rejectedCalls = (LOG_CALL *)chis_p->callsLog;
#endif
//   chis_p->saveMissedScrn = FALSE;

	dataPtr = (GET_LAST_NUMBER_REQ *)OslConstructDataPtr(sizeof(GET_LAST_NUMBER_REQ));
	dataPtr->type = PHB_LNRJ;
	if (chis_p->sequenceId == 0)
	{
		chis_p->sequenceId = 1;
	}
	dataPtr->seq_id = chis_p->sequenceId;

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_GET_LAST_NUMBER_REQ;
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

	if( chis_p->isFirstTime == FALSE )
	{
		AlmDisableExpiryHandler();
		ClearAllKeyHandler();
	}

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspReadRejectedNum, PRT_MMI_PHB_GET_LAST_NUMBER_RSP );
#else 
	SetProtocolEventHandler( RspReadRejectedNum, PRT_MMI_PHB_GET_LAST_NUMBER_RSP );
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqReadMissedNum

  	PURPOSE				: Read missed call log from protocol

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqReadMissedNum(void)
{
	MYQUEUE Message;
	GET_LAST_NUMBER_REQ   *dataPtr;

#ifdef __MMI_MULTI_SIM__
	mmi_trace(TRUE," Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif

#ifndef __MMI_CH_MIXED_CALL_LOG__
	chis_p->missedCalls = (LOG_CALL *)chis_p->callsLog;
#endif
	chis_p->saveMissedScrn = FALSE;

	dataPtr = (GET_LAST_NUMBER_REQ *)
	OslConstructDataPtr(sizeof(GET_LAST_NUMBER_REQ));
	dataPtr->type = PHB_LNM;
	if (chis_p->sequenceId == 0)
	{
		chis_p->sequenceId = 1;
	}
	dataPtr->seq_id = chis_p->sequenceId;

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_GET_LAST_NUMBER_REQ;
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

	if( chis_p->isFirstTime == FALSE )
	{
		AlmDisableExpiryHandler();
		ClearAllKeyHandler();
	}

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler(RspReadMissedNum, PRT_MMI_PHB_GET_LAST_NUMBER_RSP );
#else
	SetProtocolEventHandler( RspReadMissedNum, PRT_MMI_PHB_GET_LAST_NUMBER_RSP );
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

void RspReadRejectedNum(void *info)
{
	GET_LAST_NUMBER_RSP *rsp;
	U8 index;
	
#ifdef __MMI_MULTI_SIM__
	mmi_trace(TRUE," Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif

	rsp = (GET_LAST_NUMBER_RSP *)info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK )
	{
	   if (rsp->type != PHB_LNRJ)
	      return;
	   
		if (chis_p->sequenceId == 1)
		{
			chis_p->nRejectedCalls = 0;
			memset ((PS8)chis_p->rejectedCalls, 0,
		        sizeof (LOG_CALL)*TOT_SIZE_OF_REJECTED_LIST);
		}
		for( index = 0; index < rsp->no_list; index ++ )
		{
			CHISTExtractPSCallLog(&chis_p->rejectedCalls[chis_p->nRejectedCalls], 
			                          &rsp->list[index]);
			chis_p->nRejectedCalls++;
		}
#if (MAX_PHB_LN_ENTRY > 20)
		if (rsp->more_data)
		{
			chis_p->sequenceId++;
			ASSERT(chis_p->sequenceId<4);
			ReqReadRejectedNum();
			return;
		}
#endif
	}
	chis_p->sequenceId = 0;

#ifndef __MMI_CH_MIXED_CALL_LOG__
	if( chis_p->isFirstTime == TRUE )
#else
   if ( chis_p->isFirstTime == TRUE || chis_p->isMixedStyle == TRUE )
#endif
	{
		chis_p->isFirstTime = FALSE;
    return;
	}

	chis_p->isReqSent = FALSE;
	EntryCHISTViewRejectedCallList();
	AlmEnableExpiryHandler();
	return;
}

extern U8 hilight_id;
extern U16 GetCurrScreenID(void);
void RspWriteRejectedNum(void *info)
{
	SET_LAST_NUMBER_RSP *rsp;

	rsp = (SET_LAST_NUMBER_RSP *) info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK && chis_p->nRejectedCalls < TOT_SIZE_OF_MISSED_LIST)
	{
#if (TOT_SIZE_OF_MISSED_LIST <= 20)
		chis_p->nRejectedCalls = (U8)rsp->no_list;
#else
		chis_p->nRejectedCalls++;
#endif
	}

#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (chis_p->process == CALL_LOG_PROC_SYNC && chis_p->isMixedStyle == TRUE)
	{
		PRINT_INFORMATION(("set missed rsp comes before read rsp!!!! re-sync."));
		chis_p->process = CALL_LOG_PROC_NONE;
		CHISTGetCallLogBySENDKey();
	}
	chis_p->process = CALL_LOG_PROC_NONE;
#endif

	if (GetActiveScreenId() == SCR_CALL_HISTORY_MAIN_MENU)
	{
		U8 curr_id = hilight_id;
		hilight_id = curr_id;
		EntryNewScreen(SCR_CHIS_DUMMY, NULL, NULL, NULL);
		EntryCHISTMainMenu();
		DeleteScreenIfPresent(SCR_CALL_HISTORY_MAIN_MENU);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: RspReadMissedNum

  	PURPOSE				: Receives missed call log from protocol

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspReadMissedNum(void *info)
{
	GET_LAST_NUMBER_RSP *rsp;
	U8 index;

#ifdef __MMI_MULTI_SIM__
	mmi_trace(TRUE," Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif

	rsp = (GET_LAST_NUMBER_RSP *)info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK )
	{
		if (rsp->type != PHB_LNM)
			return;

		if (chis_p->sequenceId == 1)
		{
			chis_p->nMissedCalls = 0;
			memset ((PS8)chis_p->missedCalls, 0, sizeof (LOG_CALL)*TOT_SIZE_OF_MISSED_LIST);
		}
		for( index = 0; index < rsp->no_list; index ++ )
		{
			CHISTExtractPSCallLog(&chis_p->missedCalls[chis_p->nMissedCalls], &rsp->list[index]);
			chis_p->nMissedCalls++;
		}
	#if (MAX_PHB_LN_ENTRY > 20)
		if (rsp->more_data)
		{
			chis_p->sequenceId++;
			ASSERT(chis_p->sequenceId<4);
			ReqReadMissedNum();
			return;
		}
	#endif
	}
	chis_p->sequenceId = 0;

#ifndef __MMI_CH_MIXED_CALL_LOG__
	if( chis_p->isFirstTime == TRUE )
#else
	if ( chis_p->isFirstTime == TRUE || chis_p->isMixedStyle == TRUE )
#endif
	{
		ReqReadRecvdNum();
		return;
	}

	chis_p->isReqSent = FALSE;
	EntryCHISTViewMissedCallList();
	AlmEnableExpiryHandler();
	return;
}

/**************************************************************

	FUNCTION NAME		: RspWriteMissedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspWriteMissedNum(void *info)
{
	SET_LAST_NUMBER_RSP *rsp;

	rsp = (SET_LAST_NUMBER_RSP *) info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK && chis_p->nMissedCalls < TOT_SIZE_OF_MISSED_LIST)
	{
#if (TOT_SIZE_OF_MISSED_LIST <= 20)
		chis_p->nMissedCalls = (U8)rsp->no_list;
#else
		chis_p->nMissedCalls++;
#endif
	}

#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (chis_p->process == CALL_LOG_PROC_SYNC && chis_p->isMixedStyle == TRUE)
	{
		PRINT_INFORMATION(("set missed rsp comes before read rsp!!!! re-sync."));
		chis_p->process = CALL_LOG_PROC_NONE;
		CHISTGetCallLogBySENDKey();
	}
	chis_p->process = CALL_LOG_PROC_NONE;
#endif

	return;
}

/**************************************************************

	FUNCTION NAME		: RspDeleteMissedNum

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspDeleteMissedNum(void *info)
{
	S32	i;
	DEL_LAST_NUMBER_RSP *rsp;

	rsp = (DEL_LAST_NUMBER_RSP *)info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK )
	{
		for ( i = chis_p->currMissedCallIndex;
		         i < ( chis_p->nMissedCalls - 1 ) ; i++ )
			memcpy( &chis_p->missedCalls[i], &chis_p->missedCalls[i+1],
			        sizeof( MISSED_CALL ) );
		chis_p->nMissedCalls--;
		if ((chis_p->currMissedCallIndex >= chis_p->nMissedCalls) &&
		    (chis_p->nMissedCalls != 0))
			chis_p->currMissedCallIndex = chis_p->nMissedCalls - 1;

		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}

	if( chis_p->nMissedCalls == 0 )
	{
#ifdef __MMI_CH_MIXED_CALL_LOG__
      if (!IsScreenPresent(SCR8056_CALLS_LIST_MISSED))
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		DeleteScreenIfPresent( SCR8521_MISSED_CALL_OPTIONS );
#else
      DeleteBetweenScreen( SCR8520_CALL_MISSED, SCR8521_MISSED_CALL_OPTIONS);
#endif
      else
#endif
	DeleteBetweenScreen( SCR8056_CALLS_LIST_MISSED, SCR8521_MISSED_CALL_OPTIONS );
#ifdef __MMI_MULTI_SIM__
	DeleteScreenIfPresent(SCR_MTPNP_CALL_HISTORY_NOTIFY);
#endif

		 
		chis_p->nMissedCallBeforeView = 0;

    #ifdef __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__
        mmi_chist_set_num_missed_call_before_view();
    #endif

   	mmi_chist_rearrange_missed_call_status_icon();
 
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
		IdleResetStatusIcon( STATUS_ICON_SUBLCD_MISSED_CALL );
#endif
 
   }
	else
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		DeleteScreenIfPresent( SCR8521_MISSED_CALL_OPTIONS );
#else
      DeleteBetweenScreen(SCR8520_CALL_MISSED, SCR8521_MISSED_CALL_OPTIONS);
#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqReadRecvdNumByKey

  	PURPOSE				: Read recvd call data when enter menu

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqReadRecvdNumByKey(void)
{
	if (chis_p->isReqSent == TRUE)
		return;
	if (chis_p->isCallLogReady == TRUE && chis_p->isFirstTime == FALSE)
	{
		/* ready and auto read call log is done */
		ReqReadRecvdNum();
		chis_p->isReqSent = TRUE;
	}
	else
	{
		EntryNewScreen(SCR_CALL_LOG_NOT_READY, ExitCHISTProcessingAnimation, NULL, NULL);

		StartTimer (PHB_NOTIFYDURATION_TIMER, PHB_NOTIFY_TIMEOUT, CHISTAnimationTimeOutHdlr);

		ShowCategory66Screen(STR_SCR8054_CAPTION, IMG_SCR8054_CAPTION,
							0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							(PU8)GetString(STR_CALLS_IN_PROGRESS_TEXT),
							IMG_PROCESSING_PHONEBOOK, NULL);

		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqReadRecvdNum

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqReadRecvdNum(void)
{
	MYQUEUE Message;
	GET_LAST_NUMBER_REQ   *dataPtr;
	
#ifdef __MMI_MULTI_SIM__
	mmi_trace(TRUE," Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif

#ifndef __MMI_CH_MIXED_CALL_LOG__
	chis_p->recvdCalls = (LOG_CALL *)chis_p->callsLog;
#endif

	dataPtr = (GET_LAST_NUMBER_REQ *)OslConstructDataPtr(sizeof(GET_LAST_NUMBER_REQ));
	dataPtr->type = PHB_LNR;
	if (chis_p->sequenceId == 0)
	{
		chis_p->sequenceId = 1;
	}
	dataPtr->seq_id = chis_p->sequenceId;

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_GET_LAST_NUMBER_REQ;
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

	if( chis_p->isFirstTime == FALSE )
	{
	   ClearAllKeyHandler();
	   AlmDisableExpiryHandler();
	}

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspReadRecvdNum, PRT_MMI_PHB_GET_LAST_NUMBER_RSP );
#else
	SetProtocolEventHandler( RspReadRecvdNum, PRT_MMI_PHB_GET_LAST_NUMBER_RSP );
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: RspReadRecvdNum

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspReadRecvdNum(void *info)
{
	GET_LAST_NUMBER_RSP *rsp;
	U8 index;

	rsp = (GET_LAST_NUMBER_RSP *)info;

#ifdef __MMI_MULTI_SIM__
	mmi_trace(TRUE," Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif

	if( rsp->result.flag == CALL_LOG_RESULT_OK )
	{
		if (rsp->type != PHB_LNR)
			return;
	if (chis_p->sequenceId == 1)
	{
		chis_p->nRecvdCalls=0;
		memset ((PS8)chis_p->recvdCalls, 0,sizeof (LOG_CALL)*TOT_SIZE_OF_RECVD_LIST);
	}
	for( index = 0; index < rsp->no_list; index ++ )
	{
		CHISTExtractPSCallLog(&chis_p->recvdCalls[chis_p->nRecvdCalls], &rsp->list[index]);
		chis_p->nRecvdCalls++;
	}
#if (MAX_PHB_LN_ENTRY > 20)
	if (rsp->more_data)
	{
		chis_p->sequenceId++;
		ASSERT(chis_p->sequenceId<4);
		ReqReadRecvdNum();
		return;
	}
#endif
	}

	chis_p->sequenceId = 0;
	if( chis_p->isFirstTime == TRUE )
	{
		ReqReadRejectedNum();
		return;
	}
	chis_p->isReqSent = FALSE;

#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (chis_p->isMixedStyle == TRUE)
	{
		CHISTSortCallLogsByTime();
		EntryCHISTViewMixedCallList();
		AlmEnableExpiryHandler();
		chis_p->isMixedStyle = FALSE;
		return;
	}
#endif

	EntryCHISTViewRecvdCallList();
	AlmEnableExpiryHandler();
	return;
}

/**************************************************************

	FUNCTION NAME		: RspWriteRecvdNum

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspWriteRecvdNum(void *info)
{
	SET_LAST_NUMBER_RSP *rsp;

	rsp = (SET_LAST_NUMBER_RSP *)info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK && chis_p->nRecvdCalls < TOT_SIZE_OF_RECVD_LIST)
	{
#if (TOT_SIZE_OF_RECVD_LIST <= 20)
		chis_p->nRecvdCalls = (U8)rsp->no_list;
#else
		chis_p->nRecvdCalls++;
#endif
	}

#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (chis_p->process == CALL_LOG_PROC_SYNC && chis_p->isMixedStyle == TRUE)
	{
		PRINT_INFORMATION(("set recvd rsp comes before read rsp!!!! re-sync."));
		chis_p->process = CALL_LOG_PROC_NONE;
		CHISTGetCallLogBySENDKey();
	}
	chis_p->process = CALL_LOG_PROC_NONE;	
#endif

	return;
}

/**************************************************************

	FUNCTION NAME		: RspDeleteRecvdNum

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspDeleteRecvdNum(void *info)
{
	S32 i;
	DEL_LAST_NUMBER_RSP *rsp;

	rsp = (DEL_LAST_NUMBER_RSP *)info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK )
	{
		for ( i = chis_p->currRecvdCallIndex;
		         i < ( chis_p->nRecvdCalls - 1 ) ; i++ )
			memcpy( &chis_p->recvdCalls[i], &chis_p->recvdCalls[i+1],
			          sizeof( RECEIVED_CALL ) );
		chis_p->nRecvdCalls--;
		if ((chis_p->currRecvdCallIndex >= chis_p->nRecvdCalls) &&
		    (chis_p->nRecvdCalls != 0))
			chis_p->currRecvdCallIndex = chis_p->nRecvdCalls - 1;

		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}

	if( chis_p->nRecvdCalls == 0 )
	{
#ifdef __MMI_CH_MIXED_CALL_LOG__
      if (!IsScreenPresent(SCR8054_CALLS_LIST_RECEIVED))
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		DeleteScreenIfPresent( SCR8007_RECEIVED_CALL_OPTIONS );
#else
      DeleteBetweenScreen( SCR8006_CALL_RECEIVED, SCR8007_RECEIVED_CALL_OPTIONS );
#endif      
      else
#endif
		DeleteBetweenScreen( SCR8054_CALLS_LIST_RECEIVED, SCR8007_RECEIVED_CALL_OPTIONS );
   }
	else
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		DeleteScreenIfPresent( SCR8007_RECEIVED_CALL_OPTIONS );
#else
      DeleteBetweenScreen( SCR8006_CALL_RECEIVED, SCR8007_RECEIVED_CALL_OPTIONS );
#endif
	return;
}

void RspDeleteRejectedNum(void *info)
{
	S32 i;
	DEL_LAST_NUMBER_RSP *rsp;

	rsp = (DEL_LAST_NUMBER_RSP *)info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK )
	{
		for ( i = chis_p->currRejectedCallIndex;
		         i < ( chis_p->nRejectedCalls - 1 ) ; i++ )
			memcpy( &chis_p->rejectedCalls[i], &chis_p->rejectedCalls[i+1],
			          sizeof( REJECTED_CALL ) );
		chis_p->nRejectedCalls--;
		if ((chis_p->currRejectedCallIndex >= chis_p->nRejectedCalls) &&
		    (chis_p->nRejectedCalls != 0))
			chis_p->currRejectedCallIndex = chis_p->nRejectedCalls - 1;

		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}

	if( chis_p->nRejectedCalls == 0 )
	{
#ifdef __MMI_CH_MIXED_CALL_LOG__
      if (!IsScreenPresent(SCR8058_CALLS_LIST_REJECTED))
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		DeleteScreenIfPresent( SCR8523_REJECTED_CALL_OPTIONS );
#else
      DeleteBetweenScreen( SCR8522_CALL_REJECTED, SCR8523_REJECTED_CALL_OPTIONS );
#endif      
      else
#endif
		DeleteBetweenScreen( SCR8058_CALLS_LIST_REJECTED, SCR8523_REJECTED_CALL_OPTIONS );
		chis_p->nRejectedCallBeforeView = 0;
   		mmi_chist_rearrange_rejected_call_status_icon();
 
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
		//IdleResetStatusIcon( STATUS_ICON_SUBLCD_REJECTED_CALL );
#endif

   }
	else
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		DeleteScreenIfPresent( SCR8523_REJECTED_CALL_OPTIONS );
#else
      DeleteBetweenScreen( SCR8522_CALL_REJECTED, SCR8523_REJECTED_CALL_OPTIONS );
#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTDeleteDialedNum

  	PURPOSE				: To delete the current active dialed call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTDeleteDialedNum( void )
{
   ReqDeleteCallLog(PHB_LND, chis_p->currDialedCallIndex, 1);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTSaveDialedNum

  	PURPOSE				: To save the dialed number into the
						  phonebook

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTSaveDialedNum( void )
{
   chis_p->curCallLogType = PHB_LND;
	if( chis_p->dialedCalls[chis_p->currDialedCallIndex].number[0] == 0 )
	{
	   /* no name, number is in name field for display */
		SaveNumberFromIdleScrn(
		   (S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].pbName , MMI_PHB_ENTER_FROM_CALL_LOG);
	}
	else
	{
		SaveNumberFromIdleScrn(
		   (S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].number , MMI_PHB_ENTER_FROM_CALL_LOG);
	}
	return;
}

#ifdef __IP_NUMBER__
void SetTempUseIPNumber(pBOOL flag);
void CHISTCallDialedNumByIP(void)
{
#ifdef __MMI_MULTI_SIM__
	U8 reqSrcMod = GetReqSrcModIndex();
	MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, reqSrcMod);
#endif
	
	SetTempUseIPNumber(TRUE);

	CHISTCallDialedNum();
}
#endif

/**************************************************************

	FUNCTION NAME		: CHISTCallDialedNum

  	PURPOSE				: To make a call to dialed list entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTCallDialedNum( void )
{

	addToHistory = 0;

	mmi_trace(1,"Call MO CHISTCallDialedNum number[0]=%d", chis_p->dialedCalls[chis_p->currDialedCallIndex].number[0]);

	if( chis_p->dialedCalls[chis_p->currDialedCallIndex].number[0] == 0 )
	{
	   /* no name, number is in name field for display */
		if( chis_p->dialedCalls[chis_p->currDialedCallIndex].pbName[0] != 0 )
		{
		   chis_p->dialFromCallLog = ITEM_DIALED_CALL;
			MakeCall(
			   (S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].pbName );
			chis_p->currDialedCallIndex = 0;
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_NONUMBER_TO_DIAL),
			             IMG_GLOBAL_UNFINISHED, 1,
			             CS_NOTIFYDURATION,ERROR_TONE);
		}
	}
	else
	{
	   chis_p->dialFromCallLog = ITEM_DIALED_CALL;
		MakeCall( (S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].number );
		chis_p->currDialedCallIndex = 0;
	}
#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (!IsScreenPresent(SCR8055_CALLS_LIST_DIALED))
		DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
	else
#endif
	DeleteUptoScrID(SCR8055_CALLS_LIST_DIALED);
	return;
}

//2007-06-14, Li Tao
/**************************************************************

	FUNCTION NAME		: CHISTSendSMSDialedNum

  	PURPOSE				: To SendSMS the current active Dialed call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void mmi_msg_get_number_from_clg(U8 number[]);
void CHISTSendSMSDialedNum( void )
{
	if(IsMessagesReEntering())
	{
		DisplayPopup ((PU8)GetString (STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}

	mmi_msg_set_phb_send_case(MMI_SEND_SMS_FROM_CLG);

	if (chis_p->dialedCalls[chis_p->currDialedCallIndex].number[0] == 0)
	{
		//DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	else
	{
		//mmi_trace(1, "**CHISTSendSMSDialedNum*** the current number for Send SMS is %s *******", (U8*)chis_p->dialedCalls[chis_p->currDialedCallIndex].number);
		mmi_msg_get_number_from_clg((U8*)chis_p->dialedCalls[chis_p->currDialedCallIndex].number);
	}
}

/**************************************************************

	FUNCTION NAME		: CHISTEditDialedNum

  	PURPOSE				: To Edit the current active Dialed call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTEditDialedNum( void )
{
	if( chis_p->dialedCalls[chis_p->currDialedCallIndex].number[0] == 0 )
	{
		pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,
		   (S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].pbName);
	}
	else
	{
		pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,
		   (S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].number);
	}

       g_idle_context.LongPressEnable = 1;
	IdleScreenDigitHandler();
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTDeleteMissedNum

  	PURPOSE				: To delete the current active missed call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTDeleteMissedNum( void )
{
   U8 index;

	index = chis_p->currMissedCallIndex;
	ReqDeleteCallLog(PHB_LNM, (U8)(index+(chis_p->nMissedCalls -
	                   nMissedCallNumBkp)), 1);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTSaveMissedNum

  	PURPOSE				: To save the missed number into the
						  phonebook

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTSaveMissedNum( void )
{
   chis_p->curCallLogType = PHB_LNM;
	if( chis_p->missedCalls[chis_p->currMissedCallIndex].number[0] == 0 )
	{
		SaveNumberFromIdleScrn(
		   (S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].pbName , MMI_PHB_ENTER_FROM_CALL_LOG);
	}
	else
	{
		SaveNumberFromIdleScrn(
		   (S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].number , MMI_PHB_ENTER_FROM_CALL_LOG);
	}
	return;
}

#ifdef __IP_NUMBER__
/**************************************************************

	FUNCTION NAME		: CHISTCallMissedNumByIP

  	PURPOSE				: Use IP dialing to call a missed number

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTCallMissedNumByIP(void)
{
#ifdef __MMI_MULTI_SIM__
	U8 reqSrcMod  = GetReqSrcModIndex();
	MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, reqSrcMod);
#endif
	SetTempUseIPNumber(TRUE);

	CHISTCallMissedNum();
}
#endif

/**************************************************************

	FUNCTION NAME		: CHISTCallMissedNum

  	PURPOSE				: To make a call to missed list entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTCallMissedNum( void )
{
   addToHistory = 0;
   
	if( chis_p->missedCalls[chis_p->currMissedCallIndex].number[0] == 0 )
	{
		if( chis_p->missedCalls[chis_p->currMissedCallIndex].pbName[0] != 0 )
		{
		   chis_p->dialFromCallLog = ITEM_MISSED_CALL;
			MakeCall(
			   (S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].pbName );
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_NONUMBER_TO_DIAL),
			             IMG_GLOBAL_UNFINISHED, 1,
			             CS_NOTIFYDURATION,ERROR_TONE);
		}
	}
	else
	{
	   chis_p->dialFromCallLog = ITEM_MISSED_CALL;
		MakeCall(
		   (S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].number );
	}
#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (!IsScreenPresent(SCR8056_CALLS_LIST_MISSED))
		DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
	else
#endif
	DeleteUptoScrID(SCR8056_CALLS_LIST_MISSED);
	return;
}

void CHISTCallRejectedNum( void )
{
   addToHistory = 0;
   
	if( chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number[0] == 0 )
	{
		if( chis_p->rejectedCalls[chis_p->currRejectedCallIndex].pbName[0] != 0 )
		{
		   chis_p->dialFromCallLog = ITEM_REJECTED_CALL;
			MakeCall(
			   (S8 *)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].pbName );
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_NONUMBER_TO_DIAL),
			             IMG_GLOBAL_UNFINISHED, 1,
			             CS_NOTIFYDURATION,ERROR_TONE);
		}
	}
	else
	{
	   chis_p->dialFromCallLog = ITEM_REJECTED_CALL;
		MakeCall(
		   (S8 *)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number );
	}
#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (!IsScreenPresent(SCR8058_CALLS_LIST_REJECTED))
		DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
	else
#endif
	DeleteUptoScrID(SCR8058_CALLS_LIST_REJECTED);
	return;
}

//2007-06-14, Li Tao
/**************************************************************

	FUNCTION NAME		: CHISTSendSMSMissedNum

  	PURPOSE				: To SendSMS the current active Missed call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTSendSMSMissedNum( void )
{
	if(IsMessagesReEntering())
	{
		DisplayPopup ((PU8)GetString (STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}

	mmi_msg_set_phb_send_case(MMI_SEND_SMS_FROM_CLG);

	if (chis_p->missedCalls[chis_p->currMissedCallIndex].number[0] == 0)
	{
		//DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	else
	{
		//mmi_trace(1, "**CHISTSendSMSMissedNum**** the current number for Send SMS is %s ****", chis_p->missedCalls[chis_p->currMissedCallIndex].number);
		mmi_msg_get_number_from_clg((U8*)chis_p->missedCalls[chis_p->currMissedCallIndex].number);
	}
}

/**************************************************************

	FUNCTION NAME		: CHISTEditMissedNum

  	PURPOSE				: To Edit the current active Missed call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTEditMissedNum( void )
{
	if( chis_p->missedCalls[chis_p->currMissedCallIndex].number[0] == 0 )
	{
		DisplayPopup((PU8)GetString(ERROR_UNRECOGNIZED_NUMBER),
		             IMG_GLOBAL_ERROR, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
#ifdef __MMI_CH_MIXED_CALL_LOG__
		if (!IsScreenPresent(SCR8056_CALLS_LIST_MISSED))
			DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
		else
#endif
		DeleteUptoScrID(SCR8056_CALLS_LIST_MISSED);
	}
	else
	{
		pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,
		   (S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].number );
		g_idle_context.LongPressEnable = 1;
		IdleScreenDigitHandler();
	}
}

/**************************************************************

	FUNCTION NAME		: CHISTDeleteRecvdNum

  	PURPOSE				: To delete the current active recvd call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTDeleteRecvdNum( void )
{
	ReqDeleteCallLog( PHB_LNR, chis_p->currRecvdCallIndex, 1 );
	return;
}

void CHISTDeleteRejectedNum( void )
{
	ReqDeleteCallLog( PHB_LNRJ, chis_p->currRejectedCallIndex, 1 );
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTSaveRecvdNum

  	PURPOSE				: To save the recvd number into the
						  phonebook

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTSaveRecvdNum( void )
{
   chis_p->curCallLogType = PHB_LNR;
	if( chis_p->recvdCalls[chis_p->currRecvdCallIndex].number[0] == 0 )
	{
		SaveNumberFromIdleScrn(
		   (S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].pbName , MMI_PHB_ENTER_FROM_CALL_LOG);
	}
	else
	{
		SaveNumberFromIdleScrn(
		   (S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number , MMI_PHB_ENTER_FROM_CALL_LOG);
	}
	return;
}

void CHISTSaveRejectedNum( void )
{
   chis_p->curCallLogType = PHB_LNRJ;
	if( chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number[0] == 0 )
	{
		SaveNumberFromIdleScrn(
		   (S8 *)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].pbName , MMI_PHB_ENTER_FROM_CALL_LOG);
	}
	else
	{
		SaveNumberFromIdleScrn(
		   (S8 *)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number , MMI_PHB_ENTER_FROM_CALL_LOG);
	}
	return;
}

#ifdef __IP_NUMBER__
/**************************************************************

	FUNCTION NAME		: CHISTCallRecvdNumByIP

  	PURPOSE				: Use IP dialing to call a recvd number

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTCallRecvdNumByIP(void)
{
#ifdef __MMI_MULTI_SIM__
	U8 reqSrcMode = GetReqSrcModIndex();
	MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, reqSrcMode);
#endif

	SetTempUseIPNumber(TRUE);

	CHISTCallRecvdNum();
}

void CHISTCallRejectedNumByIP(void)
{
#ifdef __MMI_MULTI_SIM__
	U8 reqSrcMod = GetReqSrcModIndex();
	MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, reqSrcMod);
#endif
	
	SetTempUseIPNumber(TRUE);

	CHISTCallRejectedNum();
}

#endif

/**************************************************************

	FUNCTION NAME		: CHISTCallRecvdNum

  	PURPOSE				: To make a call to recvd list entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTCallRecvdNum( void )
{
   addToHistory = 0;
   
	if( chis_p->recvdCalls[chis_p->currRecvdCallIndex].number[0] == 0 )
	{
		if( chis_p->recvdCalls[chis_p->currRecvdCallIndex].pbName[0] != 0 )
		{
		   chis_p->dialFromCallLog = ITEM_RECVD_CALL;
			MakeCall(
			   (S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].pbName );
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_NONUMBER_TO_DIAL),
			             IMG_GLOBAL_UNFINISHED, 1,
			             CS_NOTIFYDURATION,ERROR_TONE);
		}
	}
	else
	{
	   chis_p->dialFromCallLog = ITEM_RECVD_CALL;
	   MakeCall(
	      (S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number );
	}
#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (!IsScreenPresent(SCR8054_CALLS_LIST_RECEIVED))
		DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
	else
#endif
	DeleteUptoScrID(SCR8054_CALLS_LIST_RECEIVED);
	return;
}

//2007-06-14, Li Tao
/**************************************************************

	FUNCTION NAME		: CHISTSendSMSRecvdNum

  	PURPOSE				: To SendSMS the current active Recvd call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTSendSMSRecvdNum( void )
{
	if(IsMessagesReEntering())
	{
		DisplayPopup ((PU8)GetString (STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}

	mmi_msg_set_phb_send_case(MMI_SEND_SMS_FROM_CLG);

	if (chis_p->recvdCalls[chis_p->currRecvdCallIndex].number[0] == 0)
	{
		//DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	else
	{
		//mmi_trace(1, "**CHISTSendSMSRecvdNum** the current number for Send SMS is %s *****", (U8*)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number);
		mmi_msg_get_number_from_clg((U8*)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number);
	}
}

void CHISTSendSMSRejectedNum( void )
{
	if(IsMessagesReEntering())
	{
		DisplayPopup ((PU8)GetString (STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}

	mmi_msg_set_phb_send_case(MMI_SEND_SMS_FROM_CLG);

	if (chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number[0] == 0)
	{
		//DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	else
	{
		//mmi_trace(1, "**CHISTSendSMSRecvdNum** the current number for Send SMS is %s *****", (U8*)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number);
		mmi_msg_get_number_from_clg((U8*)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number);
	}
}

#if defined(__MMI_PHB_SEND_MMS_FROM_PHB__)
void CHISTSendMMSRecvdNum( void )
{
#ifdef JATAAYU_SUPPORT_MMS
	jMMS_SendFromApp((U8*)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number);
#endif
}

void CHISTSendMMSRejectedNum( void )
{
#ifdef JATAAYU_SUPPORT_MMS
	jMMS_SendFromApp((U8*)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number);
#endif
}

void CHISTSendMMSMissedNum( void )
{
#ifdef JATAAYU_SUPPORT_MMS
	jMMS_SendFromApp((U8*)chis_p->missedCalls[chis_p->currMissedCallIndex].number);
#endif
}

void CHISTSendMMSDialedNum( void )
{
#ifdef JATAAYU_SUPPORT_MMS
	jMMS_SendFromApp((U8*)chis_p->dialedCalls[chis_p->currDialedCallIndex].number);
#endif
}
#endif
/**************************************************************

	FUNCTION NAME		: CHISTEditRecvdNum

  	PURPOSE				: To Edit the current active Recvd call entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTEditRecvdNum( void )
{
   if( chis_p->recvdCalls[chis_p->currRecvdCallIndex].number[0] == 0 )
	{
		DisplayPopup((PU8)GetString(ERROR_UNRECOGNIZED_NUMBER),
		             IMG_GLOBAL_ERROR, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
#ifdef __MMI_CH_MIXED_CALL_LOG__
		if (!IsScreenPresent(SCR8054_CALLS_LIST_RECEIVED))
			DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
		else
#endif
		DeleteUptoScrID(SCR8054_CALLS_LIST_RECEIVED);
	}
	else
	{
		pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,
		   (S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number );
		g_idle_context.LongPressEnable = 1;
		IdleScreenDigitHandler();
	}
}

void CHISTEditRejectedNum( void )
{
   if( chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number[0] == 0 )
	{
		DisplayPopup((PU8)GetString(ERROR_UNRECOGNIZED_NUMBER),
		             IMG_GLOBAL_ERROR, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
#ifdef __MMI_CH_MIXED_CALL_LOG__
		if (!IsScreenPresent(SCR8058_CALLS_LIST_REJECTED))
			DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
		else
#endif
		DeleteUptoScrID(SCR8058_CALLS_LIST_REJECTED);
	}
	else
	{
		pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,
		   (S8 *)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number );
		g_idle_context.LongPressEnable = 1;
		IdleScreenDigitHandler();
	}
}


/**************************************************************

	FUNCTION NAME		: CHISTPopulateDialedCallList

  	PURPOSE				: To fill in the array of numbers/names
						  to show dialed call list.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTPopulateDialedCallList( U8* numberList[], U8* phoneList[] )
{

	U16	index	= 0;
	
	U16 nSuited = 0;
	#ifdef __MMI_VIP_FUNCTION__
	U16 i = 0;
	#endif
	
	for ( index = 0; index < chis_p->nDialedCalls; index++ )
	{
		if ( chis_p->dialedCalls[index].nameLength > 0)
		{
			#ifdef __MMI_VIP_FUNCTION__
			if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
			{
				nSuited++;
			}
			else
			#endif
			{
				phoneList[index - nSuited] = (U8 *)chis_p->dialedCalls[index].pbName;
				numberList[index - nSuited] = (U8 *)chis_p->dialedCalls[index].number;
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
		}
		else
		{
			if(chis_p->dialedCalls[index].number[0] != 0)
			{
			#ifdef __MMI_VIP_FUNCTION__
				if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
				{
					nSuited++;
				}
				else
			#endif
				{
					phoneList[index - nSuited] = (U8 *)chis_p->dialedCalls[index].number;
					#ifdef __MMI_VIP_FUNCTION__
					g_vip_context.CallLogIndexMap[i++] = index;
					#endif
				}
			}
			else
			{
				phoneList[index- nSuited] = (PU8)GetString(STR_UNKNOWN_NUMBER);
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
			numberList[index- nSuited] = NULL;
		}
	}
	#ifdef __MMI_VIP_FUNCTION__
	g_vip_context.nCallLogSuited = nSuited;
	#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTPopulateMissedCallList

  	PURPOSE				: To fill in the array of numbers/names
						  to show missed call list.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTPopulateMissedCallList( U8* numberList[], U8* phoneList[] )
{
	U16	index	= 0;

	U16 nSuited = 0;
	#ifdef __MMI_VIP_FUNCTION__
	U16 i = 0;
	#endif

	for ( index = 0; index < chis_p->nMissedCalls; index++ )
	{
		if ( chis_p->missedCalls[index].nameLength > 0)
		{
			#ifdef __MMI_VIP_FUNCTION__
			if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
			{
				nSuited++;
			}
			else
			#endif
			{
				phoneList[index - nSuited] = (U8 *)chis_p->missedCalls[index].pbName;
				numberList[index - nSuited] = (U8 *)chis_p->missedCalls[index].number;
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
		}
		else
		{
			if( chis_p->missedCalls[index].number[0] != 0 )
			{
			#ifdef __MMI_VIP_FUNCTION__
				if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
				{
					nSuited++;
				}
				else
			#endif
				{
					phoneList[index - nSuited] = (U8 *)chis_p->missedCalls[index].number;
					#ifdef __MMI_VIP_FUNCTION__
					g_vip_context.CallLogIndexMap[i++] = index;
					#endif
				}
			}
			else
			{
				phoneList[index - nSuited] = (PU8)GetString(STR_UNKNOWN_NUMBER);
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
			numberList[index - nSuited] = NULL;
		}
	}
	#ifdef __MMI_VIP_FUNCTION__
	g_vip_context.nCallLogSuited = nSuited;
	#endif
	return;
}

void CHISTPopulateRejectedCallList( U8* numberList[], U8* phoneList[] )
{
	U16	index	= 0;

	U16 nSuited = 0;
	#ifdef __MMI_VIP_FUNCTION__
	U16 i = 0;
	#endif

	for ( index = 0; index < chis_p->nRejectedCalls; index++ )
	{
		if ( chis_p->rejectedCalls[index].nameLength > 0)
		{
			#ifdef __MMI_VIP_FUNCTION__
			if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
			{
				nSuited++;
			}
			else
			#endif
			{
				phoneList[index - nSuited] = (U8 *)chis_p->rejectedCalls[index].pbName;
				numberList[index - nSuited] = (U8 *)chis_p->rejectedCalls[index].number;
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
		}
		else
		{
			if( chis_p->rejectedCalls[index].number[0] != 0 )
			{
			#ifdef __MMI_VIP_FUNCTION__
				if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
				{
					nSuited++;
				}
				else
			#endif
				{
					phoneList[index - nSuited] = (U8 *)chis_p->rejectedCalls[index].number;
					#ifdef __MMI_VIP_FUNCTION__
					g_vip_context.CallLogIndexMap[i++] = index;
					#endif
				}
			}
			else
			{
				phoneList[index - nSuited] = (PU8)GetString(STR_UNKNOWN_NUMBER);
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
			numberList[index - nSuited] = NULL;
		}
	}
	#ifdef __MMI_VIP_FUNCTION__
	g_vip_context.nCallLogSuited = nSuited;
	#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTPopulateRecvdCallList

  	PURPOSE				: To fill in the array of numbers/names
						  to show recvd call list.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTPopulateRecvdCallList( U8* numberList[], U8* phoneList[] )
{
	U16	index	= 0;

	U16 nSuited = 0;
	#ifdef __MMI_VIP_FUNCTION__
	U16 i = 0;
	#endif

	for ( index = 0; index < chis_p->nRecvdCalls; index++ )
	{
		if ( chis_p->recvdCalls[index].nameLength > 0)
		{
			#ifdef __MMI_VIP_FUNCTION__
			if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
			{
				nSuited++;
			}
			else
			#endif
			{
				phoneList[index] = (U8 *)chis_p->recvdCalls[index].pbName;
				numberList[index] = (U8 *)chis_p->recvdCalls[index].number;
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
		}
		else
		{
			if( chis_p->recvdCalls[index].number[0] != 0 )
			{
			#ifdef __MMI_VIP_FUNCTION__
				if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
				{
					nSuited++;
				}
				else
			#endif
				{
					phoneList[index - nSuited] = (U8 *)chis_p->recvdCalls[index].number;
					#ifdef __MMI_VIP_FUNCTION__
					g_vip_context.CallLogIndexMap[i++] = index;
					#endif
				}
			}
			else
			{
				phoneList[index - nSuited] = (PU8)GetString(STR_UNKNOWN_NUMBER);
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
			numberList[index - nSuited] = NULL;
		}
	}
	#ifdef __MMI_VIP_FUNCTION__
	g_vip_context.nCallLogSuited = nSuited;
	#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTGetCurrDialedCallIndex

  	PURPOSE				: To set the current active call item

	INPUT PARAMETERS	: ind - index into list of call entries

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTGetCurrDialedCallIndex( S32 ind )
{
	#ifdef __MMI_VIP_FUNCTION__
	chis_p->currDialedCallIndex = g_vip_context.CallLogIndexMap[ind];
	#else
	chis_p->currDialedCallIndex = (U8)ind;
	#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTGetCurrMissedCallIndex

  	PURPOSE				: To set the current active call item

	INPUT PARAMETERS	: ind - index into list of call entries

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTGetCurrMissedCallIndex( S32 ind )
{
	#ifdef __MMI_VIP_FUNCTION__
	chis_p->currMissedCallIndex = g_vip_context.CallLogIndexMap[ind];
	#else
	chis_p->currMissedCallIndex = (U8)ind;
	#endif
	return;
}

void CHISTGetCurrRejectedCallIndex( S32 ind )
{
	#ifdef __MMI_VIP_FUNCTION__
	chis_p->currRejectedCallIndex = g_vip_context.CallLogIndexMap[ind];
	#else
	chis_p->currRejectedCallIndex = (U8)ind;
	#endif
	return;
}


/**************************************************************

	FUNCTION NAME		: CHISTGetCurrRecvdCallIndex

  	PURPOSE				: To set the current active call item

	INPUT PARAMETERS	: ind - index into list of call entries

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTGetCurrRecvdCallIndex( S32 ind )
{
	#ifdef __MMI_VIP_FUNCTION__
	chis_p->currRecvdCallIndex = g_vip_context.CallLogIndexMap[ind];
	#else
	chis_p->currRecvdCallIndex = (U8)ind;
	#endif
	return;
}

void CHISTResetCallLogIndex (void)
{
   chis_p->currCostItem = 0;
   chis_p->currDialedCallIndex = 0;
   chis_p->currMissedCallIndex = 0;
   chis_p->currRecvdCallIndex = 0;
#ifdef __MMI_CH_MIXED_CALL_LOG__
   chis_p->currMixedCallIndex = 0;
#endif
   return;
}

/**************************************************************

	FUNCTION NAME		: CHISTLogDialedCall

  	PURPOSE				: To make an entry into dialed call list

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

 **************************************************************/
void CHISTLogDialedCall( CALL_INFO* call )
{
   DeleteScreenIfPresent(SCR8055_CALLS_LIST_DIALED);
   DeleteScreenIfPresent(SCR8008_CALL_DIALED);
   DeleteScreenIfPresent(SCR8009_DIALED_CALL_OPTIONS);
	ReqWriteCallLog( call, PHB_LND );
	return;
}

/**************************************************************

	FUNCTION NAME		: CalAdjustDecimal

  	PURPOSE				: Adjust radix point

	INPUT PARAMETERS	: U8 *operator1, U8 *operator2

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CalAdjustDecimal(U8 *operator1, U8 *operator2, U8 uDirection)
{
	U8 i;
	UINT8 n;
	BOOL Flag = FALSE;
	ASSERT(operator1 != NULL);
	ASSERT(uDirection  == 0 || uDirection == 1);

	if (uDirection == 0)
	{
		n = strlen(operator2);
		for (i = 0; i < n; i++)
		{
			if (operator2[i] == '.')
			{
				Flag = TRUE;
				strcpy(operator1, operator2);
				break;		
			}
		}
		if (Flag == FALSE)
		{
			strcpy(operator1, operator2);
			strcat(operator1, ".00");
		}
	}	
	else
	{
		if (operator2[0] == '.')
			{
				strcpy(operator1, "0");
				strcat(operator1, operator2);
			}
	}

}

/**************************************************************

	FUNCTION NAME		: CalcAndSaveCallcost

  	PURPOSE				: Calculate and save Call Cost

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
 void StrToRate( CALOPERATOR_STRUCT *Rate, char * str);
void RateToStr(char *str, CALOPERATOR_STRUCT *Rate);
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void coolsand_UI_atof(CALOPERATOR_STRUCT *result,  U8 *stringCnt, UI_string_type  s);
#else
DOUBLE coolsand_UI_atof(UI_string_type  s);
#endif
void CalcAndSaveCallcost(void)
{
	#ifdef _MMI_OPERATOR_
		UINT16 uCallTime = 0;
		UINT16 error = 0;

		CALOPERATOR_STRUCT Cop_currency;
		CALOPERATOR_STRUCT Cop_ppu;
		CALOPERATOR_STRUCT Cop_CallTime;
		CALOPERATOR_STRUCT Cop_AllCallCost;
		CALOPERATOR_STRUCT Cop_Temp;

		U8 sCallTime[10] = {0}; // 24 * 60
		//U8 sUnicodeBuffer[100] = {0};
		U8 szBuffer[50] = {0};
		U8 szBuffer1[50] = {0};
		U8 szBuffer2[50] = {0};
		U8 uStrPos = 0;
		//U8 i;

		/*
		计算最后通话费用.
		**/	
		uCallTime = chis_p->last_call_time.nHour * 60 + chis_p->last_call_time.nMin;
		if (chis_p->last_call_time.nSec > 0)
			uCallTime++;

		coolsand_UI_itoa(uCallTime, (UI_string_type)sCallTime, 10);
		UnicodeToAnsii((S8 *)szBuffer, (S8 *)sCallTime);
		StrToRate(&Cop_CallTime, szBuffer);
		UnicodeToAnsii(szBuffer, chis_p->newPPUStr);
		StrToRate(&Cop_ppu, szBuffer);
		StrToRate(&Cop_currency, chis_p->currencyStr);
			
		AnsiiToUnicodeString(chis_p->newCurrencyStr, chis_p->currencyStr);	
		UnicodeToAnsii(szBuffer, sCallTime);
		
		CalculatorMult(&Cop_CallTime, &Cop_ppu);
		CalculatorMult(&Cop_CallTime, &Cop_currency);
		RateToStr(szBuffer2, &Cop_CallTime);
		CalAdjustDecimal(chis_p->lastCallCost, szBuffer2, 1);
		
		/*
		计算总通话费用.
		**/
		Cop_CallTime.Exponent = 1;
		Cop_CallTime.Fraction = 0;
		Cop_CallTime.Symbol = 1;
		memset(Cop_CallTime.operBuff, 0, sizeof(Cop_CallTime.operBuff));
		memset(sCallTime, 0, sizeof(sCallTime));
		memset(szBuffer, 0, sizeof(szBuffer));
		
		CalAdjustDecimal(szBuffer1, chis_p->lastCallCost, 0);
		AnsiiToUnicodeString(szBuffer, szBuffer1);
		uStrPos = 0;
		coolsand_UI_atof(&Cop_CallTime, &uStrPos, (UI_string_type)szBuffer);

		CalAdjustDecimal(szBuffer1, chis_p->allCallCost, 0);
		AnsiiToUnicodeString(szBuffer, szBuffer1);

		Cop_AllCallCost.Exponent = 1;
		Cop_AllCallCost.Fraction = 0;
		Cop_AllCallCost.Symbol = 1;
		memset(Cop_AllCallCost.operBuff, 0, sizeof(Cop_AllCallCost.operBuff));
		uStrPos = 0;
		coolsand_UI_atof(&Cop_AllCallCost, &uStrPos, (UI_string_type)szBuffer);

		CalculatorAligDot(&Cop_AllCallCost, &Cop_CallTime);
		CalculatorAdd(Cop_AllCallCost.operBuff, Cop_CallTime.operBuff);
		
		CalcAdjustResult1(&Cop_AllCallCost);	
		Calgetrealresult(&Cop_Temp, &Cop_AllCallCost);

		strcpy(chis_p->allCallCost, Cop_Temp.operBuff);

		/*
		把最后通话费用和总通话费用存储到Flash中.
		**/
		#ifdef __MMI_MULTI_SIM__
		WriteRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, chis_p->nSimId, 
					(void *)chis_p->lastCallCost, sizeof(chis_p->lastCallCost), &error);
		WriteRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, chis_p->nSimId, 
					(void *)chis_p->allCallCost, sizeof(chis_p->allCallCost), &error);
		#else  /* No Defined __MMI_MULTI_SIM__ */
		WriteRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, 1, 
					(void *)chis_p->lastCallCost, sizeof(chis_p->lastCallCost), &error);
		WriteRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, 1, 
					(void *)chis_p->allCallCost, sizeof(chis_p->allCallCost), &error);
		#endif /* __MMI_MULTI_SIM__ */
	#else /* No Defined _MMI_OPERATOR_*/
		double fLastCallCost = 0;
		double fAllCallCost = 0;
		UINT16 uCallTime = 0;
		UINT16 error = 0;

		uCallTime = chis_p->last_call_time.nHour * 60 + chis_p->last_call_time.nMin;
		if (chis_p->last_call_time.nSec > 0)
			uCallTime++;
		/*
		计算最后通话费用和总通话费用.
		**/
		fLastCallCost = uCallTime * atof((s8 *)chis_p->currencyStr) * chis_p->ppu;
		fAllCallCost = chis_p->allCallCost + fLastCallCost;

		/*
		把最后通话费用和总通话费用存储到Flash中.
		**/
#ifdef __MMI_MULTI_SIM__
	WriteRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, chis_p->nSimId, 
			(void *)&fLastCallCost, sizeof(fLastCallCost), &error);
	WriteRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, chis_p->nSimId, 
			(void *)&fAllCallCost, sizeof(fAllCallCost), &error);
#else  /* No Defined __MMI_MULTI_SIM__ */
		WriteRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, 1, 
			(void *)&fLastCallCost, sizeof(fLastCallCost), &error);
		WriteRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, 1, 
			(void *)&fAllCallCost, sizeof(fAllCallCost), &error);
#endif /* __MMI_MULTI_SIM__ */
	#endif
}


/**************************************************************

	FUNCTION NAME		: CHISTLogDialedCallDuration

  	PURPOSE				: To make an entry into dialed call list

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

 **************************************************************/
void CHISTLogDialedCallDuration( CALL_INFO* call )
{
	S16 error=0;

	if( CompareTime(call->end_time, call->start_time, NULL) != TIME_EQUAL )
	{
		GetTimeDifference(&call->end_time, &call->start_time,
		                  &chis_p->last_call_time);
		chis_p->total_out_time = IncrementTime(chis_p->total_out_time,
		                                       chis_p->last_call_time);

#ifdef __MMI_MULTI_SIM__
	WriteRecord(NVRAM_EF_CALL_TIME_LID,2+3 * chis_p->nSimId,(void *)&chis_p->total_out_time,
		            sizeof(MYTIME),&error);

	WriteRecord(NVRAM_EF_CALL_TIME_LID,1 + 3* chis_p->nSimId,(void *)&chis_p->last_call_time,
		            sizeof(MYTIME),&error);		
#else  /* No Defined __MMI_MULTI_SIM__ */
	WriteRecord(NVRAM_EF_CALL_TIME_LID,2,(void *)&chis_p->total_out_time,
		            sizeof(MYTIME),&error);

	WriteRecord(NVRAM_EF_CALL_TIME_LID,1,(void *)&chis_p->last_call_time,
		            sizeof(MYTIME),&error);		
#endif /* __MMI_MULTI_SIM__ */
		CalcAndSaveCallcost();			
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTLogMissedCall

  	PURPOSE				: To make an entry into missed call list

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTLogMissedCall( CALL_INFO* call )
{
	
#ifdef __MMI_MULTI_SIM__
	mmi_trace(g_sw_SS," Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif

	mmi_trace(g_sw_SS," Func: %s , chis_p->nMissedCallBeforeView=%d", __FUNCTION__, chis_p->nMissedCallBeforeView);

   chis_p->nMissedCallBeforeView++;
   /* overflow, loop back */
   if (chis_p->nMissedCallBeforeView == 0)
      chis_p->nMissedCallBeforeView = 1;
#ifdef __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__
    mmi_chist_set_num_missed_call_before_view();
#endif
   PRINT_INFORMATION(("CHISTLogMissedCall: show status icon"));
//   IdleSetStatusIcon( STATUS_ICON_MISSED_CALL );
	mmi_chist_rearrange_missed_call_status_icon();
	mmi_trace(g_sw_SS," Func: %s , chis_p->nMissedCallBeforeView=%d", __FUNCTION__, chis_p->nMissedCallBeforeView);
 
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
   IdleSetStatusIcon( STATUS_ICON_SUBLCD_MISSED_CALL );   
#endif
 
   DeleteScreenIfPresent(SCR8056_CALLS_LIST_MISSED);
   DeleteScreenIfPresent(SCR8520_CALL_MISSED);
   DeleteScreenIfPresent(SCR8521_MISSED_CALL_OPTIONS);
   ReqWriteCallLog( call, PHB_LNM );
	return;
}

void CHISTLogRejectedCall( CALL_INFO* call )
{
   chis_p->nRejectedCallBeforeView++;
   /* overflow, loop back */
   if (chis_p->nRejectedCallBeforeView == 0)
      chis_p->nRejectedCallBeforeView = 1;
   PRINT_INFORMATION(("CHISTLogRejectedCall: show status icon"));
   mmi_chist_rearrange_rejected_call_status_icon();
 
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
   //IdleSetStatusIcon( STATUS_ICON_SUBLCD_REJECTED_CALL );   
#endif
 
   DeleteScreenIfPresent(SCR8058_CALLS_LIST_REJECTED);
   DeleteScreenIfPresent(SCR8522_CALL_REJECTED);
   DeleteScreenIfPresent(SCR8523_REJECTED_CALL_OPTIONS);
   ReqWriteCallLog( call, PHB_LNRJ );
	
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTLogRecvdCall

  	PURPOSE				: To make an entry into recvd call list

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTLogRecvdCall( CALL_INFO* call )
{
   DeleteScreenIfPresent(SCR8054_CALLS_LIST_RECEIVED);
   DeleteScreenIfPresent(SCR8006_CALL_RECEIVED);
   DeleteScreenIfPresent(SCR8007_RECEIVED_CALL_OPTIONS);
	ReqWriteCallLog( call, PHB_LNR );
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTLogRecvdCallDuration

  	PURPOSE				: To make an entry into recvd call list

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTLogRecvdCallDuration( CALL_INFO* call )
{
	S16 error=0;

	if( CompareTime(call->end_time, call->start_time, NULL) != TIME_EQUAL )
	{
		GetTimeDifference(&call->end_time, &call->start_time,
		                  &chis_p->last_call_time);
		chis_p->total_in_time = IncrementTime(chis_p->total_in_time,
		                                      chis_p->last_call_time);

#ifdef __MMI_MULTI_SIM__
	WriteRecord(NVRAM_EF_CALL_TIME_LID,3 + 3*chis_p->nSimId,(void *)&chis_p->total_in_time,
		            sizeof(MYTIME),&error);
	WriteRecord(NVRAM_EF_CALL_TIME_LID,1 + 3* chis_p->nSimId,(void *)&chis_p->last_call_time,
		            sizeof(MYTIME),&error);
#else  /* No Defined __MMI_MULTI_SIM__ */
	WriteRecord(NVRAM_EF_CALL_TIME_LID,3,(void *)&chis_p->total_in_time,
		            sizeof(MYTIME),&error);
	WriteRecord(NVRAM_EF_CALL_TIME_LID,1,(void *)&chis_p->last_call_time,
		            sizeof(MYTIME),&error);
#endif /* __MMI_MULTI_SIM__ */
		CalcAndSaveCallcost();
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTExtractPSCallLog

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTExtractPSCallLog( LOG_CALL * logCall,
                                l4c_phb_ln_entry_struct* phbLastNumber)
{
	U32 m=0;

   if ((phbLastNumber->alpha_id.name_length > MAX_PB_NAME_LENGTH) ||
      (strlen((S8*)phbLastNumber->tel.number) >= MAX_CC_ADDR_LEN))
   {
	   PRINT_INFORMATION(("CHISTExtractPSCallLog: wrong length"));
      return;
   }

	if(MMI_PHB_UCS2==(phbLastNumber->alpha_id.name_dcs))
	{
#ifdef MMI_ON_HARDWARE_P
        ASSERT(phbLastNumber->alpha_id.name_length%2 == 0);
#endif

        while(m != (phbLastNumber->alpha_id.name_length))
        {
            /* big endian <-> little endian */
            logCall->pbName[m] = phbLastNumber->alpha_id.name[m+1];
            logCall->pbName[m+1] = phbLastNumber->alpha_id.name[m];
            m+=2;
        }
        logCall->pbName[m] = '\0';
        logCall->pbName[m+1] = '\0';
	}
	else
	{
	   if (strlen ((S8*)phbLastNumber->alpha_id.name) <= MAX_PB_NAME_LENGTH)	   
		AnsiiToUnicodeString((S8*)logCall->pbName,
		                     (S8*)phbLastNumber->alpha_id.name);
	}
	logCall->nameLength = (U8)pfnUnicodeStrlen((PS8)logCall->pbName);
   logCall->nameDCS = phbLastNumber->alpha_id.name_dcs;

	if( phbLastNumber->tel.type & 0x10 )
	{
		logCall->number[0] = '+';
		AnsiiToUnicodeString( (PS8)(logCall->number+2),
		                      (PS8)phbLastNumber->tel.number);
	}
	else
	{
	   AnsiiToUnicodeString( (PS8)logCall->number,
	                         (PS8)phbLastNumber->tel.number);
	}
	logCall->numType = phbLastNumber->tel.type;
	logCall->numLength = (U8)pfnUnicodeStrlen((PS8)logCall->number);

	ConvertRTC2MYTIME(&logCall->startTime,&phbLastNumber->time);
	mmi_dt_utc_sec_2_mytime(phbLastNumber->call_duration, &logCall->duration);
	logCall->no_of_times = phbLastNumber->count;
   return;
}

/**************************************************************

	FUNCTION NAME		: ReadyLogIndication

  	PURPOSE				: Receives call log ready indication from PS during 
  							  power on

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReadyLogIndication(void *info)
{

	U8 nSimID;
	mmi_chis_num_struct *pchis_num;
	pchis_num = (mmi_chis_num_struct *)info;
	

	nSimID = MTPNP_AD_get_protocol_event_src_mode();

	#ifdef __MMI_MULTI_SIM__
	SetCHISContext(nSimID);
	#endif

	chis_p->nDialedCalls =  pchis_num->nDialedcall;
	chis_p->nMissedCalls =  pchis_num->nMissedcall;
	chis_p->nRecvdCalls =  pchis_num->nReceivedcall;
	chis_p->nRejectedCalls =  pchis_num->nRejectedcall;

#ifdef __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__
	chis_p->nMissedCallBeforeView = CHISGetUnreadMissedCallNum();
	mmi_chist_rearrange_missed_call_status_icon();
#endif
	mmi_trace(g_sw_CALLLOG," SIM1 Func: %s nDialedCalls: %d, nMissedCalls: %d, nRecvdCalls: %d, nRejectedCalls: %d,nUnreadMissedcall: %d", __FUNCTION__,
				chis_p->nDialedCalls, chis_p->nMissedCalls, chis_p->nRecvdCalls, chis_p->nRejectedCalls, chis_p->nMissedCallBeforeView);

	chis_p->isCallLogReady = TRUE;
	chis_p->isFirstTime = FALSE;
	chis_p->sequenceId = 0;

//	ReqReadDialedNum();
//	return;
}

U16 CHISGetUnreadMissedCallNum()
{
	U16 nUnreadMissedCallnum = 0;
	S16 error;
#ifdef __MMI_MULTI_SIM__
	ReadRecord(NVRAM_EF_UNREAD_MISSED_NUM_LID, 1 + chis_p->nSimId, (void *)&nUnreadMissedCallnum, DS_SHORT, &error);
#else  /* No Defined __MMI_MULTI_SIM__ */
	ReadRecord(NVRAM_EF_UNREAD_MISSED_NUM_LID, 1 , (void *)&nUnreadMissedCallnum, DS_SHORT, &error);
#endif /* __MMI_MULTI_SIM__ */
	return nUnreadMissedCallnum;
}
/**************************************************************

	FUNCTION NAME		: CHISTGetDialFromCallLogFlag

  	PURPOSE				: Returns dial from which type of call log to 
  							  call management

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 CHISTGetDialFromCallLogFlag (void)
{
   return chis_p->dialFromCallLog;
}

/**************************************************************

	FUNCTION NAME		: CHISTSetDialFromCallLogFlag

  	PURPOSE				: Sets dial which type of call log 

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTSetDialFromCallLogFlag (U8 flag)
{
   chis_p->dialFromCallLog = flag;
   return;
}

/**************************************************************

	FUNCTION NAME		: CHISTGetCallLogName

  	PURPOSE				: Returns the name of call log that is dialed

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTGetCallLogName(LOG_CALL *item)
{
   LOG_CALL *curr_hilite;

   memset (item, 0, sizeof(LOG_CALL));

   switch (chis_p->dialFromCallLog)
   {
      case ITEM_DIALED_CALL:
         curr_hilite = &chis_p->dialedCalls[chis_p->currDialedCallIndex];
         break;
      case ITEM_MISSED_CALL:
         curr_hilite = &chis_p->missedCalls[chis_p->currMissedCallIndex];
         break;
      case ITEM_RECVD_CALL:
         curr_hilite = &chis_p->recvdCalls[chis_p->currRecvdCallIndex];
         break;
      case ITEM_REJECTED_CALL:
         curr_hilite = &chis_p->rejectedCalls[chis_p->currRejectedCallIndex];
         break;

#ifdef __MMI_CH_MIXED_CALL_LOG__
      case ITEM_MIXED_CALL:
         curr_hilite = chis_p->mixedCalls[chis_p->currMixedCallIndex];
         break;
#endif
      default:
         return;
   }

   if (curr_hilite->number[0] != 0)
   {
      memcpy(item->pbName, curr_hilite->pbName, sizeof (item->pbName));
      item->nameLength = curr_hilite->nameLength;
      item->nameDCS = curr_hilite->nameDCS;
   }
   return;
}

/**************************************************************

	FUNCTION NAME		: CHISTGetMissedCallCountBeforeView

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 CHISTGetMissedCallCountBeforeView(U8 nSimID )
{
#ifdef __MMI_MULTI_SIM__
	return g_CallHistory_cntxt[nSimID].nMissedCallBeforeView;
#else
	return chis_p->nMissedCallBeforeView;
#endif
}


/**************************************************************

	FUNCTION NAME		: CHISTGetRejectedCallCountBeforeView

  	PURPOSE				:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 CHISTGetRejectedCallCountBeforeView(U8 nSimID )
{
#ifdef __MMI_MULTI_SIM__
	return g_CallHistory_cntxt[nSimID].nRejectedCallBeforeView;
#else
	return chis_p->nRejectedCallBeforeView;
#endif
}



/*****************************************************************************
 * FUNCTION
 *  mmi_chist_rearrange_missed_call_status_icon
 * DESCRIPTION
 *  determine to show missed call icon or not
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_chist_rearrange_missed_call_status_icon(void)
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
		HideStatusIcon(STATUS_ICON_MISSED_CALL + i);
		if(CHISTGetMissedCallCountBeforeView(i) != 0 
			#ifdef __MMI_MULTI_SIM__
			&& MTPNP_PFAL_Is_Card_Usable(i)
			#endif
			)
		{
			ShowStatusIcon(STATUS_ICON_MISSED_CALL + i);
		}
	}

	UpdateStatusIcons();
}



#ifdef __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__
/*****************************************************************************
 * FUNCTION
 *  mmi_chist_set_num_missed_call_before_view
 * DESCRIPTION
 *  set NVRAM_CHIST_NUM_MISSED_CALL_BEFORE_VIEW
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_chist_set_num_missed_call_before_view(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 pError;
    U16 tempNumMissedCallBeforeView;    /* Temp Solution For Write 2 bytes NVRAM_PHB_NAME_LIST_FILTER */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    tempNumMissedCallBeforeView = (U16)chis_p->nMissedCallBeforeView;
//    WriteValue(NVRAM_CHIST_NUM_MISSED_CALL_BEFORE_VIEW, &tempNumMissedCallBeforeView, DS_SHORT, &pError);        /* 2 bytes for DS_SHORT */
#ifdef __MMI_MULTI_SIM__
    WriteRecord(NVRAM_EF_UNREAD_MISSED_NUM_LID, 1 + chis_p->nSimId, &tempNumMissedCallBeforeView, DS_SHORT, &pError);        /* 2 bytes for DS_SHORT */
#else  /* No Defined __MMI_MULTI_SIM__ */
    WriteRecord(NVRAM_EF_UNREAD_MISSED_NUM_LID, 1, &tempNumMissedCallBeforeView, DS_SHORT, &pError);        /* 2 bytes for DS_SHORT */
#endif /* __MMI_MULTI_SIM__ */
}
#endif /* __MMI_CHIST_MISSED_CALL_NUM_ACCUMULATE_AFTER_REBOOT__ */


/*****************************************************************************
 * FUNCTION
 *  mmi_chist_rearrange_rejected_call_status_icon
 * DESCRIPTION
 *  determine to show rejected call icon or not
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_chist_rearrange_rejected_call_status_icon(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 i;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	for( i = 0; i < MMI_SIM_NUMBER; i++)
	{
		HideStatusIcon(STATUS_ICON_REJECTED_CALL + i);
		if(CHISTGetRejectedCallCountBeforeView(i) != 0 
			#ifdef __MMI_MULTI_SIM__
			&& MTPNP_PFAL_Is_Card_Usable(i)
			#endif
			)
		{
			ShowStatusIcon(STATUS_ICON_REJECTED_CALL + i);
		}
	}
	UpdateStatusIcons();
}

#ifndef __MMI_CH_MIXED_CALL_LOG__

/**************************************************************

	FUNCTION NAME		: CHISTGetCallLogBySENDKey

  	PURPOSE				: Reads call log data from idle screen by send key

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTGetCallLogBySENDKey(void)
{
	if (chis_p->isReqSent == TRUE)
		return;

	if (chis_p->process == CALL_LOG_PROC_WRITE)
	{
		PRINT_INFORMATION(("still writing, wait till write complete then read!!!!"));
		chis_p->process = CALL_LOG_PROC_SYNC;
		if(!isInCall())
		{
		    DisplayIdleScreen();    
		}
		return;
	}
   CHISTResetCallLogIndex();
   ReqReadDialedNumByKey();
   return;
}

#else

/**************************************************************

	FUNCTION NAME		: EntryCHISTViewMixedCallList

  	PURPOSE				: Display mixed call log list screen
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTViewMixedCallList(void)
{
   U8*	phoneNumbers[TOT_SIZE_OF_MIXED_LIST];
	U8*	phoneNames[TOT_SIZE_OF_MIXED_LIST];
	U8*	guiBuffer;
	U16	Cat23DemoIcons[TOT_SIZE_OF_MIXED_LIST];
	U8    count;
	U16 title_str;
	U8 nSuited = 0;

	if (isInCall())
		return;

	if ( chis_p->nMixedCalls )
	{
		mmi_chist_rearrange_missed_call_status_icon();	
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	if(call_log_on_idle)
	{
		set_pre_tab_pane_display();		
	}
#endif
	
		EntryNewScreen(SCR8057_CALLS_LIST_MIXED, wgui_restore_list_menu_slim_style,
	               CHISTGetCallLogBySENDKey, NULL);

		guiBuffer = GetCurrGuiBuffer( SCR8057_CALLS_LIST_MIXED );
		RegisterHighlightHandler( CHISTGetCurrMixedCallIndex );
		CHISTPopulateMixedCallList( phoneNumbers, phoneNames );

		#ifdef __MMI_VIP_FUNCTION__
		nSuited = g_vip_context.nCallLogSuited;
		if(chis_p->nMixedCalls == nSuited)
		{
			DisplayPopup((PU8)GetString(STR_EMPTY_LIST_NOTIFICATION) ,
						IMG_GLOBAL_EMPTY, 1,
						CS_NOTIFYDURATION, EMPTY_LIST_TONE);
			DeleteScreenIfPresent(SCR8057_CALLS_LIST_MIXED);
			#ifdef __MMI_MULTI_SIM__
			DeleteScreenIfPresent(SCR_MTPNP_CALL_HISTORY_NOTIFY);
			#endif
			return;
		}
		#endif

		if( chis_p->nMissedCallBeforeView != 0 )
		{
			ClearSoundWarnningTimer();
			chis_p->nMissedCallBeforeView = 0;
		#ifdef __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__
			mmi_chist_set_num_missed_call_before_view();
		#endif

		#ifdef __MMI_MULTI_SIM__
			HideStatusIcon( STATUS_ICON_MISSED_CALL + chis_p->nSimId);
		//	UpdateStatusIcons();     delete it for not freash screen when entry call history
		#endif
		}
		
		for( count = 0; count < chis_p->nMixedCalls; count++)
		{
		   if (chis_p->mixedCalls[count] >= &chis_p->missedCalls[0] &&
		       chis_p->mixedCalls[count] <=
		            &chis_p->missedCalls[TOT_SIZE_OF_MISSED_LIST-1])
			{
			#ifdef __MMI_MULTI_SIM__
				#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
				Cat23DemoIcons[count] = IMG_MENU8103_MISSED_CALLS;
				#else
				Cat23DemoIcons[count] = IMG_MTPNP_CARD1_MENU_MISSED_CALLS + chis_p->nSimId;
				#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
			#else /* __MMI_MULTI_SIM__ */
				Cat23DemoIcons[count] = IMG_MENU8103_MISSED_CALLS;
			#endif /* __MMI_MULTI_SIM__ */
			}
		   else if (chis_p->mixedCalls[count] >= &chis_p->dialedCalls[0] &&
		            chis_p->mixedCalls[count] <=
		                  &chis_p->dialedCalls[TOT_SIZE_OF_DIALED_LIST-1])
			{
			#ifdef __MMI_MULTI_SIM__
				#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
				Cat23DemoIcons[count] = IMG_MENU8102_DIALED_CALLS;
				#else
				Cat23DemoIcons[count] = IMG_MTPNP_CARD1_MENU_DIALED_CALLS + chis_p->nSimId;
				#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
			#else /* __MMI_MULTI_SIM__ */
				Cat23DemoIcons[count] = IMG_MENU8102_DIALED_CALLS;
			#endif /* __MMI_MULTI_SIM__ */
			}
		   else
			{
			#ifdef __MMI_MULTI_SIM__
				#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
				Cat23DemoIcons[count] = IMG_MENU8101_RECEIVED_CALLS;
				#else
				Cat23DemoIcons[count] = IMG_MTPNP_CARD1_MENU_RECEIVED_CALLS+ chis_p->nSimId;
				#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
			#else /* __MMI_MULTI_SIM__ */
				Cat23DemoIcons[count] = IMG_MENU8101_RECEIVED_CALLS;
			#endif /* __MMI_MULTI_SIM__ */
			}
		}
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	title_str = STRING_MTPNP_CARD1_CALL_HISTORY + chis_p->nSimId;	
#else
	title_str = STR_SCR8057_CAPTION;
#endif /* defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_) */

		wgui_override_list_menu_slim_style(WGUI_LIST_MENU_SLIM_STYLE_DRAW_ICON);

#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		ShowCategory53Screen( title_str, IMG_SCR8057_CAPTION,
		                      STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK,
		                      IMG_GLOBAL_BACK, chis_p->nMixedCalls - nSuited, phoneNames,
		                      (U16*)Cat23DemoIcons,phoneNumbers,0,
		                      chis_p->currMixedCallIndex,guiBuffer );
#else
		ShowCategory53Screen( title_str, IMG_SCR8057_CAPTION,
		                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
		                      IMG_GLOBAL_BACK, chis_p->nMixedCalls - nSuited, phoneNames,
		                      (U16*)Cat23DemoIcons,phoneNumbers,0,
		                      chis_p->currMixedCallIndex,guiBuffer );
#endif

#ifdef __MMI_IP_KEY__
/* under construction !*/
#endif

		SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
		SetKeyHandler( EntryCHISTMixedListOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetLeftSoftkeyFunction( EntryCHISTMixedListOptions, KEY_EVENT_UP );
#else
		SetKeyHandler( EntryCHISTViewMixedCall, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetLeftSoftkeyFunction( EntryCHISTViewMixedCall, KEY_EVENT_UP );
#endif
		SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#ifdef __MMI_MULTI_SIM__
	    MTPNP_PFAL_CC_HandleSendKeys(CHISTCallMixedNum,KEY_EVENT_DOWN);
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
	SetKeyHandler(EntryCHISTMixedListOptions,KEY_ENTER,KEY_EVENT_UP );
#else
	SetKeyHandler(EntryCHISTViewMixedCall,KEY_ENTER,KEY_EVENT_UP );
#endif

#else
	    SetKeyHandler(CHISTCallMixedNum, KEY_SEND, KEY_EVENT_DOWN);
#endif

#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__	
	if(call_log_on_idle)
	{
			set_post_tab_pane_display();
	}
#endif
   }
   else
   {
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__
	if(call_log_on_idle)
	{
		set_pre_tab_pane_display();
		EntryNewScreen(SCR8057_CALLS_LIST_MIXED, NULL,
	        CHISTGetCallLogBySENDKey, NULL);
		guiBuffer = GetCurrGuiBuffer( SCR8057_CALLS_LIST_MIXED );
		//phoneNames[0]=(PU8)GetString(STR_EMPTY_LIST_NOTIFICATION);
		//Cat23DemoIcons[0]=0;
		ShowCategory53Screen( STR_SCR8057_CAPTION, IMG_SCR8057_CAPTION,
								    0, 0, STR_GLOBAL_BACK,IMG_GLOBAL_BACK, 
								    chis_p->nMixedCalls - nSuited, NULL,NULL,NULL,0,0, guiBuffer );
		/*ShowCategory56Screen( STR_SCR8057_CAPTION, IMG_SCR8057_CAPTION,
		               0,0, STR_GLOBAL_BACK,
		               IMG_GLOBAL_BACK, 1, phoneNames,
		               NULL,
		               0, guiBuffer );*/
		SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
		set_post_tab_pane_display();
	}
	else
#endif
	{
		DisplayPopup((PU8)GetString(STR_EMPTY_LIST_NOTIFICATION) ,
		             IMG_GLOBAL_EMPTY, 1,
		             CS_NOTIFYDURATION, EMPTY_LIST_TONE);
	}
   }
#ifdef __MMI_MULTI_SIM__
	DeleteScreenIfPresent(SCR_MTPNP_CALL_HISTORY_NOTIFY);
#endif

	return;
}
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
/**************************************************************

	FUNCTION NAME		: EntryCHISTMixedListOptions

  	PURPOSE				: Display a option list of call log from mixed call list

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTMixedListOptions( void )
{
   if (chis_p->mixedCalls[chis_p->currMixedCallIndex] >=
            &chis_p->missedCalls[0] &&
		 chis_p->mixedCalls[chis_p->currMixedCallIndex] <=
		      &chis_p->missedCalls[TOT_SIZE_OF_MISSED_LIST-1])
   {
      chis_p->currMissedCallIndex = (U8)
         (((U32)chis_p->mixedCalls[chis_p->currMixedCallIndex] -
          (U32)&chis_p->missedCalls[0])/sizeof(MISSED_CALL));
		EntryCHISTMissedListOptions();
	}
	else if (chis_p->mixedCalls[chis_p->currMixedCallIndex] >=
	            &chis_p->dialedCalls[0] &&
		      chis_p->mixedCalls[chis_p->currMixedCallIndex] <=
		         &chis_p->dialedCalls[TOT_SIZE_OF_DIALED_LIST-1])
   {
      chis_p->currDialedCallIndex = (U8)
         (((U32)chis_p->mixedCalls[chis_p->currMixedCallIndex] -
          (U32)&chis_p->dialedCalls[0])/sizeof(DIALED_CALL));
		EntryCHISTDialedListOptions();
	}
	else
	{
	   chis_p->currRecvdCallIndex = (U8)
         (((U32)chis_p->mixedCalls[chis_p->currMixedCallIndex] -
          (U32)&chis_p->recvdCalls[0])/sizeof(RECEIVED_CALL));
		EntryCHISTRecvdListOptions();
	}
	return;
}
#else
/**************************************************************

	FUNCTION NAME		: EntryCHISTViewMixedCall

  	PURPOSE				: Display a single entry of call log from mixed call list
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTViewMixedCall( void )
{
   if (chis_p->mixedCalls[chis_p->currMixedCallIndex] >=
            &chis_p->missedCalls[0] &&
		 chis_p->mixedCalls[chis_p->currMixedCallIndex] <=
		      &chis_p->missedCalls[TOT_SIZE_OF_MISSED_LIST-1])
   {
      chis_p->currMissedCallIndex = (U8)
         (((U32)chis_p->mixedCalls[chis_p->currMixedCallIndex] -
          (U32)&chis_p->missedCalls[0])/sizeof(MISSED_CALL));
		EntryCHISTViewMissedCall();
	}
	else if (chis_p->mixedCalls[chis_p->currMixedCallIndex] >=
	            &chis_p->dialedCalls[0] &&
		      chis_p->mixedCalls[chis_p->currMixedCallIndex] <=
		         &chis_p->dialedCalls[TOT_SIZE_OF_DIALED_LIST-1])
   {
      chis_p->currDialedCallIndex = (U8)
         (((U32)chis_p->mixedCalls[chis_p->currMixedCallIndex] -
          (U32)&chis_p->dialedCalls[0])/sizeof(DIALED_CALL));
		EntryCHISTViewDialedCall();
	}
	else
	{
	   chis_p->currRecvdCallIndex = (U8)
         (((U32)chis_p->mixedCalls[chis_p->currMixedCallIndex] -
          (U32)&chis_p->recvdCalls[0])/sizeof(RECEIVED_CALL));
		EntryCHISTViewRecvdCall();
	}
	return;
}
#endif
#ifdef __IP_NUMBER__
/**************************************************************

	FUNCTION NAME		: CHISTCallMixedNumByIP

  	PURPOSE				: Use IP dialing to call a single entry in mixed call list
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTCallMixedNumByIP(void)
{
	SetTempUseIPNumber(TRUE);
	CHISTCallMixedNum();
}
#endif

/**************************************************************

	FUNCTION NAME		: CHISTCallMixedNum

  	PURPOSE				: make call to a single entry in mixed call list
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTCallMixedNum( void )
{
	addToHistory = 0;

	if( chis_p->mixedCalls[chis_p->currMixedCallIndex]->number[0] == 0 )
	{
		if( chis_p->mixedCalls[chis_p->currMixedCallIndex]->pbName[0] != 0)
		{
		   chis_p->dialFromCallLog = ITEM_MIXED_CALL;
			MakeCall(
			   (S8 *)chis_p->mixedCalls[chis_p->currMixedCallIndex]->pbName );
			chis_p->currMixedCallIndex = 0;
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_NONUMBER_TO_DIAL),
			             IMG_GLOBAL_UNFINISHED, 1,
			             CS_NOTIFYDURATION,ERROR_TONE);
		}
	}
	else
	{
	   chis_p->dialFromCallLog = ITEM_MIXED_CALL;
		MakeCall(
		   (S8 *)chis_p->mixedCalls[chis_p->currMixedCallIndex]->number );
		chis_p->currMixedCallIndex = 0;
	}
	DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTGetCallLogBySENDKey

  	PURPOSE				: Reads mixed call logs from idle screen with send key
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTGetCallLogBySENDKey(void)
{
	if (chis_p->isReqSent == TRUE)
		return;
	#ifdef __CALL_LOG_VIEW_SWITCH_BY_ARROW_KEY__
	CHISViewIndex = 0;
	#endif
//   CHISTResetCallLogIndex();  //delete for bug 14535
#ifdef __MMI_MULTI_SIM__
	mmi_trace(g_sw_CALLLOG,"MMI_Calllog: Func: %s SIM: %d", __FUNCTION__, chis_p->nSimId);
#endif	
	mmi_trace(g_sw_CALLLOG,"MMI_Calllog: Func: %s ,chis_p->isCallLogReady=%d,chis_p->isFirstTime=%d", __FUNCTION__,chis_p->isCallLogReady,chis_p->isFirstTime);
   if (chis_p->isCallLogReady == TRUE && chis_p->isFirstTime == FALSE)
	{
		if (chis_p->process == CALL_LOG_PROC_WRITE)
		{
			PRINT_INFORMATION(("still writing, wait till write complete then read!!!!"));
			chis_p->process = CALL_LOG_PROC_SYNC;
			if(!isInCall())
			{
			    DisplayIdleScreen();    
			}
			return;
		}
	   chis_p->isMixedStyle = TRUE;
	   ReqReadDialedNumByKey();
   }
	else
   {
	   EntryNewScreen(SCR_CALL_LOG_NOT_READY, ExitCHISTProcessingAnimation, NULL, NULL);

	   StartTimer (PHB_NOTIFYDURATION_TIMER, PHB_NOTIFY_TIMEOUT,  CHISTAnimationTimeOutHdlr);

	   ShowCategory66Screen(STR_SCR8057_CAPTION, IMG_SCR8057_CAPTION,
	                        0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                        (PU8)GetString(STR_CALLS_IN_PROGRESS_TEXT),
	                        IMG_PROCESSING_PHONEBOOK, NULL);

	   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTPopulateMixedCallList

  	PURPOSE				: Construct mixed call list
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTPopulateMixedCallList( U8* numberList[], U8* phoneList[] )
{
	U16	index	= 0;

	U16 nSuited = 0;
	#ifdef __MMI_VIP_FUNCTION__
	U16 i = 0;
	#endif
	
	for ( index = 0; index < chis_p->nMixedCalls; index++ )
	{
		if ( chis_p->mixedCalls[index]->nameLength > 0)
		{
		#ifdef __MMI_VIP_FUNCTION__
			if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
			{
				nSuited++;
			}
			else
		#endif
			{
				phoneList[index - nSuited] = (U8 *)chis_p->mixedCalls[index]->pbName;
				numberList[index - nSuited] = (U8 *)chis_p->mixedCalls[index]->number;
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
		}
		else
		{
			if(chis_p->mixedCalls[index]->number[0] != 0)
			{
			#ifdef __MMI_VIP_FUNCTION__
				if(mmi_VIP_is_vip_number((U8 *)chis_p->dialedCalls[index].number))
				{
					nSuited++;
				}
				else
			#endif
				{
					phoneList[index - nSuited] = (U8 *)chis_p->mixedCalls[index]->number;
					#ifdef __MMI_VIP_FUNCTION__
					g_vip_context.CallLogIndexMap[i++] = index;
					#endif
				}
			}
			else
			{
				phoneList[index - nSuited] = (PU8)GetString(STR_UNKNOWN_NUMBER);
				#ifdef __MMI_VIP_FUNCTION__
				g_vip_context.CallLogIndexMap[i++] = index;
				#endif
			}
			numberList[index] = NULL;
		}
	}
	#ifdef __MMI_VIP_FUNCTION__
	g_vip_context.nCallLogSuited = nSuited;
	#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTGetCurrMixedCallIndex

  	PURPOSE				: Sets current highlighted index in mixed call list
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTGetCurrMixedCallIndex( S32 ind )
{
	chis_p->currMixedCallIndex = (U8)ind;
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTSortCallLogsByTime

  	PURPOSE				: Sort all three types of call logs by time
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTSortCallLogsByTime(void)
{
   U8 index=0, index1=0, index2=0, index3=0;
   U8 total_logs;

   total_logs = chis_p->nDialedCalls + chis_p->nMissedCalls +
                chis_p->nRecvdCalls;
   chis_p->nMixedCalls = 0;

   while (total_logs > 0)
   {
      if (index1 >= chis_p->nDialedCalls)
      {
         /* no more dialed */
         if (index2 >= chis_p->nMissedCalls)
         {
            /* no more missed */
            if (index3 >= chis_p->nRecvdCalls)
            {
               /* all three types are done, no need to sort */
               break;
            }
            else
            {
               /* only Recvd remains, recvd wins */
               chis_p->mixedCalls[index] = &chis_p->recvdCalls[index3];
               index3++;
            }
         }
         else
         {
            if (index3 >= chis_p->nRecvdCalls)
            {
               /* only missed remains, missed wins */
               chis_p->mixedCalls[index] = &chis_p->missedCalls[index2];
               index2++;
            }
            else
            {
               /* missed and recvd remain, compare */
               if (CompareTime(chis_p->missedCalls[index2].startTime,
                         chis_p->recvdCalls[index3].startTime, NULL) == TIME_LESS)
               {
                  // recvd wins
                  chis_p->mixedCalls[index] = &chis_p->recvdCalls[index3];
                  index3++;
               }
               else
               {
                  // missed wins
                  chis_p->mixedCalls[index] = &chis_p->missedCalls[index2];
                  index2++;
               }
            }
         }
      }
      else
      {
         /* dialed remains */
         if (index2 >= chis_p->nMissedCalls)
         {
            /* no more missed */
            if (index3 >= chis_p->nRecvdCalls)
            {
               /* only dialed remains, dialed wins */
               chis_p->mixedCalls[index] = &chis_p->dialedCalls[index1];
               index1++;
            }
            else
            {
               /* dialed and recvd remains, compare */
               if (CompareTime(chis_p->dialedCalls[index1].startTime,
                            chis_p->recvdCalls[index3].startTime, NULL) == TIME_LESS)
               {
                  // recvd wins
                  chis_p->mixedCalls[index] = &chis_p->recvdCalls[index3];
                  index3++;
               }
               else
               {
                  // dialed wins
                  chis_p->mixedCalls[index] = &chis_p->dialedCalls[index1];
                  index1++;
               }
            }
         }
         else
         {
            /* missed remains */
            if (index3 >= chis_p->nRecvdCalls)
            {
               /* dialed and missed remains, compare */
               if (CompareTime(chis_p->dialedCalls[index1].startTime,
                      chis_p->missedCalls[index2].startTime, NULL) == TIME_LESS)
               {
                  // missed wins
                  chis_p->mixedCalls[index] = &chis_p->missedCalls[index2];
                  index2++;
               }
               else
               {
                  // dialed wins
                  chis_p->mixedCalls[index] = &chis_p->dialedCalls[index1];
                  index1++;
               }
            }
            else
            {
               /* all remain, compare */
      if (CompareTime(chis_p->dialedCalls[index1].startTime,
                      chis_p->missedCalls[index2].startTime, NULL) == TIME_LESS)
      {
         if (CompareTime(chis_p->missedCalls[index2].startTime,
                         chis_p->recvdCalls[index3].startTime, NULL) == TIME_LESS)
         {
            // recvd wins
            chis_p->mixedCalls[index] = &chis_p->recvdCalls[index3];
            index3++;
         }
         else
         {
            // missed wins
            chis_p->mixedCalls[index] = &chis_p->missedCalls[index2];
            index2++;
         }
      }
      else
      {
         if (CompareTime(chis_p->missedCalls[index2].startTime,
                         chis_p->recvdCalls[index3].startTime, NULL) == TIME_LESS)
         {
            if (CompareTime(chis_p->dialedCalls[index1].startTime,
                            chis_p->recvdCalls[index3].startTime, NULL) == TIME_LESS)
            {
               // recvd wins
               chis_p->mixedCalls[index] = &chis_p->recvdCalls[index3];
               index3++;
            }
            else
            {
               // dialed wins
               chis_p->mixedCalls[index] = &chis_p->dialedCalls[index1];
               index1++;
            }
         }
         else
         {
            // dialed wins
            chis_p->mixedCalls[index] = &chis_p->dialedCalls[index1];
            index1++;
         }
      }
            }
         }
      }
      
      total_logs--;
      index++;
   }
	
   for (chis_p->nMixedCalls=0; chis_p->nMixedCalls<index; chis_p->nMixedCalls++)
   {
      if (CHISTIsDuplicatedNumber(chis_p->nMixedCalls) == TRUE)
      {
         if (chis_p->nMixedCalls+1 < index)
         {
            /* shift up */
            memcpy (&chis_p->mixedCalls[chis_p->nMixedCalls],
                    &chis_p->mixedCalls[chis_p->nMixedCalls+1],
                    4*(index - chis_p->nMixedCalls - 1));
            chis_p->mixedCalls[index-1] = NULL;
            chis_p->nMixedCalls--;
         }
         else
            chis_p->mixedCalls[index-1] = NULL;
         index--;
      }
   }
   chis_p->nMixedCalls = index;
   return;
}

/**************************************************************

	FUNCTION NAME		: CHISTIsDuplicatedNumber

  	PURPOSE				: Check if there's a dusplicated number in all three 
  							  types of call logs
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
BOOL CHISTIsDuplicatedNumber(U8 index)
{
   U8 i;
   U32 temp1, temp2;

   if (index == 0)
      return FALSE;

   for (i=0; i<index; i++)
   {
      U8 ascii_num1[MAX_DIGIT];
		U8 ascii_num2[MAX_DIGIT];

      /* only match last 7 digits */
      UnicodeToAnsii((PS8)ascii_num1, (PS8)chis_p->mixedCalls[i]->number);
      UnicodeToAnsii((PS8)ascii_num2, (PS8)chis_p->mixedCalls[index]->number);
	
      temp1 = mmi_phb_util_convert_number_to_int((PU8)ascii_num1);
      temp2 = mmi_phb_util_convert_number_to_int((PU8)ascii_num2);
      if (temp1 == temp2)
		{
#ifdef __MMI_PHB_ADV_NUM_MATCH_MOBILE__
			if (mmi_phb_number_compare_extension((PS8)ascii_num1, (PS8)ascii_num2) == FALSE)
				return FALSE;
#endif			
			if (temp1 != INVALID_NUMBER &&
         			pfnUnicodeStrcmp((PS8)chis_p->mixedCalls[i]->number, 
            					(PS8)chis_p->mixedCalls[index]->number) == 0)
    {
        if((chis_p->mixedCalls[i]->number >= &chis_p->missedCalls[0] &&
		       chis_p->mixedCalls[i]->number <=
		            &chis_p->missedCalls[TOT_SIZE_OF_MISSED_LIST-1])
		            &&
		         (chis_p->mixedCalls[index]->number >= &chis_p->missedCalls[0] &&
		       chis_p->mixedCalls[index]->number <=
		            &chis_p->missedCalls[TOT_SIZE_OF_MISSED_LIST-1]))
    
				return TRUE;

        if((chis_p->mixedCalls[i]->number >= &chis_p->dialedCalls[0] &&
		       chis_p->mixedCalls[i]->number <=
		            &chis_p->dialedCalls[TOT_SIZE_OF_MISSED_LIST-1])
		            &&
		         (chis_p->mixedCalls[index]->number >= &chis_p->dialedCalls[0] &&
		       chis_p->mixedCalls[index]->number <=
		            &chis_p->dialedCalls[TOT_SIZE_OF_MISSED_LIST-1]))
    
				return TRUE;
        
        if((chis_p->mixedCalls[i]->number >= &chis_p->recvdCalls[0] &&
		       chis_p->mixedCalls[i]->number <=
		            &chis_p->recvdCalls[TOT_SIZE_OF_MISSED_LIST-1])
		            &&
		         (chis_p->mixedCalls[index]->number >= &chis_p->recvdCalls[0] &&
		       chis_p->mixedCalls[index]->number <=
		            &chis_p->recvdCalls[TOT_SIZE_OF_MISSED_LIST-1]))
    
				return TRUE;        
             }
		}

   }
   return FALSE;
}

#endif

/**************************************************************

	FUNCTION NAME		: CHISTIgnoreMissedCallScreen

  	PURPOSE				: Exit the missed call notification screen
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTIgnoreMissedCallScreen(void)
{
	/*After Ignore Missed call,set the flag as False-for fixing 12867 by liuxl on 20090629 */	
	#ifndef __MMI_MULTI_SIM__
    		chis_p->saveMissedScrn = FALSE;
	#else  /*__MMI_MULTI_SIM__*/
   		 SetMissedCallFlag(FALSE);
	#endif  /*__MMI_MULTI_SIM__*/
	//   GoBackBothLCDHistory();
	EntryIdleScreen();
}

/**************************************************************

	FUNCTION NAME		: CHISTReadMissedNum

  	PURPOSE				: Reads missed call log from missed call notification screen
  	
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTReadMissedNum (void)
{
   GoBackSubLCDHistory();
   ReqReadMissedNum();
}

/**************************************************************

	FUNCTION NAME		: ConvertRTC2MYTIME

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ConvertRTC2MYTIME(MYTIME * time, CALL_LOG_RTCTIMEFORMAT * rtc)
{
	time->nYear		= (rtc->rtc_year)+YEARFORMATE;
	time->nMonth	= rtc->rtc_mon ;
	time->nDay		= rtc->rtc_day ;
	time->nHour		= rtc->rtc_hour;
	time->nMin		= rtc->rtc_min ;
	time->nSec		= rtc->rtc_sec ;
	time->DayIndex	= rtc->rtc_wday;
	return;
}

/**************************************************************

	FUNCTION NAME		: ConvertMYTIME2RTC

  	PURPOSE				:

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ConvertMYTIME2RTC(CALL_LOG_RTCTIMEFORMAT * rtc, MYTIME * time)
{
	rtc->rtc_year	= (U8)((time->nYear)-YEARFORMATE);
	rtc->rtc_mon 	= time->nMonth;
	rtc->rtc_day 	= time->nDay;
	rtc->rtc_hour	= time->nHour;
	rtc->rtc_min 	= time->nMin;
	rtc->rtc_sec 	= time->nSec;
	rtc->rtc_wday	= time->DayIndex;
	return;
}


#ifdef __MMI_BT_SUPPORT__
extern void rda_bt_hfg_send_BldnFail_ind(void);


void rdabt_read_log(void)
{

	U8 i;
	if(GetCurrentState()==	CM_OUTGOING_STATE)
	{
		mmi_trace(1,"f(GetCurrentState()==	CM_OUTGOING_STATE)");
		mmi_trace(1,"f(GetCurrentState()==	CM_OUTGOING_STATE)");
		rda_bt_hfg_send_BldnFail_ind();
		return ;
	}

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		#ifdef __MMI_MULTI_SIM__
		if(MTPNP_PFAL_Is_Card_Usable(i))
		#endif
		{
			#ifdef __MMI_MULTI_SIM__
			SetCHISContext(i);
			#endif
			chis_p->isFirstTime  = TRUE;
			ReqReadDialedNum();
		}
	}

}


void rdabt_hfg_bldn_process(void)
{
 //   kal_uint8 index=0;
	OUTGOING_CALL outCall;
	U8 nExist = 0;
	BOOL bExist[MMI_SIM_NUMBER];
	S16 i;
	S16 startIndex = -1;
	
	mmi_trace(1,"rdabt_hfg_bldn_process");
	memset (&outCall, 0, sizeof (OUTGOING_CALL));
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		mmi_trace(1,"rdabt_hfg_bldn_process g_CallHistory_cntxt[%d].nDialedCalls=%d", i, g_CallHistory_cntxt[i].nDialedCalls);
		if(g_CallHistory_cntxt[i].nDialedCalls==0)
		{
			bExist[i] = MMI_FALSE;
		}
		else
		{
			bExist[i] = MMI_TRUE;
			nExist++;
			if(startIndex == -1)
			{
				startIndex = i;
			}
		}
	}
	if(nExist == 0)
	{
		rda_bt_hfg_send_BldnFail_ind();
		mmi_trace(1,"no dialed history return");
		return;
	}

	for(i = startIndex + 1; i < MMI_SIM_NUMBER; i ++)
	{
		if(bExist[i])
		{
			if (CompareTime(g_CallHistory_cntxt[startIndex].dialedCalls[0].startTime, g_CallHistory_cntxt[i].dialedCalls[0].startTime, NULL) == TIME_LESS)
			{
				startIndex = i;
			}
		}
	}

	pfnUnicodeStrcpy((void*)outCall.Number,(void*)g_CallHistory_cntxt[startIndex].dialedCalls[0].number);
	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, startIndex);
	#endif

	MakeCall((PS8) outCall.Number);
}


#endif //__MMI_BT_SUPPORT__
#endif


