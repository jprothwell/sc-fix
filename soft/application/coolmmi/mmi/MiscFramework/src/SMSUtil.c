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
 *  SMSUtil.c
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
#include "phonebookgprot.h"
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
#include "smsfunc.h"

#include "mmi_trace.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "mtpnp_ad_resdef.h"
static mmi_frm_sms_send_struct *g_pSendData = NULL;
static PsFuncPtrU16 g_callback;
static module_type g_mod_src;
U16 mmi_frm_sms_show_list_index[MMI_SMS_MAX_MSG_NUM];
#endif
#define smstest


#define MMI_FRM_SMS_INVALID_INDEX	0xffff
#ifndef MMI_FRM_SMS_INVALID_NUM
#define MMI_FRM_SMS_INVALID_NUM		0xff
#endif
#ifndef MMI_FRM_SMS_MSG_LEN
#define MMI_FRM_SMS_MSG_LEN			160
#endif
#ifndef MMI_FRM_SMS_MSG_CONTENT_NUM
#define MMI_FRM_SMS_MSG_CONTENT_NUM	20
#endif
#ifndef MMI_FRM_SMS_ACTION_SIZE
#define MMI_FRM_SMS_ACTION_SIZE		10
#endif
static U16 mmi_frm_sms_msgbox_size=0;
static U16 mmi_frm_sms_inbox_size=0;
static U16 mmi_frm_sms_outbox_size=0;
#ifdef __MMI_MESSAGES_DRAFT_BOX__
static U16 mmi_frm_sms_drafts_size=0;
#endif
static U8 mmi_frm_sms_list_done=FALSE;

static mmi_frm_sms_deliver_msg_struct mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE+1];
static mmi_frm_sms_deliver_data_struct mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE+1];
static mmi_frm_sms_content_struct mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM+1];

#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
static mmi_frm_sms_delivery_report_list_struct mmi_frm_sms_delivery_report_awaited[MMI_FRM_SMS_ACTION_SIZE];
#endif

static U8 mmi_frm_sms_awaited_counter[MMI_FRM_SMS_ACTION_SIZE];
static U8 mmi_frm_sms_awaited_counting=FALSE;
static U8 mmi_frm_sms_data_counter[MMI_FRM_SMS_ACTION_SIZE];
static U8 mmi_frm_sms_data_counting=FALSE;
static U16 mmi_frm_sms_data_checked = 0;

#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
static U8 mmi_frm_sms_delivery_report_awaited_next_available = 0;
#endif

//begin: add by panxu for delete all 2006-8-9
extern mmi_frm_sms_msgbox_enum msgbox_type;
//end: add by panxu for delete all 2006-8-9

#define ONE_DIGIT(value)						((value) % 10)

#define EMS_ESCAPE_CHARACTER	(27)

extern U8 GetSecondSemiOctetTarget(U8 ch);
extern U8 GetFirstSemiOctetTarget(U8 ch);
extern void EMS_ConvertGSM7BitDefaultEncodingToAsciiWithoutEMSlib(EMSData *data);

/*****************************************************************************
* FUNCTION
*  ComparePhoneNum
* DESCRIPTION
*   It Compares phone numbers
*
* PARAMETERS
*  Source and destination phone number
* RETURNS
*  status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL ComparePhoneNum(PU8 src,PU8 dst)
{
	// wangwb open @2007.06.16
	#if 1              
	long num1=0,num2=0;
	U8 temp1,temp2;
	
	num1=mmi_phb_util_convert_number_to_int(src);
	num2=mmi_phb_util_convert_number_to_int(dst);
	if(num1==num2)
	{
		if(src[0] == '+') src++;
		if(dst[0] == '+') dst++;
		temp1 = strcspn((PS8)src, "+pw*#");
		temp2 = strcspn((PS8)dst, "+pw*#");
		if(temp1 && temp2)
		{
			src = src + temp1;
			dst = dst + temp2;
 		}
 
		if(!strcmp((PS8)src, (PS8)dst))
			return TRUE;
		else 
			return FALSE;
	}
	else
		#endif
		// wangbei del end
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_convert_time_stamp
* DESCRIPTION
*  convert time stamp
*
* PARAMETERS
*  a  scts		scts
*  b  timestamp		timestamp
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_convert_time_stamp (U8* scts, U8* timestamp)
{
	/*year*/
	timestamp[0]= ONE_DIGIT(GetSecondSemiOctetTarget(scts[0]))*10
		+ ONE_DIGIT(GetFirstSemiOctetTarget(scts[0]));
	/*month*/
	timestamp[1]= ONE_DIGIT(GetSecondSemiOctetTarget(scts[1]))*10
		+ ONE_DIGIT(GetFirstSemiOctetTarget(scts[1]));
	/*day*/
	timestamp[2] = ONE_DIGIT(GetSecondSemiOctetTarget(scts[2]))*10
		+ ONE_DIGIT(GetFirstSemiOctetTarget(scts[2]));
		/*hour*/
	timestamp[3]= ONE_DIGIT(GetSecondSemiOctetTarget(scts[3]))*10
		+ ONE_DIGIT(GetFirstSemiOctetTarget(scts[3]));
	/*minute*/
	timestamp[4]= ONE_DIGIT(GetSecondSemiOctetTarget(scts[4]))*10
		+ ONE_DIGIT(GetFirstSemiOctetTarget(scts[4]));
	/*second*/
	timestamp[5]= ONE_DIGIT(GetSecondSemiOctetTarget(scts[5]))*10
		+ ONE_DIGIT(GetFirstSemiOctetTarget(scts[5]));
		/**timezone, convert date/time to one variable*/
	timestamp[6]= ONE_DIGIT(GetSecondSemiOctetTarget(scts[6]))*10
		+ ONE_DIGIT(GetFirstSemiOctetTarget(scts[6]));
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_new_sms
* DESCRIPTION
*  free new sms buff
*
* PARAMETERS
*  entry  IN		entry
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_add_new_sms (mmi_frm_sms_deliver_msg_struct* entry, U16 length, U8* data)
{
	mmi_frm_sms_concat_struct* concatinfo=(mmi_frm_sms_concat_struct*)&(entry->concat_info);
	U16 index=entry->index;
	U8 segment=1;
	if((concatinfo->seg>concatinfo->total_seg)||(concatinfo->seg==0)||(concatinfo->total_seg==0))
	{
		concatinfo->seg=1;
		concatinfo->total_seg=1;
	}

	if(((concatinfo->seg)%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)==0)
		segment=(U8)g_frm_sms_cntx.mmi_frm_sms_msg_seg_size;
	else
		segment=(concatinfo->seg)%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size;
	
	/*change seg info to start seg*/
	concatinfo->seg=((concatinfo->seg-1)/g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)*g_frm_sms_cntx.mmi_frm_sms_msg_seg_size+1;

	mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].dcs=entry->dcs;
	mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].display_type=entry->display_type;
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
	mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].storageType=entry->storageType;
#endif
	mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].fo=entry->fo;
	mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].index=entry->index;
	mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].mti=entry->mti;
	mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].pid=entry->pid;
	memcpy(&(mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].concat_info), concatinfo, sizeof(smsal_concat_struct));
	
	memcpy((S8*)&(mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].addr_number), 
			(S8*)&(entry->addr_number), sizeof(l4c_number_struct));
	memcpy((S8*)&(mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].sca_number), 
			(S8*)&(entry->sca_number), sizeof(l4c_number_struct));
	memcpy((S8*)(mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].scts), (S8*)(entry->scts), 7);
	
	mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].msg_data[0]=MMI_FRM_SMS_MSG_CONTENT_NUM;
	mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].no_msg_data=entry->no_msg_data;

	/*handle message content part*/	
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment=segment;
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].msglen=length;
	if(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data!=NULL)
	{
		OslMfree(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data);
		mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data = NULL;
	}
	if(length)
	{
		mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data=OslMalloc(length);
		memcpy(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data, (S8*)data, length);
	}
	
	/*class 0 message*/
	if(index==MMI_FRM_SMS_INVALID_INDEX)
		return;
			
	/* used index list entry, replace the old entry with the new one */
	if(mmi_frm_sms_index_list[index].segment != MMI_FRM_SMS_INVALID_NUM)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_add_new_sms remove used L4 list entry %d", index);
		mmi_frm_sms_remove_sms_entry( index);
	}

	mmi_frm_sms_index_list[index].segment=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment;
	mmi_frm_sms_index_list[index].nextindex=MMI_FRM_SMS_INVALID_INDEX;
	mmi_frm_sms_index_list[index].previndex=MMI_FRM_SMS_ACTION_SIZE;;
	mmi_frm_sms_index_list[index].prevtype=MMI_FRM_SMS_AWAITS;

}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_free_new_sms
* DESCRIPTION
*  free new sms buff
*
* PARAMETERS
*  entry  IN		entry
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_free_new_sms (void)
{
	if(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data!=NULL)
	{
		OslMfree(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data);
		mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data=NULL;
	}
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].msglen=0;
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].nextindex=MMI_FRM_SMS_INVALID_NUM;
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment=MMI_FRM_SMS_INVALID_NUM;
	memset((S8*)&mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE], MMI_FRM_SMS_INVALID_NUM, sizeof(mmi_frm_sms_deliver_msg_struct));
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_free_awaited
* DESCRIPTION
*  Free awaited message by index
*
* PARAMETERS
*  a  index		index
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_free_awaited(U8 index)
{
	U8 i=mmi_frm_sms_awaited[index].msg_data[0];
	while(i!=MMI_FRM_SMS_INVALID_NUM)
	{
		U8 nextindex=mmi_frm_sms_content[i].nextindex;
		if(mmi_frm_sms_content[i].data!=NULL)
		{
			OslMfree(mmi_frm_sms_content[i].data);
			mmi_frm_sms_content[i].data=NULL;
		}
		mmi_frm_sms_content[i].msglen=0;
		mmi_frm_sms_content[i].nextindex=MMI_FRM_SMS_INVALID_NUM;
		mmi_frm_sms_content[i].segment=MMI_FRM_SMS_INVALID_NUM;
		i=nextindex;
	}
	memset((S8*)&mmi_frm_sms_awaited[index], MMI_FRM_SMS_INVALID_NUM, sizeof(mmi_frm_sms_deliver_msg_struct));
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_new_sms
* DESCRIPTION
*  free new sms buff
*
* PARAMETERS
*  entry  IN		entry
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_add_new_data (mmi_frm_sms_deliver_data_struct* entry, U16 length, U8* data)
{
	mmi_frm_sms_concat_struct* concatinfo=(mmi_frm_sms_concat_struct*)&(entry->concat_info);
	U8 segment=1;
	if((concatinfo->seg>concatinfo->total_seg)||(concatinfo->seg==0)||(concatinfo->total_seg==0))
	{
		concatinfo->seg=1;
		concatinfo->total_seg=1;
	}

	if(((concatinfo->seg)%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)==0)
		segment=(U8)g_frm_sms_cntx.mmi_frm_sms_msg_seg_size;
	else
		segment=(concatinfo->seg)%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size;
	
	/*change seg info to start seg*/
	concatinfo->seg=((concatinfo->seg-1)/g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)*g_frm_sms_cntx.mmi_frm_sms_msg_seg_size+1;

	mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].dest_mod_id=entry->dest_mod_id;
	mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].dest_port=entry->dest_port;
	mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].dcs=entry->dcs;
	mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].mti=entry->mti;
	memcpy(&(mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].concat_info), concatinfo, sizeof(smsal_concat_struct));
	
	memcpy((S8*)&(mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].oa), 
			(S8*)&(entry->oa), sizeof(l4c_number_struct));
	memcpy((S8*)(mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].scts), (S8*)(entry->scts), 7);
	
	
	mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].data[0]=MMI_FRM_SMS_MSG_CONTENT_NUM;
	mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].message_len=entry->message_len;

	/*handle message content part*/	
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment=segment;
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].msglen=length;
	if(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data!=NULL)
	{
		OslMfree(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data);
		mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data = NULL;
	}
	if(length)
	{
		mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data=OslMalloc(length);
		memcpy(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data, (S8*)data, length);
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_free_new_sms
* DESCRIPTION
*  free new sms buff
*
* PARAMETERS
*  entry  IN		entry
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_free_new_data (void)
{
	if(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data!=NULL)
	{
		OslMfree(mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data);
		mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data=NULL;
	}
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].msglen=0;
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].nextindex=MMI_FRM_SMS_INVALID_NUM;
	mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment=MMI_FRM_SMS_INVALID_NUM;
	memset((S8*)&mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE], MMI_FRM_SMS_INVALID_NUM, sizeof(mmi_frm_sms_deliver_data_struct));
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_free_data
* DESCRIPTION
*  Free SMS with port number by index
*
* PARAMETERS
*  a  index		index
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_free_data(U8 index)
{
	U8 i=mmi_frm_sms_data[index].data[0];
	while(i!=MMI_FRM_SMS_INVALID_NUM)
	{
		U8 nextindex=mmi_frm_sms_content[i].nextindex;
		if(mmi_frm_sms_content[i].data!=NULL)
		{
			OslMfree(mmi_frm_sms_content[i].data);
			mmi_frm_sms_content[i].data=NULL;
		}
		mmi_frm_sms_content[i].msglen=0;
		mmi_frm_sms_content[i].nextindex=MMI_FRM_SMS_INVALID_NUM;
		mmi_frm_sms_content[i].segment=MMI_FRM_SMS_INVALID_NUM;
		i=nextindex;
	}
	memset((S8*)&mmi_frm_sms_data[index], MMI_FRM_SMS_INVALID_NUM, sizeof(mmi_frm_sms_deliver_data_struct));
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_convert_new_data
* DESCRIPTION
*  convert mt structure to entry structure
*
* PARAMETERS
*  a  data		new data
*  b  entry	message entry
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_convert_new_data (MMI_FRM_SMS_APP_DATA_IND_STRUCT* data, mmi_frm_sms_deliver_data_struct* entry)
{
	entry->dest_mod_id=data->dest_mod_id;
	entry->dest_port=data->dest_port;
	entry->src_port=data->src_port;
	entry->dcs=data->dcs;
	entry->mti=data->mti;
	entry->message_len=data->message_len;
	entry->concat_info=data->concat_info[0];
	memcpy(&entry->oa, &data->oa, sizeof(l4_addr_bcd_struct));
	memcpy(entry->scts, data->scts, 7);
	entry->data[0]=MMI_FRM_SMS_MSG_CONTENT_NUM;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_convert_new_sms
* DESCRIPTION
*  convert mt structure to entry structure
*
* PARAMETERS
*  a  index		index
*  b  type		type
*  c  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_convert_new_sms ( MMI_FRM_SMS_DELIVER_MSG_IND_STRUCT* data, mmi_frm_sms_deliver_msg_struct* entry)
{
	entry->fo=data->fo;
	memcpy(entry->scts, data->scts, 7);
	memcpy(&entry->sca_number, &data->sca_number, sizeof(l4c_number_struct));
	memcpy(&entry->addr_number, &data->oa_number, sizeof(l4c_number_struct));
	entry->pid=data->pid;
	entry->dcs=data->dcs;
	entry->index=data->index; //for awaited message, this is the start index
	entry->mti=data->mti;
	entry->display_type=data->display_type;
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
	entry->storageType = data->storage_type;
#endif
	entry->concat_info=data->concat_info;
	entry->no_msg_data=data->no_msg_data;
	entry->msg_data[0]=MMI_FRM_SMS_MSG_CONTENT_NUM;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_convert_mt_to_entry
* DESCRIPTION
*  convert mt structure to entry structure
*
* PARAMETERS
*  a  index		index
*  b  type		type
*  c  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_convert_mt_to_entry (mmi_frm_sms_deliver_msg_struct* data, mmi_frm_sms_msgbox_struct* entry)
{
	U8 type;
	U8 numbertype = (data->addr_number.type==CSMCC_INTERNATIONAL_ADDR)?1:0;
	mmi_frm_sms_concat_struct* concat=(mmi_frm_sms_concat_struct*)&(data->concat_info);
	memset((S8*)entry->number, 0, MAX_DIGITS_SMS);
	if(numbertype)
		entry->number[0]='+';
	memcpy(entry->number+numbertype, data->addr_number.number, data->addr_number.length);
	type=MMI_FRM_SMS_UNREAD;
	entry->msgtype=(data->mti<<4)|type;
	entry->startseg=((concat->seg-1)/g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)*g_frm_sms_cntx.mmi_frm_sms_msg_seg_size+1;
	entry->totalseg=concat->total_seg;
	entry->startindex=data->index;
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
	entry->storageType = data->storageType;
#endif

	entry->ref=concat->ref;
	mmi_frm_sms_convert_time_stamp(data->scts, entry->timestamp);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_convert_mo_to_entry
* DESCRIPTION
*  convert mo structure to entry structure
*
* PARAMETERS
*  a  index		index
*  b  type		type
*  c  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
void mmi_frm_sms_convert_mo_to_entry (mmi_frm_sms_send_struct* data, mmi_frm_sms_msgbox_struct* entry, 
															U16 index, U8 status, U8 totalsegment, U16 reference,U8 storagetype)
#else
void mmi_frm_sms_convert_mo_to_entry (mmi_frm_sms_send_struct* data, mmi_frm_sms_msgbox_struct* entry, 
															U16 index, U8 status, U8 totalsegment, U16 reference)
#endif
{
	U8 type=0;
	switch(status)
	{
		case SMSAL_REC_UNREAD:
			type=MMI_FRM_SMS_UNREAD;
			break;
		case SMSAL_REC_READ:
			type=MMI_FRM_SMS_INBOX;
			break;
		case SMSAL_STO_SENT:
			type=MMI_FRM_SMS_OUTBOX;
			break;
		case SMSAL_STO_UNSENT:
			type=MMI_FRM_SMS_DRAFTS;
			break;
	}
	entry->msgtype=(SMSAL_MTI_SUBMIT<<4)|type;
	entry->startindex=index;

	entry->startseg=1;
	entry->totalseg=totalsegment;

	memset((S8*)entry->timestamp, 0, 7);
	memset((S8*)entry->number, 0, 21);
	memcpy((S8*)entry->number, (S8*)data->number, strlen((S8*)data->number));

	entry->ref=reference;
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
	entry->storageType = storagetype;
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_convert_read_to_entry
* DESCRIPTION
*  convert read structure to entry structure
*
* PARAMETERS
*  a  index		index
*  b  type		type
*  c  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_convert_read_to_entry (MMI_FRM_SMS_GET_MSG_RSP_STRUCT* data, mmi_frm_sms_msgbox_struct* entry, U16 index)
{
	U8 type;
	mmi_frm_sms_concat_struct concat; 
	concat.seg = 1;
	concat.total_seg = 1;
	concat.ref= 0;

	
	
	switch(data->status)
	{
		case SMSAL_REC_UNREAD:
			type=MMI_FRM_SMS_UNREAD;
			break;
		case SMSAL_REC_READ:
			type=MMI_FRM_SMS_INBOX;
			break;
		case SMSAL_STO_SENT:
			type=MMI_FRM_SMS_OUTBOX;
			break;
		case SMSAL_STO_UNSENT:
			type=MMI_FRM_SMS_DRAFTS;
			break;
		default:
			type=MMI_FRM_SMS_NOBOX;
			break;
	}
	entry->msgtype=(data->mti<<4)|type;

	entry->startseg=((concat.seg-1)/g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)*g_frm_sms_cntx.mmi_frm_sms_msg_seg_size+1;
	entry->totalseg=concat.total_seg;

	memset((S8*)entry->number, 0, MAX_DIGITS_SMS);
	type=(data->addr_number.type==CSMCC_INTERNATIONAL_ADDR)?1:0;
	if(type)
		entry->number[0] = '+';
	memcpy((S8*)&(entry->number[type]), (S8*)data->addr_number.number, data->addr_number.length);

	entry->ref=concat.ref;
	entry->startindex=index;
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
	entry->storageType = data->storage_type;
#endif
	mmi_frm_sms_convert_time_stamp(data->scts, entry->timestamp);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_convert_startup_read_to_entry
* DESCRIPTION
*  convert startup read structure to entry structure
*
* PARAMETERS
*  a  index		index
*  b  type		type
*  c  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_convert_startup_read_to_entry (MMI_FRM_SMS_STARTUP_READ_MSG_IND_STRUCT* data, mmi_frm_sms_msgbox_struct* entry)
{
	U8 type;
	mmi_frm_sms_concat_struct concat; 
	concat.seg = (data->concat_info & 0xff000000)>>24;
	concat.total_seg = (data->concat_info & 0x00ff0000)>>16;
	concat.ref= (data->concat_info & 0x0000ffff);

	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_convert_startup_read_to_entry, seg =%d, total_seg = %d, ref = 0x%x",concat.seg, concat.total_seg, concat.ref);
	switch(data->status)
	{
		case SMSAL_REC_UNREAD:
			type=MMI_FRM_SMS_UNREAD;
			break;
		case SMSAL_REC_READ:
			type=MMI_FRM_SMS_INBOX;
			break;
		case SMSAL_STO_SENT:
			type=MMI_FRM_SMS_OUTBOX;
			break;
		case SMSAL_STO_UNSENT:
			type=MMI_FRM_SMS_DRAFTS;
			break;
		default:
			type=MMI_FRM_SMS_NOBOX;
			break;
	}
	entry->msgtype=(data->mti<<4)|type;

	entry->startseg=((concat.seg-1)/g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)*g_frm_sms_cntx.mmi_frm_sms_msg_seg_size+1;
	entry->totalseg=concat.total_seg;

	memset((S8*)entry->number, 0, MAX_DIGITS_SMS);
	type=(data->addr_number.type==CSMCC_INTERNATIONAL_ADDR)?1:0;
	if(type)
		entry->number[0] = '+';
	memcpy((S8*)&(entry->number[type]), (S8*)data->addr_number.number, data->addr_number.length);

	entry->ref=concat.ref;
	entry->startindex=data->index;
	mmi_frm_sms_convert_time_stamp(data->scts, entry->timestamp);
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
	entry->storageType = data->storage_type;
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_compare_entry
* DESCRIPTION
*  convert mo structure to entry structure
*
* PARAMETERS
*  a  index		index
*  b  type		type
*  c  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
U8 mmi_frm_sms_compare_entry (U16 index, mmi_frm_sms_msgbox_struct* data, U8 thissegment)
{
	/* concatenate class0 message will have a MMI_FRM_SMS_INVALID_INDEX startindex */
	if ((data->startindex != MMI_FRM_SMS_INVALID_INDEX) && (mmi_frm_sms_msg_box[index].startindex == data->startindex))
		return TRUE;
	if((mmi_frm_sms_msg_box[index].msgtype & 0xf0)==(data->msgtype & 0xf0))
		if(mmi_frm_sms_msg_box[index].ref==data->ref)
			if(mmi_frm_sms_msg_box[index].totalseg==data->totalseg)
				if(mmi_frm_sms_msg_box[index].startseg==data->startseg)
					if(ComparePhoneNum((U8*)data->number, (U8*)mmi_frm_sms_msg_box[index].number)==TRUE)
					{
						U16 i=mmi_frm_sms_msg_box[index].startindex;
						U16 previndex=MMI_FRM_SMS_INVALID_INDEX;
						while(i!=MMI_FRM_SMS_INVALID_INDEX)
						{
							if(mmi_frm_sms_index_list[i].segment==thissegment)
								return FALSE;
							else if(mmi_frm_sms_index_list[i].segment>thissegment)
							{
								return TRUE;
							}
							previndex=i;
							i=mmi_frm_sms_index_list[i].nextindex;
						}
						if((previndex!=MMI_FRM_SMS_INVALID_INDEX)&&(mmi_frm_sms_index_list[previndex].segment<thissegment))
							return TRUE;
					}
					return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_sms_to_list
* DESCRIPTION
*  mmi_frm_sms_add_sms_to_list
*
* PARAMETERS
*  a  index		index
*  b  type		type
*  c  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
#define NEW_MESSAGE_AT_FRONT_IN_OUTBOX 1
void mmi_frm_sms_add_sms_to_list (mmi_frm_sms_msgbox_struct* data, U16 index)
{
	U8 type=MMI_FRM_SMS_NOBOX;
	if((data->msgtype&MMI_FRM_SMS_UNREAD)==MMI_FRM_SMS_UNREAD)
		type=MMI_FRM_SMS_UNREAD;
	else if((data->msgtype&MMI_FRM_SMS_INBOX)==MMI_FRM_SMS_INBOX)
		type=MMI_FRM_SMS_INBOX;
	else if((data->msgtype&MMI_FRM_SMS_OUTBOX)==MMI_FRM_SMS_OUTBOX)
		type=MMI_FRM_SMS_OUTBOX;
	else if((data->msgtype&MMI_FRM_SMS_DRAFTS)==MMI_FRM_SMS_DRAFTS)
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
		type=MMI_FRM_SMS_DRAFTS;
	#else
		type=MMI_FRM_SMS_OUTBOX;
	#endif

	switch(type)
	{
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
		{
			U16 i = 0;
			U16 j;
			while(i < mmi_frm_sms_inbox_size)
			{
				if(mmi_frm_sms_inbox_list[i]==index)
					return;
				if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[0] < data->timestamp[0])
					break;
				else if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[0] > data->timestamp[0])
				{
					i++;
					continue;
				}
				if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[1] < data->timestamp[1])
					break;
				else if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[1] > data->timestamp[1])
				{
					i++;
					continue;
				}
				if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[2] < data->timestamp[2])
					break;
				else if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[2] > data->timestamp[2])
				{
					i++;
					continue;
				}
				if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[3] < data->timestamp[3])
					break;
				else if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[3] > data->timestamp[3])
				{
					i++;
					continue;
				}
				if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[4] < data->timestamp[4])
					break;
				else if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[4] > data->timestamp[4])
				{
					i++;
					continue;
				}
				if(mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[i]].timestamp[5] < data->timestamp[5])
					break;
				i++;
				continue;
			}
			if(mmi_frm_sms_inbox_size>0)
			{
				for(j = mmi_frm_sms_inbox_size; j > i; j--)
					mmi_frm_sms_inbox_list[j] = mmi_frm_sms_inbox_list[j-1];
			}
			mmi_frm_sms_inbox_list[i] = index;
			mmi_frm_sms_inbox_size++;
		}
		break;

		case MMI_FRM_SMS_OUTBOX:
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
	#endif
		{
			U16 i = 0;
			for(i=0; i<mmi_frm_sms_outbox_size; i++)
			{
				if(mmi_frm_sms_outbox_list[i]==index)
					return;
			}
			#ifdef NEW_MESSAGE_AT_FRONT_IN_OUTBOX
			for(i = mmi_frm_sms_outbox_size; i > 0; i--)
			{
				mmi_frm_sms_outbox_list[i] = mmi_frm_sms_outbox_list[i - 1];
			}
			mmi_frm_sms_outbox_list[0] = index;
			mmi_frm_sms_outbox_size++;
			#else
			mmi_frm_sms_outbox_list[mmi_frm_sms_outbox_size] = index;
			mmi_frm_sms_outbox_size++;
			#endif
		}
		break;

	#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
		{
			U16 i = 0;
			for(i=0; i<mmi_frm_sms_drafts_size; i++)
			{
				if(mmi_frm_sms_drafts_list[i]==index)
					return;
			}
			mmi_frm_sms_drafts_list[mmi_frm_sms_drafts_size] = index;
			mmi_frm_sms_drafts_size++;
		}
		break;
	#endif
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_remove_sms_from_list
* DESCRIPTION
*  mmi_frm_sms_remove_sms_from_list
*
* PARAMETERS
*  a  index		index
*  b  type		type
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_inbox_list
*****************************************************************************/
void mmi_frm_sms_remove_sms_from_list (U16 index, U8 type)
{
	U16 i = 0;
	U16 j = 0;
	switch(type)
	{
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			while(i<mmi_frm_sms_inbox_size)
			{
				if(mmi_frm_sms_inbox_list[i]==index)
					break;
				i++;
			}
			for(j=i; j<(mmi_frm_sms_inbox_size-1); j++)
				mmi_frm_sms_inbox_list[j] = mmi_frm_sms_inbox_list[j+1];
			mmi_frm_sms_inbox_list[j] = MMI_FRM_SMS_INVALID_INDEX;
			mmi_frm_sms_inbox_size--;
		break;

		case MMI_FRM_SMS_OUTBOX:
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
	#endif
			while(i<mmi_frm_sms_outbox_size)
			{
				if(mmi_frm_sms_outbox_list[i]==index)
					break;		
				i++;
			}
			for(j=i; j<(mmi_frm_sms_outbox_size-1); j++)
				mmi_frm_sms_outbox_list[j] = mmi_frm_sms_outbox_list[j+1];
			mmi_frm_sms_outbox_list[j] = MMI_FRM_SMS_INVALID_INDEX;
			mmi_frm_sms_outbox_size--;
		break;

	#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			while(i<mmi_frm_sms_drafts_size)
			{
				if(mmi_frm_sms_drafts_list[i]==index)
					break;		
				i++;
			}
			for(j=i; j<(mmi_frm_sms_drafts_size-1); j++)
				mmi_frm_sms_drafts_list[j] = mmi_frm_sms_drafts_list[j+1];
			mmi_frm_sms_drafts_list[j] = MMI_FRM_SMS_INVALID_INDEX;
			mmi_frm_sms_drafts_size--;
		break;
	#endif
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_sms_segment
* DESCRIPTION
*  add one segment to sms entry
*
* PARAMETERS
*  a  index			index
*  b  addindex		addindex
*  c  segment		segment
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_index_list
*****************************************************************************/
void mmi_frm_sms_add_sms_segment (U16 index, U16 addindex, U8 segment)
{
	U16 thisindex = mmi_frm_sms_msg_box[index].startindex;
	U8 islast=TRUE;

	mmi_frm_sms_index_list[addindex].segment = segment;

	//mmi_frm_sms_index_list[addindex].index_cout = addindex;
	
	/* thisindex is the current smallest segment index of this message */
	if(mmi_frm_sms_index_list[thisindex].segment > segment)
	{
		islast=FALSE;
		/* addindex is inserted in the head, now addindex is the current smallest segment index of this message */
		mmi_frm_sms_msg_box[index].startindex = addindex;
	}
	else
	{
		while(mmi_frm_sms_index_list[thisindex].nextindex != MMI_FRM_SMS_INVALID_INDEX)
		{
			thisindex = mmi_frm_sms_index_list[thisindex].nextindex;
			if(mmi_frm_sms_index_list[thisindex].segment > segment)
			{
				islast=FALSE;
				break;
			}
		}
	}
	
	if(islast==TRUE)
	{
		/* addindex is inserted in the tail, now addindex is the current largest segment index of this message */
		mmi_frm_sms_index_list[thisindex].nextindex = addindex;
		mmi_frm_sms_index_list[addindex].nextindex = MMI_FRM_SMS_INVALID_INDEX;
		mmi_frm_sms_index_list[addindex].previndex = thisindex;
		mmi_frm_sms_index_list[addindex].prevtype = MMI_FRM_SMS_NOBOX;
	}
	else
	{
		/* addindex is inserted in the middle */
		mmi_frm_sms_index_list[addindex].nextindex = thisindex;
		mmi_frm_sms_index_list[addindex].previndex=mmi_frm_sms_index_list[thisindex].previndex;
		mmi_frm_sms_index_list[addindex].prevtype=mmi_frm_sms_index_list[thisindex].prevtype;
		if(mmi_frm_sms_index_list[thisindex].prevtype==MMI_FRM_SMS_NOBOX)
			mmi_frm_sms_index_list[mmi_frm_sms_index_list[thisindex].previndex].nextindex=addindex;
		mmi_frm_sms_index_list[thisindex].previndex=addindex;
		mmi_frm_sms_index_list[thisindex].prevtype=MMI_FRM_SMS_NOBOX;
	}	
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_sms_entry
* DESCRIPTION
*  add sms entry: 
*
* PARAMETERS
*  a  index			index
*  b  addindex		addindex
*  c  segment		segment
* RETURNS
*  index
* GLOBALS AFFECTED
*  mmi_frm_sms_index_list
*****************************************************************************/
U16 mmi_frm_sms_add_sms_entry (mmi_frm_sms_msgbox_struct* data, U16 L4index, U8 thissegment)
{
	U16 addindex = g_frm_sms_cntx.mmi_frm_sms_msg_box_size;
	U16 emptyindex=g_frm_sms_cntx.mmi_frm_sms_msg_box_size;
	U8 found=FALSE;
	while((found==FALSE)&&(addindex>0))
	{
		addindex--;
		if(mmi_frm_sms_msg_box[addindex].startindex==MMI_FRM_SMS_INVALID_INDEX)
		{
			emptyindex=addindex;
			continue;
		}
		if(mmi_frm_sms_compare_entry(addindex, data, thissegment)==TRUE)
		{
			found=TRUE;
			break;
		}
	}
	if(found==FALSE)
	{
		U8 type;
		if((data->msgtype&MMI_FRM_SMS_INBOX)==MMI_FRM_SMS_INBOX)
			type=MMI_FRM_SMS_INBOX;
		else if((data->msgtype&MMI_FRM_SMS_UNREAD)==MMI_FRM_SMS_UNREAD)
			type=MMI_FRM_SMS_UNREAD;
		else if((data->msgtype&MMI_FRM_SMS_OUTBOX)==MMI_FRM_SMS_OUTBOX)
			type=MMI_FRM_SMS_OUTBOX;
		else
			type=MMI_FRM_SMS_DRAFTS;
		memcpy((S8*)&mmi_frm_sms_msg_box[emptyindex], (S8*)data, sizeof(mmi_frm_sms_msgbox_struct));
		mmi_frm_sms_index_list[L4index].segment=thissegment;
//		mmi_frm_sms_index_list[L4index].nextindex=MMI_FRM_SMS_INVALID_INDEX;
		mmi_frm_sms_index_list[L4index].previndex=emptyindex;
		mmi_frm_sms_index_list[L4index].prevtype=type;
		
		//mmi_frm_sms_index_list[L4index].index_cout=L4index;//wangwb add @2007.06.16
		
		mmi_frm_sms_msgbox_size++;		
		addindex=emptyindex;
	}
	else
	{		
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
		if(mmi_frm_sms_msg_box[addindex].storageType != MMI_FRM_SMS_PREFER_STORAGE)
		{
			mmi_frm_sms_msg_box[addindex].storageType = data->storageType;
		}
#endif
		mmi_frm_sms_add_sms_segment(addindex, L4index, thissegment);
	}
	return addindex;
}



/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_remove_sms_entry
* DESCRIPTION
*  remove sms entry: 
*
* PARAMETERS
*  a  index			index
*  b  addindex		addindex
*  c  segment		segment
* RETURNS
*  index
* GLOBALS AFFECTED
*  mmi_frm_sms_index_list
*****************************************************************************/
void mmi_frm_sms_remove_sms_entry (U16 index)
{
	U16 previndex, nextindex;
	U8 type = mmi_frm_sms_index_list[index].prevtype;
	previndex = mmi_frm_sms_index_list[index].previndex;
	nextindex = mmi_frm_sms_index_list[index].nextindex;
        /* chat message is not in message box, so message box should remain unchanged and only delete index list entry */
        /* For message with only one segment, also update msg box and inbox/outbox/draft list */
        if ((type!=MMI_FRM_SMS_NOBOX && type!=MMI_FRM_SMS_AWAITS && type!=MMI_FRM_SMS_DATA) && (nextindex==MMI_FRM_SMS_INVALID_INDEX))
	{
		mmi_frm_sms_remove_sms_from_list(previndex, type);
		memset((S8*)&mmi_frm_sms_msg_box[previndex], 0xff, sizeof(mmi_frm_sms_msgbox_struct));
	}
	else
	{
		/*the first segment of message*/
		if(type!=MMI_FRM_SMS_NOBOX)
		{
			/*for message in awaited list, ex. chat msg,  modify start index of awaitd list entry to next index*/
			if ( type==MMI_FRM_SMS_AWAITS ||  type ==MMI_FRM_SMS_DATA)
				mmi_frm_sms_awaited[previndex].index = nextindex;

			/*for message in msg box, ex. inbox/outbox/draft  message, modify start index of msg box entry to next index  */
			else
			mmi_frm_sms_msg_box[previndex].startindex=nextindex;
		} 

		/* the 2nd or after segment of message*/
		else
			mmi_frm_sms_index_list[previndex].nextindex=nextindex;			
		
		if(nextindex!=MMI_FRM_SMS_INVALID_INDEX)
		{
			mmi_frm_sms_index_list[nextindex].prevtype=type;
			mmi_frm_sms_index_list[nextindex].previndex=previndex;
		}
	}
	memset((S8*)&mmi_frm_sms_index_list[index], 0xff, sizeof(mmi_frm_sms_index_struct));		
}



/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_sms_to_msgbox
* DESCRIPTION
*  add sms to msgbox
*
* PARAMETERS
*  a  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U16 mmi_frm_sms_add_sms_to_msgbox (mmi_frm_sms_msgbox_struct* entry, U16 index, U8 thisseg)
{
	U16 entryindex;

	/*add to msg entry*/
	entryindex = mmi_frm_sms_add_sms_entry(entry, index, thisseg);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_add_sms_to_msgbox");
	/*update msg list*/
	if(mmi_frm_sms_list_done)
		mmi_frm_sms_add_sms_to_list(entry, entryindex);
	return entryindex;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_index
* DESCRIPTION
*  get sms index from bitmap
*
* PARAMETERS
*  a  index		index
*  b  index_bitmap	index_bitmap
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U16 mmi_frm_sms_get_index (U16 index, U8 *index_bitmap)
{
	U16  start_byte_idx, i;
	U8   start_bit_idx,  j, mapping_byte;
	start_byte_idx = (index >> 3 );  /* divide 8 */
	start_bit_idx  = (index & 0x7);  /* reminder */
	for(i = start_byte_idx ; i < ((g_frm_sms_cntx.mmi_frm_sms_msg_box_size>>3) +1) ; i++ )
	{
		mapping_byte = index_bitmap[i] ;
		for(j = start_bit_idx ; j < 8 ; j++ )
		{
			if(((mapping_byte >> j) & 0x01)== 0x01)
				return (i*8 + j);
		}
		start_bit_idx = 0 ;
	}
	return MMI_FRM_SMS_INVALID_INDEX;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_bitmap
* DESCRIPTION
*  set bitmap from index
*
* PARAMETERS
*  a  index		index
*  b  index_bitmap	index_bitmap
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_set_bitmap (U16 index, U8* index_bitmap)
{
	U16  byte_index;
	U8   bit_index;
	byte_index = (index >> 3);  /* divide 8 */
	bit_index  = (index & 0x7);  /* reminder */
	index_bitmap[byte_index] |=  (0x01 << bit_index);

}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_remove_sms_from_msgbox
* DESCRIPTION
*  mmi_frm_sms_remove_sms_from_msgbox
*
* PARAMETERS
*  a  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_remove_sms_from_msgbox (U8* data)
{
	U8* index_bitmap = (U8*)data;
	U16 index=mmi_frm_sms_get_index(0, index_bitmap);
	while(index!=MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_frm_sms_remove_sms_entry(index);
		index++;
		index = mmi_frm_sms_get_index(index, index_bitmap);
	}
	return;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_create_sms_list
* DESCRIPTION
*  mmi_frm_sms_create_sms_list
*
* PARAMETERS
*  a  data		data
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
#ifdef __MMI_MULTI_SIM__
extern MMI_BOOL g_mmi_frm_sms_list_finish[];
#endif
void mmi_frm_sms_create_sms_list (void)
{
	U16 i;
	
	#ifdef __MMI_MULTI_SIM__
	BOOL bAllFinish= MMI_TRUE;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!g_mmi_frm_sms_list_finish[i])
		{
			bAllFinish = MMI_FALSE;
			break;
		}
	}
	if(bAllFinish)
	#endif
	{
		for(i = 0; i < g_frm_sms_cntx.mmi_frm_sms_msg_box_size; i++)
		{
			if(mmi_frm_sms_msg_box[i].startindex == MMI_FRM_SMS_INVALID_INDEX)
				continue;
			mmi_frm_sms_add_sms_to_list(&mmi_frm_sms_msg_box[i], i);
		}
		mmi_frm_sms_list_done=TRUE;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_list_index
* DESCRIPTION
*  map message box index to message list index
*
* PARAMETERS
*  a  type			type
*  b  index			index
*  c  msgbox_index		msgbox_index
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_get_sms_list_index (U8* type, U16* index, U16 msgbox_index)
{
	U16 i=0;
	while(i<mmi_frm_sms_inbox_size)
	{
		if(mmi_frm_sms_inbox_list[i]==msgbox_index)
		{
			*type = MMI_FRM_SMS_INBOX;
			*index = i;
			return;
		}
		i++;
	}
	i=0;
	while(i<mmi_frm_sms_outbox_size)
	{
		if(mmi_frm_sms_outbox_list[i]==msgbox_index)
		{
			*type = MMI_FRM_SMS_OUTBOX;
			*index = i;
			return;
		}
		i++;
	}
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	i=0;
	while(i<mmi_frm_sms_drafts_size)
	{
		if(mmi_frm_sms_drafts_list[i]==msgbox_index)
		{
			*type = MMI_FRM_SMS_DRAFTS;
			*index = i;
			return;
		}
		i++;
	}
	#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_index
* DESCRIPTION
*  get sms index array, in sequence
*
* PARAMETERS
*  a  type			type
*  b  index			index
*  c  l4_index		l4_index
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_get_sms_index (mmi_frm_sms_msgbox_enum type, U16 index, U16* l4_index)
{
	U16 i = 0;
	U16 L4index=0;
	for(i=0; i<g_frm_sms_cntx.mmi_frm_sms_msg_seg_size; i++)
		l4_index[i]=MMI_FRM_SMS_INVALID_INDEX;
	switch(type)
	{
		case MMI_FRM_SMS_NOBOX:
			{
				L4index = mmi_frm_sms_msg_box[index].startindex;
				while(L4index != MMI_FRM_SMS_INVALID_INDEX)
				{
					l4_index[mmi_frm_sms_index_list[L4index].segment-1]=L4index;
					L4index = mmi_frm_sms_index_list[L4index].nextindex;
				}
			}
		break;
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			{
				L4index = mmi_frm_sms_msg_box[mmi_frm_sms_inbox_list[index]].startindex;
				while(L4index != MMI_FRM_SMS_INVALID_INDEX)
				{
					l4_index[mmi_frm_sms_index_list[L4index].segment-1]=L4index;
					L4index = mmi_frm_sms_index_list[L4index].nextindex;
				}
			}
		break;
		case MMI_FRM_SMS_OUTBOX:
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
	#endif
			{
				L4index = mmi_frm_sms_msg_box[mmi_frm_sms_outbox_list[index]].startindex;
				while(L4index != MMI_FRM_SMS_INVALID_INDEX)
				{
					l4_index[mmi_frm_sms_index_list[L4index].segment-1]=L4index;
					L4index = mmi_frm_sms_index_list[L4index].nextindex;
				}
			}
		break;
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			{
				L4index = mmi_frm_sms_msg_box[mmi_frm_sms_drafts_list[index]].startindex;
				while(L4index != MMI_FRM_SMS_INVALID_INDEX)
				{
					l4_index[mmi_frm_sms_index_list[L4index].segment-1]=L4index;
					L4index = mmi_frm_sms_index_list[L4index].nextindex;
				}
			}
		break;
	#endif
		case MMI_FRM_SMS_AWAITS:
		case MMI_FRM_SMS_DATA:
			break;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_msgbox_index
* DESCRIPTION
 
*
* PARAMETERS
 
*  b  msgbox_index		msgbox_index
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_get_sms_msgbox_index (U16 l4_index, U16* msgbox_index)
{
	U16 L4index = l4_index;
	*msgbox_index = MMI_FRM_SMS_INVALID_INDEX;

	while(mmi_frm_sms_index_list[L4index].prevtype == MMI_FRM_SMS_NOBOX)
		L4index = mmi_frm_sms_index_list[L4index].previndex;

	if(mmi_frm_sms_index_list[L4index].prevtype != MMI_FRM_SMS_AWAITS && 
				mmi_frm_sms_index_list[L4index].prevtype != MMI_FRM_SMS_DATA)
		*msgbox_index = mmi_frm_sms_index_list[L4index].previndex;
	
	return;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_bitmap
* DESCRIPTION
*  get sms index in bitmap
*
* PARAMETERS
*  a  type			type
*  b  index			index
*  c  l4_index		l4_index
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_get_sms_bitmap (mmi_frm_sms_msgbox_enum type, U16 index, U8* l4_index)
{
	U16 i = 0;
	U16 startindex = index;
	U16 endindex=index+1;
	U16 entryindex=0;
	U16 L4index=0;

	//begin:add by panxu for delete all 2006-8-9
	if(index != MMI_FRM_SMS_INVALID_INDEX) //delete one message
	{
		msgbox_type = 0x30; //default type for delete one message
	}
	else
	{
		msgbox_type = type;
	}
	//end: add by panxu for delete all 2006-8-9
	
	if(mmi_frm_sms_list_done==FALSE)
		return FALSE;
	
	if(type & MMI_FRM_SMS_NOBOX)
		mmi_frm_sms_set_bitmap(index, l4_index);

	
	if(type & MMI_FRM_SMS_AWAITS)
	{
		for(i=startindex; i<endindex; i++)
		{
			entryindex = index;
			L4index = entryindex;
			while(L4index != MMI_FRM_SMS_INVALID_INDEX)
			{
				mmi_frm_sms_set_bitmap(L4index, l4_index);
				L4index = mmi_frm_sms_index_list[L4index].nextindex;
			}
		}
	}
	
	if((type & MMI_FRM_SMS_INBOX)||(type & MMI_FRM_SMS_UNREAD))
	{
		if(index==MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex=0;
			endindex=mmi_frm_sms_inbox_size;
		}
		for(i=startindex; i<endindex; i++)
		{
			entryindex = mmi_frm_sms_inbox_list[i];
			L4index = mmi_frm_sms_msg_box[entryindex].startindex;
			
			mmi_trace(g_sw_SMS, "HHHHHHHHHHHHHHHHH      L4index = %d",L4index);
			
			while(L4index != MMI_FRM_SMS_INVALID_INDEX)
			{
				mmi_frm_sms_set_bitmap(L4index, l4_index);
				L4index = mmi_frm_sms_index_list[L4index].nextindex;
			}
		}
	}
	if((type & MMI_FRM_SMS_OUTBOX)
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		||(type & MMI_FRM_SMS_DRAFTS)
	#endif
		)
	{
		if(index==MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex=0;
			endindex=mmi_frm_sms_outbox_size;
		}
		for(i=startindex; i<endindex; i++)
		{
			entryindex = mmi_frm_sms_outbox_list[i];
			L4index = mmi_frm_sms_msg_box[entryindex].startindex;
			while(L4index != MMI_FRM_SMS_INVALID_INDEX)
			{
				mmi_frm_sms_set_bitmap(L4index, l4_index);
				L4index = mmi_frm_sms_index_list[L4index].nextindex;
			}
		}
	}
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	if((type & MMI_FRM_SMS_DRAFTS))
	{
		if(index==MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex=0;
			endindex=mmi_frm_sms_drafts_size;
		}
		for(i=startindex; i<endindex; i++)
		{
			entryindex = mmi_frm_sms_drafts_list[i];
			L4index = mmi_frm_sms_msg_box[entryindex].startindex;
			while(L4index != MMI_FRM_SMS_INVALID_INDEX)
			{
				mmi_frm_sms_set_bitmap(L4index, l4_index);
				L4index = mmi_frm_sms_index_list[L4index].nextindex;
			}
		}
	}
	#endif
	return TRUE;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_awaited_segment
* DESCRIPTION
*  add one segment to awaited entry
*
* PARAMETERS
*  a  index		the index found in mmi_frm_sms_awaited that matches the last entry
*  b  addindex		addindex
*  c  segment		the current segment of the last entry in mmi_frm_sms_awaited
* RETURNS
*  add success or fail
* GLOBALS AFFECTED
*  mmi_frm_sms_index_list
*****************************************************************************/
U8 mmi_frm_sms_add_awaited_segment (U8 index, U16 addindex, U8 segment, mmi_frm_sms_deliver_msg_struct* entry)
{
	U16 thisindex = mmi_frm_sms_awaited[index].index;
	U8 islast=TRUE;
	U8 dataindex=0;
	U8 thisdataindex=mmi_frm_sms_awaited[index].msg_data[0];
	U8 prevdataindex=MMI_FRM_SMS_INVALID_NUM;

	/* find an empty entry in the mmi_frm_sms_content in order to move the last entry to that entry */
	while(dataindex<MMI_FRM_SMS_MSG_CONTENT_NUM)
	{
		if(mmi_frm_sms_content[dataindex].msglen==0)
			break;//found empty entry
		dataindex++;
	}
	/* if mmi_frm_sms_content is full and cannot find any empty entry, don't have to move */
	if (dataindex!=MMI_FRM_SMS_MSG_CONTENT_NUM)
	{
		/* move the actual data of the last entry to the entry with the index dataindex in mmi_frm_sms_content */
	        if(entry->no_msg_data)
	        {
		mmi_frm_sms_content[dataindex].data=OslMalloc(entry->no_msg_data);
		memcpy((S8*)mmi_frm_sms_content[dataindex].data, (S8*)mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data, entry->no_msg_data);
	        }
	        mmi_frm_sms_content[dataindex].msglen=entry->no_msg_data;
		mmi_frm_sms_content[dataindex].segment=segment;	
	}
	mmi_frm_sms_awaited[index].no_msg_data+=entry->no_msg_data;
	MMI_ASSERT(mmi_frm_sms_awaited[index].no_msg_data <= MMI_SMS_MAX_MSG_SEG*MMI_FRM_SMS_MSG_LEN);
	if(addindex!=MMI_FRM_SMS_INVALID_INDEX)
		mmi_frm_sms_index_list[addindex].segment = segment;
	/* thisdataindex is the current smallest segment index of this message */
	if(mmi_frm_sms_content[thisdataindex].segment > segment)
	{
		islast=FALSE;
		/* dataindex is inserted in the head, now dataindex is the current smallest segment index of this message */
		mmi_frm_sms_content[dataindex].nextindex=thisdataindex;
		mmi_frm_sms_awaited[index].msg_data[0]=dataindex;
		if(addindex!=MMI_FRM_SMS_INVALID_INDEX)
			mmi_frm_sms_awaited[index].index = addindex;
	}
	else
	{
		while(mmi_frm_sms_content[thisdataindex].nextindex != MMI_FRM_SMS_INVALID_NUM)
		{
			if(addindex!=MMI_FRM_SMS_INVALID_INDEX)
				thisindex = mmi_frm_sms_index_list[thisindex].nextindex;
			prevdataindex = thisdataindex;
			thisdataindex = mmi_frm_sms_content[thisdataindex].nextindex;
			if(mmi_frm_sms_content[thisdataindex].segment > segment)
			{
				islast=FALSE;
				break;
			}
		}
	}
	
	if(islast==TRUE)
	{
		/* dataindex is inserted in the tail, now dataindex is the current largest segment index of this message */
		mmi_frm_sms_content[thisdataindex].nextindex = dataindex;
		mmi_frm_sms_content[dataindex].nextindex = MMI_FRM_SMS_INVALID_NUM;
		if(addindex!=MMI_FRM_SMS_INVALID_INDEX)
		{
			mmi_frm_sms_index_list[thisindex].nextindex = addindex;
			mmi_frm_sms_index_list[addindex].nextindex = MMI_FRM_SMS_INVALID_INDEX;
			mmi_frm_sms_index_list[addindex].previndex = thisindex;
			mmi_frm_sms_index_list[addindex].prevtype = MMI_FRM_SMS_NOBOX;
		}
	}
	else
	{
		/* dataindex is inserted in the middle */
		if(prevdataindex!=MMI_FRM_SMS_INVALID_NUM)
			mmi_frm_sms_content[prevdataindex].nextindex = dataindex;
		mmi_frm_sms_content[dataindex].nextindex = thisdataindex;
		if(addindex!=MMI_FRM_SMS_INVALID_INDEX)
		{
			mmi_frm_sms_index_list[addindex].nextindex = thisindex;
			mmi_frm_sms_index_list[addindex].previndex=mmi_frm_sms_index_list[thisindex].previndex;
			mmi_frm_sms_index_list[addindex].prevtype=mmi_frm_sms_index_list[thisindex].prevtype;
			if(mmi_frm_sms_index_list[thisindex].prevtype==MMI_FRM_SMS_NOBOX)
				mmi_frm_sms_index_list[mmi_frm_sms_index_list[thisindex].previndex].nextindex=addindex;
			mmi_frm_sms_index_list[thisindex].previndex=addindex;
			mmi_frm_sms_index_list[thisindex].prevtype=MMI_FRM_SMS_NOBOX;
		}
	}	
	/* mmi_frm_sms_content is full and cannot handle other new segment coming anymore */
	if(dataindex==MMI_FRM_SMS_MSG_CONTENT_NUM)
		return FALSE;
	else
		return TRUE;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_awaited_entry
* DESCRIPTION
*  add sms to awaited list
*
* PARAMETERS
*  a  type			type
*  b  index			index
*  c  l4_index		l4_index
* RETURNS
*  TRUE: inserted, FALSE: not inserted
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_add_awaited_entry (void)
{
	U8 i=0;
	U8 dataindex=0;
	U16 index=mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].index;
	U16 length=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].msglen;
		
	while(dataindex<MMI_FRM_SMS_MSG_CONTENT_NUM)
	{
		if(mmi_frm_sms_content[dataindex].segment==MMI_FRM_SMS_INVALID_NUM)
			break;//found empty entry
		dataindex++;
	}
	/* cannot find any empty entry in mmi_frm_sms_content */
	if(dataindex==MMI_FRM_SMS_MSG_CONTENT_NUM)
		return MMI_FRM_SMS_INVALID_NUM;
	
	mmi_frm_sms_content[dataindex].msglen=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].msglen;
	mmi_frm_sms_content[dataindex].nextindex=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].nextindex;
	mmi_frm_sms_content[dataindex].segment=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment;
	// mmi_frm_sms_content[dataindex].data=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data;
	// mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data=NULL;
	if(length)
	{
		mmi_frm_sms_content[dataindex].data=OslMalloc(length);
		memcpy(mmi_frm_sms_content[dataindex].data, mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data, length);
	}

	while(i<MMI_FRM_SMS_ACTION_SIZE)
	{
		if(mmi_frm_sms_awaited[i].msg_data[0]==MMI_FRM_SMS_INVALID_NUM)
		{
			memcpy(&(mmi_frm_sms_awaited[i]), &(mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE]), sizeof(mmi_frm_sms_deliver_msg_struct));
			mmi_frm_sms_awaited[i].msg_data[0]=dataindex;

			/*non-class 0 message!!!!!!!*/
			if(index!=MMI_FRM_SMS_INVALID_INDEX)
				mmi_frm_sms_index_list[index].previndex=i;;
			return i;
		}
		i++;
	}
	/* cannot find any empty entry in mmi_frm_sms_awaited */
	return MMI_FRM_SMS_INVALID_NUM;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_awaited_sms
* DESCRIPTION
*  Get awaited SMS by index
*
* PARAMETERS
*  a  index		index
*  b  entry		message entry info
*  c  content		message content
* RETURNS
*  object present
* GLOBALS AFFECTED
*  none
*****************************************************************************/
UINT8 *ch_memcpy_bigEnd(UINT8 *des,UINT8*src,UINT32 cpyLen) //chenhe add for jessmine chatRoom
{
	UINT32 tmp=0;
	mmi_trace(1,"ch_memcpy_bigEnd,cpyLen is %d",cpyLen);
	if(NULL == des || NULL ==src)
	{
		mmi_trace(1,"ch_memcpy_bigEnd,invalid parameter");
		return NULL;
	}
	while( (*src!='\0'||*(src+1)!='\0')&& ( (tmp+=2)<cpyLen)  )
	{
		*des = *(src+1);
		*(des+1) = *(src+0);
		mmi_trace(1,"ch_memcpy_bigEnd,src0 is %x,src1 is %x",*src,*(src+1));
		mmi_trace(1,"ch_memcpy_bigEnd,des is %x,des1 is %x",*des,*(des+1));
		src+=2;
		des+=2;
		
	}

	return des;
}
U8 mmi_frm_sms_get_awaited_sms (U8 index, mmi_frm_sms_deliver_msg_struct* entry, U8* content)
{
	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_frm_sms_get_awaited_sms,mmi_frm_sms_awaited[index].dcs is %d",mmi_frm_sms_awaited[index].dcs);
	memcpy((S8*)entry, (S8*)&mmi_frm_sms_awaited[index], sizeof(mmi_frm_sms_deliver_msg_struct));
	
	//if(mmi_frm_sms_awaited[index].dcs == SMSAL_UCS2_DCS || mmi_frm_sms_awaited[index].dcs == SMSAL_DEFAULT_DCS) //chenhe for jasper
	#if 0
	{
		U8 	i=0;
		U8	nextindex=mmi_frm_sms_awaited[index].msg_data[0];
		EMSData *pEMSdata=NULL;
		U8* textBuff;
		U8 * TPUD[10];
		U8 TPUDLEN[10];
		U8 is_obj_present=0;
		U16 buffLen=mmi_frm_sms_awaited[index].no_msg_data;
		U16 textLen=0;
		U8 totalseg=0;
		EMSTATUS result;
		
		for(i=0; i<10; i++)
		{
			TPUDLEN[i]=0;
			TPUD[i]=NULL;
		}
		i=0;
		while(nextindex!=MMI_FRM_SMS_INVALID_NUM && i<10)
		{
			totalseg = i + 1; /* total segment that the TPUD is not NULL */
			TPUDLEN[i]=(U8)mmi_frm_sms_content[nextindex].msglen;
			TPUD[i]=(U8*)mmi_frm_sms_content[nextindex].data;
			nextindex=mmi_frm_sms_content[nextindex].nextindex;
			i++;
		}
		if(mmi_frm_sms_awaited[index].dcs == SMSAL_DEFAULT_DCS)
			buffLen*=2;
		if(buffLen)
		{
			pEMSdata=OslMalloc(sizeof(EMSData));
			 
			buffLen=(buffLen<(MMI_FRM_SMS_MSG_LEN*10))?buffLen:(MMI_FRM_SMS_MSG_LEN*10);
			textBuff=OslMalloc(buffLen);
			result = EMSMsgPreview(totalseg, mmi_frm_sms_awaited[index].dcs, (U8)GetUDHIBit(mmi_frm_sms_awaited[index].fo), 
						      TPUD, TPUDLEN, buffLen, textBuff, &is_obj_present, &textLen);

			if(result == EMS_OK)
			{
				pEMSdata->textBuffer=textBuff;
				pEMSdata->textBufferSize=mmi_frm_sms_awaited[index].no_msg_data;
				pEMSdata->textLength=textLen;
				pEMSdata->dcs = mmi_frm_sms_awaited[index].dcs;
				
				EMS_ConvertGSM7BitDefaultEncodingToAsciiWithoutEMSlib (pEMSdata);
				entry->no_msg_data=pEMSdata->textLength;
				
				if (mmi_frm_sms_awaited[index].dcs == SMSAL_UCS2_DCS)
				   memcpy((S8*)content, (S8*)pEMSdata->textBuffer, pEMSdata->textLength);
				else
				{
					S8 *pOutBuffer=(S8*)content;
					S8 *pInBuffer=(S8*)pEMSdata->textBuffer;
					U32 len=pEMSdata->textLength;
					/* 34.2.5.1 test case contains a null terminator in the message content but actually it is not the end of the message */
					// UnicodeNToAnsii((S8*)content, (S8*)pEMSdata->textBuffer, pEMSdata->textLength);
					while(len)
					{
						*pOutBuffer = *(pInBuffer) ; 
						pInBuffer += 2; 
						pOutBuffer++;
						len-=2;
					}
				}
			}
			else
			{
				entry->no_msg_data = 0;
				memset((S8*)content, 0, ENCODING_LENGTH);
			}
			
			OslMfree(textBuff);
			OslMfree(pEMSdata);
		}
		else
			memset((S8*)content, 0, ENCODING_LENGTH);

		return is_obj_present;

	}
	#else
	{
	
		U8	j=mmi_frm_sms_awaited[index].msg_data[0];
		U16 len=0;
		UINT8 *tmpb,*pMsgContent ;
		int i;

		while(j!=MMI_FRM_SMS_INVALID_NUM)
		{
		if(mmi_frm_sms_awaited[index].dcs == SMSAL_UCS2_DCS)
		{

			pMsgContent = mmi_frm_sms_content[j].data;

			#if 0
			pfnUnicodeStrncat(content,pMsgContent+1,mmi_frm_sms_content[j].msglen-2);
			#else
			//memcpy(content+2,pMsgContent+1,mmi_frm_sms_content[j].msglen-1);
			/*wangzl: 090707 del for bug 13058 and 13073
			if(len == 0)
			{
				AnsiiToUnicodeString(content,":");
				len += 2;
			}
			*/
//			ch_memcpy_bigEnd(content + len,pMsgContent,mmi_frm_sms_content[j].msglen+1);
			memcpy(content + len,pMsgContent,mmi_frm_sms_content[j].msglen);

			for (i=0; i< mmi_frm_sms_content[j].msglen; i++)
			{
				mmi_trace(g_sw_SMS,"MMI_SMS: Func: %s content[%d]=0x%x", __FUNCTION__, i, content[i]);	
			}

			#endif

			#if 0
			/*********************little end and big end convert,start****/
			pMsgContent = mmi_frm_sms_content[j].data;
			pEMSdata=OslMalloc(sizeof(EMSData));
			textBuff=OslMalloc(MMI_FRM_SMS_MSG_LEN*10);
			memcpy(textBuff,pMsgContent+1,mmi_frm_sms_content[j].msglen-1);
			pEMSdata->textBuffer = textBuff;
			pEMSdata->textBufferSize=mmi_frm_sms_awaited[index].no_msg_data;
			pEMSdata->textLength=mmi_frm_sms_content[j].msglen -1;
			pEMSdata->dcs = mmi_frm_sms_awaited[index].dcs;
			EMS_ConvertGSM7BitDefaultEncodingToAsciiWithoutEMSlib (pEMSdata);
			/*********************little end and big end convert,end****/	
			 memcpy((S8*)content+2, (S8*)pEMSdata->textBuffer, pEMSdata->textLength);
			#endif
			
			
		//	while(tmp++<mmi_frm_sms_content[j].msglen+3)
		//	mmi_trace(1,"chenhe,tmp,%x",*(content+tmp-1));
		//	mmi_trace(1,"chenhe ,SMSAL_UCS2_DCS,chat content is end,mmi_frm_sms_content[j].msglen is %d",mmi_frm_sms_content[j].msglen);
			len += mmi_frm_sms_content[j].msglen - mmi_frm_sms_content[j].msglen%2;
			j=mmi_frm_sms_content[j].nextindex;
		}
		else
		{
			
			tmpb = content;
			/*wangzl: 090707 del for bug 13058 and 13073
			if(len == 0)
			{
				len ++;
				memcpy(content,":",len);
			}
			*/
			memcpy(((S8*)content) + len, (S8*)mmi_frm_sms_content[j].data, mmi_frm_sms_content[j].msglen);
		//	while(tmp++<mmi_frm_sms_content[j].msglen)
			//mmi_trace(1,"chenhe,tmp,%x",*(tmpb+tmp));
		//	mmi_trace(1,"chenhe ,chat content is end,mmi_frm_sms_content[j].msglen is %d",mmi_frm_sms_content[j].msglen);
			len += mmi_frm_sms_content[j].msglen;
			j=mmi_frm_sms_content[j].nextindex;
		}
		}
		return FALSE;
	}
	#endif
	
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_awaited_complete
* DESCRIPTION
*  Check awaited message completeness
*
* PARAMETERS
*  a  index		index
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_check_awaited_complete (U8 index)
{
	U8 i=0;
	mmi_frm_sms_concat_struct* concat_info=(mmi_frm_sms_concat_struct*)&(mmi_frm_sms_awaited[index].concat_info);
	U8 total_seg = ((concat_info->total_seg-concat_info->seg+1)>g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)?
	   g_frm_sms_cntx.mmi_frm_sms_msg_seg_size:(concat_info->total_seg-concat_info->seg+1);
	U8 dataindex = mmi_frm_sms_awaited[index].msg_data[0];

	/* if the message has only one segment */
	if(index==MMI_FRM_SMS_ACTION_SIZE)
		return TRUE;
	while(dataindex!=MMI_FRM_SMS_INVALID_NUM)
	{
		/* if mmi_frm_sms_content is full */
		if (dataindex==MMI_FRM_SMS_MSG_CONTENT_NUM)
			return TRUE;
		if(mmi_frm_sms_content[dataindex].segment!=(i+1))
			return FALSE;
		dataindex = mmi_frm_sms_content[dataindex].nextindex;		
		i++;
	}
	if(i==total_seg)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_awaited_complete_msg
* DESCRIPTION
*  Check awaited message completeness , excluding mmi_frm_sms_content full 
*
* PARAMETERS
*  a  index		index
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_check_awaited_complete_msg  (U8 index)
{
	U8 i=0;
	mmi_frm_sms_concat_struct* concat_info=(mmi_frm_sms_concat_struct*)&(mmi_frm_sms_awaited[index].concat_info);
	U8 total_seg = ((concat_info->total_seg-concat_info->seg+1)>g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)?
	   g_frm_sms_cntx.mmi_frm_sms_msg_seg_size:(concat_info->total_seg-concat_info->seg+1);
	U8 dataindex = mmi_frm_sms_awaited[index].msg_data[0];

	while(dataindex!=MMI_FRM_SMS_INVALID_NUM)
	{
		if(mmi_frm_sms_content[dataindex].segment!=(i+1))
			return FALSE;
		dataindex = mmi_frm_sms_content[dataindex].nextindex;		
		i++;
	}
	if(i==total_seg)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_awaited
* DESCRIPTION
*  Check awaited entry, see if any match
*
* PARAMETERS
*  none
* RETURNS
*  index
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_check_awaited (void)
{
	/* find the entry in the awaited list that matches the last entry in the mmi_frm_sms_awaited  */
	U8 i=0;
	mmi_frm_sms_deliver_msg_struct* datainfo=&mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE];
	mmi_frm_sms_concat_struct* concatinfo = ((mmi_frm_sms_concat_struct*)&(mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].concat_info));
	U8 thisseg = mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment;

	if(((concatinfo->total_seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)==1)&&(concatinfo->total_seg==concatinfo->seg))
		return MMI_FRM_SMS_ACTION_SIZE;
	
	while(i<MMI_FRM_SMS_ACTION_SIZE)
	{
		mmi_frm_sms_deliver_msg_struct* data=&mmi_frm_sms_awaited[i];
		mmi_frm_sms_concat_struct* concat = ((mmi_frm_sms_concat_struct*)&(mmi_frm_sms_awaited[i].concat_info));
		if((data->display_type==datainfo->display_type)&&(concat->total_seg == concatinfo->total_seg)&&(concat->seg==concatinfo->seg)&&(concat->ref==concatinfo->ref)
			&&((ComparePhoneNum((PU8)data->addr_number.number,(PU8)datainfo->addr_number.number))==TRUE))
		{
			U8 j=mmi_frm_sms_awaited[i].msg_data[0];
			while(j!=MMI_FRM_SMS_INVALID_NUM)
			{
				/* find exactly the same entry in the awaited list as the last entry */
				if(mmi_frm_sms_content[j].segment==thisseg)
					break;
				/* thisseg is smaller than the current smallest segment number of this message */
				else if(mmi_frm_sms_content[j].segment>thisseg)
				{
				#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
					if (mmi_frm_sms_awaited[i].storageType != MMI_FRM_SMS_PREFER_STORAGE)
					{
						mmi_frm_sms_awaited[i].storageType = datainfo->storageType;
					}
				#endif				
					/* if add awaited segment fail, it will popup arbitrarily, so leave the mmi_frm_sms_indicate_sms to free memory */
					if (mmi_frm_sms_add_awaited_segment(i, datainfo->index, thisseg, datainfo)==TRUE)
					{						
					        mmi_frm_sms_free_new_sms();
					}
					return i;
				}
				/* thisseg is greater than the current largest segment number of this message*/
				else if(mmi_frm_sms_content[j].segment<thisseg && mmi_frm_sms_content[j].nextindex==MMI_FRM_SMS_INVALID_NUM) 
			        {
				#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
				        if (mmi_frm_sms_awaited[i].storageType != MMI_FRM_SMS_PREFER_STORAGE)
				        {
					        mmi_frm_sms_awaited[i].storageType = datainfo->storageType;
				         }
				#endif			        
				        /* if add awaited segment fail, it will popup arbitrarily, so leave the mmi_frm_sms_indicate_sms to free memory */
				        if (mmi_frm_sms_add_awaited_segment(i, datainfo->index, thisseg, datainfo)==TRUE)
				        {				
				                mmi_frm_sms_free_new_sms();
				        }
				        return i;
			        }
				j=mmi_frm_sms_content[j].nextindex;
			}
		}
		i++;
	}
	return MMI_FRM_SMS_INVALID_NUM;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_inbox
* DESCRIPTION
*  Check inbox, see if any match
*
* PARAMETERS
*  a  entry	message entry info
*  b  thissegment		segment
* RETURNS
*  index
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U16 mmi_frm_sms_check_inbox (mmi_frm_sms_msgbox_struct* entry, U8 thissegment)
{
	U16 i=0;
	while(i<g_frm_sms_cntx.mmi_frm_sms_msg_box_size)
	{
		if(mmi_frm_sms_compare_entry(i, entry, thissegment)==TRUE)
			return i;
		i++;
	}
	return MMI_FRM_SMS_INVALID_INDEX;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_timeout_concatenate_sms
* DESCRIPTION
*  timeout check concatenate SMS
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_timeout_concatenate_sms(void)
{
	U8 i=0;
	U8 awaited_remain = FALSE;
	while(i<MMI_FRM_SMS_ACTION_SIZE)
	{
		if(mmi_frm_sms_awaited_counter[i]!=MMI_FRM_SMS_INVALID_NUM)
		{
			mmi_frm_sms_awaited_counter[i]++;
			if(mmi_frm_sms_awaited_counter[i]==MMI_FRM_SMS_ACTION_SIZE)
			{
				mmi_frm_sms_awaited_counter[i]=MMI_FRM_SMS_INVALID_NUM;
				mmi_frm_sms_indicate_sms(i);
			}
			else
				awaited_remain=TRUE;
		}
		i++;
	}
	/* timer expires but still some segments are missing in the mmi_frm_sms_awaited */
	if(awaited_remain==TRUE)
		StartTimer(MESSAGES_LMS_RECV_TIMER_ID, MMI_FRM_SMS_LMS_TIME_OUT, mmi_frm_sms_timeout_concatenate_sms);
	else
		mmi_frm_sms_awaited_counting=FALSE;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_concatenate_sms
* DESCRIPTION
*  Check concatenate SMS in awaited and inbox list
*
* PARAMETERS
*  a  index		index
* RETURNS
*  type
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_check_concatenate_sms (U16* index)
{
	mmi_frm_sms_concat_struct* concatinfo=(mmi_frm_sms_concat_struct*)&(mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].concat_info);
	if(((concatinfo->total_seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)==1)&&(concatinfo->total_seg==concatinfo->seg))
	{
		*index=MMI_FRM_SMS_ACTION_SIZE;
		return MMI_FRM_SMS_AWAITS;
	}
	else
	{
		U8 awaited_index = mmi_frm_sms_check_awaited();
		/* the last entry in the mmi_frm_sms_awaited belongs to one of the segments of the entry with index awaited_index in the awaited list */
		if(awaited_index != MMI_FRM_SMS_INVALID_NUM)
		{
			if(mmi_frm_sms_check_awaited_complete(awaited_index)==TRUE)
			{
				mmi_frm_sms_awaited_counter[awaited_index]=MMI_FRM_SMS_INVALID_NUM;
				*index=awaited_index;
				return MMI_FRM_SMS_AWAITS;
			}
			/* not all segments of this message are arrived, restart the counter to keep waiting for other segment's arriving */
			else
			{
				mmi_frm_sms_awaited_counter[awaited_index]=0;
				*index=awaited_index;
				return MMI_FRM_SMS_NOBOX;
			}
		}
		/* the last entry in the mmi_frm_sms_awaited matches nothing with other entries in the awaited list */
		else if(mmi_frm_sms_list_done)
		{
			U16 inbox_index;
			mmi_frm_sms_msgbox_struct * entry=OslMalloc(sizeof(mmi_frm_sms_msgbox_struct));
			U8 thissegment = mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment;
			mmi_frm_sms_convert_mt_to_entry(&(mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE]), entry);
			inbox_index = mmi_frm_sms_check_inbox(entry, thissegment);
			OslMfree(entry);
				
			/* the last entry in the mmi_frm_sms_awaited belongs to one of the segments of the entry with index inbox_index in the inbox list */
			if(inbox_index != MMI_FRM_SMS_INVALID_INDEX)
			{
				mmi_frm_sms_add_sms_segment(inbox_index, mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].index, thissegment);
         #if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
				if(mmi_frm_sms_msg_box[inbox_index].storageType != MMI_FRM_SMS_PREFER_STORAGE)
					mmi_frm_sms_msg_box[inbox_index].storageType = mmi_frm_sms_awaited[MMI_FRM_SMS_ACTION_SIZE].storageType;
         #endif
				mmi_frm_sms_free_new_sms();
				*index=inbox_index;
				return MMI_FRM_SMS_INBOX;
			}
		}
		/* cannot find any match in awaited list and inbox list, add one entry with index awaited_index in the mmi_frm_sms_awaited */
		awaited_index=mmi_frm_sms_add_awaited_entry();
		/* find an empty entry in the awaited list and add successfully */
		if(awaited_index!=MMI_FRM_SMS_INVALID_NUM)
		{
			mmi_frm_sms_free_new_sms();
			/* the timer is not started yet */
		        if(mmi_frm_sms_awaited_counting==FALSE)
		        {
			        StartTimer(MESSAGES_LMS_RECV_TIMER_ID, MMI_FRM_SMS_LMS_TIME_OUT, mmi_frm_sms_timeout_concatenate_sms);
			        mmi_frm_sms_awaited_counting=TRUE;
		        }
			mmi_frm_sms_awaited_counter[awaited_index]=0;
			*index=MMI_FRM_SMS_INVALID_INDEX;
			return MMI_FRM_SMS_NOBOX;
		}
		/* awaited list is full and the segment should be popup arbitrarily */
		else
		{
			*index=MMI_FRM_SMS_ACTION_SIZE;
			return MMI_FRM_SMS_AWAITS;
		}
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_sms_complete
* DESCRIPTION
*  Check SMS completeness
*
* PARAMETERS
*  a  index		index
*  b  type		type
* RETURNS
*  TRUE: complete, FALSE: not
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_check_sms_complete (U8 type, U16 index)
{
	U8 i=0;
	U8 total_seg;
	U16 dataindex;
	U16 msgindex;

	switch(type)
	{
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			msgindex = mmi_frm_sms_inbox_list[index];
		break;
		case MMI_FRM_SMS_OUTBOX:
		#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
		#endif
			msgindex = mmi_frm_sms_outbox_list[index];
		break;
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			msgindex = mmi_frm_sms_drafts_list[index];
		break;
		#endif
		default:
			msgindex = 0;
		break;
	}
	total_seg = ((mmi_frm_sms_msg_box[msgindex].totalseg-mmi_frm_sms_msg_box[msgindex].startseg+1)>g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)?
		g_frm_sms_cntx.mmi_frm_sms_msg_seg_size:(mmi_frm_sms_msg_box[msgindex].totalseg-mmi_frm_sms_msg_box[msgindex].startseg+1);
	dataindex = mmi_frm_sms_msg_box[msgindex].startindex;
	while(dataindex!=MMI_FRM_SMS_INVALID_INDEX)
	{
		if(mmi_frm_sms_index_list[dataindex].segment!=(i+1))
			return FALSE;
		dataindex = mmi_frm_sms_index_list[dataindex].nextindex;		
		i++;
	}
	if(i==total_seg)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_data_index
* DESCRIPTION
*  get data index of specific source port and module
*
* PARAMETERS
*  a  mod		mod
*  b  mod		port
* RETURNS
*  index
* GLOBALS AFFECTED
*  mmi_frm_sms_index_list
*****************************************************************************/
U8 mmi_frm_sms_get_data_index (module_type mod, U16 port, U8 startindex)
{
	U8 i=0;
	for(i=startindex; i<MMI_FRM_SMS_ACTION_SIZE; i++)
	{
		if((mmi_frm_sms_data[i].dest_mod_id==mod)&&(mmi_frm_sms_data[i].dest_port==port))
			return i;
	}
	return MMI_FRM_SMS_INVALID_NUM;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_remove_data
* DESCRIPTION
*  Remove SMS with port, by module and port
*
* PARAMETERS
*  a  mod		module
*  b  port		port number
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_remove_data (module_type mod, U16 port)
{
	U8 i=0;
	for(i=0; i<MMI_FRM_SMS_ACTION_SIZE; i++)
	{
		if((mmi_frm_sms_data[i].dest_mod_id==mod)&&(mmi_frm_sms_data[i].dest_port==port))
			mmi_frm_sms_free_data(i);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_data_segment
* DESCRIPTION
*  add one segment to awaited entry
*
* PARAMETERS
*  a  index			index
*  b  addindex		addindex
*  c  segment		segment
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_index_list
*****************************************************************************/
U8 mmi_frm_sms_add_data_segment (U8 index, U8 segment, mmi_frm_sms_deliver_data_struct* entry)
{
	U8 islast=TRUE;
	U8 dataindex=0;
	U8 thisdataindex=mmi_frm_sms_data[index].data[0];
	U8 prevdataindex=MMI_FRM_SMS_INVALID_NUM;

	/* find an empty entry in mmi_frm_sms_content in order to move the last entry to that entry */
	while(dataindex<MMI_FRM_SMS_MSG_CONTENT_NUM)
	{
		if(mmi_frm_sms_content[dataindex].msglen==0)
			break;//found empty entry
		dataindex++;
	}
	/* if mmi_frm_sms_content is full and cannot find any empty entry, don't have to move */
	if (dataindex!=MMI_FRM_SMS_MSG_CONTENT_NUM)
	{
		/* move the actual data of the last entry to the entry with the index dataindex in mmi_frm_sms_content */
	        if(entry->message_len)
	        {
		mmi_frm_sms_content[dataindex].data=OslMalloc(entry->message_len);
		memcpy((S8*)mmi_frm_sms_content[dataindex].data, (S8*)mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data, entry->message_len);
	        }
	        mmi_frm_sms_content[dataindex].msglen=entry->message_len;
		mmi_frm_sms_content[dataindex].segment=segment;
	}
	mmi_frm_sms_data[index].message_len+=entry->message_len;
	MMI_ASSERT(mmi_frm_sms_data[index].message_len <= MMI_SMS_MAX_MSG_SEG*MMI_FRM_SMS_MSG_LEN);
	
	/* thisdataindex is the current smallest segment index of this message */
	if(mmi_frm_sms_content[thisdataindex].segment > segment)
	{
		islast=FALSE;
		/* dataindex is inserted in the head, now dataindex is the current smallest segment index of this message */
		mmi_frm_sms_content[dataindex].nextindex=thisdataindex;
		mmi_frm_sms_data[index].data[0]=dataindex;
	}
	else
	{
		while(mmi_frm_sms_content[thisdataindex].nextindex != MMI_FRM_SMS_INVALID_NUM)
		{
			prevdataindex = thisdataindex;
			thisdataindex = mmi_frm_sms_content[thisdataindex].nextindex;
			if(mmi_frm_sms_content[thisdataindex].segment > segment)
			{
				islast=FALSE;
				break;
			}
		}
	}
	
	if(islast==TRUE)
	{
		/* dataindex is inserted in the tail, now dataindex is the current largest segment index of this message */
		mmi_frm_sms_content[thisdataindex].nextindex = dataindex;
		mmi_frm_sms_content[dataindex].nextindex = MMI_FRM_SMS_INVALID_NUM;
	}
	else
	{
		/* dataindex is inserted in the middle */
		if (prevdataindex!=MMI_FRM_SMS_INVALID_NUM)
			mmi_frm_sms_content[prevdataindex].nextindex = dataindex;
		mmi_frm_sms_content[dataindex].nextindex = thisdataindex;
}
	/* mmi_frm_sms_content is full and cannot handle other new segment coming anymore */
	if(dataindex==MMI_FRM_SMS_MSG_CONTENT_NUM)
		return FALSE;
	else
		return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_add_data_entry
* DESCRIPTION
*  add sms to awaited list
*
* PARAMETERS
*  a  type			type
*  b  index			index
*  c  l4_index		l4_index
* RETURNS
*  TRUE: inserted, FALSE: not inserted
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_add_data_entry (void)
{
	U8 i=0;
	U8 dataindex=0;
	U16 length=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].msglen;
		
	while(dataindex<MMI_FRM_SMS_MSG_CONTENT_NUM)
	{
		if(mmi_frm_sms_content[dataindex].segment==MMI_FRM_SMS_INVALID_NUM)
			break;//found empty entry
		dataindex++;
	}
	/* cannot find any empty entry in mmi_frm_sms_content */
	if(dataindex==MMI_FRM_SMS_MSG_CONTENT_NUM)
		return MMI_FRM_SMS_INVALID_NUM;
	
	mmi_frm_sms_content[dataindex].msglen=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].msglen;
	mmi_frm_sms_content[dataindex].nextindex=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].nextindex;
	mmi_frm_sms_content[dataindex].segment=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment;
	// mmi_frm_sms_content[dataindex].data=mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data;
	// mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data=NULL;
	if(length)
	{
		mmi_frm_sms_content[dataindex].data=OslMalloc(length);
		memcpy(mmi_frm_sms_content[dataindex].data, mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].data, length);
	}
	
	while(i<MMI_FRM_SMS_ACTION_SIZE)
	{
		if(mmi_frm_sms_data[i].data[0]==MMI_FRM_SMS_INVALID_NUM)
		{
			memcpy(&(mmi_frm_sms_data[i]), &(mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE]), sizeof(mmi_frm_sms_deliver_data_struct));
			mmi_frm_sms_data[i].data[0]=dataindex;
			return i;
		}
		i++;
	}
	/* cannot find any empty entry in mmi_frm_sms_data */
	return MMI_FRM_SMS_INVALID_NUM;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_data
* DESCRIPTION
*  Get SMS with port: get content
*
* PARAMETERS
*  a  index		index
*  b  data		entry info
*  c  content		content
* RETURNS
*  TRUE: OK, FALSE: content empty
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_get_data(U8 index, mmi_frm_sms_deliver_data_struct* data, U8* content)
{
	if(mmi_frm_sms_data[index].message_len==0)
		return FALSE;
		
	memcpy((S8*)data, (S8*)&mmi_frm_sms_data[index], sizeof(mmi_frm_sms_deliver_data_struct));
	memset((S8*)content, 0, mmi_frm_sms_data[index].message_len);

	if(mmi_frm_sms_data[index].dcs == SMSAL_UCS2_DCS || mmi_frm_sms_data[index].dcs == SMSAL_DEFAULT_DCS)
	{
		U8 i=mmi_frm_sms_data[index].data[0];
		EMSData *pEMSdata=NULL;
		U8* textBuff=NULL;
		U8* textBuffShifted=NULL;
		U8 * TPUD[1];
		U8 TPUDLEN[1];
		U8 is_obj_present=0;
		U16 buffLen=mmi_frm_sms_data[index].message_len;
		U16 textLen=0;
		U8 udhi = KAL_FALSE;

		if(mmi_frm_sms_data[index].dcs  == SMSAL_DEFAULT_DCS)
			buffLen*=2;

		if(buffLen)
		{
			pEMSdata=OslMalloc(sizeof(EMSData));
			 
			buffLen=(buffLen<(MMI_FRM_SMS_MSG_LEN*10))?buffLen:(MMI_FRM_SMS_MSG_LEN*10);
			textBuff=OslMalloc(buffLen);
			textBuffShifted = textBuff;
			
			pEMSdata->textBuffer=textBuff;
			pEMSdata->textBufferSize=buffLen;
			pEMSdata->textLength=0;
			pEMSdata->dcs = mmi_frm_sms_data[index].dcs;
			
			while(i!=MMI_FRM_SMS_INVALID_NUM)
			{
				TPUDLEN[0]=(U8)mmi_frm_sms_content[i].msglen;
				TPUD[0]=(U8*)mmi_frm_sms_content[i].data;
				EMSMsgPreview(1, mmi_frm_sms_data[index].dcs, udhi, 
						      TPUD, TPUDLEN, buffLen, textBuffShifted, &is_obj_present, &textLen);

				pEMSdata->textLength += textLen;
				textBuffShifted += textLen;
				buffLen -= textLen;
				textLen=0;
				
				i=mmi_frm_sms_content[i].nextindex;
			}
			

			EMS_ConvertGSM7BitDefaultEncodingToAsciiWithoutEMSlib (pEMSdata);
			//UnicodeNToAnsii((S8*)content, (S8*)pEMSdata->textBuffer, pEMSdata->textBufferSize);
			memcpy((S8*)content, (S8*)pEMSdata->textBuffer, pEMSdata->textLength);
			data->message_len = pEMSdata->textLength;
			OslMfree(textBuff);
			OslMfree(pEMSdata);
		}
		else
			memset((S8*)content, 0, ENCODING_LENGTH);
		return TRUE;
	}
	else
	{
		U8 i=mmi_frm_sms_data[index].data[0];
		U16 len=0;
		while(i!=MMI_FRM_SMS_INVALID_NUM)
		{
			memcpy(((S8*)content)+len, (S8*)mmi_frm_sms_content[i].data, mmi_frm_sms_content[i].msglen);
			len += mmi_frm_sms_content[i].msglen;
			i=mmi_frm_sms_content[i].nextindex;
		}
		return TRUE;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_data
* DESCRIPTION
*  Check SMS with port in data list
*
* PARAMETERS
*  none
* RETURNS
*  index
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_check_data (void)
{
	/* find the entry in the data list that matches the last entry in the mmi_frm_sms_data */
	U8 i=0;
	mmi_frm_sms_deliver_data_struct* datainfo=&mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE];
	mmi_frm_sms_concat_struct* concatinfo = ((mmi_frm_sms_concat_struct*)&(mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].concat_info));
	U8 thisseg = mmi_frm_sms_content[MMI_FRM_SMS_MSG_CONTENT_NUM].segment;

	if(((concatinfo->total_seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)==1)&&(concatinfo->total_seg==concatinfo->seg))
		return MMI_FRM_SMS_ACTION_SIZE;
	
	while(i<MMI_FRM_SMS_ACTION_SIZE)
	{
		mmi_frm_sms_deliver_data_struct* data=&mmi_frm_sms_data[i];
		mmi_frm_sms_concat_struct* concat = ((mmi_frm_sms_concat_struct*)&(mmi_frm_sms_data[i].concat_info));
		if((concat->total_seg == concatinfo->total_seg)&&(concat->seg==concatinfo->seg)&&(concat->ref==concatinfo->ref)
			&&((ComparePhoneNum((PU8)data->oa.number,(PU8)datainfo->oa.number))==TRUE))
		{
			U8 j=mmi_frm_sms_data[i].data[0];
			while(j!=MMI_FRM_SMS_INVALID_NUM)
			{
				/* find exactly the same entry in the data list as the last entry */
				if(mmi_frm_sms_content[j].segment==thisseg)
					break;
				/* thisseg is smaller than the current smallest segment number of this message */
				else if(mmi_frm_sms_content[j].segment>thisseg)
				{
					/* if add data segment fail, the last entry is not duplicated, so it cannot free here */
					if (mmi_frm_sms_add_data_segment(i, thisseg, datainfo)==TRUE)
					mmi_frm_sms_free_new_data();
					return i;
				}
				/* thisseg is greater than the current largest segment number of this message*/
				else if(mmi_frm_sms_content[j].segment<thisseg && mmi_frm_sms_content[j].nextindex==MMI_FRM_SMS_INVALID_NUM)
			        {
				        /* if add data segment fail, the last entry is not duplicated, so it cannot free here */
				        if (mmi_frm_sms_add_data_segment(i, thisseg, datainfo)==TRUE)
				        mmi_frm_sms_free_new_data();
				        return i;
			        }
				j=mmi_frm_sms_content[j].nextindex;
			}
		}
		i++;
	}
	return MMI_FRM_SMS_INVALID_NUM;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_timeout_concatenate_data
* DESCRIPTION
*  timeout check concatenate SMS
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_timeout_concatenate_data(void)
{
	U8 i=0;
	U8 awaited_remain = FALSE;
	while(i<MMI_FRM_SMS_ACTION_SIZE)
	{
		if(mmi_frm_sms_data_counter[i]!=MMI_FRM_SMS_INVALID_NUM)
		{
			mmi_frm_sms_data_counter[i]++;
			if(mmi_frm_sms_data_counter[i]==MMI_FRM_SMS_ACTION_SIZE)
			{
				mmi_frm_sms_data_counter[i]=MMI_FRM_SMS_INVALID_NUM;
				mmi_frm_sms_free_data(i);
			}
			else
				awaited_remain=TRUE;
		}
		i++;
	}
	/* timer expires but still some segments are missing in the mmi_frm_sms_awaited */
	if(awaited_remain==TRUE)
		StartTimer(MESSAGES_LMS_RECV_TIMER_ID, MMI_FRM_SMS_LMS_TIME_OUT, mmi_frm_sms_timeout_concatenate_data);
	else
		mmi_frm_sms_data_counting=FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_concatenate_data
* DESCRIPTION
*  Check concatenate data
*
* PARAMETERS
*  a  index		index
* RETURNS
*  index
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_check_concatenate_data (U8* index)
{
	mmi_frm_sms_concat_struct* concatinfo=(mmi_frm_sms_concat_struct*)&(mmi_frm_sms_data[MMI_FRM_SMS_ACTION_SIZE].concat_info);
	if(((concatinfo->total_seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)==1)&&(concatinfo->total_seg==concatinfo->seg))
	{
		*index=MMI_FRM_SMS_ACTION_SIZE;
		return MMI_FRM_SMS_DATA;
	}
	else
	{
		U8 awaited_index = mmi_frm_sms_check_data();
		/* the last entry in the mmi_frm_sms_data belongs to one of the segments of the entry with index awaited_index in the data list */
		if(awaited_index != MMI_FRM_SMS_INVALID_NUM)
		{
			if(mmi_frm_sms_check_data_complete(awaited_index)==TRUE)
			{
				mmi_frm_sms_data_counter[awaited_index]=MMI_FRM_SMS_INVALID_NUM;
				*index=awaited_index;
				return MMI_FRM_SMS_DATA;
			}
			/* not all segments of this message are arrived */
			else
			{
				U8 dataindex=0;
				/* check if mmi_frm_sms_content is full */
				while(dataindex<MMI_FRM_SMS_MSG_CONTENT_NUM)
				{
					if(mmi_frm_sms_content[dataindex].segment==MMI_FRM_SMS_INVALID_NUM)
					{
						/* mmi_frm_sms_content is not full, restart the counter to keep waiting for other segment's arriving */
						mmi_frm_sms_data_counter[awaited_index]=0;
						*index=awaited_index;
						return MMI_FRM_SMS_NOBOX;
					}
					dataindex++;
				}
				/* mmi_frm_sms_content is full, reset the counter and free this message */
				mmi_frm_sms_data_counter[awaited_index]=MMI_FRM_SMS_INVALID_NUM;
				mmi_frm_sms_free_data(awaited_index);
				*index=awaited_index;
				return MMI_FRM_SMS_NOBOX;
			}
		}
		else
		{
			/* cannot find any match in data list, add one entry with index awaited_index in the mmi_frm_sms_data */
			awaited_index=mmi_frm_sms_add_data_entry();
			/* find an empty entry in the data list and add successfully */
			if(awaited_index!=MMI_FRM_SMS_INVALID_NUM)
			{
			        mmi_frm_sms_free_new_data();
				/* the timer is not started yet */
				if(mmi_frm_sms_data_counting==FALSE)
				{
					StartTimer(MESSAGES_LMS_RECV_TIMER_ID, MMI_FRM_SMS_LMS_TIME_OUT, mmi_frm_sms_timeout_concatenate_data);
					mmi_frm_sms_data_counting=TRUE;
				}
				mmi_frm_sms_data_counter[awaited_index]=0;
			        *index=MMI_FRM_SMS_INVALID_NUM;
			        return MMI_FRM_SMS_NOBOX;
		        }
			/* data list or content list is full and the segment should be discarded */
			else
			{
				mmi_frm_sms_free_new_data();
				*index=MMI_FRM_SMS_ACTION_SIZE;
				return MMI_FRM_SMS_NOBOX;
			}
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_data_complete
* DESCRIPTION
*  Check SMS with port completeness
*
* PARAMETERS
*  a  index		index
* RETURNS
*  TRUE: complete, FALSE: not
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_check_data_complete(U8 index)
{
	U8 i=0;
	mmi_frm_sms_concat_struct* concat_info=(mmi_frm_sms_concat_struct*)&(mmi_frm_sms_data[index].concat_info);
	U8 total_seg = ((concat_info->total_seg-concat_info->seg+1)>g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)?
	   g_frm_sms_cntx.mmi_frm_sms_msg_seg_size:(concat_info->total_seg-concat_info->seg+1);
	U8 dataindex = mmi_frm_sms_data[index].data[0];

	/* if the message has only one segment */
	if(index==MMI_FRM_SMS_ACTION_SIZE)
		return TRUE;
	while(dataindex!=MMI_FRM_SMS_INVALID_NUM)
	{
		if(mmi_frm_sms_content[dataindex].segment!=(i+1))
			return FALSE;
		dataindex = mmi_frm_sms_content[dataindex].nextindex;		
		i++;
	}
	if(i==total_seg)
		return TRUE;
	else
		return FALSE;
}

/*given msg box index, change to type*/
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_sms_status
* DESCRIPTION
*  Set SMS status
*
* PARAMETERS
*  a  type		origin type
*  b  index		index
*  c  newtype		type to be set
* RETURNS
*  TRUE
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_set_sms_status (U8 type, U16 index, U8 newtype)
{
	U8 mti;
	U16 msgindex;
	switch(type)
	{
		case MMI_FRM_SMS_NOBOX:
			msgindex=index;
			mti=(mmi_frm_sms_msg_box[msgindex].msgtype & 0xf0);	
			mmi_frm_sms_msg_box[msgindex].msgtype = mti | newtype;
			break;
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			msgindex=mmi_frm_sms_inbox_list[index];
			mti=(mmi_frm_sms_msg_box[msgindex].msgtype & 0xf0);	
			mmi_frm_sms_msg_box[msgindex].msgtype = mti | newtype;
			mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_set_sms_status, mmi_frm_sms_msg_box[%d].msgtype = %d, mti is:%d", msgindex, mmi_frm_sms_msg_box[msgindex].msgtype, mti);
			break;
		default:
			mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_set_sms_status DO NOTHING");
		break;
	}
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_status
* DESCRIPTION
*  Get SMS status
*
* PARAMETERS
*  a  type		type
*  b  index		index
* RETURNS
*  status
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_get_sms_status (U8 type, U16 index)
{
	U16 entryindex=index;
	switch (type)
	{
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			entryindex=mmi_frm_sms_inbox_list[index];
			break;
		case MMI_FRM_SMS_OUTBOX:
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
	#endif
			entryindex=mmi_frm_sms_outbox_list[index];
			break;
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			entryindex=mmi_frm_sms_drafts_list[index];
			break;
	#endif
	}
	return mmi_frm_sms_msg_box[entryindex].msgtype;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_dcs
* DESCRIPTION
*  Get SMS DCS
*
* PARAMETERS
*  a  type		type
*  b  index		index
* RETURNS
*  dcs
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_get_sms_dcs (U8 type, U16 index)
{
	if(type==MMI_FRM_SMS_DATA)
		return mmi_frm_sms_data[index].dcs;
	else
		return SMSAL_8BIT_DCS;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_address
* DESCRIPTION
*  Get SMS address
*
* PARAMETERS
*  a  type		type
*  b  index		index
* RETURNS
*  address
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8* mmi_frm_sms_get_sms_address (U8 type, U16 index)
{
	U16 entryindex=index;
	switch (type)
	{
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			entryindex=mmi_frm_sms_inbox_list[index];
			break;
		case MMI_FRM_SMS_OUTBOX:
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
	#endif
			entryindex=mmi_frm_sms_outbox_list[index];
			break;
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			entryindex=mmi_frm_sms_drafts_list[index];
			break;
	#endif
		default:
			entryindex=index;
			break;
	}
	if(type==MMI_FRM_SMS_DATA)
		return (U8*)&mmi_frm_sms_data[entryindex].oa;
	else
		return mmi_frm_sms_msg_box[entryindex].number;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_timestamp
* DESCRIPTION
*  Get SMS timestamp
*
* PARAMETERS
*  a  type		type
*  b  index		index
* RETURNS
*  timestamp
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8* mmi_frm_sms_get_sms_timestamp (U8 type, U16 index)
{
	U16 entryindex=index;
	switch (type)
	{
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			entryindex=mmi_frm_sms_inbox_list[index];
			break;
		case MMI_FRM_SMS_OUTBOX:
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
	#endif
			entryindex=mmi_frm_sms_outbox_list[index];
			break;
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			entryindex=mmi_frm_sms_drafts_list[index];
			break;
	#endif
	}
	if(type==MMI_FRM_SMS_DATA)
		return mmi_frm_sms_data[entryindex].scts;
	else
		return mmi_frm_sms_msg_box[entryindex].timestamp;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_size
* DESCRIPTION
*  Get SMS size
*
* PARAMETERS
*  a  type		type
*  b  index		index
* RETURNS
*  size
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_get_sms_size (U8 type, U16 index)
{
	U16 msgindex=0;
	U8 totalseg=0;
	switch(type)
	{
		case MMI_FRM_SMS_NOBOX:
			msgindex=index;
		break;

		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			msgindex=mmi_frm_sms_inbox_list[index];
		break;

		case MMI_FRM_SMS_OUTBOX:
		#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
		#endif
			msgindex=mmi_frm_sms_outbox_list[index];
		break;

		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			msgindex=mmi_frm_sms_drafts_list[index];
		break;
		#endif
	}
	totalseg = mmi_frm_sms_msg_box[msgindex].totalseg-mmi_frm_sms_msg_box[msgindex].startseg+1;

	mmi_trace(g_sw_SMS, "HHHHHHHHHHHHHHHHHHHHH    totalseg = %d",totalseg);
	
	return ((totalseg>g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)?g_frm_sms_cntx.mmi_frm_sms_msg_seg_size:totalseg);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_received_size
* DESCRIPTION
*  Get SMS received segment number 
*
* PARAMETERS
*  a  type		type
*  b  index		index
* RETURNS
*  size
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_get_sms_received_size (U8 type, U16 index)
{
	U16 msgindex=0;
	U8 receivedseg=0;
	unsigned short l4index;
	switch(type)
	{
		case MMI_FRM_SMS_NOBOX:
			msgindex=index;
		break;

		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			msgindex=mmi_frm_sms_inbox_list[index];
		break;

		case MMI_FRM_SMS_OUTBOX:
		#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
		#endif
			msgindex=mmi_frm_sms_outbox_list[index];
		break;

		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			msgindex=mmi_frm_sms_drafts_list[index];
		break;
		#endif
	}
	
	l4index = mmi_frm_sms_msg_box[msgindex].startindex;
	while( l4index != MMI_FRM_SMS_INVALID_INDEX)
	{
		receivedseg++;
		l4index = mmi_frm_sms_index_list[l4index].nextindex;
	}
	return receivedseg ;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_list_size
* DESCRIPTION
*  Get SMS list size
*
* PARAMETERS
*  a  type		type
* RETURNS
*  size
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U16 mmi_frm_sms_get_sms_list_size (U8 type)
{
	BOOL bAllReady = MMI_TRUE;
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!g_frm_sms_cntx.mmi_frm_sms_ready[i])
		{
			bAllReady = MMI_FALSE;
			break;
		}
	}

	if (!bAllReady || mmi_frm_sms_list_done == FALSE)
	{
		return MMI_FRM_SMS_INVALID_INDEX;
	}
	
	switch (type)
	{
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			return mmi_frm_sms_inbox_size;
		case MMI_FRM_SMS_OUTBOX:
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
	#endif
			return mmi_frm_sms_outbox_size;
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			return mmi_frm_sms_drafts_size;
	#endif
	}
	return 0;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_data_check
* DESCRIPTION
*  See if data is checked
*
* PARAMETERS
*  a  index		index
* RETURNS
*  Check or not
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_get_data_check (U8 index)
{
	return (mmi_frm_sms_data_checked>>index)&0x01;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_data_check
* DESCRIPTION
*  Set SMS check
*
* PARAMETERS
*  a  index		index
*  b  set		1/0
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_set_data_check (U8 index, U8 set)
{
	U16 resetVector = (U16)( 0xffff - (1<<index));
	U16 setVector = (U16)set;

	setVector = setVector << index;
	
	/* Reset the check bit to zero  */
	mmi_frm_sms_data_checked &= resetVector;

	mmi_frm_sms_data_checked |= setVector;
}

#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms_storage
* DESCRIPTION
*  get SMS storage type
*
* PARAMETERS
*  a  type :	mmi_frm_sms_app_msgbox_enum
*  b  index:	message list index
* RETURNS
*  smsal_storage_enum (SMSAL_SM or SMSAL_ME)
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_get_sms_storage(U8 type, U16 index)
{
	U16 msgindex=0;

	switch(type)
	{
		case MMI_FRM_SMS_NOBOX:
			msgindex=index;
		break;

		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			msgindex=mmi_frm_sms_inbox_list[index];
		break;

		case MMI_FRM_SMS_OUTBOX:
		#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
		#endif
			msgindex=mmi_frm_sms_outbox_list[index];
		break;

		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			msgindex=mmi_frm_sms_drafts_list[index];
		break;
		#endif
	}
	
	return mmi_frm_sms_msg_box[msgindex].storageType;
}

#ifdef __MMI_MESSAGES_COPY__
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_copy_sms_abort
* DESCRIPTION
*  abort SMS copy action
*
* PARAMETERS
* none
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_copy_sms_abort()
{
	g_frm_sms_copy_info.is_aborted = TRUE;
	return;
}
#endif
#endif

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_swap_list_index
* DESCRIPTION
*  swap list index
*
* PARAMETERS
*  a  type :	mmi_frm_sms_app_msgbox_enum, only inbox, outbox or draft
*  b  index1:	
*	  index2:
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_swap_list_index(U8 type, U16 index1, U16 index2)
{
	U16 msgindex;

	switch(type)
	{
		case MMI_FRM_SMS_INBOX:
		case MMI_FRM_SMS_UNREAD:
			msgindex=mmi_frm_sms_inbox_list[index1];
			mmi_frm_sms_inbox_list[index1] = mmi_frm_sms_inbox_list[index2];
			mmi_frm_sms_inbox_list[index2] = msgindex;
		break;

		case MMI_FRM_SMS_OUTBOX:
		#ifndef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
		#endif
			msgindex=mmi_frm_sms_outbox_list[index1];
			mmi_frm_sms_outbox_list[index1] = mmi_frm_sms_outbox_list[index2];
			mmi_frm_sms_outbox_list[index2] = msgindex;
		break;

		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case MMI_FRM_SMS_DRAFTS:
			msgindex=mmi_frm_sms_drafts_list[index1];
			mmi_frm_sms_drafts_list[index1] = mmi_frm_sms_drafts_list[index2];
			mmi_frm_sms_drafts_list[index2] = msgindex;
		break;
		#endif

		default:
			return;
	}

	return;
}

#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_delivery_report_awaited_check
* DESCRIPTION
*  set check bit of msg box in delivery report list  
*
* PARAMETERS

* RETURNS
*
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_set_delivery_report_awaited_check(U16 delivery_report_awaited_index, U8 msg_ref, U8 status_type)
{
	U16 list_index = delivery_report_awaited_index;
	U8 mr = msg_ref;
	U8 st =  status_type;
	int i =0;
	mmi_frm_sms_delivery_report_box_struct* box = mmi_frm_sms_delivery_report_awaited[list_index].ptr;

	if(st != ST_COMP_MSG_RECV_BY_SME)
		mmi_frm_sms_delivery_report_awaited[list_index].result = st;

	for(i=0;i<mmi_frm_sms_delivery_report_awaited[list_index].box_number;i++)	
	{
		if(box[i].mr == mr)
		{
			box[i].check_bit = 1;
			return;
		}			
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_delivery_report_awaited_mr
* DESCRIPTION
*  set message reference of msg box in delivery report list  
*
* PARAMETERS

* RETURNS
*
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_set_delivery_report_awaited_mr(U16 delivery_report_awaited_index , U8 msg_ref, U8 seq_number)
{
	U8 mr = msg_ref;
	U8 seq_no =  seq_number;
	U16 list_index = delivery_report_awaited_index;
	mmi_frm_sms_delivery_report_box_struct* box;
	
	if(seq_no < mmi_frm_sms_delivery_report_awaited[list_index].box_number)
	{
		box = mmi_frm_sms_delivery_report_awaited[list_index].ptr;
		box[seq_no].mr = mr;		
	}
	
	return;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_delivery_report_awaited_complete
* DESCRIPTION
*  check if entry of delivery report awaited list is complete
*
* PARAMETERS

* RETURNS
*
* GLOBALS AFFECTED
*  none
*****************************************************************************/
BOOL mmi_frm_sms_check_delivery_report_awaited_complete(U16 delivery_report_awaited_index)
{
	U16 list_index = delivery_report_awaited_index;
	int i =0;
	mmi_frm_sms_delivery_report_box_struct* box = mmi_frm_sms_delivery_report_awaited[list_index].ptr;

	for(i=0;i<mmi_frm_sms_delivery_report_awaited[list_index].box_number;i++)	
	{
		if(box[i].check_bit == 0)
			return FALSE;
	}
	
	return TRUE;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_remove_delivery_report_awaited
* DESCRIPTION
*  remove delivery report list entry with the specified list index  
*
* PARAMETERS

* RETURNS
*  index of delivery_report_awaited list
* MMI_FRM_SMS_ACTION_SIZE means not found
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_remove_delivery_report_awaited(U16 delivery_report_awaited_list_index)
{
	U16 list_index = delivery_report_awaited_list_index;;

	mmi_frm_sms_delivery_report_awaited[list_index].result = 0;
	mmi_frm_sms_delivery_report_awaited[list_index].ref = 0;
	mmi_frm_sms_delivery_report_awaited[list_index].box_number = 0;
	if (mmi_frm_sms_delivery_report_awaited[list_index].ptr != NULL)
	{
		OslMfree(mmi_frm_sms_delivery_report_awaited[list_index].ptr);
		mmi_frm_sms_delivery_report_awaited[list_index].ptr = NULL;
	}	
	return;
}



/*****************************************************************************
* FUNCTION
mmi_frm_sms_get_delivery_report_index_by_mr
* DESCRIPTION
*  check if delivery report box with the specified message reference already in list
*
* PARAMETERS

* RETURNS
*  index of delivery_report_awaited list
* MMI_FRM_SMS_ACTION_SIZE means not found
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U16 mmi_frm_sms_get_delivery_report_index_by_mr(U8 mr)
{
	U8 msg_reference = mr;
	int list_index=0;

	for( list_index=0 ; list_index<MMI_FRM_SMS_ACTION_SIZE ; list_index++)
	{
		if(mmi_frm_sms_delivery_report_awaited[list_index].box_number != 0)
		{
			int box_no = mmi_frm_sms_delivery_report_awaited[list_index].box_number;
			int box_index = 0;
			mmi_frm_sms_delivery_report_box_struct* box = mmi_frm_sms_delivery_report_awaited[list_index].ptr;
			for(box_index=0;box_index<box_no;box_index++)
			{
				if (box[box_index].mr == msg_reference)
					return list_index;
			}			
		}
	}

	return list_index;

}

/*****************************************************************************
* FUNCTION
mmi_frm_sms_get_delivery_report_index_by_ref
* DESCRIPTION
*  check if delivery report entry with the specified reference number already in list
*
* PARAMETERS

* RETURNS
*  index of delivery_report_awaited list
* MMI_FRM_SMS_ACTION_SIZE means not found
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U16 mmi_frm_sms_get_delivery_report_index_by_ref(U16 ref)
{
	U16 reference_no = ref;
	int list_index=0;

	for( list_index=0 ; list_index<MMI_FRM_SMS_ACTION_SIZE ; list_index++)
	{
		if(mmi_frm_sms_delivery_report_awaited[list_index].box_number != 0)
		{
			if (mmi_frm_sms_delivery_report_awaited[list_index].ref == reference_no)
				return list_index;
		}
	}
	
	return list_index;
}

/*****************************************************************************
* FUNCTION
* 	mmi_frm_sms_add_delivery_report_awaited
* DESCRIPTION
*  add entry to delivery report awaited list
*
* PARAMETERS
*
* RETURNS
*  ref 			: reference number
*  total_segment 	: number of total segments
*  index : segment number -1 
*  mr 	: message reference
*
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_add_delivery_report_awaited(U16 reference_number, U8 total_segment, U8 index, U8 mr)
{
	U16 ref_no = reference_number;
	U8 total_seg = total_segment;
	U8 box_index = index;
	U8 msg_reference = mr;
	mmi_frm_sms_delivery_report_box_struct* box;
		
	if(mmi_frm_sms_delivery_report_awaited[mmi_frm_sms_delivery_report_awaited_next_available].box_number != 0)
		mmi_frm_sms_remove_delivery_report_awaited(mmi_frm_sms_delivery_report_awaited_next_available);

	mmi_frm_sms_delivery_report_awaited[mmi_frm_sms_delivery_report_awaited_next_available].ref = ref_no;
	mmi_frm_sms_delivery_report_awaited[mmi_frm_sms_delivery_report_awaited_next_available].box_number = total_seg;
	mmi_frm_sms_delivery_report_awaited[mmi_frm_sms_delivery_report_awaited_next_available].result = ST_COMP_MSG_RECV_BY_SME;
	mmi_frm_sms_delivery_report_awaited[mmi_frm_sms_delivery_report_awaited_next_available].ptr = OslMalloc(total_seg*sizeof(mmi_frm_sms_delivery_report_box_struct));
	memset((S8*)mmi_frm_sms_delivery_report_awaited[mmi_frm_sms_delivery_report_awaited_next_available].ptr , 0, total_seg*sizeof(mmi_frm_sms_delivery_report_box_struct));
	box = mmi_frm_sms_delivery_report_awaited[mmi_frm_sms_delivery_report_awaited_next_available].ptr;
	box[box_index].mr = msg_reference;		

	mmi_frm_sms_delivery_report_awaited_next_available++;

	if(mmi_frm_sms_delivery_report_awaited_next_available == MMI_FRM_SMS_ACTION_SIZE)
		mmi_frm_sms_delivery_report_awaited_next_available = 0;

	return;
}

/*****************************************************************************
* FUNCTION
* 	mmi_frm_sms_get_delivery_report_awaited_result
* DESCRIPTION
*  get result of delivery report awaited list entry
*
* PARAMETERS
*
* RETURNS
*  delivery_report_awaited_index : index of delivery report list  
*
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_get_delivery_report_awaited_result( U16 delivery_report_awaited_index)
{
	return mmi_frm_sms_delivery_report_awaited[delivery_report_awaited_index].result;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_delivery_report_awaited
* DESCRIPTION
*  entry of check delivery report awaited list
*
* PARAMETERS

* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_check_delivery_report_awaited(MMI_FRM_SMS_SEND_MSG_RSP_STRUCT *inMsg)
{
	MMI_FRM_SMS_SEND_MSG_RSP_STRUCT *msgRsp	=	inMsg;
	U16 delivery_report_awaited_index;
	U16 ref = msgRsp->concat_info.ref;
	U8 msg_reference = msgRsp->mr;		

	if(msgRsp->result == TRUE) // success
	{

		/*check if mr is in delivery report awaited list*/
		delivery_report_awaited_index = mmi_frm_sms_get_delivery_report_index_by_mr(msg_reference);

		/*mr is in delivery report  awaited list*/
		while(delivery_report_awaited_index != MMI_FRM_SMS_ACTION_SIZE)
		{
			mmi_frm_sms_remove_delivery_report_awaited(delivery_report_awaited_index);

			/*check if mr is in delivery report awaited list*/
			delivery_report_awaited_index = mmi_frm_sms_get_delivery_report_index_by_mr(msg_reference);
		}

		/* Single-segment msg is always added into awaited list.  Ref of single-segment msg is always zero. */
		if(msgRsp->concat_info.total_seg == 1)
			delivery_report_awaited_index = MMI_FRM_SMS_ACTION_SIZE;
		else
			delivery_report_awaited_index = mmi_frm_sms_get_delivery_report_index_by_ref(ref);			
		
		/* reference number not in awaited list, add an entry to delivery report awaited list*/
		if(delivery_report_awaited_index == MMI_FRM_SMS_ACTION_SIZE)
		{
			U8 total_seg = msgRsp->concat_info.total_seg;
			U8 box_index = msgRsp->concat_info.seg-1; 	// start from 0
			
			/*add entry to delivery report awaited list*/
			mmi_frm_sms_add_delivery_report_awaited(ref,total_seg, box_index, msg_reference);
		}
		/*reference number is in awaited list*/
		else
		{
			U8 box_index = msgRsp->concat_info.seg-1; 	// start from 0

			/*add mr to delivery report awaited list*/
			mmi_frm_sms_set_delivery_report_awaited_mr(delivery_report_awaited_index, msg_reference, box_index);			
		}		
	}
	/*Send_RSP fail*/
	else
	{
		/* Single-segment msg should not be taken care. */
		if(msgRsp->concat_info.total_seg == 1)
			delivery_report_awaited_index = MMI_FRM_SMS_ACTION_SIZE;
		else
			delivery_report_awaited_index = mmi_frm_sms_get_delivery_report_index_by_ref(ref);						
		
		/* reference number in awaited list*/
		while(delivery_report_awaited_index != MMI_FRM_SMS_ACTION_SIZE)
		{		
			mmi_frm_sms_remove_delivery_report_awaited(delivery_report_awaited_index);			
			delivery_report_awaited_index = mmi_frm_sms_get_delivery_report_index_by_ref(ref);
		}
	}
	
}
#endif // __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_handle_extension_char
* DESCRIPTION
*  added '1b' before extionsion char
*  caller must make sure the dstBuf size is at least twice more than srcBuf size
* PARAMETERS
*  charNum : number of char in srcBuffer
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_handle_extension_char(U16* dstBuf, U16* srcBuf,U16 charNum )
{
	U16* dstBuffer = dstBuf;
	U16* srcBuffer = srcBuf;
	U16 charNumber = charNum;
	U16 c;
	U16 srcCount=0;
	U16 dstCount=0;

	while(srcCount < charNumber)
	{
		c = srcBuffer[srcCount];
		if(UI_TEST_GSM_EXTENDED(c))
		{
			dstBuffer[dstCount] = EMS_ESCAPE_CHARACTER;
			dstCount++;
		}
		dstBuffer[dstCount] = c;
		srcCount++;
		dstCount++;
	}

	dstBuffer[dstCount] = '\0';

	return;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_UCS2
* DESCRIPTION
*  check if UCS2 char exists
* PARAMETERS
*  charNum : number of char in buffer
* RETURNS
*  return true if UCS2 char exists, else return false
* GLOBALS AFFECTED
*  none
*****************************************************************************/
BOOL mmi_frm_sms_check_UCS2(U16* buf , U16 charNum)
{
	U16* buffer = buf;
	U16 charNumber = charNum;
	U16 c;
	U16 count=0;

	while(count < charNumber)
	{
		c = buffer[count];
		if(UI_TEST_UCS2_CHARACTER(c))
			return KAL_TRUE;
		count++;
	}

	return KAL_FALSE;
}



/*****************************************************************************
 * FUNCTION
 *  mmi_frm_sms_check_dst_number
 * DESCRIPTION
 *  check if the number is valid
 * PARAMETERS
 *  num         [?]         dst number
 *  numLen      [IN]        number length
 * RETURNS
 *  return true if number is valid, else return false
 *****************************************************************************/
BOOL mmi_frm_sms_check_dst_number(U8 *num, U8 numLen)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *number = num;
    U8 numberlen = numLen;
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (; i < numberlen; i++)
    {
        U8 digit = number[i];

        if (digit == 'p' || digit == 'P' || digit == 'w' || digit == 'W')
        {
            return KAL_FALSE;
        }
    }

    return KAL_TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_init_msgbox
* DESCRIPTION
*  Init message box
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_init_msgbox (void)
{
	U8 i=0;
	
	memset((S8*)mmi_frm_sms_inbox_list, MMI_FRM_SMS_INVALID_NUM, sizeof(U16)*g_frm_sms_cntx.mmi_frm_sms_msg_box_size);
	memset((S8*)mmi_frm_sms_outbox_list, MMI_FRM_SMS_INVALID_NUM, sizeof(U16)*g_frm_sms_cntx.mmi_frm_sms_msg_box_size);
#ifdef __MMI_MESSAGES_DRAFT_BOX__
	memset((S8*)mmi_frm_sms_drafts_list, MMI_FRM_SMS_INVALID_NUM, sizeof(U16)*g_frm_sms_cntx.mmi_frm_sms_msg_box_size);
#endif
	memset((S8*)mmi_frm_sms_msg_box, MMI_FRM_SMS_INVALID_NUM, sizeof(mmi_frm_sms_msgbox_struct)*g_frm_sms_cntx.mmi_frm_sms_msg_box_size);
	memset((S8*)mmi_frm_sms_index_list, MMI_FRM_SMS_INVALID_NUM, sizeof(mmi_frm_sms_index_struct)*g_frm_sms_cntx.mmi_frm_sms_msg_box_size);
	memset((S8*)mmi_frm_sms_awaited, MMI_FRM_SMS_INVALID_NUM, sizeof(mmi_frm_sms_deliver_msg_struct)*(MMI_FRM_SMS_ACTION_SIZE+1));
	memset((S8*)mmi_frm_sms_awaited_counter, MMI_FRM_SMS_INVALID_NUM, sizeof(U8)*(MMI_FRM_SMS_ACTION_SIZE));
	memset((S8*)mmi_frm_sms_data, MMI_FRM_SMS_INVALID_NUM, sizeof(mmi_frm_sms_deliver_data_struct)*(MMI_FRM_SMS_ACTION_SIZE+1));	
	memset((S8*)mmi_frm_sms_data_counter, MMI_FRM_SMS_INVALID_NUM, sizeof(U8)*(MMI_FRM_SMS_ACTION_SIZE));
	memset((S8*)mmi_frm_sms_content, 0, sizeof(mmi_frm_sms_content_struct)*(MMI_FRM_SMS_MSG_CONTENT_NUM+1));

#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
	memset((S8*)mmi_frm_sms_delivery_report_awaited, 0, sizeof(mmi_frm_sms_delivery_report_list_struct)*(MMI_FRM_SMS_ACTION_SIZE));
#endif 
	for(i=0; i<(MMI_FRM_SMS_MSG_CONTENT_NUM+1); i++)
	{
		mmi_frm_sms_content[i].segment=MMI_FRM_SMS_INVALID_NUM;
		mmi_frm_sms_content[i].nextindex=MMI_FRM_SMS_INVALID_NUM;
	}
	mmi_frm_sms_inbox_size=0;
	mmi_frm_sms_outbox_size=0;
#ifdef __MMI_MESSAGES_DRAFT_BOX__
	mmi_frm_sms_drafts_size=0;
#endif
        /* reset message icon in the status bar in case of sim refresh */    
        g_msg_cntx.msg_ind_in_idle=FALSE;
#ifndef __MMI_MULTI_SIM__
	mmi_msg_set_msg_icon(FALSE, FALSE);
#else
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		g_msg_cntx.msg_full_ind[i] = FALSE;
		MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_FALSE, MTPNP_FALSE, i);
	}
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_SC_number
* DESCRIPTION
*  check if the SMS SC is valid
* PARAMETERS
*  num         [?]         SC number
*  numLen      [IN]        SC number length
* RETURNS
*  return true if number is valid, else return false
*****************************************************************************/
BOOL mmi_frm_sms_check_SC_number(U8 *num, U8 numLen)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if (numLen == 1 && num[0] == '+')
	{
		return KAL_FALSE;
	}
	return mmi_frm_sms_check_dst_number(num, numLen);
}

#ifdef __MMI_MULTI_SIM__
U8 gSMSActiveSIMID = 0;
void mmi_frm_sms_set_active_simid(U8 nSimID)
{
	MMI_ASSERT(nSimID < MMI_SIM_NUMBER);
	gSMSActiveSIMID = nSimID;
	mmi_trace(g_sw_SMS, "SMS: mmi_frm_sms_set_active_simid, gSMSActiveSIMID is %d",gSMSActiveSIMID);
}
U8 mmi_frm_sms_get_active_simid(void)
{
	mmi_trace(g_sw_SMS, "SMS: mmi_frm_sms_get_active_simid, gSMSActiveSIMID is %d",gSMSActiveSIMID);
	return gSMSActiveSIMID;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_frm_sms_handle_sms_refresh
 * DESCRIPTION
 *  handle sms refresh and remove sms entry:
 * PARAMETERS
 *  index       [IN]        Index
 *  b  addindex    addindex(?)
 *  c  segment     segment(?)
 * RETURNS
 *  index(?)
 *****************************************************************************/
void mmi_frm_sms_handle_sms_refresh(U16 index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 previndex, nextindex;
	U16 type = mmi_frm_sms_index_list[index].prevtype;
	U16 i = 0;
	U16 j = 0;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	previndex = mmi_frm_sms_index_list[index].previndex;
	nextindex = mmi_frm_sms_index_list[index].nextindex;

	if (previndex == MMI_FRM_SMS_INVALID_INDEX)
	{
		return;
	}

	/* chat message is not in message box, so message box should remain unchanged and only delete index list entry */
	/* For message with only one segment, also update msg box and inbox/outbox/draft list */
	if ((type != MMI_FRM_SMS_NOBOX && type != MMI_FRM_SMS_AWAITS && type != MMI_FRM_SMS_DATA) &&
		(nextindex == MMI_FRM_SMS_INVALID_INDEX))
	{        
		switch (type)
		{
			case MMI_FRM_SMS_INBOX:
			case MMI_FRM_SMS_UNREAD:
			while (i < mmi_frm_sms_inbox_size)
			{
				if (mmi_frm_sms_inbox_list[i] == previndex)
				{
					break;
				}
				i++;
			}
			for (j = i; j < (mmi_frm_sms_inbox_size - 1); j++)
			{
				mmi_frm_sms_inbox_list[j] = mmi_frm_sms_inbox_list[j + 1];
			}
			mmi_frm_sms_inbox_list[j] = MMI_FRM_SMS_INVALID_INDEX;
			mmi_frm_sms_inbox_size--;
			break;

			case MMI_FRM_SMS_OUTBOX:
			#ifndef __MMI_MESSAGES_DRAFT_BOX__
			case MMI_FRM_SMS_DRAFTS:
			#endif 
			while (i < mmi_frm_sms_outbox_size)
			{
				if (mmi_frm_sms_outbox_list[i] == previndex)
				{
					break;
				}
				i++;
			}
			for (j = i; j < (mmi_frm_sms_outbox_size - 1); j++)
			{
				mmi_frm_sms_outbox_list[j] = mmi_frm_sms_outbox_list[j + 1];
			}
			mmi_frm_sms_outbox_list[j] = MMI_FRM_SMS_INVALID_INDEX;
			mmi_frm_sms_outbox_size--;
			break;

			#ifdef __MMI_MESSAGES_DRAFT_BOX__
			case MMI_FRM_SMS_DRAFTS:
			while (i < mmi_frm_sms_drafts_size)
			{
				if (mmi_frm_sms_drafts_list[i] == previndex)
				{
					break;
				}
				i++;
			}
			for (j = i; j < (mmi_frm_sms_drafts_size - 1); j++)
			{
				mmi_frm_sms_drafts_list[j] = mmi_frm_sms_drafts_list[j + 1];
			}
			mmi_frm_sms_drafts_list[j] = MMI_FRM_SMS_INVALID_INDEX;
			mmi_frm_sms_drafts_size--;
			break;
			#endif /* __MMI_MESSAGES_DRAFT_BOX__ */ 
			#ifdef __MMI_UNIFIED_MESSAGE__
			case MMI_FRM_SMS_UNSENT:
			while (i < mmi_frm_sms_unsent_size)
			{
				if (mmi_frm_sms_unsent_list[i] == previndex)
				{
					break;
				}
				i++;
			}
			for (j = i; j < (mmi_frm_sms_unsent_size - 1); j++)
			{
				mmi_frm_sms_unsent_list[j] = mmi_frm_sms_unsent_list[j + 1];
			}
			mmi_frm_sms_unsent_list[j] = MMI_FRM_SMS_INVALID_INDEX;
			mmi_frm_sms_unsent_size--;
			break;
			#endif /* __MMI_UNIFIED_MESSAGE__ */         
			#ifdef __UNIFIED_MESSAGE_SIMBOX_SUPPORT__
			case MMI_FRM_SMS_SIM:
			if (mmi_frm_sms_sim_list != NULL)
			{
				while (i < mmi_frm_sms_simbox_size)
				{
					if (mmi_frm_sms_sim_list[i] == previndex)
					{
						break;
					}
					i++;
				}
				for (j = i; j < (mmi_frm_sms_simbox_size - 1); j++)
				{
					mmi_frm_sms_sim_list[j] = mmi_frm_sms_sim_list[j + 1];
				}
				mmi_frm_sms_sim_list[j] = MMI_FRM_SMS_INVALID_INDEX;
				mmi_frm_sms_simbox_size--;
			}
			break;
			#endif /* __UNIFIED_MESSAGE_SIMBOX_SUPPORT__ */ 
		}
		memset((S8*) & mmi_frm_sms_msg_box[previndex], 0xff, sizeof(mmi_frm_sms_msgbox_struct));
	}
	else
	{
		/* the first segment of message */
		if (type != MMI_FRM_SMS_NOBOX)
		{
			/* for message in awaited list, ex. chat msg,  modify start index of awaitd list entry to next index */
			if (type == MMI_FRM_SMS_AWAITS || type == MMI_FRM_SMS_DATA)
			{
				mmi_frm_sms_awaited[previndex].index = nextindex;
			}

			/* for message in msg box, ex. inbox/outbox/draft  message, modify start index of msg box entry to next index  */
			else
			{
				mmi_frm_sms_msg_box[previndex].startindex = nextindex;
			}
		}
		/* the 2nd or after segment of message */
		else
		{
			mmi_frm_sms_index_list[previndex].nextindex = nextindex;
		}

		if (nextindex != MMI_FRM_SMS_INVALID_INDEX)
		{
			mmi_frm_sms_index_list[nextindex].prevtype = type;
			mmi_frm_sms_index_list[nextindex].previndex = previndex;
		}
	}
	memset((S8*) & mmi_frm_sms_index_list[index], 0xff, sizeof(mmi_frm_sms_index_struct));
}


U8 mmi_frm_sms_record_get_sim_id(U16 box, U16 index)
{
	U8 nSimID = 0;
	U16 startindex = index;
	U16 endindex = index + 1;
	U16 entryindex = 0;
	U16 L4index = 0;
	U16 currIndex = 0;

	if (box & MMI_FRM_SMS_AWAITS)
	{
		entryindex = index;
		L4index = entryindex;
		nSimID = mmi_frm_msg_get_simid_by_l4_index(L4index);
	}

	if ((box & MMI_FRM_SMS_INBOX) || (box & MMI_FRM_SMS_UNREAD))
	{
		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_inbox_size;
			currIndex = g_msg_cntx.currBoxIndex;
			entryindex = mmi_frm_sms_inbox_list[currIndex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
		else
		{
			entryindex = mmi_frm_sms_inbox_list[startindex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
	}
	if ((box & MMI_FRM_SMS_OUTBOX)
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		|| (box & MMI_FRM_SMS_DRAFTS)
	#endif
	)
	{
		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_outbox_size;
			currIndex = g_msg_cntx.currBoxIndex;
			entryindex = mmi_frm_sms_outbox_list[currIndex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
		else
		{
			entryindex = mmi_frm_sms_outbox_list[startindex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
	}
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	if ((box & MMI_FRM_SMS_DRAFTS))
	{
		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_drafts_size;
			currIndex = g_msg_cntx.currBoxIndex;
			entryindex = mmi_frm_sms_drafts_list[currIndex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
		else
		{
			entryindex = mmi_frm_sms_drafts_list[startindex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
	}
	#endif /* __MMI_MESSAGES_DRAFT_BOX__ */

	#ifdef __MMI_UNIFIED_MESSAGE__
	if ((box & MMI_FRM_SMS_UNSENT))
	{
		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_unsent_size;
			currIndex = g_msg_cntx.currBoxIndex;
			entryindex = mmi_frm_sms_unsent_list[currIndex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
		else
		{
			entryindex = mmi_frm_sms_unsent_list[startindex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
	}
	#endif /* __MMI_UNIFIED_MESSAGE__ */

	#ifdef __UNIFIED_MESSAGE_SIMBOX_SUPPORT__
	if ((box & MMI_FRM_SMS_SIM))
	{
		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_simbox_size;
			currIndex = g_msg_cntx.currBoxIndex;
			entryindex = mmi_frm_sms_sim_list[currIndex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
		else
		{
			entryindex = mmi_frm_sms_sim_list[startindex];
			nSimID = mmi_frm_msg_get_simid_by_l4_index(mmi_frm_sms_msg_box[entryindex].startindex);
		}
	}
	#endif /* __UNIFIED_MESSAGE_SIMBOX_SUPPORT__ */

	return nSimID;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_frm_msg_entry_send_from_SIM_Select
 * DESCRIPTION
 *  Entry send only screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_frm_msg_entry_send_from_SIM_Select(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/    

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if (mmi_frm_sms_check_action_pending() == TRUE)
	{
		if(g_pSendData!=NULL)
		{
			if (g_pSendData->string!= NULL)
			{
				OslMfree(g_pSendData->string);
				g_pSendData->string = NULL;
			}
			if(g_pSendData->filename != NULL)
			{
				OslMfree(g_pSendData->filename);
				g_pSendData->filename =NULL;
			}
			OslMfree(g_pSendData);
			g_pSendData = NULL;
		}
		g_callback(NULL, (module_type) g_mod_src, MMI_FRM_SMS_NOTREADY);       
	}
	else
	{
		U8 reqSrcMod = GetReqSrcModIndex();
		MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
		MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, reqSrcMod);
		g_pSendData->sendrequire &= ~MMI_FRM_SMS_DISP_SIM_OPT;
		mmi_msg_set_msg_need_select_sim_when_send(FALSE);
		SetLeftSoftkeyFunction(UI_dummy_function, KEY_EVENT_UP);
		mmi_frm_sms_send_sms(g_callback, g_mod_src, g_pSendData);
		if(g_pSendData!=NULL)
		{
			OslMfree(g_pSendData);
			g_pSendData = NULL;
		}
	}
			
}

/*****************************************************************************
 * FUNCTION
 *  mmi_msg_highlight_send_from_SIM_Select
 * DESCRIPTION
 *  Highlight send from SIM select
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_highlight_send_from_SIM_Select(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/    

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	mmi_msg_highlight_generic(STR_GLOBAL_OK, IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								mmi_frm_msg_entry_send_from_SIM_Select,
								GoBackHistory);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_msg_entry_Select_option_screen
 * DESCRIPTION
 *  enter sms option screen
 * PARAMETERS
 *  void        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_frm_msg_entry_Select_option_screen(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 nStrItemList[2];        /* Stores the strings id of submenus returned */
	U16 nNumofItem;             /* Stores no of children in the submenu */
	U8 *guiBuffer;              /* Buffer holding history data */
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	EntryNewScreen(SCR_MTPNP_SMS_SELECT_OPTION, NULL, NULL, NULL);

	/* 2 Get current screen to gui buffer  for history purposes */
	guiBuffer = GetCurrGuiBuffer(SCR_MTPNP_SMS_SELECT_OPTION);

	MTPNP_AD_SetMenuHideUnhide(MENU_MTPNP_SMS_SELECT_SIM1_OPTION);
	
	/* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU_MTPNP_SMS_SELECT_OPTION);

	/* 4. Get attribute of menu to be displayed */
	//nDispAttribute = GetDispAttributeOfItem(MENU8234_FDL_VIEW_OPTIONS_MAIN  );
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU_MTPNP_SMS_SELECT_OPTION, nStrItemList);
	//GetSequenceImageIds(SLAVE_MENU_CC_MO_SELETECT_NET     , nImageItemList);

	/* 6 Set current parent id */
	SetParentHandler(MENU_MTPNP_SMS_SELECT_OPTION);
	/* 7 Register highlight handler to be called in menu screen */
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif
	/* 8 Display Category15 Screen */
	ShowCategory15Screen(STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION,
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							nNumofItem, nStrItemList, (PU16) gIndexIconsImageList, LIST_MENU, 0, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

U8 mmi_frm_sms_get_multi_sms_bitmap(U16 type, U16 index, U8 *l4_index, U8 nSimID)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 i = 0;
	U16 startindex = index;
	U16 endindex = index + 1;
	U16 entryindex = 0;
	U16 L4index = 0;

	if(index != MMI_FRM_SMS_INVALID_INDEX) //delete one message
	{
		msgbox_type = 0x30; //default type for delete one message
	}
	else
	{
		msgbox_type = type;
	}
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if (mmi_frm_sms_list_done == FALSE)
	{
		return FALSE;
	}

	if (type & MMI_FRM_SMS_NOBOX)
	{
		mmi_frm_sms_set_bitmap(index, l4_index);
	}

	if (type & MMI_FRM_SMS_AWAITS)
	{
		for (i = startindex; i < endindex; i++)
		{
			entryindex = index;
			L4index = entryindex;
			while (L4index != MMI_FRM_SMS_INVALID_INDEX &&mmi_frm_msg_get_simid_by_l4_index(L4index) == nSimID)
			{
				mmi_frm_sms_set_bitmap(L4index, l4_index);
				L4index = mmi_frm_sms_index_list[L4index].nextindex;
			}
		}
	}

	if ((type & MMI_FRM_SMS_INBOX) || (type & MMI_FRM_SMS_UNREAD))
	{
		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_inbox_size;
		}
		for (i = startindex; i < endindex; i++)
		{
			entryindex = mmi_frm_sms_inbox_list[i];
			L4index = mmi_frm_sms_msg_box[entryindex].startindex;
			while (L4index != MMI_FRM_SMS_INVALID_INDEX &&mmi_frm_msg_get_simid_by_l4_index(L4index) == nSimID)
			{
				mmi_frm_sms_set_bitmap(L4index, l4_index);
				L4index = mmi_frm_sms_index_list[L4index].nextindex;
			}
		}
	}
	if ((type & MMI_FRM_SMS_OUTBOX)
	#ifndef __MMI_MESSAGES_DRAFT_BOX__
		|| (type & MMI_FRM_SMS_DRAFTS)
	#endif 
	)
	{
		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_outbox_size;
		}
		for (i = startindex; i < endindex; i++)
		{
			entryindex = mmi_frm_sms_outbox_list[i];
			L4index = mmi_frm_sms_msg_box[entryindex].startindex;
			while (L4index != MMI_FRM_SMS_INVALID_INDEX &&mmi_frm_msg_get_simid_by_l4_index(L4index) == nSimID)
			{
				mmi_frm_sms_set_bitmap(L4index, l4_index);
				L4index = mmi_frm_sms_index_list[L4index].nextindex;
			}
		}
	}
#ifdef __MMI_MESSAGES_DRAFT_BOX__
	if (type & MMI_FRM_SMS_DRAFTS)
	{
		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_drafts_size;
		}
		for (i = startindex; i < endindex; i++)
		{
			entryindex = mmi_frm_sms_drafts_list[i];
			L4index = mmi_frm_sms_msg_box[entryindex].startindex;
			while (L4index != MMI_FRM_SMS_INVALID_INDEX &&mmi_frm_msg_get_simid_by_l4_index(L4index) == nSimID)
			{
				mmi_frm_sms_set_bitmap(L4index, l4_index);
				L4index = mmi_frm_sms_index_list[L4index].nextindex;
			}
		}
	}
#endif /* __MMI_MESSAGES_DRAFT_BOX__ */

#ifdef __MMI_UNIFIED_MESSAGE__
	if (type & MMI_FRM_SMS_UNSENT)
	{
	#ifdef __UNIFIED_MESSAGE_BACKGROUND_SEND_SUPPORT__
		BOOL isOnlySending = TRUE;
	#endif

		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_unsent_size;
		}
		for (i = startindex; i < endindex; i++)
		{
			entryindex = mmi_frm_sms_unsent_list[i];
		#ifdef __UNIFIED_MESSAGE_BACKGROUND_SEND_SUPPORT__
			if (mmi_frm_sms_msg_box[entryindex].send_status != MMI_FRM_SMS_SENDING)
			{
				isOnlySending = FALSE;
				mmi_frm_sms_msg_box[entryindex].send_status = MMI_FRM_SMS_FAILTOSEND;
		#endif /* __UNIFIED_MESSAGE_BACKGROUND_SEND_SUPPORT__ */
				L4index = mmi_frm_sms_msg_box[entryindex].startindex;
				while (L4index != MMI_FRM_SMS_INVALID_INDEX && mmi_frm_msg_get_simid_by_l4_index(L4index) == nSimID)
				{
					mmi_frm_sms_set_bitmap(L4index, l4_index);
					L4index = mmi_frm_sms_index_list[L4index].nextindex;
				}
		#ifdef __UNIFIED_MESSAGE_BACKGROUND_SEND_SUPPORT__
			}
		#endif
		}
	#ifdef __UNIFIED_MESSAGE_BACKGROUND_SEND_SUPPORT__
		if (isOnlySending && mmi_frm_sms_unsent_size > 0)
		{
			return (U8)MMI_FRM_SMS_NOT_DELETE_SENDING;
		}
	#endif /* __UNIFIED_MESSAGE_BACKGROUND_SEND_SUPPORT__ */
	}
#endif /* __MMI_UNIFIED_MESSAGE__ */ 

#ifdef __UNIFIED_MESSAGE_SIMBOX_SUPPORT__
	if ((type & MMI_FRM_SMS_SIM))
	{
		if (index == MMI_FRM_SMS_INVALID_INDEX)
		{
			startindex = 0;
			endindex = mmi_frm_sms_simbox_size;
		}
		if (mmi_frm_sms_sim_list != NULL)
		{
			for (i = startindex; i < endindex; i++)
			{
				entryindex = mmi_frm_sms_sim_list[i];
				L4index = mmi_frm_sms_msg_box[entryindex].startindex;
				while (L4index != MMI_FRM_SMS_INVALID_INDEX &&mmi_frm_msg_get_simid_by_l4_index(L4index) == nSimID)
				{
					mmi_frm_sms_set_bitmap(L4index, l4_index);
					L4index = mmi_frm_sms_index_list[L4index].nextindex;
				}
			}
		}
	}
#endif /* __UNIFIED_MESSAGE_SIMBOX_SUPPORT__ */ 

	return TRUE;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_msg_entry_select_save_option_screen
 * DESCRIPTION
 *  enter save sms option screen
 * PARAMETERS
 *  void        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_entry_select_save_option_screen(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 nStrItemList[2];        /* Stores the strings id of submenus returned */
	U16 nNumofItem;             /* Stores no of children in the submenu */
	U8 *guiBuffer;              /* Buffer holding history data */

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	EntryNewScreen(SCR_MTPNP_SMS_SAVE_SELECT_OPTION, NULL, NULL, NULL);

	/* 2 Get current screen to gui buffer  for history purposes */
	guiBuffer = GetCurrGuiBuffer(SCR_MTPNP_SMS_SAVE_SELECT_OPTION);
	
	MTPNP_AD_SetMenuHideUnhide(MENU_MTPNP_SMS_SELECT_SAVE_SIM1_OPTION);
	/* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU_MTPNP_SMS_SELECT_SAVE_OPTION);

	/* 4. Get attribute of menu to be displayed */
	//      nDispAttribute = GetDispAttributeOfItem(MENU8234_FDL_VIEW_OPTIONS_MAIN  );
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU_MTPNP_SMS_SELECT_SAVE_OPTION, nStrItemList);
	//GetSequenceImageIds(SLAVE_MENU_CC_MO_SELETECT_NET     , nImageItemList);

	/* 6 Set current parent id */
	SetParentHandler(MENU_MTPNP_SMS_SELECT_SAVE_OPTION);
	/* 7 Register highlight handler to be called in menu screen */
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif
	/* 8 Display Category15 Screen */
	ShowCategory15Screen(STR_GLOBAL_OPTIONS, 0,
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						nNumofItem, nStrItemList, (PU16) gIndexIconsImageList, LIST_MENU, 0, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

/*****************************************************************************
 * FUNCTION
 *  mmi_frm_msg_entry_save_from_SIM_select
 * DESCRIPTION
 *  Entry save only screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_frm_msg_entry_save_from_SIM_select(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/    

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if (mmi_frm_sms_check_action_pending() == TRUE)
	{
		if(g_pSendData!=NULL)
		{
			if (g_pSendData->string!= NULL)
			{
				OslMfree(g_pSendData->string);
				g_pSendData->string = NULL;
			}
			if(g_pSendData->filename != NULL)
			{
				OslMfree(g_pSendData->filename);
				g_pSendData->filename =NULL;
			}
			OslMfree(g_pSendData);
			g_pSendData = NULL;
		}
		g_callback(NULL, (module_type) g_mod_src, MMI_FRM_SMS_NOTREADY);       
	}
	else
	{
		U8 reqSrcMod = GetReqSrcModIndex();
		MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
		MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, reqSrcMod);
		g_pSendData->sendrequire &= ~MMI_FRM_SMS_DISP_SIM_OPT;
		SetLeftSoftkeyFunction(UI_dummy_function, KEY_EVENT_UP);
		mmi_frm_sms_save_sms(g_callback, g_mod_src, g_pSendData);
		if(g_pSendData!=NULL)
		{
			OslMfree(g_pSendData);
			g_pSendData = NULL;
		}
	}
			
}

/*****************************************************************************
 * FUNCTION
 *  mmi_msg_highlight_save_from_SIM_select
 * DESCRIPTION
 *  Highlight save from SIM select
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_highlight_save_from_SIM_select(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/    

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	mmi_msg_highlight_generic(STR_GLOBAL_OK, IMG_GLOBAL_OK,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								mmi_frm_msg_entry_save_from_SIM_select,
								GoBackHistory);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_frm_msg_set_send_data
 * DESCRIPTION
 *  Entry send only screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_frm_msg_set_send_data(PsFuncPtrU16 callback, module_type mod_src, mmi_frm_sms_send_struct *sendData)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/    

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	g_pSendData = sendData;
	g_callback = callback;
	g_mod_src = mod_src;

}

#endif /*__MMI_MULTI_SIM__*/
#endif


