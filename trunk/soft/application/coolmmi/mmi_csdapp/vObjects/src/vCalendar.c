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
 *	vCalendar.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file is implements vCalendar application.
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
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h" 
#ifndef _MMI_VCALENDAR_C
#define _MMI_VCALENDAR_C
/*  Include: MMI header file */
#if defined(__MMI_VCALENDAR__)
#define __NEWSIMULATOR 

#include "stdc.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "historygprot.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "commonscreens.h"
#include "debuginitdef.h"
#include "filesystemdef.h"
#include "globalmenuitems.h"
#include "unicodexdcl.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "asciiprot.h"

#include "vcalendar.h"
#include "vobjects.h"
#include "todolistdef.h"
#include "alarmframeworkprot.h"
#include "datetimegprot.h"
#include "todolistenum.h"
#include "emsmiscell.h"
#include "irdammigprots.h"
#include "filemgr.h"
#include "filemanagergprot.h"
#include "fmt_struct.h"
#include "messagesexdcl.h"
#include "simdetectiongexdcl.h"

//Renwy add begine 2008/08/06
#include "btmmigprots.h"
#include "oppmmigprots.h"
#include "extdevicedefs.h"
//Renwy add end 2008/08/06
// dingjian 20080227 add vObject Module
#include "fs.h"

#if defined(MMS_SUPPORT)
#include "wapprot.h"
#endif
#undef __NEWSIMULATOR 

#define VCLNDR_TEST_CODE
#include "conversions.h"
#include "app_qp.h"
#include "phonesetupgprots.h"

/* 
** Typedef 
*/

/* 
** Define
*/

#define	VCLNDR_MAX_DT_LEN				24
#define	VCLNDR_NUM_OF_DAYS				7
#define	VCLNDR_MIN_RECURR_LEN			5
#define	VCLNDR_MAX_PARAM_LEN			24

#define	VCLNDR_DT_SEP_CHAR				'T'	/* seperator of date and time */
#define	VCLNDR_UTC_CHAR					'Z'
#define	VCLNDR_POUND_CHAR				'#'
#define	VCLNDR_PARA_SEP_CHAR			';'
#define	VCLNDR_VALUE_SEP_CHAR			':'

#define	VCLNDR_ES_CHAR					0x5C	/* "\" */
#define	VCLNDR_LWSP_CHAR				0x20
#define	VCLNDR_TAB_CHAR					0x09

#define	VCLNDR_R_FOREVER_STR			" #0"
#define	VCLNDR_PARA_SEP_STR				";"
#define	VCLNDR_EQUAL_STR				"="
#define  VCLNDR_CHARSET_UTF8			"UTF-8"

/* 
** Local Variable
*/


/* 
** Local Function
*/
extern S32 mmi_vclndr_writer(ToDoListNode*, S8*);
extern BOOL mmi_vlcndr_uti_flush_to_file(FILE_HANDLE, S8*);
extern S32 mmi_vclndr_reader(S8*, ToDoListNode*);
extern S32 mmi_vobj_uti_get_line(S8* strOutput, FS_HANDLE fh);


/* 
** Global Variable
*/
extern const S8* g_vobj_charset[];
vclndr_context_struct g_vclndr_cntx;


const S8* const g_vclndr_fileds[VCLNDR_TOTAL_FILEDS] =
{
	"BEGIN:VCALENDAR",
	"VERSION:1.0",
	"BEGIN:VEVENT",
	"BEGIN:VTODO",
	"DTSTART",
	"DTEND",
	"DUE",
	"SUMMARY",
	"DESCRIPTION",
	"RRULE",
	"DALARM",
	"AALARM",
	"END:VTODO",
	"END:VEVENT",
	"END:VCALENDAR"
};

const S8* const g_vclndr_days[VCLNDR_NUM_OF_DAYS] = 
{
	"SU", "MO", "TU", "WE", "TH", "FR", "SA"
};


const S8* const g_vclndr_recurr[VCLNDR_RECURR_TOTAL] = 
{
	"D1",
	"W1",
	"MD1"
};
extern const S8* g_vobj_encoding[];
extern const S8* g_vobj_paras[];


/* 
** Global Function
*/
extern U8 TDLGetTotalItem(void);
extern FLOAT GetTimeZone(U8);
extern U8 PhnsetGetHomeCity(void);
extern BOOL GetUCS2Flag(S8*);


#define VCLNDR_INITIALIZATION
/*****************************************************************************
* FUNCTION
*	mmi_vclndr_init
* DESCRIPTION
*	Init function of vCalendar.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_init(void)
{
	SetHiliteHandler(MENU_VCLNDR_FORWARD, mmi_vclndr_highlight_forward);

	SetHiliteHandler(MENU_VCLNDR_RECV_OPT_TDL, mmi_vclndr_highlight_recv_opt_save_to_tdl);

#if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)
	SetHiliteHandler(MENU_VCLNDR_SEND_BY_SMS, mmi_vclndr_sms_highlight_forward);
#endif

#if defined(__MMI_MMS__)
	SetHiliteHandler(MENU_VCLNDR_SEND_BY_MMS, mmi_vclndr_mms_highlight_forward);
#endif

#if  defined(__MMI_IRDA_SUPPORT__)
	SetHiliteHandler(MENU_VCLNDR_SEND_BY_IRDA, mmi_vclndr_irda_highlight_forward);
#endif
//Renwy add begine 2008/09/10
#if (defined(__MMI_ACCEL_OPP_SUPPORT__) ||defined(__MMI_OPP_SUPPORT__))
    SetHiliteHandler(MENU_VCLNDR_SEND_BY_BT, mmi_vclndr_opp_highlight_forward);
#endif 
//Renwy add end 2008/09/10
#if defined(EMAIL_APP_SUPPORT)
	SetHiliteHandler(MENU_VCLNDR_SEND_BY_EMAIL, mmi_vclndr_email_highlight_forward);
#endif

#if  defined(__MMI_FILE_MANAGER__)
	SetHiliteHandler(MENU_VCLNDR_OPT_INPUT_METHOD, mmi_vclndr_highlight_opt_input_method);
	SetHiliteHandler(MENU_VCLNDR_OPT_DONE, mmi_vclndr_highlight_opt_done);
#if defined(__FS_MOVE_SUPPORT__)
	SetHiliteHandler(MENU_VCLNDR_RECV_OPT_FMGR, mmi_vclndr_highlight_recv_opt_save_to_file);
#endif	
	SetHiliteHandler(MENU_VCLNDR_SEND_BY_FILEMGR, mmi_vclndr_fmgr_highlight_forward);
#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_deinit
* DESCRIPTION
*	Deinit vCalendar applicatoin
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_deinit(void)
{
	mmi_vclndr_free_buffer();
}

/*****************************************************************************
* FUNCTION
*	mmi_vclndr_free_buffer
* DESCRIPTION
*	Release all dynamic allocated memory used in vCalendar.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_free_buffer(void)
{
	if(g_vclndr_cntx.FilePathPtr)
	{
		OslMfree(g_vclndr_cntx.FilePathPtr);
		g_vclndr_cntx.FilePathPtr = NULL;
	}

	if(g_vclndr_cntx.FileNamePtr)
	{
		OslMfree(g_vclndr_cntx.FileNamePtr);
		g_vclndr_cntx.FileNamePtr = NULL;
	}
}

#define VCLNDR_SCREENS
/*****************************************************************************
* FUNCTION
*	mmi_vclndr_highlight_forward
* DESCRIPTION
*	Highlight handler of Forward option in To Do List application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_highlight_forward(void)
{
	SetLeftSoftkeyFunction(mmi_vclndr_entry_forward, KEY_EVENT_UP);
	SetKeyHandler(mmi_vclndr_entry_forward, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_entry_forward
* DESCRIPTION
*	Entry function of Forward option in To Do List application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_entry_forward(void)
{
	U8* guiBuffer;
	U16 nStrItemList[6]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;     /* Stores no of children in the submenu*/

	//ExecuteCurrExitHandler();
	EntryNewScreen(SCR_ID_VCLNDR_FORWARD, NULL, mmi_vclndr_entry_forward,NULL);

	if(g_tdl_cntx.TotalListedTask != 0)
		g_tdl_cntx.CurrTaskIndex = g_tdl_cntx.SortedList[g_tdl_cntx.CurrSelectedTask];

	nNumofItem = GetNumOfChild(MENU_VCLNDR_FORWARD);
	GetSequenceStringIds(MENU_VCLNDR_FORWARD,nStrItemList);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_VCLNDR_FORWARD);

	SetParentHandler(MENU_VCLNDR_FORWARD);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* 8 Display Category Screen */
	ShowCategory15Screen(STR_ID_VCLNDR_FORWARD, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	//SetGenericExitHandler(SCR_ID_VCLNDR_FORWARD, NULL, mmi_vclndr_entry_forward);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_entry_recv_option
* DESCRIPTION
*	Display option screen when receiving an vCalendar object
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_entry_recv_option(void)
{
	U8* guiBuffer;
	U16 nStrItemList[VCLNDR_OPT_TOTAL]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;		/* Stores no of children in the submenu*/

	//ExecuteCurrExitHandler();
	EntryNewScreen(SCR_ID_VCLNDR_RECV_OPT, NULL, mmi_vclndr_entry_recv_option,NULL);

	nNumofItem = GetNumOfChild(MENU_VCLNDR_RECV_OPTION);
	GetSequenceStringIds(MENU_VCLNDR_RECV_OPTION,nStrItemList);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_VCLNDR_RECV_OPT);

	SetParentHandler(MENU_VCLNDR_RECV_OPTION);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* 8 Display Category Screen */
	ShowCategory15Screen(STR_GLOBAL_OPTIONS, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	//SetGenericExitHandler(SCR_ID_VCLNDR_RECV_OPT, NULL, mmi_vclndr_entry_recv_option);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_highlight_recv_opt_save_to_tdl
* DESCRIPTION
*	Highlight handler of save to to do list option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_highlight_recv_opt_save_to_tdl(void)
{
	SetLeftSoftkeyFunction(mmi_vclndr_recv_save_to_tdl, KEY_EVENT_UP);
	SetKeyHandler(mmi_vclndr_recv_save_to_tdl, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_recv_save_to_tdl
* DESCRIPTION
*	Handler of Save To To Do List option when receiving a vCalendar.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_recv_save_to_tdl(void)
{
	U8 result = VOBJ_ERR_NO_ERROR;
	S8 path_name[VOBJ_MAX_PATH_LENGTH];
	ToDoListNode task;

	mmi_vobj_get_full_path_name(g_vobj_cntx.buffer_begin, path_name, MMI_VOBJ_VCLNDR);	

	if((result =mmi_vclndr_reader(path_name, &task)) == VOBJ_ERR_NO_ERROR)
	{
		if(TdlInsertTask(&task, TODO_FROM_NETWORK))
			result = VOBJ_ERR_NO_ERROR;
		else
			result = VOBJ_ERR_MEMORY_FULL;
	}

	switch(result)
	{
		case VOBJ_ERR_NO_ERROR:
			break;
		case VOBJ_ERR_MEMORY_FULL:
			DisplayPopup((U8*)GetString(STR_ID_VCLNDR_TASK_FULL), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
			break;
		case VOBJ_ERR_UNSUPPORT_FORMAT:
			DisplayPopup((U8*)GetString(STR_ID_VOBJ_FORMAT_NOT_SUPPORT), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
			break;
		default: // case VOBJ_ERR_NO_ERROR:
			DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_popup_not_support
* DESCRIPTION
*	Popup not supported screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_popup_not_support(void)
{
	DisplayPopup((U8*)GetString(STR_ID_VOBJ_FUNC_NOT_SUPPORTED), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
}

#define	VCLNDR_SAVE_FILE
/*****************************************************************************
* FUNCTION
*	mmi_vclndr_entry_filename
* DESCRIPTION
*	Full screen editor to edit filename
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_entry_filename(void)
{
	U8* guiBuffer;

	//ExecuteCurrExitHandler();
	EntryNewScreen(SCR_ID_VCLNDR_FILENAME, NULL, mmi_vclndr_entry_filename,NULL);

	SetParentHandler(0);	

	guiBuffer = GetCurrGuiBuffer(SCR_ID_VCLNDR_FILENAME);

	ShowCategory5Screen(STR_ID_VOBJ_FILENAME, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, //  |INPUT_TYPE_USE_ENCODING_BASED_LENGTH
						  (U8*)g_vclndr_cntx.FileNamePtr,
						  VOBJ_MAX_FILENAME_LENGTH/ENCODING_LENGTH, guiBuffer);

	SetLeftSoftkeyFunction(mmi_vclndr_entry_filename_option,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	//SetGenericExitHandler(SCR_ID_VCLNDR_FILENAME, NULL, mmi_vclndr_entry_filename);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_entry_filename_option
* DESCRIPTION
*	Display option screen for filename
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_entry_filename_option(void)
{
	U8* guiBuffer;
	U16	ItemList[2];
	U16 nItems;

	//ExecuteCurrExitHandler();
	EntryNewScreen(SCR_ID_VCLNDR_FILENAME_OPT, NULL, mmi_vclndr_entry_filename_option,NULL);
	 
	guiBuffer = GetCurrGuiBuffer(SCR_ID_VCLNDR_FILENAME_OPT);
	SetParentHandler(MENU_VCLNDR_FILENAME_OPT);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	nItems = GetNumOfChild(MENU_VCLNDR_FILENAME_OPT);
	GetSequenceStringIds(MENU_VCLNDR_FILENAME_OPT,ItemList);
	
	ShowCategory52Screen(STR_GLOBAL_OPTIONS, GetRootTitleIcon(ORGANIZER_TODOLIST_MENU),
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList,
							(PU16)gIndexIconsImageList, 0,
							0, 0, guiBuffer);
		
	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	//SetGenericExitHandler(SCR_ID_VCLNDR_FILENAME_OPT, NULL, mmi_vclndr_entry_filename_option);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_goback_1_history
* DESCRIPTION
*	Go back 1 history.
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void mmi_vclndr_goback_1_history(void)
{
	GoBacknHistory(1);
}

#define VCLNDR_COMMON_INTERFACE
#if  defined(__MMI_IRDA_SUPPORT__)
/*****************************************************************************
* FUNCTION
*	mmi_vclndr_irda_highlight_forward
* DESCRIPTION
*	Highlight handler of Send To Irda option.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_irda_highlight_forward(void)
{
	SetLeftSoftkeyFunction(mmi_vclndr_irda_forward, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_irda_forward
* DESCRIPTION
*	Forward a task to Irda
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_irda_forward(void)
{
	S8 path[VOBJ_MAX_PATH_LENGTH];
	U8 i;
	S32 check_result = mmi_vobj_fs_check_folder();
	if(check_result == FS_NO_ERROR) 
	{
		S32 result;
		mmi_vclndr_uti_get_default_filename(path);

		i = g_tdl_cntx.CurrTaskIndex;
		pfdelete(path);	
		result = mmi_vclndr_writer(&g_tdl_cntx.TaskList[i], path);

		if(result != VOBJ_ERR_NO_ERROR) 
		{
			FS_Delete((U16*)path);
			DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
			return;		
		}		
		
		if( MMI_IRDA_ERR_SUCCESS == mmi_irda_send_file_obj(path,  mmi_vclndr_irda_forward_callback) )
		{
		mmi_vobj_entry_irda_processing_screen();
	}

	}
	else
	{
		/* display popup screen for error */
		DisplayPopup((PU8)GetString(GetFileSystemErrorString(check_result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_vcard_forward_irda_callback
* DESCRIPTION
*	Callback function for Irda application when sending via Irda.
* PARAMETERS
*	buf_p	IN		unknown
*	result	IN		result state for sending
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_irda_forward_callback(void *buf_p, U32 result )
{
	if(buf_p != NULL)
	{
		OslMfree(buf_p);
		buf_p = NULL;
	}

	mmi_vclndr_uti_del_default_file();

	if(result == MMI_IRDA_ERR_SUCCESS)
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	else
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_ERROR, TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);

	DeleteBetweenScreen(SCR_ID_VCLNDR_FORWARD, SCR_ID_VOBJ_IRDA_FORWARD_PROCESSING);

	//DeleteUptoScrID(SCR_TDL_TASK_LIST);
}
#endif	// #if  defined(__MMI_IRDA_SUPPORT__)

#if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)
/*****************************************************************************
* FUNCTION
*  mmi_vclndr_sms_forward_callback
* DESCRIPTION
*   Callback function used by sending SMS. Delete temporary file.
* PARAMETERS
*  flag	IN	unknown
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_vclndr_sms_forward_callback(void* flag)
{
	mmi_vclndr_uti_del_default_file();

	//DeleteUptoScrID(SCR_TDL_TASK_LIST);

	DeleteScreenIfPresent(SCR_ID_VCLNDR_FORWARD);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_sms_forward
* DESCRIPTION
*	Forward a task to SMS
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_sms_forward(void)
{
	S32 check_result = mmi_vobj_fs_check_folder();
	if(check_result == FS_NO_ERROR) 
	{
		S32 result;
		S8 path[VOBJ_MAX_PATH_LENGTH];
		U8 i;
		mmi_vclndr_uti_get_default_filename(path);

		i = g_tdl_cntx.CurrTaskIndex;

		result = mmi_vclndr_writer(&g_tdl_cntx.TaskList[i], path);
		if(result != VOBJ_ERR_NO_ERROR) 
		{
			FS_Delete((U16*)path);
			DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
			return;		
		}
		mmi_ems_vobject_send_by_ems (0x0000, VOBJ_VCLNDR_PORT, path, mmi_vclndr_sms_forward_callback);
	}
	else
	{
		/* display popup screen for error */
		DisplayPopup((PU8)GetString(GetFileSystemErrorString(check_result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_sms_highlight_forward
* DESCRIPTION
*	Highlight handler of Send To SMS option.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_sms_highlight_forward(void)
{
	SetLeftSoftkeyFunction(mmi_vclndr_sms_forward, KEY_EVENT_UP);
	SetKeyHandler(mmi_vclndr_sms_forward, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
#endif // #if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)

#if defined(__MMI_MMS__)
/*****************************************************************************
* FUNCTION
*	mmi_vclndr_mms_highlight_forward
* DESCRIPTION
*	Highlight handler of Send To MMS option.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_mms_highlight_forward(void)
{
	SetLeftSoftkeyFunction(mmi_vclndr_mms_forward, KEY_EVENT_UP);
	SetKeyHandler(mmi_vclndr_mms_forward, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	mmi_vclndr_mms_forward
* DESCRIPTION
*	Create temporary file and forward to MMS
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_mms_forward(void)
{
	S32 check_result = mmi_vobj_fs_check_folder();
	if(check_result == FS_NO_ERROR) 
	{
		S8 path[VOBJ_MAX_PATH_LENGTH];
		U8 i;
		S32 result;
		mmi_vclndr_uti_get_default_filename(path);

		i = g_tdl_cntx.CurrTaskIndex;
		result =	mmi_vclndr_writer(&g_tdl_cntx.TaskList[i], path);
		if(result != VOBJ_ERR_NO_ERROR) 
		{
			FS_Delete((U16*)path);
			DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
			return;		
		}		

		mms_content_insert_hdlr(path);
	}
	else
	{
		/* display popup screen for error */
		DisplayPopup((PU8)GetString(GetFileSystemErrorString(check_result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
	}
}
#endif // #if defined(__MMI_MMS__)

#if (defined(__MMI_ACCEL_OPP_SUPPORT__)||   defined(__MMI_OPP_SUPPORT__))
/*****************************************************************************
 * FUNCTION
 *  mmi_vclndr_opp_highlight_forward
 * DESCRIPTION
 *  Highlight handler of Send To Bluetooth option.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vclndr_opp_highlight_forward(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vclndr_opp_forward, KEY_EVENT_UP);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vclndr_opp_forward
 * DESCRIPTION
 *  Forward a task to Bluetooth
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vclndr_opp_forward(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 path[VOBJ_MAX_PATH_LENGTH];
    U8 i;
    S32 check_result = mmi_vobj_fs_check_folder();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (check_result == FS_NO_ERROR)
    {
        S32 result;

        mmi_vclndr_uti_get_default_filename(path);

        i = g_tdl_cntx.CurrTaskIndex;
	
        //MMI_FS_Delete((U16*) path);//Renwy del 2008/08/07
		
        result = mmi_vclndr_writer(&g_tdl_cntx.TaskList[i], path);

	mmi_trace(g_sw_BT, "mmi_vclndr_writer->result = %d",result);
	
        if (result != VOBJ_ERR_NO_ERROR)
        {
            MMI_FS_Delete((U16*) path);
            DisplayPopup(
                (PU8) GetString(GetFileSystemErrorString(result)),
                IMG_GLOBAL_ERROR,
                1,
                UI_POPUP_NOTIFYDURATION_TIME,
                (U8) ERROR_TONE);
            return;
        }
        if (MMI_OPP_ERR_SUCCESS == mmi_opp_discovery_device((PS8) path, mmi_vclndr_opp_forward_callback))
        {
            mmi_opp_entry_opp_device_discovery();
        }

    }
    else
    {
        /* display popup screen for error */
        DisplayPopup(
            (PU8) GetString(GetFileSystemErrorString(check_result)),
            IMG_GLOBAL_ERROR,
            1,
            UI_POPUP_NOTIFYDURATION_TIME,
            (U8) ERROR_TONE);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vclndr_opp_forward_callback
 * DESCRIPTION
 *  Callback function for Opp application when sending via Bluetooth.
 * PARAMETERS
 *  buf_p       [IN]        Unknown
 *  result      [IN]        Result state for sending
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vclndr_opp_forward_callback(void *buf_p, U32 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (buf_p != NULL)
    {
        OslMfree(buf_p);
        buf_p = NULL;
    }

    mmi_vclndr_uti_del_default_file();

    if (result == MMI_OPP_ERR_SUCCESS)
    {
        DisplayPopup(
            (PU8) GetString(STR_GLOBAL_DONE),
            IMG_GLOBAL_ACTIVATED,
            TRUE,
            UI_POPUP_NOTIFYDURATION_TIME,
            SUCCESS_TONE);
    }
    else
    {
        DisplayPopup(
            (PU8) GetString(STR_GLOBAL_UNFINISHED),
            IMG_GLOBAL_ERROR,
            TRUE,
            UI_POPUP_NOTIFYDURATION_TIME,
            ERROR_TONE);
    }

    DeleteBetweenScreen(SCR_ID_VCLNDR_FORWARD, SCR_OPP_SENDING);

    /* DeleteUptoScrID(SCR_TDL_TASK_LIST); */
}
#endif /* defined(__MMI_ACCEL_OPP_SUPPORT__) */ // #if  defined(__MMI_OPP_SUPPORT__)

#if defined(EMAIL_APP_SUPPORT)
/*****************************************************************************
* FUNCTION
*	mmi_vclndr_email_highlight_forward
* DESCRIPTION
*	Highlight handler of Send To Email option.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_email_highlight_forward(void)
{
	SetLeftSoftkeyFunction(mmi_vclndr_popup_not_support, KEY_EVENT_UP);
	SetKeyHandler(mmi_vclndr_popup_not_support, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
#endif // #if defined(EMAIL_APP_SUPPORT)


#if defined(__MMI_FILE_MANAGER__)
/*****************************************************************************
* FUNCTION
*	mmi_vclndr_fmgr_highlight_forward
* DESCRIPTION
*	Highlight handler of Send To File Manager option.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_fmgr_highlight_forward(void)
{
	g_vclndr_cntx.SrcType = VCLNDR_SAVE_FROM_TDL;
	SetLeftSoftkeyFunction(mmi_vclndr_fmgr_forward, KEY_EVENT_UP);
	SetKeyHandler(mmi_vclndr_fmgr_forward, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_fmgr_forward
* DESCRIPTION
*	Forward file to file manager
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_fmgr_forward(void)
{
	FMGR_FILTER filter;
	
	FMGR_FILTER_INIT(&filter);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
	
	mmi_fmgr_select_path_and_enter(APP_TODOLIST, 
								FMGR_SELECT_FOLDER, 
								filter,
								(S8*)L"root", 
								mmi_vclndr_fmgr_select_path_done);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_highlight_opt_save_to_file
* DESCRIPTION
*	Highlight handler of save to file option
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_highlight_recv_opt_save_to_file(void)
{
	g_vclndr_cntx.SrcType = VCLNDR_SAVE_FROM_RECV;
	SetLeftSoftkeyFunction(mmi_vclndr_fmgr_forward, KEY_EVENT_UP);
	SetKeyHandler(mmi_vclndr_fmgr_forward, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  mmi_vclndr_fmgr_select_path_done
* DESCRIPTION
*   Callback function for file manager when path selection finished.
* PARAMETERS
*  path		IN		user selected path
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_vclndr_fmgr_select_path_done(void *path, int is_short)
{
	if(path)
	{
		U8 i;

		i = g_tdl_cntx.CurrTaskIndex;

		if(g_vclndr_cntx.FilePathPtr == NULL)
			g_vclndr_cntx.FilePathPtr = OslMalloc(VOBJ_MAX_PATH_LENGTH);

		if(g_vclndr_cntx.FileNamePtr == NULL)
			g_vclndr_cntx.FileNamePtr = OslMalloc(MAX_TODO_LIST_NOTE);

		pfnUnicodeStrcpy(g_vclndr_cntx.FilePathPtr, (S8*)path);

		if(g_vclndr_cntx.SrcType == VCLNDR_SAVE_FROM_TDL)
			pfnUnicodeStrcpy(g_vclndr_cntx.FileNamePtr, (S8*)g_tdl_cntx.TaskList[i].Note);
		else
			memset(g_vclndr_cntx.FileNamePtr, 0, ENCODING_LENGTH);

		mmi_vclndr_entry_filename();

		/* to delete all screens for selecting path */
		if(g_vclndr_cntx.SrcType == VCLNDR_SAVE_FROM_TDL)
			DeleteUptoScrID(SCR_ID_VCLNDR_FORWARD);
		else
			DeleteUptoScrID(SCR_ID_VCLNDR_RECV_OPT);
	}
	else
	{
		if(g_vclndr_cntx.SrcType == VCLNDR_SAVE_FROM_TDL)
			GoBackToHistory(SCR_ID_VCLNDR_FORWARD);
		else
			GoBackToHistory(SCR_ID_VCLNDR_RECV_OPT);
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_save_tdl_to_file
* DESCRIPTION
*	Save to do list to file
* PARAMETERS
*	none
* RETURNS
*	Error cause
* GLOBALS AFFECTED
*	none
*****************************************************************************/
S32 mmi_vclndr_save_tdl_to_file(void)
{
	S32 result = VOBJ_ERR_UNKNOWN;
	S8 *file_path_name;
	S8 file_ext[VOBJ_MAX_FILENAME_LENGTH];
	U8 i;

	i = g_tdl_cntx.CurrTaskIndex;

	file_path_name = OslMalloc(VOBJ_MAX_PATH_LENGTH + MAX_TODO_LIST_NOTE);

	pfnUnicodeStrcpy(file_path_name, (S8*)g_vclndr_cntx.FilePathPtr);
	pfnUnicodeStrcat(file_path_name, (S8*)g_vclndr_cntx.FileNamePtr);
	AnsiiToUnicodeString(file_ext, (S8*)VOBJ_VCLNDR_FILE_EXT);
	pfnUnicodeStrcat(file_path_name, file_ext);

	if(mmi_vclndr_uti_is_file_exist(file_path_name))
		result = VOBJ_ERR_DUPLICATE_NAME;
	else
		result = mmi_vclndr_writer(&g_tdl_cntx.TaskList[i], file_path_name);

	OslMfree(file_path_name);

	return result;
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_save_obj_to_file
* DESCRIPTION
*	Save receiving vCalendar object to file.
* PARAMETERS
*	None.
* RETURNS
*	Error codes
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
S32 mmi_vclndr_save_obj_to_file(void)
{
	S32 result = VOBJ_ERR_UNKNOWN;
	S8 *src_path_name;
	S8 *dest_path_name;
	S8 file_ext[VOBJ_MAX_FILENAME_LENGTH];

	if(mmi_vobj_get_file_buffer_size() > 0)
	{
		src_path_name = OslMalloc(VOBJ_MAX_PATH_LENGTH + MAX_TODO_LIST_NOTE);
		mmi_vobj_get_full_path_name(g_vobj_cntx.buffer_begin, src_path_name, MMI_VOBJ_VCLNDR);

		dest_path_name = OslMalloc(VOBJ_MAX_PATH_LENGTH + MAX_TODO_LIST_NOTE);	
		pfnUnicodeStrcpy(dest_path_name, (S8*)g_vclndr_cntx.FilePathPtr);
		pfnUnicodeStrcat(dest_path_name, (S8*)g_vclndr_cntx.FileNamePtr);
		AnsiiToUnicodeString(file_ext, (S8*)VOBJ_VCLNDR_FILE_EXT);
		pfnUnicodeStrcat(dest_path_name, file_ext);

		/*Use move instead of rename because user may save to card.*/
		//result = pfrename(src_path_name,dest_path_name);
#if defined(__FS_MOVE_SUPPORT__)
		mmi_fmgr_send_copy_req(FMGR_ACTION_MOVE, (U8*)src_path_name, (U8*)dest_path_name, mmi_vclndr_save_obj_to_file_callback);
		result = VOBJ_ERR_NO_ERROR;
#else
		result = VOBJ_ERR_UNKNOWN;	/*Should hine menu when MOVE not support.*/
#endif
		
		OslMfree(src_path_name);
		OslMfree(dest_path_name);
	}

	return result;
}

/*****************************************************************************
* FUNCTION
*	mmi_vclndr_save_obj_to_file_callback
* DESCRIPTION
*	Save receiving vCalendar object to file.
* PARAMETERS
*	None.
* RETURNS
*	Error codes
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_save_obj_to_file_callback(void* info)
{
	mmi_fmt_copy_rsp_strcut *msgPtr;

	msgPtr = (mmi_fmt_copy_rsp_strcut *)info;

	if(msgPtr->result >=0)
		mmi_vclndr_save_file_done(VOBJ_ERR_NO_ERROR);
	else
		mmi_vclndr_save_file_done(msgPtr->result);
}

/*****************************************************************************
* FUNCTION
*  mmi_vclndr_fmgr_recv
* DESCRIPTION
*   Receive vCalendar object from file manager.
* PARAMETERS
*  file_path_name		IN		filename and path of selected file. 
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_vclndr_fmgr_recv(S8* file_path_name)
{
	ToDoListNode task;
	
	if(file_path_name == NULL)
		return;

	if(mmi_vclndr_reader(file_path_name, &task) == VOBJ_ERR_NO_ERROR)
	{
		if(TdlInsertTask(&task, TODO_FROM_FMGR))
			return;
		else
			DisplayPopup((PU8)GetString(STR_ID_VCLNDR_TASK_FULL), IMG_GLOBAL_ERROR, TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
	}
	else
		DisplayPopup((PU8)GetString(STR_ID_VOBJ_FORMAT_NOT_SUPPORT), IMG_GLOBAL_ERROR, TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_fmgr_sms_forward
* DESCRIPTION
*	Forward a vCalendar object from file manager via SMS
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_fmgr_sms_forward(S8 *path)
{
#if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)
	mmi_ems_vobject_send_by_ems (0x0000, VOBJ_VCLNDR_PORT, path, NULL);
#endif
}


 /*****************************************************************************
* FUNCTION
*	mmi_vclndr_highlight_opt_done
* DESCRIPTION
*	Highlight handler for "Done" option when editing filename.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_highlight_opt_done(void)
{
	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_vclndr_save_file, KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_save_file
* DESCRIPTION
*	Save vCalendar object to file.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_save_file(void)
{
	S32 result;
	U16 file_path_len;

	file_path_len = pfnUnicodeStrlen(g_vclndr_cntx.FileNamePtr) 
				+ pfnUnicodeStrlen(g_vclndr_cntx.FilePathPtr)
				+VOBJ_MAX_FILE_EXT_LEN/ENCODING_LENGTH;

	if(file_path_len > (FMGR_MAX_PATH_LEN/ENCODING_LENGTH))
	{
		result = VOBJ_ERR_SELECT_PATH_TOO_LONG;
	}
	else if(pfnUnicodeStrlen(g_vclndr_cntx.FileNamePtr) == 0)
	{
		result = VOBJ_ERR_EMPTY_NAME;
	}
	else
	{
		if(g_vclndr_cntx.SrcType == VCLNDR_SAVE_FROM_TDL)
		{
			result = mmi_vclndr_save_tdl_to_file();
		}
		else //if(g_vclndr_cntx.SrcType == VCLNDR_SAVE_FROM_RECV)
		{
			result = mmi_vclndr_save_obj_to_file();
			if (result == VOBJ_ERR_NO_ERROR)
				return;	/*Handle copy result in callback*/	
		}
	}

	mmi_vclndr_save_file_done(result);
}

/*****************************************************************************
* FUNCTION
*	mmi_vclndr_save_file_done
* DESCRIPTION
*	Save vCalendar object to file.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_save_file_done(S32 result)
{
	switch(result)
	{
		case VOBJ_ERR_NO_ERROR:
		{
			mmi_vclndr_free_buffer();
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
			if(g_vclndr_cntx.SrcType == VCLNDR_SAVE_FROM_TDL)
			{
				DeleteUptoScrID(SCR_TDL_TASK_LIST);
			}
			else
			{
				mmi_vobj_clear_one_file_buffer();
				DeleteNScrId(SCR_ID_VOBJ_RECV_OBJECT);
			}

			break;
		}
		case VOBJ_ERR_SELECT_PATH_TOO_LONG:
		{
			DisplayPopup((PU8)GetString(STR_ID_VOBJ_FILE_PATH_TOO_LONG), IMG_GLOBAL_ERROR,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);			
			DeleteUptoScrID(SCR_ID_VCLNDR_FILENAME);

			break;
		}
		case VOBJ_ERR_DUPLICATE_NAME:
		case FS_FILE_EXISTS:
		{
			DisplayPopup((PU8)GetString(STR_ID_VOBJ_DUPLICATE_FILENAME), IMG_GLOBAL_ERROR,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
			DeleteUptoScrID(SCR_ID_VCLNDR_FILENAME);
			
			break;			
		}
		case VOBJ_ERR_EMPTY_NAME:
		{
			DisplayPopup((PU8)GetString(STR_ID_VOBJ_EMPTY_FILENAME), IMG_GLOBAL_ERROR,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
			DeleteUptoScrID(SCR_ID_VCLNDR_FILENAME);	
			
			break;
		}
		case VOBJ_ERR_UNKNOWN:
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_ERROR,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
			if(g_vclndr_cntx.SrcType == VCLNDR_SAVE_FROM_TDL)
				DeleteUptoScrID(SCR_ID_VCLNDR_FORWARD);
			else
				DeleteUptoScrID(SCR_ID_VCLNDR_RECV_OPT);
			
			break;		
		}
		default:  
		{
			DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
			if(g_vclndr_cntx.SrcType == VCLNDR_SAVE_FROM_TDL)
				DeleteUptoScrID(SCR_ID_VCLNDR_FORWARD);
			else
				DeleteUptoScrID(SCR_ID_VCLNDR_RECV_OPT);
			
			break;
		}

	}

}

/*****************************************************************************
* FUNCTION
*	mmi_vclndr_highlight_opt_input_method
* DESCRIPTION
*	Highlight handler for "Input Method" option when editing filename.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_highlight_opt_input_method(void)
{
	SetLeftSoftkeyFunction(EntryInputMethodScreen, KEY_EVENT_UP);
	SetKeyHandler(EntryInputMethodScreen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	RegisterInputMethodScreenCloseFunction(mmi_vclndr_goback_1_history);
}

#endif	/* #if defined(__MMI_FILE_MANAGER__) */


#define VCLNDR_UTILITY
/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_get_line
* DESCRIPTION
*	Get a line from file system, excluding the escape characters.
*       Handle SUMMARY & DESCRIPTION fileds specially.
* PARAMETERS
*	strOutput	IN/OUT	Storage location for data
*	fh		IN		file handle
* RETURNS
*	None.
* GLOBALS AFFECTED
#define VOBJ_READ_BUFF_LEN			512
#define VOBJ_WRITE_BUFF_LEN		1024
*	None.
*****************************************************************************/
S32 mmi_vclndr_uti_get_line(S8* strOutput, FS_HANDLE fh)
{
	S32 nBytes = 0;
	S8 *buffer = OslMalloc(VOBJ_READ_BUFF_LEN);
	S32 i;
	S32 strLen;
	S8* curr_cursor;
	S8* cursor = NULL;

	pfread((INT32)buffer, (UINT8 *)1, VOBJ_READ_BUFF_LEN-1, fh, &nBytes);
	buffer[nBytes] = '\0';

	if(!nBytes)
	{
     		OslMfree(buffer);
		return 0;
	}

	if(strncmp(g_vclndr_fileds[VCLNDR_SUMMARY], buffer, strlen(g_vclndr_fileds[VCLNDR_SUMMARY])) == 0)
	{
		/* summary */
	}
	else if(strncmp(g_vclndr_fileds[VCLNDR_DESCRIPTION], buffer, strlen(g_vclndr_fileds[VCLNDR_DESCRIPTION])) == 0)
	{
		/* description */
	}
	else
	{
		pfseek(fh,0-nBytes, FS_SEEK_CUR);
		OslMfree(buffer);
		return mmi_vobj_uti_get_line(strOutput, fh);
	}

	/* find next filed */
	for(i=VCLNDR_DTSTART;i<VCLNDR_TOTAL_FILEDS;i++)
	{
		if(i ==VCLNDR_SUMMARY || i== VCLNDR_SUMMARY)
		   continue;

		curr_cursor = strstr(buffer, g_vclndr_fileds[i]);

		if(curr_cursor == (S8*)NULL)
		   continue;
		else if(cursor == (S8*)NULL)
		   cursor = curr_cursor;
		else
		{
		   if((S32)cursor > (S32)curr_cursor)
		      cursor = curr_cursor;
		}
	}
	if(cursor == NULL)
	   return 0;

	strLen = (S32)cursor - (S32)buffer;
	strncpy(strOutput, buffer,strLen);
	strOutput[strLen] = VOBJ_NULL;

	pfseek(fh,(S32)(strLen-nBytes), FS_SEEK_CUR);

	OslMfree(buffer);
	return strLen;
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_get_clndr_type
* DESCRIPTION
*	Determine the type of a To Do List task.
* PARAMETERS
*	None.
* RETURNS
*	type of the task.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_vclndr_uti_get_clndr_type(ToDoListNode *TDLNode)
{
	if((TDLNode->EndHour < TDLNode->Time.nHour) 
		|| (TDLNode->EndHour == TDLNode->Time.nHour && TDLNode->EndMin == TDLNode->Time.nMin)) 
	{
		return VCLNDR_TYPE_TODO;
	}
	else
	{
		return VCLNDR_TYPE_EVENT;
	}	
}


/*****************************************************************************
* FUNCTION
*	mi_vclndr_writer
* DESCRIPTION
*	Generate a task to a vCalendar object.
* PARAMETERS
*	TDLNode			IN		task to be convert
*	file_path_name	IN		file path to store the vCalendar object.
* RETURNS
*	Error codes.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
S32 mmi_vclndr_writer(ToDoListNode *TDLNode, S8* file_path_name)
{
	INT32 wByte;
	S32 result;
	FILE_HANDLE		fh;
	S8 strDateTime[VCLNDR_MAX_DT_LEN];
	S8 strBuff[MAX_TODO_LIST_NOTE*2];
	S8 *qpBuff;
	U8 vType;
	S8* vClndrBuff;
	S32 len; 

	fh= MMI_FS_Open(file_path_name, FS_CREATE |FS_READ_WRITE);//Renwy modify 2008/08/07
	
	if(fh < 0)
#ifdef MMI_ON_HARDWARE_P
		return fh;
#else
		return VOBJ_ERR_UNKNOWN;
#endif

	vClndrBuff = OslMalloc(VOBJ_WRITE_BUFF_LEN);

	/* BEGIN:VCALENDAR */
	strcpy(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_BEGIN]);
	strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);

	/* VERSION */
	strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_VERSION]);
	strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);

	/* BEGIN:VEVENT/VTODO */
	vType = mmi_vclndr_uti_get_clndr_type(TDLNode);

	if(vType == VCLNDR_TYPE_EVENT)
		strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_VEVENT_BEGIN]);
	else
		strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_VTODO_BEGIN]);

	strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);

	/* DTSTART: */
	strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_DTSTART]);
	strcat(vClndrBuff, (S8*)VOBJ_VALUE_SEP_STR);
	sprintf(strDateTime,"%d%02d%02d%c%02d%02d%02d",
				TDLNode->Time.nYear,
				TDLNode->Time.nMonth,
				TDLNode->Time.nDay,
				VCLNDR_DT_SEP_CHAR,
				TDLNode->Time.nHour,
				TDLNode->Time.nMin,
				TDLNode->Time.nSec);
	strcat(vClndrBuff, (S8*)strDateTime);
	strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);

	/* DTEND */
	if(TDLNode->Repeat == ONCE)
	{
		if(vType == VCLNDR_TYPE_TODO)
		{
			strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_DTEND]);
			strcat(vClndrBuff, (S8*)VOBJ_VALUE_SEP_STR); 
			strcat(vClndrBuff, (S8*)strDateTime);
			strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);	
		}
		else
		{
			strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_DTEND]);
			strcat(vClndrBuff, (S8*)VOBJ_VALUE_SEP_STR); 

			sprintf(strDateTime,"%d%02d%02d%c%02d%02d%02d",
				TDLNode->Time.nYear,
				TDLNode->Time.nMonth,
				TDLNode->Time.nDay,
				VCLNDR_DT_SEP_CHAR,
				TDLNode->EndHour,
				TDLNode->EndMin,
				TDLNode->Time.nSec);
			strcat(vClndrBuff, (S8*)strDateTime);
			strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);	
		}
	}

	/* Alarm */
	if(TDLNode->Alarm == TRUE)
	{
		/* DALARM */
		strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_DALARM]);
		strcat(vClndrBuff, (S8*)VOBJ_VALUE_SEP_STR);
   		strcat(vClndrBuff, (S8*)strDateTime);
		strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);

		/* AALARM */
		strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_AALARM]);
		strcat(vClndrBuff, (S8*)VOBJ_VALUE_SEP_STR);
		strcat(vClndrBuff, (S8*)strDateTime);
		strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);
	}

	/* SUMMARY: */
	strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_SUMMARY]);
	qpBuff = OslMalloc(VOBJ_WRITE_BUFF_LEN);
	mmi_chset_convert(MMI_CHSET_UCS2, MMI_CHSET_UTF8, (S8*)TDLNode->Note, strBuff, sizeof(strBuff));
	applib_qprint_encode_ext(strBuff, strlen(strBuff), qpBuff, VOBJ_WRITE_BUFF_LEN, TRUE);
	strcat(vClndrBuff, (S8*)VCLNDR_PARA_SEP_STR);	// ";"
	strcat(vClndrBuff, (S8*)g_vobj_paras[VOBJ_PARA_ENCODING]);	// ENCODING
	strcat(vClndrBuff, (S8*)VCLNDR_EQUAL_STR);		// "="
	strcat(vClndrBuff, (S8*)g_vobj_encoding[2]);	// "QP"
	strcat(vClndrBuff, (S8*)VCLNDR_PARA_SEP_STR);	// ";"
	strcat(vClndrBuff, (S8*)g_vobj_paras[VOBJ_PARA_CHARSET]);	// CHARSET
	strcat(vClndrBuff, (S8*)VCLNDR_EQUAL_STR);		// "="
	strcat(vClndrBuff, (S8*)VCLNDR_CHARSET_UTF8);	// "UTF-8"

	strcat(vClndrBuff, (S8*)VOBJ_VALUE_SEP_STR);
	strcat(vClndrBuff, (S8*)qpBuff);
	strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);
	OslMfree(qpBuff);

	/* RRULE: */
	if(TDLNode->Repeat != ONCE)
	{
		strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_RRULE]);
		strcat(vClndrBuff, (S8*)VOBJ_VALUE_SEP_STR);

		switch(TDLNode->Repeat)
		{
			case EVERYDAY:
			{
				strcat(vClndrBuff, (S8*)g_vclndr_recurr[VCLNDR_DAILY]);
				break;
			}
			case DAYS:
			{
				S32 i;
				strcat(vClndrBuff, (S8*)g_vclndr_recurr[VCLNDR_WEEKLY]);
				memset(strBuff, 0, sizeof(strBuff));
				for(i=0;i<VCLNDR_NUM_OF_DAYS;i++)
				{
					if(TDLNode->Days & (0x1 << i))
					{
						sprintf(strDateTime," %s", g_vclndr_days[i]);
						strcat(strBuff, strDateTime);
					}
				}
				strcat(vClndrBuff, (S8*)strBuff);
				break;
			}			
			case WEEKLY:
			{
				strcat(vClndrBuff, (S8*)g_vclndr_recurr[VCLNDR_WEEKLY]);
				break;
			}
			case MONTHLY:
			{
				strcat(vClndrBuff, (S8*)g_vclndr_recurr[VCLNDR_MONTHLY]);
				sprintf(strBuff, " %d", TDLNode->Time.nDay);
				strcat(vClndrBuff, (S8*)strBuff);
				break;
			}
		}
		/* #0 and CRLF */
		strcat(vClndrBuff, (S8*)VCLNDR_R_FOREVER_STR);
		strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);
	}

	/* END:VEVENT/VTODO */
	if(vType == VCLNDR_TYPE_EVENT)
		strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_VEVENT_END]);
	else
		strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_VTODO_END]);

	strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);

	/* END:VCALENDAR */
	strcat(vClndrBuff, (S8*)g_vclndr_fileds[VCLNDR_END]);
	strcat(vClndrBuff, (S8*)VOBJ_CRLF_STR);

	len = strlen(vClndrBuff);
	MMI_ASSERT(len <= VOBJ_WRITE_BUFF_LEN);

	result = MMI_FS_Write(fh,vClndrBuff,len,&wByte);
	MMI_FS_Close(fh);//Renwy modify 2008/08/07

	if(result < 0)
		MMI_FS_Delete(file_path_name);//Renwy modify 2008/08/07
	
	OslMfree(vClndrBuff);
	
	return result;

}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_dt_is_valid
* DESCRIPTION
*   To compute is the date/time is within reasonable range (1970 ~ 2030)
* PARAMETERS
*	t	time to be computed
* RETURNS	
*	TURE if date/time valid; otherwise, FALSE
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_vclndr_dt_is_valid(MYTIME *t)
{
	/* error handling */
	if(t->nYear > 2030 || t->nYear < 1970)
		return FALSE;
	
	if(t->nMonth > 12)
		return FALSE;

	if(t->nDay > LastDayOfMonth(t->nMonth, t->nYear))
		return FALSE;

	if(t->nHour > 23)
		return FALSE;

	if(t->nMin > 59)
		return FALSE;

	if(t->nSec > 59)
		return FALSE;

	return TRUE;

}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_reader
* DESCRIPTION
*	Parse a vCalendar file
* PARAMETERS
*	file_path_name	IN			filename including whole path
*	TdlNode			IN/OUT		task buffer to store the result
* RETURNS
*	Error cause
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
S32 mmi_vclndr_reader(S8 *file_path_name, ToDoListNode *TdlNode)
{
	FILE_HANDLE fh = (int)NULL;					/* file handle */
	S8	inBuff[VOBJ_READ_BUFF_LEN];			/* buffer to store input */
	S8	DescritionBuff[MAX_TODO_LIST_NOTE];	/* buffer to store input */
	S8	*strValue;							/* pointer to value of filed */	

	U8	filedType;
	U8	IsDTValid = FALSE;
	U8	IsObjValid = FALSE;

	MYTIME	DTStart, DTEnd, DTDue;

	memset(&DTStart, 0, sizeof(MYTIME));
	memset(&DTDue, 0, sizeof(MYTIME));
	memset(&DTEnd, 0, sizeof(MYTIME));
	memset(TdlNode, 0, sizeof(ToDoListNode));
	memset(DescritionBuff, 0, sizeof(DescritionBuff));

	fh= pfopen(file_path_name,PFS_READ);
	if(fh<=0)
		return fh;

	/* BEGIN:VCALENDAR */
	if(mmi_vclndr_uti_get_line(inBuff, fh) != 0)
	{
		mmi_vclndr_uti_remove_ws(inBuff, (S8*)VOBJ_VALUE_SEP_STR);
		if(mmi_vclndr_uti_check_field(inBuff) != VCLNDR_BEGIN)
		{
			pfclose(fh);
			return VOBJ_ERR_UNSUPPORT_FORMAT;
	   	}
	}

	/* VERSION */
	while(mmi_vclndr_uti_get_line(inBuff, fh)!= 0)
	{
		mmi_vclndr_uti_remove_ws(inBuff, (S8*)VOBJ_VALUE_SEP_STR);	
		if(mmi_vclndr_uti_check_field(inBuff) == VCLNDR_VERSION)
			break;
	}


	/* VEVENT or VTODO */
	while(mmi_vclndr_uti_get_line(inBuff, fh)!= 0)
	{
		mmi_vclndr_uti_remove_ws(inBuff, (S8*)VOBJ_VALUE_SEP_STR);	
		filedType = mmi_vclndr_uti_check_field(inBuff);
		if(filedType == VCLNDR_VEVENT_BEGIN)
			break;
		else if(filedType == VCLNDR_VTODO_BEGIN)
			break;
	}

	/* DTSTART, DTEND, SUMMARY, DESCRIPTION, DUE, RRULE */
	while(mmi_vclndr_uti_get_line(inBuff, fh)!= 0)
	{
		mmi_vclndr_uti_remove_ws(inBuff, (S8*)VOBJ_VALUE_SEP_STR);
		filedType = mmi_vclndr_uti_check_field(inBuff);
		strValue = (S8*)mmi_vobj_uti_get_value(inBuff);

		switch(filedType)
		{
			case VCLNDR_DTSTART:
				DTStart = mmi_vclndr_uti_get_time(strValue);
				break;
			case VCLNDR_DTEND:
				DTEnd = mmi_vclndr_uti_get_time(strValue);
				break;
			case VCLNDR_DUE:
				DTDue = mmi_vclndr_uti_get_time(strValue);
				break;
			case VCLNDR_SUMMARY:
			case VCLNDR_DESCRIPTION:
			{
				//S8	Langbuff[VCLNDR_MAX_PARAM_LEN];		/* buffer to store language */
				S8 *Encodebuff = OslMalloc(VCLNDR_MAX_PARAM_LEN);	/* buffer to store encoding type */
				S8 *Charsetebuff = OslMalloc(VCLNDR_MAX_PARAM_LEN);/* buffer to store charset */
				S8 *ResultBuff  = OslMalloc(2048); /* for UTF-8 case */

				/* currently not supported */
				#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
				#endif

				/* encoding type */
				strcpy(ResultBuff, strValue);
				if(mmi_vclndr_uti_get_param(inBuff, (S8*)g_vobj_paras[VOBJ_PARA_ENCODING], Encodebuff, VCLNDR_MAX_PARAM_LEN))
				{
					if(mmi_vclndr_uti_get_encoding_type(Encodebuff) == VOBJ_EN_QP)
					{
						applib_qprint_decode(strValue, strlen(strValue), ResultBuff, 2048);
					}
				}

				/* charset */
				if(mmi_vclndr_uti_get_param(inBuff, (S8*)g_vobj_paras[VOBJ_PARA_CHARSET], Charsetebuff, VCLNDR_MAX_PARAM_LEN))
				{
					if(mmi_vclndr_uti_get_charset(Charsetebuff) == VOBJ_CH_UTF8)
						mmi_chset_convert(MMI_CHSET_UTF8, MMI_CHSET_UCS2, ResultBuff, DescritionBuff, MAX_TODO_LIST_NOTE);
					else
					{
   						U8 charset = PhnsetGetDefEncodingType();
   						mmi_chset_convert((mmi_chset_enum)charset, MMI_CHSET_UCS2, ResultBuff, DescritionBuff, MAX_TODO_LIST_NOTE);   					
	//					AnsiiNToUnicodeString((S8*)DescritionBuff, ResultBuff, MAX_TODO_NOTE_LEN-1);
					}
				}
				else
				{
   				U8 charset = PhnsetGetDefEncodingType();
   				mmi_chset_convert((mmi_chset_enum)charset, MMI_CHSET_UCS2, ResultBuff, DescritionBuff, MAX_TODO_LIST_NOTE);
//					AnsiiNToUnicodeString((S8*)DescritionBuff, ResultBuff, MAX_TODO_NOTE_LEN-1);
				}


				if(filedType == VCLNDR_SUMMARY)
					pfnUnicodeStrcpy((S8*)TdlNode->Note, DescritionBuff);

				OslMfree(Encodebuff);
				OslMfree(Charsetebuff);
				OslMfree(ResultBuff);
				break;
			}
			case VCLNDR_RRULE:
			{
							/* RRULE */
				vclndr_recurr_struct recurr;
				if(mmi_vclndr_uti_get_recurr(strValue, &recurr))
				{
					TdlNode->Repeat = recurr.Repeat;
					TdlNode->Days = recurr.Days;
				}
				else
				{
					TdlNode->Repeat = ONCE;
				}
				
				break;
			}
			case VCLNDR_AALARM:
			case VCLNDR_DALARM:
				TdlNode->Alarm = TRUE;
				break;
			case VCLNDR_VEVENT_END:
			case VCLNDR_VTODO_END:
			case VCLNDR_END:
				break;
			default:
				break;
		}
	}

	pfclose(fh);

	/* store date and time */
	if(mmi_vclndr_dt_is_valid(&DTStart))
	{
		TdlNode->Time = DTStart;

		/* if end date is the same date */
		if(DTStart.nYear == DTEnd.nYear &&
			DTStart.nMonth == DTEnd.nMonth &&
				DTStart.nDay == DTEnd.nDay)
		{
			TdlNode->EndHour = DTEnd.nHour;
			TdlNode->EndMin = DTEnd.nMin;
		}
		IsDTValid = TRUE;
	}
	else if(mmi_dt_is_valid(&DTDue))
	{
		TdlNode->Time = DTDue;
		IsDTValid = TRUE;
	}
#if 0	
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	if(pfnUnicodeStrlen((S8*)TdlNode->Note))
		IsObjValid = TRUE;	
	else if(pfnUnicodeStrlen((S8*)DescritionBuff))
	{
		pfnUnicodeStrncpy((S8*)TdlNode->Note, (S8*)DescritionBuff, MAX_TODO_NOTE_LEN-1);
		IsObjValid = TRUE;
	}
	
	if(IsDTValid && IsObjValid)
		return VOBJ_ERR_NO_ERROR;
	else
		return VOBJ_ERR_UNSUPPORT_FORMAT;

}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_check_field
* DESCRIPTION
*	Compute the filed name of a given string.s
* PARAMETERS
*	String	IN	string to be checked
* RETURNS
*	Type of the filed
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_vclndr_uti_check_field(S8* String)
{
	S32 i;

	if(strlen(String) == 0)
		return VCLNDR_TOTAL_FILEDS;

	for(i=0; i<VCLNDR_TOTAL_FILEDS; i++)
		if(mmi_vobj_uti_cmp_filed(String, g_vclndr_fileds[i]))
			break;

	return i;	
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_remove_ws
* DESCRIPTION
*	Remove white sapce of a string.
* PARAMETERS
*	String		IN/OUT		string to be converted
*	Seperator 	IN			WS before and next to this Seperator will be removed.
* RETURNS
*	 None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_uti_remove_ws(S8* String, S8* Seperator)
{
	S8 *sCursor;	/* position of start seperator */
	S8 *eCursor;	/* position of end of seperator */
	S8* currCursor;

	/* locate to Seperator */
	if((sCursor = strstr(String, Seperator)) != NULL)
	{
		if(sCursor == String)	/* to prevent abnormal case */
			return;

		/* remove ws after seperator */
		currCursor = eCursor = sCursor+strlen(Seperator);
		while((currCursor[0] == VCLNDR_LWSP_CHAR 
			|| currCursor[0] == VCLNDR_TAB_CHAR) 
			&& (currCursor[0] != (S8)NULL))
		{
			currCursor++;
		}

		if(currCursor != eCursor)
			strcpy(eCursor, currCursor);

		/* remove ws before seperator */
		currCursor = sCursor-1;
		while((currCursor[0] == VCLNDR_LWSP_CHAR 
			|| currCursor[0] == VCLNDR_TAB_CHAR) 
			&& (currCursor != String))
		{
			currCursor--;
		}

		currCursor++;
		if(currCursor != sCursor)
			strcpy(currCursor, sCursor);	
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_get_time
* DESCRIPTION
*	Get time value from a date/time string
* PARAMETERS
*	strTime	IN	date/time string in yyyymmddThhmmssZ format.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
MYTIME	mmi_vclndr_uti_get_time(S8* strTime)
{
	MYTIME result;
	S8* strCursor = strTime;
	S8 strBuff[VCLNDR_MAX_DT_LEN];

	memset(&result, 0, sizeof(MYTIME));

	/* year */
	memset(strBuff, 0, VCLNDR_MAX_DT_LEN);
	AnsiiNToUnicodeString(strBuff, strCursor, 4);
	result.nYear = (U16)coolsand_UI_atoi((U16*)strBuff);
	strCursor += 4;

	/* month */
	memset(strBuff, 0, VCLNDR_MAX_DT_LEN);	
	AnsiiNToUnicodeString(strBuff, strCursor, 2);
	result.nMonth = (U16)coolsand_UI_atoi((U16*)strBuff);
	strCursor += 2;

	/* day */
	memset(strBuff, 0, VCLNDR_MAX_DT_LEN);	
	AnsiiNToUnicodeString(strBuff, strCursor, 2);
	result.nDay = (U16)coolsand_UI_atoi((U16*)strBuff);
	strCursor += 2;

	/* ignore 'T' */
	if(strCursor[0] != VCLNDR_DT_SEP_CHAR)
	{
		memset(&result, 0, sizeof(MYTIME));
		return result;
	}

	strCursor++;

	/* hour */
	memset(strBuff, 0, VCLNDR_MAX_DT_LEN);	
	AnsiiNToUnicodeString(strBuff, strCursor, 2);
	result.nHour = (U16)coolsand_UI_atoi((U16*)strBuff);
	strCursor += 2;

	/* minute */
	memset(strBuff, 0, VCLNDR_MAX_DT_LEN);	
	AnsiiNToUnicodeString(strBuff, strCursor, 2);
	result.nMin = (U16)coolsand_UI_atoi((U16*)strBuff);	
	strCursor += 2;

	/* second */
	memset(strBuff, 0, VCLNDR_MAX_DT_LEN);	
	AnsiiNToUnicodeString(strBuff, strCursor, 2);
	result.nSec = (U16)coolsand_UI_atoi((U16*)strBuff);
	strCursor += 2;

	if(strCursor[0] == VCLNDR_UTC_CHAR)	/* UTC time */
	{
		S16 TimeZone;
		U8 Neglect = FALSE;
		MYTIME incTime;
		memset(&incTime, 0, sizeof(MYTIME));
		/* convert to local time */
		TimeZone = (S16)(GetTimeZone(PhnsetGetHomeCity())*60);
		if(TimeZone < 0)
		{
			Neglect = TRUE;
			TimeZone = 0 - TimeZone;
		}

		incTime.nHour = TimeZone/60;
		incTime.nMin = TimeZone%60;

		if(Neglect)
			DecrementTime(result, incTime, &result);
		else
			result = IncrementTime(result, incTime);
	}
	return result;
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_get_recurr
* DESCRIPTION
*	Get recurrence of a vCalendar object from an RRULE string
* PARAMETERS
*	strRRule	IN			string represents the RRULE
*	recurr		IN/OUT		struct variable the store the result of converting
* RETURNS
*	TURE if the RRULE supported, otherwise, FALSE
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8	mmi_vclndr_uti_get_recurr(S8* strRRule, vclndr_recurr_struct* recurr)
{
	S8*	tmpString = strRRule;
	S8	compString[VCLNDR_MAX_DT_LEN];

	recurr->Days = 0;

	if(strlen(tmpString) < VCLNDR_MIN_RECURR_LEN) 	/* string too short */
		return FALSE;

	/* Daily */
	sprintf(compString, "%s %c",g_vclndr_recurr[VCLNDR_DAILY], VCLNDR_POUND_CHAR);
	if(mmi_vobj_uti_cmp_filed(tmpString, compString))
	{
		recurr->Repeat = EVERYDAY;
		return TRUE;
	}

	/* Weekly */
	sprintf(compString, "%s %c",g_vclndr_recurr[VCLNDR_WEEKLY], VCLNDR_POUND_CHAR);
	if(mmi_vobj_uti_cmp_filed(tmpString, compString))	
	{
		recurr->Repeat = WEEKLY;
		return TRUE;
	}

	/* Days */
	if(mmi_vobj_uti_cmp_filed(tmpString, (S8*)g_vclndr_recurr[VCLNDR_WEEKLY]))	
	{
		/* get week days */
		tmpString +=3;	/* move to starting point */

		recurr->Days = mmi_vclndr_uti_get_days(tmpString);
		if(recurr->Days)
		{
			recurr->Repeat = DAYS;
			return TRUE;
		}
		else
			return FALSE;
	}

	/* Monthly */
	sprintf(compString, "%s %c",g_vclndr_recurr[VCLNDR_MONTHLY], VCLNDR_POUND_CHAR);
	if(mmi_vobj_uti_cmp_filed(tmpString, g_vclndr_recurr[VCLNDR_MONTHLY]))	
	{
		recurr->Repeat = MONTHLY;
		return TRUE;
	}

	return FALSE;
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_get_days
* DESCRIPTION
*	Get the day of week value from string
* PARAMETERS
*	strDays		IN			string to be converted
* RETURNS
*	the value of days if found, otherwise 0;
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8	mmi_vclndr_uti_get_days(S8* strDays)
{
	U8 result = 0;
	S8* tmpString = strDays;
	S32 i;

	while(tmpString[0] == VCLNDR_LWSP_CHAR)
		tmpString++;

	while(tmpString[0] != VOBJ_NULL && tmpString[0] != VCLNDR_POUND_CHAR)
	{
		for(i=0; i<VCLNDR_NUM_OF_DAYS; i++)
		{
			if(!mmi_vobj_uti_strnicmp(tmpString, (S8*)g_vclndr_days[i], 2))
			{
				result |= (0x1 << i);
				tmpString+=2;				
				break;
			}
		}

		if(i == VCLNDR_NUM_OF_DAYS)
			return 0;

		if(tmpString[0] != VCLNDR_LWSP_CHAR)
			return 0;

		tmpString++;

	}

	return result;
}


/*****************************************************************************
* FUNCTION
*	mmi_vlcndr_uti_flush_to_file
* DESCRIPTION
*	Write string buffer to file.
* PARAMETERS
*	fh		IN		file handle
*	strBuff	IN		string to be write
* RETURNS
*	TRUE if success; otherwise FALSE
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL mmi_vlcndr_uti_flush_to_file(FILE_HANDLE fh, S8* strBuff)
{
	//S32 wByte;
	if(pfwrite((INT32)strBuff,(INT8 *)1,AsciiStrlen((const S8*)strBuff),fh,&wByte) == FS_NO_ERROR)
		return TRUE;

	return FALSE;
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_get_param
* DESCRIPTION
*	To retrieve the parameter value from a string
* PARAMETERS
*	source		IN		source string to be computed.
*	para_name	IN		parameter to be retrieve.
*	result		IN/OUT	value of parameter
*	max_len		IN		maximum length to be convert
* RETURNS
*	TRUE if parameter found; otherwise FALSE
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_vclndr_uti_get_param(S8 *source, S8 *para_name, S8 *result, S32 max_len)
{
	S8 *cursor;
	S32 i;
	if((cursor = strstr(source, para_name)) == NULL)
		return FALSE;

	if((cursor = strstr(cursor, (S8*)VCLNDR_EQUAL_STR)) == NULL)  /* move cursor next to "=" */
		return FALSE;

	cursor++;

	for(i=0; i<max_len; i++,cursor++)
	{
		if(cursor[0] == VCLNDR_VALUE_SEP_CHAR 
			|| cursor[0] == VCLNDR_PARA_SEP_CHAR
			|| cursor[0] == VOBJ_NULL)
			break;
			
		result[i] = cursor[0];
	}

	result[i] = VOBJ_NULL;

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_get_charset
* DESCRIPTION
*	To retrieve the charset
* PARAMETERS
*	string		IN		string to be parsed
* RETURNS
*	Enum value of charset.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_vclndr_uti_get_charset(S8* string)
{
	/* currently only support UTF-8 */
	if(!mmi_vobj_uti_strnicmp((S8*)string, (S8*)g_vobj_charset[0], strlen(g_vobj_charset[0])))
		return VOBJ_CH_UTF8;
	else
		return 0xff;
}



/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_get_encoding_type
* DESCRIPTION
*	To retrieve the coding type
* PARAMETERS
*	string		IN		string to be parsed
* RETURNS
*	Enum value of encoding type.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_vclndr_uti_get_encoding_type(S8* string)
{
	S32 i;

	for(i=0; i<VOBJ_EN_TOTAL; i++)
	{
		if(!mmi_vobj_uti_strnicmp((S8*)string, (S8*)g_vobj_encoding[i], strlen(g_vobj_encoding[i])))
			break;
	}

	return i;
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_is_file_exist
* DESCRIPTION
*	To check if file is already exist
* PARAMETERS
*	file_path_name		IN		file path and name
* RETURNS
*	TURE of file exists; otherwise FALSE.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_vclndr_uti_is_file_exist(S8* file_path_name)
{
	FILE_HANDLE fh;
	/* if file already exist */
	fh = pfopen(file_path_name, PFS_READ);
	
	if(fh > 0)
	{
		pfclose(fh);
		return TRUE;
	}

	return FALSE;
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_get_default_filename
* DESCRIPTION
*	To generate default file name used for other application
* PARAMETERS
*	result	IN/OUT		pointer to the result
* RETURNS
*	TURE of file exists; otherwise FALSE.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_vclndr_uti_get_default_filename(S8* result)
{
	S8 ascii_path[VOBJ_MAX_PATH_LENGTH/ENCODING_LENGTH];

	sprintf(ascii_path,"%c:/%s/%s",VOBJ_DRV,VOBJ_FILE_PATH,VOBJ_VCLNDR_DEFAULT_FILE_NAME);//Renwy modify 2008/08/07
	AnsiiToUnicodeString(result, ascii_path);
}


/*****************************************************************************
* FUNCTION
*	mmi_vclndr_uti_is_file_exist
* DESCRIPTION
*	To delete default file buffer used for other application
* PARAMETERS
*	none
* RETURNS
*	TURE of file exists; otherwise FALSE.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
extern BOOL MMI_FS_IsExist(const UINT8 * FileName);
void mmi_vclndr_uti_del_default_file(void)
{
	S8 path[VOBJ_MAX_PATH_LENGTH];
	S8 ascii_path[VOBJ_MAX_PATH_LENGTH/ENCODING_LENGTH];

	sprintf(ascii_path,"%c:/%s/%s",VOBJ_DRV,VOBJ_FILE_PATH,VOBJ_VCLNDR_DEFAULT_FILE_NAME);//Renwy modify 2008/08/07
	AnsiiToUnicodeString(path, ascii_path);
	if(MMI_FS_IsExist(path))
	{
	    MMI_FS_Delete(path);
	}
}


#define	VCLNDR_TEMP_USAGE
static const unsigned char widget_utf8_bytes_per_character[16] = 
{
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 2, 2, 3, 0 /* we don't support UCS4 */
};

S32 utf8_strlen(const U8 *s)
{
	int count = 0;
	int rawlen;
	int d;

	if (s == 0) return 0;

	rawlen = strlen((const char *)s);

	while (*s != '\0' && rawlen>0) 
	{
		d = widget_utf8_bytes_per_character[*s >> 4];
		if(d==0) 
			return count;
		rawlen -= d;
		s += d;

		count++;
	}
	
	return count;
}


S32 utf8_to_ucs2 (U16 *ucs2, U8 *utf8)
{
	unsigned char c = utf8[0];

	if (c < 0x80) 
	{
		*ucs2 = c;
		return 1;
	} 
	else if (c < 0xe0)
	{
		*ucs2 = ((U16) (c & 0x1f) << 6) | (U16) (utf8[1] ^ 0x80);
		return 2;
	} 
	else
	{
	    *ucs2 =	  ((U16) (c & 0x0f) << 12)
				| ((U16) (utf8[1] ^ 0x80) << 6)
				|  (U16) (utf8[2] ^ 0x80);
		return 3;
	}
}

S32 ucs2_to_utf8 (U8 *utf8, U16 ucs2)
{
  int count;
  if (ucs2 < 0x80)
    count = 1;
  else if (ucs2 < 0x800)
    count = 2;
  else
    count = 3;
  switch (count) 
  {
    case 3: utf8[2] = 0x80 | (ucs2 & 0x3f); ucs2 = ucs2 >> 6; ucs2 |= 0x800;
    case 2: utf8[1] = 0x80 | (ucs2 & 0x3f); ucs2 = ucs2 >> 6; ucs2 |= 0xc0;
    case 1: utf8[0] = (U8)ucs2;
  }
  return count;
}

S32 ucs2_to_utf8_len(U16 ucs2)
{
	if (ucs2 < 0x80)
		return 1;
	else if (ucs2 < 0x800)
		return 2;
	else
		return 3;
}


S32 utf8_to_ucs2_string(U8 *dest,int dest_size,U8* src)
{
	int pos = 0;
	
	// FIXME	1. we should check the sanity of the UTF8 string 
	// 			2. this check is not correct when dest_size < 2 or dest_size is odd
	while(*src)
	{
		src += utf8_to_ucs2((U16*)(dest+pos),src);
		pos += 2;
		if(pos>=dest_size-2) break;
	}
	dest[pos] = 0;
	dest[pos+1] = 0;
	return pos+2;
}

S32 ucs2_to_utf8_string(U8 *dest,S32 dest_size,U8* src)
{
	S32 pos = 0;

	while( src[0] || src[1])
	{
		if (pos + ucs2_to_utf8_len(*(U16*)src) >= dest_size) // leave space of '\0'
		{
			break;
		}
		
		pos += ucs2_to_utf8(dest+pos,(*(U16*)src));
		src += 2;
		if(pos>=dest_size-1) break;
	}
	if(pos>=dest_size) {
		dest[dest_size-1] = 0;
	} else
	    dest[pos] = 0;
	return pos+1;
}

#endif /* #if defined(__MMI_VCALENDAR__)*/
#endif /*END #define _MMI_VCALENDAR_C*/


