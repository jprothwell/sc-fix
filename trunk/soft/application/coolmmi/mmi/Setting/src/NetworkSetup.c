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
 * NetworkSetup.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#define __NEWSIMULATOR 

#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"
#include "csp.h"
#include "frameworkstruct.h"
#include "networksetupdefs.h"
#include "networksetupgprot.h"
#include "debuginitdef.h"
#include "mainmenudef.h"
#include "protocolevents.h"
#include "settingprofile.h"
#include "settingdefs.h"
#include "prefnwksgprot.h"
#include "commonscreens.h"
#include "messagesexdcl.h"
#include "simdetectiongprot.h"
#include "simdetectiongexdcl.h"
//micha0901
#include "callmanagementgprot.h"
//micha_Man_sel
#include "mmi_trace.h"	
#include "engineermodedef.h"  
#include "nvram_user_defs.h"
#include "nvramprot.h"
#include "dual_sim.h"
#undef __NEWSIMULATOR

static NetworkSetupContext gNetworkSetup[MMI_SIM_NUMBER];
static NetworkSetupDisplay gNetworkSetupDisplay;

static NetworkSetupMsgQueue gNetworkSetupMsgQueue;
extern void mmi_show_qq_select_confirm(void (*default_entry) (void));


//micha1214, Remember to update ref_list.txt

const U16 nBandStrList[] = {
#ifdef __GSM850__
	STR_ID_NETSET_BAND_850,
#endif
#ifdef __EGSM900__	
	STR_ID_NETSET_BAND_900,
#endif
#ifdef __DCS1800__
	STR_ID_NETSET_BAND_1800,
#endif
#ifdef __PCS1900__
	STR_ID_NETSET_BAND_1900,
#endif
#if 0
#ifdef __GSM850__
#ifdef __DCS1800__
	STR_ID_NETSET_BAND_850_1800,
#endif
#endif
#endif /* Guoj delete. It's not used now at 2009-7-30 No such band match*/
#ifdef __EGSM900__
#ifdef __DCS1800__
	STR_ID_NETSET_BAND_DUAL,
#endif
#endif
#ifdef __GSM850__
#ifdef __PCS1900__	
	STR_ID_NETSET_BAND_850_1900,
#endif							
#endif							

//Lisen 06132005
//#if defined(__AUTO_BAND_SWITCH__)
       STR_ID_NETSET_BAND_AUTO,
//#endif
	0
};

const U16 nBandEnumList[] = {
#ifdef __GSM850__	
	MMI_BAND_850,
#endif							
#ifdef __EGSM900__	
	MMI_BAND_900,
#endif							
#ifdef __DCS1800__	
	MMI_BAND_1800,
#endif							
#ifdef __PCS1900__	
	MMI_BAND_1900,
#endif							
#if 0
#ifdef __GSM850__
#ifdef __DCS1800__	
	MMI_BAND_850_1800,
#endif														
#endif							
#endif /* Guoj delete. It's not used now at 2009-7-30 No such match*/
#ifdef __EGSM900__
#ifdef __DCS1800__	
	MMI_BAND_DUAL,
#endif														
#endif							
#ifdef __GSM850__	
#ifdef __PCS1900__	
	MMI_BAND_850_1900,
#endif							
#endif							
//Lisen 06132005
//#if defined(__AUTO_BAND_SWITCH__)
       MMI_BAND_AUTO,
//#endif

	0
};


//micha1015
extern S8 gHomePLMNNum[MMI_SIM_NUMBER][MAX_PLMN_LEN_MMI+1];

//micha1101
extern U8 gGPRSStatusModified;

/**************************************************************

	FUNCTION NAME		: InitNetworkSetupApp()

  	PURPOSE				: Initialization function for network setup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitNetworkSetupApp(void)
{
	U8 i;
	
	SetHiliteHandler(MENU9185_NETWORK_SETUP			,HighlightNETSETMain);
	SetHiliteHandler(MENU9186_SELECTION_MODE			,HighlightNETSETSelectionMode);
	SetHiliteHandler(MENU9186_SELECTION_MODE_MANUAL	,HighlightNETSETSelectionManual);
	SetHiliteHandler(MENU9186_SELECTION_MODE_AUTOMATIC	,HighlightNETSETSelectionAuto);
	SetHiliteHandler(MENU9187_PREFERRED_NETWORKS		,HighlightNETSETPreferredNetworks);
	SetHiliteHandler(MENU9188_BAND_SELECTION			,HighlightNETSETBandSelection);
	
#ifdef __MMI_GPRS_FEATURES__
	SetHiliteHandler(MENU9189_GPRS_CONNECTION_MODE	,HighlightNETSETGPRSConnectionMode);
	SetHintHandler(MENU9189_GPRS_CONNECTION_MODE		,HintNETSETGPRSConnectionMode);
#endif

#ifdef __MANUAL_MODE_NW_SEL__
	SetHiliteHandler(MENU9186_SELECTION_MODE_CHOICE	,HighlightNETSETSelectionModeChoice);
	SetHintHandler(MENU9186_SELECTION_MODE_CHOICE		,HintNETSETSelectionMode);
#endif
	SetHintHandler(MENU9188_BAND_SELECTION,			HintNETSETBandSelection);
	//SetHintHandler(MENU9186_SELECTION_MODE,		HintNETSETSelectionMode);
	InitPrefNwk();

	#ifdef __MMI_MULTI_SIM__
	for(i = 1; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MENU_MTPNP_NETWORK_SETUP + i -1, HighlightNETSETMain);
	}
	#endif

	gNetworkSetupDisplay.CurrSelectBand = 0;
	gNetworkSetupDisplay.CurrSelectNetwork = 0;
	gNetworkSetupDisplay.TempCurrSelectNetwork = 0;
	gNetworkSetupDisplay.PLMNListIcons = NULL;

	for ( i = 0; i < MAX_PLMN_LIST_LEN; i++)
		gNetworkSetupDisplay.PLMNName[i] = NULL;
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		gNetworkSetup[i].BandType = 1;
		gNetworkSetup[i].NetworkSelectAbort = MMI_FALSE;
		gNetworkSetup[i].PlmnList = NULL;
	#ifdef __MMI_GPRS_FEATURES__	
		gNetworkSetup[i].GPRSConnType = L4C_ALWAYS;
	#endif
	}

	gNetworkSetupMsgQueue.Index = 0;

	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightNETSETMain()

  	PURPOSE				: Hilite handler for network setup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETMain(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(NETSETGetBandSelectionReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(NETSETGetBandSelectionReq, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryNETSETMain()

  	PURPOSE				: Entry function for network setup 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETMain(void)
{
	U16 nStrItemList[MAX_SUB_MENUS];	/* Stores the strings id of submenus returned */
	U16 nNumofItem;					/* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;					/* Stores display attribue */
	U8* guiBuffer;						/* Buffer holding history data */
	/*U32 maskingByte=0xFFFFFFFF;
	U32 menuItemId=0xFFFFFFFF;*/
	U8 *NwType[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_NETSET_MAIN, NULL, EntryNETSETMain, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_MAIN);

	 
	nNumofItem = GetNumOfChild_Ext(MENU9185_NETWORK_SETUP);
//	nDispAttribute = GetDispAttributeOfItem(MENU9185_NETWORK_SETUP);
	GetSequenceStringIds_Ext(MENU9185_NETWORK_SETUP,nStrItemList);

	SetParentHandler(MENU9185_NETWORK_SETUP);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	ConstructHintsList(MENU9185_NETWORK_SETUP, NwType);

 
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */

	ShowCategory52Screen(  STR_ID_NETSET_SCR9185_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,(U16 *)gIndexIconsImageList,
						  NwType,
						  0,0,guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_pwr_context[0].IsCampOnTonePlayed=1; /* Flight mode might set this flag to 0 and it needs to be set again here */
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightNETSETSelectionMode()

  	PURPOSE				: Hilite handler for selection mode 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETSelectionMode(void) {
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryNETSETSelectionMode,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryNETSETSelectionMode, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}

//micha_Man_sel
#ifdef __MANUAL_MODE_NW_SEL__
/**************************************************************

	FUNCTION NAME		: HighlightNETSETSelectionModeChoice()

  	PURPOSE				: Hilite handler for selection mode 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETSelectionModeChoice(void) 
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryNETSETSelectionModeReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryNETSETSelectionModeReq, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	EntryNETSETSelectionModeReq
* DESCRIPTION
*	
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryNETSETSelectionModeReq(void)
{
	MYQUEUE Message;
	mmi_nw_set_plmn_select_mode_req_struct* myMsgPtr;	

	myMsgPtr = (mmi_nw_set_plmn_select_mode_req_struct*) OslConstructDataPtr(sizeof(mmi_nw_set_plmn_select_mode_req_struct));

	ClearAllKeyHandler();
	SetKeyHandler(0,KEY_END,KEY_EVENT_DOWN);	

#if (0)
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	if (g_pwr_context[0].PLMNSelectionMode == AUTOMATIC_MODE) {
		myMsgPtr->mode= MANUAL_MODE;
	}
	else {
		myMsgPtr->mode = AUTOMATIC_MODE;
	}
	
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	Message.oslMsgId = PRT_NW_SET_PLMN_SELECT_MODE_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);

	SetProtocolEventHandler(EntryNETSETSelectionModeRsp, PRT_NW_SET_PLMN_SELECT_MODE_RSP);
}

/*****************************************************************************
* FUNCTION
*	EntryNETSETSelectionModeRsp
* DESCRIPTION
*	
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryNETSETSelectionModeRsp(void *info) 
{
	mmi_nw_set_plmn_select_mode_rsp_struct *msgRsp;
	msgRsp = (mmi_nw_set_plmn_select_mode_rsp_struct *)info;
	U8 nSimID;

	nSimID = MTPNP_AD_get_protocol_event_src_mode();

	ClearProtocolEventHandler(PRT_NW_SET_PLMN_SELECT_MODE_RSP);

	if (msgRsp->result == KAL_TRUE) {
		DisplayPopup ((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)SUCCESS_TONE);
		if (g_pwr_context[nSimID].PLMNSelectionMode == AUTOMATIC_MODE) {
			g_pwr_context[nSimID].PLMNSelectionMode = MANUAL_MODE;
		}
		else {
			g_pwr_context[nSimID].PLMNSelectionMode = AUTOMATIC_MODE;
		}
	}
	else {
		DisplayPopup ((U8*)GetString(STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
	}
}



#endif

/**************************************************************

	FUNCTION NAME		: EntryNETSETSelectionMode()

  	PURPOSE				: Entry function for selection mode 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETSelectionMode(void) 
{
	U16 nStrItemList[MAX_SUB_MENUS];	/* Stores the strings id of submenus returned */
	U16 nNumofItem;					/* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;					/* Stores display attribue */
	U8* guiBuffer;						/* Buffer holding history data */
	U8 reqSrcIndex = 0;
//micha_Man_sel
#ifdef __MANUAL_MODE_NW_SEL__
	U8 *NwType[MAX_SUB_MENUS];
#endif

	#ifdef __MMI_MULTI_SIM__
	reqSrcIndex = GetReqSrcModIndex();
	#endif
	
	if (g_pwr_context[reqSrcIndex].InvalidSim == 0) {

		EntryNewScreen(SCR_ID_NETSET_SELECTION_MODE, NULL, EntryNETSETSelectionMode, NULL);

		guiBuffer = GetCurrGuiBuffer (SCR_ID_NETSET_SELECTION_MODE);
		nNumofItem = GetNumOfChild(MENU9186_SELECTION_MODE);
//		nDispAttribute = GetDispAttributeOfItem(MENU9186_SELECTION_MODE);
		GetSequenceStringIds(MENU9186_SELECTION_MODE,nStrItemList);
		SetParentHandler(MENU9186_SELECTION_MODE);
		RegisterHighlightHandler(ExecuteCurrHiliteHandler);
//micha_Man_sel
#ifdef __MANUAL_MODE_NW_SEL__
		ConstructHintsList(MENU9186_SELECTION_MODE, NwType);
#endif

//micha_Man_sel
#ifdef __MANUAL_MODE_NW_SEL__
	ShowCategory52Screen(  STR_ID_NETSET_SCR9186_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,(U16 *)gIndexIconsImageList,
						  NwType,
						  LIST_MENU,0,guiBuffer);
#else
		ShowCategory15Screen(  STR_ID_NETSET_SCR9186_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
							  STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  nNumofItem,nStrItemList,(U16 *)gIndexIconsImageList,
							  LIST_MENU,0,guiBuffer);
#endif
		SetKeyHandler(ExitNETSETSelectionMode, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	}
	else {
		DisplayPopup((PU8)GetString(STR_SIM_INVALID_SIM_SELECT_NETWORK_TXT) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
	}
	return;
}


/**************************************************************

	FUNCTION NAME		: ExitNETSETSelectionMode()

  	PURPOSE				: Entry function for selection mode 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitNETSETSelectionMode(void) 
{
	NETSETCheckAndRestoreNetwork();
	GoBackHistory();
}



/**************************************************************

	FUNCTION NAME		: HighlightNETSETSelectionManual()

  	PURPOSE				: Hilite handler for selection mode manual

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETSelectionManual(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(NETSETGetPLMNListCheck,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightNETSETSelectionAuto()

  	PURPOSE				: Hilite handler for selection mode Automatic

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETSelectionAuto(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(NETSETAutoSelectNetworkCheck,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: NETSETGetPLMNListCheck(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETGetPLMNListCheck(void) 
{
	U8 reqSrcMod = 0;
	if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)	/* FLIGHTMODE No Net */
	{
		DisplayPopup((PU8)GetString(STR_BDL_NOT_READY) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_MANUAL_SEARCH_PROGRESS);

		return;
	}

	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
	NETSETMsgQueuePush(GET_PLMN_LIST, NETSETGetPLMNListReq, PRT_NW_GET_PLMN_LIST_RSP, NETSETGetPLMNListRsp,reqSrcMod);
	EntryNETSETManualSearchProgress();
}



/**************************************************************

	FUNCTION NAME		: NETSETGetPLMNListReq(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETGetPLMNListReq(void) 
{
	MYQUEUE Message;
	U8 reqSrcIndex = 0;
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_GET_PLMN_LIST_REQ));
	
	#ifdef __MMI_MULTI_SIM__
	reqSrcIndex = GetReqSrcModIndex();
	#endif

	mmi_trace(g_sw_NW, "MMI_NW: NETSETGetPLMNListReq");
	SetProtocolEventHandlerExt(NETSETAbortPLMNListRsp, PRT_NW_ABORT_PLMN_LIST_RSP,reqSrcIndex);//man 2007-7-26 16:16
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C + reqSrcIndex;
	Message.oslMsgId = PRT_NW_GET_PLMN_LIST_REQ;
	Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryNETSETManualSearchProgress(void)

  	PURPOSE				: function to display progress screen for manual search

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETManualSearchProgress(void)
{
//micha1229
	U8 *str;

	EntryNewScreen(SCR_ID_NETSET_MANUAL_SEARCH_PROGRESS, NULL, EntryNETSETManualSearchProgress, NULL);

//	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_MANUAL_SEARCH_PROGRESS);	
	str = (U8*)GetString(STR_ID_NETSET_SEARCHIN_NW);
	ShowCategory66Screen(STR_ID_NETSET_MITEM9186,MAIN_MENU_TITLE_SETTINGS_ICON,0,0,STR_GLOBAL_ABORT,IMG_ID_NETSET_ABORT_LSK,
						str,IMG_ID_NETSET_NW_SEARCH,NULL);
	SetRightSoftkeyFunction(NETSETAbortPLMNListReq,KEY_EVENT_UP);
	SetKeyHandler(AbortGetPLMNListToIdle,KEY_END,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: AbortGetPLMNListToIdle(void)

  	PURPOSE				: abort the get plmn list req and goes back to idle screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AbortGetPLMNListToIdle(void)
{
//micha0715
	MYQUEUE Message;
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_ABORT_PLMN_LIST_TO_IDLE));	

	if (NETSETMsgQueueCheckCurrOP() == GET_PLMN_LIST) {
//micha0726
		if (gNetworkSetupMsgQueue.Index != gNetworkSetupMsgQueue.Top) {
			NETSETMsgQueueTopMinusOne();
		}		
		ClearProtocolEventHandler(PRT_NW_ABORT_PLMN_LIST_RSP);
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C;
		Message.oslMsgId = PRT_NW_ABORT_PLMN_LIST_REQ;
		Message.oslDataPtr = NULL;
		Message.oslPeerBuffPtr= NULL;
		OslMsgSendExtQueue(&Message);
	}
	else {
		if (NETSETMsgQueueCheckLastOP() == GET_PLMN_LIST) {
//micha0726
			if (gNetworkSetupMsgQueue.Index != gNetworkSetupMsgQueue.Top) {
				NETSETMsgQueueTopMinusOne();
			}		
		}
	}
	gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].CallBackRspHandler = NULL;
	DisplayIdleScreen();
	return;
}
/**************************************************************

	FUNCTION NAME		: NETSETGetPLMNListRsp(void)

  	PURPOSE				: response handler for get plmn list req 

	INPUT PARAMETERS	: void * 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETGetPLMNListRsp(void* info)
{
	U16 i;
	S8 *Temp=NULL;
	S8 pOutBuffer[(MAX_PLMN_LEN+2) * ENCODING_LENGTH];
//micha1015
	l4c_plmn_info_struct tempPLMNList;
	U8 nSimID;
	
	mmi_nw_get_plmn_list_rsp_struct *localPtr;
	localPtr = (mmi_nw_get_plmn_list_rsp_struct *)info;
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	mmi_trace(g_sw_NW, "MMI_NW: NETSETGetPLMNListRsp, net number is:%d", localPtr->length);
	if ( gNetworkSetup[nSimID].PlmnList != NULL)
	{
		OslMfree(gNetworkSetup[nSimID].PlmnList);
		gNetworkSetup[nSimID].PlmnList = NULL;
	}
	gNetworkSetup[nSimID].PlmnList = OslMalloc(sizeof(mmi_nw_get_plmn_list_rsp_struct));
	memcpy(gNetworkSetup[nSimID].PlmnList, localPtr, sizeof(mmi_nw_get_plmn_list_rsp_struct));

//micha1015
	if (memcmp(gHomePLMNNum[nSimID],"46002", 5) == 0) 
	{
		for(i=0;i<gNetworkSetup[nSimID].PlmnList->length;i++) 
		{
			if (memcmp(gNetworkSetup[nSimID].PlmnList->list[i].plmn_id,"46000", 5) == 0) 
			{
				memcpy(&tempPLMNList, &(gNetworkSetup[nSimID].PlmnList->list[0]), sizeof(l4c_plmn_info_struct));
				memcpy(&(gNetworkSetup[nSimID].PlmnList->list[0]),&( gNetworkSetup[nSimID].PlmnList->list[i]), sizeof(l4c_plmn_info_struct));				
				memcpy(&(gNetworkSetup[nSimID].PlmnList->list[i]), &tempPLMNList, sizeof(l4c_plmn_info_struct));				
			}
		}
	}

	
	if(gNetworkSetup[nSimID].PlmnList->result.flag != L4C_OK || gNetworkSetup[nSimID].PlmnList->length == 0 || isInCall())
	{
		if (gNetworkSetup[nSimID].PlmnList != NULL) 
		{
			OslMfree(gNetworkSetup[nSimID].PlmnList);
			gNetworkSetup[nSimID].PlmnList = NULL;
		}
		DisplayPopup((PU8)GetString(STR_ID_NETSET_NW_LIST_FAIL) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_MANUAL_SEARCH_PROGRESS);
	}
	else 
	{
		for(i = 0; i < gNetworkSetup[nSimID].PlmnList->length; ++i)
		{
			if( gNetworkSetupDisplay.PLMNName[i] != NULL)
			{
				OslMfree(gNetworkSetupDisplay.PLMNName[i]);
				gNetworkSetupDisplay.PLMNName[i]=NULL;
			}
			gNetworkSetupDisplay.PLMNName[i] = OslMalloc(PLMN_NAME_LENGTH * ENCODING_LENGTH);
			memset((void *)gNetworkSetupDisplay.PLMNName[i], 0, PLMN_NAME_LENGTH * ENCODING_LENGTH);
			Temp = GetCurrentPLMNName((S8*) (gNetworkSetup[nSimID].PlmnList->list[i].plmn_id));
			if(Temp== NULL)
			{
				AnsiiToUnicodeString(pOutBuffer,(PS8) gNetworkSetup[nSimID].PlmnList->list[i].plmn_id );
				pfnUnicodeStrcpy((PS8)gNetworkSetupDisplay.PLMNName[i], pOutBuffer);
			}
			else
			{
				pfnUnicodeStrcpy((PS8)gNetworkSetupDisplay.PLMNName[i],Temp);
			}
		}
       	 EntryNETSETShowPlmnList();
	}	
}
/**************************************************************

	FUNCTION NAME		: EntryNETSETShowPlmnList(void)

  	PURPOSE				: entry function to show plmn list 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETShowPlmnList(void)
{
//micha1229
//	U8* guiBuffer;
	U16	i;

	U8 nSimID = 0;

	#ifdef __MMI_MULTI_SIM__
	nSimID = GetReqSrcModIndex();
	#endif

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_SHOW_PLMN_LIST));
	
	EntryNewScreen(SCR_ID_NETSET_SHOW_PLMN_LIST, ExitNETSETShowPLMNList, NULL, NULL);
	
//	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_SHOW_PLMN_LIST);
	RegisterHighlightHandler(HiliteHandlerShowPLMNList);
	if( gNetworkSetupDisplay.PLMNListIcons!= NULL)
	{
		OslMfree(gNetworkSetupDisplay.PLMNListIcons);
		gNetworkSetupDisplay.PLMNListIcons= NULL;
	}
	gNetworkSetupDisplay.PLMNListIcons= OslMalloc((gNetworkSetup[nSimID].PlmnList->length*sizeof(U16)));

	for(i=0;i<gNetworkSetup[nSimID].PlmnList->length;i++)
	{
			gNetworkSetupDisplay.PLMNListIcons[i] = IMG_ID_NETSET_PRF_ADDNEW_MMC ;//man for bug 6032
	}
//micha1015
//	if(pfnUnicodeStrcmp((PS8)gNetworkSetupDisplay.PLMNName[0],(PS8)GetHomePLMNName())==0  || 
 
//	if(memcmp(gNetworkSetup.PlmnList->list[0].plmn_id, gHomePLMNNum, strlen((PS8)gNetworkSetup.PlmnList->list[0].plmn_id)) == 0 ||
//		((memcmp(gHomePLMNNum,"46002", 5) == 0 && memcmp(gNetworkSetup.PlmnList->list[0].plmn_id,"46000", 5) == 0)))

	{
		gNetworkSetupDisplay.PLMNListIcons[0] = (U16)IMG_ID_NETSET_PRF_HOME_PLMN;
	}
		
	if(*gNetworkSetupDisplay.PLMNName[0] != '\0')
	{
		ShowCategory84Screen(  STR_ID_NETSET_SCR_SELECT_NETWORK_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
							  STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  gNetworkSetup[nSimID].PlmnList->length, gNetworkSetupDisplay.PLMNName,gNetworkSetupDisplay.PLMNListIcons,1, 0, NULL);
	}
	else
	{
		ShowCategory84Screen(  STR_ID_NETSET_SCR_SELECT_NETWORK_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  0, gNetworkSetupDisplay.PLMNName,gNetworkSetupDisplay.PLMNListIcons,1, 0, NULL);
		SetKeyHandler(0, KEY_UP_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(0, KEY_DOWN_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(0, KEY_UP_ARROW,KEY_EVENT_UP);
		SetKeyHandler(0, KEY_DOWN_ARROW,KEY_EVENT_UP);
	}
	SetRightSoftkeyFunction(GoBackNetSelection,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(NETSETManualSelectNetworkCheck, KEY_EVENT_UP);
	SetKeyHandler(GoBackNetSelection, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: ExitNETSETShowPLMNList(void)

  	PURPOSE				: exit function to show plmn list 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitNETSETShowPLMNList(void) {

	history_t   currHistory;
	S16		nHistory	=	0;
	currHistory.scrnID = SCR_ID_NETSET_SHOW_PLMN_LIST;
	currHistory.entryFuncPtr = (FuncPtr)EntryNETSETShowPlmnList;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(currHistory.guiBuffer);
	AddHistory(currHistory);
	OslMfree(gNetworkSetupDisplay.PLMNListIcons);
	gNetworkSetupDisplay.PLMNListIcons= NULL;
	return;
}
/**************************************************************

	FUNCTION NAME		: GoBackNetSelection(void)

  	PURPOSE				: clear buffers and go back to network selection

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBackNetSelection(void) 
{
	U16 i=0;
	U8 reqSrcMod = 0;
	
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
	for(i = 0; i < gNetworkSetup[reqSrcMod].PlmnList->length; i++){
		if(gNetworkSetupDisplay.PLMNName[i])
		{
			OslMfree(gNetworkSetupDisplay.PLMNName[i]);
			gNetworkSetupDisplay.PLMNName[i]=NULL;
		}
	}

	 
	if(GetCurrScrnId() == SCR_ID_NETSET_MANUAL_SEARCH_PROGRESS)
	{
		GoBacknHistory(1);
	}
	else
	{
		DeleteScreenIfPresent(SCR_ID_NETSET_MANUAL_SEARCH_PROGRESS);
		GoBackHistory();
	}
}

/**************************************************************

	FUNCTION NAME		: NETSETManualSelectNetworkCheck(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETManualSelectNetworkCheck(void) 
{
	U8 reqSrcMod = 0;

	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
//micha_Man_sel
	gNetworkSetupDisplay.CurrSelectNetwork = gNetworkSetupDisplay.TempCurrSelectNetwork;
	NETSETMsgQueuePush(NETWORK_SELECTION, NETSETManualSelectNetworkReq, PRT_NW_SET_PLMN_RSP, NETSETManuSelectNetworkRsp,reqSrcMod);
	EntryNETSETManualRegProgress();
}

/**************************************************************

	FUNCTION NAME		: NETSETManualSelectNetworkReq(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETManualSelectNetworkReq(void) 
{
	mmi_nw_set_plmn_req_struct *myMsgPtr;
	MYQUEUE Message;
	U8 reqSrcMod = 0;

	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif

	gGPRSStatusModified = 1;
	mmi_trace(g_sw_NW, "MMI_NW: NETSETManualSelectNetworkReq");
	myMsgPtr = (mmi_nw_set_plmn_req_struct*) OslConstructDataPtr(sizeof(mmi_nw_set_plmn_req_struct));
	myMsgPtr->opcode = MMI_MANUAL_SEL;
	memset(myMsgPtr->oper, 0, MAX_PLMN_LEN+1);
	memcpy((PS8)myMsgPtr->oper,(PS8)gNetworkSetup[reqSrcMod].PlmnList->list[gNetworkSetupDisplay.CurrSelectNetwork].plmn_id, MAX_PLMN_LEN+1);
	strcpy((PS8) gNetworkSetup[reqSrcMod].PLMNRequested, (PS8) gNetworkSetup[reqSrcMod].PlmnList->list[gNetworkSetupDisplay.CurrSelectNetwork].plmn_id);
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C + reqSrcMod;
	Message.oslMsgId = PRT_NW_SET_PLMN_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryNETSETManualRegProgress(void)

  	PURPOSE				: entry function for progress screen in network selection from list

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETManualRegProgress(void)
{
//micha1229
	U8 *str=NULL;
//	U8 *guiBuffer=NULL

	EntryNewScreen(SCR_ID_NETSET_MANUAL_REG_PROGRESS, NULL, EntryNETSETManualRegProgress, NULL);

//	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_MANUAL_REG_PROGRESS);	
	str = (U8*)GetString(STR_ID_NETSET_REGISTERING_NW);
	ShowCategory66Screen(STR_ID_NETSET_MITEM9186,MAIN_MENU_TITLE_SETTINGS_ICON,
						0,0,STR_GLOBAL_ABORT,IMG_ID_NETSET_ABORT_LSK,
						str,IMG_ID_NETSET_NW_SEARCH,NULL);
	SetRightSoftkeyFunction(AbortManualRegGoBack,KEY_EVENT_UP);
	SetKeyHandler(AbortManualRegToIdle,KEY_END,KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: AbortManualRegGoBack(void)

  	PURPOSE				: network setup abort and go back to network select

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AbortManualRegGoBack(void)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
//micha_Man_sel
#ifdef __MANUAL_MODE_NW_SEL__
	if (g_pwr_context[reqSrcMod].PLMNSelectionMode == AUTOMATIC_MODE) {
		gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_TRUE;
	}
#else
	gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_TRUE;
#endif
	gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].CallBackRspHandler = NULL;
	GoBackToHistory(SCR_ID_NETSET_SHOW_PLMN_LIST);
	return;
}
/**************************************************************

	FUNCTION NAME		: AbortManualRegToIdle(void)

  	PURPOSE				: selection mode setup abort and go back to idle screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AbortManualRegToIdle(void)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
//micha_Man_sel
#ifdef __MANUAL_MODE_NW_SEL__
	if (g_pwr_context[reqSrcMod].PLMNSelectionMode == AUTOMATIC_MODE) {
	gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_TRUE;
		}
#else
	gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_TRUE;
#endif

	gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].CallBackRspHandler = NULL;
	DisplayIdleScreen();
	return;
}
/**************************************************************

	FUNCTION NAME		: AbortAutoSearchGoBack(void)

  	PURPOSE				: selection mode abort and go back to selection mode

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AbortAutoSearchGoBack(void)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif

	mmi_trace(g_sw_NW,"MMI_NW: Func: %s ", __FUNCTION__);

#ifdef __MANUAL_MODE_NW_SEL__
	if (g_pwr_context[reqSrcMod].PLMNSelectionMode == AUTOMATIC_MODE) {
		gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_TRUE;
	}
#else
	gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_TRUE;
#endif
	gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].CallBackRspHandler = NULL;

	if (g_pwr_context[reqSrcMod].CurrentServiceType == FULL_SERVICE && gPLMN[reqSrcMod][0] != 0)
	{
		ChangeRoamingState(reqSrcMod);
	}

	GoBackToHistory(SCR_ID_NETSET_SELECTION_MODE);
	return;
}
/**************************************************************

	FUNCTION NAME		: AbortAutoSearchToIdle(void)

  	PURPOSE				: network setup abort and go back to idle screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AbortAutoSearchToIdle(void)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
//micha_Man_sel
#ifdef __MANUAL_MODE_NW_SEL__
	if (g_pwr_context[reqSrcMod].PLMNSelectionMode == AUTOMATIC_MODE) {
		gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_TRUE;
	}
#else
	gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_TRUE;
#endif
	gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].CallBackRspHandler = NULL;

	if (g_pwr_context[reqSrcMod].CurrentServiceType == FULL_SERVICE && gPLMN[reqSrcMod][0] != 0)
	{
		ChangeRoamingState(reqSrcMod);
	}
    
	DisplayIdleScreen();
	return;
}

/**************************************************************

	FUNCTION NAME		: NETSETManualNetworkRestoreReq(void)

  	PURPOSE				: network setup abort 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETManualNetworkRestoreReq(void)
{
	mmi_nw_set_plmn_req_struct *myMsgPtr;
	MYQUEUE Message;
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif

		myMsgPtr = (mmi_nw_set_plmn_req_struct*) OslConstructDataPtr(sizeof(mmi_nw_set_plmn_req_struct));
		myMsgPtr->opcode = MMI_MANUAL_SEL;
		memset(myMsgPtr->oper, 0, MAX_PLMN_LEN+1);
//micha1229
		memcpy((PS8)myMsgPtr->oper,(PS8)gPLMN[reqSrcMod], MAX_PLMN_LEN);
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C + reqSrcMod;
		Message.oslMsgId = PRT_NW_SET_PLMN_REQ;
		Message.oslDataPtr = (oslParaType *)myMsgPtr;
		Message.oslPeerBuffPtr= NULL;
		OslMsgSendExtQueue(&Message);
	}

/**************************************************************

	FUNCTION NAME		: RestorePreviousNetwork(void)

  	PURPOSE				: network setup abort 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RestorePreviousNetwork(void)
{
	U8 reqSrcMod = 0;
	
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
	mmi_trace(g_sw_NW,"MMI_NW: RestorePreviousNetwork");
	gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_FALSE;
//	if(gPLMN[0]=='\0')
//	{
//		mmi_trace(1,"1111111111	if(gPLMN[0]==)");
//		NETSETMsgQueuePush(NETWORK_RESTORE, NETSETAutoNetworkSearchReq, PRT_NW_SET_PLMN_RSP, NULL);	
//	}
//	else
//	{
//		mmi_trace(1,"222222222 if(gPLMN[0  else");
//		NETSETMsgQueuePush(NETWORK_RESTORE, NETSETManualNetworkRestoreReq, PRT_NW_SET_PLMN_RSP, NULL);	
//	}
	NETSETMsgQueuePush(NETWORK_RESTORE, NETSETAutoNetworkSearchReq, PRT_NW_SET_PLMN_RSP, NULL,reqSrcMod);
}
/**************************************************************

	FUNCTION NAME		: NETSETManuSelectNetworkRsp(void)

  	PURPOSE				: response handler for select network 

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETManuSelectNetworkRsp(void *info) 
{
	mmi_nw_set_plmn_rsp_struct *localPtr;
	S8 *Temp=NULL;
	U16 i=0;
	S8 pOutBuffer[(MAX_PLMN_LEN+2)*ENCODING_LENGTH];
	U8 nSimID;
	
	localPtr = (mmi_nw_set_plmn_rsp_struct *)info;
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	mmi_trace(g_sw_NW, "MMI_NW: NETSETManuSelectNetworkRsp");
	if(localPtr->result.flag == L4C_OK && localPtr->status == L4C_RAC_OK
		&& !strcmp((PS8) gNetworkSetup[nSimID].PLMNRequested, (PS8) localPtr->plmn))
	{
		
		Temp = GetCurrentPLMNName((S8*) (localPtr->plmn));
		memset(gNWProviderName[nSimID],0,MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH);
		if(Temp== NULL)
		{
				AnsiiToUnicodeString(pOutBuffer,(PS8) localPtr->plmn );
				pfnUnicodeStrcpy((PS8)gNWProviderName[nSimID],(PS8)pOutBuffer);
		}
		else
		{
			pfnUnicodeStrcpy((PS8)gNWProviderName[nSimID],Temp);
		}
		for(i = 0; i < gNetworkSetup[nSimID].PlmnList->length; i++)
		{
			if(gNetworkSetupDisplay.PLMNName[i])
			{
				OslMfree(gNetworkSetupDisplay.PLMNName[i]);
				gNetworkSetupDisplay.PLMNName[i]=NULL;
			}
		}
		OslMfree(gNetworkSetup[nSimID].PlmnList);
		gNetworkSetup[nSimID].PlmnList = NULL;
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_MANUAL_SEARCH_PROGRESS);
		DeleteScreenIfPresent(SCR_ID_NETSET_SHOW_PLMN_LIST);
		DeleteScreenIfPresent(SCR_ID_NETSET_MANUAL_REG_PROGRESS);
	}
	else 
	{
		DisplayPopup((PU8)GetString(STD_ID_NETSET_NW_SELECT_FAIL) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_MANUAL_REG_PROGRESS);
		RestorePreviousNetwork();//man
	}		
	return;
}

/**************************************************************

	FUNCTION NAME		: HiliteHandlerShowPLMNList(void)

  	PURPOSE				: function to update the current selected index

	INPUT PARAMETERS	: S32 index

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteHandlerShowPLMNList(S32 index) 
{
	gNetworkSetupDisplay.TempCurrSelectNetwork = (U8) index;
	return;
}
/**************************************************************

	FUNCTION NAME		: NETSETAbortPLMNListReq(void)

  	PURPOSE				: abort success req function

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETAbortPLMNListReq(void)
{
	MYQUEUE Message;
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_ABORT_PLMN_LIST_REQ));

	if (NETSETMsgQueueCheckCurrOP() == GET_PLMN_LIST) {
//micha0726
		if (gNetworkSetupMsgQueue.Index != gNetworkSetupMsgQueue.Top) {
			NETSETMsgQueueTopMinusOne();
		}		
		SetProtocolEventHandler(NETSETAbortPLMNListRsp, PRT_NW_ABORT_PLMN_LIST_RSP);
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C;
		Message.oslMsgId = PRT_NW_ABORT_PLMN_LIST_REQ;
		Message.oslDataPtr = NULL;
		Message.oslPeerBuffPtr= NULL;
		OslMsgSendExtQueue(&Message);
	}
	else {
		if (NETSETMsgQueueCheckLastOP() == GET_PLMN_LIST) {
//micha0726
			if (gNetworkSetupMsgQueue.Index != gNetworkSetupMsgQueue.Top) {
				NETSETMsgQueueTopMinusOne();
			}		
		}
		GoBackHistory();
	}
	return;
}
/**************************************************************

	FUNCTION NAME		: NETSETAbortPLMNListRsp(void)

  	PURPOSE				: abort success response handler

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETAbortPLMNListRsp(void *info) 
{
	GoBackHistory();
	//NETSETMsgQueueIndexAddOne();// man 2007-7-31 add 
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightNETSETPreferredNetworks(void)

  	PURPOSE				: highlite handler for preferred networks

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETPreferredNetworks(void) 
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(NETSETGetPreferredNetworkReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(NETSETGetPreferredNetworkReq, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightNETSETBandSelection(void)

  	PURPOSE				: highlite handler for band selection menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETBandSelection(void) 
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	 
	#ifdef __MMI_ENGINEER_MODE__
	if(GetActiveScreenId() == EM_MAIN_MENU_SCR)
	{
		SetLeftSoftkeyFunction(NETSETGetBandSelectionReq,KEY_EVENT_UP);
		SetKeyHandler(NETSETGetBandSelectionReq, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
	else
	#endif
	{
		SetLeftSoftkeyFunction(EntryNETSETBandSelection,KEY_EVENT_UP);
		SetKeyHandler(EntryNETSETBandSelection, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
	

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryNETSETBandSelection(void)

  	PURPOSE				: entry function for band selection menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETBandSelection(void) 
{
	U8 nNumofItem;
	U8* guiBuffer;
	U16 x;
	U16 *nStrItemList = NULL;
	U8 reqSrcMod = 0;

	for (nNumofItem = 0; nBandStrList[nNumofItem] != 0; nNumofItem++);

	nStrItemList=(U16 *)OslMalloc(nNumofItem*sizeof(U16));

	for( x=0; x < nNumofItem; x++)
		nStrItemList[x] = nBandStrList[x];

#ifdef __MMI_REMOVE_BAND_1900__		//remove 1900 from main menu
   {
      U8 i,j, count=nNumofItem;
      for(i=j=0;i<nNumofItem;i++)
	      if(nStrItemList[i] != STR_ID_NETSET_BAND_1900)
	      {
	         nStrItemList[j] = nStrItemList[i];
	         j++;
	      }
	      else
	         count--;

      nNumofItem = count;
   }
#endif
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
	EntryNewScreen(SCR_ID_NETSET_BAND_SELECTION, NULL, EntryNETSETBandSelection, NULL);

	guiBuffer = GetCurrGuiBuffer (SCR_ID_NETSET_BAND_SELECTION);
	RegisterHighlightHandler (HiliteHandlerBandSelection);
	ShowCategory11Screen (STR_ID_NETSET_SCR9188_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, gNetworkSetup[reqSrcMod].BandType, guiBuffer);
	SetLeftSoftkeyFunction(NETSETSetBandSelectionCheck,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	if (nNumofItem == 0)
		ChangeLeftSoftkey(0,0);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	if (nStrItemList != NULL)
		OslMfree(nStrItemList);
	
	return;
}

#ifdef JATAAYU_SUPPORT
#ifdef JATAAYU_SUPPORT_MMS
extern BOOL jMMS_IsMMSAutoDownloading(U8 nSIMId);
#endif
#endif
/*************************************************
	FUNCTION NAME		: NETSETSetBandSelectionCheck(void)

  	PURPOSE				: function to make req for set band selection.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETSetBandSelectionCheck(void)
{
	U8 reqSrcMod = 0;
#ifdef JATAAYU_SUPPORT
#ifdef JATAAYU_SUPPORT_MMS
	if(jMMS_IsMMSAutoDownloading(0xff))
		return ;
#endif	
#endif
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif

	NETSETMsgQueuePush( BAND_SELECTION, NETSETSetBandSelectionReq, PRT_NW_SET_BAND_RSP, NETSETSetBandSelectionRsp,reqSrcMod);
	EntryNETSETBandSelectionProgress();
}



/*************************************************
	FUNCTION NAME		: NETSETSetBandSelectionReq(void)

  	PURPOSE				: function to make req for set band selection.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETSetBandSelectionReq(void)
{
	MYQUEUE Message;
	mmi_nw_set_preferred_band_req_struct *myMsgPtr;
	U8 reqSrcMod  = 0;

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_SET_BAND_REQ));

	myMsgPtr = (mmi_nw_set_preferred_band_req_struct*) OslConstructDataPtr(sizeof(mmi_nw_set_preferred_band_req_struct));
	myMsgPtr->band = (U8)nBandEnumList[gNetworkSetupDisplay.CurrSelectBand];

#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
#endif
	
	mmi_trace(g_sw_NW, "MMI_NW: NETSETSetBandSelectionReq, select band is: %d", myMsgPtr->band);
	
	#ifdef __MMI_REMEMBER_BAND_AUTO__
	{
		S16 error;
		U8 set_auto=0;
		if(gNetworkSetupDisplay.CurrSelectBand == NETSETAutoBandIndex())
			set_auto = 1;
		WriteRecord(NVRAM_EF_BAND_SELECT_AUTO + reqSrcMod, 1, (void*)&set_auto, sizeof(set_auto), &error);
	}
	#endif
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C + reqSrcMod;
	Message.oslMsgId = PRT_NW_SET_BAND_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
}
/*************************************************
	FUNCTION NAME		: EntryNETSETBandSelectionProgress(void)

  	PURPOSE				: entry function for band selection in progress screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETBandSelectionProgress(void)
{
//micha1229
	U8 *str;

	EntryNewScreen(SCR_ID_NETSET_BAND_SELECTION_PROGRESS, NULL, EntryNETSETBandSelectionProgress, NULL);

	SetKeyHandler(NULL, KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_END,KEY_EVENT_UP);
//	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_BAND_SELECTION_PROGRESS);	
	str = (U8*)GetString(STR_ID_NETSET_SEARCHIN_NW);
	ShowCategory66Screen(STR_ID_NETSET_SCR9188_CAPTION,MAIN_MENU_TITLE_SETTINGS_ICON,0,0,0,0,
						str,IMG_ID_NETSET_NW_SEARCH,NULL);
	return;
}

/*************************************************
	FUNCTION NAME		: NETSETSetBandSelectionRsp(void)

  	PURPOSE				: response handler for set band selection req

	INPUT PARAMETERS	: void * 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETSetBandSelectionRsp(void* info) 
{
	U8 nSimID;
	mmi_nw_set_preferred_band_rsp_struct* localPtr;
	U16 index;
	index = GetCurrHiliteID();
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_SET_BAND_RSP));
	
	localPtr = (mmi_nw_set_preferred_band_rsp_struct *)info;
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	mmi_trace(g_sw_NW, "MMI_NW: NETSETSetBandSelectionRsp, localPtr->result is:%d",localPtr->result);
	gNetworkSetup[nSimID].BandType = (U8) gNetworkSetupDisplay.CurrSelectBand;
	pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(nBandStrList[gNetworkSetup[nSimID].BandType]));
	Category52ChangeItemDescription(2,hintData[index]);
	if (localPtr->result == MMI_TRUE) 
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_BAND_SELECTION);
		DeleteScreenIfPresent(SCR_ID_NETSET_BAND_SELECTION_PROGRESS);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_ID_NETSET_SET_BAND_FAIL) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_BAND_SELECTION);
		DeleteScreenIfPresent(SCR_ID_NETSET_BAND_SELECTION_PROGRESS);
	}
	return;
}
/*************************************************
	FUNCTION NAME		: HiliteHandlerBandSelection(void)

  	PURPOSE				: hight light handler for getting curr index

	INPUT PARAMETERS	: S32 index 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteHandlerBandSelection(S32 index) 
{
	gNetworkSetupDisplay.CurrSelectBand = (U8) index;
	return;
}

/**************************************************************

	FUNCTION NAME		: NETSETAutoSelectNetworkCheck(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETAutoSelectNetworkCheck(void) 
{
	U8 reqSrcMode = 0;
	
	if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)	/* FLIGHTMODE No Net */
	{
		DisplayPopup((PU8)GetString(STR_BDL_NOT_READY) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_MANUAL_SEARCH_PROGRESS);

		return;
	}
	#ifdef __MMI_MULTI_SIM__
	reqSrcMode = GetReqSrcModIndex();
	#endif
	
	NETSETMsgQueuePush(NETWORK_SELECTION, NETSETAutoNetworkSearchReq, PRT_NW_SET_PLMN_RSP, NETSETAutoNetworkSearchRsp,reqSrcMode);
	EntryNETSETAutoSearchProgress();
}


/*************************************************
	FUNCTION NAME		: NETSETAutoNetworkSearchReq(void)

  	PURPOSE				: function to make req for new search

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETAutoNetworkSearchReq(void) 
{
	MYQUEUE Message;
	mmi_nw_set_plmn_req_struct *myMsgPtr;
	U8 reqSrcMod = 0;
	mmi_trace(g_sw_NW, "MMI_NW: NETSETAutoNetworkSearchReq");
//micha1101
	gGPRSStatusModified = 1;

	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
	myMsgPtr = (mmi_nw_set_plmn_req_struct*) OslConstructDataPtr(sizeof(mmi_nw_set_plmn_req_struct));
//	memset(gNWProviderName[0],0,MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH);//man
//	pfnUnicodeStrcpy((PS8)gNWProviderName[0],(PS8)GetString(STR_ID_NETSET_SEARCHIN_NW));
	myMsgPtr->opcode = MMI_AUTO_SELECT;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C + reqSrcMod;
	Message.oslMsgId = PRT_NW_SET_PLMN_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	return;
}
/*************************************************
	FUNCTION NAME		: EntryNETSETAutoSearchProgress(void)

  	PURPOSE				: entry function for progress screen in auto search

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETAutoSearchProgress(void)
{
//micha1229
	U8 *str;

	mmi_trace(g_sw_NW, "MMI_NW: EntryNETSETAutoSearchProgress");
	EntryNewScreen(SCR_ID_NETSET_AUTO_SEARCH_PROGRESS, NULL, EntryNETSETAutoSearchProgress, NULL);

//	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_AUTO_SEARCH_PROGRESS);	
	str = (U8*)GetString(STR_ID_NETSET_REGISTERING_NW);
	ShowCategory66Screen(STR_ID_NETSET_MITEM9186,MAIN_MENU_TITLE_SETTINGS_ICON,0,0,STR_GLOBAL_ABORT,IMG_ID_NETSET_ABORT_LSK,
						str,IMG_ID_NETSET_NW_SEARCH,NULL);
	SetRightSoftkeyFunction(AbortAutoSearchGoBack,KEY_EVENT_UP);
	SetKeyHandler(AbortAutoSearchToIdle,KEY_END,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: NETSETAutoNetworkSearchRsp(void)

  	PURPOSE				: response handler for new network search

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETAutoNetworkSearchRsp(void *info) 
{
	
	mmi_nw_set_plmn_rsp_struct *localPtr;
	S8 *Temp=NULL;
	S8 pOutBuffer[(MAX_PLMN_LEN+2)*ENCODING_LENGTH];
	U8 nSimID;
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	localPtr = (mmi_nw_set_plmn_rsp_struct *)info;
	mmi_trace(g_sw_NW, "MMI_NW: NETSETAutoNetworkSearchRsp, localPtr->result.flag is:%d,localPtr->status is:%d",localPtr->result.flag,localPtr->status);
	if((localPtr->result.flag == L4C_OK) && (localPtr->status == L4C_RAC_OK )) 
	{
		Temp = GetCurrentPLMNName((S8*) (localPtr->plmn));
		memset(gNWProviderName[nSimID],0,MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH);
		if(Temp== NULL) 
		{
			AnsiiToUnicodeString(pOutBuffer,(PS8) localPtr->plmn );
			pfnUnicodeStrcpy((PS8)gNWProviderName[nSimID],(PS8)pOutBuffer);
		}
		else 
		{
			pfnUnicodeStrcpy((PS8)gNWProviderName[nSimID],Temp);
		}
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_AUTO_SEARCH_PROGRESS);
	}
	else
	{
		DisplayPopup((PU8)GetString(STD_ID_NETSET_NW_SELECT_FAIL) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_AUTO_SEARCH_PROGRESS);
	}
}
/**************************************************************

	FUNCTION NAME		: NETSETGetBandSelectionReq(void)

  	PURPOSE				: function for making req to get band selection

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETGetBandSelectionReq(void)
{
	MYQUEUE msgStruct;
	U8 reqSrcIndex = 0;

	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(NETSETGetBandSelectionReq);
		return;
	}
	
	#ifdef __MMI_MULTI_SIM__
	reqSrcIndex = GetReqSrcModIndex();
	#endif

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_GET_BAND_REQ));

	SetProtocolEventHandlerExt(NETSETGetBandSelectionRsp, PRT_NW_GET_BAND_RSP, reqSrcIndex);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C + reqSrcIndex;
//micha1229	
	msgStruct.oslMsgId = PRT_NW_GET_BAND_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr= NULL;
	mmi_trace(g_sw_NW, "MMI_NW: NETSETGetBandSelectionReq");
	OslMsgSendExtQueue(&msgStruct);
	return;
}
/**************************************************************

	FUNCTION NAME		: NETSETGetBandSelectionRsp(void)

  	PURPOSE				: response handler req to get band selection

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void * 

	RETURNS				: void

 

**************************************************************/
void NETSETGetBandSelectionRsp(void *info)
{
	mmi_nw_get_band_rsp_struct* localPtr;
	U16 nNumofItem;
	U8 nSimID;
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
//micha1109
#ifdef __MMI_GPRS_FEATURES__
//	static U8 gotGPRSStatus = 0;
#endif	
	for (nNumofItem = 0; nBandStrList[nNumofItem] != 0; nNumofItem++);

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_GET_BAND_RSP));
	
	localPtr = (mmi_nw_get_band_rsp_struct *)info;
	mmi_trace(g_sw_NW, "MMI_NW: NETSETGetBandSelectionRsp, localPtr->result is:%d, localPtr->type is:%d", localPtr->result,localPtr->type);
	if(localPtr->result == 1)
	{
		for (gNetworkSetup[nSimID].BandType = 0; 
			((localPtr->type != nBandEnumList[gNetworkSetup[nSimID].BandType]) &&  (gNetworkSetup[nSimID].BandType <nNumofItem)) ; 
			gNetworkSetup[nSimID].BandType++);
	}
	else
	{
		gNetworkSetup[nSimID].BandType = 1;
	}
	#ifdef __MMI_REMEMBER_BAND_AUTO__
	{
		S16 error;
		U8 set_auto=0;
		ReadRecord(NVRAM_EF_BAND_SELECT_AUTO, 1, (void*)&set_auto, sizeof(set_auto), &error);
		if(set_auto !=0 && set_auto !=1)
		{
			set_auto = 0;
			WriteRecord(NVRAM_EF_BAND_SELECT_AUTO, 1, (void*)&set_auto, sizeof(set_auto), &error);
		}
		if(set_auto&&localPtr->type == MMI_BAND_AUTO)
		{
			gNetworkSetup[nSimID].BandType = nNumofItem -1;
		}
	}
	#endif

	 
	#ifdef __MMI_ENGINEER_MODE__  // zrx add 20060612
	if(GetActiveScreenId() == EM_MAIN_MENU_SCR)
	{
		EntryNETSETBandSelection();
	}
	else
	#endif
	{
#if 0//def __MMI_GPRS_FEATURES__  //guojian deleted for no this setting in gprs 
//micha1109
	if ( gotGPRSStatus == 0) 
	{
		NETSETGetGPRSModeReq();
		gotGPRSStatus = 1;
	}
	else 
	{
		EntryNETSETMain();
	}
#else
	EntryNETSETMain();
#endif
	}
}
/**************************************************************

	FUNCTION NAME		: HintNETSETBandSelection(void)

  	PURPOSE				: hint handler for band selection 

	INPUT PARAMETERS	: u16 index 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintNETSETBandSelection(U16 index)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(nBandStrList[gNetworkSetup[reqSrcMod].BandType]));
}

/**************************************************************

	FUNCTION NAME		: HintNETSETSelectionMode(void)

  	PURPOSE				: hint handler for selection mode 

	INPUT PARAMETERS	: u16 index 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintNETSETSelectionMode(U16 index)
{
//micha_Man_sel
#ifdef __MANUAL_MODE_NW_SEL__
	if ( g_pwr_context[0].PLMNSelectionMode == AUTOMATIC_MODE) {
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_ID_NETSET_MITEM9186_AUTOMATIC));
	}
	else {
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_ID_NETSET_MITEM9186_MANUAL));
	}

#else
	pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_ID_NETSET_MITEM9186_AUTOMATIC)); 
#endif
}



#ifdef __MMI_GPRS_FEATURES__

/**************************************************************

	FUNCTION NAME		: HighlightNETSETGPRSConnectionMode

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETGPRSConnectionMode(void) 
{
	ChangeLeftSoftkey(STR_ID_NETSET_CHANGE, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(NETSETSetGPRSConnModeReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: HintNETSETGPRSConnectionMode

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void HintNETSETGPRSConnectionMode(U16 index)
{
	U8 reqSrcMod = 0;
	
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);

	if(gNetworkSetup[reqSrcMod].GPRSConnType == L4C_ALWAYS)
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_ID_NETSET_GPRS_CONNECTION_MODE_ALWAYS));
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_ID_NETSET_GPRS_CONNECTION_MODE_WHENNEEDED));
	}
}


void EntryNETSETGPRSConnectionProgress(void)
{
	U8 *str;

	EntryNewScreen(SCR_ID_NETSET_DUMMY_PROGRESS, NULL, EntryNETSETGPRSConnectionProgress, NULL);

	SetKeyHandler(NULL, KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_END,KEY_EVENT_UP);
//	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_BAND_SELECTION_PROGRESS);	
	str = (U8*)GetString(STR_ID_NETSET_SEARCHIN_NW);
	ShowCategory66Screen(STR_ID_NETSET_MITEM9189,0,0,0,0,0,str,IMG_ID_NETSET_NW_SEARCH,NULL);
	return;
}

void NETSETSetGPRSConnModeRspTimeoutHdlr(void)
{
	if(IsScreenPresent(SCR_ID_NETSET_DUMMY_PROGRESS)) /* Any interrupt happens so don't display popup */
	{
		DeleteScreenIfPresent(SCR_ID_NETSET_DUMMY_PROGRESS);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
		DeleteScreenIfPresent(SCR_ID_NETSET_DUMMY_PROGRESS);
	}
	#ifndef MMI_ON_HARDWARE_P
	StopTimer(TIMER_SET_GPRS_MODE);
	#endif
}

/**************************************************************

	FUNCTION NAME		: NETSETSetGPRSConnModeReq

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETSetGPRSConnModeReq(void)
{
	U8 reqSrcMod = 0;
	
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
	//StopTimer(TIMER_SET_GPRS_MODE);
	//StartTimer(TIMER_SET_GPRS_MODE, SET_GPRS_MODE_DELAY, NETSETSetGPRSConnModeTimeOut);

	if(gNetworkSetup[reqSrcMod].GPRSConnType == L4C_ALWAYS) {
		gNetworkSetup[reqSrcMod].GPRSConnType = L4C_WHEN_NEEDED;
	}
	else {
		gNetworkSetup[reqSrcMod].GPRSConnType = L4C_ALWAYS;
	}
	//DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
	
	 
	EntryNETSETGPRSConnectionProgress();
	NETSETSetGPRSConnModeTimeOut();
}


/**************************************************************

	FUNCTION NAME		: NETSETSetGPRSConnModeTimeOut

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETSetGPRSConnModeTimeOut(void)
{
	MYQUEUE   Message;
	mmi_nw_set_gprs_connect_type_req_struct *msg = NULL;
	U8 reqSrcMod = 0;
	
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif

	msg = (mmi_nw_set_gprs_connect_type_req_struct*) 
			OslConstructDataPtr(sizeof(mmi_nw_set_gprs_connect_type_req_struct));


	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C + reqSrcMod;
	if(gNetworkSetup[reqSrcMod].GPRSConnType == L4C_ALWAYS) {
		msg->type = L4C_ALWAYS;
	}
	else {
		msg->type = L4C_WHEN_NEEDED;
	}
	
	SetProtocolEventHandlerExt(NETSETSetGPRSConnModeRsp, PRT_NW_SET_GPRS_CONNECT_TYPE_RSP,reqSrcMod);
	Message.oslMsgId = PRT_NW_SET_GPRS_CONNECT_TYPE_REQ;
	Message.oslDataPtr = (oslParaType *)msg;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: NETSETSetGPRSConnModeRsp

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETSetGPRSConnModeRsp(void *info)
{
	//ignore the response
	StopTimer(TIMER_SET_GPRS_MODE);
	StartTimer(TIMER_SET_GPRS_MODE, SET_GPRS_MODE_DELAY, NETSETSetGPRSConnModeRspTimeoutHdlr);
}


//micha1109 end

/**************************************************************

	FUNCTION NAME		: NETSETGetGPRSModeReq

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETGetGPRSModeReq(void)
{
	MYQUEUE msgStruct;
	U8 reqSrcMod = 0;
	
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
	ClearAllKeyHandler();
	
	SetProtocolEventHandlerExt(NETSETGetGPRSModeRsp, PRT_NW_GET_GPRS_CONNECT_TYPE_RSP, reqSrcMod);
	/* format the message to get band selection */
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C + reqSrcMod;
//micha1229	
	msgStruct.oslMsgId = PRT_NW_GET_GPRS_CONNECT_TYPE_REQ;
	msgStruct.oslSapId = INVALID_SAP;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr= NULL;
	 
	OslMsgSendExtQueue(&msgStruct);
	return;

}

/**************************************************************

	FUNCTION NAME		: NETSETGetGPRSModeRsp

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETGetGPRSModeRsp(void *info) 
{

	U8 nSimID;
	mmi_nw_get_gprs_connect_type_rsp_struct* localPtr;

	localPtr = (mmi_nw_get_gprs_connect_type_rsp_struct *)info;
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	if(localPtr->result == MMI_TRUE)
	{
		gNetworkSetup[nSimID].GPRSConnType = localPtr->type;
	}
	else
	{
		/* else take the default values */
		gNetworkSetup[nSimID].GPRSConnType = L4C_ALWAYS;
	}
	EntryNETSETMain();
}


#endif


/**************************************************************

	FUNCTION NAME		: NETSETMsgQueueIndexAddOne
  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETMsgQueueIndexAddOne(void) 
{
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_MSG_Q_IDX_ADD_ONE, gNetworkSetupMsgQueue.Index, gNetworkSetupMsgQueue.Top ));

	ASSERT(gNetworkSetupMsgQueue.Index != gNetworkSetupMsgQueue.Top);
	if ( (gNetworkSetupMsgQueue.Index+1) < NETSETMSGQUEUESIZE) {
		gNetworkSetupMsgQueue.Index++;
	}
	else{
		gNetworkSetupMsgQueue.Index = 0;
	}
}


/**************************************************************

	FUNCTION NAME		: NETSETMsgQueueTopAddOne
  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETMsgQueueTopAddOne(void) 
{
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_MSG_Q_TOP_ADD_ONE, gNetworkSetupMsgQueue.Index, gNetworkSetupMsgQueue.Top));

	if ( (gNetworkSetupMsgQueue.Top+1) < NETSETMSGQUEUESIZE) {
		gNetworkSetupMsgQueue.Top++;
	}
	else{
		gNetworkSetupMsgQueue.Top = 0;
	}
	ASSERT(gNetworkSetupMsgQueue.Index != gNetworkSetupMsgQueue.Top);
}



/**************************************************************

	FUNCTION NAME		: NETSETMsgQueueTopMinusOne
  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETMsgQueueTopMinusOne(void) 
{
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_MSG_Q_TOP_MINUS_ONE, gNetworkSetupMsgQueue.Index, gNetworkSetupMsgQueue.Top));
	
	ASSERT(gNetworkSetupMsgQueue.Index != gNetworkSetupMsgQueue.Top);
	gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].CallBackRspHandler = NULL;
	if ( gNetworkSetupMsgQueue.Top == 0) {
		gNetworkSetupMsgQueue.Top = NETSETMSGQUEUESIZE - 1;
	}
	else{
		gNetworkSetupMsgQueue.Top--;
	}
	gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].CallBackRspHandler = NULL;
}

/**************************************************************

	FUNCTION NAME		: NETSETMsgQueueGetPreviousTop
  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 NETSETMsgQueueGetPreviousTop(void) 
{
	if (  gNetworkSetupMsgQueue.Top == 0) {
		return(NETSETMSGQUEUESIZE - 1);
	}
	else{
		return( gNetworkSetupMsgQueue.Top - 1);
	}
}

/**************************************************************

	FUNCTION NAME		: NETSETMsgQueueGetPreviousIndex
  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 NETSETMsgQueueGetPreviousIndex(void) 
{
	if (  gNetworkSetupMsgQueue.Index == 0) {
		return(NETSETMSGQUEUESIZE - 1);
	}
	else{
		return( gNetworkSetupMsgQueue.Index - 1);
	}
}


/**************************************************************

	FUNCTION NAME		: NETSETMsgQueueProtocolHandler

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETMsgQueueProtocolHandler(void *info) 
{
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_MSG_Q_PROT_HDLR, gNetworkSetupMsgQueue.Index, gNetworkSetupMsgQueue.Top));
//micha0720
	if (gNetworkSetupMsgQueue.Index == gNetworkSetupMsgQueue.Top) {
		return;
	}		

	NETSETMsgQueueIndexAddOne();
	if (gNetworkSetupMsgQueue.Index == gNetworkSetupMsgQueue.Top) {
		if (gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousIndex()].CallBackRspHandler != NULL) {
			(* gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousIndex()].CallBackRspHandler)(info);
		}
	}
	else {
		(* gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Index].CallBackFunc)();
		SetProtocolEventHandler(NETSETMsgQueueProtocolHandler, gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Index].MsgID);
	}
}


/**************************************************************

	FUNCTION NAME		: NETSETMsgQueuePush

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETMsgQueuePush(U8 ItemType, FuncPtr CallBackFunc, UINT32 MsgID, PsFuncPtr CallBackRspHandler, U8 src_mod) 
{
	if (gNetworkSetupMsgQueue.Index == gNetworkSetupMsgQueue.Top ) {
		(* CallBackFunc)();
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].ItemType = ItemType;
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].CallBackFunc = CallBackFunc;
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].MsgID= MsgID;
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].CallBackRspHandler= CallBackRspHandler;
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].srcMode = src_mod;
		NETSETMsgQueueTopAddOne();
		SetProtocolEventHandlerExt(NETSETMsgQueueProtocolHandler, MsgID,gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Index].srcMode);
		MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_MSG_Q_PUSH_EXEC, ItemType));		
	}
	else if (NETSETMsgQueueGetPreviousTop() ==gNetworkSetupMsgQueue.Index ) {
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].ItemType = ItemType;
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].CallBackFunc = CallBackFunc;
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].MsgID= MsgID;
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].CallBackRspHandler= CallBackRspHandler;
		gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].srcMode = src_mod;
		NETSETMsgQueueTopAddOne();
		MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_MSG_Q_PUSH_ONLY_ONE_ADD, ItemType));		
	}
//micha0715
	else if (gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].ItemType == NETWORK_RESTORE
				&& (( ItemType == GET_PLMN_LIST) || ( ItemType == BAND_SELECTION))) {
			gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].ItemType = ItemType;
			gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].CallBackFunc = CallBackFunc;
			gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].MsgID= MsgID;
			gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].CallBackRspHandler= CallBackRspHandler;
			gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Top].srcMode = src_mod;
			NETSETMsgQueueTopAddOne();
			MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_MSG_Q_PUSH_ADD_RESTORE, ItemType));		
	}
	else {
		gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].ItemType = ItemType;
		gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].CallBackFunc = CallBackFunc;
		gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].MsgID= MsgID;
		gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].CallBackRspHandler= CallBackRspHandler;
		gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].srcMode = src_mod;
		MMI_TRACE((MMI_TRACE_G7_MISC, MMI_NETSET_MSG_Q_PUSH_REPLACE_LAST, ItemType));		
	}
}



/**************************************************************

	FUNCTION NAME		: NETSETMsgQueueCheckCurrOP

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 NETSETMsgQueueCheckCurrOP(void) 
{
	return(gNetworkSetupMsgQueue.Queue[gNetworkSetupMsgQueue.Index].ItemType);
}



/**************************************************************

	FUNCTION NAME		: NETSETMsgQueueCheckLastOP

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 NETSETMsgQueueCheckLastOP(void) 
{
	return(gNetworkSetupMsgQueue.Queue[NETSETMsgQueueGetPreviousTop()].ItemType);
}


/**************************************************************

	FUNCTION NAME		: NETSETCheckAndRestoreNetwork

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
//micha0901
void NETSETCheckAndRestoreNetwork(void) 
{
	U8 reqSrcMod = 0;
	
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	
	if (	gNetworkSetup[reqSrcMod].NetworkSelectAbort == MMI_TRUE) {
		RestorePreviousNetwork();
		gNetworkSetup[reqSrcMod].NetworkSelectAbort = MMI_FALSE;
	}
}

U8 NETSETAutoBandIndex(void)
{
	return (sizeof(nBandEnumList)/sizeof(nBandEnumList[0]) -2);
}



