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
 *  Modification Notice:
 *  --------------------------
 *  This software is modified by COOLSAND Inc. and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of COOLSAND Inc. (C) 2001
 *
 *******************************************************************************/

 /*******************************************************************************
 * Filename:
 * ---------
 *   PsCallHelper.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  .......
 *
 * Author:
 * -------
 * -------
 *
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/CallManagement/src/PsCallHelper.c-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:02   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:37:30   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:23:46   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:32:54   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 19:10:52   zhoumn
 * No change.
 * 
 *    Rev 1.1   Aug 28 2006 17:38:44   zhangxb
 * 合并了PSI和Simon修改的一些代码.
 * 
 *    Rev 1.0   Jul 06 2006 17:42:38   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Jul 04 2006 22:56:48   zhangx
 *
 *
 *******************************************************************************/

/**************************************************************

	FILENAME	: PsCallHelper.c

  	PURPOSE		: Call Management application

 

	AUTHOR		: Subramanya BG

	DATE		: Nov 12,02

**************************************************************/
#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "queuegprot.h"
#include "commonscreens.h"
#include "wgui_categories.h"
#include "wgui_status_icons.h"
#include "keybrd.h"
#include "gsm7bitdefaultalphabet.h"

#include "settingsgexdcl.h"
#include "callsetup.h"
#include "managecallmain.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#ifdef __MMI_SOUND_RECORDER__
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "soundrecorderdef.h"
#include "soundrecorderprot.h"
#endif

#include "callmanagementiddef.h"
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
#include "callmanagementgprot.h"
#include "commonstubsprot.h"
#include "cmerrortypes.h"
#include "cmerrorhandlersprot.h"
#include "pscallhelperdef.h"
#include "incomingcallmanagementprot.h"
#include "outgoingcallhelperprot.h"
#include "outgoingcallmanagementprot.h"
#include "mmi.h"   //tangjie add begin 20060708
#include "pscallhelpergprot.h"
#include "helpdcl.h"


#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#include "profilegprots.h"
#endif
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "simdetectiongprot.h"
#include "gpioinc.h"

#undef __NEWSIMULATOR

S8  gUSSDbuf[MAX_DIGITS_USSD*ENCODING_LENGTH];

extern U16 gSSEvent;

// wangbei del start 20060614
#if 0
void mmi_frm_sms_delete_action_pending(void)
{
}
#endif
// wangbei del end
/*  
void TurnOnBacklight(U8 TimerFlag)
{
}
*/
U16 GetCurrScreenID(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gCurrScreenID;
}

// zrx add end

/**************************************************************

	FUNCTION NAME		: MakePsInitiateCall(U8* MsgStruct, void* callBack)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request outgoing call

	INPUT PARAMETERS	: U8*(This contains Telephone number of the outgoing call)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsInitiateCall(U8* MsgStruct, void* callBack)
{
   U16 length;
   U8 IPNum[MAX_IP_NUMBER_LEN+1] = {0};

	mmi_trace(TRUE,"MMI_Call: Func: %s MsgStruct=0x%x : %s", __FUNCTION__,MsgStruct, MsgStruct);
	if (MsgStruct)
	{
		ClearKeyEvents();
		SetDialIndication(FALSE);
		memset ((PS8)IPNum, 0, MAX_IP_NUMBER_LEN+1);

#ifdef __IP_NUMBER__
		if (GetTempUseIPNumber())
		{
			U8 nsimid = 0;
			#ifdef __MMI_MULTI_SIM__
			nsimid = MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL);
			#endif
			GetCurrIPNumber(IPNum, nsimid);
			mmi_trace(TRUE,"MMI_Call Func: %s IPNum=%s", __FUNCTION__, IPNum);	
		}
#endif

		length = strlen((PS8)MsgStruct);
		if(memcmp(MsgStruct, "+86", 3) == 0)
		{	
			MakePsSSVUSSDReq(MsgStruct + 3,(PU8)IPNum,length - 3,(void*)PsCBackSetCallFeatures);
		}
		else
		{
			MakePsSSVUSSDReq(MsgStruct,(PU8)IPNum,length,(void*)PsCBackSetCallFeatures);
		}
	}
	return;
}

BOOL MakePsCheckIfEmergencyNo(U8 *Ascii_num)
{
	BOOL IsEmergencyNo = FALSE;
	UINT8 UnicodeNum[MAX_DIGITS_USSD * ENCODING_LENGTH] = {0};

	AnsiiToUnicodeString(UnicodeNum, Ascii_num);

	if (CheckValidEmergencyNo1(UnicodeNum))
	{
		IsEmergencyNo = TRUE; 
	}

	return IsEmergencyNo;
}


/**************************************************************

	FUNCTION NAME		: MakePsSSVUSSDReq(U8* MsgStruct, void* callBack)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request outgoing call

	INPUT PARAMETERS	: U8*(This contains Telephone number of the outgoing call)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsSSVUSSDReq(U8* MsgStruct, U8* IPNum, U16 length, void* callBack)
{
	MYQUEUE   Message;
	MMI_PARSE_STR* strParse;

	ClearAllKeyHandler();

	/* PARSE_SS_REQ */
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_USSDVSS_REQ;
	Message.oslPeerBuffPtr= NULL;

	if (MsgStruct)
	{
		strParse = (MMI_PARSE_STR*) OslConstructDataPtr(sizeof(MMI_PARSE_STR));
		memset(strParse->input, 0, MAX_DIGITS_USSD);
		strParse->length =(U8)length;
		memcpy (strParse->input, MsgStruct, strParse->length);
		strParse->dcs = 0x0f;
		memcpy (strParse->ip_string, IPNum, sizeof (strParse->ip_string));
		strParse->IsEmergencyNo = MakePsCheckIfEmergencyNo(strParse->input); //check emergency number
		Message.oslDataPtr = (oslParaType *)strParse;


	}
	else
		Message.oslDataPtr = NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_USSDVSS_RSP, MTPNP_AD_CALL_CHANNEL);
#else  /* No Defined __MMI_MULTI_SIM__ */
	SetProtocolEventHandler((PsFuncPtr)callBack, PRT_USSDVSS_RSP);
#endif /* __MMI_MULTI_SIM__ */
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsCallAcceptIncoming(void*)

  	PURPOSE				: Makes call to Protcol Stack to Accept Incoming Call

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MakePsCallAcceptIncoming(void* callBack)
{
	MYQUEUE	Message;
	FuncPtr fptrKeyEnd,fptrKeyRsk;

   /* ATA_REQ */
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_INCOMINGCALL_ACCEPT_EVENT;
   Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr= NULL;

   mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
   /* preserve END and RSK key handler */
	fptrKeyEnd=GetKeyHandler(KEY_END,KEY_EVENT_DOWN);
	fptrKeyRsk=GetKeyHandler(KEY_RSK,KEY_EVENT_UP);
	ClearAllKeyHandler();
	SetKeyHandler(fptrKeyEnd,KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(fptrKeyRsk,KEY_RSK,KEY_EVENT_UP);

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_INCOMINGCALL_ACCEPT_SUCCESS, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler((PsFuncPtr)callBack,
                           PRT_INCOMINGCALL_ACCEPT_SUCCESS);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsAthRequest(void)

  	PURPOSE				: Sends ECT request to Protocol stack

	INPUT PARAMETERS	: callback to register with response

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsAthRequest(void* callBack)
{
	MYQUEUE Message;
	MMI_ATH_REQ *athReq;

    
#ifdef __MMI_CM_BLACK_LIST__
   /* if auto-rejected, the flag will be false and it should not clear key handler */
   if (GetClearKeyFlag()==TRUE)
      	ClearAllKeyHandler();
   else
   	SetClearKeyFlag(TRUE);
#else
   ClearAllKeyHandler();
#endif
   

   /* ATH_REQ */
   Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_ATH_REQ;
	Message.oslPeerBuffPtr= NULL;
	athReq = (MMI_ATH_REQ*) OslConstructDataPtr(sizeof(MMI_ATH_REQ));
     Message.oslDataPtr = (oslParaType *)athReq;
      
	if (GetCallAbortReqSentFlag())
	{
		/* set abort MO flag */
		athReq->op_code = L4C_DISCONNECT_MO;
	}
	else
	{
		athReq->op_code = L4C_DISCONNECT_NONE;
	}

	if (GetCurrentState() == CM_INCOMING_STATE)
		SetAllCallsHangupFlag();
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_ATH_REQ_SUCCESS, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_ATH_REQ_SUCCESS);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}
 
/**************************************************************

	FUNCTION NAME		: MakePsReleaseCompleteRequest

  	PURPOSE				: Send relese complete message to L4C to strictly releasae the disconnecting call

	INPUT PARAMETERS	:  

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsReleaseCompleteRequest(CM_CALL_HANDLE handle)
{
	MYQUEUE Message;
	MMI_RELCOMP_REQ *relcompReq;

	ClearAllKeyHandler();
   
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_RELCOMP_EVENT;
	relcompReq = (MMI_RELCOMP_REQ*) OslConstructDataPtr(sizeof(MMI_RELCOMP_REQ));
     	Message.oslDataPtr = (oslParaType *)relcompReq;
     	relcompReq->call_id =(U8) handle;
      	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	return;
}
 
/**************************************************************

	FUNCTION NAME		: MakePsSendUDUB(void*)

  	PURPOSE				: Makes call to Protcol Stack to Send User Determined User Busy

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MakePsSendUDUB(void* callBack)
{
	MYQUEUE Message;
	MMI_CC_CHLD_REQ* udubReq;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsSendUDUB));
	 
#ifdef __MMI_CM_BLACK_LIST__
	/* if auto-rejected, the flag will be false and it should not clear key handler */
	if (GetClearKeyFlag()==TRUE)
		ClearAllKeyHandler();
	else
		SetClearKeyFlag(TRUE);
#else
	ClearAllKeyHandler();
#endif
	 
	SetChldReqSent(CM_UDUB_REQ_SENT);

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_UDUB_REQ;

	udubReq = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	udubReq->opcode = CSMCC_REL_HELD_OR_UDUB;

	Message.oslDataPtr = (oslParaType *)udubReq;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_UDUB_RES_SUCCESS, MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_UDUB_RES_SUCCESS);
	SetProtocolEventHandler ((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsHangupallActiveRequest(void)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request the Hangup of all Active calls

	INPUT PARAMETERS	: void(request to drop all Active calls)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsHangupallActiveAcceptOtherRequest(void* callBack)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ* hangUp;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsHangupallActiveAcceptOtherRequest));

	ClearAllKeyHandler();
	SetChldReqSent(CM_HANGUPALLACTIVE_REQ_SENT);

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_ENDALLACTIVE_EVENT;

	hangUp = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	hangUp->opcode = CSMCC_REL_ACTIVE_AND_ACCEPT;

	Message.oslDataPtr = (oslParaType *)hangUp;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_ENDALLACTIVEREQ_SUCCESS, MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)CBackHangupAllActiveAcceptOther, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_ENDALLACTIVEREQ_SUCCESS);
	SetProtocolEventHandler ((PsFuncPtr)CBackHangupAllActiveAcceptOther,
	                         PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsEndSelectiveCall(void*, U16)

  	PURPOSE				: Makes call to Protcol Stack to end a Selective Active Call

	INPUT PARAMETERS	: void*, U16

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
	                      The handle describes the call to end

**************************************************************/
void MakePsEndSelectiveCall(void* callBack, CM_CALL_HANDLE handle)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ *hangupReq;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsEndSelectiveCall, handle));

	ClearAllKeyHandler();
	SetChldReqSent(CM_ENDSELECTIVE_REQ_SENT);

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId  = PRT_CALLENDSPECIFIC_EVENT;

	if (GetCurrentState () == CM_OUTGOING_STATE)
	{
		SetKeyHandler(DropRequest,KEY_LSK, KEY_EVENT_UP);
		SetKeyHandler(DropRequest,KEY_END, KEY_EVENT_DOWN);
	}
	hangupReq = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	if (GetCurrentState() != CM_OUTGOING_STATE &&
	    GetCallState(handle) == CM_ACTIVE_STATE)
	   hangupReq->opcode = CSMCC_REL_SPECIFIC_ACTIVE_CALL;
	else
	   hangupReq->opcode = CSMCC_REL_SPECIFIC_CALL;
	hangupReq->call_id =(U8) handle;

	Message.oslDataPtr = (oslParaType *)hangupReq;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_CALLENDSPECIFIC_SUCCESS, MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_CALLENDSPECIFIC_SUCCESS);
	SetProtocolEventHandler ((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsActiveHold(void*)

  	PURPOSE				: Makes call to Protcol Stack to Retrieve Held Call

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MakePsActiveHold(void* callBack)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ* retrReq;

	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	
	ClearAllKeyHandler();

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId  = PRT_RETRIEVECALL_EVENT;

	retrReq = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	retrReq->opcode = CSMCC_HOLD_ACTIVE_AND_ACCEPT;
	Message.oslDataPtr = (oslParaType *)retrReq;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsSplitRequest(CM_CALL_HANDLE nCallHandle)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request the split of particular call in a group of active calls.

	INPUT PARAMETERS	: CM_CALL_HANDLE(request is made by sending a particular call's call handle)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsSplitRequest(CM_CALL_HANDLE nCallHandle)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ* splitReq;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsSplitRequest, nCallHandle));

	ClearAllKeyHandler();

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId  = PRT_SPLITCALL_EVENT;

	splitReq = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	splitReq->opcode = CSMCC_HOLD_ACTIVE_EXCEPT_SPECIFIC_CALL;
	splitReq->call_id =(U8) nCallHandle;

	Message.oslDataPtr = (oslParaType *)splitReq;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)SplitReqSucess, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (SplitReqSucess, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsConfCallRequest(void)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request the Conference of the calls

	INPUT PARAMETERS	: void(request to Conference i.e all the calls become active)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsConfCallRequest(void)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ* confReq;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsConfCallRequest));

	ClearAllKeyHandler();

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_CONFCALL_EVENT;

	confReq = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	confReq->opcode = CSMCC_ADD_HELD_CALL;

	Message.oslDataPtr = (oslParaType *)confReq;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)ConfReqSucess, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (ConfReqSucess, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsEctRequest(void)

  	PURPOSE				: Sends ECT request to Protocol stack

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsEctRequest(void)
{
	MYQUEUE Message;
	MMI_CC_CHLD_REQ* ectReq;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsEctRequest));

	ClearAllKeyHandler();

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId= MMI_CC_DEST_MOD;
   Message.oslMsgId = PRT_ECT_REQ;

	ectReq = (MMI_CC_CHLD_REQ*)OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	ectReq->opcode = CSMCC_EXPLICIT_CALL_TRANSFER;

	Message.oslDataPtr = (oslParaType *)ectReq;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)EctReqSucess, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (EctReqSucess, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsActivateCCBS(void*)

  	PURPOSE				: Makes call to Protcol Stack to Activate CCBS

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MakePsActivateCCBS(void* callBack)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ * ccbsReq;

   mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
   ClearAllKeyHandler();

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_CCBS_ACTIVATE_REQ;

	ccbsReq = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	ccbsReq->opcode = CSMCC_ACTIVATE_CCBS_CALL;

	Message.oslDataPtr = (oslParaType *)ccbsReq;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_END_CHLD_RSP);
#endif
   OslMsgSendExtQueue(&Message);
   return;
}

/**************************************************************

	FUNCTION NAME		: MakePsHangupallRequest(void)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request the Hangup of all calls

	INPUT PARAMETERS	: void(request to drop all the calls)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsHangupallRequest(void* callBack)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ* hangUp;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsHangupallRequest));

	ClearAllKeyHandler();
	SetChldReqSent(CM_HANGUPALL_REQ_SENT);

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_ENDALLCALL_EVENT;

	hangUp = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	hangUp->opcode = CSMCC_REL_ALL_EXCEPT_WAITING_CALL;

	Message.oslDataPtr = (oslParaType *)hangUp;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_ENDALLCALLREQ_SUCCESS, MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_ENDALLCALLREQ_SUCCESS);
	SetProtocolEventHandler ((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsSwapCallRequest(void)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request the Swap of the calls

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsSwapCallRequest(void* callBack)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ* swapReq;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsSwapCallRequest));

	ClearAllKeyHandler();

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId  = PRT_SWAPCALL_EVENT;

	swapReq = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	swapReq->opcode = CSMCC_SWAP_CALL;

	Message.oslDataPtr = (oslParaType *)swapReq;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsSendRelHeld(void)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request the Release of the held calls

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsSendRelHeld(void* callBack)
{
	MYQUEUE Message;
	MMI_CC_CHLD_REQ* relHeldReq;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsSendRelHeld));

	ClearAllKeyHandler();
	SetChldReqSent(CM_UDUB_REQ_SENT);

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_UDUB_REQ;

	relHeldReq = (MMI_CC_CHLD_REQ*)
	               OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	relHeldReq->opcode = CSMCC_REL_HELD;

	Message.oslDataPtr = (oslParaType *)relHeldReq;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_UDUB_RES_SUCCESS, MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_UDUB_RES_SUCCESS);
	SetProtocolEventHandler ((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsHangupallActiveRequest(void)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request the Hangup of all Active calls

	INPUT PARAMETERS	: void(request to drop all Active calls)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsHangupallActiveRequest(void* callBack)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ* hangUp;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsHangupallActiveRequest));

	ClearAllKeyHandler();
	SetChldReqSent(CM_HANGUPALLACTIVE_REQ_SENT);

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_ENDALLACTIVE_EVENT;

	hangUp = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	hangUp->opcode = CSMCC_REL_ACTIVE;

	Message.oslDataPtr = (oslParaType *)hangUp;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_ENDALLACTIVEREQ_SUCCESS, MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)CBackHangupAllActive, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_ENDALLACTIVEREQ_SUCCESS);
	SetProtocolEventHandler ((PsFuncPtr)CBackHangupAllActive, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsHangupallCallsRequest(void* callBack)

  	PURPOSE				: Helper function to Invoke Stack function or pass
						  the msg structure that can be understandable to Protocol stack.
						  This is the stack function call to request the Hangup of all calls

	INPUT PARAMETERS	: void(request to drop all the calls)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsHangupallCallsRequest(void* callBack)
{
	MYQUEUE	Message;
	MMI_CC_CHLD_REQ* hangUp;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsHangupallCallsRequest));

	ClearAllKeyHandler();
	SetChldReqSent(CM_HANGUPALL_REQ_SENT);

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_ENDALLCALL_EVENT;

	hangUp = (MMI_CC_CHLD_REQ*) OslConstructDataPtr(sizeof (MMI_CC_CHLD_REQ));
	hangUp->opcode = CSMCC_REL_ALL;

	Message.oslDataPtr = (oslParaType *)hangUp;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_ENDALLCALLREQ_SUCCESS, MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);

#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_ENDALLCALLREQ_SUCCESS);
	SetProtocolEventHandler ((PsFuncPtr)CheckFailureChld, PRT_END_CHLD_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsCallDeflect(void)

  	PURPOSE				: Makes call to Protcol Stack to deflect incoming Call

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MakePsCallDeflect(void* callBack,PU8 CallDeflectNumber)
{
	MYQUEUE	Message;
	MMI_CALL_DEFLECT_REQ * calldeflect;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsCallDeflect));

	ClearAllKeyHandler();

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_CALL_DEFLECT_REQ;

	calldeflect = (MMI_CALL_DEFLECT_REQ*)
	                  OslConstructDataPtr(sizeof (MMI_CALL_DEFLECT_REQ));

	if(CallDeflectNumber[0] == '+')
	{
		calldeflect->dest.type = CSMCC_INTERNATIONAL_ADDR;
		strcpy((S8*)calldeflect->dest.number, (S8*)CallDeflectNumber+1);
   }
	else
	{
		calldeflect->dest.type = CSMCC_DEFAULT_ADDR_TYPE;
		strcpy((S8*)calldeflect->dest.number, (S8*)CallDeflectNumber);
	}

	calldeflect->dest.length = strlen((PS8)calldeflect->dest.number);
	calldeflect->sub_addr_flag =KAL_FALSE ;
	memset((void*)&(calldeflect->sub_dest),0,sizeof(calldeflect->sub_dest));

	Message.oslDataPtr = (oslParaType *)calldeflect;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_CALL_DEFLECT_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack, PRT_CALL_DEFLECT_RSP);
#endif
   OslMsgSendExtQueue(&Message);
   return;
}

/**************************************************************

	FUNCTION NAME		: MakePsGetCallList

  	PURPOSE				: Makes call to Protcol Stack to get call list

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsGetCallList(void* callBack)
{
	MYQUEUE Message;

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PTR_GET_CALL_LIST_REQ;
	Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PTR_GET_CALL_LIST_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler ((PsFuncPtr)callBack,PTR_GET_CALL_LIST_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsDropUSSDReq(void)

  	PURPOSE				: Aborts SS string request

	INPUT PARAMETERS	: callback to register with response

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsDropUSSDReq(PsFuncPtr callBack)
{
	MYQUEUE Message;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsDropUSSDReq));

 
	if (callBack != NULL)
	ClearAllKeyHandler();

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId= MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_SS_STRING_ABORT_REQ;
	Message.oslPeerBuffPtr= NULL;
	Message.oslDataPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)callBack, PRT_SS_STRING_ABORT_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (callBack, PRT_SS_STRING_ABORT_RSP);
#endif
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsSendUSSDReq(void)

  	PURPOSE				: Send SS string request

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsSendUSSDReq(S8 *pSrc)
{
	MYQUEUE Message;
	mmi_ss_parsing_string_req_struct *pUSSDReq;
	
	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsDropUSSDReq));

	ASSERT(pSrc != NULL);

	ClearAllKeyHandler();

	pUSSDReq = (mmi_ss_parsing_string_req_struct*) OslConstructDataPtr(sizeof(mmi_ss_parsing_string_req_struct));
	memset(pUSSDReq, 0, sizeof(mmi_ss_parsing_string_req_struct));
	
	pUSSDReq->dcs = 0x0F;
	pUSSDReq->length = strlen(pSrc);
	memcpy(pUSSDReq->input, pSrc, pUSSDReq->length + 1);
		
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId= MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_SS_STRING_SEND_USSD_REQ;
	Message.oslPeerBuffPtr= NULL;
	Message.oslDataPtr= (oslParaType *)pUSSDReq;

	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsSendDTMFTone

  	PURPOSE				: Helper function to start sending a dtmf tone

	INPUT PARAMETERS	: U8* This contains digit to send to pstack

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsSendDTMFTone(U16* MsgStruct)
{
	if (MsgStruct)
	{
	   MYQUEUE Message;
	   MMI_DTMF_DIGIT* dtmf;

     		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsSendDTMFTone));

	   Message.oslSrcId=MOD_MMI;
	   Message.oslDestId=MMI_CC_DEST_MOD;
	   Message.oslMsgId = PRT_DTMF_START_REQ;

		dtmf = (MMI_DTMF_DIGIT*) OslConstructDataPtr (sizeof(MMI_DTMF_DIGIT));
		dtmf->digit = MapDtmfEnum2Ascii(*MsgStruct);// the actuall digit

     		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_MakePsSendDTMFTone, dtmf->digit));

		Message.oslDataPtr = (oslParaType *)dtmf;
		Message.oslPeerBuffPtr= NULL;
	   OslMsgSendExtQueue(&Message);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsStopDTMFTone

  	PURPOSE				: Helper function to stop dtmf tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsStopDTMFTone(void)
{
	MYQUEUE Message;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsStopDTMFTone));

   Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_DTMF_STOP_REQ;

	Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsSendDTMFString

  	PURPOSE				: Helper function to start sending a dtmf tone

	INPUT PARAMETERS	: U8* This contains digit to send to pstack

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsSendDTMFString(U8* MsgStruct)
{
	MYQUEUE Message;
	MMI_DTMF_CONFIRM_REQ* dtmf;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsSendDTMFString));

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_DTMF_CONFIRM_REQ;

   dtmf = (MMI_DTMF_CONFIRM_REQ*)
            OslConstructDataPtr (sizeof(MMI_DTMF_CONFIRM_REQ));
	strcpy((void *)dtmf->digit,(void *)MsgStruct);

	Message.oslDataPtr = (oslParaType *)dtmf;
	Message.oslPeerBuffPtr= NULL;

	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsReverseSpeechStatus

  	PURPOSE				: Makes call to Protcol Stack to send USSD data to n/w

	INPUT PARAMETERS	: void*, void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
	                      info describes the data to transparently send to n/w

**************************************************************/
void MakePsReverseSpeechStatus(pBOOL status)
{
   BOOL speechOn;
   	mmi_trace(g_sw_BT," MakePsReverseSpeechStatus  ");
    
#if !(defined(__MMI_BT_HP_SUPPORT__)|| defined(__MMI_BT_SUPPORT__))
    if( status == TRUE)
    {
        mdi_audio_set_speech_mode(KAL_TRUE);   	
    }
    else
    {
        mdi_audio_set_speech_mode(KAL_FALSE);
    }
    return;
#endif

	if (status == TRUE)
	{
		if (GetInternalAlert() == TRUE)
		{
			StopInternalOutgoingTone();
		}
		speechOn = KAL_TRUE;
	}
	else
	{
		speechOn = KAL_FALSE;
		if(IsPhoneInMute())
	   {
		   UnMutethePhone();
	   }
	}
#ifdef  __MMI_BT_SUPPORT__

       /* Turn on/off BT speech channel */
	mmi_trace(g_sw_BT," MakePsReverseSpeechStatus speechOn %d ",speechOn);
	mmi_trace(g_sw_BT," MakePsReverseSpeechStatus mmi_bt_is_aud2hf() %d ",mmi_bt_is_aud2hf());
       if( KAL_TRUE == speechOn)
       {
            if( mmi_bt_is_aud2hf() == MMI_TRUE /*&& mmi_bt_is_answer_by_ag() == MMI_FALSE */)
            {
	//            mmi_bt_turn_on_speech_req(KAL_TRUE);
            }

            mdi_audio_set_speech_mode(KAL_TRUE);
		#ifdef __MMI_BT_PROFILE__
          mmi_profiles_bt_call_start_callback();
             #endif /* __MMI_BT_PROFILE__ */ 
   	
      }
       else
      {
            mdi_audio_set_speech_mode(KAL_FALSE);

            if( mmi_bt_is_aud2hf() == MMI_TRUE || mmi_bt_is_incall_aud_swap2bt() == MMI_TRUE)
	            mmi_bt_turn_on_speech_req(MMI_FALSE);
			
			#ifdef __MMI_BT_PROFILE__	
		mmi_profiles_bt_call_end_callback();	
		  #endif /* __MMI_BT_PROFILE__ */ 	
      }

#else   
   mdi_audio_set_speech_mode(speechOn);
#endif       
	return;
}

/**************************************************************

	FUNCTION NAME		: MakePsStartVoiceRecord(void)

  	PURPOSE				: Sends start voice record request to Protocol stack

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsStartVoiceRecord(void)
{
#ifdef MMI_ON_HARDWARE_P

#ifdef __MMI_SOUND_RECORDER__
	U8 file_name[60];

	mmi_sndrec_get_new_file_name((UI_string_type)file_name);
	mdi_audio_start_record((void*)file_name, MDI_FORMAT_GSM_EFR, NULL, (mdi_callback)StartVoiceRecordReqSucess);
	return;
#endif

#ifdef __MMI_VOICE_MEMO__
	{
	MYQUEUE	Message;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsStartVoiceRecord));

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_START_VOICERECORD_REQ;
	Message.oslPeerBuffPtr= NULL;
	Message.oslDataPtr= NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)StartVoiceRecordReqSucess, PRT_START_VOICERECORD_RSP, MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)VoiceRecordFinishHandler, PRT_VOICERECORD_FINISH_IND, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (StartVoiceRecordReqSucess, PRT_START_VOICERECORD_RSP);
   	SetProtocolEventHandler (VoiceRecordFinishHandler, PRT_VOICERECORD_FINISH_IND);
#endif
	OslMsgSendExtQueue(&Message);
	}
#endif

#endif
}

/**************************************************************

	FUNCTION NAME		: MakePsStopVoiceRecord(void)

  	PURPOSE				: Sends stop voice record request to Protocol stack

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsStopVoiceRecord(void)
{
#ifdef MMI_ON_HARDWARE_P

#ifdef __MMI_SOUND_RECORDER__
	mdi_audio_stop_record();
	return;
#endif

#ifdef __MMI_VOICE_MEMO__
	{
	MYQUEUE	Message;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsStopVoiceRecord));

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CC_DEST_MOD;
	Message.oslMsgId = PRT_STOP_VOICERECORD_REQ;
	Message.oslPeerBuffPtr= NULL;
	Message.oslDataPtr= NULL;

	/*SetProtocolEventHandler (StopVoiceRecordReqSucess,
	                         PRT_STOP_VOICERECORD_RSP);*/
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)NULL, PRT_VOICERECORD_FINISH_IND, MTPNP_AD_CALL_CHANNEL);
#else 
	SetProtocolEventHandler (NULL, PRT_VOICERECORD_FINISH_IND);
#endif
	OslMsgSendExtQueue(&Message);
	}
#endif
#endif
}

/**************************************************************

	FUNCTION NAME		: StartVoiceRecordReqSucess(void* MsgStruct)

  	PURPOSE				: gets the response of Start Voice Record

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  without any structure.
**************************************************************/
void StartVoiceRecordReqSucess(void* MsgStruct)
{
	S32 result;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_StartVoiceRecordReqSucess));

	result = (S32)MsgStruct;

#ifdef __MMI_SOUND_RECORDER__
	if (result == MDI_AUDIO_SUCCESS)
	{
   		SetVoiceRecordingFlag(TRUE);
   		ShowStatusIcon(STATUS_ICON_VOICE_RECORD);
   		UpdateStatusIcons();
	}
	else if (result == MDI_AUDIO_TERMINATED)
	{
		/* do nothing */
	}
	else if (result == MDI_AUDIO_DISC_FULL)
	{
		VoiceRecordFinishHandler(NULL);
		DisplayPopup((U8*)GetString((U16)mmi_sndrec_get_res_string((U8)result)),
			IMG_GLOBAL_UNFINISHED, 0,
			2000, ERROR_TONE_IN_CALL);
	}
	else
	{
		DisplayPopup((U8*)GetString((U16)mmi_sndrec_get_res_string((U8)result)),
			IMG_GLOBAL_UNFINISHED, 0,
			2000, ERROR_TONE_IN_CALL);
	}
	return;
#else
	{
   mmi_vm_record_rsp_struct *rsp = (mmi_vm_record_rsp_struct*)MsgStruct;

   if (rsp->result)
   {
   	SetVoiceRecordingFlag(TRUE);
   	ShowStatusIcon(STATUS_ICON_VOICE_RECORD);
   	UpdateStatusIcons();
   }
	}
#endif
}

/**************************************************************

	FUNCTION NAME		: VoiceRecordFinishHandler(void* MsgStruct)

  	PURPOSE				: gets the indication of Voice Record has completed

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  without any structure.
**************************************************************/
void VoiceRecordFinishHandler(void* MsgStruct)
{
	SetVoiceRecordingFlag(FALSE);
	HideStatusIcon(STATUS_ICON_VOICE_RECORD);
	UpdateStatusIcons();
}

/**************************************************************

	FUNCTION NAME		: DeriveAndSetCallFeatures

  	PURPOSE				: Function derives call features such as CUG, CLIP etc.

	INPUT PARAMETERS	: Response structure from CHLD

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL DeriveAndSetCallFeatures(void* info)
{
	MMI_PARSE_STR_RSP *rsp;
	rsp = (MMI_PARSE_STR_RSP*)info;

	SetCCFlag(TRUE);
	//chepi for time being.. dependent on pluto
	//if(L4C_OK==rsp->result.flag)
	if(1 == rsp->result.flag)
	{
	  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_DeriveAndSetCallFeatures, rsp->info.opcode));

		if (CC_OPERATION == rsp->info.opcode ||
		    SS_OPERATION == rsp->info.opcode ||
		    SIM_OPERATION == rsp->info.opcode)
		{
			if (CC_OPERATION != rsp->info.opcode)
			{
				SetCCFlag(FALSE);
			}
		   return TRUE;
      }
   }

   SetCCFlag(FALSE);
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: DeriveOutgoingCallIdRsp

  	PURPOSE				: Derives call ID for Outgoing call

	INPUT PARAMETERS	: Call list from PS

	OUTPUT PARAMETERS	: Call ID

	RETURNS				: U8

 
**************************************************************/
S16 DeriveOutgoingCallIdRsp(void* info, U16 *cause)
{
	MMI_DIAL_IND* rsp;

	rsp = (MMI_DIAL_IND*)info;
	if (rsp->result.flag == L4C_OK)
	{
	  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_DeriveOutgoingCallIdRsp1, rsp->call_id));
		return rsp->call_id;
	}
	else
	{
	  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_DeriveOutgoingCallIdRsp2));
		*cause = rsp->result.cause;
		return -1;
	}
}

/**************************************************************

	FUNCTION NAME		: DeriveCallHandle

  	PURPOSE				: Derives call ID for End  call

	INPUT PARAMETERS	: Call list from PS

	OUTPUT PARAMETERS	: Call ID

	RETURNS				: U8

 
**************************************************************/
CM_CALL_HANDLE DeriveCallHandle(void* info)
{
	return (CM_CALL_HANDLE)((MMI_RELEASE_IND*)info)->call_id;
}

/**************************************************************

	FUNCTION NAME		: DeriveCallInfo

 

	INPUT PARAMETERS	: void*, void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeriveCallInfo(void* info, IncomingCall* incmg)
{
	MMI_INCOMING* inc;
	U8 flag=0x10;

	inc = (MMI_INCOMING*) info;
	memset(incmg,'\0',sizeof(IncomingCall));

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_DeriveCallInfo, inc->call_id));

	incmg->callHandle = inc->call_id;
	incmg->callType = inc->call_type;

	if (inc->num.number[0] != '\0')
	{
	   if((flag)&(inc->num.type))
	   {
		   strcpy ((PS8)incmg->Number,"+");
	   }
	   strcat((PS8)incmg->Number,(PS8)(inc->num.number));
	}

	if ((inc->sub_addr.number[0] != '\0') &&
		(strlen ((PS8)inc->sub_addr.number) + strlen ((PS8)incmg->Number) <
		MAX_NUMBER/ENCODING_LENGTH - 1))
	{
		strcat ((PS8)incmg->Number, "-");
		strcat ((PS8)incmg->Number, (PS8)inc->sub_addr.number);
	}

	if (inc->auto_answer == KAL_TRUE)
		SetPriorityCall(TRUE);
	else
		SetPriorityCall(FALSE);
	return;
}

/**************************************************************

	FUNCTION NAME		: DeriveCallInfoFromWait

 

	INPUT PARAMETERS	: void*, void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeriveCallInfoFromWait(void* info, IncomingCall* incmg)
{
	MMI_CALL_WAIT* wait;
	U8 flag = 0x10;

	memset(incmg,'\0',sizeof(IncomingCall));
	wait = (MMI_CALL_WAIT*)info;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_DeriveCallInfoFromWait, wait->call_id));

	incmg->callType = wait->call_mode;
   incmg->callHandle = wait->call_id;

   if (wait->num.number[0] != '\0')
	{
	   if((flag)&(wait->num.type))
	   {
		   strcpy ((PS8)incmg->Number,"+");
	   }
	   strcat((PS8)incmg->Number,(PS8)(wait->num.number));
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: DeriveSSEvent

  	PURPOSE				: Function derives call features such as CUG, CLIP etc.

	INPUT PARAMETERS	: Response structure from CHLD

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void DeriveSSEvent(void* info)
{
	MMI_NOTIFY_SS *rsp;

	rsp = (MMI_NOTIFY_SS*) info;
	gSSEvent = rsp->type;
	return;
}

/**************************************************************

	FUNCTION NAME		: GetStrDisplayForId

  	PURPOSE				: Function derives call features such as CUG, CLIP etc.

	INPUT PARAMETERS	: Response structure from CHLD

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
U16 GetStrDisplayForId(U16 event)
{
	switch (event)
	{
	case CSMCC_NOTIFY_CFC:
		return STR_NFY_CALL_FWDNG_CONDITIONAL;
	case CSMCC_NOTIFY_CFU:
		return STR_NFY_CALL_FWDNG_UNCONDITIONAL;
	case CSMCC_NOTIFY_CF_CFU:
		return STR_NFY_CALL_FWDED_UNCONDITIONAL;
	case CSMCC_NOTIFY_CF_CFC:
		return STR_NFY_CALL_FWDED_CONDITIONAL;
	case CSMCC_NOTIFY_CF_CFB:
		return STR_NFY_CALL_BUSY_FORWARDED;
	case CSMCC_NOTIFY_CF_CFNRY:
		return STR_NFY_CALL_FWDED_NOREPLY;
	case CSMCC_NOTIFY_CF_CFNRC:
		return STR_NFY_CALL_FWDED_NOT_REACHABLE;
	case CSMCC_NOTIFY_CF:
		return STR_NFY_CALL_FWDED;

	case CSMCC_NOTIFY_BAC:
		return STR_NFY_CALL_ALL_BARRED;
	case CSMCC_NOTIFY_BAOC:
		return STR_NFY_CALL_ALL_OUTGOING_BARRED;
	case CSMCC_NOTIFY_BAIC:
		return STR_NFY_CALL_ALL_INCOMING_BARRED;
	case CSMCC_NOTIFY_CW:
		return STR_NFY_CALL_WTG;

	case CSMCC_NOTIFY_CALL_HELD:
		return STR_NFY_CALL_HLD;

	case CSMCC_NOTIFY_CALL_RETRIEVED:
		return STR_NFY_CALL_RTVD;

	case CSMCC_NOTIFY_MPTY:
		return STR_NFY_CALL_CONF;

	case CSMCC_NOTIFY_CUG:
		return STR_NFY_CALL_CUG;

	case CSMCC_NOTIFY_ECT_ACTIVE:
		return STR_NFY_CALL_CTFAC;

	default:
		return 0;
	}
}

/**************************************************************

	FUNCTION NAME		: DeriveAcmValue

  	PURPOSE				: Derives ACM value from the PS value

	INPUT PARAMETERS	: ACM value struct

	OUTPUT PARAMETERS	: U16

	RETURNS				: U8

 
**************************************************************/
U32 DeriveCcmValue(void* info)
{
	MMI_CCM_STRUCT* ccmStruct;

	ccmStruct = (MMI_CCM_STRUCT*)info;
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_DeriveCcmValue, ccmStruct->ccm));

	if (ccmStruct->warning)
	  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_DeriveCcmWarning));

	return ccmStruct->ccm;
}

/**************************************************************

	FUNCTION NAME		: DeriveInternalAlrtIndVal

  	PURPOSE				: Derives value of internal Alert indication message

	INPUT PARAMETERS	: Internal Alert Indication structure

	OUTPUT PARAMETERS	: Value(BOOL) of msg

	RETURNS				: U8

 
**************************************************************/
U8 DeriveInternalAlrtIndVal(void* info)
{
	return ((MMI_CC_INTERNAL_ALERT_IND*)info)->alert;
}

/**************************************************************

	FUNCTION NAME		: CheckStatusOfSpeech

  	PURPOSE				: Helper to check is speech is set or not

	INPUT PARAMETERS	: Indication structure

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
	                      info describes the data to transparently send to n/w

**************************************************************/
pBOOL CheckStatusOfSpeech(void* info)
{
	return ((MMI_SPEECH_IND*)info)->on_off;
}

/**************************************************************

	FUNCTION NAME		: DeriveUSSDData

  	PURPOSE				: Function derives data in USSN, USSR, and USSD messages

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void DeriveUSSDData (U8 dcs, U8* ussd_string)
{
   	U16 ussrLen = 0;
	U8 *tempStr = NULL;

#if 0
	if(dcs & 0x40)
	{
		if(!(dcs & 0x20))//uncompressed
		{
			dcs &= 0x0F;
		}
	}
#endif /* Guoj delete. It's not used now at 2009-10-22 */

	if (dcs == MMI_UCS2_DCS || dcs == MMI_UCS2_81 || dcs == MMI_UCS2_82)
	{
		ussrLen = (U16)pfnUnicodeStrlen((PS8)ussd_string);
		ussrLen = ussrLen * 2;
	}
	else
	{
		ussrLen = strlen((PS8)ussd_string);
	}

    if (ussrLen >= MAX_DIGITS_USSD)
    {
		if(dcs == MMI_UCS2_DCS)
		{
			ussrLen = MAX_DIGITS_USSD - 2;
		}
		else
		{
			ussrLen = MAX_DIGITS_USSD - 1;
		}
    }
	tempStr = CovertStringForPlatform(ussd_string, ussrLen,
					                      dcs, &ussrLen);
	
	if (ussrLen>= MAX_DIGITS_USSD)
		ussrLen = MAX_DIGITS_USSD -1;

	memset((void *)gUSSDbuf,0,MAX_DIGITS_USSD* ENCODING_LENGTH);

	if (tempStr != NULL)
	{
	   pfnUnicodeStrncpy ((PS8)gUSSDbuf, (PS8) tempStr, ussrLen);
		OslMfree(tempStr);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: DeriveUSSNbuf

  	PURPOSE				: Function derives USSN

	INPUT PARAMETERS	: Response structure from CHLD

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void DeriveUSSNbuf(void* info)
{
   DeriveUSSDData (((MMI_SS_USSN_IND_STRUCT *)info)->dcs,
                    ((MMI_SS_USSN_IND_STRUCT *)info)->ussd_string);
   return;
}

/**************************************************************

	FUNCTION NAME		: DeriveUSSRbuf

  	PURPOSE				: Function derives USSR

	INPUT PARAMETERS	: Response structure from CHLD

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void DeriveUSSRbuf(void* info)
{
   DeriveUSSDData (((MMI_SS_USSR_IND_STRUCT *)info)->dcs,
                    ((MMI_SS_USSR_IND_STRUCT *)info)->ussd_string);
}

/**************************************************************

	FUNCTION NAME		: DeriveUSSDbuf

  	PURPOSE				: Function derives USSN

	INPUT PARAMETERS	: Response structure from CHLD

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void DeriveUSSDbuf(void* info)
{
   DeriveUSSDData (((MMI_SS_USSD_RSP *)info)->dcs,
                    ((MMI_SS_USSD_RSP *)info)->ussd_str);
}

/**************************************************************

	FUNCTION NAME		: CheckHoldRequestSuccess

  	PURPOSE				: checks for failure of CHLD from PS

	INPUT PARAMETERS	: CHLD rsp structure

	OUTPUT PARAMETERS	: true for success false for failure

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL CheckChldRequestSuccess(void* info, U16 *cause)
{
	MMI_CHLD_RSP* chldRsp;

	chldRsp = (MMI_CHLD_RSP*)info;
	if (chldRsp->result == KAL_TRUE)
	{
	   *cause = 0;
		return TRUE;
   }
	else
	{
	  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CheckChldRequestSuccess));
	   *cause = chldRsp->cause;
		return FALSE;
	}
}

/**************************************************************

	FUNCTION NAME		: CheckFailureChld

  	PURPOSE				: function detemines if CHLD request for end calls has failed

	INPUT PARAMETERS	: Response structure from CHLD

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void CheckFailureChld(void* info)
{
   U16 result;

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s ", __FUNCTION__);
	if (CheckChldRequestSuccess(info, &result))
	{
#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
		SetChldReqSent(CM_ACTION_NONE);
		return;
	}
	else
	{
	  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CheckFailureChld, result));

	   switch(GetChldReqSent())
		{
			case CM_HANGUPALLHLD_REQ_SENT:
			case CM_HANGUPALLACTIVE_REQ_SENT:
			case CM_HANGUPALL_REQ_SENT:
			case CM_ENDSELECTIVE_REQ_SENT:
			   ResetAllHangupFlag();
				break;
			default:
			   return;
		}

		SetChldReqSent(CM_ACTION_NONE);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
		ShowCallManagementErrorMessage(result);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: CheckCallDeflectSuccess

  	PURPOSE				: Function checks to see if the Call Deflect rsp was successful

	INPUT PARAMETERS	: Response structure

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
U16 CheckCallDeflectSuccess(void* info, U16* cause)
{
	MMI_CALL_DEFLECT_RSP* rsp;

	rsp = (MMI_CALL_DEFLECT_RSP*)info;

	if (rsp->result == TRUE)
	{
	   *cause = 0;
		return TRUE;
	}
	else
	{
		*cause = rsp->cause;
		return FALSE;
	}
}

/**************************************************************

	FUNCTION NAME		: CheckAtaSuccess

  	PURPOSE				: Function checks to see if the ATA rsp was successful

	INPUT PARAMETERS	: Response structure from CHLD

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
U16 CheckAtaSuccess(void* info,U16 *cause)
{
	MMI_ATA_RSP* rsp;

	rsp = (MMI_ATA_RSP*)info;

	if (rsp->result.flag == L4C_OK)
	{
	   *cause = 0;
		return TRUE;
	}
	else
	{
		/* can not answer MT data call thru MMI */
		if (rsp->result.cause == CM_L4C_GEN_CAUSE && rsp->call_mode == SINGLE_DATA_CALL)
			*cause = ERR_CM_MT_DATA_CALL;
		else
		*cause = rsp->result.cause;
		return FALSE;
	}
}

/**************************************************************

	FUNCTION NAME		: CheckRspCcbsFlag

  	PURPOSE				: checks for CCBS Flag in Disconnect Response from PS

	INPUT PARAMETERS	: CHLD rsp structure

	OUTPUT PARAMETERS	: true for success false for failure

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL CheckRspCcbsFlag(void* info)
{
	return ((MMI_CC_CALL_DISCONNECT_IND*)info)->ccbs;
}

/**************************************************************

	FUNCTION NAME		: CBackDropUSSDReq(void *)

  	PURPOSE				: Aborts SS string request

	INPUT PARAMETERS	: callback to register with response

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
extern void  aud_speech_set_mode(kal_bool on);
void CBackDropUSSDReq(void *info)
{
    mmi_trace(g_sw_SS, "SS AbortUSSDRequest result = %d", ((MMI_SS_ABORT_RSP*)(info))->result);	

    aud_speech_set_mode(KAL_FALSE);//bug 10229, zhangm
    SetAbortCallRequestedFlag(FALSE);

    if(((MMI_SS_ABORT_RSP*)(info))->result == TRUE)
    {
        SetDelScrnIDCallbackHandler(SCR_CM_REQUESTINGUSSD, (HistoryDelCBPtr) NULL);
        DeleteScreenIfPresent(SCR_CM_REQUESTINGUSSD);
        
        GoBackHistory();
    }
    else
    {
        ShowCallManagementErrorMessage(ERROR_FAIL);
    }
}

/**************************************************************

	FUNCTION NAME		: CBackDropUSSDReqEND(void *)

  	PURPOSE				: Aborts SS string request on END Key

	INPUT PARAMETERS	: callback to register with response

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void CBackDropUSSDReqEND(void *info)
{
	aud_speech_set_mode(KAL_FALSE);//bug 10229, zhangm
	
	SetAbortCallRequestedFlag(FALSE);
    
	if(((MMI_SS_ABORT_RSP*)(info))->result==TRUE)
	{
		SetDelScrnIDCallbackHandler(SCR_CM_REQUESTINGUSSD, (HistoryDelCBPtr) NULL);
		DeleteScreenIfPresent(SCR_CM_REQUESTINGUSSD);
	
		if(isInCall())
		{
			GoBackHistory();
		}
		else
		{
		   DisplayIdleScreen();
	   	}
	}
	else
	{
		ShowCallManagementErrorMessage(ERROR_FAIL);
	}
}

/**************************************************************

	FUNCTION NAME		: MapDtmfEnum2Ascii

  	PURPOSE				: Function maps keycode from enum value to ascii equiv.

	INPUT PARAMETERS	: DTMF keycode digit

	OUTPUT PARAMETERS	: Ascii equivalent

	RETURNS				: U8

 
**************************************************************/
U8 MapDtmfEnum2Ascii(U16 digit)
{
	switch (digit)
	{
	case KEY_0:
		return '0';
	case KEY_1:
		return '1';
	case KEY_2:
		return '2';
	case KEY_3:
		return '3';
	case KEY_4:
		return '4';
	case KEY_5:
		return '5';
	case KEY_6:
		return '6';
	case KEY_7:
		return '7';
	case KEY_8:
		return '8';
	case KEY_9:
		return '9';
	case KEY_STAR:
		return '*';
	case KEY_POUND:
		return '#';
	default:
		return KEY_INVALID;
	}
}


