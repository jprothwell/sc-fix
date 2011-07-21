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
 *	CallFixedDialList.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Fixed Dial List functions
 *
 * Author:
 * -------
 * -------
 *
*==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: CallFixedDialList.c

  	PURPOSE		: Call Setting Application

 

	 

	DATE		: 

**************************************************************/
#define __NEWSIMULATOR
#include "mmi_features.h"
#include "debuginitdef.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mainmenudef.h"
#include "protocolevents.h"
#include "phonebookdef.h"
#include "wgui_categories.h"
#include "settinggprots.h"
#include "callfixeddial.h"
#include "commonscreens.h"
#include "callmanagementgprot.h"
#include "phonebookgprot.h"
#include "gsm7bitdefaultalphabet.h"
#include "settingdefs.h"
#include "settingsgexdcl.h"
#include "settingsgdcl.h"
#include "securitysetup.h"
//micha0901
#include "profilegprots.h"
#include "simdetectiondef.h"

#include "gbhmi.h" //added by zhoumn for the bug of editor full @2007/06/16

#undef __NEWSIMULATOR

 
#include "wgui_categories_popup.h"
#include "wgui_categories_inputs.h"
#include "mmi_trace.h"
#include "mmi.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#endif

FDLBDLContext gFDLBDLContext[MMI_SIM_NUMBER];
FDLBDLContext *gpFDLBDLContext = gFDLBDLContext;



extern BOOL g_FixedInit[];
extern BOOL gSimReady[];

extern U8 gSecSetupMenuSimID;
/**************************************************************

	FUNCTION NAME		: InitFixedDialList(void)
  	PURPOSE				: Initialize FDL.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void InitFixedDialList(void)
{
	SetHiliteHandler(MENU8234_FDL_VIEW_OPTIONS_ADD,HighlighSrcAddFDL);
	SetHiliteHandler(MENU8234_FDL_VIEW_OPTIONS_EDIT,HighlightScrEditFDL);
	SetHiliteHandler(MENU8234_FDL_VIEW_OPTIONS_DELETE,HighLightDeleteFDL);
	gpFDLBDLContext->FDNBDNCount = 0;
	gpFDLBDLContext->FDNBDNEdit = 0;
	gpFDLBDLContext->HighlightedFDN = 255;

}

/**************************************************************

	FUNCTION NAME		: SECSETGetFdlEntriesReq(void)
  	PURPOSE				: getting all entries req
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/

void SECSETGetFdlEntriesReq()
{
	MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *pMsgIndexReq = NULL ;
	MYQUEUE	Message;
	module_type dest_mod_id = MOD_L4C + gSecSetupMenuSimID;

	gpFDLBDLContext = &gFDLBDLContext[gSecSetupMenuSimID];
	
	mmi_trace(1, TSTXT("SECSETGetFdlEntriesReq = %d"),gpFDLBDLContext->FDNBDNCount);
	pMsgIndexReq = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT));
	pMsgIndexReq->index=gpFDLBDLContext->FDNBDNCount + 1;
	pMsgIndexReq->record_index=0xFFFF;
	pMsgIndexReq->storage=MMI_SIM ;
	pMsgIndexReq->type=MMI_PHB_FDN ;
	pMsgIndexReq->no_data=MAX_ENTRIES_IN_LIST;
	
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;

	Message.oslMsgId = PRT_PHB_GET_ENTRY_BY_INDEX_REQ;
	Message.oslDataPtr = (oslParaType *)pMsgIndexReq;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandlerExt(SECSETGetFdlEntriesRsp, PRT_PHB_GET_ENTRY_BY_INDEX_RSP,gSecSetupMenuSimID);

	OslMsgSendExtQueue(&Message);

}

/**************************************************************

	FUNCTION NAME		: SECSETGetFdlEntriesRsp(void)
  	PURPOSE				: getting all entries rsp
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETGetFdlEntriesRsp(void *info)
{
	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U8 i= 0,j=0;
//micha0317
	U8 TempBuff[(MAX_PB_NUMBER_LENGTH+2)*ENCODING_LENGTH];
	U8 *tmpConvertBuf = NULL;
	U16 convertOutLen = 0;
	U8 name_dcs;//Renwy add 2008/06/25 for '$' error 
	U8 nSimID;

	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	mmi_trace(1, TSTXT("SECSETGetFdlEntriesRsp"));
	SetProtocolEventHandlerExt(NULL, PRT_PHB_GET_ENTRY_BY_INDEX_RSP,nSimID);

	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *)info;

	if (localPtr->result.flag == 0 /*OK*/) 
	{
		 mmi_trace(1, TSTXT("if (localPtr->result.flag == 0 /*OK*/) "));
		i =  gpFDLBDLContext->FDNBDNCount;
		gpFDLBDLContext->FDNBDNCount +=  localPtr->no_list;	//Add number of new entries in current list length.

		if (gpFDLBDLContext->FDNBDNCount > MAX_SIM_ENTRIES)
			gpFDLBDLContext->FDNBDNCount = MAX_SIM_ENTRIES;

		for (; i < gpFDLBDLContext->FDNBDNCount; i++, j++) 
		{
			gpFDLBDLContext->FDLBDLEntries[i].storage = localPtr->list[j].storage;
			gpFDLBDLContext->FDLBDLEntries[i].index = localPtr->list[j].index;
			gpFDLBDLContext->FDLBDLEntries[i].record_index = localPtr->list[j].record_index;

			memset(gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name, 0, MAX_NAME_LEN_FSB_LIST*ENCODING_LENGTH);
			memset(gpFDLBDLContext->FDLBDLEntries[i].tel.number, 0, MAX_NUMBER_LEN_FSB_LIST*ENCODING_LENGTH);
			gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_length = localPtr->list[j].alpha_id.name_length;
			gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_dcs = localPtr->list[j].alpha_id.name_dcs;

			
			{//Renwy add begine 2008/06/25 for '$' error 

				name_dcs = gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_dcs;
				
				if (MMI_PHB_ASCII == gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_dcs)
				{
					name_dcs = MMI_8BIT_DCS;
				}
				
			}//Renwy add end 2008/06/25 for '$' error 
			tmpConvertBuf = CovertStringForPlatform(localPtr->list[j].alpha_id.name, gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_length,name_dcs,&convertOutLen);//Renwy modify 2008/06/25
			//tmpConvertBuf = CovertStringForPlatform(localPtr->list[j].alpha_id.name, gFDLBDLContext.FDLBDLEntries[i].alpha_id.name_length,gFDLBDLContext.FDLBDLEntries[i].alpha_id.name_dcs,&convertOutLen);

			gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_length = (U8) convertOutLen;

			if (pfnUnicodeStrlen((PS8) tmpConvertBuf) <= MAX_PB_NAME_LENGTH) {
				pfnUnicodeStrcpy((PS8) gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name, (PS8) tmpConvertBuf);
			}
			else {
				pfnUnicodeStrncpy((PS8) gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name, (PS8) tmpConvertBuf, MAX_PB_NAME_LENGTH);
			}

			if (tmpConvertBuf!=NULL)
				OslMfree(tmpConvertBuf);

			if (localPtr->list[j].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR) 
			{
				gpFDLBDLContext->FDLBDLEntries[i].tel.type = localPtr->list[j].tel.type;
				gpFDLBDLContext->FDLBDLEntries[i].tel.number[0] = '+';//insert '+' before the number.
				gpFDLBDLContext->FDLBDLEntries[i].tel.length = localPtr->list[j].tel.length;
				AnsiiToUnicodeString((PS8)TempBuff, (PS8)localPtr->list[j].tel.number);
				memcpy(gpFDLBDLContext->FDLBDLEntries[i].tel.number + ENCODING_LENGTH, TempBuff, (localPtr->list[j].tel.length)*ENCODING_LENGTH);
			} 
			else 
			{
				gpFDLBDLContext->FDLBDLEntries[i].tel.type = localPtr->list[j].tel.type;
				gpFDLBDLContext->FDLBDLEntries[i].tel.length = localPtr->list[j].tel.length;
				AnsiiToUnicodeString((PS8)TempBuff, (PS8)localPtr->list[j].tel.number);
				memcpy(gpFDLBDLContext->FDLBDLEntries[i].tel.number, (PS8)TempBuff, (localPtr->list[j].tel.length)*ENCODING_LENGTH);
			}

		}
		if (localPtr->no_list == MAX_ENTRIES_IN_LIST)
		{
			 mmi_trace(1, TSTXT("	if (localPtr->no_list == MAX_ENTRIES_IN_LIST) "));
			SECSETGetFdlEntriesReq();
		}
		else 
		{
			 mmi_trace(1, TSTXT("	ViewFDList "));
		 	if(g_FixedInit[nSimID] != TRUE)////man for fdn
		 	{
				ViewFDList();
			}
			gSimReady[nSimID] = TRUE;
			
			/*
			*    Retrieve FDN list to ADN buffer when FDN is enable.
			*    Because ADN will not read when FDN is on, so use ADN's buffer to store FDN temporarily
			*    This is for call lookup FDN number usage.
			*/   
		//	if(g_SecuritySetupContext.FdlStatus)
		//		mmi_phb_fdn_get_list_to_ram();
		}
	}
	else if (localPtr->result.flag == 1 && localPtr->result.cause == 5) 
	{
//		gFDLBDLContext.FDNBDNCount = 0;

		gSimReady[nSimID] = TRUE;
		ViewFDList();
	}
	else 
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_ID_SECSET_FIX_DIAL);
	}
}

//added by guojian :get the fdn list to gFDLBDLContext, not PhoneBook[]. for our fdn process, not the same with previous one
void GetFDLToContext(void *info)
{
	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U8 i= 0,j=0;
//micha0317
	U8 TempBuff[(MAX_PB_NUMBER_LENGTH+2)*ENCODING_LENGTH];
	U8 *tmpConvertBuf = NULL;
	U16 convertOutLen = 0;
	U8 name_dcs;//Renwy add 2008/06/25 for '$' error 
	U8 nSimId = 0;

	#ifdef __MMI_MULTI_SIM__
	nSimId = gSecSetupMenuSimID;
	#endif
	
	 mmi_trace(1, TSTXT("GetFDLToContext"));

	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *)info;

	if (localPtr->result.flag == 0 /*OK*/) 
	{
		 mmi_trace(1, TSTXT("if GetFDLToContext (localPtr->result.flag == 0 /*OK*/) "));
		i =  gFDLBDLContext[nSimId].FDNBDNCount;
		gFDLBDLContext[nSimId].FDNBDNCount =  localPtr->no_list;	//Add number of new entries in current list length.

		mmi_trace(TRUE," Func: %s gpFDLBDLContext->FDNBDNCount=%d", __FUNCTION__,gFDLBDLContext[nSimId].FDNBDNCount);
		if (gFDLBDLContext[nSimId].FDNBDNCount > MAX_SIM_ENTRIES)
			gFDLBDLContext[nSimId].FDNBDNCount = MAX_SIM_ENTRIES;

		for (; i < gFDLBDLContext[nSimId].FDNBDNCount; i++, j++) 
		{
			gFDLBDLContext[nSimId].FDLBDLEntries[i].storage = localPtr->list[j].storage;
			gFDLBDLContext[nSimId].FDLBDLEntries[i].index = localPtr->list[j].index;
			gFDLBDLContext[nSimId].FDLBDLEntries[i].record_index = localPtr->list[j].record_index;

			memset(gFDLBDLContext[nSimId].FDLBDLEntries[i].alpha_id.name, 0, MAX_NAME_LEN_FSB_LIST*ENCODING_LENGTH);
			memset(gFDLBDLContext[nSimId].FDLBDLEntries[i].tel.number, 0, MAX_NUMBER_LEN_FSB_LIST*ENCODING_LENGTH);
			gFDLBDLContext[nSimId].FDLBDLEntries[i].alpha_id.name_length = localPtr->list[j].alpha_id.name_length;
			gFDLBDLContext[nSimId].FDLBDLEntries[i].alpha_id.name_dcs = localPtr->list[j].alpha_id.name_dcs;

			
			{//Renwy add begine 2008/06/25 for '$' error 

				name_dcs = gFDLBDLContext[0].FDLBDLEntries[i].alpha_id.name_dcs;
				
				if (MMI_PHB_ASCII == gFDLBDLContext[0].FDLBDLEntries[i].alpha_id.name_dcs)
				{
					name_dcs = MMI_8BIT_DCS;
				}
				
			}//Renwy add end 2008/06/25 for '$' error 
			tmpConvertBuf = CovertStringForPlatform(localPtr->list[j].alpha_id.name, gFDLBDLContext[nSimId].FDLBDLEntries[i].alpha_id.name_length,name_dcs,&convertOutLen);//Renwy modify 2008/06/25
			//tmpConvertBuf = CovertStringForPlatform(localPtr->list[j].alpha_id.name, gFDLBDLContext.FDLBDLEntries[i].alpha_id.name_length,gFDLBDLContext.FDLBDLEntries[i].alpha_id.name_dcs,&convertOutLen);

			gFDLBDLContext[nSimId].FDLBDLEntries[i].alpha_id.name_length = (U8) convertOutLen;

			if (pfnUnicodeStrlen((PS8) tmpConvertBuf) <= MAX_PB_NAME_LENGTH) {
				pfnUnicodeStrcpy((PS8) gFDLBDLContext[nSimId].FDLBDLEntries[i].alpha_id.name, (PS8) tmpConvertBuf);
			}
			else {
				pfnUnicodeStrncpy((PS8) gFDLBDLContext[nSimId].FDLBDLEntries[i].alpha_id.name, (PS8) tmpConvertBuf, MAX_PB_NAME_LENGTH);
			}

			if (tmpConvertBuf!=NULL)
				OslMfree(tmpConvertBuf);

			if (localPtr->list[j].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR) 
			{
				gFDLBDLContext[nSimId].FDLBDLEntries[i].tel.type = localPtr->list[j].tel.type;
				gFDLBDLContext[nSimId].FDLBDLEntries[i].tel.number[0] = '+';//insert '+' before the number.
				gFDLBDLContext[nSimId].FDLBDLEntries[i].tel.length = localPtr->list[j].tel.length;
				AnsiiToUnicodeString((PS8)TempBuff, (PS8)localPtr->list[j].tel.number);
				memcpy(gFDLBDLContext[nSimId].FDLBDLEntries[i].tel.number + ENCODING_LENGTH, TempBuff, (localPtr->list[j].tel.length)*ENCODING_LENGTH);
			} 
			else 
			{
				gFDLBDLContext[nSimId].FDLBDLEntries[i].tel.type = localPtr->list[j].tel.type;
				gFDLBDLContext[nSimId].FDLBDLEntries[i].tel.length = localPtr->list[j].tel.length;
				AnsiiToUnicodeString((PS8)TempBuff, (PS8)localPtr->list[j].tel.number);
				memcpy(gFDLBDLContext[nSimId].FDLBDLEntries[i].tel.number, (PS8)TempBuff, (localPtr->list[j].tel.length)*ENCODING_LENGTH);
			}

		}
	}
}
/**************************************************************

	FUNCTION NAME		: ViewFDList(void)
  	PURPOSE				: View FDL List
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void ViewFDList()
{
	gpFDLBDLContext->HighlightedFDN=0;
	EntrySECSETFDNList();
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETFDNList(void)
  	PURPOSE				: Entry screen to view FDl list
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETFDNList()
{

	U8 *pstfdlist[MAX_SIM_ENTRIES],*fdlHint[MAX_SIM_ENTRIES];
	U16 index;
	U8* guiBuffer;
	U16 fdlimagelist[MAX_SIM_ENTRIES];
	
	EntryNewScreen(SCR_ID_SECSET_FDN_LIST, NULL, EntrySECSETFDNList, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_FDN_LIST);		/* Getting the Index from history */

	for(index = 0; index < gpFDLBDLContext->FDNBDNCount; index++) 
	{
		if(gpFDLBDLContext->FDLBDLEntries[index].alpha_id.name[0] != '\0')
		{
			pstfdlist[index] = (U8*)gpFDLBDLContext->FDLBDLEntries[index].alpha_id.name;
			fdlHint[index] = (U8*)gpFDLBDLContext->FDLBDLEntries[index].tel.number;
		}
		else
		{
			pstfdlist[index] = (U8*)gpFDLBDLContext->FDLBDLEntries[index].tel.number;
			fdlHint[index] = NULL;
		}
		fdlimagelist[index]= IMG_STORAGE_SIM;
	}

	RegisterHighlightHandler(GetActiveIndex);

	if( guiBuffer != NULL)
	{
		memset( (void *) guiBuffer, 0, 2);
	}

	if (( gpFDLBDLContext->HighlightedFDN >= gpFDLBDLContext->FDNBDNCount) && (gpFDLBDLContext->FDNBDNCount > 0)) 
		gpFDLBDLContext->HighlightedFDN = gpFDLBDLContext->FDNBDNCount-1;

	
	if(gpFDLBDLContext->FDNBDNCount == 0)
	{
		pstfdlist[0] = (U8*)GetString(STR_GLOBAL_EMPTY_LIST);
		fdlimagelist[0]= 0;
		fdlHint[0] = NULL;
		ShowCategory6Screen(STR_SCR8054_CALL_FDL_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON, 
							STR_GLOBAL_ADD, IMG_GLOBAL_OPTIONS, 
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 1, 
							pstfdlist, NULL,0, guiBuffer);
		gpFDLBDLContext->FDNBDNEdit = FALSE ;
		memset(gpFDLBDLContext->FDLBDLName,0,(MAX_NAME_LEN_FSB_LIST+1)*ENCODING_LENGTH);
		memset(gpFDLBDLContext->FDLBDLNumber,0,(MAX_NUMBER_LEN_FSB_LIST+1+1)*ENCODING_LENGTH);
		SetLeftSoftkeyFunction(EntrySECSETFDNDetails,KEY_EVENT_UP);
//micha0529
		SetKeyHandler(EntrySECSETFDNDetails, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(0,KEY_SEND,KEY_EVENT_UP);
	}
	else
	{

		ShowCategory53Screen(STR_SCR8054_CALL_FDL_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
//micha0610
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  gpFDLBDLContext->FDNBDNCount, pstfdlist,fdlimagelist, 
						  fdlHint,0, gpFDLBDLContext->HighlightedFDN, guiBuffer);

		SetLeftSoftkeyFunction(EntrySECSETFDNOption,KEY_EVENT_UP);
//micha0529
		SetKeyHandler(EntrySECSETFDNOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		#ifdef __MMI_MULTI_SIM__
		MTPNP_PFAL_CC_HandleSendKeys(FdlMakeCall, KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
		ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
		SetKeyHandler(EntrySECSETFDNOption,KEY_ENTER,KEY_EVENT_UP );
#endif
		#else
		SetKeyHandler(FdlMakeCall, KEY_SEND, KEY_EVENT_DOWN);
		#endif
	}
}


/**************************************************************

	FUNCTION NAME		: ExitSECSETFDNListReload(void)
  	PURPOSE				: 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void ExitSECSETFDNListReload(void)
{
	history_t   Scr;
	S16 nHistory = 0;	
	DeleteScreenIfPresent(SCR_ID_SECSET_FDN_LIST);
	Scr.scrnID = SCR_ID_SECSET_FDN_LIST;
	Scr.entryFuncPtr = EntryFdlList;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETFDNOption(void)
  	PURPOSE				: Entry screen of FDL list Options, when list is not empty.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETFDNOption()
{
	
	U16 nStrItemList[MAX_SUB_MENUS];/* Stores the strings id of submenus returned */
	U16 nNumofItem;		 /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */

	EntryNewScreen(SCR_ID_SECSET_FDN_OPTION, NULL, EntrySECSETFDNOption, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_FDN_OPTION);	

	/* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU8234_FDL_VIEW_OPTIONS_MAIN	);

	/* 4. Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU8234_FDL_VIEW_OPTIONS_MAIN	);
	
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU8234_FDL_VIEW_OPTIONS_MAIN	, nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU8234_FDL_VIEW_OPTIONS_MAIN	);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* 8 Display Category15 Screen */
	ShowCategory15Screen(STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/**************************************************************

	FUNCTION NAME		: HighlighSrcAddFDL(void)
  	PURPOSE				: add option
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void HighlighSrcAddFDL()
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntrySECSETFDNDetails, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntrySECSETFDNDetails,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	gpFDLBDLContext->FDNBDNEdit = FALSE ;
	memset(gpFDLBDLContext->FDLBDLName,0,(MAX_NAME_LEN_FSB_LIST+1)*ENCODING_LENGTH);
	memset(gpFDLBDLContext->FDLBDLNumber,0,(MAX_NUMBER_LEN_FSB_LIST+1+1)*ENCODING_LENGTH);
}

void mmi_EntrySECSETFDNDetails_hdlr(S32 index)
{
	switch(index)
	{
	case 0:
			ChangeTitleString((PU8)GetString(STR_ID_PHB_NAME));
		break;
	case 1:
			ChangeTitleString((PU8)GetString(STR_ID_PHB_MOBILE_NUMBER));
		break;
	default:
		break;
	}
	draw_title();
}
/**************************************************************

	FUNCTION NAME		: EntrySECSETFDNDetails(void)
  	PURPOSE				: Entry screen to add FDL entry.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETFDNDetails(void)
{
	U16 nNumofItem;      /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U16	ItemIcons[2];
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
	mmi_trace(1, TSTXT("EntrySECSETFDNDetails"));
	if((gpFDLBDLContext->FDNBDNCount == (U16)gpFDLBDLContext->MaxFDL ||gpFDLBDLContext->FDNBDNCount == MAX_SIM_ENTRIES)  &&( gpFDLBDLContext->FDNBDNEdit == FALSE))
	{
		DisplayPopup((U8*)GetString(STR_SETTING_STORAGE_FULL),IMG_GLOBAL_WARNING,0,ST_NOTIFYDURATION,(U8)WARNING_TONE);
	}
	else
	{
		EntryNewScreen(SCR_ID_SECSET_FDN_DETAILS, ExitSECSETFDNDetails, NULL, NULL);

		guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_FDN_DETAILS);
		inputBuffer = GetCurrNInputBuffer ( SCR_ID_SECSET_FDN_DETAILS, &inputBufferSize );    //added for inline edit history

		nNumofItem = GetNumOfChild(MENU8234_FDL_VIEW_OPTIONS_ADD);
		
//		nDispAttribute = GetDispAttributeOfItem(MENU8234_FDL_VIEW_OPTIONS_ADD);

		GetSequenceImageIds(MENU8234_FDL_VIEW_OPTIONS_ADD,ItemIcons);

		InitializeCategory57Screen();
		RegisterHighlightHandler(mmi_EntrySECSETFDNDetails_hdlr);

		FillFdlInlineEditStruct();

		if(inputBuffer!=NULL)                                            //added for inline edit history
			SetCategory57Data(gpFDLBDLContext->FDLBDLInlineDisplay, nNumofItem, inputBuffer);//sets the data 

		ShowCategory57Screen(STR_SETTING_FDL_EDIT_ENTRY,MAIN_MENU_TITLE_SETTINGS_ICON,
							STR_EDIT,IMG_GLOBAL_OK,
							STR_DONE, IMG_GLOBAL_BACK,
							nNumofItem,ItemIcons,gpFDLBDLContext->FDLBDLInlineDisplay,
							0,guiBuffer);
	#if defined(__MMI_GB__)
		mmi_frm_set_highlight_handler();
		GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone); 
	#endif
		SetCategory57RightSoftkeyFunctions( PreNewFdlEntryReq, GoBackHistory);
		mmi_trace(1, TSTXT("SCR_ID_SECSET_FDN_DETAILS"));
	}
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETFDNDetails(void)
  	PURPOSE				: Exit fn for FDL add screen
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void ExitSECSETFDNDetails(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	S16 nHistory = 0;
	U16 size;

	CloseCategory57Screen();   
	Scr.scrnID = SCR_ID_SECSET_FDN_DETAILS;
	Scr.entryFuncPtr = EntrySECSETFDNDetails;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	size = (U16)GetCategory57DataSize( );     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, size);   
}

/**************************************************************

	FUNCTION NAME		: FillFdlInlineEditStruct(void)
  	PURPOSE				: Fill Inline Edit structs.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void FillFdlInlineEditStruct(void)
{
	SetInlineItemActivation(&gpFDLBDLContext->FDLBDLInlineDisplay[0],KEY_LSK,KEY_EVENT_UP);
	SetInlineItemFullScreenEdit(&gpFDLBDLContext->FDLBDLInlineDisplay[0],STR_SETTING_FDL_NAME,MAIN_MENU_TITLE_SETTINGS_ICON,(PU8)gpFDLBDLContext->FDLBDLName,
        (14 + 1) ,INPUT_TYPE_ALPHANUMERIC_UPPERCASE | INPUT_TYPE_USE_ENCODING_BASED_LENGTH | INPUT_TYPE_ONE_LESS_CHARACTER);
	SetInlineFullScreenEditCustomFunction(&gpFDLBDLContext->FDLBDLInlineDisplay[0], SettingInlineCustomFunction);
	SetInlineItemActivation(&gpFDLBDLContext->FDLBDLInlineDisplay[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&gpFDLBDLContext->FDLBDLInlineDisplay[1], (PU8)gpFDLBDLContext->FDLBDLNumber, MAX_PB_NUMBER_LENGTH+1, INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING );
	DisableInlineItemHighlight(&gpFDLBDLContext->FDLBDLInlineDisplay[1]);
}

/**************************************************************

	FUNCTION NAME		: PreNewFdlEntryReq(void)
  	PURPOSE				: Goto Confirm Save screen.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void PreNewFdlEntryReq()
{
	if(!pfnUnicodeStrlen((PS8)gpFDLBDLContext->FDLBDLNumber) && !pfnUnicodeStrlen((PS8)gpFDLBDLContext->FDLBDLName))
	{
		GoBackHistory();
	}
	else
	{
		EntrySECSETFDNSaveConfirm();
	}
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETFDNSaveConfirm(void)
  	PURPOSE				: Show confirm Save Screen
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETFDNSaveConfirm(void)
{
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_GLOBAL_SAVE),IMG_GLOBAL_QUESTION, WARNING_TONE);

	g_SecuritySetupContext.VerifyPassSuccessCallBack = SECSETSetFdlEntryReq;
	SetLeftSoftkeyFunction(SECSETGetPIN2StatusReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoToFDLList,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETFDNSaveConfirm(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETFDNSaveConfirm(void)
{
//micha0531
	stopRequestedTone(WARNING_TONE);
}

/**************************************************************

	FUNCTION NAME		: SECSETSetFdlEntryReq(void)
 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 
	 

**************************************************************/
void SECSETSetFdlEntryReq(void)
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *pMsgNewEntry = NULL ;
	MYQUEUE Message;
	//U8 pbName_ASCII[MAX_PB_NAME_LENGTH];
	U8 pbNumber_ASCII[MAX_PB_NUMBER_LENGTH+1];		
	module_type dest_mod_id = MOD_L4C + gSecSetupMenuSimID;


	EntrySECSETFDNBDNSaving();
	pMsgNewEntry  = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));

	 
	//memset(pMsgNewEntry->list.alpha_id.name, 0, strlen((PS8)pbName_ASCII)+1);
	//memset(pMsgNewEntry->list.tel.number, 0, strlen((PS8)pbNumber_ASCII)+1);
	memset(pMsgNewEntry->list.alpha_id.name, 0, MAX_PB_NAME_LENGTH+1);
	memset(pMsgNewEntry->list.tel.number, 0, MAX_PB_NAME_LENGTH+1);
	 

	pMsgNewEntry->type = MMI_PHB_FDN;
	pMsgNewEntry->no_data = 1;
	pMsgNewEntry->list.storage = MMI_SIM + gSecSetupMenuSimID;
	pMsgNewEntry->list.index = 0xFFFF; // For New Entry It should Always be 0xFFFF;
	pMsgNewEntry->list.record_index = 0xFFFF; // For New Entry It should be alaways 0xFFFF

	if(gpFDLBDLContext->FDNBDNEdit)//If entry is edited
	{
		pMsgNewEntry->list.index = gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedFDN].index;
		pMsgNewEntry->list.record_index = gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedFDN].record_index;
	}
	if (gpFDLBDLContext->FDLBDLNumber[0] == '+')//If Number is International
	{
		pMsgNewEntry->list.tel.type = MMI_CSMCC_INTERNATIONAL_ADDR; //INTERNATIONAL
		pMsgNewEntry->list.tel.length = (U8)pfnUnicodeStrlen((const S8 *)(gpFDLBDLContext->FDLBDLNumber+ ENCODING_LENGTH));
		UnicodeToAnsii((PS8)pbNumber_ASCII,(PS8)( gpFDLBDLContext->FDLBDLNumber+ ENCODING_LENGTH));
		memcpy(pMsgNewEntry->list.tel.number, pbNumber_ASCII, strlen((PS8)pbNumber_ASCII)+1);
	}
	else
	{
		pMsgNewEntry->list.tel.type = MMI_CSMCC_DEFAULT_ADDR_TYPE; //DEFAULT
		pMsgNewEntry->list.tel.length = (U8)pfnUnicodeStrlen((PS8)gpFDLBDLContext->FDLBDLNumber);
		UnicodeToAnsii((PS8)pbNumber_ASCII, (PS8)gpFDLBDLContext->FDLBDLNumber);
		memcpy(pMsgNewEntry->list.tel.number, pbNumber_ASCII, strlen((PS8)pbNumber_ASCII)+1);
	}
	if(GetUCS2Flag((PS8)gpFDLBDLContext->FDLBDLName)) {
		pMsgNewEntry->list.alpha_id.name_dcs = MMI_PHB_UCS2;
		pMsgNewEntry->list.alpha_id.name_length = ((U8)pfnUnicodeStrlen((PS8)gpFDLBDLContext->FDLBDLName))*ENCODING_LENGTH;
		BigEndianToLittleEndian((PS8)pMsgNewEntry->list.alpha_id.name, (PS8)gpFDLBDLContext->FDLBDLName);
	}
	else {
		pMsgNewEntry->list.alpha_id.name_dcs = MMI_PHB_ASCII;
		pMsgNewEntry->list.alpha_id.name_length = (U8)pfnUnicodeStrlen((PS8)gpFDLBDLContext->FDLBDLName);
		UnicodeToAnsii((PS8)pMsgNewEntry->list.alpha_id.name, (PS8)gpFDLBDLContext->FDLBDLName);
		PhbAsciiToGSM7Bit(pMsgNewEntry->list.alpha_id.name);
	}
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_SET_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)pMsgNewEntry;
	Message.oslPeerBuffPtr= NULL;
	DeleteUptoScrID(SCR_ID_SECSET_FDN_LIST);
	DeleteScreenIfPresent(SCR_ID_SECSET_FDN_LIST);
	ExitSECSETFDNListReload();
	SetProtocolEventHandlerExt(SECSETSetFdlEntryRsp,PRT_PHB_SET_ENTRY_RSP,gSecSetupMenuSimID);
	OslMsgSendExtQueue(&Message);
}

/**************************************************************

	FUNCTION NAME		: SECSETSetFdlEntryRsp(void)
  	PURPOSE				: Rsp fn for getting all FDL entries.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETSetFdlEntryRsp(void * info)
{
	MSG_PHB_SET_ENRTY_RSP_STRUCT* localPtr;

	localPtr = (MSG_PHB_SET_ENRTY_RSP_STRUCT *)info;

	if (localPtr->result.flag == 0)  {
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
	}
	else if (localPtr->result.cause == MMI_SIM_EF_RECORD_FULL) {
		DisplayPopup((PU8)GetString(STR_NOT_SAVED_NUMBER_TOO_LONG), IMG_GLOBAL_ERROR, 1, ST_NOTIFYDURATION, ERROR_TONE);
	}
	else {
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
	}
	gpFDLBDLContext->FDNBDNEdit = FALSE;
	DeleteUptoScrID(SCR_ID_SECSET_FDN_LIST);
}

/**************************************************************

	FUNCTION NAME		: HighlightScrEditFDL(void)
  	PURPOSE				: Highlight Edit option
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void HighlightScrEditFDL()
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntrySECSETFDNDetails, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntrySECSETFDNDetails,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	pfnUnicodeStrcpy((S8*)gpFDLBDLContext->FDLBDLName,(S8*)gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedFDN].alpha_id.name);
	pfnUnicodeStrcpy((S8*)gpFDLBDLContext->FDLBDLNumber,(S8*)gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedFDN].tel.number);
	gpFDLBDLContext->FDNBDNEdit = TRUE ;

}


/**************************************************************

	FUNCTION NAME		: HighLightDeleteFDL(void)
  	PURPOSE				: Highlight Delete Option.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void HighLightDeleteFDL()
{
//micha0811
//	SetKeyHandler(EntrySECSETFDNDelConfirm, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntrySECSETFDNDelConfirm,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETFDNDelConfirm(void)
  	PURPOSE				: Entry fn for Confirmation of Delete.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETFDNDelConfirm()
{
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_DELETE_QUERY_MSG),IMG_GLOBAL_QUESTION, WARNING_TONE);

	g_SecuritySetupContext.VerifyPassSuccessCallBack = SECSETDelFdlReq;
	SetLeftSoftkeyFunction (SECSETGetPIN2StatusReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoToFDLList,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETFDNDelConfirm(void)
  	PURPOSE				: 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void ExitSECSETFDNDelConfirm()
{
//micha0531
	stopRequestedTone(WARNING_TONE);
}

/**************************************************************

	FUNCTION NAME		: SECSETDelFdlReq(void)
 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETDelFdlReq()
{
	MSG_PHB_DELETE_ENTRY_REQ_STRUCT *pMsgDelEntry = NULL ;
	MYQUEUE Message;
	module_type dest_mod_id = MOD_L4C + gSecSetupMenuSimID;

	EntrySECSETFDNBDNDeleting();
	pMsgDelEntry = (MSG_PHB_DELETE_ENTRY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_DELETE_ENTRY_REQ_STRUCT));
	pMsgDelEntry->no_data = 1;
	pMsgDelEntry->del_all = MMI_FALSE; //No 
	pMsgDelEntry->index = gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedFDN].index;

	pMsgDelEntry->type = MMI_PHB_FDN ; //PHB_PHONEBOOK
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_DEL_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)pMsgDelEntry;
	Message.oslPeerBuffPtr= NULL;

	DeleteUptoScrID(SCR_ID_SECSET_FDN_LIST);
	DeleteScreenIfPresent(SCR_ID_SECSET_FDN_LIST);
	ExitSECSETFDNListReload();	
	SetProtocolEventHandlerExt(SECSETDelFdlRsp,PRT_PHB_DEL_ENTRY_RSP,gSecSetupMenuSimID);
	
	OslMsgSendExtQueue(&Message);

}

/**************************************************************

	FUNCTION NAME		: SECSETDelFdlRsp(void)
  	PURPOSE				: Rsp handler for Delete req.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETDelFdlRsp(void *info )
{
	MSG_PHB_DELETE_ENTRY_RSP_STRUCT* localPtr;
	
	localPtr = (MSG_PHB_DELETE_ENTRY_RSP_STRUCT *)info;

	if (localPtr->result.flag == 0 /*OK*/) 
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
	}
	else 
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
	}
	DeleteUptoScrID(SCR_ID_SECSET_FDN_LIST);
}


/**************************************************************

	FUNCTION NAME		: FdlMakeCall(void)
  	PURPOSE				: make call from FDL
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void FdlMakeCall() 
{
	if(gpFDLBDLContext->HighlightedFDN != 255)
	{
		g_phb_cntx.dial_from_list  = MMI_PHB_FDN;
		MakeCall((PS8)gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedFDN].tel.number);
	}

}

/**************************************************************

	FUNCTION NAME		: GetActiveIndex(void)
  	PURPOSE				: 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void GetActiveIndex(S32 index)
{
	gpFDLBDLContext->HighlightedFDN = (U8) index ;
}

/**************************************************************

	FUNCTION NAME		: GoToFDLList(void)
  	PURPOSE				: Goback to FDl List Screen.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void GoToFDLList(void)
{
	GoBackToHistory(SCR_ID_SECSET_FDN_LIST);
}

/**************************************************************

	FUNCTION NAME		: GoToAddEditScreen(void)
  	PURPOSE				: Go back to Add screen or edit screen.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void GoToAddEditScreen(void)
{
	if(IsScreenPresent(SCR_ID_SECSET_FDN_DETAILS))
		GoBackToHistory(SCR_ID_SECSET_FDN_DETAILS);
	else
		GoBackToHistory(SCR_ID_SECSET_FDN_LIST);
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETFDNBDNSaving(void)
  	PURPOSE				: Show progress screen for Settings.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETFDNBDNSaving(void)
{
	EntryNewScreen(SCR_ID_SECSET_FDNBDN_SAVING, NULL, EntrySECSETFDNBDNSaving, NULL);
	
//micha1020
	ShowCategory66Screen(STR_SCR9002_SECURITY_CAPTION,IMG_SCR_PBOOK_CAPTION, 0, 0, 0, 0, (PU8)GetString(STR_GLOBAL_SAVING), IMG_GLOBAL_SAVE, NULL);
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETFDNBDNDeleting(void)
  	PURPOSE				: Show progress screen for Settings.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/

void EntrySECSETFDNBDNDeleting(void)
{
	EntryNewScreen(SCR_ID_SECSET_FDNBDN_DELETING, NULL, EntrySECSETFDNBDNDeleting, NULL);

	ShowCategory66Screen(STR_SCR9000_SETTINGS_CAPTION,IMG_SCR_PBOOK_CAPTION, 0, 0, 0, 0, (PU8)GetString(STR_PHB_DELETING), IMG_GLOBAL_DELETED, NULL);
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
}


/**************************************************************

	FUNCTION NAME		: EntryFdlList(void)

  	PURPOSE				: Check if phonebook is ready, if yes then load FDL List

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#include "satdefs.h"
extern BOOL is_phb_init_Finishi(void);
void EntryFdlList(void)
{
	if((gSimReady[gSecSetupMenuSimID] == FALSE) ||!is_phb_init_Finishi())
        {
        	DisplayPopup((PU8)GetString(SAT_WAIT_STRING_ID), IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
        	return;	
        }
	
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	
	//if(g_phb_cntx.phb_ready)
	{
		g_FixedInit[gSecSetupMenuSimID] = FALSE;
		gpFDLBDLContext->FDNBDNCount =0;
		SECSETGetFdlEntriesReq();
	}
	#if 0
	else
	{
		DisplayPopup((PU8)GetString(STR_FDL_NOT_READY) ,IMG_PROCESSING_PHONEBOOK, 1,ST_NOTIFYDURATION,0);
	}
	#endif
}

