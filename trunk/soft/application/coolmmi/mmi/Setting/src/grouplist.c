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
 *	grouplist.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   <file description>.
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


//micha0908
#ifndef  __MMI_REMOVE_CUG__

/**************************************************************

	FILENAME	: GroupList.c

  	PURPOSE		: Screens for GroupList application

 

 

	DATE		: 5/5/2003

**************************************************************/
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"

#include "mainmenudef.h"
#include "settingdefs.h"
#include "wgui_categories.h"
#include "protocolevents.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "settingprofile.h"
#include "callsetup.h"
#include "debuginitdef.h"
#include "callsetupenum.h"
#include "profilegprots.h"
#include "commonscreens.h"
#undef __NEWSIMULATOR

 
#include "wgui_categories_popup.h"
#include "wgui_categories_inputs.h"
#include "callsdefs.h"


#define	MAX_CUG_GROUP_NUM		11
#define	MAX_CUG_NAME_LENGTH		16
#define	NVRAM_MAX_GROUP_NAME_SIZE		36


typedef struct CUGItemStruct
{
	U8 name[(MAX_CUG_NAME_LENGTH * ENCODING_LENGTH) + 2 ];
	U8 index;
} CUGItemStruct;


typedef struct {
	CUGItemStruct GroupList[MAX_CUG_GROUP_NUM];
	CUGItemStruct GroupItemInput;
	U8 InputIndexBuf[3 * ENCODING_LENGTH];
	U8 NumOfList;
	U8 SelectIndex;
	U8 DeactiveThenDelete;
} CUGContext;


CUGContext g_cug_context;


void EntryCALLSETCUGEditOptions( void );
void HighlightEditOptionsDone(void);
void HighlightEditOptionsInputMethod(void);
void InitNvramCUG(void);
void AddNewGroup(void);
void HighlightGroupList(S32 index);
void EntryCALLSETCUGMain(void);
void EntryCALLSETCUGOptions(void);
void HighlightCALLSETCUGActive(void);
void EntryGroupListActivate(void);
void CALLSETCUGActiveRsp(void * info);
void HighlightCALLSETCUGDeactive(void);
void EntryGroupListDeactivate(void);
void CALLSETCUGDeactiveRsp(void * info);
U8 IsValidateIndex(U8 index);
void HighlightCALLSETCUGAdd(void);
void EntryCALLSETCUGAddList(void);
void AddEntryToNVRAM(void );
void EntryCALLSETCUGAddListIndex(void);
void HighlightCALLSETCUGEdit(void);
void GroupListDelete(void);
void EntryCALLSETCUGDelListCfrm( void );
void EntryCALLSETCUGEditList(void);
void EditEntryNVRAM(void );
void HighlightCALLSETCUGDelete(void);
void GoBackTwoScreens(void);
void EntryCALLSETCUGEditListIndex(void);
void ExitCALLSETCUGAddListIndex(void);


/**************************************************************

	FUNCTION NAME		: InitGroupListApp(void)

  	PURPOSE				: Initialize GroupList Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: S16

	RETURNS				: void

 

**************************************************************/
S16 InitGroupListApp(void)
{	
	/* set the menu hilite handlers */
	SetHiliteHandler(MAIN_MENU_GROUPLIST_ACTIVATE,HighlightCALLSETCUGActive);
	SetHiliteHandler(MAIN_MENU_GROUPLIST_DEACTIVATE,HighlightCALLSETCUGDeactive);
	SetHiliteHandler(MAIN_MENU_GROUPLIST_ADD,HighlightCALLSETCUGAdd);
	SetHiliteHandler(MAIN_MENU_GROUPLIST_DELETE,HighlightCALLSETCUGDelete);
	SetHiliteHandler(MAIN_MENU_GROUPLIST_EDIT,HighlightCALLSETCUGEdit);	
	SetHiliteHandler(MENU_CUG_DONE_OPTION_ID,HighlightEditOptionsDone);
	SetHiliteHandler(MENU_CUG_INPUT_METHOD_OPTION_ID,HighlightEditOptionsInputMethod);
	return 0;		
}
/**************************************************************

	FUNCTION NAME		: InitNvramCUG(void)

  	PURPOSE				: Initialize NVRAM for GroupList

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: S16

	RETURNS				: void

 

**************************************************************/
void InitNvramCUG(void)
{
	S16 error ;
	U8	j;
	/* Initialize the group list by reading from NVRAM */
	for(j=0;j< MAX_CUG_GROUP_NUM;j++)
    {
		ReadRecord (NVRAM_EF_GROUPLIST_LID, (U8)(j+1) , &g_cug_context.GroupList[j], NVRAM_MAX_GROUP_NAME_SIZE, 
		        &error);
		
		if( g_cug_context.GroupList[j].name[0] != 0xFF)
			g_cug_context.NumOfList++;
		else
			break;
	}
}
/**************************************************************

	FUNCTION NAME		: HighlightGroupList(void)

  	PURPOSE				: Highlight grouplist menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightGroupList(S32 index)
{
	/* updating the highlited index */
	g_cug_context.SelectIndex = (U8) index;
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETCUGMain(void)

  	PURPOSE				: Showing grouplist screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETCUGMain(void)
{
	U8 *nStrItemList[MAX_CUG_GROUP_NUM]; /* Stores the strings id of submenus returned */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 i;
	static U8 firstTime = 0;
	/* if its not for the first time initialize the nvram for CUG */
	if( firstTime != 1)
	{
		firstTime = 1;
		/* initialize nvram */
		InitNvramCUG();
	}

	EntryNewScreen(SCR_ID_CALLSET_CUG_MAIN, NULL, EntryCALLSETCUGMain, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_CUG_MAIN);	
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler( HighlightGroupList);
	for(i=0;i< g_cug_context.NumOfList;i++)
	{
		nStrItemList[i] = (U8 *)g_cug_context.GroupList[i].name;
	}
	if(  g_cug_context.NumOfList == 0)
	{
		/* if the list is empty show empty list */
		nStrItemList[0] = (U8*)GetString(STR_GLOBAL_EMPTY_LIST);
		/* Display corresponding Screen */
		ShowCategory6Screen(STR_GROUPLIST_CAPTION,MAIN_MENU_TITLE_SETTINGS_ICON,
			STR_GLOBAL_ADD, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,1, nStrItemList,NULL,
						  0,guiBuffer);
		/* Register function with left/right softkey */
		SetLeftSoftkeyFunction(EntryCALLSETCUGAddList,KEY_EVENT_UP);
//micha0529
		SetKeyHandler(EntryCALLSETCUGAddList, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);	
	}
	else
	{

//micha0426
		if( guiBuffer != NULL)
		{
			memset( (void *) guiBuffer, 0, 2);
		if ( g_cug_context.SelectIndex >= g_cug_context.NumOfList)  {
			g_cug_context.SelectIndex = g_cug_context.NumOfList-1;
		}
		}
		else {
			g_cug_context.SelectIndex = 0;
		}

		/* Display corresponding Screen */
		ShowCategory84Screen(STR_GROUPLIST_CAPTION,MAIN_MENU_TITLE_SETTINGS_ICON,
							  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,g_cug_context.NumOfList, nStrItemList,
							  (U16 *)gIndexIconsImageList,1,(S32) g_cug_context.SelectIndex,guiBuffer);
		/* Register function with left/right softkey */
		SetLeftSoftkeyFunction(EntryCALLSETCUGOptions,KEY_EVENT_UP);
//micha0522
		SetKeyHandler(EntryCALLSETCUGOptions, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}	
	/* Register function with left/right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* Register left arrow key */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETCUGOptions(void)

  	PURPOSE				: Showing grouplist Options Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void GBSetInputboxLSKFunction(FuncPtr f);
void EntryCALLSETCUGOptions(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U16 menuId;

	EntryNewScreen(SCR_ID_CALLSET_CUG_OPTIONS, NULL, EntryCALLSETCUGOptions, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_CUG_OPTIONS);	
	/* check if the list is full or not*/
	if (g_cug_context.NumOfList == 10)
	{
		/* if the list is full remove the add option */
		menuId = MAIN_MENU_GROUPLIST_OPTIONS_MENUID_SUB;
	}
	else
	{
		/* if the list is not full */
		menuId = MAIN_MENU_GROUPLIST_OPTIONS_MENUID;
	}
    /* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(menuId);
	/* Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(menuId);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(menuId,nStrItemList);
	/* Set current parent id*/
	SetParentHandler(menuId);
	if( g_cug_context.NumOfList == 0)
	{
		/* if the list is empty */
		/* Display Category15 Screen */
		ShowCategory15Screen(STR_GROUPLIST_OPTION_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
							  STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,nNumofItem=1, nStrItemList+1,
							  (U16 *)gIndexIconsImageList,1,0,guiBuffer);
		/* Register function with left/right softkey */
		SetLeftSoftkeyFunction( EntryCALLSETCUGAddList, KEY_EVENT_UP);
	}
	else
	{
		/* if the list is not empty */
		/* Register highlight handler to be called in menu screen */
		RegisterHighlightHandler (ExecuteCurrHiliteHandler);
		/* Display Category15 Screen */
		ShowCategory15Screen(STR_GROUPLIST_OPTION_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
							  STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,nNumofItem, nStrItemList,
							  (U16 *)gIndexIconsImageList,1,0,guiBuffer);
#if defined(__MMI_GB__)                                                          //add by yaosq 20061113 
		GBSetInputboxLSKFunction(EntryCALLSETCUGEditOptions); 
#endif
	}
	/* Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
//micha0522
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**********************************************************************

	FUNCTION NAME		: HighlightCALLSETCUGActive(void)

  	PURPOSE				: Highlight grouplist Activate Options menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**********************************************************************/
void HighlightCALLSETCUGActive(void)
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryGroupListActivate,KEY_EVENT_UP);
	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryGroupListActivate(void)

  	PURPOSE				: function for sending req for grouplist Activate

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryGroupListActivate(void)
{
	mmi_cc_set_cug_req_struct 	*dataPtr;
	MYQUEUE Message;

	ClearAllKeyHandler();
	
	 
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Message.oslMsgId = PRT_MMI_CC_SET_CUG_REQ;
	dataPtr =  (mmi_cc_set_cug_req_struct *) OslConstructDataPtr(sizeof(mmi_cc_set_cug_req_struct));
	dataPtr->index = g_cug_context.GroupList[g_cug_context.SelectIndex].index;
	dataPtr->mode = CSMCC_ENABLE_CUG_TEMPORARY_MODE;
	dataPtr->info = 0;
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;
	/* set rsp handler for set cug activate req */
	SetProtocolEventHandler( CALLSETCUGActiveRsp, PRT_MMI_CC_SET_CUG_RSP);
	 
	OslMsgSendExtQueue(&Message);
	return;
}
/**************************************************************

	FUNCTION NAME		: CALLSETCUGActiveRsp(void *)

  	PURPOSE				: function for processing rsp for grouplist Activate req 

	INPUT PARAMETERS	: void * 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETCUGActiveRsp(void * info)
{
	
	mmi_cc_set_cug_rsp_struct *rsp;
//	S16 error ;	
	rsp = (mmi_cc_set_cug_rsp_struct *) info;
	/* if not for the for the first time */
	if( rsp->result == 1)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,CS_NOTIFYDURATION,SUCCESS_TONE);
		g_cug_context.GroupList[g_cug_context.SelectIndex].index += 1;
//		WriteValue(NVRAM_GROUPINDEX, &g_cug_context.GroupList[g_cug_context.SelectIndex].index, DS_BYTE, &error); 
		g_cug_context.GroupList[g_cug_context.SelectIndex].index -= 1;
	}
	else
	{
		/* incase of failure display the not done popup */
		DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE) ,IMG_GLOBAL_UNFINISHED, 1,CS_NOTIFYDURATION,ERROR_TONE);
	}
	/* delete screens for maitaining hte history */
	DeleteUptoScrID(SCR_ID_CALLSET_CUG_MAIN);
	return;
}
/**********************************************************************

	FUNCTION NAME		: HighlightCALLSETCUGDeactive(void)

  	PURPOSE				: Highlight grouplist Deactivate Options menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**********************************************************************/
void HighlightCALLSETCUGDeactive(void)
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryGroupListDeactivate,KEY_EVENT_UP);
	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryGroupListDeactivate(void)

  	PURPOSE				: function for sending req for grouplist deactivate

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryGroupListDeactivate(void)
{
	mmi_cc_set_cug_req_struct 	*dataPtr;
	MYQUEUE Message;

	ClearAllKeyHandler();
	
	 
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Message.oslMsgId = PRT_MMI_CC_SET_CUG_REQ;
	dataPtr =  (mmi_cc_set_cug_req_struct *) OslConstructDataPtr(sizeof(mmi_cc_set_cug_req_struct));
	dataPtr->index = g_cug_context.GroupList[g_cug_context.SelectIndex].index;
	dataPtr->mode = CSMCC_DISABLE_CUG_TEMPORARY_MODE;
	dataPtr->info = 0;
	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;
	/* set rsp handler for set cug deactivate req */
	SetProtocolEventHandler( CALLSETCUGDeactiveRsp, PRT_MMI_CC_SET_CUG_RSP);
	 
	OslMsgSendExtQueue(&Message);
	return;
}
/**************************************************************

	FUNCTION NAME		: CALLSETCUGDeactiveRsp(void *)

  	PURPOSE				: function for processing rsp for grouplist deactivate req 

	INPUT PARAMETERS	: void * 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETCUGDeactiveRsp(void * info)
{
	
	mmi_cc_set_cug_rsp_struct *rsp;
	/* check the status of the response result */
	rsp = (mmi_cc_set_cug_rsp_struct *) info;
	/* check if the flow is from deactivate or from delete option */
	if( g_cug_context.DeactiveThenDelete == 1)
	{
		/* if its from delete then delete the item */
		g_cug_context.DeactiveThenDelete = 0;
		if (rsp->result == 1)
		{
			/* incase of successful deactivate delete the item from list */
			GroupListDelete();
			return;
		}
	}
	if( rsp->result == 1)
	{
		/* in case of success display the popup for the result status */
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,CS_NOTIFYDURATION,SUCCESS_TONE);
	}
	else
	{
		/* incase of failure display the not done popup */
		DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE) ,IMG_GLOBAL_UNFINISHED, 1,CS_NOTIFYDURATION,WARNING_TONE);
	}
	/* delete screens for maitaining hte history */
	DeleteUptoScrID(SCR_ID_CALLSET_CUG_MAIN);
	return;
}
/**************************************************************

	FUNCTION NAME		: GoBackToCallSetup(void)

  	PURPOSE				: function to take back to call setup main screen 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBackToCallSetup(void)
{
	if(IsScreenPresent(SCR_ID_CALLSET_MAIN))
		GoBackToHistory( SCR_ID_CALLSET_MAIN);
	else
		GoBackHistory();
	return;
}
/**************************************************************

	FUNCTION NAME		: IsValidateIndex(U8)

  	PURPOSE				: check the validity of the iput index

	INPUT PARAMETERS	: index zero based

	OUTPUT PARAMETERS	: U8

	RETURNS				: void

 

**************************************************************/
U8 IsValidateIndex(U8 index)
{
	U16 count;
	/* check the index is within range or not */
	if ((index <= 9))
	{
		/* check for the duplicacy */
		for( count = 0; count < g_cug_context.NumOfList; count ++)
			if( index == g_cug_context.GroupList[count].index)
			return 0;
			return 1;
	}
	else
	{
		return 0;
	}
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCUGAdd(void)

  	PURPOSE				: Highlight grouplist Add menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCUGAdd(void)
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	/* Register function for left/right soft key */
	SetLeftSoftkeyFunction( EntryCALLSETCUGAddList, KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntryCALLSETCUGAddList, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETCUGAddList(void)

  	PURPOSE				: Showing grouplist Add Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETCUGAddList(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	EntryNewScreen(SCR_ID_CALLSET_CUG_ADD_LIST, NULL, EntryCALLSETCUGAddList, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_CUG_ADD_LIST);	
	if(guiBuffer == NULL)
	memset( g_cug_context.GroupItemInput.name , 0, sizeof(g_cug_context.GroupItemInput.name ));
	/* Display Category Screen */
	ShowCategory5Screen( STR_GLOBAL_ADD, MAIN_MENU_TITLE_SETTINGS_ICON, 
						STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, (PU8)g_cug_context.GroupItemInput.name, (MAX_CUG_NAME_LENGTH)+1, guiBuffer);
	/* Register function with left/right softkey */
#if defined(__MMI_GB__)                                                          //add by yaosq 20061122	
	GBSetInputboxLSKFunction(EntryCALLSETCUGEditOptions); 
#endif
	SetLeftSoftkeyFunction(EntryCALLSETCUGEditOptions, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETCUGAddListIndex(void)

  	PURPOSE				: Showing grouplist Add index Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETCUGAddListIndex(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	EntryNewScreen(SCR_ID_CALLSET_CUG_ADD_LIST_INDEX, NULL, EntryCALLSETCUGAddListIndex, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_CUG_ADD_LIST_INDEX);	
	if(guiBuffer == NULL)
		memset( g_cug_context.InputIndexBuf, 0 , sizeof(g_cug_context.InputIndexBuf));
	/* Display Category Screen */
	ShowCategory5Screen( STR_GROUPLIST_ADD_INDEX, MAIN_MENU_TITLE_SETTINGS_ICON, 
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						INPUT_TYPE_NUMERIC, (PU8)g_cug_context.InputIndexBuf, (1 * ENCODING_LENGTH), guiBuffer);
	/* Register function with left/right softkey */
	SetCategory5RightSoftkeyFunction(GoBackTwoScreens,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(AddEntryToNVRAM, KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: AddEntryToNVRAM(void)

  	PURPOSE				: add entry to nv ram

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AddEntryToNVRAM(void )
{
	S16 error;
	U8 index;
	U8 tempBuf[5];
	UnicodeToAnsii((S8*)tempBuf, (S8*)g_cug_context.InputIndexBuf);
	/* check the input buffer is empty or not */
	if ( strlen((S8 *)tempBuf) == 0)
	{
		/* if empty then display invalid index and terminate the add process */
		DisplayPopup((PU8)GetString(STR_GROUPLIST_INVALID_INDEX) ,IMG_GLOBAL_WARNING, 1,CS_NOTIFYDURATION,WARNING_TONE);
		DeleteUptoScrID(SCR_ID_CALLSET_CUG_MAIN);
		return;
	}
	/* if there is an not empty input */
	index = atoi( (S8*)tempBuf);
	/* validate the index */
	if (IsValidateIndex( index))
	{
		/* if the index is valid then add element to the nvram*/
		g_cug_context.GroupItemInput.index = index;
		if( WriteRecord (NVRAM_EF_GROUPLIST_LID,(U8)(g_cug_context.NumOfList+1), &g_cug_context.GroupItemInput, NVRAM_MAX_GROUP_NAME_SIZE,
			& error) < NVRAM_MAX_GROUP_NAME_SIZE){
			PRINT_INFORMATION (("## ERROR: write grouplist at index %d: WriteRecord "
									"to NVRAM failed (ecode%d)\n", index,(int) error));
			return;
			}
		memcpy(( S8 * )&g_cug_context.GroupList[g_cug_context.NumOfList], ( S8 * ) &g_cug_context.GroupItemInput, sizeof(CUGItemStruct));
		/* update the count for entries in list */	
		g_cug_context.NumOfList++;
		/* delete screens for maitaining the history */
		DeleteUptoScrID(SCR_ID_CALLSET_CUG_MAIN);
		GoBackHistory();
	}
	else
	{
		/* incase of invalid index then display error popup */
		DisplayPopup((PU8)GetString(STR_GROUPLIST_INVALID_INDEX) ,IMG_GLOBAL_WARNING, 1,CS_NOTIFYDURATION,WARNING_TONE);
		DeleteUptoScrID(SCR_ID_CALLSET_CUG_MAIN);
		return;
	}
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCUGEdit(void)

  	PURPOSE				: Highlight grouplist Edit menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCUGEdit(void)
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	/* Register function for left/right soft key */
	SetLeftSoftkeyFunction( EntryCALLSETCUGEditList, KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntryCALLSETCUGEditList, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;	
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETCUGEditList(void)

  	PURPOSE				: Showing grouplist Edit Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETCUGEditList(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	EntryNewScreen(SCR_ID_CALLSET_CUG_EDIT_LIST, NULL, EntryCALLSETCUGEditList, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_CUG_EDIT_LIST);	
	if( guiBuffer == NULL)
		memcpy( g_cug_context.GroupItemInput.name , g_cug_context.GroupList[g_cug_context.SelectIndex].name, sizeof(g_cug_context.GroupItemInput.name ) );
	/* Display Category Screen */
	ShowCategory5Screen( STR_GLOBAL_EDIT, MAIN_MENU_TITLE_SETTINGS_ICON, 
						STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, (PU8)(g_cug_context.GroupItemInput.name), (MAX_CUG_NAME_LENGTH )+1, guiBuffer);
	/* Register function with left/right softkey */
	SetLeftSoftkeyFunction(EntryCALLSETCUGEditOptions, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETCUGEditListIndex(void)

  	PURPOSE				: Showing grouplist edit index Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETCUGEditListIndex(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	EntryNewScreen(SCR_ID_CALLSET_CUG_EDIT_LIST_INDEX, NULL, EntryCALLSETCUGEditListIndex, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_CUG_EDIT_LIST_INDEX);	
	if( guiBuffer == NULL)
	{
		memset( g_cug_context.InputIndexBuf, 0 , sizeof(g_cug_context.InputIndexBuf));
		g_cug_context.InputIndexBuf[0] = (U8)('0' + g_cug_context.GroupList[g_cug_context.SelectIndex].index);
	}
	/* Display Category Screen */
	ShowCategory5Screen( STR_GROUPLIST_EDIT_INDEX, MAIN_MENU_TITLE_SETTINGS_ICON, 
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						INPUT_TYPE_NUMERIC, (PU8)g_cug_context.InputIndexBuf, (1 * ENCODING_LENGTH), guiBuffer);
	/* Register function with left/right softkey */
	SetCategory5RightSoftkeyFunction(GoBackTwoScreens,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EditEntryNVRAM, KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: EditEntryNVRAM(void)

  	PURPOSE				: edit entry to nv ram

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EditEntryNVRAM(void )
{
	S16 error;
	U8 index;
	/* check the index from input buffer */
	index = g_cug_context.InputIndexBuf[0] - '0';
	/* validate the index */
	if (IsValidateIndex( index))
	{
		/* if the index is valid then edit element in the nvram*/
		g_cug_context.GroupList[g_cug_context.SelectIndex].index = index;
//micha1229
		if( WriteRecord (NVRAM_EF_GROUPLIST_LID,(U8)(g_cug_context.SelectIndex+1), (U32*)&g_cug_context.GroupItemInput, NVRAM_MAX_GROUP_NAME_SIZE,
		& error) < NVRAM_MAX_GROUP_NAME_SIZE){

		PRINT_INFORMATION (("## ERROR: write grouplist at index %d: WriteRecord "
								"to NVRAM failed (ecode%d)\n", g_cug_context.SelectIndex,(int) error));
		return;
		}
	else
	{
		pfnUnicodeStrcpy(( S8 * )g_cug_context.GroupList[g_cug_context.SelectIndex].name, ( S8 * ) g_cug_context.GroupItemInput.name);
		/* delete screens for maitaining the history */
		DeleteUptoScrID(SCR_ID_CALLSET_CUG_MAIN);
		GoBackHistory();
		return;
	}
}
	else
	{
		if ( g_cug_context.GroupList[g_cug_context.SelectIndex].index == index)
		{
			/* if the index is valid then edit element in the nvram*/
			g_cug_context.GroupList[g_cug_context.SelectIndex].index = index;
			if( WriteRecord (NVRAM_EF_GROUPLIST_LID,(U8)(g_cug_context.SelectIndex+1), (U32*)&g_cug_context.GroupItemInput, NVRAM_MAX_GROUP_NAME_SIZE,
				& error) < NVRAM_MAX_GROUP_NAME_SIZE){

				PRINT_INFORMATION (("## ERROR: write grouplist at index %d: WriteRecord "
										"to NVRAM failed (ecode%d)\n", g_cug_context.SelectIndex,(int) error));
				return;
				}
			else
			{
				pfnUnicodeStrcpy(( S8 * )g_cug_context.GroupList[g_cug_context.SelectIndex].name, ( S8 * ) g_cug_context.GroupItemInput.name);
				/* delete screens for maitaining the history */
				DeleteUptoScrID(SCR_ID_CALLSET_CUG_MAIN);
				GoBackHistory();
				return;
			}
		}
		/* incase of invalid index then display error popup */
		DisplayPopup((PU8)GetString(STR_GROUPLIST_INVALID_INDEX) ,IMG_GLOBAL_WARNING, 1,CS_NOTIFYDURATION,WARNING_TONE);
		/* delete screens for maitaining the history */
		DeleteUptoScrID(SCR_ID_CALLSET_CUG_MAIN);
		return;
	}
//micha1229
//	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCUGDelete(void)

  	PURPOSE				: Highlight grouplist delete menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCUGDelete(void)
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	/* Register function for left/right soft key */
	SetLeftSoftkeyFunction( EntryCALLSETCUGDelListCfrm, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}

/**************************************************************

	FUNCTION NAME		: DeactivateAndDelete(void)

  	PURPOSE				: deactivate the group list item and then delete.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeactivateAndDelete( void )
{
	/* set delete flag */
	g_cug_context.DeactiveThenDelete = 1;
	/* deactivate before deleting the entry */
	EntryGroupListDeactivate();
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETCUGDelListCfrm(void)

  	PURPOSE				: entry function for groupname delete confirmation

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETCUGDelListCfrm( void )
{
	DisplayConfirm(
		STR_GLOBAL_YES,IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO, 
		get_string(STR_GLOBAL_DELETE), IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* Register function with left/right softkey */
	SetLeftSoftkeyFunction(DeactivateAndDelete,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
	return;
}
/**************************************************************

	FUNCTION NAME		: GroupListDelete(void)

  	PURPOSE				: grouplist delete

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GroupListDelete(void)
{
	S16 error;
	U32 count;
	memset( &g_cug_context.GroupItemInput, (U8)-1, sizeof(g_cug_context.GroupItemInput ) );
	/* delete element to the nvram */
	if( WriteRecord (NVRAM_EF_GROUPLIST_LID,(U8)(g_cug_context.SelectIndex+1), (U32*)&g_cug_context.GroupItemInput, NVRAM_MAX_GROUP_NAME_SIZE,
		& error) < NVRAM_MAX_GROUP_NAME_SIZE){

		PRINT_INFORMATION (("## ERROR: write grouplist at index %d: WriteRecord "
								"to NVRAM failed (ecode%d)\n", g_cug_context.SelectIndex,(int) error));
		return;
		}
	else
	{
		/* if nvram is updated properly then update the list for display */
		for( count = g_cug_context.SelectIndex; count < g_cug_context.NumOfList; count++)
			memcpy( &g_cug_context.GroupList[count], &g_cug_context.GroupList[count+1], sizeof(g_cug_context.GroupList[g_cug_context.SelectIndex]));
		/* update the count */
		g_cug_context.NumOfList--;
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,CS_NOTIFYDURATION,SUCCESS_TONE);
		DeleteUptoScrID(SCR_ID_CALLSET_CUG_MAIN);
		return;
	}
//micha1229
//	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETCUGEditOptions(void)

  	PURPOSE				: entry function for edit group name options screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETCUGEditOptions( void )
{
	U8*			guiBuffer;       /* Buffer holding history data */
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 numItems;

	EntryNewScreen(SCR_ID_CALLSET_CUG_EDIT_OPTIONS, NULL, EntryCALLSETCUGEditOptions, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer (SCR_ID_CALLSET_CUG_EDIT_OPTIONS);	
	/* Set current parent id*/
	SetParentHandler (MENU_CUG_INPUT_METHOD_AND_DONE_OPTION_ID);
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	/* Retrieve no of child of menu item to be displayed */
	numItems = GetNumOfChild (MENU_CUG_INPUT_METHOD_AND_DONE_OPTION_ID);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_CUG_INPUT_METHOD_AND_DONE_OPTION_ID, nStrItemList);
	/* Display Category15 Screen */
	ShowCategory15Screen (STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_SETTINGS_ICON, 
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (U16 *)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer );
	/* Register function with left/right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
	return;
}
/**********************************************************************

	FUNCTION NAME		: HighlightEditOptionsDone(void)

  	PURPOSE				: Highlight grouplist edit Options done

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**********************************************************************/
void HighlightEditOptionsDone(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	if ( IsScreenPresent( SCR_ID_CALLSET_CUG_ADD_LIST ) )
		/*3 Register function for left soft key */
		SetLeftSoftkeyFunction(EntryCALLSETCUGAddListIndex,KEY_EVENT_UP);
	if ( IsScreenPresent( SCR_ID_CALLSET_CUG_EDIT_LIST ) )
		/*3 Register function for left soft key */
		SetLeftSoftkeyFunction(EntryCALLSETCUGEditListIndex,KEY_EVENT_UP);
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}
/**********************************************************************

	FUNCTION NAME		: HighlightEditOptionsInputMethod(void)

  	PURPOSE				: Highlight grouplist edit Options inputMethod

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**********************************************************************/
void HighlightEditOptionsInputMethod(void)
{
#if defined(__MMI_GB__)// zrx del 20060612         //open this define by yaosq 20061122
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	RegisterInputMethodScreenCloseFunction( GoBackTwoScreens);
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryInputMethodScreen,KEY_EVENT_UP);
	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
	#endif
}
/**************************************************************

	FUNCTION NAME		: GoBackTwoScreens(void)

  	PURPOSE				: go back history by two screens

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBackTwoScreens(void)
{
	GoBacknHistory( 1 );
	return;
}


#endif	//__MMI_REMOVE_CUG__


