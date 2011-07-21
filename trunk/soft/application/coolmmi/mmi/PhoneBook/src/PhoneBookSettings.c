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
 * PhoneBookSettings.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Implementation of feature "Settings" in the Phobebook Application
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _PHONEBOOKSETTINGS_C
#define _PHONEBOOKSETTINGS_C


#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalmenuitems.h"
#include "phonebooktypes.h"
#include "phonebookdef.h"
#include "debuginitdef.h"
#include "eventsgprot.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "protocolevents.h"
#include "globaldefs.h"
#include "historygprot.h"
#include "unicodexdcl.h"
#include "timerevents.h"
#include "queuegprot.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "speeddial.h"
#include "commonscreens.h"
#if defined(__MMI_VCARD__)
#include "vcard.h"
#endif

#if defined(__MMI_PHB_CALLER_RES_SETTING__)
#include "filemgr.h"  /* should include filemgr.h before profilemgrgprot.h */
#include "filemanagergprot.h"

#include "profilesenum.h"
#include "profilesdefs.h"
#include "profiles_prot.h"
#include "profilegprots.h" //for getringtoneid
#if defined(__MMI_INCOMING_CALL_VIDEO__)
#include "vdorecgprot.h"
#endif
#endif

#ifdef __FLIGHT_MODE_SUPPORT__
#include "simdetectiongexdcl.h"
#endif
#undef __NEWSIMULATOR
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "mtpnp_ad_resdef.h"
#endif
#include "mmi_trace.h"
#include "simdetectiongprot.h"


/*
** Local Variable
*/
static U8 g_phb_temp_field_state[MAX_PB_VIEW_FIELDS];
U8 g_phb_temp_storage;

/*
** Global Variable
*/
/*For Edit vCard*/
extern wgui_inline_item wgui_inline_items[];
extern U16 TitleStrings[];
extern S8 pbName[];
extern S8 pbNumber[];
extern S8 pbHomeNumber[];
extern S8 pbCompanyName[];
extern S8 pbEmailAddress[];
extern S8 pbOfficeNumber[];
extern S8 pbFaxNumber[];
extern U8 gPictureItemIndex;
extern U8 gRingtoneItemIndex;
#if defined(__MMI_INCOMING_CALL_VIDEO__)
extern U8 g_phb_video_item_index;
#endif


extern pBOOL viewOnce;
extern MMI_PHB_ENTRY_BCD_STRUCT	PhoneBook[];
extern PHB_OPTIONAL_FIELDS_STRUCT	PhoneBookOptionalFields;

#if defined(__MMI_PHB_CALLER_RES_SETTING__)
#if defined(__MMI_FILE_MANAGER__)
extern S8 g_phb_file_path[];
#endif

#if defined(__MMI_INCOMING_CALL_VIDEO__)
extern S8 g_phb_video_file_path[];
#endif
#if defined(__MMI_CAMERA__)
extern BOOL mmi_camera_get_storage_file_path(PS8 filepath);
#endif
extern void PrfSetRingToneConfType(PRFSET_MELODY_CONF_TYPE type);
#endif

/*
** Global Function
*/

#define MMI_PHB_SETTING_INIT
/*****************************************************************************
* FUNCTION
*  mmi_phb_setting_init()
* DESCRIPTION
*   Initialization of "Settings" feature
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_setting_init(void)
{
	U8 i;
	mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

	/*Settings*/
	SetHiliteHandler(MITEM110_PBOOK_SETTINGS				,mmi_phb_highlight_setting);
	SetHiliteHandler(MITEM110_PBOOK_SETTINGS_NO_SIM		,mmi_phb_highlight_setting);
	SetHiliteHandler(MITEM1101_PBOOK_MEMORY_STATUS		,mmi_phb_highlight_memory_status);
	SetHiliteHandler(MITEM1102_PBOOK_STORAGE_LOCATION	,mmi_phb_highlight_storage_location);
	SetHiliteHandler(MITEM_STORAGE_LOCATION_PHONE		,mmi_phb_highlight_storage_location_in_phone);
	SetHiliteHandler(MITEM1103_PBOOK_VIEW_FIELDS			,mmi_phb_highlight_view_field);
	SetHintHandler(MITEM_STORAGE_LOCATION_PHONE		,mmi_phb_hint_storage_location_in_phone);

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MITEM_STORAGE_LOCATION_SIM + i, mmi_phb_highlight_storage_location_in_sim);
		SetHintHandler(MITEM_STORAGE_LOCATION_SIM + i, mmi_phb_hint_storage_location_in_sim);
	}
#if defined(__MMI_VCARD__)    
	/*My vCard*/
	SetHiliteHandler(MENU_ID_PHB_VCARD_MYCARD,			mmi_phb_highlight_vcard_mycard);
	SetHiliteHandler(MENU_ID_PHB_VCARD_MYCARD_NO_SIM,			mmi_phb_highlight_vcard_mycard);
	SetHiliteHandler(MENU_ID_PHB_VCARD_EDIT_MYCARD,	mmi_phb_highlight_vcard_edit_mycard);
	SetHiliteHandler(MENU_ID_PHB_VCARD_SEND_MYCARD,	mmi_phb_highlight_vcard_send_mycard);
#endif
#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
	mmi_phb_init_speak_name();
    #if defined(__PROJECT_GALLITE_C01__)
    #else
	SetHiliteHandler(MENU_ID_PHB_SPEAK_NAME				,mmi_phb_highlight_speak_name);
	SetHintHandler(MENU_ID_PHB_SPEAK_NAME				,mmi_phb_hint_speak_name);
    #endif
#endif

	/*Forward resource to phonebook entry*/
#if defined(__MMI_PHB_CALLER_RES_SETTING__)
	/*For submenu in phonebook*/
	SetHiliteHandler(MENU_ID_PHB_CALLER_PIC	,mmi_phb_highlight_caller_pic);
	SetHiliteHandler(MENU_ID_PHB_CALLER_RING	,mmi_phb_highlight_caller_ring);
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	SetHiliteHandler(MENU_ID_PHB_CALLER_VIDEO	,mmi_phb_highlight_caller_video);
#endif

	/*For submenu in other application, link to same menu*/
	SetHiliteHandler(MENU_ID_PHB_CALLER_PIC_1	,mmi_phb_highlight_caller_pic);
	SetHiliteHandler(MENU_ID_PHB_CALLER_RING_1	,mmi_phb_highlight_caller_ring);
#endif

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_setting()
* DESCRIPTION
*   Highlight handler for Settings in PHB main menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_setting(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_entry_setting,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_setting, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_setting()
* DESCRIPTION
*   Draws the Phonebook settings menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_setting(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MAX_SUB_MENUS];
	U8*	nHintList[MAX_SUB_MENUS];
	U16 nNumofItem;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_setting.>\n",__FILE__,__LINE__));
	EntryNewScreen(SCR_PBSETTINGS_LIST, mmi_phb_exit_setting, mmi_phb_entry_setting, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_PBSETTINGS_LIST);		/* Getting the Index from history  */
	if (HasValidSIM())
	{
		nNumofItem = GetNumOfChild(MITEM110_PBOOK_SETTINGS);
		GetSequenceStringIds(MITEM110_PBOOK_SETTINGS, nStrItemList);
		SetParentHandler(MITEM110_PBOOK_SETTINGS);
		ConstructHintsList(MITEM110_PBOOK_SETTINGS, nHintList);
	}
	else
	{
		nNumofItem = GetNumOfChild(MITEM110_PBOOK_SETTINGS_NO_SIM);
		GetSequenceStringIds(MITEM110_PBOOK_SETTINGS_NO_SIM, nStrItemList);
		SetParentHandler(MITEM110_PBOOK_SETTINGS_NO_SIM);
		ConstructHintsList(MITEM110_PBOOK_SETTINGS_NO_SIM, nHintList);
	}

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory52Screen(STR_SCR_PBSETTINGS_LIST_CAPTION, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  nHintList, 0, 0, guiBuffer);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_setting()
* DESCRIPTION
*   Exit function for mmi_phb_entry_setting()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_setting(void)
{
	g_phb_cntx.end_scr_id	= SCR_PBSETTINGS_LIST;
}

#define MMI_PHB_SETTING_MEMORY
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_memory_status()
* DESCRIPTION
*  Highlight handler for Memory Status in Settings menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_memory_status(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_entry_memory_status,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_memory_status, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
* mmi_phb_entry_memory_status()
* DESCRIPTION
*   Draws the memory status screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_memory_status(void)
{
	U8* guiBuffer;
	S8 memoryStatus[256];
	S8 tmpStr[16];
	S8 tmpStr_UCS2[32];

	#ifdef __MMI_MULTI_SIM__
	U8 i;
	#endif

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_memory_status.>\n",__FILE__,__LINE__));

	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		memset(memoryStatus, 0, sizeof(memoryStatus));
		
	#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			if(MTPNP_PFAL_Is_Card_Usable(i))
			{
				pfnUnicodeStrncat(memoryStatus, GetString(STRING_MTPNP_SIM1 + i), 16);
				memset(tmpStr, 0, 10);
				sprintf(tmpStr, ":%d/%d\n", g_phb_cntx.sim_used[i], g_phb_cntx.sim_total[i]);
				AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
				pfnUnicodeStrcat(memoryStatus, tmpStr_UCS2);
			}
		}
	#else
		pfnUnicodeStrncat(memoryStatus, GetString(STR_MEMORY_STATUS_SIM), 16);
		memset(tmpStr, 0, 10);
		sprintf(tmpStr, ":%d/%d\n", g_phb_cntx.sim_used[0], g_phb_cntx.sim_total[0]);
		AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
		pfnUnicodeStrcat(memoryStatus, tmpStr_UCS2);
	#endif
		
		pfnUnicodeStrncat(memoryStatus, GetString(STR_MEMORY_STATUS_PHONE), 16);
		memset(tmpStr, 0, 10);
		sprintf(tmpStr, ":%d/%d", g_phb_cntx.phone_used, g_phb_cntx.phone_total);
		AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
		pfnUnicodeStrcat(memoryStatus, tmpStr_UCS2);

		EntryNewScreen(SCR_MEMORY_STATUS, mmi_phb_exit_memory_status, mmi_phb_entry_memory_status, NULL);

		guiBuffer = GetCurrGuiBuffer (SCR_MEMORY_STATUS);
		ShowCategory7Screen(STR_SCR_MEMORY_USAGE_CAPTION,IMG_SCR_PBOOK_CAPTION, 0, 0,
										STR_GLOBAL_BACK, IMG_GLOBAL_BACK, (PU8)memoryStatus, guiBuffer);

		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	}
	else
		mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_memory_status()
* DESCRIPTION
*    Exit function of mmi_phb_entry_memory_status()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_memory_status(void)
{
	g_phb_cntx.end_scr_id = SCR_MEMORY_STATUS;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_memory_status_get_sim()
* DESCRIPTION
*   Gets the memory usage for SIM
*
* PARAMETERS
*  ucs2_status		IN/OUT	buffer for result
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_memory_status_get_sim(S8* ucs2_status, U8 nSimID)
{
	S8 ascii_status[20];

	sprintf(ascii_status, "%d/%d", g_phb_cntx.sim_used[nSimID], g_phb_cntx.sim_total[nSimID]);
	AnsiiToUnicodeString(ucs2_status, ascii_status);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_memory_status_get_phone()
* DESCRIPTION
*   Gets the memory usage for Phone
*
* PARAMETERS
*  ucs2_status		IN/OUT	buffer for result
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_memory_status_get_phone(S8* ucs2_status)
{
	S8 ascii_status[20];

	sprintf(ascii_status, "%d/%d",g_phb_cntx.phone_used, g_phb_cntx.phone_total);
	AnsiiToUnicodeString(ucs2_status, ascii_status);
}

#define MMI_PHB_SETTING_STORAGE
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_storage_location()
* DESCRIPTION
*   Highlight handler for Preferred Storage in Setting menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_storage_location(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_entry_storage_location,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_storage_location, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_storage_location()
* DESCRIPTION
*   Hint handler for Preferred Storage in Setting menu
*
* PARAMETERS
*  index IN	hint item
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_hint_storage_location(U16 index)
{
	memset((S8*)hintData[index], 0, sizeof(hintData[index]));

#ifdef  __MMI_MULTI_SIM__
	if (g_phb_cntx.prefer_storage >= MMI_SIM && g_phb_cntx.prefer_storage <= MMI_SIM4)
	{   
	#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
		pfnUnicodeStrcpy((S8*) hintData[index], GetString(STR_STORAGE_SIM));
	#else
		pfnUnicodeStrcpy((S8*) hintData[index], GetString(STRING_MTPNP_SIM1 + g_phb_cntx.prefer_storage - MMI_SIM));
	#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
	}
	else
		pfnUnicodeStrcpy((S8*) hintData[index], GetString(STR_STORAGE_PHONE));
#else	
	if (g_phb_cntx.prefer_storage == MMI_SIM)
		pfnUnicodeStrcpy((S8*) hintData[index], GetString(STR_STORAGE_SIM));
	else
		pfnUnicodeStrcpy((S8*) hintData[index], GetString(STR_STORAGE_PHONE));
#endif	
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_storage_location_in_sim()
* DESCRIPTION
*  Highlight handler for SIM in Preferred Storage menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_storage_location_in_sim(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_storage_location_set_req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_storage_location_in_phone()
* DESCRIPTION
*   Highlight handler for Phone in Preferred Storage menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_storage_location_in_phone(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_storage_location_set_req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_phb_temp_storage = MMI_NVRAM;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_storage_location_in_sim()
* DESCRIPTION
*   Hint handler for SIM in Preferred Storage menu
*
* PARAMETERS
*  index IN	hint item
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_hint_storage_location_in_sim(U16 index)
{
	S8 simStatus[40];
	U8 nSimID = 0;

	#ifdef __MMI_MULTI_SIM__
	S32 hintItemID = 0;
	
	hintItemID = GetSeqItemId_Ext((U16)MITEM1102_PBOOK_STORAGE_LOCATION, (S16)index);
	
	switch(hintItemID)
	{
		case MITEM_STORAGE_LOCATION_SIM:
			nSimID = 0;
			break;
		case MITEM_STORAGE_LOCATION_SIM2:
			nSimID = 1;
			break;
		case MITEM_STORAGE_LOCATION_SIM3:
			nSimID = 2;
			break;
		case MITEM_STORAGE_LOCATION_SIM4:
			nSimID = 2;
			break;
		default:
			nSimID = 0;
			break;
	}
	#endif

	memset((S8*)hintData[index], 0, sizeof(hintData[index]));
	mmi_phb_memory_status_get_sim(simStatus, nSimID);
	pfnUnicodeStrcpy((S8*) hintData[index], simStatus);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_storage_location_in_phone()
* DESCRIPTION
*   Hint handler for Phone in Preferred Storage menu
*
* PARAMETERS
*  index IN	hint item
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_hint_storage_location_in_phone(U16 index)
{
	S8 phoneStatus[40];

	memset((S8*)hintData[index], 0, sizeof(hintData[index]));
	mmi_phb_memory_status_get_phone(phoneStatus);
	pfnUnicodeStrcpy((S8*) hintData[index], phoneStatus);
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_storage_location()
* DESCRIPTION
*   Draws the preferred storage menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void StorageLocationHighLightHandler(S32 hiliteid)
{
	S32 hiliteItemID = 0;
	
	hiliteItemID = GetSeqItemId_Ext((U16)MITEM1102_PBOOK_STORAGE_LOCATION, (S16)hiliteid);
	
	switch(hiliteItemID)
	{
		case MITEM_STORAGE_LOCATION_SIM:
			g_phb_temp_storage = MMI_SIM;
			break;
		case MITEM_STORAGE_LOCATION_SIM2:
			g_phb_temp_storage = MMI_SIM2;
			break;
		case MITEM_STORAGE_LOCATION_SIM3:
			g_phb_temp_storage = MMI_SIM3;
			break;
		case MITEM_STORAGE_LOCATION_SIM4:
			g_phb_temp_storage = MMI_SIM4;
			break;
		default:
			g_phb_temp_storage = MMI_SIM;
			break;
	}
	
	ExecuteCurrHiliteHandler(hiliteid);
}
void mmi_phb_entry_storage_location (void)
{
	U8* guiBuffer;
	U16 nStrItemList[MMI_SIM_NUMBER + 1];
	U8* nStrList[MMI_SIM_NUMBER + 1];
	U8*	nHintList[MMI_SIM_NUMBER + 1];
	U16 nNumofItem;
	U8 hiliteStorage;
	U8 i;

	if(GetIndexOfStringId(MITEM1102_PBOOK_STORAGE_LOCATION, MITEM_STORAGE_LOCATION_SIM))
		hiliteStorage = !(g_phb_cntx.prefer_storage - 1);
	else
		hiliteStorage = g_phb_cntx.prefer_storage - 1;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_storage_location.>\n",__FILE__,__LINE__));
	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		EntryNewScreen(SCR_STORAGE_LOCATION_OPTIONS, mmi_phb_exit_storage_location, mmi_phb_entry_storage_location, NULL);
		guiBuffer = GetCurrGuiBuffer (SCR_STORAGE_LOCATION_OPTIONS);
		nNumofItem = GetNumOfChild(MITEM1102_PBOOK_STORAGE_LOCATION);
		GetSequenceStringIds(MITEM1102_PBOOK_STORAGE_LOCATION, nStrItemList);
		SetParentHandler(MITEM1102_PBOOK_STORAGE_LOCATION);
		RegisterHighlightHandler(StorageLocationHighLightHandler);
		ConstructHintsList(MITEM1102_PBOOK_STORAGE_LOCATION, nHintList);
		for(i = 0; i < MMI_SIM_NUMBER + 1; i++)
		{
			nStrList[i] = (PU8)GetString(nStrItemList[i]);
		}
		ShowCategory109Screen(STR_SCR_STORAGE_LOCATION_CAPTION, IMG_SCR_PBOOK_CAPTION,
					STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					nNumofItem, nStrList, nHintList,
					hiliteStorage, guiBuffer);

	}
	else
		mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_storage_location()
* DESCRIPTION
*   Exit function of mmi_phb_entry_storage_location()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_storage_location(void)
{
	g_phb_cntx.end_scr_id = SCR_STORAGE_LOCATION_OPTIONS;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_storage_location_set_req()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_storage_location_set_req(void)
{
	S16 pError;

	MSG_PHB_SET_PREFERRED_STORAGE_REQ_STRUCT *myMsgPtr;
	MYQUEUE	Message;
	U16 tempStorageLocation;	/*Temp Solution For Write 2 bytes NVRAM_PHB_STORAGE_LOCATION*/
	module_type	dest_mod_id = MOD_L4C;
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_storage_location_set_req.>\n",__FILE__,__LINE__));

	if(g_phb_cntx.prefer_storage != g_phb_temp_storage)
	{
		myMsgPtr = (MSG_PHB_SET_PREFERRED_STORAGE_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_PREFERRED_STORAGE_REQ_STRUCT));

		g_phb_cntx.prefer_storage = g_phb_temp_storage;

		myMsgPtr->storage = g_phb_cntx.prefer_storage;
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=dest_mod_id;
		Message.oslMsgId = PRT_PHB_SET_PREFERRED_STORAGE_REQ;
		Message.oslDataPtr = (oslParaType *)myMsgPtr;
		Message.oslPeerBuffPtr= NULL;
		OslMsgSendExtQueue(&Message);

		tempStorageLocation = g_phb_cntx.prefer_storage;
		WriteValue(NVRAM_PHB_STORAGE_LOCATION, &tempStorageLocation, DS_SHORT, &pError);	/*2 bytes for DS_SHORT*/
	}
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	DeleteNScrId(SCR_STORAGE_LOCATION_OPTIONS);
}


#define MMI_PHB_SETTING_FIELDS
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_view_field()
* DESCRIPTION
*   Highlight handler for "Fields" in Settings menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_view_field(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_entry_view_field,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_view_field, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_view_field_get_string
* DESCRIPTION
*
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
U8* mmi_phb_view_field_get_string(U8 index)
{
	U16 str_id = STR_GLOBAL_ERROR;

	switch(index)
	{
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
		case MMI_PHB_FIELD_HOME:
			str_id = STR_HOME_NUMBER;
 			break;
	 	case MMI_PHB_FIELD_COMPANY_NAME:
	 		str_id = STR_COMPNAY_NAME;
 			break;
	 	case MMI_PHB_FIELD_EMAIL:
	 		str_id = STR_EMAIL_ADRESS;
 			break;
	 	case MMI_PHB_FIELD_OFFICE:
	 		str_id = STR_OFFICE_NUMBER;
 			break;
	 	case MMI_PHB_FIELD_FAX:
	 		str_id = STR_FAX_NUMBER;
 			break;
#endif
	 	case MMI_PHB_FIELD_PIC:
	 		str_id = STR_ASSOCIATE_PICTURE;
			break;
	 	case MMI_PHB_FIELD_RING:
	 		str_id = STR_ASSOCIATE_SOUND;
			break;
	 	case MMI_PHB_FIELD_GROUP:
	 		str_id = STR_CALLER_GROUP;
			break;
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	 	case MMI_PHB_FIELD_VIDEO:
	 		str_id = STR_ID_PHB_CALLER_VIDEO;
			break;
#endif
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
	 	case MMI_PHB_FIELD_BIRTHDAY:
	 		str_id = STR_ID_PHB_BIRTHDAY;
			break;
#endif
		default:
			break;
 	}

	return (U8*)GetString(str_id);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_view_field()
* DESCRIPTION
*   Draws the Phonebook view fields screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_view_field(void)
{
	U8* guiBuffer;
	U8* viewFields[MAX_PB_VIEW_FIELDS];
	U8	i, count = 0;

	/*Assign string for each view field*/
	for(i = (MMI_PHB_FIELD_NUMBER + 1); i < MMI_PHB_FIELD_TOTAL; i++)
	{
		viewFields[count++] = mmi_phb_view_field_get_string(i);
	}

	/*Read out setting from NVRAM*/
	if(!g_phb_cntx.nvram_data_init)
	{
		mmi_phb_init_get_data_from_nvram();
		g_phb_cntx.nvram_data_init = TRUE;
	}

	memcpy(g_phb_temp_field_state, g_phb_cntx.view_field, MAX_PB_VIEW_FIELDS);

	EntryNewScreen(SCR_VIEW_FIELDS, mmi_phb_exit_view_field, mmi_phb_entry_view_field, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_VIEW_FIELDS);

	SetCheckboxToggleRightSoftkeyFunctions(mmi_phb_entry_view_field_save_confirm, GoBackHistory);

	ShowCategory140Screen(STR_SCR_VIEW_FIELDS_CAPTION,
									IMG_SCR_PBOOK_CAPTION,
									count ,
									viewFields,
									g_phb_temp_field_state,
									0,
									guiBuffer);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_view_field()
* DESCRIPTION
*   Exit function for mmi_phb_entry_view_field()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_view_field(void)
{
	g_phb_cntx.end_scr_id = SCR_VIEW_FIELDS;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_view_field_save_confirm()
* DESCRIPTION
*  Confirms before saving the chnages in view  fields settings
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_view_field_save_confirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(mmi_phb_view_field_save,KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_phb_go_back_2_history,KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_view_field_save()
* DESCRIPTION
*   Saves the change in view fields setting
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_view_field_save(void)
{
	S16 pError;
	U8 viewField[NVRAM_PHB_SETTINGS_SIZE];

	memcpy(g_phb_cntx.view_field, g_phb_temp_field_state, MAX_PB_VIEW_FIELDS);
	memset(viewField,1,NVRAM_PHB_SETTINGS_SIZE);
	memcpy(viewField, g_phb_cntx.view_field, MAX_PB_VIEW_FIELDS);
	WriteRecord(NVRAM_EF_PHB_SETTINGS_LID, 1, viewField, NVRAM_PHB_SETTINGS_SIZE, &pError);

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	DeleteNScrId(SCR_VIEW_FIELDS);
}

/*-----------------------------------------------------Begin of vCard -------------------------------------------------------*/
#if defined(__MMI_VCARD__)
#define MMI_PHB_SETTING_VCARD
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_vcard_send_select_card()
* DESCRIPTION
*   Highlight function of vcard option -> Send through SMS
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_vcard_send_select_card(void)
{
#ifndef __MMI_NOKIA_STYLE_N800__
#if defined(__MMI_VCARD__) && defined(MMI_ON_HARDWARE_P)
	SetLeftSoftkeyFunction(mmi_vcard_forward_option_select_card,KEY_EVENT_UP);
	SetKeyHandler(mmi_vcard_forward_option_select_card, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#else
#ifndef FLASH_OPT
#if defined(__MMI_VCARD__) && defined(MMI_ON_HARDWARE_P)
	SetLeftSoftkeyFunction(mmi_vcard_forward_option_select_card,KEY_EVENT_UP);
	SetKeyHandler(mmi_vcard_forward_option_select_card, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#endif	
#endif
#endif
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_vcard_mycard()
* DESCRIPTION
*   Highlight function of vcard option -> Send through SMS
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_vcard_mycard(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_entry_mycard,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_entry_mycard, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_vcard_edit_mycard()
* DESCRIPTION
*   Highlight function of vcard option -> Send through SMS
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_vcard_edit_mycard(void)
{
#if defined(__MMI_VCARD__) && defined(MMI_ON_HARDWARE_P)
	SetLeftSoftkeyFunction(mmi_phb_entry_edit_mycard,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_edit_mycard, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(mmi_phb_entry_edit_mycard,KEY_EVENT_UP);
	SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#endif

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_vcard_send_mycard()
* DESCRIPTION
*   Highlight function of vcard option -> Send through SMS
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_vcard_send_mycard(void)
{
mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
#if defined(__MMI_VCARD__) && defined(MMI_ON_HARDWARE_P)
	SetLeftSoftkeyFunction(mmi_vcard_forward_option_my_card,KEY_EVENT_UP);
	SetKeyHandler(mmi_vcard_forward_option_my_card, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(NULL,KEY_EVENT_UP);
	SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#endif

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_mycard()
* DESCRIPTION
*    Inline Editor Screen of My vCard for editing.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_mycard(void)
{
	U8* guiBuffer;
	U16 nStrItemList[3];
	U16 nNumofItem;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_mycard.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_ID_PHB_VCARD_MYCARD, mmi_phb_exit_mycard, mmi_phb_entry_mycard, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_VCARD_MYCARD);
	if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
  	)
	{
		nNumofItem = GetNumOfChild(MENU_ID_PHB_VCARD_MYCARD);
		GetSequenceStringIds(MENU_ID_PHB_VCARD_MYCARD, nStrItemList);
		SetParentHandler(MENU_ID_PHB_VCARD_MYCARD);
	}
	else
	{
		nNumofItem = GetNumOfChild(MENU_ID_PHB_VCARD_MYCARD_NO_SIM);
		GetSequenceStringIds(MENU_ID_PHB_VCARD_MYCARD_NO_SIM, nStrItemList);
		SetParentHandler(MENU_ID_PHB_VCARD_MYCARD_NO_SIM);
	}
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(STR_ID_PHB_VCARD, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_mycard()
* DESCRIPTION
*   Inline Editor Screen of My vCard for editing.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_mycard(void)
{
	g_phb_cntx.end_scr_id = SCR_ID_PHB_VCARD_MYCARD;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_edit_mycard()
* DESCRIPTION
*   Inline Editor Screen of My vCard for editing.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_edit_mycard(void)
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
	S16 pError;
	U8 item_index = 0;
	U16 DetailPbNameImageList[MAX_PB_FIELDS];
	MMI_PHB_VCARD_STRUCT myCard;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_edit_mycard.>\n",__FILE__,__LINE__));
	EntryNewScreen(SCR_ID_PHB_VCARD_EDIT_MYCARD, mmi_phb_exit_edit_mycard, NULL, NULL);
	InitializeCategory57Screen();
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_VCARD_EDIT_MYCARD);
	inputBuffer = GetCurrNInputBuffer(SCR_ID_PHB_VCARD_EDIT_MYCARD, &inputBufferSize);

	gPictureItemIndex = 0xff;	/*Set to default before use.*/
	gRingtoneItemIndex = 0xff;
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	g_phb_video_item_index = 0xff;
#endif
#if defined(__MMI_PHB_IMPS_FIELD__)
	phb_imps_item_index = 0xff;
#endif

	RegisterHighlightHandler(mmi_phb_op_edit_highlight_hdlr);

	ReadRecord(NVRAM_EF_PHB_VCARD_LID, 1, (void*)&myCard, MMI_PHB_VCARD_RECORD_SIZE, &pError);

	pfnUnicodeStrcpy(pbName, myCard.name);
	AnsiiToUnicodeString(pbNumber, myCard.number);
	AnsiiToUnicodeString(pbHomeNumber, myCard.homeNumber);
	pfnUnicodeStrcpy(pbCompanyName, myCard.companyName);
	AnsiiToUnicodeString(pbEmailAddress, myCard.emailAddress);
	AnsiiToUnicodeString(pbOfficeNumber, myCard.officeNumber);
	AnsiiToUnicodeString(pbFaxNumber, myCard.faxNumber);

	mmi_phb_op_fill_inline_item(MMI_PHB_FIELD_NAME, item_index++, MAX_PB_PHONE_ENTRIES-1, TitleStrings, DetailPbNameImageList);
	mmi_phb_op_fill_inline_item(MMI_PHB_FIELD_NUMBER, item_index++, MAX_PB_PHONE_ENTRIES-1, TitleStrings, DetailPbNameImageList);

#ifndef __MMI_PHB_NO_OPTIONAL_FIELD__
	/*Fill inline item for each option field*/
	for(item_index = (MMI_PHB_FIELD_HOME); item_index <= MMI_PHB_FIELD_FAX; item_index++)
	{
		mmi_phb_op_fill_inline_item(item_index, item_index, MAX_PB_PHONE_ENTRIES-1, TitleStrings, DetailPbNameImageList);
	}
#endif

#if 1	/*Disable image and sound in my card first, will enable when can send image and sound through IrDa*/
	if(inputBuffer!=NULL)
		SetCategory57Data(wgui_inline_items, item_index, inputBuffer);
#else
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
#endif

	ShowCategory57Screen(STR_SCR_INLINE_EDIT_CAPTION, IMG_STORAGE_HANDSET,
				  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				  item_index, DetailPbNameImageList, wgui_inline_items, 0, guiBuffer);

	SetCategory57RightSoftkeyFunctions(mmi_phb_entry_edit_mycard_confirm_save,GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_edit_mycard()
* DESCRIPTION
*   Exit Screen for edit my vcard.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_edit_mycard(void)
{
	history_t	h;
	U16		inputBufferSize;

	g_phb_cntx.end_scr_id = SCR_ID_PHB_VCARD_EDIT_MYCARD;
	gPictureItemIndex = 10;
	mmi_phb_stop_ring_tone_id();
	CloseCategory57Screen();

	h.scrnID = SCR_ID_PHB_VCARD_EDIT_MYCARD;
	h.entryFuncPtr = mmi_phb_entry_edit_mycard;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data((U8*) h.inputBuffer);
	AddNHistory(h, inputBufferSize);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_vcard_pre_save_mycard()
* DESCRIPTION
*   check vcard fields
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static void mmi_phb_vcard_pre_save_mycard(void)
{
	if(!mmi_phb_op_check_pre_save())
	{
		return;
	}
	mmi_phb_vcard_save_mycard();	
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_edit_mycard_confirm_save()
* DESCRIPTION
*   save confirmation for edit my vcard
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_edit_mycard_confirm_save(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO,
			get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, WARNING_TONE);


	SetLeftSoftkeyFunction(mmi_phb_vcard_pre_save_mycard, KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_phb_go_back_2_history, KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_vcard_save_mycard()
* DESCRIPTION
*   save my vcard to flash
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_vcard_save_mycard(void)
{
	MMI_PHB_VCARD_STRUCT myCard;
	S16 pError;

	pfnUnicodeStrcpy(myCard.name, pbName);
	UnicodeToAnsii(myCard.number, pbNumber);
	UnicodeToAnsii(myCard.homeNumber, pbHomeNumber);
	pfnUnicodeStrcpy(myCard.companyName, pbCompanyName);
	UnicodeToAnsii(myCard.emailAddress, pbEmailAddress);
	UnicodeToAnsii(myCard.officeNumber, pbOfficeNumber);
	UnicodeToAnsii(myCard.faxNumber, pbFaxNumber);
	myCard.pictureTagID =  g_phb_cntx.image_id_list[g_phb_cntx.selected_pic_index];
	myCard.ringToneID = g_phb_cntx.ring_tone_list[g_phb_cntx.selected_ring_index];

	WriteRecord(NVRAM_EF_PHB_VCARD_LID, 1, (void*)&myCard, MMI_PHB_VCARD_RECORD_SIZE, &pError);

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	DeleteNScrId(SCR_ID_PHB_VCARD_EDIT_MYCARD);
}
#endif
/*-----------------------------------------------------END of vCard-------------------------------------------------------*/

#define MMI_PHB_SETTING_SPEAK_NAME
#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
/*****************************************************************************
* FUNCTION
*  mmi_phb_init_speak_name()
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_init_speak_name(void)
{
	S16 pError;

	ReadValue(NVRAM_PHB_SPEAK_NAME_ON_OFF, &g_phb_cntx.speak_name, DS_BYTE, &pError);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_speak_name()
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_speak_name(void)
{

	if(g_phb_cntx.speak_name)
		ChangeLeftSoftkey(STR_GLOBAL_OFF, IMG_GLOBAL_OK);
	else
		ChangeLeftSoftkey(STR_GLOBAL_ON, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(mmi_phb_speak_name_toggle,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_speak_name()
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_hint_speak_name(U16 index)
{
	memset((S8*)hintData[index], 0, sizeof(hintData[index]));
	if (g_phb_cntx.speak_name)
		pfnUnicodeStrcpy((S8*) hintData[index], GetString(STR_GLOBAL_ON));
	else
		pfnUnicodeStrcpy((S8*) hintData[index], GetString(STR_GLOBAL_OFF));
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_speak_name_switch()
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_speak_name_toggle(void)
{
	S16 pError;

	if (g_phb_cntx.speak_name)
		g_phb_cntx.speak_name = 0;
	else
		g_phb_cntx.speak_name = 1;

	WriteValue(NVRAM_PHB_SPEAK_NAME_ON_OFF, &g_phb_cntx.speak_name, DS_BYTE, &pError);

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
}

#endif

#define MMI_PHB_CALLER_RES_SETTING
#if defined(__MMI_PHB_CALLER_RES_SETTING__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_caller_pic
* DESCRIPTION
*   highlight function for caller picture setting
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_caller_pic(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	SetLeftSoftkeyFunction(mmi_phb_caller_pic_select_from_file_mamager,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_caller_pic_select_from_file_mamager,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

// dingjian 20090310
void mmi_phb_highlight_import_and_export(void)
{
    mmi_phb_highlight_caller_pic();
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_caller_pic_select_from_file_mamager
* DESCRIPTION
*   select file from file manager function. The initial path is the same as camera folder
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_caller_pic_select_from_file_mamager(void)
{
	FMGR_FILTER filter;
	S8	buf_filepath[FMGR_PATH_BUFFER_SIZE];

/*+ zhouqin modify for dual t-flash card 20110617*/
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
/*- zhouqin modify for dual t-flash card 20110617*/
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	/*Use camera folder as default folder to select image*/
#if defined(__MMI_CAMERA__)
	mmi_camera_get_storage_file_path(buf_filepath);
#else
	pfnUnicodeStrcpy(buf_filepath, (S8*)L"root");
#endif

	FMGR_FILTER_INIT(&filter);
#if defined(__GDI_MEMORY_PROFILE_2__)	/*Able to select jpeg file if supported*/
	FMGR_FILTER_SET(&filter, FMGR_TYPE_JPG);
#endif
	FMGR_FILTER_SET(&filter, FMGR_TYPE_GIF);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_BMP);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);

	mmi_fmgr_select_path_and_enter(APP_PHONEBOOK,
								FMGR_SELECT_FILE|FMGR_SELECT_REPEAT,
								filter,
								/*(S8*)buf_filepath*/(S8*)L"root",//chenhe to select for both phone and card,20081229,fix bug 11065 
								(PsExtFuncPtr)mmi_phb_caller_pic_select_from_file_mamager_done);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_caller_pic_select_from_file_mamager_done
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  path			IN:	selected file path
*  is_short		IN: is selected file name short name
*
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_caller_pic_select_from_file_mamager_done(void *path, int is_short)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if(path == NULL)
	{
		GoBackHistory();
		return;
	}
#if defined(__MMI_FILE_MANAGER__)   // zrx add 20060612
	/*Check image content*/

	if(mmi_phb_image_check_valid_image(path))
	{
		pfnUnicodeStrcpy(g_phb_file_path, path);

		fmgr_reset_app_select();

		g_phb_cntx.recv_res_from = MMI_PHB_RECV_RES_IMAGE;
		mmi_phb_entry_fmgr_save_option();
	}
#endif   // zrx add 20060612
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_caller_ring
* DESCRIPTION
*   Highlight function for caller ring tone settngs
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_caller_ring(void)
{
	SetLeftSoftkeyFunction(mmi_phb_caller_ring_select_profile,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_caller_ring_select_profile, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_caller_ring_select_profile
* DESCRIPTION
*   Select ring tone from profile list.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern int checkUsbstate(void);
#if defined(MMI_ON_HARDWARE_P)
extern VOID mmi_display_usb_busy(void);
#endif
void mmi_phb_caller_ring_select_profile(void)
{
#if 1 // zrx del 20060612
	//mmi_profiles_entry_ring_tone_list(mmi_phb_caller_ring_select_profile,
	//											  mmi_phb_caller_ring_select_profile_done);
	U8* guiBuffer;       /* Buffer holding history data */
	//U16 nDispAttribute;  /* Stores display attribue */
	U16 selecteditem =0;
	U8 totalItems=0;

#if	defined(MMI_ON_HARDWARE_P)
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
#endif	
	PrfSetRingToneConfType(PRFSET_MELODY_TYPE_PHB_RING);
	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_RING_TONE_SETUP, mmi_profiles_exit_ring_tone_list,
		mmi_phb_caller_ring_select_profile, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_RING_TONE_SETUP);

	/* 4. Get attribute of menu to be displayed */
	//nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_RING_TONE);

	/* 3. Retrieve no of child of menu item to be displayed */
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	totalItems = mmi_profiles_get_melody_list( GetRingToneID(), &selecteditem);

	/* 6 Set current parent id*/
	SetParentHandler(SCR_PBOOK_MAIN_MENU);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(mmi_profiles_melody_list_highlight_hdlr);

	/* 8 Display Category Screen */
	ShowCategory89Screen(STR_ID_PHB_CALLER_RING,IMG_SCR_PBOOK_CAPTION,
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 totalItems,subMenuDataPtrs, hintDataPtrs, 0,selecteditem, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(StopRingToneBack,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_phb_caller_ring_select_profile_done,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	#endif

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_caller_ring_select_profile_done
* DESCRIPTION
*   select a ring tone id from profile done.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_caller_ring_select_profile_done(void)
{
	EntryScrSetRingToneIdCountId(mmi_phb_caller_ring_get_id_callback);  // zrx del 20060612
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_caller_ring_get_id_callback
* DESCRIPTION
*   Get the ring tone id from profile
*
* PARAMETERS
*  ExtMelodyFlag	IN	: Is it a external resource
*  ringToneId		IN	: Selected ring tone id.
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_caller_ring_get_id_callback(U8 ExtMelodyFlag, U16 ringToneId)
{

#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	if( ExtMelodyFlag == MMI_TRUE )
	{
		PrfSetRingToneConfType(PRFSET_MELODY_TYPE_PHB_RING);
		EntryPrfSetRingToneExtMelody();
		return;
	}
#endif

	mmi_phb_caller_ring_forward_to_entry(ringToneId);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_caller_ring_forward_to_entry
* DESCRIPTION
*   Forward a ring tone resource to phonebook entry.
*
* PARAMETERS
*  ringToneId	IN	: ring tone ID associate to phonebook entry
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_caller_ring_forward_to_entry(U16 ringToneId)
{
	g_phb_cntx.recv_res_from = MMI_PHB_RECV_RES_AUDIO;
	g_phb_cntx.recv_res_id = ringToneId;
	mmi_phb_entry_fmgr_save_option();  // zrx del 20060612

	DeleteScreenIfPresent(SCR_PROFILES_RING_TONE_SETUP);
	DeleteScreenIfPresent(MENU_TONE_SETUP_RING_TONE_EXT_MELODY);
}

#if defined(__MMI_INCOMING_CALL_VIDEO__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_caller_video
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_caller_video(void)
{
	SetLeftSoftkeyFunction(mmi_phb_caller_video_select_from_file_mamager,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_caller_video_select_from_file_mamager,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_caller_video_select_from_file_mamager
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_caller_video_select_from_file_mamager(void)
{
	FMGR_FILTER filter;
	S8	buf_filepath[FMGR_PATH_BUFFER_SIZE];

    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	/*Use camera folder as default folder to select image*/
	mmi_vdorec_get_storage_file_path(buf_filepath);

	FMGR_FILTER_INIT(&filter);
	FMGR_FILTER_SET_VIDEO(&filter);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);

	mmi_fmgr_select_path_and_enter(APP_PHONEBOOK,
								FMGR_SELECT_FILE|FMGR_SELECT_REPEAT,
								filter,
								(S8*)buf_filepath,
								(PsExtFuncPtr)mmi_phb_caller_video_select_from_file_mamager_done);
}


/*****************************************************************************
* FUNCTION
*  mmi_phb_caller_video_select_from_file_mamager_done
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_caller_video_select_from_file_mamager_done(void *path, int is_short)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if(path == NULL)
	{
		GoBackHistory();
		return;
	}
	if (!mmi_phb_video_check_data_space((S8*)path))
	{
		return;
	}
	pfnUnicodeStrcpy(g_phb_video_file_path, path);
	fmgr_reset_app_select();

	g_phb_cntx.recv_res_from = MMI_PHB_RECV_RES_VIDEO;
	mmi_phb_entry_fmgr_save_option();
}

#endif /*#if defined(__MMI_INCOMING_CALL_VIDEO__)*/

#endif /*#if defined(__MMI_PHB_CALLER_RES_SETTING__)*/

#endif


