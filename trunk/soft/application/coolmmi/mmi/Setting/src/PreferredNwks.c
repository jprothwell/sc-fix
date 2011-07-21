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
 * PreferredNwks.c
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "debuginitdef.h"
#include "mainmenudef.h"
#include "protocolevents.h"
#include "globaldefs.h"
#include "settingdefs.h"
#include "frameworkstruct.h"
#include "networksetupdefs.h"
#include "networksetupgprot.h"
#include "settingprofile.h"
#include "simdetectiongexdcl.h"
#include "settingprofile.h"
#include "preferrednwksstruct.h"
#include "prefnwksgprot.h"
#include "custresdef.h"
#include "plmnenum.h"

//micha0901
#include "commonscreens.h"
#undef __NEWSIMULATOR 
 
#include "wgui_categories_popup.h"
#include "wgui_categories_inputs.h"



static PrefNetworkListContext gPrefList;
static PrefNetworkListDisplay gPrefListDisplay;


/**************************************************************

	FUNCTION NAME		: InitPrefNwk(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitPrefNwk(void)
{
	SetHiliteHandler(MITEM_PRR_NWK_DELETE, HighlightNETSETDelPrefNwk);
	SetHiliteHandler(MITEM_PRR_NWK_EDIT, HighlightNETSETEditPrefNwk);
	SetHiliteHandler(MITEM_PRR_NWK_ADD, HighlightNETSETAddPrefNwk);
	SetHiliteHandler(MITEM_PRR_NWK_NEW, HighlightNETSETNewPrefNwk);

	gPrefList.PrefListSize = 0;
	gPrefListDisplay.CurrSelAddPLMN = 0;
	gPrefListDisplay.CurrSelPrefNetwork = 0;
	gPrefListDisplay.IsNewPrefNetwork = 0;
	gPrefListDisplay.PrefNwGotResponse = 0;
	gPrefListDisplay.PriorityVal = 0;
}

/**************************************************************

	FUNCTION NAME		: HighlightNETSETDelPrefNwk(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETDelPrefNwk(void)
{
	SetLeftSoftkeyFunction(EntryNETSETConfirmDeleteNetwork,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: HighlightNETSETEditPrefNwk(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETEditPrefNwk(void)
{
	SetLeftSoftkeyFunction(EntryNETSETEditNetwork,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
//micha0529
	SetKeyHandler(EntryNETSETEditNetwork, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: EntryNETSETEditNetwork(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
 void EntryNETSETEditNetwork(void)
 {
 	PU8 guiBuffer;

	EntryNewScreen(SCR_ID_NETSET_EDIT_NETWORK, NULL, EntryNETSETEditNetwork, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_EDIT_NETWORK); 
	gPrefListDisplay.PriorityVal= gPrefListDisplay.CurrSelPrefNetwork+1;
	ShowCategory87Screen(STR_ID_NETSET_PRF_ADD_SHW_NWK_PRIORITY_TITLE, MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						1, (S32)(gPrefList.PrefListSize), &gPrefListDisplay.PriorityVal,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(NETSETEditNetworkDeletePhaseReq, KEY_EVENT_UP);
}


/**************************************************************

	FUNCTION NAME		: NETSETEditNetworkDeletePhaseReq(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETEditNetworkDeletePhaseReq(void)
{
		mmi_smu_set_preferred_oper_list_req_struct *myMsgPtr;
		MYQUEUE Message;
		U8 reqsrc = 0;
		#ifdef __MMI_MULTI_SIM__
		reqsrc = GetReqSrcModIndex();
		#endif
		ClearAllKeyHandler();
		myMsgPtr = (mmi_smu_set_preferred_oper_list_req_struct*) OslConstructDataPtr(sizeof(mmi_smu_set_preferred_oper_list_req_struct));
		myMsgPtr->opcode = SMU_REMOVE_PLMNSEL;
		myMsgPtr->index = (U8)gPrefListDisplay.CurrSelPrefNetwork;
		memcpy (myMsgPtr->oper, gPrefList.PrefPLMN[gPrefListDisplay.CurrSelPrefNetwork],(MAX_PLMN_LEN + 1));
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C + reqsrc;

		Message.oslMsgId = PRT_SET_PREFERRED_NETWORKS_REQ;
		Message.oslDataPtr = (oslParaType *)myMsgPtr;
		Message.oslPeerBuffPtr= NULL;
		SetProtocolEventHandlerExt(NETSETEditNetworkDeletePhaseRsp,PRT_SET_PREFERRED_NETWORKS_RSP, reqsrc);
		OslMsgSendExtQueue(&Message);
 }



/**************************************************************

	FUNCTION NAME		: NETSETEditNetworkDeletePhaseRsp(void *)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETEditNetworkDeletePhaseRsp(void* info)
{
	mmi_smu_set_preferred_oper_list_rsp_struct *myMsgPtr;

	PRINT_INFORMATION(("*** In NETSETEditNetworkDeletePhaseRsp\n"));
	myMsgPtr = (mmi_smu_set_preferred_oper_list_rsp_struct*)info;
	mmi_trace(0,"wangzl: NETSETEditNetworkDeletePhaseRsp, myMsgPtr->result is %d",myMsgPtr->result);
	mmi_trace(0,"wangzl: NETSETEditNetworkDeletePhaseRsp, gPrefList.PrefListSize is %d,gPrefListDisplay.CurrSelPrefNetwork is %d",gPrefList.PrefListSize,gPrefListDisplay.CurrSelPrefNetwork);
	if (myMsgPtr->result == MMI_FALSE ||
		gPrefList.PrefListSize <= gPrefListDisplay.CurrSelPrefNetwork){
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
//micha0612
		DeleteScreenIfPresent(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
		ExitNETSETShowPreferredListReGet();
	} 
	else
	{
		NETSETEditNetworkAddPhaseReq();
	}
}

/**************************************************************

	FUNCTION NAME		: NETSETEditNetworkAddPhaseReq(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETEditNetworkAddPhaseReq(void)
{
		mmi_smu_set_preferred_oper_list_req_struct *myMsgPtr;
		MYQUEUE Message;
		U8 reqsrc = 0;

		#ifdef __MMI_MULTI_SIM__
		reqsrc = GetReqSrcModIndex();
		#endif

		myMsgPtr = (mmi_smu_set_preferred_oper_list_req_struct*) OslConstructDataPtr(sizeof(mmi_smu_set_preferred_oper_list_req_struct));
		myMsgPtr->opcode = SMU_ADD_PLMNSEL;
		myMsgPtr->index = gPrefListDisplay.PriorityVal - 1;
		memcpy (myMsgPtr->oper, gPrefList.PrefPLMN[gPrefListDisplay.CurrSelPrefNetwork],(MAX_PLMN_LEN + 1));
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C + reqsrc;

		Message.oslMsgId = PRT_SET_PREFERRED_NETWORKS_REQ;
		Message.oslDataPtr = (oslParaType *)myMsgPtr;
		Message.oslPeerBuffPtr= NULL;
		SetProtocolEventHandlerExt(NETSETEditNetworkAddPhaseRsp,PRT_SET_PREFERRED_NETWORKS_RSP, reqsrc);
		OslMsgSendExtQueue(&Message);
 }

/**************************************************************

	FUNCTION NAME		: NETSETEditNetworkAddPhaseRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETEditNetworkAddPhaseRsp(void* info)
{
	mmi_smu_set_preferred_oper_list_rsp_struct *myMsgPtr;
	myMsgPtr = (mmi_smu_set_preferred_oper_list_rsp_struct*)info;
	if (myMsgPtr->result == MMI_FALSE){
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
//micha0612
		DeleteScreenIfPresent(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
		ExitNETSETShowPreferredListReGet();
	} 
	else {
		
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
//micha0612
		DeleteScreenIfPresent(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
		ExitNETSETShowPreferredListReGet();
	}
	
}

/**************************************************************

	FUNCTION NAME		: HighlightNETSETAddPrefNwk(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETAddPrefNwk(void)
{
	SetLeftSoftkeyFunction(EntryNETSETAddNetworkPLMNList,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryNETSETAddNetworkPLMNList, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: NETSETGetPrefNetworkListItem

  	PURPOSE				: Return Item for dynamic category screen

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS: nil

	RETURNS				: void

 

**************************************************************/

pBOOL NETSETGetPrefNetworkListItem( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{

	if((item_index < 0) || (item_index > MAX_PLMN_NO_LIST_LENGTH))
		return FALSE;

	pfnUnicodeStrcpy((S8*) str_buff, (S8*)GetCurrentPLMNName(sPlmnsToDisplayName[item_index]));

	*img_buff_p = NULL;

	return TRUE;
}


/**************************************************************

	FUNCTION NAME		: NETSETGetPrefNetworkListHint(void)

  	PURPOSE				: Return Hint for dynamic category screen

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS: nil

	RETURNS				: void

 

**************************************************************/
S32 NETSETGetPrefNetworkListHint ( S32 item_index, UI_string_type *hint_array )
{

	AnsiiToUnicodeString((PS8) *hint_array,(PS8)sPlmnsToDisplayName[item_index]);
	
	return TRUE;
}


/**************************************************************

	FUNCTION NAME		: EntryNETSETAddNetworkPLMNList(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: info struct from PS

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETAddNetworkPLMNList(void)
{
	U16 index;
	PU8 guiBuffer;
	PU8 tempNetworkName;	

	EntryNewScreen(SCR_ID_NETSET_ADD_NEW_NETWORK_PLMN_LIST, NULL, EntryNETSETAddNetworkPLMNList, NULL);

	for(index = 0; index < MAX_PLMN_NO_LIST_LENGTH ; ++index)	{
		tempNetworkName = (PU8)GetCurrentPLMNName(sPlmnsToDisplayName[index]);
		if( tempNetworkName == NULL) break;
	}
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_ADD_NEW_NETWORK_PLMN_LIST);
	RegisterHighlightHandler(HiliteHandlerAddNetworkPLMNList);

#if (0)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	ShowCategory184Screen( STR_ID_NETSET_PRF_ADD_SHW_NWK_TITLE, MAIN_MENU_TITLE_SETTINGS_ICON,
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
									index, NETSETGetPrefNetworkListItem, NETSETGetPrefNetworkListHint,
									0, guiBuffer );

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryNETSETAddNetworkPriority, KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntryNETSETAddNetworkPriority, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: EntryNETSETAddNetworkPriority(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryNETSETAddNetworkPriority(void)
{
	PU8 guiBuffer;

	EntryNewScreen(SCR_ID_NETSET_ADD_NEW_NETWORK_PRIORITY, NULL, EntryNETSETAddNetworkPriority, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_ADD_NEW_NETWORK_PRIORITY); 
	gPrefListDisplay.PriorityVal = (S32)(gPrefList.PrefListSize+1);
	ShowCategory87Screen(STR_ID_NETSET_PRF_ADD_SHW_NWK_PRIORITY_TITLE, MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						1, (S32)(gPrefList.PrefListSize+1), &gPrefListDisplay.PriorityVal,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(ChcekThenAddNetwork, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: ChcekThenAddNetwork(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ChcekThenAddNetwork(void)
{
	if (IsNetworkDuplicated() == 1 || (gPrefListDisplay.PriorityVal > MAX_PREFFERED_PLMN) || (gPrefListDisplay.PriorityVal > g_pwr_context[0].NoOfPLMNSupported)){
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
	}
	else
		NETSETAddPrefNetworkReq();
}
/**************************************************************

	FUNCTION NAME		: HiliteHandlerPrefList(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteHandlerPrefList(S32 hiliteIdx)
{
	gPrefListDisplay.CurrSelPrefNetwork= (U8)hiliteIdx;
}


/**************************************************************

	FUNCTION NAME		: HiliteHandlerAddNetworkPLMNList(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteHandlerAddNetworkPLMNList(S32 hiliteIdx)
{
	gPrefListDisplay.CurrSelAddPLMN= (U8)hiliteIdx;
}


/**************************************************************

	FUNCTION NAME		: NETSETAddPrefNetworkReq(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETAddPrefNetworkReq(void)
{
	mmi_smu_set_preferred_oper_list_req_struct *myMsgPtr;
	MYQUEUE Message;
	U8 reqsrc = 0;
	#ifdef __MMI_MULTI_SIM__
	reqsrc = GetReqSrcModIndex();
	#endif
	ClearAllKeyHandler();
	PRINT_INFORMATION(("*** In NETSETAddPrefNetworkReq\n"));
	myMsgPtr = (mmi_smu_set_preferred_oper_list_req_struct*) OslConstructDataPtr(sizeof(mmi_smu_set_preferred_oper_list_req_struct));
	myMsgPtr->opcode = SMU_ADD_PLMNSEL;
	memset(myMsgPtr->oper,0,MAX_PLMN_LEN+1);
	myMsgPtr->index = gPrefListDisplay.PriorityVal - 1;
	if(gPrefListDisplay.IsNewPrefNetwork)
	{
		UnicodeToAnsii((PS8)myMsgPtr->oper,(PS8)gPrefListDisplay.PlmnNew);
		gPrefListDisplay.IsNewPrefNetwork = 0;
	}
	else
	{
		memcpy (myMsgPtr->oper,sPlmnsToDisplayName[gPrefListDisplay.CurrSelAddPLMN],(MAX_PLMN_LEN + 1));
	}
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C + reqsrc;
	Message.oslMsgId = PRT_SET_PREFERRED_NETWORKS_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	SetProtocolEventHandlerExt(NETSETAddPrefNetworkRsp,PRT_SET_PREFERRED_NETWORKS_RSP,reqsrc);
	OslMsgSendExtQueue(&Message);
}

/**************************************************************

	FUNCTION NAME		: NETSETAddPrefNetworkRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETAddPrefNetworkRsp(void* info)
{
	mmi_smu_set_preferred_oper_list_rsp_struct *myMsgPtr;

	myMsgPtr = (mmi_smu_set_preferred_oper_list_rsp_struct*)info;
	if (myMsgPtr->result == MMI_FALSE){
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
//micha0612
		DeleteScreenIfPresent(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
		ExitNETSETShowPreferredListReGet();
	} 
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
//micha0612
		DeleteScreenIfPresent(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
		ExitNETSETShowPreferredListReGet();
	}
	
}


void EntryNETSETConfirmDeleteNetwork(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_GLOBAL_DELETE),IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(NETSETDeletePrefNetworkReq,KEY_EVENT_UP);
}


/**************************************************************

	FUNCTION NAME		: ExitNETSETConfirmDeleteNetwork(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitNETSETConfirmDeleteNetwork(void)
{
	stopRequestedTone(WARNING_TONE);
//micha0531
	GenericExitScreen(SCR_ID_NETSET_CONFIRM_DELETE_NETWORK, EntryNETSETConfirmDeleteNetwork);
}

/**************************************************************

	FUNCTION NAME		: NETSETDeletePrefNetworkReq(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETDeletePrefNetworkReq(void)
{
	mmi_smu_set_preferred_oper_list_req_struct *myMsgPtr;
	MYQUEUE Message;
	U8 reqsrc = 0;
	#ifdef __MMI_MULTI_SIM__
	reqsrc = GetReqSrcModIndex();
	#endif
	ClearAllKeyHandler();
	myMsgPtr = (mmi_smu_set_preferred_oper_list_req_struct*) OslConstructDataPtr(sizeof(mmi_smu_set_preferred_oper_list_req_struct));
	myMsgPtr->opcode = SMU_REMOVE_PLMNSEL;
	myMsgPtr->index = (U8)gPrefListDisplay.CurrSelPrefNetwork;
	memcpy (myMsgPtr->oper,gPrefList.PrefPLMN[gPrefListDisplay.CurrSelPrefNetwork],(MAX_PLMN_LEN + 1));
	
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C+ reqsrc;
	Message.oslMsgId = PRT_SET_PREFERRED_NETWORKS_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	SetProtocolEventHandlerExt(NETSETDeletePrefNetworkRsp,PRT_SET_PREFERRED_NETWORKS_RSP, reqsrc);
	OslMsgSendExtQueue(&Message);
}

/**************************************************************

	FUNCTION NAME		: NETSETDeletePrefNetworkRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETDeletePrefNetworkRsp(void* info)
{
	mmi_smu_set_preferred_oper_list_rsp_struct *myMsgPtr;

	myMsgPtr = (mmi_smu_set_preferred_oper_list_rsp_struct*)info;
	if (myMsgPtr->result == MMI_FALSE){
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
//micha0612
		DeleteScreenIfPresent(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
		ExitNETSETShowPreferredListReGet();
	} 
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
//micha0612
		DeleteScreenIfPresent(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
		ExitNETSETShowPreferredListReGet();
	}
}


/**************************************************************

	FUNCTION NAME		: NETSETGetPreferredNetworkReq(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void NETSETGetPreferredNetworkReq(void)
{
	mmi_smu_get_preferred_oper_list_req_struct *myMsgPtr;
	MYQUEUE Message;
	U8 reqsrc = 0;
	
	#ifdef __MMI_MULTI_SIM__
	reqsrc = GetReqSrcModIndex();
	#endif

	ClearAllKeyHandler();
	if( g_pwr_context[reqsrc].NoOfPLMNSupported == 0)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_NETSET_MAIN);

		return;
	}

	myMsgPtr = (mmi_smu_get_preferred_oper_list_req_struct*) OslConstructDataPtr(sizeof(mmi_smu_get_preferred_oper_list_req_struct));
	myMsgPtr->num = g_pwr_context[reqsrc].NoOfPLMNSupported;
	myMsgPtr->index = 0;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C + reqsrc;
	Message.oslMsgId = PRT_PREFERRED_NETWORKS_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	SetProtocolEventHandlerExt(NETSETGetPreferredNetworkRsp,PRT_PREFERRED_NETWORKS_REQ_SUCCESS,reqsrc);
	OslMsgSendExtQueue(&Message);

}

 

static NETQUEUE *net_queue;
static U8 queue_cnt=0;
static U8 send_cnt=0;

void NETSETModifyNetworkRsp(void *info);

void NETSETModifyNetworkByIndex(void)
{
	mmi_smu_set_preferred_oper_list_req_struct *myMsgPtr;
	MYQUEUE Message;
	U8 reqsrc = 0;

	#ifdef __MMI_MULTI_SIM__
	reqsrc = GetReqSrcModIndex();
	#endif

	myMsgPtr = (mmi_smu_set_preferred_oper_list_req_struct*) OslConstructDataPtr(sizeof(mmi_smu_set_preferred_oper_list_req_struct));
	myMsgPtr->opcode = net_queue[send_cnt].op;
	myMsgPtr->index = net_queue[send_cnt].index;
	memcpy (myMsgPtr->oper, net_queue[send_cnt].plmn, (MAX_PLMN_LEN + 1));
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C + reqsrc;
	Message.oslMsgId = PRT_SET_PREFERRED_NETWORKS_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandlerExt(NETSETModifyNetworkRsp, PRT_SET_PREFERRED_NETWORKS_RSP, reqsrc);
	OslMsgSendExtQueue(&Message);
	send_cnt++;
}

void NETSETModifyNetworkRsp(void *info)
{
	if(send_cnt == queue_cnt)
	{
		OslMfree(net_queue);
		send_cnt = 0;
		queue_cnt = 0;
	}
	else
	{
		NETSETModifyNetworkByIndex();
	}
}

void NETSETModifyNetworkToQueue(U8 op, U8 index, U8* plmn)
{
	net_queue[queue_cnt].op = op;
	net_queue[queue_cnt].index = index;
	memcpy(net_queue[queue_cnt].plmn, plmn, MAX_PLMN_LEN + 1);
	queue_cnt++;
}

/**************************************************************

	FUNCTION NAME		: NETSETGetPreferredNetworkRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NETSETGetPreferredNetworkRsp(void* info)
{
	U8 count;
	U8 index;
	U8 *list_array;
	mmi_smu_get_preferred_oper_list_rsp_struct *myMsgPtr;

	PRINT_INFORMATION(("*** In NETSETGetPreferredNetworkRsp\n"));
	myMsgPtr = (mmi_smu_get_preferred_oper_list_rsp_struct*)info;
	if (myMsgPtr->result == MMI_FALSE) 
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
	
	}
	else
	{
		list_array = OslMalloc(myMsgPtr->retnum);
		net_queue = OslMalloc(myMsgPtr->retnum*2*sizeof(NETQUEUE));

		for (count = 0,gPrefList.PrefListSize=0; count < myMsgPtr->retnum; count++) {
			if (( myMsgPtr->oper_list[count * (MAX_PLMN_LEN + 1)] != 0X3F) &&
				( myMsgPtr->oper_list[count * (MAX_PLMN_LEN + 1)] != 0X00))
			{
				for (index = 0; index < (MAX_PLMN_LEN + 1); index++) {
					//gPrefList.PrefPLMN[count][index] = myMsgPtr->oper_list[index + count * (MAX_PLMN_LEN + 1)];
					gPrefList.PrefPLMN[gPrefList.PrefListSize][index] = myMsgPtr->oper_list[index + count * (MAX_PLMN_LEN + 1)];
				}
				gPrefList.PrefListSize++;

				*(list_array+count) = MMI_TRUE;

				if(count==0)
					continue;

				if( *(list_array+count-1) == MMI_FALSE )
				{
				/* There is a record not in order because its previous record is empty */
					*(list_array+count) = MMI_FALSE;
				/* delete it */
					NETSETModifyNetworkToQueue(SMU_REMOVE_PLMNSEL, count, gPrefList.PrefPLMN[gPrefList.PrefListSize-1]);
				/* add it */
					for(index = 0; index < count; index++)
					{
						if( *(list_array+index) == MMI_FALSE)
							break;
					}
					NETSETModifyNetworkToQueue(SMU_ADD_PLMNSEL, (U8)(gPrefList.PrefListSize-1), gPrefList.PrefPLMN[gPrefList.PrefListSize-1]);
					*(list_array+index) = MMI_TRUE;
				}
			}
			else
			{
				 
				*(list_array+count) = MMI_FALSE;
				//break;
			}
		}

		if(queue_cnt > 0)
			NETSETModifyNetworkByIndex();
		else
			OslMfree(net_queue);

		gPrefListDisplay.PrefNwGotResponse= MMI_TRUE;
		if (!IsScreenPresent(SCR_ID_NETSET_SHOW_PREFERRED_LIST) == ST_SUCCESS) {
			EntryNETSETShowPreferredList();
		}

		OslMfree(list_array);
	}
}


/**************************************************************

	FUNCTION NAME		: GoBackNetworkSetup(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBackNetworkSetup(void)
{
	GoBackToHistory(SCR_ID_NETSET_MAIN);
}


/**************************************************************

	FUNCTION NAME		: ConstructPreferredListHint(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ConstructPreferredListHint(U8 **hintArray)
{
	U8 i;
//micha0901
//	PS8 tempName;

	for(i=0;i<gPrefList.PrefListSize;i++)
	{
		memset((S8 *)hintData[i],0,MAX_SUB_MENU_SIZE);
		AnsiiToUnicodeString((S8*)hintData[i], (S8*)gPrefList.PrefPLMN[i]);
		hintArray[i]=hintData[i];
	}
}

/**************************************************************

	FUNCTION NAME		: EntryNETSETShowPreferredList(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETShowPreferredList(void)
{
	PU8 guiBuffer;
	U8 *PrefListHint[MAX_SUB_MENUS];
	
	EntryNewScreen(SCR_ID_NETSET_SHOW_PREFERRED_LIST, NULL, EntryNETSETShowPreferredList, NULL);
	
	if (gPrefListDisplay.PrefNwGotResponse == MMI_TRUE) {
		guiBuffer = NULL;
	}
	else {
		guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_SHOW_PREFERRED_LIST); 	
	}
	gPrefListDisplay.PrefNwGotResponse = MMI_FALSE;
	GetPrefNwkNameFromPLMN();
	RegisterHighlightHandler(HiliteHandlerPrefList);
	if(!gPrefList.PrefListSize)
	{
		gPrefListDisplay.nStrItemList[0] = (U8*)GetString(STR_GLOBAL_EMPTY_LIST);
		ShowCategory6Screen(STR_ID_NETSET_PRF_NWK_TITLE, MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						1, gPrefListDisplay.nStrItemList, NULL,0,guiBuffer);
	}
	else
	{
		ConstructPreferredListHint(PrefListHint);
		ShowCategory6Screen(STR_ID_NETSET_PRF_NWK_TITLE, MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						gPrefList.PrefListSize, gPrefListDisplay.nStrItemList, PrefListHint, 0,guiBuffer);
	}
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
	SetLeftSoftkeyFunction(EntryNETSETShowPrefNwkOptions,KEY_EVENT_UP);
	SetKeyHandler(EntryNETSETShowPrefNwkOptions, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackNetworkSetup,KEY_EVENT_UP);
	SetKeyHandler(GoBackNetworkSetup, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: GetPrefNwkNameFromPLMN

  	PURPOSE				: This function fills up the array for preferred network name of numbers
						  						                      
	INPUT PARAMETERS	: [1] PU8 *Dispnames,	// Array of names or numbers
						  
	OUTPUT PARAMETERS	: Number of elements in the array

	RETURNS				: U16

 

**************************************************************/

 

/* 
   PLMN number from IMSI might be 5 or 6 digit and it is NOT a null-terminated string.
   The reference comparison length is set to the string table.
   To avoid user input 6-digit PLMN number and first 5-digit is the same as someone in string table,
   a customized function "GetCurrentPLMNName2" is used instead of original one "GetCurrentPLMNName".
*/

PS8 GetCurrentPLMNName2(S8* PlmnNo)
{
	U16 count;

	for(count=0;count<MAX_PLMN_NO_LIST_LENGTH;count++)
	{
		if (strcmp(PlmnNo,sPlmnsToDisplayName[count])==0 )
		{
			count=count+STR_PLMN_NAME_1;
			return GetString(count);
		}
	}
	return NULL;
}
 

U16 GetPrefNwkNameFromPLMN()
{
	U16 count;
	U16 index=0;
	PS8 tempName;
	S8 pOutBuffer[MAX_LENGTH_DISPLAY_NAME*ENCODING_LENGTH];
	
	if (gPrefList.PrefListSize > MAX_PREFFERED_PLMN)
		gPrefList.PrefListSize = MAX_PREFFERED_PLMN;

	for (count = 0; count < MAX_PREFFERED_PLMN; count++)
		gPrefListDisplay.nStrItemList[count] = NULL;
	
		for (count = 0 ; count< gPrefList.PrefListSize; count++, index++) 
		{
			tempName = GetCurrentPLMNName2( (PS8)(gPrefList.PrefPLMN[count]));  
			if ((tempName != NULL) && (pfnUnicodeStrlen((PS8)tempName) < MAX_LENGTH_DISPLAY_NAME)) {
				pfnUnicodeStrcpy((PS8)gPrefListDisplay.PrefNetworkListDislpay[count],tempName);
				gPrefListDisplay.nStrItemList[count] = (PU8) gPrefListDisplay.PrefNetworkListDislpay[count];
			}
			else
			{
				AnsiiToUnicodeString(pOutBuffer, (PS8)(gPrefList.PrefPLMN[count]));
//micha1229
				pfnUnicodeStrcpy((PS8)gPrefListDisplay.PrefNetworkListDislpay[count],(PS8)pOutBuffer);			
				gPrefListDisplay.nStrItemList[count] = (PU8 )gPrefListDisplay.PrefNetworkListDislpay[count];
			}
	}
	return count;
}


//micha0612
/**************************************************************

	FUNCTION NAME		: ExitNETSETShowPreferredListReGet(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitNETSETShowPreferredListReGet(void)
{
	U8 count;
	for(count = 0; count < gPrefList.PrefListSize ; ++count)
		gPrefListDisplay.nStrItemList[count] = NULL;
//micha0531
	GenericExitScreen(SCR_ID_NETSET_SHOW_PREFERRED_LIST, NETSETGetPreferredNetworkReq);
}

/**************************************************************

	FUNCTION NAME		: HiliteHandlerPrefNwkHideAddIfFull(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifndef __MMI_KEEP_ORI_STYLE__
void HiliteHandlerPrefNwkHideAddIfFull(S32 index)
{
	if (( gPrefList.PrefListSize >= MAX_PREFFERED_PLMN) || (gPrefList.PrefListSize >= g_pwr_context[0].NoOfPLMNSupported)) {
			ExecuteCurrHiliteHandler(index+2);
	}
	else {
			ExecuteCurrHiliteHandler(index);
	}
}
#endif

/**************************************************************

	FUNCTION NAME		: EntryNETSETShowPrefNwkOptions(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryNETSETShowPrefNwkOptions(void)
{
	PU8 guiBuffer;
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
		U16		ItemIcons[15];

	EntryNewScreen(SCR_ID_NETSET_SHOW_PREF_NWK_OPTIONS, NULL, EntryNETSETShowPrefNwkOptions, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_SHOW_PREF_NWK_OPTIONS);
	nNumofItem = GetNumOfChild(MITEM_PRF_NWK_OPTIONS_PSEUDO);

//	nDispAttribute = GetDispAttributeOfItem(MITEM_PRF_NWK_OPTIONS_PSEUDO);

	ItemIcons[0] = IMG_GLOBAL_L1;
	ItemIcons[1] = IMG_GLOBAL_L2;
	ItemIcons[2] = IMG_GLOBAL_L3;
	ItemIcons[3] = IMG_GLOBAL_L4;
	GetSequenceStringIds(MITEM_PRF_NWK_OPTIONS_PSEUDO,nStrItemList);
	SetParentHandler(MITEM_PRF_NWK_OPTIONS_PSEUDO);

#ifndef __MMI_KEEP_ORI_STYLE__
	RegisterHighlightHandler(HiliteHandlerPrefNwkHideAddIfFull);	
#else
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#endif

	if(gPrefList.PrefListSize == 0 )
	{
		nNumofItem =2;
	}

#ifndef __MMI_KEEP_ORI_STYLE__
	if (( gPrefList.PrefListSize >= MAX_PREFFERED_PLMN) || (gPrefList.PrefListSize >= g_pwr_context[0].NoOfPLMNSupported)) {
		nNumofItem =2;
		nStrItemList[0] = nStrItemList[2];
		nStrItemList[1] = nStrItemList[3];
	}
#endif

	
	ShowCategory15Screen(STR_GLOBAL_OPTIONS,MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, ItemIcons,
						  LIST_MENU, 0, guiBuffer);
//micha0612
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: IsNetworkDuplicated(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 IsNetworkDuplicated(void)
{
	U8 index;
	S8 pOutBuffer1[ENCODING_LENGTH*(MAX_PLMN_LEN+1)];

	if(gPrefListDisplay.IsNewPrefNetwork==1)
	{
		memset(pOutBuffer1,0,ENCODING_LENGTH*(MAX_PLMN_LEN+1));
		UnicodeToAnsii((PS8)pOutBuffer1,(PS8) gPrefListDisplay.PlmnNew );
		for( index = 0; index < gPrefList.PrefListSize; index++)
		{
			if( (strcmp(pOutBuffer1, (PS8)gPrefList.PrefPLMN[index]) == 0))
			{
				return 1;
			}
		}
	}
	else
	{
		for( index = 0; index < gPrefList.PrefListSize; index++)
		{
			if( (strcmp(sPlmnsToDisplayName[gPrefListDisplay.CurrSelAddPLMN], (PS8)gPrefList.PrefPLMN[index]) == 0))
			{
				return 1;
			}
		}
	}
	return 0;
}

/**************************************************************

	FUNCTION NAME		: HighlightNETSETNewPrefNwk(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNETSETNewPrefNwk(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	ChangeLeftSoftkey(STR_GLOBAL_OK,0);
	SetLeftSoftkeyFunction(EntryNETSETNewNetwork,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntryNETSETNewNetwork, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: HighlightNewEntryHandler(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightNewEntryHandler(S32 index) 
{
	if( gPrefListDisplay.PlmnNew[0] == 0)
		ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK);
    	SetLeftSoftkeyFunction(SendNewPrr,KEY_EVENT_UP);		
}

/**************************************************************

	FUNCTION NAME		: EntryNETSETNewNetwork(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryNETSETNewNetwork(void)
{
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
	
	EntryNewScreen(SCR_ID_NETSET_NEW_NETWORK, ExitNETSETNewNetwork, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_NETSET_NEW_NETWORK);
	inputBuffer = GetCurrNInputBuffer ( SCR_ID_NETSET_NEW_NETWORK, &inputBufferSize );    //added for inline edit history
	
//	nDispAttribute = GetDispAttributeOfItem(MITEM_PRR_NWK_NEW);
	InitializeCategory57Screen();
	FillNewPrefNetworkInlineEditStruct();
	RegisterHighlightHandler(HighlightNewEntryHandler);
	if(inputBuffer!=NULL)
		SetCategory57Data(wgui_inline_items, 4, inputBuffer);
	ShowCategory57Screen(STR_ID_NETSET_PRR_NWK_NEW,MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						4,NULL,wgui_inline_items,
						1,guiBuffer);
	DisableCategory57ScreenDone();
//micha0522
	SetCategory57RightSoftkeyFunctions( SendNewPrr, GoBackHistory);
}


/**************************************************************

	FUNCTION NAME		: ExitNETSETNewNetwork(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitNETSETNewNetwork(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	S16 nHistory = 0;
	U16 inputBufferSize;                //added for inline edit history

	CloseCategory57Screen();     
	Scr.scrnID = SCR_ID_NETSET_NEW_NETWORK;
	Scr.entryFuncPtr = EntryNETSETNewNetwork;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	inputBufferSize = (S16)GetCategory57DataSize( );     //added for inline edit history
	GetCategory57Data ((U8*)Scr.inputBuffer );         //added for inline edit history
	GetCategory57History(Scr.guiBuffer);
	AddNHistory(Scr, inputBufferSize);    
}

/**************************************************************

	FUNCTION NAME		: FillNewPrefNetworkInlineEditStruct(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void FillNewPrefNetworkInlineEditStruct(void) 
{
	memset(gPrefListDisplay.PlmnNew,0,(MAX_PLMN_LEN+1) * ENCODING_LENGTH);

	SetInlineItemActivation(&wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[0], (PU8)(U8 *) GetString(STR_ID_NETSET_PRR_MCC_CAPTION));

	SetInlineItemActivation(&wgui_inline_items[2], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[2], (PU8)(U8 *) GetString(STR_ID_NETSET_PRR_PR_CAPTION));

	SetInlineItemActivation(&wgui_inline_items[1],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemActivation(&wgui_inline_items[3],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);

	SetInlineItemUserDefinedSelect(&wgui_inline_items[3],InLinePriorityDefault,InLinePriorityMinusOne,InLinePriorityAddOne);
	SetInlineItemTextEdit(&wgui_inline_items[1],(PU8)gPrefListDisplay.PlmnNew, MAX_PLMN_LEN +1,INPUT_TYPE_NUMERIC);
	SetInlineItemFullWidth(&wgui_inline_items[1]);
	EnableInlineItemBoundary(&wgui_inline_items[1]);
}

/**************************************************************

	FUNCTION NAME		: InLinePriorityDefault(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8* InLinePriorityDefault(void)
{
	gPrefListDisplay.PriorityVal = gPrefList.PrefListSize+1;
	coolsand_UI_itoa(gPrefListDisplay.PriorityVal,(UI_string_type)gPrefListDisplay.NewNwPriorityDisplay,10);
	return gPrefListDisplay.NewNwPriorityDisplay;
}

/**************************************************************

	FUNCTION NAME		: InLinePriorityMinusOne(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8* InLinePriorityMinusOne(void)
{
	gPrefListDisplay.PriorityVal--;
	if(gPrefListDisplay.PriorityVal <=0)
	{
		gPrefListDisplay.PriorityVal =1;
	}
	coolsand_UI_itoa(gPrefListDisplay.PriorityVal,(UI_string_type)gPrefListDisplay.NewNwPriorityDisplay,10);
	return gPrefListDisplay.NewNwPriorityDisplay;
}

/**************************************************************

	FUNCTION NAME		: InLinePriorityAddOne(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8* InLinePriorityAddOne(void)
{
	if ( gPrefListDisplay.PriorityVal < gPrefList.PrefListSize+1) {
		gPrefListDisplay.PriorityVal++;
		coolsand_UI_itoa(gPrefListDisplay.PriorityVal,(UI_string_type)gPrefListDisplay.NewNwPriorityDisplay,10);
	}
	return gPrefListDisplay.NewNwPriorityDisplay;
}

/**************************************************************

	FUNCTION NAME		: SendNewPrr(void)

  	PURPOSE				: nil
						                      
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SendNewPrr(void)
{
	gPrefListDisplay.IsNewPrefNetwork =1;
	CloseCategory57Screen();     
	if ((gPrefListDisplay.PriorityVal > g_pwr_context[0].NoOfPLMNSupported) || (gPrefListDisplay.PriorityVal > 10 ) ||
		(IsNetworkDuplicated() == 1)||(pfnUnicodeStrlen((PS8)gPrefListDisplay.PlmnNew) < 5) ){
		gPrefListDisplay.IsNewPrefNetwork =0;
		DisplayPopup((PU8)GetString(STR_ID_NETSET_PRF_NWK_ADDED_ERR) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_NETSET_SHOW_PREFERRED_LIST);
				
	}
	else
	{		
		if(gPrefListDisplay.PriorityVal > gPrefList.PrefListSize+1)
		{
			gPrefListDisplay.PriorityVal = gPrefList.PrefListSize+1;
		}
		NETSETAddPrefNetworkReq();
	}

}


/**************************************************************

	FUNCTION NAME		: GoBackPrefList(void)

  	PURPOSE				: function to traverse history back by one

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBackPrefList(void) 
{
	GoBacknHistory(1);
	return;
}


