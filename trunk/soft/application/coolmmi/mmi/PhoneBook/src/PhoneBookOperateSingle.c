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
 * PhoneBookOperateSingle.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for phonebook single entry operation.
 *   Such as Add, Delete, Edit.
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
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-06-13, Li Tao      fix bug P78000000081: controll the length of the edit for SIM PhoneNumber
  02.01.002  2007-06-19, Li Tao      fixbug 65,67,68, if the end user only input the "+", it will return false
 ************************************************************************/
 
#ifndef _PHONEBOOKOPERATESINGLE_C
#define _PHONEBOOKOPERATESINGLE_C

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "debuginitdef.h"	/*debug info*/
#include "protocolevents.h"
#include "thrdosinterface.h"
#include "frameworkstruct.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_cm.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebooktypes.h"
#include "queuegprot.h"
#include "mainmenudef.h"
#include "unicodexdcl.h"
#include "globaldefs.h"
#include "timerevents.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "nvramenum.h"
#include "nvram_data_items.h"
#include "settinggprots.h"
#include "settingdefs.h"  /*csd 2004-02-26, for fdn on message*/
#include "settingsgdcl.h"
#include "gdi_include.h"			/* graphic library */
#include "wgui_categories_multimedia.h"
#include "commonscreens.h"
#include "simdetectiondef.h"
#include "messagesexdcl.h"
#include "callmanagementgprot.h"

#include "gbhmi.h"   //added by zhoumn for the bug of editor full @2007/06/16
#include "globalmenuitems.h"
#include "vcard.h"

#if defined(__MMI_FILE_MANAGER__)
#include "filemanagerdef.h"		/* error string id */
#endif

#include "filemgr.h"  /* should include filemgr.h before profilemgrgprot.h */
#include "fat_fs.h"
#include "filesystemdef.h"
#include "filemanagergprot.h"

#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
#include "profilemgrgprot.h"
#endif

#if defined(__MMI_VRSD_DIAL__)
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "vrsddial.h"
#endif

#if defined(__MMI_VRSI__)
#include "mdi_datatype.h"
#include "vrsienum.h"
#include "vrsitype.h"
#include "vrsiprot.h"
#endif

#if defined(__J2ME__)
#include "phonebookjavahandler.h"
#endif

#if defined(__MMI_INCOMING_CALL_VIDEO__)
#include "datamgr.h"
#include "vdoplygprot.h"
#endif

#if defined(__MMI_VCARD__)
#include "vobjects.h"
#endif

#ifdef __FLIGHT_MODE_SUPPORT__
#include "simdetectiongexdcl.h"
#endif

#ifdef __MMI_PHB_SEND_SMS_FROM_PHB__
#include "messagesmiscell.h"         //for ismessagesreentering
#include "messagesresourcedata.h"    //for str_sms_msg_not_ready_yet
#endif

// Add adp trace
#include "mmi_trace.h"
#include "callhistorygprots.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#endif
#include "simdetectiongprot.h"
#include "managecallmain.h"

#undef __NEWSIMULATOR
#if defined(__MMI_MAINLCD_220X176__)
extern void ShowCategoryPopMenuScreen(
								U16 left_softkey,
								U16 left_softkey_icon,
								U16 right_softkey,
								U16 right_softkey_icon,
								S32 number_of_items,	
								U16 *list_of_items,
								U16 *list_of_icons,
								S32 highlighted_item,
								U8 *history_buffer)	;
#endif
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
U8	g_phb_edit_email_addr = 0;
#endif
/*
** Local Variable
*/

U8 gRingtoneItemIndex;
U8* myCallerGrpnames[MAX_PB_CALLER_GROUPS+1];
pBOOL viewOnce = FALSE;

UINT32 copyEntry = FALSE;
UINT32 moveEntry = FALSE;
UINT32 duplicateEntry = FALSE;
#ifdef __MMI_MULTI_SIM__
MMI_PHB_STORAGE_LOCATION phbcopymovedes = MMI_STORAGE_NONE;
MMI_PHB_STORAGE_LOCATION phbcopymovesrc = MMI_STORAGE_NONE;
#endif
static U16 moveEntryStoreIndex;
static U8 moveModified = 0;

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
static BOOL bday_modified;
static U16 bday_inline_index;
#endif

#if defined(__MMI_FILE_MANAGER__)
S8 g_phb_file_path[FMGR_MAX_PATH_LEN*ENCODING_LENGTH+ENCODING_LENGTH];	/*Select file from file manager*/
extern S8 nPrintableStr[];	/*Temp global buffer for convert name*/
#endif

#if defined(__MMI_INCOMING_CALL_VIDEO__)
U8 g_phb_video_item_index = 0xff;
S8 g_phb_video_file_path[FMGR_MAX_PATH_LEN*ENCODING_LENGTH+ENCODING_LENGTH];	/*Select file from file manager*/
BOOL g_phb_video_view = FALSE;
static mmi_data_record_store_type g_phb_video_record_store; /*Video Path Object*/
static S8 phb_caller_video_file_path[FMGR_MAX_PATH_LEN*ENCODING_LENGTH+ENCODING_LENGTH];	/*caller video*/
#endif

/*
** Global Variable
*/
extern U16 PhoneBookEntryCount;
extern S8 pbName[];
extern S8 pbNumber[];
extern S8 pbHomeNumber[];
extern S8 pbCompanyName[];
extern S8 pbEmailAddress[];
extern S8 pbOfficeNumber[];
extern S8 pbFaxNumber[];
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
extern S8	pbBday[];
extern S8	pbDay[];
extern S8	pbMon[];
extern S8	pbYear[];
#endif

extern MMI_PHB_ENTRY_BCD_STRUCT		PhoneBook[];

extern wgui_inline_item wgui_inline_items[];
extern U8 gPictureItemIndex;

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
extern PHB_OPTIONAL_FIELDS_STRUCT	PhoneBookOptionalFields;
#endif

extern U16 TitleStrings[];
extern U16 g_phb_name_index[];
extern U8 g_phb_enter_from;

#if defined(__MMI_PHB_EMAIL_SORT__)
U8 g_phb_email_sorted = 0;
#endif

/*
** Global Function
*/
#if defined(__IP_NUMBER__)
extern void SetTempUseIPNumber(pBOOL flag);
#endif

extern void GBSetInputboxLSKFunction(FuncPtr f);

/*
** Static Function
*/
#ifdef __MMI_PHB_BIRTHDAY_FIELD__
static void mmi_phb_bday_changed_callback(U16 index);
#endif

/*****************************************************************************
* FUNCTION
*   mmi_phb_clear_old_add_history()
* DESCRIPTION
*  Deletes old PHB add screens from history
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static void mmi_phb_clear_old_add_history(void)
{
	U16 end_scr_id = 0;
	//get the previous screen id of POPUP_SCREENID
	GetPreviousScrnIdOf (GetExitScrnID(), &end_scr_id);

	if (end_scr_id == GetCmMarkerScrnID())
	{
		GetPreviousScrnIdOf (GetCmMarkerScrnID(), &end_scr_id);
		if (end_scr_id)
		{
			if (IsScreenPresent(SCR_STORAGE_LOCATION_MENU))
			{
				DeleteBetweenScreen(SCR_STORAGE_LOCATION_MENU, end_scr_id);
			}
			else if (IsScreenPresent(SCR_ID_PHB_FMGR_SAVE_OPTION))
			{
				DeleteBetweenScreen(SCR_ID_PHB_FMGR_SAVE_OPTION, end_scr_id);
			}
			return;
		}
	}
	if(!DeleteNScrId(SCR_STORAGE_LOCATION_MENU))
	{
		DeleteNScrId(SCR_ID_PHB_FMGR_SAVE_OPTION);	/*Enter From Save image from file manager*/
	}
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_clear_old_edit_history()
* DESCRIPTION
*  Deletes old PHB edit screens from history
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static void mmi_phb_clear_old_edit_history(void)
{
	U16 end_scr_id = 0;
	//get the previous screen id of POPUP_SCREENID
	GetPreviousScrnIdOf (GetExitScrnID(), &end_scr_id);

	if (end_scr_id == GetCmMarkerScrnID())
	{
		GetPreviousScrnIdOf (GetCmMarkerScrnID(), &end_scr_id);
		if (end_scr_id)
		{
			if (IsScreenPresent(SCR_PBOOK_ENTRY_OPTIONS))
			{
				DeleteBetweenScreen(SCR_PBOOK_ENTRY_OPTIONS, end_scr_id);
			}
			else if (IsScreenPresent(SCR_ID_PHB_FMGR_SAVE_OPTION))
			{
				DeleteBetweenScreen(SCR_ID_PHB_FMGR_SAVE_OPTION, end_scr_id);
			}
			return;
		}
	}
	if(DeleteNScrId(SCR_PBOOK_ENTRY_OPTIONS)) ;	/*Enter From Edit Option*/
	else DeleteNScrId(SCR_ID_PHB_FMGR_SAVE_OPTION);	/*Enter From Save image from file manager*/
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_entry_op_remove_invalid_name_and_number
 * DESCRIPTION
 *  remove invalid char before add entry.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_phb_entry_op_remove_invalid_name_and_number(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_phb_name_remove_invalid_character(pbName);
    mmi_phb_number_remove_invalid_character(pbNumber);
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) || defined(__MMI_PHB_USIM_FIELD__)
    mmi_phb_number_remove_invalid_character(pbHomeNumber);
    mmi_phb_number_remove_invalid_character(pbOfficeNumber);
    mmi_phb_number_remove_invalid_character(pbFaxNumber);
#endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) || defined(__MMI_PHB_USIM_FIELD__) */
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_operate_single_init
* DESCRIPTION
*   Register highlight handler and intialization for phonebook single entry operation.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_operate_single_init(void)
{
	U8 i;
	
	/*Add Entry*/
	SetHiliteHandler(MITEM102_PBOOK_NEW_ENTRY		,mmi_phb_highlight_op_add_entry);
	SetHiliteHandler(MITEM_ADD_ENTRY_TO_PHONE		,mmi_phb_highlight_op_add_entry_to_phone);
	SetHintHandler(MITEM_ADD_ENTRY_TO_PHONE			,mmi_phb_hint_op_add_entry_to_phone);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MITEM_ADD_ENTRY_TO_SIM + i, mmi_phb_highlight_op_add_entry_to_sim);
		SetHintHandler(MITEM_ADD_ENTRY_TO_SIM + i, mmi_phb_hint_op_add_entry_to_sim);
	}

	/*View, Edit, Delete, Copy, Move, Duplicate*/
	SetHiliteHandler(MITEM1010_PBOOK_VIEW			,mmi_phb_highlight_op_view_entry);
	SetHiliteHandler(MITEM1011_PBOOK_EDIT			,mmi_phb_highlight_op_edit_entry);
	SetHiliteHandler(MITEM1012_PBOOK_DELETE		,mmi_phb_highlight_op_delete_entry);
	SetHiliteHandler(MITEM1013_PBOOK_COPY_ENTRY	,mmi_phb_highlight_op_copy_entry);
	SetHiliteHandler(MITEM1014_PBOOK_MOVE			,mmi_phb_highlight_op_move_entry);
	SetHiliteHandler(MITEM1015_PBOOK_DUPLICATE	,mmi_phb_highlight_op_duplicate_entry);

	/*Optional Operation*/
#if defined(__MMI_PHB_COMBINE_COPY_DUPLICATE__)	/*Combine copy and duplicate into one option.*/
	SetHiliteHandler(MENU_ID_PHB_OPTION_COPY_DUPLICATE,		mmi_phb_highlight_op_copy_duplicate);
#endif

	SetHiliteHandler(MENU_ID_PHB_OPTION_DIAL	,mmi_phb_highlight_op_dial_number);
#if defined(__MMI_PHB_IP_DIAL_FROM_PHB__)		/*IP Dial from phonebook entry option menu.*/
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MENU_ID_PHB_OPTION_IP_DIAL+i, mmi_phb_highlight_op_ip_number);
	}
#endif

#if defined(__MMI_PHB_SEND_SMS_FROM_PHB__)		/*Send SMS from phonebook entry option menu.*/
	SetHiliteHandler(MENU_ID_PHB_OPTION_SEND_SMS	,mmi_phb_highlight_op_send_sms);
#endif
#ifdef __MMI_PHB_SEND_MMS_FROM_PHB__
	SetHiliteHandler(MENU_ID_PHB_OPTION_SEND_MMS, mmi_phb_highlight_op_send_mms);
#endif

#if defined(__MMI_VCARD__)/*Send vCard*/
	SetHiliteHandler(MENU_ID_PHB_VCARD_SEND_ENTRY,	mmi_phb_highlight_vcard_send_select_card);
#endif

#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	 /*Associate Images Handling*/
	SetHiliteHandler(MENU_ID_PHB_IMAGE_OPTION_VIEW		,mmi_phb_highlight_image_view);
	SetHiliteHandler(MENU_ID_PHB_IMAGE_OPTION_SELECT	,mmi_phb_highlight_image_select);
	SetHiliteHandler(MENU_ID_PHB_IMAGE_OPTION_DELETE	,mmi_phb_highlight_image_delete);

	/*Save Image From File Manager*/
	SetHiliteHandler(MENU_ID_PHB_FMGR_SAVE_NEW		,mmi_phb_highlight_fmgr_add_entry);
	SetHiliteHandler(MENU_ID_PHB_FMGR_SAVE_LIST		,mmi_phb_highlight_fmgr_edit_entry);
	SetHiliteHandler(MENU_ID_PHB_FMGR_SAVE_CALLGRP	,mmi_phb_highlight_edit_callergroup);
#endif

#if defined(__MMI_VRSD_DIAL__)
	SetHiliteHandler(MENU_ID_VRSD_DIAL_PHB_OPTION		,mmi_phb_highlight_op_voice_dial);
#endif

#if defined(__MMI_INCOMING_CALL_VIDEO__)
	SetHiliteHandler(MENU_ID_PHB_VIDEO_DEFAULT_VIEW		,mmi_phb_highlight_video_default_view);
	SetHiliteHandler(MENU_ID_PHB_VIDEO_DEFAULT_SETTING	,mmi_phb_highlight_video_setting);
	SetHiliteHandler(MENU_ID_PHB_VIDEO_FILE_VIEW		,mmi_phb_highlight_video_file_view);
	SetHiliteHandler(MENU_ID_PHB_VIDEO_FILE_SELECT		,mmi_phb_highlight_video_file_select);
	SetHiliteHandler(MENU_ID_PHB_VIDEO_FILE_DELETE		,mmi_phb_highlight_video_file_delete);
	SetHiliteHandler(MENU_ID_PHB_VIDEO_FILE_SETTING		,mmi_phb_highlight_video_setting);

	SetHintHandler(MENU_ID_PHB_VIDEO_DEFAULT_SETTING	,mmi_phb_hint_video_setting);
	SetHintHandler(MENU_ID_PHB_VIDEO_FILE_SETTING		,mmi_phb_hint_video_setting);

	mmi_phb_video_init_data_store();
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_op_option()
* DESCRIPTION
*   Draws the Entry Options menu
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_entry_op_option(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MMI_PHB_OPTION_ENUM_TOTAL];	/*Need Expand when has more options.*/
	U16 nNumofItem;
	U32 maskingByte=-1;
#if defined(__MMI_MAINLCD_220X176__)
	U16	ItemIcons[MAX_SUB_MENUS];
	memset(ItemIcons,0,sizeof(ItemIcons));
#endif
#if defined(__MMI_VRSD_DIAL__) || defined(__MMI_PHB_SEND_MMS_FROM_PHB__)
	U32 HideItemId = -1;
	mmi_phb_operate_single_init(); /*Regiser highlight handler again.*/
#endif

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_op_option.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_PBOOK_ENTRY_OPTIONS, mmi_phb_exit_op_option, mmi_phb_entry_op_option, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_PBOOK_ENTRY_OPTIONS);

#ifdef __MMI_MULTI_SIM__
	#ifdef __IP_NUMBER__
 	SetIPMenuOption(MENU_ID_PHB_OPTION_IP_DIAL);
	#endif

	mmi_frm_unhide_menu_item(MENU_ID_PHB_OPTION_SEND_SMS);
	if (0 == MTPNP_AD_Get_UsableSide_Number())
	{
		mmi_frm_hide_menu_item(MENU_ID_PHB_OPTION_SEND_SMS);		
	}
#endif/*__MMI_MULTI_SIM__*/

	if (HasValidSIM()
#ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
#endif
  	)
	{	
		nNumofItem = GetNumOfChild_Ext(MITEM101_PBOOK_VIEW_OPTIONS);
		GetSequenceStringIds_Ext(MITEM101_PBOOK_VIEW_OPTIONS, nStrItemList);
		SetParentHandler(MITEM101_PBOOK_VIEW_OPTIONS);
		#ifdef __MMI_MULTI_SIM__
		RegisterHighlightHandler( MultiSimMenuHighLightHandler );
		#else
		RegisterHighlightHandler( ExecuteCurrHiliteHandler);
		#endif

	#if defined(__MMI_TOUCH_SCREEN__) && defined(__MMI_WGUI_CSK_ENABLE__)
		#ifdef __FLIGHT_MODE_SUPPORT__
		if (mmi_bootup_get_active_flight_mode() != 1)
		#endif
		{
			#ifdef __MMI_MULTI_SIM__
			if (MTPNP_AD_Get_UsableSide_Number() > 0)
			{
				EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
			}
			#else
			EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
			#endif
		
		}
	#endif
	#if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
		DrawSimulateCenterKey();
	#endif

		/*Hide Menu Item "Voice Tag" when In Call.*/
	#if defined(__MMI_VRSD_DIAL__)
		HideItemId = GetChildMenuIDIndexByParentMenuID(MITEM101_PBOOK_VIEW_OPTIONS,MENU_ID_VRSD_DIAL_PHB_OPTION);
		if(HideItemId != -1)
		{
			if(GetTotalCallCount())
			{
				ResetBit(maskingByte,HideItemId);
			}
		}
	#endif
	#if defined(__MMI_PHB_SEND_MMS_FROM_PHB__)
		HideItemId = GetChildMenuIDIndexByParentMenuID(MITEM101_PBOOK_VIEW_OPTIONS, MENU_ID_PHB_OPTION_SEND_MMS);
		if (HideItemId!=-1)
		{
			if(GetTotalCallCount())
			{
				ResetBit(maskingByte, HideItemId);
			}
		}
	#endif
	}
	else
	{
		nNumofItem = GetNumOfChild_Ext(MITEM101_PBOOK_VIEW_OPTIONS_NO_SIM);
		GetSequenceStringIds_Ext(MITEM101_PBOOK_VIEW_OPTIONS_NO_SIM, nStrItemList);
		SetParentHandler(MITEM101_PBOOK_VIEW_OPTIONS_NO_SIM);
		RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	}
	MaskHiliteItems(MITEM101_PBOOK_VIEW_OPTIONS, maskingByte);
	nNumofItem = MaskItems(nStrItemList,(U8)nNumofItem,maskingByte);
#if defined(__MMI_MAINLCD_220X176__)
	ShowCategoryPopMenuScreen(STR_GLOBAL_OK,
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,
							IMG_GLOBAL_BACK,
							nNumofItem,
							nStrItemList,
							ItemIcons,
							0,
							guiBuffer);
#else
	ShowCategory15Screen(STR_SCR_VIEW_OPTIONS_CAPTION, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);
#endif
#if defined(__MMI_TOUCH_SCREEN__) && defined(__MMI_WGUI_CSK_ENABLE__)
	if (HasValidSIM()
		#ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
		#endif
	)
	{
	#ifdef __MMI_MULTI_SIM__
		MTPNP_PFAL_CC_SetCenterFunc(mmi_phb_choose_number_normal);
		SetCenterSoftkeyFunction(MTPNP_PFAL_CC_CSKHandler, KEY_EVENT_UP);
	#else  /* __MMI_MULTI_SIM__ */
		SetCenterSoftkeyFunction(mmi_phb_choose_number_normal, KEY_EVENT_UP);
	#endif /* __MMI_MULTI_SIM__ */

	}
#else
	#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_SetCenterFunc(mmi_phb_choose_number_normal);
	#endif
#endif

#if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
  	)
	{
		SimulateCenterKeyDailFunction(mmi_phb_choose_number_normal, KEY_EVENT_UP);
		SetNeedCenterKeyFlag(TRUE);
	}

#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_op_option()
* DESCRIPTION
*   Exit function for mmi_phb_entry_op_option()
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_exit_op_option(void)
{
	g_phb_cntx.end_scr_id = SCR_PBOOK_ENTRY_OPTIONS;
#if defined(__MMI_TOUCH_SCREEN__) && defined(__MMI_WGUI_CSK_ENABLE__)
	if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
  	)
	{
		ResetCenterSoftkey();
	}
#endif /* __MMI_TOUCH_SCREEN__ && __MMI_WGUI_CSK_ENABLE__ */

#if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
  ClearSimulateCenterKeyDailhandlers();
  SetNeedCenterKeyFlag(FALSE);
#endif
}

#define MMI_PHB_OP_ADD
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_op_add_entry()
* DESCRIPTION
*   Highlight handler for "Add New Entry" in PHB main menu
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_highlight_op_add_entry(void)
{
	memset(pbNumber, 0, ENCODING_LENGTH);
	g_phb_cntx.set_done_flag = 0;

	SetLeftSoftkeyFunction(mmi_phb_op_add_pre_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_op_add_pre_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_op_add_entry_to_sim()
* DESCRIPTION
*   Highlight handler for "To SIM" in Add Entry menu
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_highlight_op_add_entry_to_sim(void)
{
	U8 nSimID = 0;
	#ifdef __MMI_MULTI_SIM__
	nSimID = GetReqSrcModIndex();
	mmi_trace(g_sw_PHB, "mmi_phb_highlight_op_add_entry_to_sim, nSimID is %d", nSimID);
	#endif
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_phb_cntx.selected_storage = MMI_SIM + nSimID;

	if(g_phb_cntx.sim_used[nSimID] >= g_phb_cntx.sim_total[nSimID])
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_add_entry,KEY_EVENT_UP);
		SetKeyHandler(mmi_phb_entry_op_add_entry,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_op_add_entry_to_phone()
* DESCRIPTION
*   Highlight handler for "To Phone" in Add Entry menu
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_highlight_op_add_entry_to_phone(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_phb_cntx.selected_storage = MMI_NVRAM;

	if(g_phb_cntx.phone_used >= g_phb_cntx.phone_total)
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_add_entry,KEY_EVENT_UP);
		SetKeyHandler(mmi_phb_entry_op_add_entry,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_op_add_entry_to_sim()
* DESCRIPTION
*   Hint handler for "To SIM" in Add Entry menu
*
* PARAMETERS
*  index IN  current index
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_hint_op_add_entry_to_sim(U16 index)
{
	S8 simStatus[40];
	U8 nSimID = 0;
	#ifdef __MMI_MULTI_SIM__
	nSimID = GetReqSrcModeIndexByIndex((U8)index);
	#endif
	memset((S8*)hintData[index], 0, sizeof(hintData[index]));
	mmi_phb_memory_status_get_sim(simStatus, nSimID);
	pfnUnicodeStrcpy((S8*) hintData[index], simStatus);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_hint_op_add_entry_to_phone()
* DESCRIPTION
*   Hint handler for "To Phone" in Add Entry menu
*
* PARAMETERS
*  index IN  current index
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_hint_op_add_entry_to_phone(U16 index)
{
	S8 phoneStatus[40];

	memset((S8*)hintData[index], 0, sizeof(hintData[index]));
	mmi_phb_memory_status_get_phone(phoneStatus);
	pfnUnicodeStrcpy((S8*) hintData[index], phoneStatus);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_add_pre_entry()
* DESCRIPTION
*   Checks the phone book list is empty or not
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_op_add_pre_entry(void)
{
	U8 i;
	U16 simTotal = 0;
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_add_pre_entry.>\n",__FILE__,__LINE__));
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		simTotal += g_phb_cntx.sim_total[i];
	}
	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		if(PhoneBookEntryCount == (simTotal + g_phb_cntx.phone_total))
		{
			DisplayPopup((PU8)GetString(STR_PBOOK_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
		else
		{
			mmi_phb_util_clear_buffer(FALSE);
			mmi_phb_entry_op_add_choose_storage();
		}
	}
	else
		mmi_phb_entry_not_ready();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_op_add_choose_storage()
* DESCRIPTION
*   Draws the storage location menu for user to choose storage for new entry.
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
#ifdef __MMI_MULTI_SIM__
U8 mmi_phb_get_highlight_item_index_for_add_new_menu(void)
{
	U8 i;
	BOOL bFound =  MMI_FALSE;
	U8 hilitedStorage = 0;
	U8 j = 0;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		mmi_frm_hide_menu_item(MITEM_ADD_ENTRY_TO_SIM + i);
		if(MTPNP_PFAL_Is_Card_Usable(i))
		{
			mmi_frm_unhide_menu_item(MITEM_ADD_ENTRY_TO_SIM + i);
			SetReqSrcModeIndex(j++, i);
		}
	}
	//search according to prefer storage
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_phb_cntx.prefer_storage == MMI_SIM + i)
		{
			if((!MTPNP_PFAL_Is_CardAbsent(i))&&g_phb_cntx.sim_used[i] < g_phb_cntx.sim_total[i])
			{
				hilitedStorage = GetIndexOfStringId_Ext(MITEM102_PBOOK_NEW_ENTRY, MITEM_ADD_ENTRY_TO_SIM+i);
				bFound = MMI_TRUE;
				break;
			}
		}
	}
	if(!bFound && g_phb_cntx.prefer_storage == MMI_NVRAM&&g_phb_cntx.phone_used < g_phb_cntx.phone_total)
	{
		hilitedStorage = GetIndexOfStringId_Ext(MITEM102_PBOOK_NEW_ENTRY, MITEM_ADD_ENTRY_TO_PHONE);
		bFound = MMI_TRUE;
	}
	//search one usable
	if(!bFound)
	{
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			if((!MTPNP_PFAL_Is_CardAbsent(i))&&g_phb_cntx.sim_used[i] < g_phb_cntx.sim_total[i])
			{
				hilitedStorage = GetIndexOfStringId_Ext(MITEM102_PBOOK_NEW_ENTRY, MITEM_ADD_ENTRY_TO_SIM+i);
				break;
			}
		}
	}
    
	if(hilitedStorage > 0)
	{
		hilitedStorage--;
	}
        
	return hilitedStorage;
}
#endif
void mmi_phb_entry_op_add_choose_storage(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MMI_SIM_NUMBER + 1];
	U16 nImgIltemList[MMI_SIM_NUMBER + 1];
	U8*	nHintList[MMI_SIM_NUMBER +1];
	U16 nNumofItem;
	U8 hilitedStorage = 0;
	#ifndef __MMI_MULTI_SIM__
	U8 hilight_sim, highlight_phone;
	#endif
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_op_add_choose_storage.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_STORAGE_LOCATION_MENU, mmi_phb_exit_op_add_choose_storage, mmi_phb_entry_op_add_choose_storage, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_STORAGE_LOCATION_MENU);

#ifdef __MMI_MULTI_SIM__
	//MTPNP_AD_SetMenuHideUnhide(MITEM_ADD_ENTRY_TO_SIM);
	hilitedStorage	= mmi_phb_get_highlight_item_index_for_add_new_menu();
#else
	/*Switch To Default Memory Storage*/
	hilight_sim = GetIndexOfStringId(MITEM102_PBOOK_NEW_ENTRY, MITEM_ADD_ENTRY_TO_SIM);
	highlight_phone = GetIndexOfStringId(MITEM102_PBOOK_NEW_ENTRY, MITEM_ADD_ENTRY_TO_PHONE);
	if(g_phb_cntx.prefer_storage == MMI_SIM)
	{
		if((g_phb_cntx.sim_used[0] < g_phb_cntx.sim_total[0]) || (g_phb_cntx.phone_used == g_phb_cntx.phone_total))
			hilitedStorage = hilight_sim;
		else
			hilitedStorage = highlight_phone;
	}
	else
	{
		if((g_phb_cntx.phone_used < g_phb_cntx.phone_total) || (g_phb_cntx.sim_used[0] == g_phb_cntx.sim_total[0]))
			hilitedStorage = highlight_phone;
		else
			hilitedStorage = hilight_sim;
	}
#endif
	
	nNumofItem = GetNumOfChild_Ext(MITEM102_PBOOK_NEW_ENTRY);
	GetSequenceStringIds_Ext(MITEM102_PBOOK_NEW_ENTRY, nStrItemList);
	GetSequenceImageIds_Ext(MITEM102_PBOOK_NEW_ENTRY, nImgIltemList);
	SetParentHandler(MITEM102_PBOOK_NEW_ENTRY);
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif
	ConstructHintsList(MITEM102_PBOOK_NEW_ENTRY, nHintList);

	ShowCategory52Screen(STR_CHOOSE_STORAGE_CAPTION, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, nImgIltemList,
						  nHintList, 0, hilitedStorage, guiBuffer);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_op_add_choose_storage()
* DESCRIPTION
*   Exit function for mmi_phb_entry_op_add_choose_storage()
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_exit_op_add_choose_storage(void)
{
	g_phb_cntx.end_scr_id = SCR_STORAGE_LOCATION_MENU;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_fill_inline_item
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
void mmi_phb_op_fill_inline_item(U8 field_index, U8 item_index,U16 store_index, U16*string_list, U16* image_list)
{
	U8 nameLength;
	U8 numberLength;
  S8 pbNumberTemp[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
  memset(pbNumberTemp,0,sizeof(pbNumberTemp));
	switch(field_index)
	{
		/*--- Name Field ---*/
		case MMI_PHB_FIELD_NAME:

			/*Assign name length*/
			if( (store_index == MAX_PB_ENTRIES && g_phb_cntx.selected_storage >= MMI_SIM && g_phb_cntx.selected_storage <= MMI_SIM4) ||	/*Add*/
			     (store_index < MAX_PB_ENTRIES && store_index >= MAX_PB_PHONE_ENTRIES)		/*Edit*/
				)		
			{
				U8 nSimIndex = 0;
				if(g_phb_cntx.selected_storage >= MMI_SIM && g_phb_cntx.selected_storage <= MMI_SIM4)
				{
					nSimIndex = g_phb_cntx.selected_storage - MMI_SIM;
				}
				else
				{
					nSimIndex = (store_index - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES;
				}
				nameLength = g_phb_cntx.sim_name_len[nSimIndex];
			}
			else
			{
				nameLength = MAX_PB_NAME_LENGTH;
			}

			SetInlineItemActivation(&wgui_inline_items[item_index], KEY_LSK, KEY_EVENT_UP);
#if defined(__PHB_0x81_SUPPORT__)
			SetInlineItemFullScreenEdit_ext(&wgui_inline_items[item_index],STR_ID_PHB_NAME,IMG_SCR_PBOOK_CAPTION,(PU8)pbName, nameLength+1,
				(INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_USE_ENCODING_BASED_LENGTH|INPUT_TYPE_ONE_LESS_CHARACTER|INPUT_TYPE_GSM_EXTENDED_HANDLING), INPUT_TYPE_EXT_USE_0x81_ENCODING|INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL);
#else
			SetInlineItemFullScreenEdit_ext(&wgui_inline_items[item_index],STR_ID_PHB_NAME,IMG_SCR_PBOOK_CAPTION,(PU8)pbName, nameLength+1,
				(INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_USE_ENCODING_BASED_LENGTH|INPUT_TYPE_ONE_LESS_CHARACTER|INPUT_TYPE_GSM_EXTENDED_HANDLING), INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL);
#endif
			SetInlineFullScreenEditCustomFunction(&wgui_inline_items[item_index], mmi_phb_set_full_scr_lsk);
			string_list[item_index] = STR_ID_PHB_NAME;
			image_list[item_index] = IMG_NAME;
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
			SetInlineItemTextEditDefaultItemNameIfEmpty(&wgui_inline_items[item_index], string_list[item_index]);
#endif

			break;

		/*--- Mobile Number Field ---*/
		case MMI_PHB_FIELD_NUMBER:
			/*Assign number length, 2007-06-13, Li Tao*/
			if( (store_index == MAX_PB_ENTRIES && g_phb_cntx.selected_storage >= MMI_SIM && g_phb_cntx.selected_storage <= MMI_SIM4) ||	/*Add*/
			     (store_index < MAX_PB_ENTRIES && store_index >= MAX_PB_PHONE_ENTRIES)		/*Edit*/
			)
			{
				numberLength = MAX_SIM_NUMBER_LENGTH + 1;
			        //add by chenq for bug 6573 B
			        memcpy(pbNumberTemp, pbNumber,(MAX_PB_NUMBER_LENGTH/2)*ENCODING_LENGTH);
			        memset(pbNumber,0,(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH);
			        pfnUnicodeStrcpy(pbNumber, pbNumberTemp);
			        //add by chenq for bug 6573 E
			       SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
				/*update number length, 2007-06-13, Li Tao*/
				//SetInlineItemTextEdit(&wgui_inline_items[item_index], (PU8)pbNumber, MAX_PB_NUMBER_LENGTH+1, INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING );
				SetInlineItemTextEdit(&wgui_inline_items[item_index], (PU8)pbNumber, numberLength, INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING );

			}
			else
			{
				numberLength = MAX_PB_NUMBER_LENGTH + 1;
			
				SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

				/*update number length, 2007-06-13, Li Tao*/
				//SetInlineItemTextEdit(&wgui_inline_items[item_index], (PU8)pbNumber, MAX_PB_NUMBER_LENGTH+1, INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING );
				SetInlineItemTextEdit(&wgui_inline_items[item_index], (PU8)pbNumber, numberLength, INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING );
			}
			DisableInlineItemHighlight(&wgui_inline_items[item_index]);
			string_list[item_index] = STR_ID_PHB_MOBILE_NUMBER;
			image_list[item_index] = IMG_MOBILE_NUMBER;
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
			SetInlineItemTextEditDefaultItemNameIfEmpty(&wgui_inline_items[item_index], string_list[item_index]);
#endif

			break;

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
		/*--- Home Number Field ---*/
	 	case MMI_PHB_FIELD_HOME:
			SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
			SetInlineItemTextEdit(&wgui_inline_items[item_index], (PU8)pbHomeNumber,MAX_PB_NUMBER_LENGTH+1,INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING);
			DisableInlineItemHighlight(&wgui_inline_items[item_index]);
			string_list[item_index] = STR_HOME_NUMBER;
			image_list[item_index] = IMG_HOME_NUMBER;
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
			SetInlineItemTextEditDefaultItemNameIfEmpty(&wgui_inline_items[item_index], string_list[item_index]);
#endif

			break;

		/*--- Company Name Field ---*/
	 	case MMI_PHB_FIELD_COMPANY_NAME:

			SetInlineItemActivation(&wgui_inline_items[item_index], KEY_LSK, KEY_EVENT_UP);
			SetInlineItemFullScreenEdit(&wgui_inline_items[item_index],STR_COMPANY_CAPTION,IMG_SCR_PBOOK_CAPTION,(PU8)pbCompanyName,MAX_PB_COMPANY_LENGTH+1, INPUT_TYPE_ALPHANUMERIC_UPPERCASE);
			SetInlineFullScreenEditCustomFunction(&wgui_inline_items[item_index], mmi_phb_set_full_scr_lsk);
			string_list[item_index] = STR_COMPNAY_NAME;
			image_list[item_index] = IMG_COMPANY_NAME;
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
			SetInlineItemTextEditDefaultItemNameIfEmpty(&wgui_inline_items[item_index], string_list[item_index]);
#endif

			break;

		/*--- Email Field ---*/
	 	case MMI_PHB_FIELD_EMAIL:
			SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
			SetInlineItemTextEdit(&wgui_inline_items[item_index], (PU8)pbEmailAddress,MAX_PB_EMAIL_LENGTH+1,INPUT_TYPE_ALPHANUMERIC_LOWERCASE|INPUT_TYPE_USE_ONLY_ENGLISH_MODES);
			DisableInlineItemHighlight(&wgui_inline_items[item_index]);
			string_list[item_index] = STR_EMAIL_ADRESS;
			image_list[item_index] = IMG_EMAIL_ADDRESS;
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
			SetInlineItemTextEditDefaultItemNameIfEmpty(&wgui_inline_items[item_index], string_list[item_index]);
#endif
			EnableInlineInputMethodHints();
			break;
		/*--- Office Number Field ---*/
	 	case MMI_PHB_FIELD_OFFICE:
 			SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
			SetInlineItemTextEdit(&wgui_inline_items[item_index], (PU8)pbOfficeNumber,MAX_PB_NUMBER_LENGTH+1,INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING);
			DisableInlineItemHighlight(&wgui_inline_items[item_index]);
			string_list[item_index] = STR_OFFICE_NUMBER;
			image_list[item_index] = IMG_OFFICE_NUMBER;
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
			SetInlineItemTextEditDefaultItemNameIfEmpty(&wgui_inline_items[item_index], string_list[item_index]);
#endif

			break;
		/*--- Fax Number Field ---*/
	 	case MMI_PHB_FIELD_FAX:
			SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
			SetInlineItemTextEdit(&wgui_inline_items[item_index], (PU8)pbFaxNumber,MAX_PB_NUMBER_LENGTH+1,INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING);
			DisableInlineItemHighlight(&wgui_inline_items[item_index]);
			string_list[item_index]  = STR_FAX_NUMBER;
			image_list[item_index] = IMG_FAX_NUMBER;
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
			SetInlineItemTextEditDefaultItemNameIfEmpty(&wgui_inline_items[item_index], string_list[item_index]);
#endif

			break;
#endif /*END #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)*/

		/*--- Caller Picture Field ---*/
	 	case MMI_PHB_FIELD_PIC:
			SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
			SetInlineItemSelect(&wgui_inline_items[item_index], g_phb_cntx.total_image_id, g_phb_cntx.image_name_list, &g_phb_cntx.selected_pic_index);
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
			RegisterInlineSelectHighlightHandler(&wgui_inline_items[item_index], mmi_phb_image_highlight_hdlr);
#endif
			string_list[item_index]  = STR_ASSOCIATE_PICTURE;
			image_list[item_index] = IMG_ID_PHB_CALLER_PIC;

			gPictureItemIndex = item_index;

			break;
		/*--- Caller Ring Tone Field ---*/
	 	case MMI_PHB_FIELD_RING:
			SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
			SetInlineItemSelect(&wgui_inline_items[item_index], g_phb_cntx.total_ring_tone, g_phb_cntx.ring_tone_name_list, &g_phb_cntx.selected_ring_index);
			RegisterInlineSelectHighlightHandler(&wgui_inline_items[item_index], mmi_phb_ring_tone_highlight_hdlr);
			string_list[item_index] = STR_ASSOCIATE_SOUND;
			image_list[item_index] = IMG_RING_TONE;

			break;
		/*--- Caller Group Field ---*/
	 	case MMI_PHB_FIELD_GROUP:
			SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
			SetInlineItemSelect(&wgui_inline_items[item_index], MAX_PB_CALLER_GROUPS + 1, myCallerGrpnames, &g_phb_cntx.selected_grp_index);
			string_list[item_index] = STR_CALLER_GROUP;
			image_list[item_index] = IMG_CALLER_GROUP;

			break;
#if defined(__MMI_INCOMING_CALL_VIDEO__)
		/*--- Caller Video Field ---*/
	 	case MMI_PHB_FIELD_VIDEO:
			SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
			SetInlineItemSelect(&wgui_inline_items[item_index], g_phb_cntx.total_video_id, g_phb_cntx.video_name_list, &g_phb_cntx.selected_video_index);
			RegisterInlineSelectHighlightHandler(&wgui_inline_items[item_index], mmi_phb_video_highlight_hdlr);

			string_list[item_index]  = STR_ID_PHB_CALLER_VIDEO;
			image_list[item_index] = IMG_ID_PHB_CALLER_VIDEO_1;

			g_phb_video_item_index = item_index;

			break;
#endif
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
		/*--- Birthday Field ---*/
	 	case MMI_PHB_FIELD_BIRTHDAY:
			RegisterInlineItemChangedNotification(mmi_phb_bday_changed_callback);
			bday_inline_index = item_index;
			SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
/*Guojian Del Start For 9825 Ver: GreenStone  on 2008-10-15 18:30 */
			#if 0
			if (bday_modified == FALSE)
			{
				MYTIME	current_time;
				S8	 	l_time_string[6];
				GetDateTime(&current_time);
				sprintf((S8*)l_time_string,"%04d", current_time.nYear);
				AnsiiToUnicodeString((S8*)pbYear, l_time_string);
				sprintf((S8*)l_time_string,"%02d", current_time.nMonth);
				AnsiiToUnicodeString((S8*)pbMon, l_time_string);
				sprintf((S8*)l_time_string,"%02d", current_time.nDay);
				AnsiiToUnicodeString((S8*)pbDay, l_time_string);
			}
			#endif
/*Guojian Del End  For 9825 Ver: GreenStone  on 2008-10-15 18:30 */
    /*wangrui add and reopen del code begin 20081201 for fixbug10497 */

/*			S8	UCS2_Day[3*ENCODING_LENGTH];
			S8	UCS2_Mon[3*ENCODING_LENGTH];
			S8	UCS2_Year[6*ENCODING_LENGTH];
			AnsiiToUnicodeString(UCS2_Year, (S8*)MMI_PHB_BDAY_YEAR_STR);
			AnsiiToUnicodeString(UCS2_Mon, (S8*)MMI_PHB_BDAY_MON_STR);
			AnsiiToUnicodeString(UCS2_Day, (S8*)MMI_PHB_BDAY_DAY_STR);

			mmi_trace(1, "bday_modified= %d", bday_modified);
			
			if ((pfnUnicodeStrcmp((const S8*)pbYear, (const S8*)UCS2_Year) == 0 &&
				pfnUnicodeStrcmp((const S8*)pbMon, (const S8*)UCS2_Mon) == 0 &&
				pfnUnicodeStrcmp((const S8*)pbDay, (const S8*)UCS2_Day) == 0) || 
				(pfnUnicodeStrcmp((const S8*)pbYear, (const S8*)"") == 0 &&
				pfnUnicodeStrcmp((const S8*)pbMon, (const S8*)"") == 0 &&
				pfnUnicodeStrcmp((const S8*)pbDay, (const S8*)"") == 0))
			{
				bday_modified = FALSE;
			}
			else
			{
				bday_modified = TRUE;
			}			
			
		*/		
		    /*wangrui add and reopen del code end */
			SetInlineItemDate(&wgui_inline_items[item_index],(U8*)pbDay,(U8*)pbMon,(U8*)pbYear, mmi_phb_bday_editor_callback);
			DisableInlineItemHighlight(&wgui_inline_items[item_index]);
			set_inline_date_boundary(MMI_PHB_BDAY_MAX_YEAR_INT, 1900, 12, 1);

			string_list[item_index] = STR_ID_PHB_BIRTHDAY;
			image_list[item_index]= IMG_ID_PHB_BIRTHDAY;
			break;
#endif
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_reconfig_inline_item
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
void mmi_phb_op_reconfig_inline_item(U8* guiBuffer)
{
	/*Reconfig picture field*/
	if((gPictureItemIndex != MAX_PB_PICTURES_TAGS) && viewOnce) /*Reconfig only if view fields has turned on*/
	{
		viewOnce = FALSE;

		/*If select from file but no image selected, clear it to default*/
		if( (g_phb_cntx.selected_pic_index == g_phb_cntx.total_image_id - 1) && \
		    (g_phb_cntx.image_location == MMI_PHB_IMAGE_NO_SELECT))
		{
			g_phb_cntx.selected_pic_index = 0;
		}
		else if( g_phb_cntx.selected_pic_index != g_phb_cntx.total_image_id - 1)
		{
			g_phb_cntx.selected_pic_index = g_phb_cntx.selected_pic_in_view;
		}

		ReConfigureInlineItemSelect(&wgui_inline_items[gPictureItemIndex], g_phb_cntx.total_image_id, g_phb_cntx.image_name_list, &g_phb_cntx.selected_pic_index);
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
		RegisterInlineSelectHighlightHandler(&wgui_inline_items[gPictureItemIndex], mmi_phb_image_highlight_hdlr);
#endif
		SetInlineDoneFlag(guiBuffer);
	}

	/*Reconfig video field*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	if((g_phb_video_item_index != MAX_PB_VIDEO_TAGS) && g_phb_video_view) /*Reconfig only if view fields has turned on*/
	{
		g_phb_video_view = FALSE;

		if( (g_phb_cntx.selected_video_index == g_phb_cntx.total_video_id - 1) && \
			(g_phb_cntx.video_location == MMI_PHB_IMAGE_NO_SELECT))
		{
			g_phb_cntx.selected_video_index = 0;
		}

		ReConfigureInlineItemSelect(&wgui_inline_items[g_phb_video_item_index], g_phb_cntx.total_video_id, g_phb_cntx.video_name_list, &g_phb_cntx.selected_video_index);
		RegisterInlineSelectHighlightHandler(&wgui_inline_items[g_phb_video_item_index], mmi_phb_video_highlight_hdlr);
		SetInlineDoneFlag(guiBuffer);
	}
#endif

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_op_add_entry()
* DESCRIPTION
*   Draws the new entry screen.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_op_add_entry(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
	U16 DetailPbNameImageList[MAX_PB_FIELDS];
	U16 image_id;
	U8 field_index, item_index = 0;;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_op_add_entry.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_ADD_NEW_ENTRY, mmi_phb_exit_op_add_entry, NULL, NULL);
	InitializeCategory57Screen();
	guiBuffer = GetCurrGuiBuffer(SCR_ADD_NEW_ENTRY);
	inputBuffer = GetCurrNInputBuffer(SCR_ADD_NEW_ENTRY, &inputBufferSize);

	RegisterHighlightHandler(mmi_phb_op_edit_highlight_hdlr);

#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
	EnableInlineItemDefaultText();
#endif
	mmi_phb_op_fill_inline_item(MMI_PHB_FIELD_NAME, item_index++, MAX_PB_ENTRIES, TitleStrings, DetailPbNameImageList);
	mmi_phb_op_fill_inline_item(MMI_PHB_FIELD_NUMBER, item_index++, MAX_PB_ENTRIES, TitleStrings, DetailPbNameImageList);
	gPictureItemIndex = 0xff;	/*Set to default before use.*/
	gRingtoneItemIndex = 0xff;
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	g_phb_video_item_index = 0xff;
#endif

	if (g_phb_cntx.selected_storage >= MMI_SIM && g_phb_cntx.selected_storage <= MMI_SIM4)
	{
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		mmi_phb_build_caller_group_list();
		mmi_phb_op_fill_inline_item(MMI_PHB_FIELD_GROUP, item_index++,MAX_PB_ENTRIES, TitleStrings, DetailPbNameImageList);
#endif
		if(inputBuffer!=NULL)
		{
			SetCategory57Data(wgui_inline_items, item_index, inputBuffer);
		}

		//image_id = IMG_ID_CARD1_PHB_STORAGE_SIM;
#ifdef __MMI_MULTI_SIM__
		image_id = IMG_STORAGE_SIM_1 + g_phb_cntx.selected_storage - MMI_SIM;
#else
		image_id = IMG_STORAGE_SIM;
#endif
	}
	else
	{	
		mmi_phb_build_image_list();
		
		mmi_phb_build_ring_tone_list();
		mmi_phb_build_caller_group_list();
		#if 1
#if defined(__MMI_INCOMING_CALL_VIDEO__)
		mmi_phb_build_video_list();

		if(guiBuffer == NULL) /*First Time Enter*/
		{
			if(g_phb_cntx.set_done_flag != 2)	/*Not Enter From File Manager*/
			{
				g_phb_cntx.video_location = MMI_PHB_IMAGE_NO_SELECT;
			}
			g_phb_cntx.video_audio = 1;
			g_phb_video_view = FALSE;
		}

		if( g_phb_cntx.video_location != MMI_PHB_IMAGE_NO_SELECT)	/*Change Inline select prompt message.*/
			g_phb_cntx.video_name_list[g_phb_cntx.total_video_id - 1] = (U8*)GetString(STR_ID_PHB_FMGR_IMAGE_SELECTED);
#endif
	#endif
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
		if((guiBuffer == NULL) && (g_phb_cntx.set_done_flag != 2)) /*First Time Enter and Not Enter From File Manager*/
				g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;

		if( g_phb_cntx.image_location != MMI_PHB_IMAGE_NO_SELECT)	/*Change Inline select prompt message.*/
			g_phb_cntx.image_name_list[g_phb_cntx.total_image_id - 1] = (U8*)GetString(STR_ID_PHB_FMGR_IMAGE_SELECTED);
#endif

		/*Fill inline item for each field*/
		for(field_index = (MMI_PHB_FIELD_NUMBER + 1); field_index < MMI_PHB_FIELD_TOTAL; field_index++)
		{	
			if(g_phb_cntx.view_field[field_index - (MMI_PHB_FIELD_NUMBER + 1)])
			{
				mmi_phb_op_fill_inline_item(field_index, item_index++, MAX_PB_ENTRIES, TitleStrings, DetailPbNameImageList);
			}
		}

		if(inputBuffer!=NULL)
		{
			SetCategory57Data(wgui_inline_items, item_index, inputBuffer);
			mmi_phb_op_reconfig_inline_item(guiBuffer);
		}

		image_id = IMG_STORAGE_HANDSET;
	}

	ShowCategory57Screen(STR_SCR_ADD_NEW_ENTRY_CAPTION,
								   image_id,
								   STR_GLOBAL_OK,
								   IMG_GLOBAL_OK,
								   STR_GLOBAL_BACK,
								   IMG_GLOBAL_BACK,
								   item_index,
								   DetailPbNameImageList,
								   wgui_inline_items,
								   0,
								   guiBuffer);

	if(g_phb_cntx.set_done_flag)
	{
		g_phb_cntx.set_done_flag = 0;
		set_wgui_inline_list_menu_changed();
	}
#if defined(__MMI_GB__)
	mmi_frm_set_highlight_handler();
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone);  //add by yaosq for leftsoft 20061102
#endif
	SetCategory57RightSoftkeyFunctions(mmi_phb_entry_op_add_entry_confirm,GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_op_add_entry()
* DESCRIPTION
*   Exit function for mmi_phb_entry_op_add_entry()
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_exit_op_add_entry(void)
{
	history_t	h;
	U16 inputBufferSize;

	g_phb_cntx.end_scr_id = SCR_ADD_NEW_ENTRY;
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	g_phb_edit_email_addr = 0;
#endif
	mmi_phb_stop_ring_tone_id();
	CloseCategory57Screen();
	h.scrnID = SCR_ADD_NEW_ENTRY;
	h.entryFuncPtr = mmi_phb_entry_op_add_entry;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data((U8*) h.inputBuffer);
	AddNHistory(h, inputBufferSize);
}

/*****************************************************************************
* FUNCTION
* mmi_phb_op_check_valid_number()
* DESCRIPTION
*   Check if number fields contain invalid '+' character.
*
* PARAMETERS
*  number	IN	input number string
* RETURNS
*  TRUE, if number format is correct.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
BOOL mmi_phb_op_check_valid_number(S8* number)
{
	U16 i = 0;
	S8 ch;

	if(pfnUnicodeStrlen(number) == 0)
		return TRUE;
//del by chenq for bug 6815 B
 	//2007-06-19, Li Tao, fix bug 65,67,68, if the end user only input the "+", it will return false
//	if (number[0] == '+' && pfnUnicodeStrlen(number) == 1)
//		return FALSE;
//del by chenq for bug 6815 E
	while(number[i] != 0x00 || number[i+1] != 0x00)
	{
		ch = number[i];

		/*Can not contain '+' sign in the middle of number string.*/
		if(!((ch <= '9' && ch >= '0') ||
			   ch == 'w' ||
			   ch == 'W' ||
			   ch == 'p' ||
			   ch =='P' ||
			   ch =='*' ||
			   ch =='#' ||
			   (i == 0 && ch == '+')))
		{
			return FALSE;
		}

		i = i + 2;
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
* mmi_phb_op_check_pre_save()
* DESCRIPTION
*   Checks for mandatory fields for phonebook entry
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
BOOL mmi_phb_op_check_pre_save(void)
{
	U16 str_id = 0;

	/*Require both name "and" numbe fields*/
#if defined(__MMI_PHB_NAME_AND_NUM_REQUIRE__)
	if(!pfnUnicodeStrlen(pbName)  && !pfnUnicodeStrlen(pbNumber))
	{
		str_id = STR_ID_PHB_NAME_NUM_REQUIRE;
	}
	else if(!pfnUnicodeStrlen(pbName))
	{
		str_id = STR_ID_PHB_NAME_REQUIRE;
	}
	else if(!pfnUnicodeStrlen(pbNumber))
	{
		str_id = STR_ID_PHB_NUM_REQUIRE;
	}
#else	/*Require Name "or" Number field*/
	if(!pfnUnicodeStrlen(pbName) && !pfnUnicodeStrlen(pbNumber))
	{
		str_id = STR_ID_PHB_NAME_NUM_REQUIRE;
	}
#endif
	else if(!mmi_phb_op_check_valid_number(pbNumber) ||
		!mmi_phb_op_check_valid_number(pbHomeNumber) ||
		!mmi_phb_op_check_valid_number(pbOfficeNumber) ||
		!mmi_phb_op_check_valid_number(pbFaxNumber))
	{
		str_id = STR_ID_PHB_NUM_INVALID;
	}

	if(str_id != 0)
	{
		DisplayPopup((PU8)GetString(str_id), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
* mmi_phb_op_add_pre_save()
* DESCRIPTION
*   Checks for mandatory fields in a new entry
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_op_add_pre_save(void)
{
	if(!mmi_phb_op_check_pre_save())
	{
		return;
	}

	mmi_phb_op_add_entry_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_op_add_entry_confirm()
* DESCRIPTION
*  Confirms before saving new entry
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_entry_op_add_entry_confirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO,
		get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(mmi_phb_op_add_pre_save, KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_phb_go_back_3_history_and_clear, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_add_entry_req()

* DESCRIPTION
*   Sends the request to the stack to save the information of a phonebook entry.
*
* PARAMETERS
*  void
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_op_add_entry_req(void)
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	module_type	dest_mod_id = MOD_L4C;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_add_entry_req.>\n",__FILE__,__LINE__));
#if defined(__J2ME__)
	if(g_phb_enter_from != MMI_PHB_ENTER_FROM_JAVA )
#endif
	if(g_phb_enter_from != MMI_PHB_ENTER_FROM_OUTLOOK_IMPORT)
	{
		mmi_phb_show_in_progress(STR_GLOBAL_SAVING, IMG_GLOBAL_SAVE);//modified by lin @20070927
	}

	myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));
	myMsgPtr->type = MMI_PHB_PHONEBOOK;
	myMsgPtr->no_data = 1;
	myMsgPtr->list.storage = g_phb_cntx.selected_storage;
	myMsgPtr->list.index = 0xFFFF;
	myMsgPtr->list.record_index = 0xFFFF;

	mmi_phb_op_set_entry_prepare_name_number(myMsgPtr, pbName, pbNumber);
	
#ifdef __MMI_MULTI_SIM__
	 if(g_phb_cntx.selected_storage >= MMI_SIM && g_phb_cntx.selected_storage <= MMI_SIM4)
	{
		dest_mod_id = MOD_L4C + g_phb_cntx.selected_storage - MMI_SIM;
	}
#endif
	mmi_trace(g_sw_PHB, "PHB Func: %s g_phb_cntx.selected_storage = %d", __FUNCTION__, g_phb_cntx.selected_storage);
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_SET_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandler(mmi_phb_op_add_entry_rsp	,PRT_PHB_SET_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);
}

#ifdef JATAAYU_SUPPORT
//2009-3-19 wuzc add for jataayu begin
void mmi_phb_op_add_entry_Directly_req(S8* in_name, S8* in_num)
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_add_entry_req.>\n",__FILE__,__LINE__));

#if defined(__J2ME__)
	if(g_phb_enter_from != MMI_PHB_ENTER_FROM_JAVA )
#endif
	mmi_phb_show_in_progress(STR_GLOBAL_SAVING, IMG_GLOBAL_SAVE);//modified by lin @20070927

	myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));
	myMsgPtr->type = MMI_PHB_PHONEBOOK;
	myMsgPtr->no_data = 1;
	myMsgPtr->list.storage = MMI_SIM;
	myMsgPtr->list.index = 0xFFFF;
	myMsgPtr->list.record_index = 0xFFFF;

	mmi_phb_op_set_entry_prepare_name_number(myMsgPtr, in_name, in_num);

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	Message.oslMsgId = PRT_PHB_SET_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandler(mmi_phb_op_add_entry_rsp	,PRT_PHB_SET_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);
}
//2009-3-19 wuzc add for jataayu end
#endif
/*****************************************************************************
* FUNCTION
*  mmi_phb_op_add_entry_rsp()
* DESCRIPTION
*   Handles the response from stack after the entry is saved.
*
* PARAMETERS
*  info  IN  response structure
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_op_add_entry_rsp(void* info)
{
	U16 store_index;
	MSG_PHB_SET_ENRTY_RSP_STRUCT* localPtr;
	U8 flag = 0;
	U8 num_ascii[MAX_PB_NUMBER_LENGTH+1+1];
	BOOL copy_image = TRUE, copy_video = TRUE;

	U8 bFdl = MMI_FALSE;
	U8 i;

	localPtr = (MSG_PHB_SET_ENRTY_RSP_STRUCT *)info;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_add_entry_rsp.>\n",__FILE__,__LINE__));

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SecuritySetupContext.FdlStatus[i])
		{
			bFdl = MMI_TRUE;
			break;
		}
	}

	if (localPtr->result.flag == 0) /*Set Entry Success*/
	{
		store_index= mmi_phb_op_get_store_index_and_increase_counter(localPtr->list[0].storage, localPtr->list[0].record_index);

		if(localPtr->list[0].storage == MMI_NVRAM)
		{
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
			if(g_phb_cntx.selected_pic_index != g_phb_cntx.total_image_id - 1)	/*De-select the file if selection is not from file.*/
				g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
			else if(g_phb_cntx.image_location == MMI_PHB_IMAGE_NO_SELECT) /*Set selection to none if not select picture from file*/
				g_phb_cntx.selected_pic_index = 0;

			copy_image = mmi_phb_image_update_by_index((U16)(store_index + 1));	/*Copy to folder in file system*/
			if(!copy_image) /*Add image fail.*/
				g_phb_cntx.selected_pic_index = 0;
#endif
			mmi_phb_op_update_optional_field(store_index, MMI_NVRAM, 1);

			/* Populate Optional Field Number*/
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			flag = mmi_phb_op_increase_lookup_table(store_index,
																  NULL,
																  (S8*)PhoneBookOptionalFields.faxNumber,
																  (S8*)PhoneBookOptionalFields.homeNumber,
																  (S8*) PhoneBookOptionalFields.officeNumber);
#endif
			/*Incoming call video setting*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
			copy_video = mmi_phb_video_update_by_index(store_index);
#endif
		}
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		else /*in MMI_SIM*/
		{
			mmi_phb_op_update_optional_field(store_index, localPtr->list[0].storage,1);
		}
#endif

		/*Populate Number Field*/
		if (localPtr->list[0].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR)
		{
			num_ascii [0] = '+';
			memcpy(&num_ascii[1], localPtr->list[0].tel.number, localPtr->list[0].tel.length);
			num_ascii[localPtr->list[0].tel.length+1] = '\0';
			//UnicodeToAnsii(&num_ascii[1], (PS8)pbNumber);//vivian modify forbug 10474
		}
		else
		{
			memcpy(&num_ascii[0], localPtr->list[0].tel.number, localPtr->list[0].tel.length);
			num_ascii[localPtr->list[0].tel.length] = '\0';
			//UnicodeToAnsii((PS8)num_ascii, (PS8)pbNumber);//vivian modify for bug 10474
		}
		flag += mmi_phb_op_increase_lookup_table(store_index,(S8*) num_ascii,NULL,NULL,NULL);

		if(flag)
			mmi_phb_lookup_table_sort();

		mmi_phb_op_set_entry_rsp_name_number(info, store_index);

		if(localPtr->list[0].storage == MMI_NVRAM)
		{
			#if defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			mmi_phb_op_update_field_flag(store_index, (S8*)localPtr->list[0].tel.number, NULL,NULL,NULL,NULL,TRUE);
			#else
			mmi_phb_op_update_field_flag(store_index,
												 (S8*)localPtr->list[0].tel.number,
												 (S8*)PhoneBookOptionalFields.homeNumber,
												 (S8*)PhoneBookOptionalFields.officeNumber,
												 (S8*)PhoneBookOptionalFields.faxNumber,
												 (S8*)PhoneBookOptionalFields.emailAddress,
												 TRUE);
			#endif

		/*Save birthday field*/
			#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
			if (!bday_modified)
			{
				mmi_phb_util_clear_bday_buffer();
			}
			mmi_phb_bday_update_field(store_index);
			ResetInlineItemChangedNotification();
			#endif
			
		}
		else
		{
			mmi_phb_op_update_field_flag(store_index, (S8*)localPtr->list[0].tel.number, NULL,NULL,NULL,NULL,TRUE);
		}

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

		/*add central command for vrsi.*/
#if defined(__MMI_VRSI__)
		mmi_vrsi_ndial_central_add_single(store_index);
#endif

		mmi_phb_clear_old_add_history();

#if defined(__MMI_PHB_UPDATE_CALL_LOG_AFTER_SAVE__)    /*Update Call Log after save number*/
		mmi_phb_convert_get_ucs2_number(pbNumber, store_index);
		if(g_phb_enter_from == MMI_PHB_ENTER_FROM_CALL_LOG &&
						CHISTSyncCallLogNeeded (PhoneBook[store_index].tel.length, PhoneBook[store_index].tel.type, (U8*)pbNumber))
		{
			ReqUpdateCallLogName(PhoneBook[store_index].alpha_id.name_length, PhoneBook[store_index].alpha_id.name_dcs, PhoneBook[store_index].alpha_id.name);
		}
		else /*Not Enter From Call Log*/
#endif
#if defined(__J2ME__)
		if(g_phb_enter_from == MMI_PHB_ENTER_FROM_JAVA )
		{
			mmi_phb_java_write_rsp(MMI_PHB_JAVA_SUCCESS,store_index);
		}
		else
#endif
		{
			/*Data add success, but image or video add fail*/
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

				pfnUnicodeStrcat(str_buff, GetString(STR_ID_PHB_ADD_FAIL));
				DisplayPopup((U8*)str_buff, IMG_GLOBAL_ERROR,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
			}
			else
			{
#if defined(__MMI_VCARD__)
				if(g_phb_enter_from == MMI_PHB_ENTER_FROM_OUTLOOK_IMPORT)
				{
					if(!mmi_vcard_outlook_recv_parse_one_by_one())
					{
						mmi_vcard_outlook_import_done_del_src();
					}
					else
					{
						return;
					}					                        
				}
				else if(g_phb_enter_from == MMI_PHB_ENTER_FROM_VCF_IMPORT)
				{
					DeleteUptoScrID(SCR_ID_VCARD_IMPORT_FROM_OPTION);
					DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
				}
				else    
#endif
				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
			}
		}

		g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;

#if defined(__MMI_VCARD__) && !defined(MMI_ON_WIN32)
		if (g_phb_enter_from == MMI_PHB_ENTER_FROM_IDLE_SCREEN)
		{
			DeleteNScrId(SCR_ID_VOBJ_RECV_OBJECT);
			mmi_vobj_clear_one_file_buffer();
		}
#endif
	}
#if defined(__J2ME__)
	else if(g_phb_enter_from == MMI_PHB_ENTER_FROM_JAVA )
	{
			mmi_phb_java_write_rsp(localPtr->result.cause,0xffff);
	}
#endif
	else /* Set Record Fail.*/
	{         
		if(localPtr->result.cause == MMI_SIM_EF_RECORD_FULL)  /*Number To Long*/
		{
			DisplayPopup((PU8)GetString(STR_NOT_SAVED_NUMBER_TOO_LONG), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
#if defined(__MMI_VCARD__)
			if (g_phb_enter_from == MMI_PHB_ENTER_FROM_IDLE_SCREEN)
			{         
				DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
			}
			else if(g_phb_enter_from == MMI_PHB_ENTER_FROM_OUTLOOK_IMPORT)
			{         
				mmi_vcard_outlook_import_done_del_src();
				mmi_vcard_import_free_buffer();
			}
			else if(g_phb_enter_from == MMI_PHB_ENTER_FROM_VCF_IMPORT)
			{         
				DeleteUptoScrID(SCR_ID_VCARD_IMPORT_FROM_OPTION);
			}            
			else
#endif
			DeleteUptoScrID(SCR_ADD_NEW_ENTRY);
		}

		else if(bFdl)  /*For FDN on Message*/
		{
			DisplayPopup((PU8)GetString(STR_FDL_IS_ON), IMG_GLOBAL_UNFINISHED, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
			mmi_phb_clear_old_add_history();
		}
		else if(localPtr->result.cause == MMI_PHB_CAUSE_CAPACITY_EXCEEDED)
		{
			DisplayPopup((PU8)GetString(STR_ID_PHB_SYSTEM_FULL), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);		
#if defined(__MMI_VCARD__)
			if (g_phb_enter_from == MMI_PHB_ENTER_FROM_IDLE_SCREEN)
			{         
				DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
			}
			else if(g_phb_enter_from == MMI_PHB_ENTER_FROM_OUTLOOK_IMPORT)
			{         
				mmi_vcard_outlook_import_done_del_src();
				mmi_vcard_import_free_buffer();
			}
			else if(g_phb_enter_from == MMI_PHB_ENTER_FROM_VCF_IMPORT)
			{         
				DeleteUptoScrID(SCR_ID_VCARD_IMPORT_FROM_OPTION);
			}            
			else
#endif
			{
				mmi_phb_clear_old_add_history();
			}
		}
		else /* Unknown Error*/
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);		
#if defined(__MMI_VCARD__)
			if (g_phb_enter_from == MMI_PHB_ENTER_FROM_IDLE_SCREEN)
			{         
				DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
			}
			else if(g_phb_enter_from == MMI_PHB_ENTER_FROM_OUTLOOK_IMPORT)
			{         
				mmi_vcard_outlook_import_done_del_src();
				mmi_vcard_import_free_buffer();
			}
			else if(g_phb_enter_from == MMI_PHB_ENTER_FROM_VCF_IMPORT)
			{         
				DeleteUptoScrID(SCR_ID_VCARD_IMPORT_FROM_OPTION);
			}            
			else
#endif
			{
				mmi_phb_clear_old_add_history();
			}		
		}
	}

	g_phb_enter_from = MMI_PHB_ENTER_NONE;
}


/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_op_view_entry()
* DESCRIPTION
*   Highlight handler for "View" in Entry options menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_op_view_entry(void)
{
	SetLeftSoftkeyFunction(mmi_phb_entry_op_view_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_entry_op_view_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_view_fill_item
* DESCRIPTION
*
*
* PARAMETERS
*  a  IN       field index for the enum value
*  b  IN       item index for the inline editor
*  c  IN/OUT   string list for the data of the field
*  d  IN/OUT   string list for title
*  e  IN/OUT   image list
* RETURNS
*  Return TRUE if non-empty field.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static pBOOL mmi_phb_op_view_fill_item(U8 field_index, U8 item_index, S8** entry_list, U16* title_list, U16*image_list)
{
	switch(field_index)
	{
		case MMI_PHB_FIELD_NAME:
			*entry_list = pbName;
			title_list[item_index] = STR_ID_PHB_NAME;
			image_list[item_index] = IMG_NAME;
			break;

		case MMI_PHB_FIELD_NUMBER:
			*entry_list = pbNumber;
			title_list[item_index] = STR_ID_PHB_MOBILE_NUMBER;
			image_list[item_index] = IMG_MOBILE_NUMBER;
			break;

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
		case MMI_PHB_FIELD_HOME:
			*entry_list = pbHomeNumber;
			title_list[item_index] = STR_HOME_NUMBER;
			image_list[item_index] = IMG_HOME_NUMBER;
 			break;

	 	case MMI_PHB_FIELD_COMPANY_NAME:
            *entry_list = pbCompanyName;   
			title_list[item_index] = STR_COMPNAY_NAME;
			image_list[item_index] = IMG_COMPANY_NAME;
 			break;

	 	case MMI_PHB_FIELD_EMAIL:
			*entry_list = pbEmailAddress;
			title_list[item_index] = STR_EMAIL_ADRESS;
			image_list[item_index] = IMG_EMAIL_ADDRESS;
			break;

	 	case MMI_PHB_FIELD_OFFICE:
			*entry_list = pbOfficeNumber;
			title_list[item_index] = STR_OFFICE_NUMBER;
			image_list[item_index] = IMG_OFFICE_NUMBER;
 			break;

	 	case MMI_PHB_FIELD_FAX:
			*entry_list = pbFaxNumber;
			title_list[item_index] = STR_FAX_NUMBER;
			image_list[item_index] = IMG_FAX_NUMBER;
 			break;
#endif
	 	case MMI_PHB_FIELD_PIC:
			if (g_phb_cntx.selected_pic_index == 0)
			{
				return FALSE;
			}
			*entry_list = (PS8)g_phb_cntx.image_name_list[g_phb_cntx.selected_pic_index];
			title_list[item_index] = STR_ASSOCIATE_PICTURE;
			image_list[item_index] = IMG_ID_PHB_CALLER_PIC;
			gPictureItemIndex = item_index;
			return TRUE;

	 	case MMI_PHB_FIELD_RING:
			if (g_phb_cntx.selected_ring_index == 0)
			{
				return FALSE;
			}
			*entry_list = (PS8)g_phb_cntx.ring_tone_name_list[g_phb_cntx.selected_ring_index];
			title_list[item_index] = STR_ASSOCIATE_SOUND;
			image_list[item_index] = IMG_RING_TONE;
			gRingtoneItemIndex = item_index;
			return TRUE;

	 	case MMI_PHB_FIELD_GROUP:
			if (g_phb_cntx.selected_grp_index == 0)
			{
				return FALSE;
			}
			*entry_list = (PS8)myCallerGrpnames[g_phb_cntx.selected_grp_index];
			title_list[item_index] = STR_CALLER_GROUP;
			image_list[item_index] = IMG_CALLER_GROUP;
			return TRUE;

#if defined(__MMI_INCOMING_CALL_VIDEO__)
	 	case MMI_PHB_FIELD_VIDEO:
			if (g_phb_cntx.selected_video_index == 0)
			{
				return FALSE;
			}
			*entry_list = (S8*)g_phb_cntx.video_name_list[g_phb_cntx.selected_video_index];
			title_list[item_index] = STR_ID_PHB_CALLER_VIDEO;
			image_list[item_index] = IMG_ID_PHB_CALLER_VIDEO_1;
			g_phb_video_item_index = item_index;
			return TRUE;
#endif
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
	 	case MMI_PHB_FIELD_BIRTHDAY:
		{
			S8	UCS2_Day[3*ENCODING_LENGTH];
			S8	UCS2_Mon[3*ENCODING_LENGTH];
			S8	UCS2_Year[6*ENCODING_LENGTH];
			AnsiiToUnicodeString(UCS2_Year, (S8*)MMI_PHB_BDAY_YEAR_STR);
			AnsiiToUnicodeString(UCS2_Mon, (S8*)MMI_PHB_BDAY_MON_STR);
			AnsiiToUnicodeString(UCS2_Day, (S8*)MMI_PHB_BDAY_DAY_STR);

			if ((pfnUnicodeStrcmp((const S8*)pbYear, (const S8*)UCS2_Year) == 0 &&
				pfnUnicodeStrcmp((const S8*)pbMon, (const S8*)UCS2_Mon) == 0 &&
				pfnUnicodeStrcmp((const S8*)pbDay, (const S8*)UCS2_Day) == 0) || 
				(pfnUnicodeStrcmp((const S8*)pbYear, (const S8*)"") == 0 &&
				pfnUnicodeStrcmp((const S8*)pbMon, (const S8*)"") == 0 &&
				pfnUnicodeStrcmp((const S8*)pbDay, (const S8*)"") == 0))
			{
				return FALSE;
			}
			else
			{
				bday_modified = TRUE;
				mmi_phb_bday_editor_callback((U8*)pbBday, (U8*) pbDay, (U8*) pbMon, (U8*) pbYear);
				*entry_list = (S8*)pbBday;
				title_list[item_index] = STR_ID_PHB_BIRTHDAY;
				image_list[item_index] = IMG_ID_PHB_BIRTHDAY;
				return TRUE;
			}
		}
#endif
 	}
	if (field_index == MMI_PHB_FIELD_NAME || field_index == MMI_PHB_FIELD_NUMBER ||
		(*entry_list && UCS2Strlen(*entry_list) > 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_op_view_entry()
* DESCRIPTION
*   Displays the details of the selected phonebook entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_op_view_entry(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8* pbEntry[MAX_PB_FIELDS];
	U16 DetailPbNameImageList[MAX_PB_FIELDS];
	U8* guiBuffer;
	U8 field_index, item_index = 0;
	S16 pError;
	U16 store_index;
	U16 image_id;
	PHB_OPTIONAL_IDS_STRUCT* opt_ids = NULL;
#if defined(__MMI_INCOMING_CALL_VIDEO__)
		U16 video_id;
#endif

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_op_view_entry.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_ENTRY_VIEW, mmi_phb_exit_op_view_entry, mmi_phb_entry_op_view_entry, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ENTRY_VIEW);

	RegisterHighlightHandler((void*)mmi_phb_view_entry_highlight_hdlr);

	store_index = g_phb_name_index[g_phb_cntx.active_index];

	pfnUnicodeStrcpy(pbName, (PS8)PhoneBook[store_index].alpha_id.name);
	mmi_phb_convert_get_ucs2_number(pbNumber, store_index);

	mmi_phb_op_view_fill_item(MMI_PHB_FIELD_NAME, item_index, &pbEntry[item_index], TitleStrings, DetailPbNameImageList);
	item_index++;
	mmi_phb_op_view_fill_item(MMI_PHB_FIELD_NUMBER, item_index, &pbEntry[item_index], TitleStrings, DetailPbNameImageList);
	item_index++;

	gPictureItemIndex = 0xff;	/*Set to default before use.*/
	g_phb_cntx.selected_pic_index = 0;
	gRingtoneItemIndex = 0xff;
	g_phb_cntx.selected_ring_index = 0;
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	g_phb_video_item_index = 0xff;
#endif
	if (store_index >= MAX_PB_PHONE_ENTRIES) /* Record store in SIM, SIM index is begin from "MAX_PB_PHONE_ENTRIES" */
	{
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		mmi_phb_build_caller_group_list();
		mmi_phb_read_optional_ids(store_index);
		opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;
		g_phb_cntx.selected_grp_index = opt_ids->callerGroupID;
    // chenqiang modify for bug 6031 B 20070802
    		mmi_trace(g_sw_PHB, "PHB Func:%s selected_grp_index is %d, store_index = %d", __FUNCTION__, g_phb_cntx.selected_grp_index,store_index);
    
		if(mmi_phb_op_view_fill_item(MMI_PHB_FIELD_GROUP, item_index, &pbEntry[item_index], TitleStrings, DetailPbNameImageList))
		{
			item_index++;
		}
    // chenqiang modify for bug 6031 E 20070802
#endif
#ifdef __MMI_MULTI_SIM__
		image_id = IMG_STORAGE_SIM_1 + (store_index - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES;	
#else
		image_id = IMG_STORAGE_SIM;
#endif
	}
	else
	{	
		mmi_phb_build_image_list();
		 
		mmi_phb_build_ring_tone_list();
		
		mmi_phb_build_caller_group_list();
		mmi_phb_read_optional_ids(store_index);
		
		opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;

#if defined(__MMI_INCOMING_CALL_VIDEO__)
		mmi_phb_build_video_list();
#endif

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
		AnsiiToUnicodeString(pbHomeNumber, (PS8)PhoneBookOptionalFields.homeNumber);
              UCS2Strcpy(pbCompanyName, (PS8) PhoneBookOptionalFields.companyName);
		AnsiiToUnicodeString(pbEmailAddress, (PS8)PhoneBookOptionalFields.emailAddress);
		AnsiiToUnicodeString(pbOfficeNumber, (PS8)PhoneBookOptionalFields.officeNumber);
		AnsiiToUnicodeString(pbFaxNumber, (PS8)PhoneBookOptionalFields.faxNumber);
#endif

#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
		if( opt_ids->pictureTagID == 1) /*Change Inline select prompt message.*/
		{
			g_phb_cntx.image_name_list[g_phb_cntx.total_image_id - 1] = (U8*)GetString(STR_ID_PHB_FMGR_IMAGE_SELECTED);
		}
#endif

		g_phb_cntx.selected_pic_index = mmi_phb_get_image_index(opt_ids->pictureTagID);
		g_phb_cntx.selected_ring_index = mmi_phb_get_ring_tone_index(opt_ids->ringToneID);
		g_phb_cntx.selected_grp_index = opt_ids->callerGroupID;
    		mmi_trace(g_sw_PHB, "PHB Func:%s selected_grp_index is %d, store_index = %d", __FUNCTION__, g_phb_cntx.selected_grp_index,store_index);

#if defined(__MMI_INCOMING_CALL_VIDEO__)
		video_id = mmi_phb_video_get_id_by_index(store_index);
		g_phb_cntx.selected_video_index = mmi_phb_get_video_index(video_id);

		/*Change Inline select prompt message.*/
		if( g_phb_cntx.selected_video_index == g_phb_cntx.total_video_id - 1)
		{
			g_phb_cntx.video_name_list[g_phb_cntx.total_video_id - 1] = (U8*)GetString(STR_ID_PHB_FMGR_IMAGE_SELECTED);
		}

		/*Check if can load video path*/
		if(mmi_phb_video_get_path_by_index(video_id))
		{
			g_phb_cntx.video_location = MMI_PHB_IMAGE_SELECT_PATH;
		}
		else
		{
			g_phb_cntx.video_location = MMI_PHB_IMAGE_NO_SELECT;
		}
#endif

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
		mmi_phb_bday_read_data_to_buff(store_index);
#endif

		/*Fill inline item for each field*/
		for(field_index = (MMI_PHB_FIELD_NUMBER + 1); field_index < MMI_PHB_FIELD_TOTAL; field_index++)
		{
			if(g_phb_cntx.view_field[field_index - (MMI_PHB_FIELD_NUMBER + 1)])
			{
				if (mmi_phb_op_view_fill_item(field_index, item_index, &pbEntry[item_index], TitleStrings, DetailPbNameImageList))
				{
					item_index++;
				}
			}
		}
		image_id = IMG_STORAGE_HANDSET;
	}

#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__)
	if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
  	)
	{
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
	}
#endif /* __MMI_TOUCH_SCREEN__ && __MMI_WGUI_CSK_ENABLE__ */

  #if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
 	if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
  	)
	{
		DrawSimulateCenterKey();
	}
		  
  #endif
	ShowCategory84Screen(STR_SCR_ENTRY_VIEW_CAPTION,
								   image_id,
								   0,
								   0,
								   STR_GLOBAL_BACK,
								   IMG_GLOBAL_BACK,
								   item_index,
								   (U8**)pbEntry,
								   DetailPbNameImageList,
								   0,
								   0,
								   guiBuffer);

	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(UI_dummy_function,KEY_ENTER,KEY_EVENT_DOWN);

	if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
  	)
	{
#ifdef __MMI_MULTI_SIM__
		 MTPNP_PFAL_CC_HandleSendKeys(mmi_phb_choose_number_normal,KEY_EVENT_DOWN);
#else
		 SetKeyHandler(mmi_phb_choose_number_normal, KEY_SEND, KEY_EVENT_DOWN);
#endif     

#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__)
#ifndef __MMI_MULTI_SIM__
    SetCenterSoftkeyFunction(mmi_phb_choose_number_normal, KEY_EVENT_UP);
#endif
#endif 


#if ( !(defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
    SimulateCenterKeyDailFunction(mmi_phb_choose_number_normal, KEY_EVENT_UP);
    SetNeedCenterKeyFlag(TRUE);
#endif

#if defined(__IP_NUMBER__) && defined(__MMI_IP_KEY__)
/* under construction !*/
#endif
	}

	/*Check if associate audio in file system exists.*/
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	/*If first time get in and record store in NVRAM*/
	if((guiBuffer == NULL) && (store_index < MAX_PB_PHONE_ENTRIES))
	{
		if(	(opt_ids->ringToneID >= PMG_EXT_MELODY_BEGIN) &&
			( opt_ids->ringToneID <= PMG_EXT_MELODY_END) &&
			!PmgQueryFileExistById(opt_ids->ringToneID))
				DisplayPopup((PU8)GetString(STR_ID_PHB_AUDIO_ID_NOT_EXIST), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_op_view_entry()
* DESCRIPTION
*   Exit function for mmi_phb_entry_op_view_entry()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_op_view_entry(void)
{
	mmi_phb_stop_ring_tone_id();
	g_phb_cntx.end_scr_id = SCR_ENTRY_VIEW;
#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__)
	if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
  	)
	{
		ResetCenterSoftkey();
	}
#endif /* __MMI_TOUCH_SCREEN__ && __MMI_WGUI_CSK_ENABLE__ */

  #if ( !(defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
  ClearSimulateCenterKeyDailhandlers();
  SetNeedCenterKeyFlag(FALSE);
  #endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_op_view_picture()
* DESCRIPTION
*   view select picture.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_op_view_picture(void)
{
	U8* guiBuffer;

	g_phb_cntx.selected_pic_in_view = 0;	/*No Use in Entry View Screen*/

	EntryNewScreen(SCR_VIEW_PICTURE, NULL, mmi_phb_entry_op_view_picture, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_VIEW_PICTURE);

	ShowCategory142Screen((PU8)GetString(STR_VIEW_PICTURE_CAPTION), IMG_SCR_PBOOK_CAPTION,
				0, 0,  STR_GLOBAL_BACK, IMG_GLOBAL_BACK, NULL,
				&g_phb_cntx.image_id_list[g_phb_cntx.selected_pic_index], NULL,1, &g_phb_cntx.selected_pic_in_view, guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_view_entry_highlight_hdlr()
* DESCRIPTION
*   view entry highlight handler.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_view_entry_highlight_hdlr(s32 index)
{
	mmi_phb_stop_ring_tone_id();
	
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	/*View Image In File Folder*/
	if((index == gPictureItemIndex) && (g_phb_cntx.selected_pic_index == (g_phb_cntx.total_image_id - 1)))
	{
		ChangeLeftSoftkey(STR_VIEW_LSK, IMG_VIEW_LSK);
		g_phb_cntx.image_location = MMI_PHB_IMAGE_FOR_ENTRY;
		SetKeyHandler(mmi_phb_image_view_from_phb_folder, KEY_ENTER, KEY_EVENT_UP);
#if defined(__PROJECT_GALLITE_C01__)
		SetKeyHandler(mmi_phb_image_view_from_phb_folder, KEY_LSK, KEY_EVENT_UP);
#endif
	}
	/*View Default Image ID through Category screen*/
	else if((index == gPictureItemIndex) && (g_phb_cntx.selected_pic_index < (g_phb_cntx.total_image_id - 1)))
	{
		ChangeLeftSoftkey(STR_VIEW_LSK, IMG_VIEW_LSK);
		SetKeyHandler(mmi_phb_entry_op_view_picture, KEY_ENTER, KEY_EVENT_UP);
#if defined(__PROJECT_GALLITE_C01__)
		SetKeyHandler(mmi_phb_entry_op_view_picture, KEY_LSK, KEY_EVENT_UP);
#endif
	}
#else
	if(index == gPictureItemIndex && g_phb_cntx.selected_pic_in_view != 0) /*Highlight on Image Field*/
	{
		ChangeLeftSoftkey(STR_VIEW_LSK, IMG_VIEW_LSK);
		SetKeyHandler(mmi_phb_entry_op_view_picture, KEY_ENTER, KEY_EVENT_UP);
#if defined(__PROJECT_GALLITE_C01__)
		SetKeyHandler(mmi_phb_entry_op_view_picture, KEY_LSK, KEY_EVENT_UP);
#endif
	}
#endif
	/*Play Ring Tone if highlight on ringtone field.*/
	else if(index == gRingtoneItemIndex)
	{
		ChangeLeftSoftkey(0, 0);
		mmi_phb_ring_tone_highlight_hdlr(g_phb_cntx.selected_ring_index);
	}
        

	/*Highlight on incoming call video item*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	else if(index == g_phb_video_item_index)
	{
		 if(g_phb_cntx.selected_video_index == (g_phb_cntx.total_video_id - 1))
		 {
			ChangeLeftSoftkey(STR_GLOBAL_VIEW, IMG_GLOBAL_OPTIONS);
			SetKeyHandler(mmi_phb_video_view_from_file, KEY_ENTER, KEY_EVENT_UP);
		 }
		 else if(g_phb_cntx.selected_video_index != 0)
		 {
 			ChangeLeftSoftkey(STR_GLOBAL_VIEW, IMG_GLOBAL_OPTIONS);
			SetKeyHandler(mmi_phb_video_view_from_default, KEY_ENTER, KEY_EVENT_UP);
		 }
		 else
		 {
			ChangeLeftSoftkey(0, 0);
			ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
			ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);
		}
	}
#endif
	else
	{   
		ChangeLeftSoftkey(0, 0);

		 ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);
#ifdef __MMI_MULTI_SIM__
		 MTPNP_PFAL_CC_HandleSendKeys(mmi_phb_choose_number_normal,KEY_EVENT_DOWN);
#else
		 SetKeyHandler(mmi_phb_choose_number_normal, KEY_SEND, KEY_EVENT_DOWN);
#endif  
	}

	ChangeTitleString((PU8)GetString(TitleStrings[index]));
	draw_title();

}


#define MMI_PHB_OP_EDIT
/**************************************************************

	FUNCTION NAME		: mmi_phb_highlight_op_edit_entry()

  	PURPOSE				: Highlight handler for "Edit" in Entry options menu

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_highlight_op_edit_entry(void)
{
	SetLeftSoftkeyFunction(mmi_phb_op_edit_pre_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_op_edit_pre_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_edit_pre_entry
* DESCRIPTION
*   Pre Entry function for phonebook edit.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_edit_pre_entry(void)
{
	mmi_phb_util_clear_buffer(TRUE);
	g_phb_cntx.set_done_flag = 0;
	mmi_phb_entry_op_edit_entry();
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_entry_op_edit_entry()

  	PURPOSE				: Inline edit screen to edit an entry

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_entry_op_edit_entry(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
	U8 field_index, item_index = 0 ;
	S16 pError;
	U16 DetailPbNameImageList[MAX_PB_FIELDS];
	U16 store_index;
	U16 image_id;
	PHB_OPTIONAL_IDS_STRUCT* opt_ids = NULL;
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_op_edit_entry.>\n",__FILE__,__LINE__));

	store_index = g_phb_name_index[g_phb_cntx.active_index];

	EntryNewScreen(SCR_INLINE_EDIT, mmi_phb_exit_op_edit_entry, NULL, NULL);
	InitializeCategory57Screen();
	guiBuffer = GetCurrGuiBuffer(SCR_INLINE_EDIT);
	inputBuffer = GetCurrNInputBuffer(SCR_INLINE_EDIT, &inputBufferSize);

	RegisterHighlightHandler(mmi_phb_op_edit_highlight_hdlr);

	/*Fill name and number field*/
	pfnUnicodeStrcpy(pbName, (PS8)PhoneBook[store_index].alpha_id.name);
	mmi_phb_convert_get_ucs2_number(pbNumber, store_index);
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
	if (!bday_modified)
	{
	mmi_phb_bday_read_data_to_buff(store_index);
	}
#endif
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
	EnableInlineItemDefaultText();
#endif

	mmi_phb_op_fill_inline_item(MMI_PHB_FIELD_NAME, item_index++, store_index, TitleStrings, DetailPbNameImageList);
	mmi_phb_op_fill_inline_item(MMI_PHB_FIELD_NUMBER, item_index++, store_index, TitleStrings, DetailPbNameImageList);

	if (store_index >= MAX_PB_PHONE_ENTRIES) /* Record store in SIM, SIM index is begin from "MAX_PB_PHONE_ENTRIES" */
	{
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		mmi_phb_build_caller_group_list();
		mmi_phb_read_optional_ids(store_index);
		opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;
		g_phb_cntx.selected_grp_index = opt_ids->callerGroupID;

		mmi_phb_op_fill_inline_item(MMI_PHB_FIELD_GROUP, item_index++, store_index, TitleStrings, DetailPbNameImageList);
#endif
		if(inputBuffer!=NULL)
		{
			SetCategory57Data(wgui_inline_items, item_index, inputBuffer);
		}
#ifdef __MMI_MULTI_SIM__
		image_id = IMG_STORAGE_SIM_1 + (store_index - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES;	
#else
		image_id = IMG_STORAGE_SIM;
#endif
	}
	else
	{
	gPictureItemIndex = MAX_PB_PICTURES_TAGS;	/*Set to default before use.*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	g_phb_video_item_index = MAX_PB_VIDEO_TAGS;
#endif
		mmi_phb_build_image_list();
		mmi_phb_build_ring_tone_list();
		mmi_phb_build_caller_group_list();
		mmi_phb_read_optional_ids(store_index);
		opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;

		g_phb_cntx.selected_ring_index = mmi_phb_get_ring_tone_index(opt_ids->ringToneID);
		g_phb_cntx.selected_grp_index = opt_ids->callerGroupID;

		g_phb_cntx.selected_pic_index = mmi_phb_get_image_index(opt_ids->pictureTagID);
#if 1
#if defined(__MMI_INCOMING_CALL_VIDEO__)
		mmi_phb_build_video_list();

		if(guiBuffer == NULL ) /*First Time Enter*/
		{
			U16 video_id = mmi_phb_video_get_id_by_index(store_index);
			g_phb_cntx.selected_video_index = mmi_phb_get_video_index(video_id);

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
#endif
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
		if(guiBuffer == NULL )
		{
			if(opt_ids->pictureTagID == 1)
				g_phb_cntx.image_location = MMI_PHB_IMAGE_FOR_ENTRY;
			else
				g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
		}
		if( g_phb_cntx.image_location != MMI_PHB_IMAGE_NO_SELECT) /*Change Inline select prompt message.*/
			g_phb_cntx.image_name_list[g_phb_cntx.total_image_id - 1] = (U8*)GetString(STR_ID_PHB_FMGR_IMAGE_SELECTED);
#endif


#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
        if (guiBuffer == NULL)  /* First Time Enter */
        {
		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);

		AnsiiToUnicodeString(pbHomeNumber, (PS8)PhoneBookOptionalFields.homeNumber);
		pfnUnicodeStrcpy(pbCompanyName, (PS8)PhoneBookOptionalFields.companyName);
		AnsiiToUnicodeString(pbEmailAddress, (PS8)PhoneBookOptionalFields.emailAddress);
		AnsiiToUnicodeString(pbOfficeNumber, (PS8)PhoneBookOptionalFields.officeNumber);
		AnsiiToUnicodeString(pbFaxNumber, (PS8)PhoneBookOptionalFields.faxNumber);
        }
#endif

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
		mmi_phb_bday_read_data_to_buff(store_index);
#endif

		/*Fill inline item for each field*/
		for(field_index = (MMI_PHB_FIELD_NUMBER + 1); field_index < MMI_PHB_FIELD_TOTAL; field_index++)
		{
			if(g_phb_cntx.view_field[field_index - (MMI_PHB_FIELD_NUMBER + 1)])
			{
				mmi_phb_op_fill_inline_item(field_index, item_index++,store_index, TitleStrings, DetailPbNameImageList);
			}
		}


		if(inputBuffer!=NULL)
		{
			SetCategory57Data(wgui_inline_items, item_index, inputBuffer);
			mmi_phb_op_reconfig_inline_item(guiBuffer);

		}

		image_id = IMG_STORAGE_HANDSET;
	}

	ShowCategory57Screen(STR_SCR_INLINE_EDIT_CAPTION,
								   image_id,
								   STR_GLOBAL_OPTIONS,
								   IMG_GLOBAL_OPTIONS,
								   STR_GLOBAL_BACK,
								   IMG_GLOBAL_BACK,
								   item_index,
								   DetailPbNameImageList,
								   wgui_inline_items,
								   0,
								   guiBuffer);
#if defined(__MMI_GB__)
	mmi_frm_set_highlight_handler();
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone); 
#endif
	SetCategory57RightSoftkeyFunctions(mmi_phb_entry_op_edit_save_confirm,GoBackHistory);

	if(g_phb_cntx.set_done_flag)
	{
		g_phb_cntx.set_done_flag = 0;
		set_wgui_inline_list_menu_changed();
	}

	/*Check if associate audio in file system exists.*/
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	/*If first time get in and record store in NVRAM*/
	if((guiBuffer == NULL) && (store_index < MAX_PB_PHONE_ENTRIES))
	{
		if(	(opt_ids->ringToneID >= PMG_EXT_MELODY_BEGIN) &&
			( opt_ids->ringToneID <= PMG_EXT_MELODY_END) &&
			!PmgQueryFileExistById(opt_ids->ringToneID))
				DisplayPopup((PU8)GetString(STR_ID_PHB_AUDIO_ID_NOT_EXIST), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}
#endif
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_exit_op_edit_entry()

  	PURPOSE				: Exit function for mmi_phb_entry_op_edit_entry()

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_exit_op_edit_entry(void)
{
	history_t	h;
	U16 inputBufferSize;

	g_phb_cntx.end_scr_id = SCR_INLINE_EDIT;

	mmi_phb_stop_ring_tone_id();
	CloseCategory57Screen();
	h.scrnID = SCR_INLINE_EDIT;
	h.entryFuncPtr = mmi_phb_entry_op_edit_entry;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data((U8*) h.inputBuffer);
	AddNHistory(h, inputBufferSize);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_op_edit_highlight_hdlr()

  	PURPOSE				: Highlight handler for inline edit screen

	INPUT PARAMETERS	: S32 index

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  and stopping the Ringtone & LED pattern

**************************************************************/
void mmi_phb_op_edit_highlight_hdlr(S32 index)
{
	mmi_phb_stop_ring_tone_id();
	mmi_phb_stop_led_pattern_id();	/*But Not LED in phonebook inline setting */
	ChangeTitleString((PU8)GetString(TitleStrings[index]));

	draw_title();
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	g_phb_edit_email_addr = 0;
	if(index == 4)
	{
		g_phb_edit_email_addr = 1;
	}
#endif

	/*Highlight on assoicate picture item*/
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
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_entry_op_edit_view_picture()

  	PURPOSE				: Shows the preview of images in the list

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_entry_op_edit_view_picture(void)
{
	U8* guiBuffer;

	g_phb_cntx.selected_pic_in_view = g_phb_cntx.selected_pic_index;
	viewOnce = TRUE;

 	EntryNewScreen(SCR_VIEW_PICTURE, mmi_phb_exit_op_edit_view_picture, mmi_phb_entry_op_edit_view_picture, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_VIEW_PICTURE);

	/*The last index is select from File Manager, just show total image - 1 files*/
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	ShowCategory142Screen((PU8)GetString(STR_VIEW_PICTURE_CAPTION), IMG_SCR_PBOOK_CAPTION,
				STR_GLOBAL_OK, IMG_GLOBAL_OK,
				0, 0, NULL, g_phb_cntx.image_id_list,  NULL,g_phb_cntx.total_image_id - 1, &g_phb_cntx.selected_pic_in_view, guiBuffer);
#else
	ShowCategory142Screen((PU8)GetString(STR_VIEW_PICTURE_CAPTION), IMG_SCR_PBOOK_CAPTION,
				STR_GLOBAL_OK, IMG_GLOBAL_OK,
				0, 0, NULL, g_phb_cntx.image_id_list,  NULL,g_phb_cntx.total_image_id, &g_phb_cntx.selected_pic_in_view, guiBuffer);
#endif
	SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
 }

/**************************************************************

	FUNCTION NAME		: mmi_phb_exit_op_edit_view_picture()

  	PURPOSE				: Exit function for mmi_phb_entry_op_edit_view_picture()

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_exit_op_edit_view_picture(void)
{
	g_phb_cntx.end_scr_id = SCR_VIEW_PICTURE;
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_op_edit_pre_save()

  	PURPOSE				: Checks if the entry can be saved

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_op_edit_pre_save(void)
{
	if(!mmi_phb_op_check_pre_save())
	{
		return;
	}

	mmi_phb_op_edit_entry_req();
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_entry_op_edit_save_confirm()

  	PURPOSE				: Confirms before saving the entry

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_entry_op_edit_save_confirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO,
			get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(mmi_phb_op_edit_pre_save, KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_phb_go_back_2_history, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: PbookEditEntryReq()

 
						  edited by the user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_op_edit_entry_req(void)
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	U16 store_index;
	U8 storage;
	module_type	dest_mod_id = MOD_L4C;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_edit_entry_req.>\n",__FILE__,__LINE__));

	mmi_phb_show_in_progress(STR_GLOBAL_SAVING, IMG_GLOBAL_SAVE);

	store_index = g_phb_name_index[g_phb_cntx.active_index];
#ifdef __MMI_MULTI_SIM__
	if (store_index >= MAX_PB_PHONE_ENTRIES)
	{
		storage = MMI_SIM + (store_index - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES;
	}
	else
	{
		storage = MMI_NVRAM;
	}	
#else
	storage = (store_index >= MAX_PB_PHONE_ENTRIES) ? MMI_SIM : MMI_NVRAM;
#endif	

	myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));
	myMsgPtr->type = MMI_PHB_PHONEBOOK;
	myMsgPtr->no_data = 1;
	myMsgPtr->list.storage = storage;
	myMsgPtr->list.index = 0xffff;  
#ifdef __MMI_MULTI_SIM__
	if (store_index >= MAX_PB_PHONE_ENTRIES)
	{
		myMsgPtr->list.record_index = store_index + 1 - MAX_PB_PHONE_ENTRIES - MAX_PB_SIM_ENTRIES*(storage - MMI_SIM);
	}
	else
	{
		myMsgPtr->list.record_index = store_index + 1;
	}
#else
	myMsgPtr->list.record_index = (storage == MMI_SIM) ?  (store_index + 1) - MAX_PB_PHONE_ENTRIES : (store_index + 1);
#endif	

	mmi_phb_op_set_entry_prepare_name_number(myMsgPtr, pbName, pbNumber);

	#ifdef __MMI_MULTI_SIM__
	 if(storage >= MMI_SIM && storage <= MMI_SIM4)
 	{
 		dest_mod_id = MOD_L4C + storage - MMI_SIM;
 	}
	#endif
	
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_SET_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandler(mmi_phb_op_edit_entry_rsp	,PRT_PHB_SET_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_op_edit_entry_rsp()

  	PURPOSE				: Response handler for entry edit request

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_op_edit_entry_rsp(void* info)
{
	MSG_PHB_SET_ENRTY_RSP_STRUCT* localPtr;
	U16 deletedCnt = 0;
	U8 flag = 0;
	BOOL copy_image = TRUE, copy_video = TRUE;
	U16 store_index;
	U16 record_index;
	U8 storage;

	BOOL bFdl = MMI_FALSE;
	U8 i;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SecuritySetupContext.FdlStatus[i])
		{
			bFdl = MMI_TRUE;
			break;
		}
	}

	localPtr = (MSG_PHB_SET_ENRTY_RSP_STRUCT *)info;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_edit_entry_rsp.>\n",__FILE__,__LINE__));

	if (localPtr->result.flag == 0 /*OK*/)
	{
		store_index = g_phb_name_index[g_phb_cntx.active_index];
	#ifdef __MMI_MULTI_SIM__
		if (store_index >= MAX_PB_PHONE_ENTRIES)
		{
			U8 nSimIndex = (store_index - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES;
			
			storage = MMI_SIM + nSimIndex;
			record_index = store_index + 1 - MAX_PB_PHONE_ENTRIES - MAX_PB_SIM_ENTRIES * nSimIndex;			
		}
		else
		{
			storage = MMI_NVRAM;
			record_index = store_index + 1;
		}	
	#else
		storage = (store_index >= MAX_PB_PHONE_ENTRIES) ? MMI_SIM : MMI_NVRAM;
		record_index = (storage == MMI_SIM) ?  (store_index + 1) - MAX_PB_PHONE_ENTRIES : (store_index + 1);
	#endif	

		PhbUpdateSpeedDial(storage, record_index, MMI_PHB_PHONEBOOK, (U8*)pbName,(U8*)pbNumber);

		/* Clear lookup table first, add later if exists.*/
		deletedCnt = mmi_phb_op_clear_lookup_table_by_index( store_index);

		if(storage == MMI_NVRAM)
		{
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
			if(g_phb_cntx.selected_pic_index != g_phb_cntx.total_image_id - 1)	/*De-select the file if selection is not from file.*/
				g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
			else if(g_phb_cntx.image_location == MMI_PHB_IMAGE_NO_SELECT) /*Set selection to none if not select picture from file*/
				g_phb_cntx.selected_pic_index = 0;

			copy_image = mmi_phb_image_update_by_index(record_index);	/*Update from file manager*/
			if(!copy_image) /*Update image fail.*/
				g_phb_cntx.selected_pic_index = 0;
#endif

			mmi_phb_op_update_optional_field(store_index,MMI_NVRAM, 2);

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			flag = mmi_phb_op_increase_lookup_table(store_index,
																  NULL,
																  (S8*)PhoneBookOptionalFields.faxNumber,
																  (S8*)PhoneBookOptionalFields.homeNumber,
																  (S8*) PhoneBookOptionalFields.officeNumber);
#endif

			/*Incoming call video setting*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
			copy_video = mmi_phb_video_update_by_index(store_index);
#endif
		}
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
		else /*in MMI_SIM*/
		{
			mmi_phb_op_update_optional_field(store_index, storage, 2);
		}
#endif

		flag += mmi_phb_op_increase_lookup_table(store_index,(S8*)localPtr->list[0].tel.number,NULL,NULL,NULL);

		if(deletedCnt || flag)
		{
			mmi_phb_lookup_table_sort();
			g_phb_cntx.lookup_table_count -= deletedCnt;
		}

		mmi_phb_op_set_entry_rsp_name_number(info, store_index);

		/*Update is column exists flag.*/
		if(localPtr->list[0].storage >= MMI_SIM && localPtr->list[0].storage <= MMI_SIM4)
		{
			mmi_phb_op_update_field_flag(store_index, (S8*)localPtr->list[0].tel.number, NULL,NULL,NULL,NULL,FALSE);
		}
		else
		{
#if defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			mmi_phb_op_update_field_flag(store_index, (S8*)localPtr->list[0].tel.number, NULL,NULL,NULL,NULL,FALSE);
#else
			mmi_phb_op_update_field_flag(store_index,
												 (S8*)localPtr->list[0].tel.number,
												 (S8*)PhoneBookOptionalFields.homeNumber,
												 (S8*)PhoneBookOptionalFields.officeNumber,
												 (S8*)PhoneBookOptionalFields.faxNumber,
												 (S8*)PhoneBookOptionalFields.emailAddress,
												 FALSE);
#endif

		/*Save birthday field*/
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
			if (!bday_modified)
			{
				mmi_phb_util_clear_bday_buffer();
			}
			mmi_phb_bday_update_field(store_index);
			ResetInlineItemChangedNotification();
#endif
		}

#if defined(__MMI_PHB_PINYIN_SORT__)	/*Update Pinyin Cache for sort*/
		mmi_phb_util_make_pinyin_cache(store_index);
#endif

		/*Name sorting mapping table should Re-Sort HERE, and get the new highlight position!!!*/
		mmi_phb_op_delete_update_index(store_index);
		g_phb_cntx.new_highlight_entry = mmi_phb_sort_insert_name_index(store_index);
		g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;

		/*Update email sorting mapping table*/
#if defined(__MMI_PHB_EMAIL_SORT__)
		if((localPtr->list[0].storage == MMI_NVRAM) && (g_phb_email_sorted == 1))
		{
			mmi_phb_sort_email_delete_index(store_index);
			mmi_phb_sort_email_insert_index(store_index);
		}
#endif

		/*add central command for vrsi.*/
#if defined(__MMI_VRSI__)
		mmi_vrsi_ndial_central_change_single(store_index);
#endif

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
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
		}
		mmi_phb_clear_old_edit_history();

	}
	else
	{
		if(localPtr->result.cause == MMI_SIM_EF_RECORD_FULL)
		{
			DisplayPopup((PU8)GetString(STR_NOT_SAVED_NUMBER_TOO_LONG), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
			DeleteUptoScrID(SCR_INLINE_EDIT);
		}
		else
		{
			 if(bFdl)  /*For FDN on Message*/
			{
				DisplayPopup((PU8)GetString(STR_FDL_IS_ON), IMG_GLOBAL_UNFINISHED, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
			}
			else
				DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);

			mmi_phb_clear_old_edit_history();
		}
	}
}

#define MMI_PHB_OP_DELETE

/**************************************************************

	FUNCTION NAME		: mmi_phb_highlight_op_delete_entry()

  	PURPOSE				: Highlight handler for "Delete" in Entry options menu

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_highlight_op_delete_entry(void)
{
	SetLeftSoftkeyFunction(mmi_phb_entry_op_delete_confirm,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_op_delete_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);			
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_entry_op_delete_confirm()

  	PURPOSE				: Confirms before deleting a phone book entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_entry_op_delete_confirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_DELETE_QUERY_MSG), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(mmi_phb_op_delete_entry_req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/**************************************************************

	FUNCTION NAME		: mmi_phb_op_delete_entry_req()

  	PURPOSE				: Sends the request to stack to delete
						  a phone book entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_op_delete_entry_req(void)
{
	MSG_PHB_DELETE_ENTRY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	U16 store_index;
	U8 storage;
	module_type	dest_mod_id = MOD_L4C;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_delete_entry_req.>\n",__FILE__,__LINE__));

	mmi_phb_show_in_progress(STR_PHB_DELETING, IMG_GLOBAL_DELETED);

	store_index = g_phb_name_index[g_phb_cntx.active_index];

	storage = PHBGetStorageByStoreIndex(store_index);

	myMsgPtr = (MSG_PHB_DELETE_ENTRY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_DELETE_ENTRY_REQ_STRUCT));

	myMsgPtr->storage = storage;
	myMsgPtr->del_all = MMI_FALSE;
	myMsgPtr->no_data = 1;
	myMsgPtr->type = MMI_PHB_PHONEBOOK;
	
	/*
	*The index means "record_index" here. This change only for MMI_PHB_PHONEBOOK(ADN) type
	* for other type( BDN ,FDN, MSISDN...) This field still need to use index as parameter.
	*/

	myMsgPtr->index = PHBGetADPIndexByStorage(storage, store_index);
	
#ifdef __MMI_MULTI_SIM__
	 if(storage >= MMI_SIM && storage <= MMI_SIM4)
 	{
 		dest_mod_id = MOD_L4C + storage - MMI_SIM;
 	}
#endif
	
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_DEL_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandler(mmi_phb_op_delete_entry_rsp,PRT_PHB_DEL_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);
	DeleteNScrId(SCR_PBOOK_ENTRY_OPTIONS);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_op_delete_entry_rsp()

  	PURPOSE				: Handles the response from stack after
						  the entry is deleted

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_op_delete_entry_rsp(void* info)
{
	MSG_PHB_DELETE_ENTRY_RSP_STRUCT* localPtr;
	U16 deletedCnt = 0;
	//U8 grpIndex;
	//S16 pError;
	U16 store_index;
	U16 record_index;
	U8 storage;
	PHB_OPTIONAL_IDS_STRUCT* opt_ids;
	//PHB_CALLER_GROUP_STRUCT* callerGroups;
	BOOL bFdl = MMI_FALSE;
	U8 i;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SecuritySetupContext.FdlStatus[i])
		{
			bFdl = MMI_TRUE;
			break;
		}
	}

	localPtr = (MSG_PHB_DELETE_ENTRY_RSP_STRUCT *)info;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_delete_entry_rsp.>\n",__FILE__,__LINE__));

	if (localPtr->result.flag == 0 /*OK*/)
	{

		store_index = g_phb_name_index[g_phb_cntx.active_index];
		mmi_trace(g_sw_PHB, "PHB Func: %s store_index = %d", __FUNCTION__,store_index);
		
	#ifdef __MMI_MULTI_SIM__
		if (store_index >= MAX_PB_PHONE_ENTRIES)
		{
			U8 nSimIndex = (store_index - MAX_PB_PHONE_ENTRIES) / MAX_PB_SIM_ENTRIES;
			storage = MMI_SIM + nSimIndex;
			record_index = store_index + 1 - MAX_PB_PHONE_ENTRIES - MAX_PB_SIM_ENTRIES*nSimIndex;			
		}
		else
		{
			storage = MMI_NVRAM;
			record_index = store_index + 1;
		}	
	#else
		storage = (store_index >= MAX_PB_PHONE_ENTRIES) ? MMI_SIM : MMI_NVRAM;
		record_index = (storage == MMI_SIM) ?  (store_index + 1) - MAX_PB_PHONE_ENTRIES : (store_index + 1);
	#endif	

		PhbUpdateSpeedDial(storage, record_index, MMI_PHB_PHONEBOOK, NULL, NULL);

		if(storage >= MMI_SIM && storage <= MMI_SIM4) /* Record store in SIM, SIM index is begin from "MAX_PB_PHONE_ENTRIES" */
		{
			g_phb_cntx.sim_used[storage - MMI_SIM]--;
		}
		else
		{
			g_phb_cntx.phone_used--;
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
		/*Delete image file in file folder*/
			mmi_phb_read_optional_ids(store_index);
			opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;

			if( opt_ids->pictureTagID == 1)
				mmi_phb_image_delete_by_index(record_index);
#endif
			/*Delete incoming call video*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
			mmi_phb_video_delete_by_index(store_index);
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

#if defined(__MMI_VRSD_DIAL__)
		/*remove voice dial tag if exists.*/
		mmi_vrsddial_delete_tag_by_store_index(store_index);
#endif

#if defined(__MMI_VRSI__)
		/*remove voice dial tag if exists.*/
		mmi_vrsi_ndial_central_delete_single(store_index);
#endif

		/*clear phonebook entry and remove from index*/
		mmi_phb_op_delete_update_index(store_index);

		g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;

		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
		if(!PhoneBookEntryCount)
			DeleteNScrId(SCR_PBOOK_LIST);

        /* Change highlight position for deleting the last one */
        if (g_phb_cntx.active_index > (PhoneBookEntryCount - 1))
        {
            g_phb_cntx.new_highlight_entry = 0;
        }
	}
	else if(bFdl)  /*For FDN on Message*/
	{
		DisplayPopup((PU8)GetString(STR_FDL_IS_ON), IMG_GLOBAL_UNFINISHED, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}
	else
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_delete_update_index
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  U16 store_index
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_op_delete_update_index(U16 store_index)
{
	U16 pos,i;

	/*It is no need to clear entry here, because all entry display depends on name index*/
	//memset(&PhoneBook[store_index], 0, sizeof(MMI_PHB_ENTRY_BCD_STRUCT));

	for( pos = 0 ;pos < PhoneBookEntryCount ; pos++)
		if(g_phb_name_index[pos] == store_index)
			break;

	for(i = pos ; i < PhoneBookEntryCount ; i++)
		g_phb_name_index[i] = g_phb_name_index[i+1];

	if (PhoneBookEntryCount > 0)
	{
		PhoneBookEntryCount--;
	}	
}

#define MMI_PHB_OP_COPY_MOVE_DUP
/**************************************************************

	FUNCTION NAME		: mmi_phb_highlight_op_copy_entry()

  	PURPOSE				: Highlight handler for "Copy" in Entry options menu

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_highlight_op_copy_entry(void)
{
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	SetLeftSoftkeyFunction(EntryScrForPHBCopyChoose,KEY_EVENT_UP);
	SetKeyHandler(EntryScrForPHBCopyChoose,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);			
#else
	SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_confirm,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_op_copy_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);			
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_highlight_op_move_entry

  	PURPOSE				: Highlight handler for "Move" in Entry Options menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_highlight_op_move_entry (void)
{
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	SetLeftSoftkeyFunction (EntryScrForPHBMoveChoose, KEY_EVENT_UP);
	SetKeyHandler(EntryScrForPHBMoveChoose,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);			
#else
	SetLeftSoftkeyFunction (mmi_phb_entry_op_move_confirm, KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_op_move_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);			
#endif
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_highlight_op_duplicate_entry

  	PURPOSE				: Highlight handler for "Duplicate" in Entry Options menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_highlight_op_duplicate_entry (void)
{
	SetLeftSoftkeyFunction (mmi_phb_entry_op_dulicate_confirm, KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_op_dulicate_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);			
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#ifdef __MMI_MULTI_SIM__
/****************************************************************
**
**
**
**
**
*****************************************************************/
void mmi_phb_highlight_op_copy_sim1_phone(void)
{
	if(g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES)
	{
		phbcopymovesrc = MMI_NVRAM;
		phbcopymovedes = MMI_SIM;
	}
	else
	{
		phbcopymovesrc = MMI_SIM;
		phbcopymovedes = MMI_NVRAM;		
	}
	
	SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_confirm, KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_op_copy_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);			
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void mmi_phb_highlight_op_move_sim1_phone(void)
{
	if(g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES)
	{
		phbcopymovesrc = MMI_NVRAM;
		phbcopymovedes = MMI_SIM;
	}
	else
	{
		phbcopymovesrc = MMI_SIM;
		phbcopymovedes = MMI_NVRAM;		
	}
	
	SetLeftSoftkeyFunction(mmi_phb_entry_op_move_confirm, KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_op_move_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);				
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif
/**************************************************************

	FUNCTION NAME		: mmi_phb_entry_op_copy_confirm()

  	PURPOSE				: Confirms before copying entry from one
						  location to another

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_entry_op_copy_confirm(void)
{

	#ifdef __MMI_MULTI_SIM__
	U8 nSimIndex;
	#endif
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_op_copy_confirm.>\n",__FILE__,__LINE__));

	copyEntry = TRUE;
	moveEntry = FALSE;
	duplicateEntry = FALSE;

#ifdef __MMI_MULTI_SIM__

	#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	if(g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES)
	{
		phbcopymovesrc = MMI_NVRAM;
		phbcopymovedes = MMI_SIM;
	}
	else
	{
		phbcopymovesrc = MMI_SIM;
		phbcopymovedes = MMI_NVRAM;		
	}
	#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */

	nSimIndex = phbcopymovedes - MMI_SIM;
	if(phbcopymovedes >= MMI_SIM && phbcopymovedes < MMI_SIM4 && g_phb_cntx.sim_used[nSimIndex] >= g_phb_cntx.sim_total[nSimIndex])
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
	}
	else if((phbcopymovedes == MMI_NVRAM)&& (g_phb_cntx.phone_used == g_phb_cntx.phone_total))
		DisplayPopup((PU8)GetString(STR_PHONE_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
#else
	if((g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES) && (g_phb_cntx.sim_used[0] == g_phb_cntx.sim_total[0]))
		DisplayPopup((PU8)GetString(STR_SIM_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	else if((g_phb_name_index[g_phb_cntx.active_index] >= MAX_PB_PHONE_ENTRIES) && (g_phb_cntx.phone_used == g_phb_cntx.phone_total))
		DisplayPopup((PU8)GetString(STR_PHONE_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
#endif	
	else
	{
		DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
							  STR_GLOBAL_NO, IMG_GLOBAL_NO,
							  get_string(STR_COPY_QUERY_MSG), IMG_GLOBAL_QUESTION, WARNING_TONE);


		SetLeftSoftkeyFunction(mmi_phb_op_copy_move_dup_req,KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	}
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_entry_op_move_confirm()

  	PURPOSE				: Confirms before moving the entry which
						  the user selects from the PHB list

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_entry_op_move_confirm(void)
{

#ifdef __MMI_MULTI_SIM__
	U8 nSimIndex;
#endif

	mmi_trace(TRUE,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	moveEntry = TRUE;
	copyEntry = FALSE;
	duplicateEntry = FALSE;

	
#ifdef __MMI_MULTI_SIM__
	#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	if(g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES)
	{
		phbcopymovesrc = MMI_NVRAM;
		phbcopymovedes = MMI_SIM;
	}
	else
	{
		phbcopymovesrc = MMI_SIM;
		phbcopymovedes = MMI_NVRAM;		
	}
	#endif
	nSimIndex = phbcopymovedes - MMI_SIM;
	if(phbcopymovedes >= MMI_SIM && phbcopymovedes <= MMI_SIM4 && g_phb_cntx.sim_used[nSimIndex] == g_phb_cntx.sim_total[nSimIndex])
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
	}
	else if((phbcopymovedes == MMI_NVRAM)&& (g_phb_cntx.phone_used == g_phb_cntx.phone_total))
		DisplayPopup((PU8)GetString(STR_PHONE_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
#else
	if((g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES) && (g_phb_cntx.sim_used[0] == g_phb_cntx.sim_total[0]))
		DisplayPopup((PU8)GetString(STR_SIM_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	else if((g_phb_name_index[g_phb_cntx.active_index] >= MAX_PB_PHONE_ENTRIES) && (g_phb_cntx.phone_used == g_phb_cntx.phone_total))
		DisplayPopup((PU8)GetString(STR_PHONE_FULL_MSG), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
#endif
	else
	{
		DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
							  STR_GLOBAL_NO, IMG_GLOBAL_NO,
							  get_string(STR_MOVE_QUERY_MSG), IMG_GLOBAL_QUESTION, WARNING_TONE);


		SetLeftSoftkeyFunction(mmi_phb_op_copy_move_dup_req,KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	}
}


/**************************************************************

	FUNCTION NAME		: mmi_phb_entry_op_dulicate_confirm()

  	PURPOSE				: Confirms before duplicating the entry which
						  the user selects from the PHB list

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_entry_op_dulicate_confirm(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 store_index;
	U16 str_id = 0;
#if defined(__MMI_FILE_MANAGER__)
		PHB_OPTIONAL_IDS_STRUCT* opt_ids;
#endif
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	U16 video_id;
#endif

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	duplicateEntry = TRUE;
	copyEntry = FALSE;
	moveEntry = FALSE;

	store_index = g_phb_name_index[g_phb_cntx.active_index];

	if((store_index >= MAX_PB_PHONE_ENTRIES) && (g_phb_cntx.sim_used == g_phb_cntx.sim_total))
	{
		str_id = STR_SIM_FULL_MSG;
	}
	else if((store_index < MAX_PB_PHONE_ENTRIES) && (g_phb_cntx.phone_used == g_phb_cntx.phone_total))
	{
		str_id = STR_PHONE_FULL_MSG;
	}
	else if(store_index < MAX_PB_PHONE_ENTRIES)
	{
	/*Check disc space before copy image*/
#if defined(__MMI_FILE_MANAGER__)
		mmi_phb_read_optional_ids(store_index);
		opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;

		if( opt_ids->pictureTagID == 1)
		{
#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
			if(!mmi_phb_image_read_nvram((store_index),g_phb_file_path))
				mmi_phb_image_get_full_path_by_index((U16)(store_index+1), g_phb_file_path);

#else
			mmi_phb_image_get_full_path_by_index((U16)(store_index+1), g_phb_file_path);
#endif
			if((pfnUnicodeStrlen(g_phb_file_path) > 0) && \
			    (!mmi_phb_image_check_valid_image(g_phb_file_path)))
			{
				return;
			}
		}
#endif

	/*Check data store space before copy video*/
#if defined(__MMI_INCOMING_CALL_VIDEO__)
		video_id = mmi_phb_video_get_id_by_index(store_index);

		if((video_id & 0x8000) && (!mmi_phb_video_check_data_space(NULL)))
		{
			return;
		}
#endif

	}

	if(str_id != 0)
	{
		DisplayPopup((PU8)GetString(str_id), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		return;
	}

	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
				     STR_GLOBAL_NO, IMG_GLOBAL_NO,
				     get_string(STR_DUPLICATE_QUERY_MSG), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(mmi_phb_op_copy_move_dup_req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

#if defined(__MMI_PHB_COMBINE_COPY_DUPLICATE__)
/**************************************************************

	FUNCTION NAME		: mmi_phb_highlight_op_copy_duplicate

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_highlight_op_copy_duplicate(void)
{
	SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_duplicate,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_op_copy_duplicate,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);				
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: mmi_phb_entry_op_copy_duplicate()

  	PURPOSE				:
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_entry_op_copy_duplicate(void)
{
	U8* guiBuffer;
	U16 nStrItemList[2];
	U16 nImgIltemList[2];
	U16 nNumofItem;


	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_op_copy_duplicate.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_ID_OPTION_COPY_DUPLICATE, mmi_phb_exit_op_copy_duplicate, mmi_phb_entry_op_copy_duplicate, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_OPTION_COPY_DUPLICATE);
	nNumofItem = GetNumOfChild(MENU_ID_PHB_OPTION_COPY_DUPLICATE);
	GetSequenceStringIds(MENU_ID_PHB_OPTION_COPY_DUPLICATE, nStrItemList);
	nImgIltemList[0] = IMG_STORAGE_HANDSET;
	nImgIltemList[1] = IMG_STORAGE_SIM;
	SetParentHandler(MENU_ID_PHB_OPTION_COPY_DUPLICATE);

	if(g_phb_name_index[g_phb_cntx.active_index] >= MAX_PB_PHONE_ENTRIES) /*Entry in SIM*/
	{
		SetHiliteHandler(MITEM1013_PBOOK_COPY_ENTRY	,mmi_phb_highlight_op_copy_entry);
		SetHiliteHandler(MITEM1015_PBOOK_DUPLICATE	,mmi_phb_highlight_op_duplicate_entry);
	}
	else /*Entry in NVRAM*/
	{
		SetHiliteHandler(MITEM1015_PBOOK_DUPLICATE	,mmi_phb_highlight_op_copy_entry);
		SetHiliteHandler(MITEM1013_PBOOK_COPY_ENTRY	,mmi_phb_highlight_op_duplicate_entry);
	}

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);


	ShowCategory15Screen(STR_ID_PHB_OPTION_COPY_DUPLICATE, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, nImgIltemList,
						  LIST_MENU, 0, guiBuffer);

}
/**************************************************************

	FUNCTION NAME		: mmi_phb_exit_op_copy_duplicate()

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_exit_op_copy_duplicate(void)
{
	g_phb_cntx.end_scr_id = SCR_ID_OPTION_COPY_DUPLICATE;
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_copy_move_dup_req()
* DESCRIPTION
*   Sends the request to the stack to copy/move/duplicate an entry that the user selects from the phonebook list.
*   that the user selects from the phonebook list.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   mone
*****************************************************************************/
void mmi_phb_op_copy_move_dup_req(void)
{
	MSG_PHB_SET_ENRTY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	U16 store_index;
	U8 storage;
	module_type	dest_mod_id = MOD_L4C;

	mmi_trace(TRUE,"-----+++++Func: %s;File: %s;Line: %d+++++----- copyEntry=%d,duplicateEntry=%d,moveEntry=%d", __FUNCTION__, __FILE__, __LINE__,copyEntry,duplicateEntry,moveEntry);

	store_index = g_phb_name_index[g_phb_cntx.active_index];
	storage = PHBGetStorageByStoreIndex(store_index);

#ifdef __MMI_MULTI_SIM__
	phbcopymovesrc = storage;
#endif
	
	myMsgPtr = (MSG_PHB_SET_ENRTY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_SET_ENRTY_REQ_STRUCT));
	myMsgPtr->type = MMI_PHB_PHONEBOOK;
	myMsgPtr->no_data = 1;
	myMsgPtr->list.index = 0xFFFF;
	myMsgPtr->list.record_index = 0xFFFF;

	if(copyEntry)
	{
		mmi_phb_show_in_progress(STR_PHB_COPYING, IMG_COPIED);
	#ifdef __MMI_MULTI_SIM__
		myMsgPtr->list.storage = phbcopymovedes;
	#else
		if (storage == MMI_SIM)
			myMsgPtr->list.storage = MMI_NVRAM;
		else
			myMsgPtr->list.storage = MMI_SIM;
	#endif	
	}
	else if(duplicateEntry)
	{
		mmi_phb_show_in_progress(STR_PHB_DUPLICATING, IMG_ENTRY_DUPLICATED_MSG);
		myMsgPtr->list.storage = storage;
	}
	else if(moveEntry)
	{
		moveEntryStoreIndex = store_index;

		mmi_phb_show_in_progress(STR_PHB_MOVING, IMG_ENTRY_MOVED_MSG);
	#ifdef __MMI_MULTI_SIM__
		myMsgPtr->list.storage = phbcopymovedes;
	#else
		if (storage == MMI_SIM)
			myMsgPtr->list.storage = MMI_NVRAM;
		else
			myMsgPtr->list.storage = MMI_SIM;
	#endif	
	}

	mmi_phb_op_set_entry_prepare_name_number_by_index(myMsgPtr, store_index);

#ifdef __MMI_MULTI_SIM__
mmi_trace(g_sw_PHB,"PHB Func: %s deslist.storage = %d src  = %d", __FUNCTION__,phbcopymovedes, phbcopymovesrc);

	 if(myMsgPtr->list.storage >= MMI_SIM && myMsgPtr->list.storage <= MMI_SIM4)
 	{
 		dest_mod_id = MOD_L4C + myMsgPtr->list.storage - MMI_SIM;
 	}
#endif
	
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_SET_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandler(mmi_phb_op_copy_move_dup_rsp	,PRT_PHB_SET_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);
	DeleteNScrId(SCR_PBOOK_ENTRY_OPTIONS);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_op_copy_move_dup_rsp()
* DESCRIPTION
*   Handles the response from stack after the entry is copied/moved/duplicated.
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void mmi_phb_op_copy_move_dup_rsp(void* info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	MSG_PHB_SET_ENRTY_RSP_STRUCT* localPtr;
	U8 modified;
	U8 flag = 0;
	U8 num_ascii[MAX_PB_NUMBER_LENGTH+1+1];
	S16 pError;
	BOOL copy_image = TRUE, copy_video = TRUE;
	U16 store_index, src_store_index;
	PHB_OPTIONAL_IDS_STRUCT* opt_ids;

	BOOL bFdl = MMI_FALSE;
	U8 i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	for(i = 0; i< MMI_SIM_NUMBER; i++)
   	{
   		if(g_SecuritySetupContext.FdlStatus[i])
   		{
			bFdl = MMI_TRUE;
			break;
   		}
   	}
	localPtr = (MSG_PHB_SET_ENRTY_RSP_STRUCT *)info;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_copy_move_dup_rsp.>\n",__FILE__,__LINE__));
#ifdef __MMI_MULTI_SIM__
	mmi_trace(g_sw_PHB, "PHB Func: %s deslist.storage = %d src  = %d", __FUNCTION__,phbcopymovedes, phbcopymovesrc);
#endif
	if (localPtr->result.flag == 0/*OK*/)
	{

		src_store_index = g_phb_name_index[g_phb_cntx.active_index];

		store_index= mmi_phb_op_get_store_index_and_increase_counter(localPtr->list[0].storage, localPtr->list[0].record_index);

		if(copyEntry || moveEntry)
		{
			if(localPtr->list[0].storage == MMI_NVRAM) /* From SIM to NVRAM*/
			{
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
				/*Optional Fields*/
#if 0
				memset(&PhoneBookOptionalFields, 0, sizeof(PhoneBookOptionalFields));
				WriteRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(localPtr->list[0].record_index), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
                memset(&PhoneBookOptionalFields, 0, sizeof(PhoneBookOptionalFields));
				WriteRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, (localPtr->list[0].record_index-1)*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
				/*Clear Optional Selection Fields*/
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
			mmi_phb_copy_optional_ids(store_index,src_store_index);

			if(localPtr->list[0].storage == MMI_SIM)
				mmi_phb_clear_optional_ids_for_sim(store_index);
#endif
		}
		/*-------------------------------Duplicate Entry ----------------------------*/
		else
		{
#if !defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
			if(localPtr->list[0].storage == MMI_NVRAM)
#endif
			{
				mmi_phb_copy_optional_ids(store_index,src_store_index);
			}

			if(localPtr->list[0].storage == MMI_NVRAM)
			{
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
				mmi_phb_read_optional_ids(store_index);
				opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;

				/*Duplicate image in file system.*/
				if( opt_ids->pictureTagID == 1)
					copy_image = mmi_phb_image_duplicate_by_index((U16)(store_index+1), (U16)(src_store_index + 1));
#endif

#if defined(__MMI_INCOMING_CALL_VIDEO__)
				copy_video = mmi_phb_video_duplicate_by_index(store_index, src_store_index);
#endif

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
				/*Optional Fields*/
#if 0
				ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(src_store_index + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
				WriteRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(localPtr->list[0].record_index), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
#endif
				ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, (src_store_index-1)*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
				WriteRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, (localPtr->list[0].record_index-1)*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
				flag = mmi_phb_op_increase_lookup_table(store_index,
																  NULL,
																  (S8*)PhoneBookOptionalFields.faxNumber,
																  (S8*)PhoneBookOptionalFields.homeNumber,
																  (S8*)PhoneBookOptionalFields.officeNumber);
#endif
			/*Duplicate birthday field*/
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
			mmi_phb_bday_duplicate_field(store_index, src_store_index);
#endif
			}
		}

		modified =  PhoneBook[src_store_index].alpha_id.name_length - localPtr->list[0].alpha_id.name_length;

		mmi_phb_convert_to_digit(num_ascii, PhoneBook[src_store_index].tel.number, MAX_PB_NUMBER_LENGTH+1);
		flag += mmi_phb_op_increase_lookup_table(store_index, (S8*)num_ascii, NULL,NULL,NULL);

		if(flag)
			mmi_phb_lookup_table_sort();

		mmi_phb_op_set_entry_rsp_name_number(info, store_index);

		if(localPtr->list[0].storage >= MMI_SIM && localPtr->list[0].storage <= MMI_SIM4)
		{
			mmi_phb_op_update_field_flag(store_index, (S8*)localPtr->list[0].tel.number, NULL,NULL,NULL,NULL,TRUE);
		}
		else
		{
#if defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
			mmi_phb_op_update_field_flag(store_index, (S8*)localPtr->list[0].tel.number, NULL,NULL,NULL,NULL,TRUE);
#else
			mmi_phb_op_update_field_flag(store_index,
												 (S8*)localPtr->list[0].tel.number,
												 (S8*)PhoneBookOptionalFields.homeNumber,
												 (S8*)PhoneBookOptionalFields.officeNumber,
												 (S8*)PhoneBookOptionalFields.faxNumber,
												 (S8*)PhoneBookOptionalFields.emailAddress,
												 TRUE);
#endif
		}

#if defined(__MMI_PHB_PINYIN_SORT__)	/*Update Pinyin Cache for sort*/
		mmi_phb_util_make_pinyin_cache(store_index);
#endif

		/*Insert name index to name sorting mapping table.*/
		mmi_phb_sort_insert_name_index(store_index);
		g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;

		/*Insert email index to email sorting mapping table*/
#if defined(__MMI_PHB_EMAIL_SORT__)
		if((localPtr->list[0].storage == MMI_NVRAM) && (g_phb_email_sorted == 1)) /* From SIM to NVRAM*/
			mmi_phb_sort_email_insert_index(store_index);
#endif

		/*add central command for vrsi.*/
#if defined(__MMI_VRSI__)
		mmi_vrsi_ndial_central_add_single(store_index);
#endif

		if(moveEntry)
		{
			moveModified = modified;
			mmi_phb_op_move_req();
		}
		else
		{
			if(modified)
			{
				DisplayPopup((PU8)GetString(STR_COPIED_NAME_MODIFIED), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
			}
			/*Data duplicate success, but image or video add fail*/
			else if(!copy_image || !copy_video)
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

				pfnUnicodeStrcat(str_buff, GetString(STR_ID_PHB_DUPLICATE_FAIL));
				DisplayPopup((U8*)str_buff, IMG_GLOBAL_ERROR,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
			}
			else
			{
			#ifdef __MMI_MULTI_SIM__
				if((copyEntry || duplicateEntry) && localPtr->list[0].storage==MMI_NVRAM)
					DisplayPopup((PU8)GetString(STR_COPIED_TO_PHONE_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
				else if((copyEntry || duplicateEntry) && localPtr->list[0].storage>=MMI_SIM && localPtr->list[0].storage<= MMI_SIM4)
					#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
					DisplayPopup((PU8)GetString(STR_COPIED_TO_SIM_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
					#else
					DisplayPopup((PU8)GetString(STR_COPIED_TO_SIM1_DONE + localPtr->list[0].storage - MMI_SIM), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
					#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
				else
					DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
			#else			
				if((copyEntry || duplicateEntry) && localPtr->list[0].storage==MMI_NVRAM)
					DisplayPopup((PU8)GetString(STR_COPIED_TO_PHONE_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
				else if((copyEntry || duplicateEntry) && localPtr->list[0].storage==MMI_SIM)
					DisplayPopup((PU8)GetString(STR_COPIED_TO_SIM_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
				else
					DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
			#endif
			}
		}
	}
	else
	{
		if(localPtr->result.cause == MMI_SIM_EF_RECORD_FULL)
		{
			if(copyEntry)
			DisplayPopup((PU8)GetString(STR_NOT_COPIED_NUMBER_TOO_LONG), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
			else if(moveEntry)
				DisplayPopup((PU8)GetString(STR_NOT_MOVED_NUMBER_TOO_LONG), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
			else if(duplicateEntry)
				DisplayPopup((PU8)GetString(STR_NOT_DUPLICATED_NUMBER_TOO_LONG), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
		else if(bFdl)  /*For FDN on Message*/
		{
			DisplayPopup((PU8)GetString(STR_FDL_IS_ON), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
		else if(localPtr->result.cause == MMI_PHB_CAUSE_CAPACITY_EXCEEDED)
		{
  			DisplayPopup((PU8)GetString(STR_ID_PHB_SYSTEM_FULL), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
		else
			DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED,  TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}

	copyEntry = duplicateEntry = moveEntry = FALSE;
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_op_move_req()

  	PURPOSE				: This function performs the 2nd step of the
						  'Move' operation. It sends the request to
 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_op_move_req(void)
{
	MSG_PHB_DELETE_ENTRY_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	U8 storage;
	module_type	dest_mod_id = MOD_L4C;
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_move_req.>\n",__FILE__,__LINE__));

	myMsgPtr = (MSG_PHB_DELETE_ENTRY_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_DELETE_ENTRY_REQ_STRUCT));

 	storage = PHBGetStorageByStoreIndex(moveEntryStoreIndex);
	
	myMsgPtr->storage = storage;
	myMsgPtr->del_all = MMI_FALSE;
	myMsgPtr->no_data = 1;
	myMsgPtr->type = MMI_PHB_PHONEBOOK;
	/*
	*The index means "record_index" here. This change only for MMI_PHB_PHONEBOOK(ADN) type
	* for other type( BDN ,FDN, MSISDN...) This field still need to use index as parameter.
	*/
	myMsgPtr->index = PHBGetADPIndexByStorage(storage, moveEntryStoreIndex);

#ifdef __MMI_MULTI_SIM__
	 if(storage >= MMI_SIM && storage <= MMI_SIM4)
 	{
 		dest_mod_id = MOD_L4C + storage - MMI_SIM;
 	}
#endif
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_DEL_ENTRY_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	SetProtocolEventHandler(mmi_phb_op_move_rsp,PRT_PHB_DEL_ENTRY_RSP);
	OslMsgSendExtQueue(&Message);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_op_move_rsp()

  	PURPOSE				: Response handler for mmi_phb_op_move_req().

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_op_move_rsp(void* info)
{
	MSG_PHB_DELETE_ENTRY_RSP_STRUCT* localPtr;
	U16 deletedCnt = 0;
	//U8 grpIndex;
	//S16 pError;
	U8 storage;
	U16 record_index;
	PHB_OPTIONAL_IDS_STRUCT* opt_ids;
	//PHB_CALLER_GROUP_STRUCT* callerGroups;
	BOOL bFdl = MMI_FALSE;
	U8 i;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SecuritySetupContext.FdlStatus[i])
		{
			bFdl = MMI_TRUE;
			break;
		}
	}
	localPtr = (MSG_PHB_DELETE_ENTRY_RSP_STRUCT *)info;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_op_move_rsp.>\n",__FILE__,__LINE__));

	if (localPtr->result.flag == 0 /*OK*/)
	{

		storage = PHBGetStorageByStoreIndex(moveEntryStoreIndex);
		record_index = PHBGetADPIndexByStorage(storage, moveEntryStoreIndex);;

		PhbUpdateSpeedDial(storage, record_index, MMI_PHB_PHONEBOOK, NULL, NULL);

		if(storage >= MMI_SIM && storage <= MMI_SIM4) /* Record store in SIM, SIM index is begin from "MAX_PB_PHONE_ENTRIES" */
		{
			g_phb_cntx.sim_used[storage - MMI_SIM]--;
		}
		else
		{
			g_phb_cntx.phone_used--;
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
			mmi_phb_read_optional_ids(moveEntryStoreIndex);
			opt_ids = (PHB_OPTIONAL_IDS_STRUCT*)g_phb_cntx.optional_ids;

			/*Delete image file in file folder*/
			if( opt_ids->pictureTagID == 1)
				mmi_phb_image_delete_by_index((U16)(moveEntryStoreIndex+1));
#endif

#if defined(__MMI_INCOMING_CALL_VIDEO__)
			mmi_phb_video_delete_by_index(moveEntryStoreIndex); /*Clear video field.*/
#endif

		/*Remove email entry from email sort mapping table*/
#if defined(__MMI_PHB_EMAIL_SORT__)
			if(g_phb_email_sorted == 1)
			mmi_phb_sort_email_delete_index(moveEntryStoreIndex);
#endif
		}

		/*Remove lookup table entry*/
		deletedCnt = mmi_phb_op_clear_lookup_table_by_index(moveEntryStoreIndex);

		if(deletedCnt)
		{
			mmi_phb_lookup_table_sort();
			g_phb_cntx.lookup_table_count -= deletedCnt;
		}

#if defined(__MMI_VRSD_DIAL__)
		/*remove voice dial tag if exists.*/
		mmi_vrsddial_delete_tag_by_store_index(moveEntryStoreIndex);
#endif

#if defined(__MMI_VRSI__)
		/*remove voice dial tag if exists.*/
		mmi_vrsi_ndial_central_delete_single(moveEntryStoreIndex);
#endif

		/*clear phonebook entry and remove from index*/
		mmi_phb_op_delete_update_index(moveEntryStoreIndex);

		if(moveModified)
		{
			moveModified = 0;
//			DisplayPopup((PU8)GetString(STR_MOVED_NAME_MODIFIED), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);  //guojian delete for 10096
		}
//		else  //guojian delete for 10096
		{
		#ifdef __MMI_MULTI_SIM__
			if(phbcopymovedes >= MMI_SIM && phbcopymovedes <= MMI_SIM4)
			{
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
				DisplayPopup((PU8)GetString(STR_MOVED_TO_SIM_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
			#else
				DisplayPopup((PU8)GetString(STR_MOVED_TO_SIM1_DONE + phbcopymovedes - MMI_SIM), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
			}
			else
			{
				DisplayPopup((PU8)GetString(STR_MOVED_TO_PHONE_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
			}		
		#else
			if(storage == MMI_SIM)
			{
				DisplayPopup((PU8)GetString(STR_MOVED_TO_PHONE_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
			}
			else
			{
				DisplayPopup((PU8)GetString(STR_MOVED_TO_SIM_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
			}
		#endif	
		}
	}
	else if(bFdl)  /*For FDN on Message*/
	{
		DisplayPopup((PU8)GetString(STR_FDL_IS_ON), IMG_GLOBAL_UNFINISHED, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}
	else
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_highlight_op_dial_number()

  	PURPOSE				: Highlight handler for "Dial" in Entry options menu

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_highlight_op_dial_number(void)
{
#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_SetCenterFunc(mmi_phb_list_pre_choose_number);
	SetLeftSoftkeyFunction(MTPNP_PFAL_CC_CSKHandler,KEY_EVENT_UP);
	SetKeyHandler(MTPNP_PFAL_CC_CSKHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(mmi_phb_list_pre_choose_number,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_list_pre_choose_number,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);		
#endif	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

#define MMI_PHB_OP_EXTRA_OPERATION
/*IP Dial from phonebook entry option menu.*/
#if defined(__MMI_PHB_IP_DIAL_FROM_PHB__)
/**************************************************************

	FUNCTION NAME		: mmi_phb_entry_list_choose_number_ip_dial()

  	PURPOSE				: Set IP dial to TRUE, then dial the number

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_entry_list_choose_number_ip_dial(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MULTI_SIM__
	U8 reqSrcMod = GetReqSrcModIndex();
	MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, reqSrcMod);
#endif
	
#if defined(__IP_NUMBER__) /*Set IP Dial option to TRUE*/
	SetTempUseIPNumber(TRUE);
#endif

	mmi_phb_list_pre_choose_number();
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_highlight_op_ip_number()

  	PURPOSE				: Highlight handler for "IP Dial" in Entry options menu

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_highlight_op_ip_number(void)
{
	SetLeftSoftkeyFunction(mmi_phb_entry_list_choose_number_ip_dial,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_list_choose_number_ip_dial,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);		
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif

/*Send SMS from phonebook entry option menu.*/
#if defined(__MMI_PHB_SEND_SMS_FROM_PHB__)
extern MMI_PHB_LIST_VIEW phbListView;
/**************************************************************

	FUNCTION NAME		: mmi_phb_sms_pre_send_data_for_entry()

  	PURPOSE				: Prepare data to send sms through PHB

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_sms_pre_send_data_for_entry(void)
{
#if 1 // zrx del 20060612
	if(IsMessagesReEntering())
	{
		DisplayPopup ((PU8)GetString (STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}

	mmi_msg_set_phb_send_case(MMI_SEND_SMS_FROM_PHB);
	g_phb_cntx.active_index_second = g_phb_cntx.active_index;

	phbListView = MMI_PHB_LIST_FOR_SMS;
	mmi_phb_sms_send_data_choose_entry();
	#endif
}

/**************************************************************

	FUNCTION NAME		: mmi_phb_highlight_op_send_sms()

  	PURPOSE				: Highlight handler for "Send SMS" in Entry options menu

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_highlight_op_send_sms(void)
{
	SetLeftSoftkeyFunction(mmi_phb_sms_pre_send_data_for_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_sms_pre_send_data_for_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif

/*Send MMS from phonebook entry option menu.*/
#if defined(__MMI_PHB_SEND_MMS_FROM_PHB__)
/**************************************************************
* FUNCTION
*  mmi_phb_highlight_op_send_mms()
* DESCRIPTION
*   Highlight handler for "Send SMS" in Entry options menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
  	PURPOSE				: Prepare data to send sms through PHB

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_phb_mms_pre_send_data_for_entry(void)
{
	g_phb_cntx.active_index_second = g_phb_cntx.active_index;
	mmi_phb_mms_send_data_choose_entry();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_op_send_mms()
* DESCRIPTION
*   Highlight handler for "Send SMS" in Entry options menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_op_send_mms(void)
{
	SetLeftSoftkeyFunction(mmi_phb_mms_pre_send_data_for_entry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_mms_pre_send_data_for_entry, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif /* __MMI_PHB_SEND_MMS_FROM_PHB__ */

#define MMI_PHB_IMAGE
/*------------------------------------------------- Image from File Manager------------------------------------------------------------*/
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_image_highlight_hdlr()
* DESCRIPTION
*   Highlight handler of inline editor image field. Used when file manager exists.
*
* PARAMETERS
*  index  IN	Current inline selection index.
* RETURNS
*  void
* GLOBALS AFFECTED
*   Change LSK function.
*****************************************************************************/
void mmi_phb_image_highlight_hdlr(S32 index)
{
	if(index == (g_phb_cntx.total_image_id - 1))
	{
		ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
		SetLeftSoftkeyFunction(mmi_phb_image_pre_entry_option, KEY_EVENT_UP);
	}
	else
	{
		ChangeLeftSoftkey(STR_VIEW_LSK, IMG_VIEW_LSK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_edit_view_picture, KEY_EVENT_UP);
	}
}

void mmi_phb_black_list_highlight_hdlr(S32 index)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
}

void mmi_phb_red_list_highlight_hdlr(S32 index)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_pre_entry_option()
* DESCRIPTION
*   Phonebook associate image operation: View, select, delete.
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_image_pre_entry_option(void)
{
	if(g_phb_cntx.image_location == MMI_PHB_IMAGE_NO_SELECT)
		mmi_phb_entry_image_option(2);
	else
		mmi_phb_entry_image_option(1);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_image_option()
* DESCRIPTION
*   Phonebook associate image operation: View, select, delete.
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_image_option(U8 type)
{
	U8* guiBuffer;
	U16 nStrItemList[5];
	U16 nNumofItem;
	U32 HideItemId = -1, HideItemId1 = -1;
	U32 maskingByte = -1;

	EntryNewScreen(SCR_ID_PHB_IMAGE_OPTION, mmi_phb_exit_image_option, mmi_phb_image_pre_entry_option, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_IMAGE_OPTION);
	nNumofItem = GetNumOfChild(MENU_ID_PHB_IMAGE_OPTION);
	GetSequenceStringIds(MENU_ID_PHB_IMAGE_OPTION, nStrItemList);
	SetParentHandler(MENU_ID_PHB_IMAGE_OPTION);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/*Hide menu item play and delete when file not selected.*/
	/*Regiser highlight handler again.*/
	mmi_phb_operate_single_init();

	/*File not selected*/
	if(type == 2)
	{
		HideItemId = GetChildMenuIDIndexByParentMenuID(MENU_ID_PHB_IMAGE_OPTION,MENU_ID_PHB_IMAGE_OPTION_VIEW);
		HideItemId1=	GetChildMenuIDIndexByParentMenuID(MENU_ID_PHB_IMAGE_OPTION,MENU_ID_PHB_IMAGE_OPTION_DELETE);
		if((HideItemId != -1) && (HideItemId != -1))
		{
			ResetBit(maskingByte,HideItemId);
			ResetBit(maskingByte,HideItemId1);
			MaskHiliteItems(MENU_ID_PHB_IMAGE_OPTION, maskingByte);
			nNumofItem=MaskItems(nStrItemList,(U8)nNumofItem,maskingByte);
		}
	}



	ShowCategory15Screen(STR_ASSOCIATE_PICTURE, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	g_phb_cntx.set_done_flag = 1;
	viewOnce = TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_image_option()
* DESCRIPTION
*   screen exit function
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*   Register exit screen id.
*****************************************************************************/
void mmi_phb_exit_image_option(void)
{
	g_phb_cntx.end_scr_id = SCR_ID_PHB_IMAGE_OPTION;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_image_view()
* DESCRIPTION
*   Highlight function of image option.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_image_view(void)
{
	SetLeftSoftkeyFunction(mmi_phb_image_view_from_phb_folder,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_image_view_from_phb_folder,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_image_select()
* DESCRIPTION
*   Highlight function of image option.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_image_select(void)
{
	SetLeftSoftkeyFunction(mmi_phb_image_select_from_file_mamager,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_image_select_from_file_mamager,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_image_delete
* DESCRIPTION
*   Highlight function of image option.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_image_delete(void)
{
	SetLeftSoftkeyFunction(mmi_phb_image_delete_from_phb_folder,KEY_EVENT_UP);
	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_view_from_phb_folder
* DESCRIPTION
*   view image from selected file path
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_image_view_from_phb_folder(void)
{
	U16 rid;
	FS_HANDLE fh = 0;
	S16 drive;
	S8 path[MMI_PHB_MAX_PATH_LENGTH];

	if((g_phb_cntx.image_location == MMI_PHB_IMAGE_FOR_ENTRY) ||	/*An Image in Phonebook folder*/
		(g_phb_cntx.image_location == MMI_PHB_IMAGE_SELECT_PATH) || 	/*An Image in select path*/
		(g_phb_cntx.image_location == MMI_PHB_IMAGE_CALLER_GROUP))	/*An Image in Caller Group Folder*/
	{
			
		if(g_phb_cntx.image_location == MMI_PHB_IMAGE_FOR_ENTRY ||
			g_phb_cntx.image_location == MMI_PHB_IMAGE_CALLER_GROUP) 
		{
			
			//drive = MMI_PHB_DRV;
                    drive = 67;// dingjian 20080329 device name  'C'
			if (drive > 0)
			{
				if(g_phb_cntx.image_location == MMI_PHB_IMAGE_FOR_ENTRY)
				{
					rid = g_phb_name_index[g_phb_cntx.active_index] +1;/*For Edit Option Only*/
					MMI_ASSERT(rid <= MAX_PB_PHONE_ENTRIES); /*Image should be associate to NVRAM record.*/
				}
				else
				{
					rid = 5000 + g_phb_cntx.active_index + 1; /*Current Caller Group, image id strat from 5000*/
				}
#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
				if(rid>5000)
				{
					mmi_phb_image_get_full_path_by_index(rid,path);
				}
				else
				{
					mmi_phb_image_read_nvram(rid-1,path);
				}
#else
				mmi_phb_image_get_full_path_by_index(rid,path);
#endif
				fh = MMI_FS_Open((U8*)path,FS_READ_ONLY);
			}
		}
		else
		{
			fh = MMI_FS_Open((U8*)g_phb_file_path, FS_READ_ONLY);
		}

		TRACE_FMGR_FUNCTION();
		
		if(fh >= 0) /*file exists*///Renwy add "=" 2008/07/01
		{
			MMI_FS_Close(fh);

			//ExecuteCurrExitHandler();
			EntryNewScreen(SCR_ID_PHB_IMAGE_VIEW_IMAGE, NULL, mmi_phb_image_view_from_phb_folder, NULL);

			/* entry cat222 to decode and display a image from file */
			if(g_phb_cntx.image_location == MMI_PHB_IMAGE_SELECT_PATH)
			{
				ShowCategory222Screen(	STR_ASSOCIATE_PICTURE, 0,
												0,	0,
												STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
												GDI_COLOR_BLACK,
												(PS8)NULL,
												(PS8)g_phb_file_path,
												FALSE,
												NULL);
			}
			else
			{
				ShowCategory222Screen(	STR_ASSOCIATE_PICTURE, 0,
												0,	0,
												STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
												GDI_COLOR_BLACK,
												(PS8)NULL,
												(PS8)path,
												FALSE,
												NULL);
			}
			SetRightSoftkeyFunction(GoBackHistory,	KEY_EVENT_UP);
			//SetGenericExitHandler(SCR_ID_PHB_IMAGE_VIEW_IMAGE, NULL, mmi_phb_image_view_from_phb_folder);

			return;
		}
	}
	g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;	/*Can't open file or not select file yet.*/
	DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_IMAGE_NO_SELECT), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_select_from_file_mamager
* DESCRIPTION
*   select image from file manager. enter file manager screen.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_image_select_from_file_mamager(void)
{
	FMGR_FILTER filter;

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
								(S8*)L"root",
								(PsExtFuncPtr)mmi_phb_image_select_from_file_mamager_done);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_select_from_file_mamager_done
* DESCRIPTION
*   call back of select image from file manager.
*
* PARAMETERS
*  path IN  path return by file manager.
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_image_select_from_file_mamager_done(void *path, int is_short)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if(path == NULL) 	/*User does not select file and go back directly.*/
	{
		GoBackToHistory(SCR_ID_PHB_IMAGE_OPTION);
		return;
	}

	/*Check image content.*/
	if(mmi_phb_image_check_valid_image((S8*)path))
	{
        mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
		pfnUnicodeStrcpy((S8*)g_phb_file_path, (S8*)path);

		fmgr_reset_app_select();

		g_phb_cntx.image_location = MMI_PHB_IMAGE_SELECT_PATH;

		fmgr_reset_app_select();	/*Use select_repeat, so clear by application itself.*/
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
		DeleteUptoScrID(SCR_ID_PHB_IMAGE_OPTION);
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_phb_image_delete_from_phb_folder
* DESCRIPTION
*   delete reference of selected image.
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   change image location to no select.
*****************************************************************************/
void mmi_phb_image_delete_from_phb_folder(void)
{
	if(g_phb_cntx.image_location == MMI_PHB_IMAGE_NO_SELECT)
		DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_IMAGE_NO_SELECT), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	else
	{
		g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
		DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_update_by_index
* DESCRIPTION
*   Update image by record_index
*
* PARAMETERS
*  record_index	IN	index of image
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
#include "nvram_user_defs.h"
#include "nvramtype.h"
#ifdef MMI_ON_WIN32
#define NVRAM_EF_PHB_PIC_SIZE 25600
#endif
BOOL mmi_phb_image_update_nvram(U16 record_index)
{
       S8* pb_pics;//[PHONE_PHB_ENTRY_COUNT][NVRAM_EF_PHB_PIC_SIZE];
	S16 err;
       S32 writen;
	Trace("mmi_phb_image_update_nvram %d",pfnUnicodeStrlen(g_phb_file_path));
	if(record_index<0||record_index> PHONE_PHB_ENTRY_COUNT-1)
	{
		Trace("mmi_phb_image_update_nvram record_index %d",record_index);
		return FALSE;
	}
	if(pfnUnicodeStrlen(g_phb_file_path) > 63)
	{
		return FALSE;
	}
      else
      {
			
	       pb_pics = COS_MALLOC(NVRAM_EF_PHB_PIC_SIZE);
		if(!pb_pics)
			return FALSE;
		memset( pb_pics, 0, NVRAM_EF_PHB_PIC_SIZE);
		writen = ReadRecord(NVRAM_EF_PHB_PIC_LID, 0, pb_pics, NVRAM_EF_PHB_PIC_SIZE, &err);

		memcpy(&(pb_pics[record_index*128]),g_phb_file_path,128);
              writen = WriteRecord(NVRAM_EF_PHB_PIC_LID, 0, pb_pics, NVRAM_EF_PHB_PIC_SIZE, &err);
              Trace("mmi_phb_image_update_nvram %d",err);
		COS_FREE(pb_pics);
		if(err!=NVRAM_WRITE_SUCCESS)
	      	{
			return FALSE;
		 }
		return TRUE;
	}
}
#endif
BOOL mmi_phb_image_update_by_index(U16 record_index)
{
//	S16 drive;
	S8 path[MMI_PHB_MAX_PATH_LENGTH];
	S8* file_ext;

    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	MMI_TRACE((MMI_TRACE_G4_PHB ,MMI_PHB_IMAGE_UPDATE_BY_INDEX,record_index));

	/*Delete Image if not select image*/
	if (g_phb_cntx.image_location == MMI_PHB_IMAGE_NO_SELECT)
	{
		mmi_phb_image_delete_by_index(record_index);
	}
	/*Update Image from select path*/
	else if(g_phb_cntx.image_location == MMI_PHB_IMAGE_SELECT_PATH)
	{
	#if !defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
		/*Delete all image with same file name before update.*/
		mmi_phb_image_delete_by_index(record_index);
	#endif
		TRACE_FMGR_FUNCTION();
		/*Make a copy of the selected file to phonebook folder.*/

/*Guojian Del Start For 11002 Ver: GreenStone  on 2008-12-23 17:12 */
		#if 0
		//drive = MMI_PHB_DRV;
		drive = 67;// dingjian 20080329 device name 'C'
		if (drive > 0)
		#endif
/*Guojian Del End  For 11002 Ver: GreenStone  on 2008-12-23 17:12 */
		{
#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
			if(mmi_phb_image_update_nvram(record_index-1))
			{
				g_phb_cntx.image_location = MMI_PHB_IMAGE_FOR_ENTRY;
				return TRUE;
			}
#endif				
			/*Create Path*/
			/*Guojian Modify Start For 11002 Ver: GreenStone  on 2008-12-23 17:11 */
			//sprintf(nPrintableStr,"%c:/%s", (S8)drive,MMI_PHB_IMAGE_FOLDER);
			sprintf(nPrintableStr,"/%s", MMI_PHB_IMAGE_FOLDER);
			/*Guojian Modify End  For 11002 Ver: GreenStone  on 2008-12-23 17:11 */

			AnsiiToUnicodeString(path, nPrintableStr);
			
			/*Check if folder exist first, if not create folder.*/
			if(mmi_phb_fs_check_image_folder(path) == FS_NO_ERROR)
			{
				/*Create file name*/
/*Guojian Modify Start For 11002 Ver: GreenStone  on 2008-12-23 17:15 */
//				sprintf(nPrintableStr,"%c:/%s/%d.",(S8)drive,MMI_PHB_IMAGE_FOLDER,record_index);
				sprintf(nPrintableStr,"/%s/%d.",MMI_PHB_IMAGE_FOLDER,record_index);
/*Guojian Modify End  For 11002 Ver: GreenStone  on 2008-12-23 17:15 */
				AnsiiToUnicodeString(path, nPrintableStr);

				/*Create file extension - from source file extension*/
				file_ext = mmi_fmgr_extract_ext_file_name(g_phb_file_path);
				pfnUnicodeStrcat(path, file_ext);

								
				if(mmi_phb_image_copy(path, g_phb_file_path))
				{					
					g_phb_cntx.image_location = MMI_PHB_IMAGE_FOR_ENTRY;
					return TRUE;
				}
			}
		}
		/*Copy Fail, clear to zero*/
		TRACE_FMGR_FUNCTION();
		g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
		//DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_IMAGE_COPY_FAIL), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_delete_by_index
* DESCRIPTION
*  Delete phonebook associate picture by index.
*  Note that all file name with different extensions should be deleted
*
* PARAMETERS
*   record_index	IN	index of image
* RETURNS
*   void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_image_delete_by_index(U16 record_index)
{
	FS_HANDLE fh_cur;
	FS_DOSDirEntry file_info;
	S8 path[MMI_PHB_MAX_PATH_LENGTH];
	S8 filename[MMI_PHB_MAX_PATH_LENGTH];
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

	//sprintf(nPrintableStr,"%c:/%s/%d.?*", MMI_PHB_DRV, MMI_PHB_IMAGE_FOLDER, record_index);
	//sprintf(nPrintableStr,"C:/%s/%d.GIF", MMI_PHB_IMAGE_FOLDER, record_index);
	sprintf(nPrintableStr,"C:/%s/%d.?*", MMI_PHB_IMAGE_FOLDER, record_index);
	AnsiiToUnicodeString(path, nPrintableStr);

	/*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 15:24 */
	if( (fh_cur = MMI_FS_FindFirst((U8*)path, 0, 0, &file_info, (U8*)filename, (INT32)MMI_PHB_MAX_PATH_LENGTH)) >= 0 )
	/*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 15:24 */
	{
		/* Loop through all file with different extension.*/
		do
		{
			/* filter out folder results*/
			if( !(file_info.Attributes & FS_ATTR_DIR) )
			{
				//sprintf(nPrintableStr,"%c:/%s/", MMI_PHB_DRV, MMI_PHB_IMAGE_FOLDER);
				sprintf(nPrintableStr,"C:/%s/", MMI_PHB_IMAGE_FOLDER);// dingjian 20080329 device name
				AnsiiToUnicodeString(path, nPrintableStr);
				pfnUnicodeStrcat(path,filename);

				/*Delete the file*/
				MMI_FS_Delete((U8*)path);
			}
		/*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 15:25 */
		} while( MMI_FS_FindNext( fh_cur, &file_info, (U8*)filename, (INT32)MMI_PHB_MAX_PATH_LENGTH ) == FS_NO_ERROR );
		/*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 15:25 */
		pffindclose( fh_cur );
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_duplicate_by_index
* DESCRIPTION
*  Duplicate image by source record_index and destination record_index.
*
* PARAMETERS
*  dst_record IN	index of destination picture
*  src_record	IN	index of source picture
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_phb_image_duplicate_by_index(U16 dst_record, U16 src_record)
{
	S16 drive;
	S8 src_path[MMI_PHB_MAX_PATH_LENGTH];
	S8 dst_path[MMI_PHB_MAX_PATH_LENGTH];
	S8* file_ext;

	MMI_TRACE((MMI_TRACE_G4_PHB ,MMI_PHB_IMAGE_DUPLICATE_BY_INDEX,dst_record, src_record));

	//drive = MMI_PHB_DRV;
       drive = 67;// dingjian 20080329 device name 'C'
	if (drive > 0)
	{
#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
		mmi_phb_image_read_nvram(src_record-1,g_phb_file_path);
		if(mmi_phb_image_update_nvram(dst_record-1))
		{
			return TRUE;
		}
#endif		
		/*Get source file path*/
		mmi_phb_image_get_full_path_by_index(src_record, src_path);

		/*Get destination file path*/
		sprintf(nPrintableStr, "%c:\\%s",(S8)drive, MMI_PHB_IMAGE_FOLDER);
		AnsiiToUnicodeString(dst_path, nPrintableStr);
		/*Check if folder exist first, if not create folder.*/
		if(mmi_phb_fs_check_image_folder(dst_path) != FS_NO_ERROR)
			return FALSE;

		sprintf(nPrintableStr, "%c:\\%s\\%d.",(S8)drive, MMI_PHB_IMAGE_FOLDER, dst_record);
		AnsiiToUnicodeString(dst_path, nPrintableStr);

		file_ext = mmi_fmgr_extract_ext_file_name(src_path);
		pfnUnicodeStrcat(dst_path, file_ext);

		if(mmi_phb_image_copy(dst_path, src_path))
			return TRUE;
	}

	//DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_IMAGE_COPY_FAIL), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_copy
* DESCRIPTION
*   Copy image to destination folder. Make sure the destination folder exists.
*
* PARAMETERS
*  void
* RETURNS
*  TRUE if copy successfully, else FALSE.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_phb_image_copy(S8* dst, S8* src)
{
	FS_HANDLE fh_src, fh_dst;
	U8 buf[256];
	INT32 read_len, write_len;
	S32 read_result = 0, write_result = 0;


	UINT i = 0;
	
	MMI_TRACE((MMI_TRACE_G4_PHB,MMI_PHB_IMAGE_COPY));

	/*Source and Destination can't be the same.*/
	if(pfnUnicodeStrcmp(dst,src) == 0)
	{
		MMI_TRACE((MMI_TRACE_G4_PHB,MMI_PHB_IMAGE_COPY_1));
		return FALSE;
	}

	fh_src = MMI_FS_Open((U8*)src, FS_READ_ONLY);
	if(fh_src < 0)
	{
		MMI_TRACE((MMI_TRACE_G4_PHB,MMI_PHB_IMAGE_COPY_2));
		return FALSE;
	}

	//fh_dst = MMI_FS_Open((U16*)dst, FS_CREATE_ALWAYS | FS_READ_WRITE);
	fh_dst = MMI_FS_Open((U8*)dst, FS_CREATE_ALWAYS );
	
	if(fh_dst < 0)
	{
		MMI_TRACE((MMI_TRACE_G4_PHB,MMI_PHB_IMAGE_COPY_3));
		MMI_FS_Close(fh_src);
		return FALSE;
	}

	MMI_FS_Seek(fh_dst, 0, FS_FILE_BEGIN);

	TRACE_FMGR_FUNCTION();
	
	for(;;)
	{
		i++;
		if ((read_result = MMI_FS_Read(fh_src,buf,256, &read_len)) == FS_NO_ERROR)
		{
			//MMI_FS_Seek(fh_dst, 0, FS_FILE_END);
			if(read_len < 256)   /*End of File*/
			{
				if ((write_result = MMI_FS_Write(fh_dst, buf, read_len, &write_len)) == FS_NO_ERROR)
				{
					TRACE_EF(g_sw_FMGR, "MMI_FS_Write(%d)  ",write_len);

					MMI_FS_Close(fh_dst);
					MMI_FS_Close(fh_src);
					return TRUE;
				}
				else
				{
					break;
				}

			}
			else if (read_len == 256)
			{
				if ((write_result = MMI_FS_Write(fh_dst, buf, read_len, &write_len)) == FS_NO_ERROR)
				{  
					TRACE_EF(g_sw_FMGR, "MMI_FS_Write(%d)  ",write_len);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	/*Copy Fail*/
	TRACE_FMGR_FUNCTION();
	MMI_TRACE((MMI_TRACE_G4_PHB,MMI_PHB_IMAGE_COPY_4,read_result, write_result));
	MMI_FS_Close(fh_dst);
	MMI_FS_Close(fh_src);
	MMI_FS_Delete((U8*)dst);
	return FALSE;

}
#endif

#if defined(__MMI_FILE_MANAGER__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_fs_check_disc_space
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#define PHB_IMAGE_SPACE_LEFT 1024*30
BOOL mmi_phb_fs_check_disc_space(S16* dir_name, kal_uint32 free_byte)
{
   kal_int32 result;
   FS_DiskInfo disc_info;
   kal_wchar dir[3];

    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
   memset(dir, 0, 6);
   memcpy(dir, dir_name, 4);
   
   TRACE_FMGR_FUNCTION();
   
   /* check the available free space */
   /*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 15:24 */
   result=MMI_FS_GetDiskInfo((U8*)dir, &disc_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
   /*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 15:24 */
	TRACE_EF(g_sw_FMGR, "GetDiskInfo result is %d and BytesPerSector is %d", result,disc_info.BytesPerSector);

   if(result>0 && result&(FS_DI_BASIC_INFO|FS_DI_FREE_SPACE))
   {
      if((disc_info.FreeClusters*disc_info.SectorsPerCluster*disc_info.BytesPerSector)< free_byte + PHB_IMAGE_SPACE_LEFT)
         return FALSE;
      else
         return TRUE;
   }
   else
   {
      return FALSE;
   }
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_check_valid_image
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_phb_image_check_valid_image(S8* path)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	FS_HANDLE fh;
	INT32 f_size = 0; /*U32 f_size = 0*/
	S8 dir_name[10];
	U16 image_type;
	S8* error_str_ptr;
	S8 dir_path[MMI_PHB_MAX_PATH_LENGTH];
	S32 result;

    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/*Check image file extension in advance.*/
	image_type = gdi_image_get_type_from_file((S8*)path);
	TRACE_EF(g_sw_FMGR, "image_type = %d", image_type);
	
	if ( (image_type != GDI_IMAGE_TYPE_JPG_FILE) &&( image_type != GDI_IMAGE_TYPE_GIF_FILE) &&
		(image_type != GDI_IMAGE_TYPE_BMP_FILE))
	{
		DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_IMAGE_NOT_SUPPORT), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		return FALSE;
	}

	/*Check if image folder exists */
    // dingjian 20080326 TODO..
	//sprintf(nPrintableStr, "%c:/%s",(S8)MMI_PHB_DRV, MMI_PHB_IMAGE_FOLDER);
	sprintf(nPrintableStr, "C:/%s", MMI_PHB_IMAGE_FOLDER);// dingjian 20080329 device name
	TRACE_EF(g_sw_FMGR, "nPrintableStr = %s", nPrintableStr);
	
	
	AnsiiToUnicodeString(dir_path, nPrintableStr);

	if((result = mmi_phb_fs_check_image_folder(dir_path)) != FS_NO_ERROR)
	{
		DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		return FALSE;
	}

	/*
	* Check image content, check if the image header can be parsed correctly
	* Check image "size", "width", "height", value can be customize in file manage app.
	*/
	if(!mmi_fmgr_util_file_limit_check(FMGR_LIMIT_CALLER_PICTURE, path, &error_str_ptr))
	{
		DisplayPopup((U8*)error_str_ptr, IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		return FALSE;
	}
	TRACE_FMGR_FUNCTION();
	/*Check disk free space for copy*/
	
	/*WUZC Modify Start For ShotCuts Ver:    on 2007-1-25 16:15 */
	fh = MMI_FS_Open((U8*)path, FS_READ_ONLY);
	/*WUZC Modify End  For ShotCuts Ver:    on 2007-1-25 16:15 */


	if ((fh>=0) && (MMI_FS_GetFileSize(fh, &f_size) == FS_NO_ERROR))
	{
		pfclose(fh);

		//sprintf(dir_name,"%c:\\", MMI_PHB_DRV);
		sprintf(dir_name,"C:\\");// dingjian 20080329 device name
		TRACE_EF(g_sw_FMGR, "dir_name is %s, and file size is %d", dir_name,f_size);
		AnsiiToUnicodeString(dir_path, dir_name);
		if(!mmi_phb_fs_check_disc_space((S16*)dir_path, f_size))
		{
			DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_DISC_FULL), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
			return FALSE;
		}
	}
	else
	{
		if(fh>=0)
		{
			pfclose(fh);
		}

		DisplayPopup((U8*)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_image_extract_ext_by_index
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_image_get_full_path_by_index(U16 record_index, S8* path)
{
	FS_HANDLE fh_cur;
	FS_DOSDirEntry file_info;
	S8 filename[20];
   mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
    mmi_trace(1, "-----+++++record_index= %d+++++-----", record_index);

	//sprintf(nPrintableStr,"%c:/%s/%d.?*", MMI_PHB_DRV, MMI_PHB_IMAGE_FOLDER, record_index);
	//sprintf(nPrintableStr,"C:/%s/%d.GIF", MMI_PHB_IMAGE_FOLDER, record_index);
	sprintf(nPrintableStr,"C:/%s/%d.?*", MMI_PHB_IMAGE_FOLDER, record_index);
	AnsiiToUnicodeString(path, nPrintableStr);
	TRACE_FMGR_FUNCTION();

	/*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 15:25 */
	if( (fh_cur = MMI_FS_FindFirst((U8*)path, 0, 0, &file_info, (U8*)filename, 20)) >= 0 )
	/*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 15:25 */
	{
		/* filter out folder results*/
		if( !(file_info.Attributes & FS_ATTR_DIR) )
		{
			//sprintf(nPrintableStr,"%c:/%s/", MMI_PHB_DRV, MMI_PHB_IMAGE_FOLDER);
			sprintf(nPrintableStr,"C:/%s/", MMI_PHB_IMAGE_FOLDER);// dingjian 20080329 device name
			AnsiiToUnicodeString(path, nPrintableStr);
			pfnUnicodeStrcat(path,filename);
		}
		else
			memset(path,0,ENCODING_LENGTH);

		pffindclose( fh_cur );
		return;
	}

	memset(path,0,ENCODING_LENGTH);
}

#endif

/*----------------------------------------------END of Image from File Manager------------------------------------------------*/
/*****************************************************************************
* FUNCTION
*  mmi_phb_fs_check_image_folder
* DESCRIPTION
*   Check if folder exists, if not create it.
*
* PARAMETERS
*  path IN  Full UCS2 path of desire folder.
* RETURNS
*  TRUE if exists or create successfully, else FALSE.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_phb_fs_check_image_folder(S8* path)
{

	FS_HANDLE fh;
	S32 result = FS_NO_ERROR;
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

	
	fh = MMI_FS_Open((U8*)path,  FS_OPEN_DIR | FS_READ_ONLY);
	/*Folder Exists*/
	if(fh == FS_FOLDER_HANDLE)
	{
		//FS_SetAttributes((U16*)path,FS_ATTR_DIR|FS_ATTR_HIDDEN);
		MMI_FS_Close(fh);
	}
	/*Folder Does Not Exist*/
	else
	{
		result = MMI_FS_CreateDir((U8*)path);
		/*Create Success*/
		if (result != FS_NO_ERROR)
		{
			//FS_SetAttributes((U16*)path,FS_ATTR_DIR|FS_ATTR_HIDDEN);
			TRACE_EF(g_sw_FMGR, "mmi_phb_fs_check_image_folder folder do not not not exit!!But create fail!!");
		}
	}
	return result;
	
}

#if defined(__MMI_INCOMING_CALL_VIDEO__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_video_init_data_store
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_video_init_data_store(void)
{
	S8 ascii_disc[10],tmp_filename[MMI_PHB_MAX_PATH_LENGTH * ENCODING_LENGTH];
	S8 path[MMI_PHB_MAX_PATH_LENGTH * ENCODING_LENGTH];
	S32 result;

	if(!g_phb_cntx.init_video)
	{
		sprintf(ascii_disc, "%c:/",(S8)MMI_PHB_DRV);
//		sprintf(ascii_disc, "C:/");// dingjian 20080329 device name
		AnsiiToUnicodeString(path, ascii_disc);
		AnsiiToUnicodeString(tmp_filename, (S8*)MMI_PHB_VIDEO_DATA_FILE);
/*Guojian Modify Start For phb_video Ver: Jassmine  on 2008-6-24 15:5 */
//		pfnUnicodeStrcat(path, (S8*)MMI_PHB_VIDEO_DATA_FILE);
		pfnUnicodeStrcat(path, (S8*)tmp_filename);
/*Guojian Modify End  For phb_video Ver: Jassmine  on 2008-6-24 15:5 */

		/*May need to check if init successfully here.*/
		result = mmi_data_init_record_store( &g_phb_video_record_store, (S8*)path, FMGR_MAX_PATH_LEN*ENCODING_LENGTH+ENCODING_LENGTH);

		if (result == 0)
		{
			g_phb_cntx.init_video = 1;
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_video_highlight_hdlr
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_video_highlight_hdlr(S32 index)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if(index == (g_phb_cntx.total_video_id - 1))
	{
		ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
		SetLeftSoftkeyFunction(mmi_phb_video_pre_entry_file_option, KEY_EVENT_UP);
	}
	else if(index !=0)
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

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_video_default_view(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	SetLeftSoftkeyFunction(mmi_phb_video_view_from_default,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_video_view_from_default,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_video_setting(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if(g_phb_cntx.video_audio)
		ChangeLeftSoftkey(STR_GLOBAL_OFF, IMG_GLOBAL_OK);
	else
		ChangeLeftSoftkey(STR_GLOBAL_ON, IMG_GLOBAL_OK);

	SetLeftSoftkeyFunction(mmi_phb_video_toggle_setting,KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_hint_video_setting(U16 index)
{
	if(g_phb_cntx.video_audio)
		pfnUnicodeStrcpy((S8*) hintData[index], GetString(STR_GLOBAL_ON));
	else
		pfnUnicodeStrcpy((S8*) hintData[index], GetString(STR_GLOBAL_OFF));
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_video_file_view(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	SetLeftSoftkeyFunction(mmi_phb_video_view_from_file,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_video_view_from_file,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_video_file_select(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	SetLeftSoftkeyFunction(mmi_phb_video_select_from_file_mamager,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_video_select_from_file_mamager,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_video_file_delete(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	SetLeftSoftkeyFunction(mmi_phb_video_delete_file_reference,KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_video_default_option
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_video_default_option(void)
{
	U8* guiBuffer;
	U16 nStrItemList[5];
	U16 nNumofItem;
	U8 *hintList[5]; /*Array for hint*/

	EntryNewScreen(SCR_ID_PHB_VIDEO_OPTION, NULL, mmi_phb_entry_video_default_option,NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_VIDEO_OPTION);		/* Getting the Index from history */

	nNumofItem = GetNumOfChild(MENU_ID_PHB_VIDEO_DEFAULT_OPTION);
	GetSequenceStringIds(MENU_ID_PHB_VIDEO_DEFAULT_OPTION, nStrItemList);
	SetParentHandler(MENU_ID_PHB_VIDEO_DEFAULT_OPTION);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* Construct List To Show Hint*/
	ConstructHintsList(MENU_ID_PHB_VIDEO_DEFAULT_OPTION, hintList);

	/*Screen with hint*/
	ShowCategory52Screen(STR_ID_PHB_CALLER_VIDEO,
								   IMG_SCR_PBOOK_CAPTION,
								   STR_GLOBAL_OK,
								   IMG_GLOBAL_OK,
								   STR_GLOBAL_BACK,
								   IMG_GLOBAL_BACK,
								   nNumofItem,
								   nStrItemList,
								   (U16*)gIndexIconsImageList,
								   hintList,
								   0,
								   0,
								   guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	g_phb_video_view = TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_video_pre_entry_file_option
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_video_pre_entry_file_option(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if(g_phb_cntx.video_location == MMI_PHB_IMAGE_NO_SELECT)
		mmi_phb_entry_video_file_option(2);
	else
		mmi_phb_entry_video_file_option(1);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_video_file_option
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_video_file_option(U8 type)
{
	U8* guiBuffer;
	U16 nStrItemList[5];
	U16 nNumofItem;
	U8 *hintList[5]; /*Array for hint*/
	U32 HideItemId = -1, HideItemId1 = -1, HideItemId2 = -1;
	U32 maskingByte = -1;
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);


	EntryNewScreen(SCR_ID_PHB_VIDEO_OPTION, NULL, mmi_phb_video_pre_entry_file_option,NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_VIDEO_OPTION);		/* Getting the Index from history */

	nNumofItem = GetNumOfChild(MENU_ID_PHB_VIDEO_FILE_OPTION);
	GetSequenceStringIds(MENU_ID_PHB_VIDEO_FILE_OPTION, nStrItemList);
	SetParentHandler(MENU_ID_PHB_VIDEO_FILE_OPTION);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* Construct List To Show Hint*/
	ConstructHintsList(MENU_ID_PHB_VIDEO_FILE_OPTION, hintList);

	/*Hide menu item play and delete when file not selected.*/
	/*Regiser highlight handler again.*/
	mmi_phb_operate_single_init();

	/*File not selected*/
	if(type == 2)
	{
		HideItemId = GetChildMenuIDIndexByParentMenuID(MENU_ID_PHB_VIDEO_FILE_OPTION,MENU_ID_PHB_VIDEO_FILE_VIEW);
		HideItemId1=	GetChildMenuIDIndexByParentMenuID(MENU_ID_PHB_VIDEO_FILE_OPTION,MENU_ID_PHB_VIDEO_FILE_DELETE);
		HideItemId2=	GetChildMenuIDIndexByParentMenuID(MENU_ID_PHB_VIDEO_FILE_OPTION,MENU_ID_PHB_VIDEO_FILE_SETTING);
		if((HideItemId != -1) && (HideItemId1 != -1) && (HideItemId2 != -1))
		{
			ResetBit(maskingByte,HideItemId);
			ResetBit(maskingByte,HideItemId1);
			ResetBit(maskingByte,HideItemId2);
			MaskHiliteItems(MENU_ID_PHB_VIDEO_FILE_OPTION, maskingByte);
			MaskStringItems(hintList,(U8)nNumofItem,maskingByte);
			nNumofItem=MaskItems(nStrItemList,(U8)nNumofItem,maskingByte);
		}
	}

	/*Screen with hint*/
	ShowCategory52Screen(STR_ID_PHB_CALLER_VIDEO,
								   IMG_SCR_PBOOK_CAPTION,
								   STR_GLOBAL_OK,
								   IMG_GLOBAL_OK,
								   STR_GLOBAL_BACK,
								   IMG_GLOBAL_BACK,
								   nNumofItem,
								   nStrItemList,
								   (U16*)gIndexIconsImageList,
								   hintList,
								   0,
								   0,
								   guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	g_phb_video_view = TRUE;
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_video_view_from_default(void)
{
	U16 video_id;

	video_id = g_phb_cntx.video_id_list[g_phb_cntx.selected_video_index];

	if(isInCall()) /*Can not preview video during call.*/
	{
		DisplayPopup((U8*)GetString(STR_ID_PHB_CANT_VIEW_VIDEO_IN_CALL), IMG_GLOBAL_WARNING, FALSE, PHB_NOTIFY_TIMEOUT, WARNING_TONE);
	}
	else if( (video_id != 0) && !(video_id & 0x8000))
	{
		mmi_vdoply_entry_player_screen_from_id(video_id, (U16)(STR_ID_PHB_VIDEO_1+g_phb_cntx.selected_video_index-1));
	}
	else
	{
		DisplayPopup((U8*)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}

}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_video_view_from_file(void)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if(isInCall())	/*Can not preview video during call.*/
	{
		DisplayPopup((U8*)GetString(STR_ID_PHB_CANT_VIEW_VIDEO_IN_CALL), IMG_GLOBAL_WARNING, FALSE, PHB_NOTIFY_TIMEOUT, WARNING_TONE);
	}
	else if((g_phb_cntx.selected_video_index == g_phb_cntx.total_video_id - 1) &&
		    (g_phb_cntx.video_location == MMI_PHB_IMAGE_SELECT_PATH))
	{
		FS_HANDLE file_handle = MMI_FS_Open((U8*)g_phb_video_file_path, FS_READ_ONLY);
		mmi_trace(TRUE," Func: %s file_handle=%d", __FUNCTION__,file_handle);
		if (file_handle >= 0)
		{
			pfclose(file_handle);
			mmi_vdoply_entry_player_screen_from_file(g_phb_video_file_path, FALSE);
		}
		else
		{
			DisplayPopup((U8*)GetString(GetFileSystemErrorString(file_handle)), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
	}
	else
	{
		DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_IMAGE_NO_SELECT), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	}
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_video_select_from_file_mamager(void)
{
	FMGR_FILTER filter;
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

	FMGR_FILTER_INIT(&filter);
	FMGR_FILTER_SET_VIDEO(&filter);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);

	mmi_fmgr_select_path_and_enter(APP_PHONEBOOK,
								FMGR_SELECT_FILE|FMGR_SELECT_REPEAT,
								filter,
								(S8*)L"root",
								mmi_phb_video_select_from_file_mamager_done);
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S8 g_callervideo_path[FMGR_MAX_PATH_LEN * ENCODING_LENGTH + ENCODING_LENGTH];
void mmi_phb_video_select_from_file_mamager_done(void* path, int is_short)
{
    //-----------------------------------------------
    S8 ttt[FMGR_MAX_PATH_LEN];
    S8 tt[FMGR_MAX_PATH_LEN];
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
    //-----------------------------------------------
    UCS2Strcpy((S8 *) g_callervideo_path, (S8 *) path);
    UnicodeToAnsii( ttt, g_callervideo_path);
    mmi_trace(g_sw_PHB, "-----+++++g_callervideo_path = %s+++++-----", ttt);
    //-----------------------------------------------
    UnicodeToAnsii( tt, path);
    mmi_trace(g_sw_PHB, "-----+++++GetCallImgPath = %s+++++-----", tt);
    //-----------------------------------------------
	if(path == NULL) 	/*User does not select file and go back directly.*/
	{
		GoBackToHistory(SCR_ID_PHB_VIDEO_OPTION);
		return;
	}

	/*Check File Path Length*/
	if(mmi_phb_video_check_data_space((S8*)path))
	{
		pfnUnicodeStrcpy((S8*)g_phb_video_file_path, (S8*)path);
		g_phb_cntx.video_location = MMI_PHB_IMAGE_SELECT_PATH;

		fmgr_reset_app_select();	/*Use select_repeat, so clear by application itself.*/
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
		DeleteUptoScrID(SCR_ID_PHB_VIDEO_OPTION);
	}
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_video_delete_file_reference(void)
{
	if(g_phb_cntx.video_location == MMI_PHB_IMAGE_NO_SELECT)
		DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_IMAGE_NO_SELECT), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
	else
	{
		g_phb_cntx.video_location = MMI_PHB_IMAGE_NO_SELECT;
		DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	}
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_video_toggle_setting(void)
{
	U16 index;
	U16 str_LSK = STR_GLOBAL_OFF, str_hint = STR_GLOBAL_ON;

	index = GetCurrHiliteID();

	if(g_phb_cntx.video_audio)
	{
		g_phb_cntx.video_audio = 0;
		str_LSK = STR_GLOBAL_ON;
		str_hint = STR_GLOBAL_OFF;
		//DisplayPopup((U8*)GetString(STR_GLOBAL_OFF), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	}
	else
	{
		g_phb_cntx.video_audio = 1;
		//DisplayPopup((U8*)GetString(STR_GLOBAL_ON), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	}

	ChangeLeftSoftkey(str_LSK,IMG_GLOBAL_OK);
	pfnUnicodeStrcpy((S8*) hintData[index], GetString(str_hint));
	Category52ChangeItemDescription(index,hintData[index]);

	/* Redraw the Category Screen As Hint Handler and LSK are going to Chnage*/
	RedrawCategoryFunction();
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_video_get_id_by_index(U16 store_index)
{
	S16 pError;
	U8 vid_record;
	U8 vid_index;
	U16 video_ids[NVRAM_EF_PHB_VIDEO_COUNT];

	 /*Save Video ID to NVRAM*/
	vid_record = (store_index  / NVRAM_EF_PHB_VIDEO_COUNT) + 1;
	vid_index = store_index  - (vid_record -1) * NVRAM_EF_PHB_VIDEO_COUNT;

	ReadRecord(NVRAM_EF_PHB_VIDEO_LID, vid_record, (void*)video_ids, NVRAM_EF_PHB_VIDEO_SIZE, &pError);

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s video_ids[%d]:%d", __FUNCTION__, vid_index, video_ids[vid_index]);
	return video_ids[vid_index];
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_phb_video_get_path_by_index(U16 video_id)
{
	U16 data_id;
	S32 data_result;

	/*Check if an external resource id*/
	if(video_id & 0x8000)
	{
		/*Get data record id and load the path*/
		data_id = video_id & 0x3FFF;
		data_result = mmi_data_get_record(&g_phb_video_record_store, data_id, (void*)g_phb_video_file_path);

		if(data_result == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S8* mmi_phb_caller_video_get_path_by_index(U16 video_id)
{
	U16 data_id;
	S32 data_result;
    mmi_trace(g_sw_PHB, "MMI_Call: Func: %s video_id:0x%x", __FUNCTION__, video_id);

	/*Check if an external resource id*/
	if(video_id & 0x8000)
	{
		/*Get data record id and load the path*/
		data_id = video_id & 0x3FFF;
		data_result = mmi_data_get_record(&g_phb_video_record_store, data_id, (void*)phb_caller_video_file_path);

		if(data_result == 0)
		{
			return (S8*)phb_caller_video_file_path;
		}
	}
	return NULL;
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_video_delete_by_index(U16 store_index)
{
	S16 pError;
	U8 vid_record;
	U8 vid_index;
	U16 video_ids[NVRAM_EF_PHB_VIDEO_COUNT];
	U16 video_id;

	 /*Delete Video ID from NVRAM*/
	vid_record = (store_index  / NVRAM_EF_PHB_VIDEO_COUNT) + 1;
	vid_index = store_index  - (vid_record -1) * NVRAM_EF_PHB_VIDEO_COUNT;

	ReadRecord(NVRAM_EF_PHB_VIDEO_LID, vid_record, (void*)video_ids, NVRAM_EF_PHB_VIDEO_SIZE, &pError);
	video_id = video_ids[vid_index];

	if(video_id != 0)
	{
		video_ids[vid_index] = 0;

		WriteRecord(NVRAM_EF_PHB_VIDEO_LID, vid_record, (void*)video_ids, NVRAM_EF_PHB_VIDEO_SIZE, &pError);

		/*Delete data record if video from file path*/
		if(video_id & 0x8000)
		{
			video_id &= 0x3FFF;
			mmi_data_delete_record(&g_phb_video_record_store, video_id);
		}
	}
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_phb_video_duplicate_by_index(U16 dest_index, U16 src_index)
{
	S16 pError;
	U8 src_vid_record, dst_vid_record;
	U8 vid_index;
	U16 video_ids[NVRAM_EF_PHB_VIDEO_COUNT];
	U16 video_id;
	S32 data_result;
	U16 data_id;
	BOOL is_success = TRUE;

	/*read source video id*/
	src_vid_record = (src_index  / NVRAM_EF_PHB_VIDEO_COUNT) + 1;
	vid_index = src_index  - (src_vid_record -1) * NVRAM_EF_PHB_VIDEO_COUNT;
	ReadRecord(NVRAM_EF_PHB_VIDEO_LID, src_vid_record, (void*)video_ids, NVRAM_EF_PHB_VIDEO_SIZE, &pError);
	video_id = video_ids[vid_index];

	/*Read Destination video id*/
	dst_vid_record = (dest_index  / NVRAM_EF_PHB_VIDEO_COUNT) + 1;
	vid_index = dest_index  - (dst_vid_record -1) * NVRAM_EF_PHB_VIDEO_COUNT;
	if(dst_vid_record != src_vid_record)
	{
		ReadRecord(NVRAM_EF_PHB_VIDEO_LID, dst_vid_record, (void*)video_ids, NVRAM_EF_PHB_VIDEO_SIZE, &pError);
	}

	/*Duplicate File, an external file path*/
	if(video_id & 0x8000)
	{
		/*Get source path*/
		data_id = video_id & 0x3FFF;
		data_result = mmi_data_get_record(&g_phb_video_record_store, data_id, (void*)g_phb_video_file_path);

		/*Get external file and video's audio bits*/
		video_id &= 0xC000;

		/*Write to destination record and get data id*/
		if((data_result == 0) && \
			(mmi_data_add_record(&g_phb_video_record_store, &data_id, (void*)g_phb_video_file_path) == 0))
		{
				video_id |= data_id;	/*Set the new data id into bits*/
		}
		else
		{
			video_id = 0;
			is_success = FALSE;	/*data record add fail*/
		}
	}

	/*Duplicate ID*/
	video_ids[vid_index] = video_id;
	WriteRecord(NVRAM_EF_PHB_VIDEO_LID, dst_vid_record, (void*)video_ids, NVRAM_EF_PHB_VIDEO_SIZE, &pError);

	return is_success;
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_phb_video_update_by_index(U16 store_index)
{
	S16 pError;
	U8 vid_record;
	U8 vid_index;
	U16 video_ids[NVRAM_EF_PHB_VIDEO_COUNT];
	BOOL update_result;

	mmi_trace(g_sw_PHB,"Func: %s store_index:%d", __FUNCTION__, store_index);
	/*Retrieve video id list from NVRAM*/
	vid_record = (store_index  / NVRAM_EF_PHB_VIDEO_COUNT) + 1;
	vid_index = store_index  - (vid_record -1) * NVRAM_EF_PHB_VIDEO_COUNT;
	ReadRecord(NVRAM_EF_PHB_VIDEO_LID, vid_record, (void*)video_ids, NVRAM_EF_PHB_VIDEO_SIZE, &pError);

	/*Wrtie to data record file*/
	video_ids[vid_index] = mmi_phb_video_update_by_index_for_path(video_ids[vid_index], &update_result);

	/*Write back to NVRAM*/
	WriteRecord(NVRAM_EF_PHB_VIDEO_LID, vid_record, (void*)video_ids, NVRAM_EF_PHB_VIDEO_SIZE, &pError);

	/*If data to data record structure fail.*/
	return update_result;
}

/*****************************************************************************
* FUNCTION
*
* DESCRIPTION
*
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_video_update_by_index_for_path(U16 original_id, BOOL *update_result)
{
	U16 data_id;
	U16 video_id;
	S32 data_result = 0;

	*update_result = TRUE;

	mmi_trace(g_sw_DataMgr,"Func: %s original_id:0x%x", __FUNCTION__, original_id);
	/*Remove data record first, then check if need to add it back*/
	if(original_id & 0x8000)
	{
		data_id = original_id & 0x3FFF;
		mmi_data_delete_record(&g_phb_video_record_store, data_id);
	}

	/*Assign new selected video id*/
	video_id = g_phb_cntx.video_id_list[g_phb_cntx.selected_video_index];

	/*Video From Outside Path, Save path to data record structure*/
	if(g_phb_cntx.selected_video_index == g_phb_cntx.total_video_id - 1)
	{
		if ( g_phb_cntx.video_location == MMI_PHB_IMAGE_SELECT_PATH)
		{
			data_result = mmi_data_add_record(&g_phb_video_record_store, &data_id, (void*)g_phb_video_file_path);
			if (data_result == FS_FILE_NOT_FOUND)
			{
				data_result = mmi_data_add_record(&g_phb_video_record_store, &data_id, (void*)g_phb_video_file_path);
			}

			if(data_result == 0)
			{
				video_id |= data_id;
			}
			else	/*Save path to file fail*/
			{
				video_id = 0;
				*update_result = FALSE;
			}
		}
		else /* didn't select the file path */
		{
			video_id = 0;
		}
	}

	/*Assign Video's Audio bit*/
	if(video_id != 0)
	{
		if(g_phb_cntx.video_audio)
			video_id |= 0x4000;
		else
			video_id &= 0xBFFF;
	}

	return video_id;
}

/*****************************************************************************
* FUNCTION
*  	mmi_phb_video_check_data_space()
* DESCRIPTION
*   Check free space in data record before save to phonebook.
*
* PARAMETERS
*  void
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_phb_video_check_data_space(S8* path)
{
    S8* error_str_ptr;
    if (mmi_data_check_space_for_add_record(&g_phb_video_record_store) != 0)
    {
        DisplayPopup((U8*)GetString(STR_ID_PHB_FMGR_DISC_FULL), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
        return FALSE;
    }
    if (path && !mmi_fmgr_util_file_limit_check(FMGR_LIMIT_CALLER_VIDEO, path, &error_str_ptr))
    {
        if(!isInCall())
        {   
            DisplayPopup((U8*)error_str_ptr, IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
        }
        else
        {
            DisplayPopup((U8*)GetString(STR_ID_PHB_CANT_VIEW_VIDEO_IN_CALL), IMG_GLOBAL_WARNING, FALSE, PHB_NOTIFY_TIMEOUT, WARNING_TONE);
        }
        return FALSE;
    }
    return TRUE;
}

#endif /*END #if defined(__MMI_INCOMING_CALL_VIDEO__)*/

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_util_clear_bday_buffer
* DESCRIPTION
*   Clears birthday buffers
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   phonebook temp buffer for extra field
*****************************************************************************/
void mmi_phb_util_clear_bday_buffer(void)
{
	AnsiiToUnicodeString(pbYear, (S8*)MMI_PHB_BDAY_YEAR_STR);
	AnsiiToUnicodeString(pbMon, (S8*)MMI_PHB_BDAY_MON_STR);
	AnsiiToUnicodeString(pbDay, (S8*)MMI_PHB_BDAY_DAY_STR);

#ifdef NEVER
	pfnUnicodeStrcpy(pbYear, (S8*)MMI_PHB_BDAY_YEAR_STR);
	pfnUnicodeStrcpy(pbMon, (S8*)MMI_PHB_BDAY_MON_STR);
	pfnUnicodeStrcpy(pbDay, (S8*)MMI_PHB_BDAY_DAY_STR);
#endif /* NEVER */
	bday_modified = FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_bday_set_changed
* DESCRIPTION
*
*
* PARAMETERS
*  None
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_bday_set_changed(void)
{
	bday_modified = TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_bday_changed_callback
* DESCRIPTION
*
*
* PARAMETERS
*  a  IN       changed item index
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
static void mmi_phb_bday_changed_callback(U16 index)
{
	if (index == bday_inline_index)
	{
		bday_modified = TRUE;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_bday_editor_callback
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
void mmi_phb_bday_editor_callback(U8 *DateStr, U8 *day,U8 *month,U8 *year)
{
	if (bday_modified)
	{
		pfnUnicodeStrcpy((S8*)DateStr,(S8*)year);
		pfnUnicodeStrcat((S8*)DateStr,(S8*)"/");
		pfnUnicodeStrcat((S8*)DateStr,(S8*)month);
		pfnUnicodeStrcat((S8*)DateStr,(S8*)"/");
		pfnUnicodeStrcat((S8*)DateStr,(S8*)day);
	}
	else
	{
		pfnUnicodeStrcpy((S8*)DateStr,(S8*)(PU8)GetString(STR_GLOBAL_NONE));
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_bday_read_data_to_buff
* DESCRIPTION
*
*
* PARAMETERS
*  a  IN       store index
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_bday_read_data_to_buff(U16 store_index)
{
	U16 bday_record, bday_index;
	S16 pError;
	S8 ascii_buff[6];
	MMI_PHB_BDAY_STRUCT bday_data[NVRAM_EF_PHB_BIRTHDAY_COUNT];
    /*wangrui add and reopen del code begin 20081201 for fixbug10497 */
	S8	UCS2_Day[3*ENCODING_LENGTH];
	S8	UCS2_Mon[3*ENCODING_LENGTH];
	S8	UCS2_Year[6*ENCODING_LENGTH];
    /*wangrui add and reopen del code end */

	bday_record = (store_index  / NVRAM_EF_PHB_BIRTHDAY_COUNT) + 1;
	bday_index = store_index  - (bday_record -1) * NVRAM_EF_PHB_BIRTHDAY_COUNT;

	ReadRecord(NVRAM_EF_PHB_BIRTHDAY_LID, bday_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);

	/*Year*/
	if(bday_data[bday_index].bYear <= MMI_PHB_BDAY_MAX_YEAR_INT)
	{
		sprintf(ascii_buff, "%04d", bday_data[bday_index].bYear);
		AnsiiToUnicodeString(pbYear, ascii_buff);
	}
	else
	{
//		pfnUnicodeStrcpy(pbYear, (S8*)MMI_PHB_BDAY_YEAR_STR);
		AnsiiToUnicodeString(pbYear, (S8*)MMI_PHB_BDAY_YEAR_STR);
	}

	/*Month*/
	if(bday_data[bday_index].bMonth <= 12)
	{
		sprintf(ascii_buff, "%02d", bday_data[bday_index].bMonth);
		AnsiiToUnicodeString(pbMon, ascii_buff);
	}
	else
	{
//		pfnUnicodeStrcpy(pbMon, (S8*)MMI_PHB_BDAY_MON_STR);
		AnsiiToUnicodeString(pbMon, (S8*)MMI_PHB_BDAY_MON_STR);
	}

	/*Day*/
	if(bday_data[bday_index].bDay <= 31)
	{
		sprintf(ascii_buff, "%02d", bday_data[bday_index].bDay);
		AnsiiToUnicodeString(pbDay, ascii_buff);
	}
	else
	{
	//	pfnUnicodeStrcpy(pbDay, (S8*)MMI_PHB_BDAY_DAY_STR);
		AnsiiToUnicodeString(pbDay, (S8*)MMI_PHB_BDAY_DAY_STR);
	}

    /*wangrui add and reopen del code begin 20081201 for fixbug10497 */
	AnsiiToUnicodeString(UCS2_Year, (S8*)MMI_PHB_BDAY_YEAR_STR);
	AnsiiToUnicodeString(UCS2_Mon, (S8*)MMI_PHB_BDAY_MON_STR);
	AnsiiToUnicodeString(UCS2_Day, (S8*)MMI_PHB_BDAY_DAY_STR);

/*Guojian Del Start For 9825 Ver: GreenStone  on 2008-10-15 17:29 */
	if (pfnUnicodeStrcmp((const S8*)pbYear, UCS2_Year) != 0 ||
		pfnUnicodeStrcmp((const S8*)pbMon, UCS2_Mon) != 0 ||
		pfnUnicodeStrcmp((const S8*)pbDay, UCS2_Day) != 0)
	{
		bday_modified = TRUE;
	}
/*Guojian Del End  For 9825 Ver: GreenStone  on 2008-10-15 17:29 */
    /*wangrui add and reopen del code end */
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_bday_update_field
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
void mmi_phb_bday_update_field(U16 store_index)
{
	U16 bday_record, bday_index;
	S16 pError;
	U16 year,month,day;
	S8 ascii_buff[6];
	MMI_PHB_BDAY_STRUCT bday_data[NVRAM_EF_PHB_BIRTHDAY_COUNT];

	UnicodeToAnsii(ascii_buff, pbYear);
	year = atoi(ascii_buff);
	UnicodeToAnsii(ascii_buff, pbMon);
	month = atoi(ascii_buff);
	UnicodeToAnsii(ascii_buff, pbDay);
	day = atoi(ascii_buff);

	bday_record = (store_index  / NVRAM_EF_PHB_BIRTHDAY_COUNT) + 1;
	bday_index = store_index  - (bday_record -1) * NVRAM_EF_PHB_BIRTHDAY_COUNT;

	ReadRecord(NVRAM_EF_PHB_BIRTHDAY_LID, bday_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);

	bday_data[bday_index].bYear = year;
	bday_data[bday_index].bMonth = (U8)month;
	bday_data[bday_index].bDay = (U8)day;

	WriteRecord(NVRAM_EF_PHB_BIRTHDAY_LID, bday_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_bday_delete_field
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
void mmi_phb_bday_delete_field(U16 store_index)
{
	U16 bday_record, bday_index;
	S16 pError;
	MMI_PHB_BDAY_STRUCT bday_data[NVRAM_EF_PHB_BIRTHDAY_COUNT];

	bday_record = (store_index  / NVRAM_EF_PHB_BIRTHDAY_COUNT) + 1;
	bday_index = store_index  - (bday_record -1) * NVRAM_EF_PHB_BIRTHDAY_COUNT;

	ReadRecord(NVRAM_EF_PHB_BIRTHDAY_LID, bday_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);

	memset(&bday_data[bday_index], 0, sizeof(MMI_PHB_BDAY_STRUCT));

	WriteRecord(NVRAM_EF_PHB_BIRTHDAY_LID, bday_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_bday_duplicate_field
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
void mmi_phb_bday_duplicate_field(U16 dest_index,  U16 src_index)
{
	S16 pError;
	U16 src_record, dst_record;
	U16 bday_index;
	MMI_PHB_BDAY_STRUCT bday_data[NVRAM_EF_PHB_BIRTHDAY_COUNT];
	MMI_PHB_BDAY_STRUCT bday_record;

	/*read source bday field*/
	src_record = (src_index / NVRAM_EF_PHB_BIRTHDAY_COUNT) + 1;
	bday_index = src_index - (src_record -1) * NVRAM_EF_PHB_BIRTHDAY_COUNT;
	ReadRecord(NVRAM_EF_PHB_BIRTHDAY_LID, src_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);

	memcpy(&bday_record, &bday_data[bday_index], sizeof(MMI_PHB_BDAY_STRUCT));

	/*Read Destination video id*/
	dst_record = (dest_index / NVRAM_EF_PHB_BIRTHDAY_COUNT) + 1;
	bday_index = dest_index - (dst_record -1) * NVRAM_EF_PHB_BIRTHDAY_COUNT;
	if(dst_record != src_record)
	{
		ReadRecord(NVRAM_EF_PHB_BIRTHDAY_LID, dst_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);
	}

	/*Duplicate ID*/
	memcpy(&bday_data[bday_index], &bday_record, sizeof(MMI_PHB_BDAY_STRUCT));
	WriteRecord(NVRAM_EF_PHB_BIRTHDAY_LID, dst_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);
}

#endif /* __MMI_PHB_BIRTHDAY_FIELD__ */

U16 mmi_phb_get_email_address_id(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index == 1)
    {
    #if defined(__MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__)
        return STR_ID_PHB_EMAIL1_ADRESS;
    #else
        return STR_EMAIL_ADRESS;
    #endif
    }
#if defined(__MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__)
    else if (index == 2)
    {
        return STR_ID_PHB_EMAIL2_ADRESS;
    }
#endif
    else
    {
        MMI_ASSERT(0);
    }

    return 0;
}

#endif /* _PHONEBOOKOPERATESINGLE_C */

