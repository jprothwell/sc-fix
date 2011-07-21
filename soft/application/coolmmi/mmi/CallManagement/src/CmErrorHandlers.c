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
 *   CmErrorHandlers.c
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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/CallManagement/src/CmErrorHandlers.c-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:56:58   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:37:26   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:23:44   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:32:54   zhangxb
 * Initial revision.
 * 
 *    Rev 1.1   Sep 19 2006 19:10:50   zhoumn
 * No change.
 * 
 *    Rev 1.0   Jul 06 2006 17:42:36   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Jul 04 2006 22:56:46   zhangx
 *
 *******************************************************************************/

/**************************************************************

	FILENAME	: CmErrorHandlers.c

	PURPOSE		: Call Management error handlers

 

	 

	DATE		: Apr 07, 03

**************************************************************/
#define __NEWSIMULATOR
#include "stdc.h"
#include "mmi_features.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "debuginitdef.h"
#include "timerevents.h"
#include "historygprot.h"
#include "sublcdhistorygprot.h"
#include "wgui_categories_popup.h"

#include "profilegprots.h"

#include "callmanagementiddef.h"
#include "cmerrortypes.h"
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
#include "commonstubsprot.h"
#include "historyhelperprot.h"
#include "redialhelpergprot.h"
#include "callmanagementgprot.h"
#include "outgoingcallmanagementprot.h"

#include "cmerrorhandlersprot.h"
#include "commonscreens.h" // for popup window timer.
#include "gpioinc.h"
#undef __NEWSIMULATOR


extern void  aud_speech_set_mode(kal_bool on);
/**************************************************************

	FUNCTION NAME		: ProcessIncomingFailureEvents (CM_EVENT_TYPES event, void* info)

  	PURPOSE				: This function handles all the failure conditions in call management.

	INPUT PARAMETERS	: Incoming Event Type, Any information associated with the event

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ProcessIncomingFailureEvents (CM_EVENT_TYPES event, U16 cause)
{
	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessIncomingFailureEvents));

	switch (event)
	{
	case CM_PS_HELD_CALL_ACTIVE:
	case CM_PS_ACTIVE_CALL_HELD:
	case CM_PS_INCOMING_CALL_CONNECTED:
	   /* fail to answer incoming call or waiting call */
		//No need to modify state machine in case of ERROR
		//ProcessIncomingFailureCallEvent(cause);
		ShowCallManagementErrorMessage(cause);
		break;

	case CM_PS_INCOMING_CALL_DEFLECT:
	case CM_PS_ACTIVATED_CCBS:
	case CM_PS_CCBS_INVOKE_DIAL:
		ShowCallManagementErrorMessage(cause);
		break;

	default:
		ShowCallManagementErrorMessage(UNKNOWN_ERROR);
		PRINT_INFORMATION(("File: [%s]  Line: [%d] <<Not Valid State Machine event>>\n",__FILE__,__LINE__));
		break;
	}
}

/**************************************************************

	FUNCTION NAME		: OutgoingProcessCMFailureEvent(CM_EVENT_TYPES ,void* )

  	PURPOSE				: This is the Entry function for all the Events while the Call-Management application
						  is Active.

	INPUT PARAMETERS	: CM_EVENT_TYPES & void*(pointer to the event's associated structure)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  The keyboard events have KB in there defination and Protocol has PS.

**************************************************************/
void OutgoingProcessCMFailureEvent(CM_EVENT_TYPES inEvenType,U16 cause)
{
	mmi_trace(g_sw_CC,"MMI_Call: Func: %s inEvenType=%d,cause=%d", __FUNCTION__,inEvenType,cause);
	switch(inEvenType)
	{
	case CM_PS_CALLCONNECTED:
	   /* fail in ss_parse_rsp, dial_ind, connect_ind or
	      got release_ind before connect */
		ProcessPSCallConnectFailEvent(cause);
		break;

	case CM_PS_HOLDSUC:           /* hold fail */
	case CM_PS_RETRIEVESUC:       /* retreive fail */
	case CM_PS_SWAPSUC:					/* swap fail */
	case CM_PS_CONFSUC:	           /* conference fail */
	case CM_PS_SPLITSUC:	           /* split fail */
	case CM_PS_ECT_SUC:	           /* ECT fail */
		ShowCallManagementErrorMessage(cause);
		break;

	default:
		mmi_trace(g_sw_CC,("\n CM Event Handler does not exist \n"));
		break;
	}
}

/**************************************************************

	FUNCTION NAME		: ProcessPSCallconnectedFailEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) failure Event.

	INPUT PARAMETERS	: void*(this points to CM_CALL_HANDLE of the call which requested CM_KB_OUTGOINGCALL)

	OUTPUT PARAMETERS	: void

	RETURNS				:

 
**************************************************************/
void ProcessPSCallConnectFailEvent(U16 cause)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    CM_CALL_HANDLE callHandle;
    U8 type;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GetCCFlag() == TRUE)
    {
        callHandle = GetOutgoingCallHandle();

        if (callHandle == -1)
        {
            PRINT_INFORMATION(("\n ProcessPSCallConnectFailEvent Getting the Connected Handle has -1 \n"));
            return;
        }

        switch (GetCurrentState())
        {
                 
            case CM_INCOMING_STATE:
                PurgeOutgoingCallsStructure();
                cause = 0;
                break;

            case CM_OUTGOING_STATE:
                type = GetOutgoingCallType();
                PurgeOutgoingCallsStructure();
                SetCurrentState(GetPreviousState());
                SetPreviousState(CM_OUTGOING_STATE);
                mmi_trace(g_sw_CC,"MMI_Call: Func: %s GetTotalCallCount()=%d, type=%d,cause=%d", __FUNCTION__, GetTotalCallCount(), type, cause);
                if (GetTotalCallCount() == 0)
                {
                    /* only voice call can redial */
                    if ((type != CM_VOICE_CALL) || (IsRedialNeeded(cause) == FALSE))
                    {
                        /* no need to redial, just show error and get out of CM */
                        ShowCallManagementErrorMessage(cause);
                        GetOutOfCMforAdjustHistory();
                        return;
                    }
                    else
                    {
                        /* need to redial, show error and redial */
                        if (IsRedialSet())
                        {
                            SetCallEndedBeforeConnFlag(FALSE);
                            CheckRedialOrShowError(cause);
        					/* abort SS if needed */
                            DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);
                            return;
                        }
                    }
                }
                /* still have other call, can't redial, just show error */
                break;

            default:
                PRINT_INFORMATION(("\n ProcessPSCallConnectFailEvent CM State m/c Corrupted \n"));
                break;
        }
    }
    ShowCallManagementErrorMessage(cause);
}
/**************************************************************

	FUNCTION NAME		: ShowCallManagementErrorMessage(U16 cause)

  	PURPOSE				: This function shows the failure in case of call management

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE or FALSE

	RETURNS				: pBOOL

 
**************************************************************/
void ShowCallManagementErrorMessage(U16 cause)
{
   U16 error_code, img_id=IMG_ERROR_NOTIFY;

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s cause=%d", __FUNCTION__,cause);
	
	if (GetCallEndCause() == 0)
		SetCallEndCause (cause);
	SetCbackAfterSS(NULL);
	if(GetNotifySSFlag())
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ShowErrorAfterNotifySS));
		SetCbackAfterSS((FuncPtr)ShowCallManagementErrorMessage);
		return;
	}

   EntryNewScreen(CM_SCR_ERR_MSG, ExitScrCMErrMsg, NULL, NULL);

	switch (GetCallEndCause())
	{
	case ERR_UNASSIGNED_NUM:
	case ERR_INVALID_NUMBER_FORMAT:
	   error_code = ERROR_UNRECOGNIZED_NUMBER;
	   aud_speech_set_mode(KAL_FALSE); //chenhe,070813. for bug #6228,CM_KB_OUTGOINGCALL is been sent,so have to do so here	
		break;

	case ERR_NO_ROUTE_TO_DESTINATION:
	case ERR_CALL_REJECTED:
	case ERR_DEST_OUT_OF_ORDER:
	case ERR_FACILITY_REJECT:
	case ERR_SWITCH_EQUIPMENT_CONGESTION:
	case ERR_SER_UNAVAILABLE:
	case ERR_BEARER_SER_UNIMPL:
	case ERR_INVALID_TI_VALUE:
	case ERR_USER_NOT_IN_CUG:
	case ERR_INCOMPATIBLE_DEST:
	case ERR_SEMANTIC_ERR:
	case ERR_INTER_WRK_UNSPECIFIED:
	case ERR_CSMCC_ILLEGAL_CALL_ID:
	case ERR_CSMCC_CALL_ALLOC_FAIL:
	case ERR_CSMCC_BC_FILL_FAIL:
	case ERR_CSMCC_ILLEGAL_BC:
	case ERR_CSMCC_MODIFY_ACTUAL_MODE:
	case ERR_CSMCC_DATA_ACT_FAIL:
	case ERR_MAC_FAILURE:
	case ERR_SYNC_FAILURE:
	case ERR_CONGESTION:
	case ERR_ILLEGAL_MS:
	case ERR_ILLEGAL_ME:
	case ERR_CM_MM_CM_ENQUEUE_FAIL:
	case ERR_CM_MM_NOT_UPDATED:
	   error_code = ERROR_FAIL;
		break;

	case ERR_CHANNEL_UN_ACCP:
	case ERR_NO_CIRCUIT_CHANNEL_AVAIL:
	case ERR_NETWORK_OUT_OF_ORDER:
	case ERR_QOS_UNAVAIL:
	case ERR_INVALID_TRANSIT_NW_SEL:
	case ERR_CSMCC_NO_RESPONSE_FROM_NW:
	case ERR_NETWORK_FAILURE:
   case ERR_CM_MM_RR_CONN_RELEASE:
	   error_code = ERROR_NETWORK_FAIL;
		break;

	case ERR_REQUESTED_CKT_CHANEL_NOT_AVIL:
	case ERR_RESOURCE_UNAVAIL_UNSPECIFIED:
	case ERR_CM_MM_AUTH_FAILURE:
	   error_code = ERROR_NETWORK_NOT_AVAILABLE;
		break;

	case ERR_OPR_DTR_BARRING:
	case ERR_CALL_BARRED:
	case ERR_IC_BAR_CUG:
	case ERR_BEARER_CAP_NOT_AVAIL:
	case ERR_CM_MM_ACCESS_BARRED:
	   error_code = ERROR_BARRED_CALL;
		break;

	case ERR_USER_BUSY:
	{
		/* voice call shows user busy, csd call shows network busy */
		if (cm_p->state_info.CallStructureForCallLog.call_type==CM_VOICE_CALL)
	                error_code = ERROR_USER_BUSY;
		else
			error_code = ERROR_BUSY_NETWORK;
	 }
	 break;

	case ERR_NO_USER_RESPONDING:
	   error_code = ERROR_NOT_RESPONDING;
		break;

	case ERR_NO_ANSWER_ON_ALERT:
	   error_code = ERROR_NO_ANSWER;
		break;

	case ERR_ACM_EXCEEDED:
	   error_code = ERROR_ACM_EXCEEDED;
		break;

	case ERR_REQ_FACILITY_UNAVAIL:
	case ERR_IE_NOT_IMPLEMENTED:
	case ERR_CONDITIONAL_MM_IE_ERROR:
	case ERR_CM_MM_ASSIGNMENT_REJECT:
	   error_code = ERROR_UNAVAILABLE;
		break;

	case ERR_CSMCC_CMD_NOT_ALLOW:
	case ERR_CSMCC_ILLEGAL_DTMF_TONE:
	case ERR_CSMCC_CALL_ACCEPT_NOT_ALLOW:
	case ERR_MS_ID_NOT_DERIVED_BY_NW:
	case ERR_IMPLICIT_DETACH:
	case ERR_MSG_NOT_COMPAT_WITH_PROTO_STATE:
	case ERR_PROTO_ERROR_UNSPECIFIED:
	   error_code = ERROR_UNEXPECTED_ERROR;
		break;

	case ERR_IMSI_UNKNOWN_IN_HLR:
	case ERR_IMSI_UNKNOWN_IN_VLR:
	case ERR_CM_MM_IMSI_DETACH:
	   error_code = ERROR_IMSI_UNKNOWN;
		break;

	case ERR_IMEI_NOT_ACCEPTED:
	   error_code = ERROR_IMEI_ERROR;
		break;

	case ERR_GPRS_NOT_ALLOWED:
	case ERR_GPRS_NON_GPRS_NOT_ALLOWED:
	   error_code = ERROR_GPRS_NOT_ALLOWED;
		break;

	case ERR_CM_MT_DATA_CALL:
		error_code = ERROR_MT_DATA_CALL;
		break;
		
   case ERR_REQ_FAC_NOT_SUBS:
	case ERR_PLMN_NOT_ALLOWED:
	case ERR_LOCATION_AREA_NOT_ALLOWED:
	case ERR_ROAMING_AREA_NOT_ALLOWED:
	case ERR_GPRS_NOT_ALLOWED_IN_PLMN:
	case ERR_NO_SUITABLE_CELLS_IN_LA:
	case ERR_MSC_TEMP_NOT_REACHABLE:
	case ERR_CM_MM_ABORT_BY_NW:
	//case ERR_CM_MM_STATE_NOT_ALLOWED_CM:
	case ERR_BEARER_CAP_NOT_AUTHORISED:
	   error_code = ERROR_NETWORK_NOT_ALLOWED;
		break;

	case ERR_SERV_OPTION_NOT_SUPPORTED:
	case ERR_REQ_SERV_OPTION_NOT_SUBSCRIBED:
	case ERR_SERV_OPTION_TEMP_OUT_OF_ORDER:
	case ERR_CALL_CANNOT_BE_IDENTIFIED:
	   error_code = ERROR_COMMAND_NOT_SUPPORTED;
		break;

  	case ERR_NO_PDP_CONTEXT_ACTIVATED:
	case ERR_SEMANTICALLY_INCORRECT_MSG:
	case ERR_INVALID_MM_MAND_INFO:
	   error_code = ERROR_DATA_ERROR;
		break;

  	case ERR_MSG_TYPE_NON_EXISTENT:
	case ERR_MSG_TYPE_INCOMPAT_WITH_PROTO_STATE:
	   error_code = ERROR_MESSAGE_TYPE_ERROR;
		break;

	case ERR_ACCESS_CLASS_BARRED:
	case ERR_CM_MM_RANDOM_ACCES_FAILURE:
	   error_code = ERROR_ACCESS_FAIL;
		break;

	case ERR_CM_MM_CONN_TIMEOUT:
	   error_code = ERROR_TIMER_EXPIRED;
		break;

	case ERR_TEMPORARY_FAILURE:
	case ERR_NORMAL_CALL_CLR:
//	case 0:	//Renwy add for bug 9535 2008/09/03
	   error_code = STR_CALL_ENDED;
	   img_id = IMG_CM_STATE_ENDED;
	   break;
	case ERR_NORMAL_UNSPECIFIED:		
		error_code = ERROR_BUSY_NETWORK;
		break;
	case ERR_L4C_USSD_ABORT:
		error_code = ERROR_USSD_ABORTED;
		break;
	case ERR_CM_MM_RR_NO_SERVICE:
	case ERR_CM_MM_NO_SERVICE:
	case ERR_RESERVED:
	case ERR_NUMBER_CHANGED:
	case ERR_PRE_EMPTION:
	case ERR_NON_SEL_USER_CLEAR:
	case ERR_RES_STATUS_ENQ:
	case ERR_ACCESS_INFO_DISCARDED:
	case ERR_RESTR_DIGITAL_INFO:
	case ERR_SER_OPT_UNIMPL:
	case ERR_INVALID_MANDATORY_INF:
	case ERR_MSG_TYPE_UNIMPL:
	case ERR_MSG_TYPE_NOT_COMPATIBLE:
	case ERR_IE_NON_EX:
	case ERR_COND_IE_ERR:
	case ERR_INCOMP_MESG_WITH_STATE:
	case ERR_RECOVERY_ON_TIMER_EXPIRY:
	case ERR_PROTOCOL_ERR_UNSPECIFIED:
	case ERR_CSMCC_CALL_RE_EST:
	case ERR_RETRY_UPON_ENTRY_INTO_NEWCELL_MIN:
	case ERR_RETRY_UPON_ENTRY_INTO_NEWCELL_MAX:
	case ERR_CM_MM_STATE_NOT_ALLOWED_CM:
	case ERR_CM_MM_CAUSE_NONE:
	case ERR_L4C_GEN_CAUSE:
	   error_code = INCOMING_DEFAULT_ERROR;
		break;

	case ERR_L4C_FDN_FAIL:
		error_code = ERROR_FDN_FAIL;
		break;
		
	case NO_ACTIVE_CALL:
	case ERR_INVALID_OPERATION:
	   error_code = ERROR_INVALID_COMMAND;
   	break;

	case ERR_CM_UNKNOWN:
	   error_code = INCOMING_UNKNOWN_ERROR;
		break;

	default:
	   error_code = INCOMING_DEFAULT_ERROR;
		break;
   }  //End of Switch

	ShowCategory62Screen(error_code,img_id,NULL);

	SetCallEndCause(0);

	if(GetTotalActiveCallCount()==0 && GetTotalHoldCallCount()==0)
	{
	   /* only play error tone when not in-call */
		playRequestedTone(ERROR_TONE_IN_CALL);
		SetErrorToneFlag(TRUE);
	}

	if (cm_p->redial_info.RedialTimer == TRUE)
	{
	   /* cannot press key before pop up disappear,
		buy more time for call log writing */
		DeleteSubLCDScreen(SUB_LCD_SCR1001_CM_OUTGOINGCALLSCREEN);
		GoBackSubLCDHistory();
		if (IsScreenPresent(CM_SCR_MARKER))
			DeleteUptoCmScreen();
		DeleteScreenIfPresent(CM_SCR_MARKER);
		ClearAllKeyHandler();

		ClearKeyHandler(KEY_UP_ARROW,KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_UP_ARROW,KEY_LONG_PRESS);
		ClearKeyHandler(KEY_DOWN_ARROW,KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_DOWN_ARROW,KEY_LONG_PRESS);
		
		ClearKeyHandler(KEY_VOL_UP,KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_VOL_UP,KEY_LONG_PRESS);
		ClearKeyHandler(KEY_VOL_DOWN,KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_VOL_DOWN,KEY_LONG_PRESS);

		SetPOPUpFlag(TRUE);
		SetKeyHandler(ResetRedialAndGoBack, KEY_END, KEY_EVENT_DOWN);
		cm_p->redial_info.ErrRedialScreen = TRUE;

		/*Guojian Add Start For reset status for redial Ver: GreenStone  on 2009-7-20 14:35 */
		/* unmute phone */
		if(TRUE==IsPhoneInMute())
		{
			UnMutethePhone();
		}
	     /* turn off loud speaker */
		if (GetHFreeHeld()==TRUE)
		{
			SetHFreeHeld(FALSE);
			DisbleHandsFree();
			SetLoudSpeaker(FALSE);
		}
		/*Guojian Add End  For reset status for redial Ver: GreenStone  on 2009-7-20 14:35 */
	
		StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT,
		            ShowRedialScreen);
	}
	else
	{
	   /* cannot press key before pop up disappear,
	      buy more time for call log writing */
		ClearAllKeyHandler();
		ClearKeyHandler(KEY_UP_ARROW,KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_UP_ARROW,KEY_LONG_PRESS);
		ClearKeyHandler(KEY_DOWN_ARROW,KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_DOWN_ARROW,KEY_LONG_PRESS);
		
		ClearKeyHandler(KEY_VOL_UP,KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_VOL_UP,KEY_LONG_PRESS);
		ClearKeyHandler(KEY_VOL_DOWN,KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_VOL_DOWN,KEY_LONG_PRESS);

		SetKeyHandler((FuncPtr)GoBackfromNotifyScr, KEY_END, KEY_EVENT_DOWN);
		SetPOPUpFlag(TRUE);
		StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT,
		            GoBackfromNotifyScr);
    }

}

/**************************************************************

	FUNCTION NAME		: ExitScrCMErrMsg(void)

  	PURPOSE				: Exit handler for the error pop up screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	:

	RETURNS				:

 
**************************************************************/
void ExitScrCMErrMsg(void)
{
	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ExitScrCMErrMsg));

	if(GetErrorToneFlag()==TRUE)
	{
		stopRequestedTone(ERROR_TONE);
		SetErrorToneFlag(FALSE);
	}
	StopTimer(CM_NOTIFYDURATION_TIMER);

	if (cm_p->redial_info.RedialTimer == TRUE)
	{
		/* in redial process */
		if (cm_p->redial_info.ErrRedialScreen == TRUE)
	{
			/* error msg got interrupted */
		history_t   Scr1009;
		S16		nHistory	=	0;

			MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ExitScrCMErrMsgRedial));

		Scr1009.scrnID = CM_SCR_RDL_TIME;
		Scr1009.entryFuncPtr = EntryScrShowUserTimeToRedial;
		pfnUnicodeStrcpy((S8*)Scr1009.inputBuffer, (S8*)&nHistory);
		GetCategoryHistory(Scr1009.guiBuffer);
		AddHistory(Scr1009);
			cm_p->redial_info.ErrRedialScreen = FALSE;
			return;
		}
		else
		{
			/* error msg timer expire => going to Notify Redial Screen */
		return;
	}
	}

	//SetRedialScreen(FALSE);

	if(GetPOPUpFlag())
	{
	   SetPOPUpFlag(FALSE);
	   AdjustHistoryForCM();
	}
}

