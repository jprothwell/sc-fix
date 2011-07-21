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
 * PhoneBookExtraNumber.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Implementation of feature "Owner Number", "Service Dial Number",
 *   and "SOS number"
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _PHONEBOOKEXTRANUMBER_C
#define _PHONEBOOKEXTRANUMBER_C

#define __NEWSIMULATOR
#include "mmi_trace.h"
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalmenuitems.h"
#include "gsm7bitdefaultalphabet.h"
#include "debuginitdef.h"
#include "globaldefs.h"
#include "eventsgprot.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebooktypes.h"
#include "historygprot.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "protocolevents.h"
#include "unicodexdcl.h"
#include "queuegprot.h"
#include "timerevents.h"
#include "simdetectiongexdcl.h"
#include "commonscreens.h"
#include "callsetup.h"
#include "settingdefs.h" /*for fdn*/
#include "settingsgdcl.h"/*for fdn*/
#include "speeddial.h"
#include "callmanagementgprot.h"
#include "gbhmi.h" //added by zhoumn for the bug of editor full @2007/07/19

#include "settinggprots.h"
#include "securitysetup.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#endif
#undef __NEWSIMULATOR

/* 
** Local Variable
*/
static MMI_PHB_ENTRY_STRUCT	g_phb_sim_list[MAX_SIM_ENTRIES];	/*Maximum entry of MSISDN, SDL and SOS in SIM*/
static U16	g_phb_sim_list_count;
U16 g_phb_read_index;
//static MMI_SOS_NUMBER_STRUCT g_phb_nvram_list[MAX_NVRAM_SOS_ENTRIES];	/*Emergency Number in NVRAM*/
static U8 *g_phb_sos_list[MAX_SIM_SOS_ENTRIES + MAX_NVRAM_SOS_ENTRIES];
static BOOL g_phb_edit_owner = FALSE;
static BOOL g_phb_operate_owner = FALSE;
//static U8 g_phb_sos_nvram_read = 0;
static U16 g_phb_fdn_count[MMI_SIM_NUMBER] = {0, };	/*For FDN Lookup List*/
static U8 g_phb_is_get_entry_rsp = 1; /*Check if already get response for previous request.*/

MMI_EXTRA_NUMBER_CONTEXT_STRUCT g_ExtraNum_context[MMI_SIM_NUMBER];
MMI_EXTRA_NUMBER_CONTEXT_STRUCT *g_pExtraNum_context = g_ExtraNum_context;

extern FDLBDLContext gFDLBDLContext[MMI_SIM_NUMBER];
extern FDLBDLContext *gpFDLBDLContext;
/* 
** Global Variable
*/
extern S8 pbName[];
extern S8 pbNumber[];
extern wgui_inline_item wgui_inline_items[];
extern MMI_OWNER_NUMBER_STRUCT  gDispOwnerNumber[MMI_SIM_NUMBER][2];	/*Owner number associate with Line ID*/

extern BOOL g_FixedInit[];
extern MMI_OWNER_NUMBER_STRUCT *g_pDispOwnerNumber;

extern MMI_PHB_ENTRY_BCD_STRUCT PhoneBook[];
/* 
** Global Function
*/
extern void RefreshDisplayOwnerNumber(U8 lineid);	/*Owner number associate with Line ID*/
extern void MakeCall(PS8);	/*For Service Dial Number*/
extern void GBSetInputboxLSKFunction(FuncPtr f);


#ifdef __MMI_MULTI_SIM__
void mmi_phb_set_extraNum_context(U8 nSimID)
{
	g_pExtraNum_context = &g_ExtraNum_context[nSimID];
	g_pExtraNum_context->simId = nSimID;
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_phb_extra_number_init()
* DESCRIPTION
*   Initialization of Owner Number feature
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_extra_number_init(void) 
{
	U8 i;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_extra_number_init.>\n",__FILE__,__LINE__));

	SetHiliteHandler(MENU_ID_PHB_EXTRA_NUMBER	,mmi_phb_highlight_extra_number);

	
	SetHiliteHandler(MITEM1051_OWNER_NUMBER_ERASE	,mmi_phb_highlight_owner_number_erase);
	SetHiliteHandler(MITEM1052_OWNER_NUMBER_EDIT	,mmi_phb_highlight_owner_number_edit);

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MITEM105_PBOOK_OWNER_NUMBER +i, mmi_phb_highlight_owner_number);
		SetHiliteHandler(MENU_PHONEBOOK_SERVICENUMBER + i, mmi_phb_highlight_sdn_list );
		/*jiabl start:20060905 add 1 line for sos number hilight*/
		SetHiliteHandler(MITEM116_PBOOK_SOS_NUMBER + i, mmi_phb_highlight_sos_list);
		g_ExtraNum_context[i].g_phb_sos_nvram_read = 0;
	}
	
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_extra_number() 
* DESCRIPTION
*   highlight function of extra number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_extra_number(void) 
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_entry_extra_number,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_extra_number, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_extra_number()
* DESCRIPTION
*   entry function of extra number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExtraNumMenuHighLightHandler(S32 hiliteid)
{
	#ifdef __MMI_MULTI_SIM__
	S32 hiliteItemID = 0;
	hiliteItemID = GetSeqItemId_Ext((U16)MENU_ID_PHB_EXTRA_NUMBER, (S16)hiliteid);
	
	switch(hiliteItemID)
	{
		case MITEM105_PBOOK_OWNER_NUMBER:
		case MENU_PHONEBOOK_SERVICENUMBER:
		case MITEM116_PBOOK_SOS_NUMBER:
			mmi_phb_set_extraNum_context(0);
			break;
		case MITEM105_PBOOK_OWNER_NUMBER_2:
		case MENU_PHONEBOOK_SERVICENUMBER_2:
		case MITEM116_PBOOK_SOS_NUMBER_2:
			mmi_phb_set_extraNum_context(1);
			break;
		case MITEM105_PBOOK_OWNER_NUMBER_3:
		case MENU_PHONEBOOK_SERVICENUMBER_3:
		case MITEM116_PBOOK_SOS_NUMBER_3:
			mmi_phb_set_extraNum_context(2);
			break;
		case MITEM105_PBOOK_OWNER_NUMBER_4:
		case MENU_PHONEBOOK_SERVICENUMBER_4:
		case MITEM116_PBOOK_SOS_NUMBER_4:
			mmi_phb_set_extraNum_context(3);
			break;
		default:
			mmi_phb_set_extraNum_context(0);
			break;
	}
	#endif
	
	ExecuteCurrHiliteHandler(hiliteid);
}

void mmi_phb_entry_extra_number(void) 
{
	U8* guiBuffer;
	U16 nStrItemList[3*MMI_SIM_NUMBER]; 
	U8* nHintList[3*MMI_SIM_NUMBER];
	U16 nNumofItem;
	#ifdef __MMI_MULTI_SIM__
	U8 i;
	#endif

	EntryNewScreen(SCR_ID_PHB_SPECIAL_NUMBER, mmi_phb_exit_extra_number, mmi_phb_entry_extra_number, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_SPECIAL_NUMBER);

	#ifdef __MMI_MULTI_SIM__
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		mmi_frm_hide_menu_item(MITEM105_PBOOK_OWNER_NUMBER + i);
		mmi_frm_hide_menu_item(MENU_PHONEBOOK_SERVICENUMBER + i);
		mmi_frm_hide_menu_item(MITEM116_PBOOK_SOS_NUMBER + i);
		if(!MTPNP_PFAL_Is_CardAbsent(i))
		{
			mmi_frm_unhide_menu_item(MITEM105_PBOOK_OWNER_NUMBER + i);
			mmi_frm_unhide_menu_item(MENU_PHONEBOOK_SERVICENUMBER + i);
			mmi_frm_unhide_menu_item(MITEM116_PBOOK_SOS_NUMBER + i);
		}
	}
	#endif

	nNumofItem = GetNumOfChild_Ext(MENU_ID_PHB_EXTRA_NUMBER);
	GetSequenceStringIds_Ext(MENU_ID_PHB_EXTRA_NUMBER, nStrItemList);
	SetParentHandler(MENU_ID_PHB_EXTRA_NUMBER);
	RegisterHighlightHandler(ExtraNumMenuHighLightHandler);
	ConstructHintsList(MENU_ID_PHB_EXTRA_NUMBER, nHintList);

	ShowCategory52Screen(STR_ID_PHB_EXTRA_NUMBER, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  nHintList, 0, 0, guiBuffer);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_extra_number()
* DESCRIPTION
*   exit function of extra number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_extra_number(void)
{
	g_phb_cntx.end_scr_id	= SCR_ID_PHB_SPECIAL_NUMBER;
}

/* --------------------------------------  Begin of Phone Book Owner Number -------------------------------------------*/
#define MMI_PHB_OWNER_NUMBER
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_owner_number()
* DESCRIPTION
*   Highlight handler for Owner Number in PHB main menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_owner_number(void) 
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_highlight_owner_number.>\n",__FILE__,__LINE__));

	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_owner_number_pre_req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_owner_number_pre_req, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_owner_number_erase()
* DESCRIPTION
*   Highlight handler for owner number's option "Erase"
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_owner_number_erase(void) 
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_highlight_owner_number_erase.>\n",__FILE__,__LINE__));

	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_owner_number_pre_erase,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_owner_number_edit()
* DESCRIPTION
*   Highlight handler for owner number's option "Edit" 
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_owner_number_edit(void) 
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_highlight_owner_number_edit.>\n",__FILE__,__LINE__));

	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_owner_number_pre_edit,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_owner_number_pre_edit, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_pre_req()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_owner_number_pre_req(void) 
{
    if(g_phb_cntx.phb_ready && !g_phb_cntx.processing) 
    {
        mmi_phb_show_in_progress(STR_PROCESSING_PHONEBOOK, IMG_PROCESSING_PHONEBOOK);
        mmi_phb_owner_number_get_entry_req();
    }
    else
    	mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_get_entry_req()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_owner_number_get_entry_req(void) 
{
	MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *myMsgPtr;
	MYQUEUE	Message;
	module_type	dest_mod_id = MOD_L4C;
	U8 storage = MMI_SIM;
	U8 nSimID = 0;
	
	//if(!g_phb_is_get_entry_rsp)
	//	return;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_owner_number_get_entry_req.>\n",__FILE__,__LINE__));
		
	myMsgPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT));

	mmi_trace(g_sw_PHB, "PHB OWN Func: %s simId = %d", __FUNCTION__, g_pExtraNum_context->simId);

	#ifdef __MMI_MULTI_SIM__
	storage += g_pExtraNum_context->simId;
	dest_mod_id += g_pExtraNum_context->simId;
	g_pDispOwnerNumber = gDispOwnerNumber[g_pExtraNum_context->simId];
	nSimID = g_pExtraNum_context->simId;
	#endif
	

	myMsgPtr->index			= 1;
	myMsgPtr->record_index	= 0xFFFF;
	myMsgPtr->storage		= storage;
	myMsgPtr->type			= MMI_PHB_MSISDN;
	myMsgPtr->no_data		= MAX_ENTRIES_IN_LIST;
		
	Message.oslSrcId		= MOD_MMI;
	Message.oslDestId		= dest_mod_id;
	Message.oslMsgId		= PRT_PHB_GET_ENTRY_BY_INDEX_REQ;
	Message.oslDataPtr		= (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr	= NULL;

	SetProtocolEventHandlerExt(mmi_phb_owner_number_get_entry_rsp, PRT_PHB_GET_ENTRY_BY_INDEX_RSP, nSimID);
	OslMsgSendExtQueue(&Message);

	//g_phb_is_get_entry_rsp = 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_get_entry_rsp()
* DESCRIPTION
*   Reads the owner numbers back from PS
*
* PARAMETERS
 
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_owner_number_get_entry_rsp(void* info) 
{
	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U16 i;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_owner_number_get_entry_rsp.>\n",__FILE__,__LINE__));

	//g_phb_is_get_entry_rsp = 1;  
//qiff modify for bug:10650,the RSP handler should be set in REQ function
	SetProtocolEventHandlerExt(NULL, PRT_PHB_GET_ENTRY_BY_INDEX_RSP,nSimID);

	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *)info;
	if (localPtr->result.flag == 0 /*OK*/) 
	{
		g_phb_sim_list_count = (localPtr->no_list < MAX_SIM_ENTRIES) ? localPtr->no_list : MAX_SIM_ENTRIES;
		
		for (i = 0; i < g_phb_sim_list_count && i < MAX_PB_OWNER_NUMBERS; i++) 
		{
			g_phb_sim_list[i].storage					= localPtr->list[i].storage;
			g_phb_sim_list[i].index					= localPtr->list[i].index;
			g_phb_sim_list[i].record_index			= localPtr->list[i].record_index;
			g_phb_sim_list[i].tel.type				= localPtr->list[i].tel.type;
			g_phb_sim_list[i].tel.length				= localPtr->list[i].tel.length;
			g_phb_sim_list[i].alpha_id.name_length	= localPtr->list[i].alpha_id.name_length;
			g_phb_sim_list[i].alpha_id.name_dcs		= localPtr->list[i].alpha_id.name_dcs;

			InputboxConvertGSMToDeviceEncoding(localPtr->list[i].alpha_id.name, g_phb_sim_list[i].alpha_id.name, localPtr->list[i].alpha_id.name_length,(g_phb_cntx.owner_name_len+1)*ENCODING_LENGTH, localPtr->list[i].alpha_id.name_dcs, 0, 1);

			if ((g_phb_sim_list[i].tel.length > 0) && ((g_phb_sim_list[i].tel.type & 0x10) != 0)) 
			{
				g_phb_sim_list[i].tel.number[0] = '+';
				AnsiiToUnicodeString((PS8)(g_phb_sim_list[i].tel.number+ENCODING_LENGTH), (PS8)localPtr->list[i].tel.number);
			} 
			else 
			{
				AnsiiToUnicodeString((PS8)g_phb_sim_list[i].tel.number, (PS8)localPtr->list[i].tel.number);
			}
		}
	}
	else /*TBD - what to do if result flag is false*/
	{	
		g_phb_sim_list_count = 0;
	}

	/*Owner number associate with Line ID*/
	memset( (void *)g_pDispOwnerNumber, 0, sizeof(MMI_OWNER_NUMBER_STRUCT) );

	for(i = 0; ((i < MAX_LINE_NUM) && (i < g_phb_sim_list_count)) ; ++i)
	{
		if(pfnUnicodeStrlen((PS8)g_phb_sim_list[i].alpha_id.name))
			pfnUnicodeStrncpy((PS8)g_pDispOwnerNumber[i].name, (PS8)g_phb_sim_list[i].alpha_id.name,  MAX_PB_NAME_LENGTH+1 );

		if(pfnUnicodeStrlen((PS8)g_phb_sim_list[i].tel.number))
			pfnUnicodeStrncpy((PS8)g_pDispOwnerNumber[i].number, (PS8)g_phb_sim_list[i].tel.number,  MAX_PB_NUMBER_LENGTH+1 );
	}

	RefreshDisplayOwnerNumber((U8)g_callset_cntx_p->LineID);

	if(g_phb_operate_owner) 
	{
		g_phb_operate_owner = FALSE;
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
		DeleteScreenIfPresent(SCR_OWNER_NUMBER_EDIT);
		DeleteScreenIfPresent(SCR_OWNER_NUMBER_OPTIONS);		
	}
	else if(g_phb_sim_list_count)
	{
		/*Display result screen only when processing screen is not interrupted.*/
		if(GetActiveScreenId() != SCR_IN_PROGRESS_PHB)
			return;
		mmi_phb_entry_owner_number();
	}
	else
		DisplayPopup((PU8)GetString(STR_NO_OWNER_NUMBER_MESSAGE), IMG_GLOBAL_EMPTY,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_get_entry_req()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_idle_disp_owner_number_get_entry_req(U8 nSimID) 
{
	MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *myMsgPtr;
	MYQUEUE	Message;

	if(!g_phb_is_get_entry_rsp)
		return;

	mmi_trace(g_sw_PHB, "PHB func:%s \n", __FUNCTION__);
		
	myMsgPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT));
	myMsgPtr->index			= 1;
	myMsgPtr->record_index	= 0xFFFF;
	myMsgPtr->storage		= MMI_SIM + nSimID;
	myMsgPtr->type			= MMI_PHB_MSISDN;
	myMsgPtr->no_data		= MAX_ENTRIES_IN_LIST;
		
	Message.oslSrcId		= MOD_MMI;
	Message.oslDestId		= MOD_L4C + nSimID;
	Message.oslMsgId		= PRT_PHB_GET_ENTRY_BY_INDEX_REQ;
	Message.oslDataPtr		= (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr	= NULL;

	SetProtocolEventHandlerExt(mmi_phb_idle_disp_owner_number_get_entry_rsp, PRT_PHB_GET_ENTRY_BY_INDEX_RSP, nSimID);
	OslMsgSendExtQueue(&Message);

	g_phb_is_get_entry_rsp = 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_get_entry_rsp()
* DESCRIPTION
*   Reads the owner numbers back from PS
*
* PARAMETERS
 
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern pBOOL mmi_idle_is_on_idle_screen();
extern void RedrawIdleScreen(void);
void mmi_phb_idle_disp_owner_number_get_entry_rsp(void* info) 
{
	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U16 i;
	U8 nSimID =MTPNP_AD_get_protocol_event_src_mode();

	g_phb_is_get_entry_rsp = 1;  
//qiff modify for bug:10650,the RSP handler should be set in REQ function
	SetProtocolEventHandler(NULL, PRT_PHB_GET_ENTRY_BY_INDEX_RSP);

	g_pDispOwnerNumber = gDispOwnerNumber[nSimID];

	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *)info;
	mmi_trace(g_sw_PHB, "PHB func:%s localPtr->result.flag = %d\n", __FUNCTION__, localPtr->result.flag);	
	if (localPtr->result.flag == 0 /*OK*/) 
	{
		g_phb_sim_list_count = (localPtr->no_list < MAX_SIM_ENTRIES) ? localPtr->no_list : MAX_SIM_ENTRIES;
		
		for (i = 0; i < g_phb_sim_list_count && i < MAX_PB_OWNER_NUMBERS; i++) 
		{
			g_phb_sim_list[i].storage					= localPtr->list[i].storage;
			g_phb_sim_list[i].index					= localPtr->list[i].index;
			g_phb_sim_list[i].record_index			= localPtr->list[i].record_index;
			g_phb_sim_list[i].tel.type				= localPtr->list[i].tel.type;
			g_phb_sim_list[i].tel.length				= localPtr->list[i].tel.length;
			g_phb_sim_list[i].alpha_id.name_length	= localPtr->list[i].alpha_id.name_length;
			g_phb_sim_list[i].alpha_id.name_dcs		= localPtr->list[i].alpha_id.name_dcs;

			InputboxConvertGSMToDeviceEncoding(localPtr->list[i].alpha_id.name, g_phb_sim_list[i].alpha_id.name, localPtr->list[i].alpha_id.name_length,(g_phb_cntx.owner_name_len+1)*ENCODING_LENGTH, localPtr->list[i].alpha_id.name_dcs, 0, 1);

			if ((g_phb_sim_list[i].tel.length > 0) && ((g_phb_sim_list[i].tel.type & 0x10) != 0)) 
			{
				g_phb_sim_list[i].tel.number[0] = '+';
				AnsiiToUnicodeString((PS8)(g_phb_sim_list[i].tel.number+ENCODING_LENGTH), (PS8)localPtr->list[i].tel.number);
			} 
			else 
			{
				AnsiiToUnicodeString((PS8)g_phb_sim_list[i].tel.number, (PS8)localPtr->list[i].tel.number);
			}
		}
	}
	else /*TBD - what to do if result flag is false*/
	{	
		g_phb_sim_list_count = 0;
	}

	/*Owner number associate with Line ID*/
	memset( (void *)g_pDispOwnerNumber, 0, sizeof(MMI_OWNER_NUMBER_STRUCT) );

	for(i = 0; ((i < MAX_LINE_NUM) && (i < g_phb_sim_list_count)) ; ++i)
	{
		if(pfnUnicodeStrlen((PS8)g_phb_sim_list[i].alpha_id.name))
			pfnUnicodeStrncpy((PS8)g_pDispOwnerNumber[i].name, (PS8)g_phb_sim_list[i].alpha_id.name,  MAX_PB_NAME_LENGTH+1 );

		if(pfnUnicodeStrlen((PS8)g_phb_sim_list[i].tel.number))
			pfnUnicodeStrncpy((PS8)g_pDispOwnerNumber[i].number, (PS8)g_phb_sim_list[i].tel.number,  MAX_PB_NUMBER_LENGTH+1 );
	}

	RefreshDisplayOwnerNumber((U8)g_callset_context[nSimID].LineID);
	if(mmi_idle_is_on_idle_screen() == TRUE)
	{
		RedrawIdleScreen();
	}	
    
	for(i = nSimID + 1; i < MMI_SIM_NUMBER; i++)
	{
		#ifdef __MMI_MULTI_SIM__
		if(!MTPNP_PFAL_Is_CardAbsent(i))
		#endif
		{
			mmi_phb_idle_disp_owner_number_get_entry_req(i);
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_owner_number()
* DESCRIPTION
*   Displays the owner numbers list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_owner_number(void) 
{
	U8 *names[MAX_PB_OWNER_NUMBERS];
	U16 i;
	U8* guiBuffer;
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_owner_number.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_OWNER_NUMBERS, mmi_phb_exit_owner_number, mmi_phb_entry_owner_number, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_OWNER_NUMBERS);		
	
#if defined(__MMI_PHB_DISPLAY_ONE_MSISDN__)
	g_phb_sim_list_count = 1;
#endif
	
	for(i = 0; i < g_phb_sim_list_count; i++) 
	{
		if (pfnUnicodeStrlen((PS8)g_phb_sim_list[i].alpha_id.name)) 
		{
			names[i] = g_phb_sim_list[i].alpha_id.name;
			if (pfnUnicodeStrlen((PS8)g_phb_sim_list[i].tel.number))
				subMenuDataPtrs[i] = g_phb_sim_list[i].tel.number;
			else 
				subMenuDataPtrs[i] = (PU8)GetString(STR_GLOBAL_EMPTY_LIST);
		}
		else 
		{
			if(pfnUnicodeStrlen((PS8)g_phb_sim_list[i].tel.number)) 
			{
				names[i] = g_phb_sim_list[i].tel.number;
				subMenuDataPtrs[i] = NULL;
			}
			else 
			{
				names[i] = (PU8)GetString(STR_GLOBAL_EMPTY_LIST);
				subMenuDataPtrs[i] = NULL;
			}
		}
	}

	RegisterHighlightHandler(mmi_phb_get_index);
	ShowCategory53Screen(STR_MITEM105, IMG_SCR_PBOOK_CAPTION,
					  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					  g_phb_sim_list_count, names, (U16*)gIndexIconsImageList, subMenuDataPtrs, 0, 0, guiBuffer);

	SetLeftSoftkeyFunction(mmi_phb_entry_onwer_number_option,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_onwer_number_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_owner_number()
* DESCRIPTION
*   Exit function of mmi_phb_entry_owner_number()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_owner_number(void) 
{
	if (g_phb_cntx.start_scr_id == 0)
	{
		g_phb_cntx.start_scr_id = GetCurrScrnId();
	}
	g_phb_cntx.end_scr_id = SCR_OWNER_NUMBERS;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_onwer_number_option()
* DESCRIPTION
*   Draws the menu for Owner number options
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_onwer_number_option(void) 
{

	U8* guiBuffer;
	U16 nStrItemList[2]; 
	U16 nNumofItem;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_onwer_number_option.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_OWNER_NUMBER_OPTIONS, mmi_phb_exit_onwer_number_option, mmi_phb_entry_onwer_number_option, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_OWNER_NUMBER_OPTIONS);		/* Getting the Index from history  */
	nNumofItem = GetNumOfChild(MITEM105_OWNER_NUMBER_OPTIONS);
	GetSequenceStringIds(MITEM105_OWNER_NUMBER_OPTIONS, nStrItemList);
	SetParentHandler(MITEM105_OWNER_NUMBER_OPTIONS);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	ShowCategory15Screen(STR_MITEM105, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_onwer_number_option()
* DESCRIPTION
*   Exit function of mmi_phb_entry_onwer_number_option()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_onwer_number_option(void) 
{
	g_phb_cntx.end_scr_id = SCR_OWNER_NUMBER_OPTIONS;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_pre_erase()
* DESCRIPTION
*   Confirms before erasing the owner number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_owner_number_pre_erase(void)
{
	if(pfnUnicodeStrlen((S8*)g_phb_sim_list[g_phb_cntx.active_index].alpha_id.name) ||
		pfnUnicodeStrlen((S8*)g_phb_sim_list[g_phb_cntx.active_index].tel.number))
		mmi_phb_entry_owner_number_erase_confirm();
	else
		DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY_LIST), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_owner_number_erase_confirm()
* DESCRIPTION
*   Confirms before erasing the owner number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_owner_number_erase_confirm(void)	 
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_ERASE_QUERY), IMG_GLOBAL_QUESTION, WARNING_TONE); 


	SetLeftSoftkeyFunction(mmi_phb_owner_number_erase_req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_phb_go_back_2_history,KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_erase_req()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_owner_number_erase_req(void) 
{
/*
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	
	g_phb_operate_owner = TRUE;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_owner_number_erase_req.>\n",__FILE__,__LINE__));
	mmi_phb_show_in_progress(STR_PHB_ERASING, IMG_GLOBAL_ERASED);

	myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));
	myMsgPtr->type = MMI_PHB_MSISDN;
	myMsgPtr->no_data = 0;
	myMsgPtr->list.storage = MMI_SIM;
	myMsgPtr->list.index = g_phb_sim_list[g_phb_cntx.active_index].index;
	myMsgPtr->list.record_index = 0xFFFF;
	memset(myMsgPtr->list.alpha_id.name, 0, ENCODING_LENGTH);
	myMsgPtr->list.alpha_id.name_length = 0;
	myMsgPtr->list.alpha_id.name_dcs = MMI_PHB_ASCII;

	myMsgPtr->list.tel.type = MMI_CSMCC_DEFAULT_ADDR_TYPE; //DEFAULT
	myMsgPtr->list.tel.length = 0;
	memset(myMsgPtr->list.tel.number, 0, ENCODING_LENGTH);
				
	Message.oslSrcId		= MOD_MMI;
	Message.oslDestId		= MOD_L4C;
	Message.oslMsgId		= PRT_PHB_SET_ENTRY_REQ;
	Message.oslDataPtr		= (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr	= NULL;

	SetProtocolEventHandler(mmi_phb_owner_number_erase_rsp	,PRT_PHB_SET_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);
	DeleteNScrId(SCR_OWNER_NUMBER_OPTIONS);
*/
	MSG_PHB_DELETE_ENTRY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	module_type	dest_mod_id = MOD_L4C;
	U8 storage = MMI_SIM;
	U8 nSimID = 0;
	
	/*If not enter from edit request, that is enter from erase request, so enter processing screen.*/
	if(!g_phb_edit_owner)
	{
		g_phb_operate_owner = TRUE;

		PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_owner_number_erase_req.>\n",__FILE__,__LINE__));
		mmi_phb_show_in_progress(STR_PHB_ERASING, IMG_GLOBAL_ERASED);
	}
	
	myMsgPtr = (MSG_PHB_DELETE_ENTRY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_DELETE_ENTRY_REQ_STRUCT));

#ifdef __MMI_MULTI_SIM__
	storage = MMI_SIM + g_pExtraNum_context->simId;
	dest_mod_id = MOD_L4C + g_pExtraNum_context->simId;
	nSimID = g_pExtraNum_context->simId;
#endif

	myMsgPtr->storage = storage;	
	myMsgPtr->del_all = MMI_FALSE;
	myMsgPtr->no_data = 1;
	myMsgPtr->type = MMI_PHB_MSISDN;
	myMsgPtr->index = g_phb_sim_list[g_phb_cntx.active_index].index;	
	
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_DEL_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandler(mmi_phb_owner_number_erase_rsp,PRT_PHB_DEL_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);

	/*If not enter from edit request, that is enter from erase request, so erase screen.*/
	//if(!g_phb_edit_owner)
	//	DeleteNScrId(SCR_PBOOK_ENTRY_OPTIONS);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_erase_rsp()
* DESCRIPTION
*  Response handler for Owner number erase request
*
* PARAMETERS
 
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_owner_number_erase_rsp(void* info) 
{
	//MSG_PHB_SET_ENRTY_RSP_STRUCT* localPtr;
	//localPtr = (MSG_PHB_SET_ENRTY_RSP_STRUCT *)info;

	MSG_PHB_DELETE_ENTRY_RSP_STRUCT* localPtr;
	
	localPtr = (MSG_PHB_DELETE_ENTRY_RSP_STRUCT *)info;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_owner_number_erase_rsp.>\n",__FILE__,__LINE__));
	
	/*If this is erase response before save request,  continue to set entry.*/
	if(g_phb_edit_owner)
	{
		mmi_phb_owner_number_edit_req();
		return;
	}
	
	if (localPtr->result.flag == 0 /*OK*/)
		mmi_phb_owner_number_get_entry_req();
	else 
	{
		g_phb_operate_owner = FALSE;
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		DeleteScreenIfPresent(SCR_OWNER_NUMBER_OPTIONS);		
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_pre_edit()
* DESCRIPTION
*   Sets the buffers before editing the owner number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_owner_number_pre_edit(void) 
{
	memset(pbName, 0, ENCODING_LENGTH);
	pfnUnicodeStrcpy((PS8)pbName, (PS8)g_phb_sim_list[g_phb_cntx.active_index].alpha_id.name);
	memset(pbNumber, 0, ENCODING_LENGTH);
	pfnUnicodeStrcpy((PS8)pbNumber, (PS8)g_phb_sim_list[g_phb_cntx.active_index].tel.number);
	mmi_phb_entry_owner_number_edit();
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_entry_owner_number_edit()
* DESCRIPTION
*   Inline edit screen to edit Owner number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_owner_number_edit(void) 
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
	U16 DetailPbNameImageList[2];

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<OwnerNumberEdit.>\n",__FILE__,__LINE__));
	EntryNewScreen(SCR_OWNER_NUMBER_EDIT, mmi_phb_exit_owner_number_edit, NULL, NULL);
	InitializeCategory57Screen();
	guiBuffer = GetCurrGuiBuffer(SCR_OWNER_NUMBER_EDIT);	
	inputBuffer = GetCurrNInputBuffer(SCR_OWNER_NUMBER_EDIT, &inputBufferSize);

	SetInlineItemActivation(&wgui_inline_items[0], KEY_LSK, KEY_EVENT_UP);
	SetInlineItemFullScreenEdit(&wgui_inline_items[0], STR_OWNER_NAME_CAPTION, IMG_SCR_PBOOK_CAPTION, (PU8)pbName, g_phb_cntx.owner_name_len+1, INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_USE_ENCODING_BASED_LENGTH|INPUT_TYPE_ONE_LESS_CHARACTER|INPUT_TYPE_GSM_EXTENDED_HANDLING);
	SetInlineFullScreenEditCustomFunction(&wgui_inline_items[0], mmi_phb_set_full_scr_lsk);
	DetailPbNameImageList[0] = IMG_NAME;
	
	SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
  // modify by chenq for bug 6252 MAX_PB_NUMBER_LENGTH -> MAX_SIM_NUMBER_LENGTH, 20080815 B
	SetInlineItemTextEdit(&wgui_inline_items[1], (PU8)pbNumber, MAX_SIM_NUMBER_LENGTH + 1, INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING );
  // modify by chenq for bug 6252 MAX_PB_NUMBER_LENGTH -> MAX_SIM_NUMBER_LENGTH, 20080815 E
	DisableInlineItemHighlight(&wgui_inline_items[1]);
	DetailPbNameImageList[1] = IMG_MOBILE_NUMBER;

	if(inputBuffer!=NULL) 
		SetCategory57Data(wgui_inline_items, 2, inputBuffer);
	
	ShowCategory57Screen(STR_GLOBAL_EDIT, IMG_SCR_PBOOK_CAPTION,
		  STR_GLOBAL_EDIT, IMG_EDIT_LSK, STR_GLOBAL_DONE, IMG_DONE_RSK,
		  2, DetailPbNameImageList, wgui_inline_items, 0, guiBuffer);
#if defined(__MMI_GB__)                                                               //add by yaosq 20061113
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone);
#endif
	SetCategory57RightSoftkeyFunctions(mmi_phb_entry_owner_number_edit_confirm,GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_exit_owner_number_edit()

* DESCRIPTION
*   Exit function of mmi_phb_entry_owner_number_edit()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_owner_number_edit(void) 
{
	history_t	h;
	U16 inputBufferSize;

	g_phb_cntx.end_scr_id = SCR_OWNER_NUMBER_EDIT; 
	CloseCategory57Screen();
	h.scrnID = SCR_OWNER_NUMBER_EDIT;
	h.entryFuncPtr = mmi_phb_entry_owner_number_edit;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();  
	GetCategory57Data((U8*) h.inputBuffer);       
	AddNHistory(h, inputBufferSize);           
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_owner_number_edit_confirm()
* DESCRIPTION
*   Confirms before saving the owner number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_owner_number_edit_confirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, WARNING_TONE); 


	SetLeftSoftkeyFunction(mmi_phb_owner_number_edit_req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_phb_go_back_3_history_and_clear,KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_edit_req()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_owner_number_edit_req(void) 
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
#if defined(__PHB_0x81_SUPPORT__)
	U8 length;
#endif
	module_type	dest_mod_id = MOD_L4C;
	U8 storage = MMI_SIM;
	U8 nSimID = 0;
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_owner_number_edit_req.>\n",__FILE__,__LINE__));

	g_phb_operate_owner = TRUE;

	/*Erase record before save*/
	if(!g_phb_edit_owner)
	{
		g_phb_edit_owner = TRUE;
		mmi_phb_show_in_progress(STR_GLOBAL_SAVING, IMG_GLOBAL_SAVE);		
		mmi_phb_owner_number_erase_req();
		return;
	}
	else
		g_phb_edit_owner = FALSE;

	myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));
#ifdef __MMI_MULTI_SIM__
	storage = MMI_SIM + g_pExtraNum_context->simId;
	dest_mod_id = MOD_L4C + g_pExtraNum_context->simId;
	nSimID = g_pExtraNum_context->simId;
#endif
	myMsgPtr->type = MMI_PHB_MSISDN;
	myMsgPtr->no_data = 0;
	myMsgPtr->list.storage = storage;
	myMsgPtr->list.index = g_phb_sim_list[g_phb_cntx.active_index].index;
	myMsgPtr->list.record_index = 0xFFFF;

	if(GetUCS2Flag(pbName)) 
	{
		myMsgPtr->list.alpha_id.name_dcs = MMI_PHB_UCS2;
		myMsgPtr->list.alpha_id.name_length = ((U8)pfnUnicodeStrlen(pbName))*ENCODING_LENGTH;
		InputboxConvertDeviceEncodingToGSM((PU8)pbName, myMsgPtr->list.alpha_id.name, MAX_PS_NAME_LENGTH, MMI_PHB_UCS2, 0, 1);
#if defined(__PHB_0x81_SUPPORT__)	/*Support maximum length of 0x81 UCS2*/
		if((length = mmi_phb_convert_to_0x81((S8*)myMsgPtr->list.alpha_id.name,TRUE)) > 0) 
		{
			myMsgPtr->list.alpha_id.name_dcs = 0x09;
			myMsgPtr->list.alpha_id.name_length = length;
		}
#endif		
	}
	else 
	{
		myMsgPtr->list.alpha_id.name_dcs = MMI_PHB_ASCII;
		myMsgPtr->list.alpha_id.name_length = InputboxConvertDeviceEncodingToGSM((PU8)pbName, myMsgPtr->list.alpha_id.name, MAX_PS_NAME_LENGTH, MMI_PHB_ASCII, 0, 1) - 1;
	}

	if (pbNumber[0] == '+') 
	{
		myMsgPtr->list.tel.type = MMI_CSMCC_INTERNATIONAL_ADDR; //INTERNATIONAL
		myMsgPtr->list.tel.length = (U8)pfnUnicodeStrlen(pbNumber + ENCODING_LENGTH);
		UnicodeToAnsii((PS8)myMsgPtr->list.tel.number, pbNumber + ENCODING_LENGTH);
	}
	else 
	{
		myMsgPtr->list.tel.type = MMI_CSMCC_DEFAULT_ADDR_TYPE; //DEFAULT
		myMsgPtr->list.tel.length = (U8)pfnUnicodeStrlen(pbNumber);
		UnicodeToAnsii((PS8)myMsgPtr->list.tel.number, pbNumber);
	}
				
	Message.oslSrcId		= MOD_MMI;
	Message.oslDestId		= dest_mod_id;
	Message.oslMsgId		= PRT_PHB_SET_ENTRY_REQ;
	Message.oslDataPtr		= (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr	= NULL;

	SetProtocolEventHandler(mmi_phb_owner_number_edit_rsp, PRT_PHB_SET_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);
	//DeleteNScrId(SCR_OWNER_NUMBER_OPTIONS);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_owner_number_edit_rsp()
* DESCRIPTION
*   Response hanlder for owner number edit request
*
* PARAMETERS
 
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_owner_number_edit_rsp(void* info) 
{
	MSG_PHB_SET_ENRTY_RSP_STRUCT* localPtr;
	
	localPtr = (MSG_PHB_SET_ENRTY_RSP_STRUCT *)info;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_owner_number_edit_rsp.>\n",__FILE__,__LINE__));
	
	if (localPtr->result.flag == 0 /*OK*/)
		mmi_phb_owner_number_get_entry_req();
	else 
	{
		g_phb_operate_owner = FALSE;
		if(localPtr->result.cause == MMI_SIM_EF_RECORD_FULL)
			DisplayPopup((PU8)GetString(STR_NOT_SAVED_NUMBER_TOO_LONG), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);		
		else
			DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);

		DeleteScreenIfPresent(SCR_OWNER_NUMBER_EDIT);
		DeleteScreenIfPresent(SCR_OWNER_NUMBER_OPTIONS);
	}
}
/* --------------------------------------  END of Phone Book Owner Number -------------------------------------------*/


/* --------------------------------------  Begin of Phone Book Service Dial Number -------------------------------------------*/
#define MMI_PHB_SDN_LIST
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_sdn_list()
* DESCRIPTION
*   Highlight handler for SDL in PHB main menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_sdn_list(void) 
{
	SetLeftSoftkeyFunction(mmi_phb_sdn_list_pre_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_sdn_list_pre_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sdn_list_pre_entry()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sdn_list_pre_entry(void) 
{
    if(g_phb_cntx.phb_ready && !g_phb_cntx.processing) 
    {
        mmi_phb_show_in_progress(STR_PROCESSING_PHONEBOOK, IMG_PROCESSING_PHONEBOOK);
        g_phb_sim_list_count = 0;
        g_phb_read_index = 1;
        mmi_phb_sdn_get_entry_req();
    }
    else
        mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sdn_get_entry_req()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sdn_get_entry_req(void) 
{
	MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *myMsgPtr;
	MYQUEUE	Message;
	module_type	dest_mod_id = MOD_L4C;
	U8 storage = MMI_SIM;
	U8 nSimID = 0;
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<LoadAllFdlist.>\n",__FILE__,__LINE__));
		
	myMsgPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT*)OslIntConstructDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT));
#ifdef __MMI_MULTI_SIM__
	storage = MMI_SIM + g_pExtraNum_context->simId;
	dest_mod_id = MOD_L4C + g_pExtraNum_context->simId;
	nSimID = g_pExtraNum_context->simId;
#endif
	myMsgPtr->index			= g_phb_read_index;	/*Begin from 1*/
	myMsgPtr->record_index	= 0xFFFF;
	myMsgPtr->storage		= storage ;
	myMsgPtr->type			= MMI_PHB_SDN ;
	myMsgPtr->no_data		= MAX_ENTRIES_IN_LIST;
	
	Message.oslSrcId		= MOD_MMI;
	Message.oslDestId		= dest_mod_id;
	Message.oslMsgId		= PRT_PHB_GET_ENTRY_BY_INDEX_REQ;
	Message.oslDataPtr		= (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr	= NULL;

	SetProtocolEventHandlerExt(mmi_phb_sdn_get_entry_rsp, PRT_PHB_GET_ENTRY_BY_INDEX_RSP, nSimID);
	OslMsgSendExtQueue(&Message);
}

/*****************************************************************************
* FUNCTION
* mmi_phb_sdn_get_entry_rsp()
* DESCRIPTION
 
*
* PARAMETERS
 
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sdn_get_entry_rsp(void * info) 
{
	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U16 i;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<GetAllFdlEntries.>\n",__FILE__,__LINE__));

	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *)info;
//qiff modify for bug:10650,the RSP handler should be set in REQ function
	SetProtocolEventHandlerExt(NULL, PRT_PHB_GET_ENTRY_BY_INDEX_RSP, nSimID);

	if (localPtr->result.flag == 0 /*OK*/) 
	{
		/*Begin to assign to list*/		
		for(i= 0; i < localPtr->no_list; i++) 
		{
			/*No Space*/
			if(g_phb_sim_list_count >= MAX_SIM_ENTRIES)
				continue;

			/*Empty Entry*/
			if((localPtr->list[i].alpha_id.name_length == 0) && (localPtr->list[i].tel.length == 0))
				continue;
		
			g_phb_sim_list[g_phb_sim_list_count].storage				= localPtr->list[i].storage;
			g_phb_sim_list[g_phb_sim_list_count].index				= localPtr->list[i].index;
			g_phb_sim_list[g_phb_sim_list_count].record_index			= localPtr->list[i].record_index;	
			g_phb_sim_list[g_phb_sim_list_count].tel.type				= localPtr->list[i].tel.type;
			g_phb_sim_list[g_phb_sim_list_count].tel.length			= localPtr->list[i].tel.length;
			g_phb_sim_list[g_phb_sim_list_count].alpha_id.name_length	= localPtr->list[i].alpha_id.name_length;
			g_phb_sim_list[g_phb_sim_list_count].alpha_id.name_dcs	= localPtr->list[i].alpha_id.name_dcs;

			if(g_phb_sim_list[g_phb_sim_list_count].alpha_id.name_dcs == MMI_PHB_UCS2)
				BigEndianToLittleEndian((PS8)g_phb_sim_list[g_phb_sim_list_count].alpha_id.name, (PS8)localPtr->list[i].alpha_id.name);
            else
				AnsiiToUnicodeString((PS8)g_phb_sim_list[g_phb_sim_list_count].alpha_id.name, (PS8)localPtr->list[i].alpha_id.name);

			if ((localPtr->list[i].tel.length > 0) && ((localPtr->list[i].tel.type & 0x10) != 0)) 
			{
				g_phb_sim_list[g_phb_sim_list_count].tel.number[0] = '+';

				AnsiiToUnicodeString((PS8)(g_phb_sim_list[g_phb_sim_list_count].tel.number+ENCODING_LENGTH), (PS8)localPtr->list[i].tel.number);
			} 	
			else 
			{
				AnsiiToUnicodeString((PS8)g_phb_sim_list[g_phb_sim_list_count].tel.number, (PS8)localPtr->list[i].tel.number);
			}

			g_phb_sim_list_count++;
		}	

		/*Try to read more entry if exists.*/
		if (localPtr->no_list == MAX_ENTRIES_IN_LIST)
		{
			g_phb_read_index += MAX_ENTRIES_IN_LIST;
			mmi_phb_sdn_get_entry_req();
			return;
		}
	}
	else
	{		
		/*TBD - what to do if result flag is false*/
	}
	
	if(g_phb_sim_list_count)
		mmi_phb_entry_sdn_list();
	else 
		DisplayPopup((PU8)GetString(STR_SDL_NO_LIST_TEXT), IMG_GLOBAL_WARNING, FALSE, PHB_NOTIFY_TIMEOUT, WARNING_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_sdn_list()
* DESCRIPTION
*   Displays the Service Dail Numbers
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_sdn_list(void) 
{
	U8 i;
	U8* guiBuffer;
	U8* names[MAX_SIM_ENTRIES];

	EntryNewScreen(SCR_SDN_LIST, mmi_phb_exit_sdn_list, mmi_phb_entry_sdn_list, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_SDN_LIST);
	
	for(i = 0; i < g_phb_sim_list_count; ++i) 
	{
		if (pfnUnicodeStrlen((PS8)g_phb_sim_list[i].alpha_id.name)) 
		{
			names[i] = g_phb_sim_list[i].alpha_id.name;
			if (pfnUnicodeStrlen((PS8)g_phb_sim_list[i].tel.number))
				subMenuDataPtrs[i] = g_phb_sim_list[i].tel.number;
			else 
				subMenuDataPtrs[i] = (PU8)GetString(STR_GLOBAL_EMPTY_LIST);
		}
		else 
		{
			if(pfnUnicodeStrlen((PS8)g_phb_sim_list[i].tel.number)) 
			{
				names[i] = g_phb_sim_list[i].tel.number;
				subMenuDataPtrs[i] = NULL;
			}
			else 
			{
				names[i] = (PU8)GetString(STR_GLOBAL_EMPTY_LIST);
				subMenuDataPtrs[i] = NULL;
			}
		}
	}

	RegisterHighlightHandler(mmi_phb_get_index);
	ShowCategory53Screen(STR_CALL_SDL_CAPTION,IMG_SCR_PBOOK_CAPTION,
			STR_GLOBAL_DIAL, IMG_SDL_CALL_LSK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			g_phb_sim_list_count, names, (U16*)gIndexIconsImageList, subMenuDataPtrs, 0, 0, guiBuffer);

	SetLeftSoftkeyFunction (mmi_phb_sdn_list_make_call,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_HandleSendKeys(mmi_phb_sdn_list_make_call,KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phb_sdn_list_make_call,KEY_ENTER,KEY_EVENT_UP );
#endif

#else
	SetKeyHandler(mmi_phb_sdn_list_make_call,KEY_SEND,KEY_EVENT_DOWN);
#endif
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_sdn_list()
* DESCRIPTION
*   Exit function of mmi_phb_entry_sdn_list()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_sdn_list(void) 
{
	g_phb_cntx.end_scr_id = SCR_SDN_LIST;	
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sdn_list_make_call()
* DESCRIPTION
*    Makes call to an SDL number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sdn_list_make_call(void) 
{
	if(pfnUnicodeStrlen((PS8)g_phb_sim_list[g_phb_cntx.active_index].tel.number))
	{
		g_phb_cntx.dial_from_list = MMI_PHB_SDN;
		if (g_phb_sim_list[g_phb_cntx.active_index].tel.number[0] == '+' &&
			g_phb_sim_list[g_phb_cntx.active_index].tel.number[
				(UCS2Strlen((const S8*)(g_phb_sim_list[g_phb_cntx.active_index].tel.number))-1)*ENCODING_LENGTH] == '#')
		{
			MakeCall((PS8)g_phb_sim_list[g_phb_cntx.active_index].tel.number + 2);
		}
		else
		{
			MakeCall((PS8)g_phb_sim_list[g_phb_cntx.active_index].tel.number);
		}
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_NO_NUMBER_TO_DIAL), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}
}

/* --------------------------------------  END of Phone Book Service Dial Number -------------------------------------------*/


/* --------------------------------------  Begin of Phone Book SOS(Emergency) Number -------------------------------------------*/
#define MMI_PHB_SOS_LIST
/*****************************************************************************
* FUNCTION
*  mmi_phb_sos_list_init()
* DESCRIPTION
*   Initialization of SOS numbers feature
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sos_list_init(void) 
{
	SetHiliteHandler(MITEM116_PBOOK_SOS_NUMBER,mmi_phb_highlight_sos_list );
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_sos_list()
* DESCRIPTION
*   Highlight handler for SOS number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_sos_list(void) 
{
	SetLeftSoftkeyFunction(mmi_phb_sos_list_pre_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_sos_list_pre_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sos_list_pre_entry()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sos_list_pre_entry(void) 
{
    if(g_phb_cntx.phb_ready && !g_phb_cntx.processing) 
    {
        mmi_phb_show_in_progress(STR_PROCESSING_PHONEBOOK, IMG_PROCESSING_PHONEBOOK);
        mmi_phb_sos_list_get_entry_req();
    }
    else
        mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sos_list_get_entry_req()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sos_list_get_entry_req(void) 
{
	MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *myMsgPtr;
	MYQUEUE	Message;
	module_type	dest_mod_id = MOD_L4C;
	U8 storage = MMI_SIM;
	U8 nSimID = 0;
	
	myMsgPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT*) OslIntConstructDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT));
#ifdef __MMI_MULTI_SIM__
	storage = MMI_SIM + g_pExtraNum_context->simId;
	dest_mod_id = MOD_L4C + g_pExtraNum_context->simId;
	nSimID = g_pExtraNum_context->simId;
#endif
	myMsgPtr->index			= 1;
	myMsgPtr->record_index	= 0xFFFF;
	myMsgPtr->storage		= storage ;
	myMsgPtr->type			= MMI_PHB_ECC ;
	myMsgPtr->no_data		= MAX_ENTRIES_IN_LIST;
	
	Message.oslSrcId		= MOD_MMI;
	Message.oslDestId		= dest_mod_id;
	Message.oslMsgId		= PRT_PHB_GET_ENTRY_BY_INDEX_REQ;
	Message.oslDataPtr		= (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr	= NULL;

	SetProtocolEventHandlerExt(mmi_phb_sos_list_get_entry_rsp, PRT_PHB_GET_ENTRY_BY_INDEX_RSP, nSimID);
	OslMsgSendExtQueue(&Message);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sos_list_get_entry_rsp()
* DESCRIPTION
 
*
* PARAMETERS
 
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sos_list_get_entry_rsp(void * info) 
{
	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U16 i;
	U8 number[MAX_PB_NUMBER_LENGTH + 1 +1];
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
//qiff modify for bug:10650,the RSP handler should be set in REQ function
	SetProtocolEventHandlerExt(NULL, PRT_PHB_GET_ENTRY_BY_INDEX_RSP,nSimID);

	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *)info;
	if (localPtr->result.flag == 0 /*OK*/) 
	{
		g_phb_sim_list_count = (localPtr->no_list < MAX_SIM_ENTRIES) ? localPtr->no_list : MAX_SIM_ENTRIES;
		
		for(i = 0; i < g_phb_sim_list_count && i < MAX_SIM_SOS_ENTRIES; ++i) 
		{
			g_phb_sim_list[i].storage = localPtr->list[i].storage;
			g_phb_sim_list[i].index = localPtr->list[i].index;
			g_phb_sim_list[i].record_index = localPtr->list[i].record_index;
			g_phb_sim_list[i].tel.type = localPtr->list[i].tel.type;

			if ((g_phb_sim_list[i].tel.length > 0) && ((g_phb_sim_list[i].tel.type & 0x10) != 0)) 
			{
				number[0]= '+';
				memcpy((void*) &number[1], (void*) &localPtr->list[i].tel.number[0], MAX_PB_NUMBER_LENGTH - 1);	
				AnsiiToUnicodeString((PS8) &g_phb_sim_list[i].tel.number,(PS8)number);

			} 	
			else 
			{
				g_phb_sim_list[i].tel.length = localPtr->list[i].tel.length ;
				memcpy((void*) &number, (void*) &localPtr->list[i].tel.number, MAX_PB_NUMBER_LENGTH );	
				AnsiiToUnicodeString((PS8) &g_phb_sim_list[i].tel.number,(PS8)number);
			}
		}
	} 
	else /* TBD - what to do if result flag is false*/
	{
		g_phb_sim_list_count = 0;
	}
	
	mmi_phb_entry_sos_list();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_sos_list()
* DESCRIPTION
*   Displays the SOS list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_sos_list(void) 
{
	U8* guiBuffer;
	S32 index = 0 ;
	S32 display_index = 0;
	S16 error = 0;
	U8* sub_ptr[MAX_NVRAM_SOS_ENTRIES+MAX_SIM_ENTRIES];

	/*Read from NVRAM for one time*/
#ifdef __MMI_MULTI_SIM__
	if( g_pExtraNum_context->g_phb_sos_nvram_read == 0) 
	{
		for(index = 0; index <MAX_NVRAM_SOS_ENTRIES;index++) 
			ReadRecord( NVRAM_EF_PHONEBOOK_SOS_LID + g_pExtraNum_context->simId ,(U16)(index + 1),&(g_pExtraNum_context->g_phb_nvram_list[index]),NVRAM_SOS_RECORD_SIZE,&error);

		g_pExtraNum_context->g_phb_sos_nvram_read = 1;
	}
#else
	if(g_pExtraNum_context->g_phb_sos_nvram_read == 0) 
	{
		for(index = 0; index <MAX_NVRAM_SOS_ENTRIES;index++) 
			ReadRecord( NVRAM_EF_PHONEBOOK_SOS_LID ,(U16)(index + 1),&(g_pExtraNum_context->g_phb_nvram_list[index]),NVRAM_SOS_RECORD_SIZE,&error);

		g_pExtraNum_context->g_phb_sos_nvram_read = 1;
	}	
#endif

	/*Prepare list*/
	for( index = 0;index < g_phb_sim_list_count; ++index) 
	{
		g_phb_sos_list[display_index] = (U8*)g_phb_sim_list[index].tel.number;
		sub_ptr[display_index++] = (U8*)GetString(STR_SOSLIST_INCARD);
	}
	for(; index<(MAX_NVRAM_SOS_ENTRIES+g_phb_sim_list_count); ++index) 
	{
		if(g_pExtraNum_context->g_phb_nvram_list[index - g_phb_sim_list_count].length == 0xFF || g_pExtraNum_context->g_phb_nvram_list[index - g_phb_sim_list_count].length == 0)
		{
#if defined(__MMI_PHB_SOS_NUM_EDIT__)
			g_phb_sos_list[display_index] = (U8*)GetString(STR_GLOBAL_EMPTY_LIST);
			sub_ptr[display_index++] = (U8 *) GetString(STR_SOSLIST_NVRAM);
#endif
		}
		else
		{
			g_phb_sos_list[display_index] = (U8*)g_pExtraNum_context->g_phb_nvram_list[index - g_phb_sim_list_count].number ;		
			sub_ptr[display_index++] = (U8 *) GetString(STR_SOSLIST_NVRAM);
		}
	}
		
#if !defined(__MMI_PHB_SOS_NUM_EDIT__)
	if(display_index == 0)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
		return;
	}
#endif

	EntryNewScreen(SCR_SOS_DISPLAY, mmi_phb_exit_sos_list, mmi_phb_entry_sos_list, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_SOS_DISPLAY);
	RegisterHighlightHandler(mmi_phb_sos_list_get_index);
	
	for(index = 0 ; index < display_index; index++)	/*subMenuDataPtrs[] will be clear in EntryNewScreen, so reassign here.*/
		subMenuDataPtrs[index] = sub_ptr[index];
	
#if defined(__MMI_PHB_SOS_NUM_EDIT__)
	ShowCategory53Screen(STR_DISPLAY_SOS_CAPTION,IMG_SCR_PBOOK_CAPTION,
				STR_GLOBAL_EDIT, IMG_GLOBAL_OPTIONS,
				STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				display_index/*(MAX_NVRAM_SOS_ENTRIES + g_phb_sim_list_count )*/,
				g_phb_sos_list, (U16*)gIndexIconsImageList, subMenuDataPtrs, 0, 0, guiBuffer);
#else
	ShowCategory53Screen(STR_DISPLAY_SOS_CAPTION,IMG_SCR_PBOOK_CAPTION,
				STR_GLOBAL_DIAL, IMG_GLOBAL_OPTIONS,
				STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				display_index/*(MAX_NVRAM_SOS_ENTRIES + g_phb_sim_list_count )*/,
				g_phb_sos_list, (U16*)gIndexIconsImageList, subMenuDataPtrs, 0, 0, guiBuffer);

	SetLeftSoftkeyFunction (mmi_phb_sos_list_pre_confirm_call ,KEY_EVENT_UP);
#endif

	SetRightSoftkeyFunction (GoBackHistory ,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_HandleSendKeys(mmi_phb_sos_list_pre_confirm_call,KEY_EVENT_DOWN);
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
#if defined(__MMI_PHB_SOS_NUM_EDIT__)
#else
	SetKeyHandler(mmi_phb_sos_list_pre_confirm_call,KEY_ENTER,KEY_EVENT_UP );
#endif

#else
	SetKeyHandler(mmi_phb_sos_list_pre_confirm_call,KEY_SEND,KEY_EVENT_DOWN);
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_sos_list()
* DESCRIPTION
*   Exit function of mmi_phb_entry_sos_list()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_sos_list(void)
{ 
	g_phb_cntx.end_scr_id = SCR_SOS_DISPLAY;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sos_list_get_index(S32)
* DESCRIPTION
*   Gets the index of currently highlighted SOS number.Also sets the LSK accordingly.
*
* PARAMETERS
*  index	IN	highlight index
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sos_list_get_index(S32 index) 
{
	g_phb_cntx.active_index = (S16) index ;
	
#if defined(__MMI_PHB_SOS_NUM_EDIT__)	
	if(pfnUnicodeStrcmp((PS8)subMenuDataPtrs[g_phb_cntx.active_index],(PS8)GetString(STR_SOSLIST_INCARD))==0) 
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_EDIT, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction (mmi_phb_sos_list_pre_entry_number ,KEY_EVENT_UP);
		SetKeyHandler(mmi_phb_sos_list_pre_entry_number, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
#endif	
}

#if defined(__MMI_PHB_SOS_NUM_EDIT__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_sos_list_pre_entry_number()
* DESCRIPTION
*   Calls the function to edit the SOS number 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sos_list_pre_entry_number(void) 
{
	if(!pfnUnicodeStrcmp((PS8)g_phb_sos_list[g_phb_cntx.active_index],(PS8)GetString(STR_GLOBAL_EMPTY_LIST)))
		memset(pbNumber, 0, ENCODING_LENGTH);
	else 
		pfnUnicodeStrcpy((PS8)pbNumber,(PS8)g_phb_sos_list[g_phb_cntx.active_index]);

	mmi_phb_entry_sos_list_enter_number();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_sos_list_enter_number()
* DESCRIPTION
*   Accpt user input for SOS number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_sos_list_enter_number(void) 
{
	U8 *guiBuffer;

	EntryNewScreen(SCR_ENTER_SOS_NUMBER, mmi_phb_exit_sos_list_enter_number, mmi_phb_entry_sos_list_enter_number, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ENTER_SOS_NUMBER); 
	ShowCategory5Screen(STR_GLOBAL_EDIT, IMG_SCR_PBOOK_CAPTION,
			   STR_GLOBAL_OK , IMG_GLOBAL_OK, 
			   STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			   INPUT_TYPE_PHONE_NUMBER,
			   (PU8)pbNumber, MAX_PB_NUMBER_LENGTH + 1, guiBuffer);

	SetLeftSoftkeyFunction(mmi_phb_sos_list_update_entry,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_sos_list_enter_number()
* DESCRIPTION
*   Exit function of mmi_phb_exit_sos_list_enter_number()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_sos_list_enter_number(void) 
{
	g_phb_cntx.end_scr_id = SCR_ENTER_SOS_NUMBER; 
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sos_list_update_entry()
* DESCRIPTION
*   Saves the SOS number on NVRAM
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sos_list_update_entry(void)
{
	S16 error ;
	S32 str_len = 0;
	str_len = pfnUnicodeStrlen((PS8)pbNumber);

	pfnUnicodeStrcpy((PS8)g_pExtraNum_context->g_phb_nvram_list[g_phb_cntx.active_index ].number ,(PS8)pbNumber);
	g_pExtraNum_context->g_phb_nvram_list[g_phb_cntx.active_index].length = (U8 ) str_len ;
	g_pExtraNum_context->g_phb_nvram_list[g_phb_cntx.active_index].type = 0;

#ifdef __MMI_MULTI_SIM__
	str_len = WriteRecord(NVRAM_EF_PHONEBOOK_SOS_LID + g_pExtraNum_context->simId,(U16)(g_phb_cntx.active_index + 1 ),
			&g_pExtraNum_context->g_phb_nvram_list[g_phb_cntx.active_index] ,NVRAM_SOS_RECORD_SIZE,&error);
#else
	str_len = WriteRecord(NVRAM_EF_PHONEBOOK_SOS_LID,(U16)(g_phb_cntx.active_index + 1 ),
	&g_pExtraNum_context->g_phb_nvram_list[g_phb_cntx.active_index] ,NVRAM_SOS_RECORD_SIZE,&error);
#endif


	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	DeleteNScrId(SCR_ENTER_SOS_NUMBER);
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_sos_list_pre_confirm_call()
* DESCRIPTION
*   Asks before calling to an SOS number 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sos_list_pre_confirm_call(void) 
{
	if(pfnUnicodeStrcmp((PS8)g_phb_sos_list[g_phb_cntx.active_index],(PS8)GetString(STR_GLOBAL_EMPTY_LIST))==0)
		DisplayPopup((PU8)GetString(STR_NO_NUMBER_TO_DIAL), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE); 
	else 
		mmi_phb_entry_sos_list_confirm_call();
} 

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_sos_list_confirm_call()
* DESCRIPTION
*   Confirm Make SOS Call
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_sos_list_confirm_call(void) 
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,	
			STR_GLOBAL_NO, IMG_GLOBAL_NO, 
			get_string(STR_GLOBAL_DIAL), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(mmi_phb_sos_list_make_call,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_sos_list_make_call()
* DESCRIPTION
*   Makes call to the SOS number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_sos_list_make_call(void) 
{
	//calling an exit function that does nothing
	EntryNewScreen(SCR_CONFIRM_SOS_CALL, NULL, NULL,NULL);
	MakeCall((S8*)g_phb_sos_list[g_phb_cntx.active_index]);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_check_valid_sos_number(PS8)
* DESCRIPTION
*   number	IN	number for check
*
* PARAMETERS
*  void
* RETURNS
*  U8	number type
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_check_valid_sos_number(PS8 number) 
{
	U16 index = 0;
	S16 error = 0; 
	S32 nNumLen;
	S32 nLen;
	
#ifdef __MMI_MULTI_SIM__
	if(g_pExtraNum_context->g_phb_sos_nvram_read == 0) 
	{
		for(index = 0; index <MAX_NVRAM_SOS_ENTRIES;index++) 
			ReadRecord( NVRAM_EF_PHONEBOOK_SOS_LID + g_pExtraNum_context->simId,(U16)(index + 1),&(g_pExtraNum_context->g_phb_nvram_list[index]),NVRAM_SOS_RECORD_SIZE,&error);

		g_pExtraNum_context->g_phb_sos_nvram_read = 1;
	}
#else
	if(g_pExtraNum_context->g_phb_sos_nvram_read == 0) 
	{
		for(index = 0; index <MAX_NVRAM_SOS_ENTRIES;index++) 
			ReadRecord( NVRAM_EF_PHONEBOOK_SOS_LID ,(U16)(index + 1),&(g_pExtraNum_context->g_phb_nvram_list[index]),NVRAM_SOS_RECORD_SIZE,&error);

		g_pExtraNum_context->g_phb_sos_nvram_read = 1;
	}	
#endif

	nNumLen = pfnUnicodeStrlen(number);
	for(index = 0; index < g_phb_sim_list_count  ;index++)	
	{
		nLen = pfnUnicodeStrlen((S8*)g_phb_sim_list[index].tel.number);
		if(nLen==nNumLen)	
		{
			if((pfnUnicodeStrcmp((PS8)g_phb_sim_list[index].tel.number,(PS8)number)==0))
			return 2;
		}
		else if(nLen>nNumLen) 
		{
			if((pfnUnicodeStrncmp((PS8)g_phb_sim_list[index].tel.number,(PS8)number,nNumLen)==0))
			return 1;
  		}
	}

	for(index = 0; index < MAX_NVRAM_SOS_ENTRIES ;index++) 
	{
		nLen = pfnUnicodeStrlen((S8*)g_pExtraNum_context->g_phb_nvram_list[index].number);
		if(nLen==nNumLen) 
		{
			if((pfnUnicodeStrcmp((PS8)g_pExtraNum_context->g_phb_nvram_list[index].number,(PS8)number)==0))
			return 2;
		}
		else if(nLen>nNumLen) 
		{
			if((pfnUnicodeStrncmp((PS8)g_pExtraNum_context->g_phb_nvram_list[index].number,(PS8)number,nNumLen)==0))
			return 1;
		}
	}
	return FALSE ;
}

/* --------------------------------------  END of Phone Book SOS(Emergeny) Number -------------------------------------------*/

#define MMI_PHB_FDN_SEARCH_LIST
/*****************************************************************************
* FUNCTION
*  mmi_phb_fdn_get_list_to_ram
* DESCRIPTION
*   This function is to load FDN list into ADN storage. Because when FDN is enable
*   ADN will not be read from SIM card. This FDN list is for number lookup in call.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   
*****************************************************************************/
extern U8 gSecSetupMenuSimID;
void mmi_phb_fdn_get_list_to_ram(void)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_fdn_get_list_to_ram.>\n",__FILE__,__LINE__));


//	if(g_phb_cntx.sim_used == 0)   /*Guojian Modify Start 9757 dd Ver: GreenStone  on 2008-10-14 13:43 */
	if(TRUE)
	{	
		g_phb_fdn_count[gSecSetupMenuSimID] = 0;
		mmi_phb_fdn_get_list_to_ram_req();
	}
	else
	{
		PhbGetSpeedDialInfo(); /* Retrieve Speed Dial List after retrieve FDN list, because they all use same message.*/
		mmi_phb_get_disp_owner_num();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_fdn_get_list_to_ram_req
* DESCRIPTION
*   This function is to retrieve FDN list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_phb_fdn_get_list_to_ram_req(void)
{
	MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *pMsgIndexReq = NULL ;
	MYQUEUE	Message;
	U8 storage = MMI_SIM;
	
	pMsgIndexReq = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT));

	pMsgIndexReq->index= g_phb_fdn_count[gSecSetupMenuSimID] + 1;
	pMsgIndexReq->record_index = 0xFFFF;
	pMsgIndexReq->storage = storage ;
	pMsgIndexReq->type = MMI_PHB_FDN ;
	pMsgIndexReq->no_data = MAX_ENTRIES_IN_LIST;
	
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C + gSecSetupMenuSimID;

	Message.oslMsgId = PRT_PHB_GET_ENTRY_BY_INDEX_REQ;
	Message.oslDataPtr = (oslParaType *)pMsgIndexReq;
	Message.oslPeerBuffPtr = NULL;

	SetProtocolEventHandlerExt(mmi_phb_fdn_get_list_to_ram_rsp, PRT_PHB_GET_ENTRY_BY_INDEX_RSP,gSecSetupMenuSimID);

	OslMsgSendExtQueue(&Message);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_fdn_get_list_to_ram_rsp
* DESCRIPTION
*   This function is to convert FDN retrievie result to ADN storage.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   
*****************************************************************************/
extern void GetFDLToContext(void *info);
void mmi_phb_fdn_get_list_to_ram_rsp(void *info)
{
	U8 nSimID;
	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U8 i;
	BOOL bFdl = MMI_FALSE;
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
//	U16 i;
//	U16 store_index;
//qiff modify for bug:10650,the RSP handler should be set in REQ function
 	SetProtocolEventHandlerExt(NULL, PRT_PHB_GET_ENTRY_BY_INDEX_RSP,nSimID);

	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *)info;

#if 0  //for bug 9757 get the fdn list to gFDLBDLContext, not PhoneBook[]. for our fdn process, not the same with previous one
	if (localPtr->result.flag == 0 /*OK*/) 
	{
		for(i= 0; i < localPtr->no_list; i++)
		{
			if(g_phb_fdn_count < MAX_SIM_ENTRIES) 
			{
				store_index = g_phb_fdn_count + MAX_PB_PHONE_ENTRIES; 
				g_phb_fdn_count++;
			}
			else 
				continue;
		
			/*Parse Name Field*/
			PhoneBook[store_index].alpha_id.name_length = localPtr->list[i].alpha_id.name_length;
			PhoneBook[store_index].alpha_id.name_dcs = localPtr->list[i].alpha_id.name_dcs;

			InputboxConvertGSMToDeviceEncoding(localPtr->list[i].alpha_id.name, 
															  PhoneBook[store_index].alpha_id.name, 
															  localPtr->list[i].alpha_id.name_length,
															  (MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH, 
															  localPtr->list[i].alpha_id.name_dcs, 
															  0, 
															  1);

			/*Parse Tel Number*/
			PhoneBook[store_index].tel.type		= localPtr->list[i].tel.type;
			PhoneBook[store_index].tel.length		= localPtr->list[i].tel.length;
			mmi_phb_convert_to_bcd(PhoneBook[store_index].tel.number, localPtr->list[i].tel.number, (MAX_PB_NUMBER_LENGTH+1)/2);
			
		}
		GetFDLToContext(info);
		if (localPtr->no_list == MAX_ENTRIES_IN_LIST)
		{
			mmi_phb_fdn_get_list_to_ram_req();
			return;
		}
	}
#else
	if (localPtr->result.flag == 0 /*OK*/) 
	{
		GetFDLToContext(info);
		if (localPtr->no_list == MAX_ENTRIES_IN_LIST)
		{
			mmi_phb_fdn_get_list_to_ram_req();
			return;
		}
	}
#endif
	/* Retrieve Speed Dial List after retrieve FDN list, because they all use same message.*/
#ifdef __MMI_MULTI_SIM__
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SecuritySetupContext.FdlStatus[i] && (!g_FixedInit[i]))
		{
			g_FixedInit[i] = TRUE;
			g_phb_fdn_count[i] = 0;	
			bFdl = MMI_TRUE;
			SECSETSetSIMIdforPIN(i);
        		mmi_phb_fdn_get_list_to_ram_req();
			break;
		}
	}
	if(!bFdl)
	{
		PhbGetSpeedDialInfo();
		mmi_phb_get_disp_owner_num();	
	}
#else
	PhbGetSpeedDialInfo();
	mmi_phb_get_disp_owner_num();
#endif	
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_fdn_search_by_number
* DESCRIPTION
*   This function is to match calling number with FDN list store in ADN storage
*   Only match when FDN is enable
* PARAMETERS
*  number  IN : ASCII format number
* 
* RETURNS
*  result index
* GLOBALS AFFECTED
*   
*****************************************************************************/
U16 mmi_phb_fdn_search_by_number(S8* number)
{
	U16 i;
	U16 store_index;
	U8 tempNumber[MAX_PB_NUMBER_LENGTH + 1 + 1];

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_fdn_search_by_number.>\n",__FILE__,__LINE__));

	if(g_phb_cntx.sim_used > 0) /*ADN is read out from SIM card, so there is no storing FDN entry data.*/
		return 0xffff;

	for(i = 0 ; i < g_phb_fdn_count[0] ; i++)
	{
		store_index = i + MAX_PB_PHONE_ENTRIES; 
		if(PhoneBook[store_index].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR) 
		{
			tempNumber[0] = '+';
			mmi_phb_convert_to_digit((U8*)(tempNumber + 1), PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
		}
		else 
		{
			mmi_phb_convert_to_digit(tempNumber, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
		}

		if(!strcmp((S8*)tempNumber, number)) 
		{
			return store_index;
		}
			
	}

	return 0xffff;
	
}
	
/*****************************************************************************
* FUNCTION
*  mmi_phb_get_sim_entry
* DESCRIPTION
*   This function is to get sdn entry for mmi_phb_call_get_call_data_for_call_mgnt
* PARAMETERS
*  NONE
* 
* RETURNS
*  result MMI_PHB_ENTRY_STRUCT*
* GLOBALS AFFECTED
*   
*****************************************************************************/
MMI_PHB_ENTRY_STRUCT* mmi_phb_get_sim_entry(void)
{
	return &g_phb_sim_list[g_phb_cntx.active_index];	
}

#endif /* _PHONEBOOKEXTRANUMBER_C */


