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
 *	ToDoList.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements To Do List application.
 *
 * Author:
 * -------
 * -------
 * * 修改记录：
 *       修改日期：20060512
 *       版 本 号：6205.v02
 *       修 改 人：苏世鹏
 *       修改内容：关闭include文件
 *------------------------------------------------------------------------------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: ToDoList.c

  	PURPOSE		: To Do List Application

 

	AUTHOR		: Arvind

	DATE		: 29th Oct, 2003

**************************************************************/
#ifndef _MMI_TODOLIST_C
#define _MMI_TODOLIST_C

#include "mmi_features.h"
#ifdef __MMI_TODOLIST__


/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "globalconstants.h"
#include "debuginitdef.h"
#include "custmenures.h"
#include "custdatares.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "timerevents.h"
#include "globaldefs.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "commonscreens.h"
#include "mainmenudef.h"
#include "todolistdef.h"
#include "todolistenum.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "organizerdef.h"
#include "sublcdhistorygprot.h"
#include "alarmgprot.h"
#include "datetimegprot.h"
#include "alarmframeworkprot.h"
#include "gpioinc.h"
#include "profilegprots.h"
#include "keybrd.h"
#include "wrappergprot.h"

#include "vcalendar.h"   
#include "vobjects.h"    
#include "simdetectiongexdcl.h"

#include "gbhmi.h" //added by zhoumn for the bug of editor full @2007/06/16

#undef __NEWSIMULATOR 
/*  Include: PS header file */
//... Add More PS header

/* 
** Define
*/


/* 
** Typedef 
*/


tdl_context_struct g_tdl_cntx;
/* 
** Local Variable
*/

// buffer used in inline editors */

#if 1//qiff for bug9630 & bug10532
/*gaosh Add Start For 6637 Ver: TBM780_T_20070913_1828  on 2007-9-14 14:48 */
static S8 edit_tdl_node_temp[MAX_TODO_NOTE_LEN*ENCODING_LENGTH];
/*gaosh Add End  For 6637 Ver: TBM780_T_20070913_1828  on 2007-9-14 14:48 */
#endif
/* 
** Local Function
*/

/* 
** Global Variable
*/
extern U8 gAlmDayOfWeek[];
extern S8 *gAlmFreqList[];		/* share global variable with alarm */
extern S8 *gAlmOnOffString[];	/* share global variable with alarm */
extern U8 gAlmDaysStates[];
extern wgui_inline_item wgui_inline_items[];

/* 
** Global Function
*/

/*****************************************************************************
* FUNCTION
*	InitToDoListApp
* DESCRIPTION
*   Set highlight handlers and retrieve data from NVRAM.
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void InitToDoListApp(void)
{
	SetHiliteHandler(ORGANIZER_TODOLIST_MENU,HighlightTDLMenu);


	SetHiliteHandler(MITEM_TODO_LIST_VIEW,HighlightTdlView);
	SetHiliteHandler(MITEM_TODO_LIST_ADD,HighlightTdlAdd);
	SetHiliteHandler(MITEM_TODO_LIST_EDIT,HighlightTdlEdit);
	SetHiliteHandler(MITEM_TODO_LIST_DELETE,HighlightTdlDelete);
	SetHiliteHandler(MITEM_TODO_LIST_DELETE_ALL,HighlightTdlDeleteAll);

	TDLReadFromNvram();
}


/*****************************************************************************
* FUNCTION
*	HighlightTDLMenu
* DESCRIPTION
*   Highlight handler of To Do List menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightTDLMenu(void)
{	
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(TDLShowAllList,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(TDLShowAllList,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */	
}


/*****************************************************************************
* FUNCTION
*	DeInitToDoListApp
* DESCRIPTION
*   Deinitialize To Do List application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void DeInitToDoListApp(void)
{

}

/*****************************************************************************
* FUNCTION
*	HighlightTDLTaskList
* DESCRIPTION
*   Highlight handler of to do list task list, store the index of selected task.
* PARAMETERS
*	nIndex	IN	index of selected task.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightTDLTaskList(S32 nIndex)
{
	g_tdl_cntx.CurrSelectedTask = (U8)nIndex;
}


/*****************************************************************************
* FUNCTION
*	TDLGetTotalItem
* DESCRIPTION
*   Get number of present task
* PARAMETERS
*	none.
* RETURNS
*	number of task
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
U8 TDLGetTotalItem(void)
{
	S32 i;
	U8 TotalItem = 0;
	
	for(i=0; i<NUM_OF_TDL; i++)
		if(g_tdl_cntx.TaskList[i].Present == 1)
			TotalItem++;

	return TotalItem;
}

/*****************************************************************************
* FUNCTION
*  EntryTDLTaskList
* DESCRIPTION
*   Entry function of To Do List application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void EntryTDLTaskList(void)
{
	U8* guiBuffer;
	S32 i;
	S32 TotalItem = 0;
	U16	nEmptyId = STR_TODO_LIST_EMPTY;
#if defined(__PROJECT_GALLITE_C01__)
	U16 ImageList[NUM_OF_TDL];
#endif
	EntryNewScreen(SCR_TDL_TASK_LIST, NULL, EntryTDLTaskList, NULL);

	if(g_tdl_cntx.IsListModified == TRUE) 
	{
		if (g_tdl_cntx.IsCalledByCalender) 	// Match the Date and list them
			TDLMakeListOfSelectedDate();
		else 								// Set the Current Date and list the task in descending order.
			TDLMakeListOfAllTask();

		g_tdl_cntx.IsListModified = FALSE;
	}
	guiBuffer = GetCurrGuiBuffer(SCR_TDL_TASK_LIST);

	SetParentHandler(0);

	RegisterHighlightHandler(HighlightTDLTaskList);	
	if(g_tdl_cntx.TotalListedTask == 0) 	/* display <Empty> */
	{
		/*dongwg Modify Start For 5971 Ver: TBM780  on 2007-8-1 13:45 */
		ShowCategory1Screen(/*CAL_STRING_VIEW*/STR_TODO_LIST_CAP_TASK_LIST, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
		/*dongwg Modify End  For 5971 Ver: TBM780  on 2007-8-1 13:45 */
						  STR_TODO_LIST_ADD, 0,
						  STR_GLOBAL_BACK, 0,
						  1,&nEmptyId,
						  0,guiBuffer);

		g_tdl_cntx.CurrHiliteOption = TODO_LIST_TASK_OPTION_ADD;
		SetLeftSoftkeyFunction(TdlExecEdit,KEY_EVENT_UP);
		SetKeyHandler(TdlExecEdit,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		/* if task full, disable LSK */
		if(g_tdl_cntx.IsCalledByCalender)
		{
			if(TDLGetTotalItem() == NUM_OF_TDL)
			{
				ChangeLeftSoftkey(0,0);		
				SetLeftSoftkeyFunction(NULL,KEY_EVENT_UP);
				SetKeyHandler(NULL,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
			}

		}
	}
	else 	/* display list of task */
	{
		/* construct task list */
		for(i=0;i<g_tdl_cntx.TotalListedTask;i++)
		{
			pfnUnicodeStrcpy((S8*)subMenuDataPtrs[TotalItem],(S8*)g_tdl_cntx.TaskList[g_tdl_cntx.SortedList[i]].Note);	
			TotalItem++;
#if defined(__PROJECT_GALLITE_C01__)
        		if(g_tdl_cntx.TaskList[g_tdl_cntx.SortedList[i]].Alarm == TODO_LIST_ALARM_ON) 
        			ImageList[i] = IMG_TODO_LIST_ALARM;// | 0x8000;
        		else
        			ImageList[i] = 0xFFFF;
#endif
		}
		/*dongwg Modify Start For 5971 Ver: TBM780  on 2007-8-1 13:46 */	
		ShowCategory84Screen(/*CAL_STRING_VIEW*/STR_TODO_LIST_CAP_TASK_LIST, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
		/*dongwg Modify End  For 5971 Ver: TBM780  on 2007-8-1 13:46 */
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  TotalItem,(U8**) subMenuDataPtrs,
					#if defined(__PROJECT_GALLITE_C01__)
						  (U16*)ImageList,0,
					#else
						  (U16*)gIndexIconsImageList,0,
					#endif
						  0,guiBuffer);

		SetLeftSoftkeyFunction(EntryTDLTaskListOption,KEY_EVENT_UP);			
		SetKeyHandler(EntryTDLTaskListOption,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	
	}

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	TdlExecView
* DESCRIPTION
*   Execute View option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TdlExecView(void)
{
	if (g_tdl_cntx.TotalListedTask != 0)
		g_tdl_cntx.CurrTaskIndex = g_tdl_cntx.SortedList[g_tdl_cntx.CurrSelectedTask];

	g_tdl_cntx.SelectedItemInView = 0;
	TDLInitInlineVariables();
	EntryTDLViewTask();
}


/*****************************************************************************
* FUNCTION
*	HighlightTdlView
* DESCRIPTION
*   Highlight handler of View option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightTdlView(void)
{
	g_tdl_cntx.CurrHiliteOption = TODO_LIST_TASK_OPTION_VIEW;
	SetLeftSoftkeyFunction(TdlExecView,KEY_EVENT_UP);
	SetKeyHandler(TdlExecView, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	TdlExecEdit
* DESCRIPTION
*   Execute Edit option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TdlExecEdit(void)
{
	if(g_tdl_cntx.TotalListedTask != 0)
		g_tdl_cntx.CurrTaskIndex = g_tdl_cntx.SortedList[g_tdl_cntx.CurrSelectedTask];

	g_tdl_cntx.IsDOWSet = 0;
	g_tdl_cntx.SelectedItemInView = 0;

	TDLInitInlineVariables();
	EntryTDLEditTask();
}


/*****************************************************************************
* FUNCTION
*	TdlExecDelete
* DESCRIPTION
*   Execute Delete option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TdlExecDelete(void)
{
	if(g_tdl_cntx.TotalListedTask != 0)
		g_tdl_cntx.CurrTaskIndex = g_tdl_cntx.SortedList[g_tdl_cntx.CurrSelectedTask];

	EntryTDLDelete();
}


/*****************************************************************************
* FUNCTION
*	HighlightTdlEdit
* DESCRIPTION
*   Highlight handler of Edit option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightTdlEdit(void)
{
	g_tdl_cntx.CurrHiliteOption = TODO_LIST_TASK_OPTION_EDIT;
	SetLeftSoftkeyFunction(TdlExecEdit,KEY_EVENT_UP);
	SetKeyHandler(TdlExecEdit, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightTdlAdd
* DESCRIPTION
*   Highlight handler of Add option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightTdlAdd(void)
{
	g_tdl_cntx.CurrHiliteOption = TODO_LIST_TASK_OPTION_ADD;
	SetLeftSoftkeyFunction(TdlExecEdit,KEY_EVENT_UP);
	SetKeyHandler(TdlExecEdit, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightTdlDelete
* DESCRIPTION
*   Highlight handler of Delete option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightTdlDelete(void)
{
	g_tdl_cntx.CurrHiliteOption = TODO_LIST_TASK_OPTION_DELETE;
	SetLeftSoftkeyFunction(TdlExecDelete, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightTdlDeleteAll
* DESCRIPTION
*   Highlight handler of Delete All option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightTdlDeleteAll(void)
{
	g_tdl_cntx.CurrHiliteOption = TODO_LIST_TASK_OPTION_DELETE_ALL;
	SetLeftSoftkeyFunction(EntryTDLDeleteAll, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	EntryTDLTaskListOption
* DESCRIPTION
*   Show option list
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryTDLTaskListOption(void)
{
	U8* guiBuffer;
	U16 nStrItemList[6]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;		/* Stores no of children in the submenu*/
	U16	MenuId;
	BOOL IsSimValid = MMI_FALSE;
	U16 i;

	EntryNewScreen(SCR_TDL_OPTION_LIST, NULL, EntryTDLTaskListOption, NULL);

	/* view, add, edit, delete, delete all */
	if(TDLGetTotalItem() < NUM_OF_TDL)
		MenuId = MITEM_TODO_LIST_TASK_OPTION_1;
	else /* view, edit, delete, delete all */
		MenuId = MITEM_TODO_LIST_TASK_OPTION_2;

	nNumofItem = GetNumOfChild(MenuId);

	GetSequenceStringIds(MenuId,nStrItemList);

	SetParentHandler(MenuId);

	guiBuffer = GetCurrGuiBuffer(SCR_TDL_OPTION_LIST);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].IsSimValid)
		{
			IsSimValid = MMI_TRUE;
			break;
		}
	}
	
#if defined(__MMI_VCALENDAR__) && defined(MMI_ON_HARDWARE_P)
	if(!IsSimValid
#ifdef __FLIGHT_MODE_SUPPORT__
	|| mmi_bootup_get_active_flight_mode() == 1
#endif
	)  
		nNumofItem--;
#endif

	ShowCategory15Screen(STR_TODO_LIST_CAP_OPTION, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightTDLViewInlineItem
* DESCRIPTION
*   Highlight handler of items in View screen
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightTDLViewInlineItem(S32 index)
{
	g_tdl_cntx.SelectedItemInView = (U8)index;
	
	ChangeLeftSoftkey(STR_TODO_LIST_LSK_EDIT,0);
	SetLeftSoftkeyFunction(EditToDoListTask,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	EntryTDLViewTask
* DESCRIPTION
*   Display task information using an inline editor screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryTDLViewTask(void)
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;

	U16 ToDoList_View_icon_list[MAX_INLINE_ITEM_TODO_LIST] = 
	{ 
		IMG_TODO_LIST_DATE, IMG_TODO_LIST_TIME, IMG_TODO_LIST_NOTE,
	  	0, IMG_TODO_LIST_ALARM, 0, IMG_TODO_LIST_REPEAT, 0, 0
	};

	EntryNewScreen(SCR_TDL_TASK_VIEW, ExitTDLViewTask, NULL, NULL);

	InitializeCategory57Screen();

	guiBuffer = GetCurrGuiBuffer(SCR_TDL_TASK_VIEW);
	
	inputBuffer = GetCurrNInputBuffer (SCR_TDL_TASK_VIEW, &inputBufferSize );    

	SetParentHandler(0);

	TDLFillInlineStructForView();

	RegisterHighlightHandler(HighlightTDLViewInlineItem);

	if(inputBuffer!=NULL)
	    SetCategory57Data(wgui_inline_items, MAX_INLINE_ITEM_TODO_LIST, inputBuffer);
 
	ShowCategory57Screen(STR_TODO_LIST_CAP_VIEW, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
						 STR_TODO_LIST_LSK_EDIT, IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 MAX_INLINE_ITEM_TODO_LIST, ToDoList_View_icon_list,
						 wgui_inline_items,
						 0,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	ExitTDLViewTask
* DESCRIPTION
*   Exit function of View screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void ExitTDLViewTask(void)
{
	history_t   ScrDA;	/* Variable to hold the history data*/
	S16 nHistory=0;
	U16 size;

	CloseCategory57Screen();
	ScrDA.scrnID = SCR_TDL_TASK_VIEW;
	ScrDA.entryFuncPtr = EntryTDLViewTask;

	pfnUnicodeStrcpy((S8*)ScrDA.inputBuffer,(S8*)&nHistory);
	GetCategoryHistory(ScrDA.guiBuffer);
	size = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*)ScrDA.inputBuffer );
	AddNHistory(ScrDA,size);	
}


/*****************************************************************************
* FUNCTION
*	EditToDoListTask
* DESCRIPTION
*   Edit to do list task from View screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EditToDoListTask(void)
{
	g_tdl_cntx.CurrHiliteOption = TODO_LIST_TASK_OPTION_EDIT;
	EntryTDLEditTask();
}


/*****************************************************************************
* FUNCTION
*	HighlightTDLEditInlineItem
* DESCRIPTION
*   Highlight handler of edit task screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void HighlightTDLEditInlineItem(S32 index)
{
	if(index == INLINE_TASK)
	{
//qiff for bug9630 & bug10532
		memset((U8 *)edit_tdl_node_temp, '\0', sizeof(edit_tdl_node_temp));
		pfnUnicodeStrcpy((S8 *)edit_tdl_node_temp, (S8*)g_tdl_cntx.NoteBuff);
		ChangeLeftSoftkey(STR_TODO_LIST_LSK_EDIT,0);
		SetLeftSoftkeyFunction(EntryTDLEditNote,KEY_EVENT_UP);
	}

#if defined(__MMI_VCALENDAR__) && defined(MMI_ON_HARDWARE_P)
	if(g_tdl_cntx.CurrHiliteOption == TODO_FROM_FMGR ||g_tdl_cntx.CurrHiliteOption ==  TODO_FROM_NETWORK)
	{
		ChangeRightSoftkey(STR_GLOBAL_DONE,0);
		SetRightSoftkeyFunction(EntryTDLSaveConfirm,KEY_EVENT_UP);	
	}
#endif
}


/*****************************************************************************
* FUNCTION
*	EntryTDLEditTask
* DESCRIPTION
*   Entry edit task screen of To Do List.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryTDLEditTask(void)
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
#if defined(__PROJECT_GALLITE_C01__)
	U16 ToDoList_View_icon_list[MAX_INLINE_ITEM_TODO_LIST] = 
	{
		IMG_TODO_LIST_DATE, IMG_TODO_LIST_TIME, IMG_TODO_LIST_NOTE,
	  	0xFFFF, IMG_TODO_LIST_ALARM, 0xFFFF, IMG_TODO_LIST_REPEAT, 0xFFFF, 0xFFFF
	};
#else
	U16 ToDoList_View_icon_list[MAX_INLINE_ITEM_TODO_LIST] = 
	{
		IMG_TODO_LIST_DATE, IMG_TODO_LIST_TIME, IMG_TODO_LIST_NOTE,
	  	0, IMG_TODO_LIST_ALARM, 0, IMG_TODO_LIST_REPEAT, 0, 0
	};
#endif
	EntryNewScreen(SCR_TDL_TASK_EDIT, ExitTDLEditTask, NULL, NULL);

	InitializeCategory57Screen();

	guiBuffer = GetCurrGuiBuffer(SCR_TDL_TASK_EDIT);

	inputBuffer = GetCurrNInputBuffer (SCR_TDL_TASK_EDIT, &inputBufferSize);

	SetParentHandler(0);

	TDLFillInlineStruct();

	RegisterHighlightHandler(HighlightTDLEditInlineItem);

	if(inputBuffer!=NULL)
		SetCategory57Data(wgui_inline_items, MAX_INLINE_ITEM_TODO_LIST, inputBuffer);

	if(TDLIsTaskChanged() && GetInlineDoneFlag(guiBuffer) == 0)
		SetInlineDoneFlag(guiBuffer);

	ShowCategory57Screen(STR_TODO_LIST_LSK_EDIT, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
						 STR_TODO_LIST_LSK_EDIT, IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 MAX_INLINE_ITEM_TODO_LIST, ToDoList_View_icon_list,
						 wgui_inline_items,
						 g_tdl_cntx.SelectedItemInView,guiBuffer);
#if 0
	GBSetInputboxLSKFunction(EntryTDLNoteOption);                      //add by yaosq 20061102
#endif
	SetCategory57RightSoftkeyFunctions(EntryTDLSaveConfirm,GoBackHistory);
#if defined(__MMI_VCALENDAR__) && defined(MMI_ON_HARDWARE_P)

	if(g_tdl_cntx.CurrHiliteOption == TODO_FROM_FMGR)
	{
		ChangeRightSoftkey(STR_GLOBAL_DONE, IMG_GLOBAL_BACK);
		SetRightSoftkeyFunction(EntryTDLSaveConfirm, KEY_EVENT_UP);
	}
#endif
}

/*****************************************************************************
* FUNCTION
*	ExitTDLEditTasks
* DESCRIPTION
*   Exit function of edit task screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void ExitTDLEditTask(void)
{
	history_t   ScrDA;	/* Variable to hold the history data*/
	S16 nHistory=0;
	U16 size;

	CloseCategory57Screen();

	ScrDA.scrnID = SCR_TDL_TASK_EDIT;
	ScrDA.entryFuncPtr = EntryTDLEditTask;

	pfnUnicodeStrcpy((S8*)ScrDA.inputBuffer,(S8*)&nHistory);
	GetCategoryHistory(ScrDA.guiBuffer);
	size = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*)ScrDA.inputBuffer );
	AddNHistory(ScrDA,size);
}


/*****************************************************************************
* FUNCTION
*	TDLGotoListOrOptionScreen
* DESCRIPTION
*   Go back to task list or option screen when giving up save.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLGotoListOrOptionScreen(void)
{
	if(g_tdl_cntx.TotalListedTask == 0)
		GoBackToHistory(SCR_TDL_TASK_LIST);
	else 
		GoBackToHistory(SCR_TDL_OPTION_LIST);
}


/*****************************************************************************
* FUNCTION
*	TDLSaveTask
* DESCRIPTION
*   Retrieve user input from inline editor screen and save to NVRAM.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLSaveTask(void)
{
	U8 i, j;
	U8 OldStatus;
	MYTIME currTime;
	TIME_EQUALITY equality;

	i = g_tdl_cntx.CurrTaskIndex;

	/* note */
	if (pfnUnicodeStrlen(g_tdl_cntx.NoteBuff) == 0) 
	{
		TDLShowPopupScreen(STR_TODO_LIST_EMPTY_NOTE_MSG);
		return;
	}

	if(LastDayOfMonth((U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.MonBuff),(U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.YearBuff)) < coolsand_UI_atoi((UI_string_type)g_tdl_cntx.DayBuff))
	{
		DisplayPopup((PU8)GetString(STR_TODO_LIST_WRONG_DATE_MSG) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
		return;
	}

	//dongwg add for Btime > Etime start 20070727
	if(((U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.HourBuff1) > (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.HourBuff2))\
		||(((U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.HourBuff1) == (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.HourBuff2))&&\
		(((U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.MinBuff1))>((U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.MinBuff2)))))
	{
		TDLShowPopupScreen(STR_TODO_LIST_WRONG_TIME_MSG);
		return;
	}
	//dongwg add for Btime > Etime end 20070727
	
	/* date */
	g_tdl_cntx.TaskList[i].Time.nDay = (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.DayBuff);
	g_tdl_cntx.TaskList[i].Time.nMonth = (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.MonBuff);
	g_tdl_cntx.TaskList[i].Time.nYear = (U16)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.YearBuff);
	g_tdl_cntx.TaskList[i].Time.DayIndex = DOW(g_tdl_cntx.TaskList[i].Time.nYear, g_tdl_cntx.TaskList[i].Time.nMonth, g_tdl_cntx.TaskList[i].Time.nDay);
	
	/* time */
	g_tdl_cntx.TaskList[i].Time.nHour = (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.HourBuff1);
	g_tdl_cntx.TaskList[i].Time.nMin = (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.MinBuff1);
	g_tdl_cntx.TaskList[i].EndHour = (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.HourBuff2);
	g_tdl_cntx.TaskList[i].EndMin = (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.MinBuff2);

	pfnUnicodeStrcpy((S8*)(g_tdl_cntx.TaskList[i].Note),g_tdl_cntx.NoteBuff);
	/* repeat */
	g_tdl_cntx.TaskList[i].Repeat = (U8)g_tdl_cntx.HilitedRepeat;
	g_tdl_cntx.TaskList[i].Days = 0;

	if(g_tdl_cntx.TaskList[i].Repeat == REPEAT_DAYS)
	{
		for(j=0; j<MAX_DAY_IN_WEEK; j++)
			if(gAlmDaysStates[j])
				g_tdl_cntx.TaskList[i].Days |= gAlmDayOfWeek[j];

		/* reset to once */
		if(g_tdl_cntx.TaskList[i].Days == 0)
			g_tdl_cntx.TaskList[i].Repeat = REPEAT_ONCE;
	}
	
	g_tdl_cntx.TaskList[i].Present = 1;

	/* alarm on/off */
	OldStatus = g_tdl_cntx.TaskList[i].Alarm;

	/* disable alarm state if already expired */
	GetDateTime(&currTime);
	equality = CompareTime(g_tdl_cntx.TaskList[i].Time, currTime, NULL);
	if(g_tdl_cntx.TaskList[i].Repeat == ONCE && equality != TIME_GREATER) /* already expired */
		g_tdl_cntx.HilitedAlmState = 0;
	
	g_tdl_cntx.TaskList[i].Alarm = (U8)g_tdl_cntx.HilitedAlmState;

	TDLWriteToNvram();

	/* alarm is canceled */
	if(OldStatus == TODO_LIST_ALARM_ON && g_tdl_cntx.TaskList[i].Alarm == TODO_LIST_ALARM_OFF)
		AlmCancelAlarm((U8)(i+ALM_TDL_START));
	else if(g_tdl_cntx.TaskList[i].Alarm == TODO_LIST_ALARM_ON)
		AlmSetAlarm((U8)(i+ALM_TDL_START));

	TDLShowPopupScreen(STR_GLOBAL_DONE);
}

/*****************************************************************************
* FUNCTION
*	TDLFillInlineStruct
* DESCRIPTION
*	Fill the inline data sturcture for edit screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLFillInlineStruct(void)
{
	S32 i;

	for (i=0;i<TODO_LIST_REPEAT_OPTION;i++) 
		gAlmFreqList[i] = GetString((U16)(STR_TODO_ONCE+i));

	gAlmOnOffString[0] = GetString(ALARM_OFF_STRING);
	gAlmOnOffString[1] = GetString(ALARM_ON_STRING);

	// To Read didnt indent the code from first column.
	SetInlineItemActivation(&(wgui_inline_items[INLINE_DATE]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemDate(&(wgui_inline_items[INLINE_DATE]),(U8*)g_tdl_cntx.DayBuff,(U8*)g_tdl_cntx.MonBuff,(U8*)g_tdl_cntx.YearBuff,TDLDateEditorCallBack);
	DisableInlineItemHighlight(&wgui_inline_items[INLINE_DATE]);
	set_inline_date_boundary(2030, 1970, 12, 1);

	SetInlineItemActivation(&(wgui_inline_items[INLINE_TIME]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemTimePeriod(&(wgui_inline_items[INLINE_TIME]),(U8*)g_tdl_cntx.HourBuff1, (U8*)g_tdl_cntx.MinBuff1, NULL,(U8*)g_tdl_cntx.HourBuff2, (U8*)g_tdl_cntx.MinBuff2, NULL, TDLTimeEditorCallBack);
	DisableInlineItemHighlight(&wgui_inline_items[INLINE_TIME]);

	SetInlineItemActivation(&(wgui_inline_items[INLINE_NOTE]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption(&(wgui_inline_items[INLINE_NOTE]),(U8*)GetString(STR_TODO_NOTE));

	SetInlineItemActivation(&(wgui_inline_items[INLINE_TASK]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemDisplayOnly(&(wgui_inline_items[INLINE_TASK]),(U8*)g_tdl_cntx.NoteBuff);

	SetInlineItemActivation(&(wgui_inline_items[ININE_ALARM]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption(&(wgui_inline_items[ININE_ALARM]),(U8*)GetString(STR_TODO_ALARM));

	SetInlineItemActivation(&(wgui_inline_items[INLINE_ON_OFF]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemSelect(&(wgui_inline_items[INLINE_ON_OFF]),(S32)TODO_LIST_ALARM_OPTION,(U8**)gAlmOnOffString,&g_tdl_cntx.HilitedAlmState);

	SetInlineItemActivation(&(wgui_inline_items[INLINE_REPEAT]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption(&(wgui_inline_items[INLINE_REPEAT]),(U8*)GetString(STR_TODO_REPEAT));

	SetInlineItemActivation(&(wgui_inline_items[INLINE_DAY]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemSelect(&(wgui_inline_items[INLINE_DAY]),(S32)TODO_LIST_REPEAT_OPTION,(U8**)gAlmFreqList,&g_tdl_cntx.HilitedRepeat);
	RegisterInlineSelectHighlightHandler(&(wgui_inline_items[INLINE_DAY]),HighlightTDLInlineRepeat);
	SetInlineItemFollowDown(&(wgui_inline_items[INLINE_DAY]),INLINE_DAY);

	SetInlineItemActivation(&(wgui_inline_items[INLINE_DAYS_OF_WEEK]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemDOWSelect(&(wgui_inline_items[INLINE_DAYS_OF_WEEK]),g_tdl_cntx.HilitedDays, gAlmDaysStates);

	if (g_tdl_cntx.HilitedRepeat == REPEAT_DAYS)
		EnableInlineItem(&(wgui_inline_items[INLINE_DAYS_OF_WEEK]),INLINE_DAYS_OF_WEEK);
	else
		DisableInlineItem(&(wgui_inline_items[INLINE_DAYS_OF_WEEK]),INLINE_DAYS_OF_WEEK);

}


/*****************************************************************************
* FUNCTION
*	HighlightTDLInlineRepeat
* DESCRIPTION
*	Highlight handler of inline selection item in edit task screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightTDLInlineRepeat(S32 index)
{
	S32 i,j;

	i = g_tdl_cntx.CurrTaskIndex;
	if(index == REPEAT_DAYS)
	{
		if (g_tdl_cntx.IsDOWSet == 0)
		{
			/* if the user did not select DOW yet */
			if(g_tdl_cntx.TaskList[i].Days == 0 )
			{
				memset(gAlmDaysStates,1,7);
				gAlmDaysStates[0] = 0;
				gAlmDaysStates[6] = 0;
			}
			else
			{
				for (j=0; j<MAX_DAY_IN_WEEK; j++)
				{
					if(g_tdl_cntx.TaskList[i].Days & gAlmDayOfWeek[j])		
						gAlmDaysStates[j] = 1;
					else
						gAlmDaysStates[j] = 0;
				}
			}
		}
		
		g_tdl_cntx.IsDOWSet = 1;
		EnableInlineItem(&(wgui_inline_items[INLINE_DAYS_OF_WEEK]),INLINE_DAYS_OF_WEEK);
	}
	else
	{
		g_tdl_cntx.IsDOWSet = 0;		
		memset(gAlmDaysStates,0,MAX_DAY_IN_WEEK);
		DisableInlineItem(&(wgui_inline_items[INLINE_DAYS_OF_WEEK]),INLINE_DAYS_OF_WEEK);
	}

	RedrawCategory57Screen();
}


/*****************************************************************************
* FUNCTION
*	TDLFillInlineStructForView
* DESCRIPTION
*	Fill the inline data sturcture for view screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLFillInlineStructForView(void)
{
	S32 i;
		
	for (i=0;i<TODO_LIST_REPEAT_OPTION;i++)
		gAlmFreqList[i] = GetString((U16)(STR_TODO_ONCE+i));

	gAlmOnOffString[0] = GetString(ALARM_OFF_STRING);
	gAlmOnOffString[1] = GetString(ALARM_ON_STRING);

	// To Read didnt indent the code from first column.
	SetInlineItemActivation(&(wgui_inline_items[INLINE_DATE]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	TDLDateEditorCallBack(g_tdl_cntx.DateStrBuff,(U8*)g_tdl_cntx.DayBuff,(U8*)g_tdl_cntx.MonBuff,(U8*)g_tdl_cntx.YearBuff);
	SetInlineItemDisplayOnly(&(wgui_inline_items[INLINE_DATE]),(U8*)g_tdl_cntx.DateStrBuff);

	SetInlineItemActivation((&wgui_inline_items[INLINE_TIME]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	TDLTimeEditorCallBack(g_tdl_cntx.TimeStrBuff,(U8*)g_tdl_cntx.HourBuff1,(U8*)g_tdl_cntx.MinBuff1,NULL,(U8*)g_tdl_cntx.HourBuff2,(U8*)g_tdl_cntx.MinBuff2,NULL);
	SetInlineItemDisplayOnly(&(wgui_inline_items[INLINE_TIME]),(U8*)g_tdl_cntx.TimeStrBuff);

	SetInlineItemActivation((&wgui_inline_items[INLINE_NOTE]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption((&wgui_inline_items[INLINE_NOTE]),(U8*)GetString(STR_TODO_NOTE));

	SetInlineItemActivation((&wgui_inline_items[INLINE_TASK]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemDisplayOnly((&wgui_inline_items[INLINE_TASK]),(U8*)g_tdl_cntx.NoteBuff);

	SetInlineItemActivation((&wgui_inline_items[ININE_ALARM]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption((&wgui_inline_items[ININE_ALARM]),(U8*)GetString(STR_TODO_ALARM));

	SetInlineItemActivation((&wgui_inline_items[INLINE_ON_OFF]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemDisplayOnly((&wgui_inline_items[INLINE_ON_OFF]),(U8*)gAlmOnOffString[g_tdl_cntx.HilitedAlmState]);

	SetInlineItemActivation((&wgui_inline_items[INLINE_REPEAT]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption((&wgui_inline_items[INLINE_REPEAT]),(U8*)GetString(STR_TODO_REPEAT));

	SetInlineItemActivation((&wgui_inline_items[INLINE_DAY]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemDisplayOnly((&wgui_inline_items[INLINE_DAY]),(U8*)gAlmFreqList[g_tdl_cntx.HilitedRepeat]);
	SetInlineItemFollowDown((&wgui_inline_items[INLINE_DAY]),INLINE_DAY);

	SetInlineItemActivation((&wgui_inline_items[INLINE_DAYS_OF_WEEK]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemDOWSelect((&wgui_inline_items[INLINE_DAYS_OF_WEEK]),(S32)INLINE_DAYS_OF_WEEK, gAlmDaysStates);
	DisableInlineItem((&wgui_inline_items[INLINE_DAYS_OF_WEEK]),INLINE_DAYS_OF_WEEK);

}


/*****************************************************************************
* FUNCTION
*	TDLDateEditorCallBack
* DESCRIPTION
*   Construct date string to to display (04/03/12 Thu)
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLDateEditorCallBack(U8 *DateStr, U8 *day,U8 *month,U8 *year)
{
	U8 mDay,mMonth;
	U16 mYear;
	U16 StrNum;
	S8 ModifiedDateStr[MAX_LEN_LEAST_VAR];

	mDay = (U8) coolsand_UI_atoi((UI_string_type)day);
	mMonth = (U8) coolsand_UI_atoi((UI_string_type)month);
	mYear = (U16) coolsand_UI_atoi((UI_string_type)year);

	/* to get the string of week day */
	StrNum = STR_TODO_SUN + DOW(mYear,mMonth,mDay);

	/* to get the last 2 digits of year */
	if (mYear >= 2000)
		mYear = mYear - 2000;
	else
		mYear = mYear - 1900;

	AlmGetStringValue(ModifiedDateStr,(U8)mYear);
	pfnUnicodeStrcpy((S8*)DateStr,(S8*)ModifiedDateStr);

	pfnUnicodeStrcat((S8*)DateStr,(S8*)"/");

	AlmGetStringValue(ModifiedDateStr,mMonth);
	pfnUnicodeStrcat((S8*)DateStr,ModifiedDateStr);
	
	pfnUnicodeStrcat((S8*)DateStr,(S8*)"/");
		
	AlmGetStringValue(ModifiedDateStr,mDay);
	pfnUnicodeStrcat((S8*)DateStr,(S8*)ModifiedDateStr);

	pfnUnicodeStrcat((S8*)DateStr,(S8*)" ");
	pfnUnicodeStrcat((S8*)DateStr,(S8*)GetString(StrNum));

}


/*****************************************************************************
* FUNCTION
*	TDLTimeEditorCallBack
* DESCRIPTION
*   Construct time string to to display (00:00 - 01:25)
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLTimeEditorCallBack(U8* string_buffer,U8* hours_buffer1,U8* minutes_buffer1,U8* AM_PM_flag1,U8* hours_buffer2,U8* minutes_buffer2,U8* AM_PM_flag2)
{
	S8 TimeH1[MAX_LEN_LEAST_VAR],TimeH2[MAX_LEN_LEAST_VAR],TimeM1[MAX_LEN_LEAST_VAR],TimeM2[MAX_LEN_LEAST_VAR];
	U8 TimeValue;

	TimeValue = (U8) coolsand_UI_atoi((UI_string_type)hours_buffer1);
	AlmGetStringValue(TimeH1,TimeValue);

	TimeValue = (U8) coolsand_UI_atoi((UI_string_type)minutes_buffer1);
	AlmGetStringValue(TimeM1,TimeValue);
	
	TimeValue = (U8) coolsand_UI_atoi((UI_string_type)hours_buffer2);
	AlmGetStringValue(TimeH2,TimeValue);
	
	TimeValue = (U8) coolsand_UI_atoi((UI_string_type)minutes_buffer2);
	AlmGetStringValue(TimeM2,TimeValue);

	pfnUnicodeStrcpy((S8*)string_buffer,TimeH1);
	pfnUnicodeStrcat((S8*)string_buffer,(S8*)":");
	pfnUnicodeStrcat((S8*)string_buffer,TimeM1);
	pfnUnicodeStrcat((S8*)string_buffer,(S8*)" ");
	pfnUnicodeStrcat((S8*)string_buffer,(S8*)"-");
	pfnUnicodeStrcat((S8*)string_buffer,(S8*)" ");
	pfnUnicodeStrcat((S8*)string_buffer,TimeH2);
	pfnUnicodeStrcat((S8*)string_buffer,(S8*)":");
	pfnUnicodeStrcat((S8*)string_buffer,(S8*)TimeM2);

}


/*****************************************************************************
* FUNCTION
*	TDLShowSpecificDateList
* DESCRIPTION
*   Entry function of To Do List from Calendar application.
*	Store the date information from Calendar and display to do list.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLShowSpecificDateList(MYTIME	*CalTime)
{
	g_tdl_cntx.IsCalledByCalender = TRUE;

	g_tdl_cntx.ClndrDate.nYear	= CalTime->nYear;
	g_tdl_cntx.ClndrDate.nMonth = CalTime->nMonth;
	g_tdl_cntx.ClndrDate.nDay	= CalTime->nDay;
	g_tdl_cntx.ClndrDate.nHour	= 23;
	g_tdl_cntx.ClndrDate.nMin	= 59;
	g_tdl_cntx.ClndrDate.nSec	= 59;
	g_tdl_cntx.ClndrDate.DayIndex = CalTime->DayIndex;

	g_tdl_cntx.IsListModified = TRUE;

	EntryTDLTaskList();
}


/*****************************************************************************
* FUNCTION
*	TDLShowSpecificDateList
* DESCRIPTION
*   Entry function of To Do List from Organizer
*	Display all task list.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLShowAllList(void)
{
	g_tdl_cntx.IsCalledByCalender = FALSE;
	g_tdl_cntx.IsListModified = TRUE;

	EntryTDLTaskList();
}


/*****************************************************************************
* FUNCTION
*	TDLReadFromNvram
* DESCRIPTION
*   Read data from nvram.
*	Intialize to default value if data is not intialized
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLReadFromNvram(void)
{
	S16	error;

	ReadRecord(NVRAM_EF_TODO_LIST_LID,(U16)1,g_tdl_cntx.TaskList,NVRAM_TODO_LIST_RECORD_SIZE, &error);

	if(g_tdl_cntx.TaskList[0].Present != 1 && g_tdl_cntx.TaskList[0].Present != 0) 
		memset(g_tdl_cntx.TaskList, 0, sizeof(g_tdl_cntx.TaskList));
}


/*****************************************************************************
* FUNCTION
*	TDLMakeListOfSelectedDate
* DESCRIPTION
*   To initialize task list of selected date from calendar application.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLMakeListOfSelectedDate(void)
{
	S16 i;
	U8 SelectedDay, Day;
	U8 IsTaskInPreviousDate;

	g_tdl_cntx.TotalListedTask = 0;

	memset(g_tdl_cntx.SortedList, 0, sizeof(g_tdl_cntx.SortedList));
	for(i=0;i<NUM_OF_TDL;i++)
	{
		if(g_tdl_cntx.TaskList[i].Present == 1)
		{
			/* display the task only when the starting day is smaller than selected day */
			if(FindMaxTime(&g_tdl_cntx.ClndrDate,&(g_tdl_cntx.TaskList[i].Time)))
				IsTaskInPreviousDate = TRUE;
			else
				IsTaskInPreviousDate = FALSE;

			switch(g_tdl_cntx.TaskList[i].Repeat)
			{
				case REPEAT_ONCE:	/* find exact match */
					if((g_tdl_cntx.TaskList[i].Time.nYear == g_tdl_cntx.ClndrDate.nYear) &&
						(g_tdl_cntx.TaskList[i].Time.nMonth == g_tdl_cntx.ClndrDate.nMonth) &&
						(g_tdl_cntx.TaskList[i].Time.nDay == g_tdl_cntx.ClndrDate.nDay))
					{
						g_tdl_cntx.SortedList[g_tdl_cntx.TotalListedTask] = (U8)i;
						g_tdl_cntx.TotalListedTask ++;
					}
					break;
					
				case REPEAT_EVERYDAY:	/* appear in each date */
					if(IsTaskInPreviousDate)
					{
						g_tdl_cntx.SortedList[g_tdl_cntx.TotalListedTask] = (U8)i;
						g_tdl_cntx.TotalListedTask++;
					}
					break;
					
				case REPEAT_DAYS:
					if(IsTaskInPreviousDate)
					{
						SelectedDay = DOW(g_tdl_cntx.ClndrDate.nYear, g_tdl_cntx.ClndrDate.nMonth,g_tdl_cntx.ClndrDate.nDay);

						for(Day=0; Day<MAX_DAY_IN_WEEK ;Day++)
							if((g_tdl_cntx.TaskList[i].Days & gAlmDayOfWeek[Day]) && (Day == SelectedDay)) 
							{
								g_tdl_cntx.SortedList[g_tdl_cntx.TotalListedTask] = (U8)i;
								g_tdl_cntx.TotalListedTask ++;
							}
					}
					break;
					
				case REPEAT_WEEKLY:
					if(IsTaskInPreviousDate) 
					{
						SelectedDay = DOW(g_tdl_cntx.ClndrDate.nYear, g_tdl_cntx.ClndrDate.nMonth,g_tdl_cntx.ClndrDate.nDay);

						Day = DOW(g_tdl_cntx.TaskList[i].Time.nYear, g_tdl_cntx.TaskList[i].Time.nMonth,g_tdl_cntx.TaskList[i].Time.nDay);

						if(SelectedDay == Day)
						{
							g_tdl_cntx.SortedList[g_tdl_cntx.TotalListedTask] = (U8)i;
							g_tdl_cntx.TotalListedTask++;
						}
					}
					break;

				case REPEAT_MONTHLY:
					if(IsTaskInPreviousDate)
					{
						if(g_tdl_cntx.ClndrDate.nDay == g_tdl_cntx.TaskList[i].Time.nDay) 
						{
							g_tdl_cntx.SortedList[g_tdl_cntx.TotalListedTask] = (U8)i;
							g_tdl_cntx.TotalListedTask++;
						}
					}
					break;
				default: 
					break;
			}
		} // if Present
	}

	TDLSortTask();

}


/*****************************************************************************
* FUNCTION
*	TDLSortTask
* DESCRIPTION
*   To sort the task list in decending time order.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLSortTask(void)
{
	U8 i, j, temp;	

	for(i=0; i<g_tdl_cntx.TotalListedTask; i++)
	{
		for(j=i; j<g_tdl_cntx.TotalListedTask; j++)
		{
			if(j>i && FindMaxTime(&g_tdl_cntx.TaskList[g_tdl_cntx.SortedList[j]].Time,&g_tdl_cntx.TaskList[g_tdl_cntx.SortedList[i]].Time))
			{
				temp = g_tdl_cntx.SortedList[i];
				g_tdl_cntx.SortedList[i] = g_tdl_cntx.SortedList[j];
				g_tdl_cntx.SortedList[j] = temp;
			}
		}
	}
}


/*****************************************************************************
* FUNCTION
*	TDLMakeListOfAllTask
* DESCRIPTION
*   To make the list of all tasks to be displayed.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLMakeListOfAllTask(void)
{
	U8 i;

	g_tdl_cntx.TotalListedTask = 0;

	memset(g_tdl_cntx.SortedList, 0, sizeof(g_tdl_cntx.SortedList));
	for(i=0;i<NUM_OF_TDL;i++)
	{
		if(g_tdl_cntx.TaskList[i].Present == 1) 
		{
			g_tdl_cntx.SortedList[g_tdl_cntx.TotalListedTask] = i;
			g_tdl_cntx.TotalListedTask++;
		}
	}

	TDLSortTask();

} // end of Fun


/*****************************************************************************
* FUNCTION
*	TDLWriteToNvram
* DESCRIPTION
*   Write todolist data to nvram.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLWriteToNvram(void)
{
	S16	error;

	g_tdl_cntx.IsListModified = TRUE;

	WriteRecord (NVRAM_EF_TODO_LIST_LID, (U16)1,(void*)g_tdl_cntx.TaskList, NVRAM_TODO_LIST_RECORD_SIZE,& error);
}


/*****************************************************************************
* FUNCTION
*	TDLInitInlineVariables
* DESCRIPTION
*	Inilialize the variables used in in inline editor screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLInitInlineVariables(void)
{
	S8 LocalStr[MAX_LEN_MOST_VAR];
	U8 i,j;

	if (g_tdl_cntx.CurrHiliteOption == TODO_LIST_TASK_OPTION_ADD)
		TDLAddDefaultRecord();

	i =	g_tdl_cntx.CurrTaskIndex;

	pfnUnicodeStrcpy(g_tdl_cntx.NoteBuff,(S8*)(g_tdl_cntx.TaskList[i].Note));

	AlmGetStringValue(g_tdl_cntx.MonBuff,g_tdl_cntx.TaskList[i].Time.nMonth);
	AlmGetStringValue(g_tdl_cntx.DayBuff,g_tdl_cntx.TaskList[i].Time.nDay);

	sprintf(LocalStr,"%d",g_tdl_cntx.TaskList[i].Time.nYear);
	AnsiiToUnicodeString(g_tdl_cntx.YearBuff,LocalStr);

	AlmGetStringValue(g_tdl_cntx.MinBuff1,g_tdl_cntx.TaskList[i].Time.nMin);
	AlmGetStringValue(g_tdl_cntx.HourBuff1,g_tdl_cntx.TaskList[i].Time.nHour);
	AlmGetStringValue(g_tdl_cntx.MinBuff2,g_tdl_cntx.TaskList[i].EndMin);
	AlmGetStringValue(g_tdl_cntx.HourBuff2,g_tdl_cntx.TaskList[i].EndHour);

	if(g_tdl_cntx.TaskList[i].Repeat == REPEAT_DAYS)
	{
		for (j=0; j<MAX_DAY_IN_WEEK; j++)
		{
			if(g_tdl_cntx.TaskList[i].Days & gAlmDayOfWeek[j])		
				gAlmDaysStates[j] = 1;
			else
				gAlmDaysStates[j] = 0;
		}

	}
	else
		memset(gAlmDaysStates,0,MAX_DAY_IN_WEEK);

	g_tdl_cntx.HilitedAlmState = g_tdl_cntx.TaskList[i].Alarm;

	g_tdl_cntx.HilitedRepeat = g_tdl_cntx.TaskList[i].Repeat;

}


/*****************************************************************************
* FUNCTION
*	TDLAddDefaultRecord
* DESCRIPTION
*	Find an empty solt of To Do List array, and initialize default value of the record.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLAddDefaultRecord(void)
{
	S32 i;

	for(i=0; i<NUM_OF_TDL; i++)
		if((g_tdl_cntx.TaskList[i].Present != 1))
			break;

	g_tdl_cntx.CurrTaskIndex = (U8)i ;

	memset(&g_tdl_cntx.TaskList[i],0,sizeof(ToDoListNode));

	if(g_tdl_cntx.IsCalledByCalender)
		memcpy(&g_tdl_cntx.TaskList[i].Time,&g_tdl_cntx.ClndrDate,sizeof(MYTIME));
	else
		GetDateTime(&g_tdl_cntx.TaskList[i].Time);

	g_tdl_cntx.TaskList[i].Time.nHour = 0;
	g_tdl_cntx.TaskList[i].Time.nMin = 0;
	g_tdl_cntx.TaskList[i].Time.nSec = 0;
	g_tdl_cntx.TaskList[i].Days = ALM_WEEK_DAYS;
}


/*****************************************************************************
* FUNCTION
*	EntryTDLDelete
* DESCRIPTION
*   Display confirmation screen for deleting one task.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryTDLDelete(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_TODO_LIST_DELETE_ONE_TASK_QUERY),
						IMG_GLOBAL_QUESTION,WARNING_TONE);

	SetLeftSoftkeyFunction(TDLDeleteOneTask,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/*****************************************************************************
* FUNCTION
*	TDLDeleteOneTask
* DESCRIPTION
*   Delete one task from To Do List
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLDeleteOneTask(void)
{
	U8 i;

	i = g_tdl_cntx.CurrTaskIndex;
	
	if (g_tdl_cntx.TaskList[i].Present == 1)
	{
		AlmCancelAlarm((U8)(i + ALM_TDL_START));

		memset(&g_tdl_cntx.TaskList[i], 0, sizeof(ToDoListNode));
		TDLWriteToNvram();

		TDLShowPopupScreen(STR_GLOBAL_DONE);
	}
	else
		DisplayPopup((U8*)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 0,UI_POPUP_NOTIFYDURATION_TIME,(U8)ERROR_TONE);

}


/*****************************************************************************
* FUNCTION
*	EntryTDLDeleteAll
* DESCRIPTION
*   Display confirmation screen for deleting all tasks.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryTDLDeleteAll(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_TODO_LIST_DELETE_ALL_TASK_QUERY),
						IMG_GLOBAL_QUESTION,WARNING_TONE);

	SetLeftSoftkeyFunction(TDLDeleteAllTask,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/*****************************************************************************
* FUNCTION
*	TDLDeleteAllTask
* DESCRIPTION
*   Delete all tasks from To Do List
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void TDLDeleteAllTask(void)
{
	S32 i, j;

	if(g_tdl_cntx.IsCalledByCalender)
	{
		for(i=0;i<g_tdl_cntx.TotalListedTask;i++)
		{
			j = g_tdl_cntx.SortedList[i];
			if(g_tdl_cntx.TaskList[j].Present == 0)
				continue;
			if(g_tdl_cntx.TaskList[j].Alarm == TODO_LIST_ALARM_ON)
				AlmCancelAlarm((U8)(j+ALM_TDL_START));

			memset(&g_tdl_cntx.TaskList[j],0,sizeof(ToDoListNode));
		}
	}
	else
	{
	for(i=0;i<NUM_OF_TDL;i++)
	{
			if(g_tdl_cntx.TaskList[i].Present == 0)
			continue;
	
			if(g_tdl_cntx.TaskList[i].Alarm == TODO_LIST_ALARM_ON)
			AlmCancelAlarm((U8)(i+ALM_TDL_START));

			memset(&g_tdl_cntx.TaskList[i],0,sizeof(ToDoListNode));
		}
	}

	TDLWriteToNvram();

	TDLShowPopupScreen(STR_GLOBAL_DONE);
}


/*****************************************************************************
* FUNCTION
*	TDLShowPopupScreen
* DESCRIPTION
*   Display popup screen with corresponding messages.
* PARAMETERS
*	Msg		IN		enum value of message type.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLShowPopupScreen(U16 Msg)
{
	U16 ScreenToShow;
	U16 ScreenImageId;
	ALL_TONE_ENUM ScreenTone;

	switch(Msg) 
	{
		case STR_TODO_LIST_EMPTY_NOTE_MSG:
		{
			ScreenImageId = IMG_GLOBAL_EMPTY;
			ScreenToShow = SCR_TDL_TASK_EDIT;
			ScreenTone = EMPTY_LIST_TONE;
			break;

		}
		//dongwg add start20070727
		case STR_TODO_LIST_WRONG_TIME_MSG:
		{
			ScreenImageId = IMG_GLOBAL_ERROR;
			ScreenToShow = SCR_TDL_TASK_EDIT;
			ScreenTone = ERROR_TONE;
			break;
		}
		//dongwg add end 20070727
		default: /* case STR_GLOBAL_DONE */
		{
			ScreenImageId = IMG_GLOBAL_ACTIVATED;
			ScreenToShow = SCR_TDL_TASK_LIST;
			ScreenTone = SUCCESS_TONE;
			break;
		}
	}
 
	DisplayPopup((U8*)GetString(Msg), ScreenImageId, 0,UI_POPUP_NOTIFYDURATION_TIME,(U8)ScreenTone);

#if defined(__MMI_VCALENDAR__) && defined(MMI_ON_HARDWARE_P)
	if(g_tdl_cntx.CurrHiliteOption == TODO_FROM_NETWORK)
	{
		if(Msg == STR_GLOBAL_DONE)
		{
	 		//mmi_vobj_clear_one_file_buffer();		//delete by liuxn 060616
	 		//mmi_vclndr_free_buffer();	//delete by liuxn 060616
			DeleteNScrId(SCR_ID_VOBJ_RECV_OBJECT);
			return;
		}
	}
	else if(g_tdl_cntx.CurrHiliteOption == TODO_FROM_FMGR)
	{
		if(Msg == STR_GLOBAL_DONE)
		{
			DeleteScreenIfPresent(SCR_TDL_TASK_EDIT);
			return;
		}
	}
#endif

	DeleteUptoScrID(ScreenToShow);
}


/*****************************************************************************
* FUNCTION
*	EntryTDLReminder
* DESCRIPTION
*   Display reminder screen when TDL alarm expires.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
/*dongwg Add Start For 6138 Ver: TBM780  on 2007-8-24 16:51 */
extern void EntryAlmPowerOnConfirmation(void);
/*dongwg Add End  For 6138 Ver: TBM780  on 2007-8-24 16:51 */
void EntryTDLReminder(void)
{
	S8 DateString[MAX_LEN_MOST_VAR*2];
	S8 TimeString[MAX_LEN_MOST_VAR*2];
	U8 i;
	S32 NoteLen;
	U8* guiBuffer;

	MYTIME EndTime;

	StopTimer(ALARM_TIMER_WAIT_FOR_CALL);
	/*dongwg Add Start For 6393 Ver: TBM780  on 2007-9-10 11:23 */
	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
	/*dongwg Add End  For 6393 Ver: TBM780  on 2007-9-10 11:23 */
	i = (g_alm_frm_cntx.CurrAlmIndex - ALM_TDL_START);
	
	if (g_tdl_cntx.TaskList[i].Present == 0 || i > NUM_OF_TDL) 
		return;

	if(g_tdl_cntx.TaskList[i].Repeat == ONCE)	/* turn off alarm */
	{
		g_tdl_cntx.TaskList[i].Alarm = 0;
		TDLWriteToNvram();
	}

/*dongwg Del Start For 6138 Ver: TBM780  on 2007-8-24 16:27 */
	#if 0
	if(AlmIsPowerOffMode())
		return;
	#endif
/*dongwg Del End  For 6138 Ver: TBM780  on 2007-8-24 16:27 */
	
	/*dongwg Modify Start For 6060 Ver: TBM780  on 2007-8-3 12:53 */
	TurnOnBacklight(1);
	//TurnOnBacklight(0);
	/*dongwg Modify End  For 6060 Ver: TBM780  on 2007-8-3 12:53 */
	
	EntryNewScreen(SCR_TDL_REMINDER, ExitTDLReminder, EntryTDLReminder, NULL);
	mdi_audio_suspend_background_play();
	AlmPlayAlarmTone();
	
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);

	ClearKeyEvents();

	/*dongwg Del Start For 6060 Ver: TBM780  on 2007-8-3 12:52 */
	#if 0
	TurnOnBacklight(1);
	#endif
	/*dongwg Del End  For 6060 Ver: TBM780  on 2007-8-3 12:52 */
	
	guiBuffer = GetCurrGuiBuffer(SCR_TDL_REMINDER);

	SetParentHandler(0);

	EndTime.nHour = g_tdl_cntx.TaskList[i].EndHour;
	EndTime.nMin = g_tdl_cntx.TaskList[i].EndMin;

	time_string(&g_tdl_cntx.TaskList[i].Time,(UI_string_type)DateString,DT_IDLE_SCREEN);
	time_string(&EndTime,(UI_string_type)TimeString,DT_IDLE_SCREEN);

	pfnUnicodeStrcpy(g_tdl_cntx.RminderNote,GetString(STR_TODO_LIST_TIME));
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,":");
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,"\n");
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,DateString);
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,"~");
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,TimeString);
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,"\n");	
	//luanyy modify for cs621 start
	//pfnUnicodeStrcat(g_tdl_cntx.RminderNote,GetString(STR_TODO_LIST_CAP_NOTE));
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,GetString(STR_TODO_NOTE));
	//luanyy modify for cs621 end
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,":");
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,"\n");
						
	pfnUnicodeStrcat(g_tdl_cntx.RminderNote,(S8*)g_tdl_cntx.TaskList[i].Note);

	NoteLen = pfnUnicodeStrlen(g_tdl_cntx.RminderNote);

	ShowCategory74Screen(STR_TODO_LIST_CAP_REMINDER, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						0 ,0, (U8*)g_tdl_cntx.RminderNote, NoteLen, guiBuffer);

	ForceSubLCDScreen(EntryAlmSubLcdScreen);
	/*dongwg Add Start For 6138 Ver: TBM780  on 2007-8-24 16:43 */
	if(AlmIsPowerOffMode())
	{
		SetLeftSoftkeyFunction(EntryAlmPowerOnConfirmation,KEY_EVENT_UP);
		SetKeyHandler(EntryAlmPowerOnConfirmation, KEY_END, KEY_EVENT_DOWN);
	}
	else
	/*dongwg Add End  For 6138 Ver: TBM780  on 2007-8-24 16:43 */
	{
		SetLeftSoftkeyFunction(TDLExitReminderScreen,KEY_EVENT_UP);
		SetKeyHandler(TDLExitReminderScreen, KEY_END, KEY_EVENT_DOWN);
	}
}


/*****************************************************************************
* FUNCTION
*	ExitTDLReminder
* DESCRIPTION
*   Exit function of reminder screen, stop tone and add screen to history.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void ExitTDLReminder(void)
{
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
	AlmsStopAlarmTone();
	mdi_audio_resume_background_play();
}


/*****************************************************************************
* FUNCTION
*	TDLExitReminderScreen
* DESCRIPTION
*   Exit To Do List reminder screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void TDLExitIndScreen(void)
{
	if(GetExitScrnID() == SCR_TDL_REMINDER)
		GoBackBothLCDHistory();
	else
	{
		DeleteScreenIfPresent(SCR_TDL_REMINDER);
		#ifdef __MMI_SUBLCD__
		   if(IsSubLCDScreenPresent(SCR_ID_ALM_SUB_INDICATION))
   			DeleteSubLCDScreen(SCR_ID_ALM_SUB_INDICATION);
		   else
		      GoBackSubLCDHistory();
		#endif
	}
}


/*****************************************************************************
* FUNCTION
*	TDLExitReminderScreen
* DESCRIPTION
*   Exit from To Do List Reminder Screen.
*	Decrease the Alarm id and Go To Previous Screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
/*dongwg Add Start For 6142 Ver: TBM780  on 2007-8-27 18:12 */
extern void AlmstopVibratorThenRing(void);
/*dongwg Add End  For 6142 Ver: TBM780  on 2007-8-27 18:12 */
void TDLExitReminderScreen(void)
{
	//TurnOffBacklight();		//delete by liuxn 060616
	/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	//StopMyTimer(ALARM_ALERT_NOTIFYDURATION_TIMER);
	coolsand_UI_cancel_timer(AlmHandleAlarmTimeout);
	/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	/*dongwg Add Start For 6142 Ver: TBM780  on 2007-8-27 17:56 */
	coolsand_UI_cancel_timer(AlmstopVibratorThenRing);
	/*dongwg Add End  For 6142 Ver: TBM780  on 2007-8-27 17:56 */
	g_alm_frm_cntx.CurrAlmIndex = 0xff;
	
	/*dongwg Add Start For 6138 Ver: TBM780  on 2007-8-27 18:5 */
	if(g_alm_frm_cntx.IsPwronAlarm == FALSE)
	{	
		TDLExitIndScreen();	
		if(g_alm_frm_cntx.IsExitByTimer == FALSE)
			AlmExecPwrOnAlarm();
	}
	else 
	{
		EntryAlmPowerOnConfirmation();
	}
	/*dongwg Add End  For 6138 Ver: TBM780  on 2007-8-27 18:5 */
/*dongwg Del Start For 6138 Ver: TBM780  on 2007-8-27 18:5 */
	#if 0
	TDLExitIndScreen();	
	if(g_alm_frm_cntx.IsExitByTimer == FALSE)
		AlmExecPwrOnAlarm();
	#endif
/*dongwg Del End  For 6138 Ver: TBM780  on 2007-8-27 18:5 */
}


/*****************************************************************************
* FUNCTION
*	EntryTDLEditNote
* DESCRIPTION
*   Full screen editor to edit note.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryTDLEditNote(void)
{
	U8* guiBuffer;

	EntryNewScreen(SCR_TDL_EDIT_NOTE, NULL, EntryTDLEditNote, NULL);

	SetParentHandler(0);

	if(g_tdl_cntx.HistoryPtr)	
		guiBuffer = g_tdl_cntx.HistoryPtr->guiBuffer;
	else 
		guiBuffer = GetCurrGuiBuffer(SCR_TDL_EDIT_NOTE);

	#if 1 //qiff for bug9630 & bug10532
	/*gaosh Add Start For 6637 Ver: TBM780_T_20070913_1828  on 2007-9-14 14:46 */
	//memset((U8 *)edit_tdl_node_temp, '\0', MAX_TODO_NOTE_LEN);
	
	ShowCategory5Screen(STR_TODO_NOTE, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),//STR_TODO_LIST_CAP_NOTE-->STR_TODO_NOTE   备忘录备注编辑窗口标题有浏览---> 备注modified by yaosq 2007.04.05
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, //  |INPUT_TYPE_USE_ENCODING_BASED_LENGTH
						  (U8*)edit_tdl_node_temp,
						  MAX_TODO_NOTE_LEN, guiBuffer);
	/*gaosh Add End  For 6637 Ver: TBM780_T_20070913_1828  on 2007-9-14 14:46 */

	#endif

	if(g_tdl_cntx.HistoryPtr)
	{
		OslMfree(g_tdl_cntx.HistoryPtr);
		g_tdl_cntx.HistoryPtr = NULL;
	}
#if defined(__MMI_GB__)
	SetLeftSoftkeyFunction(EntryTDLNoteOption,KEY_EVENT_UP);
	GBSetInputboxLSKFunction(EntryTDLNoteOption);
#endif
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	TDLGoBackEditorScreen
* DESCRIPTION
*   Go back to full screen editor.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLGoBackEditorScreen(void)
{
	GoBackToHistory(SCR_TDL_EDIT_NOTE);
}


/*****************************************************************************
* FUNCTION
*	TDLGoBackInlineScreen
* DESCRIPTION
*   Go back to inline editor screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLGoBackInlineScreen(void)
{
	#if 1//qiff for bug9630 & bug10532
	/*gaosh Add Start For 6637 Ver: TBM780_T_20070913_1828  on 2007-9-14 14:47 */
	pfnUnicodeStrcpy( (S8*)g_tdl_cntx.NoteBuff,(S8 *)edit_tdl_node_temp);
	/*gaosh Add End  For 6637 Ver: TBM780_T_20070913_1828  on 2007-9-14 14:47 */
	#endif
	GoBackToHistory(SCR_TDL_TASK_EDIT);
}

/*****************************************************************************
* FUNCTION
*	HighlightTDLNoteOption
* DESCRIPTION
*   Highlight handler of note options.
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightTDLNoteOption(S32 nIndex)
{
	switch (nIndex) 
	{
		case 0:
			SetLeftSoftkeyFunction(TDLGoBackInlineScreen,KEY_EVENT_UP);
			break;
		case 1:
			SetLeftSoftkeyFunction(EntryTDLTemplate,KEY_EVENT_UP);
			break;
		case 2:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP);
			RegisterInputMethodScreenCloseFunction(TDLGoBackEditorScreen);
			break;
		default:
			break;
	}
}

/*****************************************************************************
* FUNCTION
*	EntryTDLNoteOption
* DESCRIPTION
*   Display list of options for editing note
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryTDLNoteOption(void)
{
	U16 nStrItemList[3]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;		/* Stores no of children in the submenu*/
	U8* guiBuffer;		/* Buffer holding history data */

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_TDL_NOTE_OPTION, NULL, EntryTDLNoteOption, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_TDL_NOTE_OPTION);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MITEM_TODO_LIST_NOTE_OPTION);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MITEM_TODO_LIST_NOTE_OPTION,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MITEM_TODO_LIST_NOTE_OPTION);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(HighlightTDLNoteOption);

	/* 8 Display Category Screen */
	ShowCategory15Screen(STR_TODO_LIST_CAP_OPTION, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	/* 10. Register Exit handler */
}


/*****************************************************************************
* FUNCTION
*	HighlightTDLTemplateIndex
* DESCRIPTION
*   Store index of selected template
* PARAMETERS
*	nIndex	IN	index of selected template.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightTDLTemplateIndex(S32 nIndex)
{
	g_tdl_cntx.SelectedTemplate = (U8)nIndex;
}


/*****************************************************************************
* FUNCTION
*	EntryTDLTemplate
* DESCRIPTION
*   Display list of template
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryTDLTemplate(void)
{
	U16 nStrItemList[MAX_TODO_LIST_TEMPLATE]; /* Stores the strings id */
	U8* guiBuffer;       /* Buffer holding history data */
	S32 j;

	EntryNewScreen(SCR_TDL_LIST_TEMPLATE, NULL, EntryTDLTemplate, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_TDL_LIST_TEMPLATE);

	SetParentHandler(MITEM_TODO_LIST_NOTE_OPTION);

	RegisterHighlightHandler(HighlightTDLTemplateIndex);

	for (j=0; j<MAX_TODO_LIST_TEMPLATE; j++)
		nStrItemList[j] = (U16)(STR_TODO_TEMPLATE_1 + j);

	ShowCategory15Screen(STR_TODO_LIST_USE_TEMPLATE, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_TODO_LIST_TEMPLATE, nStrItemList,
						  (U16*)gIndexIconsImageList,LIST_MENU,
						  0, guiBuffer);

	SetLeftSoftkeyFunction(TDLInsertTemplate,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	TDLInsertTemplate
* DESCRIPTION
*   Inssert selected template into buffer and go back to editor screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void TDLInsertTemplate(void)
{
	S8 Tempbuffer[MAX_TODO_LIST_NOTE];
	S32 TemplateLen;

	if(g_tdl_cntx.HistoryPtr)
		OslMfree(g_tdl_cntx.HistoryPtr);
	
	g_tdl_cntx.HistoryPtr = OslMalloc(sizeof(history_t));
	
	TemplateLen = pfnUnicodeStrlen(GetString((U16)(STR_TODO_TEMPLATE_1+ g_tdl_cntx.SelectedTemplate)));
	
	pfnUnicodeStrncpy(Tempbuffer,
			GetString((U16)(STR_TODO_TEMPLATE_1+ g_tdl_cntx.SelectedTemplate)),
			TemplateLen);

	GetHistory(SCR_TDL_EDIT_NOTE, g_tdl_cntx.HistoryPtr);

	AppendCategory5String(INPUT_TYPE_ALPHANUMERIC_SENTENCECASE , // | INPUT_TYPE_USE_ENCODING_BASED_LENGTH
						(U8*)edit_tdl_node_temp,  //qiff for bug9630 & bug10532
						MAX_TODO_NOTE_LEN, (U8*) Tempbuffer,
						g_tdl_cntx.HistoryPtr->guiBuffer) ;

	GoBacknHistory(1);
}


/*****************************************************************************
* FUNCTION
*	EntryTDLSaveConfirm
* DESCRIPTION
*   Confirmation screen for saving To Do List
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void EntryTDLSaveConfirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_TODO_LIST_SAVE_TASK_QUERY),
						IMG_GLOBAL_QUESTION,WARNING_TONE);

	SetLeftSoftkeyFunction(TDLSaveTask,KEY_EVENT_UP);

#if defined(__MMI_VCALENDAR__) && defined(MMI_ON_HARDWARE_P)
	if(g_tdl_cntx.CurrHiliteOption == TODO_FROM_NETWORK)
		SetRightSoftkeyFunction(mmi_vclndr_goback_1_history/*mmi_vobj_abort_object*/, KEY_EVENT_UP);
	else if(g_tdl_cntx.CurrHiliteOption == TODO_FROM_FMGR)
		SetRightSoftkeyFunction(mmi_vclndr_goback_1_history, KEY_EVENT_UP);
	else	
#endif
	SetRightSoftkeyFunction(TDLGotoListOrOptionScreen,KEY_EVENT_UP);

}


/*****************************************************************************
* FUNCTION
*	TDLIsTaskChanged
* DESCRIPTION
*   To verify is task information changed in edit task screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 TDLIsTaskChanged(void)
{
	U8 i;

	i =	g_tdl_cntx.CurrTaskIndex;

	if (g_tdl_cntx.TaskList[i].Time.nDay != (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.DayBuff))
		return TRUE;

	if (g_tdl_cntx.TaskList[i].Time.nMonth !=(U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.MonBuff))
		return TRUE;

	if (g_tdl_cntx.TaskList[i].Time.nYear != (U16)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.YearBuff))
		return TRUE;

	if(g_tdl_cntx.TaskList[i].Time.nHour != (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.HourBuff1))
		return TRUE;

	if(g_tdl_cntx.TaskList[i].Time.nMin != (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.MinBuff1))
		return TRUE;

	if(g_tdl_cntx.TaskList[i].EndHour != (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.HourBuff2))
		return TRUE;

	if(g_tdl_cntx.TaskList[i].EndMin != (U8)coolsand_UI_atoi((UI_string_type)g_tdl_cntx.MinBuff2))
		return TRUE;

	if ((U8) g_tdl_cntx.HilitedAlmState != (U8) g_tdl_cntx.TaskList[i].Alarm)
		return TRUE;

	if ((U8) g_tdl_cntx.HilitedRepeat != (U8) g_tdl_cntx.TaskList[i].Repeat)
		return TRUE;

	if (memcmp(g_tdl_cntx.NoteBuff,g_tdl_cntx.TaskList[i].Note,MAX_TODO_LIST_NOTE) != 0)
		return TRUE;

	return FALSE;
}


/*****************************************************************************
* FUNCTION
*	TDLGetDaysToBeHilited
* DESCRIPTION
*   Get list of days which a task is associated to.
* PARAMETERS
*	SelectedDate	IN		date to be calculated.
*	daylist			IN/OUT	array to store the result.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void TDLGetDaysToBeHilited(MYTIME SelectedDate,U8 *daylist)
{
	S32 i,j;
	U8 index =0;
	U8 TaskInDay[MAX_LEN_MOST_VAR];
	U8 NextDay = 0;
	U8 IsInSameMonth;
	U8 FirstDay = 0;

	memset(daylist,0,MAX_LEN_MOST_VAR);
	memset(TaskInDay,0,MAX_LEN_MOST_VAR);

	for(i=0;i<NUM_OF_TDL;i++)
	{
		if(*daylist == TDL_MAX_DAYS_PER_MONTH)
			return;

		if(g_tdl_cntx.TaskList[i].Present == 1)
		{
			if((SelectedDate.nMonth == g_tdl_cntx.TaskList[i].Time.nMonth) && (SelectedDate.nYear == g_tdl_cntx.TaskList[i].Time.nYear ))
				IsInSameMonth = TRUE;
			else
				IsInSameMonth = FALSE;

			switch (g_tdl_cntx.TaskList[i].Repeat) 
			{
				case REPEAT_ONCE:
					if (IsInSameMonth == TRUE)
						TaskInDay[g_tdl_cntx.TaskList[i].Time.nDay] = 1;
					break;
					
				case REPEAT_EVERYDAY:
					if (IsInSameMonth == TRUE)
					{
						for (j=g_tdl_cntx.TaskList[i].Time.nDay; j<=TDL_MAX_DAYS_PER_MONTH;j++)
							TaskInDay[j] = 1;
					}
					else if(FindMaxTime(&SelectedDate,&(g_tdl_cntx.TaskList[i].Time)))
					{
						*daylist = TDL_MAX_DAYS_PER_MONTH;
					}
					break;
				case REPEAT_DAYS:
					if(IsInSameMonth == TRUE)
					{
						FirstDay = DOW(g_tdl_cntx.TaskList[i].Time.nYear, g_tdl_cntx.TaskList[i].Time.nMonth,g_tdl_cntx.TaskList[i].Time.nDay);
						for(j=0; j<MAX_DAY_IN_WEEK; j++)
						{
							if(g_tdl_cntx.TaskList[i].Days & gAlmDayOfWeek[j])
							{
								if(FirstDay == j)
									NextDay = g_tdl_cntx.TaskList[i].Time.nDay;
								else if(FirstDay < j)
									NextDay = g_tdl_cntx.TaskList[i].Time.nDay + j - FirstDay;
								else
									NextDay = g_tdl_cntx.TaskList[i].Time.nDay + j - FirstDay + MAX_DAY_IN_WEEK ;


								while(NextDay <= TDL_MAX_DAYS_PER_MONTH) 
								{
									TaskInDay[NextDay] = 1;
									NextDay = NextDay+MAX_DAY_IN_WEEK;
								} 
							}
						}
					}
					else if(FindMaxTime(&SelectedDate,&(g_tdl_cntx.TaskList[i].Time)))
					{
						FirstDay = DOW(SelectedDate.nYear, SelectedDate.nMonth,1);
						for (j=0; j<MAX_DAY_IN_WEEK ;j++)
						{
							if(g_tdl_cntx.TaskList[i].Days & gAlmDayOfWeek[j])
							{
								if(FirstDay == j)
									NextDay = 1;
								else if( FirstDay < j)
									NextDay = 1 + j - FirstDay;
								else
									NextDay = 1 + j - FirstDay + MAX_DAY_IN_WEEK;

								while(NextDay <= TDL_MAX_DAYS_PER_MONTH) 
								{
									TaskInDay[NextDay] = 1;
									NextDay = NextDay+MAX_DAY_IN_WEEK;
								}
							}
						}
					} 

					break;

				case REPEAT_WEEKLY	:
					if (IsInSameMonth == TRUE || FindMaxTime(&SelectedDate,&g_tdl_cntx.TaskList[i].Time))
					{
						j = DOW(g_tdl_cntx.TaskList[i].Time.nYear, g_tdl_cntx.TaskList[i].Time.nMonth,g_tdl_cntx.TaskList[i].Time.nDay);

						FirstDay = DOW(SelectedDate.nYear, SelectedDate.nMonth,1);

						if(FirstDay == j)
							NextDay = 1;
						else if(FirstDay < j)
							NextDay = 1 + j - FirstDay;
						else
							NextDay = 1 + j - FirstDay + MAX_DAY_IN_WEEK;

						if(IsInSameMonth == TRUE)
							while( NextDay < g_tdl_cntx.TaskList[i].Time.nDay)
								NextDay = NextDay+MAX_DAY_IN_WEEK;

						while(NextDay <= TDL_MAX_DAYS_PER_MONTH) 
						{
							TaskInDay[NextDay] = 1;
							NextDay = NextDay+MAX_DAY_IN_WEEK;
						}
					}

					break;

				case REPEAT_MONTHLY	:
					if(IsInSameMonth == TRUE || FindMaxTime(&SelectedDate,&g_tdl_cntx.TaskList[i].Time))
					{
						index = g_tdl_cntx.TaskList[i].Time.nDay;
						TaskInDay[index] = 1;
					}
					break;

				default: 
					break;
			}// End of switch

		} // if Present

	} // end of For Loop

	if(*daylist != TDL_MAX_DAYS_PER_MONTH)
	{
		for(j = 0, i=1;i<=TDL_MAX_DAYS_PER_MONTH;i++)
			if (TaskInDay[i]) 
			{
				j++;
				daylist[j] = (U8)i;
			}
		*daylist = (U8)j ;
	}
}// End of Function

#if defined(__MMI_VCALENDAR__) && defined(MMI_ON_HARDWARE_P)
U8 TdlInsertTask(ToDoListNode* task, U8 source)
{
	U8 i;
	if(TDLGetTotalItem() == NUM_OF_TDL)
		return FALSE;

	TDLAddDefaultRecord();

	i = g_tdl_cntx.CurrTaskIndex;
	g_tdl_cntx.CurrHiliteOption = source;

	memcpy(&g_tdl_cntx.TaskList[i], task, sizeof(ToDoListNode));
	TDLInitInlineVariables();
	EntryTDLEditTask();	
	return TRUE;
}
#endif

#endif // #ifdef __MMI_TODOLIST__
#endif // #ifndef _MMI_TODOLIST_C


