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
 * SpeedDial.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Speed dial application
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

	FILENAME	: SpeedDial.c

  	PURPOSE		: Speed Dial Application 

 

	AUTHOR		: Hariprasad, Vanita Jain

	DATE		: Dec 5,02

**************************************************************/

#ifndef _SPEEDDIAL_C
#define _SPEEDDIAL_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "phonebookdef.h"
#include "phonebooktypes.h"
#include "phonebookgprot.h"
#include "phonebookprot.h"
#include "wgui_categories.h"
#include "mainmenudef.h"
#include "callmanagementgprot.h"
#include "unicodexdcl.h"
#include "globaldefs.h"
#include "settingdefs.h"
#include "speeddial.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "nvramtype.h"
#include "globalmenuitems.h"
#include "timerevents.h"
#include "commonscreens.h"

//micha0420
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "simdetectiongexdcl.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
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
SpeedDialInfo speedDialTable[MAX_SPEED_DIAL_NUMBER];
static pBOOL CurrentSpeedTypeChange = FALSE;
U8 speedKeyIndex = 0, CurrentSpeedType=0;
S16 currentKeyIndex = 0;
pBOOL readNVRAM = FALSE;


/* 
** Global Variable
*/
extern MMI_PHB_ENTRY_BCD_STRUCT			PhoneBook[];				
extern U16 g_phb_name_index[];
extern U16 PhoneBookEntryCount;

/* 
** Global Function
*/
extern void	mmi_phb_init_build_lookup_table(void);


/*****************************************************************************
* FUNCTION
*  HintPhbSpeedDial
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> SpeedDial
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhbSpeedDial(U16 index)
{
	if(PhbGetSpeedDialStatus())
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
	}
}



/*****************************************************************************
* FUNCTION
*  HighlightPhbSpeedDial
* DESCRIPTION
*   This function is highlight handler for Phonesetup-> SpeedDial
*   Follow functions: EntryPhbSpeedDial
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhbSpeedDial(void)
{
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	ChangeLeftSoftkey(STR_GLOBAL_OK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryPhbSpeedDial,KEY_EVENT_UP);
	SetKeyHandler(EntryPhbSpeedDial, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*  EntryPhbSpeedDial
* DESCRIPTION
*   This function is Entry function for Phonesetup-> SpeedDial
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*  
*****************************************************************************/
void EntryPhbSpeedDial(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */

	U8 *SpeedDialType[MAX_SUB_MENUS];
	
	/*Call Exit Handler*/
	EntryNewScreen(SCR_SETTING_SPEED_DIAL, ExitPhbSpeedDial, EntryPhbSpeedDial, NULL);

	/*Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_SETTING_SPEED_DIAL);	

    /*Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_SETTING_SPEED_DIAL);

	/*Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU9102_INITIAL_SETUP);

	/*Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_SETTING_SPEED_DIAL,nStrItemList);

	/*Set current parent id*/
	SetParentHandler(MENU_SETTING_SPEED_DIAL);

	/*Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/*Construct the Hints List */
	ConstructHintsList(MENU_SETTING_SPEED_DIAL, SpeedDialType);
	
	/*Display Category1 Screen */
	ShowCategory52Screen(  STR_MENU_SETTING_SPEED_DIAL,GetRootTitleIcon(MENU_SETTING_SPEED_DIAL)/*MAIN_MENU_TITLE_SETTINGS_ICON*/,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						    nNumofItem, nStrItemList,(PU16)gIndexIconsImageList, SpeedDialType,
							0,0, guiBuffer);
	/*Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*  ExitPhbSpeedDial
* DESCRIPTION
*   This function is Exit function for Phonesetup-> SpeedDial
*   Functionality:  Save speed dial status
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitPhbSpeedDial(void)
{
   S16 error;
   
	/* Save To NVRAM if Value is Changed*/
	if( CurrentSpeedTypeChange == TRUE )
	{
		WriteValue( NVRAM_SETTING_SPEED_DIAL, &CurrentSpeedType, DS_BYTE , &error);
		CurrentSpeedTypeChange = FALSE;
	}
}


/*****************************************************************************
* FUNCTION
*  HighlightPhbSpeedDialStatus
* DESCRIPTION
*   This function is highlight handler for Phonesetup-> SpeedDial -> Status
*   Follow functions: EntryPhbSpeedDialStatus
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhbSpeedDialStatus(void)
{
	U16 index;
	index = GetCurrHiliteID();

	/* If Speed Dial is On Change The LSK to OFF*/	
	if( CurrentSpeedType == 1 )
		{
			ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
		}
	/*Else Chnage it to ON*/
		else
		{
			ChangeLeftSoftkey(STR_GLOBAL_ON,0);
		}

	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);

	SetLeftSoftkeyFunction(EntryPhbSpeedDialStatus,KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*  EntryPhbSpeedDialStatus
* DESCRIPTION
*   This function is Entry function for Phonesetup-> SpeedDial -> Enable Speed Dial
*   Functionality: toggle on/off
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*  
*****************************************************************************/
void EntryPhbSpeedDialStatus(void)
{
	U16 index;
	index = GetCurrHiliteID();
	
	/*If Speed Dial is On Then Turn It Off*/
	if( CurrentSpeedType == 1 )
	{
		CurrentSpeedType = 0;
	}
	/*Else Turn it On*/
	else
	{
		CurrentSpeedType = 1;
	}

	/* Store the updated Value */
	CurrentSpeedTypeChange = TRUE;

	if(CurrentSpeedType)
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}

	/*Redraw the Category Screen with Modified Values*/
	RedrawCategoryFunction();
	/*Highlite Speed Dial Status Option*/
	HighlightPhbSpeedDialStatus();
}



/*****************************************************************************
* FUNCTION
*  PhbInitSpeedDial
* DESCRIPTION
*   This function is to init highlight handler for speed dial
*   Functionality:HighlightPhbSpeedDialSetNum
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbInitSpeedDial(void) 
{
     SetHiliteHandler(MENU_SETTING_SPEED_DIAL,  HighlightPhbSpeedDial); 
	SetHiliteHandler(MITEM113_PBOOK_SPEED_DIAL,HighlightPhbSpeedDialSetNum);
	SetHiliteHandler(MENU_SETTING_SPEED_STATUS,HighlightPhbSpeedDialStatus);

	SetHintHandler(MENU_SETTING_SPEED_DIAL,HintPhbSpeedDial);
	SetHintHandler(MENU_SETTING_SPEED_STATUS,HintPhbSpeedDial); //SetHintHandler(MENU_SETTING_SPEED_STATUS,HintSpeedDialOn);

	SetHiliteHandler(MENU_ID_SPEED_DIAL_OP_EDIT,  HighlightPhbSpeedDialSetNumOptionEdit); 
	SetHiliteHandler(MENU_ID_SPEED_DIAL_OP_DEL,  HighlightPhbSpeedDialSetNumOptionDel); 	
}



/*****************************************************************************
* FUNCTION
*  HighlightPhbSpeedDialSetNum
* DESCRIPTION
*   This function is Highlight handler for Phonebook-> Speeddial
*   Follow functions: EntryPhbSpeedDial
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhbSpeedDialSetNum() {
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(EntryPhbSpeedDialSetNum,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(EntryPhbSpeedDialSetNum, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  EntryPhbSpeedDialSetNum
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhbSpeedDialSetNum(void) {
	U8 *guiBuffer, i;
	U8 *speedKeyName[MAX_SPEED_DIAL_NUMBER];
	//U8 *speedKeyNumber[MAX_SPEED_DIAL_NUMBER];

	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing) 
	{
		EntryNewScreen(SCR_SPEED_DIAL_LIST, wgui_restore_list_menu_slim_style, EntryPhbSpeedDialSetNum, NULL);
		
		for(i = 0; i < MAX_SPEED_DIAL_NUMBER; ++i) 
		{
			if(pfnUnicodeStrlen((PS8)speedDialTable[i].name) || pfnUnicodeStrlen((PS8)speedDialTable[i].number)) 
			 {
				if(pfnUnicodeStrlen((PS8)speedDialTable[i].name))  /* An entry with name */
				{
					speedKeyName[i] = speedDialTable[i].name;
					if(pfnUnicodeStrlen((PS8)speedDialTable[i].number))
						subMenuDataPtrs[i] = speedDialTable[i].number;
					else
						subMenuDataPtrs[i] = NULL;
				}
				else /* An entry with number only */
				{
					speedKeyName[i] = speedDialTable[i].number;
					subMenuDataPtrs[i] = NULL;
				}
			}
			else /* Entry without name and number */
			{
				speedKeyName[i] = (PU8)GetString(STR_GLOBAL_EMPTY_LIST);
				subMenuDataPtrs[i] = NULL;
			}
		}

		wgui_override_list_menu_slim_style(WGUI_LIST_MENU_SLIM_STYLE_DRAW_ICON);

		guiBuffer = GetCurrGuiBuffer (SCR_SPEED_DIAL_LIST);
		RegisterHighlightHandler(PhbSpeedDailEntryHiliteHandler);

		ShowCategory53Screen(STR_SCR_SPEED_DIAL_LIST_CAPTION,GetRootTitleIcon(MENU_SETTING_SPEED_DIAL)/*MAIN_MENU_TITLE_SETTINGS_ICON*/,
				STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
				STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				MAX_SPEED_DIAL_NUMBER, speedKeyName, (U16*)(gIndexIconsImageList+1), 
				subMenuDataPtrs, 0, 0, guiBuffer);




		SetLeftSoftkeyFunction(EntryPhbSpeedDialSetNumOption, KEY_EVENT_UP);
		SetKeyHandler(EntryPhbSpeedDialSetNumOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

#ifdef __MMI_MULTI_SIM__
	    MTPNP_PFAL_CC_HandleSendKeys(PhbMakeCallFromSpeedDial,KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
		ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
		SetKeyHandler(EntryPhbSpeedDialSetNumOption,KEY_ENTER,KEY_EVENT_UP );
#endif

#else
	    SetKeyHandler(PhbMakeCallFromSpeedDial, KEY_SEND, KEY_EVENT_DOWN);
#endif

	}
	else
		mmi_phb_entry_not_ready();
}
/*****************************************************************************
* FUNCTION
*  mmi_frm_speed_dial_number_done
* DESCRIPTION
*   This function is handler of speed dial number input done
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 number_temp[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
void mmi_frm_speed_dial_number_done(void)
{
	S16 error;
	
	speedDialTable[currentKeyIndex].storage		= MMI_NVRAM;
	speedDialTable[currentKeyIndex].type			= MMI_PHB_NONE;
	speedDialTable[currentKeyIndex].record_index	= 0;
	//pfnUnicodeStrcpy((PS8)speedDialTable[currentKeyIndex].name, (PS8)PhoneBook[store_index].alpha_id.name);
	//mmi_phb_convert_get_ucs2_number((S8*)speedDialTable[currentKeyIndex].number, store_index);
	memcpy(speedDialTable[currentKeyIndex].number, number_temp,sizeof(speedDialTable[currentKeyIndex].number));
	WriteRecord(NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID, 1, speedDialTable, NVRAM_SPEEDDIAL_RECORD_SIZE, &error);

	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	DeleteUptoScrID(SCR_SPEED_DIAL_LIST);
}
/*****************************************************************************
* FUNCTION
*  mmi_frm_speed_dail_number_empty
* DESCRIPTION
*   This function is handler of speed dial number empty
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void mmi_frm_speed_dail_number_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_SEARCH, 0);
	SetLeftSoftkeyFunction (mmi_phb_speed_dial_enter_phb, KEY_EVENT_UP);
}
/*****************************************************************************
* FUNCTION
*  mmi_frm_speed_dail_number_not_empty
* DESCRIPTION
*   This function is handler of speed dial number not empty
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void mmi_frm_speed_dail_number_not_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, 0);
	SetLeftSoftkeyFunction (mmi_frm_speed_dial_number_done, KEY_EVENT_UP);
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_speed_dial_enter_num
* DESCRIPTION
*   This function is enter of speed dial number input
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void mmi_phb_speed_dial_enter_num(void )
{
	U8* 	guiBuffer;
	EntryNewScreen(SCR_SETTING_SPEED_DIAL_ENTER_NUM, NULL, mmi_phb_speed_dial_enter_num, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_SETTING_SPEED_DIAL_ENTER_NUM);

	RegisterInputBoxEmptyFunction (mmi_frm_speed_dail_number_empty);
	RegisterInputBoxNotEmptyFunction (mmi_frm_speed_dail_number_not_empty);

	ShowCategory5Screen (STR_ID_PHB_NUM_REQUIRE, 0,//
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0, 
						  INPUT_TYPE_PHONE_NUMBER,
						  (U8*)number_temp, MAX_PB_NUMBER_LENGTH + 1, guiBuffer); 
	
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	
}
/*CSD 20041011 Wilson, Add operation menu for speed dial*/
/*****************************************************************************
* FUNCTION
*  HighlightPhbSpeedDialSetNumOptionEdit
* DESCRIPTION
*   This function is Highlight handler for Speeddial->Edit
*   Follow functions: EntryPhbSpeedDial
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhbSpeedDialSetNumOptionEdit(void)
{
	#if 1
	SetLeftSoftkeyFunction(mmi_phb_speed_dial_enter_phb,KEY_EVENT_UP);
	#else
	SetLeftSoftkeyFunction(mmi_phb_speed_dial_enter_num,KEY_EVENT_UP);
	#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_speed_dial_enter_phb, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightPhbSpeedDialSetNumOptionDel
* DESCRIPTION
*   This function is Highlight handler for Speeddial->Delete
*   Follow functions: EntryPhbSpeedDial
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhbSpeedDialSetNumOptionDel(void)
{
	SetLeftSoftkeyFunction(EntryPhbSpeedDialSetNumDelConfirm,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  EntryPhbSpeedDialSetNumOption
* DESCRIPTION
*   This function is Entry function for Speed Dial Operation
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhbSpeedDialSetNumOption(void)
{
	U8* guiBuffer;
	U16 nStrItemList[3];
	U16 nNumofItem;
	
	EntryNewScreen(SCR_ID_SPEED_DIAL_SET_NUM_OPTION, NULL, EntryPhbSpeedDialSetNumOption, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SPEED_DIAL_SET_NUM_OPTION);

	nNumofItem = GetNumOfChild(MENU_ID_SPEED_DIAL_OPERATION);
	GetSequenceStringIds(MENU_ID_SPEED_DIAL_OPERATION, nStrItemList);
	SetParentHandler(MENU_ID_SPEED_DIAL_OPERATION);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/*Record Empty - Remove Delete Operation*/
	if(!pfnUnicodeStrlen((S8*)speedDialTable[currentKeyIndex].name) && !pfnUnicodeStrlen((S8*)speedDialTable[currentKeyIndex].number)) 
		nNumofItem--;

	ShowCategory15Screen(STR_SCR_SPEED_DIAL_LIST_CAPTION,GetRootTitleIcon(MENU_SETTING_SPEED_DIAL),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);
}

/*****************************************************************************
* FUNCTION
*  EntryPhbSpeedDialSetNumDelConfirm
* DESCRIPTION
*   Delete confirmation screen.
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhbSpeedDialSetNumDelConfirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_GLOBAL_DELETE), IMG_GLOBAL_QUESTION, WARNING_TONE); 


	SetLeftSoftkeyFunction(PhbDelSpeedDialFromList,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}
/*END CSD 20041011 Wilson*/


/*****************************************************************************
* FUNCTION
*  PhbGetSpeedDialInfo
* DESCRIPTION
*   Start a timer to read speeddial from NVRAM after enter idle screen.
*   (Avoid timeout callback to read NVRAM before enter idle screen.)
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbGetSpeedDialInfo(void)
{
	StopTimer(PHB_READ_SPEED_DIAL_TIMER);
	if(!HasValidSIM())
	{
		g_phb_cntx.phb_ready = TRUE;
		mmi_phb_init_build_lookup_table();
		return;
	}
	
	if(IS_IDLE_ARRIVED)
	{
		/*Start a timer to avoid extensive NVRAM access which may cause MMI hang shortly*/
		StartTimer(PHB_READ_SPEED_DIAL_TIMER, 500,PhbGetSpeedDialInfo_Ext);	
	}
	else
	{	
		/*Retry again until enter idle screen.*/
		StartTimer(PHB_READ_SPEED_DIAL_TIMER, 500,PhbGetSpeedDialInfo);	
	}
}	

/*****************************************************************************
* FUNCTION
*  PhbGetSpeedDialInfo_Ext
* DESCRIPTION
 
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbGetSpeedDialInfo_Ext(void) 
{
	//S16 error;
	PhbReadSpeedDialNVRAM();
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
#else
 	for (speedKeyIndex = 0; speedKeyIndex< MAX_SPEED_DIAL_NUMBER; speedKeyIndex++)
 	{
 		if(speedDialTable[speedKeyIndex].record_index != 0) 
 		{
			U16 store_index;
			if (speedDialTable[speedKeyIndex].storage == MMI_SIM)
				store_index = speedDialTable[speedKeyIndex].record_index + MAX_PB_PHONE_ENTRIES -1;
			else
				store_index = speedDialTable[speedKeyIndex].record_index - 1;
			
			if(store_index==(0xffff-1))//if 0xffff,not init,we don't minus 1
			{
				store_index = 0xffff;
			}
			PhbSetSpeedDialPhbIndex(store_index);
  		}
 	}
   g_phb_cntx.phb_ready = TRUE;
   mmi_phb_init_build_lookup_table();
#endif
}

/**************************************************************

	FUNCTION NAME		: PhbGetSpeedDialPhbIndexReq()

 
						  storage & type

	INPUT PARAMETERS	: U8 storage, U16 rIndex, U8 type

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void PhbGetSpeedDialPhbIndexReq(U8 storage, U16 rIndex, U8 type) 
{
#if defined(MMI_ON_WIN32)
	PhbSetSpeedDialPhbIndex(0xffff);
#else

	/*If FDN in On, Don't Query Sim Entry*/
	if(g_SecuritySetupContext.FdlStatus[0] && (storage == MMI_SIM))
			PhbSetSpeedDialPhbIndex(0xffff);
	else 
	{

		MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *myMsgPtr;
		MYQUEUE	Message;
		PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<PhbGetSpeedDialPhbIndexReq.>\n",__FILE__,__LINE__));

		myMsgPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT));
		myMsgPtr->index			= 0xFFFF;
		myMsgPtr->record_index	= rIndex;
		myMsgPtr->storage		= storage;
		myMsgPtr->type			= MMI_PHB_PHONEBOOK;
		myMsgPtr->no_data		= 1;
	
		Message.oslSrcId		= MOD_MMI;
		Message.oslDestId		= MOD_L4C;
		Message.oslMsgId		= PRT_PHB_GET_ENTRY_BY_INDEX_REQ;
		Message.oslDataPtr		= (oslParaType *)myMsgPtr;
		Message.oslPeerBuffPtr	= NULL;

		SetProtocolEventHandler(PhbGetSpeedDialPhbIndexRsp	,PRT_PHB_GET_ENTRY_BY_INDEX_RSP);
		OslMsgSendExtQueue(&Message);

	}
#endif
}

/**************************************************************

	FUNCTION NAME		: PhbGetSpeedDialPhbIndexRsp()

  	PURPOSE				: Response handler for PhbGetSpeedDialPhbIndexReq()

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void PhbGetSpeedDialPhbIndexRsp(void* info) 
{
	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U16 store_index;
	
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<PhbGetSpeedDialPhbIndexRsp.>\n",__FILE__,__LINE__));
	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT*)info;
//qiff modify for bug:10650,the RSP handler should be set in REQ function
	SetProtocolEventHandler(NULL, PRT_PHB_GET_ENTRY_BY_INDEX_RSP);

	if (localPtr->result.flag == 0 /*OK*/) 
	{
		if(localPtr->no_list)
		{
			if(localPtr->list[0].storage >= MMI_SIM && localPtr->list[0].storage < MMI_SIM4)
			{
				store_index = localPtr->list[0].record_index + MAX_PB_PHONE_ENTRIES -1 + MAX_PB_SIM_ENTRIES*(localPtr->list[0].storage - MMI_SIM);
			}
			else
				store_index = localPtr->list[0].record_index - 1;
			PhbSetSpeedDialPhbIndex(store_index);
		}
		else
			PhbSetSpeedDialPhbIndex(0xffff);
	}
	else
		PhbSetSpeedDialPhbIndex(0xffff);
}

/*****************************************************************************
* FUNCTION
*  PhbSetSpeedDialPhbIndex
* DESCRIPTION
 
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbSetSpeedDialPhbIndex(U16 store_index) {
	//S16 error;

	if(store_index != 0xffff) 
	{
		if (pfnUnicodeStrlen(PhoneBook[store_index].alpha_id.name) || pfnUnicodeStrlen(PhoneBook[store_index].tel.number))
		{
			pfnUnicodeStrcpy((PS8)speedDialTable[speedKeyIndex].name, (PS8)PhoneBook[store_index].alpha_id.name);
			mmi_phb_convert_get_ucs2_number((S8*)speedDialTable[speedKeyIndex].number, store_index);
		}
		else
		{
			memset(&speedDialTable[speedKeyIndex], 0, sizeof(SpeedDialInfo));	
		}
	}
//micha0420
	else if(!(speedDialTable[speedKeyIndex].storage >= MMI_SIM && speedDialTable[speedKeyIndex].storage <= MMI_SIM4 && g_SecuritySetupContext.FdlStatus[speedDialTable[speedKeyIndex].storage - MMI_SIM]))
	{
		memset(speedDialTable[speedKeyIndex].name, 0, ENCODING_LENGTH);
		memset(speedDialTable[speedKeyIndex].number, 0, ENCODING_LENGTH);
	}
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
#endif
}



/*****************************************************************************
* FUNCTION
*  PhbReadSpeedDialNVRAM
* DESCRIPTION
*   This function is to read speed dial record from nvram
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbReadSpeedDialNVRAM(void)
{
	S16 error ;
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
	ReadRecord(NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID, 1, speedDialTable, NVRAM_SPEEDDIAL_RECORD_SIZE, &error);
#endif
}


/*****************************************************************************
* FUNCTION
*  PhbSetSpeedDialFormList
* DESCRIPTION
*   This function is to set speed dial entry form phone book list
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbSetSpeedDialFormList(void) 
{
	S16 error ;
	U8 tempNumber[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
	U16 store_index;
	U8 storage;
	U16 record_index;

	store_index = g_phb_name_index[g_phb_cntx.active_index_second];

	if(store_index < MAX_PB_PHONE_ENTRIES)
	{
		storage = MMI_NVRAM;
		record_index = store_index + 1;
	}
	else
	{
		storage = MMI_SIM;
		record_index = store_index - MAX_PB_PHONE_ENTRIES+ 1;
	}

	mmi_phb_convert_get_ucs2_number((S8*)tempNumber, store_index);

	if(pfnUnicodeStrlen((PS8)tempNumber) == 0)
	{
		DisplayPopup((PU8)GetString(STR_SPEEDDIAL_NO_NUM_TODIAL_TEXT) ,IMG_GLOBAL_ERROR, 1,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(SCR_SPEED_DIAL_LIST);		

		return;
	}
	
      /* Do not update if record is same */
	if(	(speedDialTable[currentKeyIndex].storage == storage) && \
		(speedDialTable[currentKeyIndex].type == MMI_PHB_PHONEBOOK) && \
		(speedDialTable[currentKeyIndex].record_index == record_index) && \
		(pfnUnicodeStrcmp((PS8)speedDialTable[currentKeyIndex].name, (PS8)PhoneBook[store_index].alpha_id.name) == 0 )
	)
	{	
		if(pfnUnicodeStrcmp((PS8)speedDialTable[currentKeyIndex].number, (PS8)tempNumber) == 0)
		{
			GoBackHistory();
			return;
		}
	}

	speedDialTable[currentKeyIndex].storage		= storage;
	speedDialTable[currentKeyIndex].type			= MMI_PHB_PHONEBOOK;
	speedDialTable[currentKeyIndex].record_index	= record_index;
	pfnUnicodeStrcpy((PS8)speedDialTable[currentKeyIndex].name, (PS8)PhoneBook[store_index].alpha_id.name);
	mmi_phb_convert_get_ucs2_number((S8*)speedDialTable[currentKeyIndex].number, store_index);

	WriteRecord(NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID, 1, speedDialTable, NVRAM_SPEEDDIAL_RECORD_SIZE, &error);

	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	DeleteUptoScrID(SCR_SPEED_DIAL_LIST);

}
 

/*CSD 20041011 Wilson, Add operation menu for speed dial*/
/*****************************************************************************
* FUNCTION
*  PhbDelSpeedDialFromList
* DESCRIPTION
*   This function is to remove speed dial entry
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbDelSpeedDialFromList(void)
{
	S16 error ;
	memset(&speedDialTable[currentKeyIndex], 0, sizeof(SpeedDialInfo));

	WriteRecord(NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID, 1, speedDialTable, NVRAM_SPEEDDIAL_RECORD_SIZE, &error);

	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	DeleteUptoScrID(SCR_SPEED_DIAL_LIST);
}
/*END CSD 20041011 Wilson*/

/*****************************************************************************
* FUNCTION
*  PhbSpeedDailEntryHiliteHandler
* DESCRIPTION
*   This function is Highlight handler for Phonebook->SpeedDial->Set Number
*   Follow functions: EntryPhnsetMainMenuStyle
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbSpeedDailEntryHiliteHandler(S32 index) {
	currentKeyIndex = (S16) index ;
	memset(number_temp, 0, sizeof(number_temp));
	memcpy(number_temp, (const void *)speedDialTable[currentKeyIndex].number, sizeof(number_temp));	
}




/*****************************************************************************
* FUNCTION
*  PhbMakeCallFromSpeedDial
* DESCRIPTION
*   This function is to make phone call by pressing SEND key from speed dial screen
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbMakeCallFromSpeedDial(void) 
{
	if(!speedDialTable[currentKeyIndex].record_index)
		DisplayPopup((PU8)GetString(STR_SPEEDDIAL_NO_NUM_TODIAL_TEXT) ,IMG_GLOBAL_ERROR, 1,ST_NOTIFYDURATION,ERROR_TONE);
	else 
	{
		if(!pfnUnicodeStrlen((PS8)speedDialTable[currentKeyIndex].number))
			DisplayPopup((PU8)GetString(STR_SPEEDDIAL_NO_NUM_TODIAL_TEXT) ,IMG_GLOBAL_ERROR, 1,ST_NOTIFYDURATION,ERROR_TONE);
		else 
		{
//micha0420
			if(speedDialTable[currentKeyIndex].storage >= MMI_SIM && speedDialTable[currentKeyIndex].storage <= MMI_SIM4 &&g_SecuritySetupContext.FdlStatus[speedDialTable[currentKeyIndex].storage - MMI_SIM])
			{
				DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
			}
			else
			{
				PhbSpeedDialLookupNumberFromList(currentKeyIndex);
				MakeCall((PS8)speedDialTable[currentKeyIndex].number);
			}
		}
	}	

}


/*****************************************************************************
* FUNCTION
*  PhbGetSpeedDialNum
* DESCRIPTION
*   This function is to get speed dail number by keyID (2~9) when LP digit number from idle screen
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
PS8 PhbGetSpeedDialNum(U16 keyId) 
{
	PS8 number;

	if(keyId < 2) 
	   return NULL;
	
	if(pfnUnicodeStrlen((PS8)speedDialTable[keyId - 2].number)) 
	{
			PhbSpeedDialLookupNumberFromList((U16)(keyId - 2));
			number = (PS8)speedDialTable[keyId - 2].number;
			return number;
	}
	else
		return NULL;
}



/*****************************************************************************
* FUNCTION
*  PhbUpdateSpeedDial
* DESCRIPTION
*   This function is an update function to modify certain speed dial entry when user change from phone book
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbUpdateSpeedDial(U8 storage, U16 record_index, U8 type, U8* name, U8* number) {
	S16 error;
	U8 i, flag = 0;

	for(i =0; i <MAX_SPEED_DIAL_NUMBER; ++i) 
	{
 		if((speedDialTable[i].storage == storage) && (speedDialTable[i].record_index == record_index)) 
 		{
			if(name == NULL && number == NULL) 
			{
				speedDialTable[i].storage = 0;
				speedDialTable[i].type = 0;
				speedDialTable[i].record_index = 0;
				memset(speedDialTable[i].name, 0, ENCODING_LENGTH);
				memset(speedDialTable[i].number, 0, ENCODING_LENGTH);
			}
			else 
			{
				pfnUnicodeStrcpy((PS8)speedDialTable[i].name, (PS8)name);
				pfnUnicodeStrcpy((PS8)speedDialTable[i].number, (PS8)number);
			 }
			flag = 1;
            }
	}
	
	if(flag) 
	   WriteRecord(NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID, 1, speedDialTable, NVRAM_SPEEDDIAL_RECORD_SIZE, &error);
}



/*****************************************************************************
* FUNCTION
*  PhbDeleteAllSpeedDial
* DESCRIPTION
*   This function is an update function to delete all speed dial entry when delete all entry from phone book
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbDeleteAllSpeedDial(U8 storage, U8 type) {
	U8 i, flag = 0;
	S16 error;

	for(i = 0; i < MAX_SPEED_DIAL_NUMBER; ++i) 
	{
		if(speedDialTable[i].storage == storage) 
		{
			memset(&speedDialTable[i], 0, sizeof(speedDialTable[i]));
			flag = 1;
		}
	}

	if( flag ) 
	   WriteRecord(NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID, 1, speedDialTable, NVRAM_SPEEDDIAL_RECORD_SIZE, &error);
}


/*****************************************************************************
* FUNCTION
*  PhbRestoreSpeedDial
* DESCRIPTION
*   This function is to restore speed dail to default value (all empty)
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
MMI_BOOL PhbRestoreSpeedDial(void) 
{
	S16 error;
	memset(speedDialTable, 0, NVRAM_SPEEDDIAL_RECORD_SIZE);
	WriteRecord(NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID, 1, speedDialTable, NVRAM_SPEEDDIAL_RECORD_SIZE, &error);
	return MMI_TRUE;
}


/*****************************************************************************
* FUNCTION
*  PhbGetSpeedDialStatus
* DESCRIPTION
*   This function is to get speed dial on/off status 
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
MMI_BOOL PhbGetSpeedDialStatus(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_SPEED_DIAL, &data, DS_BYTE , &error);

	if(data == 1)
	{
		CurrentSpeedType = 1;
		return MMI_TRUE;
	}
	else
	{
		CurrentSpeedType = 0;
		return MMI_FALSE;
	}

}

/*****************************************************************************
* FUNCTION
*  PhbSpeedDialLookupNumberFromList
* DESCRIPTION
*  Check if the entry exist in phonebook list, if Yes, dial out from list, do not lookup number again
*
* PARAMETERS
*  speed_index  IN  : speed dial entry index
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhbSpeedDialLookupNumberFromList(U16 speed_index)
{
	U16 i;
	U16 store_index;
	
	/*
	* Check if the entry exist in phonebook list, if Yes, dial out from list, do not lookup number again
	*/
	if(speedDialTable[speed_index].storage >= MMI_SIM && speedDialTable[speed_index].storage < MMI_SIM4)
		store_index = speedDialTable[speed_index].record_index + MAX_PB_PHONE_ENTRIES - 1 + MAX_PB_SIM_ENTRIES *(speedDialTable[speed_index].storage - MMI_SIM);
	else
		store_index = speedDialTable[speed_index].record_index - 1;

	for(i = 0 ; i < PhoneBookEntryCount ; i++)
		if(g_phb_name_index[i] == store_index)
		{
			g_phb_cntx.dial_from_list = MMI_PHB_PHONEBOOK;
			g_phb_cntx.active_index  = i;
			break;
		}
}

#endif /* _SPEEDDIAL_C */


