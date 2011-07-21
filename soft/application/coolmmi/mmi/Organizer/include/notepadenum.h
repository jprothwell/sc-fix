/**************************************************************

	FILENAME	: NotepadEnum.h

  	PURPOSE		: Notepad application

 

	AUTHOR		: wangzl

	DATE		: 2009-02-10

**************************************************************/
 
#ifndef _COOLSAND_NOTEPAD_ENUM_H
#define _COOLSAND_NOTEPAD_ENUM_H

#include "mmi_features.h"
#ifdef __MMI_NOTEPAD__

#include "stdc.h"
#include "mmi_data_types.h"

typedef enum 
{
	SCR_NOTEPAD_LIST =	NOTEPAD_BASE + 1,
	SCR_NOTEPAD_OPTION_LIST,
	SCR_NOTEPAD_RECORD_EDIT,
	SCR_NOTEPAD_RECORD_OPTION,
	SCR_NOTEPAD_VIEW,

} NOTEPAD_SCR_ID_ENUM ;


// Define the All image Id of Data Account

typedef enum 
{
	 IMG_NOTEPAD_CAPTION = NOTEPAD_BASE + 1,
	 ORGANIZER_MENU_NOTEPAD_IMAGEID,
} IMG_NOTEPAD;


// Define the All string Id of Data Account

typedef enum 
{
	STR_NOTEPAD_MAIN = NOTEPAD_BASE + 1,
	STR_NOTEPAD_EMPTY,
	STR_NOTEPAD_RECORD_LIST,
	STR_NOTEAD_VIEW,
	STR_NOTEPAD_ADD,
	STR_NOTEPAD_EDIT,
	STR_NOTEPAD_DELETE,
	STR_NOTEPAD_DELETE_ALL,
	STR_NOTEPAD_DELETE_ONE_RECORD_QUERY,
	STR_NOTEPAD_DELETE_ALL_TASK_QUERY,
	STR_NOTEPAD_EMPTY_RECORD_MSG,
	STR_NOTEPAD_CAP_OPTION,
	STR_NOTEPAD_ABANDON,
 } STR_NOTEPAD ;

#endif
#endif /* _COOLSAND_TODO_LIST_ENUM_H */
