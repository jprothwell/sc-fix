/*******************************************************************************
 * Filename:
 * ---------
 * NotepadDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 * This file defines global function prototypes of notepad application.
 *
 * Author: wangzl
 * Date: 2009-02-10
 * ------
 *
 *****************************************************************************/

#ifndef _MMI_NOTEPADDEF_H
#define _MMI_NOTEPADDEF_H
#include "mmi_features.h"


#ifdef __MMI_NOTEPAD__
#include "mmi_data_types.h"
#include "nvram_user_defs.h"

#define MAX_RECORD_LEN  (NVRAM_NOTEPAD_RECORD_SIZE/ENCODING_LENGTH)
#define MAX_NOTEPAD_RECORD MAX_RECORD_LEN * ENCODING_LENGTH
#define NUM_OF_NOTEPAD  NVRAM_NOTEPAD_RECORD_TOTAL
#define SUB_MENU_STR_LEN_NOTEPAD 29

//typedef
typedef struct _notepad_node
{
    U8 Name[SUB_MENU_STR_LEN_NOTEPAD * ENCODING_LENGTH];	//Name of an record
    U8 Present;			//Show the presence of other values	
    U8 index; 				//Show the record index by in time asending
} NotepadNode;

typedef struct
{
    NotepadNode RecordList[NUM_OF_NOTEPAD];
    U8 SortedList[NUM_OF_NOTEPAD];
    U8 CurrSelectedRecord;   /* index of current selected record */
    U8 CurrHiliteOption;    /* index of current higlighted option */
    U8 TotalListedRecord;    /* total number of listed record */
    U8 IsListModified;  /*if need make list*/
    U8 CurrRecordIndex;
} notepad_context_struct;

typedef enum 
{
	NOTEPAD_RECORD_OPTION_VIEW=0,
	NOTEPAD_RECORD_OPTION_ADD,
	NOTEPAD_RECORD_OPTION_EDIT,
	NOTEPAD_RECORD_OPTION_DELETE,
	NOTEPAD_RECORD_OPTION_DELETE_ALL,
	NOTEPAD_OPTION_TOTAL

} NOTEPAD_OPTION_ENUM;

//Extern Global Functions
extern void InitNotepadtApp(void);



//Local Functions
void HighlightNotepadMenu(void);
void NotepadReadFromNvram(void);
void NotepadWriteToNvram(void);
void NotepadShowAllList(void);
void EntryNotepadList(void);
void NotepadMakeList(void);
void HighlightNotepadView(void);
void NotepadExecView(void);
void NotepadInitVariables(void);
void EntryNotepadViewRecord(void);
void HighlightNotepadAdd(void);
void HighlightNotepadEdit(void);
void HighlightNotepadDelete(void);
void HighlightNotepadDeleteAll(void);
void HighlightNotepadList(S32 nIndex);
void EntryNotepadRecordListOption(void);
void EntryNotepadEditRecord(void);
void NotepadDeleteOneRecord(void);
void NotepadShowPopupScreen(U16 Msg);
U8 NotepadGetTotalItem(void);
void NotepadExecDelete(void);
void EntryNotepadDeleteAll(void);
void EntryNotepadDelete(void);
void NotepadDeleteAllRecord(void);
void EntryRecordEditOption(void);
void NotepadSaveRecord(void);
void NotepadGoBackEditorScreen(void);

#endif /*__MMI_NOTEPAD__*/
#endif /*_MMI_NOTEPADDEF_H*/
