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

	FILENAME	: CallMgmtStruct.h

  	PURPOSE		: Call Management Structures

	REMARKS		: nil

	AUTHOR		: Shweta Vyas 

	DATE		: Sep 17,03

**************************************************************/
#ifndef _CALLMGMSTRUCT_H
#define _CALLMGMSTRUCT_H

#include "MMI_Data_Types.h"
#define ENCODING_LENGTH 1

typedef struct MYTIME
{
	U16	  nYear;
	U8	  nMonth;
	U8	  nDay;
	U8	  nHour;
	U8	  nMin;
	U8	  nSec;
	U8	  DayIndex; /* 0=Sunday */
}MYTIME;

#define MAX_CALL_OPTIONS		15
#define MAX_PBNAME				22
#define MAX_CALLS				7
#define MAX_HOLD				5
#define CM_NOTIFY_TIMEOUT		2000//typical notify timeout for CM
#define CM_AUTOANSWER_NOTIFY_TIMEOUT		2000//typical notify timeout for CM
#define CM_CCBS_NOTIFY_TIMEOUT		10000//ccbs notify timeout for CM

#define	CM_WAITFORSS_TIMEOUT	1000
#define MAX_DIGIT				41	// Max digits in DTMF Buf


#define MAX_CM_SCREEN_DIGITS_ANYKEY	17// Idle screen digits	
#define MAX_CM_SCREEN_DIGITS	13// Idle screen digits	

#ifndef MMI_ON_HARDWARE_P

#define MAX_DIGITS_USSD			182

#endif 

#define CM_HANGUP_REQUESTED		0x01
#define CM_HOLD_REQUESTED		0x02
#define CM_RETRIEVE_REQUESTED	0x04
#define CM_SWAP_REQUESTED		0x08
#define CM_CONF_REQUESTED		0x10
#define CM_SPLIT_REQUESTED		0x20


#ifdef MMI_ON_HARDWARE_P
	#define MAX_NUMBER			(MAX_CC_ADDR_LEN*ENCODING_LENGTH)
	#define MAX_NAME			L4C_MAX_NAME_LEN
#else
	#define MAX_NUMBER			(40*ENCODING_LENGTH)
	#define MAX_NAME			40
#endif
#define MAX_PHB_NAME			20

typedef S16 CM_CALL_HANDLE;

typedef enum CmEventTypes
{
	CM_PS_CALL_INCOMING,	
	CM_PS_INCOMING_CALL_CONNECTED,
	CM_PS_INCOMING_CALL_REJECTED,
	CM_PS_INCOMING_CALL_DROPPED,
	CM_PS_HELD_CALL_DISCONNECTED,
	CM_PS_INCOMING_CALL_FORWARDED,
	CM_PS_ALL_ACTIVE_CALL_DISCONNECTED,
	CM_PS_ACTIVE_CALL_DISCONNECTED,
	CM_PS_ACTIVE_CALL_HELD,
	CM_PS_HELD_CALL_ACTIVE,
	CM_PS_CALL_INCOMINGDROPPED,
	CM_PS_HELD_CALL_ENDED,
	CM_PS_UDUB_SENT,
	CM_PS_NWK_CALL_DROP,
	CM_PS_ALL_CALLS_ENDED,
	CM_PS_AOC_INDICATION, 
	CM_PS_CALLCONNECTED,	//associated data structure is the int(callIndex).
	CM_PS_HANGUPSUC,		//associated data structure is the int(callIndex).
	CM_PS_HOLDSUC,			//associated data is void
	CM_PS_RETRIEVESUC,		//associated data is void
	CM_PS_SWAPSUC,			//associated data is void
	CM_PS_HLD_ACT_SWAPSUC,
	CM_PS_CONFSUC,			//associated data is void
	CM_PS_HANGUPALLSUC,		//associated data is void
	CM_PS_HANGUPALLACTSUC,	//associated data is void
	CM_PS_HANGUPALLHLDSUC,	//associated data is void
	CM_PS_SPLITSUC,			//associated data structure is the int(callIndex).
	CM_PS_USSD_RECVD,
	CM_PS_ECT_SUC,
	CM_PS_ECT_HANGUPALLSUC,
	CM_PS_SPEECH_IND,
	CM_PS_CHECK_CALL_LIST_STATE,
	CM_PS_ACTIVE_HELD_IC_ACCPT,
	CM_PS_SHOW_OUTGOING_CALL_SCREEN,
	CM_PS_SHOW_REQUESTING_CALL_SCREEN,
	CM_PS_CALL_WAIT,
	CM_PS_SS_EVENT,
	CM_PS_INCOMING_CALL_DEFLECT,
	CM_PS_CCBS_ACTIVATE,
	CM_PS_ACTIVATED_CCBS,
	CM_PS_CCBS_INVOKE,
	CM_PS_CCBS_INVOKE_DIAL,
	CM_PS_CCBS_INVOKE_REJECT,
	CM_PS_ACCEPT_INCOMING_FOR_SAT,

	CM_KB_HANGUPALLREQ,		
	CM_KB_OUTGOINGCALL,		
	CM_KB_HANGUPREQ,		
	CM_KB_HOLDREQ,			
	CM_KB_RETRIEVEREQ,		
	CM_KB_SWAPREQ,			
	CM_KB_CONFREQ,			
	CM_KB_HANGUPALLACTREQ,	
	CM_KB_HANGUPALLHLDREQ, 
	CM_KB_SPLITREQ,		

	CM_KB_INCOMING_CALL_ACCEPT,
	CM_KB_INCOMING_CALL_REJECT,
	CM_KB_ACTIVE2HOLD,
	CM_KB_HOLD2ACTIVE,
	CM_KB_END_ALL_ACTIVE,
	CM_KB_END_ALL_ACTIVE_RETRIEVE_HELD,
	CM_KB_END_HELD_CALLS,
	CM_KB_END_ACTIVE_ACCEPT_INCOMING,
	CM_KB_END_ACTIVE_EXCEPT_INCOMING,
	CM_KB_END_ALL_CALLS,
	CM_KB_HOTKEY_PRESSED,
	CM_KB_UDUB,			
	CM_KB_ECT_REQ,			
	CM_KB_CALL_DEFLECT_REQ,
	CM_KB_ACCEPT_INCOMING ,
	CM_KB_HLD_ACT_SWAPREQ
}CM_EVENT_TYPES;

typedef enum ACTION_RESULT
{
	CM_CALL_SUCCESS,
	CM_CALL_FAILURE,
	CM_NO_INCOMING
}ACTION_RESULT;

typedef enum CHLD_REQ_ACTION
{
	CM_HANGUPALL_REQ_SENT=1,
	CM_HANGUPALLACTIVE_REQ_SENT,
	CM_HANGUPALLHLD_REQ_SENT,
	CM_ENDSELECTIVE_REQ_SENT,
	CM_UDUB_REQ_SENT
}CHLD_REQ_ACTION;

typedef enum CallState {
	CM_IDLE_STATE=0,
	CM_OUTGOING_STATE,
	CM_INCOMING_STATE,
	CM_ACTIVE_STATE,
	CM_HOLD_STATE,
	CM_STATE_UNKNOWN
}CALL_STATE;

typedef enum CallDir {
	CM_CALL_MO=1,
	CM_CALL_MT,
	CM_CALL_MO_STK
}CALL_DIR;


typedef struct _inc_callinfo
{
	U8				number[MAX_NUMBER];	
	U8				pBname[MAX_PHB_NAME*ENCODING_LENGTH];				
	U8				name_dcs;
	CALL_STATE			prev_state;		
	CALL_STATE			curr_state;		
	MYTIME				start_time;		
	MYTIME				end_time;		
	CALL_DIR			orgination_flag;	
	U8				status_flag;		
	CM_CALL_HANDLE		        call_handle;		
}CALL_INFO;

typedef struct _outgoingcall
{
//micha0729 
//	U8	Number[MAX_NUMBER];	//Calling Number.
	U8	Number[MAX_DIGITS_USSD* ENCODING_LENGTH];
	PS8	Name;	
}OUTGOING_CALL;

typedef struct l4_result{
	U8 result;
	U16 cause;
}RESULT;

typedef struct _ussdData{
	PU8 data;
	U16 length;
}USSD_DATA;

typedef struct _StrHandle {
		U8				result;
		U16				cause;
		CM_CALL_HANDLE	handle;
}STR_HANDLE;

typedef struct _ccmValue {
	U32 ccm;
	U8 warning;
}CCM_VALUE;

typedef struct IncomingCall
{
	U8 Name[MAX_NAME];
	U8 Number[MAX_NUMBER];
	S16 callHandle;
}IncomingCall; 

typedef char           kal_uint8;
typedef short int      kal_uint16;
#define MAX_CC_ADDR_LEN          41
#define MAX_CALL_HISTORY_ENTRIES	20

#define LOCAL_PARA_HDR \
   U8	ref_count; \
   U16	msg_len;

typedef struct l4c_result_struct
{
   kal_uint8   flag;    
   kal_uint16   cause;
} l4c_result_struct;

typedef struct
{
   kal_uint8 type;
   kal_uint8 length;
   kal_uint8 number[MAX_CC_ADDR_LEN];
} l4c_number_struct;

typedef struct l4_name_struct
{
	kal_uint8 name_length;
	kal_uint8 name_dcs;
	kal_uint8 name[32];
} l4_name_struct;

typedef struct {
    kal_uint8 rtc_sec;
    kal_uint8 rtc_min;
    kal_uint8 rtc_hour;
    kal_uint8 rtc_day;
    kal_uint8 rtc_mon;
    kal_uint8 rtc_wday;
    kal_uint8 rtc_year;
} rtc_format_struct;

typedef struct {
   l4c_number_struct tel;
   l4_name_struct alpha_id;
   rtc_format_struct time;
   kal_uint8 count;
} l4c_phb_ln_entry_struct;

typedef struct
{
	LOCAL_PARA_HDR
	l4c_result_struct	result;
	kal_uint8	no_list;
	kal_uint8	type;
	kal_uint8	more_data;
	l4c_phb_ln_entry_struct	list[1];
}GET_LAST_NUMBER_RSP_STRUCT;

typedef struct
{
	LOCAL_PARA_HDR
	l4c_result_struct	result;
	kal_uint16	no_list;
	l4c_phb_ln_entry_struct	list[1];
}SET_LAST_NUMBER_RSP_STRUCT;

typedef struct
{
	LOCAL_PARA_HDR
	kal_uint8	type;
	kal_uint8	no_data;
	kal_uint8	index;
	l4c_phb_ln_entry_struct	entry;
} MMI_PHB_SET_LAST_NUMBER_REQ_STRUCT;

typedef struct
{
	LOCAL_PARA_HDR
	kal_uint8	type;
} MMI_PHB_GET_LAST_NUMBER_REQ_STRUCT;
typedef struct
{
	LOCAL_PARA_HDR
	kal_uint8	type;
	kal_uint8	index;
	kal_uint8	no_data;
} MMI_PHB_DEL_LAST_NUMBER_REQ_STRUCT;
/*
typedef struct
{
	LOCAL_PARA_HDR
	l4c_result_struct	result;
	kal_uint8	call_id;
} MMI_CC_DIAL_IND_STRUCT;
*/

typedef struct
{
   kal_uint8 type;
   kal_uint8 length;
   kal_uint8 number[MAX_CC_ADDR_LEN];
} l4c_sub_addr_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_uint8	call_id;
	l4c_number_struct	num;
	l4c_sub_addr_struct	sub_addr;
	kal_uint8	name[30];
	kal_uint8	auto_answer;
} MMI_CC_CALL_RING_IND_STRUCT;

void *SetLastNoRsp(UINT32 nSimNum);
void *GetLastNoRsp(UINT32 nSimNum);
void * DeleteAllCallsRsp(UINT32 nSimNum);
//void * IncomingCallInd(void);

void CallHistoryWriteToFile(kal_uint16 FileType,UINT32 nSimNum); 
void CallHistoryReadFromFile(kal_uint16 FileType,UINT32 nSimNum);

//void * DialIndPlugIn();

#endif //_CALLMGMSTRUCT
