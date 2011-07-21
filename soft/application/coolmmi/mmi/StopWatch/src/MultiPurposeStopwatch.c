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
 * MultiPurposeStopwatch.c
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

	FILENAME	: MultiPurposeStopwatch.c

  	PURPOSE		: Multi Purpose Stopwatch application implementation

 

 

	DATE		: 05/01/2004

**************************************************************/
#include "mmi_features.h"
#ifdef __MMI_STOPWATCH__

/***********  Include: MMI header file **************/
#include "mmi_data_types.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "wgui_categories.h"
#include "wgui_categories_stopwatch.h"
#include "globaldefs.h"
#include "hardwareevents.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "mainmenudef.h"
#include "profilegprots.h"

#include "stopwatchdef.h"
#include "stopwatchgprot.h"
#include "stopwatchprot.h"
#include "stopwatchgexdcl.h"
//#include "wallpaper.h"
#include "gpioinc.h"
#include "wgui_status_icons.h"

/***************************************************************************** 
* Define
*****************************************************************************/

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
void StarKeyReset(void);


/**********************************************************************
*****************************************************************************/

/**************************************************************

	FUNCTION NAME		: InitMultipurposeStopwatch(void)

  	PURPOSE				: Init fn for Multipurpose stop watch.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void InitMultipurposeStopwatch(void)
{
	U8 index;

	memset(&gLastMPTimeRecord,0,sizeof(UI_stop_watch));
	memset(gMultiPurposeTimerList,0,sizeof(UI_stop_watch)*MAX_MULTIPURPOSE_TIMER);
	for(index=0;index < MAX_MULTIPURPOSE_TIMER;index++)
	{
		gMultiPurposeTimerList[index].normalimagedata = GetImage( (U16)(IMG_MULTIPURPOSE_TIMER1 + index) );
		gMultiPurposeTimerList[index].highlightimagedata = GetImage( (U16)( IMG_MULTIPURPOSE_TIMER1 + index) );
	}

	gMPTimerInit.assignedKey[0]=KEY_LEFT_ARROW;
	gMPTimerInit.timerIndex[0]=0;

	gMPTimerInit.assignedKey[1]=KEY_UP_ARROW;
	gMPTimerInit.timerIndex[1]=1;

	gMPTimerInit.assignedKey[2]=KEY_RIGHT_ARROW;
	gMPTimerInit.timerIndex[2]=2;

	gMPTimerInit.assignedKey[3]=KEY_DOWN_ARROW;
	gMPTimerInit.timerIndex[3]=3;

}

/**************************************************************

	FUNCTION NAME		: EntryWtchScrMultipurposeStopwatch(void)

  	PURPOSE				: Entry fn for Multipurpose stop watch.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryWtchScrMultipurposeStopwatch(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_MULTIPURPOSE_STOPWATCH, ExitWtchScrMultiPurposeStopwatch, EntryWtchScrMultipurposeStopwatch, NULL);

	HideStatusIcon(STATUS_ICON_STOP_WATCH);
	UpdateStatusIcons();

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_MULTIPURPOSE_STOPWATCH);	

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_MULTIPURPOSEWATCH);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_MULTIPURPOSEWATCH);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_MULTIPURPOSEWATCH,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_MULTIPURPOSEWATCH);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
		
	/* 8 Display Category Screen */
	if(gStopwatchAppInfo.MPTimerState==TIMER_NOT_STARTED)
        {
		ShowCategory306Screen(&gLastMPTimeRecord,
						  GetRootTitleIcon(MENU_MULTIPURPOSEWATCH),STR_MUTIPURPOSE_TITLE_SIDE_MSG,
						  STR_STOPWATCH_START,0,STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_MULTIPURPOSE_TIMER,gMultiPurposeTimerList,0,
						  STR_MUTIPURPOSE_TITLE_MSG,guiBuffer,gStopwatchAppInfo.currentHiliteTimer);

//		SetLeftSoftkeyFunction(ResetAllTimer,KEY_LONG_PRESS);	
		SetKeyHandler(StarKeyReset,KEY_STAR,KEY_EVENT_UP);	
		SetLeftSoftkeyFunction(StartIndexTimer,KEY_EVENT_UP);
        } 
	else if(gStopwatchAppInfo.MPTimerState==TIMER_STOPPED)
	{
		ShowCategory306Screen(&gLastMPTimeRecord,
						  0,STR_MUTIPURPOSE_TITLE_SIDE_MSG,
						  STR_STOPWATCH_RESUME,0,STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_MULTIPURPOSE_TIMER,gMultiPurposeTimerList,0,
						  STR_MUTIPURPOSE_TITLE_MSG,guiBuffer,gStopwatchAppInfo.currentHiliteTimer);

	SetKeyHandler(StarKeyReset,KEY_STAR,KEY_EVENT_UP);	

//	SetLeftSoftkeyFunction(ResetAllTimer,KEY_LONG_PRESS);	
	SetLeftSoftkeyFunction(StartIndexTimer,KEY_EVENT_UP);
	}
	else
        {
		ShowCategory306Screen(&gLastMPTimeRecord,
						  0,STR_MUTIPURPOSE_TITLE_SIDE_MSG,
						  STR_STOPWATCH_STOP,0,STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_MULTIPURPOSE_TIMER,gMultiPurposeTimerList,0,
						  STR_MUTIPURPOSE_TITLE_MSG,guiBuffer,gStopwatchAppInfo.currentHiliteTimer);
		SetLeftSoftkeyFunction(StopCurrentTimer,KEY_EVENT_UP);
//		SetLeftSoftkeyFunction(0,KEY_LONG_PRESS);
        }

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(StarKeyReset,KEY_STAR,KEY_EVENT_UP);	

    
   /* If using EVENT_UP as action key event, when pressing left key entering the nWay Stopwatch at the previous menu */
   /* The key up event will trigger the related stopwatch start */
	SetGroupKeyHandler(StartKeyTimer,gMPTimerInit.assignedKey,MAX_MULTIPURPOSE_TIMER,KEY_EVENT_DOWN);
	
	//SetExitHandler(SCR_MULTIPURPOSE_STOPWATCH,ExitWtchScrMultiPurposeStopwatch);

}

/**************************************************************

	FUNCTION NAME		: ExitWtchScrMultiPurposeStopwatch(void)

  	PURPOSE				: Exit fn for Multipurpose Stopwatch screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void ExitWtchScrMultiPurposeStopwatch(void)
{
	GetMultipurposeTimeValues(/*&gLastMPTimeRecord,gMultiPurposeTimerList,*/&gStopwatchAppInfo.currentHiliteTimer);

	if(gStopwatchAppInfo.MPTimerState== TIMER_RUNNING || gStopwatchAppInfo.TimerState== TIMER_RUNNING)
		ShowStatusIcon(STATUS_ICON_STOP_WATCH);
	else
		HideStatusIcon(STATUS_ICON_STOP_WATCH);
	UpdateStatusIcons();
}

/**************************************************************

	FUNCTION NAME		: StartKeyTimer(void)

  	PURPOSE				: Start timer associated with any Key.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void StartKeyTimer(void)
{
	U8 count=0;
	U16 keycode,keytype;

	GetkeyInfo(&keycode,&keytype);

	for(count=0;count<MAX_MULTIPURPOSE_TIMER;count++) 	
	{
		if(keycode==gMPTimerInit.assignedKey[count])
		{
            gStopwatchAppInfo.currentTimerIndex=gMPTimerInit.timerIndex[count];
			StartIndexTimer();
			break;
		}
	}
}
/**************************************************************

	FUNCTION NAME		: StopCurrentTimer(void)

  	PURPOSE				: Stop Currently active timer.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void StopCurrentTimer(void)
{
	ChangeLeftSoftkey(STR_STOPWATCH_RESUME, 0);
	SetLeftSoftkeyFunction(StartIndexTimer,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(ResetAllTimer,KEY_LONG_PRESS);	
	stop_multipurpose_timers();

//	TurnOnBacklight(1);

	gStopwatchAppInfo.MPTimerState=TIMER_STOPPED;

}

/**************************************************************

	FUNCTION NAME		: ResetAllTimer(void)

  	PURPOSE				: Reset All Timer when Resume is long 
						  pressed.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void ResetAllTimer(void)
{
	ChangeLeftSoftkey(STR_STOPWATCH_START, 0);
//qiff modify for bug:10641
	SetLeftSoftkeyFunction(SetLSKMultiPurposeTimerResumeLongPress,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(0,KEY_LONG_PRESS);	
//	SetLeftSoftkeyFunction(StartIndexTimer,KEY_EVENT_UP);
	reset_all_multipurpose_time();
	gStopwatchAppInfo.currentTimerIndex=0;
	gStopwatchAppInfo.currentHiliteTimer=0;
	gStopwatchAppInfo.MPTimerState=TIMER_NOT_STARTED;

}

/**************************************************************

	FUNCTION NAME		: SetLSKMultiPurposeTimerResumeLongPress

  	PURPOSE				: Set LSK handler fn when Resume is long 
						  pressed to reset the Multipurpose timer.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void SetLSKMultiPurposeTimerResumeLongPress(void)
{
	gStopwatchAppInfo.currentTimerIndex=0;
	SetLeftSoftkeyFunction(StartIndexTimer,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: StartIndexTimer

  	PURPOSE				: Call back fn to start Timer set in gCurrentTimerIndex.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void StartIndexTimer(void)
{
	gStopwatchAppInfo.MPTimerState=TIMER_RUNNING;
	ChangeLeftSoftkey(STR_STOPWATCH_STOP, 0);
	SetLeftSoftkeyFunction(StopCurrentTimer,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(0,KEY_LONG_PRESS);	
	start_multipurpose_list_watch(gStopwatchAppInfo.currentTimerIndex);

//	TurnOnBacklight(0);

}
void StarKeyReset(void)
{
	ResetAllTimer();
	SetLeftSoftkeyFunction(StartIndexTimer,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(0,KEY_LONG_PRESS);	
}

#endif

