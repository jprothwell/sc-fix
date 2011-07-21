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
 * TypicalStopwatch.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for phone setup application
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

/***************** Stopwatch Application implementation *********************/

/**************************************************************

	FILENAME	: TypicalStopwatch.c

  	PURPOSE		: Typical Stopwatch application implementation

 

 

	DATE		: 05/01/2004

**************************************************************/
#include "mmi_features.h"
#ifdef __MMI_STOPWATCH__
#include "debuginitdef.h"
/***********  Include: MMI header file **************/
#include "mmi_data_types.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_stopwatch.h"
#include "globaldefs.h"
#include "globalscrenum.h"
#include "hardwareevents.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "mainmenudef.h"
#include "profilegprots.h"

#include "stopwatchgdcl.h"
#include "stopwatchdef.h"
#include "stopwatchgprot.h"
#include "stopwatchprot.h"
//#include "wallpaper.h"
#include "unicodexdcl.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "gpioinc.h"
#include "wgui_status_icons.h"
#include "mmi_trace.h"


/***************************************************************************** 
* Define
*****************************************************************************/
#if (!defined(MMI_ON_HARDWARE_P) && !defined(NVRAM_EF_STOPWATCH_RECORD_SIZE) ) 
#define NVRAM_EF_STOPWATCH_RECORD_SIZE sizeof(gTimerRecordList)
#endif


/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Local Variable (Local to this file - static)
*****************************************************************************/

/***************************************************************************** 
* Local Function (local to this file - static)
*****************************************************************************/

/***************************************************************************** 
* Global Variable
*****************************************************************************/

/***************************************************************************** 
* Global Function
*****************************************************************************/


/**********************************************************************
*****************************************************************************/


/**************************************************************

	FUNCTION NAME		: InitStopwatchApp(void)

  	PURPOSE				: Initialize stopwatch Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void InitStopwatchApp(void)
{	
	
	SetHiliteHandler(MENU_STOPWATCH,HighlightStopwatch);
	SetHiliteHandler(MENU_TYPICALSTOPWATCH,HighlightTypicalStopwatch);
	SetHiliteHandler(MENU_MULTIPURPOSEWATCH,HighlightMultipurposeStopwatch);

	SetHiliteHandler(MENU_SPLIT_TIMING,HighlightSplitTiming);
	SetHiliteHandler(MENU_LAP_TIMING,HighlightLapTiming);
	SetHiliteHandler(MENU_VIEW_RECORD,HighlightViewRecord);

	SetHiliteHandler(MENU_SAVE_DONE,HighlightOptionDone);
	SetHiliteHandler(MENU_SAVE_INPUTMETHOD,HighlightOptionInputMethod);

	SetHiliteHandler(MENU_OPTION_VR_VIEW,HighlightOptionView);
	SetHiliteHandler(MENU_OPTION_VR_DELETE,HighlightOptionDelete);
	SetHiliteHandler(MENU_OPTION_VR_DELETE_ALL,HighlightOptionDeleteAll);

	InitMultipurposeStopwatch();
	ResetTypicalStopwatch();
}

/**************************************************************

	FUNCTION NAME		: InitStopwatchNVRAM(void)

  	PURPOSE				: Initialize NVRAM for Stopwatch

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void InitStopwatchNVRAM(void)
{
	S16 error ;

	if(ReadRecord (NVRAM_EF_STOPWATCH_LID, 1 , &gTimerRecordList[0],MAX_STOPWATCH_RECORDS * sizeof(stopwatchRecord),&error) < 0)
	{
		PRINT_INFORMATION (("TYPICAL STOPWATCH : ReadRecord to read Timer Record failed (error code %d)",(int) error));
	}
   if(ReadValue( NVRAM_STOPWATCH_TOTAL_RECORDS, &gStopwatchAppInfo.numOfSavedTimerRecords, DS_BYTE , &error) < 0)
	{
		PRINT_INFORMATION (("TYPICAL STOPWATCH : ReadValue to read total Number of records failed (error code %d)",(int) error));
	}

	if(gStopwatchAppInfo.numOfSavedTimerRecords==0xFF)
	{
		 gStopwatchAppInfo.numOfSavedTimerRecords=0;
		 if(WriteValue( NVRAM_STOPWATCH_TOTAL_RECORDS, &gStopwatchAppInfo.numOfSavedTimerRecords, DS_BYTE , &error) < 0)
		 {
			PRINT_INFORMATION (("TYPICAL STOPWATCH-Bootup Time : WriteValue to write total Number of records failed (error code %d)",(int) error));
		 }
	}

}
/**************************************************************

	FUNCTION NAME		: HighlightStopwatch(void)

  	PURPOSE				: Highlight handler fn for stopwatch.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightStopwatch(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryWtchScrStopwatch,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(EntryWtchScrStopwatch,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */	

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	

}

/**************************************************************

	FUNCTION NAME		: HighlightTypicalStopwatch(void)

  	PURPOSE				: Highlight handler fn for Typical stopwatch

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightTypicalStopwatch(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryWtchScrTypicalStopwatch,KEY_EVENT_UP);
	SetKeyHandler(EntryWtchScrTypicalStopwatch,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	

}

/**************************************************************

	FUNCTION NAME		: HighlightMultipurposeStopwatch(void)

  	PURPOSE				: Highlight handler function for Multipurpose stop watch.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightMultipurposeStopwatch(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryWtchScrMultipurposeStopwatch,KEY_EVENT_UP);
	SetKeyHandler(EntryWtchScrMultipurposeStopwatch,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	//ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightSplitTiming(void)

  	PURPOSE				: Highlight handler fn for Split timing.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightSplitTiming(void)
{
	if((gStopwatchAppInfo.TimerState!= TIMER_NOT_STARTED) && (gStopwatchAppInfo.splitLapTimer==LAP_TIMER))
        {
	   SetLeftSoftkeyFunction(DisplayDiscontinueTimerQuery,KEY_EVENT_UP);
	   SetKeyHandler(DisplayDiscontinueTimerQuery,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
        }
	else
	{
	gStopwatchAppInfo.splitLapTimer=SPLIT_TIMER;
	   /* Register function for left soft key */
	   SetLeftSoftkeyFunction(EntryWtchScrSplitLapTimingStopwatch,KEY_EVENT_UP);
	   SetKeyHandler(EntryWtchScrSplitLapTimingStopwatch,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}

	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightLapTiming(void)

  	PURPOSE				: Highlight handler fn for Lap Timing.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightLapTiming(void)
{
	if((gStopwatchAppInfo.TimerState!= TIMER_NOT_STARTED) && (gStopwatchAppInfo.splitLapTimer==SPLIT_TIMER))
        {
	   SetLeftSoftkeyFunction(DisplayDiscontinueTimerQuery,KEY_EVENT_UP);
   	   SetKeyHandler(DisplayDiscontinueTimerQuery,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
        }
	else
	{
	gStopwatchAppInfo.splitLapTimer=LAP_TIMER;
	   /* Register function for left soft key */
	   SetLeftSoftkeyFunction(EntryWtchScrSplitLapTimingStopwatch,KEY_EVENT_UP);
	   SetKeyHandler(EntryWtchScrSplitLapTimingStopwatch,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}

	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightViewRecord(void)

  	PURPOSE				: Highlight handler fn for View record

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightViewRecord(void)
{
	gStopwatchAppInfo.currHilitedRecord=0;

	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryWtchScrViewRecord,KEY_EVENT_UP);
	SetKeyHandler(EntryWtchScrViewRecord,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	//ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightOptionDone(void)

  	PURPOSE				: Highlight handler for option Done

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightOptionDone(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*3 Register function for left soft key */
	if(gStopwatchAppInfo.replaceRecord==TRUE)
	{
		SetLeftSoftkeyFunction(ReplaceSelectedRecord,KEY_EVENT_UP);
	   SetKeyHandler(ReplaceSelectedRecord,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
	else
	{
	   SetLeftSoftkeyFunction(SaveTimerRecord,KEY_EVENT_UP);
	   SetKeyHandler(SaveTimerRecord,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightOptionInputMethod(void)

  	PURPOSE				: Highlight handler fn for option Input method

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightOptionInputMethod(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	RegisterInputMethodScreenCloseFunction(GoBackHistoryFnFromInputMethod);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryInputMethodScreen,KEY_EVENT_UP);
	SetKeyHandler(EntryInputMethodScreen,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	

}

/**************************************************************

	FUNCTION NAME		: HighlightOptionView(void)

  	PURPOSE				: Highlight handler fn for option View

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightOptionView(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryWtchScrViewSavedRecord,KEY_EVENT_UP);
	SetKeyHandler(EntryWtchScrViewSavedRecord,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	//ClearKeyHandler(KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightOptionDelete(void)

  	PURPOSE				: Highlight handler fn for option Delete

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightOptionDelete(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryWtchScrDeleteRecord,KEY_EVENT_UP);
	//SetKeyHandler(EntryWtchScrDeleteRecord,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	//ClearKeyHandler(KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightOptionDeleteAll(void)

  	PURPOSE				: Highlight handler fn for option Delete All.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void HighlightOptionDeleteAll(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryWtchScrDeleteAllRecord,KEY_EVENT_UP);
	//SetKeyHandler(EntryWtchScrDeleteAllRecord,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	//ClearKeyHandler(KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: EntryWtchScrStopwatch(void)

  	PURPOSE				: Entry fn for Stopwatch

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrStopwatch(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_STOPWATCH, NULL, EntryWtchScrStopwatch, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_STOPWATCH);	

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_STOPWATCH);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_STOPWATCH);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_STOPWATCH,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_STOPWATCH);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
		
	/* 8 Display Category Screen */
	ShowCategory15Screen(STR_MUTIPURPOSE_TITLE_MSG/*STR_STOPWATCH_TITLE*/, GetRootTitleIcon(MENU_STOPWATCH),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  LIST_MENU, 0,guiBuffer);

	//SetExitHandler(SCR_STOPWATCH,ExitWtchScrStopwatch);
}



/**************************************************************

	FUNCTION NAME		: EntryWtchScrTypicalStopwatch(void)

  	PURPOSE				: Entry fn for Typical Stopwatch.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrTypicalStopwatch(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_TYPICAL_STOPWATCH, NULL, EntryWtchScrTypicalStopwatch, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_TYPICAL_STOPWATCH);	

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_TYPICALSTOPWATCH);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TYPICALSTOPWATCH);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_TYPICALSTOPWATCH,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_TYPICALSTOPWATCH);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
		
	/* 8 Display Category Screen */
	ShowCategory15Screen(STR_TYPICAL_STOPWATCH, GetRootTitleIcon(MENU_TYPICALSTOPWATCH),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  LIST_MENU, 0,guiBuffer);

	//SetExitHandler(SCR_TYPICAL_STOPWATCH,ExitWtchScrTypicalStopwatch);

}



/**************************************************************

	FUNCTION NAME		: EntryWtchScrSplitLapTimingStopwatch(void)

  	PURPOSE				: Entry fn for Split/Lap timing stopwatch screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrSplitLapTimingStopwatch(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_SPLIT_TIMING, ExitWtchScrSplitLapTimingStopwatch, EntryWtchScrSplitLapTimingStopwatch, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_SPLIT_TIMING);	

	HideStatusIcon(STATUS_ICON_STOP_WATCH);
	UpdateStatusIcons();
    
	/* 6 Set current parent id*/
	SetParentHandler(MENU_SPLIT_TIMING);

	memset(gStopwatchAppInfo.recordName,0,(MAX_RECORD_NAME_LENGTH+1)*ENCODING_LENGTH);
	/* 8 Display Category Screen */
	 
	if(gStopwatchAppInfo.TimerState== TIMER_NOT_STARTED)
	{
		if(gStopwatchAppInfo.splitLapTimer==SPLIT_TIMER)
			ShowCategory307Screen(STR_STOPWATCH_START, 0,
								  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								  0,NULL,0,guiBuffer,IMG_TYPICALSW_BACKGROUND,CONTINUE_TIMER);
		else
			ShowCategory307Screen(STR_STOPWATCH_START, 0,
								  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								  0,NULL,0,guiBuffer,IMG_TYPICALSW_BACKGROUND,STOP_START_AGAIN);
		SetLeftSoftkeyFunction(StartSplitLapTimer,KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	}
	else if(gStopwatchAppInfo.TimerState== TIMER_STOPPED)
	{
		if(gStopwatchAppInfo.splitLapTimer==SPLIT_TIMER)
			ShowCategory307Screen(STR_STOPWATCH_RESUME, 0,
								  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								  gStopwatchAppInfo.numofCurrTimerRecords,gCurrentRecords,0,guiBuffer,IMG_TYPICALSW_BACKGROUND,CONTINUE_TIMER);
		else
			ShowCategory307Screen(STR_STOPWATCH_RESUME, 0,
								  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								  gStopwatchAppInfo.numofCurrTimerRecords,gCurrentRecords,0,guiBuffer,IMG_TYPICALSW_BACKGROUND,STOP_START_AGAIN);

		SetLeftSoftkeyFunction(ResumeSplitLapTimer,KEY_EVENT_UP);
		SetLeftSoftkeyFunction(ResetSplitLapTimer,KEY_LONG_PRESS);	
		SetRightSoftkeyFunction(ConfirmSaveTimerRecord,KEY_EVENT_UP);
	}
	else
	{
		if(gStopwatchAppInfo.numofCurrTimerRecords < MAX_TIMER_RECORDS)
		{	
			if(gStopwatchAppInfo.splitLapTimer==SPLIT_TIMER)
				ShowCategory307Screen(STR_STOPWATCH_STOP, 0,
									STR_STOPWATCH_SPLIT, IMG_GLOBAL_BACK,
									gStopwatchAppInfo.numofCurrTimerRecords,gCurrentRecords,0,guiBuffer,IMG_TYPICALSW_BACKGROUND,CONTINUE_TIMER);
			else
				ShowCategory307Screen(STR_STOPWATCH_STOP, 0,
									STR_STOPWATCH_LAP, IMG_GLOBAL_BACK,
									gStopwatchAppInfo.numofCurrTimerRecords,gCurrentRecords,0,guiBuffer,IMG_TYPICALSW_BACKGROUND,STOP_START_AGAIN);

			SetRightSoftkeyFunction(RecordSplitLapTime,KEY_EVENT_UP);
		}
		else
		{
			if(gStopwatchAppInfo.splitLapTimer==SPLIT_TIMER)
				ShowCategory307Screen(STR_STOPWATCH_STOP, 0,
									0, 0,
									gStopwatchAppInfo.numofCurrTimerRecords,gCurrentRecords,0,guiBuffer,IMG_TYPICALSW_BACKGROUND,CONTINUE_TIMER);
			else
				ShowCategory307Screen(STR_STOPWATCH_STOP, 0,
									0, 0,
									gStopwatchAppInfo.numofCurrTimerRecords,gCurrentRecords,0,guiBuffer,IMG_TYPICALSW_BACKGROUND,STOP_START_AGAIN);
		}
			
		SetLeftSoftkeyFunction(StopSplitLapTimer,KEY_EVENT_UP);
	}
}


/**************************************************************

	FUNCTION NAME		: ExitWtchScrSplitLapTimingStopwatch(void)

  	PURPOSE				: Exit fn for Split/Lap timing screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void ExitWtchScrSplitLapTimingStopwatch(void)
{
	 
    
	if(gStopwatchAppInfo.TimerState== TIMER_RUNNING || gStopwatchAppInfo.MPTimerState== TIMER_RUNNING)
	   ShowStatusIcon(STATUS_ICON_STOP_WATCH);
	else
	   HideStatusIcon(STATUS_ICON_STOP_WATCH);
	UpdateStatusIcons();
}

/**************************************************************

	FUNCTION NAME		: StartSplitLapTimer(void)

  	PURPOSE				: Start Split/lap Timer.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void StartSplitLapTimer(void)
{
	start_stop_watch_timer(TIMER_START);
	ChangeLeftSoftkey(STR_STOPWATCH_STOP, 0);
	if(gStopwatchAppInfo.splitLapTimer)
		ChangeRightSoftkey(STR_STOPWATCH_SPLIT, 0);
	else
		ChangeRightSoftkey(STR_STOPWATCH_LAP, 0);

	SetLeftSoftkeyFunction(StopSplitLapTimer,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(NULL,KEY_LONG_PRESS);	
	SetRightSoftkeyFunction(RecordSplitLapTime,KEY_EVENT_UP);
	gStopwatchAppInfo.TimerState=TIMER_RUNNING;

//	TurnOnBacklight(0);

}

/**************************************************************

	FUNCTION NAME		: StopSplitLapTimer(void)

  	PURPOSE				: Stop split/Lap timer.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void StopSplitLapTimer(void)
{
	start_stop_watch_timer(TIMER_STOP);
	ChangeLeftSoftkey(STR_STOPWATCH_RESUME, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
	SetLeftSoftkeyFunction(ResumeSplitLapTimer,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(ResetSplitLapTimer,KEY_LONG_PRESS);	
	SetRightSoftkeyFunction(ConfirmSaveTimerRecord,KEY_EVENT_UP);

//	TurnOnBacklight(1);

	gStopwatchAppInfo.TimerState=TIMER_STOPPED;

}

/**************************************************************

	FUNCTION NAME		: ResumeSplitLapTimer(void)

  	PURPOSE				: Resume current split/Lap timer.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void ResumeSplitLapTimer(void)
{
	start_stop_watch_timer(TIMER_START);
	ChangeLeftSoftkey(STR_STOPWATCH_STOP, 0);
	if(gStopwatchAppInfo.numofCurrTimerRecords >= MAX_TIMER_RECORDS)
		ChangeRightSoftkey(0,0);
	else
	{
		if(gStopwatchAppInfo.splitLapTimer)
			ChangeRightSoftkey(STR_STOPWATCH_SPLIT, 0);
		else
			ChangeRightSoftkey(STR_STOPWATCH_LAP, 0);
		SetRightSoftkeyFunction(RecordSplitLapTime,KEY_EVENT_UP);
	}

	SetLeftSoftkeyFunction(StopSplitLapTimer,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(NULL,KEY_LONG_PRESS);	

//	TurnOnBacklight(0);

	gStopwatchAppInfo.TimerState=TIMER_RUNNING;
}

/**************************************************************

	FUNCTION NAME		: ResetSplitLapTimer(void)

  	PURPOSE				: Reset the Split/Lap Timer when Resume is long 
						  pressed.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void ResetSplitLapTimer(void)
{
	reset_timer();

	ResetTypicalStopwatch();

	ChangeLeftSoftkey(STR_STOPWATCH_START, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
	SetLeftSoftkeyFunction(SetLSKforSplitLapTimerResumeLongPress,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(NULL,KEY_LONG_PRESS);	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/**************************************************************

	FUNCTION NAME		: SetLSKforSplitLapTimerResumeLongPress

  	PURPOSE				: Set LSK handler fn when Resume is long 
						  pressed to reset the Split timer..

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void SetLSKforSplitLapTimerResumeLongPress(void)
{
	SetLeftSoftkeyFunction(StartSplitLapTimer,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: RecordSplitLapTime

  	PURPOSE				: This function splits/Laps the Time..

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void RecordSplitLapTime(void)
{
	S32 mili,hr,min,sec;

	UpdateHistoryValue(&mili,&hr,&min,&sec);

	gCurrentRecords[gStopwatchAppInfo.numofCurrTimerRecords].hour=hr;
	gCurrentRecords[gStopwatchAppInfo.numofCurrTimerRecords].min=min;
	gCurrentRecords[gStopwatchAppInfo.numofCurrTimerRecords].sec=sec;
	gCurrentRecords[gStopwatchAppInfo.numofCurrTimerRecords].mili_sec=mili;

	gStopwatchAppInfo.numofCurrTimerRecords++;
	if(gStopwatchAppInfo.numofCurrTimerRecords>=MAX_TIMER_RECORDS)
		ChangeRightSoftkey(0,0);

	gStopwatchAppInfo.isRecordPresent=TRUE;

}


/**************************************************************

	FUNCTION NAME		: ConfirmSaveTimerRecord

  	PURPOSE				: Confirmation screen for saving the timer record.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void ConfirmSaveTimerRecord(void)
{
	if(gStopwatchAppInfo.isRecordPresent==TRUE)
	{
		DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
								STR_GLOBAL_NO, IMG_GLOBAL_NO,
								get_string(STR_GLOBAL_SAVE),IMG_GLOBAL_QUESTION, WARNING_TONE);
	
		/* 9.Register function with right softkey */
		SetRightSoftkeyFunction(GoBackToStopwatchMenu,KEY_EVENT_UP);

		if(gStopwatchAppInfo.numOfSavedTimerRecords < MAX_STOPWATCH_RECORDS)
		{
			SetLeftSoftkeyFunction(EntryWtchScrEnterRecordName,KEY_EVENT_UP);
		}
		else
		{
			SetLeftSoftkeyFunction(EntryWtchScrMemoryFull,KEY_EVENT_UP);
		}

	}
	else
    {
        ResetTypicalStopwatch();
		GoBackHistory();
    }
}

/**************************************************************

	FUNCTION NAME		: GoBackToStopwatchMenu

  	PURPOSE				: This fn takes user to StopwatchMenu screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void GoBackToStopwatchMenu(void)
{
	DeleteUptoScrID(SCR_TYPICAL_STOPWATCH);
    ResetTypicalStopwatch();
	GoBackHistory();
}
/**************************************************************

	FUNCTION NAME		: EntryWtchScrEnterRecordName

  	PURPOSE				: This function gets the record name.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
extern void Messages2GoBackHistory (void);
extern void GBSetInputboxLSKFunction(FuncPtr f);
void EntryWtchScrEnterRecordName(void)
{
	U8* guiBuffer;

	EntryNewScreen(SCR_ENTER_RECORD_NAME, ExitWtchScrRecordName, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ENTER_RECORD_NAME);

	ShowCategory5Screen(STR_STOPWATCH_SAVE_STOPWATCH, MAIN_MENU_TITLE_ORGANIZER_ICON,
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, 
						  (PU8)gStopwatchAppInfo.recordName,
						  (MAX_RECORD_NAME_LENGTH+1), guiBuffer);

	SetLeftSoftkeyFunction(EntryWtchRecordSaveOption,KEY_EVENT_UP);
	#if defined(__MMI_GB__)                                                              
		GBSetInputboxLSKFunction(EntryWtchRecordSaveOption);
       #endif
	RegisterInputMethodScreenCloseFunction (Messages2GoBackHistory);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: ExitWtchScrRecordName()

  	PURPOSE				: Exit function for Record Name Screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitWtchScrRecordName(void) 
{
	history_t   scr;
	scr.scrnID = SCR_ENTER_RECORD_NAME;
	scr.entryFuncPtr = EntryWtchScrEnterRecordName;
	pfnUnicodeStrcpy((S8*)scr.inputBuffer, ( S8*)gStopwatchAppInfo.recordName);
	GetCategoryHistory(scr.guiBuffer);
	AddHistory(scr);

}

/**************************************************************

	FUNCTION NAME		: EntryWtchRecordSaveOption

  	PURPOSE				: This function Displays the options for record save.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchRecordSaveOption(void)
{
	U8* guiBuffer = NULL;
	U16	itemList[2];
	U16 nItems;
	mmi_trace(1," EntryWtchRecordSaveOption");
	EntryNewScreen(SCR_SAVE_OPTIONS, NULL, EntryWtchRecordSaveOption, NULL);

	SetParentHandler(MENU_RECORD_SAVE_OPTIONS);

	guiBuffer = GetCurrGuiBuffer(SCR_SAVE_OPTIONS);

	nItems = GetNumOfChild(MENU_RECORD_SAVE_OPTIONS);
	GetSequenceStringIds(MENU_RECORD_SAVE_OPTIONS,itemList);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	
	ShowCategory52Screen(	STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_ORGANIZER_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, itemList,
							(PU16)gIndexIconsImageList, 0,
							0, 0, guiBuffer	);
	

}


/**************************************************************

	FUNCTION NAME		: GoBackHistoryFnFromInputMethod()

  	PURPOSE				: Fn to be register for go back history from Input method screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBackHistoryFnFromInputMethod(void)
{
	GoBacknHistory(1);
}


/**************************************************************

	FUNCTION NAME		: SaveTimerRecord()

  	PURPOSE				: Saves Timer Record in NVRAM.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SaveTimerRecord(void) 
{
	U8 index;
	S16 error ;

	if(pfnUnicodeStrlen((S8*)gStopwatchAppInfo.recordName)!=0)
	{

		memset(&gTimerRecordList[gStopwatchAppInfo.numOfSavedTimerRecords],0,sizeof(stopwatchRecord));
		memcpy(&gTimerRecordList[gStopwatchAppInfo.numOfSavedTimerRecords].name,gStopwatchAppInfo.recordName,MAX_RECORD_NAME_LENGTH*ENCODING_LENGTH);
		gTimerRecordList[gStopwatchAppInfo.numOfSavedTimerRecords].numOfRecords=(U8)gStopwatchAppInfo.numofCurrTimerRecords;
		for(index=0;index < gStopwatchAppInfo.numofCurrTimerRecords;index++)
		{
			gTimerRecordList[gStopwatchAppInfo.numOfSavedTimerRecords].nRecordData[index].recHr=(U8)gCurrentRecords[index].hour;
			gTimerRecordList[gStopwatchAppInfo.numOfSavedTimerRecords].nRecordData[index].recMin=(U8)gCurrentRecords[index].min;
			gTimerRecordList[gStopwatchAppInfo.numOfSavedTimerRecords].nRecordData[index].recSec=(U8)gCurrentRecords[index].sec;
			gTimerRecordList[gStopwatchAppInfo.numOfSavedTimerRecords].nRecordData[index].recMsec=(U8)gCurrentRecords[index].mili_sec;
		}
				
		gStopwatchAppInfo.numOfSavedTimerRecords++;

  		if(WriteValue( NVRAM_STOPWATCH_TOTAL_RECORDS, &gStopwatchAppInfo.numOfSavedTimerRecords, DS_BYTE , &error)<0)
		{
			mmi_trace (1,"TYPICAL STOPWATCH - Save Record : WriteValue to write total Number of records failed (error code %d)",(int) error);
		}

		if(WriteRecord (NVRAM_EF_STOPWATCH_LID, 1 , &gTimerRecordList[0],NVRAM_EF_STOPWATCH_RECORD_SIZE,&error)<0)
		{
			mmi_trace (1,"TYPICAL STOPWATCH - Save Record : WriteRecord to write Timer record failed (error code %d)",(int) error);
		}
	
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, POPUP_TIME, SUCCESS_TONE);
		ResetTypicalStopwatch();
		DeleteUptoScrID(SCR_TYPICAL_STOPWATCH);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_EMPTY_NAME), IMG_GLOBAL_EMPTY, 1, POPUP_TIME, ERROR_TONE);
		DeleteUptoScrID(SCR_ENTER_RECORD_NAME);
	}
}


/**************************************************************

	FUNCTION NAME		: FailedSaveTimerRecord()

  	PURPOSE				: Failed in saving timer record.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void FailedSaveTimerRecord(void)
{
	DisplayPopup((PU8)GetString(STR_STOPWATCH_NOT_SAVED),IMG_GLOBAL_WARNING,1, POPUP_TIME, WARNING_TONE);
	DeleteUptoScrID(SCR_TYPICAL_STOPWATCH);
	ResetTypicalStopwatch();
}

/**************************************************************

	FUNCTION NAME		: EntryWtchScrViewRecord(void)

  	PURPOSE				: Entry fn for view record list.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrViewRecord(void)
{
	U8* guiBuffer = NULL;
	U8 count =0;


	if ( gStopwatchAppInfo.numOfSavedTimerRecords > 0) 
	{
		for ( count=0;count < gStopwatchAppInfo.numOfSavedTimerRecords;count++)
		{
			pfnUnicodeStrcpy((S8*)subMenuDataPtrs[count],(S8*)gTimerRecordList[count].name);
		}
	}

	if ( gStopwatchAppInfo.numOfSavedTimerRecords == 0) 
	{
	    
		DisplayPopup((PU8)GetString( STR_GLOBAL_EMPTY /*STR_STOPWATCH_LIST_EMPTY*/), IMG_GLOBAL_EMPTY, 1, POPUP_TIME, EMPTY_LIST_TONE);
	}
	else 
	{
		guiBuffer = GetCurrGuiBuffer(SCR_VIEW_RECORD);

  	   	EntryNewScreen(SCR_VIEW_RECORD, NULL, EntryWtchScrViewRecord, NULL);

		SetParentHandler(MENU_VIEW_RECORD);

		RegisterHighlightHandler(GetSelectedItemIndex);
		/* Register function for left soft key */
		ShowCategory84Screen(STR_VIEW_RECORD, MAIN_MENU_TITLE_ORGANIZER_ICON,
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  gStopwatchAppInfo.numOfSavedTimerRecords,(U8**) subMenuDataPtrs,
						  (PU16)gIndexIconsImageList,0,
						  gStopwatchAppInfo.currHilitedRecord,guiBuffer);
		SetLeftSoftkeyFunction(EntryWtchScrViewRecordOptions,KEY_EVENT_UP);
			

		/* Register function for right soft key */
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	}


}


/**************************************************************

	FUNCTION NAME		: GetSelectedItemIndex(S32 HilitedIndex)

  	PURPOSE				: Function to store current hilited record in global variable.

	INPUT PARAMETERS	: S32, Current Hilite Index

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void GetSelectedItemIndex(S32 hilitedIndex)
{
	gStopwatchAppInfo.currHilitedRecord=hilitedIndex;
}

/**************************************************************

	FUNCTION NAME		: EntryWtchScrViewRecordOptions(void)

  	PURPOSE				: Entry fn for view record options list.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrViewRecordOptions(void)
{
	U16 nStrItemList[MAX_SUB_MENUS];/* Stores the strings id of submenus returned */
	U16 nNumofItem;		 /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_VIEW_RECORD_OPTIONS, NULL, EntryWtchScrViewRecordOptions, NULL);							

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_VIEW_RECORD_OPTIONS);	

	/* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_VIEW_RECORD_OPTIONS);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_VIEW_RECORD_OPTIONS	);
	
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_VIEW_RECORD_OPTIONS,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_VIEW_RECORD_OPTIONS);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_ORGANIZER_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  LIST_MENU, 0,guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10. Register Exit handler */
}



/**************************************************************

	FUNCTION NAME		: EntryWtchScrViewSavedRecord(void)

  	PURPOSE				: Entry fn To view saved record.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrViewSavedRecord(void)
{
	U8* guiBuffer = NULL;
	U8 currRecordNum =0;
	U8 *listItemsIcons[MAX_SUB_MENUS];
	nRecordDetail *localRecord;
	U8 localRecordString[(MAX_TIMER_RECORD_DIGITS+1)*ENCODING_LENGTH];

	EntryNewScreen(SCR_VIEW_SAVED_RECORD, NULL, EntryWtchScrViewSavedRecord, NULL);

	for (currRecordNum=0;currRecordNum<gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].numOfRecords;currRecordNum++)
	{
		memset(localRecordString,0,(MAX_TIMER_RECORD_DIGITS+1)*ENCODING_LENGTH);
		localRecord=&gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].nRecordData[currRecordNum];
		change_stopwatchtime_to_string((S32)localRecord->recMsec,(S32)localRecord->recHr,(S32)localRecord->recMin,(S32)localRecord->recSec,&localRecordString[0],DISPLAY_MILISEC_TRUE);
		pfnUnicodeStrcpy((S8*)subMenuDataPtrs[currRecordNum],(S8*)localRecordString);
		listItemsIcons[currRecordNum]=(U8*)GetImage(gIndexIconsImageList[currRecordNum]);
	}

	guiBuffer = GetCurrGuiBuffer(SCR_VIEW_SAVED_RECORD);

	SetParentHandler(MENU_OPTION_VR_VIEW);

//qiff replace this category for common list 
#if 0
	/* Register function for left soft key */
	ShowCategory32Screen((U8*)gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].name,NULL,
							NULL,NULL,
							(PU8)GetString(STR_GLOBAL_BACK),(PU8)GetImage(IMG_GLOBAL_BACK),
							currRecordNum,(U8**)subMenuDataPtrs,
							listItemsIcons,
							0,0,
							guiBuffer);
#else
	ShowCategory174Screen((U8*)gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].name	, 
							(U8*)GetImage(GetRootTitleIcon(MENU_STOPWATCH)),
							NULL,NULL,
							(PU8)GetString(STR_GLOBAL_BACK),(PU8)GetImage(IMG_GLOBAL_BACK),
							currRecordNum,(U8**)subMenuDataPtrs,listItemsIcons,NULL,
							0,0,guiBuffer);
#endif
	/* Register function for right soft key */
	DeleteUptoScrID(SCR_VIEW_RECORD);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);


}


/**************************************************************

	FUNCTION NAME		: EntryWtchScrDeleteRecord(void)

  	PURPOSE				: Entry fn To Delete record.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrDeleteRecord(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_GLOBAL_DELETE),IMG_GLOBAL_QUESTION, WARNING_TONE);
	/* 9.Register function with right softkey */
	DeleteUptoScrID(SCR_VIEW_RECORD);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(DeleteSelectedRecord,KEY_EVENT_UP);

}

/**************************************************************

	FUNCTION NAME		: DeleteSelectedRecord(void)

  	PURPOSE				: This function deletes curently selected record.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void DeleteSelectedRecord(void)
{
	U8 count;
	S16 error ;
	
	memset(&gTimerRecordList[gStopwatchAppInfo.currHilitedRecord],0,sizeof(stopwatchRecord));

	for(count=(U8)gStopwatchAppInfo.currHilitedRecord; count+1 < gStopwatchAppInfo.numOfSavedTimerRecords;count++)
	{
		memcpy(&gTimerRecordList[count],&gTimerRecordList[count+1],sizeof(stopwatchRecord));
	}
	memset(&gTimerRecordList[--gStopwatchAppInfo.numOfSavedTimerRecords],0,sizeof(stopwatchRecord));

 	if(WriteValue( NVRAM_STOPWATCH_TOTAL_RECORDS, &gStopwatchAppInfo.numOfSavedTimerRecords, DS_BYTE , &error)<0)
	{
		PRINT_INFORMATION (("TYPICAL STOPWATCH - Delete Record : WriteValue to write total Number of records failed (error code %d)",(int) error));
	}
	
	if(WriteRecord (NVRAM_EF_STOPWATCH_LID, 1 , &gTimerRecordList[0],NVRAM_EF_STOPWATCH_RECORD_SIZE,&error)<0)
	{
		PRINT_INFORMATION (("TYPICAL STOPWATCH - Delete Record : WriteRecord to write Timer record failed (error code %d)",(int) error));
	}
	
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, POPUP_TIME, SUCCESS_TONE);
	if(gStopwatchAppInfo.numOfSavedTimerRecords==0)
		DeleteUptoScrID(SCR_TYPICAL_STOPWATCH);
	else
		DeleteUptoScrID(SCR_VIEW_RECORD);


}

/**************************************************************

	FUNCTION NAME		: EntryWtchScrDeleteAllRecord(void)

  	PURPOSE				: Entry fn To confirm Delete all record.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrDeleteAllRecord(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_GLOBAL_DELETE_ALL )/*STR_CONFIRM_DELETE_ALL_RECORD*/,IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* 9.Register function with right softkey */
	DeleteUptoScrID(SCR_VIEW_RECORD);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(DeleteAllTimerRecord,KEY_EVENT_UP);

}

/**************************************************************

	FUNCTION NAME		: DeleteAllTimerRecord(void)

  	PURPOSE				: This function deletes all Saved Timer Records.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void DeleteAllTimerRecord(void)
{
	S16 error ;

	memset(gTimerRecordList,0,sizeof(stopwatchRecord)*MAX_STOPWATCH_RECORDS);
	gStopwatchAppInfo.numOfSavedTimerRecords=0;
 
	if(WriteValue( NVRAM_STOPWATCH_TOTAL_RECORDS, &gStopwatchAppInfo.numOfSavedTimerRecords, DS_BYTE , &error)<0)
	{
		PRINT_INFORMATION (("TYPICAL STOPWATCH - Delete All Record: WriteValue to write total Number of records failed (error code %d)",(int) error));
	}
	
	if(WriteRecord (NVRAM_EF_STOPWATCH_LID, 1 , &gTimerRecordList[0],NVRAM_EF_STOPWATCH_RECORD_SIZE,&error)<0)	
	{
		PRINT_INFORMATION (("TYPICAL STOPWATCH - Delete All Record : WriteRecord to write Timer record failed (error code %d)",(int) error));

	}

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, POPUP_TIME, SUCCESS_TONE);
	DeleteUptoScrID(SCR_TYPICAL_STOPWATCH);

}

/**************************************************************

	FUNCTION NAME		: EntryWtchScrMemoryFull()

  	PURPOSE				: Displays the screen for memory full

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrMemoryFull(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_STOPWATCH_MEMORY_FULL),IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(FailedSaveTimerRecord,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryWtchScrReplaceRecord,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: EntryWtchScrReplaceRecord(void)

  	PURPOSE				: Entry fn To replace previous record when memory is full

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrReplaceRecord(void)
{
	U8* guiBuffer = NULL;
	U8 count =0;

	EntryNewScreen(SCR_REPLACE_RECORD, NULL, EntryWtchScrReplaceRecord, NULL);

	for ( count=0;count < gStopwatchAppInfo.numOfSavedTimerRecords;count++)
	{
		pfnUnicodeStrcpy((S8*)subMenuDataPtrs[count],(S8*)gTimerRecordList[count].name);
	}
	
	guiBuffer = GetCurrGuiBuffer(SCR_REPLACE_RECORD);

	gStopwatchAppInfo.replaceRecord=TRUE;

	RegisterHighlightHandler(GetSelectedItemIndex);

	/* Register function for left soft key */
	ShowCategory84Screen(STR_STOPWATCH_REPLACE_RECORD, MAIN_MENU_TITLE_ORGANIZER_ICON,
					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
					  STR_GLOBAL_CANCEL, 0,
					  gStopwatchAppInfo.numOfSavedTimerRecords,(U8**) subMenuDataPtrs,
					  (PU16)gIndexIconsImageList,0,
					  0,guiBuffer);

	SetLeftSoftkeyFunction(EntryWtchScrEnterRecordName,KEY_EVENT_UP);
	SetRightSoftkeyFunction(FailedSaveTimerRecord,KEY_EVENT_UP);

}



/**************************************************************

	FUNCTION NAME		: ReplaceSelectedRecord(void)

  	PURPOSE				: Replace Existing Record.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void ReplaceSelectedRecord(void)
{
	U8 count;
	S16 error ;
	
	if(pfnUnicodeStrlen((S8*)gStopwatchAppInfo.recordName)!=0)
	{
		gStopwatchAppInfo.replaceRecord=FALSE;

		memset(&gTimerRecordList[gStopwatchAppInfo.currHilitedRecord],0,sizeof(stopwatchRecord));
		memcpy(&gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].name,gStopwatchAppInfo.recordName,MAX_RECORD_NAME_LENGTH*ENCODING_LENGTH);
		gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].numOfRecords=(U8)gStopwatchAppInfo.numofCurrTimerRecords;

		for(count=0;count < gStopwatchAppInfo.numofCurrTimerRecords;count++)
		{
			gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].nRecordData[count].recHr=(U8)gCurrentRecords[count].hour;
			gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].nRecordData[count].recMin=(U8)gCurrentRecords[count].min;
			gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].nRecordData[count].recSec=(U8)gCurrentRecords[count].sec;
			gTimerRecordList[gStopwatchAppInfo.currHilitedRecord].nRecordData[count].recMsec=(U8)gCurrentRecords[count].mili_sec;
		}

	 	if(WriteValue( NVRAM_STOPWATCH_TOTAL_RECORDS, &gStopwatchAppInfo.numOfSavedTimerRecords, DS_BYTE , &error)<0)
		{
			PRINT_INFORMATION (("TYPICAL STOPWATCH - Replace Record: WriteValue to write total Number of records failed (error code %d)",(int) error));
		}

		if(WriteRecord (NVRAM_EF_STOPWATCH_LID, 1 , &gTimerRecordList[0],NVRAM_EF_STOPWATCH_RECORD_SIZE,&error) < 0)
		{
			PRINT_INFORMATION (("TYPICAL STOPWATCH - Replace Record : WriteRecord to write Timer record failed (error code %d)",(int) error));
		}

		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, POPUP_TIME, SUCCESS_TONE);
		DeleteUptoScrID(SCR_TYPICAL_STOPWATCH);
		ResetTypicalStopwatch();
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_EMPTY_NAME), IMG_GLOBAL_EMPTY, 1, POPUP_TIME, ERROR_TONE);
		DeleteUptoScrID(SCR_ENTER_RECORD_NAME);
	}
}


/********************For Stopwatch running in background***************************/
/**************************************************************

	FUNCTION NAME		: DisplayTimerRunningPopup(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void DisplayDiscontinueTimerQuery(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_DISCONTINUE_TIMER_QUERY),IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(DiscontinuePreviousStopwatch,KEY_EVENT_UP);
}
/**************************************************************

	FUNCTION NAME		: DiscontinuePreviousStopwatch(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void DiscontinuePreviousStopwatch(void)
{
	ResetTypicalStopwatch();

	if(gStopwatchAppInfo.splitLapTimer==LAP_TIMER)
		gStopwatchAppInfo.splitLapTimer=SPLIT_TIMER;
	else
		gStopwatchAppInfo.splitLapTimer=LAP_TIMER;

	EntryWtchScrSplitLapTimingStopwatch();
}
/**************************************************************

	FUNCTION NAME		: ResetTypicalStopwatch(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void ResetTypicalStopwatch(void)
{
	gStopwatchAppInfo.replaceRecord=FALSE;
	gStopwatchAppInfo.isRecordPresent=FALSE;
	gStopwatchAppInfo.numofCurrTimerRecords=0;
	gStopwatchAppInfo.TimerState=TIMER_NOT_STARTED;
	memset(gCurrentRecords,0,sizeof(UI_stop_watch)*MAX_TIMER_RECORDS);
	initialize_stop_watch_timer();
}

#endif /* __MMI_STOPWATCH__ */


