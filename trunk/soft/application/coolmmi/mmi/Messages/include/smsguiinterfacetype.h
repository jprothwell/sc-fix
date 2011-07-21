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
 * SmsGuiInterfaceType.h
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: SmsGuiInterfaceType.h

  	PURPOSE		: ???

 

	AUTHOR		: Manu Vikram and Magesh k

	DATE		: ???

**************************************************************/

#ifndef _PIXCOM_SMS_GUI_INTERFACE_TYPE_H
#define _PIXCOM_SMS_GUI_INTERFACE_TYPE_H
#include "mmi_data_types.h"
#include "messagesl4def.h"
#include "messagesmiscell.h"

#include "customer_ps_inc.h"
#include "sms_common_setting.h" 

#if defined(__MMI_MESSAGES_TEMPLATE__)
#ifdef MMI_ON_HARDWARE_P
#include "nvram_user_defs.h"
#endif
#ifndef NVRAM_SMS_TEMPL_RECORD_SIZE
#define NVRAM_SMS_TEMPL_RECORD_SIZE	     84
#endif
#endif 


#define	MAX_USE_NUMBERS					6
#ifdef	__MMI_MESSAGES_USE_URL__
#define		MAX_URL_LEN		(256)
#define TO_LOWER(ch)	( (((ch)>='A') && ((ch)<='Z'))?('a'+((ch)-'A')) :((ch))  )
#endif	/* __MMI_MESSAGES_USE_URL__ */
#define MAX_WAITING_TYPE_NUM		4
#define MAX_WAITING_LINE_NUM		2
#define MAX_SEND_TO_MANY_ITEMS	10

typedef struct
{
	U8 number[MAX_WAITING_TYPE_NUM];
}msg_waiting_ind_struct;


typedef struct
{
	U8 delivered;
	U8 type;
	U8 number[20];
	U8 timestamp[6];
}msg_deliver_report_struct;

typedef struct
{
	U16 length;	
	U8 fo;
	U8 dcs;
	U8 totalseg;
	U8 number[(MAX_DIGITS_SMS+1)*ENCODING_LENGTH];
	U8 timestamp[6];
	U8* data;
}msg_class0_msg_struct;

typedef struct
{
	/* send by group */
	U16 TotalSendSmsNumbers;
	U16* PhbIndex;
	/* send to many */
	U16 CurrSendSmsNumbers;	
	S8 PhbNumber[MAX_SEND_TO_MANY_ITEMS][(MAX_DIGITS+2)*ENCODING_LENGTH];
#ifdef __MMI_MESSAGES_CONTINUE_SEND_AFTER_FAIL__
	U8 FailedIndex[MAX_SEND_TO_MANY_ITEMS];
	U8 FailedNumber;
	U8 FailedSaveNumber;
#endif
}msg_send_many_struct;

typedef enum 
{
	MSG_NEW_NONE,
	MSG_NEW_VOICEMAIL,
	MSG_NEW_FAX,
	MSG_NEW_EMAIL,

	MSG_NEW_MSG_NORMAL,
#if defined(__MMI_SMART_MESSAGE_MT__) ||( defined(__MMI_SMART_MESSAGE_MO__)&&!defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
#endif
	MSG_NEW_MSG_CLASS0,
	MSG_NEW_DELIVERY_REPORT
}msg_new_msg_enum;


typedef enum {
	SEND_CASE_SEND_ONLY,
	SEND_CASE_SAVE_N_SEND,
	SEND_CASE_SAVE_ONLY,
	SEND_CASE_REPLY,
	SEND_CASE_SEND_TO_MANY,
	SEND_CASE_SEND_BY_GROUP,
	SEND_CASE_SEND_FROM_PHB,
	SEND_CASE_SEND_VCARD,
	SEND_CASE_SEND_CHAT,
	SEND_CASE_SEND_CLUB_CODE,
	SEND_CASE_SEND_FROM_CLG,//wangwb @2007.06.12
	SEND_CASE_SEND_NONE
}msg_send_case_enum;

typedef enum
{
	SEND_SAVE_NIL = 0,
	SEND_SUCCESS,
	SEND_FAILED,
	SAVE_SUCCESS,
	SAVE_FAILED,
	SEND_SUCESS_SAVE_FILED,
	SEND_FAILED_SAVE_SUCCESS,
	SEND_SAVE_SUCESS,
	SEND_SAVE_FAILED,
	SAVE_ABORT_SENDING_SMS,
	SAVE_WHEN_SC_IS_EMPTY,
	SAVE_WHEN_SC_IS_EMPTY_FAILED,
	SAVE_WHEN_SC_IS_EMPTY_SUCCESS,
	SAVE_AFTER_FDN_CHECK,
	SAVE_AFTER_FDN_CHECK_FAILED,
	SAVE_AFTER_FDN_CHECK_SUCCESS
}msg_send_save_status_enum; 

typedef enum
{
	TO_DISPLAY_MESSAGE_LIST_NONE,
	TO_DISPLAY_MESSAGE_LIST_INBOX,
	TO_DISPLAY_MESSAGE_LIST_OUTBOX,
#ifdef __MMI_MESSAGES_DRAFT_BOX__
	TO_DISPLAY_MESSAGE_LIST_DRAFTBOX,
#endif
	TO_DISPLAY_MESSAGE_LIST_INBOX_AFTER_DELETE_ALL,
	TO_DISPLAY_MESSAGE_LIST_OUTBOX_AFTER_SAVE_MSG,
	TO_DISPLAY_MESSAGE
}msg_display_list_enum;


typedef enum
{
	MSG_STATUS_IDLE,
	MSG_STATUS_INPUT_NUM
}msg_status_enum;

#ifdef __MMI_GPRS_FEATURES__
typedef enum
{
	MMI_SMS_PREFER_GPRS,
	MMI_SMS_PREFER_GSM,
	MMI_SMS_GPRS_ONLY,
	MMI_SMS_GSM_ONLY
} msg_bearer_enum;
#endif

#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__)&&!defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

#ifdef __MMI_MESSAGES_USE_URL__
#define IS_GSM_SPECIAL_CHAR(c)	(( (c)==0x000C || (c)==0x005B || (c)==0x005C || (c)==0x005D || (c)==0x005E || (c)==0x007B || (c)==0x007C || (c)==0x007A || (c)==0x007E))
#define IS_GSM_CHAR(c)	(((c)<=0x007f || IS_GSM_SPECIAL_CHAR((c)))?1:0)
#endif /* __MMI_MESSAGES_USE_URL__ */

typedef struct
{
	S32	currHiliteIndex;
	S32	currBoxIndex;
	U16	MessagesScrnIdToDelHistoryNodes;
	U16	MessagesScrnIdDelUptoHistoryNodes;
	
	U16 	msg_ind_index;
	U16	msg_ind_chat_index;
	U8 	msg_ind_in_idle;
	U8 	msg_ind_after_call;
	
	U8 	msg_class0_ind;
	U8	msg_full_ind[MMI_SIM_NUMBER];
	U8 	msg_exceed_ind[MMI_SIM_NUMBER];
	U8 	msg_deliver_ind;
	U8 	msg_waiting_ind;
	
	U8 	msg_init; //init to be false
	U8	number_from_phb; //init to be 0
	
	S8	smsPhoneNumber [(MAX_DIGITS+1)*ENCODING_LENGTH];
	#ifdef __MMI_MESSAGES_TEMPLATE__
	S8*	templates [NUM_TEMPLATES] ;
	S8	scratchTemplRec [NVRAM_SMS_TEMPL_RECORD_SIZE+ENCODING_LENGTH] ;
	#endif
	
	S8* numbersList [MAX_USE_NUMBERS] ;
#ifdef __MMI_MESSAGES_USE_URL__
	S8 *URLlist[MAX_USE_NUMBERS];
	U16 URLCount;
#endif /* __MMI_MESSAGES_USE_URL__ */
	U8 	PhbSmsInterfaceState; //MMI_PHB_SMS_INTERFACE
	#ifdef __MMI_MULTI_SIM__
	U8 newMsgSimID;
	BOOL is_voicemail_from_idle;
	BOOL needSelectSIM;
	#endif
	msg_display_list_enum		toDisplayMessageList; //init to be TO_DISPLAY_MESSAGE_LIST_NONE
	msg_send_case_enum 			sendMessageCase;
	msg_send_save_status_enum 		sendSaveStatus;
	msg_status_enum 			msg_status; /* store the status in number editing screen */

	
	msg_waiting_ind_struct 	msg_waiting_info[MAX_WAITING_LINE_NUM]; //init to zero
	msg_class0_msg_struct* msg_class0_p; //init to NULL
	msg_deliver_report_struct* msg_deliver_report[MAX_STATUS_REPORT_NUM]; //init to NULL
	msg_send_many_struct msg_send_info;
#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__)&&!defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
#endif
}msg_context_struct;

extern msg_context_struct g_msg_cntx;

#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__)&&!defined(__MMI_MESSAGES_EMS__))
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
#endif	/* __MMI_SMART_MESSAGE_MT__ || __MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__  */

#if (defined(__MMI_SMART_MESSAGE_MO__)&&!defined(__MMI_MESSAGES_EMS__))
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
#endif /* __MMI_SMART_MESSAGE_MO__*/

#endif	/* _PIXCOM_SMS_GUI_INTERFACE_TYPE_H */


