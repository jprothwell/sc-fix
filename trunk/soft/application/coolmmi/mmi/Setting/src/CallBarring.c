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
 *	CallBarring.c
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
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: CallBarring.c

  	PURPOSE		: Call Barring Application - call setup

 

 

	DATE		: 11/14/2003

**************************************************************/
#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "csp.h"
#include "mainmenudef.h"
#include "protocolevents.h"
#include "phonebooktypes.h"
#include "callsetup.h"
#include "callbarrng.h"
#include "callsetupenum.h"
#include "commonscreens.h"
#include "callmanagementiddef.h"
#include "callmanagementgprot.h"
#include "messagesexdcl.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "settingdefs.h"
#include "callsdefs.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim_call.h"
#endif
#undef __NEWSIMULATOR



/**************************************************************

	FUNCTION NAME		: InitCallBarring(void)

  	PURPOSE				: Initializes the call barring application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void InitCallBarring(void)
{
	/* set hilite handlers for the individual menu items */
	SetHiliteHandler(MENU8237_SCR8093_MNGCALL_MENU_BARRING,HighlightCALLSETCallBarring);
	SetHiliteHandler(MENU_CALL_BARRING_OUTGOING,HighlightCALLSETCallBarringOutgoing);
	SetHiliteHandler(MENU_CALL_BARRING_INCOMING,HighlightCALLSETCallBarringIncoming);
	SetHiliteHandler(MENU_CALL_BARRING_CANCELALL,HighlightCALLSETCallBarringCancelAll);
	SetHiliteHandler(MENU_CALL_BARRING_CHANGEPASS,HighlightCALLSETCallBarringChangePwd);
	SetHiliteHandler(MENU_BARRING_OUTGOING_ALLCALLS,HighlightCALLSETCallBarringAllOutCalls);
	SetHiliteHandler(MENU_BARRING_OUTGOING_INTERCALL,HighlightCALLSETCallBarringAllOutInter);
	SetHiliteHandler(MENU_BARRING_OUTGOING_INTEREXCEPTHOME,HighlightCALLSETCallBarringAllOutInterExcHome);
	SetHiliteHandler(MENU_BARRING_INCOMING_ALLCALLS,HighlightCALLSETCallBarringAllInCalls);
	SetHiliteHandler(MENU_BARRING_INCOMING_ROAMING,HighlightCALLSETCallBarringAllInRoaming);
	SetHiliteHandler(MENU_BARRING_ACTIVATE,HighlightCALLSETCallBarringActive);
	SetHiliteHandler(MENU_BARRING_DEACTIVATE,HighlightCALLSETCallBarringDeactive);
	SetHiliteHandler(MENU_BARRING_QUERY,HighlightCALLSETCallBarringQuery);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarring(void)

  	PURPOSE				: highlite handler for call barring menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarring(void)
{
	/* Change left/right soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringMain,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETBarringMain, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringOutgoing(void)

  	PURPOSE				: highlite handler for the call barring outgoing menu option

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringOutgoing(void)
{
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringOut,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETBarringOut, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringIncoming(void)

  	PURPOSE				: highlight handler for call barring in case of incoming.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void HighlightCALLSETCallBarringIncoming(void)
{
	/* Change right soft key icon and label */
	SetLeftSoftkeyFunction (EntryCALLSETBarringIn,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETBarringIn, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringCancelAll(void)

  	PURPOSE				: highlight handler for call barring cancel all application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringCancelAll(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSBarAction = BARRING_CANCEL_ALL;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringPass,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntryCALLSETBarringPass, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringChangePwd(void)

  	PURPOSE				: highlight handler for changing call barring passwd

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringChangePwd(void )
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSBarAction = BARRING_CHANGE_PSW;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringOldPass,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntryCALLSETBarringOldPass, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);

	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringAllOutCalls(void)

  	PURPOSE				: highlight handler for call barring all outgoing calls.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringAllOutCalls(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSBarType = BARRING_OUTGOING_ALLCALLS ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringOption,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETBarringOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringAllOutInter(void)

  	PURPOSE				: high light handler for call barring outgoing international calls.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringAllOutInter(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSBarType = BARRING_OUTGOING_INTERCALL ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringOption,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETBarringOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringAllOutInterExcHome(void)

  	PURPOSE				: highlight handler for call barring outgoing international
							calls except home.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringAllOutInterExcHome(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSBarType =  BARRING_OUTGOING_INTEREXCEPTHOME ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringOption,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETBarringOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringAllInCalls(void)

  	PURPOSE				: highlight handler for call barring all international calls.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringAllInCalls(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSBarType =  BARRING_INCOMING_ALLCALLS ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringOption,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETBarringOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringAllInRoaming(void)

  	PURPOSE				: highlight handler for call barring incoming on roaming calls.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringAllInRoaming(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSBarType =  BARRING_INCOMING_ROAMING ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringOption,KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETBarringOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringActive(void)

  	PURPOSE				: hightlight handler for call barring activate

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringActive(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSBarAction = BARRING_ON ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringPass,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntryCALLSETBarringPass, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringDeactive(void)

  	PURPOSE				: hightlight handler for call barring deactivate

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringDeactive(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSBarAction = BARRING_OFF ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (EntryCALLSETBarringPass,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntryCALLSETBarringPass, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallBarringQuery(void)

  	PURPOSE				: hightlight handler for call barring query status

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallBarringQuery(void)
{
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction (SendBarrQuery,KEY_EVENT_UP);
//micha0529
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCALLSETBarringMain(void)

  	PURPOSE				: entry function for call barring main menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETBarringMain(void)
{
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */
	U16 nNumofItem;
	U16 nStrItemList[MAX_SUB_MENUS];
	/*U32 maskingByte=0xFF;
	U32 menuItemId=0xFFFFFFFF;*/

	EntryNewScreen(SCR_ID_CALLSET_BARRING_MAIN, NULL, EntryCALLSETBarringMain, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_BARRING_MAIN);	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU8237_SCR8093_MNGCALL_MENU_BARRING);  
	/* Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU8237_SCR8093_MNGCALL_MENU_BARRING);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU8237_SCR8093_MNGCALL_MENU_BARRING, nStrItemList);  
	/* Set current parent id*/
	SetParentHandler(MENU8237_SCR8093_MNGCALL_MENU_BARRING);
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

 
#if 0
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
#endif /* 0 */

	/* Display Screen */
	ShowCategory15Screen(STR_MENU_CALL_BARRING, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,nNumofItem, nStrItemList,
						  (U16 *)gIndexIconsImageList,1,0,guiBuffer);
	/* Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	/* Register Exit handler */
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETBarringOut(void)

  	PURPOSE				: entry function for out going call barring menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETBarringOut(void)
{
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */
	U16 nNumofItem;
	U16 nStrItemList[MAX_SUB_MENUS];
	/*U32 maskingByte=0xFFFFFFFF;
	U32 menuItemId=0xFFFFFFFF;*/

	EntryNewScreen(SCR_ID_CALLSET_BARRING_OUT, NULL, EntryCALLSETBarringOut, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_BARRING_OUT);	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU_CALL_BARRING_OUTGOING);  
	/* Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU_CALL_BARRING_OUTGOING);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU_CALL_BARRING_OUTGOING, nStrItemList);  
	/* Set current parent id*/
	SetParentHandler(MENU_CALL_BARRING_OUTGOING);
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

 
#if 0
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
#endif /* 0 */

	/* Display Category Screen */
	ShowCategory15Screen( STR_BARRING_OUTGOING_CALL_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK , IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,nNumofItem,nStrItemList,
						  (U16 *)gIndexIconsImageList,1,0,guiBuffer);
	/* Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* Register Exit handler */
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETBarringIn(void)

  	PURPOSE				: entry function for incoimg call barring menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryCALLSETBarringIn(void)
{
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */
	U16 nNumofItem;
	U16 nStrItemList[MAX_SUB_MENUS];
	/*U32 maskingByte=0xFFFFFFFF;
	U32 menuItemId=0xFFFFFFFF;*/

	EntryNewScreen(SCR_ID_CALLSET_BARRING_IN, NULL, EntryCALLSETBarringIn, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_BARRING_IN);	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU_CALL_BARRING_INCOMING);  
	/* Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU8237_SCR8093_MNGCALL_MENU_BARRING);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU_CALL_BARRING_INCOMING, nStrItemList);  
	/* Set current parent id*/
	SetParentHandler(MENU_CALL_BARRING_INCOMING);
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

 
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */

	/* Display Category Screen */
	ShowCategory15Screen(STR_MENU_BARRING_INCOMING_MAIN, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK , IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,nNumofItem, nStrItemList,
						  (U16 *)gIndexIconsImageList,1,0,guiBuffer);
	/* Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* Register Exit handler */
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETBarringOption(void)

  	PURPOSE				: entry function for call barring option menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETBarringOption(void)
{
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */
	U16 nNumofItem;
	U16 nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_CALLSET_BARRING_OPTION, NULL, EntryCALLSETBarringOption, NULL);
	
	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_BARRING_OPTION);	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_BARRING_OPTION);
	/* Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU_BARRING_OPTION);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_BARRING_OPTION, nStrItemList);
	/* Set current parent id*/
	SetParentHandler(MENU_BARRING_OPTION	);
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	/* Display Category Screen */
	ShowCategory15Screen(	STR_MENU_BARRING_OPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,nNumofItem, nStrItemList,
						  (U16 *)gIndexIconsImageList,1,0,guiBuffer);
	/* Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	/* Register Exit handler */
	return;
}
/**************************************************************

	FUNCTION NAME		: ScrBarringPasswordaValidation(void)

  	PURPOSE				: validating the call barring passwords

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ScrBarringPasswordaValidation(U8* text,U8* cursor,S32 length)
{

	if((length) < BARR_SIM_PASS_LEN)
		ChangeLeftSoftkey(0,0);
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		/* check the screen from which validation came */
		switch (g_callset_cntx_p->SSBarPassStep)
		{
		/* if the call barring options is selected */
		case 0 :
			{
				SetLeftSoftkeyFunction(CallBarrOption,KEY_EVENT_UP);
				break;
			}
		/* if the flow is from call barring new passwords screen */
		case 1:
			{
				SetLeftSoftkeyFunction(EntryCALLSETBarringNewPass,KEY_EVENT_UP);
				break;
			}
		/* if the flow is from confirmation screen of call barring password screen */
		case 2:
			{
				SetLeftSoftkeyFunction(EntryCALLSETBarringCfrmPass,KEY_EVENT_UP);
				break;
			}
		/* if the call barring options is selected */
		case 3:
			{
				SetLeftSoftkeyFunction(CallBarrOption,KEY_EVENT_UP);
				break;
			}
		}
	}
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETBarringPass(void)

  	PURPOSE				: entry function for call barring passwd

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETBarringPass(void )
{
	U8* guiBuffer;
//	U8* inputBuffer;

	EntryNewScreen(SCR_ID_CALLSET_BARRING_PASS, NULL, EntryCALLSETBarringPass, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_BARRING_PASS); 
//	inputBuffer = GetCurrInputBuffer(SCR_ID_CALLSET_BARRING_PASS);

	if(guiBuffer==NULL) 
	memset( g_callset_cntx_p->SSBarPassOld, 0, sizeof(g_callset_cntx_p->SSBarPassOld));
//	else
//		pfnUnicodeStrcpy((S8*)g_callset_cntx_p->SSBarPassOld, ( S8*)inputBuffer);

	/* Register input validation function*/
	RegisterInputBoxValidationFunction(ScrBarringPasswordaValidation);
	/* Display Category Screen */
	ShowCategory111Screen(STR_GLOBAL_OK, IMG_GLOBAL_OK,
							   STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							   STR_BARRING_PASSWORD_CAP,INPUT_TYPE_NUMERIC_PASSWORD, 
							   (PU8)g_callset_cntx_p->SSBarPassOld, BARR_SIM_PASS_LEN+1, NULL, guiBuffer);
	/* Register function with left/right softkey */
	SetLeftSoftkeyFunction(CallBarrOption,KEY_EVENT_UP);
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	g_callset_cntx_p->SSBarPassStep = 0;
	/* Register Exit handler */
	return;
}
/**************************************************************

	FUNCTION NAME		: CallBarrOption(void)

  	PURPOSE				: call barring options processing function

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CallBarrOption(void)
{
	/* menu option selected is call barring activate */
	if(	g_callset_cntx_p->SSBarAction == BARRING_ON)
		SendCallBarrActivate();
	/* menu option selected is call barring deactivate */
	else if (g_callset_cntx_p->SSBarAction == BARRING_OFF)
		SendCallBarrDeactivate();
	/* menu option selected is call barring query status */
	else if(g_callset_cntx_p->SSBarAction == BARRING_QUERY)
		SendBarrQuery();
	/* menu option selected is call barring cancel all */
	else if(g_callset_cntx_p->SSBarAction == BARRING_CANCEL_ALL)
		SendCancelAllReq();
	/* menu option selected is call barring password change req */
	else if(g_callset_cntx_p->SSBarAction == BARRING_CHANGE_PSW)
		SendChangePswdReq();
	return;
}
/**************************************************************

	FUNCTION NAME		: SendCallBarrActivate(void)

  	PURPOSE				: function to send req for call barring activate 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SendCallBarrActivate(void)
{

	S8 input_string[(MAX_DIGITS_USSD * ENCODING_LENGTH)];
	PS8 hash_string = NULL ;
	S8 string_out[20];
	memset(string_out,0,20);
	 
	memset(input_string,0, (MAX_DIGITS_USSD * ENCODING_LENGTH));
	hash_string = (PS8) GetString(STR_HASH_VALUE);
	switch(g_callset_cntx_p->SSBarType)
	{
	/* incase of call barring all outgoing calls */
	case BARRING_OUTGOING_ALLCALLS:
		strcpy((S8*) string_out, "*33*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* incase of call barring all outgoing international calls */
	case BARRING_OUTGOING_INTERCALL :
		strcpy((S8*) string_out, "*331*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* incase of call barring all outgoing international calls expect home */
	case BARRING_OUTGOING_INTEREXCEPTHOME :
		strcpy((S8*) string_out, "*332*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* incase of call barring all incoming calls */
	case BARRING_INCOMING_ALLCALLS :
		strcpy((S8*) string_out, "*35*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* incase of call barring all incoming roaming calls */
	case BARRING_INCOMING_ROAMING :
		strcpy((S8*) string_out, "*351*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	}
	pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSBarPassOld);
	pfnUnicodeStrcat((PS8) input_string,(PS8) hash_string);
	/* set protocol event handler for the call barring response */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETBarringRsp, PRT_MMI_SS_CALL_BARRING_RSP);
#else
	SetProtocolEventHandler(CALLSETBarringRsp, PRT_MMI_SS_CALL_BARRING_RSP);
#endif

	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif

	/* make a corresponding ss string for call barring option */
	MakeCall((PS8) input_string);
	return;	
}

/**************************************************************

	FUNCTION NAME		: SendCallBarrDeactivate(void)

  	PURPOSE				: function to send req for call barring deactivate 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void SendCallBarrDeactivate(void)
{
	S8 input_string[(MAX_DIGITS_USSD * ENCODING_LENGTH)];
	PS8 hash_string = NULL ;
	S8 string_out[20];
	memset(string_out,0,20);
	 
	memset(input_string,0, (MAX_DIGITS_USSD * ENCODING_LENGTH));
	hash_string = (PS8) GetString(STR_HASH_VALUE);
	switch(g_callset_cntx_p->SSBarType)
	{
	/* incase of call barring all outgoing calls */
	case BARRING_OUTGOING_ALLCALLS:
		strcpy((S8*) string_out, "#33*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* incase of call barring all outgoing international calls */
	case BARRING_OUTGOING_INTERCALL :
		strcpy((S8*) string_out, "#331*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* incase of call barring all outgoing international calls expect home */
	case BARRING_OUTGOING_INTEREXCEPTHOME :
		strcpy((S8*) string_out, "#332*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* incase of call barring all incoming calls */
	case BARRING_INCOMING_ALLCALLS :
		strcpy((S8*) string_out, "#35*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* incase of call barring all incoming roaming calls */
	case BARRING_INCOMING_ROAMING :
		strcpy((S8*) string_out, "#351*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	}
	pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSBarPassOld);
	pfnUnicodeStrcat((PS8) input_string,(PS8) hash_string);
	/* set protocol event handler for the call barring response */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETBarringRsp, PRT_MMI_SS_CALL_BARRING_RSP);
#else
	SetProtocolEventHandler(CALLSETBarringRsp, PRT_MMI_SS_CALL_BARRING_RSP);
#endif

	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif
	
	/* make a corresponding ss string for call barring option */
	MakeCall((PS8) input_string);
	return;	
}
/**************************************************************

	FUNCTION NAME		: SendBarrQuery(void)

  	PURPOSE				: function to send req for call barring query status.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SendBarrQuery(void)
{
	S8 input_string[(MAX_DIGITS_USSD * ENCODING_LENGTH)];
	S8 string_type[30];
	 
	memset(input_string,0,( MAX_DIGITS_USSD * ENCODING_LENGTH));
	memset(string_type,0,30);
	switch(g_callset_cntx_p->SSBarType)
	{
	/* incase of call barring all outgoing calls */
	case BARRING_OUTGOING_ALLCALLS:
		strcpy((S8*) string_type, "*#33#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_type);		
		break;
	/* incase of call barring all outgoing international calls */
	case BARRING_OUTGOING_INTERCALL :
		strcpy((S8*) string_type, "*#331#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_type);		
		break;
	/* incase of call barring all outgoing international calls expect home */
	case BARRING_OUTGOING_INTEREXCEPTHOME :
		strcpy((S8*)string_type, "*#332#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_type);		
		break;
	/* incase of call barring all incoming calls */
	case BARRING_INCOMING_ALLCALLS :
		strcpy((S8*) string_type, "*#35#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_type);		
		break;
	/* incase of call barring all incoming roaming calls */
	case BARRING_INCOMING_ROAMING :
		strcpy((S8*) string_type, "*#351#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_type);		
		break;
	}
	/* set protocol event handler for the call barring response */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETBarringRsp, PRT_MMI_SS_CALL_BARRING_RSP);
#else
	SetProtocolEventHandler(CALLSETBarringRsp, PRT_MMI_SS_CALL_BARRING_RSP);
#endif

	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif

	/* make a corresponding ss string for call barring option */
	MakeCall((PS8) input_string);
	return;
}
/**************************************************************

	FUNCTION NAME		: CALLSETBarringRsp(void)

  	PURPOSE				: function for handling response for call barring query req.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void  aud_speech_set_mode(kal_bool on);
void CALLSETBarringRsp(void * info )
{

	mmi_ss_call_barring_rsp_struct *pMsgBarringRsp = 
		(mmi_ss_call_barring_rsp_struct *) info ;
	U8 bs_index;

	memset(g_callset_cntx_p->SSDisplayBuffer,0, MAX_DISP_UCS2);

	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_OUT);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_IN);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_OLD_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_OPTION);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_NEW_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_CFRM_PASS);
	
	/* check the status of the response result */
	if(pMsgBarringRsp->result.flag == L4C_OK) {
		/* if the response is success check the type */
		switch ( pMsgBarringRsp->type) {
			/* call barring all outgoing calls */
			case L4_BAOC:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_BAOC_A));
				break;
			/* call barring all outgoing international calls */
			case L4_BOIC:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_BOIC_A));
				break;
			/* call barring all outgoing international calls except home */
			case L4_BOICEXHC:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_BOICEXHC_A));
				break;
			/* call barring all incoming calls */
			case L4_BAIC:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_BAIC_A));
				break;
			/* call barring all incoming roaming calls */
			case L4_BAICROAM:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_BAICROAM_A));
				break;
			/* call barring cancel all calls */
			case L4_BAC:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_BAC_A));
				break;
			/* call barring all outgoing calls */
			case L4_BOC:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_BOC_A));
				break;
			/* call barring all incoming calls */
			case L4_BIC:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_BIC_A));
				break;
			default:
				pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_DEFAULT_A));
				break;
			}
			/* format the result string with active/deactive tags. */
			if (pMsgBarringRsp->count == 0 ) {
				pfnUnicodeStrcat((PS8) g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_DEACTIVATED));
			}
			for ( bs_index = 0; bs_index < pMsgBarringRsp->count; bs_index++)
			{
				if (( pfnUnicodeStrlen((PS8)GetString(STR_L4_ALL_TELESERVICES_EXCEPT_SMS))
						+ pfnUnicodeStrlen((PS8)GetString(STR_DEACTIVATED))
						+ pfnUnicodeStrlen((PS8)g_callset_cntx_p->SSDisplayBuffer)) < MAX_DISP_UCS2 / ENCODING_LENGTH) 
				{
					if (pMsgBarringRsp->list[bs_index].ss_status & SS_ABIT) 
			{
				pfnUnicodeStrcat((PS8) g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_ACTIVATED));
			}
			else
			{
				pfnUnicodeStrcat((PS8) g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_DEACTIVATED));
			}
					AppendBScodeString( pMsgBarringRsp->list[bs_index].bs_code, (PS8)g_callset_cntx_p->SSDisplayBuffer);
				}
			}
		/* call the result screen function to display the result string */
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);			
		EntryCALLSETGeneralSSResut();
	}
	else {
		/* in case of response failure */
#ifdef  __MMI_SS_SHOW_CAUSE__
		ShowCauseString( pMsgBarringRsp->result.cause, (PS8)g_callset_cntx_p->SSDisplayBuffer);
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
#else
		/* display the not done popup */
//micha0616
		if ( pMsgBarringRsp->result.cause == CM_SS_ERR_NEGATIVEPW_CHECK ) {
			DisplayPopup( (PU8) GetString( STR_SETTING_WRONG), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
		}
		else if (pMsgBarringRsp->result.cause == CM_SS_ERR_NUMBEROFPW_ATTEMPTSVIOLATION) {
			DisplayPopup( (PU8) GetString( STR_SETTING_BLOCK), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
		}
		else {
			DisplayPopup( (PU8) GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
		}

//micha0915
		RemoveSSReqScr();
#endif
	}
	aud_speech_set_mode(KAL_FALSE); //chenhe,070808. for callforward,CM_KB_OUTGOINGCALL is been sent,so have to do so here
	return;
}
/**************************************************************

	FUNCTION NAME		: SendChangePswdReq(void )

  	PURPOSE				: function for sending req for cancel all (call barring) .

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SendCancelAllReq(void)
{
	U8 input_string[MAX_DIGITS_USSD];
	PS8 hash_string = NULL ;
	U8 string_type[20];
	/* format the string for the ss string handling */	
	memset(input_string,0, MAX_DIGITS_USSD);
	memset(string_type,0,20);
	hash_string= (PS8) GetString(STR_HASH_VALUE);
	strcpy((PS8) string_type,(PS8)"#330*");
	AnsiiToUnicodeString((PS8) input_string,(PS8)string_type);		
	pfnUnicodeStrcat((S8*) input_string,(S8*) g_callset_cntx_p->SSBarPassOld);
	pfnUnicodeStrcat((S8*) input_string,(S8*) hash_string);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);	
#endif

	/* make a corresponding ss string for call fwd option */
	MakeCall((PS8) input_string);
	return;
}
/**************************************************************

	FUNCTION NAME		: SendChangePswdReq(void )

  	PURPOSE				: function for sending req for call barring change passwd.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SendChangePswdReq(void)
{
	U8 input_string[MAX_DIGITS_USSD];
	U8 string_type[20];
	PS8 hash_string = NULL ;
	U8 uni_string[10];
	memset(uni_string,0,10);
	/* format the string for the ss string handling */	
	memset(input_string,0, MAX_DIGITS_USSD);
	memset(string_type,0,20);
	strcpy((PS8) string_type,(PS8)"*03**");
	hash_string = (PS8) GetString(STR_HASH_VALUE);
	AnsiiToUnicodeString((PS8) input_string,(PS8)string_type);		
	pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSBarPassOld);
	strcpy((PS8) string_type,(PS8) "*");
	AnsiiToUnicodeString((PS8) uni_string,(PS8)string_type);	
	pfnUnicodeStrcat((PS8) input_string,(PS8) uni_string);
	pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSBarPassNew);
	pfnUnicodeStrcat((PS8) input_string,(PS8) uni_string);
	pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSBarPassCfrm);
	pfnUnicodeStrcat((PS8) input_string,(PS8) hash_string);
	/* set protocol event handler for the call barring passwd change response */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETBarringPassRsp, PRT_MMI_SS_CHANGE_PASSWORD_RSP);
#else
	SetProtocolEventHandler(CALLSETBarringPassRsp, PRT_MMI_SS_CHANGE_PASSWORD_RSP);
#endif

	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif

	/* make a corresponding ss string for call barring passwd change response option */
	MakeCall((PS8) input_string);
	return;
}

/**************************************************************

	FUNCTION NAME		: CALLSETBarringPassRsp(void )

  	PURPOSE				: function for handling response for call barring change passwd req.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETBarringPassRsp(void *info)
{
	mmi_ss_call_barring_rsp_struct *pMsgBarringRsp = 
		(mmi_ss_call_barring_rsp_struct *) info ;
	/* change passwd response handling */
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_OUT);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_IN);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_OLD_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_OPTION);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_NEW_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_CFRM_PASS);
	
	if(pMsgBarringRsp->result.flag == 0)
	{
		pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_CHANGE_PSW_SUCCESS_TEXT));
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
	}else {

//micha0616
		if ( pMsgBarringRsp->result.cause == CM_SS_ERR_NEGATIVEPW_CHECK ) {
			DisplayPopup( (PU8) GetString( STR_SETTING_WRONG), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
		}
		else if (pMsgBarringRsp->result.cause == CM_SS_ERR_NUMBEROFPW_ATTEMPTSVIOLATION) {
			DisplayPopup( (PU8) GetString( STR_SETTING_BLOCK), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
		}
		else {
			DisplayPopup( (PU8) GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
		}

//micha0915
		RemoveSSReqScr();
	}
	aud_speech_set_mode(KAL_FALSE); //chenhe,070808. for callforward,CM_KB_OUTGOINGCALL is been sent,so have to do so here
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETBarringOldPass(void )

  	PURPOSE				: entry function for call barring old passwd screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETBarringOldPass(void)
{
	U8* guiBuffer;
//	U8 * inputBuffer;

	EntryNewScreen(SCR_ID_CALLSET_BARRING_OLD_PASS, NULL, EntryCALLSETBarringOldPass, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_BARRING_OLD_PASS); 
//	inputBuffer = GetCurrInputBuffer(SCR_ID_CALLSET_BARRING_OLD_PASS);
	if( guiBuffer == NULL)
		memset( g_callset_cntx_p->SSBarPassOld, 0, sizeof(g_callset_cntx_p->SSBarPassOld));
//	else
//		pfnUnicodeStrcpy((S8*)g_callset_cntx_p->SSBarPassOld, ( S8*)inputBuffer);
	/* Register input validation function*/
	RegisterInputBoxValidationFunction(ScrBarringPasswordaValidation);
	/* display the corresponding screen */
	ShowCategory111Screen(STR_GLOBAL_OK, IMG_GLOBAL_OK,
							   STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							   STR_OLD_BARRINGPASS_CAP, INPUT_TYPE_NUMERIC_PASSWORD, (PU8)g_callset_cntx_p->SSBarPassOld, 
							   BARR_SIM_PASS_LEN+1, NULL, guiBuffer);
	/* Set the left/right soft key funtions */
	SetLeftSoftkeyFunction(EntryCALLSETBarringNewPass,KEY_EVENT_UP);
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	g_callset_cntx_p->SSBarPassStep = 1;

}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETBarringNewPass(void )

  	PURPOSE				: entry function for call barring new passwd screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETBarringNewPass(void)
{

	U8* guiBuffer;
//	U8 * inputBuffer;

	EntryNewScreen(SCR_ID_CALLSET_BARRING_NEW_PASS, NULL, EntryCALLSETBarringNewPass, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_BARRING_NEW_PASS); 
//	inputBuffer = GetCurrInputBuffer(SCR_ID_CALLSET_BARRING_NEW_PASS);
	if( guiBuffer == NULL)
		memset( g_callset_cntx_p->SSBarPassNew, 0, sizeof(g_callset_cntx_p->SSBarPassNew));
//	else
//		pfnUnicodeStrcpy((S8*)g_callset_cntx_p->SSBarPassNew, ( S8*)inputBuffer);
	/* Register input validation function*/
	RegisterInputBoxValidationFunction(ScrBarringPasswordaValidation);
	/* display the corresponding screen */
	ShowCategory111Screen(STR_GLOBAL_OK, IMG_GLOBAL_OK,
							   STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  STR_NEW_BARRINGPASS_CAP, INPUT_TYPE_NUMERIC_PASSWORD, (PU8)g_callset_cntx_p->SSBarPassNew, 
							   BARR_SIM_PASS_LEN+1, NULL, guiBuffer);
	g_callset_cntx_p->SSBarPassStep = 2;
	/* Set the left/right soft key funtions */
	SetLeftSoftkeyFunction(EntryCALLSETBarringCfrmPass,KEY_EVENT_UP);
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCALLSETBarringCfrmPass(void )

  	PURPOSE				: entry function for call barring change passwd confirmation screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETBarringCfrmPass(void)
{
	U8* guiBuffer;
//	U8 * inputBuffer;

	EntryNewScreen(SCR_ID_CALLSET_BARRING_CFRM_PASS, NULL, EntryCALLSETBarringCfrmPass, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_BARRING_CFRM_PASS); 
//	inputBuffer = GetCurrInputBuffer(SCR_ID_CALLSET_BARRING_CFRM_PASS);
	if(guiBuffer==NULL) 
		memset( g_callset_cntx_p->SSBarPassCfrm, 0, sizeof(g_callset_cntx_p->SSBarPassCfrm));
//	else
//		pfnUnicodeStrcpy((S8*)g_callset_cntx_p->SSBarPassCfrm, ( S8*)inputBuffer);
	/* Register input validation function*/
	RegisterInputBoxValidationFunction(ScrBarringPasswordaValidation);
	/* display the corresponding screen */
	ShowCategory111Screen(STR_GLOBAL_OK, IMG_GLOBAL_OK,
							   STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							   STR_CONFIRM_BARRINGPASS_CAP,INPUT_TYPE_NUMERIC_PASSWORD, (PU8)g_callset_cntx_p->SSBarPassCfrm, 
							   BARR_SIM_PASS_LEN+1, NULL, guiBuffer);
	g_callset_cntx_p->SSBarPassStep = 3;
	/* Set the left/right soft key funtions */
	SetLeftSoftkeyFunction(CallBarrOption,KEY_EVENT_UP);
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}

