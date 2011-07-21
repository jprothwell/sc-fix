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

/**************************************************************

	FILENAME	: ProtocolEvents.h

  	PURPOSE		: struct, defines & globle variables

	REMARKS		: nil

	AUTHOR		: Magesh k

	DATE		: sep' 03, 2002

**************************************************************/

#ifndef _PIXCOM_PROTOCOL_EVENT_H
#define _PIXCOM_PROTOCOL_EVENT_H

#define PRT_CHARGERCONNECTED_EVENT			1	
#define PRT_CHARGERDISCONNECTED_EVENT		2	
/* protocol events */
#define PRT_INCOMINGCALL_EVENT				1
#define PRT_INCOMINGCALL_ACCEPT_EVENT		2
#define PRT_INCOMINGCALL_ACCEPT_FAILURE		3
#define PRT_INCOMINGCALL_ACCEPT_SUCCESS		4
#define PRT_INCOMINGCALL_REJECT_EVENT		5
#define PRT_INCOMINGCALL_DROPPED			6
#define PRT_HELDCALL_DISCONNECTED			7
#define PRT_INCOMINGCALL_FORWARD_EVENT		8
#define PRT_INCOMINGCALL_FORWARD_SUCCESS	9
#define PRT_INCOMINGCALL_FORWARD_FAILURE	10
#define PRT_CALLHOLD_EVENT					11
#define PRT_CALLHOLD_SUCCESS				12
#define PRT_CALLHOLD_FAILURE				13
#define PRT_CALLENDALLHOLD_EVENT			14
#define PRT_HELDCALLS_END_SUCCESS			15
#define PRT_HELDCALLS_END_FAILURE			16
#define PRT_CALLENDACTIVE_EVENT				17
#define PRT_CALLENDACTIVE_SUCCESS			18
#define PRT_CALLENDACTIVE_FAILURE			19
#define PRT_NETWORK_ERROR					20
#define PRT_OUTGOINGCALL_EVENT				21
#define PRT_OUTGOINGCALL_CONNECTED			22
#define PRT_OUTGOINGCALL_FAILURE			23
#define PRT_RETRIEVECALL_EVENT				24
#define PRT_RETRIEVECALLREQ_SUCCESS			25
#define PRT_RETRIEVECALLREQ_FAILURE			26
#define PRT_SWAPCALL_EVENT					27
#define PRT_SWAPCALLREQ_SUCCESS				28
#define PRT_SWAPCALLREQ_FAILURE				29
#define PRT_CONFCALL_EVENT					30
#define PRT_CONFCALLREQ_SUCCESS				31
#define PRT_CONFCALLREQ_FAILURE				32
#define PRT_SPLITCALL_EVENT					33
#define PRT_SPLITCALLREQ_SUCCESS			34
#define PRT_SPLITCALLREQ_FAILURE			35

#define PRT_ENDALLCALL_EVENT				36
#define PRT_ENDALLCALLREQ_SUCCESS			37
#define PRT_ENDALLCALLREQ_FAILURE			38

#define PRT_ENDALLACTIVE_EVENT				39
#define PRT_ENDALLACTIVEREQ_SUCCESS			40
#define PRT_ENDALLACTIVEREQ_FAILURE			41

#define PRT_CALLENDSPECIFIC_EVENT			63
#define PRT_CALLENDSPECIFIC_SUCCESS			64
#define PRT_CALLENDSPECIFIC_FAILURE			65
#define PRT_END_CHLD_RSP					66

#define PRT_WAITINGCALL_REJECT_EVENT		67
#define PRT_WAITINGCALL_REJECT_RSP			68
#define PRT_AOC_IND							69

//Phone book events msg ids
#define PRT_PHB_SET_ENTRY_REQ				101
#define PRT_PHB_SET_ENTRY_RSP				102
#define PRT_PHB_GET_ENTRY_BY_INDEX_REQ		103
#define PRT_PHB_GET_ENTRY_BY_INDEX_RSP		104
#define PRT_PHB_DEL_ENTRY_REQ				105
#define PRT_PHB_DEL_ENTRY_RSP				106
#define PRT_PHB_GET_ENTRY_BY_NAME_REQ		107
#define PRT_PHB_GET_ENTRY_BY_NAME_RSP		108
#define	PRT_PHB_GET_PREFERRED_STORAGE_REQ	109
#define	PRT_PHB_GET_PREFERRED_STORAGE_RSP	110

#define PRT_SMS_BASE							150
#define MSG_ID_SMS_SET_BEARER_SERVICE_RSP		PRT_SMS_BASE+1
#define MSG_ID_SMS_GET_BEARER_SERVICE_RSP		PRT_SMS_BASE+2
#define MSG_ID_SMS_GET_SEND_COMMAND_RSP			PRT_SMS_BASE+3
#define MSG_ID_SMS_GET_VALIDITY_PERIOD_RSP		PRT_SMS_BASE+4
#define MSG_ID_SMS_SET_VALIDITY_PERIOD_RSP		PRT_SMS_BASE+5
#define MSG_ID_SMS_DEL_MSG_RSP					PRT_SMS_BASE+6
#define MSG_ID_SMS_DEL_ALL_MSG_RSP				PRT_SMS_BASE+7
#define MSG_ID_SMS_SET_FORMAT_RSP				PRT_SMS_BASE+8
#define MSG_ID_SMS_GET_FORMAT_RSP				PRT_SMS_BASE+9
#define MSG_ID_SMS_GET_MSG_LIST_RSP				PRT_SMS_BASE+10
#define MSG_ID_SMS_GET_MSG_RSP					PRT_SMS_BASE+11
#define MSG_ID_SMS_SEND_MSG_RSP					PRT_SMS_BASE+12
#define MSG_ID_SMS_SET_MSG_RSP					PRT_SMS_BASE+13
#define MSG_ID_SMS_SET_MORE_RSP					PRT_SMS_BASE+14
#define MSG_ID_SMS_GET_MORE_RSP					PRT_SMS_BASE+15
#define MSG_ID_SMS_SEND_STORAGE_MSG_RSP			PRT_SMS_BASE+16
#define MSG_ID_SMS_SET_REPORT_MODE_RSP			PRT_SMS_BASE+17
#define MSG_ID_SMS_GET_REPORT_MODE_RSP			PRT_SMS_BASE+18
#define MSG_ID_SMS_SEND_ACK_RSP					PRT_SMS_BASE+19
#define MSG_ID_SMS_SET_PREFERRED_STORAGE_RSP	PRT_SMS_BASE+20
#define MSG_ID_SMS_GET_PREFERRED_STORAGE_RSP	PRT_SMS_BASE+21
#define MSG_ID_SMS_SET_SC_ADDR_RSP				PRT_SMS_BASE+22
#define MSG_ID_SMS_GET_SC_ADDR_RSP				PRT_SMS_BASE+23
#define MSG_ID_SMS_SET_POST_PARAMS_RSP			PRT_SMS_BASE+24
#define MSG_ID_SMS_GET_POST_PARAMS_RSP			PRT_SMS_BASE+25
#define MSG_ID_SMS_SET_SERVICE_MODE_RSP			PRT_SMS_BASE+26
#define MSG_ID_SMS_GET_SERVICE_MODE_RSP			PRT_SMS_BASE+27
#define MSG_ID_SMS_DEL_MSG_BY_REF_RSP			PRT_SMS_BASE+28
#define MSG_ID_SMS_GET_MSG_STATUS_LIST_RSP		PRT_SMS_BASE+29
#define MSG_ID_SMS_GET_MSG_BY_REF_RSP			PRT_SMS_BASE+30
#define MSG_ID_SMS_DEL_ALL_RSP					PRT_SMS_BASE+31
#define MSG_ID_SMS_NEW_MSG_INDEX_IND			PRT_SMS_BASE+32
#define MSG_ID_SMS_DELIVER_MSG_IND				PRT_SMS_BASE+33
#define MSG_ID_SMS_STATUS_REPORT_IND			PRT_SMS_BASE+34	
#define MSG_ID_SMS_MEM_AVAILABLE_IND			PRT_SMS_BASE+35
#define MSG_ID_SMS_MEM_EXCEED_IND				PRT_SMS_BASE+36
#define MSG_ID_SMS_MEM_FULL_IND					PRT_SMS_BASE+37
#define MSG_ID_SMS_SYNC_MSG_IND					PRT_SMS_BASE+38
#define PRT_DTMF_START_REQ					42
#define PRT_DTMF_START_RES					43
#define PRT_DTMF_STOP_REQ					44
#define PRT_DTMF_STOP_RES					45


/* Added for Network Attached indication*/
#define NETWORK_ATTACHED_BASE					5000
#define	PRT_NW_ATTACH_IND						NETWORK_ATTACHED_BASE+1
#define PRT_NW_RX_LEVEL_IND						NETWORK_ATTACHED_BASE +	2					
#define PRT_BATTERY_STATUS_IND					NETWORK_ATTACHED_BASE + 3
#define PRT_SERVICE_TYPE_IND					NETWORK_ATTACHED_BASE + 4
#define PRT_PLMN_LIST_IND						NETWORK_ATTACHED_BASE + 5
#define PRT_GET_IMSI_RSP						NETWORK_ATTACHED_BASE + 6
#define PRT_GET_IMSI_REQ						NETWORK_ATTACHED_BASE + 7



#define MSG_ID_SMS_SET_BEARER_SERVICE_REQ		PRT_SMS_BASE+1
#define MSG_ID_SMS_GET_BEARER_SERVICE_REQ		PRT_SMS_BASE+2
#define MSG_ID_SMS_GET_SEND_COMMAND_REQ			PRT_SMS_BASE+3
#define MSG_ID_SMS_GET_VALIDITY_PERIOD_REQ		PRT_SMS_BASE+4	
#define MSG_ID_SMS_SET_VALIDITY_PERIOD_REQ		PRT_SMS_BASE+5
#define MSG_ID_SMS_DEL_MSG_REQ					PRT_SMS_BASE+6
#define MSG_ID_SMS_DEL_ALL_MSG_REQ				PRT_SMS_BASE+7
#define MSG_ID_SMS_SET_FORMAT_REQ				PRT_SMS_BASE+8
#define MSG_ID_SMS_GET_FORMAT_REQ				PRT_SMS_BASE+9
#define MSG_ID_SMS_GET_MSG_LIST_REQ				PRT_SMS_BASE+10
#define MSG_ID_SMS_GET_MSG_REQ					PRT_SMS_BASE+11
#define MSG_ID_SMS_SEND_MSG_REQ					PRT_SMS_BASE+12		
#define MSG_ID_SMS_SET_MSG_REQ					PRT_SMS_BASE+13	
#define MSG_ID_SMS_SET_MORE_REQ					PRT_SMS_BASE+14	
#define MSG_ID_SMS_GET_MORE_REQ					PRT_SMS_BASE+15
#define MSG_ID_SMS_SEND_STORAGE_MSG_REQ			PRT_SMS_BASE+16	
#define MSG_ID_SMS_SEND_ACK_REQ					PRT_SMS_BASE+17
#define MSG_ID_SMS_SET_PREFERRED_STORAGE_REQ	PRT_SMS_BASE+18
#define MSG_ID_SMS_GET_PREFERRED_STORAGE_REQ	PRT_SMS_BASE+19
#define MSG_ID_SMS_SET_SC_ADDR_REQ				PRT_SMS_BASE+20	
#define MSG_ID_SMS_GET_SC_ADDR_REQ				PRT_SMS_BASE+21
#define MSG_ID_SMS_SET_POST_PARAMS_REQ			PRT_SMS_BASE+22	
#define MSG_ID_SMS_GET_POST_PARAMS_REQ			PRT_SMS_BASE+23
#define MSG_ID_SMS_SET_SERVICE_MODE_REQ			PRT_SMS_BASE+24
#define MSG_ID_SMS_GET_SERVICE_MODE_REQ			PRT_SMS_BASE+25
#define MSG_ID_SMS_DEL_MSG_BY_REF_REQ			PRT_SMS_BASE+26
#define MSG_ID_SMS_GET_MSG_STATUS_LIST_REQ		PRT_SMS_BASE+27
#define MSG_ID_SMS_GET_MSG_BY_REF_REQ			PRT_SMS_BASE+28
#define MSG_ID_SMS_SET_REPORT_MODE_REQ			PRT_SMS_BASE+29
#define MSG_ID_SMS_GET_REPORT_MODE_REQ			PRT_SMS_BASE+30
#define MSG_ID_SMS_DEL_ALL_REQ					PRT_SMS_BASE+31	
#define PRT_USSD_REQ						46
#define PRT_USSD_RES_SUCCESS				47
#define PRT_USSD_RES_FAILURE				48

#define PRT_UDUB_REQ						49
#define PRT_UDUB_RES_SUCCESS					50
#define PRT_UDUB_RES_FAILURE				51

#define PRT_ECT_REQ							52
#define PRT_ECT_REQ_SUCCESS					53
#define PRT_ECT_REQ_FAILURE					54


#define PRT_CELLBROADCAST_BASE					190 /* need to remove value ..asap ..magesh <for rajesha>*/
#define PRT_ACTIVATE_CELL_BROADCAST_REQ			PRT_CELLBROADCAST_BASE+1
#define PRT_DEACTIVATE_CELL_BROADCAST_REQ		PRT_CELLBROADCAST_BASE+2
#define PRT_ACTIVATE_CELL_BROADCAST_LANG_REQ	PRT_CELLBROADCAST_BASE+3
#define PRT_DEACTIVATE_CELL_BROADCAST_LANG_REQ	PRT_CELLBROADCAST_BASE+4
#define PRT_GET_CB_CHANNEL_LIST_REQ				PRT_CELLBROADCAST_BASE+5
#define PRT_ENABLE_CB_CHANNEL_REQ				PRT_CELLBROADCAST_BASE+6
#define PRT_DISABLE_CB_CHANNEL_REQ				PRT_CELLBROADCAST_BASE+7
#define PRT_STORE_CB_CHANNEL_REQ				PRT_CELLBROADCAST_BASE+8
#define PRT_DELETE_CB_CHANNEL_REQ				PRT_CELLBROADCAST_BASE+9
#define PRT_GET_LIST_CB_LANG_REQ				PRT_CELLBROADCAST_BASE+10

#define PRT_DEACTIVATE_CELL_BROADCAST_RSP		PRT_CELLBROADCAST_BASE+1
#define PRT_ACTIVATE_CELL_BROADCAST_RSP			PRT_CELLBROADCAST_BASE+2
#define PRT_ACTIVATE_CELL_BROADCAST_LANG_RSP	PRT_CELLBROADCAST_BASE+3
#define PRT_DEACTIVATE_CELL_BROADCAST_LANG_RSP	PRT_CELLBROADCAST_BASE+4
#define PRT_GET_CB_CHANNEL_LIST_RSP				PRT_CELLBROADCAST_BASE+5
#define PRT_ENABLE_CB_CHANNEL_RSP				PRT_CELLBROADCAST_BASE+6
#define PRT_DISABLE_CB_CHANNEL_RSP				PRT_CELLBROADCAST_BASE+7
#define PRT_STORE_CB_CHANNEL_RSP				PRT_CELLBROADCAST_BASE+8
#define PRT_DELETE_CB_CHANNEL_RSP				PRT_CELLBROADCAST_BASE+9
#define PRT_GET_LIST_CB_LANG_RSP				PRT_CELLBROADCAST_BASE+10
#define PRT_CELL_BROADCAST_MSG_IND				PRT_CELLBROADCAST_BASE+11

#define PRT_SIM_DETECTION_BASE					400
#define MSG_ID_QUERY_SIM_CARD_REQ				PRT_SIM_DETECTION_BASE+1
#define MSG_ID_QUERY_SIM_CARD_FAILURE			PRT_SIM_DETECTION_BASE+2
#define MSG_ID_QUERY_SIM_CARD_SUCCESS			PRT_SIM_DETECTION_BASE+3
#define	MSG_ID_GET_PIN_REQ						PRT_SIM_DETECTION_BASE+4
#define	MSG_ID_GET_PIN_RES						PRT_SIM_DETECTION_BASE+5

typedef enum _errorConditions {
	//misc error
	NETWORK_ERROR=1,
	UNKNOWN_ERROR,
	//Drop Cause
	SENDER_DISCONNECT,
	//Held Disconnect Cause
	USER_DISCONNECT, 
	OTHER_PARTY_DISCONNECT,
	//Hold Failure Cause
	NO_ACTIVE_CALL,
	//HeldEndFailureCause 
	NO_HOLD_CALLTO_END,
	//RetriveFailureCause 
	NO_HELD_CALL, 
	//ConfFailureCause 
	SERVICE_NOT_AVAILABLE, 
	//OugoingFailureCause 
	CALL_BARRED, 
	NOT_FIXED_DIAL,
	//SplitFailureCause 
	NO_CALL_TO_SPLIT

}ERROR_CONDITIONS;

#define ALL_CALL_COST_FILE_ID 1
#define CALLER_ID_FILE_ID 2
#define PHONE_LINE_FILE_ID 3
#define NEXT_INCOMING_FILE_ID 4
#define TIMER_DISPLAY_FILE_ID 5


typedef struct _nvram_file_str{
	unsigned int FileIdx;
	unsigned int option;
}nvram_file_str;

typedef enum _In_call_timer_option {
	InCallTimerHide,
	InCallTimerShow
}In_call_timer_option;

typedef struct _call_cost {
	unsigned int Duration;
	unsigned int Cost;
} call_cost;

//char PIN2[MAX_PASSWORD_LENGTH];

typedef enum _my_callerID_option {
	CallerIDHide,
	CallerIDShow
}my_callerID_option;

typedef enum _phone_line_option {
	FirstLine,
	SecondLine
}phone_line_option;

typedef enum _next_incoming_option {
	Voice,
	Fax,
	Data
}next_incoming_option;

#define PRT_GET_CREDIT_AVAILABLE_REQ				251
#define	PRT_GET_CREDIT_AVAILABLE_REQ_SUCCESS		252
#define PRT_GET_CREDIT_AVAILABLE_REQ_FAILURE		253

#define	PRT_GET_ALL_CALL_COSTS_REQ					254
#define	PRT_GET_ALL_CALL_COSTS_REQ_SUCCESS			255
#define	PRT_GET_ALL_CALL_COSTS_REQ_FAILURE			256

#define PRT_GET_RECENT_CALL_COST_REQ				257
#define	PRT_GET_RECENT_CALL_COST_REQ_SUCCESS		258	
#define	PRT_GET_RECENT_CALL_COST_REQ_FAILURE		259
#define	PRT_SET_CALL_TIMER_REQ						260
#define	PRT_SET_CALL_TIMER_REQ_SUCCESS				261
#define	PRT_SET_CALL_TIMER_REQ_FAILURE				261

#define PRT_SET_ALL_CALL_COST_REQ					262
#define	PRT_SET_ALL_CALL_COST_REQ_SUCCESS			263

#define	PRT_SET_ALL_CALL_COST_REQ_FAILURE			265

#define	PRT_GET_CREDIT_LIMIT_REQ					266
#define	PRT_GET_CREDIT_LIMIT_REQ_SUCCESS			267
#define PRT_GET_CREDIT_LIMIT_REQ_FAILURE			268

#define	PRT_SET_MYCALLERID_REQ						269
#define	PRT_SET_MYCALLERID_REQ_SUCCESS				270
#define	PRT_SET_MYCALLERID_REQ_FAILURE				271

#define	PRT_SET_PHONE_LINE_REQ						272
#define	PRT_SET_PHONE_LINE_REQ_SUCCESS				273
#define	PRT_SET_PHONE_LINE_REQ_FAILURE				274

#define	PRT_SET_NEXT_INCOMING_REQ					275
#define	PRT_SET_NEXT_INCOMING_REQ_SUCCESS			276

#define	PRT_SET_NEXT_INCOMING_REQ_FAILURE			277

#define PRT_SEARCH_NETWORK_REQ				201
#define PRT_SEARCH_NETWORK_REQ_SUCCESS		202
#define PRT_SEARCH_NETWORK_REQ_FAILURE		203
#define PRT_SELECT_NETWORK_REQ				204
#define PRT_SELECT_NETWORK_SUCCCES			205
#define PRT_SELECT_NETWORK_FAILURE			206
#define PRT_SELECT_MODE_REQ					207
#define PRT_SELECT_MODE_SUCCESS				208
#define PRT_SELECT_MODE_FAILURE				208
#define PRT_PREFERRED_NETWORKS_REQ			209
#define PRT_PREFERRED_NETWORKS_REQ_SUCCESS	210
#define PRT_PREFERRED_NETWORKS_REQ_FAILURE	211
#define PRT_ADD_NEW_REQ						212
#define PRT_ADD_NEW_REQ_SUCCESS				213
#define PRT_ADD_NEW_REQ_FAILURE				214
#define PRT_CHANGE_PRIORITY_REQ				215
#define PRT_CHANGE_PRIORITY_REQ_SUCCESS		216
#define PRT_CHANGE_PRIORITY_REQ_FAILURE		217
#define PRT_FULL_NAME_REQ					218
#define PRT_FULL_NAME_REQ_SUCCESS			219
#define	PRT_FULL_NAME_REQ_FAILURE			220
#define PRT_DELETE_PLMN_REQ					221
#define PRT_DELETE_PLMN_REQ_SUCCESS			222
#define PRT_DELETE_PLMN_REQ_FAILURE			223

/* Hardware Messages ********/

#define HW_SET_TIME_REQ							551
#define HW_SET_TIME_REQ_SUCCESS					552
#define HW_SET_TIME_REQ_FAILURE					553

#define HW_SET_TIME_FORMAT_REQ					554
#define HW_SET_TIME_FORMAT_REQ_SUCCESS			555
#define HW_SET_TIME_FORMAT_REQ_FAILURE			556


#define HW_SET_DATE_REQ							560
#define HW_SET_DATE_REQ_SUCCESS					561
#define HW_SET_DATE_REQ_FAILURE					562

#define HW_SET_DATE_FORMAT_REQ					563
#define HW_SET_DATE_FORMAT_REQ_SUCCESS			564
#define HW_SET_DATE_FORMAT_REQ_FAILURE			565

#define HW_SET_FIXED_DAIL_REQ					283
#define HW_SET_FIXED_DAIL_RES					284

#define HW_SET_BARRED_DIAL_REQ					286
#define HW_SET_BARRED_DIAL_RES					287

#define HW_SET_SIM_LOCK_REQ						289
#define HW_SET_SIM_LOCK_RES						290

#define HW_CHANGE_PASSWORD_REQ					291
#define HW_CHANGE_PASSWORD_RES					292

#define HW_AUTO_REDIAL_REQ						293
#define HW_AUTO_REDIAL_RES						294


/* SIM Application ToolKit Messages*/

#define PRT_SAT_BASE					16000
#define MSG_ID_MMI_SAT_SETUP_MENU_IND	PRT_SAT_BASE + 1
#define MSG_ID_MMI_SAT_SETUP_MENU_RES_REQ PRT_SAT_BASE + 2
#define MSG_ID_MMI_SAT_SETUP_MENU_RES_RSP PRT_SAT_BASE + 3
#define MSG_ID_MMI_SAT_MENU_SELECT_REQ PRT_SAT_BASE + 4
#define MSG_ID_MMI_SAT_MENU_SELECT_RSP PRT_SAT_BASE + 5
#define MSG_ID_MMI_SAT_SELECT_ITEM_IND PRT_SAT_BASE + 6
#define MSG_ID_MMI_SAT_SELECT_ITEM_RES_REQ PRT_SAT_BASE + 7
#define MSG_ID_MMI_SAT_SELECT_ITEM_RES_RSP PRT_SAT_BASE + 8
#define MSG_ID_MMI_SAT_DISPLAY_TEXT_IND PRT_SAT_BASE + 9
#define MSG_ID_MMI_SAT_DISPLAY_TEXT_RES_REQ PRT_SAT_BASE + 10
#define MSG_ID_MMI_SAT_DISPLAY_TEXT_RES_RSP PRT_SAT_BASE + 11



#if 0
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
#endif	// _PIXCOM_PROTOCOL_EVENT_H


	
