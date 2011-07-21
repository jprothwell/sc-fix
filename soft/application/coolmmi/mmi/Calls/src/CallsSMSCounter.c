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
 * CallsSMSCounter.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
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

/**************************************************************

	FILENAME	: CallsSMSCounter.c

  	PURPOSE		: To maintain sms counters in call history.

 

 

	DATE		: 10/22/2003

**************************************************************/

#ifndef _CALLS_SMS_COUNTER_C
#define _CALLS_SMS_COUNTER_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

//... Add More MMI header
#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "wgui_categories_popup.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "unicodexdcl.h"

#include "nvramenum.h"
#include "nvramtype.h"
#include "nvramprot.h"

#include "settingprofile.h"
#include "profilegprots.h"
#include "phonebooktypes.h"
#include "messagesexdcl.h"

#include "callhistoryenum.h"
#include "callsstruct.h"
#include "callsdefs.h"
#include "callssmscounter.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_calllog.h"
#endif /* __MMI_MULTI_SIM__ */
#undef __NEWSIMULATOR 

extern call_history_context_struct g_CallHistory_cntxt[];
/**************************************************************

	FUNCTION NAME		: InitSMSCounter

  	PURPOSE				: Initialize the hilite/hint Handlers

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitSMSCounter( void )
{
	/* set the menu hilite handlers */
	SetHiliteHandler( MENU_CALL_HISTORY_SMSCOUNTER, HighlightCHISTSMSCount );
	SetHiliteHandler(MENU_CALL_HISTORY_SENT,HighlightCHISTSentSMS);
	SetHiliteHandler(MENU_CALL_HISTORY_RECEIVED,HighlightCHISTRecvdSMS);
	/* set the menu hint handlers */
	SetHintHandler(MENU_CALL_HISTORY_SENT,HintCHISTSentSMS);
	SetHintHandler(MENU_CALL_HISTORY_RECEIVED,HintCHISTRecvdSMS);
	/* initialize the sms counter values reading from nvram */

	InitNvramSMSCounters();
	return;
}

/**************************************************************

	FUNCTION NAME		: InitNvramSMSCounters

  	PURPOSE				: Initialize NVRAM for SMS counters

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: S16

	RETURNS				: void

 

**************************************************************/
void InitNvramSMSCounters( void )
{
	U8 i;
	S16 error ;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		#ifdef __MMI_MULTI_SIM__
		SetCHISContext(i);
		#endif
		/* Initialize the sendsms Counter by reading from NVRAM */
		ReadValue( NVRAM_SMSCOUNTERS_SENT_SHORT + i, &chis_p->nSendSMSCount, DS_SHORT , &error);
		/*NVRAM read failure then reset the send sms counter */
		if( chis_p->nSendSMSCount == 0xffff)
				chis_p->nSendSMSCount = 0;

		/* Initialize the receive sms Counter by reading from NVRAM */
		ReadValue( NVRAM_SMSCOUNTERS_RECD_SHORT + i, &chis_p->nRecvSMSCount, DS_SHORT , &error);
		/*NVRAM read failure then reset the receive sms counter */
		if( chis_p->nRecvSMSCount == 0xffff)
			chis_p->nRecvSMSCount = 0;
	}	
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTSMSCount

  	PURPOSE				: To set key handlers for sms counter menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTSMSCount( void )
{
	/* Set the left/right arrow key handler on sms count selected */
	SetKeyHandler( EntryCHISTSMSCount,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	/* Register function for left soft key */
	SetLeftSoftkeyFunction( EntryCHISTSMSCount, KEY_EVENT_UP );
	/* Register function for right soft key */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTSentSMS(void)

  	PURPOSE				: Funtion is called when sent sms menu item selected

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTSentSMS(void)
{
	/* check the send sms counter is zero or not */
	if( chis_p->nSendSMSCount == 0)
	{
		/*Change left soft key icon and label if send sms counter is zero */
		ChangeLeftSoftkey( 0, 0);
		ClearOneKeyAllHandler(KEY_RIGHT_ARROW);
	}
	else
	{
		/*Change left soft key icon and label */
		ChangeLeftSoftkey( STR_SMS_COUNTER_LSK, IMG_GLOBAL_OK );
		/* Set key handler if the send sms counter is not zero */
		SetLeftSoftkeyFunction( EntryCHISTResetSentSMSConfirm,
		                        KEY_EVENT_UP );
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTRecvdSMS

  	PURPOSE				: Funtion is called when sent sms menu item selected

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTRecvdSMS(void)
{
	/* check the receive sms counter is zero or not */
	if( chis_p->nRecvSMSCount == 0)
	{
		/*Change left soft key icon and label if receive sms counter is zero */
		ChangeLeftSoftkey( 0, 0);
		ClearOneKeyAllHandler(KEY_RIGHT_ARROW);
	}
	else
	{
		/*Change left soft key icon and label */
		ChangeLeftSoftkey( STR_SMS_COUNTER_LSK, IMG_GLOBAL_OK );
		/* Set key handler if the receive sms counter is not zero */
		SetLeftSoftkeyFunction( EntryCHISTResetRecvdSMSConfirm,
		                        KEY_EVENT_UP );
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: HintCHISTSentSMS

  	PURPOSE				: To construct hint popup for sent sms counter

	INPUT PARAMETERS	: u16 - index of the current menu item

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTSentSMS(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	/* reinitialize the hint data */
	memset((PS8)hintData[index], 0, sizeof(hintData[index]));
	/* check send sms counter is zero or not */
	if (chis_p->nSendSMSCount == 0)
	{
		/* if zero then display No msg */
		pfnUnicodeStrncpy((S8 *)hintData[index],
		                  (S8 *)GetString( STR_SMS_NO_NUMBER ),
		                  MAX_GET_STRING_LEN);
	}
	else
	{
		/* if non zero; display the count with tag msg at the end */
		sprintf( (S8 *)value, "%d ", chis_p->nSendSMSCount );
		AnsiiToUnicodeString( (PS8)hintData[index], (PS8) value );
		pfnUnicodeStrncat((PS8)hintData[index],
		                  (S8 *)GetString( STR_SMS_STRING ),
		                  MAX_GET_STRING_LEN);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: HintCHISTRecvdSMS

  	PURPOSE				: To construct hint popup for receive sms counter

	INPUT PARAMETERS	: u16 - index of the current menu item

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTRecvdSMS(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	/* reinitialize the hint data */
	memset((PS8)hintData[index], 0, sizeof(hintData[index]));

	/* check receive sms counter is zero or not */
	if (chis_p->nRecvSMSCount == 0)
	{
		/* if zero then display No msg */
		pfnUnicodeStrncpy((S8 *)hintData[index],
		                  (S8 *)GetString( STR_SMS_NO_NUMBER ),
		                  MAX_GET_STRING_LEN);
	}
	else
	{
		/* if non zero; display the count with tag msg at the end */
		sprintf( (S8 *) value, "%d ", chis_p->nRecvSMSCount );
		AnsiiToUnicodeString( (PS8)hintData[index], (PS8)value );
		pfnUnicodeStrncat((PS8)hintData[index],
		                  (S8 *)GetString( STR_SMS_STRING ),
		                  MAX_GET_STRING_LEN);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTSMSCount

  	PURPOSE				: Entry function to display sms counter screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTSMSCount( void )
{
	U16			nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U8*			guiBuffer;					/* Buffer holding history data */
	U16			nNumofItem;      /* Stores no of children in the submenu*/
	//U16			nDispAttribute;  /* Stores display attribue */
	U8*			u8SmsCount[MAX_SUB_MENUS]; /* sms counters for display hint popups */

	/* Call Exit Handler*/
	EntryNewScreen(SCR8004_SMS_COUNTER, ExitCHISTSMSCount,
	               NULL, NULL);
	/* Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR8004_SMS_COUNTER );
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild( MENU_CALL_HISTORY_SMSCOUNTER );
	/* Get attribute of menu to be displayed */
	//nDispAttribute = GetDispAttributeOfItem( MENU_CALL_HISTORY_SMSCOUNTER );
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds( MENU_CALL_HISTORY_SMSCOUNTER, nStrItemList );
	/* Set current parent id*/
	SetParentHandler( MENU_CALL_HISTORY_SMSCOUNTER );
	/* Set current screen id*/
	SetMessagesCurrScrnID(SCR8004_SMS_COUNTER);		
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler( ExecuteCurrHiliteHandler );
	/* construct hint popups for the menu items */
	ConstructHintsList(MENU_CALL_HISTORY_SMSCOUNTER, u8SmsCount);
	/* Display Category52 Screen */
	ShowCategory52Screen( STR_SMS_CAPTION, IMG_SCR8056_CAPTION, STR_SMS_COUNTER_LSK,
	                      IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                      nNumofItem, nStrItemList, (U16 *)gIndexIconsImageList,
	                      u8SmsCount, 0, 0, guiBuffer );
	/* Register left arrow key */
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	/* Register function with left/right softkey */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTSMSCount

  	PURPOSE				: Exit funtion for sms counter screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTSMSCount( void )
{
	history_t	Scr; /* Variable to hold the history data*/
	S16		nHistory	= 0;
	/* getting details of the current screen (to be added to history) */
	Scr.scrnID = SCR8004_SMS_COUNTER;
	Scr.entryFuncPtr = EntryCHISTSMSCount;
	pfnUnicodeStrcpy( ( S8 * ) Scr.inputBuffer, ( S8 * ) &nHistory );
	GetCategoryHistory( Scr.guiBuffer );
	/* add the screen to history */
	AddHistory( Scr );
	SetMessagesCurrScrnID(0);
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTResetSentSMSConfirm

  	PURPOSE				: SMS sent counter reset confirmation screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTResetSentSMSConfirm( void )
{

	DisplayConfirm( STR_GLOBAL_YES,IMG_GLOBAL_YES,
							STR_GLOBAL_NO,IMG_GLOBAL_NO,
							get_string(STR_SCR_RESET_RCVD_SMS_CONFIRM_TEXT),
							IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* Set the left/right soft key funtions */
	SetLeftSoftkeyFunction(CHISTResetSentSMS,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the exit handler for the current screen */
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTResetRecvdSMSConfirm

  	PURPOSE				: SMS receive counter reset confirmation screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTResetRecvdSMSConfirm( void )
{

	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
			   STR_GLOBAL_NO,IMG_GLOBAL_NO,
				get_string(STR_SCR_RESET_RCVD_SMS_CONFIRM_TEXT),
				IMG_GLOBAL_QUESTION, WARNING_TONE);

	/* Set the left/right soft key funtions */
	SetLeftSoftkeyFunction(CHISTResetRecvdSMS,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the exit handler for the current screen */
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTResetSentSMS

  	PURPOSE				: Reset the send sms counter value

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTResetSentSMS( void )
{
   S16 error ;

	/* Reset the send sms counter value */
	chis_p->nSendSMSCount = 0;

	/* Update the send sms counter into NVRAM */
	WriteValue(NVRAM_SMSCOUNTERS_SENT_SHORT + chis_p->nSimId, &chis_p->nSendSMSCount, DS_SHORT , &error);

	/* display popup for the finish success case */
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,
	             IMG_GLOBAL_ACTIVATED, 1,
	             CS_NOTIFYDURATION,SUCCESS_TONE);

	/* delete upto sms counter main screen to maintain screen flow */
	DeleteUptoScrID(SCR8004_SMS_COUNTER);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTResetRecvdSMS

  	PURPOSE				: Reset the receive sms counter value

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTResetRecvdSMS( void )
{
	S16 error ;

	/* Reset the receive sms counter value */
	chis_p->nRecvSMSCount = 0;
	
	/* Update the receive sms counter into NVRAM */
	WriteValue(NVRAM_SMSCOUNTERS_RECD_SHORT + chis_p->nSimId, &chis_p->nRecvSMSCount, DS_SHORT , &error);
	
	/* display popup for the finish success case */
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,
	             IMG_GLOBAL_ACTIVATED, 1,
	             CS_NOTIFYDURATION,SUCCESS_TONE);

	/* delete upto sms counter main screen to maintain screen flow */
	DeleteUptoScrID(SCR8004_SMS_COUNTER);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTIncSentSMS

  	PURPOSE				: Function to increment send sms counter

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTIncSentSMS(U8 nsimID)
{
	S16 error ;
	
#ifdef __MMI_MULTI_SIM__
	SetCHISContext(nsimID);
#endif

	/* increment send sms counter */
	chis_p->nSendSMSCount++;
	if (chis_p->nSendSMSCount == 0)
		chis_p->nSendSMSCount = 1;

	/* update the send sms counter into NVRAM */
	WriteValue(NVRAM_SMSCOUNTERS_SENT_SHORT + chis_p->nSimId, &chis_p->nSendSMSCount,DS_SHORT , &error);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTIncRecvdSMS

  	PURPOSE				: To Increment receive sms counter

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTIncRecvdSMS( U8 nsimID )
{
	S16 error ;
#ifdef __MMI_MULTI_SIM__
	SetCHISContext(nsimID);
#endif

	/* increment send sms counter */
	chis_p->nRecvSMSCount++;
	if (chis_p->nRecvSMSCount == 0)
		chis_p->nRecvSMSCount = 1;

	/* update the send sms counter into NVRAM */
	WriteValue(NVRAM_SMSCOUNTERS_RECD_SHORT + chis_p->nSimId, &chis_p->nRecvSMSCount, DS_SHORT , &error);

	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTRefreshSMSCount

  	PURPOSE			: To Refresh sms counter

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTRefreshSMSCount(void)
{
	if(GetMessagesCurrScrnID()==SCR8004_SMS_COUNTER)
	{
		S32 menuItemId=-1;
		U8 value[MAX_SUB_MENU_HINT_SIZE]; 
		
		//menuItemId=GetChildMenuIDIndexByParentMenuID(MENU_CALL_HISTORY_SMSCOUNTER, MENU_CALL_HISTORY_RECEIVED);  // zrx del 20060612
		if(menuItemId!=-1)
		{
			memset( ( PS8 ) hintData[(menuItemId)], 0, sizeof(hintData[(menuItemId)]));
			sprintf( ( S8 * ) value, "%d ", chis_p->nRecvSMSCount );
			AnsiiToUnicodeString( ( PS8 ) hintData[(menuItemId)], ( PS8 ) value );
			pfnUnicodeStrcat( ( PS8 ) hintData[(menuItemId)], ( S8 * ) GetString( STR_SMS_STRING ));		
			Category52ChangeItemDescription(menuItemId,hintData[(menuItemId)]);
		}
		if(GetCurrHiliteID()==menuItemId)
		{
			ChangeLeftSoftkey( STR_SMS_COUNTER_LSK, IMG_GLOBAL_OK );
			SetLeftSoftkeyFunction( EntryCHISTResetRecvdSMSConfirm,
			                        KEY_EVENT_UP );
		}
		RedrawCategoryFunction ();
	}
}


#endif


