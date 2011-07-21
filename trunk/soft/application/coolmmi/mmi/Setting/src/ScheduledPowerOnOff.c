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
 *	ScheduledPowerOnOff.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file is intends for scheduled power on / off application
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
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2004
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *	SchedulePowerOnOff.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements schedule power on of applications.
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

/***************** Settings Application implementation *********************/

/**************************************************************

	FILENAME	: ScheduledPowerOnoff.c

  	PURPOSE		: Scheduled power on of fhandling

 

	AUTHOR		: Gautam Saluja

	DATE		: 

**************************************************************/
#ifndef _MMI_SCHEDULEPOWERONOFF_C
#define _MMI_SCHEDULEPOWERONOFF_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_data_types.h"

#include "globaldefs.h"
#include "debuginitdef.h"
#include "scheduledpoweronoff.h" 
#include "nvramtype.h"
#include "nvramenum.h"
#include "nvramprot.h"

#include "alarmframeworkprot.h"
#include "alarmgprot.h"
#include "datetimegprot.h"

#include "wgui_categories.h"
#include "settingdefs.h"
#include "commonscreens.h"
//... Add More MMI header
#undef __NEWSIMULATOR 

/*  Include: PS header file */
//... Add More PS header


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
spof_context_struct g_spof_cntx;
extern wgui_inline_item wgui_inline_items[];

/* 
** Global Function
*/
extern void ShutdownSystemOperation(void);
void SpofEntryPwrOffInd(void);

/*****************************************************************************
* FUNCTION
*	SPOFInit
* DESCRIPTION
*   Initialize SPOF application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void SPOFInit(void)
{
}


/*****************************************************************************
* FUNCTION
*	SPOFDeInit
* DESCRIPTION
*   Deinitialize SPOF application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void SPOFDeInit(void)
{
}


/*****************************************************************************
* FUNCTION
*	SPOFInitNvRam
* DESCRIPTION
*   Read nvram data of SPOF
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void SPOFInitNvRam(void) 
{
    S16 error;
	U8 nvram_buf[NVRAM_ALM_SPOF_DATA_SIZE];
    
    ReadRecord(NVRAM_EF_ALM_SPOF_DATA_LID, 2, &nvram_buf, NVRAM_ALM_SPOF_DATA_SIZE, &error);
    memcpy(g_spof_cntx.SPOFList,nvram_buf, sizeof(g_spof_cntx.SPOFList));

    /* zero initialized */
	if(g_spof_cntx.SPOFList[0].Hour > NUM_HRS_DAY)
		memset(&g_spof_cntx.SPOFList,0,sizeof(g_spof_cntx.SPOFList));
	
	/* no require to set any alarm */
}


/*****************************************************************************
* FUNCTION
*	EntrySpofApp
* DESCRIPTION
*   Entry function of schedule power on off application.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntrySpofApp(void)
{
	U8* guiBuffer;
	S32 i;
	S32 num_of_items = NUM_OF_SPOF;
	MYTIME	tmpTime;
	U16 spof_states_icon[NUM_OF_SPOF];
	U16 spof_type_icon[NUM_OF_SPOF];
	
#ifdef __MMI_TWO_SPOF__
	num_of_items = 2;
#endif

	EntryNewScreen(SCR_ID_SPOF_APP, NULL, EntrySpofApp, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SPOF_APP);
	if(guiBuffer == NULL)
		g_spof_cntx.CurrItem = 0;

	/* initialize list to display */
	for (i=0;i<NUM_OF_SPOF;i++) 
	{
		/* get the value of spof_list*/ 
		memset(&tmpTime,0,sizeof(MYTIME));
		
		tmpTime.nHour = g_spof_cntx.SPOFList[i].Hour;
		tmpTime.nMin = g_spof_cntx.SPOFList[i].Min;
		
		time_string(&tmpTime,(UI_string_type)subMenuDataPtrs[i],DT_IDLE_SCREEN);
		
		/* get the value of spof_vicons*/   
#if defined(__PROJECT_GALLITE_C01__)
		if(g_spof_cntx.SPOFList[i].Status == SPOF_DISABLE) 
			spof_states_icon[i] = IMG_VICON;
		else 
		{      
        		/*get the value of spof_type poweon off */
        		if(g_spof_cntx.SPOFList[i].Type == SPOF_POWERON)
        			spof_states_icon[i] = SPOF_TYPEPOWERON_IMAGEID;
        		else
        			spof_states_icon[i] = SPOF_TYPEPOWEROFF_IMAGEID;
		}
#else 
		if(g_spof_cntx.SPOFList[i].Status == SPOF_DISABLE) 
			spof_states_icon[i] = SPOF_BLANK_IMAGEID;
		else 
			spof_states_icon[i] = IMG_VICON;
#endif
		/*get the value of spof_type poweon off */
		if(g_spof_cntx.SPOFList[i].Type == SPOF_POWERON)
			spof_type_icon[i] = SPOF_TYPEPOWERON_IMAGEID;
		else
			spof_type_icon[i] = SPOF_TYPEPOWEROFF_IMAGEID;

	}

	ShowCategory73Screen(SPOF_MAIN_TITLE_STRINGID,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_EDIT,
    	SPOF_EDIT_BUTTON_IMAGEID,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,num_of_items,subMenuDataPtrs,
	spof_states_icon,spof_type_icon, 0,guiBuffer,1);

	RegisterHighlightHandler(HighlightSpofList);

	SetLeftSoftkeyFunction(EntrySpofEdit,KEY_EVENT_UP);	
	SetKeyHandler(EntrySpofEdit, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightSpofList
* DESCRIPTION
*   Highlight handler of SPOF list
* PARAMETERS
*	nIndex	IN	index item
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightSpofList(S32 nIndex)
{
	g_spof_cntx.CurrItem = (U8)nIndex;
}

/*****************************************************************************
* FUNCTION
*	HighlightSPOFInlineItem
* DESCRIPTION
*   Highlight handler of inline editor screen
* PARAMETERS
*	nIndex	IN	index of inline item
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightSPOFInlineItem(S32 nIndex)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction (SpofSaveData);	
}


/*****************************************************************************
* FUNCTION
*	EntrySpofEdit
* DESCRIPTION
*   Entry function of inline editor screen of schedule power on off
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntrySpofEdit(void)
{
	U8* guiBuffer;
	U8* inputBuffer;			//added for inline edit history
	U16 inputBufferSize;		//added for inline edit history
	U16 spof_icon_list[] = {SPOF_SETTING_STATUS_IMAGEID, 0, SPOF_TYPEPOWERON_IMAGEID,	IMG_TIME};

	EntryNewScreen(SCR_ID_SPOF_EDITOR, ExitSpofEdit, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SPOF_EDITOR);
	InitializeCategory57Screen();
	SpofFillInlineEditStruct();

	inputBuffer = GetCurrNInputBuffer (SCR_ID_SPOF_EDITOR, &inputBufferSize );    //added for inline edit history
	if(inputBuffer != NULL)
		SetCategory57Data(wgui_inline_items, NUM_ITEM_EDIT_SCREEN, inputBuffer);

	RegisterHighlightHandler(HighlightSPOFInlineItem);
	ShowCategory57Screen(SPOF_EDIT_TITLE_STRINGID,IMG_SCR_SETTING_CAPTION,
		STR_GLOBAL_OK,IMG_GLOBAL_OK,STR_GLOBAL_BACK, 
		IMG_GLOBAL_BACK,NUM_ITEM_EDIT_SCREEN,spof_icon_list,
		wgui_inline_items,0,guiBuffer);

	DisableCategory57ScreenDone();

	SetCategory57RightSoftkeyFunctions(SpofSaveData,GoBackHistory);
}


/*****************************************************************************
* FUNCTION
*	ExitSpofEdit
* DESCRIPTION
*   Exit Handler For SPOF Edit Screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitSpofEdit(void)
{
	history_t   Scr; 
	U16 inputBufferSize;                //added for inline edit history
 
	S16 nHistory =0;
	Scr.scrnID = SCR_ID_SPOF_EDITOR;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntrySpofEdit;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, inputBufferSize);                       //added for inline edit history
}

 
/*****************************************************************************
* FUNCTION
*	SpofFillInlineEditStruct
* DESCRIPTION
*   Initialize inline editor structure
* PARAMETERS
*	None.
 * RETURNS
*	None.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void SpofFillInlineEditStruct(void)
{
	S8 dummyString[12];

	/* initialize string resource for inline selection */
	g_spof_cntx.ActStateList[0] = GetString(SPOF_DISABLE_STRINGID);
	g_spof_cntx.ActStateList[1] = GetString(SPOF_ENABLE_STRINGID);
	g_spof_cntx.OnOffStateList[0] = GetString(STR_MENU_SETTING_SCH_POW_SET1_POW_ON);
	g_spof_cntx.OnOffStateList[1] = GetString(STR_MENU_SETTING_SCH_POW_SET1_POW_OFF);
	
	g_spof_cntx.CurrHiliteOnOff = g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Type;
	g_spof_cntx.CurrHiliteActivation= g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Status;

	/* hour buffer */
	sprintf(dummyString,"%02d",g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Hour);
	AnsiiToUnicodeString(g_spof_cntx.HourBuf,dummyString);

	/* minute buffer */
	sprintf(dummyString,"%02d",g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Min);
	AnsiiToUnicodeString(g_spof_cntx.MinBuf,dummyString); 

	/* caption */
	SetInlineItemCaption(&wgui_inline_items[0],(PU8)GetString(SPOF_STATUS_STRINGID));

	/* activate/inactivate */
	SetInlineItemActivation(&wgui_inline_items[1],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemSelect(&wgui_inline_items[1],SPOF_NUM_STATES_ACTINACT,(U8**)g_spof_cntx.ActStateList,&g_spof_cntx.CurrHiliteActivation);

    /* power on/off */
	SetInlineItemActivation(&wgui_inline_items[2],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	DisableInlineItemHighlight(&wgui_inline_items[2]);
	SetInlineItemSelect(&wgui_inline_items[2],SPOF_NUM_STATES_ONOFF,(U8**)g_spof_cntx.OnOffStateList,&g_spof_cntx.CurrHiliteOnOff);

	/* time */
	SetInlineItemActivation(&wgui_inline_items[3],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemTime(&wgui_inline_items[3],(U8*)g_spof_cntx.HourBuf,(U8*)g_spof_cntx.MinBuf,NULL,AlmTimeEditorCallBack/*SOPFTimeEditorCallBack*/);
	DisableInlineItemHighlight(&wgui_inline_items[3]);
}


/*****************************************************************************
* FUNCTION
*	SpofSaveData
* DESCRIPTION
*   Save SPOF data to NVRAM
* PARAMETERS
*	None.
 * RETURNS
*	None.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void SpofSaveData(void)
{
	U8 NewHour, NewMin;

	NewHour =  (g_spof_cntx.HourBuf[0]-'0')* 10 + (g_spof_cntx.HourBuf[ENCODING_LENGTH]-'0');
	NewMin = (g_spof_cntx.MinBuf[0]-'0')* 10 + (g_spof_cntx.MinBuf[ENCODING_LENGTH]-'0');

	if(SpofIsTimeClash(NewHour, NewMin)) 
	{
		DisplayPopup((PU8) GetString(SPOF_TIME_CONFLICT), IMG_GLOBAL_UNFINISHED, 0, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
		return;
	}
	
	/* if setting changed */
	if(g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Hour !=  NewHour ||
		g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Min != NewMin ||
		g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Type != (U8)g_spof_cntx.CurrHiliteOnOff ||
		g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Status!= g_spof_cntx.CurrHiliteActivation)
	{
		/* fill the SPOFList structure depending on the selected values */
		g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Status = (U8)g_spof_cntx.CurrHiliteActivation;
		g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Type = (U8)g_spof_cntx.CurrHiliteOnOff;
		g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Hour = NewHour;
		g_spof_cntx.SPOFList[g_spof_cntx.CurrItem].Min = NewMin;

		/*save the alarm in the nvram*/
		SpofWritetoNvram();

		if(g_spof_cntx.CurrHiliteActivation == SPOF_ENABLE)
		{
			if(g_spof_cntx.CurrHiliteOnOff == SPOF_POWERON)
			{
				AlmCancelAlarm((U8)(g_spof_cntx.CurrItem+ALM_SPOF_START));
				AlmSetAlarm((U8)(g_spof_cntx.CurrItem+ALM_SPON_START));
			}
			else
			{
				AlmCancelAlarm((U8)(g_spof_cntx.CurrItem+ALM_SPON_START));
				AlmSetAlarm((U8)(g_spof_cntx.CurrItem+ALM_SPOF_START));
			}
		}
		else
		{
			AlmCancelAlarm((U8)(g_spof_cntx.CurrItem+ALM_SPON_START));
			AlmCancelAlarm((U8)(g_spof_cntx.CurrItem+ALM_SPOF_START));
		}
	}

	
	if(!g_spof_cntx.IsRestoreDefault)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);	
		DeleteUptoScrID(SCR_ID_SPOF_APP);
	}
}



#ifdef __MMI_ENGINEER_MODE__
extern MYTIME IncrementTime(MYTIME timeToIncrement,MYTIME increment);
void SetAutoPwrONdata(void)
{

	MYTIME currTime;
	MYTIME incTime;
	MYTIME ResultTime;
	DTGetRTCTime(&currTime);
	memset(&incTime, 0, sizeof(incTime));
	incTime.nMin= 1;
	incTime.nSec= 20;
	ResultTime = IncrementTime(currTime,incTime);
		/* fill the SPOFList structure depending on the selected values */
		g_spof_cntx.SPOFList[0].Status = SPOF_ENABLE;
		g_spof_cntx.SPOFList[0].Type = SPOF_POWERON;
		g_spof_cntx.SPOFList[0].Hour = ResultTime.nHour;
		g_spof_cntx.SPOFList[0].Min = ResultTime.nMin;

		/*save the alarm in the nvram*/
		SpofWritetoNvram();

		AlmCancelAlarm((U8)(ALM_SPOF_START));
		AlmSetAlarm((U8)(ALM_SPON_START));


}
#endif
/*****************************************************************************
* FUNCTION
*	SpofWritetoNvram
* DESCRIPTION
*   Write SPOF Value to NVRAM
* PARAMETERS
*	None.
 * RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void SpofWritetoNvram(void)
{
	S16 error;
	U8 nvram_buf[NVRAM_ALM_SPOF_DATA_SIZE];
	
	memset(nvram_buf, 0, NVRAM_ALM_SPOF_DATA_SIZE);
	memcpy(nvram_buf,g_spof_cntx.SPOFList, sizeof(g_spof_cntx.SPOFList));
	WriteRecord(NVRAM_EF_ALM_SPOF_DATA_LID, 2, (void*)nvram_buf, NVRAM_ALM_SPOF_DATA_SIZE, &error);
}


/*****************************************************************************
* FUNCTION
*	SpofHandler
* DESCRIPTION
*   Match The ALARM With SPOF Index, If Matches 
*		1. If Phone is off, switch on the phone else normally expire the alarm if POWERON Expire
*		2. If Phone is on, switch off the phone else normally expire the alarm if PowerOff Expire
* PARAMETERS
*	id	IN	index of spof alarm
 * RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void SpofHandler(U8 index)
{
	mmi_trace(1," SpofHandler index = %d ,g_alm_frm_cntx.IsPwronAlarm = %d ",(index-ALM_SPOF_START),g_alm_frm_cntx.IsPwronAlarm );
	
	if(index >= ALM_SPOF_START)
		index -= ALM_SPOF_START;
	
	/* depending on the index check whether it was power on or off */
	if((g_spof_cntx.SPOFList[index].Type == SPOF_POWERON) && (g_alm_frm_cntx.IsPwronAlarm == TRUE))
	{
		AlmSendPowerOn();
	}
	else if((g_spof_cntx.SPOFList[index].Type == SPOF_POWEROFF) && (g_alm_frm_cntx.IsPwronAlarm == FALSE))
	{
		SpofEntryPwrOffInd();
	} 
}


/*****************************************************************************
* FUNCTION
*	SpofRestore
* DESCRIPTION
*   Restore default value of SPOF variables and save to NVRAM
* PARAMETERS
*	None.
 * RETURNS
*	None.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void SpofRestore(void)
{
	S32 i;
	g_spof_cntx.CurrHiliteActivation= SPOF_DISABLE;
	g_spof_cntx.CurrHiliteOnOff = SPOF_POWERON;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== SpofRestore(): Cancel SPOF Aalrm. ===\n"));

	for(i=0;i<NUM_OF_SPOF;i++)	
		if(g_spof_cntx.SPOFList[i].Status == SPOF_ENABLE)
		{
			AlmCancelAlarm((U8)(ALM_SPOF_START+i));
			AlmCancelAlarm((U8)(ALM_SPON_START+i));
		}

	memset(g_spof_cntx.SPOFList,0,sizeof(g_spof_cntx.SPOFList));
	SpofWritetoNvram();
}


/*****************************************************************************
* FUNCTION
*	SpofIsTimeClash
* DESCRIPTION
*	To check if the setting time clashes with other SPOF alarm
* PARAMETERS
*	hour	IN	hour to be compared.
*	min		IN	min to be compared.
 * RETURNS
*	TRUE if clash, otherwise FALSE.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL SpofIsTimeClash(U8 hour, U8 min)
{
	S32 i;

	if(g_spof_cntx.CurrHiliteActivation == SPOF_ENABLE) 
	{
		for(i=0; i<NUM_OF_SPOF;i++) 
		{
			if(i!=g_spof_cntx.CurrItem)
				if (g_spof_cntx.SPOFList[i].Status == SPOF_ENABLE && hour == g_spof_cntx.SPOFList[i].Hour  && min == g_spof_cntx.SPOFList[i].Min) 
					return TRUE;
		}
	}

	return FALSE;
}

extern void TurnOnBacklight(U8 TimerFlag);
extern void EntryIdleScreen(void);
extern U8 isIdleScreenReached;
void QuitPwrOffIndBackToIdleScreen(void)
{
	StopTimer(SYSTEM_SHUTDOWN);
	if(isIdleScreenReached == 1)
		EntryIdleScreen();
	else
		GoBackHistory();
}
void QuitPwrOffIndBack(void)
{
	StopTimer(SYSTEM_SHUTDOWN);
	GoBackHistory();
}
void SpofEntryPwrOffInd(void)
{
	mmi_trace(1," SpofEntryPwrOffInd ");
	
	TurnOnBacklight(1);
	
	EntryNewScreen(SCR_ID_SPOF_PWROFF_IND, NULL, SpofEntryPwrOffInd, NULL);

	ClearAllKeyHandler();
	ClearKeyHandler( KEY_END, KEY_EVENT_DOWN );
	ClearKeyHandler( KEY_END, KEY_LONG_PRESS );
	/*modify the right softkey caption from "Back" to "Cancel" by liuxl on 20090626*/
	ShowCategory66Screen(  SPOF_MAIN_TITLE_STRINGID,
                                            #if defined(__PROJECT_GALLITE_C01__)
							NULL,
                                            #else
							SPOF_TYPEPOWEROFF_IMAGEID,
                                            #endif
							STR_GLOBAL_OK, 0, 
							STR_GLOBAL_CANCEL, 0, 
							(PU8)GetString(SPOF_POWER_OFF_CONFIRM), 
                                            #if defined(__PROJECT_GALLITE_C01__)
							NULL,
                                            #else
							SPOF_TYPEPOWEROFF_IMAGEID,
                                            #endif 
							NULL);

	SetLeftSoftkeyFunction(ShutdownSystemOperation, KEY_EVENT_UP);
	//wangzl: 090912 modify for 14334
	//SetRightSoftkeyFunction(QuitPwrOffIndBackToIdleScreen, KEY_EVENT_UP);
	SetRightSoftkeyFunction(QuitPwrOffIndBack, KEY_EVENT_UP);
	//wangzl: 090912 modify end

	StartTimer(SYSTEM_SHUTDOWN, 10*1000, ShutdownSystemOperation);
}

#endif // #ifndef _MMI_SCHEDULEPOWERONOFF_C


