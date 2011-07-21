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
 * CallsCost.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for .
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: CallsCost.c

  	PURPOSE		: Call Setting Application

 

	 

	DATE		: 10/22/2003

**************************************************************/

/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-06-20, Li Tao, fix bug, before enter the "Call Cost" menu, the sim ready is must.
 ************************************************************************/
 
#ifndef _CALLS_COST_C
#define _CALLS_COST_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "mmi_trace.h"
 
//... Add More MMI header
#include "debuginitdef.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
//#include "wgui_categories.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_inputs.h"
#include "commonscreens.h"
#include "custmenures.h"
#include "unicodexdcl.h"
#include "queuegprot.h"
#include "protocolevents.h"
#include "idleappprot.h"
#include "wgui_status_icons.h"

#include "settingdefs.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "phonebooktypes.h"
#include "alarmframeworkprot.h"

#include "callhistoryenum.h"
#include "callsdefs.h"
#include "callsstruct.h"
#include "callscost.h"
//
#include "mmi_msg_struct.h"

//micha0420
#include "settingsgdcl.h"
#include "satdefs.h"		//2007-06-20, li tao, for "sat_wait_string_id"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_calllog.h"
#endif
#include "settinggprots.h"
#undef __NEWSIMULATOR 

U8 gCallCost = 0;
U16 SetTitleStrings[2];
extern InlineItem wgui_inline_items[];


/**************************************************************

	FUNCTION NAME		: InitCallCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitCallCost( void )
{
	SetHiliteHandler( MENU_CALL_HISTORY_CALLCOST, HighlightCHISTCallCost );
	SetHiliteHandler( MENU_CALL_COST_LASTCALL, HighlightCHISTLastCost );
	SetHiliteHandler( MENU_CALL_COST_ALLCOST, HighlightCHISTAllCost );
	SetHiliteHandler( MENU_CALL_COST_PRICEUNIT, HighlightCHISTPriceAndUnit );
	SetHiliteHandler( MENU_CALL_COST_RESETCOUNTER, HighlightCHISTResetCost );
	SetHiliteHandler( MENU_CALL_COST_MAXCOUNT, HighlightCHISTMaxCost );

	SetHintHandler( MENU_CALL_COST_LASTCALL, HintCHISTLastCost );
	SetHintHandler( MENU_CALL_COST_ALLCOST, HintCHISTAllCost );
	SetHintHandler( MENU_CALL_COST_PRICEUNIT, HintCHISTPriceAndUnit );
	SetHintHandler( MENU_CALL_COST_MAXCOUNT, HintCHISTMaxCost );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTCallCostItem(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTCallCostItem( S32 index )
{
	chis_p->currCostIndex = index;

	ExecuteCurrHiliteHandler(index);
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTCallCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTCallCost( void )
{
	gCallCost = 0;
	chis_p->currCostIndex = 0;

	SetKeyHandler( CHISTGetCallCost,KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetLeftSoftkeyFunction( CHISTGetCallCost, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTLastCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTLastCost( void )
{
   gCallCost = 0;
   chis_p->currCostIndex = (U8)GetCurrHiliteID();
	chis_p->currCostItem = MENU_CALL_COST_LASTCALL;

	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( 0, 0 );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	ClearOneKeyAllHandler(KEY_RIGHT_ARROW);

	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTAllCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTAllCost( void )
{
   gCallCost = 0;
   chis_p->currCostIndex = (U8)GetCurrHiliteID();
	chis_p->currCostItem = MENU_CALL_COST_ALLCOST;

	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( 0, 0 );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );
   SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	ClearOneKeyAllHandler(KEY_RIGHT_ARROW);

	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTResetCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTResetCost( void )
{
	gCallCost = 1;
	chis_p->currCostIndex = (U8)GetCurrHiliteID();
	chis_p->currCostItem = MENU_CALL_COST_RESETCOUNTER;

	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );

//micha0420
	g_SecuritySetupContext.VerifyPassSuccessCallBack = RspPin2ForCallCostSuccess;
	#ifdef __MMI_MULTI_SIM__
	SECSETSetSIMIdforPIN(chis_p->nSimId);
	#endif
	SetLeftSoftkeyFunction( SECSETGetPIN2StatusReq, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTMaxCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTMaxCost( void )
{
	gCallCost = 1;
	chis_p->currCostIndex = (U8)GetCurrHiliteID();
	chis_p->currCostItem = MENU_CALL_COST_MAXCOUNT;

	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_CALL_COST_PRICEANDUNIT_VIEW_LSK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );

//micha0420
	g_SecuritySetupContext.VerifyPassSuccessCallBack = RspPin2ForCallCostSuccess;
	#ifdef __MMI_MULTI_SIM__
	SECSETSetSIMIdforPIN(chis_p->nSimId);
	#endif
	SetLeftSoftkeyFunction( SECSETGetPIN2StatusReq,KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightCHISTPriceAndUnit(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCHISTPriceAndUnit( void )
{
	gCallCost = 1;
	chis_p->currCostIndex = (U8)GetCurrHiliteID();
	chis_p->currCostItem = MENU_CALL_COST_PRICEUNIT;

	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey( STR_CALL_COST_PRICEANDUNIT_VIEW_LSK, IMG_GLOBAL_OK );
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey( STR_GLOBAL_BACK, IMG_GLOBAL_BACK );

//micha0420
	g_SecuritySetupContext.VerifyPassSuccessCallBack = RspPin2ForCallCostSuccess;
	#ifdef __MMI_MULTI_SIM__
	SECSETSetSIMIdforPIN(chis_p->nSimId);
	#endif
	SetLeftSoftkeyFunction(SECSETGetPIN2StatusReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

	return;
}

/**************************************************************

	FUNCTION NAME		: HighlightPriceAndUnitHandler(void)

  	PURPOSE				: set inline editor LSK handler

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightPriceAndUnitHandler(S32 index)
{
	ChangeTitleString((PU8)GetString(SetTitleStrings[index]));
	draw_title();
	
	SetLeftSoftkeyFunction(CHISTValidatePriceUnit,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HintCHISTLastCost()

  	PURPOSE				: hint handler to get last call cost

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTLastCost(U16 index)
{
	#if 0
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	if ((chis_p->lastCallCost % 1000) != 0)
	{
		sprintf((S8 *)value, "%u.%u",
		        chis_p->lastCallCost/1000,
		        chis_p->lastCallCost%1000);
	}
	else
	{
		sprintf((S8 *) value, "%u", chis_p->lastCallCost/1000);
	}

	AnsiiToUnicodeString((S8 *)hintData[index], (S8 *)value);
	#else
	U8 value[MAX_SUB_MENU_HINT_SIZE];
// add by chenq for bug 6148 20080810 B
  memset(value,0,sizeof(value));
  value[0] = '0';
#if 0
	#ifdef _MMI_OPERATOR_
	strcpy(value, chis_p->lastCallCost);
	#else
	sprintf((S8 *)value, "%.2f",0 /*chis_p->lastCallCost*/);
	#endif
#endif
// add by chenq for bug 6148 20080810 E
	AnsiiToUnicodeString((S8 *)hintData[index], (S8 *)value);
	#endif
}

/**************************************************************

	FUNCTION NAME		: HintCHISTAllCost()

  	PURPOSE				: hint handler to get all call cost

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTAllCost(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

#ifdef _MMI_OPERATOR_
	strcpy(value, chis_p->allCallCost);
#else
	sprintf((S8 *)value, "%.2f", chis_p->allCallCost);
#endif

	AnsiiToUnicodeString((S8 *)hintData[index], (S8 *)value);
}

/**************************************************************

	FUNCTION NAME		: HintCHISTMaxCost()

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTMaxCost(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	sprintf( (S8 *)value, "%u", chis_p->maxCallCost );

	AnsiiToUnicodeString((S8 *)hintData[index], (S8 *)value);
}

/**************************************************************

	FUNCTION NAME		: HintCHISTPriceAndUnit(void)

  	PURPOSE				: hint handler to get price and unit

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintCHISTPriceAndUnit(U16 index)
{
   #if 0 // zrx del 20060612
	U8 value[MAX_SUB_MENU_HINT_SIZE];
	U32 ppu = (U32) chis_p->ppu;

	if ( chis_p->ppu - ppu == 0 )
		sprintf((S8 *)value,"%u\0", (U32)chis_p->ppu);
	else
		sprintf((S8 *)value,"%u.%03d\0", ppu,
		        (U32)((chis_p->ppu - ppu) * 1000));

	if( chis_p->currencyStr[0] != 0 )
	{
	   strcat((S8 *)value, (S8 *)chis_p->currencyStr);
	}

	AnsiiToUnicodeString((S8 *)hintData[index], (S8 *)value);
	#else	
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	strcpy(value, chis_p->PPUStr);
	if (pfnUnicodeStrcmp(chis_p->currencyStr, "0") != 0)
	{
	   strcat((S8*) value, " ");
	   strcat((S8*) value, (S8*) chis_p->currencyStr);
	}
	
	AnsiiToUnicodeString((S8 *)hintData[index], (S8 *)value);
	
	#endif
}

/**************************************************************

	FUNCTION NAME		: CHISTGetCallCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
static BOOL bNeedShowScreen = FALSE; //判断是否进入函数EntryCHISTCallCost();
void CHISTGetCallCost( void )
{
	AlmDisableExpiryHandler();
	ReqGetAllCallCost();
	ReqGetLastCallCost();
	ReqGetPriceAndUnit();

	bNeedShowScreen = TRUE;
	ReqGetMaxCallCost();
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTCallCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTCallCost( void )
{
	U16	nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16	nNumofItem;      /* Stores no of children in the submenu*/
	//U16	nDispAttribute;  /* Stores display attribue */
	U8*	guiBuffer;       /* Buffer holding history data */
	U8*	displayBuffer[MAX_SUB_MENUS];

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_CALL_COST_MAIN, NULL,
	               CHISTGetCallCost, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR_CALL_COST_MAIN );
	/* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild( MENU_CALL_HISTORY_CALLCOST );
	/* 4. Get attribute of menu to be displayed */
	//nDispAttribute = GetDispAttributeOfItem( MENU_CALL_HISTORY_CALLCOST );
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds( MENU_CALL_HISTORY_CALLCOST, nStrItemList );
	/* 6 Set current parent id*/
	SetParentHandler( MENU_CALL_HISTORY_CALLCOST );
	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler( HighlightCHISTCallCostItem );
	/* 8 Display Category1 Screen */
	ConstructHintsList(MENU_CALL_HISTORY_CALLCOST, displayBuffer);

	ShowCategory52Screen( STR_MENU_CALL_COST_MAIN, IMG_SCR_CALL_COST_MAIN_CAP,
	                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
	                      IMG_GLOBAL_BACK, nNumofItem, nStrItemList,
	                      (U16 *)gIndexIconsImageList, displayBuffer, 0,
	                      chis_p->currCostIndex, guiBuffer );

	/* 9.Register function with right softkey */
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN );	
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	/* 10. Register Exit handler */

	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTResetCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTResetCost( void )
{
	MSG_MMI_CC_RESET_ACM_REQ_STRUCT* pMsgIndexReq	= NULL ;
	MYQUEUE Message;

	PRINT_INFORMATION( ( "File: [%s]  Line: [%d] <<LoadAllFdlist.>\n",
	                     __FILE__, __LINE__ ) );

	pMsgIndexReq = (MSG_MMI_CC_RESET_ACM_REQ_STRUCT *)
	                  OslConstructDataPtr(
	                     sizeof( MSG_MMI_CC_RESET_ACM_REQ_STRUCT ) );

	memset( (S8 *)pMsgIndexReq->pin, 0, MAX_SIM_PASSWD_LEN );

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_CC_RESET_ACM_REQ;
	// Message.oslSapId = 0;
	Message.oslDataPtr = (oslParaType *)pMsgIndexReq;
	Message.oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspResetAllCallCost, PRT_MMI_CC_RESET_ACM_RES );
#else
	SetProtocolEventHandler( RspResetAllCallCost, PRT_MMI_CC_RESET_ACM_RES );
#endif
	OslMsgSendExtQueue( &Message );
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTSetMaxCost(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTSetMaxCost( void )
{
	U8*	guiBuffer;

	PRINT_INFORMATION( ( "File: [%s]  Line: [%d] <<EntryScr104PbookNumberEdit.>\n", __FILE__, __LINE__ ) );

	EntryNewScreen(SCR_MAX_COST_SET, ExitCHISTSetMaxCost,
	               NULL, NULL);

	guiBuffer = GetCurrGuiBuffer( SCR_MAX_COST_SET );

	ShowCategory5Screen( STR_MAX_COST_SET_CAPTION,
	                     IMG_SCR_MAX_COST_GET_CAPTION, STR_GLOBAL_OK,
	                     IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                     INPUT_TYPE_NUMERIC,
	                     (PU8)chis_p->newMaxCostStr, 8,
	                     guiBuffer );

	SetLeftSoftkeyFunction( CHISTValidateMaxCost, KEY_EVENT_UP );
	SetCategory5RightSoftkeyFunction( CHISTGoBackCallCostHistory, KEY_EVENT_UP );
	return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTSetMaxCost(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTSetMaxCost( void )
{
	history_t	Scr; /* Variable to hold the history data*/
	S16		nHistory	= 0;

	Scr.scrnID = SCR_MAX_COST_SET;
	Scr.entryFuncPtr = EntryCHISTSetMaxCost;
	pfnUnicodeStrcpy( ( S8 * ) Scr.inputBuffer, ( S8 * ) &nHistory );
	GetCategoryHistory( Scr.guiBuffer );
	AddHistory( Scr );
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTSetPriceUnit(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTSetPriceUnit( void )
{
	U8* guiBuffer;		   /* Buffer holding history data */
	U8* inputBuffer;     /* added for inline edit history */
	U16 inputBufferSize; /* added for inline edit history */
	U16 DetailPriceUnitImageList[3];

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_CALL_COST_PRICEANDUNIT_SET, ExitCHISTSetPriceUnit,
	               NULL, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR_CALL_COST_PRICEANDUNIT_SET );

	AnsiiToUnicodeString((S8 *)chis_p->newCurrencyStr,
	                     (S8 *)chis_p->currencyStr);
	AnsiiToUnicodeString((S8 *)chis_p->newPPUStr,
	                     (S8 *)chis_p->PPUStr);

	/* 3 Set current parent id*/
	InitializeCategory57Screen();
	SetInlineItemActivation(&wgui_inline_items[0],
	                        INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[0],
	                      (PU8)chis_p->newPPUStr, 20,
	                      INPUT_TYPE_DECIMAL_NUMERIC);
	DisableInlineItemHighlight(&wgui_inline_items[0]);
	SetTitleStrings[0] = STR_MENU_CALL_COST_PRICEANDUNIT_SET_PRICE;
	DetailPriceUnitImageList[0] = IMG_GLOBAL_L1;
	SetInlineItemActivation(&wgui_inline_items[1],
	                        INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[1],
	                      (PU8)chis_p->newCurrencyStr, 4,
	                      INPUT_TYPE_ALPHANUMERIC_UPPERCASE | INPUT_TYPE_USE_ONLY_ENGLISH_MODES );
	DisableInlineItemHighlight(&wgui_inline_items[1]);
	SetTitleStrings[1] = STR_MENU_CALL_COST_PRICEANDUNIT_SET_UNIT;
	DetailPriceUnitImageList[1] = IMG_GLOBAL_L2;

   /* added for inline edit history */
	inputBuffer = GetCurrNInputBuffer ( SCR_CALL_COST_PRICEANDUNIT_SET,
	                                    &inputBufferSize );

	RegisterHighlightHandler(HighlightPriceAndUnitHandler);

   /* added for inline edit history */
	if((guiBuffer != NULL) && (guiBuffer[0]!=0))
	{
	   /* sets the data */
	   SetCategory57Data(wgui_inline_items, 2, inputBuffer);
	}
         
	DisableCategory57ScreenDone();
	/*CSD end*/

	ShowCategory57Screen(STR_MENU_SCR_CALL_COST_PRICEANDUNIT_SET,
	                     IMG_SCR_CALL_COST_PRICEANDUNIT_SET_CAPTION,
	                     STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
	                     IMG_GLOBAL_BACK, 2,
	                     DetailPriceUnitImageList,wgui_inline_items,
	                     0, guiBuffer );

	SetCategory57RightSoftkeyFunctions( CHISTValidatePriceUnit,
	                                    CHISTGoBackCallCostHistory);

	return;
}

/**************************************************************

	FUNCTION NAME		: ExitCHISTSetPriceUnit(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCHISTSetPriceUnit( void )
{
	history_t currHistory;
	U16 inputBufferSize;                //added for inline edit history
	S16 nHistory	= 0;

	CloseCategory57Screen();
	currHistory.scrnID = SCR_CALL_COST_PRICEANDUNIT_SET;
	currHistory.entryFuncPtr = EntryCHISTSetPriceUnit;
	pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
	GetCategoryHistory( currHistory.guiBuffer );

   //added for inline edit history
	inputBufferSize = (U16)GetCategory57DataSize( );
	GetCategory57Data ((U8 *) currHistory.inputBuffer );
	AddNHistory(currHistory, inputBufferSize);
	return;
}

/**************************************************************

	FUNCTION NAME		: EntryCHISTSetPriceUnitConfirm(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTSetPriceUnitConfirm( void )
{
	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
		                   STR_GLOBAL_NO,IMG_GLOBAL_NO,
		                   get_string(STR_GLOBAL_SAVE),
		                   IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(ReqSetPriceAndUnit,KEY_EVENT_UP);
	SetRightSoftkeyFunction(CHISTGoBackCallCostHistory,KEY_EVENT_UP);
   return;
}

/**************************************************************

	FUNCTION NAME		: CHISTValidateMaxCost(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTValidateMaxCost( void )
{
	if ( pfnUnicodeStrlen( (S8 *)chis_p->newMaxCostStr ) > 0 )
		ReqSetMaxCallCost();

	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTValidatePriceUnit(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTValidatePriceUnit( void )
{
	U8 count=0, num=0;
	U8	temp_ppu[40];

	CloseCategory57Screen();

	UnicodeToAnsii( (S8 *)temp_ppu, (S8 *)chis_p->newPPUStr );

	while (temp_ppu[count] != 0)
	{
	   if (temp_ppu[count] <= '9' && temp_ppu[count] >= '0')
	      num++;
		if (temp_ppu[count++] == '.')
			break;
	}
	// overflow
	if ((count > 9 && num > 9) || count == 0)
	{
		DisplayPopup((PU8)GetString(STR_ALL_CALL_COSTS_ERROR_MSG) ,
		             IMG_GLOBAL_WARNING, 0,
		             CS_NOTIFYDURATION,WARNING_TONE);
		return;
	}

	EntryCHISTSetPriceUnitConfirm();
	return;
}

/**************************************************************

	FUNCTION NAME		: RspPin2ForCallCostSuccess(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspPin2ForCallCostSuccess( void )
{
   if ( chis_p->currCostItem == MENU_CALL_COST_RESETCOUNTER )
	{
	   EntryCHISTResetCost();
	}
	if ( chis_p->currCostItem == MENU_CALL_COST_MAXCOUNT )
	{
		DisplayPopup((PU8)GetString(STR_SETTING_CODE_VERIFIED) ,
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
		/* add screen to history, after pop up disappear will show the history screen  */
		ExitCHISTSetMaxCost();
		memset( chis_p->newMaxCostStr, 0, 10*ENCODING_LENGTH );
		DeleteUptoScrID(SCR_MAX_COST_SET);
	}
	if ( chis_p->currCostItem == MENU_CALL_COST_PRICEUNIT )
	{
		DisplayPopup((PU8)GetString(STR_SETTING_CODE_VERIFIED) ,
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
		{
			history_t	currHistory;
         /* added for inline edit history */
			U16      inputBufferSize;
			S16		nHistory	= 0;

			currHistory.scrnID = SCR_CALL_COST_PRICEANDUNIT_SET;
			currHistory.entryFuncPtr = EntryCHISTSetPriceUnit;
			pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer,
			                  (S8 *)&nHistory );
			pfnUnicodeStrcpy( (S8 *)currHistory.guiBuffer ,
			                  (S8 *)&nHistory );
			inputBufferSize = 0;     //added for inline edit history
			AddNHistory(currHistory, inputBufferSize);
		}
		DeleteUptoScrID(SCR_CALL_COST_PRICEANDUNIT_SET);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqGetLastCallCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqGetLastCallCost( void )
{
	MYQUEUE	msgStruct;

	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MMI_CHIS_DEST_MOD;
	msgStruct.oslMsgId = PRT_MMI_CC_GET_CCM_REQ;
	// msgStruct.oslSapId = 0;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspGetLastCallCost, PRT_MMI_CC_GET_CCM_RES );
#else
	SetProtocolEventHandler( RspGetLastCallCost, PRT_MMI_CC_GET_CCM_RES );
#endif
	OslMsgSendExtQueue( &msgStruct );
	return;
}

/**************************************************************

	FUNCTION NAME		: RspGetLastCallCost(void *)

  	PURPOSE				:

	INPUT PARAMETERS	: response of acm buffer.

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspGetLastCallCost( void* info )
{
	MSG_MMI_CC_GET_CCM_RSP_STRUCT*	ccm_rsp;

	ccm_rsp = (MSG_MMI_CC_GET_CCM_RSP_STRUCT *) info;
	
   if ( ccm_rsp->result == TRUE )
   {
		#ifdef _MMI_OPERATOR_
		
		if (!(ccm_rsp->ccm[0] == '-' && ccm_rsp->ccm[1] == '1'))
			strcpy(chis_p->lastCallCost, ccm_rsp->ccm);
		
		#else
		
		if ( ccm_rsp->ccm != 0xFFFFFF )
		   chis_p->lastCallCost = ccm_rsp->ccm;
		
		#endif
   }
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqGetAllCallCost(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqGetAllCallCost( void )
{
	MYQUEUE	msgStruct;

	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MMI_CHIS_DEST_MOD;
	msgStruct.oslMsgId = PRT_MMI_CC_GET_ACM_REQ;
	// msgStruct.oslSapId = 0;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspGetAllCallCost, PRT_MMI_CC_GET_ACM_RES );
#else
	SetProtocolEventHandler( RspGetAllCallCost, PRT_MMI_CC_GET_ACM_RES );
#endif
	OslMsgSendExtQueue( &msgStruct );
	return;
}

/**************************************************************

	FUNCTION NAME		: RspGetAllCallCost(void *)

  	PURPOSE				:

	INPUT PARAMETERS	: response of acm buffer.

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspGetAllCallCost( void* info )
{
	MSG_MMI_CC_GET_ACM_RSP_STRUCT  *acm_rsp;

	acm_rsp = (MSG_MMI_CC_GET_ACM_RSP_STRUCT *)info;

	if (acm_rsp->result == TRUE)
	{
		#ifdef _MMI_OPERATOR_
		strcpy(chis_p->allCallCost, acm_rsp->acm);
		#else
		chis_p->allCallCost = acm_rsp->acm;
		#endif
	}
	return;
}


/**************************************************************

	FUNCTION NAME		: ReqGetMaxCallCost(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqGetMaxCallCost( void )
{
	MYQUEUE	Message;
	
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_CC_GET_MAX_ACM_REQ;
	// Message.oslSapId = 0;
	Message.oslDataPtr = NULL ;
	Message.oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspGetMaxCallCost, PRT_MMI_CC_GET_MAX_ACM_RES );
#else
	SetProtocolEventHandler( RspGetMaxCallCost, PRT_MMI_CC_GET_MAX_ACM_RES );
#endif
	OslMsgSendExtQueue( &Message );
	
	return;
}

/**************************************************************

	FUNCTION NAME		: RspGetMaxCallCost(void *info)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspGetMaxCallCost( void* info )
{
	MSG_MMI_CC_GET_MAX_ACM_RSP_STRUCT*	localPtr;
	localPtr = (MSG_MMI_CC_GET_MAX_ACM_RSP_STRUCT *) info;

	if ( localPtr->result == TRUE )
	{
	    chis_p->maxCallCost = localPtr->acm_max;
		
		if (bNeedShowScreen)
		{
			EntryCHISTCallCost();
			AlmEnableExpiryHandler();
		}
		bNeedShowScreen = FALSE;
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqGetPriceAndUnit(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqGetPriceAndUnit( void )
{
	MYQUEUE	Message;

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_CALL_COST_PRICEUNIT_GET_REQ;
	// Message.oslSapId = 0;
	Message.oslDataPtr = NULL ;
	Message.oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspGetPriceAndUnit, PRT_MMI_CALL_COST_PRICEUNIT_GET_RSP );
#else
	SetProtocolEventHandler( RspGetPriceAndUnit, PRT_MMI_CALL_COST_PRICEUNIT_GET_RSP );
#endif
	OslMsgSendExtQueue( &Message );
	return;
}

/**************************************************************

	FUNCTION NAME		: RspGetPriceAndUnit(void *info)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspGetPriceAndUnit( void* info )
{

#ifdef _MMI_OPERATOR_// zrx del 20060612

	MSG_MMI_SMU_GET_PUC_RSP_STRUCT *localPtr;
	//U32 temp_ppu;
	U8 i;

	localPtr = ( MSG_MMI_SMU_GET_PUC_RSP_STRUCT *) info;

	if ( localPtr->result == TRUE )
	{
		//chis_p->ppu = 0;
		memset( chis_p->currencyStr, 0, 4 * ENCODING_LENGTH );
		memset( chis_p->newCurrencyStr, 0, 4 * ENCODING_LENGTH );
		memset( chis_p->PPUStr, 0, 20 * ENCODING_LENGTH );
		memset( chis_p->newPPUStr, 0, 20 * ENCODING_LENGTH );

		for (i = 0; (i < 3) && (localPtr->currency[i] != 0xFF); i++)
			chis_p->currencyStr[i] = localPtr->currency[i];
		//strcpy(chis_p->currencyStr, localPtr->currency);
		AnsiiToUnicodeString((S8 *)chis_p->newCurrencyStr,
		                     (S8 *)chis_p->currencyStr);

		strcpy(chis_p->PPUStr, localPtr->ppu);
		AnsiiToUnicodeString((S8 *)chis_p->newPPUStr,
		                     (S8 *)chis_p->PPUStr);
	}
	return;

#else

		MSG_MMI_SMU_GET_PUC_RSP_STRUCT*	localPtr;
		U32 temp_ppu;
		U8 i;

		localPtr = ( MSG_MMI_SMU_GET_PUC_RSP_STRUCT * ) info;

		if ( localPtr->result == TRUE )
		{
			chis_p->ppu = 0;
			memset( chis_p->currencyStr, 0, 4*ENCODING_LENGTH );
			memset( chis_p->newCurrencyStr, 0, 4*ENCODING_LENGTH );
			memset( chis_p->PPUStr, 0, 20*ENCODING_LENGTH );
			memset( chis_p->newPPUStr, 0, 20*ENCODING_LENGTH );

			for (i=0; (i<3) && (localPtr->currency[i] != 0xFF); i++)
					chis_p->currencyStr[i] = localPtr->currency[i];

				AnsiiToUnicodeString((S8 *)chis_p->newCurrencyStr,
			                     (S8 *)chis_p->currencyStr);

			if( (localPtr->ppu[0] == '0') && (localPtr->ppu[1] == '0'))
			{
				memcpy( (S8 *)chis_p->PPUStr, (S8 *)localPtr->ppu, 11 );
				chis_p->ppu = atof( (S8*)chis_p->PPUStr );
				localPtr->ppu[14] = 0;
				chis_p->ppu += atoi( (S8*)localPtr->ppu +11 ) / 1000.0;
			}

			temp_ppu = (U32)chis_p->ppu;
			if ( chis_p->ppu - temp_ppu != 0)
			{
			   /* non integer */
				sprintf((S8 *)chis_p->PPUStr, "%u.%03d", temp_ppu,
				        (U32)((chis_p->ppu - (DOUBLE)temp_ppu) * 1000));
			}
			else
			{
	   			sprintf((S8 *)chis_p->PPUStr,"%u\0", (U32)chis_p->ppu);
			}
			AnsiiToUnicodeString((S8 *)chis_p->newPPUStr,
			                     (S8 *)chis_p->PPUStr);
		}
		return;
#endif
}

/**************************************************************

	FUNCTION NAME		: RspResetAllCallCost(void *info)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspResetAllCallCost( void* info )
{
	MSG_MMI_CC_RESET_ACM_RSP_STRUCT*	resp;

	resp = (MSG_MMI_CC_RESET_ACM_RSP_STRUCT *)info;

	if ( resp->result == TRUE )
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,CS_NOTIFYDURATION,SAVE_TONE);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,CS_NOTIFYDURATION,ERROR_TONE);
	}
	DeleteUptoScrID(SCR_CALL_COST_MAIN);
	return;
}

/**************************************************************

	FUNCTION NAME		: ReqSetMaxCallCost(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqSetMaxCallCost( void )
{
	MSG_MMI_CC_SET_MAX_ACM_REQ_STRUCT* pMsgIndexReq	= NULL ;
	MYQUEUE Message;
	U8	tempMaxCost[10];

	PRINT_INFORMATION( ( "File: [%s]  Line: [%d] <<LoadAllFdlist.>\n",
	                     __FILE__, __LINE__ ) );

	pMsgIndexReq = (MSG_MMI_CC_SET_MAX_ACM_REQ_STRUCT *)
	                  OslConstructDataPtr(
	                     sizeof( MSG_MMI_CC_SET_MAX_ACM_REQ_STRUCT ) );

   memset( (S8 *)pMsgIndexReq->pin, 0, MAX_SIM_PASSWD_LEN );
	UnicodeToAnsii( (S8 *)tempMaxCost, (S8 *)chis_p->newMaxCostStr );
	pMsgIndexReq->val = atoi( (S8 *)tempMaxCost );

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_CC_SET_MAX_ACM_REQ;
	// Message.oslSapId = 0;
	Message.oslDataPtr = (oslParaType *) pMsgIndexReq;
	Message.oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspSetMaxCallCost, PRT_MMI_CC_SET_MAX_ACM_RSP );
#else  /* No Defined __MMI_MULTI_SIM__ */
	SetProtocolEventHandler( RspSetMaxCallCost, PRT_MMI_CC_SET_MAX_ACM_RSP );
#endif /* __MMI_MULTI_SIM__ */
	OslMsgSendExtQueue( &Message );
	return;
}

/**************************************************************

	FUNCTION NAME		: RspSetMaxCallCost(void *info)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspSetMaxCallCost( void* info )
{
	MSG_MMI_CC_SET_MAX_ACM_RSP_STRUCT*	localPtr;

	localPtr = (MSG_MMI_CC_SET_MAX_ACM_RSP_STRUCT *)info;

	if ( localPtr->result == TRUE )
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SAVE_TONE);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}
	DeleteUptoScrID(SCR_CALL_COST_MAIN);
	return;
}


/**************************************************************

	FUNCTION NAME		: ReqSetPriceAndUnit(void )

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqSetPriceAndUnit( void )
{
	MSG_MMI_SMU_SET_PUC_REQ_STRUCT* pMsgIndexReq	= NULL;
	MYQUEUE Message;
	//U8	tempPPU[20];

	PRINT_INFORMATION( ( "File: [%s]  Line: [%d] <<LoadAllFdlist.>\n",
	                     __FILE__, __LINE__ ) );

	ClearAllKeyHandler();

	pMsgIndexReq = (MSG_MMI_SMU_SET_PUC_REQ_STRUCT *)
	                  OslConstructDataPtr(
	                     sizeof( MSG_MMI_SMU_SET_PUC_REQ_STRUCT ) );

   memset( (S8 *)pMsgIndexReq->pin, 0, MAX_SIM_PASSWD_LEN);
	UnicodeToAnsii( (S8 *)pMsgIndexReq->currency,
	                (S8 *)chis_p->newCurrencyStr );
	#if 0
	UnicodeToAnsii( (S8 *)tempPPU,
	                (S8 *)chis_p->newPPUStr );
	CHISTConvertPPU( (S8 *)pMsgIndexReq->ppu,
	                      (S8 *)tempPPU );
	#else
	UnicodeToAnsii( (S8 *)pMsgIndexReq->ppu,
	                (S8 *)chis_p->newPPUStr );
	
	#endif
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MMI_CHIS_DEST_MOD;
	Message.oslMsgId = PRT_MMI_CALL_COST_PRICEUNIT_SET_REQ;
	// Message.oslSapId = 0;
	Message.oslDataPtr = (oslParaType *)pMsgIndexReq;
	Message.oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
	MTPNP_CHISSetProtocolEventHandler( RspSetPriceAndUnit,PRT_MMI_CALL_COST_PRICEUNIT_SET_RSP );
#else
	SetProtocolEventHandler( RspSetPriceAndUnit, PRT_MMI_CALL_COST_PRICEUNIT_SET_RSP );
#endif
	OslMsgSendExtQueue( &Message );
	return;
}

/**************************************************************

	FUNCTION NAME		: RspSetPriceAndUnit(void *info)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RspSetPriceAndUnit( void* info )
{
	MSG_MMI_SMU_SET_PUC_RSP_STRUCT*	localPtr;

	localPtr = (MSG_MMI_SMU_SET_PUC_RSP_STRUCT *)info;

	if ( localPtr->result == TRUE )
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED),
		             IMG_GLOBAL_UNFINISHED, 1,
		             CS_NOTIFYDURATION,ERROR_TONE);
	}
	DeleteUptoScrID(SCR_CALL_COST_MAIN);
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTConvertPPU()

 
  							  string for setting

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTConvertPPU( S8 *reqBuf, S8 *inputBuf )
{
	U8 count=0, i, num=0;

	memset( reqBuf, '0', 18);

	while (inputBuf[count] != 0)
	{
	   if (inputBuf[count] <= '9' && inputBuf[count] >= '0')
	      num++;
		if (inputBuf[count++] == '.')
			break;
	}
	// overflow
	if ((count > 11 && num > 11) || count == 0)
		return;

   if (count == num)   // integer only
	{
      for (i=0; i<count; i++)
		   reqBuf[10-i] = inputBuf[count-i-1];
      return;
	}

   // save integer part
   for (i=0; i<count-1; i++)
		reqBuf[10-i] = inputBuf[count-i-2];

	// with decimal point
	i=0;
	while (inputBuf[count+i] != 0)
	{
		 reqBuf[11+i] = inputBuf[count+i];
		 i++;
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: CHISTGoBackCallCostHistory(void)

  	PURPOSE				: Getting Current Screen Handler

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CHISTGoBackCallCostHistory( void )
{
	GoBackToHistory(SCR_CALL_COST_MAIN);
}

#endif


