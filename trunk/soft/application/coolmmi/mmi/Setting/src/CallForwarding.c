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
 *	CallForwarding.c
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
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: CallForwarding.c

  	PURPOSE		: Call Setting - Call forwarding Application

 

 

	DATE		: 11/14/2003

**************************************************************/
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "csp.h"
#include "mainmenudef.h"
#include "protocolevents.h"
#include "phonebooktypes.h"
#include "phonebookgprot.h"
#include "callsetup.h"
#include "debuginitdef.h"
#include "callforwarding.h"
#include "callsetupenum.h"
#include "commonscreens.h"
#include "callmanagementiddef.h"
#include "messagesexdcl.h"
#include "callmanagementgprot.h"
#include "wgui_inputs.h"
#include "wgui_categories_inputs.h"
#include "mmi_features.h"

#include "wgui_status_icons.h"
#include "nvram_user_defs.h"
#include "nvramprot.h"
#include "callsdefs.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#endif /* __MMI_MULTI_SIM__ */

#undef __NEWSIMULATOR 
 
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwdCancelAll

  	PURPOSE				: Highlight handler for cancel all menu option

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallFwdCancelAll(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSCFType = FORWARD_CANCEL_ALL ;
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EnterScrCallFwdOffReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: InitCallForwarding

  	PURPOSE				: Initialize funtion for call fwding application.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitCallForwarding(void)
{
	/* Set hilite handlers for the individual sub menu items of call forwarding */
	SetHiliteHandler(MENU8237_SCR8093_MNGCALL_MENU_FWD,HighlightCALLSETCallFwd );
	SetHiliteHandler(MENU8238_MNGCALL_CALFWD_VOICECALL,HighlightCALLSETCallFwdVoice);
	SetHiliteHandler(MENU8238_MNGCALL_CALFWD_DATACALL,HighlightCALLSETCallFwdData);
	SetHiliteHandler(MENU8239_CALFWD_VOICECALL_UNREACHABLE,HighlightCALLSETCallFwdUnreach);
	SetHiliteHandler(MENU8239_CALFWD_VOICECALL_NOANSWER,HighlightCALLSETCallFwdNoAns);
	SetHiliteHandler(MENU8239_CALFWD_VOICECALL_BUSY,HighlightCALLSETCallFwdBusy);
	SetHiliteHandler(MENU8240_MNGCALL_CALLFWD_FWD_ON,HighlightCALLSETCallFwdOn);
	SetHiliteHandler(MENU8240_MNGCALL_CALLFWD_FWD_OFF,HighlightCALLSETCallFwdOff);
	SetHiliteHandler(MENU8240_MNGCALL_CALLFWD_FWD_QUERY,HighlightCALLSETCallFwdQuery);
	SetHiliteHandler(MENU_CALL_FWD_CANCELALL,HighlightCALLSETCallFwdCancelAll);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwd

  	PURPOSE				: Highlight handler for the call fwding menu option

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallFwd(void)
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(EntryCALLSETForwardMain, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryCALLSETForwardMain,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwdVoice

  	PURPOSE				: Hightlight handler for call fwd voice sub menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallFwdVoice(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSCFType = FORWARD_ALLVOICE;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryCALLSETForwardOptions,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETForwardOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwdData

  	PURPOSE				: highlight hander for call fwd data sub menu item.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallFwdData(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSCFType = FORWARD_DATA;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryCALLSETForwardOptions,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key functions handlers */	
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETForwardOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwdUnreach

  	PURPOSE				: Highlight handler for voice unreach sub menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallFwdUnreach(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSCFType = FORWARD_UNREACHABLE ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryCALLSETForwardOptions,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETForwardOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwdBusy

  	PURPOSE				: Highlight handler for when busy sub menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallFwdBusy(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSCFType = FORWARD_BUSY ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryCALLSETForwardOptions,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETForwardOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwdNoAns

  	PURPOSE				: Highlight handler for call fwd when no answer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallFwdNoAns(void)
{
	/* storing the menu item selected */
	g_callset_cntx_p->SSCFType = FORWARD_NOANSWER ;
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryCALLSETForwardOptions,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryCALLSETForwardOptions, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwdOn

  	PURPOSE				: Highlight handler for call fwd on submenu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallFwdOn(void)
{
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EntryCALLSETForwardNumber,KEY_EVENT_UP);
//micha0529
	SetKeyHandler(EntryCALLSETForwardNumber, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwdOff

  	PURPOSE				: Highlight handler for call fwd off submenu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void HighlightCALLSETCallFwdOff(void)
{
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EnterScrCallFwdOffReq,KEY_EVENT_UP);
//micha0529
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallFwdQuery

  	PURPOSE				: Highlight handler for call fwd on query sub menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallFwdQuery(void)
{
	/* set the left/right soft key functions handlers */	
	SetLeftSoftkeyFunction(EnterScrCallFwdQueryReq,KEY_EVENT_UP);
//micha0529
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETForwardMain

  	PURPOSE				: Entry function for call fwd main screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETForwardMain(void)
{
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */
	U16 nNumofItem;
	U16 nStrItemList[MAX_SUB_MENUS];
	/*U32 maskingByte=0xFFFFFFFF;
	U32 menuItemId=0xFFFFFFFF;*/

	EntryNewScreen(SCR_ID_CALLSET_FORWARD_MAIN, NULL, EntryCALLSETForwardMain, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_FORWARD_MAIN);	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU8237_SCR8093_MNGCALL_MENU_FWD);  
	/* Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU8237_SCR8093_MNGCALL_MENU_FWD);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU8237_SCR8093_MNGCALL_MENU_FWD, nStrItemList);  
	/* Set current parent id*/
	SetParentHandler(MENU8237_SCR8093_MNGCALL_MENU_FWD);
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

 
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
#endif /* 0 */

	/* Display Category15 Screen */
	ShowCategory15Screen(STR_MENU8238_MNGCALL_CALLFWD_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK , IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,nNumofItem, nStrItemList,
						  (U16 *)gIndexIconsImageList,1,0,guiBuffer);	
	/* Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETForwardOptions

  	PURPOSE				: Entry function for call fwd options screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETForwardOptions(void )
{
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */
	U16 nNumofItem;
	U16 nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_CALLSET_FORWARD_OPTIONS, NULL, EntryCALLSETForwardOptions, NULL);

	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_FORWARD_OPTIONS);	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU8240_MNGCALL_CALLFWD_FWD_OPTION);
	/* Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU8240_MNGCALL_CALLFWD_FWD_OPTION	);
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU8240_MNGCALL_CALLFWD_FWD_OPTION	, nStrItemList);
	/* Set current parent id*/
	SetParentHandler(MENU8240_MNGCALL_CALLFWD_FWD_OPTION	);
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	/* Display Category1 Screen */
	ShowCategory15Screen(STR_MENU8240_MNGCALL_CALLFWD_FWD_OPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK , IMG_GLOBAL_BACK,nNumofItem, nStrItemList,
						  (U16 *)gIndexIconsImageList,1,0,guiBuffer);
	/* Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: StarKeyHandler

  	PURPOSE				: Key handler for the astrick key.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void StarKeyHandler(void)
{
	if( pfnUnicodeStrlen( ( S8 * )g_callset_cntx_p->SSCFNum) == 0)
		multiline_inputbox_direct_input( '+');
	return;
}



/*****************************************************************************
* FUNCTION
*  mmi_call_forward_number_empty
* DESCRIPTION
*   LSK handle when number input is empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_call_forward_number_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_SEARCH, 0);
	SetLeftSoftkeyFunction (mmi_phb_call_enter_from_divert, KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*  mmi_call_forward_number_not_empty
* DESCRIPTION
*   LSK handle when number input is not empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_call_forward_number_not_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, 0);
	SetLeftSoftkeyFunction (EnterScrCallFwdOnReq, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: EntryCALLSETForwardNumber

  	PURPOSE				: Entry function for the enter number for call fwd screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETForwardNumber(void)
{
	U8* guiBuffer;

	EntryNewScreen(SCR_ID_CALLSET_FORWARD_NUMBER, NULL, EntryCALLSETForwardNumber, NULL);
	
	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_FORWARD_NUMBER); 
	/* Check the screen is from forward or backward traversal*/
	if( guiBuffer == NULL)
		memset( g_callset_cntx_p->SSCFNum, 0, (MAX_CC_ADDR_LEN * ENCODING_LENGTH) + 2);
	if ( g_callset_cntx_p->SSCFNumFromPHB ) guiBuffer = NULL;
	g_callset_cntx_p->SSCFNumFromPHB = FALSE;

	/*Guojian Add Start For 10107 Ver: GreenStone  on 2008-10-19 14:32 */
	RegisterInputBoxEmptyFunction (mmi_call_forward_number_empty);
	RegisterInputBoxNotEmptyFunction (mmi_call_forward_number_not_empty);
	/*Guojian Add End  For 10107 Ver: GreenStone  on 2008-10-19 14:32 */

	/* to display the screen for taking input for the callfwd number */
	ShowCategory5Screen( STR_CALL_FWD_ENTER_NUM_CAP, MAIN_MENU_TITLE_SETTINGS_ICON,
							   STR_GLOBAL_OK, IMG_GLOBAL_OK,
							   STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							   INPUT_TYPE_PHONE_NUMBER, (PU8)g_callset_cntx_p->SSCFNum, 
							   MAX_CC_ADDR_LEN, guiBuffer);
	/* clear key handler for the pound key */
	ClearKeyHandler( KEY_POUND, KEY_EVENT_DOWN);
	/* set key handler for the astrick key */
	ClearKeyHandler(KEY_STAR, KEY_EVENT_UP); //qiff 08/10/14 add for greenstone bug:10069
	SetKeyHandler( StarKeyHandler,KEY_STAR, KEY_EVENT_DOWN);
	/* set the left and right soft key functionalities */
/*Guojian Del Start For 10107 Ver: GreenStone  on 2008-10-19 14:40 */
	#if 0
	SetLeftSoftkeyFunction(EnterScrCallFwdOnReq,KEY_EVENT_UP);
	#endif
/*Guojian Del End  For 10107 Ver: GreenStone  on 2008-10-19 14:40 */
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/**************************************************************

	FUNCTION NAME		: EnterScrCallFwdOnReq

  	PURPOSE				: Entry function for call fwd screen when on req .

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EnterScrCallFwdOnReq(void)
{
	S8 input_string[MAX_DIGITS_USSD * ENCODING_LENGTH ];
	S8 string_code[50];
	PS8 hash_string = NULL ;
	S8 uni_input_string[MAX_DIGITS_USSD * ENCODING_LENGTH ];
	U8 i;

/*Guojian Del Start For 10107 Ver: GreenStone  on 2008-10-19 14:31 */
	#if 0
	/* check the buffer of the call fwding number */
	if ( pfnUnicodeStrlen( ( S8 * )g_callset_cntx_p->SSCFNum) == 0)
	{
		mmi_phb_call_enter_from_divert();
		return;
	}
	#endif
/*Guojian Del End  For 10107 Ver: GreenStone  on 2008-10-19 14:31 */

	for ( i = 0; i <  pfnUnicodeStrlen( ( S8 * )g_callset_cntx_p->SSCFNum); i++) {
		if ( (g_callset_cntx_p->SSCFNum[i*2] == 'p') ||  (g_callset_cntx_p->SSCFNum[i*2] == 'w')) {
			DisplayPopup(  (PU8) GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
			return;
		}
	}
	
	/* format the string for the ss string handling */
	memset(input_string,0, (MAX_DIGITS_USSD * ENCODING_LENGTH));
	memset(string_code,0,50);
	hash_string = (PS8) GetString(STR_HASH_VALUE);
	
	switch(g_callset_cntx_p->SSCFType)
	{
	/* if the menu item is selected is voice call fwd when un reachable */
	case FORWARD_UNREACHABLE:
		strcpy((S8*) string_code, "**62*");	
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_code);
		pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSCFNum);
		pfnUnicodeStrcat((PS8) input_string,(PS8) hash_string);
		break;
	/* if the menu item is selected is voice call fwd when no answer */
	case FORWARD_NOANSWER :
		EntryCALLSETForwardNoAnsTime();
		return;
	/* if the menu item is selected is voice call fwd when busy */
	case FORWARD_BUSY :
		strcpy((S8*) string_code, "**67*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_code);
		pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSCFNum);
		pfnUnicodeStrcat((PS8) input_string,(PS8) hash_string);
		break;
	/* if the menu item is selected all voice call fwd */
	case FORWARD_ALLVOICE :
		strcpy((S8*) string_code, "**21*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_code);
		pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSCFNum);
		strcpy((S8*) string_code, "*11");
		AnsiiToUnicodeString((PS8) uni_input_string,(PS8)string_code);
		pfnUnicodeStrcat((PS8) input_string,(PS8) uni_input_string);
		pfnUnicodeStrcat((PS8) input_string,(PS8) hash_string);
		break;
	/* if the menu item is selected is data call fwd */
	 case  FORWARD_DATA:
		strcpy((S8*) string_code, "**21*");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_code);
		pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSCFNum);
		strcpy((S8*) string_code, "*20");
		AnsiiToUnicodeString((PS8) uni_input_string,(PS8)string_code);
		pfnUnicodeStrcat((PS8) input_string,(PS8) uni_input_string);
		pfnUnicodeStrcat((PS8) input_string,(PS8) hash_string);
		break;
	}
	/* set protocol event handler for the call fwd response */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETForwardRsp, PRT_MMI_SS_CALL_FORWARD_RSP);
#else
	SetProtocolEventHandler(CALLSETForwardRsp, PRT_MMI_SS_CALL_FORWARD_RSP);
#endif
	/* make a corresponding ss string for call fwd option */
	
	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif
	
	MakeCall((PS8) input_string);
	return;
}
/**************************************************************

	FUNCTION NAME		: EnterScrCallFwdOffReq

  	PURPOSE				: Entry function for call fwd screen when off req .

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EnterScrCallFwdOffReq(void)
{
	S8 input_string[MAX_DIGITS_USSD * ENCODING_LENGTH];
	S8 string_out[20];
//micha1229
//	PS8 hash_string = NULL ;
	/* format the string for the ss string handling */	
	memset(input_string,0,( MAX_DIGITS_USSD * ENCODING_LENGTH));
	memset(string_out,0,20);
//	hash_string = (PS8) GetString(STR_HASH_VALUE);
	switch(g_callset_cntx_p->SSCFType)
	{
	/* if the menu item is selected is voice call fwd when un reachable */
	case FORWARD_UNREACHABLE:
		strcpy((S8*) string_out, "##62#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* if the menu item is selected is voice call fwd when no answer */
	case FORWARD_NOANSWER :
		strcpy((S8*) string_out, "##61#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* if the menu item is selected is voice call fwd when busy */
	case FORWARD_BUSY :
		strcpy((S8*) string_out, "##67#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* if the menu item is selected all voice call fwd */
	case FORWARD_ALLVOICE :
		strcpy((S8*) string_out, "##21**11#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* if the menu item is selected is cancel all fwd */
	case FORWARD_CANCEL_ALL :
		strcpy((S8*) string_out, "##002#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* if the menu item is selected is data call fwd */
	case  FORWARD_DATA:
		strcpy((S8*) string_out, "##21**20#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	}
	/* set protocol event handler for the call fwd response */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETForwardRsp, PRT_MMI_SS_CALL_FORWARD_RSP);
#else
	SetProtocolEventHandler(CALLSETForwardRsp, PRT_MMI_SS_CALL_FORWARD_RSP);
#endif

	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif
	
	/* make a corresponding ss string for call fwd option */
	MakeCall((PS8) input_string);
	return;
}
/**************************************************************

	FUNCTION NAME		: EnterScrCallFwdQueryReq

  	PURPOSE				: Entry function for call fwd screen qurey request.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EnterScrCallFwdQueryReq(void)
{
	S8 input_string[MAX_DIGITS_USSD * ENCODING_LENGTH];
	S8 string_out[20];
//micha1229
//	PS8 hash_string = NULL ;
	
	/* format the string for the ss string handling */	
	memset(input_string,0, (MAX_DIGITS_USSD * ENCODING_LENGTH));
	memset(string_out,0,20);
//	hash_string = (PS8) GetString(STR_HASH_VALUE);
	switch(g_callset_cntx_p->SSCFType)
	{
	/* if the menu item is selected is voice call fwd when un reachable */
	case FORWARD_UNREACHABLE:
		strcpy((S8*) string_out, "*#62#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* if the menu item is selected is voice call fwd when no answer */
	case FORWARD_NOANSWER :
		strcpy((S8*) string_out, "*#61#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* if the menu item is selected is voice call fwd when busy */
	case FORWARD_BUSY :
		strcpy((S8*) string_out, "*#67#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* if the menu item is selected all voice call fwd */
	case FORWARD_ALLVOICE :
		strcpy((S8*) string_out, "*#21**11#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	/* if the menu item is selected is data call fwd */
	case FORWARD_DATA:
		strcpy((S8*) string_out, "*#21**20#");
		AnsiiToUnicodeString((PS8) input_string,(PS8)string_out);		
		break;
	}
	/* set protocol event handler for the call fwd response */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETForwardRsp, PRT_MMI_SS_CALL_FORWARD_RSP);
#else
	SetProtocolEventHandler(CALLSETForwardRsp, PRT_MMI_SS_CALL_FORWARD_RSP);
#endif

	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif

	/* make a corresponding ss string for call fwd option */
	MakeCall((PS8) input_string);
	return;
}
/**************************************************************

	FUNCTION NAME		: CALLSETForwardRsp

  	PURPOSE				: response handler for the call fwd query request.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void  aud_speech_set_mode(kal_bool on);

/*****************************************************************************
 * FUNCTION
 *  CALLSETForwardRsp
 * DESCRIPTION
 *  response handler for the call fwd query request.
 * NA
 *  
 * PARAMETERS
 *  info        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void CALLSETForwardRsp(void *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 short_string[45], TempShortString[90];
    S8 TempBuff[250];
    U8 bs_index;
    mmi_ss_call_forward_rsp_struct *pMsgFwdQueryrsp = (mmi_ss_call_forward_rsp_struct*) info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(g_callset_cntx_p->SSDisplayBuffer, 0, MAX_DISP_UCS2);

    DeleteScreenIfPresent(SCR_ID_CALLSET_FORWARD_OPTIONS);
    DeleteScreenIfPresent(SCR_ID_CALLSET_FORWARD_NUMBER);
    DeleteScreenIfPresent(SCR_ID_CALLSET_FORWARD_NO_ANS_TIME);

    /* check the status of the response result */
    if (pMsgFwdQueryrsp->result.flag == L4C_OK)
    {
        /* if the response is success check the type */
        /* format the result string */
        switch (pMsgFwdQueryrsp->type)
        {
                /* Call Forward Unconditional */
            case L4_CFU:
                pfnUnicodeStrcpy((PS8) g_callset_cntx_p->SSDisplayBuffer, (PS8) GetString(STR_L4_CFU));
                break;
                /* Call Forward When Busy */
            case L4_CFB:
                pfnUnicodeStrcpy((PS8) g_callset_cntx_p->SSDisplayBuffer, (PS8) GetString(STR_L4_CFB));
                break;
                /* Call Forward When Not Reply */
            case L4_CFNRY:
                pfnUnicodeStrcpy((PS8) g_callset_cntx_p->SSDisplayBuffer, (PS8) GetString(STR_L4_CFNRY));
                break;
                /* Call Forward When Not Reachable */
            case L4_CFNRC:
                pfnUnicodeStrcpy((PS8) g_callset_cntx_p->SSDisplayBuffer, (PS8) GetString(STR_L4_CFNRC));
                break;
                /* All Call Forward */
            case L4_CFA:
                pfnUnicodeStrcpy((PS8) g_callset_cntx_p->SSDisplayBuffer, (PS8) GetString(STR_L4_CFA));
                break;
                /* All Conditional Call Forward  */
            case L4_CFC:
                pfnUnicodeStrcpy((PS8) g_callset_cntx_p->SSDisplayBuffer, (PS8) GetString(STR_L4_CFC));
                break;
                /* Unknown Service Type */
            default:
                pfnUnicodeStrcpy((PS8) g_callset_cntx_p->SSDisplayBuffer, (PS8) GetString(STR_FWD_UNKNOWN_SERVICE));
                break;
        }
        /* format the result string with active/deactive tags. */
        if (pMsgFwdQueryrsp->count == 0)
        {
            pfnUnicodeStrcat((PS8) g_callset_cntx_p->SSDisplayBuffer, (PS8) GetString(STR_DEACTIVATED));
        }
        pfnUnicodeStrcat((S8*) g_callset_cntx_p->SSDisplayBuffer, (S8*)" \0");

        for (bs_index = 0; bs_index < pMsgFwdQueryrsp->count; bs_index++)
        {
            memset(TempBuff, 0, sizeof(TempBuff));
            if (pMsgFwdQueryrsp->list[bs_index].ss_status & SS_ABIT)
            {
                pfnUnicodeStrcat((PS8) TempBuff, (PS8) GetString(STR_ACTIVATED));
            }
            else if (pMsgFwdQueryrsp->list[bs_index].ss_status & SS_PBIT)
            {
                pfnUnicodeStrcat((PS8) TempBuff, (PS8) GetString(STR_DEACTIVATED));
            }
            else
            {
                pfnUnicodeStrcat((PS8) TempBuff, (PS8) GetString(STR_NOT_PROVIDED));
            }
            if (pMsgFwdQueryrsp->list[bs_index].no_reply_timer != 0)
            {
                sprintf(short_string, "%d ", (U8) pMsgFwdQueryrsp->list[bs_index].no_reply_timer);
                AnsiiToUnicodeString((PS8) TempShortString, (PS8) short_string);
                pfnUnicodeStrcat((S8*) TempBuff, (S8*) TempShortString);
                pfnUnicodeStrcat((PS8) TempBuff, (PS8) GetString(STR_SECONDS));
            }

            AppendBScodeString(pMsgFwdQueryrsp->list[bs_index].bs_code, TempBuff);
            if (strlen(pMsgFwdQueryrsp->list[bs_index].address) != 0)
            {
                memset(short_string, 0, sizeof(short_string));
                memset(TempShortString, 0, sizeof(TempShortString));
                strncpy((PS8) short_string, (PS8) pMsgFwdQueryrsp->list[bs_index].address, sizeof(short_string) - 5);
                strcat((PS8) short_string, " ");
                AnsiiToUnicodeString((PS8) TempShortString, (PS8) short_string);
                pfnUnicodeStrcat((S8*) TempBuff, (S8*) TempShortString);
            }

#ifdef __MMI_SHOW_SS_TWO_LINES__
            if (!IsResetCSPGroupService(CSP_CPHS_TELESERVICES_SERVICE_GROUP, ALS))
            {
                if ((pMsgFwdQueryrsp->list[bs_index].bs_code == L4_FACSIMILE_SERVICES) ||
                    (pMsgFwdQueryrsp->list[bs_index].bs_code == L4_ALL_BEARER_SERVICES) ||
                    (pMsgFwdQueryrsp->list[bs_index].bs_code == L4_ALL_ASYNC_SERVICES) ||
                    (pMsgFwdQueryrsp->list[bs_index].bs_code == L4_ALL_SYNC_SERVICES) ||
                    (pMsgFwdQueryrsp->list[bs_index].bs_code == L4_ALL_DATA_CIRCUIT_SYNC) ||
                    (pMsgFwdQueryrsp->list[bs_index].bs_code == L4_ALL_DATA_CIRCUIT_ASYNC))
                {
                    pfnUnicodeStrcat((PS8) TempBuff, (PS8) GetString(STR_BOTH_LINES));
                }
            }
#endif            
            
            if ((pfnUnicodeStrlen((PS8) TempBuff) + pfnUnicodeStrlen((PS8) g_callset_cntx_p->SSDisplayBuffer))
                < MAX_DISP_UCS2 / ENCODING_LENGTH)
            {
                pfnUnicodeStrcat((S8*) g_callset_cntx_p->SSDisplayBuffer, (S8*) TempBuff);
            }
        }
        /* call the result screen function to display the result string */
        /* micha0322 */
        DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
        EntryCALLSETGeneralSSResut();
    }
    else
    {
        /* in case of response failure */
    #ifdef __MMI_SS_SHOW_CAUSE__
        ShowCauseString(pMsgFwdQueryrsp->result.cause, g_callset_cntx_p->SSDisplayBuffer);
        /* micha0322 */
        DeleteScreenIfPresent(SCR_ID_CALLSET_GENERAL_SS_RESULT);
        EntryCALLSETGeneralSSResut();
    #else /* __MMI_SS_SHOW_CAUSE__ */ 
        /* display the not done popup */
        DisplayPopup((PU8) GetString(STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
        /* micha0915 */
        RemoveSSReqScr();
    #endif /* __MMI_SS_SHOW_CAUSE__ */ 
    }

	aud_speech_set_mode(KAL_FALSE); //chenhe,070808. for callforward,CM_KB_OUTGOINGCALL is been sent,so have to do so here

    return;
}
		
/**************************************************************

	FUNCTION NAME		: ActivateVNoAnswer

  	PURPOSE				: Sending req for the call fwd when no answer case

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ActivateVNoAnswer( void )
{
	S8 input_string[MAX_DIGITS_USSD * ENCODING_LENGTH ];
	S8 string_code[50];
	PS8 hash_string = NULL ;
	S8 uni_input_string[MAX_DIGITS_USSD * ENCODING_LENGTH ];

	/* format the string for the ss string handling */	
	memset(input_string,0, (MAX_DIGITS_USSD * ENCODING_LENGTH));
	memset(string_code,0,50);
	hash_string = (PS8) GetString(STR_HASH_VALUE);

	strcpy((S8*) string_code, "**61*");
	AnsiiToUnicodeString((PS8) input_string,(PS8)string_code);
	pfnUnicodeStrcat((PS8) input_string,(PS8) g_callset_cntx_p->SSCFNum);
	strcpy((PS8) string_code,"**");
	AnsiiToUnicodeString((PS8) uni_input_string,(PS8)string_code);
	pfnUnicodeStrcat((PS8) input_string,(PS8) uni_input_string);

	sprintf( (PS8) string_code, "%d", 5 * (g_callset_cntx_p->SSCFNoAnsTimeIndex+1));
	AnsiiToUnicodeString((PS8) uni_input_string,(PS8)string_code);
	pfnUnicodeStrcat((PS8) input_string,(PS8) uni_input_string);
	pfnUnicodeStrcat((PS8) input_string,(PS8) hash_string);
	/* set protocol event handler for the call fwd response */
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETForwardRsp, PRT_MMI_SS_CALL_FORWARD_RSP);
#else
	SetProtocolEventHandler(CALLSETForwardRsp, PRT_MMI_SS_CALL_FORWARD_RSP);
#endif

	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, g_callset_cntx_p->nSimId);
	#endif

	/* make a corresponding ss string for call fwd option */
	MakeCall((PS8) input_string);
	return;	
}
/**************************************************************

	FUNCTION NAME		: GetCurrHandler

  	PURPOSE				: Getting Current highlighted item 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GetCurrHandler( S32 item_index )
{
	g_callset_cntx_p->SSCFNoAnsTimeIndex = (U8)item_index;
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryCALLSETForwardNoAnsTime

  	PURPOSE				: entry function for no answer 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETForwardNoAnsTime(void)
{

	U16			nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U8*			guiBuffer;       /* Buffer holding history data */
	U16			nNumofItem;

	EntryNewScreen(SCR_ID_CALLSET_FORWARD_NO_ANS_TIME, NULL, EntryCALLSETForwardNoAnsTime, NULL);
	
	/* Get current screen info into gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR_ID_CALLSET_FORWARD_NO_ANS_TIME );	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild( MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS );
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds( MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS, nStrItemList );
	/* Set current parent id*/
	SetParentHandler( MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS );
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler( GetCurrHandler);
	/* display the corresponding screen*/
	ShowCategory15Screen ( SCR_CALL_FWD_ENTER_NO_ANSWER_TIME_CAP, 0, STR_GLOBAL_OK,
	                      IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                      nNumofItem, nStrItemList, (U16 *) gIndexIconsImageList,
	                      1, 0, guiBuffer );	
	/* Register function with left/right softkey */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	SetLeftSoftkeyFunction( ActivateVNoAnswer, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: SendPhbEntryForDivert

  	PURPOSE				: send phone number for diverting to the number

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SendPhbEntryForDivert(U8 name[], U8 number[])
{
	g_callset_cntx_p->SSCFNumFromPHB = TRUE;
	pfnUnicodeStrcpy ((PS8)g_callset_cntx_p->SSCFNum, (PS8)number);
	GoBackHistory ();
	return;
}

