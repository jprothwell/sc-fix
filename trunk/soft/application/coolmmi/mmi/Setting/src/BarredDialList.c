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
 *	BarredDialList.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Barred Dial List related functions
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

	FILENAME	: CallBarredDialList.c

  	PURPOSE		: Call Setting Application

 

	 

	DATE		: 

**************************************************************/
#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mainmenudef.h"
#include "protocolevents.h"
#include "wgui_categories.h"
#include "phonebookdef.h"
#include "settinggprots.h"
#include "barreddiallist.h"
#include "commonscreens.h"
#include "phonebookgprot.h"
#include "gsm7bitdefaultalphabet.h"
#include "settingdefs.h"
#include "settingsgexdcl.h"
#include "settingsgdcl.h"
#include "securitysetup.h"
//micha0901
#include "profilegprots.h"
#include "simdetectiondef.h"
#undef __NEWSIMULATOR

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
 
#include "wgui_categories_popup.h"
#include "wgui_categories_inputs.h"

extern U8 gSecSetupMenuSimID;
/**************************************************************

	FUNCTION NAME		: InitBarredDialList(void)
  	PURPOSE				: 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void InitBarredDialList(void)
{
	SetHiliteHandler(MENU8236_BDL_VIEW_OPTIONS_ADD,HighlightAddBDlist );
	SetHiliteHandler(MENU8236_BDL_VIEW_OPTIONS_EDIT,HighlightEditBDlist );
	SetHiliteHandler(MENU8236_BDL_VIEW_OPTIONS_DELETE,HighlightDeleteBDlist );
	gpFDLBDLContext->FDNBDNCount = 0;
	gpFDLBDLContext->FDNBDNEdit = FALSE;
	gpFDLBDLContext->HighlightedBDN = 255;
}

/**************************************************************

	FUNCTION NAME		: SECSETGetBdlEntriesReq(void)
 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETGetBdlEntriesReq()
{
	MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *pMsgIndexReq = NULL ;
	MYQUEUE	Message;
	module_type dest_mod_id = MOD_L4C + gSecSetupMenuSimID;

	pMsgIndexReq = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT));
	pMsgIndexReq->index=gpFDLBDLContext->FDNBDNCount + 1;
	pMsgIndexReq->record_index=0xFFFF;
	pMsgIndexReq->storage=MMI_SIM ;
	pMsgIndexReq->type=MMI_PHB_BDN ;
	pMsgIndexReq->no_data=MAX_ENTRIES_IN_LIST;

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;

	Message.oslMsgId = PRT_PHB_GET_ENTRY_BY_INDEX_REQ;
	Message.oslDataPtr = (oslParaType *)pMsgIndexReq;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandlerExt(SECSETGetBdlEntriesRsp, PRT_PHB_GET_ENTRY_BY_INDEX_RSP,gSecSetupMenuSimID);

	OslMsgSendExtQueue(&Message);

}

/**************************************************************

	FUNCTION NAME		: SECSETGetBdlEntriesRsp(void)
  	PURPOSE				: Rsp handler of req to load BDL list.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETGetBdlEntriesRsp(void * info)
{

	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U8 nSimID;
	
	U8 i = 0,j=0;
//micha0317
	U8 TempBuff[(MAX_PB_NUMBER_LENGTH+2)*ENCODING_LENGTH];
	U8 *tmpConvertBuf = NULL;
	U16 convertOutLen = 0;

	nSimID = MTPNP_AD_get_protocol_event_src_mode();

	SetProtocolEventHandlerExt(NULL, PRT_PHB_GET_ENTRY_BY_INDEX_RSP,nSimID);
	
	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *)info;

	if (localPtr->result.flag == 0 /*OK*/) 
	{
		i =  gpFDLBDLContext->FDNBDNCount;
		gpFDLBDLContext->FDNBDNCount +=  localPtr->no_list;//Add New entries count to BDL list count

		if (gpFDLBDLContext->FDNBDNCount > MAX_SIM_ENTRIES)
			gpFDLBDLContext->FDNBDNCount = MAX_SIM_ENTRIES;
		for (; i < gpFDLBDLContext->FDNBDNCount ; i++, j++) 
		{
			gpFDLBDLContext->FDLBDLEntries[i].storage = localPtr->list[j].storage;
			gpFDLBDLContext->FDLBDLEntries[i].index = localPtr->list[j].index;
			gpFDLBDLContext->FDLBDLEntries[i].record_index = localPtr->list[j].record_index;
			memset(gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name, 0, MAX_NAME_LEN_FSB_LIST*ENCODING_LENGTH);
			memset(gpFDLBDLContext->FDLBDLEntries[i].tel.number, 0, MAX_NUMBER_LEN_FSB_LIST*ENCODING_LENGTH);
			gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_length = localPtr->list[j].alpha_id.name_length;
			gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_dcs = localPtr->list[j].alpha_id.name_dcs;
			tmpConvertBuf = CovertStringForPlatform(localPtr->list[j].alpha_id.name, gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_length,gpFDLBDLContext->FDLBDLEntries[i].alpha_id.name_dcs,&convertOutLen);
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
				gpFDLBDLContext->FDLBDLEntries[i].tel.number[0] = '+';
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
			SECSETGetBdlEntriesReq();
		}
		else {
			ViewBDList();
		}
	}
	else if (localPtr->result.flag == 1 /*Failure*/ && localPtr->result.cause == 5) 
	{
//		gFDLBDLContext.FDNBDNCount =0;
		ViewBDList();
	}
	else 
	{
			DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
			DeleteUptoScrID(SCR_ID_SECSET_BAR_DIAL);
	}
}

/**************************************************************

	FUNCTION NAME		: ViewBDList(void)
  	PURPOSE				: go to view BDL list screen.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void ViewBDList()
{
	gpFDLBDLContext->HighlightedBDN =0;
	EntrySECSETBDNList();
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETBDNList(void)
  	PURPOSE				: Entry fn to view BDL list
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETBDNList()
{

	U8 *pstbdlist[MAX_SIM_ENTRIES],*bdlHint[MAX_SIM_ENTRIES];
	U16 index;
	U8* guiBuffer;
	U16 bdlimagelist[MAX_SIM_ENTRIES];

	EntryNewScreen(SCR_ID_SECSET_BDN_LIST, NULL, EntrySECSETBDNList, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_BDN_LIST);		/* Getting the Index from history */
	
	for(index = 0; index < gpFDLBDLContext->FDNBDNCount ; index++) 
	{

		if(pfnUnicodeStrlen((PS8)(gpFDLBDLContext->FDLBDLEntries[index].alpha_id.name)) > 0)
		{
			pstbdlist[index] = (U8*)gpFDLBDLContext->FDLBDLEntries[index].alpha_id.name;
			bdlHint[index] = (U8*)gpFDLBDLContext->FDLBDLEntries[index].tel.number;
		}
		else
		{
			pstbdlist[index] = (U8*)gpFDLBDLContext->FDLBDLEntries[index].tel.number;
			bdlHint[index] = NULL;
		}
		bdlimagelist[index]= IMG_STORAGE_SIM;
	}


	RegisterHighlightHandler(GetActiveBDLIndex);

	if( guiBuffer != NULL)
	{
		memset( (void *) guiBuffer, 0, 2);
	}
	if (( gpFDLBDLContext->HighlightedBDN >= gpFDLBDLContext->FDNBDNCount ) && (gpFDLBDLContext->FDNBDNCount > 0)) 
		gpFDLBDLContext->HighlightedBDN = gpFDLBDLContext->FDNBDNCount -1;

	if(gpFDLBDLContext->FDNBDNCount == 0)//If list is empty
	{
		pstbdlist[0] = (U8*)GetString(STR_GLOBAL_EMPTY_LIST);
		bdlimagelist[0]= 0;
		bdlHint[0] = NULL;

		ShowCategory6Screen(STR_SCR8079_BDL_VIEW_LIST_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON, 
							STR_GLOBAL_ADD, IMG_GLOBAL_OPTIONS, 
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 1, 
							pstbdlist,NULL, 0, guiBuffer);
		gpFDLBDLContext->FDNBDNEdit = FALSE ;

		memset(gpFDLBDLContext->FDLBDLName,0,(MAX_NAME_LEN_FSB_LIST+1)*ENCODING_LENGTH);
		memset(gpFDLBDLContext->FDLBDLNumber,0,(MAX_NUMBER_LEN_FSB_LIST+1+1)*ENCODING_LENGTH);

		SetLeftSoftkeyFunction(EntrySECSETBDNDetails,KEY_EVENT_UP);
//micha0529
		SetKeyHandler(EntrySECSETBDNDetails, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(0,KEY_SEND,KEY_EVENT_UP);
	}
	else
	{
		ShowCategory53Screen(STR_SCR8079_BDL_VIEW_LIST_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
//micha0610
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS	,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK	,
						  gpFDLBDLContext->FDNBDNCount , pstbdlist,bdlimagelist,
						  bdlHint,0,gpFDLBDLContext->HighlightedBDN , guiBuffer);

		SetLeftSoftkeyFunction(EntrySECSETBDNOption,KEY_EVENT_UP);
//micha0529
		SetKeyHandler(EntrySECSETBDNOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(0,KEY_SEND,KEY_EVENT_UP);
	}
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETBDNListReload(void)
  	PURPOSE				: 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void ExitSECSETBDNListReload(void)
{
	history_t   Scr;
	S16 nHistory = 0;	
	DeleteScreenIfPresent(SCR_ID_SECSET_BDN_LIST);
	Scr.scrnID = SCR_ID_SECSET_BDN_LIST;
	Scr.entryFuncPtr = EntryBdlList;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}


/**************************************************************

	FUNCTION NAME		: EntrySECSETBDNOption(void)
  	PURPOSE				: Entry fn to view Options of BDL list screen
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETBDNOption()
{

	U16 nStrItemList[MAX_SUB_MENUS];/* Stores the strings id of submenus returned */
	U16 nNumofItem;		 /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */

	EntryNewScreen(SCR_ID_SECSET_BDN_OPTION, NULL, EntrySECSETBDNOption, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_BDN_OPTION);	

	nNumofItem = GetNumOfChild(MENU8236_BDL_VIEW_OPTIONS_MAIN);

//	nDispAttribute = GetDispAttributeOfItem(MENU8236_BDL_VIEW_OPTIONS_MAIN	);
	
	GetSequenceStringIds(MENU8236_BDL_VIEW_OPTIONS_MAIN	, nStrItemList);
	SetParentHandler(MENU8236_BDL_VIEW_OPTIONS_MAIN	);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
//micha0610
	ShowCategory15Screen(STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  LIST_MENU, 0,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: GetActiveBDLIndex(void)
  	PURPOSE				: Get index of currently highlighted BDL entry.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void GetActiveBDLIndex(S32 index)
{
	gpFDLBDLContext->HighlightedBDN = (U8) index ;
}

/**************************************************************

	FUNCTION NAME		: HighlightAddBDlist(void)
  	PURPOSE				: Highlight handler of Add option
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void HighlightAddBDlist()
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntrySECSETBDNDetails, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntrySECSETBDNDetails,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	gpFDLBDLContext->FDNBDNEdit = FALSE ;

	memset(gpFDLBDLContext->FDLBDLName,0,(MAX_NAME_LEN_FSB_LIST+1)*ENCODING_LENGTH);
	memset(gpFDLBDLContext->FDLBDLNumber,0,(MAX_NUMBER_LEN_FSB_LIST+1+1)*ENCODING_LENGTH);
}
/**************************************************************

	FUNCTION NAME		: EntrySECSETBDNDetails(void)
  	PURPOSE				: Main Inline edit entry sceen fn to add and edit a BDL entry.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETBDNDetails(void)
{
	U16 nNumofItem;      /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U16		ItemIcons[2];
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
	
	if((gpFDLBDLContext->FDNBDNCount == (U16)gpFDLBDLContext->MaxBDL ||gpFDLBDLContext->FDNBDNCount == MAX_SIM_ENTRIES)  &&( gpFDLBDLContext->FDNBDNEdit == FALSE))
	{
		DisplayPopup((U8*)GetString(STR_SETTING_STORAGE_FULL),IMG_GLOBAL_WARNING,0,ST_NOTIFYDURATION,(U8)WARNING_TONE);
	}
	else
	{
		EntryNewScreen(SCR_ID_SECSET_BDN_DETAILS, ExitSECSETBDNDetails, NULL, NULL);		


		guiBuffer = GetCurrGuiBuffer(SCR_ID_SECSET_BDN_DETAILS);
		inputBuffer = GetCurrNInputBuffer ( SCR_ID_SECSET_BDN_DETAILS, &inputBufferSize );    //added for inline edit history

		nNumofItem = GetNumOfChild(MENU8236_BDL_VIEW_OPTIONS_ADD);

//		nDispAttribute = GetDispAttributeOfItem(MENU8236_BDL_VIEW_OPTIONS_ADD);

		GetSequenceImageIds(MENU8236_BDL_VIEW_OPTIONS_ADD,ItemIcons);

		InitializeCategory57Screen();

		FillBdlInlineEditStruct();

		if(inputBuffer!=NULL)                                            //added for inline edit history
			SetCategory57Data(gpFDLBDLContext->FDLBDLInlineDisplay, nNumofItem, inputBuffer);//sets the data 

		ShowCategory57Screen(STR_SETTING_FDL_EDIT_ENTRY,MAIN_MENU_TITLE_SETTINGS_ICON,
							STR_EDIT,IMG_GLOBAL_OK,
							STR_DONE, IMG_GLOBAL_BACK,
							nNumofItem,ItemIcons,gpFDLBDLContext->FDLBDLInlineDisplay,
							0,guiBuffer);

		SetCategory57RightSoftkeyFunctions( PreNewBdlEntryReq, GoBackHistory);

	}
}

/**************************************************************

	FUNCTION NAME		: ExitSECSETBDNDetails(void)
  	PURPOSE				: 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void ExitSECSETBDNDetails(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	S16 nHistory = 0;
	U16 size;
	CloseCategory57Screen();   
	Scr.scrnID = SCR_ID_SECSET_BDN_DETAILS;
	Scr.entryFuncPtr = EntrySECSETBDNDetails;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	size = (U16)GetCategory57DataSize( );     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr,size);
}

/**************************************************************

	FUNCTION NAME		: FillBdlInlineEditStruct(void)
  	PURPOSE				: Fill Inline Edit structs.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void FillBdlInlineEditStruct(void)
{
	SetInlineItemActivation(&gpFDLBDLContext->FDLBDLInlineDisplay[0],KEY_LSK,KEY_EVENT_UP);
	SetInlineItemFullScreenEdit(&gpFDLBDLContext->FDLBDLInlineDisplay[0],STR_SETTING_FDL_NAME,MAIN_MENU_TITLE_SETTINGS_ICON,(PU8)gpFDLBDLContext->FDLBDLName,
        (g_phb_cntx.bdn_name_len + 1) ,INPUT_TYPE_ALPHANUMERIC_UPPERCASE | INPUT_TYPE_USE_ENCODING_BASED_LENGTH|INPUT_TYPE_ONE_LESS_CHARACTER);

	SetInlineFullScreenEditCustomFunction(&gpFDLBDLContext->FDLBDLInlineDisplay[0], SettingInlineCustomFunction);

	SetInlineItemActivation(&gpFDLBDLContext->FDLBDLInlineDisplay[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&gpFDLBDLContext->FDLBDLInlineDisplay[1], (PU8)gpFDLBDLContext->FDLBDLNumber, MAX_PB_NUMBER_LENGTH+1, INPUT_TYPE_PHONE_NUMBER| INPUT_TYPE_PLUS_CHARACTER_HANDLING);
	DisableInlineItemHighlight(&gpFDLBDLContext->FDLBDLInlineDisplay[1]);
}

/**************************************************************

	FUNCTION NAME		: PreNewBdlEntryReq(void)
  	PURPOSE				: Funtion to be called after press of button save.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void PreNewBdlEntryReq()
{
	if(!pfnUnicodeStrlen((PS8)gpFDLBDLContext->FDLBDLNumber) && !pfnUnicodeStrlen((PS8)gpFDLBDLContext->FDLBDLName))
	{
		GoBackHistory();
	}
	else
	{
		EntrySECSETBDNSaveConfirm();
	}
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETBDNSaveConfirm(void)
  	PURPOSE				: Entry fn to confirm Entry save.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETBDNSaveConfirm(void)
{
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_GLOBAL_SAVE),IMG_GLOBAL_QUESTION, WARNING_TONE);


	g_SecuritySetupContext.VerifyPassSuccessCallBack = SECSETSetBdlEntryReq;
	SetRightSoftkeyFunction(GoToBDLList,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(SECSETGetPIN2StatusReq,KEY_EVENT_UP);

}

/**************************************************************

	FUNCTION NAME		: ExitSECSETBDNSaveConfirm(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSECSETBDNSaveConfirm(void)
{
//micha0531
	stopRequestedTone(WARNING_TONE);
}

/**************************************************************

	FUNCTION NAME		: SECSETSetBdlEntryReq(void)
 
						  in Bdl list after confirmation of PIN2.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETSetBdlEntryReq()
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *pMsgNewEntry = NULL ;
	MYQUEUE Message;
	U8 pbNumber_ASCII[MAX_PB_NUMBER_LENGTH+1];		
	module_type dest_mod_id = MOD_L4C + gSecSetupMenuSimID;

	
	EntrySECSETFDNBDNSaving();
	
	pMsgNewEntry  = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));

	pMsgNewEntry->type = MMI_PHB_BDN;
	pMsgNewEntry->no_data = 1;
	pMsgNewEntry->list.storage = MMI_SIM + gSecSetupMenuSimID;
	pMsgNewEntry->list.index = 0xFFFF; // For New Entry It should Always be 0xFFFF;
	pMsgNewEntry->list.record_index = 0xFFFF; // For New Entry It should be alaways 0xFFFF

	if(gpFDLBDLContext->FDNBDNEdit )//If entry is edited
	{
		pMsgNewEntry->list.index = gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedBDN ].index;
		pMsgNewEntry->list.record_index = gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedBDN ].record_index;
	}

	if (gpFDLBDLContext->FDLBDLNumber[0] == '+')//if number is international
	{
		pMsgNewEntry->list.tel.type = MMI_CSMCC_INTERNATIONAL_ADDR; //INTERNATIONAL
		pMsgNewEntry->list.tel.length = (U8)pfnUnicodeStrlen((const S8 *)(gpFDLBDLContext->FDLBDLNumber + ENCODING_LENGTH));
		UnicodeToAnsii((PS8)pbNumber_ASCII,(PS8)( gpFDLBDLContext->FDLBDLNumber + ENCODING_LENGTH));
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
	DeleteUptoScrID(SCR_ID_SECSET_BDN_LIST);	
	DeleteScreenIfPresent(SCR_ID_SECSET_BDN_LIST);
	ExitSECSETBDNListReload();
	SetProtocolEventHandlerExt(SECSETSetBdlEntryRsp,PRT_PHB_SET_ENTRY_RSP,gSecSetupMenuSimID);
	OslMsgSendExtQueue(&Message);
}

/**************************************************************

	FUNCTION NAME		: SECSETSetBdlEntryRsp(void)
  	PURPOSE				: Rsp handler to add new entry in BDL.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETSetBdlEntryRsp(void * info)
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
	DeleteUptoScrID(SCR_ID_SECSET_BDN_LIST);
}

/**************************************************************

	FUNCTION NAME		: HighlightEditBDlist(void)
  	PURPOSE				: Highlight handler of Edit option.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void HighlightEditBDlist()
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntrySECSETBDNDetails, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntrySECSETBDNDetails,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	pfnUnicodeStrcpy((S8*)gpFDLBDLContext->FDLBDLName,(S8*)gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedBDN ].alpha_id.name);
	pfnUnicodeStrcpy((S8*)gpFDLBDLContext->FDLBDLNumber ,(S8*)gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedBDN ].tel.number);
	
	gpFDLBDLContext->FDNBDNEdit = TRUE ;
}



/**************************************************************

	FUNCTION NAME		: HighlightDeleteBDlist(void)
  	PURPOSE				: Highlight handler Of dlete option
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void HighlightDeleteBDlist()
{
//micha0811
//	SetKeyHandler(EntrySECSETBDNDelConfirm, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntrySECSETBDNDelConfirm,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: EntrySECSETBDNDelConfirm(void)
  	PURPOSE				: Entry screen to confirm delete operation from user.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void EntrySECSETBDNDelConfirm()
{
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_DELETE_QUERY_MSG),IMG_GLOBAL_QUESTION, WARNING_TONE);
	
	g_SecuritySetupContext.VerifyPassSuccessCallBack = SECSETDelBdlReq;
	SetLeftSoftkeyFunction (SECSETGetPIN2StatusReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoToBDLList,KEY_EVENT_UP);

}

/**************************************************************

	FUNCTION NAME		: ExitSECSETBDNDelConfirm(void)
  	PURPOSE				: 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void ExitSECSETBDNDelConfirm()
{
//micha0531
	stopRequestedTone(WARNING_TONE);
}

/**************************************************************

	FUNCTION NAME		: SECSETDelBdlReq(void)
 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETDelBdlReq()
{
	MSG_PHB_DELETE_ENTRY_REQ_STRUCT *pMsgDelEntry = NULL ;
	MYQUEUE Message;
	module_type dest_mod_id = MOD_L4C + gSecSetupMenuSimID;

	EntrySECSETFDNBDNDeleting();
	pMsgDelEntry = (MSG_PHB_DELETE_ENTRY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_DELETE_ENTRY_REQ_STRUCT));

	pMsgDelEntry->no_data = 1;
	pMsgDelEntry->del_all = MMI_FALSE; //No 
	pMsgDelEntry->index = gpFDLBDLContext->FDLBDLEntries[gpFDLBDLContext->HighlightedBDN ].index;
	
	pMsgDelEntry->type = MMI_PHB_BDN ; //PHB_PHONEBOOK
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_DEL_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)pMsgDelEntry;
	Message.oslPeerBuffPtr= NULL;

	DeleteUptoScrID(SCR_ID_SECSET_BDN_LIST);	
	DeleteScreenIfPresent(SCR_ID_SECSET_BDN_LIST);
	ExitSECSETBDNListReload();	

	SetProtocolEventHandlerExt(SECSETDelBdlRsp,PRT_PHB_DEL_ENTRY_RSP,gSecSetupMenuSimID);
	OslMsgSendExtQueue(&Message);
}


/**************************************************************

	FUNCTION NAME		: SECSETDelBdlRsp(void)
 
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void SECSETDelBdlRsp(void *info)
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
	DeleteUptoScrID(SCR_ID_SECSET_BDN_LIST);
}



/**************************************************************

	FUNCTION NAME		: GoToBDLList(void)
  	PURPOSE				: Go to view BDL list.
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
  	Author				: Vanita Jain
 

**************************************************************/
void GoToBDLList(void)
{
	GoBackToHistory(SCR_ID_SECSET_BDN_LIST);
}




/**************************************************************

	FUNCTION NAME		: EntryBdlList(void)

  	PURPOSE				: Check if Phone Book is ready, if yes then Enter in BDL List screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryBdlList(void)
{
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);

	if(g_phb_cntx.phb_ready)
	{
		gpFDLBDLContext->FDNBDNCount = 0;
		SECSETGetBdlEntriesReq();
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_BDL_NOT_READY) ,IMG_PROCESSING_PHONEBOOK, 1,ST_NOTIFYDURATION,0);
	}
}


