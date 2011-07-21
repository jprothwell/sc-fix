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
 *	Shortcuts.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements Shortcuts application.
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

/**************************************************************

	FILENAME	: Shortcuts.c

  	PURPOSE		: Shortcuts Application

 

 

	DATE		: May 12,03

**************************************************************/
#include "mmi_features.h"
#include "mmi_trace.h"

#ifdef SHORTCUTS_APP

#ifndef _MMI_SHORTCUTS_C
#define _MMI_SHORTCUTS_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "shortcutsdefs.h"
#include "commonscreens.h"
#include "shortcutsprots.h"
#include "mainmenudef.h"
#include "custmenures.h"
#include "eventsdef.h"
#include "custdatares.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_softkeys.h"
#include "unicodexdcl.h"
#include "debuginitdef.h"
#include "wrappergprot.h"
#include "timerevents.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "idleappdef.h"
#include "profilegprots.h"
#include "globaldefs.h"
#include "audioinc.h"
#include "messagesresourcedata.h"
#include "settingdefs.h"
/*dongwg Add Start For 6152 Ver: TBM780  on 2007-8-9 13:56 */
#include "smsguiinterfaceprot.h"
#include "profiles_prot.h"
/*dongwg Add End  For 6152 Ver: TBM780  on 2007-8-9 13:56 */
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#undef __NEWSIMULATOR 
/*
** Define
*/


/* 
** Typedef 
*/


/*
** Local Variable
*/



/* 
** Local Function
*/

/* 
** Global Variable
*/
shct_context_struct g_shct_cntx;
extern CUSTOM_MENU		nCustMenus[];
extern hiliteInfo maxHiliteInfo[MAX_HILITE_HANDLER];			/* available hilite func ptrs */

/* retrieve shortcut from custom folder */
extern const U8 shct_max_list_cand;
extern const U8 shct_max_list_default;
extern const U16 gShctCandList[];
extern const U16 gShctDefaultList[];
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
extern U16 gShctDefaultDediList[];
#endif

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
extern void  EntryShctOptionScreen(void);

/* 
** Global Function
*/

/*****************************************************************************
* FUNCTION
*	ShctInit
* DESCRIPTION
*   Initialises variables & Protocol Handlers for Shortcuts
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctInit(void)
{
	ShctGetCandidateList();

	ShctReadFromNvram();

	#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
		g_shct_cntx.IsKeyRetrieving = FALSE;
		ShctReadDedicatedListFromNVRAM();
	#endif
}


/*****************************************************************************
* FUNCTION
*	ShctWriteToNvram
* DESCRIPTION
*   Write shortcut data to NVRAM.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctWriteToNvram(void)
{
	U8 nvramData[SHCT_NVRAM_SIZE];
	S16 error;

	memset(nvramData, 0xff, sizeof(nvramData));
	nvramData[SHCT_COUNT_BYTE] = g_shct_cntx.NumSelShct;
	nvramData[SHCT_INTEGRITY_BYTE]=1;	

	memcpy((void*)&nvramData[SHCT_DATA_BYTE],(void*)&g_shct_cntx.SelShctList, sizeof(U16)*g_shct_cntx.NumSelShct);

	WriteRecord(NVRAM_EF_SHORTCUTS_LID,1,(void*)nvramData, SHCT_NVRAM_SIZE, &error);
}


/*****************************************************************************
* FUNCTION
*	ShctGetCandidateList
* DESCRIPTION
*   Get candidate list of shortcuts.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctGetCandidateList()
{
	S32 i;
	S32 count = (S32)shct_max_list_cand;
	g_shct_cntx.NumCandShct = 0;
	
	for(i=0;i<count;i++)
		if(maxHiliteInfo[gShctCandList[i]].entryFuncPtr)
			g_shct_cntx.CandShctList[g_shct_cntx.NumCandShct++] = gShctCandList[i];
}


/*****************************************************************************
* FUNCTION
*	ShctReadFromNvram
* DESCRIPTION
*   Read shortcut data from NVRAM and initialize selected shortcut list.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctReadFromNvram(void)
{
	S16  j=0, total=0, index = 0;

	U8 	nvramData[SHCT_NVRAM_SIZE];
	U16 menuID = 0;
	S16 error = 0;
	S8	shortcut_not_found = FALSE;

	ReadRecord(NVRAM_EF_SHORTCUTS_LID,1,(void *)nvramData, SHCT_NVRAM_SIZE, &error);
	
	if(nvramData[1] != 1)
	{
		nvramData[SHCT_COUNT_BYTE] = SHCT_MAX_SEL_LIST;
		nvramData[SHCT_INTEGRITY_BYTE] = 1;
		memcpy((void*)&nvramData[SHCT_DATA_BYTE],(void*)&gShctDefaultList, sizeof(U16)*shct_max_list_default);
		WriteRecord(NVRAM_EF_SHORTCUTS_LID,1,(void *)nvramData, SHCT_NVRAM_SIZE, &error);
	}

	total = nvramData[SHCT_COUNT_BYTE];
	for(j=0; j<total; j++)
	{
		memcpy(&menuID, &nvramData[j*2+SHCT_DATA_BYTE], 2);
		
		if(maxHiliteInfo[menuID].entryFuncPtr)
			g_shct_cntx.SelShctList[index++] = menuID;
		else
		{
			shortcut_not_found = TRUE;
			break;
		}
	}
#ifdef MMI_ON_HARDWARE_P
	//MMI_ASSERT(shortcut_not_found == TRUE);
#endif
	g_shct_cntx.NumSelShct = index;
}


/*****************************************************************************
* FUNCTION
*	ShctMenuShortcut
* DESCRIPTION
*   Shortcut handler of selected menu item
* PARAMETERS
*	a  IN	index of menu item
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ShctMenuShortcut(S32 index)
{
	if(index>=0 && index<g_shct_cntx.NumSelShct) 
		execute_left_softkey_function(KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	EntryShctInIdle
* DESCRIPTION
*   Display shorcut list in IDLE screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryShctInIdle(void)
{
	U8 i;
	U8 *guiBuffer;
#ifdef __MMI_TOUCH_SCREEN__
	S32 numShct;
#endif

#ifdef __MMI_TOUCH_SCREEN__
	U16 ShortcutList[SHCT_MAX_SEL_LIST+1];
#else
	U16 ShortcutList[SHCT_MAX_SEL_LIST];
#endif /*__MMI_TOUCH_SCREEN__*/
		
	EntryNewScreen(SCR_ID_SHCT_IDLE_SHORTCUT, NULL, EntryShctInIdle, NULL);
	
	for(i=0; i<g_shct_cntx.NumSelShct; i++)
		ShortcutList[i] = ShctGetStringID(g_shct_cntx.SelShctList[i]);

#ifdef __MMI_TOUCH_SCREEN__
	numShct = (S32)g_shct_cntx.NumSelShct;

	/* Last Entry is always Calibration*/
	if (maxHiliteInfo[MENU_SETTING_CALIBRATION].entryFuncPtr)
	{
		g_shct_cntx.SelShctList[numShct] = MENU_SETTING_CALIBRATION;
		ShortcutList[numShct++] = ShctGetStringID(MENU_SETTING_CALIBRATION);
	}
#endif /*__MMI_TOUCH_SCREEN__*/

	if(g_shct_cntx.CurrSelShct>g_shct_cntx.NumSelShct-1) 
		g_shct_cntx.CurrSelShct=0;
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SHCT_IDLE_SHORTCUT);
	
	ClearAllKeyHandler();
	RegisterHighlightHandler(HighlightShctSelShortcut);

	if (0 == g_shct_cntx.NumSelShct)
	{
		ShowCategory15Screen(SHORTCUTS_TITLE_STRING_ID,GetRootTitleIcon(MAIN_MENU_SHORTCUTS_MENUID),
							0,0,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							g_shct_cntx.NumSelShct, ShortcutList,(U16*)gIndexIconsImageList, LIST_MENU, 0, guiBuffer);
		

		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

		register_menu_shortcut_selected(ShctMenuShortcut);
	}
	else
	{
		ShowCategory15Screen(SHORTCUTS_TITLE_STRING_ID,GetRootTitleIcon(MAIN_MENU_SHORTCUTS_MENUID),
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							g_shct_cntx.NumSelShct, ShortcutList,(U16*)gIndexIconsImageList, LIST_MENU, 0, guiBuffer);
		
		SetLeftSoftkeyFunction(ShctExecSelectedShortcut,KEY_EVENT_UP);
		SetKeyHandler(ShctExecSelectedShortcut,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

		register_menu_shortcut_selected(ShctMenuShortcut);
	}

}		


/*****************************************************************************
* FUNCTION
*	HighlightShctSelShortcut
* DESCRIPTION
*   Highlight handler for IDLE screen shortcut.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightShctSelShortcut(S32 index)
{
	g_shct_cntx.CurrSelShct=(U8)index;
	
	(*maxHiliteInfo[g_shct_cntx.SelShctList[g_shct_cntx.CurrSelShct]].entryFuncPtr)();
		
	g_shct_cntx.LSKFunc = get_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY);
	
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(ShctExecSelectedShortcut,KEY_EVENT_UP);
	SetKeyHandler(ShctExecSelectedShortcut,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	ShctExecSelectedShortcut
* DESCRIPTION
*   Execute correspoding function of selected shortcuts.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctExecSelectedShortcut(void)
{
	SetCurrentProfileAsActivatedProfile();
	
	(*g_shct_cntx.LSKFunc)();
}


/*****************************************************************************
* FUNCTION
*	EntryShctInMainMenu
* DESCRIPTION
*   Execute correspoding function of selected shortcuts.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryShctInMainMenu(void)
{
	U8 i;
	U8 *guiBuffer;
	U16 ShortcutList[SHCT_MAX_SEL_LIST];

	EntryNewScreen(SCR_ID_SHCT_MAIN_MENU, NULL, EntryShctInMainMenu, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SHCT_MAIN_MENU);	
	
	for(i=0;i<g_shct_cntx.NumSelShct;i++)
		ShortcutList[i] = ShctGetStringID(g_shct_cntx.SelShctList[i]);
	
	
	if(g_shct_cntx.CurrSelShct>g_shct_cntx.NumSelShct-1) 
		g_shct_cntx.CurrSelShct=0;
	
	ClearAllKeyHandler();
	RegisterHighlightHandler(ShortcutsMenuIndex);
	
	ShowCategory15Screen(MAIN_MENU_SHORTCUTS_TEXT,GetRootTitleIcon(MAIN_MENU_SHORTCUTS_MENUID),
						STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
						g_shct_cntx.NumSelShct, ShortcutList,(U16*)gIndexIconsImageList,LIST_MENU, 0, guiBuffer);
		
	
	SetLeftSoftkeyFunction(EntryShctOptionScreen,KEY_EVENT_UP);
	SetKeyHandler(EntryShctOptionScreen,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	ShortcutsMenuIndex
* DESCRIPTION
*   Execute correspoding function of selected shortcuts.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShortcutsMenuIndex(S32 index)
{
	g_shct_cntx.CurrSelShct=(U8)index;
}


/*****************************************************************************
* FUNCTION
*	EntryShctEditScreen
* DESCRIPTION
*   Execute correspoding function of selected shortcuts.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryShctEditScreen(void)	
{
	U8 i;
	U8 highlighted = 0;
	U16 EditShortcutsIds[SHCT_MAX_CANDI_LIST];
	U8 *guiBuffer;

	if (0 == g_shct_cntx.NumSelShct)
	{
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
		return;
	}
	
	EntryNewScreen(SCR_ID_SHCT_EDIT, NULL, EntryShctEditScreen, NULL);
	ClearAllKeyHandler();

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SHCT_EDIT);

	for(i=0;i<g_shct_cntx.NumCandShct;i++)
	{
		EditShortcutsIds[i]=ShctGetStringID(g_shct_cntx.CandShctList[i]);
		if(guiBuffer == NULL)
			if(g_shct_cntx.CandShctList[i]==g_shct_cntx.SelShctList[g_shct_cntx.CurrSelShct])
   			highlighted=i;
 	}
 
	RegisterHighlightHandler(HighlightShctEdit);
	ShowCategory1Screen(MAIN_MENU_SHORTCUTS_TEXT, GetRootTitleIcon(MAIN_MENU_SHORTCUTS_MENUID),
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
						  g_shct_cntx.NumCandShct, EditShortcutsIds,highlighted, guiBuffer);
	SetLeftSoftkeyFunction(ShctSaveChanges,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	remove_shct_index
* DESCRIPTION
*   Remove index from the shortcut array
* PARAMETERS
*	U16 *shctList, INT16 listLen, INT16 index
* RETURNS
*	The array len. If the result is -1, it means the function implies failed.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
INT16 remove_shct_index(U16 *shctList, INT16 listLen, INT16 index)
{
	U8 i = 0;
	if(index < 0 || index > listLen -1 || listLen < 1 || listLen > SHCT_MAX_SEL_LIST)
		return -1;


	for(i = index; i < listLen-1; ++i)
	{
		shctList[i] = shctList[i+1];
	}

	listLen--;
	return listLen;
}
/*****************************************************************************
* FUNCTION
*	ShctDeleteOneItem
* DESCRIPTION
*    Delete short cut item in to short cut list
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctDeleteOneItem()
{
	INT8 count;
	ClearAllKeyHandler();
	TRACE_EF(g_sw_MP3, " g_shct_cntx.CurrSelShct = %d, g_shct_cntx.NumSelShct = %d", g_shct_cntx.CurrSelShct, g_shct_cntx.NumSelShct);
	count = 0;
 	count = remove_shct_index(g_shct_cntx.SelShctList, g_shct_cntx.NumSelShct ,  g_shct_cntx.CurrSelShct);

	TRACE_EF(g_sw_MP3, "count = %d", count);
	
	if (-1 != count)
		g_shct_cntx.NumSelShct  = count;

	ShctWriteToNvram();
	
	#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE);
	#else
	DisplayPopup((U8*)GetString(SHORTCUTS_HINT_STRING_ID), IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE);
	#endif

	DeleteUptoScrID(SCR_ID_SHCT_MAIN_MENU);
}
/*****************************************************************************
* FUNCTION
*	EntryShctDelScreen
* DESCRIPTION
*   In delete short cut item screen
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryShctDelScreen(void)	
{
	TRACE_EF(g_sw_MP3, "mmi_msg_entry_delete STR_GLOBAL_DELETE = %d", STR_GLOBAL_DELETE);

	if (0 == g_shct_cntx.NumSelShct)
	{
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	}
	else
	{
		mmi_msg_set_confirm_screen(STR_GLOBAL_DELETE, ShctDeleteOneItem, GoBackHistory);
		mmi_msg_entry_confirm_generic();
	}
}
/*****************************************************************************
* FUNCTION
*	HighlightShctAdd
* DESCRIPTION
*    Hilight one shortcut item when add one item
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightShctAdd(S32 index)
{
	g_shct_cntx.CurrAddShct=(U8)index;
}
/*****************************************************************************
* FUNCTION
*	ShctAddOneItem
* DESCRIPTION
*    Add short cut item in to short cut list
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctAddOneItem()
{		
	g_shct_cntx.SelShctList[g_shct_cntx.NumSelShct] = g_shct_cntx.CandShctList[g_shct_cntx.CurrAddShct];
	
	g_shct_cntx.NumSelShct++;

	ShctWriteToNvram();

	#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE);
	#else
	DisplayPopup((U8*)GetString(SHORTCUTS_HINT_STRING_ID), IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE);
	#endif

	DeleteUptoScrID(SCR_ID_SHCT_MAIN_MENU);
}
/*****************************************************************************
* FUNCTION
*	EntryShctAddScreen
* DESCRIPTION
*   In add short cut item screen
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryShctAddScreen()
{
	U8 i;
	U8 highlighted = 0;
	U16 EditShortcutsIds[SHCT_MAX_CANDI_LIST];
	U8 *guiBuffer;

	TRACE_EF(g_sw_MP3, "STR_GLOBAL_DELETE = %d", STR_GLOBAL_ADD);
	if (g_shct_cntx.NumSelShct >= SHCT_MAX_SEL_LIST )
	{
		DisplayPopup ((U8*)GetString (STR_ID_SHORTCUTS_LIST_FULL), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
		return;
	}
	
	EntryNewScreen(SCR_ID_SHCT_EDIT, NULL, EntryShctEditScreen, NULL);
	ClearAllKeyHandler();

	TRACE_EF(g_sw_MP3, "SCR_ID_SHCT_EDIT = %d", SCR_ID_SHCT_EDIT);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SHCT_EDIT);

	for(i=0;i<g_shct_cntx.NumCandShct;i++)
	{
		EditShortcutsIds[i]=ShctGetStringID(g_shct_cntx.CandShctList[i]);
		if(guiBuffer == NULL)
			if(g_shct_cntx.CandShctList[i]==g_shct_cntx.SelShctList[g_shct_cntx.CurrSelShct])
   			highlighted=i;
 	}
 
	RegisterHighlightHandler(HighlightShctAdd);
	ShowCategory1Screen(MAIN_MENU_SHORTCUTS_TEXT, GetRootTitleIcon(MAIN_MENU_SHORTCUTS_MENUID),
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
						  g_shct_cntx.NumCandShct, EditShortcutsIds,highlighted, guiBuffer);
	SetLeftSoftkeyFunction(ShctAddOneItem,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	ShctGetStringID
* DESCRIPTION
*   Return the string ID of a menu item.
* PARAMETERS
*	menu_id		IN 	index of a menu item.
* RETURNS
*	string index
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U16 ShctGetStringID(U16 menu_id)
{
	return nCustMenus[menu_id-1].nStrId;
}


/*****************************************************************************
* FUNCTION
*	HighlightShctEdit
* DESCRIPTION
*   Highlight handler for edit shortcut screen
* PARAMETERS
*	menu_id		IN 	index of a menu item.
* RETURNS
*	string index
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightShctEdit(S32 index)
{
	g_shct_cntx.CurrEditShct=(U8)index;
}


/*****************************************************************************
* FUNCTION
*	ATSetShortcuts
* DESCRIPTION
*   Set shortcutfrom AT command
* PARAMETERS
*	inde		IN 	list of shortcut index
* RETURNS
*	string index
* GLOBALS AFFECTED
*	None
*****************************************************************************/
BOOL ATSetShortcuts(U8 *index)
{
	U8 i;
	U8 ID;
	if(index == NULL)
		return MMI_FALSE;

	for(i=0; i<SHCT_MAX_SEL_LIST; i++)
	{
		ID = index[i]-1;
		if(ID < g_shct_cntx.NumCandShct)
			g_shct_cntx.SelShctList[i]=g_shct_cntx.CandShctList[ID];
	}

	ShctWriteToNvram();		

	return MMI_TRUE;
}


/*****************************************************************************
* FUNCTION
*	ShctSaveChanges
* DESCRIPTION
*   Save change of shorcut
* PARAMETERS
*	index		IN		list of shortcut index
* RETURNS
*	string index
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctSaveChanges(void)
{
	g_shct_cntx.SelShctList[g_shct_cntx.CurrSelShct] = g_shct_cntx.CandShctList[g_shct_cntx.CurrEditShct];
	ShctWriteToNvram();	

#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE);
#else
	DisplayPopup((U8*)GetString(SHORTCUTS_HINT_STRING_ID), IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE);
#endif

	DeleteUptoScrID(SCR_ID_SHCT_MAIN_MENU);
}


#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
/*****************************************************************************
* FUNCTION
*	ShctReadDedicatedListFromNVRAM
* DESCRIPTION
*   Read dedicated key shortcut list and initialized.
* PARAMETERS
*	index		IN		list of shortcut index
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctReadDedicatedListFromNVRAM(void)
{
	U8 i;
	U16 nvramData[SHCT_NVRAM_SIZE];
	S16 error;
	S32 result = 0;
	result=ReadRecord(NVRAM_EF_SHORTCUTS_LID,2,(void *)nvramData, SHCT_NVRAM_SIZE, &error);

	for(i=0; i<TOTAL_DEDICATED_KEYS; i++)
	{
		/*dongwg Modify Start For 6152 Ver: TBM780  on 2007-8-14 14:13 */
#ifdef MMI_ON_WIN32
		if(nvramData[i]==0xffff||NVRAM_READ_SUCCESS != result)
#else
		if(nvramData[i]==0xff)
#endif
		/*dongwg Modify End  For 6152 Ver: TBM780  on 2007-8-14 14:13 */
		{
			memcpy((void*)&nvramData,(void *)&gShctDefaultDediList,TOTAL_DEDICATED_KEYS*2);
			break;
		}			
	}

	for(i=0;i<TOTAL_DEDICATED_KEYS;i++)
	{
		if(maxHiliteInfo[nvramData[i]].entryFuncPtr == NULL)
			g_shct_cntx.DediShctList[i] = SHCT_DEFAULT_MENU_ID;
		else
			memcpy((void*)&g_shct_cntx.DediShctList[i],(void*)&nvramData[i],2);
	}
}


/*****************************************************************************
* FUNCTION
*	ShctWriteDedicatedListToNVRAM
* DESCRIPTION
*   Write dedicated key shortcut list to NVRAM.
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctWriteDedicatedListToNVRAM(void)
{
	U8 nvramData[SHCT_NVRAM_SIZE];
	S16 error;

	memset(nvramData, 0xff, sizeof(nvramData));

	memcpy((void *)&nvramData,(void *)&g_shct_cntx.DediShctList,sizeof(g_shct_cntx.DediShctList));

	WriteRecord(NVRAM_EF_SHORTCUTS_LID,2,(void *)nvramData, SHCT_NVRAM_SIZE, &error);
}


/*****************************************************************************
* FUNCTION
*	EntryShctArrowKeyList
* DESCRIPTION
*   Display Up/Down/Left/Right key for editing.
* PARAMETERS
*	None
* RETURNS
*	string index
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryShctArrowKeyList(void)
{
	U16	ItemList[TOTAL_DEDICATED_KEYS];
	U16	nItems;
	U8*	guiBuffer;
	U8*	popupList[TOTAL_DEDICATED_KEYS];
	S32 	i ;
	
#ifdef __MMI_MULTI_SIM__
	mmi_frm_unhide_menu_item(MENU_SETTING_DEF_ENCODING_UCS2);
	if(MTPNP_AD_Get_UsableSide_Number() == 0)
	{
		mmi_frm_hide_menu_item(MENU_SETTING_DEF_ENCODING_UCS2);
	}
#endif


	EntryNewScreen(SCR_ID_SHCT_DEDICATED_KEYS, NULL, EntryShctArrowKeyList, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SHCT_DEDICATED_KEYS);
	nItems = GetNumOfChild_Ext(MENU_SETTING_DEDICATED_KEYS);
	GetSequenceStringIds_Ext(MENU_SETTING_DEDICATED_KEYS,ItemList);

	for(i=0;i<TOTAL_DEDICATED_KEYS;i++)
	{
		if(g_shct_cntx.DediShctList[i] == SHCT_DEFAULT_MENU_ID)
			popupList[i] = (U8*)GetString(MAIN_MENU_SHORTCUTS_TEXT);
		else
			popupList[i] = (U8*)GetString(ShctGetStringID(g_shct_cntx.DediShctList[i]));
	}
	
	SetParentHandler(MENU_SETTING_DEDICATED_KEYS);
	RegisterHighlightHandler(HighlightShctSelectedKey);

	ShowCategory52Screen(	STR_DEDICATED_KEY,GetRootTitleIcon(MENU_SETTING_DEDICATED_KEYS),
							STR_EDIT,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, (U16*)gIndexIconsImageList, popupList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryShctEditDedicatedKey,KEY_EVENT_UP);
	SetKeyHandler(EntryShctEditDedicatedKey, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	HighlightDedicatedKeys
* DESCRIPTION
*   Highlight handler of dedicated key shortcut item in Phonesetup
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightDedicatedKeys(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(EntryShctArrowKeyList,KEY_EVENT_UP);
	SetKeyHandler(EntryShctArrowKeyList, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightShctSelectedKey
* DESCRIPTION
*   Highlight handler of dedicated keys setting
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightShctSelectedKey(S32 nIndex)
{
	g_shct_cntx.CurrSelArrow = (U8)nIndex;
}


/*****************************************************************************
* FUNCTION
*	EntryShctEditDedicatedKey
* DESCRIPTION
*   Entry edit dedicated key shortcut screen.
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryShctEditDedicatedKey(void)
{
	U8 i;
	U8 highlighted = 0;
	U16 EditShortcutsIds[SHCT_MAX_CANDI_LIST+1];
	U8 *guiBuffer;

	EntryNewScreen(SCR_ID_SHCT_EDIT_DEDICATED_KEYS, NULL, EntryShctEditDedicatedKey, NULL);
	ClearAllKeyHandler();

	/* the first item == Shortcuts */
	EditShortcutsIds[0] = MAIN_MENU_SHORTCUTS_TEXT;

	guiBuffer = GetCurrGuiBuffer(SCR_ID_SHCT_EDIT_DEDICATED_KEYS);

	for(i=0;i<g_shct_cntx.NumCandShct;i++)
	{
		EditShortcutsIds[i+1]=ShctGetStringID(g_shct_cntx.CandShctList[i]);
		if(guiBuffer == NULL)
			if(g_shct_cntx.CandShctList[i]==g_shct_cntx.DediShctList[g_shct_cntx.CurrSelArrow])
				highlighted=i+1;
	}

	RegisterHighlightHandler(HighlightShctEdit);

	ShowCategory1Screen(MAIN_MENU_SHORTCUTS_TEXT, GetRootTitleIcon(MENU_SETTING_DEDICATED_KEYS),
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
						 g_shct_cntx.NumCandShct+1, EditShortcutsIds,highlighted, guiBuffer);
	SetLeftSoftkeyFunction(ShctSaveDedicated,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	ShctSaveDedicated
* DESCRIPTION
*   Save dedicated key shortcut.
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctSaveDedicated(void)
{
	if(g_shct_cntx.CurrEditShct > 0)
		g_shct_cntx.DediShctList[g_shct_cntx.CurrSelArrow] = g_shct_cntx.CandShctList[g_shct_cntx.CurrEditShct-1];
	else
		g_shct_cntx.DediShctList[g_shct_cntx.CurrSelArrow] = SHCT_DEFAULT_MENU_ID;

	ShctWriteDedicatedListToNVRAM();


	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE);
	DeleteUptoScrID(SCR_ID_SHCT_DEDICATED_KEYS);
}


/*****************************************************************************
* FUNCTION
*	ShctExecuteUpKey
* DESCRIPTION
*   Up key handler in IDLE screen.
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctExecuteUpKey(void)
{
if(1 == g_shct_cntx.IsUpKeyPressed)
{
	if(g_shct_cntx.DediShctList[0] != SHCT_DEFAULT_MENU_ID)
	{
		g_shct_cntx.IsKeyRetrieving = TRUE;
		(*maxHiliteInfo[g_shct_cntx.DediShctList[0]].entryFuncPtr)();	
		g_shct_cntx.IsKeyRetrieving = FALSE;

		execute_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY);
	}
	else
	{
		EntryShctInIdle();
	}
	g_shct_cntx.IsUpKeyPressed = 0;
}
}

/*****************************************************************************
* FUNCTION
*	ShctExecuteDownKey
* DESCRIPTION
*   Down key handler in IDLE screen.
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctExecuteDownKey(void)
{
if(1 == g_shct_cntx.IsDownKeyPressed)
{
	if(g_shct_cntx.DediShctList[1] != SHCT_DEFAULT_MENU_ID)
	{
		g_shct_cntx.IsKeyRetrieving = TRUE;
		(*maxHiliteInfo[g_shct_cntx.DediShctList[1]].entryFuncPtr)();	
		g_shct_cntx.IsKeyRetrieving = FALSE;
		execute_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY);
	}
	else
	{
		EntryShctInIdle();
	}
	g_shct_cntx.IsDownKeyPressed = 0;
}
}

/*****************************************************************************
* FUNCTION
*	ShctExecuteLeftKey
* DESCRIPTION
*   Left key handler in IDLE screen.
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctExecuteLeftKey(void)
{
if(1 == g_shct_cntx.IsLeftKeyPressed)
{
	if(g_shct_cntx.DediShctList[2] != SHCT_DEFAULT_MENU_ID)
	{
		g_shct_cntx.IsKeyRetrieving = TRUE;
		(*maxHiliteInfo[g_shct_cntx.DediShctList[2]].entryFuncPtr)();
		g_shct_cntx.IsKeyRetrieving = FALSE;
		execute_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY);
	}
	else
	{
		EntryShctInIdle();
	}
	g_shct_cntx.IsLeftKeyPressed = 0;
}
}

/*****************************************************************************
* FUNCTION
*	ShctExecuteRightKey
* DESCRIPTION
*   Right key handler in IDLE screen.
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctExecuteRightKey(void)
{
if(1 == g_shct_cntx.IsRightKeyPressed)
{
	if(g_shct_cntx.DediShctList[3] != SHCT_DEFAULT_MENU_ID)
	{
		g_shct_cntx.IsKeyRetrieving = TRUE;
		(*maxHiliteInfo[g_shct_cntx.DediShctList[3]].entryFuncPtr)();	
		g_shct_cntx.IsKeyRetrieving = FALSE;
		execute_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY);
	}
	else
	{
		EntryShctInIdle();
	}
	g_shct_cntx.IsRightKeyPressed = 0;
}
}

/*****************************************************************************
* FUNCTION
*	ShctExecuteRightKey
* DESCRIPTION
*   Right key handler in IDLE screen.
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctExecuteOkKey(void)
{
if(1 == g_shct_cntx.IsOkKeyPressed)
{
#if defined(__PROJECT_GALLITE_C01__)
	if (MTPNP_AD_Get_UsableSide_Number() != 0)
	{
		EntryMainMenuFromIdleScreen();
	}
	else
	{
		EntryScrSimOptionMenu();
	}
#else	
	if(g_shct_cntx.DediShctList[4] != SHCT_DEFAULT_MENU_ID)
	{
		g_shct_cntx.IsKeyRetrieving = TRUE;
		(*maxHiliteInfo[g_shct_cntx.DediShctList[4]].entryFuncPtr)();	
		g_shct_cntx.IsKeyRetrieving = FALSE;
		execute_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY);
	}
	else
	{
		EntryShctInIdle();
	}
#endif
	g_shct_cntx.IsOkKeyPressed = 0;
}
}

/*****************************************************************************
* FUNCTION
*	RegisterDedicatedKeyHandlers
* DESCRIPTION
*   Register arrow key handlers in IDLE screen.
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ShctPressUpKey(void)
{
	g_shct_cntx.IsUpKeyPressed = 1;
}
void ShctPressDownKey(void)
{
	g_shct_cntx.IsDownKeyPressed = 1;
}
void ShctPressLeftKey(void)
{
	g_shct_cntx.IsLeftKeyPressed = 1;
}
void ShctPressRightKey(void)
{
	g_shct_cntx.IsRightKeyPressed = 1;
}
void ShctPressOkKey(void)
{
	g_shct_cntx.IsOkKeyPressed = 1;
}
void RegisterDedicatedKeyHandlers(void)
{
	SetCurrentProfileAsActivatedProfile();
	SetKeyHandler(ShctPressUpKey, KEY_UP_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(ShctPressDownKey, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(ShctPressLeftKey, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(ShctPressRightKey, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
    #if !defined(__MMI_PROJECT_T33__)
#ifndef __MMI_USE_LSK_AS_SEND2__
	SetKeyHandler(ShctPressOkKey, KEY_ENTER, KEY_EVENT_DOWN);
#endif
	#endif
	SetKeyHandler(ShctExecuteUpKey, KEY_UP_ARROW, KEY_EVENT_UP);
	SetKeyHandler(ShctExecuteDownKey, KEY_DOWN_ARROW, KEY_EVENT_UP);
	SetKeyHandler(ShctExecuteLeftKey, KEY_LEFT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(ShctExecuteRightKey, KEY_RIGHT_ARROW, KEY_EVENT_UP);	
    #if !defined(__MMI_PROJECT_T33__)
#ifndef __MMI_USE_LSK_AS_SEND2__
	SetKeyHandler(ShctExecuteOkKey, KEY_ENTER, KEY_EVENT_UP);	
#else
	SetKeyHandler(EntryMainMenuFromIdleScreen,KEY_ENTER,KEY_EVENT_UP);
#endif
    #endif
}

#endif

/*****************************************************************************
* FUNCTION
*	HighlightShctOptionEdit
* DESCRIPTION
*     Highlight option edit
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightShctOptionEdit()
{
	TRACE_SYS_FUNCTION();
	
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntryShctEditScreen,KEY_EVENT_UP);
	SetKeyHandler(EntryShctEditScreen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/*****************************************************************************
* FUNCTION
*	HighlightShctOptionDelete
* DESCRIPTION
*     Highlight option delete
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightShctOptionDelete()
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntryShctDelScreen,KEY_EVENT_UP);
	SetKeyHandler(EntryShctDelScreen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/*****************************************************************************
* FUNCTION
*	HighlightShctOptionAdd
* DESCRIPTION
*     Highlight option add
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightShctOptionAdd()
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntryShctAddScreen,KEY_EVENT_UP);
	SetKeyHandler(EntryShctAddScreen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/*****************************************************************************
* FUNCTION
*	EntryShctOptionScreen
* DESCRIPTION
*      Entry shuct option screen
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryShctOptionScreen()
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem = 0;      /* Stores no of children in the submenu*/
	U16 nDispAttribute = 0;  /* Stores display attribue */
	U8* guiBuffer = NULL;       /* Buffer holding history data */
	U8* PopUpList[MAX_SUB_MENUS];
	S16 i=0;
	U16 ParentId = 0;

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_ID_SHCT_OPTION, NULL, EntryShctOptionScreen, NULL);



	ParentId = EXTRA_SHORTCUTS_OPTION_MENUID;

	/* 1 Set parent handler */
	SetParentHandler(ParentId);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SHCT_OPTION);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(ParentId);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(ParentId);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(ParentId,nStrItemList);

	for(i=0;i<nNumofItem;i++)
	{
		PopUpList[i] = NULL;
	}

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);

	/* 8 Display Category Screen */

	ShowCategory52Screen(STR_PROFILE_OPTION_CAPTION,MAIN_MENU_TITLE_SHORTCUTS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,
						  nStrItemList,
						  (PU16)gIndexIconsImageList,
						  PopUpList ,
						  0,0,
						  guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}
#endif /*SHORTCUTS_APP*/
#endif //_MMI_SHORTCUTS_C


