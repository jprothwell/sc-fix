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
 * PhoneBookOperateMass.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for phonebook mass operation.
 *   Such as Copy All and Delete All.
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
/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-06-15, Li Tao      fix bug P78000000056: 
  		The maximal length of the entry to the phone can't be moved and copied to the SIM
  02.01.002  2007-06-21, Li Tao      fix bug P78000000056: add handle for chinese font
 ************************************************************************/
 
#ifndef _PHONEBOOKOPERATEMASS_C
#define _PHONEBOOKOPERATEMASS_C

#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalmenuitems.h"
#include "debuginitdef.h"
#include "custdatares.h"
#include "mmi_data_types.h"
#include "protocolevents.h"
#include "phonebookgprot.h"
#include "globaldefs.h"
#include "eventsgprot.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "gsm7bitdefaultalphabet.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "historygprot.h"
#include "unicodexdcl.h"
#include "timerevents.h"
#include "custmenures.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "nvramenum.h"
#ifdef MMI_ON_WIN32
#include "nvram_data_items.h"
#endif
#include "settingdefs.h"
#include "settinggprots.h"
#include "settingsgdcl.h"
#include "callsetupenum.h"
#include "gpioinc.h"
#include "commonscreens.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "mtpnp_ad_resdef.h"
#endif
#if defined(__MMI_VRSD_DIAL__)
#include "vrsddial.h"
#endif

#if defined(__MMI_VRSI__)
#include "mdi_datatype.h"
#include "vrsienum.h"
#include "vrsitype.h"
#include "vrsiprot.h"
#endif

#include "mmi_trace.h"

#undef __NEWSIMULATOR 

/* 
** Local Variable
*/
static U16 g_phb_mass_op_index;
static U16 g_phb_mass_op_count;
static U16 g_phb_mass_op_lookup_count;
static BOOL g_phb_mass_op_abort;
static BOOL g_phb_mass_op_success = TRUE;

// add by chenqiang 20070808 for bug 5889 5800 b
BOOL g_phb_copyordelete_error_flag = FALSE;
// add by chenqiang 20070808 for bug 5889 5800 e
BOOL g_phb_add_nvram_full_error_flag = FALSE;

/* 
** Global Variable
*/
extern U16	 PhoneBookEntryCount;

extern MMI_PHB_ENTRY_BCD_STRUCT			PhoneBook[MAX_PB_ENTRIES];

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
extern PHB_OPTIONAL_FIELDS_STRUCT	PhoneBookOptionalFields;
#endif 

extern U8 gPhonePassword[];
extern U16 g_phb_name_index[];

extern MMI_PHB_LOOKUP_NODE_STRUCT LookUpTable[];

extern S8 pbName[];
extern S8 pbNumber[];
extern S8 pbHomeNumber[];
extern S8 pbCompanyName[];
extern S8 pbEmailAddress[];
extern S8 pbOfficeNumber[];
extern S8 pbFaxNumber[];
#ifdef __MMI_MULTI_SIM__
extern MMI_PHB_STORAGE_LOCATION phbcopymovedes;
extern MMI_PHB_STORAGE_LOCATION phbcopymovesrc;
#endif
#if defined(__MMI_PHB_EMAIL_SORT__)
extern U8 g_phb_email_sorted;
#endif
/* 
** Global Function
*/
extern void SECSETVerifyPINReq(U8 pin_type, U8* ucs2_pin, U8* ucs2_new_pin, FuncPtr success_callback);

#define MMI_PHB_MASS_INIT
/*****************************************************************************
* FUNCTION
*  mmi_phb_operate_mass_init()
* DESCRIPTION
*   Initialization of "Copy All, Delete All" feature
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void MTPNP_PFAL_Slave_PHB_Init(void);
void mmi_phb_operate_mass_init(void) 
{
	U8 i;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_operate_mass_init.>\n",__FILE__,__LINE__));

	/*Copy All*/
	SetHiliteHandler(MITEM104_PBOOK_COPY_ALL			,mmi_phb_highlight_copy_all);
	SetHiliteHandler(MITEM_PBOOK_COPY_ALL_SIM			,mmi_phb_highlight_copy_all_from_sim);
	SetHiliteHandler(MITEM_PBOOK_COPY_ALL_PHONE		,mmi_phb_highlight_copy_all_from_phone);
	SetHintHandler(MITEM_PBOOK_COPY_ALL_SIM			,mmi_phb_hint_copy_all_from_sim);
	SetHintHandler(MITEM_PBOOK_COPY_ALL_PHONE		,mmi_phb_hint_copy_all_from_phone);

	/*Delete All*/
	SetHiliteHandler(MITEM103_PBOOK_DELETE_ALL		,mmi_phb_highlight_delete_all);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MITEM_PBOOK_DELETE_ALL_SIM + i, mmi_phb_highlight_delete_all_from_sim);
		SetHintHandler(MITEM_PBOOK_DELETE_ALL_SIM + i, mmi_phb_hint_delete_all_from_sim);
	}
	SetHiliteHandler(MITEM_PBOOK_DELETE_ALL_PHONE	,mmi_phb_highlight_delete_all_from_phone);
	SetHintHandler(MITEM_PBOOK_DELETE_ALL_PHONE		,mmi_phb_hint_delete_all_from_phone);
	SetHiliteHandler(MENU_ID_PHB_DELETE_ALL_ONE_BY_ONE	,mmi_phb_highlight_delete_all_one_by_one);
#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_Slave_PHB_Init();
#endif
	/*AT Command Update*/
	SetProtocolEventHandler(mmi_phb_ind_update	,PRT_PHB_UPDATE_IND);		
}
#ifdef __MMI_MULTI_SIM__
void DstSelectHighLightHandler(S32 hiliteid)
{
	U8 i;
	S32 tempIndex =0;
	BOOL bFound = MMI_FALSE;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!MTPNP_PFAL_Is_CardAbsent(i) && (g_phb_cntx.sim_used[i] < g_phb_cntx.sim_total[i]) && (phbcopymovesrc != MMI_SIM + i))
		{
			if(tempIndex == hiliteid)
			{
				phbcopymovedes= MMI_SIM + i;
				bFound = MMI_TRUE;
				break;
			}
			tempIndex++;
		}
	}
	if(!bFound)
	{
		phbcopymovedes = MMI_NVRAM;
	}
	mmi_trace(g_sw_PHB, "DstSelectHighLightHandler,phbcopymovedes is %d", phbcopymovedes);
}
void mmi_phb_copy_all_select_dst(void)
{
	U8* guiBuffer;
	U8* nStrList[MMI_SIM_NUMBER + 1];
	//U8*	nHintList[MMI_SIM_NUMBER + 1];
	U16 nNumofItem = 0;
	U8 hiliteStorage = 0;
	U8 i;
	mmi_trace(g_sw_PHB, "mmi_phb_copy_all_select_dst, phbcopymovesrc is %d",phbcopymovesrc);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!MTPNP_PFAL_Is_CardAbsent(i) && (g_phb_cntx.sim_used[i] < g_phb_cntx.sim_total[i]) && (phbcopymovesrc != MMI_SIM + i))
			nStrList[nNumofItem++] = (PU8)GetString(STRING_MTPNP_SIM1 + i);
	}

	if((g_phb_cntx.phone_used < g_phb_cntx.phone_total) && (phbcopymovesrc != MMI_NVRAM))
	{
		nStrList[nNumofItem++] = (PU8)GetString(STR_STORAGE_PHONE);
	}
	if(nNumofItem)
	{
		EntryNewScreen(SCR_COPY_ALL_SELECT_DST, NULL, mmi_phb_copy_all_select_dst, NULL);
		guiBuffer = GetCurrGuiBuffer(SCR_COPY_ALL_SELECT_DST);

		RegisterHighlightHandler(DstSelectHighLightHandler);

		// TODO: prepare hint list , titile

		ShowCategory109Screen(STRING_MTPNP_DST_SELECT, IMG_SCR_PBOOK_CAPTION,
					STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					nNumofItem, nStrList, NULL,
					hiliteStorage, guiBuffer);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_all_confirm, KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_PBOOK_FULL_MSG), IMG_GLOBAL_EMPTY,  TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
	}
}
void SrcSelectHighLightHandler(S32 hiliteid)
{
	U8 i;
	BOOL bFound = MMI_FALSE;
	S32 tempIndex =0;
    
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!MTPNP_PFAL_Is_CardAbsent(i) && g_phb_cntx.sim_used[i])
		{
			if(tempIndex == hiliteid)
			{      
				g_phb_cntx.selected_storage = MMI_SIM + i;
				phbcopymovesrc = MMI_SIM + i;
				bFound = MMI_TRUE;
				break;
			}
			tempIndex++;
		}
	}
    
	if(!bFound)
	{
		g_phb_cntx.selected_storage = MMI_NVRAM;
		phbcopymovesrc = MMI_NVRAM;
	}
	mmi_trace(g_sw_PHB, "SrcSelectHighLightHandler,phbcopymovesrc is %d", phbcopymovesrc);
}
void mmi_phb_copy_all_select_src(void)
{
	U8* guiBuffer;
	U8* nStrList[MMI_SIM_NUMBER + 1];
	//U8*	nHintList[MMI_SIM_NUMBER + 1];
	U16 nNumofItem = 0;
	U8 hiliteStorage = 0;
	U8 i;

	g_phb_mass_op_abort = FALSE;

	if (g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		if(PhoneBookEntryCount)
		{
			EntryNewScreen(SCR_COPY_ALL_SELECT_SRC, NULL, mmi_phb_copy_all_select_src, NULL);
			guiBuffer = GetCurrGuiBuffer(SCR_COPY_ALL_SELECT_SRC);

			RegisterHighlightHandler(SrcSelectHighLightHandler);

			// TODO: prepare hint list , title
		
			for(i = 0; i < MMI_SIM_NUMBER; i++)
			{
				if(!MTPNP_PFAL_Is_CardAbsent(i) && g_phb_cntx.sim_used[i])
					nStrList[nNumofItem++] = (PU8)GetString(STRING_MTPNP_SIM1 + i);
			}

			if(g_phb_cntx.phone_used)
			{
				nStrList[nNumofItem++] = (PU8)GetString(STR_STORAGE_PHONE);
			}
			
			ShowCategory109Screen(STRING_MTPNP_SRC_SELECT, IMG_SCR_PBOOK_CAPTION,
						STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						nNumofItem, nStrList, NULL,
						hiliteStorage, guiBuffer);

			SetLeftSoftkeyFunction(mmi_phb_copy_all_select_dst, KEY_EVENT_UP);
			SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY,  TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
		}
	}
	else
	{
		mmi_phb_entry_not_ready();
	}
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_copy_all()
* DESCRIPTION
*   Highlight handler for "Copy All" in PHB main menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_copy_all(void) 
{

	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
#ifdef __MMI_MULTI_SIM__
	SetLeftSoftkeyFunction(mmi_phb_copy_all_select_src,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_copy_all_select_src, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(mmi_phb_entry_copy_all,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_copy_all, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_copy_all_from_sim()
* DESCRIPTION
*   Highlight handler for "From SIM" in Copy All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/

void mmi_phb_highlight_copy_all_from_sim(void) 
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_phb_cntx.selected_storage = MMI_SIM;
#ifdef __MMI_MULTI_SIM__
	phbcopymovesrc = MMI_SIM;	
	phbcopymovedes = MMI_NVRAM;	
#endif

	if(g_phb_cntx.sim_used) 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_all_confirm, KEY_EVENT_UP);
	}
	else
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_copy_all_from_phone()
* DESCRIPTION
*   Highlight handler for "From Phone" in Copy All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_copy_all_from_phone(void) 
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_phb_cntx.selected_storage = MMI_NVRAM;
#ifdef __MMI_MULTI_SIM__
	phbcopymovesrc = MMI_NVRAM;	
	phbcopymovedes = MMI_SIM;	
#endif

	if(g_phb_cntx.phone_used) 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_all_confirm, KEY_EVENT_UP);
	}
	else
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_copy_all_from_sim()
* DESCRIPTION
*   Hint handler for "From SIM" in Copy All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_hint_copy_all_from_sim(U16 index) 
{
	S8 simStatus[40];

	memset((S8*)hintData[index], 0, sizeof(hintData[index]));
	mmi_phb_memory_status_get_sim(simStatus, 0);
	pfnUnicodeStrcpy((S8*) hintData[index], simStatus);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_copy_all_from_phone()
* DESCRIPTION
*   Hint handler for "From Phone" in Copy All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_hint_copy_all_from_phone(U16 index) 
{
	S8 phoneStatus[40];

	memset((S8* )hintData[index], 0, sizeof(hintData[index]));
	mmi_phb_memory_status_get_phone(phoneStatus);
	pfnUnicodeStrcpy((S8*) hintData[index], phoneStatus);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_copy_all()
* DESCRIPTION
*   Draws the Copy All menu.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_copy_all(void) 
{
	U8* guiBuffer;
	U16 nStrItemList[6]; 
	U16 nImgIltemList[6];
	U8*	nHintList[6];
	U16 nNumofItem;
	U8 hilitedStorage;
	U8 hilight_sim, highlight_phone;

	hilight_sim = GetIndexOfStringId(MITEM104_PBOOK_COPY_ALL, MITEM_PBOOK_COPY_ALL_SIM);
	highlight_phone = GetIndexOfStringId(MITEM104_PBOOK_COPY_ALL, MITEM_PBOOK_COPY_ALL_PHONE);
	
#ifdef __MMI_MULTI_SIM__
	hilitedStorage = 0;
#endif
	
	if(g_phb_cntx.prefer_storage == MMI_SIM)
	{
		if((g_phb_cntx.sim_used > 0) || (g_phb_cntx.phone_used == 0))
			hilitedStorage = hilight_sim;
		else
			hilitedStorage = highlight_phone;
	}
	else
	{
		if((g_phb_cntx.phone_used >0) || (g_phb_cntx.sim_used == 0))
			hilitedStorage = highlight_phone;
		else
			hilitedStorage = hilight_sim;
	}
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_copy_all.>\n",__FILE__,__LINE__));
	g_phb_mass_op_abort = FALSE;
	if (g_phb_cntx.phb_ready && !g_phb_cntx.processing) 
	{
		if(PhoneBookEntryCount) 
		{
			EntryNewScreen(SCR_COPY_ALL_MENU, mmi_phb_exit_copy_all, mmi_phb_entry_copy_all, NULL);
			guiBuffer = GetCurrGuiBuffer(SCR_COPY_ALL_MENU);
#ifdef __MMI_MULTI_SIM__			
			if(MTPNP_PFAL_Is_CardAbsent(0) == TRUE && MTPNP_PFAL_Is_CardAbsent(1) == FALSE)
			{			
				nNumofItem = GetNumOfChild(MITEM104_PBOOK_COPY_ALL_NO_SIM1);
				GetSequenceStringIds(MITEM104_PBOOK_COPY_ALL_NO_SIM1, nStrItemList);
				GetSequenceImageIds(MITEM104_PBOOK_COPY_ALL_NO_SIM1, nImgIltemList);
				SetParentHandler(MITEM104_PBOOK_COPY_ALL_NO_SIM1);
				RegisterHighlightHandler(ExecuteCurrHiliteHandler);
				ConstructHintsList(MITEM104_PBOOK_COPY_ALL_NO_SIM1, nHintList);		
			}
			else if(MTPNP_PFAL_Is_CardAbsent(1) == TRUE && MTPNP_PFAL_Is_CardAbsent(0) == FALSE)
			{
				nNumofItem = GetNumOfChild(MITEM104_PBOOK_COPY_ALL_NO_SIM2);
				GetSequenceStringIds(MITEM104_PBOOK_COPY_ALL_NO_SIM2, nStrItemList);
				GetSequenceImageIds(MITEM104_PBOOK_COPY_ALL_NO_SIM2, nImgIltemList);
				SetParentHandler(MITEM104_PBOOK_COPY_ALL_NO_SIM2);
				RegisterHighlightHandler(ExecuteCurrHiliteHandler);
				ConstructHintsList(MITEM104_PBOOK_COPY_ALL_NO_SIM2, nHintList);		
			}
			else
#endif
			{
				nNumofItem = GetNumOfChild(MITEM104_PBOOK_COPY_ALL);
				GetSequenceStringIds(MITEM104_PBOOK_COPY_ALL, nStrItemList);
				GetSequenceImageIds(MITEM104_PBOOK_COPY_ALL, nImgIltemList);
				SetParentHandler(MITEM104_PBOOK_COPY_ALL);
				RegisterHighlightHandler(ExecuteCurrHiliteHandler);
				ConstructHintsList(MITEM104_PBOOK_COPY_ALL, nHintList);
			}
/*Fix Bug#12589 for cs2324c by cong.li on 2009.06.16. */
#ifdef __MMI_NUMBER_MENU__
			ShowCategory52Screen(STR_SCR_COPY_ALL_MENU_CAPTION, IMG_SCR_PBOOK_CAPTION,
					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
					  nHintList, 0, hilitedStorage, guiBuffer);
#else
			ShowCategory52Screen(STR_SCR_COPY_ALL_MENU_CAPTION, IMG_SCR_PBOOK_CAPTION,
					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					  nNumofItem, nStrItemList, nImgIltemList,
					  nHintList, 0, hilitedStorage, guiBuffer);
#endif
		}
		else
			DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY,  TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
	}
	else
		mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_copy_all()
* DESCRIPTION
*   Exit function for mmi_phb_entry_copy_all()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_copy_all(void) 
{
	g_phb_cntx.end_scr_id = SCR_COPY_ALL_MENU;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_op_copy_all_confirm()
* DESCRIPTION
*   confirm copy all from sim or phone
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_op_copy_all_confirm(void)
{
	UI_string_type message;
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_copy_all_confirm.>\n",__FILE__,__LINE__));

#ifdef __MMI_MULTI_SIM__
	if(g_phb_cntx.selected_storage >= MMI_SIM && g_phb_cntx.selected_storage <= MMI_SIM4)
        {   
	#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
            message = get_string(STR_COPY_ALL_SIM); //STR_OVERWRITE_TARGET;
	#else
            message = get_string(STR_COPY_ALL_SIM1 + g_phb_cntx.selected_storage - MMI_SIM); //STR_OVERWRITE_TARGET;
	#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
        }
	else
		message = get_string(STR_COPY_ALL_PHONE);//STR_OVERWRITE_TARGET;
#else
	if(g_phb_cntx.selected_storage == MMI_SIM)
		message = get_string(STR_COPY_ALL_SIM); //STR_OVERWRITE_TARGET;
	else
		message = get_string(STR_COPY_ALL_PHONE);//STR_OVERWRITE_TARGET;
#endif		
#ifdef __MMI_MULTI_SIM__
	mmi_trace(g_sw_PHB, "PHB Func: %s src = %d, des = %d, g_phb_cntx.selected_storage = %d", __FUNCTION__, phbcopymovesrc, phbcopymovedes,g_phb_cntx.selected_storage);
#endif	
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  message, IMG_GLOBAL_QUESTION, WARNING_TONE); 

	SetLeftSoftkeyFunction(mmi_phb_op_copy_all_pre_req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_copy_all_pre_req()
* DESCRIPTION
*   pre-entry function of copy all. check storage first.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_copy_all_pre_req(void)
{
#ifdef __MMI_MULTI_SIM__
	U8 nSimIndex;
#endif
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_copy_all_pre_req.>\n",__FILE__,__LINE__));
#ifdef __MMI_MULTI_SIM__
	nSimIndex = phbcopymovedes - MMI_SIM;
	if(phbcopymovedes >= MMI_SIM && phbcopymovedes <= MMI_SIM4 && g_phb_cntx.sim_used[nSimIndex] >= g_phb_cntx.sim_total[nSimIndex]) 
	{
		if(g_SecuritySetupContext.FdlStatus[nSimIndex])
		{
			DisplayPopup((PU8)GetString(STR_FDL_IS_ON), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
		else
		{
		#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			DisplayPopup((PU8)GetString(STR_SIM_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		#else
			DisplayPopup((PU8)GetString(STR_SIM1_FULL_MSG + nSimIndex), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		}
		DeleteNScrId(SCR_COPY_ALL_SELECT_SRC);
	}
	else if((phbcopymovedes == MMI_NVRAM) && (g_phb_cntx.phone_used >= g_phb_cntx.phone_total))
	{
		DisplayPopup((PU8)GetString(STR_PHONE_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		DeleteNScrId(SCR_COPY_ALL_SELECT_SRC);
	}
#else/*__MMI_MULTI_SIM__*/
	if( (g_phb_cntx.selected_storage == MMI_NVRAM) && (g_phb_cntx.sim_used[0] >= g_phb_cntx.sim_total)) 
	{
		DisplayPopup((PU8)GetString(STR_SIM_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		DeleteNScrId(SCR_COPY_ALL_MENU);
	}
	else if((g_phb_cntx.selected_storage == MMI_SIM) && (g_phb_cntx.phone_used >= g_phb_cntx.phone_total))
	{
		DisplayPopup((PU8)GetString(STR_PHONE_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		DeleteNScrId(SCR_COPY_ALL_MENU);
	}
#endif/*__MMI_MULTI_SIM__*/	
	else /*Have free space, begin to copy all*/
	{
		g_phb_mass_op_index = 0;
		g_phb_mass_op_count = 0;

		g_phb_cntx.processing = TRUE;
		mmi_phb_show_mass_processing(STR_PHB_COPYING, IMG_COPIED);
		#ifndef __MMI_MULTI_SIM__
		DeleteNScrId(SCR_COPY_ALL_MENU);
		#else
		DeleteNScrId(SCR_COPY_ALL_SELECT_SRC);
		#endif

		mmi_phb_op_copy_all_one_after_one_req(g_phb_cntx.selected_storage);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_copy_all_one_after_one_req
* DESCRIPTION
*   Copy all one entry after one.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_copy_all_one_after_one_req(U8 from_storage)
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	U16 store_index;
	module_type	dest_mod_id = MOD_L4C;	

#ifdef __MMI_MULTI_SIM__
	U16 nStart = 0, nEnd = 0;
	U8 nSimIndex = 0;
	switch(phbcopymovesrc)
	{
		case MMI_SIM:
		case MMI_SIM2:
		case MMI_SIM3:
		case MMI_SIM4:
			nSimIndex = phbcopymovesrc - MMI_SIM;
			nStart = MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES*nSimIndex;
			nEnd = nStart + MAX_PB_SIM_ENTRIES -1;
			break;
		case MMI_NVRAM:
			nStart = 0;
			nEnd = MAX_PB_PHONE_ENTRIES - 1;
			break;
		default:
			MMI_ASSERT(0);
			break;
	}
	while (((g_phb_name_index[g_phb_mass_op_index] < nStart) || (g_phb_name_index[g_phb_mass_op_index] > nEnd)) && (g_phb_mass_op_index < PhoneBookEntryCount))
		g_phb_mass_op_index++;
#else
	if(from_storage == MMI_SIM)
		while ((g_phb_name_index[g_phb_mass_op_index] < MAX_PB_PHONE_ENTRIES) && (g_phb_mass_op_index < PhoneBookEntryCount)) /*Not in SIM*/
			g_phb_mass_op_index++;
	else
		while ((g_phb_name_index[g_phb_mass_op_index] >= MAX_PB_PHONE_ENTRIES) && (g_phb_mass_op_index < PhoneBookEntryCount)) /*Not in NVRAM*/
			g_phb_mass_op_index++;
#endif
	if(g_phb_mass_op_index >= PhoneBookEntryCount)
	{
		mmi_phb_op_copy_all_done();
		return;
	}

	store_index = g_phb_name_index[g_phb_mass_op_index];

#if defined(__MMI_PHB_PINYIN_SORT__)	/*Make Pinyin cache for later sorting.*/
	mmi_phb_util_make_pinyin_cache(store_index);
#endif

	myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));

	myMsgPtr->type						= MMI_PHB_PHONEBOOK;
	myMsgPtr->no_data					= 1;
#ifdef __MMI_MULTI_SIM__
	myMsgPtr->list.storage				= phbcopymovedes;
#else
	myMsgPtr->list.storage				= (from_storage == MMI_SIM) ? MMI_NVRAM : MMI_SIM;
#endif
	myMsgPtr->list.index				= 0xFFFF; 
	myMsgPtr->list.record_index			= 0xFFFF;

	mmi_phb_op_set_entry_prepare_name_number_by_index(myMsgPtr, store_index);
#ifdef __MMI_MULTI_SIM__
	 if(myMsgPtr->list.storage >= MMI_SIM && myMsgPtr->list.storage <= MMI_SIM4)
 	{
 		dest_mod_id = MOD_L4C + myMsgPtr->list.storage - MMI_SIM;
 	}
#endif
	mmi_trace(g_sw_PHB,"-----+++++Func: %s;File: %s;Line: %d+++++-----from_storage=%d,store_index=%d,g_phb_mass_op_index=%d", __FUNCTION__, __FILE__, __LINE__,from_storage,store_index,g_phb_mass_op_index);
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_SET_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);

	SetProtocolEventHandler(mmi_phb_op_copy_all_one_after_one_rsp	,PRT_PHB_SET_ENTRY_RSP);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_copy_all_one_after_one_rsp()
* DESCRIPTION
*   copy all one entry set response.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_copy_all_one_after_one_rsp(void* info)
{
	MSG_PHB_SET_ENRTY_RSP_STRUCT* localPtr;
	S16 pError;
	U8 copy_from,copy_to;
	U16 store_index;
	U32 temp;

	
	localPtr = (MSG_PHB_SET_ENRTY_RSP_STRUCT *)info;

	if (localPtr->result.flag == 0 /*OK*/) 
	{
    // add by chenqiang 20070808 for bug 5889 5800 b
		g_phb_copyordelete_error_flag = FALSE;
    // add by chenqiang 20070808 for bug 5889 5800 e 
    		g_phb_add_nvram_full_error_flag = FALSE;
  	#ifdef __MMI_MULTI_SIM__
		if(localPtr->list[0].storage >= MMI_SIM && localPtr->list[0].storage <= MMI_SIM4)
		{
			U8 nSimIndex = localPtr->list[0].storage - MMI_SIM;
			g_phb_cntx.sim_used[nSimIndex]++;
			store_index = localPtr->list[0].record_index + MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES*nSimIndex - 1; /*Storage Location in array.*/
			copy_to = MMI_SIM + nSimIndex;
			copy_from = g_phb_cntx.selected_storage;
		}	
		else
		{
			g_phb_cntx.phone_used++;
			store_index = localPtr->list[0].record_index - 1; /*Storage Location in array.*/
			copy_to = MMI_NVRAM;
			copy_from = g_phb_cntx.selected_storage;
		}		
	#else	  
		if(localPtr->list[0].storage == MMI_SIM)
		{
			g_phb_cntx.sim_used[0]++;
			store_index = localPtr->list[0].record_index + MAX_PB_PHONE_ENTRIES - 1; /*Storage Location in array.*/
			copy_to = MMI_SIM;
			copy_from = MMI_NVRAM;

		}
		else
		{
			g_phb_cntx.phone_used++;
			store_index = localPtr->list[0].record_index - 1; /*Storage Location in array.*/
			copy_to = MMI_NVRAM;
			copy_from = MMI_SIM;

		}
	#endif
		mmi_trace(g_sw_PHB,"PHB Func: %s copy_to = %d, copy_from = %d", __FUNCTION__, copy_to, copy_from);
		if(copy_to == MMI_NVRAM)
		{
			/*Clear Optional Field*/
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			memset(&PhoneBookOptionalFields, 0, sizeof(PHB_OPTIONAL_FIELDS_STRUCT));
			WriteRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
			/*Clear Optional Id*/
			mmi_phb_clear_optional_ids(store_index);

			 /*Clear video field.*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
			mmi_phb_video_delete_by_index(store_index);
#endif
			/*Clear birthday field*/
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
			mmi_phb_bday_delete_field(store_index);
#endif
		}

#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		mmi_phb_copy_optional_ids(store_index, g_phb_name_index[g_phb_mass_op_index]);

		if(copy_to >= MMI_SIM && copy_to <= MMI_SIM4)
		{
			mmi_phb_clear_optional_ids_for_sim(store_index);
		}
#endif

		/*Populate Number Field*/
		temp = mmi_phb_util_convert_number_to_int(localPtr->list[0].tel.number);
		if(temp < INVALID_NUMBER) 
		{
			LookUpTable[g_phb_cntx.lookup_table_count].store_index = store_index;
			LookUpTable[g_phb_cntx.lookup_table_count++].number = temp;
		}

		/* Add result to Phonebook Storage*/
		mmi_phb_op_set_entry_rsp_name_number(info, store_index);

		/*Update field flag*/
		mmi_phb_op_update_field_flag(store_index, (S8*)localPtr->list[0].tel.number, NULL,NULL,NULL,NULL,TRUE);

#if defined(__MMI_PHB_PINYIN_SORT__)	/*Make Pinyin cache for later sorting.*/
	mmi_phb_util_make_pinyin_cache(store_index);
#endif
		/*Append new entry in the end of sorted list. Sorting when copy all done.*/
		g_phb_name_index[PhoneBookEntryCount] = store_index;
		PhoneBookEntryCount++;
		g_phb_mass_op_count++;

		/*Insert email to email mapping table, not like name field, it should sort here.*/
#if defined(__MMI_PHB_EMAIL_SORT__)
		if((copy_to == MMI_NVRAM) && (g_phb_email_sorted == 1))
			mmi_phb_sort_email_insert_index(store_index);
#endif

		if(g_phb_mass_op_abort) 
		{
			g_phb_mass_op_success = FALSE;
			mmi_phb_op_copy_all_done();
		}
		else 
		{
		#ifdef __MMI_MULTI_SIM__
			BOOL bCopyNext = MMI_TRUE;
			if(copy_from == MMI_NVRAM)
			{
				if(g_phb_mass_op_count >= g_phb_cntx.phone_used)
					bCopyNext = MMI_FALSE;
			}
			else
			{
				U8 nSimIndex = copy_from - MMI_SIM;
				if(g_phb_mass_op_count >= g_phb_cntx.sim_used[nSimIndex])
					bCopyNext = MMI_FALSE;
			}
			if(bCopyNext)
			{
				if(copy_to == MMI_NVRAM)
				{
					if(g_phb_cntx.phone_used >= MAX_PB_PHONE_ENTRIES)
						bCopyNext = MMI_FALSE;
				}
				else
				{
					U8 nSimIndex = copy_to - MMI_SIM;
					if(g_phb_cntx.sim_used[nSimIndex] >= g_phb_cntx.sim_total[nSimIndex])
						bCopyNext = MMI_FALSE;
				}
			}
			if(bCopyNext)
			{
				++g_phb_mass_op_index;
				mmi_phb_op_copy_all_one_after_one_req(copy_from);
			}
			else
			{
				g_phb_mass_op_success = TRUE;
				/*zhangm Add Start For 10582 Ver: GreenStone  on 2008-12-3 16:51 */
				if((copy_from >= MMI_SIM && copy_from <= MMI_SIM4 && g_phb_mass_op_count == g_phb_cntx.sim_used[copy_from - MMI_SIM])
					||(copy_from == MMI_NVRAM && g_phb_mass_op_count == g_phb_cntx.phone_used))
				{
					g_phb_mass_op_success = TRUE;
				}
				else if((copy_to >= MMI_SIM && copy_to <= MMI_SIM4 && g_phb_cntx.sim_used[copy_to - MMI_SIM] >= g_phb_cntx.sim_total[copy_to - MMI_SIM])
					||(copy_to == MMI_NVRAM && g_phb_cntx.phone_used >= MAX_PB_PHONE_ENTRIES))
				{
					g_phb_mass_op_success = FALSE;
				}
				/*zhangm Add End  For 10582 Ver: GreenStone  on 2008-12-3 16:51 */
				
				mmi_phb_op_copy_all_done();
			}		
		#else
			if((copy_to == MMI_SIM && g_phb_mass_op_count < g_phb_cntx.phone_used&& g_phb_cntx.sim_used[0] < g_phb_cntx.sim_total[0]) ||
				(copy_to == MMI_NVRAM && g_phb_mass_op_count < g_phb_cntx.sim_used[0] && g_phb_cntx.phone_used < MAX_PB_PHONE_ENTRIES))
			{
				++g_phb_mass_op_index;
				mmi_phb_op_copy_all_one_after_one_req(copy_from);
			}
			else
			{
				/*zhangm Add Start For 10582 Ver: GreenStone  on 2008-12-3 16:51 */
				if((copy_from == MMI_SIM && g_phb_mass_op_count == g_phb_cntx.sim_used[0])
					||(copy_from == MMI_NVRAM && g_phb_mass_op_count == g_phb_cntx.phone_used))
				{
					g_phb_mass_op_success = TRUE;
				}				
				else if((copy_to == MMI_SIM && g_phb_cntx.sim_used[0] >= g_phb_cntx.sim_total[0]) ||
					(copy_to == MMI_NVRAM && g_phb_cntx.phone_used >= MAX_PB_PHONE_ENTRIES))
				{
					g_phb_mass_op_success = FALSE;
				}
				/*zhangm Add End  For 10582 Ver: GreenStone  on 2008-12-3 16:51 */
				
				mmi_phb_op_copy_all_done();
			}
		#endif	
		}
	}
	else 
	{
		g_phb_mass_op_success = FALSE;
//		copy_from = (g_phb_name_index[g_phb_mass_op_index] >= MAX_PB_PHONE_ENTRIES) ? MMI_SIM : MMI_NVRAM;
		if (g_phb_name_index[g_phb_mass_op_index] >= MAX_PB_PHONE_ENTRIES)
		{
			copy_from = MMI_SIM + (g_phb_name_index[g_phb_mass_op_index] - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES ;
		}
		else
		{
			copy_from = MMI_NVRAM;
		}

		/*Not copy fail due to capacity full.*/
		if((localPtr->result.cause != MMI_PHB_CAUSE_CAPACITY_EXCEEDED) && 
			((copy_from == MMI_NVRAM && g_phb_mass_op_count < g_phb_cntx.phone_used) ||
				(copy_from >= MMI_SIM && copy_from <= MMI_SIM4 && g_phb_mass_op_count < g_phb_cntx.sim_used[copy_from - MMI_SIM])
		))
		{
			g_phb_mass_op_index++;
			mmi_phb_op_copy_all_one_after_one_req(copy_from);
		}
		else if(localPtr->result.cause == MMI_PHB_CAUSE_CAPACITY_EXCEEDED)
		{
			g_phb_mass_op_success = FALSE;
			g_phb_add_nvram_full_error_flag = TRUE;
			mmi_phb_op_copy_all_done();		
		}
		else
		{
			g_phb_mass_op_success = TRUE;
			mmi_phb_op_copy_all_done();
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_copy_all_done()
* DESCRIPTION
*   Display copy result
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_copy_all_done(void)
{
	S8 outBuf[100], count[10];

	g_phb_cntx.processing = FALSE;
	TurnOnBacklight(1);  /*Turn On Back Light When done*/
	DeleteScreenIfPresent(SCR_IN_PROGRESS_PHB);

	/*Sort name index*/
	mmi_phb_sort_build_name_index();
	
	/*Sort Number index*/
	mmi_phb_lookup_table_sort();
	g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;

	if(g_phb_mass_op_success)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	}
	//bug11349
#ifdef __MMI_MULTI_SIM__
	else if(phbcopymovesrc >= MMI_SIM && phbcopymovesrc <= MMI_SIM4 && g_SecuritySetupContext.FdlStatus[phbcopymovesrc - MMI_SIM]) /*For FDN on Message*/
#else
	else if(g_SecuritySetupContext.FdlStatus[0])  /*For FDN on Message*/
#endif		
	{ 
		DisplayPopup((PU8)GetString(STR_FDL_IS_ON), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		g_phb_mass_op_success = TRUE;
	}
 	 // add by chenqiang 20070808 for bug 5889 5800 b
 	 else if(g_phb_copyordelete_error_flag)  
	{ 
			DisplayPopup((PU8)GetString(STR_ID_PHB_COPY_OR_DELETE_TIME_OUR_ERROR), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
				g_phb_copyordelete_error_flag = FALSE;
	}
  	// add by chenqiang 20070808 for bug 5889 5800 e
 	 else if(g_phb_add_nvram_full_error_flag)
  	{
  		DisplayPopup((PU8)GetString(STR_ID_PHB_SYSTEM_FULL), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		g_phb_add_nvram_full_error_flag = FALSE;	
  	}
	else /*!g_phb_mass_op_success */
	{
			g_phb_mass_op_success = TRUE;
			sprintf(count, "%d ", g_phb_mass_op_count);
			AnsiiToUnicodeString(outBuf, count);
			pfnUnicodeStrcat(outBuf, GetString(STR_ENTRIES_COPIED)); 
			DisplayPopup((PU8)outBuf, IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}

	/*Solve Copying Screen Hang Problem*/
	DeleteScreenIfPresent(SCR_COPY_ALL_PROGRESS); 

	/*add central command for vrsi.*/
#if defined(__MMI_VRSI__)
	mmi_vrsi_init_central_abort();
	//mmi_vrsi_init_central_add_all();
#endif	
}

#define MMI_PHB_OP_DELETE_ALL
/*----------------------------------------------  Begin of PhoneBook Delete ALL -----------------------------------------*/
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_delete_all()
* DESCRIPTION
*   Highlight handler for "Delete All" in PHb main menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_delete_all(void) 
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_highlight_delete_all.>\n",__FILE__,__LINE__));

	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(mmi_phb_entry_delete_all,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_delete_all, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_delete_all_from_sim()
* DESCRIPTION
*   Highlight handler for "From SIM" in Delete All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_delete_all_from_sim(void) 
{
	U8 reqSrcMod = 0; 
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	g_phb_cntx.selected_storage = MMI_SIM + reqSrcMod;

	if(g_phb_cntx.sim_used[reqSrcMod]) 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
#if defined(__MMI_PHB_DELETE_ALL_CHECK_PHONE_LOCK__) && defined(MMI_ON_HARDWARE_P)
		SetLeftSoftkeyFunction(mmi_phb_entry_delete_all_phone_lock, KEY_EVENT_UP);
		SetKeyHandler(mmi_phb_entry_delete_all_phone_lock, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#else
		SetLeftSoftkeyFunction(mmi_phb_entry_delete_all_confirm, KEY_EVENT_UP);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#endif
	}
	else
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_delete_all_from_phone()
* DESCRIPTION
*    Highlight handler for "From Phone" in Delete All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_delete_all_from_phone(void) 
{
	g_phb_cntx.selected_storage = MMI_NVRAM;

	if(g_phb_cntx.phone_used) 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
#if defined(__MMI_PHB_DELETE_ALL_CHECK_PHONE_LOCK__) && defined(MMI_ON_HARDWARE_P)
		SetLeftSoftkeyFunction(mmi_phb_entry_delete_all_phone_lock, KEY_EVENT_UP);
		SetKeyHandler(mmi_phb_entry_delete_all_phone_lock, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#else
		SetLeftSoftkeyFunction(mmi_phb_entry_delete_all_confirm, KEY_EVENT_UP);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#endif
	}
	else
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_delete_all_from_sim()
* DESCRIPTION
*   Hint handler for "From SIM" in Delete All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_hint_delete_all_from_sim(U16 index) 
{
	U8 reqSrcMod = 0;
	S8 simStatus[40];

	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModeIndexByIndex((U8)index);
	#endif

	memset((S8*)hintData[index], 0, sizeof(hintData[index]));
	mmi_phb_memory_status_get_sim(simStatus, reqSrcMod);
	pfnUnicodeStrcpy((S8*) hintData[index], simStatus);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_delete_all_from_phone()
* DESCRIPTION
*   Hint handler for "From Phone" in Delete All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_hint_delete_all_from_phone(U16 index) 
{
	S8 phoneStatus[40];

	memset((S8*)hintData[index], 0, sizeof(hintData[index]));
	mmi_phb_memory_status_get_phone(phoneStatus);
	pfnUnicodeStrcpy((S8*) hintData[index], phoneStatus);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_delete_all_one_by_one()
* DESCRIPTION
*   Highlight handler for "One By One" in Delete All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_delete_all_one_by_one(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_phb_list_pre_delete_one_by_one, KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_list_pre_delete_one_by_one, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_delete_all()
* DESCRIPTION
*   Draws the Delete All menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_delete_all(void) 
{
	U8* guiBuffer;
	U16 nStrItemList[MMI_SIM_NUMBER + 2]; 
	U16 nImgIltemList[MMI_SIM_NUMBER + 2];
	U8*	nHintList[MMI_SIM_NUMBER + 2];
	U16 nNumofItem;
	U8 hilitedStorage;
	U8 hilight_sim, highlight_phone;

	hilight_sim = GetIndexOfStringId(MITEM103_PBOOK_DELETE_ALL, MITEM_PBOOK_DELETE_ALL_SIM);
	highlight_phone = GetIndexOfStringId(MITEM103_PBOOK_DELETE_ALL, MITEM_PBOOK_DELETE_ALL_PHONE);

#ifdef __MMI_MULTI_SIM__
	// TODO: 
	hilitedStorage = hilight_sim;
#else
	if(g_phb_cntx.prefer_storage == MMI_SIM)
	{
		if(g_phb_cntx.sim_used > 0 || g_phb_cntx.phone_used == 0)
			hilitedStorage = hilight_sim;
		else
			hilitedStorage = highlight_phone;
	}
	else
	{
		if(g_phb_cntx.phone_used > 0 || g_phb_cntx.sim_used == 0)
			hilitedStorage = highlight_phone;
		else
			hilitedStorage = hilight_sim;
	}
#endif
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_delete_all.>\n",__FILE__,__LINE__));

	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing) 
	{
		if(PhoneBookEntryCount) 
		{
			EntryNewScreen(SCR_DELETE_ALL_MENU, mmi_phb_exit_delete_all, mmi_phb_entry_delete_all, NULL);
			guiBuffer = GetCurrGuiBuffer(SCR_DELETE_ALL_MENU);	
	
			#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
			MTPNP_AD_SetMenuHideUnhide(MITEM_PBOOK_DELETE_ALL_SIM);
			#endif
			
			nNumofItem = GetNumOfChild_Ext(MITEM103_PBOOK_DELETE_ALL);
			GetSequenceStringIds_Ext(MITEM103_PBOOK_DELETE_ALL, nStrItemList);
			GetSequenceImageIds_Ext(MITEM103_PBOOK_DELETE_ALL, nImgIltemList);
			SetParentHandler(MITEM103_PBOOK_DELETE_ALL);
			#ifdef __MMI_MULTI_SIM__
			RegisterHighlightHandler( MultiSimMenuHighLightHandler );
			#else
			RegisterHighlightHandler( ExecuteCurrHiliteHandler);
			#endif
			ConstructHintsList(MITEM103_PBOOK_DELETE_ALL, nHintList);

			ShowCategory52Screen(STR_GLOBAL_DELETE, IMG_SCR_PBOOK_CAPTION,
					STR_GLOBAL_OK, IMG_GLOBAL_OK,
					STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					nNumofItem, nStrItemList, nImgIltemList,
					nHintList, 0, hilitedStorage, guiBuffer);

			SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
			SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

		}
		else
			DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY,  TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
	}
	else
		mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_delete_all()
* DESCRIPTION
*   Exit function for mmi_phb_entry_delete_all()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_delete_all(void) 
{
	g_phb_cntx.end_scr_id = SCR_DELETE_ALL_MENU;
}

#if defined(__MMI_PHB_DELETE_ALL_CHECK_PHONE_LOCK__) && defined(MMI_ON_HARDWARE_P)
/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_delete_all_phone_lock()
* DESCRIPTION
*   Exit function for mmi_phb_entry_delete_all_phone_lock()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_delete_all_phone_lock(void) 
{
	g_phb_cntx.end_scr_id = SCR_ID_DELETE_ALL_CHECK_PHONE_LOCK;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_delete_all_phone_lock()
* DESCRIPTION
*   check phone lock before delete all
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_delete_all_phone_lock(void)
{
	U8* guiBuffer;

	EntryNewScreen(SCR_ID_DELETE_ALL_CHECK_PHONE_LOCK, mmi_phb_exit_delete_all_phone_lock, mmi_phb_entry_delete_all_phone_lock, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_DELETE_ALL_CHECK_PHONE_LOCK);	

	if ( guiBuffer == NULL)
		memset(gPhonePassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1));

	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);

	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_RESTORE_PHONE,
							INPUT_TYPE_NUMERIC_PASSWORD,
							gPhonePassword,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,
							NULL, 
							NULL);

	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(mmi_phb_delete_all_verify_phone_lock,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_delete_all_verify_phone_lock,KEY_POUND,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif

//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = mmi_phb_delete_all_verify_phone_lock;
	if(pfnUnicodeStrlen((S8*)gPhonePassword) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}

	
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_delete_all_verify_phone_lock()
* DESCRIPTION
*   Call security function to check phone lock. Also provide call back function.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_delete_all_verify_phone_lock(void)
{
	U8 tempBuffer[(MAX_SIM_SETTING_PASSWD_LEN+1)];

	memset(tempBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
	SECSETVerifyPINReq(MMI_SETTING_TYPE_PHONELOCK, gPhonePassword,  tempBuffer, mmi_phb_op_delete_all_pre_req);
}

#else
/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_delete_all_confirm()
* DESCRIPTION
*   Confirms before deleting all entries from the selected storage. 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_delete_all_confirm(void) 
{
	if(g_phb_cntx.selected_storage == MMI_NVRAM)
		DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_DELETEALL_QUERY_PHONE_MSG), IMG_GLOBAL_QUESTION, WARNING_TONE); 

	else
		DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_DELETEALL_QUERY_SIM_MSG), IMG_GLOBAL_QUESTION, WARNING_TONE); 

	SetLeftSoftkeyFunction(mmi_phb_op_delete_all_pre_req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	

}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_delete_all_pre_req();
* DESCRIPTION
*   pre check before delete all.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_delete_all_pre_req(void)
{
	g_phb_mass_op_index = 0;

	g_phb_cntx.processing = TRUE;
	g_phb_mass_op_abort = FALSE;
	g_phb_mass_op_count = 0;
	g_phb_mass_op_lookup_count = 0;
	
	mmi_phb_show_mass_processing(STR_PHB_DELETING, IMG_GLOBAL_DELETED);
	DeleteNScrId(SCR_DELETE_ALL_MENU);

	mmi_phb_op_delete_all_one_after_one_req(g_phb_cntx.selected_storage);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_delete_all_one_after_one_req
* DESCRIPTION
*   delete all one entry after one.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_delete_all_one_after_one_req(U8 delete_storage)
{
	MSG_PHB_DELETE_ENTRY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	U16 store_index;
	module_type	dest_mod_id = MOD_L4C;
#ifdef __MMI_MULTI_SIM__
	U16 nStart = 0, nEnd = 0;
	U8 nSimIndex = 0;
#endif
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_delete_all_one_after_one_req.>\n",__FILE__,__LINE__));

#ifdef __MMI_MULTI_SIM__
	switch(delete_storage)
	{
		case MMI_SIM:
		case MMI_SIM2:
		case MMI_SIM3:
		case MMI_SIM4:
			nSimIndex = delete_storage - MMI_SIM;
			nStart = MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES*nSimIndex;
			nEnd = nStart + MAX_PB_SIM_ENTRIES -1;
			break;
		case MMI_NVRAM:
			nStart = 0;
			nEnd = MAX_PB_PHONE_ENTRIES - 1;
			break;
		default:
			MMI_ASSERT(0);
			break;
	}
	
	while (g_phb_name_index[g_phb_mass_op_index] < nStart|| g_phb_name_index[g_phb_mass_op_index] > nEnd)
			g_phb_mass_op_index++;
#else
	if(delete_storage == MMI_SIM)
		while (g_phb_name_index[g_phb_mass_op_index] < MAX_PB_PHONE_ENTRIES) /*Not in SIM*/
			g_phb_mass_op_index++;
	else
		while (g_phb_name_index[g_phb_mass_op_index] >= MAX_PB_PHONE_ENTRIES) /*Not in NVRAM*/
			g_phb_mass_op_index++;
#endif
	store_index = g_phb_name_index[g_phb_mass_op_index];


	myMsgPtr = (MSG_PHB_DELETE_ENTRY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_DELETE_ENTRY_REQ_STRUCT));

	myMsgPtr->storage = delete_storage;	
	myMsgPtr->del_all = MMI_FALSE;
	myMsgPtr->no_data = 1;
	myMsgPtr->type = MMI_PHB_PHONEBOOK;
	/*
	*The index means "record_index" here. This change only for MMI_PHB_PHONEBOOK(ADN) type
	* for other type( BDN ,FDN, MSISDN...) This field still need to use index as parameter.
	*/
	myMsgPtr->index = PHBGetADPIndexByStorage(delete_storage, store_index);	

#ifdef __MMI_MULTI_SIM__
	 if(delete_storage >= MMI_SIM && delete_storage <= MMI_SIM4)
 	{
 		dest_mod_id = MOD_L4C + delete_storage - MMI_SIM;
 	}
#endif	
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_DEL_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandler(mmi_phb_op_delete_all_one_after_one_rsp,PRT_PHB_DEL_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);
	DeleteNScrId(SCR_DELETE_ALL_MENU);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_delete_all_one_after_one_rsp()
* DESCRIPTION
*   response message after delete one entry.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_delete_all_one_after_one_rsp(void* info)
{
	MSG_PHB_DELETE_ENTRY_RSP_STRUCT* localPtr;
	U16 store_index;
	U16 record_index;
	U8 storage = 0;

	localPtr = (MSG_PHB_DELETE_ENTRY_RSP_STRUCT *)info;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_delete_all_one_after_one_rsp.>\n",__FILE__,__LINE__));

	if (localPtr->result.flag == 0 /*OK*/) 
	{
    // add by chenqiang 20070808 for bug 5889 5800 b
		g_phb_copyordelete_error_flag = FALSE;
    // add by chenqiang 20070808 for bug 5889 5800 e 
		store_index = g_phb_name_index[g_phb_mass_op_index];
		storage = PHBGetStorageByStoreIndex(store_index);
		record_index = PHBGetADPIndexByStorage(storage, store_index);

		g_phb_mass_op_count++;
	
		PhbUpdateSpeedDial(storage, record_index, MMI_PHB_PHONEBOOK, NULL, NULL);

		if(storage >= MMI_SIM && storage <= MMI_SIM4) /* Record store in SIM, SIM index is begin from "MAX_PB_PHONE_ENTRIES" */
		{
			g_phb_cntx.sim_used[storage - MMI_SIM]--;
		}
		else 
		{
			g_phb_cntx.phone_used--;

			/*Remove email entry from email sort mapping table*/
#if defined(__MMI_PHB_EMAIL_SORT__)
			if(g_phb_email_sorted == 1)
			mmi_phb_sort_email_delete_index(store_index);
#endif			
		}


		/*Remove lookup table entry, and sort result when delete all done.*/
		g_phb_mass_op_lookup_count += mmi_phb_op_clear_lookup_table_by_index(store_index);

#if defined(__MMI_VRSD_DIAL__)
		/*remove voice dial tag if exists.*/
		mmi_vrsddial_delete_tag_by_store_index(store_index);
#endif

		/*clear phonebook entry and remove from index*/
		mmi_phb_op_delete_update_index(store_index);

		if(g_phb_mass_op_abort) 
		{
			g_phb_mass_op_success = FALSE;
			mmi_phb_op_delete_all_done(storage);
		}
		else 
		{
			if((storage >= MMI_SIM && storage <= MMI_SIM4 && g_phb_cntx.sim_used[storage - MMI_SIM] > 0) 
				||(storage == MMI_NVRAM && g_phb_cntx.phone_used > 0)
			)
			{
				//g_phb_mass_op_index++;
				mmi_phb_op_delete_all_one_after_one_req(storage);
			}
			else
			mmi_phb_op_delete_all_done(storage);
		}
	}
	else
	{
		g_phb_mass_op_success = FALSE;
		mmi_phb_op_delete_all_done(storage);

	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_delete_all_done
* DESCRIPTION
*   delete all done. displays result.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_delete_all_done(U8 storage)
{
	S8 outBuf[100], count[10];
	
	g_phb_cntx.processing = FALSE;
	TurnOnBacklight(1);  /*Turn On Back Light When done*/
	DeleteScreenIfPresent(SCR_IN_PROGRESS_PHB);
	g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;

	/*Sort lookup table*/
	if(g_phb_mass_op_lookup_count) 
	{
		mmi_phb_lookup_table_sort();
		g_phb_cntx.lookup_table_count -= g_phb_mass_op_lookup_count;
	}

	/*Clear up optional id field*/	
	if(g_phb_cntx.selected_storage == MMI_NVRAM)
		mmi_phb_op_delete_all_update_optioal_id();

	/*No need to clear video id, because it will reset when set new entry*/

	/*Display result*/
	if(g_phb_mass_op_success)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	}	//bug11349
#ifdef __MMI_MULTI_SIM__
	else if(storage >= MMI_SIM && storage <= MMI_SIM4 && g_SecuritySetupContext.FdlStatus[storage - MMI_SIM])/*For FDN on Message*/
#else
	else if(g_SecuritySetupContext.FdlStatus[0])  /*For FDN on Message*/
#endif	
	{ 
			DisplayPopup((PU8)GetString(STR_FDL_IS_ON), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
				g_phb_mass_op_success = TRUE;
	}
  // add by chenqiang 20070808 for bug 5889 5800 b
  else if(g_phb_copyordelete_error_flag)  
	{ 
			DisplayPopup((PU8)GetString(STR_ID_PHB_COPY_OR_DELETE_TIME_OUR_ERROR), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
				g_phb_copyordelete_error_flag = FALSE;
	}
  // add by chenqiang 20070808 for bug 5889 5800 e
	else
	{
		if(!g_phb_mass_op_success) 
		{
			g_phb_mass_op_success = TRUE;
			sprintf(count, "%d ", g_phb_mass_op_count);
			AnsiiToUnicodeString(outBuf, count);
			pfnUnicodeStrcat(outBuf, GetString(STR_ID_PHB_ENTRIES_DELETED)); 
			DisplayPopup((U8*)outBuf, IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
	}

	/*Solve Copying Screen Hang Problem*/
	DeleteScreenIfPresent(SCR_COPY_ALL_PROGRESS);

	/*Re-add all central command for vrsi.*/
#if defined(__MMI_VRSI__)
	mmi_vrsi_init_central_abort();

	/*Also remove SD tag if entry is deleted.*/
#if defined(__MMI_VRSI_TRAIN_TAG__)
	mmi_vrsi_ndial_central_update_sd_mapping();
#endif

	//mmi_vrsi_init_central_add_all();
#endif		
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_refresh_phb_list()
* DESCRIPTION
*  Refresh phonebook list screens
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static void mmi_phb_refresh_phb_list(void)
{
	U16 scr_id = GetExitScrnID();
	
	if (scr_id == SCR_PBOOK_LIST || scr_id == SCR_ID_PHB_QUICK_SEARCH_LIST)
	{
		DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  IMG_GLOBAL_ACTIVATED, 
			MMI_FALSE, UI_POPUP_NOTIFYDURATION_TIME, (U8)SUCCESS_TONE );
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_ind_update_confirm()
* DESCRIPTION
*   After MMI update done, send back confirm message.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_ind_update_confirm(void)
{

	mmi_at_general_res_req_struct *myMsgPtr;
	MYQUEUE Message;

	myMsgPtr = (mmi_at_general_res_req_struct*) OslConstructDataPtr(sizeof(mmi_at_general_res_req_struct));
	myMsgPtr->result = TRUE;

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	Message.oslMsgId = MSG_ID_MMI_AT_GENERAL_RES_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	OslMsgSendExtQueue(&Message);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_ind_update()
* DESCRIPTION
*   This function handles the update indication  from stack,  after the phonebook has been 
*   updated through AT commands
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_ind_update (void* info)
{
	MSG_PHB_UPDATE_IND_STRUCT* localPtr;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_ind_update.>\n",__FILE__,__LINE__));
	localPtr = (MSG_PHB_UPDATE_IND_STRUCT *)info;
	switch(localPtr->action) 
	{
		case MMI_PHB_UPDATE :
			if ((g_phb_cntx.sim_used[0] == localPtr->sim_used) && (g_phb_cntx.phone_used == localPtr->nvram_used))
				mmi_phb_at_cmd_edit(info);
			else
				mmi_phb_at_cmd_add(info);
			break;
		case MMI_PHB_DELETE : 
			mmi_phb_at_cmd_delete(info);
			break;
		case MMI_PHB_DELETE_ALL : 
			mmi_phb_at_cmd_delete_all(info);
			break;
	}
	
	mmi_phb_ind_update_confirm();
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_at_cmd_edit()
* DESCRIPTION
*   Handles the phonebook update indication after an entry is edited through AT command
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_at_cmd_edit(void *info) 
{
	MSG_PHB_UPDATE_IND_STRUCT* localPtr;
	U16 deletedCnt = 0;
	U8 flag = 0;
	S16 pError;
	U16 store_index;

	localPtr = (MSG_PHB_UPDATE_IND_STRUCT*)info;

	if(localPtr->list[0].storage == MMI_SIM)
		store_index = localPtr->list[0].record_index + MAX_PB_PHONE_ENTRIES - 1; /*Storage Location in array.*/
	else
		store_index = localPtr->list[0].record_index - 1; /*Storage Location in array.*/
	
	/* Clear lookup table first, add later if exists.*/
	deletedCnt = mmi_phb_op_clear_lookup_table_by_index( store_index);

	/*Clear Optional ID,  Need to clean SIM entry when Associate caller group in SIM card entry*/
#if !defined(__MMI_PHB_CALLERGROUP_IN_SIM__)
	if(localPtr->list[0].storage == MMI_NVRAM) 
#endif
	{
		mmi_phb_clear_optional_ids(store_index);	
	}
			
	if(localPtr->list[0].storage == MMI_NVRAM) 
	{	
		/*Clear Optional Field*/
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)		
		memset(&PhoneBookOptionalFields, 0, sizeof(PHB_OPTIONAL_FIELDS_STRUCT));
		WriteRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
		/*Clear video field.*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
		mmi_phb_video_delete_by_index(store_index); 
#endif
		/*Clear birthday field*/
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
		mmi_phb_bday_delete_field(store_index);
#endif
	}

	flag = mmi_phb_op_increase_lookup_table(store_index,(S8*) localPtr->list[0].tel.number,NULL,NULL,NULL);

	if(flag || deletedCnt)
	{
		mmi_phb_lookup_table_sort();
		g_phb_cntx.lookup_table_count -= deletedCnt;	
	}

	PhoneBook[store_index].alpha_id.name_length	= localPtr->list[0].alpha_id.name_length;
	PhoneBook[store_index].alpha_id.name_dcs		= localPtr->list[0].alpha_id.name_dcs; 
	InputboxConvertGSMToDeviceEncoding(localPtr->list[0].alpha_id.name, 
													  PhoneBook[store_index].alpha_id.name, 
													  localPtr->list[0].alpha_id.name_length,
													  (MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH, 
													  localPtr->list[0].alpha_id.name_dcs, 
													  0, 
													  1);

	PhoneBook[store_index].tel.type				= localPtr->list[0].tel.type;
	PhoneBook[store_index].tel.length				= localPtr->list[0].tel.length;
	mmi_phb_convert_to_bcd(PhoneBook[store_index].tel.number, localPtr->list[0].tel.number, (MAX_PB_NUMBER_LENGTH+1)/2);

	/*Update is column exists flag.*/
	PhoneBook[store_index].field = 0;	
	if(strlen((S8*)localPtr->list[0].tel.number))
		PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_NUMBER;

	/*Update Pinyin Cache for sort*/
#if defined(__MMI_PHB_PINYIN_SORT__)
	mmi_phb_util_make_pinyin_cache(store_index);
#endif
		
	/*Name sorting mapping table should Re-Sort HERE, and get the new highlight position!!!*/
	mmi_phb_op_delete_update_index(store_index);
	mmi_phb_sort_insert_name_index(store_index);
	g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;

	/*Update email sorting mapping table*/
#if defined(__MMI_PHB_EMAIL_SORT__)
	if((localPtr->list[0].storage == MMI_NVRAM) && (g_phb_email_sorted == 1))
	{
		mmi_phb_sort_email_delete_index(store_index);
		mmi_phb_sort_email_insert_index(store_index);
	}
#endif		
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_at_cmd_add()
* DESCRIPTION
*   Handles the phonebook update indication after an entry is added through AT command
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_at_cmd_add(void *info) 
{

	MSG_PHB_UPDATE_IND_STRUCT* localPtr;
	S16 pError;
	U16 store_index;
	U8 flag = 0;

	localPtr = (MSG_PHB_UPDATE_IND_STRUCT*)info;

	store_index =  mmi_phb_op_get_store_index_and_increase_counter(localPtr->list[0].storage, localPtr->list[0].record_index);

	if(localPtr->list[0].storage == MMI_NVRAM) 
	{
		/*Clear Optional Field*/
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
		memset(&PhoneBookOptionalFields, 0, sizeof(PHB_OPTIONAL_FIELDS_STRUCT));
		WriteRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
		/*Clear video field.*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
		mmi_phb_video_delete_by_index(store_index);
#endif
		/*Clear birthday field*/
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
		mmi_phb_bday_delete_field(store_index);
#endif
	}

#if !defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
	if(localPtr->list[0].storage == MMI_NVRAM) 
#endif
	{
		mmi_phb_clear_optional_ids(store_index);
	}


	flag = mmi_phb_op_increase_lookup_table(store_index,(S8*) localPtr->list[0].tel.number,NULL,NULL,NULL);
	if(flag)
		mmi_phb_lookup_table_sort();

	PhoneBook[store_index].alpha_id.name_length	= localPtr->list[0].alpha_id.name_length;
	PhoneBook[store_index].alpha_id.name_dcs	= localPtr->list[0].alpha_id.name_dcs; 
	InputboxConvertGSMToDeviceEncoding(localPtr->list[0].alpha_id.name, 
													  PhoneBook[store_index].alpha_id.name, 
													  localPtr->list[0].alpha_id.name_length,
													  (MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH, 
													  localPtr->list[0].alpha_id.name_dcs, 
													  0, 
													  1);

	PhoneBook[store_index].tel.type				= localPtr->list[0].tel.type;
	PhoneBook[store_index].tel.length				= localPtr->list[0].tel.length;
	mmi_phb_convert_to_bcd(PhoneBook[store_index].tel.number, localPtr->list[0].tel.number, (MAX_PB_NUMBER_LENGTH+1)/2);

	/*Update is column exists flag.*/
	PhoneBook[store_index].field = 0;
	if(strlen((S8*)localPtr->list[0].tel.number))
		PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_NUMBER;

	/*Update Pinyin Cache for sort*/
#if defined(__MMI_PHB_PINYIN_SORT__)
	mmi_phb_util_make_pinyin_cache(store_index);
#endif

	/*Insert name to name mapping table.*/
	mmi_phb_sort_insert_name_index(store_index);

	/*Insert email to email mapping table*/
#if defined(__MMI_PHB_EMAIL_SORT__)
	if((localPtr->list[0].storage == MMI_NVRAM) && (g_phb_email_sorted == 1))
		mmi_phb_sort_email_insert_index(store_index);
#endif

	g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;	
	
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_at_cmd_delete()
* DESCRIPTION
*   Handles the phonebook update indication after an entry is deleted through AT command.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_at_cmd_delete(void *info) 
{
	MSG_PHB_UPDATE_IND_STRUCT* localPtr;
	U16 deletedCnt = 0;
	U16 store_index;
	PHB_OPTIONAL_IDS_STRUCT* opt_ids;

	localPtr = (MSG_PHB_UPDATE_IND_STRUCT *)info;

	if(localPtr->list[0].storage == MMI_SIM)
	{
		store_index = localPtr->list[0].record_index + MAX_PB_PHONE_ENTRIES - 1; /*Storage Location in array.*/
		g_phb_cntx.sim_used[0]--;
	}
	else /*MMI_NVRAM*/
	{
		store_index = localPtr->list[0].record_index - 1;
		g_phb_cntx.phone_used--;

#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
		/*Delete image file in file folder*/
		mmi_phb_read_optional_ids(store_index);
		opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;
	
		if( opt_ids->pictureTagID == 1)
			mmi_phb_image_delete_by_index((U16)(store_index+1));
#endif				

#if defined(__MMI_INCOMING_CALL_VIDEO__)
		mmi_phb_video_delete_by_index(store_index); /*Clear video field.*/
#endif

		/*Remove email entry from email sort mapping table*/
#if defined(__MMI_PHB_EMAIL_SORT__)
		if(g_phb_email_sorted == 1)
		mmi_phb_sort_email_delete_index(store_index);
#endif
	}

	/*Remove lookup table entry*/
	deletedCnt = mmi_phb_op_clear_lookup_table_by_index(store_index);

	if(deletedCnt) 
	{
		mmi_phb_lookup_table_sort();
		g_phb_cntx.lookup_table_count -= deletedCnt;
	}

	/*clear phonebook entry and remove from index*/
	mmi_phb_op_delete_update_index(store_index);

	g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;
	mmi_phb_refresh_phb_list();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_at_cmd_delete_all()
* DESCRIPTION
*   Handles the phonebook update indication after an all the entries of one storage 
*   location is deleted through AT command
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_at_cmd_delete_all(void *info) 
{
	U8 storage;
	MSG_PHB_UPDATE_IND_STRUCT* localPtr;

	localPtr = (MSG_PHB_UPDATE_IND_STRUCT*)info;
	
	if(localPtr->nvram_used == 0 && g_phb_cntx.phone_used != 0) 
	{
		storage = MMI_NVRAM;
		mmi_phb_op_delete_all_update_optioal_id();
	}
	else
		storage = MMI_SIM;

	/*Clear all storage and clear up name index.*/
	mmi_phb_op_delete_all_update_index(storage);
	mmi_phb_op_delete_all_update_lookup_table(storage);

	/*Remove email entry from email sort mapping table*/
#if defined(__MMI_PHB_EMAIL_SORT__)
	if((storage == MMI_NVRAM) && (g_phb_email_sorted == 1))
		mmi_phb_sort_email_delete_all_index();
#endif	
	mmi_phb_refresh_phb_list();
}


#define MMI_PHB_OPERATE_UTIL
/*****************************************************************************
* FUNCTION
*  mmi_phb_mass_processing_abort()
* DESCRIPTION
*   abort function of processing screen. It will set up the abort flag.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   set up abort flag
*****************************************************************************/
void mmi_phb_mass_processing_abort(void) 
{
	g_phb_mass_op_abort = TRUE;
	mmi_phb_show_in_progress(STR_COPY_ALL_ABORTING, IMG_COPY_ALL_ABORTING);
	DeleteScreenIfPresent(SCR_COPY_ALL_PROGRESS);
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_op_delete_all_update_index()
* DESCRIPTION
*   update name index after delete all.
*
* PARAMETERS
*  storage IN storage location
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_delete_all_update_index(U8 storage)
{
	U16 pos,i;

	for(i = 0 ; i < PhoneBookEntryCount ; i++)
	{
		if((storage == MMI_SIM && g_phb_name_index[i] >= MAX_PB_PHONE_ENTRIES) ||
			(storage == MMI_NVRAM && g_phb_name_index[i] < MAX_PB_PHONE_ENTRIES))
				g_phb_name_index[i] = 0xffff;
	}

	pos = 0;
	while (pos < PhoneBookEntryCount)
	{
		if(g_phb_name_index[pos] == 0xffff)
		{
			for(i = pos ; i < PhoneBookEntryCount ; i++)
				g_phb_name_index[i] = g_phb_name_index[i+1];

			PhoneBookEntryCount--;
		}
		pos++;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_delete_all_update_lookup_table()
* DESCRIPTION
*   clear lookup table
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_delete_all_update_lookup_table(U8 storage)
{
	U16 k;
	U16 deletedCnt = 0;

	/*Remove lookup table entry*/
	for(k = 0; k < g_phb_cntx.lookup_table_count; ++k) 
	{
		if((storage == MMI_SIM && LookUpTable[k].store_index >= MAX_PB_PHONE_ENTRIES) ||
			(storage == MMI_NVRAM && LookUpTable[k].store_index < MAX_PB_PHONE_ENTRIES)) 
		{

			LookUpTable[k].number = INVALID_NUMBER;
			++deletedCnt;
		}
	}
	if(deletedCnt) 
	{
		mmi_phb_lookup_table_sort();
		g_phb_cntx.lookup_table_count -= deletedCnt;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_delete_all_update_optioal_id()
* DESCRIPTION
*   Update optional id on NVRAM after delete all.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_delete_all_update_optioal_id(void)
{
	U16 i,j;
	S16 pError;
	U8 optid_record, optid_index, current_record = 0;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

	for(i = 0 ; i < MAX_PB_PHONE_ENTRIES; i++) 
	{
		/*If entry still exist, do not clear.*/
		for(j = 0 ; j < PhoneBookEntryCount ; j++)
			if(i == g_phb_name_index[j])
				continue;
	
		/*Get optional IDs record.*/
		optid_record = (i / OPTIONAL_IDS_RECORD_TOTAL) + 1;
		optid_index = i - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

		if(optid_record != current_record)
		{
			ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
			current_record = optid_record;
		}

		/*Clear image in file system*/
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
		if(PhbOptIDs[optid_index].pictureTagID == 1)
			mmi_phb_image_delete_by_index((U16)(i+1));
#endif
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_update_optional_field()
* DESCRIPTION
*   Saves optional fields of a Phone entry on NVRAM
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_update_optional_field(U16 store_index, U8 storage, U8 type) 
{
	S16 pError;
	U8 optid_record;
	U8 optid_index;	
	//U8 grpIndex;
	PHB_OPTIONAL_IDS_STRUCT tempIDs;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];
	//PHB_CALLER_GROUP_STRUCT *callerGroups;

	/*
	*  Update optional fields.
	*/
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	if(storage == MMI_NVRAM)
	{
	UnicodeToAnsii((PS8)PhoneBookOptionalFields.homeNumber , pbHomeNumber);
	pfnUnicodeStrcpy((PS8)PhoneBookOptionalFields.companyName, pbCompanyName);
	UnicodeToAnsii((PS8)PhoneBookOptionalFields.emailAddress, pbEmailAddress);
	UnicodeToAnsii((PS8)PhoneBookOptionalFields.officeNumber, pbOfficeNumber);
	UnicodeToAnsii((PS8)PhoneBookOptionalFields.faxNumber, pbFaxNumber);

		WriteRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
	}
#endif

	/*
	*  Update optional selection field (id), and caller group counter
	*/
	/*Read out related optional id record*/
	optid_record = (store_index  / OPTIONAL_IDS_RECORD_TOTAL) + 1;
	optid_index = store_index  - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;
	ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);


	/*assign user selected optional id*/
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	if(g_phb_cntx.selected_pic_index == g_phb_cntx.total_image_id - 1)
		tempIDs.pictureTagID = 1;			
	else
#endif
		tempIDs.pictureTagID = g_phb_cntx.image_id_list[g_phb_cntx.selected_pic_index];

	tempIDs.callerGroupID = (U8)g_phb_cntx.selected_grp_index;
	tempIDs.ringToneID = g_phb_cntx.ring_tone_list[g_phb_cntx.selected_ring_index];

#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
	if(storage >= MMI_SIM && storage <= MMI_SIM4)
	{
		tempIDs.pictureTagID = 0;
		tempIDs.ringToneID = 0;	
	}
#endif

	/*Update associate id and write it back*/
	memcpy(&PhbOptIDs[optid_index], &tempIDs, sizeof(PHB_OPTIONAL_IDS_STRUCT));	
	WriteRecord(NVRAM_EF_PHB_IDS_LID, optid_record, PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_read_optional_ids()
* DESCRIPTION
*   read one optional id for one entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_read_optional_ids(U16 store_index)
{
	U8 optid_record;
	U8 optid_index;
	S16 pError;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];
  // add by chenqiang for init PhbOptIDs 20070802
  memset(PhbOptIDs , 0, sizeof(PhbOptIDs));
	optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
	optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

	ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
  // add by chenqiang for bug5908 B 20070802
  if(PhbOptIDs[optid_index].callerGroupID > MAX_PB_CALLER_GROUPS)
  {
    PhbOptIDs[optid_index].callerGroupID  = 0;
  }
  // add by chenqiang for bug5908 E 20070802
	memcpy(g_phb_cntx.optional_ids, &PhbOptIDs[optid_index], sizeof(PHB_OPTIONAL_IDS_STRUCT));
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_clear_optional_ids()
* DESCRIPTION
*   clear optional id field
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_clear_optional_ids(U16 store_index)
{
	U8 optid_record;
	U8 optid_index;
	S16 pError;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

	optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
	optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

	ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

	memset(&PhbOptIDs[optid_index], 0, sizeof(PHB_OPTIONAL_IDS_STRUCT));
	WriteRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_clear_optional_ids_for_sim()
* DESCRIPTION
*   clear optional id field
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
void mmi_phb_clear_optional_ids_for_sim(U16 store_index)
{
	U8 optid_record;
	U8 optid_index;
	S16 pError;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

	optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
	optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

	ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

	/* Clear Picture ID and Ring Tone ID for SIM because SIM does not have these fields.*/
	PhbOptIDs[optid_index].pictureTagID = 0;
	PhbOptIDs[optid_index].ringToneID = 0;
	
	WriteRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_copy_optional_ids()
* DESCRIPTION
*   copy optional id field
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_copy_optional_ids(U16 dst_store_index, U16 src_store_index)
{
	U8 src_optid_record, dst_optid_record;
	U8 optid_index;
	S16 pError;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];
	PHB_OPTIONAL_IDS_STRUCT tempIDs;

	/*Read Source ID first*/
	src_optid_record = (src_store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
	optid_index = src_store_index - (src_optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

	ReadRecord(NVRAM_EF_PHB_IDS_LID, src_optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

	/*Copy to temp buffer.*/
	memcpy(&tempIDs, &PhbOptIDs[optid_index], sizeof(PHB_OPTIONAL_IDS_STRUCT));				

	/*Read Destination ID*/
	dst_optid_record = (dst_store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
	optid_index = dst_store_index - (dst_optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

	if(dst_optid_record != src_optid_record)
		ReadRecord(NVRAM_EF_PHB_IDS_LID, dst_optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

	/*Copy from temp buffer.*/
	memcpy(&PhbOptIDs[optid_index], &tempIDs, sizeof(PHB_OPTIONAL_IDS_STRUCT));				

	/*Write Back To buffer*/
	WriteRecord(NVRAM_EF_PHB_IDS_LID, dst_optid_record, PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

}
/*****************************************************************************
* FUNCTION
*  mmi_phb_op_set_entry_prepare_name_number()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_set_entry_prepare_name_number(void* dst_struct, S8* in_name, S8* in_num)
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
#if defined(__PHB_0x81_SUPPORT__)	
	U8 length;
#endif
	
	myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) dst_struct;

	if(dst_struct == NULL)
		return;

	if(in_num != NULL)
	{
		/*Prepate Number Data*/
		if (in_num[0] == '+') 
		{
			myMsgPtr->list.tel.type = MMI_CSMCC_INTERNATIONAL_ADDR; /* INTERNATIONAL */
			myMsgPtr->list.tel.length = (U8)pfnUnicodeStrlen(in_num + ENCODING_LENGTH);  /* Exclude "+" */
			UnicodeToAnsii((PS8)myMsgPtr->list.tel.number, in_num + ENCODING_LENGTH);
		}
		else 
		{
			myMsgPtr->list.tel.type = MMI_CSMCC_DEFAULT_ADDR_TYPE; /*DEFAULT*/
			myMsgPtr->list.tel.length = (U8)pfnUnicodeStrlen(in_num);
			UnicodeToAnsii((PS8)myMsgPtr->list.tel.number, in_num);
		}
	}

	if(in_name != NULL)
	{
		/*Prepare Name Data*/
		if(GetUCS2Flag(in_name)) 
		{
#if defined(__PHB_0x81_SUPPORT__)	/*Support maximum length of 0x81 UCS2*/
			if((length = mmi_phb_convert_to_0x81((S8*)in_name,TRUE)) > 0) 
			{
				myMsgPtr->list.alpha_id.name_dcs = 0x09;
				myMsgPtr->list.alpha_id.name_length = length;
				memcpy(myMsgPtr->list.alpha_id.name, in_name, length);
				memset(myMsgPtr->list.alpha_id.name+length, 0x00, 1);
			}
			else
#endif
			{
			myMsgPtr->list.alpha_id.name_dcs = MMI_PHB_UCS2;
			myMsgPtr->list.alpha_id.name_length = ((U8)pfnUnicodeStrlen(in_name))*ENCODING_LENGTH;
			InputboxConvertDeviceEncodingToGSM((PU8)in_name, 
														   myMsgPtr->list.alpha_id.name, 
														   MAX_PS_NAME_LENGTH, 
														   MMI_PHB_UCS2, 
														   0, 
														   1);
			}
	    }
	    else 
	    {
			myMsgPtr->list.alpha_id.name_dcs = MMI_PHB_ASCII;
			myMsgPtr->list.alpha_id.name_length = 
				InputboxConvertDeviceEncodingToGSM((PU8)in_name, 
																myMsgPtr->list.alpha_id.name, 
																MAX_PS_NAME_LENGTH, 
																MMI_PHB_ASCII, 
																0, 
																1) - 1;
	    }
	}
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_truncate_pbname_buffer
 * DESCRIPTION
 
 * PARAMETERS
 *  dst_store_index     [IN]        
 *  src_store_index     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
void mmi_phb_truncate_pbname_buffer(U8 storage)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 max_name_len = 0;
#if defined(__PHB_0x81_SUPPORT__)  
    U8 ucs2_0x81_name_length;
    S8 ucs2_0x81_name[(MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH];
#endif /* defined(__PHB_0x81_SUPPORT__) */
    U8 name_extend_char_num = GetUCS2ExtendedNum((PS8)pbName, max_name_len);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (storage == MMI_NVRAM)
    {
        max_name_len = MAX_PB_NAME_LENGTH;
    }
    else
    {
        max_name_len = g_phb_cntx.sim_name_len[0];    
    }

#if defined(__PHB_0x81_SUPPORT__)       /* Support maximum length of 0x81 UCS2 */
    mmi_ucs2ncpy(ucs2_0x81_name, pbName, (MAX_PB_NAME_LENGTH + 1));
    // use ucs2_0x81_name to store original content
    if ((ucs2_0x81_name_length = mmi_phb_convert_to_0x81((S8*) ucs2_0x81_name, TRUE)) > 0)
    {
        if (ucs2_0x81_name_length > max_name_len)
        {
            memset(pbName + ((max_name_len - 3) * 2), 0, 2);
        }
        else
        {
            memset(pbName + ((ucs2_0x81_name_length - 3) * 2), 0, 2);
        }
    }
    else
#endif /* defined(__PHB_0x81_SUPPORT__) */         	
    if (GetUCS2Flag(pbName)) /* there is chinese character, length = 14 */
    {
        if (storage == MMI_NVRAM)
        {
            memset(pbName + ((max_name_len) / 2 * 2), 0, 2);
        }
        else
        {
            memset(pbName + ((max_name_len - 1) / 2 * 2), 0, 2);
        }
    }
    else
    {
        if ((mmi_ucs2strlen(pbName) + name_extend_char_num) > max_name_len)
        {     
            memset(pbName + ((max_name_len - name_extend_char_num) * 2), 0, 2);
        }
        else
        {
            memset(pbName + (max_name_len * 2), 0, 2);
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_calculate_number_valid_length
 * DESCRIPTION
 
 * PARAMETERS
 *  in_number     [IN]     
 *  out_number    [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
U8 mmi_phb_calculate_number_valid_length(S8 *in_number)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 valid_length = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (!((*in_number == 0) && (*(in_number + 1) == 0)))
    {
        if ((*in_number >= '0' && *in_number <= '9') || *in_number =='*' || *in_number == '+' || 
            *in_number == '#' || *in_number == 'p'|| *in_number == 'P'|| *in_number == 'w' || *in_number == 'W')
        {
            valid_length ++;
        }
        in_number += 2;        
    }
    return valid_length;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_number_remove_invalid_character
 * DESCRIPTION
 *  remove invalid characters in number
 * PARAMETERS
 *  in_number     [IN]     
 *  out_number    [IN/OUT]     
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_phb_number_remove_invalid_character(S8 *num)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (!((*num == 0) && (*(num + 1) == 0)))
    {
        if (!((*num >= '0' && *num <= '9') || *num =='*' || *num == '+' || 
            *num == '#' || *num == 'p'|| *num == 'P'|| *num == 'w' || *num == 'W'))
        {
            for(i = 0; (!(*(num + i) == 0) && (*(num + i + 1) == 0)); (i = i + 2))
            {
                *(num + i) = *(num + i + 2);
                *(num + i + 1) = *(num + i + 3);
            }
            *(num + i) = 0;
            *(num + i + 1) = 0;
        }
        else
        {
            if (*num == 'P')
            {
                *num = 'p';
            }
            else if (*num == 'W')
            {
                *num = 'w';
            }
            num += 2;
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_name_remove_invalid_character
 * DESCRIPTION
 *  remove invalid characters in name
 * PARAMETERS
 *  in_name     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_phb_name_remove_invalid_character(S8 *name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 pivot, copy_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (!((*name == 0) && (*(name + 1) == 0)))
    {
        /* now only remove 0D0A, phb doesn't support 0D0A */
        if ((*name == 0x0D) && (*(name + 1) == 0) && (*(name + 2) == 0x0A) && (*(name + 3) == 0))
        {
            pivot = 0;
            copy_ptr = 4;
            do
            {
                *(name + pivot) = *(name + copy_ptr);
                *(name + pivot + 1) = *(name + copy_ptr + 1);
                pivot += 2;
                copy_ptr += 2;
            }while (!((*(name + pivot - 2) == 0) && (*(name + pivot - 1) == 0)));
        }
        /* remove 1B00, because editor use 1B00 to be an identification, so 1B00 should be filtered*/
        else if ((*name == 0x1B) && (*(name + 1) == 0))
        {
            for(pivot = 0; (!(*(name + pivot) == 0) && (*(name + pivot + 1) == 0)); (pivot += 2))
            {
                *(name + pivot) = *(name + pivot + 2);
                *(name + pivot + 1) = *(name + pivot + 3);
            }
            *(name + pivot) = 0;
            *(name + pivot + 1) = 0;        
        }
        else
        {
            name += 2;
        }
    }
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_set_entry_prepare_name_number_by_index()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_set_entry_prepare_name_number_by_index(void* dst_struct, U16 store_index)
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
	U8 maxSimNameLen = 0;
	
#if defined(__PHB_0x81_SUPPORT__)
	U8 length;
#endif

	myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) dst_struct;
	myMsgPtr->list.alpha_id.name_length = PhoneBook[store_index].alpha_id.name_length;
	myMsgPtr->list.alpha_id.name_dcs = PhoneBook[store_index].alpha_id.name_dcs;
	PhoneBook[store_index].alpha_id.name[MAX_PB_NAME_LENGTH * ENCODING_LENGTH - 1] = '\0';   //make sure the end of array
	PhoneBook[store_index].alpha_id.name[MAX_PB_NAME_LENGTH * ENCODING_LENGTH] = '\0';

	//2007-06-15, Li Tao, fix bug P78000000056, begin
	if (GetUCS2Flag(PhoneBook[store_index].alpha_id.name))		//the name include chinese font
	{
		maxSimNameLen = MAX_SIM_NAME_CHINESE_LENGTH * ENCODING_LENGTH;

		if (myMsgPtr->list.alpha_id.name_length > maxSimNameLen)
			myMsgPtr->list.alpha_id.name_length = maxSimNameLen;
		
		memset(myMsgPtr->list.alpha_id.name, '\0', myMsgPtr->list.alpha_id.name_length + ENCODING_LENGTH);

		InputboxConvertDeviceEncodingToGSM(PhoneBook[store_index].alpha_id.name, 
													  myMsgPtr->list.alpha_id.name, 
												  	  //MAX_PS_NAME_LENGTH, 
													  maxSimNameLen,	//2007-06-15, Li Tao, fix bug P78000000056
													  PhoneBook[store_index].alpha_id.name_dcs, 
													  0, 
													  1);

#if defined(__PHB_0x81_SUPPORT__)	/*Support maximum length of 0x81 UCS2*/
		if(PhoneBook[store_index].alpha_id.name_dcs == MMI_PHB_UCS2) 
			if((length = mmi_phb_convert_to_0x81((S8*)myMsgPtr->list.alpha_id.name,TRUE)) > 0) 
			{
				myMsgPtr->list.alpha_id.name_dcs = 0x09;
				myMsgPtr->list.alpha_id.name_length = length;
			}
#endif
		
	}
	else
	{
		maxSimNameLen = MAX_SIM_NAME_LENGTH;
		
		if (myMsgPtr->list.alpha_id.name_length > maxSimNameLen)
			myMsgPtr->list.alpha_id.name_length = maxSimNameLen;
		
		memset(myMsgPtr->list.alpha_id.name, '\0', myMsgPtr->list.alpha_id.name_length + 1);

		InputboxConvertDeviceEncodingToGSM(PhoneBook[store_index].alpha_id.name, 
													  myMsgPtr->list.alpha_id.name, 
												  	  //MAX_PS_NAME_LENGTH, 
													  maxSimNameLen,	//2007-06-15, Li Tao, fix bug P78000000056
													  PhoneBook[store_index].alpha_id.name_dcs, 
													  0, 
													  1);
	}
/*
mmi_trace(1, "<<<PhoneBookOperateMass.c<<<prepare_name_number_by_index<<< maxSimNameLen = %d, myMsgPtr->list.alpha_id.name_length = %d",
	maxSimNameLen, myMsgPtr->list.alpha_id.name_length);

mmi_trace(1, "<<<PhoneBookOperateMass.c<<<prepare_name_number_by_index<<< Data Source -- PhoneBook[store_index].alpha_id.name_length = %d",
	PhoneBook[store_index].alpha_id.name_length);
*/
	//2007-06-15, Li Tao, fix bug P78000000056, end
	
	myMsgPtr->list.tel.type = PhoneBook[store_index].tel.type;
	//myMsgPtr->list.tel.length = PhoneBook[store_index].tel.length;

	//2007-06-15, Li Tao, fix bug P78000000056
	//mmi_phb_convert_to_digit(myMsgPtr->list.tel.number, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
	mmi_phb_convert_to_digit(myMsgPtr->list.tel.number, PhoneBook[store_index].tel.number, MAX_SIM_NUMBER_LENGTH + 1);
	myMsgPtr->list.tel.length = strlen((S8*)myMsgPtr->list.tel.number);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_set_entry_rsp_name_number()
* DESCRIPTION
*   set name and number in the set entry response message.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_set_entry_rsp_name_number(void* rsp_struct, U16 store_index)
{

	MSG_PHB_SET_ENRTY_RSP_STRUCT* localPtr;


	localPtr = (MSG_PHB_SET_ENRTY_RSP_STRUCT*)rsp_struct;

	PhoneBook[store_index].alpha_id.name_length	= localPtr->list[0].alpha_id.name_length;
	PhoneBook[store_index].alpha_id.name_dcs	= localPtr->list[0].alpha_id.name_dcs; 
	InputboxConvertGSMToDeviceEncoding(localPtr->list[0].alpha_id.name, 
														  PhoneBook[store_index].alpha_id.name, 
														  localPtr->list[0].alpha_id.name_length,
														  (MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH, 
														  localPtr->list[0].alpha_id.name_dcs, 
														  0, 
														  1);
#if defined(__PHB_0x81_SUPPORT__)	/*Support maximum length of 0x81 UCS2*/
		if(PhoneBook[store_index].alpha_id.name_dcs == MMI_PHB_UCS2_81 || 
		   PhoneBook[store_index].alpha_id.name_dcs == MMI_PHB_UCS2_82) 
			   PhoneBook[store_index].alpha_id.name_dcs = MMI_PHB_UCS2;
#endif

		PhoneBook[store_index].tel.type				= localPtr->list[0].tel.type;
		PhoneBook[store_index].tel.length				= localPtr->list[0].tel.length;
		mmi_phb_convert_to_bcd(PhoneBook[store_index].tel.number, localPtr->list[0].tel.number, (MAX_PB_NUMBER_LENGTH+1)/2);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_get_store_index_and_increase_counter()
* DESCRIPTION
*   Use storage and record_index to get phonebook store_index(store position in the array)
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_op_get_store_index_and_increase_counter(U8 storage, U16 record_index)
{
	U16 store_index = 0;

	if(storage >= MMI_SIM && storage <= MMI_SIM4)
	{
		U8 nSimIndex = storage - MMI_SIM;
		g_phb_cntx.sim_used[nSimIndex]++;
		store_index = record_index + MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES*nSimIndex - 1; /*Storage Location in array.*/
		MMI_ASSERT((g_phb_cntx.sim_used[nSimIndex] <= g_phb_cntx.sim_total[nSimIndex]) && (store_index < MAX_PB_SIM_ENTRIES*(nSimIndex+1)+ MAX_PB_PHONE_ENTRIES));
	}
	else
	{
		g_phb_cntx.phone_used++;
		store_index = record_index - 1; /*Storage Location in array.*/
		MMI_ASSERT((g_phb_cntx.phone_used <= g_phb_cntx.phone_total) && (store_index < MAX_PB_PHONE_ENTRIES));
	}


	return store_index;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_clear_lookup_table_by_index()
* DESCRIPTION
*   clear one entry in number lookup table
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_op_clear_lookup_table_by_index(U16 store_index)
{
	U16 deletedCnt = 0;
	U16 k;

	for(k = 0; k < g_phb_cntx.lookup_table_count; ++k) 
	{
		if(LookUpTable[k].store_index == store_index )
		{
			LookUpTable[k].number = INVALID_NUMBER;
			deletedCnt++;
		}
	}

	return deletedCnt;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_increase_lookup_table()
* DESCRIPTION
*   add one entry into lookup table
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_op_increase_lookup_table(U16 store_index, S8* number, S8* fax, S8* home, S8* office)
{
	U32 temp;
	U8 flag = 0;

	if(number != NULL)
	{
		temp = mmi_phb_util_convert_number_to_int((U8*)number);
		if(temp < INVALID_NUMBER) 
		{
			flag = 1;
			LookUpTable[g_phb_cntx.lookup_table_count].store_index = store_index;
			LookUpTable[g_phb_cntx.lookup_table_count++].number = temp;
		}
	}

	if(fax != NULL)
	{
		temp = mmi_phb_util_convert_number_to_int((U8*)fax);
		if(temp < INVALID_NUMBER) 
		{
			flag = 1;
			LookUpTable[g_phb_cntx.lookup_table_count].store_index = store_index;
			LookUpTable[g_phb_cntx.lookup_table_count++].number = temp;
		}
	}

	if(home != NULL)
	{
		temp = mmi_phb_util_convert_number_to_int((U8*)home);
		if(temp < INVALID_NUMBER) 
		{
			flag = 1;
			LookUpTable[g_phb_cntx.lookup_table_count].store_index = store_index;
			LookUpTable[g_phb_cntx.lookup_table_count++].number = temp;
		}
	}

	if(office != NULL)
	{
		temp = mmi_phb_util_convert_number_to_int((U8*)office);
		if(temp < INVALID_NUMBER) 
		{
			flag = 1;
			LookUpTable[g_phb_cntx.lookup_table_count].store_index = store_index;
			LookUpTable[g_phb_cntx.lookup_table_count++].number = temp;
		}
	}

	return flag;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_update_field_flag()
* DESCRIPTION
*   update fileter flag of each entry.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_update_field_flag(U16 store_index, S8* number, S8* home, S8* office, S8* fax, S8* email, BOOL is_new)
{
	/*New record, clear first.*/
	if(is_new)
		PhoneBook[store_index].field = 0;

	if(number != NULL)
	{
		if(strlen(number))
			PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_NUMBER;
		else if(!is_new)	/*Clear flag when the field is empty and the record is not a new record*/
			PhoneBook[store_index].field &= ~MMI_PHB_ENTRY_FIELD_NUMBER;
	}
		
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	if(home != NULL)
	{
		if(strlen(home) > 0)
			PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_HOME;
		else if(!is_new)
			PhoneBook[store_index].field &= ~MMI_PHB_ENTRY_FIELD_HOME;				
	}

	if(office != NULL)
	{
		if(strlen(office) > 0)
			PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_OFFICE;
		else if(!is_new)
			PhoneBook[store_index].field &= ~MMI_PHB_ENTRY_FIELD_OFFICE;
	}
	
	if(fax != NULL)
	{
		if(strlen(fax) > 0)
			PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_FAX;
		else if(!is_new)
			PhoneBook[store_index].field &= ~MMI_PHB_ENTRY_FIELD_FAX;		
	}
	
	if(email != NULL)
	{
		if(strlen(email) > 0)
			PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_EMAIL;
		else if(!is_new)
			PhoneBook[store_index].field &= ~MMI_PHB_ENTRY_FIELD_EMAIL;		
	}
#endif
}

#endif /* _PHONEBOOKOPERATEMASS_C */


