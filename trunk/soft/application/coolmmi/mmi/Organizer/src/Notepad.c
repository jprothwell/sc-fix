/**************************************************************

	FILENAME	: Notepad.c

  	PURPOSE		: Notepad Application

 

	AUTHOR		: wangzl

	DATE		: 2009-02-10

**************************************************************/
#ifndef _MMI_NOTEPAD_C
#define _MMI_NOTEPAD_C

#include "mmi_features.h"
#ifdef __MMI_NOTEPAD__
#include "nvramenum.h"

#include "notepaddef.h"
#include "notepadenum.h"
#include "mmi_trace.h"
#include "globalmenuitems.h"
#include "globaldefs.h"
#include "globalconstants.h"
#include "historygprot.h"
#include "eventsgprot.h"
#include "wgui_categories_util.h"
#include "commonscreens.h"
#include "settingprofile.h"
#include "gbhmi.h"

#include "nvramprot.h"


//global variable
notepad_context_struct g_notepad_cntx;
static S8 RecordBuff[MAX_NOTEPAD_RECORD + 4];


//global functions
/*****************************************************************************
* FUNCTION
*	InitNotepadApp
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
void InitNotepadtApp(void)
{
	mmi_trace(g_sw_notepad,"MMI_NOTEPAD: InitNotepadApp");
	
	SetHiliteHandler(ORGANIZER_NOTEPAD_MENU, HighlightNotepadMenu);

	SetHiliteHandler(MITEM_NOTEPAD_VIEW, HighlightNotepadView);
	SetHiliteHandler(MITEM_NOTEPAD_ADD, HighlightNotepadAdd);
	SetHiliteHandler(MITEM_NOTEPAD_EDIT, HighlightNotepadEdit);
	SetHiliteHandler(MITEM_NOTEPAD_DELETE, HighlightNotepadDelete);
	SetHiliteHandler(MITEM_NOTEPAD_DELETE_ALL, HighlightNotepadDeleteAll);

	NotepadReadFromNvram();
}

//local functions
/*****************************************************************************
* FUNCTION
*	HighlightNotepadMenu
* DESCRIPTION
*   Highlight handler of notepad menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightNotepadMenu(void)
{	
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(NotepadShowAllList, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(NotepadShowAllList, KEY_RIGHT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*	NotepadShowAllList
* DESCRIPTION
*   Entry function of notepad
*	Display all record list.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void NotepadShowAllList(void)
{
	g_notepad_cntx.IsListModified = TRUE;

	EntryNotepadList();
}

/*****************************************************************************
* FUNCTION
*	NotepadExecEdit
* DESCRIPTION
*   Execute Edit option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadExecEdit(void)
{
	if(g_notepad_cntx.TotalListedRecord != 0)
		g_notepad_cntx.CurrRecordIndex = g_notepad_cntx.SortedList[g_notepad_cntx.CurrSelectedRecord];

	NotepadInitVariables();
	EntryNotepadEditRecord();
}

/*****************************************************************************
* FUNCTION
*  NotepadMakeSubMenu
* DESCRIPTION
*   make sub menu display str of notepad list
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
S32 NotepadMakeSubMenu()
{
	S32 i;
	S32 TotalItem = 0;
	for(i=0; i<g_notepad_cntx.TotalListedRecord; i++)
	{
		pfnUnicodeStrcpy((S8*)subMenuDataPtrs[TotalItem], (const S8*)g_notepad_cntx.RecordList[g_notepad_cntx.SortedList[i]].Name);
		TotalItem++;
	}
	return TotalItem;
	
}

/*****************************************************************************
* FUNCTION
*  NotepadEnterKeyHandler
* DESCRIPTION
*   handle enter key in list view
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadEnterKeyHandler(void)
{
	if(g_notepad_cntx.TotalListedRecord == 0)
	{
		g_notepad_cntx.CurrHiliteOption = NOTEPAD_RECORD_OPTION_ADD;
		NotepadExecEdit();
	}
	else
	{
		g_notepad_cntx.CurrHiliteOption = NOTEPAD_RECORD_OPTION_VIEW;
		NotepadExecView();
	}
}

/*****************************************************************************
* FUNCTION
*  EntryNotepadTaskList
* DESCRIPTION
*   Entry function of To Do List application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void EntryNotepadList(void)
{
	U8* guiBuffer;
	S32 TotalItem = 0;
	U16 nEmptyId = STR_NOTEPAD_EMPTY;
	mmi_trace(g_sw_notepad,"MMI_NOTEPAD: EntryNotepadList");
	EntryNewScreen(SCR_NOTEPAD_LIST, NULL, EntryNotepadList, NULL);

	if(g_notepad_cntx.IsListModified == TRUE) 
	{
		NotepadMakeList();

		g_notepad_cntx.IsListModified = FALSE;
	}
	
	
	guiBuffer = GetCurrGuiBuffer(SCR_NOTEPAD_LIST);

	SetParentHandler(0);

	RegisterHighlightHandler(HighlightNotepadList);
	if(g_notepad_cntx.TotalListedRecord == 0) /* no record, display <Empty> */
	{
		ShowCategory1Screen(STR_NOTEPAD_RECORD_LIST, GetRootTitleIcon(ORGANIZER_NOTEPAD_MENU),
						  	STR_NOTEPAD_ADD, 0,
						  	STR_GLOBAL_BACK, 0,
						  	1, &nEmptyId, 0, guiBuffer);

		g_notepad_cntx.CurrHiliteOption = NOTEPAD_RECORD_OPTION_ADD;
		mmi_trace(g_sw_notepad,"MMI_NOTEPAD: EntryNotepadList>>no record");
		SetLeftSoftkeyFunction(NotepadExecEdit,KEY_EVENT_UP);
		SetKeyHandler(NotepadExecEdit, KEY_ENTER, KEY_EVENT_UP);
	}
	else /* display list of record */
	{
		mmi_trace(g_sw_notepad,"MMI_NOTEPAD: EntryNotepadList>>have some record");
		/* construct record list */
		TotalItem = NotepadMakeSubMenu();
		ShowCategory84Screen(STR_NOTEPAD_RECORD_LIST, GetRootTitleIcon(ORGANIZER_NOTEPAD_MENU),
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  TotalItem, (U8**) subMenuDataPtrs, (U16*)gIndexIconsImageList,
						  0, 0,guiBuffer);

		SetLeftSoftkeyFunction(EntryNotepadRecordListOption,KEY_EVENT_UP);			
		SetKeyHandler(EntryNotepadRecordListOption, KEY_ENTER, KEY_EVENT_UP);
	}

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*	NotepadMakeList
* DESCRIPTION
*   To make the list of all records to be displayed.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadMakeList(void)
{
	U8 i;
	mmi_trace(g_sw_notepad, "MMI_NOTEPAD: NotepadMakeList");
	g_notepad_cntx.TotalListedRecord = 0;

	memset(g_notepad_cntx.SortedList, 0, sizeof(g_notepad_cntx.SortedList));
	for(i=0; i<NUM_OF_NOTEPAD; i++)
	{
		if(g_notepad_cntx.RecordList[i].Present == 1) 
		{
			g_notepad_cntx.SortedList[g_notepad_cntx.RecordList[i].index] = i;
			g_notepad_cntx.TotalListedRecord++;
		}
	}
} 

/*****************************************************************************
* FUNCTION
*	HighlightNotepadList
* DESCRIPTION
*   Highlight handler of notepad record list, store the index of selected task.
* PARAMETERS
*	nIndex	IN	index of selected task.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightNotepadList(S32 nIndex)
{
	g_notepad_cntx.CurrSelectedRecord = (U8)nIndex;
}

/*****************************************************************************
* FUNCTION
*	NotepadGobackBeforeEdit
* DESCRIPTION
*   back to scr before edit record.
* PARAMETERS
*	nIndex	IN	index of selected task.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadGobackBeforeEdit(void)
{
	DeleteScreenIfPresent(SCR_NOTEPAD_RECORD_EDIT);
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*	HighlightNotepadRecordOption
* DESCRIPTION
*   Highlight handler of record options.
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightNotepadRecordOption(S32 nIndex)
{
	switch (nIndex) 
	{
		case 0:
			SetLeftSoftkeyFunction(NotepadSaveRecord, KEY_EVENT_UP);
			break;
		case 1:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP);
			RegisterInputMethodScreenCloseFunction(NotepadGoBackEditorScreen);
			break;
		case 2:
			SetLeftSoftkeyFunction(NotepadGobackBeforeEdit, KEY_EVENT_UP);
			break;
		default:
			break;
	}
}

/*****************************************************************************
* FUNCTION
*	NotepadGoBackEditorScreen
* DESCRIPTION
*   Go back to full screen editor.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void NotepadGoBackEditorScreen(void)
{
	GoBackToHistory(SCR_NOTEPAD_RECORD_EDIT);
}

/*****************************************************************************
* FUNCTION
*	EntryRecordEditOption
* DESCRIPTION
*   Entry Record Edit Option
*
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryRecordEditOption(void)
{
	U16 nStrItemList[3]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;		/* Stores no of children in the submenu*/
	U8* guiBuffer;		/* Buffer holding history data */

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_NOTEPAD_RECORD_OPTION, NULL, EntryRecordEditOption, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_NOTEPAD_RECORD_OPTION);

	/* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MITEM_NOTEPAD_EDIT_OPTION);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MITEM_NOTEPAD_EDIT_OPTION,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MITEM_NOTEPAD_EDIT_OPTION);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(HighlightNotepadRecordOption);

	/* 8 Display Category Screen */
	ShowCategory15Screen(STR_NOTEPAD_MAIN, GetRootTitleIcon(ORGANIZER_NOTEPAD_MENU),
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						LIST_MENU, 0, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	/* 10. Register Exit handler */
}

/*****************************************************************************
* FUNCTION
*	EntryNotepadEditRecord
* DESCRIPTION
*   Entry edit record screen of Notepad.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryNotepadEditRecord(void)
{
	
	U8* guiBuffer;

	EntryNewScreen(SCR_NOTEPAD_RECORD_EDIT, NULL, EntryNotepadEditRecord, NULL);

	SetParentHandler(0);

	guiBuffer = GetCurrGuiBuffer(SCR_NOTEPAD_RECORD_EDIT);
	
	ShowCategory5Screen(STR_NOTEPAD_MAIN, GetRootTitleIcon(ORGANIZER_NOTEPAD_MENU),
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
						  (U8*)RecordBuff,
						  MAX_RECORD_LEN + 1, guiBuffer);
#if defined(__MMI_GB__)
	SetLeftSoftkeyFunction(EntryRecordEditOption, KEY_EVENT_UP);
	GBSetInputboxLSKFunction(EntryRecordEditOption);
#endif
	SetCategory5RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*	EntryNotepadRecordListOption
* DESCRIPTION
*   Show option list
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryNotepadRecordListOption(void)
{
	U8* guiBuffer;
	U16 nStrItemList[6];	/* Stores the strings id of submenus returned */
	U16 nNumofItem;		/* Stores no of children in the submenu*/
	U16 MenuId;

	EntryNewScreen(SCR_NOTEPAD_OPTION_LIST, NULL, EntryNotepadRecordListOption, NULL);

	/* view, add, edit, delete, delete all */
	if(NotepadGetTotalItem() < NUM_OF_NOTEPAD)
		MenuId = MITEM_NOTEPAD_LIST_OPTION_1;
	else /* view, edit, delete, delete all */
		MenuId = MITEM_NOTEPAD_LIST_OPTION_2;

	nNumofItem = GetNumOfChild(MenuId);

	GetSequenceStringIds(MenuId, nStrItemList);

	SetParentHandler(MenuId);

	guiBuffer = GetCurrGuiBuffer(SCR_NOTEPAD_OPTION_LIST);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(STR_NOTEPAD_CAP_OPTION, GetRootTitleIcon(ORGANIZER_NOTEPAD_MENU),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	NotepadGetTotalItem
* DESCRIPTION
*   Get number of present record
* PARAMETERS
*	none.
* RETURNS
*	number of record
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
U8 NotepadGetTotalItem(void)
{
	S32 i;
	U8 TotalItem = 0;
	
	for(i=0; i<NUM_OF_NOTEPAD; i++)
		if(g_notepad_cntx.RecordList[i].Present == 1)
			TotalItem++;

	return TotalItem;
}

/*****************************************************************************
* FUNCTION
*	HighlightNotepadView
* DESCRIPTION
*   Highlight handler of View option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightNotepadView(void)
{
	g_notepad_cntx.CurrHiliteOption = NOTEPAD_RECORD_OPTION_VIEW;
	SetLeftSoftkeyFunction(NotepadExecView, KEY_EVENT_UP);
	SetKeyHandler(NotepadExecView, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	HighlightNotepadAdd
* DESCRIPTION
*   Highlight handler of Add option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightNotepadAdd(void)
{
	g_notepad_cntx.CurrHiliteOption = NOTEPAD_RECORD_OPTION_ADD;
	SetLeftSoftkeyFunction(NotepadExecEdit, KEY_EVENT_UP);
	SetKeyHandler(NotepadExecEdit, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	HighlightNotepadEdit
* DESCRIPTION
*   Highlight handler of Edit option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightNotepadEdit(void)
{
	g_notepad_cntx.CurrHiliteOption = NOTEPAD_RECORD_OPTION_EDIT;
	SetLeftSoftkeyFunction(NotepadExecEdit, KEY_EVENT_UP);
	SetKeyHandler(NotepadExecEdit, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	HighlightNotepadDelete
* DESCRIPTION
*   Highlight handler of Delete option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightNotepadDelete(void)
{
	g_notepad_cntx.CurrHiliteOption = NOTEPAD_RECORD_OPTION_DELETE;
	SetLeftSoftkeyFunction(NotepadExecDelete, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	HighlightNotepadDeleteAll
* DESCRIPTION
*   Highlight handler of Delete All option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void HighlightNotepadDeleteAll(void)
{
	g_notepad_cntx.CurrHiliteOption = NOTEPAD_RECORD_OPTION_DELETE_ALL;
	SetLeftSoftkeyFunction(EntryNotepadDeleteAll, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	NotepadExecView
* DESCRIPTION
*   Execute View option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadExecView(void)
{
	if (g_notepad_cntx.TotalListedRecord != 0)
		g_notepad_cntx.CurrRecordIndex = g_notepad_cntx.SortedList[g_notepad_cntx.CurrSelectedRecord];

	EntryNotepadViewRecord();
}

/*****************************************************************************
* FUNCTION
*	NotepadAddDefaultRecord
* DESCRIPTION
*	Find an empty solt of Notepad array, and initialize default value of the record.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadAddDefaultRecord(void)
{
	S32 i;
	for(i=0; i<NUM_OF_NOTEPAD; i++)
		if((g_notepad_cntx.RecordList[i].Present != 1))
			break;

	g_notepad_cntx.CurrRecordIndex = (U8)i ;

	memset(&g_notepad_cntx.RecordList[i], 0, sizeof(NotepadNode));
}

/*****************************************************************************
* FUNCTION
*	NotepadInitVariables
* DESCRIPTION
*	Inilialize the variables used in editor screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadInitVariables(void)
{
	U8 i;
	S16 error;

	if (g_notepad_cntx.CurrHiliteOption == NOTEPAD_RECORD_OPTION_ADD)
	{
		NotepadAddDefaultRecord();
	}

	i =	g_notepad_cntx.CurrRecordIndex;
	memset(RecordBuff, 0, sizeof(RecordBuff));
	if(g_notepad_cntx.CurrHiliteOption != NOTEPAD_RECORD_OPTION_ADD)
	{
		mmi_trace(g_sw_notepad,"MMI_NOTEPAD: NotepadInitVariables>>read record");
		ReadRecord(NVRAM_EF_NOTEPAD_RECORD_LID, (U16)(i + 1), RecordBuff, NVRAM_NOTEPAD_RECORD_SIZE, &error);
	}
}

/*****************************************************************************
* FUNCTION
*	EntryNotepadViewRecord
* DESCRIPTION
*   Display record information.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryNotepadViewRecord(void)
{
	U8* guiBuffer;
	S16 error;
	S32 bufferSize = 0;

	EntryNewScreen(SCR_NOTEPAD_VIEW, NULL, EntryNotepadViewRecord, NULL);

	SetParentHandler(0);
	guiBuffer = GetCurrGuiBuffer(SCR_NOTEPAD_VIEW);
	
	ReadRecord(NVRAM_EF_NOTEPAD_RECORD_LID, (U16)(g_notepad_cntx.CurrRecordIndex+ 1), RecordBuff, NVRAM_NOTEPAD_RECORD_SIZE, &error);

       /*Modified to add the function of PageUP and PageDown by liuxl on 20090521 to fix bug-12375 of CS2323C project */
	bufferSize	= pfnUnicodeStrlen((U8*)RecordBuff);
	ShowCategory74Screen( STR_NOTEPAD_MAIN, GetRootTitleIcon(ORGANIZER_NOTEPAD_MENU),
					  		STR_GLOBAL_EDIT, 0,
					  		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							(U8*)RecordBuff, bufferSize, guiBuffer);

	SetLeftSoftkeyFunction(NotepadExecEdit, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*	NotepadExecDelete
* DESCRIPTION
*   Execute Delete option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadExecDelete(void)
{
	if(g_notepad_cntx.TotalListedRecord != 0)
		g_notepad_cntx.CurrRecordIndex = g_notepad_cntx.SortedList[g_notepad_cntx.CurrSelectedRecord];

	EntryNotepadDelete();
}

/*****************************************************************************
* FUNCTION
*	EntryNotepadDelete
* DESCRIPTION
*   Display confirmation screen for deleting one record.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryNotepadDelete(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_NOTEPAD_DELETE_ONE_RECORD_QUERY),
						IMG_GLOBAL_QUESTION,WARNING_TONE);

	SetLeftSoftkeyFunction(NotepadDeleteOneRecord, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*	NotepadDeleteOneRecord
* DESCRIPTION
*   Delete one record from notepad
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadDeleteOneRecord(void)
{
	U8 i,j;

	i = g_notepad_cntx.CurrRecordIndex;
	
	if (g_notepad_cntx.RecordList[i].Present == 1)
	{
		for(j=0; j< NUM_OF_NOTEPAD; j++)
		{
			if(g_notepad_cntx.RecordList[j].index > g_notepad_cntx.RecordList[i].index)
			{
				g_notepad_cntx.RecordList[j].index --;
			}
		}
		memset(&g_notepad_cntx.RecordList[i], 0, sizeof(NotepadNode));
		NotepadWriteToNvram();
		NotepadShowPopupScreen(STR_GLOBAL_DONE);
	}
	else
		DisplayPopup((U8*)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 0,UI_POPUP_NOTIFYDURATION_TIME,(U8)ERROR_TONE);

}

/*****************************************************************************
* FUNCTION
*	NotepadShowPopupScreen
* DESCRIPTION
*   Display popup screen with corresponding messages.
* PARAMETERS
*	Msg IN enum value of message type.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void NotepadShowPopupScreen(U16 Msg)
{
	U16 ScreenToShow;
	U16 ScreenImageId;
	ALL_TONE_ENUM ScreenTone;

	switch(Msg) 
	{
		case STR_NOTEPAD_EMPTY_RECORD_MSG:
		{
			ScreenImageId = IMG_GLOBAL_EMPTY;
			ScreenToShow = SCR_NOTEPAD_RECORD_EDIT;
			ScreenTone = EMPTY_LIST_TONE;
			break;

		}
		default: /* case STR_GLOBAL_DONE */
		{
			ScreenImageId = IMG_GLOBAL_ACTIVATED;
			ScreenToShow = SCR_NOTEPAD_LIST;
			ScreenTone = SUCCESS_TONE;
			break;
		}
	}
 
	DisplayPopup((U8*)GetString(Msg), ScreenImageId, 0,UI_POPUP_NOTIFYDURATION_TIME,(U8)ScreenTone);

	DeleteUptoScrID(ScreenToShow);
}

/*****************************************************************************
* FUNCTION
*	EntryNotepadDeleteAll
* DESCRIPTION
*   Display confirmation screen for deleting all tasks.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryNotepadDeleteAll(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_NOTEPAD_DELETE_ALL_TASK_QUERY),
						IMG_GLOBAL_QUESTION,WARNING_TONE);

	SetLeftSoftkeyFunction(NotepadDeleteAllRecord, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*	NotepadDeleteAllRecord
* DESCRIPTION
*   Delete all records from notepad
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadDeleteAllRecord(void)
{
	S32 i;

	for(i=0;i<NUM_OF_NOTEPAD;i++)
	{
		if(g_notepad_cntx.RecordList[i].Present == 0)
			continue;
	
		memset(&g_notepad_cntx.RecordList[i], 0, sizeof(NotepadNode));
	}

	NotepadWriteToNvram();

	NotepadShowPopupScreen(STR_GLOBAL_DONE);
}

/*****************************************************************************
* FUNCTION
*	NotepadSaveRecord
* DESCRIPTION
*   Retrieve user input from editor screen and save to NVRAM.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void NotepadSaveRecord(void)
{
	U8 i;
	S16 error;

	i = g_notepad_cntx.CurrRecordIndex;

	/* empty record */
	if (pfnUnicodeStrlen(RecordBuff) == 0) 
	{
		NotepadShowPopupScreen(STR_NOTEPAD_EMPTY_RECORD_MSG);
		return;
	}
	//use previous char as name
	pfnUnicodeStrncpy((S8*)(g_notepad_cntx.RecordList[i].Name), RecordBuff, (SUB_MENU_STR_LEN_NOTEPAD - 1));
	g_notepad_cntx.RecordList[i].Present = 1;
	if(g_notepad_cntx.CurrHiliteOption == NOTEPAD_RECORD_OPTION_ADD)
		g_notepad_cntx.RecordList[i].index = g_notepad_cntx.TotalListedRecord;

	WriteRecord(NVRAM_EF_NOTEPAD_RECORD_LID, (U16)(i + 1), RecordBuff, NVRAM_NOTEPAD_RECORD_SIZE, &error);

	NotepadWriteToNvram();
	
	NotepadShowPopupScreen(STR_GLOBAL_DONE);
}

/*****************************************************************************
* FUNCTION
*	NotepadReadFromNvram
* DESCRIPTION
*   Read data from Nvram.
*	Intialize to default value if data is not intialized
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadReadFromNvram(void)
{
	S16	error;
	S32 ret;

	ret = ReadRecord(NVRAM_EF_NOTEPAD_LIST_LID, (U16)1, g_notepad_cntx.RecordList, NVRAM_NOTEPAD_LIST_SIZE, &error);
	mmi_trace(g_sw_notepad,"MMI_NOTEPAD: NotepadReadFromNvram, error is:%d,ret is:%d",error, ret);

	if(g_notepad_cntx.RecordList[0].Present != 1 && g_notepad_cntx.RecordList[0].Present != 0) 
	{
		mmi_trace(g_sw_notepad,"MMI_NOTEPAD:NotepadReadFromNvram>>data error, init");
		memset(g_notepad_cntx.RecordList, 0, sizeof(g_notepad_cntx.RecordList));
	}

}

/*****************************************************************************
* FUNCTION
*	NotepadWriteToNvram
* DESCRIPTION
*   Write data to Nvram
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void NotepadWriteToNvram(void)
{
	S16	error;

	g_notepad_cntx.IsListModified = TRUE;

	WriteRecord (NVRAM_EF_NOTEPAD_LIST_LID,  (U16)1, (void*)g_notepad_cntx.RecordList, NVRAM_NOTEPAD_LIST_SIZE, &error);
}

#endif/*__MMI_NOTEPAD__*/
#endif /*_MMI_NOTEPAD_C*/
