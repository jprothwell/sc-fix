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
 *	ToDoListDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines global function prototypes of To Do List application.
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

/**************************************************************

	FILENAME	: ToDoListDef.h

  	PURPOSE		: Data Account application

 

	AUTHOR		: Arvind

	DATE		: 14th July,2003

**************************************************************/
#ifndef _MMI_TODOLISTDEF_H
#define _MMI_TODOLISTDEF_H
#include "mmi_features.h"
#ifdef __MMI_TODOLIST__

#include "mmi_data_types.h"
#include "alarmframeworkprot.h"

/* 
** Define
*/
#define MAX_TODO_NOTE_LEN				45
#define	MAX_TODO_LIST_NOTE				MAX_TODO_NOTE_LEN * ENCODING_LENGTH
#define MAX_INLINE_ITEM_TODO_LIST		9
#define MAX_LEN_LEAST_VAR				8
#define MAX_LEN_MOST_VAR				32
#define TDL_MAX_TIME_STR_LEN			14*ENCODING_LENGTH	/* 12:12 - 13:15 */
//Fix Bug#11787 for CS2324C by cong.li on 2009.05.31. Some languages need more bytes for the day of week. 
#define TDL_MAX_DAY_STR_LEN			22*ENCODING_LENGTH	/* 04/03/12 Thu */
#define MAX_MONTH_LEN					3*ENCODING_LENGTH
#define MAX_YEAR_LEN					5*ENCODING_LENGTH
#define TODO_LIST_ALARM_OPTION			2
#define TODO_LIST_REPEAT_OPTION			4
#define TODO_LIST_TASK_OPTION			5
#define	TDL_MAX_DAYS_PER_MONTH			31

#define	MAX_DAY_IN_WEEK					7

#define MAX_TODO_LIST_TEMPLATE			10

#define TDL_MAX_NOTE_PREFIX_LEN			30*ENCODING_LENGTH
#define MAX_TODO_LIST_REM_NOTE			MAX_TODO_LIST_NOTE + TDL_MAX_NOTE_PREFIX_LEN

#define NVRAM_TODO_LIST_RECORD_SIZE		860

/* 
** Typedef 
*/
typedef struct
{
	MYTIME				Time;			// This variable use to store the date, day and Time of an task
	U8					EndHour;		// This variable stote the End time in Hours of an Task ( Duration )
	U8					EndMin;			// This variable stote the End time in Min of an Task ( Duration )
	U8					Note[MAX_TODO_LIST_NOTE];		// Note of an Task
	U8					Alarm;				// Weather Alarm is On or Off
	U8					Repeat;				// Repeat values of Alarm
	U8					Days;				// Week days 
	U8					Present;		// Show the presence of other values	
} ToDoListNode ; // Total Size is 86 Bytes.

typedef struct
{
	S32	HilitedAlmState;	/* alarm on/off state in inline editor screen */
	S32	HilitedRepeat;		/* repeat criterion in inline editor screen */
	S32	HilitedDays;		/* day of week value in inline editor screen */
	history_t *HistoryPtr;	/* pointer to store history buffer when inserting template */
	MYTIME ClndrDate;		/* date/time information from calendar application */
	ToDoListNode TaskList[NUM_OF_TDL];
	S8	MonBuff[MAX_MONTH_LEN];	
	S8	DayBuff[MAX_MONTH_LEN];
	S8	YearBuff[MAX_YEAR_LEN];
	S8	HourBuff1[MAX_MONTH_LEN];	
	S8	MinBuff1[MAX_MONTH_LEN];
	S8	HourBuff2[MAX_MONTH_LEN];	
	S8	MinBuff2[MAX_MONTH_LEN];
	S8	RminderNote[MAX_TODO_LIST_REM_NOTE];
	S8	NoteBuff[MAX_TODO_LIST_NOTE];	
	U8	DateStrBuff[TDL_MAX_DAY_STR_LEN];
	U8	TimeStrBuff[TDL_MAX_TIME_STR_LEN];
	U8	SortedList[NUM_OF_TDL];
	U8	IsCalledByCalender;	/* if to do list is entered via Calendar app */
	U8	CurrSelectedTask;	/* index of current selected task */
	U8	CurrHiliteOption;	/* index of current higlighted option */
	U8	TotalListedTask;	/* total number of listed task */
	U8	SelectedTemplate;	/* index of current selected template */
	U8	IsListModified;
	U8	SelectedItemInView;	/* index of selected item in View screen */
	U8	CurrTaskIndex;		/* index of current selected task */	
	U8	IsDOWSet;
}tdl_context_struct;

typedef enum 
{
	REPEAT_ONCE = 0,
	REPEAT_EVERYDAY,
	REPEAT_DAYS,
	REPEAT_WEEKLY,
	REPEAT_MONTHLY
} TODO_LIST_REPEAT;

typedef enum
{
	TODO_LIST_ALARM_OFF = 0,
	TODO_LIST_ALARM_ON

}TODO_LIST_ALARM;

typedef enum 
{
	INLINE_DATE = 0,
	INLINE_TIME,
	INLINE_NOTE,
	INLINE_TASK,
	ININE_ALARM,
	INLINE_ON_OFF,
	INLINE_REPEAT,
	INLINE_DAY,
	INLINE_DAYS_OF_WEEK
} INLINE_ITEM_DISPLAY;

typedef enum 
{
	TODO_LIST_TASK_OPTION_VIEW=0,
	TODO_LIST_TASK_OPTION_ADD,
	TODO_LIST_TASK_OPTION_EDIT,
	TODO_LIST_TASK_OPTION_DELETE,
	TODO_LIST_TASK_OPTION_DELETE_ALL,
#if defined(__MMI_VCALENDAR__)
	TODO_FROM_FMGR,
	TODO_FROM_NETWORK,
#endif
	TODO_LIST_OPTION_TOTAL

} TDL_OPTION_ENUM;


/* 
** Extern Global Variable
*/
extern tdl_context_struct g_tdl_cntx;
/* 
** Extern Global Function
*/
extern void InitToDoListApp(void);
extern void DeInitToDoListApp(void);	/* currently unused */
extern void TDLShowSpecificDateList(MYTIME	*CalTime) ;
extern void TDLGetDaysToBeHilited(MYTIME mytime,U8 *daylist);
extern void TDLExitReminderScreen(void);
extern void EntryTDLReminder(void);

// local function
extern void HighlightTDLMenu(void);
extern void EntryTDLTaskList(void);
extern void HighlightTDLTaskList(S32 nIndex);

extern void EntryTDLTaskListOption(void);
extern void HighlightTDLTaskOption(S32 nIndex);

extern void EntryTDLDelete(void);
extern void EntryTDLDeleteAll(void);

extern void EntryTDLNoteOption(void);
extern void HighlightTDLNoteOption(S32 nIndex);

extern void EntryTDLTemplate(void);
extern void HighlightTDLTemplateIndex(S32 nIndex);
extern void TDLInsertTemplate(void);

extern void EntryTDLViewTask(void);
extern void ExitTDLViewTask(void);

extern void EntryTDLEditTask(void);
extern void ExitTDLEditTask(void);
extern void HighlightTDLInlineRepeat(S32 index);
extern void EditToDoListTask(void);
extern void HighlightTDLViewInlineItem(S32 index);

extern void HighlightTDLEditInlineItem(S32 index);
extern void EntryTDLEditNote(void);

extern void TDLShowAllList(void);

extern void EntryTDLReminder(void);
extern void ExitTDLReminder(void);
extern void ToDoListReminder(void *ReminderId);

extern void TDLGoBackInlineScreen(void);

extern void EntryTDLSaveConfirm(void );

extern void TDLInitInlineVariables(void);

extern void TDLFillInlineStruct(void);
extern void TDLFillInlineStructForView(void);

extern void TDLGoBack2History(void);

extern void TDLDateEditorCallBack(U8 *DateStr, U8 *day,U8 *month,U8 *year);
extern void TDLTimeEditorCallBack(U8* string_buffer,U8* hours_buffer1,U8* minutes_buffer1,U8* AM_PM_flag1,U8* hours_buffer2,U8* minutes_buffer2,U8* AM_PM_flag2);

extern void TDLWriteToNvram(void);
extern void TDLReadFromNvram(void);

extern void TDLGoToSelectedOption(void);

extern void TDLAddDefaultRecord(void);

extern void TDLDeleteOneTask(void);
extern void TDLDeleteAllTask(void);

extern void TDLShowPopupScreen(U16 Msg);

extern void TDLGotoListOrOptionScreen(void);

extern void TDLSaveTask(void);

extern void TDLMakeListOfAllTask(void);
extern void TDLMakeListOfSelectedDate(void);
extern void TDLSortTask(void);

extern U8 TDLIsTaskChanged(void);
extern void SetToDoListAlarm(U8 i);


extern void HighlightTdlView(void);
extern void HighlightTdlEdit(void);
extern void HighlightTdlDelete(void);
extern void HighlightTdlAdd(void);
extern void HighlightTdlDeleteAll(void);

extern void TdlExecView(void);
extern void TdlExecEdit(void);
extern void TdlExecDelete(void);
#ifdef __MMI_VCALENDAR__
extern U8 TdlInsertTask(ToDoListNode*, U8);
#endif
#endif
#endif /* _MMI_TODOLISTDEF_H */


