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
 * SmsSettingsGuiInterface.C
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: SmsSettingsGuiInterface.c

  	PURPOSE		: it allows user to access the following settings,
					 - service center number, 
					 - validity period,
					 - message type,
					 - prefer storage
					 - memory status,
					 - reply path,
					 - deliver report 

 

	AUTHOR		: Magesh, hiran

	DATE		: 01-01-2003

**************************************************************/

#ifdef __MOD_SMSAL__
//
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
#include "wgui_categories_inputs.h"
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
//
#include "gsm7bitdefaultalphabet.h"
#include "wgui_status_icons.h"
//
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
//
#include "csp.h"
#include "settingprofile.h"
#include "phonebookgprot.h"
#include "settingprot.h"
#include "alarmframeworkprot.h"

#include "gbhmi.h" //added by zhoumn for the bug of editor full @2007/06/16

//
#undef  __NEWSIMULATOR

#include "smsapi.h"
#include "smsstruct.h"

// add by wuzc for debug
#include "mmi_trace.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim_call.h"
#include "dual_sim.h"
#include "mtpnp_ad_resdef.h"
extern MMI_BOOL mmi_frm_test_menu_item_hide(U16 menu_item_id);
#endif

static U8 DeliveryRepyStates[2];
static msg_profile_edit_struct psProfileEditInfo;		// attn: reduce one of this struct obj....
static msg_profile_list_struct psProfileNameList;
static PU8 vpItems[MAX_LFOUR_VP];
static PU8 msgTypeItems[MAX_LFOUR_MSG_TYPE];
static U16 defaultProfileNames[MAX_LFOUR_PROFILES] =
{	
	STR_SET0_MENUENTRY,
	STR_SET1_MENUENTRY,
	STR_SET2_MENUENTRY,
	STR_SET3_MENUENTRY
};
static U16 profileEditIcons[MAX_PROFILE_EDIT_ITEMS] =
{	
	IMG_SMS_PROFILE_NAME,
	IMG_SMS_PROFILE_SC,
	IMG_SMS_PROFILE_VP,
	IMG_SMS_PROFILE_MSG_TYPE
};
U16 profileEditStrings[MAX_PROFILE_EDIT_ITEMS];
static U8 *pMailBoxName[MAX_MAILBOX_PS_SUPPORT]={NULL, NULL};
static U8 *pMailBoxNumber[MAX_MAILBOX_PS_SUPPORT]={NULL, NULL};
static U8 psMailBoxDCS[MAX_MAILBOX_PS_SUPPORT];
static msg_mailbox_edit_struct psMailboxEditInfo;
#if defined(__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__) 
static msg_mem_status_struct memStatus = {SMSAL_STORAGE_UNSPECIFIED, 0, 0, 0, 0};
#endif // defined(__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__) 
#ifdef __MMI_GPRS_FEATURES__
static msg_bearer_enum preferBearer;
#endif


#ifdef __MMI_MULTI_SIM__
extern U8 mmi_frm_sms_get_active_simid(void);
extern void mmi_frm_sms_set_active_simid(U8 nSimID);
#endif
/********************************************************************

						variables

********************************************************************/
extern msg_active_profile_struct activeProfile;

extern U8	hintData[MAX_SUB_MENUS][MAX_SUB_MENU_HINT_SIZE];
extern PU8 	hintDataPtrs[MAX_SUB_MENUS];
extern U8	subMenuData[MAX_SUB_MENUS][MAX_SUB_MENU_SIZE];
extern PU8	subMenuDataPtrs[MAX_SUB_MENUS];
extern wgui_inline_item wgui_inline_items[];
extern void MakeCall(PS8 strNumber);
extern U16 GetTotalCallCount(void);

void mmi_msg_dial_mailbox_rsp (void* mailbox, module_type mod, U16 result)
{
	U8 nSimID = 0;
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	if(result==MMI_FRM_SMS_OK)
	{
		mmi_frm_sms_mailbox_struct* mailboxlist = (mmi_frm_sms_mailbox_struct*)mailbox;
		U8 i=0;
		if(mailboxlist->mailboxNumber==0)
			DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);		
		else
		{
			while(i<mailboxlist->mailboxNumber)
			{
				pMailBoxNumber[i]=OslMalloc((MAX_CC_ADDR_LEN*ENCODING_LENGTH)+ENCODING_LENGTH);
				pfnUnicodeStrcpy((S8*)pMailBoxNumber[i], (S8*)mailboxlist->pMailBoxNumber[i]);
				if(pfnUnicodeStrlen((PS8)pMailBoxNumber[i])!=0)
					break;
				i++; 
			}
			if(i<MAX_MAILBOX_PS_SUPPORT)
			{
			#ifdef __MMI_MULTI_SIM__
				MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, nSimID);
			#endif /* __MMI_MULTI_SIM__ */
				MakeCall((PS8)pMailBoxNumber[i]);
			}
			else
			{
				if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)
					DisplayPopup ((PU8)GetString(STR_VOICE_MAIL_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
				else
					DisplayPopup ((PU8)GetString (STR_VM_NO_NUMBER_TO_DIAL), IMG_GLOBAL_WARNING, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);
			}

			FreeVoiceMailData();	
		}
	}
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
}

void mmi_msg_dial_mailbox_req (void)
{
	ClearAllKeyHandler();
#ifdef __MMI_TOUCH_DIAL_SCREEN__
	SetKeyHandler(DialVoiceMail,KEY_1,KEY_LONG_PRESS);
#endif
	mmi_frm_sms_get_mailbox_info(mmi_msg_dial_mailbox_rsp, MOD_MMI);
}

void mmi_msg_get_mailbox_list_rsp (void* mailbox, module_type mod, U16 result)
{
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
	if(result==MMI_FRM_SMS_OK)
	{
		mmi_frm_sms_mailbox_struct* mailboxlist = (mmi_frm_sms_mailbox_struct*)mailbox;
		U8 i=0;
    psMailboxEditInfo.malboxNameLength=mailboxlist->mailboxNameLength;
  	if(mailboxlist->mailboxNumber==0)
			DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);		
  	else
  	{
  			for(i=0; i<mailboxlist->mailboxNumber; i++)
  			{
  				pMailBoxName[i]=OslMalloc((MAX_PROFILE_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH);
  				pfnUnicodeStrcpy((S8*)pMailBoxName[i], (S8*)mailboxlist->pMailBoxName[i]);

  				pMailBoxNumber[i]=OslMalloc((MAX_CC_ADDR_LEN*ENCODING_LENGTH)+ENCODING_LENGTH);
  				pfnUnicodeStrcpy((S8*)pMailBoxNumber[i], (S8*)mailboxlist->pMailBoxNumber[i]);

  				psMailBoxDCS[i]=mailboxlist->mailboxNameDcs[i];
  			}
  	 }
	  			mmi_msg_entry_voice_mail_server();
  }		
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
}

void mmi_msg_get_mailbox_list_req (void)
{
	ClearAllKeyHandler();
	mmi_frm_sms_get_mailbox_info(mmi_msg_get_mailbox_list_rsp, MOD_MMI);
}

void mmi_msg_set_profile_active_rsp(void* index, module_type mod_src, U16 result)
{
	psProfileNameList.currReqProfileNo=*((U8*)index);
	DisplayPopup((PU8)GetString(STR_GLOBAL_SAVED),IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	DeleteMessagesHistoryNodes();		
}

void mmi_msg_set_profile_active_req(U8* index)
{
	ClearAllKeyHandler();
	mmi_frm_sms_set_profile_active(mmi_msg_set_profile_active_rsp, MOD_MMI, index);
}

void mmi_msg_save_profile_active (void)
{
	ClearAllKeyHandler();
	mmi_msg_set_profile_active_req((U8*)&(g_msg_cntx.currHiliteIndex));
}

void mmi_msg_set_mailbox_inline_item_rsp(void* mailboxdetail, module_type mod, U16 result)
{
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_VOICE_MAIL_OPTION);
	if(result==MMI_FRM_SMS_OK)
	{
		msg_mailbox_edit_struct* mailbox=(msg_mailbox_edit_struct*)mailboxdetail;
		pfnUnicodeStrcpy((S8*)pMailBoxName[mailbox->mailboxIndex], (S8*)mailbox->mailboxName);
		pfnUnicodeStrcpy((S8*)pMailBoxNumber[mailbox->mailboxIndex], (S8*)mailbox->mailboxAddress);
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	}
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
  mmi_msg_entry_voice_mail_server();
  DeleteMessagesHistoryNodes ();
    
	}


void mmi_msg_set_mailbox_inline_item_req(void)
{
	ClearAllKeyHandler();
	mmi_frm_sms_set_mailbox_info(mmi_msg_set_mailbox_inline_item_rsp, MOD_MMI, (void*)&psMailboxEditInfo);
}

void mmi_msg_dial_mailbox_address (void)
{
	if(pfnUnicodeStrlen((PS8)pMailBoxNumber[g_msg_cntx.currHiliteIndex])!=0)
		#ifndef __MMI_MULTI_SIM__
		MakeCall((PS8)pMailBoxNumber[g_msg_cntx.currHiliteIndex]);
		#else
		mmi_ucm_app_entry_dial_option((U8*)pMailBoxNumber[g_msg_cntx.currHiliteIndex], NULL, NULL);
		#endif
	else
		DisplayPopup ((PU8)GetString (STR_VM_NO_NUMBER_TO_DIAL), IMG_GLOBAL_WARNING, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);
}

void mmi_msg_get_profile_list_rsp (void* profilelist, module_type mod, U16 result)
{
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
	if(result==MMI_FRM_SMS_OK)
	{
		memset(&psProfileNameList, 0, sizeof(msg_profile_list_struct));
		memcpy((S8*)&psProfileNameList, (S8*)profilelist, sizeof(msg_profile_list_struct));
		if(psProfileNameList.currMaxPsProfiles>0)
			mmi_msg_entry_profile_settings();
		else
			DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);		
	}
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
}

void mmi_msg_get_profile_list_req (void)
{
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = mmi_frm_sms_get_active_simid();
	#endif
	ClearAllKeyHandler();
	mmi_frm_sms_get_profile_list(mmi_msg_get_profile_list_rsp, MOD_MMI, reqSrcMod);
}

void mmi_msg_get_profile_highlight_index (S32 nIndex)
{
	g_msg_cntx.currHiliteIndex=nIndex;
	if(g_msg_cntx.currHiliteIndex!=psProfileNameList.currReqProfileNo)
	{
		mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, mmi_msg_save_profile_active, Messages2GoBackHistory);
		ChangeRightSoftkey (STR_GLOBAL_DONE, IMG_GLOBAL_BACK);
		SetRightSoftkeyFunction (mmi_msg_entry_confirm_generic, KEY_EVENT_UP);
	}
	else
{
		ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
		SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	}
}
	
void mmi_msg_set_profile_inline_item_rsp(void* profiledetail, module_type mod, U16 result)
{
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROFILE_EDIT);
	if(result==MMI_FRM_SMS_OK)
	{
		msg_profile_edit_struct* profile=(msg_profile_edit_struct*)profiledetail;
		pfnUnicodeStrcpy((S8*)&psProfileNameList.profileName[psProfileEditInfo.profileIndex], (S8*)profile->profileName);
		switch(profile->vpIndex)
	{
			case SMSAL_VP_1_HR:		psProfileEditInfo.vpIndex	=	0;	break;
			case SMSAL_VP_12_HR:	psProfileEditInfo.vpIndex	=	1;	break;	
			case SMSAL_VP_24_HR:	psProfileEditInfo.vpIndex	=	2;	break;
			case SMSAL_VP_1_WEEK:	psProfileEditInfo.vpIndex	=	3;	break; 
			case SMSAL_VP_MAX:		psProfileEditInfo.vpIndex	=	4;	break; 
			default:					psProfileEditInfo.vpIndex	=	4;	break;
	}	

		switch(profile->msgTypeIndex)
	{
			case SMSAL_DEFAULT_PID:		psProfileEditInfo.msgTypeIndex	=	0;	break;
			case SMSAL_G3_FAX_PID:		psProfileEditInfo.msgTypeIndex	=	1;	break;
			case SMSAL_PAGING_PID:		psProfileEditInfo.msgTypeIndex	=	2;	break;
			case SMSAL_EMAIL_PID:		psProfileEditInfo.msgTypeIndex	=	3;	break;
			default:						psProfileEditInfo.msgTypeIndex	=	0;	break;
		}
		DisplayPopup ((PU8)GetString(STR_GLOBAL_SAVED), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	}
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
	DeleteMessagesHistoryNodes();
	}	

void mmi_msg_set_profile_inline_item_req(void)
{
	if(!IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP, CSP_Validity_Period))
	{
		switch((U8)psProfileEditInfo.vpIndex)
		{
			case 0:	psProfileEditInfo.vpIndex	=	SMSAL_VP_1_HR;	break;	
			case 1:	psProfileEditInfo.vpIndex	=	SMSAL_VP_12_HR;	break;
			case 2:	psProfileEditInfo.vpIndex	=	SMSAL_VP_24_HR;	break;
			case 3:	psProfileEditInfo.vpIndex	=	SMSAL_VP_1_WEEK;	break;
			case 4:	psProfileEditInfo.vpIndex	=	SMSAL_VP_MAX;		break;
			default:	psProfileEditInfo.vpIndex	=	SMSAL_VP_MAX;		break;
		}	
	}
	if(!IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP, CSP_Protocol_ID))
	{
		switch((U8)psProfileEditInfo.msgTypeIndex)
		{
			case 0:	psProfileEditInfo.msgTypeIndex	=	SMSAL_DEFAULT_PID;	break;
			case 1:	psProfileEditInfo.msgTypeIndex	=	SMSAL_G3_FAX_PID;	break;
			case 2:	
			if(!IsResetCSPGroupService(CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP,CSP_SM_MO_PAGING))
			{			psProfileEditInfo.msgTypeIndex	=	SMSAL_PAGING_PID;	break;	}
			case 3:
			if(!IsResetCSPGroupService(CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP,CSP_SM_MO_EMAIL))	
			{			psProfileEditInfo.msgTypeIndex	=	SMSAL_EMAIL_PID; 	break;	}
			default:	psProfileEditInfo.msgTypeIndex	=	SMSAL_DEFAULT_PID;	break;
		}
	}
	ClearAllKeyHandler ();
	mmi_frm_sms_set_profile_detail(mmi_msg_set_profile_inline_item_rsp, MOD_MMI, (void*)&psProfileEditInfo);
}

void mmi_msg_get_profile_inline_item_rsp(void* profiledetail, module_type mod, U16 result)
{
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
	if(result==MMI_FRM_SMS_OK)
	{
		memcpy((S8*)&psProfileEditInfo, (S8*)profiledetail, sizeof(msg_profile_edit_struct));
		psProfileEditInfo.profileIndex=g_msg_cntx.currHiliteIndex;
		switch(psProfileEditInfo.vpIndex)
		{
			case SMSAL_VP_1_HR:		psProfileEditInfo.vpIndex	=	0;	break;
			case SMSAL_VP_12_HR:	psProfileEditInfo.vpIndex	=	1;	break;	
			case SMSAL_VP_24_HR:	psProfileEditInfo.vpIndex	=	2;	break;
			case SMSAL_VP_1_WEEK:	psProfileEditInfo.vpIndex	=	3;	break; 
			case SMSAL_VP_MAX:		psProfileEditInfo.vpIndex	=	4;	break; 
			default:					psProfileEditInfo.vpIndex	=	4;	break;
		}

		switch(psProfileEditInfo.msgTypeIndex)
		{
			case SMSAL_DEFAULT_PID:		psProfileEditInfo.msgTypeIndex	=	0;	break;
			case SMSAL_G3_FAX_PID:		psProfileEditInfo.msgTypeIndex	=	1;	break;
			case SMSAL_PAGING_PID:		psProfileEditInfo.msgTypeIndex	=	2;	break;
			case SMSAL_EMAIL_PID:		psProfileEditInfo.msgTypeIndex	=	3;	break;
			default:						psProfileEditInfo.msgTypeIndex	=	0;	break;
		}
		mmi_msg_entry_profile_edit();
	}
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
}

void mmi_msg_get_profile_inline_item_req(void)
{
	ClearAllKeyHandler();
	// 在进入profile编辑时不让MMI向下发送请求，也就是不让进入编辑框
	 
	mmi_frm_sms_get_profile_detail(mmi_msg_get_profile_inline_item_rsp, MOD_MMI, (U8)g_msg_cntx.currHiliteIndex); 
}

void mmi_msg_get_common_settings_rsp(void* data, module_type mod, U16 result)
{
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
	{
		/*add start by wuzc for debug*/
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_get_common_settings_rsp, be interrupted");
		/*add by start for debug*/
		return;
	}
#ifndef MMI_ON_HARDWARE_P
	mmi_msg_entry_common_settings();
#else
	if(result == MMI_FRM_SMS_OK)
	{
		U8* DrRp=(U8*)data;
		DeliveryRepyStates[0]=DrRp[0];
		DeliveryRepyStates[1]=DrRp[1];
		
		/*add start by wuzc for debug*/
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_get_common_settings_rsp, OK");
		/*add by start for debug*/
		mmi_msg_entry_common_settings();		
	}
	else
		DisplayPopup ((PU8)GetString (STR_MESSAGES_UNKNOWN_ERROR), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	
#endif
}

void mmi_msg_get_common_settings_req(void)
{
	ClearAllKeyHandler();
	mmi_frm_sms_get_common_settings(mmi_msg_get_common_settings_rsp, MOD_MMI);
}


void mmi_msg_set_common_settings_rsp(void* dummy, module_type mod, U16 result)
{
#ifndef MMI_ON_HARDWARE_P
	DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	DeleteMessagesHistoryNodes();
#else
	if(result == MMI_FRM_SMS_OK)
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
	DeleteMessagesHistoryNodes();
#endif
}

void mmi_msg_set_common_settings_req(void)
{
	ClearAllKeyHandler();
	mmi_frm_sms_set_common_settings(mmi_msg_set_common_settings_rsp, MOD_MMI, DeliveryRepyStates);
}

#ifdef __MMI_GPRS_FEATURES__
void mmi_msg_get_bearer_service_rsp (void* bearer, module_type mod, U16 result)
{
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
#ifndef MMI_ON_HARDWARE_P
	mmi_msg_entry_msg_settings();
#else
	if(result == MMI_FRM_SMS_OK)
	{
		preferBearer = (msg_bearer_enum)(*(U8*)bearer);
		mmi_msg_entry_msg_settings();
	}
	else
		DisplayPopup ((PU8)GetString (STR_MESSAGES_UNKNOWN_ERROR), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
#endif
}

void mmi_msg_get_bearer_service_req (void)
{
	ClearAllKeyHandler();
	mmi_frm_sms_get_preferred_bearer(mmi_msg_get_bearer_service_rsp, MOD_MMI);
}

void mmi_msg_set_bearer_service_rsp (void* dummy, module_type mod, U16 result)
{
#ifndef MMI_ON_HARDWARE_P
	preferBearer=g_msg_cntx.currHiliteIndex;
	DisplayPopup ((PU8)GetString (STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	DeleteMessagesHistoryNodes ();
#else
	if(result == MMI_FRM_SMS_OK)
{
		preferBearer = (msg_bearer_enum)g_msg_cntx.currHiliteIndex;
		DisplayPopup ((PU8)GetString (STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
		DeleteMessagesHistoryNodes ();
	}
	else
	{
		DisplayPopup ((PU8)GetString (STR_MESSAGES_UNKNOWN_ERROR), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteMessagesHistoryNodes ();
	}
#endif
}

void mmi_msg_set_bearer_service_req (U8 bearer)
{
	ClearAllKeyHandler();
	mmi_frm_sms_set_preferred_bearer(mmi_msg_set_bearer_service_rsp, MOD_MMI, bearer);
}
#endif //__MMI_GPRS_FEATURES__


#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
void mmi_msg_get_preferred_storage_rsp (void* storage, module_type mod, U16 result)
{
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
#ifndef MMI_ON_HARDWARE_P
	#ifdef __MMI_GPRS_FEATURES__
		mmi_msg_get_bearer_service_req();
	#else
		mmi_msg_entry_msg_settings();
	#endif
#else
	if(result == MMI_FRM_SMS_OK)
	{
		memStatus.prefStorage=*(U8*)storage;
	#ifdef __MMI_GPRS_FEATURES__
		mmi_msg_get_bearer_service_req();
	#else
		mmi_msg_entry_msg_settings();
	#endif
	}
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);		
#endif
}

void mmi_msg_get_preferred_storage_req (void)
{
	ClearAllKeyHandler();
	mmi_frm_sms_get_preferred_storage(mmi_msg_get_preferred_storage_rsp, MOD_MMI);
}

void mmi_msg_set_preferred_storage_rsp (void* storage, module_type mod, U16 result)
{
#ifndef MMI_ON_HARDWARE_P
	memStatus.prefStorage=g_msg_cntx.currHiliteIndex;
	DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	DeleteMessagesHistoryNodes ();
#else
	if(result == MMI_FRM_SMS_OK)
	{
		memStatus.prefStorage=*(U8*)storage;
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
		DeleteMessagesHistoryNodes ();
	}
	else
	{
		DisplayPopup ((PU8)GetString (STR_MESSAGES_UNKNOWN_ERROR), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteMessagesHistoryNodes ();
	}
#endif
}

void mmi_msg_set_preferred_storage_req (U8 storage)
{
	ClearAllKeyHandler();
	mmi_frm_sms_set_preferred_storage(mmi_msg_set_preferred_storage_rsp, MOD_MMI, storage);
}

void mmi_msg_get_memory_status_rsp(void* data, module_type mod, U16 result)
{
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
#ifndef MMI_ON_HARDWARE_P
	mmi_msg_entry_memory_status();
#else
	if(result == MMI_FRM_SMS_OK)
	{
		msg_mem_status_struct* memorystatus = (msg_mem_status_struct*)data;
		memStatus.simTotal		=	memorystatus->simTotal;
		memStatus.meTotal		=	memorystatus->meTotal;
		memStatus.simUsed		=	memorystatus->simUsed;
		memStatus.meUsed		=	memorystatus->meUsed;
		if(IsScreenPresent(SCR_ID_MSG_MEMORY_STATUS))
			HistoryReplace(SCR_ID_MSG_MEMORY_STATUS, SCR_ID_MSG_MEMORY_STATUS, mmi_msg_entry_memory_status);
		else
		mmi_msg_entry_memory_status();
	}
	else
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	
	AlmEnableExpiryHandler();
#endif
}

void mmi_msg_get_memory_status_req(void)
{
	/*Do not  call ClearAllKeyHandler() if SCR_ID_MSG_MEMORY_STATUS is already in history. Ex. new msg ind pops up when screen is SCR_ID_MSG_MEMORY_STATUS*/
	if(!IsScreenPresent(SCR_ID_MSG_MEMORY_STATUS))
	   ClearAllKeyHandler();
	mmi_frm_sms_get_memory_status(mmi_msg_get_memory_status_rsp, MOD_MMI);
}

void mmi_msg_get_preferred_memory_status_rsp(void* data, module_type mod, U16 result)
{
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
#ifndef MMI_ON_HARDWARE_P
	mmi_msg_entry_preferred_storage();
#else
	if(result == MMI_FRM_SMS_OK)
	{
		msg_mem_status_struct* memorystatus = (msg_mem_status_struct*)data;
		memStatus.simTotal		=	memorystatus->simTotal;
		memStatus.meTotal		=	memorystatus->meTotal;
		memStatus.simUsed		=	memorystatus->simUsed;
		memStatus.meUsed		=	memorystatus->meUsed;
		if(IsScreenPresent(SCR_ID_MSG_PREFERRED_STORAGE))
			HistoryReplace(SCR_ID_MSG_PREFERRED_STORAGE, SCR_ID_MSG_PREFERRED_STORAGE, mmi_msg_entry_preferred_storage);
		else
			mmi_msg_entry_preferred_storage();
	}
	else
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	AlmEnableExpiryHandler();
#endif
}

void mmi_msg_get_preferred_memory_status_req(void)
{
	/*Do not  call ClearAllKeyHandler() if SCR_ID_MSG_PREFERRED_STORAGE is already in history. Ex. new msg ind pops up when screen is SCR_ID_MSG_PREFERRED_STORAGE*/
	if(!IsScreenPresent(SCR_ID_MSG_PREFERRED_STORAGE))
	   ClearAllKeyHandler();
	mmi_frm_sms_get_memory_status(mmi_msg_get_preferred_memory_status_rsp, MOD_MMI);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_save_preferred_storage
* DESCRIPTION
*   Set preferred storage
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_save_preferred_storage (void)
{
	ClearAllKeyHandler ();
	if(((g_msg_cntx.currHiliteIndex==0)&&(memStatus.prefStorage==0))
		||((g_msg_cntx.currHiliteIndex==1)&&(memStatus.prefStorage==1)))
	{
		DisplayPopup ((PU8)GetString (STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
		DeleteMessagesHistoryNodes ();
	}
	else if(((g_msg_cntx.currHiliteIndex==0)&&(memStatus.simTotal==0))||((g_msg_cntx.currHiliteIndex==1)&&(memStatus.meTotal==0)))
	{
		DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT),IMG_GLOBAL_WARNING, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
		DeleteMessagesHistoryNodes ();
	}		
	else
	{
		if(g_msg_cntx.currHiliteIndex==0)
			mmi_msg_set_preferred_storage_req(SMSAL_SM_PREFER);
		else if(g_msg_cntx.currHiliteIndex==1)
			mmi_msg_set_preferred_storage_req(SMSAL_ME_PREFER);
	}	
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_refresh_memory_status
* DESCRIPTION
*   Refresh memory status
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_refresh_memory_status (void)
{
	if(IsScreenPresent(SCR_ID_MSG_MEMORY_STATUS))
		mmi_msg_get_memory_status_req();
	else if(IsScreenPresent(SCR_ID_MSG_PREFERRED_STORAGE))
		mmi_msg_get_preferred_memory_status_req();

}	

#endif //__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__

#ifdef __MMI_GPRS_FEATURES__
/*****************************************************************************
* FUNCTION
*  mmi_msg_save_bearer_settings
* DESCRIPTION
*   Set bearer settings
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_save_bearer_settings (void)
{
	ClearAllKeyHandler ();
	if(g_msg_cntx.currHiliteIndex==preferBearer)
	{
		DisplayPopup ((PU8)GetString (STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
		DeleteMessagesHistoryNodes ();
}
	else
		mmi_msg_set_bearer_service_req((U8)g_msg_cntx.currHiliteIndex);

}
#endif //__MMI_GPRS_FEATURES__

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_profile_inline_item
* DESCRIPTION
*   Set profile items for edit
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_profile_inline_item (void) 
{
	U8	numOfMenuItems	=	2;
	U8	numOfMsgTypes		=	2;
	
	vpItems[0]	=	(PU8) GetString (STR_1HOUR_MENUENTRY);
	vpItems[1]	=	(PU8) GetString (STR_12HOUR_MENUENTRY);
	vpItems[2]	=	(PU8) GetString (STR_24HOUR_MENUENTRY);
	vpItems[3]	=	(PU8) GetString (STR_1WEEK_MENUENTRY);
	vpItems[4]	=	(PU8) GetString (STR_63WEEKS_MENUENTRY);
	// message type
	msgTypeItems[0]	=	(PU8) GetString (STR_TEXT_MENUENTRY);
	msgTypeItems[1]	=	(PU8) GetString (STR_FAX_MENUENTRY);
	if(!IsResetCSPGroupService(CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP,CSP_SM_MO_PAGING))
	{
		msgTypeItems[numOfMsgTypes]	=	(PU8) GetString (STR_PAGE_MENUENTRY);
		numOfMsgTypes++;
	}	
	if(!IsResetCSPGroupService(CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP,CSP_SM_MO_EMAIL))
	{
		msgTypeItems[numOfMsgTypes]	=	(PU8) GetString (STR_MT_EMAIL_MENUENTRY);
		numOfMsgTypes++;
	}	
	// say activate these items
	SetInlineItemActivation(&wgui_inline_items[0], KEY_LSK, KEY_EVENT_UP);
	// profile name 
	SetInlineItemFullScreenEdit(&wgui_inline_items[0], STR_PROFILE_NAME, IMG_SMS_ENTRY_SCRN_CAPTION, (PU8)psProfileEditInfo.profileName,
		/*MAX_PROFILE_NAME_LEN*/psProfileNameList.profileNameLength+1, INPUT_TYPE_USE_ENCODING_BASED_LENGTH | INPUT_TYPE_ONE_LESS_CHARACTER |INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_GSM_EXTENDED_HANDLING);
	SetInlineFullScreenEditCustomFunction(&wgui_inline_items[0], mmi_msg_inline_generic_custom_function);
	// service center
	SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);	
	SetInlineItemTextEdit(&wgui_inline_items[1], (PU8)psProfileEditInfo.scAddress, MAX_DIGITS_SMS,
		INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING);
	DisableInlineItemHighlight(&wgui_inline_items[1]);
	//validity period
	if(!IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP, CSP_Validity_Period))
	{
		SetInlineItemActivation(&wgui_inline_items[numOfMenuItems], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);	
		SetInlineItemSelect(&wgui_inline_items[numOfMenuItems], MAX_LFOUR_VP, vpItems, &psProfileEditInfo.vpIndex );
		numOfMenuItems++;
	}
	// message type	
	if(!IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP, CSP_Protocol_ID))
	{
		SetInlineItemActivation(&wgui_inline_items[numOfMenuItems], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
		SetInlineItemSelect (&wgui_inline_items[numOfMenuItems], numOfMsgTypes, msgTypeItems, &psProfileEditInfo.msgTypeIndex );
		numOfMenuItems++;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_msg_settings
* DESCRIPTION
*   Entry message settings menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_msg_settings (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	U8*	popUpList[MAX_SUB_MENUS];
	/*S32	maskingByte=-1;*/
	EntryNewScreen(SCR_ID_MSG_SETTINGS, mmi_msg_exit_generic, mmi_msg_entry_msg_settings, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: EntryScrMessageSetup");

	guiBuffer = GetCurrGuiBuffer(SCR_ID_MSG_SETTINGS);

	 		
	numItems = GetNumOfChild_Ext(MESSAGES_MENU_MSG_SETTINGS_MENUID);
	GetSequenceStringIds_Ext(MESSAGES_MENU_MSG_SETTINGS_MENUID, nStrItemList);	

	SetParentHandler(MESSAGES_MENU_MSG_SETTINGS_MENUID);	
	ConstructHintsList(MESSAGES_MENU_MSG_SETTINGS_MENUID, popUpList);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);

	SetMessagesCurrScrnID(SCR_ID_MSG_SETTINGS);
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_SETTINGS);

	mmi_msg_set_msg_menu_highlight_handler();	

	ShowCategory52Screen(STR_MESSAGESETUP_MENUENTRY,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, (U8 **)popUpList,
						  			0, 0,	guiBuffer );
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_msg_settings
* DESCRIPTION
*   Before entering message settings menu, get preferred storage and/or bearer settings
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_msg_settings (void)
{
#ifdef MMI_ON_HARDWARE_P
	#if 1
		if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)
		{
			DisplayPopup ((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			return;
		}
		/* disallow re-entering SMS application when there is a pending SMS job running in the background */
		else if(mmi_frm_sms_check_action_pending())
		{
			DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			return;
		}
		
		#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
				mmi_msg_get_preferred_storage_req();
		#elif defined(__MMI_GPRS_FEATURES__)
				mmi_msg_get_bearer_service_req();
		#else
				mmi_msg_entry_msg_settings();
		#endif
		
	#else
		mmi_msg_entry_msg_settings();
	#endif
	/*modified end by wuzc for psi-mmi at 200688*/

#endif // wangbei add 20060617
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_profile_edit_sc_address_empty_handler
* DESCRIPTION
*   sc address is empty hendler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_profile_edit_sc_address_empty_handler(void)
{
	set_right_softkey_label((UI_string_type) get_string(STR_GLOBAL_BACK));
	set_right_softkey_icon(NULL);
	redraw_right_softkey();
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_profile_edit
* DESCRIPTION
*   Entry profile edit screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_profile_edit_highlight_handler (S32 index)
{
	ChangeTitleString((PU8)GetString(profileEditStrings[index]));
	draw_title();
	RegisterInputBoxEmptyFunction(mmi_msg_profile_edit_sc_address_empty_handler); //wangzl: 091012 add for bug 14757
}

void mmi_msg_entry_profile_edit (void)
{
	U8*	guiBuffer=NULL;   	
	U8*	inputBuffer=NULL;                    
	U16	inputBufferSize=0;               

	U8		numOfItems=MAX_PROFILE_EDIT_ITEMS;
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_profile_edit");

	EntryNewScreen(SCR_ID_MSG_PROFILE_EDIT, mmi_msg_exit_profile_edit, NULL, NULL);

	InitializeCategory57Screen ();
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_PROFILE_EDIT);
	inputBuffer = GetCurrNInputBuffer ( SCR_ID_MSG_PROFILE_EDIT, &inputBufferSize);

	RegisterHighlightHandler(mmi_msg_profile_edit_highlight_handler);
	profileEditStrings[0]=STR_PROFILE_NAME;
	profileEditStrings[1]=	STR_PROFILE_SC;
	profileEditStrings[2]=	STR_PROFILE_VP;
	profileEditStrings[3]=	STR_PROFILE_MSG_TYPE;

	if(IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP,CSP_Validity_Period))
		numOfItems--;
	if(IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP,CSP_Protocol_ID))
		numOfItems--;
	
	mmi_msg_set_profile_inline_item();	
	if(guiBuffer!=NULL)
		SetCategory57Data(wgui_inline_items, numOfItems, inputBuffer);

	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_PROFILE_EDIT);
	SetMessagesCurrScrnID(SCR_ID_MSG_PROFILE_EDIT);
	
	mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, mmi_msg_set_profile_inline_item_req, Messages2GoBackHistory);// wangbei del 20060617
	
	//mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, NULL, Messages2GoBackHistory);// wangbei add 20060617
	ShowCategory57Screen(STR_PROFILE_NAME, IMG_SMS_ENTRY_SCRN_CAPTION,
									   STR_GLOBAL_EDIT,	IMG_GLOBAL_OK,
									   STR_GLOBAL_BACK,				IMG_GLOBAL_BACK,
				  					numOfItems, profileEditIcons, wgui_inline_items, 0, guiBuffer);
#if defined(__MMI_GB__)                                                          //add by yaosq 20061110 for leftsoft in profile
	mmi_frm_set_highlight_handler();
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone); 
#endif
	SetCategory57RightSoftkeyFunctions (mmi_msg_entry_confirm_generic, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_exit_profile_edit
* DESCRIPTION
*   Exit profile edit screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   psProfileEditInfo.profileNameDcs, g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes
*****************************************************************************/
void mmi_msg_exit_profile_edit (void)
{
	history_t currHistory; 
	U16 inputBufferSize=0;                //added for inline edit history
	S16 nHistory = 0;

	mmi_trace(g_sw_SMS, "MMI_SMS: ExitScrProfileSetting");
	if(GetUCS2Flag ((PS8)psProfileEditInfo.profileName))
		psProfileEditInfo.profileNameDcs = PHB_UCS2 ;
	else
		psProfileEditInfo.profileNameDcs = PHB_ASCII ;
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_PROFILE_EDIT;
	SetMessagesCurrScrnID(0);
	
	currHistory.scrnID = SCR_ID_MSG_PROFILE_EDIT;
	CloseCategory57Screen();
	currHistory.entryFuncPtr = mmi_msg_entry_profile_edit;
	pfnUnicodeStrcpy((S8*)currHistory.inputBuffer,(S8*)&nHistory);
	GetCategory57History(currHistory.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize( );     	//added for inline edit history
	GetCategory57Data ((U8*) currHistory.inputBuffer );         //added for inline edit history
	AddNHistory(currHistory, inputBufferSize);					//added for inline edit history
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_profile_settings
* DESCRIPTION
*   Entry profile settings screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_profile_settings (void)
{
	PU8	guiBuffer=NULL;
	S32	hiliteitem;
	INT	index=0;
	
	EntryNewScreen(SCR_ID_MSG_PROFILE_LIST, mmi_msg_exit_generic, mmi_msg_entry_profile_settings, NULL);
	
	/*delete start by wuzc for psi-mmi at 200684*/
	#if 0
        
       //为了进入短消息profile Setting屏幕暂时去掉由底层获取的条件值，直接赋值进入菜单。
	#if 0
	for(index=0; (index<psProfileNameList.currMaxPsProfiles)&&(index<MAX_LFOUR_PROFILES); index++)
	{
		if((psProfileNameList.profileName[index][0]=='\0') && (psProfileNameList.profileName[index][1]=='\0') )
			subMenuDataPtrs[index]=(PU8)GetString((U16)defaultProfileNames[index]);
		else
			subMenuDataPtrs[index]=(PU8)psProfileNameList.profileName[index];
	}
	#endif
	// wangbei del end
	 
	for(index=0; index<4; index++)
	{
		subMenuDataPtrs[index]=(PU8)GetString((U16)defaultProfileNames[index]);
	}
	// wangbei add end
	#endif
  	/*delete end by wuzc for psi-mmi at 200684*/

	/*add start by wuzc for psi-mmi at 200684*/
       //为了进入短消息profile Setting
	for(index=0; (index<psProfileNameList.currMaxPsProfiles)&&(index<MAX_LFOUR_PROFILES); index++)
	{
		if((psProfileNameList.profileName[index][0]=='\0') && (psProfileNameList.profileName[index][1]=='\0') )
			subMenuDataPtrs[index]=(PU8)GetString((U16)defaultProfileNames[index]);
		else
			subMenuDataPtrs[index]=(PU8)psProfileNameList.profileName[index];
	}
	/*add end by wuzc for psi-mmi at 200684*/
	
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_PROFILE_LIST);
	if (guiBuffer!=NULL)
		hiliteitem=g_msg_cntx.currHiliteIndex;
	else
		hiliteitem=psProfileNameList.currReqProfileNo;
	RegisterHighlightHandler (mmi_msg_get_profile_highlight_index);

	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROFILE_LIST);
	SetMessagesCurrScrnID(SCR_ID_MSG_PROFILE_LIST);
	
	ShowCategory36Screen( STR_PROFILE_SETTING_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION,
						  				STR_GLOBAL_EDIT, IMG_SMS_COMMON_NOIMAGE,
						  				STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  				index, subMenuDataPtrs , (U16)hiliteitem, guiBuffer);
	SetLeftSoftkeyFunction (mmi_msg_get_profile_inline_item_req, KEY_EVENT_UP);// wangbei del 20060617
	
	//SetLeftSoftkeyFunction (mmi_msg_entry_profile_edit, KEY_EVENT_UP);// wangbei add 20060617
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_common_settings
* DESCRIPTION
*   Entry common settings screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_common_settings (void)
{
	U8*	guiBuffer;
	U16	numItems=2;	

	EntryNewScreen(SCR_ID_MSG_COMMON_SETTINGS, mmi_msg_exit_generic, mmi_msg_entry_common_settings, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: EntryScrCommonSettings");

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_COMMON_SETTINGS);
	subMenuDataPtrs[0] = (PU8)GetString ((U16)STR_DELIVERY_REPORT_MENUENTRY);
	subMenuDataPtrs[1] = (PU8)GetString ((U16)STR_REPLY_PATH_MENUENTRY);

	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_COMMON_SETTINGS);
	SetMessagesCurrScrnID(SCR_ID_MSG_COMMON_SETTINGS);
	/*modified start by wzc for psi-mmi at 200684*/
#if 1
	mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, mmi_msg_set_common_settings_req, Messages2GoBackHistory);// wangbei del 20060617 
#else
	// 暂时这样做的原因是，不需要存储修改过的状态值而直接跳出。
	//mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, mmi_msg_set_common_settings_req, Messages2GoBackHistory);// wangbei del 20060617 
	mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, NULL, Messages2GoBackHistory);// wangbei del 20060617
	if(IsResetCSPGroupService (CSP_TELESERVICES_SERVICE_GROUP,CSP_Del_Conf))
		ResetBit (maskingByte,0);
	if(IsResetCSPGroupService (CSP_TELESERVICES_SERVICE_GROUP,CSP_Reply_path))
		ResetBit (maskingByte,1);
	
	MaskStringItems (subMenuDataPtrs, (U8)numItems, maskingByte);
	numItems=MaskItemsU8 (DeliveryRepyStates, (U8)numItems, maskingByte);
#endif	
	/*modified end by wuzc for psi-mmi at 200684*/   
	
	SetCheckboxToggleRightSoftkeyFunctions (mmi_msg_entry_confirm_generic, GoBackHistory);
	ShowCategory140Screen (STR_COMMONSETTING_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION,
										numItems,  (U8**)subMenuDataPtrs, (U8 *)DeliveryRepyStates, 0, guiBuffer);
	
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_memory_status
* DESCRIPTION
*   Entry memory status screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_memory_status (void)
{
	U8*	guiBuffer;
#if !defined(__MMI_PROJECT_T33__)	    
	int i;
#endif 

	char	tempBuf [MAX_SUBMENU_CHARACTERS];
	char	tempBufUnicode[MAX_SUBMENU_CHARACTERS*ENCODING_LENGTH];
	char	spaceBuf [5];
	S8 memoryStatus[MAX_SUBMENU_CHARACTERS*ENCODING_LENGTH*2];
	msg_mem_status_struct tempmemoryStatus;
	EntryNewScreen(SCR_ID_MSG_MEMORY_STATUS, mmi_msg_exit_generic, mmi_msg_entry_memory_status, NULL);
	
	/*modified start by wuzc for psi-mmi at 200684*/
	memset(tempBuf, 0, MAX_SUBMENU_CHARACTERS);
	memset(tempBufUnicode, 0, MAX_SUBMENU_CHARACTERS*ENCODING_LENGTH);
	memset(memoryStatus, 0, sizeof(memoryStatus));
	/*modified start by wuzc for psi-mmi at 200684*/
	
		memset(spaceBuf, 0, 5);
		AnsiiToUnicodeString (spaceBuf, "\n");

	pfnUnicodeStrcpy(memoryStatus, GetString(STR_SMSSTATUS_TITLE));
		pfnUnicodeStrcat(memoryStatus, spaceBuf);

	pfnUnicodeStrcat(memoryStatus, GetString(STR_SMS_MEMSTATUS_PREFIX_SIM));
	memset(tempBuf, 0, MAX_SUBMENU_CHARACTERS);
	// wangbei add start 20060617
	
	/*modified start by wuzc for psi-mmi at 200684*/
	tempmemoryStatus.simTotal= memStatus.simTotal;
	tempmemoryStatus.simUsed= memStatus.simUsed;
	tempmemoryStatus.meTotal= memStatus.meTotal;	
	tempmemoryStatus.meUsed= memStatus.meUsed;
	tempmemoryStatus.prefStorage=memStatus.prefStorage;
	/*modified end by wuzc for psi-mmi at 200684*/
	
	// wangbei add end
	//if (memStatus.simTotal>0)// wangbei del 20060617
#if defined(__MMI_PROJECT_T33__)	
	if (tempmemoryStatus.simTotal>0)
	{
		sprintf (tempBuf, "%d/%d", tempmemoryStatus.simUsed, tempmemoryStatus.simTotal);
		AnsiiToUnicodeString (tempBufUnicode, tempBuf);
		pfnUnicodeStrcat(memoryStatus, tempBufUnicode);
	}
	else
		pfnUnicodeStrcat(memoryStatus, (S8*)GetString(STR_SMS_DOES_NOT_SUPPORT));
#else
	if (tempmemoryStatus.simTotal>0) // wangbei add 20060617
	{
		//sprintf (tempBuf, "%d%%", ((memStatus.simUsed)*100) / (memStatus.simTotal));// wangbei del 20060617
		sprintf (tempBuf, "%d", ((tempmemoryStatus.simUsed)*100) / (tempmemoryStatus.simTotal));// wangbei add 20060617
		i = strlen(tempBuf);
		tempBuf[i] = '%';
		tempBuf[i+1] = '\0';
		AnsiiToUnicodeString (tempBufUnicode, tempBuf);
		pfnUnicodeStrcat(memoryStatus, tempBufUnicode);
	}
	else
		pfnUnicodeStrcat(memoryStatus, (S8*)GetString(STR_SMS_DOES_NOT_SUPPORT));
#endif	
	pfnUnicodeStrcat(memoryStatus, spaceBuf);

	pfnUnicodeStrncat(memoryStatus, GetString(STR_SMS_MEMSTATUS_PREFIX_PHONE), 16);
	memset(tempBuf, 0, MAX_SUBMENU_CHARACTERS);
#if defined(__MMI_PROJECT_T33__)	
	if (tempmemoryStatus.meTotal>0)
	{
		sprintf (tempBuf, "%d/%d", tempmemoryStatus.meUsed, tempmemoryStatus.meTotal);
		AnsiiToUnicodeString (tempBufUnicode, tempBuf);
		pfnUnicodeStrcat(memoryStatus, tempBufUnicode);
	}
	else
		pfnUnicodeStrcat(memoryStatus, (S8*)GetString(STR_SMS_DOES_NOT_SUPPORT));
#else    
	//if (memStatus.meTotal>0) // wangbei del 20060617
	if (tempmemoryStatus.meTotal>0) // wangbei add 20060617
	{
		//sprintf (tempBuf, "%d%%", ((memStatus.meUsed)*100) / (memStatus.meTotal));// wangbei del 20060617
		sprintf (tempBuf, "%d", ((tempmemoryStatus.meUsed)*100) / (tempmemoryStatus.meTotal)); // wangbei add 20060617
		i = strlen(tempBuf);
		tempBuf[i] = '%';
		tempBuf[i+1] = '\0';
		AnsiiToUnicodeString (tempBufUnicode, tempBuf);
		pfnUnicodeStrcat(memoryStatus, tempBufUnicode);		
	}
	else
		pfnUnicodeStrcat(memoryStatus, (S8*)GetString(STR_SMS_DOES_NOT_SUPPORT));
#endif
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_MEMORY_STATUS);
		if ((guiBuffer!=NULL)||(g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes==SCR_ID_MSG_MEMORY_STATUS))
			DeleteNScrId(SCR_ID_MSG_MEMORY_STATUS);

	SetMessagesCurrScrnID(SCR_ID_MSG_MEMORY_STATUS);
		SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_MEMORY_STATUS);

	ShowCategory7Screen(STR_SMSSTATUS_MENUENTRY,IMG_SMS_ENTRY_SCRN_CAPTION, 0, 0,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, (PU8)memoryStatus, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_preferred_storage
* DESCRIPTION
*   Entry preferred storage screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_preferred_storage (void)
{
	U8*	guiBuffer;
	U16	nNumofItem=2;
	S32	hiliteitem;
	int i;

	char  tempBuf [MAX_SUBMENU_CHARACTERS];
       msg_mem_status_struct tempmemStatus={7, 10, 5, 20, 0};// wangbei add 20060617 // 因为内存的获取需要从底层获取消息，为了显示屏幕暂时将这个数据结构赋值。
	EntryNewScreen(SCR_ID_MSG_PREFERRED_STORAGE, mmi_msg_exit_generic, mmi_msg_entry_preferred_storage, NULL);

	memset(hintData[0], 0, MAX_SUB_MENU_HINT_SIZE);
	memset(hintData[1], 0, MAX_SUB_MENU_HINT_SIZE);

	/*modified start by wuzc for psi-mmi at 200684*/
	memset(tempBuf,0,MAX_SUBMENU_CHARACTERS);
	tempmemStatus.simTotal= memStatus.simTotal;
	tempmemStatus.simUsed=memStatus.simUsed;
	tempmemStatus.meTotal=memStatus.meTotal;	
	tempmemStatus.meUsed=memStatus.meUsed;
	tempmemStatus.prefStorage=memStatus.prefStorage;			
	/*modified end by wuzc for psi-mmi at 200684*/

	
	//这个函数是进入存储状态
	//if (memStatus.simTotal>0) // wangbei del 20060617 //
	if (tempmemStatus.simTotal>0) // wangbei add 20060617
	{
		//sprintf (tempBuf, "%d%%", ((memStatus.simUsed)*100) / (memStatus.simTotal));//wangbei del 20060617
		sprintf (tempBuf, "%d", ((tempmemStatus.simUsed)*100) / (tempmemStatus.simTotal));// wangbei add 20060617
		i = strlen(tempBuf);
		tempBuf[i] = '%';
		tempBuf[i+1] = '\0';
		AnsiiToUnicodeString ((S8*)hintData[0], tempBuf);
	}
	else
		pfnUnicodeStrcpy((S8*)hintData[0], (S8 *)GetString(STR_SMS_DOES_NOT_SUPPORT));

	memset(tempBuf,0,MAX_SUBMENU_CHARACTERS);

	//if (memStatus.meTotal>0)// wangbei del 20060617
	if (tempmemStatus.meTotal>0)// wangbei add 20060617
	{
		//sprintf (tempBuf, "%d%%", ((memStatus.meUsed)*100) / (memStatus.meTotal));//wangbei del 20060617
		sprintf (tempBuf, "%d", ((tempmemStatus.meUsed)*100) / (tempmemStatus.meTotal));// wangbei add 20060617
		i = strlen(tempBuf);
		tempBuf[i] = '%';
		tempBuf[i+1] = '\0';
		AnsiiToUnicodeString ((S8*)hintData[1], tempBuf);
	}
	else
		pfnUnicodeStrcpy((S8*)hintData[1], (S8 *)GetString(STR_SMS_DOES_NOT_SUPPORT));
		
	hintDataPtrs[0]=hintData[0];
	hintDataPtrs[1]=hintData[1];


	subMenuDataPtrs[0] = (PU8)GetString (STR_SIM_STORAGE_MENUENTRY);
	subMenuDataPtrs[1] = (PU8)GetString (STR_ME_STORAGE_MENUENTRY);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_PREFERRED_STORAGE);		 	
	if ((guiBuffer!=NULL)||(g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes==SCR_ID_MSG_PREFERRED_STORAGE))
	{
		DeleteNScrId(SCR_ID_MSG_PREFERRED_STORAGE);
		hiliteitem	=	g_msg_cntx.currHiliteIndex;
	}
	else
	{
		//if(memStatus.prefStorage == 0)//wangbei del 20060617
		if(tempmemStatus.prefStorage == 0)// wangbei add 20060617
			hiliteitem	=	0;
		else
			hiliteitem	=	1;
	}	
	RegisterHighlightHandler (GetHiliteIndex);
	
	SetMessagesCurrScrnID(SCR_ID_MSG_PREFERRED_STORAGE);
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_PREFERRED_STORAGE);

	ShowCategory109Screen (STR_PREFEREDSTORAGE_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION,
									  STR_GLOBAL_OK, IMG_GLOBAL_OK, 
									  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
									  nNumofItem, subMenuDataPtrs, hintDataPtrs,
									  hiliteitem, guiBuffer); 
	SetLeftSoftkeyFunction (mmi_msg_save_preferred_storage, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif //__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__


#ifdef __MMI_GPRS_FEATURES__
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_bearer_settings
* DESCRIPTION
*   Entry bearer settings screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_bearer_settings (void)
{
	U8*	guiBuffer;
	U16	nStrItemList[MAX_SUB_MENUS];
	U16	nNumofItem;
	S32	hiliteitem;


	EntryNewScreen(SCR_ID_MSG_BEARER_SETTINGS, mmi_msg_exit_generic, mmi_msg_entry_bearer_settings, NULL);
	
	guiBuffer	=	GetCurrGuiBuffer (SCR_ID_MSG_BEARER_SETTINGS);	
	nNumofItem	=	GetNumOfChild(SETUP_MENU_PREFEREDBEARER_MENUID);
	GetSequenceStringIds(SETUP_MENU_PREFEREDBEARER_MENUID,nStrItemList);
	SetParentHandler(SETUP_MENU_PREFEREDBEARER_MENUID);
	if (guiBuffer!=NULL)
		hiliteitem=g_msg_cntx.currHiliteIndex;
	else
		hiliteitem=(S32)preferBearer;	
	RegisterHighlightHandler(GetHiliteIndex);

	SetMessagesCurrScrnID(SCR_ID_MSG_BEARER_SETTINGS);
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_BEARER_SETTINGS);
	
	ShowCategory11Screen(STR_PREFEREDBEARER_MENUENTRY,IMG_SMS_ENTRY_SCRN_CAPTION,
						  			STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 			nNumofItem, nStrItemList,(U16)hiliteitem,guiBuffer);

	SetLeftSoftkeyFunction(mmi_msg_save_bearer_settings, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_voice_mail_server
* DESCRIPTION
*   Free voice mail data before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_voice_mail_server (void)
{
	FreeVoiceMailData();
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_voice_mail_server
* DESCRIPTION
*   Free voice mail data before go to idle
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_end_key_from_voice_mail_server (void)
{
	if(!GetTotalCallCount())
		FreeVoiceMailData(); 
	ExecCurrEndKeyDownHandler();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_voice_mail_server
* DESCRIPTION
*   Before entry voice mail server list, get mailbox address
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_voice_mail_server (void)
{
#ifdef MMI_ON_HARDWARE_P
	FreeVoiceMailData();
	ClearAllKeyHandler();
	// wangbei del start 20060619
	#if 1
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)
	{
		DisplayPopup ((PU8)GetString(STR_VOICE_MAIL_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	else if(mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	#endif
	// wangbei del end
#endif
	#ifdef __MMI_MULTI_SIM__
	if (g_msg_cntx.is_voicemail_from_idle == FALSE)
	#endif
	{
		mmi_msg_get_mailbox_list_req();// wangbei del 20060619
	}
	#ifdef __MMI_MULTI_SIM__
	else
	{
		mmi_msg_dial_mailbox_req();
	}
	#endif
     //  mmi_msg_entry_voice_mail_server(); // wangbei add 20060619
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_voice_mail_server
* DESCRIPTION
*   Entry voice mail server list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_voice_mail_server (void)
{
	U8*	guiBuffer=NULL;
	S32	hiliteitem=0;	
	EntryNewScreen(SCR_ID_MSG_VOICE_MAIL_LIST, mmi_msg_exit_generic, mmi_msg_entry_voice_mail_server, NULL);

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_VOICE_MAIL_LIST);		 
	if(guiBuffer!=NULL)
		hiliteitem=(g_msg_cntx.currHiliteIndex<MAX_MAILBOX_PS_SUPPORT)?g_msg_cntx.currHiliteIndex:0;
	RegisterHighlightHandler (GetHiliteIndex);	
	
	SetMessagesCurrScrnID(SCR_ID_MSG_VOICE_MAIL_LIST);		
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_VOICE_MAIL_LIST);
	
	GetCurrEndKeyDownHandler();
	ShowCategory184Screen(STR_VMAIL_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							MAX_MAILBOX_PS_SUPPORT,
							mmi_msg_voice_mail_server_get_item, 
							mmi_msg_voice_mail_server_get_hint , 
							hiliteitem, 
							guiBuffer);
	SetLeftSoftkeyFunction(mmi_msg_entry_voice_mail_server_option, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_entry_voice_mail_server_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(mmi_msg_go_back_from_voice_mail_server, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_go_back_from_voice_mail_server, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_end_key_from_voice_mail_server, KEY_END, KEY_EVENT_DOWN);
	
	#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_HandleSendKeys(mmi_msg_dial_mailbox_address,KEY_EVENT_UP);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_entry_voice_mail_server_option,KEY_ENTER,KEY_EVENT_UP );
#endif
	#else
	SetKeyHandler(mmi_msg_dial_mailbox_address, KEY_SEND, KEY_EVENT_UP);
	#endif

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_voice_mail_server_get_item
* DESCRIPTION
*   Display voice mail server item
*
* PARAMETERS
*  a  IN       current item index
*  b  IN/OUT       current item string
*  c  IN/OUT       current item icon
*  d  IN/OUT       current item mask
* RETURNS
*  TRUE
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_msg_voice_mail_server_get_item (S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask)
{
	if((item_index < 0) || (item_index >= MAX_MAILBOX_PS_SUPPORT))
		return FALSE;
	if(pfnUnicodeStrlen((S8*)pMailBoxName[item_index])!=0)
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)pMailBoxName[item_index]);
	else if(pfnUnicodeStrlen((S8*)pMailBoxNumber[item_index])!=0)
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)pMailBoxNumber[item_index]);
	else
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)GetString((U16)STR_GLOBAL_EMPTY_LIST));
	*img_buff_p = get_image(gIndexIconsImageList[item_index]);
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_voice_mail_server_get_hint
* DESCRIPTION
*   Display voice mail server hint
*
* PARAMETERS
*  a  IN       current item index
*  b  IN/OUT       current item hint
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_msg_voice_mail_server_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	if((pfnUnicodeStrlen((S8*)pMailBoxName[item_index])!=0)
		&&(pfnUnicodeStrlen((S8*)pMailBoxNumber[item_index])!=0))
	{
		pfnUnicodeStrcpy((S8*)hint_array[0], (S8*)pMailBoxNumber[item_index]);
		return 1;
	}
	else
		return 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_voice_mail_server_option
* DESCRIPTION
*   Entry voice mail server option
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_voice_mail_server_option (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	EntryNewScreen(SCR_ID_MSG_VOICE_MAIL_OPTION, mmi_msg_exit_generic, mmi_msg_entry_voice_mail_server_option, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_option_other");
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_VOICE_MAIL_OPTION);
	
	numItems = GetNumOfChild (MENU_ITEM_VOICE_MAIL_OPTION);	
	GetSequenceStringIds(MENU_ITEM_VOICE_MAIL_OPTION, nStrItemList);	
	SetParentHandler (MENU_ITEM_VOICE_MAIL_OPTION);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_VOICE_MAIL_OPTION);
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_VOICE_MAIL_OPTION);
		
	GetCurrEndKeyDownHandler();
	ShowCategory52Screen(STR_VOICE_MAIL_OPTION_ID,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
	SetKeyHandler(mmi_msg_end_key_from_voice_mail_server, KEY_END, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_voice_mail_server_edit
* DESCRIPTION
*   Entry voice mail server edit screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_mailbox_inline_item (void) 
{
	// say activate these items
	SetInlineItemActivation(&wgui_inline_items[0], KEY_LSK, KEY_EVENT_UP);
	// profile name 
	SetInlineItemFullScreenEdit(&wgui_inline_items[0], STR_VOICE_MAIL_NAME, IMG_SMS_ENTRY_SCRN_CAPTION, (PU8)psMailboxEditInfo.mailboxName,
		/*MAX_PROFILE_NAME_LEN*/psMailboxEditInfo.malboxNameLength+1, INPUT_TYPE_USE_ENCODING_BASED_LENGTH | INPUT_TYPE_ONE_LESS_CHARACTER |INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_GSM_EXTENDED_HANDLING);
	SetInlineFullScreenEditCustomFunction(&wgui_inline_items[0], mmi_msg_inline_generic_custom_function);
	// service center
	SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);	
	SetInlineItemTextEdit(&wgui_inline_items[1], (PU8)psMailboxEditInfo.mailboxAddress, MAX_DIGITS_SMS,
		INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING);
	DisableInlineItemHighlight(&wgui_inline_items[1]);
	//validity period
}

void mmi_msg_voice_mail_server_edit_highlight_handler (S32 index)
{
	ChangeTitleString((PU8)GetString(profileEditStrings[index]));
	draw_title();
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_exit_profile_edit
* DESCRIPTION
*   Exit profile edit screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   psProfileEditInfo.profileNameDcs, g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes
*****************************************************************************/
void mmi_msg_exit_voice_mail_server_edit (void)
{
	history_t currHistory; 
	U16 inputBufferSize=0;                //added for inline edit history
	S16 nHistory = 0;
	mmi_trace(g_sw_SMS, "MMI_SMS: ExitScrProfileSetting");

	if(GetUCS2Flag ((PS8)psMailboxEditInfo.mailboxName))
		psMailboxEditInfo.mailboxNameDcs = PHB_UCS2 ;
	else
		psMailboxEditInfo.mailboxNameDcs = PHB_ASCII ;
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_VOICE_MAIL_EDIT;
	SetMessagesCurrScrnID(0);
	
	currHistory.scrnID = SCR_ID_MSG_VOICE_MAIL_EDIT;
	CloseCategory57Screen();
	currHistory.entryFuncPtr = mmi_msg_entry_voice_mail_server_edit;
	pfnUnicodeStrcpy((S8*)currHistory.inputBuffer,(S8*)&nHistory);
	GetCategory57History(currHistory.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize( );     	//added for inline edit history
	GetCategory57Data ((U8*) currHistory.inputBuffer );         //added for inline edit history
	AddNHistory(currHistory, inputBufferSize);					//added for inline edit history
}

void mmi_msg_entry_voice_mail_server_edit (void)
{
	U8*	guiBuffer=NULL;
	U8*	inputBuffer=NULL;                    
	U16	inputBufferSize=0;               
	
	U8		numOfItems=MAX_MAILBOX_PS_SUPPORT;
	
	EntryNewScreen(SCR_ID_MSG_VOICE_MAIL_EDIT, mmi_msg_exit_voice_mail_server_edit, NULL, NULL);
	
	InitializeCategory57Screen ();
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_VOICE_MAIL_EDIT);	
	inputBuffer = GetCurrNInputBuffer ( SCR_ID_MSG_VOICE_MAIL_EDIT, &inputBufferSize);

	RegisterHighlightHandler(mmi_msg_voice_mail_server_edit_highlight_handler);
	profileEditStrings[0]=STR_VOICE_MAIL_NAME;
	profileEditStrings[1]=STR_VOICE_MAIL_NUM;

	psMailboxEditInfo.mailboxIndex=g_msg_cntx.currHiliteIndex;
	psMailboxEditInfo.mailboxNameDcs=psMailBoxDCS[psMailboxEditInfo.mailboxIndex];
	pfnUnicodeStrcpy((S8*)psMailboxEditInfo.mailboxName, (S8*)pMailBoxName[psMailboxEditInfo.mailboxIndex]);
	pfnUnicodeStrcpy((S8*)psMailboxEditInfo.mailboxAddress, (S8*)pMailBoxNumber[psMailboxEditInfo.mailboxIndex]);		
	#ifndef MMI_ON_HARDWARE_P
        psMailboxEditInfo.malboxNameLength = pfnUnicodeStrlen((S8*)psMailboxEditInfo.mailboxName);
	#endif
	mmi_msg_set_mailbox_inline_item();	
	if(guiBuffer!=NULL)
		SetCategory57Data(wgui_inline_items, numOfItems, inputBuffer);

	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_VOICE_MAIL_EDIT);
	SetMessagesCurrScrnID(SCR_ID_MSG_VOICE_MAIL_EDIT);		
	mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, mmi_msg_set_mailbox_inline_item_req, Messages2GoBackHistory);
	ShowCategory57Screen(STR_VMAIL_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION,
									STR_GLOBAL_EDIT,	IMG_GLOBAL_OK,
									STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
				  					numOfItems, profileEditIcons, wgui_inline_items, 0, guiBuffer);
#if defined(__MMI_GB__)
	mmi_frm_set_highlight_handler();
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone); 
#endif

	SetCategory57RightSoftkeyFunctions (mmi_msg_entry_confirm_generic, GoBackHistory);
	SetKeyHandler(mmi_msg_end_key_from_voice_mail_server, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_msg_settings
* DESCRIPTION
*   Handle highlight on message settings menu item
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_msg_settings (void)
{
	#ifdef __MMI_MULTI_SIM__
	U8 reqSrcMod = GetReqSrcModIndex();
	mmi_frm_sms_set_active_simid(reqSrcMod);
	#endif
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
							STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
							mmi_msg_pre_entry_msg_settings, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_profile_settings
* DESCRIPTION
*   Handle highlight on profile settings menu item
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_profile_settings (void)
{
       /*modifid start by wuzc for psi-mmi at 200684*/
	#if 1 //需要向底层发送请求而直接进入短消息"模式设置"子菜单。
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_get_profile_list_req, GoBackHistory);
	#else
	 // wangbei del start 20060617
       // 不需要向底层发送请求而直接进入短消息"模式设置"子菜单。
	/**/
	// wangbei del end
	// wangbei add start 20060617
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_entry_profile_settings, GoBackHistory);
       // wangbei add end
	#endif
	/*modifid end by wuzc for psi-mmi at 200684*/		
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_common_settings
* DESCRIPTION
*   Handle highlight on common settings menu item
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_common_settings (void)
{
	#if 1        // 需要向底层发送请求而直接进入短消息"状态设置"子菜单。
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_get_common_settings_req, GoBackHistory);
	#else
       // wangbei del start 20060617
       // 不需要向底层发送请求而直接进入短消息"状态设置"子菜单。
	// wangbei del end
	// wangbei add start 20060617
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_entry_common_settings, GoBackHistory);
	// wangbei add end
	#endif
}

#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_memory_status
* DESCRIPTION
*   Handle highlight on memory status menu item
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_memory_status (void)
{
	/*modified start by wuzc for psi-mmi at 200684*/
	#if 1 // 不需要向底层发送请求而直接进入短消息"容量查询"子菜单。
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_get_memory_status_req, GoBackHistory);
	#else	// 不需要向底层发送请求而直接进入短消息"容量查询"子菜单。
	// wangbei del end
	// wangbei add start 20060617
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
	STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
	mmi_msg_entry_memory_status, GoBackHistory);
	// wangbei add end
	#endif
	/*modified end by wuzc for psi-mmi200684*/
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_preferred_storage
* DESCRIPTION
*   Handle highlight on preferred storage menu item
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_preferred_storage (void)
{
      /*modified start by wuzc for psi-mmi at 200683*/
       #if 1        // 需要向底层发送请求而直接进入短消息"储存位置"子菜单。
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_get_preferred_memory_status_req, GoBackHistory);
	 #else
       // 不需要向底层发送请求而直接进入短消息"储存位置"子菜单。
	/**/
	// wangbei del end
	// wangbei add start 20060617
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_entry_preferred_storage, GoBackHistory);
       // wangbei add end
	#endif
	/*modified end by wuzc for psi-mmi at 200683*/
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_hint_preferred_storage
* DESCRIPTION
*   Handle hint on preferred storage
*
* PARAMETERS
*  a  IN       current highlight index
* RETURNS
*  none
* GLOBALS AFFECTED
*   hintData
*****************************************************************************/
void mmi_msg_hint_preferred_storage (U16 index)
{
	PU8 StorageStr;
	if(memStatus.prefStorage == 0)
		StorageStr = (PU8)GetString((U16)STR_SIM_STORAGE_MENUENTRY);
	else
		StorageStr = (PU8)GetString((U16)STR_ME_STORAGE_MENUENTRY);
	memset((PS8)hintData[index], 0, sizeof(hintData[index]));
	pfnUnicodeStrcpy((PS8) hintData[index], (PS8)StorageStr);
}
#endif

#ifdef __MMI_GPRS_FEATURES__
/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_bearer_settings
* DESCRIPTION
*   Handle highlight on bearer settings menu item
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_bearer_settings (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_entry_bearer_settings, GoBackHistory);

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_hint_bearer_settings
* DESCRIPTION
*   Handle hint on bearer settings menu item
*
* PARAMETERS
*  a  IN       current highlight index
* RETURNS
*  none
* GLOBALS AFFECTED
*   hintData
*****************************************************************************/
void mmi_msg_hint_bearer_settings (U16 index)
{
	PU8 BearerStr;
	switch (preferBearer)
	{
		case MMI_SMS_PREFER_GPRS:	
			BearerStr = (PU8)GetString((U16)STR_PREFER_GPRS_BEARER_MENUENTRY);
			break;
		case MMI_SMS_PREFER_GSM:
			BearerStr = (PU8)GetString((U16)STR_PREFER_GSM_BEARER_MENUENTRY);
			break;
		case MMI_SMS_GPRS_ONLY:
			BearerStr = (PU8)GetString((U16)STR_GPRS_BEARER_MENUENTRY);
			break;
		case MMI_SMS_GSM_ONLY:
			BearerStr = (PU8)GetString((U16)STR_GSM_BEARER_MENUENTRY);
			break;
		default:
			BearerStr = (PU8)GetString((U16)STR_PREFER_GPRS_BEARER_MENUENTRY);
			break;			
	}
	memset((PS8)hintData[index], 0, sizeof(hintData[index]));
	pfnUnicodeStrcpy((PS8) hintData[index], (PS8)BearerStr);
}

#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_voice_mail_server_edit
* DESCRIPTION
*   Handle highlight on voice mail server edit menu item
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_voice_mail_server_edit (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_voice_mail_server_edit, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_voice_mail_server_dial
* DESCRIPTION
*   Handle highlight on voice mail server dial menu item
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_voice_mail_server_dial (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_dial_mailbox_address, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  ConvertL4NumberStructToDisplay
* DESCRIPTION
 
*
* PARAMETERS
 
* 
 
* RETURNS
*  converted number
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* ConvertL4NumberStructToDisplay (U8 *number, U8 length, U8 type)
{
	U8 *convertedData;
	U8 typeInternational=0;
	mmi_trace(g_sw_SMS, "MMI_SMS: ConvertL4NumberStructToDisplay");
	if(length)
	{
		if (type == CSMCC_INTERNATIONAL_ADDR)
			typeInternational = 1;
		else
			typeInternational = 0;

		convertedData = OslMalloc (((length+typeInternational)*ENCODING_LENGTH)+ENCODING_LENGTH);
	#ifdef __UCS2_ENCODING
		if (typeInternational)
			AnsiiToUnicodeString ((PS8)convertedData, (PS8)"+");
		AnsiiNToUnicodeString ((PS8)convertedData+(typeInternational*ENCODING_LENGTH),
			(PS8)number, length);
		memset((void*)&convertedData[(length+typeInternational)*ENCODING_LENGTH], 0, ENCODING_LENGTH);
	#else
		if (typeInternational)
			pfnUnicodeStrcpy ((PS8)convertedData, (PS8)"+");
		pfnUnicodeStrcpy ((PS8)convertedData+typeInternational, (PS8)number, length);
		convertedData [length+typeInternational] = '\0';
	#endif
	 	return convertedData;
	}
	else
	{
	 	convertedData = OslMalloc (ENCODING_LENGTH);
		memset ((void *)convertedData, 0, ENCODING_LENGTH);
	 	return convertedData;
	}
}

/*****************************************************************************
* FUNCTION
*  DialVoiceMail
* DESCRIPTION
*   Dial voice mail server number
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DialVoiceMail(void)
{
	ClearAllKeyHandler();
#ifdef __MMI_TOUCH_DIAL_SCREEN__
	SetKeyHandler(DialVoiceMail,KEY_1,KEY_LONG_PRESS);
#endif
#ifndef __MMI_MULTI_SIM__
	mmi_msg_dial_mailbox_req();
#else
	if (MTPNP_AD_Get_UsableSide_Number() > 1)
	{
		g_msg_cntx.is_voicemail_from_idle = TRUE;
		mmi_mtpnp_entry_menu(SCR_MTPNP_DM_MSG_VOICE_MAIL, NULL, DialVoiceMail, 
								MENU_MTPNP_DM_VMAIL, STR_VMAIL_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION);
	}
	else if (MTPNP_AD_Get_UsableSide_Number() == 1)
	{
		FreeVoiceMailData();
		mmi_msg_dial_mailbox_req();
	}
	else
	{
		DisplayPopup((PU8) GetString(STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8) ERROR_TONE);
	}
#endif
}

/*****************************************************************************
* FUNCTION
*  FreeVoiceMailData
* DESCRIPTION
*   Free voice mail server data
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   pMailBoxName, pMailBoxNumber
*****************************************************************************/
void FreeVoiceMailData (void)
{
	U8 i=0;
	mmi_trace(g_sw_SMS, "MMI_SMS: FreeVoiceMailData");
	for(i=0; i<MAX_MAILBOX_PS_SUPPORT; i++)
	{
		if(pMailBoxName[i])
		{
			OslMfree (pMailBoxName[i]);
			pMailBoxName[i] = NULL;
		}
		if(pMailBoxNumber[i])
		{
			OslMfree (pMailBoxNumber[i]);
			pMailBoxNumber[i] = NULL;
		}
	}
}

/*****************************************************************************
* FUNCTION
*  DeInitVoiceMailApp
* DESCRIPTION
*   Deinit voice mail application (free voice mail server data)
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DeInitVoiceMailApp(void)
{
	FreeVoiceMailData();
}

#ifndef MMI_ON_HARDWARE_P
void InitSMSSetting(void)
{
	memStatus.prefStorage = SMSAL_SM_PREFER;
	memStatus.simUsed = 1;
	memStatus.simTotal= 10;
	memStatus.meUsed = 0;
	memStatus.meTotal= 10;
#ifdef __MMI_GPRS_FEATURES__
	preferBearer= MMI_SMS_PREFER_GPRS;
#endif
	DeliveryRepyStates[0]=0;
	DeliveryRepyStates[1]=0;
	psProfileNameList.currMaxPsProfiles=1;
	AnsiiToUnicodeString((PS8)psProfileNameList.profileName[0], (PS8)"profile name");
	memcpy((PS8)psProfileEditInfo.profileName, (PS8)psProfileNameList.profileName, 24);
	AnsiiToUnicodeString ((PS8)psProfileEditInfo.scAddress, (PS8)"+12345");
	psProfileEditInfo.vpIndex=4;
	psProfileEditInfo.msgTypeIndex=0;
}
#endif
#endif


