/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2009, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*       dual_sim_calllog.c                                                           */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   MTPNP layer soruce file, includes the implementation of MTPNP Call Log APIs.      */
/*                                                                            */
/******************************************************************************/

#include "mmi_features.h"
#include "globaldefs.h"
#include "globalscrenum.h"
#include "frameworkstruct.h"
#include "gui_data_types.h"
#include "eventsgprot.h"
#include "mmi_msg_struct.h"
#include "simdetectiondef.h"
#include "simdetectionhwmapfile.h"
#include "debuginitdef.h"
#include "wgui_status_icons.h"

#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "debuginitdef.h"	/*debug info*/
#include "protocolevents.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_cm.h"
#include "wgui_categories_inputs.h"
#include "wgui_status_icons.h"
#include "allappgprot.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebooktypes.h"

#include "callsdefs.h"
#include "settingstructs.h"
#include "settinggprots.h"
#include "simdetectiongprot.h"
#include "protocolevents.h"
#include "callfixeddial.h"
#include "barreddiallist.h"
#include "settingprofile.h"
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
#include "simdetectiongexdcl.h"
#include "settingsgexdcl.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "securitysetup.h"
#include "callmanagementgprot.h"
#include "callhistorymain.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_calllog.h"
#include "protocolevents.h"
#include "smsapi.h"
#include "smsstruct.h"
#include "wgui_status_icons.h"
#include "smsguiinterfacetype.h"
#include "mmi_msg_context.h"
#include"historygprot.h"
#include "smsguiinterfaceprot.h"
#include "smsfunc.h"
#include "phonebookgprot.h"
#include "commonscreens.h"
#include "settingprofile.h"

#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"

#include "mtpnp_ad_resdef.h"
#include "messagesresourcedata.h"

#include "incomingcallmanagementprot.h"
#include "mainmenudef.h"
#include "callhistoryenum.h"
#include "callsstruct.h"
#include "callhistorygprots.h"
#include "callslog.h"
#include "sublcdhistorygprot.h"
#include "servicedefs.h"
extern call_history_context_struct g_CallHistory_cntxt[];
extern U8 addToHistory;
extern U16   gKeyPadLockFlag;


U8 gPrevSimId =  0xff;
////////////////////////////////////////////////////////
//Call Log
void SetCHISContext(U8 nSimId)
{
	if(nSimId >= MMI_SIM_NUMBER)
	{
		return;
	}
	chis_p = &g_CallHistory_cntxt[nSimId];
	chis_p->nSimId = nSimId;
	mmi_trace(g_sw_CALLLOG,"MMI_Calllog: Func: %s SIM: %d", __FUNCTION__,chis_p->nSimId);
		
}

void SetCHISToCall()
{
	gPrevSimId = chis_p->nSimId;
	SetCHISContext(MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
}

void RestoreCHISContext()
{
	if (gPrevSimId == 0xff)
	{
		return;
	}
	SetCHISContext(gPrevSimId);
	gPrevSimId = 0xff;
}
void SetSCHISReady(U8 nSimId, BOOL bReady)
{
	if(nSimId >= MMI_SIM_NUMBER)
	{
		return;
	}
	g_CallHistory_cntxt[nSimId].isCallLogReady = bReady;
}
BOOL GetSCHISReady(U8 nSimId )
{
	if(nSimId >= MMI_SIM_NUMBER)
	{
		return FALSE;
	}
	return g_CallHistory_cntxt[nSimId].isCallLogReady;
}

void EntryDMCHISTMainMenu(void)
{
#ifdef __MMI_BLACKBERRY_QWERTY__
	if (MTPNP_PFAL_Is_CardValid(0) == 0 && MTPNP_PFAL_Is_CardValid(1) == 0)
	{
		DisplayPopup( (PU8)GetString(SERVICES_NOT_PROVIDED_STRING_ID), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		return;
	};
#endif	
	MTPNP_AD_SetMenuHideUnhide(MAIN_MENU_CALL_HISTORY);
	mmi_mtpnp_entry_menu(SCR_MTPNP_DM_CALLHIS_MAIN_MENU, NULL, EntryDMCHISTMainMenu,
							MENU_MTPNP_DM_CALL_HISTORY, MAIN_MENU_CALL_HISTORY_TEXT, GetRootTitleIcon(MAIN_MENU_CALL_HISTORY));
}

void HighlightDMCHISTMainMenu(void)
{
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(EntryDMCHISTMainMenu, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void MTPNP_CHISSetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID)
{
	SetProtocolEventHandlerExt( funcPtr, eventID,chis_p->nSimId);
}

//#ifdef __SENDKEY2_SUPPORT__
void CHISTGetCallLogBySENDKey_SIM1()
{
	SetCHISContext(MMI_SIM_1);
	CHISTGetCallLogBySENDKey();
}
void CHISTGetCallLogBySENDKey_SIM2()
{
	SetCHISContext(MMI_SIM_2);
	CHISTGetCallLogBySENDKey();
}
void CHISTGetCallLogBySENDKey_SIM3()
{
	SetCHISContext(MMI_SIM_3);
	CHISTGetCallLogBySENDKey();
}
//#endif
void MTPNP_PFAL_ReadDualMixedCallLog(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer;                          /* Buffer holding history data */
	U16 nStrItemList[MAX_SUB_MENUS];        /* Stores the strings id of submenus returned */
	U16 nNumofItem;                         /* Stores no of children in the submenu */
	U16 ItemIcons[MAX_SUB_MENUS];

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	if(MTPNP_AD_Get_UsableSide_Number() == 1)
	{	
		SetCHISContext(MTPNP_AD_GET_UsableSide_Index());
		CHISTGetCallLogBySENDKey();
		return;
	}

	EntryNewScreen(SCR_ID_CHIST_DUAL_CARD_MIXED_CALLLOG, NULL, MTPNP_PFAL_ReadDualMixedCallLog, NULL);

	/* Get current screen to gui buffer  for history purposes */
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CHIST_DUAL_CARD_MIXED_CALLLOG);
	MTPNP_AD_SetMenuHideUnhide(MENU_ID_CHIST_DUAL_CARD_MIXED_CALL_LOG_MASTER);
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU_ID_CHIST_DUAL_CARD_MIXED_CALL_LOG); 
	/* Retrieve string/image ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU_ID_CHIST_DUAL_CARD_MIXED_CALL_LOG, nStrItemList);     
	GetSequenceImageIds_Ext(MENU_ID_CHIST_DUAL_CARD_MIXED_CALL_LOG, ItemIcons); 
	/* Set current parent id */
	SetParentHandler(MENU_ID_CHIST_DUAL_CARD_MIXED_CALL_LOG);

	RegisterHighlightHandler(MultiSimMenuHighLightHandler);

	ShowCategory52Screen(
						MAIN_MENU_CALL_HISTORY_TEXT,
						GetRootTitleIcon(MAIN_MENU_CALL_HISTORY),
						STR_GLOBAL_OK,
						IMG_GLOBAL_OK,
						STR_GLOBAL_BACK,
						IMG_GLOBAL_BACK,
						nNumofItem,
						nStrItemList,
						(U16 *)gIndexIconsImageList,
						0,
						0,
						0,  /* u16callhist_curindex, */
						guiBuffer);
	return;
}

void HighlightCHISTDualMixedCallLog(void)
{
	U8 reqSrcMod = GetReqSrcModIndex();
	SetCHISContext(reqSrcMod);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(CHISTGetCallLogBySENDKey, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(CHISTGetCallLogBySENDKey, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
void MTPNP_PFAL_Call_Rejected(void)
{
    addToHistory = 0;

	if( chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number[0] == 0 )
	{
		if( chis_p->rejectedCalls[chis_p->currRejectedCallIndex].pbName[0] != 0 )
		{
		   chis_p->dialFromCallLog = ITEM_REJECTED_CALL;
                mmi_ucm_app_entry_dial_option((U8 *) chis_p->rejectedCalls[chis_p->currRejectedCallIndex].pbName, NULL, NULL);
        }
        else
        {
            DisplayPopup((PU8) GetString(STR_NONUMBER_TO_DIAL), IMG_GLOBAL_UNFINISHED, 1, CS_NOTIFYDURATION,
                         ERROR_TONE);
        }
    }
    else
    {
	   chis_p->dialFromCallLog = ITEM_REJECTED_CALL;
            mmi_ucm_app_entry_dial_option((U8*) chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number, NULL, NULL);
    }
#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (!IsScreenPresent(SCR8058_CALLS_LIST_REJECTED))
		DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
	else
#endif
	DeleteUptoScrID(SCR8058_CALLS_LIST_REJECTED);

}


/**************************************************************

	FUNCTION NAME		: CHISTCallMissedNum

  	PURPOSE				: To make a call to missed list entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MTPNP_PFAL_Call_Missed( void )
{
   addToHistory = 0;
   
	if( chis_p->missedCalls[chis_p->currMissedCallIndex].number[0] == 0 )
	{
		if( chis_p->missedCalls[chis_p->currMissedCallIndex].pbName[0] != 0 )
		{
		   chis_p->dialFromCallLog = ITEM_MISSED_CALL;
			mmi_ucm_app_entry_dial_option(
			   (S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].pbName, NULL, NULL);
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
		mmi_ucm_app_entry_dial_option(
		   (S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].number, NULL, NULL);
	}
#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (!IsScreenPresent(SCR8056_CALLS_LIST_MISSED))
		DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
	else
#endif
	DeleteUptoScrID(SCR8056_CALLS_LIST_MISSED);
	return;
}


/**************************************************************

	FUNCTION NAME		: CHISTCallRecvdNum

  	PURPOSE				: To make a call to recvd list entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MTPNP_PFAL_Call_Recved( void )
{
   addToHistory = 0;
   
	if( chis_p->recvdCalls[chis_p->currRecvdCallIndex].number[0] == 0 )
	{
		if( chis_p->recvdCalls[chis_p->currRecvdCallIndex].pbName[0] != 0 )
		{
		   chis_p->dialFromCallLog = ITEM_RECVD_CALL;
			mmi_ucm_app_entry_dial_option(
			   (S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].pbName, NULL, NULL);
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
	   mmi_ucm_app_entry_dial_option(
	      (S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number, NULL, NULL);
	}
#ifdef __MMI_CH_MIXED_CALL_LOG__
	if (!IsScreenPresent(SCR8054_CALLS_LIST_RECEIVED))
		DeleteUptoScrID(SCR8057_CALLS_LIST_MIXED);
	else
#endif
	DeleteUptoScrID(SCR8054_CALLS_LIST_RECEIVED);
	return;
}


/**************************************************************

	FUNCTION NAME		: CHISTCallDialedNum

  	PURPOSE				: To make a call to dialed list entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MTPNP_PFAL_Call_Dialed( void )
{

	addToHistory = 0;

	if( chis_p->dialedCalls[chis_p->currDialedCallIndex].number[0] == 0 )
	{
		/* no name, number is in name field for display */
		if( chis_p->dialedCalls[chis_p->currDialedCallIndex].pbName[0] != 0 )
		{
			chis_p->dialFromCallLog = ITEM_DIALED_CALL;
			mmi_ucm_app_entry_dial_option((S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].pbName, NULL, NULL);
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
		mmi_ucm_app_entry_dial_option( (S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].number, NULL, NULL );
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


MTPNP_UINT8 MTPNP_PFAL_chist_get_num_rejected_call_before_view(void)
{
    return g_CallHistory_cntxt[MMI_SIM_2].nRejectedCallBeforeView;
}

void CHISTMissedNotifyHandler(void)
{
	CHISTReadMissedNum();
	chis_p->currMissedCallIndex = 0;
	chis_p->nMissedCallBeforeView = 0;
}

void HighlightCHISTMissedNotify(void)
{
	U8 reqSrcMod = GetReqSrcModIndex();
	if (gKeyPadLockFlag == 0)
	{
		SetCHISContext(reqSrcMod);
		ChangeLeftSoftkey(MISSED_LSK_STR, IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
		SetLeftSoftkeyFunction(CHISTMissedNotifyHandler, KEY_EVENT_UP);
		SetRightSoftkeyFunction(CHISTIgnoreMissedCallScreen, KEY_EVENT_UP);
		SetKeyHandler(CHISTMissedNotifyHandler, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(CHISTIgnoreMissedCallScreen, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	}
	else
	{
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
		ClearKeyHandler(KEY_RSK, KEY_EVENT_UP);
	}
}

void HintCHISTMissedNotify(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];
	U8 reqSrcMod = GetReqSrcModeIndexByIndex(index);
	SetCHISContext(reqSrcMod);
	memset((PS8) hintData[index], 0, sizeof(hintData[index]));
	sprintf((S8 *) value, "%d ", chis_p->nMissedCallBeforeView);
	mmi_asc_to_ucs2((PS8) hintData[index], (PS8) value);
	mmi_ucs2ncat((S8 *) hintData[index], (S8 *) GetString(STR_MISSED_CALLS), MAX_GET_STRING_LEN);
}

/* below three function is service for master call history */
void MTPNP_PFAL_ReadDualModeMissedNum(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer;                          /* Buffer holding history data */
	U16 nStrItemList[MAX_SUB_MENUS];        /* Stores the strings id of submenus returned */
	U16 nNumofItem;                         /* Stores no of children in the submenu */
	U16 ItemIcons[MAX_SUB_MENUS];
	U8 *PopUpList[MAX_SUB_MENUS];
	U16 LSKStr;
	U16 LSKImg;
	U8 nNeedShowMenu = 0;
	U8 nSimIndex;
	U8 nNeedShowSimIndex = 0;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* Call Exit Handler */
	for(nSimIndex = 0; nSimIndex < MMI_SIM_NUMBER; nSimIndex++)
	{
		if(CHISTGetMissedCallCountBeforeView(nSimIndex) != 0 && MTPNP_PFAL_Is_Card_Usable(nSimIndex))
		{
			nNeedShowMenu++;
			nNeedShowSimIndex = nSimIndex;
		}
	}
	
	if(nNeedShowMenu > 1)
	{
		;//do nothing
	}
	else if(nNeedShowMenu == 1)
	{
		SetCHISContext(nNeedShowSimIndex);
		CHISTReadMissedNum();
		return;
	}
	else
	{
		SetMissedCallFlag(FALSE);
		EntryIdleScreen();
		return;
	}
	
	EntryNewScreen(SCR_MTPNP_CALL_HISTORY_NOTIFY, NULL, MTPNP_PFAL_ReadDualModeMissedNum, NULL);

	/* Get current screen to gui buffer  for history purposes */
	guiBuffer = GetCurrGuiBuffer(SCR_MTPNP_CALL_HISTORY_NOTIFY);
	MTPNP_AD_SetMenuHideUnhide(MENU_MTPNP_MISSED_CALL_NOTIFY_MASTER);
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU_MTPNP_MISSED_CALL_NOTIFY); 
	/* Retrieve string/image ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU_MTPNP_MISSED_CALL_NOTIFY, nStrItemList);     
	GetSequenceImageIds_Ext(MENU_MTPNP_MISSED_CALL_NOTIFY, ItemIcons); 
	/* Set current parent id */
	SetParentHandler(MENU_MTPNP_MISSED_CALL_NOTIFY);

	RegisterHighlightHandler(MultiSimMenuHighLightHandler);

	/* construct hint popup strings for menu items */
	ConstructHintsList(MENU_MTPNP_MISSED_CALL_NOTIFY, PopUpList);

	LSKStr = MISSED_LSK_STR;
	LSKImg = IMG_GLOBAL_OK;        
	if (gKeyPadLockFlag == 0)
	{
		ShowCategory52Screen(
						STRING_MTPNP_MISSED_CALL_NOTIFY,
						GetRootTitleIcon(MAIN_MENU_CALL_HISTORY),
						LSKStr,
						LSKImg,
						STR_GLOBAL_BACK,
						IMG_GLOBAL_BACK,
						nNumofItem,
						nStrItemList,
						/*ItemIcons*/(U16 *)gIndexIconsImageList,
						(U8 **)PopUpList,
						0,
						0,  /* u16callhist_curindex, */
						guiBuffer);
						SetKeyHandler(CHISTIgnoreMissedCallScreen, KEY_END, KEY_EVENT_DOWN);    
	}

	ForceSubLCDScreen(EntrySubCHISTNotifyMissedCall);
	return;
}

///////////////////////////////////////////////////////////////
#endif

