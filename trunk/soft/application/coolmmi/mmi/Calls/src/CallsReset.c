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
 * CallsReset.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: CallsReset.c

  	PURPOSE		: Call reset submodule of call history Application

 

 

	DATE		: 10/22/2003

**************************************************************/

#ifndef _CALLS_RESET_C
#define _CALLS_RESET_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
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
#include "custmenures.h"
#include "unicodexdcl.h"
#include "queuegprot.h"
#include "protocolevents.h"
#include "idleappprot.h"
#include "commonscreens.h"
#include "wgui_status_icons.h"
#include "wgui_categories_popup.h"

#include "settingprofile.h"
#include "profilegprots.h"
#include "phonebooktypes.h"
#include "callmanagementstruct.h"

#include "callhistoryenum.h"
#include "callsdefs.h"
#include "callsstruct.h"
#include "callslog.h"
#include "callsreset.h"
#include "callhistorygprots.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_calllog.h"
#include "mtpnp_ad_resdef.h"
#endif /* __MMI_MULTI_SIM__ */
#undef __NEWSIMULATOR 


/**************************************************************

	FUNCTION NAME		: InitCallsReset

  	PURPOSE				: Set highlite/hint handlers for sub menu items.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitCallsReset( void )
{
	/* Set the highlight handler for sub menu items */
	SetHiliteHandler( MENU8113_CALLHISTORY_RESETCALL, HighlightCHISTResetCallLog );
	SetHiliteHandler( MENU_DIALED_CALLS, HighlightCHISTDeleteAllDialed );
	SetHiliteHandler( MENU_MISSED_CALLS, HighlightCHISTDeleteAllMissed );
	SetHiliteHandler( MENU_RECEIVED_CALLS, HighlightCHISTDeleteAllRecvd );
  SetHiliteHandler( MENU_REJECTED_CALL, HighlightCHISTDeleteAllRejected );

#ifdef __MMI_CH_DELETE_ALL_LOGS__
	SetHiliteHandler( MENU_ALL_CALLS, HighlightCHISTDeleteAllLogs );
#endif

	/* Set the hinthandlers for the sub menu items */
	SetHintHandler( MENU_DIALED_CALLS, HintCHISTDialedCalls );
	SetHintHandler( MENU_MISSED_CALLS, HintCHISTMissedCalls );
	SetHintHandler( MENU_RECEIVED_CALLS, HintCHISTRecvdCalls );
  SetHintHandler( MENU_REJECTED_CALL, HintCHISTRejectedCalls );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTResetCallLog

  	PURPOSE				: Function called when reset is highlited

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTResetCallLog( void )
{
	/* Set the left/right arrow key functions */
	SetKeyHandler( EntryCHISTResetCallLog,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	/* Set the left and right soft key functions */
	SetLeftSoftkeyFunction( EntryCHISTResetCallLog, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTDeleteAllDialed

  	PURPOSE				: Dial call list option Delete All handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTDeleteAllDialed( void )
{
	chis_p->isDeleteAll = FALSE;

	/* Check if the number of dialed calls is zero or not */
	if( chis_p->nDialedCalls != 0)
	{
		/* dialed calls count is non zero */
		/* Change left soft key icon and label */
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
		/*3 Register function for left soft key */
		SetLeftSoftkeyFunction( EntryCHISTDeleteAllDialedConfirm,
		                        KEY_EVENT_UP );
	}
	else
	{
		/* dialed calls count is zero then disable left soft key */
		ChangeLeftSoftkey( 0, 0);
	}

	/* Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/* Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTDeleteAllMissed

  	PURPOSE				: Missed call list option delete all handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTDeleteAllMissed( void )
{
  	chis_p->isDeleteAll = FALSE;

	/* Check if the number of missed calls is zero or not */
	if( chis_p->nMissedCalls != 0)
	{
		/* missed calls count is non zero */
		/* Change left soft key icon and label */
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
		/* Register function for left soft key */
		SetLeftSoftkeyFunction( EntryCHISTDeleteAllMissedConfirm,
		                        KEY_EVENT_UP );
	}
	else
	{
		/* Missed calls count is zero then disable left soft key */
		ChangeLeftSoftkey( 0, 0);
	}

	/* Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/* Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTDeleteAllRecvd

  	PURPOSE				: Recd call list option Delete All handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTDeleteAllRecvd( void )
{
   chis_p->isDeleteAll = FALSE;

	/* Check if the number of received calls is zero or not */
	if( chis_p->nRecvdCalls != 0)
	{
		/* received calls count is non zero */
		/* Change left soft key icon and label */
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
		/* Register function for left soft key */
		SetLeftSoftkeyFunction( EntryCHISTDeleteAllRecvdConfirm,
		                        KEY_EVENT_UP );
	}
	else
	{
		/* received calls count is zero then disable left soft key */
		ChangeLeftSoftkey( 0, 0);
	}

	/* Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/* Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void HighlightCHISTDeleteAllRejected( void )
{
   chis_p->isDeleteAll = FALSE;

	/* Check if the number of received calls is zero or not */
	if( chis_p->nRejectedCalls != 0)
	{
		/* received calls count is non zero */
		/* Change left soft key icon and label */
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
		/* Register function for left soft key */
		SetLeftSoftkeyFunction( EntryCHISTDeleteAllRejectedConfirm,
		                        KEY_EVENT_UP );
	}
	else
	{
		/* received calls count is zero then disable left soft key */
		ChangeLeftSoftkey( 0, 0);
	}

	/* Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/* Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTDeleteAllLogs

  	PURPOSE				: Call list option Delete All handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTDeleteAllLogs( void )
{
   /* Check if the number of dialed calls is zero or not */
	if( chis_p->nDialedCalls != 0 )
	{
      chis_p->isDeleteAll = TRUE;
		/* dialed calls count is non zero */
		/* Change left soft key icon and label */
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
		/*3 Register function for left soft key */
		SetLeftSoftkeyFunction( EntryCHISTDeleteAllDialedConfirm,
		                        KEY_EVENT_UP );
	}
	else if ( chis_p->nMissedCalls != 0 )
	{
	   chis_p->isDeleteAll = TRUE;
		/* missed calls count is non zero */
		/* Change left soft key icon and label */
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
		/*3 Register function for left soft key */
		SetLeftSoftkeyFunction( EntryCHISTDeleteAllMissedConfirm,
		                        KEY_EVENT_UP );
   }
	else if ( chis_p->nRecvdCalls != 0 )
	{
		/* recd calls count is non zero */
		/* Change left soft key icon and label */
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
		/*3 Register function for left soft key */
		SetLeftSoftkeyFunction( EntryCHISTDeleteAllRecvdConfirm,
		                        KEY_EVENT_UP );
	}
  else if ( chis_p->nRejectedCalls != 0)
  {
    /* recd calls count is non zero */
		/* Change left soft key icon and label */
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
		/*3 Register function for left soft key */
		SetLeftSoftkeyFunction( EntryCHISTDeleteAllRejectedConfirm,
		                        KEY_EVENT_UP );
  }
	else
	{
		/* dialed calls count is zero then disable left soft key */
		ChangeLeftSoftkey( 0, 0);
	}

	/* Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/* Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTResetCallLog

  	PURPOSE				: Entry function of reset calls.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTResetCallLog( void )
{
	//U16			nDispAttribute;  /* Stores display attribue */
	U8*			guiBuffer;       /* Buffer holding history data */
	U16			nStrItemList[MAX_SUB_MENUS];/* Stores the strings id of submenus returned */
	U16			nNumofItem;		 /* Stores no of children in the submenu*/
	U16			ItemIcons[MAX_SUB_MENUS];
	U8*			PopUpList[MAX_SUB_MENUS];

	/* Call Exit Handler*/
#ifdef __MMI_CH_NUMERIC_LIST_ICON__
	EntryNewScreen(SCR_CALLHISTORY_RESETCALL, wgui_restore_list_menu_slim_style,
	               EntryCHISTResetCallLog, NULL);
#else

	EntryNewScreen(SCR_CALLHISTORY_RESETCALL, NULL,
	               EntryCHISTResetCallLog, NULL);
#endif
	/* Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR_CALLHISTORY_RESETCALL);
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild( MENU8113_CALLHISTORY_RESETCALL );
	/* Get attribute of menu to be displayed */
	//nDispAttribute = GetDispAttributeOfItem( MENU8113_CALLHISTORY_RESETCALL );
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds( MENU8113_CALLHISTORY_RESETCALL, nStrItemList );
	GetSequenceImageIds( MENU8113_CALLHISTORY_RESETCALL, ItemIcons );
	/* Set current parent id*/
	SetParentHandler( MENU8113_CALLHISTORY_RESETCALL );
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler( ExecuteCurrHiliteHandler );
	/* Construct the hint handler for the menu items */
	ConstructHintsList(MENU8113_CALLHISTORY_RESETCALL, PopUpList);

#ifdef __MMI_CH_NUMERIC_LIST_ICON__
      wgui_override_list_menu_slim_style(WGUI_LIST_MENU_SLIM_STYLE_NUMBER);
#endif

	/* Display the screen for the reset call logs */
	ShowCategory52Screen( STR_MENU8113_CALLHISTORY_RESETCALL,
	                      IMG_SCR8056_CAPTION,
	                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
	                      IMG_GLOBAL_BACK, nNumofItem, nStrItemList,
	                      ItemIcons, ( U8 * * )
	                      PopUpList, 0, 0,guiBuffer );

	/* Register function with right softkey */
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTDeleteAllDialedConfirm

  	PURPOSE				: Entry funtion for delete dialed list confirmation screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTDeleteAllDialedConfirm( void )
{

	DisplayConfirm(
			STR_GLOBAL_YES,IMG_GLOBAL_YES,
			STR_GLOBAL_NO,IMG_GLOBAL_NO,
			get_string(STR_SCR_RESETCALLS_CONFIRM_CAPTION), IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* Set left/right soft key functions */
	SetLeftSoftkeyFunction(DeleteAllDialedCalls,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* Set the exit function for the current screen */
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTDeleteAllMissedConfirm

  	PURPOSE				: Entry funtion for delete missed call list
  	                    confirmation screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTDeleteAllMissedConfirm( void )
{

	DisplayConfirm(
			STR_GLOBAL_YES,IMG_GLOBAL_YES,
			STR_GLOBAL_NO,IMG_GLOBAL_NO,
			get_string(STR_SCR_RESETCALLS_CONFIRM_CAPTION), IMG_GLOBAL_QUESTION, WARNING_TONE);


	/* Set the left/right soft key functions */
	SetLeftSoftkeyFunction(DeleteAllMissedCalls,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* Set the exit function for the current screen */
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTDeleteAllRecvdConfirm

  	PURPOSE				: Entry funtion for delete received list confirmation screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTDeleteAllRecvdConfirm( void )
{

	DisplayConfirm(
			STR_GLOBAL_YES,IMG_GLOBAL_YES,
			STR_GLOBAL_NO,IMG_GLOBAL_NO,
			get_string(STR_SCR_RESETCALLS_CONFIRM_CAPTION), IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* Set left/right soft key functions */
	SetLeftSoftkeyFunction(DeleteAllRecdCalls,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* Set the exit function for the current screen */
	return;
}

void EntryCHISTDeleteAllRejectedConfirm( void )
{

	DisplayConfirm(
			STR_GLOBAL_YES,IMG_GLOBAL_YES,
			STR_GLOBAL_NO,IMG_GLOBAL_NO,
			get_string(STR_SCR_RESETCALLS_CONFIRM_CAPTION), IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* Set left/right soft key functions */
	SetLeftSoftkeyFunction(DeleteAllRejectedCalls,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* Set the exit function for the current screen */
	return;
}


/**************************************************************

	FUNCTION NAME		: DeleteAllDialedCalls

  	PURPOSE				: To delete all the dialed call entries

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeleteAllDialedCalls( void )
{
	MYQUEUE Message;
	DEL_LAST_NUMBER_REQ   *dataPtr;

	/* clear key handler; not to process any other key */
	ClearAllKeyHandler();

	/* format the message for delete all dialed calls */
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_DEL_LAST_NUMBER_REQ;

	dataPtr = (DEL_LAST_NUMBER_REQ *)
	               OslConstructDataPtr(sizeof(DEL_LAST_NUMBER_REQ));

	dataPtr->type = PHB_LND;
	dataPtr->index = 0xff;
	dataPtr->no_data = 0;

	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

	/* Set the response handler for the delete all missed calls request */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspDeleteAllDialedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#else
	SetProtocolEventHandler( RspDeleteAllDialedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#endif

	 
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: RspDeleteAllDialedNum

  	PURPOSE				: Response handler for deleteall dialed calls request.

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspDeleteAllDialedNum( void *info )
{
	DEL_LAST_NUMBER_RSP *rsp;

	/* retrive the data from the response info structure */
	rsp = (DEL_LAST_NUMBER_RSP *) info;

   /* Check the response is success or not */
   if( rsp->result.flag == CALL_LOG_RESULT_OK)
   {
		chis_p->nDialedCalls = 0;
	}

	if (chis_p->isDeleteAll == TRUE)
	{
	   if (chis_p->nMissedCalls != 0)
	   {
         DeleteAllMissedCalls();
         return;
	   }
	   else if (chis_p->nRecvdCalls != 0)
	   {
	      DeleteAllRecdCalls();
	      return;
	   }
     else if (chis_p->nRejectedCalls != 0)
     {
	      DeleteAllRejectedCalls();
	      return;
     }
	   chis_p->isDeleteAll = FALSE;
	}

	if( rsp->result.flag == CALL_LOG_RESULT_OK)
	{
		/* Incase of success */
		/* Display success popup for user notification */
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
	}
	else
	{
		/* If failure; then display failure popup for user notification */
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}

   /* Delete screens in history to go back to reset call main screen */
	//DeleteUptoScrID(SCR_CALLHISTORY_RESETCALL);
	return;
}

/**************************************************************

	FUNCTION NAME		: DeleteAllMissedCalls

  	PURPOSE				: To delete all the missed call entries

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeleteAllMissedCalls( void )
{
	MYQUEUE Message;
	DEL_LAST_NUMBER_REQ   *dataPtr;

	/* clear key handler; not to process any other key */
	ClearAllKeyHandler();

	/* format the message for delete all missed calls */
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_DEL_LAST_NUMBER_REQ;

	dataPtr = (DEL_LAST_NUMBER_REQ *)
	   OslConstructDataPtr(sizeof(DEL_LAST_NUMBER_REQ));

	dataPtr->type = PHB_LNM;
	dataPtr->index = 0xff;
	dataPtr->no_data = 0;

	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

	/* Set the response handler for the delete all missed calls request */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspDeleteAllMissedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#else
	SetProtocolEventHandler( RspDeleteAllMissedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#endif

	 
	OslMsgSendExtQueue(&Message);
	return;
}

/**************************************************************

	FUNCTION NAME		: RspDeleteAllMissedNum

  	PURPOSE				: Response handler for deleteallmissedcalls request.

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspDeleteAllMissedNum( void *info )
{
	DEL_LAST_NUMBER_RSP *rsp;

	/* retrive the data from the response info structure */
	rsp = (DEL_LAST_NUMBER_RSP *) info;

	/* Check the response is success or not */
	if( rsp->result.flag == CALL_LOG_RESULT_OK)
	{
		/* Incase of success */
		/* Reset the missed call icon on idle screen */
		chis_p->nMissedCalls = 0;
		chis_p->nMissedCallBeforeView = 0;
		mmi_chist_rearrange_missed_call_status_icon();	

#ifdef __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__
        mmi_chist_set_num_missed_call_before_view();
#endif

#ifdef __MMI_MULTI_SIM__
	DeleteScreenIfPresent(SCR_MTPNP_CALL_HISTORY_NOTIFY);
#endif
	}

	if (chis_p->isDeleteAll == TRUE)
	{
	   if (chis_p->nRecvdCalls != 0)
	   {
	      DeleteAllRecdCalls();
	      return;
	   }
     else if (chis_p->nRejectedCalls != 0)
     {
	      DeleteAllRejectedCalls();
	      return;
     }
	   chis_p->isDeleteAll = FALSE;
	}

	if( rsp->result.flag == CALL_LOG_RESULT_OK)
	{
	   /* Incase of success */
	   /* Display success popup for user notification */
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SAVE_TONE);
	}
	else
	{
		/* If failure; then display failure popup for user notification */
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}

	/* Delete screens in history to go back to reset call main screen */
	//DeleteUptoScrID(SCR_CALLHISTORY_RESETCALL);
	return;
}

/**************************************************************

	FUNCTION NAME		: DeleteAllRecdCalls

  	PURPOSE				: To delete all the recd call entries

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeleteAllRecdCalls( void )
{
	MYQUEUE Message;
	DEL_LAST_NUMBER_REQ   *dataPtr;

	/* clear key handler; not to process any other key */
	ClearAllKeyHandler();

	/* format the message for delete all received calls */
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_DEL_LAST_NUMBER_REQ;

	dataPtr = (DEL_LAST_NUMBER_REQ *)
	               OslConstructDataPtr(sizeof(DEL_LAST_NUMBER_REQ));

	dataPtr->type = PHB_LNR;
	dataPtr->index = 0xff;
	dataPtr->no_data = 0;

	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

	/* Set the response handler for the delete all received calls request */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspDeleteAllRecvdNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#else
	SetProtocolEventHandler( RspDeleteAllRecvdNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#endif

	 
	OslMsgSendExtQueue(&Message);
	return;
}

void DeleteAllRejectedCalls( void )
{
	MYQUEUE Message;
	DEL_LAST_NUMBER_REQ   *dataPtr;

	/* clear key handler; not to process any other key */
	ClearAllKeyHandler();

	/* format the message for delete all received calls */
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_PHB_DEL_LAST_NUMBER_REQ;

	dataPtr = (DEL_LAST_NUMBER_REQ *)
	               OslConstructDataPtr(sizeof(DEL_LAST_NUMBER_REQ));

	dataPtr->type = PHB_LNRJ;
	dataPtr->index = 0xff;
	dataPtr->no_data = 0;

	Message.oslDataPtr = (oslParaType *)dataPtr;
	Message.oslPeerBuffPtr = NULL;

	/* Set the response handler for the delete all received calls request */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspDeleteAllRejectedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#else
	SetProtocolEventHandler( RspDeleteAllRejectedNum, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP );
#endif

	 
	OslMsgSendExtQueue(&Message);
	return;
}

void RspDeleteAllRejectedNum( void *info )
{
	DEL_LAST_NUMBER_RSP *rsp;

	/* retrive the data from the response info structure */
	rsp = (DEL_LAST_NUMBER_RSP *) info;

	if( rsp->result.flag == CALL_LOG_RESULT_OK)
	{
		/* Incase of success */
		/* Reset the missed calls counters */
 
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
		//IdleResetStatusIcon( STATUS_ICON_SUBLCD_REJECTED_CALL );
#endif
 
		chis_p->nRejectedCalls = 0;
		chis_p->nRejectedCallBeforeView = 0;

		/* Reset the missed call icon on idle screen */
		mmi_chist_rearrange_rejected_call_status_icon();
	}

	chis_p->isDeleteAll = FALSE;

	/* Check the response is success or not */
   if( rsp->result.flag == CALL_LOG_RESULT_OK)
	{
		/* Incase of success */
		/* Reset the received calls counters */
		chis_p->nRejectedCalls = 0;
		/* Display success popup for user notification */
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
	}
	else
	{
		/* If failure; then display failure popup for user notification */
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}

	/* Delete screens in history to go back to reset call main screen */
	//DeleteUptoScrID(SCR_CALLHISTORY_RESETCALL);
	return;
}

/**************************************************************

	FUNCTION NAME		: RspDeleteAllRecvdNum

  	PURPOSE				: Response handler for deleteall received calls request.

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspDeleteAllRecvdNum( void *info )
{
	DEL_LAST_NUMBER_RSP *rsp;

	/* retrive the data from the response info structure */
	rsp = (DEL_LAST_NUMBER_RSP *) info;

   if( rsp->result.flag == CALL_LOG_RESULT_OK)
   {
		chis_p->nRecvdCalls = 0;
	}

	if (chis_p->isDeleteAll == TRUE)
	{
     if (chis_p->nRejectedCalls != 0)
     {
	      DeleteAllRejectedCalls();
	      return;
     }
	   chis_p->isDeleteAll = FALSE;
	}

	/* Check the response is success or not */
   if( rsp->result.flag == CALL_LOG_RESULT_OK)
	{
		/* Incase of success */
		/* Reset the received calls counters */
		chis_p->nRecvdCalls = 0;
		/* Display success popup for user notification */
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
	}
	else
	{
		/* If failure; then display failure popup for user notification */
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}

	/* Delete screens in history to go back to reset call main screen */
	//DeleteUptoScrID(SCR_CALLHISTORY_RESETCALL);
	return;
}

#endif


