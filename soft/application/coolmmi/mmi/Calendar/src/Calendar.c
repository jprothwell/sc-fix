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
 *	Calendar.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements Calendar application.
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

/**********************************************************************************
	Filename:		Calendar.c
	Author:			GAUTAM
	Date Created:	July-12-2003
**********************************************************************************/

#ifndef _MMI_CALENDAR_C
#define _MMI_CALENDAR_C
#include "mmi_features.h"
#include "mmi_data_types.h"
#ifdef __MMI_CALENDAR__

#include "datetimetype.h"
#include "datetimegprot.h"
#include "alarmgprot.h"
#include "unicodexdcl.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "wgui_categories.h"
#include "wgui_calendar.h"
#include "custdatares.h"
#include "globaldefs.h"
#include "organizerdef.h"

#include "calendardef.h"
#include "calendarprot.h"
#include "nvramenum.h"
#include "audioinc.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "debuginitdef.h"
#include "commonscreens.h"
#include "todolistenum.h"
#include "wgui_categories_defs.h"	 
  
/*  Include: PS header file */
//... Add More PS header


/* 
** Define
*/
#define	CLNDR_DAY_LEN				3*ENCODING_LENGTH
#define	CLNDR_MON_LEN				3*ENCODING_LENGTH
#define	CLNDR_YEAR_LEN				6*ENCODING_LENGTH

#define	CLNDR_TABLE_START_YEAR 		1969  /* the frist year in the database table*/
#define	CLNDR_TABLE_END_YEAR 	 	2031	/* the last year in the database table */
#define	CLNDR_TIME_PER_YEAR			31556926
/* indication codes */
#define CLNDR_BASE 1939

/*
** Typedef 
*/
/* struct clndr_lnuar_date_info_struct defines all data needed for lunar calendar */
typedef struct 
{
  U8 BaseDays;		/* different days from solar Jan 1 to lunar Jan 1. */
  U8 BaseWeekday;	/* week day of Jan 1 this year. 0=Sunday*/
  U8 BaseKanChi;	/* the Kan and Chin of Jan 1 this year. */
  S8 Intercalary;	/* an intercalary month in the lunar calendar. 							
						0==no intercalary month in this year. */
  U16 MonthDays;	/* the number of days in lunar month, bit-level representation 0=29, 1=30. */
}clndr_lunar_table_struct;


typedef struct
{
	// U16	ImgID;
	MYTIME CalTime;
	clndr_lnuar_date_info_struct LunarInfo;
	// S8	TitleString[CLNDR_TITLE_LEN]; 
	S8	DayBuf[CLNDR_DAY_LEN];
	S8	MonBuf[CLNDR_MON_LEN];
	S8	YearBuf[CLNDR_YEAR_LEN];
	U8	TaskDayList[CLNDR_MAX_DAYS+1];
	U8	LunarState;
	U8	DayOnFirst;
	U8	NumDaysInMonth;
}clndr_context_struct;


/* 
** Local Variable
*/

/*the solar term days for lunarinfo by feifan 8/1/2005 */
const U8 solar_term_table[62][24] =
{
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,9,24,8,23,7,22},//1970
{6,21,4,19,6,21,5,21,6,22,6,22,8,23,8,24,8,24,9,24,8,23,8,22},
{6,21,5,19,5,20,5,20,5,21,5,21,7,23,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,5,21,6,21,7,23,8,23,8,23,8,23,7,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,9,24,8,23,7,22},
{6,21,4,19,6,21,5,21,6,22,6,22,8,23,8,24,8,23,9,24,8,23,8,22},//1975
{6,21,5,19,5,20,4,20,5,21,5,21,7,23,7,23,7,23,8,23,7,22,7,23},
{5,20,4,19,6,21,5,20,5,21,6,21,7,23,7,23,8,23,8,23,7,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,8,24,8,23,7,22},
{6,21,4,19,6,21,5,21,6,21,6,22,8,23,8,24,8,23,9,24,8,23,8,22},
{6,21,5,19,5,20,4,20,5,21,5,21,7,23,7,23,7,23,8,23,7,22,7,22},//1980
{5,20,4,19,6,21,5,20,5,21,6,21,7,23,7,23,8,23,8,23,7,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,8,24,8,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,8,23,8,24,8,23,9,24,8,23,8,22},
{6,21,4,19,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,5,21,5,20,5,21,6,21,7,23,7,23,8,23,8,23,7,22,7,22},//1985
{5,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,8,24,8,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,24,8,23,9,24,8,23,7,22},
{6,21,4,19,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,21},
{5,20,4,19,5,20,5,20,5,21,6,21,7,23,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,6,21,6,21,7,23,8,23,8,23,8,24,8,22,7,22},//1990
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,9,24,8,23,7,22},
{6,21,4,19,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,21},
{5,20,4,18,5,20,5,20,5,21,6,21,7,23,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,6,21,6,21,7,23,8,23,8,23,8,23,7,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,9,24,8,23,7,22},//1995
{6,21,4,19,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,21},
{5,20,4,18,5,20,5,20,5,21,5,21,7,23,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,6,21,6,21,7,23,8,23,8,23,8,23,7,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,9,24,8,23,7,22},
{6,21,4,19,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,21},//2000
{5,20,4,18,5,20,5,20,5,21,5,21,7,23,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,6,21,6,21,7,23,8,23,8,23,8,23,7,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,9,24,8,23,7,22},
{6,21,4,19,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,21},
{5,20,4,18,5,20,5,20,5,21,5,21,7,23,7,23,7,23,8,23,7,22,7,22},//2005
{5,20,4,19,6,21,5,20,5,21,6,21,7,23,7,23,8,23,8,23,7,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,9,24,8,23,7,22},
{6,21,4,19,5,20,4,20,5,21,5,21,7,22,7,23,7,22,8,23,7,22,7,21},
{5,20,4,18,5,20,4,20,5,21,5,21,7,23,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,5,21,6,21,7,23,7,23,8,23,8,23,7,22,7,22},//2010
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,8,24,8,23,7,22},
{6,21,4,19,5,20,4,20,5,20,5,21,7,22,7,23,7,22,8,23,7,22,7,21},
{5,20,4,18,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,5,21,6,21,7,23,7,23,8,23,8,23,7,22,7,22},
{6,20,4,19,6,21,5,20,6,21,6,22,7,23,8,23,8,23,8,24,8,22,7,22},//2015
{6,20,4,19,5,20,4,19,5,20,5,21,7,22,7,23,7,22,8,23,7,22,7,21},
{5,20,3,18,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,5,21,5,20,5,21,6,21,7,23,7,23,8,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,6,21,6,21,7,23,8,23,8,23,8,24,8,22,7,22},
{6,20,4,19,5,20,4,19,5,20,5,21,6,22,7,22,7,22,8,23,7,22,7,21},//2020
{5,20,3,18,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,21},
{5,20,4,19,5,20,5,20,5,21,6,21,7,23,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,6,21,6,21,7,23,8,23,8,23,8,24,8,22,7,22},
{6,20,4,19,5,20,4,19,5,20,5,21,6,22,7,22,7,22,8,23,7,22,6,21},
{5,20,3,18,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,21},//2025
{5,20,4,18,5,20,5,20,5,21,5,21,7,23,7,23,7,23,8,23,7,22,7,22},
{5,20,4,19,6,21,5,20,6,21,6,21,7,23,8,23,8,23,8,23,7,22,7,22},
{6,20,4,19,5,20,4,19,5,20,5,21,6,22,7,22,7,22,8,23,7,22,6,21},
{5,20,3,18,5,20,4,20,5,21,5,21,7,22,7,23,7,23,8,23,7,22,7,21},
{5,20,4,18,5,20,5,20,5,21,5,21,7,23,7,23,7,23,8,23,7,22,7,22},//2030
};


/* number of accumulated days per solar month, non-leap year and leap year. */
const S16 gClndrAccuDays[] = 
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 396 };

/* array of lunar calendar */

const clndr_lunar_table_struct gClndrLunarInfo[] = 
{
	{ 47, 2, 11,  0, 5482}, 		
	{ 36, 3, 16,  0, 2921},	/* 1970 */
	{ 26, 4, 21,  5, 5970},
	{ 45, 5, 26,  0, 6994},
	{ 33, 0, 32,  0, 6949},
	{ 22, 1, 37,  4, 5707},
	{ 41, 2, 42,  0, 6731},	/* 1975 */
	{ 30, 3, 47,  8, 5291},
	{ 48, 5, 53,  0, 4781},
	{ 37, 6, 58,  0, 5485},
	{ 27, 0,  3,  6, 2921},
	{ 46, 1,  8,  0, 3497},	/* 1980 */
	{ 35, 3, 14,  0, 7570},
	{ 24, 4, 19,  4, 7461},
	{ 43, 5, 24,  0, 7461},
	{ 32, 6, 29, 10, 6733},
	{ 50, 1, 35,  0, 2646},	/* 1985 */
	{ 39, 2, 40,  0, 4790}, 
	{ 28, 3, 45,  6, 1461},
	{ 47, 4, 50,  0, 5845},
	{ 36, 6, 56,  0, 3785},
	{ 26, 0,  1,  5, 7826},	/* 1990 */
	{ 45, 1,  6,  0, 3730},
	{ 34, 2, 11,  0, 3366},
	{ 22, 4, 17,  3, 2646},
	{ 40, 5, 22,  0, 2647},
	{ 30, 6, 27,  8, 5334},	/* 1995 */
	{ 49, 0, 32,  0, 4954},
	{ 37, 2, 38,  0, 5845},
	{ 27, 3, 43,  5, 5833},
	{ 46, 4, 48,  0, 5961},
	{ 35, 5, 53,  0, 5779},	/* 2000 */
	{ 23, 0, 59,  4, 5419},
	{ 42, 1,  4,  0, 5419},
	{ 31, 2,  9,  0, 2651},
	{ 21, 3, 14,  2, 5466},
	{ 39, 5, 20,  0, 5482},	/* 2005 */
	{ 28, 6, 25,  7, 6997},
	{ 48, 0, 30,  0, 7076},
	{ 37, 1, 35,  0, 6985},
	{ 25, 3, 41,  5, 6803},
	{ 44, 4, 46,  0, 6805},	/* 2010 */
	{ 33, 5, 51,  0, 5421},
	{ 22, 6, 56,  4, 2733},
	{ 40, 1,  2,  0, 2741},
	{ 30, 2,  7,  9, 5546},
	{ 49, 3, 12,  0, 5586},	/* 2015 */
	{ 38, 4, 17,  0, 3493},
	{ 27, 6, 23,  6, 7498}, 
	{ 46, 0, 28,  0, 3402},
	{ 35, 1, 33,  0, 3222},
	{ 24, 2, 38,  4, 5422},	/* 2020 */
	{ 42, 4, 44,  0, 5462},
	{ 31, 5, 49,  0, 2741},
	{ 21, 6, 54,  2, 5546},
	{ 40, 0, 59,  0, 5842},
	{ 28, 2,  5,  6, 3749},	/* 2025 */
	{ 47, 3, 10,  0, 5925},
	{ 36, 4, 15,  0, 5707},
	{ 25, 5, 20,  5, 3223},
	{ 43, 0, 26,  0, 5291},
	{ 32, 1, 31,  0, 1371},	/* 2030 */
	{ 22, 2, 36,  3, 2774},
};
/* 
** Local Function
*/

/* 
** Global Variable
*/
/* the number of days for each solar month. */
U8	gClndrAddToHistory;
clndr_context_struct *g_clndr_cntx = NULL;
extern wgui_inline_item wgui_inline_items[];
extern const U8 gDaysInMonth[];

/* 
** Global Function
*/
extern void	SetCategory82Title1(U8*);
extern void	SetCategory82Title2(U8*);
extern void	SetCategory82Title3(U8*);
extern pBOOL IsChineseSet(void);


#ifdef __MMI_TODOLIST__
extern void TDLGetDaysToBeHilited(MYTIME myTime,U8* daylist);
extern void TDLShowSpecificDateList(MYTIME*) ;
#endif

#define CLNDR_INITIALIZE
/*****************************************************************************
* FUNCTION
*	ClndrInit
* DESCRIPTION
*   Register highlight handlers
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrInit(void)
{
	SetHiliteHandler(ORGANIZER_CALENDER_MENU,HighlightClndrMenu);
	SetHintHandler(MENU_CAL_TYPE,ClndrLunarHintHandler);
	SetHiliteHandler(MENU_GOTO_TASK_CAL, HighlightClndrViewTask);
	SetHiliteHandler(MENU_CAL_TYPE, HighlightClndrLunar);
	SetHiliteHandler(MENU_ID_CLNDR_JUMP_TO_DATE, HighlightJumpToDate);
}


/*****************************************************************************
* FUNCTION
*	ClndrDeInit
* DESCRIPTION
*   Free allocated memory
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrDeInit(void)
{
	if(g_clndr_cntx)
	{
		OslMfree(g_clndr_cntx);
		g_clndr_cntx = NULL;
	}
}

/*****************************************************************************
* FUNCTION
*  HighlightClndrMenu
* DESCRIPTION
*   Highlight handler of Calendar menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightClndrMenu(void)
{	
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(ClndrPreEntryApp,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */	
    #ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(ClndrPreEntryApp,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
    #endif
    /*- zhouqin 2011-02-16 modify for q6 320X240 */	
}

#define CLNDR_APP_SCREEN
/*****************************************************************************
* FUNCTION
*	ClndrPreEntryApp
* DESCRIPTION
*   Initialize variables and go to calendar screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrPreEntryApp(void) 
{
	MYTIME	currentTime;
	S16 error;

	if(g_clndr_cntx == NULL)
		g_clndr_cntx = OslMalloc(sizeof(clndr_context_struct));

	GetDateTime(&currentTime);

	memset(&g_clndr_cntx->CalTime, 0, sizeof(MYTIME));

	g_clndr_cntx->CalTime.nDay = currentTime.nDay;
	g_clndr_cntx->CalTime.nMonth = currentTime.nMonth;
	g_clndr_cntx->CalTime.nYear = currentTime.nYear;

	/* lunar state */
	ReadValue(LUNAR_CALENDAR,(void*)&g_clndr_cntx->LunarState,DS_BYTE,&error);	

	if(g_clndr_cntx->LunarState != TRUE)
		g_clndr_cntx->LunarState = FALSE;
	EntryClndrScreen();
}


/*****************************************************************************
* FUNCTION
*	UpdateClndrContext
* DESCRIPTION
*  Callback from GUI layer to update calendar application
* PARAMETERS
*	day		IN
*	month		IN
*	year		IN
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void UpdateClndrContext(s32 day,s32 month,s32 year)
{
	MYTIME myTime;
#ifdef __MMI_TODOLIST__
	S32 i;
#endif	/*__MMI_TODOLIST__*/
	
	g_clndr_cntx->CalTime.nDay = day;
	g_clndr_cntx->CalTime.nMonth = month;
	g_clndr_cntx->CalTime.nYear = year;

	/* get the list of days to be highlithed*/
	memset(&myTime, 0, sizeof(MYTIME));

#ifdef __MMI_TODOLIST__
	myTime.nYear = g_clndr_cntx->CalTime.nYear;
	myTime.nMonth = g_clndr_cntx->CalTime.nMonth;
	myTime.nDay = g_clndr_cntx->CalTime.nDay;	

	TDLGetDaysToBeHilited(myTime,g_clndr_cntx->TaskDayList); 

	if(g_clndr_cntx->TaskDayList[0] != CLNDR_MAX_DAYS) 
	{
		for(i=1;i<=g_clndr_cntx->TaskDayList[0]; i++)
			SetCalendarDay(g_clndr_cntx->TaskDayList[i],CALENDAR_TASK_DAY);
	}
	else 
	{
		for(i=1; i<=CLNDR_MAX_DAYS; i++)
			SetCalendarDay(i,CALENDAR_TASK_DAY);
	}
#endif

	/* browsing to current month */
	GetDateTime(&myTime);
	if(myTime.nMonth == month  && myTime.nYear == year)
		SetCalendarDay(myTime.nDay,CALENDAR_CURRENT_DAY);
	
	if(g_clndr_cntx->LunarState == TRUE && IsChineseSet())
	{
		g_clndr_cntx->LunarInfo.SolarMonth = g_clndr_cntx->CalTime.nMonth;
		g_clndr_cntx->LunarInfo.SolarYear = g_clndr_cntx->CalTime.nYear;
		g_clndr_cntx->LunarInfo.SolarDay = g_clndr_cntx->CalTime.nDay;
		ClndrComputeLunarInfo(&g_clndr_cntx->LunarInfo);
		SetCalendarPopupDay (g_clndr_cntx->LunarInfo.SolarDay ,(U8*)g_clndr_cntx->LunarInfo.MessageLine1 ,(U8*)g_clndr_cntx->LunarInfo.MessageLine2 , (U8*)g_clndr_cntx->LunarInfo.MessageLine3 );

		SetCategory82Title1((U8*)g_clndr_cntx->LunarInfo.TitleInfo1);/* OR calInfo.totlestring*/
		SetCategory82Title3((U8*)g_clndr_cntx->LunarInfo.TitleInfo2);
		
	}
}

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
U16 g_image_list[3];
U8* g_description_list[3];
void ClndrUpdateInfoBox(S32 *num_info, U16 **icon_list, U8 ***description_list)
{
	*num_info = 3;

	g_description_list[0] = (U8*)g_clndr_cntx->LunarInfo.MessageLine1;
	g_description_list[1] = (U8*)g_clndr_cntx->LunarInfo.MessageLine2;
	g_description_list[2] = (U8*)g_clndr_cntx->LunarInfo.MessageLine3;
	*description_list = g_description_list;

	g_image_list[0] = g_image_list[1] = g_image_list[2] = 0;

	*icon_list = g_image_list;
}
#endif


/*****************************************************************************
* FUNCTION
*	EntryClndrScreen
* DESCRIPTION
*   Display calendar screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
extern void wgui_calendar_previous_day(void);
extern void wgui_calendar_previous_row(void);
extern void wgui_calendar_next_day(void);
extern void wgui_calendar_next_row(void);
extern void UpdateClndrContext(s32 day,s32 month,s32 year);

void EntryClndrScreen(void) 
{
#ifdef __MMI_TODOLIST__
	S32 i;
#endif	/*__MMI_TODOLIST__*/

	MYTIME myTime;
	U8 lunarOptionInternal;

	gClndrAddToHistory = FALSE;
	EntryNewScreen(SCR_ID_CLNDR_SCREEN, ExitClndrScreen, NULL, NULL);
	gClndrAddToHistory = TRUE;

	ClndrGetScreenInfo();
	
	if(g_clndr_cntx->LunarState == TRUE && IsChineseSet())
	{
		lunarOptionInternal = TRUE;
		InitializeCategory82Screen(g_clndr_cntx->DayOnFirst, g_clndr_cntx->NumDaysInMonth, g_clndr_cntx->CalTime.nDay,g_clndr_cntx->CalTime.nMonth,g_clndr_cntx->CalTime.nYear);
	}
	else
	{
		lunarOptionInternal = FALSE;
		InitializeCategory80Screen(g_clndr_cntx->DayOnFirst, g_clndr_cntx->NumDaysInMonth, g_clndr_cntx->CalTime.nDay,g_clndr_cntx->CalTime.nMonth,g_clndr_cntx->CalTime.nYear);
	}

#ifdef __MMI_TODOLIST__
	if(g_clndr_cntx->TaskDayList[0] != CLNDR_MAX_DAYS) 
	{
		for(i=1;i<=g_clndr_cntx->TaskDayList[0]; i++)
			SetCalendarDay(g_clndr_cntx->TaskDayList[i],CALENDAR_TASK_DAY);
	}
	else 
	{
		for(i=1; i<=CLNDR_MAX_DAYS; i++)
			SetCalendarDay(i,CALENDAR_TASK_DAY);
	}
#endif

	/* browsing to current month */
	GetDateTime(&myTime);
	if(myTime.nMonth == g_clndr_cntx->CalTime.nMonth && myTime.nYear == g_clndr_cntx->CalTime.nYear)
		SetCalendarDay(myTime.nDay,CALENDAR_CURRENT_DAY);

	RegisterCalendarHighlightHandler(HighlightClndrDate);

	RegisterCalendarUpdateHandler((void*)UpdateClndrContext);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
	if(lunarOptionInternal==TRUE) 
		RegisterCalendarUpdateInfoBoxHandler(ClndrUpdateInfoBox);
#endif

	if(IsChineseSet())
		SetCalendarTitleImage(CAL_CHINESE_DAYS_IMAGE);
	else
		SetCalendarTitleImage(CAL_ENGLISH_DAYS_IMAGE);

	if(lunarOptionInternal==FALSE) 
	{
		ShowCategory80Screen(STR_GLOBAL_OPTIONS,
		IMG_GLOBAL_OPTIONS,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,0); 
	} 
	else 
	{
		g_clndr_cntx->LunarInfo.SolarMonth = g_clndr_cntx->CalTime.nMonth;
		g_clndr_cntx->LunarInfo.SolarYear = g_clndr_cntx->CalTime.nYear;
		g_clndr_cntx->LunarInfo.SolarDay = g_clndr_cntx->CalTime.nDay;
		ClndrComputeLunarInfo(&g_clndr_cntx->LunarInfo);
		SetCalendarPopupDay (g_clndr_cntx->LunarInfo.SolarDay ,(U8*)g_clndr_cntx->LunarInfo.MessageLine1 ,(U8*)g_clndr_cntx->LunarInfo.MessageLine2 , (U8*)g_clndr_cntx->LunarInfo.MessageLine3 );


		SetCategory82Title1((U8*)g_clndr_cntx->LunarInfo.TitleInfo1);/* OR calInfo.totlestring*/
		// SetCategory82Title2((U8*)g_clndr_cntx->TitleString);
		SetCategory82Title3((U8*)g_clndr_cntx->LunarInfo.TitleInfo2);
		SetCalendarPopupTitleLunarFunc(ClndrGeneratePopupInfo);
		
 
		ShowCategory82Screen(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS,
			STR_GLOBAL_BACK,IMG_GLOBAL_OK,0);
/*dongwg Del Start For 6137 Ver: TBM780  on 2007-8-27 16:45 */
			#if 0
			RedrawCategory82Screen();//chenhe ,tmp,070731
			#endif
/*dongwg Del End  For 6137 Ver: TBM780  on 2007-8-27 16:45 */
	}
	SetCalendarMonthFunctions(wgui_calendar_previous_month_day,wgui_calendar_next_month_day,
	wgui_calendar_previous_month_week,wgui_calendar_next_month_week);
	
	SetKeyHandler(wgui_calendar_previous_day,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(wgui_calendar_next_day,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(wgui_calendar_previous_row,KEY_UP_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(wgui_calendar_next_row,KEY_DOWN_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(wgui_calendar_previous_month,KEY_VOL_UP,KEY_EVENT_DOWN);
	SetKeyHandler(wgui_calendar_previous_month,KEY_VOL_UP,KEY_LONG_PRESS);
	SetKeyHandler(wgui_calendar_next_month,KEY_VOL_DOWN,KEY_EVENT_DOWN);
	SetKeyHandler(wgui_calendar_next_month,KEY_VOL_DOWN,KEY_LONG_PRESS);


#ifdef __MMI_TOUCH_SCREEN__
	SetCalendarMonthChangeFunctions(wgui_calendar_previous_month,wgui_calendar_next_month);
	 
	#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
	SetCalendarYearChangeFunctions(wgui_calendar_previous_year,wgui_calendar_next_year);
	#endif
	 
#endif
	SetLeftSoftkeyFunction(EntryClndrOptions,KEY_EVENT_UP); 
	SetRightSoftkeyFunction(ClndrGoBackFromCalendar,KEY_EVENT_UP);		
}


/*****************************************************************************
* FUNCTION
*	ExitClndrScreen
* DESCRIPTION
*   Exit function of calendar screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitClndrScreen(void) 
{
	if(gClndrAddToHistory)
	{
		history_t   Scr; 
		S16 nHistory = 0;
		Scr.scrnID = SCR_ID_CLNDR_SCREEN;
		Scr.entryFuncPtr = EntryClndrScreen;
		GetCategoryHistory(Scr.guiBuffer);
		pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
		AddHistory(Scr);
	}
}


/*****************************************************************************
* FUNCTION
*	HighlightClndrDate
* DESCRIPTION
*   Highlight handler of a date on calendar
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightClndrDate(S32 date)
{
	g_clndr_cntx->CalTime.nDay = (U8)date;
}


/*****************************************************************************
* FUNCTION
*	ClndrGetScreenInfo
* DESCRIPTION
*   Get screen display information of.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrGetScreenInfo(void) 
{
#ifdef __MMI_TODOLIST__
	MYTIME myTime;
#endif	/*__MMI_TODOLIST__*/
	
	/* get the DOW 1st of todays month*/
	g_clndr_cntx->DayOnFirst = DOW(g_clndr_cntx->CalTime.nYear,g_clndr_cntx->CalTime.nMonth,1);
	
	/* get the num of days in todays month*/
	g_clndr_cntx->NumDaysInMonth = LastDayOfMonth(g_clndr_cntx->CalTime.nMonth,g_clndr_cntx->CalTime.nYear);

	/* get the list of days to be highlithed*/
#ifdef __MMI_TODOLIST__
	memset(&myTime, 0, sizeof(MYTIME));
	myTime.nYear = g_clndr_cntx->CalTime.nYear;
	myTime.nMonth = g_clndr_cntx->CalTime.nMonth;
	myTime.nDay = g_clndr_cntx->CalTime.nDay;	
	TDLGetDaysToBeHilited(myTime,g_clndr_cntx->TaskDayList); 
#endif
}


/*****************************************************************************
* FUNCTION
*	ClndrGoBackFromCalendar
* DESCRIPTION
*   Free memory and go back to previous screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrGoBackFromCalendar(void) 
{
	ClndrDeInit();
	GoBackHistory();
}


/*****************************************************************************
* FUNCTION
*	ClndrGeneratePopupInfo
* DESCRIPTION
*   Adjust solar day accoding and get lunar info.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrGeneratePopupInfo(U8 type,U8 num_cols,U8 new_day,U8 old_day)
{
	switch(type)
	{
		case NEXT_DAY	:	g_clndr_cntx->LunarInfo.SolarDay +=1;	break;
		case PREV_DAY	:	g_clndr_cntx->LunarInfo.SolarDay -=1;	break;
		case NEXT_ROW	:	g_clndr_cntx->LunarInfo.SolarDay += (S16)num_cols ;	break;
		case PREV_ROW	:	g_clndr_cntx->LunarInfo.SolarDay -= (S16)num_cols ;	break;
		case SELECTED_DAY:	g_clndr_cntx->LunarInfo.SolarDay +=new_day-old_day;	break;
		default			:	break;
	}

	ClndrComputeLunarInfo(&g_clndr_cntx->LunarInfo);
	SetCalendarPopupDay (g_clndr_cntx->LunarInfo.SolarDay ,(U8*)g_clndr_cntx->LunarInfo.MessageLine1, (U8*)g_clndr_cntx->LunarInfo.MessageLine2, (U8*)g_clndr_cntx->LunarInfo.MessageLine3);
	ChangeCategory82Title1((U8*)g_clndr_cntx->LunarInfo.TitleInfo1);/* OR calInfo.totlestring*/
	ChangeCategory82Title3((U8*)g_clndr_cntx->LunarInfo.TitleInfo2);
}


#define CLNDR_OPTION
/*****************************************************************************
* FUNCTION
*	EntryClndrOptions
* DESCRIPTION
*   Side down key handler, go to 1st day of next month.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryClndrOptions(void) 
{
	U8* guiBuffer;
	U8 *pop_up_desc[3] ;
	U16 nStrItemList[3]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
		
	gClndrAddToHistory = TRUE;
	EntryNewScreen(SCR_ID_CLNDR_OPTION, ExitClndrOptions, EntryClndrOptions, NULL);
	gClndrAddToHistory = FALSE;

	guiBuffer = GetCurrGuiBuffer(SCR_ID_CLNDR_OPTION);

    /*  Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(ORGANIZER_CALENDER_MENU);

	/*  Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(ORGANIZER_CALENDER_MENU,nStrItemList);
	
	ConstructHintsList(ORGANIZER_CALENDER_MENU, pop_up_desc);

	/*  Set current parent id*/
	SetParentHandler(ORGANIZER_CALENDER_MENU);
 
	/*  Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	if(IsChineseSet())
	{
		ShowCategory52Screen(STR_GLOBAL_OPTIONS,GetRootTitleIcon(ORGANIZER_CALENDER_MENU),STR_GLOBAL_OK,
		IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_OK,nNumofItem,
		nStrItemList,(PU16)gIndexIconsImageList,pop_up_desc,0,0,guiBuffer);
	}
	else
	{
		ShowCategory52Screen(STR_GLOBAL_OPTIONS,ORGANIZER_TITLE_IMAGEID,STR_GLOBAL_OK,
		IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_OK,(nNumofItem - 1),
		nStrItemList,(PU16)gIndexIconsImageList,pop_up_desc,0,0,guiBuffer);
	}

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
	
}

/*****************************************************************************
* FUNCTION
*	ExitClndrOptions
* DESCRIPTION
*   Exit function of calendar option screen. write lunar state to nvram.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitClndrOptions(void) 
{
	S16 error;
	WriteValue(LUNAR_CALENDAR,&g_clndr_cntx->LunarState,DS_BYTE,&error);
}


/*****************************************************************************
* FUNCTION
*	ClndrSetLunarState
* DESCRIPTION
*   Toggle state of lunar calendar.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrSetLunarState(void)
{
	U16 indexPopup;
	indexPopup = GetCurrHiliteID();

	if(g_clndr_cntx->LunarState == TRUE)
	{
		g_clndr_cntx->LunarState = FALSE;
		pfnUnicodeStrcpy((S8*)hintData[indexPopup],(S8*)GetString(STR_GLOBAL_OFF));
		ChangeLeftSoftkey(STR_GLOBAL_ON,IMG_CAL_ON);	
	}
	else
	{
		g_clndr_cntx->LunarState = TRUE;
		pfnUnicodeStrcpy((S8*)hintData[indexPopup],(S8*)GetString(STR_GLOBAL_ON));
		ChangeLeftSoftkey(STR_GLOBAL_OFF, IMG_CAL_OFF);			
	}

	Category52ChangeItemDescription(indexPopup,hintData[indexPopup]);
	SetLeftSoftkeyFunction(ClndrSetLunarState, KEY_EVENT_UP);
	RedrawCategoryFunction();
}


/*****************************************************************************
* FUNCTION
*	ClndrDisplayTask
* DESCRIPTION
*   Display to do list task of selected date.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrDisplayTask(void)
{
	MYTIME calTime;
	calTime.nDay = g_clndr_cntx->CalTime.nDay;
	calTime.nMonth = g_clndr_cntx->CalTime.nMonth;
	calTime.nYear = g_clndr_cntx->CalTime.nYear;
#ifdef __MMI_TODOLIST__
	TDLShowSpecificDateList(&calTime);
#endif
}


/*****************************************************************************
* FUNCTION
*	ClndrLunarHintHandler
* DESCRIPTION
*   Hint handler of lunar calendar option.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrLunarHintHandler(U16 index)
{
	if(g_clndr_cntx->LunarState == FALSE) 
		pfnUnicodeStrcpy((S8*) hintData[index], (S8*)GetString(STR_GLOBAL_OFF));
	else 
		pfnUnicodeStrcpy((S8*) hintData[index], (S8*)GetString(STR_GLOBAL_ON));
}


/*****************************************************************************
* FUNCTION
*	HighlightClndrViewTask
* DESCRIPTION
*   Highlight handler of View Task option.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightClndrViewTask(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(ClndrDisplayTask,KEY_EVENT_UP); 
	SetKeyHandler(ClndrDisplayTask, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightClndrLunar
* DESCRIPTION
*   Highlight handler on lunar calendar option.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightClndrLunar(void)
{
	U16 indexPopup= GetCurrHiliteID();
	if(g_clndr_cntx->LunarState == FALSE) 
	{	
		pfnUnicodeStrcpy((S8*)hintData[indexPopup],(S8*)GetString(STR_GLOBAL_OFF));
		ChangeLeftSoftkey(STR_GLOBAL_ON, IMG_CAL_ON);	
	}
	else
	{
		pfnUnicodeStrcpy((S8*)hintData[indexPopup],(S8*)GetString(STR_GLOBAL_ON));
		ChangeLeftSoftkey(STR_GLOBAL_OFF, IMG_CAL_OFF);
	}
	
	SetLeftSoftkeyFunction(ClndrSetLunarState,KEY_EVENT_UP); 
	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

#define CLNDR_JUMP_TO_DATE
/*****************************************************************************
* FUNCTION
*	ClndrValidateInput
* DESCRIPTION
*   Validate correctness of date and jump to the date in calendar.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrValidateInput(void)
{
	S8 tmpMon[CLNDR_DAY_LEN];
	S8 tmpDay[CLNDR_MON_LEN];
	S8 tmpYear[CLNDR_YEAR_LEN];
	U8 DayOfMon;

	U8 nDay, nMonth;
	U16 nYear;

	CloseCategory57Screen();

	/* validate day */
	UnicodeToAnsii(tmpDay, g_clndr_cntx->DayBuf);
	UnicodeToAnsii(tmpMon, g_clndr_cntx->MonBuf);
	UnicodeToAnsii(tmpYear, g_clndr_cntx->YearBuf);

	nDay = atoi(tmpDay);
	nMonth = atoi(tmpMon);
	nYear = atoi(tmpYear);

	/* daty out of boundary */
	DayOfMon = LastDayOfMonth(nMonth, nYear);
	if(nDay > DayOfMon)
	{
		DisplayPopup((U8*)GetString(STR_CLNDR_WRONG_DATE),IMG_GLOBAL_WARNING,0,UI_POPUP_NOTIFYDURATION_TIME,(U8)WARNING_TONE);
	}
	else
	{
		g_clndr_cntx->CalTime.nDay = nDay;
		g_clndr_cntx->CalTime.nMonth = nMonth;
		g_clndr_cntx->CalTime.nYear = nYear;
		GoBackToHistory(SCR_ID_CLNDR_SCREEN);
	}
}


/*****************************************************************************
* FUNCTION
*	ClndrFillInilineStruct
* DESCRIPTION
*   Fill inline structure of date input screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrFillInilineStruct(void)
{
	S8 tmpMon[CLNDR_DAY_LEN];
	S8 tmpDay[CLNDR_MON_LEN];
	
	coolsand_UI_itoa((S32)g_clndr_cntx->CalTime.nYear,(U16*)g_clndr_cntx->YearBuf,10);

	sprintf(tmpMon,"%02d",g_clndr_cntx->CalTime.nMonth);
	AnsiiToUnicodeString(g_clndr_cntx->MonBuf, tmpMon);
		
	sprintf(tmpDay,"%02d",g_clndr_cntx->CalTime.nDay);
	AnsiiToUnicodeString(g_clndr_cntx->DayBuf, tmpDay);	

	SetInlineItemActivation(&wgui_inline_items[0],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[0],(U8*) GetString(STR_CLNDR_DATE));
	
	SetInlineItemActivation(&wgui_inline_items[1],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemDate(&wgui_inline_items[1],(U8*)g_clndr_cntx->DayBuf,(U8*)g_clndr_cntx->MonBuf,(U8*)g_clndr_cntx->YearBuf, ClndrDateEditorCallBack);
	RightJustifyInlineItem(&wgui_inline_items[1]);
	EnableInlineItemBoundary(&wgui_inline_items[1]);
	set_inline_date_boundary(2030, 1970, 12, 1);

}


/*****************************************************************************
* FUNCTION
*	HighlightClndrInputItem
* DESCRIPTION
*   Highlight handler of inline editor items.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightClndrInputItem(S32 nIndex)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(ClndrValidateInput, KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	ExitClndrJumpToDate
* DESCRIPTION
*   Exit function of Jump To screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitClndrJumpToDate(void)
{
	history_t   Scr; 
	U16 inputBufferSize;                //added for inline edit history
 
	S16 nHistory =0;
	Scr.scrnID = SCR_ID_CLNDR_JUMP;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntryClndrJumpToDate;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	GetCategory57Data ((U8*)Scr.inputBuffer);         //added for inline edit history
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	AddNHistory(Scr, inputBufferSize);                       //added for inline edit history

}


/*****************************************************************************
* FUNCTION
*	EntryClndrJumpToDate
* DESCRIPTION
*   Entry function of Jump To screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryClndrJumpToDate(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	U8* inputBuffer;
	U16 inputBufferSize;                //added for inline edit history
	    
	EntryNewScreen(SCR_ID_CLNDR_JUMP, ExitClndrJumpToDate, NULL, NULL);
	InitializeCategory57Screen();

	ClndrFillInilineStruct();
	
	RegisterHighlightHandler(HighlightClndrInputItem);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_CLNDR_JUMP);

	inputBuffer = GetCurrNInputBuffer(SCR_ID_CLNDR_JUMP, &inputBufferSize); 

	if(inputBuffer!=NULL)                                            //added for inline edit history
		SetCategory57Data(wgui_inline_items, 2, inputBuffer);//sets the data 

	DisableCategory57ScreenDone();

	ShowCategory57Screen(STR_CLNDR_JUMP_TO,GetRootTitleIcon(ORGANIZER_CALENDER_MENU),
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						2,NULL,wgui_inline_items,1,guiBuffer);
	
	SetKeyHandler(ClndrValidateInput, KEY_LSK, KEY_EVENT_UP);

	SetCategory57RightSoftkeyFunctions(ClndrValidateInput ,GoBackHistory);

}


/*****************************************************************************
* FUNCTION
*	HighlightJumpToDate
* DESCRIPTION
*   Highlight handler on lunar calendar option.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightJumpToDate(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetKeyHandler(EntryClndrJumpToDate, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryClndrJumpToDate, KEY_EVENT_UP); 
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}



#define CLNDR_LUNAR_CALENDAR
/*****************************************************************************
* FUNCTION                                                            
*	ClndrComputeSolarTerm
* DESCRIPTION                                                           
*   This function is to calculate the date of solar item in given year. 
* PARAMETERS
*	year 		IN		year in solar calendar, e.g. 2003
*	solar_index	IN		index of solar term, 0 = SC (Slight Cold), 1 = GC,...
* RETURNS
*	the solar day of given solar item
* GLOBALS AFFECTED
*   NONE
*****************************************************************************/
S16 ClndrComputeSolarTerm(S16 year, S16 month, S16 solar_index) 
{

/* adjust the solar term days for lunarinfo by feifan 8/1/2005*/

return solar_term_table[year-CLNDR_FIRST_NAVI_YEAR][solar_index];

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
}


/*****************************************************************************
* FUNCTION                                                            
*	ClndrComputeLunarInfo
* DESCRIPTION                                                           
*   This function is to convert solar date infomation to lunar date. 
* CALLS  
* PARAMETERS
*	DI 	IN/OUT	a struct contains solar date information
*								and lunar information will also be stored here
* RETURNS
*	None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void ClndrComputeLunarInfo(clndr_lnuar_date_info_struct *DI)
{
	U8	LeapDay;
	S16 StartMonth;
	S16 YearIndex;
	S16 AccumulateDay;
	S16 KanChiDay;
	S16 DateDiff;
	S16 IntercalaryMonth;
	S16 LunarDays;		
	S16 SolarDate1,SolarDate2,SolarDate3;
	S16 MonthIndex;
	S8 dummyString[CLNDR_TITLE_LEN];

	/* to check if the month out of boundary */
	StartMonth = DI->SolarMonth - 1;

	/* to calculate the number of days in Feb. this year */
	if(StartMonth > 1)
   	LeapDay = (U8)IsLeapYear(DI->SolarYear);
	else
	   LeapDay = 0;

	YearIndex = DI->SolarYear - CLNDR_TABLE_START_YEAR;
	
	/* to calculate weekday */
	AccumulateDay = (gClndrAccuDays[StartMonth] + LeapDay + DI->SolarDay);

	/* to calculate the Kan and Chin of today */    
	KanChiDay = AccumulateDay + gClndrLunarInfo[YearIndex].BaseKanChi;

	/* to check if today is earlier than the Chinese New Year day of this year */
	if(AccumulateDay <= gClndrLunarInfo[YearIndex].BaseDays)
	{
		YearIndex--;
		DI->LunarYear = DI->SolarYear - 1;
		LeapDay = (U8)IsLeapYear(DI->LunarYear);
		StartMonth += 12;
		AccumulateDay = (gClndrAccuDays[StartMonth] + LeapDay + DI->SolarDay);
	}
	else
	{
		DI->LunarYear = DI->SolarYear;
	}	

	/* to calculate the lunar month and day */
	IntercalaryMonth = gClndrLunarInfo[YearIndex].Intercalary;
	DateDiff = gClndrLunarInfo[YearIndex].BaseDays;
	for(MonthIndex=0; MonthIndex<13; MonthIndex++)
	{
		LunarDays = DateDiff + 29;
		if(gClndrLunarInfo[YearIndex].MonthDays & (0x01 << MonthIndex))
			LunarDays++;
		
		if (AccumulateDay <= LunarDays)
	        break;
	        
		DateDiff = LunarDays;
	}

	DI->LunarMonth = (MonthIndex + 1);
	DI->LunarDay = AccumulateDay - DateDiff;

	/* to adjust Lunar month if there is a intercalary month in this year */
	if(IntercalaryMonth != 0 && DI->LunarMonth > IntercalaryMonth)
		DI->LunarMonth--;

	if(DI->LunarMonth > 12)
		DI->LunarMonth -= 12;
	
	/* the unfortunate direction, age, and animal today */
	DI->Direction = KanChiDay % 4;
	DI->Age = (82-(KanChiDay % 60) + (DI->LunarYear - CLNDR_BASE))%60;	

	if(DI->Age <= 10)
		DI->Age+=60;

	DI->Animal = (KanChiDay+6)%12;

	/* to calculate the Kan and Chin of this year */
	if(DI->LunarYear == DI->SolarYear)
	{
		DI->KanYear = (DI->SolarYear-1900+36)%10;
		DI->ChiYear = (DI->SolarYear-1900+36)%12;

		/* to calculate the animal of this year */
		DI->YearAnimal = (DI->SolarYear +8)%12;
	}
	else
	{
		DI->KanYear = (DI->SolarYear-1900+36 - 1)%10;
		DI->ChiYear = (DI->SolarYear-1900+36 - 1)%12;

		/* to calculate the animal of this year */
		DI->YearAnimal = (DI->SolarYear +8 - 1)%12;		
	}

	/* to calculate the lunar day of solar term in this month */
	SolarDate1=ClndrComputeSolarTerm(DI->SolarYear,
							DI->SolarMonth,
							(S16)((DI->SolarMonth-1)*2));

	SolarDate2=ClndrComputeSolarTerm(DI->SolarYear,
							DI->SolarMonth,
							(S16)((DI->SolarMonth-1)*2+1));

	if(DI->SolarMonth < 12)
		SolarDate3=ClndrComputeSolarTerm(DI->SolarYear, (S16)(DI->SolarMonth+1), (S16)((DI->SolarMonth-1)*2+2));
	else
		SolarDate3=ClndrComputeSolarTerm((S16)(DI->SolarYear+1),(S16)1,(S16)(0));	

	if(DI->SolarDay < SolarDate1)
	{
		DI->SolarTerm = 0;
		DI->SolarDate = SolarDate1 - DI->SolarDay + DI->LunarDay;
	}
	else if(DI->SolarDay == SolarDate1)
	{
		DI->SolarTerm = 0;
		DI->SolarDate = 0;
	}
	else if(DI->SolarDay < SolarDate2)
	{
		DI->SolarTerm = 1;
		DI->SolarDate = SolarDate2 - DI->SolarDay + DI->LunarDay;
	}
	else if(DI->SolarDay == SolarDate2)
	{
		DI->SolarTerm = 1;
		DI->SolarDate = 0;
	}
	else
	{		
		DI->SolarTerm = 2;
		if(DI->SolarMonth == 2)
			DI->SolarDate = gDaysInMonth[DI->SolarMonth -1] - DI->SolarDay + SolarDate3 + DI->LunarDay + LeapDay;
		else
			DI->SolarDate = gDaysInMonth[DI->SolarMonth -1] - DI->SolarDay + SolarDate3 + DI->LunarDay;
	}

	if(gClndrLunarInfo[YearIndex].MonthDays & (0x01 << MonthIndex))
		LunarDays = 30;
	else
		LunarDays = 29;

	if(DI->SolarDate > LunarDays)
		DI->SolarDate -= LunarDays;

	/* to recode the information to present */
	
	/* Chinese year/month/day information */
	pfnUnicodeStrcpy(DI->TitleInfo1,GetString((U16)(STR_CAL_KAN1 + DI->KanYear)));
	pfnUnicodeStrcat(DI->TitleInfo1,GetString((U16)(STR_CAL_CHI1 + DI->ChiYear)));
	pfnUnicodeStrcat(DI->TitleInfo1,GetString(STR_CAL_YEAR));

	if(DI->LunarMonth < 10)
		pfnUnicodeStrcat(DI->TitleInfo1,GetString((U16)(STR_CAL_CHINESE_NO1 + DI->LunarMonth - 1)));
	else
	{
		pfnUnicodeStrcat(DI->TitleInfo1,GetString(STR_CAL_CHINESE_NO10));
		if(DI->LunarMonth > 10)
			pfnUnicodeStrcat(DI->TitleInfo1,GetString((U16)(STR_CAL_CHINESE_NO1 + (DI->LunarMonth%10-1))));
	}

	pfnUnicodeStrcat(DI->TitleInfo1,GetString(STR_CAL_MONTH));	

	if(DI->LunarDay < 10)
		pfnUnicodeStrcat(DI->TitleInfo1,GetString((U16)(STR_CAL_CHINESE_NO1 + DI->LunarDay - 1)));
	else if(DI->LunarDay < 20)
	{
		pfnUnicodeStrcat(DI->TitleInfo1,GetString(STR_CAL_CHINESE_NO10));
		if(DI->LunarDay > 10)
			pfnUnicodeStrcat(DI->TitleInfo1,GetString((U16)(STR_CAL_CHINESE_NO1 + DI->LunarDay%10-1)));
	}
	else if(DI->LunarDay < 30)	
	{
		pfnUnicodeStrcat(DI->TitleInfo1,GetString(STR_CAL_CHINESE_NO20));
		if(DI->LunarDay > 20)
			pfnUnicodeStrcat(DI->TitleInfo1,GetString((U16)(STR_CAL_CHINESE_NO1 + DI->LunarDay%10-1)));
	}
	else
	{
		pfnUnicodeStrcat(DI->TitleInfo1,GetString(STR_CAL_CHINESE_NO30));
		if(DI->LunarDay > 30)
			pfnUnicodeStrcat(DI->TitleInfo1,GetString((U16)(STR_CAL_CHINESE_NO1 + DI->LunarDay%10-1)));
	}	

	pfnUnicodeStrcat(DI->TitleInfo1,GetString(STR_CAL_DAY));
	
	/* Animal index */
	pfnUnicodeStrcpy(DI->TitleInfo2,GetString((U16)(STR_CAL_ANIMAL1 + DI->YearAnimal)));

	/* Solar Term */
	if(DI->SolarDate == 0)
		pfnUnicodeStrcpy(DI->MessageLine1,GetString((U16)(STR_CAL_SOLARTERM1+(DI->SolarMonth-1)*2+DI->SolarTerm)));
	else
	{	
		sprintf(dummyString,"%d",DI->SolarDate);
		AnsiiToUnicodeString(DI->MessageLine1,dummyString);	
			
		if(DI->SolarTerm==2 && DI->SolarMonth == 12)
			pfnUnicodeStrcat(DI->MessageLine1,GetString(STR_CAL_SOLARTERM1));
		else
			pfnUnicodeStrcat(DI->MessageLine1,GetString((U16)(STR_CAL_SOLARTERM1 + (DI->SolarMonth-1)*2+DI->SolarTerm)));
	}

	/* Unfortunate */
	pfnUnicodeStrcpy(DI->MessageLine2,GetString(STR_CAL_UNLUCKY));
	pfnUnicodeStrcat(DI->MessageLine2,GetString((U16)(STR_CAL_DIRECTION1 + DI->Direction)));

	/* Unlucky */	
	sprintf(DI->MessageLine3,"%d",DI->Age);
	AnsiiToUnicodeString(dummyString, DI->MessageLine3);
	pfnUnicodeStrcpy(DI->MessageLine3, GetString(STR_CAL_UNFORTUNATE));
	pfnUnicodeStrcat(DI->MessageLine3, GetString((U16)(STR_CAL_ANIMAL1 + DI->Animal)));
	pfnUnicodeStrcat(DI->MessageLine3, dummyString);
	
}


#endif
/*****************************************************************************
* FUNCTION
*	ClndrDateEditorCallBack
* DESCRIPTION
*   Call back funtion for inline date input.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ClndrDateEditorCallBack(U8 *DateStr, U8 *day,U8 *month,U8 *year)
{
	pfnUnicodeStrcpy((S8*)DateStr,(S8*)year);
	pfnUnicodeStrcat((S8*)DateStr,(S8*)"/");
	pfnUnicodeStrcat((S8*)DateStr,(S8*)month);
	pfnUnicodeStrcat((S8*)DateStr,(S8*)"/");
	pfnUnicodeStrcat((S8*)DateStr,(S8*)day);
}

#endif // #ifndef _MMI_CALENDAR_C

