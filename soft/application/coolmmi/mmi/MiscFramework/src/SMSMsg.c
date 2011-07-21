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
 *  SMSMsg.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
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
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifdef __MOD_SMSAL__

#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

//... Add More MMI header
#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "unicodexdcl.h"
#include "timerevents.h"
#include "debuginitdef.h"
#include "protocolevents.h"

//...Add MSG header
#include "customer_ps_inc.h"
#include "mmi_msg_context.h"
#include "messagesl4def.h"
#include "messagesresourcedata.h"
#include "messagesmiscell.h"
#include "messagesexdcl.h"
#include "smsguiinterfaceprot.h"
#include "smsguiinterfacetype.h"
#include "smspshandler.h"
#include "cellbroadcasttype.h"
#ifdef __MMI_MESSAGES_CHAT__
#include "chatappgprot.h"
#include "chatappdef.h"
#endif
//
#include "customer_ps_inc.h"
#include "l4c2smsal_struct.h"
#include "smsal_l4c_enum.h"
#include "ems.h"
//
#include "gui_ems.h"
#include "wgui_ems_categories.h"
#include "gsm7bitdefaultalphabet.h"
//
#include "callhistorygprots.h"
#include "idleappprot.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "alarmframeworkprot.h"
#include "phonebooktypes.h"
#include "simdetectiongexdcl.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "csp.h"
#ifdef MMI_ON_HARDWARE_P
#include "sim_common_enums.h"
#endif
//
#include "wgui_categories_inputs.h"
#include "wgui_status_icons.h"
#include "keybrd.h"
//
#include "filesystemdef.h"
#include "filesystemgprot.h"
//
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "nvram_data_items.h"
//
#undef  __NEWSIMULATOR

#include "smsapi.h"
#include "smsstruct.h"
#include "smsfunc.h"

// add by wuzc for debug
#include "mmi_trace.h"
#include "cmerrortypes.h"

//add by wangwb for long sms
#include "customer_ps_inc.h"

#if defined(__MMI_MESSAGES_USE_URL__)
#include "wap_ps_struct.h"
#endif	/* __MMI_MESSAGES_USE_URL__ */

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_calllog.h"
MMI_BOOL g_mmi_frm_sms_list_finish[MMI_SIM_NUMBER] = {FALSE,};
#endif

#ifndef MMI_FRM_SMS_INVALID_INDEX
#define MMI_FRM_SMS_INVALID_INDEX	0xffff
#endif
#ifndef MMI_FRM_SMS_INVALID_NUM
#define MMI_FRM_SMS_INVALID_NUM		0xff
#endif
#ifndef MMI_FRM_SMS_MSG_LEN
#define MMI_FRM_SMS_MSG_LEN		160
#endif
#ifndef MMI_FRM_SMS_MSG_SEQ_NUM
#define MMI_FRM_SMS_MSG_SEQ_NUM	5
#endif
#ifndef MMI_FRM_SMS_ACTION_SIZE
#define MMI_FRM_SMS_ACTION_SIZE		10
#endif

#define MMI_FRM_SMS_SEND_SEG		1


static msg_profile_list_struct* profileList;
static msg_profile_edit_struct* profileDetail;
static msg_mailbox_edit_struct* mailboxDetail;
#ifdef __MMI_MESSAGES_COPY__
static mmi_frm_sms_copy_result_struct* copyResult;
#endif
#ifdef __MMI_MULTI_SIM__
mmi_frm_sms_send_struct* sendSMS;
#else
static mmi_frm_sms_send_struct* sendSMS;
#endif
static mmi_frm_sms_send_struct* saveSMS;
static U16 sendResult=MMI_FRM_SMS_OK;
static U16 readIndex;

static U8 ems_temp[612];      //wangwb @2007.06.16
static UINT16 len_temp = 0;       //wangwb @2007.06.16

static mmi_frm_sms_register_struct mmi_frm_sms_interrupt_handler[MMI_FRM_SMS_ACTION_SIZE];
static U8 mmi_frm_sms_interrupt_handler_size=0;

extern EMSData * GetEMSDataForView              (EMSData ** p, U8 force) ;
extern EMSData * GetEMSDataForEdit              (EMSData ** p, U8 force) ;
extern U8        DeInitMessagesEMSBuffer        (msg_ems_data_struct * pE);

#ifdef __MMI_MULTI_SIM__
extern BOOL  AllowFixedDial(PS8 strNumber, UINT8 nSimId);
#else
extern BOOL  AllowFixedDial(PS8 strNumber);   //wangwb @2007.06.20
#endif


//begin: add by panxu for delete all 2006-8-9
mmi_frm_sms_msgbox_enum msgbox_type;
//end: add by panxu for delete all 2006-8-9

#if ( defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
#endif

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_send_message
* DESCRIPTION
*   Common function for sending message
*
* PARAMETERS
*  MOD_SRC	IN	source module
*  MOD_DEST	IN	destination module
*  MSG_SAP	IN	message SAP
*  MSG_ID	IN	message ID
*  LOCAL	IN	local parameter
*  PEER	IN	peer buffer
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_send_message(module_type MOD_SRC, module_type MOD_DEST, U8 MSG_SAP, const U32/*U16*/ MSG_ID, void* LOCAL1, void* PEER)   
{													
	MYQUEUE		Message;	 
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_send_message");

	Message.oslSrcId	=	MOD_SRC;	
	Message.oslDestId=	MOD_DEST;	
	Message.oslMsgId	=	MSG_ID;	
	Message.oslDataPtr	=	LOCAL1;	
	Message.oslPeerBuffPtr	=	PEER;	

	OslMsgSendExtQueue(&Message);	
}									


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_reg_interrupt_handler
* DESCRIPTION
*   Register interrupt handle
*
* PARAMETERS
*  mod	IN	module type
*  msgid	IN	message id
*  callback	IN	callback of interrupt
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_reg_interrupt_handler(module_type mod, /*U16*/U32 msgid, PsFuncPtrU16 callback)   
{
	mmi_frm_sms_interrupt_handler[mmi_frm_sms_interrupt_handler_size].molude=mod;
	mmi_frm_sms_interrupt_handler[mmi_frm_sms_interrupt_handler_size].msgid=msgid;
	mmi_frm_sms_interrupt_handler[mmi_frm_sms_interrupt_handler_size].callback=callback;
	mmi_frm_sms_interrupt_handler_size++;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_frm_sms_interrupt_callback
 * DESCRIPTION
 *  callback interrupt handle
 * PARAMETERS
 *  mod             [IN]        Module type
 *  msgid           [IN]        Message id
 *  inMsg           [IN]        msg
 *  result			[IN]		result
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_frm_sms_interrupt_callback(module_type mod, U32 msgid, void *inMsg, U16 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U8 i = 0;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (i < mmi_frm_sms_interrupt_handler_size)
    {
        if (mmi_frm_sms_interrupt_handler[i].msgid == msgid)
        {
            mmi_frm_sms_interrupt_handler[i].callback(inMsg, mod, result);
        }
        i++;
    }
}

BOOL mmi_frm_sms_is_ready(void)
{
	U8 index =0;
	for(index = 0; index < MMI_SIM_NUMBER; index++)
	{
		if(g_frm_sms_cntx.mmi_frm_sms_ready[index] == FALSE)
			return FALSE;
	}
	return TRUE;
	//return g_frm_sms_cntx.mmi_frm_sms_ready;
}
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_ready_ind
* DESCRIPTION
*   Handle ready indication
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_ready_ind (void* dummy)
{
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	g_frm_sms_cntx.mmi_frm_sms_ready[nSimID]=TRUE;
	
	mmi_frm_sms_get_profile_list(NULL, MOD_MMI, nSimID);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_ready_ind");

#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
	 
#endif 
	//mmi_msg_pre_entry_cb_channel_bootup();   
	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX) != MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_frm_sms_interrupt_callback(MOD_MMI, PRT_MSG_ID_MMI_SMS_READY_IND, NULL, MMI_FRM_SMS_OK); 
	}
	 
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_startup_read_req
* DESCRIPTION
*   SMS startup read request
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_startup_read_req (U8 nSimID)
{
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + nSimID, 0, PRT_MSG_ID_MMI_SMS_STARTUP_READ_NEXT_REQ, NULL, NULL);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_startup_read_ind
* DESCRIPTION
*   Handle SMS startup read indication
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_startup_read_ind (void* inMsg)
{
	MMI_FRM_SMS_STARTUP_READ_MSG_IND_STRUCT* msgInd = (MMI_FRM_SMS_STARTUP_READ_MSG_IND_STRUCT*)inMsg;
	mmi_frm_sms_msgbox_struct * entry=OslMalloc(sizeof(mmi_frm_sms_msgbox_struct));
	U8 sendreq=(msgInd->startup_seq_num==0)?1:0;
	U8 seg=(msgInd->concat_info & 0xff000000)>>24;
	U8 thisseg=((seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)==0)?g_frm_sms_cntx.mmi_frm_sms_msg_seg_size:(seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size);
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_startup_read_ind>>msgInd->startup_seq_num is:%d,msgInd->index is %d",msgInd->startup_seq_num,msgInd->index);

	if(msgInd->startup_seq_num==0)
		sendreq=TRUE;

	mmi_frm_sms_convert_startup_read_to_entry((MMI_FRM_SMS_STARTUP_READ_MSG_IND_STRUCT*)msgInd, entry);
	mmi_frm_sms_add_sms_to_msgbox(entry, msgInd->index, thisseg);
	OslMfree(entry);
	
	if(sendreq)
		mmi_frm_sms_startup_read_req(nSimID);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_startup_finish_ind
* DESCRIPTION
*   Handle SMS startup finish indication
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_startup_finish_ind (void* dummy)
{
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_startup_finish_ind");
	
	#ifdef __MMI_MULTI_SIM__
	g_mmi_frm_sms_list_finish[nSimID] = TRUE;
	#endif
	
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_STARTUP_READ_MSG_IND,nSimID);
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_STARTUP_FINISH_IND, nSimID);
	mmi_frm_sms_create_sms_list();

	
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_startup_begin_ind
* DESCRIPTION
*   Handle SMS startup begin indication
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_startup_begin_ind (void* dummy)
{
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_startup_begin_ind(%d)", nSimID);
	/* init msgbox in case of sim refresh */
	#ifndef __MMI_MULTI_SIM__
	mmi_frm_sms_init_msgbox();
	#else /*deined __MMI_MULTI_SIM__ */
	if (!MTPNP_AD_Msgbox_Is_Init())
	{
		mmi_frm_sms_init_msgbox();
		MTPNP_AD_Msgbox_HasInit();
	}
	else
	{
		//now we should not handle this
		//MTPNP_PFAL_SMS_Unload(nSimID);
	}
	#endif
	SetProtocolEventHandlerExt(mmi_frm_sms_startup_read_ind, PRT_MSG_ID_MMI_SMS_STARTUP_READ_MSG_IND,nSimID);
	SetProtocolEventHandlerExt(mmi_frm_sms_startup_finish_ind, PRT_MSG_ID_MMI_SMS_STARTUP_FINISH_IND,nSimID);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_status_report_ind
* DESCRIPTION
*   Handle status report
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_status_report_ind (void* inMsg)
{
	MMI_FRM_SMS_STATUS_REPORT_IND_STRUCT* msgInd = (MMI_FRM_SMS_STATUS_REPORT_IND_STRUCT*)inMsg;
	U8 i=0;

#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
	U8 msg_ref = msgInd->mr; 
	U16 delivery_report_awaited_index; 
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_status_report_ind");

	/*check if mr is in delivery report awaited list*/
	delivery_report_awaited_index = mmi_frm_sms_get_delivery_report_index_by_mr(msg_ref);

	/* mr is in delivery report awaited list */
	if(delivery_report_awaited_index != MMI_FRM_SMS_ACTION_SIZE )
	{		
		mmi_frm_sms_set_delivery_report_awaited_check(delivery_report_awaited_index, msg_ref, msgInd->st);

		if(mmi_frm_sms_check_delivery_report_awaited_complete(delivery_report_awaited_index))
		{
			msgInd->st = mmi_frm_sms_get_delivery_report_awaited_result(delivery_report_awaited_index);
			mmi_frm_sms_remove_delivery_report_awaited(delivery_report_awaited_index);

	mmi_frm_sms_convert_time_stamp(msgInd->dt, msgInd->dt);
	while(i<mmi_frm_sms_interrupt_handler_size)
	{
		if(mmi_frm_sms_interrupt_handler[i].msgid==PRT_MSG_ID_MMI_SMS_STATUS_REPORT_IND)
		{
			mmi_frm_sms_interrupt_handler[i].callback(inMsg, MOD_MMI, MMI_FRM_SMS_OK);
			break;
		}
		i++;
	}
}
	}
#else
	
	mmi_frm_sms_convert_time_stamp(msgInd->dt, msgInd->dt);
	while(i<mmi_frm_sms_interrupt_handler_size)
	{
		if(mmi_frm_sms_interrupt_handler[i].msgid==PRT_MSG_ID_MMI_SMS_STATUS_REPORT_IND)
		{
			#ifdef __MMI_MULTI_SIM__
			g_msg_cntx.newMsgSimID = MTPNP_AD_get_protocol_event_src_mode();
			#endif
			mmi_frm_sms_interrupt_handler[i].callback(inMsg, MOD_MMI, MMI_FRM_SMS_OK);
			break;
		}
		i++;
	}
#endif  // __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_msg_waiting_ind
* DESCRIPTION
*   Handle message waiting indication
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_msg_waiting_ind (void* inMsg)
{
	U8 i=0;
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_msg_waiting_ind");
	while(i<mmi_frm_sms_interrupt_handler_size)
	{
		if(mmi_frm_sms_interrupt_handler[i].msgid==PRT_MSG_ID_MMI_SMS_MSG_WAITING_IND)
		{
			mmi_frm_sms_interrupt_handler[i].callback(inMsg, MOD_MMI, MMI_FRM_SMS_OK);
			break;
		}
		i++;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_mem_full_ind
* DESCRIPTION
*   Handle memory full indication
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_mem_full_ind (void* inMsg)
{
	U8 i=0;
	while(i<mmi_frm_sms_interrupt_handler_size)
	{
		if(mmi_frm_sms_interrupt_handler[i].msgid==PRT_MSG_ID_MMI_SMS_MEM_FULL_IND)
		{
			mmi_frm_sms_interrupt_handler[i].callback(inMsg, MOD_MMI, MMI_FRM_SMS_OK);
			break;
		}
		i++;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_mem_exceed_ind
* DESCRIPTION
*   Handle memory exceed indication
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_mem_exceed_ind (void* inMsg)
{
	U8 i=0;
	while(i<mmi_frm_sms_interrupt_handler_size)
	{
		if(mmi_frm_sms_interrupt_handler[i].msgid==PRT_MSG_ID_MMI_SMS_MEM_EXCEED_IND)
		{
			mmi_frm_sms_interrupt_handler[i].callback(inMsg, MOD_MMI, MMI_FRM_SMS_OK);
			break;
		}
		i++;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_mem_avai_ind
* DESCRIPTION
*   Handle memory available indication
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_mem_avai_ind (void* inMsg)
{
	U8 i=0;
	while(i<mmi_frm_sms_interrupt_handler_size)
	{
		if(mmi_frm_sms_interrupt_handler[i].msgid==PRT_MSG_ID_MMI_SMS_MEM_AVAILABLE_IND)
		{
			mmi_frm_sms_interrupt_handler[i].callback(inMsg, MOD_MMI, MMI_FRM_SMS_OK);
			break;
		}
		i++;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_new_msg_ind
* DESCRIPTION
*   Callback new message indication
*
* PARAMETERS
*  index	IN	new SMS index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_new_msg_ind (U16 index)
{
	U8 i=0;
	U16* data=OslMalloc(sizeof(U16));
	*data=index;
	while(i<mmi_frm_sms_interrupt_handler_size)
	{
		if(mmi_frm_sms_interrupt_handler[i].msgid==PRT_MSG_ID_MMI_SMS_DELIVER_MSG_IND)
		{
			mmi_frm_sms_interrupt_handler[i].callback((void*)data, MOD_MMI, MMI_FRM_SMS_OK);
			break;
		}
		i++;
	}
	OslMfree(data);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_new_sms_ind
* DESCRIPTION
*   Handle new SMS indication
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern U16 current_chat_sms_index;
void mmi_frm_sms_new_sms_ind (void* inMsg)
{
	MMI_FRM_SMS_DELIVER_MSG_IND_STRUCT* msgInd = (MMI_FRM_SMS_DELIVER_MSG_IND_STRUCT*)inMsg;
	mmi_frm_sms_deliver_msg_struct* data=OslMalloc(sizeof(mmi_frm_sms_deliver_msg_struct));

	 
	mmi_frm_sms_convert_new_sms(msgInd, data);
	
#ifdef __ZYX_PLATFORM_SUPPORT__
#ifdef __MMI_MULTI_SIM__
	if (data->index < MTPNP_AD_Get_Card2_StartIdx_In_Msgbox())
	{
		extern BOOL ZyxSmsCheck(void *,void *);
		if (ZyxSmsCheck(inMsg,data))
			return;
	}
	else
	{
		extern BOOL ZyxSmsSlaveCheck(void *,void *);
		if (ZyxSmsSlaveCheck(inMsg,NULL))
			return;
	}
#else  /* No Defined __MMI_MULTI_SIM__ */
	{
		extern BOOL ZyxSmsCheck(void *inMsg,void *data);
		if (ZyxSmsCheck(inMsg,data))
			return;
	}
#endif
#endif
	//add by chenq 20080325 B
	current_chat_sms_index = data->index;
	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_msg_handle_new_msg_ind, current_chat_sms_index is: %d",current_chat_sms_index);
	//add by chenq 20080325 B
	
 	#ifdef __MMI_MULTI_SIM__
	{
		U8 nsimID;
		nsimID = mmi_frm_msg_get_simid_by_l4_index(data->index);
		CHISTIncRecvdSMS(nsimID);
	}
	#else  /* No Defined __MMI_MULTI_SIM__ */
	CHISTIncRecvdSMS(0);   //短信计数器应用
	#endif /* __MMI_MULTI_SIM__ */

	/*put to awaited list last entry, from now on only process new data as awaited list last entry*/
	/*modified by wuzc at 2006818*/
	#if 1
		mmi_frm_sms_add_new_sms(data, msgInd->no_msg_data, msgInd->msg_data); 
	#else
		mmi_frm_sms_add_new_sms(data, msgInd->no_msg_data, msgInd->pdata);//添加数据到列表中 
	#endif
	/*process new sms data*/
	mmi_frm_sms_new_sms();
	
	OslMfree(data);
}

 
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_new_data_ind
* DESCRIPTION
*   Handle new SMS with port number indication
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_new_data_ind (void *inMsg)
{
	MMI_FRM_SMS_APP_DATA_IND_STRUCT* msgInd = (MMI_FRM_SMS_APP_DATA_IND_STRUCT*)inMsg;
	mmi_frm_sms_deliver_data_struct* data=OslMalloc(sizeof(mmi_frm_sms_deliver_data_struct));

	 
	mmi_frm_sms_convert_new_data(msgInd, data);

	/*put to awaited list last entry, from now on only process new data as awaited list last entry*/
	mmi_frm_sms_add_new_data(data, msgInd->message_len, msgInd->msg_data);

	/*process new sms data*/
	mmi_frm_sms_new_data();
	
	OslMfree(data);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_reg_port_rsp
* DESCRIPTION
*   Register port response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_reg_port_rsp (void* inMsg)
{
	MMI_FRM_SMS_REG_PORT_NUM_RSP_STRUCT* msgRsp = (MMI_FRM_SMS_REG_PORT_NUM_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U16 cause = msgRsp->cause;

	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_REG_PORT_NUM_RSP);
	if(msgRsp->result == TRUE)
		result = MMI_FRM_SMS_OK;
	mmi_frm_sms_callback_action((void*)&cause, result);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_reg_port_req
* DESCRIPTION
*   Register port request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_reg_port_req (void* inMsg)
{
	MMI_FRM_SMS_REG_PORT_NUM_REQ_STRUCT* msgReq;
	mmi_sms_reg_port_num_req_struct* data	= (mmi_sms_reg_port_num_req_struct*)inMsg;
	msgReq				=	(MMI_FRM_SMS_REG_PORT_NUM_REQ_STRUCT*)
							OslConstructDataPtr(sizeof (*msgReq));
	msgReq->enable 		= data->enable;
	msgReq->mod_id 		= data->mod_id;
	msgReq->port_num 	= data->port_num;
	SetProtocolEventHandler(mmi_frm_sms_reg_port_rsp, PRT_MSG_ID_MMI_SMS_REG_PORT_NUM_RSP);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_REG_PORT_NUM_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sc_addr_rsp
* DESCRIPTION
*   Get SC address response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_sc_addr_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT *msgRsp	=	(MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP, nSimID);
	if(msgRsp->result == TRUE)
	{
		result = MMI_FRM_SMS_OK;
		if(msgRsp->sc_addr.length==0)
			mmi_frm_sms_entry_sc();
		else
		{
			sendSMS->sendcheck |= MMI_FRM_SMS_SC;
			mmi_frm_sms_send_sms_req();
		}
	}
	else
		mmi_frm_sms_callback_action(NULL, result);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sc_addr_req
* DESCRIPTION
*   Get SC address request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_sc_addr_req (void* inMsg)
{
	MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT* msgReq;
	U8 index = *(U8*)inMsg;
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif

	msgReq				=	(MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*)
							OslConstructDataPtr(sizeof (*msgReq));
	msgReq->profile_no	=	index; 	
	SetProtocolEventHandlerExt(mmi_frm_sms_get_sc_addr_rsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP, reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ, (oslParaType *)msgReq, NULL);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_sc_addr_rsp
* DESCRIPTION
*   Set SC address response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
// wangbei add start 20060614

sim_card_status_enum check_sim_card_status(void)
{
	return SIM_NOT_READY;
}
EMSTATUS EMSSetMsgClass(EMSData *emsData, kal_uint8 msg_class)
{
	return EMS_OK;
}
#if 0
S32 GetFSFileSize(FILE_HANDLE handle)
{    
        return 0;
}
INT pfError(INT err)
{
        return 0;
}

#endif
// wangbei add end
void mmi_frm_sms_set_sc_addr_rsp (void* inMsg)
{
	MMI_FRM_SMS_SET_PROFILE_PARAMS_RSP_STRUCT *msgRsp = (MMI_FRM_SMS_SET_PROFILE_PARAMS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_RSP);	
	if(msgRsp->result == TRUE)
	{
		result = MMI_FRM_SMS_OK;
		DisplayPopupCallBack((PU8)GetString(STR_GLOBAL_SAVED), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE, mmi_frm_sms_send_sms_req);
		DeleteScreenIfPresent(SCR_ID_MSG_SC_NUMBER);
		sendSMS->sendcheck &= MMI_FRM_SMS_SC;
	}
	else
	{
		DisplayPopup ((PU8)GetString(STR_MESSAGES_UNKNOWN_ERROR), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteScreenIfPresent(SCR_ID_MSG_SC_NUMBER);
		//mmi_frm_sms_callback_action(NULL, FALSE);//chenhe comment it for bug 11130 
        }
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_sc_addr_req
* DESCRIPTION
*   Set SC address request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_sc_addr_req (void* inMsg)
{
	MMI_FRM_SMS_SET_PROFILE_PARAMS_REQ_STRUCT* msgReq;
	msg_profile_edit_struct* profile = (msg_profile_edit_struct*)inMsg;
	S8 scAddress[MAX_DIGITS_SMS*ENCODING_LENGTH];
	msgReq	=	(MMI_FRM_SMS_SET_PROFILE_PARAMS_REQ_STRUCT*)
					OslConstructDataPtr(sizeof (*msgReq));

	memset(scAddress, 0, MAX_DIGITS_SMS*ENCODING_LENGTH);
	UnicodeNToAnsii((PS8)scAddress, (PS8)profile->scAddress, MAX_DIGITS_SMS*ENCODING_LENGTH);
	msgReq->profile_no = profile->profileIndex; 
	if(scAddress[0] == '+')
	{
		msgReq->sc_addr.type		=	CSMCC_INTERNATIONAL_ADDR;
		msgReq->sc_addr.length 	=	(strlen(scAddress)>MAX_DIGITS_SMS)?(MAX_DIGITS_SMS-1):(strlen(scAddress)-1);
		memcpy(msgReq->sc_addr.number, scAddress+1, msgReq->sc_addr.length);
	}
	else
	{
		msgReq->sc_addr.type		=	CSMCC_DEFAULT_ADDR_TYPE;
		msgReq->sc_addr.length	=	(strlen(scAddress)>(MAX_DIGITS_SMS-1))?(MAX_DIGITS_SMS-1):(strlen(scAddress));
		memcpy(msgReq->sc_addr.number, scAddress, msgReq->sc_addr.length);
	}
	memset(&msgReq->sc_addr.number[msgReq->sc_addr.length], 0, 1);
	msgReq->para_ind	=	SMSAL_PARA_SCA;
	/*Wangzl Add Start For 6599 Ver: TBM780  on 2007-9-6 10:43 */
	{
		U16 error =0;
		ReadRecord(NVRAM_SMSPROFNE_NVRAM_LID, 1, (void *)&msgReq->profile_name, sizeof(l4_name_struct), &error);
	}
	/*Wangzl Add End  For 6599 Ver: TBM780  on 2007-9-6 10:43 */
	SetProtocolEventHandler(mmi_frm_sms_set_sc_addr_rsp, PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_RSP);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_REQ, (oslParaType *)msgReq, NULL);
}



#ifdef _CHECK_FDL_BY_MMI_
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_fdl_rsp
* DESCRIPTION
*   Check FDL response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_check_fdl_rsp (void* inMsg)
{
	MMI_FRM_SMS_FDN_CHECK_RSP_STRUCT *msgRsp = (MMI_FRM_SMS_FDN_CHECK_RSP_STRUCT *)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_FDN_CHECK_RSP);
	if ((msgRsp->result) == TRUE)
	{
		result = MMI_FRM_SMS_OK;
		sendSMS->sendcheck |= MMI_FRM_SMS_FDL;
		mmi_frm_sms_send_sms_req();
	}
	else
		mmi_frm_sms_callback_action(NULL, result);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_fdl_req
* DESCRIPTION
*   Check FDL request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_check_fdl_req (void* inMsg)
{
	MMI_FRM_SMS_FDN_CHECK_REQ_STRUCT*	msgReq;	
	msgReq	=	(MMI_FRM_SMS_FDN_CHECK_REQ_STRUCT*) OslConstructDataPtr (sizeof(*msgReq));
	SetProtocolEventHandler(mmi_frm_sms_check_fdl_rsp, PRT_MSG_ID_MMI_SMS_FDN_CHECK_RSP);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_FDN_CHECK_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_abort_sms_rsp
* DESCRIPTION
*   Abort SMS response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_abort_sms_rsp (void* inMsg)
{
	MMI_FRM_SMS_SEND_ABORT_FINISH_IND_STRUCT *msgRsp = (MMI_FRM_SMS_SEND_ABORT_FINISH_IND_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SEND_ABORT_FINISH_IND);
	if(msgRsp->result == TRUE)
	 	result = MMI_FRM_SMS_OK;
	if (sendSMS!=NULL)
	{
		if (sendSMS->string!=NULL)
                {
		        OslMfree(sendSMS->string);
		        sendSMS->string = NULL;
		}
		OslMfree(sendSMS);
		sendSMS=NULL;
	}
	mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_ABORT);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_abort_sms_ind
* DESCRIPTION
*   Handle abort indication
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_abort_sms_ind (void* inMsg)
{
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP);			
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SEND_ABORT_START_IND);
	mmi_frm_sms_entry_abort();
	SetProtocolEventHandler(mmi_frm_sms_abort_sms_rsp, PRT_MSG_ID_MMI_SMS_SEND_ABORT_FINISH_IND);	
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_abort_sms_req
* DESCRIPTION
*   Abort SMS request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_abort_sms_req (void* inMsg)
{
	mmi_frm_sms_abort_sms_ind(NULL);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_ABORT_REQ, NULL, NULL);
}


//#define __MMI_MSG_RESEND_AFTER_FAIL__
#ifdef __MMI_MSG_RESEND_AFTER_FAIL__
U8 nReSend = 0;
#define MAX_RESEND_TIME 2
extern void HangupAllCalls(void);

void mmi_msg_entry_resend_screen(U8 nresend)
{
	EntryNewScreen(SCR_ID_MSG_SENDING, NULL,mmi_msg_entry_resend_screen, NULL);
	if(IsScreenPresent(SCR_ID_MSG_SENDING))
	{
		mmi_trace(g_sw_SMS,"MMI_SMS: SCR_ID_MSG_SENDING is in hisotry after entry new");
	}
	DeleteScreenIfPresent(SCR_ID_MSG_SENDING);
	DeleteScreenIfPresent(SCR_ID_MSG_RESEDN_CONFRIM);
	/* reset the status because sending SMS in the idle screen background is allowable */
	g_msg_cntx.msg_status = MSG_STATUS_IDLE;
	//将STR_SENDIING_SMS_BODY改成"第nReSend 次重新发送，请稍等"
	ShowCategory8Screen (STR_SENDING_SMS, 0, 0,0, 0, 0, STR_SENDIING_SMS_BODY, IMG_NEW_SMS_SEND, NULL);
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
	{
		SetKeyHandler(HangupAllCalls, KEY_END,KEY_EVENT_DOWN);
	}
}

void mmi_frm_sms_resend_sms(void)
{
	nReSend++;
	mmi_frm_sms_send_sms_req();
	mmi_msg_entry_resend_screen(nReSend);
}

void mmi_frm_sms_giveup_resend_sms(void)
{
	U8* number;
	nReSend = 0;
	DeleteScreenIfPresent(SCR_ID_MSG_RESEDN_CONFRIM);
	
	number=OslMalloc(MAX_DIGITS_SMS);
	memset((S8*)number, 0, MAX_DIGITS_SMS);
	memcpy((S8*)number, (S8*)sendSMS->number, MAX_DIGITS_SMS);
	if(((sendSMS->sendrequire & MMI_FRM_SMS_SAVE)==0)&&((sendSMS->sendrequire & MMI_FRM_SMS_SAVE_AFTER_FAIL)==0))
		ReleaseEMSEditBuffer();
	if (sendSMS!=NULL)
	{
		if (sendSMS->string!=NULL)
		{
			OslMfree(sendSMS->string);
			sendSMS->string = NULL;
		}
		OslMfree(sendSMS);
		sendSMS=NULL;
	}
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP);
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SEND_ABORT_START_IND);
	/*no more to send or send fail: returns the result*/
	mmi_frm_sms_callback_action((void*)number, sendResult);	
	OslMfree(number);

}


void mmi_frm_sms_resend_confrim(void)
{
		mmi_trace(g_sw_SMS,"MMI_SMS: mmi_frm_sms_resend_confrim");
		EntryNewScreen(SCR_ID_MSG_RESEDN_CONFRIM, NULL,mmi_frm_sms_resend_confrim, NULL);
	//string STR_SMS_MESSAGES_RESEND is "Send Fail, Resend?" "发送失败，重新发送?"
		DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,STR_GLOBAL_NO,IMG_GLOBAL_NO,
						get_string(STR_SMS_MESSAGES_RESEND),IMG_GLOBAL_QUESTION,WARNING_TONE);
		//if goback to idle from here directly, then the action will be deleted
		g_msg_cntx.msg_status = MSG_STATUS_INPUT_NUM;
		
		DeleteScreenIfPresent(SCR_ID_MSG_SENDING);
		ClearAllKeyHandler();
		#if 0
		ClearKeyHandler(KEY_END, KEY_EVENT_UP);
		ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
		ClearKeyHandler(KEY_END, KEY_REPEAT);
		#endif
		SetLeftSoftkeyFunction(mmi_frm_sms_resend_sms, KEY_EVENT_UP);
		SetRightSoftkeyFunction(mmi_frm_sms_giveup_resend_sms, KEY_EVENT_UP);
}
#endif
/*loop handle*/
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_send_sms_rsp
* DESCRIPTION
*   Send SMS response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_send_sms_rsp (void* inMsg)
{
	U8* number;
	MMI_FRM_SMS_SEND_MSG_RSP_STRUCT *msgRsp	=	(MMI_FRM_SMS_SEND_MSG_RSP_STRUCT *)inMsg;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	//PendingSaveSendData.presentSegments--;
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_send_sms_rsp, result = %d", msgRsp->result);
#ifdef __MMI_MULTI_SIM__
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_send_sms_rsp, channel = %d",MTPNP_AD_Get_Channel(MTPNP_AD_SMS_CHANNEL));
#endif
	if((msgRsp->result) == TRUE)
	{
		CHISTIncSentSMS(nSimID);
	}
	else
		sendResult=MMI_FRM_SMS_ERROR;
	
#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
	if (g_frm_sms_cntx.mmi_frm_sms_delivery_report_setting[nSimID])
	{
		mmi_frm_sms_check_delivery_report_awaited((BOOL)msgRsp->result, msgRsp->concat_info, msgRsp->mr);
	}
#endif

#ifdef __MMI_MSG_RESEND_AFTER_FAIL__
	if(!msgRsp->result&&nReSend<=MAX_RESEND_TIME)
	{
		mmi_frm_sms_resend_confrim();
		return;
	}
	nReSend = 0;
#endif

	/*send SMS: pack and send the following segment, inMsg keeps the sent index*/
	//if(PendingSaveSendData.nextSegment<PendingSaveSendData.totalSegments)
		//mmi_frm_sms_send_sms_seg();
	//if(PendingSaveSendData.presentSegments==0)
	if ((msgRsp->concat_info.seg < msgRsp->concat_info.total_seg) && (msgRsp->concat_info.seg <= MMI_SMS_MAX_MSG_SEG) && (msgRsp->concat_info.total_seg <= MMI_SMS_MAX_MSG_SEG))
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_send_sms_rsp,  return, sendResult = %d",sendResult);
		return;
	}	// wangwb add @2007.06.28
	else
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_send_sms_rsp,  goto callback, sendResult = %d",sendResult);
		/*not always; use a flag to check*/
		number=OslMalloc(MAX_DIGITS_SMS);
		memset((S8*)number, 0, MAX_DIGITS_SMS);
		memcpy((S8*)number, (S8*)sendSMS->number, MAX_DIGITS_SMS);
		if(((sendSMS->sendrequire & MMI_FRM_SMS_SAVE)==0)&&((sendSMS->sendrequire & MMI_FRM_SMS_SAVE_AFTER_FAIL)==0))
			ReleaseEMSEditBuffer();
		if (sendSMS!=NULL)
		{
                        if (sendSMS->string!=NULL)
                        {
                                OslMfree(sendSMS->string);
			        sendSMS->string = NULL;
                        }
			OslMfree(sendSMS);
			sendSMS=NULL;
		}
		ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP, nSimID);
		ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SEND_ABORT_START_IND);
		/*no more to send or send fail: returns the result*/
		mmi_frm_sms_callback_action((void*)number, sendResult);	
		OslMfree(number);
	}
}


extern void MoveCursorToPosition(EMSData *data,EMSPosition *dst);
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_pack_sms
* DESCRIPTION
*   Pack SMS before send/save
*
* PARAMETERS
*   none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_frm_sms_pack_sms (void)
{
	EMSData*		pEMS=GetEMSDataForEdit (0, 0);
	EMSPosition	SavedPosition=pEMS->CurrentPosition;
	int i;
	int	result;
		
	memset (PendingSaveSendData.TPUD, 0, sizeof(PendingSaveSendData.TPUD));

	#if defined(MMI_ON_WIN32) && ( defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#endif	

	for(i=0; i<g_frm_sms_cntx.mmi_frm_sms_msg_seg_size; i++)
	{
		PendingSaveSendData.TPUD_udhi[i] = 0;
		PendingSaveSendData.TPUDLen[i] = 0;
		PendingSaveSendData.TPUD_p[i] = &PendingSaveSendData.TPUD[i*MMI_FRM_SMS_MSG_LEN];
	}
	
	if(pEMS->dcs == SMSAL_DEFAULT_DCS || pEMS->dcs == SMSAL_UCS2_DCS)
	{
	#ifdef MMI_ON_HARDWARE_P
		if(check_sim_card_status() == TEST_SIM_PRESENCE)
			EMS_ConvertAsciiEncodingToGSM7BitDefault(pEMS);	
		else
	#endif
			EMS_ConvertAsciiEncodingToGSM7BitDefaultWithExtended(pEMS);
	}
	
	result	=	EMSPack(pEMS, 1, &PendingSaveSendData.totalSegments, 
				PendingSaveSendData.TPUD_p, PendingSaveSendData.TPUDLen) ;
	if(pEMS->dcs == SMSAL_DEFAULT_DCS || pEMS->dcs == SMSAL_UCS2_DCS)
	{
	#ifdef MMI_ON_HARDWARE_P
		if(check_sim_card_status() == TEST_SIM_PRESENCE)
			EMS_ConvertGSM7BitDefaultEncodingToAscii(pEMS);
		else
	#endif
			EMS_ConvertGSM7BitDefaultEncodingToAsciiWithExtended(pEMS);
	}
	MoveCursorToPosition(pEMS, &SavedPosition);
	if (sendSMS != NULL) /* in sending procedure */
	{
		PendingSaveSendData.phNumber = (S8*)sendSMS->number;
	}
	else if (saveSMS != NULL) /* in saving procedure */
	{
		PendingSaveSendData.phNumber = (S8*)saveSMS->number;
	}
	else
	{
		MMI_ASSERT(sendSMS != NULL  ||  saveSMS != NULL);
	}
	PendingSaveSendData.udhi			=	pEMS->udhi ;
	PendingSaveSendData.dcs			=	pEMS->ps_dcs ;
	return result;
}



/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_unpack_sms
* DESCRIPTION
*   Unpack SMS after read/receive
*
* PARAMETERS
*  dcs	IN	dcs
*  fo		IN	fo
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_unpack_sms (U8 dcs, U8 fo)
{
	U8 i=0;
	EMSData*	pEMSdata;
	U8		UDHI;

#ifdef __MMI_MESSAGES_USE_URL__
	GetEMSDataForView(&pEMSdata, 0);
	memset(pEMSdata->textBuffer, 0x00, pEMSdata->textLength);	/* Remove previous message content */
#endif	/* __MMI_MESSAGES_USE_URL__ */
	GetEMSDataForView(&pEMSdata, 1);
	if(PendingSaveSendData.totalSegments==1)
		UDHI = GetUDHIBit (fo);
	else
	{
		UDHI = KAL_TRUE;
		for (i=0; i<PendingSaveSendData.totalSegments; i++)
		{
			if(PendingSaveSendData.TPUD_udhi[i] == KAL_FALSE)
				PendingSaveSendData.TPUD_p[i] = NULL;
		}
	}	
	if((EMSSetDCS(pEMSdata, dcs)==EMS_OK)
		&&(EMSUnPack(pEMSdata, UDHI, PendingSaveSendData.totalSegments, PendingSaveSendData.TPUD_p, PendingSaveSendData.TPUDLen)==EMS_OK))
	{
	}
	else
	{
		GetEMSDataForView(&pEMSdata, 1);
		if(PendingSaveSendData.totalSegments==1)
			UDHI = KAL_FALSE;
		else
			UDHI = KAL_TRUE;		
		for(i=0; i<PendingSaveSendData.totalSegments; i++)
		{
			PendingSaveSendData.TPUD_p[i]=NULL;	
			PendingSaveSendData.TPUDLen[i]=0;
		}	
		EMSSetDCS(pEMSdata, SMSAL_DEFAULT_DCS);
		EMSUnPack(pEMSdata, UDHI, PendingSaveSendData.totalSegments, PendingSaveSendData.TPUD_p, PendingSaveSendData.TPUDLen);
	}
		
	
	if(pEMSdata->dcs == SMSAL_DEFAULT_DCS || pEMSdata->dcs == SMSAL_UCS2_DCS)
	{
	#ifdef MMI_ON_HARDWARE_P
		if(check_sim_card_status() == TEST_SIM_PRESENCE)
			EMS_ConvertGSM7BitDefaultEncodingToAscii (pEMSdata);
		else
	#endif
			EMS_ConvertGSM7BitDefaultEncodingToAsciiWithExtended (pEMSdata);
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_send_sms_seg
* DESCRIPTION
*   Send SMS segment
*
* PARAMETERS
*   none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_send_sms_seg (void)
{
	MMI_FRM_SMS_SEND_MSG_REQ_STRUCT*	sendSms;
	S8* addr = (S8*)sendSMS->number;
	EMSData*		pEMS;   
	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_send_sms_seg" );
	GetEMSDataForEdit (&pEMS, 0);   
	
	sendSms = (MMI_FRM_SMS_SEND_MSG_REQ_STRUCT*) OslConstructDataPtr(sizeof(*sendSms));

 
#if 0
	sendSms->dcs=PendingSaveSendData.dcs;
	sendSms->udhi=PendingSaveSendData.udhi;
	sendSms->length = PendingSaveSendData.TPUDLen[PendingSaveSendData.nextSegment];
	memset(sendSms->msg, 0, SMSAL_MAX_MSG_LEN);
	memcpy(sendSms->msg, PendingSaveSendData.TPUD_p[PendingSaveSendData.nextSegment],
		PendingSaveSendData.TPUDLen[PendingSaveSendData.nextSegment]); 
#endif
 
 
	sendSms->dcs=pEMS->dcs;
	sendSms->udhi=pEMS->udhi;
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_send_sms_seg, pEMS->textLength is:%d" ,pEMS->textLength);
	memset(sendSms->msg, 0, SMSAL_MAX_MSG_LEN);
	if (sendSms->dcs==0)//wangwb @2007.06.12
	{
		UnicodeToAnsii(sendSms->msg, pEMS->textBuffer);
		sendSms->length = pEMS->textLength>>1;
	}	
	else
	{
		memcpy(sendSms->msg,pEMS->textBuffer,pEMS->textLength);
		sendSms->length = pEMS->textLength;
		sendSms->dcs = 0x08;
	}
 
	sendSms->profile	= sendSMS->profile;
	sendSms->para	= 0;
	sendSms->reply_index=MMI_FRM_SMS_INVALID_INDEX;
	if((sendSMS->sendrequire & MMI_FRM_SMS_REPLY)==MMI_FRM_SMS_REPLY)
	{
		U8* replyaddr=mmi_frm_sms_get_sms_address(MMI_FRM_SMS_INBOX, sendSMS->replyindex);
		U16 l4_index[MMI_SMS_MAX_MSG_SEG];
		U8 i=0;
		memset((S8*)sendSMS->number, 0, MAX_DIGITS_SMS);
		memcpy((S8*)sendSMS->number, (S8*)replyaddr, strlen((S8*)replyaddr));
		mmi_frm_sms_get_sms_index(MMI_FRM_SMS_INBOX, sendSMS->replyindex, l4_index);
		while(i<g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)
		{
			if(l4_index[i]!=MMI_FRM_SMS_INVALID_INDEX)
			{
				#ifndef __MMI_MULTI_SIM__
				sendSms->reply_index = l4_index[i];
				#else /* __MMI_MULTI_SIM__ */
				if (MTPNP_AD_Dual_Card_Reply_SMS())
				{
					sendSms->reply_index = l4_index[i];
				}
				else
				{
					sendSms->reply_index = l4_index[i];
				}
				#endif /*__MMI_MULTI_SIM__*/
				break;
			}
			i++;
		}
	}
	
	if(sendSMS->number[0]=='+')
	{
		sendSms->dest.type	=	CSMCC_INTERNATIONAL_ADDR;
		addr++;
	}
	else
		sendSms->dest.type	=	CSMCC_DEFAULT_ADDR_TYPE;
	memset((S8*)sendSms->dest.number, 0, MAX_CC_ADDR_LEN);
	memcpy((S8*)sendSms->dest.number, (S8*)addr, strlen((S8*)addr));
	sendSms->dest.length=strlen((S8*)addr);


	//sendSms->seq_num=PendingSaveSendData.nextSegment;

	sendSms->seq_num = 1;//wangwb @2007.06.16

	#if 0
	/* set  to 0 only in the last segment of the last message*/
	if(((PendingSaveSendData.nextSegment+1)<PendingSaveSendData.totalSegments) || 
		( (g_msg_cntx.msg_send_info.CurrSendSmsNumbers+1) < g_msg_cntx.msg_send_info.TotalSendSmsNumbers) )
		sendSms->mms_mode=1;
	else
		sendSms->mms_mode=0;
	#endif
	#ifndef __MMI_MULTI_SIM__
	if(sendSMS->sendrequire&MMI_FRM_SMS_MORE)
	{
		mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_SEND_BEGIN, NULL, NULL);
	}
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_SEND_MSG_REQ, (oslParaType *)sendSms, NULL);
	#else
	{
		U8 nSimID = MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_SMS_CHANNEL);
		
		mmi_trace(g_sw_SMS,"MMI_SMS: mmi_frm_sms_send_sms_seg>>>send for sim%d", nSimID);
		if(sendSMS->sendrequire&MMI_FRM_SMS_MORE)
		{
			mmi_frm_sms_send_message(MOD_MMI, MOD_L4C+nSimID, 0, PRT_MSG_ID_MMI_SMS_SEND_BEGIN, NULL, NULL);
		}
		mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + nSimID, 0, PRT_MSG_ID_MMI_SMS_SEND_MSG_REQ, (oslParaType *)sendSms, NULL);							
		
	}
	#endif
	//PendingSaveSendData.nextSegment++;
	//PendingSaveSendData.presentSegments++;
}



/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_pre_send_sms
* DESCRIPTION
*   Pre-send SMS: copy useful data and set flags
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_pre_send_sms (void* inMsg)
{
	mmi_frm_sms_send_struct* sendData=(mmi_frm_sms_send_struct*)inMsg;

	#if defined(MMI_ON_WIN32) && (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
	#endif	/* MMI_ON_WIN32 */
	
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_NOTREADY);
		OslMfree(sendData);
		return;
	}
	if(sendSMS!=NULL)
	{
                if (sendSMS->string!=NULL)
                {
                        OslMfree(sendSMS->string);
			sendSMS->string = NULL;
                }
		OslMfree(sendSMS);
		sendSMS=NULL;
	}
	sendSMS=OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memcpy((S8*)sendSMS, (S8*)sendData, sizeof(mmi_frm_sms_send_struct));
	OslMfree(sendData);
	mmi_frm_sms_send_sms_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_entry_number_done
* DESCRIPTION
*   After input number, keep send process
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_entry_number_done (void)
{
	UnicodeToAnsii((S8*)sendSMS->number, (S8*)g_frm_sms_cntx.mmi_frm_sms_number);
	if(strlen((S8*)sendSMS->number)==0)
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
		mmi_frm_sms_send_sms_req();
}


extern EMSTATUS EMSSetMsgClass(EMSData *emsData, kal_uint8 msg_class);
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_send_sms_req
* DESCRIPTION
*   Send SMS request: check sc, number and copy content for send
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_send_sms_req (void)
{
	/*MOD_MMI only, other module skips*/
	EMSData*	pEMS;
	#ifdef __MMI_MULTI_SIM__
	U8 simID;
	#endif

	
	/*Wangzl Del Start For 6441 Ver: TBM780  on 2007-8-23 15:17 */
	 #if 0
	S8 num_temp[50];
	mmi_trace(g_sw_SMS, "HHHHHHHHHHHHHHH      sendSMS.number = %s",sendSMS->number);
	AnsiiToUnicodeString(num_temp, sendSMS->number);
	
	 if( !AllowFixedDial(num_temp) )
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);
		if (sendSMS!=NULL)
		{
                        if (sendSMS->string!=NULL)
                        {
                                OslMfree(sendSMS->string);
			        sendSMS->string = NULL;
                        }
			OslMfree(sendSMS);
			sendSMS=NULL;
		}
		return;
	}//wangwb add @2007.06.20 
	 #endif
/*Wangzl Del End  For 6441 Ver: TBM780  on 2007-8-23 15:17 */
	
	#if defined(MMI_ON_WIN32)&& (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
	#endif

	/*check SC: get SC address, inMsg keeps the flag*/
	if((sendSMS->sendcheck & MMI_FRM_SMS_SC)!=MMI_FRM_SMS_SC)
	{
		#ifndef __MMI_MULTI_SIM__
		S16	errorCode=0;
		S8	nvramActiveProfileIndex=0;
		ReadValue(NVRAM_SMS_ACTIVE_PROFILE_INDEX, &nvramActiveProfileIndex, DS_BYTE, &errorCode);
		mmi_frm_sms_get_sc_addr_req(&nvramActiveProfileIndex);
                sendSMS->profile=nvramActiveProfileIndex;
		return;
		#else /*defined __MMI_MULTI_SIM__*/
		switch (MTPNP_AD_Get_Slave_Type())
		{
			case MTPNP_CM_GSM_MODULE:
			case MTPNP_PC_SIMULATOR:
			{
				S16 errorCode = 0;
				S8 nvramActiveProfileIndex = 0;
				U8 nSimID = 0;
				#ifdef __MMI_MULTI_SIM__
				nSimID = MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_SMS_CHANNEL);
				mmi_frm_sms_set_active_simid(nSimID);
				#endif
				ReadValue(NVRAM_SMS_ACTIVE_PROFILE_INDEX + nSimID, &nvramActiveProfileIndex, DS_BYTE, &errorCode);
				mmi_frm_sms_get_sc_addr_req(&nvramActiveProfileIndex);
				sendSMS->profile = nvramActiveProfileIndex;
				return;
			}
			break;
			case MTPNP_CM_PHS_MODULE:
			case MTPNP_CALIBRATION_TOOLS:
			case MTPNP_UNKNOWN_MODULE:
			default:
				break;
       	 }
		#endif /*__MMI_MULTI_SIM__*/
	}

	/*check DA: show input screen, inMsg keeps the number*/
	if((sendSMS->sendcheck & MMI_FRM_SMS_DA)!=MMI_FRM_SMS_DA)
	{
		memset((S8*)g_frm_sms_cntx.mmi_frm_sms_number, 0, MAX_DIGITS_SMS);
		if((sendSMS->sendrequire & MMI_FRM_SMS_NUM_SCR)==MMI_FRM_SMS_NUM_SCR)
		{
			AnsiiToUnicodeString((S8*)g_frm_sms_cntx.mmi_frm_sms_number, (S8*)sendSMS->number);
			mmi_frm_sms_entry_number();
			sendSMS->sendrequire&=~MMI_FRM_SMS_NUM_SCR;
			return;
		}
		else if((strlen((S8*)sendSMS->number)==0)&&((sendSMS->sendrequire & MMI_FRM_SMS_REPLY)==0))
		{
			if(GetCurrScrnId()!=SCR_ID_MSG_NUMBER)
				mmi_frm_sms_entry_number();
			return;
		}
		sendSMS->sendcheck |= MMI_FRM_SMS_DA;
	}

	/*check FDL: when FDN on*/
	#ifdef _CHECK_FDL_BY_MMI_
	if((sendSMS->sendcheck & MMI_FRM_SMS_FDL)!=MMI_FRM_SMS_FDL)
	{
	#ifdef __MMI_MULTI_SIM__
		U8 nSimID = mmi_frm_sms_get_active_simid();
		if(g_SecuritySetupContext.FdlStatus[nSimID])
	#else
		if(g_SecuritySetupContext.FdlStatus[0])
	#endif
		{
			U8* msg=OslMalloc(strlen((S8*)sendSMS->number));
			memcpy((S8*)msg, (S8*)sendSMS->number, strlen((S8*)sendSMS->number));
			mmi_frm_sms_check_fdl_req(msg);
			return;
		}
		sendSMS->sendcheck |= MMI_FRM_SMS_FDL;
	}
	#endif

	{
		S8*			textbuf=NULL;
		S8*			textbufucs2=NULL;
		U16			textlen=0;
		U16			result=0;
		history_t		temphistory;
		S8 num_temp[50];
		
		if(sendSMS->filename != NULL)
		{
			INT					errorCode=0;
			FILE_HANDLE		emsObjectFilePtr;
			emsObjectFilePtr = MMI_FS_Open ((U8*)sendSMS->filename, PFS_READ);
			/* the file exists without error */
			if(!pfOpenError(emsObjectFilePtr))
			{
				textlen = (U16)GetFSFileSize (emsObjectFilePtr);
				/* the file size is 0 or file too large. 
				The limitation of 5 segments instead of 10 segments is because the content will be converted to unicode later. */
				if(textlen==0 ||textlen>MMI_FRM_SMS_MSG_LEN*5)
				{
					pfclose (emsObjectFilePtr);
					if(textlen==0)
					        mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_DATAEMPTY);
					else if(textlen>MMI_FRM_SMS_MSG_LEN*5)
						mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_DATAEXCEED);	
					if (sendSMS!=NULL)
					{
						if(sendSMS->string!=NULL)
                                                {
							OslMfree(sendSMS->string);
							sendSMS->string = NULL;
						}
						OslMfree(sendSMS);
						sendSMS=NULL;
					}
					return;
				}
				/* the file size is allowed */
				textbuf=OslMalloc(textlen+1);
//luanyy modify start
#ifndef MMI_ON_WIN32
				pfread (emsObjectFilePtr, textbuf, textlen);
#else
				pfread (textbuf, 1, textlen, emsObjectFilePtr, &errorCode);
#endif
//luanyy modify end
				
				//pfread (textbuf, 1, textlen, emsObjectFilePtr, &errorCode);
				pfclose (emsObjectFilePtr);
				/* the file content is corrupted */
				if(pfError (errorCode))
				{
					mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_DATAINVALID);
					OslMfree(textbuf);
					if (sendSMS!=NULL)
					{
                                                if(sendSMS->string!=NULL)
                                                {
							OslMfree(sendSMS->string);
							sendSMS->string = NULL;
						}
						OslMfree(sendSMS);
						sendSMS=NULL;
					}
					return;
				}
				/* the file content is allowed */
			        textbufucs2=OslMalloc((textlen+1)*ENCODING_LENGTH);
			        AnsiiNToUnicodeString(textbufucs2, textbuf, textlen);
			        memset(textbufucs2+textlen*ENCODING_LENGTH, 0, ENCODING_LENGTH);
			        OslMfree(textbuf);
			}
		}
		else if(sendSMS->string!=NULL && sendSMS->stringlength>0)
		{
				textbufucs2=OslMalloc((sendSMS->stringlength)*ENCODING_LENGTH);
				memset((S8*)textbufucs2, 0, ((sendSMS->stringlength)*ENCODING_LENGTH));
				memcpy(textbufucs2, sendSMS->string, ((sendSMS->stringlength)*ENCODING_LENGTH));
		}

		if(textbufucs2)
		{
			EMSTATUS EMSSetPortResult = EMS_OK;
			EMSTATUS EMSSetClassResult = EMS_OK;			
		
			ReleaseEMSEditBuffer();
			pEMS = GetEMSDataForEdit (0, 1);
			#ifdef MMI_ON_HARDWARE_P
			if((sendSMS->sendrequire & MMI_FRM_SMS_PORT)==MMI_FRM_SMS_PORT)
				EMSSetPortResult = EMSSetPortNum(pEMS, sendSMS->srcport, sendSMS->desport);
			if((sendSMS->sendrequire & MMI_FRM_SMS_CLASS)==MMI_FRM_SMS_CLASS)
				EMSSetClassResult = EMSSetMsgClass(pEMS,sendSMS->dcs_class);
			
			if(( EMSSetPortResult != EMS_OK) ||( EMSSetClassResult != EMS_OK) )
			{
				mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_ERROR);
				if (sendSMS!=NULL)
				{
					if(sendSMS->string!=NULL)
					{
						OslMfree(sendSMS->string);
						sendSMS->string = NULL;
					}
					OslMfree(sendSMS);
					sendSMS=NULL;
				}
				return;
			}
			#endif
		
			if (sendSMS->filename != NULL)
			{
				result = AppendEMSString (INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, pEMS, (U8*)textbufucs2, sendSMS->dcs, temphistory.guiBuffer  );
			}
			else if (sendSMS->string!=NULL)
			{
				EMSSetDCS(pEMS, sendSMS->dcs);	/* Jp for tracker issue 419 */
				result = AddString(pEMS, (U8*)textbufucs2, sendSMS->stringlength, NULL);
			}
			OslMfree(textbufucs2);
			
			if ((sendSMS->filename != NULL && result ==0) || (sendSMS->string!=NULL && result != EMS_OK))
			{
				mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_DATAEXCEED);
				if (sendSMS!=NULL)
				{
                                        if(sendSMS->string!=NULL)
                                        {
				                OslMfree(sendSMS->string);
						sendSMS->string = NULL;
					}
					OslMfree(sendSMS);
					sendSMS=NULL;
				}
				return;
			}
		}

#if (0)	
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	/* (0) */
 
#if 0
		result = mmi_frm_sms_pack_sms();
		if(result!=EMS_OK)
		{
			mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_DATAINVALID);
			if (sendSMS!=NULL)
			{
                                if(sendSMS->string!=NULL)
                                {
				        OslMfree(sendSMS->string);
					sendSMS->string = NULL;
				}
				OslMfree(sendSMS);
				sendSMS=NULL;
			}
			return;
		}
#endif
 
		#ifdef __MMI_MULTI_SIM__
		simID = MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_SMS_CHANNEL);
		SetProtocolEventHandlerExt(mmi_frm_sms_send_sms_rsp, PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP, simID);
		#else
		SetProtocolEventHandler(mmi_frm_sms_send_sms_rsp, PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP);
		#endif
		SetProtocolEventHandler(mmi_frm_sms_abort_sms_ind, PRT_MSG_ID_MMI_SMS_SEND_ABORT_START_IND);
		if((sendSMS->sendcheck & MMI_FRM_SMS_SCR)!=MMI_FRM_SMS_SCR)
			mmi_frm_sms_entry_send();

		/*Wangzl Add Start For 6441 Ver: TBM780  on 2007-8-23 15:18 */
		//wangzl:开启固定拨号后，发送时不允许发送给非固定拨号列表中的号码，故模拟发送失败的消息

		memset(num_temp,0,50);
		AnsiiToUnicodeString(num_temp, sendSMS->number);
    		{		
#ifdef __MMI_MULTI_SIM__
			if( !AllowFixedDial(num_temp, simID) )//man for fdn
#else
			if( !AllowFixedDial(num_temp) )//man for fdn
#endif	
			{
				ilm_struct  ilm_ptr;
				mmi_sms_send_msg_rsp_struct *pMMISmsNode = OslMalloc(sizeof( mmi_sms_send_msg_rsp_struct));
				ASSERT(NULL != pMMISmsNode);
				memset(pMMISmsNode, 0, sizeof(mmi_sms_send_msg_rsp_struct));
				
				pMMISmsNode->result = FALSE;
				pMMISmsNode->cause = INVALID_REQ_PARAMETER;

				ilm_ptr.local_para_ptr = (local_para_struct*)pMMISmsNode;
				ilm_ptr.dest_mod_id = MOD_MMI;
				ilm_ptr.src_mod_id = MOD_MMI;
				ilm_ptr.msg_id = PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP;
				ilm_ptr.sap_id = MNSMS_SAP;
				ilm_ptr.peer_buff_ptr = NULL;

				OslMsgSendExtQueue(&ilm_ptr);

				return;
			}
    		}
		/*Wangzl Add End  For 6441 Ver: TBM780  on 2007-8-23 15:18 */
		
		#if 0
		j=(PendingSaveSendData.totalSegments>=MMI_FRM_SMS_SEND_SEG)?MMI_FRM_SMS_SEND_SEG:1;
		PendingSaveSendData.nextSegment=0;
		PendingSaveSendData.presentSegments=0;
		sendResult=MMI_FRM_SMS_OK;
		for(i=0;i<j;i++)
		#endif      //wangwb @2007.06.16
		sendResult=MMI_FRM_SMS_OK; 
		mmi_frm_sms_send_sms_seg();
	}
}


//extern U16 GetRefFromNVRAM (void);

/*loop handle*/
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_save_sms_rsp
* DESCRIPTION
*   Save SMS response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern BOOL mmi_msg_need_save_seprate_msg_after_send_fail(void);
void mmi_frm_sms_save_sms_rsp (void* inMsg)
{
	U16 ref; //luanyy modify 
	MMI_FRM_SMS_SET_MSG_RSP_STRUCT    *msgRsp = (MMI_FRM_SMS_SET_MSG_RSP_STRUCT*)inMsg;
	
	if(msgRsp->result==TRUE)
	{
		mmi_frm_sms_msgbox_struct* entry=OslMalloc(sizeof(mmi_frm_sms_msgbox_struct));
		
		/*U16 ref=(U16)GetRefFromNVRAM();
		if (PendingSaveSendData.totalSegments == 1)
			ref = (ref>0) ? (ref-1) : MMI_FRM_SMS_INVALID_NUM;*/ 
	      //wangwb close  @2007.06.16
			
		PendingSaveSendData.presentSegments = msgRsp->concat_info.seg;
	
		PendingSaveSendData.totalSegments = msgRsp->concat_info.total_seg;
	
		ref = msgRsp->concat_info.ref;
		
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
		mmi_frm_sms_convert_mo_to_entry(saveSMS, entry, msgRsp->index, 
			msgRsp->status, PendingSaveSendData.totalSegments, ref, msgRsp->storage_type);
#else
		mmi_frm_sms_convert_mo_to_entry(saveSMS, entry, msgRsp->index, 
			msgRsp->status, PendingSaveSendData.totalSegments, ref);                    

#endif
		mmi_frm_sms_add_sms_to_msgbox(entry, msgRsp->index, PendingSaveSendData.presentSegments);  
		OslMfree(entry);
		//if(PendingSaveSendData.nextSegment<PendingSaveSendData.totalSegments)      //  wangwb   @2007.06.16
		if ((PendingSaveSendData.presentSegments < PendingSaveSendData.totalSegments) && (PendingSaveSendData.presentSegments <= MMI_SMS_MAX_MSG_SEG) && (PendingSaveSendData.totalSegments <= MMI_SMS_MAX_MSG_SEG))
		{
			return;
			//mmi_frm_sms_save_sms_seg();
		}//wangwb add @2007.06.28
		else
		{
			U8 saveSMSStatus = saveSMS->status;
			ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SET_MSG_RSP);                   //wangwb add @2007.06.16
			
			#ifdef __MMI_MESSAGES_CONTINUE_SEND_AFTER_FAIL__
			if(!mmi_msg_need_save_seprate_msg_after_send_fail())
			#endif
				ReleaseEMSEditBuffer();
			
			if (saveSMS != NULL)
			{
                                if(saveSMS->string!=NULL)
                                {
					OslMfree(saveSMS->string);
					saveSMS->string = NULL;
			        }
				OslMfree(saveSMS);
				saveSMS = NULL;
			}

			/*no more to send or send fail: returns the result*/
			mmi_frm_sms_callback_action(&saveSMSStatus, MMI_FRM_SMS_OK);
		}
	}
	else
	{
		mmi_frm_sms_callback_action(&(saveSMS->status), MMI_FRM_SMS_ERROR);
		if (saveSMS != NULL)
		{
                        if(saveSMS->string!=NULL)
                        {
				OslMfree(saveSMS->string);
				saveSMS->string = NULL;
			}
			OslMfree(saveSMS);
			saveSMS = NULL;
		}
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_save_sms_seg
* DESCRIPTION
*   Save SMS segment
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_save_sms_seg (void)
{
	MMI_FRM_SMS_SET_MSG_REQ_STRUCT*	saveSms;
	S8* addr = (S8*)saveSMS->number;

	EMSData*		pEMS;   
	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_save_sms_seg" );
	GetEMSDataForEdit (&pEMS, 0);   

	saveSms = (MMI_FRM_SMS_SET_MSG_REQ_STRUCT*) OslConstructDataPtr(sizeof(*saveSms));
//	memset((S8*)saveSms, 0, sizeof(MMI_FRM_SMS_SET_MSG_REQ_STRUCT));
	saveSms->status	= saveSMS->status;
	if(addr[0]=='+')
	{
		saveSms->dest.type	=	CSMCC_INTERNATIONAL_ADDR;
		addr++;
	}
	else
		saveSms->dest.type	=	CSMCC_DEFAULT_ADDR_TYPE;	
		
	memset((S8*)saveSms->dest.number, 0, MAX_CC_ADDR_LEN);
	memcpy((S8*)saveSms->dest.number, (S8*)addr, strlen((S8*)addr));
	saveSms->dest.length=strlen((S8*)addr);

 
#if 0
	saveSms->profile	=	saveSMS->profile;	// active profile number..		
	saveSms->para		=	0;
	saveSms->udhi		=	PendingSaveSendData.udhi;		
	saveSms->dcs		=	PendingSaveSendData.dcs;
	saveSms->length		=	PendingSaveSendData.TPUDLen[PendingSaveSendData.nextSegment];
	
	memset(saveSms->msg, 0, SMSAL_MAX_MSG_LEN);
	memcpy(saveSms->msg, PendingSaveSendData.TPUD_p[PendingSaveSendData.nextSegment],
	PendingSaveSendData.TPUDLen[PendingSaveSendData.nextSegment]);
#endif
 

 
	saveSms->profile	=	saveSMS->profile;	// active profile number..		
	saveSms->para		=	0;
	saveSms->dcs=pEMS->dcs;
	saveSms->udhi=pEMS->udhi;
	saveSms->length = pEMS->textLength;
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_save_sms_seg, pEMS->textLength=%d" ,pEMS->textLength);
	memset(saveSms->msg, 0, SMSAL_MAX_MSG_LEN);
	if(saveSms->dcs==0)
	{
		UnicodeToAnsii(saveSms->msg, pEMS->textBuffer);
		/*add start by wuzc for psi-mmi at 2006811*/
		saveSms->length = (pEMS->textLength>>1);
		/*add end by wuzc for psi-mmi at 2006811*/
	}
	else
	{
		memcpy(saveSms->msg,pEMS->textBuffer,saveSms->length);
	}
 
	mmi_trace(g_sw_SMS, "MMI_SMS: PendingSaveSendData.totalSegments = %d ",PendingSaveSendData.totalSegments);
	mmi_trace(g_sw_SMS, "MMI_SMS: saveSms->dcs = %d ",saveSms->dcs);
	mmi_trace(g_sw_SMS, "MMI_SMS: saveSms->length = %d ",saveSms->length);
	mmi_trace(g_sw_SMS, "MMI_SMS: pEMS->textLength = %d", pEMS->textLength);
	#ifndef __MMI_MULTI_SIM__
	SetProtocolEventHandler(mmi_frm_sms_save_sms_rsp, PRT_MSG_ID_MMI_SMS_SET_MSG_RSP);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_SET_MSG_REQ, (oslParaType *)saveSms, NULL);
	#else
	{
		U8 reqSrcMod = MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_SMS_CHANNEL);
		SetProtocolEventHandlerExt(mmi_frm_sms_save_sms_rsp, PRT_MSG_ID_MMI_SMS_SET_MSG_RSP, reqSrcMod);
		mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_SET_MSG_REQ, (oslParaType*) saveSms, NULL);
	}
	#endif
	
	//PendingSaveSendData.nextSegment++;
	//PendingSaveSendData.presentSegments++;   //wangwb @2007.06.16
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_save_sms_req
* DESCRIPTION
*   Save SMS request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_save_sms_req (void* inMsg)
{
	/*Guojian Add Start For 10242 Ver: GreenStone  on 2008-11-1 15:4 */
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_NOTREADY);
		OslMfree(inMsg);
		return;
	}
	/*Guojian Add End  For 10242 Ver: GreenStone  on 2008-11-1 15:4 */

	if (saveSMS!=NULL)
	{
                if (saveSMS->string!=NULL)
                {
			OslMfree(saveSMS->string);
			saveSMS->string = NULL;
		}
		OslMfree(saveSMS);
		saveSMS = NULL;
	}
	saveSMS = OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memcpy((S8*)saveSMS, (S8*)inMsg, sizeof(mmi_frm_sms_send_struct));
	OslMfree(inMsg);

	/* JP added to save a NSm message */
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
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
#endif	/*__MMI_SMART_MESSAGE_MO__  && !__MMI_MESSAGES_EMS__ */

//wangwb   open  @2007.06.16
#if 0
	result=mmi_frm_sms_pack_sms();
	if (result != EMS_OK)
	{
	    mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_DATAINVALID);
	    if (saveSMS!=NULL)
	    {
                if (saveSMS->string!=NULL)
                {
		    OslMfree(saveSMS->string);
		    saveSMS->string = NULL;
		}
		OslMfree(saveSMS);
		saveSMS = NULL;
	    }
	}
	else
#endif
//wangwb open @2007.06.16
	{
		PendingSaveSendData.nextSegment=0;
		PendingSaveSendData.presentSegments=0;  //wangwb 2007.06.16
		mmi_frm_sms_save_sms_seg();
	}
}

extern msg_ems_data_struct 	bufForDisplay ;   //wangwb @2007.06.16
extern  kal_uint8 ems_Display_buffer[1244];         //wangwb @2007.06.16

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_concat_sms_ind
* DESCRIPTION
*   concat sms
*
* PARAMETERS
*  message
* RETURNS
*  none
* GLOBALS AFFECTED
*   bufForDisplay,ems_Display_buffer
*****************************************************************************/
void mmi_frm_sms_concat_sms_ind (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: PendingSaveSendData.TPUDLen[PendingSaveSendData.presentSegments] = %d",PendingSaveSendData.TPUDLen[PendingSaveSendData.presentSegments]);
	
	memcpy(ems_temp+ len_temp, PendingSaveSendData.TPUD_p[PendingSaveSendData.presentSegments], PendingSaveSendData.TPUDLen[PendingSaveSendData.presentSegments]);
	len_temp += PendingSaveSendData.TPUDLen[PendingSaveSendData.presentSegments];

	if (PendingSaveSendData.nextSegment == PendingSaveSendData.totalSegments)
	{
		
		if(bufForDisplay.emsData.dcs == 0)
		{
			AnsiiToUnicodeString(bufForDisplay.emsData.textBuffer,ems_temp);
			bufForDisplay.emsData.textLength = len_temp * 2;
		}
		else
		{
			memcpy(bufForDisplay.emsData.textBuffer,ems_temp,len_temp); 
			bufForDisplay.emsData.textLength = len_temp;
		}
	}
	return;
}//wangwb add @2007.06.16
void mmi_frm_sms_concat_sms_ind1 (void)
{
	UINT16 seg;
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_concat_sms_ind1, PendingSaveSendData.totalSegments = %d", PendingSaveSendData.totalSegments);
	for (seg = 0;seg<PendingSaveSendData.totalSegments;seg++)
	{
		memcpy(ems_temp+ len_temp, PendingSaveSendData.TPUD_p[seg], PendingSaveSendData.TPUDLen[seg]);
		len_temp += PendingSaveSendData.TPUDLen[seg];
	}

	bufForDisplay.emsData.textBuffer = ems_Display_buffer;
	bufForDisplay.emsData.dcs = PendingSaveSendData.dcs;
	bufForDisplay.emsData.ps_dcs = bufForDisplay.emsData.dcs;
	bufForDisplay.emsData.textBufferSize = 1224;

	if(bufForDisplay.emsData.dcs == SMSAL_DEFAULT_DCS||bufForDisplay.emsData.dcs == SMSAL_8BIT_DCS)
	{
		AnsiiToUnicodeString(bufForDisplay.emsData.textBuffer,ems_temp);
		bufForDisplay.emsData.textLength = len_temp * 2;
		bufForDisplay.emsData.dcs = SMSAL_UCS2_DCS;
	}
	else
	{
		memcpy(bufForDisplay.emsData.textBuffer,ems_temp,len_temp); 
		bufForDisplay.emsData.textLength = len_temp;
	}
	
	len_temp = 0;
	//ems_temp[] = {0};
	mmi_trace(g_sw_SMS, "MMI_SMS: bufForDisplay.emsData.dcs = %d",bufForDisplay.emsData.dcs);
	mmi_trace(g_sw_SMS, "MMI_SMS: bufForDisplay.emsData.textLength = %d",bufForDisplay.emsData.textLength);
	return;
}//wangwb add @2007.06.16

/*loop handle*/
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_read_sms_rsp
* DESCRIPTION
*   Read SMS response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/

void mmi_frm_sms_read_sms_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_MSG_RSP_STRUCT    *msgRsp = (MMI_FRM_SMS_GET_MSG_RSP_STRUCT *)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 type;
	//static UINT16 len = 0;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_MSG_RSP);
	if (msgRsp->result == TRUE)
	{
		memcpy(PendingSaveSendData.TPUD_p[PendingSaveSendData.presentSegments], msgRsp->msg_data, msgRsp->no_msg_data);  
		
		PendingSaveSendData.TPUDLen[PendingSaveSendData.presentSegments] = (U8)msgRsp->no_msg_data;
		PendingSaveSendData.TPUD_udhi[PendingSaveSendData.presentSegments] = (U8)GetUDHIBit (msgRsp->fo);
		PendingSaveSendData.mti	=	msgRsp->mti;
		PendingSaveSendData.dcs	=	msgRsp->dcs;
		PendingSaveSendData.st		=	msgRsp->st;

		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_read_sms_rsp,PendingSaveSendData.TPUDLen[%d] = %d, totalSegement = %d, PendingSaveSendData.mti = %d",
			PendingSaveSendData.presentSegments, PendingSaveSendData.TPUDLen[PendingSaveSendData.presentSegments],
			PendingSaveSendData.totalSegments, PendingSaveSendData.mti);

	}	
	else
	{
		PendingSaveSendData.TPUD_p[PendingSaveSendData.presentSegments]=NULL;
		PendingSaveSendData.TPUDLen[PendingSaveSendData.presentSegments]=0;
		PendingSaveSendData.TPUD_udhi[PendingSaveSendData.presentSegments]=KAL_FALSE;
	}

	if (PendingSaveSendData.nextSegment < PendingSaveSendData.totalSegments && mmi_frm_sms_read_sms_seg()==TRUE)
	{
		return;
	}
	else
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: len_temp = %d, msgRsp->result is: %d",len_temp, msgRsp->result);
		mmi_frm_sms_concat_sms_ind1 ();
		  len_temp = 0;
		  result = MMI_FRM_SMS_OK;
                if (msgRsp->result == TRUE)
                {  
                	mmi_trace(g_sw_SMS,"MMI_SMS: msgRsp->status is:%d",msgRsp->status);
			switch(msgRsp->status)
			{
			case SMSAL_REC_UNREAD:
				type = (msgRsp->mti<<4)|MMI_FRM_SMS_UNREAD;
				break;
			case SMSAL_REC_READ:
				type = (msgRsp->mti<<4)|MMI_FRM_SMS_INBOX;
				break;
				
			case SMSAL_STO_SENT:
				type = (msgRsp->mti<<4)|MMI_FRM_SMS_OUTBOX;
				break;
				
			case SMSAL_STO_UNSENT:
				type = (msgRsp->mti<<4)|MMI_FRM_SMS_DRAFTS;
				break;
			}
			 
                }
                else
                {
              	   PendingSaveSendData.mti=SMSAL_MTI_UNSPECIFIED;
              	   type = (SMSAL_MTI_UNSPECIFIED<<4)|MMI_FRM_SMS_NOBOX;
                }
		  mmi_trace(g_sw_SMS,"MMI_SMS: mmi_frm_sms_read_sms_rsp, type is:%d", type);
		 mmi_frm_sms_callback_action(&type, result);	//mmi_msg_get_msg_rsp		
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_read_sms_seg
* DESCRIPTION
*   Read SMS segment
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_frm_sms_read_sms_seg (void)
{
 	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_frm_sms_read_sms_seg");
	while(PendingSaveSendData.nextSegment<g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)
	{
    		mmi_trace(g_sw_SMS,"MMI_SMS: nextSegment is:%d, mmi_frm_sms_msg_seg_size is:%d",PendingSaveSendData.nextSegment, g_frm_sms_cntx.mmi_frm_sms_msg_seg_size);
		if(PendingSaveSendData.l4index[PendingSaveSendData.nextSegment]!=MMI_FRM_SMS_INVALID_INDEX)
		{
			MMI_FRM_SMS_GET_MSG_REQ_STRUCT* msgReq;
			U8 nSimID = 0;
			msgReq	=	(MMI_FRM_SMS_GET_MSG_REQ_STRUCT*)OslConstructDataPtr(sizeof (*msgReq));
			
			msgReq->index			=	PendingSaveSendData.l4index[PendingSaveSendData.nextSegment];
			msgReq->change_status	=	TRUE;
			
			mmi_trace(g_sw_SMS, "MMI_SMS: msgReq->index = %d ",msgReq->index);
			#ifdef __MMI_MULTI_SIM__
			nSimID = mmi_frm_msg_get_simid_by_l4_index(msgReq->index);
			#endif
			SetProtocolEventHandlerExt(mmi_frm_sms_read_sms_rsp, PRT_MSG_ID_MMI_SMS_GET_MSG_RSP, nSimID);
			mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + nSimID, 0, PRT_MSG_ID_MMI_SMS_GET_MSG_REQ, (oslParaType *)msgReq, NULL);
			PendingSaveSendData.presentSegments=PendingSaveSendData.nextSegment;
			PendingSaveSendData.nextSegment++;
			return TRUE;
		}
		PendingSaveSendData.nextSegment++;
	}
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_read_sms_req
* DESCRIPTION
*   Read SMS request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_read_sms_req (void* inMsg)
{
	U16* index=(U16*)inMsg;
	U8 segment=0;
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_read_sms_req, g_frm_sms_cntx.mmi_frm_sms_msg_seg_size = %d", g_frm_sms_cntx.mmi_frm_sms_msg_seg_size);
	for(segment=0; segment<g_frm_sms_cntx.mmi_frm_sms_msg_seg_size; segment++)
	{
		PendingSaveSendData.l4index[segment]=index[segment];
		PendingSaveSendData.TPUD_udhi[segment] = 0;
		PendingSaveSendData.TPUDLen[segment] = 0;
		PendingSaveSendData.TPUD_p[segment] = &PendingSaveSendData.TPUD[segment*MMI_FRM_SMS_MSG_LEN];
	}
	PendingSaveSendData.totalSegments=(U8)index[g_frm_sms_cntx.mmi_frm_sms_msg_seg_size];
	OslMfree(inMsg);

	PendingSaveSendData.dcs=0;
	PendingSaveSendData.nextSegment=0;
	PendingSaveSendData.presentSegments=0;
    	mmi_frm_sms_read_sms_seg();
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_rsp
* DESCRIPTION
*   Get SMS response: update message box
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_sms_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_MSG_RSP_STRUCT    *msgRsp = (MMI_FRM_SMS_GET_MSG_RSP_STRUCT *)inMsg;
	mmi_frm_sms_msgbox_struct * entry=OslMalloc(sizeof(mmi_frm_sms_msgbox_struct));
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_MSG_RSP);
	mmi_frm_sms_convert_read_to_entry(msgRsp, entry, readIndex);
	mmi_frm_sms_add_sms_to_msgbox(entry, readIndex, 1);
	OslMfree(entry);
	mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_OK);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_req
* DESCRIPTION
*   Get SMS request for updating message box
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_sms_req (void* inMsg)
{
	U16* index=(U16*)inMsg;
	MMI_FRM_SMS_GET_MSG_REQ_STRUCT* msgReq;
	msgReq	=	(MMI_FRM_SMS_GET_MSG_REQ_STRUCT*)OslConstructDataPtr(sizeof (*msgReq));
	readIndex = *index;
	msgReq->index			=	*index;
	msgReq->change_status	=	FALSE;
	SetProtocolEventHandler(mmi_frm_sms_get_sms_rsp, PRT_MSG_ID_MMI_SMS_GET_MSG_RSP);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_GET_MSG_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_delete_sms_rsp
* DESCRIPTION
*   Delete SMS response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_delete_sms_rsp (void* inMsg)
{
	MMI_FRM_SMS_DEL_MSG_RSP_STRUCT    *msgRsp = (MMI_FRM_SMS_DEL_MSG_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP, nSimID);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_delete_sms_rsp, msgRsp->result is:%d",result);
	if (msgRsp->result==TRUE)
	{
		result = MMI_FRM_SMS_OK;
		 

		if (msgRsp->index == 0xfff0) //msgRsp->index == 0
		{
			mmi_trace(g_sw_SMS, "MMI_SMS: entry dellet all mmi_frm_sms_remove_sms_from_msgbox");
			mmi_frm_sms_remove_sms_from_msgbox(msgRsp->del_bitmap);
		}
		else if(msgRsp->index!=MMI_FRM_SMS_INVALID_INDEX /*&& msgRsp->index!=0*/) 
		{
			mmi_trace(g_sw_SMS, "MMI_SMS: entry dellet one message"); //add by panxu 2006-8-9
			if (mmi_frm_sms_get_index((msgRsp->index + 1), msgRsp->del_bitmap) != MMI_FRM_SMS_INVALID_INDEX)
			{
				MMI_FRM_SMS_DEL_MSG_REQ_STRUCT*	msgReq;  //luanyy modify

				msgReq = OslMalloc(sizeof(MMI_FRM_SMS_DEL_MSG_REQ_STRUCT));
				
				ASSERT(msgReq != NULL);

				memcpy(msgReq->del_bitmap, msgRsp->del_bitmap, 480);	 

				msgReq->index=mmi_frm_sms_get_index(msgRsp->index + 1, msgReq->del_bitmap);

				msgReq->del_flag = SMSAL_DEL_BITMAP;

				mmi_trace(g_sw_SMS, "MMI_SMS: msgReq->del_bitmap = %d",msgReq->del_bitmap[0]);
				
				SetProtocolEventHandlerExt(mmi_frm_sms_delete_sms_rsp, PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP,nSimID);
				
				mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + nSimID, 0, PRT_MSG_ID_MMI_SMS_DEL_MSG_REQ, (oslParaType *)msgReq, NULL);
				
				return;
			}

			//mmi_frm_sms_remove_sms_entry(msgRsp->index);
			mmi_frm_sms_remove_sms_from_msgbox(msgRsp->del_bitmap);
			
		}

	}
	mmi_frm_sms_callback_action(NULL, result);
}

/*use index array delete at once!!!*/
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_delete_sms_req
* DESCRIPTION
*   Delete SMS request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_delete_sms_req (void* inMsg)
{
	MMI_FRM_SMS_DEL_MSG_REQ_STRUCT*	msgReq;
	U8* index = (U8*)inMsg;
	U8 reqSrcMod = 0;

	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif

	msgReq	=	(MMI_FRM_SMS_DEL_MSG_REQ_STRUCT*) OslConstructDataPtr(sizeof(*msgReq));
	memcpy((S8*)msgReq->del_bitmap, (S8*)index, 480);	 
	
	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_frm_sms_delete_sms_req, msgbox_type is:%d", msgbox_type);
	switch(msgbox_type)
	{
		case MMI_FRM_SMS_INBOX:
				msgReq->del_flag =  SMSAL_DEL_INBOX;
				break;
				
		case MMI_FRM_SMS_OUTBOX:
				msgReq->del_flag = SMSAL_DEL_OUTBOX;
				break;
				
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
				msgReq->del_flag = SMSAL_DEL_OUTBOX;
			break;
		#endif
	
		default:
				msgReq->index=mmi_frm_sms_get_index(0, msgReq->del_bitmap);
				msgReq->del_flag =	SMSAL_DEL_BITMAP;
				break;
		}

		
	SetProtocolEventHandlerExt(mmi_frm_sms_delete_sms_rsp, PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP, reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_DEL_MSG_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}





/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_profile_name_rsp
* DESCRIPTION
*   Get profile name response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_profile_name_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT* msgRsp = (MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP, nSimID);
	if(msgRsp->result == TRUE)
	{
		U16 tempLen = ((MAX_PROFILE_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH);
		U8* tempbuf=OslMalloc(tempLen);
		memset((PS8)tempbuf, 0, (tempLen));
		//tempbuf = CovertStringForPlatform (msgRsp->profile_name.name, msgRsp->profile_name.name_length, msgRsp->profile_name.name_dcs, &outLen);
		
		if(msgRsp->profile_name.name_length >0)
		{
			InputboxConvertGSMToDeviceEncoding(msgRsp->profile_name.name, tempbuf, msgRsp->profile_name.name_length,tempLen, msgRsp->profile_name.name_dcs, 0, 1);
		}
		memcpy((PS8)profileList->profileName[profileList->currReqProfileNo], tempbuf, tempLen);
		if(tempbuf)
		{
			OslMfree (tempbuf);
			tempbuf = NULL;
		}
		profileList->currReqProfileNo++;
		result=MMI_FRM_SMS_OK;
	}
	if((profileList->currReqProfileNo>=profileList->currMaxPsProfiles)||(result==MMI_FRM_SMS_ERROR))
	{
		if(profileList->currMaxPsProfiles>0)
		{
			S16	errorCode=0;		
			ReadValue(NVRAM_SMS_ACTIVE_PROFILE_INDEX, &(profileList->currReqProfileNo), DS_BYTE, &errorCode);	
			if(profileList->currReqProfileNo>profileList->currMaxPsProfiles)
			{
				profileList->currReqProfileNo=0;
				WriteValue(NVRAM_SMS_ACTIVE_PROFILE_INDEX, &(profileList->currReqProfileNo), DS_BYTE, &errorCode);				
			}
		}

		mmi_frm_sms_callback_action((void*)profileList, result);
		OslMfree(profileList);
	}
	else
		mmi_frm_sms_get_profile_name_req(&(profileList->currReqProfileNo));
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_profile_name_req
* DESCRIPTION
*   Get profile name request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_profile_name_req (void* inMsg)
{
	U8 reqSrcMod = 0;
	MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*	msgReq;
	U8 index = *(U8*)inMsg;
	msgReq				=	(MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*)
									OslConstructDataPtr(sizeof (*msgReq));
	msgReq->profile_no	=	index;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif
	SetProtocolEventHandlerExt(mmi_frm_sms_get_profile_name_rsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP,reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ, (oslParaType *)msgReq, NULL);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_profile_num_rsp
* DESCRIPTION
*   Get profile number response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_profile_num_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_PROFILE_NUM_RSP_STRUCT *msgRsp = (MMI_FRM_SMS_GET_PROFILE_NUM_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	profileList=OslMalloc(sizeof(msg_profile_list_struct));
	profileList->currMaxPsProfiles=0;
	profileList->currReqProfileNo=0;
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_GET_PROFILE_NUM_RSP, nSimID);		
	if(msgRsp->result == TRUE)
	{
		profileList->currMaxPsProfiles= msgRsp->profile_no;
		profileList->profileNameLength= msgRsp->name_len;
		result = MMI_FRM_SMS_OK;
	}
	if(profileList->currMaxPsProfiles==0)
	{
		mmi_frm_sms_callback_action((void*)profileList, result);
		OslMfree(profileList);
	}
	else
	{
		#ifdef __MMI_MULTI_SIM__
		mmi_frm_sms_set_active_simid(nSimID);
		#endif
		mmi_frm_sms_get_profile_name_req(&(profileList->currReqProfileNo));
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_profile_num_req
* DESCRIPTION
*   Get profile number request
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_profile_num_req (void* pSimID)
{
	U8 nSimID = *(U8*)pSimID;
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_get_profile_num_req, nSimID is %d", nSimID);
	SetProtocolEventHandlerExt(mmi_frm_sms_get_profile_num_rsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_NUM_RSP, nSimID);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + nSimID, 0, PRT_MSG_ID_MMI_SMS_GET_PROFILE_NUM_REQ, NULL, NULL);
	OslMfree(pSimID);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_profile_detail_rsp
* DESCRIPTION
*   Get profile detail response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_profile_detail_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT* msgRsp = (MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	profileDetail=OslMalloc(sizeof(msg_profile_edit_struct));
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP,nSimID);
	if(msgRsp->result == TRUE)
	{

		U16 tempLen = ((MAX_PROFILE_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH);
		U8* tempbuf=OslMalloc(tempLen);
		U8		type=(msgRsp->sc_addr.type == CSMCC_INTERNATIONAL_ADDR)?1:0;

		memset((PS8)tempbuf, 0, (tempLen));
		
		//tempbuff	=	CovertStringForPlatform (msgRsp->profile_name.name, msgRsp->profile_name.name_length, 
		//				msgRsp->profile_name.name_dcs, &outLen);
		if(msgRsp->profile_name.name_length >0)
		{
			InputboxConvertGSMToDeviceEncoding(msgRsp->profile_name.name, tempbuf, msgRsp->profile_name.name_length,tempLen, msgRsp->profile_name.name_dcs, 0, 1);
		}
		memset((PS8)profileDetail->profileName, 0, (MAX_PROFILE_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH);
		memcpy((PS8)profileDetail->profileName,(PS8) tempbuf, tempLen);
		profileDetail->profileNameDcs=msgRsp->profile_name.name_dcs;
		if(tempbuf)
		{
			OslMfree (tempbuf);
			tempbuf = NULL;
		}
		profileDetail->vpIndex = (U32)msgRsp->vp;
		profileDetail->msgTypeIndex = (U32)msgRsp->pid;
		
		if(type)
			AnsiiToUnicodeString ((PS8)profileDetail->scAddress, (PS8)"+");
		AnsiiNToUnicodeString ((PS8)profileDetail->scAddress+(type*ENCODING_LENGTH), (PS8)msgRsp->sc_addr.number, msgRsp->sc_addr.length);
		memset((void*)&profileDetail->scAddress[(msgRsp->sc_addr.length+type)*ENCODING_LENGTH], 0, ENCODING_LENGTH);
		result = MMI_FRM_SMS_OK;
	}
	mmi_frm_sms_callback_action((void*)profileDetail, result);
	OslMfree(profileDetail);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_profile_detail_req
* DESCRIPTION
*   Get profile detail request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_profile_detail_req (void* inMsg)
{
	U8 reqSrcMod = 0;
	MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*	msgReq;
	U8 index = *(U8*)inMsg;
	msgReq	=	(MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*)
									OslConstructDataPtr(sizeof (*msgReq));
	msgReq->profile_no	=	index;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif
	SetProtocolEventHandlerExt(mmi_frm_sms_get_profile_detail_rsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP,reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_profile_detail_rsp
* DESCRIPTION
*   Set profile detail response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_profile_detail_rsp (void* inMsg)
{
	MMI_FRM_SMS_SET_PROFILE_PARAMS_RSP_STRUCT* msgRsp	=	(MMI_FRM_SMS_SET_PROFILE_PARAMS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_RSP,nSimID);
	if(msgRsp->result == TRUE)
		result = MMI_FRM_SMS_OK;
	mmi_frm_sms_callback_action((void*)profileDetail, result);
	OslMfree(profileDetail);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_profile_detail_req
* DESCRIPTION
*   Set profile detail response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_profile_detail_req (void* inMsg)
{
	U8 reqSrcMod = 0;
	MMI_FRM_SMS_SET_PROFILE_PARAMS_REQ_STRUCT* msgReq;
	msg_profile_edit_struct* profile = (msg_profile_edit_struct*)inMsg;
	S8 			scAddress[MAX_DIGITS+1];
	profileDetail=OslMalloc(sizeof(msg_profile_edit_struct));
	memcpy(profileDetail, profile, sizeof(msg_profile_edit_struct));
	msgReq				=	(MMI_FRM_SMS_SET_PROFILE_PARAMS_REQ_STRUCT*)
							OslConstructDataPtr(sizeof (*msgReq));
	msgReq->para_ind	=	0;
	if(!IsResetCSPGroupService (CSP_TELESERVICES_SERVICE_GROUP, CSP_Validity_Period))
	{
		msgReq->vp	=	(U8)profile->vpIndex;
		msgReq->para_ind	|=	SMSAL_PARA_VP;
	}
	
	if(!IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP, CSP_Protocol_ID))
	{
		msgReq->pid	=	(U8)profile->msgTypeIndex;
		msgReq->para_ind	|=	SMSAL_PARA_PID;
	}
	
	if(profile->profileNameDcs==PHB_UCS2) 
	{
		msgReq->profile_name.name_dcs = MMI_PHB_UCS2;
		msgReq->profile_name.name_length = ((U8)pfnUnicodeStrlen((PS8)profile->profileName))*ENCODING_LENGTH;
		InputboxConvertDeviceEncodingToGSM((PU8)profile->profileName, msgReq->profile_name.name, MAX_PROFILE_NAME_LEN, MMI_PHB_UCS2, 0, 1);
	}
	else
	{
		msgReq->profile_name.name_dcs = MMI_PHB_ASCII;
		msgReq->profile_name.name_length = InputboxConvertDeviceEncodingToGSM((PU8)profile->profileName, msgReq->profile_name.name, MAX_PROFILE_NAME_LEN, MMI_PHB_ASCII, 0, 1) - 1;
	}
	memset(&msgReq->profile_name.name[msgReq->profile_name.name_length], 0, 1);
	msgReq->para_ind	|=	SMSAL_PARA_PROFILE_NAME;
	
	memset(&scAddress, 0, MAX_DIGITS_SMS*ENCODING_LENGTH);
	UnicodeNToAnsii((PS8)scAddress, (PS8)profile->scAddress, MAX_DIGITS_SMS*ENCODING_LENGTH);
	msgReq->profile_no = profile->profileIndex; 
	if(scAddress[0] == '+')
	{
		msgReq->sc_addr.type		=	CSMCC_INTERNATIONAL_ADDR;
		msgReq->sc_addr.length 	=	(strlen(scAddress)>MAX_DIGITS_SMS)?(MAX_DIGITS_SMS-1):(strlen(scAddress)-1);
		memcpy(msgReq->sc_addr.number, scAddress+1, msgReq->sc_addr.length);
	}
	else
	{
		msgReq->sc_addr.type		=	CSMCC_DEFAULT_ADDR_TYPE;
		msgReq->sc_addr.length	=	(strlen(scAddress)>(MAX_DIGITS_SMS-1))?(MAX_DIGITS_SMS-1):(strlen(scAddress));
		memcpy(msgReq->sc_addr.number, scAddress, msgReq->sc_addr.length);
	}
	memset(&msgReq->sc_addr.number[msgReq->sc_addr.length], 0, 1);
	msgReq->para_ind	|=	SMSAL_PARA_SCA;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif
	SetProtocolEventHandlerExt(mmi_frm_sms_set_profile_detail_rsp, PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_RSP, reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_common_settings_rsp
* DESCRIPTION
*   Get common settings response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_common_settings_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_COMMON_PARAMS_RSP_STRUCT* msgRsp = (MMI_FRM_SMS_GET_COMMON_PARAMS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 settings[2]={0, 0};
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_GET_COMMON_PARAMS_RSP,nSimID);

	if(msgRsp->result == TRUE)
	{
		settings[0]=msgRsp->status_report;
		settings[1]=msgRsp->reply_path;
		result = MMI_FRM_SMS_OK;
		
#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
		g_frm_sms_cntx.mmi_frm_sms_delivery_report_setting[nSimID] = msgRsp->status_report;
#endif
	}
	mmi_frm_sms_callback_action((void*)settings, result);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_common_settings_req
* DESCRIPTION
*   Get common settings request
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_common_settings_req (void* dummy)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif
	SetProtocolEventHandlerExt(mmi_frm_sms_get_common_settings_rsp, PRT_MSG_ID_MMI_SMS_GET_COMMON_PARAMS_RSP,reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C+reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_GET_COMMON_PARAMS_REQ, NULL, NULL);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_common_settings_rsp
* DESCRIPTION
*   Set common settings response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_common_settings_rsp (void* inMsg)
{
	MMI_FRM_SMS_SET_COMMON_PARAMS_RSP_STRUCT* msgRsp	=	(MMI_FRM_SMS_SET_COMMON_PARAMS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_SET_COMMON_PARAMS_RSP,nSimID);
	if(msgRsp->result == TRUE)
	{
		result = MMI_FRM_SMS_OK;
#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
		g_frm_sms_cntx.mmi_frm_sms_delivery_report_setting[nSimID] = g_frm_sms_cntx.mmi_frm_sms_delivery_report_temp_setting;
#endif
	}
	mmi_frm_sms_callback_action(NULL, result);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_common_settings_req
* DESCRIPTION
*   Set common settings request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_common_settings_req (void* inMsg)
{
	U8 reqSrcMod = 0;
	MMI_FRM_SMS_SET_COMMON_PARAMS_REQ_STRUCT* msgReq;
	U8* DrRp 			= 	(U8*)inMsg;
	msgReq				=	(MMI_FRM_SMS_SET_COMMON_PARAMS_REQ_STRUCT*)
								OslConstructDataPtr(sizeof (*msgReq));
	
#if 0 //guojian delete for GreenStone  bug 10015. we don't have the function CSP
        if(IsResetCSPGroupService (CSP_TELESERVICES_SERVICE_GROUP,CSP_Del_Conf))
	{
		if(DrRp[0] == 0)
		{
			DrRp[0] = 0;
			DrRp[1] = 0;
		}
		else // DrRp[0] == 1
		{
			DrRp[0] = 0;
			DrRp[1] = 1;
		}
	}
#endif 

	msgReq->status_report		=	DrRp[0];
	msgReq->reply_path		=	DrRp[1];
	msgReq->para_ind	=	SMSAL_COM_PARA_STATUS_REP | SMSAL_COM_PARA_REPLY_PATH;
	
#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
	g_frm_sms_cntx.mmi_frm_sms_delivery_report_temp_setting = msgReq->status_report;
#endif
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif
	SetProtocolEventHandlerExt(mmi_frm_sms_set_common_settings_rsp, PRT_MSG_ID_MMI_SMS_SET_COMMON_PARAMS_RSP, reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_SET_COMMON_PARAMS_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_inform_init_done
* DESCRIPTION
*   inform sms init done on MMI layer
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_inform_init_done(void)
{
	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_msg_inform_init_done");
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_INIT_DONE, NULL, NULL);
}


#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_memory_status_rsp
* DESCRIPTION
*   Get memory status response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_memory_status_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_MSG_NUM_RSP_STRUCT *msgRsp = (MMI_FRM_SMS_GET_MSG_NUM_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	msg_mem_status_struct memory;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_GET_MSG_NUM_RSP,nSimID);
	/*modified start by wuzc for psi-mmi at 200684*/
	if(msgRsp->result==TRUE)
	{
		memory.simTotal		=	msgRsp->total_sim_num;
		memory.meTotal		=	msgRsp->total_me_num;
		memory.simUsed		=	msgRsp->in_sim_no+msgRsp->out_sim_no;
		memory.meUsed		=	msgRsp->in_me_no+msgRsp->out_me_no;
		memory.prefStorage      =	msgRsp->is_sim_card_change;
		result = MMI_FRM_SMS_OK;
	}
	/*modified start by wuzc for psi-mmi at 200684*/	
	mmi_frm_sms_callback_action((void*)&memory, result);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_memory_status_req
* DESCRIPTION
*   Get memory status request
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_memory_status_req (void* dummy)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif
	SetProtocolEventHandlerExt(mmi_frm_sms_get_memory_status_rsp, PRT_MSG_ID_MMI_SMS_GET_MSG_NUM_RSP,reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_GET_MSG_NUM_REQ, NULL, NULL);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_preferred_storage_rsp
* DESCRIPTION
*   Get preferred storage response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_preferred_storage_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_PREFERRED_STORAGE_RSP_STRUCT* msgRsp = (MMI_FRM_SMS_GET_PREFERRED_STORAGE_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 storage = 0;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_GET_PREFERRED_STORAGE_RSP,nSimID);
	if(msgRsp->result == TRUE)
	{
		if(msgRsp->info.mem2 == SMSAL_SM || msgRsp->info.mem2 == SMSAL_SM_PREFER)
			storage	=	SMSAL_SM;
		else
			storage	=	SMSAL_ME;
		result = MMI_FRM_SMS_OK;
	}
	mmi_frm_sms_callback_action((void*)&storage, result);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_preferred_storage_req
* DESCRIPTION
*   Get preferred storage request
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_preferred_storage_req (void* dummy)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif
	
	SetProtocolEventHandlerExt(mmi_frm_sms_get_preferred_storage_rsp, PRT_MSG_ID_MMI_SMS_GET_PREFERRED_STORAGE_RSP,reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_GET_PREFERRED_STORAGE_REQ, NULL, NULL);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_preferred_storage_rsp
* DESCRIPTION
*   Set preferred storage response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_preferred_storage_rsp (void* inMsg)
{
	MMI_FRM_SMS_SET_PREFERRED_STORAGE_RSP_STRUCT* msgRsp = (MMI_FRM_SMS_SET_PREFERRED_STORAGE_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 preferStorage = 0;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_SET_PREFERRED_STORAGE_RSP,nSimID);	
	if((msgRsp->result) == TRUE)
	{
		if(msgRsp->param.mem2 == SMSAL_SM || msgRsp->param.mem2 == SMSAL_SM_PREFER)
			preferStorage	=	SMSAL_SM;
		else
			preferStorage	=	SMSAL_ME;
		result = MMI_FRM_SMS_OK;
	}
	mmi_frm_sms_callback_action((void*)&preferStorage, result);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_preferred_storage_req
* DESCRIPTION
*   Set preferred storage request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_preferred_storage_req (void* inMsg)
{
	MMI_FRM_SMS_SET_PREFERRED_STORAGE_REQ_STRUCT* msgReq;
	U8 reqSrcMod = 0;
	U8 storage = *(U8*)inMsg;

	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif

	msgReq			= 	(MMI_FRM_SMS_SET_PREFERRED_STORAGE_REQ_STRUCT*)
							OslConstructDataPtr(sizeof (*msgReq));
	msgReq->mem2	=	storage;
	msgReq->mem1	=	SMSAL_STORAGE_UNSPECIFIED;
	msgReq->mem3	=	SMSAL_STORAGE_UNSPECIFIED;
	SetProtocolEventHandlerExt(mmi_frm_sms_set_preferred_storage_rsp, PRT_MSG_ID_MMI_SMS_SET_PREFERRED_STORAGE_RSP,reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_SET_PREFERRED_STORAGE_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}
#endif
#ifdef __MMI_GPRS_FEATURES__
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_preferred_bearer_rsp
* DESCRIPTION
*   Get preferred bearer response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_preferred_bearer_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_COMMON_PARAMS_RSP_STRUCT* msgRsp = (MMI_FRM_SMS_GET_COMMON_PARAMS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 bearer = 0;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_GET_COMMON_PARAMS_RSP,nSimID);	
	if(msgRsp->result == TRUE)
	{
		if (msgRsp->bearer_service == SMS_PREFER_GPRS)
			bearer= MMI_SMS_PREFER_GPRS;
		else if (msgRsp->bearer_service == SMS_PREFER_GSM)
			bearer = MMI_SMS_PREFER_GSM;
		else if (msgRsp->bearer_service == SMS_GPRS_ONLY)
			bearer = MMI_SMS_GPRS_ONLY;
		else if (msgRsp->bearer_service == SMS_GSM_ONLY)
			bearer = MMI_SMS_GSM_ONLY;		
		result = MMI_FRM_SMS_OK;
	}
	mmi_frm_sms_callback_action((void*)&bearer, result);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_preferred_bearer_req
* DESCRIPTION
*   Get preferred bearer request
*
* PARAMETERS
*  dummy	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_preferred_bearer_req (void *dummy)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif
	SetProtocolEventHandlerExt(mmi_frm_sms_get_preferred_bearer_rsp, PRT_MSG_ID_MMI_SMS_GET_COMMON_PARAMS_RSP,reqSrcMod);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C+reqSrcMod, 0, PRT_MSG_ID_MMI_SMS_GET_COMMON_PARAMS_REQ, NULL, NULL);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_preferred_bearer_rsp
* DESCRIPTION
*   Set preferred bearer response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_preferred_bearer_rsp (void* inMsg)
{
	MMI_FRM_SMS_SET_COMMON_PARAMS_RSP_STRUCT* msgRsp = (MMI_FRM_SMS_SET_COMMON_PARAMS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SET_COMMON_PARAMS_RSP);		
	if((msgRsp->result) == TRUE)
		result = MMI_FRM_SMS_OK;
	mmi_frm_sms_callback_action(NULL, result);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_preferred_bearer_req
* DESCRIPTION
*   Set preferred bearer request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_preferred_bearer_req (void* inMsg)
{
	MMI_FRM_SMS_SET_COMMON_PARAMS_REQ_STRUCT*	msgReq;
	U8 bearer 	= *(U8*)inMsg;
	msgReq		=	(MMI_FRM_SMS_SET_COMMON_PARAMS_REQ_STRUCT*)
							OslConstructDataPtr(sizeof (*msgReq));
	switch(bearer)
	{
		case MMI_SMS_PREFER_GPRS:
			bearer = SMS_PREFER_GPRS;
			break;
		case MMI_SMS_PREFER_GSM:
			bearer = SMS_PREFER_GSM;
			break;
		case MMI_SMS_GPRS_ONLY:
			bearer = SMS_GPRS_ONLY;
			break;
		case MMI_SMS_GSM_ONLY:
			bearer = SMS_GSM_ONLY;
			break;
	}
	msgReq->bearer_service	=	bearer;
	msgReq->para_ind		=	SMSAL_COM_PARA_BEARER;
	SetProtocolEventHandler(mmi_frm_sms_set_preferred_bearer_rsp, PRT_MSG_ID_MMI_SMS_SET_COMMON_PARAMS_RSP);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_SET_COMMON_PARAMS_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}

#endif

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_mailbox_info_rsp
* DESCRIPTION
*   Get mailbox info response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_mailbox_info_rsp(void* inMsg)
{
	MMI_FRM_SMS_GET_MAILBOX_ADDRESS_RSP_STRUCT* msgRsp=(MMI_FRM_SMS_GET_MAILBOX_ADDRESS_RSP_STRUCT*)inMsg;
	mmi_frm_sms_mailbox_struct mailboxInfo;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 i=0;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_GET_COMMON_PARAMS_RSP, nSimID);	
	if(msgRsp->result == TRUE)
	{
		mailboxInfo.mailboxNumber=msgRsp->mailbox_num;
		mailboxInfo.mailboxNameLength=msgRsp->alpha_length;
		for(i=0; i<mailboxInfo.mailboxNumber; i++)
		{
			U8 type = (msgRsp->mailbox_info[i].number_info.type == CSMCC_INTERNATIONAL_ADDR)?1:0;
			U8 length = msgRsp->mailbox_info[i].number_info.length;
			U16 nameLen = (msgRsp->mailbox_info[i].name.name_length+1)*ENCODING_LENGTH;
				
			mailboxInfo.mailboxNameDcs[i]=msgRsp->mailbox_info[i].name.name_dcs;
			mailboxInfo.pMailBoxName[i] = OslMalloc(nameLen);
			memset(mailboxInfo.pMailBoxName[i],0,nameLen);
			if(msgRsp->mailbox_info[i].name.name_length >0)
			{
				InputboxConvertGSMToDeviceEncoding(msgRsp->mailbox_info[i].name.name, mailboxInfo.pMailBoxName[i], 
					msgRsp->mailbox_info[i].name.name_length, nameLen, msgRsp->mailbox_info[i].name.name_dcs, 0, 1);
			}
			//mailboxInfo.pMailBoxName[i] = CovertStringForPlatform (msgRsp->mailbox_info[i].name.name, 
				//msgRsp->mailbox_info[i].name.name_length, msgRsp->mailbox_info[i].name.name_dcs, &outlen);
			mailboxInfo.pMailBoxNumber[i] = OslMalloc((type+length+1)*ENCODING_LENGTH);
			if (type)
			{
				AnsiiToUnicodeString((PS8)mailboxInfo.pMailBoxNumber[i], (PS8)"+");
			}
			AnsiiNToUnicodeString((PS8)(mailboxInfo.pMailBoxNumber[i]+(type*ENCODING_LENGTH)), (PS8)msgRsp->mailbox_info[i].number_info.number, length);
			memset(mailboxInfo.pMailBoxNumber[i]+((type+length)*ENCODING_LENGTH), 0, ENCODING_LENGTH);
		}
		result = MMI_FRM_SMS_OK;
	}
	mmi_frm_sms_callback_action((void*)&mailboxInfo, result);
	for(i=0; i<mailboxInfo.mailboxNumber; i++)
	{
		if(mailboxInfo.pMailBoxName[i])
			OslMfree(mailboxInfo.pMailBoxName[i]);
		if(mailboxInfo.pMailBoxNumber[i])
			OslMfree(mailboxInfo.pMailBoxNumber[i]);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_mailbox_info_req
* DESCRIPTION
*   Get mailbox info request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_mailbox_info_req (void *dummy)
{
	U8 nSimId = 0;
	#ifdef __MMI_MULTI_SIM__
	nSimId = mmi_frm_sms_get_active_simid();
	#endif
	SetProtocolEventHandlerExt(mmi_frm_sms_get_mailbox_info_rsp, PRT_MSG_ID_MMI_SMS_GET_MAILBOX_ADDRESS_RSP,nSimId);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + nSimId, 0, PRT_MSG_ID_MMI_SMS_GET_MAILBOX_ADDRESS_REQ, NULL, NULL);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_mailbox_info_rsp
* DESCRIPTION
*   Set mailbox info response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_mailbox_info_rsp(void* inMsg)
{
	MMI_FRM_SMS_SET_MAILBOX_ADDRESS_RSP_STRUCT* msgRsp = (MMI_FRM_SMS_SET_MAILBOX_ADDRESS_RSP_STRUCT*)inMsg;
	U16 result = MMI_FRM_SMS_ERROR;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_SMS_SET_MAILBOX_ADDRESS_RSP,nSimID);		
	if((msgRsp->result) == TRUE)
		result = MMI_FRM_SMS_OK;
	mmi_frm_sms_callback_action(mailboxDetail, result);
	OslMfree(mailboxDetail);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_mailbox_info_req
* DESCRIPTION
*   Set mailbox info request
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_mailbox_info_req(void* inMsg)
{
	MMI_FRM_SMS_SET_MAILBOX_ADDRESS_REQ_STRUCT* msgReq;
	msg_mailbox_edit_struct* mailbox = (msg_mailbox_edit_struct*)inMsg;
	S8 			mailboxAddress[MAX_DIGITS+1];
	U8 nSimID = 0;
	mailboxDetail=OslMalloc(sizeof(msg_mailbox_edit_struct));
	memcpy(mailboxDetail, mailbox, sizeof(msg_mailbox_edit_struct));
	msgReq				=	(MMI_FRM_SMS_SET_MAILBOX_ADDRESS_REQ_STRUCT*)
							OslConstructDataPtr(sizeof (*msgReq));
	
	if(mailbox->mailboxNameDcs==PHB_UCS2) 
	{
		msgReq->name.name_dcs = MMI_PHB_UCS2;
		msgReq->name.name_length = ((U8)pfnUnicodeStrlen((PS8)mailbox->mailboxName))*ENCODING_LENGTH;
		InputboxConvertDeviceEncodingToGSM((PU8)mailbox->mailboxName, msgReq->name.name, MAX_PROFILE_NAME_LEN, MMI_PHB_UCS2, 0, 1);
	}
	else
	{
		msgReq->name.name_dcs = MMI_PHB_ASCII;
		msgReq->name.name_length = InputboxConvertDeviceEncodingToGSM((PU8)mailbox->mailboxName, msgReq->name.name, MAX_PROFILE_NAME_LEN, MMI_PHB_ASCII, 0, 1) - 1;
	}
	memset(&msgReq->name.name[msgReq->name.name_length], 0, 1);
	
	memset(mailboxAddress, 0, MAX_DIGITS_SMS*ENCODING_LENGTH);
	UnicodeNToAnsii((PS8)mailboxAddress, (PS8)mailbox->mailboxAddress, MAX_DIGITS_SMS*ENCODING_LENGTH);
	msgReq->mailbox_num = mailbox->mailboxIndex; 
	if(mailboxAddress[0] == '+')
	{
		msgReq->num_addr.type		=	CSMCC_INTERNATIONAL_ADDR;
		msgReq->num_addr.length 	=	(strlen(mailboxAddress)>MAX_DIGITS_SMS)?(MAX_DIGITS_SMS-1):(strlen(mailboxAddress)-1);
		memcpy(msgReq->num_addr.number, mailboxAddress+1, msgReq->num_addr.length);
	}
	else
	{
		msgReq->num_addr.type		=	CSMCC_DEFAULT_ADDR_TYPE;
		msgReq->num_addr.length	=	(strlen(mailboxAddress)>(MAX_DIGITS_SMS-1))?(MAX_DIGITS_SMS-1):(strlen(mailboxAddress));
		memcpy(msgReq->num_addr.number, mailboxAddress, msgReq->num_addr.length);
	}
	memset(&msgReq->num_addr.number[msgReq->num_addr.length], 0, 1);

	#ifdef __MMI_MULTI_SIM__
	nSimID = mmi_frm_sms_get_active_simid();
	#endif
	
	SetProtocolEventHandlerExt(mmi_frm_sms_set_mailbox_info_rsp, PRT_MSG_ID_MMI_SMS_SET_MAILBOX_ADDRESS_RSP, nSimID);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + nSimID, 0, PRT_MSG_ID_MMI_SMS_SET_MAILBOX_ADDRESS_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);

}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_sync_msg_ind
* DESCRIPTION
*   Handle sync indication
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_sync_msg_ind (void* inMsg)
{
	MMI_FRM_SMS_SYNC_MSG_IND_STRUCT* msgInd=(MMI_FRM_SMS_SYNC_MSG_IND_STRUCT*)inMsg;
	U8 i=0;
	
	switch(msgInd->action)
	{
		case L4C_AT_SMS_ADD:
			mmi_frm_sms_get_sms(NULL, MOD_MMI, msgInd->index);
			break;
		case L4C_AT_SMS_READ:
		{
			U16 msgbox_index=0;
			U8 msg_type = 0;
			mmi_frm_sms_get_sms_msgbox_index(msgInd->index, &msgbox_index);
			
			if(msgbox_index != MMI_FRM_SMS_INVALID_INDEX)
			{
				msg_type = (mmi_frm_sms_msg_box[msgbox_index].msgtype  & 0x0f);	

				/* From UNREAD to READ */
				if(msg_type == MMI_FRM_SMS_UNREAD && msgInd->status == SMSAL_REC_READ)
				{
					mmi_frm_sms_set_sms_status(MMI_FRM_SMS_NOBOX, msgbox_index, MMI_FRM_SMS_INBOX);
					mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_sync_msg_ind UNREAD index %d is updated-------*\n", msgInd->index);			
				}
				/* From READ to UNREAD */
				else if(msg_type == MMI_FRM_SMS_INBOX && msgInd->status == SMSAL_REC_UNREAD)
				{
					mmi_frm_sms_set_sms_status(MMI_FRM_SMS_NOBOX, msgbox_index, MMI_FRM_SMS_UNREAD);
					mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_sync_msg_ind UNREAD index %d is updated-------*\n", msgInd->index);
				}

				/* Do nothing if from READ to READ and from UNREAD to UNREAD */
			}
			else
				mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_sync_msg_ind READ no MSG BOX index %d-------*\n", msgInd->index);

			break;
		}
		case L4C_AT_SMS_DELETE:
			mmi_frm_sms_remove_sms_from_msgbox(msgInd->del_bitmap);
			break;
		case L4C_SMS_SIM_REFRESH:
			mmi_frm_sms_ready_ind(NULL);
			break;
	}
	while(i<mmi_frm_sms_interrupt_handler_size)
	{
		if(mmi_frm_sms_interrupt_handler[i].msgid==PRT_MSG_ID_MMI_SMS_SYNC_MSG_IND)
		{
			mmi_frm_sms_interrupt_handler[i].callback(NULL, MOD_MMI, MMI_FRM_SMS_OK);
			break;
		}
		i++;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_sim_refresh_ind
* DESCRIPTION
*  Handle sim refresh indication from SAT
*
* PARAMETERS
*  dummy	IN
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_sim_refresh_ind(U16 dummy)
{
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_sim_refresh_ind SIM Refresh -------*\n");
        g_frm_sms_cntx.mmi_frm_sms_ready[nSimID]=FALSE;
}

#ifdef __MMI_MESSAGES_COPY__
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_copy_sms_callback
* DESCRIPTION
*   generic callback action for SMS copy 
*
* PARAMETERS
*  result, cause
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_copy_sms_callback(U16 result, U8 cause)
{
	copyResult = OslMalloc(sizeof(mmi_frm_sms_copy_result_struct));
	copyResult->msg_number = g_frm_sms_copy_info.copy_msg_number;
	copyResult->cause = cause;
	mmi_frm_sms_callback_action(copyResult, result);
	OslMfree(copyResult);
	copyResult = NULL;
	return;		
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_copy_sms_rollback_rsp
* DESCRIPTION
*   Delete SMS response for rollback
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_copy_sms_rollback_rsp (void* inMsg)
{
	MMI_FRM_SMS_DEL_MSG_RSP_STRUCT    *msgRsp = (MMI_FRM_SMS_DEL_MSG_RSP_STRUCT*)inMsg;
	U8 result = MMI_FRM_SMS_ERROR;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP);
	if (msgRsp->result==TRUE)
	{
		mmi_frm_sms_remove_sms_from_msgbox(msgRsp->del_bitmap);		
	}
	
	mmi_frm_sms_copy_sms_callback((U16)result,result);
}
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_copy_sms_move_rsp
* DESCRIPTION
*   Delete SMS response for move action
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_copy_sms_move_rsp (void* inMsg)
{
	MMI_FRM_SMS_DEL_MSG_RSP_STRUCT    *msgRsp = (MMI_FRM_SMS_DEL_MSG_RSP_STRUCT*)inMsg;
	U8 result = MMI_FRM_SMS_ERROR;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP);
	if (msgRsp->result==TRUE)
	{
		result = MMI_FRM_SMS_OK;
		mmi_frm_sms_remove_sms_from_msgbox(msgRsp->del_bitmap);

		g_frm_sms_copy_info.copy_msg_number++;
		g_frm_sms_copy_info.present_L4_index = MMI_FRM_SMS_INVALID_INDEX;
		g_frm_sms_copy_info.dst_msg_index = MMI_FRM_SMS_INVALID_INDEX;
		g_frm_sms_copy_info.handled_msg_number++;
		/*all msgs are handled*/
		if(g_frm_sms_copy_info.handled_msg_number >= g_frm_sms_copy_info.total_msg_number)
		{
			if(g_frm_sms_copy_info.is_memory_full)
				mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_MEMFULL,MMI_FRM_SMS_MEMFULL);
			else	
				mmi_frm_sms_copy_sms_callback((U16)result,result);
			return;
		}
		/*handle the next msg*/
		else
		{	
			g_frm_sms_copy_info.present_msg_index++;	
			mmi_frm_sms_pre_copy_sms_req(NULL);
			return;
		}			
	}
	else
		mmi_frm_sms_copy_sms_callback((U16)result,result);
}
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_copy_sms_seg_rsp
* DESCRIPTION
*   copy one sms segment rsp
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_copy_sms_seg_rsp (void* inMsg)
{
	MMI_FRM_SMS_COPY_MSG_RSP_STRUCT* msgRsp	=	(MMI_FRM_SMS_COPY_MSG_RSP_STRUCT*)inMsg;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_COPY_MSG_RSP);
	
	if(msgRsp->result == TRUE)
	{
		U16 msgindex=0;
		mmi_frm_sms_msgbox_struct* entry;

		switch(g_frm_sms_copy_info.msg_list_type)
		{
			case MMI_FRM_SMS_NOBOX:
				msgindex=g_frm_sms_copy_info.present_msg_index;
			break;

			case MMI_FRM_SMS_INBOX:
			case MMI_FRM_SMS_UNREAD:
				msgindex=mmi_frm_sms_inbox_list[g_frm_sms_copy_info.present_msg_index];
			break;

			case MMI_FRM_SMS_OUTBOX:
			#ifndef __MMI_MESSAGES_DRAFT_BOX__
			case MMI_FRM_SMS_DRAFTS:
			#endif
				msgindex=mmi_frm_sms_outbox_list[g_frm_sms_copy_info.present_msg_index];
			break;

			#ifdef __MMI_MESSAGES_DRAFT_BOX__
			case MMI_FRM_SMS_DRAFTS:
				msgindex=mmi_frm_sms_drafts_list[g_frm_sms_copy_info.present_msg_index];
			break;
			#endif
		}
	
		/*kal_uint8	result;
		//kal_uint16	cause;
		//kal_uint8	action;
		kal_uint8	dst_storage;
		kal_uint16	src_index;
		kal_uint16	dst_index;*/
		entry=OslMalloc(sizeof(mmi_frm_sms_msgbox_struct));
		memcpy((S8*)entry, (S8*)&mmi_frm_sms_msg_box[msgindex], sizeof(mmi_frm_sms_msgbox_struct));
		//if(entry->storageType != MMI_FRM_SMS_PREFER_STORAGE)
		entry->storageType = msgRsp->dst_storage;
		entry->startindex = msgRsp->dst_index;
		
		g_frm_sms_copy_info.dst_msg_index = mmi_frm_sms_add_sms_to_msgbox(entry, msgRsp->dst_index, mmi_frm_sms_index_list[msgRsp->src_index].segment);
		OslMfree(entry);

		/* remaing unsent segment of this msg */
		if(mmi_frm_sms_index_list[g_frm_sms_copy_info.present_L4_index].nextindex != MMI_FRM_SMS_INVALID_INDEX)
		{
			mmi_frm_sms_copy_sms_seg_req();
			return;
		}
		/* total segments of this msg have sent*/
		else
		{
			/*delete original msg if action == move*/
			if(g_frm_sms_copy_info.action == SMSAL_MOVE_MSG)
			{
				U8* data=OslMalloc(sizeof(U8)*480);
				U16 listSize = mmi_frm_sms_get_sms_list_size (g_frm_sms_copy_info.msg_list_type);
				U16 deleteMsgIndex = g_frm_sms_copy_info.present_msg_index;
				memset((S8*)data, 0, sizeof(U8)*480);

				/*For inbox, check if there are msgs with same timestamp*/
				if(g_frm_sms_copy_info.msg_list_type == MMI_FRM_SMS_INBOX)
				{
					/*For inbox, the new added one supposed to be the next one of the original msg. If not, it means there are msgs with same timestamp */
					if(mmi_frm_sms_inbox_list[(g_frm_sms_copy_info.present_msg_index)+1] != g_frm_sms_copy_info.dst_msg_index)
					{	
						U8 type;
						U16 inboxIndex;
						
						mmi_frm_sms_get_sms_list_index (&type, &inboxIndex, g_frm_sms_copy_info.dst_msg_index);

						/*swap the new added one with the next original next one*/
						mmi_frm_sms_swap_list_index(g_frm_sms_copy_info.msg_list_type, (U16)(g_frm_sms_copy_info.present_msg_index+1) , inboxIndex);
					}
				}
				
				/*For outbox or draft, swap the new added one (last oen of list) with the original one*/
				else
				{
					mmi_frm_sms_swap_list_index(g_frm_sms_copy_info.msg_list_type, g_frm_sms_copy_info.present_msg_index , (U16)(listSize-1));
					deleteMsgIndex = listSize-1;
				}
				if(mmi_frm_sms_get_sms_bitmap((mmi_frm_sms_msgbox_enum)g_frm_sms_copy_info.msg_list_type, deleteMsgIndex, data)==FALSE)
				{
					OslMfree(data);
					mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_ERROR,MMI_FRM_SMS_ERROR);
					return;
				}
				else
				{			
					MMI_FRM_SMS_DEL_MSG_REQ_STRUCT*	msgReq;
					msgReq	=	(MMI_FRM_SMS_DEL_MSG_REQ_STRUCT*) OslConstructDataPtr(sizeof(*msgReq));
					msgReq->del_flag=	SMSAL_DEL_BITMAP;
					memcpy((S8*)msgReq->del_bitmap, (S8*)data, 480);	
					OslMfree(data);
					SetProtocolEventHandler(mmi_frm_sms_copy_sms_move_rsp, PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP);
					mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_DEL_MSG_REQ, (oslParaType *)msgReq, NULL);					
					return;
				}
			}

			/*if action == copy */
			g_frm_sms_copy_info.copy_msg_number++;
			g_frm_sms_copy_info.present_L4_index = MMI_FRM_SMS_INVALID_INDEX;
			g_frm_sms_copy_info.dst_msg_index = MMI_FRM_SMS_INVALID_INDEX;
			g_frm_sms_copy_info.handled_msg_number++;

			/*The new msg is already in list. Get new list size if copy all*/
			if(g_frm_sms_copy_info.total_msg_number > 1)
				g_frm_sms_copy_info.total_msg_number = mmi_frm_sms_get_sms_list_size (g_frm_sms_copy_info.msg_list_type);

			/*all msgs are handled*/
			if(g_frm_sms_copy_info.handled_msg_number >= g_frm_sms_copy_info.total_msg_number)
			{
				if(g_frm_sms_copy_info.is_memory_full)
					mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_MEMFULL,MMI_FRM_SMS_MEMFULL);
				else			
					mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_OK,MMI_FRM_SMS_OK);
				return;
			}
			/*handle the next msg*/
			else
			{
				g_frm_sms_copy_info.present_msg_index++;	
				mmi_frm_sms_pre_copy_sms_req(NULL);
				return;
			}			
		}
	}
	/*if(msgRsp->result == TRUE)*/
	else
	{
		/*kal_uint8	result;
		kal_uint16	cause;
		kal_uint8	action;
		kal_uint8	dst_storage;
		kal_uint16	src_index;
		kal_uint16	dst_index;*/

		/*first segment , no need to rollback*/
		if(g_frm_sms_copy_info.dst_msg_index == MMI_FRM_SMS_INVALID_INDEX)
		{
			mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_ERROR,MMI_FRM_SMS_ERROR);
			return;
		}
		/*rollback the last incomplete copied msg*/
		else
		{
			U8* data=OslMalloc(sizeof(U8)*480);
			memset((S8*)data, 0, sizeof(U8)*480);
			if(mmi_frm_sms_get_sms_bitmap(MMI_FRM_SMS_AWAITS, mmi_frm_sms_msg_box[g_frm_sms_copy_info.dst_msg_index].startindex, data)==FALSE)
			{
				OslMfree(data);
				mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_ERROR,MMI_FRM_SMS_ERROR);
				return;
			}
			else
			{			
				MMI_FRM_SMS_DEL_MSG_REQ_STRUCT*	msgReq;
				msgReq	=	(MMI_FRM_SMS_DEL_MSG_REQ_STRUCT*) OslConstructDataPtr(sizeof(*msgReq));
				msgReq->del_flag=	SMSAL_DEL_BITMAP;
				memcpy((S8*)msgReq->del_bitmap, (S8*)data, 480);	
				SetProtocolEventHandler(mmi_frm_sms_copy_sms_rollback_rsp, PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP);
				mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_DEL_MSG_REQ, (oslParaType *)msgReq, NULL);
				OslMfree(data);
				return;
			}
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_copy_sms_seg_req
* DESCRIPTION
*   copy one sms segment req
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_copy_sms_seg_req ()
{
	MMI_FRM_SMS_COPY_MSG_REQ_STRUCT*	msgReq;
	msgReq	=	(MMI_FRM_SMS_COPY_MSG_REQ_STRUCT*)
									OslConstructDataPtr(sizeof (*msgReq));
	
	/*first segment of message*/
	if(g_frm_sms_copy_info.present_L4_index == MMI_FRM_SMS_INVALID_INDEX)
	{
		U16 msgindex=0;
		switch(g_frm_sms_copy_info.msg_list_type)
		{
			case MMI_FRM_SMS_NOBOX:
				msgindex=g_frm_sms_copy_info.present_msg_index;
			break;

			case MMI_FRM_SMS_INBOX:
			case MMI_FRM_SMS_UNREAD:
				msgindex=mmi_frm_sms_inbox_list[g_frm_sms_copy_info.present_msg_index];
			break;

			case MMI_FRM_SMS_OUTBOX:
			#ifndef __MMI_MESSAGES_DRAFT_BOX__
			case MMI_FRM_SMS_DRAFTS:
			#endif
				msgindex=mmi_frm_sms_outbox_list[g_frm_sms_copy_info.present_msg_index];
			break;

			#ifdef __MMI_MESSAGES_DRAFT_BOX__
			case MMI_FRM_SMS_DRAFTS:
				msgindex=mmi_frm_sms_drafts_list[g_frm_sms_copy_info.present_msg_index];
			break;
			#endif
		}
	
		g_frm_sms_copy_info.present_L4_index = mmi_frm_sms_msg_box[msgindex].startindex;
	}
	
	/* not the first segment*/
	else
	{	/*check if the nextindex == invalid index in mmi_frm_sms_copy_sms_seg_rsp, not here*/
		g_frm_sms_copy_info.present_L4_index = mmi_frm_sms_index_list[g_frm_sms_copy_info.present_L4_index].nextindex;
	}

	msgReq->action = SMSAL_COPY_MSG;
	msgReq->dst_storage = g_frm_sms_copy_info.dst_storage;
	msgReq->src_index = g_frm_sms_copy_info.present_L4_index;
	
	SetProtocolEventHandler(mmi_frm_sms_copy_sms_seg_rsp, PRT_MSG_ID_MMI_SMS_COPY_MSG_RSP);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_COPY_MSG_REQ, (oslParaType *)msgReq, NULL);
	
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_pre_copy_sms_rsp
* DESCRIPTION
*   Get memory status response
*
* PARAMETERS
*  inMsg	IN	input message
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_pre_copy_sms_rsp (void* inMsg)
{
	MMI_FRM_SMS_GET_MSG_NUM_RSP_STRUCT *msgRsp = (MMI_FRM_SMS_GET_MSG_NUM_RSP_STRUCT*)inMsg;
	U8	segmentNumber = mmi_frm_sms_get_sms_received_size(g_frm_sms_copy_info.msg_list_type, g_frm_sms_copy_info.present_msg_index);
	U16 availableMemorySize = 0;

	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_MSG_NUM_RSP);

	if(msgRsp->result==TRUE)
	{
		if(g_frm_sms_copy_info.dst_storage == SMSAL_SM)
		{
			availableMemorySize = msgRsp->total_sim_num - msgRsp->in_sim_no - msgRsp->out_sim_no;
		}
		else  //SMSAL_ME
		{
			availableMemorySize = msgRsp->total_me_num - msgRsp->in_me_no - msgRsp->out_me_no;
		}

		if(availableMemorySize == 0) // no more available Memory
		{
			mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_MEMFULL,MMI_FRM_SMS_MEMFULL);
			return;			
		}
		else if (availableMemorySize < segmentNumber) // not enough for this msg, try the next msg
		{
			g_frm_sms_copy_info.is_memory_full = TRUE;
			g_frm_sms_copy_info.handled_msg_number++;
			if(g_frm_sms_copy_info.handled_msg_number >= g_frm_sms_copy_info.total_msg_number)
			{
				mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_MEMFULL,MMI_FRM_SMS_MEMFULL);
				return;
			}
			else
			{
				g_frm_sms_copy_info.present_msg_index++;	
				mmi_frm_sms_pre_copy_sms_req(NULL);
			}
		}
		else
		{
			mmi_frm_sms_copy_sms_seg_req();
		}
	}
	else
	{
		mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_ERROR,MMI_FRM_SMS_ERROR);
		return;			
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_pre_copy_sms_req
* DESCRIPTION
*   pre-copy SMS : check before copy SMS
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_pre_copy_sms_req(void* dummy)
{
	/* check if user aborts SMS copy action*/
	if(g_frm_sms_copy_info.is_aborted)
	{
		g_frm_sms_copy_info.is_aborted = FALSE;
		mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_ABORT,MMI_FRM_SMS_ABORT);
		return;
	}

	/*check msg storage */
	while(mmi_frm_sms_get_sms_storage(g_frm_sms_copy_info.msg_list_type, g_frm_sms_copy_info.present_msg_index) == g_frm_sms_copy_info.dst_storage)
	{
		g_frm_sms_copy_info.handled_msg_number++;
		
		/*  copy one by one should not enter this loop, or will return here  */
		if(g_frm_sms_copy_info.handled_msg_number >= g_frm_sms_copy_info.total_msg_number)
		{
			if (g_frm_sms_copy_info.is_memory_full)
				mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_MEMFULL,MMI_FRM_SMS_MEMFULL);
			else
				mmi_frm_sms_copy_sms_callback(MMI_FRM_SMS_OK,MMI_FRM_SMS_OK);
			return;
		}
		
		/*only for copy all */
		g_frm_sms_copy_info.present_msg_index++;	
	}

	SetProtocolEventHandler(mmi_frm_sms_pre_copy_sms_rsp, PRT_MSG_ID_MMI_SMS_GET_MSG_NUM_RSP);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C, 0, PRT_MSG_ID_MMI_SMS_GET_MSG_NUM_REQ, NULL, NULL);


}
#endif  // __MMI_MESSAGES_COPY__

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_startup_wap_browser_req
* DESCRIPTION
*  extract url in SMS text and link to WAP browser
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/

#ifdef __MMI_MESSAGES_USE_URL__
void mmi_frm_sms_startup_wap_browser_req(void *URL)
{
	MMI_FRM_SMS_WAP_BROWSER_STARTUP_REQ  *open_url_req;
		
	open_url_req =	(MMI_FRM_SMS_WAP_BROWSER_STARTUP_REQ*) OslConstructDataPtr(sizeof(*open_url_req));
	open_url_req->type = 2;
	pfnUnicodeStrcpy((S8*)open_url_req->url, (S8*)URL);
	mmi_frm_sms_send_message(MOD_MMI,  MOD_WAP, 0, MSG_ID_WAP_BROWSER_STARTUP_REQ, (void *)open_url_req, NULL);
}
#endif	/* __MMI_MESSAGES_USE_URL__ */

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_protocol_event_handler
* DESCRIPTION
*   Set protocol event handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_protocol_event_handler (void)
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(mmi_frm_sms_startup_begin_ind, PRT_MSG_ID_MMI_SMS_STARTUP_BEGIN_IND, i);
		SetProtocolEventHandlerExt(mmi_frm_sms_ready_ind, PRT_MSG_ID_MMI_SMS_READY_IND, i);
		SetProtocolEventHandlerExt(mmi_frm_sms_new_sms_ind, PRT_MSG_ID_MMI_SMS_DELIVER_MSG_IND, i);
		SetProtocolEventHandlerExt(mmi_frm_sms_status_report_ind, PRT_MSG_ID_MMI_SMS_STATUS_REPORT_IND,i);
		SetProtocolEventHandlerExt(mmi_frm_sms_mem_full_ind, PRT_MSG_ID_MMI_SMS_MEM_FULL_IND, i);
		SetProtocolEventHandlerExt(mmi_frm_sms_mem_avai_ind, PRT_MSG_ID_MMI_SMS_MEM_AVAILABLE_IND, i);
		SetProtocolEventHandlerExt(mmi_frm_sms_mem_exceed_ind, PRT_MSG_ID_MMI_SMS_MEM_EXCEED_IND, i);
	}
	
	SetProtocolEventHandler(mmi_frm_sms_new_data_ind, PRT_MSG_ID_MMI_SMS_APP_DATA_IND);
	SetProtocolEventHandler(mmi_frm_sms_msg_waiting_ind, PRT_MSG_ID_MMI_SMS_MSG_WAITING_IND);
	SetProtocolEventHandler(mmi_frm_sms_sync_msg_ind, PRT_MSG_ID_MMI_SMS_SYNC_MSG_IND);
}

#ifdef JATAAYU_SUPPORT
U8 jmms_sms_interrupt_hdlr(void* inMsg)
{
	SetInterruptPopupDisplay(POPUP_ALL_OFF);
	return FALSE;
}
U8 jmms_post_interrupt_hdlr(void* inMsg)
{
	SetInterruptPopupDisplay(POPUP_ALL_ON);
	return FALSE;
}

void jmms_setInterruptHandler(void)
{
	mmi_trace(1," jmms_setInterruptHandler  ");
	SetInterruptEventHandler(jmms_sms_interrupt_hdlr, jmms_post_interrupt_hdlr, PRT_MSG_ID_MMI_SMS_DELIVER_MSG_IND);
	SetInterruptEventHandler(jmms_sms_interrupt_hdlr, jmms_post_interrupt_hdlr, PRT_MSG_ID_MMI_SMS_STATUS_REPORT_IND);
	SetInterruptEventHandler(jmms_sms_interrupt_hdlr, jmms_post_interrupt_hdlr, PRT_MSG_ID_MMI_SMS_MSG_WAITING_IND);	
}

void jmms_clearInterruptHandler(void)
{
	mmi_trace(1," Jmms_clearInterruptHandler  ");

	ClearInterruptEventHandler(PRT_MSG_ID_MMI_SMS_DELIVER_MSG_IND);
	ClearInterruptEventHandler(PRT_MSG_ID_MMI_SMS_STATUS_REPORT_IND);
	ClearInterruptEventHandler(PRT_MSG_ID_MMI_SMS_MSG_WAITING_IND);
}
#endif

#endif


