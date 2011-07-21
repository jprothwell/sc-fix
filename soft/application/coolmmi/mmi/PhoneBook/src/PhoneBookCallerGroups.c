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
 * PhoneBookCallerGroups.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Implemantation of "Caller Group" feature in the Phonebook Application
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
#ifndef _PHONEBOOKCALLERGROUP_C
#define _PHONEBOOKCALLERGROUP_C

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebooktypes.h"
#include "queuegprot.h"
#include "mainmenudef.h"
#include "unicodexdcl.h"
#include "globaldefs.h"
#include "timerevents.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "profilegprots.h"
#include "gpioinc.h"
#include "downloaddefs.h"
#include "downloadprotos.h"
#include "commonscreens.h"
#include "profilesenum.h"	
#include "callmanagementgprot.h"

#include "gbhmi.h"//added by zhoumn for the bug of editor full @2007/06/16
#include "audioplayerdef.h"  //added by jiashuo for jasper:20080314
#include "audioplayertype.h" //added by jiashuo for jasper:20080314

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
#include "filemgr.h"
#include "profilemgrgprot.h"
#include "conversions.h"
#endif

#if defined(__MMI_MESSAGES_SEND_BY_GROUP__)
#include "messagesexdcl.h"
#include "smsguiinterfaceprot.h"
#endif
#undef __NEWSIMULATOR

#include "mmi_trace.h"
#include "todolistenum.h"
#include "callsetupenum.h"
#include "todolistenum.h"
/*
** Local Variable
*/
static U16 g_phb_last_ring_tone_id = 0;
static U16 g_phb_curr_ring_tone_id = 0;
static U8 g_phb_last_pattern_id = 0;
static U8 g_phb_curr_pattern_id = 0;

#if defined(__MMI_PHB_CALLERGROUP_MEMBER_LIST__)	/*Caller Group Member List*/
static U16 g_phb_callergroup_member_count;
static U16* g_phb_callergroup_ptr = NULL;
static U8 g_phb_callergroup_member_index = 10;
#endif

#if defined(__MMI_INCOMING_CALL_VIDEO__)
extern U8 g_phb_video_item_index;
extern BOOL g_phb_video_view;
#endif

/*
** Global Variable
*/
extern InlineItem wgui_inline_items[];
extern pBOOL viewOnce;
extern MMI_PHB_LIST_VIEW phbListView;
extern U8 gPictureItemIndex;
#if defined(__MMI_PHB_CALLERGROUP_MEMBER_LIST__)	/*Caller Group Member List*/
extern U16 PhoneBookEntryCount;
extern MMI_PHB_ENTRY_BCD_STRUCT	PhoneBook[];
#endif
extern U16 g_phb_name_index[];
extern S8 pbName[];
extern U8* myCallerGrpnames[];
extern audply_struct g_audply;  //Added by jiashuo for jasper:20080314
UINT8 phbBRlist = 0; //0 is none,1 is black list,2 is red list

/*
** Global Function
*/
extern void GBSetInputboxLSKFunction(FuncPtr f); 

#define MMI_PHB_CALLERGROUP
/*****************************************************************************
* FUNCTION
*  mmi_phb_caller_group_init()
* DESCRIPTION
*   Initialization of "Caller Group" feature
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_caller_group_init(void)
{
  	S16 pError;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_caller_group_init.>\n",__FILE__,__LINE__));
	SetHiliteHandler(MITEM109_PBOOK_CALLER_GROUPS	,mmi_phb_highlight_callergroup);

#if defined (__MMI_PHB_CALLERGROUP_MEMBER_LIST__)
	SetHiliteHandler(MENU_ID_PHB_CALLERGROUP_MEMBER_ADD_MEMBER	,mmi_phb_highlight_callergroup_member_add);
	SetHiliteHandler(MENU_ID_PHB_CALLERGROUP_MEMBER_DELETE_MEMBER		,mmi_phb_highlight_callergroup_member_delete);
#endif
	ReadValue(NVRAM_EF_PHB_BLACK_AND_RED_LIST, (void*)&phbBRlist, DS_BYTE, &pError); 
	if(phbBRlist == 0xff)
	{
		phbBRlist = 0;
		WriteValue(NVRAM_EF_PHB_BLACK_AND_RED_LIST, (void*)&phbBRlist, DS_BYTE, &pError);		
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_restore_default()
* DESCRIPTION
*   Sets Caller Groups default values. This is an external API called by restore defaults application
*
* PARAMETERS
*  void
* RETURNS
*  BOOL: TRUE if restore successfully, else FALSE
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
BOOL mmi_phb_callergroup_restore_default(void)
{
	S16 pError;
	U8 i, midiToneStartId, midiToneTotalId;
	PHB_CALLER_GROUP_STRUCT* callerGroups;

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;

	GetMidiRange(&midiToneStartId, &midiToneTotalId);

	for (i = 0; i < MAX_PB_CALLER_GROUPS; ++i)
	{

#if defined(__MMI_CALLERGROUP_NO_ALERT__)
		callerGroups[i].alertType = MMI_ALERT_NONE;
#else
		callerGroups[i].alertType = MMI_RING;
#endif

		callerGroups[i].ringToneID = midiToneStartId;
		callerGroups[i].pictureTagID = IMG_PHB_DEFAULT;

#if defined(__MMI_STATUS_LED__)
		callerGroups[i].LEDPatternId = 0; /*pattern 1*/
#else
		callerGroups[i].LEDPatternId = 0; /*no pattern*/
#endif
		callerGroups[i].is_modified = 0; /* Restore to default name*/

		/*Restore Name according to language*/
		pfnUnicodeStrncpy((PS8)callerGroups[i].groupName, GetString((U16)(STR_CALLER_GROUP_1 + i)), MAX_GRP_NAME_LENGTH);
	}

	WriteRecord(NVRAM_EF_PHB_CALLER_GROUPS_LID, 1, callerGroups, CALLER_GROUPS_RECORD_SIZE, &pError);

	if(pError == NVRAM_WRITE_SUCCESS)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_set_name_by_language()
* DESCRIPTION
*   Sets Caller Groups name values according to current language.
*    This is an external API called by restore defaults application
*
* PARAMETERS
*  void
* RETURNS
*  BOOL: TRUE if restore successfully, else FALSE
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_callergroup_set_name_by_language(void)
{
	S16 pError;
	U8 i;
	PHB_CALLER_GROUP_STRUCT callerGroups[MAX_PB_CALLER_GROUPS] ;//= {0};//Renwy add "= {0}" 2008/06/28

	memset(callerGroups, 0, MAX_PB_CALLER_GROUPS*sizeof(PHB_CALLER_GROUP_STRUCT));
	ReadRecord(NVRAM_EF_PHB_CALLER_GROUPS_LID, 1, callerGroups, CALLER_GROUPS_RECORD_SIZE, &pError);

	if(pError == NVRAM_READ_SUCCESS)
	{

		for (i = 0; i < MAX_PB_CALLER_GROUPS; ++i)
			if(callerGroups[i].is_modified == 0) /*User not modify yet*/
				pfnUnicodeStrncpy((PS8)callerGroups[i].groupName, GetString((U16)(STR_CALLER_GROUP_1+i)), MAX_GRP_NAME_LENGTH);

		WriteRecord(NVRAM_EF_PHB_CALLER_GROUPS_LID, 1, callerGroups, CALLER_GROUPS_RECORD_SIZE, &pError);

		if((pError == NVRAM_WRITE_SUCCESS) && (g_phb_cntx.caller_group != NULL) && g_phb_cntx.nvram_data_init)
			memcpy(g_phb_cntx.caller_group,callerGroups,CALLER_GROUPS_RECORD_SIZE);

	}
}
void mmi_phb_init_callergroup_name(void)
{
  	S16 pError;
  	U8 i;
  	//INT32 error; 
	PHB_CALLER_GROUP_STRUCT* callerGroups;

	mmi_trace(g_sw_PHB, "PHB fucn:%s ", __FUNCTION__);

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;
	for (i = 0; i < MAX_PB_CALLER_GROUPS; ++i)
	{
		if(callerGroups[i].is_modified == 0) 
		{
		pfnUnicodeStrncpy((PS8)callerGroups[i].groupName, GetString((U16)(STR_CALLER_GROUP_1 + i)), MAX_GRP_NAME_LENGTH);
		}
	}
	WriteRecord(NVRAM_EF_PHB_CALLER_GROUPS_LID, 1, callerGroups, CALLER_GROUPS_RECORD_SIZE, &pError);
}


/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_callergroup()
* DESCRIPTION
*   Highlight handler for Caller Group in PHB main menu
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_callergroup(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	phbListView = MMI_PHB_LIST_FOR_PHB;
	SetLeftSoftkeyFunction(mmi_phb_entry_callergroup,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_callergroup, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_callergroup()
* DESCRIPTION
*   Displays the list of available Caller Groups
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_callergroup(void)
{
	U8 *grpNames[MAX_PB_CALLER_GROUPS];
	U16 i;
	U8* guiBuffer;
	PHB_CALLER_GROUP_STRUCT* callerGroups;

	mmi_trace(g_sw_ADP_PHB, TSTXT("mmi_phb_entry_callergroup.  \n"));
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_callergroup.>\n",__FILE__,__LINE__));

	if(!g_phb_cntx.nvram_data_init)
	{
		mmi_phb_init_get_data_from_nvram();
		g_phb_cntx.nvram_data_init = TRUE;
	}

	EntryNewScreen(SCR_CALLER_GROUPS, mmi_phb_exit_callergroup, mmi_phb_entry_callergroup, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_CALLER_GROUPS);		/* Getting the Index from history */

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;
	for(i = 0; i < MAX_PB_CALLER_GROUPS; i++)
		grpNames[i] = callerGroups[i].groupName;

	if(phbListView == MMI_PHB_LIST_FOR_SMS)
	{
#if defined(__MMI_MESSAGES_SEND_BY_GROUP__)
		RegisterHighlightHandler(mmi_phb_get_index_second_level);
		SetMessagesCurrScrnID(SCR_CALLER_GROUPS);
		ShowCategory53Screen(STR_SCR_CALLER_GROUP_CAPTION, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_PB_CALLER_GROUPS, grpNames, (U16*)gIndexIconsImageList, NULL, 0, 0, guiBuffer);

		SetLeftSoftkeyFunction(mmi_phb_sms_send_data_for_group,KEY_EVENT_UP);
#endif
	}
	else
	{
		RegisterHighlightHandler(mmi_phb_get_index);
		ShowCategory53Screen(STR_SCR_CALLER_GROUP_CAPTION, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_PB_CALLER_GROUPS, grpNames, (U16*)gIndexIconsImageList, NULL, 0, 0, guiBuffer);
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
		if(phbListView == MMI_PHB_LIST_FOR_SAVE_FROM_FILE_MGR)
			SetLeftSoftkeyFunction(mmi_phb_fmgr_pre_edit_callergrp,KEY_EVENT_UP);
		else
#endif
		{
			SetLeftSoftkeyFunction(mmi_phb_entry_callergroup_detail,KEY_EVENT_UP);
			SetKeyHandler(mmi_phb_entry_callergroup_detail, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		}

	}

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_callergroup()
* DESCRIPTION
*   Exit function of mmi_phb_entry_callergroup()
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_callergroup(void)
{
	g_phb_cntx.end_scr_id = SCR_CALLER_GROUPS;
#if defined( __MMI_MESSAGES_SEND_BY_GROUP__)
	mmi_msg_exit_generic();
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_callergroup_detail()
* DESCRIPTION
*   Displayes the attributes of the selected Caller Group and also allows to edit it
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_callergroup_detail(void)
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
	U8 j;
	U16 CGDetailsImageList[MAX_CALLER_GRP_ITEMS];
	PHB_CALLER_GROUP_STRUCT* callerGroups;

    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_callergroup_detail.>\n",__FILE__,__LINE__));

 
	mmi_phb_build_led_and_alert_list();
	mmi_phb_build_image_list();
	mmi_phb_build_ring_tone_list();

#if defined(__MMI_INCOMING_CALL_VIDEO__)
	mmi_phb_build_video_list();
#endif
    mmi_phb_build_black_list_list();
    mmi_phb_build_red_list_list();

	EntryNewScreen(SCR_CALLER_GRP_DETAILS, mmi_phb_exit_callergroup_detail, NULL, NULL);
	InitializeCategory57Screen();

	guiBuffer = GetCurrGuiBuffer(SCR_CALLER_GRP_DETAILS);
	inputBuffer = GetCurrNInputBuffer(SCR_CALLER_GRP_DETAILS, &inputBufferSize);

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;
	pfnUnicodeStrcpy(pbName, (PS8)callerGroups[g_phb_cntx.active_index].groupName);
	g_phb_cntx.selected_alert_index = callerGroups[g_phb_cntx.active_index].alertType;
	g_phb_cntx.selected_ring_index =  mmi_phb_get_ring_tone_index(callerGroups[g_phb_cntx.active_index].ringToneID);
	g_phb_cntx.selected_pattern_index = callerGroups[g_phb_cntx.active_index].LEDPatternId;
	g_phb_cntx.selected_pic_index = mmi_phb_get_image_index(callerGroups[g_phb_cntx.active_index].pictureTagID);
    g_phb_cntx.selected_black_list_index = mmi_phb_get_black_list_index(callerGroups[g_phb_cntx.active_index].black_list_flag_Id);
    g_phb_cntx.selected_red_list_index = mmi_phb_get_red_list_index(callerGroups[g_phb_cntx.active_index].red_list_flag_Id);

#if defined(__MMI_INCOMING_CALL_VIDEO__)
	g_phb_cntx.selected_video_index = mmi_phb_get_video_index(callerGroups[g_phb_cntx.active_index].VideoID);

	if(guiBuffer == NULL ) /*First Time Enter and Not Enter From File Manager*/
	{
		U16 video_id = callerGroups[g_phb_cntx.active_index].VideoID;

		g_phb_cntx.video_audio = ((video_id == 0) || (video_id & 0x4000)) ? 1:0;

		/*Check if can load video path*/
		if(mmi_phb_video_get_path_by_index(video_id))
		{
			g_phb_cntx.video_location = MMI_PHB_IMAGE_SELECT_PATH;
		}
		else
		{
			g_phb_cntx.video_location = MMI_PHB_IMAGE_NO_SELECT;
		}
	}

	if( g_phb_cntx.video_location != MMI_PHB_IMAGE_NO_SELECT)	/*Change Inline select prompt message.*/
	{
		g_phb_cntx.video_name_list[g_phb_cntx.total_video_id - 1] = (U8*)GetString(STR_ID_PHB_FMGR_IMAGE_SELECTED);
	}

#endif

#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	if(guiBuffer == NULL)  //&& (g_phb_cntx.set_done_flag != 2))
	{
		if(callerGroups[g_phb_cntx.active_index].pictureTagID== 1)
			g_phb_cntx.image_location = MMI_PHB_IMAGE_CALLER_GROUP;
		else
			g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
	}
	if( g_phb_cntx.image_location != MMI_PHB_IMAGE_NO_SELECT) /*Change Inline select prompt message.*/
		g_phb_cntx.image_name_list[g_phb_cntx.total_image_id- 1] = (U8*)GetString(STR_ID_PHB_FMGR_IMAGE_SELECTED);
#endif

	j = 0;

	/*Caller Group Name*/
	SetInlineItemActivation(&wgui_inline_items[j], KEY_LSK, KEY_EVENT_UP);
	SetInlineItemFullScreenEdit(&wgui_inline_items[j],STR_GRP_NAME_CAPTION,IMG_SCR_PBOOK_CAPTION,(PU8)pbName, MAX_GRP_NAME_LENGTH+1, INPUT_TYPE_ALPHANUMERIC_UPPERCASE);
	SetInlineFullScreenEditCustomFunction(&wgui_inline_items[j], mmi_phb_set_full_scr_lsk);
	CGDetailsImageList[j++] = IMG_CG_NAME;

	/*Alert Setting*/
#if !defined(__MMI_CALLERGROUP_NO_ALERT__)
	SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[j], MAX_PB_ALERT_TYPES + 1, g_phb_cntx.alert_name_list, &g_phb_cntx.selected_alert_index);
	CGDetailsImageList[j++] = IMG_CG_ALERT_TYPE;
#endif

	/*Ring Tone Setting*/
	SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[j], g_phb_cntx.total_ring_tone, g_phb_cntx.ring_tone_name_list, &g_phb_cntx.selected_ring_index);
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[j], mmi_phb_ring_tone_highlight_hdlr);
	CGDetailsImageList[j++] = IMG_CG_RING_TONE;

	/*Associate Picture*/
	SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[j], g_phb_cntx.total_image_id, g_phb_cntx.image_name_list, &g_phb_cntx.selected_pic_index);
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[j], mmi_phb_image_highlight_hdlr);
#endif
	gPictureItemIndex = j;
	CGDetailsImageList[j++] = IMG_ID_PHB_CALLER_PIC;

	/*LED Setting*/
#if defined(__MMI_STATUS_LED__)
	SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[j], MAX_PB_LED_PATTERNS + 1, g_phb_cntx.led_name_list, &g_phb_cntx.selected_pattern_index);
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[j], mmi_phb_led_pattern_highlight_hdlr);
	CGDetailsImageList[j++] = IMG_CG_INDICATOR_LITE;
#endif

	/*Incoming call video*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[j], g_phb_cntx.total_video_id, g_phb_cntx.video_name_list, &g_phb_cntx.selected_video_index);
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[j], mmi_phb_video_highlight_hdlr);
	g_phb_video_item_index = j;
	CGDetailsImageList[j++] = IMG_ID_PHB_CALLER_VIDEO_1;
#endif

    /*black list*/
    SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(&wgui_inline_items[j], g_phb_cntx.total_black_list_id, g_phb_cntx.black_list_name_list, &g_phb_cntx.selected_black_list_index);
    RegisterInlineSelectHighlightHandler(&wgui_inline_items[j], mmi_phb_black_list_highlight_hdlr);
    CGDetailsImageList[j++] = IMG_ID_PHB_CALLERGROUP_BLACKLIST;

    /*red list*/
    SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(&wgui_inline_items[j], g_phb_cntx.total_red_list_id, g_phb_cntx.red_list_name_list, &g_phb_cntx.selected_red_list_index);
    RegisterInlineSelectHighlightHandler(&wgui_inline_items[j], mmi_phb_red_list_highlight_hdlr);
    CGDetailsImageList[j++] = IMG_ID_PHB_CALLERGROUP_REDLIST;

	/*Caller Group Member List*/
#if defined(__MMI_PHB_CALLERGROUP_MEMBER_LIST__)
	SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemDisplayOnly(&wgui_inline_items[j],(U8*)GetString(STR_ID_PHB_CALLERGROUP_MEMBER_LIST));
	g_phb_callergroup_member_index = j;
	CGDetailsImageList[j++] = IMG_ID_PHB_CALLERGRP_MEMBER;
#endif

	RegisterHighlightHandler(mmi_phb_callergroup_highlight_hdlr);

	if(inputBuffer!=NULL)
	{
		SetCategory57Data(wgui_inline_items, j, inputBuffer);

		if(viewOnce) /*Reconfig only if view fields has turned on*/
		{
			viewOnce = FALSE;

			/*If select from file but no image selected, clear it to default*/
			if( (g_phb_cntx.selected_pic_index == g_phb_cntx.total_image_id - 1) && \
			    (g_phb_cntx.image_location == MMI_PHB_IMAGE_NO_SELECT))
				g_phb_cntx.selected_pic_index = 0;
			else if( g_phb_cntx.selected_pic_index != g_phb_cntx.total_image_id - 1)
				g_phb_cntx.selected_pic_index = g_phb_cntx.selected_pic_in_view;

			ReConfigureInlineItemSelect(&wgui_inline_items[gPictureItemIndex], g_phb_cntx.total_image_id, g_phb_cntx.image_name_list, &g_phb_cntx.selected_pic_index);
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
			RegisterInlineSelectHighlightHandler(&wgui_inline_items[gPictureItemIndex], mmi_phb_image_highlight_hdlr);
#endif
			SetInlineDoneFlag(guiBuffer);
		}

#if defined(__MMI_INCOMING_CALL_VIDEO__)
			if(g_phb_video_view)
			{
				g_phb_video_view = FALSE;
				if( (g_phb_cntx.selected_video_index == g_phb_cntx.total_video_id - 1) && \
					(g_phb_cntx.video_location == MMI_PHB_IMAGE_NO_SELECT))
					g_phb_cntx.selected_video_index = 0;

				ReConfigureInlineItemSelect(&wgui_inline_items[g_phb_video_item_index], g_phb_cntx.total_video_id, g_phb_cntx.video_name_list, &g_phb_cntx.selected_video_index);
				RegisterInlineSelectHighlightHandler(&wgui_inline_items[g_phb_video_item_index], mmi_phb_video_highlight_hdlr);
				SetInlineDoneFlag(guiBuffer);
			}
#endif
	}
	ShowCategory57Screen(STR_SCR_CG_DETAILS_CAPTION, IMG_SCR_PBOOK_CAPTION,
				  STR_GLOBAL_EDIT, IMG_EDIT_LSK, STR_GLOBAL_DONE, IMG_DONE_RSK,
				  j, CGDetailsImageList, wgui_inline_items, 0, guiBuffer);

#if defined(__MMI_GB__)	
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone);          //add by yaosq 20061108
#endif
	SetCategory57RightSoftkeyFunctions(mmi_phb_callergroup_detail_pre_save,GoBackHistory);

	if(g_phb_cntx.set_done_flag)
	{
		g_phb_cntx.set_done_flag = 0;
		set_wgui_inline_list_menu_changed();
	}

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	/*Check if associate audio in file system exists.*/
	if((guiBuffer == NULL) &&
		(callerGroups[g_phb_cntx.active_index].ringToneID >= PMG_EXT_MELODY_BEGIN) &&
		( callerGroups[g_phb_cntx.active_index].ringToneID <= PMG_EXT_MELODY_END) &&
		!PmgQueryFileExistById(callerGroups[g_phb_cntx.active_index].ringToneID))
			DisplayPopup((PU8)GetString(STR_ID_PHB_AUDIO_ID_NOT_EXIST), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
#endif

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_callergroup_detail()
* DESCRIPTION
*  Exit function for mmi_phb_entry_callergroup_detail()
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_callergroup_detail(void)
{
	history_t	h;
	U16 inputBufferSize;

	g_phb_cntx.end_scr_id = SCR_CALLER_GRP_DETAILS;
	gPictureItemIndex = 10;
	mmi_phb_stop_ring_tone_id();
	mmi_phb_stop_led_pattern_id();
	CloseCategory57Screen();
	h.scrnID = SCR_CALLER_GRP_DETAILS;
	h.entryFuncPtr = mmi_phb_entry_callergroup_detail;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data((U8*) h.inputBuffer);
	AddNHistory(h, inputBufferSize);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_highlight_hdlr()
* DESCRIPTION
*   Highlight handler for Caller Groups details screen
*
* PARAMETERS
*  index	IN	highlight position
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_callergroup_highlight_hdlr(S32 index)
{
    UI_string_ID_type item_texts[] =
    {
        STR_GRP_NAME_CAPTION, // 1-群组名称
        STR_ID_PHB_CALLER_RING, // 2-铃声
        STR_ID_PHB_CALLER_PIC, // 3-来电大头贴   
#if defined(__MMI_STATUS_LED__)
	STR_LIGHT_LCD_BACKLIGHT,
#endif
#if defined(__MMI_INCOMING_CALL_VIDEO__)
        STR_ID_PHB_CALLER_VIDEO, // 4-来电影片
#endif
        STR_SETTING_BLACK_LIST_ACTIVE, // 5-群组黑名单
        STR_SETTING_RED_LIST_ACTIVE, // 6-群组红名单
#if defined(__MMI_PHB_CALLERGROUP_MEMBER_LIST__)
        STR_ID_PHB_CALLERGROUP_MEMBER_LIST/*STR_SCR_CG_DETAILS_CAPTION*/, // 7-群组成员
#endif
    };
    ChangeTitleString( (PU8)GetString( item_texts[index] ) );
    draw_title();

	mmi_phb_stop_ring_tone_id();
	mmi_phb_stop_led_pattern_id();

	/*Highlight on image item*/
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	if((index == gPictureItemIndex) && (g_phb_cntx.selected_pic_index == (g_phb_cntx.total_image_id - 1))) /*View Image File*/
	{
		ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
		SetLeftSoftkeyFunction(mmi_phb_image_pre_entry_option, KEY_EVENT_UP);
	}
	else if((index == gPictureItemIndex) && (g_phb_cntx.selected_pic_index < (g_phb_cntx.total_image_id - 1))) /*View Image ID through Category screen*/
	{
		ChangeLeftSoftkey(STR_VIEW_LSK, IMG_VIEW_LSK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_edit_view_picture, KEY_EVENT_UP);
	}
#else
	if(index == gPictureItemIndex)
	{
		ChangeLeftSoftkey(STR_VIEW_LSK, IMG_VIEW_LSK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_edit_view_picture, KEY_EVENT_UP);
	}
#endif

	/*Highlight on incoming call video item*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	if(index == g_phb_video_item_index)
	{
		 if(g_phb_cntx.selected_video_index == (g_phb_cntx.total_video_id - 1))
		 {
			ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
			SetLeftSoftkeyFunction(mmi_phb_video_pre_entry_file_option, KEY_EVENT_UP);
		 }
		 else if(g_phb_cntx.selected_video_index != 0)
		 {
 			ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
			SetLeftSoftkeyFunction(mmi_phb_entry_video_default_option, KEY_EVENT_UP);
		 }
		 else
		 {
			ChangeLeftSoftkey(0, 0);
			ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
		}
	}
#endif

	/*Highlight on member list item*/
#if defined(__MMI_PHB_CALLERGROUP_MEMBER_LIST__)
	if (index == g_phb_callergroup_member_index)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, STR_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_phb_callergroup_member_pre_enter_list, KEY_EVENT_UP);
	}
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_detail_pre_save()
* DESCRIPTION
*   Checks before saving the Caller Group details
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_callergroup_detail_pre_save(void)
{
	U8 i;
	pBOOL flag = FALSE;
	PHB_CALLER_GROUP_STRUCT* callerGroups;

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;

	/*Each group name can not be the same.*/
	for(i = 0; i < MAX_PB_CALLER_GROUPS; ++i)
	{
		if(i != g_phb_cntx.active_index)
		{
			if(!pfnUnicodeStrcmp((PS8)callerGroups[i].groupName, pbName))
			{
				flag = TRUE;
				break;
			}
		}
	}

	if(flag)
		DisplayPopup((PU8)GetString(STR_CG_RENAME_FAIL_MSG), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	else
		mmi_phb_entry_callergroup_detail_save_confirm();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_callergroup_detail_save_confirm()
* DESCRIPTION
*   Confirms for saving Caller groups details
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_callergroup_detail_save_confirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO,
		get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, WARNING_TONE);


	SetLeftSoftkeyFunction(mmi_phb_callergroup_detail_save, KEY_EVENT_UP);

	SetRightSoftkeyFunction(mmi_phb_go_back_2_history, KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_detail_save()
* DESCRIPTION
*   Saves the Caller Group details on the NVRAM
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_callergroup_detail_save(void)
{
	S16 pError;
	BOOL copy_image = TRUE, copy_video = TRUE;
	PHB_CALLER_GROUP_STRUCT* callerGroups;

mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	mmi_phb_show_in_progress(STR_GLOBAL_SAVING, IMG_GLOBAL_SAVE);

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;

	pfnUnicodeStrcpy ((S8*)callerGroups[g_phb_cntx.active_index].groupName, pbName);

	/*Ring Tone Setting*/
	callerGroups[g_phb_cntx.active_index].ringToneID = g_phb_cntx.ring_tone_list[g_phb_cntx.selected_ring_index];

	/*Image Setting*/
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	if(g_phb_cntx.selected_pic_index != g_phb_cntx.total_image_id - 1)	/*De-select the file if selection is not from file.*/
		g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
	else if(g_phb_cntx.image_location == MMI_PHB_IMAGE_NO_SELECT) /*Set selection to none if not select picture from file*/
		g_phb_cntx.selected_pic_index = 0;

	copy_image = mmi_phb_image_update_by_index((U16)(g_phb_cntx.active_index + 5001));	/*Copy image to folder in file system, 5001 for group 1 and so on.*/
	if(!copy_image) /*Edit image fail.*/
		g_phb_cntx.selected_pic_index = 0;

	if(g_phb_cntx.selected_pic_index == g_phb_cntx.total_image_id - 1)
		callerGroups[g_phb_cntx.active_index].pictureTagID = 1;
	else
#endif
		callerGroups[g_phb_cntx.active_index].pictureTagID = g_phb_cntx.image_id_list[g_phb_cntx.selected_pic_index];

	/*Alert Setting*/
#if defined(__MMI_CALLERGROUP_NO_ALERT__)
	callerGroups[g_phb_cntx.active_index].alertType = MMI_ALERT_NONE;
#else
	callerGroups[g_phb_cntx.active_index].alertType = (MMI_ALERT_TYPE)g_phb_cntx.selected_alert_index;
#endif

	/*LED setting*/
#if defined(__MMI_STATUS_LED__)
	callerGroups[g_phb_cntx.active_index].LEDPatternId = (U8)g_phb_cntx.selected_pattern_index;
#else
	callerGroups[g_phb_cntx.active_index].LEDPatternId = 0; /*no pattern*/
#endif

	/*Incoming call video setting*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	callerGroups[g_phb_cntx.active_index].VideoID =
	 		mmi_phb_video_update_by_index_for_path(callerGroups[g_phb_cntx.active_index].VideoID, &copy_video);
#endif
    callerGroups[g_phb_cntx.active_index].black_list_flag_Id = g_phb_cntx.black_list_id_list[g_phb_cntx.selected_black_list_index];
    callerGroups[g_phb_cntx.active_index].red_list_flag_Id = g_phb_cntx.red_list_id_list[g_phb_cntx.selected_red_list_index];

	/*Check if name is modified.*/
	if((callerGroups[g_phb_cntx.active_index].is_modified == 0) &&
	    (pfnUnicodeStrcmp((S8*)callerGroups[g_phb_cntx.active_index].groupName,GetString((U16)(STR_CALLER_GROUP_1+g_phb_cntx.active_index))) != 0))
		callerGroups[g_phb_cntx.active_index].is_modified = 1;

	WriteRecord(NVRAM_EF_PHB_CALLER_GROUPS_LID, 1, callerGroups, CALLER_GROUPS_RECORD_SIZE, &pError);

	/*Data update success, but image or video add fail*/
	if(!copy_image || !copy_video)
	{
		S8 str_buff[64*2];
		memset(str_buff, 0 ,2);

		if(!copy_image)
		{
			pfnUnicodeStrcat(str_buff, GetString(STR_ID_PHB_OP_IMAGE_FAIL));
		}

		if(!copy_video)
		{
			if(!copy_image)
			{
				pfnUnicodeStrcat(str_buff, GetString(STR_ID_PHB_OP_COMMA));
			}
			pfnUnicodeStrcat(str_buff, GetString(STR_ID_PHB_OP_VIDEO_FAIL));
		}
		pfnUnicodeStrcat(str_buff, GetString(STR_ID_PHB_EDIT_FAIL));
		DisplayPopup((U8*)str_buff, IMG_GLOBAL_ERROR,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	}

	if(!DeleteNScrId(SCR_ID_PHB_FMGR_SAVE_OPTION))	/*Enter From Save image from file manager*/
		DeleteNScrId(SCR_CALLER_GRP_DETAILS);	/*Enter from caller group*/
}


#define MMI_PHB_CALLER_GROUP_MEMBER
/*-------------------------------------------------Caller Group Member List -----------------------------------------------------------*/
#if defined( __MMI_PHB_CALLERGROUP_MEMBER_LIST__	)
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_callergroup_member_add
* DESCRIPTION
*   Highlight function.
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_callergroup_member_add(void)
{
	SetLeftSoftkeyFunction(mmi_phb_callergroup_member_pre_enter_add,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_callergroup_member_pre_enter_add, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_callergroup_member_delete
* DESCRIPTION
*   Highlight function
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_callergroup_member_delete(void)
{
	SetLeftSoftkeyFunction(mmi_phb_entry_callergroup_member_delete,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_callergroup_member_list
* DESCRIPTION
*   Entry function of phonebook caller group member list.
*    Will show list if any entry exists, or will display "add" screen at begin.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_callergroup_member_list(void)
{
	U8* guiBuffer;
	S8 state;

	EntryNewScreen(SCR_ID_PHB_CALLERGROUP_MEMBER_LIST, mmi_phb_exit_callergroup_member_list, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_CALLERGROUP_MEMBER_LIST);
	state = (S8)-1;
	if(guiBuffer != NULL)
	{
		state = get_list_menu_category_history_state(SCR_ID_PHB_CALLERGROUP_MEMBER_LIST,guiBuffer);
		if (state != -1)
		{
			phbListView = (U8)state;
			g_phb_callergroup_member_count = mmi_phb_callergroup_member_count_number((U8)phbListView);
		}
	}

	/*Allocate a buffer to store caller group of each entry to enhance speed.*/
	if(g_phb_callergroup_ptr == NULL)
#if (MAX_PB_ENTRIES >= 1000)
		g_phb_callergroup_ptr = (U16*)g_phb_list_filter;
#else
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		g_phb_callergroup_ptr = OslMalloc(MAX_PB_ENTRIES*sizeof(U16));
#else
		g_phb_callergroup_ptr = OslMalloc(MAX_PB_PHONE_ENTRIES*sizeof(U16));
#endif
#endif /* MAX_PB_ENTRIES >= 1000 */

	/*After allocate buffer, make cache for caller group of each entry.*/
	mmi_phb_callergroup_member_make_group_id_cache();

	if (phbListView == MMI_PHB_LIST_FOR_VIEW_MEMBER_LIST)
	{
		/*Build alpha index list*/
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		mmi_phb_list_build_alpha_index((U16*)g_phb_callergroup_ptr, (U16)(g_phb_cntx.active_index + 1 ), TRUE, MMI_STORAGE_NONE);
#else
		mmi_phb_list_build_alpha_index((U16*)g_phb_callergroup_ptr, (U16)(g_phb_cntx.active_index + 1 ), TRUE, MMI_NVRAM);
#endif
		g_phb_cntx.refresh_list = 1;

		RegisterHighlightHandler(mmi_phb_get_index_second_level);

		ShowCategory184Screen(STR_ID_PHB_CALLERGROUP_MEMBER_LIST, IMG_SCR_PBOOK_CAPTION,
								STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								g_phb_callergroup_member_count,
								mmi_phb_callergroup_member_list_get_item,
								mmi_phb_callergroup_member_list_get_hint,
								g_phb_cntx.highlight_entry,
								guiBuffer);
		SetLeftSoftkeyFunction(mmi_phb_entry_callergroup_member_option,KEY_EVENT_UP);
		SetKeyHandler(mmi_phb_entry_callergroup_member_option,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
	else if (phbListView == MMI_PHB_LIST_FOR_ADD_MEMBER_LIST)
	{
		/*Build alpha index list*/
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		mmi_phb_list_build_alpha_index((U16*)g_phb_callergroup_ptr, (U16)(g_phb_cntx.active_index + 1 ),FALSE,MMI_STORAGE_NONE);
#else
		mmi_phb_list_build_alpha_index((U16*)g_phb_callergroup_ptr, (U16)(g_phb_cntx.active_index + 1 ),FALSE,MMI_NVRAM);
#endif
		g_phb_cntx.refresh_list = 1;

		RegisterHighlightHandler(mmi_phb_get_index_third_level);

		ShowCategory184Screen(STR_SCR_PBOOK_VIEW_CAPTION, IMG_SCR_PBOOK_CAPTION,
								STR_GLOBAL_ADD, 0,
								STR_GLOBAL_BACK, 0,
								g_phb_callergroup_member_count,
								mmi_phb_callergroup_member_list_get_item,
								mmi_phb_callergroup_member_list_get_hint,
								g_phb_cntx.highlight_entry,
								guiBuffer);
		SetLeftSoftkeyFunction(mmi_phb_entry_callergroup_member_add,KEY_EVENT_UP);
	}

	/*register multitap key for fast jump by alpha index*/
	register_multitap_no_draw_key_handlers();
	set_multitap_functions(mmi_phb_list_alpha_index, mmi_phb_list_alpha_index_input_callback);
	change_multitap_mode(0);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_callergroup_member_list
* DESCRIPTION
*   Exit function of caller group member list.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_callergroup_member_list(void)
{
	history_t	h;
	S16		nHistory	 = 0;
	S8 state = (S8)phbListView;

	g_phb_cntx.end_scr_id = SCR_ID_PHB_CALLERGROUP_MEMBER_LIST;

	h.scrnID = SCR_ID_PHB_CALLERGROUP_MEMBER_LIST;
	h.entryFuncPtr = mmi_phb_entry_callergroup_member_list;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, ( S8*)&nHistory);
	GetCategoryHistory(h.guiBuffer);

	set_list_menu_category_history_state(SCR_ID_PHB_CALLERGROUP_MEMBER_LIST,h.guiBuffer,state);
	AddHistory(h);

	/*Free memory*/
#if (MAX_PB_ENTRIES < 1000)
	if(g_phb_callergroup_ptr)
	{
		OslMfree(g_phb_callergroup_ptr);
		g_phb_callergroup_ptr = NULL;
	}
#endif /* MAX_PB_ENTRIES < 1000 */
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_callergroup_member_option
* DESCRIPTION
*   Entry function of caller group member list options.
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_callergroup_member_option(void)
{
	U8* guiBuffer;
	U16 nStrItemList[2];
	U16 nNumofItem;

	EntryNewScreen(SCR_ID_PHB_CALLERGROUP_MEMBER_VIEW_OPTION, mmi_phb_exit_callergroup_member_option, mmi_phb_entry_callergroup_member_option, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_CALLERGROUP_MEMBER_VIEW_OPTION);
	nNumofItem = GetNumOfChild(MENU_ID_PHB_CALLERGROUP_MEMBER_VIEW_OPTION);
	GetSequenceStringIds(MENU_ID_PHB_CALLERGROUP_MEMBER_VIEW_OPTION, nStrItemList);
	SetParentHandler(MENU_ID_PHB_CALLERGROUP_MEMBER_VIEW_OPTION);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/*Show Add New Only.*/
	if (((guiBuffer == NULL) && (g_phb_callergroup_member_count == 0))	||
		(mmi_phb_callergroup_member_count_number(MMI_PHB_LIST_FOR_VIEW_MEMBER_LIST) == 0) )
		nNumofItem = 1;

	ShowCategory15Screen(STR_ID_PHB_CALLERGROUP_MEMBER_LIST, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_callergroup_member_option
* DESCRIPTION
*   Exit function
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_callergroup_member_option(void)
{
	g_phb_cntx.end_scr_id = SCR_ID_PHB_CALLERGROUP_MEMBER_VIEW_OPTION;
}


/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_callergroup_member_add
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_callergroup_member_add(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_GLOBAL_ADD), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(mmi_phb_callergroup_member_add_member,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_callergroup_member_delete
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_callergroup_member_delete(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_DELETE_QUERY_MSG), IMG_GLOBAL_QUESTION, WARNING_TONE);


	SetLeftSoftkeyFunction(mmi_phb_callergroup_member_delete_member,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_member_list_get_item
* DESCRIPTION
*   Item call back function for dynamic list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_phb_callergroup_member_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	U16 i;

	if((item_index < 0) || (item_index > PhoneBookEntryCount))
		return FALSE;

	i = mmi_phb_callergroup_member_get_index((U8)phbListView, (U16)item_index);

	if (pfnUnicodeStrlen((S8*)PhoneBook[i].alpha_id.name))
		pfnUnicodeStrcpy((S8*) str_buff, (S8*)PhoneBook[i].alpha_id.name);
	else
		mmi_phb_convert_get_ucs2_number((S8*)str_buff, i);

#ifdef __MMI_MULTI_SIM__
	if (i < MAX_PB_PHONE_ENTRIES)
	{
		*img_buff_p = get_image(IMG_STORAGE_HANDSET);
	}
	else
	{
		U8 nSimIndex = (i - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES;
		*img_buff_p = get_image(IMG_STORAGE_SIM_1+nSimIndex);
	}
#else
	if (i < MAX_PB_PHONE_ENTRIES)
		*img_buff_p = get_image(IMG_STORAGE_HANDSET);
	else
		*img_buff_p = get_image(IMG_STORAGE_SIM);
#endif	

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_member_list_get_hint
* DESCRIPTION
*   hint call back function for dynamic list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_phb_callergroup_member_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	U16 i;
	S8 temp_number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];

	if((item_index < 0) || (item_index > PhoneBookEntryCount))
		return FALSE;

	i = mmi_phb_callergroup_member_get_index((U8)phbListView, (U16)item_index);

	mmi_phb_convert_get_ucs2_number((S8*)temp_number, i);
	if (pfnUnicodeStrlen((S8*)PhoneBook[i].alpha_id.name) && pfnUnicodeStrlen((S8*)temp_number))
		pfnUnicodeStrcpy((S8*)hint_array[0], (S8*)temp_number);
	else
		return 0;

	return 1;  /*One hint data only, can be more hints.*/
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_member_pre_enter_list
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_callergroup_member_pre_enter_list(void)
{
	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		if(PhoneBookEntryCount)
		{
			g_phb_cntx.highlight_entry = 0;
			phbListView = MMI_PHB_LIST_FOR_VIEW_MEMBER_LIST;
			g_phb_callergroup_member_count = mmi_phb_callergroup_member_count_number((U8)phbListView);
			if ( g_phb_callergroup_member_count > 0)
				mmi_phb_entry_callergroup_member_list();
			else
				mmi_phb_entry_callergroup_member_option();
		}
		else
			DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
	}
	else
		mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_member_pre_enter_add
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_callergroup_member_pre_enter_add(void)
{
	phbListView = MMI_PHB_LIST_FOR_ADD_MEMBER_LIST;

	g_phb_callergroup_member_count = mmi_phb_callergroup_member_count_number((U8)phbListView);
	if ( g_phb_callergroup_member_count > 0)
		mmi_phb_entry_callergroup_member_list();
	else
		DisplayPopup((PU8)GetString(STR_ID_PHB_NO_ENTRY_TO_SELECT), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_member_add_member
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_callergroup_member_add_member(void)
{
	U16 count, i;
	S16 pError;
	U16 store_index;
	U8 optid_record, optid_index, current_record = 0;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

	count = 0;
	for(i = 0; i < PhoneBookEntryCount; i++)
	{
		store_index = g_phb_name_index[i];

#if !defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		if(store_index < MAX_PB_PHONE_ENTRIES)	/*In NVRAM*/
#endif
		{
			/*Get optional IDs record.*/
			optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
			optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

			if(optid_record != current_record)
			{
				ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
				current_record = optid_record;
			}

			/*Get index*/
			if(PhbOptIDs[optid_index].callerGroupID != (g_phb_cntx.active_index + 1 ))
			{
				if(count == g_phb_cntx.active_index_third)
					break;

				count++;
			}
		}
	}

	store_index = g_phb_name_index[i];

	/*Read optional ID field and write it back.*/
	optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
	optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

	ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

	PhbOptIDs[optid_index].callerGroupID = g_phb_cntx.active_index + 1;
	WriteRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	g_phb_callergroup_member_count++;
	g_phb_cntx.highlight_entry = 0;
	DeleteNScrId(SCR_ID_PHB_CALLERGROUP_MEMBER_VIEW_OPTION);
	phbListView = MMI_PHB_LIST_FOR_VIEW_MEMBER_LIST;
	g_phb_callergroup_member_count = mmi_phb_callergroup_member_count_number((U8)phbListView);
	if (g_phb_callergroup_member_count == 1)
	{
		mmi_phb_entry_callergroup_member_list();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_member_delete_member
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
UINT8 get_phb_BR(void)
{
	return phbBRlist;
}
void mmi_phb_blackAndRedList_set_value(void)
{
	INT16 error;
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 1000, SUCCESS_TONE);
	DeleteNHistory(1);
	phbBRlist = (U16)GetCurrHiliteID();	
	WriteValue(NVRAM_EF_PHB_BLACK_AND_RED_LIST, (void*)&phbBRlist, DS_BYTE, &error);
}

void mmi_phb_blackAndRedList_set(void)
{
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8*		guiBuffer;
	EntryNewScreen(SCR_ID_PHB_BLACK_RED_LIST, NULL, mmi_phb_blackAndRedList_set, NULL);


	guiBuffer	= GetCurrGuiBuffer(SCR_ID_PHB_BLACK_RED_LIST);
	nItems		= GetNumOfChild(CM_BL_AND_RL_STATUS);//this ID is to do
	GetSequenceStringIds(CM_BL_AND_RL_STATUS,ItemList);//this ID is to do
	GetSequenceImageIds(CM_BL_AND_RL_STATUS,ItemIcons);//this ID is to do
	SetParentHandler(CM_BL_AND_RL_STATUS);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	ShowCategory11Screen(	STR_SETTING_CALLGROUP_LIST,
									GetRootTitleIcon(CM_BL_AND_RL_STATUS),
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
									nItems,
									ItemList,
									phbBRlist,
									guiBuffer);
	
	SetLeftSoftkeyFunction(mmi_phb_blackAndRedList_set_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

void mmi_phb_callergroup_member_delete_member(void)
{
	U16 count, i;
	S16 pError;
	U16 store_index;
	U8 optid_record, optid_index, current_record = 0;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];
	//PHB_CALLER_GROUP_STRUCT* callerGroups;

	//callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;
	count = 0;
	for(i = 0; i < PhoneBookEntryCount; i++)
	{
		store_index = g_phb_name_index[i];

#if !defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		if(store_index < MAX_PB_PHONE_ENTRIES)
#endif
		{
			/*Get optional IDs record.*/
			optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
			optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

			if(optid_record != current_record)
			{
				ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
				current_record = optid_record;
			}


			if( (g_phb_cntx.active_index + 1 ) == PhbOptIDs[optid_index].callerGroupID)
			{
				if(count == g_phb_cntx.active_index_second)
					break;

				count++;
			}
		}
	}

	store_index = g_phb_name_index[i];
	/*Read optional ID field and write it back.*/
	optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
	optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

	ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

	PhbOptIDs[optid_index].callerGroupID = 0;
	WriteRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);

	if(mmi_phb_callergroup_member_count_number(MMI_PHB_LIST_FOR_VIEW_MEMBER_LIST) == 0)
	{
		DeleteNScrId(SCR_ID_PHB_CALLERGROUP_MEMBER_LIST);
	}
	else
	{
		g_phb_cntx.highlight_entry = 0;
		DeleteUptoScrID(SCR_ID_PHB_CALLERGROUP_MEMBER_LIST);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_member_count_number
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_callergroup_member_count_number(U8 list_enum)
{
	U16 i,j,count = 0;
	S16 pError;
	U8 optid_record, optid_index, current_record = 0;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
	for(i = 0; i < MAX_PB_ENTRIES; i++)
#else
	for(i = 0; i < MAX_PB_PHONE_ENTRIES; i++)
#endif
		{
			/*Get optional IDs record.*/
		optid_record = (i / OPTIONAL_IDS_RECORD_TOTAL) + 1;
		optid_index = i - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

			if(optid_record != current_record)
			{
				ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
				current_record = optid_record;
			}

			/*Count Entry Number*/
			if(	((list_enum == MMI_PHB_LIST_FOR_VIEW_MEMBER_LIST) &&
			    	( PhbOptIDs[optid_index].callerGroupID) == (g_phb_cntx.active_index + 1 ))
				||
				((list_enum == MMI_PHB_LIST_FOR_ADD_MEMBER_LIST) &&
			    	(PhbOptIDs[optid_index].callerGroupID != (g_phb_cntx.active_index + 1 )))	)
			{

			for( j=0 ; j<PhoneBookEntryCount ; j++)
				if(i == g_phb_name_index[j])
				{
				count++;
					break;
			}
		}

	}

	return count;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_member_make_group_id_cache
* DESCRIPTION
*   Allocate a buffer and make cache for callergroup of each phonebook entry.
*   This is to reduce the NVRAM when showing in list screen.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_callergroup_member_make_group_id_cache(void)
{
	U16 store_index;
	S16 pError;
	U8 optid_record, optid_index, current_record = 0;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
	for(store_index = 0; store_index < MAX_PB_ENTRIES; store_index++)
#else
	for(store_index = 0; store_index < MAX_PB_PHONE_ENTRIES; store_index++)
#endif
	{
		/*Get optional IDs record.*/
		optid_record = (store_index / OPTIONAL_IDS_RECORD_TOTAL) + 1;
		optid_index = store_index - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

		if(optid_record != current_record)
		{
			ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
			current_record = optid_record;
		}

		g_phb_callergroup_ptr[store_index] = (U16)PhbOptIDs[optid_index].callerGroupID;

	}


}

/*****************************************************************************
* FUNCTION
*  mmi_phb_callergroup_member_get_index
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_callergroup_member_get_index(U8 list_enum, U16 item_index)
{
	U16 i,count = 0;
	U16 store_index = 0;

	for(i = 0; i < PhoneBookEntryCount; i++)
	{
		store_index = g_phb_name_index[i];

#if !defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		if(store_index < MAX_PB_PHONE_ENTRIES)
#endif
		{
			if(	((list_enum == MMI_PHB_LIST_FOR_VIEW_MEMBER_LIST) &&
			    	( g_phb_callergroup_ptr[store_index] == (g_phb_cntx.active_index + 1 )))
				||
				((list_enum == MMI_PHB_LIST_FOR_ADD_MEMBER_LIST) &&
			    	(g_phb_callergroup_ptr[store_index] != (g_phb_cntx.active_index + 1 )))	)
			{
				if(count == item_index)
					break;
				count++;
			}
		}
	}

	return store_index;
}

#endif  /* END of #if defined( __MMI_PHB_CALLERGROUP_MEMBER_LIST__	) */
/*--------------------------------------------END Caller Group Member List -----------------------------------------------------------*/

#define MMI_PHB_INLINE_ITEM
/*****************************************************************************
* FUNCTION
*  mmi_phb_ring_tone_highlight_hdlr()
* DESCRIPTION
*   Highlight handler for ringtones inline selection list
*
* PARAMETERS
*  ringToneIndex	IN	ring tone id for play
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_ring_tone_highlight_hdlr(S32 ringToneIndex)
{
	mmi_phb_stop_ring_tone_id();


	if(ringToneIndex)
	{
		g_phb_curr_ring_tone_id = g_phb_cntx.ring_tone_list[ringToneIndex];
		g_phb_last_ring_tone_id = g_phb_curr_ring_tone_id;
		StartTimer(PHB_NOTIFYDURATION_TIMER,1000,mmi_phb_play_ring_tone_id);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_play_ring_tone_id()
* DESCRIPTION
*   Plays the selected ringtone
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_play_ring_tone_id(void)
{
	//g_phb_last_ring_tone_id = g_phb_curr_ring_tone_id;
	StopTimer(PHB_NOTIFYDURATION_TIMER);
	/*Play ring tone repeatedly*/
	if( !IsSilentModeActivated() && !isInCall() ) // need to check if it's in call because phonebook use this function, too
	{
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
			AudioPlayReq(g_phb_curr_ring_tone_id,(U8)DEVICE_AUDIO_PLAY_INFINITE);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_stop_ring_tone_id()
* DESCRIPTION
*   Stops the currently played ringtone
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_stop_ring_tone_id(void)
{
	if(g_phb_last_ring_tone_id)
	{
		StopTimer(PHB_NOTIFYDURATION_TIMER);
		AudioStopReq(g_phb_last_ring_tone_id);
	}
	g_phb_last_ring_tone_id = 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_led_pattern_highlight_hdlr()
* DESCRIPTION
*   Highlight handler for LED patterns inline selection list
*
* PARAMETERS
*  index	IN	index for play
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/

void mmi_phb_led_pattern_highlight_hdlr(S32 index)
{
	mmi_phb_stop_led_pattern_id();
	if(index)
	{
		g_phb_curr_pattern_id = (U8)index;
		g_phb_last_pattern_id = g_phb_curr_pattern_id;
		StartTimer(PHB_NOTIFYDURATION_TIMER,1000,mmi_phb_play_led_pattern_id);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_play_led_pattern_id()
* DESCRIPTION
*   Plays the selected LED pattern
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_play_led_pattern_id(void)
{
	//g_phb_last_pattern_id = g_phb_curr_pattern_id;
	StopTimer(PHB_NOTIFYDURATION_TIMER);
	//PlayPatternCallerGroup(g_phb_curr_pattern_id);    // zrx de 20060612
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_stop_led_pattern_id()
* DESCRIPTION
*   Stops the currently played LED pattern
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_stop_led_pattern_id(void)
{
	if(g_phb_last_pattern_id)
	{
		StopTimer(PHB_NOTIFYDURATION_TIMER);
		//StopPatternCallerGroup(g_phb_last_pattern_id);  // zrx del 20060612
	}
	g_phb_last_pattern_id = 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_build_led_and_alert_list()
* DESCRIPTION
*   Builds the Ringtone, Pictures, Alerts & Patterns list for inline selection
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_build_led_and_alert_list(void)
{
	/*Use the same string ID with ProfilesEnum.h */
#if defined(__MMI_STATUS_LED__)
	g_phb_cntx.led_name_list[0] = (PU8)GetString(STR_GLOBAL_NONE);
	g_phb_cntx.led_name_list[1] = (PU8)GetString(STR_LIGHT_STATUS_COLOR1);
	g_phb_cntx.led_name_list[2] = (PU8)GetString(STR_LIGHT_STATUS_COLOR2);
	g_phb_cntx.led_name_list[3] = (PU8)GetString(STR_LIGHT_STATUS_COLOR3);
	g_phb_cntx.led_name_list[4] = (PU8)GetString(STR_LIGHT_STATUS_COLOR4);
	g_phb_cntx.led_name_list[5] = (PU8)GetString(STR_LIGHT_STATUS_COLOR5);
	g_phb_cntx.led_name_list[6] = (PU8)GetString(STR_LIGHT_STATUS_COLOR6);
#endif

	/*Use Same String ID with ProfilesEnum.h	*/
	g_phb_cntx.alert_name_list[0] = (PU8)GetString(STR_GLOBAL_NONE);
	g_phb_cntx.alert_name_list[1] = (PU8)GetString(STR_RING);
	g_phb_cntx.alert_name_list[2] = (PU8)GetString(STR_VIBRATION_ONLY);
	g_phb_cntx.alert_name_list[3] = (PU8)GetString(STR_VIBRATION_AND_RING);
	g_phb_cntx.alert_name_list[4] = (PU8)GetString(STR_VIBRATION_THEN_RING);
#if defined(__MMI_STATUS_LED__)
	g_phb_cntx.alert_name_list[5] = (PU8)GetString(STR_SILENT);
#endif
}


/*****************************************************************************
* FUNCTION
*  mmi_phb_build_ring_tone_list()
* DESCRIPTION
*   Builds the ringtone list for inline selection
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   Build global ring tone list.
*****************************************************************************/
void mmi_phb_build_ring_tone_list(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                          */
	/*----------------------------------------------------------------*/
	U8 i = 0, j;
	U8 ringToneStartId, ringToneTotalId, midiToneStartId, midiToneTotalId;
	//U8 factoryTonesCnt;	/*Factory Default*/
	U8 factoryMelodiesCnt, composedMelodiesCnt,downloadMelodiesCnt, downloadEmsMelodiesCnt; /*iMelody*/
	U8 downloadTonesCnt; /*WAP download*/
	U8 extMelodyCnt; /*File Manager Associate.*/

	S8 dummyStr[8];

	/*Default Factory Tones.*/
	//DYNAUDIOINFO *factoryTones;

	/*iMelody support*/
#if defined(__MMI_IMELODY_SUPPORT__)
	DYNAUDIOINFO *factoryMelodies;
  #if defined(__MMI_MESSAGES_EMS__) && defined(__DOWNLOAD__)
	DYNAUDIOINFO *downloadEmsMelodies;
  #endif
  #if defined(__MMI_WAP__)
	DYNAUDIOINFO *downloadMelodies;
  #endif
  #if defined(__MMI_RING_COMPOSER__)
	DYNAUDIOINFO *composedMelodies;
  #endif
#endif

	/*WAP Download supported, only turn on when no file manager app.*/
#if defined(__MMI_WAP__)
	DYNAUDIOINFO *downloadTones;
#endif

	/*External Melody, select from file manager.*/
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
     DYNEXTMELODYINFO *ExtMelodyInfo;
#endif

	/*----------------------------------------------------------------*/
	/* Code Body                                                               */
	/*----------------------------------------------------------------*/

	/*Default Factory Tones.*/
	GetRingToneRange(&ringToneStartId, &ringToneTotalId);
	GetMidiRange(&midiToneStartId, &midiToneTotalId);
	//factoryTonesCnt = GetFactoryTonesInfo(&factoryTones);  // zrx del 20060612

	/*iMelody support*/
#if defined(__MMI_IMELODY_SUPPORT__)
	factoryMelodiesCnt	= GetFactoryIMelodyInfo(&factoryMelodies);
  #if defined(__MMI_WAP__)
	downloadMelodiesCnt = GetDownloadIMelodyInfo(&downloadMelodies);
  #else
	downloadMelodiesCnt = 0;
  #endif

  #if defined(__MMI_MESSAGES_EMS__) && defined(__DOWNLOAD__)
     downloadEmsMelodiesCnt = GetDownloadEmsIMelodyInfo(&downloadEmsMelodies);
  #else
	downloadEmsMelodiesCnt = 0;
  #endif

  #if defined(__MMI_RING_COMPOSER__)
	composedMelodiesCnt = GetComposedIMelodyInfo(&composedMelodies);
  #else
	composedMelodiesCnt = 0;
  #endif
#else
	factoryMelodiesCnt	= 0;
	downloadMelodiesCnt = 0;
	downloadEmsMelodiesCnt = 0;
	composedMelodiesCnt = 0;
#endif

	/*WAP Download supported, only turn on when no file manager app.*/
#if defined(__MMI_WAP__)
	downloadTonesCnt	= GetDownloadTonesInfo(&downloadTones);
#else
	downloadTonesCnt = 0;
#endif

	/*External Melody, select from file manager.*/
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
     extMelodyCnt = PmgGetExtMelodyInfo(&ExtMelodyInfo);
#else
     extMelodyCnt = 0;
#endif

	/*
	*  The following section will Begin to associate label for inline selection.
	*/

	g_phb_cntx.total_ring_tone = 1 + ringToneTotalId + midiToneTotalId + extMelodyCnt;
#if 0
		+ midiToneTotalId
		+ factoryTonesCnt
		+ factoryMelodiesCnt
		+ downloadTonesCnt
		+ downloadMelodiesCnt
		+ composedMelodiesCnt
		+ downloadEmsMelodiesCnt
		+ extMelodyCnt;
#endif
	g_phb_cntx.ring_tone_name_list[0] = (PU8)GetString(STR_GLOBAL_NONE);
	g_phb_cntx.ring_tone_list[0] = 0;
	pfnUnicodeStrcpy((PS8)subMenuData[0], GetString(STR_GLOBAL_NONE));

	/*Default Factory Tones.*/
	for(j = 0; j < ringToneTotalId; ++j)
	{
		pfnUnicodeStrcpy((PS8)subMenuData[++i], GetString(STR_PROFILES_RING));
		coolsand_UI_itoa((j+1), (UI_string_type)dummyStr, 10);
		pfnUnicodeStrcat((PS8)subMenuData[i], dummyStr);
		g_phb_cntx.ring_tone_list[i] = j+ringToneStartId;
		g_phb_cntx.ring_tone_name_list[i] = subMenuData[i];
	}
	
	for(j = 0; j < midiToneTotalId; ++j)
	{
		pfnUnicodeStrcpy((PS8)subMenuData[++i], GetString(STR_PROFILES_MIDI));
		coolsand_UI_itoa((j+1), (UI_string_type)dummyStr, 10);
		pfnUnicodeStrcat((PS8)subMenuData[i], dummyStr);
		g_phb_cntx.ring_tone_list[i] = j+midiToneStartId;
		g_phb_cntx.ring_tone_name_list[i] = subMenuData[i];
	}
	#if 0
	for(j = 0; j < factoryTonesCnt; ++j)
	{
		g_phb_cntx.ring_tone_list[++i] = factoryTones[j].AudioId;
		g_phb_cntx.ring_tone_name_list[i] = factoryTones[j].AudioName;
	}
#endif
	/*iMelody support*/
#if defined(__MMI_IMELODY_SUPPORT__)
	for(j = 0; j < factoryMelodiesCnt; ++j)
	{
		g_phb_cntx.ring_tone_list[++i] = factoryMelodies[j].AudioId;
		g_phb_cntx.ring_tone_name_list[i] = factoryMelodies[j].AudioName;
	}

	#if defined(__MMI_WAP__)
		for(j = 0; j < downloadMelodiesCnt; ++j)
		{
			g_phb_cntx.ring_tone_list[++i] = downloadMelodies[j].AudioId;
			g_phb_cntx.ring_tone_name_list[i] = downloadMelodies[j].AudioName;
		}
	#endif

	#if defined(__MMI_RING_COMPOSER__)
		for(j = 0; j < composedMelodiesCnt; ++j)
		{
			g_phb_cntx.ring_tone_list[++i] = composedMelodies[j].AudioId;
			g_phb_cntx.ring_tone_name_list[i] = composedMelodies[j].AudioName;
		}
	#endif

	#if defined(__MMI_MESSAGES_EMS__) && defined(__DOWNLOAD__)
		for(j = 0; j < downloadEmsMelodiesCnt; ++j)
		{
			g_phb_cntx.ring_tone_list[++i] = downloadEmsMelodies[j].AudioId;
			g_phb_cntx.ring_tone_name_list[i] = downloadEmsMelodies[j].AudioName;
		}
	#endif
#endif

	/*WAP Download supported, only turn on when no file manager app.*/
#ifdef __MMI_WAP__
	for(j = 0; j < downloadTonesCnt; ++j)
	{
		g_phb_cntx.ring_tone_list[++i] = downloadTones[j].AudioId;
		g_phb_cntx.ring_tone_name_list[i] = downloadTones[j].AudioName;
	}
#endif

	/*External Melody, select from file manager.*/
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	for(j = 0; j < MAXIMUM_EXT_MELODY_NUM; ++j)
	{
	     if( PmgIsEmptySlot(j) == MMI_TRUE )
			continue;

		g_phb_cntx.ring_tone_list[++i] = ExtMelodyInfo->info[j].id;

		if( IS_EXTMELODY_SHORTNAME( ExtMelodyInfo->info[j].filepath ) )
			mmi_chset_mixed_text_to_ucs2_str( subMenuData[i], (FMGR_MAX_FILE_LEN+1) * ENCODING_LENGTH, (U8*)ExtMelodyInfo->info[j].filename, g_chset_text_encoding );
		else
			pfnUnicodeStrcpy((PS8)subMenuData[i],  (PS8) ExtMelodyInfo->info[j].filename );

	#if (!defined(__MMI_SHOW_FILE_EXT__))
		PmgRemoveFileExt( (S8*) subMenuData[i] );
	#elif (!defined(__MMI_SHOW_DAF_FILE_EXT__))
		PmgRemoveFileDAFExt( (S8*) subMenuData[i] );
	#endif
		g_phb_cntx.ring_tone_name_list[i] = subMenuData[i];
	}
#endif


}

/*****************************************************************************
* FUNCTION
*  mmi_phb_build_image_list()
* DESCRIPTION
*   Builds Images list for inline selection
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   global image list for phonebook.
*****************************************************************************/
void mmi_phb_build_image_list(void)
{
#ifndef	__MMI_RESOURCE_SLIM_ON_128X160__	
	U8 i=0, j;
	U8 maxSystemImages = 4;
#else
	U8 i=0;
	U8 maxSystemImages = 0;
#endif
	//DYNIMAGEINFO *fullScrFactoryImages, *associatedFactoryImages;
	//U8 fullScrFactoryImagesCnt, associatedFactoryImagesCnt;
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
#if defined(__MMI_WAP__)
	DYNIMAGEINFO *downloadedImages;
	U8 downloadedImagesCnt;

	UI_image_type image;	/*Check Image Size Before Associate*/
	S32 w,h;
#endif

	/*System Default Image*/
	//fullScrFactoryImagesCnt = GetFullScreenFactoryImagesInfo(&fullScrFactoryImages);  // zrx del 20060612
	//associatedFactoryImagesCnt = GetAssociatedFactoryImagesInfo(&associatedFactoryImages);  // zrx del 20060612

	 
#if 0
	g_phb_cntx.total_image_id = 1 + maxSystemImages + fullScrFactoryImagesCnt + associatedFactoryImagesCnt;
#endif
	g_phb_cntx.total_image_id = 1 + maxSystemImages;
	/*WAP Download supported, only turn on when no file manager app.*/
#if defined(__MMI_WAP__)
	downloadedImagesCnt = GetDownloadImagesInfo(&downloadedImages);
	g_phb_cntx.total_image_id += downloadedImagesCnt;
#endif

	/*System Default Image*/
	g_phb_cntx.image_name_list[0] = (PU8)GetString(STR_GLOBAL_NONE);
	g_phb_cntx.image_id_list[0] = IMG_PHB_DEFAULT;
#ifndef	__MMI_RESOURCE_SLIM_ON_128X160__
	for(j = 0; j < maxSystemImages; ++j)
	{
		g_phb_cntx.image_name_list[++i] = (PU8)GetString((U16)(STR_PICTURE_1+j));
		g_phb_cntx.image_id_list[i] = IMG_PHB_PICTURE1 + j;
	}
#endif	
	#if 0
	for(j = 0; j < fullScrFactoryImagesCnt; ++j)
	{
		g_phb_cntx.image_name_list[++i] = fullScrFactoryImages[j].ImageName;
		g_phb_cntx.image_id_list[i] = fullScrFactoryImages[j].ImagesId;
	}

	for(j = 0; j < associatedFactoryImagesCnt; ++j)
	{
		g_phb_cntx.image_name_list[++i] = associatedFactoryImages[j].ImageName;
		g_phb_cntx.image_id_list[i] = associatedFactoryImages[j].ImagesId;
	}
	#endif
	/*WAP Download supported, only turn on when no file manager app.*/
#if defined(__MMI_WAP__)
	for(j = 0; j < downloadedImagesCnt; ++j)
	{
		/*CSD 2004-03-16 Wilson, Check Image Size Before Associate*/
		image = get_image(downloadedImages[j].ImagesId);
		coolsand_UI_measure_image(image, &w, &h);

		if( (w <= 30) && (h <= 30))	/*Shold use define value in the future.*/
		{
			g_phb_cntx.image_name_list[++i] = downloadedImages[j].ImageName;
			g_phb_cntx.image_id_list[i] = downloadedImages[j].ImagesId;
		}
		else
			g_phb_cntx.total_image_id--;
	}
#endif

	/*Select Image From File Manager*/
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	g_phb_cntx.total_image_id++;
	g_phb_cntx.image_name_list[++i] = (U8*)GetString(STR_ID_PHB_FMGR_IMAGE_SELECT_FILE);
	g_phb_cntx.image_id_list[i] = 1;
#endif
    mmi_trace(g_sw_PHB, "-----+++++Line: %d;total_image_id=%d+++++-----", __LINE__, g_phb_cntx.total_image_id);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_build_caller_group_list()
* DESCRIPTION
*   Builds caller group list for inline selection
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   global image list for phonebook.
*****************************************************************************/
void mmi_phb_build_caller_group_list(void)
{
	U8 i;

	PHB_CALLER_GROUP_STRUCT *callerGroups;

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;

	myCallerGrpnames[0] = (PU8)GetString(STR_GLOBAL_NONE);
	for ( i = 1; i <= MAX_PB_CALLER_GROUPS; i++)
		myCallerGrpnames[i] = callerGroups[i-1].groupName;
}

#if defined(__MMI_INCOMING_CALL_VIDEO__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_build_video_list()
* DESCRIPTION
*   Builds Video list for inline selection
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   global video list for phonebook.
*****************************************************************************/
void mmi_phb_build_video_list(void)
{
	U8 count = 0;
	//U16 i;
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

	/*System Default video*/
	g_phb_cntx.video_name_list[count] = (U8*)GetString(STR_GLOBAL_NONE);
	g_phb_cntx.video_id_list[count] = 0;
	g_phb_cntx.total_video_id = 1;

// dingjian 20080407 Tmp
#if 0
	for(i = 0 ; i < 1; i++)
	{
		g_phb_cntx.video_name_list[++count] = (U8*)GetString((U16)(STR_ID_PHB_VIDEO_1+i));
		g_phb_cntx.video_id_list[count] = VDO_ID_PHB_MTCALL_1 + i;
		g_phb_cntx.total_video_id++;
	}
#endif 

	/*Select From File*/
	g_phb_cntx.video_name_list[++count] = (U8*)GetString(STR_ID_PHB_FMGR_IMAGE_SELECT_FILE);
	g_phb_cntx.video_id_list[count] = 0x8000;
	g_phb_cntx.total_video_id++;

}
/*****************************************************************************
* FUNCTION
*  mmi_phb_get_video_index()
* DESCRIPTION
*   Retrieve video list index by video id.
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*
*****************************************************************************/
U8 mmi_phb_get_video_index(U16 video_id)
{
	U8 i;

	/*An external video resource*/
	if(video_id & 0x8000)
	{
		return (U8)(g_phb_cntx.total_video_id-1);
	}

	/*An internal video resource, mask it to get resource id*/
	video_id &= 0x3FFF;

	if(video_id)
	{
		for(i = 0 ; i < (g_phb_cntx.total_video_id - 1); i++)
		{
			if(video_id == g_phb_cntx.video_id_list[i])
			{
				return i;
			}
		}
	}

	return 0;
}

#endif


void mmi_phb_build_black_list_list(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

    /*Setting Open */
    g_phb_cntx.black_list_name_list[0] = (U8*)GetString(STR_GLOBAL_OFF);
    g_phb_cntx.black_list_id_list[0] = 0;

    /*Setting Close*/
    g_phb_cntx.black_list_name_list[1] = (U8*)GetString(STR_GLOBAL_ON);
    g_phb_cntx.black_list_id_list[1] = 1;
    
    g_phb_cntx.total_black_list_id = 2;
}

void mmi_phb_build_red_list_list(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

    /*Setting Open */
    g_phb_cntx.red_list_name_list[0] = (U8*)GetString(STR_GLOBAL_OFF);
    g_phb_cntx.red_list_id_list[0] = 0;

    /*Setting Close*/
    g_phb_cntx.red_list_name_list[1] = (U8*)GetString(STR_GLOBAL_ON);
    g_phb_cntx.red_list_id_list[1] = 1;
    
    g_phb_cntx.total_red_list_id = 2;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_ring_tone_index()
* DESCRIPTION
*   Gets the index of the current ringtone in ringtone list
*
* PARAMETERS
*  ringToneID	IN	ring tone id  for lookup
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_get_ring_tone_index(U16 ringToneId)
{
	U8 i;
	if(ringToneId)
	{
		for(i = 0; i < g_phb_cntx.total_ring_tone; ++i)
		{
			if(g_phb_cntx.ring_tone_list[i] == ringToneId)
				break;
		}
		return i;
	}
	else
		return 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_image_index()
* DESCRIPTION
*   Gets the index of the current image tag in  images list
*
* PARAMETERS
*  imageID	IN	image for lookup
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_get_image_index(U16 imageId)
{
	U8 i;
	if(imageId)
	{
		for(i = 0; i < g_phb_cntx.total_image_id; ++i)
		{
			if(g_phb_cntx.image_id_list[i] == imageId)
				break;
		}
		return i;
	}
	else
		return 0;
}

U8 mmi_phb_get_black_list_index(U16 black_list_Id)
{
    U8 i;
    
    if(black_list_Id)
    {
        for(i = 0; i < g_phb_cntx.total_black_list_id; ++i)
        {
            if(g_phb_cntx.black_list_id_list[i] == black_list_Id)
            break;
        }
        return i;
    }
    else
    {
        return 0;
    }
}

U8 mmi_phb_get_red_list_index(U16 black_list_Id)
{
    U8 i;
    
    if(black_list_Id)
    {
        for(i = 0; i < g_phb_cntx.total_black_list_id; ++i)
        {
            if(g_phb_cntx.black_list_id_list[i] == black_list_Id)
            break;
        }
        return i;
    }
    else
    {
        return 0;
    }
}

#endif /* _PHONEBOOKCALLERGROUP_C */


