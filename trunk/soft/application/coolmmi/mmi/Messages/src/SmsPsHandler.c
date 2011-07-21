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
 * SmsPsHandler.C
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
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: SmsPsHandler.c

  	PURPOSE		: It handles protocol SMS messages. 

 

	AUTHOR		: Magesh K, Hiran 

	DATE		: 01-01-2003

**************************************************************/

#ifdef __MOD_SMSAL__
//
//CSD Joy added for FDN check, 1121, shall be removed
//#define FDN_CHECK_TESTING
//CSD end
#define __NEWSIMULATOR 
//
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
#include "gpioinc.h"
#include "callhistorygprots.h"
#include "idleappprot.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "alarmframeworkprot.h"
#include "phonebooktypes.h"
#include "simdetectiongexdcl.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
//
#include "wgui_status_icons.h"
#include "keybrd.h"
#undef  __NEWSIMULATOR

#include "smsapi.h"
#include "smsstruct.h"
#include "mmi_trace.h"
#include "cmerrortypes.h"
#include "smsfunc.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "mtpnp_ad_resdef.h"
#endif
U16 current_chat_sms_index = 0; //add by chenq 20080325
//
//
#ifdef __MMI_OPERATOR_LOGO__
extern void mmi_op_logo_parse_ota_data(void);
#endif /*__MMI_OPERATOR_LOGO__*/

#ifdef __MMI_CLI_ICON__
extern void mmi_cli_icon_parse_ota_data(void);
#endif	/* __MMI_CLI_ICON__ */

#ifdef __MMI_MESSAGES_CHAT__
extern pBOOL gOutBoxSaveFlag;
extern U16 gChatMsgFlag;  
#endif
extern msg_active_profile_struct activeProfile;
extern U16 processRSKStrId;
extern PS8 lookUpNumber (PS8 number);
extern void MoveCursorToPosition(EMSData *data,EMSPosition *dst);
extern U16 GetTotalCallCount(void);
extern void mmi_phb_sms_enter_send_by_group (void);
extern U16 GetCmMarkerScrnID (void);
extern MMI_PHB_LIST_VIEW phbListView;
#ifdef __MMI_MULTI_SIM__
extern BOOL  AllowFixedDial(PS8 strNumber, UINT8 nSimId);
#else
extern BOOL  AllowFixedDial(PS8 strNumber);   //wangwb @2007.06.20
#endif

extern EMSData * GetEMSDataForView              (EMSData ** p, U8 force) ;
extern EMSData * GetEMSDataForEdit              (EMSData ** p, U8 force) ;
extern U8        DeInitMessagesEMSBuffer        (msg_ems_data_struct * pE);
extern void SetCallDeflectFlag(pBOOL flag);
extern void ShowCallManagementErrorMessage(U16 cause);
extern pBOOL ModifyChatRoom(U16 index, U8 flag, void* data, S8* content);

#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
#endif
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
#endif /* __MMI_SMART_MESSAGE_MT__ || __MMI_SMART_MESSAGE_MO__ */

#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
#endif
/*******************************************************


				protocol event handler				


*******************************************************/
extern msg_msgbox_info_struct msgbox_info;
extern PendingSaveSendDataStruct PendingSaveSendData;

extern void mmi_phb_sms_enter_send_entry(void);
extern U8 mmi_phb_convert_to_digit (U8 *dest, U8 *source, U8 max_dest_len);
#ifdef __MMI_MESSAGES_SEND_BY_GROUP__
extern MMI_PHB_ENTRY_BCD_STRUCT		PhoneBook[];				
#endif // ifdef __MMI_MESSAGES_SEND_BY_GROUP__

extern U8	mmi_msg_get_seg_num(void);
extern U16	mmi_msg_get_msgbox_size(void);

void mmi_msg_handle_ready_ind (void* dummy, module_type mod, U16 result)
{
#ifdef __MMI_MULTI_SIM__
	U8 i;
#endif
	U16 totalInbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
	U16 totalOutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
	U16 totalDraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);
	#ifdef __MMI_MULTI_SIM__
	if ( mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_NOBOX) == MMI_FRM_SMS_INVALID_INDEX)
	{
		return;
	}
	#endif
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_handle_ready_ind");
	g_msg_cntx.msg_init=TRUE;
	mmi_msg_set_msg_num_hint(totalInbox, totalOutbox, totalDraftbox);
	RefreshMessagesMenuList();
	#ifndef __MMI_MULTI_SIM__
	mmi_msg_set_msg_icon(FALSE, FALSE);
	#else
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_FALSE, MTPNP_FALSE, i);
	}
	#endif
	/* it is in the processing screen or the processing screen has been interrupted */
	if (GetMessagesCurrScrnID()==SCR_ID_MSG_PROCESSING || IsScreenPresent(SCR_ID_MSG_PROCESSING))
	{
		if (processRSKStrId==STR_LOADING_INBOX_BACK)
		{
			mmi_msg_entry_inbox_list();
			/* if the processing screen is in the history after entry inbox list */
			DeleteScreenIfPresent(SCR_ID_MSG_PROCESSING);
		}
		else if (processRSKStrId==STR_LOADING_OUTBOX_BACK)
		{
			mmi_msg_entry_outbox_list();
			/* if the processing screen is in the history after entry outbox list */
			DeleteScreenIfPresent(SCR_ID_MSG_PROCESSING);
		}
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		else if (processRSKStrId==STR_LOADING_DRAFTBOX_BACK)
		{
			mmi_msg_entry_draftbox_list();
			/* if the processing screen is in the history after entry draftbox list */
			DeleteScreenIfPresent(SCR_ID_MSG_PROCESSING);
		}
		#endif
	}
	
	//wangzl: 090818 add for bug13140
	mmi_msg_inform_init_done();
}

void mmi_msg_sync_msgbox_with_at (void* dummy, module_type mod, U16 result)
{
	U16 todelscrid=0;
        U16 totalInbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
	U16 totalOutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
	U16 totalDraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);

	if (GetMessagesCurrScrnID()==SCR_ID_MSG_INBOX_LIST || IsScreenPresent(SCR_ID_MSG_INBOX_LIST))
		todelscrid=SCR_ID_MSG_INBOX_LIST;
	else if (GetMessagesCurrScrnID()==SCR_ID_MSG_OUTBOX_LIST || IsScreenPresent(SCR_ID_MSG_OUTBOX_LIST))
		todelscrid=SCR_ID_MSG_OUTBOX_LIST;	
	#ifdef __MMI_MESSAGES_DRAFT_BOX__		
	else if (GetMessagesCurrScrnID()==SCR_ID_MSG_DRAFTBOX_LIST || IsScreenPresent(SCR_ID_MSG_DRAFTBOX_LIST))
		todelscrid=SCR_ID_MSG_DRAFTBOX_LIST;
	#endif	
	if(todelscrid!=0)
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
		SetMessagesScrnIdToDelHistoryNodes(todelscrid);
		DeleteMessagesHistoryNodes();
	}
        mmi_msg_set_msg_num_hint(totalInbox, totalOutbox, totalDraftbox);
	RefreshMessagesMenuList();
	mmi_msg_set_msg_icon(FALSE, FALSE);
}

void mmi_msg_handle_msg_waiting_ind (void *inMsg, module_type mod, U16 result)
{
	static U8 reset1=TRUE;
	static U8 reset2=TRUE;
	U8 i=0;
	MMI_FRM_SMS_MSG_WAITING_IND_STRUCT *msgInd = (MMI_FRM_SMS_MSG_WAITING_IND_STRUCT*)inMsg;

	switch(msgInd->line_no)
	{
	case SMSAL_LINE_1:
	if((mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)&&(reset1==TRUE))
	{
		reset1=FALSE;
		for(i=0; i<MAX_WAITING_TYPE_NUM; i++)
		{
			g_msg_cntx.msg_waiting_info[(msgInd->line_no)].number[i]=msgInd->waiting_num[i];
			if(msgInd->waiting_num[i])
				mmi_msg_set_msg_waiting_icon(msgInd->line_no, i, msgInd->waiting_num[i]);
		}
		return;
	}
	reset1=FALSE;	
	break;
	case SMSAL_LINE_2:
	if((mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)&&(reset2==TRUE))
	{
		reset2=FALSE;
		for(i=0; i<MAX_WAITING_TYPE_NUM; i++)
		{
			g_msg_cntx.msg_waiting_info[(msgInd->line_no)].number[i]=msgInd->waiting_num[i];
			if(msgInd->waiting_num[i])
				mmi_msg_set_msg_waiting_icon(msgInd->line_no, i, msgInd->waiting_num[i]);
		}
		return;
	}
			reset2=FALSE;
			break;
	}
	for(i=0; i<MAX_WAITING_TYPE_NUM; i++)
	{
		if((((msgInd->waiting_num[i])==0)&&(g_msg_cntx.msg_waiting_info[(msgInd->line_no)].number[i]>0))
			||(((msgInd->waiting_num[i])>0)&&(g_msg_cntx.msg_waiting_info[(msgInd->line_no)].number[i]==0)))
			mmi_msg_set_msg_waiting_icon(msgInd->line_no, i, msgInd->waiting_num[i]);
		if((msgInd->waiting_num[i])>g_msg_cntx.msg_waiting_info[(msgInd->line_no)].number[i])
			mmi_msg_msg_waiting_ind(i);		
		g_msg_cntx.msg_waiting_info[(msgInd->line_no)].number[i]=msgInd->waiting_num[i];
	}
}
void mmi_msg_handle_deliver_report_ind (void* inMsg, module_type mod, U16 result)
{
	U8 i=0;
	MMI_FRM_SMS_STATUS_REPORT_IND_STRUCT* msgInd = (MMI_FRM_SMS_STATUS_REPORT_IND_STRUCT*)inMsg;
	for(i=0; i<MAX_STATUS_REPORT_NUM; i++)
	{
		if(g_msg_cntx.msg_deliver_report[i]==NULL)
			break;
	}
	if(i==MAX_STATUS_REPORT_NUM)
	{
		U8 j=0;
		OslMfree(g_msg_cntx.msg_deliver_report[0]);
		g_msg_cntx.msg_deliver_ind--;
		for(j=0; j<(MAX_STATUS_REPORT_NUM-1); j++)
			g_msg_cntx.msg_deliver_report[j]=g_msg_cntx.msg_deliver_report[j+1];
		g_msg_cntx.msg_deliver_report[j]=NULL;
		i=j;
	}
	g_msg_cntx.msg_deliver_report[i]=OslMalloc(sizeof(msg_deliver_report_struct));
	g_msg_cntx.msg_deliver_report[i]->delivered=msgInd->st;
	if(msgInd->ra_number.type==CSMCC_INTERNATIONAL_ADDR)
		g_msg_cntx.msg_deliver_report[i]->type=1;
	else
		g_msg_cntx.msg_deliver_report[i]->type=0;
	memset((S8*)g_msg_cntx.msg_deliver_report[i]->number, 0, MSG_NUM_LEN);
	memcpy((S8*)g_msg_cntx.msg_deliver_report[i]->number, msgInd->ra_number.number, msgInd->ra_number.length);
	memcpy((s8*)g_msg_cntx.msg_deliver_report[i]->timestamp, msgInd->dt, 6);
	mmi_msg_deliver_report_ind();

}

void mmi_msg_handle_mem_full_ind(void* inMsg, module_type mod, U16 result)
{
	MMI_FRM_SMS_MEM_FULL_IND_STRUCT* msgInd = (MMI_FRM_SMS_MEM_FULL_IND_STRUCT*) inMsg;
	if (msgInd->type == SMSAL_MEM_FULL_NORMAL)
        {
		#ifndef __MMI_MULTI_SIM__
		mmi_msg_set_msg_icon(TRUE, FALSE);
		g_msg_cntx.msg_full_ind[0]=TRUE;
		#else
		U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
		MTPNP_PFAL_Refresh_StatusBar_SMS_Display(TRUE, FALSE, nSimID);
		g_msg_cntx.msg_full_ind[nSimID]=TRUE;
		#endif
	        
        }
}
U8 g_msg_mem_exceed_ind_simID = 0;
void mmi_msg_handle_mem_exceed_ind(void* inMsg, module_type mod, U16 result)
{
	MMI_FRM_SMS_MEM_EXCEED_IND_STRUCT* msgInd = (MMI_FRM_SMS_MEM_EXCEED_IND_STRUCT*) inMsg;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_handle_mem_exceed_ind(%d)", nSimID);
	g_msg_mem_exceed_ind_simID = nSimID;
	
	if (msgInd->type == SMSAL_MEM_EXCEED_NORMAL)
	{
		g_msg_cntx.msg_exceed_ind[nSimID] = TRUE;
		TurnOnBacklight(1);
		if(IsMMIInIdleState())
		{
			ClearKeyEvents();
			if(IsKeyPadLockState())
				EntryIdleScreen();
			else
				mmi_msg_entry_mem_exceed_ind();
		}
	}
}

void mmi_msg_handle_mem_avail_ind(void* inMsg, module_type mod, U16 result)
{
	MMI_FRM_SMS_MEM_AVAILABLE_IND_STRUCT* msgInd = (MMI_FRM_SMS_MEM_AVAILABLE_IND_STRUCT*) inMsg;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	if (msgInd->type == SMSAL_MEM_AVAIL_NORMAL)
	{
		g_msg_cntx.msg_full_ind[nSimID]=FALSE;
		g_msg_cntx.msg_exceed_ind[nSimID]=FALSE;
		#ifndef __MMI_MULTI_SIM__
		mmi_msg_set_msg_icon(FALSE, TRUE);
		#else
		MTPNP_PFAL_Refresh_StatusBar_SMS_Display(FALSE, TRUE, nSimID);
		#endif
	}
}

U8 mmi_msg_handle_class0_msg_ind(void* inMsg, void* content)
{
	mmi_frm_sms_deliver_msg_struct* msgInd = (mmi_frm_sms_deliver_msg_struct*)inMsg;
	mmi_frm_sms_concat_struct* concatinfo=(mmi_frm_sms_concat_struct*)&(msgInd->concat_info);
	U8 type=0;
	S16 len = 0;
	S8 *unicodecontent = NULL;

	if(g_msg_cntx.msg_class0_p!=NULL)
	{
		if(g_msg_cntx.msg_class0_p->data!=NULL)
		{
			OslMfree(g_msg_cntx.msg_class0_p->data);
			g_msg_cntx.msg_class0_p->data=NULL;
		}
		OslMfree(g_msg_cntx.msg_class0_p);
		g_msg_cntx.msg_class0_p=NULL;
	}
	g_msg_cntx.msg_class0_p=OslMalloc(sizeof(msg_class0_msg_struct));
	g_msg_cntx.msg_class0_p->length=msgInd->no_msg_data;
	g_msg_cntx.msg_class0_p->fo=msgInd->fo;
	g_msg_cntx.msg_class0_p->dcs=msgInd->dcs;
	g_msg_cntx.msg_class0_p->totalseg=concatinfo->total_seg;
	if(msgInd->addr_number.type==CSMCC_INTERNATIONAL_ADDR)
	{
		AnsiiToUnicodeString((S8*)g_msg_cntx.msg_class0_p->number, "+");
		type=1;
	}
	AnsiiNToUnicodeString((S8*)g_msg_cntx.msg_class0_p->number+type*ENCODING_LENGTH, (S8*)msgInd->addr_number.number, MSG_NUM_LEN);
	memcpy(g_msg_cntx.msg_class0_p->timestamp, msgInd->scts, 6);
	/* content might be unicode or ascii, convert it to unicode and store it to unicodecontent */
	if (g_msg_cntx.msg_class0_p->dcs == SMSAL_UCS2_DCS)
	{
		// len = (S16)pfnUnicodeStrlen(content);
		len = g_msg_cntx.msg_class0_p->length;
		unicodecontent = OslMalloc(len+2);
		memcpy(unicodecontent, content, len);
		memset(unicodecontent+len, 0, 2);
	}
	else
	{
		// len = strlen(content);
		len = g_msg_cntx.msg_class0_p->length/2;
		unicodecontent = OslMalloc((len+1)*2);
		AnsiiNToUnicodeString(unicodecontent, content, len);
		memset(unicodecontent+len*2, 0, 2);
	}
	/* indicate a class0 message with missing segment with (.....) in the front of message content */
	if(!mmi_frm_sms_check_awaited_complete_msg(msgInd->msg_data[0]))
	{
		S8 ascii_miss_str[] = EMS_MISS_SEG_DELIMIT_STR;
		U16 ascii_miss_str_len = strlen((S8*)ascii_miss_str);
		S8* unicode_miss_str = NULL;
		U16 unicode_miss_str_len = 0; /* the byte number of unicode_miss_str */
		
		unicode_miss_str = OslMalloc((ascii_miss_str_len+1)*2);
		AnsiiNToUnicodeString(unicode_miss_str, ascii_miss_str, ascii_miss_str_len);
		memset(unicode_miss_str+ascii_miss_str_len*2, 0, 2);
		unicode_miss_str_len = (U16)pfnUnicodeStrlen(unicode_miss_str) * 2;

		/* g_msg_cntx.msg_class0_p->length contains (.....) and the length of unicodecontent */
		g_msg_cntx.msg_class0_p->length = g_msg_cntx.msg_class0_p->length + unicode_miss_str_len;
		g_msg_cntx.msg_class0_p->data = OslMalloc(g_msg_cntx.msg_class0_p->length);
		memset((S8*)g_msg_cntx.msg_class0_p->data, 0, g_msg_cntx.msg_class0_p->length);
		memcpy((S8*)g_msg_cntx.msg_class0_p->data, (S8*)unicode_miss_str, unicode_miss_str_len);
		memcpy((S8*)(g_msg_cntx.msg_class0_p->data + unicode_miss_str_len), (S8*)unicodecontent, (g_msg_cntx.msg_class0_p->length - unicode_miss_str_len));

		/* free unicode_miss_str buffer */
		OslMfree(unicode_miss_str);
		unicode_miss_str = NULL;
	}
	else
	{
		g_msg_cntx.msg_class0_p->data=OslMalloc(g_msg_cntx.msg_class0_p->length);
		memset((S8*)g_msg_cntx.msg_class0_p->data, 0, g_msg_cntx.msg_class0_p->length);
		memcpy((S8*)g_msg_cntx.msg_class0_p->data, (S8*)unicodecontent, g_msg_cntx.msg_class0_p->length);
	}
	mmi_msg_class0_msg_ind();
	/* free unicodecontent buffer */
	if (unicodecontent!=NULL)
	{
		OslMfree(unicodecontent);
		unicodecontent = NULL;
	}
	return FALSE;
}

void mmi_msg_handle_new_msg_ind(void* data, module_type mod, U16 result)
{

	U16 index=*(U16*)data;
	//copy the message to show, "before" read sms screen it shall be copied again
	mmi_msg_new_msg_ind(index);
}
//add by chenq 20080325 B
extern void mmi_frm_sms_read_sms (PsFuncPtrU16 callback, module_type mod_src, U8 type, U16 index);
extern void mmi_msg_get_msg_rsp(void* data, module_type mod, U16 result);
void mmi_msg_get_msg_newchat (void)
{
	U8 type;
	U16 list_index;
	mmi_trace(g_sw_SMS,"MMI_SMS: (Chat) mmi_msg_get_msg_newchat, current_chat_sms_index is:%d",current_chat_sms_index);

	g_msg_cntx.msg_ind_in_idle=FALSE;
	g_msg_cntx.toDisplayMessageList=TO_DISPLAY_MESSAGE_LIST_INBOX;
	g_msg_cntx.msg_ind_index = current_chat_sms_index;
	mmi_frm_sms_read_sms(mmi_msg_get_msg_rsp, MOD_MMI, MMI_FRM_SMS_APP_NOBOX, g_msg_cntx.msg_ind_index);
	mmi_frm_sms_get_list_index(&type, &list_index, g_msg_cntx.msg_ind_index);
}
//add by chenq 20080325 E
void mmi_msg_get_msg_new (void)
{
	U8 type;
	U16 list_index;
	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_msg_get_msg_new g_msg_cntx.msg_ind_index=%d", g_msg_cntx.msg_ind_index);
  //stop msg tone
  //mdi_audio_stop_all(); //used to cs_xxx (long ring)20071203 
	/*Go back to Idle screen if the new msg is deleted by AT */
	if(!mmi_msg_need_new_msg_ind())
	{
		mmi_msg_go_back_from_new_msg_ind();
		return;
	}
    /*Guojian Add Start For 13423 Ver: GreenStone  on 2009-7-27 17:1 */
   	else if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX)==MMI_FRM_SMS_INVALID_INDEX)
	{
		DisplayPopup((PU8) GetString(STR_SMS_MSG_NOT_READY_YET),
					 IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8) ERROR_TONE);
		return;
	}
    /*Guojian Add End  For 13423Ver: GreenStone  on 2009-7-27 17:1 */

	mmi_msg_exit_inbox_list_dummy();
	g_msg_cntx.msg_ind_in_idle=FALSE;
	g_msg_cntx.toDisplayMessageList=TO_DISPLAY_MESSAGE_LIST_INBOX;
	mmi_msg_get_msg_req(MMI_FRM_SMS_APP_NOBOX, g_msg_cntx.msg_ind_index);
	mmi_frm_sms_get_list_index(&type, &list_index, g_msg_cntx.msg_ind_index);
	/* map message box index to message list index to get the correct inbox header */
	if(type==MMI_FRM_SMS_APP_INBOX)
	{
		g_msg_cntx.currBoxIndex=list_index;
		if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)!=MMI_FRM_SMS_INVALID_INDEX)
		{
			msgbox_info.totalinbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
		        msgbox_info.totaloutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
		        msgbox_info.totaldraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);	
	                mmi_msg_set_msg_num_hint (msgbox_info.totalinbox, msgbox_info.totaloutbox, msgbox_info.totaldraftbox);
		        RefreshMessagesMenuList();
		}
	}
}
void mmi_msg_send_to_many_last(void)
{
	U8 nSimID = 0;
	#ifdef __MMI_MULTI_SIM__
	nSimID  = MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_SMS_CHANNEL);
	#endif
	
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C + nSimID, 0, PRT_MSG_ID_MMI_SMS_SEND_END, NULL, NULL);
}
#ifdef __MMI_MESSAGES_CONTINUE_SEND_AFTER_FAIL__
BOOL mmi_msg_need_save_seprate_msg_after_send_fail(void)
{
	if(g_msg_cntx.msg_send_info.FailedSaveNumber < g_msg_cntx.msg_send_info.FailedNumber)
	{//save the failed to outbox
		U8 CurrentIndex = g_msg_cntx.msg_send_info.FailedSaveNumber;
		#ifdef __MMI_MESSAGES_SEND_TO_MANY__
		if((g_msg_cntx.sendMessageCase == SEND_CASE_SEND_TO_MANY)
			&&(pfnUnicodeStrlen((S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.FailedIndex[CurrentIndex]])))
		{
			return TRUE;
		}
		#endif
		#ifdef __MMI_MESSAGES_SEND_BY_GROUP__
		else if((g_msg_cntx.sendMessageCase == SEND_CASE_SEND_BY_GROUP)
			&&(g_msg_cntx.msg_send_info.PhbIndex!=NULL))
		{
			return TRUE;
		}
		#endif
	}
	return FALSE;
}
BOOL mmi_msg_save_seprate_msg_after_send_fail(void)
{
	if(g_msg_cntx.msg_send_info.FailedSaveNumber < g_msg_cntx.msg_send_info.FailedNumber)
	{//save the failed to outbox
		U8 CurrentIndex = g_msg_cntx.msg_send_info.FailedSaveNumber;
		g_msg_cntx.msg_send_info.FailedSaveNumber++;
		#ifdef __MMI_MESSAGES_SEND_TO_MANY__
		if((g_msg_cntx.sendMessageCase == SEND_CASE_SEND_TO_MANY)
			&&(pfnUnicodeStrlen((S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.FailedIndex[CurrentIndex]])))
		{
			U8 length=pfnUnicodeStrlen(g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.FailedIndex[CurrentIndex]]);
			memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
			UnicodeNToAnsii((S8*)g_msg_cntx.smsPhoneNumber, (S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.FailedIndex[CurrentIndex]], length*ENCODING_LENGTH);
			mmi_msg_save_msg_after_send_req(SMSAL_STO_UNSENT,(U8*)g_msg_cntx.smsPhoneNumber);
			return TRUE;
		}
		#endif
		#ifdef __MMI_MESSAGES_SEND_BY_GROUP__
		else if((g_msg_cntx.sendMessageCase == SEND_CASE_SEND_BY_GROUP)
			&&(g_msg_cntx.msg_send_info.PhbIndex!=NULL))
		{
			U8 PhbNuminBCD[(MAX_DIGITS+1)/2];
			U8 PhbNum[MAX_DIGITS+1];
			S8 PhbNumUCS2[(MAX_DIGITS+2)*ENCODING_LENGTH];
			U8 type=0;
			U8 length=(PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.FailedIndex[CurrentIndex]]].tel.length>(MAX_DIGITS/2))?(MAX_DIGITS/2):PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.FailedIndex[CurrentIndex]]].tel.length;
			memset(PhbNuminBCD, 0, (MAX_DIGITS+1)/2);
			memcpy(PhbNuminBCD, PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.FailedIndex[CurrentIndex]]].tel.number, length);
			memset(PhbNum, 0, MAX_DIGITS+1);
			mmi_phb_convert_to_digit((U8*)PhbNum, (U8*)PhbNuminBCD, MAX_DIGITS + 1);
			memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
			if(PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.FailedIndex[CurrentIndex]]].tel.type==CSMCC_INTERNATIONAL_ADDR)
			{
				type = 1;
				AnsiiToUnicodeString((S8*)PhbNumUCS2, "+");
			}	
			AnsiiNToUnicodeString((S8*)PhbNumUCS2+type*ENCODING_LENGTH, (S8*)PhbNum, length);
			UnicodeNToAnsii((S8*)g_msg_cntx.smsPhoneNumber, (S8*)PhbNumUCS2, (length+type)*ENCODING_LENGTH);
			mmi_msg_save_msg_after_send_req(SMSAL_STO_UNSENT, (U8*)g_msg_cntx.smsPhoneNumber);
			return TRUE;
		}
		#endif
	}
	return FALSE;
}
#endif
void mmi_msg_send_msg_rsp (void* number, module_type mod, U16 result)
{
	switch(result)
	{
		case MMI_FRM_SMS_OK:
			g_msg_cntx.msg_send_info.CurrSendSmsNumbers++;
			while(g_msg_cntx.msg_send_info.CurrSendSmsNumbers<g_msg_cntx.msg_send_info.TotalSendSmsNumbers)
			{
				#ifdef __MMI_MESSAGES_SEND_TO_MANY__
				if((g_msg_cntx.sendMessageCase == SEND_CASE_SEND_TO_MANY)
					&&(pfnUnicodeStrlen((S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.CurrSendSmsNumbers])))
				{
					U8 length=pfnUnicodeStrlen(g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]);
					memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
					UnicodeNToAnsii((S8*)g_msg_cntx.smsPhoneNumber, (S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.CurrSendSmsNumbers], length*ENCODING_LENGTH);
					if(g_msg_cntx.msg_send_info.CurrSendSmsNumbers == g_msg_cntx.msg_send_info.TotalSendSmsNumbers -1)
						mmi_msg_send_to_many_last();
					mmi_msg_send_msg_req((U8*)g_msg_cntx.smsPhoneNumber, MMI_FRM_SMS_INVALID_INDEX, 1);
					return;
				}
				#endif
				#ifdef __MMI_MESSAGES_SEND_BY_GROUP__
				else if((g_msg_cntx.sendMessageCase == SEND_CASE_SEND_BY_GROUP)
					&&(g_msg_cntx.msg_send_info.PhbIndex!=NULL))
				{
					U8 PhbNuminBCD[(MAX_DIGITS+1)/2];
					U8 PhbNum[MAX_DIGITS+1];
					S8 PhbNumUCS2[(MAX_DIGITS+2)*ENCODING_LENGTH];
					U8 type=0;
					U8 length=(PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]].tel.length>(MAX_DIGITS/2))?(MAX_DIGITS/2):PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]].tel.length;
					memset(PhbNuminBCD, 0, (MAX_DIGITS+1)/2);
					memcpy(PhbNuminBCD, PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]].tel.number, length);
					memset(PhbNum, 0, MAX_DIGITS+1);
					mmi_phb_convert_to_digit((U8*)PhbNum, (U8*)PhbNuminBCD, MAX_DIGITS + 1);
					memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
					if(PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]].tel.type==CSMCC_INTERNATIONAL_ADDR)
					{
						type = 1;
						AnsiiToUnicodeString((S8*)PhbNumUCS2, "+");
					}	
					AnsiiNToUnicodeString((S8*)PhbNumUCS2+type*ENCODING_LENGTH, (S8*)PhbNum, length);
					UnicodeNToAnsii((S8*)g_msg_cntx.smsPhoneNumber, (S8*)PhbNumUCS2, (length+type)*ENCODING_LENGTH);
					if(g_msg_cntx.msg_send_info.CurrSendSmsNumbers == g_msg_cntx.msg_send_info.TotalSendSmsNumbers -1)
						mmi_msg_send_to_many_last();
					mmi_msg_send_msg_req((U8*)g_msg_cntx.smsPhoneNumber, MMI_FRM_SMS_INVALID_INDEX,1);
					return;
				}
				#endif
				g_msg_cntx.msg_send_info.CurrSendSmsNumbers++;
			}
		#ifdef __MMI_MESSAGES_CONTINUE_SEND_AFTER_FAIL__
			if(mmi_msg_save_seprate_msg_after_send_fail())
				return;
		#endif
			if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_MSG);
			else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
			else if(IsScreenPresent(SCR_ID_MSG_DEFAULT_MSG))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DEFAULT_MSG);

	#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
		#endif
			else if(IsScreenPresent(SCR_ID_MSG_WRITE))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_WRITE);
			DisplayPopup ((PU8)GetString(STR_SMS_SEND_SUCCESS), IMG_SEND_SUCCESS_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
			if(g_msg_cntx.msg_send_info.CurrSendSmsNumbers==g_msg_cntx.msg_send_info.TotalSendSmsNumbers)
			{
				mmi_frm_sms_delete_screen_history();
				DeleteMessagesHistoryNodes ();
			}
			ReleaseEMSEditBuffer();
			
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
#endif 
			AlmEnableSPOF();		
		break;
	#ifdef __MMI_MESSAGES_CONTINUE_SEND_AFTER_FAIL__	
		case MMI_FRM_SMS_ERROR:
			g_msg_cntx.msg_send_info.FailedIndex[g_msg_cntx.msg_send_info.FailedNumber++] = g_msg_cntx.msg_send_info.CurrSendSmsNumbers;
			g_msg_cntx.msg_send_info.CurrSendSmsNumbers++;
			while(g_msg_cntx.msg_send_info.CurrSendSmsNumbers<g_msg_cntx.msg_send_info.TotalSendSmsNumbers)
			{//send next
				#ifdef __MMI_MESSAGES_SEND_TO_MANY__
				if((g_msg_cntx.sendMessageCase == SEND_CASE_SEND_TO_MANY)
					&&(pfnUnicodeStrlen((S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.CurrSendSmsNumbers])))
				{
					U8 length=pfnUnicodeStrlen(g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]);
					memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
					UnicodeNToAnsii((S8*)g_msg_cntx.smsPhoneNumber, (S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.msg_send_info.CurrSendSmsNumbers], length*ENCODING_LENGTH);
					if(g_msg_cntx.msg_send_info.CurrSendSmsNumbers == g_msg_cntx.msg_send_info.TotalSendSmsNumbers -1)
						mmi_msg_send_to_many_last();
					mmi_msg_send_msg_req((U8*)g_msg_cntx.smsPhoneNumber, MMI_FRM_SMS_INVALID_INDEX, 1);
					return;
				}
				#endif
				#ifdef __MMI_MESSAGES_SEND_BY_GROUP__
				else if((g_msg_cntx.sendMessageCase == SEND_CASE_SEND_BY_GROUP)
					&&(g_msg_cntx.msg_send_info.PhbIndex!=NULL))
				{
					U8 PhbNuminBCD[(MAX_DIGITS+1)/2];
					U8 PhbNum[MAX_DIGITS+1];
					S8 PhbNumUCS2[(MAX_DIGITS+2)*ENCODING_LENGTH];
					U8 type=0;
					U8 length=(PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]].tel.length>(MAX_DIGITS/2))?(MAX_DIGITS/2):PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]].tel.length;
					memset(PhbNuminBCD, 0, (MAX_DIGITS+1)/2);
					memcpy(PhbNuminBCD, PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]].tel.number, length);
					memset(PhbNum, 0, MAX_DIGITS+1);
					mmi_phb_convert_to_digit((U8*)PhbNum, (U8*)PhbNuminBCD, MAX_DIGITS + 1);
					memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
					if(PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[g_msg_cntx.msg_send_info.CurrSendSmsNumbers]].tel.type==CSMCC_INTERNATIONAL_ADDR)
					{
						type = 1;
						AnsiiToUnicodeString((S8*)PhbNumUCS2, "+");
					}	
					AnsiiNToUnicodeString((S8*)PhbNumUCS2+type*ENCODING_LENGTH, (S8*)PhbNum, length);
					UnicodeNToAnsii((S8*)g_msg_cntx.smsPhoneNumber, (S8*)PhbNumUCS2, (length+type)*ENCODING_LENGTH);
					if(g_msg_cntx.msg_send_info.CurrSendSmsNumbers == g_msg_cntx.msg_send_info.TotalSendSmsNumbers -1)
						mmi_msg_send_to_many_last();
					mmi_msg_send_msg_req((U8*)g_msg_cntx.smsPhoneNumber, MMI_FRM_SMS_INVALID_INDEX,1);
					return;
				}
				#endif
				g_msg_cntx.msg_send_info.CurrSendSmsNumbers++;
			}
		
			if(mmi_msg_save_seprate_msg_after_send_fail())
				return;

			if (IsScreenPresent(SCR_ID_MSG_OUTBOX_SEND_OPTION))
			{
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
				DisplayPopup ((PU8)GetString(STR_SMS_SEND_FAILED), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
				mmi_frm_sms_delete_screen_history();
				DeleteMessagesHistoryNodes ();
				AlmEnableSPOF();		
			}
			else
			        mmi_msg_save_msg_after_send_req(SMSAL_STO_UNSENT, number);
			
			break;
		#endif	
		case MMI_FRM_SMS_ABORT:

#if ( defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#endif

		/*Start : JP to fix tracker issue 84 */
#ifdef __MMI_SMART_MESSAGE_MT__
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
#endif	/* __MMI_SMART_MESSAGE_MT__ */
		/*End : JP to fix tracker issue 84 */
		if(IsScreenPresent(SCR_ID_MSG_WRITE_OPTION))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_WRITE_OPTION);
#ifdef __MMI_SMART_MESSAGE_MT__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
			else if(IsScreenPresent(SCR_ID_MSG_SEND_OPTION))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_SEND_OPTION);
			else if (IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);		
			DisplayPopup ((PU8)GetString (STR_ABORT_SENDING_SMS), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
			mmi_frm_sms_delete_screen_history();
			DeleteMessagesHistoryNodes ();
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_NOTREADY:
			DisplayPopup ((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);			
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_RETURN:
		case MMI_FRM_SMS_END:
#ifdef __MMI_SMART_MESSAGE_MT__
/* under construction !*/
/* under construction !*/
#endif	/* __MMI_SMART_MESSAGE_MT__ */
		break;
		default:
			/* when sending fail, only save the content to outbox if the message is not sent from outbox originally */
			if (IsScreenPresent(SCR_ID_MSG_OUTBOX_SEND_OPTION))
			{
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
				DisplayPopup ((PU8)GetString(STR_SMS_SEND_FAILED), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
				mmi_frm_sms_delete_screen_history();
				DeleteMessagesHistoryNodes ();
				AlmEnableSPOF();		
			}
			else
			        mmi_msg_save_msg_after_send_req(SMSAL_STO_UNSENT, number);
		break;
	}
	mmi_msg_send_to_many_last();
}

void mmi_msg_send_msg_req (U8* number, U16 replyindex, U8 hidenumscreen)
{
	
	
	mmi_frm_sms_send_struct * sendData=OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
	ClearKeyHandler(KEY_LSK,KEY_EVENT_UP);//wangwb add @2007.06.29
	if(GetMessagesCurrScrnID()!=SCR_ID_MSG_OUTBOX_SEND_OPTION)	
	sendData->sendrequire |= MMI_FRM_SMS_SAVE_AFTER_FAIL;
	memset(sendData->number, 0, MAX_DIGITS_SMS);
	if(strlen((S8*)number))
	   memcpy((S8*)sendData->number, (S8*)number, strlen((S8*)number));
	else if(replyindex!=MMI_FRM_SMS_INVALID_INDEX)
	{
		sendData->sendrequire |= MMI_FRM_SMS_REPLY;
		sendData->replyindex=replyindex;
	}
	/* for send to many or send to group, show only one sending screen for all numbers */
	if((g_msg_cntx.msg_send_info.CurrSendSmsNumbers+1)<g_msg_cntx.msg_send_info.TotalSendSmsNumbers)
		sendData->sendrequire |= MMI_FRM_SMS_MORE;
	/* if sendcheck has a bit of MMI_FRM_SMS_SCR, do not show sending screen */
	if(g_msg_cntx.msg_send_info.CurrSendSmsNumbers!=0)
		sendData->sendcheck |= MMI_FRM_SMS_SCR;
	if(!hidenumscreen)
	{
		if(IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
			sendData->sendrequire|= MMI_FRM_SMS_NUM_SCR;
	}
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
#endif 
#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
#endif	/* __MMI_SMART_MESSAGE_MT__ */
	mmi_frm_sms_send_sms(mmi_msg_send_msg_rsp, MOD_MMI, sendData);
	OslMfree(sendData);
}

void mmi_msg_send_and_save_msg_rsp (void* number, module_type mod, U16 result)
{
	switch(result)
	{
		case MMI_FRM_SMS_OK:
			mmi_msg_save_msg_after_send_req(SMSAL_STO_SENT, number);
		break;
		case MMI_FRM_SMS_ABORT:
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
	#endif
			if(IsScreenPresent(SCR_ID_MSG_WRITE_OPTION))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_WRITE_OPTION);

#if defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_SMART_MESSAGE_MO__)
/* under construction !*/
/* under construction !*/
#endif
			else if(IsScreenPresent(SCR_ID_MSG_SEND_OPTION))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_SEND_OPTION);
			else if (IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);		
		/*#ifdef __MMI_SMART_MESSAGE_MT__
			else if(IsScreenPresent(SCR_ID_MYPIC_DISPLAY))
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MYPIC_DISPLAY);
		#endif*/
			DisplayPopup ((PU8)GetString (STR_ABORT_SENDING_SMS), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
			mmi_frm_sms_delete_screen_history();
			DeleteMessagesHistoryNodes ();
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_NOTREADY:
			DisplayPopup ((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);			
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_RETURN:
		case MMI_FRM_SMS_END:
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__) )
/* under construction !*/
/* under construction !*/
	#endif
		break;
		default:
			mmi_msg_save_msg_after_send_req(SMSAL_STO_UNSENT, number);
		break;
	}
}

void mmi_msg_send_and_save_msg_req (U8* number, U16 replyindex)
{
	mmi_frm_sms_send_struct * sendData=OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
	sendData->sendrequire |= MMI_FRM_SMS_SAVE;
	memset(sendData->number, 0, MAX_DIGITS_SMS);	
	if(number!=NULL)
		memcpy((S8*)sendData->number, (S8*)number, strlen((S8*)number));
	else if(replyindex!=MMI_FRM_SMS_INVALID_INDEX)
	{
		sendData->sendrequire |=MMI_FRM_SMS_REPLY;//wangzl:modify from '=' to '|=' for bug 6898 @ 071029
		sendData->replyindex=replyindex;
	}

#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	/*__MMI_SMART_MESSAGE_MO__*/
//#else
#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
#endif
	mmi_frm_sms_send_sms(mmi_msg_send_and_save_msg_rsp, MOD_MMI, sendData);
	OslMfree(sendData);
}



void mmi_msg_save_msg_after_send_rsp (void* status, module_type mod, U16 result)
{
	U8 sendresult=*(U8*)status;
	switch(result)
	{
		case MMI_FRM_SMS_OK:
		#ifdef __MMI_MESSAGES_CONTINUE_SEND_AFTER_FAIL__
			if(mmi_msg_save_seprate_msg_after_send_fail())
				return;
		#endif
			if(sendresult == SMSAL_STO_UNSENT)
                    {         
#ifdef __MMI_MESSAGES_DRAFT_BOX__
                        DisplayPopup ((PU8)GetString(STR_SMS_SEND_FAILED_SAVE_DRAFT_SUCCESS), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
#else
				DisplayPopup ((PU8)GetString(STR_SMS_SEND_FAILED_SAVE_SUCCESS), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
#endif
                    }
			else
				DisplayPopup ((PU8)GetString(STR_SMS_SEND_SAVE_SUCESS), IMG_SEND_SUCCESS_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
#endif 
		break;
		case MMI_FRM_SMS_ERROR:
		case MMI_FRM_SMS_MEMFULL:
			if(sendresult == SMSAL_STO_UNSENT)
                        {
				DisplayPopup ((PU8)GetString (STR_SMS_SEND_SAVE_FAILED), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif 
			}
			else
				DisplayPopup ((PU8)GetString(STR_SMS_SEND_SUCESS_SAVE_FILED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		break;
		default :
			DisplayPopup ((PU8)GetString(STR_SMS_SEND_SAVE_FAILED), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);			
			#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
			#endif 
		break;
	}
	if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_DEFAULT_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DEFAULT_MSG);
#ifdef __MMI_SMART_MESSAGE_MT__
/* under construction !*/
/* under construction !*/
#endif
	else if(IsScreenPresent(SCR_ID_MSG_WRITE))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_WRITE);
	mmi_frm_sms_delete_screen_history();
	DeleteMessagesHistoryNodes ();
	AlmEnableSPOF();		

#ifdef __MMI_MESSAGES_CONTINUE_SEND_AFTER_FAIL__
	g_msg_cntx.msg_send_info.FailedSaveNumber = 0;
	g_msg_cntx.msg_send_info.FailedNumber = 0;
#endif
}

void mmi_msg_save_msg_after_send_req (U8 status, void* number)
{
	mmi_frm_sms_send_struct * sendData=OslMalloc(sizeof(mmi_frm_sms_send_struct));
	EMSData* data=GetEMSDataForEdit(0,0);
	memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
	sendData->dcs=data->dcs;
	sendData->status=status;
	if(number != NULL)
		memcpy((S8*)sendData->number, (S8*)number, strlen((S8*)number));
	mmi_frm_sms_save_sms(mmi_msg_save_msg_after_send_rsp, MOD_MMI, sendData);
	OslMfree(sendData);
}

 
void mmi_msg_search_phb (void)
{
	g_msg_cntx.PhbSmsInterfaceState = MMI_SEND_ONLY;
	mmi_phb_sms_enter_send_entry (); 
}



#ifdef __MMI_MESSAGES_SEND_TO_MANY__
void mmi_msg_send_msg_to_many (void)
{
	U8 last=MAX_SEND_TO_MANY_ITEMS;
	S16 avail=-1;
	U8 total=0;

	/*Clear key immediately to avoid user press twice.*/
	ClearKeyHandler(KEY_LSK,KEY_EVENT_UP);
	
	while (last>0)
	{
		last--;
		if(pfnUnicodeStrlen((S8*)g_msg_cntx.msg_send_info.PhbNumber[last]))
		{
			/* search for the first free entry to sort the list */
			while (avail<last)
			{
				avail++;
				if (!pfnUnicodeStrlen((S8*)g_msg_cntx.msg_send_info.PhbNumber[avail]))
				{
					memcpy(g_msg_cntx.msg_send_info.PhbNumber[avail], g_msg_cntx.msg_send_info.PhbNumber[last], sizeof(g_msg_cntx.msg_send_info.PhbNumber[last]));
					memset(g_msg_cntx.msg_send_info.PhbNumber[last], 0, ENCODING_LENGTH);
					break;
				}
			}
		}
	}
	/* calculate total valid entries */
	for (total=0; total<MAX_SEND_TO_MANY_ITEMS; total++)
	{
		if (!pfnUnicodeStrlen((S8*)g_msg_cntx.msg_send_info.PhbNumber[total]))
			break;
	}
	if (avail==-1)
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
	{
		U8 length=pfnUnicodeStrlen(g_msg_cntx.msg_send_info.PhbNumber[0]);
		memset (g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
		g_msg_cntx.msg_send_info.TotalSendSmsNumbers=total;
		g_msg_cntx.msg_send_info.CurrSendSmsNumbers=0;
		UnicodeNToAnsii((S8*)g_msg_cntx.smsPhoneNumber, (S8*)g_msg_cntx.msg_send_info.PhbNumber[0], length*ENCODING_LENGTH);
		mmi_msg_send_msg_req((U8*)g_msg_cntx.smsPhoneNumber, MMI_FRM_SMS_INVALID_INDEX,1);
	}
}

#endif
#ifdef __MMI_MESSAGES_SEND_BY_GROUP__
void mmi_msg_send_msg_to_group (U16 cnt, U16* PhbIndex)
{
	U16	i;
	U16	actualCount;
	U8 length;
	U8 numbers[(MAX_DIGITS+1)/2];
	U8	tempNumber[MAX_DIGITS+1];

	if(cnt==0)
	{
		DisplayPopup ((PU8)GetString (STR_MESSAGES_EMPTY_GROUP_ID), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
		return;
	}

	if(g_msg_cntx.msg_send_info.PhbIndex!=NULL)
	{
		OslMfree(g_msg_cntx.msg_send_info.PhbIndex);
		g_msg_cntx.msg_send_info.PhbIndex=NULL;
	}
	g_msg_cntx.msg_send_info.TotalSendSmsNumbers=0;
	g_msg_cntx.msg_send_info.PhbIndex=OslMalloc(cnt*sizeof(U16));

	for(i = 0, actualCount = 0; i < cnt; i++)
	{
		length=(PhoneBook[PhbIndex[i]].tel.length>(MAX_DIGITS/2))?(MAX_DIGITS/2):PhoneBook[PhbIndex[i]].tel.length;
		memset(numbers, 0, (MAX_DIGITS+1)/2);
		memcpy(numbers, PhoneBook[PhbIndex[i]].tel.number, length);
		if ((PhoneBook[PhbIndex[i]].tel.length == 0) || (mmi_phb_convert_to_digit(tempNumber, (U8*)numbers, MAX_DIGITS + 1) == 0))
			continue;
		
		g_msg_cntx.msg_send_info.PhbIndex[actualCount]=PhbIndex[i];
		actualCount++;
	}
	
	g_msg_cntx.msg_send_info.TotalSendSmsNumbers = (U16)actualCount;

	if(g_msg_cntx.msg_send_info.TotalSendSmsNumbers > 0)
	{
		U8 PhbNuminBCD[(MAX_DIGITS+1)/2];
		U8 PhbNum[MAX_DIGITS+1];
		S8 PhbNumUCS2[(MAX_DIGITS+2)*ENCODING_LENGTH];
		U8 type=0;
		length=(PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[0]].tel.length>(MAX_DIGITS/2))?(MAX_DIGITS/2):PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[0]].tel.length;
		memset(PhbNuminBCD, 0, (MAX_DIGITS+1)/2);
		memcpy(PhbNuminBCD, PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[0]].tel.number, length);
		memset(PhbNum, 0, MAX_DIGITS+1);
		mmi_phb_convert_to_digit((U8*)PhbNum, (U8*)PhbNuminBCD, MAX_DIGITS + 1);
		memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
		if(PhoneBook[g_msg_cntx.msg_send_info.PhbIndex[0]].tel.type==CSMCC_INTERNATIONAL_ADDR)
		{
			type = 1;
			AnsiiToUnicodeString((S8*)PhbNumUCS2, "+");
		}	
		AnsiiNToUnicodeString((S8*)PhbNumUCS2+type*ENCODING_LENGTH, (S8*)PhbNum, length);
		g_msg_cntx.msg_send_info.CurrSendSmsNumbers=0;
		UnicodeNToAnsii((S8*)g_msg_cntx.smsPhoneNumber, (S8*)PhbNumUCS2, (length+type)*ENCODING_LENGTH);
		mmi_msg_send_msg_req((U8*)g_msg_cntx.smsPhoneNumber, MMI_FRM_SMS_INVALID_INDEX,1);
	}
	else
	{
		if(g_msg_cntx.msg_send_info.PhbIndex!=NULL)
		{
			OslMfree(g_msg_cntx.msg_send_info.PhbIndex);
			g_msg_cntx.msg_send_info.PhbIndex=NULL;
		}
		g_msg_cntx.msg_send_info.TotalSendSmsNumbers=0;
		DisplayPopup ((PU8)GetString (STR_MESSAGES_EMPTY_GROUP_ID), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	}
}
void mmi_msg_send_msg_by_group (void)
{
	g_msg_cntx.sendMessageCase = SEND_CASE_SEND_BY_GROUP;
	mmi_phb_sms_enter_send_by_group ();
}
#endif
void mmi_msg_send_msg_to_only_entry (void)
{
	/*Clear key immediately to avoid user press twice.*/
	ClearKeyHandler(KEY_LSK,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);

	if(g_msg_cntx.sendMessageCase == SEND_CASE_REPLY)
	{
		mmi_msg_reply_msg();
		return;
	}
	g_msg_cntx.msg_send_info.CurrSendSmsNumbers=0;
	g_msg_cntx.msg_send_info.TotalSendSmsNumbers=1;
	/* pressing SEND key from send to many phone number editor  or send message from phonebook */
	if (g_msg_cntx.sendMessageCase == SEND_CASE_SEND_TO_MANY || g_msg_cntx.sendMessageCase == SEND_CASE_SEND_FROM_PHB || g_msg_cntx.sendMessageCase == SEND_CASE_SEND_FROM_CLG)//wangwb @2007.06.12
	{
		/* convert g_msg_cntx.smsPhoneNumber from unicode to ascii */
		S8	phonenumber [(MAX_DIGITS+1)*ENCODING_LENGTH];
		U8	length=pfnUnicodeStrlen(g_msg_cntx.smsPhoneNumber);
		memset(phonenumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
		pfnUnicodeStrncpy(phonenumber, g_msg_cntx.smsPhoneNumber, length*ENCODING_LENGTH);
		memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
		UnicodeNToAnsii((S8*)g_msg_cntx.smsPhoneNumber, (S8*)phonenumber, length*ENCODING_LENGTH);
	}
	else if ((g_msg_cntx.sendMessageCase != SEND_CASE_SEND_FROM_PHB) && (g_msg_cntx.sendMessageCase != SEND_CASE_SEND_FROM_CLG))//wangwb @2007.06.12
	{
		memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
		if (GetMessagesCurrScrnID() == SCR_ID_MSG_OUTBOX_SEND_OPTION)
		{
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
			if(IsScreenPresent(SCR_ID_MSG_DRAFTBOX_LIST))
				memcpy(g_msg_cntx.smsPhoneNumber, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_DRAFTS, (U16)g_msg_cntx.currBoxIndex), MAX_DIGITS_SMS);
			else
		#endif
				memcpy(g_msg_cntx.smsPhoneNumber, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_OUTBOX, (U16)g_msg_cntx.currBoxIndex), MAX_DIGITS_SMS);
		}
	}
	mmi_msg_send_msg_req((U8*)g_msg_cntx.smsPhoneNumber, MMI_FRM_SMS_INVALID_INDEX,0);
}
#ifdef __MMI_SEND_BY_SEARCH_NAME__	
void mmi_msg_send_msg_to_only_name_entry (void)
{
	g_msg_cntx.PhbSmsInterfaceState = MMI_SEND_BY_NAME;
	phbListView = MMI_PHB_LIST_FOR_SMS_SEARCH_NAME;
	memset (g_msg_cntx.smsPhoneNumber, 0, ENCODING_LENGTH);	
	mmi_phb_search_name_pre_entry();
}

#endif

void mmi_msg_save_msg_rsp(void* dummy, module_type mod, U16 result)
{
	U16 totalInbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
	U16 totalOutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
	U16 totalDraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);
	if(result==MMI_FRM_SMS_OK)
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	}
	/*Guojian Add Start For 10242 Ver: GreenStone  on 2008-11-1 15:6 */
	else if(result == MMI_FRM_SMS_NOTREADY)
	{
		DisplayPopup ((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);
		DeleteMessagesHistoryNodes();
		mmi_msg_set_msg_num_hint(totalInbox, totalOutbox, totalDraftbox);

		return;			
	}
	/*Guojian Add End  For 10242 Ver: GreenStone  on 2008-11-1 15:6 */
	else
	{
		DisplayPopup ((PU8)GetString (STR_SMS_FAILURE_MEM_FULL), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		

	}
	if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_DEFAULT_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DEFAULT_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_WRITE))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_WRITE);
	else
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);
	DeleteMessagesHistoryNodes();
	mmi_msg_set_msg_num_hint(totalInbox, totalOutbox, totalDraftbox);
}

void mmi_msg_save_msg_req(void)
{
	mmi_frm_sms_send_struct * sendData=OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
	sendData->status=SMSAL_STO_UNSENT;
	memset((S8*)sendData->number, 0, MAX_DIGITS_SMS);
	
	if(g_msg_cntx.sendMessageCase == SEND_CASE_REPLY)
	{	
		memcpy(sendData->number, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex),MAX_DIGITS_SMS);
	}

#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
#endif 
	mmi_frm_sms_save_sms(mmi_msg_save_msg_rsp, MOD_MMI, sendData);
	OslMfree(sendData);
}

void mmi_msg_save_msg_to_entry (void)
{       
	mmi_msg_set_processing_screen (STR_MESSAGE_MAIN_MENU_CAPTION, STR_GLOBAL_SAVING, IMG_GLOBAL_SAVE, 0);
	mmi_msg_entry_processing_generic();
	mmi_msg_save_msg_req();
}
void mmi_msg_send_and_save_to_entry (void)
{
	if(g_msg_cntx.sendMessageCase == SEND_CASE_REPLY)
	{
		mmi_msg_send_and_save_msg_req(NULL, (U16)g_msg_cntx.currBoxIndex);

	}
	else 
	{
		mmi_msg_send_and_save_msg_req(NULL, MMI_FRM_SMS_INVALID_INDEX);
	}
}
void mmi_msg_reply_msg (void)
{
	memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
	g_msg_cntx.msg_send_info.CurrSendSmsNumbers=0;
	g_msg_cntx.msg_send_info.TotalSendSmsNumbers=1;
	mmi_msg_send_msg_req((U8*)g_msg_cntx.smsPhoneNumber, (U16)g_msg_cntx.currBoxIndex,1);
}

void mmi_msg_reply_msg_to_entry (void)
{
	g_msg_cntx.sendMessageCase = SEND_CASE_REPLY;
	ReleaseEMSEditBuffer();
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
#endif 
	mmi_msg_entry_write_msg();
}
void mmi_msg_get_number_from_phb (U8 name[], U8 number[])
{
	if(pfnUnicodeStrlen ((PS8)number) >  (MAX_DIGITS_SMS-1))
	{
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		return;
	}

#if !(defined __MMI_MULTI_SIM__)
	if( !AllowFixedDial(number) )//man for fdn
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);	
		return;
	}//wangwb add @2007.06.20
#endif
	memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
	if(number)
		pfnUnicodeStrcpy (g_msg_cntx.smsPhoneNumber, (PS8)number);
	if(g_msg_cntx.PhbSmsInterfaceState == MMI_SEND_SMS_FROM_PHB)
	{
		g_msg_cntx.sendMessageCase = SEND_CASE_SEND_FROM_PHB;
		mmi_msg_entry_write_msg();
	}
	else if(IsScreenPresent(SCR_ID_MSG_NUMBER))
	{
		/* put the selected number to editor */
		HistoryReplace(SCR_ID_MSG_NUMBER, SCR_ID_MSG_NUMBER, mmi_msg_entry_phone_number);	
		GoBackToHistory(SCR_ID_MSG_NUMBER);
	}
	else
		mmi_msg_entry_phone_number();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_number_from_clg
* DESCRIPTION
*   get number form call log
*
* PARAMETERS
*    IN		item number[]
* RETURNS
*  none
* GLOBALS AFFECTED
*   g_msg_cntx.PhbSmsInterfaceState
*****************************************************************************/
void mmi_msg_get_number_from_clg(U8 number[])
{
	
	ASSERT(number != NULL);
#ifdef __MMI_MULTI_SIM__
	if( !AllowFixedDial(number, MMI_SIM_1) )//man for fdn
#else
	if( !AllowFixedDial(number) )//man for fdn
#endif	
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);	
		return;
	}//wangwb add @2007.06.20
	
	if (pfnUnicodeStrlen((PS8)number) >  (MAX_DIGITS_SMS-1))
	{
		DisplayPopup((PU8)GetString(STR_SMS_NUM_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		return;	
	}

	memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1) * ENCODING_LENGTH);
	
	if (number != NULL)
		pfnUnicodeStrcpy(g_msg_cntx.smsPhoneNumber, (PS8)number);
		
	if (g_msg_cntx.PhbSmsInterfaceState == MMI_SEND_SMS_FROM_CLG)
	{
		g_msg_cntx.sendMessageCase = SEND_CASE_SEND_FROM_CLG;
		mmi_msg_entry_write_msg();
	}
	
}//wangwb @2007.06.12   


void mmi_msg_edit_msg_from_view (void)
{
	U8 result = EMS_OK;
	
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
	result = CopyEMSViewToEditBuffer();
	/* copy view content to edit buffer maybe fail due to EMS LIB virtual pack fail */
	if (result != EMS_OK)
		DisplayPopup((PU8)GetString(STR_SMS_FAILURE_MSG_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
	mmi_msg_entry_write_msg();
}

void mmi_msg_delete_msg_rsp (void* dummy, module_type mod, U16 result)
{
	#ifdef __MMI_MULTI_SIM__
	U16 totalInbox = MTPNP_PFAL_SMS_Get_SMS_List_Size(MMI_FRM_SMS_APP_INBOX);
	U16 totalOutbox = MTPNP_PFAL_SMS_Get_SMS_List_Size(MMI_FRM_SMS_APP_OUTBOX);
	#else
	U16 totalInbox = mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
	U16 totalOutbox = mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);	
	#endif 
	U16 totalDraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);
	if(result==MMI_FRM_SMS_OK)
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
	if(IsScreenPresent(SCR_ID_MSG_INBOX_LIST))
	{
		if(totalInbox==0)
	{
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_LIST);
		g_msg_cntx.msg_ind_in_idle = 0;
			g_msg_cntx.currBoxIndex = 0;
	}

		/* Delete the last msg of msg list  */
		#ifndef __MMI_MULTI_SIM__
		else if(g_msg_cntx.currBoxIndex >= totalInbox)
		{
			g_msg_cntx.currBoxIndex = totalInbox -1;
		}
		#else
		MTPNP_PFAL_SMS_Modify_Index(MMI_FRM_SMS_APP_INBOX);
		#endif
	}
	else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_LIST))
	{
		if(totalOutbox==0)
		{
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_LIST);
			g_msg_cntx.currBoxIndex = 0;
		}
		
		/* Delete the last msg of msg list  */
		#ifndef __MMI_MULTI_SIM__
		else if(g_msg_cntx.currBoxIndex >= totalOutbox)
		{
			g_msg_cntx.currBoxIndex = totalOutbox -1;
		}
		#else
		MTPNP_PFAL_SMS_Modify_Index(MMI_FRM_SMS_APP_OUTBOX);
		#endif
	}
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	else if(IsScreenPresent(SCR_ID_MSG_DRAFTBOX_LIST))
	{
		if(totalDraftbox==0)
		{
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DRAFTBOX_LIST);
			g_msg_cntx.currBoxIndex = 0;
		}
		
		/* Delete the last msg of msg list  */
		else if(g_msg_cntx.currBoxIndex >= totalDraftbox)
		{
			g_msg_cntx.currBoxIndex = totalDraftbox -1;
		}
	}
	#endif
	#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
		#endif
/* under construction !*/
	#endif
	DeleteMessagesHistoryNodes();
	#ifndef __MMI_MULTI_SIM__
	g_msg_cntx.msg_full_ind[0]=FALSE;
	mmi_msg_set_msg_icon(FALSE, FALSE);
	#else
	{
		U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
		g_msg_cntx.msg_full_ind[nSimID] = FALSE;
		MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_FALSE, MTPNP_FALSE, nSimID);
	}
	#endif
	mmi_msg_set_msg_num_hint(totalInbox, totalOutbox, totalDraftbox);
}

void mmi_msg_delete_msg_req (U8 type, U16 index)
{
	mmi_frm_sms_delete_sms(mmi_msg_delete_msg_rsp, MOD_MMI, type, index);
}

void mmi_msg_delete_msg (void)
{
	switch(g_msg_cntx.toDisplayMessageList)
	{
		case TO_DISPLAY_MESSAGE_LIST_INBOX:
			mmi_msg_set_processing_screen (STR_INBOX_MENUENTRY, STR_DELETING_SMS_CAPTION, IMG_GLOBAL_DELETED, 0);
			mmi_msg_delete_msg_req(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex);
			break;
		case TO_DISPLAY_MESSAGE_LIST_OUTBOX:
			mmi_msg_set_processing_screen (STR_OUTBOX_MENUENTRY, STR_DELETING_SMS_CAPTION, IMG_GLOBAL_DELETED, 0);
			mmi_msg_delete_msg_req(MMI_FRM_SMS_APP_OUTBOX, (U16)g_msg_cntx.currBoxIndex);
			break;
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case TO_DISPLAY_MESSAGE_LIST_DRAFTBOX:
			mmi_msg_set_processing_screen (STR_DRAFTBOX_MENUENTRY, STR_DELETING_SMS_CAPTION, IMG_GLOBAL_DELETED, 0);
			mmi_msg_delete_msg_req(MMI_FRM_SMS_APP_DRAFTS, (U16)g_msg_cntx.currBoxIndex);
			break;
		#endif
		default:
			break;
	}
	mmi_msg_entry_processing_generic();	
}
void mmi_msg_delete_msg_all_inbox (void)
{
	mmi_msg_set_processing_screen (STR_INBOX_MENUENTRY, STR_DELETING_SMS_CAPTION, IMG_GLOBAL_DELETED, 0);
	mmi_msg_entry_processing_generic();
	if(IsScreenPresent(SCR_ID_MSG_INBOX_LIST))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_LIST);
	else
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);

	g_msg_cntx.msg_ind_in_idle=FALSE;
	g_msg_cntx.msg_ind_after_call=FALSE;
	
	#ifndef __MMI_MULTI_SIM__
	mmi_msg_delete_msg_req(MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX);
	#else
	switch (MTPNP_AD_SMS_Get_Filter_Rule())
	{
		case MTPNP_AD_FILTER_MASTER:
			mmi_msg_delete_dm_msg_req(DM_MASTER_BOX, MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX);        
			break;
		case MTPNP_AD_FILTER_SLAVE:
			mmi_msg_delete_dm_msg_req(DM_SLAVE_BOX, MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX);
			break;
		case MTPNP_AD_FILTER_SIM3:
			mmi_msg_delete_dm_msg_req(DM_SIM3_BOX, MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX);
			break;
		case MTPNP_AD_FILTER_SIM4:
			mmi_msg_delete_dm_msg_req(DM_SIM4_BOX, MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX);
			break;
		case MTPNP_AD_FILTER_ALL:
			mmi_msg_delete_dm_msg_req(DM_MIX_BOX, MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX);
			break;
		default:
			break;
	}
	#endif
}

void mmi_msg_pre_delete_msg_all_inbox (void)
{
	U16 confirm_str_id = STR_INBOX_DELETEALL_QUERY;
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_INBOX)==MMI_FRM_SMS_INVALID_INDEX)
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX)==0)
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	else
	{
	#ifdef __MMI_MULTI_SIM__
		#ifndef __MMI_DUAL_SIM_SINGLE_OPENED_
		confirm_str_id = STRING_MTPNP_MASTER_DELETE_INBOX_MENUENTRY + MTPNP_AD_SMS_Get_Filter_Rule();
		#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
	#endif
		mmi_msg_set_confirm_screen(confirm_str_id, mmi_msg_delete_msg_all_inbox, GoBackHistory);
		mmi_msg_entry_confirm_generic();
	}
}

void mmi_msg_delete_msg_all_outbox (void)
{
	mmi_msg_set_processing_screen (STR_OUTBOX_MENUENTRY, STR_DELETING_SMS_CAPTION, IMG_GLOBAL_DELETED, 0);
	mmi_msg_entry_processing_generic();
	if(IsScreenPresent(SCR_ID_MSG_OUTBOX_LIST))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_LIST);
	else
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);

	#ifndef __MMI_MULTI_SIM__
	mmi_msg_delete_msg_req(MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX);
	#else 
	switch (MTPNP_AD_SMS_Get_Filter_Rule())
	{
		case MTPNP_AD_FILTER_MASTER:
			mmi_msg_delete_dm_msg_req(DM_MASTER_BOX, MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX);        
			break;
		case MTPNP_AD_FILTER_SLAVE:
			mmi_msg_delete_dm_msg_req(DM_SLAVE_BOX, MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX);
			break;
		case MTPNP_AD_FILTER_SIM3:
			mmi_msg_delete_dm_msg_req(DM_SIM3_BOX, MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX);
			break;
		case MTPNP_AD_FILTER_SIM4:
			mmi_msg_delete_dm_msg_req(DM_SIM4_BOX, MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX);
			break;
		case MTPNP_AD_FILTER_ALL:
			mmi_msg_delete_dm_msg_req(DM_MIX_BOX, MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX);
			break;
		default:
			break;
	}
	#endif 
}

void mmi_msg_pre_delete_msg_all_outbox (void)
{
	U16 confrim_str_id = STR_OUTBOX_DELETEALL_QUERY;
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_OUTBOX)==MMI_FRM_SMS_INVALID_INDEX)
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX)==0)
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	else
	{
	#ifdef __MMI_MULTI_SIM__
		#ifndef __MMI_DUAL_SIM_SINGLE_OPENED_
		confrim_str_id = STRING_MTPNP_MASTER_DELETE_OUTBOX_MENUENTRY + MTPNP_AD_SMS_Get_Filter_Rule();
		#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
	#endif
		mmi_msg_set_confirm_screen(confrim_str_id, mmi_msg_delete_msg_all_outbox, GoBackHistory);
		mmi_msg_entry_confirm_generic();
	}
}

#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_delete_msg_all_draftbox (void)
{
	mmi_msg_set_processing_screen (STR_DRAFTBOX_MENUENTRY, STR_DELETING_SMS_CAPTION, IMG_GLOBAL_DELETED, 0);
	mmi_msg_entry_processing_generic();
	if(IsScreenPresent(SCR_ID_MSG_DRAFTBOX_LIST))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DRAFTBOX_LIST);
	else
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);

	mmi_msg_delete_msg_req(MMI_FRM_SMS_APP_DRAFTS, MMI_FRM_SMS_INVALID_INDEX);
}

void mmi_msg_pre_delete_msg_all_draftbox (void)
{
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_DRAFTS)==MMI_FRM_SMS_INVALID_INDEX)
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS)==0)
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	else
	{
		mmi_msg_set_confirm_screen(STR_DRAFTBOX_DELETEALL_QUERY, mmi_msg_delete_msg_all_draftbox, GoBackHistory);
		mmi_msg_entry_confirm_generic();
	}
}

#endif
#ifdef __MMI_MESSAGES_DELETE_ALL_MENU__
void mmi_msg_delete_msg_all (void)
{
	mmi_msg_set_processing_screen (STR_DELETE_ALL_MENUENTRY, STR_DELETING_SMS_CAPTION, IMG_GLOBAL_DELETED, 0);
	mmi_msg_entry_processing_generic();
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);

	mmi_msg_delete_msg_req(MMI_FRM_SMS_APP_INBOX | MMI_FRM_SMS_APP_OUTBOX | MMI_FRM_SMS_APP_DRAFTS, MMI_FRM_SMS_INVALID_INDEX);
}

void mmi_msg_pre_delete_msg_all (void)
{
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else if((mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX)==0)
		&&(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX)==0)
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		&&(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS)==0)
		#endif
		)
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	else
	{
		mmi_msg_set_confirm_screen(STR_DELETE_ALL_QUERY, mmi_msg_delete_msg_all, GoBackHistory);
		mmi_msg_entry_confirm_generic();
	}
}
#endif

#ifdef __MMI_MESSAGES_COPY__

extern void HangupAllCalls(void);
extern pBOOL isInCall(void);

void mmi_msg_copy_msg_rsp (void* data, module_type mod, U16 result)
{
	U16 totalInbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
	U16 totalOutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
	U16 totalDraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);
	//mmi_frm_sms_copy_result_struct* rsp = (mmi_frm_sms_copy_result_struct*)data;
	if(result==MMI_FRM_SMS_OK)
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	else 
	{		
		if (result == MMI_FRM_SMS_MEMFULL)
			DisplayPopup ((PU8)GetString (STR_SMS_FAILURE_MEM_FULL), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
		else
			DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);						
	}
	
#if defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	/* __MMI_SMART_MESSAGE_MT__ || __MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__ */

	if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_DEFAULT_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DEFAULT_MSG);	
	else
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);
	DeleteMessagesHistoryNodes();
	mmi_msg_set_msg_num_hint(totalInbox, totalOutbox, totalDraftbox);
}

void mmi_msg_copy_msg_all_rsp (void* data, module_type mod, U16 result)
{
	U16 totalInbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
	U16 totalOutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
	U16 totalDraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);
	mmi_frm_sms_copy_result_struct* rsp = (mmi_frm_sms_copy_result_struct*)data;

	S8 *asciiString = OslMalloc(30);
	S8 *ucs2String = OslMalloc(60);
	S8 *causeString = OslMalloc(100);

	if(result==MMI_FRM_SMS_OK)
		pfnUnicodeStrcpy ((S8*)causeString, (S8*)GetString(STR_GLOBAL_DONE));	
	else if (result == MMI_FRM_SMS_MEMFULL)
		pfnUnicodeStrcpy ((S8*)causeString, (S8*)GetString(STR_SMS_FAILURE_MEM_FULL));
	else if(result == MMI_FRM_SMS_ABORT)
		pfnUnicodeStrcpy ((S8*)causeString, (S8*)GetString(STR_GLOBAL_ABORT));
	else
		pfnUnicodeStrcpy ((S8*)causeString, (S8*)GetString(STR_GLOBAL_UNFINISHED));
	
	if(rsp->msg_number <= 1)
		sprintf ((S8*)asciiString, "\n  %d ", (U32)rsp->msg_number);
	else
		sprintf ((S8*)asciiString, "\n  %d ", (U32)rsp->msg_number);

	AnsiiToUnicodeString ((S8*)ucs2String, (S8*)asciiString);				
	pfnUnicodeStrcat ((S8*)causeString, (S8*)ucs2String);

	if(rsp->msg_number <= 1)
		pfnUnicodeStrcat ((S8*)causeString, (S8*)GetString(STR_SMS_MESSAGE_COPIED));
	else
		pfnUnicodeStrcat ((S8*)causeString, (S8*)GetString(STR_SMS_MESSAGES_COPIED));
	
	if(result==MMI_FRM_SMS_OK)
		DisplayPopup ((PU8)causeString, IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	else
		DisplayPopup ((PU8)causeString, IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
	OslMfree(asciiString);
	OslMfree(ucs2String);
	OslMfree(causeString);			
	
#if defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	/* __MMI_SMART_MESSAGE_MT__ || __MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__ */

	if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_DEFAULT_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DEFAULT_MSG);	
	else
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);
	DeleteMessagesHistoryNodes();
	mmi_msg_set_msg_num_hint(totalInbox, totalOutbox, totalDraftbox);
}

void mmi_msg_copy_msg_req (U8 type, U16 index, U8 action ,U8 dst_storage)
{
	if(index == MMI_FRM_SMS_INVALID_INDEX)
		mmi_frm_sms_copy_sms(mmi_msg_copy_msg_all_rsp, MOD_MMI, type, index,action,dst_storage);
	else
		mmi_frm_sms_copy_sms(mmi_msg_copy_msg_rsp, MOD_MMI, type, index,action,dst_storage);
}

void mmi_msg_copy_msg_all_abort()
{
	mmi_frm_sms_copy_abort();
}

void mmi_msg_copy_msg_all_from_phone_inbox(void)
{
	mmi_msg_set_processing_screen (STR_INBOX_MENUENTRY, STR_SMS_COPYING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_copy_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_copy_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_copy_msg_req(MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX,SMSAL_COPY_MSG,SMSAL_SM);	
}

void mmi_msg_copy_msg_all_from_sim_inbox(void)
{
	mmi_msg_set_processing_screen (STR_INBOX_MENUENTRY, STR_SMS_COPYING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_copy_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_copy_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_copy_msg_req(MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX,SMSAL_COPY_MSG,SMSAL_ME);	
}

void mmi_msg_copy_msg_all_from_phone_outbox(void)
{
	mmi_msg_set_processing_screen (STR_OUTBOX_MENUENTRY, STR_SMS_COPYING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_copy_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_copy_msg_all_abort, KEY_EVENT_UP);

	mmi_msg_copy_msg_req(MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX,SMSAL_COPY_MSG,SMSAL_SM);		
}


void mmi_msg_copy_msg_all_from_sim_outbox(void)
{
	mmi_msg_set_processing_screen (STR_OUTBOX_MENUENTRY, STR_SMS_COPYING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_copy_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_copy_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_copy_msg_req(MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX,SMSAL_COPY_MSG,SMSAL_ME);	
}

void mmi_msg_copy_msg (void)
{
	U8	dst_storage = SMSAL_ME;
	switch(g_msg_cntx.toDisplayMessageList)
	{
		case TO_DISPLAY_MESSAGE_LIST_INBOX:
			mmi_msg_set_processing_screen (STR_INBOX_MENUENTRY, STR_SMS_COPYING, IMG_GLOBAL_PROGRESS, 0);
			 if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;			
			mmi_msg_copy_msg_req(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex,SMSAL_COPY_MSG,dst_storage);
			break;
		case TO_DISPLAY_MESSAGE_LIST_OUTBOX:
			mmi_msg_set_processing_screen (STR_OUTBOX_MENUENTRY, STR_SMS_COPYING, IMG_GLOBAL_PROGRESS, 0);
			if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX, (U16)g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			mmi_msg_copy_msg_req(MMI_FRM_SMS_APP_OUTBOX,(U16) g_msg_cntx.currBoxIndex,SMSAL_COPY_MSG,dst_storage);
			break;
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case TO_DISPLAY_MESSAGE_LIST_DRAFTBOX:
			mmi_msg_set_processing_screen (STR_DRAFTBOX_MENUENTRY, STR_SMS_COPYING, IMG_GLOBAL_PROGRESS, 0);
			if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS,(U16) g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			mmi_msg_copy_msg_req(MMI_FRM_SMS_APP_DRAFTS,(U16) g_msg_cntx.currBoxIndex,SMSAL_COPY_MSG,dst_storage);
			break;
		#endif
	}
	mmi_msg_entry_processing_generic();	
}

#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_copy_msg_all_from_phone_draftbox(void)
{
	mmi_msg_set_processing_screen (STR_DRAFTBOX_MENUENTRY, STR_SMS_COPYING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_copy_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_copy_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_copy_msg_req(MMI_FRM_SMS_APP_DRAFTS, MMI_FRM_SMS_INVALID_INDEX,SMSAL_COPY_MSG,SMSAL_SM);	
}

void mmi_msg_copy_msg_all_from_sim_draftbox(void)
{
	mmi_msg_set_processing_screen (STR_DRAFTBOX_MENUENTRY, STR_SMS_COPYING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_copy_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_copy_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_copy_msg_req(MMI_FRM_SMS_APP_DRAFTS, MMI_FRM_SMS_INVALID_INDEX,SMSAL_COPY_MSG,SMSAL_ME);	
}
#endif


void mmi_msg_move_msg_rsp (void* data, module_type mod, U16 result)
{
	U16 totalInbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
	U16 totalOutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
	U16 totalDraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);
	//mmi_frm_sms_copy_result_struct* rsp = (mmi_frm_sms_copy_result_struct*)data;
	if(result==MMI_FRM_SMS_OK)
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	else 
	{		
		if (result == MMI_FRM_SMS_MEMFULL)
			DisplayPopup ((PU8)GetString (STR_SMS_FAILURE_MEM_FULL), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
		else
			DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);						
	}
	
#if defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	/* __MMI_SMART_MESSAGE_MT__ || __MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__ */
	if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_DEFAULT_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DEFAULT_MSG);	
	else
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);
	DeleteMessagesHistoryNodes();
	mmi_msg_set_msg_num_hint(totalInbox, totalOutbox, totalDraftbox);
}

void mmi_msg_move_msg_all_rsp (void* data, module_type mod, U16 result)
{
	U16 totalInbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
	U16 totalOutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
	U16 totalDraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);
	mmi_frm_sms_copy_result_struct* rsp = (mmi_frm_sms_copy_result_struct*)data;

	S8 *asciiString = OslMalloc(30);
	S8 *ucs2String = OslMalloc(60);
	S8 *causeString = OslMalloc(100);

	if(result==MMI_FRM_SMS_OK)
		pfnUnicodeStrcpy ((S8*)causeString, (S8*)GetString(STR_GLOBAL_DONE));	
	else if (result == MMI_FRM_SMS_MEMFULL)
		pfnUnicodeStrcpy ((S8*)causeString, (S8*)GetString(STR_SMS_FAILURE_MEM_FULL));
	else if(result == MMI_FRM_SMS_ABORT)
		pfnUnicodeStrcpy ((S8*)causeString, (S8*)GetString(STR_GLOBAL_ABORT));
	else
		pfnUnicodeStrcpy ((S8*)causeString, (S8*)GetString(STR_GLOBAL_UNFINISHED));
	
	if(rsp->msg_number <= 1)
		sprintf ((S8*)asciiString, "\n  %d ", (U32)rsp->msg_number);
	else
		sprintf ((S8*)asciiString, "\n  %d ", (U32)rsp->msg_number);

	AnsiiToUnicodeString ((S8*)ucs2String, (S8*)asciiString);				
	pfnUnicodeStrcat ((S8*)causeString, (S8*)ucs2String);

	if(rsp->msg_number <= 1)
		pfnUnicodeStrcat ((S8*)causeString, (S8*)GetString(STR_SMS_MESSAGE_MOVED));
	else
		pfnUnicodeStrcat ((S8*)causeString, (S8*)GetString(STR_SMS_MESSAGES_MOVED));
	
	if(result==MMI_FRM_SMS_OK)
		DisplayPopup ((PU8)causeString, IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	else
		DisplayPopup ((PU8)causeString, IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
	OslMfree(asciiString);
	OslMfree(ucs2String);
	OslMfree(causeString);			
	
#if defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	/* __MMI_SMART_MESSAGE_MT__ || __MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__ */
	if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_OUTBOX_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
	else if(IsScreenPresent(SCR_ID_MSG_DEFAULT_MSG))
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DEFAULT_MSG);	
	else
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);
	DeleteMessagesHistoryNodes();
	mmi_msg_set_msg_num_hint(totalInbox, totalOutbox, totalDraftbox);
}

void mmi_msg_move_msg_req (U8 type, U16 index, U8 action ,U8 dst_storage)
{
	if(index == MMI_FRM_SMS_INVALID_INDEX)
		mmi_frm_sms_copy_sms(mmi_msg_move_msg_all_rsp, MOD_MMI, type, index,action,dst_storage);
	else
		mmi_frm_sms_copy_sms(mmi_msg_move_msg_rsp, MOD_MMI, type, index,action,dst_storage);
}

void mmi_msg_move_msg_all_abort()
{
	mmi_frm_sms_copy_abort();
}

void mmi_msg_move_msg_all_from_phone_inbox(void)
{
	mmi_msg_set_processing_screen (STR_INBOX_MENUENTRY, STR_SMS_MOVING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_move_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_move_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_move_msg_req(MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX,SMSAL_MOVE_MSG,SMSAL_SM);	
}

void mmi_msg_move_msg_all_from_sim_inbox(void)
{
	mmi_msg_set_processing_screen (STR_INBOX_MENUENTRY, STR_SMS_MOVING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_move_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_move_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_move_msg_req(MMI_FRM_SMS_APP_INBOX, MMI_FRM_SMS_INVALID_INDEX,SMSAL_MOVE_MSG,SMSAL_ME);	
}

void mmi_msg_move_msg_all_from_phone_outbox(void)
{
	mmi_msg_set_processing_screen (STR_OUTBOX_MENUENTRY, STR_SMS_MOVING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_move_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_move_msg_all_abort, KEY_EVENT_UP);

	mmi_msg_move_msg_req(MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX,SMSAL_MOVE_MSG,SMSAL_SM);		
}


void mmi_msg_move_msg_all_from_sim_outbox(void)
{
	mmi_msg_set_processing_screen (STR_OUTBOX_MENUENTRY, STR_SMS_MOVING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_move_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_move_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_move_msg_req(MMI_FRM_SMS_APP_OUTBOX, MMI_FRM_SMS_INVALID_INDEX,SMSAL_MOVE_MSG,SMSAL_ME);	
}

void mmi_msg_move_msg (void)
{
	U8	dst_storage = SMSAL_ME;
	switch(g_msg_cntx.toDisplayMessageList)
	{
		case TO_DISPLAY_MESSAGE_LIST_INBOX:
			mmi_msg_set_processing_screen (STR_INBOX_MENUENTRY, STR_SMS_MOVING, IMG_GLOBAL_PROGRESS, 0);
			 if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;			
			mmi_msg_move_msg_req(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex,SMSAL_MOVE_MSG,dst_storage);
			break;
		case TO_DISPLAY_MESSAGE_LIST_OUTBOX:
			mmi_msg_set_processing_screen (STR_OUTBOX_MENUENTRY, STR_SMS_MOVING, IMG_GLOBAL_PROGRESS, 0);
			if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX,(U16) g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			mmi_msg_move_msg_req(MMI_FRM_SMS_APP_OUTBOX, (U16)g_msg_cntx.currBoxIndex,SMSAL_MOVE_MSG,dst_storage);
			break;
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case TO_DISPLAY_MESSAGE_LIST_DRAFTBOX:
			mmi_msg_set_processing_screen (STR_DRAFTBOX_MENUENTRY, STR_SMS_MOVING, IMG_GLOBAL_PROGRESS, 0);
			if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS, (U16)g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			mmi_msg_move_msg_req(MMI_FRM_SMS_APP_DRAFTS, (U16)g_msg_cntx.currBoxIndex,SMSAL_MOVE_MSG,dst_storage);
			break;
		#endif
	}
	mmi_msg_entry_processing_generic();	
}

#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_move_msg_all_from_phone_draftbox(void)
{
	mmi_msg_set_processing_screen (STR_DRAFTBOX_MENUENTRY, STR_SMS_MOVING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_move_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_move_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_move_msg_req(MMI_FRM_SMS_APP_DRAFTS, MMI_FRM_SMS_INVALID_INDEX,SMSAL_MOVE_MSG,SMSAL_SM);	
}

void mmi_msg_move_msg_all_from_sim_draftbox(void)
{
	mmi_msg_set_processing_screen (STR_DRAFTBOX_MENUENTRY, STR_SMS_MOVING, IMG_GLOBAL_PROGRESS, STR_GLOBAL_ABORT);
	mmi_msg_entry_processing_generic();

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);

	SetKeyHandler (mmi_msg_move_msg_all_abort, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_move_msg_all_abort, KEY_EVENT_UP);
	
	mmi_msg_move_msg_req(MMI_FRM_SMS_APP_DRAFTS, MMI_FRM_SMS_INVALID_INDEX,SMSAL_MOVE_MSG,SMSAL_ME);	
}
#endif // ifdef __MMI_MESSAGES_DRAFT_BOX__

#endif // ifdef __MMI_MESSAGES_COPY__

void mmi_msg_get_msg_rsp(void* data, module_type mod, U16 result)
{
	
	U8 type;
	EMSData*	pEms ;

	if(NULL == data)
		return;

	type =*(U8*)data;
	
	GetEMSDataForView (&pEms, 0);
  	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_msg_get_msg_rsp");
	switch (pEms->PortNum.dst_port)
	{
#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* __MMI_SMART_MESSAGE_MT__ || __MMI_SMART_MESSAGE_MO__ */
#ifdef	__MMI_OPERATOR_LOGO__
		case MMI_OPLOGO_MSG_PORT_NUM:
			mmi_nsm_set_msg_type(PICTURE_MESSAGE);
			mmi_op_logo_parse_ota_data();
			break;
#endif	/*__MMI_OPERATOR_LOGO__*/
#ifdef __MMI_CLI_ICON__
		case MMI_CLI_ICON_MSG_PORT_NUM:
			mmi_nsm_set_msg_type(PICTURE_MESSAGE);
			mmi_cli_icon_parse_ota_data();
			break;
#endif	/* __MMI_CLI_ICON__ */
		default:				/* MMI_NORMAL_MSG_PORT_NUM */
#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
		#endif

			/* The SMS is NSM so that SMS content shall show "Not Support" */
			if (pEms->PortNum.dst_port == 0x1581 || pEms->PortNum.dst_port == 0x158A
				|| pEms->PortNum.dst_port == 0x1582 || pEms->PortNum.dst_port == 0x1583)
			{
				type |= (SMSAL_MTI_UNSPECIFIED<<4);
				PendingSaveSendData.mti = SMSAL_MTI_UNSPECIFIED;
				mmi_trace(g_sw_SMS,"MMI_SMS: mmi_msg_get_msg_rsp, pEms->PortNum.dst_port is: 0x%x, will entry mmi_msg_entry_default_msg",pEms->PortNum.dst_port);
				mmi_msg_entry_default_msg();
				break;				
			}
			mmi_trace(g_sw_SMS,"MMI_SMS: mmi_msg_get_msg_rsp, type is:0x%x",type);
			switch(type&0x0f)
			{
				case MMI_FRM_SMS_APP_UNREAD:
				case MMI_FRM_SMS_APP_INBOX:
					if((type&0xf0)>>4==SMSAL_MTI_STATUS_REPORT)
						mmi_msg_entry_default_msg();
					else
						mmi_msg_entry_inbox_msg();
					break;
				case MMI_FRM_SMS_APP_OUTBOX:
				case MMI_FRM_SMS_APP_DRAFTS:
					mmi_msg_entry_outbox_msg();
					break;
				default:
					mmi_msg_entry_default_msg();
					break;
			}
			break;

	}
	
	DeleteScreenIfPresent(SCR_ID_MSG_PROCESSING);
	#ifndef __MMI_MULTI_SIM__
	mmi_msg_set_msg_icon(FALSE, FALSE);
	#else
	{
		U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
		MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_FALSE, MTPNP_FALSE, nSimID);
	}
	#endif
}
void mmi_msg_get_msg_req (U8 type, U16 index)
{
	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	if(mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	mmi_msg_set_processing_screen (STR_MESSAGE_MAIN_MENU_CAPTION, STR_READING_SMS, IMG_GLOBAL_PROGRESS, 0);		
	mmi_msg_entry_processing_generic();
	mmi_frm_sms_read_sms(mmi_msg_get_msg_rsp, MOD_MMI, type, index);
}

void mmi_msg_get_msg_inbox (void)
{
	g_msg_cntx.toDisplayMessageList=TO_DISPLAY_MESSAGE_LIST_INBOX;
	mmi_msg_get_msg_req(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex);
}
void mmi_msg_get_msg_outbox (void)
{
	g_msg_cntx.toDisplayMessageList=TO_DISPLAY_MESSAGE_LIST_OUTBOX;
	mmi_msg_get_msg_req(MMI_FRM_SMS_APP_OUTBOX, (U16)g_msg_cntx.currBoxIndex);
}

#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_get_msg_draftbox (void)
{
	g_msg_cntx.toDisplayMessageList=TO_DISPLAY_MESSAGE_LIST_DRAFTBOX;
	mmi_msg_get_msg_req(MMI_FRM_SMS_APP_DRAFTS, (U16)g_msg_cntx.currBoxIndex);
}

#endif
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
#endif

U8 mmi_msg_check_class0_msg(void* data, int withobject, void* content)
{
	mmi_frm_sms_deliver_msg_struct* msg=(mmi_frm_sms_deliver_msg_struct*)data;
	if(msg->display_type==SMSAL_MSG_TYPE_CLASS0)
		return TRUE;
	return FALSE;
}

#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
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
#if (0)
/* under construction !*/
/* under construction !*/
#endif	/* (0) */
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /*__MMI_SMART_MESSAGE_MT__*/
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
	#if	defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
	#elif	defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
	#endif	/* __MMI_SMART_MESSAGE_MO__ */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#if	defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
	#elif defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
	#endif	/* __MMI_SMART_MESSAGE_MO__ */
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

// wangbei add start 20060614
/*
U8 CheckForChatMessage (void* data, int withobject, void* content)
{
    return FALSE;
}
U8 HandleChatMessage (void* data, void* content)
{
	return FALSE;
}*/
// wangbei add end
void mmi_msg_set_protocol_event_handler (void)
{
	mmi_frm_sms_reg_interrupt_check(MOD_MMI, PRT_MSG_ID_MMI_SMS_DELIVER_MSG_IND, mmi_msg_handle_new_msg_ind);
	mmi_frm_sms_reg_interrupt_check(MOD_MMI, PRT_MSG_ID_MMI_SMS_STATUS_REPORT_IND, mmi_msg_handle_deliver_report_ind);
	mmi_frm_sms_reg_interrupt_check(MOD_MMI, PRT_MSG_ID_MMI_SMS_READY_IND, mmi_msg_handle_ready_ind);
	mmi_frm_sms_reg_interrupt_check(MOD_MMI, PRT_MSG_ID_MMI_SMS_MEM_AVAILABLE_IND, mmi_msg_handle_mem_avail_ind);
	mmi_frm_sms_reg_interrupt_check(MOD_MMI, PRT_MSG_ID_MMI_SMS_MEM_EXCEED_IND, mmi_msg_handle_mem_exceed_ind);
	mmi_frm_sms_reg_interrupt_check(MOD_MMI, PRT_MSG_ID_MMI_SMS_MEM_FULL_IND, mmi_msg_handle_mem_full_ind);
	mmi_frm_sms_reg_interrupt_check(MOD_MMI, PRT_MSG_ID_MMI_SMS_MSG_WAITING_IND, mmi_msg_handle_msg_waiting_ind);
	mmi_frm_sms_reg_interrupt_check(MOD_MMI, PRT_MSG_ID_MMI_SMS_SYNC_MSG_IND, mmi_msg_sync_msgbox_with_at);
	mmi_frm_sms_reg_msg_check(mmi_msg_check_class0_msg, mmi_msg_handle_class0_msg_ind);
	#ifdef __MMI_MESSAGES_CHAT__
	mmi_frm_sms_reg_msg_check(CheckForChatMessage, HandleChatMessage);
	#endif
}

#ifdef __MMI_MULTI_SIM__
/*****************************************************************************
 * FUNCTION
 *  mmi_msg_dm_delete_all_msg_rsp
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dummy       [?]         
 *  mod         [IN]        
 *  result      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_dm_delete_all_msg_rsp(void *dummy, module_type mod, U16 result)
{
    mmi_msg_delete_msg_rsp(dummy, mod, result);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_msg_delete_dm_msg_req
 * DESCRIPTION
 *  
 * PARAMETERS
 * side         [IN]
 *  type        [IN]        
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_delete_dm_msg_req(U8 side, U16 type, U16 index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	#ifdef __UNIFIED_MESSAGE_ARCHIVE_SUPPORT__
	if (type == MMI_FRM_SMS_APP_ARCHIVE)
	{
		mmi_frm_sms_delete_one_archive_msg_req(mmi_msg_delete_msg_rsp, type, index);    
	}	
	else	
	#endif
	{
		mmi_frm_sms_dm_delete_sms(side, mmi_msg_dm_delete_all_msg_rsp, MOD_MMI, type, index);
	}
}
#endif
#endif

