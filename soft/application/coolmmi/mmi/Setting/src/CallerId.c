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
 *	CallerId.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   <file description>.
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
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: CallerId.c


  	PURPOSE		: Caller id application of call setup

 

 

	DATE		: 11/14/2003

**************************************************************/
#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"
#include "mainmenudef.h"
#include "callsetup.h"
#include "phonebooktypes.h"
#include "protocolevents.h"
#include "debuginitdef.h"
#include "csp.h"
#include "callsetupenum.h"
#include "settinggprots.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "calleridgprot.h"
#include "commonscreens.h"
#include "callmanagementiddef.h"
//micha0915
#include "callmanagementgprot.h"
#include "messagesexdcl.h"
#include "callsdefs.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim_call.h"
#endif
#undef __NEWSIMULATOR



/**************************************************************

	FUNCTION NAME		: InitCallerId(void)

  	PURPOSE				: initialization function of the caller id application.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitCallerId(void)
{
	SetHiliteHandler(MENU_CALLER_ID, HighlightCALLSETCallerId);
	SetHiliteHandler(MENU_CID_SET_NWK, HiliteSetByNwk);
	SetHiliteHandler(MENU_CID_SHOW_ID, HiliteShowCid);
	SetHiliteHandler(MENU_CID_HIDE_ID, HiliteHideCid);
	return;
}
/**************************************************************

	FUNCTION NAME		: InitNvramCallerId(void)

  	PURPOSE				: Initialize NVRAM for callerid

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: S16

	RETURNS				: void

 

**************************************************************/
S16 InitNvramCallerId(void )
{
	S16 error ;
	/* read the current caller id value from nv ram */
	ReadValue (NVRAM_CALLERID, &g_callset_cntx_p->CallerIDInput, DS_BYTE, &error);
	/* if failure in read. set the default value */
	if( g_callset_cntx_p->CallerIDInput == 0xff)
		g_callset_cntx_p->CallerIDInput = 2;
	 
	SetCallerIdReq();
	return 0;
}
/**************************************************************

	FUNCTION NAME		: HiliteSetByNwk(void)

  	PURPOSE				: hilite handler for set by network sub menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteSetByNwk(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->CallerIDInput = CLIR_AUTO;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(SetCallerIdReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallerId(void)

  	PURPOSE				: hilite handler for the caller id option

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallerId(void)
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EnterCALLSETCallerIDMain,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EnterCALLSETCallerIDMain, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HiliteShowCid(void)

  	PURPOSE				: hilite handler for the show caller id.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteShowCid(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->CallerIDInput = CLIR_SUPPRESS;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(SetCallerIdReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HiliteHideCid(void)

  	PURPOSE				: hilite handler for the hide id.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteHideCid(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->CallerIDInput = CLIR_INVOKE;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(SetCallerIdReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: EnterCALLSETCallerIDMain(void)

  	PURPOSE				: Entry screen for the caller id menu items.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EnterCALLSETCallerIDMain(void)
{
	U16 nNumofItem;
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */
	U16 nStrItemList[MAX_SUB_MENUS];
	/*U32 maskingByte=0xFFFFFFFF;
	U32 menuItemId=0xFFFFFFFF;*/

	EntryNewScreen(SCR_ID_CALLSET_CALLER_ID_MAIN, NULL, EnterCALLSETCallerIDMain, NULL);

	/* Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_CALLER_ID_MAIN);	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU_CALLER_ID);  
	/* Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU_CALLER_ID);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU_CALLER_ID, nStrItemList);  
	/* Set current parent id*/
	SetParentHandler(MENU_CALLER_ID);
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

 
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */

	/* Display Category1 Screen */
	ShowCategory15Screen(STR_MENU_CALLER_ID, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK , IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,nNumofItem, nStrItemList,
						  (U16 *)gIndexIconsImageList,1,(g_callset_cntx_p->CallerID+1)%3,guiBuffer);	
	/* Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: SetCallerIdReq(void)

  	PURPOSE				: Sends caller id request to stack

	INPUT PARAMETERS	: callback to register with response

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void SetCallerIdReq(void)
{
	MYQUEUE   Message;
	mmi_ss_set_clir_flag_req_struct *pMsgReq = NULL  ;
	
	static U8 firstTime = 0;
	if (firstTime)
		ClearAllKeyHandler();

	firstTime = 1;

	PRINT_INFORMATION (("*** In MakePsSendClir *** \n"));
	pMsgReq = (mmi_ss_set_clir_flag_req_struct *) 
				OslConstructDataPtr(sizeof(mmi_ss_set_clir_flag_req_struct));
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CALLSET_DEST_MOD;
	pMsgReq->clir_flag = g_callset_cntx_p->CallerIDInput;
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETCallerIDRsp, PRT_MMI_SS_SET_CLIR_FLAG_RSP);
#else
	SetProtocolEventHandler(CALLSETCallerIDRsp, PRT_MMI_SS_SET_CLIR_FLAG_RSP);
#endif
	Message.oslMsgId = PRT_MMI_SS_SET_CLIR_FLAG_REQ;
	Message.oslDataPtr = (oslParaType *)pMsgReq;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	return;
}
/**************************************************************

	FUNCTION NAME		: PsCMakeCallerIdRsp(void)

  	PURPOSE				: response handler for the set caller id request 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETCallerIDRsp(void * info)
{
	mmi_ss_set_clir_flag_rsp_struct *localPtr = NULL ;
	static U8 firstTime = 0;
	S16 error;
	PRINT_INFORMATION (("*** In PsCBackClirSet *** \n"));
	localPtr = (mmi_ss_set_clir_flag_rsp_struct*) info ;
	if (localPtr->result == 1) 
	{
		g_callset_cntx_p->CallerID = g_callset_cntx_p->CallerIDInput;
		if( firstTime)
		{
			if( WriteValue (NVRAM_CALLERID, &g_callset_cntx_p->CallerIDInput, DS_BYTE, & error) < DS_BYTE) {
				PRINT_INFORMATION (("## ERROR: write caler id : WriteRecord "
										"to NVRAM failed (%d)\n", (int) error));
				return;
			}
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,CS_NOTIFYDURATION,SUCCESS_TONE);
			DeleteScreenIfPresent(SCR_ID_CALLSET_CALLER_ID_MAIN);
		}
	}
	else {
		if( firstTime)
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE) ,IMG_GLOBAL_UNFINISHED, 1,CS_NOTIFYDURATION,ERROR_TONE);
			DeleteScreenIfPresent(SCR_ID_CALLSET_CALLER_ID_MAIN);
		}
	}
	firstTime = 1;
	return;
}




/**************************************************************

	FUNCTION NAME		: CALLSETClipRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETClipRsp(void * info)
{
	mmi_ss_clip_interrogate_ind_struct *local;

	memset(g_callset_cntx_p->SSDisplayBuffer,0, MAX_DISP_UCS2);

	local = (mmi_ss_clip_interrogate_ind_struct*)info;
	if (local->result.flag == L4C_OK) 
	{
		if (local->status & SS_PBIT) 
		{
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CLIP_STATUS_ACT_TXT));
		}
		else 
		{
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CLIP_STATUS_NACT_TXT));

		}
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();		
	}
	else 
	{
#ifdef  __MMI_SS_SHOW_CAUSE__
		ShowCauseString( local->result.cause, g_callset_cntx_p->SSDisplayBuffer);
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
#else
		DisplayPopup( (PU8)GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
//micha0915
		RemoveSSReqScr();
#endif
	}

}


/**************************************************************

	FUNCTION NAME		: CALLSETClirRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETClirRsp(void * info)
{
	mmi_ss_clir_interrogate_ind_struct *local;

	memset(g_callset_cntx_p->SSDisplayBuffer,0, MAX_DISP_UCS2);
	local = (mmi_ss_clir_interrogate_ind_struct*)info;

	if (local->result.flag == L4C_OK) 
	{
		if (local->status & SS_PBIT) 
		{
			switch (local->clir_option) 
				{
			case 0:  /* permanently hide */
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CLIR_STATUS_ACT_PERMANENTLY_TXT));
				break;
			case 1:  /* temporary default restrict(hide) */
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CLIR_STATUS_ACT_DEFAULT_RESTRICT_TXT));
				break;
			case 2:  /* temporary default allow(show) */
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CLIR_STATUS_ACT_DEFAULT_ALLOW_TXT));
				break;
			default:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CLIR_STATUS_ACT_TXT));
				break;
			}
		}
		else 
		{
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CLIR_STATUS_NACT_TXT));
		}
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();		
	}
	else 
	{
#ifdef  __MMI_SS_SHOW_CAUSE__
		ShowCauseString( local->result.cause, g_callset_cntx_p->SSDisplayBuffer);
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
#else
		DisplayPopup( (PU8)GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
//micha0915
		RemoveSSReqScr();
#endif
	}
}

/**************************************************************

	FUNCTION NAME		: CALLSETColpRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETColpRsp(void * info)
{
	mmi_ss_colp_interrogate_ind_struct *local;
	memset(g_callset_cntx_p->SSDisplayBuffer,0, MAX_DISP_UCS2);

	local = (mmi_ss_colp_interrogate_ind_struct*)info;
	if (local->result.flag == L4C_OK) 
	{
		if (local->status & SS_PBIT) 
		{
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_COLP_STATUS_ACT_TXT));
		}
		else 
		{
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_COLP_STATUS_NACT_TXT));

		}
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();		
	}
	else 
	{
#ifdef  __MMI_SS_SHOW_CAUSE__
		ShowCauseString( local->result.cause, g_callset_cntx_p->SSDisplayBuffer);
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
#else
		DisplayPopup( (PU8)GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
//micha0915
		RemoveSSReqScr();
#endif
	}

}

/**************************************************************

	FUNCTION NAME		: CALLSETColrRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETColrRsp(void * info)
{
	mmi_ss_colr_interrogate_ind_struct *local;
	memset(g_callset_cntx_p->SSDisplayBuffer,0, MAX_DISP_UCS2);

	local = (mmi_ss_colr_interrogate_ind_struct*)info;
	if (local->result.flag == L4C_OK) 
	{
		if (local->status & SS_PBIT) 
		{
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_COLR_STATUS_ACT_TXT));
		}
		else 
		{
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_COLR_STATUS_NACT_TXT));

		}
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();		
	}
	else 
	{
#ifdef  __MMI_SS_SHOW_CAUSE__
		ShowCauseString( local->result.cause, g_callset_cntx_p->SSDisplayBuffer);
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
#else
		DisplayPopup( (PU8)GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
//micha0915
		RemoveSSReqScr();
#endif
	}
}
/**************************************************************
	
	FUNCTION NAME		: CALLSETCnapRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETCnapRsp(void * info)
{
	mmi_ss_cnap_interrogate_ind_struct *local;
	memset(g_callset_cntx_p->SSDisplayBuffer,0, MAX_DISP_UCS2);
	local = (mmi_ss_cnap_interrogate_ind_struct*)info;

	if (local->result.flag == L4C_OK) {
		if (local->status & SS_PBIT) {
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CNAP_STATUS_ACT_TXT));
		}
		else {
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CNAP_STATUS_NACT_TXT));
		}
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();		
	}
	else {
#ifdef  __MMI_SS_SHOW_CAUSE__
		ShowCauseString( local->result.cause, g_callset_cntx_p->SSDisplayBuffer);
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
#else
		DisplayPopup( (PU8) GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
//micha0915
		RemoveSSReqScr();
#endif
	}
}

/**************************************************************

	FUNCTION NAME		: CALLSETCcbsQueryRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETCcbsQueryRsp(void * info)
{
	mmi_ss_ccbs_rsp_struct *local;
	memset(g_callset_cntx_p->SSDisplayBuffer,0, MAX_DISP_UCS2);
	local = (mmi_ss_ccbs_rsp_struct*)info;

	if (local->result.flag == L4C_OK) {
		if (local->list.ss_status & SS_PBIT) {
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CCBS_STATUS_ACT_TXT));
		}
		else {
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CCBS_STATUS_NACT_TXT));
		}
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();		
	}
	else {
#ifdef  __MMI_SS_SHOW_CAUSE__
		ShowCauseString( local->result.cause, g_callset_cntx_p->SSDisplayBuffer);
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
#else
		DisplayPopup( (PU8) GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
//micha0915
		RemoveSSReqScr();
#endif
	}
}


//micha1228
/**************************************************************

	FUNCTION NAME		: CALLSETEmlppQueryRsp(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETEmlppQueryRsp(void * info)
{
	mmi_ss_emlpp_rsp_struct *local;
	S8 TempBuff[50];
	
	memset(g_callset_cntx_p->SSDisplayBuffer,0, MAX_DISP_UCS2);
	memset( TempBuff, 0, sizeof(TempBuff));		
	local = (mmi_ss_emlpp_rsp_struct*)info;

	if (local->result.flag == L4C_OK) {
		
		sprintf( TempBuff, "Default: %d\nMax: %d", local->default_pri, local->max_pri);

		AnsiiToUnicodeString((PS8) g_callset_cntx_p->SSDisplayBuffer, (PS8)TempBuff);
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();		
	}
	else {
#ifdef  __MMI_SS_SHOW_CAUSE__
		ShowCauseString( local->result.cause, g_callset_cntx_p->SSDisplayBuffer);
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
#else
		DisplayPopup( (PU8) GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
//micha0915
		RemoveSSReqScr();
#endif
	}
}


