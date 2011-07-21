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
 * CallTimes.C
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

	FILENAME	: CallTimes.c 

  	PURPOSE		: To maintain call times in call history

 

 

	DATE		: 10/22/2003

**************************************************************/

#ifndef _CALL_TIMES_C
#define _CALL_TIMES_C

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
#include "wgui_categories_popup.h"
#include "wgui_categories_inputs.h"
#include "wgui_inline_edit.h"
#include "commonscreens.h"
#include "custmenures.h"
#include "unicodexdcl.h"

#include "settingdefs.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "phonebooktypes.h"
#include "callsetupenum.h"

#include "nvramenum.h"
#include "nvramtype.h"
#include "nvramprot.h"

#include "callhistoryenum.h"
#include "callsstruct.h"
#include "callsdefs.h"
#include "calltimes.h"

 
#ifdef __MMI_KLG__
#include "debuginitdef.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "settinggenum.h"
#endif
 
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_calllog.h"
#endif
#undef __NEWSIMULATOR 

#ifdef __MMI_CH_QUICK_END__
U8 *QuickEndString[2];
U8 QuickEndInput[20];
#endif

/**************************************************************
	FUNCTION NAME		: InitCallTimes

 	PURPOSE				: Set highlite/hint handlers for sub menu items.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitCallTimes( void )
{
	/* Set the highlight handler for sub menu items */
	SetHiliteHandler( MENU_CALL_TIME_SETUP, HighlightCHISTCallTime );
	SetHiliteHandler( MENU_LAST_CALL_TIME, HighlightCHISTLastTime );
	SetHiliteHandler( MENU_CALL_OUT_TIME, HighlightCHISTSentTime );
	SetHiliteHandler( MENU_CALL_IN_TIME, HighlightCHISTRecvdTime );
	SetHiliteHandler( MENU_ALL_CALL_TIME, HighlightCHISTResetAllTime );
	/* Set the hinthandlers for the sub menu items */
	SetHintHandler( MENU_LAST_CALL_TIME, HintCHISTLastTime );
	SetHintHandler( MENU_CALL_OUT_TIME, HintCHISTSentTime );
	SetHintHandler( MENU_CALL_IN_TIME, HintCHISTRecvdTime );

#ifdef __MMI_CH_QUICK_END__
	SetHiliteHandler( MENU_CALL_HISTORY_QUICK_END, HighlightCHISTQuickEnd );
	SetHintHandler(MENU_CALL_HISTORY_QUICK_END, HintCHISTQuickEnd);
#endif

	return;
}

/**************************************************************
	FUNCTION NAME		: HighlightCHISTCallTime

 	PURPOSE				: Call time menu option highlite handler

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTCallTime( void )
{
	/* Set the left/right arrow key function */
	SetKeyHandler( EntryCHISTCallTime,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	/* Register functions for left/right soft keys */
	SetLeftSoftkeyFunction( EntryCHISTCallTime, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTLastTime

  	PURPOSE				: Highlite function for last call time

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTLastTime( void )
{
	/* Change left/right soft key icon and label */
	ChangeLeftSoftkey( 0, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/* Set left/right soft key functions */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	ClearOneKeyAllHandler(KEY_RIGHT_ARROW);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTSentTime

  	PURPOSE				: Highlight function for total call time out

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTSentTime( void )
{
	/* Change left/right soft key icon and label */
	ChangeLeftSoftkey( 0, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/* Set right soft key functions */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	ClearOneKeyAllHandler(KEY_RIGHT_ARROW);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTRecvdTime

  	PURPOSE				: Highlight function for received call time

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTRecvdTime( void )
{
	/* Change left/right soft key icon and label */
	ChangeLeftSoftkey( 0, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	/* Set right softkey functions */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	ClearOneKeyAllHandler(KEY_RIGHT_ARROW);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTResetAllTime

  	PURPOSE				: Highlight handler for reset call time

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTResetAllTime( void )
{
	/* Change left soft key icon and label */
	if ( CHISTNeedToResetTimes() == 0)
	{
		ChangeLeftSoftkey( 0, 0);
	}
	else
	{
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
 
#ifdef __MMI_KLG__
		SetLeftSoftkeyFunction( EntryCHISTResetCheckPhoneLock, KEY_EVENT_UP ); /* check phone lock always */
#else
		SetLeftSoftkeyFunction( EntryCHISTResetAllTimeConfirm, KEY_EVENT_UP );
#endif
 
	}

	/* Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

	return;
}

#ifdef __MMI_CH_QUICK_END__
/**************************************************************

	FUNCTION NAME		: HighlightCHISTQuickEnd

  	PURPOSE				: Highlight handler for quick end

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTQuickEnd( void )
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	
	/* Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );

	SetLeftSoftkeyFunction( EntryCHISTQuickEnd, KEY_EVENT_UP );
	SetKeyHandler(EntryCHISTQuickEnd, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HintCHISTQuickEnd

  	PURPOSE				: Construct hint string for quick end time

	INPUT PARAMETERS	: u16 - index of the hint popup to display.

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTQuickEnd( U16 index )
{
	S16 error;

	ReadValue( NVRAM_CH_QUICK_END_FLAG, &chis_p->nQuickEndFlag, DS_BYTE , &error);

	if(chis_p->nQuickEndFlag == 0xff)
	{
		chis_p->nQuickEndFlag=0;
		WriteValue( NVRAM_CH_QUICK_END_FLAG, &chis_p->nQuickEndFlag, DS_BYTE , &error);
	}

		ReadValue( NVRAM_CH_QUICK_END_TIME, &chis_p->nQuickEndTime, DS_SHORT, &error);
		if (chis_p->nQuickEndTime == 0xffff)
		{
			chis_p->nQuickEndTime = 0;
			WriteValue( NVRAM_CH_QUICK_END_TIME, &chis_p->nQuickEndFlag, DS_SHORT , &error);			
		}

	if (chis_p->nQuickEndFlag == 1)
	{
		U8 data[10];
		
		sprintf ((PS8)data, "%d \0", chis_p->nQuickEndTime);
		AnsiiToUnicodeString((S8*)hintData[index], (S8*)data);
		pfnUnicodeStrcat((S8*)hintData[index], (S8*)GetString(STR_SECONDS));
	}
	else
	{
		pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString(STR_GLOBAL_OFF));
	}
	
	return;
}
#endif

/**************************************************************

	FUNCTION NAME		: HintCHISTLastTime

  	PURPOSE				: Construct hint for last call time

	INPUT PARAMETERS	: u16 - index of the hint popup to display.

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTLastTime( U16 index )
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	/* Format the time to display for hint popup */
	sprintf((S8 *)value, "%02d:%02d:%02d",
	        chis_p->last_call_time.nHour,
	        chis_p->last_call_time.nMin,
	        chis_p->last_call_time.nSec );

	AnsiiToUnicodeString((S8 *)hintData[index], (S8 *)value);

	return;
}

/**************************************************************

	FUNCTION NAME		: HintCHISTSentTime

  	PURPOSE				: construct hint string for call total out call time

	INPUT PARAMETERS	: u16 - index of the hint popup to display.

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTSentTime( U16 index )
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	/* Format the time to display for hint popup */
	sprintf( (S8 *)value, "%02d:%02d:%02d",
	         chis_p->total_out_time.nHour,
	         chis_p->total_out_time.nMin,
	         chis_p->total_out_time.nSec );

	AnsiiToUnicodeString((S8 *)hintData[index], (S8 *)value);

	return;
}

/**************************************************************

	FUNCTION NAME		: HintCHISTRecvdTime

  	PURPOSE				: Construct hint string for in call time

	INPUT PARAMETERS	: u16 - index of the hint popup to display.

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTRecvdTime( U16 index )
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	/* Format the time to display for hint popup */
	sprintf( (S8 *)value, "%02d:%02d:%02d",
	         chis_p->total_in_time.nHour,
	         chis_p->total_in_time.nMin,
	         chis_p->total_in_time.nSec );

	AnsiiToUnicodeString((S8 *)hintData[index], (S8 *)value);

	return;
}

/**************************************************************
	FUNCTION NAME		: EntryCHISTCallTime

 	PURPOSE				: Entry screen for call times.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTCallTime( void )
{
	U16	nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16	nNumofItem;      /* Stores no of children in the submenu*/
	//U16	nDispAttribute;  /* Stores display attribue */
	U8*	guiBuffer;       /* Buffer holding history data */
	U8	   *displayBuffer[MAX_SUB_MENUS];
#ifdef __MMI_MULTI_SIM__
	S16   error=0;
#endif

	/* Call Exit Handler*/
	EntryNewScreen(SCR_CALL_TIME_SETUP_MAIN, NULL, EntryCHISTCallTime, NULL);

#ifdef __MMI_MULTI_SIM__
	ReadRecord(NVRAM_EF_CALL_TIME_LID,1 + 3*chis_p->nSimId, (void *)&chis_p->last_call_time, sizeof(MYTIME), &error);

	ReadRecord(NVRAM_EF_CALL_TIME_LID,2 + 3*chis_p->nSimId, (void *)&chis_p->total_out_time, sizeof(MYTIME), &error);

	ReadRecord(NVRAM_EF_CALL_TIME_LID,3 + 3*chis_p->nSimId, (void *)&chis_p->total_in_time, sizeof(MYTIME), &error);
#else  /* No Defined __MMI_MULTI_SIM__ */
#endif /* __MMI_MULTI_SIM__ */

	/* Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR_CALL_TIME_SETUP_MAIN );
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild( MENU_CALL_TIME_SETUP );
	/* Get attribute of menu to be displayed */
	//nDispAttribute = GetDispAttributeOfItem( MENU_CALL_TIME_SETUP );
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds( MENU_CALL_TIME_SETUP, nStrItemList );
	/* Set current parent id*/
	SetParentHandler( MENU_CALL_TIME_SETUP );
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler( ExecuteCurrHiliteHandler );
	/* Construct hint for menu items */
	ConstructHintsList(MENU_CALL_TIME_SETUP, displayBuffer);

	/* Display Category1 Screen */
	ShowCategory52Screen( STR_MENU_CALL_TIME_SETUP, IMG_MENU_CALL_TIMES,
	                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
	                      IMG_GLOBAL_BACK, nNumofItem, nStrItemList,
	                      (U16 *)gIndexIconsImageList,
	                      displayBuffer, 0, 0, guiBuffer );

	/* Register function with right softkey */
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTResetAllTimeConfirm

  	PURPOSE				: Entry function for call time reset confirm screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTResetAllTimeConfirm( void )
{

	DisplayConfirm(
		STR_GLOBAL_YES,IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO,
		get_string(STR_RESET_ALL_CALL_TIME_CONFIRM), IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* set left/right soft key functions */
	SetLeftSoftkeyFunction(CHISTResetAllTime,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* Set exit funtion for the current screen */
	return;
}

#ifdef __MMI_CH_QUICK_END__
/**************************************************************

	FUNCTION NAME		: EntryCHISTQuickEnd

  	PURPOSE				: Entry function for set quick end time screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTQuickEnd(void)
{
	U8* guiBuffer;
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
 	U16 QuickEndIcons[4] = {IMG_SETTING_GREET_STATUS, 0, IMG_TIME, 0};	
	extern wgui_inline_item wgui_inline_items[];
 
	EntryNewScreen(SCR_CALL_TIME_QUICK_END, ExitCHISTQuickEnd, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_CALL_TIME_QUICK_END);

	InitializeCategory57Screen();
	
	memset( wgui_inline_items, 0, sizeof(wgui_inline_item) * MAX_INLINE_ITEMS );
	CHISTQuickEndFillInlineStruct( (void *) wgui_inline_items );
	
	inputBuffer = GetCurrNInputBuffer (SCR_CALL_TIME_QUICK_END, &inputBufferSize );    //added for inline edit history
	
	if(inputBuffer!=NULL)                                            //added for inline edit history
	    SetCategory57Data(wgui_inline_items, 4, inputBuffer);//sets the data 
 
	RegisterHighlightHandler(CHISTQuickEndHiliteHandler);

	
	ShowCategory57Screen(STR_MENU_QUICK_END, GetRootTitleIcon(MENU_CALL_HISTORY_QUICK_END),
						STR_GLOBAL_OK,IMG_GLOBAL_OK, STR_GLOBAL_BACK,STR_GLOBAL_BACK,
						4, QuickEndIcons,
						wgui_inline_items, 0,guiBuffer);


	SetCategory57RightSoftkeyFunctions(CHISTSetQuickEndTime,GoBackHistory);
	DisableCategory57ScreenDone();	
}

/*****************************************************************************
* FUNCTION
*  ExitCHISTQuickEnd
* DESCRIPTION
*   This function is Exit function for Quick End
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitCHISTQuickEnd(void)
{
	history_t   Scr; 
	S16 nHistory =0;
	U16 inputBufferSize;                //added for inline edit history
	
	Scr.scrnID = SCR_CALL_TIME_QUICK_END;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntryCHISTQuickEnd;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history
}

/*****************************************************************************
* FUNCTION
*  CHISTQuickEndFillInlineStruct
* DESCRIPTION
*   This function is to fill inline selection structure for Quick End Time
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void CHISTQuickEndFillInlineStruct( void *p )
{
	S16 error;
	U8 data[10];
	wgui_inline_item *setting_inline_edit_items = (wgui_inline_item *) p;

	ReadValue( NVRAM_CH_QUICK_END_FLAG, &chis_p->nQuickEndFlag, DS_BYTE , &error);
	ReadValue( NVRAM_CH_QUICK_END_TIME, &chis_p->nQuickEndTime, DS_SHORT, &error);

	QuickEndString[0] = (PU8)GetString(STR_GLOBAL_OFF);
	QuickEndString[1] = (PU8)GetString(STR_GLOBAL_ON);
	sprintf ((PS8)data, "%d\0", chis_p->nQuickEndTime);
	AnsiiToUnicodeString((PS8)QuickEndInput, (PS8)data);

	SetInlineItemActivation(&(setting_inline_edit_items[1]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);
	SetInlineItemActivation(&(setting_inline_edit_items[3]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);

	SetInlineItemCaption(&(setting_inline_edit_items[0]),(U8*)GetString(STR_MENU_QUICK_END_SET));
	SetInlineItemSelect(&(setting_inline_edit_items[1]),2,(U8**)QuickEndString, (S32*)&chis_p->nQuickEndFlag);
	SetInlineItemFullWidth(&setting_inline_edit_items[1]);
	
	SetInlineItemCaption(&(setting_inline_edit_items[2]),(U8*)GetString(STR_MENU_QUICK_END_DURATION));
	SetInlineItemTextEdit(&(setting_inline_edit_items[3]), QuickEndInput, 5, INPUT_TYPE_NUMERIC);
}

/*****************************************************************************
* FUNCTION
*  CHISTQuickEndHiliteHandler
* DESCRIPTION
*   This function is Inline line editor Highlight handler for Quick End TIme
*   Follow functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void CHISTQuickEndHiliteHandler(S32 currId)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	SetCategory57LeftSoftkeyFunction(CHISTSetQuickEndTime);	
}

/**************************************************************

	FUNCTION NAME		: CHISTSetQuickEndTime

  	PURPOSE				: Sets quick end time.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTSetQuickEndTime( void )
{
	S16 error=0;
	U8 data[10];

	UnicodeToAnsii((PS8)data, (PS8)QuickEndInput);
	chis_p->nQuickEndTime = atoi ((PS8)data);

	if (chis_p->nQuickEndFlag == MMI_TRUE && chis_p->nQuickEndTime == 0)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_INVALID), 
			IMG_GLOBAL_ERROR, 0, 1000, WARNING_TONE);
		return;
	}
	WriteValue( NVRAM_CH_QUICK_END_FLAG, &chis_p->nQuickEndFlag, DS_BYTE , &error);
	WriteValue( NVRAM_CH_QUICK_END_TIME, &chis_p->nQuickEndTime, DS_SHORT , &error);
	
	/* Display success popup for reset */
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
	             IMG_GLOBAL_ACTIVATED, 1,
	             CS_NOTIFYDURATION,SUCCESS_TONE);

	/* delete screens from history to go to main call history screen */
	DeleteScreenIfPresent(SCR_CALL_TIME_QUICK_END);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTGetQuickEndTime

  	PURPOSE				: Sets quick end time.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U16 CHISTGetQuickEndTime( void )
{
	S16 error;

	ReadValue( NVRAM_CH_QUICK_END_FLAG, &chis_p->nQuickEndFlag, DS_BYTE , &error);
	if (chis_p->nQuickEndFlag == 0xff)
	{
		chis_p->nQuickEndFlag = 0;
		WriteValue( NVRAM_CH_QUICK_END_FLAG, &chis_p->nQuickEndFlag, DS_BYTE , &error);
	}
	if (chis_p->nQuickEndFlag > 0)
	{
		ReadValue( NVRAM_CH_QUICK_END_TIME, &chis_p->nQuickEndTime, DS_SHORT, &error);
		if (chis_p->nQuickEndTime == 0xffff)
		{
			chis_p->nQuickEndTime = 0;
			WriteValue( NVRAM_CH_QUICK_END_TIME, &chis_p->nQuickEndFlag, DS_SHORT , &error);			
		}
		
		return chis_p->nQuickEndTime;
	}
	else
		return 0;
}
#endif

/**************************************************************

	FUNCTION NAME		: CHISTResetAllTime

  	PURPOSE				: Resets the all call times.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTResetAllTime( void )
{
	S16 error=0;

	/* reset the last call time */
	memset( &chis_p->last_call_time, 0, sizeof( MYTIME ) );
	/* Reset the total out time */
	memset( &chis_p->total_out_time, 0, sizeof( MYTIME ) );
	/* Reset the total in time */
	memset( &chis_p->total_in_time, 0, sizeof( MYTIME ) );

#ifdef __MMI_MULTI_SIM__
	WriteRecord(NVRAM_EF_CALL_TIME_LID,1 + 3*chis_p->nSimId,(void *)&chis_p->last_call_time,
	            sizeof(MYTIME), &error);
	WriteRecord(NVRAM_EF_CALL_TIME_LID,2 + 3*chis_p->nSimId,(void *)&chis_p->total_out_time,
	            sizeof(MYTIME), &error);
	WriteRecord(NVRAM_EF_CALL_TIME_LID,3 + 3*chis_p->nSimId,(void *)&chis_p->total_in_time,
	            sizeof(MYTIME), &error);
#else
	WriteRecord(NVRAM_EF_CALL_TIME_LID,1,(void *)&chis_p->last_call_time,
	            sizeof(MYTIME), &error);
	WriteRecord(NVRAM_EF_CALL_TIME_LID,2,(void *)&chis_p->total_out_time,
	            sizeof(MYTIME), &error);
	WriteRecord(NVRAM_EF_CALL_TIME_LID,3,(void *)&chis_p->total_in_time,
	            sizeof(MYTIME), &error);

#endif /* __MMI_MULTI_SIM__ */

	/* Display success popup for reset */
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
	             IMG_GLOBAL_ACTIVATED, 1,
	             CS_NOTIFYDURATION,SUCCESS_TONE);

	/* delete screens from history to go to main calltime screen */
	DeleteUptoScrID(SCR_CALL_TIME_SETUP_MAIN);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTNeedToResetTimes

  	PURPOSE				: Reset times to be done or not.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8

 

**************************************************************/
BOOL CHISTNeedToResetTimes( void )
{
	U8 count;
	U8 *buf;

	buf = (U8*)&chis_p->last_call_time;
	/* Check whether last call time is zero or not */
	for( count = 0; count < sizeof(MYTIME); count++)
	{
		/* if non zero return 1 */
		if( buf[count] != 0)
			return TRUE;
	}
	buf = (U8*)&chis_p->total_out_time;
	/* Check whether total out call time is zero or not */
	for( count = 0; count < sizeof(MYTIME); count++)
	{
		/* if non zero return 1 */
		if( buf[count] != 0)
			return TRUE;
	}
	buf = (U8*)&chis_p->total_in_time;
	/* Check whether total in call time is zero or not */
	for( count = 0; count < sizeof(MYTIME); count++)
	{
		/* if non zero return 1 */
		if( buf[count] != 0)
			return TRUE;
	}
	/* call time reset is not to be done as its already in reset mode */
	return FALSE;
}

 
#ifdef __MMI_KLG__

extern void SECSETVerifyPINReq(U8 pin_type, U8* ucs2_pin, U8* ucs2_new_pin, FuncPtr success_callback);
extern void RstrSettingPasswdScrSoftKeyFunc(U8* text,U8* cursor,S32 length);

extern U8 gPhonePassword[];

void CHISTResetVerifyPhoneLockReq(void)
{
 	U8 tempBuffer[(MAX_SIM_SETTING_PASSWD_LEN+1)];
	memset(tempBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
	SECSETVerifyPINReq(MMI_SETTING_TYPE_PHONELOCK, gPhonePassword, tempBuffer, EntryCHISTResetAllTimeConfirm);
}

void EntryCHISTResetCheckPhoneLock(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== EntryCHISTResetCheckPhoneLock() ===\n"));

	/* Call Exit Handler*/
	EntryNewScreen(SCR_CALL_TIME_RESET, NULL, EntryCHISTResetCheckPhoneLock, NULL);
	
	/* Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_CALL_TIME_RESET);	
	
	/* check the screen flow is from forward or backward */
	if ( guiBuffer == NULL)
		memset(gPhonePassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
	
	/* register the input validation function handler */
	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	
	/* display the corresponding catergory function */
	ShowCategory111Screen(	STR_GLOBAL_OK , 
				IMG_GLOBAL_OK ,
				STR_GLOBAL_BACK, 
				IMG_GLOBAL_BACK,
				STR_RESTORE_PHONE,
				INPUT_TYPE_NUMERIC_PASSWORD,
				gPhonePassword,
				(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, NULL);

	/* Register function with left/right softkey */
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(CHISTResetVerifyPhoneLockReq,KEY_EVENT_UP);
	
	/* register the key handler function for the pound key */
	SetKeyHandler(CHISTResetVerifyPhoneLockReq,KEY_POUND,KEY_EVENT_UP);

	g_SecuritySetupContext.PasswdLenCheckCallBack = CHISTResetVerifyPhoneLockReq;
	if(pfnUnicodeStrlen((S8*)g_SecuritySetupContext.Password) < MIN_PASSWORD_LENGTH)
	{
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

#endif
 

#endif
