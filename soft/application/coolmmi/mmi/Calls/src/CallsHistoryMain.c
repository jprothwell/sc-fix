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
 * CallsHistoryMain.C
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: CallsHistoryMain.c

  	PURPOSE		: Call history main screen

 

	 

	DATE		: 10/22/2003

**************************************************************/

#ifndef _CALLS_HISTORY_MAIN_C
#define _CALLS_HISTORY_MAIN_C

#include "mmi_trace.h"
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
#include "csp.h"
#include "nvramenum.h"
#include "nvramprot.h"

#include "messagesexdcl.h"
#include "callmanagementstruct.h"
#include "phonebooktypes.h"

#include "callhistoryenum.h"
#include "callsdefs.h"
#include "callsstruct.h"
#include "callhistorymain.h"
#include "callslog.h"
#include "callsreset.h"
#include "callssmscounter.h"
#include "calltimes.h"
//micha1020
#include "protocolevents.h"
#include "commonscreens.h"
#include "servicedefs.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_calllog.h"
#include "mtpnp_ad_resdef.h"
#endif /* __MMI_MULTI_SIM__ */

#ifdef __MMI_GPRS_FEATURES__
#include "commonscreens.h"
#ifdef __MMI_MULTI_SIM__
U32 gprsTotalSent[MMI_SIM_NUMBER] = {0};
U32 gprsTotalReceived[MMI_SIM_NUMBER] = {0};
U32 gprsLastSent[MMI_SIM_NUMBER] = {0};
U32 gprsLastReceived[MMI_SIM_NUMBER] = {0};

U32 *p_gprsTotalSent = gprsTotalSent;
U32 *p_prsTotalReceived = gprsTotalReceived;
U32 *p_prsLastSent = gprsLastSent;
U32 *p_prsLastReceived = gprsLastReceived;
#else
U32 gprsTotalSent = 0;
U32 gprsTotalReceived = 0;
U32 gprsLastSent = 0;
U32 gprsLastReceived = 0;

U32 *p_gprsTotalSent = &gprsTotalSent;
U32 *p_prsTotalReceived = &gprsTotalReceived;
U32 *p_prsLastSent = &gprsLastSent;
U32 *p_prsLastReceived = &gprsLastReceived;
#endif
//#define __MMI_GPRS_UNIT_TEST__
#ifdef __MMI_MULTI_SIM__
void SetGPRSCounterBySimID(U8 simid);
#endif
#endif

U8 hilight_id = 0;

#undef __NEWSIMULATOR 

call_history_context_struct g_CallHistory_cntxt[MMI_SIM_NUMBER];
call_history_context_struct *chis_p = g_CallHistory_cntxt;

/**************************************************************

	FUNCTION NAME		: InitCallHistory(void)

  	PURPOSE				: Init sub modules of call history application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitCallHistory( void )
{
	/* initialization of callsreset submodule  */
	InitCallsReset();
	/* initialization of smscounters submodule */
	InitSMSCounter();
	/* initialization of calltimes submodule */
	InitCallTimes();

#ifdef __MMI_GPRS_FEATURES__
	InitGPRSCounter();
#endif
	
	return;
}

/**************************************************************

	FUNCTION NAME		: InitCallHistoryContext(void)

  	PURPOSE				: Init call history context

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReqGetPriceAndUnit( void );
void ReqGetAllCallCost( void );
void ReqGetMaxCallCost( void );
void InitCallHistoryContext( void )
{
	U8 i;
	S16 error;
	//U8 j = 1;
	MYTIME tempTime[3*MMI_SIM_NUMBER];

	memset(tempTime, 0, sizeof(tempTime));
	ReadRecord(NVRAM_EF_CALL_TIME_LID, 1, tempTime, sizeof(tempTime), &error);
	
	for( i = 0; i < MMI_SIM_NUMBER; i++)
	{
	#ifdef __MMI_MULTI_SIM__		
		SetSCHISReady(i, FALSE);
		SetCHISContext(i);
	#endif		
		memset ((U8 *)chis_p, 0, sizeof(call_history_context_struct));
		
		//ReadRecord(NVRAM_EF_CALL_TIME_LID, j++, (void *)&chis_p->last_call_time, sizeof(MYTIME), &error);

		//ReadRecord(NVRAM_EF_CALL_TIME_LID, j++, (void *)&chis_p->total_out_time, sizeof(MYTIME), &error);

		//ReadRecord(NVRAM_EF_CALL_TIME_LID, j++, (void *)&chis_p->total_in_time, sizeof(MYTIME), &error);
		
		memcpy((void *)&chis_p->last_call_time, &(tempTime[i*MMI_SIM_NUMBER]), sizeof(MYTIME));
		memcpy((void *)&chis_p->total_out_time, &(tempTime[i*MMI_SIM_NUMBER + 1]), sizeof(MYTIME));
		memcpy((void *)&chis_p->total_in_time, &(tempTime[i*MMI_SIM_NUMBER + 2]), sizeof(MYTIME));
	}
	
#ifdef __MMI_MULTI_SIM__
   	SetCHISContext(MMI_SIM_1);
#endif
	ReqGetPriceAndUnit();
	ReqGetAllCallCost();
	ReqGetMaxCallCost();
}

void ExitCHISTMainMenu()
{
	hilight_id = 0;
}
/**************************************************************

	FUNCTION NAME		: EntryCHISTMainMenu(void)

  	PURPOSE				: Main screen of call history

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCHISTMainMenu( void )
{
	//U16			nDispAttribute;  /* Stores display attribue */
	U8*			guiBuffer;       /* Buffer holding history data */
	U16			nStrItemList[MAX_SUB_MENUS];/* Stores the strings id of submenus returned */
	U16			nNumofItem;		 /* Stores no of children in the submenu*/
	U16			ItemIcons[MAX_SUB_MENUS];
	U8*			PopUpList[MAX_SUB_MENUS];
	U32 		title_str;
	/*S32			maskingByte=-1;
	S32         		menuItemId=-1;*/

#if defined(__MMI_MAINLCD_220X176__) || defined(__MMI_MAINLCD_160X128__)
	if (MTPNP_PFAL_Is_CardValid(0) == 0 && MTPNP_PFAL_Is_CardValid(1) == 0)
	{
		DisplayPopup( (PU8)GetString(SERVICES_NOT_PROVIDED_STRING_ID), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		return;
	};
#endif	
	/* Call Exit Handler*/
#ifdef __MMI_CH_NUMERIC_LIST_ICON__
	EntryNewScreen(SCR_CALL_HISTORY_MAIN_MENU, wgui_restore_list_menu_slim_style,
	               EntryCHISTMainMenu, NULL);
#else
	EntryNewScreen(SCR_CALL_HISTORY_MAIN_MENU, ExitCHISTMainMenu,
	               EntryCHISTMainMenu, NULL);
#endif

	/* reset the indicies for dialed/recived/missed call list menus */
	chis_p->currRecvdCallIndex = 0;
	chis_p->currMissedCallIndex = 0;
	chis_p->currDialedCallIndex = 0;
  	chis_p->currRejectedCallIndex = 0;

	/* Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR_CALL_HISTORY_MAIN_MENU );
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext( MAIN_MENU_CALL_HISTORY );  
	/* Get attribute of menu to be displayed */
	//nDispAttribute = GetDispAttributeOfItem( MAIN_MENU_CALL_HISTORY );
	/* Retrieve string/image ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext( MAIN_MENU_CALL_HISTORY, nStrItemList );  
	GetSequenceImageIds_Ext( MAIN_MENU_CALL_HISTORY, ItemIcons );  
	/* Set current parent id*/
	SetParentHandler( MAIN_MENU_CALL_HISTORY );
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler( ExecuteCurrHiliteHandler );

 
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */

	/* construct hint popup strings for menu items */
	ConstructHintsList(MAIN_MENU_CALL_HISTORY, PopUpList);

#ifdef __MMI_CH_NUMERIC_LIST_ICON__
      wgui_override_list_menu_slim_style(WGUI_LIST_MENU_SLIM_STYLE_NUMBER);
#endif

 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_CALL_HISTORY);
#endif
 
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	title_str = STRING_MTPNP_CARD1_CALL_HISTORY + chis_p->nSimId;
#else
	title_str = STR_SCR_CALL_HISTORY_MAIN_MENU_CAPTION;
#endif /* defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_) */

	/* Display the call history main screen */
#ifdef __MMI_NUMBER_MENU__
		ShowCategory52Screen( title_str,
	                      IMG_SCR_CALL_HISTORY_MAIN_MENU_CAPTION,
	                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
	                      IMG_GLOBAL_BACK, nNumofItem, nStrItemList,
	                      (PU16)gIndexIconsImageList, ( U8 * * )
	                      PopUpList, 0, hilight_id,//u16callhist_curindex,
	                      guiBuffer );
#else
	ShowCategory52Screen( title_str,
	                      IMG_SCR_CALL_HISTORY_MAIN_MENU_CAPTION,
	                      STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
	                      IMG_GLOBAL_BACK, nNumofItem, nStrItemList,
	                      ItemIcons, ( U8 * * )
	                      PopUpList, 0, hilight_id,//u16callhist_curindex,
	                      guiBuffer );
#endif
	/* Register function with right softkey */
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

#ifdef __MMI_GPRS_FEATURES__

void InitGPRSCounter( void )
{
	SetHiliteHandler( MENU_CALL_HISTORY_GPRSCOUNTER, HighlightGPRSCount );
	SetHiliteHandler(MENU_CALL_HISTORY_GPRS_LASTSENT,HighlightGPRSCountLastSent);
	SetHiliteHandler(MENU_CALL_HISTORY_GPRS_LASTRECEIVED,HighlightGPRSCountLastRecieved);
	SetHiliteHandler(MENU_CALL_HISTORY_GPRS_ALLSENT,HighlightGPRSCountAllSent);
	SetHiliteHandler(MENU_CALL_HISTORY_GPRS_ALLRECEIVED,HighlightGPRSCountAllReceived);
	SetHiliteHandler(MENU_CALL_HISTORY_GPRS_RESETCOUNTER,HighlightGPRSCountCounterReset);

	SetHintHandler(MENU_CALL_HISTORY_GPRS_LASTSENT,HintGPRSCountLastSent);
	SetHintHandler(MENU_CALL_HISTORY_GPRS_LASTRECEIVED,HintGPRSCountLastRecieved);
	SetHintHandler(MENU_CALL_HISTORY_GPRS_ALLSENT,HintGPRSCountAllSent);
	SetHintHandler(MENU_CALL_HISTORY_GPRS_ALLRECEIVED,HintGPRSCountAllReceived);
#ifdef __MMI_MULTI_SIM__
	SetHiliteHandler( MENU_MTPNP_CALL_HISTORY_GPRSCOUNTER, HighlightGPRSCount );
	SetHiliteHandler(MENU_MTPNP_CALL_HISTORY_GPRS_LASTSENT,HighlightGPRSCountLastSent);
	SetHiliteHandler(MENU_MTPNP_CALL_HISTORY_GPRS_LASTRECEIVED,HighlightGPRSCountLastRecieved);
	SetHiliteHandler(MENU_MTPNP_CALL_HISTORY_GPRS_ALLSENT,HighlightGPRSCountAllSent);
	SetHiliteHandler(MENU_MTPNP_CALL_HISTORY_GPRS_ALLRECEIVED,HighlightGPRSCountAllReceived);
	SetHiliteHandler(MENU_MTPNP_CALL_HISTORY_GPRS_RESETCOUNTER,HighlightGPRSCountCounterReset);

	SetHintHandler(MENU_MTPNP_CALL_HISTORY_GPRS_LASTSENT,HintGPRSCountLastSent);
	SetHintHandler(MENU_MTPNP_CALL_HISTORY_GPRS_LASTRECEIVED,HintGPRSCountLastRecieved);
	SetHintHandler(MENU_MTPNP_CALL_HISTORY_GPRS_ALLSENT,HintGPRSCountAllSent);
	SetHintHandler(MENU_MTPNP_CALL_HISTORY_GPRS_ALLRECEIVED,HintGPRSCountAllReceived);
#endif
	return;
}

void HighlightGPRSCount( void )
{

#ifdef __MMI_GPRS_UNIT_TEST__

	*p_gprsTotalSent = 0;
	*p_prsTotalReceived = 1025;
	*p_prsLastSent = 1048533;
	*p_prsLastReceived = 11048576;
	SetKeyHandler( EntryScrGPRSCounter,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction( EntryScrGPRSCounter, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#else

	SetKeyHandler( GetGPRSCountersRequest,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction( GetGPRSCountersRequest, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

#endif

	return;
}

void HighlightGPRSCountLastSent(void)
{
	ChangeLeftSoftkey( 0, 0);
	return;
}

void HighlightGPRSCountLastRecieved(void)
{
	ChangeLeftSoftkey( 0, 0);
	return;
}

/*****************************************************************************
 * FUNCTION
 *  HighlightGPRSCountAllSent
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightGPRSCountAllSent(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(0, 0);
    return;
}


/*****************************************************************************
 * FUNCTION
 *  HighlightGPRSCountAllReceived
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightGPRSCountAllReceived(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(0, 0);
    return;
}



/*****************************************************************************
 * FUNCTION
 *  HighlightGPRSCountCounterReset
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightGPRSCountCounterReset(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (*p_gprsTotalSent == 0 &&
        *p_prsTotalReceived == 0 &&
        *p_prsLastSent ==   0 &&
        *p_prsLastReceived == 0)
    {
        ChangeLeftSoftkey(0, 0);
    }
    else
    {    
        ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
        SetLeftSoftkeyFunction( EntryScrGPRSRecvCounterResetConfirmation, KEY_EVENT_UP );
    }
    return;
}


//micha0202 begin

void HintGPRSCountLastSent(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE]; 
	/* reinitialize the hint data */

	memset( ( PS8 ) hintData[(index)], 0, sizeof(hintData[(index)]));
//micha0216
		sprintf((S8 * ) value, "%d B", *p_prsLastSent);
	AnsiiToUnicodeString( ( PS8 ) hintData[(index)], ( PS8 ) value );
	return;
}

void HintGPRSCountLastRecieved(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE]; 
	/* reinitialize the hint data */

	memset( ( PS8 ) hintData[(index)], 0, sizeof(hintData[(index)]));
//micha0216
		sprintf((S8 * ) value, "%d B", *p_prsLastReceived);
	AnsiiToUnicodeString( ( PS8 ) hintData[(index)], ( PS8 ) value );
	return;
}

/*****************************************************************************
 * FUNCTION
 *  HintGPRSCountAllSent
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void HintGPRSCountAllSent(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 value[MAX_SUB_MENU_HINT_SIZE];
    U32 fractionvalue = 0;
    U32 divvalud = 0;
    char Unit = 'K';
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_ADP_GPRS, "Func:%s, TotalSent = %d", __FUNCTION__, *p_gprsTotalSent);
    memset( ( PS8 ) hintData[(index)], 0, sizeof(hintData[(index)]));
    if(*p_gprsTotalSent < 1024)
    {
        sprintf((S8 * ) value, "%d B", *p_gprsTotalSent);
    }
    else
    {
        if(*p_gprsTotalSent < 1024 * 1024)
        {
            divvalud = 1024;
        }
        else
        {
            divvalud = 0x100000;
            Unit = 'M';
        }
        
        fractionvalue = (*p_gprsTotalSent)%(divvalud);
        fractionvalue = 100 * fractionvalue / divvalud;

        if(fractionvalue == 0)
        {
            sprintf((S8 * ) value, "%d %cB", *p_gprsTotalSent/divvalud, Unit);        
        }
        else if(((fractionvalue) %(10)) == 0)
        {
            fractionvalue /= 10;
            sprintf((S8 * ) value, "%d.%d %cB", *p_gprsTotalSent/divvalud, fractionvalue, Unit);        
        }
        else
        {
            sprintf((S8 * ) value, "%d.%d %cB", *p_gprsTotalSent/divvalud, fractionvalue, Unit);        
        }
    }
    
    AnsiiToUnicodeString( ( PS8 ) hintData[(index)], ( PS8 ) value );
    return;
}

/*****************************************************************************
 * FUNCTION
 *  HintGPRSCountAllReceived
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void HintGPRSCountAllReceived(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 value[MAX_SUB_MENU_HINT_SIZE];
    U32 fractionvalue = 0;
    U32 divvalud = 0;
    char Unit = 'K';

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_ADP_GPRS, "Func:%s, TotalReceived = %d", __FUNCTION__, *p_prsTotalReceived);
    memset( ( PS8 ) hintData[(index)], 0, sizeof(hintData[(index)]));
    if(*p_prsTotalReceived < 1024)
    {
        sprintf((S8 * ) value, "%d B", *p_prsTotalReceived);
    }
    else
    {
        if(*p_prsTotalReceived < 1024 * 1024)
        {
            divvalud = 1024;
        }
        else
        {
            divvalud = 0x100000;
            Unit = 'M';
        }
        
        fractionvalue = (*p_prsTotalReceived)%(divvalud);
        fractionvalue = 100 * fractionvalue / divvalud;

        if(fractionvalue == 0)
        {
            sprintf((S8 * ) value, "%d %cB", *p_prsTotalReceived/divvalud, Unit);        
        }
        else if(((fractionvalue) %(10)) == 0)
        {
            fractionvalue /= 10;
            sprintf((S8 * ) value, "%d.%d %cB", *p_prsTotalReceived/divvalud, fractionvalue, Unit);        
        }
        else
        {
            sprintf((S8 * ) value, "%d.%d %cB", *p_prsTotalReceived/divvalud, fractionvalue, Unit);        
        }
    }

    AnsiiToUnicodeString( ( PS8 ) hintData[(index)], ( PS8 ) value );
    return;
}


/*****************************************************************************
 * FUNCTION
 *  GetGPRSCountersRequest
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GetGPRSCountersRequest(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* micha1020 */
#ifdef __MMI_GPRS_FEATURES__
    MYQUEUE Message;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearAllKeyHandler();
    ClearKeyHandler(KEY_END, KEY_EVENT_UP);
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
    //micha1020
    SetProtocolEventHandler(GetGPRSCountersResponse, PRT_MMI_PS_GET_GPRS_DATA_COUNTER_RSP);
    /* format message to send req for get plmn list */
    Message.oslSrcId=MOD_MMI;
    Message.oslDestId=MOD_L4C;
#ifdef __MMI_MULTI_SIM__
    SetGPRSCounterBySimID(chis_p->nSimId);
    Message.oslDestId = MOD_L4C + chis_p->nSimId;        
#endif
    //micha1020
    Message.oslMsgId = PRT_MMI_PS_GET_GPRS_DATA_COUNTER_REQ;
    Message.oslDataPtr = NULL;
    Message.oslPeerBuffPtr= NULL;
    OslMsgSendExtQueue(&Message);
    return;
#endif	
}

/*****************************************************************************
 * FUNCTION
 *  GetGPRSCountersResponse
 * DESCRIPTION
 *  
 * PARAMETERS
 *  info        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void GetGPRSCountersResponse(void *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* micha1020 */
#ifdef __MMI_GPRS_FEATURES__
    mmi_ps_get_gprs_data_counter_rsp_struct *localPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    localPtr = (mmi_ps_get_gprs_data_counter_rsp_struct *)info;

    if (localPtr->result == MMI_TRUE) 
    {
        *p_gprsTotalSent = localPtr->counter_info.total_tx_data;
        *p_prsTotalReceived = localPtr->counter_info.total_rx_data;
        *p_prsLastSent = localPtr->counter_info.last_tx_data;
        *p_prsLastReceived = localPtr->counter_info.last_rx_data;
    }
    else
    {
        *p_gprsTotalSent = 0;
        *p_prsTotalReceived = 0;
        *p_prsLastSent = 0;
        *p_prsLastReceived = 0;
    }		

    EntryScrGPRSCounter();
#endif	
}


/*****************************************************************************
 * FUNCTION
 *  EntryScrGPRSRecvCounterResetConfirmation
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryScrGPRSRecvCounterResetConfirmation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO, 
        get_string(STR_GPRS_RESET_COUNTER), 
        IMG_GLOBAL_QUESTION, WARNING_TONE);

    /* Set the left/right soft key funtions */
    SetLeftSoftkeyFunction(ResetGPRSCounterReq,KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
    /* set the exit handler for the current screen */
    
    return;
}

/*****************************************************************************
 * FUNCTION
 *  ResetGPRSCounterReq
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ResetGPRSCounterReq(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_GPRS_UNIT_TEST__	

    *p_gprsTotalSent = 0;
    *p_prsTotalReceived = 0;
    *p_prsLastSent = 0;
    *p_prsLastReceived = 0;
    DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,CS_NOTIFYDURATION,SUCCESS_TONE);
    DeleteUptoScrID(SCR8004_GPRS_COUNTER);

#else

    //micha1020
#ifdef __MMI_GPRS_FEATURES__

    MYQUEUE Message;

    ClearAllKeyHandler();
    ClearKeyHandler(KEY_END, KEY_EVENT_UP);
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
    
    SetProtocolEventHandler(ResetGPRSCounterRsp, PRT_MMI_PS_RESET_GPRS_DATA_COUNTER_RSP);
    
    /* format message to send req for get plmn list */
    Message.oslSrcId=MOD_MMI;
    Message.oslDestId=MOD_L4C;
#ifdef __MMI_MULTI_SIM__
    Message.oslDestId = MOD_L4C + chis_p->nSimId;        
#endif

    Message.oslMsgId = PRT_MMI_PS_RESET_GPRS_DATA_COUNTER_REQ;
    Message.oslDataPtr = NULL;
    Message.oslPeerBuffPtr= NULL;
    OslMsgSendExtQueue(&Message);

#endif

#endif

    return;
}

/*****************************************************************************
 * FUNCTION
 *  ResetGPRSCounterRsp
 * DESCRIPTION
 *  
 * PARAMETERS
 *  info        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void ResetGPRSCounterRsp(void *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* micha1020 */
#ifdef __MMI_GPRS_FEATURES__

    mmi_ps_reset_gprs_data_counter_rsp_struct *localPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    localPtr = (mmi_ps_reset_gprs_data_counter_rsp_struct *)info;

    if (localPtr->result == MMI_TRUE)
    {
        *p_gprsTotalSent = 0;
        *p_prsTotalReceived = 0;
        *p_prsLastSent = 0;
        *p_prsLastReceived = 0;
        DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,CS_NOTIFYDURATION,SUCCESS_TONE);
    }
    else 
    {
        DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE) ,IMG_GLOBAL_UNFINISHED, 1,CS_NOTIFYDURATION,ERROR_TONE);
    }		

    DeleteUptoScrID(SCR8004_GPRS_COUNTER);
    return;

#endif
	
}



void EntryScrGPRSCounter( void )
{
	U16			nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U8*			guiBuffer;					/* Buffer holding history data */
	U16			nNumofItem;      /* Stores no of children in the submenu*/
	//U16			nDispAttribute;  /* Stores display attribue */
	U8*			u8GPRSCount[MAX_SUB_MENUS]; /* sms counters for display hint popups */

	/* Call Exit Handler*/
	EntryNewScreen(SCR8004_GPRS_COUNTER, NULL,
	               EntryScrGPRSCounter, NULL);

	/* Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR8004_GPRS_COUNTER );	
	/* Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild( MENU_CALL_HISTORY_GPRSCOUNTER );
	/* Get attribute of menu to be displayed */
	//nDispAttribute = GetDispAttributeOfItem( MENU_CALL_HISTORY_GPRSCOUNTER );
	/* Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds( MENU_CALL_HISTORY_GPRSCOUNTER, nStrItemList );
	/* Set current parent id*/
	SetParentHandler( MENU_CALL_HISTORY_GPRSCOUNTER );
	/* Register highlight handler to be called in menu screen */
	RegisterHighlightHandler( ExecuteCurrHiliteHandler );
	/* construct hint popups for the menu items */
	ConstructHintsList(MENU_CALL_HISTORY_GPRSCOUNTER, u8GPRSCount);
	/* Display Category52 Screen */
	ShowCategory52Screen( STR_GPRS_CAPTION, IMG_SCR8056_CAPTION, 0,
	                      0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                      nNumofItem, nStrItemList, (U16 *)gIndexIconsImageList,
	                      u8GPRSCount, 0, 0, guiBuffer );	
	/* Register left arrow key */
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	/* Register function with left/right softkey */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

#ifdef __MMI_MULTI_SIM__
void SetGPRSCounterBySimID(U8 simid)
{
    p_gprsTotalSent = &gprsTotalSent[simid];
    p_prsTotalReceived = &gprsTotalReceived[simid];
    p_prsLastSent = &gprsLastSent[simid];
    p_prsLastReceived = &gprsLastReceived[simid];
}
#endif
#endif
#endif


