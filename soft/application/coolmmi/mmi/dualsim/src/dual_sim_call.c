/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2009, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*       dual_sim_call.c                                                           */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   MTPNP layer soruce file, includes the implementation of MTPNP Call APIs.      */
/*                                                                            */
/******************************************************************************/

#include "mmi_features.h"
#include "globaldefs.h"
#include "globalscrenum.h"
#include "frameworkstruct.h"
#include "gui_data_types.h"
#include "eventsgprot.h"
#include "mmi_msg_struct.h"
#include "simdetectiondef.h"
#include "simdetectionhwmapfile.h"
#include "debuginitdef.h"
#include "wgui_status_icons.h"

#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "debuginitdef.h"	/*debug info*/
#include "protocolevents.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_cm.h"
#include "wgui_categories_inputs.h"
#include "wgui_status_icons.h"
#include "allappgprot.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebooktypes.h"

#include "callsdefs.h"
#include "settingstructs.h"
#include "settinggprots.h"
#include "simdetectiongprot.h"
#include "protocolevents.h"
#include "callfixeddial.h"
#include "barreddiallist.h"
#include "settingprofile.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "commonscreens.h"
#include "mmi_features.h"
#include "callsetupenum.h"
#include "settingprot.h"
#include "callmanagementiddef.h"
#include "networksetupgprot.h"
#include "organizergprot.h"
#include "idleappdef.h"
#include "simdetectiongexdcl.h"
#include "settingsgexdcl.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "securitysetup.h"
#include "callmanagementgprot.h"
#include "callhistoryenum.h"

#ifdef __MMI_MULTI_SIM__

#include "dual_sim.h"
#include "dual_sim_call.h"

#include "protocolevents.h"
#include "smsapi.h"
#include "smsstruct.h"
#include "wgui_status_icons.h"
#include "smsguiinterfacetype.h"
#include "mmi_msg_context.h"
#include"historygprot.h"
#include "smsguiinterfaceprot.h"
#include "smsfunc.h"
#include "phonebookgprot.h"
#include "commonscreens.h"
#include "settingprofile.h"


#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"


#include "mtpnp_ad_resdef.h"
#include "messagesresourcedata.h"
#include "incomingcallmanagementprot.h"
#include "mainmenudef.h"
#include "idleappprot.h"

#include "callsetup.h"
mmi_ucm_cntx_struct g_mmi_ucm_cntx;
mmi_ucm_cntx_struct *g_ucm_p = &g_mmi_ucm_cntx;

extern BOOL SSCStringParsing2(void);
extern pBOOL isInCall(void);
extern pBOOL mmi_bootup_get_active_flight_mode();

//////////////////////////////////////////////////////
////CC
mmi_ucm_call_type_enum call_type_array[] =
{
	MMI_UCM_VOICE_CALL_TYPE,
	MMI_UCM_VOICE_CALL_TYPE_SIM2,
	MMI_UCM_VOICE_CALL_TYPE_SIM3,
	MMI_UCM_VOICE_CALL_TYPE_SIM4
};

void UCMResourceData(void)
{
	U8 i;
	/* dial call type */
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MENU_ID_UCM_CALL_TYPE_VOICE+i, mmi_ucm_highlight_call_type_voice);
	}
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_highlight_call_type_voice
 * DESCRIPTION
 *  Funtion is called when voice call is selected
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_highlight_call_type_voice(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (g_ucm_p->call_misc.entry_type)
    {
        case MMI_UCM_ENTRY_MENU:
        {
            g_ucm_p->mo_info.dial_type = call_type_array[g_ucm_p->nSimID];
            SetLeftSoftkeyFunction(mmi_ucm_dial_option_make_call, KEY_EVENT_UP);
            SetKeyHandler(mmi_ucm_dial_option_make_call, KEY_SEND, KEY_EVENT_DOWN);
        }
        break;

        case MMI_UCM_ENTRY_SEND_KEY:
        case MMI_UCM_ENTRY_CENTER_KEY:
        {
            mmi_ucm_set_call_channel(g_ucm_p->nSimID);
            SetLeftSoftkeyFunction(mmi_ucm_dial_option_make_call_exec_callback, KEY_EVENT_UP);
            SetKeyHandler(mmi_ucm_dial_option_make_call_exec_callback, KEY_SEND, KEY_EVENT_DOWN);
        }
        break;

        default:
        {
            MMI_ASSERT(0);
        }
        break;
    }

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    
    return;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_dial_option_make_call
 * DESCRIPTION
 *  Funtion is called when make call from dial option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_dial_option_make_call(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8* num_ucs2 = NULL;
    U32 num_len = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
    DeleteScreenIfPresent(SCR_ID_UCM_DIAL_CALL_TYPE_MENU);

    g_ucm_p->mo_info.module_id = MMI_UCM_FROM_IDLE;

    num_ucs2 = OslMalloc(MMI_UCM_MAX_NUM_URI_LEN * ENCODING_LENGTH);
    memset(num_ucs2, 0, MMI_UCM_MAX_NUM_URI_LEN * ENCODING_LENGTH);
    
    num_len = strlen((S8*)g_ucm_p->mo_info.dial_num.num_uri);

    if (num_len > (MMI_UCM_MAX_NUM_URI_LEN - 1))
    {
        num_len = (MMI_UCM_MAX_NUM_URI_LEN - 1);
    }

    mmi_asc_n_to_ucs2((S8*)num_ucs2, (S8*)g_ucm_p->mo_info.dial_num.num_uri, num_len);
    
    mmi_ucm_app_make_call(g_ucm_p->mo_info.dial_type, (U8*) num_ucs2);

    OslMfree(num_ucs2);

    if (g_ucm_p->mo_info.callback != NULL)
    {
        g_ucm_p->mo_info.callback((void*)g_ucm_p->mo_info.callback_para);
    }    
    return;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_set_channel
 * DESCRIPTION
 *  set call channel for dual SIM mode
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_set_call_channel(U8 nSimID)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_MASTER + nSimID);
}



/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_dial_option_make_call_sendkey
 * DESCRIPTION
 *  Funtion is called when make call from dial option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_dial_option_make_call_exec_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
    DeleteScreenIfPresent(SCR_ID_UCM_DIAL_CALL_TYPE_MENU);

    switch (g_ucm_p->call_misc.entry_type)
    {
        case MMI_UCM_ENTRY_SEND_KEY:
        {
            mmi_ucm_exec_sendkey_hdlr();
        }
        break;
        
        case MMI_UCM_ENTRY_CENTER_KEY:
        {
            mmi_ucm_exec_centerkey_hdlr();
        }
        break;

        case MMI_UCM_ENTRY_MENU:
        default:
        {
            MMI_ASSERT(0);
        }
        break;
    }

    return;        
}


void MTPNP_PFAL_CC_SetCenterFunc(FuncPtr cenfunc)
{
    mmi_ucm_set_centerkey_hdlr(cenfunc);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_set_centerkey_hdlr
 * DESCRIPTION
 *  set center key handler function 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_set_centerkey_hdlr(FuncPtr centerkey_hdlr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ucm_p->call_misc.centerkey_hdlr = centerkey_hdlr;
}

void MTPNP_PFAL_CC_CSKHandler(void)
{
	U8 nUsableSide;
	U8 UsableSideIndex;
	U8 StartUpMode;
	U8 i;

	if (NULL == MTPNP_PFAL_CC_GetCenterFunc())
	{
		kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_CSKHandler NO MO_CALL_FUNCTION ");
		return;
	}
	
	nUsableSide = MTPNP_AD_Get_UsableSide_Number();
	UsableSideIndex = MTPNP_AD_GET_UsableSide_Index();
	StartUpMode = MTPNP_AD_Get_Startup_Mode();
	
	if(isInCall())  //it there is a call, don't change the channel
	{
		MTPNP_PFAL_CC_ExeCenterFunc();
	}
	else if (nUsableSide > 1)
	{
		mmi_ucm_entry_dial_option_by_centerkey();
	}
	else if (nUsableSide == 1)
	{
		MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, UsableSideIndex);

		MTPNP_PFAL_CC_ExeCenterFunc();
	}
	/* UCM should do the error handling for flight mode */
	else
	{
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			if(StartUpMode&(DM_STATUS_MASK<<i))
				break;
		}
		MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_MASTER + i);

		MTPNP_PFAL_CC_ExeCenterFunc();

		kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKeyHandler no sim to make call, use sim%d to call",i);
		return;
	}
}



/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_entry_dial_option_by_centerkey
 * DESCRIPTION
 *  General dial call type option by center key
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_entry_dial_option_by_centerkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ucm_p->call_misc.entry_type = MMI_UCM_ENTRY_CENTER_KEY;
    
    mmi_ucm_dial_option_unhide_menu_item();
    mmi_ucm_entry_call_type();
 }



/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_entry_call_type
 * DESCRIPTION
 *  Shows the call type selection screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_call_type_highlight_handler(S32 hiliteid)
{
	g_ucm_p->nSimID = GetReqSrcModeIndexByIndex((U8)hiliteid);
	ExecuteCurrHiliteHandler(hiliteid);
}
void mmi_ucm_entry_call_type(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer = NULL;
	U16 numItems = 0;
	U16 nStrItemList[MAX_SUB_MENUS];

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	EntryNewScreen(SCR_ID_UCM_DIAL_CALL_TYPE_MENU, NULL, mmi_ucm_entry_call_type, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_UCM_DIAL_CALL_TYPE_MENU);
	
	MTPNP_AD_SetMenuHideUnhide(MENU_ID_UCM_CALL_TYPE_VOICE);
	
	numItems = GetNumOfChild_Ext(MENU_ID_UCM_CALL_TYPE);
	
	GetSequenceStringIds_Ext(MENU_ID_UCM_CALL_TYPE, nStrItemList);    
	SetParentHandler(MENU_ID_UCM_CALL_TYPE);

	RegisterHighlightHandler(mmi_ucm_call_type_highlight_handler);

	ShowCategory52Screen(
				STR_GLOBAL_DIAL,
				IMG_CALL_SIM_SELECT_CAPTION,
				STR_GLOBAL_OK,
				IMG_GLOBAL_OK,
				STR_GLOBAL_BACK,
				IMG_GLOBAL_BACK,
				numItems,
				nStrItemList,
				(U16*) gIndexIconsImageList,
				NULL,
				0,
				0,
				guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_dial_option_unhide_menu_item
 * DESCRIPTION
 *  Unhide menu items of dial option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_dial_option_unhide_menu_item(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 i;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		mmi_frm_hide_menu_item(MENU_ID_UCM_CALL_TYPE_VOICE + i);
		if(MTPNP_PFAL_Is_Card_Usable(i))
			mmi_frm_unhide_menu_item(MENU_ID_UCM_CALL_TYPE_VOICE + i);
	}

	#ifdef __MMI_VOIP__
	mmi_frm_unhide_menu_item(MENU_ID_UCM_CALL_TYPE_VOIP);
	#endif /* __MMI_VOIP__ */

	#ifdef __MMI_VIDEO_TELEPHONY__
	mmi_frm_unhide_menu_item(MENU_ID_UCM_CALL_TYPE_VIDEO);
	#endif /* __MMI_VIDEO_TELEPHONY__ */
        
}
FuncPtr MTPNP_PFAL_CC_GetCenterFunc(void)
{
    return mmi_ucm_get_centerkey_hdlr();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_get_centerkey_hdlr
 * DESCRIPTION
 *  get center key handler function 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
FuncPtr mmi_ucm_get_centerkey_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_ucm_p->call_misc.centerkey_hdlr;
}
void MTPNP_PFAL_CC_ExeCenterFunc(void)
{
    mmi_ucm_exec_centerkey_hdlr();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_exec_centerkey_hdlr
 * DESCRIPTION
 *  get center handler function 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_exec_centerkey_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_ucm_p->call_misc.centerkey_hdlr != NULL)
    {
        g_ucm_p->call_misc.centerkey_hdlr();
    }
    else
    {
        MMI_ASSERT(0);
    }
}


/*****************************************************
**function MTPNP_PFAL_CC_SendKey2Handler
**to handle sendkey2 when it's function is to make a GSM call
******************************************************/
void MTPNP_PFAL_CC_SendKey1Handler(void)
{
    //to make GSM call through Slave Net    
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
    kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKey1Handler MO_CALL_FUNCTION=%x", MTPNP_PFAL_CC_GetMOCallFunc());
    if (0 != MTPNP_PFAL_CC_GetMOCallFunc())
    {
        MTPNP_PFAL_CC_ExeMOCallFunc();
    }
    else
    {
        kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKey1Handler NO MO_CALL_FUNCTION ");
    }
}

/*****************************************************
**function MTPNP_PFAL_CC_SendKey2Handler
**to handle sendkey2 when it's function is to make a GSM call
******************************************************/
void MTPNP_PFAL_CC_SendKey2Handler(void)
{
    //to make GSM call through Slave Net    
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
    kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKey2Handler MO_CALL_FUNCTION=%x", MTPNP_PFAL_CC_GetMOCallFunc());
    if (0 != MTPNP_PFAL_CC_GetMOCallFunc())
    {
        MTPNP_PFAL_CC_ExeMOCallFunc();
    }
    else
    {
        kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKey2Handler NO MO_CALL_FUNCTION ");
    }
}
#ifdef __SENDKEY2_SUPPORT__

void MTPNP_PFAL_CC_SendKey3Handler(void)
{
    //to make GSM call through Slave Net    
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SIM3);
    kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKey2Handler MO_CALL_FUNCTION=%x", MTPNP_PFAL_CC_GetMOCallFunc());
    if (0 != MTPNP_PFAL_CC_GetMOCallFunc())
    {
        MTPNP_PFAL_CC_ExeMOCallFunc();
    }
    else
    {
        kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKey2Handler NO MO_CALL_FUNCTION ");
    }
}
void MTPNP_PFAL_CC_SendKey4Handler(void)
{
    //to make GSM call through Slave Net    
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SIM4);
    kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKey2Handler MO_CALL_FUNCTION=%x", MTPNP_PFAL_CC_GetMOCallFunc());
    if (0 != MTPNP_PFAL_CC_GetMOCallFunc())
    {
        MTPNP_PFAL_CC_ExeMOCallFunc();
    }
    else
    {
        kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKey2Handler NO MO_CALL_FUNCTION ");
    }
}

#endif


//#ifdef __SENDKEY2_SUPPORT__
/*****************************************************
**function MTPNP_PFAL_CC_Sim1Invaild
******************************************************/
void MTPNP_PFAL_CC_Sim1Invaild(void)
{
#if defined(__PROJECT_GALLITE_C01__)
	if (MTPNP_PFAL_Is_CardAbsent(0))
		DisplayPopup((PU8)GetString(STRING_MTPNP_SIM1_NOT_INSERT) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	else
#endif
	DisplayPopup((PU8)GetString(STRING_MTPNP_SIM1_NOT_AVAILABLE) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
}

void MTPNP_PFAL_CC_Sim2Invaild(void)
{
#if defined(__PROJECT_GALLITE_C01__)
	if (MTPNP_PFAL_Is_CardAbsent(1))
	{
		DisplayPopup((PU8)GetString(STRING_MTPNP_SIM2_NOT_INSERT) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	}
	else
#endif
	DisplayPopup((PU8)GetString(STRING_MTPNP_SIM2_NOT_AVAILABLE) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
}
void MTPNP_PFAL_CC_Sim3Invaild(void)
{
	DisplayPopup((PU8)GetString(STRING_MTPNP_SIM3_NOT_AVAILABLE) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
}
void MTPNP_PFAL_CC_Sim4Invaild(void)
{
	DisplayPopup((PU8)GetString(STRING_MTPNP_SIM4_NOT_AVAILABLE) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
}

//#endif
/*****************************************************
**function MTPNP_PFAL_CC_HandleSendKeys
input:newCallFunc current key_send 
******************************************************/
/******************************************************/
#ifdef __MMI_USE_LSK_AS_SEND2__
void DummyKeyHandler(void)
{
}
#endif
void MTPNP_PFAL_CC_HandleSendKeys(FuncPtr newCallFunc, mmi_key_types_enum keyevent)
{
#ifdef __SENDKEY2_SUPPORT__
	U8 call_state_1 = 0, call_state_2 = 0;
#endif
        U8 call_state_3 = 0;
        U8 call_state_4 = 0;

	mmi_trace(g_sw_CC, "MTPNP_PFAL_CC_HandleSendKeys newCallFunc=%x", newCallFunc);
	MTPNP_PFAL_CC_SetMOCallFunc(newCallFunc);

	SetKeyHandler(MTPNP_PFAL_CC_SendKeyHandler, KEY_SEND, keyevent);

#ifdef __SENDKEY2_SUPPORT__
	ClearKeyHandler(KEY_SEND1, keyevent);
	#ifndef __MMI_USE_LSK_AS_SEND2__
	ClearKeyHandler(KEY_SEND2, keyevent);
	#else
	SetKeyHandler(get_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY),KEY_ENTER,KEY_EVENT_UP);
	#ifdef __MMI_WGUI_CSK_ENABLE__
	SetCenterSoftkeyFunction(get_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY), KEY_EVENT_UP);
	#endif
#ifdef TGT_GALLITE_G800 
#if (NUMBER_OF_SIM > 2)    
	ClearKeyHandler(KEY_LSK, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
#endif	
#endif

#ifdef TGT_TD_4SIM
#if (NUMBER_OF_SIM > 2)    
	ClearKeyHandler(KEY_LSK, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
#endif
	ClearKeyHandler(KEY_CAMERA, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_CAMERA, KEY_EVENT_UP);
#endif
	#endif/*__MMI_USE_LSK_AS_SEND2__*/
	MTPNP_PFAL_Retrieve_CallDial_State(&call_state_1, &call_state_2, &call_state_3, &call_state_4);


	mmi_trace(g_sw_CC, "call_state_1=%d, call_state_2=%d", call_state_1, call_state_2);
#if !defined(__PROJECT_GALLITE_C01__)
        if (call_state_1)
        {

            SetKeyHandler(MTPNP_PFAL_CC_SendKey1Handler, KEY_SEND1, keyevent);
        }
        else
        {
     //       MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
            SetKeyHandler(MTPNP_PFAL_CC_Sim1Invaild, KEY_SEND1, keyevent);
        }
        if (call_state_2)
        {
#ifndef __MMI_USE_LSK_AS_SEND2__
    		SetKeyHandler(MTPNP_PFAL_CC_SendKey2Handler, KEY_SEND2, keyevent);
#else
		SetLeftSoftkeyFunction(DummyKeyHandler,KEY_EVENT_UP);
		SetLeftSoftkeyFunction(MTPNP_PFAL_CC_SendKey2Handler,keyevent);
		
#endif

        }
        else
        {
         //   MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);

#ifndef __MMI_USE_LSK_AS_SEND2__
            SetKeyHandler(MTPNP_PFAL_CC_Sim2Invaild, KEY_SEND2, keyevent);
#else
		if(call_state_1)
			{
			//SetKeyHandler(get_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY), KEY_LSK, keyevent);
			SetKeyHandler(MTPNP_PFAL_CC_SendKeyHandler, KEY_LSK, keyevent);
			}
		else
			{
		        SetKeyHandler(MTPNP_PFAL_CC_Sim2Invaild, KEY_LSK, keyevent);
			}
#endif
         
        }
		#else
        if ((call_state_1) && (call_state_2))
        {
			
            SetKeyHandler(MTPNP_PFAL_CC_SendKey1Handler, KEY_SEND1, keyevent);
			SetKeyHandler(MTPNP_PFAL_CC_SendKey2Handler, KEY_SEND2, keyevent);
        }
        else if(call_state_1)
        {
            SetKeyHandler(MTPNP_PFAL_CC_SendKey1Handler, KEY_SEND1, keyevent);
			SetKeyHandler(MTPNP_PFAL_CC_SendKey1Handler, KEY_SEND2, keyevent);

        }
        else if(call_state_2)
        {
            SetKeyHandler(MTPNP_PFAL_CC_SendKey2Handler, KEY_SEND1, keyevent);
			SetKeyHandler(MTPNP_PFAL_CC_SendKey2Handler, KEY_SEND2, keyevent);
        }
		else
		{
        	SetKeyHandler(MTPNP_PFAL_CC_Sim1Invaild, KEY_SEND1, keyevent);
        	SetKeyHandler(MTPNP_PFAL_CC_Sim2Invaild, KEY_SEND2, keyevent);
		}
		#endif
#ifdef TGT_GALLITE_G800 
#if (NUMBER_OF_SIM > 2)    
	if (call_state_3)
	{
		SetKeyHandler(MTPNP_PFAL_CC_SendKey3Handler, KEY_LSK, keyevent);
	}
	else
	{
      //  	 MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SIM3);
		SetKeyHandler(MTPNP_PFAL_CC_Sim3Invaild, KEY_LSK, keyevent);
	}
#endif	
#endif
#ifdef TGT_TD_4SIM 
#if (NUMBER_OF_SIM > 2)    
	if (call_state_3)
	{
		SetKeyHandler(MTPNP_PFAL_CC_SendKey3Handler, KEY_LSK, keyevent);
	}
	else
	{
      //  	 MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SIM3);
		SetKeyHandler(MTPNP_PFAL_CC_Sim3Invaild, KEY_LSK, keyevent);
	}

	if (call_state_4)
	{
		SetKeyHandler(MTPNP_PFAL_CC_SendKey4Handler, KEY_CAMERA, keyevent);
	}
	else
	{
      //  	 MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SIM3);
		SetKeyHandler(MTPNP_PFAL_CC_Sim4Invaild, KEY_CAMERA, keyevent);
	}
#endif	
#endif
#if (NUMBER_OF_SIM == 2) 
    if ((!call_state_1) && (!call_state_2))
    {
        SetKeyHandler(MTPNP_PFAL_CC_SendKey1Handler, KEY_SEND1, keyevent);
		SetKeyHandler(MTPNP_PFAL_CC_SendKey2Handler, KEY_SEND2, keyevent);
    }
#endif
#endif /* __SENDKEY2_SUPPORT__ */

}

void MTPNP_PFAL_CC_SetMOCallFunc(FuncPtr funcname)
{
	mmi_ucm_set_sendkey_hdlr(funcname);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_set_sendkey_hdlr
 * DESCRIPTION
 *  set sendkey handler function 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_set_sendkey_hdlr(FuncPtr sendkey_hdlr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ucm_p->call_misc.sendkey_hdlr = sendkey_hdlr;
}


/*****************************************************
**function MTPNP_PFAL_CC_Send1KeyHandler
**to handle sendkey1 when it's function is to make a GSM call
******************************************************/
void MTPNP_PFAL_CC_SendKeyHandler(void)
{
	U8 nUsableSide;
	U8 UsableSideIndex;
	U8 StartUpMode;
	U8 i;

	kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKeyHandler MO_CALL_FUNCTION=%x", MTPNP_PFAL_CC_GetMOCallFunc());
	
	if (NULL == MTPNP_PFAL_CC_GetMOCallFunc())
	{
		kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKeyHandler NO MO_CALL_FUNCTION ");
		return;
	}
	
	nUsableSide = MTPNP_AD_Get_UsableSide_Number();
	UsableSideIndex = MTPNP_AD_GET_UsableSide_Index();
	StartUpMode = MTPNP_AD_Get_Startup_Mode();

	
	mmi_trace(g_sw_CC, "MTPNP_PFAL_CC_SendKeyHandler, nUsableSide is %d, UsableSideIndex is %d,StartUpMode is 0x%x", nUsableSide,UsableSideIndex,StartUpMode);
	
	if(isInCall())  //it there is a call, don't change the channel
	{
		MTPNP_PFAL_CC_ExeMOCallFunc();
	}
	else if (nUsableSide > 1)
	{     
		#ifdef __SENDKEY2_SUPPORT__
		MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
		MTPNP_PFAL_CC_ExeMOCallFunc();
		#else
		if (GetExitScrnID()== IDLE_SCREEN_DIGIT_HANDLER_ID)
		{
			if (!SSCStringParsing2() && g_idle_context.DialPadCallBuffer[0] != '\0')
			{
				mmi_ucm_entry_dial_option_by_sendkey();
			}
		}
		else
		{
			mmi_ucm_entry_dial_option_by_sendkey();
		}
		#endif
	}
	else if (nUsableSide == 1)
	{
		MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, UsableSideIndex);
		MTPNP_PFAL_CC_ExeMOCallFunc();
	}
	else
	{
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			if(StartUpMode&(DM_STATUS_MASK<<i))
				break;
		}
		MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_MASTER + i);

		MTPNP_PFAL_CC_ExeMOCallFunc();

		kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_CC_SendKeyHandler no sim to make call, use sim%d to call",i);
		return;
	}
}

FuncPtr MTPNP_PFAL_CC_GetMOCallFunc(void)
{
	return mmi_ucm_get_sendkey_hdlr();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_get_sendkey_hdlr
 * DESCRIPTION
 *  get sendkey handler function 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
FuncPtr mmi_ucm_get_sendkey_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_ucm_p->call_misc.sendkey_hdlr;
}

void MTPNP_PFAL_CC_ExeMOCallFunc(void)
{
    mmi_ucm_exec_sendkey_hdlr();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_exec_sendkey_hdlr
 * DESCRIPTION
 *  get sendkey handler function 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_exec_sendkey_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_ucm_p->call_misc.sendkey_hdlr != NULL)
    {
        g_ucm_p->call_misc.sendkey_hdlr();
    }
    else
    {
        MMI_ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_entry_dial_option_by_sendkey
 * DESCRIPTION
 *  General dial call type option by send key
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_entry_dial_option_by_sendkey(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ucm_p->call_misc.entry_type = MMI_UCM_ENTRY_SEND_KEY;

    mmi_ucm_dial_option_unhide_menu_item();
    mmi_ucm_entry_call_type();
 }


/**************************************************************

	FUNCTION NAME		: MTPNP_AD_PsCBackCallIncoming(void*)

  	PURPOSE				: This function is called back by the protocol stack when there
	                      is an incoming call

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MTPNP_AD_PsCBackCallIncoming (void* info)
{
	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL,MTPNP_AD_CHANNEL_SLAVE);
	ProcessIncomingEvents (CM_PS_CALL_INCOMING, info);
}



/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_check_gsm_number
 * DESCRIPTION
 *  Check if possible digit for gsm number
 * PARAMETERS
 *  strNumber       [IN]
 * RETURNS
 *  pBOOL
 *****************************************************************************/
MMI_BOOL mmi_ucm_check_gsm_number(S8 *strNumber)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 pos = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (strNumber[pos] != '\0')
    {
        if((strNumber[pos] == '+') ||
           (strNumber[pos] == '*') ||
           (strNumber[pos] == '#') ||
           (strNumber[pos] == 'p') ||
           (strNumber[pos] == 'w') ||
           (strNumber[pos] == '?') ||
           ((strNumber[pos] >= '0') && (strNumber[pos] <= '9')))
        {
            pos += 2;
        }
        else
        {
            return MMI_FALSE;
        }        
    }
	return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_app_entry_dial_option_internal
 * DESCRIPTION
 *  General dial call type option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_app_entry_dial_option_internal(U8 *num_uri, mmi_ucm_callback_func_ptr callback_after_dial, void* callback_para)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 unicodeLen = 0;
	U8* ascii_num = NULL;
	U8 i;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/    

	unicodeLen = (MMI_UCM_MAX_NUM_URI_LEN > mmi_ucs2strlen((S8*)num_uri)) ? (mmi_ucs2strlen((S8*)num_uri)) : (MMI_UCM_MAX_NUM_URI_LEN - 1);

	ascii_num = OslMalloc(unicodeLen + 1);
	memset(ascii_num, 0, unicodeLen + 1);
	mmi_ucs2_n_to_asc((S8*)ascii_num, (S8*)num_uri, unicodeLen * ENCODING_LENGTH);

	if (mmi_ucm_check_gsm_number((S8*)num_uri))
	{
		for( i = 0; i < MMI_SIM_NUMBER; i++)
		{
			mmi_frm_unhide_menu_item(MENU_ID_UCM_CALL_TYPE_VOICE + i);
		}
	#ifdef __MMI_VIDEO_TELEPHONY__
		mmi_frm_unhide_menu_item(MENU_ID_UCM_CALL_TYPE_VIDEO);
	#endif /* __MMI_VIDEO_TELEPHONY__ */    
	}        
	else
	{
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			mmi_frm_hide_menu_item(MENU_ID_UCM_CALL_TYPE_VOICE + i);
		}
	#ifdef __MMI_VIDEO_TELEPHONY__
		mmi_frm_hide_menu_item(MENU_ID_UCM_CALL_TYPE_VIDEO);
	#endif /* __MMI_VIDEO_TELEPHONY__ */    
	}

	MTPNP_AD_SetMenuHideUnhide(MENU_ID_UCM_CALL_TYPE_VOICE);


	if (CheckValidEmergencyNo1(num_uri))
	{        
		#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetMenuHideUnhide(MENU_ID_UCM_CALL_TYPE_VOICE);
		#else
		mmi_frm_unhide_menu_item(MENU_ID_UCM_CALL_TYPE_VOICE);
		#endif
	}

	OslMfree(ascii_num);

	/* No proper call type */
	if (mmi_bootup_get_active_flight_mode() ||
		GetNumOfChild_Ext(MENU_ID_UCM_CALL_TYPE) == 0)
	{
		DisplayPopup((U8*)GetString(STR_ID_UCM_NOT_ALLOW_TO_DIAL),
					IMG_GLOBAL_UNFINISHED,
					1,
					UI_POPUP_NOTIFYDURATION_TIME,
					ERROR_TONE);
					return;
	}

	DeleteScreenIfPresent(SCR_ID_UCM_DIAL_CALL_TYPE_MENU);
	g_ucm_p->mo_info.callback = callback_after_dial;
	g_ucm_p->mo_info.callback_para = callback_para;

	if (isInCall())
	{
		MakeCall(num_uri);
		return;
	}
	/* Dial call directly if only one call type */
	if (GetNumOfChild_Ext(MENU_ID_UCM_CALL_TYPE) == 1)
	{
		U16 call_menu_id = GetSeqItemId_Ext(MENU_ID_UCM_CALL_TYPE, 0);

		switch (call_menu_id)
		{
			case MENU_ID_UCM_CALL_TYPE_VOIP:
				mmi_ucm_app_make_call(MMI_UCM_VOIP_CALL_TYPE , num_uri);
				break;

			case MENU_ID_UCM_CALL_TYPE_VIDEO:
				mmi_ucm_app_make_call(MMI_UCM_VIDEO_CALL_TYPE , num_uri);
				break;

			case MENU_ID_UCM_CALL_TYPE_VOICE:
			case MENU_ID_UCM_CALL_TYPE_VOICE_SIM2:
			case MENU_ID_UCM_CALL_TYPE_VOICE_SIM3:
			case MENU_ID_UCM_CALL_TYPE_VOICE_SIM4:
				g_ucm_p->nSimID = call_menu_id - MENU_ID_UCM_CALL_TYPE_VOICE;
				mmi_ucm_app_make_call(MMI_UCM_VOICE_CALL_TYPE<<(call_menu_id - MENU_ID_UCM_CALL_TYPE_VOICE) , num_uri);
				break;
			default: 
				MMI_ASSERT(0); 
				break; 
		}         

		if (g_ucm_p->mo_info.callback != NULL)
		{
			g_ucm_p->mo_info.callback((void*)g_ucm_p->mo_info.callback_para);
		}
	}
	else
	{
		memset(g_ucm_p->mo_info.dial_num.num_uri, 0, MMI_UCM_MAX_NUM_URI_LEN);
		mmi_ucs2_n_to_asc((S8*)g_ucm_p->mo_info.dial_num.num_uri, (S8*)num_uri, unicodeLen * ENCODING_LENGTH);

		mmi_ucm_entry_call_type();
	}
}



/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_app_entry_dial_option
 * DESCRIPTION
 *  General dial call type option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_app_entry_dial_option(U8 *num_uri, mmi_ucm_callback_func_ptr callback_after_dial, void* callback_para)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ucm_p->call_misc.entry_type = MMI_UCM_ENTRY_MENU;
        
    mmi_ucm_app_entry_dial_option_internal(num_uri, callback_after_dial, callback_para);
}



/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_app_make_call
 * DESCRIPTION
 *  Sample code
 * PARAMETERS
 *  call_type       [IN]            Call type
 *  num_uri			[IN]			Number or URI in unicode string
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_app_make_call(mmi_ucm_call_type_enum call_type, U8 *num_uri)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/    
    g_ucm_p->mo_info.module_id = MMI_UCM_FROM_IDLE;
    g_ucm_p->mo_info.dial_type = call_type;
        
#if defined(__MMI_MULTI_SIM__)
    mmi_ucm_set_call_channel(g_ucm_p->nSimID);
#endif

	MakeCall(num_uri);


}

//////////////////////////////////////////////////////
//Call Setting


void SetCallSetContext(U8 nSimId)
{
	g_callset_cntx_p = &g_callset_context[nSimId];
	g_callset_cntx_p->nSimId = nSimId;
	mmi_trace(TRUE," Func: %s nSimId: %d", __FUNCTION__, nSimId);
	
}

void MTPNP_CallSetSetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID)
{
	SetProtocolEventHandlerExt( funcPtr, eventID,g_callset_cntx_p->nSimId);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_entry_call_general_setting
 * DESCRIPTION
 *  call setting sim1 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_entry_call_general_setting(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer = NULL;
	U16 numItems = 0;
	U16 nStrItemList[MAX_SUB_MENUS];
	U8 *popUpList[MAX_SUB_MENUS];
	#ifdef __MMI_NUMBER_MENU__
	#else	
	#ifdef WIN32 
						U16 ItemList[MAX_SUB_MENUS]; /* add 2 due to Java and Java Setting menu are added into resource but not in PC environment */
						U16 ItemIcons[MAX_SUB_MENUS];
	#else 
//						U16 ItemList[MAX_SUB_MENUS];
						U16 ItemIcons[MAX_SUB_MENUS];
	#endif /* WIN32 */
	#endif
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	if(GetNumOfChild_Ext(MENU_MTPNP_SIM1_CALL_SETTING + g_callset_cntx_p->nSimId) == 0)
	{	
		DisplayPopup((PU8) GetString(STR_GLOBAL_NOT_SUPPORTED), 
					IMG_GLOBAL_ERROR,
					1, 
					UI_POPUP_NOTIFYDURATION_TIME, 
					ERROR_TONE);	

		return;
	}

	EntryNewScreen(SCR_ID_UCM_GENERAL_CALL_SETTING, NULL, mmi_ucm_entry_call_general_setting, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_UCM_GENERAL_CALL_SETTING);

	numItems = GetNumOfChild_Ext(MENU_MTPNP_SIM1_CALL_SETTING);
	GetSequenceStringIds_Ext(MENU_MTPNP_SIM1_CALL_SETTING, nStrItemList);    
	#ifdef __MMI_NUMBER_MENU__
	#else
	GetSequenceImageIds_Ext(MENU_MTPNP_SIM1_CALL_SETTING,ItemIcons);
	#endif
	SetParentHandler(MENU_MTPNP_SIM1_CALL_SETTING);

	ConstructHintsList(MENU_MTPNP_SIM1_CALL_SETTING, popUpList);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	#ifdef __MMI_NUMBER_MENU__

    ShowCategory52Screen(
        STRING_MTPNP_CARD1_CALL_SETTING + g_callset_cntx_p->nSimId,
        GetRootTitleIcon(MENU8237_SCR8093_MNGCALL_MENU_MAIN),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        numItems,
        nStrItemList,
        (U16*) gIndexIconsImageList,
        (U8 **) popUpList,
        0,
        0,
        guiBuffer);
	#else
	    ShowCategory52Screen(
        STRING_MTPNP_CARD1_CALL_SETTING + g_callset_cntx_p->nSimId,
        GetRootTitleIcon(MENU8237_SCR8093_MNGCALL_MENU_MAIN),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        numItems,
        nStrItemList,
        ItemIcons,
        (U8 **) popUpList,
        0,
        0,
        guiBuffer);
	#endif

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    
}   


/*****************************************************************************
 * FUNCTION
 *  mmi_ucm_highlight_general_call_setting
 * DESCRIPTION
 *  Funtion is called when general call setting sim1 menu item selected
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ucm_highlight_general_call_setting(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    SetLeftSoftkeyFunction(mmi_ucm_entry_call_general_setting, KEY_EVENT_UP);
    SetKeyHandler(mmi_ucm_entry_call_general_setting, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    return;
}

void MTPNP_PFAL_CallSetting_Init(void)
{
	U8 i;
	/* hilite */
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MENU_MTPNP_SIM1_CALL_SETTING + i, mmi_ucm_highlight_general_call_setting);
	}
}


U16 GetCallSetLine(U8 nSimId)
{
	return g_callset_context[nSimId].LineID;
}
#if 1//def __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
MTPNP_BOOL MTPNP_PFAL_Retrieve_CallDial_State(MTPNP_UINT8 * m_dial_enable, MTPNP_UINT8 * s_dial_enable,MTPNP_UINT8 * t_dial_enable,MTPNP_UINT8 * f_dial_enable)
{
    if (m_dial_enable == MTPNP_NULL || s_dial_enable == MTPNP_NULL||t_dial_enable == MTPNP_NULL ||f_dial_enable == MTPNP_NULL )
    {
        return MTPNP_FALSE;
    }

    if(MTPNP_PFAL_Is_CardValid(0))
      	     *m_dial_enable = 1;
    else
      	     *m_dial_enable = 0;

     if(MTPNP_PFAL_Is_CardValid(1))
      	     *s_dial_enable = 1;
    else
      	     *s_dial_enable = 0;

    if(MTPNP_PFAL_Is_CardValid(2))
      	     *t_dial_enable = 1;
    else
      	     *t_dial_enable = 0;
    
       if(MTPNP_PFAL_Is_CardValid(3))
      	     *f_dial_enable = 1;
    else
      	     *f_dial_enable = 0;

    
	if (isInCall())
	{
		if (MTPNP_AD_IsMasterChannel(MTPNP_AD_CALL_CHANNEL))
		{
			*s_dial_enable = 0; // sim1 is in call, disable sim2 call
			*t_dial_enable = 0;
                     *f_dial_enable = 0;
		}
              else	if (MTPNP_AD_IsSlaveChannel(MTPNP_AD_CALL_CHANNEL))
		{
			*m_dial_enable = 0;
			*t_dial_enable = 0;
                     *f_dial_enable = 0;
		}
              else if (MTPNP_AD_IsThirdChannel(MTPNP_AD_CALL_CHANNEL))
              {
			*m_dial_enable = 0;
			*s_dial_enable = 0;
                     *f_dial_enable = 0;
              }
              else
              {
			*m_dial_enable = 0;
			*t_dial_enable = 0;
                     *s_dial_enable = 0;
              }
	}
    mmi_trace(g_sw_CC, "MMI_Call: MTPNP_PFAL_Retrieve_CallDial_State:m_dial_enable = %d ,s_dial_enable = %d,t_dial_enable = %d\n,",*m_dial_enable,*s_dial_enable,*t_dial_enable);
    return MTPNP_TRUE;
}

#endif /* #if defined(__MMI_BLACKBERRY_QWERTY__)||defined(__DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__)*/

#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
/*****************************************************************************
 * FUNCTION
 *  mmi_idle_set_dual_sim_dialing_keypad_call_handler
 * DESCRIPTION
 *  Dual SIM: Set dialing call handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__
 extern void SetDialingKeypadCall2Handler(void (*handler) (void));
 extern void SetDialingKeypadCall3Handler(void (*handler) (void));
 extern void SetDialingKeypadCall4Handler(void (*handler) (void));
 #endif
void mmi_idle_set_dual_sim_dialing_keypad_call_handler(FuncPtr sim1_func, FuncPtr sim2_func,FuncPtr sim3_func,FuncPtr sim4_func)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 call_icon_1 = 0, call_icon_2 = 0;
    U8 call_icon_3 = 0;
    U8 call_icon_4=0;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MTPNP_PFAL_Retrieve_CallDial_State(&call_icon_1, &call_icon_2,&call_icon_3,&call_icon_4);
#ifdef __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__
    if (call_icon_1)
    {
        SetDialingKeypadCallHandler(sim1_func);
    }
    if (call_icon_2)
    {
	    SetDialingKeypadCall2Handler(sim2_func);
	}
    if (call_icon_3)
    {
	    SetDialingKeypadCall3Handler(sim3_func);
	}
    if (call_icon_4)
    {
	    SetDialingKeypadCall4Handler(sim4_func);
	}
#endif
}


#endif /* __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__ */
/*****************************************************************************
 * FUNCTION
 *  mmi_sim_error_set_emergency_call_path
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 extern S8 nIndexSimRemoved;
void mmi_sim_error_set_emergency_call_path(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	//    E_MTPNP_AD_CHANNELS call_channel;
	E_MTPNP_AD_NVRAM_STATUS dmStatus;
	U8 i;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	dmStatus = MTPNP_AD_Get_Startup_Mode();
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(dmStatus&(DM_STATUS_MASK<<i))
			break;
	}
	if(nIndexSimRemoved != -1)
	{
		MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_MASTER + nIndexSimRemoved);
		return;
	}
	switch(i)
	{
		case 0:
			MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_MASTER);
			break;
		case 1:
			MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_SLAVE);
			break;
		case 2:
			MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_SIM3);
			break;
		case 3:
			MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_SIM4);
			break;
		default:
			break;

	}
}

#endif

