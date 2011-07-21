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
 *	CallWaiting.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   <file description>.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: CallWaiting.c


  	PURPOSE		: Call waiting application in call setup.

 

 

	DATE		: 11/14/2003

**************************************************************/
#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mainmenudef.h"
#include "protocolevents.h"
#include "callsetup.h"
#include "settingprofile.h"
#include "callwaiting.h"
#include "callsetupenum.h"
#include "commonscreens.h"
#include "callmanagementiddef.h"
#include "callmanagementgprot.h"
#include "callsdefs.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim_call.h"
#endif
#undef __NEWSIMULATOR


/**************************************************************

	FUNCTION NAME		: InitCallWaiting(void)

  	PURPOSE				: Initializes the call waiting application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitCallWaiting()
{
	/* set hilite handlers for the individual menu items */
	SetHiliteHandler(MENU8237_SCR8093_MNGCALL_MENU_WAIT,HighlightCALLSETCallWaiting);
	SetHiliteHandler(MENU8237_MNGCALL_CALWAIT_MENU_ON,HighlightCALLSETCallWaitingOn);
	SetHiliteHandler(MENU8237_MNGCALL_CALWAIT_MENU_OFF,HighlightCALLSETCallWaitingOff);
	SetHiliteHandler(MENU8237_MNGCALL_CALWAIT_MENU_QUERY,HighlightCALLSETCallWaitingQuery);
	return;	
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallWaiting(void)

  	PURPOSE				: highlite handler for call waiting options 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallWaiting()
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryCALLSETWaitingMain,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETWaitingMain, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallWaitingOn(void)

  	PURPOSE				: highlite handler for callwaiting on option

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallWaitingOn(void)
{
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryScrCallWaitingOnreq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallWaitingOff(void)

  	PURPOSE				: highlight handler for call waiting off option 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallWaitingOff(void)
{
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryScrCallWaitingOffreq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallWaitingQuery(void)

  	PURPOSE				: highlight handler for call waiting query status option

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallWaitingQuery(void )
{
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryScrCallWaitingQueryreq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryScrCallWaitingOnreq(void)

  	PURPOSE				: function for call waiting on req.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrCallWaitingOnreq()
{
	S8 input_string[MAX_DIGITS_USSD];
	S8 uniinput_string[MAX_DIGITS_USSD * ENCODING_LENGTH];
	/* format the ss string for the call waiting on req */
	memset(input_string,0, MAX_DIGITS_USSD);
	strcpy((S8*) input_string, "*43#");
	memset(uniinput_string,0, (MAX_DIGITS_USSD * ENCODING_LENGTH));
	AnsiiToUnicodeString((PS8) uniinput_string,(PS8)input_string);
	
	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif

	/* make a corresponding ss string for call waiting option */
	MakeCall((PS8) uniinput_string);
	/* delete screens for maitaining hte history */
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETWaitingMain(void)

  	PURPOSE				: entry function for the call waiting menu option 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryCALLSETWaitingMain(void )
{
	U16 nStrItemList[MAX_SUB_MENUS];/* Stores the strings id of submenus returned */
	U16 nNumofItem;		 /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */

	EntryNewScreen(SCR_ID_CALLSET_WAITING_MAIN, NULL, EntryCALLSETWaitingMain, NULL);
	
	/* Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_WAITING_MAIN);	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU8237_SCR8093_MNGCALL_MENU_WAIT);
	/* Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU8237_SCR8093_MNGCALL_MENU_WAIT);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU8237_SCR8093_MNGCALL_MENU_WAIT, nStrItemList);
	/* Set current parent id*/
	SetParentHandler(MENU8237_SCR8093_MNGCALL_MENU_WAIT);
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	/* Display Category15 Screen */
	ShowCategory15Screen(STR_SCR8094_MNGCALL_CALWAIT_MENU_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK , IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16 *)gIndexIconsImageList, 1,0,guiBuffer);
	/* Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryScrCallWaitingOffreq(void)

  	PURPOSE				: function for call waiting on req.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrCallWaitingOffreq()
{
	S8 input_string[MAX_DIGITS_USSD];
	S8 uniinput_string[MAX_DIGITS_USSD * ENCODING_LENGTH];
	/* format ss string for call waiting off req */
	memset(input_string,0, MAX_DIGITS_USSD);
	strcpy((S8*) input_string, "#43#");

	memset(uniinput_string,0, (MAX_DIGITS_USSD * ENCODING_LENGTH));
	AnsiiToUnicodeString((PS8) uniinput_string,(PS8)input_string);

	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif

	/* make a corresponding ss string for call waiting option */
	MakeCall((PS8) uniinput_string);
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryScrCallWaitingQueryreq(void)

  	PURPOSE				: function for call waiting on req.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrCallWaitingQueryreq(void )
{
	S8 input_string[MAX_DIGITS_USSD];
	S8 uniinput_string[MAX_DIGITS_USSD * ENCODING_LENGTH];
	/* format ss string for call waiting query req*/
	memset(input_string,0, MAX_DIGITS_USSD);
	strcpy((S8*) input_string, "*#43#");
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETWaitingRsp, PRT_MMI_SS_CALL_WAIT_RSP);
#else
	SetProtocolEventHandler(CALLSETWaitingRsp, PRT_MMI_SS_CALL_WAIT_RSP);
#endif 
	memset(uniinput_string,0, (MAX_DIGITS_USSD * ENCODING_LENGTH));
	AnsiiToUnicodeString((PS8) uniinput_string,(PS8)input_string);

	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif

	/* make a corresponding ss string for call waiting option */
	MakeCall((PS8) uniinput_string);
	return;
}
/**************************************************************

	FUNCTION NAME		: CALLSETWaitingRsp(void * info)

  	PURPOSE				: response handler for call waiting query response

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void  aud_speech_set_mode(kal_bool on);
void CALLSETWaitingRsp(void * info)
{
	U8 bs_index;

	mmi_ss_call_wait_rsp_struct *pMsgCallWaitRsp = 
		(mmi_ss_call_wait_rsp_struct *) info ;

	memset(g_callset_cntx_p->SSDisplayBuffer,0, MAX_DISP_UCS2);

	DeleteScreenIfPresent(SCR_ID_CALLSET_WAITING_MAIN);
	
	/* check the status of the response result */
	if(pMsgCallWaitRsp->result.flag == L4C_OK)
	{
		/* if the response is success check the type */
		/* format the result string */
		if (pMsgCallWaitRsp->status & SS_ABIT) 
		{
			/* format the result string with active/deactive tags. */
			pfnUnicodeStrcpy((PS8)g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_SCR8096_MNGCALL_CALLWAIT_ON));
			pfnUnicodeStrcat((PS8) g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_ACTIVATED));
			for ( bs_index = 0; bs_index < pMsgCallWaitRsp->count; bs_index++)	{
				if (( pfnUnicodeStrlen((PS8)GetString(STR_L4_ALL_TELESERVICES_EXCEPT_SMS))
						+ pfnUnicodeStrlen((PS8)g_callset_cntx_p->SSDisplayBuffer)) < MAX_DISP_UCS2 / ENCODING_LENGTH) {
				AppendBScodeString( pMsgCallWaitRsp->list[bs_index], (PS8)g_callset_cntx_p->SSDisplayBuffer);
				}
			}
		} else 
		{
			/* format the result string with active/deactive tags. */
			pfnUnicodeStrcpy((S8*) g_callset_cntx_p->SSDisplayBuffer, (PS8)GetString(STR_SCR8096_MNGCALL_CALLWAIT_ON));
			pfnUnicodeStrcat((PS8) g_callset_cntx_p->SSDisplayBuffer,(PS8)GetString(STR_DEACTIVATED));
			for ( bs_index = 0; bs_index < pMsgCallWaitRsp->count; bs_index++) 
			{
				if (( pfnUnicodeStrlen((PS8)GetString(STR_L4_ALL_TELESERVICES_EXCEPT_SMS))
						+ pfnUnicodeStrlen((PS8)g_callset_cntx_p->SSDisplayBuffer)) < MAX_DISP_UCS2 / ENCODING_LENGTH) 
				{
					AppendBScodeString( pMsgCallWaitRsp->list[bs_index], (PS8)g_callset_cntx_p->SSDisplayBuffer);
				}
			}
		}
		/* call the result screen function to display the result string */
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
	} 
	else 
	{
		/* in case of response failure */
#ifdef  __MMI_SS_SHOW_CAUSE__
		ShowCauseString( pMsgCallWaitRsp->result.cause, (PS8)g_callset_cntx_p->SSDisplayBuffer);
//micha0322
		DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
		EntryCALLSETGeneralSSResut();
#else
		/* display the not done popup */
		DisplayPopup( (PU8) GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
//micha0915
		RemoveSSReqScr();
#endif
	}
	
	aud_speech_set_mode(KAL_FALSE); //chenhe,070808. for callwaiting,CM_KB_OUTGOINGCALL is been sent,so have to do so here
	return;
}



