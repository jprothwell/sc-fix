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
 *	HalethMenstrual.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements Menstrual application.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MMI_HEALTHMENSTRUAL_C
#define _MMI_HEALTHMENSTRUAL_C
#include "mmi_features.h"
#ifdef __MMI_MENSTRUAL__
#include "healthmenstural.h"
#include "gui_data_types.h"
#include "datetimegprot.h"
#include "organizerdef.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_calendar.h"
#include "gui.h"
#include "globaldefs.h"
#include "historygprot.h"
#include "unicodexdcl.h"
#include "calendardef.h"
#include "commonscreens.h"

/* 
** Define
*/
#define	MENS_ZERO_PROB_PERIOD		3
#define	MENS_OVULATION_DAYS			2
#define	MENS_PERIOD					5

#define	MENS_DAY_LEN			3*ENCODING_LENGTH
#define	MENS_MON_LEN			3*ENCODING_LENGTH
#define	MENS_YEAR_LEN			5*ENCODING_LENGTH
#define	MENS_PERIOD_LEN			3*ENCODING_LENGTH
#define	MENS_TITLE_LEN			9*ENCODING_LENGTH

#define	MENS_MAX_PERIOD			99
#define	MENS_MIN_PERIOD			14
#define	MENS_NUM_PREV_MON		3	/* maximal prediction month */
#define	MENS_MAX_NAVI_MON		6	/* maximal navigation month */

/* 
** Typedef 
*/
typedef struct
{
	MYTIME	InputDate;
	S8	DayBuf[MENS_DAY_LEN];
	S8	MonBuf[MENS_MON_LEN];
	S8	YearBuf[MENS_YEAR_LEN];
	S8	PeriodBuf[MENS_PERIOD_LEN];
	S8	TitleString[MENS_TITLE_LEN]; 
	U16	CurrYear;
	U8	CurrMonth;	
	U8	CurrDay;
	U8	NumOfMon;
	U8	AvgPeriod;
	U8	IsAddToHistory;	
}mens_context_struct;


/* 
** Local Variable
*/

/* 
** Local Function
*/

/* 
** Global Variable
*/
mens_context_struct *g_mens_cntx = NULL;
extern wgui_inline_item wgui_inline_items[];

extern BOOL ClndPopupFlag; 

/* 
** Global Function
*/
extern void ClndrDateEditorCallBack(U8 *DateStr, U8 *day,U8 *month,U8 *year);

#define MENS_INITIALIZE
/*****************************************************************************
* FUNCTION
*	MensInit
* DESCRIPTION
*   Initialize mens app.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensInit(void)
{
	SetHiliteHandler(ORGANIZER_HEALTH_MENU_MENSTRUAL,HighlightMensApp);
}


/*****************************************************************************
* FUNCTION
*	MensDeInit
* DESCRIPTION
*   Free allocated memory for global context.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensDeInit(void)
{	
	if(g_mens_cntx)
	{
		OslMfree(g_mens_cntx);
		g_mens_cntx = NULL;
	}
}

/*****************************************************************************
* FUNCTION
*	HighlightMensApp
* DESCRIPTION
*   Highlight handler of Menstrual menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightMensApp(void) 
{
#if 0
	SetLeftSoftkeyFunction(EntryMensWelcome,KEY_EVENT_UP);
	SetKeyHandler(EntryMensWelcome,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(MensPreEntryInput,KEY_EVENT_UP);
    	SetKeyHandler(MensPreEntryInput,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif
}

/*****************************************************************************
* FUNCTION
*	EntryMensWelcome
* DESCRIPTION
*   Display welcome screen of mens application.
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryMensWelcome(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	EntryNewScreen(SCR_ID_MENS_WELCOME, NULL, EntryMensWelcome, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_MENS_WELCOME);	

	ShowCategory129Screen((U8*)get_string(STR_SCR_HEALTH_MENSIS_CAPTION), GetRootTitleIcon(ORGANIZER_HEALTH_MENU),
	                    STR_GLOBAL_OK, 0,	
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						IMG_SCR_HEALTH_MENSIS, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(MensPreEntryInput,KEY_EVENT_UP);
#ifdef __MMI_NOKIA_STYLE_N800__
        ClearKeyHandler( KEY_LSK,  KEY_EVENT_DOWN);
        ClearKeyHandler( KEY_RSK,  KEY_EVENT_DOWN);

        SetKeyHandler(MensPreEntryInput , KEY_LSK,  KEY_EVENT_UP);
        SetKeyHandler(GoBackHistory , KEY_RSK,  KEY_EVENT_UP);

#endif
}


#define MENS_INPUT_SCREEN
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

void MensFillInlineStruct(void)
{
	SetInlineItemActivation(&wgui_inline_items[MENS_INLINE_DATE_CAP],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[MENS_INLINE_DATE_CAP],(U8*) GetString(STR_LAST_DATE));

	SetInlineItemActivation(&wgui_inline_items[MEMS_INLINE_DATE],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemDate(&wgui_inline_items[MEMS_INLINE_DATE],(U8*)g_mens_cntx->DayBuf,(U8*)g_mens_cntx->MonBuf,(U8*)g_mens_cntx->YearBuf,ClndrDateEditorCallBack);
	RightJustifyInlineItem(&wgui_inline_items[MEMS_INLINE_DATE]);
	EnableInlineItemBoundary(&wgui_inline_items[MEMS_INLINE_DATE]);
	set_inline_date_boundary(2030, 1970, 12, 1);

	SetInlineItemActivation(&wgui_inline_items[MEMS_INLINE_PERIOD_CAP],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[MEMS_INLINE_PERIOD_CAP],(U8*)GetString(STR_AVERAGE_DAYS_HEALTH_MENSIS));
	
	SetInlineItemActivation(&wgui_inline_items[MEMS_INLINE_PERIOD],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[MEMS_INLINE_PERIOD], (U8*)g_mens_cntx->PeriodBuf, 3,INPUT_TYPE_NUMERIC);

	RightJustifyInlineItem(&wgui_inline_items[MEMS_INLINE_PERIOD]);
	EnableInlineItemBoundary(&wgui_inline_items[MEMS_INLINE_PERIOD]);

	SetInlineItemActivation(&wgui_inline_items[MEMS_INLINE_NUM_DAYS],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[MEMS_INLINE_NUM_DAYS],(U8*)GetString(STR_DATE_FORMAT_HEALTH_MENSIS));

}
//add by liuxn 060616 start
#else
void MensFillInlineStruct(void)
{
//add by zough 061121 start
	SetInlineItemActivation(&wgui_inline_items[MENS_INLINE_DATE_CAP], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[MENS_INLINE_DATE_CAP], (U8*) GetString(STR_LAST_DATE));
	SetInlineItemActivation(&wgui_inline_items[MEMS_INLINE_DATE], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemDate(&wgui_inline_items[MEMS_INLINE_DATE], (U8*)g_mens_cntx->DayBuf, (U8*)g_mens_cntx->MonBuf, (U8*)g_mens_cntx->YearBuf, ClndrDateEditorCallBack);
	RightJustifyInlineItem(&wgui_inline_items[MEMS_INLINE_DATE]);
	EnableInlineItemBoundary(&wgui_inline_items[MEMS_INLINE_DATE]);
	set_inline_date_boundary(2030, 1970, 12, 1);
	SetInlineItemActivation(&wgui_inline_items[MEMS_INLINE_PERIOD_CAP], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[MEMS_INLINE_PERIOD_CAP], (U8*)GetString(STR_AVERAGE_DAYS_HEALTH_MENSIS));
	SetInlineItemActivation(&wgui_inline_items[MEMS_INLINE_PERIOD], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[MEMS_INLINE_PERIOD], (U8*)g_mens_cntx->PeriodBuf, 3, INPUT_TYPE_NUMERIC);
	RightJustifyInlineItem(&wgui_inline_items[MEMS_INLINE_PERIOD]);
	EnableInlineItemBoundary(&wgui_inline_items[MEMS_INLINE_PERIOD]);
	SetInlineItemActivation(&wgui_inline_items[MEMS_INLINE_NUM_DAYS], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[MEMS_INLINE_NUM_DAYS], (U8*)GetString(STR_DATE_FORMAT_HEALTH_MENSIS));
//add by zough 061121 end	
}
#endif
//add by liuxn 060616 end


/*****************************************************************************
* FUNCTION
*	MensPreEntryInput
* DESCRIPTION
*  	Initialize variables for input screen and then go to input screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensPreEntryInput(void)
{
	MYTIME currTime;
	S8 tmpMon[MENS_MON_LEN];
	S8 tmpDay[MENS_MON_LEN];

	if(g_mens_cntx == NULL)
		g_mens_cntx = OslMalloc(sizeof(mens_context_struct));

	memset(g_mens_cntx, 0, sizeof(mens_context_struct));
	
	GetDateTime(&currTime);

	coolsand_UI_itoa((S32)currTime.nYear, (U16*)g_mens_cntx->YearBuf, 10);

	sprintf(tmpMon,"%02d",currTime.nMonth);
	AnsiiToUnicodeString(g_mens_cntx->MonBuf, tmpMon);
		
	sprintf(tmpDay,"%02d",currTime.nDay);
	AnsiiToUnicodeString(g_mens_cntx->DayBuf, tmpDay);

	EntryMensInput();
}


/*****************************************************************************
* FUNCTION
*	MensFreeMemAndGoBack
* DESCRIPTION
*  	Free memory of global context and go back history.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensFreeMemAndGoBack(void)
{
	MensDeInit();
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*	EntryMensInput
* DESCRIPTION
*  	Display screen to input
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryMensInput(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	U8* inputBuffer;
	U16 inputBufferSize;                //added for inline edit history
	    
	EntryNewScreen(SCR_ID_MENS_INPUT, ExitMensInput, NULL, NULL);

	InitializeCategory57Screen();

	MensFillInlineStruct();
	RegisterHighlightHandler(HighlightMensInputItem);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_MENS_INPUT);
	
	inputBuffer = GetCurrNInputBuffer(SCR_ID_MENS_INPUT, &inputBufferSize); 
	
	if(inputBuffer!=NULL)                                            //added for inline edit history
		SetCategory57Data(wgui_inline_items, MEMS_INLINE_TOTAL, inputBuffer);//sets the data 
 		
	DisableCategory57ScreenDone();

	ShowCategory57Screen(STR_HEALTH_MENSIS_DATE_CAPTION,GetRootTitleIcon(ORGANIZER_HEALTH_MENU),
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						MEMS_INLINE_TOTAL,NULL,wgui_inline_items,1,guiBuffer);
	
	SetKeyHandler(MensDisplayResult,KEY_LSK, KEY_EVENT_UP);

	SetCategory57RightSoftkeyFunctions(MensDisplayResult,MensFreeMemAndGoBack);
}


/*****************************************************************************
* FUNCTION
*	ExitMensInput
* DESCRIPTION
*  	Exit function of input screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitMensInput(void)
{
	history_t   Scr; 
	U16 inputBufferSize;                //added for inline edit history
 
	S16 nHistory =0;
	Scr.scrnID = SCR_ID_MENS_INPUT;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntryMensInput;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	GetCategory57Data ((U8*)Scr.inputBuffer);         //added for inline edit history
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	AddNHistory(Scr, inputBufferSize);                       //added for inline edit history

}


/*****************************************************************************
* FUNCTION
*	HighlightMensInputItem
* DESCRIPTION
*  	Highlight handler of inline item in input screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightMensInputItem(S32 nIndex)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(MensDisplayResult);
}

#define MENS_RESULT_SCREEN
/*****************************************************************************
* FUNCTION
*	MensDisplayResult
* DESCRIPTION
*  	Display corresponding screen according to computation result.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensDisplayResult(void)
{
	S8 error;

	CloseCategory57Screen();
	error = MensValidateInput();

	switch(error)
	{
		case MENS_ERR_DATE:
			DisplayPopup((U8*)GetString(STR_HEALTH_WRONG_DATE_MSG),IMG_GLOBAL_WARNING,0,UI_POPUP_NOTIFYDURATION_TIME,(U8)WARNING_TONE);
			break;
		case MENS_ERR_PERIOD:
			DisplayPopup((U8*)GetString(STR_INVALID_AVERAGE_DAY_MESSAGE),IMG_GLOBAL_WARNING,0,UI_POPUP_NOTIFYDURATION_TIME,(U8)WARNING_TONE);
			break;
		default:	// correct 
			MensPreEntryResult();
			break;
	}
}


/*****************************************************************************
* FUNCTION
*	MensDisplayResult
* DESCRIPTION
*  	Validate the correctness of user input.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U8 MensValidateInput(void)
{
	S8 tmpMon[MENS_MON_LEN];
	S8 tmpDay[MENS_DAY_LEN];
	S8 tmpYear[MENS_YEAR_LEN];
	S8 AvgDays[MENS_PERIOD_LEN];
	MYTIME currTime, incTime, finalTime;
	U8 tmpPeriod;
	U8 DayOfMon;

	/* validate period */
	UnicodeToAnsii(AvgDays, g_mens_cntx->PeriodBuf);
	g_mens_cntx->AvgPeriod = tmpPeriod = (U8)atoi(AvgDays);		
	if(tmpPeriod <MENS_MIN_PERIOD || tmpPeriod >MENS_MAX_PERIOD)
		return MENS_ERR_PERIOD;

	/* validate day */
	UnicodeToAnsii(tmpDay, g_mens_cntx->DayBuf);
	UnicodeToAnsii(tmpMon, g_mens_cntx->MonBuf);
	UnicodeToAnsii(tmpYear, g_mens_cntx->YearBuf);

	memset(&g_mens_cntx->InputDate, 0, sizeof(MYTIME));
	g_mens_cntx->InputDate.nDay = atoi(tmpDay);
	g_mens_cntx->InputDate.nMonth = atoi(tmpMon);
	g_mens_cntx->InputDate.nYear = atoi(tmpYear);

	/* daty out of boundary */
	DayOfMon = LastDayOfMonth(g_mens_cntx->InputDate.nMonth, g_mens_cntx->InputDate.nYear);
	if(g_mens_cntx->InputDate.nDay > DayOfMon)
		return MENS_ERR_DATE;

	/* date too large */
	GetDateTime(&currTime);
	if(CompareTime(currTime, g_mens_cntx->InputDate, NULL) == TIME_LESS)
		return MENS_ERR_DATE;

	/* date too small */
	memset(&incTime, 0, sizeof(MYTIME));
	incTime.nMonth = MENS_NUM_PREV_MON;
	DecrementTime(currTime,incTime,&finalTime);
	if(CompareTime(g_mens_cntx->InputDate, finalTime, NULL) == TIME_LESS)
		return MENS_ERR_DATE;

	return MENS_ERR_NONE;
}


/*****************************************************************************
* FUNCTION
*	ExitMensResult
* DESCRIPTION
*  	Exit function of calendar screen.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitMensResult(void)
{
	if(g_mens_cntx->IsAddToHistory)
	{
		history_t   Scr; /* Variable to hold the history data*/
		S16 nHistory = 0;
		Scr.scrnID = SCR_ID_MENS_RESULT;
		Scr.entryFuncPtr = EntryMensResult;
		pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
		if(g_mens_cntx->CurrDay > 0)
			g_mens_cntx->CurrDay = g_mens_cntx->CurrDay;
		AddHistory(Scr);
	}
}


/*****************************************************************************
* FUNCTION
*	EntryMensResult
* DESCRIPTION
*  	Display result calendar of computation.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#include"mainmenudef.h"
void EntryMensResult(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	S16 DiffDays;
	S16  Color[31];
	MYTIME currTime;
	S8 tmpString[MENS_TITLE_LEN];

	S32 i;
	S32 FirstDOW;
	S32 DayOfMon;
	
	EntryNewScreen(SCR_ID_MENS_RESULT, ExitMensResult, NULL, NULL);
	
	g_mens_cntx->IsAddToHistory = TRUE;
	
	RegisterCalendarHighlightHandler(HighlightMensNevigeteDate);

	memset(&currTime, 0, sizeof(MYTIME));
	
	currTime.nMonth = (U8)g_mens_cntx->CurrMonth;
	currTime.nYear = (U16)g_mens_cntx->CurrYear;
	currTime.nDay = 1;

	guiBuffer = GetCurrGuiBuffer(SCR_ID_MENS_RESULT);

	/* back ground initialize */
	FirstDOW = DOW((U16)g_mens_cntx->CurrYear,(U8) g_mens_cntx->CurrMonth,1);
	DayOfMon = LastDayOfMonth((U8)g_mens_cntx->CurrMonth,(U16)g_mens_cntx->CurrYear);
	InitializeCategory81Screen(FirstDOW,DayOfMon,
		                       g_mens_cntx->CurrDay,g_mens_cntx->CurrMonth,g_mens_cntx->CurrYear);
#if defined(MEM_ULC_3216)
SetCalendarBackground((U16)WALLPAPER_1);	
#elif defined (__MMI_MAINLCD_220X176__)	
	SetCalendarBackground(IMG_MAIN_MENU_BG1);
#else
#ifdef __MMI_SLIM_CALENDAR__
	SetCalendarBackground((U16)IMG_CALENDAR_JAN);	
#else
	SetCalendarBackground((U16)(IMG_CALENDAR_JAN + g_mens_cntx->CurrMonth - 1));	
#endif
#endif


	/* title string */
	sprintf(tmpString,"%d %02d",(U16)g_mens_cntx->CurrYear,g_mens_cntx->CurrMonth);
	AnsiiToUnicodeString(g_mens_cntx->TitleString, tmpString);

	/* get/set color of days */
	DiffDays = MensGetDaysDifference(&currTime,&g_mens_cntx->InputDate);
	for(i=1; i<= DayOfMon; i++, DiffDays++)
	{
		Color[i] = (MensGetDisplayColor(DiffDays,g_mens_cntx->AvgPeriod));
		
		if(Color[i] != COLOR_NONE) 	
			SetCalendarDay(i,(U8)(CALENDAR_SPECIAL_DAY1+Color[i] -1));
	}

	SetCalendarMonthFunctions(MensGoToPreviousDay,
		                    	MensGoToNextDay,
								MensGoToPreviousWeek,
								MensGoToNextWeek);
	ShowCategory81Screen((U8*)g_mens_cntx->TitleString,
		                 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 (U8*)GetString(STR_CALENDER_MESSAAGE1),(U8*)GetString(STR_CALENDER_MESSAAGE2),
						 guiBuffer);


#ifdef __MMI_TOUCH_SCREEN__
	/*Click the arrow icon in the side of month to previous/next month-fixbug-12605 by liuxl on 20090615*/
	SetCalendarMonthChangeFunctions(MensGoToPreviousMonth,MensGoToNextMonth);
#endif	/*__MMI_TOUCH_SCREEN__*/

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryMensProbability,KEY_EVENT_UP);

	SetKeyHandler(MensGoToNextMonth,KEY_VOL_DOWN, KEY_EVENT_DOWN);
	SetKeyHandler(MensGoToPreviousMonth,KEY_VOL_UP, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	MensPreEntryResult
* DESCRIPTION
*  	Initialize variables and go to result screen.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensPreEntryResult(void) 
{
	MYTIME currTime;
	
	GetDateTime(&currTime);
	/*modify to not view the previou month -fixbug-12603 by liuxl on 20090615*/
	g_mens_cntx->NumOfMon = 0;
	g_mens_cntx->CurrMonth = currTime.nMonth;
	g_mens_cntx->CurrYear = currTime.nYear;
	g_mens_cntx->CurrDay = currTime.nDay;

	EntryMensResult();
}


/*****************************************************************************
* FUNCTION
*	HighlightMensNevigeteDate
* DESCRIPTION
*  	Highlight handler of calendar screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightMensNevigeteDate(S32 index)
{
   g_mens_cntx->CurrDay = (U8)index;
}


/*****************************************************************************
* FUNCTION
*	MensGoToPreviousDay
* DESCRIPTION
*  	Handle left arrow key when highlighting on 1st day of month.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensGoToPreviousDay(void)
{
	if(g_mens_cntx->NumOfMon > 0)
	{
		MYTIME myTime,incTime,finaltime;

		/* do not have to prevent 1970/01/01 because can not nevigate previous month */
		memset(&myTime, 0, sizeof(MYTIME));
		myTime.nDay = (U8) g_mens_cntx->CurrDay;
		myTime.nMonth =(U8) g_mens_cntx->CurrMonth;
		myTime.nYear =(U16)  g_mens_cntx->CurrYear;

		memset(&incTime, 0, sizeof(MYTIME));
		incTime.nDay = 1;
		DecrementTime(myTime, incTime, &finaltime);
		if(g_mens_cntx->CurrMonth != finaltime.nMonth)
			g_mens_cntx->NumOfMon++;
		g_mens_cntx->CurrYear = finaltime.nYear;
		g_mens_cntx->CurrMonth = finaltime.nMonth;
		g_mens_cntx->CurrDay = finaltime.nDay;
		g_mens_cntx->IsAddToHistory = FALSE;

		EntryMensResult();
	}
	else
	{
		g_mens_cntx->IsAddToHistory = TRUE;
		MensDisplayBoundaryPopup();
		ClndPopupFlag = TRUE; 
	}
}


/*****************************************************************************
* FUNCTION
*	MensGoToNextDay
* DESCRIPTION
*  	Handle right arrow key when highlighting on last day of month.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensGoToNextDay(void)
{
	if(g_mens_cntx->NumOfMon < MENS_MAX_NAVI_MON)
	{
		if(g_mens_cntx->CurrYear!= 2030 || g_mens_cntx->CurrMonth != 12) 
		{
			MYTIME myTime,incTime,finaltime;

			memset(&myTime, 0, sizeof(MYTIME));
			
			myTime.nDay = (U8) g_mens_cntx->CurrDay;
			myTime.nMonth =(U8) g_mens_cntx->CurrMonth;
			myTime.nYear = (U16) g_mens_cntx->CurrYear;

			memset(&incTime, 0, sizeof(MYTIME));
			incTime.nDay= 1;
			finaltime = IncrementTime(myTime,incTime);

			if(g_mens_cntx->CurrMonth != finaltime.nMonth)
				g_mens_cntx->NumOfMon++;

			g_mens_cntx->CurrYear = finaltime.nYear;
			g_mens_cntx->CurrMonth = finaltime.nMonth;
			g_mens_cntx->CurrDay = finaltime.nDay;
			g_mens_cntx->IsAddToHistory = FALSE;
			EntryMensResult();
			return;		
		}
	}
	
	g_mens_cntx->IsAddToHistory = TRUE;
	MensDisplayBoundaryPopup();	
	ClndPopupFlag = TRUE; 
}


/*****************************************************************************
* FUNCTION
*	MensGoToPreviousWeek
* DESCRIPTION
*  	Handle up arrow key when highlighting on first week of the month.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensGoToPreviousWeek(void)
{	
	if(g_mens_cntx->NumOfMon > 0)
	{
		MYTIME myTime ,incTime, finaltime;
		memset(&myTime, 0, sizeof(MYTIME));

		myTime.nDay = (U8) g_mens_cntx->CurrDay;
		myTime.nMonth =(U8) g_mens_cntx->CurrMonth;
		myTime.nYear = (U16) g_mens_cntx->CurrYear;

		memset(&incTime, 0, sizeof(MYTIME));
		incTime.nDay = 7;

		DecrementTime(myTime,incTime,&finaltime);
		if(g_mens_cntx->CurrMonth != finaltime.nMonth)
			g_mens_cntx->NumOfMon--;

		g_mens_cntx->CurrYear = finaltime.nYear;
		g_mens_cntx->CurrMonth = finaltime.nMonth;
		g_mens_cntx->CurrDay = finaltime.nDay;
		
		g_mens_cntx->IsAddToHistory = FALSE;

		EntryMensResult();
	}
	else
	{
		g_mens_cntx->IsAddToHistory = TRUE;
		MensDisplayBoundaryPopup();
		ClndPopupFlag = TRUE; 
	}
}


/*****************************************************************************
* FUNCTION
*	MensGoToNextWeek
* DESCRIPTION
*  	Handle down arrow key when highlighting on last week of the month.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensGoToNextWeek(void)
{	
	if(	g_mens_cntx->NumOfMon < MENS_MAX_NAVI_MON)
	{
		if(g_mens_cntx->CurrYear != 2030 || g_mens_cntx->CurrMonth != 12) 
		{
			MYTIME myTime,incTime,finaltime;
			memset(&myTime, 0, sizeof(MYTIME));

			myTime.nDay =(U8) g_mens_cntx->CurrDay;
			myTime.nMonth = (U8)g_mens_cntx->CurrMonth;
			myTime.nYear = (U16) g_mens_cntx->CurrYear;

			memset(&incTime, 0, sizeof(MYTIME));
			incTime.nDay = 7;
			finaltime = IncrementTime(myTime,incTime);
			if(g_mens_cntx->CurrMonth != finaltime.nMonth)
				g_mens_cntx->NumOfMon++;

			g_mens_cntx->CurrYear = finaltime.nYear ;
			g_mens_cntx->CurrMonth = finaltime.nMonth ;
			g_mens_cntx->CurrDay = finaltime.nDay ;
			g_mens_cntx->IsAddToHistory = FALSE;
			EntryMensResult();
			return;
		}
	}
	
	g_mens_cntx->IsAddToHistory = TRUE;
	MensDisplayBoundaryPopup();
	ClndPopupFlag = TRUE; 
}


/*****************************************************************************
* FUNCTION
*	MensGoToPreviousMonth
* DESCRIPTION
*  	Side up key handler, go to the first day of previois month.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensGoToPreviousMonth(void)
{
	if(g_mens_cntx->NumOfMon > 0)
	{
		MYTIME myTime,incTime,finaltime;
		memset(&myTime, 0, sizeof(MYTIME));

		myTime.nDay = 1;
		myTime.nMonth =(U8)g_mens_cntx->CurrMonth;
		myTime.nYear =(U16)g_mens_cntx->CurrYear;
			
		memset(&incTime, 0, sizeof(MYTIME));
		incTime.nMonth = 1;
			
		DecrementTime(myTime,incTime,&finaltime);
		if(g_mens_cntx->CurrMonth != finaltime.nMonth)
			g_mens_cntx->NumOfMon--;
		g_mens_cntx->CurrYear = finaltime.nYear;
		g_mens_cntx->CurrMonth = finaltime.nMonth;
		g_mens_cntx->CurrDay = finaltime.nDay;

		g_mens_cntx->IsAddToHistory = FALSE;

		EntryMensResult();
	}
	else
	{
		g_mens_cntx->IsAddToHistory = TRUE;
		MensDisplayBoundaryPopup();
	}
}


/*****************************************************************************
* FUNCTION
*	MensGoToNextMonth
* DESCRIPTION
*  	Side down key handler, go to the first day of next month.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensGoToNextMonth()
{
	if(g_mens_cntx->NumOfMon<MENS_MAX_NAVI_MON)
	{
		if(g_mens_cntx->CurrYear != 2030 || g_mens_cntx->CurrMonth != 12) 
		{
			MYTIME myTime,incTime,finaltime;
			memset(&myTime, 0, sizeof(MYTIME));
			
			myTime.nDay = 1;
			myTime.nMonth = (U8) g_mens_cntx->CurrMonth;
			myTime.nYear = (U16) g_mens_cntx->CurrYear;

			memset(&incTime, 0, sizeof(MYTIME));
			incTime.nMonth = 1;
			finaltime = IncrementTime(myTime,incTime);

			if(g_mens_cntx->CurrMonth != finaltime.nMonth)
				g_mens_cntx->NumOfMon++;

			g_mens_cntx->CurrYear = finaltime.nYear ;
			g_mens_cntx->CurrMonth = finaltime.nMonth ;
			g_mens_cntx->CurrDay = finaltime.nDay ;
			g_mens_cntx->IsAddToHistory = FALSE;
			EntryMensResult();
			return;
		}
	}
		
	g_mens_cntx->IsAddToHistory = TRUE;
	MensDisplayBoundaryPopup();

}


/*****************************************************************************
* FUNCTION
*	MensDisplayBoundaryPopup
* DESCRIPTION
*  	Display popup screen for out of boundary.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MensDisplayBoundaryPopup(void)
{
	DisplayPopup((U8*)GetString(STR_CANNOT_NAVIGATE_FURTHER_MESSAGE),IMG_GLOBAL_WARNING,0,UI_POPUP_NOTIFYDURATION_TIME,(U8)WARNING_TONE);
}
	

#define MENS_PROBABILITY
/*****************************************************************************
* FUNCTION                                                            
*	MensCalcProbability
* DESCRIPTION                                                   
*   To calculate the probablity of ovulation.
* PARAMETERS
*	NONE
* RETURNS
*	probablity value
* GLOBALS AFFECTED
*   NONE
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

S32 MensCalcProbability(void)
{
	FLOAT prob;
	S16 DiffDays;
	MYTIME tmpDate;

	memset(&tmpDate, 0, sizeof(MYTIME));
	tmpDate.nDay = (U8)g_mens_cntx->CurrDay;
	tmpDate.nMonth = (U8)g_mens_cntx->CurrMonth;
	tmpDate.nYear = (U16)g_mens_cntx->CurrYear;

	DiffDays = MensGetDaysDifference(&tmpDate, &g_mens_cntx->InputDate);
	prob = (MensGetPregancyProb((S16)DiffDays,(S16)g_mens_cntx->AvgPeriod));
	
	return (S32)(prob * 100);
}
//add by liuxn 060616 start
#else
S32 MensCalcProbability(void)
{
//add by zough 061121 start
	S32 prob;
	S16 DiffDays;
	MYTIME tmpDate;
	memset(&tmpDate, 0, sizeof(MYTIME));
	tmpDate.nDay = (U8)g_mens_cntx->CurrDay;
	tmpDate.nMonth = (U8)g_mens_cntx->CurrMonth;
	tmpDate.nYear = (U16)g_mens_cntx->CurrYear;
	DiffDays = MensGetDaysDifference(&tmpDate, &g_mens_cntx->InputDate);
	prob = (MensGetPregancyProb((S16)DiffDays, (S16)g_mens_cntx->AvgPeriod));
	
	return prob ;
//add by zough 061121 end
}
#endif
//add by liuxn 060616 end


/*****************************************************************************
* FUNCTION                                                            
*	EntryMensProbability
* DESCRIPTION                                                   
*   Display probability screen.
* PARAMETERS
*	NONE
* RETURNS
*	None.
* GLOBALS AFFECTED
*   NONE
*****************************************************************************/
void EntryMensProbability(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
    S32 Prob;

	EntryNewScreen(SCR_ID_MENS_PROB, NULL, EntryMensProbability, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_MENS_PROB);	

	Prob = MensCalcProbability();
	
	ShowCategory72Screen(STR_SCR_HEALTH_MENSIS_PROBABILITY_CAPTION,GetRootTitleIcon(ORGANIZER_HEALTH_MENU),
		                 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 0, 0,
						 (U8*)GetString(STR_SCR_HEALTH_MENSIS_PROBABILITY_MESSAGE),Prob, guiBuffer);
						 
	SetLeftSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}



#define MENS_UTILITY
/*****************************************************************************
* FUNCTION                                                            
*	MensGetPregancyProb
* DESCRIPTION                                                           
*   This function is to calculate probability of pregnancy.
* PARAMETERS
*	S16	DiffDay 	IN		the number of diffenent dates between given date
*							and menstrual date
*	S16	AvgPeriod	IN		average perviods
* RETURNS
*	FLOAT		the probability of pregnancy
*
* GLOBALS AFFECTED
*   NONE
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

FLOAT MensGetPregancyProb(S16 DiffDay, S16 AvgPeriod)
{
	S16 i;
	S16 Nth;	/* the Nth day from menstrual date */
	S16 OD;		/* the estimated ovulated date */
	
	for(i=0;DiffDay<0;i++)
		DiffDay += AvgPeriod*i;		

	Nth = DiffDay % AvgPeriod;
	
	OD = AvgPeriod/2;

	if (Nth <MENS_ZERO_PROB_PERIOD)
		return (FLOAT)0.01;
	else if (Nth <= (OD+MENS_OVULATION_DAYS) && Nth >= (OD-MENS_OVULATION_DAYS))
		return (FLOAT)0.99;
	else if (Nth < AvgPeriod/2)
		return (FLOAT) Nth*2/AvgPeriod;
	else
		return (2 - (FLOAT)2*Nth/(AvgPeriod));

}
//add by liuxn 060616 start
#else
//add by zough 061121 start
S32 MensGetPregancyProb(S16 DiffDay, S16 AvgPeriod)
{
	S16 i;
	S16 Nth;	/* the Nth day from menstrual date */
	S16 OD;		/* the estimated ovulated date */
	
	for (i = 0; DiffDay < 0; i++)
		DiffDay += AvgPeriod * i;		
	Nth = DiffDay % AvgPeriod;
	OD = AvgPeriod / 2;
	
	if (Nth < MENS_ZERO_PROB_PERIOD)
	{
		return 1;
	}
	else if (Nth <= (OD + MENS_OVULATION_DAYS) && Nth >= (OD  - MENS_OVULATION_DAYS))
	{
		return 99;
	}
	else if (Nth < AvgPeriod / 2)
	{
		return  Nth * 2 * 100 / AvgPeriod;
	}
	else
	{
		return (200 - 200 * Nth / (AvgPeriod));
	}
	
 	return 1;
 //add by zough 061121 end
}
#endif
//add by liuxn 060616 end


/*****************************************************************************
* FUNCTION                                                            
*	MensGetDisplayColor
* DESCRIPTION                                                           
*   This function is to calculate which color to display of a given date.
* PARAMETERS
*	S16	DiffDay 	IN		the number of diffenent dates between given date
*							and menstrual date
*	S16	AvgPeriod	IN		average perviods
* RETURNS
*	enum MENS_COLOR_EMUM	the color to be display
* GLOBALS AFFECTED
*   NONE
*****************************************************************************/
MENS_COLOR_EMUM MensGetDisplayColor(S16 DiffDay, U8 AvgPeriod)
{	
	S16 i;
	S16 Nth;	/* the Nth day from menstrual date */
	S16 OD;		/* the estimated ovulated date */
	
	for(i=0;DiffDay<0;i++)
		DiffDay += AvgPeriod*i;		

	Nth = DiffDay % AvgPeriod;
	
	OD = AvgPeriod/2;

	if (Nth <MENS_PERIOD)
		return RED_COLOR;
	else if(Nth == OD)
		return ORANGE_COLOR;
	else if (Nth <= (OD+MENS_OVULATION_DAYS) && Nth >= (OD-MENS_OVULATION_DAYS))
		return YELLOW_COLOR;
	else
		return COLOR_NONE;

}


/*****************************************************************************
* FUNCTION                                                            
*	MensGetDaysDifference
* DESCRIPTION                                                           
*   This function is to calculate number difference days between two date.
* PARAMETERS
*	t1 	IN		date to be computed
*	t2	IN		date to be computed
* RETURNS
*	Difference between t1 and t2 (t1 - t2).
* GLOBALS AFFECTED
*   NONE
*****************************************************************************/
S16 MensGetDaysDifference(MYTIME *t1, MYTIME *t2)
{
	S16 Result;
	MYTIME startTime, endTime, incTime;
	U8 SignFlag;

	TIME_EQUALITY eq = CompareTime(*t1, *t2, NULL);

	memset(&incTime, 0, sizeof(MYTIME));
	if(eq == TIME_LESS)	/* t1 < t2 */
	{
		startTime = *t1;
		endTime = *t2;
		SignFlag = 0;
	}
	else if(eq == TIME_GREATER)	/* t1 > t2 */
	{
		startTime = *t2;
		endTime = *t1;
		SignFlag = 1;
	}
	else /* no difference */
		return 0;

	/* day */
	if(endTime.nMonth > startTime.nMonth || endTime.nYear > startTime.nYear)
	{
		Result = endTime.nDay + LastDayOfMonth(startTime.nMonth, startTime.nYear) - startTime.nDay;
		incTime.nMonth = 1;
		startTime = IncrementTime(startTime, incTime);
		startTime.nDay = 1; /* to prevent day out of boundary */

	   	/* month */
   		while(!(endTime.nMonth == startTime.nMonth && endTime.nYear == startTime.nYear))
	   	{
	   		Result += LastDayOfMonth(startTime.nMonth, startTime.nYear);
	   		startTime = IncrementTime(startTime, incTime);
	   	}
	}
	else
		Result = endTime.nDay - startTime.nDay;

	if(SignFlag == 1)
		return Result;
	else
		return Result*(-1);
}

#endif // #ifdef __MMI_MENSTRUAL__
#endif	// _MMI_HEALTHMENSTRUAL_C


